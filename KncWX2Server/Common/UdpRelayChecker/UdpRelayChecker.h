#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <boost/timer.hpp>
#include "UdpCheckerSession.h"
#include "CommonPacket.h"
#include "ServerPacket.h"


//{{ 2012. 04. 23	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER


class KQueryPerformanceCounter
{
public:
	KQueryPerformanceCounter() : 
	m_bUsingQPF( false ),
	m_llQPFTicksPerSec( 0 )
	{
		Init();
	}
	~KQueryPerformanceCounter()
	{
	}

	void Init()
	{
		if( !m_bUsingQPF )
		{
			LARGE_INTEGER qwTicksPerSec;
			m_bUsingQPF = (bool) (::QueryPerformanceFrequency( &qwTicksPerSec ) != 0 );
			if( !m_bUsingQPF )
			{
				START_LOG( cerr, L"이거 사용 못하나?" )
					<< END_LOG;
				return;
			}

            m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
		}
	}

	bool GetQPCounter( LARGE_INTEGER& qwTime )
	{
		if( !m_bUsingQPF )
			return false;

		::QueryPerformanceCounter( &qwTime );
		return true;
	}

	float GetElapsedTime( const LARGE_INTEGER& qwLastQPCounter )
	{
		LARGE_INTEGER qwTime;
		if( GetQPCounter( qwTime ) == false )
			return -1;

		return (float)((double)( qwTime.QuadPart - qwLastQPCounter.QuadPart ) / (double) m_llQPFTicksPerSec );
	}

private:
	LONGLONG	m_llQPFTicksPerSec;
	bool		m_bUsingQPF;
};


class KUdpRelayChecker : public KThread, public KPerformer
{
	DeclInstanceKeeping( KUdpRelayChecker );
	DeclDump;
	DeclToStringW;
	DeclPfID;
public:
	struct SUdpRelayServerInfo
	{
		UidType						m_iServerUID;
		std::wstring				m_wstrServerIP;
		short						m_sServerPort;
		bool						m_bRelayConnected;
		bool						m_bSendRelayPacket;
		boost::timer				m_tRelayCheckTimer;
		DWORD						m_dwLastTickCount;
		DWORD						m_dwLastTimeGetTime;
        LARGE_INTEGER				m_qwLastQPCounter;

		// 통계
		boost::timer				m_tDBUpdateTimer;
		DWORD						m_dwTotalTimeGetTime;
		DWORD						m_dwCountTimeGetTime;
		DWORD						m_dwMaxTimeGetTime;

		SUdpRelayServerInfo()
		{
			m_iServerUID = 0;
			m_bRelayConnected = false;
			m_bSendRelayPacket = false;
			m_sServerPort = 0;
			m_dwLastTickCount = 0;
			m_dwLastTimeGetTime = 0;

			// 통계
			ClearStatistics();
		}

		void ClearStatistics()
		{
			m_dwTotalTimeGetTime = 0;
			m_dwCountTimeGetTime = 0;
			m_dwMaxTimeGetTime = 0;
		}
	};	

public:
	KUdpRelayChecker(void);
	~KUdpRelayChecker(void);

	void Init();

	// for lua
	void OpenScriptFile();
	void RegToLua();
	
	void SetUdpRelayCheckerOn_LUA( bool bOn );
	void AddUdpRelayCheckerOnServerClass_LUA( int iServerClass );
	void SetUdpRelayCheckerPrintLog_LUA( bool bOn );
	void SetUdpRelayCheckerPort_LUA( short sPort );
	void SetUdpRelayCheckerTimeGap_LUA( double fGap );
	void SetDBUpdateTimeGap_LUA( double fGap );

	bool IsPrintLog() const		{ return m_bPrintLog; }

	// override from KPerformer	
	virtual void Tick();
	virtual void ProcessEvent( const KEventPtr& spEvent );
	virtual void ShutDown();

protected:  // util function
	virtual void Run(); // derived from KThread

	void	CheckUdpServer();

	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

	template < typename T >	void Serialize( KSerBuffer* pBuff, T* pData );
	template < typename T >	void DeSerialize( KSerBuffer* pBuff, T* pData );

	DECL_ON_FUNC( DBE_GET_CENTER_SERVER_LIST_ACK );
	DECL_ON_FUNC( E_CONNECT_RELAY_ACK );
	DECL_ON_FUNC( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT );

private:
	std::set< int >									m_setInitServerClass;
	bool											m_bCheckerOn;
	bool											m_bPrintLog;
	short											m_sUdpRelayCheckerPort;
	double											m_fRelayCheckPacketGap;
	double											m_fDBUpdateGap;
	boost::timer									m_tDBUpdateTimer;
	KQueryPerformanceCounter						m_kQPCounter;
	boost::shared_ptr<KUdpCheckerSession>			m_spUdpCheckerSession;
	std::map< std::wstring, SUdpRelayServerInfo >	m_mapUdpRelayServerInfo;
};

template < class T >
void KUdpRelayChecker::SendToLogDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_LOG_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

template < typename T >
void KUdpRelayChecker::Serialize( KSerBuffer* pBuff, T* pData )
{
	KSerializer ks;
	ks.BeginWriting( pBuff );
	ks.Put( *pData );
	ks.EndWriting();
}

template < typename T >
void KUdpRelayChecker::DeSerialize( KSerBuffer* pBuff, T* pData )
{
	KSerializer ks;
	ks.BeginReading( pBuff );
	ks.Get( *pData );
	ks.EndReading();
}


#endif SERV_UDP_RELAY_CHECKER
//}}


