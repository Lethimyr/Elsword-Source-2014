#pragma once

class CX2InformerInven : public CX2Informer
{
	public:

		CX2InformerInven(void);
		virtual ~CX2InformerInven(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );

		void Reset();
		bool CheckNewItem();
#ifdef NEW_ITEM_NOTICE
		void SetNewQuickSlotItem(bool bVal, UINT uiSlotID = 0){ m_bNewQuickSlotItem = bVal; m_uiSlotID = uiSlotID; }
#endif //NEW_ITEM_NOTICE

	protected:

		void SetNowInfo();

		bool IsNowInfoChange();
		bool CheckEnoughInven();		
			

		void SetShow( bool bShow );

		bool IsEnoughAllInvenSlot();

		map < CX2Inventory::SORT_TYPE, int > m_mapNowInvenSizeForEnough;
#ifdef MODIFY_INFORMER_INVEN
		set < int > m_setNowInvenForNewItem;			//ItemID
#else
		map < int, int > m_mapNowInvenForNewItem;		//ItemID, 수량
#endif //MODIFY_INFORMER_INVEN

		bool	m_bEnoughInven;
		bool	m_bNewItem;

#ifdef NEW_ITEM_NOTICE
		bool	m_bNewQuickSlotItem;		//퀵슬롯에 자동으로 들어가는 아이템에 대한 판단
		UINT	m_uiSlotID;
#endif //NEW_ITEM_NOTICE

		int		m_EnoughRemainSlotNum;		//체크에 사용할 카테고리에 남은 슬롯 개수 ( 예, 이 갯수 이하로 남으면 알린다 ) 
		float	m_fEnoughAllInvenSlotCheckTime;
};
