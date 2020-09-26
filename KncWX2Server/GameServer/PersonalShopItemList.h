#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "X2Data/XSLItem.h"


//{{ 2010. 03. 24  최육사	define처리
#ifdef DEF_TRADE_BOARD


class KPersonalShopItemList
{
	DeclareSingleton( KPersonalShopItemList );
	DeclDump;
	DeclToStringW;

	enum UNIT_TYPE_SORT_INDEX
	{
		UTSI_ELSWORD = 1,
		UTSI_LENA,
		UTSI_AISHA,
		UTSI_RAVEN,
		UTSI_EVE,
		UTSI_CHUNG,
#ifdef SERV_ARA_MARTIAL_ARTIST
		UTSI_ARA,
#endif
#ifdef SERV_NEW_CHARACTER_EL
		UTSI_ELESIS,
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
		UTSI_NEW_CHARACTER,
#endif //SERV_9TH_NEW_CHARACTER
	};

public:
	KPersonalShopItemList(void);
	~KPersonalShopItemList(void);

	bool	AddItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & kNot );
	bool	DelItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & kNot );
	bool	DelItemInfo( const UidType & iCenterUID );
	bool	ChangeItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & kNot );

	void	GetSearchList( IN KEGS_SEARCH_TRADE_BOARD_REQ & kReq, OUT KEGS_SEARCH_TRADE_BOARD_ACK & kAck );
    void    GetStrictSearchList( IN KEGS_SEARCH_TRADE_BOARD_REQ & kReq, OUT KEGS_SEARCH_TRADE_BOARD_ACK & kAck );

private:
	//## Inv 함수는 기획서에서 의도한 키값으로 재정하는 함수
	//#  : 코드키값과 기획의도 키값이 상충하여 상하관계가 맞지 않아
	//#    키값 비교시 불편함이 있어 작성하는 Invert 함수
	//#  Ex. SORT_TYPE::ST_AVATRA == 7  =>  return 1;
    char	InvSlotCategoryID( KSellPersonalShopItemInfo & kInfo );
	char	InvSlotCategoryID( char cID );
	char	InvEqipPosID( const CXSLItem::ItemTemplet* pInfo );
	char	InvEqipPosID( char cID );
	char	InvUnitTypeID( const CXSLItem::ItemTemplet* pInfo );
	char	InvUnitTypeID( char cID );
	char	InvItemGradeID( const CXSLItem::ItemTemplet* pInfo );
	char	InvItemGradeID( char cID );

	bool	CheckSlotCategoryID( char cID );

private:
	std::map< char, std::list<KTradeBoardItemInfo> > m_mapItemInfo;
};

DefSingletonInline( KPersonalShopItemList );


#endif DEF_TRADE_BOARD
//}}