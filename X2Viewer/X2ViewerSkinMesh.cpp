#include "dxstdafx.h"
#include ".\x2viewerskinmesh.h"

#pragma region CX2ViewerSkinMesh
CX2ViewerSkinMesh::CX2ViewerSkinMesh(void)
{
	CX2ViewerObject::SetObjectStyle( CX2ViewerObject::OS_SKIN_MESH );

	m_pXSkinAnim		= NULL;
	m_pXET				= NULL;
	m_pXMeshLight		= NULL;
	m_bWireframeMode	= false;
	m_bIsLight			= true;
	m_bIsAnimPlay		= true;
	m_bIsMotion			= false;
	m_ImpactNowAnimTime	= 0.0f;
	m_bIsBounding		= false;
	m_bShowAttackBox	= false;

	m_mapXSkinWeapon.clear();
	m_pXETWeapon = NULL;

	m_AnimPlaytype = CKTDGXSkinAnim::XAP_LOOP;

	//Light 위치표기 메쉬
	m_pXMeshLight		= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Sun.X" );
	m_pMatrixLight		= new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	m_pMatrixLight->Scale( 0.5f, 0.5f, 0.5f );

	// 충돌박스 메쉬
	m_pXMeshSphere = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Bounding_Sphere.X" );
	// 어택박스 메쉬
	m_pXAttackMeshSphere = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Attack_Sphere.X" );
	m_pMatrixSphere	= new CKTDGMatrix( g_pKTDXApp->GetDevice() );

	TEX_STAGE_DATA texStageData;
	m_vecTexStageData.push_back( texStageData );
	m_vecTexStageData.push_back( texStageData );
	m_vecTexStageData.push_back( texStageData );

	m_pWeaponMatrix = NULL;
	m_RotWeapon = D3DXVECTOR3( 0, 0, 0 );

    m_pRendererAccessory	= new CKTDGXRenderer( g_pKTDXApp->GetDevice() );
    
	m_mapXMeshAccessory.clear();
    //m_pXMeshAccessory	= NULL;
    //m_pMatrixAccessory  = NULL;
    //m_TransAccessory    = D3DXVECTOR3( 0, 0, 0 );
    //m_RotAccessory      = D3DXVECTOR3( 0, 0, 0 );
    //m_ScaleAccessory    = D3DXVECTOR3( 100, 100, 100 );
	//D3DXCreateCylinder( g_pKTDXApp->GetDevice(), 100.0f, 100.0f, 200.0f, 10, 10, &m_pSMesh, NULL );
	//m_pSMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );

	INIT_VECTOR3( m_vScale,	1.0f, 1.0f, 1.0f );
	INIT_VECTOR3( m_vPosition,	0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_RenderParam.lightPos, 300, 300, -500 );

	m_bAttachPoint      = false;
    m_pRendererPoint	= new CKTDGXRenderer( g_pKTDXApp->GetDevice() );
    m_pXMeshPoint		= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Bone.X" );
    m_pMatrixPoint		= NULL;
	INIT_VECTOR3( m_vScale,	1.0f, 1.0f, 1.0f );
}
#pragma endregion 생성자

#pragma region ~CX2ViewerSkinMesh
CX2ViewerSkinMesh::~CX2ViewerSkinMesh(void)
{
	//SAFE_RELEASE( m_pSMesh );
	//SAFE_DELETE( m_pSMatrix );

	g_pKTDXApp->GetDeviceManager()->CloseDevice( m_pXMeshLight->GetDeviceID() );
    SAFE_DELETE( m_pMatrixLight );

	g_pKTDXApp->GetDeviceManager()->CloseDevice( m_pXAttackMeshSphere->GetDeviceID() );
	g_pKTDXApp->GetDeviceManager()->CloseDevice( m_pXMeshSphere->GetDeviceID() );
	SAFE_DELETE( m_pMatrixSphere );
	
    SAFE_DELETE( m_pRendererPoint );
    g_pKTDXApp->GetDeviceManager()->CloseDevice( m_pXMeshPoint->GetDeviceID() );
    
	SAFE_DELETE( m_pRendererAccessory );

	BOOST_FOREACH( SUnitBoneModify* pUnitBoneModifyData, m_vUnitBoneModify )
	{
		BOOST_FOREACH( SBoneInfo* pBoneInfo, pUnitBoneModifyData->m_vecModifyBoneInfo )
		{
			SAFE_DELETE( pBoneInfo );
		}

		SAFE_DELETE( pUnitBoneModifyData );
	}

	Reset();
}
#pragma endregion 소멸자

#pragma region OnFrameMove
HRESULT CX2ViewerSkinMesh::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return S_OK;

	CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();
	*pRenderParam = m_RenderParam;

	D3DXVECTOR3 position = m_pXSkinAnim->GetMatrix().GetPos();
	//모션 이동
	position.x += m_pXSkinAnim->GetMotionOffset().x / 3.0f;
	position.z += m_pXSkinAnim->GetMotionOffset().z / 3.0f;

	m_pXSkinAnim->GetMatrix().Scale( m_vScale );
	m_pXSkinAnim->GetMatrix().Move( position );
	m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );
	
	if( IsSamef( m_pXSkinAnim->IsAnimationEnd() ) )
	{
		m_pXSkinAnim->GetMatrix().Move( D3DXVECTOR3( m_vPosition.x, m_vPosition.y, m_vPosition.z ));
	}

	//Impact Anim setting
	if( m_ImpactData.fAnimTime != 0.0f )
	{
		if( m_ImpactData.fAnimTime <= m_ImpactNowAnimTime )
		{
			m_ImpactNowAnimTime = 0.0f;
			pRenderParam->fLightFlowImpact = m_ImpactData.fMin;
		}
		else
		{
			m_ImpactNowAnimTime += fElapsedTime;
		}
		pRenderParam->fLightFlowImpact = m_ImpactData.fMin + ((m_ImpactData.fMax - m_ImpactData.fMin)*(m_ImpactNowAnimTime/m_ImpactData.fAnimTime));
	}
	else
	{
		m_ImpactNowAnimTime = 0.0f;
	}

	//Texture Stage 0~2 Setting
	for( int i = 0; i < (int)m_vecTexStageData.size(); ++i )
	{
		if( m_vecTexStageData[i].fAnimTime != 0.0f )
		{
			if( m_vecTexStageData[i].fAnimTime <= m_vecTexStageData[i].fNowAnimTime )
			{
				m_vecTexStageData[i].fNowAnimTime = 0.0f;
				switch( i )
				{
				case 0: pRenderParam->texOffsetStage0 = m_vecTexStageData[i].vMin; break;
				case 1: pRenderParam->texOffsetStage1 = m_vecTexStageData[i].vMin; break;
				case 2: pRenderParam->texOffsetStage2 = m_vecTexStageData[i].vMin; break;
				}
			}
			else
			{
				m_vecTexStageData[i].fNowAnimTime += fElapsedTime;
			}
			switch( i )
			{
			case 0:
				{
					pRenderParam->texOffsetStage0.x = m_vecTexStageData[i].vMin.x + ((m_vecTexStageData[i].vMax.x - m_vecTexStageData[i].vMin.x)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
					pRenderParam->texOffsetStage0.y = m_vecTexStageData[i].vMin.y + ((m_vecTexStageData[i].vMax.y - m_vecTexStageData[i].vMin.y)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
				}
				break;
			case 1:
				{
					pRenderParam->texOffsetStage1.x = m_vecTexStageData[i].vMin.x + ((m_vecTexStageData[i].vMax.x - m_vecTexStageData[i].vMin.x)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
					pRenderParam->texOffsetStage1.y = m_vecTexStageData[i].vMin.y + ((m_vecTexStageData[i].vMax.y - m_vecTexStageData[i].vMin.y)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
				}
				break;
			case 2:
				{
					pRenderParam->texOffsetStage2.x = m_vecTexStageData[i].vMin.x + ((m_vecTexStageData[i].vMax.x - m_vecTexStageData[i].vMin.x)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
					pRenderParam->texOffsetStage2.y = m_vecTexStageData[i].vMin.y + ((m_vecTexStageData[i].vMax.y - m_vecTexStageData[i].vMin.y)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
				}
				break;
			}
		}
		else
		{
			m_vecTexStageData[i].fNowAnimTime = 0.0f;
		}
	}

	if( m_bIsLight == true )
	{
        m_pMatrixLight->Move( m_RenderParam.lightPos );
	}

	BOOST_FOREACH( WeaponMap::value_type& mapPair, m_mapXSkinWeapon )
	{
		if ( mapPair.second != NULL )
		{
			CKTDGXRenderer::RenderParam* pEqipRenderParam = mapPair.second->m_pXSkinAnim->GetRenderParam();
			//pEqipRenderParam->worldMatrix = *m_pMatrix;
			if( pRenderParam != NULL )
			{				
				*pEqipRenderParam = m_RenderParam;
			}

			D3DXMATRIX matDX = *(mapPair.second->m_pMatrix);
			D3DXMATRIX matRot;

			D3DXMatrixRotationYawPitchRoll( &matRot, mapPair.second->m_vRotate.y, mapPair.second->m_vRotate.x, mapPair.second->m_vRotate.z );

			matDX = matRot * matDX;

			mapPair.second->m_pXSkinAnim->SetDXMatrix( matDX );
			mapPair.second->m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );
			//m_pXSkinWeapon->GetMatrix().RotateRel( m_RotWeapon );
		}
	}

    //if ( m_pXMeshAccessory != NULL )
    //{
    //    CKTDGXRenderer::RenderParam* pRenderParam = m_pRendererAccessory->GetRenderParam();
    //    *pRenderParam = m_RenderParam;
    //    pRenderParam->bAlphaBlend = true;
    //    //pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
    //    //pRenderParam->fOutLineWide	= 1.5f;
    //    //INIT_VECTOR3( m_RenderParam.lightPos, 500, 500, 500 );
    //    D3DXMATRIX localMove, matRot, matScale;
    //    D3DXMATRIX matTrans1, matTrans2, matTrans3;

    //    D3DXVECTOR3 rotDegree = D3DXVECTOR3((D3DX_PI / 180.0f) * m_RotAccessory.x, (D3DX_PI / 180.0f) * m_RotAccessory.y, (D3DX_PI / 180.0f) * m_RotAccessory.z);
    //    D3DXVECTOR3 scaleDegree = D3DXVECTOR3(m_ScaleAccessory.x / 100.0f, m_ScaleAccessory.y / 100.0f, m_ScaleAccessory.z / 100.0f);

    //    D3DXMatrixScaling(&matScale, scaleDegree.x, scaleDegree.y, scaleDegree.z);
    //    D3DXMatrixRotationYawPitchRoll( &matRot, rotDegree.y, rotDegree.x, rotDegree.z );
    //    D3DXMatrixTranslation( &localMove, m_TransAccessory.x, m_TransAccessory.y, m_TransAccessory.z );

    //    D3DXMatrixMultiply( &matTrans1, &localMove, m_pMatrixAccessory); 
    //    D3DXMatrixMultiply( &matTrans2, &matRot, &matTrans1); 
    //    D3DXMatrixMultiply( &matTrans3, &matScale, &matTrans2);

    //    pRenderParam->worldMatrix = matTrans3; 
    //}	

    //if(m_bAttachPoint)
    //{
    //    D3DXMATRIX  matScale, wMat;
    //    CKTDGXRenderer::RenderParam* pRenderParam = m_pRendererPoint->GetRenderParam();

    //    D3DXMatrixScaling(&matScale, 3.0f, 3.0f, 3.0f);
    //    D3DXMatrixMultiply(&wMat, &matScale, m_pMatrixPoint);
    //    pRenderParam->worldMatrix = wMat;
    //}

	return S_OK;
}
#pragma endregion 매 프레임 수행

#pragma region OnFrameRender
HRESULT CX2ViewerSkinMesh::OnFrameRender()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return S_OK;

	//-- Wireframe Mode
	if( m_bWireframeMode )
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	else
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	m_pXSkinAnim->OnFrameRender();  


	BOOST_FOREACH( WeaponMap::value_type& mapPair, m_mapXSkinWeapon )
	{
		if ( mapPair.second != NULL )
			mapPair.second->m_pXSkinAnim->OnFrameRender();
	}

    /*if( m_pXMeshAccessory != NULL)
    {
		CKTDGXRenderer::RenderParam renderParam;
		renderParam = m_RenderParam;
		renderParam.bAlphaBlend = true;
		D3DXMATRIX localMove, matRot, matScale;
		D3DXMATRIX matTrans1, matTrans2, matTrans3;

		D3DXVECTOR3 rotDegree = D3DXVECTOR3((D3DX_PI / 180.0f) * m_RotAccessory.x, (D3DX_PI / 180.0f) * m_RotAccessory.y, (D3DX_PI / 180.0f) * m_RotAccessory.z);
		D3DXVECTOR3 scaleDegree = D3DXVECTOR3(m_ScaleAccessory.x / 100.0f, m_ScaleAccessory.y / 100.0f, m_ScaleAccessory.z / 100.0f);

		D3DXMatrixScaling(&matScale, scaleDegree.x, scaleDegree.y, scaleDegree.z);
		D3DXMatrixRotationYawPitchRoll( &matRot, rotDegree.y, rotDegree.x, rotDegree.z );
		D3DXMatrixTranslation( &localMove, m_TransAccessory.x, m_TransAccessory.y, m_TransAccessory.z );

		D3DXMatrixMultiply( &matTrans1, &localMove, m_pMatrixAccessory); 
		D3DXMatrixMultiply( &matTrans2, &matRot, &matTrans1); 
		D3DXMatrixMultiply( &matTrans3, &matScale, &matTrans2);

        m_pRendererAccessory->OnFrameRender( renderParam, matTrans3, *m_pXMeshAccessory );
    }*/

	BOOST_FOREACH( AccessoryMap::value_type& mapPair, m_mapXMeshAccessory )
	{
		if ( mapPair.second != NULL )
		{
			CKTDGXRenderer::RenderParam renderParam;
			renderParam = m_RenderParam;
			renderParam.bAlphaBlend = true;
			D3DXMATRIX localMove, matRot, matScale;
			D3DXMATRIX matTrans1, matTrans2, matTrans3;

			D3DXVECTOR3 rotDegree = D3DXVECTOR3( (D3DX_PI / 180.0f) * mapPair.second->m_vRotate.x ,
				(D3DX_PI / 180.0f) *  mapPair.second->m_vRotate.y, (D3DX_PI / 180.0f) *  mapPair.second->m_vRotate.z);
			D3DXVECTOR3 scaleDegree = D3DXVECTOR3( mapPair.second->m_vScale.x, mapPair.second->m_vScale.y,
				mapPair.second->m_vScale.z);

			D3DXMatrixScaling( &matScale, scaleDegree.x, scaleDegree.y, scaleDegree.z );
			D3DXMatrixRotationYawPitchRoll( &matRot, rotDegree.y, rotDegree.x, rotDegree.z );
			D3DXMatrixTranslation( &localMove, mapPair.second->m_vMove.x, mapPair.second->m_vMove.y, mapPair.second->m_vMove.z );

			D3DXMatrixMultiply( &matTrans1, &localMove, mapPair.second->m_pMatrix ); 
			D3DXMatrixMultiply( &matTrans2, &matRot, &matTrans1 ); 
			D3DXMatrixMultiply( &matTrans3, &matScale, &matTrans2 );

			m_pRendererAccessory->OnFrameRender( renderParam, matTrans3, *mapPair.second->m_pXMesh );
		}
	}

	if( m_bIsBounding == true )
	{
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		
		BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollisionData, m_pXSkinAnim->GetCollisionDataList() )
		{
			if( pCollisionData->m_CollisionType == CKTDXCollision::CT_SPHERE )
			{
				m_pMatrixSphere->Move( pCollisionData->GetPointStart() );
				float fScale = pCollisionData->GetScaleRadius();
				m_pMatrixSphere->Scale( fScale, fScale, fScale );
				m_pMatrixSphere->UpdateWorldMatrix();
				m_pXMeshSphere->Render();
			}
		}
	}

	if( m_bShowAttackBox == true )
	{
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		
		BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollisionData, m_pXSkinAnim->GetAttackDataList() )
		{			
			if( pCollisionData->m_CollisionType == CKTDXCollision::CT_SPHERE )
			{
				m_pMatrixSphere->Move( pCollisionData->GetPointStart() );
				float fScale = pCollisionData->GetScaleRadius();
				m_pMatrixSphere->Scale( fScale, fScale, fScale );
				m_pMatrixSphere->UpdateWorldMatrix();
				m_pXAttackMeshSphere->Render();
			}
			else if( pCollisionData->m_CollisionType == CKTDXCollision::CT_LINE )
			{
				D3DXVECTOR3 vPos = pCollisionData->GetPointStart();
				D3DXVECTOR3 vPosDelta = pCollisionData->GetPointEnd() - pCollisionData->GetPointStart();
				float fScale = pCollisionData->GetScaleRadius();
				vPosDelta /= /*29.f /*/ fScale;
				int iNum = static_cast<int>( D3DXVec3Length( &vPosDelta ) ) - 1;
				D3DXVec3Normalize( &vPosDelta, &vPosDelta );
				vPosDelta *= fScale;
				vPos += vPosDelta;

				for( int i = 0; i < iNum; ++i )
				{
					m_pMatrixSphere->Move( vPos );
					m_pMatrixSphere->Scale( fScale, fScale, fScale );
					m_pMatrixSphere->UpdateWorldMatrix();
					m_pXAttackMeshSphere->Render();
					vPos += vPosDelta;
				}
			}
		}

		BOOST_FOREACH( WeaponMap::value_type& mapPair, m_mapXSkinWeapon )
		{
			if ( mapPair.second != NULL )
			{
				BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollisionData, mapPair.second->m_pXSkinAnim->GetAttackDataList() )
				{
					if( pCollisionData->m_CollisionType == CKTDXCollision::CT_SPHERE )
					{
						m_pMatrixSphere->Move( pCollisionData->GetPointStart() );
						float fScale = pCollisionData->GetScaleRadius();
						m_pMatrixSphere->Scale( fScale, fScale, fScale );
						m_pMatrixSphere->UpdateWorldMatrix();
						m_pXAttackMeshSphere->Render();
					}
					else if( pCollisionData->m_CollisionType == CKTDXCollision::CT_LINE )
					{
						D3DXVECTOR3 vPos = pCollisionData->GetPointStart();
						D3DXVECTOR3 vPosDelta = pCollisionData->GetPointEnd() - pCollisionData->GetPointStart();
						float fScale = pCollisionData->GetScaleRadius();
						vPosDelta /= /*29.f /*/ fScale;
						int iNum = static_cast<int>( D3DXVec3Length( &vPosDelta ) ) - 1;
						D3DXVec3Normalize( &vPosDelta, &vPosDelta );
						vPosDelta *= fScale;
						vPos += vPosDelta;

						for( int i = 0; i < iNum; ++i )
						{
							m_pMatrixSphere->Move( vPos );
							m_pMatrixSphere->Scale( fScale, fScale, fScale );
							m_pMatrixSphere->UpdateWorldMatrix();
							m_pXAttackMeshSphere->Render();
							vPos += vPosDelta;
						}			
					}
				}
			}
		}
	}

	/// 선택된 본 출력 출력
    if ( m_bAttachPoint )
    {
		D3DXMATRIX  matScale, wMat;
		CKTDGXRenderer::RenderParam renderParam;
		renderParam.color = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );

		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		
		D3DXMatrixScaling( &matScale, m_vScale.x * 2.f, m_vScale.y * 4.f, m_vScale.z * 2.f );
		D3DXMatrixMultiply( &wMat, &matScale, m_pMatrixPoint );

        m_pRendererPoint->OnFrameRender( renderParam, wMat, *m_pXMeshPoint );
    }
    

	//Light Rendering
	if ( m_bIsLight == true )
	{
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		m_pMatrixLight->UpdateWorldMatrix( CKTDGMatrix::BT_ALL );

		CKTDGStateManager::PushRenderState( D3DRS_ALPHABLENDENABLE,	true );
		m_pXMeshLight->Render();
		CKTDGStateManager::PopRenderState( D3DRS_ALPHABLENDENABLE );
	}

	return S_OK;
}
#pragma endregion 매 프레임 렌더링

#pragma region OnResetDevice
HRESULT CX2ViewerSkinMesh::OnResetDevice()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return S_OK;

	m_pXSkinAnim->OnResetDevice();
    m_pRendererAccessory->OnResetDevice();
    m_pRendererPoint->OnResetDevice();

	return S_OK;
}
#pragma endregion 매 프레임 렌더링

#pragma region OnLostDevice
HRESULT CX2ViewerSkinMesh::OnLostDevice()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return S_OK;

	m_pXSkinAnim->OnLostDevice();
	
	return S_OK;
}
#pragma endregion 장치 소실 시

#pragma region SetAnimXSkinMesh
bool CX2ViewerSkinMesh::SetAnimXSkinMesh( std::wstring fileName )
{
	//하나의 SkinMesh에 하나의 애니메이션 파일이 들어가므로 만약 삭제가
	//되어있지 않은 상황이 올경우를 대비해 삭제후 할당한다.
	//if( m_pXSkinAnim != NULL )
	//{
	//	SAFE_DELETE( m_pXSkinAnim );
	//	ERRORMSG( L"애니메이션 셋팅 상황이 잘못되었음.! 이런" );
	//}

	//m_pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
	m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

	CKTDXDeviceXSkinMesh* pMotion = NULL;
	pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

	if( pMotion != NULL )
	{
		m_pXSkinAnim->SetAnimXSkinMesh( pMotion );

		m_mapSkinMesh.insert( std::make_pair( fileName, pMotion ) );

		return true;
	}

	return false;
}
#pragma endregion 스킨 애니메이션 메쉬 설정

#pragma region InsertSkinMesh
CX2ViewerSkinMesh::SKINMESH_OPEN_TYPE CX2ViewerSkinMesh::InsertSkinMesh( std::wstring& fileName, std::wstring& dir )
{
	//g_pKTDXApp->GetDeviceManager()->CloseDevice( fileName );

	CKTDXDeviceXSkinMesh* pMotion = NULL;
	pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

	if( pMotion == NULL )
	{
		std::string strDir;
		ConvertWCHARToChar( strDir, dir.c_str() );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

		pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

		if( pMotion == NULL )
		{
			WARNINGMSG( L"Skin Mesh 파일 열기 실패~ 이런." );
			return SOT_NONE;
		}
	}

	if( pMotion->GetIsOnlyModelData() == false )
	{
		Reset();

		//m_pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
		m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

		//
		wstring xetName = fileName + L"ET";

		m_pXET = g_pKTDXApp->GetDeviceManager()->OpenXET( xetName );

		m_pXSkinAnim->SetAnimXSkinMesh( pMotion, m_pXET );
		
		// 모션 xet파일 있으면 불러들이게끔.. 변경 06 12 14 태욱

		m_pXSkinAnim->AddAnimXSkinMesh( pMotion );

		m_bIsMotion = true;

		m_mapSkinMesh.insert( std::make_pair( fileName, pMotion ) );

		//SetUnitReForm( eValue_ );

		m_pXSkinAnim->ChangeAnim( 0 );
		m_pXSkinAnim->Play( m_AnimPlaytype );

		return SOT_SKINMESH;
	}
	else
	{
		if( pMotion != NULL && m_pXSkinAnim != NULL )
		{
			if( m_pXSkinAnim->GetAnimXSkinMesh()->GetFrameNum() == pMotion->GetFrameNum() )
			{
				CKTDXDeviceXET* pModelXET = NULL;

				wstring xetName = fileName + L"ET";

				pModelXET = g_pKTDXApp->GetDeviceManager()->OpenXET( xetName );

				m_pXSkinAnim->AddModelXSkinMesh( pMotion, pModelXET, NULL, pModelXET );

				m_mapSkinMesh.insert( std::make_pair( fileName, pMotion ) );

				return SOT_MESH;
			}

            //g_pKTDXApp->GetDeviceManager()->CloseCachedXSkinMeshes();
			g_pKTDXApp->GetDeviceManager()->CloseDevice( pMotion->GetDeviceID() );

			if( MessageBox( g_pKTDXApp->GetHWND(), L"넣을수 없는 몸뚱이 메쉬만 랜더링 할까욤..?", L"Info", MB_OKCANCEL ) 
				== IDOK )
				return SOT_NOT_ADDMESH;
		}
		else
		{
            //g_pKTDXApp->GetDeviceManager()->CloseCachedXSkinMeshes();
			g_pKTDXApp->GetDeviceManager()->CloseDevice( pMotion->GetDeviceID() );
			return SOT_NOT_ADDMESH;
		}
	}

	return SOT_NONE;
}
#pragma endregion 스킨 메쉬 삽입

#pragma region AddWeapon
bool CX2ViewerSkinMesh::AddWeapon( const WCHAR* pFullFileName, const WCHAR* attachFrameName, D3DXVECTOR3 rot )
{	
	WCHAR drive[10] = L"";
	WCHAR dir[256]   = L"";
	WCHAR fname[256] = L"";
	WCHAR ext[10]    = L"";

	WCHAR fileName[256] = L"";
	WCHAR PullDir[256] = L"";
	_wsplitpath( pFullFileName, drive, dir, fname, ext);

	wcscat( fileName, fname);
	wcscat( fileName, ext);
	wcscat( PullDir, drive );
	wcscat( PullDir, dir );
	
	CKTDXDeviceXSkinMesh* pMotion = NULL;
	pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

	if( pMotion == NULL )
	{
		std::string strDir;
		ConvertWCHARToChar( strDir, PullDir );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

		pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

		if( pMotion == NULL )
		{
			WARNINGMSG( L"Skin Mesh 파일 열기 실패~ 이런." );
			return false;
		}
	}

	if ( m_pXSkinAnim == NULL )
	{
		WARNINGMSG( L"애니메이션 파일이 로드 되지 않았어요." );
		SAFE_CLOSE( pMotion );
		return false;
	}

	CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
	pFrame = m_pXSkinAnim->GetCloneFrame( attachFrameName );

	if ( pFrame == NULL )
	{
		WARNINGMSG( L"본 이름이 정확하지 않아요!" );
		SAFE_CLOSE( pMotion );
		return false;
	}
	
	//SAFE_DELETE( m_pXSkinWeapon );
	SAFE_CLOSE( m_pXETWeapon ); 
	m_pWeaponMatrix = NULL;
	
	CKTDGXSkinAnim* pXSkinWeapon = CKTDGXSkinAnim::CreateSkinAnim();

	wstring fileNameWstring = fileName;
	//
	wstring xetName = fileNameWstring + L"ET";

	m_pXETWeapon = g_pKTDXApp->GetDeviceManager()->OpenXET( xetName );

	pXSkinWeapon->SetAnimXSkinMesh( pMotion, m_pXETWeapon );
	pXSkinWeapon->AddAnimXSkinMesh( pMotion );

	m_bIsMotion = true;
	m_mapSkinMesh.insert( std::make_pair( fileName, pMotion ) );

	pXSkinWeapon->ChangeAnim( 0 );
	pXSkinWeapon->Play( CKTDGXSkinAnim::XAP_LOOP );

	m_pWeaponMatrix = &(pFrame->combineMatrix);
	pXSkinWeapon->UseDXMatrix( true );
	m_RotWeapon = rot;

	SWeaponInfo* pWeaponInfo = new SWeaponInfo();

	pWeaponInfo->m_BoneName = attachFrameName;
	pWeaponInfo->m_pMatrix = &(pFrame->combineMatrix);
	pWeaponInfo->m_vRotate = rot;
	pWeaponInfo->m_pXSkinAnim = pXSkinWeapon;
	
	m_mapXSkinWeapon.insert( std::make_pair( fileName, pWeaponInfo ) );

	return true;
}
#pragma endregion 무기 추가

#pragma region AddAccessory
bool CX2ViewerSkinMesh::AddAccessory( const WCHAR* pFullFileName, const WCHAR* attachFrameName, D3DXVECTOR3 trans )
{	
    WCHAR drive[10] = L"";
    WCHAR dir[256]   = L"";
    WCHAR fname[256] = L"";
    WCHAR ext[10]    = L"";

    WCHAR fileName[256] = L"";
    WCHAR PullDir[256] = L"";
    _wsplitpath( pFullFileName, drive, dir, fname, ext);

    wcscat( fileName, fname);
    wcscat( fileName, ext);

    wcscat( PullDir, drive );
    wcscat( PullDir, dir );

	CKTDXDeviceXMesh* pXMeshAccessory = NULL;
	
    /*if(m_pXMeshAccessory != NULL)
    {
        SAFE_CLOSE( m_pXMeshAccessory );        
    }*/

    m_pRendererAccessory->OnResetDevice();

	//m_pXMeshAccessory  = g_pKTDXApp->GetDeviceManager()->OpenXMesh( fileName );
	pXMeshAccessory = g_pKTDXApp->GetDeviceManager()->OpenXMesh( fileName );

    ///if( m_pXMeshAccessory == NULL )
	if( pXMeshAccessory == NULL )
    {
        std::string strDir;
        ConvertWCHARToChar( strDir, PullDir );
        g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

        //m_pXMeshAccessory = g_pKTDXApp->GetDeviceManager()->OpenXMesh( fileName );
		pXMeshAccessory = g_pKTDXApp->GetDeviceManager()->OpenXMesh( fileName );

        //if( m_pXMeshAccessory == NULL )
		if( pXMeshAccessory == NULL )
        {
            WARNINGMSG( L"Mesh 파일 열기 실패~ 이런." );
            return false;
        }
    }
    
    if ( m_pXSkinAnim == NULL )
    {
        WARNINGMSG( L"애니메이션 파일이 로드 되지 않았어요." );
        //SAFE_CLOSE( m_pXMeshAccessory );
		SAFE_CLOSE( pXMeshAccessory );
        return false;
    }

    CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
    pFrame = m_pXSkinAnim->GetCloneFrame( attachFrameName );
    
    if ( pFrame == NULL )
    {
        WARNINGMSG( L"본 이름이 정확하지 않아요!" );
        //SAFE_CLOSE( m_pXMeshAccessory );
		SAFE_CLOSE( pXMeshAccessory );
        return false;
    }    

    m_bIsMotion = true;

    //m_pMatrix = &(pFrame->combineMatrix);
    //m_pMatrixAccessory = &(pFrame->combineMatrix);
	//m_TransAccessory = trans;

	SAccessoryInfo* pAccessoryInfo = new SAccessoryInfo();

	pAccessoryInfo->m_BoneName = attachFrameName;
	pAccessoryInfo->m_pMatrix = &(pFrame->combineMatrix);
	pAccessoryInfo->m_pXMesh= pXMeshAccessory;

	m_mapXMeshAccessory.insert( std::make_pair( fileName, pAccessoryInfo ) );
	
    return true;
}
#pragma endregion 악세서리 추가

#pragma region DelModelXSkinAnim
bool CX2ViewerSkinMesh::DelModelXSkinAnim( std::wstring fileName )
{
	WeaponMap::iterator itr;
	itr = m_mapXSkinWeapon.find( fileName );

	if( itr != m_mapXSkinWeapon.end() )
	{
		m_mapXSkinWeapon.erase( itr );

		return true;
	}

	return false;
}
#pragma endregion 모델 스킨 메쉬 제거

#pragma region DelModelXMesh
bool CX2ViewerSkinMesh::DelModelXMesh( std::wstring fileName )
{
	AccessoryMap::iterator itr;
	itr = m_mapXMeshAccessory.find( fileName );

	if( itr != m_mapXMeshAccessory.end() )
	{
		m_mapXMeshAccessory.erase( itr );

		return true;
	}

	return false;
}
#pragma endregion 모델 스킨 메쉬 제거

#pragma region DelModelXSkinMesh
bool CX2ViewerSkinMesh::DelModelXSkinMesh( std::wstring fileName )
{
	std::map<std::wstring, CKTDXDeviceXSkinMesh*>::iterator itr;
	itr = m_mapSkinMesh.find( fileName );

	if( itr != m_mapSkinMesh.end() )
	{
		m_pXSkinAnim->RemoveModelXSkinMesh( itr->second );
		g_pKTDXApp->GetDeviceManager()->CloseDevice( itr->first );

		m_mapSkinMesh.erase( itr );

		return true;
	}

	return false;
}
#pragma endregion 모델 스킨 메쉬 제거

#pragma region Reset
void CX2ViewerSkinMesh::Reset()
{
	if( m_bIsMotion == true && m_pXSkinAnim != NULL )
	{
		m_pXSkinAnim->Stop();
		m_pXSkinAnim->RemoveModelXSkinMesh( m_pXSkinAnim->GetAnimXSkinMesh() );
	}
	
	std::map<std::wstring, CKTDXDeviceXSkinMesh*>::iterator itr;
	for( itr = m_mapSkinMesh.begin(); itr != m_mapSkinMesh.end(); ++itr )
	{
		m_pXSkinAnim->Stop();
		m_pXSkinAnim->RemoveModelXSkinMesh( itr->second );
		g_pKTDXApp->GetDeviceManager()->CloseDevice( itr->first );
	}
	m_mapSkinMesh.clear();

	BOOST_FOREACH( WeaponMap::value_type& mapPair, m_mapXSkinWeapon )
	{
		if ( mapPair.second != NULL )
		{
			delete mapPair.second;
		}
	}
	m_mapXSkinWeapon.clear();

	//SAFE_DELETE( m_pXSkinAnim );
	//SAFE_DELETE( m_pXSkinWeapon );
    //SAFE_CLOSE( m_pXMeshAccessory );

	BOOST_FOREACH( AccessoryMap::value_type& mapPair, m_mapXMeshAccessory )
	{
		if ( mapPair.second != NULL )
		{
			SAFE_CLOSE( mapPair.second->m_pXMesh );
			delete mapPair.second;
		}
	}
	m_mapXMeshAccessory.clear();
    
	SAFE_CLOSE( m_pXET );
	SAFE_CLOSE( m_pXETWeapon ); 

	INIT_VECTOR3( m_vScale,	1.0f, 1.0f, 1.0f );
	INIT_VECTOR3( m_vPosition,	0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_RenderParam.lightPos, 300, 300, -500 );

	m_pWeaponMatrix = NULL;
}
#pragma endregion 재설정

#pragma region ChangeAnim
void CX2ViewerSkinMesh::ChangeAnim( std::wstring animName )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->ChangeAnim( animName.c_str(), false );
	m_pXSkinAnim->Play( m_AnimPlaytype );
}
#pragma endregion 애니메이션 변경_String

#pragma region ChangeAnim
void CX2ViewerSkinMesh::ChangeAnim( int index )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->ChangeAnim( index, false );
	m_pXSkinAnim->Play( m_AnimPlaytype );
}
#pragma endregion 애니메이션 변경_Index

#pragma region SetPlayType
void CX2ViewerSkinMesh::SetPlayType( WCHAR* wszPlayType )
{
	if( wcscmp( wszPlayType, L"ONE" ) == 0 )
	{
		m_AnimPlaytype = CKTDGXSkinAnim::XAP_ONE_WAIT;
	}
	else if( wcscmp( wszPlayType, L"LOOP" ) == 0 )
	{
		m_AnimPlaytype = CKTDGXSkinAnim::XAP_LOOP;
	}

	m_pXSkinAnim->Play( m_AnimPlaytype );
}
#pragma endregion 재생 방식 변경

#pragma region SetScale
void CX2ViewerSkinMesh::SetScale( float fX, float fY, float fZ )
{
	D3DXVECTOR3 vScale;
	vScale.x = fX;
	vScale.y = fY;
	vScale.z = fZ;

	SetScale( vScale );
}
#pragma endregion 비율 설정

#pragma region SetLightPos
void CX2ViewerSkinMesh::SetLightPos( float fX, float fY, float fZ )
{
	D3DXVECTOR3 vLightPos;
	vLightPos.x = fX;
	vLightPos.y = fY;
	vLightPos.z = fZ;

	SetLightPos( vLightPos );
}
#pragma endregion 빛 위치 설정

#pragma region GetFrameNameList
bool CX2ViewerSkinMesh::GetFrameNameList( VecMultiAnimFrame& vecFrameNameList )
{
    if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
    {
        return false;
    }   
    
    LPCSTR			szName;
    std::wstring	wstrName;
    WCHAR			wszName[128] = L"";
    string          frameName;

    CKTDXDeviceXSkinMesh *pSM = m_pXSkinAnim->GetAnimXSkinMesh();   
	
    for( UINT i = 0; i < pSM->GetFrameNum(); ++i )
    {
        CKTDXDeviceXSkinMesh::MultiAnimFrame *pFM = pSM->GetFrame(i);
        szName = pFM->Name;

        MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);
        
        wstrName = wszName;

        if( wstrName.compare(L"Scene_Root") != 0 && wstrName.compare(L"") != 0 && wstrName.compare(L"<no_name>") != 0 )
            vecFrameNameList.push_back( pFM );
    }

    return true;
}
#pragma endregion 뼈대 리스트 얻기

#pragma region GetAnimNameList
bool CX2ViewerSkinMesh::GetAnimNameList( std::vector<std::wstring>& vecAnimNameList )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
	{
		return false;
	}

	LPD3DXANIMATIONCONTROLLER	pAC;
	LPD3DXANIMATIONSET			pAnimSet;
	UINT			aniNum;
	LPCSTR			szName;
	std::wstring	wstrName;
	WCHAR			wszName[128] = L"";

	pAC		= m_pXSkinAnim->GetAnimXSkinMesh()->GetCloneAC();
	aniNum	= pAC->GetNumAnimationSets();

	for( UINT i = 0; i < aniNum; ++i )
	{
		pAC->GetAnimationSet( i, &pAnimSet );
		szName = pAnimSet->GetName();

		MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);

		wstrName = wszName;
		vecAnimNameList.push_back( wstrName );

		pAnimSet->Release();
	}

	SAFE_RELEASE( pAC);

	return true;
}
#pragma endregion 애니메이션 리스트 얻기

#pragma region SetPlayOnOff
bool CX2ViewerSkinMesh::SetAnimPlayPause()
{
	m_bIsAnimPlay = !m_bIsAnimPlay;

	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return false;

	if( m_bIsAnimPlay == true )
	{
		m_pXSkinAnim->Play( m_AnimPlaytype );
	}
	else
	{
		m_pXSkinAnim->Wait();
	}

	return m_bIsAnimPlay;
}
#pragma endregion 재생/일시정지 설정

#pragma region SetAnimStop
void CX2ViewerSkinMesh::SetAnimStop()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_bIsAnimPlay = false;
	m_pXSkinAnim->Reset( 0.f );
	m_pXSkinAnim->SetAnimationTime( 0.f );
	m_pXSkinAnim->Wait();
}
#pragma endregion 

#pragma region SetMotionOnOff
void CX2ViewerSkinMesh::SetMotionOnOff( bool bIsMotion )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
	{
		return;
	}

	if( m_bIsMotion == false && bIsMotion == true )
	{
		m_pXSkinAnim->AddAnimXSkinMesh( m_pXSkinAnim->GetAnimXSkinMesh() );
		m_bIsMotion = true;
	}
	else if( m_bIsMotion == true && bIsMotion == false )
	{
		m_pXSkinAnim->RemoveModelXSkinMesh( m_pXSkinAnim->GetAnimXSkinMesh() );
		m_bIsMotion = false;
	}
}
#pragma endregion 모션 메쉬 켜기/끄기 설정

#pragma region SetPlaySpeed
void CX2ViewerSkinMesh::SetPlaySpeed( float fPlaySpeed )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
	{
		return;
	}

	m_pXSkinAnim->SetPlaySpeed( fPlaySpeed );
}
#pragma endregion 재생 속도 설정

#pragma region GetAnimTime
void CX2ViewerSkinMesh::GetAnimTime( float& fNowTime, float& fMaxTime )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
	{
		fNowTime = fMaxTime = -1.0f;
		return;
	}

	fNowTime = m_pXSkinAnim->GetNowAnimationTime();
	fMaxTime = m_pXSkinAnim->GetMaxAnimationTime();
}
#pragma endregion 애니메이션 시간 얻기

#pragma region SetAnimTime
void CX2ViewerSkinMesh::SetAnimTime( float fTime )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
	{
		return;
	}

	m_pXSkinAnim->SetAnimationTime( fTime );
}
#pragma endregion 애니메이션 시간 설정

#pragma region MsgProc
bool CX2ViewerSkinMesh::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#if 0 
    switch(uMsg)
    {
    case WM_LBUTTONDOWN:        
        SelectionChange(LOWORD(lParam), HIWORD(lParam));
        break;
    }
#endif
    return false;
}
#pragma endregion 메시지 처리

/*void CX2ViewerSkinMesh::SelectionChange( DWORD dwX,  DWORD dwY)
{
    D3DVIEWPORT9 Viewport;
   D3DXMATRIX matProjection, matView, matWorld;

   float fDistMin= 0.0f;
   DWORD dwFaceHit= 0;
   DWORD dwVertHit= 0;

   g_pKTDXApp->GetDevice()->GetViewport(&Viewport);
   g_pKTDXApp->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
   g_pKTDXApp->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);

   
   CKTDXDeviceXSkinMesh *pRoot = m_pXSkinAnim->GetAnimXSkinMesh();

   for( UINT i = 0; i < pRoot->GetFrameNum(); ++i )
   {
       CKTDXDeviceXSkinMesh::MultiAnimFrame *pdeCur = pRoot->GetFrame(i);


       if (pdeCur->pMeshContainer != NULL && 
           BIntersectMeshContainer((CKTDXDeviceXSkinMesh::MultiAnimMC *)pdeCur->pMeshContainer, dwX, dwY, &Viewport, &matProjection, &matView, &fDistMin, &dwFaceHit, &dwVertHit))
       {
           LPCSTR			szName;
           std::wstring	wstrName;
           WCHAR			wszName[128] = L"";

           szName = pdeCur->Name;

           MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);

           wstrName = wszName;

           if( wstrName.compare(L"Scene_Root") != 0 && wstrName.compare(L"") != 0)
           {                   
               MessageBox(NULL, wstrName.c_str(), L"Name", MB_OK);
           }
       }
   }
}




BOOL CX2ViewerSkinMesh::BIntersectMeshContainer( CKTDXDeviceXSkinMesh::MultiAnimMC *pmcMesh, DWORD dwX, DWORD dwY,
	D3DVIEWPORT9 *pViewport, D3DXMATRIX *pmatProjection, D3DXMATRIX *pmatView, float *pfDistMin, //SMeshContainer **ppmcHit,
	DWORD *pdwFaceHit, DWORD *pdwVertHit )
{
   // 충돌검사하자
   HRESULT hr = S_OK;
   D3DXVECTOR3 vProjected;
   D3DXVECTOR3 vRayPos;
   D3DXVECTOR3 vRayDirection;
   float fDist= 0.0f;
   BOOL bHit= FALSE;
   BOOL bFound = FALSE;
   float fRayLength= 0.0f;
   DWORD dwFace= 0;
   float fU= 0.0f, fV= 0.0f;
   DWORD iVertexIndex= 0;
   PBYTE pbIndices= NULL;
   LPD3DXBASEMESH pMeshCur= NULL;
   PBYTE       pbVerticesSrc= NULL;
   PBYTE       pbVerticesDest= NULL;
   D3DXMATRIXA16* rgBoneMatrices= NULL;            


   if(pmcMesh == NULL)
       return false;

   LPD3DXMESH pWorkingMesh;

   
   DWORD dwOldFVF;
   
   if( pmcMesh->m_pWorkingMesh == NULL)
       return false;

   D3DVERTEXELEMENT9 Decl[MAX_FVF_DECL_SIZE];
   pmcMesh->m_pWorkingMesh->GetDeclaration(Decl);
   hr = pmcMesh->m_pWorkingMesh->CloneMesh(D3DXMESH_SYSTEMMEM, Decl,
       g_pKTDXApp->GetDevice(), &pWorkingMesh);


   if (pmcMesh != NULL)
   {
       //pmcMesh->m_bSelected = false;

       // calculate ray position in world space
       vProjected = D3DXVECTOR3((float)dwX, (float)dwY, 0.0f);
       D3DXVec3Unproject(&vRayPos, &vProjected, pViewport, pmatProjection, pmatView, NULL);


       // calculate ray direction in world space
       vProjected = D3DXVECTOR3((float)dwX, (float)dwY, 1.0f);
       D3DXVec3Unproject(&vRayDirection, &vProjected, pViewport, pmatProjection, pmatView, NULL);
       vRayDirection -= vRayPos;


       // get the bone count

       DWORD   cBones  = pmcMesh->pSkinInfo->GetNumBones();


       // allocate bone transform array

       rgBoneMatrices  = new D3DXMATRIXA16[cBones];

       if (!rgBoneMatrices)
           return FALSE;


       // set up bone transforms

       for (DWORD iBone = 0; iBone < cBones; ++iBone)
       {
           D3DXMatrixMultiply( &rgBoneMatrices[iBone],
               &( pmcMesh->m_amxBoneOffsets[ iBone ] ),  
               &pmcMesh->m_ppBoneFrames[ iBone ]->combineMatrix );
          
       }

       hr= pmcMesh->m_pWorkingMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);
       if (FAILED(hr))
       {
           pmcMesh->m_pWorkingMesh->UnlockVertexBuffer();
           return false;
       }
       hr= pWorkingMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);
       if (FAILED(hr))
       {
           pmcMesh->m_pWorkingMesh->UnlockVertexBuffer();
           pWorkingMesh->UnlockVertexBuffer();
           return false;
       }
       // generate skinned mesh, use the system memory copy
       hr = pmcMesh->pSkinInfo->UpdateSkinnedMesh(rgBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest);
       pmcMesh->m_pWorkingMesh->UnlockVertexBuffer();
       pWorkingMesh->UnlockVertexBuffer();
       if (FAILED(hr))
       {
           delete[] rgBoneMatrices;
           rgBoneMatrices= NULL;
           return false;
       }

       // free bone transform array
       delete[] rgBoneMatrices;
       rgBoneMatrices= NULL;

       // perform ray-mesh intersection
       hr = D3DXIntersect(
           pWorkingMesh,
           &vRayPos, 
           &vRayDirection, 
           &bHit, 
           &dwFace,
           &fU,
           &fV,
           &fDist,
           NULL,
           NULL);
       if (FAILED(hr))
           return FALSE;

       if (bHit)       // intersection found
       {
#if 1 
           // normalize intersection distance
           fDist  /= D3DXVec3Length(&vRayDirection);

           if (fDist < *pfDistMin)     // intersection distance is the smallest seen so far
           {
               // update smallest intersection distance & intersected frame
               *pfDistMin  = fDist;
               //*ppmcHit = pmcMesh;
               *pdwFaceHit = dwFace;                    

               // indicate that we found a new smallest intersection distance
               bFound      = TRUE;

               //pmcMesh->m_bSelected = true;
               return TRUE;
           }
#else
           bFound      = TRUE;
           return TRUE;
#endif
           //return FALSE;            
       }
   }

}
*/

#pragma region SetAttachPoint
void CX2ViewerSkinMesh::SetAttachPoint( WCHAR *szName )
{
    CKTDXDeviceXSkinMesh::MultiAnimFrame *pFrame;
    wstring wstrName = szName;

    if(wstrName.compare(L"") == 0)
    {
        m_bAttachPoint = false;
        return;
    }

    m_pRendererPoint->OnResetDevice();
    pFrame		= m_pXSkinAnim->GetCloneFrame( szName );

    if ( pFrame == NULL )
    {
        WARNINGMSG( L"본 이름이 정확하지 않아요!" );
        SAFE_CLOSE( m_pXMeshPoint );
        return;
    }                   

	//m_vScale = pFrame->m_vScaleRel;
    m_bAttachPoint = true;
    //m_pMatrix = &(pFrame->combineMatrix);
    m_pMatrixPoint = &(pFrame->combineMatrix); 
}
#pragma endregion 붙이는 위치 설정

#pragma region OpenScriptFile
bool CX2ViewerSkinMesh::OpenScriptFile( IN const WCHAR* pFileName_, KLuabinder* pKLuabinder )
{
	if ( pKLuabinder == NULL )
		pKLuabinder = g_pKTDXApp->GetLuaBinder();

	lua_tinker::decl( pKLuabinder->GetLuaState(), "g_pViewerSkinMesh", this );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName_ );
	if ( Info == NULL )
	{
		ERRORMSG( L"lua 파일열기 실패" );
		return false;
	}

	if( pKLuabinder->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		ERRORMSG( L"lua 파싱 실패" );
		return false;
	}

	return true;
}
#pragma endregion g_pViewerSkinMesh 함수를 사용하는 스크립트 파일 로딩

#pragma region RefreshBoneInfoScript
void CX2ViewerSkinMesh::RefreshBoneInfoScript( KLuabinder* pKLuabinder )
{
	OpenScriptFile( L"DefaultBoneInfo.lua", pKLuabinder );
}
#pragma endregion 스크립트 파일 다시 읽기

#pragma region AddBoneInfo
bool CX2ViewerSkinMesh::AddBoneInfo()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	SUnitBoneModify* pUnitBoneModify = new SUnitBoneModify();

	LUA_GET_VALUE_RETURN_ENUM( luaManager, L"UNIT_ID", pUnitBoneModify->m_eUnitID, CHARACTER_ID, CI_NONE,
		SAFE_DELETE( pUnitBoneModify ); return false; );

	int index = 1;

	while( true == luaManager.BeginTable( L"BONE_MODIFY", index ) )
	{
		SBoneInfo* pBoneInfo = new SBoneInfo();

		LUA_GET_VALUE( luaManager, 1, pBoneInfo->m_strBoneName, L"" );
		LUA_GET_VALUE_ENUM( luaManager, 2, pBoneInfo->m_eModifyBoneStyle, MODIFY_BONE_STYLE, MBS_NONE );
				
		if ( pBoneInfo->m_eModifyBoneStyle == MBS_MOVE )
		{
			LUA_GET_VALUE( luaManager, 3, pBoneInfo->m_vValue.x, 0.f );
			LUA_GET_VALUE( luaManager, 4, pBoneInfo->m_vValue.y, 0.f );
			LUA_GET_VALUE( luaManager, 5, pBoneInfo->m_vValue.z, 0.f );
		}
		else if ( pBoneInfo->m_eModifyBoneStyle == MBS_SCALE )
		{
			LUA_GET_VALUE( luaManager, 3, pBoneInfo->m_vValue.x, 1.f );
			LUA_GET_VALUE( luaManager, 4, pBoneInfo->m_vValue.y, 1.f );
			LUA_GET_VALUE( luaManager, 5, pBoneInfo->m_vValue.z, 1.f );
		}

		pUnitBoneModify->m_vecModifyBoneInfo.push_back( pBoneInfo );

		++index;
		luaManager.EndTable();
	}

	m_vUnitBoneModify.push_back( pUnitBoneModify );

	return true;
}
#pragma endregion 기본 뼈대 변환 정보를 추가

#pragma region SetUnitReForm
void CX2ViewerSkinMesh::SetUnitReForm( CHARACTER_ID eValue_/* = CI_NONE*/ )
{
	if( m_pXSkinAnim == NULL )
		return;
	
	BOOST_FOREACH( SUnitBoneModify* pUnitBoneModifyData, m_vUnitBoneModify )
	{
		if ( pUnitBoneModifyData->m_eUnitID == eValue_ )
		{
			BOOST_FOREACH( SBoneInfo* pBoneInfo, pUnitBoneModifyData->m_vecModifyBoneInfo )
			{
				if ( pBoneInfo->m_eModifyBoneStyle == MBS_MOVE )
				{
					SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( pBoneInfo->m_strBoneName.c_str() ), pBoneInfo->m_vValue.y );
				}
				else if ( pBoneInfo->m_eModifyBoneStyle == MBS_SCALE )
				{
					CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = m_pXSkinAnim->GetCloneFrame( pBoneInfo->m_strBoneName.c_str() );

					if ( pFrame )
						SetFrameScale( &(pBoneInfo->m_vValue), true, pFrame );
				}
			}
		}
	}

#ifdef DONT
	D3DXVECTOR3 vHead = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXVECTOR3 vThigh = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXVECTOR3 vCalf = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXVECTOR3 vFoot = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXVECTOR3 vHand = D3DXVECTOR3(1.f, 1.f, 1.f);	
	D3DXVECTOR3 vSpine = D3DXVECTOR3(1.f, 1.f, 1.f);
	float fScale = 1.f;

	#pragma region MoveBoneY
	switch( eValue_ )
	{
	case CI_ELSWORD:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 2.f );
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -2.f );
		}
		break;

	case CI_SWORD_KNIGHT:
	case CI_MAGIC_KNIGHT:
	case CI_SHEATH_KNIGHT:
	case CI_LORD_KNIGHT:
	case CI_RUNE_SLAYER:
	case CI_INFINITE_SWORD:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 10.f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -10.f);
		}
		break;

	case CI_ELEMENTAL_MASTER:
	case CI_VOID_PRINCESS:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 3.f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -3.f);
		}
		break;

	case CI_LENA:
	case CI_COMBAT_RANGER:
	case CI_SNIPING_RANGER:
	case CI_TRAPPING_RANGER:
	case CI_WIND_SNIKER:
	case CI_GRAND_ARCHER:
	case CI_NIGHT_WATCHER:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 1.0f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -1.0f);
		}
		break;

	case CI_RAVEN:
	case CI_SWORD_TAKER:
	case CI_OVER_TAKER:
	case CI_WEAPON_TAKER:
	case CI_BLADE_MASTER:
	case CI_RECKRESS_FIST:
	case CI_VETERAN_COMMANDER:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 7.f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -7.f);
		}
		break;

	case CI_EVE:
	case CI_EXOTIC_GEAR:
	case CI_ARCHTECTURE:
	case CI_ELECTRA:
	case CI_CODE_NEMESIS:
	case CI_CODE_EMPRESS:
	case CI_BATTLE_SERAPH:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), -0.8f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), 0.8f);
		}
		break;

	default:
		break;
	}
	#pragma endregion 특정 본 Y 축 변경

	#pragma region ScaleBone
	switch( eValue_ )
	{
	case CI_ELSWORD:
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(1.f, 0.95f, 0.97f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.f, 1.f, 1.f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.05f, 1.f, 1.f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.9f, 1.f, 0.9f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );

			if ( pFrameHead != NULL )
				SetFrameScale( &vHead, true, pFrameHead );

			if ( pFrame_R_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_R_Leg );

			if ( pFrame_L_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_L_Leg );

			if ( pFrame_R_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_R_Calf );

			if ( pFrame_L_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_L_Calf );

			if ( pFrame_R_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_R_Foot );

			if ( pFrame_L_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_L_Foot );	
		}
		break;

	case CI_SWORD_KNIGHT:
	case CI_MAGIC_KNIGHT:
	case CI_SHEATH_KNIGHT:
	case CI_LORD_KNIGHT:
	case CI_RUNE_SLAYER:
	case CI_INFINITE_SWORD:
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(1.f, 0.95f, 0.97f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.07f, 1.f, 1.f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.12f, 1.f, 1.f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.9f, 1.f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
			
			if ( pFrameHead != NULL )
				SetFrameScale( &vHead, true, pFrameHead );

			if ( pFrame_R_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_R_Leg );

			if ( pFrame_L_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_L_Leg );

			if ( pFrame_R_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_R_Calf );

			if ( pFrame_L_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_L_Calf );

			if ( pFrame_R_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_R_Foot );

			if ( pFrame_L_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_L_Foot );
		}
		break;

	case CI_ELEMENTAL_MASTER:
	case CI_VOID_PRINCESS:
	case CI_DIMENSION_WITCH:
		{
			D3DXVECTOR3 vHead	= D3DXVECTOR3(0.97f, 0.95f, 0.95f);
			D3DXVECTOR3 vThigh	= D3DXVECTOR3(1.f, 1.f, 1.f);
			D3DXVECTOR3 vCalf	= D3DXVECTOR3(1.06f, 1.f, 1.f);
			D3DXVECTOR3 vFoot	= D3DXVECTOR3(1.f, 1.f, 1.f);
			D3DXVECTOR3 vHand	= D3DXVECTOR3(0.9f, 0.93f, 0.93f);	
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Hand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
			
			if ( pFrameHead != NULL )
				SetFrameScale( &vHead, true, pFrameHead );

			if ( pFrame_R_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_R_Leg );

			if ( pFrame_L_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_L_Leg );

			if ( pFrame_R_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_R_Calf );

			if ( pFrame_L_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_L_Calf );

			if ( pFrame_R_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_R_Foot );

			if ( pFrame_L_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_L_Foot );

			if ( pFrame_R_Hand != NULL )
				SetFrameScale( &vHand, true, pFrame_R_Hand );

			if ( pFrame_L_Hand != NULL )
				SetFrameScale( &vHand, true, pFrame_L_Hand );

			vDummyHand.x = 1.f / vHand.x;
			vDummyHand.y = 1.f / vHand.y;
			vDummyHand.z = 1.f / vHand.z;

			if ( pFrame_R_Dummy != NULL )
				SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );

			if ( pFrame_L_Dummy != NULL )
				SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );
		}
		break;

	case CI_LENA:
	case CI_COMBAT_RANGER:
	case CI_SNIPING_RANGER:
	case CI_TRAPPING_RANGER:
	case CI_WIND_SNIKER:
	case CI_GRAND_ARCHER:
	case CI_NIGHT_WATCHER:
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(0.96f, 0.94f, 0.92f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.02f, 1.f, 1.f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.02f, 1.f, 1.f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.93f, 0.93f, 0.93f);
			D3DXVECTOR3 vHand = D3DXVECTOR3(0.9f, 0.9f, 0.9f);
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Hand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );

			if ( pFrameHead != NULL )
				SetFrameScale( &vHead, true, pFrameHead );

			if ( pFrame_R_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_R_Leg );

			if ( pFrame_L_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_L_Leg );

			if ( pFrame_R_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_R_Calf );

			if ( pFrame_L_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_L_Calf );

			if ( pFrame_R_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_R_Foot );

			if ( pFrame_L_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_L_Foot );

			if ( pFrame_R_Hand != NULL )
				SetFrameScale( &vHand, true, pFrame_R_Hand );

			if ( pFrame_L_Hand != NULL )
				SetFrameScale( &vHand, true, pFrame_L_Hand );

			vDummyHand.x = 1.f / vHand.x;
			vDummyHand.y = 1.f / vHand.y;
			vDummyHand.z = 1.f / vHand.z;

			if ( pFrame_R_Dummy != NULL )
				SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );

			if ( pFrame_L_Dummy != NULL )
				SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );
		}
		break;

	case CI_RAVEN:
	case CI_SWORD_TAKER:
	case CI_OVER_TAKER:
	case CI_WEAPON_TAKER:
	case CI_BLADE_MASTER:
	case CI_RECKRESS_FIST:
	case CI_VETERAN_COMMANDER:
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(0.9f, 0.92f, 0.94f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.05f, 0.98f, 0.98f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.05f, 1.f, 1.f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.95f, 0.95f, 0.85f);
			D3DXVECTOR3 vSpine = D3DXVECTOR3(0.93f, 0.97f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameSpine = m_pXSkinAnim->GetCloneFrame( L"Bip01_Spine" );

			if ( pFrameHead != NULL )
				SetFrameScale( &vHead, true, pFrameHead );

			if ( pFrame_R_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_R_Leg );

			if ( pFrame_L_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_L_Leg );

			if ( pFrame_R_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_R_Calf );

			if ( pFrame_L_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_L_Calf );

			if ( pFrame_R_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_R_Foot );

			if ( pFrame_L_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_L_Foot );

			if ( pFrameSpine != NULL )
				SetFrameScale( &vSpine, false, pFrameSpine );
		}
		break;

	case CI_EVE:
	case CI_EXOTIC_GEAR:
	case CI_ARCHTECTURE:
	case CI_ELECTRA:
	case CI_CODE_NEMESIS:
	case CI_CODE_EMPRESS:
	case CI_BATTLE_SERAPH:
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(0.94f, 0.97f, 0.98f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.0f, 0.98f, 0.98f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.0f, 0.98f, 0.98f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.95f, 0.95f, 0.95f);
			D3DXVECTOR3 vHand = D3DXVECTOR3(0.9f, 0.96f, 0.96f);
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Hand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );

			if ( pFrameHead != NULL )
				SetFrameScale( &vHead, true, pFrameHead );

			if ( pFrame_R_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_R_Leg );

			if ( pFrame_L_Leg != NULL )
				SetFrameScale( &vThigh, true, pFrame_L_Leg );

			if ( pFrame_R_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_R_Calf );

			if ( pFrame_L_Calf != NULL )
				SetFrameScale( &vCalf, true, pFrame_L_Calf );

			if ( pFrame_R_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_R_Foot );

			if ( pFrame_L_Foot != NULL )
				SetFrameScale( &vFoot, true, pFrame_L_Foot );

			if ( pFrame_R_Hand != NULL )
				SetFrameScale( &vHand, true, pFrame_R_Hand );

			if ( pFrame_L_Hand != NULL )
				SetFrameScale( &vHand, true, pFrame_L_Hand );

			vDummyHand.x = 1.f;
			vDummyHand.y = 1.f;
			vDummyHand.z = 1.f;

			if ( pFrame_R_Dummy != NULL )
				SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );

			if ( pFrame_L_Dummy != NULL )
				SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );
		}
		break;

	case CI_IRON_PALADIN:
	case CI_DEADLY_CHASER:
	case CI_TATICAL_TROOPER:
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(0.93f, 0.96f, 0.96f);
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);
			float fScale = 1.13f;

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );

			vDummyHand = vDummyHand / fScale;

			if ( pFrameHead != NULL )
				SetFrameScale( &vHead, true, pFrameHead );

			if ( pFrame_R_Dummy != NULL )
				SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );

			if ( pFrame_L_Dummy != NULL )
				SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );
		}

	default:
		break;
	}
	#pragma endregion 특정 본 비율 변경
#endif
}
#pragma endregion Unit 형태 변경

#pragma region GetModifyBoneList
void CX2ViewerSkinMesh::GetModifyBoneList( CHARACTER_ID eValue_, VecMultiAnimFrame &vecBoneList_ )
{
	if( m_pXSkinAnim == NULL )
		return;

	BOOST_FOREACH( SUnitBoneModify* pUnitBoneModifyData, m_vUnitBoneModify )
	{
		if ( pUnitBoneModifyData->m_eUnitID == eValue_ )
		{
			BOOST_FOREACH( SBoneInfo* pBoneInfo, pUnitBoneModifyData->m_vecModifyBoneInfo )
			{
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pMAF = m_pXSkinAnim->GetCloneFrame( pBoneInfo->m_strBoneName.c_str() );

				if ( NULL != pMAF )
					vecBoneList_.push_back( pMAF );
			}
		}
	}
}
#pragma endregion 변경된 Bone List 얻음

#pragma region GetWeaponXSkinAnim
CKTDGXSkinAnim* CX2ViewerSkinMesh::GetWeaponXSkinAnim( wstring strName_ )
{
	WeaponMap::iterator itr;
	itr = m_mapXSkinWeapon.find( strName_ );

	if( itr != m_mapXSkinWeapon.end() )
	{
		return itr->second->m_pXSkinAnim;
	}

	return NULL;
}
#pragma endregion 

#pragma region GetWeaponInfo
SWeaponInfo* CX2ViewerSkinMesh::GetWeaponInfo( wstring strName_ )
{
	WeaponMap::iterator itr;
	itr = m_mapXSkinWeapon.find( strName_ );

	if( itr != m_mapXSkinWeapon.end() )
	{
		return itr->second;
	}

	return NULL;
}
#pragma endregion 

#pragma region GetAccessoryXMesh
CKTDXDeviceXMesh* CX2ViewerSkinMesh::GetAccessoryXMesh( wstring strName_ )
{
	AccessoryMap::iterator itr;
	itr = m_mapXMeshAccessory.find( strName_ );

	if( itr != m_mapXMeshAccessory.end() )
	{
		return itr->second->m_pXMesh;
	}

	return NULL;
}
#pragma endregion 

#pragma region GetAccessoryInfo
SAccessoryInfo* CX2ViewerSkinMesh::GetAccessoryInfo( wstring strName_ )
{
	AccessoryMap::iterator itr;
	itr = m_mapXMeshAccessory.find( strName_ );

	if( itr != m_mapXMeshAccessory.end() )
	{
		return itr->second;
	}

	return NULL;
}
#pragma endregion 악세 정보 얻기