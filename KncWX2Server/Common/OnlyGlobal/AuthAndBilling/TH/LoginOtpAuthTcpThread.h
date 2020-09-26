#pragma once
#include "SubThread.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_TH

class KLoginOtpAuthTcpThread : public KSubThread
{
    DeclPfID;
public:
	KLoginOtpAuthTcpThread( const std::wstring& wstrIP, const short sPort );
    virtual ~KLoginOtpAuthTcpThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
	bool	AKeyConnect( IN SOCKET sock );
	bool	AKeySend( IN SOCKET sock, IN std::string sendPacket );
	bool	AKeyRecv( IN SOCKET sock, OUT std::string &sendRecvPacket );

    // packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToChannelUser( UidType nTo, unsigned short usEventID, const T& data );
    void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID );

	DECL_ON_FUNC( EAS_AUTH_AKEY_REQ );
};

template < class T >
void KLoginOtpAuthTcpThread::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KLoginOtpAuthTcpThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nGS, -1 };
    KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}

template < class T >
void KLoginOtpAuthTcpThread::SendToChannelUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_CHANNEL_USER, nTo, NULL, usEventID, data );
}

#endif SERV_COUNTRY_TH
