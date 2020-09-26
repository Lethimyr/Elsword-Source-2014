#include "EventNotifyMsgServerInfo.h"
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include "BaseServer.h"
#include "DBLayer.h"

ImplementSingleton( KEventNotifyMsgServerInfo );

KEventNotifyMsgServerInfo::KEventNotifyMsgServerInfo()
{
	m_iNotifyCnServerUID = 0;
	m_bNotifyMSGEvent	 = false;
}

KEventNotifyMsgServerInfo::~KEventNotifyMsgServerInfo()
{
}

ImplToStringW( KEventNotifyMsgServerInfo )
{
	stm_	<< L"----------[ Event Data Refresh Manager ]----------" << std::endl
		<< TOSTRINGW( m_iNotifyCnServerUID )
		<< TOSTRINGW( m_bNotifyMSGEvent )
		;

	return stm_;
}

void KEventNotifyMsgServerInfo::Init( void )
{
	// 공지날릴 서버 설정
	m_bNotifyMSGEvent = ( KBaseServer::GetKObj()->GetUID() == m_iNotifyCnServerUID );
	if( m_bNotifyMSGEvent )
	{
		START_LOG( cout, L"공지 이벤트를 날릴 센터 서버로 등록되었음!" );
	}
}

void KEventNotifyMsgServerInfo::UpdateEventNotifyMsgServerInfo( void )
{
	UidType						anTrace[2] = { 0, -1 };
	KEventPtr					spEvent( new KEvent );

	spEvent->SetData( PC_LOG_DB, anTrace, DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_REQ );

	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

void KEventNotifyMsgServerInfo::SetEventNotifyMsgServerInfoData( IN const std::vector< std::pair< int, int > >& vecEventNotifyMsgServerInfo )
{
	int iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();

	for( size_t i=0; i < vecEventNotifyMsgServerInfo.size(); ++i )
	{
		const int iConstIndex = i;

		// 해당 서버군인지 체크
		if( iServerGroupID == vecEventNotifyMsgServerInfo[iConstIndex].first )
		{
			m_iNotifyCnServerUID = vecEventNotifyMsgServerInfo[iConstIndex].second;
		}
	}
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}