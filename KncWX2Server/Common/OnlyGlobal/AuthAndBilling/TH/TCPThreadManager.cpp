#include "TCPThreadManager.h"
#include "KncSend.h" 


//{{ 최육사 : [2012/10/9] //	태국 OTP 인증 통신 모듈
#ifdef SERV_AUTH_TCP_THREAD_MANAGER


ImplPfID( KTCPThreadManager, PI_NULL );    // 각각의 객체마다 다른 값을 가져야 한다. 그냥 사용 x.

NiImplementRTTI( KTCPThreadManager, KThreadManager );
ImplToStringW( KTCPThreadManager )
{
	stm_ << ", Thread : " << GetThreadNum() << ", Event : " << GetQueueSize() << std::endl;
	//{{ 2009. 9. 16  최육사	덤프

	KMaxQueueSize kInfo = GetMaxQueueSize();
	std::wstring wstrRegDate = ( CStringW )( kInfo.m_tRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );
	stm_ << L"  MaxQueueSize : " << kInfo.m_QueueSize << ", RegDate : " << wstrRegDate << std::endl;
	//}}

    return stm_;
}

extern KThread* CreateTCPThread( const std::wstring& wstrIP, const short sPort );

KThread* KTCPThreadManager::CreateThread()
{
	return CreateTCPThread( m_wstrIP, m_sPort );
}

void KTCPThreadManager::SetTCPConnectionInfo( const std::wstring& wstrIP, const short sPort )
{
	m_wstrIP = wstrIP;
	m_sPort = sPort;
}

ImplOstmOperatorW( KTCPThreadManager );


#endif SERV_AUTH_TCP_THREAD_MANAGER
//}}