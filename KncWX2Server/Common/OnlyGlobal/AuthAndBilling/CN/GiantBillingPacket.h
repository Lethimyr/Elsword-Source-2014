#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"
#include "GiantCommonPacket.h"

#ifdef SERV_COUNTRY_CN

SmartPointer( KGiantBillingPacket );

class KGiantBillingPacket : public KGiantCommonPacket
{
public:
	enum ENUM_BILLING_COMMAND_TYPE 
	{ 
		GB_BCT_CONSUME						= 2,
		GB_BCT_BALANCE						= 4,
		GB_BCT_RETURN						= 5,
		GB_BCT_CARD							= 6,
		//GB_BCT_USE_CARD					= 9,	// 충전 카드 사용 (사용안함)
	};

	enum ENUM_BILLING_PARACOMMAND_TYPE 
	{ 
		GB_PCT_USE_CARD_REQ					= 3,
		GB_PCT_USE_CARD_ACK					= 4,
		GB_PCT_PCARD						= 6,
		GB_PCT_SCARD						= 7,
	};

// KEBILL_USE_COUPON_REQ 구조체로 해당 enum 값 이동 시킴
// 	enum ENUM_RETURN_CODE_TYPE 
// 	{
// 		GB_RCT_SUCCESS						= 0,	// 성공
// 		GB_RCT_FAIL							= -1,	// 실패
// 		GB_RCT_ACCOUNT_NOT_EXIST			= -2,	// 유저 부재
// 		GB_RCT_CASH_NOT_EXIST				= -3,	// 잔액 부족		
// 		GB_RCT_ITEM_CARD_NOT_EXIST			= -4,	// 아이템 카드 부재
// 		GB_RCT_ITEM_CARD_NOT_AREA			= -5,	// 해당 아이템 카드로 해당 구역에서 사용 불가
// 		GB_RCT_ITEM_CARD_DUPLICATE			= -6,	// 매트릭스 카드 바인딩
// 		GB_RCT_ZONE_CARD_ID_ERROR			= -7,	// 전용 카드 ID에러
// 		GB_RCT_ZONE_CARD_PASSWORD_ERROR		= -8,	// 전용 카드 패스워드 에러
// 		GB_RCT_DB_ERROR						= -9,	// DB에러
// 	};

	enum ENUM_REQUEST_TYPE 
	{
		GB_RT_RECHARGE						= 0,	// 충전 요청
		GB_RT_EXCHANGE_POINT				= 1,	// 포인트 교환 요청
		//GB_RT_USE_ZONE_CARD				= 2,	// 전용 카드 사용 요청
		GB_RT_BALLANCE						= 3,	// 잔고 체크 요청
		//GB_RT_RECHARGE_MONTH_CARD			= 4,	// 월간 카드 충전 요청
		GB_RT_USE_ITEM_CARD					= 5,	// 아이템 카드 사용 요청
		//GB_RT_EXCHANGE_POINT_FUND			= 6,	// ??
		//GB_RT_EXCHANGE_POINT_INSURANCE	= 7,	// ??
		//GB_RT_EXCHANGE_POINT_SECOND		= 8,	// ??
		//GB_RT_EXCHANGE_POINT_MONTH		= 9,	// ??
	};


public:
	KGiantBillingPacket();
	virtual ~KGiantBillingPacket();

	// 상속에 따른 오버로딩 재정의
	using KGiantCommonPacket::Read;
	using KGiantCommonPacket::Write;

	//{{ 2011. 02. 28	최육사	중국 자이언트 빌링
	//virtual bool Read( KEGIANT_BILLING_USE_CARD_CMD& kPacket );
	//virtual bool Write( const KEGIANT_BILLING_USE_CARD_CMD& kPacket );

	//virtual bool Read( KEGIANT_BILLING_USE_CARD_RET_CMD& kPacket );
	//virtual bool Write( const KEGIANT_BILLING_USE_CARD_RET_CMD& kPacket );

	virtual bool Read( KEPUBLISHER_BILLING_BALANCE_REQ& kPacket );
	virtual bool Write( const KEPUBLISHER_BILLING_BALANCE_REQ& kPacket );	

	virtual bool Read( KEGIANT_BILLING_CONSUME_REQ& kPacket );
	virtual bool Write( const KEGIANT_BILLING_CONSUME_REQ& kPacket );

	virtual bool Read( KEGIANT_BILLING_PCARD_CMD& kPacket );
	virtual bool Write( const KEGIANT_BILLING_PCARD_CMD& kPacket );

	virtual bool Read( KEGIANT_BILLING_SCARD_CMD& kPacket );
	virtual bool Write( const KEGIANT_BILLING_SCARD_CMD& kPacket );

	virtual bool Read( KEGIANT_BILLING_RETURN& kPacket );
	virtual bool Write( const KEGIANT_BILLING_RETURN& kPacket );
	//}}
};


#endif // SERV_COUNTRY_CN