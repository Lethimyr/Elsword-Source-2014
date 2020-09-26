#include "ActorManager.h"
#include "GiantBilling.h"

#ifdef SERV_COUNTRY_CN

#include "KncSend.h"
#include "GiantBillingManager.h"
#include "GiantCouponManager.h"
#include "NetError.h"

#include "LoginSimLayer.h"
#include "UserList.h"

#define CLASS_TYPE  KGiantBilling

KGiantBilling::KGiantBilling( void )
{
}

KGiantBilling::~KGiantBilling( void )
{
	End( 15000 );
}



// packet send function

//void KGiantBilling::SendToBillManager( unsigned short usEventID )
//{
//	SendToBillManager( usEventID, char() );
//}

void KGiantBilling::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID )
{
	SendToGSUser( nGS, nTo, usEventID, char() );
}

void KGiantBilling::SendToKOGBillingDB( unsigned short usEventID )
{
	SendToKOGBillingDB( usEventID, char() );
}

//----------------------//
// Process Event handle //
//----------------------//
void KGiantBilling::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
 	CASE( EGIANT_COMMON_NULL_SERVER );
 	CASE( EGIANT_COMMON_NULL_CLIENT );

	//{{ 2011. 02. 28	최육사	중국 자이언트 빌링
	CASE( EGIANT_BILLING_RETURN );
	CASE( EPUBLISHER_BILLING_BALANCE_REQ );
	CASE( EGIANT_BILLING_CONSUME_REQ );
	CASE( EGIANT_BILLING_PCARD_CMD );
	CASE( EGIANT_BILLING_SCARD_CMD );

	// 충전 카드는 필요시에 재구현해야함
	//CASE( EGIANT_BILLING_USE_CARD_CMD );
	//CASE( EGIANT_BILLING_USE_CARD_RET_CMD );
	//}}

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
	START_LOG( clog, L"UserServer - Null Server" )
		<< END_LOG;

	boost::shared_ptr< KGiantBillingPacket > spPacket( new KGiantBillingPacket );
	spPacket->Write( kPacket_ );
	SiKGiantBillingManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_COMMON_NULL_CLIENT )
{
	START_LOG( clog, L"UserServer - Null Client" )
		<< END_LOG;

	boost::shared_ptr< KGiantBillingPacket > spPacket( new KGiantBillingPacket );
	spPacket->Write( kPacket_ );
	SiKGiantBillingManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_BILLING_CONSUME_REQ )
{
	START_LOG( clog, L"UserServer - EGIANT_BILLING_CONSUME_REQ" )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_iPoint )
		<< BUILD_LOG( kPacket_.m_iSource )
		<< BUILD_LOG( kPacket_.m_wstrTID )
		<< BUILD_LOG( kPacket_.m_wstrRemark )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_iAT )
		<< END_LOG;

	boost::shared_ptr< KGiantBillingPacket > spPacket( new KGiantBillingPacket );
	spPacket->Write( kPacket_ );
	SiKGiantBillingManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_BILLING_RETURN )
{
	START_LOG( clog, L"UserServer - EGIANT_BILLING_RETURN" )
		<< BUILD_LOG( kPacket_.m_wstrTID )
		<< BUILD_LOG( kPacket_.m_iBalance )
		<< BUILD_LOG( kPacket_.m_iBonus )
		<< BUILD_LOG( kPacket_.m_iHadFilled )
		<< BUILD_LOG( kPacket_.m_iRet )
		<< BUILD_LOG( kPacket_.m_iSubat )
		<< BUILD_LOG( kPacket_.m_iAT )
		<< BUILD_LOG( kPacket_.m_iPoint )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< END_LOG;
	
	KUserList::KGSUserInfo kInfo;
	if( false == GetKLoginSimLayer()->GetGSUserInfoByPublisherUID( kPacket_.m_uiGiantUID, kInfo ) )
	{
		// 시간차로 끊기거나 로그인 서버 크래시
		START_LOG( cerr, L"해당 유저가 로그인 되어 있지 않음" )
			<< BUILD_LOG( kPacket_.m_wstrTID )
			<< BUILD_LOG( kPacket_.m_iBalance )
			<< BUILD_LOG( kPacket_.m_iBonus )
			<< BUILD_LOG( kPacket_.m_iHadFilled )
			<< BUILD_LOG( kPacket_.m_iRet )
			<< BUILD_LOG( kPacket_.m_iSubat )
			<< BUILD_LOG( kPacket_.m_iAT )
			<< BUILD_LOG( kPacket_.m_iPoint )
			<< BUILD_LOG( kPacket_.m_uiGiantUID )
			<< END_LOG;

		return;
	}

	switch( kPacket_.m_iAT )
	{
	case KGiantBillingPacket::GB_RT_BALLANCE:
		{
			START_LOG(clog, L"자이언트 캐시 잔액 조회 결과")
				<< BUILD_LOG( kPacket_.m_iRet )
				<< END_LOG;

			KEPUBLISHER_BILLING_BALANCE_ACK kPacket;
			kPacket.m_iOK = CheckReturn(kPacket_.m_iRet);
			kPacket.m_ulBalance = ( unsigned long )kPacket_.m_iBalance;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
			// CN의 경우 통신으로 받는 캐시를 퍼블리셔 캐쉬로 본다.
			kPacket.m_iCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

			SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, EPUBLISHER_BILLING_BALANCE_ACK, kPacket );
		}
		break;
	case KGiantBillingPacket::GB_RT_EXCHANGE_POINT:
		{
			KEBILL_EXCHANGE_CASH_REQ kPacket;
			kPacket.m_iRet = kPacket_.m_iRet;
			kPacket.m_wstrTransaction = kPacket_.m_wstrTID;
			kPacket.m_iUserUID = kInfo.m_nUserUID;
			kPacket.m_iGSUID = kInfo.m_nGSUID;
			kPacket.m_iPoint = kPacket_.m_iPoint;
			
			SendToKOGBillingDB( EBILL_EXCHANGE_CASH_REQ, kPacket );
		}
		break;
	case KGiantBillingPacket::GB_RT_USE_ITEM_CARD:
		{
			KEBILL_USE_COUPON_RESULT_REQ kPacket;
			kPacket.m_iRet = kPacket_.m_iRet;
			kPacket.m_wstrTransaction = kPacket_.m_wstrTID;
			kPacket.m_iUserUID = kInfo.m_nUserUID;
			kPacket.m_iGSUID = kInfo.m_nGSUID;
			kPacket.m_iChargeItem = kPacket_.m_iSubat;
			kPacket.m_iChargeCash = kPacket_.m_iPoint;
			kPacket.m_iChargeBonus = kPacket_.m_iBonus;
			SendToKOGBillingDB( EBILL_USE_COUPON_RESULT_REQ, kPacket );
		}
		break;
//	case KGiantBillingPacket::GB_RT_USE_ZONE_CARD:
//		break;
//	case KGiantBillingPacket::GB_RT_RECHARGE_MONTH_CARD:
//		break;
	default:
		START_LOG( cerr, L"패킷 타입 이상." )
			<< BUILD_LOG( kPacket_.m_wstrTID )
			<< BUILD_LOG( kPacket_.m_iBalance )
			<< BUILD_LOG( kPacket_.m_iBonus )
			<< BUILD_LOG( kPacket_.m_iHadFilled )
			<< BUILD_LOG( kPacket_.m_iRet )
			<< BUILD_LOG( kPacket_.m_iSubat )
			<< BUILD_LOG( kPacket_.m_iAT )
			<< BUILD_LOG( kPacket_.m_iPoint )
			<< BUILD_LOG( kPacket_.m_uiGiantUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ )
{
	START_LOG( clog, L"UserServer - EPUBLISHER_BILLING_BALANCE_REQ" )
		<< BUILD_LOG( kPacket_.m_uiPublisherUID )
		<< BUILD_LOG( kPacket_.m_wstrAccount )
		<< BUILD_LOG( kPacket_.m_wstrTID )
		<< BUILD_LOG( kPacket_.m_iAT )
		<< END_LOG;

	boost::shared_ptr< KGiantBillingPacket > spPacket( new KGiantBillingPacket );
	spPacket->Write( kPacket_ );
	SiKGiantBillingManager()->QueueingSendPacket( spPacket );
}

// 충전 카드는 필요시에 재구현해야함
/*
IMPL_ON_FUNC( EGIANT_BILLING_USE_CARD_CMD )
{
	boost::shared_ptr< KGiantBillingPacket > spPacket( new KGiantBillingPacket );
	spPacket->Write( kPacket_ );
	SiKGiantBillingManager()->QueueingSendPacket( spPacket );
}	

IMPL_ON_FUNC( EGIANT_BILLING_USE_CARD_RET_CMD )
{
	UidType iActorUID = SiKGiantBillingManager()->UnregisterUserIDUserUID( kPacket_.m_wstrAccount );

	UidType anTrace[2] = { iActorUID, -1 };
	KncSend( PI_NULL, 0, PI_LOGIN_USER, 0, anTrace, EGIANT_BILLING_USE_CARD_RET_CMD, kPacket_ );
}
}*/

IMPL_ON_FUNC( EGIANT_BILLING_PCARD_CMD )
{
	boost::shared_ptr< KGiantBillingPacket > spPacket( new KGiantBillingPacket );
	spPacket->Write( kPacket_ );
	SiKGiantCouponManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( EGIANT_BILLING_SCARD_CMD )
{
	boost::shared_ptr< KGiantBillingPacket > spPacket( new KGiantBillingPacket );
	spPacket->Write( kPacket_ );
	SiKGiantCouponManager()->QueueingSendPacket( spPacket );
}

int KGiantBilling::CheckReturn(int iRet)
{
	switch(iRet)
	{
	case 0:
		return NetError::NET_OK;
	case -1:
		return NetError::ERR_GIANT_BILLING_09;
	case -2:
		return NetError::ERR_GIANT_BILLING_01;
	case -3:
		return NetError::ERR_GIANT_BILLING_02;
	case -4:
		return NetError::ERR_GIANT_BILLING_03;
	case -5:
		return NetError::ERR_GIANT_BILLING_04;
	case -6:
		return NetError::ERR_GIANT_BILLING_05;
	case -7:
		return NetError::ERR_GIANT_BILLING_06;
	case -8:
		return NetError::ERR_GIANT_BILLING_07;
	case -9:
		return NetError::ERR_GIANT_BILLING_08;
	}

	return NetError::ERR_UNKNOWN;
}
//}}

#endif // SERV_COUNTRY_CN