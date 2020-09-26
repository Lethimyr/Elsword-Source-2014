#pragma once

#include "RefreshSingleton.h"
#include <boost/timer.hpp>
#include <atltime.h>

#include "Event.h"
#include "ServerPacket.h"
#include "KncSend.h"

//{{ 2013. 02. 15	최육사	로그 시스템 개선
//#ifdef SERV_LOG_SYSTEM_NEW


class KLogManager
{
    DeclareRefreshSingleton( KLogManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
    KLogManager();
    virtual ~KLogManager();

	// for lua
	void SetNewLogByFileSize_LUA( bool bValue );
	void SetNewLogDaily_LUA( bool bValue );
	void SetErrorLogStat_LUA( bool bValue );

	void SetCheckTimeGap_LUA( double fTimeGap );
	void SetErrorLogStatTimeGap_LUA( double fTimeGap );
	void SetLogFileMaxSize_LUA( int iMByte, int iKByte );

	void Tick();

protected:
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );

protected:
	double			m_fCheckTimeGap;
	double			m_fErrorLogStatTimeGap;
	int				m_iLogFileMaxSize;
	boost::timer	m_tCheckTimer;
	boost::timer	m_tErrorLogStatTimer;
	CTime			m_tLastCheckTime;

	// 실행 여부
	bool			m_bNewLogByFileSize;
	bool			m_bNewLogDaily;
	bool			m_bErrorLogStat;
};

DefRefreshSingletonInline( KLogManager );

template < class T >
void KLogManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PC_SERVER, 0, PC_LOG_DB, 0, NULL, usEventID, data );
}


//#endif SERV_LOG_SYSTEM_NEW
//}}