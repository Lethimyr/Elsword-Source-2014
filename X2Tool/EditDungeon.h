#pragma once

#include "ToolDungeonMap.h"
#include "TriggerFunc.h"
#include "NpcList.h"
#include "SkinDialog.h"

#include "afxpropertygridctrl.h"
#include "afxcmn.h"

#include "UserProperty.h"
#include "afxvslistbox.h"
#include "afxwin.h"

#include "SkinButton.h"
#include "XListCtrl.h"

enum EDIT_DUNGEON
{
	DUNGEON_WORLD_COLOR_GROUP = 1,
	DUNGEON_WORLD_COLOR_R,
	DUNGEON_WORLD_COLOR_G,
	DUNGEON_WORLD_COLOR_B,
	DUNGEON_UNIT_COLOR_GROUP,
	DUNGEON_UNIT_COLOR_R,
	DUNGEON_UNIT_COLOR_G,
	DUNGEON_UNIT_COLOR_B,

	ED_STAGE_GROUP,
	ED_WORLD_ID,
	ED_WORLD_NAME,
	ED_START_MOTION,
	ED_SECRET_STAGE,
	ED_STAGE_TYPE,
	ED_WORLD_COLOR_GROUP,
	ED_WORLD_COLOR_R,
	ED_WORLD_COLOR_G,
	ED_WORLD_COLOR_B,
	ED_UNIT_COLOR_GROUP,
	ED_UNIT_COLOR_R,
	ED_UNIT_COLOR_G,
	ED_UNIT_COLOR_B,

	ED_SUB_STAGE_GROUP,
	ED_MAIN_LINE_SET,
	ED_START_LINE_SET,
	ED_END_LINE_SET,
	ED_GO_TYPE,
	ED_USE_POS_OF_MONSTER_FOR_REBIRTH,
	ED_KEY_CODE_OF_MONSTER_FOR_REBIRTH,
	ED_KEYCODE_N_START_STATE_GROUP,
	ED_KEYCODE_N_START_STATE_KEYCODE,
	ED_KEYCODE_N_START_STATE_STARTSTATE,

	ED_CURTAIN_GROUP,
	ED_LINE_INDEX,
	ED_AT_START,
	ED_LOOK_LEFT,
	
	ED_CLEAR_CONDITION_GROUP,
	ED_CLEAR_CONDITION_TYPE,
	ED_CLEAR_CONDITION_NEXT_STAGE_GROUP,
	ED_CLEAR_CONDITION_NEXT_STAGE_CLEAR_TYPE,
	ED_CLEAR_CONDITION_NEXT_STAGE_STAGE_INDEX,
	ED_CLEAR_CONDITION_NEXT_STAGE_SUBSTAGE_INDEX,
	ED_CLEAR_CONDITION_NEXT_STAGE_RATE,
	ED_CLEAR_CONDITION_NEXT_STAGE_ENDINGSPEECHINDEX,
	ED_CLEAR_CONDITION_DATA_GROUP,
	ED_CC_PORTAL_LINE_INDEX,
	ED_CC_NPC_KEYCODE,
	ED_CC_NPC_COUNT,
	ED_CC_CLEAR_TIME,
	ED_CC_NPC_ID,

	ED_NPC_GROUP,
	ED_NPC_UNITID,
	ED_NPC_KEYCODE,
	ED_NPC_FOCUSCAMERA,
	ED_NPC_STARTPOS,
	ED_NPC_ACTIVE,
	ED_NPC_SHOWGAGE,
	ED_NPC_ADDPOS_GROUP,
	ED_NPC_ADDPOS_X,
	ED_NPC_ADDPOS_Y,
	ED_NPC_ADDPOS_Z,
	ED_NPC_LEVEL,
	ED_NPC_RATE,
	ED_NPC_HASBOSSGAGE,
	ED_NPC_SHOWBOSSNAME,
	ED_NPC_SHOWSUBBOSSNAME,
	ED_NPC_SIEGEMODE,
	ED_NPC_ISRIGHT,
	ED_NPC_UNITSCALE,
	ED_NPC_BOSSGAUAGEHPLINES,
	ED_NPC_ALLYNPC,
	ED_NPC_NODROP,
	ED_NPC_MONSTER_GRADE,
	ED_NPC_SUBNPC,
	ED_NPC_SUBNPCINDEX,
	ED_NPC_SUBNPCRATE,

	ED_TRIGGER_GROUP,
	ED_TRIGGER_HOST,
	ED_TRIGGER_ONTIME,
	ED_TRIGGER_INTERVAL,
	ED_TRIGGER_CONDITION_FUNC,
	ED_TRIGGER_REACT_FUNC,
};

// CEditDungeon 대화 상자입니다.

class CEditDungeon : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDungeon)

public:
	struct READY_NPC
	{
		CX2UnitManager::NPC_UNIT_ID m_eUnitId;
		int m_iUnitIndex;
		D3DXVECTOR3 m_vAddPos;
		bool	m_bIsRight;
	};

	CEditDungeon(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditDungeon();

	void SaveAs(wstring wstrFileName);
	char* LoadLuaFunction(SEnum::DUNGEON_ID eDungeonID);
	void ResetWorld(wstring wstrWorldFullName);
	void SetPropListFont();
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	CMFCPropertyGridProperty *GetProp(int iData) { return m_wndPropList.FindItemByData(iData); }
	void SetPropValue(int iData, const COleVariant& varValue)
	{
		CMFCPropertyGridProperty *pProp = GetProp( iData );
		if( pProp != NULL )
		{			
			pProp->SetOriginalValue(varValue);
			pProp->SetValue(varValue);	

			switch(iData)
			{
			case DUNGEON_WORLD_COLOR_R:
			case DUNGEON_WORLD_COLOR_G:
			case DUNGEON_WORLD_COLOR_B:
			case DUNGEON_UNIT_COLOR_R:
			case DUNGEON_UNIT_COLOR_G:
			case DUNGEON_UNIT_COLOR_B:
				{
					CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
					pPropSlider->UpdateSlider();
				}
				break;
			default:
				break;
			}
		}
	}

	void InitPropList();
	void InitClearCondData();
	void UpdateProp(int iFlag);
	void UpdateDungeonProp();
	void UpdateStageProp();
	void UpdateSubStageProp();
	void UpdateCurtainProp(int iIndex);
	void UpdateClearConditionProp(int iIndex);
	void UpdateClearConditionNextStageProp(int iIndex);
	void UpdateClearCondDataProp(int iIndex);
	int UpdateNpcProp(int iIndex = -1);
	void UpdateTriggerProp(int iIndex);

	void ResetDungeon( SEnum::DUNGEON_ID eDungeonID );
	void UpdateReadyNpc(bool bReMake = false);
	void UpdateStage();
	void UpdateSubStage();
	void UpdateCurtainGroup();
	void UpdateClearCondition();
	void UpdateClearConditionNextStage();
	void UpdateClearConditionData();
	void UpdateNpc();
	void AddReadyNpc( vector<CX2UnitManager::NPC_UNIT_ID> vecNpc );
	void UpdateTrigger();

	void ChangeSelItem(int iStage, int iSubStage);
	
	void ReadyNpcData();
	void AddNpcData(int iStartPos = -1, int iLineIndex = -1);
	void RefreshNpcData(int iKey = -1);
	vector<CEditDungeon::READY_NPC>	GetNpcInfoByStartPos(int iStartPos);
	CX2UnitManager::NPC_UNIT_ID GetSelectedReadyNpc();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DUNGEON_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CToolDungeonMap m_DlgDungeonMap;
	CTriggerFunc m_DlgTriggerFunc;
	CNpcList	m_DlgNpcList;
	vector<CX2DungeonStage::StageData*> m_vecStageData;
	vector<CX2DungeonSubStage::SubStageData*> m_vecSubStageData;

	int m_iSelectedStage;
	int m_iSelectedReadyNpc;
	int m_iSelectedSubStage;
	int m_iSelectedCurtainData;
	int m_iSelectedClearCondition;
	int m_iSelectedNextStage;
	int m_iSelectedClearCondData;
	int m_iSelectedNpc;
	int m_iSelectedTrigger;
	
	CFont m_fntPropList;
	CUserPropertyGrid m_wndPropList;
	CListCtrl m_listStage;
	CListCtrl m_listSubStage;
	char *m_pFunctionData;
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkListStage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListStage(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnNMDblclkListSubStage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNowstage();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged( WPARAM wParam, LPARAM lParam );
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CListCtrl m_listReadyNpc;
	afx_msg void OnLvnItemchangedListSubStage(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listCurtainGroup;
	afx_msg void OnLvnItemchangedListCurtainGroup(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listClearCondition;
	afx_msg void OnLvnItemchangedListClearCondition(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listNextStage;
	afx_msg void OnLvnItemchangedListClearCondition2(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listClearCondData;
	afx_msg void OnLvnItemchangedListClearCondition3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMfcbutton3();
	afx_msg void OnBnClickedMfcbutton4();
	afx_msg void OnBnClickedMfcbutton5();
	afx_msg void OnBnClickedMfcbutton6();
	afx_msg void OnBnClickedMfcbutton7();
	afx_msg void OnBnClickedMfcbutton8();
	afx_msg void OnBnClickedMfcbutton9();
	afx_msg void OnBnClickedMfcbutton10();
	afx_msg void OnBnClickedMfcbutton19();
	afx_msg void OnBnClickedMfcbutton20();
	afx_msg void OnBnClickedMfcbutton13();
	afx_msg void OnBnClickedMfcbutton14();
	afx_msg void OnBnClickedMfcbutton15();
	afx_msg void OnBnClickedMfcbutton16();
	afx_msg void OnBnClickedMfcbutton17();
	afx_msg void OnBnClickedMfcbutton18();
	afx_msg void OnBnClickedCheckNpcStartPosition();
	CButton m_checkNpc;
	//CListCtrl m_listNpc;
	CXListCtrl m_listNpc;
	afx_msg void OnLvnItemchangedListReadyNpc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListNpcGroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMfcbutton11();
	afx_msg void OnBnClickedMfcbutton12();
	CEdit m_editDungeonLevel;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton1();
	CListCtrl m_listTriggerData;
	afx_msg void OnLvnItemchangedListTrigger(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMfcbutton21();
	afx_msg void OnBnClickedMfcbutton22();
	

	afx_msg void OnBnClickedButton2();
};
