/**	@file	: X2ViewerGrid.cpp
	@desc	: 뷰어 내부에 격자 출력
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#include "dxstdafx.h"
#include ".\x2viewergrid.h"

#pragma region CX2ViewerGrid
CX2ViewerGrid::CX2ViewerGrid(void)
	: m_pKTDIMouse( NULL )
	, m_pGridVB( NULL )
	, m_bOnOff( true )
{
	CX2ViewerObject::SetObjectStyle( CX2ViewerObject::OS_GRID );
	m_pFont	= new CKTDGFontManager::CUKFont( L"굴림체", 15, 0, false );
	m_pMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );
}
#pragma endregion 생성자

#pragma region ~CX2ViewerGrid
CX2ViewerGrid::~CX2ViewerGrid(void)
{
	SAFE_DELETE( m_pMatrix );
	SAFE_DELETE( m_pFont );
	SAFE_RELEASE( m_pGridVB );
}
#pragma endregion 소멸자

#pragma region Init
bool CX2ViewerGrid::Init()
{
	HRESULT			hr;

	SAFE_RELEASE( m_pGridVB );

	//m_pd3dDevice	= g_pKTDXApp->GetDevice();
	m_pKTDIMouse	= g_pKTDXApp->GetDIManager()->GetMouse();

	if( FAILED( hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( LINE_NUM * sizeof(GRID_VERTEX), 0, D3DFVF_GRID_VERTEX, D3DPOOL_DEFAULT, &m_pGridVB, NULL ) ) )
	{
		ERRORMSG( L"Grid VertexBuffer 생성에러(CX2ViewerGrid::Init())" );
		return false;
	}

	GRID_VERTEX*	pVertices;

	if( FAILED( hr = m_pGridVB->Lock( 0, LINE_NUM * sizeof(GRID_VERTEX), (void**)&pVertices, 0 ) ) )
	{
		ERRORMSG( L"Grid Lock 접근에러(CX2ViewerGrid::Init())" );
		return false;
	}

	//note : 기준
	// X
	pVertices[0].pos	= D3DXVECTOR3( -500.0f, 0.0f, 0.0f);
	pVertices[0].color	= 0xffff0000;
	pVertices[1].pos	= D3DXVECTOR3(  500.0f, 0.0f, 0.0f);
	pVertices[1].color	= 0xffff0000;
	// Y
	pVertices[2].pos	= D3DXVECTOR3( 0.0f, -500.0f, 0.0f);
	pVertices[2].color	= 0xff00ff00;
	pVertices[3].pos	= D3DXVECTOR3( 0.0f,  500.0f, 0.0f);
	pVertices[3].color	= 0xff00ff00;
	// Z
	pVertices[4].pos	= D3DXVECTOR3( 0.0f, 0.0f, -500.0f);
	pVertices[4].color	= 0xff0000ff;
	pVertices[5].pos	= D3DXVECTOR3( 0.0f, 0.0f,  500.0f);
	pVertices[5].color	= 0xff0000ff;

	int nCut = 6;
	for( int i = -5; i <= 5; ++i )
	{
		if( i != 0 )
		{
			pVertices[nCut  ].pos	= D3DXVECTOR3( -500.0f, 0.0f, 0.0f );
			pVertices[nCut  ].pos.z	= 100.0f * i;
			pVertices[nCut++].color	= 0xffffffff;
			pVertices[nCut  ].pos	= D3DXVECTOR3(  500.0f, 0.0f, 0.0f );
			pVertices[nCut  ].pos.z	= 100.0f * i;
			pVertices[nCut++].color	= 0xffffffff;

			pVertices[nCut  ].pos	= D3DXVECTOR3( 0.0f, 0.0f, -500.0f );
			pVertices[nCut  ].pos.x	= 100.0f * i;
			pVertices[nCut++].color	= 0xffffffff;
			pVertices[nCut  ].pos	= D3DXVECTOR3( 0.0f, 0.0f,  500.0f );
			pVertices[nCut  ].pos.x	= 100.0f * i;
			pVertices[nCut++].color	= 0xffffffff;
		}
	}

	if( FAILED( hr = m_pGridVB->Unlock() ) )
	{
		ERRORMSG( L"Grid Lock 접근에러(CX2ViewerGrid::Init())" );
		return false;
	}

	return true;
}
#pragma endregion 초기화

#pragma region OnFrameMove
HRESULT CX2ViewerGrid::OnFrameMove( double fTime, float fElapsedTime)
{
	if( m_bOnOff == false )
		return S_OK;

	return S_OK;
}
#pragma endregion 매 프레임 갱신

#pragma region OnFrameRender
HRESULT CX2ViewerGrid::OnFrameRender()
{
	if( m_bOnOff == false )
		return S_OK;

	m_pMatrix->UpdateWorldMatrix();

	HRESULT hr;

	if( FAILED(hr = g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_GRID_VERTEX )) )
		return hr;

	if( FAILED(hr = g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pGridVB, 0, sizeof(GRID_VERTEX) )) )
		return hr;

	//g_pKTDXApp->SetNULLTexture( 0 );
	CKTDGStateManager::SetTexture( 0, NULL );
	if( FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_LINELIST, 0, LINE_NUM/2)) )
		return hr;

	/*m_pFont->Draw3DText( L"X", D3DXVECTOR3( 200.0f,  20.0f,   0.0f ), 0xffff0000, 0xffffffff, DT_LEFT, CKTDGFontManager::FS_SHELL );
	m_pFont->Draw3DText( L"Y", D3DXVECTOR3(  10.0f, 200.0f,   0.0f ), 0xff00ff00, 0xffffffff, DT_LEFT, CKTDGFontManager::FS_SHELL );
	m_pFont->Draw3DText( L"Z", D3DXVECTOR3(   0.0f,  20.0f, 200.0f ), 0xff0000ff, 0xffffffff, DT_LEFT, CKTDGFontManager::FS_SHELL );*/

	return S_OK;
}
#pragma endregion 매 프레임 렌더링

#pragma region OnResetDevice
HRESULT CX2ViewerGrid::OnResetDevice()
{
	Init();

	m_pFont->OnResetDevice();

	return S_OK;
}
#pragma endregion 장치 재설정

#pragma region OnLostDevice
HRESULT CX2ViewerGrid::OnLostDevice()
{
	SAFE_RELEASE( m_pGridVB );

	m_pKTDIMouse	= NULL;

	m_pFont->OnLostDevice();

	return S_OK;
}
#pragma endregion 장치 소실