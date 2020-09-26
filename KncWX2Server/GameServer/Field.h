#pragma once 

//////////////////////////////////////////////////////////////////////////
//081022.hoons.
//des : 1차 버전은 마을, 던전게이트 의 공통 유저관리만 체크하여 제작함.
//
//////////////////////////////////////////////////////////////////////////

#include "TRServer.h"
#include "BaseServer.h"
#include "Performer.h"
#include "FSM/support_FSM.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"

#include "FieldUserManager.h"

//{{ 2008. 12. 19  최육사	프로파일러
#include "Profiler/Profiler.h"
//}}

#define KFieldPtr KField*
#undef KFieldPtr
SmartPointer( KField );

#define _VERIFY_FIELD_STATE( varg, eventid, packet, neterror ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	kPacket.m_iOK = neterror; \
	SendToCharacter( LAST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \

#define VERIFY_FIELD_STATE( varg, eventid, packet ) _VERIFY_FIELD_STATE( varg, eventid, packet, NetError::ERR_ROOM_09 );

//{{ 2007. 8. 20  최육사  일반 채팅 전용 검증 매크로
#define VERIFY_FIELD_STATE_CHAT_ONLY( varg, eventid, packet ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	kPacket.m_iOK = NetError::ERR_ROOM_09; \
	kPacket.m_cChatPacketType = KEGS_CHAT_REQ::CPT_TOTAL; \
	SendToCharacter( FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \
	//}}


class KField : public KPerformer
{
	NiDeclareRTTI;
	DeclToStringW;
	DeclareFSM_MT;
	DeclPfID;

public:
	KField(void);
	virtual ~KField(void);

	// override.
	virtual	void	Tick();

	void			CloseField();

	void			SetFieldInfo( int iMapID, const KTileInfo& kTileInfo, const KMapInfo& kMapInfo );

	bool			IsEmpty() { return ( m_kFieldUserManager.GetNumMember() == 0 ); }

	//{{ 2009. 4. 20  최육사	좀비체크
	void			CheckZombieUser( UidType nCID );
	//}}
	
	// packet send function	
	template < class T > void SendToServer( unsigned short usEventID, const T& data );	
	void SendToServer( unsigned short usEventID );

	template < class T > void SendToCharacter( UidType nTo, unsigned short usEventID, const T& data );
	void SendToCharacter( UidType nTo, unsigned short usEventID );

	template < class T > void SendToTR( unsigned short usEventID, const T& data );

protected:
	virtual	void	ProcessEvent( const KEventPtr& spEvent ); // derived from KPerformer	

	////////////////////////////////////////////////////////////////////////// 
	// tcp relay function - SectorListType에는 vector와 set만 넣으세요
	//template < class T >
	//void BroadCastGSUser( const std::vector< UidType >& vecUnitList, u_short usEventID, const T& data );

	//void _BroadCastGSUser( const std::vector< UidType >& vecUnitList, const KEvent& kEvent );

	//////////////////////////////////////////////////////////////////////////
	// event handler
	DECL_ON_FUNC_NOPARAM( ERM_OPEN_FIELD_NOT );

    DECL_ON_FUNC( ERM_JOIN_FIELD_REQ );
	//{{ 2009. 6. 4  최육사		채널이동
   _DECL_ON_FUNC( ERM_JOIN_FIELD_CHANNEL_CHANGE_REQ, KERM_JOIN_FIELD_REQ );
	//}}

	DECL_ON_FUNC( ERM_LEAVE_FIELD_REQ );
   _DECL_ON_FUNC( ERM_FIELD_UNIT_SYNC_DATA_NOT, KEGS_FIELD_UNIT_SYNC_DATA_NOT );	
	DECL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT );
	//{{ 2008. 12. 9  최육사	친구리스트
	DECL_ON_FUNC( ERM_OPTION_UPDATE_NOT );
	DECL_ON_FUNC( ERM_UPDATE_PARTY_NOT );
	//}}
	//{{ 2008. 12. 13  최육사	채팅
   _DECL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ );
	//}}
	//{{ 2008. 12. 13  최육사	개인거래
	DECL_ON_FUNC( ERM_REQUEST_TRADE_NOT );
	DECL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT );
	//}}
	//{{ 2008. 12. 31  최육사	덤프
	DECL_ON_FUNC_NOPARAM( DMP_DUMP_EVENT_NOT );
	//}}
	//{{ 2009. 1. 15  최육사	유저 리스트
   _DECL_ON_FUNC( ERM_FIELD_USER_LIST_REQ, KEGS_COMMUNITY_USER_LIST_REQ );
	//}}
	//{{ 2009. 4. 14  최육사	친구리스트
	DECL_ON_FUNC( ERM_UPDATE_FRIEND_NOT );
	//}}
	//{{ 2009. 9. 23  최육사	1차전직캐쉬템
	DECL_ON_FUNC( ERM_CHANGE_FIELD_USER_INFO_NOT );
	//}}
	//{{ 2010. 7. 26  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	DECL_ON_FUNC( ERM_SUMMON_PET_NOT );
	DECL_ON_FUNC( ERM_PET_ACTION_NOT );
	DECL_ON_FUNC( ERM_PET_EVOLUTION_NOT );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	DECL_ON_FUNC( ERM_FIELD_USER_LIST_MONITORING_MODE_REQ );
#endif SERV_ED_MONITORING_IN_GAME
	//}}

#ifdef SERV_INVISIBLE_GM
	DECL_ON_FUNC( ERM_TOGGLE_INVISIBLE_NOT );
#endif SERV_INVISIBLE_GM

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	DECL_ON_FUNC( ERM_COUPLE_MAKING_SUCCESS_NOT );
	DECL_ON_FUNC_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT );
	_DECL_ON_FUNC( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT );
	DECL_ON_FUNC( ERM_CALL_MY_LOVER_LEAVE_FIELD_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	_DECL_ON_FUNC( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT );
	_DECL_ON_FUNC( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT );
#endif	// SERV_RIDING_PET_SYSTM

protected:
	int					m_iMapID;

	KFieldUserManager	m_kFieldUserManager;
};

template < class T >
void KField::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_FIELD, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KField::SendToCharacter( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_FIELD, 0, PI_GS_CHARACTER, nTo, NULL, usEventID, data );
}

//template < class T >
//void KField::BroadCastGSUser( const std::vector< UidType >& vecUnitList, u_short usEventID, const T& data )
//{
//	KEvent kEvent;  // _BroadCast에서 clone할 것이므로 여기선 heap 아니어도 됨.
//	kEvent.m_kDestPerformer.m_dwPerformerID = PI_GS_CHARACTER;
//	kEvent.m_usEventID = usEventID;
//
//	KSerializer ks;
//	kEvent.m_kbuff.Clear();
//
//	// serialize - only data
//	ks.BeginWriting( &kEvent.m_kbuff );
//	ks.Put( data );
//	ks.EndWriting();
//
//	_BroadCastGSUser( vecUnitList, kEvent );
//}

template < class T >
void KField::SendToTR( unsigned short usEventID, const T& data_ )
{
	//생성 시점은 이곳이고
	//삭제 시점은 TRUserManager::SendToRelay() 에서 처리후 삭제되어야함.
	KEventPtr spEvent( new KEvent() );

	spEvent->SetData( 0, NULL, usEventID, data_ );

	SiKTRServer()->QueueingRelayData( spEvent );
}

