// ToolDungeonMap.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "ToolDungeonMap.h"
//#include "EditDungeon.h"
#include "afxdialogex.h"

extern CGlobal Global;

// CToolDungeonMap 대화 상자입니다.

IMPLEMENT_DYNAMIC(CToolDungeonMap, CDialogEx)

CToolDungeonMap::CToolDungeonMap(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolDungeonMap::IDD, pParent)
{

}

CToolDungeonMap::~CToolDungeonMap()
{
}

void CToolDungeonMap::UpdateDungeonMap()
{
	CStringArray saBoolean;
	saBoolean.Add(_T("TRUE"));
	saBoolean.Add(_T("FALSE"));

	m_listDungeonMap.DeleteColumn(7);
	m_listDungeonMap.DeleteColumn(6);
	m_listDungeonMap.DeleteColumn(5);
	m_listDungeonMap.DeleteColumn(4);
	m_listDungeonMap.DeleteColumn(3);
	m_listDungeonMap.DeleteColumn(2);
	m_listDungeonMap.DeleteColumn(1);
	m_listDungeonMap.DeleteColumn(0);
	m_listDungeonMap.DeleteAllItems();

	m_listDungeonMap.InsertColumn(0, _T("Active"), LVCFMT_CENTER, 70, 0 );
	m_listDungeonMap.InsertColumn(1, _T("Stage"), LVCFMT_CENTER, 70, 1 );
	m_listDungeonMap.InsertColumn(2, _T("SubStage"), LVCFMT_CENTER, 70, 2 );
	m_listDungeonMap.InsertColumn(3, _T("X"), LVCFMT_CENTER, 50, 3 );
	m_listDungeonMap.InsertColumn(4, _T("Y"), LVCFMT_CENTER, 50, 4 );
	m_listDungeonMap.InsertColumn(5, _T("Boss"), LVCFMT_CENTER, 70, 5 );
	m_listDungeonMap.InsertColumn(6, _T("Random"), LVCFMT_CENTER, 70, 6 );
	m_listDungeonMap.InsertColumn(7, _T("Rest"), LVCFMT_CENTER, 70, 7 );

	m_listDungeonMap.SetUnderLine(FALSE);	
	m_listDungeonMap.m_HeaderCtrl.SetHeight(30);
	m_listDungeonMap.m_HeaderCtrl.SetTextFont( Global.GetFont(IDX_FONT_SMALL) );
	m_listDungeonMap.SetTextFont( Global.GetFont(IDX_FONT_SMALL) );

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

	map< std::pair<int,int>, CX2Dungeon::DungeonMapData > mapDungeonMap = pDungeon->GetMapDungeonMapData4Tool();

	int iRow = -1;
	vector<CX2DungeonStage::StageData*>& vecStageData = pDungeon->GetStageDataList();
	for(UINT i=0; i<vecStageData.size(); ++i)
	{
		CX2DungeonStage::StageData* pStageData = vecStageData[i];
		for(UINT j=0; j<pStageData->m_SubStageDataList.size(); ++j)
		{
			++iRow;

			CX2DungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[j];
			CX2Dungeon::DungeonMapData mapData;

			bool bExistMap = false;
			map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::iterator mit = mapDungeonMap.find(make_pair(i, j));
			if( mit != mapDungeonMap.end() )
			{
				bExistMap = true;
				mapData = mit->second;
			}

			

			WCHAR wbuf[100];			
			if( bExistMap == true )
			{	
				m_listDungeonMap.InsertItem(iRow,  L"TRUE");
				m_listDungeonMap.SetComboBox(iRow, 0, TRUE, &saBoolean );
			}
			else
			{
				m_listDungeonMap.InsertItem(iRow,  L"FALSE", RGB(255, 0, 0), RGB(255, 255, 255));
				m_listDungeonMap.SetComboBox(iRow, 0, TRUE, &saBoolean );
			}

			wsprintfW(wbuf, L"%d", i);
			m_listDungeonMap.SetItemText(iRow, 1, wbuf);
			wsprintfW(wbuf, L"%d", j);
			m_listDungeonMap.SetItemText(iRow, 2, wbuf);
			
			WCHAR wbufX[10];
			WCHAR wbufY[10];
			WCHAR wbufBoss[10];
			WCHAR wbufRandom[10];
			WCHAR wbufRest[10];
			if( bExistMap == true )
			{
				wsprintfW(wbufX, L"%d", (int)mapData.m_vPos.x);
				wsprintfW(wbufY, L"%d", (int)mapData.m_vPos.y);
				if( mapData.m_bIsBossStage == true )
					wsprintfW(wbufBoss, L"TRUE");
				else
					wsprintfW(wbufBoss, L"FALSE");
				if( mapData.m_bIsNextStageRandom == true )
					wsprintfW(wbufRandom, L"TRUE");
				else
					wsprintfW(wbufRandom, L"FALSE");
				if( mapData.m_bRestRoomState == true )
					wsprintfW(wbufRest, L"TRUE");
				else
					wsprintfW(wbufRest, L"FALSE");
			}
			else
			{
				wsprintfW(wbufX, L"%d", 0);
				wsprintfW(wbufY, L"%d", 0);
				wsprintfW(wbufBoss, L"FALSE");
				wsprintfW(wbufRandom, L"FALSE");
				wsprintfW(wbufRest, L"FALSE");
			}
			
			m_listDungeonMap.SetItemText(iRow, 3, wbufX);			
			m_listDungeonMap.SetItemText(iRow, 4, wbufY);
			m_listDungeonMap.SetItemText(iRow, 5, wbufBoss);
			m_listDungeonMap.SetItemText(iRow, 6, wbufRandom);
			m_listDungeonMap.SetItemText(iRow, 7, wbufRest);

			m_listDungeonMap.SetEditBox(iRow, 3, TRUE, -1000, 1000);
			m_listDungeonMap.SetEditBox(iRow, 4, TRUE, -1000, 1000);
			m_listDungeonMap.SetComboBox(iRow, 5, TRUE, &saBoolean );
			m_listDungeonMap.SetComboBox(iRow, 6, TRUE, &saBoolean );
			m_listDungeonMap.SetComboBox(iRow, 7, TRUE, &saBoolean );
		}
	}

	m_listDungeonMap.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

	OnPaintMap();
}

void CToolDungeonMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DUNGEON_MAP, m_listDungeonMap);
}


BEGIN_MESSAGE_MAP(CToolDungeonMap, CDialogEx)
	//ON_WM_PAINT()
	ON_REGISTERED_MESSAGE( WM_XLISTCTRL_COMBOBOX_CHANGE, CToolDungeonMap::OnComboChange)
	ON_REGISTERED_MESSAGE( WM_XLISTCTRL_EDITBOX_CHANGE, CToolDungeonMap::OnEditChange)
END_MESSAGE_MAP()



// CToolDungeonMap 메시지 처리기입니다.
void CToolDungeonMap::OnPaintMap()
{
	if( GetDlgItem(IDC_VIEW_MAP) == NULL )
		return;

	CDC* pDC = GetDlgItem(IDC_VIEW_MAP)->GetDC();
	CRect rc;
	GetDlgItem(IDC_VIEW_MAP)->GetWindowRect(&rc);
	GetDlgItem(IDC_VIEW_MAP)->ScreenToClient(&rc);

	int iWidht = rc.Width();
	int iHeight = rc.Height();

	COLORREF crText = RGB(255, 0, 0);
	COLORREF crBkgnd = RGB(255, 255, 255);		
	pDC->SetTextColor(crText);
	pDC->SetBkColor(crBkgnd);
	pDC->SelectStockObject(ANSI_VAR_FONT);
	pDC->FillSolidRect(&rc, crBkgnd);

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	map< std::pair<int,int>, CX2Dungeon::DungeonMapData > &mapDungeonMap = pDungeon->GetMapDungeonMapData4Tool();

	vector<CX2DungeonStage::StageData*>& vecStageData = pDungeon->GetStageDataList();
	for(UINT i=0; i<vecStageData.size(); ++i)
	{
		CX2DungeonStage::StageData* pStageData = vecStageData[i];
		for(UINT j=0; j<pStageData->m_SubStageDataList.size(); ++j)
		{
			map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::iterator mit = mapDungeonMap.find(make_pair(i,j));
			if( mit != mapDungeonMap.end() )
			{
				CX2Dungeon::DungeonMapData mapData = mit->second;

				int iX, iY;
				iX = static_cast<int>(mapData.m_vPos.x * iWidht * 1.3f) / 1024;
				iY = static_cast<int>(mapData.m_vPos.y * iHeight * 1.3f) / 768;
				
				WCHAR wBuf[10];
				wsprintfW(wBuf, L"[%d-%d]", i, j);
				pDC->TextOutW( iX+10, iY+10, wBuf );
			}
		}
	}
	

	//pDC->RestoreDC(nSavedDC);
	ReleaseDC(pDC);
}

LRESULT CToolDungeonMap::OnComboChange(WPARAM wParam, LPARAM lParam)
{
	int iItemIndex = (int)wParam;
	int iSubItemIndex = (int)lParam;

	static int iOldItem = -1;
	static int iOldSubItem = -1;
	static CString csOldChangedItem = L"";

	int iStage = -1;
	int iSubStage = -1;

	CString csStage = m_listDungeonMap.GetItemText(iItemIndex, 1);
	CString csSubStage = m_listDungeonMap.GetItemText(iItemIndex, 2);

	iStage = _ttoi(csStage.GetString());
	iSubStage = _ttoi(csSubStage.GetString());
	CString csChangedItem = m_listDungeonMap.GetItemText(iItemIndex, iSubItemIndex);

	if( iItemIndex == iOldItem && iSubItemIndex == iOldSubItem && csChangedItem == csOldChangedItem )
		return 0L;

	iOldItem = iItemIndex;
	iOldSubItem = iSubItemIndex;
	csOldChangedItem = csChangedItem;

	bool bValue = false;
	if( csOldChangedItem.Compare(L"TRUE") == 0 )
		bValue = true;

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	map< std::pair<int,int>, CX2Dungeon::DungeonMapData > &mapDungeonMap = pDungeon->GetMapDungeonMapData4Tool();
	
	CX2Dungeon::DungeonMapData mapData;

	map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::iterator mit = mapDungeonMap.find(make_pair(iStage, iSubStage));
	if( mit != mapDungeonMap.end() )
	{
		mapData = mit->second;
		switch( iSubItemIndex )
		{
		case 0:
			{
				// 삭제
				if( bValue == false )
				{
					mapDungeonMap.erase(make_pair(iStage, iSubStage));
					UpdateDungeonMap();
				}
			}
			break;
		case 5:
			{
				// boss
				mapData.m_bIsBossStage = bValue;
				mapDungeonMap[ make_pair(iStage, iSubStage) ] = mapData;
			}
			break;
		case 6:
			{
				// random
				mapData.m_bIsNextStageRandom = bValue;
				mapDungeonMap[ make_pair(iStage, iSubStage) ] = mapData;
			}
			break;
		case 7:
			{
				// rest
				mapData.m_bRestRoomState = bValue;
				mapDungeonMap[ make_pair(iStage, iSubStage) ] = mapData;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		if( iSubItemIndex == 0 && bValue == true )
		{
			CString csTemp = m_listDungeonMap.GetItemText(iItemIndex, 3);
			int iTemp = _ttoi(csTemp.GetString());
			mapData.m_vPos.x = (float)iTemp;

			csTemp = m_listDungeonMap.GetItemText(iItemIndex, 4);
			iTemp = _ttoi(csTemp.GetString());
			mapData.m_vPos.y = (float)iTemp;

			csTemp = m_listDungeonMap.GetItemText(iItemIndex, 5);
			bool bTemp = false;
			if( csTemp.Compare(L"TRUE") == 0 )
				bTemp = true;			
			mapData.m_bIsBossStage = bTemp;

			csTemp = m_listDungeonMap.GetItemText(iItemIndex, 6);
			bTemp = false;
			if( csTemp.Compare(L"TRUE") == 0 )
				bTemp = true;			
			mapData.m_bIsNextStageRandom = bTemp;

			csTemp = m_listDungeonMap.GetItemText(iItemIndex, 7);
			bTemp = false;
			if( csTemp.Compare(L"TRUE") == 0 )
				bTemp = true;			
			mapData.m_bRestRoomState = bTemp;

			mapDungeonMap[ make_pair(iStage, iSubStage) ] = mapData;
			UpdateDungeonMap();
		}

	}

	return 0L;
}

 LRESULT CToolDungeonMap::OnEditChange( WPARAM wParam, LPARAM lParam )
 {
	 int iItemIndex = (int)wParam;
	 int iSubItemIndex = (int)lParam;

	 static int iOldItem = -1;
	 static int iOldSubItem = -1;
	 static CString csOldChangedItem = L"";

	 int iStage = -1;
	 int iSubStage = -1;

	 CString csStage = m_listDungeonMap.GetItemText(iItemIndex, 1);
	 CString csSubStage = m_listDungeonMap.GetItemText(iItemIndex, 2);

	 iStage = _ttoi(csStage.GetString());
	 iSubStage = _ttoi(csSubStage.GetString());
	 CString csChangedItem = m_listDungeonMap.GetItemText(iItemIndex, iSubItemIndex);

	 if( iItemIndex == iOldItem && iSubItemIndex == iOldSubItem && csChangedItem == csOldChangedItem )
		 return 0L;

	 int iValue = _ttoi(csChangedItem.GetString());

	 iOldItem = iItemIndex;
	 iOldSubItem = iSubItemIndex;
	 csOldChangedItem = csChangedItem;

	 CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	 CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
	 map< std::pair<int,int>, CX2Dungeon::DungeonMapData > &mapDungeonMap = pDungeon->GetMapDungeonMapData4Tool();

	 CX2Dungeon::DungeonMapData mapData;

	 map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::iterator mit = mapDungeonMap.find(make_pair(iStage, iSubStage));
	 if( mit != mapDungeonMap.end() )
	 {
		 mapData = mit->second;

		 switch(iSubItemIndex)
		 {
		 case 3:
			 {
				 // x
				 mapData.m_vPos.x = (float)iValue;
				 mapDungeonMap[ make_pair(iStage, iSubStage) ] = mapData;
				 OnPaintMap();
			 }
			 break;
		 case 4:
			 {
				 // y
				  mapData.m_vPos.y = (float)iValue;
				  mapDungeonMap[ make_pair(iStage, iSubStage) ] = mapData;
				  OnPaintMap();
			 }
			 break;
		 default:
			 break;
		 }
	 }

	 return 0L;
 }

 BOOL CToolDungeonMap::PreTranslateMessage(MSG* pMsg)
 {
	 if( pMsg->message == WM_KEYDOWN )
	 {
		 switch( pMsg->wParam )
		 {
		 case VK_RETURN:
			 return TRUE;
		 default:
			 break;
		 }
	 }

	 return CDialogEx::PreTranslateMessage(pMsg);
 }