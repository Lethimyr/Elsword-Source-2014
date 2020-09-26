#include "GSNetLayer.h"

NiImplementRTTI( KGSNetLayer, KNetLayer );

KGSNetLayer::KGSNetLayer(void)
{
}

KGSNetLayer::~KGSNetLayer(void)
{
}

ImplToStringW( KGSNetLayer )
{
    return START_TOSTRING_PARENTW( KNetLayer )
        << L"   nothing to dump." << std::endl;
}

#include <KncLua.h>
#include <lua_tinker.h>
//using namespace lua_tinker;
#include "BaseServer.h"

void KGSNetLayer::RegToLua()
{
    KNetLayer::RegToLua();

	lua_tinker::class_add<KGSNetLayer>( g_pLua, "KGSNetLayer" );
	lua_tinker::class_inh<KGSNetLayer, KNetLayer>( g_pLua );
	lua_tinker::class_def<KGSNetLayer>( g_pLua, "dump", &KGSNetLayer::Dump );

	lua_tinker::decl( g_pLua, "NetLayer", this );
}