#include "K3RComboauthmanager.h"

#ifdef SERV_COUNTRY_US

#include "KncSend.h"
#include "BaseServer.h"
#include "K3RComboAuth.h"


ImplToStringW( KK3RComboAuthManager )
{
	return stm_;
}

//ImplPfID( KGameForgeAuthManager, PI_CHANNEL_GF_AUTH );
ImplPfID( KK3RComboAuthManager, PI_NULL );

ImplementSingleton( KK3RComboAuthManager );

KK3RComboAuthManager::KK3RComboAuthManager()
{
	m_strWsdlPath = "";
}

KK3RComboAuthManager::~KK3RComboAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
//using namespace lua_tinker;
#include "BaseServer.h"

void KK3RComboAuthManager::RegToLua()
{
	lua_tinker::class_add<KK3RComboAuthManager>( g_pLua, "KK3RComboAuthManager" );
	lua_tinker::class_def<KK3RComboAuthManager>( g_pLua, "Init",				 &KK3RComboAuthManager::Init );
	lua_tinker::class_def<KK3RComboAuthManager>( g_pLua, "dump",				 &KK3RComboAuthManager::Dump );
	lua_tinker::class_def<KK3RComboAuthManager>( g_pLua, "AddWebServiceAddress", &KK3RComboAuthManager::AddWebServiceAddress );
	lua_tinker::class_def<KK3RComboAuthManager>( g_pLua, "AddWebMethod",	&KK3RComboAuthManager::AddWebMethod );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KK3RComboAuth::##id );
#   include "K3RComboAuth_def.h"


	lua_tinker::decl( g_pLua, "K3RComboAuthManager", this );
}

void KK3RComboAuthManager::Init( int nThreadNum )
{
	// Init 호출 시점은 항상 AddWebServiceAddress 지정후여야 함 //
	// 그 이유는 바로 아래 CreateThread에서 sdlFile과 smlFile이 입력되어야 하기 때문이다.
	KThreadManager::Init( nThreadNum );
}

//    KSimLayer::GetKObj()->Init(); 될때 BeginThread 호출됨


void KK3RComboAuthManager::AddWebServiceAddress( const char* szWSDL )
{
	m_strWsdlPath = szWSDL;
}

void KK3RComboAuthManager::AddWebMethod( int iSoapAction, const char* szMethod )
{
	KK3RComboAuth::AddWebMethod( iSoapAction, szMethod );
}


KThread* KK3RComboAuthManager::CreateThread()
{
	// Init 호출 시점은 항상 AddWebServiceAddress 지정후여야 함 //
	// 그 이유는 바로 아래 생성자에 sdlFile과 smlFile이 입력되어야 하기 때문이다.


	return new KK3RComboAuth(m_strWsdlPath.c_str());
}
void KK3RComboAuthManager::Tick()
{	
}

// void KGameForgeAuthManager::BeginThread()
// {
// 	KThreadManager::BeginThread();        
//}

#endif // SERV_COUNTRY_US