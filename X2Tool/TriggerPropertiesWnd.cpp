
#include "stdafx.h"

#include "TriggerPropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "X2Tool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vector<int> m_vAddTriggerButtonId;

wstring wstrTriggerMeshActionType[] = 
{
	L"TAT_MESH_NONE",
	L"TAT_MESH_CHANGE_ANIM",
	L"TAT_MESH_CHANGE_SPEED",
};
wstring wstrTriggerLineActionType[] = 
{
	L"TAT_LINE_NONE",
	L"TAT_LINE_CHANGE_ENABLE",
	L"TAT_LINE_CHANGE_SPEED",
	L"TAT_LINE_CHANGE_JUMPSPEED",
	L"TAT_LINE_CHANGE_CAMERA",
	L"TAT_LINE_CHANGE_CANPASS",
	L"TAT_LINE_CHANGE_TELEPORT",
};
wstring wstrTriggerEtcActionType[] =
{
	L"TAT_ETC_NONE",
	L"TAT_ETC_PLAY_SOUND",
	L"TAT_ETC_DURATION_TIME",
	L"TAT_ETC_CAMERA",
	L"TAT_ETC_CAMERA_SHAKE",
	L"TAT_ETC_ACTIVE_LAYER",
};

wstring wstrTriggerMeshPlayType[] =
{
	L"WOSMPY_ONE",
	L"WOSMPY_ONE_WAIT",
	L"WOSMPY_LOOP",
	L"WOSMPY_PINGPONG",
	L"WOSMPY_OCCASIONAL_ONE",
	L"WOSMPY_OCCASIONAL_ONE_WAIT",
};

#if 0
// CPropertyGridButtonProperty ///////////////////////////////

IMPLEMENT_DYNAMIC(CPropertyGridButtonProperty, CMFCPropertyGridProperty)

#define AFX_PROP_HAS_BUTTON 0x0002
#define visualManager CMFCVisualManager::GetInstance()

CPropertyGridButtonProperty::CPropertyGridButtonProperty(const CString& strName, const CString& strFolderName, DWORD_PTR dwData, LPCTSTR lpszDescr) : CMFCPropertyGridProperty(strName, COleVariant((LPCTSTR)strFolderName), lpszDescr, dwData), m_bIsFolder(TRUE)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
	m_bInPlaceEdit = false;
	m_bAllowEdit = false;
	m_pWndInPlace = NULL;
}

CPropertyGridButtonProperty::CPropertyGridButtonProperty( const CString& strName, BOOL bOpenFileDialog, const CString& strFileName, LPCTSTR lpszDefExt, DWORD dwFileFlags, LPCTSTR lpszFilter, LPCTSTR lpszDescr, DWORD_PTR dwData) : CMFCPropertyGridProperty(strName, COleVariant((LPCTSTR)strFileName), lpszDescr, dwData), m_bOpenFileDialog(bOpenFileDialog), m_dwFileOpenFlags(dwFileFlags)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
	m_strDefExt = lpszDefExt == NULL ? _T("") : lpszDefExt;
	m_strFilter = lpszFilter == NULL ? _T("") : lpszFilter;

	m_bIsFolder  = FALSE;
}

CPropertyGridButtonProperty::~CPropertyGridButtonProperty()
{
}

void CPropertyGridButtonProperty::OnDrawValue(CDC* pDC, CRect rect)
{
	OnDrawButton( pDC, rect );
}

void CPropertyGridButtonProperty::OnDrawButton(CDC* pDC, CRect rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT_VALID(m_pWndList);

	CMFCToolBarComboBoxButton button;

	pDC->FillRect(rect, m_pWndList->DrawControlBarColors() ? &afxGlobalData.brBarFace : &afxGlobalData.brBtnFace);
	

	CString str = wstrButtonName.c_str();
	pDC->DrawText(str, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	CMFCVisualManager::AFX_BUTTON_STATE state = m_bButtonIsDown ? CMFCVisualManager::ButtonsIsPressed : CMFCVisualManager::ButtonsIsRegular;
	visualManager->OnDrawButtonBorder(pDC, &button, rect, state);

	if (m_bButtonIsFocused)
	{
		CRect rectFocus = rect;
		rectFocus.DeflateRect(2, 2);

		pDC->DrawFocusRect(rectFocus);
	}
}

void CPropertyGridButtonProperty::AdjustButtonRect()
{
	ASSERT_VALID(this);

	m_rectButton = m_Rect;
	m_rectButton.left = m_pWndList->GetPropertyColumnWidth() + 1;
	m_rectButton.top ++;
}

void CPropertyGridButtonProperty::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));


	m_bButtonIsDown = TRUE;
	Redraw();

	int iSelectedItem = GetData();
	if( iSelectedItem <= 0 )
	{
		if( m_pWndList != NULL )
		{
			CPropertyGridCtrl *pWnd = (CPropertyGridCtrl*)m_pWndList;
			if( pWnd != NULL && pWnd->m_pTriggerPropertiesWnd != NULL )
				pWnd->m_pTriggerPropertiesWnd->AddTrigger(iSelectedItem);
		}
	}

	m_bButtonIsDown = FALSE;
	Redraw();

	//m_pWndList->OnPropertyChanged(this);	
	//m_pWndList->SetFocus();
}

///////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNAMIC(CPropertyGridCtrl, CMFCPropertyGridCtrl)

BEGIN_MESSAGE_MAP(CPropertyGridCtrl, CMFCPropertyGridCtrl)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CPropertyGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{	
	CMFCPropertyGridCtrl::OnLButtonUp(nFlags, point);
 	AdjustLayout();
 
 	for(UINT i=0; i<m_vAddTriggerButtonId.size(); ++i)
 	{
 		CMFCPropertyGridProperty *pProp = FindItemByData( m_vAddTriggerButtonId[i] );
 		CPropertyGridButtonProperty *pButtonProp = dynamic_cast<CPropertyGridButtonProperty*>(pProp);
 		if( pButtonProp != NULL && pButtonProp->HasButton() == true)
 		{
 			pButtonProp->SetButtonDown(false);
 			pButtonProp->Redraw();
 		}
 	}
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

IMPLEMENT_DYNAMIC(CTriggerPropertiesWnd, CDockablePane)

CTriggerPropertiesWnd::CTriggerPropertiesWnd()
{
}

CTriggerPropertiesWnd::~CTriggerPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CTriggerPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE( AFX_WM_PROPERTY_CHANGED, OnPropertyChanged )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 메시지 처리기

void CTriggerPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);


	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;


	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CTriggerPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 콤보 상자를 만듭니다.
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, ID_TRIGGER_ID))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CTriggerPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CTriggerPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CTriggerPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CTriggerPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CTriggerPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CTriggerPropertiesWnd::InitWorld()
{
	if( g_pX2Game == NULL || g_pX2Game->GetWorld() == NULL || g_pX2Game->GetWorld()->GetLineMap() == NULL )
		return;

	//CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;	
	
	const vector<CX2WorldObjectPtr>&vecWorldObject = g_pX2Game->GetWorld()->GetWorldObjectList();
	for(UINT i=0; i<vecWorldObject.size(); ++i)
	{
		CX2WorldObject* pWorldObject = vecWorldObject[i].get();
		if( pWorldObject == NULL )
			continue;
		if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
		{
			CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)pWorldObject;
			pSkinMesh->ChangePlayAnim_LUA( pSkinMesh->m_wstrOriAnimName, (CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE)pSkinMesh->m_iOriPlayType );
		}
	}

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
		pLineData->bEnable = pLineData->bOriEnable;
		pLineData->m_bCanPass = pLineData->m_bOriCanPass;
		pLineData->m_fSpeed = pLineData->m_fOriSpeed;
		pLineData->m_iCameraIndex = pLineData->m_iOriCameraIndex;
		pLineData->m_iTeleportLineIndex = pLineData->m_iOriTeleportLineIndex;
		pLineData->m_vJumpSpeed = pLineData->m_vOriJumpSpeed;
	}

	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
	pTrigger->m_vecNowTrigger.clear();
	for(UINT i=0; i<pTrigger->m_vecTriggerSeg.size(); ++i)
	{
		// trigger
		CKTDGLineMap::TRIGGER_SEGMENT &triggerSegment = pTrigger->m_vecTriggerSeg[i];
		triggerSegment.m_iTriggerDataIndex = 0;
		for(UINT j=0; j<triggerSegment.m_vecTriggerData.size(); ++j)
		{
			CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSegment.m_vecTriggerData[j];
			triggerData.m_fElapsedTime = 0.f;
			for(UINT k=0; k<triggerData.m_vecTriggerLine.size(); ++k)
			{
				CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine = triggerData.m_vecTriggerLine[k];
				triggerLine.m_bDone = false;				
			}
			for(UINT k=0; k<triggerData.m_vecTriggerMesh.size(); ++k)
			{
				CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh = triggerData.m_vecTriggerMesh[k];
				triggerMesh.m_bDone = false;				
			}
			for(UINT k=0; k<triggerData.m_vecTriggerEtc.size(); ++k)
			{
				CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc = triggerData.m_vecTriggerEtc[k];
				triggerEtc.m_bDone = false;				
			}
		}
	}
}		

void CTriggerPropertiesWnd::OnProperties1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// InitTrigger
	InitWorld();
}

void CTriggerPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CTriggerPropertiesWnd::OnProperties2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// new trigger
	AfxMessageBox(L"Line Property", MB_OK);
}

void CTriggerPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CTriggerPropertiesWnd::InitPropList()
{		
	//m_wndPropList.SetOwnerClass(this);
	//m_wndPropList.InitPropertiesWnd();
	m_wndPropList.RemoveAll();
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	if( g_pX2Game == NULL || g_pX2Game->GetWorld() == NULL || g_pX2Game->GetWorld()->GetLineMap() == NULL )
		return;

	m_vAddTriggerButtonId.clear();

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();

	WCHAR wTemp[20];
	
	bool bTriggerloop = false;
	int iTriggerSeg = -1;
	int iTriggerData = -1;
	int iTriggerElem = -1;
	int iFlag = 0;
	
	for(UINT i=0; i<pTrigger->m_vecTriggerSeg.size(); ++i)
	{
		// trigger
		CKTDGLineMap::TRIGGER_SEGMENT triggerSegment = pTrigger->m_vecTriggerSeg[i];
		int iSegId = MakeTriggerID(false, i, -1, -1, 0);
		wsprintfW( wTemp, L"Trigger %d", i );		
		CMFCPropertyGridProperty* pTriggerGroupItem = new CMFCPropertyGridProperty(wTemp, iSegId);
		{
			int iSegLoopId = MakeTriggerID(true, i, -1, -1, 0);
			CMFCPropertyGridProperty *pTriggerData1 = new CMFCPropertyGridProperty(_T("Trigger Loop"), (_variant_t)(bool)triggerSegment.m_bLoop, _T("트리거 반복 여부를 지정한다."), iSegLoopId);			
			pTriggerGroupItem->AddSubItem( pTriggerData1 );			

			for(UINT j=0; j<triggerSegment.m_vecTriggerData.size(); ++j)
			{
				// trigger data
				int iDataId = MakeTriggerID(false, i, j, -1, 0);
				CKTDGLineMap::TRIGGER_DATA triggerData = triggerSegment.m_vecTriggerData[j];
				wsprintfW( wTemp, L"TriggerData %d", j );		
				CMFCPropertyGridProperty* pTriggerDataItem = new CMFCPropertyGridProperty(wTemp, iDataId);				
				{
					for(UINT iTLine=0; iTLine<triggerData.m_vecTriggerLine.size(); ++iTLine)
					{
						// Line Trigger
						int iTLineId = MakeTriggerID(false, i, j, iTLine, 1);
						CKTDGLineMap::TRIGGER_DATA_LINE triggerLine = triggerData.m_vecTriggerLine[iTLine];
						wsprintfW( wTemp, L"Line Trigger %d", iTLine );		
						CMFCPropertyGridProperty* pTriggerItem = new CMFCPropertyGridProperty(wTemp, iTLineId);
						{
							CMFCPropertyGridProperty *pTrigger = NULL;

							++iTLineId;
							pTrigger = new CMFCPropertyGridProperty(_T("Action Type"), (_variant_t)(WCHAR*)wstrTriggerLineActionType[triggerLine.m_iLineTriggerAction].c_str(), _T("line trigger의 행동을 지정합니다."), iTLineId );
							for(int k=0; k<ARRAY_SIZE(wstrTriggerLineActionType); ++k)
							{
								pTrigger->AddOption(wstrTriggerLineActionType[k].c_str());
							}
							pTrigger->AllowEdit(FALSE);
							pTriggerItem->AddSubItem(pTrigger);

							++iTLineId;
							pTrigger = new CMFCPropertyGridProperty(_T("Delay Time"), (_variant_t)(float)triggerLine.m_fTriggerTime, _T("트리거가 작동할 시간을 지정합니다."), iTLineId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTLineId;
							pTrigger = new CMFCPropertyGridProperty(_T("Line Id"), (_variant_t)(long)triggerLine.m_iTriggerLineIndex, _T("트리거가 작동시킬 라인을 지정합니다."), iTLineId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTLineId;
							pTrigger = new CMFCPropertyGridProperty(_T("LineSpeed"), (_variant_t)(float)triggerLine.m_fSpeed, _T("Line Speed를 지정합니다."), iTLineId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTLineId;
							pTrigger = new CMFCPropertyGridProperty(_T("JumpSpeed X"), (_variant_t)(float)triggerLine.m_vJumpSpeed.x, _T("Jump Speed X를 지정합니다."), iTLineId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTLineId;
							pTrigger = new CMFCPropertyGridProperty(_T("JumpSpeed Y"), (_variant_t)(float)triggerLine.m_vJumpSpeed.y, _T("Jump Speed Y를 지정합니다."), iTLineId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTLineId;
							pTrigger = new CMFCPropertyGridProperty(_T("Teleport/Camera ID"), (_variant_t)(long)triggerLine.m_iCamera, _T("action이 (TAT_LINE_CHANGE_TELEPORT, TAT_LINE_CHANGE_CAMERA)일 경우 각각 line teleport id, line camera id를 지정합니다. "), iTLineId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTLineId;
							pTrigger = new CMFCPropertyGridProperty(_T("Enable/CanPass"), (_variant_t)(bool)triggerLine.m_bEnable, _T("action이 (TAT_LINE_CHANGE_ENABLE, TAT_LINE_CHANGE_CANPASS)일 경우 각각 line Enable, line CanPass 여부를 지정합니다. "), iTLineId );
							pTriggerItem->AddSubItem(pTrigger);
						}						


						pTriggerDataItem->AddSubItem(pTriggerItem);
					}
					for(UINT iTMesh=0; iTMesh<triggerData.m_vecTriggerMesh.size(); ++iTMesh)
					{
						// Mesh Trigger
						int iTMeshId = MakeTriggerID(false, i, j, iTMesh, 2);
						CKTDGLineMap::TRIGGER_DATA_MESH triggerMesh = triggerData.m_vecTriggerMesh[iTMesh];
						wsprintfW( wTemp, L"Mesh Trigger %d", iTMesh );		
						CMFCPropertyGridProperty* pTriggerItem = new CMFCPropertyGridProperty(wTemp, iTMeshId);
						{
							CMFCPropertyGridProperty *pTrigger = NULL;

							++iTMeshId;
							pTrigger = new CMFCPropertyGridProperty(_T("Action Type"), (_variant_t)(WCHAR*)wstrTriggerMeshActionType[triggerMesh.m_iMeshTriggerAction].c_str(), _T("mesh trigger의 행동을 지정합니다."), iTMeshId );
							for(int k=0; k<ARRAY_SIZE(wstrTriggerMeshActionType); ++k)
							{
								pTrigger->AddOption(wstrTriggerMeshActionType[k].c_str());
							}
							pTrigger->AllowEdit(FALSE);
							pTriggerItem->AddSubItem(pTrigger);

							++iTMeshId;
							pTrigger = new CMFCPropertyGridProperty(_T("Delay Time"), (_variant_t)(float)triggerMesh.m_fTriggerTime, _T("트리거가 작동할 시간을 지정합니다."), iTMeshId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTMeshId;
							pTrigger = new CMFCPropertyGridProperty(_T("Mesh Name"), (_variant_t)(WCHAR*)triggerMesh.m_wstrMeshName.c_str(), _T("트리거가 작동할 메쉬 이름을 지정합니다."), iTMeshId );
							const vector<CX2WorldObjectPtr>&vecWorldObject = g_pX2Game->GetWorld()->GetWorldObjectList();
							std::vector<wstring> vecAnimList;
							for(UINT k=0; k<vecWorldObject.size(); ++k)
							{
								CX2WorldObject* pWorldObject = vecWorldObject[k].get();								
								if( pWorldObject != NULL && pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
								{
									CX2WorldObjectSkinMesh *pWorldSkinObject = (CX2WorldObjectSkinMesh*)pWorldObject;
									wstring wstrMeshName = pWorldSkinObject->GetXSkinMeshName();
									pTrigger->AddOption(wstrMeshName.c_str());
									
									// animation list를 뽑아낸다.
									CKTDGXSkinAnim *pSkinAnim = pWorldSkinObject->GetXSkinAnim();
									wstring wstrMeshName1 = triggerMesh.m_wstrMeshName;
									wstring wstrMeshName2 = pWorldSkinObject->GetXSkinMeshName();
									MakeUpperCase(wstrMeshName1);
									MakeUpperCase(wstrMeshName2);
									if( pSkinAnim != NULL && wstrMeshName1 == wstrMeshName2 )
									{
										pSkinAnim->GetAnimNameList(vecAnimList);
									}
								}
							}
							pTrigger->AllowEdit(FALSE);
							pTriggerItem->AddSubItem(pTrigger);
							
							++iTMeshId;
							pTrigger = new CMFCPropertyGridProperty(_T("AnimName"), (_variant_t)(WCHAR*)triggerMesh.m_wstrName.c_str(), _T("해당 mesh의 motion 이름을 지정합니다."), iTMeshId );
							for(UINT k=0; k<vecAnimList.size(); ++k)
							{
								pTrigger->AddOption( vecAnimList[k].c_str() );
							}
							pTrigger->AllowEdit(FALSE);
							pTriggerItem->AddSubItem(pTrigger);
							
							++iTMeshId;
							pTrigger = new CMFCPropertyGridProperty(_T("Play Type"), (_variant_t)(WCHAR*)wstrTriggerMeshPlayType[triggerMesh.m_iPlayType].c_str(), _T("해당 mesh의 Play Type을 지정합니다."), iTMeshId );
							for(int k=0; k<ARRAY_SIZE(wstrTriggerMeshPlayType); ++k)
							{
								pTrigger->AddOption( wstrTriggerMeshPlayType[k].c_str() );
							}
							pTrigger->AllowEdit(FALSE);
							pTriggerItem->AddSubItem(pTrigger);

							++iTMeshId;
							pTrigger = new CMFCPropertyGridProperty(_T("AnimSpeed"), (_variant_t)(float)triggerMesh.m_fAnimSpeed, _T("Animation Speed를 지정합니다."), iTMeshId );
							pTriggerItem->AddSubItem(pTrigger);
						}
						pTriggerDataItem->AddSubItem(pTriggerItem);
					}
					for(UINT iTEtc=0; iTEtc<triggerData.m_vecTriggerEtc.size(); ++iTEtc)
					{
						// Etc Trigger
						int iTEtcId = MakeTriggerID(false, i, j, iTEtc, 3);
						CKTDGLineMap::TRIGGER_DATA_ETC triggerEtc = triggerData.m_vecTriggerEtc[iTEtc];
						wsprintfW( wTemp, L"Etc Trigger %d", iTEtc );		
						CMFCPropertyGridProperty* pTriggerItem = new CMFCPropertyGridProperty(wTemp, iTEtcId);
						{
							CMFCPropertyGridProperty *pTrigger = NULL;

							++iTEtcId;
							pTrigger = new CMFCPropertyGridProperty(_T("Action Type"), (_variant_t)(WCHAR*)wstrTriggerEtcActionType[triggerEtc.m_iEtcTriggerAction].c_str(), _T("etc trigger의 행동을 지정합니다."), iTEtcId );
							for(int k=0; k<ARRAY_SIZE(wstrTriggerEtcActionType); ++k)
							{
								pTrigger->AddOption(wstrTriggerEtcActionType[k].c_str());
							}
							pTrigger->AllowEdit(FALSE);
							pTriggerItem->AddSubItem(pTrigger);

							++iTEtcId;
							pTrigger = new CMFCPropertyGridProperty(_T("Delay Time"), (_variant_t)(float)triggerEtc.m_fTriggerTime, _T("트리거가 작동할 시간을 지정합니다."), iTEtcId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTEtcId;
							pTrigger = new CMFCPropertyGridProperty(_T("Sound Name"), (_variant_t)(WCHAR*)triggerEtc.m_wstrActionName.c_str(), _T("TAT_ETC_PLAY_SOUND action일때 플레이할 사운드파일 이름을 지정합니다."), iTEtcId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTEtcId;
							pTrigger = new CMFCPropertyGridProperty(_T("Camera ID/Shake Gap/Layer ID"), (_variant_t)(long)triggerEtc.m_iDummyValue01, _T("각각 TAT_ETC_CAMERA, TAT_ETC_CAMERA_SHAKE, TAT_ETC_ACTIVE_LAYER에 대응되는 값을 지정합니다."), iTEtcId );
							pTriggerItem->AddSubItem(pTrigger);

							++iTEtcId;
							pTrigger = new CMFCPropertyGridProperty(_T("Camera Duration/Fade Time"), (_variant_t)(float)triggerEtc.m_fDummyValue01, _T("각각 camera duration time, layer fade time을 지정합니다."), iTEtcId );
							pTriggerItem->AddSubItem(pTrigger);
						}
						pTriggerDataItem->AddSubItem(pTriggerItem);
					}
				}

				pTriggerGroupItem->AddSubItem(pTriggerDataItem);

				// triggerLine 추가
				{
					int iaddLineTriggerId = MakeTriggerID(false, i, j, -1, 1, true);
					CMFCPropertyGridProperty *pAddTrigger = new CMFCPropertyGridProperty(_T("Add Line Trigger"), (_variant_t)(bool)false, _T("trigger를 추가합니다."), iaddLineTriggerId);					
					pTriggerDataItem->AddSubItem( pAddTrigger );					
					
					m_vAddTriggerButtonId.push_back(iaddLineTriggerId);
				}
				// triggerMesh 추가
				{
					int iaddMeshTriggerId = MakeTriggerID(false, i, j, -1, 2, true);
					CMFCPropertyGridProperty *pAddTrigger = new CMFCPropertyGridProperty(_T("Add Mesh Trigger"), (_variant_t)(bool)false, _T("trigger를 추가합니다."), iaddMeshTriggerId);
					pTriggerDataItem->AddSubItem( pAddTrigger );
					
					m_vAddTriggerButtonId.push_back(iaddMeshTriggerId);
				}
				// triggerEtc 추가
				{
					int iaddEtcTriggerId = MakeTriggerID(false, i, j, -1, 3, true);
					CMFCPropertyGridProperty *pAddTrigger = new CMFCPropertyGridProperty(_T("Add Etc Trigger"), (_variant_t)(bool)false, _T("trigger를 추가합니다."), iaddEtcTriggerId);
					pTriggerDataItem->AddSubItem( pAddTrigger );
					
					m_vAddTriggerButtonId.push_back(iaddEtcTriggerId);
				}
			}
		}

		m_wndPropList.AddProperty(pTriggerGroupItem);

		// triggerdata 추가
		{				
			int iAddTriggerDataId = MakeTriggerID(false, i, -1, -1, 0, true);
			CMFCPropertyGridProperty *pAddTrigger = new CMFCPropertyGridProperty(_T("Add Trigger Data"), (_variant_t)(bool)false, _T("trigger를 추가합니다."), iAddTriggerDataId);
			pTriggerGroupItem->AddSubItem( pAddTrigger );
			
			m_vAddTriggerButtonId.push_back(iAddTriggerDataId);
		}
	}

	// trigger 추가
	{
		int iaddTriggerId = MakeTriggerID(false, -1, -1, -1, 0, true);
		CMFCPropertyGridProperty *pAddTrigger = new CMFCPropertyGridProperty(_T("Add Trigger"), (_variant_t)(bool)false, _T("trigger를 추가합니다."), iaddTriggerId);
		m_wndPropList.AddProperty( pAddTrigger );
		
		m_vAddTriggerButtonId.push_back(iaddTriggerId);
	}
}

void CTriggerPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CTriggerPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();

	AfxMessageBox(lpszSection, MB_OK);
}

void CTriggerPropertiesWnd::SetPropListFont()
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

CMFCPropertyGridProperty* CTriggerPropertiesWnd::GetProp(int iData)
{
	return m_wndPropList.FindItemByData(iData);		
}

void CTriggerPropertiesWnd::AddTrigger(int iSelectedAddTrigger)
{
	if( iSelectedAddTrigger > 0 )
		return;

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();

	if( iSelectedAddTrigger == 0 )
	{
		// new trigger
		CKTDGLineMap::TRIGGER_SEGMENT triggerSegment;
		triggerSegment.m_iTriggerId = pTrigger->m_vecTriggerSeg.size();
		pTrigger->m_vecTriggerSeg.push_back(triggerSegment);

		InitPropList();
		return;
	}
	
	bool bLoop = false;
	int itriggerseg = -1;
	int itriggerdata = -1;
	int itriggerelem = -1;
	int iFlag = 0;
	int iElemIndex = 0;

	iElemIndex = GetTriggerID( iSelectedAddTrigger, bLoop, itriggerseg, itriggerdata, itriggerelem, iFlag );
		
	if( itriggerseg >= 0 && itriggerdata < 0 && iFlag <= 0 )
	{		
		// add triggerdata 
		CKTDGLineMap::TRIGGER_DATA triggerData;			
		pTrigger->m_vecTriggerSeg[itriggerseg].m_vecTriggerData.push_back(triggerData);
		InitPropList();
		return;
	}
	else if( itriggerseg >= 0 && itriggerdata >= 0 && iFlag > 0 )
	{
		// new trigger item
		switch( iFlag )
		{
		case 1:
			{
				// line
				CKTDGLineMap::TRIGGER_DATA_LINE triggerLine;
				pTrigger->m_vecTriggerSeg[itriggerseg].m_vecTriggerData[itriggerdata].m_vecTriggerLine.push_back(triggerLine);
				InitPropList();
			}
			break;
		case 2:
			{
				// mesh
				CKTDGLineMap::TRIGGER_DATA_MESH triggerMesh;
				pTrigger->m_vecTriggerSeg[itriggerseg].m_vecTriggerData[itriggerdata].m_vecTriggerMesh.push_back(triggerMesh);
				InitPropList();
			}
			break;
		case 3:
			{
				// etc
				CKTDGLineMap::TRIGGER_DATA_ETC triggerEtc;
				pTrigger->m_vecTriggerSeg[itriggerseg].m_vecTriggerData[itriggerdata].m_vecTriggerEtc.push_back(triggerEtc);
				InitPropList();
			}
			break;
		default:
			break;
		}
	}
}

// return :
// new TriggerSeg : 0
// new triggerData : triggerId * -1 (seg)
// new triggerElement : triggerId + Flag * -1 (seg, data)
int	CTriggerPropertiesWnd::MakeTriggerID(bool bLoop, int iTriggerSeg, int iTriggerData, int iTriggerElement, int Flag, bool bAddGroup)
{
	int iTriggerSegGroup = 100000;
	int iTriggerLoopGroup = 10000;
	int iTriggerDataGroup = 1000;
	int iTriggerElementGroup = 100;
	int iFlagGroup = 10;
	int iAddGroup = 1;

	// Flag --
	// Get
	// 1 : Line
	// 2 : Mesh
	// 3 : Etc

	if( bAddGroup == true )
		iAddGroup = -1;

	if( iTriggerSeg < 0 )
		return 0;
	
	int iTriggerId = iTriggerSegGroup * (iTriggerSeg + 1);	
	if( bLoop == true )
		iTriggerId += iTriggerLoopGroup;
	iTriggerId += (iTriggerDataGroup * (iTriggerData + 1));
	iTriggerId += (iTriggerElementGroup * (iTriggerElement + 1));
	iTriggerId += (Flag * iFlagGroup);

	return iTriggerId * iAddGroup;
}

int CTriggerPropertiesWnd::GetTriggerID(int iSelectedItem, bool &triggerLoop, int &triggerId, int &triggerDataId, int &triggerElementId, int &FlagId)
{
	int iTriggerSegGroup = 100000;
	int iTriggerLoop = 10000;
	int iTriggerDataGroup = 1000;
	int iTriggerElementGroup = 100;
	int iFlagGroup = 10;

	int iItem = iSelectedItem;
	if( iItem == 0 )
	{
		triggerId = -1;
		triggerDataId = -1;
		triggerElementId = -1;
		FlagId = 0;
		return 0;
	}

	if( iSelectedItem < 0 )
		iItem *= -1;

	triggerId = (iItem / iTriggerSegGroup) - 1;
	iItem = iItem % iTriggerSegGroup;
	if(iItem / iTriggerLoop > 0 )
		triggerLoop = true;
	else
		triggerLoop = false;
	iItem = iItem % iTriggerLoop;
	triggerDataId = (iItem / iTriggerDataGroup) - 1;
	iItem = iItem % iTriggerDataGroup;
	triggerElementId = (iItem / iTriggerElementGroup) - 1;
	iItem = iItem % iTriggerElementGroup;
	FlagId = iItem / iFlagGroup;
	iItem = iItem % iFlagGroup;

	if( FlagId <= 0 )
		return 0;

	return iItem;
}

LRESULT CTriggerPropertiesWnd::OnPropertyChanged( WPARAM wParam, LPARAM lParam )
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lParam;
	if( !pProp ) 
		return 0;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	int iSelectedItem = pProp->GetData();

	int iTrigger = -1;
	int iTriggerData = -1;
	int iTriggerElem = -1;
	int iFlag = 0;
	int iElemIndex = 0;
	bool bTriggerLoop = false;


	if( iSelectedItem <= 0 )
	{
		if( pProp->GetValue().boolVal == VARIANT_TRUE )
			AddTrigger(iSelectedItem);
		return 0;
	}


	iElemIndex = GetTriggerID(iSelectedItem, bTriggerLoop, iTrigger, iTriggerData, iTriggerElem, iFlag);
		
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();

	for(UINT i=0; i<pTrigger->m_vecTriggerSeg.size(); ++i)
	{
		CKTDGLineMap::TRIGGER_SEGMENT &triggerSegment = pTrigger->m_vecTriggerSeg[i];
		if( bTriggerLoop == true )
		{
			triggerSegment.m_bLoop = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false ;
			break;
		}

		if( triggerSegment.m_iTriggerId == iTrigger && iTriggerData >= 0 && iTriggerData < (int)triggerSegment.m_vecTriggerData.size())
		{
			CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSegment.m_vecTriggerData[iTriggerData];			
			
			if( iTriggerElem >= 0 && iFlag > 0 )
			{
				switch( iFlag )
				{
				case 1:
					{
						// line
						CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine = triggerData.m_vecTriggerLine[iTriggerElem];
						switch(iElemIndex)
						{
						case 1:
							{
								wstring wstrActionType = (WCHAR*)pProp->GetValue().bstrVal;
								for(int j=0; j<ARRAY_SIZE(wstrTriggerLineActionType); ++j)
								{
									if( wstrActionType.compare(wstrTriggerLineActionType[j]) == 0 )
									{
										triggerLine.m_iLineTriggerAction = (CKTDGLineMap::TRIGGER_LINE_ACTION_TYPE)j;
										break;
									}
								}
							}
							break;
						case 2:
							triggerLine.m_fTriggerTime = pProp->GetValue().fltVal;
							break;
						case 3:
							triggerLine.m_iTriggerLineIndex = pProp->GetValue().intVal;
							break;
						case 4:
							triggerLine.m_fSpeed = pProp->GetValue().fltVal;
							break;
						case 5:
							triggerLine.m_vJumpSpeed.x = pProp->GetValue().fltVal;
							break;
						case 6:
							triggerLine.m_vJumpSpeed.y = pProp->GetValue().fltVal;
							break;
						case 7:
							triggerLine.m_iCamera = pProp->GetValue().intVal;
							break;
						case 8:
							triggerLine.m_bEnable = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
							break;
						default:
							break;
						}
					}
					break;
				case 2:
					{
						// mesh
						CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh = triggerData.m_vecTriggerMesh[iTriggerElem];
						switch(iElemIndex)
						{
						case 1:
							{
								wstring wstrActionType = (WCHAR*)pProp->GetValue().bstrVal;
								for(int j=0; j<ARRAY_SIZE(wstrTriggerMeshActionType); ++j)
								{
									if( wstrActionType.compare(wstrTriggerMeshActionType[j]) == 0 )
									{
										triggerMesh.m_iMeshTriggerAction = (CKTDGLineMap::TRIGGER_MESH_ACTION_TYPE)j;
										break;
									}
								}
							}
							break;
						case 2:
							triggerMesh.m_fTriggerTime = pProp->GetValue().fltVal;
							break;
						case 3:
							{
								triggerMesh.m_wstrMeshName =(WCHAR*)pProp->GetValue().bstrVal;

								const vector<CX2WorldObjectPtr>&vecWorldObject = g_pX2Game->GetWorld()->GetWorldObjectList();
								std::vector<wstring> vecAnimList;
								for(UINT k=0; k<vecWorldObject.size(); ++k)
								{
									CX2WorldObject* pWorldObject = vecWorldObject[k].get();								
									if( pWorldObject != NULL && pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
									{
										CX2WorldObjectSkinMesh *pWorldSkinObject = (CX2WorldObjectSkinMesh*)pWorldObject;
										wstring wstrMeshName = pWorldSkinObject->GetXSkinMeshName();

										// animation list를 뽑아낸다.
										CKTDGXSkinAnim *pSkinAnim = pWorldSkinObject->GetXSkinAnim();
										wstring wstrMeshName1 = triggerMesh.m_wstrMeshName;
										wstring wstrMeshName2 = pWorldSkinObject->GetXSkinMeshName();
										MakeUpperCase(wstrMeshName1);
										MakeUpperCase(wstrMeshName2);
										if( pSkinAnim != NULL && wstrMeshName1 == wstrMeshName2 )
										{
											pSkinAnim->GetAnimNameList(vecAnimList);
											break;
										}
									}
								}

								CMFCPropertyGridProperty *pAnimProp = GetProp( pProp->GetData() + 1 );
								if( vecAnimList.size() >= 1 )
								{
									triggerMesh.m_wstrName = vecAnimList[0];							
									if( pAnimProp != NULL )
									{
										pAnimProp->RemoveAllOptions();
										for(UINT k=0; k<vecAnimList.size(); ++k)
											pAnimProp->AddOption(vecAnimList[k].c_str());
										pAnimProp->SetValue( (_variant_t)(WCHAR*)triggerMesh.m_wstrName.c_str() );
									}
								}
								else
								{
									pAnimProp->RemoveAllOptions();
									pAnimProp->SetValue( (_variant_t)(WCHAR*)L"" );
								}
							}					
							break;
						case 4:
							triggerMesh.m_wstrName = (WCHAR*)pProp->GetValue().bstrVal;
							break;
						case 5:
							{
								wstring wstrActionType = (WCHAR*)pProp->GetValue().bstrVal;
								for(int j=0; j<ARRAY_SIZE(wstrTriggerMeshPlayType); ++j)
								{
									if( wstrActionType.compare(wstrTriggerMeshPlayType[j]) == 0 )
									{
										triggerMesh.m_iPlayType = j;
										break;
									}
								}
							}
							break;
						case 6:
							triggerMesh.m_fAnimSpeed = pProp->GetValue().fltVal;
							break;
						default:
							break;
						}
					}
					break;
				case 3:
					{
						// etc
						CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc = triggerData.m_vecTriggerEtc[iTriggerElem];
						switch(iElemIndex)
						{
						case 1:
							{
								wstring wstrActionType = (WCHAR*)pProp->GetValue().bstrVal;
								for(int j=0; j<ARRAY_SIZE(wstrTriggerEtcActionType); ++j)
								{
									if( wstrActionType.compare(wstrTriggerEtcActionType[j]) == 0 )
									{
										triggerEtc.m_iEtcTriggerAction = (CKTDGLineMap::TRIGGER_ETC_ACTION_TYPE)j;
										break;
									}
								}
							}
							break;
						case 2:
							triggerEtc.m_fTriggerTime = pProp->GetValue().fltVal;
							break;
						case 3:
							triggerEtc.m_wstrActionName =(WCHAR*)pProp->GetValue().bstrVal;
							break;
						case 4:
							triggerEtc.m_iDummyValue01 = pProp->GetValue().intVal;
							break;
						case 5:
							triggerEtc.m_fDummyValue01 = pProp->GetValue().fltVal;
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
		}			
	}
		
	return 0;
}

void CTriggerPropertiesWnd::DeleteTrigger(int triggerId, int triggerDataId, int triggerElementId, int FlagId)
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();

	CKTDGLineMap::TRIGGER_SEGMENT &triggerSegment = pTrigger->m_vecTriggerSeg[triggerId];
		
	if( triggerDataId < 0 || triggerSegment.m_vecTriggerData.size() == 0 )
	{
		// trigger 삭제 
		triggerSegment.m_vecTriggerData.clear();
		pTrigger->m_vecTriggerSeg.erase(pTrigger->m_vecTriggerSeg.begin() + triggerId);

		for(UINT i=0; i<pTrigger->m_vecTriggerSeg.size(); ++i)
		{
			CKTDGLineMap::TRIGGER_SEGMENT &triggerSegment = pTrigger->m_vecTriggerSeg[triggerId];
			triggerSegment.m_iTriggerId = i;
		}
	}
	else
	{
		CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSegment.m_vecTriggerData[triggerDataId];
		if( triggerElementId < 0 || FlagId <= 0 )
		{
			// trigger data 삭제
			triggerData.m_vecTriggerLine.clear();
			triggerData.m_vecTriggerMesh.clear();
			triggerData.m_vecTriggerEtc.clear();

			triggerSegment.m_vecTriggerData.erase(triggerSegment.m_vecTriggerData.begin() + triggerDataId);				
		}
		else if( FlagId >= 1 && triggerElementId >= 0 )
		{
			switch( FlagId )
			{
			case 1:
				{
					// line
					triggerData.m_vecTriggerLine.erase(triggerData.m_vecTriggerLine.begin() + triggerElementId);
				}
				break;
			case 2:
				{
					// mesh
					triggerData.m_vecTriggerMesh.erase(triggerData.m_vecTriggerMesh.begin() + triggerElementId);
				}
				break;
			case 3:
				{
					// etc
					triggerData.m_vecTriggerEtc.erase(triggerData.m_vecTriggerEtc.begin() + triggerElementId);
				}
				break;
			default:
				break;
			}
		}
	}
}

BOOL CTriggerPropertiesWnd::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
		case VK_DELETE:
			{
				CMFCPropertyGridProperty *pProp = m_wndPropList.GetCurSel();
				if( pProp != NULL )
				{
					bool bGorup = false;
					int iSelectData = pProp->GetData();
					
					if( iSelectData < 0 )
						break;

					//m_wndPropList.DeleteProperty(pProp);
					int iTrigger = -1;
					int iTriggerData = -1;
					int iTriggerElem = -1;
					int iFlag = 0;
					int iElemIndex = 0;
					bool bLoop = false;

					iElemIndex = GetTriggerID(iSelectData, bLoop, iTrigger, iTriggerData,iTriggerElem, iFlag);
					if( iElemIndex == 0 )
					{
						DeleteTrigger(iTrigger, iTriggerData, iTriggerElem, iFlag);
						InitPropList();
					}					
				}
			}
			return TRUE;
		default:
			break;
		}
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}
