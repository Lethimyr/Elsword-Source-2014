#pragma once

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <boost/timer.hpp>
#include <map>
#include "CommonPacket.h"
#include "ClientPacket.h"


//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT


class KDungeonEventManager
{
	DeclareRefreshSingleton( KDungeonEventManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	KDungeonEventManager(void);
	~KDungeonEventManager(void);

	//struct KTimeDropEventMonsterInfo
	//{
	//	int						m_iEventUID;
	//	int						m_iNpcID;
	//	CTime					m_tDropDate;

	//	KTimeDropEventMonsterInfo()
	//	{
	//		m_iEventUID	= 0;
	//		m_iNpcID	= 0;
	//	}
	//};

public:
	// function
	void Init();
	void UpdateTimeDropEventMonsterInfo( IN const std::vector< KDungeonDropEventNpcInfo >& vecEventInfo );
	bool CheckTimeDropEventMonster( IN int iDungeonID, IN int iStageID, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT int& iEventUID, OUT int& iNpcID );
	

protected:
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	std::map< __int64, KDungeonDropEventNpcInfo >	m_mapTimeDropEventMonster;
};

DefRefreshSingletonInline( KDungeonEventManager );

template < class T >
void KDungeonEventManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_LOG_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}


#endif SERV_TIME_DROP_MONSTER_EVENT
//}}


