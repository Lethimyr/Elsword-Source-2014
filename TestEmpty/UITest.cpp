#include "dxstdafx.h"
//#include ".\x2statelogin.h"
#include ".\uitest.h"

CUITest::CUITest(void) : m_pDlgUiLoginFront( NULL ), m_pDlgUiLoginBack( NULL ), m_pCheckBoxLogin( NULL )
{

	//dbg::logfile::SetLogLevel(2);

	m_pDlgUiLoginBack = new CKTDGUIDialog( this, L"DLG_UI_Pvp_Make.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgUiLoginBack );
/*
	m_pDlgUiLoginFront = new CKTDGUIDialog( this, L"DLG_Login_Front.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgUiLoginBack );
	
	CKTDGUIButton* pLoginButton = m_pDlgUiLoginBack->GetButton_LUA( "ButtonLogin" );
	pLoginButton->SetShow( true );
	m_pDlgUiLoginBack->GetControl(L"IMELoginID")->RequestFocus();
*/	
	/*m_pCheckBoxLogin = (CKTDGUICheckBox*) m_pDlgUiLoginFront->GetCheckBox_LUA( "CheckBoxAutoLogin" );
	if ( m_pCheckBoxLogin != NULL )
		m_pCheckBoxLogin->SetChecked(true);
		*/
/*
	CKTDGUIStatic* m_pStaticLogin = (CKTDGUIStatic *) m_pDlgUiLoginBack->GetStatic_LUA( "Login" );
	if ( m_pStaticLogin != NULL )
	{
		m_pStaticLogin
	}*/
}

CUITest::~CUITest(void)
{
	/*if ( m_pCheckBoxLogin != NULL )
	{
		delete m_pCheckBoxLogin;
		m_pCheckBoxLogin = NULL;
	}
*/
	SAFE_DELETE_DIALOG( m_pDlgUiLoginBack );
}

HRESULT CUITest::OnFrameMove( double fTime, float fElapsedTime )
{
	//if(m_pDlgUiLoginBack != NULL && m_pDlgUiLoginBack->GetIsMouseOver() == true)
	//{
		return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );
	//}

	//return S_OK;
}

HRESULT CUITest::OnFrameRender( )
{
	g_pKTDXApp->GetDGManager()->ObjectChainSort();
	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
	g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();

	g_pKTDXApp->GetDGManager()->FrontUIRender();
	return S_OK;
}

HRESULT CUITest::OnResetDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
}

HRESULT CUITest::OnLostDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
}

bool CUITest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{    

	case CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT:
		{
			//if ( m_bIgnoreAllUIEvent == true )
				//return false;

			if ( UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
				return true;
		}
		break;

	}


	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	return false;
	/*
	bool bFlag = false;
	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		//case WM_MOUSEWHEEL:
		{
			if ( false == m_pDlgUiLoginBack->GetIsMouseOver())
			{
				bFlag = false;;//return false;
				break;
			}
			bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDlgUiLoginBack->GetIsMouseOver())
			{
				bFlag = false;
				break;
				//return false;
			}
			//g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_QUEST, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}
	*/

	//return bFlag;

	/*
	switch( uMsg )
	{    

	case CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT:
		{
			//if ( m_bIgnoreAllUIEvent == true )
			//return false;

			if ( UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
				return true;
		}
		break;
	}
	*/

/*
	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		bFlag = true;
		

	return bFlag;
	*/
}

bool CUITest::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case STUCM_CHECK1SET:
		MessageBox( hWnd, L"STUCM_CHECK1SET", L"STUCM_CHECK1SET", MB_OK );
		return true;

	case STUCM_CHECK1UNSET:
		MessageBox( hWnd, L"STUCM_CHECK1UNSET", L"STUCM_CHECK1SET", MB_OK );
		return true;
	}

	return false;
}

bool CUITest::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;
}