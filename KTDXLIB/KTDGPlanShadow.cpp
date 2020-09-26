#include "StdAfx.h"
#include ".\ktdgplanshadow.h"

CKTDGPlanShadow::CKTDGPlanShadow( float fSize )
{
	SetAlphaObject( true );

	m_pShadowTexture	= NULL;
	m_Position			= D3DXVECTOR3(0,0,0);
	m_LandPosition		= D3DXVECTOR3(0,0,0);
	m_Rotate			= D3DXVECTOR3(0,0,0);
	m_fSize				= fSize;
	m_fSizeFactor		= 1.0f;
	m_fAlpha			= 0.8f;

	SetRenderStateID( g_pKTDXApp->GetDGManager()->GetRSICKTDGPlanShadow() );

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	m_pVB = NULL;
//	HRESULT hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( 4 * sizeof(UNIT_SHADOW_VERTEX), 
//		D3DUSAGE_WRITEONLY, D3DFVF_UNIT_SHADOW_VERTEX, 
//		D3DPOOL_MANAGED, &m_pVB, NULL );
//	ASSERT( SUCCEEDED( hr ) );
//	m_bVBToBeUpdated = true;
//#endif


#ifdef X2OPTIMIZE_NPC_SHADOW_CULLING
	//반지름을 대략 계산한다.
	SetBoundingRadius( ( ( m_fSize * m_fSizeFactor ) / 2.0f ) * 1.5f );
#endif//X2OPTIMIZE_NPC_SHADOW_CULLING	
}

CKTDGPlanShadow::~CKTDGPlanShadow(void)
{
	SAFE_CLOSE( m_pShadowTexture );

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	SAFE_RELEASE( m_pVB );
//#endif
}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
RENDER_HINT CKTDGPlanShadow::OnFrameRender_Prepare()
{
	if ( m_pShadowTexture == NULL )
		return RENDER_HINT_NORENDER;

	return RENDER_HINT_DEFAULT;
}//CKTDGPlanShadow::OnFrameRender_Prepare()
//#endif

//{{ robobeg : 2008-10-28
/*virtual*/
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
/*virtual*/
void    CKTDGPlanShadow::OnFrameRender_Draw()
{
	GetMatrix().UpdateWorldMatrix();

	if ( m_pShadowTexture == NULL )
		return;

	UNIT_SHADOW_VERTEX vertex[4];

	//Set up the 4 corners of a small square
	vertex[0].x = -(m_fSize/2.0f) * m_fSizeFactor;     
	vertex[0].y = 2.0f;
	vertex[0].z = -(m_fSize/2.0f) * m_fSizeFactor;
	vertex[0].u = 0.0f;   
	vertex[0].v = 0.0f;

	vertex[1].x = -(m_fSize/2.0f) * m_fSizeFactor;     
	vertex[1].y = 2.0f;
	vertex[1].z = (m_fSize/2.0f) * m_fSizeFactor;
	vertex[1].u = 0.0f;   
	vertex[1].v = 1.0f;

	vertex[2].x = (m_fSize/2.0f) * m_fSizeFactor;     
	vertex[2].y = 2.0f;
	vertex[2].z = -(m_fSize/2.0f) * m_fSizeFactor;
	vertex[2].u = 1.0f;   
	vertex[2].v = 0.0f;	

	vertex[3].x = (m_fSize/2.0f) * m_fSizeFactor;     
	vertex[3].y = 2.0f;
	vertex[3].z = (m_fSize/2.0f) * m_fSizeFactor;
	vertex[3].u = 1.0f;   
	vertex[3].v = 1.0f;

	D3DXCOLOR color = 0xffffffff;
	color.a = m_fAlpha;

	vertex[0].color = vertex[1].color = 
		vertex[2].color = vertex[3].color = color;

	m_pShadowTexture->SetDeviceTexture();

//#ifdef  DYNAMIC_VERTEX_BUFFER_OPT

	BOOST_STATIC_ASSERT( D3DFVF_XYZ_DIFFUSE_TEX1 == D3DFVF_UNIT_SHADOW_VERTEX );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, vertex );

//#else   DYNAMIC_VERTEX_BUFFER_OPT
//
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_UNIT_SHADOW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(UNIT_SHADOW_VERTEX) );
//
//#endif  DYNAMIC_VERTEX_BUFFER_OPT


}
//#else
//void    CKTDGPlanShadow::OnFrameRender_Draw()
//{
//	GetMatrix().UpdateWorldMatrix();
//
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
//	if ( m_bVBToBeUpdated == true )
//	{
//		m_bVBToBeUpdated = false;
//#endif
//
//		UNIT_SHADOW_VERTEX vertex[4];
//		ZeroMemory( vertex, sizeof(UNIT_SHADOW_VERTEX) * 4 );
//
//		//Set up the 4 corners of a small square
//		vertex[0].x = -(m_fSize/2.0f) * m_fSizeFactor;     
//		vertex[0].y = 2.0f;
//		vertex[0].z = -(m_fSize/2.0f) * m_fSizeFactor;
//		vertex[0].u = 0.0f;   
//		vertex[0].v = 0.0f;
//
//		vertex[1].x = -(m_fSize/2.0f) * m_fSizeFactor;     
//		vertex[1].y = 2.0f;
//		vertex[1].z = (m_fSize/2.0f) * m_fSizeFactor;
//		vertex[1].u = 0.0f;   
//		vertex[1].v = 1.0f;
//
//		vertex[2].x = (m_fSize/2.0f) * m_fSizeFactor;     
//		vertex[2].y = 2.0f;
//		vertex[2].z = -(m_fSize/2.0f) * m_fSizeFactor;
//		vertex[2].u = 1.0f;   
//		vertex[2].v = 0.0f;	
//
//		vertex[3].x = (m_fSize/2.0f) * m_fSizeFactor;     
//		vertex[3].y = 2.0f;
//		vertex[3].z = (m_fSize/2.0f) * m_fSizeFactor;
//		vertex[3].u = 1.0f;   
//		vertex[3].v = 1.0f;
//	
//		D3DXCOLOR color = 0xffffffff;
//		color.a = m_fAlpha;
//
//		vertex[0].color = vertex[1].color = 
//			vertex[2].color = vertex[3].color = color;
//
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
//		if ( m_pVB != NULL )
//		{
//			UNIT_SHADOW_VERTEX* pLockedVertex = NULL;
//			HRESULT hr = m_pVB->Lock( 0, 4 * sizeof(UNIT_SHADOW_VERTEX), (void**) &pLockedVertex, 0L );
//			if ( SUCCEEDED( hr ) )
//			{
//				CopyMemory( pLockedVertex, vertex, 4 * sizeof(UNIT_SHADOW_VERTEX) );
//				hr = m_pVB->Unlock();
//				ASSERT( SUCCEEDED( hr ) );
//			}
//		}//if
//	}
//#endif
//
//		m_pShadowTexture->SetDeviceTexture();
//		g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_UNIT_SHADOW_VERTEX );
//
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
//		if ( m_pVB != NULL )
//		{
//			g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pVB, 0, sizeof(UNIT_SHADOW_VERTEX) );
//			g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
//		}//if
//#else
//		g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(UNIT_SHADOW_VERTEX) );
//#endif
//
//}//CKTDGPlanShadow::OnFrameRender_Draw()
//#endif
//}} robobeg : 2008-10-28

void CKTDGPlanShadow::SetShadowTexture( wstring tex )
{
	m_pShadowTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( tex );
}

void CKTDGPlanShadow::UpdatePosition( D3DXVECTOR3 pos, D3DXVECTOR3 landPos, D3DXVECTOR3 rotate )
{
	KTDXPROFILE();

	m_Position		= pos;
	m_LandPosition	= landPos;
	m_Rotate		= rotate;

	GetMatrix().Move( m_LandPosition );
	GetMatrix().RotateDegree( m_Rotate );

	float fDistance = GetDistance( m_Position, m_LandPosition );
	m_fSizeFactor	= 1.0f - (fDistance/300.0f);
	m_fAlpha		= 0.8f - (fDistance/200.0f);
}




