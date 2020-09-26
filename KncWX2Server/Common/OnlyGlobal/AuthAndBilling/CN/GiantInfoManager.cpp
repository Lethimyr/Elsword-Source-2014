#include <WinSock2.h>
#include "GiantInfoPacket.h"
#include "GiantInfoManager.h"

#ifdef SERV_COUNTRY_CN

#include "Socket/NetCommon.h"
#include "NetLayer.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "GiantInfo.h"


ImplToStringW( KGiantInfoManager )
{
	return stm_;
}
//template <typename TMgr, typename TPacket>
ImplPfID( KGiantInfoManager, PI_LOGIN_GIANT_INFO );

ImplementSingleton( KGiantInfoManager );


KGiantInfoManager::KGiantInfoManager()
{
}


KGiantInfoManager::~KGiantInfoManager()
{
}

//#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KGiantInfoManager::RegToLua()
{
	lua_tinker::class_add<KGiantInfoManager>( g_pLua, "KGiantInfoManager" );
	lua_tinker::class_inh<KGiantInfoManager, KGiantBaseManager>( g_pLua );

	lua_tinker::decl( g_pLua, "GiantInfoManager", this );
}

KThread* KGiantInfoManager::CreateThread()
{
	return new KGiantInfo;
}

void KGiantInfoManager::MakeEventFromReceivedPacket()
{
	DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KGiantInfoPacket kReceivedPacket;
	if( !kReceivedPacket.ReadFromBuffer( ( BYTE* )m_cRecvBuffer ) )
	{
		START_LOG( cerr, L"버퍼에서 읽기 실패." )
			<< END_LOG;

		DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
		return;
	}

	switch( kReceivedPacket.GetCommand() )
	{
	case KGiantCommonPacket::GCP_CCT_NULL:
	case KGiantCommonPacket::GCP_CCT_INITIALIZE:
		Handle_OnCommonPacket(kReceivedPacket);
		return;
	case KGiantInfoPacket::GIP_CT_INFOSERVER:
		Handle_InfoServerPacket(kReceivedPacket);
		return;
	case KGiantInfoPacket::GIP_CT_GMTOOL:
		Handle_GMToolPacket(kReceivedPacket);
		break;
	}

//end_proc:
	START_LOG( cerr, L"패킷 타입 이상." )
		<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
		<< BUILD_LOG( kReceivedPacket.GetCommand() )
		<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
		<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
}


void KGiantInfoManager::Handle_OnCommonPacket(KGiantCommonPacket& kReceivedPacket)
{
	switch( kReceivedPacket.GetCommand() )
	{
	case KGiantCommonPacket::GCP_CCT_INITIALIZE:
		{
			switch(kReceivedPacket.GetParaCommand())
			{
			case KGiantCommonPacket::GCP_PCT_INITIALIZE_ACK:
				{
					KEGIANT_COMMON_INITIALIZE_ACK kPacket;
					kReceivedPacket.Read( kPacket );

					m_kGiantServerInfo.m_usZone = kPacket.m_usZone;
					m_kGiantServerInfo.m_usGame = kPacket.m_usGame;
					m_kGiantServerInfo.m_strZoneName = kPacket.m_wstrZoneName;
					m_kGiantServerInfo.m_byteNetType = kPacket.m_byteNetType;

					START_LOG( cout, L"InfoServer 인증 응답 성공!" )
						<< BUILD_LOG( kPacket.m_usZone )
						<< BUILD_LOG( kPacket.m_usGame )				
						<< BUILD_LOG( kPacket.m_wstrZoneName )
						<< BUILD_LOG( kPacket.m_byteNetType )
						<< END_LOG;
				}
				return;
			default:
				// 인증 실패했으므로 소켓 연결을 종료한다.
				CLOSE_SOCKET( m_sock );		
				goto end_proc;
			}
		}
		break;
	case KGiantCommonPacket::GCP_CCT_NULL:
		{
			switch( kReceivedPacket.GetParaCommand() )
			{
			case KGiantCommonPacket::GCP_PCT_NULL_SERVER:
				{
					// 커넥션 유지를 위해 서버에서 서버 널 패킷이 오면 그대로 돌려준다.
					START_LOG( clog, L"InfoServer Null Server Req" )
						<< END_LOG;

					KEGIANT_COMMON_NULL_SERVER kPacket;
					kReceivedPacket.Read( kPacket );

					KEventPtr spEvent( new KEvent );
					spEvent->SetData(PI_NULL, NULL, EGIANT_COMMON_NULL_SERVER, kPacket );
					QueueingEvent( spEvent );

					return;
				}
				break;
			case KGiantCommonPacket::GCP_PCT_NULL_CLIENT:
				{
					// 돌려 받은 패킷이므로 별다른 처리 없이 그냥 무시.
					START_LOG( clog, L"InfoServer Null Client Ack" )
						<< END_LOG;

					return;
				}
				break;
			}
		}
		break;
	}

end_proc:
	START_LOG( cerr, L"패킷 타입 이상." )
		<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
		<< BUILD_LOG( kReceivedPacket.GetCommand() )
		<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
		<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
}

void KGiantInfoManager::Handle_InfoServerPacket(KGiantInfoPacket& kReceivedPacket)
{
	if( kReceivedPacket.GetCommand() != KGiantInfoPacket::GIP_CT_INFOSERVER )
		goto end_proc;

	switch( kReceivedPacket.GetParaCommand() )
	{
	case KGiantInfoPacket::GI_PCT_CCU_REQ:
		{
			KEGIANT_INFO_GET_CCU_REQ kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_INFO_GET_CCU_REQ, kPacket );
			QueueingEvent( spEvent );
		}
		return;
	case KGiantInfoPacket::GI_PCT_TOTAL_CCU_ACK:
		{
			KEGIANT_INFO_GET_TOTAL_CCU_ACK kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_INFO_GET_TOTAL_CCU_ACK, kPacket );
			QueueingEvent( spEvent );
		}
		return;
	case KGiantInfoPacket::GI_PCT_CHANNEL_CCU_ACK:
		{
			KEGIANT_INFO_GET_CHANNEL_CCU_ACK kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_INFO_GET_CHANNEL_CCU_ACK, kPacket );
			QueueingEvent( spEvent );
		}
		return;
	case KGiantInfoPacket::GI_PCT_CHAT:
		{
			KEGIANT_INFO_CHAT_NOT kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_INFO_CHAT_NOT, kPacket );
			QueueingEvent( spEvent );
		}
		return;
	}

end_proc:
	START_LOG( cerr, L"패킷 타입 이상." )
	<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
	<< BUILD_LOG( kReceivedPacket.GetCommand() )
	<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
	<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
}

void KGiantInfoManager::Handle_GMToolPacket(KGiantInfoPacket& kReceivedPacket)
{
	if( kReceivedPacket.GetCommand() != KGiantInfoPacket::GIP_CT_GMTOOL )
		goto end_proc;

	switch( kReceivedPacket.GetParaCommand() )
	{
	//case KGiantInfoPacket::GI_PCT_PUNISH:
	//	{
	//		KEGIANT_INFO_PUNISH_REQ kPacket;
	//		kReceivedPacket.Read( kPacket );

	//		KEventPtr spEvent( new KEvent );
	//		spEvent->SetData(PI_NULL, NULL, EGIANT_INFO_PUNISH_REQ, kPacket );
	//		QueueingEvent( spEvent );
	//	}
	//	return;
#ifdef SERV_INFOSERVER_ADD_WORK
	case KGiantInfoPacket::GI_PCT_USER_ON_LINE:
		{
			KEGIANT_INFO_USERONLINE_REQ kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_INFO_USERONLINE_REQ, kPacket );
			QueueingEvent( spEvent );
		}
		return;

	case KGiantInfoPacket::GI_PCT_REQUEST_ID:
		{
			KEGIANT_INFO_REQUESTID_REQ kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_INFO_REQUESTID_REQ, kPacket );
			QueueingEvent( spEvent );
		}
		return;
#endif //SERV_INFOSERVER_ADD_WORK
	}

end_proc:
	START_LOG( cerr, L"패킷 타입 이상." )
		<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
		<< BUILD_LOG( kReceivedPacket.GetCommand() )
		<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
		<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
}

void KGiantInfoManager::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}

#endif // SERV_COUNTRY_CN