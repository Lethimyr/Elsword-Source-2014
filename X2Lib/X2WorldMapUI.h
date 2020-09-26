
#pragma once


#include "X2WorldFieldMap.h"

class CX2WorldMapUI 
{
public:
	enum WORLD_MAP_UI_CUSTOM_MSG
	{
		WMUCM_VILLAGE_CLICKED = 23000,
		WMUCM_DUNGEON_GATE_CLICKED,

		WMUCM_CLOSE_WORLD_MAP,
		WMUCM_CLOSE_FIELD_MAP,

		WMUCM_PREV_FIELD_MAP,
		WMUCM_NEXT_FIELD_MAP,

		WMUCM_NPC_MOUSE_OVER,
		WMUCM_NPC_MOUSE_OUT,

		WMUCM_NAVI_LEFT_CLICKED,
		WMUCM_NAVI_LEFT_DRAGGING,
		WMUCM_NAVI_WORLD_MAP_STOP_DRAGGING,

		WMUCM_MYCHAR_MOUSE_OVER,
		WMUCM_MYCHAR_MOUSE_OUT,
		WMUCM_PARTY_MEMBER_MOUSE_OVER,
		WMUCM_PARTY_MEMBER_MOUSE_OUT,

		WMUCM_POSTBOX_MOUSE_OVER,
		WMUCM_POSTBOX_MOUSE_OUT,
		WMUCM_MARKET_MOUSE_OVER,
		WMUCM_MARKET_MOUSE_OUT,
		WMUCM_OTHER_FIELD_MOUSE_OVER,
		WMUCM_OTHER_FIELD_MOUSE_OUT,

		WMUCM_BILLBOARD_MOUSE_OVER,
		WMUCM_BILLBOARD_MOUSE_OUT,

		WMUCM_VILLAGE_MOUSE_OVER,
		WMUCM_VILLAGE_MOUSE_OUT,
		WMUCM_FIELD_RIGHT_CLICK,
		WMUCM_NAVI_FIELD_MAP_STOP_DRAGGING,
		WMUCM_WORLD_RIGHT_CLICK,
	};

public:
	CX2WorldMapUI(void);
	virtual ~CX2WorldMapUI(void);

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void OpenWorldMap( bool bOpen );
	void UpdateWorldMap();

	CKTDGUIDialogType GetFieldMapDlg( int iFieldID );
	void OpenFieldMap( bool bOpen, int iFieldID = -1);
	void UpdateFieldMap();

	void SetStage( CKTDXStage* pStage );

	void OpenNPCToolTip( bool bOpen, int iNPCID = -1, D3DXVECTOR2 vPos = D3DXVECTOR2(0, 0) );
	// 2009.3.11 김태완
	void OpenMiscToolTip(bool bOpen, D3DXVECTOR2 vPos = D3DXVECTOR2(0,0), WORLD_MAP_UI_CUSTOM_MSG msg = WMUCM_OTHER_FIELD_MOUSE_OUT, int iFieldID = -1 );
	// 2009.2.13 김태완
	void OpenPartyMemberToolTip( bool bOpen, int PartyMemberIndex = -1, D3DXVECTOR2 vPos = D3DXVECTOR2(0,0) );

	void SetNpcToolTipImage(int iNPCID);

	CX2WorldFieldMap* GetWorldFieldMap(){ return m_pWorldFieldMap; }
	bool GetShow();

	void CloseAllToolTip();

	// 2009.03.21 김태완
	wstring GetNPCQuestDesc( CX2UnitManager::NPC_UNIT_ID eNPCID );
	
	void UpdateNaviMap( IN D3DXVECTOR2& vPos_, IN CKTDGUINavigation* pNavi_ );
	bool GetDragNaviMap();
	void CloseWorldMapUI()
	{
		if( true == GetShow() )
		{
			OpenWorldMap( false );
		}
		OpenFieldMap(false);
		CloseAllToolTip();
	}

#ifdef FIX_WORLD_MAP_UI_BUG
	void SetZoomed( bool bVal_ ) { m_bZoomed = bVal_; }		/// 마을, 휴식처, 필드 맵으로 확대 여부 설정
#endif FIX_WORLD_MAP_UI_BUG

#ifdef VILLAGE_MAP_FAST_RELOADING
	void ReLoadingVillageMap();
#endif //VILLAGE_MAP_FAST_RELOADING

private:
	CKTDXStage* m_pStage;

	CKTDGUIDialogType m_pDLGWorldMap;
	CKTDGUIDialogType m_pDLGWorldMapFront;

	CKTDGUIDialogType m_pDLGFieldMap;
	int			m_iCurrFieldMapID;
	map< int, CKTDGUIDialogType >	m_mapDLGFieldMap;	// village_id, dialog pointer
	CX2WorldFieldMap*	m_pWorldFieldMap;

	CKTDGUIDialogType m_pDLGNPCToolTip;

	// 2009.02.13 김태완
	CKTDGUIDialogType m_pDLGPartyToolTip;

	// 2009.03.11 김태완
	CKTDGUIDialogType m_pDLGMiscToolTip;

	bool	m_bZoomed;			/// 마을, 휴식처, 필드 맵으로 확대 여부
	bool	m_bBeforeField;		/// 월드맵 상태의 마우스 오른쪽 버튼 이벤트를 위한 플래그


};
