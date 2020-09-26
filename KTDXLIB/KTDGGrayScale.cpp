#include "StdAfx.h"
#include ".\ktdggrayscale.h"

CKTDGGrayScale::CKTDGGrayScale(void)
: m_fSetRate( 1.f )
, m_fSetTime( 1.f )
{
	m_pd3dDevice	= g_pKTDXApp->GetDevice();

	m_bGray			= false;
	m_bGrayInverse	= false;
	m_fCurrRate		= 0.0f;
	m_fCurrTime		= 0.0f;

	m_pTexture		= new TargetTexture();
	m_pDeviceShader	= new DeviceShader( L"RenderState.fx" );

	D3DVIEWPORT9 viewPort;
	m_pd3dDevice->GetViewport( &viewPort );
	float	fWidth		= (float)viewPort.Width;
	float	fHeight		= (float)viewPort.Height;
	m_pTexture->CreateTarget( viewPort.Width, viewPort.Height );

	float fXOff = 0.5f / fWidth;
	float fYOff = 0.5f / fHeight;

	m_Vertex[0].SetVertex(   0.0f,    0.0f, 0.5f, 0+fXOff, 0+fYOff );
	m_Vertex[1].SetVertex( fWidth,    0.0f, 0.5f, 1+fXOff, 0+fYOff );
	m_Vertex[2].SetVertex(   0.0f, fHeight, 0.5f, 0+fXOff, 1+fYOff );
	m_Vertex[3].SetVertex( fWidth, fHeight, 0.5f, 1+fXOff, 1+fYOff );
}

CKTDGGrayScale::~CKTDGGrayScale(void)
{	
	SAFE_DELETE( m_pDeviceShader );
	SAFE_DELETE( m_pTexture );
}

HRESULT CKTDGGrayScale::OnFrameMove( float fElapsedTime )
{
	if( m_bGray == true )
	{
		m_fCurrTime += fElapsedTime;

		if( m_fCurrTime > m_fSetTime )
			m_bGray = false;

		m_fCurrRate = (m_fCurrTime*m_fSetRate)/m_fSetTime;

		debugPrintf( " Curr Rate = %f ", m_fCurrRate );
	}

	return S_OK;
}

HRESULT CKTDGGrayScale::OnResetDevice()
{
	HRESULT hr = 0;

	hr = m_pDeviceShader->OnResetDevice();
	m_pDeviceShader->SetTechnique( "RenderState" );
	m_pTexture->OnResetDevice();

	return hr;
}

HRESULT CKTDGGrayScale::OnLostDevice()
{
	HRESULT hr = 0;

	hr = m_pDeviceShader->OnLostDevice();
	m_pTexture->OnLostDevice();

	return hr;
}

void CKTDGGrayScale::SetGrayInverse( bool bEnable )
{
	m_bGrayInverse = bEnable;
}

void CKTDGGrayScale::SetGray( float fRate, float fTime )
{
	m_fCurrTime = 0.0f;
	m_fCurrRate	= 0.0f;

	m_fSetRate	= fRate;
	m_fSetTime	= fTime;

	m_bGray = true;
}

void CKTDGGrayScale::BeginRender()
{
	if( m_bGray == false && m_bGrayInverse == false )
		return;

	m_pTexture->BeginRenderTarget();
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 100, 100, 100), 1.0f, 0);
}

void CKTDGGrayScale::EndRender()
{
	if( m_bGray == false && m_bGrayInverse == false )
		return;

	m_pTexture->EndRenderTarget();
}

void CKTDGGrayScale::Render()
{
	if( m_bGray == false && m_bGrayInverse == false )
		return;

	m_pDeviceShader->GetEffect()->SetTexture( "GrayTexture", m_pTexture->GetDeviceTexture() );
	m_pDeviceShader->GetEffect()->SetFloat( "g_fGrayRate", m_fCurrRate );

	if( m_bGray == true )
		m_pDeviceShader->BeginPass( 11 );
	else
		m_pDeviceShader->BeginPass( 12 );

	m_pd3dDevice->SetFVF( VERTEX_GS::FVF );
	m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VERTEX_GS) );
	m_pDeviceShader->EndPass();
}

//////////////////////////////////////////////////////////////////////////
//note : TargetTexture
CKTDGGrayScale::TargetTexture::TargetTexture()
{
	m_pd3dDevice	= g_pKTDXApp->GetDevice();
	m_pTexture		= NULL;
	m_pSurface		= NULL;
	m_pOldSurface	= NULL;

	m_dwWidth		= 0;
	m_dwHeight		= 0;
}

CKTDGGrayScale::TargetTexture::~TargetTexture()
{
}

void CKTDGGrayScale::TargetTexture::OnResetDevice()
{
	if( m_dwWidth == 0 || m_dwHeight == 0 )
		return;

	if( m_pTexture != NULL )
		return;

	HRESULT hr = D3DXCreateTexture( m_pd3dDevice,
									m_dwWidth, m_dwHeight,
									1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
									&m_pTexture );

	if( hr != S_OK )
	{
		m_pTexture = NULL;
		return;
	}

	m_pTexture->GetSurfaceLevel( 0, &m_pSurface );

	return;
}

void CKTDGGrayScale::TargetTexture::OnLostDevice()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pSurface );
	SAFE_RELEASE( m_pOldSurface );
}

bool CKTDGGrayScale::TargetTexture::CreateTarget( DWORD dwWidth, DWORD dwHeight )
{
	if( m_dwHeight > 0 && m_dwWidth > 0 )
		return false;

	m_dwWidth	= dwWidth;
	m_dwHeight	= dwHeight;

	return true;
}

void CKTDGGrayScale::TargetTexture::BeginRenderTarget()
{
	SAFE_RELEASE( m_pOldSurface );
	m_pd3dDevice->GetRenderTarget( 0, &m_pOldSurface );
	m_pd3dDevice->SetRenderTarget( 0, m_pSurface );
}

void CKTDGGrayScale::TargetTexture::EndRenderTarget()
{
	m_pd3dDevice->SetRenderTarget( 0, m_pOldSurface );
	SAFE_RELEASE( m_pOldSurface );
}

//////////////////////////////////////////////////////////////////////////
//note : Device Shader
CKTDGGrayScale::DeviceShader::DeviceShader( wstring wstrFileName )
{
	m_wstrFileName	= wstrFileName;
	m_pEffect		= NULL;	
	m_bBegin		= false;
}

CKTDGGrayScale::DeviceShader::~DeviceShader(void)
{
	SAFE_RELEASE(m_pEffect);
}

HRESULT CKTDGGrayScale::DeviceShader::OnResetDevice()
{
	HRESULT hr = 0;
	if( m_pEffect == NULL )
	{
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( m_wstrFileName );
		if( Info == NULL )
		{
			string strName;
			ConvertWCHARToChar( strName, m_wstrFileName.c_str() );
			ErrorLogMsg( KEM_ERROR16, strName.c_str() );

			return E_FAIL;
		}

		DWORD dwShaderFlags = 0; // D3DXFX_NOT_CLONEABLE
		if(FAILED(hr = D3DXCreateEffect( g_pKTDXApp->GetDevice(), Info->pRealData, Info->size,
										NULL, NULL, dwShaderFlags, NULL, &m_pEffect, NULL )))
		{
			ErrorLog( KEM_ERROR17 );

			SAFE_RELEASE( m_pEffect );
			return hr;
		}

		SetTechnique( const_cast<char*>(m_strTechnique.c_str()) );
	}

	return m_pEffect->OnResetDevice(); 
}

HRESULT CKTDGGrayScale::DeviceShader::OnLostDevice()
{
	if( m_pEffect != NULL )
		m_pEffect->OnLostDevice();

	return S_OK;
}

bool CKTDGGrayScale::DeviceShader::SetTechnique( char* strTechnique )
{
	if( m_pEffect == NULL )
		return false;

	if( m_pEffect->SetTechnique( strTechnique ) != S_OK )
		return false;

	m_strTechnique = strTechnique;

	return true;
}

void CKTDGGrayScale::DeviceShader::BeginPass( UINT iIndex )
{
	if( m_pEffect == NULL )
		return;

	if( m_bBegin )
	{
		assert( !"Nested BeginPsss!!!");
		return;
	}


	UINT uiPass;
	if( m_pEffect->Begin(&uiPass, 0) != S_OK )
	{
		assert( !"Effect Begin Failed!!" );
		return;
	}

	if( m_pEffect->BeginPass(iIndex)  != S_OK )
	{
		assert( !"Effect BeginPass Failed!!" );
		return;
	}

	m_bBegin = true;
}

void CKTDGGrayScale::DeviceShader::EndPass()
{
	if( m_pEffect == NULL || m_bBegin == false )
		return;

	if( m_pEffect->EndPass() != S_OK )
	{
		assert( !"Effect EndPass Failed!!" );
		return;
	}

	if( m_pEffect->End() != S_OK )
	{
		assert( !"Effect End Failed!!" );
		return;
	}

	m_bBegin = false;
}
