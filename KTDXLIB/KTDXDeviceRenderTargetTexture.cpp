#include "StdAfx.h"
#include ".\ktdxdevicerendertargettexture.h"


CKTDXDeviceRenderTargetTexture::CKTDXDeviceRenderTargetTexture( LPDIRECT3DDEVICE9 pd3dDevice, wstring deviceID, int width, int height, D3DFORMAT fmt /*= D3DFMT_UNKNOWN*/ )
	: CKTDXDeviceBaseTexture( deviceID, DT_RENDER_TARGET_TEXTURE )
{
	m_DeviceType				= DT_RENDER_TARGET_TEXTURE;
	
	m_D3DFormat					= fmt;
	m_Width						= width;
	m_Height					= height;

	m_pRenderTargetTexture		= NULL;
	m_pRenderTargetSurface		= NULL;
	m_pRenderTargetZSurface		= NULL;

	m_pBackupBuffer				= NULL;
	m_pBackupZBuffer			= NULL;

	m_pDisplayVertexBuffer		= NULL;


//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	m_pVB_RHW_VERTEX				= NULL;
//	m_pVB_RENDERTARGET_RHW_VERTEX	= NULL;
//	m_pVB_RENDERTARGET_VERTEX		= NULL;
//#endif

//{{ robobeg : 2011-03-18
    m_bWithoutFile = true;
//}} robobeg : 2011-03-18

 #ifdef DXUT_KTDXLIB_SAFE_ON_RESET
     if ( DXUTGetDeviceObjectsReset() == true )
 #endif
    {
	    OnResetDevice();
    }//if

}

CKTDXDeviceRenderTargetTexture::~CKTDXDeviceRenderTargetTexture(void)
{
	OnLostDevice();
}

HRESULT CKTDXDeviceRenderTargetTexture::OnResetDevice()
{
	HRESULT hr;

	OnLostDevice();
	

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//
//	if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( 4 * sizeof(CKTDXDeviceTexture::TEXTURE_RHW_VERTEX), 
//		D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_TEXTURE_RHW_VERTEX, 
//		D3DPOOL_DEFAULT, &m_pVB_RHW_VERTEX, NULL) ))
//	{
//		ErrorLogMsg( KEM_ERROR88, L"oops 이게 뭥미" );
//		return false;
//	}
//
//	if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( 4 * sizeof(RENDERTARGET_RHW_VERTEX), 
//		D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_RENDERTARGET_RHW_VERTEX, 
//		D3DPOOL_DEFAULT, &m_pVB_RENDERTARGET_RHW_VERTEX, NULL) ))
//	{
//		ErrorLogMsg( KEM_ERROR88, L"oops 이게 뭥미2" );
//		return false;
//	}
//
//	if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( 4 * sizeof(RENDERTARGET_VERTEX), 
//		D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_RENDERTARGET_VERTEX, 
//		D3DPOOL_DEFAULT, &m_pVB_RENDERTARGET_VERTEX, NULL) ))
//	{
//		ErrorLogMsg( KEM_ERROR88, L"oops 이게 뭥미3" );
//		return false;
//	}
//#endif

	//Keep a handle to the back buffer for easy swapping
	if(FAILED( hr = g_pKTDXApp->GetDevice()->GetRenderTarget( 0, &m_pBackupBuffer )))
	{
		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR76, strName.c_str());

		return E_FAIL;
	}

	//Same for the depth buffer
	if(FAILED( hr = g_pKTDXApp->GetDevice()->GetDepthStencilSurface( &m_pBackupZBuffer )))
	{
		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR77, strName.c_str());

		return E_FAIL;
	}

	D3DSURFACE_DESC desc;
	m_pBackupBuffer->GetDesc( &desc );

	if( D3DFMT_UNKNOWN == m_D3DFormat )
	{
		m_D3DFormat = desc.Format;
	}
	
	if(FAILED( hr = D3DXCreateTexture(g_pKTDXApp->GetDevice(), m_Width, m_Height, 1, 
										D3DUSAGE_RENDERTARGET, m_D3DFormat,
										D3DPOOL_DEFAULT, &m_pRenderTargetTexture)))
	{
		m_pRenderTargetTexture = NULL;

		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR78, strName.c_str());
		
		return E_FAIL;
	}

	//Get the top level surface of the target texture.
	if(FAILED( hr = m_pRenderTargetTexture->GetSurfaceLevel( 0, &m_pRenderTargetSurface )))
	{
		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR79, strName.c_str());

		return E_FAIL;
	}

	m_pBackupZBuffer->GetDesc( &desc );

	if(FAILED( hr = g_pKTDXApp->GetDevice()->CreateDepthStencilSurface(m_Width, 
															m_Height,
															desc.Format,
															desc.MultiSampleType,
															desc.MultiSampleQuality,
															TRUE,
															&m_pRenderTargetZSurface,
															NULL)))
	{
		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR80, strName.c_str());

		return E_FAIL;
	}
	return S_OK;
}

HRESULT CKTDXDeviceRenderTargetTexture::OnLostDevice()
{
//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	SAFE_RELEASE( m_pVB_RHW_VERTEX );
//	SAFE_RELEASE( m_pVB_RENDERTARGET_RHW_VERTEX );
//	SAFE_RELEASE( m_pVB_RENDERTARGET_VERTEX );
//#endif

	SAFE_RELEASE( m_pRenderTargetTexture );
	SAFE_RELEASE( m_pRenderTargetSurface );
	SAFE_RELEASE( m_pRenderTargetZSurface );
	SAFE_RELEASE( m_pBackupBuffer );
	SAFE_RELEASE( m_pBackupZBuffer );
	SAFE_RELEASE( m_pDisplayVertexBuffer );

	return S_OK;
}

void CKTDXDeviceRenderTargetTexture::BeginRender( bool bClear )
{
	KTDXPROFILE();
	//Set up the renderTarget
	HRESULT hr;
	hr = g_pKTDXApp->GetDevice()->SetRenderTarget( 0, m_pRenderTargetSurface );
	hr = g_pKTDXApp->GetDevice()->SetDepthStencilSurface( m_pRenderTargetZSurface );

	if( bClear == true )
	{
		g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	}
}

void CKTDXDeviceRenderTargetTexture::EndRender()
{
	KTDXPROFILE();
	//Set the renderTarget back to the normal back buffer
	HRESULT hr;
	hr = g_pKTDXApp->GetDevice()->SetRenderTarget( 0, m_pBackupBuffer );
	hr = g_pKTDXApp->GetDevice()->SetDepthStencilSurface( m_pBackupZBuffer );
}

void CKTDXDeviceRenderTargetTexture::Clear( D3DCOLOR color )
{
	KTDXPROFILE();

	g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

void CKTDXDeviceRenderTargetTexture::Draw( int nX, int nY, int nWidth, int nHeight, D3DCOLOR color /* = 0xffffffff  */)
{
	KTDXPROFILE();

    KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDXDeviceTexture() )

	D3DXVECTOR3 pos		= g_pKTDXApp->ConvertByResolution( (float)nX, (float)nY );
	D3DXVECTOR3 size	= g_pKTDXApp->ConvertByResolution( (float)nWidth, (float)nHeight );



	float X1 = (float)pos.x - 0.5f;
	float Y1 = (float)pos.y - 0.5f;
	float X2 = (float)(pos.x + size.x);
	float Y2 = (float)(pos.y + size.y);

	float UMax = 1.0f;
	float VMax = 1.0f;

//#ifdef REMOVE_DRAW_PRIMITIVE_UP_2
	CKTDXDeviceTexture::TEXTURE_RHW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(CKTDXDeviceTexture::TEXTURE_RHW_VERTEX) * 4 );

	//Set up the 4 corners of a small square
	vertex[0].x = X1;     vertex[0].y = Y1;
	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
	vertex[0].u = 0.0f;   vertex[0].v = 0.0f;

	vertex[1].x = X1;     vertex[1].y = Y2;
	vertex[1].u = 0.0f;   vertex[1].v = VMax;
	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;

	vertex[2].x = X2;     vertex[2].y = Y1;
	vertex[2].u = UMax;   vertex[2].v = 0.0f;
	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;

	vertex[3].x = X2;     vertex[3].y = Y2;
	vertex[3].u = UMax;   vertex[3].v = VMax;
	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;

	vertex[0].color = vertex[1].color = 
	vertex[2].color = vertex[3].color = color;
//#endif

	SetDeviceTexture();

//#endif //REMOVE_DRAW_PRIMITIVE_UP_2

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_TEXTURE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
		, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_TEXTURE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(CKTDXDeviceTexture::TEXTURE_RHW_VERTEX) );
//#endif DYNAMIC_VERTEX_BUFFER_OPT
	KD3DEND()
}

void CKTDXDeviceRenderTargetTexture::RenderQuadRHW( D3DCOLOR color, float fX, float fY, float fWidth, float fHeight )
{
	KTDXPROFILE();
    KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDXDeviceTexture() );
	

	D3DVIEWPORT9 Viewport;
	g_pKTDXApp->GetDevice()->GetViewport(&Viewport);

	float X1, Y1, X2, Y2;
	if( fWidth == -1.f || fHeight == -1.f )
	{
		X1 = (float)Viewport.X;
		Y1 = (float)Viewport.Y;
		X2 = (float)Viewport.X + Viewport.Width;
		Y2 = (float)Viewport.Y + Viewport.Height;
	}
	else
	{
		X1 = fX;
		Y1 = fY;
		X2 = X1 + fWidth;
		Y2 = Y1 + fHeight;
	}

	float UMax = 1.0f;
	float VMax = 1.0f;

//#ifdef REMOVE_DRAW_PRIMITIVE_UP_2
	RENDERTARGET_RHW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(RENDERTARGET_RHW_VERTEX) * 4 );

	//Set up the 4 corners of a small square
	vertex[0].x = X1;     vertex[0].y = Y1;
	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
	vertex[0].u = 0.0f;   vertex[0].v = 0.0f;

	vertex[2].x = X1;     vertex[2].y = Y2;
	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;
	vertex[2].u = 0.0f;   vertex[2].v = VMax;

	vertex[1].x = X2;     vertex[1].y = Y1;
	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;
	vertex[1].u = UMax;   vertex[1].v = 0.0f;

	vertex[3].x = X2;     vertex[3].y = Y2;
	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;
	vertex[3].u = UMax;   vertex[3].v = VMax;

	vertex[0].color = vertex[1].color = 
		vertex[2].color = vertex[3].color = color;

//#endif

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_RENDERTARGET_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
		, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else   DYNAMIC_VERTEX_BUFFER_OPT
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_RENDERTARGET_RHW_VERTEX );
//	
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(RENDERTARGET_RHW_VERTEX) );
//#endif  DYNAMIC_VERTEX_BUFFER_OPT

	KD3DEND()
}

void CKTDXDeviceRenderTargetTexture::RenderQuad( D3DCOLOR color )
{
	KTDXPROFILE();
    KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDXDeviceTexture() )


	//HRESULT hr;
	
//#ifdef REMOVE_DRAW_PRIMITIVE_UP_2 

	RENDERTARGET_VERTEX vertex[6];
	//ZeroMemory( vertex, sizeof(RENDERTARGET_VERTEX) * 6 );

	vertex[0].vPos = D3DXVECTOR3(-1,1,0);
	vertex[0].vTex = D3DXVECTOR2(0,0);

	vertex[1].vPos = D3DXVECTOR3(1,1,0);
	vertex[1].vTex = D3DXVECTOR2(1,0);

	vertex[2].vPos = D3DXVECTOR3(-1,-1,0);
	vertex[2].vTex = D3DXVECTOR2(0,1);

	vertex[3].vPos = D3DXVECTOR3(1,-1,0);
	vertex[3].vTex = D3DXVECTOR2(1,1);

	vertex[4].vPos = D3DXVECTOR3(-1,-1,0);
	vertex[4].vTex = D3DXVECTOR2(0,1);

	vertex[5].vPos = D3DXVECTOR3(1,1,0);
	vertex[5].vTex = D3DXVECTOR2(1,0);

	/*
	vertex[0].color = vertex[1].color = 
		vertex[2].color = vertex[3].color = 
			vertex[4].color = vertex[5].color = color;
			*/
//#endif //REMOVE_DRAW_PRIMITIVE_UP_2

//#ifdef  DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_RENDERTARGET_VERTEX == D3DFVF_XYZ_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
		, D3DPT_TRIANGLELIST, 2, vertex );
//#else   DYNAMIC_VERTEX_BUFFER_OPT
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_RENDERTARGET_VERTEX );	
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, (void*)vertex, sizeof(RENDERTARGET_VERTEX) );
//
//#endif  DYNAMIC_VERTEX_BUFFER_OPT
	KD3DEND()
}

void CKTDXDeviceRenderTargetTexture::SetDeviceTexture( int stage )
{
//{{ robobeg : 2008-10-22
	//if( g_pKTDXApp->IsSameBeforeTexture( stage, m_pRenderTargetTexture ) == false )
	//{
	//	if ( FAILED(g_pKTDXApp->GetDevice()->SetTexture( stage, m_pRenderTargetTexture ) ) )
	//	{
	//		string strName;
	//		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
	//		ErrorLogMsg( KEM_ERROR81, strName.c_str() );
	//		return;
	//	}
	//	g_pKTDXApp->SetBeforeTexture( stage, m_pRenderTargetTexture );
	//}
    if ( FAILED( CKTDGStateManager::SetTexture( stage, m_pRenderTargetTexture ) ) )
    {
		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR81, strName.c_str() );
    }//if
//}} robobeg : 2008-10-22
}

HRESULT CKTDXDeviceRenderTargetTexture::CopySurface( LPDIRECT3DSURFACE9 pDestSurface )
{
	KTDXPROFILE();

	HRESULT hr;

	hr = g_pKTDXApp->GetDevice()->StretchRect( m_pRenderTargetSurface, NULL, pDestSurface, NULL, D3DTEXF_LINEAR );
	if ( hr != S_OK )
	{
		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR82, strName.c_str() );
	}
	return hr;
}

HRESULT CKTDXDeviceRenderTargetTexture::CopySurfaceToBackBuffer()
{
	KTDXPROFILE();

	HRESULT hr;

	hr = g_pKTDXApp->GetDevice()->StretchRect( m_pRenderTargetSurface, NULL, m_pBackupBuffer, NULL, D3DTEXF_LINEAR );

	if ( hr != S_OK )
	{
		switch( hr )
		{
		case D3DERR_INVALIDCALL:
			{
				wstring errorMSG;
				errorMSG = L"D3DERR_INVALIDCALL ";
				errorMSG += m_DeviceID;
				ErrorLogMsg( KEM_ERROR83, errorMSG.c_str() );
			}
			break;
		}
	}
	return hr;
}

HRESULT CKTDXDeviceRenderTargetTexture::CopyBackBufferToSurface()
{
	KTDXPROFILE();

	HRESULT hr;

	if( g_pKTDXApp->GetCannotStretchRect() == true )
	{
		return E_FAIL;
	}

	hr = g_pKTDXApp->GetDevice()->StretchRect( m_pBackupBuffer, NULL, m_pRenderTargetSurface, NULL, D3DTEXF_LINEAR );

	if ( hr != S_OK )
	{
		switch( hr )
		{
		case D3DERR_INVALIDCALL:
			{
				wstring errorMSG;
				errorMSG = L"D3DERR_INVALIDCALL ";
				errorMSG += m_DeviceID;
				ErrorLogMsg( KEM_ERROR84, errorMSG.c_str() );
			}
			break;
		}
	}
	return hr;
}
