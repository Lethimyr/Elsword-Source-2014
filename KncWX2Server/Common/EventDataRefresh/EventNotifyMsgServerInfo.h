#pragma once

#include "ServerDefine.h"
#include "KncUtil.h"
#include "KncUidType.h"

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KNCSingleton.h>
#include <ToString.h>

class KEventNotifyMsgServerInfo
{
	DeclareSingleton( KEventNotifyMsgServerInfo );
	DeclDump;
	DeclToStringW;

public:
	KEventNotifyMsgServerInfo();
	~KEventNotifyMsgServerInfo();

	// function
	void Init( void );
	bool IsNotifyMSGEvent( void ) const { return m_bNotifyMSGEvent; }
	void UpdateEventNotifyMsgServerInfo( void );
	// DB에서 이벤트 패킷이 올경우..
	void SetEventNotifyMsgServerInfoData( IN const std::vector< std::pair< int, int > >& vecEventNotifyMsgServerInfo );

private:
	//{{ 2008. 7. 11  최육사	공지 이벤트
	UidType									m_iNotifyCnServerUID;
	bool									m_bNotifyMSGEvent;
	//}}
};

DefSingletonInline( KEventNotifyMsgServerInfo );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}