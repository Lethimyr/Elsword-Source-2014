#include "dxstdafx.h"
#include ".\forfeittest.h"

CForfeitTest::CForfeitTest(void)
{
	//m_pDLGTestBack = new CKTDGUIDialog( this, L"DLG_Forfeit_Test_Back.lua" );
	//g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTestBack );

	m_pDLGTestFront = new CKTDGUIDialog( this, L"DLG_Forfeit_Test_Front.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTestFront );

	m_NowTestMode = STM_BG1;
	CKTDGUICheckBox* pCheckBox;
	pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox1" );
	//if ( pCheckBox != NULL )
	//	pCheckBox->SetChecked(true);
	
	pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox2" );
	//pCheckBox->SetChecked(true);
	pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox3" );
	//pCheckBox->SetChecked(true);
	//pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox4" );
	//pCheckBox->SetChecked(true);

	//CKTDGUIStatic* pStatic_BG;
	//pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic3" );
	//pStatic_BG->SetShow(false);
	//pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic4" );
	//pStatic_BG->SetShow(false);

	CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*) m_pDLGTestFront->GetRadioButton_LUA( "RadioButton1" );
	pRadio->SetChecked(true);
	pRadio = (CKTDGUIRadioButton*) m_pDLGTestFront->GetRadioButton_LUA( "RadioButton2" );
	pRadio->SetChecked(true);
	//pRadio->
	//pRadio->SetChecked( FALSE );
/*
	//CKTDGUIIMEEditBox* pIME = 
	m_pDLGTestBack->GetIMEEdieBox_LUA( "IMETest" )->RequestFocus();

	//combobox initialize
	CKTDGUIComboBox* pCombo_Graphic_Resoultion = (CKTDGUIComboBox*) m_pDLGTestBack->GetComboBox_LUA("Option_Resolution");
	//CKTDGUIComboBox* pCombo_Graphic_Resoultion = (CKTDGUIComboBox*) m_pDLGTestBack->GetControl( L"Option_Resolution" );
	unsigned long a = 1;
	unsigned long b = 2;
	pCombo_Graphic_Resoultion->RemoveAllItems();
	pCombo_Graphic_Resoultion->AddItem( L"a", ULongToPtr(a));
	pCombo_Graphic_Resoultion->AddItem( L"b", ULongToPtr(b));

	pCombo_Graphic_Resoultion->SetDropHeight( 10 * pCombo_Graphic_Resoultion->GetNumItems() );

	//ListBox
	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGTestBack->GetListBox_LUA("ListBox_Test");
	pListBox->RemoveAllItems();
	pListBox->AddItem(L"a", ULongToPtr(a));
	pListBox->AddItem(L"b", ULongToPtr(b));
	
*/

}

CForfeitTest::~CForfeitTest(void)
{
	//SAFE_DELETE_DIALOG( m_pDLGTestBack );
	SAFE_DELETE_DIALOG( m_pDLGTestFront );
}

HRESULT CForfeitTest::OnFrameMove( double fTime, float fElapsedTime )
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );
	
	return S_OK;
}

HRESULT CForfeitTest::OnFrameRender(){
	
	g_pKTDXApp->GetDGManager()->ObjectChainSort();

	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
	g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
	g_pKTDXApp->GetDGManager()->FrontUIRender();


	return S_OK;
}

HRESULT CForfeitTest::OnResetDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
}

HRESULT CForfeitTest::OnLostDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
}

bool CForfeitTest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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

}



bool CForfeitTest::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	switch( wParam )
	{
	case STUCM_RADIO1SET:
		{
			m_NowTestMode = STM_BG1;
			CKTDGUICheckBox* pCheckBox;
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox1" );
			pCheckBox->SetEnable(true);
			pCheckBox->SetShow(true);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox2" );
			pCheckBox->SetEnable(true);
			pCheckBox->SetShow(true);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox3" );
			pCheckBox->SetEnable(true);
			pCheckBox->SetShow(true);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox4" );
			pCheckBox->SetEnable(true);
			pCheckBox->SetShow(true);

		}
		break;	
	case STUCM_RADIO2SET:
		{
			m_NowTestMode = STM_BG2;
			CKTDGUICheckBox* pCheckBox;
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox1" );
			pCheckBox->SetEnable(false);
			pCheckBox->SetShow(false);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox2" );
			pCheckBox->SetEnable(false);
			pCheckBox->SetShow(false);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox3" );
			pCheckBox->SetEnable(false);
			pCheckBox->SetShow(false);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox4" );
			pCheckBox->SetEnable(false);
			pCheckBox->SetShow(false);

		}
		break;
	case STUCM_RADIO3SET:
		{
			m_NowTestMode = STM_BG3;
			CKTDGUICheckBox* pCheckBox;
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox1" );
			pCheckBox->SetEnable(true);
			pCheckBox->SetShow(true);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox2" );
			pCheckBox->SetEnable(true);
			pCheckBox->SetShow(false);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox3" );
			pCheckBox->SetEnable(false);
			pCheckBox->SetShow(true);
			pCheckBox = (CKTDGUICheckBox*) m_pDLGTestFront->GetCheckBox_LUA( "CheckBox4" );
			pCheckBox->SetEnable(false);
			pCheckBox->SetShow(false);
		}
		break;
/*
	case STUCM_CHECK1SET:
		{

			CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic1" );
			pStatic_BG->SetShow(true);

		}
		break;
	case STUCM_CHECK2SET:
		{
			CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic2" );
			pStatic_BG->SetShow(true);

		}
		break;
	case STUCM_CHECK3SET:
		{
			CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic3" );
			pStatic_BG->SetShow(true);

		}
		break;
	case STUCM_CHECK4SET:
		{
			CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic4" );
			pStatic_BG->SetShow(true);

		}
		break;

	case STUCM_CHECK1UNSET:
		{
			CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic1" );
			pStatic_BG->SetShow(false);

		}
		break;
	case STUCM_CHECK2UNSET:
		{
			CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic2" );
			pStatic_BG->SetShow(false);

		}
		break;
	case STUCM_CHECK3UNSET:
		{
			CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic3" );
			pStatic_BG->SetShow(false);

		}
		break;
	case STUCM_CHECK4UNSET:
		{
			CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGTestBack->GetControl( L"Test_Back_Pic4" );
			pStatic_BG->SetShow(false);

		}
		break;
	case STUCM_BUTTONON:
		{
			WCHAR* OMG;
			OMG = (WCHAR*) m_pDLGTestBack->GetIMEEdieBox_LUA( "IMETest" )->GetText();
			//SetUserID( pIDEditBox->GetText() );
			//void SetUserID( const WCHAR* pUserID ){ m_UserID = pUserID; }

            MessageBox(NULL, OMG, L"OMG", MB_OK);

			break;
		}
	case STUCM_COMBO_SELECT:
		{

			CKTDGUIComboBox* pCombo_Graphic_Resoultion = (CKTDGUIComboBox*) m_pDLGTestBack->GetControl( L"Option_Resolution" );

			unsigned long getvalue;
			getvalue = PtrToUlong( pCombo_Graphic_Resoultion->GetSelectedData());

			WCHAR buf_tmp[20];
			StringCchPrintf( buf_tmp, 20, L"%d", getvalue );
			MessageBox( NULL, buf_tmp, L"Combo", MB_OK);


			break;

		}
	case STUCM_LIST_SELECT:
		{
			CKTDGUIListBox* pList_Test = (CKTDGUIListBox*) m_pDLGTestBack->GetControl( L"ListBox_Test" );

			unsigned long getvalue;
			getvalue = pList_Test->GetSelectedIndex();

			WCHAR buf_tmp[20];
			StringCchPrintf( buf_tmp, 20, L"Index : %d", getvalue );
			MessageBox( NULL, buf_tmp, L"List", MB_OK);

			break;
		}
		*/
	default:
		return false;

	}
	return true;
}

bool CForfeitTest::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{

	}
	return false;
}

