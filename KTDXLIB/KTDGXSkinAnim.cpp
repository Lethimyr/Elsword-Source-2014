#include "StdAfx.h"
#include ".\ktdgxskinanim.h"

bool CKTDGXSkinAnim::m_sbTestUpdateAnimation = true;

CKTDGXSkinAnim::CKTDGXSkinAnim() 
#ifdef FIX_MESH_RENDER_BEFORE_INITINALIZE
:m_bIsProcessFrameMove(false)
#endif // FIX_MESH_RENDER_BEFORE_INITINALIZE

{
	m_eObjectType = OT_CKTDGXSkinAnim;

	m_pAnimXSkinMesh		= NULL;
	m_pAnimAniXET			= NULL;
	m_pRenderer				= g_pKTDXApp->GetDGManager()->GetXRenderer();
	m_bUseTex[0]			= true;
	m_bUseTex[1]			= true;
	m_bUseTex[2]			= true;

	m_NowTrack				= 0;
	m_NewTrack				= 1;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fElapsedTime			= 0.0f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fAnimGlobalTime		= 0.0f;
	m_fAnimCurrTime			= 0.0f;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fAnimCurrSpeed        = 1.0f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fAnimBeforeTime		= 0.0f;

	m_fAnimMaxTime			= 0.0f;
	m_pAC					= NULL;

	m_AnimState				= XAS_STOP;
	m_PlayType				= XAP_LOOP;
	m_NowAnimIndex			= 0;
	m_bAnimEnd				= false;
	m_fPlaySpeed			= 1.0f;
	m_fTransitionTime		= 0.1f;
	m_NowPlayCount			= 0;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fAnimOneshotTimer = 0.f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	m_MotionMoveBack.x		= 0.0f;
	m_MotionMoveBack.y		= 0.0f;
	m_MotionMoveBack.z		= 0.0f;

	m_MotionMoveNow.x		= 0.0f;
	m_MotionMoveNow.y		= 0.0f;
	m_MotionMoveNow.z		= 0.0f;

	m_MotionOffset.x		= 0.0f;
	m_MotionOffset.y		= 0.0f;
	m_MotionOffset.z		= 0.0f;

	m_bApplyMotionOffset	= true;

	m_bUseDXMatrix			= false;
	m_pCloneFrameRoot		= NULL;

	m_FrameNum				= 1;	
    m_BillBoardType			= CKTDGMatrix::BT_NONE;
//{{ robobeg : 2008-10-16
	//m_uiPass				= 0;
//}} robobeg : 2008-10-16
	ZeroMemory( m_FrameMatrixList, sizeof(D3DXMATRIX*) * MAX_FRAME_NUM );

	m_ModelDetailPercent	= 100;

	SetIsCulled( false );

	m_pModelXSkinMeshList.reserve( 16 );
	m_pModelAniXETList.reserve( 16 );
	m_pModelAniDataList.reserve( 16 );
	m_pModelMultiTexXETList.reserve( 16 );
	m_pModelTexChangeXETList.reserve( 16 );

	//m_CollisionDataList.reserve( 32 );
	//m_AttackDataList.reserve( 32 );

	m_vecAlphaSkinMesh.reserve( 32 );

#ifdef ARA_CHARACTER_BASE
	m_vecRenderParamSkinMesh.reserve( 32 );
#endif




#ifdef SKIN_LIGHTFLOW
    m_bLightFlow                = false;
    m_lightFlowType             = CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Y;    
    m_pLightFlow                = NULL;
    m_fFlowMin                  = 0.f;
    m_fFlowMax                  = 0.f;
    m_fFlowSpeed                = 0.f;
    m_fNowPoint                 = 0.f;
    m_fWide                     = 0.f;

    //m_BillboardType		= CKTDGMatrix::BT_NONE;    
    INIT_COLOR( m_coColor, 1, 1, 1, 1 );
    //INIT_VECTOR3( m_LightPos, 1000, 1000, 1000 );
#endif

#ifdef JIGGLEBONE_TEST
	m_bJiggleMesh = false;
	m_fTension = -1.f;
	m_iExpand = 0;
#endif

	m_bApplyMoveBone = true;

#ifdef FACE_OFF_MONSTER_HEAD_TEST
	m_bHideNoRenderable = false;
#endif FACE_OFF_MONSTER_HEAD_TEST

}

CKTDGXSkinAnim::~CKTDGXSkinAnim(void)
{
	CleanAnimStructure();

	BOOST_TEST_FOREACH( CKTDXDeviceXSkinMesh*, pSkinMesh, m_vecAddedSkinMesh )
	{
		SAFE_CLOSE( pSkinMesh );
	}
	m_vecAddedSkinMesh.clear();
}


//{{ seojt // 2008-10-15, 14:48
void CKTDGXSkinAnim::PreLoad()
{
    if ( m_pAnimXSkinMesh )
        m_pAnimXSkinMesh->PreLoad();
}//CKTDGXSkinAnim::PreLoad()
//}} seojt // 2008-10-15, 14:48


void CKTDGXSkinAnim::CleanAnimStructure()
{
	for( int i = 0; i < (int)m_vAnimSetList.size(); i++ )
	{
		SAFE_RELEASE( m_vAnimSetList[i] );		
	}
	
	m_mAnimSetList.clear();
	m_mIndexByName.clear();		
	m_mCloneFrameMap.clear();



	m_vAnimSetList.resize(0);
	m_pModelXSkinMeshList.resize(0);
	m_pModelAniXETList.resize(0);
	m_pModelAniDataList.resize(0);
	m_pModelMultiTexXETList.resize(0);
	m_pModelTexChangeXETList.resize(0);

	
	
	CKTDXDeviceXSkinMesh::CMultiAnimAllocateHierarchy AH;
	if( m_pCloneFrameRoot != NULL )
	{		
		D3DXFrameDestroy( m_pCloneFrameRoot, &AH );
		m_pCloneFrameRoot = NULL;
	}

    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollision, m_CollisionDataList )
    {
        SAFE_DELETE( pCollision );
    }//BOOST_TEST_FOREACH()
	m_CollisionDataList.clear();

    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pAttack, m_AttackDataList )
    {
        SAFE_DELETE( pAttack );
    }//BOOST_TEST_FOREACH(
	m_AttackDataList.clear();

	SAFE_RELEASE( m_pAC );
}

void CKTDGXSkinAnim::BuildCloneFrameStructure( CKTDXDeviceXSkinMesh* pAnimXSkinMesh )
{
	KTDXPROFILE();
	
	m_mCloneFrameMap.clear();

	CKTDXDeviceXSkinMesh::CMultiAnimAllocateHierarchy AH;
	if( m_pCloneFrameRoot != NULL )
	{
		D3DXFrameDestroy( m_pCloneFrameRoot, &AH );
		m_pCloneFrameRoot = NULL;
	}
	m_pCloneFrameRoot = new CKTDXDeviceXSkinMesh::MultiAnimFrame();
	BuildCloneFrame( m_pCloneFrameRoot, pAnimXSkinMesh->GetFrameRoot() );

	m_FrameNum = 0;
	BuildCloneFrameList( m_pCloneFrameRoot );
}

void CKTDGXSkinAnim::BuildCloneFrame( CKTDXDeviceXSkinMesh::MultiAnimFrame* pCloneFrame, CKTDXDeviceXSkinMesh::MultiAnimFrame* pSrcFrame )
{
	KTDXPROFILE();

	if( pSrcFrame->Name )
	{		
		if( 0 == strlen( pSrcFrame->Name ) )
		{
			pCloneFrame->Name	= (CHAR *) StrHeapCopy( "<no_name>" );
		}
		else
		{
			pCloneFrame->Name	= (CHAR *) StrHeapCopy( (CHAR *) pSrcFrame->Name );
		}
	}
	else
	{		
		pCloneFrame->Name	= (CHAR *) StrHeapCopy( "<no_name>" );
	}
	pCloneFrame->combineMatrix		= pSrcFrame->combineMatrix;
	pCloneFrame->fBoundingRadius	= pSrcFrame->fBoundingRadius;


//#ifdef HEAD_INVERSE_KINEMATICS_TEST
//	pCloneFrame->m_iIndex = pSrcFrame->m_iIndex;
//#endif HEAD_INVERSE_KINEMATICS_TEST


	WCHAR wszBoneName[ MAX_PATH ];
	MultiByteToWideChar( CP_ACP, 0, pCloneFrame->Name, -1, wszBoneName, MAX_PATH );
	m_mCloneFrameMap[wszBoneName] = pCloneFrame;
	
	//wstring wstrBoneName = L"";
	//ConvertCharToWCHAR( wstrBoneName, pCloneFrame->Name );
	//ASSERT( false == wstrBoneName.empty() );
	//m_mCloneFrameMap[ wstrBoneName ] = pCloneFrame;

#ifdef JIGGLEBONE_TEST
	std::string boneName = pCloneFrame->Name;
	if( strstr(boneName.c_str(), "Jiggle_Bone_Root") == NULL && strstr(boneName.c_str(), "Jiggle_Bone_") != NULL )
	{
		pCloneFrame->m_bJiggleFrame = true;

		char cNum = boneName[12];
		pCloneFrame->m_iJiggleLevel = cNum-48;

		//float fRand = (rand() % 61) / 100.f - 0.3f;
		//fTension += fRand;
	}
#endif


	// transform siblings by the same matrix
	if( pSrcFrame->pFrameSibling )
	{
		pCloneFrame->pFrameSibling = new CKTDXDeviceXSkinMesh::MultiAnimFrame();		
		BuildCloneFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameSibling, (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pSrcFrame->pFrameSibling );
	}
	else
		pCloneFrame->pFrameSibling = NULL;

	// transform children by the transformed matrix - hierarchical transformation
	if( pSrcFrame->pFrameFirstChild )
	{
		pCloneFrame->pFrameFirstChild = new CKTDXDeviceXSkinMesh::MultiAnimFrame();
		BuildCloneFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameFirstChild, (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pSrcFrame->pFrameFirstChild );
	}
	else
		pCloneFrame->pFrameFirstChild = NULL;
}

void CKTDGXSkinAnim::BuildCloneFrameList( CKTDXDeviceXSkinMesh::MultiAnimFrame* pCloneFrame )
{
	m_FrameMatrixList[m_FrameNum] = &pCloneFrame->combineMatrix;
	m_FrameNum++;

	if( pCloneFrame->pFrameSibling )
	{
		// recursive call
		BuildCloneFrameList( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pCloneFrame->pFrameSibling );
	}

	if( pCloneFrame->pFrameFirstChild )
	{
		// recursive call
		BuildCloneFrameList( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pCloneFrame->pFrameFirstChild );
	}
}

void CKTDGXSkinAnim::SetAnimXET( CKTDXDeviceXET* pAnimAniXET )
{
	m_pAnimAniXET = pAnimAniXET;
}


// @bSimple : 타격, 피격 박스가 없는 모션파일의 빠른 로딩을 위해서
bool CKTDGXSkinAnim::SetAnimXSkinMesh( CKTDXDeviceXSkinMesh* pAnimXSkinMesh, CKTDXDeviceXET* pAnimAniXET, bool bSimple /*= false*/ )
{ 
	KTDXPROFILE();

    //{{ jseop. pAnimXSkinMesh 가 NULL 인 경우가 있었음.
    ASSERT( pAnimXSkinMesh );
    //if( !pAnimXSkinMesh )
    //{
    //    return false;
    //}
    //}} jseop
	if ( NULL == pAnimXSkinMesh )
		return false;

	if( pAnimXSkinMesh->GetIsOnlyModelData() == true )
		return false;

	CleanAnimStructure();

	//SAFE_RELEASE( m_pAC );
	m_pAC = pAnimXSkinMesh->GetCloneAC();
	if( m_pAC == NULL )
	{
		ErrorLog( KEM_ERROR47 );
		return false;
	}

	ResetAnimSet();

	//D3DXMATRIX mat;
	//D3DXMatrixIdentity( &mat );

	m_pAnimXSkinMesh = pAnimXSkinMesh;
	if( m_pCloneFrameRoot == NULL )
	{
		BuildCloneFrameStructure( m_pAnimXSkinMesh );

		if( false == bSimple )
		{
			//{{ dmlee 2008.05.30 데미지 이펙트 이상한 위치에서 타격되는 문제 수정
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            UpdateAnimationFrameStructure( 0.f );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			UpdateAnimationFrameStructure();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			//}} dmlee 2008.05.30 데미지 이펙트 이상한 위치에서 타격되는 문제 수정	
		}
	}

	m_pAnimAniXET = pAnimAniXET;

	return true;
}

#ifdef SEPARATION_MOTION
bool CKTDGXSkinAnim::MixAnim( CKTDXDeviceXSkinMesh* pAnimXSkinMesh, bool bSimple /*= false*/ )
{ 
	KTDXPROFILE();

	//{{ jseop. pAnimXSkinMesh 가 NULL 인 경우가 있었음.
	ASSERT( pAnimXSkinMesh );
	//if( !pAnimXSkinMesh )
	//{
	//    return false;
	//}
	//}} jseop

	if( pAnimXSkinMesh->GetIsOnlyModelData() == true )
		return false;

	if( m_pAC == NULL )
		return false;

	LPD3DXANIMATIONCONTROLLER pAnimMixer = NULL;
	LPD3DXANIMATIONCONTROLLER pAnimMixerNew = NULL;
	LPD3DXANIMATIONSET pAnimSet;

	pAnimMixerNew = pAnimXSkinMesh->GetCloneAC();
	if( pAnimMixerNew == NULL )
		return false;

	UINT cAnimSets = pAnimMixerNew->GetNumAnimationSets();
	
	HRESULT hr = m_pAC->CloneAnimationController(
		m_pAC->GetMaxNumAnimationOutputs(),
		m_pAC->GetMaxNumAnimationSets() + cAnimSets,
		m_pAC->GetMaxNumTracks(),
		m_pAC->GetMaxNumEvents(),
		&pAnimMixer);

	if(FAILED(hr))
		return false;

	SAFE_RELEASE( m_pAC );	

	m_pAC = pAnimMixer;
	pAnimMixer = NULL;

	for( UINT iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++ )
	{
		pAnimMixerNew->GetAnimationSet(iAnimSet, &pAnimSet);
		m_pAC->RegisterAnimationSet(pAnimSet);

		SAFE_RELEASE(pAnimSet);
	}
	
	SAFE_RELEASE( pAnimMixerNew );

	//CleanAnimStructure();
	for( int i = 0; i < (int)m_vAnimSetList.size(); i++ )
	{
		SAFE_RELEASE( m_vAnimSetList[i] );		
	}
	m_mAnimSetList.clear();	

	ResetAnimSet();

	//D3DXMATRIX mat;
	//D3DXMatrixIdentity( &mat );

	//m_pAnimXSkinMesh = pAnimXSkinMesh;
	if( m_pCloneFrameRoot == NULL )
	{
		BuildCloneFrameStructure( m_pAnimXSkinMesh );
	}

	if( false == bSimple )
	{
		//{{ dmlee 2008.05.30 데미지 이펙트 이상한 위치에서 타격되는 문제 수정
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        UpdateAnimationFrameStructure( 0.f );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		UpdateAnimationFrameStructure();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		//}} dmlee 2008.05.30 데미지 이펙트 이상한 위치에서 타격되는 문제 수정	
	}

	return true;
}
#endif


void CKTDGXSkinAnim::ReplaceAniXET( CKTDXDeviceXET* pModelAniXETFrom, CKTDXDeviceXET* pModelAniXETTo )
{
	for ( unsigned int iA = 0; iA < m_pModelAniXETList.size(); ++iA )
	{
		CKTDXDeviceXET* pModelAniXET = m_pModelAniXETList[iA];
		if ( pModelAniXET == pModelAniXETFrom )
		{
			pModelAniXET = pModelAniXETTo;
		}
	}
}


// index번째에 XET가 있으면 교체하고, 없으면 만들어서 넣는다. return 값은 기존에 있던 xet이다.
CKTDXDeviceXET* CKTDGXSkinAnim::ReplaceMultiTexXET( int iIndex, CKTDXDeviceXET* pModelMultiTexXETTo )
{
	if( m_pModelMultiTexXETList.empty() )
	{
		m_pModelMultiTexXETList.push_back( pModelMultiTexXETTo );
	}
	else
	{
		CKTDXDeviceXET* pOldXet = m_pModelMultiTexXETList[0];
		m_pModelMultiTexXETList[0] = pModelMultiTexXETTo;
		return pOldXet;
	}

	return NULL;
}


void CKTDGXSkinAnim::ReplaceMultiTexXET( CKTDXDeviceXET* pModelMultiTexXETFrom, CKTDXDeviceXET* pModelMultiTexXETTo )
{
	for ( unsigned int iA = 0; iA < m_pModelMultiTexXETList.size(); ++iA )
	{
		CKTDXDeviceXET* pModelAniXET = m_pModelMultiTexXETList[iA];
		if ( pModelAniXET == pModelMultiTexXETFrom )
		{
			pModelAniXET = pModelMultiTexXETTo;
		}
	}
}

void CKTDGXSkinAnim::ReplaceChangeXET( CKTDXDeviceXET* pModelTexChangeXETFrom, CKTDXDeviceXET* pModelTexChangeXETTo )
{
	for ( unsigned int iA = 0; iA < m_pModelTexChangeXETList.size(); ++iA )
	{
		CKTDXDeviceXET* pModelAniXET = m_pModelTexChangeXETList[iA];
		if ( pModelAniXET == pModelTexChangeXETFrom )
		{
			m_pModelTexChangeXETList[iA] = pModelTexChangeXETTo;
		}
	}
}

void CKTDGXSkinAnim::ReplaceChangeXET( int iIndex, CKTDXDeviceXET* pModelTexChangeXETTo )
{
	if( iIndex < 0 || iIndex > (int)m_pModelTexChangeXETList.size()-1 )
		return;

	m_pModelTexChangeXETList[iIndex] = pModelTexChangeXETTo;
}



// @bCloseDeviceAtDestructor : 추가된 device를 skinanim객체가 지워질 때 close_device해줄지 말지. 일단은 pModelXSkinMesh만 처리 되게 만들었음.
void CKTDGXSkinAnim::AddModelXSkinMesh( CKTDXDeviceXSkinMesh* pModelXSkinMesh, CKTDXDeviceXET* pModelAniXET, 
									   CKTDXDeviceXET* pModelMultiTexXET, CKTDXDeviceXET* pModelTexChangeXET, bool bCloseDeviceAtDestructor /*= false*/
                                       , bool bAddCollisionData /*= true*/ 
									   )
{

	KTDXPROFILE();

	if( NULL == pModelXSkinMesh )
		return;

	if( true == bCloseDeviceAtDestructor )
	{
		m_vecAddedSkinMesh.push_back( pModelXSkinMesh );
	}

	m_pModelXSkinMeshList.push_back( pModelXSkinMesh );
	m_pModelAniXETList.push_back( pModelAniXET );
	m_pModelAniDataList.push_back( NULL );
	m_pModelMultiTexXETList.push_back( pModelMultiTexXET );
	m_pModelTexChangeXETList.push_back( pModelTexChangeXET );

	if( bAddCollisionData == false )
		return;

	CKTDXCollision::CollisionData* pCollisionData = NULL;
	CKTDXCollision::CollisionData* pAttackData = NULL;

    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pModelCollision, pModelXSkinMesh->GetCollisionDataList() )
    {
        ASSERT( pModelCollision != NULL );
		pCollisionData = new CKTDXCollision::CollisionData();
        ASSERT( pCollisionData );
        *pCollisionData = *pModelCollision;
		if( LinkCollisionDataFrame( pCollisionData ) == false )
		{
			// 오현빈 // 2013-09-13 // 
			// 기존 : LinkCollisionDataFrame 값이 false를 return하지 않도록 함으로써 강제 크래시 발생 유도
			// 변경 : 로그 남기기 위해 false를 return하는 대신 continue; 수행 하지 않도록 하여 크래시 발생 유도
			ErrorLogMsg( KEM_ERROR432, pModelXSkinMesh->GetDeviceID().c_str() );
// 			SAFE_DELETE( pCollisionData );
// 			continue;
		}
		m_CollisionDataList.push_back( pCollisionData );
    }//BOOST_TEST_FOREACH()

    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pModelAttack, pModelXSkinMesh->GetAttackDataList() )
    {
        ASSERT( pModelAttack != NULL );
		pAttackData = new CKTDXCollision::CollisionData();
		*pAttackData = *pModelAttack;
		if( LinkCollisionDataFrame( pAttackData ) == false )
		{
			ErrorLogMsg( KEM_ERROR432, pModelXSkinMesh->GetDeviceID().c_str() );
// 			SAFE_DELETE( pAttackData );
// 			continue;
		}
		m_AttackDataList.push_back( pAttackData );
    }//BOOST_TEST_FOREACH()


// 	CKTDXDeviceXET* pXET = pModelXSkinMesh->GetXET();
// 
// 	if( pXET != NULL )
// 	{
//         BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pXETCollision, pXET->GetCollisionList() )
// 		{
//             ASSERT( pXETCollision != NULL );
// 			pCollisionData = new CKTDXCollision::CollisionData();
//             ASSERT( pCollisionData != NULL );
// 			*pCollisionData = *pXETCollision;
// 			if( LinkCollisionDataFrame( pCollisionData ) == false )
// 			{
// 				SAFE_DELETE( pCollisionData );
// 				continue;
// 			}
// 			m_CollisionDataList.push_back( pCollisionData );
// 		}
//         BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pXETAttack, pXET->GetAttackList() )
// 		{
//             ASSERT( pXETAttack != NULL );
// 			pAttackData = new CKTDXCollision::CollisionData();
//             ASSERT( pAttackData != NULL );
// 			*pAttackData = *pXETAttack;
// 			if( LinkCollisionDataFrame( pAttackData ) == false )
// 			{
// 				SAFE_DELETE( pAttackData );
// 				continue;
// 			}
// 			m_AttackDataList.push_back( pAttackData );
// 		}
// 	}
}

void CKTDGXSkinAnim::AddAnimXSkinMesh( CKTDXDeviceXSkinMesh* pModelXSkinMesh )
{
	m_pModelXSkinMeshList.push_back( pModelXSkinMesh );
	m_pModelAniXETList.push_back( NULL );
	m_pModelAniDataList.push_back( NULL );
	m_pModelMultiTexXETList.push_back( NULL );
	m_pModelTexChangeXETList.push_back( NULL );

	CKTDXCollision::CollisionData* pCollisionData	= NULL;
	CKTDXCollision::CollisionData* pAttackData		= NULL;

    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pModelCollision, pModelXSkinMesh->GetCollisionDataList() )
    {
        ASSERT( pModelCollision != NULL );
		pCollisionData = new CKTDXCollision::CollisionData();
        ASSERT( pCollisionData != NULL );
		*pCollisionData = *pModelCollision;
		if( LinkCollisionDataFrame( pCollisionData ) == false )
		{
			// 오현빈 // 2013-09-13 // 
			// 기존에는 LinkCollisionDataFrame 값이 false를 return하지 않도록 함으로써 강제 크래시 발생 하도록 했음.
			// 로그 남기기 false를 return 하고 continue 하지 않도록 변경거
			ErrorLogMsg( KEM_ERROR432, pModelXSkinMesh->GetDeviceID().c_str() );
			//SAFE_DELETE( pCollisionData );
			//continue;
		}
		m_CollisionDataList.push_back( pCollisionData );
    }//BOOST_TEST_FOREACH()


    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pModelAttack, pModelXSkinMesh->GetAttackDataList() )
    {
        ASSERT( pModelAttack != NULL );
		pAttackData = new CKTDXCollision::CollisionData();
        ASSERT( pAttackData != NULL );
		*pAttackData = *pModelAttack;
		if( LinkCollisionDataFrame( pAttackData ) == false )
		{
			ErrorLogMsg( KEM_ERROR432, pModelXSkinMesh->GetDeviceID().c_str() );
// 			SAFE_DELETE( pAttackData );
// 			continue;
		}
		m_AttackDataList.push_back( pAttackData );
    }//BOOST_TEST_FOREACH()
}

bool CKTDGXSkinAnim::LinkCollisionDataFrame( CKTDXCollision::CollisionData* pCollisionData )
{	
	ASSERT( NULL != pCollisionData );
	
    if( pCollisionData->m_CollisionType == CKTDXCollision::CT_SPHERE || pCollisionData->m_CollisionType == CKTDXCollision::CT_GUARD)
	{
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = GetCloneFrame( pCollisionData->m_FrameName.c_str() );
		if( NULL == pFrame )
		{
			ASSERT( !L"GetCloneFrame Error" );
			return false;
		}

		pCollisionData->m_pCombineMatrix		= &pFrame->combineMatrix;
		pCollisionData->m_pTransformMatrix		= &pFrame->TransformationMatrix;
		pCollisionData->m_pBaseMatrix			= &pFrame->baseMatrix;
	}
	else if( pCollisionData->m_CollisionType == CKTDXCollision::CT_LINE )
	{
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = GetCloneFrame( pCollisionData->m_FrameName.c_str() );
		if( NULL == pFrame )
		{
			ASSERT( !L"GetCloneFrame Error" );
			return false;
		}

		pCollisionData->m_pCombineMatrix		= &pFrame->combineMatrix;
		pCollisionData->m_pTransformMatrix		= &pFrame->TransformationMatrix;
		pCollisionData->m_pBaseMatrix			= &pFrame->baseMatrix;

		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameEnd = GetCloneFrame( pCollisionData->m_FrameNameLineEnd.c_str() );
		if( NULL == pFrameEnd )
		{
			ASSERT( !L"GetCloneFrame Error" );
			return false;
		}

		pCollisionData->m_pCombineMatrixLineEnd		= &pFrameEnd->combineMatrix;
		pCollisionData->m_pTransformMatrixLineEnd	= &pFrameEnd->TransformationMatrix;
		pCollisionData->m_pBaseMatrixLineEnd		= &pFrameEnd->baseMatrix;
	}

	return true;
}

void CKTDGXSkinAnim::RemoveModelXSkinMesh( CKTDXDeviceXSkinMesh* pModelXSkinMesh )
{
	if ( pModelXSkinMesh == NULL )
		return;

	for( int i = 0; i < (int)m_pModelXSkinMeshList.size(); i++ )
	{
		if( m_pModelXSkinMeshList[i] == pModelXSkinMesh )
		{
			//충돌박스 제거

            CKTDXCollision::CollisionDataList::iterator iter, iterNext;
            for( iter = m_CollisionDataList.begin(); iter != m_CollisionDataList.end(); iter = iterNext )
            {
                iterNext = iter; iterNext++;
                CKTDXCollision::CollisionData* pCollision = *iter;
                ASSERT( pCollision != NULL );
				if( pCollision->m_pDeviceMesh == pModelXSkinMesh )
				{
					SAFE_DELETE( pCollision );
					m_CollisionDataList.erase( iter );
				}//if
            }//for

            for( iter = m_AttackDataList.begin(); iter != m_AttackDataList.end(); iter = iterNext )
            {
                iterNext = iter; iterNext++;
                CKTDXCollision::CollisionData* pAttack = *iter;
                ASSERT( pAttack != NULL );
                if ( pAttack->m_pDeviceMesh == pModelXSkinMesh )
                {
                    SAFE_DELETE( pAttack );
                    m_AttackDataList.erase( iter );
                }//if
            }//for

			if( i < (int)m_pModelXSkinMeshList.size() )
				m_pModelXSkinMeshList.erase( m_pModelXSkinMeshList.begin() + i );
			if( i < (int)m_pModelAniXETList.size() )
				m_pModelAniXETList.erase( m_pModelAniXETList.begin() + i );
			if( i < (int)m_pModelAniDataList.size() )
				m_pModelAniDataList.erase( m_pModelAniDataList.begin() + i );
			if( i < (int)m_pModelMultiTexXETList.size() )
				m_pModelMultiTexXETList.erase( m_pModelMultiTexXETList.begin() + i );
			if( i < (int)m_pModelTexChangeXETList.size() )
				m_pModelTexChangeXETList.erase( m_pModelTexChangeXETList.begin() + i );
			break;
		}
	}
}

bool CKTDGXSkinAnim::IsInModelXSkinMesh( CKTDXDeviceXSkinMesh* pModelXSkinMesh )
{
	for( int i = 0; i < (int)m_pModelXSkinMeshList.size(); i++ )
	{
		if( m_pModelXSkinMeshList[i] == pModelXSkinMesh )
		{
			return true;
		}
	}
	return false;
}

HRESULT CKTDGXSkinAnim::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fElapsedTime = fElapsedTime;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	if( m_AnimState == XAS_PLAYING )
	{
		m_bAnimEnd = false;
		
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        UpdateAnimationFrameStructure( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		UpdateAnimationFrameStructure();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		switch( m_PlayType )
		{
			case XAP_LOOP:
				if( m_fAnimCurrTime > m_fAnimMaxTime - m_fTransitionTime )
				{
					m_bAnimEnd = true;
					ChangeAnim( m_NowAnimIndex );
					m_NowPlayCount++;
				}
				break;

			case XAP_ONE:
				if( m_fAnimCurrTime > m_fAnimMaxTime - m_fTransitionTime )
				{
					m_bAnimEnd		= true;
					m_AnimState		= XAS_STOP;
					m_NowPlayCount	= 1;
				}
				break;

			case XAP_ONE_WAIT:
				if( m_fAnimCurrTime > m_fAnimMaxTime - m_fTransitionTime )
				{
					m_bAnimEnd		= true;
					m_AnimState		= XAS_WAIT;
					m_NowPlayCount	= 1;
				}
				break;

		}
	}
	else if( m_AnimState == XAS_WAIT )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		UpdateAnimationFrameStructure( 0.f );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_fElapsedTime = 0.0f;
		UpdateAnimationFrameStructure();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else
    {
        UpdateBeforeAnimationTime();
    }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


#ifdef SKIN_LIGHTFLOW
    if(m_bLightFlow == true)
    {
        LightFlowFrameMove(fTime, fElapsedTime);
    }    
#endif

#ifdef FIX_MESH_RENDER_BEFORE_INITINALIZE
	if( false == GetProcessFrameMove() )
		SetProcessFrameMove(true);
#endif // FIX_MESH_RENDER_BEFORE_INITINALIZE


	return S_OK;
}


//{{ robobeg : 2008-10-16



//{{ robobeg : 2008-10-16
/*virtual*/
RENDER_HINT   CKTDGXSkinAnim::OnFrameRender_Prepare() 
{ 
    if ( m_pRenderer == NULL || m_AnimState == XAS_STOP )
        return RENDER_HINT_NORENDER;

    unsigned uSize = m_pModelXSkinMeshList.size();
    if ( uSize == 0 )
        return RENDER_HINT_NORENDER;



#ifdef SKIN_LIGHTFLOW
    if ( m_bLightFlow == true && m_pLightFlow != NULL ) 
        return RENDER_HINT_DEFAULT;
#endif // #ifdef SKIN_LIGHTFLOW

    unsigned uSkinningVerts = 0;
    for( unsigned u = 0; u < uSize; u++ )
    {
		CKTDXDeviceXSkinMesh* pXSkinMesh = m_pModelXSkinMeshList[u];

#ifdef FACE_OFF_MONSTER_HEAD_TEST
		if(m_bHideNoRenderable == true && pXSkinMesh->GetNoRenderable() == true)
			continue;
#endif FACE_OFF_MONSTER_HEAD_TEST

        ASSERT( pXSkinMesh != NULL );
        if ( uSkinningVerts < pXSkinMesh->GetMaxSkinningVertices() )
            uSkinningVerts = pXSkinMesh->GetMaxSkinningVertices();
    }//for

    return RENDER_HINT_XSKINMESH( m_RenderParam.renderType, m_RenderParam.bFog && m_RenderParam.bFogShow, uSkinningVerts );

}//OnFrameRender_Prepare()


/*virtual*/
void CKTDGXSkinAnim::OnFrameRender_Draw()
{
	KTDXPROFILE();

#ifdef FIX_MESH_RENDER_BEFORE_INITINALIZE
	if( false == GetProcessFrameMove() )
		return;
#endif // FIX_MESH_RENDER_BEFORE_INITINALIZE


	CopyModelFrameStructure();

#ifdef SKIN_LIGHTFLOW
    int modelIndex = 0;
#endif

    m_vecAlphaSkinMesh.resize( 0 );

#ifdef ARA_CHARACTER_BASE
	m_vecRenderParamSkinMesh.resize( 0 );
#endif

//{{ robobeg : 2008-10-06

    for( HRESULT hr = m_pRenderer->BeginRender( m_RenderParam ); SUCCEEDED( hr ); hr = m_pRenderer->NextRender() )
    {	
		for( int i = 0; i < (int)m_pModelXSkinMeshList.size(); i++ )
		{		
			CKTDXDeviceXSkinMesh* pXSkinMesh = m_pModelXSkinMeshList[i];
            ASSERT( pXSkinMesh != NULL );
#ifdef  X2OPTIMIZE_KTDGXSKINANIM_CRAHSH_BUG_FIX
            if ( pXSkinMesh == NULL )
                continue;
#endif  X2OPTIMIZE_KTDGXSKINANIM_CRAHSH_BUG_FIX
#ifdef FACE_OFF_MONSTER_HEAD_TEST
			if(m_bHideNoRenderable == true && pXSkinMesh->GetNoRenderable() == true)
				continue;
#endif FACE_OFF_MONSTER_HEAD_TEST

			for( int j = 0; j < (int) pXSkinMesh->GetDrawFrameNum(); j++ )
			{			
#ifdef SKIN_LIGHTFLOW
                if(m_bLightFlow == true && m_pLightFlow == pXSkinMesh->GetDrawFrame( j ))
                {
                    modelIndex = i;
                    continue;
                }
#endif

#ifdef ARA_CHARACTER_BASE
				if( false == pXSkinMesh->GetAlphaRender() && false == pXSkinMesh->GetUseRenderParam() )
				{
					DrawFrame( pXSkinMesh->GetDrawFrame( j ), i );
				}
				else if ( true == pXSkinMesh->GetAlphaRender() )
				{
					// alpha push
					Alpha_SkinMesh stAlphaObj = { pXSkinMesh, i };
					m_vecAlphaSkinMesh.push_back(stAlphaObj);
				}
				else if ( true == pXSkinMesh->GetUseRenderParam() )
				{
					RenderParam_SkinMesh stRenderParamObj = { pXSkinMesh, i };
					m_vecRenderParamSkinMesh.push_back( stRenderParamObj );
				}
#else
				if(pXSkinMesh->GetAlphaRender() == false)
				{
					DrawFrame( pXSkinMesh->GetDrawFrame( j ), i );
				}
				else
				{
					// alpha push
					Alpha_SkinMesh stAlphaObj = { pXSkinMesh, i };
					m_vecAlphaSkinMesh.push_back(stAlphaObj);
				}
#endif
			}
		}
	}//for
//}} robobeg : 2008-10-16

#ifdef SKIN_LIGHTFLOW
    if(m_bLightFlow == true && m_pLightFlow != NULL)
    {
//{{ robobeg : 2008-10-16

        CKTDGXRenderer::RenderParam renderParam = m_RenderParam;

        // light flow renderparam 설정
        renderParam.bAlphaBlend = true;
        renderParam.renderType = m_lightFlowType;
        renderParam.color.r		= m_coColor.r * m_RenderParam.color.r;
        renderParam.color.g		= m_coColor.g * m_RenderParam.color.g;
        renderParam.color.b		= m_coColor.b * m_RenderParam.color.b;
        renderParam.color.a		= m_coColor.a * m_RenderParam.color.a;

//{{ robobeg : 2008-10-24
//#if 0 
//        if( m_bUseDXMatrix == false )
//        {
//            D3DXMATRIX temp = GetMatrix().GetMatrix( m_BillBoardType );
//            renderParam.worldMatrix = temp;
//            //UpdateAnimationFrame( m_pAnimXSkinMesh->GetFrameRoot(), &temp, m_pCloneFrameRoot );
//        }
//        else
//        {
//            renderParam.worldMatrix = m_DXMatrix;
//            //UpdateAnimationFrame( m_pAnimXSkinMesh->GetFrameRoot(), &m_DXMatrix, m_pCloneFrameRoot );
//        }
//#endif
//}} robobeg : 2008-10-24
		//}}AFX

        //renderParam.lightPos		= m_LightPos;
        renderParam.fLightFlowImpact	= m_fNowPoint;
        renderParam.fLightFlowWide	= m_fWide;	

        for( HRESULT hr = m_pRenderer->BeginRender( renderParam ); SUCCEEDED( hr ); hr = m_pRenderer->NextRender() )
        {	
            DrawFrame(m_pLightFlow, modelIndex);
        }//for


    }    
#endif

    if( !m_vecAlphaSkinMesh.empty() )
    {
        CKTDGXRenderer::RenderParam renderParam = m_RenderParam;
        renderParam.bAlphaBlend = true;

        for( HRESULT hr = m_pRenderer->BeginRender( renderParam ); SUCCEEDED( hr ); hr = m_pRenderer->NextRender() )
        {
            for(int alphaIndex = 0; alphaIndex < (int)m_vecAlphaSkinMesh.size(); ++alphaIndex)
            {	
                Alpha_SkinMesh &stAlphaObj = m_vecAlphaSkinMesh[alphaIndex];  
            
                for( int j = 0; j < (int) stAlphaObj.m_pAlphaSkinMesh->GetDrawFrameNum(); j++ )
                {		
                    DrawFrame(stAlphaObj.m_pAlphaSkinMesh->GetDrawFrame( j ), stAlphaObj.m_indexMode);
                } //for
            } //for
        } //for
    } //if

#ifdef ARA_CHARACTER_BASE
	if( !m_vecRenderParamSkinMesh.empty() )
	{
		CKTDGXRenderer::RenderParam renderParam = m_RenderParam;
		renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;
		renderParam.bAlphaBlend = true;
		renderParam.bZEnable = true;
		renderParam.bZWriteEnable = true;
		renderParam.cullMode = D3DCULL_CCW;
		renderParam.srcBlend = D3DBLEND_SRCALPHA;
		renderParam.destBlend = D3DBLEND_DESTALPHA;
		
		for( HRESULT hr = m_pRenderer->BeginRender( renderParam ); SUCCEEDED( hr ); hr = m_pRenderer->NextRender() )
		{
			for( int RenderParamIndex = 0; RenderParamIndex < (int)m_vecRenderParamSkinMesh.size(); ++RenderParamIndex )
			{	
				RenderParam_SkinMesh &stRenderParamObj = m_vecRenderParamSkinMesh[RenderParamIndex];

				for( int j = 0; j < (int) stRenderParamObj.m_pRenderParamSkinMesh->GetDrawFrameNum(); j++ )
				{		
					DrawFrame( stRenderParamObj.m_pRenderParamSkinMesh->GetDrawFrame( j ), stRenderParamObj.m_indexMode );
				} //for
			} //for
		} //for
	} //if
#endif
    
//	return S_OK;
}

//}} robobeg : 2008-10-16



void CKTDGXSkinAnim::DrawFrame( CKTDXDeviceXSkinMesh::MultiAnimFrame* pModelFrame, int modelIndex )
{
	KTDXPROFILE();
#ifdef  X2OPTIMIZE_KTDGXSKINANIM_CRAHSH_BUG_FIX
    if ( modelIndex < 0 || modelIndex >= (int) m_pModelXSkinMeshList.size() || m_pModelXSkinMeshList[modelIndex] == NULL || pModelFrame == NULL )
        return;
#endif  X2OPTIMIZE_KTDGXSKINANIM_CRAHSH_BUG_FIX
    m_pRenderer->DrawFrame( *m_pModelXSkinMeshList[modelIndex], *pModelFrame, m_pModelTexChangeXETList[modelIndex]
        , m_pModelMultiTexXETList[modelIndex], m_pModelAniDataList[modelIndex], GetNowAnimationTime(), m_ModelDetailPercent
        , m_bUseTex, m_pAnimAniXET, m_NowAnimName.c_str() );
}//CKTDGXSkinAnim::DrawFrame()


void CKTDGXSkinAnim::Play( XSKIN_ANIM_PLAYTYPE playType )
{
	m_AnimState		= XAS_PLAYING;
	m_PlayType		= playType;
	m_NowPlayCount	= 0;
}

void CKTDGXSkinAnim::Wait()
{
	m_AnimState = XAS_WAIT;
}

void CKTDGXSkinAnim::Stop()
{
	m_AnimState = XAS_STOP;
}

void CKTDGXSkinAnim::Reset( float fAnimTime )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pAC == NULL )
        return;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_pAC->ResetTime();
	m_pAC->AdvanceTime( fAnimTime, NULL );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fAnimBeforeTime = 0.f;
    m_fAnimGlobalTime = (float) m_pAC->GetTime();
	D3DXTRACK_DESC td;
	m_pAC->GetTrackDesc( m_NowTrack, &td );
    m_fAnimCurrTime = (float) td.Position;
    if ( m_fAnimCurrTime < 0.f )
        m_fAnimCurrTime = 0.f;
    m_fAnimCurrSpeed = td.Speed;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fAnimCurrTime = fAnimTime;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}

bool CKTDGXSkinAnim::ChangeAnim( int index, bool transition, float fStartTime )
{
	KTDXPROFILE();
	bool retval;
	LPD3DXANIMATIONSET pAS;
	pAS = GetAnimSet(index);
	if( pAS == NULL )
	{
		return false;
	}

	WCHAR wszAnimName[ MAX_PATH ] = {0,};
	MultiByteToWideChar( CP_ACP, 0, pAS->GetName(), -1, wszAnimName, MAX_PATH );

	m_NowAnimName = wszAnimName;
	m_NowAnimIndex = index;

	retval = ChangeAnim( pAS, transition, fStartTime );

	return retval;
}

bool CKTDGXSkinAnim::ChangeAnim( const WCHAR* name, bool transition, float fStartTime )
{
	KTDXPROFILE();
	bool retval; 
	LPD3DXANIMATIONSET pAS;
	wstring tempName = name;
	//MakeUpperCase( tempName );
	pAS = GetAnimSet(tempName.c_str());
	if( pAS == NULL )
	{
		return false;
	}

	m_NowAnimName = tempName;
	for( int i = 0; i < (int)m_vAnimSetList.size(); i++ )
	{
		if( pAS == m_vAnimSetList[i] )
		{
			m_NowAnimIndex = i;
			break;
		}
	}

	retval = ChangeAnim( pAS, transition, fStartTime );	

	return retval;
}

bool CKTDGXSkinAnim::ChangeAnim( LPD3DXANIMATIONSET pAS, bool transition, float fStartTime )
{
	KTDXPROFILE();
	DWORD m_NewTrack = ( m_NowTrack == 0 ? 1 : 0 );
	if( transition == false )
		m_NewTrack = m_NowTrack;

	m_MotionMoveNow.x = 0.0f;
	m_MotionMoveNow.y = 0.0f;
	m_MotionMoveNow.z = 0.0f;

	m_MotionMoveBack.x = 0.0f;
	m_MotionMoveBack.y = 0.0f;
	m_MotionMoveBack.z = 0.0f;

	m_fAnimGlobalTime	= 0.0f;
	m_fAnimCurrTime		= 0.0f;
	m_fAnimBeforeTime	= 0.0f;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fAnimCurrSpeed    = 1.f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_pAC->ResetTime();

#ifdef ANIMATIONCONTROLLER_FIX
	// D3DXANIMATIONCONTROLLER 관련 수정-하형진
	m_pAC->UnkeyAllTrackEvents( 0 );
	m_pAC->UnkeyAllTrackEvents( 1 );
#else
	m_pAC->UnkeyAllTrackEvents( m_NowTrack );
	m_pAC->UnkeyAllTrackEvents( m_NewTrack );
#endif ANIMATIONCONTROLLER_FIX

	if( transition == true )
	{
		m_pAC->KeyTrackEnable( m_NowTrack, FALSE, m_fAnimGlobalTime + (m_fTransitionTime/m_fPlaySpeed) );
		m_pAC->KeyTrackSpeed( m_NowTrack, 0.0f, m_fAnimGlobalTime, (m_fTransitionTime/m_fPlaySpeed), D3DXTRANSITION_LINEAR );
		m_pAC->KeyTrackWeight( m_NowTrack, 0.0f, m_fAnimGlobalTime, (m_fTransitionTime/m_fPlaySpeed), D3DXTRANSITION_LINEAR );

		m_pAC->SetTrackAnimationSet( m_NewTrack, pAS );
		m_pAC->SetTrackEnable( m_NewTrack, TRUE );

#ifdef ANIMATIONCONTROLLER_FIX
		// D3DXANIMATIONCONTROLLER 관련 수정-하형진
		m_pAC->SetTrackSpeed( m_NewTrack, 0.f );
		m_pAC->SetTrackWeight( m_NewTrack, 0.f );
#endif ANIMATIONCONTROLLER_FIX

		m_pAC->KeyTrackSpeed( m_NewTrack, m_fPlaySpeed, m_fAnimGlobalTime, (m_fTransitionTime/m_fPlaySpeed), D3DXTRANSITION_LINEAR );
		m_pAC->KeyTrackWeight( m_NewTrack, 1.0f, m_fAnimGlobalTime, (m_fTransitionTime/m_fPlaySpeed), D3DXTRANSITION_LINEAR );

	}
	else
	{
#ifdef ANIMATIONCONTROLLER_FIX
		// D3DXANIMATIONCONTROLLER 관련 수정-하형진
		DWORD dwOldTrack = ( m_NewTrack == 0 ? 1 : 0 );
		m_pAC->SetTrackEnable( dwOldTrack, FALSE );
#endif ANIMATIONCONTROLLER_FIX

		m_pAC->SetTrackAnimationSet( m_NewTrack, pAS );
		m_pAC->SetTrackEnable( m_NewTrack, TRUE );
		m_pAC->KeyTrackSpeed( m_NewTrack, m_fPlaySpeed, m_fAnimGlobalTime, 0.00001f, D3DXTRANSITION_LINEAR );
		m_pAC->KeyTrackWeight( m_NewTrack, 1.0f, m_fAnimGlobalTime, 0.00001f, D3DXTRANSITION_LINEAR );
	}

	m_pAC->SetTrackPosition( m_NewTrack, fStartTime );

	m_fAnimMaxTime = (float)pAS->GetPeriod();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    D3DXTRACK_DESC td;
    m_pAC->GetTrackDesc( m_NewTrack, &td );
	//m_fAnimCurrTime = (float)td.Position;
 //   if ( m_fAnimCurrTime < 0.f )
 //       m_fAnimCurrTime = 0.f;
 //   if ( m_fAnimCurrTime > m_fAnimMaxTime )
 //       m_fAnimCurrTime = m_fAnimMaxTime;
    m_fAnimCurrSpeed = (float)td.Speed;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	for( int i = 0; i < (int)m_pModelAniXETList.size(); i++ )
	{
		CKTDXDeviceXET* pCKTDXDeviceXET = m_pModelAniXETList[i];
		if( pCKTDXDeviceXET == NULL )
			continue;

		const CKTDXDeviceXET::AniData* pAniData = pCKTDXDeviceXET->GetAniData( m_NowAnimName.c_str() );
		m_pModelAniDataList[i] = pAniData;
	}

	if( m_pAnimAniXET != NULL )
	{
		const CKTDXDeviceXET::AniData* pAniData = m_pAnimAniXET->GetAniData( m_NowAnimName.c_str() );
		if( pAniData != NULL )
		{
			if( pAniData->fTransitionTime == 0.0f )
				m_fTransitionTime = m_fAnimMaxTime / 10.0f;
			if( pAniData->fTransitionTime > m_fAnimMaxTime / 20.0f )
				m_fTransitionTime = pAniData->fTransitionTime;
			else
				m_fTransitionTime = m_fAnimMaxTime / 20.0f;
		}
		else
		{
			m_fTransitionTime = m_fAnimMaxTime / 10.0f;
		}
	}
	else
	{
		m_fTransitionTime = m_fAnimMaxTime / 10.0f;
	}	

#ifdef FIX_SKIN_ANIM_ONE_WAIT_BUG // 오현빈 // 2013-07-19
	// 0.3초 이하의 지속시간을 가진 모션을 ONE_WAIT로 설정 했을 때, 첫 프레임에서 멈추는 문제 수정
	// 오차범위로 체크하는 값이 한프레임인 0.03보다 낮아서 체크 무시 되는 경우가 발생.
	// m_fTransitionTime의 최저값을 0.03으로 수정
	m_fTransitionTime = max( m_fTransitionTime, 0.03f );
#endif // FIX_SKIN_ANIM_ONE_WAIT_BUG

	m_NowTrack = m_NewTrack;

	return true;
}

CKTDXDeviceXSkinMesh::MultiAnimFrame* CKTDGXSkinAnim::GetCloneFrame( const WCHAR* name )
{
	KTDXPROFILE();
	if( NULL == name )
		return NULL;
		
	map<wstring, CKTDXDeviceXSkinMesh::MultiAnimFrame*>::iterator iter = m_mCloneFrameMap.find( name );
	if( iter != m_mCloneFrameMap.end() )
	{
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = iter->second;
		return pFrame;
	}

	return NULL;
}



void CKTDGXSkinAnim::ResetAnimSet()
{
	HRESULT hr;
	LPD3DXANIMATIONSET pAS;

	for( int i = 0; i < (int)m_pAC->GetNumAnimationSets(); i++ )
	{
		pAS = NULL;
		m_vAnimSetList.push_back( pAS );

		hr = m_pAC->GetAnimationSet( i, &pAS );
		if( FAILED( hr ) )
			continue;

		m_vAnimSetList[i] = pAS;

		WCHAR wszAnimName[ MAX_PATH ];
		MultiByteToWideChar( CP_ACP, 0, pAS->GetName(), -1, wszAnimName, MAX_PATH );
		//MakeUpperCase( wszAnimName );

		if( m_mAnimSetList.find( wszAnimName ) == m_mAnimSetList.end() )
		{
			m_mAnimSetList.insert( std::make_pair( wszAnimName, pAS ) );
		}
		if( m_mIndexByName.find( wszAnimName ) == m_mIndexByName.end() )
		{
			m_mIndexByName.insert( std::make_pair( wszAnimName, i ) );
		}
	}

	m_NowTrack				= 0;
	m_NewTrack				= 1;

	m_fAnimGlobalTime		= 0.0f;
	m_fAnimCurrTime			= 0.0f;
	m_fAnimBeforeTime		= 0.0f;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fAnimCurrSpeed        = 1.f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fAnimMaxTime			= 0.0f;
}

bool CKTDGXSkinAnim::EventTimer( float fTime )
{
	if( m_fAnimBeforeTime < fTime
		&& m_fAnimCurrTime >= fTime )
		return true;
	else
		return false;
}

void CKTDGXSkinAnim::SetModelDetailPercent( int detailPercent )
{
	if ( detailPercent >= 100 )
		detailPercent = 100;

	if ( detailPercent <= 0 )
		detailPercent = 0;

	m_ModelDetailPercent = detailPercent;
}





#ifdef BOOST_SINGLETON_POOL_TEST
	typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CKTDGXSkinAnim)> CKTDGXSkinAnimPool;



	void* CKTDGXSkinAnim::operator new(size_t s)
	{
		return CKTDGXSkinAnimPool::malloc();
	}

	void CKTDGXSkinAnim::operator delete(void *p)
	{
		CKTDGXSkinAnimPool::free(p);
	}
#endif BOOST_SINGLETON_POOL_TEST







#ifdef SKIN_LIGHTFLOW
void CKTDGXSkinAnim::SetChildLightFlow(CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame)
{
    if(pFrame == NULL)
        return;

    if(pFrame->pMeshContainer)
    {
        m_bLightFlow = true;
        m_pLightFlow = pFrame;
        return;
    }
    else
    {
        if(pFrame->pFrameSibling)
            SetChildLightFlow((CKTDXDeviceXSkinMesh::MultiAnimFrame*)pFrame->pFrameSibling);
        
        if(pFrame->pFrameFirstChild)
            SetChildLightFlow((CKTDXDeviceXSkinMesh::MultiAnimFrame*)pFrame->pFrameFirstChild);
    }
    
}

void CKTDGXSkinAnim::SetLightFlow(wstring szName, CKTDGXRenderer::RENDER_TYPE flowType) 
{   
    if(szName.compare(L"") == 0)
        return;

    LPCSTR			tempName;    
    WCHAR			wszName[MAX_PATH] = L"";
    wstring         frameName;

    m_wstrLightFlowName = szName; 
    m_lightFlowType     = flowType;    

    m_fFlowMin = 0.0f;
    m_fFlowMax = 3000.0f;
    m_fFlowSpeed = 300.0f;
    m_fWide = 10.0f;
    m_coColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

    for( int i = 0; i < (int)m_pModelXSkinMeshList.size(); i++ )
    {        
        CKTDXDeviceXSkinMesh* pXSkinMesh = m_pModelXSkinMeshList[i];
        int maxFrameNum = pXSkinMesh->GetFrameNum();

        for( int j = 0; j < maxFrameNum; j++ )
        {            
          
            CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = pXSkinMesh->GetFrame(j);
#ifdef  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
            if ( pFrame == NULL )
                continue;
#endif  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
            tempName = pFrame->Name;
            MultiByteToWideChar( CP_ACP, 0, tempName, -1, wszName, MAX_PATH);
            frameName = wszName;

            if(m_wstrLightFlowName.compare(frameName.c_str()) == 0)
            {
                if(pFrame->pMeshContainer)
                {
                    m_bLightFlow = true;
                    m_pLightFlow = pFrame;
                    return;
                }
                else
                {
                    if(pFrame->pFrameFirstChild)
                        SetChildLightFlow((CKTDXDeviceXSkinMesh::MultiAnimFrame*)pFrame->pFrameFirstChild);
                }                
                return;
            }
        }
    }
}

HRESULT CKTDGXSkinAnim::LightFlowFrameMove(double fTime, float fElapsedTime)
{
    HRESULT hr = S_OK;

    m_fNowPoint += m_fFlowSpeed * fElapsedTime; 

    if( m_fNowPoint < m_fFlowMin )
        m_fNowPoint = m_fFlowMax;
    else if( m_fNowPoint > m_fFlowMax )
        m_fNowPoint = m_fFlowMin;

    return hr;
}

#endif

#if defined(WORLD_TOOL) || defined(X2TOOL)
bool CKTDGXSkinAnim::GetAnimNameList( std::vector<std::wstring>& vecAnimNameList )
{
	if( m_pAnimXSkinMesh == NULL )
	{
		return false;
	}

	LPD3DXANIMATIONCONTROLLER	pAC;
	LPD3DXANIMATIONSET			pAnimSet;
	UINT			aniNum;
	LPCSTR			szName;
	std::wstring	wstrName;
	WCHAR			wszName[128] = L"";

	pAC		= m_pAnimXSkinMesh->GetCloneAC();
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

#endif

#ifdef MODIFY_AFTER_IMAGE
/** @function : ClearModelXSkinMeshList
	@brief : XSkinMeshList 초기화 함수.

			 애프터 이미지 갱신 시 비용을 줄이기 위해 만든 함수.
			 다른 곳에서 불려졌을 때의 오류를 방지하기 위해 
			 m_CollisionDataList, m_AttackDataList 체크 함
*/
void CKTDGXSkinAnim::ClearModelXSkinMeshList()
{
	if( false == m_CollisionDataList.empty() ||
		false == m_AttackDataList.empty() )
	{
		ASSERT(!"ClearModelXSkinMeshList 사용 할 수 없는 상태입니다.");
	}

	m_pModelXSkinMeshList.clear();
	m_pModelAniXETList.clear();
	m_pModelAniDataList.clear();
	m_pModelMultiTexXETList.clear();
	m_pModelTexChangeXETList.clear();
}
#endif // MODIFY_AFTER_IMAGE