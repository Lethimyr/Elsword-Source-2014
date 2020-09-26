#include "CnNetLayer.h"
#include <Socket/KTDNUDP.h>

NiImplementRTTI( KCnNetLayer, KNetLayer );

KCnNetLayer::KCnNetLayer(void)
{    
}

KCnNetLayer::~KCnNetLayer(void)
{
}

ImplToStringW( KCnNetLayer )
{
	int iKTUDPPeerSize = -1;
	int iKTUDPPacketLossCount = -1;
	//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
	__int64 iUDPSendTraffic = -1;
	__int64 iUDPRecvTraffic = -1;
	//}}

#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
	if( CKTDNUDPPtr spUDP = m_kLBSUdpEcho.GetKTUDP() )
	{
		iKTUDPPeerSize = spUDP->GetPeerSize();
		//iKTUDPPacketLossCount = spUDP->GetPacketLossCount();
		iUDPSendTraffic = spUDP->GetUDPSendTraffic();
		iUDPRecvTraffic = spUDP->GetUDPRecvTraffic();
    }
#else   SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
	CKTDNUDPPtr spUDP = m_kLBSUdpEcho.GetKTUDP();
	if( spUDP != NULL )
	{
		iKTUDPPeerSize = m_kLBSUdpEcho.GetKTUDP()->GetPeerSize();
		//iKTUDPPacketLossCount = m_kLBSUdpEcho.GetKTUDP()->GetPacketLossCount();
		iUDPSendTraffic = m_kLBSUdpEcho.GetKTUDP()->GetUDPSendTraffic();
		iUDPRecvTraffic = m_kLBSUdpEcho.GetKTUDP()->GetUDPRecvTraffic();
	}
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX


	return START_TOSTRING_PARENTW( KNetLayer )
		<< TOSTRINGW( iKTUDPPeerSize )
		//{{ 2009. 12. 14  최육사	RELAY서버예외처리
		//<< TOSTRINGW( iKTUDPPacketLossCount )
		//}}
		//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
		<< TOSTRINGW( iUDPSendTraffic )
		<< TOSTRINGW( iUDPRecvTraffic )
		//}}
        << L"   nothing to dump." << std::endl;
}

#include <KncLua.h>
#include <lua_tinker.h>
#include "BaseServer.h"
//using namespace lua_tinker;

void KCnNetLayer::RegToLua()
{
    KNetLayer::RegToLua();

    //class_<KCnNetLayer>( "KCnNetLayer" )
    //    .inh<KNetLayer>()
    //    .def( "dump", KCnNetLayer::Dump );

    //decl( "NetLayer", this );
	lua_tinker::class_add<KCnNetLayer>( g_pLua, "KCnNetLayer" );
	lua_tinker::class_inh<KCnNetLayer, KNetLayer>( g_pLua );
	lua_tinker::class_def<KCnNetLayer>( g_pLua, "dump", &KCnNetLayer::Dump );

	lua_tinker::decl( g_pLua, "NetLayer", this );
}

//{{ 2009. 12. 15  최육사	서버관리
void KCnNetLayer::DumpToLogFile()
{
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
//}}