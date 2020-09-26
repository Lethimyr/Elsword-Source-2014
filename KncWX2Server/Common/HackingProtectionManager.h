#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "KncSend.h"


//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST


class KHackingProtectionManager
{
	DeclareSingleton( KHackingProtectionManager );
	DeclDump;
	DeclToStringW;

public:
	KHackingProtectionManager();
	~KHackingProtectionManager();

	void Tick();

	void Init();
	
	void UpdateHackingToolList( IN const KDBE_CHECK_HACKING_TOOL_LIST_ACK& kInfo );
	void GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList );

	// send event
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	int											m_iReleaseTick;
	std::vector< KHackingToolInfo >				m_vecHackingToolList;	
	boost::timer								m_tRefreshTimer;
};

DefSingletonInline( KHackingProtectionManager );

template < class T >
void KHackingProtectionManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}


#endif SERV_HACKING_TOOL_LIST
//}}