#pragma once
#include "ServerDefine.h"
#include "RefreshSingleton.h"
//#include "ServerDefine.h"
#include "ServerPacket.h"
#include "DBLayer.h"
#include <KncSend.h>


//{{ 2012. 10. 15	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM


class KCorruptionMonitor
{
	DeclareRefreshSingleton( KCorruptionMonitor );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:
	KCorruptionMonitor(void);
	~KCorruptionMonitor(void);

	// for lua
	void SetMornitoringOn_LUA( bool bVal );

	// function
	void Init();
	void Update( IN const KDBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK& kInfo );

protected:
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	bool						m_bMornitoringOn;
	std::map< int, __int64 >	m_mapEDMornitoring;
	std::map< int, int >		m_mapItemMornitoring;
};

DefRefreshSingletonInline( KCorruptionMonitor );

template < class T >
void KCorruptionMonitor::SendToLogDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_LOG_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}


#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}