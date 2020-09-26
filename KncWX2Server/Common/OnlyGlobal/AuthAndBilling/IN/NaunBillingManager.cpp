#include <WinSock2.h>
#include "NaunBillingManager.h"

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "NaunBilling.h"

#ifdef SERV_COUNTRY_IN

//DWORD KNaunBillingManager::ms_dwConnectionCheckGap[] = { 15 * 1000,  15 * 1000  };

ImplToStringW( KNaunBillingManager )
{
	return stm_;
}

ImplPfID( KNaunBillingManager, PI_GS_PUBLISHER_BILLING );
ImplementSingleton( KNaunBillingManager );

KNaunBillingManager::KNaunBillingManager()
{
	/*
	for( int iPortNum = 0; iPortNum < NP_NUM; iPortNum++ )
	{
		m_dwLastConnectionCheckTick[iPortNum] = ::GetTickCount();
		m_iRecvCP[iPortNum] = 0;
	}

	m_mapBuyProductReq.clear();
	m_mapGiftItemReq.clear();
	*/
}

KNaunBillingManager::~KNaunBillingManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KNaunBillingManager::RegToLua()
{
	lua_tinker::class_add<KNaunBillingManager>( g_pLua, "KNaunBillingManager" );
	//lua_tinker::class_def<KNaunBillingManager>( g_pLua, "dump",							&KNaunBillingManager::Dump );
	//lua_tinker::class_def<KNaunBillingManager>( g_pLua, "InitNaunBillingAddress",	&KNaunBillingManager::InitNaunBillingAddress );
	lua_tinker::class_def<KNaunBillingManager>( g_pLua, "SetNumThread",					&KNaunBillingManager::Init );
	//	lua_tinker::class_def<KNaunBillingManager>( g_pLua, "InitNaunBillingValue",		&KNaunBillingManager::InitNaunBillingValue );

	lua_tinker::decl( g_pLua, "NaunBillingManager", this );
}

/*
void KNaunBillingManager::InitNaunBillingValue( const char* szServiceCode, const char* szServiceRegion )
{
	m_kGASHAuthInfo.m_strServiceCode = szServiceCode;
	m_kGASHAuthInfo.m_strServiceRegion = szServiceRegion;

	return;
}
*/

/*
void KNaunBillingManager::InitNaunBillingAddress( const char* szIPGServerIP, unsigned short  usAuthorizationPort, unsigned short  usAccountingPort )
{
	m_kNaunBillingInfo.m_strIP = szIPGServerIP;
	m_kNaunBillingInfo.m_usNaunPort[NP_AUTHORIZATION] = usAuthorizationPort;
	m_kNaunBillingInfo.m_usNaunPort[NP_ACCOUNTING] = usAccountingPort;

	return;
}
*/
void KNaunBillingManager::Init( int nThreadNum )
{
/*
	for( int iPortNum = 0; iPortNum < NP_NUM; iPortNum++ )
	{
		m_iRecvCP[iPortNum] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
	m_spThreadRecv[NP_AUTHORIZATION] = boost::shared_ptr< KTThread< KNaunBillingManager > >
		( new KTThread< KNaunBillingManager >( *this, KNaunBillingManager::RecvFromAuthorization, 50 ) );

	m_spThreadSend[NP_AUTHORIZATION] = boost::shared_ptr< KTThread< KNaunBillingManager > >
		( new KTThread< KNaunBillingManager >( *this, KNaunBillingManager::SendToAuthorization, 100 ) );

	m_spThreadRecv[NP_ACCOUNTING] = boost::shared_ptr< KTThread< KNaunBillingManager > >
		( new KTThread< KNaunBillingManager >( *this, KNaunBillingManager::RecvFromAccounting, 50 ) );

	m_spThreadSend[NP_ACCOUNTING] = boost::shared_ptr< KTThread< KNaunBillingManager > >
		( new KTThread< KNaunBillingManager >( *this, KNaunBillingManager::SendToAccounting, 100 ) );
*/
	KThreadManager::Init( nThreadNum );

	START_LOG(cout, L"NaunBilling TCP Thread 생성 했다!!")
		<< BUILD_LOG( nThreadNum )
		<< END_LOG;
}

KThread* KNaunBillingManager::CreateThread()
{
	return new KNaunBilling;
}

void KNaunBillingManager::BeginThread()
{
/*
	for( int iPortNum = 0; iPortNum < NP_NUM; iPortNum++ )
	{	
		if( m_spThreadRecv[iPortNum] )
		{
			m_spThreadRecv[iPortNum]->Begin();
		}

		if( m_spThreadSend[iPortNum] )
		{
			m_spThreadSend[iPortNum]->Begin();
		}
	}
*/
	KThreadManager::BeginThread();        
/*
	for( int iPortNum = 0; iPortNum < NP_NUM; iPortNum++ )
	{	
		if( !Connect( iPortNum ) )
		{
			START_LOG( cerr, L"NaunBilling 서버 접속 실패." )
				<< END_LOG;
		}
	}
*/
}

void KNaunBillingManager::EndThread()
{
	KThreadManager::EndThread();
/*
	for( int iPortNum = 0; iPortNum < NP_NUM; iPortNum++ )
	{	
		if( m_spThreadSend[iPortNum] )
		{
			m_spThreadSend[iPortNum]->End( 10000 );
		}

		if( m_spThreadRecv[iPortNum] )
		{
			m_spThreadRecv[iPortNum]->End( 3000 );
		}
	}
*/
}

void KNaunBillingManager::Tick()
{
}

/*
void KNaunBillingManager::QueueingSendPacket( int iPortSelect, const boost::shared_ptr< KNaunBillingPacket >& spPacket )
{
	JIF( spPacket );
	{
		KLocker lock( m_csSendQueue[iPortSelect] );

		m_kSendQueue[iPortSelect].push( spPacket );
	}
}

void KNaunBillingManager::SendTo( int iPortEnum )
{
	JIF( iPortEnum >= 0 && iPortEnum < NP_NUM );

	if( !IsConnected(iPortEnum) )
	{
		return;
	}

	int ret;
	char buf[MAX_PACKET_SIZE_NUBT];
	boost::shared_ptr< KNaunBillingPacket > spPacket;

	while( GetSendPacket( iPortEnum, spPacket ) )
	{
		if( !spPacket )
		{
			START_LOG( cerr, L"포인터 이상." )
				<< BUILD_LOG( m_kSendQueue[iPortEnum].size() )
				<< END_LOG;

			continue;
		}

		if( sizeof(spPacket) > MAX_PACKET_SIZE_NUBT )
		{
			START_LOG( cerr, L"패킷 사이즈 이상." )
				<< BUILD_LOG( sizeof(spPacket) )
				<< END_LOG;

			continue;
		}

		spPacket->WriteToBuffer( ( BYTE* )buf );

		char* cCurBufPos = buf;
		int iRemainedSendSize = sizeof(spPacket);
		while( iRemainedSendSize > 0 )
		{
			ret = ::send( m_sock[iPortEnum],
				cCurBufPos,
				iRemainedSendSize,
				0 );

			if( ret == SOCKET_ERROR )
			{
				START_LOG( cwarn, GET_WSA_MSG );
				CLOSE_SOCKET( m_sock[iPortEnum] );
				return;
			}

			iRemainedSendSize -= ret;
			cCurBufPos += ret;
		}
	}
}

void KNaunBillingManager::RecvFrom(int iPortEnum)
{
	JIF( iPortEnum >= 0 && iPortEnum < NP_NUM );

	if( !IsConnected(iPortEnum) )
	{
		return;
	}

	int ret;
	ret = ::recv( m_sock[iPortEnum],
		m_cRecvBuffer[iPortEnum] + m_iRecvCP[iPortEnum],
		MAX_PACKET_SIZE_NUBT - m_iRecvCP[iPortEnum],
		0 );

	START_LOG( clog, L"패킷 받음." )
		<< BUILD_LOG( ret );

	if( ret == SOCKET_ERROR )
	{
		START_LOG( cerr, GET_WSA_MSG );
		CLOSE_SOCKET( m_sock[iPortEnum] );
		return;
	}

	if( ret == 0 )
	{
		//START_LOG( cerr, L"소켓 연결이 끊어짐." )
		//	<< END_LOG;

		return;
	}

	if( ret > MAX_PACKET_SIZE_NUBT - m_iRecvCP[iPortEnum] )
	{
		START_LOG( cerr, L"전송된 크기가 너무 크다." )
			<< BUILD_LOG( ret )
			<< BUILD_LOG( MAX_PACKET_SIZE_NUBT )
			<< BUILD_LOG( m_iRecvCP[iPortEnum] )
			<< END_LOG;

		CLOSE_SOCKET( m_sock[iPortEnum] );
		return;
	}
	m_iRecvCP[iPortEnum] += ret;

	while( m_iRecvCP[iPortEnum] > 0 )
	{
		// 온전한 하나의 패킷 사이즈(MAX_PACKET_SIZE_NUBT)가 갖추어지기 전까지 처리하지 않는다.
		//if( MAX_PACKET_SIZE_NUBT > m_iRecvCP[iPortEnum] )
		//{
		//	return;
		//}

		MakeEventFromReceivedPacket( iPortEnum );

		if( m_iRecvCP[iPortEnum] > MAX_PACKET_SIZE_NUBT )
		{
			::memmove( m_cRecvBuffer[iPortEnum], m_cRecvBuffer[iPortEnum] + MAX_PACKET_SIZE_NUBT, m_iRecvCP[iPortEnum] - MAX_PACKET_SIZE_NUBT );
		}

		m_iRecvCP[iPortEnum] -= ret;
	}	
}

void KNaunBillingManager::RecvFromAuthorization( )
{
	RecvFrom(NP_AUTHORIZATION);
}

void KNaunBillingManager::SendToAuthorization( )
{
	SendTo(NP_AUTHORIZATION);
}

void KNaunBillingManager::RecvFromAccounting( )
{
	RecvFrom(NP_ACCOUNTING);
}

void KNaunBillingManager::SendToAccounting( )
{
	SendTo(NP_ACCOUNTING);
}

bool KNaunBillingManager::Connect( int iPortSelect )
{
	_JIF( iPortSelect >= 0 && iPortSelect < NP_NUM, return false );

	m_iRecvCP[iPortSelect] = 0;
	//m_sock[iPortSelect] = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성
	SOCKET sock = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성

	if( INVALID_SOCKET == sock )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;

		return false;
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr( m_kNaunBillingInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kNaunBillingInfo.m_usNaunPort[iPortSelect] );

	int ret = ::connect( sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< BUILD_LOG( m_kNaunBillingInfo.m_strIP )
			<< BUILD_LOG( m_kNaunBillingInfo.m_usNaunPort[iPortSelect] )
			<< END_LOG;

		CLOSE_SOCKET( sock );
		return false;
	}		

	m_sock[iPortSelect] = sock;

	return true;
}

bool KNaunBillingManager::IsConnected( int iPortSelect )
{
	_JIF( iPortSelect >= 0 && iPortSelect < NP_NUM, return false );

	return ( m_sock[iPortSelect] != INVALID_SOCKET );
}

void KNaunBillingManager::CheckConnection()
{
	for( int iPortNum = 0; iPortNum < NP_NUM; iPortNum++ )
	{
		if( ::GetTickCount() - m_dwLastConnectionCheckTick[iPortNum] < ms_dwConnectionCheckGap[iPortNum] )
		{
			continue;
		}

		m_dwLastConnectionCheckTick[iPortNum] = ::GetTickCount();

		if( !IsConnected(iPortNum) )
		{
			Connect(iPortNum);
		}
	}
}

bool KNaunBillingManager::GetSendPacket( int iPortSelect,  boost::shared_ptr< KNaunBillingPacket >& spPacket )
{
	_JIF( iPortSelect >= 0 && iPortSelect < NP_NUM, return false );

	KLocker lock( m_csSendQueue[iPortSelect] );

	if( m_kSendQueue[iPortSelect].empty() )
	{
		return false;
	}

	spPacket = m_kSendQueue[iPortSelect].front();
	m_kSendQueue[iPortSelect].pop();

	return true;
}

void KNaunBillingManager::MakeEventFromReceivedPacket( int iPortEnum )
{
	JIF( iPortEnum >= 0 && iPortEnum < NP_NUM );

	KNaunBillingPacket kPacket;
	if( !kPacket.ReadFromBuffer( ( BYTE* )m_cRecvBuffer[iPortEnum] ) )
	{
		START_LOG( cerr, L"버퍼에서 읽기 실패." )
			<< END_LOG;

		return;
	}	

	KEventPtr spEvent( new KEvent );
	KNaunGetCashRequestData kPacketGetCashRequestData;
	KNaunBuyProductRequestData kPacketBuyProductData;

	switch(iPortEnum)
	{
	case NP_AUTHORIZATION:
		kPacket.Read( kPacketGetCashRequestData );

		spEvent->SetData( PI_GS_PUBLISHER_BILLING, NULL, ENAUN_BILLING_BALANCE_ACK, kPacketGetCashRequestData );
		QueueingEvent( spEvent );
		break;
	case NP_ACCOUNTING:
		kPacket.Read( kPacketBuyProductData );

		spEvent->SetData(PI_GS_PUBLISHER_BILLING, NULL, ENAUN_BUY_PRODUCT_ACK, kPacketBuyProductData );
		QueueingEvent( spEvent );
		break;
	default:
		START_LOG( cerr, L"포트 종류 이상." )
			<< BUILD_LOG( iPortEnum )
			<< END_LOG;
		break;
	}
}

bool KNaunBillingManager::InsertBuyPacket( IN UidType iSenderUID, IN KEBILL_BUY_PRODUCT_REQ packet)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		// 같은 유저의 구매 정보가 이미 있다. 이것이 처리 될 때 까지 대기하자
		if( m_mapBuyProductReq.find( iSenderUID ) != m_mapBuyProductReq.end() )
		{
			START_LOG(cerr, L"NaunBilling 구매 오류 : 구매 도중 유저의 구매 요청 패킷이 이미 보관 중!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = false;
		}
		else if( m_mapGiftItemReq.find( iSenderUID ) != m_mapGiftItemReq.end() )
		{
			START_LOG(cerr, L"NaunBilling 구매 오류 : 구매 도중 유저의 선물 구매 요청 패킷이 이미 보관 중!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = false;
		}
		else
		{
			START_LOG(clog, L"NaunBilling 구매 : 패킷 보관 성공")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			m_mapBuyProductReq.insert( std::make_pair( iSenderUID, packet ) );

			ret = true;
		}
	}

	return ret;
}

bool KNaunBillingManager::GetBuyPacket( IN UidType iSenderUID, OUT KEBILL_BUY_PRODUCT_REQ &packet)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		std::map< UidType, KEBILL_BUY_PRODUCT_REQ >::iterator mit = m_mapBuyProductReq.find( iSenderUID );

		if( mit == m_mapBuyProductReq.end() )
		{
			START_LOG(cerr, L"NaunBilling 구매 오류 : 보관중인 구매 패킷이 없음!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = false;
		}
		else
		{
			START_LOG(clog, L"NaunBilling 구매 : 보관중인 구매 패킷 가져오기 성공")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			packet = static_cast<KEBILL_BUY_PRODUCT_REQ>( mit->second );
			m_mapBuyProductReq.erase( mit );

			ret = true;
		}
	}

	return ret;
}

bool KNaunBillingManager::InsertGiftPacket( IN UidType iSenderUID, IN KEBILL_GIFT_ITEM_REQ packet)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		// 같은 유저의 구매 정보가 이미 있다. 이것이 처리 될 때 까지 대기하자
		if( m_mapGiftItemReq.find(iSenderUID) != m_mapGiftItemReq.end() )
		{
			START_LOG(cerr, L"NaunBilling 선물하기 오류 : 선물 구매 도중 유저의 선물 구매 요청 패킷이 이미 보관 중!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = false;
		}
		else if( m_mapBuyProductReq.find(iSenderUID) != m_mapBuyProductReq.end() )
		{
			START_LOG(cerr, L"NaunBilling 선물하기 오류 : 선물 구매 도중 유저의 구매 요청 패킷이 이미 보관 중!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = false;
		}
		else
		{
			START_LOG(clog, L"NaunBilling 선물하기 : 패킷 보관 성공")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			m_mapGiftItemReq.insert(std::make_pair(iSenderUID, packet));

			ret = true;
		}
	}

	return ret;
}

bool KNaunBillingManager::GetGiftPacket( IN UidType iSenderUID, OUT KEBILL_GIFT_ITEM_REQ &packet)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		std::map<UidType, KEBILL_GIFT_ITEM_REQ>::iterator mit = m_mapGiftItemReq.find( iSenderUID );
		
		if( mit == m_mapGiftItemReq.end() )
		{
			START_LOG(cerr, L"NaunBilling 선물하기 오류 : 보관중인 선물하기 패킷이 없음!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = false;
		}
		else
		{
			START_LOG(clog, L"NaunBilling 선물하기 : 보관중인 선물하기 패킷 가져오기 성공")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			packet = static_cast<KEBILL_GIFT_ITEM_REQ>(mit->second);
			m_mapGiftItemReq.erase( mit );

			ret = true;
		}
	}

	return ret;
}

bool KNaunBillingManager::IsHaveBuyPacket( IN UidType iSenderUID )
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		std::map<UidType, KEBILL_BUY_PRODUCT_REQ>::iterator mit = m_mapBuyProductReq.find( iSenderUID );
		
		if( mit == m_mapBuyProductReq.end() )
		{
			START_LOG(clog, L"NaunBilling 구매 : 보관 중인 구매 패킷이 없음!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = false;
		}
		else
		{
			START_LOG(clog, L"NaunBilling 구매 : 보관 중인 구매 패킷이 있음!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = true;
		}
	}	

	return ret;
}

bool KNaunBillingManager::IsHaveGiftPacket( IN UidType iSenderUID )
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		std::map<UidType, KEBILL_GIFT_ITEM_REQ>::iterator mit =  m_mapGiftItemReq.find( iSenderUID );
		
		if( mit == m_mapGiftItemReq.end() )
		{
			START_LOG(clog, L"NaunBilling 선물하기 : 보관 중인 선물하기 패킷이 없음!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = false;
		}
		else
		{
			START_LOG(clog, L"NaunBilling 선물하기 : 보관 중인 선물하기 패킷이 있음!")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;

			ret = true;
		}
	}

	return ret;
}
*/
#endif SERV_COUNTRY_IN