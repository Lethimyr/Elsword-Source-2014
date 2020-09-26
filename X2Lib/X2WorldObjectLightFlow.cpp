#include "stdafx.h"
#include ".\x2worldobjectlightflow.h"


CX2WorldObjectLightFlow::~CX2WorldObjectLightFlow(void)
{
	SAFE_CLOSE( m_pXMesh );
}

CX2WorldObjectLightFlow::CX2WorldObjectLightFlow( bool bBackgroundLoad_ )
	: CX2WorldObject( bBackgroundLoad_ ),
	m_bXMeshLoaded( false )
{
	m_ObjectType		= CX2WorldObject::OT_LIGHT_FLOW;

	m_XMeshName			= L"";
	m_pXMesh			= NULL;

	m_BillboardType		= CKTDGMatrix::BT_NONE;
	m_RenderParam.bAlphaBlend = true;
	m_RenderParam.renderType = CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Y;
	//INIT_COLOR( m_RenderParam.color, 1, 1, 1, 1 );
	INIT_COLOR( m_coColor, 1, 1, 1, 1 );
	INIT_VECTOR3( m_LightPos, 1000, 1000, 1000 );

	m_fFlowMin		= 0.f;
	m_fFlowMax		= 0.f;
	m_fFlowSpeed	= 0.f;
	m_fNowPoint		= 0.f;
	m_fWide			= 0.f;
}


HRESULT CX2WorldObjectLightFlow::OnFrameMove( double fTime, float fElapsedTime )
{	
	KTDXPROFILE();

	HRESULT hr = S_OK;

	if ( false == m_bXMeshLoaded && NULL != m_pXMesh )
	{
		CKTDXDevice::EDeviceState eState = m_pXMesh->GetDeviceState();
		if ( eState == CKTDXDevice::DEVICE_STATE_LOADED )
		{
			SetCenter( m_pXMesh->GetCenter() );
			SetBoundingRadius( m_pXMesh->GetRadius() );
			m_bXMeshLoaded = true;
		}
		else if ( eState == CKTDXDevice::DEVICE_STATE_FAILED )
		{
			SAFE_CLOSE( m_pXMesh );
		}//if.. else..
	}

	m_fNowPoint += m_fFlowSpeed * fElapsedTime; 

	if( m_fNowPoint < m_fFlowMin )
		m_fNowPoint = m_fFlowMax;
	else if( m_fNowPoint > m_fFlowMax )
		m_fNowPoint = m_fFlowMin;

	return hr;
}


//{{ robobeg : 2008-10-15
/*virtual*/
RENDER_HINT   CX2WorldObjectLightFlow::OnFrameRender_Prepare() 
{ 
    //__super::SetLastAccessTime( g_NowTime );

    if ( NULL == m_pXMesh || false == m_bXMeshLoaded )
        return RENDER_HINT_NORENDER;

    return RENDER_HINT_XMESH( m_RenderParam.renderType, m_RenderParam.bFog && m_RenderParam.bFogShow );
}//OnFrameRender_Prepare()


//HRESULT CX2WorldObjectLightFlow::OnFrameRender()
/*virtual*/
void    CX2WorldObjectLightFlow::OnFrameRender_Draw()
{
	KTDXPROFILE();
    //__super::SetLastAccessTime( g_NowTime );

	if ( false == m_bXMeshLoaded )
		return;
	
	//랜더 파라메터 세팅
//{{ robobeg : 2008-10-15
	//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	//renderParam				= m_RenderParam;
    CKTDGXRenderer::RenderParam renderParam( m_RenderParam );
//}} robobeg : 2008-10-15
	renderParam.color.r		= m_coColor.r * m_RenderParam.color.r;
	renderParam.color.g		= m_coColor.g * m_RenderParam.color.g;
	renderParam.color.b		= m_coColor.b * m_RenderParam.color.b;
	renderParam.color.a		= m_coColor.a * m_RenderParam.color.a;
//{{ robobeg : 2008-10-24
	//renderParam.worldMatrix	= GetMatrix().GetMatrix( m_BillboardType ); 
    D3DXMATRIX  worldMatrix = GetMatrix().GetMatrix( m_BillboardType ); 
//}} robobeg : 2008-10-24
	renderParam.lightPos		= m_LightPos;
	renderParam.fLightFlowImpact	= m_fNowPoint;
	renderParam.fLightFlowWide	= m_fWide;	

	//랜더

    g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMesh, NULL, NULL );

}
//}} robobeg : 2008-10-15


void CX2WorldObjectLightFlow::SetXMesh_LUA( const char* pFileName )
{
	SAFE_CLOSE( m_pXMesh );
	ConvertUtf8ToWCHAR( m_XMeshName, pFileName );

	m_bXMeshLoaded = false;

	if ( IsBackgroundLoad() == true )
		m_pXMesh	= g_pKTDXApp->GetDeviceManager()->OpenXMeshInBackground( m_XMeshName.c_str() );
	else
	{
		m_pXMesh	= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_XMeshName.c_str() );
		m_bXMeshLoaded = true;
	}//if.. else..

	if( NULL == m_pXMesh )
	{
		ErrorLogMsg( XEM_ERROR108, pFileName );
	}
	else if ( true == m_bXMeshLoaded )
	{
		SetCenter( m_pXMesh->GetCenter() );
		SetBoundingRadius( m_pXMesh->GetRadius() );
	}
}


/*
CX2WorldObjectLightFlow::CX2WorldObjectLightFlow( LightFlowObjectData* pLightFlowObjectData )
						:CX2WorldObject( pLightFlowObjectData )
{	
	m_pLightFlowObjectData	= pLightFlowObjectData;
	m_pXMesh				= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_pLightFlowObjectData->name );
	m_pRenderer				= g_pKTDXApp->GetDGManager()->GetXLightFlowRenderer();
	m_fNowPoint				= m_pLightFlowObjectData->fFlowMin;

	SetCenter( m_pXMesh->GetBoundingCenter() );
	SetBoundingRadius( m_pXMesh->GetBoundingSphere() );
}


HRESULT CX2WorldObjectLightFlow::OnFrameMove( double fTime, float fElapsedTime )
{	
	m_fNowPoint += m_pLightFlowObjectData->fFlowSpeed * fElapsedTime;

	if( m_fNowPoint < m_pLightFlowObjectData->fFlowMin )
		m_fNowPoint = m_pLightFlowObjectData->fFlowMax;
	else if( m_fNowPoint > m_pLightFlowObjectData->fFlowMax )
		m_fNowPoint = m_pLightFlowObjectData->fFlowMin;

	return S_OK;
}

HRESULT CX2WorldObjectLightFlow::OnFrameRender()
{
	m_pRenderer->SetRenderType( m_pLightFlowObjectData->renderType );
	m_pRenderer->SetPoint( m_fNowPoint );
	m_pRenderer->SetColor( m_pLightFlowObjectData->color );
	m_pRenderer->SetWide( m_pLightFlowObjectData->fWide );
	m_pRenderer->SetDXMatrix( GetMatrix().GetMatrix( m_pLightFlowObjectData->billboardType ) );

	m_pRenderer->BeginRender();
	m_pRenderer->OnFrameRender( m_pXMesh );
	m_pRenderer->EndRender();

	return S_OK;
}
*/
