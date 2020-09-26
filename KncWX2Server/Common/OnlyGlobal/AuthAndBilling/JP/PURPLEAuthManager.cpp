#include <WinSock2.h>
#include "PurpleAuthManager.h"

#ifdef SERV_COUNTRY_JP

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "PurpleAuth.h"


/*
ImplToStringW( KPurpleAuthManager )
{
	return stm_;
}
*/

ImplToStringW( KPurpleAuthManager )
{
	return stm_;
}

ImplPfID( KPurpleAuthManager, PI_NULL );
ImplementSingleton( KPurpleAuthManager );


KPurpleAuthManager::KPurpleAuthManager()
{
}

KPurpleAuthManager::~KPurpleAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KPurpleAuthManager::RegToLua()
{
	//class_< KPurpleAuthManager >( "KNexomBillingAuthManager" )
	//	.def( "dump",						KPurpleAuthManager::Dump )
	//	.def( "InitPurpleAuthInfo",	KPurpleAuthManager::InitPurpleAuthInfo )
	//	.def( "SetNumThread",				KPurpleAuthManager::Init );

	//decl( "NXBillingAuthManager", this );
	lua_tinker::class_add<KPurpleAuthManager>( g_pLua, "KPurpleAuthManager" );
	//lua_tinker::class_def<KPurpleAuthManager>( g_pLua, "dump",					 &KPurpleAuthManager::Dump );	
	lua_tinker::class_def<KPurpleAuthManager>( g_pLua, "SetNumThread",			 &KPurpleAuthManager::Init );

	lua_tinker::decl( g_pLua, "PurpleAuthManager", this );
}

void KPurpleAuthManager::Tick()
{	
}

void KPurpleAuthManager::Init( int nThreadNum )
{
	KThreadManager::Init( nThreadNum );	
}

KThread* KPurpleAuthManager::CreateThread()
{
	return new KPURPLEAuth;
}

void KPurpleAuthManager::BeginThread()
{	
	KThreadManager::BeginThread();

}

void KPurpleAuthManager::EndThread()
{
	KThreadManager::EndThread();
}

#endif // SERV_COUNTRY_JP