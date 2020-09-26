#include "BattleFieldUserManager.h"
#include "Enum/Enum.h"
#include "X2Data/XSLDungeon.h"
#include "X2Data/XSLTitleManager.h"
#include "ResultProcess.h"
#include "NetError.h"
#include "X2Data/XSLBattleFieldManager.h"
#include "Room/RoomUserFSM.h"


//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldUserManager::KBattleFieldUserManager() :
m_MaxSlot( SEnum::BATTLE_FIELD_MAX_NUM )
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
, m_bIsCheckZombieAlert( false )
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
{
}

KBattleFieldUserManager::~KBattleFieldUserManager()
{
}

//{{ 2013. 01. 07	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
void KBattleFieldUserManager::Reset( USERLIST_TYPE eType /*= UT_GAME*/ )
{
	KRoomUserManager::Reset( eType );

	m_mapPartyList.clear();
}
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
//}}

bool KBattleFieldUserManager::EnterRoom( IN const KRoomUserInfo& kInfo, IN const KBattleFieldJoinInfo& kJoinInfo, IN const bool bConsiderTeam )
{
	SET_ERROR( NET_OK );

	std::vector< UidType > vecUnitUIDList;

	// 이미 입장해있는 유저인지 체크!
	if( IsExist( kInfo.m_nUnitUID ) == true )
	{
		START_LOG( cerr, L"이미 입장해있는 유저 입니다!" )
			<< BUILD_LOG( kInfo.m_nUnitUID )
			<< BUILD_LOG( kInfo.m_iPartyUID )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_01 );
		return false;
	}

	// 실제로 입장할 슬롯 여유가 있는지 확인! ( 빈슬롯이 없으면 예약도 소용없다! )
	if( IsExistEmptySlot() == false )
	{
		SET_ERROR( ERR_BATTLEFIELD_02 );
		return false;
	}

	// 현재 같은 파티원이 이 필드에 있어서 예약된 유저인지 확인! ( 같은 파티원이 해당 필드에 있으면 입장 가능하다! )
	if( IsReservedPartyMember( kInfo.m_nUnitUID ) == false )
	{
		// 현재 배틀필드에 입장한 유저들의 파티원수 총합을 가지고 입장 가능 여부를 체크!
		if( GetNumMemberTotalParty() >= GetMaxSlot() )
		{
			// [memo] 시간차로 발생할수 있는 문제. cerr를 cwarn으로 낮출까..? ( 최육사 )

			START_LOG( cerr, L"현재 방에 입장할 공간이 없다!방에 입장 처리 실패!" )
				<< BUILD_LOG( kInfo.m_nUnitUID )
				<< BUILD_LOG( kInfo.m_iPartyUID )				
				<< BUILD_LOG( GetMaxSlot() )
				<< BUILD_LOG( GetNumMember() )
				<< BUILD_LOG( GetNumMemberTotalParty() )				
				<< END_LOG;

			SET_ERROR( ERR_BATTLEFIELD_03 );
			return false;
		}
	}

	// 실제 입장 처리 한다!
	if( KRoomUserManager::EnterRoom( kInfo, bConsiderTeam ) == false )
	{
		START_LOG( cerr, L"방에 입장 처리 실패!" )
			<< BUILD_LOG( kInfo.m_nUnitUID )
			<< BUILD_LOG( kInfo.m_iPartyUID )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_04 );
		return false;
	}
		
	// 룸유저에 입장 성공했다면 배틀필드 파티 리스트에도 추가하자!
	if( AddRoomUserInBattleFieldPartyList( kInfo.m_nUnitUID, kInfo.m_iPartyUID, kJoinInfo.m_vecPartyMemberList ) == false )
	{
		START_LOG( cerr, L"파티 멤버 정보 추가 실패!" )
			<< BUILD_LOG( kInfo.m_nUnitUID )
			<< BUILD_LOG( kInfo.m_iPartyUID )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_04 );
		goto end_proc;
	}
	
	GetUnitUIDList( vecUnitUIDList, kInfo.m_nUnitUID );

	if( vecUnitUIDList.empty() == false )
	{
		if( RegIntrudeUnitLoadUserList( kInfo.m_nUnitUID, vecUnitUIDList ) == false )
		{
			START_LOG( cerr, L"유저 정보 로드 대기자 리스트 등록 실패!" )
				<< BUILD_LOG( kInfo.m_nUnitUID )
				<< BUILD_LOG( vecUnitUIDList.size() )
				<< END_LOG;

			SET_ERROR( ERR_BATTLEFIELD_04 );
			goto end_proc;
		}
	}

	return true;

end_proc:
	// 이탈 처리 합니다!
	LIF( KRoomUserManager::LeaveRoom( kInfo.m_nUnitUID ) );
	return false;
}

bool KBattleFieldUserManager::LeaveRoom( IN const UidType iUnitUID )
{
	if( DelRoomUserInBattleFieldPartyList( iUnitUID ) == false )
	{
		START_LOG( cerr, L"파티 멤버 정보 삭제 실패!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
	}

	return KRoomUserManager::LeaveRoom( iUnitUID );
}

void KBattleFieldUserManager::GetAllPartyMemberListInfo( OUT std::map< UidType, UidType >& mapUnitUIDPartyUID, OUT std::map< UidType, std::set< UidType > >& mapAllPartyMemberList )
{
	mapUnitUIDPartyUID.clear();
	mapAllPartyMemberList.clear();

	// 현재 입장한 유저의 UnitUID정보와 PartyUID정보를 얻는다.
	GetUnitUIDByPartyUIDList( mapUnitUIDPartyUID );
    
	// 파티 리스트 얻기
	std::map< UidType, KBattleFieldParty >::const_iterator mit;
	for( mit = m_mapPartyList.begin(); mit != m_mapPartyList.end(); ++mit )
	{
		mapAllPartyMemberList.insert( std::make_pair( mit->first, mit->second.GetPartyMemberList() ) );
	}

	// 검사를 한번 하자!
	{
		std::map< UidType, UidType >::const_iterator mit;
		for( mit = mapUnitUIDPartyUID.begin(); mit != mapUnitUIDPartyUID.end(); ++mit )
		{
			const UidType iUnitUID = mit->first;
			const UidType iPartyUID = mit->second;

			if( iPartyUID == 0 )
				continue;

			std::map< UidType, std::set< UidType > >::const_iterator mitPL;
			mitPL = mapAllPartyMemberList.find( iPartyUID );
			if( mitPL == mapAllPartyMemberList.end() )
			{
				// 에러로그가 너무 많이찍혀서 로그 레벨을 낮춘다.
				START_LOG( cwarn, L"파티UID값은 있는데 파티 리스트에는 해당 파티 정보가 없다? 일어나서는 안되는 에러!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( iPartyUID )
					<< END_LOG;
				continue;
			}

			const std::set< UidType >& setPartyMemberList = mitPL->second;
			if( setPartyMemberList.find( iUnitUID ) == setPartyMemberList.end() )
			{
				// 에러로그가 너무 많이찍혀서 로그 레벨을 낮춘다.
				START_LOG( cwarn, L"파티 리스트에 파티 정보는 있는데 해당 유저가 파티 소속이 아니다? 일어나서는 안되는 에러!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( iPartyUID )
					<< BUILD_LOG( setPartyMemberList.size() )
					<< END_LOG;
				continue;
			}
		}
	}	
}

bool KBattleFieldUserManager::GetPartyMemberListInfo( IN const UidType iPartyUID, OUT std::vector< UidType >& vecPartyMemberList )
{
	vecPartyMemberList.clear();

	std::map< UidType, KBattleFieldParty >::const_iterator mit;
	mit = m_mapPartyList.find( iPartyUID );
	if( mit == m_mapPartyList.end() )
	{
		START_LOG( cerr, L"파티를 찾지 못하였습니다!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return false;
	}

	mit->second.GetPartyMemberList( vecPartyMemberList );
	return true;
}

bool KBattleFieldUserManager::AddRoomUserInBattleFieldPartyList( IN const UidType iUnitUID, IN const UidType iPartyUID, IN const std::vector< UidType >& vecPartyMemberList )
{
	if( iPartyUID == 0 )
		return true;

	// 이미 이 필드에 입장한 파티원이 있는지 찾아보기!
	std::map< UidType, KBattleFieldParty >::iterator mit;
	mit = m_mapPartyList.find( iPartyUID );
	if( mit == m_mapPartyList.end() )
	{
		// 없다면 파티 정보 저장하자!
		KBattleFieldParty kParty;
		kParty.UpdatePartyMemberList( vecPartyMemberList );
		m_mapPartyList.insert( std::make_pair( iPartyUID, kParty ) );
		return true;
	}

	if( mit->second.IsPartyMember( iUnitUID ) == false )
	{
		START_LOG( cerr, L"예약된 유저가 아닌데 파티원 정보를 추가하려고 한다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KBattleFieldUserManager::DelRoomUserInBattleFieldPartyList( IN const UidType iUnitUID )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"유저가 존재하지 않는데 지울려고 했음!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	if( spRoomUser->GetPartyUID() == 0 )
		return true;

	std::map< UidType, KBattleFieldParty >::iterator mitP;
	mitP = m_mapPartyList.find( spRoomUser->GetPartyUID() );
	if( mitP == m_mapPartyList.end() )
	{
		START_LOG( cwarn, L"해당 파티 정보가 존재하지 않습니다!" )
			<< BUILD_LOG( spRoomUser->GetPartyUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		// 어차피 지울 파티정보였으니 true를 반환해서 정상적으로 이탈 처리하게 하자!
		return true;
	}
	
	BOOST_TEST_FOREACH( const UidType, iMemberUnitUID, mitP->second.GetPartyMemberList() )
	{
		if( iMemberUnitUID == iUnitUID )
			continue;

		// 파티원중에 다른 파티원이 현재 이 배틀필드 소속인지 확인하자!		
		if( IsExist( iMemberUnitUID ) == true )
		{
			// 소속이 맞다면 배틀필드 파티정보는 지우지 않아도 된다!
			return true;
		}
	}

	// 이 유저가 소속된 모든 파티원이 이탈하면 파티 정보도 같이 지워야 한다!
    m_mapPartyList.erase( mitP );
	return true;
}

int KBattleFieldUserManager::GetNumMemberTotalParty() const
{
	int iNumMemberTotalParty = 0;

	std::map< UidType, KBattleFieldParty >::const_iterator mit;
	for( mit = m_mapPartyList.begin(); mit != m_mapPartyList.end(); ++mit )
	{
		iNumMemberTotalParty += mit->second.GetNumMember();
	}

	return iNumMemberTotalParty;
}

bool KBattleFieldUserManager::IsReservedPartyMember( IN const UidType iUnitUID )
{
	std::map< UidType, KBattleFieldParty >::const_iterator mit;
	for( mit = m_mapPartyList.begin(); mit != m_mapPartyList.end(); ++mit )
	{
		if( mit->second.IsPartyMember( iUnitUID ) == true )
			return true;
	}

	return false;
}

bool KBattleFieldUserManager::IsPartyMembersSameGuild( IN const UidType iUnitUID )
{
	int iGuildUID = 0;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	if( spRoomUser->GetPartyUID() == 0 )
		return false;

	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	if( iGuildUID == 0 )
		return false;

	iGuildUID = spRoomUser->GetGuildUID();

	USERLIST_TYPE eType = UT_GAME;
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		// 동일 데이터는 통과(자기자신과 검사)
		if( mit->second->GetCID() == iUnitUID )
			continue;

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		if( mit->second->GetGuildUID() != iGuildUID )
			return false;
	}

	return true;
#else

	USERLIST_TYPE eType = UT_GAME;
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		if( mit->second->GetGuildUID() == 0 )
			return false;

		if( iGuildUID == 0 )
		{
			iGuildUID = mit->second->GetGuildUID();
			continue;
		}

		if( iGuildUID != mit->second->GetGuildUID() )
			return false;
	}

	return true;
#endif SERV_REFORM_ITEM_DROP
	//}}
}

bool KBattleFieldUserManager::RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"해당 유저를 찾을수가 없습니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const UidType, iRegUnitUID, vecUnitUIDList )
	{
		spRoomUser->RegIntrudeUnitLoadUser( iRegUnitUID );
	}

	return true;
}

bool KBattleFieldUserManager::RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iRegUnitUID )
{
	std::vector< UidType > vecReg;
	vecReg.push_back( iRegUnitUID );
    return RegIntrudeUnitLoadUserList( iUnitUID, vecReg );
}

bool KBattleFieldUserManager::UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"해당 유저를 찾을수가 없습니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const UidType, iRegUnitUID, vecUnitUIDList )
	{
		spRoomUser->UnregIntrudeUnitLoadUser( iRegUnitUID );
	}

	return true;
}

bool KBattleFieldUserManager::UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iUnregUnitUID )
{
	std::vector< UidType > vecUnreg;
	vecUnreg.push_back( iUnregUnitUID );
	return UnregIntrudeUnitLoadUserList( iUnitUID, vecUnreg );
}

void KBattleFieldUserManager::CheckAndGetIntrudeUnitLoadCompleteUserList( OUT std::vector< UidType >& vecNpcLoadUserList )
{
	vecNpcLoadUserList.clear();

	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin() ; mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// 게임로딩이 모두 완료되었는지 체크!
		if( spRoomUser->GetLoadingProgress() < 100 )
			continue;

		// 난입자 로딩이 모두 완료되었는지 체크!
		if( spRoomUser->IsEmptyIntrudeUnitLoadUserList() == false )
			continue;

		// 난입자가 Npc로딩을 한적이 있는지 체크!
		if( spRoomUser->IsBattleFieldNpcLoad() == true )
			continue;

		// 몬스터 정보 보낼 대상자 리스트에 넣자!
		vecNpcLoadUserList.push_back( spRoomUser->GetCID() );

		// 플래그값 변경!
		spRoomUser->SetBattleFieldNpcLoad( true );
	}
}

int	 KBattleFieldUserManager::GetSamePartyMember( IN const UidType iUnitUID, USERLIST_TYPE eType /*= UT_GAME*/ )
{
	int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return 1;
	}

	if( spRoomUser->GetPartyUID() == 0 )
		return 1;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		++iCount;
	}

	if( iCount <= 0 )
		iCount = 1;

	return iCount;
}

int KBattleFieldUserManager::GetSamePartyLiveMember( IN const UidType iUnitUID, USERLIST_TYPE eType /*= UT_GAME*/ )
{
	int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return 1;
	}

	if( spRoomUser->GetPartyUID() == 0 )
		return 1;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsDie() == true )
			continue;

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		++iCount;
	}

	if( iCount <= 0 )
		iCount = 1;

	return iCount;
}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
bool KBattleFieldUserManager::GetRoomUserList( std::map< UidType, KRoomUserPtr >& mapRoomUserList, USERLIST_TYPE eType /*= UT_GAME*/ )
{
	if( CheckUserlistType( eType ) == false )
		return false;

	mapRoomUserList.clear();

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mapRoomUserList.insert( std::make_pair( mit->first, mit->second ) );
	}

	if( mapRoomUserList.size() <= 0 )
		return false;

	return true;
}

bool KBattleFieldUserManager::AddBattleFieldRewardEXP( IN const UidType iUnitUID, IN const float fLevelFactor, IN const int iExp, OUT KERM_NPC_UNIT_DIE_NOT& kInfo )
{
	USERLIST_TYPE eType = UT_GAME;

	// 레벨 조건에 의해서 경험치를 습득 못한 경우
	if( iExp <= 0 && fLevelFactor <= 0.f )
	{
		// 자신이 습득한 경험치가 0이라고 알려줘야 한다.
		KEXPData kEXPData;
		kEXPData.m_iEXP = 0;
		kEXPData.m_iPartyEXP = 0;
		kEXPData.m_iSocketOptEXP = 0;

		kInfo.m_EXPList.insert( std::make_pair( iUnitUID, kEXPData ) );

		return true;
	}
	else if( iExp <= 0 )
	{
		return false;
	}

	// EXP 지금 유저 찾기
	KRoomUserPtr spExpRewardRoomUser = GetUser( iUnitUID );
	if( IS_NULL( spExpRewardRoomUser ) )
	{
		START_LOG( cerr, L"존재하지 않는 유저에게 exp 를 지급하려 한다." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iExp )
			<< END_LOG;
		return false;
	}
			
	const int iLivePartyMember = GetSamePartyLiveMember( iUnitUID );

	//////////////////////////////////////////////////////////////////////////
	// 경험치 보상 처리
	//////////////////////////////////////////////////////////////////////////
	
	// 몬스터 레벨 보정 상수
	AddHaveExpInDungeon( spExpRewardRoomUser->GetCID(), fLevelFactor > 0.0f );
	
	const int iBattleFieldPartyBonusEXP = static_cast<int>( iExp * GetBattleFieldPartyBonusRate( iUnitUID ) );


	// GSUser에 업데이트하고 클라이언트 화면에 뿌릴 EXP 데이터
	KEXPData kEXPData;
	kEXPData.m_iEXP			= iExp;	// 이미 기본 경험치는 레벨 상수를 곱했었다.
	kEXPData.m_iPartyEXP	= iBattleFieldPartyBonusEXP;

	//////////////////////////////////////////////////////////////////////////		
	// 소켓옵션 보너스 적용
	bool bSocketOptBonus = false;
	int iSocketOptEXP		= 0;
	int iSocketOptPartyEXP	= 0;

	const float fAddSocketOptExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_EXP_RATE );
	if( fAddSocketOptExpRate > 0.0f )
	{
		iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddSocketOptExpRate );
		iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddSocketOptExpRate );

		bSocketOptBonus = true;
	}

	// 같은 파티원이 2명이상!
	if( iLivePartyMember >= 2 )
	{
		// 같은 파티원이 같은 길드원이면?
		if( IsPartyMembersSameGuild( iUnitUID ) )
		{
			const float fAddGuildBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_GUILD_BONUS_EXP_RATE );
			if( fAddGuildBonusExpRate > 0.0f )
			{
				iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddGuildBonusExpRate );
				iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddGuildBonusExpRate );

				bSocketOptBonus = true;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////		
	// 채널 보너스
	const float fAddChannelBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_CHANNEL_BONUS_EXP_RATE );
	if( fAddChannelBonusExpRate > 0.0f )
	{
		iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddChannelBonusExpRate );
		iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddChannelBonusExpRate );

		bSocketOptBonus = true;
	}

	// 소켓옵션 보너스
	kEXPData.m_iSocketOptEXP	 = iSocketOptEXP + iSocketOptPartyEXP;

	// 소켓옵션 보너스
	if( bSocketOptBonus == true  &&  kEXPData.m_iSocketOptEXP == 0  &&  fLevelFactor > 0.0f )
	{
		kEXPData.m_iSocketOptEXP = iSocketOptEXP = 1; // 보너스 경험치 최소값 1주기
	}

	//{{ 2012. 12. 18	최육사	필드에서도 경험치 이벤트 적용
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
	// 이벤트 경험치 보너스
	const float fAddEventBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP );
	if( fAddEventBonusExpRate > 0.f )
	{
		const int iBaseEXP = ( kEXPData.m_iEXP + kEXPData.m_iPartyEXP + kEXPData.m_iSocketOptEXP );
        kEXPData.m_iEventBonusEXP = static_cast<int>( iBaseEXP * fAddEventBonusExpRate );
	}
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
	//}}

	// 로그 기록을 위한 획득 경험치 정보 저장
	spExpRewardRoomUser->AddRewardEXP( kEXPData.SumEXP() );

	//개인별 EXP 처리..
	std::map< UidType, KEXPData >::iterator mitTemp;
	mitTemp = kInfo.m_EXPList.find( spExpRewardRoomUser->GetCID() );
	if( mitTemp == kInfo.m_EXPList.end() )
	{
		kInfo.m_EXPList.insert( std::make_pair( spExpRewardRoomUser->GetCID(), kEXPData ) );
	}
	else
	{
		mitTemp->second.m_iEXP += kEXPData.m_iEXP;
		mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
		mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
		//{{ 2012. 12. 18	최육사	필드에서도 경험치 이벤트 적용
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		mitTemp->second.m_iEventBonusEXP += kEXPData.m_iEventBonusEXP;
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		//}}
	}

	START_LOG( clog, L"::: 내가 받은 EXP :::" )
		<< BUILD_LOG( spExpRewardRoomUser->GetCID() )
		<< BUILD_LOG( kEXPData.m_iEXP )
		<< BUILD_LOG( kEXPData.m_iPartyEXP )
		<< BUILD_LOG( kEXPData.m_iSocketOptEXP )
		//{{ 2012. 12. 18	최육사	필드에서도 경험치 이벤트 적용
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		<< BUILD_LOG( kEXPData.m_iEventBonusEXP )
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		//}}
		;

	return true;
}

#else		// SERV_REFORM_ITEM_DROP

//bool KBattleFieldUserManager::AddBattleFieldRewardEXP( IN const char cNpcLevel,
//													  IN const char cMonsterTypeFactor,
//													  IN const int iNpcMaxHP,
//													  IN const std::map< UidType, float > mapDamageByUser, 
//													  IN const int iDungeonID, 
//													  IN const char cDifficultyLevel, 
//													  OUT KERM_NPC_UNIT_DIE_NOT& kInfo )
//{
//	USERLIST_TYPE eType = UT_GAME;
//
//	// 데이터 검증
//	if( iNpcMaxHP <= 0 )
//	{
//		START_LOG( cerr, L"몬스터 HP정보가 이상합니다! 보상을 지급할 수 없습니다!" )
//			<< BUILD_LOG( iNpcMaxHP )
//			<< END_LOG;
//		return false;
//	}
//
//	// 새로 만든 테이블로부터 경험치 데이터를 가져와야함! ( 총 경험치 = 몬스터 레벨 x 몬스터 타입 상수 )
//	const int nTotalExp = SiKDropTable()->GetNpcExp( cNpcLevel ) * static_cast<int>(cMonsterTypeFactor);
//
//	// 배틀필드 EXP상수값 곱하기! ( 배틀필드 총 경험치 = 총 경험치 x 배틀필드 상수 )
//	const int iBattleFieldTotalExp = static_cast<int>( static_cast<double>(nTotalExp) * SiCXSLBattleFieldManager()->GetBattleFieldFactor() );
//	if( iBattleFieldTotalExp <= 0 )
//	{
//		return false;
//	}
//
//	std::map< UidType, int > mapBattleFieldExpReward; // 각 캐릭터별 경험치 획득량
//
//	// 1. 각 캐릭터들이 받아야할 기본 EXP 계산
//	{
//		std::map< UidType, float >::const_iterator mit;
//		for( mit = mapDamageByUser.begin(); mit != mapDamageByUser.end(); ++mit )
//		{
//			// 보상 상수 = 대미지 / 몬스터HP
//			float fRewardFactor = mit->second / static_cast<float>(iNpcMaxHP);
//			if( fRewardFactor > 1.5f )
//				fRewardFactor = 1.5f;
//
//			// 보상 EXP = 보상경험치 * 보상상수
//			const int iRewardEXP = static_cast<int>( static_cast<float>(iBattleFieldTotalExp) * fRewardFactor );
//
//			mapBattleFieldExpReward.insert( std::make_pair( mit->first, iRewardEXP ) );
//		}
//	}
//
//	//EXP 배분.
//	std::map< UidType, KRoomUserPtr >::iterator mit;
//	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
//	{
//		KRoomUserPtr spExpRewardRoomUser = mit->second;
//		if( spExpRewardRoomUser == NULL )
//			continue;
//
//		// 죽지 않은 유저만..
//		if( spExpRewardRoomUser->IsDie() == true )
//			continue;
//
//		// 경험치 보상 대상자인지 확인!
//		std::map< UidType, int >::const_iterator mitBF;
//		mitBF = mapBattleFieldExpReward.find( spExpRewardRoomUser->GetCID() );
//		if( mitBF == mapBattleFieldExpReward.end() )
//			continue;
//
//		// 1. 기본 보상 경험치
//		const int iRewardEXP = mitBF->second;
//		if( iRewardEXP == 0 )
//			continue;
//
//		// 현재 필드안에서 살아있는 파티원 수
//		const int iLivePartyMember = GetSamePartyLiveMember( spExpRewardRoomUser->GetCID() );
//
//		// 2. 파티 보너스 ( 필드 파티 보너스 = NPC경험치 / 현재 지역에 있는 파티원 수 * 파티원 상수 * 난이도 상수 )
//		int iRewardPartyEXP = 0;
//		if( iLivePartyMember > 2 )
//		{
//			const float fDungeonPartyBonusRate = SiKResultProcess()->GetDungeonPartyBonusRate( iLivePartyMember );
//			const float fDungeonPartyDifficultyBonusRate = SiKResultProcess()->GetDungeonPartyDifficultyBonusRate( CXSLDungeon::DL_EXPERT );
//			iRewardPartyEXP = static_cast<int>( static_cast<float>(iBattleFieldTotalExp) / static_cast<float>(iLivePartyMember) * fDungeonPartyBonusRate * fDungeonPartyDifficultyBonusRate );
//		}
//
//		//////////////////////////////////////////////////////////////////////////
//		// 경험치 보상 처리
//
//		// 몬스터 레벨 보정 상수
//		const float fMonsterLvBalRate = CXSLDungeon::GetMonsterLevelBalanceRate( spExpRewardRoomUser->GetLevel(), cNpcLevel );
//
//		//{{ 2011. 03. 08	김민성 경험치를 획득한 던전의 퀘스트 완료 조건 변경
//#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
//		AddHaveExpInDungeon( mit->second->GetCID(), fMonsterLvBalRate > 0.0f );
//#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
//		//}}
//
//		// GSUser에 업데이트하고 클라이언트 화면에 뿌릴 EXP 데이터
//		KEXPData kEXPData;
//		kEXPData.m_iEXP			= static_cast<int>( iRewardEXP * fMonsterLvBalRate );
//		kEXPData.m_iPartyEXP	= static_cast<int>( iRewardPartyEXP * fMonsterLvBalRate );
//
//		//////////////////////////////////////////////////////////////////////////		
//		// 소켓옵션 보너스 적용
//		bool bSocketOptBonus = false;
//		int iSocketOptEXP		= 0;
//		int iSocketOptPartyEXP	= 0;
//
//		//{{ 2010. 05. 11  최육사	이벤트 보상 코드 정리
//		//#ifdef SERV_ROOMUSERINFO_REFAC
//		const float fAddSocketOptExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_EXP_RATE );
//		//#endif SERV_ROOMUSERINFO_REFAC
//		//}}
//		if( fAddSocketOptExpRate > 0.0f )
//		{
//			iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddSocketOptExpRate );
//			iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddSocketOptExpRate );
//
//			bSocketOptBonus = true;
//		}
//
//		//{{ 2009. 12. 4  최육사	길드스킬
//#ifdef GUILD_SKILL_TEST
//		// 같은 파티원이 2명이상!
//		if( iLivePartyMember >= 2 )
//		{
//			// 같은 파티원이 같은 길드원이면?
//			if( IsPartyMembersSameGuild( spExpRewardRoomUser->GetCID() ) )
//			{
//				//{{ 2010. 05. 11  최육사	이벤트 보상 코드 정리
//				//#ifdef SERV_ROOMUSERINFO_REFAC
//				const float fAddGuildBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_GUILD_BONUS_EXP_RATE );
//				//#endif SERV_ROOMUSERINFO_REFAC
//				//}}
//				if( fAddGuildBonusExpRate > 0.0f )
//				{
//					iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddGuildBonusExpRate );
//					iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddGuildBonusExpRate );
//
//					bSocketOptBonus = true;
//				}
//			}
//		}
//#endif GUILD_SKILL_TEST
//		//}}
//
//		// 채널 보너스
//		const float fAddChannelBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_CHANNEL_BONUS_EXP_RATE );
//		if( fAddChannelBonusExpRate > 0.0f )
//		{
//			iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddChannelBonusExpRate );
//			iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddChannelBonusExpRate );
//
//			bSocketOptBonus = true;
//		}
//
//		// 소켓옵션 보너스
//		kEXPData.m_iSocketOptEXP	 = iSocketOptEXP + iSocketOptPartyEXP;
//
//		// 소켓옵션 보너스
//		if( bSocketOptBonus == true  &&  kEXPData.m_iSocketOptEXP == 0  &&  fMonsterLvBalRate > 0.0f )
//		{
//			kEXPData.m_iSocketOptEXP = iSocketOptEXP = 1; // 보너스 경험치 최소값 1주기
//		}
//
//		// 소켓옵션 보너스 합산
//		const int iEXPWithBonus		 = kEXPData.m_iEXP + iSocketOptEXP;
//		const int iPartyEXPWithBonus = kEXPData.m_iPartyEXP + iSocketOptPartyEXP;
//
//		//////////////////////////////////////////////////////////////////////////		
//		//080408.hoons. 해당유저에게 제자가 있으면 제자에게 경험치를 준다.
//		std::vector< UidType > vecTakeEXPList; //스승의 EXP 받은 제자는 누구인지.
//		{
//			std::vector< UidType > vecTutorialUIDList;
//			spExpRewardRoomUser->GetTutorialUIDList( vecTutorialUIDList );
//
//			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTutorialUIDList )
//			{
//				KRoomUserPtr spRoomUser = GetUser( iUnitUID );
//				if( IS_NULL( spRoomUser ) )
//					continue;
//
//				//제자가 있을경우.
//				spRoomUser->AddRewardEXP( iEXPWithBonus );
//				spRoomUser->AddRewardPartyEXP( iPartyEXPWithBonus );
//				vecTakeEXPList.push_back( iUnitUID );
//			}
//		}
//
//		if( vecTakeEXPList.empty() == true )
//		{
//			//개인별 EXP 처리..
//			spExpRewardRoomUser->AddRewardEXP( iEXPWithBonus );
//			spExpRewardRoomUser->AddRewardPartyEXP( iPartyEXPWithBonus );
//
//			std::map< UidType, KEXPData >::iterator mitTemp;
//			mitTemp = kInfo.m_EXPList.find( mit->first );
//			if( mitTemp == kInfo.m_EXPList.end() )
//			{
//				kInfo.m_EXPList.insert( std::make_pair( mit->first, kEXPData ) );
//			}
//			else
//			{
//				mitTemp->second.m_iEXP += kEXPData.m_iEXP;
//				mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
//				mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
//			}
//
//			START_LOG( clog, L"::: 내가 받은 EXP :::" )
//				<< BUILD_LOG( spExpRewardRoomUser->GetCID() )
//				<< BUILD_LOG( kEXPData.m_iEXP )
//				<< BUILD_LOG( kEXPData.m_iPartyEXP )
//				<< BUILD_LOG( kEXPData.m_iSocketOptEXP )
//				;
//		}
//		else
//		{
//			// 제자에게 경험치를 준 경우 처리
//			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTakeEXPList )
//			{
//				std::map< UidType, KEXPData >::iterator mitTemp;
//				mitTemp = kInfo.m_EXPList.find( iUnitUID );
//				if( mitTemp == kInfo.m_EXPList.end() )
//				{
//					kInfo.m_EXPList.insert( std::make_pair( iUnitUID, kEXPData ) );
//				}
//				else
//				{
//					mitTemp->second.m_iEXP += kEXPData.m_iEXP;
//					mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
//					mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
//				}
//			}
//		}		
//	}
//
//	return true;
//}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
bool KBattleFieldUserManager::AddBattleFieldRewardED( IN const char cNpcLevel,
													 IN UidType& iUserUID,
													 IN float fPartyBonusRate,
													 IN const int iED,
													 IN const std::map< UidType, float >& mapDamageByUser,
													 OUT KEGS_GET_ITEM_NOT& kInfo )
{
	kInfo.m_mapGetED.clear();

	if( iED <= 0 )
	{
		START_LOG( cwarn, L"ED가 0인경우도 있나?" )
			<< BUILD_LOG( iED )
			<< END_LOG;
		return false;
	}

	// 1. UserFactor 구하기
	{
		std::map< UidType, float >::const_iterator mit = mapDamageByUser.find( iUserUID );
		if( mit == mapDamageByUser.end() )
		{
			START_LOG( cerr, L"습득 가능 유저가 아닌데....어떻게?!" )
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOGc( cNpcLevel )
				<< END_LOG;
			return false;
		}
	}
	
	//ED 분배하지 않음.
	std::map< UidType, KRoomUserPtr >::iterator mit = m_mapRoomUser[UT_GAME].find( iUserUID );
	if( mit == m_mapRoomUser[UT_GAME].end() )
	{
		START_LOG( cerr, L"존재하지 않는 유저랍니다." )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOGc( cNpcLevel )
			<< END_LOG;
		return false;
	}

	KRoomUserPtr spEDRewardRoomUser = mit->second;
	if( spEDRewardRoomUser == NULL )
		return false;

	// 죽지 않은 유저에게만 보상
	if( mit->second->IsDie() == true )
		return false;

	//////////////////////////////////////////////////////////////////////////
	// ED보상 처리
	// 보너스
	const float fAddSocketObtEDRate = spEDRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_ED_RATE );

	KDungeonRewardED kRewardED;
	kRewardED.m_iED = iED;
	kRewardED.m_iBonusED = static_cast<int>( iED * fAddSocketObtEDRate );

	if( fAddSocketObtEDRate > 0.0f  &&  kRewardED.m_iBonusED == 0 )
	{
		kRewardED.m_iBonusED = 1; // ED보너스 최소값 1ED 지급
	}

	if( fPartyBonusRate > 0.0f )
	{
		kRewardED.m_iBonusED += static_cast<int>( iED * fPartyBonusRate );
	}

	if( IsComeBackUserInParty( iUserUID ) == true )
	{
		kRewardED.m_iBonusED += static_cast<int>( iED * 0.1f );
	}

	// 유저별로 ED보상저장
	spEDRewardRoomUser->AddRewardED( kRewardED.SumED() );

	// GSUser에 업데이트할 ED보상 저장
	kInfo.m_mapGetED.insert( std::make_pair( spEDRewardRoomUser->GetCID(), kRewardED ) );

	return true;
}
#else
bool KBattleFieldUserManager::AddBattleFieldRewardED( IN const char cNpcLevel,
													 IN const int iNpcMaxHP,
													 IN const std::map< UidType, float > mapDamageByUser, 
													 IN const int iED,
													 OUT KEGS_GET_ITEM_NOT& kInfo )
{
	kInfo.m_mapGetED.clear();

	if( iED <= 0 )
	{
		START_LOG( cwarn, L"ED가 0인경우도 있나?" )
			<< BUILD_LOG( iED )
			<< END_LOG;
		return false;
	}

	if( iNpcMaxHP <= 0 )
	{
		START_LOG( cerr, L"몬스터 HP정보가 이상합니다! 보상을 지급할 수 없습니다!" )
			<< BUILD_LOG( iNpcMaxHP )
			<< END_LOG;
		return false;
	}

	std::map< UidType, int > mapBattleFieldEDReward; // 각 캐릭터별 ED 획득량

	// 1. 각 캐릭터들이 받아야할 기본 ED 계산
	{
		std::map< UidType, float >::const_iterator mit;
		for( mit = mapDamageByUser.begin(); mit != mapDamageByUser.end(); ++mit )
		{
			// 보상 상수 = 타격 대미지 / 몬스터HP
			float fRewardFactor = mit->second / static_cast<float>(iNpcMaxHP);
			if( fRewardFactor > 1.5f )
				fRewardFactor = 1.5f;

			// 해당 캐릭터에게 지급될 ED보상 = ED보상 * 보상 상수
			float fRewardED = static_cast<float>(iED) * fRewardFactor;

			// ED보상값 보정
			if( fRewardED > 0  &&  fRewardED < 1.f )
			{
				fRewardED = 1.f;
			}

			mapBattleFieldEDReward.insert( std::make_pair( mit->first, static_cast<int>(fRewardED) ) );
		}
	}

	//ED 배분.
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		KRoomUserPtr spEDRewardRoomUser = mit->second;
		if( spEDRewardRoomUser == NULL )
			continue;

		// 죽지 않은 유저에게만 보상
		if( mit->second->IsDie() == true )
			continue;

		std::map< UidType, int >::const_iterator mitBF;
		mitBF = mapBattleFieldEDReward.find( spEDRewardRoomUser->GetCID() );
		if( mitBF == mapBattleFieldEDReward.end() )
			continue;

		// 해당 캐릭터가 받아야할 보상 ED
		const int iRewardED = mitBF->second;
		if( iRewardED == 0 )
			continue;

		//////////////////////////////////////////////////////////////////////////
		// ED보상 처리

		// 레벨 보정 상수
		const float fMonsterLvBalRate = CXSLDungeon::GetMonsterLevelBalanceRate( spEDRewardRoomUser->GetLevel(), cNpcLevel );

		//{{ 2010. 05. 11  최육사	이벤트 보상 코드 정리
		//#ifdef SERV_ROOMUSERINFO_REFAC
		const float fAddSocketObtEDRate = spEDRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_ED_RATE );
		//#endif SERV_ROOMUSERINFO_REFAC
		//}}

		KDungeonRewardED kRewardED;
		kRewardED.m_iED = static_cast<int>( iRewardED * fMonsterLvBalRate );
		kRewardED.m_iBonusED = static_cast<int>( iRewardED * fAddSocketObtEDRate );

		if( fAddSocketObtEDRate > 0.0f  &&  kRewardED.m_iBonusED == 0 )
		{
			kRewardED.m_iBonusED = 1; // ED보너스 최소값 1ED 지급
		}

		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		if( IsComeBackUserInRoom( iUserUID ) == true )
		{
			kRewardED.m_iBonusED += static_cast<int>( iRewardED * 0.1f );
		}
#endif SERV_COME_BACK_USER_REWARD
		//}} 

		// 유저별로 ED보상저장
		spEDRewardRoomUser->AddRewardED( kRewardED.SumED() );

		// GSUser에 업데이트할 ED보상 저장
		kInfo.m_mapGetED.insert( std::make_pair( spEDRewardRoomUser->GetCID(), kRewardED ) );
	}

	return true;
}
#endif SERV_REFORM_ITEM_DROP
//}}

bool KBattleFieldUserManager::GetItemWithRandom( IN const std::map< UidType, float > mapDamageByUser, 
												 IN const UidType iGetItemUnitUID, 
												 IN const int iItemID, 
												 IN const int iDungeonID,  
												 OUT UidType& iGetUnitUID, 
												 OUT UidType& iGSUID )
{
	USERLIST_TYPE eType = UT_GAME;	

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"Item Templet 정보가 없음. 일어날 수 없는 에러" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	// 아이템을 획득할 유저 리스트를 얻자!
	std::map< UidType, float > mapGetItemProb;	
	float fTotalDamage = 0.f;

	{
		// 획득 가능한 상태의 유저만 골라내자!
		std::map< UidType, float >::const_iterator mit;
		for( mit = mapDamageByUser.begin(); mit != mapDamageByUser.end(); ++mit )
		{
			KRoomUserPtr spRoomUser = GetUser( mit->first );
			if( spRoomUser == NULL )
			{
				START_LOG( cerr, L"룸 유저 포인터 이상. CUID : " << mit->first );
				continue;
			}

			if( spRoomUser->IsDie() == true )
				continue;

			fTotalDamage += mit->second;
			mapGetItemProb.insert( std::make_pair( mit->first, mit->second ) );
		}
	}

	if( fTotalDamage == 0.f )
	{
		// 아이템을 받을 대상자가 없다.
		return false;
	}

	std::map< int, UidType > mapUnitUIDByIndex;
	int iGetItemUnitIndex = 0;
	KLottery kGetItemRandom;

	{
		std::map< UidType, float >::const_iterator mit;
		for( mit = mapGetItemProb.begin(); mit != mapGetItemProb.end(); ++mit )
		{
			const float fDamagePercent = mit->second / fTotalDamage;
			const int iUnitIndex = ++iGetItemUnitIndex;

			// 해당 인덱스를 키값으로 UnitUID를 저장하자!
			mapUnitUIDByIndex.insert( std::make_pair( iUnitIndex, mit->first ) );

			// 해당 인덱스를 case로 랜덤값을 저장하자!
			LIF( kGetItemRandom.AddCase( iUnitIndex, fDamagePercent ) );
		}
	}

	// 모든 확률 합이 100%
	if( kGetItemRandom.GetTotalProb() < 100.f )
	{
		kGetItemRandom.MakeHundredProbabillty();
	}

	const int iResultCase = kGetItemRandom.Decision();
	if( iResultCase == KLottery::CASE_BLANK )
	{
		START_LOG( cerr, L"아이템 획득할 유저가 없습니다!" )
			<< BUILD_LOG( kGetItemRandom.GetCaseNum() )
			<< END_LOG;
		return false;
	}

	std::map< int, UidType >::const_iterator mitIdx;
	mitIdx = mapUnitUIDByIndex.find( iResultCase );
	if( mitIdx == mapUnitUIDByIndex.end() )
	{
		START_LOG( cerr, L"아이템 획득할 유저를 찾지 못했습니다!" )
			<< BUILD_LOG( iResultCase )
			<< END_LOG;
		return false;
	}

	KRoomUserPtr spSelectedUser = GetUser( mitIdx->second );
	if( spSelectedUser == NULL )
	{
		START_LOG( cerr, L"아이템 획득할 유저를 찾지 못했습니다!" )
			<< BUILD_LOG( iResultCase )
			<< END_LOG;
		return false;
	}

	iGetUnitUID = spSelectedUser->GetCID();
	iGSUID = spSelectedUser->GetGSUID();
	return true;
}

void KBattleFieldUserManager::UpdatePartyInfo( IN const UidType iUnitUID, IN const KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT& kNot )
{
	// 해당 유저 정보 얻기
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"존재 하지 않는 유저 입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	// 만약 PartyUID값이 유효하다면 파티 정보 갱신 또는 가입 처리다!
	if( kNot.m_iPartyUID != 0 )
	{
		// PartyUID값 갱신!
		spRoomUser->SetPartyUID( kNot.m_iPartyUID );

		// 파티원 리스트 갱신!
		std::map< UidType, KBattleFieldParty >::iterator mit;
		mit = m_mapPartyList.find( kNot.m_iPartyUID );
		if( mit == m_mapPartyList.end() )
		{
			// 해당 파티정보가 없으면 추가하면 된다.
			KBattleFieldParty kPartyInfo;
			kPartyInfo.UpdatePartyMemberList( kNot.m_vecPartyUnitUIDList );
			m_mapPartyList.insert( std::make_pair( kNot.m_iPartyUID, kPartyInfo ) );
		}
		else
		{
			mit->second.Clear();
			mit->second.UpdatePartyMemberList( kNot.m_vecPartyUnitUIDList );
		}
	}
	// 아니라면 파티 이탈 처리 합니다.
	else
	{
		const UidType iPartyUID = spRoomUser->GetPartyUID();
		if( iPartyUID != 0 )
		{
			if( DelRoomUserInBattleFieldPartyList( iUnitUID ) == false )
			{
				START_LOG( cerr, L"파티에 소속되어 있었는데 파티 정보는 없네? 일어나서는 안되는 에러!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( iPartyUID )
					<< END_LOG;
			}

			spRoomUser->SetPartyUID( 0 );
		}
	}
	
}

/*
//{{ 2012. 07. 16	최육사		배틀필드 입장 퀘스트
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
bool KBattleFieldUserManager::GetQuestDropItem( IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo, IN float fQuestItemDropRate )
#else
bool KBattleFieldUserManager::GetQuestDropItem( IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo )
#endif SERV_QUEST_ITEM_DROP_EVENT
//}
{
	SET_ERROR( NET_OK );

	vecDropInfo.clear();

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsDie() == false )
		{
			//퀘스트 아이템 드롭이 있으면..
			KRoomUser::QUEST_ITEM_DROP_INFO sDropInfo;
			//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			if( mit->second->GetQuestDropItemInBattleField( iBattleFieldID, iNPCID, sDropInfo, fQuestItemDropRate ) == true )
#else
			if( mit->second->GetQuestDropItemInBattleField( iBattleFieldID, iNPCID, sDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}
			{
				vecDropInfo.push_back( sDropInfo );
			}
		}
	}

	if( vecDropInfo.size() > 0 )
		return true;

	return false;
}
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
//}}
*/

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
bool KBattleFieldUserManager::GetQuestDropItem( IN const UidType iUnitUID, IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo, IN float fQuestItemDropRate )
#else
bool KBattleFieldUserManager::GetQuestDropItem( IN const UidType iUnitUID, IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo )
#endif SERV_QUEST_ITEM_DROP_EVENT
//}
{
	SET_ERROR( NET_OK );

	vecDropInfo.clear();

	std::map< UidType, KRoomUserPtr >::iterator mit = m_mapRoomUser[UT_GAME].find( iUnitUID );
	if( mit != m_mapRoomUser[UT_GAME].end() )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsDie() == false )
		{
			//퀘스트 아이템 드롭이 있으면..
			KRoomUser::QUEST_ITEM_DROP_INFO sDropInfo;
			//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			if( mit->second->GetQuestDropItemInBattleField( iBattleFieldID, iNPCID, fQuestItemDropRate, sDropInfo ) == true )
#else
			if( mit->second->GetQuestDropItemInBattleField( iBattleFieldID, iNPCID, sDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}
			{
				vecDropInfo.push_back( sDropInfo );
			}
		}
	}

	if( vecDropInfo.size() > 0 )
		return true;

	return false;
}

float KBattleFieldUserManager::GetBattleFieldPartyBonusRate( IN const UidType& iUnitUID )
{
	int iPartyMemeber = GetSamePartyLiveMember( iUnitUID );

	return KRoomUserManager::GetPartyBonusRate( iPartyMemeber );
}

bool KBattleFieldUserManager::IsComeBackUserInParty( IN UidType iUnitUID )
{
	int iCount = 0;

	if( CheckUserlistType( UT_GAME ) == false )
		return false;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	if( spRoomUser->IsComeBackUser() == true )
		return true;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsDie() == true )
			continue;

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		if( spRoomUser->IsComeBackUser() == true)
		{
			return true;
		}
	}

	return false;
}
#endif SERV_REFORM_ITEM_DROP
//}}

float KBattleFieldUserManager::GetPartyTotalDamage( IN const UidType iUnitUID, 
												  IN const std::map< UidType, float>& mapDamageByUser, 
												  OUT std::vector<UidType>& vecPartyUserList, 
												  IN const USERLIST_TYPE eType /*= UT_GAME*/ )
{
	vecPartyUserList.clear();

	if( CheckUserlistType( eType ) == false )
	{
		std::map< UidType, float>::const_iterator mit = mapDamageByUser.find( iUnitUID );
		if( mit != mapDamageByUser.end() )
		{
			return mit->second;
		}

		return 0.f;
	}

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		// 시간차로 방을 이탈했다면 오류가 아니다.
		START_LOG( cwarn, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return 0.f;
	}

	// party 가 없다.
	if( spRoomUser->GetPartyUID() == 0 )
	{
		std::map< UidType, float>::const_iterator mit = mapDamageByUser.find( iUnitUID );
		if( mit != mapDamageByUser.end() )
		{
			return mit->second;
		}

		return 0.f;
	}

	float fTotalDamage = 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetPartyUID() == spRoomUser->GetPartyUID() )
		{
			std::map< UidType, float>::const_iterator mitDamage = mapDamageByUser.find( mit->first );
			if( mitDamage != mapDamageByUser.end() )
			{
				 fTotalDamage += mitDamage->second;
			}

			vecPartyUserList.push_back( mit->first );
		}
	}

	return fTotalDamage;
}

bool KBattleFieldUserManager::RequestTradeTo( UidType nCID, UidType nCIDTo )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cwarn, L"유저가 없음." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    KRoomUserPtr spRoomUserTo = GetUser( nCIDTo );
    if( !spRoomUserTo )
    {
        START_LOG( cwarn, L"유저가 없음." )
            << BUILD_LOG( nCIDTo )
            << END_LOG;

        return false;
    }

    if( spRoomUser->GetStateID() != KRoomUserFSM::S_PLAY )
    {
        START_LOG( cwarn, L"신청하는 유저의 상태가 이상함." )
            << BUILD_LOG( nCID )
            << BUILD_LOG( spRoomUser->GetStateID() )
            << BUILD_LOG( spRoomUser->GetStateIDString() )
            << END_LOG;

        return false;
    }

    if( spRoomUserTo->GetStateID() != KRoomUserFSM::S_PLAY )
    {
        START_LOG( cwarn, L"신청받는 유저의 상태가 이상함." )
            << BUILD_LOG( nCIDTo )
            << BUILD_LOG( spRoomUserTo->GetStateID() )
            << BUILD_LOG( spRoomUserTo->GetStateIDString() )
            << END_LOG;

        return false;
    }

    return spRoomUser->RequestTradeTo( nCIDTo );
}

bool KBattleFieldUserManager::TradeAcceptedBy( UidType nCID, UidType nCIDAcceptor )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	KRoomUserPtr spRoomUserAcceptor = GetUser( nCIDAcceptor );
	if( !spRoomUserAcceptor )
	{
		START_LOG( cwarn, L"유저가 없음." )
			<< BUILD_LOG( nCIDAcceptor )
			<< END_LOG;

		return false;
	}

	if( spRoomUserAcceptor->GetStateID() != KRoomUserFSM::S_PLAY )
	{
		START_LOG( cwarn, L"신청받는 유저의 상태가 이상함." )
			<< BUILD_LOG( nCIDAcceptor )
			<< BUILD_LOG( spRoomUserAcceptor->GetStateID() )
			<< BUILD_LOG( spRoomUserAcceptor->GetStateIDString() )
			<< END_LOG;

		return false;
	}

	return spRoomUser->TradeAcceptedBy( nCIDAcceptor );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void KBattleFieldUserManager::CalculatePartyRank( IN const std::map< UidType, float >& mapDamageByUser, OUT std::map< UidType, bool >& mapBonusItem )
{
	// 파티 수(솔플 이라면 파티 하나로 판단한다.)  < 파티 구분 index, 유저리스트 >
	std::map< int, std::set<UidType> > mapPartyUserList;

	// 파티 구분용 임시 값
	int iPartyIndex = 1;	
	std::map< UidType, float >::const_iterator mitDamage = mapDamageByUser.begin();
	for( ; mitDamage != mapDamageByUser.end() ; ++mitDamage )
	{
		KRoomUserPtr spUser = GetUser( mitDamage->first );
		if( !spUser )
		{
			START_LOG( cwarn, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mitDamage->first )
				<< END_LOG;
			continue;
		}
	
		std::set<UidType> setUserList;

		// 솔플 유저
		if(spUser->GetPartyUID() == 0 )
		{
			setUserList.insert( spUser->GetCID() );
			mapPartyUserList.insert( std::make_pair( iPartyIndex++, setUserList ) );
			continue;
		}
		

		std::map< UidType, KRoomUserPtr >::iterator mit;
		for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
		{
			if( !mit->second )
			{
				START_LOG( cwarn, L"룸 유저 포인터 이상." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			if( mit->second->GetPartyUID() == spUser->GetPartyUID() )
			{
				setUserList.insert( spUser->GetCID() );
			}
		}

		mapPartyUserList.insert( std::make_pair( iPartyIndex++, setUserList ) );
	}

	// 파티 구분 index 별 누적 데미지량(솔플 이라면 파티 하나로 판단한다.)  < 데미지, 파티 구분 index >
	// 데미지 를 key 로 둔 이유는 상위 30% 를 선정하기 위해 자동 정렬 시킴
	std::map< float, int, std::greater<float> > mapPartyDamage;

	std::map< int, std::set<UidType> >::iterator mitParty = mapPartyUserList.begin();
	for( ; mitParty != mapPartyUserList.end() ; ++mitParty )
	{
		float fTatalDamage = 0.f;
		std::set<UidType>::iterator sit = mitParty->second.begin();
		for( ; sit != mitParty->second.end() ; ++sit )
		{
			std::map< UidType, float >::const_iterator mitUser = mapDamageByUser.find( *sit );
			if( mitUser != mapDamageByUser.end() )
			{
				fTatalDamage += mitUser->second;
			}
		}
	
		mapPartyDamage.insert( std::make_pair( fTatalDamage, mitParty->first ) );
    }

	// 그룹 별 순위 정해야함
	int iTotalPartyCount = static_cast<int>(mapPartyUserList.size());
	int iTopRankPartyCount = static_cast<int>(iTotalPartyCount * 0.3);						// 상위 30%인 파티 수

	std::map< float, int, std::greater<float> >::iterator mitBonus = mapPartyDamage.begin();
	for( int iCount = 0 ; mitBonus != mapPartyDamage.end(); ++iCount, ++mitBonus )
	{
		if( iCount < iTopRankPartyCount )
		{
			std::map< int, std::set<UidType> >::iterator mitParty = mapPartyUserList.find( mitBonus->second );
			if( mitParty != mapPartyUserList.end() )
			{
				std::set<UidType>::iterator sitUnit = mitParty->second.begin();
				for( ; sitUnit != mitParty->second.end() ; ++sitUnit )
				{
					mapBonusItem.insert( std::make_pair( *sitUnit, true ) );
				}
			}			
		}
		else
		{
			std::map< int, std::set<UidType> >::iterator mitParty = mapPartyUserList.find( mitBonus->second );
			if( mitParty != mapPartyUserList.end() )
			{
				std::set<UidType>::iterator sitUnit = mitParty->second.begin();
				for( ; sitUnit != mitParty->second.end() ; ++sitUnit )
				{
					mapBonusItem.insert( std::make_pair( *sitUnit, false ) );
				}
			}
		}		
	}
}

void KBattleFieldUserManager::GetPartyLevelFactor( IN const UidType iUnitUID, IN char cLevel, OUT float& fPartyLevelFactorMin, OUT float& fPartyLevelFactorMax )
{
	fPartyLevelFactorMin = 0.f;
	fPartyLevelFactorMax = 0.f;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		// 시간차로 방을 이탈했다면 오류가 아니다.
		START_LOG( cwarn, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	// party 가 없다.
	if( spRoomUser->GetPartyUID() == 0 )
	{
		fPartyLevelFactorMin = CXSLDungeon::GetMonsterLevelBalanceRate( spRoomUser->GetLevel(), cLevel );
		fPartyLevelFactorMax = fPartyLevelFactorMin;
		return;
	}

	fPartyLevelFactorMin = CXSLDungeon::GetMonsterLevelBalanceRate( spRoomUser->GetLevel(), cLevel );
	fPartyLevelFactorMax = fPartyLevelFactorMin;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetPartyUID() == spRoomUser->GetPartyUID() )
		{
			float fUserLevelFactor = CXSLDungeon::GetMonsterLevelBalanceRate( mit->second->GetLevel(), cLevel );

			fPartyLevelFactorMin = ( fUserLevelFactor < fPartyLevelFactorMin ) ? fUserLevelFactor : fPartyLevelFactorMin;
			fPartyLevelFactorMax = ( fUserLevelFactor > fPartyLevelFactorMax ) ? fUserLevelFactor : fPartyLevelFactorMax;
		}
	}

	return;
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}


#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
void KBattleFieldUserManager::ZombieAlert_CheckStart()
{
	USERLIST_TYPE eType = UT_GAME;

	START_LOG( clog, L"유령유저 ALERT 체크시작.!" );

	//유령유저 체크 시작하기전 시간 초기화
	for( int i = UT_GAME; i < UT_TOTAL_NUM; ++i )
	{
		USERLIST_TYPE eType = (USERLIST_TYPE)i;

		std::map< UidType, KRoomUserPtr >::iterator mit;
		for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
		{
			if( !mit->second )
			{
				START_LOG( cerr, L"룸 유저 포인터 이상." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
			if( mit->second->IsPvpNpc() == true )
				continue;
#endif SERV_PVP_NEW_SYSTEM
			//}}

            mit->second->ZombieAlert_Set( true );
            mit->second->ZombieAlert_Refresh();
		}
	}

	//유령유저 체크를 시작
	m_bIsCheckZombieAlert = true;
}


bool KBattleFieldUserManager::ZombieAlert_CheckStart(  IN const UidType nCID )
{
	if( ZombieAlert_IsCheck() == false )
		return false;

	KRoomUserPtr spRoomUser = GetUser( nCID, UT_GAME );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"룸 유저 검색 실패." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

			//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( spRoomUser->IsPvpNpc() == true )
        return false;
#endif SERV_PVP_NEW_SYSTEM
			//}}

    spRoomUser->ZombieAlert_Set( true );
    spRoomUser->ZombieAlert_Refresh();
	return true;
}


void KBattleFieldUserManager::ZombieAlert_CheckEnd()
{
	START_LOG( clog, L"유령유저 ALERT 체크종료.!" );

	//유령유저 체크 중지.
	m_bIsCheckZombieAlert = false;
}

bool KBattleFieldUserManager::ZombieAlert_Tick()
{
	if( ZombieAlert_IsCheck() == false )
		return false;

	USERLIST_TYPE eType = UT_GAME;

    UidType cidHost = 0;
    bool    bHostFound = false;
    bool    bHostAlive = false;
    bool    bNonHostAlive = false;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		if( mit->second->IsPvpNpc() == true )
			continue;
#endif SERV_PVP_NEW_SYSTEM
		//}}

        mit->second->ZombieAlert_Tick();
        bool    bNewZombieAlert = mit->second->ZombieAlert_Get();
        if ( mit->second->IsHost() == true )
        {
            bHostFound = true;
            cidHost = mit->second->GetCID();
            bHostAlive = mit->second->IsPlaying() == true && bNewZombieAlert == false;
        }
        else if ( bNonHostAlive == false )
        {
            bNonHostAlive = mit->second->IsPlaying() == true && bNewZombieAlert == false;
        }
	}

    if ( bHostFound == true
#ifdef  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
        && GetForceHostCID() != cidHost
#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
        && bHostAlive == false && bNonHostAlive == true )
    {
        return true;
    }

	return false;
}

#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-15	// 박세훈
void KBattleFieldUserManager::GetUnitListGroupdByParty( OUT std::vector< std::set< UidType > >& vecUnitListGroupdByParty ) const
{
	const std::map<UidType, KRoomUserPtr>& mapRoomUser = m_mapRoomUser[UT_GAME];
	std::set<UidType> setChecker;

	for( std::map<UidType, KRoomUserPtr>::const_iterator it = mapRoomUser.begin(); it != mapRoomUser.end(); ++it )
	{
		if( it->second == NULL )
		{
			START_LOG( cwarn, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( it->first )
				<< END_LOG;
			continue;
		}

		if( setChecker.find( it->second->GetCID() ) != setChecker.end() )
			continue;

		const UidType		iPartyUID = it->second->GetPartyUID();
		std::set<UidType>	setUnitList;

		if( 0 < iPartyUID )
		{
			for( std::map<UidType, KRoomUserPtr>::const_iterator itTemp = mapRoomUser.begin(); itTemp != mapRoomUser.end(); ++itTemp )
			{
				if( iPartyUID == itTemp->second->GetPartyUID() )
				{
					const UidType iUnitUID = itTemp->second->GetCID();
					setUnitList.insert( iUnitUID );
					setChecker.insert( iUnitUID );
				}
			}
		}
		else
		{
			const UidType iUnitUID = it->second->GetCID();
			setUnitList.insert( iUnitUID );
			setChecker.insert( iUnitUID );
		}

		vecUnitListGroupdByParty.push_back( setUnitList );
	}
}
#endif // SERV_BATTLE_FIELD_BOSS