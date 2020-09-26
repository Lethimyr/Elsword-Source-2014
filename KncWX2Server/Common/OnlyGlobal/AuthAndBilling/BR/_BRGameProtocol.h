#ifndef __GAME_PROTOCOL_H__
#define __GAME_PROTOCOL_H__


#define GTX_PK_TYPE_GETBALANCE					10	// 잔액조회 요청/응답
#define GTX_PK_TYPE_PURCHASEITEM				20	// 아이템구매 

// GTX 프로토콜
#pragma pack( push , 1)

// 잔액조회 구조체
typedef struct _BILL_PACK_BALANCE
{
	WORD    ReqLen;
	WORD	ReqType;
	WORD	RetCode;

	__int64 GUserNo;
	char	GUserID[50+1];
	DWORD	CashRemain;

} BILL_PACK_BALANCE, *PBILL_PACK_BALANCE;

// 아이템 구매 구조체
typedef struct _BILL_PACK_BUY
{
	WORD	ReqLen;
	WORD	ReqType;
	WORD	RetCode;

	__int64 GUserNo;
	char	GUserID[50+1];

	DWORD	CashRemain;
	char	ChargeNos[512+1];

	char	ItemIDs[256+1];
	char	ItemNames[2048+1];
	char    ItemUnitPrices[256+1];
} BILL_PACK_BUY, *PBILL_PACK_BUY;

#pragma pack(pop)

#endif
