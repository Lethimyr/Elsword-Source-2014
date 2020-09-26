#include "ChannelNetLayer.h"
#include <Socket/KTDNUDP.h>

NiImplementRTTI( KChannelNetLayer, KNetLayer );

KChannelNetLayer::KChannelNetLayer(void)
{    
}

KChannelNetLayer::~KChannelNetLayer(void)
{
}

ImplToStringW( KChannelNetLayer )
{
	return START_TOSTRING_PARENTW( KNetLayer )
        << L"   nothing to dump." << std::endl;
}

#include <KncLua.h>
#include <lua_tinker.h>
//using namespace lua_tinker;
#include "BaseServer.h"

void KChannelNetLayer::RegToLua()
{
    KNetLayer::RegToLua();

	lua_tinker::class_add<KChannelNetLayer>( g_pLua, "KChannelNetLayer" );
	lua_tinker::class_inh<KChannelNetLayer, KNetLayer>( g_pLua );
	lua_tinker::class_def<KChannelNetLayer>( g_pLua, "dump", &KChannelNetLayer::Dump );

	lua_tinker::decl( g_pLua, "NetLayer", this );
}