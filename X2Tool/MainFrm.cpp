/**
	@file	MainFrm.cpp
	@author	Wonpok
	@brief	CMainFrame 클래스의 구현
		
	@date	JHKang on 2013/02/19,
			- 기능 파악
			- 수정 작업( 주석처리, 블럭 나누기, 가로 넓이 지정 )
*/

#include "stdafx.h"
#include "X2Tool.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

#pragma region 전역 변수
const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	//	ID_USER_STRING1,
	// 	ID_INDICATOR_CAPS,
	// 	ID_INDICATOR_NUM,
	// 	ID_INDICATOR_SCRL,
};
#pragma endregion iMaxUserToolbars, uiFirstUserToolBarId, uiLastUserToolBarId, indicators

#pragma region MESSAGE_MAP
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_EDIT_LINEMAP, &CMainFrame::OnEditLinemap)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LINEMAP, &CMainFrame::OnUpdateEditLinemap)
	ON_COMMAND(ID_EDIT_WORLD, &CMainFrame::OnEditWorld)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WORLD, &CMainFrame::OnUpdateEditWorld)
	ON_COMMAND(ID_VIEW_LINE, &CMainFrame::OnViewLine)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE, &CMainFrame::OnUpdateViewLine)
	ON_COMMAND(ID_VIEW_LINEINFO, &CMainFrame::OnViewLineInfo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINEINFO, &CMainFrame::OnUpdateViewLineInfo)
	ON_COMMAND(ID_VIEW_WORLD, &CMainFrame::OnViewWorld)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORLD, &CMainFrame::OnUpdateViewWorld)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_WORLDOBJECT_MOVE, &CMainFrame::OnWorldObjectMove)
	ON_COMMAND(ID_WORLDOBJECT_ROTATION, &CMainFrame::OnWorldobjectRotation)
	ON_COMMAND(ID_WORLDOBJECT_SCALE, &CMainFrame::OnWorldobjectScale)
	ON_COMMAND(ID_VIEW_WORLDOBJECT, &CMainFrame::OnViewWorldObject)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORLDOBJECT, &CMainFrame::OnUpdateViewWorldobject)
	ON_COMMAND(ID_EDIT_DUNGEON, &CMainFrame::OnEditDungeon)
	ON_COMMAND(ID_VIEW_LINE_NUMBER, &CMainFrame::OnViewLineNumber)
	ON_COMMAND(ID_VIEW_LINE_SET_NUMBER, &CMainFrame::OnViewLineSetNumber)
	ON_COMMAND(ID_VIEW_LINE_CAMERA_NUMBER, &CMainFrame::OnViewLineCameraNumber)
	ON_COMMAND(ID_VIEW_LINE_TRIGGER_NUMBER, &CMainFrame::OnViewLineTriggerNumber)
	ON_COMMAND(ID_VIEW_WATER_INFO, &CMainFrame::OnViewWaterInfo)
	ON_COMMAND(ID_TRIGGER_BCK, &CMainFrame::OnTriggerBck)
END_MESSAGE_MAP()
#pragma endregion 메시지 맵

#pragma region CMainFrame
CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}
#pragma endregion 생성자, 기본적인 Frame 형태를 지정(Visual Studio 2008 Frame)

#pragma region ~CMainFrame
CMainFrame::~CMainFrame()
{
}
#pragma endregion Empty 소멸자

#pragma region OnCreate
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CFrameWndEx::OnCreate(lpCreateStruct) == -1 )
		return -1;

	BOOL bNameValid;
	
	OnApplicationLook(theApp.m_nAppLook);	// 보관된 값에 따라 비주얼 관리자 및 스타일을 설정합니다.

	#pragma region m_wndMenuBar
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("메뉴 모음을 만들지 못했습니다.\n");
		return -1;
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
	#pragma endregion 메뉴바 생성

	#pragma region m_wndToolBar
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME) )
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;
	}

	m_wndToolBar.SetSizes( CSize(24, 38), CSize(24, 38) );
	m_wndToolBar.SetHeight( 38 );

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 사용자 정의 도구 모음 작업을 허용합니다.
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	#pragma endregion 툴바 생성

	#pragma region m_wndStatusBar
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	#pragma endregion 상태 표시줄 생성

	#pragma region Docking
	// TODO: 도구 모음 및 메뉴 모음을 도킹할 수 없게 하려면 이 다섯 줄을 삭제하십시오.
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 탐색 창이 왼쪽에 만들어지므로 일시적으로 왼쪽에 도킹을 비활성화합니다.
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);
	#pragma endregion 속성

	#pragma region 추가 생성된 다이얼로그 
	// 메뉴 항목 이미지를 로드합니다(표준 도구 모음에 없음).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);
	
	// 일반 다이얼로그 초기화한다.
	m_DlgLineBaseEditor.Create(IDD_DIALOG_EDIT_LINE, this);	
	m_DlgLineBaseEditor.SetWindowPos( &wndTop, 1000, 200, 1, 1, SWP_NOMOVE | SWP_NOSIZE );
	
	m_DlgWorldEditor.Create(IDD_DIALOG_EDIT_WORLD, this);
	m_DlgWorldEditor.SetWindowPos( &wndTop, 1000, 200, 1, 2, SWP_NOMOVE | SWP_NOSIZE );
	
	m_DlgDungeonEditor.Create(IDD_DIALOG_DUNGEON_EDITOR, this);
	m_DlgDungeonEditor.SetWindowPos( &wndTop, 1000, 200, 1, 3, SWP_NOMOVE | SWP_NOSIZE );
	m_DlgDungeonEditor.ShowWindow(SW_SHOW);

	m_DlgSelectUnit.Create(IDD_DIALOG_SELECT_UNIT, this);
	m_DlgSelectUnit.SetWindowPos( &wndTop, 1000, 200, 1, 3, SWP_NOMOVE | SWP_NOSIZE );
	m_DlgSelectUnit.ShowWindow(SW_HIDE);

	m_DlgTriggerEdit.Create( IDD_DIALOG_TRIGGER, this );
	m_DlgTriggerEdit.SetWindowPos( &wndTop, 1000, 200, 1, 3, SWP_NOMOVE | SWP_NOSIZE );

	CreateDockingWindowAfter();
	
#if 0
	// 도킹 창을 만듭니다.
	if (!CreateDockingWindows())
	{
		TRACE0("도킹 창을 만들지 못했습니다.\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
#endif

	#pragma endregion 라인 에디터, 월드 에디터, 던전 에디터, 유닛 선택창 생성
	
	// 도구 모음 및 도킹 창 메뉴 바꾸기를 활성화합니다.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 빠른(<Alt> 키를 누른 채 끌기) 도구 모음 사용자 지정을 활성화합니다.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 사용자 정의 도구 모음 이미지를 로드합니다.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	#pragma region 최근 사용한 명령
	// 메뉴 개인 설정을 활성화합니다(가장 최근에 사용한 명령).
	// TODO: 사용자의 기본 명령을 정의하여 각 풀다운 메뉴에 하나 이상의 기본 명령을 포함시킵니다.

	/*CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
 	lstBasicCommands.AddTail(ID_APP_ABOUT);
 	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
 	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
 	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
 	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
 	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
 	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);
	lstBasicCommands.AddTail(ID_EDIT_LINEMAP);
	lstBasicCommands.AddTail(ID_EDIT_WORLD);
	lstBasicCommands.AddTail(ID_VIEW_LINEINFO);
	lstBasicCommands.AddTail(ID_VIEW_LINE);
	lstBasicCommands.AddTail(ID_VIEW_WORLD);
	lstBasicCommands.AddTail(ID_VIEW_WORLDOBJECT);
	
	CMFCToolBar::SetBasicCommands(lstBasicCommands);*/
	#pragma endregion 불편한 기능이라 제거

	/* JHKang / 2013.2.19 / 이름 교체함
	   X2GameTool --> Elsword Map Tool
	*/
	this->SetWindowTextW(L"Elsword Map Tool");
	return 0;
}
#pragma endregion 기본적인 UI 생성

#pragma region PreCreateWindow
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}
#pragma endregion 전 생성

#pragma region CreateDockingWindows
BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	#pragma region m_wndClassView
	CString strClassView = L"던전 리스트";
 	bNameValid = strClassView.LoadString( IDS_CLASS_VIEW );
 	ASSERT( bNameValid );
 	if ( !m_wndClassView.Create( strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW,
							    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI ) )
 	{
 		TRACE0( "클래스 뷰 창을 만들지 못했습니다.\n" );
 		return FALSE;
 	}
	#pragma endregion 클래스 뷰 생성
	
	#pragma region m_wndProperties
	CString strPropertiesWnd =  L"속성";
 	bNameValid = strPropertiesWnd.LoadString( IDS_PROPERTIES_WND );
 	ASSERT( bNameValid );
	if ( !m_wndProperties.Create( strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND,
								 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "속성 창을 만들지 못했습니다.\n" );
		return FALSE;
	}
	#pragma endregion 속성 창 생성

	#pragma region m_wndTriggerProperties
	CString strTriggerPropertiesWnd =  L"트리거 속성";
	bNameValid = strPropertiesWnd.LoadString( IDS_PROPERTIES_WND );
	ASSERT( bNameValid );
	if ( !m_wndTriggerProperties.Create( strTriggerPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, 4,
										 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "트리거 속성 창을 만들지 못했습니다.\n" );
		return FALSE;
	}
	#pragma endregion 트리거 속성 창 생성
	
	#pragma region m_wndDungeonInfo
	if ( !m_wndDungeonInfo.CreateEx( NULL, _T("던전 정보"), this, CRect(0,0,100,100), true, 1,
									 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "던전 정보 창을 만들지 못했습니다.\n" );
		return FALSE;
	}
	
	m_wndDungeonInfo.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndDungeonInfo );
	#pragma endregion 던전 정보 창 생성

	#pragma region m_wndCameraInfo
	if ( !m_wndCameraInfo.CreateEx( NULL, _T("카메라"), this, CRect(0,0,100,100), true, 2,
									WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "카메라 정보 창을 만들지 못했습니다.\n" );
		return FALSE;
	}

	m_wndCameraInfo.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndCameraInfo);
	#pragma endregion 카메라 정보 창 생성

	#pragma region m_wndStartPosition
	if ( !m_wndStartPosition.CreateEx( NULL, _T("시작 위치"), this, CRect(0,0,100,100), true, 3,
								 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "시작 위치 정보 창을 만들지 못했습니다.\n" );
		return FALSE;
	}

	m_wndStartPosition.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndStartPosition);
	#pragma endregion 시작 위치 정보 창 생성
	
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}
#pragma endregion 도킹 윈도우 생성

#pragma region SetDockingWindowIcons
void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hTriggerPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTriggerProperties.SetIcon(hTriggerPropertiesBarIcon, FALSE);
}
#pragma endregion 도킹 윈도우 아이콘 설정

#pragma region AssertValid, Dump
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG
#pragma endregion CMainFrame 진단


// CMainFrame 메시지 처리기

#pragma region OnViewCustomize
void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}
#pragma endregion 사용자 정의 툴바

#pragma region OnToolbarCreateNew
LRESULT CMainFrame::OnToolbarCreateNew( WPARAM wp,LPARAM lp )
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}
#pragma endregion 툴바 생성

#pragma region OnApplicationLook
void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}
#pragma endregion 스킨 변경

#pragma region OnUpdateApplicationLook
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
#pragma endregion 스킨 갱신

#pragma region LoadFrame
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// 모든 사용자 도구 모음에 사용자 지정 단추를 활성화합니다.
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}
#pragma endregion 사용자 지정 단추 부르기

#pragma region OnSettingChange
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}
#pragma endregion 설정 변경 시 발생

#pragma region CreateDockingWindowAfter
void CMainFrame::CreateDockingWindowAfter()
{
	// 도킹 창을 만듭니다.
	if (!CreateDockingWindows())
	{
		TRACE0("도킹 창을 만들지 못했습니다.\n");
		return;
	}
		
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	CDockablePane* pTabbedBar = NULL;
	DockPane(&m_wndClassView);
	
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	m_wndTriggerProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndTriggerProperties);
}
#pragma endregion 도킹 창 활성화

#pragma region OnResetMain
void CMainFrame::OnResetMain( bool bEditLine, bool bEditWorld, bool bViewLine, bool bViewWorld, bool bEditTrigger )
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HMENU hMenu = GetMenuBar()->GetHMenu();

	UINT editLine	= MF_BYCOMMAND | MF_UNCHECKED;
	UINT editWorld	= MF_BYCOMMAND | MF_UNCHECKED;
	UINT viewLine	= MF_BYCOMMAND | MF_UNCHECKED;
	UINT viewWorld	= MF_BYCOMMAND | MF_UNCHECKED;
	UINT editTrigger = MF_BYCOMMAND | MF_UNCHECKED;

	if( bEditLine )
		editLine	= MF_BYCOMMAND | MF_CHECKED;

	if( bEditWorld )
		editWorld	= MF_BYCOMMAND | MF_CHECKED;

	if( bViewLine )
		viewLine	= MF_BYCOMMAND | MF_CHECKED;

	if( bViewWorld )
		viewWorld	= MF_BYCOMMAND | MF_CHECKED;

	if ( bEditTrigger )
		editTrigger = MF_BYCOMMAND | MF_CHECKED;
	
	::CheckMenuItem(hMenu, ID_EDIT_LINEMAP, editLine);
	::CheckMenuItem(hMenu, ID_EDIT_WORLD, editWorld);
	::CheckMenuItem(hMenu, ID_VIEW_LINE, viewLine);
	::CheckMenuItem(hMenu, ID_VIEW_WORLD, viewWorld);
	::CheckMenuItem( hMenu, ID_TRIGGER_BCK, editTrigger );

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);

	if( bEditLine == false )
		m_DlgLineBaseEditor.ShowWindow(SW_HIDE);
	else
		m_DlgLineBaseEditor.ShowWindow(SW_SHOW);

	if ( bEditTrigger == false )
		m_DlgTriggerEdit.ShowWindow( SW_HIDE );
	else
		m_DlgTriggerEdit.ShowWindow( SW_SHOW );

	if( bEditWorld == false )
	{
		m_DlgWorldEditor.ShowWindow(SW_HIDE);
	}
	else
	{
		m_DlgWorldEditor.ShowWindow(SW_SHOW);
		m_DlgWorldEditor.ResetWorldEditor();
	}

	if ( m_DlgDungeonEditor )
		m_DlgDungeonEditor.ShowWindow(SW_SHOW);
}
#pragma endregion Main 갱신 시 처리할 것들

#pragma region OnEditLinemap
void CMainFrame::OnEditLinemap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND);
	bool bEdit = false;
	if( iChecked & MF_CHECKED )
	{
		::CheckMenuItem(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = false;
	}
	else
	{	
		::CheckMenuItem(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND | MF_CHECKED);
		::CheckMenuItem(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = true;
	}
		
	bool bVertexEdit = true;
	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;		
		pMain->m_bEditLine = bEdit;
		pMain->m_bEditWorld = false;
		pMain->SetFreeCamera();
		bVertexEdit = pMain->m_bEditLineVertex;
		
		CKTDGLineMap *pLinemap = g_pX2Game->GetWorld()->GetLineMap();
		for(int i=0; i<pLinemap->GetNumLineData(); ++i)
		{
			CKTDGLineMap::LineData *pLineData = pLinemap->AccessLineData(i);
			pLineData->m_bOver = false;
			pLineData->m_bSelected = false;
			pLineData->bEnable = true;
		}
	}

	if( bEdit == true )
	{
		m_DlgLineBaseEditor.ShowWindow(SW_SHOW);
		if( bVertexEdit == true)
			m_wndProperties.UpdateProp(1);
		else
			m_wndProperties.UpdateProp(2);

		m_DlgWorldEditor.ShowWindow(SW_HIDE);
	}
	else
	{
		m_DlgLineBaseEditor.ShowWindow(SW_HIDE);
		m_wndProperties.UpdateProp(0);
	}
}
#pragma endregion 라인맵 에디터 활성화

#pragma region OnUpdateEditLinemap
void CMainFrame::OnUpdateEditLinemap(CCmdUI *pCmdUI)
{
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND);
	int iCheck = 1;

	if( iChecked & MF_CHECKED )		
		iCheck = 1;
	else
		iCheck = 0;

	pCmdUI->SetCheck(iCheck);
	//if( ::IsWindow(pCmdUI->m_pOther->GetSafeHwnd()) )
	//	pCmdUI->m_pOther->SendMessage(TB_CHECKBUTTON, pCmdUI->m_nID, MAKELONG(iCheck,0));
}
#pragma endregion 라인맵 에디터 갱신 시

#pragma region OnEditWorld
void CMainFrame::OnEditWorld()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bEdit = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND | MF_CHECKED);
		::CheckMenuItem(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = true;
	}
	
	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bEditWorld = bEdit;
		pMain->m_bEditLine = false;
		pMain->SetFreeCamera();
	}

	if( bEdit == true )
	{
		m_DlgWorldEditor.ShowWindow(SW_SHOW);
		m_DlgWorldEditor.ResetWorldEditor();
		m_wndProperties.UpdateProp(3);

		m_DlgLineBaseEditor.ShowWindow(SW_HIDE);
		
	}
	else
	{
		m_DlgWorldEditor.ShowWindow(SW_HIDE);
		m_wndProperties.UpdateProp(0);
	}
}
#pragma endregion 월드 에디터 활성화

#pragma region OnUpdateEditWorld
void CMainFrame::OnUpdateEditWorld(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND);
	
	int iCheck = 1;
	if( iChecked & MF_CHECKED )		
		iCheck = 1;
	else
		iCheck = 0;

	pCmdUI->SetCheck(iCheck);
}
#pragma endregion 월드 에디터 갱신

#pragma region OnViewLine
void CMainFrame::OnViewLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bViewLine = bView;
	}
}
#pragma endregion 라인뷰 활성화

#pragma region OnUpdateViewLine
void CMainFrame::OnUpdateViewLine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE, MF_BYCOMMAND);
	int iCheck = 1;
	if( iChecked & MF_CHECKED )		
	{	
		iCheck = 1;
	}
	else
	{
		iCheck = 0;
	}

	pCmdUI->SetCheck(iCheck);
}
#pragma endregion 라인뷰 갱신

#pragma region OnViewLineInfo
void CMainFrame::OnViewLineInfo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINEINFO, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem( hMenu, ID_VIEW_LINEINFO, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_CAMERA_NUMBER, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND | MF_UNCHECKED );
		bView = false;
	}
	else
	{
		::CheckMenuItem( hMenu, ID_VIEW_LINEINFO, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_CAMERA_NUMBER, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND | MF_CHECKED );
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineInfo = bView;
		pMain->m_bShowLineNumber = bView;
		pMain->m_bShowLineSetNumber = bView;
		pMain->m_bShowLineCameraNumber = bView;
		pMain->m_bShowLineTriggerNumber = bView;
		pMain->m_bShowInnerWaterInfo = bView;
	}
}
#pragma endregion 라인뷰 정보 활성화

#pragma region OnUpdateViewLineInfo
void CMainFrame::OnUpdateViewLineInfo(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINEINFO, MF_BYCOMMAND);
	int iCheck = 1;
	if( iChecked & MF_CHECKED )		
	{	
		iCheck = 1;
	}
	else
	{
		iCheck = 0;
	}

	pCmdUI->SetCheck(iCheck);
}
#pragma endregion 라인뷰 정보 갱신

#pragma region OnViewWorld
void CMainFrame::OnViewWorld()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_WORLD, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_WORLD, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_WORLD, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bViewWorld = bView;
	}
}
#pragma endregion 월드뷰 활성화

#pragma region OnUpdateViewWorld
void CMainFrame::OnUpdateViewWorld(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_WORLD, MF_BYCOMMAND);
	int iCheck = 1;
	if( iChecked & MF_CHECKED )		
	{	
		iCheck = 1;
	}
	else
	{
		iCheck = 0;
	}

	pCmdUI->SetCheck(iCheck);
}
#pragma endregion 월드뷰 갱신

#pragma region OnDropFiles
void CMainFrame::OnDropFiles( HDROP hDropInfo )
{
	WCHAR wBuf[MAX_PATH];
	UINT uDropCount = ::DragQueryFile( hDropInfo, 0xffffffff, wBuf, 1024 );

	for( UINT i=0; i<uDropCount; ++i )
	{
		::DragQueryFile( hDropInfo, i, wBuf, MAX_PATH );

		// drop 된 파일 처리
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{			
			wstring wFile = wBuf;
			MakeUpperCase(wBuf);
			if( StrStrW(wBuf, L"LINEMAP") != NULL )
			{
				// linemap script
				pMain->LoadLineMap( wBuf );
			}
			else if( StrStrW(wBuf, L".X") != NULL || StrStrW(wBuf, L".Y") != NULL )
			{
				// world object
			}
			else
			{
				// world script
				pMain->LoadWorld( wBuf );
			}
		}
	}

	::DragFinish( hDropInfo );
}
#pragma endregion 드롭된 파일일 들어올 시

#pragma region OnWorldObjectMove
void CMainFrame::OnWorldObjectMove()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
				
		pMain->SetRotationWorldObject(false);
		pMain->SetScaleWorldObject(false);
		pMain->SetMoveWorldObject(true);
	}
}
#pragma endregion 월드 객체 이동 시

#pragma region OnWorldobjectRotation
void CMainFrame::OnWorldobjectRotation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{	
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

		pMain->SetMoveWorldObject(false);
		pMain->SetScaleWorldObject(false);
		pMain->SetRotationWorldObject(true);
	}
}
#pragma endregion 월드 오브젝트 회전 시

#pragma region OnWorldobjectScale
void CMainFrame::OnWorldobjectScale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

		pMain->SetMoveWorldObject(false);
		pMain->SetRotationWorldObject(false);
		pMain->SetScaleWorldObject(true);
	}
}
#pragma endregion 월드 오브젝트 크기 변경 시

#pragma region OnViewWorldobject
void CMainFrame::OnViewWorldObject()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bViewSelectedWorldobject = !pMain->m_bViewSelectedWorldobject;
		bool bView = pMain->m_bViewSelectedWorldobject;
		HMENU hMenu = GetMenuBar()->GetHMenu();
		
		if( bView == false )
		{		
			::CheckMenuItem(hMenu, ID_VIEW_WORLDOBJECT, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			::CheckMenuItem(hMenu, ID_VIEW_WORLDOBJECT, MF_BYCOMMAND | MF_CHECKED);
		}

		m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);

		m_wndProperties.SetPropValue( PI_EW_WORLD_SELECTED, (_variant_t)(bool)bView );
	}
}
#pragma endregion 월드 오브젝트 뷰 활성화

#pragma region OnUpdateViewWorldobject
void CMainFrame::OnUpdateViewWorldobject(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		bool bCheck = pMain->m_bViewSelectedWorldobject;
		int iCheck = 0;
		if( bCheck )
			iCheck = 1;
		else
			iCheck = 0;

		pCmdUI->SetCheck(iCheck);
	}
}
#pragma endregion 월드 오브젝트 뷰 갱신

#pragma region OnViewWorldObjectSelected
void CMainFrame::OnViewWorldObjectSelected()
{
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;		
		bool bView = pMain->m_bViewSelectedWorldobject;
		HMENU hMenu = GetMenuBar()->GetHMenu();

		if( bView == false )
		{		
			::CheckMenuItem(hMenu, ID_VIEW_WORLDOBJECT, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			::CheckMenuItem(hMenu, ID_VIEW_WORLDOBJECT, MF_BYCOMMAND | MF_CHECKED);
		}

		m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	}
}
#pragma endregion 월드 오브젝트 뷰 선택 시

#pragma region OnEditDungeon
void CMainFrame::OnEditDungeon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_DlgDungeonEditor.ShowWindow(SW_SHOW);
}
#pragma endregion 던전 에디터 활성화

#pragma region OnViewLineNumber
void CMainFrame::OnViewLineNumber()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineNumber = bView;
	}
}
#pragma endregion 라인 번호 출력

#pragma region OnViewLineSetNumber
void CMainFrame::OnViewLineSetNumber()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineSetNumber = bView;
	}
}
#pragma endregion 라인셋 번호 출력

#pragma region OnViewLineCameraNumber
void CMainFrame::OnViewLineCameraNumber()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE_CAMEAR_NUMBER, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE_CAMERA_NUMBER, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE_CAMERA_NUMBER, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineCameraNumber = bView;
	}
}
#pragma endregion 라인 카메라 번호 출력

#pragma region OnViewLineTriggerNumber
void CMainFrame::OnViewLineTriggerNumber()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineTriggerNumber = bView;
	}
}
#pragma endregion 라인 트리거 번호 출력

#pragma region OnViewWaterInfo
void CMainFrame::OnViewWaterInfo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowInnerWaterInfo = bView;
	}
}
#pragma endregion 물속 정보

void CMainFrame::OnTriggerBck()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_TRIGGER_BCK, MF_BYCOMMAND);
	bool bEdit = false;
	if( iChecked & MF_CHECKED )
	{
		::CheckMenuItem(hMenu, ID_TRIGGER_BCK, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = false;
	}
	else
	{	
		::CheckMenuItem(hMenu, ID_TRIGGER_BCK, MF_BYCOMMAND | MF_CHECKED);
		::CheckMenuItem(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = true;
	}
	
	if( bEdit == true )
	{
		m_DlgTriggerEdit.ShowWindow(SW_SHOW);
		m_DlgWorldEditor.ShowWindow(SW_HIDE);
	}
	else
	{
		m_DlgTriggerEdit.ShowWindow(SW_HIDE);
		m_wndProperties.UpdateProp(0);
	}
}
