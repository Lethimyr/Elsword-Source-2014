#include "GlobalNetLayer.h"
#include <Socket/KTDNUDP.h>

NiImplementRTTI( KGlobalNetLayer, KNetLayer );

KGlobalNetLayer::KGlobalNetLayer(void)
{    
}

KGlobalNetLayer::~KGlobalNetLayer(void)
{
}

ImplToStringW( KGlobalNetLayer )
{
	return START_TOSTRING_PARENTW( KNetLayer )		
        << L"   nothing to dump." << std::endl;
}

#include <KncLua.h>
#include <lua_tinker.h>
//using namespace lua_tinker;
#include "BaseServer.h"

void KGlobalNetLayer::RegToLua()
{
    KNetLayer::RegToLua();

    //class_<KLoginNetLayer>( "KLoginNetLayer" )
    //    .inh<KNetLayer>()
    //    .def( "dump", KLoginNetLayer::Dump );

    //decl( "NetLayer", this );
	lua_tinker::class_add<KGlobalNetLayer>( g_pLua, "KGlobalNetLayer" );
	lua_tinker::class_inh<KGlobalNetLayer, KNetLayer>( g_pLua );
	lua_tinker::class_def<KGlobalNetLayer>( g_pLua, "dump", &KGlobalNetLayer::Dump );

	lua_tinker::decl( g_pLua, "NetLayer", this );
}