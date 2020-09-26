#pragma once

#include "Performer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"

//{{ 2011. 11. 21  ±è¹Î¼º	Åë°è ·Î±× ¼öÄ¡
#ifdef SERV_STATISTICS_LOG_COUNT
	#include <boost/timer.hpp>
#endif SERV_STATISTICS_LOG_COUNT
//}}

#ifdef SERV_STATISTICS_THREAD


class KTSMEventProcess : public KPerformer
{
	DeclPfID;

public:
	KTSMEventProcess();
	virtual ~KTSMEventProcess();

	// override.	
	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer

	DECL_ON_FUNC( E_STATISTICS_INFO_NOT );
	DECL_ON_FUNC_NOPARAM( EGS_SEND_STATISTICS_LOCLOG_REQ );
	DECL_ON_FUNC_NOPARAM( EGS_SEND_STATISTICS_DBLOG_REQ );
	DECL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_LOCLOG_REQ );
	DECL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_DBLOG_REQ );
	DECL_ON_FUNC( E_HENIR_RANKING_LOG_NOT );
	DECL_ON_FUNC( E_LOCAL_LOG_PVP_NOT );
	DECL_ON_FUNC( E_LOCAL_LOG_PVP_ROOM_NOT );
	DECL_ON_FUNC( E_LOCAL_LOG_DUNGEON_ROOM_NOT );
	DECL_ON_FUNC( E_LOCAL_LOG_TITLE_NOT );
	DECL_ON_FUNC( E_LOCAL_LOG_DUNGEON_NOT );
	DECL_ON_FUNC( E_LOCAL_LOG_HENIR_RANKING_NOT );
	//{{ 2010. 9. 14	ÃÖÀ°»ç	Æê ½Ã½ºÅÛ
#ifdef SERV_PET_SYSTEM
	DECL_ON_FUNC( E_LOCAL_LOG_PET_SUMMON_NOT );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 10. 11	ÃÖÀ°»ç	¼­¹ö°£ Á¢¼Ó ²÷±è ·Î±×
#ifdef SERV_SERVER_DISCONNECT_LOG
	DECL_ON_FUNC( E_LOCAL_LOG_SERVER_DISCONNECT_NOT );
#endif SERV_SERVER_DISCONNECT_LOG
	//}}
	//{{ 2010. 10. 11	ÃÖÀ°»ç	³Ø½¼ ºô¸µ ÆÐÅ¶ ·Î±×
#ifdef SERV_BILLING_PACKET_LOG
	DECL_ON_FUNC( E_LOCAL_LOG_BILLING_PACKET_NOT );
#endif SERV_BILLING_PACKET_LOG
	//}}
	//{{ 2010. 11. 17	ÃÖÀ°»ç	¿ÀÅäÇÙ °ËÁõ ±â´É
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	DECL_ON_FUNC( E_LOCAL_LOG_ABUSER_MORNITORING_NOT );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}

#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
	DECL_ON_FUNC( E_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT );
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

protected:	

	//{{ 2011. 11. 21  ±è¹Î¼º	Åë°è ·Î±× ¼öÄ¡
#ifdef SERV_STATISTICS_LOG_COUNT
	boost::timer						m_tStatisticsLogCount;
	UidType								m_iStatisticsLogCount;
#endif SERV_STATISTICS_LOG_COUNT
	//}}

};


#endif SERV_STATISTICS_THREAD