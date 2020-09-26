#include "dxstdafx.h"
#include ".\newdialogtool.h"

CNewDialogTool::CNewDialogTool(void)
{
	m_pLuaManager = NULL;

	g_pKTDXApp->RegisterLuabind( g_pKTDXApp->GetLuaBinder() );


	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"Enum.lua" );
	if( Info == NULL )
		return;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
		return;

	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"Config.lua" );
	if( Info == NULL )
		return;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		return;
	}


}

CNewDialogTool::~CNewDialogTool(void)
{
}

HRESULT CNewDialogTool::OnFrameMove( double fTime, float fElapsedTime )
{
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F4) == TRUE )
	{
		OpenScript();
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}

HRESULT CNewDialogTool::OnFrameRender()
{

	g_pKTDXApp->GetDGManager()->ObjectChainSort();
	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
	g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
	g_pKTDXApp->GetDGManager()->FrontUIRender();

	return S_OK;
}

HRESULT CNewDialogTool::OnResetDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
}

HRESULT CNewDialogTool::OnLostDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
}

bool CNewDialogTool::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );
}

void CNewDialogTool::OpenScript( const WCHAR* scriptFilename )
{
	if( scriptFilename == NULL )
		return;



	g_pKTDXApp->GetDGManager()->GetDialogManager()->ClearAllDlg();

	//파일 로드
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( scriptFilename );

	if( Info == NULL )
	{
		return;
	}
	
	m_pLuaManager = new KLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

	if( m_pLuaManager->DoMemory( Info->pRealData, Info->size ) == false )
	{
		MessageBox(g_pKTDXApp->GetHWND(), L"DialogTool 루아 로드 에러", L"에러", MB_OK);
		SAFE_DELETE( m_pLuaManager );
		return;
	}


	//다이얼로그 데이터 로딩
	if( m_pLuaManager->BeginTable( "FILE_NAME" ) == true )
	{
		char	strKeyName[MAX_PATH]	= "";
		std::wstring	wszFileName;
		std::string		szKeyName;

		for( int i = 1;; i++ )
		{
			sprintf( strKeyName, "DIALOG_FILE%d", i );

			if( m_pLuaManager->GetValue( strKeyName, szKeyName ) == true )
			{
				ConvertCharToWCHAR( wszFileName, szKeyName.c_str() );
				CKTDGUIDialogType	pDialog = new CKTDGUIDialog( NULL, wszFileName.c_str() );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialog );
			}
			else
			{
				break;
			}
		}

		m_pLuaManager->EndTable();
	}

	SAFE_DELETE( m_pLuaManager );
}

