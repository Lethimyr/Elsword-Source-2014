#pragma once

//#include "Event.h"

// Performer 종류
#define PC_PROXY                0x00000001
#define PC_USER                 0x00000002
#define PC_CHARACTER            0x00000003
#define PC_SERVER               0x00000004
#define PC_ROOM                 0x00000005
#define PC_ACCOUNT_DB           0x00000006
#define PC_GAME_DB              0x00000007
#define PC_LOG_DB               0x00000008
#define PC_MASK_BIT             0x000000FF

// Server 종류
#define SC_CLIENT               0x00000100
#define SC_GAME                 0x00000200
#define SC_CENTER               0x00000300
#define SC_CHANNEL				0x00000400	// SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#define SC_MASK_BIT             0X00000F00

// 각각의 Performer Class 마다 ID 를 지정한다.
enum ePerformerID {
    // client
    PI_GS_PROXY         = SC_CLIENT | PC_PROXY,
    // game server
    PI_GS_USER          = SC_GAME | PC_USER,
    PI_GS_CHARACTER     = SC_GAME | PC_CHARACTER,
    PI_GS_SERVER        = SC_GAME | PC_SERVER,
    PI_GS_GAME_DB       = SC_GAME | PC_GAME_DB,
    PI_GS_ACCOUNT_DB    = SC_GAME | PC_ACCOUNT_DB,
    PI_GS_LOG_DB        = SC_GAME | PC_LOG_DB,
    PI_CN_PROXY         = SC_GAME | PC_PROXY,
    // center server
    PI_CN_USER          = SC_CENTER | PC_USER,
    PI_CN_SERVER        = SC_CENTER | PC_SERVER,
    PI_CN_ROOM          = SC_CENTER | PC_ROOM,
    PI_CN_ACCOUNT_DB    = SC_CENTER | PC_ACCOUNT_DB,
    PI_CN_LOG_DB        = SC_CENTER | PC_LOG_DB,
	// channel server
	PI_CHANNEL_USER		= SC_CHANNEL | PC_USER,
	PI_CHANNEL_SERVER	= SC_CHANNEL | PC_SERVER,
	PI_CHANNEL_LOG_DB	= SC_CHANNEL | PC_LOG_DB,
    // common db
    PI_ACCOUNT_DB       = PC_ACCOUNT_DB,
    PI_LOG_DB           = PC_LOG_DB,
    PI_NULL             = -1,
};
//
//int CompareServerLevel( DWORD dwDestPI, DWORD dwCurrentPI );
//DWORD GetServerLevel( DWORD dwPI );
//DWORD GetPerformerLevel( DWORD dwPI );
//DWORD GetPerformerClass( DWORD dwPI );
//DWORD MakePIOfSameLevel( DWORD dwPI, DWORD dwPC );
//
//void KncSend( DWORD dwPIFrom, UidType nFrom, KEventPtr spEvent );
//
//template <class T>
//void KncSend( DWORD dwPIFrom_, UidType nFrom_, DWORD dwPITo_, UidType nTo_, std::vector< UidType > vecTrace_, u_short usEventID_, const T& data_ )
//{
//    KEventPtr spEvent( new KEvent );
//    spEvent->SetData( dwPITo_, nTo_, vecTrace_, usEventID_, data_ );
//    KncSend( dwPIFrom_, nFrom_, spEvent );
//}
