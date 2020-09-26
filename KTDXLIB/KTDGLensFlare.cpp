#include "StdAfx.h"
#include ".\ktdglensflare.h"


//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
		// render spots
		KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
		KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),
		KRenderState( D3DRS_ZENABLE,			FALSE ),
		KRenderState( D3DRS_DESTBLEND,			D3DBLEND_ONE ),
		KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
		KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE ),
		KTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_DIFFUSE ),
		KTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_TEXTURE ),
};//s_akStates[]
//}} robobeg : 2008-10-13


CKTDGLensFlare::CKTDGLensFlare( HWND hWnd,
							   WCHAR* pHaloTex,
							   WCHAR* pFlareTex1,
							   WCHAR* pFlareTex2,
							   WCHAR* pFlareTex3 )
{
	m_hWnd			= hWnd;

	m_LightPosition = D3DXVECTOR3(0,0,0);
	m_vScreen		= D3DXVECTOR3(0,0,0);

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	m_pVBSpots		= NULL;
//	m_iVBSize		= 0;
//#endif

	SetIntensity(CMinMax<float>(0.0f, 1.0f));
	SetIntensityBorder(300);

	m_pTexHalo	= g_pKTDXApp->GetDeviceManager()->OpenTexture( pHaloTex );
	m_pTex1		= g_pKTDXApp->GetDeviceManager()->OpenTexture( pFlareTex1 );
	m_pTex2		= g_pKTDXApp->GetDeviceManager()->OpenTexture( pFlareTex2 );
	m_pTex3		= g_pKTDXApp->GetDeviceManager()->OpenTexture( pFlareTex3 );

//{{ robobeg : 2008-10-13
    SetRenderStateID( s_akStates );
//}} robobeg : 2008-10-13

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	AddSpot(CLensFlareSpot(m_pTexHalo, 0.40f,  1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
	AddSpot(CLensFlareSpot(m_pTex2, 0.5f,		0.8f, D3DXCOLOR(0.7f, 0.5f, 0.0f, 0.2f)));
	AddSpot(CLensFlareSpot(m_pTex2, 0.005f,		0.7f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f)));
	AddSpot(CLensFlareSpot(m_pTex1, 0.025f,		0.6f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f)));
	AddSpot(CLensFlareSpot(m_pTex3, 0.05f,		0.5f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f)));
	AddSpot(CLensFlareSpot(m_pTex1, 0.025f,		0.4f, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f)));
	AddSpot(CLensFlareSpot(m_pTex2, 0.025f,		0.1f, D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f)));
	AddSpot(CLensFlareSpot(m_pTex1, 0.025f,		-0.2f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));
	AddSpot(CLensFlareSpot(m_pTex2, 0.045f,		-0.3f, D3DXCOLOR(1.0f, 1.0f, 0.6f, 0.5f)));
	AddSpot(CLensFlareSpot(m_pTex1, 0.07f,		-0.4f, D3DXCOLOR(1.0f, 0.7f, 0.0f, 0.3f)));
	AddSpot(CLensFlareSpot(m_pTex3, 0.07f,		-0.7f, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.2f)));
	AddSpot(CLensFlareSpot(m_pTex2, 0.16f,		-1.0f, D3DXCOLOR(1.0f, 0.7f, 0.0f, 0.4f)));
	AddSpot(CLensFlareSpot(m_pTex3, 0.20f,		-1.3f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f)));
//#else
//	OnResetDevice();
//#endif
}

CKTDGLensFlare::~CKTDGLensFlare(void)
{
	SAFE_CLOSE( m_pTexHalo );
	SAFE_CLOSE( m_pTex1 );
	SAFE_CLOSE( m_pTex2 );
	SAFE_CLOSE( m_pTex3 );

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	SAFE_RELEASE( m_pVBSpots );
//#endif

	m_Spots.clear();
}

//{{ robobeg : 2008-10-28
//virtual HRESULT CKTDGLensFlare::OnFrameRender();
/*virtual*/
RENDER_HINT CKTDGLensFlare::OnFrameRender_Prepare()
{
	CalcLightSourceScreenCoords();

	if( m_vScreen.z < 0.0f )
        return RENDER_HINT_NORENDER;

	RECT WindowRect;
	GetClientRect( m_hWnd, &WindowRect );
	int winWidth	= WindowRect.right - WindowRect.left;
	int winHeight	= WindowRect.bottom - WindowRect.top;

	// calculate actual intensity based on the given intensity and how far
	// from the edge of the screen the sun is.

	float fRealIntensity;
	int iAwayX, iAwayY;
	float fAway;

	if( m_vScreen.x < 0.0f )
	{
		iAwayX =  (int)-m_vScreen.x;
	}
	else
	{
		iAwayX = 0;
	}
	if( m_vScreen.y < 0.0f )
	{
		iAwayY =  (int)-m_vScreen.y;
	}
	else
	{
		iAwayY = 0;
	}
	if( iAwayX > iAwayY )
	{
		fAway = (float)iAwayX;
	}
	else
	{
		fAway = (float)iAwayY;
	}

	if (fAway > m_fIntensityBorder) 
		fAway = m_fIntensityBorder;

	fRealIntensity = 1.0f - (fAway / m_fIntensityBorder);

	fRealIntensity = m_fIntensity.m_Min + (fRealIntensity*(m_fIntensity.GetRange()));

	// calculate spot positions and fill VB
	{
		int iCenterOfScreenX = winWidth/2;
		int iCenterOfScreenY = winHeight/2;

		int iDistanceX = iCenterOfScreenX - (int)m_vScreen.x;
		int iDistanceY = iCenterOfScreenY - (int)m_vScreen.y;

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		VERTEX_LENSFLARE *pVertices = &m_vertices[0];
//#else
//		// lock the vertex buffer
//		VERTEX_LENSFLARE *pVertices;
//
//		if(FAILED(m_pVBSpots->Lock( 0, m_iVBSize*6*sizeof(VERTEX_LENSFLARE), (void**)&pVertices, 0)))
//            return RENDER_HINT_NORENDER;
//#endif

		// for each spot in this flare...
		for (std::vector<CLensFlareSpot>::iterator i = m_Spots.begin(); i != m_Spots.end(); i++) 
		{
			CLensFlareSpot &spot = (*i);

			// calculate this spot's center position
			int iSpotCenterPosX = iCenterOfScreenX - (int)((float)iDistanceX * spot.GetLinePos());
			int iSpotCenterPosY = iCenterOfScreenY - (int)((float)iDistanceY * spot.GetLinePos());
			int iSizeDiv2 = (int)((float)winWidth * spot.GetSize()/2.0f);

			D3DXCOLOR color = spot.GetColor();
			color.a *= fRealIntensity;
			if (color.a > 1.0f)
				color.a = 1.0f;
			if (color.a < 0.0f)
				color.a = 0.0f;
			//if (bFirstOnly && i != m_Spots.begin())
			//	color.a = 0.0f;

			// first triangle
			pVertices->color = color;

			pVertices->position = D3DXVECTOR3((float)iSpotCenterPosX-iSizeDiv2, (float)iSpotCenterPosY-iSizeDiv2, 0.0f);
			pVertices->tu = 0.0f; pVertices->tv = 0.0f;	pVertices->rhw = 1.0f;
			pVertices++;

			pVertices->color = color;
			pVertices->position = D3DXVECTOR3((float)iSpotCenterPosX+iSizeDiv2, (float)iSpotCenterPosY-iSizeDiv2, 0.0f);
			pVertices->tu = 1.0f; pVertices->tv = 0.0f; pVertices->rhw = 1.0f;
			pVertices++;

			pVertices->color = color;
			pVertices->position = D3DXVECTOR3((float)iSpotCenterPosX+iSizeDiv2, (float)iSpotCenterPosY+iSizeDiv2, 0.0f);
			pVertices->tu = 1.0f; pVertices->tv = 1.0f; pVertices->rhw = 1.0f;
			pVertices++;


			// second triangle
			pVertices->color = color;
			pVertices->position = D3DXVECTOR3((float)iSpotCenterPosX-iSizeDiv2, (float)iSpotCenterPosY-iSizeDiv2, 0.0f);
			pVertices->tu = 0.0f; pVertices->tv = 0.0f; pVertices->rhw = 1.0f;
			pVertices++;

			pVertices->color = color;
			pVertices->position = D3DXVECTOR3((float)iSpotCenterPosX+iSizeDiv2, (float)iSpotCenterPosY+iSizeDiv2, 0.0f);
			pVertices->tu = 1.0f; pVertices->tv = 1.0f; pVertices->rhw = 1.0f;
			pVertices++;

			pVertices->color = color;
			pVertices->position = D3DXVECTOR3((float)iSpotCenterPosX-iSizeDiv2, (float)iSpotCenterPosY+iSizeDiv2, 0.0f);
			pVertices->tu = 0.0f; pVertices->tv = 1.0f; pVertices->rhw = 1.0f;
			pVertices++;
		} // next spot

		
//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		// unlock VB
//		m_pVBSpots->Unlock();
//#endif
	}

    return RENDER_HINT_DEFAULT;
}//CKTDGLensFlare::OnFrameRender_Prepare()


/*virtual*/
void            CKTDGLensFlare::OnFrameRender_Draw()
{
    KD3DPUSH( GetRenderStateID() )

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		BOOST_STATIC_ASSERT( D3DFVF_LENSFLARE == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
		for( int q = 0; q < (int) m_Spots.size(); q++ )
		{
			m_Spots[q].GetTexture()->SetDeviceTexture();
			g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
				, D3DPT_TRIANGLELIST, 2, &m_vertices[ q * 6 ] );
		}//for
//#else
//	    // set custom vertex shader
//	    g_pKTDXApp->GetDevice()->SetStreamSource(0, m_pVBSpots, 0, sizeof(VERTEX_LENSFLARE));
//
//	    g_pKTDXApp->GetDevice()->SetFVF(D3DFVF_LENSFLARE);
//
//
//	    // this isn't the fastest way to do things, but it's easy to understand.
//	    // optimization left as an exercise for the reader :)
//	    for( int q = 0; q < (int)m_Spots.size(); q++ ) 
//	    {
//		    m_Spots[q].GetTexture()->SetDeviceTexture();
//		    g_pKTDXApp->GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, q*6, 2);
//
//	    }
//#endif

    KD3DEND()
}//CKTDGLensFlare::OnFrameRender_Draw()
//}} robobeg : 2008-10-28






//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//HRESULT CKTDGLensFlare::OnResetDevice()
//{
//	m_pVBSpots = NULL;
//	m_iVBSize = 1;
//
//	AddSpot(CLensFlareSpot(m_pTexHalo, 0.40f,  1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
//
//	AddSpot(CLensFlareSpot(m_pTex2, 0.5f,		0.8f, D3DXCOLOR(0.7f, 0.5f, 0.0f, 0.2f)));
//	AddSpot(CLensFlareSpot(m_pTex2, 0.005f,		0.7f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f)));
//	AddSpot(CLensFlareSpot(m_pTex1, 0.025f,		0.6f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f)));
//	AddSpot(CLensFlareSpot(m_pTex3, 0.05f,		0.5f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f)));
//	AddSpot(CLensFlareSpot(m_pTex1, 0.025f,		0.4f, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f)));
//	AddSpot(CLensFlareSpot(m_pTex2, 0.025f,		0.1f, D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f)));
//	AddSpot(CLensFlareSpot(m_pTex1, 0.025f,		-0.2f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));
//	AddSpot(CLensFlareSpot(m_pTex2, 0.045f,		-0.3f, D3DXCOLOR(1.0f, 1.0f, 0.6f, 0.5f)));
//	AddSpot(CLensFlareSpot(m_pTex1, 0.07f,		-0.4f, D3DXCOLOR(1.0f, 0.7f, 0.0f, 0.3f)));
//	AddSpot(CLensFlareSpot(m_pTex3, 0.07f,		-0.7f, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.2f)));
//	AddSpot(CLensFlareSpot(m_pTex2, 0.16f,		-1.0f, D3DXCOLOR(1.0f, 0.7f, 0.0f, 0.4f)));
//	AddSpot(CLensFlareSpot(m_pTex3, 0.20f,		-1.3f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f)));
//
//	return RecreateVB();
//}
//
//HRESULT CKTDGLensFlare::OnLostDevice()
//{
//	return S_OK;
//}
//
//HRESULT CKTDGLensFlare::RecreateVB()
//{
//	HRESULT hr = S_OK;
//
//	SAFE_RELEASE(m_pVBSpots);
//
//	if (m_iVBSize) 
//	{
//		hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer(m_iVBSize*sizeof(VERTEX_LENSFLARE)*6,	0, 
//			D3DFVF_LENSFLARE, D3DPOOL_SYSTEMMEM, &m_pVBSpots, NULL);
//
//		if (FAILED(hr)) 
//		{
//			ErrorLog( KEM_ERROR18 );
//			m_pVBSpots = NULL;
//		}
//	}
//	return hr;
//}
//#endif

void CKTDGLensFlare::AddSpot( CLensFlareSpot &spot )
{
	m_Spots.push_back(spot);
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	m_vertices.resize( m_Spots.size() * 6 );
//#else
//	m_iVBSize = (int)m_Spots.size();
//#endif
}

void CKTDGLensFlare::CalcLightSourceScreenCoords()
{
	RECT WindowRect;
	GetClientRect( m_hWnd, &WindowRect );
	int winWidth	= WindowRect.right - WindowRect.left;
	int winHeight	= WindowRect.bottom - WindowRect.top;

	D3DXMATRIX matWorld, matProj, matView, matConcat, matViewportScale;
	D3DXVECTOR4 vResult;

	matViewportScale = D3DXMATRIX( (float)winWidth/2.0f,	0.0f,					0.0f, 0.0f,
									0.0f,					(float)-winHeight/2.0f,	0.0f, 0.0f,
									0.0f,					0.0f,					1.0f, 0.0f,
									(float)winWidth/2.0f,	(float)winHeight/2.0f,	0.0f, 1.0f );

	g_pKTDXApp->GetViewTransform( &matView );
	g_pKTDXApp->GetProjectionTransform( &matProj );

	D3DXMatrixIdentity(&matWorld); // no need for a world xform

	matConcat = matWorld;
	matConcat *= matView;
	matConcat *= matProj;
	matConcat *= matViewportScale;

	D3DXVec3Transform(&vResult, &m_LightPosition, &matConcat);

	m_vScreen.x = vResult.x/vResult.w;
	m_vScreen.y = vResult.y/vResult.w;
	m_vScreen.z = vResult.w;
}




CKTDGLensFlare::CLensFlareSpot::CLensFlareSpot()
{
	m_pTexture	= NULL;
	m_fSize		= 1.0f;
	m_fLinePos	= 0.0f;
	m_Color		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

CKTDGLensFlare::CLensFlareSpot::CLensFlareSpot(CKTDXDeviceTexture* pTex, float fSize, 
											   float fLinePos, D3DXCOLOR Color)
{
	m_pTexture	= pTex;
	m_fSize		= fSize;
	m_fLinePos	= fLinePos;
	m_Color		= Color;	
}

CKTDGLensFlare::CLensFlareSpot::~CLensFlareSpot()
{
}
