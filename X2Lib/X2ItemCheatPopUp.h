#pragma once


#ifdef ITEM_CHEAT_POPUP_TEST


class CX2ItemCheatPopUp
{

	enum SORT_TYPE
	{
		ST_NONE,
		ST_ID,
		ST_NAME,
	};


	enum ITEM_CHEAT_POPUP_CUSTOM_MSG
	{
		ICPCM_COMBO_SORT_TYPE = 29100,

		ICPCM_COMBO_FASHION_ONLY,
		ICPCM_COMBO_UNIT_TYPE,
		ICPCM_COMBO_UNIT_CLASS,
		ICPCM_COMBO_EQUIP_POSITION,

		ICPCM_EDIT_MIN_LEVEL,
		ICPCM_EDIT_MAX_LEVEL,

		ICPCM_BUY_ITEM,
		ICPCM_EDIT_ITEM_COUNT,
		ICPCM_EDIT_SEARCH_KEYWORD,

		ICPCM_CLOSE_WINDOW,
	};


public:
	CX2ItemCheatPopUp();
	~CX2ItemCheatPopUp()
	{
		SAFE_DELETE_DIALOG( m_pDLGItemList );
	}


	bool IsOpened()
	{
		if( NULL != m_pDLGItemList )
		{
			return m_pDLGItemList->GetShow();
		}
		else 
		{
			return false;
		}
	}
	void OpenItemList( bool bOpen );
	void UpdateItemList();

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	void SetStage( CKTDXStage* pStage )
	{
		if( NULL != m_pDLGItemList )
		{
			m_pDLGItemList->SetStage( pStage );
		}
	}

private:
	std::wstring m_SearchKeyword;
	int m_iFashionOnly;		// -1:don't care, 0: false, 1:true
	CX2Unit::UNIT_TYPE m_eUnitType;
	CX2Unit::UNIT_CLASS m_eUnitClass;
	CX2Unit::EQIP_POSITION m_eEquipPosition;
	int m_iMinLevel;
	int m_iMaxLevel;

	CKTDGUIDialogType m_pDLGItemList;

	SORT_TYPE m_eSortType;		
	int m_iItemCount;						// 아이템 더블 클릭했을 때 구매할 개수




	//bool m_bUpdateItemList;

}; // class CX2ItemCheatPopUp




#endif ITEM_CHEAT_POPUP_TEST




