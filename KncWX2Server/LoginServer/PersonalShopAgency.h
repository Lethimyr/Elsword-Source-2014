#pragma once

#include "LoginBaseRoom.h"
#include "FSM/support_FSM.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"
#include <boost/timer.hpp>

#include "Room/PersonalShopUserManager.h"
#include "PersonalShopItemManager.h"
#include "X2Data/XSLSquareUnit.h"

//{{ 2011. 04. 13	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY


SmartPointer( KPersonalShopAgency );


class KPersonalShopAgency : public KLoginBaseRoom
{
	NiDeclareRTTI;
	DeclToStringW;
public:
	enum ENUM_TIMER
	{
		TM_WAIT_OTHER,
		TM_CHECK_EXPIRATION,
		//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
		TM_CHECK_ZOMBIE_USER,
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
		//}}
		TM_TOTAL_NUM,
	};

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
#else
	enum PSHOP_AGENCY_ENUM
	{
		PAE_SELL_ITEM_LIMIT_NUM = 9,
	};
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
	
public:
	KPersonalShopAgency(void);
	virtual ~KPersonalShopAgency(void);

	virtual void CloseRoom();
	virtual void Tick();

	bool IsEmpty();
	bool IsFull();

protected:
	//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
    void CheckZombieUser();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}

protected:
	bool BreakPersonalShop( IN const int iReason, IN const UidType iHostGSUID, IN const UidType iHostUID );

	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer

	//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
	template < class T > void BroadCastSameGroupGS( unsigned short usEventID, const T& data );
#endif SERV_INTEGRATION
	//}}
	template < class T > void BroadCast( u_short usEventID, const T& data );
	template < class T > void BroadCastWithOutOneChar( u_short usEventID, const T& data, const UidType iUnitUID );
	void BroadCastID( u_short usEventID );
	void _BroadCast( const KEvent& kEvent );
	void _BroadCastWithOutOneChar( const KEvent& kEvent, const UidType iUnitUID );

#ifdef DEF_TRADE_BOARD
	//## 아이템 리스트 전송.
	//# 추가 : 상점 개설시 아이템 등록완료
	//# 삭제 : 상점 종료, 아이템별 판매완료
	//## 함수인자
	//# NerError::ERR_PERSONAL_SHOP_26 : 아이템 리스트 추가
	//# NerError::ERR_PERSONAL_SHOP_27 : 아이템 리스트 수정
	//# NerError::ERR_PERSONAL_SHOP_28 : 아이템 리스트 삭제
	void SendPShopListInfo( IN const int iCode, IN const std::vector< KSellPersonalShopItemInfo > & veckInfo, IN const UidType iGSUID = 0 );
	void SendPShopListInfo( IN const int iCode, IN const KSellPersonalShopItemInfo& kInfo, IN const UidType iGSUID = 0 );
#endif DEF_TRADE_BOARD

	void SendPShopAgencyMessage( IN const KEGS_PSHOP_AGENCY_MESSAGE_NOT::PSHOP_AGENCY_MESSAGE_TYPE eType, IN const int iItemID = 0, IN const int iQuantity = 0, IN const int iSellED = 0 );

	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	void SendToGameDB( unsigned short usEventID );

	//////////////////////////////////////////////////////////////////////////
	// event handler
   _DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );

	DECL_ON_FUNC( ERM_LOAD_PSHOP_BOARD_INFO_NOT );

	DECL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_REQ );											// 대리상인 개설 요청
   _DECL_ON_FUNC( DBE_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );				// 대리상인 개설 요청 DB등록   

	DECL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT );								// 서버가 생성하는 대리상인 개설	

	DECL_ON_FUNC_NOPARAM( ERM_CHECK_OPEN_PSHOP_AGENCY_REQ );							// 대리상점 상태 확인

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	_DECL_ON_FUNC( ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_REQ, KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

   _DECL_ON_FUNC( ERM_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_REQ );		// 대리상인 판매 아이템 등록
   _DECL_ON_FUNC( DBE_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

   _DECL_ON_FUNC( ERM_JOIN_MY_PSHOP_AGENCY_REQ, KERM_JOIN_PERSONAL_SHOP_REQ );			// 대리상인 개설자의 입장
	DECL_ON_FUNC( ERM_JOIN_PERSONAL_SHOP_REQ );											// 일반 유저의 입장

   _DECL_ON_FUNC( ERM_LEAVE_PSHOP_AGENCY_REQ, KERM_LEAVE_PERSONAL_SHOP_REQ );			// 개설자가 직접 방 이탈 또는 서버에 의한 대리상점 방 이탈
	DECL_ON_FUNC( ERM_LEAVE_PERSONAL_SHOP_REQ );										// 구매자 대리상점 방 이탈

	DECL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_REQ );
   _DECL_ON_FUNC( DBE_BUY_PSHOP_AGENCY_ITEM_ACK, KDBE_BUY_PSHOP_AGENCY_ITEM_REQ );

	DECL_ON_FUNC_NOPARAM( ERM_STOP_SALE_PSHOP_AGENCY_REQ );
	DECL_ON_FUNC( DBE_STOP_SALE_PSHOP_AGENCY_ACK );

	DECL_ON_FUNC( ERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT );

   _DECL_ON_FUNC( ERM_PICK_UP_FROM_PSHOP_AGENCY_REQ, KEGS_PICK_UP_FROM_PSHOP_AGENCY_REQ );
	DECL_ON_FUNC( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK );
	DECL_ON_FUNC( DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK );

	//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	DECL_ON_FUNC( ERM_CHECK_ZOMBIE_USER_ACK );
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}
    DECL_ON_FUNC( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ );

protected:
	KPersonalShopUserManager						m_kPersonalShopUserManager;
	KPersonalShopItemManager						m_kPersonalShopItemManager;
	boost::timer									m_kTimer[TM_TOTAL_NUM];

	std::wstring									m_wstrPersonalShopName;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
	SEnum::AGENCY_SHOP_TYPE				            m_PersonalShopType;
#else //SERV_UPGRADE_TRADE_SYSTEM
	CXSLSquareUnit::PERSONAL_SHOP_TYPE				m_PersonalShopType;
#endif //SERV_UPGRADE_TRADE_SYSTEM
	
	std::wstring									m_wstrAgencyExpirationDate;
	CTime											m_tAgencyExpirationDate;
	CTime											m_tAgencyOpenDate;
	CTime											m_tAgencyOpenOver24Hours;
};

//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
template < class T >
void KPersonalShopAgency::BroadCastSameGroupGS( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_LOGIN_ROOM, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER_SAME, 0, anTrace, usEventID, data );
}
#endif SERV_INTEGRATION
//}}

template< class T >
void KPersonalShopAgency::BroadCast( u_short usEventID, const T& data )
{
	KEvent kEvent;  // _BroadCast에서 clone할 것이므로 여기선 heap 아니어도 됨.
	kEvent.m_kDestPerformer.m_dwPerformerID = PI_GS_CHARACTER;
	kEvent.m_usEventID = usEventID;

	KSerializer ks;
	kEvent.m_kbuff.Clear();

	// serialize - only data
	ks.BeginWriting( &kEvent.m_kbuff );
	ks.Put( data );
	ks.EndWriting();

	_BroadCast( kEvent );
}

template < class T >
void KPersonalShopAgency::BroadCastWithOutOneChar( u_short usEventID, const T& data, const UidType iUnitUID )
{
	KEvent kEvent;  // _BroadCast에서 clone할 것이므로 여기선 heap 아니어도 됨.
	kEvent.m_kDestPerformer.m_dwPerformerID = PI_GS_CHARACTER;
	kEvent.m_usEventID = usEventID;

	KSerializer ks;
	kEvent.m_kbuff.Clear();

	// serialize - only data
	ks.BeginWriting( &kEvent.m_kbuff );
	ks.Put( data );
	ks.EndWriting();

	_BroadCastWithOutOneChar( kEvent, iUnitUID );
}

template < class T >
void KPersonalShopAgency::SendToGameDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_LOGIN_ROOM, GetUID(), PC_GAME_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KPersonalShopAgency::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_ROOM, GetUID(), PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}


#endif SERV_PSHOP_AGENCY
//}}


