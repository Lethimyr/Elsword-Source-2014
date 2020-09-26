#include "stdafx.h"
#include ".\x2worldobjectmesh.h"
#include "KTDXDeviceManager.h"

CX2WorldObjectMesh::CX2WorldObjectMesh( bool bBackgroundLoad_ )
	: CX2WorldObject( bBackgroundLoad_ )
{
    ASSERT( ::GetCurrentThreadId() == g_pKTDXApp->GetMainThreadID() );

	m_ObjectType		= CX2WorldObject::OT_STATIC_MESH;

	m_bLOD				= false;
	m_pXMesh			= NULL;
	m_pXMeshLOD			= NULL;
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
    m_bXMeshLoaded      = false;
    m_bXMeshLODLoaded   = false;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20
	m_pMultiTexXET		= NULL;
	m_pChangeTexXET		= NULL;

	m_bCanHide			= false;
	m_bNotRenderWhenHidden = false;
	m_bAnyCamera		= false;
	m_fNowAlpha			= 1.0f;
	
	m_BillboardType		= CKTDGMatrix::BT_NONE;		

	//m_RenderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;
	//m_RenderParam.color = D3DXCOLOR( 1, 1, 1, 1 );
	//m_RenderParam.bAlphaBlend = false;

	//INIT_COLOR( m_Color, 1.0f, 1.0f, 1.0f, 1.0f );
	INIT_VECTOR2( m_Tex0UVSpeed, 0.0f, 0.0f );
	INIT_VECTOR2( m_Tex1UVSpeed, 0.0f, 0.0f );
	INIT_VECTOR2( m_Tex2UVSpeed, 0.0f, 0.0f );

#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
	m_RenderParam.fOutLineWide = CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
	m_RenderParam.fOutLineWide = 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
	m_RenderParam.outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	m_pkBoundingSphere = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Bounding_Sphere.x" );
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST

}


CX2WorldObjectMesh::~CX2WorldObjectMesh(void)
{
    ASSERT( ::GetCurrentThreadId() == g_pKTDXApp->GetMainThreadID() );

	SAFE_CLOSE( m_pXMesh );
	SAFE_CLOSE( m_pXMeshLOD );

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
    m_bXMeshLoaded      = false;
    m_bXMeshLODLoaded   = false;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20

	SAFE_CLOSE( m_pMultiTexXET );
	SAFE_CLOSE( m_pChangeTexXET );

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	SAFE_CLOSE( m_pkBoundingSphere );
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
}

HRESULT CX2WorldObjectMesh::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	CX2WorldObject::OnFrameMove( fTime, fElapsedTime );

    if ( m_bXMeshLoaded == false
        && m_pXMesh != NULL )
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
    }//if

    if ( m_bXMeshLODLoaded == false 
        && m_pXMeshLOD != NULL )
    {
        CKTDXDevice::EDeviceState eState = m_pXMeshLOD->GetDeviceState();
        if ( eState == CKTDXDevice::DEVICE_STATE_LOADED )
        {
            SetCenter( m_pXMeshLOD->GetCenter() );
            SetBoundingRadius( m_pXMeshLOD->GetRadius() );
            m_bXMeshLODLoaded = true;
        }
        else if ( eState == CKTDXDevice::DEVICE_STATE_FAILED )
        {
            SAFE_CLOSE( m_pXMeshLOD );
        }//if.. else..
    }//if

	m_RenderParam.texOffsetStage0.x += m_Tex0UVSpeed.x * m_fElapsedTime;
	m_RenderParam.texOffsetStage0.y += m_Tex0UVSpeed.y * m_fElapsedTime;

	m_RenderParam.texOffsetStage1.x += m_Tex1UVSpeed.x * m_fElapsedTime;
	m_RenderParam.texOffsetStage1.y += m_Tex1UVSpeed.y * m_fElapsedTime;
	
	m_RenderParam.texOffsetStage2.x += m_Tex2UVSpeed.x * m_fElapsedTime;
	m_RenderParam.texOffsetStage2.y += m_Tex2UVSpeed.y * m_fElapsedTime;

// 09.03.02 김태완
// OnFrameRender_Prepare로 이동
// 	if(m_fHideDistance > 0.f)
// 	{		
// 		if( GetDistanceToCamera() <= m_fHideDistance )
// 			SetShowObject(false);
// 		else
// 			SetShowObject(true);
// 	}
	return S_OK;
}


//{{ robobeg : 2008-10-15

/*virtual*/
RENDER_HINT   CX2WorldObjectMesh::OnFrameRender_Prepare() 
{ 
    //__super::SetLastAccessTime( g_NowTime );
	
	//{{ 09.03.02 김태완 : 지정된 거리 이상 멀어지면 안 보이게 하기
	if(m_fHideDistance > 0.f)
	{		
		if( GetDistanceToCamera() > m_fHideDistance )
			return RENDER_HINT_NORENDER;
	}
	if(m_fHideNearDistance > 0.f)
	{
		if( GetDistanceToCamera() < m_fHideNearDistance )
			return RENDER_HINT_NORENDER;
	}
	//}}

    // TODO: jintaeks on 2008-10-21, 10:24
	//CKTDXDeviceXMesh* pXMesh = NULL;

	//if( m_bLOD == true )
	//	pXMesh = m_pXMeshLOD;
	//else
	//	pXMesh = m_pXMesh;

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
    /** background loading을 사용하는 경우, 메시가 아직 로드되지 않았다면
        렌더링을 skip한다. - jintaeks on 2008-10-24, 10:40 */
	if( m_bLOD == true )
    {
		if ( !IsValidLODMesh() ) return RENDER_HINT_NORENDER;
    }
	else
    {
		if ( !IsValidMesh() ) return RENDER_HINT_NORENDER;
    }//if.. else..
//#else // BACKGROUND_LOADING_TEST // 2008-10-20
//
//	if( pXMesh == NULL )
//		return RENDER_HINT_NORENDER;
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20

	bool bHide = false;

	//반투명 체크(시야가림) - 중요한 연산이 아니므로 Render에서 처리한다
	if( m_bCanHide == true )
	{
		if( NULL != g_pX2Game )
		{
			if( m_bAnyCamera == true || g_pX2Game->GetX2Camera()->GetCameraState() != CX2Camera::CS_NORMAL )
			{		
				//레이가 모델에 히트하면
				float fDist = 0.f;
				if( RayHit( g_pKTDXApp->GetDGManager()->GetCamera().GetEye(), g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt(), 
					&fDist, 
					GetMatrix().GetMatrix(GetBillboardType()) ) == true )
				{
					//가려져야 한다
					bHide			= true;

					//알파를 낮춘다
					if( m_fNowAlpha > 0.3f )
						m_fNowAlpha -= 4.0f * m_fElapsedTime;
					else
					{
						m_fNowAlpha = 0.3f;
						if( true == m_bNotRenderWhenHidden )
						{
							return RENDER_HINT_NORENDER;
						}
					}
				}
			}
		}
	}

	//레이가 히트하지 않으면
	if( bHide == false )
	{
		m_fNowAlpha += 4.0f * m_fElapsedTime;

		if( m_fNowAlpha > m_RenderParam.color.a * m_coColor.a )
			m_fNowAlpha = m_RenderParam.color.a * m_coColor.a;
	}
	

	//랜더 파라메터 세팅
	if( m_bCanHide == true && bHide == true )
	{
		SetAlphaObject( true );
	}
	else
	{
		if( m_fNowAlpha >= m_RenderParam.color.a * m_coColor.a )
		{
			SetAlphaObject( m_RenderParam.bAlphaBlend );
		}
		else
		{
			SetAlphaObject( true );
		}
	}//if.. else..

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	if ( m_pkBoundingSphere != NULL )
	{
		return RENDER_HINT_DEFAULT;
	}//if
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST

    return RENDER_HINT_XMESH( m_RenderParam.renderType, m_RenderParam.bFog && m_RenderParam.bFogShow );
}


//HRESULT CX2WorldObjectMesh::OnFrameRender()
/*virtual*/
void    CX2WorldObjectMesh::OnFrameRender_Draw()
{
	KTDXPROFILE();

	CKTDXDeviceXMesh* pXMesh = NULL;

	if( m_bLOD == true && m_bXMeshLODLoaded )
		pXMesh = m_pXMeshLOD;
	else if ( m_bXMeshLoaded )
		pXMesh = m_pXMesh;

	if( pXMesh == NULL )
		return;

	if ( m_pChangeTexXET != NULL && m_pChangeTexXET->IsLoaded() == false 
		|| m_pMultiTexXET != NULL && m_pMultiTexXET->IsLoaded() == false )
		return;

	//랜더 파라메터 세팅

	CKTDGXRenderer::RenderParam renderParam( m_RenderParam );

	renderParam.color.r		= m_coColor.r * m_RenderParam.color.r;
	renderParam.color.g		= m_coColor.g * m_RenderParam.color.g;
	renderParam.color.b		= m_coColor.b * m_RenderParam.color.b;
	renderParam.color.a		= m_fNowAlpha;

    const D3DXMATRIX& worldMatrix = GetMatrix().GetMatrix( m_BillboardType );

	renderParam.lightPos		= m_LightPos;	

    if ( IsAlphaObject() )
        renderParam.bAlphaBlend = true;
    else
        renderParam.bAlphaBlend = false;

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *pXMesh, m_pChangeTexXET, m_pMultiTexXET, NULL, 0, 1, &m_vecDrawSubset );
#else//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *pXMesh, m_pChangeTexXET, m_pMultiTexXET );
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	if ( m_pkBoundingSphere != NULL )
	{
		DWORD dwOldFillMode = CKTDGStateManager::GetRenderState( D3DRS_FILLMODE );
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		CKTDGStateManager::SetTexture( 0, NULL );
		CKTDGStateManager::SetTexture( 1, NULL );

		//mesh 전체에 대한 bounding sphere 렌더링
		{
			D3DXVECTOR3 vPos = GetCenter();
			float   fRadius = GetBoundingRadius();

			CKTDGXRenderer::RenderParam renderParam;
			renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;		
			renderParam.color = D3DXCOLOR(1,0,0,1);
			CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );

			matTemp.Scale( fRadius, fRadius, fRadius );
			matTemp.Move( vPos );	
			D3DXMATRIX worldMatrix = matTemp.GetMatrix();

			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pkBoundingSphere );
		}

		//mesh subset에 대한 bounding sphere 렌더링
		{
			const std::vector< CKTDXDeviceXMesh::KSubsetCullingInfo >& vecSubsetBoundingSphere = pXMesh->GetSubsetCullingInfoVec();
			for( int i = 0; i < m_vecDrawSubset.size(); i++ )
			{
				if( m_vecDrawSubset.size() > i && m_vecDrawSubset[i] == false )
					continue;

				D3DXVECTOR3 vPos = vecSubsetBoundingSphere[i].m_vCenter;
				float   fRadius = vecSubsetBoundingSphere[i].m_fRadius;

				CKTDGXRenderer::RenderParam renderParam;
				renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;		
				renderParam.color = D3DXCOLOR(0,1,0,1);
				CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );

				matTemp.Scale( fRadius, fRadius, fRadius );
				matTemp.Move( vPos );	
				D3DXMATRIX worldMatrix = matTemp.GetMatrix();

				g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pkBoundingSphere );
			}
		}

		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, dwOldFillMode );
	}//if
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
}

void CX2WorldObjectMesh::SetLOD( bool bLOD )
{
	CX2WorldObject::SetLOD( bLOD );

	if( m_bLOD == true && m_pXMesh == NULL )
	{
		m_pXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_XMeshName );
        if ( m_pXMesh != NULL )
        {
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
            m_bXMeshLoaded = true;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20
		    SetCenter( m_pXMesh->GetCenter() );
		    SetBoundingRadius( m_pXMesh->GetRadius() );
        }//if
	}
	else if( m_pXMeshLOD == NULL )
	{
		m_pXMeshLOD = g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_XMeshLODName );
        if ( m_pXMeshLOD != NULL )
        {
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
            m_bXMeshLODLoaded = true;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20
		    SetCenter( m_pXMeshLOD->GetCenter() );
		    SetBoundingRadius( m_pXMeshLOD->GetRadius() );
        }//if
	}
}

bool CX2WorldObjectMesh::RayHit( D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance )
{
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
    if ( !IsValidMesh() )
        return false;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20

	D3DXVECTOR3 rayDir = rayEnd - rayStart;
	D3DXVec3Normalize( &rayDir, &rayDir );
	float fHitDistance = 0.0f;

	D3DXVECTOR3 transform = GetMatrix().GetPos();
	bool bResult = false;

	// fix! 회전된 모델에 대해서는 raycheck가 올바르게 되지 않는다.
	if(	g_pKTDXApp->GetCollision()->DXRayCheck( m_pXMesh->GetMesh(),
										rayStart, rayDir, transform, &fHitDistance ) )
	{
		if( fHitDistance <= GetDistance( rayStart, rayEnd ) )
		{
			bResult = true;
		}		
	}	

	if( NULL != pHitDistance )
		*pHitDistance = fHitDistance;
	
	return bResult;
}


bool CX2WorldObjectMesh::RayHit( const D3DXVECTOR3& rayStart, const D3DXVECTOR3& rayEnd, float* pHitDistance, const D3DXMATRIX& transform )
{
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
	if ( !IsValidMesh() )
		return false;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20

	D3DXVECTOR3 rayDir = rayEnd - rayStart;
	D3DXVec3Normalize( &rayDir, &rayDir );
	float fHitDistance = 0.0f;
		
	bool bResult = false;

	if(	g_pKTDXApp->GetCollision()->DXRayCheck( m_pXMesh->GetMesh(),
		rayStart, rayDir, transform, &fHitDistance ) )
	{
		if( fHitDistance <= GetDistance( rayStart, rayEnd ) )
		{
			bResult = true;
		}		
	}	

	if( NULL != pHitDistance )
		*pHitDistance = fHitDistance;

	return bResult;
}


void CX2WorldObjectMesh::SetXMesh_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
    ASSERT( pFileName );

	SAFE_CLOSE( m_pXMesh );
	ConvertUtf8ToWCHAR( m_XMeshName, pFileName );

#ifdef	_IN_HOUSE_
	if ( NULL != pFileName && !m_XMeshName.empty() )
	{
		StateLog( m_XMeshName.c_str() );
	}
	else
	{
		StateLog( L"FileName NULL" );
	}
#endif	_IN_HOUSE_

    //{{ seojt // 2008-10-22, 11:53
    /** 객체의 이름을 설정한다. .lua 파일에서 별도로 객체의 이름을 설정하는 곳이 없어서
        SetXMesh_LUA()함수의 내부에서, 메시 파일 이름을 객체의 이름으로 설정하도록 하다.
        - jintaeks on 2008-10-22, 11:53

        background 로딩을 사용할 때, 객체의 이름으로 실제 객체에 대한 포인터를 얻기 위해
        필요하다.
        - jintaeks on 2008-10-24, 10:43 */
    SetName( pFileName );
    //}} seojt // 2008-10-22, 11:53

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20

    /** EnableThreadLoading 플래그가 true로 설정되어 있으면, 실제 로드가 아니라,
        리소스 로딩 쓰레드 큐로 큐잉을 진행한다.
        - jintaeks on 2008-10-24, 10:44 */
    m_bXMeshLoaded = false;
    //if ( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() )
    if ( IsBackgroundLoad() == true )
    {
        m_pXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMeshInBackground( m_XMeshName.c_str(), CKTDXDeviceManager::PRIORITY_HIGH );
        if ( m_pXMesh == NULL )
        {
            ErrorLogMsg( XEM_ERROR109, pFileName );
        }//if
    }
    else
    {
	    m_pXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_XMeshName );
		ASSERT( NULL != m_pXMesh );
	    if( m_pXMesh == NULL )
	    {
		    ErrorLogMsg( XEM_ERROR109, pFileName );
	    }
	    else
	    {
		    SetCenter( m_pXMesh->GetCenter() );
		    SetBoundingRadius( m_pXMesh->GetRadius() );
            m_bXMeshLoaded = true;

//#ifdef	_HACKPROOF_OBSOLETE_
//            //{{ seojt // 2008-10-21, 14:11
//            /** 메시의 정보를 file로 write한다.
//                현재 - jintaeks on 2008-10-21, 14:15 - 메시이름과 bounding sphere 정보를 write하는데
//                이 정보는 background loading thread가 메시를 로드하기전에 객체의 culling 정보로
//                사용한다. - jintaeks on 2008-10-21, 14:16 */
//            if ( g_pData->GetWorldManager()->IsWritingPreprocessingData() )
//            {
//                CX2WorldManager::WORLD_PREPROCESSING_INFO   preInfo;
//                preInfo.m_eInfoType         = CX2WorldManager::X2WM_PPIT_XMESH;
//                preInfo.m_strName          = m_XMeshName;
//                preInfo.m_vCenter           = GetCenter();
//                preInfo.m_fBSphereRadiua    = GetBoundingRadius();
//                g_pData->GetWorldManager()->WritePreprocessingData( preInfo );
//            }//if
//            //}} seojt // 2008-10-21, 14:11
//#endif	_HACKPROOF_OBSOLETE_
	    }//if.. else..
    }//if.. else..

//#else // BACKGROUND_LOADING_TEST // 2008-10-20
//
//    m_pXMesh	= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_XMeshName.c_str() );
//	if( m_pXMesh == NULL )
//	{
//		ErrorLogMsg( XEM_ERROR109, pFileName );
//	}
//	else
//	{
//		SetCenter( m_pXMesh->GetCenter() );
//		SetBoundingRadius( m_pXMesh->GetRadius() );
//
////#ifdef	_HACKPROOF_OBSOLETE_
////        //{{ seojt // 2008-10-21, 14:11
////        /** 메시의 정보를 file로 write한다.
////            현재 - jintaeks on 2008-10-21, 14:15 - 메시이름과 bounding sphere 정보를 write하는데
////            이 정보는 background loading thread가 메시를 로드하기전에 객체의 culling 정보로
////            사용한다. - jintaeks on 2008-10-21, 14:16 */
////        if ( g_pData->GetWorldManager()->IsWritingPreprocessingData() )
////        {
////            CX2WorldManager::WORLD_PREPROCESSING_INFO   preInfo;
////            preInfo.m_eInfoType         = CX2WorldManager::X2WM_PPIT_XMESH;
////            preInfo.m_strName          = m_XMeshName;
////            preInfo.m_vCenter           = GetCenter();
////            preInfo.m_fBSphereRadiua    = GetBoundingRadius();
////            g_pData->GetWorldManager()->WritePreprocessingData( preInfo );
////        }//if
////        //}} seojt // 2008-10-21, 14:11
////#endif	_HACKPROOF_OBSOLETE_
//	}//if.. else..
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20
//#endif // BACKGROUND_LOADING_TEST

}//CX2WorldObjectMesh::SetXMesh_LUA()


void CX2WorldObjectMesh::SetXMeshLOD_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	SAFE_CLOSE( m_pXMeshLOD );
	ConvertUtf8ToWCHAR( m_XMeshLODName, pFileName );

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20

    m_bXMeshLODLoaded = false;
    //if ( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() )
    if ( IsBackgroundLoad() == true )
    {
        m_pXMeshLOD = g_pKTDXApp->GetDeviceManager()->OpenXMeshInBackground( m_XMeshLODName.c_str(), CKTDXDeviceManager::PRIORITY_HIGH );
        if ( m_pXMeshLOD == NULL )
        {
            ErrorLogMsg( XEM_ERROR109, pFileName );
        }//if
    }
    else
    {
	    m_pXMeshLOD = g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_XMeshLODName );
	    if( m_pXMeshLOD == NULL )
	    {
		    ErrorLogMsg( XEM_ERROR110, pFileName );
	    }
	    else
	    {
		    SetCenter( m_pXMeshLOD->GetCenter() );
		    SetBoundingRadius( m_pXMeshLOD->GetRadius() );
            m_bXMeshLODLoaded = true;

//#ifdef	_HACKPROOF_OBSOLETE_
//            //{{ seojt // 2008-10-21, 14:11
//            /** 메시의 정보를 file로 write한다.
//                현재 - jintaeks on 2008-10-21, 14:15 - 메시이름과 bounding sphere 정보를 write하는데
//                이 정보는 background loading thread가 메시를 로드하기전에 객체의 culling 정보로
//                사용한다. - jintaeks on 2008-10-21, 14:16 */
//            if ( g_pData->GetWorldManager()->IsWritingPreprocessingData() )
//            {
//                CX2WorldManager::WORLD_PREPROCESSING_INFO   preInfo;
//                preInfo.m_eInfoType         = CX2WorldManager::X2WM_PPIT_XMESHLOD;
//                preInfo.m_strName          = m_XMeshLODName;
//                preInfo.m_vCenter           = GetCenter(); 
//                preInfo.m_fBSphereRadiua    = GetBoundingRadius();
//                g_pData->GetWorldManager()->WritePreprocessingData( preInfo );
//            }//if
//            //}} seojt // 2008-10-21, 14:11
//#endif	_HACKPROOF_OBSOLETE_

	    }//if.. else..
    }//if.. else..

//#else // BACKGROUND_LOADING_TEST // 2008-10-20
//	m_pXMeshLOD		= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_XMeshLODName.c_str() );
//	if( m_pXMeshLOD == NULL )
//	{
//		ErrorLogMsg( XEM_ERROR110, pFileName );
//	}
//	else
//	{
//		SetCenter( m_pXMeshLOD->GetCenter() );
//		SetBoundingRadius( m_pXMeshLOD->GetRadius() );
//
////#ifdef	_HACKPROOF_OBSOLETE_
////        //{{ seojt // 2008-10-21, 14:11
////        /** 메시의 정보를 file로 write한다.
////            현재 - jintaeks on 2008-10-21, 14:15 - 메시이름과 bounding sphere 정보를 write하는데
////            이 정보는 background loading thread가 메시를 로드하기전에 객체의 culling 정보로
////            사용한다. - jintaeks on 2008-10-21, 14:16 */
////        if ( g_pData->GetWorldManager()->IsWritingPreprocessingData() )
////        {
////            CX2WorldManager::WORLD_PREPROCESSING_INFO   preInfo;
////            preInfo.m_eInfoType         = CX2WorldManager::X2WM_PPIT_XMESHLOD;
////            preInfo.m_strName          = m_XMeshLODName;
////            preInfo.m_vCenter           = GetCenter(); 
////            preInfo.m_fBSphereRadiua    = GetBoundingRadius();
////            g_pData->GetWorldManager()->WritePreprocessingData( preInfo );
////        }//if
////        //}} seojt // 2008-10-21, 14:11
////#endif	_HACKPROOF_OBSOLETE_
//	}//if.. else..
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20



}//CX2WorldObjectMesh::SetXMeshLOD_LUA()


void CX2WorldObjectMesh::SetMultiTexXET_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
	CKTDXDeviceXET* pXET = m_pMultiTexXET;
    if ( pFileName  != NULL && pFileName [0] != NULL )
    {
		wstring strBuf;
		ConvertUtf8ToWCHAR( strBuf, pFileName );

		if ( IsBackgroundLoad() == true )
			m_pMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXETInBackground( strBuf );
		else
			m_pMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( strBuf );
	    if( m_pMultiTexXET == NULL )
	    {
		    ErrorLogMsg( XEM_ERROR111, pFileName );
	    }
    }
	else
	{	
		m_pMultiTexXET = NULL;
	}//if.. else..
	SAFE_CLOSE( pXET );
}//CX2WorldObjectMesh::SetMultiTexXET_LUA()


void CX2WorldObjectMesh::SetChangeTexXET_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	SAFE_CLOSE( m_pChangeTexXET );
    if ( pFileName != NULL )
    {
		wstring strBuf;
		ConvertUtf8ToWCHAR( strBuf, pFileName );
	
		if ( IsBackgroundLoad() == true )
			m_pChangeTexXET = g_pKTDXApp->GetDeviceManager()->OpenXETInBackground( strBuf );
		else
			m_pChangeTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( strBuf );
	    if( m_pChangeTexXET == NULL )
	    {
		    ErrorLogMsg( XEM_ERROR112, pFileName );
	    }
    }//if

}//CX2WorldObjectMesh::SetChangeTexXET_LUA()


CKTDXDeviceXMesh* CX2WorldObjectMesh::GetMesh()
{
    //ASSERT( m_bXMeshLoaded );
    //ASSERT( m_bXMeshLODLoaded );

	CKTDXDeviceXMesh* pXMesh = NULL;

	if( m_bLOD == true && m_bXMeshLODLoaded )
		pXMesh = m_pXMeshLOD;
	else if ( m_bXMeshLoaded )
		pXMesh = m_pXMesh;

	return pXMesh;
}//CX2WorldObjectMesh::GetMesh()


#ifdef FOG_WORLD
void CX2WorldObjectMesh::SetFog(float xNear, float xFar, float yNear, float yFar, D3DXCOLOR color, float density)
{
	m_RenderParam.bFogShow      = true;
	m_RenderParam.bFog          = true;
	m_RenderParam.fogColor      = color;
	m_RenderParam.fogDensity    = density;
	m_RenderParam.fogFarX       = xFar;
	m_RenderParam.fogFarY       = yFar;
	m_RenderParam.fogNearX      = xNear;
	m_RenderParam.fogNearY      = yNear;
}
#endif

