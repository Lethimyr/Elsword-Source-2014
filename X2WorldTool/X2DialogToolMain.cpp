#include "dxstdafx.h"
#include ".\x2dialogtoolmain.h"

// to do : 
// static, picture, editbox 추가
// 각 control의 텍스쳐 및 위치 좌표 텍스트로 입력할 수 있도록 
//

void CALLBACK CX2DialogToolMain_OnDialogEvent0( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2DialogToolMain* pMain = (CX2DialogToolMain*) pUserContext;

	//switch( nControlID )
	//{
	////case CX2DialogToolMain::IDC_BUTTON_SHOW_DIALOG:
	////	{
	////		pMain->GetShowDialog() = !pMain->GetShowDialog();
	////	} break;

	//default: 
	//	break;
	//}
}

void CALLBACK CX2DialogToolMain_OnDialogEvent1( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	CX2DialogToolMain* pMain = (CX2DialogToolMain*) pUserContext;

	switch( nControlID )
	{
	case CX2DialogToolMain::IDC_BUTTON_FILE_OPEN:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"dialog script\0*.lua\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetOpenFileName(&ofn) == TRUE ) 
			{
				pMain->OpenScript( ofn.lpstrFile );
				pMain->UpdateControlListBox();
			}
		} break;

	case CX2DialogToolMain::IDC_BUTTON_FILE_SAVE:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"dialog script\0*.lua\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetSaveFileName(&ofn) == TRUE ) 
			{
				pMain->SaveScritp2File( ofn.lpstrFile );				
			}
		} break;

	case CX2DialogToolMain::IDC_BUTTON_ADD_CONTROL:
		{
			CDXUTComboBox* pCombo = pMain->GetDialog( CX2DialogToolMain::DLG_DIALOG0 )->GetComboBox( CX2DialogToolMain::IDC_COMBO_CONTROL_TYPE );
			
			string strFileName;
			if( 0 == StrCmpW( L"Button", pCombo->GetSelectedItem()->strText ) )
			{
				strFileName = "X2DialogTool_Button.lua";
			}
			else if( 0 == StrCmpW( L"CheckBox", pCombo->GetSelectedItem()->strText ) )
			{		
				strFileName = "X2DialogTool_CheckBox.lua";
			}
			else if( 0 == StrCmpW( L"EditBox", pCombo->GetSelectedItem()->strText ) )
			{
				strFileName = "X2DialogTool_EditBox.lua";
			}
			else if( 0 == StrCmpW( L"IMEEditBox", pCombo->GetSelectedItem()->strText ) )
			{
				strFileName = "X2DialogTool_IMEEditBox.lua";
			}
			else if( 0 == StrCmpW( L"Static", pCombo->GetSelectedItem()->strText ) )
			{
				strFileName = "X2DialogTool_Static.lua";
			}
			else if( 0 == StrCmpW( L"RadioButton", pCombo->GetSelectedItem()->strText ) )
			{
				strFileName = "X2DialogTool_RadioButton.lua";
			}
			else if( 0 == StrCmpW( L"ComboBox", pCombo->GetSelectedItem()->strText ) )
			{
				strFileName = "X2DialogTool_ComboBox.lua";
			}
			else if( 0 == StrCmpW( L"ListBox", pCombo->GetSelectedItem()->strText ) )
			{
				strFileName = "X2DialogTool_ListBox.lua";
			}
			else if( 0 == StrCmpW( L"Slider", pCombo->GetSelectedItem()->strText ) )
			{
				strFileName = "X2DialogTool_Slider.lua";
			}
			//else if( 0 == StrCmpW( L"Picture", pCombo->GetSelectedItem()->strText ) )
			//{
			//	if( NULL == pMain->GetPickedControl() || 
			//		CKTDGUIControl::UCT_STATIC != pMain->GetPickedControl()->GetType() )
			//		break;
			//	else
			//	{
			//		strFileName = "X2DialogTool_Picture.lua";
			//		lua_tinker::decl( "g_pStatic0", pMain->GetPickedControl() );
			//	}
			//}
			else
			{
				break;
			}

			if( NULL == pMain->GetDialog() )
				pMain->CreateNewDialog();


			lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIDialog", pMain->GetDialog() );

			KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
			Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( strFileName.c_str() );
			if( Info != NULL )
			{
				if( E_FAIL == g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) )
				{						
					break;
				}
			}

			pMain->UpdateControlListBox();
		} break;
	
	case CX2DialogToolMain::IDC_BUTTON_REMOVE_ALL_CONTROL:
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ClearAllDlg();
			pMain->SetDialog( NULL );
			pMain->UpdateControlListBox();
		} break;

	case CX2DialogToolMain::IDC_LIST_CONTROLS:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_ITEM_DBLCLK:
			case EVENT_LISTBOX_SELECTION:
				{
					DXUTListBoxItem* pItem = ((CDXUTListBox *)pControl)->GetSelectedItem();
					CKTDGUIControl* pControl = (CKTDGUIControl*) pItem->pData;

					CDXUTEditBox* pEditBox = pMain->GetDialog( CX2DialogToolMain::DLG_DIALOG0 )->GetEditBox( CX2DialogToolMain::IDC_EDIT_CONTROL_NAME );
					pEditBox->SetText( pControl->GetName() );
					pMain->SetPickedControl( pControl );
				} break;
			} 
		} break;

	case CX2DialogToolMain::IDC_EDIT_CONTROL_NAME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
			//case EVENT_EDITBOX_CHANGE:
				{
					CKTDGUIControl* pGUIControl = pMain->GetPickedControl();
					if( NULL == pMain->GetDialog() || NULL == pGUIControl )
						break;

					pGUIControl->SetName( ((CDXUTEditBox*)pControl)->GetText() );
					pMain->UpdateControlListBox();
				} break;
			}
		} break;

	default:
		break;
	}
}

void CALLBACK CX2DialogToolMain_OnDialogEvent2( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
//	//	OPENFILENAME ofn;        // common dialog box structure
//	//	WCHAR wszFileName[512];  // path까지 포함한 파일 이름
//	//	WCHAR wszOutput[512];
//
//	CX2DialogToolMain* pWorldTool = (CX2DialogToolMain*) pUserContext;
//
//	switch( nControlID )
//	{
//	case CX2DialogToolMain::IDC_BUTTON_ADD_XMESH:
//		{
//			pWorldTool->AddXMesh2World();
//			pWorldTool->UpdateWorldObjectListBox();
//		} break;
//	}
}

void CALLBACK CX2DialogToolMain_OnTextureSamplingDialogEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	CX2DialogToolMain* pMain = (CX2DialogToolMain*) pUserContext;

	switch( nControlID )
	{
	case CX2DialogToolMain::IDC_BUTTON_TEXTURE_FILE_OPEN:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"texture file\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetOpenFileName(&ofn) == TRUE ) 
			{
				pMain->GetTexturedRectangle().ReleaseTexture();
				pMain->GetTexturedRectangle().CreateTexture( ofn.lpstrFile );				
			}
		} break;

	case CX2DialogToolMain::IDC_BUTTON_TEXTURE_COORD_TO_NOTEPAD:
		{
			const string MAGIC_TEMP_FILE_PATH = "C:\\다이얼로그툴_임시파일.txt";
			std::ofstream ofs;
			ofs.open( MAGIC_TEMP_FILE_PATH.c_str() );			

			string strTextureName;
			ConvertWCHARToChar( strTextureName, pMain->GetTexturedRectangle().GetTextureName().c_str() );
			const D3DXVECTOR2* pTexCoords = pMain->GetTexCoords();
			D3DXVECTOR2 pTexUV[4];
			for( int i=0; i<4; i++ )
			{
				pTexUV[i].x = (float)(int)pTexCoords[i].x;
				pTexUV[i].y = (float)(int)pTexCoords[i].y;
			}

			ofs << std::endl;
			ofs << std::endl;
			ofs << "{" << std::endl; 
			ofs << "\tTEX_NAME       \t= \"" << strTextureName << "\"," << std::endl; 
			ofs << "\t\"LEFT_TOP     \t= D3DXVECTOR2( " << pTexUV[0].x << ", " << pTexUV[0].y << " )\"," << std::endl; 
			ofs << "\t\"RIGHT_TOP    \t= D3DXVECTOR2( " << pTexUV[1].x << ", " << pTexUV[1].y << " )\"," << std::endl; 
			ofs << "\t\"LEFT_BOTTOM  \t= D3DXVECTOR2( " << pTexUV[2].x << ", " << pTexUV[2].y << " )\"," << std::endl; 
			ofs << "\t\"RIGHT_BOTTOM \t= D3DXVECTOR2( " << pTexUV[3].x << ", " << pTexUV[3].y << " )\"," << std::endl; 
			ofs << "}" << std::endl;
			ofs << std::endl;
			ofs << std::endl;
			ofs.close();

			_spawnlp( _P_NOWAIT, "notepad.exe", "notepad.exe", MAGIC_TEMP_FILE_PATH.c_str(), NULL );
		} break;

	case CX2DialogToolMain::IDC_EDIT_TEX_LEFT_TOP:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					pMain->SetLastFocusedEditBox( (CDXUTEditBox*)pControl );
				} break;
			case EVENT_EDITBOX_STRING:
				{
					float fTemp[2];
					CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;
					pEditBox->ParseFloatArray( (float*)&fTemp[0], 2 );
					D3DXVECTOR2* pTexUV = pMain->GetTexCoords();
					pTexUV[0].x = fTemp[0];
					pTexUV[0].y = fTemp[1];

					D3DXVECTOR3 vMouse(fTemp[0], fTemp[1], 1.f );
					vMouse.x /= pMain->GetTexturedRectangle().GetWidth() / pMain->GetTexturedRectangle().GetRectSize();
					vMouse.y /= pMain->GetTexturedRectangle().GetHeight() / pMain->GetTexturedRectangle().GetRectSize();
					vMouse.y = pMain->GetTexturedRectangle().GetRectSize() - vMouse.y;
					pMain->GetWiredRect().SetVertexPos( 0, vMouse );			

					pMain->SetLastFocusedEditBox( (CDXUTEditBox*)pControl );
				} break;
			}
		} break;

	case CX2DialogToolMain::IDC_EDIT_TEX_RIGHT_TOP:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					pMain->SetLastFocusedEditBox( (CDXUTEditBox*)pControl );
				} break;
			case EVENT_EDITBOX_STRING:
				{
					float fTemp[2];
					CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;
					pEditBox->ParseFloatArray( (float*)&fTemp[0], 2 );
					D3DXVECTOR2* pTexUV = pMain->GetTexCoords();
					pTexUV[1].x = fTemp[0];
					pTexUV[1].y = fTemp[1];

					D3DXVECTOR3 vMouse(fTemp[0], fTemp[1], 1.f );
					vMouse.x /= pMain->GetTexturedRectangle().GetWidth() / pMain->GetTexturedRectangle().GetRectSize();
					vMouse.y /= pMain->GetTexturedRectangle().GetHeight() / pMain->GetTexturedRectangle().GetRectSize();
					vMouse.y = pMain->GetTexturedRectangle().GetRectSize() - vMouse.y;
					pMain->GetWiredRect().SetVertexPos( 1, vMouse );			

					pMain->SetLastFocusedEditBox( (CDXUTEditBox*)pControl );
				} break;
			}
		} break;

	case CX2DialogToolMain::IDC_EDIT_TEX_LEFT_BOTTOM:	
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					pMain->SetLastFocusedEditBox( (CDXUTEditBox*)pControl );
				} break;
			case EVENT_EDITBOX_STRING:
				{
					float fTemp[2];
					CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;
					pEditBox->ParseFloatArray( (float*)&fTemp[0], 2 );
					D3DXVECTOR2* pTexUV = pMain->GetTexCoords();
					pTexUV[2].x = fTemp[0];
					pTexUV[2].y = fTemp[1];

					D3DXVECTOR3 vMouse(fTemp[0], fTemp[1], 1.f );
					vMouse.x /= pMain->GetTexturedRectangle().GetWidth() / pMain->GetTexturedRectangle().GetRectSize();
					vMouse.y /= pMain->GetTexturedRectangle().GetHeight() / pMain->GetTexturedRectangle().GetRectSize();
					vMouse.y = pMain->GetTexturedRectangle().GetRectSize() - vMouse.y;
					pMain->GetWiredRect().SetVertexPos( 2, vMouse );			

					pMain->SetLastFocusedEditBox( (CDXUTEditBox*)pControl );
				} break;
			}
		} break;

	case CX2DialogToolMain::IDC_EDIT_TEX_RIGHT_BOTTOM:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					pMain->SetLastFocusedEditBox( (CDXUTEditBox*)pControl );
				} break;
			case EVENT_EDITBOX_STRING:
				{
					float fTemp[2];
					CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;
					pEditBox->ParseFloatArray( (float*)&fTemp[0], 2 );
					D3DXVECTOR2* pTexUV = pMain->GetTexCoords();
					pTexUV[3].x = fTemp[0];
					pTexUV[3].y = fTemp[1];

					D3DXVECTOR3 vMouse(fTemp[0], fTemp[1], 1.f );
					vMouse.x /= pMain->GetTexturedRectangle().GetWidth() / pMain->GetTexturedRectangle().GetRectSize();
					vMouse.y /= pMain->GetTexturedRectangle().GetHeight() / pMain->GetTexturedRectangle().GetRectSize();
					vMouse.y = pMain->GetTexturedRectangle().GetRectSize() - vMouse.y;
					pMain->GetWiredRect().SetVertexPos( 3, vMouse );			

					pMain->SetLastFocusedEditBox( (CDXUTEditBox*)pControl );
				} break;
			}
		} break;

	}
}

void CX2DialogToolMain::FinalDialog()
{
	for( unsigned i=0; i<m_vecpDialog.size(); i++ )
	{
		SAFE_DELETE( m_vecpDialog[i] );
	}
	m_vecpDialog.clear();

	SAFE_DELETE( m_pTextureSamplingDialog );
}

void CX2DialogToolMain::InitDialog()
{
	// initialize UI
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CDXUTDialog* pDlg = NULL;

	pDlg = new CDXUTDialog();
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( CX2DialogToolMain_OnDialogEvent0, this );
	pDlg->SetSize( 10, 300 );
	pDlg->SetFont(0, L"굴림", 12, 12);
	
	int iX = 10;
	int iY = 10;
	int iDX = 100;
	//pDlg->AddButton( IDC_BUTTON_SHOW_DIALOG, L"SHOW", iX, iY, 100, 22 );
	    

	pDlg = new CDXUTDialog(); // DLG_DIALOG0
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( CX2DialogToolMain_OnDialogEvent1, this );
	pDlg->SetSize( 200, 300 );
	pDlg->SetFont(0, L"굴림", 12, 12);

	pDlg->AddButton( IDC_BUTTON_FILE_OPEN, L"LOAD", iX, iY, 100, 22 );
	iY += 25;
	pDlg->AddButton( IDC_BUTTON_FILE_SAVE, L"SAVE", iX, iY, 100, 22 );
	iY += 25;
	pDlg->AddButton( IDC_BUTTON_ADD_CONTROL, L"add control", iX, iY, 100, 22 );
	iY += 25;
	pDlg->AddButton( IDC_BUTTON_REMOVE_ALL_CONTROL, L"remove all", iX, iY, 100, 22 );
	iY += 25;

	CDXUTComboBox *pCombo = NULL;
	pDlg->AddComboBox( IDC_COMBO_CONTROL_TYPE, iX, iY, 160, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 100 );
		pCombo->AddItem( L"Button", (LPVOID) 0 );
		pCombo->AddItem( L"CheckBox", (LPVOID) 0 );
		//pCombo->AddItem( L"EditBox", (LPVOID) 0 );
		//pCombo->AddItem( L"IMEEditBox", (LPVOID) 0 );
		pCombo->AddItem( L"Static", (LPVOID) 0 );
		//pCombo->AddItem( L"RadioButton", (LPVOID) 0 );
		//pCombo->AddItem( L"ComboBox", (LPVOID) 0 );
		pCombo->AddItem( L"ListBox", (LPVOID) 0 );
		//pCombo->AddItem( L"Slider", (LPVOID) 0 );
		//pCombo->AddItem( L"Picture", (LPVOID) 0 );
	}
	iY += 30;

	pDlg->AddEditBox( IDC_EDIT_CONTROL_NAME, L"", iX, iY, 200, 30 );
	iY += 35;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"controls", iX, iY, 320, 22);
	iY += 25;	

	CDXUTListBox* pListBox = NULL;
	pDlg->AddListBox( IDC_LIST_CONTROLS, iX, iY, 320, 200, 0, &pListBox );
	//pListBox->SetStyle( CDXUTListBox::MULTISELECTION );

	iY += 220;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"LeftTop", iX, iY, iDX-10, 22 );
	pDlg->AddEditBox( IDC_EDIT_POINT_LEFT_TOP, L"", iX+iDX, iY, 200, 30 );
	iY += 35;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"RightTop", iX, iY, iDX-10, 22 );
	pDlg->AddEditBox( IDC_EDIT_POINT_RIGHT_TOP, L"", iX+iDX, iY, 200, 30 );
	iY += 35;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"LeftBottom", iX, iY, iDX-10, 22 );
	pDlg->AddEditBox( IDC_EDIT_POINT_LEFT_BOTTOM, L"", iX+iDX, iY, 200, 30 );
	iY += 35;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"RightBottom", iX, iY, iDX-10, 22 );
	pDlg->AddEditBox( IDC_EDIT_POINT_RIGHT_BOTTOM, L"", iX+iDX, iY, 200, 30 );
	iY += 35;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"Color", iX, iY, iDX-10, 22 );
	pDlg->AddEditBox( IDC_EDIT_POINT_COLOR, L"", iX+iDX, iY, 200, 30 );
	iY += 35;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"ChangeTime", iX, iY, iDX-10, 22 );
	pDlg->AddEditBox( IDC_EDIT_POINT_CHANGE_TIME, L"", iX+iDX, iY, 200, 30 );
	iY += 35;

	pDlg = new CDXUTDialog(); // DLG_CONTROL_BUTTON
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( CX2DialogToolMain_OnDialogEvent2, this );
	pDlg->SetSize( 200, 300 );
	pDlg->SetFont(0, L"굴림", 12, 12);

	iY = 10;
	iX = 10;
	pDlg->AddButton( IDC_BUTTON_BUTTON_SET_NORMAL_TEXTURE, L"SetNormalTex", iX, iY, 100, 22 );
	iY += 25;
	pDlg->AddButton( IDC_BUTTON_BUTTON_SET_OVER_TEXTURE, L"SetOverTex", iX, iY, 100, 22 );
	iY += 25;
	pDlg->AddButton( IDC_BUTTON_BUTTON_SET_DOWN_TEXTURE, L"SetDownTex", iX, iY, 100, 22 );


	// 텍스쳐 샘플링 모드에서 보이는 다이얼로그
	pDlg = new CDXUTDialog();
	m_pTextureSamplingDialog = pDlg;
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( CX2DialogToolMain_OnTextureSamplingDialogEvent, this );
	pDlg->SetSize( 200, 300 );
	pDlg->SetFont(0, L"굴림", 12, 12);

	iX = 10;
	iY = 10;	
	pDlg->AddButton( IDC_BUTTON_TEXTURE_FILE_OPEN, L"open texture", iX, iY, 100, 22 );
	iY += 25;
	pDlg->AddButton( IDC_BUTTON_TEXTURE_COORD_TO_NOTEPAD, L"notepad", iX, iY, 100, 22 );
	iY += 25;
	pDlg->AddEditBox( IDC_EDIT_TEX_LEFT_TOP, L"", iX, iY, 200, 30 );
	iY += 35;
	pDlg->AddEditBox( IDC_EDIT_TEX_RIGHT_TOP, L"", iX, iY, 200, 30 );
	iY += 35;
	pDlg->AddEditBox( IDC_EDIT_TEX_LEFT_BOTTOM, L"", iX, iY, 200, 30 );
	iY += 35;
	pDlg->AddEditBox( IDC_EDIT_TEX_RIGHT_BOTTOM, L"", iX, iY, 200, 30 );
	iY += 35;
}

CX2DialogToolMain::CX2DialogToolMain(void)
{
	char szWindowTitle[1024];
	sprintf( szWindowTitle, "X2DialogTool v0.51; compiled at %s, %s", __DATE__, __TIME__ );
	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );

	m_bRButtonDragging = false;
	m_pLastFocusedEditBox = NULL;
	m_iMode = MODE_NORMAL;
	m_pControlDragging = NULL;
	m_pControlPicked = NULL;
	m_bShowDialog = false;
	m_pLuaManager = NULL;
	m_pDialog = new CKTDGUIDialog( this, L"NewDialog" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDialog );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"Enum.lua" );
	if( Info == NULL )
		return;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
		return;

	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"DungeonEnum.lua" );
	if( Info == NULL )
		return;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
		return;

	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"enum_fonts.lua" );
	if( Info == NULL )
		return;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
		return;

	g_pKTDXApp->RegisterLuabind( g_pKTDXApp->GetLuaBinder() );
    	
	InitDialog();

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pKTDXApp->SetWorldTransform( &matWorld );
	g_pKTDXApp->GetDGManager()->GetCamera()->Point( 0,0,-700, 0,0,0 );
	g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera()->GetViewMatrix() );
	//g_pKTDXApp->GetDevice()->SetTransform( D3DTS_VIEW, (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera()->GetViewMatrix() );
	g_pKTDXApp->GetDGManager()->SetProjection( 0.01f, 12000.f, false );

//	D3DXMATRIX matProj;
//	D3DXMatrixOrthoLH( &matProj, 1024, 1024, 0.01f, 12000.f );
//	g_pKTDXApp->GetDevice()->SetTransform( D3DTS_PROJECTION, &matProj );
}

CX2DialogToolMain::~CX2DialogToolMain(void)
{
	FinalDialog();
}

HRESULT CX2DialogToolMain::OnFrameMove( double fTime, float fElapsedTime )
{
	switch( m_iMode )
	{
	case MODE_NORMAL:
		{
			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F5) == TRUE )
			{
				OpenScript();
			}

			g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

		} break;

	case MODE_TEXTURE_SAMPLING:
		{
			
		} break;
	}
	return S_OK;
}

HRESULT CX2DialogToolMain::OnFrameRender()
{
	switch( m_iMode )
	{
	case MODE_NORMAL:
		{	
			g_pKTDXApp->GetDGManager()->ObjectChainSort();
			g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
			g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
			g_pKTDXApp->GetDGManager()->FrontUIRender();

			if( true == m_bShowDialog )
			{
				for( unsigned i=0; i<m_vecpDialog.size(); i++ )
				{
					m_vecpDialog[i]->OnRender( g_pKTDXApp->GetElapsedTime() );
				}
			}
		} break;

	case MODE_TEXTURE_SAMPLING:
		{
			m_TexturedRectangle.OnFrameRender();
			
			//CKTDGStateManager::PushRenderState( D3DRS_POINTSCALEENABLE, TRUE );
			//CKTDGStateManager::PushRenderState( D3DRS_POINTSIZE, 20 );
			//CKTDGStateManager::PushRenderState( D3DRS_POINTSCALE_A, 10 );
			CKTDGStateManager::PushRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
			CKTDGStateManager::PushRenderState( D3DRS_CULLMODE, D3DCULL_NONE );				
			//CKTDGStateManager::PushRenderState( D3DRS_LIGHTING, FALSE );
			m_WiredRect.OnFrameRender();			
			//CKTDGStateManager::PopRenderState( D3DRS_LIGHTING );
			CKTDGStateManager::PopRenderState( D3DRS_CULLMODE );
			CKTDGStateManager::PopRenderState( D3DRS_FILLMODE );			
			//CKTDGStateManager::PopRenderState( D3DRS_POINTSCALE_A );
			//CKTDGStateManager::PopRenderState( D3DRS_POINTSIZE );
			//CKTDGStateManager::PopRenderState( D3DRS_POINTSCALEENABLE );			

			m_pTextureSamplingDialog->OnRender( g_pKTDXApp->GetElapsedTime() );
			
		} break;
	}

	return S_OK;
}

HRESULT CX2DialogToolMain::OnResetDevice()
{
	int iX = 10;
	for( unsigned i=0; i<m_vecpDialog.size(); i++ )
	{
		m_vecpDialog[i]->SetLocation( iX, 50 );
		iX += m_vecpDialog[i]->GetWidth()+10;
	}
	m_pTextureSamplingDialog->SetLocation( 10, 50 );

	m_TexturedRectangle.OnResetDevice();
	m_WiredRect.OnResetDevice();
	
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
}

HRESULT CX2DialogToolMain::OnLostDevice()
{
	m_TexturedRectangle.OnLostDevice();
	m_WiredRect.OnLostDevice();

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
}

bool CX2DialogToolMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{	
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_TAB:
			{
				if( MODE_NORMAL == m_iMode )
					m_iMode = MODE_TEXTURE_SAMPLING;
				else
					m_iMode = MODE_NORMAL;
			} break;
		} break;
	}


	switch( m_iMode )
	{
	case MODE_NORMAL:
		{
			switch( uMsg )
			{	
			case WM_KEYDOWN:
				switch( wParam )
				{
				case VK_DELETE:
					{
					} break;

				case VK_SPACE:
					{
						m_bShowDialog = !m_bShowDialog;
					} break;

				} break;

				//case WM_CHAR:
				//	switch(wParam)
				//	{
				//	//case 'c':
				//	//case 'C':
				//	//	{
				//	//		g_pKTDXApp->GetDGManager()->GetDialogManager()->ClearAllDlg();	
				//	//		m_pDialog = NULL;
				//	//		UpdateControlListBox();
				//	//	} break;

				//	//case 'a':
				//	//	{
				//	//		CKTDGUIDialogManager* pDialogManager = g_pKTDXApp->GetDGManager()->GetDialogManager();
				//	//		vector<CKTDGUIDialogType>& dialogList = pDialogManager->GetDialogList();
				//	//		for( unsigned i=0; i<dialogList.size(); i++ )
				//	//		{
				//	//			CKTDGUIDialogType pDialog = dialogList[i];			
				//	//			for( unsigned j=0; j<dialogList.size(); j++ )
				//	//			{
				//	//				CKTDGUIControl* pControl = pDialog->GetControl(j);
				//	//				if( NULL != pControl )
				//	//				{						
				//	//					pControl->Move( D3DXVECTOR2(-10,-30), D3DXCOLOR(1,1,1,1), 0.000001f, false, false );
				//	//				}
				//	//				else
				//	//					break;
				//	//			}
				//	//		}
				//	//	} break;

				//	} break; // WM_CHAR

			case WM_RBUTTONDOWN:
				{
					POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

					m_pControlDragging = NULL;

					CKTDGUIDialogManager* pDialogManager = g_pKTDXApp->GetDGManager()->GetDialogManager();
					vector<CKTDGUIDialogType>& dialogList = pDialogManager->GetDialogList();
					for( unsigned i=0; i<dialogList.size(); i++ )
					{
						CKTDGUIDialogType pDialog = dialogList[i];

						vector<CKTDGUIControl*>& controlList = pDialog->GetControlList();
						for( unsigned j=0; j<controlList.size(); j++ )
						{
							CKTDGUIControl* pControl = controlList[j];					
							if( true == pControl->IsOnMouseOver() )
							{
								m_pControlDragging = pControl;
								m_pControlPicked = pControl;
								UpdateControlListBox();
								break;
							}
							else if( pControl->GetType() == CKTDGUIControl::UCT_STATIC )
							{
								D3DXVECTOR2 offsetPos;
								offsetPos = g_pKTDXApp->ConvertByResolution( pControl->GetOffsetPos() );

								POINT mousePointFinal = mousePoint;
								mousePointFinal.x -= (LONG)offsetPos.x;
								mousePointFinal.y -= (LONG)offsetPos.y;

								D3DXVECTOR2 mousePos = g_pKTDXApp->MouseConvertByResolution( (float)mousePointFinal.x, (float)mousePointFinal.y );
								mousePointFinal.x = (LONG)mousePos.x;
								mousePointFinal.y = (LONG)mousePos.y;

								CKTDGUIStatic* pStatic = (CKTDGUIStatic*) pControl;

								for( int i=0; i<100; i++ )
								{
									CKTDGUIControl::CPictureData* pPicture = pStatic->GetPicture(i);
									if( NULL == pPicture )
										break;

									if( true == Pick2DRect( mousePointFinal, 
										pPicture->pPoint->leftTopPoint, 
										pPicture->pPoint->rightTopPoint,
										pPicture->pPoint->leftBottomPoint,
										pPicture->pPoint->rightBottomPoint ) &&
										//pControl->GetEnable() == true &&
										pControl->GetShow() == true )
									{
										m_pControlDragging = pControl;
										m_pControlPicked = pControl;
										UpdateControlListBox();
										break;
									}						
								}

								//	if( pControl->ContainsPoint( mousePoint ) ) 
								//	{
									//m_pControlDragging = pControl;
									//m_pControlPicked = pControl;
									//UpdateControlListBox();
									//break;
							//	}
							}
						}

						if( NULL != m_pControlDragging )
						{
							break;
						}		
					}
				} break;

			case WM_RBUTTONUP:
				{			
					m_pControlDragging = NULL;
				} break;

			case WM_MOUSEMOVE:
				{
					POINTS mousepos = MAKEPOINTS( lParam );
					if( NULL != m_pControlDragging )
					{				
						D3DXVECTOR2 vOffsetPos = D3DXVECTOR2( (float)(mousepos.x-m_OldMousePos.x), (float)(mousepos.y-m_OldMousePos.y) );

						switch( m_pControlDragging->GetType() )
						{
						case CKTDGUIControl::UCT_LISTBOX:
							{
								CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pControlDragging;
								vOffsetPos += pListBox->GetPos();
								pListBox->SetPos_LUA( (int)vOffsetPos.x, (int)vOffsetPos.y );
							} break;
						//case CKTDGUIControl::UCT_STATIC:
						//	{
						//		CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pControlDragging;								
						//		//pStatic->GetPicture()
						//		//pStatic->SetOffsetPos()
						//		//vOffsetPos ++ pStatic->getp
						//	} break;

						default:
							vOffsetPos += m_pControlDragging->GetOffsetPos();
							m_pControlDragging->SetOffsetPos( vOffsetPos );
						}

					}
					m_OldMousePos = mousepos;

				} break;

			case WM_DROPFILES:
				{
					WCHAR wszFilePath[MAX_PATH] = L"";
					DragQueryFile((HDROP) wParam, 0, wszFilePath, MAX_PATH);
					OpenScript( wszFilePath );
					UpdateControlListBox();
				} break;
			}

			if( true == m_bShowDialog )
			{
				for( unsigned i=0; i<m_vecpDialog.size(); i++ )
				{
					bool bNoFurtherProcessing = m_vecpDialog[i]->MsgProc( hWnd, uMsg, wParam, lParam );
					if( bNoFurtherProcessing == true )
						return true;
				}
			}	
		} break;

	case MODE_TEXTURE_SAMPLING:
		{
			bool bNoFurtherProcessing = m_pTextureSamplingDialog->MsgProc( hWnd, uMsg, wParam, lParam );
			if( bNoFurtherProcessing == true )
				return true;

			POINTS mousepos;
			const int MAGIC_MOVE_AMOUNT = 16;

			switch( uMsg )
			{	
			case WM_KEYDOWN:
				switch( wParam )
				{
				case VK_SPACE:
					{
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( 0, 0, -700, 0, 0, 0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );						
					} break;

				case VK_LEFT:
					{
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x-MAGIC_MOVE_AMOUNT, pCamera->GetEye().y, -700, pCamera->GetLookAt().x-MAGIC_MOVE_AMOUNT, pCamera->GetLookAt().y,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );						
					} break;

				case VK_RIGHT:
					{
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x+MAGIC_MOVE_AMOUNT, pCamera->GetEye().y, -700, pCamera->GetLookAt().x+MAGIC_MOVE_AMOUNT, pCamera->GetLookAt().y,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
					} break;

				case VK_UP:
					{
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x, pCamera->GetEye().y+MAGIC_MOVE_AMOUNT, -700, pCamera->GetLookAt().x, pCamera->GetLookAt().y+MAGIC_MOVE_AMOUNT,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
					} break;

				case VK_DOWN:
					{
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x, pCamera->GetEye().y-MAGIC_MOVE_AMOUNT, -700, pCamera->GetLookAt().x, pCamera->GetLookAt().y-MAGIC_MOVE_AMOUNT,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
					} break;

				case VK_PRIOR:
					{
						m_TexturedRectangle.Rescale( 2.f );
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x*2.f, pCamera->GetEye().y*2.f, -700, pCamera->GetLookAt().x*2.f, pCamera->GetLookAt().y*2.f,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
					} break;

				case VK_NEXT:
					{
						m_TexturedRectangle.Rescale( 0.5f );
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x*0.5f, pCamera->GetEye().y*0.5f, -700, pCamera->GetLookAt().x*0.5f, pCamera->GetLookAt().y*0.5f,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
					} break;

				} break;

				//case WM_CHAR:
				//	switch(wParam)
				//	{
				//	//case 'c':
				//	//case 'C':
				//	//	{
				//	//		g_pKTDXApp->GetDGManager()->GetDialogManager()->ClearAllDlg();	
				//	//		m_pDialog = NULL;
				//	//		UpdateControlListBox();
				//	//	} break;

				//	} break; // WM_CHAR
			case WM_RBUTTONDOWN:
				{
					m_bRButtonDragging = true;
				} break;

			case WM_RBUTTONUP:
				{
					m_bRButtonDragging = false;
				} break;

			case WM_MOUSEMOVE:
				{
					POINTS mousepos = MAKEPOINTS( lParam );
					if( true == m_bRButtonDragging )
					{
						D3DXVECTOR2 vOffsetPos = D3DXVECTOR2( (float)(mousepos.x-m_OldMousePos.x), (float)(mousepos.y-m_OldMousePos.y) );
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x-vOffsetPos.x, pCamera->GetEye().y+vOffsetPos.y, -700, pCamera->GetLookAt().x-vOffsetPos.x, pCamera->GetLookAt().y+vOffsetPos.y,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
					}					
					m_OldMousePos = mousepos;
				} break;

			case WM_MOUSEWHEEL:
				{
					int zDelta = (int) wParam;
					if( zDelta > 0 )
					{
						m_TexturedRectangle.Rescale( 2.f );	
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x*2.f, pCamera->GetEye().y*2.f, -700, pCamera->GetLookAt().x*2.f, pCamera->GetLookAt().y*2.f,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
					}
					else
					{
						m_TexturedRectangle.Rescale( 0.5f );
						CKTDGCamera* pCamera = g_pKTDXApp->GetDGManager()->GetCamera();
						pCamera->Point( pCamera->GetEye().x*0.5f, pCamera->GetEye().y*0.5f, -700, pCamera->GetLookAt().x*0.5f, pCamera->GetLookAt().y*0.5f,0 );
						g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
					}
				} break;

			case WM_LBUTTONDOWN:
				{				
					mousepos = MAKEPOINTS(lParam);

					D3DVIEWPORT vp; 
					g_pKTDXApp->GetDevice()->GetViewport( &vp );
					D3DXMATRIX matWorld, matView, matProj;
					g_pKTDXApp->GetDevice()->GetTransform( D3DTS_WORLD, &matWorld );
					g_pKTDXApp->GetDevice()->GetTransform( D3DTS_VIEW, &matView );
					g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );

					D3DXVECTOR3 vMouse2 = D3DXVECTOR3( mousepos.x, mousepos.y, 0 );
					D3DXVECTOR3 vMouse;
					D3DXVec3Unproject( &vMouse, &vMouse2, &vp, &matProj, &matView, &matWorld );
					D3DXVECTOR3 vMouseWorld = vMouse;

					vMouse.y = m_TexturedRectangle.GetRectSize() - vMouse.y;
					vMouse.x *= m_TexturedRectangle.GetWidth() / m_TexturedRectangle.GetRectSize();
					vMouse.y *= m_TexturedRectangle.GetHeight() / m_TexturedRectangle.GetRectSize();
					float fTemp2[2] = { (float)(int)vMouse.x, (float)(int)vMouse.y };
					vMouse.x = fTemp2[0];
					vMouse.y = fTemp2[1];

					if( NULL != m_pLastFocusedEditBox )
					{
						m_pLastFocusedEditBox->SetTextFloatArray( (float*)&fTemp2[0], 2 );
						switch( m_pLastFocusedEditBox->GetID() )
						{
						case IDC_EDIT_TEX_LEFT_TOP:
							{
								m_vUVTexCoord[0].x = vMouse.x;
								m_vUVTexCoord[0].y = vMouse.y;
								m_WiredRect.SetVertexPos( 0, vMouseWorld );
							} break;
						case IDC_EDIT_TEX_RIGHT_TOP:
							{
								m_vUVTexCoord[1].x = vMouse.x;
								m_vUVTexCoord[1].y = vMouse.y;
								m_WiredRect.SetVertexPos( 1, vMouseWorld );
							} break;

						case IDC_EDIT_TEX_LEFT_BOTTOM:
							{
								m_vUVTexCoord[2].x = vMouse.x;
								m_vUVTexCoord[2].y = vMouse.y;
								m_WiredRect.SetVertexPos( 2, vMouseWorld );
							} break;

						case IDC_EDIT_TEX_RIGHT_BOTTOM:
							{
								m_vUVTexCoord[3].x = vMouse.x;
								m_vUVTexCoord[3].y = vMouse.y;
								m_WiredRect.SetVertexPos( 3, vMouseWorld );
							} break;
						}						
					}
				} break;
			}
		} break;
	}

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );
}

void CX2DialogToolMain::OpenScript( const WCHAR* scriptFilename )
{
	if( scriptFilename == NULL )
		return;

	//g_pKTDXApp->RegisterLuabind();
	//g_pKTDXApp->GetDGManager()->GetDialogManager()->ClearAllDlg();

	// path랑 file name 분리
	WCHAR FileName[256] = L"";
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	_wsplitpath( scriptFilename, drive, dir, fname, ext);
	wcscat( FileName, fname);
	wcscat( FileName, ext);

	CKTDGUIDialogType pDialog = new CKTDGUIDialog( this, FileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialog );

	////파일 로드
	//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	//Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( FileName );

	//if( Info == NULL )
	//{
	//	return;
	//}

	//m_pLuaManager = new KLuaManager();

	//if( m_pLuaManager->DoMemory( Info->pRealData, Info->size ) == false )
	//{
	//	MessageBox(g_pKTDXApp->GetHWND(), L"DialogTool 루아 로드 에러", L"에러", MB_OK);
	//	SAFE_DELETE( m_pLuaManager );
	//	return;
	//}


	////다이얼로그 데이터 로딩
	//if( m_pLuaManager->BeginTable( "FILE_NAME" ) == true )
	//{
	//	char	strKeyName[MAX_PATH]	= "";
	//	std::wstring	wszFileName;
	//	std::string		szKeyName;

	//	for( int i = 1;; i++ )
	//	{
	//		sprintf( strKeyName, "DIALOG_FILE%d", i );

	//		if( m_pLuaManager->GetValue( strKeyName, szKeyName ) == true )
	//		{
	//			ConvertCharToWCHAR( wszFileName, szKeyName.c_str() );
	//			CKTDGUIDialogType	pDialog = new CKTDGUIDialog( NULL, wszFileName.c_str() );
	//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialog );
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}

	//	m_pLuaManager->EndTable();
	//}

	//SAFE_DELETE( m_pLuaManager );
}


bool CX2DialogToolMain::SaveScritp2File( WCHAR* wszFullPath )
{
	if( NULL == wszFullPath || wszFullPath[0] == L'\0' )
		return false;

#define DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE \
	ofs << "{" << std::endl; \
	ofs << "\tTEX_NAME       \t= \"" << strTextureName << "\"," << std::endl; \
	ofs << "\t\"LEFT_TOP     \t= D3DXVECTOR2( " << fTexWidth*pTexUV[0].x << ", " << fTexHeight*pTexUV[0].y << " )\"," << std::endl; \
	ofs << "\t\"RIGHT_TOP    \t= D3DXVECTOR2( " << fTexWidth*pTexUV[1].x << ", " << fTexHeight*pTexUV[1].y << " )\"," << std::endl; \
	ofs << "\t\"LEFT_BOTTOM  \t= D3DXVECTOR2( " << fTexWidth*pTexUV[2].x << ", " << fTexHeight*pTexUV[2].y << " )\"," << std::endl; \
	ofs << "\t\"RIGHT_BOTTOM \t= D3DXVECTOR2( " << fTexWidth*pTexUV[3].x << ", " << fTexHeight*pTexUV[3].y << " )\"," << std::endl; \
	ofs << "}" << std::endl; 


	//WCHAR wszDir[_MAX_DRIVE+_MAX_DIR];
	//WCHAR wszFileName[_MAX_FNAME+_MAX_EXT];
	//DecomposePathAndFileName( wszDir, wszFileName, wszFullPath );

	string strFullPath;
	ConvertWCHARToChar( strFullPath, wszFullPath );

	int iStrLen = strFullPath.length();
	if( 'l' != strFullPath[iStrLen-3] || 'u' != strFullPath[iStrLen-2] || 'a' != strFullPath[iStrLen-1] )
	{
		strFullPath.append(".lua");
	}
	
	std::ofstream ofs( strFullPath.c_str() );

	CKTDGUIDialogManager* pDialogManager = g_pKTDXApp->GetDGManager()->GetDialogManager();
	vector<CKTDGUIDialogType>& dialogList = pDialogManager->GetDialogList();
	for( unsigned i=0; i<dialogList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = dialogList[i];
		vector<CKTDGUIControl*>& controlList = pDialog->GetControlList();

		for( unsigned j=0; j<controlList.size(); j++ )
		{
			CKTDGUIControl* pControl = controlList[j];
			switch( pControl->GetType() )
			{
			case CKTDGUIControl::UCT_BUTTON:
				{
					CKTDGUIButton* pButton = (CKTDGUIButton*) pControl;
					string strButtonName;
					ConvertWCHARToChar( strButtonName, pButton->GetName() );
										
					ofs << std::endl;
					ofs << "-- 버튼" << std::endl;
					ofs << "g_p" << strButtonName << " = g_pUIDialog:CreateButton()" << std::endl;
					ofs << "g_p" << strButtonName << ":SetName( \"" << strButtonName << "\" )" << std::endl;
					ofs << "g_pUIDialog:AddControl( g_p" << strButtonName << " )" << std::endl;
					ofs << std::endl;
										
					D3DXVECTOR2 vOffsetPos = pButton->GetOffsetPos();					

					string strTextureName;
					ConvertWCHARToChar( strTextureName, pButton->GetNormalTexData()->texName.c_str() );
					const D3DXVECTOR2* pTexUV = & (pButton->GetNormalTexData()->uvTexture[0]);
					float fTexWidth  = (float) pButton->GetNormalTexData()->pTexture->GetWidth();
					float fTexHeight = (float) pButton->GetNormalTexData()->pTexture->GetHeight();					
					ofs << "g_p" << strButtonName << ":SetNormalTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					const CKTDGUIControl::UIPointData* pPoint = pButton->GetNormalPoint();

					ofs << "g_p" << strButtonName << ":SetNormalPoint" << std::endl;
					ofs << "{" << std::endl;
					ofs << "\t\"LEFT_TOP     \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->leftTopPoint.x     << ", " << vOffsetPos.y + pPoint->leftTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_TOP    \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->rightTopPoint.x    << ", " << vOffsetPos.y + pPoint->rightTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"LEFT_BOTTOM  \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->leftBottomPoint.x  << ", " << vOffsetPos.y + pPoint->leftBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_BOTTOM \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->rightBottomPoint.x << ", " << vOffsetPos.y + pPoint->rightBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"COLOR        \t= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," << std::endl;
					ofs << "\tCHANGE_TIME  \t= 0.0," << std::endl;
					ofs << "}" << std::endl;


					ConvertWCHARToChar( strTextureName, pButton->GetMouseOverTexData()->texName.c_str() );
					pTexUV = & (pButton->GetMouseOverTexData()->uvTexture[0]);
					fTexWidth  = (float) pButton->GetMouseOverTexData()->pTexture->GetWidth();
					fTexHeight = (float) pButton->GetMouseOverTexData()->pTexture->GetHeight();
					ofs << "g_p" << strButtonName << ":SetOverTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					pPoint = pButton->GetMouseOverPoint();

					ofs << "g_p" << strButtonName << ":SetOverPoint" << std::endl;
					ofs << "{" << std::endl;					
					ofs << "\t\"LEFT_TOP     \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->leftTopPoint.x     << ", " << vOffsetPos.y + pPoint->leftTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_TOP    \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->rightTopPoint.x    << ", " << vOffsetPos.y + pPoint->rightTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"LEFT_BOTTOM  \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->leftBottomPoint.x  << ", " << vOffsetPos.y + pPoint->leftBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_BOTTOM \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->rightBottomPoint.x << ", " << vOffsetPos.y + pPoint->rightBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"COLOR        \t= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," << std::endl;
					ofs << "\tCHANGE_TIME  \t= 0.0," << std::endl;
					ofs << "}" << std::endl;


					ConvertWCHARToChar( strTextureName, pButton->GetMouseDownTexData()->texName.c_str() );
					pTexUV = & (pButton->GetMouseDownTexData()->uvTexture[0]);
					fTexWidth  = (float) pButton->GetMouseDownTexData()->pTexture->GetWidth();
					fTexHeight = (float) pButton->GetMouseDownTexData()->pTexture->GetHeight();
					ofs << "g_p" << strButtonName << ":SetDownTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					pPoint = pButton->GetMouseDownPoint();

					ofs << "g_p" << strButtonName << ":SetDownPoint" << std::endl;
					ofs << "{" << std::endl;					
					ofs << "\t\"LEFT_TOP     \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->leftTopPoint.x     << ", " << vOffsetPos.y + pPoint->leftTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_TOP    \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->rightTopPoint.x    << ", " << vOffsetPos.y + pPoint->rightTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"LEFT_BOTTOM  \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->leftBottomPoint.x  << ", " << vOffsetPos.y + pPoint->leftBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_BOTTOM \t= D3DXVECTOR2( " << vOffsetPos.x +pPoint->rightBottomPoint.x << ", " << vOffsetPos.y + pPoint->rightBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"COLOR        \t= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," << std::endl;
					ofs << "\tCHANGE_TIME  \t= 0.0," << std::endl;
					ofs << "}" << std::endl;				

				} break;

			case CKTDGUIControl::UCT_CHECKBOX:
				{
					CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) pControl;					
					string strCheckBoxName;
					ConvertWCHARToChar( strCheckBoxName, pCheckBox->GetName() );

					ofs << std::endl;
					ofs << "-- 체크 박스" << std::endl;
					ofs << "g_p" << strCheckBoxName << " = g_pUIDialog:CreateCheckBox()" << std::endl;
					ofs << "g_p" << strCheckBoxName << ":SetName( \"" << strCheckBoxName << "\" )" << std::endl;
					ofs << "g_pUIDialog:AddControl( g_p" << strCheckBoxName << " )" << std::endl;
					ofs << std::endl;

					string strTextureName;
					ConvertWCHARToChar( strTextureName, pCheckBox->GetNormalTexData()->texName.c_str() );
					const D3DXVECTOR2* pTexUV = & (pCheckBox->GetNormalTexData()->uvTexture[0]);
					float fTexWidth  = (float) pCheckBox->GetNormalTexData()->pTexture->GetWidth();
					float fTexHeight = (float) pCheckBox->GetNormalTexData()->pTexture->GetHeight();					

					ofs << "g_p" << strCheckBoxName << ":SetNormalTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					ConvertWCHARToChar( strTextureName, pCheckBox->GetCheckedTexData()->texName.c_str() );
					pTexUV = & (pCheckBox->GetCheckedTexData()->uvTexture[0]);
					fTexWidth  = (float) pCheckBox->GetCheckedTexData()->pTexture->GetWidth();
					fTexHeight = (float) pCheckBox->GetCheckedTexData()->pTexture->GetHeight();

					ofs << "g_p" << strCheckBoxName << ":SetCheckedTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					D3DXVECTOR2 vOffsetPos = pCheckBox->GetOffsetPos();
					const CKTDGUIControl::UIPointData* pPoint = pCheckBox->GetBGPoint();

					ofs << "g_p" << strCheckBoxName << ":SetBGPoint" << std::endl;
					ofs << "{" << std::endl;
					ofs << "\t\"LEFT_TOP     \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->leftTopPoint.x     << ", " << vOffsetPos.y + pPoint->leftTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_TOP    \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->rightTopPoint.x    << ", " << vOffsetPos.y + pPoint->rightTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"LEFT_BOTTOM  \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->leftBottomPoint.x  << ", " << vOffsetPos.y + pPoint->leftBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_BOTTOM \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->rightBottomPoint.x << ", " << vOffsetPos.y + pPoint->rightBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"COLOR        \t= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," << std::endl;
					ofs << "\tCHANGE_TIME  \t= 0.0," << std::endl;
					ofs << "}" << std::endl;

					pPoint = pCheckBox->GetCheckPoint();

					ofs << "g_p" << strCheckBoxName << ":SetCheckPoint" << std::endl;
					ofs << "{" << std::endl;
					ofs << "\t\"LEFT_TOP     \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->leftTopPoint.x     << ", " << vOffsetPos.y + pPoint->leftTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_TOP    \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->rightTopPoint.x    << ", " << vOffsetPos.y + pPoint->rightTopPoint.y << " )\"," << std::endl;
					ofs << "\t\"LEFT_BOTTOM  \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->leftBottomPoint.x  << ", " << vOffsetPos.y + pPoint->leftBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"RIGHT_BOTTOM \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->rightBottomPoint.x << ", " << vOffsetPos.y + pPoint->rightBottomPoint.y << " )\"," << std::endl;
					ofs << "\t\"COLOR        \t= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," << std::endl;
					ofs << "\tCHANGE_TIME  \t= 0.0," << std::endl;
					ofs << "}" << std::endl;

				} break;

			case CKTDGUIControl::UCT_LISTBOX:
				{
					CKTDGUIListBox* pListBox = (CKTDGUIListBox*) pControl;
					string strListBoxName;
					ConvertWCHARToChar( strListBoxName, pListBox->GetName() );

					ofs << std::endl;
					ofs << "-- 리스트 박스" << std::endl;
					ofs << "g_p" << strListBoxName << " = g_pUIDialog:CreateListBox()" << std::endl;
					ofs << "g_p" << strListBoxName << ":SetName( \"" << strListBoxName << "\" )" << std::endl;
					ofs << "g_pUIDialog:AddControl( g_p" << strListBoxName << " )" << std::endl;
					ofs << std::endl;

					string strFontName;
					switch( pListBox->GetStringData()->fontIndex )
					{
					case 0:
						strFontName = "XF_DODUM_15_NORMAL";
						break;
					case 1:
						strFontName = "XF_DODUM_15_BOLD";
						break;
					case 2:
						strFontName = "XF_DODUM_20_NORMAL";
						break;
					case 3:
						strFontName = "XF_DODUM_20_BOLD";
						break;
					case 4:
						strFontName = "XF_DODUM_25_BOLD";
						break;

					case 5:
						strFontName = "XF_DODUM_10_SEMIBOLD";
						break;
					case 6:
						strFontName = "XF_DODUM_11_SEMIBOLD";
						break;
					case 7:
						strFontName = "XF_DODUM_12_SEMIBOLD";
						break;
					case 8:
						strFontName = "XF_DODUM_13_SEMIBOLD";
						break;
					case 9:
						strFontName = "XF_DODUM_13_NORMAL";
						break;

					case 10:
						strFontName = "XF_DODUM_14_HEAVY";
						break;
					case 11:
						strFontName = "XF_DODUM_14_SEMIBOLD";
						break;
					case 12:
						strFontName = "XF_DODUM_15_BOLD";
						break;
					case 13:
						strFontName = "XF_DODUM_20_BOLD";
						break;
					case 14:
						strFontName = "XF_DODUM_50_BOLD";
						break;

					case 15:
						strFontName = "XF_DODUM_40_BOLD";
						break;
					case 16:
						strFontName = "XF_DODUM_30_BOLD";
						break;
					}

					ofs << "g_p" << strListBoxName << ":SetString" << std::endl;
					ofs << "{" << std::endl;
					ofs << "\tMSG             \t= \"NoName\"," << std::endl;					
					ofs << "\tFONT_INDEX      \t= " << strFontName << "," << std::endl;
					ofs << "\t\"POS           \t= D3DXVECTOR2(0,0)\"," << std::endl;
					ofs << "\t\"COLOR         \t= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," << std::endl;
					ofs << "\t\"OUTLINE_COLOR \t= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," << std::endl;
					ofs << "}" << std::endl;					

					string strTextureName;
					ConvertWCHARToChar( strTextureName, pListBox->GetTextTexData()->texName.c_str() );
					const D3DXVECTOR2* pTexUV = & (pListBox->GetTextTexData()->uvTexture[0]);
					float fTexWidth  = (float) pListBox->GetTextTexData()->pTexture->GetWidth();
					float fTexHeight = (float) pListBox->GetTextTexData()->pTexture->GetHeight();

					ofs << "g_p" << strListBoxName << ":SetTextTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					ConvertWCHARToChar( strTextureName, pListBox->GetBoundingBoxTexData()->texName.c_str() );
					pTexUV = & (pListBox->GetBoundingBoxTexData()->uvTexture[0]);
					fTexWidth  = (float) pListBox->GetBoundingBoxTexData()->pTexture->GetWidth();
					fTexHeight = (float) pListBox->GetBoundingBoxTexData()->pTexture->GetHeight();
					ofs << "g_p" << strListBoxName << ":SetBoundingBoxTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					ConvertWCHARToChar( strTextureName, pListBox->GetScrollBarUpButtonTexData()->texName.c_str() );
					pTexUV = & (pListBox->GetScrollBarUpButtonTexData()->uvTexture[0]);
					fTexWidth  = (float) pListBox->GetScrollBarUpButtonTexData()->pTexture->GetWidth();
					fTexHeight = (float) pListBox->GetScrollBarUpButtonTexData()->pTexture->GetHeight();
					ofs << "g_p" << strListBoxName << ":SetScollBarUpButtonTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					ConvertWCHARToChar( strTextureName, pListBox->GetScrollBarUpButtonDownTexData()->texName.c_str() );
					pTexUV = & (pListBox->GetScrollBarUpButtonDownTexData()->uvTexture[0]);
					fTexWidth  = (float) pListBox->GetScrollBarUpButtonDownTexData()->pTexture->GetWidth();
					fTexHeight = (float) pListBox->GetScrollBarUpButtonDownTexData()->pTexture->GetHeight();
					ofs << "g_p" << strListBoxName << ":SetScollBarUpButtonDownTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					ConvertWCHARToChar( strTextureName, pListBox->GetScrollBarDownButtonTexData()->texName.c_str() );
					pTexUV = & (pListBox->GetScrollBarDownButtonTexData()->uvTexture[0]);
					fTexWidth  = (float) pListBox->GetScrollBarDownButtonTexData()->pTexture->GetWidth();
					fTexHeight = (float) pListBox->GetScrollBarDownButtonTexData()->pTexture->GetHeight();
					ofs << "g_p" << strListBoxName << ":SetScollBarDownButtonTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					ConvertWCHARToChar( strTextureName, pListBox->GetScrollBarDownButtonDownTexData()->texName.c_str() );
					pTexUV = & (pListBox->GetScrollBarDownButtonDownTexData()->uvTexture[0]);
					fTexWidth  = (float) pListBox->GetScrollBarDownButtonDownTexData()->pTexture->GetWidth();
					fTexHeight = (float) pListBox->GetScrollBarDownButtonDownTexData()->pTexture->GetHeight();
					ofs << "g_p" << strListBoxName << ":SetScollBarDownButtonDownTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					ConvertWCHARToChar( strTextureName, pListBox->GetScrollBarThumbTexData()->texName.c_str() );
					pTexUV = & (pListBox->GetScrollBarThumbTexData()->uvTexture[0]);
					fTexWidth  = (float) pListBox->GetScrollBarThumbTexData()->pTexture->GetWidth();
					fTexHeight = (float) pListBox->GetScrollBarThumbTexData()->pTexture->GetHeight();
					ofs << "g_p" << strListBoxName << ":SetScollBarThumbTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					ConvertWCHARToChar( strTextureName, pListBox->GetScrollBarTrackTexData()->texName.c_str() );
					pTexUV = & (pListBox->GetScrollBarTrackTexData()->uvTexture[0]);
					fTexWidth  = (float) pListBox->GetScrollBarTrackTexData()->pTexture->GetWidth();
					fTexHeight = (float) pListBox->GetScrollBarTrackTexData()->pTexture->GetHeight();
					ofs << "g_p" << strListBoxName << ":SetScollBarTrackTex" << std::endl;
					DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;

					//D3DXVECTOR2 vOffsetPos = pListBox->GetOffsetPos();
					//D3DXVECTOR2 vPos = vOffsetPos + pListBox->GetPos();
					D3DXVECTOR2 vPos = pListBox->GetPos();
					
					ofs << "g_p" << strListBoxName << ":InitScrollBar()" << std::endl;					
					ofs << "--g_p" << strListBoxName << ":SetScrollBarWidth( " << pListBox->GetScrollBarWidth() << ")" << std::endl;                    
					ofs << "g_p" << strListBoxName << ":SetPos( " << vPos.x << ", " << vPos.y << " )" << std::endl;
					ofs << "g_p" << strListBoxName << ":SetSize( " << pListBox->GetWidth() << ", " << pListBox->GetHeight() << " )" << std::endl;
					
					ofs << "g_p" << strListBoxName << ":AddItem( \"item0\", NULL )" << std::endl;
					ofs << "g_p" << strListBoxName << ":AddItem( \"item1\", NULL )" << std::endl;
					ofs << "g_p" << strListBoxName << ":AddItem( \"item2\", NULL )" << std::endl;			

				} break;

			case CKTDGUIControl::UCT_STATIC:
				{
					CKTDGUIStatic* pStatic = (CKTDGUIStatic*) pControl;					
					string strStaticName;
					ConvertWCHARToChar( strStaticName, pStatic->GetName() );

					ofs << std::endl;
					ofs << "-- 스태틱" << std::endl;
					ofs << "g_p" << strStaticName << " = g_pUIDialog:CreateStatic()" << std::endl;
					ofs << "g_p" << strStaticName << ":SetName( \"" << strStaticName << "\" )" << std::endl;
					ofs << "g_pUIDialog:AddControl( g_p" << strStaticName << " )" << std::endl;
					ofs << std::endl;

					string strPictureName = "Picture";
					for( int i=0; i<100 ; i++ )
					{
						CKTDGUIControl::CPictureData* pPicture = pStatic->GetPicture(i);
						if( NULL == pPicture )
							break;

						ofs << "-- 픽쳐" << i << std::endl;
						ofs << "g_p" << strPictureName << i << " = g_pUIDialog:CreatePicture()" << std::endl;
						ofs << "g_p" << strStaticName << ":AddPicture( g_p" << strPictureName << i << ")" << std::endl;

						string strTextureName;
						ConvertWCHARToChar( strTextureName, pPicture->pTexture->texName.c_str() );
						const D3DXVECTOR2* pTexUV = & (pPicture->pTexture->uvTexture[0]); 
						float fTexWidth  = (float) pPicture->pTexture->pTexture->GetWidth();
						float fTexHeight = (float) pPicture->pTexture->pTexture->GetHeight();

						ofs << "g_p" << strPictureName << i << ":SetTex" << std::endl;
						DIALOG_TOOL_TEXTURE_COORDINATES_TEMPLATE;
						
						D3DXVECTOR2 vOffsetPos = pStatic->GetOffsetPos();
						const CKTDGUIControl::UIPointData* pPoint = pPicture->pPoint;
						
						ofs << "g_p" << strPictureName << i << ":SetPoint" << std::endl;						
						ofs << "{" << std::endl;
						ofs << "\t\"LEFT_TOP     \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->leftTopPoint.x     << ", " << vOffsetPos.y + pPoint->leftTopPoint.y << " )\"," << std::endl;
						ofs << "\t\"RIGHT_TOP    \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->rightTopPoint.x    << ", " << vOffsetPos.y + pPoint->rightTopPoint.y << " )\"," << std::endl;
						ofs << "\t\"LEFT_BOTTOM  \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->leftBottomPoint.x  << ", " << vOffsetPos.y + pPoint->leftBottomPoint.y << " )\"," << std::endl;
						ofs << "\t\"RIGHT_BOTTOM \t= D3DXVECTOR2( " << vOffsetPos.x + pPoint->rightBottomPoint.x << ", " << vOffsetPos.y + pPoint->rightBottomPoint.y << " )\"," << std::endl;
						ofs << "\t\"COLOR        \t= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," << std::endl;
						ofs << "\tCHANGE_TIME  \t= 0.0," << std::endl;
						ofs << "}" << std::endl;						

					}

				} break;
			} // switch()
		} // for(j)
	} // for(i)

	ofs.close();
	return true;
}

bool CX2DialogToolMain::DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath )
{
	if( NULL == wszFullPath )
		return false;
	
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	_wsplitpath( wszFullPath, drive, dir, fname, ext);
	if( NULL != wszFileName )
	{
		wcscat( wszFileName, fname);
		wcscat( wszFileName, ext);	
	}
	if( NULL != wszPath )
	{
		wcscat( wszPath, drive );
		wcscat( wszPath, dir );
	}


	return true;
}


void CX2DialogToolMain::UpdateControlListBox()
{
	CDXUTListBox* pListBox = m_vecpDialog[DLG_DIALOG0]->GetListBox( IDC_LIST_CONTROLS );
	pListBox->RemoveAllItems();
	pListBox->m_nSelected = -1;
	pListBox->m_nSelStart = 0;

	CKTDGUIDialogManager* pDialogManager = g_pKTDXApp->GetDGManager()->GetDialogManager();
	vector<CKTDGUIDialogType>& dialogList = pDialogManager->GetDialogList();
	for( unsigned i=0; i<dialogList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = dialogList[i];
		vector<CKTDGUIControl*>& controlList = pDialog->GetControlList();
						
		//pListBox->AddItem( L"-----------", (LPVOID)NULL );
		//pListBox->AddItem( pDialog->GetName(), (LPVOID)pDialog );
		//pListBox->AddItem( L"", (LPVOID)pDialog );		
		for( unsigned j=0; j<controlList.size(); j++ )
		{
			CKTDGUIControl* pControl = controlList[j];
			pListBox->AddItem( pControl->GetName(), (LPVOID)pControl );
			
			if( pControl == m_pControlPicked )
			{
				pListBox->SelectItem( pListBox->GetSize()-1 );
			}

			//wstring wstrItemText;
			//CKTDGUIControl::UI_CONTROL_TYPE uiControlType = pControl->GetType();
			//switch( uiControlType )
			//{
			//case CKTDGUIControl::UCT_BUTTON:
			//	{
			//		wstrItemText = L"BUTTON: ";
			//		wstrItemText += pControl->GetName();
			//		pListBox->AddItem( wstrItemText.c_str(), (LPVOID)pControl );
			//	} break;
			//}

			

		}
	}	

}




//void CALLBACK OnFileRenameToolDialogEvent0( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
//{
//	//	OPENFILENAME ofn;        // common dialog box structure
//	//	WCHAR wszFileName[512];  // path까지 포함한 파일 이름
//
//	CFileRenameTool* pFileTool = (CFileRenameTool*) pUserContext;
//
//	switch( nControlID )
//	{
//	case CFileRenameTool::IDC_EDIT_FILE_EXTENSION:
//		switch( nEvent )
//		{
//		case EVENT_EDITBOX_STRING:
//			{
//				pFileTool->SetFileExtension( ((CDXUTEditBox*)pControl)->GetText() );				
//			} break;
//
//		case EVENT_EDITBOX_CHANGE:
//			{
//				pFileTool->SetFileExtension( ((CDXUTEditBox*)pControl)->GetText() );
//			} break;
//		} break;
//
//	case CFileRenameTool::IDC_EDIT_PREFIX_OR_SUFFIX:
//		switch( nEvent )
//		{
//		case EVENT_EDITBOX_STRING:
//			{
//				pFileTool->SetPrefixOrSuffix( ((CDXUTEditBox*)pControl)->GetText() );				
//			} break;
//
//		case EVENT_EDITBOX_CHANGE:
//			{
//				pFileTool->SetPrefixOrSuffix( ((CDXUTEditBox*)pControl)->GetText() );
//			} break;
//		} break;
//
//	case CFileRenameTool::IDC_BUTTON_ATTACH_OR_DETACH:
//		{
//			if( CFileRenameTool::MODE_ATTACH == pFileTool->GetMode() )
//			{
//				pFileTool->SetMode( CFileRenameTool::MODE_DETACH );
//				((CDXUTButton*)pControl)->SetText( L"DETACH" );
//			}				
//			else
//			{
//				pFileTool->SetMode( CFileRenameTool::MODE_ATTACH );
//				((CDXUTButton*)pControl)->SetText( L"ATTACH" );
//			}				
//		} break;
//
//	case CFileRenameTool::IDC_BUTTON_PREFIX_OR_SUFFIX:
//		{
//			if( CFileRenameTool::STATE_PREFIX == pFileTool->GetState() )
//			{
//				pFileTool->SetState( CFileRenameTool::STATE_SUFFIX );
//				((CDXUTButton*)pControl)->SetText( L"SUFFIX" );
//			}
//			else
//			{
//				pFileTool->SetState( CFileRenameTool::STATE_PREFIX );			
//				((CDXUTButton*)pControl)->SetText( L"PREFIX" );
//			}				
//		} break;
//
//	case CFileRenameTool::IDC_BUTTON_HELP:
//		{
//			wstring wstrHelp;
//			wstrHelp = L"somebody help me~ \n";
//			MessageBox( g_pKTDXApp->GetHWND(), wstrHelp.c_str(), L"HELP", MB_OK );
//		} break;
//
//	}
//}
//
//
//void CFileRenameTool::FinalDialog()
//{
//	for( unsigned i=0; i<m_vecDialog.size(); i++ )
//	{
//		SAFE_DELETE( m_vecDialog[i] );
//	}
//	m_vecDialog.clear();
//}
//
//void CFileRenameTool::InitDialog()
//{
//	// initialize UI
//	extern CDXUTDialogResourceManager g_DialogResourceManager;
//	CDXUTDialog* pDlg = NULL;
//
//	pDlg = new CDXUTDialog();
//	m_vecDialog.push_back( pDlg );
//	pDlg->Init( &g_DialogResourceManager );
//	pDlg->SetCallback( OnFileRenameToolDialogEvent0, this );
//	pDlg->SetSize( 100, 300 );
//	pDlg->SetFont(0, L"굴림", 12, 12);
//
//	int iX = 10;
//	int iY = 100;
//	int iDX = 100;	
//	CDXUTEditBox* pEditBox = NULL;
//	pDlg->AddStatic( IDC_STATIC_DUMMY, L"Prefix or Suffix", iX, iY, 100, 22);
//	iY += 25;
//	pDlg->AddEditBox( IDC_EDIT_PREFIX_OR_SUFFIX, L"", iX, iY, 100, 30, false, &pEditBox );
//	pEditBox->SetText( m_wstrPrefixOrSuffix.c_str() );
//	iY += 35;
//	pDlg->AddStatic( IDC_STATIC_DUMMY, L"File Extension", iX, iY, 100, 22);
//	iY += 25;
//	pDlg->AddEditBox( IDC_EDIT_FILE_EXTENSION, L"", iX, iY, 100, 30, false, &pEditBox );
//	pEditBox->SetText( m_wstrFileExtension.c_str() );
//	iY += 35;
//	CDXUTButton* pButton = NULL;
//	pDlg->AddButton( IDC_BUTTON_PREFIX_OR_SUFFIX, L"", iX, iY, 100, 22, 0, false, &pButton);
//	{
//		if( STATE_PREFIX == m_iState )
//			pButton->SetText( L"PREFIX" );
//		else
//			pButton->SetText( L"SUFFIX" );
//	}
//	iY += 25;
//
//	pDlg->AddButton( IDC_BUTTON_ATTACH_OR_DETACH, L"", iX, iY, 100, 22, 0, false, &pButton);
//	{
//		if( MODE_ATTACH == m_iMode )
//			pButton->SetText( L"ATTACH" );
//		else
//			pButton->SetText( L"DETACH" );
//	}	
//	iY += 25;
//	pDlg->AddButton( IDC_BUTTON_HELP,      L"HELP", iX, iY, 100, 22);
//}
//
//
//CFileRenameTool::~CFileRenameTool(void)
//{
//	FinalDialog();
//}
//
//CFileRenameTool::CFileRenameTool(void)
//{
//	char szWindowTitle[1024];
//	sprintf( szWindowTitle, "File Rename Tool v0.51; compiled at %s, %s", __DATE__, __TIME__ );
//	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );
//
//	m_iMode  = MODE_ATTACH;
//	m_iState = STATE_PREFIX;
//	InitDialog();
//}
//
//HRESULT CFileRenameTool::OnLostDevice()
//{
//	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
//}
//
//
//HRESULT CFileRenameTool::OnResetDevice()
//{
//	int iX = 100;
//	for( unsigned i=0; i<m_vecDialog.size(); i++ )
//	{
//		m_vecDialog[i]->SetLocation( iX, 10 );
//		iX += m_vecDialog[i]->GetWidth()+10;
//	}	
//
//	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
//}
//
//
//HRESULT CFileRenameTool::OnFrameMove( double fTime, float fElapsedTime )
//{
//	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F5) == TRUE )
//	{
//		;
//	}
//
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );
//
//	return S_OK;
//}
//
//HRESULT CFileRenameTool::OnFrameRender()
//{
//	for( unsigned i=0; i<m_vecDialog.size(); i++ )
//	{
//		m_vecDialog[i]->OnRender( g_pKTDXApp->GetElapsedTime() );
//	}
//
//	g_pKTDXApp->GetDGManager()->ObjectChainSort();
//	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
//	g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
//
//	return S_OK;
//}
//
//bool CFileRenameTool::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	for( unsigned i=0; i<m_vecDialog.size(); i++ )
//	{
//		bool bNoFurtherProcessing = m_vecDialog[i]->MsgProc( hWnd, uMsg, wParam, lParam );
//		if( bNoFurtherProcessing == true )
//			return true;
//	}
//
//	switch( uMsg )
//	{
//	case WM_DROPFILES:
//		{
//			WCHAR wszFilePath[MAX_PATH] = L"";
//			DragQueryFile((HDROP) wParam, 0, wszFilePath, MAX_PATH);
//
//			//// temp!!
//			//ReplaceSubString( wszFilePath, L"_PVP_", L"_Dungeon_" );
//			//break;
//
//
//			switch( m_iMode )
//			{
//			case MODE_ATTACH:
//				{
//					if( STATE_PREFIX == m_iState )
//					{
//						if( m_wstrPrefixOrSuffix.length() <= 0 )
//							break;
//
//						AttachPrefixToFileName( wszFilePath, m_wstrPrefixOrSuffix.c_str() );
//					}
//					else
//					{
//					}
//				} break;
//
//			case MODE_DETACH:
//				{
//					if( STATE_PREFIX == m_iState )
//					{
//						if( m_wstrPrefixOrSuffix.length() <= 0 )
//							break;
//
//						DetachPrefixFromFileName( wszFilePath, m_wstrPrefixOrSuffix.c_str() );
//					}
//					else
//					{
//					}
//				} break;
//			}
//
//		} break;
//	}
//
//	return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );
//}
//
//void CFileRenameTool::DetachPrefixFromFileName( WCHAR* wszFilePath, const WCHAR* wszPrefix )
//{
//	if( wszPrefix == NULL || wszFilePath == NULL )
//		return;
//
//	// file path와 name을 분리
//	WCHAR drive[_MAX_DRIVE];
//	WCHAR dir[_MAX_DIR];
//	WCHAR fname[_MAX_FNAME];
//	WCHAR ext[_MAX_EXT];
//
//	_wsplitpath( wszFilePath, drive, dir, fname, ext );
//
//	WCHAR wszCurrDir[256] = L"";
//	string strCurrDir;
//	wcscat( wszCurrDir, drive );
//	wcscat( wszCurrDir, dir );
//	ConvertWCHARToChar( strCurrDir, wszCurrDir );
//	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strCurrDir );
//	string strDirectory = strCurrDir;
//	wcscat( wszCurrDir, L"*.*" );
//	ConvertWCHARToChar( strCurrDir, wszCurrDir );
//
//	HANDLE				hSearch;
//	WIN32_FIND_DATAA	fd;
//
//	hSearch = FindFirstFileA( strCurrDir.c_str(), &fd);
//
//	if(hSearch == INVALID_HANDLE_VALUE)
//		return;
//
//	wstring wstrPrefix = wszPrefix;
//	MakeUpperCase(wstrPrefix);
//	wstring wstrFileName;
//
//	do
//	{
//		if( 0 != strcmp(fd.cFileName, ".") && 0 != strcmp(fd.cFileName, "..") )
//		{
//			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
//			{			
//				;
//			}
//			else
//			{
//				ConvertCharToWCHAR( wstrFileName, fd.cFileName );
//				MakeUpperCase( wstrFileName );
//				int iResult = wstrFileName.find( wstrPrefix );
//				if( 0 == iResult )
//				{
//					string strOldFileName;
//					strOldFileName = fd.cFileName;
//
//					string strNewFileName;
//					strNewFileName = strOldFileName.substr( wstrPrefix.length(), strOldFileName.length() );
//
//					strOldFileName = strDirectory + strOldFileName;
//					strNewFileName = strDirectory + strNewFileName;
//
//					MoveFileA( strOldFileName.c_str(), strNewFileName.c_str() );
//				}
//			}			
//		}
//	} while( FindNextFileA(hSearch, &fd) );
//
//	FindClose(hSearch);
//}
//
//void CFileRenameTool::AttachPrefixToFileName( WCHAR* wszFilePath, const WCHAR* wszPrefix )
//{
//	if( wszPrefix == NULL || wszFilePath == NULL )
//		return;
//
//	// file path와 name을 분리
//	WCHAR drive[_MAX_DRIVE];
//	WCHAR dir[_MAX_DIR];
//	WCHAR fname[_MAX_FNAME];
//	WCHAR ext[_MAX_EXT];
//
//	_wsplitpath( wszFilePath, drive, dir, fname, ext );
//
//	WCHAR wszCurrDir[256] = L"";
//	string strCurrDir;
//	wcscat( wszCurrDir, drive );
//	wcscat( wszCurrDir, dir );
//	ConvertWCHARToChar( strCurrDir, wszCurrDir );
//	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strCurrDir );
//	string strDirectory = strCurrDir;
//	wcscat( wszCurrDir, L"*.*" );
//	ConvertWCHARToChar( strCurrDir, wszCurrDir );
//
//	HANDLE				hSearch;
//	WIN32_FIND_DATAA	fd;
//
//	hSearch = FindFirstFileA( strCurrDir.c_str(), &fd);
//
//	if(hSearch == INVALID_HANDLE_VALUE)
//		return;
//
//	wstring wstrPrefix = wszPrefix;
//	MakeUpperCase(wstrPrefix);
//	wstring wstrFileName;
//
//	do
//	{
//		if( 0 != strcmp(fd.cFileName, ".") && 0 != strcmp(fd.cFileName, "..") )
//		{
//			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
//			{			
//				;
//			}
//			else
//			{
//				ConvertCharToWCHAR( wstrFileName, fd.cFileName );
//				MakeUpperCase( wstrFileName );
//				//int iResult = wstrFileName.find_first_of( wstrPrefix );
//				int iResult = wstrFileName.find( wstrPrefix );
//				if( 0 == iResult )
//				{
//					continue;
//				}
//				else
//				{
//					string strNewFileName;
//					ConvertWCHARToChar( strNewFileName, wstrPrefix.c_str() );
//					strNewFileName += fd.cFileName;
//					strNewFileName = strDirectory + strNewFileName;
//
//					string strOldFileName;
//					strOldFileName = strDirectory + fd.cFileName;
//					//strOldFileName += fd.cFileName;
//
//					//char szCmd[512];
//					//StringCchPrintfA(szCmd, sizeof(szCmd), "rename %s %s", strOldFileName.c_str(), strNewFileName.c_str() );
//					MoveFileA( strOldFileName.c_str(), strNewFileName.c_str() );
//					//system( szCmd );
//				}				
//			}			
//		}
//	} while( FindNextFileA(hSearch, &fd) );
//
//	FindClose(hSearch);
//}
//
//void CFileRenameTool::ReplaceSubString( WCHAR* wszFilePath, const WCHAR* wszOriginal, const WCHAR* wszReplacement )
//{
//	if( NULL == wszReplacement || wszOriginal == NULL || wszFilePath == NULL )
//		return;
//
//	// file path와 name을 분리
//	WCHAR drive[_MAX_DRIVE];
//	WCHAR dir[_MAX_DIR];
//	WCHAR fname[_MAX_FNAME];
//	WCHAR ext[_MAX_EXT];
//
//	_wsplitpath( wszFilePath, drive, dir, fname, ext );
//
//	WCHAR wszCurrDir[256] = L"";
//	string strCurrDir;
//	wcscat( wszCurrDir, drive );
//	wcscat( wszCurrDir, dir );
//	ConvertWCHARToChar( strCurrDir, wszCurrDir );
//	//g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strCurrDir );
//	string strDirectory = strCurrDir;
//	wcscat( wszCurrDir, L"*.*" );
//	ConvertWCHARToChar( strCurrDir, wszCurrDir );
//
//	HANDLE				hSearch;
//	WIN32_FIND_DATAA	fd;
//
//	hSearch = FindFirstFileA( strCurrDir.c_str(), &fd);
//
//	if(hSearch == INVALID_HANDLE_VALUE)
//		return;
//
//	wstring wstrOriginal = wszOriginal;
//	wstring wstrReplacement = wszReplacement;
//	//MakeUpperCase(Original);
//	wstring wstrFileName;
//
//	do
//	{
//		if( 0 != strcmp(fd.cFileName, ".") && 0 != strcmp(fd.cFileName, "..") )
//		{
//			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
//			{			
//				;
//			}
//			else
//			{
//				ConvertCharToWCHAR( wstrFileName, fd.cFileName );
//				//MakeUpperCase( wstrFileName );				
//				int iResult = wstrFileName.find( wstrOriginal );
//				if( 0 == iResult )
//				{
//					continue;
//				}
//				else
//				{
//					wstrFileName.replace( iResult, wstrOriginal.length(), wstrReplacement.c_str() );
//
//					string strNewFileName;
//					ConvertWCHARToChar( strNewFileName, wstrFileName.c_str() );
//					//ConvertWCHARToChar( strNewFileName, wstrPrefix.c_str() );
//					//strNewFileName += fd.cFileName;
//					strNewFileName = strDirectory + strNewFileName;
//
//					string strOldFileName;
//					strOldFileName = strDirectory + fd.cFileName;
//
//					MoveFileA( strOldFileName.c_str(), strNewFileName.c_str() );
//				}				
//			}			
//		}
//	} while( FindNextFileA(hSearch, &fd) );
//
//	FindClose(hSearch);
//}
//
//

