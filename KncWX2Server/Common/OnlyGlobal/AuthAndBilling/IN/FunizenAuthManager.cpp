#include <WinSock2.h>
#include "FunizenAuthManager.h"

#ifdef SERV_COUNTRY_IN
#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "FunizenAuth.h"

ImplToStringW( KFunizenAuthManager )
{
	return stm_;
}

ImplPfID( KFunizenAuthManager, PI_NULL );
ImplementSingleton( KFunizenAuthManager );

KFunizenAuthManager::KFunizenAuthManager()
{
}

KFunizenAuthManager::~KFunizenAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KFunizenAuthManager::RegToLua()
{
	lua_tinker::class_add<KFunizenAuthManager>( g_pLua, "KFunizenAuthManager" );
	lua_tinker::class_def<KFunizenAuthManager>( g_pLua, "SetNumThread",				&KFunizenAuthManager::Init );

	lua_tinker::decl( g_pLua, "FunizenAuthManager", this );
}

void KFunizenAuthManager::Tick()
{	
}

void KFunizenAuthManager::Init( int nThreadNum )
{
	KThreadManager::Init( nThreadNum );	
}

KThread* KFunizenAuthManager::CreateThread()
{
	return new KFunizenAuth;
}

void KFunizenAuthManager::BeginThread()
{	
	KThreadManager::BeginThread();
}

void KFunizenAuthManager::EndThread()
{
	KThreadManager::EndThread();
}
#endif SERV_COUNTRY_IN