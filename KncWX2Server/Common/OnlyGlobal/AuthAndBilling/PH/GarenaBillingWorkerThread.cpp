#include "GarenaBillingServer.h"
//#include "GarenaBillingMiddleware.h"
#include "GarenaBillingWorkerThread.h"
#include <dbg/dbg.hpp>

#include "Enum/Enum.h"
#include "GarenaBillingUtil.h"
#include "NetError.h"

#ifdef SERV_COUNTRY_PH


#include "UserList.h"


#define CLASS_TYPE      KGarenaBillingWorkerThread

KGarenaBillingWorkerThread::KGarenaBillingWorkerThread()
{
}

KGarenaBillingWorkerThread::~KGarenaBillingWorkerThread()
{
	End( 3000 );
}

void KGarenaBillingWorkerThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
		CASE( EJSON_GN_CHECK_ACCOUNT_REQ );
		CASE( EJSON_GN_CHECK_ACCOUNT_ACK );

		CASE( EJSON_GN_CHANGE_GAME_CURRENCY_REQ );
		CASE( EJSON_GN_CHANGE_GAME_CURRENCY_ACK );
		CASE( EJSON_GN_TRANSACTION_REQ );
		CASE( EJSON_GN_TRANSACTION_ACK );

		CASE( EBILL_LOGIN_CHECK_BALANCE_ACK );
		CASE( EBILL_GN_CHANGE_GAME_CURRENCY_ACK );
		CASE( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK );
		CASE( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK );	


	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음 - " << spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID );
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 1000 )
	{
		START_LOG( cwarn, L"TCP RELAY 패킷 처리 시간지연.!" )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

IMPL_ON_FUNC( EJSON_GN_CHECK_ACCOUNT_REQ )
{

	//KEJSON_GN_CHECK_ACCOUNT_REQ kPacket;
	//KEJSON_GN_CHECK_ACCOUNT_ACK kPacketAck;

	//kPacket.m_uiGarenaUID = kPacket_.m_uiGarenaUID;

	/*SendToUser( FIRST_SENDER_UID, EJSON_GN_CHECK_ACCOUNT_REQ, kPacket );*/

	KUserList::KGSUserInfo kInfo;

	
	if( false == SiKGarenaBillingServer()->GetCheckUserInfoByPublisherUID(kPacket_.m_uiGarenaUID, kInfo) )
	{

		START_LOG( clog, L"해당 유저가 로그인 되어 있지 않음" )
			<< BUILD_LOG( kPacket_.m_uiGarenaUID )
			<< END_LOG;

		KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ kPacketReq;
		kPacketReq.m_uiGarenaUID = kPacket_.m_uiGarenaUID;
		kPacketReq.m_iSessionUID = LAST_SENDER_UID;
		SendToAccountDB( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ, kPacketReq);
	}
	else
	{

		START_LOG( clog, L"Garena에서 계정 조회 요청이 들어왔다." )
			<< BUILD_LOG( kPacket_.m_uiGarenaUID )
			<< END_LOG;

		KEBILL_LOGIN_CHECK_BALANCE_REQ kPacketBalanceReq;

		kPacketBalanceReq.m_uiPublisherUID = kPacket_.m_uiGarenaUID;
		kPacketBalanceReq.m_wstrCharName = kInfo.m_wstrUnitNickName;
		kPacketBalanceReq.m_iSessionUID = LAST_SENDER_UID;

		SendToKOGBillingDB( EBILL_LOGIN_CHECK_BALANCE_REQ, kPacketBalanceReq );
	}


}

IMPL_ON_FUNC( EJSON_GN_CHECK_ACCOUNT_ACK )
{


	SendToGarenaBilling(kPacket_.m_iSessionUID, EJSON_GN_CHECK_ACCOUNT_ACK, kPacket_ );

}

IMPL_ON_FUNC( EJSON_GN_CHANGE_GAME_CURRENCY_REQ )
{

	KEJSON_GN_CHANGE_GAME_CURRENCY_REQ kPacket;
	kPacket.m_uiGameCurrency_Amount = kPacket_.m_uiGameCurrency_Amount;
	kPacket.m_uiShell_Amount = kPacket_.m_uiShell_Amount;
	kPacket.m_uiGarenaUID = kPacket_.m_uiGarenaUID;
	kPacket.m_strGarenaTransactionID = kPacket_.m_strGarenaTransactionID;
	kPacket.m_iSessionUID = LAST_SENDER_UID;

	KUserList::KGSUserInfo kInfo;
	if( false == SiKGarenaBillingServer()->GetCheckUserInfoByPublisherUID(kPacket_.m_uiGarenaUID, kInfo) )
	{

		START_LOG( clog, L"해당 유저가 로그인 되어 있지 않음" )
			<< BUILD_LOG( kPacket_.m_uiGarenaUID )
			<< BUILD_LOG( kPacket_.m_uiGameCurrency_Amount )
			<< BUILD_LOG( kPacket_.m_uiShell_Amount )
			<< BUILD_LOG( kPacket_.m_strGarenaTransactionID )
			<< END_LOG;

		KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_REQ kPacketReq;
		kPacketReq.m_kGNGameCurrencyREQ.m_uiGarenaUID = kPacket_.m_uiGarenaUID;
		kPacketReq.m_kGNGameCurrencyREQ.m_uiGameCurrency_Amount = kPacket_.m_uiGameCurrency_Amount;
		kPacketReq.m_kGNGameCurrencyREQ.m_uiShell_Amount = kPacket_.m_uiShell_Amount;
		kPacketReq.m_kGNGameCurrencyREQ.m_strGarenaTransactionID = kPacket_.m_strGarenaTransactionID;
		kPacketReq.m_kGNGameCurrencyREQ.m_iSessionUID = LAST_SENDER_UID;

		SendToAccountDB( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_REQ, kPacketReq);

	}
	else
	{
		START_LOG( clog, L"Garena에서 캐쉬 전환 조회 요청이 들어왔다." )
			<< BUILD_LOG( kPacket_.m_uiGarenaUID )
			<< BUILD_LOG( kPacket_.m_uiGameCurrency_Amount )
			<< BUILD_LOG( kPacket_.m_uiShell_Amount )
			<< BUILD_LOG( kPacket_.m_strGarenaTransactionID )
			<< END_LOG;

		SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, EJSON_GN_CHANGE_GAME_CURRENCY_REQ, kPacket );
	}

}

IMPL_ON_FUNC( EJSON_GN_CHANGE_GAME_CURRENCY_ACK )
{


	boost::shared_ptr< KGarenaBillingPacket > spPacket( new KGarenaBillingPacket );

	if ( kPacket_.m_iResult != 0)
	{

		SiKGarenaBillingServer()->SetmapChangeGameCurrencyFailed(kPacket_);

	}


	SendToGarenaBilling(kPacket_.m_iSessionUID, EJSON_GN_CHANGE_GAME_CURRENCY_ACK, kPacket_ );

	/*spPacket->Write( kPacket_ );
	SiKGarenaBillingManager()->QueueingSendPacket( spPacket );*/

}

IMPL_ON_FUNC( EJSON_GN_TRANSACTION_REQ )
{

	kPacket_.m_iSessionUID = LAST_SENDER_UID;

	SendToKOGBillingDB( EBILL_GN_TRANSACTION_CHECK_REQ, kPacket_ );



	/*KEJSON_GN_TRANSACTION_ACK kPacket;

	kPacket.m_iSessionUID = LAST_SENDER_UID;*/
	

	//SiKGarenaBillingServer()->GetmapChangeGameCurrencyFailed(kPacket_, kPacket );


	//SendToGarenaBilling(kPacket.m_iSessionUID, EJSON_GN_TRANSACTION_ACK, kPacket );

	//boost::shared_ptr< KGarenaBillingPacket > spPacket( new KGarenaBillingPacket );
	//spPacket->Write( kPacket );
	//SiKGarenaBillingManager()->QueueingSendPacket( spPacket );

}

IMPL_ON_FUNC( EJSON_GN_TRANSACTION_ACK )
{
	SendToGarenaBilling(kPacket_.m_iSessionUID, EJSON_GN_TRANSACTION_ACK, kPacket_ );
}

#ifdef SERV_COUNTRY_PH
IMPL_ON_FUNC( EBILL_LOGIN_CHECK_BALANCE_ACK )
{

	//VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) ); // 2013.08.14 lygan_조성욱 // 동남아시아는 유저가 로그인 하지 않은상태에서도 잔액 조회를 해야 한다.

	KEJSON_GN_CHECK_ACCOUNT_ACK kPacket;
	bool bCheckBalanceFailed = false;

	kPacket.m_iResult = 0;

	kPacket.m_uiGarenaUID = kPacket_.m_uiPublisherUID;
	kPacket.m_iSessionUID = kPacket_.m_iSessionUID;


	if (kPacket_.m_iOK != NetError::NET_OK)
	{
		bCheckBalanceFailed = true;
		kPacket.m_iResult = 99;
		kPacket.m_wstrCharName = kPacket_.m_wstrCharName;
		kPacket.m_uiCurrency_Balance = 0;

		START_LOG( cerr, L"Garena에서 계정 조회 요청 들어 왔는데 엘소드 캐쉬 잔액 얻어 오는 부분에서 DB에러" )
			<< BUILD_LOG( kPacket_.m_uiPublisherUID)
			<< BUILD_LOG( kPacket_.m_wstrCharName)
			<< BUILD_LOG( kPacket_.m_iOK)
			<< BUILD_LOG( kPacket_.m_iSessionUID)
			<< END_LOG;
	}


	if (bCheckBalanceFailed == false)
	{
		kPacket.m_wstrCharName = kPacket_.m_wstrCharName;
		kPacket.m_uiCurrency_Balance = kPacket_.m_ulBalance;

		START_LOG( clog, L"Garena 에서 계정 조회" )
			<< BUILD_LOG( kPacket.m_iResult )
			<< BUILD_LOG( kPacket.m_uiGarenaUID )
			<< BUILD_LOG( kPacket.m_wstrCharName )
			<< BUILD_LOG( kPacket.m_uiCurrency_Balance )
			<< BUILD_LOG( kPacket.m_iSessionUID )
			<< END_LOG;
	}

	SendToGarenaBilling(kPacket.m_iSessionUID, EJSON_GN_CHECK_ACCOUNT_ACK, kPacket );

	


}

IMPL_ON_FUNC( EBILL_GN_CHANGE_GAME_CURRENCY_ACK )
{

	KEJSON_GN_CHANGE_GAME_CURRENCY_ACK kPacket;

	int iOK = 0;
	if ( kPacket_.m_kGarenaACK.m_iResult != 0 )
	{

		iOK = NetError::ERR_GIANT_BILLING_08;

		kPacket.m_uiGameCurrency_Amount		= kPacket_.m_kGarenaACK.m_uiGameCurrency_BeforeAmount;
		kPacket.m_iSessionUID				= kPacket_.m_kGarenaACK.m_iSessionUID;

		START_LOG( cerr, L"Garena 에서 캐쉬 전환 실패" )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_iResult )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_int64GameTransactionID )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_strGarenaTransactionID )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_uiGameCurrency_Amount )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_uiGameCurrency_Balance )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_uiGarenaUID )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_iSessionUID )
			<< END_LOG;


	}
	else
	{

		kPacket.m_uiGameCurrency_Amount		= kPacket_.m_kGarenaACK.m_uiGameCurrency_Amount;
		kPacket.m_iSessionUID				= kPacket_.m_kGarenaACK.m_iSessionUID;

		START_LOG( clog, L"Garena 에서 캐쉬 전환 성공" )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_iResult )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_int64GameTransactionID )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_strGarenaTransactionID )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_uiGameCurrency_Amount )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_uiGameCurrency_Balance )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_uiGarenaUID )
			<< BUILD_LOG( kPacket_.m_kGarenaACK.m_iSessionUID )
			<< END_LOG;

	}

	kPacket.m_iResult					= kPacket_.m_kGarenaACK.m_iResult;
	kPacket.m_int64GameTransactionID		= kPacket_.m_kGarenaACK.m_int64GameTransactionID;
	kPacket.m_strGarenaTransactionID	= kPacket_.m_kGarenaACK.m_strGarenaTransactionID;

	kPacket.m_uiGameCurrency_Balance	= kPacket_.m_kGarenaACK.m_uiGameCurrency_Balance;
	kPacket.m_uiGarenaUID				= kPacket_.m_kGarenaACK.m_uiGarenaUID;


	SendToGarenaBilling(kPacket.m_iSessionUID, EJSON_GN_CHANGE_GAME_CURRENCY_ACK, kPacket );


	if (kPacket_.m_bIsOffLine == false)
	{

		KEGS_EXCHANGE_CASH_NOT kPacketNOT;
		kPacketNOT.m_iOK = iOK;
		kPacketNOT.m_iPoint = kPacket_.m_kGarenaACK.m_uiGameCurrency_Amount;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		kPacketNOT.m_AddBonusCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT] = kPacket_.m_AddCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT];
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		
		KUserList::KGSUserInfo kInfo;
		if( true == SiKGarenaBillingServer()->GetCheckUserInfoByPublisherUID(kPacket_.m_kGarenaACK.m_uiGarenaUID, kInfo) )
		{
			SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, EGS_EXCHANGE_CASH_NOT, kPacketNOT );
		}
	}

}
IMPL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK )
{
	if (kPacket_.m_iResult != 0)
	{
		KEJSON_GN_CHECK_ACCOUNT_ACK kPacket;

		kPacket.m_iResult = 1;
		kPacket.m_wstrCharName = L"";
		kPacket.m_uiCurrency_Balance = 0;
		kPacket.m_uiGarenaUID = kPacket_.m_uiPublisherUID;
		kPacket.m_iSessionUID = kPacket_.m_iSessionUID;

		START_LOG( cwarn, L"Garena에서 계정 조회 요청 들어 왔는데 우리한테 없는 유저다!!" )
			<< BUILD_LOG( kPacket_.m_uiPublisherUID)
			<< END_LOG;

		SendToGarenaBilling(kPacket.m_iSessionUID, EJSON_GN_CHECK_ACCOUNT_ACK, kPacket );


		return;
	}



	KEBILL_LOGIN_CHECK_BALANCE_REQ kPacketBalanceReq;

	kPacketBalanceReq.m_uiPublisherUID = kPacket_.m_uiPublisherUID;
	kPacketBalanceReq.m_wstrCharName = L""; // 2013.06.14 lygan_조성욱 // 로그인 한 시점이 아니라서 정해진 캐릭터 이름을 알 수 없다.
	kPacketBalanceReq.m_iSessionUID = kPacket_.m_iSessionUID;

	SendToKOGBillingDB( EBILL_LOGIN_CHECK_BALANCE_REQ, kPacketBalanceReq );




}

IMPL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK )
{

	if (kPacket_.m_iResult != 0)
	{
		KEJSON_GN_CHANGE_GAME_CURRENCY_ACK kPacketACK;
		kPacketACK.m_iResult = 1;
		kPacketACK.m_uiGarenaUID = kPacket_.m_kGNGameCurrencyREQ.m_uiGarenaUID;
		kPacketACK.m_int64GameTransactionID = 0LL;
		kPacketACK.m_strGarenaTransactionID = "";
		kPacketACK.m_uiGameCurrency_Amount = 0;
		kPacketACK.m_uiGameCurrency_Balance = 0;

		CTime tCurrent  = CTime::GetCurrentTime();
		kPacketACK.m_strDateTime = (CStringA)tCurrent.Format(_T("%Y-%m-%d %H:%M:%S"));
		kPacketACK.m_uiShell_Amount = kPacket_.m_kGNGameCurrencyREQ.m_uiShell_Amount;
		kPacketACK.m_uiGameCurrency_BeforeAmount = kPacket_.m_kGNGameCurrencyREQ.m_uiGameCurrency_Amount;
		kPacketACK.m_iSessionUID = kPacket_.m_kGNGameCurrencyREQ.m_iSessionUID;

		SendToKOGBillingDB(EBILL_GN_CHANGE_GAME_CURRENCY_FAIL_NOT, kPacketACK);

		START_LOG( cerr, L"Garena에서 캐쉬 전환 요청 들어 왔는데 우리한테 없는 유저다!!" )
			<< BUILD_LOG( kPacket_.m_kGNGameCurrencyREQ.m_uiGarenaUID)
			<< END_LOG;

		SendToGarenaBilling(kPacketACK.m_iSessionUID, EJSON_GN_CHANGE_GAME_CURRENCY_ACK, kPacketACK );


		return;
	}

	KEBILL_GN_CHANGE_GAME_CURRENCY_REQ kPacket;


	kPacket.m_iResult = kPacket_.m_iResult;

	kPacket.m_kGarenaREQ = kPacket_.m_kGNGameCurrencyREQ;

	kPacket.m_PurchaserInfo.m_iServerGroupID = 0;
	kPacket.m_PurchaserInfo.m_iUserUID = kPacket_.m_iUserUID;
	kPacket.m_PurchaserInfo.m_wstrUserName = kPacket_.m_wstrUserName;
	kPacket.m_PurchaserInfo.m_iUnitUID = 0;
	kPacket.m_PurchaserInfo.m_wstrUnitNickName = L"";
	kPacket.m_PurchaserInfo.m_ucLevel = 0;
	kPacket.m_PurchaserInfo.m_wstrIP = L"";
	kPacket.m_PurchaserInfo.m_uiPublisherUID = kPacket_.m_kGNGameCurrencyREQ.m_uiGarenaUID;


	SendToKOGBillingDB(EBILL_GN_CHANGE_GAME_CURRENCY_REQ, kPacket);



}
#endif //SERV_COUNTRY_PH

#endif SERV_COUNTRY_PH
//}}


