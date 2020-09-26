#pragma once
#include "../Socket/KTDNUDP.h"
#include <thread/thread.h>
#include <windows.h>
#pragma comment( lib, "winmm.lib" )

#ifdef  SERV_UDP_RELAY_CHECKER


class KUdpRelayChecker;
class KUdpCheckerSession : public CKTDNUDP, public KThread
{
public:
    KUdpCheckerSession( short sUdpPort );
    virtual ~KUdpCheckerSession(void);

	// function
	void Init( KUdpRelayChecker* pUdpRelayChecker, const __int64 iPeerUID );

	// connect
	void SetRelayIPPort( const std::wstring& wstrRelayServIP, const short sRelayServPort );
	void ConnectRelayServer( const std::wstring& wstrRelayServIP, const short sRelayServPort );

	// derived from KThread
    virtual void Run();
    void ShutDown();

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	// derived from CKTDNUDP
	void OnFrameMove();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	// derived from CKTDNUDP
//	void OnFrameMove( double fTime, float fElapsedTime );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

protected:
    inline void Loop();

protected:
	KUdpRelayChecker*	m_pUdpRelayChecker;

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
private:
	void _ProcessRecvData();
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
};


#endif  SERV_UDP_RELAY_CHECKER
