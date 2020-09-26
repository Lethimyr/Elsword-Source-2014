#include "LoginNetLayer.h"
#include <Socket/KTDNUDP.h>

NiImplementRTTI( KLoginNetLayer, KNetLayer );

KLoginNetLayer::KLoginNetLayer(void)
{    
}

KLoginNetLayer::~KLoginNetLayer(void)
{
}

ImplToStringW( KLoginNetLayer )
{
	return START_TOSTRING_PARENTW( KNetLayer )		
        << L"   nothing to dump." << std::endl;
}

#include <KncLua.h>
#include <lua_tinker.h>
//using namespace lua_tinker;
#include "BaseServer.h"

void KLoginNetLayer::RegToLua()
{
    KNetLayer::RegToLua();

    //class_<KLoginNetLayer>( "KLoginNetLayer" )
    //    .inh<KNetLayer>()
    //    .def( "dump", KLoginNetLayer::Dump );

    //decl( "NetLayer", this );
	lua_tinker::class_add<KLoginNetLayer>( g_pLua, "KLoginNetLayer" );
	lua_tinker::class_inh<KLoginNetLayer, KNetLayer>( g_pLua );
	lua_tinker::class_def<KLoginNetLayer>( g_pLua, "dump", &KLoginNetLayer::Dump );

	lua_tinker::decl( g_pLua, "NetLayer", this );
}