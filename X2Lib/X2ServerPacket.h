#pragma once

//#include "X2ServerPacketDef.h"

#include "CommonPacket.h"

#include "SystemPacket.h"
#include "ClientPacket.h"

#ifdef ADD_COLLECT_CLIENT_INFO
#include "CollectPacket.h"
#endif

class CX2ServerPacket
{
public:
	CX2ServerPacket(void);
	~CX2ServerPacket(void);
};
