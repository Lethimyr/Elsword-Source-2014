// DungeonDataDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "X2Tool.h"
#include "MainFrm.h"
#include "X2ToolDoc.h"
#include "X2ToolView.h"
#include "DungeonDataDlg.h"



// CDungeonDataDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDungeonDataDlg, CDialog)

CDungeonDataDlg::CDungeonDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDungeonDataDlg::IDD, pParent)
{

}

CDungeonDataDlg::~CDungeonDataDlg()
{
}

BOOL CDungeonDataDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
			return TRUE;		
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDungeonDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_USER_CAMERA, m_CheckUserCamera);
	DDX_Control(pDX, IDC_EDIT_LINENAME, m_editLineName);
	DDX_Control(pDX, IDC_EDIT_WORLDNAME, m_editWorldName);
	DDX_Control(pDX, IDC_EDIT_CAMERA_EYE, m_editCameraEye);
	DDX_Control(pDX, IDC_EDIT_CAMERA_LOOKAT, m_editCameraLookat);
	DDX_Control(pDX, IDC_EDIT1, m_editCameraSpeed);
	DDX_Control(pDX, IDC_CHECK_CANPLAY, m_checkPlay);
	DDX_Control(pDX, IDC_EDIT_DUNGEONNAME, m_editDungeonName);
}


BEGIN_MESSAGE_MAP(CDungeonDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDungeonDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_USER_CAMERA, &CDungeonDataDlg::OnBnClickedCheckUserCamera)
	ON_EN_CHANGE(IDC_EDIT_TOOL_WIDTH, &CDungeonDataDlg::OnEnChangeEditToolWidth)
	ON_EN_CHANGE(IDC_EDIT_TOOL_HEIGHT, &CDungeonDataDlg::OnEnChangeEditToolHeight)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_SIZE, &CDungeonDataDlg::OnBnClickedButtonChangeSize)
	ON_BN_CLICKED(IDC_RADIO_HIGH, &CDungeonDataDlg::OnBnClickedRadioQuality)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE, &CDungeonDataDlg::OnBnClickedRadioQuality)
	ON_BN_CLICKED(IDC_RADIO_LOW, &CDungeonDataDlg::OnBnClickedRadioQuality)
	ON_EN_CHANGE(IDC_EDIT1, &CDungeonDataDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK_CANPLAY, &CDungeonDataDlg::OnBnClickedCheckCanplay)
	ON_BN_CLICKED(IDC_BUTTON1, &CDungeonDataDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDungeonDataDlg 메시지 처리기입니다.

void CDungeonDataDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	CX2ToolView* pView = (CX2ToolView *)pMainWnd->GetActiveView();

	pView->ResetDungeon();
	

	// 최초 실행 시 불러지는 던전
	static bool bFirstDungeon = true;
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( bFirstDungeon == true )
	{
		pMain->LoadWorld( L"new_create_World.lua" );
		bFirstDungeon = false;
	}

	// 던전 시작 버튼 클릭으로 불려지는 던전
	CX2Dungeon::DungeonData* pDungeonData = (CX2Dungeon::DungeonData*)pMain->GetSelectedDungeonItem();
	if( pDungeonData != NULL )
	{		
		pMain->ResetTool( false, false, true, true, false );
		pMain->ResetGame( (SEnum::DUNGEON_ID)pDungeonData->m_DungeonID );
	}
	
}


void CDungeonDataDlg::OnBnClickedCheckUserCamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

	int iState = m_CheckUserCamera.GetCheck();
	switch(iState)
	{
	case BST_UNCHECKED:
		pMain->SetUserCamera(false);
		break;
	case BST_CHECKED:
		pMain->SetUserCamera(true);
		break;
	case BST_INDETERMINATE:
		pMain->SetUserCamera(false);
		break;
	}
}


void CDungeonDataDlg::OnEnChangeEditToolWidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}


void CDungeonDataDlg::OnEnChangeEditToolHeight()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}


void CDungeonDataDlg::OnBnClickedButtonChangeSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true || pMain->m_bUserCamera == true )
		return;

	CString csWidth;
	CString csHeight;
	GetDlgItem(IDC_EDIT_TOOL_WIDTH)->GetWindowText(csWidth);
	GetDlgItem(IDC_EDIT_TOOL_HEIGHT)->GetWindowText(csHeight);

	int iWidth = _ttoi(csWidth);
	int iHeight = _ttoi(csHeight);

	pMain->SetWindowSize(iWidth, iHeight);

	FILE *fp = fopen("X2ToolConfig.cfg", "w");
	if( fp != NULL )
	{
		fprintf(fp, "%d %d\n", iWidth, iHeight);
		if( pMain->m_bHeightMap == true )
			fprintf(fp, "1\n");
		else
			fprintf(fp, "0\n");

		string strLibPath;
		ConvertWCHARToChar(strLibPath, pMain->m_wstrLibPath.c_str());
		fprintf(fp, "%s\n", strLibPath.c_str());

		fclose(fp);
	}
}




void CDungeonDataDlg::OnBnClickedRadioQuality()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		
	int iChecked = GetCheckedRadioButton(IDC_RADIO_HIGH, IDC_RADIO_LOW);
	switch( iChecked )
	{
	case IDC_RADIO_HIGH:
		g_pMain->GetGameOption().SetMapDetail( CX2GameOption::OL_HIGH );
		break;
	case IDC_RADIO_MIDDLE:
		g_pMain->GetGameOption().SetMapDetail( CX2GameOption::OL_MEDIUM );
		break;
	case IDC_RADIO_LOW:
		g_pMain->GetGameOption().SetMapDetail( CX2GameOption::OL_LOW );
		break;
	default:
		g_pMain->GetGameOption().SetMapDetail( CX2GameOption::OL_HIGH );
		break;
	}
}


void CDungeonDataDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString csSpeed;
	GetDlgItem(IDC_EDIT1)->GetWindowText(csSpeed);

	float fSpeed = (float)_ttof(csSpeed);
	if( g_pX2Game != NULL && g_pX2Game->GetFPSCamera() != NULL )
	{
		g_pX2Game->GetFPSCamera()->SetScalers(0.01f, fSpeed);
	}
}


void CDungeonDataDlg::OnBnClickedCheckCanplay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

	int iState = m_checkPlay.GetCheck();
	switch(iState)
	{
	case BST_UNCHECKED:
		pMain->m_bCanPlay = false;
		break;
	case BST_CHECKED:
		pMain->m_bCanPlay = true;
		break;
	case BST_INDETERMINATE:
		pMain->m_bCanPlay = false;
		break;
	}

	g_pInstanceData->m_bCanDungeonPlay = pMain->m_bCanPlay;
	if( pMain->m_bCanPlay == true )
	{
		g_pMain->SetNowStateID( CX2Main::XS_DUNGEON_GAME );
	}
	else
	{
		g_pMain->SetNowStateID( CX2Main::XS_START_UP );
	}
}


void CDungeonDataDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	pToolMain->ChangeUnit();
}
