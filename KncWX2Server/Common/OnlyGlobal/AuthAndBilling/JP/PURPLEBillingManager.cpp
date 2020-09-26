#include <WinSock2.h>
#include "PURPLEBillingManager.h"

#ifdef SERV_COUNTRY_JP


#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "PURPLEBilling.h"

ImplToStringW( KPurpleBillingManager )
{
	return stm_;
}

ImplPfID( KPurpleBillingManager, PI_GS_PUBLISHER_BILLING );
ImplementSingleton( KPurpleBillingManager );


KPurpleBillingManager::KPurpleBillingManager()
{
	
}

KPurpleBillingManager::~KPurpleBillingManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KPurpleBillingManager::RegToLua()
{
	//class_< KPurpleAuthManager >( "KNexomBillingAuthManager" )
	//	.def( "dump",						KPurpleAuthManager::Dump )
	//	.def( "InitPurpleAuthInfo",	KPurpleAuthManager::InitPurpleAuthInfo )
	//	.def( "SetNumThread",				KPurpleAuthManager::Init );

	//decl( "NXBillingAuthManager", this );
	lua_tinker::class_add<KPurpleBillingManager>( g_pLua, "KPurpleBillingManager" );
	//lua_tinker::class_def<KPurpleAuthManager>( g_pLua, "dump",					 &KPurpleAuthManager::Dump );	
	lua_tinker::class_def<KPurpleBillingManager>( g_pLua, "SetNumThread",			 &KPurpleBillingManager::Init );

	lua_tinker::decl( g_pLua, "PurpleBillingManager", this );
}

void KPurpleBillingManager::Tick()
{	
}

void KPurpleBillingManager::Init( int nThreadNum )
{
	KThreadManager::Init( nThreadNum );	
}

KThread* KPurpleBillingManager::CreateThread()
{
	return new KPURPLEBilling;
}

void KPurpleBillingManager::BeginThread()
{	
	KThreadManager::BeginThread();

}

void KPurpleBillingManager::EndThread()
{
	KThreadManager::EndThread();
}

#endif //SERV_COUNTRY_JP