#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <boost/timer.hpp>
#include "KncSend.h"


//////////////////////////////////////////////////////////////////////////
// DLL Manager : 계정 블럭 관리
#ifdef SERV_DLL_MANAGER


class KDLLManager
{
	DeclareSingleton( KDLLManager );
	DeclDump;
	DeclToStringW;

public:
	KDLLManager(void);
	~KDLLManager(void);

	void Tick();
	void OnServerReadyComplete();

	void Shutdown();
	//{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
	void Init( IN const KDBE_GET_MODULE_INFO_LIST_ACK& kPacket );
	bool CheckHackingModuleList( IN std::vector< std::wstring >& vecModuleList, OUT KELG_CHECK_MODULE_INFO_UPDATE_NOT& kPacket );
	bool UpdateModuleInfo( IN const std::wstring& wstrModuleName, OUT KModuleInfo& kModule );
	void InitHackingModuleList( IN const std::vector< KModuleInfo >& vecModuleList, IN int& iReleaseTick );
#else
	void Init( IN const std::vector< KModuleInfo >& vecModuleList );
#endif SERV_HACKING_DLL_CHECK
	//}}

	void UpdateModuleInfo( IN const std::wstring& wstrModuleName );
	void UpdateModuleList( IN const std::vector< std::wstring >& vecModuleList );
	void UpdateModuleList( IN const std::vector< KModuleInfo >& vecModuleList );
	bool AddModuleInfo( IN const KModuleInfo& kInfo );
	bool UpdateModuleUID( IN const std::wstring& wstrModuleName, IN int iModuleUID );
	void GetDBUPdateInfo( OUT std::vector< KModuleInfo >& vecUpdateInfo );

private:
	//{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
	template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );
	void SendToAccountDB( unsigned short usEventID );
#endif SERV_HACKING_DLL_CHECK
	//}}

	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	std::map< std::wstring, KModuleInfo >	m_mapModuleList;
	boost::timer							m_kTimer;

	//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
	int										m_iReleaseTick;
	boost::timer							m_kReleaseTickTimer;
	std::map< std::wstring, KModuleInfo >	m_mapHackingModuleList;
#endif SERV_HACKING_DLL_CHECK
	//}}

};

DefSingletonInline( KDLLManager );

template < class T >
void KDLLManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_LOG_DB, 0, NULL, usEventID, data );
}
//{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
template < class T >
void KDLLManager::SendToAccountDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_ACCOUNT_DB, 0, NULL, usEventID, data );
}
#endif SERV_HACKING_DLL_CHECK
//}}

#endif SERV_DLL_MANAGER