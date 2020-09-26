//{{ 2012. 03. 29	±è¹Î¼º		Áßº¹ sn Á¢¼Ó °¨½Ã
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER

#pragma once

#include "RefreshSingleton.h"
#include <atltime.h>
#include <boost/timer.hpp>
#include "CommonPacket.h"
#include "ServerPacket.h"



class KServerSNManager
{
	DeclareRefreshSingleton( KServerSNManager );
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	KServerSNManager(void);
	~KServerSNManager(void);

	void RegServerSN( IN __int64& iServerSN, IN UidType& iUserUID, IN UidType& iGSUID, OUT int& iResult, OUT std::map<UidType, UidType>& mapOverlapUser );
	void UnRegServerSN( IN __int64& iServerSN, IN UidType& iUserUID, OUT int& iResult );

private:
	std::map< __int64, std::map<UidType, UidType> >				m_mapServerSNManager;
};

DefRefreshSingletonInline( KServerSNManager );


#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}

