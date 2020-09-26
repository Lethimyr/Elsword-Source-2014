#include "dxstdafx.h"
#include ".\x2textureviewer.h"



void CALLBACK OnMainDialogEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2TextureViewer* pTextureViewer = (CX2TextureViewer*) pUserContext;
	CDXUTDialog& mainDialog = pTextureViewer->GetMainDialog();

	switch( nControlID )
	{
	case CX2TextureViewer::IDC_EDIT_CENTER_POS:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int iValue[2];

					pTextureViewer->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iValue[0], 2 );
					D3DXVECTOR2 vLeftTopPos( (float) iValue[0], (float)iValue[1] );
					vLeftTopPos -= pTextureViewer->GetSize()*0.5f;
					pTextureViewer->SetLeftTopPos( vLeftTopPos );
					mainDialog.ClearFocus();

				} break;
			}

		} break;

	case CX2TextureViewer::IDC_EDIT_SIZE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int iValue[2];

					pTextureViewer->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iValue[0], 2 );
					pTextureViewer->SetSize( D3DXVECTOR2( (float)iValue[0], (float)iValue[1] ) );
					mainDialog.ClearFocus();

				} break;
			}

		} break;
	}
}


CX2TextureViewer::CX2TextureViewer(void)
{
	char szWindowTitle[1024];
	sprintf( szWindowTitle, "X2 Texture Viewer; compiled at %s, %s", __DATE__, __TIME__ );
	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );

	m_pDeviceTexture = NULL;


	//g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(),	true );
	g_pKTDXApp->GetDGManager()->GetCamera()->Point( 0, 0, 0, 0, 0, 1 );
	g_pKTDXApp->GetDGManager()->GetCamera()->UpdateCamera( 1.0f );

	
	m_vSize = D3DXVECTOR2( 400, 400 );
	m_vLeftTopPos = D3DXVECTOR2(512, 384) - m_vSize * 0.5f;

	InitDialog();
	UpdateDialog();
}

CX2TextureViewer::~CX2TextureViewer(void)
{
	SAFE_CLOSE( m_pDeviceTexture );
	FinalDialog();
}

HRESULT CX2TextureViewer::OnFrameMove( double fTime, float fElapsedTime )
{


	////m_FPSCamera.FrameMove( fElapsedTime );
	//g_pKTDXApp->GetDGManager()->GetCamera()->Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
	//D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * g_pKTDXApp->GetDGManager()->GetFar()*0.1f + *m_FPSCamera.GetEyePt();
	//g_pKTDXApp->GetDGManager()->GetCamera()->LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
	//g_pKTDXApp->GetDGManager()->GetCamera()->UpdateCamera( fElapsedTime );



	return S_OK;
}

HRESULT CX2TextureViewer::OnFrameRender()
{
	if( NULL != m_pDeviceTexture )
	{
		m_pDeviceTexture->Draw( (int)m_vLeftTopPos.x, (int)m_vLeftTopPos.y, (int)m_vSize.x, (int)m_vSize.y );

		m_MainDialog.OnRender( g_pKTDXApp->GetElapsedTime() );
	}

	return S_OK;
}

HRESULT CX2TextureViewer::OnResetDevice()
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();

	return S_OK;
}

HRESULT CX2TextureViewer::OnLostDevice()
{

	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();


	return S_OK;
}

bool CX2TextureViewer::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bNoFurtherProcessing = false;
	//m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);


	bNoFurtherProcessing = m_MainDialog.MsgProc( hWnd, uMsg, wParam, lParam );
	if( true == bNoFurtherProcessing )
		return true;


	switch( uMsg )
	{
	case WM_DROPFILES:
		{
			OnDropFile( hWnd, uMsg, wParam, lParam );

			bNoFurtherProcessing = true;
		} break;

	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_UP:
				{
					m_vLeftTopPos.y -= 10.f;
					UpdateDialog();
				} break;
			case VK_DOWN:
				{
					m_vLeftTopPos.y += 10.f;
					UpdateDialog();
				} break;
			case VK_LEFT:
				{
					m_vLeftTopPos.x -= 10.f;
					UpdateDialog();
				} break;
			case VK_RIGHT:
				{
					m_vLeftTopPos.x += 10.f;
					UpdateDialog();
				} break;

			case VK_ADD:
				{
					D3DXVECTOR2 vCenterPos = m_vLeftTopPos + m_vSize * 0.5f;
					m_vSize *= 1.1f;

					m_vLeftTopPos = vCenterPos - m_vSize * 0.5f;
					UpdateDialog();
				} break;

			case VK_SUBTRACT:
				{
					D3DXVECTOR2 vCenterPos = m_vLeftTopPos + m_vSize * 0.5f;
					m_vSize *= 0.9f;
					if( m_vSize.x < 1.f )
					{
						m_vSize.x = 1.f;
					}

					if( m_vSize.y < 1.f )
					{
						m_vSize.y = 1.f;
					}
					
					m_vLeftTopPos = vCenterPos - m_vSize * 0.5f;
					UpdateDialog();
				} break;
			}

		} break;
	}

	return bNoFurtherProcessing;
} 


void CX2TextureViewer::FinalDialog()
{

}

void CX2TextureViewer::InitDialog()
{
	// initialize UI
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	m_MainDialog.Init( &g_DialogResourceManager );
	m_MainDialog.SetCallback( OnMainDialogEvent, this );
	m_MainDialog.SetSize( 300, 300 );
	m_MainDialog.SetFont(0, L"±¼¸²", 12, 12);
	m_MainDialog.SetLocation( 0, 0 );

	int iX = 10;
	int iY = 30;
	int iDX = 100;
	int iDY = 35;
	
	m_MainDialog.AddStatic( IDC_STATIC_DUMMY, L"center pos:", iX, iY, 125, 22 );
	m_MainDialog.AddEditBox( IDC_EDIT_CENTER_POS, L"", iX+iDX, iY, 80, 30 );
	iY += 35;

	m_MainDialog.AddStatic( IDC_STATIC_DUMMY, L"size:", iX, iY, 125, 22 );
	m_MainDialog.AddEditBox( IDC_EDIT_SIZE, L"", iX+iDX, iY, 80, 30 );
	iY += 35;
}

void CX2TextureViewer::UpdateDialog()
{
	
	CDXUTEditBox* pEdit_CenterPos = (CDXUTEditBox*) m_MainDialog.GetControl( IDC_EDIT_CENTER_POS );
	CDXUTEditBox* pEdit_Size = (CDXUTEditBox*) m_MainDialog.GetControl( IDC_EDIT_SIZE );
 
	int iValue[2] = { 0, };

	iValue[0] = (int) ( m_vLeftTopPos.x + m_vSize.x * 0.5f );
	iValue[1] = (int) ( m_vLeftTopPos.y + m_vSize.y * 0.5f );
	SetTextIntArrayToEditBox( pEdit_CenterPos, (int*)&iValue, 2 );

	iValue[0] = (int) ( m_vSize.x );
	iValue[1] = (int) ( m_vSize.y );
	SetTextIntArrayToEditBox( pEdit_Size, (int*)&iValue, 2 );
}


bool CX2TextureViewer::OnDropFile( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	SAFE_CLOSE( m_pDeviceTexture );

	WCHAR wszFilePath[MAX_PATH] = L"";	
	WCHAR wszPath[MAX_PATH] = L"";	
	WCHAR wszFileName[MAX_PATH] = L"";	
	DragQueryFile( (HDROP)wParam, 0, wszFilePath, MAX_PATH );
	DecomposePathAndFileName( wszPath, wszFileName, wszFilePath );

	m_pDeviceTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( wszFileName );
	if( NULL == m_pDeviceTexture )
	{
		string strDir;
		ConvertWCHARToChar( strDir, wszPath );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

		m_pDeviceTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( wszFileName );
		if( NULL == m_pDeviceTexture )
		{
			return false;
		}
	}

	SetWindowText( g_pKTDXApp->GetHWND(), wszFilePath );
	return true;
}


bool CX2TextureViewer::DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath )
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



void CX2TextureViewer::ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount )
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


void CX2TextureViewer::SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount )
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











//////////////////////////////////////////////////////////////////////////
using namespace std;

void CALLBACK CX2NPCTool_OnMainDialogEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2NPCTool* pNPCTool = (CX2NPCTool*) pUserContext;
	CDXUTDialog& mainDialog = pNPCTool->GetMainDialog();

	//switch( nControlID )
	//{
	//case CX2TextureViewer::IDC_EDIT_CENTER_POS:
	//	{
	//		switch( nEvent )
	//		{
	//		case EVENT_EDITBOX_STRING:
	//			{
	//				int iValue[2];

	//				pTextureViewer->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iValue[0], 2 );
	//				D3DXVECTOR2 vLeftTopPos( (float) iValue[0], (float)iValue[1] );
	//				vLeftTopPos -= pTextureViewer->GetSize()*0.5f;
	//				pTextureViewer->SetLeftTopPos( vLeftTopPos );
	//				mainDialog.ClearFocus();

	//			} break;
	//		}

	//	} break;

	//case CX2TextureViewer::IDC_EDIT_SIZE:
	//	{
	//		switch( nEvent )
	//		{
	//		case EVENT_EDITBOX_STRING:
	//			{
	//				int iValue[2];

	//				pTextureViewer->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iValue[0], 2 );
	//				pTextureViewer->SetSize( D3DXVECTOR2( (float)iValue[0], (float)iValue[1] ) );
	//				mainDialog.ClearFocus();

	//			} break;
	//		}

	//	} break;
	//}
}


CX2NPCTool::CX2NPCTool(void)
{
	char szWindowTitle[1024];
	sprintf( szWindowTitle, "X2 NPC Tool; compiled at %s, %s", __DATE__, __TIME__ );
	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );


	g_pKTDXApp->GetDGManager()->GetCamera()->Point( 0, 0, 0, 0, 0, 1 );
	g_pKTDXApp->GetDGManager()->GetCamera()->UpdateCamera( 1.0f );


	InitDialog();
	UpdateDialog();

	RegisterLuabind();
}

CX2NPCTool::~CX2NPCTool(void)
{
	FinalDialog();
}

HRESULT CX2NPCTool::OnFrameMove( double fTime, float fElapsedTime )
{


	return S_OK;
}

HRESULT CX2NPCTool::OnFrameRender()
{

	m_MainDialog.OnRender( g_pKTDXApp->GetElapsedTime() );

	return S_OK;
}

HRESULT CX2NPCTool::OnResetDevice()
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();

	return S_OK;
}

HRESULT CX2NPCTool::OnLostDevice()
{

	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();


	return S_OK;
}

bool CX2NPCTool::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bNoFurtherProcessing = false;


	bNoFurtherProcessing = m_MainDialog.MsgProc( hWnd, uMsg, wParam, lParam );
	if( true == bNoFurtherProcessing )
		return true;


	switch( uMsg )
	{
	case WM_DROPFILES:
		{
			OnDropFile( hWnd, uMsg, wParam, lParam );

			bNoFurtherProcessing = true;
		} break;

	//case WM_KEYDOWN:
	//	{
	//		switch( wParam )
	//		{
	//		case VK_UP:
	//			{
	//				m_vLeftTopPos.y -= 10.f;
	//				UpdateDialog();
	//			} break;
	//		case VK_DOWN:
	//			{
	//				m_vLeftTopPos.y += 10.f;
	//				UpdateDialog();
	//			} break;
	//		case VK_LEFT:
	//			{
	//				m_vLeftTopPos.x -= 10.f;
	//				UpdateDialog();
	//			} break;
	//		case VK_RIGHT:
	//			{
	//				m_vLeftTopPos.x += 10.f;
	//				UpdateDialog();
	//			} break;

	//		case VK_ADD:
	//			{
	//				D3DXVECTOR2 vCenterPos = m_vLeftTopPos + m_vSize * 0.5f;
	//				m_vSize *= 1.1f;

	//				m_vLeftTopPos = vCenterPos - m_vSize * 0.5f;
	//				UpdateDialog();
	//			} break;

	//		case VK_SUBTRACT:
	//			{
	//				D3DXVECTOR2 vCenterPos = m_vLeftTopPos + m_vSize * 0.5f;
	//				m_vSize *= 0.9f;
	//				if( m_vSize.x < 1.f )
	//				{
	//					m_vSize.x = 1.f;
	//				}

	//				if( m_vSize.y < 1.f )
	//				{
	//					m_vSize.y = 1.f;
	//				}

	//				m_vLeftTopPos = vCenterPos - m_vSize * 0.5f;
	//				UpdateDialog();
	//			} break;
	//		}

	//	} break;
	}

	return bNoFurtherProcessing;
} 


void CX2NPCTool::FinalDialog()
{

}

void CX2NPCTool::InitDialog()
{
	// initialize UI
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	m_MainDialog.Init( &g_DialogResourceManager );
	m_MainDialog.SetCallback( OnMainDialogEvent, this );
	m_MainDialog.SetSize( 300, 300 );
	m_MainDialog.SetFont(0, L"±¼¸²", 12, 12);
	m_MainDialog.SetLocation( 0, 0 );

	int iX = 10;
	int iY = 30;
	int iDX = 100;
	int iDY = 35;

	m_MainDialog.AddStatic( IDC_STATIC_DUMMY, L"center pos:", iX, iY, 125, 22 );
	m_MainDialog.AddEditBox( IDC_EDIT_CENTER_POS, L"", iX+iDX, iY, 80, 30 );
	iY += 35;

	m_MainDialog.AddStatic( IDC_STATIC_DUMMY, L"size:", iX, iY, 125, 22 );
	m_MainDialog.AddEditBox( IDC_EDIT_SIZE, L"", iX+iDX, iY, 80, 30 );
	iY += 35;
}

void CX2NPCTool::UpdateDialog()
{

	//CDXUTEditBox* pEdit_CenterPos = (CDXUTEditBox*) m_MainDialog.GetControl( IDC_EDIT_CENTER_POS );
	//CDXUTEditBox* pEdit_Size = (CDXUTEditBox*) m_MainDialog.GetControl( IDC_EDIT_SIZE );

	//int iValue[2] = { 0, };

	//iValue[0] = (int) ( m_vLeftTopPos.x + m_vSize.x * 0.5f );
	//iValue[1] = (int) ( m_vLeftTopPos.y + m_vSize.y * 0.5f );
	//SetTextIntArrayToEditBox( pEdit_CenterPos, (int*)&iValue, 2 );

	//iValue[0] = (int) ( m_vSize.x );
	//iValue[1] = (int) ( m_vSize.y );
	//SetTextIntArrayToEditBox( pEdit_Size, (int*)&iValue, 2 );
}


bool CX2NPCTool::OnDropFile( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	WCHAR wszFilePath[MAX_PATH] = L"";	
	WCHAR wszPath[MAX_PATH] = L"";	
	WCHAR wszFileName[MAX_PATH] = L"";	
	DragQueryFile( (HDROP)wParam, 0, wszFilePath, MAX_PATH );
	DecomposePathAndFileName( wszPath, wszFileName, wszFilePath );


	

	if( false == OpenScriptFile( wszFileName ) )
	{
		string strDir;
		ConvertWCHARToChar( strDir, wszPath );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

		if( false == OpenScriptFile( wszFileName ) )
		{
			return false;
		}
	}

	LoadNPCTemplet( wszFileName );

	SaveNPCScript();


	SetWindowText( g_pKTDXApp->GetHWND(), wszFilePath );
	return true;
}


bool CX2NPCTool::DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath )
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



void CX2NPCTool::ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount )
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


void CX2NPCTool::SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount )
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


bool CX2NPCTool::OpenScriptFile( const WCHAR* pFileName )
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( pFileName ) )
		return false;
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"Enum.lua" ) )
		return false;
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, pFileName ) )
		return false;


	m_vecNPCData.clear();


	int iMonsterIndex = 0;
	while( true == luaManager.BeginTable( L"MONSTER", iMonsterIndex ) )
	{
		NPCData npcData;

		LUA_GET_VALUE( luaManager, L"NAME",						npcData.NAME,						L"" );
		LUA_GET_VALUE( luaManager, L"MOTION_FILE_NAME",			npcData.MOTION_FILE_NAME,			L"" );
		LUA_GET_VALUE( luaManager, L"MOTION_ANI_TEX_XET",		npcData.MOTION_ANI_TEX_XET,			L"" );
		LUA_GET_VALUE( luaManager, L"MOTION_CHANGE_TEX_XET",	npcData.MOTION_CHANGE_TEX_XET,		L"" );

		LUA_GET_VALUE( luaManager, L"USE_SLASH_TRACE",		npcData.USE_SLASH_TRACE,	false );
		LUA_GET_VALUE( luaManager, L"DRAW_SMALL_MP_BAR",	npcData.DRAW_SMALL_MP_BAR,	false );
		LUA_GET_VALUE( luaManager, L"STATE_WIN",			npcData.STATE_WIN,			false );
		LUA_GET_VALUE( luaManager, L"STATE_WAITHABIT", 		npcData.STATE_WAITHABIT, 	false );
		LUA_GET_VALUE( luaManager, L"STATE_WAITSTART", 		npcData.STATE_WAITSTART, 	false );
		LUA_GET_VALUE( luaManager, L"STATE_JUMP_READY",		npcData.STATE_JUMP_READY,	false );
		LUA_GET_VALUE( luaManager, L"STATE_DASH",			npcData.STATE_DASH,			false );
		LUA_GET_VALUE( luaManager, L"STATE_DASH_END",		npcData.STATE_DASH_END,		false );

		LUA_GET_VALUE( luaManager, L"STATE_START_CAMERA",	npcData.STATE_START_CAMERA,		false );


		



		m_vecNPCData.push_back( npcData );
		iMonsterIndex++;

		luaManager.EndTable();
	}

	return true;

}


bool CX2NPCTool::LoadNPCTemplet( const WCHAR* pFileName )
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( pFileName ) )
		return false;
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"Enum.lua" ) )
		return false;
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, pFileName ) )
		return false;


	m_NPCTemplet.Init();


	string strFileName = "";
	ConvertWCHARToChar( strFileName, pFileName );
	
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( strFileName.c_str() );
	if( Info == NULL )
		return false;

	wstring wstrNPCTemplet = L"";
	ConvertCharToWCHAR( wstrNPCTemplet, Info->pRealData, Info->size );


	int iStartIndex, iEndIndex;
	if( true == FindTable( wstrNPCTemplet, L"INIT_SYSTEM", iStartIndex, iEndIndex ) )
	{
		m_NPCTemplet.INIT_SYSTEM = wstrNPCTemplet.substr( iStartIndex, iEndIndex-iStartIndex+1 );
	}

	if( true == FindTable( wstrNPCTemplet, L"INIT_DEVICE", iStartIndex, iEndIndex ) )
	{
		m_NPCTemplet.INIT_DEVICE = wstrNPCTemplet.substr( iStartIndex, iEndIndex-iStartIndex+1 );
	}

	if( true == FindTable( wstrNPCTemplet, L"INIT_PHYSIC", iStartIndex, iEndIndex ) )
	{
		m_NPCTemplet.INIT_PHYSIC = wstrNPCTemplet.substr( iStartIndex, iEndIndex-iStartIndex+1 );
	}

	if( true == FindTable( wstrNPCTemplet, L"INIT_COMPONENT", iStartIndex, iEndIndex ) )
	{
		m_NPCTemplet.INIT_COMPONENT = wstrNPCTemplet.substr( iStartIndex, iEndIndex-iStartIndex+1 );
	}

	if( true == FindTable( wstrNPCTemplet, L"INIT_AI", iStartIndex, iEndIndex ) )
	{
		m_NPCTemplet.INIT_AI = wstrNPCTemplet.substr( iStartIndex, iEndIndex-iStartIndex+1 );
	}




	//
	//
	//if( true == luaManager.BeginTable( L"INIT_DEVICE" ) ) 
	//{
	//	if( true == luaManager.BeginTable( L"READY_TEXTURE" ) )
	//	{
	//		wstring wstrValue;

	//		int i = 1;
	//		while( true == luaManager.GetValue( i, wstrValue ) )
	//		{
	//			m_NPCTemplet.READY_TEXTURE.push_back( wstrValue ); 
	//			i++;
	//		}

	//		luaManager.EndTable();
	//	}
	//	if( true == luaManager.BeginTable( L"READY_SOUND" ) )
	//	{
	//		wstring wstrValue;

	//		int i = 1;
	//		while( true == luaManager.GetValue( i, wstrValue ) )
	//		{
	//			m_NPCTemplet.READY_SOUND.push_back( wstrValue ); 
	//			i++;
	//		}

	//		luaManager.EndTable();
	//	}

	//	if( true == luaManager.BeginTable( L"READY_XMESH" ) )
	//	{
	//		wstring wstrValue;

	//		int i = 1;
	//		while( true == luaManager.GetValue( i, wstrValue ) )
	//		{
	//			m_NPCTemplet.READY_XMESH.push_back( wstrValue ); 
	//			i++;
	//		}

	//		luaManager.EndTable();
	//	}
	//	if( true == luaManager.BeginTable( L"READY_XSKIN_MESH" ) )
	//	{
	//		wstring wstrValue;

	//		int i = 1;
	//		while( true == luaManager.GetValue( i, wstrValue ) )
	//		{
	//			m_NPCTemplet.READY_XSKIN_MESH.push_back( wstrValue ); 
	//			i++;
	//		}

	//		luaManager.EndTable();
	//	}

	//	luaManager.EndTable();
	//}



	//
	return true;

}

void CX2NPCTool::SaveNPCScript()
{
	std::wofstream ofs;
	
	for( UINT i=0; i<m_vecNPCData.size(); i++ )
	{
		NPCData& npcData = m_vecNPCData[i];

		string strFileName = "";
		ConvertWCHARToChar( strFileName, npcData.NAME.c_str() );
		strFileName += ".lua";

		ofs.open( strFileName.c_str(), ios::binary );
		if( false == ofs.is_open() )
			continue;


		ofs << L"-- " << npcData.NAME << endl;
		ofs << m_NPCTemplet.INIT_SYSTEM << endl;
		ofs << m_NPCTemplet.INIT_DEVICE << endl;
		

		ofs << L"INIT_MOTION =" << endl;
		ofs << L"{" << endl;
		ofs << L"\tMOTION_FILE_NAME = \"" << npcData.MOTION_FILE_NAME << "\"," << endl;
		ofs << L"\tMOTION_CHANGE_TEX_XET = \"" << npcData.MOTION_CHANGE_TEX_XET << "\"," << endl;
		ofs << L"\tMOTION_ANI_TEX_XET = \"" << npcData.MOTION_ANI_TEX_XET << "\"," << endl;
		ofs << L"}" << endl;
		

		ofs << m_NPCTemplet.INIT_PHYSIC << endl;
		ofs << m_NPCTemplet.INIT_COMPONENT << endl;
		ofs << m_NPCTemplet.INIT_AI << endl;

		ofs << L"INIT_STATE =" << endl;
		ofs << L"{" << endl;

		if( true == npcData.STATE_START_CAMERA )
		{
			ofs << L"{ STATE_NAME = \"" << npcData.NAME << L"_START\", \t\t LUA_CAMERA_MOVE_FUNC = \"" 
				<< npcData.NAME << L"_START_CAMERA_MOVE\", }," << endl;
		}
		else
		{
			ofs << L"{ STATE_NAME = \"" << npcData.NAME << L"_START\", \t\t }, " << endl;
		}


		ofs << L"}" << endl;
		

		//{ STATE_NAME = "GIANT_PPORU_BLACK_START",						LUA_CAMERA_MOVE_FUNC = "GIANT_PPORU_BLACK_START_CAMERA_MOVE",					},
		//{ STATE_NAME = "GIANT_PPORU_BLACK_WIN",						},




		ofs.close();
	}
}

void CX2NPCTool::RegisterLuabind()
{
	//lua_tinker::class_<CX2NPCTool>("CX2NPCTool")
	//	.def("SetTeamStartPos",		CKTDGLineMap::SetTeamStartPos)
	//	;

}






bool CX2NPCTool::FindTable( wstring& wstrText, WCHAR* wszTableName, int& iStartIndex, int& iEndIndex )
{
	iStartIndex = wstrText.find( wszTableName );
	iEndIndex = 0;

	if( iStartIndex != std::wstring::npos )
	{
		int iOpeningBracketCount = 0;
		int iClosingBracketCount = 0;

		int iLastClosingBracketIndex = iStartIndex;
		int iLastOpeningBracketIndex = iStartIndex;

		int iNewClosingBracketIndex = wstrText.find( L"}", iLastClosingBracketIndex+1 );
		if( -1 == iNewClosingBracketIndex ||
			iNewClosingBracketIndex == iLastClosingBracketIndex+1 )
			return false;
		iLastClosingBracketIndex = iNewClosingBracketIndex;
		iClosingBracketCount++;




		while( true )
		{
			int iNewOpeningBracketIndex = wstrText.find( L"{", iLastOpeningBracketIndex+1 );
			if( -1 == iNewOpeningBracketIndex ||
				iNewOpeningBracketIndex == iLastOpeningBracketIndex+1 )
			{
				if( iOpeningBracketCount == iClosingBracketCount )
				{
					iEndIndex = iLastClosingBracketIndex;
					return true;
				}
				else
					return false;
			}
			iLastOpeningBracketIndex = iNewOpeningBracketIndex;
			iOpeningBracketCount++;


			if( iLastOpeningBracketIndex > iLastClosingBracketIndex )
			{
				iEndIndex = iLastClosingBracketIndex;
				return true;
			}

			if( iOpeningBracketCount > iClosingBracketCount )
			{
				int iNewClosingBracketIndex = wstrText.find( L"}", iLastClosingBracketIndex+1 );
				if( -1 == iNewClosingBracketIndex ||
					iNewClosingBracketIndex == iLastClosingBracketIndex+1 )
					return false;
				iLastClosingBracketIndex = iNewClosingBracketIndex;				
				iClosingBracketCount++;
			}
		}
	}

	return false;
}