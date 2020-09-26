#pragma once

#include "ActorProxy.h"

#include <Thread/Locker.h>
#include <KncSingleton.h>
#include <map>
#include <ToString.h>
#include "CommonPacket.h"
//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	#include "TickDumper.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}

class KProxyManager
{
	DeclareSingleton( KProxyManager );
    DeclToStringW;
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	DeclareTickDumper;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

    typedef KActorProxy* (*PFNCreateProxy)( int );

public:
	enum PROXY_TYPE
	{
		PT_CENTER,
		PT_LOGIN,

		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		PT_GLOBAL,
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		
		PT_NUM,
	};

	//{{ 2009. 11. 23  최육사	서버간접속구조개선
	struct KProxyInfo 
	{
		int m_iProxyType;
		int m_iProxyID;

		KProxyInfo( int iProxyType, int iProxyID )
		{
			m_iProxyType = iProxyType;
			m_iProxyID = iProxyID;
		}
	};
	//}}

    KProxyManager(void);
    virtual ~KProxyManager(void);

    void SetProxyBuilder( PFNCreateProxy fnBuilder );

	void AddRemoteAddress( IN int iProxyType, IN int iID, IN const WCHAR* wIP_, IN unsigned short usPort );
	int GetRemoteAddrNum( int iProxyType );

    void ConnectAll();

    void Tick();

    bool Shutdown( float fWaitTime );
    void DestroyProxy( int iProxyType, int iProxyID );

    void SetRoomServerLoad( int iProxyID, bool bIncrease );
    bool SendPacket( UidType nFrom, KEvent& kEvent );
	//int  BroadCast( int iProxyType, KEvent& kEvent ); -- 사용하지 않는 함수 : 나중에 정리

	//{{ 2009. 11. 23  최육사	서버간접속구조개선
	void GetDisconnectedProxyList( OUT std::vector< KProxyInfo >& vecDisconnProxy );
	//}}

	//{{ 2010. 06. 16  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
	void BroadCastToCnServer( KEvent& kEvent );
	void BroadCastToCnServerEachServerGroup( KEvent& kEvent );
#endif SERV_REALTIME_SCRIPT
	//}}

protected:
    inline bool ShutdownFinished();
    inline bool Connect( int iProxyType, int iID );     // 내부에서만 호출. 해당 아이디의 Proxy로 접속을 시도한다.
    KActorProxyPtr Get( int iProxyType, int iProxyID );
    int GetCnProxyID( UidType nUserUID );
    KActorProxyPtr GetCnProxy( UidType nUserUID );
    int GetRoomProxyID( UidType nRoomUID );
    KActorProxyPtr GetRoomProxy( UidType nRoomUID, u_short usEventID );
	int GetLoginProxyID();
	KActorProxyPtr GetLoginProxy();	

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	int GetGlobalProxyID();
	KActorProxyPtr GetGlobalProxy();	
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2009. 11. 23  최육사	서버간접속구조개선
	void KeepConnectionThread();
	//}}

	//{{ 2010. 01. 27  최육사	방생성개선
#ifdef SERV_ROOM_COUNT
	int GetRoomProxyID_NEW( UidType nRoomUID, u_short usEventID );
	int GetTypeByEventID( u_short usEventID );
#endif SERV_ROOM_COUNT
	//}}

	//{{ 2010. 02. 16  최육사	채널파티통합
#ifdef SERV_CHANNEL_PARTY
	int GetPartyProxyID( UidType nPartyUID );
	KActorProxyPtr GetPartyProxy( UidType nPartyUID );
#endif SERV_CHANNEL_PARTY
	//}}

	//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
public:
	void AddSameServerGroupProxyID( int iProxyID );
	void DelSameServerGroupProxyID( int iProxyID );
#endif SERV_INTEGRATION
	//}}

public:
protected:
	//{{ 2009. 11. 23  최육사	서버간접속구조개선	
	std::map< int, KActorProxyPtr >		m_mapProxy[PT_NUM];             // Proxy ID, ProxyPtr
	//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	std::vector< int >					m_vecSameServerGroupProxy;
#endif SERV_INTEGRATION
	//}}
	mutable KncCriticalSection			m_csProxy;
	//}}

	std::vector< KNetAddress >          m_vecAddress[PT_NUM];           // 연결이 손실된 경우 자동 복구를 위해 필요.
	std::vector< int >                  m_vecDestroyList[PT_NUM];

	DWORD								m_dwReconnectTick;  // 일정 주기로 접속상태를 검색해, 재접속을 시도한다.
	static DWORD						ms_dwReconnectGap;  // 접속상태 체크 주기.

	PFNCreateProxy						m_fnCreateProxy;

	//{{ 2009. 11. 23  최육사	서버간접속구조개선
	boost::shared_ptr< KTThread< KProxyManager > >	m_spThreadKeepConnect;
	//}}
};

DefSingletonInline( KProxyManager );


