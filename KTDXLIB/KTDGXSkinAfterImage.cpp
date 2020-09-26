#include "StdAfx.h"
#include ".\ktdgxskinafterimage.h"

CKTDGXSkinAfterImage::CKTDGXSkinAfterImage( CKTDGXSkinAnim* pOriginAnim, int imageNum, int layer )
#ifdef MODIFY_AFTER_IMAGE
	: m_pHairXSkinMesh(NULL)
#endif // MODIFY_AFTER_IMAGE
{
    ASSERT( pOriginAnim != NULL );

	SetAlphaObject( true );

	//{{ seojt // 2009-1-12, 14:57
	m_pOriginalAnim		= pOriginAnim;
    //m_pOriginalAnim = pOriginAnim->get_shared_ptr<CKTDGXSkinAnim>();
    //ASSERT( m_pOriginalAnim.use_count() >= 1 );
	//}} seojt // 2009-1-12, 14:57

	m_bEnable			= false;
	m_AfterImageNum		= imageNum;
	m_AfterImageColor	= 0xffffffff;
	m_NextImageTime		= 0.02f;
	m_NextImageTimeNow	= 0.0f;
	m_HideImageSpeed	= 1.6f;
	m_NewImageIndex		= 0;
	SetLayer( layer );

	CKTDGXRenderer::RenderParam		renderParam;
	renderParam.renderType			= CKTDGXRenderer::RT_COLOR_EDGE;
	renderParam.fOutLineWide		= -1.0f;
	renderParam.bAlphaBlend			= true;
	renderParam.bZEnable			= true;


	m_AfterImageList.reserve( 16 );


	AfterImageData data;
	for( int i = 0 ; i < m_AfterImageNum; i++ )
	{
		//data.pAnim = CKTDGXSkinAnim::CreateSkinAnim();
		data.pAnim = CKTDGXSkinAnim::CreateSkinAnim();
		CKTDGXRenderer::RenderParam* pRenderParam = data.pAnim->GetRenderParam();
		*pRenderParam = renderParam;
		data.pAnim->SetAlphaObject( true );		
        data.pAnim->SetLayer( GetLayer() );
		data.pAnim->Wait();
		data.pAnim->SetAnimXSkinMesh( m_pOriginalAnim->GetAnimXSkinMesh() );

//{{ robobeg : 2008-10-20
		//data.pAnim->SetShow( false );
//}} robobeg : 2008-10-20
		data.pAnim->SetShowObject( false );
		data.fAlpha = 0.0f;
		m_AfterImageList.push_back( data );

		data.pAnim->SetCenter( pOriginAnim->GetAnimXSkinMesh()->GetCenter() );
		data.pAnim->SetBoundingRadius( pOriginAnim->GetAnimXSkinMesh()->GetBoundingRadius() );
		g_pKTDXApp->GetDGManager()->AddObjectChain( data.pAnim );
	}
}

CKTDGXSkinAfterImage::~CKTDGXSkinAfterImage(void)
{
	for( int i = 0; i < (int)m_AfterImageList.size(); i++ )
	{
		SAFE_DELETE_KTDGOBJECT( m_AfterImageList[i].pAnim );
	}
	m_AfterImageList.clear();
}

void CKTDGXSkinAfterImage::AddMesh( CKTDXDeviceXSkinMesh* pMesh )
{
	for( int i = 0 ; i < m_AfterImageNum; i++ )
	{
		m_AfterImageList[i].pAnim->AddModelXSkinMesh( pMesh );
	}
}

HRESULT CKTDGXSkinAfterImage::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	if( m_bEnable == true )
	{
		m_NextImageTimeNow -= fElapsedTime;
		if( m_NextImageTimeNow <= 0.0f )
		{
			m_AfterImageList[m_NewImageIndex].fAlpha = 0.5f;
//{{ robobeg : 2008-10-20
			//m_AfterImageList[m_NewImageIndex].pAnim->SetShow( true );
//}} robobeg : 2008-10-20
			m_AfterImageList[m_NewImageIndex].pAnim->SetShowObject( true );
#ifdef FIX_MESH_RENDER_BEFORE_INITINALIZE
			m_AfterImageList[m_NewImageIndex].pAnim->SetProcessFrameMove(true);
#endif // FIX_MESH_RENDER_BEFORE_INITINALIZE

			m_AfterImageList[m_NewImageIndex].pAnim->CopyCloneFrameStructure( m_pOriginalAnim->GetCloneFrameRoot() );

			m_AfterImageList[m_NewImageIndex].pAnim->GetMatrix().Move( m_pOriginalAnim->GetMatrix().GetPos() );				// frumstum culling 되는 것을 방지하기 위해서
			m_AfterImageList[m_NewImageIndex].pAnim->GetMatrix().Rotate( m_pOriginalAnim->GetMatrix().GetRotate() );
			m_AfterImageList[m_NewImageIndex].pAnim->GetMatrix().Scale( m_pOriginalAnim->GetMatrix().GetScale() );

			m_NewImageIndex++;
			if( m_NewImageIndex >= m_AfterImageNum )
				m_NewImageIndex = 0;

			m_NextImageTimeNow = m_NextImageTime;
		}
	}

	for( int i = 0; i < (int)m_AfterImageList.size(); i++ )
	{
		AfterImageData* pAfterImageData = &m_AfterImageList[i];
		if( pAfterImageData->fAlpha <= 0.0f )
		{
			pAfterImageData->fAlpha = 0.0f;
//{{ robobeg : 2008-10-20
			//pAfterImageData->pAnim->SetShow( false );
//}} robobeg : 2008-10-20
			pAfterImageData->pAnim->SetShowObject( false );
		}
		else
		{
			pAfterImageData->fAlpha -= m_HideImageSpeed * fElapsedTime;
			pAfterImageData->pAnim->GetRenderParam()->outLineColor		= m_AfterImageColor;
			pAfterImageData->pAnim->GetRenderParam()->outLineColor.a	= pAfterImageData->fAlpha;
		}		
	}
	return S_OK;
}

//{{ robobeg : 2008-10-24
//virtual HRESULT OnFrameRender();
/*virtual*/
RENDER_HINT   CKTDGXSkinAfterImage::OnFrameRender_Prepare()
{
    if ( m_AfterImageList.empty() )
        return RENDER_HINT_NORENDER;

    RENDER_HINT renderHintResult = RENDER_HINT_NORENDER;
    RENDER_HINT renderHint;
    int iSize = (int)m_AfterImageList.size();
	for( int i = 0 ; i < iSize; i++ )
	{
		AfterImageData* pAfterImageData = &m_AfterImageList[i];
        ASSERT( pAfterImageData->pAnim != NULL );
        if ( pAfterImageData->pAnim == NULL )
            continue;
        pAfterImageData->pAnim->SetInstantShow( false );
        if ( !pAfterImageData->pAnim->GetShowObject() )
            continue;
        renderHint = pAfterImageData->pAnim->OnFrameRender_Prepare();
        if ( renderHint.m_eHint != RENDER_HINT::NORENDER )
        {
            pAfterImageData->pAnim->SetInstantShow( true );
            renderHintResult.MergeRenderHint( renderHint );
        }//if
	}//for

    return renderHintResult;

}//CKTDGXSkinAfterImage::OnFrameRender_Prepare()

/*virtual*/
void    CKTDGXSkinAfterImage::OnFrameRender_Draw()
{
    int iSize = (int)m_AfterImageList.size();

	for( int i = 0 ; i < iSize; i++ )
	{
		AfterImageData* pAfterImageData = &m_AfterImageList[i];
        ASSERT( pAfterImageData->pAnim != NULL );
        if ( pAfterImageData->pAnim == NULL )
            continue;
        if ( pAfterImageData->pAnim->GetShowObject() && pAfterImageData->pAnim->IsInstantShow() )
    		pAfterImageData->pAnim->OnFrameRender_Draw();
	}
//	return S_OK;
}

//}} robobeg : 2008-10-24



#ifdef MODIFY_AFTER_IMAGE
/** @function : UpdateEquipView
	@brief : 착용 장비가 변겨되었을 때 애프터 이미지도 같이 변경.
*/
void CKTDGXSkinAfterImage::UpdateEquipView( CKTDGXSkinAnim* pOriginAnim )
{
	if( NULL == pOriginAnim )
		return;

	BOOST_FOREACH( AfterImageData& sAfterIamgeData, m_AfterImageList )
	{
		// 기존 복장 모두 초기화
		sAfterIamgeData.pAnim->ClearModelXSkinMeshList();

		// 복장 추가
		vector<CKTDXDeviceXSkinMesh*> vecModelXSkinMeshLIst;
		m_pOriginalAnim->GetModelXSkinMeshList(vecModelXSkinMeshLIst);
		BOOST_FOREACH(CKTDXDeviceXSkinMesh* pDeviceXSkinMesh,  vecModelXSkinMeshLIst )
		{
			sAfterIamgeData.pAnim->AddModelXSkinMesh(pDeviceXSkinMesh, NULL, NULL, NULL, false, false );
		}

		// 헤어 포함되지 않았다면 (포함 되었다면 m_pHairXSkinMesh 은 NULL
		// 미리 저장해둔 헤어 메시 추가
		if( NULL != m_pHairXSkinMesh )
		{
			sAfterIamgeData.pAnim->AddModelXSkinMesh(m_pHairXSkinMesh, NULL, NULL, NULL, false, false );
		}
	}
}
#endif // MODIFY_AFTER_IMAGE