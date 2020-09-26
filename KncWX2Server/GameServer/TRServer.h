#pragma once 

#include <WinSock2.h>
#include "ToString.h"
#include "KNCSingleton.h"
#include "Thread/Thread.h"
#include "RTTI.h"
#include "Socket/Accepter.h"
#include "TRUserManager.h"
#include <queue>
#include "Event.h"
#include "ThreadManager.h"
//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	#include "TickDumper.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}


//////////////////////////////////////////////////////////////////////////
// For Dump
class KTRServerDumpManager
{
public:
	KTRServerDumpManager();	
	~KTRServerDumpManager();
	
	void	Reset()								{ m_uiTotalSendSize = 0; m_uiTotalSendCount = 0; }
	void	Tick();	
	void	DumpToLogFile();

	// Update Data	
	void	UpdateSendPacketInfo( u_int uiSendSize );
	void	IncreaseEventCount()				{ KLocker lock( m_csEventCount ); ++m_iEventCount; }

	// Get Data
	void	GetSendInfo( u_int& uiTotalSendSize, u_int& uiTotalSendCount );	
	void	GetEventCount( int& iEventCount )	{ KLocker kLock( m_csEventCount ); iEventCount = m_iEventCount; m_iEventCount = 0; }

private:
	// Send Info
	mutable KncCriticalSection					m_csSendInfo;
	u_int										m_uiTotalSendSize;
	u_int										m_uiTotalSendCount;

	// Event Count
	mutable KncCriticalSection					m_csEventCount;
	int											m_iEventCount;

	// 덤프 타이머
	boost::timer								m_kDumpTimer;
};
//////////////////////////////////////////////////////////////////////////



class KTRThreadManager : public KThreadManager
{
	DeclPfID;

public:
	KTRThreadManager() {}
	virtual ~KTRThreadManager() {}

protected:
	virtual KThread* CreateThread();
};


class KTRServer
{
	DeclareSingleton( KTRServer );
	DeclDump;
	DeclToStringW;
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	DeclareTickDumper;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

protected:
	KTRServer(void);
public:
	virtual ~KTRServer(void);	

	void	RegToLua();
	bool	Init();
	void	ShutDown();
	
	void	SetTRWorkerThreadCount( int iTRWorkerThreadCount ) { m_iTRWorkerThreadCount = iTRWorkerThreadCount; }

	void	QueueingRelayData( const KEventPtr& spEvent );

	// TCP Relay accepter thread 만 접근.
	static	void	OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote );

	KTRUserManager&	GetTRUserManager() { return m_kTRUserManager; }

	void	Tick();

protected:
	void	OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote );	

protected:

	struct KMaxTimeInfo 
	{
		mutable KncCriticalSection  m_cs;
		std::wstring                m_strIP;
		DWORD                       m_dwTerm;
		CTime                       m_cTime;
	} m_kMaxTimeInfo;

	KAccepter						m_kAccepter;
	KTRUserManager					m_kTRUserManager;
	KTRThreadManager				m_kThreadManager;

	int								m_iTRWorkerThreadCount;

	//////////////////////////////////////////////////////////////////////////
	// For Dump	
	//KTRServerDumpManager			m_kDumpManager;
	//////////////////////////////////////////////////////////////////////////
};

DefSingletonInline( KTRServer );


