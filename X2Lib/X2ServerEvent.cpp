#include "StdAfx.h"
#include ".\x2serverevent.h"

#undef _ENUM
#define _ENUM( id ) L#id,

#ifndef _SERVICE_
const WCHAR*  CX2ServerEvent::SERVER_EVENT_ID_STR[] = { 
#   include "EventID_System.h"
#   include "EventID_Client.h"
};

#ifdef ADD_COLLECT_CLIENT_INFO
const WCHAR*  CX2ServerEvent::COLLECT_SERVER_EVENT_ID_STR[] = { 
#   include "EventID_Collect.h"
};
#endif //ADD_COLLECT_CLIENT_INFO

#else
const WCHAR*  CX2ServerEvent::SERVER_EVENT_ID_STR[] = {
	L"",
};

#ifdef ADD_COLLECT_CLIENT_INFO
const WCHAR*  CX2ServerEvent::COLLECT_SERVER_EVENT_ID_STR[] = { L"", };
#endif //ADD_COLLECT_CLIENT_INFO
#endif



CX2ServerEvent::CX2ServerEvent(void)
{
}

CX2ServerEvent::~CX2ServerEvent(void)
{
}
