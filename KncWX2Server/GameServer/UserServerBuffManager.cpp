#include "GSUser.h"
#include "BaseServer.h"
#include ".\UserServerBuffManager.h"
#include "X2Data/XSLBuffManager.h"
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLRoom.h"
#include "GSWorldMissionManager.h"


//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM


KUserServerBuffManager::KUserServerBuffManager(void)
{
	Clear();
}

KUserServerBuffManager::~KUserServerBuffManager(void)
{
}

void KUserServerBuffManager::Clear()
{
	m_iInsertTime = 0;
	m_mapBuff.clear();
}

void KUserServerBuffManager::Init( IN const KUnitBuffInfo& kUnitBuffInfo )
{
	m_mapBuff.clear();

	// 로그인 서버의 버프 매니저에 있는 시간을 저장한다.
	m_iInsertTime = kUnitBuffInfo.m_iInsertTime;

	// 버프 정보를 저장하자
	m_mapBuff = kUnitBuffInfo.m_mapBuffInfo;
}

bool KUserServerBuffManager::ActivateBuff( IN KBuffInfo& kBuff, OUT std::vector< KBuffInfo >& vecActivatedBuff, IN const CTime tUseTime /* = 0*/, IN int iPeriod /* = 0*/ )
{
	//////////////////////////////////////////////////////////////////////////
	if( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
	{
		START_LOG( clog, L"[버프체크] 어둠의 문 방어 버프가 활성화 됩니다!" )
			<< BUILD_LOG( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
	}
	//////////////////////////////////////////////////////////////////////////

	// 존재하는 버프인지 검사
	const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
	if( pBuffTemplet == NULL )
	{
		START_LOG( cerr, L"존재 하지 않는 버프입니다!" )
			<< BUILD_LOG( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
			<< END_LOG;
		return false;
	}

	// 이미 발동되어있는 버프인지 검사
	if( IsBuffActivated( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID ) == true )
	{
		return false;
	}

	if( tUseTime == 0 )
	{
		CTime tCurr = CTime::GetCurrentTime();
		kBuff.m_iBuffStartTime = tCurr.GetTime();
	}
	else
	{
		kBuff.m_iBuffStartTime = tUseTime.GetTime();
	}

	if( iPeriod == 0 )
	{
		if( pBuffTemplet->m_fBuffPeriodSecond == 0 )
		{
			kBuff.m_iBuffEndTime = kBuff.m_iBuffStartTime;
		}
		else
		{
			kBuff.m_iBuffEndTime = kBuff.m_iBuffStartTime + static_cast<__int64>(pBuffTemplet->m_fBuffPeriodSecond);
		}
	}
	else
	{
		kBuff.m_iBuffEndTime = kBuff.m_iBuffStartTime + static_cast<__int64>(iPeriod);
	}

	m_mapBuff.insert( std::make_pair( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, kBuff ) );

	vecActivatedBuff.push_back( kBuff );
	return true;
}

bool KUserServerBuffManager::DeactivateBuff( IN const int iBuffID, IN OUT std::vector< int >& vecDeleteBuff )
{
	//////////////////////////////////////////////////////////////////////////
	if( iBuffID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
	{
		START_LOG( clog, L"[버프체크] 어둠의 문 방어 버프가 비활성화 됩니다!" )
			<< BUILD_LOG( iBuffID );
	}
	//////////////////////////////////////////////////////////////////////////
	

	if( IsBuffActivated( iBuffID ) == false )
	{
		START_LOG( cerr, L"이미 삭제된 버프입니다." )
			<< BUILD_LOG( iBuffID )
			<< END_LOG;
		return false;
	}

	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.find( iBuffID );
	if( mit != m_mapBuff.end() )
	{
		// 삭제하자
		m_mapBuff.erase( mit );
		vecDeleteBuff.push_back( iBuffID );
	}
	else
	{
		START_LOG( cerr, L"존재하지 않는 버프 입니다." )
			<< BUILD_LOG( iBuffID )
			<< END_LOG;
		return false;
	}
	
	return true;
}

//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
bool KUserServerBuffManager::GetActivateBuff( IN const int iBuffID, OUT KBuffInfo& kBuff ) const
{
	std::map< int, KBuffInfo >::const_iterator it = m_mapBuff.find( iBuffID );
	
	if( it != m_mapBuff.end() )
	{
		kBuff = it->second;
		return true;
	}

	return false;
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

void KUserServerBuffManager::GetBuffList( OUT std::vector< KBuffInfo >& vecBuffInfo, IN const int iRoomType /*= CXSLRoom::RT_NONE*/ ) const
{
	vecBuffInfo.clear();

	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffInfo = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffInfo == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 버프 ID가 적용 중?" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( iRoomType != CXSLRoom::RT_NONE )
		{
			if( pBuffInfo->CheckRoomType( iRoomType ) == false )
				continue;
		}

		vecBuffInfo.push_back( mit->second );
	}
}

void KUserServerBuffManager::GetBuffIDList( OUT std::vector< int >& vecBuffID, IN const int iRoomType /*= CXSLRoom::RT_NONE*/ ) const
{
	vecBuffID.clear();

	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffInfo = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffInfo == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 버프 ID가 적용 중?" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( iRoomType != CXSLRoom::RT_NONE )
		{
			if( pBuffInfo->CheckRoomType( iRoomType ) == false )
				continue;
		}

		vecBuffID.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
	}
}

void KUserServerBuffManager::GetBuffFactorList( OUT std::vector< KBuffFactor >& vecBuffFactorID ) const
{
	vecBuffFactorID.clear();

	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffInfo = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffInfo == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 버프 ID가 적용 중?" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		vecBuffFactorID.push_back( mit->second.m_kFactorInfo );
	}
}

void KUserServerBuffManager::GetBuffBonusRate( IN OUT KRoomUserInfo& kRoomUserInfo, IN const int iRoomType /*= CXSLRoom::RT_NONE*/ )
{
	if( m_mapBuff.empty() )
		return;

	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffInfo = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffInfo == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 버프 ID가 적용 중?" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( iRoomType != CXSLRoom::RT_NONE )
		{
			if( pBuffInfo->CheckRoomType( iRoomType ) == false )
				continue;
		}

		// 소켓 옵션 적용
		BOOST_TEST_FOREACH( const int, iSocketOption, pBuffInfo->m_vecOption )
		{
			const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( iSocketOption );
			if( pSocketData == NULL )
			{
				START_LOG( cerr, L"존재하지않는 소켓옵션이 버프의 소켓 옵션으로 세팅되어 있습니다." )
					<< BUILD_LOG( pBuffInfo->m_eBuffID )
					<< BUILD_LOG( iSocketOption )
					<< END_LOG;
				continue;
			}

			if( pSocketData->m_fIncreaseExpPer > 0.0f )
			{
				kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_EXP, pSocketData->m_fIncreaseExpPer );
			}
			if( pSocketData->m_fIncreaseEDPer > 0.0f )
			{
				kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_ED, pSocketData->m_fIncreaseEDPer );
			}
		}
	}
}

void KUserServerBuffManager::CheckBuffPeriod( OUT std::vector< int >& vecDeactivatedBuff, IN const int& iRoomType /*= CXSLRoom::RT_NONE*/ )
{
	vecDeactivatedBuff.clear();

	CTime tCurr = CTime::GetCurrentTime();
	std::vector<int> vecDeleteBuff;
    // 만료된 버프를 체크한다!
	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		if( mit->second.m_iBuffStartTime != mit->second.m_iBuffEndTime )
		{
			if( tCurr > mit->second.m_iBuffEndTime )
			{
				vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );

				//////////////////////////////////////////////////////////////////////////
				if( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
				{
					START_LOG( clog, L"[버프체크] 어둠의 문 방어 버프가 시간 제한에 걸렸습니다!" )
						<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
				}
				//////////////////////////////////////////////////////////////////////////
			}
		}
	}

	BOOST_TEST_FOREACH( int, iBuffID, vecDeleteBuff )
	{
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

void KUserServerBuffManager::OnLeaveField( IN KGSUserPtr spUser, OUT std::vector< int >& vecDeactivatedBuff )
{
	vecDeactivatedBuff.clear();

	std::vector< int > vecDeleteBuff;
	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		switch( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
		case CXSLBuffManager::BTI_BUFF_WARM_ENERGE_OF_EL:			// 따뜻한 엘의 기운
			vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			break;
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// 버프 삭제
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
void KUserServerBuffManager::OnLeaveDungeonRoom( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivateBuffList, IN bool bBadAttitudeUser )
#else
void KUserServerBuffManager::OnLeaveDungeonRoom( IN KGSUserPtr spUser, std::vector< int >& vecDeactivateBuffList )
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}
{
	std::vector<int> vecDeleteBuff;

	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"존재 하지 않는 버프입니다!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		switch( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_1:		// 영웅의 집결
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_2:
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_3:
			vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			break;

		case CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY:		// 재집결을 위한 휴식은 이탈해도 버프가 삭제되지 않는다.
			//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
			if( bBadAttitudeUser == true )		// 불량 유저로 인한 퇴장이라면' 재집결을 위한 휴식' 디버프를 새로 걸어 준다.
			{
				vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
			//}
			break;

		default:
			{
				if( pBuffTemplet->m_bEndBuffAtTheLeaveDungeonRoom == true )
				{
					vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );

					//////////////////////////////////////////////////////////////////////////
					if( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
					{
						START_LOG( clog, L"[버프체크] 어둠의 문 방어 버프가 던전 이탈로 삭제됩니다." )
							<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
					}
					//////////////////////////////////////////////////////////////////////////
				}
			}
			break;
		}
	}
	
	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// 버프 삭제
		DeactivateBuff( iBuffID, vecDeactivateBuffList );
	}

	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	if( bBadAttitudeUser == true )		// 불량 유저로 인한 퇴장이라면' 재집결을 위한 휴식' 디버프를 새로 걸어 준다.
	{
		KBuffInfo kBuff;
		kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY;

		// 존재하는 버프인지 검사
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY );
		if( pBuffTemplet != NULL )
		{
			kBuff.m_kFactorInfo.m_iFactorID = pBuffTemplet->m_iFactorID;
			ActivateBuff( kBuff, vecActivateBuff );
		}
	}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
}

void KUserServerBuffManager::OnLeaveBattleField( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivatedBuff )
{
	vecDeactivatedBuff.clear();
	std::vector<int> vecDeleteBuff;
	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"존재 하지 않는 버프입니다!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		switch( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
		case CXSLBuffManager::BTI_BUFF_FIELD_DEFENCE:		// 배틀필드 이벤트 버프(경치 +30%, ED +30%)
			{
				vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			}break;

		default:
			if( pBuffTemplet->m_bEndBuffAtTheLeaveBattleFieldRoom == true )
			{
				vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			}
			break;
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// 버프 삭제
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

void KUserServerBuffManager::OnEndDungeonGame( IN KGSUserPtr spUser, IN const bool bDungeonClearSuccess, IN const bool bStartedByAutoParty, OUT std::vector< int >& vecDeactivatedBuff )
{
	vecDeactivatedBuff.clear();
	std::vector< int > vecDeleteBuff;
	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"존재 하지 않는 버프입니다!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		switch( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_1:	// 영웅의 집결
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_2:
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_3:
			{
				if( bStartedByAutoParty )
				{
					vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
				}				
			}			
			break;

		case CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY:	// 재집결을 위한 휴식
			{
				// 재집결을 위한 휴식은 던전클리어를 해야만 사라진다.
				// 성공 실패 여부는 중요하지 않다
				if( bStartedByAutoParty )
				{
					vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
				}
			}
			break;

		default:
			{
				if( pBuffTemplet->m_bEndBuffAtTheEndDungeonGame == true )
				{
					vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );

					//////////////////////////////////////////////////////////////////////////
					if( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
					{
						START_LOG( clog, L"[버프체크] 어둠의 문 방어 버프가 던전 종료로 삭제됩니다." )
							<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
					}
					//////////////////////////////////////////////////////////////////////////
				}
			}
			break;
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// 버프 삭제
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

void KUserServerBuffManager::OnUserDied( IN KGSUserPtr spUser, std::vector< int >& vecDeactivatedBuff )
{
	std::vector< int > vecDeleteBuff;

	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"존재 하지 않는 버프입니다!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( pBuffTemplet->m_bEndBuffAtTheUserDied == true )
		{
			vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );

			//////////////////////////////////////////////////////////////////////////
			if( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
			{
				START_LOG( clog, L"[버프체크] 어둠의 문 방어 버프가 캐릭터 사망으로 삭제됩니다." )
					<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// 버프 삭제
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

#ifdef SERV_FIX_REVENGE_BUFF// 작업날짜: 2013-08-09	// 박세훈
void KUserServerBuffManager::OnLeavePVPRoom( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivatedBuff )
{
	std::vector<int> vecDeleteBuff;

	for( std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin(); mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"존재 하지 않는 버프입니다!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( pBuffTemplet->m_bEndBuffAtTheLeavePvpRoom == true )
		{
			vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// 버프 삭제
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}
#endif // SERV_FIX_REVENGE_BUFF

#endif SERV_SERVER_BUFF_SYSTEM
//}}


