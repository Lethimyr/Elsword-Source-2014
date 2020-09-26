#pragma once

#include "RefreshSingleton.h"
#include <ToString.h>
#include <boost/timer.hpp>
#include "KncSend.h"
#include <deque>
#include <set>


//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM


class KWaitQueueManager
{
	DeclareRefreshSingleton( KWaitQueueManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum WQ_ENUM
	{
		WQ_NONE = 0,
		WQ_CCU_LIMIT,
		WQ_ENTRY_BAN,
	};

public:
	KWaitQueueManager(void);
	~KWaitQueueManager(void);

	// for lua
	void SetMode_LUA( int iMode );
	void SetSendTimeGap_LUA( double fTimeGap );
	void SetTotalCCULimit_LUA( int iServerGroupID, int iCCU );

	// tick
	void Tick();

	// functions
	WQ_ENUM GetMode() const							{ return m_eMode; }
	int GetTotalCCU() const							{ return m_iTotalCCU; }
	void SetTotalCCU( IN const int iTotalCCU );
	int GetTotalCCULimit() const					{ return m_iTotalCCULimit; }

	bool IsCanEnterChannel() const					{ return ( ( m_iTotalCCU + GetCountPassedUser() ) < m_iTotalCCULimit ); }

	bool IsExist( IN const UidType iUID ) const		{ return ( m_setWaitUserList.find( iUID ) != m_setWaitUserList.end() ); }
	bool IsNextEnterUser( IN const UidType iUID );
	bool RegWaitUser( IN const UidType iUID );
	bool UnRegWaitUser( IN const UidType iUID );
	bool GetWaitUserCount( IN const UidType iUID, OUT int& iWaitUserCount ) const;

	void RegPassedUser( IN const UidType iUID )		{ m_setPassedUserList.insert( iUID ); }
	void UnRegPassedUser( IN const UidType iUID )	{ m_setPassedUserList.erase( iUID ); }
	int  GetCountPassedUser() const					{ return static_cast<int>(m_setPassedUserList.size()); }

protected:
	template < class T > void SendToLogDB( IN const unsigned short usEventID, IN const T& data );
	void SendToLogDB( IN const unsigned short usEventID );

private:
	WQ_ENUM						m_eMode;				// 대기열 모드
	boost::timer				m_kTimer;
	int							m_iTotalCCU;			// 현재 서버군 동접
	int							m_iTotalCCULimit;		// 서버군 동접 제한
	int							m_iPassCount;			// 입장 가능한 인원
	double						m_fSendTimeGap;
	bool						m_bSendGetCCU;

	std::deque< UidType >		m_deqWaitUserList;
	std::set< UidType >			m_setWaitUserList;

	std::set< UidType >			m_setPassedUserList;
};

DefRefreshSingletonInline( KWaitQueueManager );


template < class T >
void KWaitQueueManager::SendToLogDB( IN const unsigned short usEventID, IN const T& data )
{
	KncSend( PI_CHANNEL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CHANNEL_LOG_DB, 0, NULL, usEventID, data );
}


#endif SERV_WAIT_QUEUE_SYSTEM
//}}
