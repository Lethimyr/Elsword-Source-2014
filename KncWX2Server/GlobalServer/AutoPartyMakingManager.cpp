#include "AutoPartyMakingManager.h"
#include "NetError.h"

#include "GlobalServer.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "X2Data/XSLRoom.h"
#include "AutoPartyFSM.h"
#include "AutoPartyUserFSM.h"
#include "CompareLimitList.h"
#include "GlobalScriptRefreshManager.h"
//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#else
	#include "AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}
#include "X2Data/XSLDungeonManager.h"


//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


//////////////////////////////////////////////////////////////////////////
#define CLASS_TYPE KAutoPartyMakingManager
ImplPfID( KAutoPartyMakingManager, PI_GLOBAL_AUTO_PARTY );

ImplementSingleton( KAutoPartyMakingManager );


KAutoPartyMakingManager::KAutoPartyMakingManager()
{
	lua_tinker::class_add<KAutoPartyMakingManager>( g_pLua, "KAutoPartyMakingManager" );
	lua_tinker::class_def<KAutoPartyMakingManager>( g_pLua, "Command",					&KAutoPartyMakingManager::Command_LUA );
	lua_tinker::class_def<KAutoPartyMakingManager>( g_pLua, "dump",						&KAutoPartyMakingManager::Dump );

	lua_tinker::decl( g_pLua, "AutoPartyManager", this );
}

KAutoPartyMakingManager::~KAutoPartyMakingManager()
{
}

ImplToStringW( KAutoPartyMakingManager )
{
	stm_ << L"-------------- Auto Party Making Manager --------------" << std::endl
		<< TOSTRINGW( m_mapAutoParty.size() )
		<< TOSTRINGW( m_vecAutoParty.size() )
		<< TOSTRINGW( m_mapAutoPartyMakingProcess.size() )
		;

	return stm_;
}

void KAutoPartyMakingManager::Command_LUA( char cType )
{
    START_LOG( cout, L"자동 파티 메이킹 개발자 전용 커맨드!" )
		<< BUILD_LOGc( cType );

	KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT kPacket;
	kPacket.m_cType = cType;
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GLOBAL_MATCH_MAKING, NULL, EGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT, kPacket );
	QueueingEvent( spEvent );
}

void KAutoPartyMakingManager::Run()
{
	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 20 );       // 0.02s 간격

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

	}

	return;
}

void KAutoPartyMakingManager::ShutDown()
{
	End();
}

bool KAutoPartyMakingManager::Init()
{
	// 스크립트 파싱이 제대로 안되었다면 동작 시키지 말자!
	const std::vector< KAutoPartyMakingProcessCondition >& vecAutoPartyDungeonInfo = SiKAutoPartyScriptManager()->GetAutoPartyDungeonInfo();
	if( vecAutoPartyDungeonInfo.empty() == true )
	{
		START_LOG( cerr, L"자동 파티 스크립트가 제대로 파싱되지 않았습니다! 자동파티 매니저를 동작시키지 않습니다!" )
			<< END_LOG;
		return false;
	}

	// 세팅에 따라 AutoPartyMakingProcess를 생성!
	BOOST_TEST_FOREACH( const KAutoPartyMakingProcessCondition&, kInfo, vecAutoPartyDungeonInfo )
	{
		KMakingProcessKey kKey( kInfo.m_iDungeonMode, kInfo.m_iDungeonIDWithDif );
		std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
		mit = m_mapAutoPartyMakingProcess.find( kKey );
		if( mit == m_mapAutoPartyMakingProcess.end() )
		{
			m_mapAutoPartyMakingProcess.insert( std::make_pair( kKey, KAutoPartyMakingProcess( kInfo ) ) );
		}
		else
		{
			mit->second.AddLevelCondition( kInfo.m_iBeginLevel, kInfo.m_iEndLevel );
		}
	}

	// DungeonData.lua에 등록되어있는 일반 던전 정보를 참고로 하여 AutoPartyMakingProcess를 생성하자!
	{
		std::vector< int > vecNormalDungeonIDList;
		SiCXSLDungeonManager()->GetNormalDungeonIDList( vecNormalDungeonIDList );

		BOOST_TEST_FOREACH( const int, iDungeonIDWithDif, vecNormalDungeonIDList )
		{
			// 튜토리얼 제외
			if( CXSLDungeon::IsTutorialDungeon( iDungeonIDWithDif ) == true )
				continue;

			//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
			const int iDifficultyLevel = iDungeonIDWithDif % 10;
			if( iDifficultyLevel == CXSLDungeon::DL_HARD  ||  iDifficultyLevel == CXSLDungeon::DL_EXPERT )
				continue;
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
			//}}

			KMakingProcessKey kKey( CXSLDungeon::DM_INVALID, iDungeonIDWithDif );
			std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
			mit = m_mapAutoPartyMakingProcess.find( kKey );
			if( mit == m_mapAutoPartyMakingProcess.end() )
			{
				KAutoPartyMakingProcessCondition kInfo;
				kInfo.m_iDungeonMode = CXSLDungeon::DM_INVALID;
				kInfo.m_iDungeonIDWithDif = iDungeonIDWithDif;
				kInfo.m_iBeginLevel = 1;
				kInfo.m_iEndLevel = 60; // 일단은 하드코딩이지만 실제 스크립트 정보를 땡겨와야함.
				m_mapAutoPartyMakingProcess.insert( std::make_pair( kKey, KAutoPartyMakingProcess( kInfo ) ) );
			}
			else
			{
				START_LOG( cerr, L"일반던전인데 동일한 DungeonID값이 존재해서는 안됩니다!" )
					<< BUILD_LOG( iDungeonIDWithDif )
					<< END_LOG;
			}
		}
	}

	return KThread::Begin();
}

void KAutoPartyMakingManager::ClearAllAutoPartyInfo()
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout2, L"[경고!] 모든 자동 파티 정보를 초기화 시킵니다!" );
	//////////////////////////////////////////////////////////////////////////	

	// 자동 파티 종료 처리!
	std::vector< UidType > vecAutoPartyList;
	BOOST_TEST_FOREACH( KAutoPartyPtr, spAutoParty, m_vecAutoParty )
	{
		if( !spAutoParty )
			continue;

		vecAutoPartyList.push_back( spAutoParty->GetAutoPartyUID() );
	}

	BOOST_TEST_FOREACH( const UidType, iAutoPartyUID, vecAutoPartyList )
	{
		DestroyAutoParty( iAutoPartyUID );
	}

	// 대기자 리스트 종료 처리
	std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
	for( mit = m_mapAutoPartyMakingProcess.begin(); mit != m_mapAutoPartyMakingProcess.end(); ++mit )
	{
		mit->second.ClearAllAutoPartyInfo();
	}
}

void KAutoPartyMakingManager::Tick()
{
	//////////////////////////////////////////////////////////////////////////	
	// AutoPartyMakingManager이벤트를 먼저 처리한뒤..
	KPerformer::Tick();

	
	//////////////////////////////////////////////////////////////////////////	
	// 자동 파티 Tick을 처리하자!	
	std::vector< UidType > vecDeleteAutoPartyUIDList;
	BOOST_TEST_FOREACH( KAutoPartyPtr, spAutoParty, m_vecAutoParty )
	{
		if( spAutoParty == NULL )
			continue;

		spAutoParty->Tick();

		// 마지막 상태가 S_CLOSE라면 종료 리스트에 AutoPartyUID값 넣자!
		if( spAutoParty->GetStateID() == KAutoPartyFSM::S_CLOSE )
		{
			vecDeleteAutoPartyUIDList.push_back( spAutoParty->GetUID() );
		}
	}


	//////////////////////////////////////////////////////////////////////////	
	// 자동 파티 종료 처리!
	BOOST_TEST_FOREACH( const UidType, iAutoPartyUID, vecDeleteAutoPartyUIDList )
	{
        DestroyAutoParty( iAutoPartyUID );
	}


	//////////////////////////////////////////////////////////////////////////	
	// 자동 파티 메이킹 프로세스!
	std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
	for( mit = m_mapAutoPartyMakingProcess.begin(); mit != m_mapAutoPartyMakingProcess.end(); ++mit )
	{
		// tick
		mit->second.Tick();

		// 자동 파티 생성 작업!
		std::vector< KAutoPartyPtr > vecMadeAutoParty;
		mit->second.MakingAutoPartyProcessing( vecMadeAutoParty );

		// 생성된 자동 파티를 리스트에 추가!
		BOOST_TEST_FOREACH( KAutoPartyPtr, spAutoParty, vecMadeAutoParty )
		{
			LIF( AddAutoParty( spAutoParty ) );
		}
	}
}

KAutoPartyMakingProcess* KAutoPartyMakingManager::GetAutoPartyMakingProcess( IN const int iDungeonMode, IN const int iDungeonIDWithDif )
{
	switch( iDungeonMode )
	{
		// 헤니르 시공
	case CXSLDungeon::DM_HENIR_PRACTICE:
	case CXSLDungeon::DM_HENIR_CHALLENGE:
		{
			KMakingProcessKey kKey( iDungeonMode, SEnum::DI_ELDER_HENIR_SPACE );

			std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
			mit	= m_mapAutoPartyMakingProcess.find( kKey );
			if( mit	== m_mapAutoPartyMakingProcess.end() )
			{
				START_LOG( cerr, L"해당 던전ID는 자동 매칭 할수 없습니다! " )
					<< BUILD_LOGc( iDungeonMode )
					<< BUILD_LOG( iDungeonIDWithDif )
					<< END_LOG;

				return NULL;
			}

			return &mit->second;
		}
		break;

		// 비밀던전
	case CXSLDungeon::DM_SECRET_NORMAL:
	case CXSLDungeon::DM_SECRET_HELL:
		{
			KMakingProcessKey kKey( iDungeonMode, iDungeonIDWithDif );

			std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
			mit	= m_mapAutoPartyMakingProcess.find( kKey );
			if( mit	== m_mapAutoPartyMakingProcess.end() )
			{
				START_LOG( cerr, L"해당 던전ID는 자동 매칭 할수 없습니다! " )
					<< BUILD_LOGc( iDungeonMode )
					<< BUILD_LOG( iDungeonIDWithDif )
					<< END_LOG;

				return NULL;
			}

			return &mit->second;
		}
		break;
	}

	// 일반 던전
	KMakingProcessKey kKey( CXSLDungeon::DM_INVALID, iDungeonIDWithDif );

	std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
	mit	= m_mapAutoPartyMakingProcess.find( kKey );
	if( mit	== m_mapAutoPartyMakingProcess.end() )
	{
		START_LOG( cerr, L"해당 던전ID는 자동 매칭 할수 없습니다! " )
			<< BUILD_LOGc( iDungeonMode )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}

KAutoPartyMakingProcess* KAutoPartyMakingManager::GetAutoPartyMakingProcess( IN const UidType iAutoPartyWaitNumber )
{
	const int iDungeonID = static_cast<int>(KAutoPartyMakingProcess::ExtractDungeonIDAtWaitNumber( iAutoPartyWaitNumber ));
	const int iDungeonMode = static_cast<char>(KAutoPartyMakingProcess::ExtractDungeonModeAtWaitNumber( iAutoPartyWaitNumber ));
	return GetAutoPartyMakingProcess( iDungeonMode, iDungeonID );
}

void KAutoPartyMakingManager::RefreshTimerAutoPartyGameStart( IN const int iDungeonMode, IN const int iDungeonIDWithDif )
{
	KAutoPartyMakingProcess* pAutoPartyMakingProcess = GetAutoPartyMakingProcess( iDungeonMode, iDungeonIDWithDif );
	if( IS_NULL( pAutoPartyMakingProcess ) )
	{
		START_LOG( cerr, L"존재 하지 않는 자동파티 던전 정보입니다!" )
			<< BUILD_LOG( iDungeonMode )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< END_LOG;
		return;
	}
	
	pAutoPartyMakingProcess->RestartGameStartLastTime();
}

KAutoPartyPtr KAutoPartyMakingManager::GetAutoParty( IN const UidType iAutoPartyUID )
{
	std::map< UidType, KAutoPartyPtr >::iterator mit = m_mapAutoParty.find( iAutoPartyUID );
	if( mit == m_mapAutoParty.end() )
	{
		START_LOG( cwarn, L"찾는 매치가 없습니다.!" )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 포인터 이상.!" )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	return mit->second;
}

bool KAutoPartyMakingManager::AddAutoParty( IN KAutoPartyPtr spAutoParty )
{
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"추가 하려는 자동 파티 정보가 이상합니다!" )
			<< END_LOG;
		return false;
	}

	if( GetAutoParty( spAutoParty->GetAutoPartyUID() ) != NULL )
	{
		START_LOG( cerr, L"이미 해당 AutoPartyUID값이 리스트에 존재합니다. 기존의 파티정보 역시 삭제하겠습니다." )
			<< BUILD_LOG( spAutoParty->GetAutoPartyUID() )
			<< END_LOG;
		
		// 기존의 정보 삭제처리
		DestroyAutoParty( spAutoParty->GetAutoPartyUID() );
	}

	m_mapAutoParty.insert( std::make_pair( spAutoParty->GetAutoPartyUID(), spAutoParty ) );
	m_vecAutoParty.push_back( spAutoParty );
	return true;
}

void KAutoPartyMakingManager::DestroyAutoParty( IN const UidType iAutoPartyUID )
{
	KAutoPartyPtr spAutoParty = GetAutoParty( iAutoPartyUID );
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"삭제하려는 매치 정보가 없습니다!" )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;
		return;
	}

	std::vector< KAutoPartyPtr >::iterator vit;
	for( vit = m_vecAutoParty.begin(); vit != m_vecAutoParty.end(); ++vit )
	{
		KAutoPartyPtr spAutoParty = *vit;
		if( spAutoParty == NULL )
			continue;

		if( spAutoParty->GetUID() == iAutoPartyUID )
			break;
	}

	if( vit != m_vecAutoParty.end() )
	{
		m_vecAutoParty.erase( vit );
	}

	m_mapAutoParty.erase( iAutoPartyUID );
}

void KAutoPartyMakingManager::QueueingToAll( IN const KEventPtr& spEvent )
{
	std::map< UidType, KAutoPartyPtr >::iterator mit;
	for( mit = m_mapAutoParty.begin(); mit != m_mapAutoParty.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"파티 포인터 이상" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mit->second->QueueingEvent( spEvent );
	}
}

void KAutoPartyMakingManager::ProcessEvent( IN const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
		CASE_NOPARAM( EGB_GET_AUTO_PARTY_BONUS_INFO_REQ );
		CASE( EGB_REQUEST_AUTO_PARTY_MAKING_REQ );
		CASE( EGB_CANCEL_AUTO_PARTY_MAKING_REQ );
		CASE_NOPARAM( EGB_CHECK_INVALID_AUTO_PARTY_NOT );
		CASE( EGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT );
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
		//{{ 2012. 11. 13	최육사		자동파티 대기열 카운트 표시
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		CASE( EGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT );
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		//}}

	default:
		{
			const UidType iAutoPartyUID = spEvent_->m_kDestPerformer.GetFirstUID();
			KAutoPartyPtr spAutoParty = GetAutoParty( iAutoPartyUID );
			if( spAutoParty == NULL )
			{
				START_LOG( cwarn, L"매치 검색 실패." )
					<< BUILD_LOG( iAutoPartyUID )
					<< BUILD_LOG( spEvent_->GetIDStr() )
					<< END_LOG;
				break;
			}

			// 자동파티에 직접 event전달
			spAutoParty->QueueingEvent( spEvent_ );
		}
		break;
	}
}

IMPL_ON_FUNC_NOPARAM( EGB_GET_AUTO_PARTY_BONUS_INFO_REQ )
{
    // 현재 자동 파티 이벤트 여부를 게임서버로 보내준다!
	KEGB_GET_AUTO_PARTY_BONUS_INFO_ACK kPacket;

	std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
	for( mit = m_mapAutoPartyMakingProcess.begin(); mit != m_mapAutoPartyMakingProcess.end(); ++mit )
	{
		KAutoPartyBonusInfo kInfo;
		kInfo.m_cDungeonMode = mit->second.GetDungeonMode();
		kInfo.m_iDungeonIDWithDif = mit->second.GetDungeonIDWithDif();
		kInfo.m_bActivatedBonus = mit->second.IsActivatedAutoPartyBonus();
        kPacket.m_vecAutoPartyBonusInfo.push_back( kInfo );
	}
	SendToGameServer( LAST_SENDER_UID, EGB_GET_AUTO_PARTY_BONUS_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( EGB_REQUEST_AUTO_PARTY_MAKING_REQ )
{
	// 매치 메이킹!
	KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	const int iDungeonMode = kPacket_.m_kAutoPartyCondition.m_cDungeonMode;
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	int iDungeonIDWithDif = kPacket_.m_kAutoPartyCondition.m_iDungeonID + static_cast<int>(kPacket_.m_kAutoPartyCondition.m_cDifficultyLevel);
	switch( iDungeonMode )
	{
	case CXSLDungeon::DM_HENIR_PRACTICE:
	case CXSLDungeon::DM_HENIR_CHALLENGE:
	case CXSLDungeon::DM_SECRET_NORMAL:
	case CXSLDungeon::DM_SECRET_HELL:
		break;

	default:
		{
			iDungeonIDWithDif = iDungeonIDWithDif - ( iDungeonIDWithDif % 10 ); // 난이도값 제거
		}
		break;
	}
#else
	const int iDungeonIDWithDif = kPacket_.m_kAutoPartyCondition.m_iDungeonID + static_cast<int>(kPacket_.m_kAutoPartyCondition.m_cDifficultyLevel);
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}	
	
	KAutoPartyMakingProcess* pAutoPartyMakingProcess = GetAutoPartyMakingProcess( iDungeonMode, iDungeonIDWithDif );
	if( pAutoPartyMakingProcess == NULL )
	{
		START_LOG( cerr, L"해당 자동 파티 요청 정보로는 자동 파티 신청을 할수 없습니다! " )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iDungeonMode )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_AUTO_PARTY_04;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}

	// 매치 대기자 리스트에 등록!
	UidType iAutoPartyWaitNumber = 0;
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	if( pAutoPartyMakingProcess->RegAutoPartyWaitList( kPacket_.m_vecAutoPartyUser, kPacket_.m_kAutoPartyCondition, iAutoPartyWaitNumber ) == false )
#else
	if( pAutoPartyMakingProcess->RegAutoPartyWaitList( kPacket_.m_vecAutoPartyUser, iAutoPartyWaitNumber ) == false )
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	{
		START_LOG( cerr, L"자동 파티 대기자 리스트에 등록이 실패하였습니다!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecAutoPartyUser.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_AUTO_PARTY_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}

	// 결과 전송
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_AUTO_PARTY_MAKING_ACK, kPacket );

	// 자동 파티 대기자 등록 성공에 대해서 알림
	BOOST_TEST_FOREACH( const KAutoPartyUserInfo&, kInfo, kPacket_.m_vecAutoPartyUser )
	{
		KEGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT kPacketNot;
		kPacketNot.m_bRemakingAutoParty = kPacket_.m_bRemakingAutoParty; // 재등록 여부!
		kPacketNot.m_iWaitNumber = iAutoPartyWaitNumber;
		kPacketNot.m_iEstimatedTime = pAutoPartyMakingProcess->GetAutoPartyMakingEstimatedTime();
		//{{ 2012. 11. 13	최육사		자동파티 대기열 카운트 표시
//#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		//{{ 2012. 11. 21	최육사		대기인원 표시 레벨 구간으로 수정
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
		kPacketNot.m_iAutoPartyWaitUserCount = pAutoPartyMakingProcess->GetWaitUserCountByWaitNumber( iAutoPartyWaitNumber );
#else
		kPacketNot.m_iAutoPartyWaitUserCount = pAutoPartyMakingProcess->GetWaitUserCount();
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
		//}}		
//#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		//}}
		SendToGSCharacter( kInfo.m_iGSUID, kInfo.m_iUnitUID, EGB_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGB_CANCEL_AUTO_PARTY_MAKING_REQ )
{
	// WaitNumber에 대한 처리!
	if( kPacket_.m_iAutoPartyWaitNumber != 0 )
	{
		// 해당 매니저를 찾아서 등록 삭제하라!
		KAutoPartyMakingProcess* pAutoPartyMakingProcess = GetAutoPartyMakingProcess( kPacket_.m_iAutoPartyWaitNumber );
		if( pAutoPartyMakingProcess != NULL )
		{
			// 시스템이 보낸 패킷이 아닌경우 ACK전송
			if( kPacket_.m_bSendBySystem == false )
			{
				KEGS_CANCEL_AUTO_PARTY_MAKING_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_CANCEL_AUTO_PARTY_MAKING_ACK, kPacket );
			}

			if( pAutoPartyMakingProcess->UnRegAutoPartyWaitListByWaitNumber( kPacket_.m_iAutoPartyWaitNumber, kPacket_.m_iReason ) == false )
			{
				START_LOG( cerr, L"자동 파티 신청 해제 실패!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
					<< BUILD_LOG( kPacket_.m_iReason )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"자동 파티 신청 해제 하려는데 자동 파티 등록 정보가 없네? 시간차 문제인가?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
				<< BUILD_LOG( kPacket_.m_iReason )
				<< END_LOG;
		}

		if( kPacket_.m_iAutoPartyUID != 0 )
		{
			START_LOG( cerr, L"대기인원인데 자동파티에 소속되어있네? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
				<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
				<< END_LOG;
			// 로그만 찍자
		}
	}

	// AutoPartyUID에 대한 처리!
	if( kPacket_.m_iAutoPartyUID != 0 )
	{
		KAutoPartyPtr spAutoParty = GetAutoParty( kPacket_.m_iAutoPartyUID );
		if( spAutoParty != NULL )
		{
			// 매치에 event전달
			KEventPtr spEvent( new KEvent );
			UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };
			spEvent->SetData( PI_GLOBAL_MATCH_MAKING, anTrace, EGB_CANCEL_AUTO_PARTY_MAKING_REQ, kPacket_ );
			spAutoParty->QueueingEvent( spEvent );
			return;
		}
		else
		{
			START_LOG( cerr, L"자동 파티 이탈 처리 하려는데 자동 파티 정보가 없네? 시간차 문제인가?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
				<< END_LOG;
		}
	}
}

IMPL_ON_FUNC_NOPARAM( EGB_CHECK_INVALID_AUTO_PARTY_NOT )
{
    START_LOG( cout, L"GlobalUser객체가 소멸되어 모든 자동 파티 정보를 삭제합니다!" )
		<< END_LOG;

	ClearAllAutoPartyInfo();
}

IMPL_ON_FUNC( EGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT )
{
    START_LOG( cout, L"개발자 커맨드에 의한 동작 시작!" )
		<< BUILD_LOGc( kPacket_.m_cType );

	switch( kPacket_.m_cType )
	{
	case KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT::CT_DUMP:
		{

		}
		break;

	case KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT::CT_CLEAR:
		{
            ClearAllAutoPartyInfo();
		}
		break;

	default:
		{
			START_LOG( cerr, L"잘못된 커맨드 입니다!" )
				<< BUILD_LOGc( kPacket_.m_cType )
				<< END_LOG;
		}
		break;
	}
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_AUTO_PARTY_SCRIPT_MANAGER, KAutoPartyScriptManager );

	default:
		{
			START_LOG( cerr, L"이쪽으로 오면 안되는 타입인데?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 11. 13	최육사		자동파티 대기열 카운트 표시
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
IMPL_ON_FUNC( EGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT )
{
	// 정상적인 WaitNumber인지 체크
	if( kPacket_.m_iAutoPartyWaitNumber == 0 )
	{
		START_LOG( cerr, L"WaitNumber가 0인데 대기열 카운트 얻으러 왔네?" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
			<< END_LOG;
		return;
	}

	// 해당 매니저를 찾아서 등록 삭제하라!
	KAutoPartyMakingProcess* pAutoPartyMakingProcess = GetAutoPartyMakingProcess( kPacket_.m_iAutoPartyWaitNumber );
	if( IS_NULL( pAutoPartyMakingProcess ) )
	{
		START_LOG( cerr, L"대기열 카운트 얻기 실패!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
			<< END_LOG;
		return;
	}

	// 시스템이 보낸 패킷이 아닌경우 ACK전송
	KEGS_AUTO_PARTY_WAIT_USER_COUNT_NOT kPacket;
	//{{ 2012. 11. 21	최육사		대기인원 표시 레벨 구간으로 수정
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	kPacket.m_iAutoPartyWaitUserCount = pAutoPartyMakingProcess->GetWaitUserCountByWaitNumber( kPacket_.m_iAutoPartyWaitNumber );
#else
	kPacket.m_iAutoPartyWaitUserCount = pAutoPartyMakingProcess->GetWaitUserCount();
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}	
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_AUTO_PARTY_WAIT_USER_COUNT_NOT, kPacket );
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
//}}



