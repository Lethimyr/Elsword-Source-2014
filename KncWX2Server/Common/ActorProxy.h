#pragma once
#include "Socket/Session.h"
#include <windows.h>
#include <Queue>

#define KActorProxyPtr KActorProxy*
#undef KActorProxyPtr
SmartPointer( KActorProxy );

struct KServerInfo;
class KActorProxy : public KSession
{
    NiDeclareRTTI;
    DeclToStringW;

protected:
    KActorProxy();

public:
    virtual ~KActorProxy(void) {}

    void SetHwnd( HWND hWnd );
    void SetThrowWinMsg( bool bThrowWinMsg );
    bool GetThrowWinMsg()                   { return m_bThrowWinMsg; }

    // 060720. 인증 처리까지 기다린다. KUserProxy, KGSProxy가 이를 사용한다.
	int ConnectNAuth( IN const WCHAR* pIP, IN const int port, IN const KEvent& kEvent, IN DWORD dwTimeOut );

    void SetClassID( int iClassID )         { m_iClassID = iClassID; }  // 윈도우 메세지를 구성할때 사용할 id 할당.
    int GetClassID()                        { return m_iClassID; }

    void SetLoad( int iLoad )               { m_iLoad = iLoad; }
    int GetLoad()                           { return m_iLoad; }

	//{{ 2010. 01. 27  최육사	방생성개선
	void SetLoad_NEW( int iType, bool bIsOpen );
	u_int GetLoad_NEW( int iType );
	//}}

	int GetProxyID()						{ return m_iProxyID; }
	void SetProxyID( int iProxyID )			{ m_iProxyID = iProxyID; }

	//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
	virtual int	 GetActorServerGroup()		{ return -1; }
	virtual bool IsSameServerGroup()		{ return false; }
	//}}

protected:
    //void PostEvent( const KEventPtr& spEvent );    // 이벤트를 윈도우 메시지로 하거나 내부 큐에 큐잉한다.	

    HWND                        m_hWnd;
    bool                        m_bThrowWinMsg;     ///< false이면 받은 메시지를 큐에 넣어두기만 한다.

    boost::shared_ptr<void>     m_spEventAck;
    int                         m_nAckOK;
    int                         m_iClassID;         // 이벤트를 윈도우 메세지로 던질 때, 구분자 역할을 한다.

    int                         m_iLoad;
	int							m_iProxyID;

	//{{ 2010. 01. 27  최육사	방생성개선
	KncCriticalSection          m_csLoad;
	std::map< int, u_int >		m_mapLoad;
	//}}	

public:

    KncCriticalSection          m_csRecvMsg;
    std::queue<KEventPtr>       m_queRecvMsg;       ///< m_bThrowWinmsg = false일때 사용.
};