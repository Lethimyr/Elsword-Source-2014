#include "KncClient.h"

ImplementSingleton( KncClient )

KncClient::KncClient(void) : m_hWnd( HWND_DESKTOP )
{
    NetCommon::InitWinSock();
    KncSecurity::InitSecurity();
}

KncClient::~KncClient(void)
{
    KncSecurity::ReleaseSecurity();
}

void KncClient::Init( HWND hWnd_ )
{
    m_hWnd = hWnd_;
}

bool KncClient::Connect( IN const KNetAddress& kInfo_ )
{
    if( IsConnected() ) 
    {
        START_LOG( cerr, "이미 접속된 상태에서 재호출됨." )
            << BUILD_LOG( m_spGSProxy->GetIPStr() )
            << BUILD_LOG( m_spGSProxy->GetPort() )
            << BUILD_LOG( kInfo_.m_wstrIP )
            << BUILD_LOG( kInfo_.m_usPort );

        return false;
    }

    KGSProxyPtr spProxy( new KGSProxy );

    spProxy->SetClassID( GSPROXY_ID );
    spProxy->Init( false );
    spProxy->SetHwnd( m_hWnd );

    std::string strIP = KncUtil::toNarrowString( kInfo_.m_strIP );
    if( !spProxy->Connect( strIP.c_str(), kInfo_.m_usPort ) )
    {
        return false;
    }
    
    m_spGSProxy = spProxy;
    return true;
}