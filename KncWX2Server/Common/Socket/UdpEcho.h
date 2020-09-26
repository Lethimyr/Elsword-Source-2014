#pragma once
#include <thread/thread.h>
#include <queue>

#include "Socket/UdpToServer.h"

class KSimLayer;
class KUdpEcho : public KThread
{
public:
    KUdpEcho(void);
    virtual ~KUdpEcho(void);

    bool Init( USHORT usPort );
    virtual void Run();     // derived from KThread
    void ShutDown();

protected:
    inline void Loop();

    template < class T > void SendToClient( UidType nTo, unsigned short usEventID, const T& data );
    void SendToClient( UidType nTo, unsigned short usEventID );

    SOCKET      m_sock;
};