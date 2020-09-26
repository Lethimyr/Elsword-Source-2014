#include "StdAfx.h"
#include ".\x2serverpacket.h"

CX2ServerPacket::CX2ServerPacket(void)
{
	bool bTest = false;
	if( bTest == false )
		bTest = true;
}

CX2ServerPacket::~CX2ServerPacket(void)
{
}

#include "CommonPacket.cpp"

#include "SystemPacket.cpp"
#include "ClientPacket.cpp"

#ifdef SERV_GLOBAL_COMMON
#include "OnlyGlobal/Packet/CommonPacket_Global.cpp"
#include "OnlyGlobal/Packet/SystemPacket_Global.cpp"
#include "OnlyGlobal/Packet/ClientPacket_Global.cpp"
#endif SERV_GLOBAL_COMMON

#ifdef ADD_COLLECT_CLIENT_INFO
#include "CollectPacket.cpp"
#endif