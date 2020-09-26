#pragma once

#   undef  _ENUM
#   define _ENUM( id ) id,
enum SERVER_EVENT_ID 
{ 
//#include "X2ServerEventDef.h" 

_ENUM( EVENT_X2_STARTUP = E_SYSTEM_EVENT_ID_END )

#include "EventID_Client.h"

};

#ifdef ADD_COLLECT_CLIENT_INFO
enum COLLECT_SERVER_EVENT_ID 
{ 	
	#include "EventID_Collect.h"
};


#endif

class CX2ServerEvent
{
	public:
		CX2ServerEvent(void);
		~CX2ServerEvent(void);

		static const WCHAR*   SERVER_EVENT_ID_STR[];

#ifdef ADD_COLLECT_CLIENT_INFO
		static const WCHAR*		COLLECT_SERVER_EVENT_ID_STR[];
#endif
};
