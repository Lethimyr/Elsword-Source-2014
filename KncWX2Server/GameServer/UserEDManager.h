#pragma once
#include "CacheData.h"


//{{ 2012. 09. 28	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM


class KUserEDManager
{
public:
	enum ED_REASON
	{
		ER_NO_REASON = 0,

		// set
		ER_SET_ED_SELECT_UNIT,							// 캐릭터 선택시 설정되는 ED값
		ER_SET_ED_SYNC_FOR_SEND_LETTER,					// 우편 전송을 위해 동기화 하는 ED값
		ER_SET_ED_SYNC_FOR_SEND_LEETER_ROLLBACK,		// 우편 전송을 위해 동기화 하는 ED 롤백 값
		ER_SET_ED_SEND_LETTER_ATTACH_ROLLBACK,			// 우편 전송 실패로 인한 첨부 ED 롤백 값
		ER_SET_ED_PERSONAL_SHOP_SELL_ITEM,				// 개인 상점 아이템 판매
		ER_SET_ED_PERSONAL_SHOP_BUY_ITEM,				// 개인 상점 아이템 구매
		ER_SET_ED_PSHOP_AGENCY_SELL_ITEM,				// 대리 상점 아이템 판매
		ER_SET_ED_PSHOP_AGENCY_BUY_ITEM,				// 대리 상점 아이템 구매
		ER_SET_ED_IN_TRADE,								// 개인거래로 받는 ED
		ER_SET_ED_OUT_TRADE,							// 개인거래로 나가는 ED

		// in
		ER_IN_ED_DUNGEON_DROP,							// 던전 ED드롭
		ER_IN_ED_SELL_ITEM_NPC_SHOP,					// NPC상점에 아이템 되팔기
		ER_IN_ED_DUNGEON_CLEAR_BONUS,					// 던전 클리어 보너스 ED
		ER_IN_ED_BUY_ITEM_NPC_SHOP_ROLLBACK,			// NPC상점에서 아이템 구매 실패시 ED 롤백 값
		ER_IN_ED_REG_GUILD_AD_COMMISSION_ROLLBACK,
		ER_IN_ED_MODIFY_GUILD_AD_COMMISSION_ROLLBACK,
		ER_IN_ED_SEND_LETTER_COMMISSION_ROLLBACK,
		ER_IN_ED_SEND_LETTER_ATTACH_ROLLBACK,
		ER_IN_ED_SYNC_FOR_SEND_LEETER_ROLLBACK,
		ER_IN_ED_GET_ED_FROM_LETTER,
		ER_IN_ED_PERSONAL_SHOP_SELL_ITEM,
		ER_IN_ED_PSHOP_AGENCY_SELL_ITEM,		
		ER_IN_ED_TRAINING_REWARD,
		ER_IN_ED_QUEST_REWARD,

		// out
		ER_OUT_ED_BUY_ITEM_NPC_SHOP,
		ER_OUT_ED_ITEM_REPAIR,
		ER_OUT_ED_ITEM_MANUFACTURE,
		ER_OUT_ED_ITEM_ENCHANT,
		ER_OUT_ED_ITEM_SOCKET,
		ER_OUT_ED_ITEM_ATTRIBUTE,
		ER_OUT_ED_SEND_LETTER_COMMISSION,
		ER_OUT_ED_SEND_LETTER_ATTACH,
		ER_OUT_ED_REG_GUILD_AD_COMMISSION,
		ER_OUT_ED_MODIFY_GUILD_AD_COMMISSION,
		ER_OUT_ED_PERSONAL_SHOP_OPEN_COMMISSION,
		ER_OUT_ED_PERSONAL_SHOP_BUY_ITEM,
		ER_OUT_ED_PSHOP_AGENCY_BUY_ITEM,
		ER_OUT_ED_USE_COBO_EXPRESS_TICKET,
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
		ER_OUT_ED_USE_CUBE_OPEN,
//#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
//#ifdef SERV_RELATIONSHIP_SYSTEM
		ER_OUT_ED_BREAK_UP_RELATIONSHIP,
//#endif SERV_RELATIONSHIP_SYSTEM
		//}
		ER_MAX,
	};

public:
	KUserEDManager(void);
	~KUserEDManager(void);

	// 초기화
	void	Clear();
	void	ClearStatistics();
	void	Init( IN const int iED, IN const ED_REASON eReason );

	// 정보 참조
	int		GetED() const			{ return m_iED; }
	int		GetChangeValue() const	{ return m_iED.GetChangeValue(); }
	const std::map< int, __int64 >& GetEDStatistics() { return m_mapEdStatistics; }

	// DB업데이트
	void	GetDBUpdateInfo( OUT int& iED );
	void	RollBackDBUpdateInfo( IN const int iED );

	// ED증가 & 감소
	bool	IncreaseMyED( IN const int iIncreaseED, IN const ED_REASON eReason );
	bool	DecreaseMyED( IN const int iDecreaseED, IN const ED_REASON eReason );

private:
	void	AddEdStatistics( IN const int iED, IN const ED_REASON eReason );

private:
	KCacheDataMaxLimit< int, __int64 >	m_iED;
	std::map< int, __int64 >			m_mapEdStatistics;
};


#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}