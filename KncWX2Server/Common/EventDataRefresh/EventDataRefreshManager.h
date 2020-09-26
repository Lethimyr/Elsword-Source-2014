#pragma once
#include "ServerDefine.h"
#include "KncUtil.h"
#include "ServerPacket.h"

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KNCSingleton.h>
#include <ToString.h>
#include <map>

class KEventDataRefreshManager
{
	DeclareSingleton( KEventDataRefreshManager );
	DeclToStringW;
	DeclDump;

public:
	KEventDataRefreshManager();
	~KEventDataRefreshManager();

	// function
	const EVENT_DATA*	GetEventData( IN int iScriptID ) const;
	void				UpdateEventData( void );
	// DB에서 이벤트 패킷이 올경우..
	void				SetEventData( IN const std::map< int, EVENT_DATA >& mapEventData );

private:
	std::map< int, EVENT_DATA >				m_mapEventData;
};

DefSingletonInline( KEventDataRefreshManager );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}