


// 2008-02-19 dmlee
// 엘소드 1차 전직 추가



#include "dxstdafx.h"
#include ".\x2skilltreetool.h"

#include "BasicExcel.hpp"
using namespace YExcel;

using namespace std;


void CALLBACK OnMainDialogEvent_SkillTreeTool( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2SkillTreeTool* pSkillTreeTool = (CX2SkillTreeTool*) pUserContext;
	CDXUTDialog& mainDialog = pSkillTreeTool->GetMainDialog();
	
	OPENFILENAME ofn;				
	WCHAR wszFileName[512] = L"";  // path까지 포함한 파일 이름

	switch( nControlID )
	{
	case CX2SkillTreeTool::IDC_BUTTON_OPEN_SKILL_TEMPLET:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"lua script\0*.lua\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetOpenFileName(&ofn) == TRUE ) 
			{
				pSkillTreeTool->OpenSkillTemplet( ofn.lpstrFile );
			}
		} break;

	case CX2SkillTreeTool::IDC_BUTTON_SAVE_SKILL_TEMPLET:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"lua script\0*.lua\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetSaveFileName(&ofn) == TRUE ) 
			{
				if( true == pSkillTreeTool->SaveSkillTemplet( ofn.lpstrFile ) )
				{
					MessageBox( g_pKTDXApp->GetHWND(), L"저장 되었습니다." , L"^^", MB_OK );
				}
				else
				{
					MessageBox( g_pKTDXApp->GetHWND(), L"읽기전용파일이 아닌지 확인해주세요.", L"파일저장실패", MB_OK );
				}
			}

		} break;

	case CX2SkillTreeTool::IDC_BUTTON_SAVE_SKILL_EXCEL:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"excel\0*.xls\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetSaveFileName(&ofn) == TRUE ) 
			{
				if( true == pSkillTreeTool->SaveExcel( ofn.lpstrFile ) )
				{
					MessageBox( g_pKTDXApp->GetHWND(), L"저장 되었습니다.", L"^^", MB_OK );
				}
				else
				{
					MessageBox( g_pKTDXApp->GetHWND(), L"읽기전용파일이 아닌지 확인해주세요.", L"파일저장실패", MB_OK );
				}
			}

		} break;

	case CX2SkillTreeTool::IDC_COMBO_UNIT_CLASS:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( NULL != pItem )
			{
				if( pSkillTreeTool->GetUnitClass() != (CX2Unit::UNIT_CLASS) (int) pItem->pData )
				{

					D3DXVECTOR2 vNewGridSize = pSkillTreeTool->GetSkillTree()->GetGridSize( (CX2Unit::UNIT_CLASS) (int) pItem->pData );
					if( pSkillTreeTool->GetGridSize() != vNewGridSize )
					{
						pSkillTreeTool->CreateSkillTreeDialog( vNewGridSize );
					}
					pSkillTreeTool->SetUnitClass( (CX2Unit::UNIT_CLASS) (int) pItem->pData );
					
					pSkillTreeTool->UpdateAllGridBySkillTemplet();
					pSkillTreeTool->UpdateAllGridUI();
					pSkillTreeTool->UpdateAvailableSkillList();
				}
			}

		} break;


	case CX2SkillTreeTool::IDC_EDIT_GRID_SIZE:
		{

			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;

					int iValue[2] = { 0, };
					pSkillTreeTool->ParseIntArrayFromEditBox( pEditBox, &iValue[0], 2 );

					D3DXVECTOR2 vGridSize = pSkillTreeTool->GetGridSize();
					if( vGridSize != D3DXVECTOR2( (float) iValue[0], (float)iValue[1] ) )
					{
						pSkillTreeTool->ResizeGrid( iValue[0], iValue[1] ); 
					}

					if( NULL != pSkillTreeTool->GetSkillTree() )
					{
						pSkillTreeTool->GetSkillTree()->SetGridSize( pSkillTreeTool->GetUnitClass(), iValue[0], iValue[1] );
					}

					mainDialog.ClearFocus();

				} break;
			}
			
		} break;



	case CX2SkillTreeTool::IDC_EDIT_MOVE_ALL_GRID:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;

					int iValue[2] = { 0, };
					pSkillTreeTool->ParseIntArrayFromEditBox( pEditBox, &iValue[0], 2 );

					D3DXVECTOR2 vGridSize = pSkillTreeTool->GetGridSize();
					if( vGridSize != D3DXVECTOR2( (float) iValue[0], (float)iValue[1] ) )
					{
						pSkillTreeTool->MoveAllSkillGrid( D3DXVECTOR2( (float)iValue[0], (float) iValue[1] ) );
					}

					mainDialog.ClearFocus();


				} break;
			}

		} break;


	case CX2SkillTreeTool::IDC_BUTTON_HELP:
		{
			wstring wstrHelp;

			wstrHelp += L"1 : main dialog button을 안보이게 \n";
			wstrHelp += L"화살표버튼: 스킬트리를 상하좌우로 이동, 마우스로 드래그 가능 \n";
			wstrHelp += L"숫자키패드 +, - : 스킬트리 확대 축소\n";
			wstrHelp += L"스킬트리 버튼에서 오른쪽 클릭: 스킬 위치를 지정\n";
			wstrHelp += L"grid size editbox : 스킬트리 전체 그리드의 크기를 지정 		예) 5 5 \n";
			wstrHelp += L"move all grid edit box: 스킬트리 전체를 지정된 만큼 이동 		예) 1 2 \n";
			wstrHelp += L"spacebar를 누른 상태에서 스킬그리드에 left click: 현재 선택된 스킬의 필요스킬을 지정 \n";
			wstrHelp += L"\n";
			wstrHelp += L"스크립트를 실행파일과 같은 폴더에 두고 작업해주세요~ \n";
			wstrHelp += L"작업을 마치고 저장하실 때는 lua script와 excel 파일 2개 모두 저장해주세요~";

			MessageBox( g_pKTDXApp->GetHWND(), wstrHelp.c_str(), L"HELP", MB_OK );
		} break;
	}
}


void CALLBACK OnPropertyDialogEvent_SkillTreeTool( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2SkillTreeTool* pSkillTreeTool = (CX2SkillTreeTool*) pUserContext;
	CDXUTDialog& propertyDialog = pSkillTreeTool->GetPropertyDialog();

	OPENFILENAME ofn;				
	WCHAR wszFileName[512] = L"";  // path까지 포함한 파일 이름


	switch( nControlID )
	{
	case CX2SkillTreeTool::IDC_EDIT_TEXTURE_NAME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;

					CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTreeTool->GetPickedSkillTemplet();
					if( NULL == pSkillTemplet )
						return;

					pSkillTemplet->m_wstrTextureName = pEditBox->GetText();
					


					pSkillTemplet = pSkillTreeTool->GetPickedSkillTemplet();
					pEditBox->SetText( pSkillTemplet->m_wstrTextureName.c_str() );

					pSkillTreeTool->UpdateGridUI( (int) pSkillTreeTool->GetPickedGridIndex().x, (int) pSkillTreeTool->GetPickedGridIndex().y );


					propertyDialog.ClearFocus();

				} break;
			}

		} break;

	case CX2SkillTreeTool::IDC_EDIT_TEXTURE_PIECE_NAME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;

					CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTreeTool->GetPickedSkillTemplet();
					if( NULL == pSkillTemplet )
						return;

					pSkillTemplet->m_wstrTexturePieceName = pEditBox->GetText();

					pSkillTemplet = pSkillTreeTool->GetPickedSkillTemplet();
					pEditBox->SetText( pSkillTemplet->m_wstrTexturePieceName.c_str() );

					pSkillTreeTool->UpdateGridUI( (int) pSkillTreeTool->GetPickedGridIndex().x, (int) pSkillTreeTool->GetPickedGridIndex().y );

					propertyDialog.ClearFocus();


				} break;
			}

		} break;


	case CX2SkillTreeTool::IDC_BUTTON_OPEN_TEXTURE:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"tga\0*.tga\0dds\0*.dds";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetOpenFileName(&ofn) == TRUE ) 
			{
				pSkillTreeTool->OpenTextureIconPiece( ofn.lpstrFile );
			}

		} break;

	case CX2SkillTreeTool::IDC_LIST_TEXTURE_PIECE_NAME:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					CDXUTListBox* pListBox = (CDXUTListBox*) pControl;
					DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
					if( NULL == pItem )
						return;
						
					CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTreeTool->GetPickedSkillTemplet();
					if( NULL == pSkillTemplet )
						return;

					pSkillTemplet->m_wstrTextureName = pSkillTreeTool->GetNameTextureIcon();
					pSkillTemplet->m_wstrTexturePieceName = pItem->strText;

					pSkillTreeTool->UpdateGridUI( (int) pSkillTreeTool->GetPickedGridIndex().x, (int) pSkillTreeTool->GetPickedGridIndex().y );
					pSkillTreeTool->UpdatePropertyDialog();

				} break;
			} break;

		} break;
	}
}





CX2SkillTreeTool::CX2SkillTreeTool(void)
{
	char szWindowTitle[1024];
	sprintf( szWindowTitle, "X2 Skill Tree; compiled at %s, %s", __DATE__, __TIME__ );
	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );


	m_pSkillTree = NULL;
	m_vPickedGridIndex = D3DXVECTOR2( -1, -1 );
	m_vGridSize = D3DXVECTOR2( 3, 3 );
	m_pDLGSkillTree = NULL;


	m_vButtonSize =  D3DXVECTOR2( 70, 70 );
	m_vButtonGap = D3DXVECTOR2( 30, 30 );


	m_pDLGSkillInfoPopup = NULL;

	m_bLeftControlPressed = false;


	m_bEnablePopupInfo = true;
	//m_pDialogManager		= new CKTDGUIDialogManager();


	//m_pDeviceTexture = NULL;


	//g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(),	true );
	g_pKTDXApp->GetDGManager()->GetCamera()->Point( 0, 0, 0, 0, 0, 1 );
	g_pKTDXApp->GetDGManager()->GetCamera()->UpdateCamera( 1.0f );


	//m_vSize = D3DXVECTOR2( 400, 400 );
	//m_vLeftTopPos = D3DXVECTOR2(512, 384) - m_vSize * 0.5f;

	InitDialog();
	//UpdateDialog();


	RegisterLuabind();
	
	OpenScriptFile( L"Enum.lua" );
	OpenScriptFile( L"SkillTreeTool_Enum.lua" );
	OpenScriptFile( L"SkillTreeTool_Config.lua" );


	CreateSkillTreeDialog( m_vGridSize );

	m_pDLGSkillList = NULL;
	m_eUnitClass = CX2Unit::UC_NONE;
	

	m_pTexturePickedGrid = NULL;
	//m_pTexturePickedGrid = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"HQ_Standing_Aishya_04.tga" );
	m_pTexturePickedGrid = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"HQ_Huk.dds" );

	m_pTextureIconPiece = NULL;
	m_wstrNameTextureIcon = L"";
}


CX2SkillTreeTool::~CX2SkillTreeTool(void)
{
	SAFE_CLOSE( m_pTexturePickedGrid );
	SAFE_CLOSE( m_pTextureIconPiece );

	FinalDialog();

	SAFE_DELETE_DIALOG( m_pDLGSkillTree );
	SAFE_DELETE_DIALOG( m_pDLGSkillList );
	SAFE_DELETE_DIALOG( m_pDLGSkillInfoPopup );

	SAFE_DELETE( m_pSkillTree );
}

HRESULT CX2SkillTreeTool::OnFrameMove( double fTime, float fElapsedTime )
{

	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );
	g_pKTDXApp->GetDGManager()->OnFrameMove( fTime, fElapsedTime );
	g_pKTDXApp->GetDGManager()->GetCamera()->UpdateCamera( fElapsedTime );



	////m_FPSCamera.FrameMove( fElapsedTime );
	//g_pKTDXApp->GetDGManager()->GetCamera()->Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
	//D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * g_pKTDXApp->GetDGManager()->GetFar()*0.1f + *m_FPSCamera.GetEyePt();
	//g_pKTDXApp->GetDGManager()->GetCamera()->LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
	//g_pKTDXApp->GetDGManager()->GetCamera()->UpdateCamera( fElapsedTime );



	return S_OK;
}

HRESULT CX2SkillTreeTool::OnFrameRender()
{

	g_pKTDXApp->GetDGManager()->ObjectChainSort();
	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
	g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
	g_pKTDXApp->GetDGManager()->FrontUIRender();


	if( m_vPickedGridIndex != D3DXVECTOR2(-1, -1 ) && NULL != m_pDLGSkillTree &&
		m_pTexturePickedGrid != NULL )
	{
		D3DXVECTOR2 vPos = m_pDLGSkillTree->GetPos();
		vPos.x += m_vPickedGridIndex.x * (m_vButtonSize.x + m_vButtonGap.x);
		vPos.y += m_vPickedGridIndex.y * (m_vButtonSize.y + m_vButtonGap.y);

		m_pTexturePickedGrid->Draw( (int)vPos.x, (int)vPos.y, (int)(m_vButtonSize.x*0.5f), (int)(m_vButtonSize.y*0.5f) );
	}
	

	m_MainDialog.OnRender( g_pKTDXApp->GetElapsedTime() );
	m_PropertyDialog.OnRender( g_pKTDXApp->GetElapsedTime() );

	return S_OK;
}

HRESULT CX2SkillTreeTool::OnResetDevice()
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();

	return S_OK;
}

HRESULT CX2SkillTreeTool::OnLostDevice()
{

	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();


	return S_OK;
}

bool CX2SkillTreeTool::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bNoFurtherProcessing = false;
	//m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);


	bNoFurtherProcessing = m_MainDialog.MsgProc( hWnd, uMsg, wParam, lParam );
	if( true == bNoFurtherProcessing )
		return true;

	bNoFurtherProcessing = m_PropertyDialog.MsgProc( hWnd, uMsg, wParam, lParam );
	if( true == bNoFurtherProcessing )
		return true;


	
	switch( uMsg )
	{
	//case CKTDXApp::KM_UI_CONTROL_EVENT:
	//	{
	//		return UIEventProc( hWnd, uMsg, wParam, lParam );	
	//	}
	//	break;

	case CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT:
		{
			return UICustomEventProc( hWnd, uMsg, wParam, lParam );	
		}
		break;

	//case CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC:
	//	{
	//		return UICustomFuncProc( hWnd, uMsg, wParam, lParam );
	//	}
		//	break;

	case WM_DROPFILES:
		{
			OnDropFile( hWnd, uMsg, wParam, lParam );
			return true;

		} break;

	case WM_CHAR:
		switch(wParam)
		{
		case '1':
			{
				m_MainDialog.SetVisible( !m_MainDialog.GetVisible() );
				return true;
			} break;

		case 'a':
		case 'A':
			{
				if( NULL != m_pDLGSkillTree )
				{
					m_pDLGSkillTree->SetPos( m_pDLGSkillTree->GetPos() + D3DXVECTOR2( -m_vButtonSize.x, 0 ) );
				}
			} break;

		case 'd':
		case 'D':
			{
				if( NULL != m_pDLGSkillTree )
				{
					m_pDLGSkillTree->SetPos( m_pDLGSkillTree->GetPos() + D3DXVECTOR2( m_vButtonSize.x, 0 ) );
				}
			} break;
		
		case 'w':
		case 'W':
			{
				if( NULL != m_pDLGSkillTree )
				{
					m_pDLGSkillTree->SetPos( m_pDLGSkillTree->GetPos() + D3DXVECTOR2( 0, -m_vButtonSize.y ) );
				}

			} break;
		
		case 's':
		case 'S':
			{
				if( NULL != m_pDLGSkillTree )
				{
					m_pDLGSkillTree->SetPos( m_pDLGSkillTree->GetPos() + D3DXVECTOR2( 0, m_vButtonSize.y ) );
				}
			} break;


		//case '2':
		//	{
		//		SetEnablePopupInfo( !m_bEnablePopupInfo );
		//		return true;
		//	} break;
		} break;


	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_LEFT:
				{
					if( NULL != m_pDLGSkillTree )
					{
						m_pDLGSkillTree->SetPos( m_pDLGSkillTree->GetPos() + D3DXVECTOR2( -m_vButtonSize.x, 0 ) );
					}
				} break;
			case VK_RIGHT:
				{
					if( NULL != m_pDLGSkillTree )
					{
						m_pDLGSkillTree->SetPos( m_pDLGSkillTree->GetPos() + D3DXVECTOR2( m_vButtonSize.x, 0 ) );
					}
				} break;
			case VK_UP:
				{
					if( NULL != m_pDLGSkillTree )
					{
						m_pDLGSkillTree->SetPos( m_pDLGSkillTree->GetPos() + D3DXVECTOR2( 0, -m_vButtonSize.y ) );
					}

				} break;
			case VK_DOWN:
				{
					if( NULL != m_pDLGSkillTree )
					{
						m_pDLGSkillTree->SetPos( m_pDLGSkillTree->GetPos() + D3DXVECTOR2( 0, m_vButtonSize.y ) );
					}
				} break;

			case VK_ADD:
				{
					ResizeSkillTreeDialog( 2.f );
					return true;

				} break;

			case VK_SUBTRACT:
				{
					ResizeSkillTreeDialog( 0.5f );
					return true;


				} break;

			case VK_SPACE:
				{
					m_bLeftControlPressed = true;

				} break;
			}

		} break;

	case WM_KEYUP:
		{
			switch( wParam )
			{
			case VK_SPACE:
				{
					m_bLeftControlPressed = false;

				} break;
			}
		} break;
	}


	bNoFurtherProcessing = g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam ); 
	if( true == bNoFurtherProcessing )
		return true;

	return bNoFurtherProcessing;
} 



bool CX2SkillTreeTool::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case STTUCM_SKILL_ICON_MOUSE_OVER:
		{
			CKTDGUIButton* pButton	= (CKTDGUIButton*)lParam;
			CKTDGUIDialogType pDialog	= pButton->GetDialog();

			int iValue = (int)wcstol( pButton->GetName(), NULL, 10 );

			
			D3DXVECTOR2 vOverGridIndex(0,0);

			vOverGridIndex.x = (float) (iValue % (int)(m_vGridSize.x) );
			vOverGridIndex.y = (float)(int)(iValue / m_vGridSize.x);


			map< pair<int, int>, SKILL_GRID >::iterator it = m_mapSkillGrid.find( make_pair( (int)vOverGridIndex.x, (int)vOverGridIndex.y ) );
			if( it == m_mapSkillGrid.end() )
				return false;

			OpenSkillInfoPopup( it->second.m_eSkillID );

			return true;

		} break;

	case STTUCM_SKILL_ICON_MOUSE_OUT:
		{
			CloseSkillInfoPopup();

			return true;

		} break;


	case STTUCM_SKILL_ICON_CLICKED:
		{
			CKTDGUIButton* pButton	= (CKTDGUIButton*)lParam;
			CKTDGUIDialogType pDialog	= pButton->GetDialog();

			int iValue = (int)wcstol( pButton->GetName(), NULL, 10 );

			if( true == m_bLeftControlPressed )
			{
				if( NULL == m_pSkillTree )
					return false;

				D3DXVECTOR2 vPickedGridIndex(0, 0);
				vPickedGridIndex.x = (float) (iValue % (int)(m_vGridSize.x) );
				vPickedGridIndex.y = (float)(int)(iValue / m_vGridSize.x);

				
				SKILL_GRID nowPickedSkillGrid = m_mapSkillGrid[ make_pair( (int)vPickedGridIndex.x, (int)vPickedGridIndex.y ) ];
                CX2SkillTree::SkillTemplet* pNowPickedSkillTemplet = m_pSkillTree->GetSkillTemplet( nowPickedSkillGrid.m_eSkillID );
				if( NULL == pNowPickedSkillTemplet )
					return false;



				CX2SkillTree::SkillTemplet* pSkillTemplet = GetPickedSkillTemplet();
				if( NULL == pSkillTemplet )
					return false;

				

				bool bWasBeforeSkill = false;
				for( UINT i=0; i < pSkillTemplet->m_BeforeSkill.size(); i++ )
				{
					CX2SkillTree::SKILL_ID eBeforeSkillID = (CX2SkillTree::SKILL_ID) pSkillTemplet->m_BeforeSkill[i];
					if( nowPickedSkillGrid.m_eSkillID == eBeforeSkillID )
					{
						pSkillTemplet->m_BeforeSkill.erase( pSkillTemplet->m_BeforeSkill.begin() + i );
						bWasBeforeSkill = true;
						break;
					}
				}

				if( false == bWasBeforeSkill )
				{
					pSkillTemplet->m_BeforeSkill.push_back( (int) nowPickedSkillGrid.m_eSkillID );
				}

				UpdateAllGridUI();
				UpdatePickedGridBeforeSkillMark();
				UpdatePropertyDialog();

			}
			else
			{
				m_vPickedGridIndex.x = (float) (iValue % (int)(m_vGridSize.x) );
				m_vPickedGridIndex.y = (float)(int)(iValue / m_vGridSize.x);

				UpdateGridUI( (int)m_vPickedGridIndex.x, (int)m_vPickedGridIndex.y );
				UpdatePickedGridBeforeSkillMark();
				UpdatePropertyDialog();

			}


			

			
			return true;

		} break;

	case STTUCM_SKILL_ICON_DOUBLE_CLICKED:
	case STTUCM_SKILL_ICON_RIGHT_CLICKED:
		{
			CKTDGUIButton* pButton	= (CKTDGUIButton*)lParam;
			CKTDGUIDialogType pDialog	= pButton->GetDialog();

			int iValue = (int)wcstol( pButton->GetName(), NULL, 10 );

			m_vPickedGridIndex.x = (float) (iValue % (int)(m_vGridSize.x) );
			m_vPickedGridIndex.y = (float)(int)(iValue / m_vGridSize.x);

			UpdateGridUI( (int)m_vPickedGridIndex.x, (int)m_vPickedGridIndex.y );

			UpdatePickedGridBeforeSkillMark();
			UpdatePropertyDialog();
			OpenSkillListWindow( true );


			return true;

		} break;

	case STTUCM_SKILL_LIST_SELECTED:
		{
			if( m_vPickedGridIndex != D3DXVECTOR2(-1, -1 ) )
			{
				CKTDGUIListBox* pListBox = (CKTDGUIListBox*)lParam;
				CKTDGUIListBox::ListBoxItem* pListBoxItem = pListBox->GetSelectedItem();
				if( pListBoxItem == NULL )
					return false;

				if( m_pSkillTree == NULL )
					return false;

			
				map< pair<int, int>, SKILL_GRID >::iterator it = m_mapSkillGrid.find( make_pair( (int)m_vPickedGridIndex.x, (int)m_vPickedGridIndex.y ) );
				if( it == m_mapSkillGrid.end() )
					return false;
				
				SKILL_GRID& skillGrid = it->second;

				// 기존에 이 위치에 배치된 스킬이 있다면 지우고
				CX2SkillTree::SkillTemplet* pSkillTemplet = m_pSkillTree->GetSkillTemplet( skillGrid.m_eSkillID );
				if( pSkillTemplet != NULL )
				{
					pSkillTemplet->m_vGridIndex = D3DXVECTOR2(-1, -1);
					skillGrid.m_eSkillID = CX2SkillTree::SI_NONE;
				}

				
				CX2SkillTree::SKILL_ID eSkillID = (CX2SkillTree::SKILL_ID)(int) pListBoxItem->pData;
				pSkillTemplet = m_pSkillTree->GetSkillTemplet( eSkillID );
				if( pSkillTemplet != NULL )
				{
					map< pair<int, int>, SKILL_GRID >::iterator it = m_mapSkillGrid.find( make_pair( (int)pSkillTemplet->m_vGridIndex.x, (int)pSkillTemplet->m_vGridIndex.y ) );
					if( it != m_mapSkillGrid.end() )
					{
						SKILL_GRID& skillGrid = it->second;
						skillGrid.m_eSkillID = CX2SkillTree::SI_NONE;

						UpdateGridUI( (int)pSkillTemplet->m_vGridIndex.x, (int)pSkillTemplet->m_vGridIndex.y );
					}

					
					pSkillTemplet->m_vGridIndex = m_vPickedGridIndex;
					skillGrid.m_eSkillID = eSkillID;
				}
			}

			OpenSkillListWindow( false );

			UpdatePropertyDialog();
			UpdateGridUI( (int)m_vPickedGridIndex.x, (int)m_vPickedGridIndex.y );

			UpdateAvailableSkillList();

			return true;
		} break;


	case STTUCM_SKILL_LIST_CLOSE:
		{
			OpenSkillListWindow( false );
			return true;
		} break;
	}

	return false;
}



void CX2SkillTreeTool::ResizeSkillTreeDialog( float fScale )
{
	if( NULL == m_pDLGSkillTree )
		return;

	if( m_vButtonSize.x * fScale < 1.f )
		return;

	if( m_vButtonSize.x * fScale > 1000.f )
		return;

	m_vButtonSize *= fScale;
	m_vButtonGap *= fScale;
	


	CKTDGUIStatic* pStatic_Skill_Icon = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Skill_Icon" );
	if( NULL == pStatic_Skill_Icon )
		return;

	CKTDGUIStatic* pStatic_Before_Skill_Mark = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Before_Skill_Mark" );
	if( NULL == pStatic_Before_Skill_Mark )
		return;



	WCHAR wszText[256] = L"";
	D3DXVECTOR2 vButtonPos(0, 0 );
	for( int iRow=0; iRow<m_vGridSize.y; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x; iCol++ )
		{
			CKTDGUIControl::CPictureData* pPicture = pStatic_Skill_Icon->GetPicture( (int)(iRow*m_vGridSize.x + iCol) );
			if( NULL == pPicture )
				continue;
			pPicture->SetSize( m_vButtonSize );
			pPicture->SetPos( vButtonPos );


			pPicture = pStatic_Before_Skill_Mark->GetPicture( (int)(iRow*m_vGridSize.x + iCol) );
			if( NULL == pPicture )
				continue;
			pPicture->SetSize( m_vButtonSize * 0.5f );
			pPicture->SetPos( vButtonPos );




			wsprintf( wszText, L"%d", (int)(iCol+ iRow*m_vGridSize.x) );
			CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGSkillTree->GetControl( wszText );


			CKTDGUIControl::UIPointData* pUIPoint_Normal = new CKTDGUIControl::UIPointData;
			pUIPoint_Normal->bUseTextureSize = false;
			pUIPoint_Normal->fChangeTime = 0.f;
			pUIPoint_Normal->addSize = D3DXVECTOR2( 0, 0 );
			pUIPoint_Normal->leftTopPoint = vButtonPos;
			pUIPoint_Normal->rightTopPoint = vButtonPos + D3DXVECTOR2( m_vButtonSize.x, 0 );
			pUIPoint_Normal->leftBottomPoint = vButtonPos + D3DXVECTOR2( 0, m_vButtonSize.y );
			pUIPoint_Normal->rightBottomPoint = vButtonPos + m_vButtonSize;
			pUIPoint_Normal->color = D3DXCOLOR( 1, 1, 1, 1 );
			pButton->SetNormalPoint( pUIPoint_Normal );


			CKTDGUIControl::UIPointData* pUIPoint_Over = new CKTDGUIControl::UIPointData;
			pUIPoint_Over->bUseTextureSize = false;
			pUIPoint_Over->fChangeTime = 0.f;
			pUIPoint_Over->addSize = D3DXVECTOR2( 0, 0 );
			pUIPoint_Over->leftTopPoint		= vButtonPos - D3DXVECTOR2( m_vButtonSize.x*0.1f, m_vButtonSize.y*0.1f );
			pUIPoint_Over->rightTopPoint	= vButtonPos + D3DXVECTOR2( m_vButtonSize.x, 0 ) + D3DXVECTOR2( m_vButtonSize.x*0.1f, -m_vButtonSize.y*0.1f );
			pUIPoint_Over->leftBottomPoint	= vButtonPos + D3DXVECTOR2( 0, m_vButtonSize.y ) + D3DXVECTOR2( -m_vButtonSize.x*0.1f, m_vButtonSize.y*0.1f );;
			pUIPoint_Over->rightBottomPoint = vButtonPos + m_vButtonSize + D3DXVECTOR2( m_vButtonSize.x*0.1f, m_vButtonSize.y*0.1f );
			pUIPoint_Over->color = D3DXCOLOR( 1, 1, 1, 1 );
			pButton->SetOverPoint( pUIPoint_Over );



			CKTDGUIControl::UIPointData* pUIPoint_Down = new CKTDGUIControl::UIPointData;
			pUIPoint_Down->bUseTextureSize = false;
			pUIPoint_Down->fChangeTime = 0.f;
			pUIPoint_Down->addSize = D3DXVECTOR2( 0, 0 );
			pUIPoint_Down->leftTopPoint		= vButtonPos;
			pUIPoint_Down->rightTopPoint	= vButtonPos + D3DXVECTOR2( m_vButtonSize.x, 0 );
			pUIPoint_Down->leftBottomPoint	= vButtonPos + D3DXVECTOR2( 0, m_vButtonSize.y );
			pUIPoint_Down->rightBottomPoint = vButtonPos + m_vButtonSize;
			pUIPoint_Down->color = D3DXCOLOR( 1, 1, 1, 1 );
			pButton->SetDownPoint( pUIPoint_Down );




			vButtonPos.x += m_vButtonSize.x + m_vButtonGap.x;
		}
		vButtonPos.x = 0.f;
		vButtonPos.y += m_vButtonSize.y + m_vButtonGap.y;
	}


	D3DXVECTOR2 vArrowPos = m_vButtonSize;
	vArrowPos.x *= 0.5f;

	CKTDGUIStatic* pStatic_Skill_Vert_Arrow = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Skill_Vert_Arrow" );
	if( NULL == pStatic_Skill_Vert_Arrow )
		return;

	for( int iRow=0; iRow<m_vGridSize.y-1; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x; iCol++ )
		{
			CKTDGUIControl::CPictureData* pPicture = pStatic_Skill_Vert_Arrow->GetPicture( (int)(iRow*m_vGridSize.x + iCol) );
			if( NULL == pPicture )
				continue;

			pPicture->SetSize( m_vButtonGap );
			pPicture->SetPos( vArrowPos );


			vArrowPos.x += m_vButtonSize.x + m_vButtonGap.x;
		}
		vArrowPos.x = m_vButtonSize.x * 0.5f;
		vArrowPos.y += m_vButtonSize.y + m_vButtonGap.y;
	}




	vArrowPos = m_vButtonSize;
	vArrowPos.y *= 0.5f;
	CKTDGUIStatic* pStatic_Skill_Hor_Arrow = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Skill_Hor_Arrow" );
	if( NULL == pStatic_Skill_Hor_Arrow )
		return;


	for( int iRow=0; iRow<m_vGridSize.y; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x-1; iCol++ )
		{

			CKTDGUIControl::CPictureData* pPicture = pStatic_Skill_Hor_Arrow->GetPicture( (int)(iRow*(m_vGridSize.x-1) + iCol) );
			if( NULL == pPicture )
				continue;

			pPicture->SetSize( m_vButtonGap );
			pPicture->SetPos( vArrowPos );

			vArrowPos.x += m_vButtonSize.x + m_vButtonGap.x;
		}
		vArrowPos.x = m_vButtonSize.x;
		vArrowPos.y += m_vButtonSize.y + m_vButtonGap.y;
	}



	D3DXVECTOR2 vBackGroundSize;
	vBackGroundSize.x = m_vGridSize.x * m_vButtonSize.x + (m_vGridSize.x-1.f) * m_vButtonGap.x;
	vBackGroundSize.y = m_vGridSize.y * m_vButtonSize.y + (m_vGridSize.y-1.f) * m_vButtonGap.y;

	CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Background" );
	if(pStatic_BG != NULL)
		pStatic_BG->GetPicture(0)->SetSize( vBackGroundSize );

	
}


void CX2SkillTreeTool::CreateSkillTreeDialog( D3DXVECTOR2 vGridSize )
{	
	SAFE_DELETE_DIALOG( m_pDLGSkillTree );
	m_pDLGSkillTree = new CKTDGUIDialog( (CKTDXStage*) this, L"DLG_Tool_SkillTreeMain.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillTree );

	if( m_pDLGSkillTree == NULL )
		return;

	
	m_vGridSize = vGridSize;
	m_mapSkillGrid.clear();
	
	WCHAR wszText[256] = L"";
	D3DXVECTOR2 vButtonPos(0, 0);



	for( int iRow=0; iRow<m_vGridSize.y; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x; iCol++ )
		{
			CKTDGUIButton* pButton = new CKTDGUIButton();
			m_pDLGSkillTree->AddControl( (CKTDGUIControl*) pButton );
			wsprintf( wszText, L"%d", (int)(iCol+ iRow*m_vGridSize.x) );
			pButton->SetName( wszText );
			pButton->SetCustomMsgMouseUp( STTUCM_SKILL_ICON_CLICKED );
			pButton->SetCustomMsgMouseRightUp( STTUCM_SKILL_ICON_RIGHT_CLICKED );
			pButton->SetCustomMsgMouseDblClk( STTUCM_SKILL_ICON_DOUBLE_CLICKED );
			pButton->SetCustomMsgMouseOver( STTUCM_SKILL_ICON_MOUSE_OVER );
			pButton->SetCustomMsgMouseOut( STTUCM_SKILL_ICON_MOUSE_OUT );

			pButton->SetNormalTex( L"DLG_COMMON_TEXTURE01.TGA", L"Radio_Button_Normal" );
			pButton->SetOverTex( L"DLG_COMMON_TEXTURE01.TGA", L"Radio_Button_Over" );
			pButton->SetDownTex( L"DLG_COMMON_TEXTURE01.TGA", L"Radio_Button_Down" );


			CKTDGUIControl::UIPointData* pUIPoint_Normal = new CKTDGUIControl::UIPointData;
			pUIPoint_Normal->bUseTextureSize = false;
			pUIPoint_Normal->fChangeTime = 0.f;
			pUIPoint_Normal->addSize = D3DXVECTOR2( 0, 0 );
			pUIPoint_Normal->leftTopPoint = vButtonPos;
			pUIPoint_Normal->rightTopPoint = vButtonPos + D3DXVECTOR2( m_vButtonSize.x, 0 );
			pUIPoint_Normal->leftBottomPoint = vButtonPos + D3DXVECTOR2( 0, m_vButtonSize.y );
			pUIPoint_Normal->rightBottomPoint = vButtonPos + m_vButtonSize;
			pUIPoint_Normal->color = D3DXCOLOR( 1, 1, 1, 1 );
			pButton->SetNormalPoint( pUIPoint_Normal );


			CKTDGUIControl::UIPointData* pUIPoint_Over = new CKTDGUIControl::UIPointData;
			pUIPoint_Over->bUseTextureSize = false;
			pUIPoint_Over->fChangeTime = 0.f;
			pUIPoint_Over->addSize = D3DXVECTOR2( 0, 0 );
			pUIPoint_Over->leftTopPoint		= vButtonPos - D3DXVECTOR2( m_vButtonSize.x*0.1f, m_vButtonSize.y*0.1f );
			pUIPoint_Over->rightTopPoint	= vButtonPos + D3DXVECTOR2( m_vButtonSize.x, 0 ) + D3DXVECTOR2( m_vButtonSize.x*0.1f, -m_vButtonSize.y*0.1f );
			pUIPoint_Over->leftBottomPoint	= vButtonPos + D3DXVECTOR2( 0, m_vButtonSize.y ) + D3DXVECTOR2( -m_vButtonSize.x*0.1f, m_vButtonSize.y*0.1f );;
			pUIPoint_Over->rightBottomPoint = vButtonPos + m_vButtonSize + D3DXVECTOR2( m_vButtonSize.x*0.1f, m_vButtonSize.y*0.1f );
			pUIPoint_Over->color = D3DXCOLOR( 1, 1, 1, 1 );
			pButton->SetOverPoint( pUIPoint_Over );



			CKTDGUIControl::UIPointData* pUIPoint_Down = new CKTDGUIControl::UIPointData;
			pUIPoint_Down->bUseTextureSize = false;
			pUIPoint_Down->fChangeTime = 0.f;
			pUIPoint_Down->addSize = D3DXVECTOR2( 0, 0 );
			pUIPoint_Down->leftTopPoint		= vButtonPos;
			pUIPoint_Down->rightTopPoint	= vButtonPos + D3DXVECTOR2( m_vButtonSize.x, 0 );
			pUIPoint_Down->leftBottomPoint	= vButtonPos + D3DXVECTOR2( 0, m_vButtonSize.y );
			pUIPoint_Down->rightBottomPoint = vButtonPos + m_vButtonSize;
			pUIPoint_Down->color = D3DXCOLOR( 1, 1, 1, 1 );
			pButton->SetDownPoint( pUIPoint_Down );

			vButtonPos.x += m_vButtonSize.x + m_vButtonGap.x;



			SKILL_GRID skillGrid;
			//skillGrid.m_pButton = pButton;
			m_mapSkillGrid[ make_pair( iCol, iRow ) /*D3DXVECTOR2( (float)iCol, (float)iRow ) */] = skillGrid;

			
		}
		vButtonPos.x = 0.f;
		vButtonPos.y += m_vButtonSize.y + m_vButtonGap.y;
	}


	vButtonPos = D3DXVECTOR2(0, 0 );

	CKTDGUIStatic* pStatic_Skill_Icon = new CKTDGUIStatic();
	pStatic_Skill_Icon->SetName( L"Skill_Icon" );
	m_pDLGSkillTree->AddControl( (CKTDGUIControl*) pStatic_Skill_Icon );

	for( int iRow=0; iRow<m_vGridSize.y; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x; iCol++ )
		{
			CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData;
			pStatic_Skill_Icon->AddPicture( pPicture );
			pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"FRONT_ROAD" );
			pPicture->SetSize( m_vButtonSize );
			pPicture->SetPos( vButtonPos );

			vButtonPos.x += m_vButtonSize.x + m_vButtonGap.x;

		}
		vButtonPos.x = 0.f;
		vButtonPos.y += m_vButtonSize.y + m_vButtonGap.y;
	}




	// before skill 을 표시해주는 그림
	CKTDGUIStatic* pStatic_Before_Skill_Mark = new CKTDGUIStatic();
	pStatic_Before_Skill_Mark->SetName( L"Before_Skill_Mark" );
	m_pDLGSkillTree->AddControl( (CKTDGUIControl*) pStatic_Before_Skill_Mark );


	vButtonPos = D3DXVECTOR2(0, 0 );
	for( int iRow=0; iRow<m_vGridSize.y; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x; iCol++ )
		{
			CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData;
			pStatic_Before_Skill_Mark->AddPicture( pPicture );
			pPicture->SetTex( L"DLG_Keys.tga", L"KEY_S" );
			pPicture->SetSize( m_vButtonSize * 0.5f );
			pPicture->SetPos( vButtonPos );
			pPicture->SetShow( false );

			m_mapSkillGrid[ make_pair(iCol, iRow) ].m_pBeforeSkillMarkPicture = pPicture;

			vButtonPos.x += m_vButtonSize.x + m_vButtonGap.x;

		}
		vButtonPos.x = 0.f;
		vButtonPos.y += m_vButtonSize.y + m_vButtonGap.y;
	}

	


	D3DXVECTOR2 vArrowPos = m_vButtonSize;
	vArrowPos.x *= 0.5f;

	CKTDGUIStatic* pStatic_Skill_Vert_Arrow = new CKTDGUIStatic();
	pStatic_Skill_Vert_Arrow->SetName( L"Skill_Vert_Arrow" );
	m_pDLGSkillTree->AddControl( (CKTDGUIControl*) pStatic_Skill_Vert_Arrow );

	for( int iRow=0; iRow<m_vGridSize.y-1; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x; iCol++ )
		{
			CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData;
			pStatic_Skill_Vert_Arrow->AddPicture( pPicture );
			pPicture->SetTex( L"DLG_Keys.tga", L"KEY_RIGHT" );
			pPicture->SetSize( m_vButtonGap );
			pPicture->SetPos( vArrowPos );
			//pPicture->RotatePoint( vArrowPos+m_vButtonGap*0.5f, 90.f );
			pPicture->SetShow( false );

			vArrowPos.x += m_vButtonSize.x + m_vButtonGap.x;
		}
		vArrowPos.x = m_vButtonSize.x * 0.5f;
		vArrowPos.y += m_vButtonSize.y + m_vButtonGap.y;
	}




	vArrowPos = m_vButtonSize;
	vArrowPos.y *= 0.5f;

	CKTDGUIStatic* pStatic_Skill_Hor_Arrow = new CKTDGUIStatic();
	pStatic_Skill_Hor_Arrow->SetName( L"Skill_Hor_Arrow" );
	m_pDLGSkillTree->AddControl( (CKTDGUIControl*) pStatic_Skill_Hor_Arrow );

	for( int iRow=0; iRow<m_vGridSize.y; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x-1; iCol++ )
		{
			CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData;
			pStatic_Skill_Hor_Arrow->AddPicture( pPicture );
			pPicture->SetTex( L"DLG_Keys.tga", L"KEY_RIGHT" );
			pPicture->SetSize( m_vButtonGap );
			pPicture->SetPos( vArrowPos );
			pPicture->SetShow( false );

			vArrowPos.x += m_vButtonSize.x + m_vButtonGap.x;
		}
		vArrowPos.x = m_vButtonSize.x;
		vArrowPos.y += m_vButtonSize.y + m_vButtonGap.y;
	}


	//CKTDGUIStatic* pStatic_PickedGrid = new CKTDGUIStatic;
	//m_pDLGSkillTree->AddControl( pStatic_PickedGrid );
	//pStatic_PickedGrid->SetName( L"Picked_Grid" );


	//CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData;
	//pStatic_Skill_Icon->AddPicture( pPicture );
	//pPicture->SetTex( L"DLG_Common_Texture01.tga", L"X_Icon" );
	//pPicture->SetSize( m_vButtonSize );
	//pPicture->SetPos( D3DXVECTOR2( 0,0 ) );
	//pPicture->SetColor( D3DXCOLOR(1,1,1,0.3f) );


	D3DXVECTOR2 vBackGroundSize;
	vBackGroundSize.x = m_vGridSize.x * m_vButtonSize.x + (m_vGridSize.x-1.f) * m_vButtonGap.x;
	vBackGroundSize.y = m_vGridSize.y * m_vButtonSize.y + (m_vGridSize.y-1.f) * m_vButtonGap.y;
	
	CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Background" );
	if(pStatic_BG != NULL)
		pStatic_BG->GetPicture(0)->SetSize( vBackGroundSize );




	UpdateMainDialog();

}


bool CX2SkillTreeTool::OpenSkillTemplet( const WCHAR* wszFilePath )
{
	SAFE_DELETE( m_pSkillTree );

	//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	//Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"Enum.lua" );
	//if( Info == NULL )
	//{
	//	MessageBox( g_pKTDXApp->GetHWND(), L"파일 열기 실패" , L"^^", MB_OK );
	//	return false;
	//}

	//if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	//{
	//	MessageBox( g_pKTDXApp->GetHWND(), L"파일 열기 실패" , L"^^", MB_OK );
	//	return false;
	//}


	WCHAR wszPath[MAX_PATH] = L"";	
	WCHAR wszFileName[MAX_PATH] = L"";	

	DecomposePathAndFileName( wszPath, wszFileName, wszFilePath );
	

	m_pSkillTree = new CX2SkillTree;
	if( false == m_pSkillTree->OpenScriptFile( wszFileName ) )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"파일 열기 실패" , L"^^", MB_OK );
		return false;
	}

	//if( m_vGridSize != m_pSkillTree->GetGridSize() )
	//{
		CreateSkillTreeDialog( m_pSkillTree->GetGridSize( m_eUnitClass ) );
	//}


	if( NULL == m_pDLGSkillTree || NULL == m_pDLGSkillTree->GetControl( L"Skill_Icon" ) )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"파일 열기 실패" , L"^^", MB_OK );
		return false;
	}

	
	UpdateAllGridBySkillTemplet();
	//CKTDGUIStatic* pStatic_Skill_Icon = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Skill_Icon" );
	//for( int i=0; i<pStatic_Skill_Icon->GetPictureNum(); i++ )
	//{
	//	CKTDGUIControl::CPictureData* pPicture = pStatic_Skill_Icon->GetPicture(i);
	//	pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"FRONT_ROAD" );
	//}
	//
	//	
	//const map<int, CX2SkillTree::SkillTemplet*>& mapSkillTemplet = m_pSkillTree->GetSkillTempletMap();
	//map<int, CX2SkillTree::SkillTemplet*>::const_iterator it;

	//for( it = mapSkillTemplet.begin(); it != mapSkillTemplet.end(); it++ )
	//{
	//	CX2SkillTree::SkillTemplet* pSkillTemplet = it->second;

	//	if( pSkillTemplet->m_UnitClass != m_eUnitClass )
	//		continue;


	//	map< pair<int, int>, SKILL_GRID >::iterator it = m_mapSkillGrid.find( make_pair( (int)pSkillTemplet->m_vGridIndex.x, (int)pSkillTemplet->m_vGridIndex.y ) );
	//	if( it == m_mapSkillGrid.end() )
	//		continue;

	//	SKILL_GRID& skillGrid = it->second;
	//	skillGrid.m_eSkillID = pSkillTemplet->m_ID;
	////	//if( NULL != skillGrid.m_pButton )
	////	if( pSkillTemplet->m_vGridIndex != D3DXVECTOR2(-1, -1) )
	////	{
	////		//if( pSkillTemplet->m_vGridIndex.x < 0.f || pSkillTemplet->m_vGridIndex.x > m_vGridSize.x-1 )
	////		//{
	////		//	pSkillTemplet->m_vGridIndex.x = 0.f;
	////		//}

	////		//if( pSkillTemplet->m_vGridIndex.y < 0.f || pSkillTemplet->m_vGridIndex.y > m_vGridSize.y-1 )
	////		//{
	////		//	pSkillTemplet->m_vGridIndex.y = 0.f;
	////		//}
	////		//
	////		int iPictureIndex = (int)(pSkillTemplet->m_vGridIndex.y * m_pSkillTree->GetGridSize().x + pSkillTemplet->m_vGridIndex.x);
	////		CKTDGUIControl::CPictureData* pPicture = pStatic_Skill_Icon->GetPicture( iPictureIndex );
	////		
	////		assert( pPicture != NULL );

	////		if( pSkillTemplet->m_wstrTextureName.length() > 0 && pSkillTemplet->m_wstrTexturePieceName.length() > 0 )
	////		{
	////			if( false == pPicture->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
	////			{
	////				pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"BOSS_MARK" );
	////			}
	////		}
	////		else
	////		{
	////			pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"BOSS_MARK" );
	////		}
	////	}
	//}



	UpdateAllGridUI();
	UpdateAvailableSkillList();


	SetWindowText( g_pKTDXApp->GetHWND(), wszFilePath );

	//MessageBox( g_pKTDXApp->GetHWND(), L"파일 열기 성공" , L"^^", MB_OK );
	return true;
}


bool CX2SkillTreeTool::SaveSkillTemplet( const WCHAR* wszFilePath )
{
	if( NULL == m_pSkillTree )
		return false;

	if( NULL == wszFilePath )
		return false;

	const map<int, CX2SkillTree::SkillTemplet*>& mapSkillTemplet = m_pSkillTree->GetSkillTempletMap();
	map<int, CX2SkillTree::SkillTemplet*>::const_iterator it;
	//WCHAR wszText[1024] = L"";

	// 파일 확장자가 없으면 붙여주자.
	std::string strFileName = "";	
	//strFileName.reserve(512);
	ConvertWCHARToChar( strFileName, wszFilePath );	
	MakeUpperCase( strFileName );

	int iStrLen = strFileName.length();
	if( iStrLen < 5 )
		return false;

	if( '.' != strFileName[iStrLen-4] || 'L' != strFileName[iStrLen-3] || 'U' != strFileName[iStrLen-2] || 'A' != strFileName[iStrLen-1] ) 
	{
		ConvertWCHARToChar( strFileName, wszFilePath );	
		strFileName.append(".lua");
	}
	else
	{
		ConvertWCHARToChar( strFileName, wszFilePath );	
	}

	//std::wstring wstrFilePath;
	//ConvertCharToWCHAR( wstrFilePath, strFileName.c_str() );

	//std::ofstream ofs( strFileName.c_str(), ios::binary );
	std::ofstream ofs( strFileName.c_str() );
	if( ofs.is_open() == false )
		return false;



	ofs << endl;
	std::map< CX2Unit::UNIT_CLASS, D3DXVECTOR2 >::iterator it_gridsize;


	// note!! 전직 추가되면 수정
	D3DXVECTOR2 vGridSize(0,0);
	// 엘소드
	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_SWORDMAN );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_ELSWORD_SWORDMAN\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;

	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_KNIGHT );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_ELSWORD_KNIGHT\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;

	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_MAGIC_KNIGHT );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_ELSWORD_MAGIC_KNIGHT\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;


	// 아이샤
	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_VIOLET_MAGE );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_ARME_VIOLET_MAGE\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;

	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_HIGH_MAGICIAN );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_ARME_HIGH_MAGICIAN\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;

	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_DARK_MAGICIAN );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_ARME_DARK_MAGICIAN\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;


	// 레나
	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_ELVEN_RANGER );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_LIRE_ELVEN_RANGER\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;

	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_COMBAT_RANGER );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_LIRE_COMBAT_RANGER\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;

	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_SNIPING_RANGER );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_LIRE_SNIPING_RANGER\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;


	// 레이븐
	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_FIGHTER );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_RAVEN_FIGHTER\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;

	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_SOUL_TAKER );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_RAVEN_SOUL_TAKER\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;

	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_OVER_TAKER );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_RAVEN_OVER_TAKER\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;



	// 이브
	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_EVE_NASOD );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_EVE_NASOD\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;
	
	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_EVE_EXOTIC_GEAR );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_EVE_EXOTIC_GEAR\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;
	
	vGridSize = m_pSkillTree->GetGridSize( CX2Unit::UC_EVE_ARCHITECTURE );
	ofs << "g_pSkillTree:SetGridSize( UNIT_CLASS[\"UC_EVE_ARCHITECTURE\"], " << (int)vGridSize.x << ", " << (int)vGridSize.y << ")" << endl;





	ofs << endl;
	ofs << endl;

	string strTemp = "";
	for( it = mapSkillTemplet.begin(); it != mapSkillTemplet.end(); it++ )
	{
		CX2SkillTree::SkillTemplet* pSkillTemplet = it->second;
		if( NULL == pSkillTemplet )
			continue;


		ofs << endl;
		ofs << "g_pSkillTree:AddSkillTemplet_LUA" << endl;
		ofs << "{" << endl;
		
		ofs << "\t" << "m_ID = " << (int)pSkillTemplet->m_ID << ", " << endl;
		ofs << "\t" << "m_Type = SKILL_TYPE[\"";
		switch( pSkillTemplet->m_Type )
		{
		case CX2SkillTree::ST_NONE:
			{
				ofs << "ST_NONE";
			} break;
		case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
			{
				ofs << "ST_PASSIVE_PHYSIC_ATTACK";
			} break;
		case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
			{
				ofs << "ST_PASSIVE_MAGIC_ATTACK";
			} break;
		case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
			{
				ofs << "ST_PASSIVE_MAGIC_DEFENCE";
			} break;

		case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
			{
				ofs << "ST_PASSIVE_PHYSIC_DEFENCE";
			} break;

		case CX2SkillTree::ST_ACTIVE_1:
			{
				ofs << "ST_ACTIVE_1";
			} break;
		case CX2SkillTree::ST_ACTIVE_2:
			{
				ofs << "ST_ACTIVE_2";
			} break;
		case CX2SkillTree::ST_ACTIVE_3:
			{
				ofs << "ST_ACTIVE_3";
			} break;
		}
		ofs << "\"]," << endl;
		
		ConvertWCHARToChar( strTemp, pSkillTemplet->m_Name.c_str() );
		ReplaceString( strTemp, "\n", "\\n" );
		ReplaceString( strTemp, "\t", "\\t" );
		ofs << "\t" << "m_Name = \"" << strTemp << "\"," << endl;
		ConvertWCHARToChar( strTemp, pSkillTemplet->m_Desc.c_str() );
		ReplaceString( strTemp, "\n", "\\n" );
		ReplaceString( strTemp, "\t", "\\t" );
		ofs << "\t" << "m_Desc = \"" << strTemp << "\"," << endl;
		if( true == pSkillTemplet->m_bShow )
		{
			ofs << "\t" << "m_Show = TRUE," << endl;
		}
		else
		{
			ofs << "\t" << "m_Show = FALSE," << endl;
		}

		ofs << "\t" << "m_iSkillLevel = " << pSkillTemplet->m_iSkillLevel << ", " << endl;


		// note!! 전직 추가되면 수정
		ofs << "\t" << "m_UnitClass = UNIT_CLASS[\"";
		switch( pSkillTemplet->m_UnitClass )
		{
		case CX2Unit::UC_NONE:			
			{
				ofs << "UC_NONE";
			} break;

		case CX2Unit::UC_ELSWORD_SWORDMAN:			
			{
				ofs << "UC_ELSWORD_SWORDMAN";
			} break;

		case CX2Unit::UC_ARME_VIOLET_MAGE:			
			{
				ofs << "UC_ARME_VIOLET_MAGE";
			} break;

		case CX2Unit::UC_ARME_HIGH_MAGICIAN:			
			{
				ofs << "UC_ARME_HIGH_MAGICIAN";
			} break;

		case CX2Unit::UC_ARME_DARK_MAGICIAN:			
			{
				ofs << "UC_ARME_DARK_MAGICIAN";
			} break;

		case CX2Unit::UC_LIRE_ELVEN_RANGER:		
			{
				ofs << "UC_LIRE_ELVEN_RANGER";
			} break;

		case CX2Unit::UC_ELSWORD_KNIGHT:
			{
				ofs << "UC_ELSWORD_KNIGHT";
			} break;

		case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
			{
				ofs << "UC_ELSWORD_MAGIC_KNIGHT";
			} break;

		case CX2Unit::UC_LIRE_COMBAT_RANGER:		
			{
				ofs << "UC_LIRE_COMBAT_RANGER";
			} break;

		case CX2Unit::UC_LIRE_SNIPING_RANGER:		
			{
				ofs << "UC_LIRE_SNIPING_RANGER";
			} break;

		case CX2Unit::UC_RAVEN_FIGHTER:			
			{
				ofs << "UC_RAVEN_FIGHTER";
			} break;

		case CX2Unit::UC_RAVEN_SOUL_TAKER:			
			{
				ofs << "UC_RAVEN_SOUL_TAKER";
			} break;

		case CX2Unit::UC_RAVEN_OVER_TAKER:			
			{
				ofs << "UC_RAVEN_OVER_TAKER";
			} break;



		case CX2Unit::UC_EVE_NASOD:			
			{
				ofs << "UC_EVE_NASOD";
			} break;

		case CX2Unit::UC_EVE_EXOTIC_GEAR:
			{
				ofs << "UC_EVE_EXOTIC_GEAR";
			}
			break;
		case CX2Unit::UC_EVE_ARCHITECTURE:
			{
				ofs << "UC_EVE_ARCHITECTURE";
			}
			break;


		}
		ofs << "\"]," << endl;
		ofs << "\t" << "m_NeedLevel = " << pSkillTemplet->m_NeedLevel << ", " << endl;
		ofs << "\t" << "m_NeedSP = " << pSkillTemplet->m_NeedSP << ", " << endl;
		ofs << "\t" << "m_NeedItemID = " << pSkillTemplet->m_NeedItemID << ", " << endl;
		ofs << "\t" << "m_NeedQuestID = " << pSkillTemplet->m_NeedQuestID << ", " << endl;
		ofs << endl;

		ofs << "\t" << "m_BeforeSkill = " << endl;
		ofs << "\t" << "{" << endl;
		for( UINT i=0; i<pSkillTemplet->m_BeforeSkill.size(); i++ )
		{
			CX2SkillTree::SkillTemplet* pBeforeSkillTemplet = 
				m_pSkillTree->GetSkillTemplet( (CX2SkillTree::SKILL_ID) pSkillTemplet->m_BeforeSkill[i] );
			if( NULL != pBeforeSkillTemplet )
			{
				ofs << "\t" << "\t" << (int)pSkillTemplet->m_BeforeSkill[i] << ", " << endl;
			}
		}
		ofs << "\t" << "}," << endl;

		ofs << endl;

		ofs << "\t" << "m_Stat =" << endl;
		ofs << "\t" << "{" << endl;
		ofs << "\t" << "\t" << "m_fBaseHP		" << " = " << pSkillTemplet->m_Stat.m_fBaseHP		 << "," << endl;
		ofs << "\t" << "\t" << "m_fAtkPhysic	" << " = " << pSkillTemplet->m_Stat.m_fAtkPhysic	 << "," << endl;
		ofs << "\t" << "\t" << "m_fAtkMagic		" << " = " << pSkillTemplet->m_Stat.m_fAtkMagic		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fAtkFire		" << " = " << pSkillTemplet->m_Stat.m_fAtkFire		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fAtkIce		" << " = " << pSkillTemplet->m_Stat.m_fAtkIce		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fAtkLightning" << " = " << pSkillTemplet->m_Stat.m_fAtkLightning	<< "," << endl;
		//ofs << "\t" << "\t" << "m_fAtkEarth		" << " = " << pSkillTemplet->m_Stat.m_fAtkEarth		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fAtkLight		" << " = " << pSkillTemplet->m_Stat.m_fAtkLight		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fAtkDark		" << " = " << pSkillTemplet->m_Stat.m_fAtkDark		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fAtkUniversal" << " = " << pSkillTemplet->m_Stat.m_fAtkUniversal	<< "," << endl;
		ofs << "\t" << "\t" << "m_fDefPhysic	" << " = " << pSkillTemplet->m_Stat.m_fDefPhysic	 << "," << endl;
		ofs << "\t" << "\t" << "m_fDefMagic		" << " = " << pSkillTemplet->m_Stat.m_fDefMagic		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fDefFire		" << " = " << pSkillTemplet->m_Stat.m_fDefFire		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fDefIce		" << " = " << pSkillTemplet->m_Stat.m_fDefIce		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fDefLightning" << " = " << pSkillTemplet->m_Stat.m_fDefLightning	<< "," << endl;
		//ofs << "\t" << "\t" << "m_fDefEarth		" << " = " << pSkillTemplet->m_Stat.m_fDefEarth		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fDefLight		" << " = " << pSkillTemplet->m_Stat.m_fDefLight		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fDefDark		" << " = " << pSkillTemplet->m_Stat.m_fDefDark		 << "," << endl;
		//ofs << "\t" << "\t" << "m_fDefUniversal" << " = " << pSkillTemplet->m_Stat.m_fDefUniversal	<< "," << endl;

		ofs << "\t" << "}," << endl;
		ofs << endl;


		if( 0 != pSkillTemplet->m_wstrTextureName.length() )
		{
			ConvertWCHARToChar( strTemp, pSkillTemplet->m_wstrTextureName.c_str() );
			ofs << "\t" << "m_wstrTextureName = \"" << strTemp << "\", " << endl;
		}

		if( 0 != pSkillTemplet->m_wstrTexturePieceName.length() )
		{
			ConvertWCHARToChar( strTemp, pSkillTemplet->m_wstrTexturePieceName.c_str() );
			ofs << "\t" << "m_wstrTexturePieceName = \"" << strTemp << "\", " << endl;
		}

		if( pSkillTemplet->m_vGridIndex != D3DXVECTOR2(-1, -1 ) )
		{
			ofs << "\t" << "GRID_INDEX_X = " << (int)pSkillTemplet->m_vGridIndex.x << ", " << endl;
			ofs << "\t" << "GRID_INDEX_Y = " << (int)pSkillTemplet->m_vGridIndex.y << ", " << endl;
		}

		ofs << "}";
		ofs << endl;
		ofs << endl;
		ofs << endl;

	}

	ofs.close();
	

	wstring wstrFilePath = L"";
	ConvertCharToWCHAR( wstrFilePath, strFileName.c_str() );
	SetWindowText( g_pKTDXApp->GetHWND(), wstrFilePath.c_str() );


	// 임시로 저장할 때 마다 스킬트리를 체크하자
	ValidateSkillTree();

	return true;
}


bool CX2SkillTreeTool::ValidateSkillTree()
{
	if( NULL == m_pSkillTree )
		return true;

	bool bResult = true;

	const map<int, CX2SkillTree::SkillTemplet*>& mapSkillTemplet = m_pSkillTree->GetSkillTempletMap();
	map<int, CX2SkillTree::SkillTemplet*>::const_iterator it;


	std::ofstream ofs( "SkillTreeTool_Log.txt" );
	string strName = "";

	for( it = mapSkillTemplet.begin(); it != mapSkillTemplet.end() ; it++ )
	{
		CX2SkillTree::SkillTemplet* pSkillTemplet = it->second;
		if( 1 != pSkillTemplet->m_BeforeSkill.size() )
		{
			ConvertWCHARToChar( strName, pSkillTemplet->m_Name.c_str() );
			ofs << "before skill error: ";
			ofs << pSkillTemplet->m_ID << ", " << strName << std::endl;
			bResult = false;
		}

		D3DXVECTOR2 vGridSize = m_pSkillTree->GetGridSize( pSkillTemplet->m_UnitClass );
		if( pSkillTemplet->m_vGridIndex.x < 0 || pSkillTemplet->m_vGridIndex.x > vGridSize.x-1 ||
			pSkillTemplet->m_vGridIndex.y < 0 || pSkillTemplet->m_vGridIndex.y > vGridSize.y-1 )
		{
			ConvertWCHARToChar( strName, pSkillTemplet->m_Name.c_str() );
			ofs << "grid index error: ";
			ofs << pSkillTemplet->m_ID << ", " << strName << std::endl;
			bResult = false;
		}
	}

	ofs.close();

	return bResult;
}



bool CX2SkillTreeTool::SaveExcel( const WCHAR* wszFilePath )
{
	if( NULL == m_pSkillTree )
		return false;

	if( NULL == wszFilePath )
		return false;

	BasicExcel e;
	e.New( 3 );
	BasicExcelCell* pCell = NULL;


	//////////////////////////////////////////////////////////////////////////
	BasicExcelWorksheet* pWorkSheet2 = e.GetWorksheet( "Sheet2" );
	pWorkSheet2->Rename( "GridSize" );

	pCell = pWorkSheet2->Cell( 0, 0 );
	pCell->SetWString( L"GridSize_UnitClass" );
	pCell = pWorkSheet2->Cell( 0, 1 );
	pCell->SetWString( L"GridSize_Col" );
	pCell = pWorkSheet2->Cell( 0, 2 );
	pCell->SetWString( L"GridSize_Row" );



	// note!! 전직 추가되면 수정

	// ELSWORD
	int iRow = 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_ELSWORD_SWORDMAN" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_SWORDMAN ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_SWORDMAN ).y );


	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_ELSWORD_KNIGHT" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_KNIGHT ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_KNIGHT ).y );

	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_ELSWORD_MAGIC_KNIGHT" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_MAGIC_KNIGHT ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ELSWORD_MAGIC_KNIGHT ).y );


	// AISHA
	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_ARME_VIOLET_MAGE" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_VIOLET_MAGE ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_VIOLET_MAGE ).y );

	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_ARME_HIGH_MAGICIAN" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_HIGH_MAGICIAN ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_HIGH_MAGICIAN ).y );

	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_ARME_DARK_MAGICIAN" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_DARK_MAGICIAN ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_ARME_DARK_MAGICIAN ).y );



	// RENA
	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_LIRE_ELVEN_RANGER" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_ELVEN_RANGER ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_ELVEN_RANGER ).y );


	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_LIRE_COMBAT_RANGER" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_COMBAT_RANGER ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_COMBAT_RANGER ).y );

	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_LIRE_SNIPING_RANGER" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_SNIPING_RANGER ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_LIRE_SNIPING_RANGER ).y );


	// RAVEN
	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_RAVEN_FIGHTER" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_FIGHTER ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_FIGHTER ).y );



	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_RAVEN_SOUL_TAKER" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_SOUL_TAKER ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_SOUL_TAKER ).y );



	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_RAVEN_OVER_TAKER" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_OVER_TAKER ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_RAVEN_OVER_TAKER ).y );






	// Eve
	iRow += 1;
	pCell = pWorkSheet2->Cell( iRow, 0 );
	pCell->SetWString( L"UC_EVE_NASOD" );
	pCell = pWorkSheet2->Cell( iRow, 1 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_EVE_NASOD ).x );
	pCell = pWorkSheet2->Cell( iRow, 2 );
	pCell->SetInteger( (int)m_pSkillTree->GetGridSize( CX2Unit::UC_EVE_NASOD ).y );











	//////////////////////////////////////////////////////////////////////////
	BasicExcelWorksheet* pWorkSheet1 = e.GetWorksheet( "Sheet1" );
	if( NULL == pWorkSheet1 )
		return false;

	pWorkSheet1->Rename( "SkillTable" );


	// column title
	{
		pCell = pWorkSheet1->Cell( 0, 0 );
		pCell->SetWString( L"ID" );
		pCell = pWorkSheet1->Cell( 0, 1 );
		pCell->SetWString( L"ID_Name" );
		pCell = pWorkSheet1->Cell( 0, 2 );
		pCell->SetWString( L"Skill_Type" );
		pCell = pWorkSheet1->Cell( 0, 3 );
		pCell->SetWString( L"Skill_Type_Name" );
		pCell = pWorkSheet1->Cell( 0, 4 );
		pCell->SetWString( L"Skill_Name" );

		pCell = pWorkSheet1->Cell( 0, 5 );
		pCell->SetWString( L"Skill_Desc" );
		pCell = pWorkSheet1->Cell( 0, 6 );
		pCell->SetWString( L"UnitClass" );
		pCell = pWorkSheet1->Cell( 0, 7 );
		pCell->SetWString( L"Show" );
		pCell = pWorkSheet1->Cell( 0, 8 );
		pCell->SetWString( L"m_iSkillLevel" );
		pCell = pWorkSheet1->Cell( 0, 9 );
		pCell->SetWString( L"Need_Level" );

		pCell = pWorkSheet1->Cell( 0, 10 );
		pCell->SetWString( L"Need_SP" );
		pCell = pWorkSheet1->Cell( 0, 11 );
		pCell->SetWString( L"Need_ItemID" );
		pCell = pWorkSheet1->Cell( 0, 12 );
		pCell->SetWString( L"Need_QuestID" );
		pCell = pWorkSheet1->Cell( 0, 13 );
		pCell->SetWString( L"BeforeSkill 1" );
		pCell = pWorkSheet1->Cell( 0, 14 );
		pCell->SetWString( L"BeforeSkill 2" );


		pCell = pWorkSheet1->Cell( 0, 15 );
		pCell->SetWString( L"BeforeSkill 3" );
		pCell = pWorkSheet1->Cell( 0, 16 );
		pCell->SetWString( L"Stat_BaseHP" );
		pCell = pWorkSheet1->Cell( 0, 17 );
		pCell->SetWString( L"Stat_AtkPhysic" );
		pCell = pWorkSheet1->Cell( 0, 18 );
		pCell->SetWString( L"Stat_AtkMagic" );

		pCell = pWorkSheet1->Cell( 0, 19 );
		pCell->SetWString( L"Stat_DefPhysic" );
		pCell = pWorkSheet1->Cell( 0, 20 );
		pCell->SetWString( L"Stat_DefMagic" );
		pCell = pWorkSheet1->Cell( 0, 21 );

		
		pCell = pWorkSheet1->Cell( 0, 22 );
		pCell->SetWString( L"Texture_Name" );
		pCell = pWorkSheet1->Cell( 0, 23 );
		pCell->SetWString( L"Texture_Piece_Name" );
		pCell = pWorkSheet1->Cell( 0, 24 );
		pCell->SetWString( L"Grid_Index_X" );
		pCell = pWorkSheet1->Cell( 0, 25 );
		pCell->SetWString( L"Grid_Index_Y" );

	}


	const map<int, CX2SkillTree::SkillTemplet*>& mapSkillTemplet = m_pSkillTree->GetSkillTempletMap();
	map<int, CX2SkillTree::SkillTemplet*>::const_iterator it;


	WCHAR wszText[512] = L"";
	iRow = 1;
	for( it = mapSkillTemplet.begin(); it != mapSkillTemplet.end(); it++ )
	{
		CX2SkillTree::SkillTemplet* pSkillTemplet = it->second;
		if( NULL == pSkillTemplet )
			continue;
		
		pCell = pWorkSheet1->Cell( iRow, 0 );
		pCell->SetInteger( (int) pSkillTemplet->m_ID );

		pCell = pWorkSheet1->Cell( iRow, 1 );
		wsprintf( wszText, L"=VLOOKUP(A%d,'SkillEnum'!A$2:B$517,2,FALSE)", iRow+1 );
		pCell->SetWString( wszText );

		pCell = pWorkSheet1->Cell( iRow, 2 );
		pCell->SetInteger( (int) pSkillTemplet->m_Type );
		

		pCell = pWorkSheet1->Cell( iRow, 3 );
		wsprintf( wszText, L"=VLOOKUP(c%d,'SkillEnum'!D$2:E$9,2,FALSE)", iRow+1 );
		pCell->SetWString( wszText );

		pCell = pWorkSheet1->Cell( iRow, 4 );
		pCell->SetWString( pSkillTemplet->m_Name.c_str() );

		pCell = pWorkSheet1->Cell( iRow, 5 );
		wstring wstrDesc = pSkillTemplet->m_Desc;
		ReplaceString( wstrDesc, L"\n", L"\\n" );
		ReplaceString( wstrDesc, L"\t", L"\\t" );
		pCell->SetWString( wstrDesc.c_str() );
		
		// note!! 전직 추가되면 수정
		pCell = pWorkSheet1->Cell( iRow, 6 );
		switch( pSkillTemplet->m_UnitClass )
		{
		case CX2Unit::UC_ELSWORD_SWORDMAN:
			{
				pCell->SetWString( L"ELSWORD_SWORDMAN" );
			} break;
		case CX2Unit::UC_ELSWORD_KNIGHT:
			{
				pCell->SetWString( L"ELSWORD_KNIGHT" );
			} break;

		case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
			{
				pCell->SetWString( L"ELSWORD_MAGIC_KNIGHT" );
			} break;


		case CX2Unit::UC_ARME_VIOLET_MAGE:
			{
				pCell->SetWString( L"ARME_VIOLET_MAGE" );
			} break;

		case CX2Unit::UC_ARME_HIGH_MAGICIAN:
			{
				pCell->SetWString( L"ARME_HIGH_MAGICIAN" );
			} break;

		case CX2Unit::UC_ARME_DARK_MAGICIAN:
			{
				pCell->SetWString( L"ARME_DARK_MAGICIAN" );
			} break;


		case CX2Unit::UC_LIRE_ELVEN_RANGER:
			{
				pCell->SetWString( L"LIRE_ELVEN_RANGER" );
			} break;
		case CX2Unit::UC_LIRE_COMBAT_RANGER:
			{
				pCell->SetWString( L"LIRE_COMBAT_RANGER" );
			} break;

		case CX2Unit::UC_LIRE_SNIPING_RANGER:
			{
				pCell->SetWString( L"LIRE_SNIPING_RANGER" );
			} break;


		case CX2Unit::UC_RAVEN_FIGHTER:
			{
				pCell->SetWString( L"RAVEN_FIGHTER" );
			} break;

		case CX2Unit::UC_RAVEN_SOUL_TAKER:
			{
				pCell->SetWString( L"RAVEN_SOUL_TAKER" );
			} break;

		case CX2Unit::UC_RAVEN_OVER_TAKER:
			{
				pCell->SetWString( L"RAVEN_OVER_TAKER" );
			} break;



		case CX2Unit::UC_EVE_NASOD:
			{
				pCell->SetWString( L"EVE_NASOD" );
			} break;



		}

		pCell = pWorkSheet1->Cell( iRow, 7 );
		if( true == pSkillTemplet->m_bShow )
		{
			pCell->SetWString( L"TRUE" );
		}
		else
		{
			pCell->SetWString( L"FALSE" );
		}

		pCell = pWorkSheet1->Cell( iRow, 8 );
		pCell->SetInteger( pSkillTemplet->m_iSkillLevel );
		
		pCell = pWorkSheet1->Cell( iRow, 9 );
		pCell->SetInteger( pSkillTemplet->m_NeedLevel );

		pCell = pWorkSheet1->Cell( iRow, 10 );
		pCell->SetInteger( pSkillTemplet->m_NeedSP );
		
		pCell = pWorkSheet1->Cell( iRow, 11 );
		pCell->SetInteger( pSkillTemplet->m_NeedItemID );

		pCell = pWorkSheet1->Cell( iRow, 12 );
		pCell->SetInteger( pSkillTemplet->m_NeedQuestID );

		pCell = pWorkSheet1->Cell( iRow, 13 );
		if( pSkillTemplet->m_BeforeSkill.size() >0 )
		{
			pCell->SetInteger( pSkillTemplet->m_BeforeSkill[0] );
		}
		else
		{
			pCell->SetInteger( 0 );
		}

		pCell = pWorkSheet1->Cell( iRow, 14 );
		pCell->SetInteger( 0 );

		pCell = pWorkSheet1->Cell( iRow, 15 );
		pCell->SetInteger( 0 );

		pCell = pWorkSheet1->Cell( iRow, 16 );
		pCell->SetDouble( (double) pSkillTemplet->m_Stat.m_fBaseHP );

		pCell = pWorkSheet1->Cell( iRow, 17 );
		pCell->SetDouble( (double) pSkillTemplet->m_Stat.m_fAtkPhysic);
	
		pCell = pWorkSheet1->Cell( iRow, 18 );
		pCell->SetDouble( (double) pSkillTemplet->m_Stat.m_fAtkMagic );

		pCell = pWorkSheet1->Cell( iRow, 19 );
		pCell->SetDouble( (double) pSkillTemplet->m_Stat.m_fDefPhysic );

		pCell = pWorkSheet1->Cell( iRow, 20 );
		pCell->SetDouble( (double) pSkillTemplet->m_Stat.m_fDefMagic );


		pCell = pWorkSheet1->Cell( iRow, 21 );
		pCell->SetWString( pSkillTemplet->m_wstrTextureName.c_str() );
		pCell = pWorkSheet1->Cell( iRow, 22 );
		pCell->SetWString( pSkillTemplet->m_wstrTexturePieceName.c_str() );
		pCell = pWorkSheet1->Cell( iRow, 23 );
		pCell->SetInteger( (int) pSkillTemplet->m_vGridIndex.x );
		pCell = pWorkSheet1->Cell( iRow, 24 );
		pCell->SetInteger( (int) pSkillTemplet->m_vGridIndex.y );

		
		iRow++;
	}



	// 파일 확장자가 없으면 붙여주자.
	std::string strFileName = "";
	ConvertWCHARToChar( strFileName, wszFilePath );	
	MakeUpperCase( strFileName );

	int iStrLen = strFileName.length();
	if( iStrLen < 5 )
		return false;

	if( '.' != strFileName[iStrLen-4] || 'X' != strFileName[iStrLen-3] || 'L' != strFileName[iStrLen-2] || 'S' != strFileName[iStrLen-1] ) 
	{
		ConvertWCHARToChar( strFileName, wszFilePath );	
		strFileName.append(".xls");
	}
	else
	{
		ConvertWCHARToChar( strFileName, wszFilePath );	
	}

	e.SaveAs( strFileName.c_str() );

	return true;
}

void CX2SkillTreeTool::InitDialog()
{
	CDXUTEditBox* pEditBox = NULL;
	CDXUTComboBox *pCombo = NULL;


	// initialize UI
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	m_MainDialog.Init( &g_DialogResourceManager );
	m_MainDialog.SetCallback( OnMainDialogEvent_SkillTreeTool, this );
	m_MainDialog.SetSize( 300, 300 );
	m_MainDialog.SetFont(0, L"굴림", 12, 12);
	m_MainDialog.SetLocation( 210, 0 );

	int iX = 2;
	int iY = 20;
	int iDX = 100;
	int iDY = 35;
	int iWidth = 200;
	int iHeight = 32;

	
	m_MainDialog.AddButton( IDC_BUTTON_HELP, L"원더걸스 쵝오!", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_MainDialog.AddButton( IDC_BUTTON_OPEN_SKILL_TEMPLET, L"open skilltemplet.lua", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_MainDialog.AddButton( IDC_BUTTON_SAVE_SKILL_TEMPLET, L"save skilltemplet.lua", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_MainDialog.AddButton( IDC_BUTTON_SAVE_SKILL_EXCEL, L"save excel", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;

	m_MainDialog.AddStatic( IDC_STATIC_DUMMY, L"grid size:", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_MainDialog.AddEditBox( IDC_EDIT_GRID_SIZE, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	int iValue[2] = { (int)m_vGridSize.x, (int)m_vGridSize.y };
	SetTextIntArrayToEditBox( pEditBox, &iValue[0], 2 );
	iY += iHeight + 4;
	
	// note!! 전직 추가되면 수정
	m_MainDialog.AddComboBox( IDC_COMBO_UNIT_CLASS, iX, iY, iWidth, iHeight, 0, false, &pCombo );
	iY += iHeight + 4;
	{
		pCombo->SetDropHeight( 120 );
		pCombo->AddItem( L"UC_NONE",					(LPVOID) CX2Unit::UC_NONE );
		pCombo->AddItem( L"UC_ELSWORD_SWORDMAN",		(LPVOID) CX2Unit::UC_ELSWORD_SWORDMAN  );
		pCombo->AddItem( L"UC_ELSWORD_KNIGHT",			(LPVOID) CX2Unit::UC_ELSWORD_KNIGHT  );
		pCombo->AddItem( L"UC_ELSWORD_MAGIC_KNIGHT",	(LPVOID) CX2Unit::UC_ELSWORD_MAGIC_KNIGHT  );

		pCombo->AddItem( L"UC_ARME_VIOLET_MAGE",		(LPVOID) CX2Unit::UC_ARME_VIOLET_MAGE  );
		pCombo->AddItem( L"UC_ARME_HIGH_MAGICIAN",		(LPVOID) CX2Unit::UC_ARME_HIGH_MAGICIAN  );
		pCombo->AddItem( L"UC_ARME_DARK_MAGICIAN",		(LPVOID) CX2Unit::UC_ARME_DARK_MAGICIAN  );
		
		pCombo->AddItem( L"UC_LIRE_ELVEN_RANGER",		(LPVOID) CX2Unit::UC_LIRE_ELVEN_RANGER  );
		pCombo->AddItem( L"UC_LIRE_COMBAT_RANGER",		(LPVOID) CX2Unit::UC_LIRE_COMBAT_RANGER  );
		pCombo->AddItem( L"UC_LIRE_SNIPING_RANGER",		(LPVOID) CX2Unit::UC_LIRE_SNIPING_RANGER  );

		pCombo->AddItem( L"UC_RAVEN_FIGHTER",			(LPVOID) CX2Unit::UC_RAVEN_FIGHTER  );
		pCombo->AddItem( L"UC_RAVEN_SOUL_TAKER",		(LPVOID) CX2Unit::UC_RAVEN_SOUL_TAKER  );
		pCombo->AddItem( L"UC_RAVEN_OVER_TAKER",		(LPVOID) CX2Unit::UC_RAVEN_OVER_TAKER  );


		pCombo->AddItem( L"UC_EVE_NASOD",				(LPVOID) CX2Unit::UC_EVE_NASOD );
		pCombo->AddItem( L"UC_EVE_EXOTIC_GEAR",			(LPVOID) CX2Unit::UC_EVE_EXOTIC_GEAR );
		pCombo->AddItem( L"UC_EVE_ARCHITECTURE",		(LPVOID) CX2Unit::UC_EVE_ARCHITECTURE );

	
	}	


	m_MainDialog.AddStatic( IDC_STATIC_DUMMY, L"move all grid:", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_MainDialog.AddEditBox( IDC_EDIT_MOVE_ALL_GRID, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	iY += iHeight + 4;


	m_MainDialog.AddStatic( IDC_STATIC_DUMMY, L"available skill:", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_MainDialog.AddListBox( IDC_LIST_AVAILABLE_SKILL, iX, iY, iWidth, iHeight*6, 0 );
	iY += iHeight*6 + 4;



	//////////////////////////////////////////////////////////////////////////
	m_PropertyDialog.Init( &g_DialogResourceManager );
	m_PropertyDialog.SetCallback( OnPropertyDialogEvent_SkillTreeTool, this );
	m_PropertyDialog.SetSize( 300, 300 );
	m_PropertyDialog.SetFont(0, L"굴림", 12, 12);
	m_PropertyDialog.SetLocation( 0, 0 );

	iX = 2;
	iY = 20;

	m_PropertyDialog.AddStatic( IDC_STATIC_DUMMY, L"grid index:", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_PropertyDialog.AddEditBox( IDC_EDIT_GRID_INDEX, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	pEditBox->SetEnabled( false );
	iY += iHeight + 4;


	m_PropertyDialog.AddStatic( IDC_STATIC_DUMMY, L"skill ID:", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_PropertyDialog.AddEditBox( IDC_EDIT_SKILL_ID, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	pEditBox->SetEnabled( false );
	iY += iHeight + 4;
	
	//m_PropertyDialog.AddStatic( IDC_STATIC_DUMMY, L"skill name:", iX, iY, iWidth, iHeight );
	//iY += iHeight + 4;
	m_PropertyDialog.AddEditBox( IDC_EDIT_SKILL_NAME, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	pEditBox->SetEnabled( false );
	iY += iHeight + 4;

	m_PropertyDialog.AddStatic( IDC_STATIC_DUMMY, L"texture file name:", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_PropertyDialog.AddEditBox( IDC_EDIT_TEXTURE_NAME, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	iY += iHeight + 4;
	m_PropertyDialog.AddButton( IDC_BUTTON_OPEN_TEXTURE, L"open texture file", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;


	m_PropertyDialog.AddStatic( IDC_STATIC_DUMMY, L"texture piece name:", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_PropertyDialog.AddEditBox( IDC_EDIT_TEXTURE_PIECE_NAME, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	iY += iHeight + 4;
	m_PropertyDialog.AddListBox( IDC_LIST_TEXTURE_PIECE_NAME, iX, iY, iWidth, iHeight*4, 0 );
	iY += iHeight*4 + 4;


	m_PropertyDialog.AddStatic( IDC_STATIC_DUMMY, L"necessary skill:", iX, iY, iWidth, iHeight );
	iY += iHeight + 4;
	m_PropertyDialog.AddListBox( IDC_LIST_NECESSARY_SKILL_ID, iX, iY, iWidth, iHeight*2, 0 );
	iY += iHeight*2 + 4;
}



void CX2SkillTreeTool::RegisterLuabind()
{
	lua_State* L= g_pKTDXApp->GetLuaBinder()->GetLuaState();

	lua_tinker::class_add<CX2SkillTree>( L, "CX2SkillTree" );
	lua_tinker::class_def<CX2SkillTree>( L, "AddSkillTemplet_LUA",			CX2SkillTree::AddSkillTemplet_LUA );
	lua_tinker::class_def<CX2SkillTree>( L, "SetGridSize",					CX2SkillTree::SetGridSize );
	lua_tinker::class_def<CX2SkillTree>( L, "SetCanNotResetSkill",			CX2SkillTree::SetCanNotResetSkill);
}


bool CX2SkillTreeTool::OpenScriptFile( const WCHAR* pFileName )
{
	//lua_tinker::decl( "g_pMain", this );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		return false;
	}

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		return false;
	}

	return true;
}





// 선택된 grid의 before skill을 보여준다
void CX2SkillTreeTool::UpdatePickedGridBeforeSkillMark()
{
	if( NULL == m_pSkillTree )
		return;


	if( m_vPickedGridIndex == D3DXVECTOR2(-1, -1 ) )
		return;

	map< std::pair<int,int>, SKILL_GRID >::iterator it;
	it = m_mapSkillGrid.find( make_pair( (int)m_vPickedGridIndex.x, (int)m_vPickedGridIndex.y ) );
	if( it == m_mapSkillGrid.end() )
		return;
	
	
	CX2SkillTree::SkillTemplet* pSkillTemplet = m_pSkillTree->GetSkillTemplet( it->second.m_eSkillID );
	if( NULL == pSkillTemplet )
		return;



	for( it= m_mapSkillGrid.begin(); it != m_mapSkillGrid.end(); it++ )
	{
		SKILL_GRID& skillGrid = it->second;
		if( NULL != skillGrid.m_pBeforeSkillMarkPicture )
		{
			skillGrid.m_pBeforeSkillMarkPicture->SetShow( false );
		}
	}


	for( UINT i=0; i<pSkillTemplet->m_BeforeSkill.size(); i++ )
	{

		CX2SkillTree::SkillTemplet* pBeforeSkillTemplet = m_pSkillTree->GetSkillTemplet( (CX2SkillTree::SKILL_ID) pSkillTemplet->m_BeforeSkill[i] );
		if( NULL == pBeforeSkillTemplet )
			continue;

		it = m_mapSkillGrid.find( make_pair( (int)pBeforeSkillTemplet->m_vGridIndex.x, (int)pBeforeSkillTemplet->m_vGridIndex.y ) );
		if( it == m_mapSkillGrid.end() )
			continue;

		SKILL_GRID& skillGrid = it->second;
		if( NULL != skillGrid.m_pBeforeSkillMarkPicture )
		{
			skillGrid.m_pBeforeSkillMarkPicture->SetShow( true );
		}
	}

}


void CX2SkillTreeTool::UpdateGridUI( int iPickedX, int iPickedY )
{
	if( NULL == m_pDLGSkillTree )
		return;


	if( m_pSkillTree == NULL )
		return;

	SKILL_GRID& skillGrid = m_mapSkillGrid[ make_pair( iPickedX, iPickedY )  ];
	CX2SkillTree::SkillTemplet* pSkillTemplet = m_pSkillTree->GetSkillTemplet( skillGrid.m_eSkillID );



	CKTDGUIStatic* pStatic_Skill_Icon = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Skill_Icon" );
	if( NULL != pStatic_Skill_Icon )
	{
		CKTDGUIControl::CPictureData* pPicture = pStatic_Skill_Icon->GetPicture( (int) ( iPickedY * m_vGridSize.x + iPickedX ) );
		pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"FRONT_ROAD" );

		//if( NULL != pSkillTemplet && pSkillTemplet->m_UnitClass == m_eUnitClass )
		if( NULL != pSkillTemplet && m_eUnitClass == pSkillTemplet->m_UnitClass )
		{
			if( pSkillTemplet->m_wstrTextureName.length() > 0 && pSkillTemplet->m_wstrTexturePieceName.length() > 0 )
			{
				if( false == pPicture->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
				{
					pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"BOSS_MARK" );
				}
			}
			else
			{
				pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"BOSS_MARK" );
			}
		}
	}




	CKTDGUIControl::CPictureData* pPicture_Up		= GetPictureArrow( iPickedX, iPickedY, DIR_UP );
	CKTDGUIControl::CPictureData* pPicture_Down 	= GetPictureArrow( iPickedX, iPickedY, DIR_DOWN );
	CKTDGUIControl::CPictureData* pPicture_Left 	= GetPictureArrow( iPickedX, iPickedY, DIR_LEFT );
	CKTDGUIControl::CPictureData* pPicture_Right	= GetPictureArrow( iPickedX, iPickedY, DIR_RIGHT );

	if( NULL != pPicture_Up )
	{
		pPicture_Up->SetShow( false );
	}

	if( NULL != pPicture_Down )
	{
		pPicture_Down->SetShow( false );
	}
	if( NULL != pPicture_Left )
	{
		pPicture_Left->SetShow( false );
	}

	if( NULL != pPicture_Right )
	{
		pPicture_Right->SetShow( false );
	}


	if( pSkillTemplet != NULL )
	{

		CX2SkillTree::SkillTemplet* pSkillTemplet_Up	= GetGridSkillTemplet( iPickedX, iPickedY-1 );
		CX2SkillTree::SkillTemplet* pSkillTemplet_Down	= GetGridSkillTemplet( iPickedX, iPickedY+1 );
		CX2SkillTree::SkillTemplet* pSkillTemplet_Left	= GetGridSkillTemplet( iPickedX-1, iPickedY );
		CX2SkillTree::SkillTemplet* pSkillTemplet_Right = GetGridSkillTemplet( iPickedX+1, iPickedY );

		
		if( NULL != pSkillTemplet_Up )
		{
			for( UINT j=0; j<pSkillTemplet_Up->m_BeforeSkill.size(); j++ )
			{
				if( pSkillTemplet_Up->m_BeforeSkill[j] == pSkillTemplet->m_ID )
				{
					pPicture_Up->SetShow( true );
					pPicture_Up->SetTex( L"DLG_Keys.tga", L"KEY_UP" );
					break;
				}
			}
		}

		if( NULL != pSkillTemplet_Down )
		{
			for( UINT j=0; j<pSkillTemplet_Down->m_BeforeSkill.size(); j++ )
			{
				if( pSkillTemplet_Down->m_BeforeSkill[j] == pSkillTemplet->m_ID )
				{
					pPicture_Down->SetShow( true );
					pPicture_Down->SetTex( L"DLG_Keys.tga", L"KEY_DOWN" );
					break;
				}
			}
		}

		if( NULL != pSkillTemplet_Left )
		{
			for( UINT j=0; j<pSkillTemplet_Left->m_BeforeSkill.size(); j++ )
			{
				if( pSkillTemplet_Left->m_BeforeSkill[j] == pSkillTemplet->m_ID )
				{
					pPicture_Left->SetShow( true );
					pPicture_Left->SetTex( L"DLG_Keys.tga", L"KEY_LEFT" );
					break;
				}
			}
		}

		if( NULL != pSkillTemplet_Right )
		{
			for( UINT j=0; j<pSkillTemplet_Right->m_BeforeSkill.size(); j++ )
			{
				if( pSkillTemplet_Right->m_BeforeSkill[j] == pSkillTemplet->m_ID )
				{
					pPicture_Right->SetShow( true );
					pPicture_Right->SetTex( L"DLG_Keys.tga", L"KEY_RIGHT" );
					break;
				}
			}

		}


		for( UINT i=0; i< pSkillTemplet->m_BeforeSkill.size(); i++ )
		{
			CKTDGUIControl::CPictureData* pPicture_Up		= GetPictureArrow( iPickedX, iPickedY, DIR_UP );
			CKTDGUIControl::CPictureData* pPicture_Down 	= GetPictureArrow( iPickedX, iPickedY, DIR_DOWN );
			CKTDGUIControl::CPictureData* pPicture_Left 	= GetPictureArrow( iPickedX, iPickedY, DIR_LEFT );
			CKTDGUIControl::CPictureData* pPicture_Right	= GetPictureArrow( iPickedX, iPickedY, DIR_RIGHT );

			if( NULL != pPicture_Up )
			{
				if( pSkillTemplet_Up != NULL )
				{
					if( pSkillTemplet->m_BeforeSkill[i] == (int)pSkillTemplet_Up->m_ID )
					{
						pPicture_Up->SetShow( true );
						pPicture_Up->SetTex( L"DLG_Keys.tga", L"KEY_DOWN" );
					}
				}
			}
			
			if( NULL != pPicture_Down )
			{
				if( pSkillTemplet_Down != NULL )
				{
					if( pSkillTemplet->m_BeforeSkill[i] == (int)pSkillTemplet_Down->m_ID )
					{
						pPicture_Down->SetShow( true );
						pPicture_Down->SetTex( L"DLG_Keys.tga", L"KEY_UP" );
					}
				}
			}
			if( NULL != pPicture_Left )
			{
				if( pSkillTemplet_Left != NULL )
				{
					if( pSkillTemplet->m_BeforeSkill[i] == (int)pSkillTemplet_Left->m_ID )
					{
						pPicture_Left->SetShow( true );
						pPicture_Left->SetTex( L"DLG_Keys.tga", L"KEY_RIGHT" );
					}
				}
			}

			if( NULL != pPicture_Right )
			{
				if( pSkillTemplet_Right != NULL )
				{
					if( pSkillTemplet->m_BeforeSkill[i] == (int)pSkillTemplet_Right->m_ID )
					{
						pPicture_Right->SetShow( true );
						pPicture_Right->SetTex( L"DLG_Keys.tga", L"KEY_LEFT" );
					}
				}
			}
		}
	}
}



void CX2SkillTreeTool::UpdateAllGridUI()
{
	for( int iRow=0; iRow<m_vGridSize.y; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x; iCol++ )
		{
			UpdateGridUI( iCol, iRow );
		}
	}
}




void CX2SkillTreeTool::UpdatePropertyDialog()
{
	
	CDXUTEditBox* pEdit_GridIndex = (CDXUTEditBox*) m_PropertyDialog.GetEditBox( IDC_EDIT_GRID_INDEX );
	CDXUTEditBox* pEdit_ID = (CDXUTEditBox*) m_PropertyDialog.GetEditBox( IDC_EDIT_SKILL_ID );
	CDXUTEditBox* pEdit_Name = (CDXUTEditBox*) m_PropertyDialog.GetEditBox( IDC_EDIT_SKILL_NAME );
	CDXUTEditBox* pEdit_TexName = (CDXUTEditBox*) m_PropertyDialog.GetEditBox( IDC_EDIT_TEXTURE_NAME );
	CDXUTEditBox* pEdit_PieceName = (CDXUTEditBox*) m_PropertyDialog.GetEditBox( IDC_EDIT_TEXTURE_PIECE_NAME );
	CDXUTListBox* pList_Necessary_Skill = (CDXUTListBox*) m_PropertyDialog.GetListBox( IDC_LIST_NECESSARY_SKILL_ID );


	WCHAR wszText[1024] = L"";
	if( NULL != pEdit_GridIndex )
	{
		wsprintf( wszText, L"%d  %d", (int)m_vPickedGridIndex.x, (int)m_vPickedGridIndex.y );
		pEdit_GridIndex->SetText( wszText );
	}

	if( NULL != pEdit_ID )
		pEdit_ID->SetText( L"" );

	if( NULL != pEdit_Name )
		pEdit_Name->SetText( L"" );

	if( NULL != pEdit_TexName )
		pEdit_TexName->SetText( L"" );

	if( NULL != pEdit_PieceName )
		pEdit_PieceName->SetText( L"" );

	if( NULL != pList_Necessary_Skill )
		pList_Necessary_Skill->RemoveAllItems();




	if( NULL == m_pSkillTree )
		return;

	SKILL_GRID& skillGrid = m_mapSkillGrid[ make_pair( (int)m_vPickedGridIndex.x, (int) m_vPickedGridIndex.y )  ];
	CX2SkillTree::SkillTemplet* pSkillTemplet = m_pSkillTree->GetSkillTemplet( skillGrid.m_eSkillID );
	if( NULL == pSkillTemplet )
		return;

	if( NULL != pEdit_ID )
	{
		wsprintf( wszText, L"%d", (int)pSkillTemplet->m_ID );
		pEdit_ID->SetText( wszText );
	}

	if( NULL != pEdit_Name )
	{
		pEdit_Name->SetText( pSkillTemplet->m_Name.c_str() );
	}

	if( NULL != pEdit_TexName )
	{
		pEdit_TexName->SetText( pSkillTemplet->m_wstrTextureName.c_str() );
	}

	if( NULL != pEdit_PieceName )
	{
		pEdit_PieceName->SetText( pSkillTemplet->m_wstrTexturePieceName.c_str() );
	}


	if( NULL != pList_Necessary_Skill )
	{
		for( UINT i=0; i< pSkillTemplet->m_BeforeSkill.size(); i++ )
		{
			CX2SkillTree::SkillTemplet* pBefoerSkillTemplet = m_pSkillTree->GetSkillTemplet( (CX2SkillTree::SKILL_ID) pSkillTemplet->m_BeforeSkill[i] );
			if( NULL != pBefoerSkillTemplet )
			{
				wsprintf( wszText, L"%-6d: %s", pBefoerSkillTemplet->m_ID, pBefoerSkillTemplet->m_Name.c_str() );
				pList_Necessary_Skill->AddItem( wszText, (void*) NULL );
			}
		}
	}

}


void CX2SkillTreeTool::UpdateMainDialog()
{
	CDXUTEditBox* pEdit_GridSize = (CDXUTEditBox*) m_MainDialog.GetEditBox( IDC_EDIT_GRID_SIZE );
	if( NULL != pEdit_GridSize )
	{
		int iValue[2] = { (int)m_vGridSize.x, (int)m_vGridSize.y };
		SetTextIntArrayToEditBox( pEdit_GridSize, &iValue[0], 2 );
	}
	
}

bool CX2SkillTreeTool::OnDropFile( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	WCHAR wszFilePath[MAX_PATH] = L"";	
	WCHAR wszPath[MAX_PATH] = L"";	
	WCHAR wszFileName[MAX_PATH] = L"";	
	DragQueryFile( (HDROP)wParam, 0, wszFilePath, MAX_PATH );

	OpenSkillTemplet( wszFilePath );

	return true;
}


bool CX2SkillTreeTool::DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath )
{
	if( NULL == wszFullPath )
		return false;

	WCHAR drive[_MAX_DRIVE] = L"";
	WCHAR dir[_MAX_DIR] = L"";
	WCHAR fname[_MAX_FNAME] = L"";
	WCHAR ext[_MAX_EXT] = L"";

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




void CX2SkillTreeTool::OpenSkillListWindow( bool bOpen )
{
	if( true == bOpen )
	{
		if( NULL == m_pDLGSkillList )
		{
			m_pDLGSkillList = new CKTDGUIDialog( (CKTDXStage*) this, L"DLG_TOOL_Skill_List_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillList );					
		}
		
		m_pDLGSkillList->SetShowEnable( true, true );

		D3DXVECTOR2 vMousePos;
		vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
		vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
		vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );
		m_pDLGSkillList->SetPos( vMousePos - D3DXVECTOR2(0.f, 0.f) );


		UpdateSkillListWindow();
	}
	else
	{
		if( NULL != m_pDLGSkillList )
		{
			m_pDLGSkillList->SetShowEnable( false, false );
		}
	}
}


void CX2SkillTreeTool::UpdateSkillListWindow( bool bCheckBeforeSkill /*= false*/ )
{
	if( NULL == m_pDLGSkillList )
		return;

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGSkillList->GetControl( L"ListBox_Monster_ID" );
	if( NULL == pListBox )
		return;

	if( NULL == m_pSkillTree )
		return;

	pListBox->RemoveAllItems();


	int iGridX = (int) m_vPickedGridIndex.x;
	int iGridY = (int) m_vPickedGridIndex.y;
	
	
	const map<int, CX2SkillTree::SkillTemplet*>& mapSkillTemplet = m_pSkillTree->GetSkillTempletMap();
	map<int, CX2SkillTree::SkillTemplet*>::const_iterator it;
	WCHAR wszText[1024] = L"";


	for( it = mapSkillTemplet.begin(); it != mapSkillTemplet.end(); it++ )
	{
		CX2SkillTree::SkillTemplet* pSkillTemplet = it->second;
	
		if( pSkillTemplet->m_UnitClass != CX2Unit::UC_NONE && m_eUnitClass != pSkillTemplet->m_UnitClass )
			continue;
		
		if( true == bCheckBeforeSkill )
		{
			if( pSkillTemplet->m_BeforeSkill.size() > 0 )
			{
				bool bOK = false;

				CX2SkillTree::SkillTemplet* pSkillTemplet_Up	= GetGridSkillTemplet( iGridX, iGridY-1 );
				CX2SkillTree::SkillTemplet* pSkillTemplet_Down	= GetGridSkillTemplet( iGridX, iGridY+1 );
				CX2SkillTree::SkillTemplet* pSkillTemplet_Left	= GetGridSkillTemplet( iGridX-1, iGridY );
				CX2SkillTree::SkillTemplet* pSkillTemplet_Right = GetGridSkillTemplet( iGridX+1, iGridY );

				for( UINT i=0; i< pSkillTemplet->m_BeforeSkill.size(); i++ )
				{
					if( pSkillTemplet_Up != NULL && pSkillTemplet->m_BeforeSkill[i] == (int)pSkillTemplet_Up->m_ID )
					{
						bOK = true;
						break;
					}

					if( pSkillTemplet_Down != NULL && pSkillTemplet->m_BeforeSkill[i] == (int)pSkillTemplet_Down->m_ID )
					{
						bOK = true;
						break;
					}
					if( pSkillTemplet_Left != NULL && pSkillTemplet->m_BeforeSkill[i] == (int)pSkillTemplet_Left->m_ID )
					{
						bOK = true;
						break;
					}
					if( pSkillTemplet_Right != NULL && pSkillTemplet->m_BeforeSkill[i] == (int)pSkillTemplet_Right->m_ID )
					{
						bOK = true;
						break;
					}
				}

				if( bOK == false )
					continue;
			}
		}


		if( pSkillTemplet->m_vGridIndex != D3DXVECTOR2(-1, -1) )
		{
			wsprintf( wszText, L"#CFF0000%-6d: %s #CX#CAA0000(%d,%d)", pSkillTemplet->m_ID, pSkillTemplet->m_Name.c_str(),
				(int)pSkillTemplet->m_vGridIndex.x, (int)pSkillTemplet->m_vGridIndex.y );
			pListBox->AddItem( wszText, (void*) pSkillTemplet->m_ID );
		}
		else
		{
			wsprintf( wszText, L"%-6d: %s", pSkillTemplet->m_ID, pSkillTemplet->m_Name.c_str() );
			pListBox->AddItem( wszText, (void*) pSkillTemplet->m_ID );
		}
	}

	pListBox->AddItem( L"삭제", (void*) -1 );
}

CX2SkillTreeTool::SKILL_GRID* CX2SkillTreeTool::GetSkillGrid( int iX, int iY )
{
	map< std::pair<int,int>, SKILL_GRID >::iterator it;
	it = m_mapSkillGrid.find( make_pair( iX, iY) );
	if( it != m_mapSkillGrid.end() )
	{
		return &it->second;
	}
	else
	{
		return NULL;
	}
}

CX2SkillTree::SkillTemplet* CX2SkillTreeTool::GetGridSkillTemplet( int iX, int iY )
{
	if( NULL == m_pSkillTree )
		return NULL;


	map< std::pair<int,int>, SKILL_GRID >::iterator it;
	it = m_mapSkillGrid.find( make_pair( iX, iY) );
	if( it != m_mapSkillGrid.end() )
	{
		return m_pSkillTree->GetSkillTemplet( it->second.m_eSkillID );
	}
	else
	{
		return NULL;
	}
}


CX2SkillTree::SkillTemplet* CX2SkillTreeTool::GetPickedSkillTemplet()
{
	if( m_pSkillTree == NULL )
		return NULL;

	if( m_vPickedGridIndex == D3DXVECTOR2(-1,-1) )
	{
		return NULL;
	}

	map< pair<int, int>, SKILL_GRID >::iterator it = m_mapSkillGrid.find( make_pair( (int)m_vPickedGridIndex.x, (int)m_vPickedGridIndex.y ) );
	if( m_mapSkillGrid.end() == it )
	{
		return NULL;
	}

	SKILL_GRID& skillGrid = it->second;
	return m_pSkillTree->GetSkillTemplet( skillGrid.m_eSkillID );
}



void CX2SkillTreeTool::ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount )
{
#define IN_INT_CHARSET( c ) \
	( (c) == L'-' || ( (c) >= L'0' && (c) <= L'9' ) )


	const WCHAR *pToken, *pEnd;
	WCHAR *pStop = NULL;

	pToken = pEditBox->GetText();
	pEnd = pToken + pEditBox->GetTextLength();

	for( int i=0; i<nCount; i++ )
	{
		*pNumbers = (int) wcstol( pToken, &pStop, 10 );
		pNumbers++;
		pToken = pStop;

		while( !IN_INT_CHARSET( *pToken ) )
		{
			if( pToken == L'\0' || pToken == pEnd )
				break;

			++pToken;
		}

		if( pToken == L'\0' || pStop == L'\0' || pToken == pEnd )
			break;
	}
}


void CX2SkillTreeTool::SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount )
{
	WCHAR wszBuffer[512] = {0};
	WCHAR wszTmp[64];

	if( pNumbers == NULL )
		return;

	for( int i = 0; i < nCount; ++i )
	{
		StringCchPrintf( wszTmp, 64, L"%d ", pNumbers[i] );
		StringCchCat( wszBuffer, 512, wszTmp );
	}

	// Don't want the last space
	if( nCount > 0 && wcslen(wszBuffer) > 0 )
		wszBuffer[wcslen(wszBuffer)-1] = 0;

	pEditBox->SetText( wszBuffer );


}

void CX2SkillTreeTool::ResizeGrid( int x, int y )
{
	map< std::pair<int,int>, SKILL_GRID >	lastSkillGrid = m_mapSkillGrid;

	CreateSkillTreeDialog( D3DXVECTOR2( (float)x, (float)y ) );


	// 이전 위치를 새 grid에 복사
	map< std::pair<int,int>, SKILL_GRID >::iterator it;
	map< std::pair<int,int>, SKILL_GRID >::iterator it2;
	for( it = lastSkillGrid.begin(); it != lastSkillGrid.end() ; it++ )
	{
		it2 = m_mapSkillGrid.find( it->first );
		if( it2 != m_mapSkillGrid.end() )
		{
			it2->second.m_eSkillID = it->second.m_eSkillID;
		}
	}


	UpdateAllGridUI();
}





void CX2SkillTreeTool::MoveAllSkillGrid( D3DXVECTOR2 vDisplace )
{
	if( m_pSkillTree == NULL )
		return;

	map< std::pair<int,int>, SKILL_GRID >::iterator it;

	// 경계를 안 벗어나고 옮길 수 있는지 테스트
	for( it = m_mapSkillGrid.begin(); it != m_mapSkillGrid.end(); it++ )
	{
		SKILL_GRID& skillGrid = it->second;
		CX2SkillTree::SkillTemplet* pSkillTemplet = m_pSkillTree->GetSkillTemplet( skillGrid.m_eSkillID );
		if( NULL != pSkillTemplet )
		{
			D3DXVECTOR2 vNewPos = pSkillTemplet->m_vGridIndex + vDisplace;
			if( vNewPos.x < 0.f || vNewPos.x > m_vGridSize.x-1.f || vNewPos.y < 0.f || vNewPos.y > m_vGridSize.y-1.f )
			{
				return;
			}
		}
	}



	map< std::pair<int,int>, SKILL_GRID > mapNewSkillGrid;
	mapNewSkillGrid = m_mapSkillGrid;
	for( int iRow=0; iRow<m_vGridSize.y; iRow++ )
	{
		for( int iCol=0; iCol<m_vGridSize.x; iCol++ )
		{
			mapNewSkillGrid[ make_pair(iCol, iRow) ].m_eSkillID = CX2SkillTree::SI_NONE;
		}
	}


	for( it = m_mapSkillGrid.begin(); it != m_mapSkillGrid.end(); it++ )
	{
		SKILL_GRID& skillGrid = it->second;
		CX2SkillTree::SkillTemplet* pSkillTemplet = m_pSkillTree->GetSkillTemplet( skillGrid.m_eSkillID );
		if( NULL != pSkillTemplet )
		{
			pSkillTemplet->m_vGridIndex += vDisplace;

			int iX = (int) pSkillTemplet->m_vGridIndex.x;
			int iY = (int) pSkillTemplet->m_vGridIndex.y;

			mapNewSkillGrid[ make_pair( iX, iY ) ].m_eSkillID = skillGrid.m_eSkillID;
		}
	}

	m_mapSkillGrid = mapNewSkillGrid;
	UpdateAllGridUI();

}


void CX2SkillTreeTool::UpdateAvailableSkillList()
{

	if( NULL == m_pSkillTree )
		return;

	CDXUTListBox* pList_AvailSkill = (CDXUTListBox*) m_MainDialog.GetListBox( IDC_LIST_AVAILABLE_SKILL );
	if( NULL == pList_AvailSkill )
		return;

	pList_AvailSkill->RemoveAllItems();



	const map<int, CX2SkillTree::SkillTemplet*>& mapSkillTemplet = m_pSkillTree->GetSkillTempletMap();
	map<int, CX2SkillTree::SkillTemplet*>::const_iterator it;

	WCHAR wszText[1024] = L"";

	
	for( it = mapSkillTemplet.begin(); it != mapSkillTemplet.end(); it++ )
	{
		CX2SkillTree::SkillTemplet* pSkillTemplet = it->second;
		
		if( pSkillTemplet->m_UnitClass != CX2Unit::UC_NONE && m_eUnitClass != pSkillTemplet->m_UnitClass )
			continue;

		//if( pSkillTemplet->m_UnitClass != CX2Unit::UC_NONE && pSkillTemplet->m_UnitClass != m_eUnitClass )
		//	continue;

		if( pSkillTemplet->m_vGridIndex != D3DXVECTOR2(-1, -1 ) )
			continue;

		// fix!! grid 영역을 벗어난 스킬도 추가해야!!

		
		wsprintf( wszText, L"%-6d: %s", pSkillTemplet->m_ID, pSkillTemplet->m_Name.c_str() );
		pList_AvailSkill->AddItem( wszText, (void*) pSkillTemplet->m_ID );

	}

}


CKTDGUIControl::CPictureData* CX2SkillTreeTool::GetPictureArrow( int iGridX, int iGridY, int iDir )
{
	if( NULL == m_pDLGSkillTree )
		return NULL;

	CKTDGUIStatic* pStatic_Hor = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Skill_Hor_Arrow" );
	CKTDGUIStatic* pStatic_Vert = (CKTDGUIStatic*) m_pDLGSkillTree->GetControl( L"Skill_Vert_Arrow" );

	if( NULL == pStatic_Vert )
		return NULL;

	if( NULL == pStatic_Hor )
		return NULL;

	//m_pDLGSkillTree->AddControl( (CKTDGUIControl*) pStatic_Skill_Vert_Arrow );
	//m_pDLGSkillTree->AddControl( (CKTDGUIControl*) pStatic_Skill_Hor_Arrow );

	switch( iDir )
	{
	case DIR_LEFT:
		{
			if( iGridX-1 < 0 || iGridX-1 > m_vGridSize.x-1 || iGridY<0 || iGridY > m_vGridSize.y-1 )
				return NULL;

			return pStatic_Hor->GetPicture( (int)(iGridY* (m_vGridSize.x-1) + iGridX-1) );
		} break;

	case DIR_RIGHT:
		{
			if( iGridX < 0 || iGridX > m_vGridSize.x-1 || iGridY<0 || iGridY > m_vGridSize.y-1 )
				return NULL;

			return pStatic_Hor->GetPicture( (int)(iGridY * (m_vGridSize.x-1) + iGridX) );

		} break;

	case DIR_UP:
		{
			if( iGridX < 0 || iGridX > m_vGridSize.x-1 || iGridY-1<0 || iGridY > m_vGridSize.y-1 )
				return NULL;

			return pStatic_Vert->GetPicture( (int)((iGridY-1)*m_vGridSize.x + iGridX) );
		} break;

	case DIR_DOWN:
		{
			if( iGridX < 0 || iGridX > m_vGridSize.x-1 || iGridY<0 || iGridY > m_vGridSize.y-1 )
				return NULL;

			return pStatic_Vert->GetPicture( (int)(iGridY * m_vGridSize.x + iGridX) );

		} break;
	}

	return NULL;
}


void CX2SkillTreeTool::CloseSkillInfoPopup()
{
	if( NULL != m_pDLGSkillInfoPopup )
	{
		m_pDLGSkillInfoPopup->SetShowEnable( false, false );
	}
}

void CX2SkillTreeTool::OpenSkillInfoPopup( CX2SkillTree::SKILL_ID eSkillID, bool bAtMousePos /*= false*/ )
{
	if( false == m_bEnablePopupInfo )
		return;

	if( NULL == m_pSkillTree )
		return;

	CX2SkillTree::SkillTemplet* pSkillTemplet = m_pSkillTree->GetSkillTemplet( eSkillID );
	if( NULL == pSkillTemplet )
		return;

	if( NULL == m_pDLGSkillInfoPopup )
	{
		m_pDLGSkillInfoPopup = new CKTDGUIDialog( NULL, L"DLG_TOOL_Skill_Info_Popup.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillInfoPopup );
	}

	m_pDLGSkillInfoPopup->SetShowEnable( true, true );

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSkillInfoPopup->GetControl( L"Static_Skill_Info" );
	if( NULL == pStatic )
		return;

	if( true == bAtMousePos )
	{

		D3DXVECTOR2 vMousePos;
		vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
		vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
		vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

		
		D3DXVECTOR2 vPos = g_pKTDXApp->MouseConvertByResolution( (float)vMousePos.x, (float)vMousePos.y );
		D3DXVECTOR2 vSize = pStatic->GetPicture(0)->GetSize();
		if( vPos.x > 1024.f - vSize.x )
		{
			vPos.x = 1024.f - vSize.x;
		}

		if( vPos.y > 768.f - vSize.y )
		{
			vPos.y = 768.f - vSize.y;
		}
		m_pDLGSkillInfoPopup->SetPos( vPos );

	}
	else
	{
		D3DXVECTOR2 vSize = pStatic->GetPicture(0)->GetSize();
		m_pDLGSkillInfoPopup->SetPos( D3DXVECTOR2(0, 768.f-vSize.y) );
	}


	// 스킬 아이콘
	WCHAR wszText[128] = L"";
	pStatic->GetPicture(1)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() );
	

	switch( pSkillTemplet->m_Type )
	{
	case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
		{
			pStatic->GetString(0)->msg = L"#C1D43FF ST_PASSIVE_PHYSIC_ATTACK";

		} break;

	case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
		{
			pStatic->GetString(0)->msg = L"#C1D43FF ST_PASSIVE_MAGIC_ATTACK";
		} break;

	case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
		{
			pStatic->GetString(0)->msg = L"#C1D43FF ST_PASSIVE_MAGIC_DEFENCE";
		} break;

	case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:				
		{
			pStatic->GetString(0)->msg = L"#C1D43FF ST_PASSIVE_PHYSIC_DEFENCE";
		} break;

	case CX2SkillTree::ST_ACTIVE_1:
		{
			pStatic->GetString(0)->msg = L"#C1D43FF ST_ACTIVE_1";
		} break;
	case CX2SkillTree::ST_ACTIVE_2:
		{
			pStatic->GetString(0)->msg = L"#C1D43FF ST_ACTIVE_2";

		} break;
	case CX2SkillTree::ST_ACTIVE_3:
		{
			pStatic->GetString(0)->msg = L"#C1D43FF ST_ACTIVE_3";

		} break;
	}


	wstringstream wstrmDesc;		
	wstrmDesc << pSkillTemplet->m_NeedSP << L" SP"; 
	pStatic->GetString(1)->msg = wstrmDesc.str();
	wstrmDesc.str( L"" );
	pStatic->GetString(2)->msg = pSkillTemplet->m_Name;
	pStatic->GetString(3)->msg = pSkillTemplet->m_Desc;



	// 필요 레벨
	std::wostringstream os;

	os << L"필요 레벨: " << (int) pSkillTemplet->m_NeedLevel;
	pStatic->GetString(4)->msg = os.str().c_str();


	wstrmDesc.str( L"" );
	wstrmDesc << L"SKILL ID: #C00AAAA " << (int)pSkillTemplet->m_ID;
	pStatic->GetString(5)->msg = wstrmDesc.str();



}



void CX2SkillTreeTool::ReplaceString(std::string& text, const std::string& find_token, const std::string& replace_token)
{
	/// \brief 주어진 문자열 내에 존재하는 문자열을 다른 문자열로 치환한다.
	/// \param text 원본 문자열
	/// \param find_token 찾고자 하는 문자열
	/// \param replace_token 치환하고자 하는 문자열

	size_t i = 0;
	while ( (i = text.find(find_token)) != std::string::npos )
	{
		text.replace(i, find_token.size(), replace_token);
	}
}


void CX2SkillTreeTool::ReplaceString(std::wstring& text, const std::wstring& find_token, const std::wstring& replace_token)
{
	/// \brief 주어진 문자열 내에 존재하는 문자열을 다른 문자열로 치환한다.
	/// \param text 원본 문자열
	/// \param find_token 찾고자 하는 문자열
	/// \param replace_token 치환하고자 하는 문자열

	size_t i = 0;
	while ( (i = text.find(find_token)) != std::wstring::npos )
	{
		text.replace(i, find_token.size(), replace_token);
	}
}

void CX2SkillTreeTool::UpdateAllGridBySkillTemplet()
{
	if( NULL == m_pSkillTree )
		return;

	//if( NULL == m_pDLGSkillTree )
	//	return;


	map< std::pair<int,int>, SKILL_GRID >::iterator it_grid;
	//map< std::pair<int,int>, SKILL_GRID >::iterator it2;
	for( it_grid = m_mapSkillGrid.begin(); it_grid != m_mapSkillGrid.end() ; it_grid++ )
	{
		it_grid->second.m_eSkillID = CX2SkillTree::SI_NONE;
	}




	const map<int, CX2SkillTree::SkillTemplet*>& mapSkillTemplet = m_pSkillTree->GetSkillTempletMap();
	map<int, CX2SkillTree::SkillTemplet*>::const_iterator it;

	for( it = mapSkillTemplet.begin(); it != mapSkillTemplet.end(); it++ )
	{
		CX2SkillTree::SkillTemplet* pSkillTemplet = it->second;

		if( m_eUnitClass != pSkillTemplet->m_UnitClass )
			continue;

		//if( pSkillTemplet->m_UnitClass != m_eUnitClass )
		//	continue;

		map< pair<int, int>, SKILL_GRID >::iterator it = m_mapSkillGrid.find( make_pair( (int)pSkillTemplet->m_vGridIndex.x, (int)pSkillTemplet->m_vGridIndex.y ) );
		if( it == m_mapSkillGrid.end() )
			continue;

		SKILL_GRID& skillGrid = it->second;
		skillGrid.m_eSkillID = pSkillTemplet->m_ID;
	}

}



void CX2SkillTreeTool::OpenTextureIconPiece( const WCHAR* wszFilePath )
{
	if( NULL == wszFilePath )
		return;

	SAFE_CLOSE( m_pTextureIconPiece );

	WCHAR wszPath[MAX_PATH] = L"";	
	WCHAR wszFileName[MAX_PATH] = L"";	

	DecomposePathAndFileName( wszPath, wszFileName, wszFilePath );

	m_pTextureIconPiece = g_pKTDXApp->GetDeviceManager()->OpenTexture( wszFileName );
	if( NULL == m_pTextureIconPiece )
	{
		string strDir;
		ConvertWCHARToChar( strDir, wszPath );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

		m_pTextureIconPiece = g_pKTDXApp->GetDeviceManager()->OpenTexture( wszFileName );
		if( NULL == m_pTextureIconPiece )
		{
			return;
		}
	}

	m_wstrNameTextureIcon = wszFileName;
	UpdateTexturePieceList();
}


void CX2SkillTreeTool::UpdateTexturePieceList()
{
	if( NULL == m_pTextureIconPiece )
		 return;

	CDXUTListBox* pList_TexPiece = (CDXUTListBox*) m_PropertyDialog.GetListBox( IDC_LIST_TEXTURE_PIECE_NAME );
	pList_TexPiece->RemoveAllItems();


	map< wstring, CKTDXDeviceTexture::TEXTURE_UV* >::const_iterator it;
	const map< wstring, CKTDXDeviceTexture::TEXTURE_UV* >& mapTexUV = m_pTextureIconPiece->GetMapTexUVRect();
	for( it = mapTexUV.begin(); it != mapTexUV.end(); it++ )
	{
		pList_TexPiece->AddItem( it->first.c_str(), (void*) it->second );
	}
}

