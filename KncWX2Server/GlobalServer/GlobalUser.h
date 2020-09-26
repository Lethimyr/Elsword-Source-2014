#pragma once
#include "Actor.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ActorManager.h"

#include <windows.h>    // ::GetTickCount()


SmartPointer(KGlobalUser);

class KGlobalUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KGlobalUser(void);
    virtual ~KGlobalUser(void);

    // override
    virtual void Tick();

protected:  // util function, Packet Handling	
	
	bool AuthCheckPassword( const wchar_t* szPassword, UINT32 uiPwdHash );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-09	// 박세훈
	int		BossFieldOpenProcess( IN const int iVillageMapID, IN const __time64_t tCurrentTime ) const;
	int		TotalDangerousValueUpdateProcess( IN const int iBattleFieldID, IN const byte byteValue, IN const __time64_t tCurrentTime ) const;
#endif // SERV_BATTLE_FIELD_BOSS

    // packet send function
    template < typename T > bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );

	//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	template < class T > void SendToGameDB( unsigned short usEventID_, const T& data_ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
    template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
    
    template < class T > void SendToGameServer( unsigned short usEventID, const T& data );

	//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	template < class T > void SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 03. 29	김민성		중복 sn 접속 감시
//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	template < class T > void SendToGSUser( UidType nToGSUser, unsigned short usEventID, const T& data );
	template < class T > void SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID, const T& data );
//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	//}}

	//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
   _DECL_ON_FUNC( EGB_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}

	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	_DECL_ON_FUNC( ELG_DISCONNECT_SERVER_REPORT_NOT, KE_DISCONNECT_SERVER_REPORT_NOT );
#endif SERV_MORNITORING
	//}}
	DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	DECL_ON_FUNC( EGB_GET_MISSION_INFO_REQ );
	_DECL_ON_FUNC( EGB_CLEAR_MISSION_DUNGEON_NOT, KEGB_CLEAR_MISSION_DUNGEON_NOT );
	_DECL_ON_FUNC( EGB_UPDATE_CCU_INFO_NOT, KELG_UPDATE_CCU_INFO_NOT );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	_DECL_ON_FUNC( EGB_TODAY_RANDOM_QUEST_REQ, UidType );
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ 2012. 03. 29	김민성		중복 sn 접속 감시
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	DECL_ON_FUNC( EGB_REGISTER_SERVERSN_REQ );
	DECL_ON_FUNC( EGB_UNREGISTER_SERVERSN_NOT );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	//}}
	//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( EGB_REGISTER_UNIT_NOT );
	DECL_ON_FUNC( EGB_UNIT_DISCONNECT_NOT );

   _DECL_ON_FUNC( EGB_INVITE_PARTY_FIND_RECEIVER_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ );
   _DECL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
   _DECL_ON_FUNC( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   //{{ 2012. 11. 21	박세훈	영웅 대전 참가자 관리용 리스트
#ifdef SERV_HERO_PVP_MANAGE_LIST
   DECL_ON_FUNC_NOPARAM( EGB_ADMIN_HERO_PVP_USER_LIST_REQ );
   _DECL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_ADD_REQ, KEGS_ADMIN_HERO_PVP_USER_ADD_REQ );
   _DECL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_REMOVE_REQ, KEGS_ADMIN_HERO_PVP_USER_REMOVE_REQ );
   _DECL_ON_FUNC( EGB_ADMIN_NOTIFY_HERO_PVP_USER_REQ, KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ );
   DECL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_NOT );
#endif SERV_HERO_PVP_MANAGE_LIST
   //}}

	//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	DECL_ON_FUNC( EGB_CHECK_BLOCK_USER_NOT );
   _DECL_ON_FUNC( EGB_MODULE_INFO_UPDATE_NOT, KEGS_MODULE_INFO_UPDATE_NOT );
#endif SERV_BLOCK_LIST
	//}}
   //{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
   DECL_ON_FUNC( EGB_GET_GLOBAL_MISSION_INFO_REQ );
   _DECL_ON_FUNC( EGB_GLOBAL_MISSION_UPDATE_NOT, KEGB_GLOBAL_MISSION_UPDATE_NOT );
#endif SERV_GLOBAL_MISSION_MANAGER
   //}} 2012. 09. 03	임홍락	글로벌 미션 매니저
   
#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
   DECL_ON_FUNC_NOPARAM( EGB_TIME_ENCHANT_EVENT_INFO_REQ );
   DECL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_NOT );
   DECL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-30	// 박세훈
   DECL_ON_FUNC_NOPARAM( EGB_EXCHANGE_LIMIT_INFO_REQ );
   DECL_ON_FUNC( EGB_EXCHANGE_LIMIT_REQ );
   DECL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK );
   DECL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-30	// 박세훈
   DECL_ON_FUNC( EGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT );
   DECL_ON_FUNC_NOPARAM( EGB_BATTLE_FIELD_BOSS_INFO_NOT );

   DECL_ON_FUNC( EGB_ADMIN_BOSS_FIELD_GATE_OPEN_REQ );
   DECL_ON_FUNC_NOPARAM( EGB_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT );
   DECL_ON_FUNC( EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ );
   DECL_ON_FUNC( EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ );
#endif // SERV_BATTLE_FIELD_BOSS
	    
    bool RoutePacket( const KEvent* pkEvent );   // CnUser, GSUser가 같이 선언하지만 상속은 아님.
    //                          ^ KEvent가 수정되지 않아야 한다. (SmartPtr로는 불가능)

    // override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED 상태로 빠져나오고, DB data update를 신청한다 

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
private:
	bool			m_bConnectGameServer;

#endif SERV_RANDOM_DAY_QUEST
	//}}
};

template < typename T >
bool KGlobalUser::SendPacket( unsigned short usEventID, T& data, bool bLogging, bool bCompress )
{
    return KSession::SendPacket( PI_GLOBAL_PROXY, GetUID(), NULL, usEventID, data, bLogging, bCompress );
}

//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
template < class T >
void KGlobalUser::SendToGameDB( unsigned short usEventID_, const T& data_ )
{
	UidType anTrace[2] = { -1, -1 };

	if( anTrace[0] == -1 )
		anTrace[0] = GetUID();
	else
		anTrace[1] = GetUID();

	KncSend( GetPfID(), GetUID(), PI_GLOBAL_GAME_DB, 0, anTrace, usEventID_, data_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

template < class T >
void KGlobalUser::SendToLogDB( unsigned short usEventID_, const T& data_ )
{
    UidType anTrace[2] = { -1, -1 };

    if( anTrace[0] == -1 )
        anTrace[0] = GetUID();
    else
        anTrace[1] = GetUID();

    KncSend( PI_GLOBAL_USER, GetUID(), PI_LOG_DB, 0, anTrace, usEventID_, data_ );
}

template < class T >
void KGlobalUser::SendToGameServer( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KEvent kEvent;
	kEvent.SetData( PI_GS_SERVER, anTrace, usEventID, data );
	KSession::SendPacket( kEvent );
}

//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
template < class T >
void KGlobalUser::SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_CHARACTER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToUnitUID ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"타 게임서버로 패킷 보내기 실패.? 왜왜" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToUnitUID )
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 03. 29	김민성		중복 sn 접속 감시
//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//다른 게임서버로 자료를 보내기 위해
template < class T >
void KGlobalUser::SendToGSUser( UidType nToGSUser, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGSUser ) );
	KSession::SendPacket( kEvent );
}

template < class T >
void KGlobalUser::SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGSUser ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"타 게임서버로 패킷 보내기 실패.? 왜왜" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToGSUser )			
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}



