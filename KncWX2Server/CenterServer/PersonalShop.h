#pragma once

#include "CnBaseRoom.h"
#include "FSM/support_FSM.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"
#include <boost/timer.hpp>

#include "Room/PersonalShopUserManager.h"
#include "X2Data/XSLSquareUnit.h"

SmartPointer( KPersonalShop );

#define PS_OPENING_ED	1000	//임시로 사용될 상점개설 비용.. 추후 개설에 대한 로직이 정해지면 삭제한다. 
#define TD_COMMISSION	0.03	//거래 게시판 수수료
#define TD_HOST_COMMISSION	0.1	//판매자 수수료(프리미엄 아닐때)

class KPersonalShop : public KCnBaseRoom
{
	NiDeclareRTTI;
	DeclToStringW;

public:
	enum ENUM_TIMER
	{
		TM_WAIT_OTHER,
		TM_TOTAL_NUM,
	};

	KPersonalShop(void);
	virtual ~KPersonalShop(void);

	virtual void CloseRoom();
	virtual void Tick();

	bool IsEmpty();
	bool IsFull();

protected:
	void BreakPersonalShop( int iReason, UidType iHostUID );

	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer
	//{{ 2011. 04. 06	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	virtual void ProcessNativeEvent( const KEventPtr& spEvent ) {}
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
	template < class T > void BroadCastSameGroupGS( unsigned short usEventID, const T& data );
#endif SERV_INTEGRATION
	//}}

	template < class T >
	void BroadCast( u_short usEventID, const T& data );
	void BroadCastID( u_short usEventID );
	void _BroadCast( const KEvent& kEvent );

#ifdef DEF_TRADE_BOARD
	//## 아이템 리스트 전송.
	//# 추가 : 상점 개설시 아이템 등록완료
	//# 삭제 : 상점 종료, 아이템별 판매완료
	//## 함수인자
	//# NerError::ERR_PERSONAL_SHOP_26 : 아이템 리스트 추가
	//# NerError::ERR_PERSONAL_SHOP_27 : 아이템 리스트 수정
	//# NerError::ERR_PERSONAL_SHOP_28 : 아이템 리스트 삭제
	void SendPShopListInfo( int iCode, std::vector<KSellPersonalShopItemInfo> & veckInfo );
	void SendPShopListInfo( int iCode, KSellPersonalShopItemInfo & kInfo );
#endif DEF_TRADE_BOARD

	//////////////////////////////////////////////////////////////////////////
	// event handler
   _DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );
	DECL_ON_FUNC( ERM_OPEN_PERSONAL_SHOP_REQ );
	DECL_ON_FUNC( ERM_REG_PERSONAL_SHOP_ITEM_REQ );
	DECL_ON_FUNC( ERM_JOIN_PERSONAL_SHOP_REQ );
   _DECL_ON_FUNC( ERM_BREAK_PERSONAL_SHOP_REQ, KEGS_BREAK_PERSONAL_SHOP_REQ );	//개설자가 보내거나 참여유저가 보내는 경우는 네트워크 오류로 보낼때만..
    DECL_ON_FUNC( ERM_LEAVE_PERSONAL_SHOP_REQ );	//개설자가 아닐경우 명시적으로 나가기 위해 존재
	DECL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_REQ );
	DECL_ON_FUNC( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK );
	//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	DECL_ON_FUNC( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}

protected:
	KPersonalShopUserManager	m_kPersonalShopUserManager;
	boost::timer				m_kTimer[TM_TOTAL_NUM];

	std::wstring							m_wstrPersonalShopName;
	std::vector<KSellPersonalShopItemInfo>	m_vecSellItemInfo;

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	SEnum::AGENCY_SHOP_TYPE		m_PersonalShopType;
#else //SERV_UPGRADE_TRADE_SYSTEM
	CXSLSquareUnit::PERSONAL_SHOP_TYPE		m_PersonalShopType;
#endif //SERV_UPGRADE_TRADE_SYSTEM
};

//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
template < class T >
void KPersonalShop::BroadCastSameGroupGS( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_ROOM, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER_SAME, 0, anTrace, usEventID, data );
}
#endif SERV_INTEGRATION
//}}

template< class T >
void KPersonalShop::BroadCast( u_short usEventID, const T& data )
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


