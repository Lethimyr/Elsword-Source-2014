#pragma once

#include "GSProxy.h"

class KncClient
{
    DeclareSingleton( KncClient );

private:    // cannot access to constructor (singleton design)
    KncClient(void);
public:
    virtual ~KncClient(void);

    enum
    {
        GSPROXY_ID = 1,
    };

public:
    void Init( HWND hWnd );

    bool IsConnected()          { return (m_spGSProxy && m_spGSProxy->IsConnected() ); }

    bool Connect( IN const KNetAddress& kInfo );

public:
	//임시 테스트 변수
	UidType                 m_iUnitUID;

protected:
    HWND            m_hWnd;
    KGSProxyPtr     m_spGSProxy;
};

DefSingletonInline( KncClient )