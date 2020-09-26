#include "dxstdafx.h"
#include ".\xfilecomparemain.h"


CXFileCompareMain::CXFileCompareMain(void)
{
	char szWindowTitle[1024];
	sprintf( szWindowTitle, "XFileCompare v0.51; compiled at %s, %s", __DATE__, __TIME__ );
	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );


	CKTDGFontManager::CKTDGFont* pFont;
	pFont = g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewFont( L"굴림", 20, 10, FW_BOLD );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->SetFont( 0, pFont );

	m_pDiaXFileCompare = new CKTDGUIDialog( this, L"NewCompare4.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDiaXFileCompare );

	m_pFCCompare	= NULL;
	m_pFCSort		= NULL;

	m_bSortChecked	= true;
	m_bFileToRemoveFrame = false;

	ParceFileToRemoveFrame();
}

CXFileCompareMain::~CXFileCompareMain(void)
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg( m_pDiaXFileCompare );
	SAFE_DELETE( m_pFCCompare );
	SAFE_DELETE( m_pFCSort );

}

HRESULT CXFileCompareMain::OnFrameMove( double fTime, float fElapsedTime )
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}


HRESULT CXFileCompareMain::OnFrameRender()
{
	HRESULT hr;
	hr = S_OK;

	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameRender();

	if ( m_TempFileName.compare( L"" ) != 0 )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(m_TempFileName.c_str(), 220, 560, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(L"드래그 된 모델이 있습니다.", 220, 580, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	}
	else
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(L"드래그 된 모델이 없습니다.", 220, 580, D3DCOLOR_ARGB( 255, 255, 0, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	}

	if (m_FirstFileName.compare( L"" ) == 0 )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(L"첫번째 모델이 아직 설정 되지 않았습니다.", 50, 90, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	}
	else
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( m_FirstFileName.c_str(), 50, 90, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	}

	if ( m_SecondFileName.compare( L"" ) == 0 )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(L"두번째 모델이 아직 설정 되지 않았습니다.", 50, 120, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	}
	else
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( m_SecondFileName.c_str() , 50, 120, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	}

	if( m_pFCCompare != NULL )
	{
		WCHAR str[MAX_PATH] = {0};

		wsprintf(str, L"첫번째 메쉬의 프레임 개수 = %d", m_pFCCompare->GetFirstFrameNum() );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( str , 50, 200, D3DCOLOR_ARGB( 255, 0, 255, 0  ), D3DCOLOR_ARGB( 255, 255, 0, 0 ) );

		wsprintf(str, L"두번째 메쉬의 프레임 개수 = %d", m_pFCCompare->GetSecondFrameNum() );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( str , 50, 220, D3DCOLOR_ARGB( 255, 0, 255, 0  ), D3DCOLOR_ARGB( 255, 255, 0, 0 ) );

		if( m_pFCCompare->GetCompareResult() == true )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( L"계층 구조가 같습니다." , 50, 250, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		}
		else
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( L"계층 구조가 같지 않습니다." , 50, 250, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( L"첫번째 메쉬구조", 400, 70 - 10, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );

			for( int i = 0; i < (int)m_pFCCompare->GetFirstMeshLogSize(); i++ )
			{
				wstring temp = m_pFCCompare->GetFirstMeshLogByIndex(i);
				if ( i == (int)m_pFCCompare->GetFirstMeshLogSize() - 1 )
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(temp.c_str(), 400, 80 + (i*20), D3DCOLOR_ARGB( 255, 255, 0, 0 ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				}
				else
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(temp.c_str(), 400, 80 + (i*20), D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				}	
			}


			g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( L"두번째 메쉬구조", 600, 70 - 10, D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );

			for( int i = 0; i < (int)m_pFCCompare->GetSecondMeshLogSize(); i++ )
			{
				wstring temp = m_pFCCompare->GetSecondMeshLogByIndex(i);
				if ( i == (int)m_pFCCompare->GetSecondMeshLogSize() - 1 )
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(temp.c_str(), 600, 80 + (i*20), D3DCOLOR_ARGB( 255, 255, 0, 0 ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				}
				else
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText(temp.c_str(), 600, 80 + (i*20), D3DCOLOR_ARGB( 255, 0, 255, 0  ) , D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				}	
			}
		}
	}
	return hr;
}

bool CXFileCompareMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	switch( uMsg )
	{
	case WM_DROPFILES:
		{
			SAFE_DELETE( m_pFCCompare );
			CatchFile( wParam );
			break;
		}

	case CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT:
		{
			switch( wParam )
			{

			case CEM_ALLSORT:
				{
					AllSort();
				}
				break;

			case CEM_MODEL_SORTCHECKED:
				{
					m_bSortChecked = true;
				}
				break;

			case CEM_MODEL_SORTUNCHECKED:
				{
					m_bSortChecked = false;
				}
				break;

			case CEM_OPENFIRSTMODELFILE:
				{
					OpenFirstModelFile();
				}
				break;

			case CEM_OPENSECONDMODELFILE:
				{
					OpenSecondModelFile();
				}
				break;

			case CEM_COMPARE:
				{
					Compare();	
				}
				break;

			case CEM_SORTFIRSTMODEL:
				{
					if( m_FirstFileName.compare( L"" ) != 0 )
					{
						SAFE_DELETE( m_pFCSort );
						m_pFCSort = new CFCSort();
						if( m_pFCSort->SortMesh( m_FirstFileName.c_str() ) == true )
						{
							m_pFCSort->SaveMesh( m_FirstFileName.c_str(), m_bSortChecked );
						}
						SAFE_DELETE( m_pFCSort );
					}
					else
					{
						MessageBox(NULL, L"첫번째 메쉬이름이 셋팅되지 않았습니다", L"소트 에러", MB_OK);
					}
				}
				break;

			case CEM_SORTSECONDMODEL:
				{
					if( m_SecondFileName.compare( L"" ) != 0 )
					{
						SAFE_DELETE( m_pFCSort );
						m_pFCSort = new CFCSort();
						if( m_pFCSort->SortMesh( m_SecondFileName.c_str() ) == true )
						{
							m_pFCSort->SaveMesh( m_SecondFileName.c_str(), m_bSortChecked );
						}
						SAFE_DELETE( m_pFCSort );
					}
					else
					{
						MessageBox(NULL, L"두번째 메쉬이름이 셋팅되지 않았습니다", L"소트 에러", MB_OK);
					}
				}
				break;

			case CEM_SELECTFIRSTMODEL:
				{
					if( m_TempFileName.compare( L"" ) != 0 )
					{
						m_FirstFileName = m_TempFileName;
						m_TempFileName = L"";
					}
					else
					{
						MessageBox(NULL, L"끌어온 메쉬가 없습니다.", L"로드 에러", MB_OK);
					}

					SAFE_DELETE( m_pFCCompare );
				}
				break;

			case CEM_SELECTSECONDMODEL:
				{
					if( m_TempFileName.compare( L"" ) != 0 )
					{
						m_SecondFileName = m_TempFileName;
						m_TempFileName = L"";
					}
					else
					{
						MessageBox(NULL, L"끌어온 메쉬가 없습니다.", L"로드 에러", MB_OK);
					}

					SAFE_DELETE( m_pFCCompare );
				}
				break;

			case CEM_REMOVE_TOP_FRAME_NAME:
				{
					if( m_bFileToRemoveFrame == false )
					{
						CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDiaXFileCompare->GetControl( L"IMEEditFrameNameToRemove" );
						if ( pIMEEditBox != NULL )
						{
							pIMEEditBox->GetText();

							CFrameRemove* pCFrameRemove = new CFrameRemove();
							pCFrameRemove->SetFile( m_FirstFileName );
							pCFrameRemove->RemoveFrameName( pIMEEditBox->GetText() );
							pCFrameRemove->SaveMesh( m_FirstFileName.c_str(), true );
							SAFE_DELETE( pCFrameRemove );
						}
					}
					else
					{
						CFrameRemove* pCFrameRemove = new CFrameRemove();
						pCFrameRemove->SetFile( m_FirstFileName );
						pCFrameRemove->RemoveFrameName( m_vecFileToRemoveFrame );
						pCFrameRemove->SaveMesh( m_FirstFileName.c_str(), true );
						SAFE_DELETE( pCFrameRemove );
					}
					
				}
				break;

			case CEM_FILE_TO_REMOVE_FRAME_CHECKED:
				{
					m_bFileToRemoveFrame = true;
				}
				break;

			case CEM_FILE_TO_REMOVE_FRAME_UN_CHECKED:
				{
					m_bFileToRemoveFrame = false;
				}	
				break;

			}
		}
		break;
	}

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );

}

HRESULT CXFileCompareMain::OnResetDevice()
{
	HRESULT hr;

	hr = S_OK;

	return hr;
}

HRESULT CXFileCompareMain::OnLostDevice()
{
	HRESULT hr;
	hr = S_OK;
	return hr;
}

bool  CXFileCompareMain::CatchFile(WPARAM wParam)
{
	WCHAR PullFileName[MAX_PATH] = L"";
	WCHAR FileName[260] = L"";
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	DragQueryFile((HDROP) wParam, 0, PullFileName, MAX_PATH);
	_wsplitpath( PullFileName, drive, dir, fname, ext);
	wcscat( FileName, fname);
	wcscat( FileName, ext);

	CKTDXDeviceXSkinMesh* pTempMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( FileName );

	if( pTempMesh == NULL )
	{
		string	strDir;
		char	csTemp[MAX_PATH] = "";

		WideCharToMultiByte( CP_ACP, 0, drive, -1, csTemp, MAX_PATH, NULL, NULL );
		strDir += csTemp;
		WideCharToMultiByte( CP_ACP, 0, dir, -1, csTemp, MAX_PATH, NULL, NULL );
		strDir += csTemp;

		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );
		pTempMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( FileName );
		if( pTempMesh == NULL )
		{
			MessageBox(NULL, L"메쉬파일을 찾을 수 없습니다", L"로드 실패", MB_OK);
			return false;
		}
	}

	m_TempFileName = FileName;
	SAFE_CLOSE( pTempMesh );

	return true;
}

bool CXFileCompareMain::Compare()
{
	if( m_FirstFileName.compare( L"" ) == 0 
		|| m_SecondFileName.compare( L"" ) == 0 )
	{
		MessageBox( g_pKTDXApp->GetHWND() , L"메쉬 셋팅이 안되었습니다.", L"실패", MB_OK);
		return false;
	}

	SAFE_DELETE( m_pFCCompare );
	m_pFCCompare = new CFCCompare();

	m_pFCCompare->SetFirstFileName( m_FirstFileName.c_str() );
	m_pFCCompare->SetSecondFileName( m_SecondFileName.c_str() );

	return m_pFCCompare->Compare();
}

bool CXFileCompareMain::OpenFirstModelFile()
{
	OPENFILENAME m_OFN;

	WCHAR lpstrFile[255] = {0,};
	memset(&m_OFN, 0, sizeof(OPENFILENAME) );
	m_OFN.lStructSize = sizeof( OPENFILENAME );
	m_OFN.hwndOwner = g_pKTDXApp->GetHWND();
	m_OFN.lpstrFilter = L"x-file(*.x)\0*.*\0";
	m_OFN.lpstrFile = lpstrFile;
	m_OFN.nMaxFile = MAX_PATH;

	wstring fileName;
	CKTDXDeviceXSkinMesh* pTempMesh = NULL;
	if ( GetOpenFileName( &m_OFN ) != 0 )
	{
		WCHAR drive[_MAX_DRIVE];
		WCHAR dir[_MAX_DIR];
		WCHAR fname[_MAX_FNAME];
		WCHAR ext[_MAX_EXT];

		_wsplitpath( lpstrFile, drive, dir, fname, ext);

		fileName = fname;
		fileName += ext;

		pTempMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

		if( pTempMesh == NULL )
		{
			string	strDir;
			char	csTemp[MAX_PATH] = "";

			WideCharToMultiByte( CP_ACP, 0, drive, -1, csTemp, MAX_PATH, NULL, NULL );
			strDir += csTemp;
			WideCharToMultiByte( CP_ACP, 0, dir, -1, csTemp, MAX_PATH, NULL, NULL );
			strDir += csTemp;

			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );
			pTempMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );
			if( pTempMesh == NULL )
			{
				MessageBox(NULL, L"메쉬파일을 찾을 수 없습니다", L"로드 실패", MB_OK);
				return false;
			}
		}
	}
	m_FirstFileName = fileName;
	SAFE_CLOSE( pTempMesh );
	SAFE_DELETE( m_pFCCompare );
	return true;
}

bool CXFileCompareMain::OpenSecondModelFile()
{
	OPENFILENAME m_OFN;

	WCHAR lpstrFile[255] = {0,};
	memset(&m_OFN, 0, sizeof(OPENFILENAME) );
	m_OFN.lStructSize = sizeof( OPENFILENAME );
	m_OFN.hwndOwner = g_pKTDXApp->GetHWND();
	m_OFN.lpstrFilter = L"x-file(*.x)\0*.*\0";
	m_OFN.lpstrFile = lpstrFile;
	m_OFN.nMaxFile = MAX_PATH;

	wstring fileName;
	CKTDXDeviceXSkinMesh* pTempMesh = NULL;
	if ( GetOpenFileName( &m_OFN ) != 0 )
	{
		WCHAR drive[_MAX_DRIVE];
		WCHAR dir[_MAX_DIR];
		WCHAR fname[_MAX_FNAME];
		WCHAR ext[_MAX_EXT];

		_wsplitpath( lpstrFile, drive, dir, fname, ext);

		fileName = fname;
		fileName += ext;

		pTempMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

		if( pTempMesh == NULL )
		{
			string	strDir;
			char	csTemp[MAX_PATH] = "";

			WideCharToMultiByte( CP_ACP, 0, drive, -1, csTemp, MAX_PATH, NULL, NULL );
			strDir += csTemp;
			WideCharToMultiByte( CP_ACP, 0, dir, -1, csTemp, MAX_PATH, NULL, NULL );
			strDir += csTemp;

			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );
			pTempMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );
			if( pTempMesh == NULL )
			{
				MessageBox(NULL, L"메쉬파일을 찾을 수 없습니다", L"로드 실패", MB_OK);
				return false;
			}
		}
	}
	m_SecondFileName = fileName;
	SAFE_CLOSE( pTempMesh );
	SAFE_DELETE( m_pFCCompare );
	return true;
}


void CXFileCompareMain::AllSort()
{	
	OPENFILENAME m_OFN;
	WCHAR lpstrFile[10000] = {0,};

	WCHAR Name[10000] = L"";
	WCHAR szTmp[MAX_PATH] = L"";

	HRESULT			hr = S_OK;
	LPD3DXMESH		pMeshMerged = NULL;
	LPD3DXBUFFER	pbufAdjacencyMerged = NULL;
	LPD3DXBUFFER	pbufMaterialsMerged = NULL;

	BOOL bSaveHierarchy = TRUE;

	memset(&m_OFN, 0, sizeof(OPENFILENAME) );
	m_OFN.lStructSize = sizeof( OPENFILENAME );
	m_OFN.hwndOwner = g_pKTDXApp->GetHWND();
	m_OFN.lpstrFilter = L"x-file(*.x)\0*.*\0";
	m_OFN.lpstrFile = lpstrFile;
	m_OFN.nMaxFile = 10000;
	m_OFN.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	if ( GetSaveFileName( &m_OFN ) == TRUE )
	{
		WCHAR* pIndex;
		pIndex = lpstrFile;

		vector<wstring> selectFileList;

		WCHAR tempPath[255] = {0,};
		wcscpy( tempPath, pIndex );
		pIndex += wcslen(pIndex) + 1;

		while( *pIndex != NULL )
		{
			WCHAR tempName[255] = {0,};
			wcscpy( tempName, pIndex );
			pIndex += wcslen(pIndex) + 1;

			selectFileList.push_back( tempName );
		}

		for( int i = 0; i < (int)selectFileList.size(); i++ )
		{
			wstring fileName = selectFileList[i];
			CKTDXDeviceXSkinMesh* pTempMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

			if( pTempMesh == NULL )
			{
				string tempPath2;
				ConvertWCHARToChar( tempPath2, tempPath );
				tempPath2 += '\\';
				g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( tempPath2 );
				pTempMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );
				if( pTempMesh == NULL )
				{
					wstring errMsg = fileName;
					errMsg += L"메쉬파일을 찾을 수 없습니다";

					MessageBox(g_pKTDXApp->GetHWND(), errMsg.c_str(), L"로드 실패", MB_OK);
					continue;
				}
			}
			SAFE_DELETE( m_pFCSort );
			m_pFCSort = new CFCSort();
			if( m_pFCSort->SortMesh( fileName.c_str() ) == true )
			{
				m_pFCSort->SaveMeshAuto( tempPath, fileName.c_str(),  m_bSortChecked );
			}
			SAFE_DELETE( m_pFCSort );

			SAFE_CLOSE( pTempMesh );
		}
	}
}

void CXFileCompareMain::ParceFileToRemoveFrame()
{
	KLuaManager kLuaManager;

	g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &kLuaManager, L"FileToRemoveFrame.lua" );
	
	if( kLuaManager.BeginTable( "FileToRemoveFrame" ) == true )
	{
		int tableIndex = 1;
		wstring fileName;
		while( kLuaManager.GetValue( tableIndex, fileName ) == true )
		{
			m_vecFileToRemoveFrame.push_back( fileName );
			tableIndex++;
		}

		kLuaManager.EndTable();
	}
}
