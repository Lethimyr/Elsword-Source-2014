#include "dxstdafx.h"
#include ".\dialogtool.h"

CDialogTool::CDialogTool(void)
{
	m_pd3dDevice		= g_pKTDXApp->GetDevice();
	m_pTexture			= NULL;
	m_sLinePos.bIsDraw	= false;
	m_vPos.x = 10.0f;
	m_vPos.y = 10.0f;

	m_RenderStateID = CKTDGStateManager::RecordStateBegin();
	{
		CKTDGStateManager::CaptureRenderState( D3DRS_ZENABLE,			FALSE );
		CKTDGStateManager::CaptureRenderState( D3DRS_ZWRITEENABLE,		FALSE );
		CKTDGStateManager::CaptureRenderState( D3DRS_CULLMODE,			D3DCULL_CW );
	}
	CKTDGStateManager::RecordStateEnd();

	//m_pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Texture.tga" );
}

CDialogTool::~CDialogTool(void)
{
	m_pd3dDevice = NULL;
	//g_pKTDXApp->GetDeviceManager()->CloseDevice( L"Texture.tga" );
	SAFE_RELEASE( m_pTexture );
}

HRESULT CDialogTool::OnFrameMove( double fTime, float fElapsedTime )
{
	return S_OK;
}

HRESULT CDialogTool::OnFrameRender()
{
	m_pd3dDevice->SetTexture( 0, m_pTexture );
	DrawFace( m_vPos.x, m_vPos.y, 512, 512 );

	if( m_sLinePos.bIsDraw == true )
	{
		m_pd3dDevice->SetTexture( 0, NULL );
		DrawLine( m_sLinePos.vPos.x, m_sLinePos.vPos.y, m_sLinePos.vSize.x, m_sLinePos.vSize.y );
	}

	return S_OK;
}

HRESULT CDialogTool::OnResetDevice()
{
	wstring datPath = L"dat/";
	datPath += L"texture.tga";

	if( FAILED( D3DXCreateTextureFromFileEx( m_pd3dDevice, L"Texture.tga",
				D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_X8R8G8B8,
				D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
				NULL, NULL,
				&m_pTexture ) ) )
	{
		if( FAILED( D3DXCreateTextureFromFileEx( m_pd3dDevice, datPath.c_str(),
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_X8R8G8B8,
			D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
			NULL, NULL,
			&m_pTexture ) ) )
		{
			MessageBox(NULL, L"Could not find texture.tga", L"test", MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CDialogTool::OnLostDevice()
{
	return S_OK;
}

bool CDialogTool::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static POINTS	pt;
	static bool		bRMouseMove = false;
	static bool		bLMouseMove = false;

	switch( uMsg )
	{
	//////////////////////////////////////////////////////////////////////////
	//note : 그림이동
	case WM_RBUTTONDOWN:
		{
			if( bLMouseMove == true )
				break;

			SetCapture( hWnd );
			pt = MAKEPOINTS( lParam );
			bRMouseMove = true;
		}
		break;
	case WM_RBUTTONUP:
		{
			if( bLMouseMove == true &&
				bRMouseMove == false )
				break;

			bRMouseMove = false;
			POINTS ptEnd;
			ptEnd = MAKEPOINTS( lParam );

			m_vPos.x += (float)(ptEnd.x - pt.x);
			m_vPos.y += (float)(ptEnd.y - pt.y);

			ReleaseCapture();
		}
		break;
	//////////////////////////////////////////////////////////////////////////
	//note : Line draw
	case WM_LBUTTONDOWN:
		{
			if( bRMouseMove == true )
				break;

			SetCapture( hWnd );
			pt = MAKEPOINTS( lParam );
			bLMouseMove = true;

			m_sLinePos.vPos.x = (float)pt.x;
			m_sLinePos.vPos.y = (float)pt.y;
			m_sLinePos.vSize.x = 0.0f;
			m_sLinePos.vSize.y = 0.0f;

			m_sLinePos.bIsDraw = true;
		}
		break;
	case WM_LBUTTONUP:
		{
			if( bRMouseMove == true &&
				bLMouseMove == false )
				break;

			bLMouseMove = false;
			POINTS ptEnd;
			ptEnd = MAKEPOINTS( lParam );

			m_sLinePos.vPos.x = (float)pt.x;
			m_sLinePos.vPos.y = (float)pt.y;

			m_sLinePos.vSize.x = (float)(ptEnd.x - pt.x);
			m_sLinePos.vSize.y = (float)(ptEnd.y - pt.y);

			ReleaseCapture();

			TextureFilltering();
		}
		break;

	//////////////////////////////////////////////////////////////////////////
	//note : 이동처리
	case WM_MOUSEMOVE:
		{
			if( bRMouseMove == true )
			{
				POINTS ptEnd;
				ptEnd = MAKEPOINTS( lParam );

				m_vPos.x += (float)(ptEnd.x - pt.x);
				m_vPos.y += (float)(ptEnd.y - pt.y);

				pt = ptEnd;
			}
			else if( bLMouseMove == true )
			{
				POINTS ptEnd;
				ptEnd = MAKEPOINTS( lParam );

				m_sLinePos.vSize.x = (float)(ptEnd.x - pt.x);
				m_sLinePos.vSize.y = (float)(ptEnd.y - pt.y);
			}
		}
		break;
	}
	return false;
}

void CDialogTool::DrawFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color /* = 0xffffffff  */)
{
	CKTDGStateManager::BeginStateBlock( m_RenderStateID );

	DRAW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(DRAW_VERTEX) * 4 );

	float X1 = (float)nX - 0.5f;
	float Y1 = (float)nY - 0.5f;
	float X2 = (float)(nX + nWidth);
	float Y2 = (float)(nY + nHeight);

	float UMax = 1.0f;
	float VMax = 1.0f;

	//Set up the 4 corners of a small square
	vertex[0].vPos.x = X1;		vertex[0].vPos.y = Y1;
	vertex[0].vPos.z = 1.0f;	vertex[0].vPos.w = 1.0f;
	vertex[0].u = 0.0f;			vertex[0].v = 0.0f;

	vertex[1].vPos.x = X1;		vertex[1].vPos.y = Y2;
	vertex[1].vPos.z = 1.0f;	vertex[1].vPos.w = 1.0f;
	vertex[1].u = 0.0f;			vertex[1].v = VMax;

	vertex[2].vPos.x = X2;		vertex[2].vPos.y = Y1;
	vertex[2].vPos.z = 1.0f;	vertex[2].vPos.w = 1.0f;
	vertex[2].u = UMax;			vertex[2].v = 0.0f;

	vertex[3].vPos.x = X2;		vertex[3].vPos.y = Y2;
	vertex[3].vPos.z = 1.0f;	vertex[3].vPos.w = 1.0f;
	vertex[3].u = UMax;			vertex[3].v = VMax;

	vertex[0].color = vertex[1].color = 
	vertex[2].color = vertex[3].color = color;

	g_pKTDXApp->GetDevice()->SetFVF( DRAW_VERTEX::FVF );
	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(DRAW_VERTEX) );

	CKTDGStateManager::EndStateBlock( m_RenderStateID );
}

void CDialogTool::DrawLine( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color /* = 0xffffffff  */)
{
	CKTDGStateManager::BeginStateBlock( m_RenderStateID );

	LINE_VERTEX vertex[8];
	ZeroMemory( vertex, sizeof(LINE_VERTEX) * 8 );

	vertex[0].vPos.x = nX;
	vertex[0].vPos.y = nY;

	vertex[1].vPos.x = nX + nWidth;
	vertex[1].vPos.y = nY;

	vertex[2].vPos.x = nX + nWidth;
	vertex[2].vPos.y = nY;

	vertex[3].vPos.x = nX + nWidth;
	vertex[3].vPos.y = nY + nHeight;

	vertex[4].vPos.x = nX + nWidth;
	vertex[4].vPos.y = nY + nHeight;

	vertex[5].vPos.x = nX;
	vertex[5].vPos.y = nY + nHeight;

	vertex[6].vPos.x = nX;
	vertex[6].vPos.y = nY + nHeight;

	vertex[7].vPos.x = nX;
	vertex[7].vPos.y = nY;

	for( int i = 0; i < 8; ++i )
	{
		vertex[i].color		= color;
		vertex[i].vPos.z	= 1.0f;
		vertex[i].vPos.w	= 1.0f;
	}

	g_pKTDXApp->GetDevice()->SetFVF( LINE_VERTEX::FVF );
	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_LINELIST, 4, vertex, sizeof(LINE_VERTEX) );

	CKTDGStateManager::EndStateBlock( m_RenderStateID );
}

void SetPoint( POINT& pt, int x, int y )
{
	pt.x = x;
	pt.y = y;
};

void CDialogTool::TextureFilltering()
{
	//m_vPos;
	//m_sLinePos;
	LPDIRECT3DSURFACE9 pSurefaceLevel = NULL;
	if( m_pTexture->GetSurfaceLevel( 0, &pSurefaceLevel ) != D3D_OK )
		return;

	if( pSurefaceLevel != NULL )
	{
		HDC hDC;
		if( pSurefaceLevel->GetDC( &hDC ) != D3D_OK )
		{
			pSurefaceLevel->Release();
			return;
		}

		POINT ptStart;
		POINT ptEnd;

		ptStart.x	= (int)(m_sLinePos.vPos.x - m_vPos.x);
		ptStart.y	= (int)(m_sLinePos.vPos.y - m_vPos.y);
		ptEnd.x		= (int)(m_sLinePos.vSize.x + ptStart.x);
		ptEnd.y		= (int)(m_sLinePos.vSize.y + ptStart.y);

		//비교대상 컬러값을 시작점 값으로 임시 사용한다.
		//추후 등록배경색으로 사용하기
		COLORREF bgColor = GetPixel( hDC, ptStart.x, ptStart.y );

		POINT ptSPos;
		POINT ptEPos;
		bool  bStartCheck = false;
		SetPoint( ptSPos, 0, 0 );
		SetPoint( ptEPos, 0, 0 );
		for( int nY = ptStart.y; nY <= ptEnd.y; ++nY )
		{
			for( int nX = ptStart.x; nX <= ptEnd.x; ++nX )
			{
				if( bgColor != GetPixel( hDC, nX, nY ) )
				{
					if( bStartCheck == false )
					{
						SetPoint( ptSPos, nX, nY );
						SetPoint( ptEPos, nX, nY );
						bStartCheck = true;
					}
					else
					{
						if( ptSPos.x > nX )
							ptSPos.x = nX;
						if( ptSPos.y > nY )
							ptSPos.y = nY;

						if( ptEPos.x < nX )
							ptEPos.x = nX;
						if( ptEPos.y < nY )
							ptEPos.y = nY;
					}
				}
			}
		}

		pSurefaceLevel->ReleaseDC( hDC );
		pSurefaceLevel->Release();

		m_sLinePos.vPos.x = ptSPos.x + m_vPos.x;
		m_sLinePos.vPos.y = ptSPos.y + m_vPos.y;
		m_sLinePos.vSize.x = (float)(ptEPos.x - ptSPos.x);
		m_sLinePos.vSize.y = (float)(ptEPos.y - ptSPos.y);
	}
}
