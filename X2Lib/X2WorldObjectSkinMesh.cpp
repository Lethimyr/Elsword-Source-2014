#include "stdafx.h"
#include ".\x2worldobjectskinmesh.h"


// constructor
CX2WorldObjectSkinMesh::CX2WorldObjectSkinMesh( bool bBackgroundLoad_ )
	: CX2WorldObject( bBackgroundLoad_ )
{
    ASSERT( ::GetCurrentThreadId() == g_pKTDXApp->GetMainThreadID() );

	m_ObjectType		= CX2WorldObject::OT_SKIN_MESH;

	m_bPingPongRightDir	= true;
	
	m_fAnimSpeed		= 1.f;
	m_pXSkinAnim		= NULL;
	m_bTransition		= true;

	m_pXSkinMotionMesh	= NULL;
	m_pAnimXET			= NULL;

	m_pMeshXET			= NULL;
	m_pMultiTexXET		= NULL;
	m_pChangeTexXET		= NULL;

	m_bCanHide			= false;
	m_fNowAlpha			= 1.0f;

	//m_RenderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;
	m_BillboardType		= CKTDGMatrix::BT_NONE;
	m_RenderParam.bAlphaBlend = true;
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
	m_RenderParam.fOutLineWide = CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
	m_RenderParam.fOutLineWide = 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
	m_RenderParam.outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);

	INIT_VECTOR3( m_LightPos, 1000, 1000, 1000 );

	//INIT_COLOR( m_Color, 1.0f, 1.0f, 1.0f, 1.0f );
	INIT_VECTOR2( m_Tex0UVSpeed, 0.0f, 0.0f );
	INIT_VECTOR2( m_Tex1UVSpeed, 0.0f, 0.0f );
	INIT_VECTOR2( m_Tex2UVSpeed, 0.0f, 0.0f );

	m_NowPlayAnimIndex	= 0;

	m_iOccasionalPlayRate	= 0;
	m_fOccasionalEventTime	= 0.f;
	m_fElapsedLastOccansionalEventTime = 0.f;

//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
	m_bXSkinMeshLoaded			= false;
	m_bModelXSkinMeshLoaded		= false;
    m_playTypeReq               = WOSMPY_ONE;
    m_bPlayTypeTransitionReq    = false;
    m_bPendingPlayAnim          = false;
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-11

	m_fLocalInterval = 0.f;
	m_fLocalElapsedTime = 0.f;


#ifdef WORLD_TRIGGER
	m_iTriggerObject = -1;
#endif

	m_PlayType	= WOSMPY_ONE_WAIT;
	m_bAnyCamera = false;

}//CX2WorldObjectSkinMesh::CX2WorldObjectSkinMesh()


// destructor
CX2WorldObjectSkinMesh::~CX2WorldObjectSkinMesh(void)
{
    ASSERT( ::GetCurrentThreadId() == g_pKTDXApp->GetMainThreadID() );

	//SAFE_DELETE( m_pXSkinAnim );
    SAFE_DELETE_KTDGOBJECT( m_pXSkinAnim );
	m_pXSkinAnim = NULL;
	
	SAFE_CLOSE( m_pXSkinMotionMesh );
	SAFE_CLOSE( m_pAnimXET ); 
	SAFE_CLOSE( m_pMeshXET );
	SAFE_CLOSE( m_pMultiTexXET );
	SAFE_CLOSE( m_pChangeTexXET );

	for ( int i = 0; i < (int)m_XSkinModelMeshList.size(); i++ )
	{
		XSkinMeshXETState& state = m_XSkinModelMeshList[ i ];
		SAFE_CLOSE( state.m_pXSkinMesh );
		SAFE_CLOSE( state.m_pMeshXET );
		SAFE_CLOSE( state.m_pMultiTexXET );
		SAFE_CLOSE( state.m_pChangeTexXET );
	}//for

	m_XSkinModelMeshList.clear();

}
HRESULT CX2WorldObjectSkinMesh::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2WorldObject::OnFrameMove( fTime, fElapsedTime );

    if ( m_bXSkinMeshLoaded == false && m_pXSkinMotionMesh != NULL )
    {
        CKTDXDevice::EDeviceState eState = m_pXSkinMotionMesh->GetDeviceState();
        if ( eState == CKTDXDevice::DEVICE_STATE_LOADED )
        {
			if ( m_pAnimXET == NULL || m_pAnimXET->IsLoaded() == true )
			{
				ASSERT( m_pXSkinAnim == NULL );
				m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
    			m_pXSkinAnim->SetAnimXSkinMesh( m_pXSkinMotionMesh, m_pAnimXET );
	    		m_bXSkinMeshLoaded = true;
			}//if
        }
        else if ( eState == CKTDXDevice::DEVICE_STATE_FAILED )
        {
            SAFE_CLOSE( m_pXSkinMotionMesh );
        }//if.. else..
    }//if

    if ( m_bModelXSkinMeshLoaded == false && IsValidMesh() == true && m_XSkinModelMeshList.empty() == false )
    {
        ASSERT( m_pXSkinAnim != NULL );
        bool    bLoaded = true;
        for( vector< XSkinMeshXETState >::iterator iter = m_XSkinModelMeshList.begin();
            iter != m_XSkinModelMeshList.end(); iter++ )
        {
			XSkinMeshXETState& state = *iter;
            if ( state.m_pXSkinMesh != NULL && state.m_bLoaded == false )
            {
				if ( state.m_pXSkinMesh->IsLoaded() == false )
				{
					bLoaded = false;
					break;
				}//if
				if ( state.m_pMeshXET != NULL && m_pMeshXET->IsLoaded() == false )
				{
					bLoaded = false;
					break;
				}//if
				if ( state.m_pMultiTexXET != NULL && m_pMultiTexXET->IsLoaded() == false )
				{
					bLoaded = false;
					break;
				}//if
				if ( state.m_pChangeTexXET != NULL && m_pChangeTexXET->IsLoaded() == false )
				{
					bLoaded = false;
					break;
				}//if
		        m_pXSkinAnim->AddModelXSkinMesh( state.m_pXSkinMesh, state.m_pMeshXET, state.m_pMultiTexXET, state.m_pChangeTexXET );
                if ( iter == m_XSkinModelMeshList.begin() )
                {
		            const CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = state.m_pXSkinMesh->GetFrameRoot();
		            if ( pFrame != NULL )
		            {
			            SetCenter( state.m_pXSkinMesh->GetCenter() );
			            SetBoundingRadius( state.m_pXSkinMesh->GetBoundingRadius() );
		            }//if
                }//if
                state.m_bLoaded = true;
            }
            else
            {
                bLoaded = false;
				break;
            }//if.. else..
        }//for

        if ( bLoaded == true )
        {
            m_bModelXSkinMeshLoaded = true;
			
			/** 미리 요청된 play anim type이 있으면 이를 처리한다.
            - jintaeks on 2008-12-12, 12:06 */
	        if( m_bPendingPlayAnim )
	        {
	            bool bSuccessPlayAnim = PlayAnim_LUA( m_playTypeReq, m_bPlayTypeTransitionReq );
	            // 요청된 play anim이 성공한 경우, 이를 제거한다.
	            if( bSuccessPlayAnim )
	            {
	                m_bPendingPlayAnim = false;
	            }//if
	        }//if
        }//if
    }//if

//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-12
    if( !IsValidMesh() || !IsValidModelMesh() )
        return S_OK;
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-12

	KTDXPROFILE();

	m_RenderParam.texOffsetStage0.x += m_Tex0UVSpeed.x * m_fElapsedTime;
	m_RenderParam.texOffsetStage0.y += m_Tex0UVSpeed.y * m_fElapsedTime;

	m_RenderParam.texOffsetStage1.x += m_Tex1UVSpeed.x * m_fElapsedTime;
	m_RenderParam.texOffsetStage1.y += m_Tex1UVSpeed.y * m_fElapsedTime;

	m_RenderParam.texOffsetStage2.x += m_Tex2UVSpeed.x * m_fElapsedTime;
	m_RenderParam.texOffsetStage2.y += m_Tex2UVSpeed.y * m_fElapsedTime;

	m_pXSkinAnim->SetBillBoardType( CKTDGMatrix::BT_NONE );
	m_pXSkinAnim->GetMatrix().Move( GetMatrix().GetPos() );
	m_pXSkinAnim->GetMatrix().Rotate( GetMatrix().GetRotate() );
	m_pXSkinAnim->GetMatrix().Scale( GetMatrix().GetScale() );    
	
	if( m_pXSkinAnim->GetState() == CKTDGXSkinAnim::XAS_WAIT )
	{
		if( m_PlayType == WOSMPY_OCCASIONAL_ONE_WAIT && 
			0.f != m_fOccasionalEventTime && 
			0 != m_iOccasionalPlayRate )
		{
			m_fElapsedLastOccansionalEventTime += fElapsedTime;

			if( m_fElapsedLastOccansionalEventTime > m_fOccasionalEventTime )
			{
				m_fElapsedLastOccansionalEventTime = 0.f;

				if( rand()%100 < m_iOccasionalPlayRate )
				{
					m_NowPlayAnimIndex++;

					if ( m_NowPlayAnimIndex >= (int)m_PlayList.size() )
						m_NowPlayAnimIndex = 0;

					m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );

					m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
				}
			}
		}
		else if(m_fLocalInterval > 0.f)
		{
			m_fLocalElapsedTime += fElapsedTime;

			if(m_fLocalElapsedTime >= m_fLocalInterval)
			{
				m_NowPlayAnimIndex++;

				if ( m_NowPlayAnimIndex >= (int)m_PlayList.size() )
					m_NowPlayAnimIndex = 0;

				m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );

				m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
				m_fLocalElapsedTime = 0.f;
			}					
		}
	}
	else if( m_pXSkinAnim->GetState() == CKTDGXSkinAnim::XAS_STOP )
	{
		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(m_PlayType)
		{
		case WOSMPY_LOOP:
			{
				m_NowPlayAnimIndex++;

				if ( m_NowPlayAnimIndex >= (int)m_PlayList.size() )
					m_NowPlayAnimIndex = 0;

				m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );

				//m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE, m_bTransition );

				if(m_fLocalInterval > 0.f)
				{
					m_fLocalElapsedTime += fElapsedTime;

					if(m_fLocalElapsedTime >= m_fLocalInterval)
					{						
						m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
						m_fLocalElapsedTime = 0.f;
					}					
				}
				else
				{
					m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
					m_fLocalElapsedTime = 0.f;
				}			

			} break; 
		case WOSMPY_PINGPONG:
			{
				if ( m_bPingPongRightDir == true )
				{
					m_NowPlayAnimIndex++;

					if ( m_NowPlayAnimIndex >= (int)m_PlayList.size() )
						m_NowPlayAnimIndex = m_NowPlayAnimIndex - 1;

					m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );

					m_bPingPongRightDir = false;

				}
				else
				{
					m_NowPlayAnimIndex--;

					if ( m_NowPlayAnimIndex < 0 )
						m_NowPlayAnimIndex = m_NowPlayAnimIndex + 1;

					m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );

					m_bPingPongRightDir = true;
				}

				m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE );

			} break;
		case WOSMPY_OCCASIONAL_ONE:
			{
				if( 0.f != m_fOccasionalEventTime && 
					0 != m_iOccasionalPlayRate )
				{

					m_fElapsedLastOccansionalEventTime += fElapsedTime;

					if( m_fElapsedLastOccansionalEventTime > m_fOccasionalEventTime )
					{
						m_fElapsedLastOccansionalEventTime = 0.f;

						if( rand()%100 < m_iOccasionalPlayRate )
						{
							m_NowPlayAnimIndex++;

							if ( m_NowPlayAnimIndex >= (int)m_PlayList.size() )
								m_NowPlayAnimIndex = 0;

							m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );

							m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE );
						}
					}
				}
			} break;
		case WOSMPY_OCCASIONAL_ONE_WAIT:
			{
				if( 0.f != m_fOccasionalEventTime && 
					0 != m_iOccasionalPlayRate )
				{
					m_fElapsedLastOccansionalEventTime += fElapsedTime;

					if( m_fElapsedLastOccansionalEventTime > m_fOccasionalEventTime )
					{
						m_fElapsedLastOccansionalEventTime = 0.f;

						if( rand()%100 < m_iOccasionalPlayRate )
						{
							m_NowPlayAnimIndex++;

							if ( m_NowPlayAnimIndex >= (int)m_PlayList.size() )
								m_NowPlayAnimIndex = 0;

							m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );

							m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
						}
					}
				}
			} break;
		default:
			break;
		}//}}
		
	}


	if ( g_pX2Game != NULL && g_pX2Game->IsHost() == false )
	{
		float fUpdateTime = 0.0f;
		m_pXSkinAnim->SetIsCulled( !g_pKTDXApp->GetDGManager()->IsInFrustum( this, fUpdateTime ) );

		//m_pXSkinAnim->SetUpdatePassedNeedTime( fUpdateTime );
	}
	else
		m_pXSkinAnim->SetIsCulled( false );

	m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}//CX2WorldObjectSkinMesh::OnFrameMove()


//{{ robobeg : 2008-10-16
/*virtual*/
RENDER_HINT   CX2WorldObjectSkinMesh::OnFrameRender_Prepare() 
{ 
    //__super::SetLastAccessTime( g_NowTime );

//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-12
    if( !IsValidMesh() || !IsValidModelMesh() )
        return RENDER_HINT_NORENDER;
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-12

    //if ( m_pXSkinMotionMesh == NULL || m_pXSkinAnim == NULL )
    //    return RENDER_HINT_NORENDER;

    m_pXSkinAnim->SetInstantShow( false );

    if ( !m_pXSkinAnim->GetShowObject() )
        return RENDER_HINT_NORENDER;




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



	//반투명 체크(시야가림) - 중요한 연산이 아니므로 Render에서 처리한다
	bool bHide = false;
	if( m_bCanHide == true )
	{
		if( g_pX2Game != NULL )
		{
			if( m_bAnyCamera == true || g_pX2Game->GetX2Camera()->GetCameraState() != CX2Camera::CS_NORMAL )
			{
				if( RayHit( g_pKTDXApp->GetDGManager()->GetCamera().GetEye(), g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt() ) == true )
				{
					//가려져야 한다
					bHide			= true;

					//알파를 낮춘다
					if( m_fNowAlpha > 0.3f )
						m_fNowAlpha -= 4.0f * m_fElapsedTime;
					else
						m_fNowAlpha = 0.3f;
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

	CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();
	*pRenderParam				= m_RenderParam;
	pRenderParam->color.r		= m_coColor.r * m_RenderParam.color.r;
	pRenderParam->color.g		= m_coColor.g * m_RenderParam.color.g;
	pRenderParam->color.b		= m_coColor.b * m_RenderParam.color.b;
	pRenderParam->color.a		= m_fNowAlpha;
	pRenderParam->lightPos		= m_LightPos;

	if( m_bCanHide == true && bHide == true )
	{
		SetAlphaObject( true );
		pRenderParam->bAlphaBlend = true;
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
			pRenderParam->bAlphaBlend = true;
		}
	}

    RENDER_HINT renderHint = m_pXSkinAnim->OnFrameRender_Prepare();

    if ( renderHint.m_eHint != RENDER_HINT::NORENDER )
        m_pXSkinAnim->SetInstantShow( true );

    return renderHint;

}//CX2WorldObjectSkinMesh::OnFrameRender_Prepare() 


//HRESULT CX2WorldObjectSkinMesh::OnFrameRender()
/*virtual*/
void CX2WorldObjectSkinMesh::OnFrameRender_Draw()
{
    KTDXPROFILE();

//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-12
    if( !IsValidMesh() || !IsValidModelMesh() )
        return;
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-12

    //if ( m_pXSkinMotionMesh == NULL || m_pXSkinAnim == NULL )
    //    return;

    if ( m_pXSkinAnim->GetShowObject() && m_pXSkinAnim->IsInstantShow() )
	    m_pXSkinAnim->OnFrameRender_Draw();
}//CX2WorldObjectSkinMesh::OnFrameRender_Draw()
//}} robobeg : 2008-10-16


HRESULT CX2WorldObjectSkinMesh::OnResetDevice()
{
    if( m_pXSkinAnim != NULL )
    {
	    m_pXSkinAnim->OnResetDevice();
    }//if

	return S_OK;
}//CX2WorldObjectSkinMesh::OnResetDevice()


HRESULT CX2WorldObjectSkinMesh::OnLostDevice()
{
    if( m_pXSkinAnim != NULL )
    {
	    m_pXSkinAnim->OnLostDevice();
    }//if

	return S_OK;
}//CX2WorldObjectSkinMesh::OnLostDevice()

//#ifdef  _HACKPROOF_CLIENT_
//
//
//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
///*virtual*/ bool CX2WorldObjectSkinMesh::OnThreadLoad(DWORD dwParam_, DWORD /*dwParam2_*/)
//{
//#ifdef _HACKPROOF_CLIENT_
//	if( dwParam_ == CKTDXDeviceManager::THREAD_REQUEST_OPEN_XSKINMESH )
//	{
//        ASSERT( m_pXSkinMotionMesh == NULL );
//        if( m_pXSkinMotionMesh != NULL )
//        {
//            return true;
//        }//if
//
//		m_pXSkinMotionMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_XSkinMeshName.c_str() );
//
//        ASSERT( m_pXSkinAnim == NULL );
//		//m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
//		m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
//
//		m_pXSkinAnim->SetAnimXSkinMesh( m_pXSkinMotionMesh, m_pAnimXET);
//		m_bXSkinMeshLoaded = true;
//
//        return true;
//	}
//	else if( dwParam_ == CKTDXDeviceManager::THREAD_REQUEST_OPEN_MODEL_XSKINMESH )
//	{
//        /** 모델 메시를 추가하려는데, 아직 skin mesh가 로드되어 있지 않으면
//            false를 리턴해서 다시 시도하도록 해야 한다.
//            - jintaeks on 2008-12-12, 11:56 */
//        if( !IsValidMesh() )
//            return false;
//
//		CKTDXDeviceXSkinMesh* pXModelMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_strModelXSkinMeshName.c_str() );
//
//		m_pXSkinAnim->AddModelXSkinMesh( pXModelMesh,  m_pMeshXET, m_pMultiTexXET, m_pChangeTexXET );
//
//		m_XSkinModelMeshList.push_back( pXModelMesh );
//
//
//		// 충돌처리를 위해서 bounding sphere를 계산해두자. 
//		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = this->m_XSkinModelMeshList[0]->GetFrameRoot();
//	//{{ robobeg : 2008-01-10
//		// CKTDXDeviceXSkinMesh는 root frame의 center, radius값을 이미 계산해 두고 있다.
//		// 만약 아래 구문을 실행했을 때 pFrame 트리 아래의 MeshContainer에서 MeshData.pMesh를 삭제한 상태라면
//		// 시스템 크래쉬가 발생한다.
//		//if( pFrame != NULL )
//		//{
//		//	D3DXFrameCalculateBoundingSphere( pFrame, &m_Sphere.center, &m_Sphere.fRadius ); 
//		//}
//		if ( pFrame != NULL )
//		{
//			SetCenter( this->m_XSkinModelMeshList[0]->GetCenter() );
//			SetBoundingRadius( this->m_XSkinModelMeshList[0]->GetBoundingRadius() );
//		}//if
//	//}} robobeg : 2008-01-10
//
//	    m_pMeshXET = NULL;
//	    m_pMultiTexXET = NULL;
//	    m_pChangeTexXET = NULL;
//
//		m_bModelXSkinMeshLoaded = true;
//
//        /** 미리 요청된 play anim type이 있으면 이를 처리한다.
//            - jintaeks on 2008-12-12, 12:06 */
//        if( m_bPendingPlayAnim )
//        {
//            bool bSuccessPlayAnim = PlayAnim_LUA( m_playTypeReq, m_bPlayTypeTransitionReq );
//            // 요청된 play anim이 성공한 경우, 이를 제거한다.
//            if( bSuccessPlayAnim )
//            {
//                m_bPendingPlayAnim = false;
//            }//if
//        }//if
//
//        return true;
//	}//if.. else if..
//
//    ASSERT( !"이 경우는 발생하면 안 됨 - jintaeks on 2008-12-13, 14:11" );
//#endif _HACKPROOF_CLIENT_
//
//
//	return false;
//}//CX2WorldObjectSkinMesh::OnThreadLoad()
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
//
//#endif  _HACKPROOF_CLIENT_


//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-12
bool CX2WorldObjectSkinMesh::PlayAnim_LUA( WORLD_OBJECT_SKIN_MESH_PLAY_TYPE playType, bool btransition )
{
    /** 이 함수는 메인 thread에서도, 리소스 로딩 쓰레드에서도 호출된다.
        - jintaeks on 2008-12-14, 17:13 */
    //ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

    /** 아직 메시가 로드되지 않은 경우는
        요청을 저장해 두었다가 메시가 로드되면 play하도록 한다.
        - jintaeks on 2008-12-12, 12:09 */
    if( !IsValidMesh() || !IsValidModelMesh() )
    {
        /** m_playTypeReq를 하나만 유지하는 이유:
            메시가 로드되기 전에, PlayAnim_LUA()를 여러번 호출하면
            아직 메시가 로드되기 전이기 때문에, 이러한 요청들 중에 가장 마지막 것만 valid한 요청일 것이다.
            그러므로, queue를 사용하여 play type를 큐잉하지 않고,
            가장 마지막 것만 저장하고 있다가,
            메시가 로드되는 즉시 이를 play하도록 구현하였다.
            - jintaeks on 2008-12-12, 12:08 */
        m_playTypeReq               = playType;
        m_bPlayTypeTransitionReq    = btransition;
        m_bPendingPlayAnim          = true;
        return false;
    }
    else
    {
	    m_PlayType = playType;
	    m_bTransition = btransition;

	    m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );
		m_pXSkinAnim->SetPlaySpeed( m_fAnimSpeed );
    	
	    if ( playType == WOSMPY_ONE_WAIT )
		    m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
	    else
		    m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE );


	    if( playType == WOSMPY_OCCASIONAL_ONE_WAIT )
		    m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
	    else if( playType == WOSMPY_OCCASIONAL_ONE )
		    m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE );

#if defined(WORLD_TOOL) || defined(X2TOOL)
		m_iOriPlayType = (int)playType;
#endif

        return true;
    }//if.. else..
}//CX2WorldObjectSkinMesh::PlayAnim_LUA()
//#else // UNIT_BACKGROUND_LOADING_TEST 
////{{AFX
//bool CX2WorldObjectSkinMesh::PlayAnim_LUA( WORLD_OBJECT_SKIN_MESH_PLAY_TYPE playType, bool btransition )
//{
//	m_PlayType = playType;
//	m_bTransition = btransition;
//
//	m_pXSkinAnim->ChangeAnim( m_PlayList[m_NowPlayAnimIndex].c_str(), m_bTransition );
//	m_pXSkinAnim->SetPlaySpeed( m_fAnimSpeed );
//
//	
//	if ( playType == WOSMPY_ONE_WAIT )
//		m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
//	else
//		m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE );
//
//
//	if( playType == WOSMPY_OCCASIONAL_ONE_WAIT )
//		m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
//	else if( playType == WOSMPY_OCCASIONAL_ONE )
//		m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE );
//
//    return true;
//}
////}}AFX
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-12


void CX2WorldObjectSkinMesh::AddAnim_LUA( const char* pAnimName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	if ( pAnimName != NULL && pAnimName[0] != NULL )
    {
		wstring tempName;
		ConvertCharToWCHAR( tempName, pAnimName );
		m_PlayList.push_back( tempName );

#if defined(WORLD_TOOL) || defined(X2TOOL)
	m_wstrOriAnimName = tempName;
#endif
	}
}//CX2WorldObjectSkinMesh::AddAnim_LUA()


//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
void CX2WorldObjectSkinMesh::SetXSkinMotion_LUA( const char* pSkinMeshFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	ConvertCharToWCHAR( m_XSkinMeshName, pSkinMeshFileName );

    m_bXSkinMeshLoaded = false;
    //if ( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() )
    if ( IsBackgroundLoad() == true )
    {
		CKTDXDeviceXSkinMesh* pOld = m_pXSkinMotionMesh;
        m_pXSkinMotionMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMeshInBackground( m_XSkinMeshName );
        ASSERT( m_pXSkinMotionMesh != NULL );
        if ( m_pXSkinMotionMesh == NULL )
        {
            ErrorLogMsg( XEM_ERROR109, pSkinMeshFileName );
        }//if
		SAFE_CLOSE( pOld );
    }
    else
	{
		CKTDXDeviceXSkinMesh* pOld = m_pXSkinMotionMesh;
		m_pXSkinMotionMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_XSkinMeshName.c_str() );
		ASSERT( NULL != m_pXSkinMotionMesh );
        if ( m_pXSkinMotionMesh == NULL )
        {
            ErrorLogMsg( XEM_ERROR109, pSkinMeshFileName );
        }
        else
        {
            ASSERT( m_pXSkinAnim == NULL );
		    //m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
		    m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
		    m_pXSkinAnim->SetAnimXSkinMesh( m_pXSkinMotionMesh, m_pAnimXET );
		    m_bXSkinMeshLoaded = true;

		} //if
		SAFE_CLOSE( pOld );

	}//if.. else..
}//CX2WorldObjectSkinMesh::SetXSkinMotion_LUA()
//#else // UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
////{{AFX
//void CX2WorldObjectSkinMesh::SetXSkinMotion_LUA( const char* pSkinMeshFileName )
//{
//	SAFE_CLOSE( m_pXSkinMotionMesh );
//	ConvertCharToWCHAR( m_XSkinMeshName, pSkinMeshFileName );
//
//	m_pXSkinMotionMesh	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_XSkinMeshName );
//
//	m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
//	m_pXSkinAnim->SetAnimXSkinMesh( m_pXSkinMotionMesh, m_pAnimXET);
//}
////}}AFX
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-11


void CX2WorldObjectSkinMesh::SetAnimAniXET_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	if ( pFileName == NULL || pFileName[0] == NULL )
		return;

	wstring strBuf;
	ConvertCharToWCHAR( strBuf, pFileName );
	
	CKTDXDeviceXET* pOld = m_pAnimXET;

	if ( IsBackgroundLoad() == true )
		m_pAnimXET = g_pKTDXApp->GetDeviceManager()->OpenXETInBackground( strBuf );
	else
		m_pAnimXET = g_pKTDXApp->GetDeviceManager()->OpenXET( strBuf );
	SAFE_CLOSE( pOld );
}//CX2WorldObjectSkinMesh::SetAnimAniXET_LUA()


//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
void CX2WorldObjectSkinMesh::AddXSkinModel_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	if ( pFileName == NULL || pFileName[0] == NULL )
		return;

	ConvertCharToWCHAR( m_wstrModelXSkinMeshName, pFileName );

    m_bModelXSkinMeshLoaded = false;
    //if ( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() )
    if ( IsBackgroundLoad() == true )
    {
        CKTDXDeviceXSkinMesh* pXModelMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMeshInBackground( m_wstrModelXSkinMeshName );
        if ( pXModelMesh == NULL )
        {
            ErrorLogMsg( XEM_ERROR109, pFileName );
        }
        else
        {
			XSkinMeshXETState state;
			state.m_pXSkinMesh = pXModelMesh;
			state.m_pMeshXET = m_pMeshXET;
			state.m_pMultiTexXET = m_pMultiTexXET;
			state.m_pChangeTexXET = m_pChangeTexXET;
            m_XSkinModelMeshList.push_back( state );
			m_pMeshXET = NULL;
	        m_pMultiTexXET = NULL;
	        m_pChangeTexXET = NULL;
        }//if.. else..
    }
	else
	{
        ASSERT( m_pXSkinAnim != NULL );
        if( m_pXSkinAnim == NULL )
            return;

		CKTDXDeviceXSkinMesh* pXModelMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_wstrModelXSkinMeshName );
        ASSERT( pXModelMesh != NULL );

        if ( pXModelMesh != NULL )
        {
            bool    bFirst = m_XSkinModelMeshList.empty();
            

		    m_pXSkinAnim->AddModelXSkinMesh( pXModelMesh,  m_pMeshXET, m_pMultiTexXET, m_pChangeTexXET );
			XSkinMeshXETState state;
			state.m_bLoaded = true;
			state.m_pXSkinMesh = pXModelMesh;
			state.m_pMeshXET = m_pMeshXET;
			state.m_pMultiTexXET = m_pMultiTexXET;
			state.m_pChangeTexXET = m_pChangeTexXET;

		    m_XSkinModelMeshList.push_back( state );


            if ( bFirst == true )
            {

		        // 충돌처리를 위해서 bounding sphere를 계산해두자. 
		        const CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = pXModelMesh->GetFrameRoot();
	        //{{ robobeg : 2008-01-10
		        // CKTDXDeviceXSkinMesh는 root frame의 center, radius값을 이미 계산해 두고 있다.
		        // 만약 아래 구문을 실행했을 때 pFrame 트리 아래의 MeshContainer에서 MeshData.pMesh를 삭제한 상태라면
		        // 시스템 크래쉬가 발생한다.
		        //if( pFrame != NULL )
		        //{
		        //	D3DXFrameCalculateBoundingSphere( pFrame, &m_Sphere.center, &m_Sphere.fRadius ); 
		        //}
		        if ( pFrame != NULL )
		        {
			        SetCenter( pXModelMesh->GetCenter() );
			        SetBoundingRadius( pXModelMesh->GetBoundingRadius() );
		        }//if
	        //}} robobeg : 2008-01-10

            }//if

	        m_pMeshXET = NULL;
	        m_pMultiTexXET = NULL;
	        m_pChangeTexXET = NULL;

		    m_bModelXSkinMeshLoaded = true;

        }

//#ifdef	_HACKPROOF_OBSOLETE_
//        // 필요한 경우 전처리 데이터를 생성한다.
//        if ( g_pData->GetWorldManager()->IsWritingPreprocessingData() )
//        {
//            CX2WorldManager::WORLD_PREPROCESSING_INFO   preInfo;
//            preInfo.m_eInfoType         = CX2WorldManager::X2WM_PPIT_XSKIN_MODEL;
//            preInfo.m_strName          = m_strModelXSkinMeshName;
//            preInfo.m_vCenter           = GetCenter();
//            preInfo.m_fBSphereRadiua    = GetBoundingRadius();
//            g_pData->GetWorldManager()->WritePreprocessingData( preInfo );
//        }//if
//#endif	_HACKPROOF_OBSOLETE_
	}//if.. else..
	
}//CX2WorldObjectSkinMesh::AddXSkinModel_LUA()
//#else // UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
////{{AFX
//void CX2WorldObjectSkinMesh::AddXSkinModel_LUA( const char* pFileName )
//{
//	wstring tempXMeshName;
//	ConvertCharToWCHAR( tempXMeshName, pFileName );
//	CKTDXDeviceXSkinMesh* pXModelMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( tempXMeshName );
//
//	m_pXSkinAnim->AddModelXSkinMesh( pXModelMesh,  m_pMeshXET, m_pMultiTexXET, m_pChangeTexXET );
//
//	m_XSkinModelMeshList.push_back( pXModelMesh );
//
//
//	// 충돌처리를 위해서 bounding sphere를 계산해두자. 
//	CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = this->m_XSkinModelMeshList[0]->GetFrameRoot();
////{{ robobeg : 2008-01-10
//    // CKTDXDeviceXSkinMesh는 root frame의 center, radius값을 이미 계산해 두고 있다.
//    // 만약 아래 구문을 실행했을 때 pFrame 트리 아래의 MeshContainer에서 MeshData.pMesh를 삭제한 상태라면
//    // 시스템 크래쉬가 발생한다.
//	//if( pFrame != NULL )
//	//{
//	//	D3DXFrameCalculateBoundingSphere( pFrame, &m_Sphere.center, &m_Sphere.fRadius ); 
//	//}
//    if ( pFrame != NULL )
//    {
//        SetCenter( this->m_XSkinModelMeshList[0]->GetCenter() );
//        SetBoundingRadius( this->m_XSkinModelMeshList[0]->GetBoundingRadius() );
//    }//if
////}} robobeg : 2008-01-10
//	
//	m_pMeshXET = NULL;
//	m_pMultiTexXET = NULL;
//	m_pChangeTexXET = NULL;
//
//    // 필요한 경우 전처리 데이터를 생성한다.
//    if ( g_pData->GetWorldManager()->IsWritingPreprocessingData() )
//    {
//        CX2WorldManager::WORLD_PREPROCESSING_INFO   preInfo;
//        preInfo.m_eInfoType         = CX2WorldManager::X2WM_PPIT_XSKIN_MODEL;
//        preInfo.m_wstrName          = tempXMeshName;
//        preInfo.m_vCenter           = GetCenter();
//        preInfo.m_fBSphereRadiua    = GetBoundingRadius();
//        g_pData->GetWorldManager()->WritePreprocessingData( preInfo );
//    }//if
//}
////}}AFX
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-11


void CX2WorldObjectSkinMesh::SetMeshAniXET_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	CKTDXDeviceXET* pOld = m_pMeshXET;

	wstring strBuf;
	ConvertCharToWCHAR( strBuf, pFileName );
	
	if ( IsBackgroundLoad() == true )
		m_pMeshXET = g_pKTDXApp->GetDeviceManager()->OpenXETInBackground( strBuf );
	else
		m_pMeshXET = g_pKTDXApp->GetDeviceManager()->OpenXET( strBuf );

	SAFE_CLOSE( pOld );
}//CX2WorldObjectSkinMesh::SetMeshAniXET_LUA()


void CX2WorldObjectSkinMesh::SetMultiTexXET_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	CKTDXDeviceXET* pOldXET = m_pMultiTexXET;
    if ( pFileName != NULL && pFileName[0] != NULL )
	{
		wstring strBuf;
		ConvertCharToWCHAR( strBuf, pFileName );
	
		if ( IsBackgroundLoad() == true )
			m_pMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXETInBackground( strBuf );
		else
			m_pMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( strBuf );
    }
    else
    {
		m_pMultiTexXET = NULL;
    }//if.. else..'
	SAFE_CLOSE( pOldXET );
}//CX2WorldObjectSkinMesh::SetMultiTexXET_LUA()


void CX2WorldObjectSkinMesh::SetChangeTexXET_LUA( const char* pFileName )
{
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	CKTDXDeviceXET* pOldXET = m_pChangeTexXET;
    if ( pFileName != NULL || pFileName[0] != NULL )
    {
		wstring strBuf;
		ConvertCharToWCHAR( strBuf, pFileName );
	
		if ( IsBackgroundLoad() == true )
    		m_pChangeTexXET = g_pKTDXApp->GetDeviceManager()->OpenXETInBackground( strBuf );
		else
			m_pChangeTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( strBuf );
    }
    else
    {
        m_pChangeTexXET = NULL;
    }//if
	SAFE_CLOSE( pOldXET );
}//CX2WorldObjectSkinMesh::SetChangeTexXET_LUA()



bool CX2WorldObjectSkinMesh::RayHit( D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance )
{
	bool bResult = false;


	D3DXVECTOR3 vCenter;
	D3DXMATRIX matTransform = GetMatrix().GetMatrix();
	D3DXVec3TransformCoord( &vCenter, &GetCenter(), &matTransform );
	D3DXVECTOR3 vCollisionPoint;
	
	if( g_pKTDXApp->GetCollision()->LineToSphere( rayStart, rayEnd, 1.f, // fix! lineRadius는 1로 하면 되는건가? 
												vCenter, GetBoundingRadius(), &vCollisionPoint ) == true )
	{
		bResult = true;
		if( NULL != pHitDistance )
		{
			vCollisionPoint -= rayStart;
			*pHitDistance = D3DXVec3Length( &vCollisionPoint );
		}		
	}
	else
	{
		bResult = false;
		if( NULL != pHitDistance )
		{			
			*pHitDistance = -1.f;
		}
	}

	return bResult;
}

#ifdef FOG_WORLD
void CX2WorldObjectSkinMesh::SetFog(float xNear, float xFar, float yNear, float yFar, D3DXCOLOR color, float density)
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

void CX2WorldObjectSkinMesh::SetCartoonRender()
{
	m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
	m_RenderParam.fOutLineWide		= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
	m_RenderParam.fOutLineWide		= 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
}

#ifdef WORLD_TRIGGER
void CX2WorldObjectSkinMesh::ChangePlayAnim_LUA( wstring playName, WORLD_OBJECT_SKIN_MESH_PLAY_TYPE playType )
{
	ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );

	m_PlayList.clear();
	m_PlayList.push_back( playName );
	PlayAnim_LUA(playType);
}//CX2WorldObjectSkinMesh::AddAnim_LUA()

#endif
