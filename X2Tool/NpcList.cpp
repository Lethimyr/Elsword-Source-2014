// NpcList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "X2Tool.h"
#include "NpcList.h"
#include "afxdialogex.h"
#include "EditDungeon.h"

EnumStringTable wstrNpcType4NpcList[] =
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

// CNpcList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNpcList, CDialogEx)

CNpcList::CNpcList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNpcList::IDD, pParent)
{

}

CNpcList::~CNpcList()
{
}

void CNpcList::InitNpcData()
{
	CX2ToolMain *pToolMain = (CX2ToolMain *)g_pMain;

	m_listNpcData.DeleteColumn(3);
	m_listNpcData.DeleteColumn(2);
	m_listNpcData.DeleteColumn(1);
	m_listNpcData.DeleteColumn(0);
	m_listNpcData.DeleteAllItems();
	m_listNpcData.InsertColumn(0, _T("Npc ID"), LVCFMT_CENTER, 300 );
	m_listNpcData.InsertColumn(1, _T("Npc Type"), LVCFMT_CENTER, 120 );
	m_listNpcData.InsertColumn(2, _T("Npc Name"), LVCFMT_CENTER, 150 );
	m_listNpcData.InsertColumn(3, _T("Npc File Name"), LVCFMT_CENTER, 200 );

	int iIndex = 0;
	
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring > mapNpcTemplet; 
	g_pData->GetUnitManager()->GetNPCUnitList(mapNpcTemplet);
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator mit = mapNpcTemplet.begin();
	while( mit != mapNpcTemplet.end() )
	{
		CX2UnitManager::NPCUnitTemplet *pNpcTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet((CX2UnitManager::NPC_UNIT_ID)mit->first);
		if( pNpcTemplet != NULL && pNpcTemplet->m_nNPCUnitID < CX2UnitManager::NUI_NPC_END )
		{
			WCHAR wbuf[128];
			wsprintf( wbuf, L"%s", pToolMain->m_enumManager.GetNpcEnumString( pNpcTemplet->m_nNPCUnitID ).c_str() );
			m_listNpcData.InsertItem(iIndex, wbuf);

			for( int j=0; j<ARRAY_SIZE(wstrNpcType4NpcList); ++j)
			{
				if( wstrNpcType4NpcList[j].inx == pNpcTemplet->m_ClassType )
				{
					m_listNpcData.SetItemText(iIndex, 1, wstrNpcType4NpcList[j].strName);
					break;
				}
			}	
			m_listNpcData.SetItemText(iIndex, 2, pNpcTemplet->m_Name.c_str());
			m_listNpcData.SetItemText(iIndex, 3, pNpcTemplet->m_LuaFileName.c_str());

			++iIndex;
		}
		++mit;
	}

	m_listNpcData.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );
}

void CNpcList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listNpcData);
}


BEGIN_MESSAGE_MAP(CNpcList, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNpcList::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CNpcList::OnNMDblclkList1)
END_MESSAGE_MAP()


// CNpcList 메시지 처리기입니다.


void CNpcList::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	m_vecNpc.clear();
	for(int i=0; i<m_listNpcData.GetItemCount(); ++i)
	{
		UINT uItemState = m_listNpcData.GetItemState ( i, LVIS_SELECTED );
		if( uItemState == LVIS_SELECTED )
		{
			CString csNpcId = m_listNpcData.GetItemText(i, 0);
			wstring wstrNpcId = csNpcId.GetString();
			m_vecNpc.push_back( pToolMain->m_enumManager.GetNpcEnum(wstrNpcId) );
		}
	}

	if( m_vecNpc.size() > 0 )
	{
		CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->m_DlgDungeonEditor.AddReadyNpc(m_vecNpc);
	}
	
	CDialogEx::OnOK();
}


void CNpcList::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	m_vecNpc.clear();
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	int iItem = pNMItemActivate->iItem;
	CString csNpcId = m_listNpcData.GetItemText(iItem, 0);
	wstring wstrNpcId = csNpcId.GetString();
	m_vecNpc.push_back( pToolMain->m_enumManager.GetNpcEnum(wstrNpcId) );

	if( m_vecNpc.size() > 0 )
	{
		CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->m_DlgDungeonEditor.AddReadyNpc(m_vecNpc);
	}
}
