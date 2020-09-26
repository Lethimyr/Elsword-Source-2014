#include "StdAfx.h"
#include ".\ktdgskydome.h"

CKTDGSkyDome::CKTDGSkyDome()
{
	m_RenderParam.renderType	= CKTDGXRenderer::RT_REAL_COLOR;
	SetForceLayer( true );

	INIT_VECTOR2( m_Tex0UVSpeed, 0.0f, 0.0f );
	INIT_VECTOR2( m_Tex1UVSpeed, 0.0f, 0.0f );
	INIT_VECTOR2( m_Tex2UVSpeed, 0.0f, 0.0f );

	m_fRotationSpeed	= 0.0f;
	m_pDomeMesh			= NULL;
	m_pMultiTexXET		= NULL;
	m_pChangeTexXET		= NULL;
	m_bViewTrace		= true;
	m_bViewTraceXZ		= false;

	m_bViewRotation		= true;

	m_bUseWorldColor	= true;
	m_vColorFactor		= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_vSkyDomeColorFactor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

#ifdef X2TOOL
	m_bUseObjectFogProperty = false;	
	m_bSelected = false;

	m_bBackupRenderParam = false;
	m_bShowObject = true;
#endif
}

CKTDGSkyDome::~CKTDGSkyDome(void)
{
	SAFE_CLOSE( m_pDomeMesh );
	SAFE_CLOSE( m_pMultiTexXET );
	SAFE_CLOSE( m_pChangeTexXET );
}

HRESULT CKTDGSkyDome::OnFrameMove( double fTime, float fElapsedTime )
{
	GetMatrix().RotateRel( 0.0f, D3DXToRadian(m_fRotationSpeed * fElapsedTime), 0.0f );
	m_RenderParam.texOffsetStage0 += (m_Tex0UVSpeed * fElapsedTime);
	m_RenderParam.texOffsetStage1 += (m_Tex1UVSpeed * fElapsedTime);
	m_RenderParam.texOffsetStage2 += (m_Tex2UVSpeed * fElapsedTime);

	SetAlphaObject( m_RenderParam.bAlphaBlend );
	return S_OK;
}

//{{ robobeg : 2008-10-24

/*virtual*/
RENDER_HINT   CKTDGSkyDome::OnFrameRender_Prepare()
{
	if( m_pDomeMesh == NULL )
		return RENDER_HINT_NORENDER;

    return RENDER_HINT_XMESH( m_RenderParam.renderType, m_RenderParam.bFog && m_RenderParam.bFogShow );
}//CKTDGSkyDome::OnFrameRender_Prepare()


/*virtual*/
void    CKTDGSkyDome::OnFrameRender_Draw()
{
	KTDXPROFILE();
	if( m_pDomeMesh == NULL )
		return;

	GetMatrix().UpdateWorldMatrix();
	
	D3DXMATRIX matOrgView;
	g_pKTDXApp->GetViewTransform( &matOrgView );
	D3DXMATRIX matSkyView = matOrgView;

	if( false == m_bViewRotation )
	{
		D3DXMatrixIdentity( &matSkyView );
		matSkyView._41 = matOrgView._41;
		matSkyView._42 = matOrgView._42;
		matSkyView._43 = matOrgView._43;

		g_pKTDXApp->SetViewTransform( &matSkyView );
	}

	if( m_bViewTrace == true )
	{		
		matSkyView._41 = matSkyView._42 = matSkyView._43 = 0.0f;

		g_pKTDXApp->SetViewTransform( &matSkyView );
	}
	else if( true == m_bViewTraceXZ )
	{
		matSkyView._41 = matSkyView._43 = 0.0f;

		g_pKTDXApp->SetViewTransform( &matSkyView );
	}
	
    g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_RenderParam, GetMatrix().GetMatrix(), *m_pDomeMesh );

	if( m_bViewTrace == true || true == m_bViewTraceXZ || false == m_bViewRotation )
	{
		g_pKTDXApp->SetViewTransform( &matOrgView );
	}
}

//}} robobeg : 2008-10-24

#ifdef X2TOOL
void CKTDGSkyDome::SetDomeMeshW(wstring wstrMeshName)
{
	SAFE_CLOSE( m_pDomeMesh );
	m_pDomeMesh	= g_pKTDXApp->GetDeviceManager()->OpenXMesh( wstrMeshName );
}
#endif

void CKTDGSkyDome::SetDomeMesh_LUA( const char* pMeshName )
{
	SAFE_CLOSE( m_pDomeMesh );
	wstring strBuf;
	ConvertUtf8ToWCHAR( strBuf, pMeshName );
	m_pDomeMesh	= g_pKTDXApp->GetDeviceManager()->OpenXMesh( strBuf );
}

void CKTDGSkyDome::SetMultiTexXET_LUA( const char* pFileName )
{
	SAFE_CLOSE( m_pMultiTexXET );
	wstring strBuf;
	ConvertUtf8ToWCHAR( strBuf, pFileName );
	m_pMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( strBuf );
}

void CKTDGSkyDome::SetChangeTexXET_LUA( const char* pFileName )
{
	SAFE_CLOSE( m_pChangeTexXET );
	wstring strBuf;
	ConvertUtf8ToWCHAR( strBuf, pFileName );
	m_pChangeTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( strBuf );
}

#ifdef FOG_WORLD
void CKTDGSkyDome::SetFog(float xNear, float xFar, float yNear, float yFar, D3DXCOLOR color, float density)
{
	m_RenderParam.bFog          = true;
	m_RenderParam.fogColor      = color;
	m_RenderParam.fogDensity    = density;
	m_RenderParam.fogFarX       = xFar;
	m_RenderParam.fogFarY       = yFar;
	m_RenderParam.fogNearX      = xNear;
	m_RenderParam.fogNearY      = yNear;
}
#endif

