#include <WinSock2.h>
#include "GashBillingTCPManager.h"

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "GashBillingTCP.h"

#ifdef SERV_COUNTRY_TWHK

//{{ 2011.7.12 지헌 : Gash 포인트 통합
#ifdef SERV_BILLING_ADD_GASH_POINT

DWORD KGASHBillingTCPManager::ms_dwConnectionCheckGap[] = { 15 * 1000,  15 * 1000  };

ImplToStringW( KGASHBillingTCPManager )
{
	return stm_;
}

ImplPfID( KGASHBillingTCPManager, PI_GS_PUBLISHER_BILLING );

ImplementSingleton( KGASHBillingTCPManager );

KGASHBillingTCPManager::KGASHBillingTCPManager()
{
	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{
		m_dwLastConnectionCheckTick[iPortNum] = ::GetTickCount();
		m_iRecvCP[iPortNum] = 0;
	}

	m_mapBuyProductReq.clear();
	m_mapGiftItemReq.clear();
	m_mapHaveGashPoint.clear();
}

KGASHBillingTCPManager::~KGASHBillingTCPManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KGASHBillingTCPManager::RegToLua()
{
	lua_tinker::class_add<KGASHBillingTCPManager>( g_pLua, "KGASHBillingTCPManager" );
	lua_tinker::class_def<KGASHBillingTCPManager>( g_pLua, "dump",                    &KGASHBillingTCPManager::Dump );
	lua_tinker::class_def<KGASHBillingTCPManager>( g_pLua, "InitGASHBillingTCPValue",       &KGASHBillingTCPManager::InitGASHBillingTCPValue );
	lua_tinker::class_def<KGASHBillingTCPManager>( g_pLua, "InitGASHBillingTCPAddress",     &KGASHBillingTCPManager::InitGASHBillingTCPAddress );
	lua_tinker::class_def<KGASHBillingTCPManager>( g_pLua, "SetNumThread",            &KGASHBillingTCPManager::Init );

	lua_tinker::decl( g_pLua, "GASHBillingTCPManager", this );
}

void KGASHBillingTCPManager::InitGASHBillingTCPValue( const char* szServiceCode, const char* szServiceRegion )
{
	m_kGASHAuthInfo.m_strServiceCode = szServiceCode;
	m_kGASHAuthInfo.m_strServiceRegion = szServiceRegion;

	return;
}

void KGASHBillingTCPManager::InitGASHBillingTCPAddress( const char* szGconsoleIP, unsigned short  usAuthorizationPort, unsigned short  usAccountingPort )
{
	m_kGASHAuthInfo.m_strIP = szGconsoleIP;
	m_kGASHAuthInfo.m_usGash_Port[GP_AUTHORIZATION]	 = usAuthorizationPort;
	m_kGASHAuthInfo.m_usGash_Port[GP_ACCOUNTING]	 = usAccountingPort;

	return;
}

void KGASHBillingTCPManager::Init( int nThreadNum )
{

	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{
		m_iRecvCP[iPortNum] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
	m_spThreadRecv[GP_AUTHORIZATION] = boost::shared_ptr< KTThread< KGASHBillingTCPManager > >
		( new KTThread< KGASHBillingTCPManager >( *this, KGASHBillingTCPManager::RecvFromAuthorization, 50 ) );

	m_spThreadSend[GP_AUTHORIZATION] = boost::shared_ptr< KTThread< KGASHBillingTCPManager > >
		( new KTThread< KGASHBillingTCPManager >( *this, KGASHBillingTCPManager::SendToAuthorization, 100 ) );

	m_spThreadRecv[GP_ACCOUNTING] = boost::shared_ptr< KTThread< KGASHBillingTCPManager > >
		( new KTThread< KGASHBillingTCPManager >( *this, KGASHBillingTCPManager::RecvFromAccounting, 50 ) );

	m_spThreadSend[GP_ACCOUNTING] = boost::shared_ptr< KTThread< KGASHBillingTCPManager > >
		( new KTThread< KGASHBillingTCPManager >( *this, KGASHBillingTCPManager::SendToAccounting, 100 ) );

	KThreadManager::Init( nThreadNum );

	START_LOG(cout, L"GASH TCP Thread 생성 했다!!")
		<< BUILD_LOG( nThreadNum )
		<< END_LOG;
}

KThread* KGASHBillingTCPManager::CreateThread()
{
	return new KGASHBillingTCP;
}

void KGASHBillingTCPManager::BeginThread()
{
	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
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

	KThreadManager::BeginThread();        

	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{	
		if( !Connect( iPortNum ) )
		{
			START_LOG( cerr, L"GASH 빌링 서버 접속 실패." )
				<< END_LOG;
		}
	}
}

void KGASHBillingTCPManager::EndThread()
{
	KThreadManager::EndThread();

	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
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
}

void KGASHBillingTCPManager::Tick()
{
	CheckConnection();
}

void KGASHBillingTCPManager::QueueingSendPacket( int iPortSelect, const boost::shared_ptr< std::string >& spPacket )
{
	JIF( spPacket );
	{
		KLocker lock( m_csSendQueue[iPortSelect] );

		m_kSendQueue[iPortSelect].push( spPacket );
	}
}

void KGASHBillingTCPManager::SendTo( int iPortEnum )
{
	JIF( iPortEnum >= 0 && iPortEnum < GP_NUM );

	if( !IsConnected(iPortEnum) )
	{
		return;
	}

	int ret;
	char buf[MAX_PACKET_SIZE_OF_GASH_AUTH];
	boost::shared_ptr< std::string > spPacket;

	while( GetSendPacket( iPortEnum, spPacket ) )
	{
		if( !spPacket )
		{
			START_LOG( cerr, L"포인터 이상." )
				<< BUILD_LOG( m_kSendQueue[iPortEnum].size() )
				<< END_LOG;

			continue;
		}

		START_LOG( clog, L"보내는 패킷" )
			<< BUILD_LOG( spPacket->c_str() )
			<< END_LOG;

		if( spPacket->size() > MAX_PACKET_SIZE_OF_GASH_AUTH )
		{
			START_LOG( cerr, L"패킷 사이즈 이상." )
				<< BUILD_LOG( spPacket->size() )
				<< END_LOG;

			continue;
		}

		::memcpy( buf, spPacket->c_str(), spPacket->size() );

		char* cCurBufPos = buf;
		int iRemainedSendSize = spPacket->size();
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

void KGASHBillingTCPManager::RecvFrom(int iPortEnum)
{
	JIF( iPortEnum >= 0 && iPortEnum < GP_NUM );

	if( !IsConnected(iPortEnum) )
	{
		return;
	}

	int ret;
	ret = ::recv( m_sock[iPortEnum],
		m_cRecvBuffer[iPortEnum] + m_iRecvCP[iPortEnum],
		MAX_PACKET_SIZE_OF_GASH_AUTH - m_iRecvCP[iPortEnum],
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
		START_LOG( cerr, L"소켓 연결이 끊어짐." )
			<< END_LOG;

		CLOSE_SOCKET( m_sock[iPortEnum] );
		return;
	}

	if( ret > MAX_PACKET_SIZE_OF_GASH_AUTH - m_iRecvCP[iPortEnum] )
	{
		START_LOG( cerr, L"전송된 크기가 너무 크다." )
			<< BUILD_LOG( ret )
			<< BUILD_LOG( MAX_PACKET_SIZE_OF_GASH_AUTH )
			<< BUILD_LOG( m_iRecvCP[iPortEnum] )
			<< END_LOG;

		CLOSE_SOCKET( m_sock[iPortEnum] );
		return;
	}
	m_iRecvCP[iPortEnum] += ret;

	//////
	std::string strRecv;
	for( int i = 0; i < m_iRecvCP[iPortEnum]; i++ )
	{
		strRecv.push_back( m_cRecvBuffer[iPortEnum][i] );
	}
	strRecv.push_back( '\0' );
	START_LOG( clog2, L"받은 GASH 패킷 그냥" )
		<< BUILD_LOG( strRecv )
		<< END_LOG;
	//////

	int iIndex = 0;
	while( iIndex + 2 <= m_iRecvCP[iPortEnum] )
	{
		if( m_cRecvBuffer[iPortEnum][iIndex] == '\r' && m_cRecvBuffer[iPortEnum][iIndex + 1] == '\n' )
		{
			char szPacket[MAX_PACKET_SIZE_OF_GASH_AUTH + 1];
			::ZeroMemory( szPacket, MAX_PACKET_SIZE_OF_GASH_AUTH + 1 );
			::memcpy( szPacket, m_cRecvBuffer[iPortEnum], iIndex + 2 );
			std::string strPacket = szPacket;

			START_LOG( clog2, L"받은 GASH Billing 패킷" )
				<< BUILD_LOG( strPacket )
				<< END_LOG;

			MakeEventFromReceived(iPortEnum, strPacket);
			if( m_iRecvCP[iPortEnum] > iIndex + 2 )
			{
				::memmove( m_cRecvBuffer[iPortEnum], m_cRecvBuffer[iPortEnum] + iIndex + 2, m_iRecvCP[iPortEnum] - ( iIndex + 2 ) );
			}
			m_iRecvCP[iPortEnum] -= ( iIndex + 2 );
			iIndex = 0;
			continue;
		}

		iIndex++;
	}

}

void KGASHBillingTCPManager::RecvFromAuthorization( )
{
	RecvFrom(GP_AUTHORIZATION);
}

void KGASHBillingTCPManager::SendToAuthorization( )
{
	SendTo(GP_AUTHORIZATION);
}

void KGASHBillingTCPManager::RecvFromAccounting( )
{
	RecvFrom(GP_ACCOUNTING);
}

void KGASHBillingTCPManager::SendToAccounting( )
{
	SendTo(GP_ACCOUNTING);
}


bool KGASHBillingTCPManager::Connect( int iPortSelect )
{
	_JIF( iPortSelect >= 0 && iPortSelect < GP_NUM, return false );

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
	addr.sin_addr.s_addr	= inet_addr( m_kGASHAuthInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kGASHAuthInfo.m_usGash_Port[iPortSelect] );

	int ret = ::connect( sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< BUILD_LOG( m_kGASHAuthInfo.m_strIP )
			<< BUILD_LOG( m_kGASHAuthInfo.m_usGash_Port[iPortSelect] )
			<< END_LOG;

		CLOSE_SOCKET( sock );
		return false;
	}		

	m_sock[iPortSelect] = sock;

	return true;
}

bool KGASHBillingTCPManager::IsConnected( int iPortSelect )
{
	_JIF( iPortSelect >= 0 && iPortSelect < GP_NUM, return false );

	return ( m_sock[iPortSelect] != INVALID_SOCKET );
}

void KGASHBillingTCPManager::CheckConnection()
{
	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
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

bool KGASHBillingTCPManager::GetSendPacket( int iPortSelect,  boost::shared_ptr< std::string >& spPacket )
{
	_JIF( iPortSelect >= 0 && iPortSelect < GP_NUM, return false );

	KLocker lock( m_csSendQueue[iPortSelect] );

	if( m_kSendQueue[iPortSelect].empty() )
	{
		return false;
	}

	spPacket = m_kSendQueue[iPortSelect].front();
	m_kSendQueue[iPortSelect].pop();

	return true;
}

void KGASHBillingTCPManager::MakeEventFromReceived(int iPortEnum, std::string& strPacket)
{
	JIF( iPortEnum >= 0 && iPortEnum < GP_NUM );

	KGashResponseData kPacketAck;

	JIF( kPacketAck.ExtractFromGASHPacket( strPacket ) );

	KEventPtr spEvent( new KEvent );
	switch(iPortEnum)
	{
	case GP_AUTHORIZATION : 
		spEvent->SetData(PI_GS_PUBLISHER_BILLING, NULL, EGASH_GET_GASHPOINT_ACK, kPacketAck );
		break;
	case GP_ACCOUNTING : 
		// 지헌 : 한 포트로 여러 종류의 패킷이 온다면, 여기서 패킷 종류 구분해서 SetData 에 넘기자.
		spEvent->SetData(PI_GS_PUBLISHER_BILLING, NULL, EGASH_DECREASE_GASHPOINT_ACK, kPacketAck );
		break;
	default:
		START_LOG( cerr, L"포트 종류 이상." )
			<< BUILD_LOG( iPortEnum )
			<< END_LOG;
		break;
	}

	QueueingEvent( spEvent );
}

// Buy 패킷 보관, 가져오기
bool KGASHBillingTCPManager::InsertBuyPacket( IN UidType iSenderUID, IN KEBILL_BUY_PRODUCT_REQ packet)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		// 같은 유저의 구매 정보가 이미 있다. 이것이 처리 될 때 까지 대기하자
		if( m_mapBuyProductReq.find(iSenderUID) != m_mapBuyProductReq.end() )
		{
			START_LOG(cerr, L"GASH 구매 오류 : 구매중- 해당 유저의 구매 패킷이 이미 보관중이다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = false;
		}
		else if( m_mapGiftItemReq.find(iSenderUID) != m_mapGiftItemReq.end() )
		{
			START_LOG(cerr, L"GASH 구매 오류 : 구매중- 해당 유저의 선물 패킷이 이미 보관중이다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = false;
		}
		else// 동일 유저의 패킷 정보가 없다면 보관해두자
		{
			START_LOG(clog, L"GASH 구매 : 해당 유저의 구매 패킷을 보관한다")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			m_mapBuyProductReq.insert(std::make_pair(iSenderUID, packet));
			ret = true;
		}
	}

	return ret;
}
bool KGASHBillingTCPManager::GetBuyPacket( IN UidType iSenderUID, OUT KEBILL_BUY_PRODUCT_REQ &packet)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		std::map<UidType, KEBILL_BUY_PRODUCT_REQ>::iterator itor;
		itor = m_mapBuyProductReq.find(iSenderUID);
		// 유저의 구매 정보가 없다? 오류!
		if(itor == m_mapBuyProductReq.end())
		{
			START_LOG(cerr, L"GASH 구매 오류 : 해당 유저의 보관중인 구매 패킷 없다??")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = false;
		}
		else
		{
			START_LOG(clog, L"GASH 구매 : 해당 유저의 보관중인 구매 패킷을 가져왔다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			packet = (KEBILL_BUY_PRODUCT_REQ)itor->second;
			m_mapBuyProductReq.erase(itor);
			ret = true;
		}
	}

	return ret;
}

bool KGASHBillingTCPManager::InsertGiftPacket( IN UidType iSenderUID, IN KEBILL_GIFT_ITEM_REQ packet)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		// 같은 유저의 구매 정보가 이미 있다. 이것이 처리 될 때 까지 대기하자
		if( m_mapGiftItemReq.find(iSenderUID) != m_mapGiftItemReq.end() )
		{
			START_LOG(cerr, L"GASH 구매 오류 : 선물중- 해당 유저의 선물 패킷이 이미 보관중이다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = false;
		}
		else if( m_mapBuyProductReq.find(iSenderUID) != m_mapBuyProductReq.end() )
		{
			START_LOG(cerr, L"GASH 구매 오류 : 선물중- 해당 유저의 구매 패킷이 이미 보관중이다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = false;
		}
		else// 동일 유저의 패킷 정보가 없다면 보관해두자
		{
			START_LOG(clog, L"GASH 구매 : 선물중- 해당 유저의 선물 패킷을 보관한다")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			m_mapGiftItemReq.insert(std::make_pair(iSenderUID, packet));
			ret = true;
		}
	}

	return ret;
}
bool KGASHBillingTCPManager::GetGiftPacket( IN UidType iSenderUID, OUT KEBILL_GIFT_ITEM_REQ &packet)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		std::map<UidType, KEBILL_GIFT_ITEM_REQ>::iterator itor;
		itor = m_mapGiftItemReq.find(iSenderUID);
		// 유저의 구매 정보가 없다? 오류!
		if(itor == m_mapGiftItemReq.end())
		{
			START_LOG(cerr, L"GASH 구매 오류 : 해당 유저의 보관중인 선물 패킷 없다??")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = false;
		}
		else
		{
			START_LOG(clog, L"GASH 구매 : 해당 유저의 보관중인 선물 패킷을 가져왔다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			packet = (KEBILL_GIFT_ITEM_REQ)itor->second;
			m_mapGiftItemReq.erase(itor);
			ret = true;
		}
	}

	return ret;
}

// 구매 패킷을 이미 보관중인지 체크
bool KGASHBillingTCPManager::IsHaveBuyPacket(IN UidType iSenderUID)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		std::map<UidType, KEBILL_BUY_PRODUCT_REQ>::iterator itor;
		itor = m_mapBuyProductReq.find(iSenderUID);
		// 유저의 선물 정보가 없다?
		if(itor == m_mapBuyProductReq.end())
		{
			START_LOG(clog, L"GASH 구매 : 해당 유저의 구매패킷은 없다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = false;
		}
		else
		{
			START_LOG(clog, L"GASH 구매 : 해당 유저의 구매패킷이 있다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = true;
		}
	}	

	return ret;
}

// 구매 패킷을 이미 보관중인지 체크
bool KGASHBillingTCPManager::IsHaveGiftPacket(IN UidType iSenderUID)
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csBuyGiftPacket );

		std::map<UidType, KEBILL_GIFT_ITEM_REQ>::iterator itor;
		itor = m_mapGiftItemReq.find(iSenderUID);
		// 유저의 선물 정보가 없다?
		if(itor == m_mapGiftItemReq.end())
		{
			START_LOG(clog, L"GASH 구매 : 해당 유저의 선물패킷은 없다")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = false;
		}
		else
		{
			START_LOG(clog, L"GASH 구매 : 해당 유저의 선물패킷이 있다.")
				<< BUILD_LOG( iSenderUID )
				<< END_LOG;
			ret = true;
		}
	}

	return ret;
}

// 로그 남기기 위해서 잔액 조회 할 때마다 그 액수를 보관하고, 가져올 수 있도록 했다.
bool KGASHBillingTCPManager::InsertHaveGashPoint( IN UidType iUserUID, IN int iGashPoint )
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csHaveGashPoint );

		std::map<UidType,  int>::iterator itor = m_mapHaveGashPoint.find(iUserUID);
		if(itor != m_mapHaveGashPoint.end())
		{
			START_LOG(clog, L"GASH 구매 로그 : 해당 유저의 잔액 정보를 갱신한다")
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( (unsigned long)itor->second )
				<< BUILD_LOG( iGashPoint )
				<< END_LOG;
			itor->second = iGashPoint;
			ret = true;
		}
		else
		{
			m_mapHaveGashPoint.insert(std::make_pair(iUserUID, iGashPoint));
			START_LOG(clog, L"GASH 구매 로그 : 해당 유저의 잔액 정보를 추가한다")
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( iGashPoint )
				<< END_LOG;
			ret = true;
		}
	}

	return ret;
}
bool KGASHBillingTCPManager::GetHaveGashPoint( IN UidType iUserUID, OUT int &iGashPoint )
{
	bool ret = false;

	//Lock Area
	{
		KLocker lock( m_csHaveGashPoint );

		std::map<UidType,  int>::iterator itor = m_mapHaveGashPoint.find(iUserUID);
		if(itor != m_mapHaveGashPoint.end())
		{
			iGashPoint = itor->second;
			ret = true;
		}
		else
		{
			START_LOG(cerr, L"GASH 구매 로그 에러 : 해당 유저의 잔액 정보가 없다??")
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( iGashPoint )
				<< END_LOG;

			iGashPoint = -1;

			ret = false;
		}
	}

	return ret;
}
#endif
//}}

#endif SERV_COUNTRY_TWHK