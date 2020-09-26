#include "BattleFieldMonsterManager.h"
#include "X2Data/XSLUnitManager.h"
#include "KAttribNpcTable.h"
//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	#include "..\Common\GameEvent\GameEventManager.h"
	#include "..\Common\GameEvent\GameEventScriptManager.h"
#endif SERV_FIELD_EVENT_MONSTER
//}}



//{{ 2011. 10. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldMonsterManager::KBattleFieldMonsterManager() : 
m_iAtStartedMonsterCount( 0 )
{
}

KBattleFieldMonsterManager::~KBattleFieldMonsterManager()
{
}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
void KBattleFieldMonsterManager::StartGame( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount, IN const int iDangerousValue, IN const bool bBossField )
{
	KRoomMonsterManager::StartGame();

	m_kNpcDieCount.Clear();
	m_mapReserveMonsterRespawn.clear();
	m_mapNpcInfoForClient.clear();

#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR// 작업날짜: 2013-11-20	// 박세훈
	m_mapNpcOwner.clear();
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR

#ifdef SERV_FIELD_EVENT_MONSTER// 작업날짜: 2013-05-21	// 박세훈
	m_mapReserveEventMonsterRespawn.clear();
	m_mapEventMonster.clear();
	m_mapEventNpcInfoForClient.clear();
	m_mapAliveEventNpcList.clear();
	m_mapDieEventNpcList.clear();
#endif // SERV_FIELD_EVENT_MONSTER

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_mapAliveMiddleBossList.clear();
	m_vecMiddleBossListForClient.clear();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_mapAliveEventBossList.clear();
	m_vecEventBossListForClient.clear();
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

	// 배틀필드가 생성된 직후에 몬스터 생성 처리!
	if( bBossField == true )
	{
		if( FirstCreateBossMonster( eBattleFieldID ) == false )
		{
			START_LOG( cerr, L"몬스터 소환 실패! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iPlayerCount )
				<< END_LOG;
		}
	}
	else
	{
		if( FirstCreateMonster( eBattleFieldID, iPlayerCount ) == false )
		{
			START_LOG( cerr, L"몬스터 소환 실패! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iPlayerCount )
				<< END_LOG;
		}
	}

	// 배틀필드 젤 처음 시작했을때 몬스터 수
	m_iAtStartedMonsterCount = GetAliveMonsterCount();
}

bool KBattleFieldMonsterManager::FirstCreateBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID )
{
	// 1. 출현시킬 몬스터 리스트를 받아오자!
	KNPCList kBattleFieldNpcList;
	if( SiCXSLBattleFieldManager()->GetCreateBossMonsterList( eBattleFieldID, kBattleFieldNpcList ) == false )
	{
		START_LOG( cerr, L"배틀필드 NPC리스트 받아오기 실패!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}

	// 2. 몬스터 생성 처리!
	BOOST_TEST_FOREACH( KNPCUnitReq&, kNpcInfo, kBattleFieldNpcList.m_NPCList )
	{
		// 몬스터 생성!
		if( CreateMonster( kNpcInfo, kNpcInfo.m_UID ) == false )
		{
			START_LOG( cerr, L"몬스터 생성을 실패하였습니다!" )
				<< BUILD_LOG( kNpcInfo.m_NPCID )
				<< END_LOG;
			continue;
		}
	}

	return true;
}

bool KBattleFieldMonsterManager::IsAliveBossMonster( IN const int iNpcUID ) const
{
	std::map< int, NPC_DATA >::const_iterator it = m_mapAliveNpcList.find( iNpcUID );	
	if( it == m_mapAliveNpcList.end() )
	{
		return false;
	}

	return it->second.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC;
}
#else // SERV_BATTLE_FIELD_BOSS
void KBattleFieldMonsterManager::StartGame( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount, IN const int iDangerousValue )
{
	KRoomMonsterManager::StartGame();

	m_kNpcDieCount.Clear();
	m_mapReserveMonsterRespawn.clear();
	m_mapNpcInfoForClient.clear();

#ifdef SERV_FIELD_EVENT_MONSTER// 작업날짜: 2013-05-21	// 박세훈
	m_mapReserveEventMonsterRespawn.clear();
	m_mapEventMonster.clear();
	m_mapEventNpcInfoForClient.clear();
	m_mapAliveEventNpcList.clear();
	m_mapDieEventNpcList.clear();
#endif // SERV_FIELD_EVENT_MONSTER

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_mapAliveMiddleBossList.clear();
	m_vecMiddleBossListForClient.clear();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_mapAliveEventBossList.clear();
	m_vecEventBossListForClient.clear();
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

	if( FirstCreateMonster( eBattleFieldID, iPlayerCount ) == false )
	{
        START_LOG( cerr, L"몬스터 소환 실패! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( iPlayerCount )
			<< END_LOG;
	}

	// 배틀필드 젤 처음 시작했을때 몬스터 수
	m_iAtStartedMonsterCount = GetAliveMonsterCount();
}
#endif // SERV_BATTLE_FIELD_BOSS

void KBattleFieldMonsterManager::EndGame()
{
	KRoomMonsterManager::EndGame();

	m_kNpcDieCount.Clear();
	m_mapReserveMonsterRespawn.clear();
	m_mapNpcInfoForClient.clear();

#ifdef SERV_FIELD_EVENT_MONSTER// 작업날짜: 2013-05-21	// 박세훈
	m_mapReserveEventMonsterRespawn.clear();
	m_mapEventMonster.clear();
	m_mapEventNpcInfoForClient.clear();
	m_mapAliveEventNpcList.clear();
	m_mapDieEventNpcList.clear();
#endif // SERV_FIELD_EVENT_MONSTER

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_mapAliveMiddleBossList.clear();
	m_vecMiddleBossListForClient.clear();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_mapAliveEventBossList.clear();
	m_vecEventBossListForClient.clear();
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
}

void KBattleFieldMonsterManager::OnCloseRoom()
{
	KRoomMonsterManager::OnCloseRoom();

	m_kNpcDieCount.Clear();
	m_mapReserveMonsterRespawn.clear();
	m_mapNpcInfoForClient.clear();

#ifdef SERV_FIELD_EVENT_MONSTER// 작업날짜: 2013-05-21	// 박세훈
	m_mapReserveEventMonsterRespawn.clear();
	m_mapEventMonster.clear();
	m_mapEventNpcInfoForClient.clear();
	m_mapAliveEventNpcList.clear();
	m_mapDieEventNpcList.clear();
#endif // SERV_FIELD_EVENT_MONSTER

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_mapAliveMiddleBossList.clear();
	m_vecMiddleBossListForClient.clear();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_mapAliveEventBossList.clear();
	m_vecEventBossListForClient.clear();
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
}

bool KBattleFieldMonsterManager::FirstCreateMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount )
{
	// 1. 출현시킬 몬스터 리스트를 받아오자!
	KNPCList kBattleFieldNpcList;
	if( SiCXSLBattleFieldManager()->GetCreateMonsterList( eBattleFieldID,
														  iPlayerCount,
														  kBattleFieldNpcList ) == false )
	{
		START_LOG( cerr, L"배틀필드 NPC리스트 받아오기 실패!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( iPlayerCount )
			<< END_LOG;
		return false;
	}

	// 2. 몬스터 생성 처리!
	BOOST_TEST_FOREACH( KNPCUnitReq&, kNpcInfo, kBattleFieldNpcList.m_NPCList )
	{
		// 몬스터 생성!
		if( CreateMonster( kNpcInfo, kNpcInfo.m_UID ) == false )
		{
			START_LOG( cerr, L"몬스터 생성을 실패하였습니다!" )
				<< BUILD_LOG( kNpcInfo.m_NPCID )
				<< END_LOG;
			continue;
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
// 몬스터 리스폰 시스템
//////////////////////////////////////////////////////////////////////////

// 필드에는 각각 N = (현재필드 구성원수)에 따라 최대로 등장 가능한 몬스터 수가 정해져 있다.

// 즉, 필드에는 항상 Max몬스터 숫자 만큼의 몬스터가 존재한다.

// Max몬스터 수 = (일반몬스터수 * 1) + (하급엘리트몬스터수*2) + (상급엘리트몬스터수*3) + (중간보스몬스터수*4) + (보스몬스터수*12)

// 처음 필드 생성시에는, (최초입장인원수*3) = N 으로 취급하여 N*3만큼의 NPC를 즉시 배치한다.

// NPC의 리스폰 규칙은 몬스터수가 MAX미만일 경우 (or미만이 될 경우), 3~9초(랜덤)이후에 몬스터를 리스폰한다(MIN~MAX시간 조절 가능하도록)

// 몬스터 수가 MAX미만일 경우(or미만이 될 경우), 3~9초(랜덤) 이후에 몬스터를 리스폰한다(MIN~MAX시간 조절 가능하도록)

// 몬스터 수가 MAX이상일 경우, 리스폰을 중지한다.

// 리스폰 몬스터를 선택하는 규칙은 아래와 같으며, (필드별로 리스폰 그룹을 설정할 수 있다)

// 해당 지역(필드)에서 등장하는 몬스터 그룹은 테이블(스크립트)로 제공한다.

//////////////////////////////////////////////////////////////////////////

bool KBattleFieldMonsterManager::CheckRespawnMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
													  IN const int iPlayerCount, 
													  IN OUT KDangerousEventInfo& kDangerousEvent,
													  OUT KEGS_NPC_UNIT_CREATE_NOT& kResultNot )
{
	// 1. 지금까지 살아있는 몬스터 수가 몇마리 인지 체크한다.
	const int ALIVE_MONSTER_COUNT = GetAliveMonsterCount();

	// 2. 현재 필드에서 최대 소환 가능한 수를 얻어서 비교한다.
	const int MAX_MONSTER_COUNT_IN_BATTLE_FIELD = SiCXSLBattleFieldManager()->GetMaxMonsterCountInBattleField( eBattleFieldID, iPlayerCount );
	if( MAX_MONSTER_COUNT_IN_BATTLE_FIELD <= ALIVE_MONSTER_COUNT )
		return false;

	// 3. 리스폰 예약된 몬스터가 있는지 확인!
	if( m_mapReserveMonsterRespawn.empty() == true )
		return false;

	// 4. 리스폰 예약 리스트를 확인하여 리스폰 타임이 지난 그룹의 몬스터는 리스폰 시킬 준비를 하자!
	std::map< int, int > mapRespawnReadyNpcCount;
	std::vector< int > vecRespawnTimeOverList;

	std::map< int, SMonsterRespawnInfo >::const_iterator mit;
	for( mit = m_mapReserveMonsterRespawn.begin(); mit != m_mapReserveMonsterRespawn.end(); ++mit )
	{
		const int iNpcUID = mit->first;

		// 리스폰 시간이 다된 몬스터 수를 카운트한다.
		if( mit->second.IsRespawnTimeOver() == true )
		{
			// 리스폰 예약을 삭제하기 위해 따로 담는다.
			vecRespawnTimeOverList.push_back( iNpcUID );

			std::map< int, int >::iterator mitRRN;
			mitRRN = mapRespawnReadyNpcCount.find( mit->second.m_iMonsterGroupID );
			if( mitRRN == mapRespawnReadyNpcCount.end() )
			{
				mapRespawnReadyNpcCount.insert( std::make_pair( mit->second.m_iMonsterGroupID, 1 ) );
			}
			else
			{
				++mitRRN->second;
			}
		}
	}

	// 5. 리스폰 타임을 만족한 몬스터가 하나도 없다면 더이상 몬스터를 추가 생산 하지 않는다.
	if( mapRespawnReadyNpcCount.empty() == true )
		return false;

	// 6. 현재 살아있는 몬스터 종류를 얻자!
	CXSLBattleField::KBattleFieldMonsterCountInfo kAliveMonsterCountInfo;
	GetAliveMonsterCountInfo( kAliveMonsterCountInfo );

	// 7. 출현시킬 몬스터 리스트를 받아오자!
	KNPCList kBattleFieldNpcList;
	if( SiCXSLBattleFieldManager()->GetRespawnMonsterList( eBattleFieldID,
														   kAliveMonsterCountInfo, 
														   mapRespawnReadyNpcCount,
														   kBattleFieldNpcList ) == false )
	{
		START_LOG( cerr, L"배틀필드 NPC리스트 받아오기 실패!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( kAliveMonsterCountInfo.GetTotalMonsterCount() )
			<< BUILD_LOG( mapRespawnReadyNpcCount.size() )
			<< END_LOG;
		return false;
	}

	// 8. 몬스터 생성 처리!
	BOOST_TEST_FOREACH( KNPCUnitReq&, kNpcInfo, kBattleFieldNpcList.m_NPCList )
	{
		// 몬스터 생성!
		if( CreateMonster( kNpcInfo, kNpcInfo.m_UID ) == false )
		{
			START_LOG( cerr, L"몬스터 생성을 실패하였습니다!" )
				<< BUILD_LOG( kNpcInfo.m_NPCID )
				<< END_LOG;
			continue;
		}

		// 지금 바로 클라이언트에 전달하기 위한 것도 저장하자!
		KNPCUnitNot kNot;
		kNot.m_kNPCUnitReq = kNpcInfo;
		kResultNot.m_vecNPCUnitAck.push_back( kNot );

		//////////////////////////////////////////////////////////////////////////
		// 속성 몬스터는 여기서 생성하면 되겠군!
		//////////////////////////////////////////////////////////////////////////
		if( SiKAttribNpcTable()->IsNoDropNpc( kNpcInfo.m_NPCID ) == true )
			continue;

		if( kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_ELITE_MONSTER_DROP ) == false )
			continue;

		kDangerousEvent.DeleteEvent( KDangerousEventInfo::DE_ELITE_MONSTER_DROP );

		// 엘리트 몬스터를 불러 오자!
		char cDifficultyLevel = 0;
		if( SiCXSLBattleFieldManager()->CheckPossibleEliteMonsterDrop( iPlayerCount, 0, cDifficultyLevel ) == false )
			continue;
		
		// 속성 몬스터 테이블에서 해당 몬스터 정보를 얻어옵니다!
		KAttribEnchantNpcInfo kAttribNpcInfo;
		if( SiKAttribNpcTable()->GetAttribNpcInfo( CXSLDungeon::DT_NORMAL, cDifficultyLevel, kNpcInfo.m_Level, kNpcInfo.m_NPCID, kAttribNpcInfo ) == false )
		{
			START_LOG( cerr, L"속성 몬스터 정보 얻기 실패!" )
				<< BUILD_LOGc( cDifficultyLevel )
				<< BUILD_LOG( kNpcInfo.m_NPCID )
				<< BUILD_LOGc( kNpcInfo.m_Level )
				<< END_LOG;
			continue;
		}
		
		// 속성 몬스터 정보를 임시저장소에 보관하자!
		if( SetAttribMonster( kNpcInfo.m_UID, kAttribNpcInfo ) == false )
		{
			START_LOG( cerr, L"속성 정보를 등록하는데 실패 하였다!" )
				<< BUILD_LOG( kNpcInfo.m_UID )
				<< END_LOG;
			continue;
		}

		// 속성 몬스터 정보 저장!
		kResultNot.m_mapAttribNpcInfo.insert( std::make_pair( kNpcInfo.m_UID, kAttribNpcInfo ) );
	}

	// 9. 리스폰 예약 리스트에서 예약 삭제
	LIF( vecRespawnTimeOverList.size() == kResultNot.m_vecNPCUnitAck.size() );

	BOOST_TEST_FOREACH( const int, iNpcUID, vecRespawnTimeOverList )
	{
		m_mapReserveMonsterRespawn.erase( iNpcUID );
	}

	return true;
}

void KBattleFieldMonsterManager::GetAliveMonsterList( OUT std::vector< KNPCUnitReq >& vecNpcList, OUT std::map< int, KAttribEnchantNpcInfo >& mapAttirbNpcInfo ) const
{
	vecNpcList.clear();
	mapAttirbNpcInfo.clear();
    
	std::map< int, SNpcInfoForClient >::const_iterator mit;
	for( mit = m_mapNpcInfoForClient.begin(); mit != m_mapNpcInfoForClient.end(); ++mit )
	{
		// 해당 몬스터가 살아있는지 확인!
		if( IsMonsterAlive( mit->first ) == false )
		{
			START_LOG( cerr, L"죽은 몬스터를 리스트에 넣을려고 한다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// Npc리스트에 넣자!
		vecNpcList.push_back( mit->second.m_kNpcInfo );

		// 해당 몬스터가 속성 몬스터라면 속성정보도 넣자!
		BOOST_TEST_FOREACH( const KAttribEnchantNpcInfo&, kAttribNpcInfo, mit->second.m_vecAttribNpcInfo )
		{
			mapAttirbNpcInfo.insert( std::make_pair( mit->first, kAttribNpcInfo ) );
			break;
		}
	}

	//{{ 2013. 1. 4	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	for( mit = m_mapEventNpcInfoForClient.begin(); mit != m_mapEventNpcInfoForClient.end(); ++mit )
	{
		const SNpcInfoForClient& sNpcInfoForClient = mit->second;

		// 해당 몬스터가 살아있는지 확인!
		if( IsEventMonsterAlive( mit->first ) == false )
		{
			START_LOG( cerr, L"죽은 몬스터를 리스트에 넣을려고 한다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// Npc리스트에 넣자!
		vecNpcList.push_back( mit->second.m_kNpcInfo );
	}
#endif SERV_FIELD_EVENT_MONSTER
	//}}
}

bool KBattleFieldMonsterManager::CreateMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID )
{
	// 복사본
	SNpcInfoForClient kCreateNpcInfo;
	kCreateNpcInfo.m_kNpcInfo = kNpcInfo;

	const bool bRet = KRoomMonsterManager::CreateMonster( kCreateNpcInfo.m_kNpcInfo, kCreateNpcInfo.m_kNpcInfo.m_UID );
	if( bRet )
	{
		// 생성된 NpcUID값을 넘기자!
		iCreatedNpcUID = kCreateNpcInfo.m_kNpcInfo.m_UID;

		// 클라이언트에 전달하기 위한 정보도 저장하자!
		m_mapNpcInfoForClient.insert( std::make_pair( kCreateNpcInfo.m_kNpcInfo.m_UID, kCreateNpcInfo ) );
	}

    return bRet;
}

bool KBattleFieldMonsterManager::SetMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID )
{
	int iGroupID = 0;

	// 몬스터 죽음 처리 하기 전에 그룹ID를 얻는다.
	if( GetMonsterGroupID( iNpcUID, iGroupID ) == false )
	{
		START_LOG( cerr, L"살아있는 몬스터가 아닙니다!" )
			<< BUILD_LOG( iNpcUID  )
			<< END_LOG;
		return false;
	}

	bool bRet = false;
	
	if( iGroupID == 0 ) 
	{
		// 몬스터 그룹ID가 0이면 서버가 생성한 몬스터가 아니다.
		bRet = KRoomMonsterManager::SetMonsterDie( iNpcUID, iAttUnitUID, false );
		if( bRet )
		{
			// 죽은 몬스터의 req정보는 삭제해도 좋다!
			m_mapNpcInfoForClient.erase( iNpcUID );
			
			//{{ 배틀 필드에서 몬스터 생성 오류 수정 - 김민성 -- 2012년 11월 15일 패치 예정
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			EraseNpcOwner( iNpcUID );
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			//}
		}
	}
	else
	{
		// 서버가 생성한 몬스터 이면 몬스터를 죽은 몬스터로 처리하고 리스폰 예약을 하자!
		bRet = KRoomMonsterManager::SetMonsterDie( iNpcUID, iAttUnitUID );
		if( bRet )
		{
			// 죽은 몬스터의 req정보는 삭제해도 좋다!
			m_mapNpcInfoForClient.erase( iNpcUID );
			//{{ 배틀 필드에서 몬스터 생성 오류 수정 - 김민성 -- 2012년 11월 15일 패치 예정
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			EraseNpcOwner( iNpcUID );
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			//}

			// 몬스터 죽음 처리가 성공했다면 마지막으로 죽은 몬스터의 시각을 저장하자!
			SMonsterRespawnInfo sRespawnInfo;
			sRespawnInfo.m_iMonsterGroupID = iGroupID;
			sRespawnInfo.m_fRespawnTime = SiCXSLBattleFieldManager()->GetMonsterRespawnTimeByRandom();
			sRespawnInfo.m_tTimer.restart();
			m_mapReserveMonsterRespawn.insert( std::make_pair( iNpcUID, sRespawnInfo ) );
		}
	}

	return bRet;
}

bool KBattleFieldMonsterManager::SetAttribMonster( IN const int iNpcUID, IN const KAttribEnchantNpcInfo& kInfo )
{
	std::map< int, SNpcInfoForClient >::iterator mit;
	mit = m_mapNpcInfoForClient.find( iNpcUID );
	if( mit == m_mapNpcInfoForClient.end() )
	{
		START_LOG( cerr, L"존재하지 않는 몬스터 입니다!" )
			<< BUILD_LOG( iNpcUID )
			<< END_LOG;
		return false;
	}

	if( mit->second.m_vecAttribNpcInfo.empty() == false )
	{
		START_LOG( cerr, L"이미 속성 정보가 있는 몬스터 입니다. 일어날수 없는 에러!" )
			<< BUILD_LOG( iNpcUID )
			<< BUILD_LOG( mit->second.m_vecAttribNpcInfo.size() )
			<< END_LOG;
		return false;
	}

	mit->second.m_vecAttribNpcInfo.push_back( kInfo );
	return true;
}

bool KBattleFieldMonsterManager::IsAttribNpc( IN const int iNpcUID )
{
	std::map< int, SNpcInfoForClient >::iterator mit;
	mit = m_mapNpcInfoForClient.find( iNpcUID );
	if( mit == m_mapNpcInfoForClient.end() )
	{
		START_LOG( clog, L"존재하지 않는 몬스터 입니다!" )
			<< BUILD_LOG( iNpcUID )
			<< END_LOG;
		return false;
	}

	return (mit->second.m_vecAttribNpcInfo.empty() == false);
}

void KBattleFieldMonsterManager::IncreaseMonsterDieCount( IN const char cMonsterTypeFator )
{
	switch( cMonsterTypeFator )
	{
	case CXSLUnitManager::MTF_NORMAL_NPC:		++m_kNpcDieCount.m_iNormalNpcDieCount;		break;
	case CXSLUnitManager::MTF_LOW_ELITE_NPC:	++m_kNpcDieCount.m_iLowEliteNpcDieCount;	break;
	case CXSLUnitManager::MTF_HIGH_ELITE_NPC:	++m_kNpcDieCount.m_iHighEliteNpcDieCount;	break;
	case CXSLUnitManager::MTF_MIDDLE_BOSS_NPC:	++m_kNpcDieCount.m_iMiddleBossDieCount;		break;
	case CXSLUnitManager::MTF_BOSS_NPC:			++m_kNpcDieCount.m_iBossDieCount;			break;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	case CXSLUnitManager::MTF_EVENT_BOSS_NPC:	++m_kNpcDieCount.m_iEventBossDieCount;			break;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	default:
		START_LOG( cerr, L"이상한 몬스터 타입 입니다!" )
			<< BUILD_LOGc( cMonsterTypeFator )
			<< END_LOG;
		return;
	}
}

//{{ 배틀 필드에서 몬스터 생성 오류 수정 - 김민성 -- 2012년 11월 15일 패치 예정
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
void KBattleFieldMonsterManager::SetNpcOwner( IN const int iNPCUID, IN const UidType iOwnerUnit )
{
	std::map< int, UidType >::iterator mit = m_mapNpcOwner.find( iNPCUID );
	if( mit == m_mapNpcOwner.end() )
	{
		m_mapNpcOwner.insert( std::make_pair( iNPCUID, iOwnerUnit ) );
	}
	else
	{
		START_LOG( cerr, L"중복 되는 npc uid 를 등록하려고 한다." )
			<< BUILD_LOGc( iNPCUID )
			<< BUILD_LOGc( iOwnerUnit )
			<< END_LOG;
	}
}

void KBattleFieldMonsterManager::EraseNpcOwner( IN const int iNPCUID )
{
	std::map< int, UidType >::iterator mit = m_mapNpcOwner.find( iNPCUID );
	if( mit == m_mapNpcOwner.end() )
	{
		START_LOG( clog, L"없는 npc uid 를 지우려 한다." )
			<< BUILD_LOGc( iNPCUID )
			<< END_LOG;
	}
	else
	{
		m_mapNpcOwner.erase( mit );
	}
}

void KBattleFieldMonsterManager::GetNpcOwnerListByUnitUID( IN const UidType iUnitUID, OUT std::vector< int >& vecNpcUID )
{
	vecNpcUID.clear();

	std::map< int, UidType >::iterator mit = m_mapNpcOwner.begin();
	for( ; mit != m_mapNpcOwner.end() ; ++mit )
	{
		if( mit->second == iUnitUID )
		{
			vecNpcUID.push_back( mit->first );
		}
	}
}
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
//}

//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
void KBattleFieldMonsterManager::ClassifyMonsterEvent( OUT std::vector<int>& vecStartEventID
													 , OUT std::vector<int>& vecEndEventID
													 )
{
	std::set<int> setEventID;
	SiKGameEventManager()->CheckEventMonster( setEventID );

	std::set<int>::const_iterator					itRunEvent = setEventID.begin();
	std::map< int, std::set<int> >::const_iterator	itMyEvent = m_mapEventMonster.begin();

	// 시작/종료 이벤트 분류 작업
	{
		while( ( itRunEvent != setEventID.end() ) && ( itMyEvent != m_mapEventMonster.end() ) )
		{
			if( *itRunEvent == itMyEvent->first )
			{
				// 이미 진행 중인 몬스터 출현 이벤트이다.
				++itRunEvent;
				++itMyEvent;
			}
			else if( itMyEvent->first < *itRunEvent )
			{
				// 이것은 종료 처리 되어야할 이벤트이다.
				vecEndEventID.push_back( itMyEvent->first );
				++itMyEvent;
			}
			else
			{
				// 이것은 시작 처리 되어야할 이벤트이다.
				vecStartEventID.push_back( *itRunEvent );
				++itRunEvent;
			}
		}

		// 새로 추가되는 이벤트 분류
		while( itRunEvent != setEventID.end() )
		{
			vecStartEventID.push_back( *itRunEvent );
			++itRunEvent;
		}

		// 종료되는 이벤트 분류
		while( itMyEvent != m_mapEventMonster.end() )
		{
			vecEndEventID.push_back( itMyEvent->first );
			++itMyEvent;
		}
	}
}

void KBattleFieldMonsterManager::StartedMonsterEventProcess( IN const std::vector<int>& vecStartEventID
														   , IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
														   , IN OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
														   )
{
	// 새로 시작된 몬스터 출현 이벤트 처리
	BOOST_TEST_FOREACH( int, iEventID, vecStartEventID )
	{
		std::vector<int> vecEventID;
		std::set<int> setEventMonsterUID;
		std::vector< KGameEventScriptManager::KAdventNpcEventInfo > vecAdventNpcEventInfo;

		vecEventID.push_back( iEventID );
		SiKGameEventScriptManager()->GetAdventNpcEventInfo( vecEventID, vecAdventNpcEventInfo );

		BOOST_TEST_FOREACH( KGameEventScriptManager::KAdventNpcEventInfo&, kInfo, vecAdventNpcEventInfo )
		{
			// 등장할 수 있는 필드인지 체크!
			if( kInfo.m_setAdventField.empty() == false )
			{
				if( kInfo.m_setAdventField.find( eBattleFieldID ) == kInfo.m_setAdventField.end() )
				{
					continue;
				}
			}

			// 등장 하면 안되는 필드인지 체크!
			if( kInfo.m_setNoAdventField.empty() == false )
			{
				if( kInfo.m_setNoAdventField.find( eBattleFieldID ) != kInfo.m_setNoAdventField.end() )
				{
					continue;
				}
			}

			// 2. 몬스터 생성 처리!
			for( int i=0; i < kInfo.m_iMonsterDropCount; ++i )
			{
				KNPCUnitReq kNpcInfo;

				// 등장할 몬스터를 결정하자!
				kNpcInfo.m_NPCID = kInfo.m_kAdventNpcLot.Decision();
				if( kNpcInfo.m_NPCID == KLottery::CASE_BLANK )
				{
					continue;
				}

				// 배틀 필드에서 필요한 정보를 얻어오자.
				bool bResult = SiCXSLBattleFieldManager()->GetEventMonsterCreateInfo( eBattleFieldID, kNpcInfo );
				if( bResult == false )
				{
					START_LOG( cerr, L"배틀필드 NPC리스트 받아오기 실패!" )
						<< BUILD_LOG( eBattleFieldID )
						<< END_LOG;
					return;
				}

				// 나머지 정보 세팅
				kNpcInfo.m_Level			+= kInfo.m_iAddLevel;
				kNpcInfo.m_bAggressive		= kInfo.m_bAggressive;
				kNpcInfo.m_AddPos.y			= kInfo.m_fAddPosY;
				kNpcInfo.m_bNoDrop			= !kInfo.m_bDrop;

				// 몬스터 생성!
				CreateEventMonster( kNpcInfo, kNpcInfo.m_UID );

				// 생성 패킷을 위한 준비
				KNPCUnitNot kNpcUnitNot;
				kNpcUnitNot.m_kNPCUnitReq = kNpcInfo;
				kNpcCreateNot.m_vecNPCUnitAck.push_back( kNpcUnitNot );

				// 종료 처리를 위한 정보 보관
				setEventMonsterUID.insert( kNpcInfo.m_UID );
			}
		}
		// 종료 처리를 위한 정보 보관
		if( setEventMonsterUID.empty() == false )
		{
			m_mapEventMonster.insert( std::map< int, std::set<int> >::value_type( iEventID, setEventMonsterUID ) );
		}
	}
}

void KBattleFieldMonsterManager::EndedMonsterEventProcess( IN const std::vector<int>& vecEndEventID
														 , OUT std::vector<KERM_NPC_UNIT_DIE_NOT>& veckPacketNpcDie
														 )
{
	// 이벤트별로 각각 처리 한다.
	BOOST_TEST_FOREACH( int, iEndEventID, vecEndEventID )
	{
		// 제거 해야 할 이벤트 정보가 있는지 검사!
		std::map< int, std::set<int> >::iterator it = m_mapEventMonster.find( iEndEventID );
		if( it == m_mapEventMonster.end() )
		{
			continue;
		}

		// 해당 이벤트에 의해 등록된 몬스터의 처리
		BOOST_TEST_FOREACH( int, iEventMonsterUID, it->second )
		{
			std::map< int, NPC_DATA >::iterator itAliveNpc = m_mapAliveEventNpcList.find( iEventMonsterUID );
			if( itAliveNpc == m_mapAliveEventNpcList.end() )
			{
				// 죽은 npc라면, 해당 목록에서만 제거한다.
				m_mapDieEventNpcList.erase( iEventMonsterUID );
			}
			else
			{
				// 살아 있는 npc라면, 해당 목록에서 제거하고,
				m_mapAliveEventNpcList.erase( itAliveNpc );

				// 난입자를 위한 npc 정보에서도 제거한다.
				m_mapEventNpcInfoForClient.erase( iEventMonsterUID );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				DeleteReservedNpcUID( iEventMonsterUID );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

				// 리스폰 예약 정보에서도 제거한다.
				m_mapReserveEventMonsterRespawn.erase( iEventMonsterUID );

				// 그리고 죽었다는 정보를 날려주기 위해 패킷 정보를 준비한다.
				KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
				kNpcDietNot.m_nDieNPCUID		 = iEventMonsterUID;
				kNpcDietNot.m_iNPCID			 = itAliveNpc->second.m_iNPCID;
				veckPacketNpcDie.push_back( kNpcDietNot );
			}
		}

		// 해당 이벤트 정보를 제거한다.
		m_mapEventMonster.erase( it );
	}
}

void KBattleFieldMonsterManager::CreateEventMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID )
{
	// 복사본
	SNpcInfoForClient kCreateNpcInfo;
	kCreateNpcInfo.m_kNpcInfo = kNpcInfo;

	if( CXSLUnitManager::IsValidMonsterGrade( static_cast<CXSLUnitManager::MONSTER_GRADE>( kNpcInfo.m_cMonsterGrade ) ) == false )
	{
		START_LOG( cerr, L"몬스터 등급 정보가 이상합니다!" )
			<< BUILD_LOG( kNpcInfo.m_NPCID )
			<< BUILD_LOGc( kNpcInfo.m_cMonsterGrade )
			<< END_LOG;
	}	

	// 1. NPC UID 발급
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	iCreatedNpcUID = GetNewNpcUID();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	iCreatedNpcUID = m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	// 3. NPC(Monster) 관리를 위한 data 보관
	NPC_DATA npcData;
	npcData.m_cLevel	= kNpcInfo.m_Level;
	npcData.m_iNPCID	= kNpcInfo.m_NPCID;
	npcData.m_bActive	= kNpcInfo.m_bActive;
	npcData.m_bNoDrop	= kNpcInfo.m_bNoDrop;
	npcData.m_bIsBoss	= ( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC );	// 몬스터 등급으로 보스몬스터인지 체크.
	//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//보스몬스터인지 체크.
	//{{ 2009. 12. 18  최육사	던전경험치개편
	npcData.m_cMonsterGrade = kNpcInfo.m_cMonsterGrade; //일반몹인지 중간보스인지 보스인지 체크.
	//}}
	//{{ 2011. 11. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	npcData.m_iGroupID	= kNpcInfo.m_iGroupID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	m_mapAliveEventNpcList.insert( std::make_pair( iCreatedNpcUID, npcData ) );

	// 생성된 NpcUID값을 넘기자!
	kCreateNpcInfo.m_kNpcInfo.m_UID = iCreatedNpcUID;

	// 클라이언트에 전달하기 위한 정보도 저장하자!
	m_mapEventNpcInfoForClient.insert( std::make_pair( kCreateNpcInfo.m_kNpcInfo.m_UID, kCreateNpcInfo ) );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	InsertReservedNpcUID( kCreateNpcInfo.m_kNpcInfo.m_UID );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

bool KBattleFieldMonsterManager::SetEventMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID )
{
	bool bRet = false;

	// 서버가 생성한 몬스터 이면 몬스터를 죽은 몬스터로 처리하고 리스폰 예약을 하자!
	{
		std::map<int, NPC_DATA>::iterator mit;
		mit = m_mapAliveEventNpcList.find( iNpcUID );
		if( mit == m_mapAliveEventNpcList.end() )
		{
			// 살아있는 몬스터 리스트에 없으면 실패!
			bRet = false;
		}

		// 죽은 몬스터 리스트에 넣는다!
		m_mapDieEventNpcList.insert( std::make_pair( iNpcUID, mit->second ) );

		// 살아있는 몬스터 리스트에서는 뺀다!
		m_mapAliveEventNpcList.erase( mit );
		bRet = true;
	}

	if( bRet )
	{
		// 죽은 몬스터의 req정보는 삭제해도 좋다!
		m_mapEventNpcInfoForClient.erase( iNpcUID );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		DeleteReservedNpcUID( iNpcUID );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		// 몬스터 죽음 처리가 성공했다면 마지막으로 죽은 몬스터의 시각을 저장하자!
		SMonsterRespawnInfo sRespawnInfo;
		//sRespawnInfo.m_iMonsterGroupID = iGroupID;
		sRespawnInfo.m_fRespawnTime = SiCXSLBattleFieldManager()->GetMonsterRespawnTimeByRandom();
		sRespawnInfo.m_tTimer.restart();
		m_mapReserveEventMonsterRespawn.insert( std::make_pair( iNpcUID, sRespawnInfo ) );
	}

	return bRet;
}

bool KBattleFieldMonsterManager::GetEventMonsterEventID( IN const int iNpcUID, OUT int& iEventID ) const
{
	for( std::map< int, std::set<int> >::const_iterator it = m_mapEventMonster.begin(); it != m_mapEventMonster.end(); ++it )
	{
		BOOST_TEST_FOREACH( const int, iEventMonsterUID, it->second )
		{
			if( iNpcUID == iEventMonsterUID )
			{
				iEventID = it->first;
				return true;
			}
		}
	}
	return false;
}

void KBattleFieldMonsterManager::CheckRespawnEventMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
														 , IN OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
														 )
{
	// 리스폰 예약된 몬스터가 있는지 확인!
	if( m_mapReserveEventMonsterRespawn.empty() == true )
		return;

	// 리스폰 예약 리스트를 확인하여 리스폰 타임이 지난 그룹의 몬스터는 리스폰 시킬 준비를 하자!
	std::set<int>	setRespawnTimeOverList;
	for( std::map<int, SMonsterRespawnInfo>::const_iterator mit = m_mapReserveEventMonsterRespawn.begin(); mit != m_mapReserveEventMonsterRespawn.end(); ++mit )
	{
		const int iNpcUID = mit->first;

		// 리스폰 시간이 다된 몬스터 수를 카운트한다.
		if( mit->second.IsRespawnTimeOver() == true )
		{
			// 리스폰 예약을 삭제하기 위해 따로 담는다.
			setRespawnTimeOverList.insert( iNpcUID );
		}
	}

	// 리스폰 타임을 만족한 몬스터가 하나도 없다면 더이상 몬스터를 추가 생산 하지 않는다.
	if( setRespawnTimeOverList.empty() == true )
		return;

	// 몬스터 생성 처리!
	std::vector<int>	vecTimeRestartRespawnList;
	BOOST_TEST_FOREACH( const int, iNpcUID, setRespawnTimeOverList )
	{
		if( m_mapDieEventNpcList.find( iNpcUID ) == m_mapDieEventNpcList.end() )
		{
			START_LOG( cwarn, L"m_mapDieEventNpcList에서 이벤트 몬스터의 리스폰 데이터를 찾을 수 없습니다." )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iNpcUID )
				<< END_LOG;
			continue;
		}

		// 해당 EventMonster가 소속된 이벤트를 찾아내도록 하자!
		std::map< int, std::set<int> >::iterator itMapEventMonster;
		std::set<int>::iterator itSetEventMonsterUID;
		for( itMapEventMonster=m_mapEventMonster.begin(); itMapEventMonster != m_mapEventMonster.end(); ++itMapEventMonster )
		{
			itSetEventMonsterUID = itMapEventMonster->second.find( iNpcUID );
			if( itSetEventMonsterUID != itMapEventMonster->second.end() )
			{
				break;
			}
		}

		// 존재하지 않는 이벤트라면, 리스폰을 할 수 없다. ( 이벤트가 종료될 때 당연 리스폰 정보도 삭제되어야 한다! )
		if( itSetEventMonsterUID == itMapEventMonster->second.end() )
		{
			START_LOG( cwarn, L"존재하지 않는 이벤트에 소속된 이벤트 몬스터를 리스폰 시키려 하였습니다." )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iNpcUID )
				<< END_LOG;
			continue;
		}
		
		std::vector<int> vecEventID;
		std::vector< KGameEventScriptManager::KAdventNpcEventInfo > vecAdventNpcEventInfo;

		// 해당 이벤트의 몬스터 정보를 읽어 오도록 하자!
		vecEventID.push_back( itMapEventMonster->first );
		SiKGameEventScriptManager()->GetAdventNpcEventInfo( vecEventID, vecAdventNpcEventInfo );

		BOOST_TEST_FOREACH( KGameEventScriptManager::KAdventNpcEventInfo&, kInfo, vecAdventNpcEventInfo )
		{
			// 등장할 수 있는 필드인지 체크!
			if( kInfo.m_setAdventField.empty() == false )
			{
				if( kInfo.m_setAdventField.find( eBattleFieldID ) == kInfo.m_setAdventField.end() )
				{
					continue;
				}
			}

			// 등장 하면 안되는 필드인지 체크!
			if( kInfo.m_setNoAdventField.empty() == false )
			{
				if( kInfo.m_setNoAdventField.find( eBattleFieldID ) != kInfo.m_setNoAdventField.end() )
				{
					continue;
				}
			}

			// 2. 몬스터 생성 처리!
			{
				KNPCUnitReq kNpcInfo;

				// 등장할 몬스터를 결정하자!
				kNpcInfo.m_NPCID = kInfo.m_kAdventNpcLot.Decision();
				if( kNpcInfo.m_NPCID == KLottery::CASE_BLANK )
				{
					vecTimeRestartRespawnList.push_back( iNpcUID );
					continue;
				}

				// 배틀 필드에서 필요한 정보를 얻어오자.
				bool bResult = SiCXSLBattleFieldManager()->GetEventMonsterCreateInfo( eBattleFieldID, kNpcInfo );
				if( bResult == false )
				{
					START_LOG( cerr, L"배틀필드 NPC리스트 받아오기 실패!" )
						<< BUILD_LOG( eBattleFieldID )
						<< END_LOG;
					return;
				}

				// 나머지 정보 세팅
				kNpcInfo.m_Level			+= kInfo.m_iAddLevel;
				kNpcInfo.m_bAggressive		= kInfo.m_bAggressive;
				kNpcInfo.m_AddPos.y			= kInfo.m_fAddPosY;
				kNpcInfo.m_bNoDrop			= !kInfo.m_bDrop;

				// 몬스터 생성!
				CreateEventMonster( kNpcInfo, kNpcInfo.m_UID );

				// 생성 패킷을 위한 준비
				KNPCUnitNot kNpcUnitNot;
				kNpcUnitNot.m_kNPCUnitReq = kNpcInfo;
				kNpcCreateNot.m_vecNPCUnitAck.push_back( kNpcUnitNot );

				// 종료 처리를 위한 정보 수정
				// setEventMonsterUID.insert( kNpcInfo.m_UID );
				itMapEventMonster->second.erase( itSetEventMonsterUID );
				itMapEventMonster->second.insert( kNpcInfo.m_UID );
			}
		}
	}

	// 이번 리스폰 타임에서 제외된 대상을 위한 정리
	BOOST_TEST_FOREACH( const int, iNpcUID, vecTimeRestartRespawnList )
	{
		std::map<int, SMonsterRespawnInfo>::iterator it = m_mapReserveEventMonsterRespawn.find( iNpcUID );
		if( it != m_mapReserveEventMonsterRespawn.end() )
		{
			it->second.m_fRespawnTime = SiCXSLBattleFieldManager()->GetMonsterRespawnTimeByRandom();
			it->second.m_tTimer.restart();

			setRespawnTimeOverList.erase( iNpcUID );
		}
	}

	// 리스폰된 대상을 리스트에서 제거한다.
	BOOST_TEST_FOREACH( const int, iNpcUID, setRespawnTimeOverList )
	{
		m_mapReserveEventMonsterRespawn.erase( iNpcUID );
	}
}

bool KBattleFieldMonsterManager::CheckEventMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
												   , OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
												   , OUT std::vector<KERM_NPC_UNIT_DIE_NOT>& veckPacketNpcDie
												   )
{
	std::vector<int>	vecStartEventID;
	std::vector<int>	vecEndEventID;

	// 현재 진행 중인 몬스터 출현 이벤트로부터 시작/종료 이벤트 분류
	ClassifyMonsterEvent( vecStartEventID, vecEndEventID );

	// 새로 시작된 몬스터 출현 이벤트 처리
	StartedMonsterEventProcess( vecStartEventID, eBattleFieldID, kNpcCreateNot );

	// 종료된 몬스터 출현 이벤트 처리
	EndedMonsterEventProcess( vecEndEventID, veckPacketNpcDie );

	// 몬스터 출현 이벤트 리스폰 처리
	CheckRespawnEventMonster( eBattleFieldID, kNpcCreateNot );

	return true;
}

bool KBattleFieldMonsterManager::IsEventMonster( IN const int iNpcUID ) const
{
	if( m_mapAliveEventNpcList.find( iNpcUID ) != m_mapAliveEventNpcList.end() )
		return true;

	if( m_mapDieEventNpcList.find( iNpcUID ) != m_mapDieEventNpcList.end() )
		return true;

	return false;
}

bool KBattleFieldMonsterManager::GetEventNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const
{
	std::map<int, NPC_DATA>::const_iterator mit;
	mit = m_mapAliveEventNpcList.find( iNpcUID );
	if( mit == m_mapAliveEventNpcList.end() )
	{
		// 살아있는 몬스터중에 없으면 죽은 몬스터리스트에서 찾는다.
		mit = m_mapDieEventNpcList.find( iNpcUID );
		if( mit == m_mapDieEventNpcList.end() )
		{
			return false;
		}

		kNpcData = mit->second;
		return true;
	}

	kNpcData = mit->second;
	return true;
}
#endif SERV_FIELD_EVENT_MONSTER
//}}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
int KBattleFieldMonsterManager::GetAttribNpcAttribCount( IN const int iNpcUID )
{
	std::map< int, SNpcInfoForClient >::iterator mit;
	mit = m_mapNpcInfoForClient.find( iNpcUID );
	if( mit == m_mapNpcInfoForClient.end() )
	{
		START_LOG( clog, L"존재하지 않는 몬스터 입니다!" )
			<< BUILD_LOG( iNpcUID )
			<< END_LOG;
		return 0;
	}

	return static_cast<int>(mit->second.m_vecAttribNpcInfo.size());
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
bool KBattleFieldMonsterManager::CheckMiddleBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
													IN const int iDangerousValue,
													IN OUT KDangerousEventInfo& kDangerousEvent,
													OUT KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT& kResultNot )
{
	// 지금 필드에 중보가 남아 있다면 나가자!
	if( IsRemainMiddleBoss() == true )
		return false;

	// 보스 몬스터 출현인지 검사
	if( kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_MIDDLE_BOSS_MONSTER_DROP ) == false )
		return false;

	// 보스 몬스터 출현 예약을 지우자!
	kDangerousEvent.DeleteEvent( KDangerousEventInfo::DE_MIDDLE_BOSS_MONSTER_DROP );

	// 해당 필드에 출현하는 보스 몬스터로 세팅을 바꿉니다.
	
	std::vector<KNPCUnitReq> vecMiddleBossNpcInfo;
	if( SiCXSLBattleFieldManager()->GetBattieFieldMiddleBossMonsterInfo( eBattleFieldID, iDangerousValue, vecMiddleBossNpcInfo ) == false )
	{
		START_LOG( cerr, L"보스 몬스터 출현 실패!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( KNPCUnitReq&, kMiddleBossNpcInfo, vecMiddleBossNpcInfo )
	{
		// 몬스터 생성!
		CreateMiddleBossMonster( kMiddleBossNpcInfo, kMiddleBossNpcInfo.m_UID );		

		// 지금 바로 클라이언트에 전달하기 위한 것도 저장하자!
		KNPCUnitNot kNot;
		kNot.m_kNPCUnitReq = kMiddleBossNpcInfo;
		kResultNot.m_kCreatePacket.m_vecNPCUnitAck.push_back( kNot );
	}

	m_vecMiddleBossListForClient.push_back( vecMiddleBossNpcInfo );

	return true;
}

void KBattleFieldMonsterManager::CreateMiddleBossMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID )
{
	// 복사본
	SNpcInfoForClient kCreateNpcInfo;
	kCreateNpcInfo.m_kNpcInfo = kNpcInfo;

	if( CXSLUnitManager::IsValidMonsterGrade( static_cast<CXSLUnitManager::MONSTER_GRADE>( kNpcInfo.m_cMonsterGrade ) ) == false )
	{
		START_LOG( cerr, L"몬스터 등급 정보가 이상합니다!" )
			<< BUILD_LOG( kNpcInfo.m_NPCID )
			<< BUILD_LOGc( kNpcInfo.m_cMonsterGrade )
			<< END_LOG;
	}	

	// 1. NPC UID 발급
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	iCreatedNpcUID = GetNewNpcUID();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	iCreatedNpcUID = m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	// 3. NPC(Monster) 관리를 위한 data 보관
	NPC_DATA npcData;
	npcData.m_cLevel	= kNpcInfo.m_Level;
	npcData.m_iNPCID	= kNpcInfo.m_NPCID;
	npcData.m_bActive	= kNpcInfo.m_bActive;
	npcData.m_bNoDrop	= kNpcInfo.m_bNoDrop;
	npcData.m_bIsBoss	= ( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC );	// 몬스터 등급으로 보스몬스터인지 체크.
	//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//보스몬스터인지 체크.
	//{{ 2009. 12. 18  최육사	던전경험치개편
	npcData.m_cMonsterGrade = kNpcInfo.m_cMonsterGrade; //일반몹인지 중간보스인지 보스인지 체크.
	//}}
	npcData.m_iGroupID	= kNpcInfo.m_iGroupID;
	npcData.m_iBossGroupID	= kNpcInfo.m_iBossGroupID;

	m_mapAliveMiddleBossList.insert( std::make_pair( iCreatedNpcUID, npcData ) );

	// 생성된 NpcUID값을 넘기자!
	kCreateNpcInfo.m_kNpcInfo.m_UID = iCreatedNpcUID;
}

bool KBattleFieldMonsterManager::SetMiddleBossMonsterDie( IN const int iNpcUID )
{
	bool bRet = false;

	// 서버가 생성한 몬스터 이면 몬스터를 죽은 몬스터로 처리하자!
	{
		std::map<int, NPC_DATA>::iterator mit;
		mit = m_mapAliveMiddleBossList.find( iNpcUID );
		if( mit == m_mapAliveMiddleBossList.end() )
		{
			// 살아있는 몬스터 리스트에 없으면 실패!
			bRet = false;
		}

		// 살아있는 몬스터 리스트에서는 뺀다!
		m_mapAliveMiddleBossList.erase( mit );
		bRet = true;
	}

	if( bRet )
	{
		// 중보 관리 리스트 정보를 정리한다(해당 중보 삭제)
		std::vector< std::vector<KNPCUnitReq> >	vecTempMiddleBossList = m_vecMiddleBossListForClient;
		m_vecMiddleBossListForClient.clear();

		BOOST_TEST_FOREACH( std::vector<KNPCUnitReq>&, vecList, vecTempMiddleBossList )
		{
			std::vector<KNPCUnitReq> vecNew;

			BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
			{
				if( kNpc.m_UID != iNpcUID )
				{
					vecNew.push_back( kNpc );
				}
			}

			m_vecMiddleBossListForClient.push_back( vecNew );
		}
	}

	return bRet;
}

bool KBattleFieldMonsterManager::IsMiddleBossMonster( IN const int iNpcUID ) const
{
	if( m_mapAliveMiddleBossList.find( iNpcUID ) != m_mapAliveMiddleBossList.end() )
		return true;

	bool bRet = false;
	
	for( int index = 0 ; index < (int)m_vecMiddleBossListForClient.size() ; ++index )
	{
		std::vector<KNPCUnitReq> vecList = m_vecMiddleBossListForClient[index];

		BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
		{
			if( kNpc.m_UID == iNpcUID )
			{
				bRet = true;
				break;
			}
		}

		if( bRet == true )
			break;
	}

	return bRet;
}

bool KBattleFieldMonsterManager::GetMiddleBossNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const
{
	std::map<int, NPC_DATA>::const_iterator mit;
	mit = m_mapAliveMiddleBossList.find( iNpcUID );
	if( mit == m_mapAliveMiddleBossList.end() )
	{
		return false;
	}

	kNpcData = mit->second;
	return true;
}

void KBattleFieldMonsterManager::GetMiddleBossMonsterList( OUT std::vector< std::vector<KNPCUnitReq> >& vecNpcDataList )
{
	vecNpcDataList = m_vecMiddleBossListForClient;
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
bool KBattleFieldMonsterManager::CheckEventBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
														IN const int iDangerousValue,
														IN OUT KDangerousEventInfo& kDangerousEvent,
														OUT KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT& kResultNot )
{
	//코드 이벤트 걸리지 않았으면 나가자
	if( SiKGameEventManager()->IsEnableCode( CEI_EVENT_BATTLEFIELD_EVENT_BOSS ) == false )
		return false;	

	// 지금 필드에 중보가 남아 있다면 나가자!
	if( IsRemainEventBoss() == true )
	{
		kDangerousEvent.DeleteEvent( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP );
		return false;
	}
		

	// 보스 몬스터 출현인지 검사
	if( kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP ) == false )
		return false;

	// 보스 몬스터 출현 예약을 지우자!
	kDangerousEvent.DeleteEvent( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP );

	// 해당 필드에 출현하는 보스 몬스터로 세팅을 바꿉니다.

	std::vector<KNPCUnitReq> vecEventBossNpcInfo;
	if( SiCXSLBattleFieldManager()->GetBattieFieldEventBossMonsterInfo( eBattleFieldID, iDangerousValue, vecEventBossNpcInfo ) == false )
	{
		START_LOG( cerr, L"보스 몬스터 출현 실패!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( KNPCUnitReq&, kEventBossNpcInfo, vecEventBossNpcInfo )
	{
		// 몬스터 생성!
		CreateEventBossMonster( kEventBossNpcInfo, kEventBossNpcInfo.m_UID );		

		// 지금 바로 클라이언트에 전달하기 위한 것도 저장하자!
		KNPCUnitNot kNot;
		kNot.m_kNPCUnitReq = kEventBossNpcInfo;
		kResultNot.m_kCreatePacket.m_vecNPCUnitAck.push_back( kNot );
	}

	m_vecEventBossListForClient.push_back( vecEventBossNpcInfo );

	return true;
}

void KBattleFieldMonsterManager::CreateEventBossMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID )
{
	// 복사본
	SNpcInfoForClient kCreateNpcInfo;
	kCreateNpcInfo.m_kNpcInfo = kNpcInfo;

	if( CXSLUnitManager::IsValidMonsterGrade( static_cast<CXSLUnitManager::MONSTER_GRADE>( kNpcInfo.m_cMonsterGrade ) ) == false )
	{
		START_LOG( cerr, L"몬스터 등급 정보가 이상합니다!" )
			<< BUILD_LOG( kNpcInfo.m_NPCID )
			<< BUILD_LOGc( kNpcInfo.m_cMonsterGrade )
			<< END_LOG;
	}	

	// 1. NPC UID 발급
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	iCreatedNpcUID = GetNewNpcUID();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	//iCreatedNpcUID = m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	// 3. NPC(Monster) 관리를 위한 data 보관
	NPC_DATA npcData;
	npcData.m_cLevel	= kNpcInfo.m_Level;
	npcData.m_iNPCID	= kNpcInfo.m_NPCID;
	npcData.m_bActive	= kNpcInfo.m_bActive;
	npcData.m_bNoDrop	= kNpcInfo.m_bNoDrop;
	npcData.m_bIsBoss	= ( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_EVENT_BOSS_NPC );	// 몬스터 등급으로 보스몬스터인지 체크.
	//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//보스몬스터인지 체크.
	//{{ 2009. 12. 18  최육사	던전경험치개편
	npcData.m_cMonsterGrade = kNpcInfo.m_cMonsterGrade; //일반몹인지 중간보스인지 보스인지 체크.
	//}}
	npcData.m_iGroupID	= kNpcInfo.m_iGroupID;
	npcData.m_iBossGroupID	= kNpcInfo.m_iBossGroupID;

	m_mapAliveEventBossList.insert( std::make_pair( iCreatedNpcUID, npcData ) );

	// 생성된 NpcUID값을 넘기자!
	kCreateNpcInfo.m_kNpcInfo.m_UID = iCreatedNpcUID;
}

bool KBattleFieldMonsterManager::SetEventBossMonsterDie( IN const int iNpcUID )
{
	bool bRet = false;

	// 서버가 생성한 몬스터 이면 몬스터를 죽은 몬스터로 처리하자!
	{
		std::map<int, NPC_DATA>::iterator mit;
		mit = m_mapAliveEventBossList.find( iNpcUID );
		if( mit == m_mapAliveEventBossList.end() )
		{
			// 살아있는 몬스터 리스트에 없으면 실패!
			bRet = false;
		}

		// 살아있는 몬스터 리스트에서는 뺀다!
		m_mapAliveEventBossList.erase( mit );
		bRet = true;
	}

	if( bRet )
	{
		// 중보 관리 리스트 정보를 정리한다(해당 중보 삭제)
		std::vector< std::vector<KNPCUnitReq> >	vecTempEventBossList = m_vecEventBossListForClient;
		m_vecEventBossListForClient.clear();

		BOOST_TEST_FOREACH( std::vector<KNPCUnitReq>&, vecList, vecTempEventBossList )
		{
			std::vector<KNPCUnitReq> vecNew;

			BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
			{
				if( kNpc.m_UID != iNpcUID )
				{
					vecNew.push_back( kNpc );
				}
			}

			m_vecEventBossListForClient.push_back( vecNew );
		}
	}

	return bRet;
}

bool KBattleFieldMonsterManager::IsEventBossMonster( IN const int iNpcUID ) const
{
	if( m_mapAliveEventBossList.find( iNpcUID ) != m_mapAliveEventBossList.end() )
		return true;

	bool bRet = false;

	for( int index = 0 ; index < (int)m_vecEventBossListForClient.size() ; ++index )
	{
		std::vector<KNPCUnitReq> vecList = m_vecEventBossListForClient[index];

		BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
		{
			if( kNpc.m_UID == iNpcUID )
			{
				bRet = true;
				break;
			}
		}

		if( bRet == true )
			break;
	}

	return bRet;
}

bool KBattleFieldMonsterManager::GetEventBossNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const
{
	std::map<int, NPC_DATA>::const_iterator mit;
	mit = m_mapAliveEventBossList.find( iNpcUID );
	if( mit == m_mapAliveEventBossList.end() )
	{
		return false;
	}

	kNpcData = mit->second;
	return true;
}

void KBattleFieldMonsterManager::GetEventBossMonsterList( OUT std::vector< std::vector<KNPCUnitReq> >& vecNpcDataList )
{
	vecNpcDataList = m_vecEventBossListForClient;
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

#endif SERV_BATTLE_FIELD_SYSTEM
//}}


