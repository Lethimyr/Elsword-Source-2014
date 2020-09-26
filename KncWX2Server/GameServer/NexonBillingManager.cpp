#include "NexonBillingManager.h"

#include "KncSend.h"
#include "NexonBilling.h"

ImplToStringW( KNexonBillingManager )
{
    return stm_;
}

ImplPfID( KNexonBillingManager, PI_GS_NX_BILLING );

ImplementSingleton( KNexonBillingManager );

KNexonBillingManager::KNexonBillingManager()
{
}

KNexonBillingManager::~KNexonBillingManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
//using namespace lua_tinker;
#include "BaseServer.h"

void KNexonBillingManager::RegToLua()
{
    //class_< KNexonBillingManager >( "KNexomBillingManager" )
    //    .def( "Init",					KNexonBillingManager::Init )
    //    .def( "dump",					KNexonBillingManager::Dump )
    //    .def( "AddWebServiceAddress", KNexonBillingManager::AddWebServiceAddress );
	lua_tinker::class_add<KNexonBillingManager>( g_pLua, "KNexonBillingManager" );
	lua_tinker::class_def<KNexonBillingManager>( g_pLua, "Init",				 &KNexonBillingManager::Init );
	lua_tinker::class_def<KNexonBillingManager>( g_pLua, "dump",				 &KNexonBillingManager::Dump );
	lua_tinker::class_def<KNexonBillingManager>( g_pLua, "AddWebServiceAddress", &KNexonBillingManager::AddWebServiceAddress );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KNexonBilling::##id );
#   include "NexonBillingOrder_def.h"

    //decl( "NXBillingManager", this );
	lua_tinker::decl( g_pLua, "NXBillingManager", this );
}

void KNexonBillingManager::AddWebServiceAddress( int iSoapAction, const char* szWSDL, const char* szMethod )
{
    KNexonBilling::AddWebServiceAddress( iSoapAction, szWSDL, szMethod );
}

KThread* KNexonBillingManager::CreateThread()
{
    return new KNexonBilling;
}