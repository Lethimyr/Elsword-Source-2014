#include "ActorManager.h"
#include "GiantInfo.h"

#ifdef SERV_COUNTRY_CN

#include "BaseServer.h"
#include "KncSend.h"
#include "GiantInfoManager.h"
#include "NetError.h"

#define CLASS_TYPE  KGiantInfo

KGiantInfo::KGiantInfo( void )
{
}

KGiantInfo::~KGiantInfo( void )
{
	End( 15000 );
}

void KGiantInfo::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	CASE( EGIANT_COMMON_NULL_SERVER );
	CASE( EGIANT_COMMON_NULL_CLIENT );

	CASE( EGIANT_INFO_GET_CCU_REQ );
	CASE( EGIANT_INFO_GET_TOTAL_CCU_ACK );
	CASE( EGIANT_INFO_GET_CHANNEL_CCU_ACK );
	CASE( EGIANT_INFO_CHAT_NOT );
	CASE( EGIANT_INFO_PUNISH_REQ );
#ifdef SERV_INFOSERVER_ADD_WORK
	CASE( EGIANT_INFO_USERONLINE_ACK );
	CASE( EGIANT_INFO_REQUESTID_ACK );
#endif //SERV_INFOSERVER_ADD_WORK

	default:
		START_LOG( cerr, "핸들러가 지정되지 않은 이벤트." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"이벤트 처리 소요 시간." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}

IMPL_ON_FUNC( EGIANT_COMMON_NULL_SERVER )
{
	boost::shared_ptr< KGiantInfoPacket > spPacket( new KGiantInfoPacket );
	spPacket->Write( kPacket_ );
	SiKGiantInfoManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_COMMON_NULL_CLIENT )
{
	START_LOG( clog, L"InfoServer Null Client Req" )
		<< END_LOG;

	boost::shared_ptr< KGiantInfoPacket > spPacket( new KGiantInfoPacket );
	spPacket->Write( kPacket_ );
	SiKGiantInfoManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_INFO_GET_CCU_REQ )
{
	START_LOG( clog, L"EGIANT_INFO_GET_CCU_REQ" )
		<< BUILD_LOG( kPacket_.m_TimeStamp )
		<< END_LOG;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_SERVER, NULL, EGIANT_INFO_GET_CCU_REQ, kPacket_ );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
};

IMPL_ON_FUNC( EGIANT_INFO_GET_TOTAL_CCU_ACK )
{
	START_LOG( clog, L"EGIANT_INFO_GET_TOTAL_CCU_ACK" )
		<< BUILD_LOG( kPacket_.m_TimeStamp )
		<< BUILD_LOG( kPacket_.m_usServerID )
		<< BUILD_LOG( kPacket_.m_usServerType )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_strZoneName )
		<< BUILD_LOG( kPacket_.m_uiCCU )
		<< END_LOG;

	boost::shared_ptr< KGiantInfoPacket > spPacket( new KGiantInfoPacket );
	spPacket->Write( kPacket_ );
	SiKGiantInfoManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_INFO_GET_CHANNEL_CCU_ACK )
{
	START_LOG( clog, L"EGIANT_INFO_GET_CHANNEL_CCU_ACK" )
		<< BUILD_LOG( kPacket_.m_TimeStamp )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		//<< BUILD_LOG( kPacket_.m_uiCountryNum )
		//<< BUILD_LOG( kPacket_.m_uiCountryID )
		//<< BUILD_LOG( kPacket_.m_uiCountryCCU )
		<< END_LOG;

	boost::shared_ptr< KGiantInfoPacket > spPacket( new KGiantInfoPacket );
	spPacket->Write( kPacket_ );
	SiKGiantInfoManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_INFO_CHAT_NOT )
{
	START_LOG( clog, L"EGIANT_INFO_CHAT_NOT" )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrUserName )
		<< BUILD_LOG( kPacket_.m_wstrGMName )
		<< BUILD_LOG( kPacket_.m_uiCountryID )
		<< BUILD_LOG( kPacket_.m_uiSceneID )
		<< BUILD_LOG( kPacket_.m_uiType )
		<< BUILD_LOG( kPacket_.m_wstrContent )
		<< BUILD_LOG( kPacket_.m_wstrServer )
		<< END_LOG;

	boost::shared_ptr< KGiantInfoPacket > spPacket( new KGiantInfoPacket );
	spPacket->Write( kPacket_ );
	SiKGiantInfoManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_INFO_PUNISH_REQ )
{
	START_LOG( cout, L"EGIANT_INFO_PUNISH_REQ" )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrUserName )
		<< BUILD_LOG( kPacket_.m_wstrCountryName )
		<< BUILD_LOG( kPacket_.m_wstrServerName )
		<< BUILD_LOG( kPacket_.m_wstrGMName )
		<< BUILD_LOG( kPacket_.m_wstrReason )
		<< BUILD_LOG( kPacket_.m_usOperation )
		<< BUILD_LOG( kPacket_.m_iDelay )
		<< BUILD_LOG( kPacket_.m_sLevel )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_uiStartTime )
		<< END_LOG;

	// 일단 되돌려보냄
	boost::shared_ptr< KGiantInfoPacket > spPacket( new KGiantInfoPacket );
	spPacket->Write( kPacket_ );
	SiKGiantInfoManager()->QueueingSendPacket( spPacket );

	// 처벌 처리
//	KEventPtr spEvent( new KEvent );
//	spEvent->SetData( PI_CHANNEL_USER, NULL, EGIANT_INFO_PUNISH_REQ, kPacket_ );
//	KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );
};

#ifdef SERV_INFOSERVER_ADD_WORK
IMPL_ON_FUNC( EGIANT_INFO_USERONLINE_ACK )
{
	KEGIANT_INFO_USERONLINE_REQ kPacket;

	kPacket.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kPacket.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
	kPacket.m_wstrUserName = kPacket_.m_wstrUserName;
	kPacket.m_wstrGMName = kPacket_.m_wstrGMName;
	kPacket.m_bUserOnline = kPacket_.m_bUserOnline;

	START_LOG( clog, L"EGIANT_INFO_USERONLINE_REQ" )
		<< BUILD_LOG( kPacket.m_usZone )
		<< BUILD_LOG( kPacket.m_usGame )
		<< BUILD_LOG( kPacket.m_wstrUserName )
		<< BUILD_LOG( kPacket.m_wstrGMName )
		<< BUILD_LOG( kPacket.m_bUserOnline )
		<< END_LOG;

	boost::shared_ptr< KGiantInfoPacket > spPacket( new KGiantInfoPacket );
	spPacket->Write( kPacket );
	SiKGiantInfoManager()->QueueingSendPacket( spPacket );

};

IMPL_ON_FUNC( EGIANT_INFO_REQUESTID_ACK )
{
	KEGIANT_INFO_REQUESTID_REQ kPacket;


	kPacket.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kPacket.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
	kPacket.m_wstrUserName = kPacket_.m_wstrUserName;
	kPacket.m_wstrGMName = kPacket_.m_wstrGMName;
	kPacket.m_uiGiantUID = kPacket_.m_uiGiantUID;
	kPacket.m_wstrAccount = kPacket_.m_wstrAccount;
	kPacket.m_bWrite = kPacket_.m_bWrite;

	START_LOG( clog, L"EGIANT_INFO_REQUESTID_REQ" )
		<< BUILD_LOG( kPacket.m_usZone )
		<< BUILD_LOG( kPacket.m_usGame )
		<< BUILD_LOG( kPacket.m_wstrUserName )
		<< BUILD_LOG( kPacket.m_wstrGMName )
		<< BUILD_LOG( kPacket.m_uiGiantUID )
		<< BUILD_LOG( kPacket.m_wstrAccount )
		<< BUILD_LOG( kPacket.m_bWrite )
		<< END_LOG;

	boost::shared_ptr< KGiantInfoPacket > spPacket( new KGiantInfoPacket );
	spPacket->Write( kPacket );
	SiKGiantInfoManager()->QueueingSendPacket( spPacket );

};
#endif //SERV_INFOSERVER_ADD_WORK


#endif // SERV_COUNTRY_CN