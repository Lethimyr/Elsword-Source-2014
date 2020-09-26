#include "dxstdafx.h"
#include "CSkinAnimObject.h"

CSkinAnimObject::~CSkinAnimObject(void)
{
	Clear();

#ifdef SHOW_ATTACK_BOX_DUMMY
	if( NULL != m_pXMeshSphere &&
		NULL != g_pKTDXApp &&
		NULL != g_pKTDXApp->GetDeviceManager() )
	{
		g_pKTDXApp->GetDeviceManager()->CloseDevice( m_pXMeshSphere->GetDeviceID() );
		SAFE_DELETE( m_pMatrixSphere );
	}
#endif // SHOW_ATTACK_BOX_DUMMY

}

CSkinAnimObject::CSkinAnimObject(void)
{
	m_pXSkinAnim		= NULL;
	m_bShow				= true;

	m_iNowAniNameIndex		= 0;

	//m_pXET			= NULL;
	//m_pXETWeapon		= NULL;
	//m_bWireframeMode	= false;
	//m_bIsLight		= true;
	//m_bIsAnimPlay		= true;
	//m_bIsMotion		= false;
	//m_ImpactNowAnimTime	= 0.0f;
	//m_bIsBounding		= false;
	//m_bShowAttackBox	= false;

	//m_AnimPlaytype	= CKTDGXSkinAnim::XAP_LOOP;

	////Light 위치표기 메쉬
	//m_pXMeshLight			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Sun.X" );
	//m_pMatrixLight		= new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	//m_pMatrixLight->Scale( 0.5f, 0.5f, 0.5f );

	////충돌박스 메쉬
#ifdef SHOW_ATTACK_BOX_DUMMY
	m_pXMeshSphere		= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Bounding_Sphere.X" );
	m_pMatrixSphere		= new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	m_bShowAttackBox	= false;
#endif // SHOW_ATTACK_BOX_DUMMY


	//TEX_STAGE_DATA texStageData;
	//m_vecTexStageData.push_back( texStageData );
	//m_vecTexStageData.push_back( texStageData );
	//m_vecTexStageData.push_back( texStageData );

	//m_pMatrix = NULL;
	//m_RotWeapon = D3DXVECTOR3( 0, 0, 0 );

	////D3DXCreateCylinder( g_pKTDXApp->GetDevice(), 100.0f, 100.0f, 200.0f, 10, 10, &m_pSMesh, NULL );
	////m_pSMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );
}



HRESULT CSkinAnimObject::OnFrameMove( double fTime, float fElapsedTime )
{


	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return S_OK;


	if( false == m_bShow )
		return S_OK;

	//CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();
	//*pRenderParam = m_RenderParam;

	D3DXVECTOR3 position = m_pXSkinAnim->GetMatrix().GetPos();
	//모션 이동
	position.x += m_pXSkinAnim->GetMotionOffset().x/3.0f;
	position.z += m_pXSkinAnim->GetMotionOffset().z/3.0f;

	//m_pXSkinAnim->GetMatrix().Scale( m_vScale );
	m_pXSkinAnim->GetMatrix().Move( position );	
	m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );

	//m_vecPlayingTimedXMeshPlayer


	return S_OK;
}

HRESULT CSkinAnimObject::OnFrameRender()
{

#ifdef SHOW_ATTACK_BOX_DUMMY
	if( true == m_bShowAttackBox )
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
#endif // SHOW_ATTACK_BOX_DUMMY


	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return S_OK;

	if( false == m_bShow )
		return S_OK;

	m_pXSkinAnim->OnFrameRender();
	
#ifdef SHOW_ATTACK_BOX_DUMMY
	if( true == m_bShowAttackBox )
	{
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollisionData, m_pXSkinAnim->GetAttackDataList() )
		{			
			//pCollisionData = m_pXSkinAnim->m_AttackDataList[i];
			if( pCollisionData->m_CollisionType == CKTDXCollision::CT_SPHERE )
			{
				m_pMatrixSphere->Move( pCollisionData->GetPointStart() );
				float fScale = pCollisionData->GetScaleRadius();
				m_pMatrixSphere->Scale( fScale, fScale, fScale );
				m_pMatrixSphere->UpdateWorldMatrix();
				if( NULL != m_pXMeshSphere )
					m_pXMeshSphere->Render();
			}
			else if( pCollisionData->m_CollisionType == CKTDXCollision::CT_LINE )
			{
				D3DXVECTOR3 vPos = pCollisionData->GetPointStart();
				D3DXVECTOR3 vPosDelta = pCollisionData->GetPointEnd() - pCollisionData->GetPointStart();
				vPosDelta /= 29.f;
				for( int j=0; j<30; j++ )
				{
					m_pMatrixSphere->Move( vPos );
					float fScale = pCollisionData->GetScaleRadius();
					m_pMatrixSphere->Scale( 1, 1, 1 );
					m_pMatrixSphere->UpdateWorldMatrix();

					if( NULL != m_pXMeshSphere )
						m_pXMeshSphere->Render();
					vPos += vPosDelta;
				}			
				//g_pKTDXApp->SetWorldTransform( pCollisionData->pCombineMatrix );
				//m_pSMesh->DrawSubset( 0 );
			}
		}
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}
#endif // SHOW_ATTACK_BOX_DUMMY


	return S_OK;
}



void CSkinAnimObject::Clear()
{
	std::map<std::wstring, CKTDXDeviceXSkinMesh*>::iterator itr;
	for( itr = m_mapSkinMesh.begin(); itr != m_mapSkinMesh.end(); ++itr )
	{
		m_pXSkinAnim->RemoveModelXSkinMesh( itr->second );
		g_pKTDXApp->GetDeviceManager()->CloseDevice( itr->first );
	}
	m_mapSkinMesh.clear();
	
	SAFE_DELETE_KTDGOBJECT( m_pXSkinAnim );
	
}


CKTDGXRenderer::RenderParam* CSkinAnimObject::GetRenderParam()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return NULL;

	return m_pXSkinAnim->GetRenderParam();

}

void CSkinAnimObject::SetPos( const D3DXVECTOR3& vPos )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->GetMatrix().Move( vPos );
}

D3DXVECTOR3 CSkinAnimObject::GetPos()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return D3DXVECTOR3(0,0,0);

	return m_pXSkinAnim->GetMatrix().GetPos();
}


void CSkinAnimObject::SetRotationDegree( const D3DXVECTOR3& vRotation )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->GetMatrix().RotateDegree( vRotation );
}

D3DXVECTOR3 CSkinAnimObject::GetRotationDegree()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return D3DXVECTOR3(0,0,0);

	return m_pXSkinAnim->GetMatrix().GetRotateDegree();
}

bool CSkinAnimObject::SetAnimXSkinMesh( std::wstring fileName )
{
	Clear();

	// m_pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
	m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

	CKTDXDeviceXSkinMesh* pMotion = NULL;
	pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

	if( pMotion != NULL )
	{
		m_pXSkinAnim->SetAnimXSkinMesh( pMotion );
		m_pXSkinAnim->AddModelXSkinMesh( pMotion );
		m_mapSkinMesh.insert( std::make_pair( fileName, pMotion ) );

		return true;
	}

	return false;
}

void CSkinAnimObject::ChangeAnim( std::wstring animName )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->ChangeAnim( animName.c_str(), false );
	
	m_iNowAniNameIndex = m_pXSkinAnim->GetIndexByName( animName.c_str() );
	//m_pXSkinAnim->Play( m_AnimPlaytype );
}

void CSkinAnimObject::ChangeAnim( int index )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->ChangeAnim( index, false );
	m_iNowAniNameIndex = index;
	//m_pXSkinAnim->Play( m_AnimPlaytype );
}

void CSkinAnimObject::Play( CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;
	
	m_pXSkinAnim->ChangeAnim( m_iNowAniNameIndex, false );
	m_pXSkinAnim->Play( playType );	
}


bool CSkinAnimObject::GetAnimNameList( std::vector<std::wstring>& vecAnimNameList )
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

bool CSkinAnimObject::GetFrameNameList( OUT std::vector<std::wstring>& vecFrameNameList )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
	{
		return false;
	}
	vecFrameNameList.clear();

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

		if( wstrName.compare(L"Scene_Root") != 0 && wstrName.compare(L"") != 0)
			vecFrameNameList.push_back( wstrName );
	}

	return true;
}




//////////////////////////////////////////////////////////////////////////