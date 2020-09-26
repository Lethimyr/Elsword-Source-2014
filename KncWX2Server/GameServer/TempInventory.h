#pragma once

#include <map>
#include "KncUidType.h"
#include "X2Data/XSLInventory.h"
#include "X2Data/XSLRoom.h"
#include <boost/timer.hpp>

class KTempInventory
{
public:

    KTempInventory();
    virtual ~KTempInventory();

    void	Clear();
	bool	IsEmpty()						{ return m_mapTempInventory.empty(); }
	int		GetNextTempItemUID()			{ return ++m_iTempItemUID; }
	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool	AddTempInventoryItem( IN const int iItemID
								, IN const u_char ucSealData
								//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
								, IN const KItemInfo& kItemInfo
#endif SERV_ENCHANT_ITEM_DROP_EVENT
								//}}
								, OUT KTempInventoryItemInfo& kTempItemInfo
								);
#else
	bool	AddTempInventoryItem( IN int iItemID, IN u_char ucSealData, IN int iDropItemUID, OUT KTempInventoryItemInfo& kTempItemInfo );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
	bool	PrepareTempItemToInventory( IN int iTempItemUID, OUT int& iGetItemID, OUT std::map< int, KItemInfo >& mapGetItem );
	bool	DeleteTempItem( IN int iTempItemUID );
	
protected:
	int											m_iTempItemUID;
	std::map< int, KTempInventoryItemInfo >		m_mapTempInventory;
};

