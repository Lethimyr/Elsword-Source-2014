#include <WinSock2.h>
#include "NexonBillingTCPManager.h"

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "NexonBillingTCP.h"
#include "BaseServer.h"
#include "SimLayer.h"

#include "X2Data/XSLItem.h"
#include "X2data/XSLCashItemManager.h"

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

#ifdef SERV_EVENT_BINGO// 작업날짜: 2013-09-09	// 박세훈
	#include "..\Common\Temp.h"
#endif // SERV_EVENT_BINGO

//{{ 2013. 01. 10	최육사	빌링 TCP 패킷 크기 예외처리
#ifdef SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
static const unsigned long VARIABLE_BUFFER_MAX_SIZE = 32768;		// 32kb
#endif SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
//}}

DWORD KNexonBillingTCPManager::ms_dwHeartBeatGap( 1 * 60 * 1000 );
DWORD KNexonBillingTCPManager::ms_dwConnectionCheckGap( 20 * 1000 );

ImplPfID( KNexonBillingTCPManager, PI_GS_NX_BILLING_TCP );

ImplementSingleton( KNexonBillingTCPManager );

KNexonBillingTCPManager::KNexonBillingTCPManager()
{
	m_kNexonBillingTCPInfo.m_strIP.clear();
	m_kNexonBillingTCPInfo.m_usPort = 0;
    m_kNexonBillingTCPInfo.m_iDomain = 0;
	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	m_sock = INVALID_SOCKET;
	m_bFirstConnectSucc = false;
	//}}
	m_dwLastHeartBeatTick = ::GetTickCount();
	m_dwLastConnectionCheckTick = ::GetTickCount();
    m_ulRecvCP = 0;
	KCSLOCK_SET_VALUE( m_iReleaseTick, 0 );
	KCSLOCK_SET_VALUE( m_ulCurrentPacketNo, 0 );
	KCSLOCK_SET_VALUE( m_ulCurrentPurchaseNo, 0 );
	KCSLOCK_SET_VALUE( m_ulCurrentProductPage, 0 );
    m_spEvent.reset( CreateEvent( NULL, false, false, NULL ), CloseHandle );
	m_iBillingServerNo = 0;
	//{{ 2010. 11. 22	최육사	청약 철회
#ifdef SERV_NX_BILLING_REFUND
	KCSLOCK_SET_VALUE( m_iCashItemRefundRequestID, 0 );
#endif SERV_NX_BILLING_REFUND
	//}}
}

KNexonBillingTCPManager::~KNexonBillingTCPManager()
{
}

ImplToStringW( KNexonBillingTCPManager )
{
	KLocker lock( m_csProductInfo );

	stm_	<< L"----------[ Nexon Billing TCP Manager ]----------" << std::endl
		<< L"IP Addr : " << KncUtil::toWideString( m_kNexonBillingTCPInfo.m_strIP ) << std::endl
		<< L"Port : " << m_kNexonBillingTCPInfo.m_usPort << std::endl
		<< L"Domain : " << m_kNexonBillingTCPInfo.m_iDomain << std::endl
		<< L"Product Info : " << m_mapProductInfo.size() << std::endl
		;

	//{{ 2013. 01. 23	최육사	빌링 패킷 수신 처리 성능 측정
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
	stm_	<< L"----------[ Nexon Billing Recv Performance Dump ]----------" << std::endl
		<< TOSTRINGW( m_kRecvPerformanceDump.m_dwMinLatency )
		<< TOSTRINGW( m_kRecvPerformanceDump.m_wstrMinRegDate )
		<< TOSTRINGW( m_kRecvPerformanceDump.m_dwMaxLatency )
		<< TOSTRINGW( m_kRecvPerformanceDump.m_wstrMaxRegDate )
		;
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
	//}}

	return stm_;
}

#include <KncLua.h>
#include <lua_tinker.h>

void KNexonBillingTCPManager::RegToLua()
{
	lua_tinker::class_add<KNexonBillingTCPManager>( g_pLua, "KNexonBillingTCPManager" );
	lua_tinker::class_def<KNexonBillingTCPManager>( g_pLua, "dump",						&KNexonBillingTCPManager::Dump );
	lua_tinker::class_def<KNexonBillingTCPManager>( g_pLua, "InitNexonBillingTCPInfo",	&KNexonBillingTCPManager::InitNexonBillingTCPInfo );
	lua_tinker::class_def<KNexonBillingTCPManager>( g_pLua, "SetNumThread",				&KNexonBillingTCPManager::Init );

	lua_tinker::decl( g_pLua, "NXBillingTCPManager", this );
}

void KNexonBillingTCPManager::InitNexonBillingTCPInfo( const char* szNexonAuthIP, unsigned short usPort, int iDomain )
{
	m_kNexonBillingTCPInfo.m_strIP		= szNexonAuthIP;
	m_kNexonBillingTCPInfo.m_usPort		= usPort;
    m_kNexonBillingTCPInfo.m_iDomain	= iDomain;
#ifdef SERV_EVENT_BINGO// 작업날짜: 2013-09-09	// 박세훈
	SiKGSBingoEventInfo()->SetBillingIP( m_kNexonBillingTCPInfo.m_strIP );
#endif // SERV_EVENT_BINGO
}

void KNexonBillingTCPManager::Init( int nThreadNum )
{
    m_ulRecvCP = 0;
	//{{ 2013. 01. 10	최육사	빌링 TCP 패킷 크기 예외처리
#ifdef SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
	m_kRecvVariableBuffer.Reserve( VARIABLE_BUFFER_MAX_SIZE );
#endif SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
	//}}

    //////////////////////////////////////////////////////////////////////////
    // thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
#ifdef _CONVERT_VS_2010
    m_spThreadRecv = boost::shared_ptr< KTThread< KNexonBillingTCPManager > >
	( new KTThread< KNexonBillingTCPManager >( *this, &KNexonBillingTCPManager::Recv, 50 ) );

    m_spThreadSend = boost::shared_ptr< KTThread< KNexonBillingTCPManager > >
        ( new KTThread< KNexonBillingTCPManager >( *this, &KNexonBillingTCPManager::Send, 100 ) );

	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	m_spThreadKeepConnect = boost::shared_ptr< KTThread< KNexonBillingTCPManager > >
		( new KTThread< KNexonBillingTCPManager >( *this, &KNexonBillingTCPManager::KeepConnectionThread, 1000 ) );
	//}}
#else
    m_spThreadRecv = boost::shared_ptr< KTThread< KNexonBillingTCPManager > >
        ( new KTThread< KNexonBillingTCPManager >( *this, KNexonBillingTCPManager::Recv, 50 ) );

    m_spThreadSend = boost::shared_ptr< KTThread< KNexonBillingTCPManager > >
        ( new KTThread< KNexonBillingTCPManager >( *this, KNexonBillingTCPManager::Send, 100 ) );

	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	m_spThreadKeepConnect = boost::shared_ptr< KTThread< KNexonBillingTCPManager > >
		( new KTThread< KNexonBillingTCPManager >( *this, KNexonBillingTCPManager::KeepConnectionThread, 1000 ) );
	//}}
#endif _CONVERT_VS_2010

	KThreadManager::Init( nThreadNum );
}

KThread* KNexonBillingTCPManager::CreateThread()
{
	return new KNexonBillingTCP;
}

void KNexonBillingTCPManager::BeginThread()
{
	//{{ 2011. 12. 07	최육사	빌링 스크립트 매니저
#ifdef SERV_BILLING_SCRIPT_MANAGER
	// 빌링 스레드 시작하기 전에 config파일 파싱을 하자!
	m_kBillingScriptManager.OpenScriptFile( "BillingConfig.lua" );
#endif SERV_BILLING_SCRIPT_MANAGER
	//}}

	// 빌링패킷 수신스레드
	if( m_spThreadRecv )
	{
		m_spThreadRecv->Begin();
	}

	// 빌링패킷 전송스레드
	if( m_spThreadSend )
	{
		m_spThreadSend->Begin();
	}

	// 빌링이벤트 핸들링스레드
	KThreadManager::BeginThread();

	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	// 빌링서버와의 하트비트 체크 및 접속 유지를 위한 접속유지스레드
	if( m_spThreadKeepConnect )
	{
		// Thread를 시작하면서 빌링서버에 접속시도한다.
		m_spThreadKeepConnect->Begin();
	}
	//}}
}

//{{ 2010. 8. 30	최육사	서버 정상 종료 처리
void KNexonBillingTCPManager::EndThread( DWORD dwTimeOut /*= 10000*/ )
{
	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	if( m_spThreadKeepConnect )
	{
		m_spThreadKeepConnect->End( 3000 );

		START_LOG( cout, L"넥슨 빌링 서버 접속 체크 스레드 종료!" );
	}
	//}}

	KThreadManager::EndThread();

    if( m_spThreadSend )
    {
	    m_spThreadSend->End( 10000 );

		START_LOG( cout, L"넥슨 빌링 서버 Send 스레드 종료!" );
    }

	// recv 스레드 죽이기 전에 소켓을 닫자!
	CLOSE_SOCKET( m_sock );
    
    if( m_spThreadRecv )
    {
	    m_spThreadRecv->End( 3000 );

		START_LOG( cout, L"넥슨 빌링 서버 Recv 스레드 종료!" );
    }
}
//}}

void KNexonBillingTCPManager::QueueingSendPacket( const KNexonBillingTCPPacketPtr& spPacket )
{
    JIF( spPacket );

	KCSLOCK_BEGIN( m_kSendQueue )
		m_kSendQueue.push( spPacket );
	KCSLOCK_END()
}

__int64 KNexonBillingTCPManager::GetReleaseTick()
{
	__int64 iReleaseTick = 0;
	KCSLOCK_BEGIN( m_iReleaseTick )
		iReleaseTick = m_iReleaseTick;
	KCSLOCK_END()

    return iReleaseTick;
}

bool KNexonBillingTCPManager::SetReleaseTick( const __int64& iReleaseTick )
{
    bool bRet;

	KCSLOCK_BEGIN( m_iReleaseTick )
		bRet = ( m_iReleaseTick == iReleaseTick );

		m_iReleaseTick = iReleaseTick;
	KCSLOCK_END()

    return bRet;
}

unsigned long KNexonBillingTCPManager::GetNextPacketNo()
{
	unsigned long ulCurrentPacketNo;

	KCSLOCK_BEGIN( m_ulCurrentPacketNo )
		m_ulCurrentPacketNo++;
		if( m_ulCurrentPacketNo > 0xfffffff0 )
		{
			m_ulCurrentPacketNo = 1;
		}
		ulCurrentPacketNo = m_ulCurrentPacketNo;
	KCSLOCK_END()

    return ulCurrentPacketNo;
}

unsigned long KNexonBillingTCPManager::GetNextPurchaseNo()
{
	unsigned long ulCurrentPurchaseNo;

	KCSLOCK_BEGIN( m_ulCurrentPurchaseNo )
		m_ulCurrentPurchaseNo++;
		if( m_ulCurrentPurchaseNo > 0xfffffff0 )
		{
			m_ulCurrentPurchaseNo = 1;
		}
		ulCurrentPurchaseNo = m_ulCurrentPurchaseNo;
	KCSLOCK_END()

    return ulCurrentPurchaseNo;
}

void KNexonBillingTCPManager::InsertPacketNoUserUID( unsigned long ulPacketNo, UidType iUserUID )
{
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"유저 UID 이상." )
            << BUILD_LOG( ulPacketNo )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return;
    }

	KCSLOCK_BEGIN( m_mapPacketNoUserUID )
		std::map< unsigned long, UidType >::iterator mit;
		mit = m_mapPacketNoUserUID.find( ulPacketNo );
		if( mit != m_mapPacketNoUserUID.end() )
		{
			START_LOG( cerr, L"패킷 번호에 해당하는 UserUID 가 이미 등록되어 있다." )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second )
				<< BUILD_LOG( ulPacketNo )
				<< BUILD_LOG( iUserUID )
				<< END_LOG;
		}

		// 없으면 삽입하고 있으면 덮어 쓴다.
		m_mapPacketNoUserUID[ulPacketNo] = iUserUID;
	KCSLOCK_END()
}

void KNexonBillingTCPManager::DeletePacketNoUserUID( unsigned long ulPacketNo )
{
	KCSLOCK_BEGIN( m_mapPacketNoUserUID )
		std::map< unsigned long, UidType >::iterator mit;
		mit = m_mapPacketNoUserUID.find( ulPacketNo );
		if( mit == m_mapPacketNoUserUID.end() )
		{
			START_LOG( cerr, L"패킷 번호에 해당하는 UserUID 가 등록되어 있지 않다." )
				<< BUILD_LOG( ulPacketNo )
				<< END_LOG;

			return;
		}

		m_mapPacketNoUserUID.erase( mit );
	KCSLOCK_END()
}

UidType KNexonBillingTCPManager::GetCorrespondingUserUID( unsigned long ulPacketNo )
{
	UidType iUserUID = 0;

	KCSLOCK_BEGIN( m_mapPacketNoUserUID )
		std::map< unsigned long, UidType >::iterator mit;
		mit = m_mapPacketNoUserUID.find( ulPacketNo );
		if( mit == m_mapPacketNoUserUID.end() )
		{
			START_LOG( cerr, L"패킷 번호에 해당하는 UserUID 가 등록되어 있지 않다." )
				<< BUILD_LOG( ulPacketNo )
				<< END_LOG;

			return 0;
		}
		iUserUID = mit->second;
	KCSLOCK_END()

    return iUserUID;
}

void KNexonBillingTCPManager::AddProductInfo( KNXBTProductInfo kInfo )
{
    KLocker lock( m_csProductInfo );

    std::map< unsigned long, KNXBTProductInfo >::iterator mit;
    mit = m_mapProductInfo.find( kInfo.m_ulProductNo );
    if( mit != m_mapProductInfo.end() )
    {
        START_LOG( cwarn, L"프로덕트 정보가 이미 존재함." )
            << BUILD_LOG( GetReleaseTick() )
            << BUILD_LOG( kInfo.m_ulProductNo )
            << BUILD_LOG( mit->second.m_wstrProductID )
            << BUILD_LOG( kInfo.m_wstrProductID )
            << END_LOG;
    }

	//////////////////////////////////////////////////////////////////////////
	// 소비 카테고리에 강제로 넣기
	const int iItemID = _wtoi( kInfo.m_wstrProductID.c_str() );

	//{{ 2009. 12. 19  최육사	서버군 확장
	if( SiCXSLCashItemManager()->IsSellItemInThisServer( iItemID ) == false )
	{
		START_LOG( cout, L"현재 서버군에서는 판매되서는 안되는 상품입니다. 리스트에 포함시키지 않습니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		//{{ 2012. 03. 21	김민성		캐쉬 상품 리스트 서버군 구별없이 DB에 기록
#ifdef SERV_CASH_ITEM_LIST_ADD
		// 제외된 캐쉬 상품을 따로 저장해 둔다
		m_mapProductInfo_Excepted[kInfo.m_ulProductNo] = kInfo;
#endif SERV_CASH_ITEM_LIST_ADD
		//}}

		return;
	}
	//}}


	//{{ 2011. 01. 04  김민성	product no 별 서버군 확장
#ifdef SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	if( SiCXSLCashItemManager()->IsSellItemInThisProductNo( kInfo.m_ulProductNo ) == false )
	{
		START_LOG( cout, L"현재 서버군에서는 판매되서는 안되는 상품입니다. 리스트에 포함시키지 않습니다." )
			<< BUILD_LOG( kInfo.m_ulProductNo )
			<< END_LOG;

		//{{ 2012. 03. 21	김민성		캐쉬 상품 리스트 서버군 구별없이 DB에 기록
#ifdef SERV_CASH_ITEM_LIST_ADD
		// 제외된 캐쉬 상품을 따로 저장해 둔다
		m_mapProductInfo_Excepted[kInfo.m_ulProductNo] = kInfo;
#endif SERV_CASH_ITEM_LIST_ADD
		//}}

		return;
	}
#endif SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	//}}

	//{{ 2011. 12. 07	최육사	빌링 스크립트 매니저
#ifdef SERV_BILLING_SCRIPT_MANAGER
	// 서비스 여부
	const bool bIsService = ( KSimLayer::GetKObj()->GetBillingFlag() == KSimLayer::BF_NEXON_KOREA );

	if( m_kBillingScriptManager.IsExistProductCategoryModifyInfoForItemID( iItemID ) == true )
	{
		if( m_kBillingScriptManager.GetModifiedProductCategoryForItemID( iItemID, bIsService, kInfo.m_ulCategoryNo ) == false )
		{
			START_LOG( cerr, L"상품 카테고리 정보 강제 변경 실패!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( bIsService )
				<< END_LOG;
		}
	}
#else
	// 전직 캐쉬템 and 강화 보조 아이템의 캐쉬샵 카테고리 변경
	//if( CXSLItem::IsJobChangeCashItem( iItemID ) == true  ||
	//	CXSLItem::IsEnchantSupportItem( iItemID ) == true )
	//{
	//	// 오픈 프리미엄 카테고리
	//	if( KSimLayer::GetKObj()->GetBillingFlag() == KSimLayer::BF_NEXON_KOREA )
	//	{
	//		kInfo.m_ulCategoryNo = 790;
	//	}
	//	else
	//	{
	//		kInfo.m_ulCategoryNo = 703;
	//	}
	//}

	//switch( iItemID )
	//{
	//case 500010: // 생명의 결정 (쁘띠 에인션트 뽀루)
	//case 500020: // 생명의 결정 (트리 나이트)
	//case 500060: // 쁘띠 에인션트 뽀루
	//case 500070: // 트리 나이트
	//case 500080: // 생명의 결정 (쁘띠 에인션트 뽀루) 특별 패키지
	//case 500090: // 생명의 결정 (트리 나이트) 특별 패키지
	//case 226860: // '생명의 결정' 코보 특가 상품권
	//case 500040: // 엘의 나무 열매
	//case 500130: // 할로위치 특별 패키지
	//case 500120: // 생명의 결정 (할로 위치) 특별 패키지
	//case 500100: // 생명의 결정 (할로 위치)
	//case 500140: // 생명의 결정 (루돌뽀)
	//case 500150: // 생명의 결정 (미스틱)
	//case 500190: // 생명의 결정 (미스틱) 특별 패키지
	//case 500200: // 생명의 결정 (미스틱 스페셜 에디션-블랙) 특별 패키지
	//case 500210: // 생명의 결정 (미스틱 스페셜 에디션-화이트) 특별 패키지
	//case 500220: // 생명의 결정 (미스틱 스페셜 에디션-레드) 특별 패키지 
	//case 500230: // 생명의 결정 (해츨링)
	//case 500240: // 생명의 결정 (해츨링-다크)
	//case 500250: // 생명의 결정 (해츨링-파이어)
	//case 500260: // 생명의 결정 (해츨링-라이트)
	//case 500270: // 생명의 결정 (해츨링) 패키지
	//case 500280: // 생명의 결정 (해츨링-다크) 패키지
	//case 500290: // 생명의 결정 (해츨링-파이어) 패키지
	//case 500300: // 생명의 결정 (해츨링-라이트) 패키지
	//case 500310: // 고스트 에인션트 뽀루-이블
	//case 500320: // 고스트 에인션트 뽀루-헬파이어
	//case 500330: // 고스트 에인션트 뽀루-할로우
	//case 500340: // 고스트 에인션트 뽀루-세라프
	//case 500350: // 할로윈 고스트 뽀루 패키지(이블)
	//case 500360: // 할로윈 고스트 뽀루 패키지(헬파이어)
	//case 500370: // 할로윈 고스트 뽀루 패키지(할로우)
	//case 500380: // 할로윈 고스트 뽀루 패키지(세라프) 
	//	{
	//		// 오픈 프리미엄 카테고리
	//		if( KSimLayer::GetKObj()->GetBillingFlag() == KSimLayer::BF_NEXON_KOREA )
	//		{
	//			kInfo.m_ulCategoryNo = 790;
	//		}
	//		else
	//		{
	//			kInfo.m_ulCategoryNo = 703;
	//		}
	//	}
	//	break;
	//}
#endif SERV_BILLING_SCRIPT_MANAGER
	//}}

	//////////////////////////////////////////////////////////////////////////

	//{{ 2009. 10. 14  최육사	자동결제상품리스트
	if( kInfo.m_ulPaymentType == AUTO_PAYMENT_TYPE )
	{
		KCSLOCK_BEGIN( m_vecAutoPaymentProductList )
			m_vecAutoPaymentProductList.push_back( kInfo.m_ulProductNo );
		KCSLOCK_END()
	}
	//}}

    // 무조건 새 정보로 덮어 쓴다.
    m_mapProductInfo[kInfo.m_ulProductNo] = kInfo;
}

int KNexonBillingTCPManager::GetItemID( unsigned long ulProductNo )
{
    KLocker lock( m_csProductInfo );

    std::map< unsigned long, KNXBTProductInfo >::iterator mit;
    mit = m_mapProductInfo.find( ulProductNo );
    if( mit == m_mapProductInfo.end() )
    {
        START_LOG( cerr, L"상품 번호 검색 실패." )
            << BUILD_LOG( ulProductNo )
            << END_LOG;

        return 0;
    }

    int iItemID;
    iItemID = ::atoi( KncUtil::toNarrowString( mit->second.m_wstrProductID ).c_str() );
    if( iItemID == 0 )
    {
        START_LOG( cerr, L"아이템 ID 변환 실패." )
            << BUILD_LOG( mit->second.m_wstrProductID )
            << END_LOG;
    }

    return iItemID;
}

//{{ 2010. 01. 28  최육사	빌링예외처리
int KNexonBillingTCPManager::GetProductPageNumber()
{
	unsigned long ulCurrentProductPage;
	KCSLOCK_BEGIN( m_ulCurrentProductPage )
		ulCurrentProductPage = m_ulCurrentProductPage;
	KCSLOCK_END()

	return ulCurrentProductPage;
}
//}}

int KNexonBillingTCPManager::GetNextProductPageNumber()
{
	unsigned long ulCurrentProductPage;
	KCSLOCK_BEGIN( m_ulCurrentProductPage )
		m_ulCurrentProductPage++;
		ulCurrentProductPage = m_ulCurrentProductPage;
	KCSLOCK_END()

    return ulCurrentProductPage;
}

void KNexonBillingTCPManager::BuildProductPage()
{
    KLocker lock( m_csProductInfo );

	//{{ 2010. 04. 26  최육사	PC방 전용 캐쉬템
	m_vecProductPage[CILT_NORMAL].clear();
	m_vecProductPage[CILT_PC_BANG].clear();
	//}}

	//{{ 2012. 02. 21	김민성	캐시샵 패킷 사이즈 수정
#ifdef SERV_QUICK_CASH_SHOP
	const int ciNumProductPerPage = KNexonBillingTCPManager::NBC_CASH_ITEM_MAX;
#else
	const int ciNumProductPerPage = 10;
#endif SERV_QUICK_CASH_SHOP
	//}}

	//{{ 2008. 5. 15  최육사  상품 정보 ItemID
	KENX_UPDATE_PRODUCT_LIST_NOT kEvent;
	kEvent.m_ulProductNoResStone = 0;
	//}}
    std::vector< KNXBTProductInfo > vecProductInfo[CILT_MAX];
    std::map< unsigned long, KNXBTProductInfo >::reverse_iterator mit;
	for( mit = m_mapProductInfo.rbegin(); mit != m_mapProductInfo.rend(); ++mit )
    {
		const int iCashItemID = _wtoi( mit->second.m_wstrProductID.c_str() );

		//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
		KCashPruductInfo kCashProductInfo;
		kCashProductInfo.m_ulProductNo		= mit->second.m_ulProductNo;
		kCashProductInfo.m_sProductExpire	= mit->second.m_sProductExpire;
		kCashProductInfo.m_usProductPieces	= mit->second.m_usProductPieces;
		kCashProductInfo.m_iItemID			= iCashItemID;
		kCashProductInfo.m_ulSalePrice		= mit->second.m_ulSalePrice;
		kEvent.m_vecCashProductList.push_back( kCashProductInfo );
#endif SERV_CASH_ITEM_LIST
		//}}

		//{{ 2008. 9. 23  최육사	캐쉬템 분해
		if( mit->second.IsInfinityProduct()  &&  iCashItemID != CXSLItem::CI_RESURRECTION_STONE )
		{
			KCashItemResolveInfo kPriceInfo;
			kPriceInfo.m_iItemID    = iCashItemID;
			kPriceInfo.m_iSalePrice = mit->second.m_ulSalePrice;

            kEvent.m_vecCashItemRessolveInfo.push_back( kPriceInfo );
		}
		//}}

		// 찜리스트에 참조될 상품ItemID 얻기
		kEvent.m_vecProductItemID.push_back( iCashItemID );

		//{{ 2008. 6. 10  최육사  실시간 부활석
		if( iCashItemID == CXSLItem::CI_RESURRECTION_STONE  &&  mit->second.m_usProductPieces == 1 )
		{
			kEvent.m_ulProductNoResStone = mit->second.m_ulProductNo;

			// 클라이언트는 못보도록 상품 페이지 리스트에서 뺀다.
			continue;
		}
		//}}

		//{{ 2010. 04. 26  최육사	PC방 전용 캐쉬템		
		if( CXSLItem::IsPcBangOnlyCashItem( iCashItemID ) == false )
		{
			vecProductInfo[CILT_NORMAL].push_back( mit->second );

			if( vecProductInfo[CILT_NORMAL].size() >= ciNumProductPerPage )
			{
				m_vecProductPage[CILT_NORMAL].push_back( vecProductInfo[CILT_NORMAL] );
				vecProductInfo[CILT_NORMAL].clear();
			}
		}
		//}}

		vecProductInfo[CILT_PC_BANG].push_back( mit->second );

		if( vecProductInfo[CILT_PC_BANG].size() >= ciNumProductPerPage )
		{
			m_vecProductPage[CILT_PC_BANG].push_back( vecProductInfo[CILT_PC_BANG] );
			vecProductInfo[CILT_PC_BANG].clear();
		}
    }
    
    if( !vecProductInfo[CILT_NORMAL].empty() )
    {
        m_vecProductPage[CILT_NORMAL].push_back( vecProductInfo[CILT_NORMAL] );
    }

	if( !vecProductInfo[CILT_PC_BANG].empty() )
	{
		m_vecProductPage[CILT_PC_BANG].push_back( vecProductInfo[CILT_PC_BANG] );
	}

	//{{ 2008. 5. 15  최육사  상품 정보 ItemID
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( 0, NULL, ENX_UPDATE_PRODUCT_LIST_NOT, kEvent );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
	//}}
}

bool KNexonBillingTCPManager::GetProductPage( IN int iPage, IN bool bIsPcBang, OUT std::vector< KNXBTProductInfo >& vecProductInfo )
{
    vecProductInfo.clear();

	//{{ 2010. 04. 26  최육사	PC방 전용 캐쉬템
	CASH_ITEM_LIST_TYPE eType = ( bIsPcBang ? CILT_PC_BANG : CILT_NORMAL );
	//}}

    if( m_vecProductPage[eType].empty() )
    {
        return true;
    }

    int iIndex = iPage - 1;

    if( iIndex >= int( m_vecProductPage[eType].size() ) )
    {
        iIndex = int( m_vecProductPage[eType].size() ) - 1;
    }

    if( iIndex < 0 )
    {
        iIndex = 0;
    }

    vecProductInfo = m_vecProductPage[eType][iIndex];
    return iIndex == ( int( m_vecProductPage[eType].size() ) - 1 );
}

bool KNexonBillingTCPManager::GetProductInfo( unsigned long ulProductNo, KNXBTProductInfo& kProductInfo )
{
    KLocker lock( m_csProductInfo );

    std::map< unsigned long, KNXBTProductInfo >::iterator mit;
    mit = m_mapProductInfo.find( ulProductNo );
    if( mit == m_mapProductInfo.end() )
    {
        return false;
    }

    kProductInfo = mit->second;
    return true;
}

bool KNexonBillingTCPManager::GetTotalPrice( const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo, unsigned long& ulPrice )
{
	KLocker lock( m_csProductInfo );

    ulPrice = 0;

    std::vector< KNXBTPurchaseReqInfo >::const_iterator vit;
    for( vit = vecPurchaseReqInfo.begin(); vit != vecPurchaseReqInfo.end(); vit++ )
    {
        std::map< unsigned long, KNXBTProductInfo >::iterator mit;
        mit = m_mapProductInfo.find( vit->m_ulProductNo );
        if( mit == m_mapProductInfo.end() )
        {
            return false;
        }

        ulPrice += mit->second.m_ulSalePrice;
    }

    return true;
}

void KNexonBillingTCPManager::HandleInitializeAck( bool bSucceed )
{
    m_bInitialized = bSucceed;
    SetEvent( m_spEvent.get() );
}

//{{ 2013. 01. 10	최육사	빌링 TCP 패킷 크기 예외처리
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
//////////////////////////////////////////////////////////////////////////

void KNexonBillingTCPManager::Recv()
{
	if( !IsConnected() )
	{
		return;
	}

	int ret;
	ret = ::recv( m_sock,
		m_cRecvBuffer + m_ulRecvCP,
		( int )( MAX_PACKET_SIZE_NBT - m_ulRecvCP ),
		0 );

	START_LOG( clog, L"패킷 받음." )
		<< BUILD_LOG( ret );

	if( ret == SOCKET_ERROR )
	{
		//{{ 2009. 11. 21  최육사	서버간접속구조개선
		if( IsFirstConnectSucc() )
		{
			//			START_LOG( cerr, GET_WSA_MSG );
		}
		//}}

		CLOSE_SOCKET( m_sock );

		//{{ 2010. 10. 13	최육사	넥슨 빌링 접속 종료
		OnDisconnectBillingServer( std::wstring( L"유효 하지 않은 소켓" ) );
		//}}
		return;
	}

	if( ret == 0 )
	{
		START_LOG( cerr, L"소켓 연결이 끊어짐." )
			<< END_LOG;

		CLOSE_SOCKET( m_sock );

		//{{ 2010. 10. 13	최육사	넥슨 빌링 접속 종료
		OnDisconnectBillingServer( std::wstring( L"원격지에서 접속 종료" ) );
		//}}
		return;
	}

	m_ulRecvCP += ret;

	// 모든 버퍼가 다 비워질때까지 패킷을 정리한다.
	while( m_ulRecvCP >= 10 )
	{
		// 헤더(9) + 타입(1) = 10
		// 모든 패킷이 10바이트 이상임

		// 가변 버퍼가 비워져 있다!
		if( m_kRecvVariableBuffer.IsAlreadyRecv() == false )
		{
			// 패킷 크기 분석!
			unsigned long ulLength;
			::memcpy( &ulLength, m_cRecvBuffer + 1, sizeof( ulLength ) );

			ulLength = ::ntohl( ulLength );
			unsigned long ulTotalPacketSize = ulLength + 5;

			// 말도 안되는 크기의 패킷이라면 여기서 통신 종료!
			if( ulTotalPacketSize > VARIABLE_BUFFER_MAX_SIZE )
			{
				//{{ 2013. 01. 08	최육사	빌링 패킷 넘치는 유저 찾기
#ifdef SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
				// 누가 넘쳤는지도 밝혀내자!
				UidType iUserUID = 0;
				BYTE bytePacketType = 0;
				std::wstring wstrPacketTypeName;
				CheckPacketMaxSizeOverUser( m_cRecvBuffer, iUserUID, bytePacketType, wstrPacketTypeName );
#endif SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
				//}}

				//{{ 2010. 10. 13	최육사	넥슨 빌링 접속 종료
				std::wstring wstrReason = boost::str( boost::wformat( L"최대 패킷 사이즈 초과! UserUID : %d, MaxPacketSize : %d, PacketType : %s" ) % iUserUID % ulTotalPacketSize % wstrPacketTypeName );
				OnDisconnectBillingServer( wstrReason );
				//}}

				START_LOG( cerr, L"추출한 패킷 사이즈 이상." )
					<< BUILD_LOG( iUserUID )
					<< BUILD_LOG( ulTotalPacketSize )
					<< BUILD_LOG( MAX_PACKET_SIZE_NBT )
					<< BUILD_LOG( wstrPacketTypeName )
					<< END_LOG;

				CLOSE_SOCKET( m_sock );
				return;
			}

			//{{ 2013. 01. 23	최육사	빌링 패킷 수신 처리 성능 측정
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
			// 여기부터!
			BeginCheckPerformance();
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
			//}}

			// 일단 패킷 크기만큼 가변 버퍼를 만들자!
			m_kRecvVariableBuffer.Init( ulTotalPacketSize );

			// 온전한 패킷 크기 하나 만큼 데이터를 모두 받았는지 확인
			if( ulTotalPacketSize <= m_ulRecvCP )
			{
				m_kRecvVariableBuffer.CopyBuffer( m_cRecvBuffer, ulTotalPacketSize );

				// 카피 한 만큼 버퍼를 이동 시키자!				
				::memmove( m_cRecvBuffer, m_cRecvBuffer + ulTotalPacketSize, m_ulRecvCP - ulTotalPacketSize );
				m_ulRecvCP -= ulTotalPacketSize;

				//////////////////////////////////////////////////////////////////////////
				START_LOG( cout, L"[테스트로그] 온전한 패킷 크기 하나만큼 데이터를 받았다! 가변버퍼에 수신된 패킷들을 쌓자!" )
					<< BUILD_LOG( ret )
					<< BUILD_LOG( m_ulRecvCP )
					<< BUILD_LOG( ulTotalPacketSize );
				//////////////////////////////////////////////////////////////////////////
			}
			// 덜 받았다면 덜 받은 대로 일단 넣어보자!
			else
			{
				m_kRecvVariableBuffer.CopyBuffer( m_cRecvBuffer, m_ulRecvCP );

				// 버퍼의 데이터를 모조리 카피했으니 버퍼는 초기화 하자!
				m_ulRecvCP = 0;
				//memset( m_cRecvBuffer, 0, MAX_PACKET_SIZE_NBT ); -- 굳이 초기화할 필요가 있을까?

				//////////////////////////////////////////////////////////////////////////
				START_LOG( cout, L"[테스트로그] 온전한 패킷 만큼은 아니지만 일단 데이터를 받았다! 가변버퍼에 수신된 패킷들을 쌓자!" )
					<< BUILD_LOG( ret )
					<< BUILD_LOG( m_ulRecvCP )
					<< BUILD_LOG( ulTotalPacketSize );
				//////////////////////////////////////////////////////////////////////////
			}
		}
		// 쌓여있는 데이터가 있다!
		else
		{
			const unsigned long ulRemainSize = m_kRecvVariableBuffer.GetRemainSize();

			// 남은 패킷만큼 패킷을 수신했는가?
			if( ulRemainSize <= m_ulRecvCP )
			{
				m_kRecvVariableBuffer.CopyBuffer( m_cRecvBuffer, ulRemainSize );

				// 카피 한 만큼 버퍼를 이동 시키자!
				::memmove( m_cRecvBuffer, m_cRecvBuffer + ulRemainSize, m_ulRecvCP - ulRemainSize );
				m_ulRecvCP -= ulRemainSize;

				//////////////////////////////////////////////////////////////////////////
				START_LOG( cout, L"[테스트로그] 이미 가변 버퍼에 받아 놓은 데이터에다가 더 추가해서 온전한 하나의 패킷으로 만들자!" )
					<< BUILD_LOG( ret )
					<< BUILD_LOG( m_ulRecvCP )
					<< BUILD_LOG( ulRemainSize );
				//////////////////////////////////////////////////////////////////////////
			}
			// 남은 패킷만큼 패킷을 못받았다.
			else
			{
				m_kRecvVariableBuffer.CopyBuffer( m_cRecvBuffer, m_ulRecvCP );

				// 버퍼의 데이터를 모조리 카피했으니 버퍼는 초기화 하자!
				m_ulRecvCP = 0;
				//memset( m_cRecvBuffer, 0, MAX_PACKET_SIZE ); -- 굳이 초기화할 필요가 있을까?

				//////////////////////////////////////////////////////////////////////////
				START_LOG( cout, L"[테스트로그] 이미 가변 버퍼에 받아 놓은 데이터에다가 더 추가해서 수신된 패킷들을 쌓자! 근데 더 받아야됭!" )
					<< BUILD_LOG( ret )
					<< BUILD_LOG( m_ulRecvCP )
					<< BUILD_LOG( ulRemainSize );
				//////////////////////////////////////////////////////////////////////////
			}
		}

		// 패킷이 완성 되었는가?
		if( m_kRecvVariableBuffer.IsComplete() == true )
		{
			//////////////////////////////////////////////////////////////////////////
			START_LOG( cout, L"[테스트로그] 가변 버퍼에 온전한 패킷 하나가 완성!!!" )
				<< BUILD_LOG( ret )
				<< BUILD_LOG( m_ulRecvCP )
				<< BUILD_LOG( m_kRecvVariableBuffer.GetBufferSize() )
				<< BUILD_LOG( (int)m_kRecvVariableBuffer.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////

			// 완성된 패킷이 최대 패킷 사이즈를 넘는지 확인!
			if( m_kRecvVariableBuffer.GetBufferSize() <= MAX_PACKET_SIZE_NBT )
			{
				// 온전한 한개의 패킷을 만들자!
				MakeEventFromReceivedPacket();
			}
			else
			{
				// 누가 넘쳤는지도 밝혀내자!
				UidType iUserUID = 0;
				BYTE bytePacketType = 0;
				std::wstring wstrPacketTypeName;
				CheckPacketMaxSizeOverUser( m_kRecvVariableBuffer.GetBuffer(), iUserUID, bytePacketType, wstrPacketTypeName );

                START_LOG( cerr, L"최대 패킷 사이즈를 넘어서는 패킷이 수신되었습니다! 해당 패킷은 소멸 시킵니다!" )
					<< BUILD_LOG( iUserUID )
					<< BUILD_LOGc( bytePacketType )
					<< BUILD_LOG( wstrPacketTypeName )
					<< END_LOG;
			}

			// 다음 패킷을 받기 위해 버퍼 초기화! ( 메모리 재할당을 하지 않기위해 Reset()함수를 사용 )
			m_kRecvVariableBuffer.Reset();

			//{{ 2013. 01. 23	최육사	빌링 패킷 수신 처리 성능 측정
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
			// 여기까지의 시간을 재면 될것 같다!
			EndCheckPerformance();
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
			//}}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
void KNexonBillingTCPManager::Recv()
{
	if( !IsConnected() )
	{
		return;
	}

	int ret;
	ret = ::recv( m_sock,
		m_cRecvBuffer + m_ulRecvCP,
		( int )( MAX_PACKET_SIZE_NBT - m_ulRecvCP ),
		0 );

	START_LOG( clog, L"패킷 받음." )
		<< BUILD_LOG( ret );

	if( ret == SOCKET_ERROR )
	{
		//{{ 2009. 11. 21  최육사	서버간접속구조개선
		if( IsFirstConnectSucc() )
		{
			//			START_LOG( cerr, GET_WSA_MSG );
		}
		//}}

		CLOSE_SOCKET( m_sock );

		//{{ 2010. 10. 13	최육사	넥슨 빌링 접속 종료
		OnDisconnectBillingServer( std::wstring( L"유효 하지 않은 소켓" ) );
		//}}
		return;
	}

	if( ret == 0 )
	{
		START_LOG( cerr, L"소켓 연결이 끊어짐." )
			<< END_LOG;

		CLOSE_SOCKET( m_sock );

		//{{ 2010. 10. 13	최육사	넥슨 빌링 접속 종료
		OnDisconnectBillingServer( std::wstring( L"원격지에서 접속 종료" ) );
		//}}
		return;
	}

	m_ulRecvCP += ret;

	while( m_ulRecvCP >= 10 )
	{
		// 헤더(9) + 타입(1) = 10
		// 모든 패킷이 10바이트 이상임

		unsigned long ulLength;
		::memcpy( &ulLength, m_cRecvBuffer + 1, sizeof( ulLength ) );

		ulLength = ::ntohl( ulLength );
		unsigned long ulTotalPacketSize = ulLength + 5;
		if( ulTotalPacketSize > MAX_PACKET_SIZE_NBT )
		{
			//{{ 2013. 01. 08	최육사	빌링 패킷 넘치는 유저 찾기
#ifdef SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
			// 누가 넘쳤는지도 밝혀내자!
			UidType iUserUID = 0;
			BYTE bytePacketType = 0;
			std::wstring wstrPacketTypeName;
			CheckPacketMaxSizeOverUser( m_cRecvBuffer, iUserUID, bytePacketType, wstrPacketTypeName );

			//{{ 2010. 10. 13	최육사	넥슨 빌링 접속 종료
			std::wstring wstrReason = boost::str( boost::wformat( L"최대 패킷 사이즈 초과! UserUID : %d, MaxPacketSize : %d, PacketType : %s" ) % iUserUID % ulTotalPacketSize % wstrPacketTypeName );
			OnDisconnectBillingServer( wstrReason );
			//}}

			START_LOG( cerr, L"추출한 패킷 사이즈 이상." )
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( ulTotalPacketSize )
				<< BUILD_LOG( MAX_PACKET_SIZE_NBT )
				<< BUILD_LOG( wstrPacketTypeName )
				<< END_LOG;

#endif SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK	// 해외팀 위치 변경
			//}}

			CLOSE_SOCKET( m_sock );
			return;
		}

		if( ulTotalPacketSize > m_ulRecvCP )
		{
			return;
		}

		//{{ 2013. 01. 23	최육사	빌링 패킷 수신 처리 성능 측정
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
		// 여기부터!
		BeginCheckPerformance();
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
		//}}

		MakeEventFromReceivedPacket();

		if( m_ulRecvCP > ulTotalPacketSize )
		{
			::memmove( m_cRecvBuffer, m_cRecvBuffer + ulTotalPacketSize, m_ulRecvCP - ulTotalPacketSize );
		}
		m_ulRecvCP -= ulTotalPacketSize;

		//{{ 2013. 01. 23	최육사	빌링 패킷 수신 처리 성능 측정
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
		// 여기까지의 시간을 재면 될것 같다!
		EndCheckPerformance();
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
		//}}
	}
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
//////////////////////////////////////////////////////////////////////////
//}}

void KNexonBillingTCPManager::Send()
{
	if( !IsConnected() )
	{
        return;
	}

	int ret;
	char buf[MAX_PACKET_SIZE_NBT] = {0,}; // 2012. 12. 3   버퍼 초기화를 배열을 선언한 부분에서 해주도록 수정.
    KNexonBillingTCPPacketPtr spPacket;
	while( GetSendPacket( spPacket ) )
	{
		if( !spPacket )
		{
			unsigned int uiSendQueueSize;
			KCSLOCK_BEGIN( m_kSendQueue )
				uiSendQueueSize = m_kSendQueue.size();
			KCSLOCK_END()

			START_LOG( cerr, L"포인터 이상." )
				<< BUILD_LOG( uiSendQueueSize )
				<< END_LOG;

			continue;
		}

		spPacket->WriteToBuffer( ( BYTE* )buf );
        DumpBuffer( ( BYTE* )buf, false );

        char* cCurBufPos = buf;
		int iRemainedSendSize = spPacket->GetTotalLength();
		while( iRemainedSendSize > 0 )
		{
			ret = ::send( m_sock,
				cCurBufPos,
				iRemainedSendSize,
				0 );

			if( ret == SOCKET_ERROR )
			{
				START_LOG( cwarn, GET_WSA_MSG );
				CLOSE_SOCKET( m_sock );
				return;
			}

			iRemainedSendSize -= ret;
            cCurBufPos += ret;
		}
	}
}

//{{ 2010. 10. 13	최육사	넥슨 빌링 접속 종료	
void KNexonBillingTCPManager::OnDisconnectBillingServer( IN const std::wstring& wstrReason )
{
	// 최초 접속이 아직 성공하지 않았기 때문에 접속 종료 리포팅을 하지 않는다.
	if( IsFirstConnectSucc() == false )
		return;

	//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
	{
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= L"Nexon Billing";
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( m_kNexonBillingTCPInfo.m_strIP );
		kNot.m_wstrReason			= wstrReason;
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
	}
#endif SERV_SERVER_DISCONNECT_LOG
	//}}
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	{
		KE_DISCONNECT_SERVER_REPORT_NOT kNot;
		kNot.m_cType = KMornitoringManager::ET_DISCONNECT_GAME_BILLING;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( 0, NULL, E_DISCONNECT_SERVER_REPORT_NOT, kNot );
		KBaseServer::GetKObj()->QueueingEvent( spEvent );
	}
#endif SERV_MORNITORING
	//}}

	START_LOG( cout, L"넥슨 빌링 서버 접속 종료!" )
		<< BUILD_LOG( wstrReason );
}
//}}

//{{ 2009. 11. 21  최육사	서버간접속구조개선
void KNexonBillingTCPManager::KeepConnectionThread()
{
	if( !IsFirstConnectSucc() )
	{
		// 최초 빌링서버 접속 시도. connect()함수 호출중에는 Thread Block
		if( Connect() )
		{
			FirstConnectSucc();
		}
		
		return;
	}

	CheckConnection();

	KeepConnection();
}
//}}

bool KNexonBillingTCPManager::Connect()
{
	//{{ [참고] Connect()함수가 호출되는 상황에서는 RecvThread에서 recv()함수가 항상 호출실패한다.
	//          따라서 m_ulRecvCP변수는 0으로 초기화 되더라도 문제되지 않음.
    m_ulRecvCP = 0;
	//}}

	m_sock = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성

	if( INVALID_SOCKET == m_sock )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;

		return false;
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr( m_kNexonBillingTCPInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kNexonBillingTCPInfo.m_usPort );

	int ret = ::connect( m_sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		//{{ 2009. 11. 21  최육사	서버간접속구조개선
		if( IsFirstConnectSucc() )
		{
//			START_LOG( cerr, GET_WSA_MSG )
//				<< BUILD_LOG( WSAGetLastError() )
//				<< BUILD_LOG( m_kNexonBillingTCPInfo.m_strIP )
//				<< BUILD_LOG( m_kNexonBillingTCPInfo.m_usPort )
//				<< END_LOG;
		}
		//}}

		CLOSE_SOCKET( m_sock );
		return false;
	}

    // KENX_BT_INITIALIZE_REQ를 큐잉하기 전에 false 로 만들어야 한다.
    m_bInitialized = false;

    KENX_BT_INITIALIZE_REQ kPacketInit;
    kPacketInit.m_ulPacketNo = GetNextPacketNo();
    kPacketInit.m_bytePacketType = KNexonBillingTCPPacket::INITIALIZE;
    kPacketInit.m_wstrServiceCode = L"ELSWD";
    kPacketInit.m_byteDomain = m_kNexonBillingTCPInfo.m_iDomain;

    boost::shared_ptr< KNexonBillingTCPPacket > spPacketInit( new KNexonBillingTCPPacket );
    spPacketInit->Write( kPacketInit );
    QueueingSendPacket( spPacketInit );

    switch( ::WaitForSingleObject( m_spEvent.get(), 5000 ) )
    {
    case WAIT_OBJECT_0: // ack를 제대로 받은 경우.
        START_LOG( cout, L"넥슨 빌링 서버 Initialize." )
            << BUILD_LOG( m_bInitialized );
        break;
    case WAIT_TIMEOUT:  // 시간 초과
        START_LOG( cerr, L"빌링 서버 Initialize 시간 초과." );
        break;
    default:
        START_LOG( cerr, L"빌링 서버 Initialize 실패." );
        break;
    }

    if( !m_bInitialized )
    {
        CLOSE_SOCKET( m_sock );
        return false;
    }

    return true;
}

bool KNexonBillingTCPManager::IsConnected()
{
	return ( m_sock != INVALID_SOCKET );
}

void KNexonBillingTCPManager::KeepConnection()
{
	if( ::GetTickCount() - m_dwLastHeartBeatTick < ms_dwHeartBeatGap )
	{
		return;
	}

    m_dwLastHeartBeatTick = ::GetTickCount();

	//{{ 2008. 5. 30  최육사  넥슨빌링 초기화 과정
	if( !m_bInitialized  ||  !IsConnected() )
		return;
	//}}

	// 허트빗 보내기
    KENX_BT_HEART_BEAT_REQ kPacketNot;
    kPacketNot.m_ulPacketNo = GetNextPacketNo();
    kPacketNot.m_bytePacketType = KNexonBillingTCPPacket::HEART_BEAT;
    kPacketNot.m_iReleaseTick = GetReleaseTick();
    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacketNot );
    QueueingSendPacket( spPacket );
}

void KNexonBillingTCPManager::CheckConnection()
{
	if( ::GetTickCount() - m_dwLastConnectionCheckTick < ms_dwConnectionCheckGap )
	{
		return;
	}

    m_dwLastConnectionCheckTick = ::GetTickCount();

	if( !IsConnected() )
	{
		// 초기화 되지 않은 상태로 설정!
		m_bInitialized = false;

		Connect();
	}
}

bool KNexonBillingTCPManager::GetSendPacket( KNexonBillingTCPPacketPtr& spPacket )
{
	KCSLOCK_BEGIN( m_kSendQueue )
		if( m_kSendQueue.empty() )
		{
			return false;
		}

		spPacket = m_kSendQueue.front();
		m_kSendQueue.pop();
	KCSLOCK_END()

    return true;
}

//{{ 2013. 01. 10	최육사	빌링 TCP 패킷 크기 예외처리
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
//////////////////////////////////////////////////////////////////////////
void KNexonBillingTCPManager::MakeEventFromReceivedPacket()
{
	DumpBuffer( ( BYTE* )m_kRecvVariableBuffer.GetBuffer(), false );
	
	// 가변 버퍼에서 온전한 하나의 패킷을 얻어낸다!
	KNexonBillingTCPPacket kPacket;
	if( !kPacket.ReadFromBuffer( ( BYTE* )m_kRecvVariableBuffer.GetBuffer() ) )
	{
		START_LOG( cerr, L"버퍼에서 읽기 실패." )
			<< END_LOG;

		DumpBuffer( ( BYTE* )m_kRecvVariableBuffer.GetBuffer(), true );
		return;
	}

	switch( kPacket.GetPacketType() )
	{
#   undef  _ENUM
#   define _ENUM( name, id ) \
	case id: \
		{ \
		K##ENX_BT_##name##_ACK kPacketAck; \
		kPacket.Read( kPacketAck ); \
		KEventPtr spEvent( new KEvent ); \
		spEvent->SetData( PI_GS_NX_BILLING_TCP, NULL, ENX_BT_##name##_ACK, kPacketAck ); \
		QueueingEvent( spEvent ); \
		} \
		break;

//#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
#undef _ENUM_PROCESS
#define _ENUM_PROCESS( name, id, pname ) _ENUM( pname, id )
//#endif // SERV_NEXON_COUPON_SYSTEM

#   include "NexonBillingTCP_def.h"
	default:
		START_LOG( cerr, L"패킷 타입이 이상함." )
			<< BUILD_LOG( kPacket.GetPacketType() )
			<< END_LOG;
		DumpBuffer( ( BYTE* )m_kRecvVariableBuffer.GetBuffer(), true );
		break;
	}
}

void KNexonBillingTCPManager::DumpBuffer( const BYTE* buffer, bool bError )
{
	JIF( buffer );
	JIF( sizeof( buffer ) >= 4 );

	unsigned long ulLength;
	::memcpy( &ulLength, buffer + 1, sizeof( ulLength ) );

	ulLength = ::ntohl( ulLength );
	unsigned long ulTotalPacketSize = ulLength + 5;

	// LIF( ulTotalPacketSize <= MAX_PACKET_SIZE_NBT ); 가변 버퍼를 쓰면 최대 패킷 크기를 넘어설수 있다!
	//ulTotalPacketSize = std::min< int >( ulTotalPacketSize, MAX_PACKET_SIZE_NBT );

	char szBuffer[MAX_PACKET_SIZE_NBT * 3 + 1];
	char szByte[4];

	for( unsigned long i = 0; i < ulTotalPacketSize; i++ )
	{
		if( ( int )buffer[i] >= 16 )
		{
			::itoa( ( int )buffer[i], szByte, 16 );
			szBuffer[i * 3] = szByte[0];
			szBuffer[i * 3 + 1] = szByte[1];
			szBuffer[i * 3 + 2] = ' ';
		}
		else
		{
			::itoa( ( int )buffer[i], szByte, 16 );
			szBuffer[i * 3] = '0';
			szBuffer[i * 3 + 1] = szByte[0];
			szBuffer[i * 3 + 2] = ' ';
		}
	}
	szBuffer[ulTotalPacketSize * 3] = '\0';

	std::string strBuffer = szBuffer;

	if( bError )
	{
		START_LOG( cerr2, L"BUFFER : ")
			<< BUILD_LOG( strBuffer );
	}
	else
	{
		START_LOG( clog2, L"BUFFER : ")
			<< BUILD_LOG( strBuffer );
	}
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
void KNexonBillingTCPManager::MakeEventFromReceivedPacket()
{
    DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KNexonBillingTCPPacket kPacket;
	if( !kPacket.ReadFromBuffer( ( BYTE* )m_cRecvBuffer ) )
    {
        START_LOG( cerr, L"버퍼에서 읽기 실패." )
            << END_LOG;

        DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
        return;
    }

    switch( kPacket.GetPacketType() )
    {
#   undef  _ENUM
#   define _ENUM( name, id ) \
    case id: \
        { \
            K##ENX_BT_##name##_ACK kPacketAck; \
            kPacket.Read( kPacketAck ); \
            KEventPtr spEvent( new KEvent ); \
            spEvent->SetData( PI_GS_NX_BILLING_TCP, NULL, ENX_BT_##name##_ACK, kPacketAck ); \
            QueueingEvent( spEvent ); \
        } \
        break;

//#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
#undef _ENUM_PROCESS
#define _ENUM_PROCESS( name, id, pname ) _ENUM( pname, id )
//#endif // SERV_NEXON_COUPON_SYSTEM

#   include "NexonBillingTCP_def.h"
    default:
        START_LOG( cerr, L"패킷 타입이 이상함." )
            << BUILD_LOG( kPacket.GetPacketType() )
            << END_LOG;
        DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
        break;
    }
}

void KNexonBillingTCPManager::DumpBuffer( const BYTE* buffer, bool bError )
{
	JIF( buffer );
	JIF( sizeof( buffer ) >= 4 );

	unsigned long ulLength;
	::memcpy( &ulLength, buffer + 1, sizeof( ulLength ) );

	ulLength = ::ntohl( ulLength );
	unsigned long ulTotalPacketSize = ulLength + 5;

	LIF( ulTotalPacketSize <= MAX_PACKET_SIZE_NBT );
	ulTotalPacketSize = std::min< int >( ulTotalPacketSize, MAX_PACKET_SIZE_NBT );

	char szBuffer[MAX_PACKET_SIZE_NBT * 3 + 1];
	char szByte[4];

	for( unsigned long i = 0; i < ulTotalPacketSize; i++ )
	{
		if( ( int )buffer[i] >= 16 )
		{
#ifdef _CONVERT_VS_2010
			_itoa_s( ( int )buffer[i], szByte, 4, 16 );
#else
			::itoa( ( int )buffer[i], szByte, 16 );
#endif _CONVERT_VS_2010
			szBuffer[i * 3] = szByte[0];
			szBuffer[i * 3 + 1] = szByte[1];
			szBuffer[i * 3 + 2] = ' ';
		}
		else
		{
#ifdef _CONVERT_VS_2010
			_itoa_s( ( int )buffer[i], szByte, 4, 16 );
#else
			::itoa( ( int )buffer[i], szByte, 16 );
#endif _CONVERT_VS_2010
			szBuffer[i * 3] = '0';
			szBuffer[i * 3 + 1] = szByte[0];
			szBuffer[i * 3 + 2] = ' ';
		}
	}
	szBuffer[ulTotalPacketSize * 3] = '\0';

	std::string strBuffer = szBuffer;

	if( bError )
	{
		START_LOG( cerr2, L"BUFFER : ")
			<< BUILD_LOG( strBuffer );
	}
	else
	{
		START_LOG( clog2, L"BUFFER : ")
			<< BUILD_LOG( strBuffer );
	}
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
//////////////////////////////////////////////////////////////////////////
//}}

void KNexonBillingTCPManager::ResetProductPageNumber()
{
	KCSLOCK_SET_VALUE( m_ulCurrentProductPage, 0 );
}

void KNexonBillingTCPManager::ResetProductInfo()
{
    KLocker lock( m_csProductInfo );

    m_mapProductInfo.clear();
	//{{ 2012. 03. 21	김민성		캐쉬 상품 리스트 서버군 구별없이 DB에 기록
#ifdef SERV_CASH_ITEM_LIST_ADD
	m_mapProductInfo_Excepted.clear();
#endif SERV_CASH_ITEM_LIST_ADD
	//}}
}

void KNexonBillingTCPManager::GetAutoPaymentProductList( std::vector< unsigned long >& vecProductList )
{
	KCSLOCK_BEGIN( m_vecAutoPaymentProductList )
		vecProductList = m_vecAutoPaymentProductList;
	KCSLOCK_END()
}

//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
void KNexonBillingTCPManager::GetCashProductList( OUT std::vector< KCashPruductInfo >& vecCashProductList ) const
{
	KLocker lock( m_csProductInfo );

	std::map< unsigned long, KNXBTProductInfo >::const_reverse_iterator mit;
	for( mit = m_mapProductInfo.rbegin(); mit != m_mapProductInfo.rend(); ++mit )
	{
		const int iCashItemID = _wtoi( mit->second.m_wstrProductID.c_str() );

		KCashPruductInfo kCashProductInfo;
		kCashProductInfo.m_ulProductNo		= mit->second.m_ulProductNo;
		kCashProductInfo.m_sProductExpire	= mit->second.m_sProductExpire;
		kCashProductInfo.m_usProductPieces	= mit->second.m_usProductPieces;
		kCashProductInfo.m_iItemID			= iCashItemID;
		kCashProductInfo.m_ulSalePrice		= mit->second.m_ulSalePrice;
		vecCashProductList.push_back( kCashProductInfo );
	}

#ifdef SERV_CASH_ITEM_LIST_ADD	// 빌드 오류로 해외팀 추가
	std::map< unsigned long, KNXBTProductInfo >::const_reverse_iterator mit_Excepted;
	for( mit_Excepted = m_mapProductInfo_Excepted.rbegin(); mit_Excepted != m_mapProductInfo_Excepted.rend(); ++mit_Excepted )
	{
		const int iCashItemID = _wtoi( mit_Excepted->second.m_wstrProductID.c_str() );

		KCashPruductInfo kCashProductInfo;
		kCashProductInfo.m_ulProductNo		= mit_Excepted->second.m_ulProductNo;
		kCashProductInfo.m_sProductExpire	= mit_Excepted->second.m_sProductExpire;
		kCashProductInfo.m_usProductPieces	= mit_Excepted->second.m_usProductPieces;
		kCashProductInfo.m_iItemID			= iCashItemID;
		kCashProductInfo.m_ulSalePrice		= mit_Excepted->second.m_ulSalePrice;
		vecCashProductList.push_back( kCashProductInfo );
	}
#endif // SERV_CASH_ITEM_LIST_ADD

}
#endif SERV_CASH_ITEM_LIST


//{{ 2010. 11. 22	최육사	청약 철회
#ifdef SERV_NX_BILLING_REFUND
bool KNexonBillingTCPManager::GetCashItemRefundRequestID( OUT std::wstring& wstrRequestID )
{
	__int64 iServerUID = KBaseServer::GetKObj()->GetUID();
	if( iServerUID > 0xFF )
	{
		START_LOG( cerr, L"ServerUID가 255를 넘으면 안됩니다!" )
			<< BUILD_LOG( iServerUID )
			<< END_LOG;
		return false;
	}

	// 날짜 정보 넣기
	{
		CTime tCurTime = CTime::GetCurrentTime();
		wstrRequestID = ( std::wstring )tCurTime.Format( _T( "%Y%m%d%H%M%S" ) );
	}

	// 게임서버 발급 ID넣기
	{
		__int64 iCashItemRefundRequestID = 0;

		KCSLOCK_BEGIN( m_iCashItemRefundRequestID )
			++m_iCashItemRefundRequestID;
			iCashItemRefundRequestID = m_iCashItemRefundRequestID;
		KCSLOCK_END()

		__int64 iResultRequestID = ( iCashItemRefundRequestID & 0xffffffff00ffffff ) | ( ( ( iServerUID & ( __int64 )0x00000000000000ff ) << 24 ) & 0x00000000ff000000 );

		wstrRequestID += boost::str( boost::wformat( L"%010d" ) % iResultRequestID );
	}
	
	return true;
}
#endif SERV_NX_BILLING_REFUND
//}}

//{{ 2013. 01. 08	최육사	빌링 패킷 넘치는 유저 찾기
#ifdef SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
bool KNexonBillingTCPManager::CheckPacketMaxSizeOverUser( IN const char* pBuffer, OUT UidType& iUserUID, OUT BYTE& bytePacketType, OUT std::wstring& wstrPacketTypeName )
{
	iUserUID = 0;
	bytePacketType = 0;

	BYTE byteReservedChar = 0;
	ULONG ulPacketLength = 0;
	ULONG ulPacketNo = 0;
	
	// 예약 문자
	::memcpy( &byteReservedChar, pBuffer, sizeof( BYTE ) );
	_JIF( byteReservedChar == 0xAF, return false );

	// 패킷 길이
	::memcpy( &ulPacketLength, pBuffer + 1, sizeof( unsigned long ) );
	ulPacketLength = ::ntohl( ulPacketLength );
	//_JIF( ulPacketLength >= HEADER_IN_PACKET_LENGTH_NBT, return false );
	//_JIF( ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT <= MAX_PACKET_CONTENT_SIZE_NBT, return false );

	// 패킷 넘버
	::memcpy( &ulPacketNo, pBuffer + 5, sizeof( unsigned long ) );
	ulPacketNo = ::ntohl( ulPacketNo );

	// 패킷 타입
	::memcpy( &bytePacketType, pBuffer + 9, sizeof( BYTE ) );

	// 어떤 유저가 요청한 패킷인지 확인하자!
	iUserUID = GetCorrespondingUserUID( ulPacketNo );

	// 패킷 타입 이름을 얻자!
	wstrPacketTypeName = KNexonBillingTCPPacket::GetPacketTypeStr( bytePacketType );

	START_LOG( cout, L"[알림] 최대 패킷 크기를 넘어선 패킷의 헤더 정보!" )
		<< BUILD_LOGc( byteReservedChar )
		<< BUILD_LOG( ulPacketLength )
		<< BUILD_LOG( ulPacketNo )
		<< BUILD_LOGc( bytePacketType )
		<< BUILD_LOG( wstrPacketTypeName );
	return true;
}
#endif SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
//}}

//{{ 2013. 01. 23	최육사	빌링 패킷 수신 처리 성능 측정
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
void KNexonBillingTCPManager::BeginCheckPerformance()
{
	m_kRecvPerformanceDump.m_dwLastTickCount = ::GetTickCount();
}

void KNexonBillingTCPManager::EndCheckPerformance()
{
	const DWORD dwLatency = ::GetTickCount() - m_kRecvPerformanceDump.m_dwLastTickCount;
	if( dwLatency < m_kRecvPerformanceDump.m_dwMinLatency )
	{
		CTime tCurTime = CTime::GetCurrentTime();

		m_kRecvPerformanceDump.m_dwMinLatency = dwLatency;
        m_kRecvPerformanceDump.m_wstrMinRegDate = (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
	}

	if( dwLatency > m_kRecvPerformanceDump.m_dwMaxLatency )
	{
		CTime tCurTime = CTime::GetCurrentTime();

		m_kRecvPerformanceDump.m_dwMaxLatency = dwLatency;
		m_kRecvPerformanceDump.m_wstrMaxRegDate = (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
	}
}
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
//}}


