// EditDungeon.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Resource.h"
#include "MainFrm.h"
#include "X2Tool.h"
#include "afxdialogex.h"
#include "EditDungeon.h"

CGlobal Global;

EnumStringTable wstrNpcType[] =
{	
	{ L"NCT_BASIC",			0, },
	{ L"NCT_THING_GATE",	100, },
	{ L"NCT_THING_BOX",		101, },
	{ L"NCT_THING_TRAP",	102, },
	{ L"NCT_THING_DEVICE",	103, },
	{ L"NCT_THING_HOUSE",	104, },
	{ L"NCT_THING_WORLD_BLOCK", 105, },
	{ L"NCT_THING_CHECKER",	106, },
	{ L"NCT_THING_NOBUFF",	107, },
};

wstring wstrStageType[] =
{
	L"ST_NONE",
	L"ST_EXTRA_NPC",
	L"ST_EXTRA",
};
wstring wstrGoType[] = 
{
	L"GT_NONE",
	L"GT_RIGHT",
	L"GT_LEFT",
	L"GT_UP",
	L"GT_DOWN",
};

wstring wstrStageClearType[] =
{
	L"CC_KILL_ALL_ACTIVE_NPC",
	L"CC_KILL_KEYCODE_NPC",
	L"CC_KILL_KEYCODE_NPC_ANY",
	L"CC_KILL_NPC_TYPE",
	L"CC_KILL_KEYCODE_NPC_COUNT",
	L"CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC",
	L"CC_TIME",
};

wstring wstrClearType[] =
{
	L"CT_SUB_STAGE",
	L"CT_STAGE",
	L"CT_STAGE_FORCE",
	L"CT_STAGE_SECRET",
	L"CT_GAME",
};

wstring wstrMonsterGrade[] =
{
	L"MG_NORMAL_NPC",
	L"MG_BOSS_NPC",
	L"MG_MIDDLE_BOSS_NPC",
};

// CEditDungeon 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditDungeon, CDialogEx)

CEditDungeon::CEditDungeon(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDungeon::IDD, pParent)
{
	m_iSelectedStage = -1;
	m_iSelectedReadyNpc = -1;
	m_iSelectedSubStage = -1;
	m_iSelectedCurtainData = -1;
	m_iSelectedClearCondition = -1;
	m_iSelectedNextStage = -1;
	m_iSelectedClearCondData = -1;
	m_iSelectedNpc = -1;
	m_iSelectedTrigger = -1;


	Global.LoadBitmaps();
	Global.LoadIcons();

	Global.SetRGB(IDX_RGB_MASK, RGB(255,0,255));
	Global.SetRGB(IDX_RGB_BACKGROUND, RGB(255,255,255));

	Global.SetRGB(IDX_RGB_FONT, RGB(51,51,51));
	Global.SetRGB(IDX_RGB_FONT_CAPTION, RGB(0,0,0));
	Global.SetRGB(IDX_RGB_FONT_CAPTION_INACTIVE, RGB(178,178,178));
	Global.SetRGB(IDX_RGB_CONTROL_BORDER, RGB(204,204,204));
	
	Global.SetFont(IDX_FONT_SMALL, _T("Arial"), 8);
	Global.SetFont(IDX_FONT_MEDIUM, _T("Arial"), 9);
	Global.SetFont(IDX_FONT_LARGE, _T("Arial"), 11, TRUE);

	m_pFunctionData = NULL;
}

CEditDungeon::~CEditDungeon()
{
	SAFE_DELETE_ARRAY( m_pFunctionData );
}

BOOL CEditDungeon::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		case VK_DELETE:
			{
				if( GetFocus() == GetDlgItem(IDC_LIST_NPC_GROUP) )
				{
					CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
					CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							SAFE_DELETE(pNpcData);
						}
						pSubStageData->m_vecSubStageNpcData.erase( pSubStageData->m_vecSubStageNpcData.begin() + m_iSelectedNpc );

						ReadyNpcData();
						UpdateNpc();
					}
				}
				else if( GetFocus() == GetDlgItem(IDC_LIST_READY_NPC) )
				{
					CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
					CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					if( m_iSelectedReadyNpc >= 0 && m_iSelectedReadyNpc < (int)pStageData->m_ReadyNPCList.size() )
					{
						pStageData->m_ReadyNPCList.erase( pStageData->m_ReadyNPCList.begin() + m_iSelectedReadyNpc );
						UpdateReadyNpc();
					}
				}
			}
			break;
		default:
			break;
		}
	}
// 	else if( pMsg->message == WM_DROPFILES )
// 	{		
// 		OnDropFiles(pMsg->hwnd, (HDROP)pMsg->wParam);
// 	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CEditDungeon::ResetDungeon( SEnum::DUNGEON_ID eDungeonID )
{
	m_iSelectedStage = 0;
	m_iSelectedReadyNpc = -1;
	m_iSelectedSubStage = 0;
	m_iSelectedNpc = -1;
	m_iSelectedTrigger = -1;

	UpdateStage();

	static bool bInitDlg = false;
	if( bInitDlg == false )
	{
		// trigger function
		m_DlgTriggerFunc.Create(IDD_DIALOG_TRIGGER_FUNC, this);		

		// Dungeon Map
		m_DlgDungeonMap.Create(IDD_DIALOG_MAP, this);

		// npc list
		m_DlgNpcList.Create(IDD_DIALOG_NPC_LIST, this);
		m_DlgNpcList.InitNpcData();
		bInitDlg = true;
	}

	CX2Dungeon::DIFFICULTY_LEVEL eDifficulty = CX2Dungeon::DL_NORMAL;
	eDifficulty = g_pData->GetDungeonManager()->GetDungeonData(eDungeonID)->m_eDifficulty;
	
	wstring wstrDungeonName = g_pData->GetDungeonManager()->MakeDungeonNameString(eDungeonID, eDifficulty, CX2Dungeon::DM_INVALID);
	GetDlgItem(IDC_STATIC_DUNGEONNAME)->SetWindowText( wstrDungeonName.c_str() );

	int iLevel = 0;
	iLevel = g_pData->GetDungeonManager()->GetDungeonData(eDungeonID)->m_iNpcLevel;
	WCHAR wLv[10];
	wsprintfW(wLv, L"%d", iLevel);
	m_editDungeonLevel.SetWindowText( wLv );

	char *pStartFunction = LoadLuaFunction(eDungeonID);
	if( m_pFunctionData != NULL && pStartFunction != NULL )
	{
		wstring wstrFunction;
		ConvertUtf8ToWCHAR(wstrFunction, pStartFunction);
		m_DlgTriggerFunc.SetFunctionData(wstrFunction);

		SAFE_DELETE_ARRAY(m_pFunctionData);
	}
	else
	{
		m_DlgTriggerFunc.SetFunctionData(L"");
	}
}

void CEditDungeon::UpdateStage()
{
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	m_vecStageData.clear();
	m_vecStageData = pDungeon->GetStageDataList();

	m_listStage.DeleteColumn(1);
	m_listStage.DeleteColumn(0);
	m_listStage.DeleteAllItems();

	m_listStage.InsertColumn(0, _T("Stage"), LVCFMT_CENTER, 50 );	
	m_listStage.InsertColumn(1, _T("World ID"), LVCFMT_CENTER, 400 );

	for(UINT i=0; i<m_vecStageData.size(); ++i)
	{	
		wstring wstrWorldID = L"";
		if( g_pData != NULL && g_pData->GetWorldManager() != NULL && 
			m_vecStageData[i] != NULL && g_pData->GetWorldManager()->GetWorldData(m_vecStageData[i]->m_WorldID) != NULL )
			wstrWorldID = g_pData->GetWorldManager()->GetWorldData(m_vecStageData[i]->m_WorldID)->worldName;

		WCHAR wStage[10];
		wsprintfW(wStage, L"%d", i);
		m_listStage.InsertItem(i, wStage);
		m_listStage.SetItemText(i, 1, wstrWorldID.c_str());
	}

	m_listStage.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

	m_listStage.SetItemState(m_iSelectedStage, LVIS_SELECTED, LVIS_SELECTED );
	m_listStage.EnsureVisible(0, TRUE);
	m_listStage.SetFocus();

	UpdateDungeonProp();
	UpdateStageProp();
	UpdateSubStageProp();
	UpdateCurtainGroup();
	UpdateClearCondition();
	UpdateClearConditionNextStage();
	UpdateTrigger();
}

void CEditDungeon::UpdateReadyNpc(bool bReMake)
{
	CX2ToolMain *pToolMain = (CX2ToolMain *)g_pMain;
	
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = m_vecStageData[m_iSelectedStage];

	m_listReadyNpc.DeleteColumn(3);
	m_listReadyNpc.DeleteColumn(2);
	m_listReadyNpc.DeleteColumn(1);
	m_listReadyNpc.DeleteColumn(0);
	m_listReadyNpc.DeleteAllItems();
	m_listReadyNpc.InsertColumn(0, _T("Npc ID"), LVCFMT_CENTER, 200 );
	m_listReadyNpc.InsertColumn(1, _T("Npc Type"), LVCFMT_CENTER, 120 );
	m_listReadyNpc.InsertColumn(2, _T("Npc Name"), LVCFMT_CENTER, 150 );
	m_listReadyNpc.InsertColumn(3, _T("Npc File Name"), LVCFMT_CENTER, 200 );


	vector<CX2UnitManager::NPC_UNIT_ID> vecReadyNpc = pStageData->m_ReadyNPCList;
	for(UINT i=0; i<vecReadyNpc.size(); ++i)
	{
		//CX2UnitManager::NPCUnitTemplet *pNpcTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet(vecReadyNpc[i]);
		CX2UnitManager::NPCUnitTemplet *pNpcTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet(vecReadyNpc[i]);
		if( pNpcTemplet != NULL )
		{
			WCHAR wbuf[128];
			wsprintf( wbuf, L"%s", pToolMain->m_enumManager.GetNpcEnumString( vecReadyNpc[i] ).c_str() );
			m_listReadyNpc.InsertItem(i, wbuf);
			for( int j=0; j<ARRAY_SIZE(wstrNpcType); ++j)
			{
				if( wstrNpcType[j].inx == pNpcTemplet->m_ClassType )
				{
					m_listReadyNpc.SetItemText(i, 1, wstrNpcType[j].strName);
					break;
				}
			}			
			m_listReadyNpc.SetItemText(i, 2, pNpcTemplet->m_Name.c_str());
			m_listReadyNpc.SetItemText(i, 3, pNpcTemplet->m_LuaFileName.c_str());
		}
	}
	m_listReadyNpc.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );


	ReadyNpcData();
}

void CEditDungeon::UpdateSubStage()
{
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	
	for(int i=0; i<m_listStage.GetItemCount(); ++i)
	{							
		UINT uItemState = m_listStage.GetItemState ( i, LVIS_SELECTED );
		if( uItemState == LVIS_SELECTED )
		{
			m_iSelectedStage = i;

			// 현재 선택된 스테이지의 서브스테이지 정보를 갱신한다.
			CX2DungeonStage::StageData* pStageData = m_vecStageData[i];
			if( pStageData != NULL )
			{
				m_vecSubStageData.clear();
				m_vecSubStageData = pStageData->m_SubStageDataList;

				m_listSubStage.DeleteColumn(3);
				m_listSubStage.DeleteColumn(2);
				m_listSubStage.DeleteColumn(1);
				m_listSubStage.DeleteColumn(0);
				m_listSubStage.DeleteAllItems();

				m_listSubStage.InsertColumn(0, _T("SubStage"), LVCFMT_CENTER, 50 );	
				m_listSubStage.InsertColumn(1, _T("Start"), LVCFMT_CENTER, 50 );	
				m_listSubStage.InsertColumn(2, _T("Main"), LVCFMT_CENTER, 50 );
				m_listSubStage.InsertColumn(3, _T("End"), LVCFMT_CENTER, 50 );

				for(UINT j=0; j<m_vecSubStageData.size(); ++j)
				{	
					CX2DungeonSubStage::SubStageData* pSubStageData = m_vecSubStageData[j];
					WCHAR wSubStageData[10];
					wsprintfW(wSubStageData, L"%d", j);
					m_listSubStage.InsertItem(j, wSubStageData);

					wsprintfW(wSubStageData, L"%d", pSubStageData->m_StartLineSet);
					m_listSubStage.SetItemText(j, 1, wSubStageData);
					wsprintfW(wSubStageData, L"%d", pSubStageData->m_MainLineSet);
					m_listSubStage.SetItemText(j, 2, wSubStageData);
					wsprintfW(wSubStageData, L"%d", pSubStageData->m_EndLineSet);
					m_listSubStage.SetItemText(j, 3, wSubStageData);
				}

				m_listSubStage.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

				m_listSubStage.SetItemState(m_iSelectedSubStage, LVIS_SELECTED, LVIS_SELECTED );
				m_listSubStage.EnsureVisible(0, TRUE);				
			}
			break;
		}
	}
}

void CEditDungeon::UpdateCurtainGroup()
{
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)m_vecStageData.size() ||
		m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)m_vecSubStageData.size() )
		return;

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = m_vecStageData[m_iSelectedStage];
	if( pStageData != NULL )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = m_vecSubStageData[m_iSelectedSubStage];
		if( pSubStageData != NULL )
		{
			m_listCurtainGroup.DeleteColumn(2);
			m_listCurtainGroup.DeleteColumn(1);
			m_listCurtainGroup.DeleteColumn(0);
			m_listCurtainGroup.DeleteAllItems();
			m_listCurtainGroup.InsertColumn(0, _T("Line Index"), LVCFMT_CENTER, 70 );	
			m_listCurtainGroup.InsertColumn(1, _T("At Start"), LVCFMT_CENTER, 80 );	
			m_listCurtainGroup.InsertColumn(2, _T("Look Left"), LVCFMT_CENTER, 80 );

			vector<CX2DungeonSubStage::CurtainData*> vecCurtainGroup = pSubStageData->m_CurtainDataList;
			for( UINT i=0; i<vecCurtainGroup.size(); ++i)
			{
				CX2DungeonSubStage::CurtainData *pCurtainData = vecCurtainGroup[i];
				if( pCurtainData != NULL )
				{
					WCHAR wCurtainData[10];
					wsprintfW(wCurtainData, L"%d", pCurtainData->m_LineIndex);
					m_listCurtainGroup.InsertItem(i, wCurtainData);

					if( pCurtainData->m_bAtStart == true )
						m_listCurtainGroup.SetItemText(i, 1, L"TRUE");
					else
						m_listCurtainGroup.SetItemText(i, 1, L"FALSE");
					if( pCurtainData->m_bLookLeft == true )
						m_listCurtainGroup.SetItemText(i, 2, L"TRUE");
					else
						m_listCurtainGroup.SetItemText(i, 2, L"FALSE");
				}
			}

			m_listCurtainGroup.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

			m_listCurtainGroup.SetItemState(m_iSelectedCurtainData, LVIS_SELECTED, LVIS_SELECTED );
			m_listCurtainGroup.EnsureVisible(0, TRUE);				
		}
	}
}

void CEditDungeon::UpdateTrigger()
{
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)m_vecStageData.size() ||
		m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)m_vecSubStageData.size() )
		return;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = m_vecStageData[m_iSelectedStage];
	if( pStageData != NULL )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = m_vecSubStageData[m_iSelectedSubStage];
		if( pSubStageData != NULL )
		{
			vector<CX2DungeonSubStage::Trigger*> &vecTriggerData = pSubStageData->m_TriggerList;

			m_listTriggerData.DeleteColumn(3);
			m_listTriggerData.DeleteColumn(2);
			m_listTriggerData.DeleteColumn(1);
			m_listTriggerData.DeleteColumn(0);
			m_listTriggerData.DeleteAllItems();
			m_listTriggerData.InsertColumn(0, _T("Id"), LVCFMT_CENTER, 100 );
			m_listTriggerData.InsertColumn(1, _T("Host"), LVCFMT_CENTER, 100 );
			m_listTriggerData.InsertColumn(2, _T("One Time"), LVCFMT_CENTER, 100 );
			m_listTriggerData.InsertColumn(3, _T("Interval"), LVCFMT_CENTER, 100 );

			for(UINT i=0; i<vecTriggerData.size(); ++i)
			{
				CX2DungeonSubStage::Trigger* pTriggerData = vecTriggerData[i];
				WCHAR wBuf[10];
				wsprintfW(wBuf, L"%d", i);
				m_listTriggerData.InsertItem(i, wBuf);

				if( pTriggerData->m_bHost == true )
					m_listTriggerData.SetItemText(i, 1, L"TRUE");
				else
					m_listTriggerData.SetItemText(i, 1, L"FALSE");

				if( pTriggerData->m_bOneTime == true )
					m_listTriggerData.SetItemText(i, 2, L"TRUE");
				else
					m_listTriggerData.SetItemText(i, 2, L"FALSE");

				char cBuf[10];
				sprintf(cBuf, "%5.2f", pTriggerData->m_Interval);
				wstring wstrTemp;
				ConvertCharToWCHAR(wstrTemp, cBuf);
				m_listTriggerData.SetItemText(i, 3, wstrTemp.c_str());
			}

			m_listTriggerData.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

			m_listTriggerData.SetItemState(m_iSelectedTrigger, LVIS_SELECTED, LVIS_SELECTED );
			m_listTriggerData.EnsureVisible(0, TRUE);
		}
	}
}

void CEditDungeon::UpdateClearCondition()
{
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)m_vecStageData.size() ||
		m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)m_vecSubStageData.size() )
		return;

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = m_vecStageData[m_iSelectedStage];
	if( pStageData != NULL )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = m_vecSubStageData[m_iSelectedSubStage];
		if( pSubStageData != NULL )
		{
			m_listClearCondition.DeleteColumn(0);
			m_listClearCondition.DeleteAllItems();
			m_listClearCondition.InsertColumn(0, _T("Clear Condition Type"), LVCFMT_CENTER, 200 );

			vector<CX2DungeonSubStage::ClearCondtionData> vecClearCond = pSubStageData->m_vecClearCondData;
			for(UINT i=0; i<vecClearCond.size(); ++i)
			{
				wstring wstrStageClearTypeValue = wstrStageClearType[(int)(vecClearCond[i].m_eClearCondition)];
				m_listClearCondition.InsertItem(i, wstrStageClearTypeValue.c_str());
			}

			m_listClearCondition.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

			m_listClearCondition.SetItemState(m_iSelectedClearCondition, LVIS_SELECTED, LVIS_SELECTED );
			m_listClearCondition.EnsureVisible(0, TRUE);
		}
	}
}

void CEditDungeon::UpdateClearConditionNextStage()
{
	if( m_iSelectedClearCondition < 0 ||
		m_iSelectedStage < 0 || m_iSelectedStage >= (int)m_vecStageData.size() ||
		m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)m_vecSubStageData.size() )
	{
		return;
	}

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = m_vecStageData[m_iSelectedStage];
	if( pStageData != NULL )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = m_vecSubStageData[m_iSelectedSubStage];
		if( pSubStageData != NULL )
		{
			if( m_iSelectedClearCondition >= (int)pSubStageData->m_vecClearCondData.size() )
				return;
			CX2DungeonSubStage::ClearCondtionData clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];

			m_listNextStage.DeleteColumn(2);
			m_listNextStage.DeleteColumn(1);
			m_listNextStage.DeleteColumn(0);
			m_listNextStage.DeleteAllItems();
			m_listNextStage.InsertColumn(0, _T("Clear Type"), LVCFMT_CENTER, 150 );
			m_listNextStage.InsertColumn(1, _T("Stage"), LVCFMT_CENTER, 50 );
			m_listNextStage.InsertColumn(2, _T("SubStage"), LVCFMT_CENTER, 50 );

			vector<CX2DungeonSubStage::NextStageData> vecNextStage = clearCond.m_vecNextStage;
			for(UINT i=0; i<vecNextStage.size(); ++i)
			{
				wstring wstrClearTypeValue = wstrClearType[(int)(vecNextStage[i].m_eClearType)];
				m_listNextStage.InsertItem(i, wstrClearTypeValue.c_str());

				WCHAR wNextStageData[10];
				wsprintfW(wNextStageData, L"%d", vecNextStage[i].m_iStageIndex);				
				m_listNextStage.SetItemText(i, 1, wNextStageData);
				wsprintfW(wNextStageData, L"%d", vecNextStage[i].m_iSubStageIndex);				
				m_listNextStage.SetItemText(i, 2, wNextStageData);
			}

			m_listNextStage.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

			m_listNextStage.SetItemState(m_iSelectedNextStage, LVIS_SELECTED, LVIS_SELECTED );
			m_listNextStage.EnsureVisible(0, TRUE);
		}
	}
}

void CEditDungeon::UpdateClearConditionData()
{
	if( m_iSelectedClearCondition < 0 ||
		m_iSelectedStage < 0 || m_iSelectedStage >= (int)m_vecStageData.size() ||
		m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)m_vecSubStageData.size() )
		return;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = m_vecStageData[m_iSelectedStage];
	if( pStageData != NULL )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = m_vecSubStageData[m_iSelectedSubStage];
		if( pSubStageData != NULL )
		{
			if( m_iSelectedClearCondition >= (int)pSubStageData->m_vecClearCondData.size() )
				return;
			CX2DungeonSubStage::ClearCondtionData clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];

			m_listClearCondData.DeleteColumn(2);
			m_listClearCondData.DeleteColumn(1);
			m_listClearCondData.DeleteColumn(0);
			m_listClearCondData.DeleteAllItems();

			switch(clearCond.m_eClearCondition)
			{
			case CX2DungeonSubStage::CC_KILL_ALL_ACTIVE_NPC:
				{
					
				}
				break;
			case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
			case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:
				{
					m_listClearCondData.InsertColumn(0, _T("Npc KeyCode"), LVCFMT_CENTER, 150 );
					m_listClearCondData.InsertColumn(1, _T(""), LVCFMT_CENTER, 50 );
					m_listClearCondData.InsertColumn(2, _T(""), LVCFMT_CENTER, 50 );
										
					for( UINT i=0; i<clearCond.m_vecNPCKeyCode.size(); ++i)
					{
						WCHAR wClearCondData[10];
						wsprintfW(wClearCondData, L"%d", clearCond.m_vecNPCKeyCode[i].m_iKey);				
						m_listClearCondData.InsertItem(i, wClearCondData);
					}
				}
				break;
			case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
				{
					m_listClearCondData.InsertColumn(0, _T("Npc ID"), LVCFMT_CENTER, 300 );
					m_listClearCondData.InsertColumn(1, _T(""), LVCFMT_CENTER, 0 );
					m_listClearCondData.InsertColumn(2, _T(""), LVCFMT_CENTER, 0 );

					CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
					for( UINT i=0; i<clearCond.m_vecNPCID.size(); ++i)
					{
						CX2UnitManager::NPC_UNIT_ID UnitId = (CX2UnitManager::NPC_UNIT_ID)clearCond.m_vecNPCID[i].m_iKey;
						CX2UnitManager::NPCUnitTemplet *pNpcTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( UnitId );
						if( pNpcTemplet != NULL )
						{
							WCHAR wbuf[128];
							wsprintf( wbuf, L"%s", pToolMain->m_enumManager.GetNpcEnumString( UnitId ).c_str() );
							m_listClearCondData.InsertItem(i, wbuf);
						}
						else
						{
							m_listClearCondData.InsertItem(i, L"NUI_NONE");
						}
					}
				}
				break;
			case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_COUNT:
				{
					m_listClearCondData.InsertColumn(0, _T("Npc Keycode"), LVCFMT_CENTER, 150 );
					m_listClearCondData.InsertColumn(1, _T("Npc Count"), LVCFMT_CENTER, 50 );
					m_listClearCondData.InsertColumn(2, _T(""), LVCFMT_CENTER, 50 );

					for( UINT i=0; i<clearCond.m_vecNPCKeyCode.size(); ++i)
					{
						WCHAR wClearCondData[10];
						wsprintfW(wClearCondData, L"%d", clearCond.m_vecNPCKeyCode[i].m_iKey);				
						m_listClearCondData.InsertItem(i, wClearCondData);
						wsprintfW(wClearCondData, L"%d", clearCond.m_vecNPCKeyCode[i].m_iObjectiveCount);				
						m_listNextStage.SetItemText(i, 1, wClearCondData);
					}
				}
				break;
			case CX2DungeonSubStage::CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC:
				{
					m_listClearCondData.InsertColumn(0, _T("Potoal Line Index"), LVCFMT_CENTER, 150 );
					m_listClearCondData.InsertColumn(1, _T(""), LVCFMT_CENTER, 50 );
					m_listClearCondData.InsertColumn(2, _T(""), LVCFMT_CENTER, 50 );

					for( UINT i=0; i<clearCond.m_vecPortalLineIndex.size(); ++i)
					{
						WCHAR wClearCondData[10];
						wsprintfW(wClearCondData, L"%d", clearCond.m_vecPortalLineIndex[i]);				
						m_listClearCondData.InsertItem(i, wClearCondData);
					}
					
				}
				break;
			case CX2DungeonSubStage::CC_TIME:
				{
					m_listClearCondData.InsertColumn(0, _T("Clear Time"), LVCFMT_CENTER, 150 );
					m_listClearCondData.InsertColumn(1, _T(""), LVCFMT_CENTER, 50 );
					m_listClearCondData.InsertColumn(2, _T(""), LVCFMT_CENTER, 50 );

					char szClearCondData[10];
					wstring wstrClearCondData;

					sprintf(szClearCondData, "%5.2f", clearCond.m_fClearTime );
					ConvertCharToWCHAR(wstrClearCondData, szClearCondData);
					m_listClearCondData.InsertItem(0, wstrClearCondData.c_str());					
				}
				break;
			default:
				return;
			}

			m_listClearCondData.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

			m_listClearCondData.SetItemState(m_iSelectedClearCondData, LVIS_SELECTED, LVIS_SELECTED );
			m_listClearCondData.EnsureVisible(0, TRUE);
		}
	}
}

void CEditDungeon::UpdateNpc()
{
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();


	CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	vector<CX2DungeonSubStage::NPCData*> vecNpcData = pSubStageData->m_vecSubStageNpcData;

	CStringArray saNpcActive;
	saNpcActive.Add(_T("TRUE"));
	saNpcActive.Add(_T("FALSE"));

	m_listNpc.DeleteColumn(4);
	m_listNpc.DeleteColumn(3);
	m_listNpc.DeleteColumn(2);
	m_listNpc.DeleteColumn(1);
	m_listNpc.DeleteColumn(0);
	m_listNpc.DeleteAllItems();
	
	m_listNpc.InsertColumn(0, _T("Npc ID"), LVCFMT_CENTER, 300, 0 );
	m_listNpc.InsertColumn(1, _T("Start Pos"), LVCFMT_CENTER, 100, 1 );
	m_listNpc.InsertColumn(2, _T("Active"), LVCFMT_CENTER, 60, 2 );
	m_listNpc.InsertColumn(3, _T("KeyCode"), LVCFMT_CENTER, 70, 3 );
	m_listNpc.InsertColumn(4, _T("SubNpc"), LVCFMT_CENTER, 80, 4 );
	
	m_listNpc.SetUnderLine(FALSE);	
	m_listNpc.m_HeaderCtrl.SetHeight(30);
	m_listNpc.m_HeaderCtrl.SetTextFont( Global.GetFont(IDX_FONT_SMALL) );
	m_listNpc.SetTextFont( Global.GetFont(IDX_FONT_SMALL) );

	for(UINT i=0; i<vecNpcData.size(); ++i)
	{	
		CX2DungeonSubStage::NPCData* pNpcData = vecNpcData[i];
		
		CX2UnitManager::NPC_UNIT_ID UnitId = pNpcData->m_UnitID;
		CX2UnitManager::NPCUnitTemplet *pNpcTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( UnitId );
		if( pNpcTemplet != NULL )
		{			
			WCHAR wbuf[128];
			wsprintf( wbuf, L"%s", pToolMain->m_enumManager.GetNpcEnumString( UnitId ).c_str() );
			if( UnitId == CX2UnitManager::NUI_NONE || StrCmpW(wbuf, L"") == 0 )
				m_listNpc.InsertItem(i, wbuf, RGB(255, 0, 0), RGB(0, 255, 255));
			else
				m_listNpc.InsertItem(i, wbuf);
						
			bool bInValidStartPos = false;
			wstring wstrStartPos = L"";
			for(UINT j=0; j<pNpcData->m_vecStartPos.size(); ++j)
			{
				WCHAR wbufSP[100];
				if( j == pNpcData->m_vecStartPos.size() - 1 )
					wsprintfW(wbufSP, L"%d", pNpcData->m_vecStartPos[j]);
				else
					wsprintfW(wbufSP, L"%d, ", pNpcData->m_vecStartPos[j]);
				wstrStartPos = wstrStartPos + wbufSP;

				if( pNpcData->m_vecStartPos[j] == -1 )
					bInValidStartPos = true;

 				if( pDungeon->GetNowStageIndex() == m_iSelectedStage )
 				{
					CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
					const CKTDGLineMap::StartInfoMap& mapStartInfo = pLineMap->GetStartInfoMap();

					CKTDGLineMap::StartInfoMap::const_iterator it = mapStartInfo.find( pNpcData->m_vecStartPos[j] );

					if( it == mapStartInfo.end() || pNpcData->m_vecStartPos[j] == -1 )
						bInValidStartPos = true;
 				}
			}

			if( bInValidStartPos == false )
				m_listNpc.SetItemText(i, 1, wstrStartPos.c_str());
			else
				m_listNpc.SetItemText(i, 1, wstrStartPos.c_str(), RGB(255, 0, 0), RGB(0, 255, 255));

			//m_listNpc.SetComboBox(i, 2, TRUE, &saNpcActive );
			if( pNpcData->m_bActive == true )
				m_listNpc.SetItemText(i, 2, L"TRUE");
			else
				m_listNpc.SetItemText(i, 2, L"FALSE");

			WCHAR wbufKeyCode[10];
			wsprintfW(wbufKeyCode, L"%d", pNpcData->m_KeyCode);
			m_listNpc.SetItemText(i, 3, wbufKeyCode);

			if( pNpcData->m_bSubNpc == false )
			{
				m_listNpc.SetItemText(i, 4, L"");
			}
			else
			{
				int iSubIndex = pNpcData->m_iSubNPCIndex;				
				WCHAR wBufSubIndex[100];
				wsprintfW(wBufSubIndex, L"%d ", iSubIndex);

				int iColor = iSubIndex * iSubIndex;
				while( iColor >= 255 )
				{
					iColor -= 255;					
				}

				m_listNpc.SetItemText(i, 4, wBufSubIndex, RGB(255, iColor, iColor), RGB(0,255-iColor,255-iColor));
			}
		}
	}

	m_listNpc.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

	m_listNpc.SetItemState(m_iSelectedNpc, LVIS_SELECTED, LVIS_SELECTED );
	m_listNpc.EnsureVisible(0, TRUE);
}

void CEditDungeon::AddReadyNpc( vector<CX2UnitManager::NPC_UNIT_ID> vecNpc )
{
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = m_vecStageData[m_iSelectedStage];

	for(UINT i=0; i<vecNpc.size(); ++i)
	{
		pStageData->m_ReadyNPCList.push_back(vecNpc[i]);
	}

	UpdateReadyNpc();
}

void CEditDungeon::ChangeSelItem(int iStage, int iSubStage)
{
	m_listStage.SetItemState(iStage, LVIS_SELECTED, LVIS_SELECTED );
	m_listStage.EnsureVisible(iStage, TRUE);
	m_listSubStage.SetItemState(iSubStage, LVIS_SELECTED, LVIS_SELECTED );
	m_listSubStage.EnsureVisible(iSubStage, TRUE);

	m_iSelectedStage = iStage;
	m_iSelectedReadyNpc = -1;
	m_iSelectedSubStage = iSubStage;
	m_iSelectedNpc = -1;
	m_iSelectedTrigger = -1;
	
	m_iSelectedCurtainData = 0;
	m_iSelectedClearCondition = 0;
	m_iSelectedNextStage = 0;
	m_iSelectedClearCondData = 0;
	
	InitClearCondData();
	UpdateStageProp();
	UpdateSubStageProp();
	UpdateCurtainGroup();
	UpdateClearCondition();
	UpdateClearConditionNextStage();
	UpdateClearConditionData();
	UpdateTrigger();
}


void CEditDungeon::InitPropList()
{
	SetPropListFont();

	//m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.SetLeftColumnWidht(130);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pDungeonWorldColorGroup = new CMFCPropertyGridProperty(_T("World Color"), DUNGEON_WORLD_COLOR_GROUP);
	{
		float fVal = 1.f;
		CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, DUNGEON_WORLD_COLOR_R );
		pColorPropr->SetRange( 0, 200 );
		pColorPropr->SetFloatValue( 2.f, -1.f );
		pDungeonWorldColorGroup->AddSubItem(pColorPropr);

		CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, DUNGEON_WORLD_COLOR_G );
		pColorPropg->SetRange( 0, 200 );
		pColorPropg->SetFloatValue( 2.f, -1.f );
		pDungeonWorldColorGroup->AddSubItem(pColorPropg);

		CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, DUNGEON_WORLD_COLOR_B );
		pColorPropb->SetRange( 0, 200 );
		pColorPropb->SetFloatValue( 2.f, -1.f );
		pDungeonWorldColorGroup->AddSubItem(pColorPropb);
	}
	m_wndPropList.AddProperty(pDungeonWorldColorGroup);
	pDungeonWorldColorGroup->Expand(FALSE);

	CMFCPropertyGridProperty* pDungeonUnitColorGroup = new CMFCPropertyGridProperty(_T("Unit Color"), DUNGEON_UNIT_COLOR_GROUP);
	{
		float fVal = 1.f;
		CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, DUNGEON_UNIT_COLOR_R );
		pColorPropr->SetRange( 0, 200 );
		pColorPropr->SetFloatValue( 2.f, -1.f );
		pDungeonUnitColorGroup->AddSubItem(pColorPropr);

		CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, DUNGEON_UNIT_COLOR_G );
		pColorPropg->SetRange( 0, 200 );
		pColorPropg->SetFloatValue( 2.f, -1.f );
		pDungeonUnitColorGroup->AddSubItem(pColorPropg);

		CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, DUNGEON_UNIT_COLOR_B );
		pColorPropb->SetRange( 0, 200 );
		pColorPropb->SetFloatValue( 2.f, -1.f );
		pDungeonUnitColorGroup->AddSubItem(pColorPropb);
	}
	m_wndPropList.AddProperty(pDungeonUnitColorGroup);
	pDungeonUnitColorGroup->Expand(FALSE);

	CMFCPropertyGridProperty* pDungeonStageGroup = new CMFCPropertyGridProperty(_T("Stage"), ED_STAGE_GROUP);
	{
		CMFCPropertyGridProperty* pWorldId = new CMFCPropertyGridProperty(_T("World ID"), _T("WI_NONE"), _T("World ID"), ED_WORLD_ID);		
		pWorldId->AllowEdit(FALSE);
		pDungeonStageGroup->AddSubItem( pWorldId );
		CUserPropertyGridFile* pWorldName = new CUserPropertyGridFile(_T("World Name"), TRUE, _T(""), _T(""), 0, _T("lua script (*.lua)|*.lua|"), _T("월드스크립트를 지정합니다."), ED_WORLD_NAME);
		pDungeonStageGroup->AddSubItem( pWorldName );
		pDungeonStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Start Motion"), (_variant_t) (bool)true, _T("Start Motion"), ED_START_MOTION));
		pDungeonStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Secret Stage"), (_variant_t) (bool)true, _T("Secret Stage"), ED_SECRET_STAGE));
		CMFCPropertyGridProperty* pStageType = new CMFCPropertyGridProperty(_T("Stage Type"), _T("ST_NONE"), _T("Stage Type"), ED_STAGE_TYPE);
		for(int i=0; i<ARRAY_SIZE(wstrStageType); ++i)
		{
			pStageType->AddOption(wstrStageType[i].c_str());
		}
		pStageType->AllowEdit(FALSE);
		pDungeonStageGroup->AddSubItem( pStageType );
		
		CMFCPropertyGridProperty* pWorldColorGroup = new CMFCPropertyGridProperty(_T("World Color"), ED_WORLD_COLOR_GROUP);
		{
			float fVal = 1.f;
			CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, ED_WORLD_COLOR_R );
			pColorPropr->SetRange( 0, 200 );
			pColorPropr->SetFloatValue( 2.f, -1.f );
			pWorldColorGroup->AddSubItem(pColorPropr);

			CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, ED_WORLD_COLOR_G );
			pColorPropg->SetRange( 0, 200 );
			pColorPropg->SetFloatValue( 2.f, -1.f );
			pWorldColorGroup->AddSubItem(pColorPropg);

			CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, ED_WORLD_COLOR_B );
			pColorPropb->SetRange( 0, 200 );
			pColorPropb->SetFloatValue( 2.f, -1.f );
			pWorldColorGroup->AddSubItem(pColorPropb);
		}
		pDungeonStageGroup->AddSubItem( pWorldColorGroup );
		pWorldColorGroup->Expand(FALSE);

		CMFCPropertyGridProperty* pUnitColorGroup = new CMFCPropertyGridProperty(_T("Unit Color"), ED_UNIT_COLOR_GROUP);
		{
			float fVal = 1.f;
			CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, ED_UNIT_COLOR_R );
			pColorPropr->SetRange( 0, 200 );
			pColorPropr->SetFloatValue( 2.f, -1.f );
			pUnitColorGroup->AddSubItem(pColorPropr);

			CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, ED_UNIT_COLOR_G );
			pColorPropg->SetRange( 0, 200 );
			pColorPropg->SetFloatValue( 2.f, -1.f );
			pUnitColorGroup->AddSubItem(pColorPropg);

			CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, ED_UNIT_COLOR_B );
			pColorPropb->SetRange( 0, 200 );
			pColorPropb->SetFloatValue( 2.f, -1.f );
			pUnitColorGroup->AddSubItem(pColorPropb);
		}
		pDungeonStageGroup->AddSubItem( pUnitColorGroup );
		pUnitColorGroup->Expand(FALSE);

	}
	m_wndPropList.AddProperty(pDungeonStageGroup);

	CMFCPropertyGridProperty* pDungeonSubStageGroup = new CMFCPropertyGridProperty(_T("Sub Stage"), ED_SUB_STAGE_GROUP);
	{
		pDungeonSubStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Start LineSet"), (_variant_t) (long)-1, _T("Start LineSet"), ED_START_LINE_SET ));
		pDungeonSubStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Main LineSet"), (_variant_t) (long)-1, _T("Main LineSet"), ED_MAIN_LINE_SET ));
		pDungeonSubStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("End LineSet"), (_variant_t) (long)-1, _T("End LineSet"), ED_END_LINE_SET ));
		CMFCPropertyGridProperty* pGoType = new CMFCPropertyGridProperty(_T("Go Type"), _T("GT_NONE"), _T("Go Type"), ED_GO_TYPE);
		for(int i=0; i<ARRAY_SIZE(wstrGoType); ++i)
		{
			pGoType->AddOption(wstrGoType[i].c_str());
		}
		pGoType->AllowEdit(FALSE);
		pDungeonSubStageGroup->AddSubItem( pGoType );
		pDungeonSubStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Use Pos NPC"), (_variant_t) (bool)false, _T("Use pos NPC for rebirth"), ED_USE_POS_OF_MONSTER_FOR_REBIRTH ));
		pDungeonSubStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("KeyCode NPC"), (_variant_t) (long)-1, _T("Key Code of NPC for rebirth"), ED_KEY_CODE_OF_MONSTER_FOR_REBIRTH ));

		CMFCPropertyGridProperty* pStartStateGroup = new CMFCPropertyGridProperty(_T("npc start state group"), ED_KEYCODE_N_START_STATE_GROUP);
		{
			pStartStateGroup->AddSubItem(new CMFCPropertyGridProperty(_T("NPC KeyCode"), (_variant_t) (long)-1, _T("해당 기능은 툴에서 지원하지 않습니다.\n해당기능을 사용하시려면 스크립트에서 직접 작성하세요"), ED_KEYCODE_N_START_STATE_KEYCODE ));
			pStartStateGroup->AddSubItem(new CMFCPropertyGridProperty(_T("NPC Start State"), (_variant_t)L"", _T("해당 기능은 툴에서 지원하지 않습니다.\n해당기능을 사용하시려면 스크립트에서 직접 작성하세요"), ED_KEYCODE_N_START_STATE_STARTSTATE ));
		}
		pDungeonSubStageGroup->AddSubItem(pStartStateGroup);
		pStartStateGroup->Expand(FALSE);
	}
	m_wndPropList.AddProperty(pDungeonSubStageGroup);

	CMFCPropertyGridProperty* pDungeonCurtainGroup = new CMFCPropertyGridProperty(_T("Curtain Group"), ED_CURTAIN_GROUP);
	{
		pDungeonCurtainGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Line Index"), (_variant_t) (long)-1, _T(""), ED_LINE_INDEX ));
		pDungeonCurtainGroup->AddSubItem(new CMFCPropertyGridProperty(_T("At Start"), (_variant_t) (bool)true, _T(""), ED_AT_START ));
		pDungeonCurtainGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Look Left"), (_variant_t) (bool)false, _T(""), ED_LOOK_LEFT ));
	}
	m_wndPropList.AddProperty(pDungeonCurtainGroup);

	CMFCPropertyGridProperty* pDungeonClearConditionGroup = new CMFCPropertyGridProperty(_T("Clear Condition Group"), ED_CLEAR_CONDITION_GROUP);
	{
		CMFCPropertyGridProperty* pStageClearType = new CMFCPropertyGridProperty(_T("Clear Condition Type"), _T("CC_KILL_ALL_ACTIVE_NPC"), _T(""), ED_CLEAR_CONDITION_TYPE);
		for(int i=0; i<ARRAY_SIZE(wstrStageClearType); ++i)
		{
			pStageClearType->AddOption(wstrStageClearType[i].c_str());
		}
		pStageClearType->AllowEdit(FALSE);
		pDungeonClearConditionGroup->AddSubItem( pStageClearType );

		CMFCPropertyGridProperty* pDungeonClearConditionDataGroup = new CMFCPropertyGridProperty(_T("Clear Condition Data Group"), ED_CLEAR_CONDITION_DATA_GROUP);
		{			
			CMFCPropertyGridProperty* pNpcId = new CMFCPropertyGridProperty(_T("Npc Id"), _T("NUI_NONE"), _T(""), ED_CC_NPC_ID);
			pDungeonClearConditionDataGroup->AddSubItem( pNpcId );
			pDungeonClearConditionDataGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Portal Line Index"), (_variant_t) (long)-1, _T(""), ED_CC_PORTAL_LINE_INDEX ));
			pDungeonClearConditionDataGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Npc KeyCode"), (_variant_t) (long)-1, _T(""), ED_CC_NPC_KEYCODE ));
			pDungeonClearConditionDataGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Npc Count"), (_variant_t) (long)-1, _T(""), ED_CC_NPC_COUNT ));
			pDungeonClearConditionDataGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Clear Time"), (_variant_t) (float)0.f, _T(""), ED_CC_CLEAR_TIME ));
		}
		pDungeonClearConditionGroup->AddSubItem(pDungeonClearConditionDataGroup);

		CMFCPropertyGridProperty* pDungeonNextStageGroup = new CMFCPropertyGridProperty(_T("Next Stage Group"), ED_CLEAR_CONDITION_NEXT_STAGE_GROUP);
		{
			CMFCPropertyGridProperty* pClearType = new CMFCPropertyGridProperty(_T("Clear Type"), _T("CT_SUB_STAGE"), _T(""), ED_CLEAR_CONDITION_NEXT_STAGE_CLEAR_TYPE);
			for(int i=0; i<ARRAY_SIZE(wstrClearType); ++i)
			{
				pClearType->AddOption(wstrClearType[i].c_str());
			}
			pClearType->AllowEdit(FALSE);
			pDungeonNextStageGroup->AddSubItem( pClearType );
			pDungeonNextStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Next Stage Index"), (_variant_t) (long)-1, _T(""), ED_CLEAR_CONDITION_NEXT_STAGE_STAGE_INDEX ));
			pDungeonNextStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Next SubStage Index"), (_variant_t) (long)-1, _T(""), ED_CLEAR_CONDITION_NEXT_STAGE_SUBSTAGE_INDEX ));
			pDungeonNextStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Next Stage Rate"), (_variant_t) (long)100, _T(""), ED_CLEAR_CONDITION_NEXT_STAGE_RATE ));
			pDungeonNextStageGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Ending Speech Index"), (_variant_t) (long)-1, _T(""), ED_CLEAR_CONDITION_NEXT_STAGE_ENDINGSPEECHINDEX ));
		}
		pDungeonClearConditionGroup->AddSubItem(pDungeonNextStageGroup);
	}
	m_wndPropList.AddProperty(pDungeonClearConditionGroup);	

	CMFCPropertyGridProperty* pNpcGroup = new CMFCPropertyGridProperty(_T("NPC Group"), ED_NPC_GROUP);
	{
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Npc Id"), (_variant_t)_T("NUI_NONE"), _T(""), ED_NPC_UNITID ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("KeyCode"), (_variant_t) (long)-1, _T(""), ED_NPC_KEYCODE ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("FocusCamera"), (_variant_t) (bool)false, _T(""), ED_NPC_FOCUSCAMERA ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("StartPos"), (_variant_t)_T(""), _T(""), ED_NPC_STARTPOS ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Active"), (_variant_t)(bool)true, _T(""), ED_NPC_ACTIVE ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("ShowGage"), (_variant_t)(bool)true, _T(""), ED_NPC_SHOWGAGE ));
		CMFCPropertyGridProperty* pNpcAddPosGroup = new CMFCPropertyGridProperty(_T("Add Pos"), ED_NPC_ADDPOS_GROUP);
		{
			pNpcAddPosGroup->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t) (float)0.f, _T(""), ED_NPC_ADDPOS_X ));
			pNpcAddPosGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t) (float)0.f, _T(""), ED_NPC_ADDPOS_Y ));
			pNpcAddPosGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t) (float)0.f, _T(""), ED_NPC_ADDPOS_Z ));
		}
		pNpcGroup->AddSubItem(pNpcAddPosGroup);
		pNpcAddPosGroup->Expand(FALSE);
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Level"), (_variant_t) (long)-1, _T(""), ED_NPC_LEVEL ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Rate"), (_variant_t) (long)100, _T(""), ED_NPC_RATE ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Has BossGage"), (_variant_t) (bool)false, _T(""), ED_NPC_HASBOSSGAGE ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Show BossName"), (_variant_t) (bool)false, _T(""), ED_NPC_SHOWBOSSNAME ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Show SubBossName"), (_variant_t) (bool)false, _T(""), ED_NPC_SHOWSUBBOSSNAME ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Siege Mode"), (_variant_t) (bool)false, _T(""), ED_NPC_SIEGEMODE ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Is Right"), (_variant_t) (bool)false, _T(""), ED_NPC_ISRIGHT ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Unit Scale"), (_variant_t) (float)-1.f, _T(""), ED_NPC_UNITSCALE ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("HP Lines"), (_variant_t) (long)1, _T(""), ED_NPC_BOSSGAUAGEHPLINES ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Ally Npc"), (_variant_t) (bool)false, _T(""), ED_NPC_ALLYNPC ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("NoDrop"), (_variant_t) (bool)false, _T(""), ED_NPC_NODROP ));		
		CMFCPropertyGridProperty* pMongerGrade = new CMFCPropertyGridProperty(_T("Monster Grade"), _T("MG_NORMAL_NPC"), _T("Monster Grade"), ED_NPC_MONSTER_GRADE);
		for(int i=0; i<ARRAY_SIZE(wstrMonsterGrade); ++i)
		{
			pMongerGrade->AddOption(wstrMonsterGrade[i].c_str());
		}
		pMongerGrade->AllowEdit(FALSE);
		pNpcGroup->AddSubItem( pMongerGrade );
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("SubNpc"), (_variant_t) (bool)false, _T(""), ED_NPC_SUBNPC ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("SubNpcIndex"), (_variant_t) (long)-1, _T(""), ED_NPC_SUBNPCINDEX ));
		pNpcGroup->AddSubItem(new CMFCPropertyGridProperty(_T("SubNpcRate"), (_variant_t) (long)100, _T(""), ED_NPC_SUBNPCRATE ));
	}
	m_wndPropList.AddProperty(pNpcGroup);

	CMFCPropertyGridProperty* pTriggerGroup = new CMFCPropertyGridProperty(_T("Trigger Group"), ED_TRIGGER_GROUP);
	{
		pTriggerGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Host"), (_variant_t)(bool)false, _T(""),  ED_TRIGGER_HOST ));
		pTriggerGroup->AddSubItem(new CMFCPropertyGridProperty(_T("One Time"), (_variant_t)(bool)true, _T(""),  ED_TRIGGER_ONTIME ));
		pTriggerGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Interval"), (_variant_t)(float)0.f, _T(""),  ED_TRIGGER_INTERVAL ));
		pTriggerGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Condition Function"), (_variant_t)_T(""), _T(""),  ED_TRIGGER_CONDITION_FUNC ));
		pTriggerGroup->AddSubItem(new CMFCPropertyGridProperty(_T("React Function"), (_variant_t)_T(""), _T(""),  ED_TRIGGER_REACT_FUNC ));
	}
	m_wndPropList.AddProperty(pTriggerGroup);

	UpdateProp(0);
}

void CEditDungeon::InitClearCondData()
{
	GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(FALSE);
	GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
	GetProp(ED_CC_NPC_KEYCODE)->Show(FALSE);
	GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
	GetProp(ED_CC_NPC_ID)->Show(FALSE);
	GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);
}

void CEditDungeon::UpdateProp(int iFlag)
{
	GetProp(DUNGEON_WORLD_COLOR_GROUP)->Show(FALSE);
	GetProp(DUNGEON_UNIT_COLOR_GROUP)->Show(FALSE);

	switch(iFlag)
	{
	case 0:
		{
			GetProp(ED_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_SUB_STAGE_GROUP)->Show(FALSE);
			GetProp(ED_CURTAIN_GROUP)->Show(FALSE);
			GetProp(ED_CLEAR_CONDITION_GROUP)->Show(FALSE);
			GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(FALSE);	
			GetProp(ED_CLEAR_CONDITION_NEXT_STAGE_GROUP)->Show(FALSE);
			GetProp(ED_NPC_GROUP)->Show(FALSE);
			GetProp(ED_TRIGGER_GROUP)->Show(FALSE);
		}
		break;
	case 1:
		{
			GetProp(ED_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_SUB_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_CURTAIN_GROUP)->Show(FALSE);
			GetProp(ED_CLEAR_CONDITION_GROUP)->Show(FALSE);
			GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(FALSE);	
			GetProp(ED_CLEAR_CONDITION_NEXT_STAGE_GROUP)->Show(FALSE);
			GetProp(ED_NPC_GROUP)->Show(FALSE);
			GetProp(ED_TRIGGER_GROUP)->Show(FALSE);
		}
		break;
	case 2:
		{
			GetProp(ED_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_SUB_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_CURTAIN_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);	
			GetProp(ED_CLEAR_CONDITION_NEXT_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_NPC_GROUP)->Show(FALSE);
			GetProp(ED_TRIGGER_GROUP)->Show(FALSE);
		}
		break;
	case 3:
		{
			GetProp(ED_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_SUB_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_CURTAIN_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);	
			GetProp(ED_CLEAR_CONDITION_NEXT_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_NPC_GROUP)->Show(FALSE);
			GetProp(ED_TRIGGER_GROUP)->Show(FALSE);
		}
		break;
	case 4:
		{
			GetProp(ED_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_SUB_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_CURTAIN_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);	
			GetProp(ED_CLEAR_CONDITION_NEXT_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_NPC_GROUP)->Show(FALSE);
			GetProp(ED_TRIGGER_GROUP)->Show(FALSE);
		}
		break;
	case 5:
		{
			GetProp(ED_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_SUB_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_CURTAIN_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);	
			GetProp(ED_CLEAR_CONDITION_NEXT_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_NPC_GROUP)->Show(TRUE);
			GetProp(ED_TRIGGER_GROUP)->Show(FALSE);
		}
		break;
	case 6:
		{
			GetProp(ED_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_SUB_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_CURTAIN_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_GROUP)->Show(TRUE);
			GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);	
			GetProp(ED_CLEAR_CONDITION_NEXT_STAGE_GROUP)->Show(TRUE);
			GetProp(ED_NPC_GROUP)->Show(TRUE);
			GetProp(ED_TRIGGER_GROUP)->Show(TRUE);
		}
		break;
	}
}

void CEditDungeon::UpdateDungeonProp()
{
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	D3DXCOLOR cWorldColor = pDungeon->GetWorldColor();
	D3DXCOLOR cUnitColor = pDungeon->GetUnitColor();

	SetPropValue( DUNGEON_WORLD_COLOR_R, (_variant_t)(float)cWorldColor.r );
	SetPropValue( DUNGEON_WORLD_COLOR_G, (_variant_t)(float)cWorldColor.g );
	SetPropValue( DUNGEON_WORLD_COLOR_B, (_variant_t)(float)cWorldColor.b );
	SetPropValue( DUNGEON_UNIT_COLOR_R, (_variant_t)(float)cUnitColor.r );
	SetPropValue( DUNGEON_UNIT_COLOR_G, (_variant_t)(float)cUnitColor.g );
	SetPropValue( DUNGEON_UNIT_COLOR_B, (_variant_t)(float)cUnitColor.b );

}

void CEditDungeon::UpdateStageProp()
{
	if( m_iSelectedStage < 0 )
		return;

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData *pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	const vector<CX2World::WorldData*> &vecWorldList = g_pData->GetWorldManager()->GetWorldDataList();
	static bool bSettingWorldId = false;
	if( bSettingWorldId == false )
	{		
		CMFCPropertyGridProperty *pProp = GetProp(ED_WORLD_ID);
		for(UINT i=0; i<vecWorldList.size(); ++i)
		{
			pProp->AddOption(vecWorldList[i]->worldName.c_str());
		}
				
		pProp = GetProp(ED_CC_NPC_ID);		
		std::vector<pair<std::wstring, CX2UnitManager::NPC_UNIT_ID>> vecSorted;
		std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring > mapNpcEnum = pToolMain->m_enumManager.GetMapNpcEnum();
		std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator it = mapNpcEnum.begin();
		std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator et = mapNpcEnum.end();
		for(; it!=et; ++it)
			vecSorted.push_back(make_pair( it->second, it->first ));

		sort( vecSorted.begin(), vecSorted.end() );
		
		std::vector<pair<std::wstring, CX2UnitManager::NPC_UNIT_ID>>::iterator mit = vecSorted.begin();
		while(mit != vecSorted.end())
		{
			wstring wstrNpcId = mit->first;
			pProp->AddOption(wstrNpcId.c_str());
			++mit;
		}

		bSettingWorldId = true;
	}

	bool bFindWorldId = false;
	for(UINT i=0; i<vecWorldList.size(); ++i)
	{
		if( pStageData->m_WorldID == vecWorldList[i]->worldID )
		{
			SetPropValue( ED_WORLD_ID, (_variant_t)vecWorldList[i]->worldName.c_str() );
			SetPropValue( ED_WORLD_NAME, (_variant_t)vecWorldList[i]->dataFileName.c_str() );
			bFindWorldId = true;
			break;
		}
	}
	if( bFindWorldId == false )
	{
		wstring wstrWorldDataName = pStageData->m_wstrWorldDataName;
		SetPropValue( ED_WORLD_ID, (_variant_t)L"WI_NONE" );
		SetPropValue( ED_WORLD_NAME, (_variant_t)wstrWorldDataName.c_str() );
	}

	SetPropValue( ED_START_MOTION, (_variant_t)(bool)pStageData->m_bStartMotion );
	SetPropValue( ED_SECRET_STAGE, (_variant_t)(bool)pStageData->m_bSecretStage );
	SetPropValue( ED_STAGE_TYPE, (_variant_t)wstrStageType[(int)pStageData->m_eStageType].c_str() );
	SetPropValue( ED_WORLD_COLOR_R, (_variant_t)(float)pStageData->m_WorldColor.r );
	SetPropValue( ED_WORLD_COLOR_G, (_variant_t)(float)pStageData->m_WorldColor.g );
	SetPropValue( ED_WORLD_COLOR_B, (_variant_t)(float)pStageData->m_WorldColor.b );
	SetPropValue( ED_UNIT_COLOR_R, (_variant_t)(float)pStageData->m_UnitColor.r );
	SetPropValue( ED_UNIT_COLOR_G, (_variant_t)(float)pStageData->m_UnitColor.g );
	SetPropValue( ED_UNIT_COLOR_B, (_variant_t)(float)pStageData->m_UnitColor.b );
}

void CEditDungeon::UpdateSubStageProp()
{
	if( m_iSelectedStage < 0 || m_iSelectedSubStage < 0 )
		return;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData *pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( pSubStageData != NULL )
	{
		SetPropValue( ED_MAIN_LINE_SET, (_variant_t)(long)pSubStageData->m_MainLineSet );
		SetPropValue( ED_START_LINE_SET, (_variant_t)(long)pSubStageData->m_StartLineSet );
		SetPropValue( ED_END_LINE_SET, (_variant_t)(long)pSubStageData->m_EndLineSet );
		SetPropValue( ED_GO_TYPE, (_variant_t)wstrGoType[(int)pSubStageData->m_GoType].c_str() );
		SetPropValue( ED_USE_POS_OF_MONSTER_FOR_REBIRTH, (_variant_t)(bool)pSubStageData->m_bUsePosOfMonsterForRebirth );
		SetPropValue( ED_KEY_CODE_OF_MONSTER_FOR_REBIRTH, (_variant_t)(long)pSubStageData->m_iKeyCodeOfMonsterForRebirth );
	}
}

void CEditDungeon::UpdateCurtainProp(int iIndex)
{
	if( m_iSelectedStage < 0 || m_iSelectedSubStage < 0 || iIndex < 0 )
		return;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData *pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( pSubStageData != NULL )
	{
		if( iIndex >= (int)pSubStageData->m_CurtainDataList.size() )
			return;

		CX2DungeonSubStage::CurtainData *pCurtainData = pSubStageData->m_CurtainDataList[iIndex];
		if( pCurtainData != NULL )
		{
			SetPropValue( ED_LINE_INDEX, (_variant_t)(long)pCurtainData->m_LineIndex );
			SetPropValue( ED_AT_START, (_variant_t)(bool)pCurtainData->m_bAtStart );
			SetPropValue( ED_LOOK_LEFT, (_variant_t)(bool)pCurtainData->m_bLookLeft );
		}
	}
}

void CEditDungeon::UpdateClearConditionProp(int iIndex)
{
	if( m_iSelectedStage < 0 || m_iSelectedSubStage < 0 || iIndex < 0 )
	{
		GetProp(ED_CLEAR_CONDITION_GROUP)->Show(FALSE);
		return;
	}

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData *pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( pSubStageData != NULL )
	{
		if( iIndex >= (int)pSubStageData->m_vecClearCondData.size() )
			return;

		CX2DungeonSubStage::ClearCondtionData clearCond = pSubStageData->m_vecClearCondData[iIndex];
		SetPropValue( ED_CLEAR_CONDITION_TYPE, (_variant_t)wstrStageClearType[(int)clearCond.m_eClearCondition].c_str() );
		
		// prop setting
	}
}

void CEditDungeon::UpdateClearConditionNextStageProp(int iIndex)
{
	if( m_iSelectedStage < 0 || m_iSelectedSubStage < 0 || m_iSelectedClearCondition < 0 || iIndex < 0 )
	{
		GetProp(ED_CLEAR_CONDITION_NEXT_STAGE_GROUP)->Show(FALSE);
		return;
	}

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData *pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( pSubStageData != NULL )
	{
		if( iIndex >= (int)pSubStageData->m_vecClearCondData.size() )
			return;

		CX2DungeonSubStage::ClearCondtionData clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];

		if( iIndex >= (int)clearCond.m_vecNextStage.size() )
			return;

		CX2DungeonSubStage::NextStageData nextStage = clearCond.m_vecNextStage[iIndex];

		SetPropValue( ED_CLEAR_CONDITION_NEXT_STAGE_CLEAR_TYPE, (_variant_t)wstrClearType[(int)nextStage.m_eClearType].c_str() );
		SetPropValue( ED_CLEAR_CONDITION_NEXT_STAGE_STAGE_INDEX, (_variant_t)(long)nextStage.m_iStageIndex );
		SetPropValue( ED_CLEAR_CONDITION_NEXT_STAGE_SUBSTAGE_INDEX, (_variant_t)(long)nextStage.m_iSubStageIndex );
		SetPropValue( ED_CLEAR_CONDITION_NEXT_STAGE_RATE, (_variant_t)(long)nextStage.m_iRate );
		SetPropValue( ED_CLEAR_CONDITION_NEXT_STAGE_ENDINGSPEECHINDEX, (_variant_t)(long)nextStage.m_iDungeonEndingSpeechTableIndex );
	}
}

void CEditDungeon::UpdateClearCondDataProp(int iIndex)
{
	if( m_iSelectedStage < 0 || m_iSelectedSubStage < 0 || m_iSelectedClearCondition < 0 || iIndex < 0 )
	{
		GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(FALSE);
		return;
	}

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData *pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( pSubStageData != NULL )
	{
		if( m_iSelectedClearCondition >= (int)pSubStageData->m_vecClearCondData.size() )
		{
			GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(FALSE);
			return;
		}

		CX2DungeonSubStage::ClearCondtionData clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];

		switch(clearCond.m_eClearCondition)
		{
		case CX2DungeonSubStage::CC_KILL_ALL_ACTIVE_NPC:
			{
				GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(FALSE);
			}
			break;
		case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
		case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:
			{
				GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
				GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
				GetProp(ED_CC_NPC_KEYCODE)->Show(TRUE);
				GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
				GetProp(ED_CC_NPC_ID)->Show(FALSE);
				GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);

				int iKeyCode = clearCond.m_vecNPCKeyCode[iIndex].m_iKey;
				SetPropValue( ED_CC_NPC_KEYCODE, (_variant_t)(long)iKeyCode);
			}
			break;
		case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
			{
				GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
				GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
				GetProp(ED_CC_NPC_KEYCODE)->Show(FALSE);
				GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
				GetProp(ED_CC_NPC_ID)->Show(TRUE);
				GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);

				CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;				
				int iKeyCode = clearCond.m_vecNPCID[iIndex].m_iKey;
				wstring wstrNpcId = pToolMain->m_enumManager.GetNpcEnumString((CX2UnitManager::NPC_UNIT_ID)iKeyCode);
				SetPropValue( ED_CC_NPC_ID, (_variant_t)wstrNpcId.c_str());
			}
			break;
		case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_COUNT:
			{
				GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
				GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
				GetProp(ED_CC_NPC_KEYCODE)->Show(TRUE);
				GetProp(ED_CC_NPC_COUNT)->Show(TRUE);
				GetProp(ED_CC_NPC_ID)->Show(FALSE);
				GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);

				int iKeyCode = clearCond.m_vecNPCKeyCode[iIndex].m_iKey;
				int iCount = clearCond.m_vecNPCKeyCode[iIndex].m_iObjectiveCount;
				SetPropValue( ED_CC_NPC_KEYCODE, (_variant_t)(long)iKeyCode);
				SetPropValue( ED_CC_NPC_COUNT, (_variant_t)(long)iCount);
			}
			break;
		case CX2DungeonSubStage::CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC:
			{
				GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
				GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(TRUE);
				GetProp(ED_CC_NPC_KEYCODE)->Show(FALSE);
				GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
				GetProp(ED_CC_NPC_ID)->Show(FALSE);
				GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);

				int iPortalIndex = clearCond.m_vecPortalLineIndex[iIndex];
				SetPropValue( ED_CLEAR_CONDITION_DATA_GROUP, (_variant_t)(long)iPortalIndex);
			}
			break;
		case CX2DungeonSubStage::CC_TIME:
			{
				GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
				GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
				GetProp(ED_CC_NPC_KEYCODE)->Show(FALSE);
				GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
				GetProp(ED_CC_NPC_ID)->Show(FALSE);
				GetProp(ED_CC_CLEAR_TIME)->Show(TRUE);

				float fClearTime = clearCond.m_fClearTime;
				SetPropValue( ED_CLEAR_CONDITION_DATA_GROUP, (_variant_t)(float)fClearTime);
			}
			break;
		default:
			break;
		}
	}
}

int CEditDungeon::UpdateNpcProp(int iIndex)
{
	static bool bSettingNpc = false;
	if( bSettingNpc == false )
	{
		CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
		CMFCPropertyGridProperty *pProp = GetProp(ED_NPC_UNITID);		
		std::vector<pair<std::wstring, CX2UnitManager::NPC_UNIT_ID>> vecSorted;
		std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring > mapNpcEnum = pToolMain->m_enumManager.GetMapNpcEnum();
		std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator it = mapNpcEnum.begin();
		std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator et = mapNpcEnum.end();
		for(; it!=et; ++it)
			vecSorted.push_back(make_pair( it->second, it->first ));

		sort( vecSorted.begin(), vecSorted.end() );

		std::vector<pair<std::wstring, CX2UnitManager::NPC_UNIT_ID>>::iterator mit = vecSorted.begin();
		while(mit != vecSorted.end())
		{
			wstring wstrNpcId = mit->first;
			pProp->AddOption(wstrNpcId.c_str());
			++mit;
		}

		bSettingNpc = true;
	}

	int iSelectedNpcIndex = -1;
	if( iIndex >= 0 )
	{
		iSelectedNpcIndex = iIndex;
	}
	else
	{		
		for(int i=0; i<m_listNpc.GetItemCount(); ++i)
		{
			UINT uItemState = m_listNpc.GetItemState ( i, LVIS_SELECTED );
			if( uItemState == LVIS_SELECTED )
			{
				iSelectedNpcIndex = i;
				break;
			}
		}
	}

	if( m_iSelectedStage < 0 || m_iSelectedSubStage < 0 || iSelectedNpcIndex < 0 )
	{
		GetProp(ED_NPC_GROUP)->Show(FALSE);
		return -1;
	}

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	
	if( m_iSelectedStage >= (int)pDungeon->GetStageDataList().size() )
	{
		GetProp(ED_NPC_GROUP)->Show(FALSE);
		return -1;
	}

	CX2DungeonStage::StageData *pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];

	if( m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
	{
		GetProp(ED_NPC_GROUP)->Show(FALSE);
		return -1;
	}

	CX2DungeonSubStage::SubStageData *pSubStageData = pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( iIndex >= (int)pSubStageData->m_vecSubStageNpcData.size() )
	{
		GetProp(ED_NPC_GROUP)->Show(FALSE);
		return -1;
	}

	CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[iSelectedNpcIndex];
	if( pNpcData != NULL )
	{
		GetProp(ED_NPC_GROUP)->Show(TRUE);

		CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;		
		wstring wstrNpcId = pToolMain->m_enumManager.GetNpcEnumString((CX2UnitManager::NPC_UNIT_ID)pNpcData->m_UnitID);
		
		SetPropValue( ED_NPC_UNITID, (_variant_t)wstrNpcId.c_str());
		SetPropValue( ED_NPC_KEYCODE, (_variant_t)(long)pNpcData->m_KeyCode );
		SetPropValue( ED_NPC_FOCUSCAMERA, (_variant_t)(bool)pNpcData->m_bFocusCamera );
		wstring wstrStartPos = L"";
		WCHAR wcBuf[10];
		for(UINT i=0; i<pNpcData->m_vecStartPos.size(); ++i)
		{
			if( i == pNpcData->m_vecStartPos.size() - 1)
				wsprintfW(wcBuf, L"%d", pNpcData->m_vecStartPos[i]);
			else
				wsprintfW(wcBuf, L"%d, ", pNpcData->m_vecStartPos[i]);
			wstrStartPos = wstrStartPos + wcBuf;
		}		
		SetPropValue( ED_NPC_STARTPOS, (_variant_t)wstrStartPos.c_str() );
		SetPropValue( ED_NPC_ACTIVE, (_variant_t)(bool)pNpcData->m_bActive );
		SetPropValue( ED_NPC_SHOWGAGE, (_variant_t)(bool)pNpcData->m_bShowGage );
		SetPropValue( ED_NPC_ADDPOS_X, (_variant_t)(float)pNpcData->m_AddPos.x );
		SetPropValue( ED_NPC_ADDPOS_Y, (_variant_t)(float)pNpcData->m_AddPos.y );
		SetPropValue( ED_NPC_ADDPOS_Z, (_variant_t)(float)pNpcData->m_AddPos.z );
		SetPropValue( ED_NPC_LEVEL, (_variant_t)(long)pNpcData->m_iLevel4Tool );
		SetPropValue( ED_NPC_RATE, (_variant_t)(long)pNpcData->m_Rate );
		SetPropValue( ED_NPC_HASBOSSGAGE, (_variant_t)(bool)pNpcData->m_bHasBossGage );
		SetPropValue( ED_NPC_SHOWBOSSNAME, (_variant_t)(bool)pNpcData->m_bShowBossName );
		SetPropValue( ED_NPC_SHOWSUBBOSSNAME, (_variant_t)(bool)pNpcData->m_bShowSubBossName );
		SetPropValue( ED_NPC_SIEGEMODE, (_variant_t)(bool)pNpcData->m_bSiegeMode );
		SetPropValue( ED_NPC_ISRIGHT, (_variant_t)(bool)pNpcData->m_bIsRight );
		SetPropValue( ED_NPC_UNITSCALE, (_variant_t)(float)pNpcData->m_fUnitScale );
		SetPropValue( ED_NPC_BOSSGAUAGEHPLINES, (_variant_t)(long)pNpcData->m_usBossGaugeHPLines );
		SetPropValue( ED_NPC_ALLYNPC, (_variant_t)(bool)pNpcData->m_bAllyNpc );
		SetPropValue( ED_NPC_NODROP, (_variant_t)(bool)pNpcData->m_bNoDrop );
		SetPropValue( ED_NPC_MONSTER_GRADE, (_variant_t)wstrMonsterGrade[pNpcData->m_MonsterGrade].c_str() );
		SetPropValue( ED_NPC_SUBNPC, (_variant_t)(bool)pNpcData->m_bSubNpc );
		SetPropValue( ED_NPC_SUBNPCINDEX, (_variant_t)(long)pNpcData->m_iSubNPCIndex );
		SetPropValue( ED_NPC_SUBNPCRATE, (_variant_t)(long)pNpcData->m_iSubNPCRate );

		if( m_iSelectedStage == pDungeon->GetNowStageIndex() )
			pToolMain->SelectStartPos(pNpcData->m_vecStartPos[0]);
	}

	return iSelectedNpcIndex;
}

void CEditDungeon::UpdateTriggerProp(int iIndex)
{
	if( m_iSelectedStage < 0 || m_iSelectedSubStage < 0 || iIndex < 0 )
	{
		GetProp(ED_TRIGGER_GROUP)->Show(FALSE);
		return;
	}

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData *pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( iIndex >= (int)pSubStageData->m_TriggerList.size() )
	{
		GetProp(ED_TRIGGER_GROUP)->Show(FALSE);
		return;
	}

	CX2DungeonSubStage::Trigger* pTrigger = pSubStageData->m_TriggerList[iIndex];
	if( pTrigger != NULL )
	{
		SetPropValue( ED_TRIGGER_HOST, (_variant_t)(bool)pTrigger->m_bHost );
		SetPropValue( ED_TRIGGER_ONTIME, (_variant_t)(bool)pTrigger->m_bOneTime );
		SetPropValue( ED_TRIGGER_INTERVAL, (_variant_t)(float)pTrigger->m_Interval );
		SetPropValue( ED_TRIGGER_CONDITION_FUNC, (_variant_t)(WCHAR*)pTrigger->m_ConditionFunc.c_str() );
		SetPropValue( ED_TRIGGER_REACT_FUNC, (_variant_t)(WCHAR*)pTrigger->m_ReactFunc.c_str() );
	}
}

void CEditDungeon::ResetWorld(wstring wstrWorldFullName)
{
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	int iStageIndex = pDungeon->GetNowStageIndex();
	
	CX2DungeonStage::StageData* pStageData = pDungeon->GetNowStage()->GetStageData();
	pStageData->m_wstrWorldDataName = wstrWorldFullName;

	SetPropValue( ED_WORLD_NAME, (_variant_t)wstrWorldFullName.c_str() );
	bool bFindID = false;
	wstring wstrWorldName = L"";
	const vector<CX2World::WorldData*> &vecWorldData = g_pData->GetWorldManager()->GetWorldDataList();
	for(UINT i=0; i<vecWorldData.size(); ++i)
	{
		if( wstrWorldFullName.compare( vecWorldData[i]->dataFileName ) == 0 )
		{
			pStageData->m_WorldID = vecWorldData[i]->worldID;
			wstrWorldName = vecWorldData[i]->worldName;
			SetPropValue( ED_WORLD_ID, (_variant_t)wstrWorldName.c_str() );

			m_listStage.SetItemText(m_iSelectedStage, 1, wstrWorldName.c_str());

			bFindID = true;
			break;
		}
	}
	if( bFindID == false )
	{
		pStageData->m_WorldID = CX2World::WI_NONE;
		SetPropValue( ED_WORLD_ID, (_variant_t)L"WI_NONE" );
		m_listStage.SetItemText(m_iSelectedStage, 1, L"WI_NONE");
	}
}

void CEditDungeon::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CEditDungeon::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID_DUNGEON, m_wndPropList);
	DDX_Control(pDX, IDC_LIST_STAGE, m_listStage);
	DDX_Control(pDX, IDC_LIST_SUB_STAGE, m_listSubStage);
	DDX_Control(pDX, IDC_LIST_READY_NPC, m_listReadyNpc);
	DDX_Control(pDX, IDC_LIST_CURTAIN_GROUP, m_listCurtainGroup);
	DDX_Control(pDX, IDC_LIST_CLEAR_CONDITION, m_listClearCondition);
	DDX_Control(pDX, IDC_LIST_CLEAR_CONDITION2, m_listNextStage);
	DDX_Control(pDX, IDC_LIST_CLEAR_CONDITION3, m_listClearCondData);
	DDX_Control(pDX, IDC_CHECK_NPC_START_POSITION, m_checkNpc);
	DDX_Control(pDX, IDC_LIST_NPC_GROUP, m_listNpc);
	DDX_Control(pDX, IDC_EDIT2, m_editDungeonLevel);
	DDX_Control(pDX, IDC_LIST_TRIGGER, m_listTriggerData);
}


BEGIN_MESSAGE_MAP(CEditDungeon, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE( AFX_WM_PROPERTY_CHANGED, OnPropertyChanged )
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STAGE, &CEditDungeon::OnNMDblclkListStage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STAGE, &CEditDungeon::OnLvnItemchangedListStage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SUB_STAGE, &CEditDungeon::OnNMDblclkListSubStage)
	ON_BN_CLICKED(IDC_BUTTON_NOWSTAGE, &CEditDungeon::OnBnClickedButtonNowstage)
	ON_WM_HSCROLL()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SUB_STAGE, &CEditDungeon::OnLvnItemchangedListSubStage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CURTAIN_GROUP, &CEditDungeon::OnLvnItemchangedListCurtainGroup)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLEAR_CONDITION, &CEditDungeon::OnLvnItemchangedListClearCondition)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLEAR_CONDITION2, &CEditDungeon::OnLvnItemchangedListClearCondition2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLEAR_CONDITION3, &CEditDungeon::OnLvnItemchangedListClearCondition3)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CEditDungeon::OnBnClickedMfcbutton3)
	ON_BN_CLICKED(IDC_MFCBUTTON4, &CEditDungeon::OnBnClickedMfcbutton4)
	ON_BN_CLICKED(IDC_MFCBUTTON5, &CEditDungeon::OnBnClickedMfcbutton5)
	ON_BN_CLICKED(IDC_MFCBUTTON6, &CEditDungeon::OnBnClickedMfcbutton6)
	ON_BN_CLICKED(IDC_MFCBUTTON7, &CEditDungeon::OnBnClickedMfcbutton7)
	ON_BN_CLICKED(IDC_MFCBUTTON8, &CEditDungeon::OnBnClickedMfcbutton8)
	ON_BN_CLICKED(IDC_MFCBUTTON9, &CEditDungeon::OnBnClickedMfcbutton9)
	ON_BN_CLICKED(IDC_MFCBUTTON10, &CEditDungeon::OnBnClickedMfcbutton10)
	ON_BN_CLICKED(IDC_MFCBUTTON19, &CEditDungeon::OnBnClickedMfcbutton19)
	ON_BN_CLICKED(IDC_MFCBUTTON20, &CEditDungeon::OnBnClickedMfcbutton20)
	ON_BN_CLICKED(IDC_MFCBUTTON13, &CEditDungeon::OnBnClickedMfcbutton13)
	ON_BN_CLICKED(IDC_MFCBUTTON14, &CEditDungeon::OnBnClickedMfcbutton14)
	ON_BN_CLICKED(IDC_MFCBUTTON15, &CEditDungeon::OnBnClickedMfcbutton15)
	ON_BN_CLICKED(IDC_MFCBUTTON16, &CEditDungeon::OnBnClickedMfcbutton16)
	ON_BN_CLICKED(IDC_MFCBUTTON17, &CEditDungeon::OnBnClickedMfcbutton17)
	ON_BN_CLICKED(IDC_MFCBUTTON18, &CEditDungeon::OnBnClickedMfcbutton18)
	ON_BN_CLICKED(IDC_CHECK_NPC_START_POSITION, &CEditDungeon::OnBnClickedCheckNpcStartPosition)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_READY_NPC, &CEditDungeon::OnLvnItemchangedListReadyNpc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NPC_GROUP, &CEditDungeon::OnLvnItemchangedListNpcGroup)
	ON_BN_CLICKED(IDC_MFCBUTTON11, &CEditDungeon::OnBnClickedMfcbutton11)
	ON_BN_CLICKED(IDC_MFCBUTTON12, &CEditDungeon::OnBnClickedMfcbutton12)
	ON_EN_CHANGE(IDC_EDIT2, &CEditDungeon::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditDungeon::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRIGGER, &CEditDungeon::OnLvnItemchangedListTrigger)
	ON_BN_CLICKED(IDC_MFCBUTTON21, &CEditDungeon::OnBnClickedMfcbutton21)
	ON_BN_CLICKED(IDC_MFCBUTTON22, &CEditDungeon::OnBnClickedMfcbutton22)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditDungeon::OnBnClickedButton2)
END_MESSAGE_MAP()


// CEditDungeon 메시지 처리기입니다.
int CEditDungeon::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
// 	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
// 	{
// 		TRACE0("속성 표를 만들지 못했습니다. \n");
// 		return -1;      // 만들지 못했습니다.
// 	}

	InitPropList();


	return 0;
}

void CEditDungeon::OnSize(UINT nType, int cx, int cy)
{
}

void CEditDungeon::OnSetFocus(CWnd* pOldWnd)
{
	m_wndPropList.SetFocus();
}

void CEditDungeon::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	SetPropListFont();

	AfxMessageBox(lpszSection, MB_OK);
}

void CEditDungeon::OnNMDblclkListStage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	if( pToolMain->m_bChangeStartPos == true)
	{
		AfxMessageBox(L"StartPosition이 변경되었습니다. 라인맵을 저장하신후 다시 시도해주세요!");
		return;
	}

	int iStage = pNMItemActivate->iItem;
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	if( pDungeonGame != NULL )
	{
		pDungeonGame->ChangeStage( iStage );

		UpdateReadyNpc();

		vector<CX2DungeonSubStage::SubStageData*> vecSubStage = pDungeonGame->GetDungeon()->GetNowStage()->GetStageData()->m_SubStageDataList;
		for(UINT i=0; i<vecSubStage.size(); ++i)
		{
			CX2DungeonSubStage::SubStageData *pSubStageData = vecSubStage[i];
			vector<CX2DungeonSubStage::Trigger*> vecTrigger = pSubStageData->m_TriggerList;
			for(UINT j=0; j<vecTrigger.size(); ++j)
			{
				CX2DungeonSubStage::Trigger *pTrigger = vecTrigger[j];
				pTrigger->m_bReacted = false;
				pTrigger->m_NowInterval = 0.f;
			}
		}
	}
}


void CEditDungeon::OnLvnItemchangedListStage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 || pNMLV->uNewState == 2 )
	{
		m_iSelectedStage = pNMLV->iItem;

		InitClearCondData();

		UpdateProp(0);
		UpdateStageProp();

		m_iSelectedReadyNpc = -1;
		m_iSelectedSubStage = 0;
		m_iSelectedCurtainData = 0;
		m_iSelectedClearCondition = 0;
		m_iSelectedNextStage = 0;
		m_iSelectedClearCondData = 0;
		m_iSelectedNpc = -1;
		m_iSelectedTrigger = -1;
		
		UpdateReadyNpc();
		UpdateSubStage();
		UpdateCurtainGroup();
		UpdateClearCondition();
		UpdateClearConditionNextStage();
		UpdateClearConditionData();
		UpdateNpc();
		UpdateTrigger();
	}	
}


void CEditDungeon::OnNMDblclkListSubStage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	if( pToolMain->m_bChangeStartPos == true)
	{
		AfxMessageBox(L"StartPosition이 변경되었습니다. 라인맵을 저장하신후 다시 시도해주세요!");
		return;
	}

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	int iNextStage = 0;
	int iNextSubStage = pNMItemActivate->iItem;
	int iNowStage = pDungeon->GetNowStageIndex();
	int iNowSubStage = pDungeon->GetNowStage()->GetNowSubStageIndex();

	if( pDungeonGame != NULL )
	{
		for(int i=0; i<m_listStage.GetItemCount(); ++i)
		{							
			UINT uItemState = m_listStage.GetItemState ( i, LVIS_SELECTED );
			if( uItemState == LVIS_SELECTED )
			{
				iNextStage = i;
				break;
			}
		}
	}
	if( iNowStage != iNextStage )
	{
		//pDungeonGame->StageLoading( iNextStage );
		//pDungeonGame->StageStart();
		pDungeonGame->ChangeStage( iNextStage );
		g_pInstanceData->m_bChangeStage = true;
	}

	pDungeonGame->SubStageOpen(iNextSubStage);
		
	CX2DungeonStage *pStage = pDungeon->GetNowStage();
	for( int i = 0; i < (int)pStage->GetStageData()->m_SubStageDataList.size(); ++i )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData	= pStage->GetStageData()->m_SubStageDataList[i];

		pSubStageData->m_NPCDataList.clear();
		if( pToolMain->m_bCanPlay == true )
		{
			pSubStageData->m_NPCDataList = pSubStageData->SummonNpcData4Tool(); //m_vecSubStageNpcData;
		}

		/* NPC 데이터 소멸 및 생성 부분 Field 형식으로 변경해야 됨*/
#ifdef CHANGE_NPC_LOAD
		pStage->FlushNPCs();
		pStage->RemoveReadyNPCResources();
		pStage->LoadReadyNPCResources();
#else
		for( UINT j = 0; j < pSubStageData->m_NPCList.size(); ++j )
		{
			CX2GUNPC *pNpc = pSubStageData->m_NPCList[j];			
			SAFE_DELETE_KTDGOBJECT( pNpc );
		}
		pSubStageData->m_NPCList.clear();

		if( i >= iNextSubStage && pToolMain->m_bCanPlay == true )
			pSubStageData->CreateNPC();
#endif

		vector<CX2DungeonSubStage::Trigger*> vecTrigger = pSubStageData->m_TriggerList;
		for(UINT j=0; j<vecTrigger.size(); ++j)
		{
			CX2DungeonSubStage::Trigger *pTrigger = vecTrigger[j];
			pTrigger->m_bReacted = false;
			pTrigger->m_NowInterval = 0.f;
		}
	}

	pDungeonGame->Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ();

	UpdateReadyNpc();
}


void CEditDungeon::OnBnClickedButtonNowstage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	if( pDungeonGame != NULL )
	{
		CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
		if( pDungeon != NULL )
		{
			int iStage = pDungeon->GetNowStageIndex();
			int iSubStage = pDungeon->GetNowStage()->GetNowSubStageIndex();

			m_listStage.SetItemState(iStage, LVIS_SELECTED, LVIS_SELECTED );
			m_listStage.EnsureVisible(iStage, TRUE);

			m_listSubStage.SetItemState(iSubStage, LVIS_SELECTED, LVIS_SELECTED );
			m_listSubStage.EnsureVisible(iSubStage, TRUE);

			m_iSelectedStage = iStage;
			m_iSelectedReadyNpc = -1;
			m_iSelectedSubStage = iSubStage;
			m_iSelectedNpc = -1;
			m_iSelectedTrigger = -1;
		}
	}
}

void CEditDungeon::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CEditDungeon::OnPropertyChanged( WPARAM wParam, LPARAM lParam )
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lParam;
	if( !pProp ) 
		return 0;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	PROPERTY_ID eID = (PROPERTY_ID)pProp->GetData();
	switch( eID )
	{		
		case DUNGEON_WORLD_COLOR_R:
			{
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pDungeon->GetWorldColor();
				vColor.r = pPropSlider->GetValue().fltVal;
				pDungeon->SetWorldColor( vColor );
			}
			break;
		case DUNGEON_WORLD_COLOR_G:
			{
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pDungeon->GetWorldColor();
				vColor.g = pPropSlider->GetValue().fltVal;
				pDungeon->SetWorldColor( vColor );
			}
			break;
		case DUNGEON_WORLD_COLOR_B:
			{
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pDungeon->GetWorldColor();
				vColor.b = pPropSlider->GetValue().fltVal;
				pDungeon->SetWorldColor( vColor );
			}
			break;
		case DUNGEON_UNIT_COLOR_R:
			{
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pDungeon->GetUnitColor();
				vColor.r = pPropSlider->GetValue().fltVal;
				pDungeon->SetUnitColor( vColor );
			}
			break;
		case DUNGEON_UNIT_COLOR_G:
			{
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pDungeon->GetUnitColor();
				vColor.g = pPropSlider->GetValue().fltVal;
				pDungeon->SetUnitColor( vColor );
			}
			break;
		case DUNGEON_UNIT_COLOR_B:
			{
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pDungeon->GetUnitColor();
				vColor.b = pPropSlider->GetValue().fltVal;
				pDungeon->SetUnitColor( vColor );
			}
			break;
			
		case ED_WORLD_ID:
			{
				wstring wstrWorldId = (WCHAR*)pProp->GetValue().bstrVal;
				wstring wstrDataName = L"";
				bool bFindWorldName = false;

				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				const vector<CX2World::WorldData*> &vecWorldData = g_pData->GetWorldManager()->GetWorldDataList();
				for(UINT i=0; i<vecWorldData.size(); ++i)
				{
					if( wstrWorldId.compare( vecWorldData[i]->worldName ) == 0 )
					{
						pStageData->m_WorldID = vecWorldData[i]->worldID;

						wstrDataName = vecWorldData[i]->dataFileName;
						SetPropValue( ED_WORLD_NAME, (_variant_t)wstrDataName.c_str() );
						pStageData->m_wstrWorldDataName = wstrDataName;

						m_listStage.SetItemText(m_iSelectedStage, 1, vecWorldData[i]->worldName.c_str());

						bFindWorldName = true;
						break;
					}
				}
				if( bFindWorldName == false )
				{
					SetPropValue( ED_WORLD_NAME, (_variant_t)L"" );
					pStageData->m_wstrWorldDataName = L"";
				}

				if( pDungeon->GetNowStageIndex() == m_iSelectedStage )
				{
					CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
					pToolMain->LoadWorld(wstrDataName.c_str());
				}
			}
			break;
		case ED_WORLD_NAME:
			{
				wstring wstrWorldName = L"";
				wstring wstrFileName = (WCHAR*)pProp->GetValue().bstrVal;
				if( wstrFileName.empty() == false )
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					pStageData->m_wstrWorldDataName = wstrFileName;

					bool bFindID = false;
					const vector<CX2World::WorldData*> &vecWorldData = g_pData->GetWorldManager()->GetWorldDataList();
					for(UINT i=0; i<vecWorldData.size(); ++i)
					{
						if( wstrFileName.compare( vecWorldData[i]->dataFileName ) == 0 )
						{
							pStageData->m_WorldID = vecWorldData[i]->worldID;
							wstrWorldName = vecWorldData[i]->worldName;
							SetPropValue( ED_WORLD_ID, (_variant_t)wstrWorldName.c_str() );

							m_listStage.SetItemText(m_iSelectedStage, 1, wstrWorldName.c_str());

							bFindID = true;
							break;
						}
					}
					if( bFindID == false )
					{
						pStageData->m_WorldID = CX2World::WI_NONE;
						SetPropValue( ED_WORLD_ID, (_variant_t)L"WI_NONE" );
						m_listStage.SetItemText(m_iSelectedStage, 1, L"WI_NONE");
					}

					if( pDungeon->GetNowStageIndex() == m_iSelectedStage )
					{
						CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
						pToolMain->LoadWorld(wstrFileName.c_str());
					}					
				}
			}
			break;
		case ED_START_MOTION:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				pStageData->m_bStartMotion = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
			}
			break;
		case ED_SECRET_STAGE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				pStageData->m_bSecretStage = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
			}
			break;
		case ED_STAGE_TYPE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				wstring wstrStageTypeValue = (WCHAR*)pProp->GetValue().bstrVal;
				for(int i=0; i<ARRAY_SIZE(wstrStageType); ++i)
				{
					if( wstrStageTypeValue.compare(wstrStageType[i]) == 0 )
					{
						pStageData->m_eStageType = (CX2DungeonStage::STAGE_TYPE)i;
						break;
					}
				}
			}
			break;
		case ED_WORLD_COLOR_R:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pStageData->m_WorldColor;
				vColor.r = pPropSlider->GetValue().fltVal;
				pStageData->m_WorldColor = vColor;
			}
			break;
		case ED_WORLD_COLOR_G:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pStageData->m_WorldColor;
				vColor.g = pPropSlider->GetValue().fltVal;
				pStageData->m_WorldColor = vColor;
			}
			break;
		case ED_WORLD_COLOR_B:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pStageData->m_WorldColor;
				vColor.b = pPropSlider->GetValue().fltVal;
				pStageData->m_WorldColor = vColor;
			}
			break;
		case ED_UNIT_COLOR_R:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pStageData->m_UnitColor;
				vColor.r = pPropSlider->GetValue().fltVal;
				pStageData->m_WorldColor = vColor;
			}
			break;
		case ED_UNIT_COLOR_G:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pStageData->m_UnitColor;
				vColor.g = pPropSlider->GetValue().fltVal;
				pStageData->m_WorldColor = vColor;
			}
			break;
		case ED_UNIT_COLOR_B:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
				pPropSlider->UpdateSlider();
				D3DXCOLOR vColor = pStageData->m_UnitColor;
				vColor.b = pPropSlider->GetValue().fltVal;
				pStageData->m_WorldColor = vColor;
			}
			break;

		case ED_MAIN_LINE_SET:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				pSubStageData->m_MainLineSet = pProp->GetValue().intVal;

				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d", pSubStageData->m_MainLineSet);
				m_listSubStage.SetItemText(m_iSelectedSubStage, 2, wbuf);
			}
			break;
		case ED_START_LINE_SET:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				pSubStageData->m_StartLineSet = pProp->GetValue().intVal;

				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d", pSubStageData->m_StartLineSet);
				m_listSubStage.SetItemText(m_iSelectedSubStage, 1, wbuf);
			}
			break;
		case ED_END_LINE_SET:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				pSubStageData->m_EndLineSet = pProp->GetValue().intVal;

				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d", pSubStageData->m_EndLineSet);
				m_listSubStage.SetItemText(m_iSelectedSubStage, 3, wbuf);
			}
			break;
		case ED_GO_TYPE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				wstring wstrGoTypeValue = (WCHAR*)pProp->GetValue().bstrVal;
				for(int i=0; i<ARRAY_SIZE(wstrGoType); ++i)
				{
					if( wstrGoTypeValue.compare(wstrGoType[i]) == 0 )
					{
						pSubStageData->m_GoType = (CX2DungeonSubStage::GO_TYPE)i;
						break;
					}
				}
			}
			break;
		case ED_USE_POS_OF_MONSTER_FOR_REBIRTH:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				pSubStageData->m_bUsePosOfMonsterForRebirth = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
			}
			break;
		case ED_KEY_CODE_OF_MONSTER_FOR_REBIRTH:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				pSubStageData->m_iKeyCodeOfMonsterForRebirth = pProp->GetValue().intVal;
			}
			break;

		case ED_LINE_INDEX:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::CurtainData *pCurtainData = pSubStageData->m_CurtainDataList[m_iSelectedCurtainData];

				pCurtainData->m_LineIndex = pProp->GetValue().intVal;

				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d", pCurtainData->m_LineIndex);
				m_listCurtainGroup.SetItemText(m_iSelectedCurtainData, 0, wbuf);
			}
			break;
		case ED_AT_START:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::CurtainData *pCurtainData = pSubStageData->m_CurtainDataList[m_iSelectedCurtainData];

				pCurtainData->m_bAtStart = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;

				if( pCurtainData->m_bAtStart == true )
					m_listCurtainGroup.SetItemText(m_iSelectedCurtainData, 1, L"TRUE");
				else
					m_listCurtainGroup.SetItemText(m_iSelectedCurtainData, 1, L"FALSE");
			}
			break;
		case ED_LOOK_LEFT:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::CurtainData *pCurtainData = pSubStageData->m_CurtainDataList[m_iSelectedCurtainData];

				pCurtainData->m_bLookLeft = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;

				if( pCurtainData->m_bLookLeft == true )
					m_listCurtainGroup.SetItemText(m_iSelectedCurtainData, 2, L"TRUE");
				else
					m_listCurtainGroup.SetItemText(m_iSelectedCurtainData, 2, L"FALSE");
			}
			break;
					
		case ED_CLEAR_CONDITION_TYPE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];

				wstring wstrClearCondTypeValue = (WCHAR*)pProp->GetValue().bstrVal;
				for(int i=0; i<ARRAY_SIZE(wstrStageClearType); ++i)
				{
					if( wstrClearCondTypeValue.compare(wstrStageClearType[i]) == 0 )
					{
						clearCond.m_eClearCondition = (CX2DungeonSubStage::CLEAR_CONDITION)i;
						break;
					}
				}

				switch(clearCond.m_eClearCondition)
				{
				case CX2DungeonSubStage::CC_KILL_ALL_ACTIVE_NPC:
					{
						GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(FALSE);
					}
					break;
				case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
				case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:
					{
						GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
						GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
						GetProp(ED_CC_NPC_KEYCODE)->Show(TRUE);
						GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
						GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);
						GetProp(ED_CC_NPC_ID)->Show(FALSE);
					}
					break;
				case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
					{
						GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
						GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
						GetProp(ED_CC_NPC_KEYCODE)->Show(FALSE);
						GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
						GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);
						GetProp(ED_CC_NPC_ID)->Show(TRUE);
					}
					break;
				case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_COUNT:
					{
						GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
						GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
						GetProp(ED_CC_NPC_KEYCODE)->Show(TRUE);
						GetProp(ED_CC_NPC_COUNT)->Show(TRUE);
						GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);
						GetProp(ED_CC_NPC_ID)->Show(FALSE);
					}
					break;
				case CX2DungeonSubStage::CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC:
					{
						GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
						GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(TRUE);
						GetProp(ED_CC_NPC_KEYCODE)->Show(FALSE);
						GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
						GetProp(ED_CC_CLEAR_TIME)->Show(FALSE);
						GetProp(ED_CC_NPC_ID)->Show(FALSE);
					}
					break;
				case CX2DungeonSubStage::CC_TIME:
					{
						GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(TRUE);
						GetProp(ED_CC_PORTAL_LINE_INDEX)->Show(FALSE);
						GetProp(ED_CC_NPC_KEYCODE)->Show(FALSE);
						GetProp(ED_CC_NPC_COUNT)->Show(FALSE);
						GetProp(ED_CC_CLEAR_TIME)->Show(TRUE);
						GetProp(ED_CC_NPC_ID)->Show(FALSE);
					}
					break;
				default:
					GetProp(ED_CLEAR_CONDITION_DATA_GROUP)->Show(FALSE);
					break;
				}

				m_listClearCondition.SetItemText(m_iSelectedClearCondition, 0, wstrClearCondTypeValue.c_str());
				UpdateClearConditionData();
			}
			break;			
		case ED_CLEAR_CONDITION_NEXT_STAGE_CLEAR_TYPE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				CX2DungeonSubStage::NextStageData &nextStageData = clearCond.m_vecNextStage[m_iSelectedNextStage];

				wstring wstrClearTypeValue = (WCHAR*)pProp->GetValue().bstrVal;
				for(int i=0; i<ARRAY_SIZE(wstrClearType); ++i)
				{
					if( wstrClearTypeValue.compare(wstrClearType[i]) == 0 )
					{
						nextStageData.m_eClearType = (CX2DungeonSubStage::CLEAR_TYPE)i;
						break;
					}
				}

				m_listNextStage.SetItemText(m_iSelectedNextStage, 0, wstrClearTypeValue.c_str());
			}
			break;
		case ED_CLEAR_CONDITION_NEXT_STAGE_STAGE_INDEX:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				CX2DungeonSubStage::NextStageData &nextStageData = clearCond.m_vecNextStage[m_iSelectedNextStage];

				nextStageData.m_iStageIndex = pProp->GetValue().intVal;
				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d", nextStageData.m_iStageIndex);
				m_listNextStage.SetItemText(m_iSelectedNextStage, 1, wbuf);
			}
			break;
		case ED_CLEAR_CONDITION_NEXT_STAGE_SUBSTAGE_INDEX:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				CX2DungeonSubStage::NextStageData &nextStageData = clearCond.m_vecNextStage[m_iSelectedNextStage];

				nextStageData.m_iSubStageIndex = pProp->GetValue().intVal;
				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d", nextStageData.m_iSubStageIndex);
				m_listNextStage.SetItemText(m_iSelectedNextStage, 2, wbuf);
			}
			break;
		case ED_CLEAR_CONDITION_NEXT_STAGE_RATE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				CX2DungeonSubStage::NextStageData &nextStageData = clearCond.m_vecNextStage[m_iSelectedNextStage];

				nextStageData.m_iRate = pProp->GetValue().intVal;
			}
			break;
		case ED_CLEAR_CONDITION_NEXT_STAGE_ENDINGSPEECHINDEX:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				CX2DungeonSubStage::NextStageData &nextStageData = clearCond.m_vecNextStage[m_iSelectedNextStage];

				nextStageData.m_iDungeonEndingSpeechTableIndex = pProp->GetValue().intVal;
			}
			break;
				
		case ED_CC_PORTAL_LINE_INDEX:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				int iPortalLineValue = pProp->GetValue().intVal;
				if( m_iSelectedClearCondData >= (int)clearCond.m_vecPortalLineIndex.size() )
				{					
					clearCond.m_vecPortalLineIndex.push_back( iPortalLineValue );
					UpdateClearConditionData();
				}
				else
				{
					int &iPortalLine = clearCond.m_vecPortalLineIndex[m_iSelectedClearCondData];
					iPortalLine = iPortalLineValue;
				}
				

				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d", iPortalLineValue);
				m_listClearCondData.SetItemText(m_iSelectedClearCondData, 0, wbuf);
			}
			break;
		case ED_CC_NPC_KEYCODE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				int iKey = pProp->GetValue().intVal;
				if( m_iSelectedClearCondData >= (int)clearCond.m_vecNPCKeyCode.size() )
				{
					CX2DungeonSubStage::ClearCondtionData::MonsterCount monsterCount;
					monsterCount.m_iKey = iKey;
					clearCond.m_vecNPCKeyCode.push_back(monsterCount);
					UpdateClearConditionData();
				}
				else
				{
					CX2DungeonSubStage::ClearCondtionData::MonsterCount &monsterCount = clearCond.m_vecNPCKeyCode[m_iSelectedClearCondData];
					monsterCount.m_iKey = iKey;
				}

				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d",iKey);
				m_listClearCondData.SetItemText(m_iSelectedClearCondData, 0, wbuf);
			}
			break;
		case ED_CC_NPC_COUNT:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				int iCount = pProp->GetValue().intVal;
				if( m_iSelectedClearCondData >= (int)clearCond.m_vecNPCKeyCode.size() )
				{
					CX2DungeonSubStage::ClearCondtionData::MonsterCount monsterCount;
					monsterCount.m_iObjectiveCount = iCount;
					clearCond.m_vecNPCKeyCode.push_back(monsterCount);
					UpdateClearConditionData();
				}
				else
				{
					CX2DungeonSubStage::ClearCondtionData::MonsterCount &monsterCount = clearCond.m_vecNPCKeyCode[m_iSelectedClearCondData];
					monsterCount.m_iObjectiveCount = iCount;

				}
				
				WCHAR wbuf[10];
				wsprintfW(wbuf, L"%d", iCount);
				m_listClearCondData.SetItemText(m_iSelectedClearCondData, 1, wbuf);
			}
			break;
		case ED_CC_CLEAR_TIME:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
				clearCond.m_fClearTime = pProp->GetValue().fltVal;
				
				char szBuf[10];
				sprintf(szBuf, "%5.2f", clearCond.m_fClearTime);
				wstring wstrClearTime;
				ConvertCharToWCHAR(wstrClearTime, szBuf);
				m_listClearCondData.SetItemText(0, 0, wstrClearTime.c_str());
			}
			break;
		case ED_CC_NPC_ID:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
				CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];

				wstring wstrNpcName = (WCHAR*)pProp->GetValue().bstrVal;
				CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;

				if( m_iSelectedClearCondData >= (int)clearCond.m_vecNPCID.size() )
				{
					CX2DungeonSubStage::ClearCondtionData::MonsterCount monsterCount;
					monsterCount.m_iKey = (int)pToolMain->m_enumManager.GetNpcEnum(wstrNpcName);
					clearCond.m_vecNPCID.push_back(monsterCount);
					UpdateClearConditionData();
				}
				else
				{
					CX2DungeonSubStage::ClearCondtionData::MonsterCount &monsterCount = clearCond.m_vecNPCID[m_iSelectedClearCondData];
					monsterCount.m_iKey = (int)pToolMain->m_enumManager.GetNpcEnum(wstrNpcName);
				}

				m_listClearCondData.SetItemText(0, 0, wstrNpcName.c_str());
			}
			break;

		case ED_NPC_UNITID:
			{	
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

				if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
				{
					CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
					if( pNpcData != NULL )
					{
						wstring wstrNpcName = (WCHAR*)pProp->GetValue().bstrVal;
						CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;

						CX2UnitManager::NPC_UNIT_ID unitId = pToolMain->m_enumManager.GetNpcEnum(wstrNpcName);
						pNpcData->m_UnitID = unitId;

						ReadyNpcData();
						UpdateNpc();
					}	
				}
			}
			break;
		case ED_NPC_KEYCODE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

				if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
				{
					CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
					if( pNpcData != NULL )
					{					
						pNpcData->m_KeyCode = (int)pProp->GetValue().intVal;
						UpdateNpc();
					}	
				}
			}
			break;
		case ED_NPC_FOCUSCAMERA:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

				if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
				{
					CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
					if( pNpcData != NULL )
					{
						pNpcData->m_bFocusCamera = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
					}
				}
			}
			break;
		case ED_NPC_STARTPOS:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

				if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
				{
					CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
					if( pNpcData != NULL )
					{
						wstring wstrStartPos = (WCHAR*)pProp->GetValue().bstrVal;
						string strStartPos = "";
						ConvertWCHARToChar(strStartPos, wstrStartPos.c_str());
						char temp[100];
						const char *pOriStartPos = strStartPos.c_str();
						const char *pStartPos = strStartPos.c_str();
						int iStartPos = -1;
						pNpcData->m_vecStartPos.clear();
						while( sscanf(pStartPos, "%d", &iStartPos) > 0 )
						{	
							sprintf(temp, "%d", iStartPos);
							pNpcData->m_vecStartPos.push_back(iStartPos);

							pStartPos = pStartPos + (strlen(temp) + 1);

							if( pStartPos > &pOriStartPos[strlen(pOriStartPos)-1] )
								break;
						}
						if( pNpcData->m_vecStartPos.size() == 0 )
							pNpcData->m_vecStartPos.push_back(-1);

						UpdateNpc();
					}
				}
			}
			break;
		case ED_NPC_ACTIVE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

				if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
				{
					CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
					if( pNpcData != NULL )
					{
						pNpcData->m_bActive = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						UpdateNpc();
					}
				}
			}
			break;
		case ED_NPC_SHOWGAGE:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

				if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
				{
					CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
					if( pNpcData != NULL )
					{
						pNpcData->m_bShowGage = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
					}
				}
			}
			break;
		case ED_NPC_ADDPOS_X:
			{
				CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
				CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

				if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
				{
					CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
					if( pNpcData != NULL )
					{
						pNpcData->m_AddPos.x = (float)pProp->GetValue().fltVal;
					}
				}
			}
			break;
			case ED_NPC_ADDPOS_Y:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_AddPos.y = (float)pProp->GetValue().fltVal;
						}
					}
				}
				break;
			case ED_NPC_ADDPOS_Z:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_AddPos.z = (float)pProp->GetValue().fltVal;
						}
					}
				}
				break;
			case ED_NPC_LEVEL:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{					
							pNpcData->m_iLevel4Tool = (int)pProp->GetValue().intVal;
							if( pNpcData->m_iLevel4Tool <= 0 )
							{						
								if( pDungeon->GetDungeonData()->m_iNpcLevel > 0 )
									pNpcData->m_Level = pDungeon->GetDungeonData()->m_iNpcLevel;
								else
									pNpcData->m_Level = pDungeon->GetDungeonData()->m_MinLevel;
							}
							else
							{
								pNpcData->m_Level = pNpcData->m_iLevel4Tool;
							}
						}	
					}
				}
				break;
			case ED_NPC_RATE:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{					
							pNpcData->m_Rate = (int)pProp->GetValue().intVal;
							//UpdateNpc();
						}	
					}
				}
				break;
			case ED_NPC_HASBOSSGAGE:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_bHasBossGage = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_NPC_SHOWBOSSNAME:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_bShowBossName = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_NPC_SHOWSUBBOSSNAME:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_bShowSubBossName = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_NPC_SIEGEMODE:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_bSiegeMode = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_NPC_ISRIGHT:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_bIsRight = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_NPC_UNITSCALE:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_fUnitScale = (float)pProp->GetValue().fltVal;
						}
					}
				}
				break;
			case ED_NPC_BOSSGAUAGEHPLINES:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{					
							pNpcData->m_usBossGaugeHPLines = (u_short)pProp->GetValue().intVal;
							//UpdateNpc();
						}	
					}
				}
				break;
			case ED_NPC_ALLYNPC:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_bAllyNpc = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_NPC_NODROP:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_bNoDrop = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_NPC_MONSTER_GRADE:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							wstring wstrMonsterGradeValue = (WCHAR*)pProp->GetValue().bstrVal;
							for( UINT i=0; i<ARRAY_SIZE(wstrMonsterGrade); ++i )
							{
								if( wstrMonsterGradeValue.compare(wstrMonsterGrade[i]) == 0 )
								{
									pNpcData->m_MonsterGrade = (CX2DungeonSubStage::MONSTER_GRADE)i;
									break;
								}
							}
						}
					}
				}
				break;
			case ED_NPC_SUBNPC:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{
							pNpcData->m_bSubNpc = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
							UpdateNpc();
						}
					}
				}
				break;
			case ED_NPC_SUBNPCINDEX:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{					
							pNpcData->m_iSubNPCIndex = (int)pProp->GetValue().intVal;
							UpdateNpc();
						}	
					}
				}
				break;
			case ED_NPC_SUBNPCRATE:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedNpc >= 0 && m_iSelectedNpc < (int)pSubStageData->m_vecSubStageNpcData.size() )
					{
						CX2DungeonSubStage::NPCData* pNpcData = pSubStageData->m_vecSubStageNpcData[m_iSelectedNpc];
						if( pNpcData != NULL )
						{					
							pNpcData->m_iSubNPCRate = (int)pProp->GetValue().intVal;
							//UpdateNpc();
						}	
					}
				}
				break;

			case ED_TRIGGER_HOST:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedTrigger >= 0 && m_iSelectedTrigger < (int)pSubStageData->m_TriggerList.size() )
					{
						CX2DungeonSubStage::Trigger *pTrigger = pSubStageData->m_TriggerList[m_iSelectedTrigger];
						if( pTrigger != NULL )
						{
							pTrigger->m_bHost = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_TRIGGER_ONTIME:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedTrigger >= 0 && m_iSelectedTrigger < (int)pSubStageData->m_TriggerList.size() )
					{
						CX2DungeonSubStage::Trigger *pTrigger = pSubStageData->m_TriggerList[m_iSelectedTrigger];
						if( pTrigger != NULL )
						{
							pTrigger->m_bOneTime = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
						}
					}
				}
				break;
			case ED_TRIGGER_INTERVAL:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedTrigger >= 0 && m_iSelectedTrigger < (int)pSubStageData->m_TriggerList.size() )
					{
						CX2DungeonSubStage::Trigger *pTrigger = pSubStageData->m_TriggerList[m_iSelectedTrigger];
						if( pTrigger != NULL )
						{
							pTrigger->m_Interval = (float)pProp->GetValue().fltVal;
						}
					}
				}
				break;
			case ED_TRIGGER_CONDITION_FUNC:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedTrigger >= 0 && m_iSelectedTrigger < (int)pSubStageData->m_TriggerList.size() )
					{
						CX2DungeonSubStage::Trigger *pTrigger = pSubStageData->m_TriggerList[m_iSelectedTrigger];
						if( pTrigger != NULL )
						{
							pTrigger->m_ConditionFunc = (WCHAR*)pProp->GetValue().bstrVal;
						}
					}
				}
				break;
			case ED_TRIGGER_REACT_FUNC:
				{
					CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
					CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

					if( m_iSelectedTrigger >= 0 && m_iSelectedTrigger < (int)pSubStageData->m_TriggerList.size() )
					{
						CX2DungeonSubStage::Trigger *pTrigger = pSubStageData->m_TriggerList[m_iSelectedTrigger];
						if( pTrigger != NULL )
						{
							pTrigger->m_ReactFunc = (WCHAR*)pProp->GetValue().bstrVal;
						}
					}
				}
				break;
		default:
			break;
	}

	return 0;
}

void CEditDungeon::OnLvnItemchangedListSubStage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 || pNMLV->uNewState == 2 )
	{
		m_iSelectedSubStage = pNMLV->iItem;

		m_iSelectedCurtainData = 0;
		m_iSelectedClearCondition = 0;
		m_iSelectedNextStage = 0;
		m_iSelectedClearCondData = 0;
		
		InitClearCondData();
		UpdateProp(1);
		UpdateSubStageProp();

		UpdateCurtainGroup();
		UpdateClearCondition();
		UpdateClearConditionNextStage();
		UpdateClearConditionData();
		UpdateNpc();
		UpdateTrigger();

		
	}
}


void CEditDungeon::OnLvnItemchangedListCurtainGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 || pNMLV->uNewState == 2 )
	{
		m_iSelectedCurtainData = pNMLV->iItem;

		UpdateProp(2);
		UpdateCurtainProp(m_iSelectedCurtainData);
	}
}


void CEditDungeon::OnLvnItemchangedListClearCondition(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 || pNMLV->uNewState == 2 )
	{
		m_iSelectedClearCondition = pNMLV->iItem;
		InitClearCondData();

		m_iSelectedNextStage = 0;
		m_iSelectedClearCondData = 0;

		UpdateProp(3);
		UpdateClearConditionProp(m_iSelectedClearCondition);
		UpdateClearConditionNextStage();
		UpdateClearConditionData();
	}	
}


void CEditDungeon::OnLvnItemchangedListClearCondition2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 || pNMLV->uNewState == 2 )
	{
		m_iSelectedNextStage = pNMLV->iItem;

		UpdateProp(4);
		UpdateClearConditionNextStageProp(m_iSelectedNextStage);
	}
}


void CEditDungeon::OnLvnItemchangedListClearCondition3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 || pNMLV->uNewState == 2 )
	{
		m_iSelectedClearCondData = pNMLV->iItem;

		UpdateProp(3);
		UpdateClearCondDataProp(m_iSelectedClearCondData);
	}
}


void CEditDungeon::OnBnClickedMfcbutton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 스테이지 추가
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	CX2DungeonStage::StageData *pStageData = new CX2DungeonStage::StageData();
	pStageData->m_bSecretStage = false;
	pStageData->m_bStartMotion = false;
	pStageData->m_WorldID = CX2World::WI_WORLDTOOL_DEFAULT;
	pStageData->m_eStageType = CX2DungeonStage::ST_NONE;
	pStageData->m_MissionTitle = L"";
	pStageData->m_WorldColor = D3DXCOLOR(-1.f, -1.f, -1.f, -1.f);
	pStageData->m_UnitColor = D3DXCOLOR(-1.f, -1.f, -1.f, -1.f);

	CX2DungeonSubStage::SubStageData *pSubStageData = new CX2DungeonSubStage::SubStageData();
	pStageData->m_SubStageDataList.push_back(pSubStageData);
	vecStageData.push_back(pStageData);

	InitClearCondData();
		
	m_iSelectedCurtainData = 0;
	m_iSelectedClearCondition = 0;
	m_iSelectedNextStage = 0;
	m_iSelectedClearCondData = 0;

	m_iSelectedStage = vecStageData.size() - 1;
	m_iSelectedReadyNpc = -1;
	m_iSelectedSubStage = 0;
	
	UpdateStage();

	pDungeonGame->ChangeStage( m_iSelectedStage );
}


void CEditDungeon::OnBnClickedMfcbutton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 스테이지 삭제	
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();

	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;
	
	if( pDungeon->GetNowStageIndex() == m_iSelectedStage )
	{
		AfxMessageBox(L"현재 진행중인 스테이지는 삭제할 수 없습니다.");
		return;
	}

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	SAFE_DELETE(pStageData);
	vecStageData.erase( vecStageData.begin() + m_iSelectedStage );
	UpdateStage();

	OnBnClickedButtonNowstage();
}


void CEditDungeon::OnBnClickedMfcbutton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Stage Move Up
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();

	if( m_iSelectedStage <= 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;
	
	CX2DungeonStage::StageData* pStageDataTemp = vecStageData[m_iSelectedStage];
	vecStageData[m_iSelectedStage] = vecStageData[m_iSelectedStage-1];
	vecStageData[m_iSelectedStage-1] = pStageDataTemp;

	m_iSelectedStage -= 1;
	UpdateStage();
}


void CEditDungeon::OnBnClickedMfcbutton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Stage Move Down
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();

	if( m_iSelectedStage < 0 || m_iSelectedStage + 1>= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageDataTemp = vecStageData[m_iSelectedStage];
	vecStageData[m_iSelectedStage] = vecStageData[m_iSelectedStage+1];
	vecStageData[m_iSelectedStage+1] = pStageDataTemp;

	m_iSelectedStage += 1;
	UpdateStage();
}


void CEditDungeon::OnBnClickedMfcbutton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 서브스테이지 추가
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData *pStageData = vecStageData[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = new CX2DungeonSubStage::SubStageData();
	pStageData->m_SubStageDataList.push_back(pSubStageData);

	InitClearCondData();	
	m_iSelectedCurtainData = 0;
	m_iSelectedClearCondition = 0;
	m_iSelectedNextStage = 0;
	m_iSelectedClearCondData = 0;
	
	m_iSelectedSubStage = pStageData->m_SubStageDataList.size() - 1;
	m_iSelectedNpc = -1;
	m_iSelectedTrigger = -1;
	UpdateSubStage();
	UpdateNpc();
}


void CEditDungeon::OnBnClickedMfcbutton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 서브스테이지 삭제

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData *pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	if( m_iSelectedSubStage == pDungeon->GetNowStage()->GetNowSubStageIndex() )
	{
		AfxMessageBox(L"현재 진생중인 서브스테이지는 삭제할 수 없습니다.");
		return;
	}

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	SAFE_DELETE(pSubStageData);
	pStageData->m_SubStageDataList.erase(pStageData->m_SubStageDataList.begin() + m_iSelectedSubStage);

	UpdateSubStage();
	m_iSelectedNpc = -1;
	m_iSelectedTrigger = -1;
	
	OnBnClickedButtonNowstage();
}


void CEditDungeon::OnBnClickedMfcbutton9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// SubStage Move Up
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage <= 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageDataTemp = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	pStageData->m_SubStageDataList[m_iSelectedSubStage] = pStageData->m_SubStageDataList[m_iSelectedSubStage-1];
	pStageData->m_SubStageDataList[m_iSelectedSubStage-1] = pSubStageDataTemp;

	m_iSelectedSubStage -= 1;
	UpdateSubStage();
}


void CEditDungeon::OnBnClickedMfcbutton10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// SubStage Move Down
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage + 1 >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageDataTemp = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	pStageData->m_SubStageDataList[m_iSelectedSubStage] = pStageData->m_SubStageDataList[m_iSelectedSubStage+1];
	pStageData->m_SubStageDataList[m_iSelectedSubStage+1] = pSubStageDataTemp;

	m_iSelectedSubStage += 1;
	UpdateSubStage();
}


void CEditDungeon::OnBnClickedMfcbutton19()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// curtain 추가

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	CX2DungeonSubStage::CurtainData *pCurtainData = new CX2DungeonSubStage::CurtainData();
	pCurtainData->m_LineIndex = -1;
	pCurtainData->m_bAtStart = false;
	pCurtainData->m_bLookLeft = true;

	pSubStageData->m_CurtainDataList.push_back(pCurtainData);
	m_iSelectedCurtainData = pSubStageData->m_CurtainDataList.size() - 1;
	UpdateCurtainGroup();
}


void CEditDungeon::OnBnClickedMfcbutton20()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// curtain 삭제
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	if( m_iSelectedCurtainData < 0 || m_iSelectedCurtainData >= (int)pSubStageData->m_CurtainDataList.size() )
		return;

	CX2DungeonSubStage::CurtainData *pCurtainData = pSubStageData->m_CurtainDataList[m_iSelectedCurtainData];
	SAFE_DELETE( pCurtainData );
	pSubStageData->m_CurtainDataList.erase(pSubStageData->m_CurtainDataList.begin() + m_iSelectedCurtainData);

	m_iSelectedCurtainData = 0;
	UpdateCurtainGroup();
}


void CEditDungeon::OnBnClickedMfcbutton13()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Clear Condition 추가
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	CX2DungeonSubStage::ClearCondtionData clearCondition;
	clearCondition.m_eClearCondition = CX2DungeonSubStage::CC_KILL_ALL_ACTIVE_NPC;

	pSubStageData->m_vecClearCondData.push_back(clearCondition );
	m_iSelectedClearCondition = pSubStageData->m_vecClearCondData.size() - 1;
	UpdateClearCondition();
}


void CEditDungeon::OnBnClickedMfcbutton14()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Clear Condition 삭제
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	if( m_iSelectedClearCondition < 0 || m_iSelectedClearCondition >= (int)pSubStageData->m_vecClearCondData.size() )
		return;

	pSubStageData->m_vecClearCondData.erase(pSubStageData->m_vecClearCondData.begin() + m_iSelectedClearCondition);
	m_iSelectedClearCondition = 0;
	UpdateClearCondition();
	UpdateClearConditionNextStage();
	UpdateClearConditionData();
}


void CEditDungeon::OnBnClickedMfcbutton15()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// next stage 추가
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	if( m_iSelectedClearCondition < 0 || m_iSelectedClearCondition >= (int)pSubStageData->m_vecClearCondData.size() )
		return;

	CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
	CX2DungeonSubStage::NextStageData nextStage;
	nextStage.m_eClearType = CX2DungeonSubStage::CT_GAME;
	nextStage.m_iStageIndex = -1;
	nextStage.m_iSubStageIndex = -1;
	nextStage.m_iRate = 100;
	nextStage.m_iDungeonEndingSpeechTableIndex = 0;
	
	clearCond.m_vecNextStage.push_back(nextStage );
	m_iSelectedNextStage = clearCond.m_vecNextStage.size() - 1;
	UpdateClearConditionNextStage();
}


void CEditDungeon::OnBnClickedMfcbutton16()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// next stage 삭제
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	if( m_iSelectedClearCondition < 0 || m_iSelectedClearCondition >= (int)pSubStageData->m_vecClearCondData.size() )
		return;

	CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
	if( m_iSelectedNextStage < 0 || m_iSelectedNextStage >= (int)clearCond.m_vecNextStage.size() )
		return;


	clearCond.m_vecNextStage.erase(clearCond.m_vecNextStage.begin() + m_iSelectedNextStage);
	m_iSelectedNextStage = 0;
	UpdateClearConditionNextStage();
}


void CEditDungeon::OnBnClickedMfcbutton17()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Clear Condition Data 추가
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	if( m_iSelectedClearCondition < 0 || m_iSelectedClearCondition >= (int)pSubStageData->m_vecClearCondData.size() )
		return;

	CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
	switch(clearCond.m_eClearCondition)
	{
	case CX2DungeonSubStage::CC_KILL_ALL_ACTIVE_NPC:
		{
			AfxMessageBox(L"CC_KILL_ALL_ACTIVE_NPC Type에서는 Data가 필요치 않습니다.");
			return;
		}
		break;
	case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
	case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:	
	case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_COUNT:
		{
			CX2DungeonSubStage::ClearCondtionData::MonsterCount monsterCount;
			clearCond.m_vecNPCKeyCode.push_back(monsterCount);
			m_iSelectedClearCondData = clearCond.m_vecNPCKeyCode.size() - 1;
		}
		break;
	case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
		{
			CX2DungeonSubStage::ClearCondtionData::MonsterCount monsterCount;
			clearCond.m_vecNPCID.push_back(monsterCount);
			m_iSelectedClearCondData = clearCond.m_vecNPCID.size() - 1;
		}
		break;
	case CX2DungeonSubStage::CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC:
		{
			clearCond.m_vecPortalLineIndex.push_back(-1);
			m_iSelectedClearCondData = clearCond.m_vecPortalLineIndex.size() - 1;
		}
		break;
	case CX2DungeonSubStage::CC_TIME:
		{
			clearCond.m_fClearTime = -1.f;
			m_iSelectedClearCondData = 0;
		}
		break;
	}

	UpdateClearConditionData();
}


void CEditDungeon::OnBnClickedMfcbutton18()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Clear Condition Data 삭제
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
		return;

	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	if( m_iSelectedClearCondition < 0 || m_iSelectedClearCondition >= (int)pSubStageData->m_vecClearCondData.size() )
		return;

	CX2DungeonSubStage::ClearCondtionData &clearCond = pSubStageData->m_vecClearCondData[m_iSelectedClearCondition];
	switch(clearCond.m_eClearCondition)
	{
	case CX2DungeonSubStage::CC_KILL_ALL_ACTIVE_NPC:
	case CX2DungeonSubStage::CC_TIME:
		{
			AfxMessageBox(L"CC_KILL_ALL_ACTIVE_NPC Type에서는 삭제할 Data가 없습니다.");
			return;
		}
		break;
	case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
	case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:	
	case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_COUNT:
		{
			if( m_iSelectedClearCondData < 0 || m_iSelectedClearCondData >= (int)clearCond.m_vecNPCKeyCode.size() )
				return;

			clearCond.m_vecNPCKeyCode.erase( clearCond.m_vecNPCKeyCode.begin() + m_iSelectedClearCondData );
		}
		break;
	case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
		{
			if( m_iSelectedClearCondData < 0 || m_iSelectedClearCondData >= (int)clearCond.m_vecNPCID.size() )
				return;

			clearCond.m_vecNPCID.erase( clearCond.m_vecNPCID.begin() + m_iSelectedClearCondData );
		}
		break;
	case CX2DungeonSubStage::CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC:
		{
			if( m_iSelectedClearCondData < 0 || m_iSelectedClearCondData >= (int)clearCond.m_vecPortalLineIndex.size() )
				return;

			clearCond.m_vecPortalLineIndex.erase( clearCond.m_vecPortalLineIndex.begin() + m_iSelectedClearCondData );
		}
		break;
	default:
		break;
	}

	m_iSelectedClearCondData = 0;
	UpdateClearConditionData();
}


void CEditDungeon::OnBnClickedCheckNpcStartPosition()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// ready 몬스터 스타트 포지션 배치 여부
	UpdateData(TRUE);
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

	int iState = m_checkNpc.GetCheck();
	switch(iState)
	{
	case BST_UNCHECKED:
		pMain->SetCheckNpc(false);
		break;
	case BST_CHECKED:
		pMain->SetCheckNpc(true);
		break;
	case BST_INDETERMINATE:
		pMain->SetCheckNpc(false);
		break;
	}
}

CX2UnitManager::NPC_UNIT_ID CEditDungeon::GetSelectedReadyNpc()
{
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)m_vecStageData.size() )
		return CX2UnitManager::NUI_NONE;

	CX2DungeonStage::StageData* pStageData = m_vecStageData[m_iSelectedStage];
	vector<CX2UnitManager::NPC_UNIT_ID> vecReadyNpc = pStageData->m_ReadyNPCList;
	
	if( m_iSelectedReadyNpc < 0 || m_iSelectedReadyNpc >= (int)vecReadyNpc.size() )
		return CX2UnitManager::NUI_NONE;

	return vecReadyNpc[m_iSelectedReadyNpc];
}

vector<CEditDungeon::READY_NPC> CEditDungeon::GetNpcInfoByStartPos(int iStartPos)
{
	vector<READY_NPC> vecReadyNpc;

	int iCount = -1;
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
		
	CX2DungeonStage::StageData* pStageData =  pDungeon->GetNowStage()->GetStageData();
	vector<CX2DungeonSubStage::SubStageData*> vecSubStageData = pStageData->m_SubStageDataList;
	for(UINT j=0; j<vecSubStageData.size(); ++j)
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = vecSubStageData[j];
		vector<CX2DungeonSubStage::NPCData*> vecNpcData = pSubStageData->m_vecSubStageNpcData;
		for(UINT k=0; k<vecNpcData.size(); ++k)
		{
			++iCount;
			CX2DungeonSubStage::NPCData* pNpcData = vecNpcData[k];
			for(UINT i = 0; i < pNpcData->m_vecStartPos.size(); ++i )
			{					
				if( pNpcData->m_vecStartPos[i] == iStartPos )
				{
					READY_NPC readyNpc;
					readyNpc.m_eUnitId = pNpcData->m_UnitID;
					readyNpc.m_iUnitIndex = iCount;
					readyNpc.m_vAddPos = pNpcData->m_AddPos;
					readyNpc.m_bIsRight = pNpcData->m_bIsRight;

					vecReadyNpc.push_back(readyNpc);
					break;
				}
			}
		}
	}

	return vecReadyNpc;
}

void CEditDungeon::ReadyNpcData()
{
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	if( pDungeon->GetNowStageIndex() != m_iSelectedStage )
		return;

	pToolMain->InitReadyDungeonNpc();

	CX2DungeonStage::StageData* pStageData = pDungeon->GetNowStage()->GetStageData();
	vector<CX2DungeonSubStage::SubStageData*> vecSubStageData = pStageData->m_SubStageDataList;
	for(UINT j=0; j<vecSubStageData.size(); ++j)
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = vecSubStageData[j];
		vector<CX2DungeonSubStage::NPCData*> vecNpcData = pSubStageData->m_vecSubStageNpcData;
		for(UINT k=0; k<vecNpcData.size(); ++k)
		{	
			CX2DungeonSubStage::NPCData* pNpcData = vecNpcData[k];
			pToolMain->AddReadyDungeonNpc(pNpcData->m_UnitID);
		}
	}
}

void CEditDungeon::AddNpcData(int iStartPos, int iLineIndex)
{	
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	if( pDungeon->GetNowStageIndex() != m_iSelectedStage )
	{
		return;
	}

	CX2DungeonStage::StageData* pStageData = pDungeon->GetNowStage()->GetStageData();
	if( m_iSelectedSubStage < 0 || m_iSelectedSubStage >= (int)pStageData->m_SubStageDataList.size() )
	{
		return;
	}

	CX2DungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];
	if( pSubStageData != NULL )
	{
		if( iLineIndex >= 0 )
		{
			CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
			const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(iLineIndex);

			if( pLineData != NULL && 
				pSubStageData->m_StartLineSet != pLineData->lineSet &&
				pSubStageData->m_MainLineSet != pLineData->lineSet &&
				pSubStageData->m_EndLineSet != pLineData->lineSet )
			{
				AfxMessageBox(L"지정된 위치는 현재 선택된 서브스테이지가 아니여서 몬스터를 배치할 수 없습니다.\n스타트포지션만 추가됩니다.");
				return;
			}
		}

		CX2DungeonSubStage::NPCData* pNpcData = new CX2DungeonSubStage::NPCData( GetSelectedReadyNpc() );
		pNpcData->m_UID = g_pX2Game->GetNpcUID4Tool();
		pNpcData->m_vecStartPos.push_back(iStartPos);
		if( pDungeon->GetDungeonData()->m_iNpcLevel > 0 )
		{
			pNpcData->m_iLevel4Tool = 0;
			pNpcData->m_Level = pDungeon->GetDungeonData()->m_iNpcLevel;
		}
		else
		{
			pNpcData->m_iLevel4Tool = 0;
			pNpcData->m_Level = pDungeon->GetDungeonData()->m_MinLevel;		
		}

		pSubStageData->m_vecSubStageNpcData.push_back(pNpcData);
		m_iSelectedNpc = pSubStageData->m_vecSubStageNpcData.size() - 1;
	}	

	ReadyNpcData();
	UpdateNpc();
}

void CEditDungeon::RefreshNpcData(int iKey)
{
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	CX2DungeonStage::StageData* pStageData =  pDungeon->GetNowStage()->GetStageData();
	vector<CX2DungeonSubStage::SubStageData*> vecSubStageData = pStageData->m_SubStageDataList;
	for(UINT j=0; j<vecSubStageData.size(); ++j)
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = vecSubStageData[j];
		vector<CX2DungeonSubStage::NPCData*> vecNpcData = pSubStageData->m_vecSubStageNpcData;
		for(UINT k=0; k<vecNpcData.size(); ++k)
		{
			CX2DungeonSubStage::NPCData* pNpcData = vecNpcData[k];
			for(UINT i = 0; i < pNpcData->m_vecStartPos.size(); ++i )
			{					
				if( pNpcData->m_vecStartPos[i] == iKey )
				{
					pNpcData->m_vecStartPos[i] = -1;
					break;
				}
			}
		}
	}

	ReadyNpcData();
	UpdateNpc();
}


void CEditDungeon::OnLvnItemchangedListReadyNpc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 || pNMLV->uNewState == 2 )
	{
		m_iSelectedReadyNpc = pNMLV->iItem;
	}
	
}


void CEditDungeon::OnLvnItemchangedListNpcGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();		

	if( (pNMLV->uNewState == 2 || pNMLV->uNewState == 3) && (pNMLV->uOldState == 0 || pNMLV->uOldState == 1) )
	{
		// 선택			
		m_iSelectedNpc = pNMLV->iItem;

		UpdateProp(5);
		UpdateNpcProp(pNMLV->iItem);
	}
	else if( pNMLV->uNewState == 0 && pNMLV->uOldState == 2 )
	{
		// 선택 해제
		UpdateProp(5);
		int iIndex = UpdateNpcProp();
	}	
}


void CEditDungeon::OnBnClickedMfcbutton11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// ready npc 추가
	m_DlgNpcList.ShowWindow(SW_SHOW);
}

void CEditDungeon::OnBnClickedMfcbutton12()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// ready npc 삭제

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	vector<CX2DungeonStage::StageData*> &vecStageData = pDungeon->GetStageDataList();
	if( m_iSelectedStage < 0 || m_iSelectedStage >= (int)vecStageData.size() )
		return;

	CX2DungeonStage::StageData* pStageData = vecStageData[m_iSelectedStage];
	pStageData->m_ReadyNPCList.erase( pStageData->m_ReadyNPCList.begin() + m_iSelectedReadyNpc );

	UpdateReadyNpc();
}


void CEditDungeon::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString csLv;
	m_editDungeonLevel.GetWindowText(csLv);
	int iLv = _ttoi(csLv.GetString());

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2Dungeon::DungeonData *pDungeonData = const_cast< CX2Dungeon::DungeonData* >(pDungeon->GetDungeonData());
	pDungeonData->m_iNpcLevel = iLv;

	vector<CX2DungeonStage::StageData*> vecStageData = pDungeon->GetStageDataList();
	for(UINT i=0; i<vecStageData.size(); ++i)
	{
		CX2DungeonStage::StageData* pStageData = vecStageData[i];
		vector<CX2DungeonSubStage::SubStageData*> vecSubStageData = pStageData->m_SubStageDataList;
		for(UINT j=0; j<vecSubStageData.size(); ++j)
		{
			CX2DungeonSubStage::SubStageData* pSubStageData = vecSubStageData[j];
			vector<CX2DungeonSubStage::NPCData*> &vecNpcData = pSubStageData->m_vecSubStageNpcData;
			for(UINT k=0; k<vecNpcData.size(); ++k)
			{
				CX2DungeonSubStage::NPCData* pNpcData = vecNpcData[k];
				if( pNpcData != NULL )
				{
					if( pNpcData->m_iLevel4Tool <= 0 )
					{						
						if( pDungeon->GetDungeonData()->m_iNpcLevel > 0 )
							pNpcData->m_Level = pDungeon->GetDungeonData()->m_iNpcLevel;
						else
							pNpcData->m_Level = pDungeon->GetDungeonData()->m_MinLevel;
					}
					else
					{
						pNpcData->m_Level = pNpcData->m_iLevel4Tool;
					}
				}
			}

			for( UINT k=0; k<pSubStageData->m_NPCDataList.size(); k++ )
			{
				CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[k];
				if( pNPCData != NULL )
				{
					if( pNPCData->m_iLevel4Tool <= 0 )
					{						
						if( pDungeon->GetDungeonData()->m_iNpcLevel > 0 )
							pNPCData->m_Level = pDungeon->GetDungeonData()->m_iNpcLevel;
						else
							pNPCData->m_Level = pDungeon->GetDungeonData()->m_MinLevel;
					}
					else
					{
						pNPCData->m_Level = pNPCData->m_iLevel4Tool;
					}
				}
			}
		}
	}
}

char* CEditDungeon::LoadLuaFunction(SEnum::DUNGEON_ID eDungeonID)
{		
	SAFE_DELETE_ARRAY( m_pFunctionData );

	const CX2Dungeon::DungeonData *pDungeonData = g_pData->GetDungeonManager()->GetDungeonData(eDungeonID);

	if( pDungeonData != NULL )
	{
		wstring wstrScriptName = pDungeonData->m_DataFileName;

		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( wstrScriptName );
		if( Info == NULL )
			return NULL;

		if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
			return NULL;
				

		m_pFunctionData = new char[Info->size+1];
		memset(m_pFunctionData, 0, sizeof(char) * (Info->size+1));
		memcpy(m_pFunctionData, Info->pRealData, sizeof(char)*Info->size);
				
		char *pStartFunction = strstr(m_pFunctionData, "function ");
		if( pStartFunction != NULL )
		{
			return pStartFunction;
		}
	}

	return NULL;
}





void CEditDungeon::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_DlgTriggerFunc.ShowWindow(SW_SHOW);
}


void CEditDungeon::OnLvnItemchangedListTrigger(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 || pNMLV->uNewState == 2 )
	{
		m_iSelectedTrigger = pNMLV->iItem;
		UpdateProp(6);
		UpdateTriggerProp(m_iSelectedTrigger);
	}
}


void CEditDungeon::OnBnClickedMfcbutton21()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// trigger 추가
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( m_iSelectedTrigger >= 0 && m_iSelectedTrigger < (int)pSubStageData->m_TriggerList.size() )
	{
		CX2DungeonSubStage::Trigger *pTrigger = new CX2DungeonSubStage::Trigger();
		pSubStageData->m_TriggerList.push_back(pTrigger);
		m_iSelectedTrigger = pSubStageData->m_TriggerList.size() - 1;

		UpdateTrigger();
		UpdateTriggerProp(m_iSelectedTrigger);
	}
}


void CEditDungeon::OnBnClickedMfcbutton22()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// trigger 삭제
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	CX2DungeonStage::StageData* pStageData = pDungeon->GetStageDataList()[m_iSelectedStage];
	CX2DungeonSubStage::SubStageData *pSubStageData = pStageData->m_SubStageDataList[m_iSelectedSubStage];

	if( m_iSelectedTrigger >= 0 && m_iSelectedTrigger < (int)pSubStageData->m_TriggerList.size() )
	{
		CX2DungeonSubStage::Trigger *pTrigger = pSubStageData->m_TriggerList[m_iSelectedTrigger];
		if( pTrigger != NULL )
		{
			SAFE_DELETE( pTrigger );
			pSubStageData->m_TriggerList.erase( pSubStageData->m_TriggerList.begin() + m_iSelectedTrigger );
			if( m_iSelectedTrigger >= (int)pSubStageData->m_TriggerList.size() )
				--m_iSelectedTrigger;

			UpdateTrigger();
			UpdateTriggerProp(m_iSelectedTrigger);
		}
	}
}

void CEditDungeon::SaveAs(wstring wstrFileName)
{
	std::string strFileName;
	ConvertWCHARToChar( strFileName, wstrFileName.c_str() );
		
	std::wofstream ofs;
	ofs.imbue(std::locale("kor"));
	ofs.open( wstrFileName.c_str() );
	//std::ofstream ofs( strFileName.c_str() );
	if( ofs.is_open() == false )
	{
		ofs.close();
		return;
	}

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
		
	// STAGE_LIST [BEGIN]	
	ofs << std::endl << std::endl;
	ofs << L"STAGE_LIST = " << std::endl;
	ofs << L"{" << std::endl;

	// DUNGEON_COLOR [BEGIN]	
	if( pDungeon->GetWorldColor().r != -1.f &&
		pDungeon->GetWorldColor().g != -1.f &&
		pDungeon->GetWorldColor().b != -1.f )
	{
		ofs << L"	DUNGEON_WORLD_COLOR = { ";
		ofs << static_cast<int>( pDungeon->GetWorldColor().r ) << L", ";
		ofs << static_cast<int>( pDungeon->GetWorldColor().g ) << L", ";
		ofs << static_cast<int>( pDungeon->GetWorldColor().b ) << L", },"<< std::endl;
	}

	if( pDungeon->GetUnitColor().r != -1.f &&
		pDungeon->GetUnitColor().g != -1.f &&
		pDungeon->GetUnitColor().b != -1.f )
	{
		ofs << L"	DUNGEON_UNIT_COLOR = { ";
		ofs << static_cast<int>( pDungeon->GetUnitColor().r ) << L", ";
		ofs << static_cast<int>( pDungeon->GetUnitColor().g ) << L", ";
		ofs << static_cast<int>( pDungeon->GetUnitColor().b ) << L", },"<< std::endl;
	}

	{
		map< std::pair<int,int>, CX2Dungeon::DungeonMapData > &mapDungeonMap = pDungeon->GetMapDungeonMapData4Tool();

		if( mapDungeonMap.size() > 0 )
		{
			// DUNGEON_MAP [BEGIN]
			ofs << std::endl;
			ofs << "	DUNGEON_MAP =" << std::endl;
			ofs << "	{"			   << std::endl;

			vector<CX2DungeonStage::StageData*>& vecStageData = pDungeon->GetStageDataList();
			for(UINT i=0; i<vecStageData.size(); ++i)
			{
				ofs << "		STAGE" << i << " =" << std::endl;
				ofs << "		{" << std::endl;

				CX2DungeonStage::StageData* pStageData = vecStageData[i];
				for(UINT j=0; j<pStageData->m_SubStageDataList.size(); ++j)
				{
					map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::iterator mit = mapDungeonMap.find(make_pair(i,j));
					if( mit != mapDungeonMap.end() )
					{
						CX2Dungeon::DungeonMapData mapData = mit->second;

						ofs << "			SUB_STAGE" << j << " = ";
						ofs << "{ ";
						ofs << (int)mapData.m_vPos.x << ", " << (int)mapData.m_vPos.y << ", ";
						if(mapData.m_bIsBossStage == true )
							ofs << " TRUE,";
						else
							ofs << " FALSE,";
						if(mapData.m_bIsNextStageRandom == true )
							ofs << " TRUE,";
						else
							ofs << " FALSE,";
						if(mapData.m_bRestRoomState == true )
							ofs << " TRUE,";
						else
							ofs << " FALSE,";

						ofs << " }," << std::endl;
					}
				}

				ofs << "		}," << std::endl;
			}

			// DUNGEON_MAP [END]
			ofs << "	},"			   << std::endl << std::endl << std::endl;
		}		
	}

	wstring wsTempStr = L"";
	string sWriteString = "";

	std::vector<CX2DungeonStage::StageData*> vecStageList = pDungeon->GetStageDataList();
	CX2DungeonStage::StageData* pStageData = NULL;

	for( UINT i=0; i < vecStageList.size(); ++i )
	{
		pStageData = vecStageList[i];

		// STAGE [BEGIN]
		ofs << L"	STAGE" << i << L" =" << std::endl;
		ofs << L"	{"					<< std::endl;

		if( pStageData->m_WorldID == CX2World::WI_NONE )
		{
			wsTempStr = pStageData->m_wstrWorldDataName;
			//ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );
			ofs << L"		WORLD_ID = WORLD_ID[\"WI_NONE\"]," << std::endl;
			ofs << L"		WORLD_DATA_NAME = \"" << wsTempStr << L"\"," << std::endl;
		}
		else
		{
			const CX2World::WorldData* pWorldData = g_pData->GetWorldManager()->GetWorldData(pStageData->m_WorldID);
			if( pWorldData != NULL )
				wsTempStr = pWorldData->worldName;			
			//ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );
			ofs << L"		WORLD_ID = WORLD_ID[\""	 << wsTempStr << L"\"]," << std::endl;
		}
		

		// DUNGEON_COLOR [BEGIN]	
		if( IsSamef(pStageData->m_WorldColor.r, -1.f) == false ||
			IsSamef(pStageData->m_WorldColor.g, -1.f) == false ||
			IsSamef(pStageData->m_WorldColor.b, -1.f) == false )
		{
			ofs << L"		WORLD_COLOR = { ";
			ofs << static_cast<int>( pStageData->m_WorldColor.r ) << L", ";
			ofs << static_cast<int>( pStageData->m_WorldColor.g ) << L", ";
			ofs << static_cast<int>( pStageData->m_WorldColor.b ) << L", },"<< std::endl;
		}

		if( IsSamef(pStageData->m_UnitColor.r, -1.f) == false ||
			IsSamef(pStageData->m_UnitColor.g, -1.f) == false ||
			IsSamef(pStageData->m_UnitColor.b, -1.f) == false )
		{
			ofs << L"		UNIT_COLOR = { ";
			ofs << static_cast<int>( pStageData->m_UnitColor.r ) << L", ";
			ofs << static_cast<int>( pStageData->m_UnitColor.g ) << L", ";
			ofs << static_cast<int>( pStageData->m_UnitColor.b ) << L", },"<< std::endl;
		}

		if( pStageData->m_bStartMotion )		
			ofs << L"		START_MOTION = TRUE"						 << L"," << std::endl;

		// READY_NPC [BEGIN]
		ofs << std::endl;
		ofs << L"		READY_NPC = "	<< std::endl;
		ofs << L"		{"	<< std::endl;

		std::vector<CX2UnitManager::NPC_UNIT_ID> vecNpcList = pStageData->m_ReadyNPCList;
		std::vector<CX2UnitManager::NPC_UNIT_ID>::iterator mit2 = vecNpcList.begin();
		for( ; mit2 != vecNpcList.end(); mit2++ )
		{
			wsTempStr = pToolMain->m_enumManager.GetNpcEnumString( *mit2 );
			//ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );
			ofs << L"			NPC_UNIT_ID[\"" << wsTempStr << L"\"]," << std::endl;
		}

		// READY_NPC [END]
		ofs << L"		},"				<< std::endl << std::endl;


		std::vector<CX2DungeonSubStage::SubStageData*> vecSubStageList = pStageData->m_SubStageDataList;
		CX2DungeonSubStage::SubStageData* pSubStageData = NULL;
		for( UINT j=0; j < vecSubStageList.size(); ++j )
		{
			pSubStageData = vecSubStageList[j];

			// SUB_STAGE [BEGIN]
			ofs << L"		SUB_STAGE" << j << L" =" << std::endl;
			ofs << L"		{" << std::endl;

			ofs << L"			START_LINE_SET      = " << pSubStageData->m_StartLineSet << L"," << std::endl;
			ofs << L"			MAIN_LINE_SET       = " << pSubStageData->m_MainLineSet  << L"," << std::endl;
			ofs << L"			END_LINE_SET        = " << pSubStageData->m_EndLineSet	 << L"," << std::endl;
			ofs << std::endl;

			wstring wstrGoTypeValue = wstrGoType[pSubStageData->m_GoType];
			//ConvertWCHARToChar(sWriteString, wstrGoTypeValue.c_str());			
			ofs << L"			GO_TYPE             = GO_TYPE[\"" << wstrGoTypeValue		 << L"\"]," << std::endl;
			ofs << std::endl;

			//////////////////////////////////////////////////////////////////////////

			// CLEAR CONDITION [BEGIN]
			std::vector<CX2DungeonSubStage::ClearCondtionData>& vecClearConList = pSubStageData->m_vecClearCondData;

			for( int k=0; k < (int)vecClearConList.size(); ++k )
			{
				CX2DungeonSubStage::ClearCondtionData& rClearConData = vecClearConList[k];

				ofs << L"			CLEAR_COND" << k << L" =" << std::endl;
				ofs << L"			{" << std::endl;
				wstring wstrClearCondValue = wstrStageClearType[rClearConData.m_eClearCondition];
				//ConvertWCHARToChar(sWriteString, wstrClearCondValue.c_str());
				ofs << L"				CLEAR_CONDITION[\"" << wstrClearCondValue << L"\"]," << std::endl;

				switch( rClearConData.m_eClearCondition ) 
				{
				case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
				case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:
					{
						ofs << L"				NPC_KEYCODE = { ";
						std::vector<CX2DungeonSubStage::ClearCondtionData::MonsterCount>::iterator vitKeyCode = rClearConData.m_vecNPCKeyCode.begin();
						bool bOnlyOne = true;
						for( ; vitKeyCode != rClearConData.m_vecNPCKeyCode.end(); vitKeyCode++ )
						{
							if( !bOnlyOne )
								ofs << L", ";

							ofs << vitKeyCode->m_iKey;							

							bOnlyOne = ( rClearConData.m_vecNPCKeyCode.size() == 1 );
						}
						ofs << L" }," << std::endl;
					}
					break;

				case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
					{
						ofs << L"				NPC_ID	 =" << std::endl;
						ofs << L"				{ "			<< std::endl;
						std::vector<CX2DungeonSubStage::ClearCondtionData::MonsterCount>::iterator vitNpcID = rClearConData.m_vecNPCID.begin();
						for( ; vitNpcID != rClearConData.m_vecNPCID.end(); vitNpcID++ )
						{
							std::wstring wsTempStr = pToolMain->m_enumManager.GetNpcEnumString( static_cast<CX2UnitManager::NPC_UNIT_ID>(vitNpcID->m_iKey) );
							//ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );
							ofs << L"					NPC_UNIT_ID[\"" << wsTempStr << L"\"]," << std::endl;
						}
						ofs << L"				}," << std::endl;
					}
					break;
					
				case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_COUNT:
					{
						ofs << L"				NPC_KEYCODE = { ";
						std::vector<CX2DungeonSubStage::ClearCondtionData::MonsterCount>::iterator vitKeyCode = rClearConData.m_vecNPCKeyCode.begin();						
						for( ; vitKeyCode != rClearConData.m_vecNPCKeyCode.end(); vitKeyCode++ )
						{
							ofs << vitKeyCode->m_iKey << L", ";	
							ofs << vitKeyCode->m_iObjectiveCount << L", ";
						}
						ofs << L" }," << std::endl;
					}
					break;

				case CX2DungeonSubStage::CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC:
					{
						ofs << L"				PORTAL_LINE_INDEX = { ";
						for(UINT iTemp = 0; iTemp < rClearConData.m_vecPortalLineIndex.size(); ++iTemp)
						{
							ofs << rClearConData.m_vecPortalLineIndex[iTemp] << L", ";
						}
						ofs << L" }," << std::endl;
					}
					break;

				case CX2DungeonSubStage::CC_TIME:
					{
						ofs << L"				CLEAR_TIME = " << rClearConData.m_fClearTime << L"," << std::endl;
					}
					break;
				default:
					break;
				}
				for( int iNextStage=0; iNextStage < (int)rClearConData.m_vecNextStage.size(); ++iNextStage )
				{
					CX2DungeonSubStage::NextStageData& rNextStageData = rClearConData.m_vecNextStage[iNextStage];

					ofs << L"				NEXT_STAGE" << iNextStage << L" = { ";
					wstring wstrClearTypeValue = wstrClearType[rNextStageData.m_eClearType];
					//ConvertWCHARToChar(sWriteString, wstrClearTypeValue.c_str() );
					ofs << L"CLEAR_TYPE[\"" << wstrClearTypeValue << L"\"], ";
					ofs << rNextStageData.m_iStageIndex		<< L", ";
					ofs << rNextStageData.m_iSubStageIndex	<< L", ";
					ofs << rNextStageData.m_iRate			<< L", }," << std::endl;
				}

				// CLEAR CONDITION [END]
				ofs << L"			}," << std::endl << std::endl;
			}

			//////////////////////////////////////////////////////////////////////////			

			std::vector<CX2DungeonSubStage::CurtainData*> vecCurtainList = pSubStageData->m_CurtainDataList;
			std::vector<CX2DungeonSubStage::CurtainData*>::iterator mit4 = vecCurtainList.begin();
			CX2DungeonSubStage::CurtainData* pCurtain;
			if( mit4 != vecCurtainList.end() )
			{
				// CURTAIN [BEGIN]
				ofs << L"			CURTAIN_GROUP =" << std::endl;
				ofs << L"			{"				 << std::endl;

				for( ; mit4 != vecCurtainList.end(); mit4++ )
				{
					pCurtain = *mit4;

					ofs << L"				{"			 << std::endl;
					ofs << L"					LINE_INDEX	= " << pCurtain->m_LineIndex << L"," << std::endl;
					if( pCurtain->m_bAtStart == true )
						ofs << L"					AT_START	= TRUE," << std::endl;
					else
						ofs << L"					AT_START	= FALSE," << std::endl;
					if( pCurtain->m_bLookLeft == true )
						ofs << L"					LOOK_LEFT	= TRUE," << std::endl;
					else
						ofs << L"					LOOK_LEFT	= FALSE," << std::endl;

					ofs << L"				},"			 << std::endl;
				}			

				// CURTAIN [END]
				ofs << L"			},"				 << std::endl << std::endl;
			}

			// NPC_GROUP [BEGIN]
			ofs << L"			NPC_GROUP ="	 << std::endl;
			ofs << L"			{"				 << std::endl;

			
			vector<int> vecSubNpcIndex;			
			vector<CX2DungeonSubStage::NPCData*> vecNpcList = pSubStageData->m_vecSubStageNpcData;
			CX2DungeonSubStage::NPCData* pNpcData = NULL;
			for( UINT iNpc = 0; iNpc != vecNpcList.size(); ++iNpc )
			{
				pNpcData = vecNpcList[iNpc];
				if( pNpcData->m_bSubNpc == false )
				{
					ofs << L"				{"		 << std::endl;

					wsTempStr = pToolMain->m_enumManager.GetNpcEnumString( pNpcData->m_UnitID );
					//ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );					
					ofs << L"					NPC_ID			= NPC_UNIT_ID[\"" << wsTempStr << L"\"]," << std::endl;
					std::vector<int> vecStartPos = pNpcData->m_vecStartPos;
					std::vector<int>::iterator mit7 = vecStartPos.begin();				
					ofs << L"					START_POS		= { ";
					for( ; mit7 != vecStartPos.end(); mit7++ )
					{
						ofs << *mit7 << L", ";
					}
					ofs << L"}," << std::endl;
					if( pNpcData->m_iLevel4Tool > 0 )
						ofs << L"					LEVEL			= " << pNpcData->m_iLevel4Tool	 << L"," << std::endl;
					if( !pNpcData->m_bActive )				
						ofs << L"					ACTIVE			= FALSE," << std::endl;
					if( !pNpcData->m_bShowGage )
						ofs << L"					GAGE_BAR		= FALSE," << std::endl;
					if( pNpcData->m_bHasBossGage )
						ofs << L"					HAVE_BOSS_GAGE	= TRUE," << std::endl;
					if( pNpcData->m_bShowBossName )
						ofs << L"					SHOW_BOSS_NAME	= TRUE," << std::endl;
					if( pNpcData->m_bShowSubBossName )
						ofs << L"					SHOW_SUB_BOSS_NAME	= TRUE," << std::endl;

					if( IsSamef(pNpcData->m_AddPos.x) == false )
						ofs << L"					ADD_POS_X		= " << pNpcData->m_AddPos.x << L"," << std::endl;
					if( IsSamef(pNpcData->m_AddPos.y) == false )
						ofs << L"					ADD_POS_Y		= " << pNpcData->m_AddPos.y << L"," << std::endl;
					if( IsSamef(pNpcData->m_AddPos.z) == false )
						ofs << L"					ADD_POS_Z		= " << pNpcData->m_AddPos.z << L"," << std::endl;
					if( pNpcData->m_Rate != 100 )
						ofs << L"					RATE			= " << pNpcData->m_Rate		<< L"," << std::endl;
					if( pNpcData->m_KeyCode > 0 )
						ofs << L"					KEY_CODE		= " << pNpcData->m_KeyCode	<< L"," << std::endl;
					if( pNpcData->m_bSiegeMode )
						ofs << L"					SIEGE_MODE		= TRUE," << std::endl;
					if( pNpcData->m_bIsRight )
						ofs << L"					IS_RIGHT		= TRUE," << std::endl;
					if( pNpcData->m_bFocusCamera )
						ofs << L"					FOCUS_CAMERA	= TRUE," << std::endl;
					if( pNpcData->m_bNoDrop )
						ofs << L"					NO_DROP			= TRUE," << std::endl;

					if( IsSamef(pNpcData->m_fUnitScale, -1.f) == false )
						ofs << L"					UNIT_SCALE		= " << pNpcData->m_fUnitScale << L"," << std::endl;
					if( pNpcData->m_usBossGaugeHPLines != 1 )
						ofs << L"					BOSS_GAUGE_HP_LINES	= " << pNpcData->m_usBossGaugeHPLines << L", " << std::endl;
					if( pNpcData->m_bAllyNpc )
						ofs << L"					ALLY_NPC		= TRUE," << std::endl;
					if( pNpcData->m_MonsterGrade > CX2DungeonSubStage::MG_NORMAL_NPC )
						ofs << L"					MONSTER_GRADE	= " << pNpcData->m_MonsterGrade << L", " << std::endl;

					ofs << L"				},"		 << std::endl;
					
				}
				else
				{
					bool bFindSubIndex = false;					
					for( UINT iSubIndex = 0; iSubIndex < vecSubNpcIndex.size(); ++iSubIndex )
					{
						if( pNpcData->m_iSubNPCIndex == vecSubNpcIndex[iSubIndex] )
						{
							bFindSubIndex = true;
							break;
						}
					}
					if( bFindSubIndex == false )
						vecSubNpcIndex.push_back(pNpcData->m_iSubNPCIndex);


				}
			}

			for( UINT iTemp = 0; iTemp != vecSubNpcIndex.size(); ++iTemp )
			{
				int iSubIndex = vecSubNpcIndex[iTemp];

				// write for SUB_NPC [BEGIN]
				ofs << std::endl;				
				ofs << L"				{"		 << std::endl;
				int iSubNpcCount = 0;
				for( UINT iNpc = 0; iNpc != vecNpcList.size(); ++iNpc )
				{
					pNpcData = vecNpcList[iNpc];
					if( pNpcData->m_bSubNpc == true && pNpcData->m_iSubNPCIndex == iSubIndex )
					{
						ofs << L"					SUB_NPC" << iSubNpcCount << L" =" << std::endl;
						ofs << L"					{"	 << std::endl;

						ofs << L"						SUB_NPC_RATE	= " << pNpcData->m_iSubNPCRate		<< L"," << std::endl;

						wsTempStr = pToolMain->m_enumManager.GetNpcEnumString( pNpcData->m_UnitID );
						//ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );					
						ofs << L"						NPC_ID			= NPC_UNIT_ID[\"" << wsTempStr << L"\"]," << std::endl;
						std::vector<int> vecStartPos = pNpcData->m_vecStartPos;
						std::vector<int>::iterator mit7 = vecStartPos.begin();				
						ofs << L"						START_POS		= { ";
						for( ; mit7 != vecStartPos.end(); mit7++ )
						{
							ofs << *mit7 << ", ";
						}
						ofs << L"}," << std::endl;
						if( pNpcData->m_iLevel4Tool > 0 )
							ofs << L"						LEVEL			= " << pNpcData->m_iLevel4Tool	 << L"," << std::endl;
						if( !pNpcData->m_bActive )				
							ofs << L"						ACTIVE			= FALSE," << std::endl;
						if( !pNpcData->m_bShowGage )
							ofs << L"						GAGE_BAR		= FALSE," << std::endl;
						if( pNpcData->m_bHasBossGage )
							ofs << L"						HAVE_BOSS_GAGE	= TRUE," << std::endl;
						if( pNpcData->m_bShowBossName )
							ofs << L"						SHOW_BOSS_NAME	= TRUE," << std::endl;
						if( pNpcData->m_bShowSubBossName )
							ofs << L"						SHOW_SUB_BOSS_NAME	= TRUE," << std::endl;

						if( IsSamef(pNpcData->m_AddPos.x) == false )
							ofs << L"						ADD_POS_X		= " << pNpcData->m_AddPos.x << L"," << std::endl;
						if( IsSamef(pNpcData->m_AddPos.y) == false )
							ofs << L"						ADD_POS_Y		= " << pNpcData->m_AddPos.y << L"," << std::endl;
						if( IsSamef(pNpcData->m_AddPos.z) == false )
							ofs << L"						ADD_POS_Z		= " << pNpcData->m_AddPos.z << L"," << std::endl;
						if( pNpcData->m_Rate != 100 )
							ofs << L"						RATE			= " << pNpcData->m_Rate		<< L"," << std::endl;
						if( pNpcData->m_KeyCode > 0 )
							ofs << L"						KEY_CODE		= " << pNpcData->m_KeyCode	<< L"," << std::endl;
						if( pNpcData->m_bSiegeMode )
							ofs << L"						SIEGE_MODE		= TRUE," << std::endl;
						if( pNpcData->m_bIsRight )
							ofs << L"						IS_RIGHT		= TRUE," << std::endl;
						if( pNpcData->m_bFocusCamera )
							ofs << L"						FOCUS_CAMERA	= TRUE," << std::endl;
						if( pNpcData->m_bNoDrop )
							ofs << L"						NO_DROP			= TRUE," << std::endl;

						if( IsSamef(pNpcData->m_fUnitScale, -1.f) == false )
							ofs << L"						UNIT_SCALE		= " << pNpcData->m_fUnitScale << L"," << std::endl;
						if( pNpcData->m_usBossGaugeHPLines != 1 )
							ofs << L"						BOSS_GAUGE_HP_LINES	= " << pNpcData->m_usBossGaugeHPLines << L", " << std::endl;
						if( pNpcData->m_bAllyNpc )
							ofs << L"						ALLY_NPC		= TRUE," << std::endl;
						if( pNpcData->m_MonsterGrade > CX2DungeonSubStage::MG_NORMAL_NPC )
							ofs << L"					MONSTER_GRADE		= " << pNpcData->m_MonsterGrade << L", " << std::endl;

						ofs << L"					},"		 << std::endl;
						++iSubNpcCount;
					}
				}

				ofs << L"				},"		 << std::endl;
			}

			// NPC_GROUP [END]
			ofs << L"			},"				 << std::endl << std::endl;

			// TRIGGER_LIST [BEGIN]
			ofs << L"			TRIGGER_LIST ="  << std::endl;
			ofs << L"			{"				 << std::endl;

			vector<CX2DungeonSubStage::Trigger*> vecTirggerList = pSubStageData->m_TriggerList;			
			CX2DungeonSubStage::Trigger* pTrigger = NULL;
			for( UINT iTrigger=0 ; iTrigger < vecTirggerList.size(); ++iTrigger )
			{
				pTrigger = vecTirggerList[iTrigger];

				ofs << L"				TRIGGER" << iTrigger << L" =" << std::endl;
				ofs << L"				{"					  << std::endl;
				if( pTrigger->m_bHost == true )
					ofs << L"					HOST			= TRUE," << std::endl;
				else
					ofs << L"					HOST			= FALSE," << std::endl;
				if( pTrigger->m_bOneTime == true )
					ofs << L"					ONE_TIME			= TRUE," << std::endl;
				else
					ofs << L"					ONE_TIME			= FALSE," << std::endl;
				ofs << L"					INTERVAL		= " << pTrigger->m_Interval << L"," << std::endl;
				//ConvertWCHARToChar( sWriteString, pTrigger->m_ConditionFunc.c_str() );
				ofs << L"					CONDITION_FUNC	= \"" << pTrigger->m_ConditionFunc		<< L"\"," << std::endl;
				//ConvertWCHARToChar( sWriteString, pTrigger->m_ReactFunc.c_str() );
				ofs << L"					REACT_FUNG		= \"" << pTrigger->m_ReactFunc		<< L"\"," << std::endl;
				ofs << L"				},"					  << std::endl;
			}

			// TRIGGER_LIST [END]
			ofs << L"			},"				 << std::endl;

			// SUB_STAGE [END]
			ofs << L"		},"				<< std::endl << std::endl;
		}

		// STAGE [END]
		ofs << L"	},"				<< std::endl << std::endl;
	}

	// STAGE_LIST [END]
	ofs << L"}" << std::endl;

	// TRIGGER FUNCTION
	ofs << std::endl << L"-- Function Data ---------------------------------------------" << std::endl;
	CString csFunctionData;
	m_DlgTriggerFunc.GetDlgItem(IDC_EDIT_TRIGGER_FUNC)->GetWindowTextW(csFunctionData);
	wstring wstrFunctionData = csFunctionData.GetString();
	ofs << wstrFunctionData << std::endl;

	// TRIGGER
	ofs.close();

	ConvertFullPathFileAnsiToUTF8( strFileName, strFileName );
}

void CEditDungeon::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Dungeon Map
	if( g_pX2Game != NULL )
	{		
		m_DlgDungeonMap.ShowWindow(SW_SHOW);
		m_DlgDungeonMap.UpdateDungeonMap();
	}
}
