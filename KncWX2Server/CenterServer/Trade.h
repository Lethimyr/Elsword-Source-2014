#pragma once

#include "CnBaseRoom.h"

#include <boost/timer.hpp>

#include "TradeUserManager.h"

SmartPointer( KTrade );

class KTrade : public KCnBaseRoom
{
	NiDeclareRTTI;
	DeclToStringW;

public:
	enum ENUM_TIMER
	{
		TM_WAIT_OTHER,
		TM_TOTAL_NUM,
	};

	KTrade(void);
	virtual ~KTrade(void);

	virtual void CloseRoom();
	virtual void Tick();

	bool IsEmpty();
	bool IsFull();

protected:
	void BreakTrade( int iReason );

	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer

	template < class T >
	void BroadCast( u_short usEventID, const T& data );
	void BroadCastID( u_short usEventID );
	void _BroadCast( const KEvent& kEvent );

	//////////////////////////////////////////////////////////////////////////
	// event handler
	DECL_ON_FUNC( ERM_OPEN_TRADE_REQ );

   _DECL_ON_FUNC( ERM_BREAK_TRADE_REQ, KEGS_BREAK_TRADE_REQ );

    DECL_ON_FUNC( ERM_JOIN_TRADE_REQ );
   _DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );

   //_DECL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ );

    DECL_ON_FUNC( ERM_UPDATE_TRADE_ITEM_REQ );
	DECL_ON_FUNC( ERM_UPDATE_TRADE_ED_REQ );
    DECL_ON_FUNC_NOPARAM( ERM_OFFER_TRADE_REQ );
	DECL_ON_FUNC_NOPARAM( ERM_APPROVE_TRADE_REQ );
	DECL_ON_FUNC( ERM_CONFIRM_TRADE_ACK );
	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	DECL_ON_FUNC( ERM_DB_UPDATE_TRADE_RESULT_NOT );
	DECL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_ACK );
#endif SERV_TRADE_DDOS_DEFENCE
	//}}

protected:
	KTradeUserManager	m_kTradeUserManager;
	boost::timer		m_kTimer[TM_TOTAL_NUM];
};

template< class T >
void KTrade::BroadCast( u_short usEventID, const T& data )
{
	KEvent kEvent;  // _BroadCast에서 clone할 것이므로 여기선 heap 아니어도 됨.
	kEvent.m_kDestPerformer.m_dwPerformerID = PI_GS_CHARACTER;
	kEvent.m_usEventID = usEventID;

	KSerializer ks;
	kEvent.m_kbuff.Clear();

	// serialize - only data
	ks.BeginWriting( &kEvent.m_kbuff );
	ks.Put( data );
	ks.EndWriting();

	_BroadCast( kEvent );
}
