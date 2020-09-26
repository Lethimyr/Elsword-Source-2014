#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  √÷¿∞ªÁ	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_RECORD_CHAT

class KGSChatLogDBThread : public KSubThread
{
	DeclareDBThread( KGSChatLogDBThread );
	//{{ 2009. 8. 17  √÷¿∞ªÁ	sp profiler
	DeclareSPProfiler;
	//}}
	DeclPfID;


public:
	KGSChatLogDBThread(void);
	virtual ~KGSChatLogDBThread(void);

protected:
	// derived from KSubThread
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

	DECL_ON_FUNC( DCHATLOG_RECORD_CHAT_NOT );
};

#endif SERV_RECORD_CHAT