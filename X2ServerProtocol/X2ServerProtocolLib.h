#pragma once

#ifndef UNICODE
#define UNICODE
#endif

//#define X2OPTIMIZE_TCP_RELAY_TEST

#define ADD_COLLECT_CLIENT_INFO_PROTOCOL
#define X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
#define X2OPTIMIZE_SESSION_THREAD_SAFETY

#pragma comment( lib, "ws2_32.lib" )
#pragma warning ( disable : 4702 )
#pragma warning ( disable : 4995 )
#pragma warning ( disable : 4996 )
#pragma warning ( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

#include <Winsock2.h>
#include <windows.h>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <atltime.h>    // CString

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include <KncSecurity/KncSecurity.h>
#include <Serializer/SerBuffer.h>
#include <Serializer/SerPair.h>
#include <Serializer/SerList.h>
#include <Serializer/SerVector.h>
#include <Serializer/SerDeque.h>
#include <Serializer/SerMap.h>
#include <Serializer/SerSet.h>
#include <Serializer/SerializeMacro.h>
#include <ToString.h>
#include <RTTI.H>
#include <Thread/Thread.h>
#include <Thread/Locker.h>
#include <ObjectPool/BlockAllocator.h>
#include <KNCSingleton.h>
#include <dbg/dbg.hpp>
#include <KncUtil.h>    // toWideString

extern bool g_bIsSERVICE;

typedef bool (*SEND_MSG_FUNC)( UINT uMsg, WPARAM wParam, LPARAM lParam, bool bDirectProcess );

#include "KncException.h"
#include "KncUidType.h"
#include "KncSmartPtr.h"
#include "Event.h"
#include "SimObject.h"
#include "Performer.h"
#include "ThreadManager.h"
#include "Overlapped.h"
#include "CircularQueue.h"
#include "SocketObject.h"
#include "Session.h"
#include "IOCP.h"
#include "IOThread.h"
#include "ActorProxy.h"
#include "KNCSingleton.h"
#include "SystemPacket.h"
#include "UserProxy.h"
#include "KncSend.h"
#include "TRUser.h"

#include "X2ServerProtocol.h"
//#ifndef X2OPTIMIZE_TCP_RELAY_TEST
#if !defined( X2TOOL ) && !defined( EFFECT_TOOL ) 
#include "Odbc.h"
#endif
//#endif//X2OPTIMIZE_TCP_RELAY_TEST




