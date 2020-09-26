#include <WinSock2.h>
#include "GiantBillingManager.h"

#ifdef SERV_COUNTRY_CN

//#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

#include "Socket/NetCommon.h"
#include "NetLayer.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "GiantBilling.h"
#include "GiantBillingPacket.h"

ImplToStringW( KGiantBillingManager )
{
	return stm_;
}

ImplPfID( KGiantBillingManager, PI_LOGIN_GIANT_BILLING );

ImplementSingleton( KGiantBillingManager );

KGiantBillingManager::KGiantBillingManager()
{
}

KGiantBillingManager::~KGiantBillingManager()
{
}

void KGiantBillingManager::RegToLua()
{
	lua_tinker::class_add<KGiantBillingManager>( g_pLua, "KGiantBillingManager" );
	lua_tinker::class_inh<KGiantBillingManager, KGiantBaseManager>( g_pLua );

	lua_tinker::decl( g_pLua, "GiantBillingManager", this );
}

KThread* KGiantBillingManager::CreateThread()
{
	return new KGiantBilling();
}

void KGiantBillingManager::MakeEventFromReceivedPacket()
{
	DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KGiantBillingPacket kReceivedPacket;
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
	case KGiantBillingPacket::GB_BCT_BALANCE:
	case KGiantBillingPacket::GB_BCT_RETURN:
	case KGiantBillingPacket::GB_BCT_CARD:
//	case KGiantBillingPacket::GB_BCT_USE_CARD:
		Handle_OnBillingPacket(kReceivedPacket);
		return;
	}

//end_proc:
	START_LOG( cerr, L"패킷 타입 이상." )
		<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
		<< BUILD_LOG( kReceivedPacket.GetCommand() )
		<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
		<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
}


void KGiantBillingManager::Handle_OnCommonPacket(KGiantCommonPacket& kReceivedPacket)
{
	switch( kReceivedPacket.GetCommand() )
	{
	case KGiantCommonPacket::GCP_CCT_INITIALIZE:
		{
			//switch(kReceivedPacket.GetParaCommand())
			{
			//case KGiantCommonPacket::GCP_PCT_INITIALIZE_ACK:			// UserServer만 별도사양으로 초기 인증시 paracommand를 구분하지 않는다 -_-
				{
					KEGIANT_COMMON_INITIALIZE_ACK kPacket;
					kReceivedPacket.Read( kPacket );

					m_kGiantServerInfo.m_usZone = kPacket.m_usZone;
					m_kGiantServerInfo.m_usGame = kPacket.m_usGame;
					m_kGiantServerInfo.m_strZoneName = kPacket.m_wstrZoneName;
					m_kGiantServerInfo.m_byteNetType = kPacket.m_byteNetType;

					START_LOG( cout, L"UserServer 인증 응답 성공!" )
						<< BUILD_LOG( kPacket.m_usZone )
						<< BUILD_LOG( kPacket.m_usGame )				
						<< BUILD_LOG( kPacket.m_wstrZoneName )
						<< BUILD_LOG( kPacket.m_byteNetType )
						<< END_LOG;
				}
				return;
			//default:
				// 인증 실패했으므로 소켓 연결을 종료한다.
			//	CLOSE_SOCKET( m_sock );		
			//	goto end_proc;
			}
		}
		break;
	case KGiantCommonPacket::GCP_CCT_NULL:
		{
			switch( kReceivedPacket.GetParaCommand() )
			{
			case KGiantCommonPacket::GCP_PCT_NULL_SERVER:
				{
					// 커넥션 체크를 위해 플랫폼에서 서버 널 패킷이 오면 그대로 돌려준다.
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
					// 커넥션 유지를 위해 클라이언트 널 패킷을 보내면 서버에서 그대로 돌려보낸다.
					// ACK이므로 별다른 처리 없이 그냥 무시.
					START_LOG( clog, L"UserServer Null Client" )
						<< END_LOG;

					return;
				}
				break;
			}
		}
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

void KGiantBillingManager::Handle_OnBillingPacket(KGiantBillingPacket& kReceivedPacket)
{
	switch( kReceivedPacket.GetCommand() )
	{
	// 카드 충전 필요시 추후 구현해야함
	/*
	case KGiantBillingPacket::GB_BCT_CARD:
		break;
	*/
	case KGiantBillingPacket::GB_BCT_RETURN:
		{
			KEGIANT_BILLING_RETURN kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_BILLING_RETURN, kPacket );
			QueueingEvent( spEvent );
			return;
		}
		break;

	default:
		break;
	}

	START_LOG( cerr, L"패킷 타입 이상." )
		<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
		<< BUILD_LOG( kReceivedPacket.GetCommand() )
		<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
		<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
}

#endif // SERV_COUNTRY_CN
