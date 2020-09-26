// X2ChinaLauncherDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "X2ChinaLauncherDlg.h"
#include "PngWindow/PngWindow.h"
#include "ServerManager.h"
#include "patchthread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2ChinaLauncherDlg 대화 상자

void PreMultiplyRGBChannels(CBitmap &bmp, LPBYTE pBitmapBits);

CX2ChinaLauncherDlg::CX2ChinaLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CX2ChinaLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CX2ChinaLauncherDlg::PreTranslateMessage(MSG* pMsg)
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
				m_MainDlg.PreTranslateMessage(pMsg);
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

void CX2ChinaLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CX2ChinaLauncherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SYSCOMMAND()
	ON_WM_NCHITTEST()
	ON_WM_MOVING()
//	ON_WM_WINDOWPOSCHANGING()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CX2ChinaLauncherDlg 메시지 처리기

BOOL CX2ChinaLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// 먼저 투명 윈도우처리
	WindowAlphaBlendFromPNG("res/LauncherBackground.png", m_hWnd);
	//DragAcceptFiles();

	// 그리고 이후의 겉표지 윈도우 처리!
	m_MainDlg.Create(CMainDlg::IDD);
	//m_MainDlg.ShowWindow(SW_SHOW);

	CString strCaption;
	strCaption.LoadString(IDS_ELSWORD);
	SetWindowText( strCaption );

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CX2ChinaLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CX2ChinaLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBITMAP CX2ChinaLauncherDlg::Create32BitBitmap( HDC hDCSource, int cx, int cy )
{
	// Create 32-bit bitmap
	BITMAPINFO bi = { 0 };
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = cx;
	bi.bmiHeader.biHeight = cy;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = 0;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;
	return ::CreateDIBSection( hDCSource, &bi, DIB_RGB_COLORS, NULL, NULL, 0);
}

void CX2ChinaLauncherDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 차일드윈도우 크기 수정
	if (m_MainDlg)
		m_MainDlg.SetWindowPos(&wndTop, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, SWP_SHOWWINDOW);
}

void CX2ChinaLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnSysCommand(nID, lParam);
}

LRESULT CX2ChinaLauncherDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return HTCAPTION;
}

void CX2ChinaLauncherDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CX2ChinaLauncherDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CX2ChinaLauncherDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CX2ChinaLauncherDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
