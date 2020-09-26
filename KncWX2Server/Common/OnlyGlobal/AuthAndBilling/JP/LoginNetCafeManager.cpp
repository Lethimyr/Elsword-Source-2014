#include <WinSock2.h>
#include "LoginNetCafeManager.h"

#ifdef SERV_COUNTRY_JP


#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "LoginNetCafe.h"




ImplToStringW( KLoginNetCafeManager )
{
	return stm_;
}

ImplPfID( KLoginNetCafeManager, PI_NULL );
ImplementSingleton( KLoginNetCafeManager );


KLoginNetCafeManager::KLoginNetCafeManager()
{
	
}

KLoginNetCafeManager::~KLoginNetCafeManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KLoginNetCafeManager::RegToLua()
{
	//class_< KPurpleAuthManager >( "KNexomBillingAuthManager" )
	//	.def( "dump",						KPurpleAuthManager::Dump )
	//	.def( "InitPurpleAuthInfo",	KPurpleAuthManager::InitPurpleAuthInfo )
	//	.def( "SetNumThread",				KPurpleAuthManager::Init );

	//decl( "NXBillingAuthManager", this );
	lua_tinker::class_add<KLoginNetCafeManager>( g_pLua, "KLoginNetCafeManager" );
	//lua_tinker::class_def<KPurpleAuthManager>( g_pLua, "dump",					 &KPurpleAuthManager::Dump );	
	lua_tinker::class_def<KLoginNetCafeManager>( g_pLua, "SetNumThread",			 &KLoginNetCafeManager::Init );

	lua_tinker::decl( g_pLua, "LoginNetCafeManager", this );
}

void KLoginNetCafeManager::Tick()
{	
}

void KLoginNetCafeManager::Init( int nThreadNum )
{
	KThreadManager::Init( nThreadNum );	
}

KThread* KLoginNetCafeManager::CreateThread()
{
	return new KLoginNetCafe;
}

void KLoginNetCafeManager::BeginThread()
{	
	KThreadManager::BeginThread();

}

void KLoginNetCafeManager::EndThread()
{
	KThreadManager::EndThread();
}

#endif //#ifdef SERV_COUNTRY_JP

