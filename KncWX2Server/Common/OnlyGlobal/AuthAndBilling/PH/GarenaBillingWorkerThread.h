#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"


#include "GarenaBillingServer.h"
#include "GarenaBillingSubThread.h"
#include "KncSend.h"



#ifdef SERV_COUNTRY_PH


class KGarenaBillingWorkerThread : public KGarenaBillingSubThread
{
public:
	KGarenaBillingWorkerThread();
	virtual ~KGarenaBillingWorkerThread();

protected:
	
	template < class T > void SendToGarenaBilling( UidType iSessionUID, unsigned short usEventID, const T& data );

	template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToKOGBillingDB( unsigned short usEventID, const T& data );
	template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );

	virtual void ProcessEvent( const KEventPtr& spEvent );

	// process tcp packet

	DECL_ON_FUNC( EJSON_GN_CHECK_ACCOUNT_REQ );
	DECL_ON_FUNC( EJSON_GN_CHECK_ACCOUNT_ACK );
	DECL_ON_FUNC( EJSON_GN_CHANGE_GAME_CURRENCY_REQ );
	DECL_ON_FUNC( EJSON_GN_CHANGE_GAME_CURRENCY_ACK );
	DECL_ON_FUNC( EJSON_GN_TRANSACTION_REQ );
	DECL_ON_FUNC( EJSON_GN_TRANSACTION_ACK );


	DECL_ON_FUNC( EBILL_LOGIN_CHECK_BALANCE_ACK );
	DECL_ON_FUNC( EBILL_GN_CHANGE_GAME_CURRENCY_ACK );
	DECL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK );
	DECL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK );


	std::map<std::string, KEJSON_GN_CHANGE_GAME_CURRENCY_ACK>		m_mapChangeGameCurrencyFailed;
};



template < class T >
void KGarenaBillingWorkerThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_LOGIN_GARENA_BILLING, 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}

template < class T >
void KGarenaBillingWorkerThread::SendToKOGBillingDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_GARENA_BILLING, 0, PI_LOGIN_KOG_BILLING_DB, 0, NULL, usEventID, data );
}

template < class T >
void KGarenaBillingWorkerThread::SendToAccountDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_GARENA_BILLING, 0, PI_LOGIN_ACCOUNT_DB, 0, NULL, usEventID, data );
}


template < class T >
void KGarenaBillingWorkerThread::SendToGarenaBilling( UidType iSessionUID, u_short usEventID, const T& data )
{
	boost::shared_ptr< KGarenaBillingPacket > spPacket( new KGarenaBillingPacket );
	spPacket->Write( data );

	KncSecurity::KByteStream kStreamBuf;
	kStreamBuf.reserve( spPacket->GetStringBuffer().size() + sizeof(unsigned long) );
	kStreamBuf.Assign( spPacket->GetStringBuffer().size(), sizeof(unsigned long) );
	kStreamBuf.append( (byte*)spPacket->GetStringBuffer().c_str(), spPacket->GetStringBuffer().size() );   // 실제 전송할 데이터 넣기.
	//	KncSecurity::KByteStream kStreamBuf;
	//	KGarenaBillingUser::MakeByteStream( kStreamBuf, usEventID, data );

	KGarenaBillingUserPtr spGarenaBillingUser = SiKGarenaBillingServer()->GetGarenaBillingUserManager().GetGarenaBillingUser( iSessionUID );
	if( IS_NULL( spGarenaBillingUser ) )
	{
		START_LOG( cerr, L"세션이 존재하지 않습니다!" )
			<< BUILD_LOG( iSessionUID )
			<< END_LOG;
		return;
	}

	spGarenaBillingUser->SendPacket( kStreamBuf );
}


#endif SERV_COUNTRY_PH

