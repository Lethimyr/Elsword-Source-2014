
#include "ServerDefine.h"

//////////////////////////////////////////////////////////////////////////
// for visual assist

#ifndef EVENT_HANDLER
#define EVENT_HANDLER( eventid )
#endif

#ifndef EVENT_HANDLER_NOPARAM
#define EVENT_HANDLER_NOPARAM( eventid )
#endif

#ifndef _EVENT_HANDLER
#define _EVENT_HANDLER( eventid, structtype )
#endif


#ifdef SERV_COUNTRY_CN

//////////////////////////////////////////////////////////////////////////
// 여기서 부터 이벤트 핸들러 정의!

// 2010. 07. 26.	박교현		유일성 인증
EVENT_HANDLER( EGIANT_ROLEREG_CREATE_UNIT_ACK );
EVENT_HANDLER( EGIANT_ROLEREG_CREATE_GUILD_ACK );

//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
EVENT_HANDLER( DBE_GET_ANTI_ADDICTION_INFO_ACK );
EVENT_HANDLER( EGS_ANTI_ADDICTION_NOT );
#endif SERV_ANTI_ADDICTION_SYSTEM
//}}

#ifdef SERV_NEW_PUNISHMENT
EVENT_HANDLER( EGIANT_INFO_PUNISH_REQ );
EVENT_HANDLER( DBE_GET_PUNISHMENT_ACK );
#endif SERV_NEW_PUNISHMENT

#ifdef SERV_INFOSERVER_ADD_WORK
EVENT_HANDLER( EGIANT_INFO_REQUESTID_REQ );
#endif //SERV_INFOSERVER_ADD_WORK


#endif SERV_COUNTRY_CN