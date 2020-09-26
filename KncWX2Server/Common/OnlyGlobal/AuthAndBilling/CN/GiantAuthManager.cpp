#include <WinSock2.h>
#include "GiantAuthManager.h"

//#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

#ifdef SERV_COUNTRY_CN

#include "Socket/NetCommon.h"
#include "NetLayer.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "GiantAuth.h"
#include "GiantAuthPacket.h"

ImplToStringW( KGiantAuthManager )
{
	return stm_;
}

ImplPfID( KGiantAuthManager, PI_LOGIN_GIANT_AUTH );

ImplementSingleton( KGiantAuthManager );


KGiantAuthManager::KGiantAuthManager()
: m_mapRequest( 100000 )
{

	const unsigned int max_size = 10000000; //200000000; //INT_MAX;		// 200000000 이상을 넘어가면 에러 발생함. 메모리를 너무 많이 사용하므로 한계값보다 작은 일부만 사용.
	for (unsigned int i=1; i<max_size; ++i)
	{
		m_qRequestID.push(i);
	}
	START_LOG( cout, L"RequestID Pool 초기화" )
		<< BUILD_LOG( m_qRequestID.unsafe_size() )		// thread unsafe
		<< END_LOG;


}

KGiantAuthManager::~KGiantAuthManager()
{

	START_LOG( cout, L"Remained RequestID Pool" )
		<< BUILD_LOG( m_qRequestID.unsafe_size() )		// thread unsafe
		<< END_LOG;

	START_LOG( cout, L"Remained Request" )
		<< BUILD_LOG( m_mapRequest.size() )
		<< END_LOG;

}

void KGiantAuthManager::RegToLua()
{
	lua_tinker::class_add<KGiantAuthManager>( g_pLua, "KGiantAuthManager" );

	lua_tinker::class_inh<KGiantAuthManager, KGiantBaseManager>( g_pLua );

	lua_tinker::decl( g_pLua, "GiantAuthManager", this );
}

KThread* KGiantAuthManager::CreateThread()
{
	return new KGiantAuth;
}

void KGiantAuthManager::MakeEventFromReceivedPacket()
{
	DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KGiantAuthPacket kReceivedPacket;
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
	case KGiantAuthPacket::GAP_ACT_LOGIN:
		Handle_OnLoginPacket(kReceivedPacket);
		return;
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	case KGiantAuthPacket::GAP_ACT_SESSION:
		Handle_OnDirectChargePacket(kReceivedPacket);
		return;
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
	default:
		START_LOG( cerr, L"패킷 타입 이상." )
			<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
			<< BUILD_LOG( kReceivedPacket.GetCommand() )
			<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
			<< END_LOG;

		DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
		break;
	}
}


void KGiantAuthManager::Handle_OnCommonPacket(KGiantCommonPacket& kReceivedPacket)
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

					START_LOG( cout, L"HLServer 인증 응답 성공!" )
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
					START_LOG( clog, L"HLServer Null Client" )
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

void KGiantAuthManager::Handle_OnLoginPacket(KGiantAuthPacket& kReceivedPacket)
{
	if( kReceivedPacket.GetCommand() != KGiantAuthPacket::GAP_ACT_LOGIN )
		goto end_proc;

	switch( kReceivedPacket.GetParaCommand() )
	{
	case KGiantAuthPacket::GA_PCT_LOGIN_FAIL:
		{
			KEGIANT_AUTH_LOGIN_FAIL kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_AUTH_LOGIN_FAIL, kPacket );
			QueueingEvent( spEvent );
		}
		return;
	case KGiantAuthPacket::GA_PCT_LOGIN_SUCCESS:
		{
			KEGIANT_AUTH_LOGIN_SUCCESS kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_AUTH_LOGIN_SUCCESS, kPacket );
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


unsigned int KGiantAuthManager::CreateRequestID()
{
	unsigned int iRequestID = 0;
	if(m_qRequestID.try_pop(iRequestID))
	{
		if(iRequestID == 0)
		{
			START_LOG( cerr, L"RequestID 0이 생성되었다?" )
				<< END_LOG;
		}
		
		return iRequestID;
	}
	else
	{
		START_LOG( cerr, L"RequestID 생성 실패" )
			<< BUILD_LOG( m_qRequestID.unsafe_size() )		// thread unsafe
			<< END_LOG;	

		return 0;
	}
}

void KGiantAuthManager::ReleaseRequestID(unsigned int iRequestID)
{
	if(iRequestID == 0)
	{
		START_LOG( cerr, L"RequestID 0은 사용할 수 없음" )
			<< END_LOG;

		return;
	}

	m_qRequestID.push(iRequestID);
}

int KGiantAuthManager::RegisterRequest(const RequestInfo& info)
{
	int iRequestID = CreateRequestID();
	if(iRequestID == 0)
		return 0;

	RequestMap::accessor acc;
	if(m_mapRequest.insert(acc, iRequestID))
	{
		acc->second = info;
		acc.release();
	}
	else
	{
		// 해당 요청이 이미 맵에 존재한다?!
		START_LOG( cerr, L"해당 RequestID가 이미 맵에 존재함" )
			<< BUILD_LOG( iRequestID )
			<< END_LOG;

		// RequestID를 다시 사용할 수 있도록 큐에 다시 넣어준다.
		m_qRequestID.push(iRequestID);

		return 0;
	}

	return iRequestID;
}

bool KGiantAuthManager::UnregisterRequest(int iRequestID, RequestInfo& info)
{
	if(iRequestID == 0)
		return false;

	RequestMap::accessor acc;
	if(m_mapRequest.find(acc, iRequestID))
	{
		info = acc->second;

		m_mapRequest.erase(acc);
		acc.release();

		// RequestID를 다시 사용할 수 있도록 큐에 다시 넣어준다.
		m_qRequestID.push(iRequestID);

		return true;
	}
	else
	{
		START_LOG( cerr, L"해당 RequestID가 맵에 존재하지 않음" )
			<< BUILD_LOG( iRequestID )
			<< END_LOG;

		return false;
	}
}


#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
void KGiantAuthManager::Handle_OnDirectChargePacket(KGiantAuthPacket& kReceivedPacket)
{
	START_LOG( cout, L"[TEST] Token 패킷 응답이 왔다!" )
		<< END_LOG;

	if( kReceivedPacket.GetCommand() != KGiantAuthPacket::GAP_ACT_SESSION )
		goto end_proc;

	switch( kReceivedPacket.GetParaCommand() )
	{
	case KGiantAuthPacket::GAS_PCT_SESSION_GETTOKEN:
		{
			KEGIANT_AUTH_DIRECT_CHARGE_ACK kPacket;
			kReceivedPacket.Read( kPacket );

			KEventPtr spEvent( new KEvent );
			spEvent->SetData(PI_NULL, NULL, EGIANT_AUTH_DIRECT_CHARGE_ACK, kPacket );
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
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH


#endif // SERV_COUNTRY_CN