#include "LoginRMEventProcess.h"
#include "LoginRoomManager.h"

#include "NetError.h"
#include "GameSysVal/GameSysVal.h"
#include "Socket/NetCommon.h"


#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance(); \
	START_LOG( cout, L#className L" 스크립트 갱신을 완료하였습니다!" ); \
	Si##className()->DumpToLogFile(); \
	break; \


ImplPfID( KLoginRMEventProcess, PI_LOGIN_ROOM );

#define CLASS_TYPE  KLoginRMEventProcess

KLoginRMEventProcess::KLoginRMEventProcess()
{
}

KLoginRMEventProcess::~KLoginRMEventProcess()
{
}

//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
#ifdef SERV_MAX_QUEUE_SIZE_DUMP

ImplToStringW( KLoginRMEventProcess )
{
	return START_TOSTRING_PARENTW( KPerformer );
}

#endif SERV_MAX_QUEUE_SIZE_DUMP
//}}

void KLoginRMEventProcess::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	CASE( ERM_OPEN_PSHOP_AGENCY_REQ );
	CASE( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT );
	CASE( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ );

	default:
		START_LOG( cerr, L"핸들러가 지정되지 않은 이벤트." )
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
}

IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_REQ )
{
	UidType iRoomUID = GetKLoginRoomManager()->OpenRoom();
	if( iRoomUID <= 0 )
	{
		KERM_OPEN_PERSONAL_SHOP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_ROOM, anTrace, ERM_OPEN_PSHOP_AGENCY_REQ, kPacket_ );
	GetKLoginRoomManager()->QueueingEventToRoomByRoomUID( iRoomUID, spEvent );
}

IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT )
{
	UidType iRoomUID = GetKLoginRoomManager()->OpenRoom();
	if( iRoomUID <= 0 )
	{
		START_LOG( cerr, L"서버에서 자동으로 대리상점 개설을 하려고하는데 빈 방이 없습니다! 일어나서는 안되는 에러!" )
			<< END_LOG;
		return;
	}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_ROOM, NULL, ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT, kPacket_ );
	GetKLoginRoomManager()->QueueingEventToRoomByRoomUID( iRoomUID, spEvent );
}

IMPL_ON_FUNC( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ )
{
    KERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK kPacket;
    
    KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT kData;
    kData.m_iUserUID = kPacket_.m_iUserUID;
    kData.m_iUnitUID = kPacket_.m_iUnitUID;
    kData.m_wstrNickName = kPacket_.m_wstrNickName;
    kData.m_cPersonalShopType = kPacket_.m_cPersonalShopType;
    kData.m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
    kData.m_wstrAgencyOpenDate = kPacket_.m_wstrAgencyOpenDate;
    kData.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
    kData.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
    kData.m_bOnSale = kPacket_.m_bOnSale;

    
    if( GetKLoginRoomManager()->GetPShopAgencyUIDByUnitUID( kPacket_.m_iUnitUID, kPacket.m_iPShopAgencyUID ) == false )
	{
        // ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT 과 동일.
        UidType iRoomUID = GetKLoginRoomManager()->OpenRoom();
        if( iRoomUID <= 0 )
        {
            START_LOG( cerr, L"서버에서 자동으로 대리상점 개설을 하려고하는데 빈 방이 없습니다! 일어나서는 안되는 에러!" )
                << END_LOG;
            return;
        }

        if ( kData.m_wstrAgencyExpirationDate.empty() )
        {
            START_LOG( cerr, L"서버에서 자동으로 대리상점 개설시도(복원)에서 만료일이 빈 문제 발생 " )
                << BUILD_LOG( kPacket_.m_iUserUID )
                << BUILD_LOG( kPacket_.m_iUnitUID )
                << END_LOG;
            return;
        }

        KEventPtr spEvent( new KEvent );
        spEvent->SetData( PI_LOGIN_ROOM, NULL, ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT, kData );
        GetKLoginRoomManager()->QueueingEventToRoomByRoomUID( iRoomUID, spEvent );

		START_LOG( cerr, L"DB에는 개설되었다고 되어있는데 실제로 대리상점 방은 개설 안되었다? 버그! 또는 예외상황!" )
            << BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

        kPacket.m_iPShopAgencyUID = iRoomUID;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK, kPacket );

		return;
	}

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_LOGIN_ROOM, NULL, ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ, kPacket_ );

    GetKLoginRoomManager()->QueueingEventToRoomByRoomUID( kPacket.m_iPShopAgencyUID, spEvent );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK, kPacket );
}




