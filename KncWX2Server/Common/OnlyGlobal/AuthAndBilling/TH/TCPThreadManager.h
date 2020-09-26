#pragma once

#include "ThreadManager.h"
//{{ 2009. 6. 15  최육사	sec string
#include "SecurityString.h"
//}}

//{{ 최육사 : [2012/10/9] //	태국 OTP 인증 통신 모듈
#ifdef SERV_AUTH_TCP_THREAD_MANAGER


#define KTCPThreadManagerPtr KTCPThreadManager*
#undef KTCPThreadManagerPtr
SmartPointer( KTCPThreadManager );

class KTCPThreadManager : public KThreadManager
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KTCPThreadManager() {}
    virtual ~KTCPThreadManager() {}

	void SetTCPConnectionInfo( const std::wstring& wstrIP, const short sPort );

protected:
    // derived from KThreadManager;
    virtual KThread* CreateThread();

protected:
	std::wstring	m_wstrIP;
	short			m_sPort;
};

DeclOstmOperatorW( KTCPThreadManager );


#endif SERV_AUTH_TCP_THREAD_MANAGER
//}}