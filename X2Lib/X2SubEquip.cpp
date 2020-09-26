#include "StdAfx.h"
#include ".\x2subequip.h"

CX2SubEquip::CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName ) :
CKTDGObject( NULL ),
m_pFrameMatrix( pFrameMatrix )
{
	ASSERT( NULL != pFrameMatrix );
	m_pNormalXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( modelName );

	m_fScale = 1.f;
	m_vRotate = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vOffset = D3DXVECTOR3(0.f, 0.f, 0.f);

#ifdef ATTACH_SKINMESH
	m_pSkinAnim = NULL;
	m_pSkinModel = NULL;
	m_bSkinMesh = false;
#endif
}

#ifdef ATTACH_SKINMESH
CX2SubEquip::CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin ) :
CKTDGObject( NULL ),
m_pFrameMatrix( pFrameMatrix )
{
	ASSERT( NULL != pFrameMatrix );

	m_bSkinMesh = bSkin;

	m_pSkinAnim = NULL;
	m_pSkinModel = NULL;
	m_pNormalXMesh = NULL;	
	
	
	if( m_bSkinMesh == false || 
		'y' == modelName.back() || 
		'Y' == modelName.back() )
	{
		m_bSkinMesh = false;
		m_pNormalXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( modelName );	
	}
	else
	{
		m_pSkinModel = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( modelName );
		if( m_pSkinModel != NULL )
		{
			m_pSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
			m_pSkinAnim->SetAnimXSkinMesh( m_pSkinModel );
			m_pSkinAnim->AddModelXSkinMesh( m_pSkinModel );
			m_pSkinAnim->ChangeAnim( L"Normal", false );
			m_pSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
			m_pSkinAnim->UseDXMatrix( true );
		}		
	}

	m_fScale = 1.f;
	m_vRotate = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vOffset = D3DXVECTOR3(0.f, 0.f, 0.f);
}
#endif

CX2SubEquip::~CX2SubEquip(void)
{
	SetManagerObject( NULL );

	SAFE_CLOSE( m_pNormalXMesh );
#ifdef ATTACH_SKINMESH
	SAFE_CLOSE( m_pSkinModel );
	SAFE_DELETE_KTDGOBJECT( m_pSkinAnim );
#endif
}

/*virtual*/ RENDER_HINT CX2SubEquip::OnFrameRender_Prepare()
{
#ifdef ATTACH_SKINMESH
	if( m_bSkinMesh == false )
	{
		if( NULL == m_pNormalXMesh )
			return RENDER_HINT_NORENDER;

		return RENDER_HINT_XMESH( m_NormalRenderParam.renderType, m_NormalRenderParam.bFog && m_NormalRenderParam.bFogShow );
	}
	else
	{
		if( m_pSkinAnim != NULL )
		{				
			return m_pSkinAnim->OnFrameRender_Prepare();
		}
		else
		{
			return RENDER_HINT_NORENDER;
		}
	}	
#else
	if( NULL == m_pNormalXMesh )
		return RENDER_HINT_NORENDER;

	return RENDER_HINT_XMESH( m_NormalRenderParam.renderType, m_NormalRenderParam.bFog && m_NormalRenderParam.bFogShow );
#endif
}

/*virtual*/ void CX2SubEquip::OnFrameRender_Draw()
{
#ifdef ATTACH_SKINMESH
	if( m_bSkinMesh == false )
	{
		if( NULL != m_pNormalXMesh )
		{	
			D3DXMATRIX localMove, matTrans, matRot, matScale, matWorld;
			
			D3DXMatrixIdentity( &matWorld );

			D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale );
			D3DXMatrixRotationYawPitchRoll( &matRot, m_vRotate.y, m_vRotate.x, m_vRotate.z );

			D3DXMatrixTranslation( &matTrans, m_vOffset.x, m_vOffset.y, m_vOffset.z );

			D3DXMatrixMultiply( &localMove, &matScale, &matRot );
			D3DXMatrixMultiply( &localMove, &localMove, &matTrans );
			
			D3DXMatrixMultiply( &matWorld, &localMove, m_pFrameMatrix );

			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_NormalRenderParam, matWorld, *m_pNormalXMesh, NULL, NULL, NULL );
		}
	}
	else
	{
		if( m_pSkinAnim != NULL)
		{			
			m_pSkinAnim->OnFrameRender_Draw();
		}
	}
#else
	if( NULL != m_pNormalXMesh )
	{
		g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_NormalRenderParam, *m_pFrameMatrix, *m_pNormalXMesh, NULL, NULL, NULL );
	}
#endif
}

#ifdef ATTACH_SKINMESH
HRESULT CX2SubEquip::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_bSkinMesh == true && m_pSkinAnim != NULL )
	{
		m_pSkinAnim->SetDXMatrix( *m_pFrameMatrix );
		m_pSkinAnim->OnFrameMove( fTime, fElapsedTime );
	}

	return S_OK;
}

void CX2SubEquip::SetUnitXSkinAnim(CKTDGXSkinAnimPtr pXSkinAnimPtr)
{
	if( m_bSkinMesh == true )
	{
		if( m_pSkinAnim != NULL )
		{
			m_pSkinAnim->SetUnitXSkinAnim( pXSkinAnimPtr );
		}
	}
}

void CX2SubEquip::SetJiggle(bool bJiggle, unsigned char fTension)
{
	if( m_bSkinMesh == true )
	{
		if( m_pSkinAnim != NULL )
		{
			m_pSkinAnim->SetJiggleMesh( bJiggle, fTension );
		}
	}
}

void CX2SubEquip::SetRenderParam( CKTDGXRenderer::RenderParam& pRenderParam )
{
	if( m_bSkinMesh == true && m_pSkinAnim != NULL )
	{
		CKTDGXRenderer::RenderParam *renderParam = m_pSkinAnim->GetRenderParam();
		*renderParam = pRenderParam;
	}
	
}
#endif


