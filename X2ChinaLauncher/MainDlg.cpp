// MainDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "X2ChinaLauncherDlg.h"
#include "MainDlg.h"
#include "ServerManager.h"
#include "BtnST.h "
#include "Config.h"
#include ".\maindlg.h"
#include "Util.h"
#include <shlwapi.h>

//KBrowserWrapper CMainDlg::m_browser;   ///< HTML file browser

// CMainDlg 대화 상자입니다.

extern bool g_bPatcherPatched;

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_pMinimizeButton = NULL;
	m_pCloseButton = NULL;
	m_pHomeButton = NULL;
	m_pGuideButton = NULL;
	m_pPaidButton = NULL;
	m_pSupportButton = NULL;
	m_pBBSButton = NULL;

	m_pAccountButton = NULL;
	m_pStartButton = NULL;

	m_pManualPatchButton = NULL;

	m_bMinimize = FALSE;
}

CMainDlg::~CMainDlg()
{
	if(m_pMinimizeButton)
		delete m_pMinimizeButton;
	if(m_pCloseButton)
		delete m_pCloseButton;
	if(m_pHomeButton)
		delete m_pHomeButton;
	if(m_pGuideButton)
		delete m_pGuideButton;
	if(m_pPaidButton)
		delete m_pPaidButton;
	if(m_pSupportButton)
		delete m_pSupportButton;
	if(m_pBBSButton)
		delete m_pBBSButton;
	
	if(m_pAccountButton)
		delete m_pAccountButton;
	if(m_pStartButton)
		delete m_pStartButton;

	if(m_pManualPatchButton)
		delete m_pManualPatchButton;

	CServerManager::ReleaseInstance();
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_PAINT()

	//계정 버튼
	ON_BN_CLICKED(IDC_MINIMIZE_BUTTON, OnMinimizeButtonClicked)
	//시작 버튼
	ON_BN_CLICKED(IDC_CLOSE_BUTTON, OnCloseButtonClicked)


	//메뉴 버튼
	ON_BN_CLICKED(IDC_HOME_BUTTON, OnHomeButtonClicked)
	ON_BN_CLICKED(IDC_GUIDE_BUTTON, OnGuideButtonClicked)
	ON_BN_CLICKED(IDC_PAID_BUTTON, OnPaidButtonClicked)
	ON_BN_CLICKED(IDC_SUPPORT_BUTTON, OnSupportButtonClicked)
	ON_BN_CLICKED(IDC_BBS_BUTTON, OnBBSButtonClicked)

	//계정 버튼
	ON_BN_CLICKED(IDC_ACCOUNT_BUTTON, OnAccountButtonClicked)
	//시작 버튼
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButtonClicked)

	//수동패치 버튼
	ON_BN_CLICKED(IDC_MANUALPATCH_BUTTON, OnManualPatchButtonClicked)
	ON_WM_MOVING()
	ON_WM_CTLCOLOR()
	ON_WM_KEYDOWN()

	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
//	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()


// CMainDlg 메시지 처리기입니다.
// 생성될 때 컬러키 지정
BOOL CMainDlg::OnInitDialog()
{
	SetWindowLong( m_hWnd, GWL_EXSTYLE, ::GetWindowLong( m_hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB( 255, 0, 255), 0, LWA_COLORKEY/* | LWA_ALPHA */);
	
	//최소화 버튼
	m_pMinimizeButton = new CButtonST;
	m_pMinimizeButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(912, 13, 0, 0), this, IDC_MINIMIZE_BUTTON);
	m_pMinimizeButton->SetBitmaps(IDB_MINIMIZE_OVER_BITMAP, RGB(255, 0, 255), IDB_MINIMIZE_BITMAP, RGB(255, 0, 255));
	m_pMinimizeButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pMinimizeButton->SizeToContent();
	m_pMinimizeButton->DrawBorder(FALSE, FALSE);
	m_pMinimizeButton->ShowWindow(SW_HIDE);
	
	//닫기 버튼
	m_pCloseButton = new CButtonST;
	m_pCloseButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(912, 27, 0, 0), this, IDC_CLOSE_BUTTON);
	m_pCloseButton->SetBitmaps(IDB_CLOSE_OVER_BITMAP, RGB(255, 0, 255), IDB_CLOSE_BITMAP, RGB(255, 0, 255));
	m_pCloseButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pCloseButton->SizeToContent();
	m_pCloseButton->DrawBorder(FALSE, FALSE);
	m_pCloseButton->ShowWindow(SW_HIDE);

	//메뉴 버튼
	m_pHomeButton = new CButtonST;
	m_pHomeButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(331+2, 133+3-123, 0, 0), this, IDC_HOME_BUTTON);
	m_pHomeButton->SetBitmaps(IDB_HOME_OVER_BITMAP, RGB(255, 0, 255), IDB_HOME_BITMAP, RGB(255, 0, 255));
	m_pHomeButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pHomeButton->SizeToContent();
	m_pHomeButton->DrawBorder(FALSE, FALSE);
	m_pHomeButton->ShowWindow(SW_HIDE);

	m_pGuideButton = new CButtonST;
	m_pGuideButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(452+2, 133+3-123, 0, 0), this, IDC_GUIDE_BUTTON);
	m_pGuideButton->SetBitmaps(IDB_GUIDE_OVER_BITMAP, RGB(255, 0, 255), IDB_GUIDE_BITMAP, RGB(255, 0, 255));
	m_pGuideButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pGuideButton->SizeToContent();
	m_pGuideButton->DrawBorder(FALSE, FALSE);
	m_pGuideButton->ShowWindow(SW_HIDE);

	//m_GuideButton.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(452, 133, 0, 0), this, IDC_GUIDE_BUTTON);
	//m_GuideButton.LoadBitmaps(IDB_GUIDE_BITMAP);
	//m_GuideButton.SizeToContent();

	m_pPaidButton = new CButtonST;
	m_pPaidButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(571+2, 133+3-123, 0, 0), this, IDC_PAID_BUTTON);
	m_pPaidButton->SetBitmaps(IDB_PAID_OVER_BITMAP, RGB(255, 0, 255), IDB_PAID_BITMAP, RGB(255, 0, 255));
	m_pPaidButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pPaidButton->SizeToContent();
	m_pPaidButton->DrawBorder(FALSE, FALSE);
	m_pPaidButton->ShowWindow(SW_HIDE);

	//m_PaidButton.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(571, 133, 0, 0), this, IDC_PAID_BUTTON);
	//m_PaidButton.LoadBitmaps(IDB_PAID_BITMAP);
	//m_PaidButton.SizeToContent();

	m_pSupportButton = new CButtonST;
	m_pSupportButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(690+2, 133+3-123, 0, 0), this, IDC_SUPPORT_BUTTON);
	m_pSupportButton->SetBitmaps(IDB_SUPPORT_OVER_BITMAP, RGB(255, 0, 255), IDB_SUPPORT_BITMAP, RGB(255, 0, 255));
	m_pSupportButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pSupportButton->SizeToContent();
	m_pSupportButton->DrawBorder(FALSE, FALSE);
	m_pSupportButton->ShowWindow(SW_HIDE);

	//m_SupportButton.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(690, 133, 0, 0), this, IDC_SUPPORT_BUTTON);
	//m_SupportButton.LoadBitmaps(IDB_SUPPORT_BITMAP);
	//m_SupportButton.SizeToContent();

	m_pBBSButton = new CButtonST;
	m_pBBSButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(808+2, 133+3-123, 0, 0), this, IDC_BBS_BUTTON);
	m_pBBSButton->SetBitmaps(IDB_BBS_OVER_BITMAP, RGB(255, 0, 255), IDB_BBS_BITMAP, RGB(255, 0, 255));
	m_pBBSButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pBBSButton->SizeToContent();
	m_pBBSButton->DrawBorder(FALSE, FALSE);
	m_pBBSButton->ShowWindow(SW_HIDE);

	//m_BBSButton.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(808, 133, 0, 0), this, IDC_BBS_BUTTON);
	//m_BBSButton.LoadBitmaps(IDB_BBS_BITMAP);
	//m_BBSButton.SizeToContent();



	//계정버튼
	m_pAccountButton = new CButtonST;
	m_pAccountButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(454+2, 426+7-122, 0, 0), this, IDC_ACCOUNT_BUTTON);
	m_pAccountButton->SetBitmaps(IDB_ACCOUNT_OVER_BITMAP, RGB(255, 0, 255), IDB_ACCOUNT_BITMAP, RGB(255, 0, 255));
	m_pAccountButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pAccountButton->SizeToContent();
	m_pAccountButton->DrawBorder(FALSE, FALSE);
	m_pAccountButton->ShowWindow(SW_HIDE);

	//시작버튼
	m_pStartButton = new CButtonST;
	m_pStartButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(609+1, 426+7-122, 0, 0), this, IDC_START_BUTTON);
	m_pStartButton->SetBitmaps(IDB_START_OVER_BITMAP, RGB(255, 0, 255), 
											IDB_START_ENABLE_BITMAP, RGB(255, 0, 255),
											NULL, 0,
											NULL, 0,
											IDB_START_DISABLE_BITMAP, RGB(255, 0, 255));
	m_pStartButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pStartButton->SizeToContent();
	m_pStartButton->DrawBorder(FALSE, FALSE);
	m_pStartButton->ShowWindow(SW_HIDE);


	//m_StartButton.EnableWindow(FALSE);

	//m_TestButton.Create(NULL, WS_VISIBLE | BS_PUSHBUTTON, CRect(609, 426+100, 600+200, 526+200), this, IDC_TEST_BUTTON);
	//m_TestButton.SetWindowText("hi");

	//m_TestButton.Create(_T("hihihihi"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(609, 426+100, 600+200, 526+200), this, IDC_START_BUTTON);
	//m_TestButton.LoadBitmaps(IDB_START_UP_BITMAP, IDB_START_DOWN_BITMAP);
	//m_TestButton.SizeToContent();


	//m_TestButton.SetDlgItemText(IDC_START_BUTTON, "start up button");
	//CFont font;
	//font.CreateFont()
	//m_StartButton.SetFont(&font);

	//BOOL test = m_TestStatic.Create("ohohoh~", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(609, 426, 609+100, 526+20), this, IDC_TEST_BUTTON);
	//m_TestStatic.SetWindowPos()

	//수동패치 버튼
	m_pManualPatchButton = new CButtonST;
	m_pManualPatchButton->Create(NULL, WS_CHILD | WS_VISIBLE, CRect(816, 765-125, 0, 0), this, IDC_MANUALPATCH_BUTTON);
	m_pManualPatchButton->SetBitmaps(IDB_MANUALPATCH_OVER_BITMAP, RGB(255, 0, 255), IDB_MANUALPATCH_BITMAP, RGB(255, 0, 255));
	m_pManualPatchButton->SetPressedStyle(CButtonST::BTNST_PRESSED_NONE, FALSE);
	m_pManualPatchButton->SizeToContent();
	m_pManualPatchButton->DrawBorder(FALSE, FALSE);
	m_pManualPatchButton->ShowWindow(SW_HIDE);

	//RECT rcWindow = { 0, };
	//RECT rcClient = { 0, };
	//HWND hEdit = ::GetDlgItem( GetSafeHwnd(), IDC_WEBBROWSER_EDIT );
	//::GetClientRect( hEdit, &rcClient );
	//POINT pt = { rcWindow.left, rcWindow.top };
	//::ScreenToClient( GetSafeHwnd(), &pt );

	//if ( true == m_browser.Create( /*const_cast<WCHAR*>( wcharBuf2 )*/ _T("c:\\test.html"), pt.x, pt.y,
	//	rcClient.right + ::GetSystemMetrics( SM_CXVSCROLL ),
	//	rcClient.bottom, GetSafeHwnd(),
	//	KBrowserWrapper::BROWSER_TYPE_CHILD ) )
	//{
	//	//::DestroyWindow( hEdit );
	//	return TRUE;
	//}

	//if ( true == m_browser.Create(_T("http://www.naver.com")/*_T("c:\\test.html")*/, 300, 100,
	//	400,
	//	400, GetSafeHwnd(),
	//	KBrowserWrapper::BROWSER_TYPE_CHILD ) )
	//{
	//}
	//RECT rc = {300, 100, 700, 500};
	//m_browser.Init();
	//m_browser.Create( rc, GetSafeHwnd() );
	//m_browser.Navigate( "http://www.naver.com", NULL );//strStreamValByPost.str().c_str() ); 

	m_BannerDlg.Create(CBannerDlg::IDD, this);
	m_BannerDlg.MoveWindow(g_iBannerRect.left, g_iBannerRect.top, g_iBannerRect.Width(), g_iBannerRect.Height());
	m_BannerDlg.ShowWindow(SW_HIDE);
	if ( true == m_browser.Create(const_cast<LPTSTR>(g_pcBannerLink), 
											0, 0, g_iBannerRect.Width(), g_iBannerRect.Height(), 
											m_BannerDlg.GetSafeHwnd(),	KBrowserWrapper::BROWSER_TYPE_CHILD ) )
	{
	}

	//m_NoticeDlg.Create(CNoticeDlg::IDD, this);
	//m_NoticeDlg.MoveWindow(450, 313, 326, 98);
	//m_NoticeDlg.ShowWindow(SW_SHOW);
	//if ( true == m_NoticeBrowser.Create(/*_T("http://www.naver.com")*/_T("http://dn.sdo.com/project/launcher/index.asp"), 0, 0,
	//	326,
	//	98, m_NoticeDlg.GetSafeHwnd(),
	//	KBrowserWrapper::BROWSER_TYPE_CHILD ) )
	//{
	//}

	//xml로 부터 서버 목록 읽어오기
	if(!CServerManager::GetInstance()->Load(this))
	{
		//"서버에 접속할 수 없습니다" 라는 문구 출력
		CString strMessage;
		strMessage.LoadString(IDS_SERVERERROR_MESSAGE);
		AfxMessageBox(strMessage, MB_OK);

		CDialog* pMainDlg = (CDialog*)::AfxGetMainWnd();   
		pMainDlg->EndDialog(0);

		return FALSE;
	}

	//버튼들
	m_pMinimizeButton->ShowWindow(SW_SHOW);
	m_pCloseButton->ShowWindow(SW_SHOW);
	m_pHomeButton->ShowWindow(SW_SHOW);
	m_pGuideButton->ShowWindow(SW_SHOW);
	m_pPaidButton->ShowWindow(SW_SHOW);
	m_pSupportButton->ShowWindow(SW_SHOW);
	m_pBBSButton->ShowWindow(SW_SHOW);
	m_pAccountButton->ShowWindow(SW_SHOW);
	m_pStartButton->ShowWindow(SW_SHOW);
	m_pManualPatchButton->ShowWindow(SW_SHOW);

	if( FALSE == CServerManager::GetInstance()->IsValidServerID( CServerManager::GetInstance()->GetSelectedServer() ) )
	{
		NotifyStartButtonEnable(FALSE);
	}

	//현재 패치 진행 Bitmap
	{
		m_PatchCurProgressBG.LoadBitmap(MAKEINTRESOURCE( IDB_PATCHCURPROGRESS_BG ));
		m_PatchCurProgressBitmap.LoadBitmap(MAKEINTRESOURCE( IDB_PATCHCURPROGRESS_BITMAP ));

		BITMAP bmpInfo;
		m_PatchCurProgressBG.GetObject(sizeof(BITMAP), &bmpInfo);

		RECT rc;
		rc.left = 344-4+44;
		rc.top = 770-123-4-17+18;
		rc.right = rc.left+bmpInfo.bmWidth;
		rc.bottom = rc.top+bmpInfo.bmHeight;

		CServerManager::GetInstance()->SetPatchCurProgressRegion(rc);
	}

	//전체 패치 진행 Bitmap
	{
		m_PatchAllProgressBG.LoadBitmap(MAKEINTRESOURCE( IDB_PATCHALLPROGRESS_BG ));
		m_PatchAllProgressBitmap.LoadBitmap(MAKEINTRESOURCE( IDB_PATCHALLPROGRESS_BITMAP ));

		BITMAP bmpInfo;
		m_PatchAllProgressBG.GetObject(sizeof(BITMAP), &bmpInfo);

		RECT rc;
		rc.left = 344-4+44;
		rc.top = 770-123-4+18;
		rc.right = rc.left+bmpInfo.bmWidth;
		rc.bottom = rc.top+bmpInfo.bmHeight;

		CServerManager::GetInstance()->SetPatchAllProgressRegion(rc);
	}

	if( g_bPatcherPatched == true )
	{
		OnStartButtonClicked();
	}

	return TRUE;
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
		//alt가 눌렸을때
		case WM_SYSKEYDOWN:
			{
				if(pMsg->wParam == VK_F4)
					return TRUE;
			}
			break;
		// 키가 눌렸을때
		case WM_KEYDOWN:
			switch(pMsg->wParam)
			{
				// 리턴키\tab 
				case VK_RETURN:
					return TRUE;

				// ESC키
				case VK_ESCAPE:
					//패치 진행중에 esc가 들어오면 패치를 중단할지 묻는다.
					//if(CServerManager::GetInstance()->GetPatchProgressRatio() > 0.0f)
					if(CServerManager::GetInstance()->GetPatchThreadStatus() == CServerManager::ePatchThread_Status_Start)
					{
						CServerManager::GetInstance()->SuspendPatchThread();

						//"패치를 중단하시겠습니까?"
						CString strMessage;
						strMessage.LoadString(IDS_PAUSEPATCH_MESSAGE);

						if(this->MessageBox(strMessage, _T("Elsword Launcher"), MB_OKCANCEL) == IDOK)
						{
							
							CServerManager::GetInstance()->DestroyPatchThread();

							//패치 진행바를 리셋한다.
							CServerManager::GetInstance()->UpdatePatchCurProgress();
							CServerManager::GetInstance()->UpdatePatchAllProgress();

							NotifyStartButtonEnable(TRUE);
						}
						else
						{
							CServerManager::GetInstance()->ResumePatchThread();
						}
					}
					else if(CServerManager::GetInstance()->GetPatchThreadStatus() == CServerManager::ePatchThread_Status_Init)//esc가 들어오면 프로그램을 종료할지 묻는다.
					{
						//"Elsword Launcher를 종료하시겠습니까?"
						CString strMessage;
						strMessage.LoadString(IDS_CLOSELAUNCHER_MESSAGE);

						if(this->MessageBox(strMessage, _T("Elsword Launcher"), MB_OKCANCEL) == IDOK)
						{
							CDialog* pMainDlg = (CDialog*)::AfxGetMainWnd();   
							pMainDlg->EndDialog(0);
						}
					}

					return TRUE;
			}
			break;
	}

	//alt+print screen 막기
	if( GetKeyState(VK_MENU) && pMsg->wParam == VK_SNAPSHOT )
	{
		::OpenClipboard(NULL);
		::EmptyClipboard();
		::CloseClipboard();
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CMainDlg::OnPaint()
{
	//OutputDebugString(_T("CMainDlg::OnPaint()......................\n"));
	
	//GetDC()->SelectObject(GetStockObject(NULL_BRUSH));



	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	OnPaintPatchCurProgress(dc);
	OnPaintPatchAllProgress(dc);



	// 흰 색으로 칠해주마
	//dc.FloodFill(0, 0, RGB(255, 0, 255));

	//RECT rc;
	//GetClientRect(&rc);
	//dc.FillSolidRect(&rc, RGB(255, 0, 255));

	//RECT rc = {0, 0, 100, 100};
	//DrawText(m_StartButton.GetDC()->m_hDC, "hohohoho", -1, &rc, DT_CENTER);
	////m_StartButton.TextOut()
	////TextOut();

	//SendMessage(WM_LOADSERVERLIST);
}

void CMainDlg::OnMinimizeButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetWindowRect(&m_WindowRect);

	//트릭을 사용하여 최소화 시켜야 한다.
	AfxGetMainWnd()->MoveWindow(999999, 999999, 0, 0);
	ShowWindow(SW_MINIMIZE);
	AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
	m_bMinimize = TRUE;
}

void CMainDlg::OnCloseButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//패치 진행중이라면 패치를 중단하고 Elsword Launcher를 종료할 지 묻는다
	if(CServerManager::GetInstance()->GetPatchThreadStatus() == CServerManager::ePatchThread_Status_Start)
	{
		CServerManager::GetInstance()->SuspendPatchThread();

		//"패치를 중단하고 Elsword Launcher를 종료하시겠습니까?"
		CString strMessage;
		strMessage.LoadString(IDS_PAUSEPATCH_CLOSELAUNCHER_MESSAGE);

		if(this->MessageBox(strMessage, _T("Elsword Launcher"), MB_OKCANCEL) == IDOK)
		{

			CServerManager::GetInstance()->DestroyPatchThread();

			//패치 진행바를 리셋한다.
			CServerManager::GetInstance()->UpdatePatchCurProgress();
			CServerManager::GetInstance()->UpdatePatchAllProgress();

			NotifyStartButtonEnable(TRUE);

			while(1)
			{
				//기존 패치가 리셋 되었는지 확인후 프로그램 종료한다.
				if(CServerManager::GetInstance()->GetPatchThreadStatus() == CServerManager::ePatchThread_Status_Init)
				{
					CDialog* pMainDlg = (CDialog*)::AfxGetMainWnd();   
					pMainDlg->EndDialog(0);
					return;
				}	

				Sleep(500);
			}
		}
		else
		{
			CServerManager::GetInstance()->ResumePatchThread();
		}
	}
	else if(CServerManager::GetInstance()->GetPatchThreadStatus() == CServerManager::ePatchThread_Status_Init)//패치중이 아니라면, Elsword Launcher를 종료할지  묻는다
	{
		//"Elsword Launcher를 종료하시겠습니까?"
		CString strMessage;
		strMessage.LoadString(IDS_CLOSELAUNCHER_MESSAGE);

		if(this->MessageBox(strMessage, _T("Elsword Launcher"), MB_OKCANCEL) == IDOK)
		{
			CDialog* pMainDlg = (CDialog*)::AfxGetMainWnd();   
			pMainDlg->EndDialog(0);
			return;
		}
	}
}

void CMainDlg::OnHomeButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// open URL in the browser.
	ShellExecute(NULL, _T("open"), _T("http://els.ztgame.com"), NULL, NULL, SW_SHOWNORMAL);
}

void CMainDlg::OnGuideButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// open URL in the browser.
	ShellExecute(NULL, _T("open"), _T("http://els.ztgame.com/html/games/20101028/1537402714.shtml?mx=sl10=acitem01"), NULL, NULL, SW_SHOWNORMAL);
}

void CMainDlg::OnPaidButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// open URL in the browser.
	ShellExecute(NULL, _T("open"), _T("http://pay.ztgame.com:81/qfillcard.php"), NULL, NULL, SW_SHOWNORMAL);
}

void CMainDlg::OnSupportButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// open URL in the browser.
	ShellExecute(NULL, _T("open"), _T("http://www.ztgame.com/kfzx.html"), NULL, NULL, SW_SHOWNORMAL);
}

void CMainDlg::OnBBSButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// open URL in the browser.
	ShellExecute(NULL, _T("open"), _T("http://bbs.els.ztgame.com"), NULL, NULL, SW_SHOWNORMAL);
}

void CMainDlg::OnAccountButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// open URL in the browser.
	ShellExecute(NULL, _T("open"), _T("http://els.ztgame.com/register.html"), NULL, NULL, SW_SHOWNORMAL);
}

void CMainDlg::OnStartButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//이전에 선택한 것과 현재 선택한 서버가 다르다면
	int temp = CServerManager::GetInstance()->GetSelectedServer();

	if( FALSE == CServerManager::GetInstance()->IsValidServerID(temp) )
		return;

	if(CServerManager::GetInstance()->GetSelectedServer() != temp && CServerManager::GetInstance()->GetPatchThreadStatus() == CServerManager::ePatchThread_Status_Start)
	{
		CServerManager::GetInstance()->SuspendPatchThread();

		//"현재 선택한 서버에서 다시 패치를 받으시겠습니까?"
		CString strMessage;
		strMessage.LoadString(IDS_RESTARTPATCH_MESSAGE);

		if(this->MessageBox(strMessage, _T("Elsword Launcher"), MB_OKCANCEL) == IDOK)
		{
			CServerManager::GetInstance()->DestroyPatchThread();

			// TODO: 무한 루프 대비를 해야하지 않을까?
			while(CServerManager::GetInstance()->GetPatchThreadStatus() != CServerManager::ePatchThread_Status_Init)
			{
				Sleep(100);
			}
		}
		else
		{
			CServerManager::GetInstance()->ResumePatchThread();
			return;
		}
	}

	if(CServerManager::GetInstance()->GetPatchThreadStatus() == CServerManager::ePatchThread_Status_Init)
	{
		//패치 스레드 생성
		CServerManager::GetInstance()->CreatePatchThread(temp);
		//게임 시작 버튼 누르면 패치 진행바에 0%가 바로 나타나게 하도록 한다.
		CServerManager::GetInstance()->UpdatePatchCurProgress();
		CServerManager::GetInstance()->UpdatePatchAllProgress();

		NotifyStartButtonEnable(FALSE);

		SetGameGuardPatchServer( CServerManager::GetInstance()->GetSelectedLine() );
	}
}

void CMainDlg::OnManualPatchButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	INT i=0;

	ShellExecute(NULL, _T("open"), _T("http://els.ztgame.com/download.shtml"), NULL, NULL, SW_SHOWNORMAL);
}

LRESULT CMainDlg::OnLoadServerList(WPARAM wParam, LPARAM lParam)
{
	int i=0;

	return 0;
}

void CMainDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

 	//::SendMessage(m_browser.GetWndHandle(), WM_MOVING, 0, 0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC: // 정적 콘트롤인 경우 

		UINT id=pWnd->GetDlgCtrlID();

		pDC->SetBkMode(TRANSPARENT); // 배경을 투명하게

		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CMainDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nChar == VK_ESCAPE)
	{
		int i=0;
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//OutputDebugString(_T("CMainDlg is OnEraseBkgnd......................\n"));
	return FALSE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CMainDlg::PreSubclassWindow()
{

}

void CMainDlg::NotifyStartButtonEnable(BOOL bEnable)
{
	m_pStartButton->EnableWindow(bEnable);
}

void CMainDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CPaintDC dc(this); // device context for painting

	//OutputDebugString(_T("OnWindowPosChanged......................\n"));

	// 흰 색으로 칠해주마
	//dc.FloodFill(0, 0, RGB(255, 0, 255));

	GetDC()->SelectObject(GetStockObject(NULL_BRUSH));


	RECT rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, RGB(255, 0, 255));

	//패치바들 한번 그려준다.
	CServerManager::GetInstance()->UpdatePatchCurProgress();
	CServerManager::GetInstance()->UpdatePatchAllProgress();

}

void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//패치 스레드 제거
	//CServerManager::GetInstance()->SuspendPatchThread();
	//CServerManager::GetInstance()->DestroyPatchThread();
}

void CMainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(nState == WA_INACTIVE)
		return;

	if(m_bMinimize)
	{
		//MoveWindow(m_WindowRect.left, m_WindowRect.top, m_WindowRect.Width(), m_WindowRect.Height());
		AfxGetMainWnd()->ShowWindow(SW_RESTORE);
		ShowWindow(SW_RESTORE);
		AfxGetMainWnd()->MoveWindow(m_WindowRect.left, m_WindowRect.top, m_WindowRect.Width(), m_WindowRect.Height());

		Invalidate();

		m_bMinimize = FALSE;
	}
}

//void CMainDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
//{
//	//CDialog::OnWindowPosChanging(lpwndpos);
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//}

void CMainDlg::OnPaintPatchCurProgress(CPaintDC& dc)
{
	RECT rcProgress = CServerManager::GetInstance()->GetPatchCurProgressRegion();

	if(m_PatchCurProgressBG.m_hObject)
	{
		BITMAP bmpInfo;
		m_PatchCurProgressBG.GetObject(sizeof(BITMAP), &bmpInfo);

		CDC dc2;
		dc2.CreateCompatibleDC(&dc);
		CBitmap* oldBmp = dc2.SelectObject(&m_PatchCurProgressBG);


		dc.BitBlt(rcProgress.left, rcProgress.top, bmpInfo.bmWidth, bmpInfo.bmHeight, &dc2, 0, 0, SRCCOPY);

		dc2.SelectObject(oldBmp);
	}

	if(m_PatchCurProgressBitmap.m_hObject)
	{
		BITMAP bmpInfo;
		m_PatchCurProgressBitmap.GetObject(sizeof(BITMAP), &bmpInfo);
		float fRatio = CServerManager::GetInstance()->GetPatchCurProgressRatio();

		CDC dc2;
		dc2.CreateCompatibleDC(&dc);
		CBitmap* oldBmp = dc2.SelectObject(&m_PatchCurProgressBitmap);


		dc.BitBlt(rcProgress.left, rcProgress.top, (int)(bmpInfo.bmWidth*fRatio), bmpInfo.bmHeight, &dc2, 0, 0, SRCCOPY);

		dc2.SelectObject(oldBmp);

		//패치 진행 퍼센트 표시
		WCHAR wPercent[10];
		RECT PatchProgressRegion;
		PatchProgressRegion.left = rcProgress.left;
		PatchProgressRegion.top = rcProgress.top;
		PatchProgressRegion.right = PatchProgressRegion.left+bmpInfo.bmWidth;
		PatchProgressRegion.bottom = PatchProgressRegion.top+20;

		std::wstring filename;
		ConvertCharToWCHAR(filename, PathFindFileNameA(CServerManager::GetInstance()->GetPatchCurProgressFilename().c_str()));

		if(filename.empty())
			wsprintf(wPercent, _T("%d%%"), (int)(fRatio*100.0f));
		else
			wsprintf(wPercent, _T(" (%d%%)"), (int)(fRatio*100.0f));

		std::wstring wstr = filename + wPercent;
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(wstr.c_str(), &PatchProgressRegion, DT_CENTER);
	}
}

void CMainDlg::OnPaintPatchAllProgress(CPaintDC& dc)
{
	RECT rcProgress = CServerManager::GetInstance()->GetPatchAllProgressRegion();

	if(m_PatchAllProgressBG.m_hObject)
	{
		BITMAP bmpInfo;
		m_PatchAllProgressBG.GetObject(sizeof(BITMAP), &bmpInfo);

		CDC dc2;
		dc2.CreateCompatibleDC(&dc);
		CBitmap* oldBmp = dc2.SelectObject(&m_PatchAllProgressBG);


		dc.BitBlt(rcProgress.left, rcProgress.top, bmpInfo.bmWidth, bmpInfo.bmHeight, &dc2, 0, 0, SRCCOPY);

		dc2.SelectObject(oldBmp);
	}

	if(m_PatchAllProgressBitmap.m_hObject)
	{
		BITMAP bmpInfo;
		m_PatchAllProgressBitmap.GetObject(sizeof(BITMAP), &bmpInfo);
		float fRatio = CServerManager::GetInstance()->GetPatchAllProgressRatio();

		CDC dc2;
		dc2.CreateCompatibleDC(&dc);
		CBitmap* oldBmp = dc2.SelectObject(&m_PatchAllProgressBitmap);


		dc.BitBlt(rcProgress.left, rcProgress.top, (int)(bmpInfo.bmWidth*fRatio), bmpInfo.bmHeight, &dc2, 0, 0, SRCCOPY);

		dc2.SelectObject(oldBmp);

		//패치 진행 퍼센트 표시
		WCHAR wPercent[10];
		RECT PatchProgressRegion;
		PatchProgressRegion.left = rcProgress.left;
		PatchProgressRegion.top = rcProgress.top;
		PatchProgressRegion.right = PatchProgressRegion.left+bmpInfo.bmWidth;
		PatchProgressRegion.bottom = PatchProgressRegion.top+20;


		wsprintf(wPercent, _T("%d%%"), (int)(fRatio*100.0f));
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(wPercent, &PatchProgressRegion, DT_CENTER);
	}
}

bool CMainDlg::SetGameGuardPatchServer(int iLine)
{
	// dwIndex 1: Telecom, 2: CNC
	DWORD dwIndex = 1;
	switch(iLine)
	{
	case CServerManager::LINE_TELECOM:
		dwIndex = 1;
		break;
	case CServerManager::LINE_CNC:
		dwIndex = 2;
		break;
	default:
		ASSERT(false && "Wrong line type!");
	}

	bool ret = false;
	HKEY hReg;
	DWORD result;
	DWORD datasize = sizeof(DWORD);
	if (RegCreateKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\INCAInternet\\nProtectGameGuard\\Update"), 0, 0, 0, KEY_ALL_ACCESS, NULL, &hReg, &result) == ERROR_SUCCESS)
	{
		if (result == REG_OPENED_EXISTING_KEY)
		{
			if (RegSetValueEx(hReg, _T("ServerIndex"), 0, REG_DWORD, (LPBYTE) &dwIndex, datasize))
			{
				ret = true;
			}
		}
		RegCloseKey(hReg);
	}

	return ret;
}