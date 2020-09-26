#pragma once

#include "Performer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"


class KCnRMEventProcess : public KPerformer
{
	//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	DeclToStringW;
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}	
	DeclPfID;

public:
	KCnRMEventProcess();
	virtual ~KCnRMEventProcess();

	// override.	
	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer

   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );

	DECL_ON_FUNC( DBE_GET_TIME_DROP_INFO_ACK );
	DECL_ON_FUNC( DBE_CHECK_TIME_DROP_RESULT_ACK );
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	DECL_ON_FUNC( DBE_ABUSER_LIST_ACK );
	DECL_ON_FUNC( DBE_EVENT_UPDATE_ACK );
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	DECL_ON_FUNC( DBE_GET_DUNGEON_EVENT_INFO_ACK );
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC( DBE_EVENT_DB_SCRIPT_ACK );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

protected:	
};