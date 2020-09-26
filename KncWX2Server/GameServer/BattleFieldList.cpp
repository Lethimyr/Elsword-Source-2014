#include "BaseServer.h"
#include ".\battlefieldlist.h"
#include "Enum\Enum.h"
#include "X2Data\XSLRoom.h"
#include "Lottery.h"
#include "NetError.h"


//{{ 2011. 11. 2	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldList::KBattleFieldList()
{
}

KBattleFieldList::~KBattleFieldList(void)
{
}

KGSBattleFieldRoomPtr KBattleFieldList::GetRoomInfo( IN const UidType iRoomUID, IN const bool bErrorLog /*= false*/ )
{
	std::map< UidType, KGSBattleFieldRoomPtr >::iterator mit;
	mit = m_mapList.find( iRoomUID );
	if( mit == m_mapList.end() )
	{
		if( bErrorLog )
		{
			START_LOG( cerr, L"방 정보가 존재하지 않습니다." )
				<< BUILD_LOG( iRoomUID )
				<< END_LOG;
		}

		return KGSBattleFieldRoomPtr();
	}

	return mit->second;
}

bool KBattleFieldList::AddRoomInfo( IN const KBattleFieldRoomInfo& kInfo )
{
	if( IsExist( kInfo.m_RoomUID ) == true )
	{
		START_LOG( cerr, L"이미 존재하는 방입니다" )
			<< BUILD_LOG( kInfo.m_RoomUID )
			<< END_LOG;
		return false;
	}

	KGSBattleFieldRoomPtr spRoomInfo( new KGSBattleFieldRoom( kInfo ) );
	if( spRoomInfo == NULL )
	{
		START_LOG( cerr, L"방정보 포인터값 생성 실패!" )
			<< BUILD_LOG( kInfo.m_iBattleFieldID )
			<< END_LOG;
		return false;
	}

	m_mapList.insert( std::make_pair( kInfo.m_RoomUID, spRoomInfo ) );
	m_vecList.push_back( spRoomInfo );
	return true;
}

bool KBattleFieldList::DelRoomInfo( IN const UidType iRoomUID )
{
	if( IsExist( iRoomUID ) == false )
	{
		START_LOG( cerr, L"없는 방을 지우려고 하였습니다" )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;
		return false;
	}

	m_mapList.erase( iRoomUID );

	std::vector< KGSBattleFieldRoomPtr >::iterator vitDEL;
	for( vitDEL = m_vecList.begin(); vitDEL != m_vecList.end(); ++vitDEL )
	{
        if( (*vitDEL) == NULL )
		{
			START_LOG( cerr, L"방 포인터값이 이상합니다!" )
				<< BUILD_LOG( iRoomUID )
				<< END_LOG;
			continue;
		}

		if( (*vitDEL)->GetRoomUID() == iRoomUID )
			break;
	}

	if( vitDEL != m_vecList.end() )
	{
		m_vecList.erase( vitDEL );
	}

	return true;
}

//{{ 2012. 11. 20	최육사		배틀필드 방 정리 기능 추가
#ifdef SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
void KBattleFieldList::DelRoomInfoByCnServerUID( IN const UidType iCnServerUID )
{
	std::vector< UidType > vecDeleteRoomUID;

	BOOST_TEST_FOREACH( const KGSBattleFieldRoomPtr, spGSBattleFieldRoom, m_vecList )
	{
		if( IS_NULL( spGSBattleFieldRoom ) )
		{
			START_LOG( cerr, L"방 포인터값이 이상합니다!" )
				<< BUILD_LOG( iCnServerUID )
				<< END_LOG;
			continue;
		}

		// 해당 센터에서 만들어진 배틀필드가 맞는지 확인!
		if( KncUid::ExtractServerID( spGSBattleFieldRoom->GetRoomUID() ) == iCnServerUID )
		{
            vecDeleteRoomUID.push_back( spGSBattleFieldRoom->GetRoomUID() );
		}
	}

	BOOST_TEST_FOREACH( const UidType, iRoomUID, vecDeleteRoomUID )
	{
		LIF( DelRoomInfo( iRoomUID ) );
	}
}
#endif SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
//}}

bool KBattleFieldList::UpdateRoomInfo( IN const KBattleFieldRoomInfo& kInfo )
{
    KGSBattleFieldRoomPtr spRoomInfo = GetRoomInfo( kInfo.m_RoomUID );
	if( spRoomInfo == NULL )
	{
		START_LOG( cerr, L"방 정보 포인터값이 이상합니다" )
			<< BUILD_LOG( kInfo.m_RoomUID )
			<< END_LOG;
		return false;
	}

	(*spRoomInfo) = kInfo;
	return true;
}

UidType KBattleFieldList::GetBattleFieldRoomUID_MaxPartyMemberCount( IN const UidType iUnitUID, IN const UidType iPartyUID )
{
	if( iPartyUID == 0 )
	{
		START_LOG( cerr, L"파티UID값이 0이다. 이런건 미리 검사 해야지!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return 0;
	}

	UidType iSelectedRoomUID = 0;
	int iMaxPartyMemberCount = 0;

	// 모든 필드에 해당 파티원이 가장 많은 필드를 선정하자
	BOOST_TEST_FOREACH( const KGSBattleFieldRoomPtr, spRoomInfo, m_vecList )
	{
		if( IS_NULL( spRoomInfo ) )
		{
			START_LOG( cerr, L"방 포인터값이 이상합니다" )
				<< END_LOG;
			continue;
		}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-06	// 박세훈
		if( SEnum::CheckFlag( spRoomInfo->GetBossFieldState(), ( SEnum::BFRS_BOSS_FIELD | SEnum::BFRS_INTRUDE_RESTRICTION ) ) == true )
			continue;
#endif // SERV_BATTLE_FIELD_BOSS

		// 해당 배틀필드에 입장해있는 같은 파티원이 있는지 확인!
		if( spRoomInfo->IsExistParty( iPartyUID ) == false )
			continue;

		int iPartyMemberCount = 0;

		// 해당 필드에 자기 자신이 입장해있는지 확인!
		if( spRoomInfo->IsJoinUser( iUnitUID ) == true )
		{
			// 실제로 입장 가능한 상태인지 확인!
			if( ( spRoomInfo->GetJoinUserCount() - 1 ) >= spRoomInfo->GetMaxUserCount() )
				continue;

			// 이 필드에 입장한 파티원은 몇명인가?
			iPartyMemberCount = ( spRoomInfo->GetPartyMemberCountByPartyUID( iPartyUID ) - 1 );
			if( iMaxPartyMemberCount >= iPartyMemberCount )
				continue;
		}
		else
		{
			// 실제로 입장 가능한 상태인지 확인! ( 슬롯이 비어있어야함 )
			if( spRoomInfo->GetJoinUserCount() >= spRoomInfo->GetMaxUserCount() )
				continue;

			// 이 필드에 입장한 파티원은 몇명인가?
			iPartyMemberCount = spRoomInfo->GetPartyMemberCountByPartyUID( iPartyUID );
			if( iMaxPartyMemberCount >= iPartyMemberCount )
				continue;
		}

		iSelectedRoomUID = spRoomInfo->GetRoomUID();
		iMaxPartyMemberCount = iPartyMemberCount;
	}

	return iSelectedRoomUID;
}

UidType KBattleFieldList::GetBattleFieldRoomUID_TargetRoom( IN const UidType iTargetRoomUID )
{
	if( iTargetRoomUID == 0 )
	{
		return 0;
	}

	if( CXSLRoom::GetRoomType( iTargetRoomUID ) != CXSLRoom::RT_BATTLE_FIELD )
	{
		// 목표되는 방이 배틀필드가 아니다.
		return 0;
	}

	KGSBattleFieldRoomPtr spBeforeRoomInfo = GetRoomInfo( iTargetRoomUID );
	if( IS_NULL( spBeforeRoomInfo ) )
	{
		// 방이 존재하지 않는다. 시간차로 사라졌을수 있다.
        return 0;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-06	// 박세훈
	if( SEnum::CheckFlag( spBeforeRoomInfo->GetBossFieldState(), ( SEnum::BFRS_BOSS_FIELD | SEnum::BFRS_INTRUDE_RESTRICTION ) ) == true )
		return 0;
#endif // SERV_BATTLE_FIELD_BOSS

	if( spBeforeRoomInfo->GetJoinUserCountWithReservedUserCount() >= spBeforeRoomInfo->GetMaxUserCount() )
	{
		// 입장할 수 있을만큼 빈 자리가 없다.
		return 0;
	}

	return spBeforeRoomInfo->GetRoomUID();
}

UidType KBattleFieldList::GetBattleFieldRoomUID_AllRandom()
{
	KLotterySameProb< UidType > kLotRandomBF;

	// 그냥 다른사람들이 있는 방을 하나 골라 주자!
	BOOST_TEST_FOREACH( const KGSBattleFieldRoomPtr, spRoomInfo, m_vecList )
	{
		if( IS_NULL( spRoomInfo ) )
		{
			START_LOG( cerr, L"방 포인터값이 이상합니다" )
				<< END_LOG;
			continue;
		}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-06	// 박세훈
		if( SEnum::CheckFlag( spRoomInfo->GetBossFieldState(), ( SEnum::BFRS_BOSS_FIELD | SEnum::BFRS_INTRUDE_RESTRICTION ) ) == true )
			continue;
#endif // SERV_BATTLE_FIELD_BOSS

		// 실제 방안에 있는 유저 + 그 유저가 파티일 경우 해당 소속 파티원 수의 합산
		if( spRoomInfo->GetJoinUserCountWithReservedUserCount() >= spRoomInfo->GetMaxUserCount() )
			continue;

		// 동일한 확률로 랜덤을 굴리기 위해 case를 저장한다.
		kLotRandomBF.AddCaseSameProb( spRoomInfo->GetRoomUID() );
	}

	if( kLotRandomBF.Size() == 0 )
	{
		// 랜덤을 굴릴만한 방이 전혀 없다.
		return 0;
	}

	// 랜덤하게 입장할 배틀필드를 선택한다!
	const UidType iJoinBFRoomUID = kLotRandomBF.DecisionSameProb();
	if( iJoinBFRoomUID == kLotRandomBF.CASE_BLANK )
	{
		// 랜덤 결과값이 BLANK로 나왔다.
		return 0;
	}

	return iJoinBFRoomUID;
}

bool KBattleFieldList::GetRoomUIDForJoinBattleField( IN const UidType iUnitUID, IN const UidType iTargetRoomUID, IN const UidType iPartyUID, OUT UidType& iJoinBFRoomUID )
{
	SET_ERROR( NET_OK );

	iJoinBFRoomUID = 0;

	// 파티 소속일 경우
	if( iPartyUID != 0 )
	{
		// 가장 파티원이 많은 방을 찾는다!
		iJoinBFRoomUID = GetBattleFieldRoomUID_MaxPartyMemberCount( iUnitUID, iPartyUID );
	}

	// 아직도 입장할 필드가 정해지지 않았다면
	if( iJoinBFRoomUID == 0 )
	{
		// 기존에 입장했었던 방에 다시 입장하기 적절한지 본다.
		iJoinBFRoomUID = GetBattleFieldRoomUID_TargetRoom( iTargetRoomUID );
	}
		
	// 아직까지도 입장할 배틀필드가 정해지지 않았다면
	if( iJoinBFRoomUID == 0 )
	{
		// 다른 필드들을 모두 검색해서 적절한 곳을 찾자!
        iJoinBFRoomUID = GetBattleFieldRoomUID_AllRandom();
	}

	return true;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}


