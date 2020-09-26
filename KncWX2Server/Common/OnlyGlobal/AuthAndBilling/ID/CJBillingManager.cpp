#include <WinSock2.h>
#include "CJBillingManager.h"

#ifdef SERV_COUNTRY_ID

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "CJBilling.h"
#include "Simlayer.h"

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

ImplToStringW( KCJBillingManager )
{
	return stm_;
}

ImplPfID( KCJBillingManager, PI_GS_PUBLISHER_BILLING );
ImplementSingleton( KCJBillingManager );


KCJBillingManager::KCJBillingManager()
{
	m_kCJBillingInfo.m_strIP.clear();
	m_kCJBillingInfo.m_usPort = 0;
}

KCJBillingManager::~KCJBillingManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KCJBillingManager::RegToLua()
{
	lua_tinker::class_add<KCJBillingManager>( g_pLua, "KCJBillingManager" );
	//lua_tinker::class_def<KCJBillingManager>( g_pLua, "dump",					 &KCJBillingManager::Dump );	
	lua_tinker::class_def<KCJBillingManager>( g_pLua, "InitCJBillingInfo",		&KCJBillingManager::InitCJBillingInfo );
	lua_tinker::class_def<KCJBillingManager>( g_pLua, "SetNumThread",			&KCJBillingManager::Init );

	lua_tinker::decl( g_pLua, "CJBillingManager", this );
}

void KCJBillingManager::InitCJBillingInfo( const char* szNexonAuthIP, unsigned short usPort )
{
	m_kCJBillingInfo.m_strIP		= szNexonAuthIP;
	m_kCJBillingInfo.m_usPort		= usPort;
}

void KCJBillingManager::Tick()
{
}

void KCJBillingManager::Init( int nThreadNum )
{
	KThreadManager::Init( nThreadNum );	
	BeginThread();
}

KThread* KCJBillingManager::CreateThread()
{
	return new KCJBilling;
}

void KCJBillingManager::BeginThread()
{	
	KThreadManager::BeginThread();
}

void KCJBillingManager::EndThread()
{
	KThreadManager::EndThread();
}

#endif //SERV_COUNTRY_ID
