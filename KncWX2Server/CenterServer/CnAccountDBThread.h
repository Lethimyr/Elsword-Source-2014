#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class KCnAccountDBThread : public KSubThread
{
    DeclareDBThread( KCnAccountDBThread );
    DeclPfID;
public:

    KCnAccountDBThread();   // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KCnAccountDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
};