#include "dxstdafx.h"
#include ".\texviewer.h"

CTEXViewer::CTEXViewer(void)
{
	OpenScriptFile( L"Enum.lua" );
	OpenScriptFile( L"Config.lua" );

	m_pDialog = new CKTDGUIDialog( this, L"DLG_TEXViewer.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDialog );

	m_pFont = g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewFont( L"굴림체");
	
	m_pEnlargeTexture = NULL;

	ReInit();

}

CTEXViewer::~CTEXViewer(void)
{
	for ( int i = 0; i < (int)m_TextrueDeviceList.size(); i++ )
	{
		CKTDXDeviceTexture* pDeviceTexture = m_TextrueDeviceList[i];
		SAFE_CLOSE( pDeviceTexture );
	}
	m_TextrueDeviceList.clear();

	for ( int i = 0; i < (int)m_TexDataList.size(); i++ )
	{
		TEX_DATA* pTexData = m_TexDataList[i];
		SAFE_DELETE( pTexData );
	}
	m_TexDataList.clear();

	SAFE_DELETE_DIALOG( m_pDialog );
	SAFE_CLOSE( m_pEnlargeTexture );

	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyFont( m_pFont );
}

HRESULT CTEXViewer::OnFrameMove( double fTime, float fElapsedTime )
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F6 ) == TRUE )
	{
		ReInit();	
	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F7 ) == TRUE )
	{
		
	}
/*
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_1 ) == TRUE )
	{
		m_EnlargePercent = 50;
		m_EnlargeTexPos = D3DXVECTOR2( 0, 0 );
	}
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_2 ) == TRUE )
	{
		m_EnlargePercent = 100;	
		m_EnlargeTexPos = D3DXVECTOR2( 0, 0 );
	}
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_3 ) == TRUE )
	{
		m_EnlargePercent = 150;
		m_EnlargeTexPos = D3DXVECTOR2( 0, 0 );
	}
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_4 ) == TRUE )
	{
		m_EnlargePercent = 200;	
		m_EnlargeTexPos = D3DXVECTOR2( 0, 0 );
	}
*/
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_1 ) == TRUE )
	{
		m_EnlargePercent -= 10;
		if ( m_EnlargePercent < 10 )
			m_EnlargePercent = 10;
	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_2 ) == TRUE )
	{
		m_EnlargePercent += 10;
		if ( m_EnlargePercent > 200 )
			m_EnlargePercent = 200;
	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_3 ) == TRUE )
	{
		m_EnlargePercent = 100;
		m_EnlargeTexPos = D3DXVECTOR2( 0, 0 );
	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_PGUP ) == TRUE)
	{
		if ( m_NowViewPage > 1 )
			m_NowViewPage--;
	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_PGDN ) == TRUE)
	{
		if ( (int)m_TexDataList.size() - ( m_NowViewPage * 12 ) > 0 )
		{
			m_NowViewPage++;
		}
	}

	return S_OK;
}

HRESULT CTEXViewer::OnFrameRender()
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameRender();

	UIRender();

	return S_OK;
}

HRESULT CTEXViewer::OnResetDevice()
{

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
}

HRESULT CTEXViewer::OnLostDevice()
{

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
}

bool CTEXViewer::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_DROPFILES:
			{
				return CatchFile( wParam );
			}
			break;

		case WM_LBUTTONDOWN:
			{
				POINT mousepos = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
				D3DXVECTOR2 pt;
				pt = g_pKTDXApp->MouseConvertByResolution( (float)mousepos.x, (float)mousepos.y );

				if ( m_bEnlargeTexture == true )
				{
					if ( IsMousePointInEnlargePicture( pt ) == true )
					{
						m_bEnlargeTexDragging = true;
						return true;
					}
					else
					{

						SAFE_CLOSE( m_pEnlargeTexture );
						m_bEnlargeTexture = false;
						m_EnlargeTexDataIndex = -1;
		

						int SelectedTexDataIndex = -1;
						if ( IsMousePointInPicture( pt, SelectedTexDataIndex ) == true )
						{
							m_bClickedPicture = true;
							m_SelectedTexDataIndex = SelectedTexDataIndex;
							return true;
						}
					}
				}
				else
				{
					int SelectedTexDataIndex = -1;
					if ( IsMousePointInPicture( pt, SelectedTexDataIndex ) == true )
					{
						m_bClickedPicture = true;
						m_SelectedTexDataIndex = SelectedTexDataIndex;
						return true;
					}
					
				}
			}
			break;

		case WM_LBUTTONUP:
			{
				if ( m_bEnlargeTexture == true )
				{
					m_bEnlargeTexDragging = false;
/*
					SAFE_CLOSE( m_pEnlargeTexture );
					m_bEnlargeTexture = false;
					m_EnlargeTexDataIndex = -1;
					*/
				}
				else
				{
					POINT mousepos = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
					D3DXVECTOR2 pt;
					pt = g_pKTDXApp->MouseConvertByResolution( (float)mousepos.x, (float)mousepos.y );

					int SelectedTexDataIndex = -1;
					if ( IsMousePointInPicture( pt, SelectedTexDataIndex ) == true )
					{
						if ( m_bClickedPicture == true && m_SelectedTexDataIndex == SelectedTexDataIndex )
						{
							m_bEnlargeTexture = true;
							m_pEnlargeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( m_TexDataList[SelectedTexDataIndex]->texFileName );
							m_EnlargeTexDataIndex = SelectedTexDataIndex;
							m_EnlargeTexPos = D3DXVECTOR2( 0, 0 );
							return true;
						}
					}
				}

				
				m_SelectedTexDataIndex = -1;
				m_bClickedPicture = false;
			}
			break;

		case WM_RBUTTONDOWN:
			{

			}
			break;

		case WM_RBUTTONUP:
			{
				if ( m_bEnlargeTexture == true )
				{
					SAFE_CLOSE( m_pEnlargeTexture );
					m_bEnlargeTexture = false;
					m_EnlargeTexDataIndex = -1;
				}
			}
			break;

		case WM_MOUSEMOVE:
			{
				POINT mousepos = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
				D3DXVECTOR2 pt;
				pt = g_pKTDXApp->MouseConvertByResolution( (float)mousepos.x, (float)mousepos.y );
				static D3DXVECTOR2 prevMousePoint = pt;

				if ( m_bEnlargeTexDragging == true )
				{
					m_EnlargeTexPos.x += pt.x - prevMousePoint.x;
					m_EnlargeTexPos.y += pt.y - prevMousePoint.y;
				}

				prevMousePoint = pt;
			}
			break;

		case CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT:
			{
				switch( wParam )
				{
				case VDM_PREV:
					{
						if ( m_NowViewPage > 1 )
							m_NowViewPage--;
					}
					break;

				case VDM_NEXT:
					{
						if ( (int)m_TexDataList.size() - ( m_NowViewPage * 12 ) > 0 )
						{
							m_NowViewPage++;
						}
					}
					break;
				}
			}
			break;
	}
	
	if ( m_bEnlargeTexture == true )
	{
		return false;
	}
	else
		return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );
}

void CTEXViewer::ReInit()
{
	m_NowViewPage = 1;
	m_bClickedPicture = false;
	m_SelectedTexDataIndex = -1;

	m_bEnlargeTexture = false;
	SAFE_CLOSE( m_pEnlargeTexture );
	m_EnlargeTexDataIndex = -1;
	m_EnlargePercent = 100;
	m_EnlargeTexPos = D3DXVECTOR2( 0, 0 );

	for ( int i = 0; i < (int)m_TextrueDeviceList.size(); i++ )
	{
		CKTDXDeviceTexture* pDeviceTexture = m_TextrueDeviceList[i];
		SAFE_CLOSE( pDeviceTexture );
	}
	m_TextrueDeviceList.clear();

	for ( int i = 0; i < (int)m_TexDataList.size(); i++ )
	{
		TEX_DATA* pTexData = m_TexDataList[i];
		SAFE_DELETE( pTexData );
	}
	m_TexDataList.clear();
}


void CTEXViewer::UIRender()
{
	int posX = 100;
	int posY = 10;
	int width = 100;
	int height = 100;

	int i = (m_NowViewPage - 1)*12 + 1;
	int j = 1;

	for ( ; i <= (int)m_TexDataList.size() && i <= m_NowViewPage * 12; i++, j++ )
	{
		TEX_DATA* pTexData = m_TexDataList[i - 1];

		int finalPosX = posX;
		int finalPosY = posY + height * j + ((j-1)*10);
		if ( i > (m_NowViewPage - 1)*12 + 12/2 )
		{
			finalPosX += 500;
			finalPosY = posY + height * ( j - 6 ) + (((j - 6)-1)*10); 
		}

		CKTDXDeviceTexture* pDeviceTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexData->texFileName );
		pDeviceTexture->Draw( finalPosX, finalPosY, width, height , pTexData->pTexUV );
		SAFE_CLOSE(pDeviceTexture);

		WCHAR texBuff[256] = {0};
		swprintf( texBuff, L"텍스쳐 이름 : %s\n키 값 : %s", pTexData->texFileName.c_str(), pTexData->keyName.c_str() );
			m_pFont->Draw2DText( texBuff, finalPosX + width, finalPosY, D3DXCOLOR(1,1,1,1), D3DXCOLOR(1,1,1,1) );
	}

	WCHAR texBuff[256] = {0};

	int texDataListSize = (int)m_TexDataList.size();//UI에 뿌리기 위한 임시 트릭 변수

	if ( texDataListSize == 0 )
		texDataListSize = 1;	//UI에 뿌리기 위한 임시 트릭 변수

	swprintf( texBuff, L"%d / %d", m_NowViewPage, (int)((texDataListSize - 1)/12 + 1) );
	m_pFont->Draw2DText( texBuff, 130, 20, D3DXCOLOR(1,1,1,1), D3DXCOLOR(1,1,1,1) );

	swprintf( texBuff, L"현제 확대 퍼센트 : %d", m_EnlargePercent );
	m_pFont->Draw2DText( texBuff, 300, 20, D3DXCOLOR(1,1,1,1), D3DXCOLOR(1,1,1,1) );

	swprintf( texBuff, L"ㅁPageUp : 이전 페이지\nㅁPageDown : 다음 페이지" );
	m_pFont->Draw2DText( texBuff, 600, 40, D3DXCOLOR(1,1,1,1), D3DXCOLOR(1,1,1,1) );

	swprintf( texBuff, L"ㅁF6 : 리셋" );
	m_pFont->Draw2DText( texBuff, 600, 20, D3DXCOLOR(1,1,1,1), D3DXCOLOR(1,1,1,1) );

	swprintf( texBuff, L"ㅁ1 : 10%% 확대\nㅁ2 : 10%% 축소\nㅁ3 : 원본 크기" );
	m_pFont->Draw2DText( texBuff, 300, 40, D3DXCOLOR(1,1,1,1), D3DXCOLOR(1,1,1,1) );

	if ( m_pEnlargeTexture != NULL )
	{
		float enlargeWidth = 0;
		float enlargeHeight = 0;

		//D3DXVECTOR2 leftTop = m_TexDataList[m_EnlargeTexDataIndex]->pTexUV->leftTop;
		//D3DXVECTOR2 rightBottom = m_TexDataList[m_EnlargeTexDataIndex]->pTexUV->rightBottom;

		float rectWidth = m_TexDataList[m_EnlargeTexDataIndex]->pTexUV->rectSize.x;
		float rectHeight = m_TexDataList[m_EnlargeTexDataIndex]->pTexUV->rectSize.y;

		//enlargeWidth = g_pKTDXApp->GetViewport().Width * m_EnlargePercent/100.0f;
		//enlargeHeight = g_pKTDXApp->GetViewport().Height * m_EnlargePercent/100.0f;
		
		enlargeWidth = rectWidth * m_EnlargePercent/100.0f;
		enlargeHeight = rectHeight * m_EnlargePercent/100.0f;

		m_pEnlargeTexture->Draw( (int)m_EnlargeTexPos.x, (int)m_EnlargeTexPos.y, (int)enlargeWidth, (int)enlargeHeight, m_TexDataList[m_EnlargeTexDataIndex]->pTexUV );
	}
}

bool CTEXViewer::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pMain", this );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
		return false;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
		return false;

	return true;
}


bool CTEXViewer::CatchFile( WPARAM wParam )
{
	WCHAR PullFileName[MAX_PATH] = L"";
	WCHAR FileName[260] = L"";
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	UINT numFiles;
	numFiles = DragQueryFile( (HDROP) wParam, -1, NULL, 0 );

	for( UINT uFile = 0; uFile < numFiles; ++uFile )
	{
		if( DragQueryFile( (HDROP) wParam, uFile, PullFileName, MAX_PATH ) > 0 )
		{
			WCHAR textBuff[256] = {0};

			_wsplitpath( PullFileName, drive, dir, fname, ext);
			memset( FileName, 0, sizeof(FileName) );
			wcscat( FileName, fname );
			wcscat( FileName, ext );

			wstring WeWantFileExt = L".TGA";
			wstring fileExt = wcsupr( ext );
			if ( WeWantFileExt == fileExt || fileExt == L".DDS" )
			{
				bool bIsValidFileCheck = true;
				for ( int i = 0; i < (int)m_TextrueDeviceList.size(); i++ )		//FIX! 여기 디바이스 매니져에서 확인해봐야할꺼같은데. 파일 많아지면 속도 많이 느려질듯
				{
					CKTDXDeviceTexture* pDeviceTexture = m_TextrueDeviceList[i];
					wstring wstrFileName = wcsupr( FileName );
					if ( pDeviceTexture->GetDeviceID().compare( wstrFileName.c_str() ) == 0 )
					{
						swprintf( textBuff, L"이미 로드된 텍스쳐 입니다.\nFileName : %s", FileName );
						MessageBox( g_pKTDXApp->GetHWND(), textBuff, L"경고", MB_OK );
						bIsValidFileCheck = false;
						break;
					}
				}
				if ( bIsValidFileCheck == true )
					AddTexture( PullFileName );
			}
			else
			{
				swprintf( textBuff, L"TGA 포맷의 텍스쳐 파일만 지원합니다.\nFileName : %s", FileName );
				MessageBox( g_pKTDXApp->GetHWND(), textBuff, L"에러", MB_OK );
			}
		}
	}   // end for


	

	

	return true;
}

bool CTEXViewer::AddTexture( const WCHAR* pFullFileName )
{
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	_wsplitpath( pFullFileName, drive, dir, fname, ext);

	wstring fileName;

	fileName = fname;
	fileName += ext;

	CKTDXDeviceTexture* pDeviceTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( fileName );

	if ( pDeviceTexture == NULL )
	{
		

		string	strDir;
		char	csTemp[MAX_PATH] = "";

		WideCharToMultiByte( CP_ACP, 0, drive, -1, csTemp, MAX_PATH, NULL, NULL );
		strDir += csTemp;
		WideCharToMultiByte( CP_ACP, 0, dir, -1, csTemp, MAX_PATH, NULL, NULL );
		strDir += csTemp;

		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );
		pDeviceTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( fileName );
		if ( pDeviceTexture == NULL )
		{
			wstring explanation;
			explanation = fileName + L"텍스쳐 파일을 찾을 수 없습니다";
			MessageBox( g_pKTDXApp->GetHWND(), explanation.c_str(), L"에러",  MB_OK );
			return false;
		}
	}

	m_TextrueDeviceList.push_back( pDeviceTexture );

	map< wstring, CKTDXDeviceTexture::TEXTURE_UV* >::iterator i;

	for ( i = pDeviceTexture->m_mapTexUVRect.begin(); i != pDeviceTexture->m_mapTexUVRect.end(); i++ )
	{
		TEX_DATA* pTexData = new TEX_DATA();
		pTexData->keyName		= i->first;

		pTexData->pTexUV		= new CKTDXDeviceTexture::TEXTURE_UV();
		CKTDXDeviceTexture::TEXTURE_UV* pOrgTexUV = i->second;
		pTexData->pTexUV->leftTop		= pOrgTexUV->leftTop;
		pTexData->pTexUV->rightBottom	= pOrgTexUV->rightBottom;
		pTexData->pTexUV->leftBottom	= pOrgTexUV->leftBottom;
		pTexData->pTexUV->rightTop		= pOrgTexUV->rightTop;

		pTexData->pTexUV->rectSize		= pOrgTexUV->rectSize;
		
		pTexData->texFileName	= fileName;

		m_TexDataList.push_back( pTexData );
	}

	return true;
}

bool CTEXViewer::IsMousePointInPicture( D3DXVECTOR2& pt, int& texDataIndex )
{
	int posX = 100;
	int posY = 10;
	int width = 100;
	int height = 100;

	int i = (m_NowViewPage - 1)*12 + 1;
	int j = 1;

	for ( ; i <= (int)m_TexDataList.size() && i <= m_NowViewPage * 12; i++, j++ )
	{
		TEX_DATA* pTexData = m_TexDataList[i - 1];

		int finalPosX = posX;
		int finalPosY = posY + height * j + ((j-1)*10);
		if ( i > (m_NowViewPage - 1)*12 + 12/2 )
		{
			finalPosX += 500;
			finalPosY = posY + height * ( j - 6 ) + (((j - 6)-1)*10); 
		}

		if ( IsInMousePoint( pt, finalPosX, finalPosY, width, height ) == true )
		{
			texDataIndex = i - 1;
			return true;
		}
	}

	return false;
}

bool CTEXViewer::IsMousePointInEnlargePicture( D3DXVECTOR2&pt )
{
	if ( m_pEnlargeTexture != NULL )
	{
		float enlargeWidth = 0;
		float enlargeHeight = 0;

		//D3DXVECTOR2 leftTop = m_TexDataList[m_EnlargeTexDataIndex]->pTexUV->leftTop;
		//D3DXVECTOR2 rightBottom = m_TexDataList[m_EnlargeTexDataIndex]->pTexUV->rightBottom;

		float rectWidth = m_TexDataList[m_EnlargeTexDataIndex]->pTexUV->rectSize.x;
		float rectHeight = m_TexDataList[m_EnlargeTexDataIndex]->pTexUV->rectSize.y;

		//enlargeWidth = g_pKTDXApp->GetViewport().Width * m_EnlargePercent/100.0f;
		//enlargeHeight = g_pKTDXApp->GetViewport().Height * m_EnlargePercent/100.0f;

		enlargeWidth = rectWidth * m_EnlargePercent/100.0f;
		enlargeHeight = rectHeight * m_EnlargePercent/100.0f;

		m_pEnlargeTexture->Draw( (int)m_EnlargeTexPos.x, (int)m_EnlargeTexPos.y, (int)enlargeWidth, (int)enlargeHeight, m_TexDataList[m_EnlargeTexDataIndex]->pTexUV );

		if ( IsInMousePoint( pt, (int)m_EnlargeTexPos.x, (int)m_EnlargeTexPos.y, (int)enlargeWidth, (int)enlargeHeight ) == true )
		{
			return true;
		}
	}

	return false;	
}

bool CTEXViewer::IsInMousePoint( D3DXVECTOR2 pt, int x, int y, int width, int height )
{
	if ( pt.x >= x && pt.x <= x + width &&
		pt.y >= y && pt.y <= y + height )
	{
		return true;
	}
	
	return false;
}