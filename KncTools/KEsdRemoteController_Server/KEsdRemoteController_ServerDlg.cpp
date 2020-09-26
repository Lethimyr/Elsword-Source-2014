// KEsdRemoteController_ServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "KEsdRemoteController_Server.h"
#include "KEsdRemoteController_ServerDlg.h"
#include ".\kesdremotecontroller_serverdlg.h"
#include "DBConnectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

lua_State* g_pLua	= NULL;


// CKEsdRemoteController_ServerDlg 대화 상자



CKEsdRemoteController_ServerDlg::CKEsdRemoteController_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKEsdRemoteController_ServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKEsdRemoteController_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SMS_CHECK, m_checkSMS);
}

BEGIN_MESSAGE_MAP(CKEsdRemoteController_ServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_PATCH, OnBnClickedButtonPatch)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH, OnBnClickedButtonLaunch)
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, OnBnClickedButtonShutdown)
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH_DBG, OnBnClickedButtonLaunchDbg)
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO, OnBnClickedButtonLaunchCo)
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO_DBG, OnBnClickedButtonLaunchCoDbg)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_DBG, OnBnClickedButtonLoginDbg)
	ON_BN_CLICKED(IDC_SMS_CHECK, OnBnClickedSmsCheck)
	ON_BN_CLICKED(IDC_BUTTON_CHANNEL, OnBnClickedButtonChannel)
	ON_BN_CLICKED(IDC_BUTTON_IDPW, OnBnClickedButtonIdpw)
	ON_BN_CLICKED(IDC_BUTTON_HSB_UPDATE, OnBnClickedButtonHsbUpdate)
	ON_BN_CLICKED(IDC_BUTTON_SVN_UPDATE, OnBnClickedButtonSvnUpdate)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWS_UPDATE, OnBnClickedButtonWindowsUpdate)
	ON_BN_CLICKED(IDC_BUTTON_VIRUS_CHECK, OnBnClickedButtonVirusCheck)
	ON_BN_CLICKED(IDC_UI_LOCK, OnBnClickedUiLock)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CKEsdRemoteController_ServerDlg 메시지 처리기

BOOL CKEsdRemoteController_ServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 윈도우 생성 위치를 설정 : 오른쪽 상단에 위치하도록!
	RECT rect;
	GetWindowRect( &rect );
	MoveWindow( GetSystemMetrics( SM_CXSCREEN ) - 270, 20, ( rect.right - rect.left ), ( rect.bottom - rect.top ) );

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	g_pLua = lua_open();

	if( g_pLua == NULL )
	{
		KDbgStm << "루아 생성 실패.!";
		return FALSE;
	}

	//luaopen_base( g_pLua );  // 루아함수 tonumber를 사용해야 한다.
	//luaopen_table( g_pLua );
	//luaopen_io( g_pLua );
	//luaopen_string( g_pLua );
	//luaopen_math( g_pLua );
	//lua_settop( g_pLua, 0 );

	luaL_openlibs( g_pLua );
	lua_settop( g_pLua, 0 );

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetTimer( 0, 1, NULL );

	theApp.m_kImp.Initialize();
	
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CKEsdRemoteController_ServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

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
HCURSOR CKEsdRemoteController_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKEsdRemoteController_ServerDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if ( nIDEvent == 0 )
	{
		theApp.m_kImp.Update();
	}

	CDialog::OnTimer(nIDEvent);
}

int CKEsdRemoteController_ServerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	theApp.m_kImp.CheckAlreadyOnServer();

	return 0;
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonPatch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	theApp.m_kImp.OnEsdSVNUpdate();
}

void CKEsdRemoteController_ServerDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	lua_close( g_pLua );

	KillTimer( 0 );

	CDialog::OnClose();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLaunch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	theApp.m_kImp.OnEsdLaunch();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonShutdown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	theApp.m_kImp.OnEsdShutdown();	
}

BOOL CKEsdRemoteController_ServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN )
	{       
		if ( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLaunchDbg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 동작 하지 않습니다!
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLaunchCo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_kImp.OnEsdLaunch_CenterServer();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLaunchCoDbg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 동작 하지 않습니다.
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_kImp.OnEsdLaunch_LoginServer();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLoginDbg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 동작 하지 않습니다.
}

void CKEsdRemoteController_ServerDlg::OnBnClickedSmsCheck()
{
	bool bCheck = ( m_checkSMS.GetCheck() == 0 ? false : true );
	theApp.m_kImp.SetPhoneEnable( bCheck );
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonChannel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_kImp.OnEsdLaunch_ChannelServer();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonIdpw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( theApp.m_kImp.IsDBConnStr() )
	{
		theApp.m_kImp.OpenDBConnectionDlg();
	}
	else
	{
		::MessageBox( NULL, L"DB Connection String을 사용하지 않도록 세팅되어 있습니다!", L"", 0 );
	}
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonHsbUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_kImp.OnEsdHSBUpdate();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonSvnUpdate()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdSVNUpdate();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonWindowsUpdate()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdWindowsUpdate();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonVirusCheck()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdVirusCheck();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedUiLock()
{
	// TODO: Add your control notification handler code here

	//ON_BN_CLICKED(IDC_BUTTON_LAUNCH, OnBnClickedButtonLaunch)
	//ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, OnBnClickedButtonShutdown)
	//ON_BN_CLICKED(IDC_BUTTON_LAUNCH_DBG, OnBnClickedButtonLaunchDbg)
	//ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO, OnBnClickedButtonLaunchCo)
	//ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO_DBG, OnBnClickedButtonLaunchCoDbg)
	//ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnBnClickedButtonLogin)
	//ON_BN_CLICKED(IDC_BUTTON_LOGIN_DBG, OnBnClickedButtonLoginDbg)
	//ON_BN_CLICKED(IDC_SMS_CHECK, OnBnClickedSmsCheck)
	//ON_BN_CLICKED(IDC_BUTTON_CHANNEL, OnBnClickedButtonChannel)
	//ON_BN_CLICKED(IDC_BUTTON_IDPW, OnBnClickedButtonIdpw)
	//ON_BN_CLICKED(IDC_BUTTON_HSB_UPDATE, OnBnClickedButtonHsbUpdate)
	//ON_BN_CLICKED(IDC_BUTTON_SVN_UPDATE, OnBnClickedButtonSvnUpdate)
	//ON_BN_CLICKED(IDC_BUTTON_WINDOWS_UPDATE, OnBnClickedButtonWindowsUpdate)
	//ON_BN_CLICKED(IDC_BUTTON_VIRUS_CHECK, OnBnClickedButtonVirusCheck)	
}


void CKEsdRemoteController_ServerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdLaunch_GlobalServer();
}
