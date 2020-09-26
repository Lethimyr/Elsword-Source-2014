#pragma once

#include "ServerDefine.h"
#include <thread/thread.h>
#include <KncSmartPtr.h>

SmartPointer( CKTDNUDP );

class KLBSUdpEcho : public KThread
{
public:
    KLBSUdpEcho(void);
    virtual ~KLBSUdpEcho(void);

    bool Init( USHORT usPort );
    virtual void Run();     // derived from KThread

    u_short GetPort();

	CKTDNUDPPtr GetKTUDP() const { return m_spUDP; }

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//protected:
//    inline void Loop();
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

protected:
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    USHORT  m_usPort;
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    CKTDNUDPPtr m_spUDP;

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
private:
	void _ProcessRecvData();

//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
};