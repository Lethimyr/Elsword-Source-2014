#pragma once

#include "Performer.h"
#include "FSM/support_FSM.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"
#include "X2Data/XSLRoom.h"

#define KCnBaseRoomPtr KCnBaseRoom*
#undef KCnBaseRoomPtr
SmartPointer( KCnBaseRoom );

//////////////////////////////////////////////////////////////////////////
// room state err
#define _VERIFY_ROOM_STATE( varg, eventid, packet, neterror ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	kPacket.m_iOK = neterror; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \

#define VERIFY_ROOM_STATE( varg, eventid, packet ) _VERIFY_ROOM_STATE( varg, eventid, packet, NetError::ERR_ROOM_09 );

//////////////////////////////////////////////////////////////////////////
// room state warn
#define _VERIFY_ROOM_STATE_WARN( varg, eventid, packet, neterror ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cwarn, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	kPacket.m_iOK = neterror; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \

#define VERIFY_ROOM_STATE_WARN( varg, eventid, packet ) _VERIFY_ROOM_STATE_WARN( varg, eventid, packet, NetError::ERR_ROOM_09 );

//////////////////////////////////////////////////////////////////////////
//결과창일때 날아와도 오류가 아닐경우에 사용 
#define _VERIFY_ROOM_STATE_AND_WARN( varg, warg, eventid, packet, vneterror ) \
	packet kPacket; \
	kPacket.m_iOK = NetError::NET_OK; \
	if( m_pkCurrentState->CheckState warg ) \
{ \
	START_LOG_WITH_NAME( cwarn ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #warg << END_LOG; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
else if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	kPacket.m_iOK = vneterror; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \

#define VERIFY_ROOM_STATE_AND_WARN( varg, warg, eventid, packet ) _VERIFY_ROOM_STATE_AND_WARN( varg, warg, eventid, packet, NetError::ERR_ROOM_09 );

//{{ 2007. 8. 20  최육사  일반 채팅 전용 검증 매크로
#define VERIFY_ROOM_STATE_CHAT_ONLY( varg, eventid, packet ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	kPacket.m_iOK = NetError::ERR_ROOM_09; \
	kPacket.m_cChatPacketType = KEGS_CHAT_REQ::CPT_TOTAL; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \
	//}}

class KCnBaseRoom : public KPerformer
{
	NiDeclareRTTI;
	DeclToStringW;
	DeclareFSM_MT;
	DeclPfID;

public:
	KCnBaseRoom();
	virtual ~KCnBaseRoom();

	// override.
	virtual void Tick();

	void SetUDPRelayInfo( const KNetAddress& kAddress );

	virtual void CloseRoom();

	// packet send function
	template < class T > void SendToGSServer( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
#endif SERV_TRADE_DDOS_DEFENCE
	//}}
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	template < class T > void BroadCastAllGSServer( unsigned short usEventID, const T& data );
	void SendToGSServer( UidType nTo, unsigned short usEventID );
	void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID );
	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	void SendToGameDB( unsigned short usEventID );
#endif SERV_TRADE_DDOS_DEFENCE
	//}}
	void SendToLogDB( unsigned short usEventID );
	void BroadCastAllGSServer( unsigned short usEventID );

protected:
	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
	void SendRoomCountInfo( CXSLRoom::ROOM_TYPE eType, bool bOpen );
#endif SERV_ROOM_COUNT
	//}}

public:

protected:
	std::wstring						m_wstrUDPRelayIP;
	u_short								m_usUDPRelayPort;
};

template < class T >
void KCnBaseRoom::SendToGSServer( UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nTo, -1 };
	KncSend( PI_CN_ROOM, GetUID(), PI_GS_SERVER, nTo, anTrace, usEventID, data );
}

template < class T >
void KCnBaseRoom::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_CN_ROOM, GetUID(), PI_GS_CHARACTER, nTo, anTrace, usEventID, data );
}

//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
template < class T >
void KCnBaseRoom::SendToGameDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_CN_ROOM, GetUID(), PC_GAME_DB, 0, anTrace, usEventID, data );
}
#endif SERV_TRADE_DDOS_DEFENCE
//}}

template < class T >
void KCnBaseRoom::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_CN_ROOM, GetUID(), PC_LOG_DB, 0, NULL, usEventID, data );
}

template < class T >
void KCnBaseRoom::BroadCastAllGSServer( unsigned short usEventID, const T& data )
{
	SendToGSServer( 0, usEventID, data );
}

