#ifndef __GAME_PROTOCOL_H__
#define __GAME_PROTOCOL_H__

/**--------------------------------------------------------------
FileName        : _GameProtocol.h
Description     : 
	이파일은 게임서버와 통신할 게임아이템 과금 프로토콜을 정의한다.
	아래 정의된 Packet 타입과 Structure는 해당 게임사와 충분히 협의
	하여 각 게임별로 정의하여야 한다.
Caution         : 
	게임프로토콜 정의시에 Structure Header에 해당하는 부분에 패킷길이
	패킷타입, 응답코드를 순서대로 정의하고 short 타입으로 정의한다.
	패킷길이 : short   ReqLen;		
	패킷타입 : short	ReqType;	
	응답코드 : short	RetCode;

Copyright 2001-2008 by PayLetter Inc. All rights reserved.
Author          : soohyunk@payletter.com, 2008-06-25
Modify History  : 
---------------------------------------------------------------*/

// 게임서버/빌링GTX서버 상호 명령어 정의
// Packet Type 정의
#define GTX_PK_TYPE_GETBALANCE					10	// 잔액조회 요청/응답
#define GTX_PK_TYPE_INSPOLICYEVENT				12	// 정책이벤트캐시 지급 요청/응답
#define GTX_PK_TYPE_PURCHASEITEM				20	// 아이템구매 
#define GTX_PK_TYPE_GIFTITEM					21	// 아이템선물 
#define GTX_PK_TYPE_CNLPURCHASE					22	// 아이템구매 취소
#define GTX_PK_TYPE_REGCOUPON					30	// 쿠폰 등록
#define GTX_PK_TYPE_CNLCOUPON					31	// 쿠폰 등록 취소
#define GTX_PK_TYPE_HEALTH_CHECK				40	// GTX Health Check

// GTX 프로토콜
#pragma pack(1)

// Health Check 구조체
typedef struct _GTX_PK_HEALTH_CHECK
{
	WORD	ReqLen;		// IN	1
	WORD	ReqType;	// IN	2
	DWORD	ReqKey;		// IN	3

	WORD	RetCode;	// OUT	4
} GTX_PK_HEALTH_CHECK, *PGTX_PK_HEALTH_CHECK;

// 잔액조회 구조체
typedef struct _GTX_PK_GETBALANCE
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	char    UserID[50+1];		// IN   6
	
	DWORD	RealCash;			// OUT	7
	DWORD	BonusCash;			// OUT	8
	char	RetMsg[256+1];		// OUT  9
} GTX_PK_GETBALANCE, *PGTX_PK_GETBALANCE;

// 정책이벤트캐시 지급 구조체
typedef struct _GTX_PK_INSPOLICYEVENT
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	char	UserID[50+1];		// IN   6
	DWORD	ClientIP;			// IN	7
    DWORD	CashAmt;			// IN	8
	DWORD	EventID;			// IN   9
	char	ExpireYMD[8+1];		// IN	10
	
	DWORD	RealCash;			// OUT	11
	DWORD	BonusCash;			// OUT	12
	DWORD	RetCashAmt;			// OUT	13
	char	RetExpireYMD[8+1];	// OUT	14
	char	RetMsg[256+1];		// OUT  15
} GTX_PK_INSPOLICYEVENT, *PGTX_PK_INSPOLICYEVENT;

// 아이템 구매 구조체
typedef struct _GTX_PK_PURCHASEITEM
{
	WORD	ReqLen;					// IN	1
	WORD	ReqType;				// IN	2
	DWORD	ReqKey;					// IN	3
	WORD	RetCode;				// OUT	4

    DWORD	UserNo;					// IN	5
	DWORD	ClientIP;				// IN	6
	char	ItemID[256+1];			// IN	7
	char	ItemCnt[128+1];			// IN	8
	char	ItemUnitPrice[256+1];	// IN	9
	WORD	GameServerNo;			// IN	10
	DWORD	WorldNo;				// IN	11
	char	UserID[50+1];			// IN	12
    char    CharacterID[50+1];		// IN	13
    char    StatProperty1[50+1];	// IN	14
    char    StatProperty2[50+1];	// IN	15
    char    StatProperty3[50+1];	// IN	16

	DWORD	RealCash;				// OUT	17
	DWORD	BonusCash;				// OUT	18
	DWORD	ChargedCashAmt;			// OUT	19
	char	ChargeNo[512+1];		// OUT	20
	char	EventItemID[256+1];		// OUT  21
	char	EventChargeNo[512+1];	// OUT  22
	char	RetMsg[256+1];			// OUT  23
} GTX_PK_PURCHASEITEM, *PGTX_PK_PURCHASEITEM;

// 아이템 선물 구조체
typedef struct _GTX_PK_GIFTITEM
{
	WORD	ReqLen;					// IN	1
	WORD	ReqType;				// IN	2
	DWORD	ReqKey;					// IN	3
	WORD	RetCode;				// OUT	4

    DWORD	UserNo;					// IN	5
	DWORD	RUserNo;				// IN	6
	DWORD	ClientIP;				// IN	7
	char	ItemID[256+1];			// IN	8
	char	ItemCnt[128+1];			// IN	9
	char	ItemUnitPrice[256+1];	// IN	10
	WORD	GameServerNo;			// IN	11
	DWORD	WorldNo;				// IN	12
	char	UserID[50+1];			// IN	13
    char    CharacterID[50+1];		// IN	14
	char    RUserID[50+1];			// IN	15
	char    RCharacterID[50+1];		// IN	16
    char    StatProperty1[50+1];	// IN	17
    char    StatProperty2[50+1];	// IN	18
    char    StatProperty3[50+1];	// IN	19

	DWORD	RealCash;				// OUT	20
	DWORD	BonusCash;				// OUT	21
	DWORD	ChargedCashAmt;			// OUT	22
	char	ChargeNo[512+1];		// OUT	23
	char	EventItemID[256+1];		// OUT  24
	char	EventChargeNo[512+1];	// OUT  25
	char    RetMsg[256+1];			// OUT  26
} GTX_PK_GIFTITEM, *PGTX_PK_GIFTITEM;

// 구매취소 구조체
typedef struct _GTX_PK_CNLPURCHASE
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	char	UserID[50+1];		// IN	6
	char	ChargeNo[512+1];	// IN	7

	DWORD	RealCash;			// OUT	8
	DWORD	BonusCash;			// OUT	9
	DWORD	CanceledCashAmt;	// OUT	10
	char	RetMsg[256+1];		// OUT  11
} GTX_PK_CNLPURCHASE, *PGTX_PK_CNLPURCHASE;

// 쿠폰등록 구조체
typedef struct _GTX_PK_REGCOUPON
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	DWORD	ClientIP;			// IN	6
	WORD	GameServerNo;		// IN	7
	DWORD	WorldNo;			// IN	8
	char	UserID[50+1];		// IN	9
    char    CharacterID[50+1];	// IN	10
	char	CouponNo[19+1];		// IN	11
	char    StatProperty1[50+1];// IN	12
    char    StatProperty2[50+1];// IN	13
    char    StatProperty3[50+1];// IN	14

	DWORD	RealCash;			// OUT	15
	DWORD	BonusCash;			// OUT	16
	char	ItemID[256+1];		// OUT	17
	char	ItemCnt[128+1];			// OUT	18
	char	ChargeNo[512+1];		// OUT	19
	char	RetMsg[256+1];		// OUT	20
} GTX_PK_REGCOUPON, *PGTX_PK_REGCOUPON;

// 쿠폰 등록 취소 구조체
typedef struct _GTX_PK_CNLCOUPON
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	char	UserID[50+1];		// IN   6
	char	CouponNo[19+1];		// IN	7

	char	RetMsg[256+1];		// OUT	8
} GTX_PK_CNLCOUPON, *PGTX_PK_CNLCOUPON;

#define GTX_PK_HEALTH_CHECK_SZ				sizeof(GTX_PK_HEALTH_CHECK)
#define GTX_PK_GETBALANCE_SZ				sizeof(GTX_PK_GETBALANCE)
#define GTX_PK_INSPOLICYEVENT_SZ			sizeof(GTX_PK_INSPOLICYEVENT)
#define GTX_PK_PURCHASEITEM_SZ				sizeof(GTX_PK_PURCHASEITEM)
#define GTX_PK_GIFTITEM_SZ					sizeof(GTX_PK_GIFTITEM)
#define GTX_PK_CNLPURCHASE_SZ				sizeof(GTX_PK_CNLPURCHASE)
#define GTX_PK_REGCOUPON_SZ					sizeof(GTX_PK_REGCOUPON)
#define GTX_PK_CNLCOUPON_SZ					sizeof(GTX_PK_CNLCOUPON)

#endif
