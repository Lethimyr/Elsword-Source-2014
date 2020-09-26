#include <StdAfx.h>


#include "KTDXDeviceXSkinMesh.h"


#ifdef _KSMTOOL
#define GET_D3DDEVICE()     DXUTGetD3DDevice()
#else
#define GET_D3DDEVICE()     g_pKTDXApp->GetDevice()
#endif // #ifdef _KSMTOOL

#define KSM_ATTRIBUTE_SORT


#pragma warning (disable: 4200)


// m_vCenter, m_fMaxBoundingRadius는 KSMFrame에 두는 것이 더 일관될 것이다.
// 하지만 현재 Elsword에서는 각 프레임에서 m_vCenter를 사용하지 않고, m_fBoundingRadius는
// 충돌체에 대해서만 사용하므로, 일단은 ...

#pragma pack( push, 1 )

struct  KSMFrame
{
    DWORD       m_dwOffset_Name;
    D3DXMATRIX  m_d3dmTransformationMatrix;
    float       m_fBoundingRadius;              // 충돌체 및 AttackList에 대해서만 > 0.f 의 값을 가진다.
    DWORD       m_dwOffset_MeshContainer;
    int         m_iIndex_FrameSibling;
    int         m_iIndex_FrameFirstChild;
};//struct  KSMFrame


struct  KSMHeader
{
    DWORD       m_dwMagic;
    DWORD       m_dwVersion;
    DWORD       m_dwOffset_AnimationController;
    D3DXVECTOR3 m_vCenter;              
    float       m_fMaxBoundingRadius;   
    DWORD       m_dwNumFrames;
    KSMFrame    m_aksmFrames[];
};//struct  KSMHeader


struct  KSMMeshContainer
{
    DWORD   m_dwOffset_Name;
    DWORD   m_dwFVF;
    DWORD   m_dwOptions;
    DWORD   m_dwNumMaterials;
    DWORD   m_dwOffset_Materials;
    DWORD   m_dwNumVertices;
    DWORD   m_dwOffset_Vertices;
    DWORD   m_dwNumFaces;
    DWORD   m_dwOffset_Indices;
    DWORD   m_dwOffset_Attributes;
    DWORD   m_dwNumBones;
    DWORD   m_dwOffset_Bones;
    DWORD   m_dwNumPaletteEntries;
    DWORD   m_dwOffset_NextMeshContainer;   // 현재는 사용되지 않음
};//struct  KSMMeshContainer


struct  KSMMaterial
{
    D3DMATERIAL9    m_d3dMat;
    DWORD           m_dwOffset_TextureFilename;
};//struct  KSMMaterial


struct  KSMBone
{
    D3DXMATRIX	m_d3dmBoneOffset;
    int         m_iIndex_BoneFrame;
    DWORD       m_dwNumInfluences;
    DWORD       m_dwOffset_Influences;
};//struct  KSMBone


#pragma pack( pop )




CKTDXDeviceXSkinMesh::XSkinMeshProxy::XSkinMeshProxy( const std::wstring& wstrDeviceID, CKTDXDevice* pThis, const std::string& strMoveBoneMB )
: m_DeviceID( wstrDeviceID )
, m_pThis( pThis )
, m_pFrameRoot( NULL )
, m_dwFrameNum( 0L )
//, m_apFrameList()
, m_vCenter( 0, 0, 0 )
, m_fMaxBoundingRadius( 0.f )
, m_pAC( NULL )
, m_strMoveBoneNameMB( strMoveBoneMB )
#ifndef _KSMTOOL 
, m_pXET( NULL )
#endif  
, m_dwDrawFrameNum( 0L )
//, m_apDrawFrameList()
, m_dwWorkingPaletteSize( 0L )
, m_amxWorkingPalette( NULL )
, m_dwMaxSkinningVertices( 0L )
{
    ZeroMemory( m_apFrameList, sizeof(MultiAnimFrame*) * MAX_FRAME_NUM );

	ZeroMemory( m_apDrawFrameList, sizeof(MultiAnimFrame*) * MAX_FRAME_NUM );
}//CKTDXDeviceXSkinMesh::XSkinMeshProxy::XSkinMeshProxy()


CKTDXDeviceXSkinMesh::XSkinMeshProxy::~XSkinMeshProxy()
{

	CMultiAnimAllocateHierarchy AH(m_strMoveBoneNameMB);

	m_dwWorkingPaletteSize = 0;
	SAFE_DELETE_ARRAY( m_amxWorkingPalette );

	SAFE_RELEASE( m_pAC );

	if( m_pFrameRoot != NULL )
	{
		D3DXFrameDestroy( m_pFrameRoot, &AH );
		m_pFrameRoot = NULL;
	}

#ifndef _KSMTOOL

	BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollision, m_CollisionDataList )
    {
		SAFE_DELETE( pCollision );
    }//
	m_CollisionDataList.clear();

    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pAttack, m_AttackDataList )
    {
        SAFE_DELETE( pAttack );
    }//
	m_AttackDataList.clear();

#endif // #ifndef _KSMTOOL

#ifndef _KSMTOOL
    if( g_pKTDXApp != NULL && g_pKTDXApp->GetAppDestroy() != true )
	{
		SAFE_CLOSE( m_pXET );
	}
    m_pXET = NULL;
#endif // #ifndef _KSMTOOL


}//CKTDXDeviceXSkinMesh::XSkinMeshProxy::~XSkinMeshProxy()
CKTDXDeviceXSkinMesh::CKTDXDeviceXSkinMesh( LPDIRECT3DDEVICE9 pd3dDevice, 
											std::wstring fileName, std::wstring moveBoneName )
											:CKTDXDevice( fileName, DT_XSKIN_MESH )
#ifdef ARA_CHARACTER_BASE
											, m_bUseRenderParam( false )
#endif
{
	m_dwFrameNum			= 0;


	m_vCenter			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_fMaxBoundingRadius	= 0.0f;
    m_pFrameRoot        = NULL;
	m_pAC				= NULL;

#ifndef _KSMTOOL
	m_pXET				= NULL;
#endif // #ifndef _KSMTOOL
	m_wstrMoveBoneName		= moveBoneName;
	ConvertWCHARToChar( m_strMoveBoneNameMB, m_wstrMoveBoneName.c_str() );

    //m_dwBoneNum = 0;

    ZeroMemory( m_apFrameList, sizeof(MultiAnimFrame*) * MAX_FRAME_NUM );
    m_dwDrawFrameNum    = 0;
	ZeroMemory( m_apDrawFrameList, sizeof(MultiAnimFrame*) * MAX_FRAME_NUM );
    m_dwWorkingPaletteSize = 0;
	m_amxWorkingPalette = NULL;
    m_dwMaxSkinningVertices  = 0;
    m_bAlphaRender = false;
#ifdef FACE_OFF_MONSTER_HEAD_TEST
	m_bNoRenderable = false;
#endif FACE_OFF_MONSTER_HEAD_TEST
}

CKTDXDeviceXSkinMesh::~CKTDXDeviceXSkinMesh(void)
{
	_UnLoad();
}




HRESULT CKTDXDeviceXSkinMesh::_Load( bool bSkipStateCheck /*= false*/
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD			
	)
{
	KTDXPROFILE();

	////CKTDXThread::CLocker locker( g_pKTDXApp->GetDeviceManager()->GetDeviceLock() );

	HRESULT hr = S_OK;

    DWORD   dwSize = 0;

    XSkinMeshProxy  proxy( GetDeviceID(), this, m_strMoveBoneNameMB );

    {
        const void* pData = NULL;


#ifdef _KSMTOOL


        KFileSerializer kFile;
	    std::string strFilename;
	    ConvertWCHARToChar( strFilename, GetDeviceID().c_str() );
	    hr = kFile.LoadFile( strFilename.c_str() );
	    if ( FAILED( hr ) )
	        return E_FAIL;

	    dwSize = kFile.GetSize();
	    pData = kFile.Lock();
	    if(pData == NULL)    
	        ErrorLogMsg(KEM_ERROR211, m_DeviceID.c_str());
	    ASSERT( pData != NULL );

#else

	    KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	    Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( m_DeviceID );
	    if( Info == NULL )
	    {
			wstring strName = L"LoadDataFile";
			strName += L" ";
			strName += m_DeviceID;
			ErrorLogMsg( KEM_ERROR100, strName.c_str());

		    return E_FAIL;
	    }


	    dwSize = Info->size;
	    pData = Info->pRealData;

#endif // #ifdef _KSMTOOL

        if ( dwSize >= sizeof(DWORD) && *static_cast<const DWORD*>( pData ) == KOGSKINNEDMESH_FORMAT_MAGIC )
	    {
		    hr = proxy.LoadKSkinnedMesh( pData, dwSize );
	    }
        else
	    {
            hr = proxy.LoadD3DXSkinnedMesh( pData, dwSize );


	    }
#ifdef _KSMTOOL

    kFile.Unlock();

#endif // #ifdef _KSMTOOL

        if ( FAILED( hr ) )
            return hr;

    }
    proxy.LoadTextures();

    proxy.Load_LuaData( 0L );

    {
        CSLock  locker( m_csDeviceState );

        if ( bSkipStateCheck == false )
        {
		    switch( m_eDeviceState )
		    {
		    case DEVICE_STATE_INIT:
		    //case DEVICE_STATE_PENDING:
		    default:
			    ASSERT( 0 );
		    case DEVICE_STATE_LOADING:
		    case DEVICE_STATE_FAILED:
                std::swap( m_pFrameRoot, proxy.m_pFrameRoot );
                {
                    MultiAnimFrame* apFrameListTemp[MAX_FRAME_NUM];
                    CopyMemory( apFrameListTemp, m_apFrameList, sizeof(MultiAnimFrame*) * m_dwFrameNum );
                    CopyMemory( m_apFrameList, proxy.m_apFrameList, sizeof(MultiAnimFrame*) * proxy.m_dwFrameNum );
                    CopyMemory( proxy.m_apFrameList, apFrameListTemp, sizeof(MultiAnimFrame*) * m_dwFrameNum );
                    std::swap( m_dwFrameNum, proxy.m_dwFrameNum );
                }
                m_vCenter = proxy.m_vCenter;
                m_fMaxBoundingRadius = proxy.m_fMaxBoundingRadius;
                std::swap( m_pAC, proxy.m_pAC );
                m_strMoveBoneNameMB.swap( proxy.m_strMoveBoneNameMB );
#ifndef _KSMTOOL
                std::swap( m_pXET, proxy.m_pXET );
                m_CollisionDataList.swap( proxy.m_CollisionDataList );
                m_AttackDataList.swap( proxy.m_AttackDataList );
#endif  _KSMTOOL
                m_dwDrawFrameNum = proxy.m_dwDrawFrameNum;  proxy.m_dwDrawFrameNum = 0;
                CopyMemory( m_apDrawFrameList, proxy.m_apDrawFrameList, sizeof(MultiAnimFrame*) * m_dwDrawFrameNum );
                m_dwWorkingPaletteSize = proxy.m_dwWorkingPaletteSize; proxy.m_dwWorkingPaletteSize = 0;
                std::swap( m_amxWorkingPalette, proxy.m_amxWorkingPalette );
                m_dwMaxSkinningVertices = proxy.m_dwMaxSkinningVertices;
	m_Size = dwSize;
		        m_eDeviceState = DEVICE_STATE_LOADED;
			    break;
		    case DEVICE_STATE_LOADED:
			    break;
		    }//switch
	    }
        else
        {
            std::swap( m_pFrameRoot, proxy.m_pFrameRoot );
            {
                MultiAnimFrame* apFrameListTemp[MAX_FRAME_NUM];
                CopyMemory( apFrameListTemp, m_apFrameList, sizeof(MultiAnimFrame*) * m_dwFrameNum );
                CopyMemory( m_apFrameList, proxy.m_apFrameList, sizeof(MultiAnimFrame*) * proxy.m_dwFrameNum );
                CopyMemory( proxy.m_apFrameList, apFrameListTemp, sizeof(MultiAnimFrame*) * m_dwFrameNum );
                std::swap( m_dwFrameNum, proxy.m_dwFrameNum );
            }
            m_vCenter = proxy.m_vCenter;
            m_fMaxBoundingRadius = proxy.m_fMaxBoundingRadius;
            std::swap( m_pAC, proxy.m_pAC );
            m_strMoveBoneNameMB.swap( proxy.m_strMoveBoneNameMB );
#ifndef _KSMTOOL
            std::swap( m_pXET, proxy.m_pXET );
            m_CollisionDataList.swap( proxy.m_CollisionDataList );
            m_AttackDataList.swap( proxy.m_AttackDataList );
#endif  _KSMTOOL
            m_dwDrawFrameNum = proxy.m_dwDrawFrameNum;  proxy.m_dwDrawFrameNum = 0;
            CopyMemory( m_apDrawFrameList, proxy.m_apDrawFrameList, sizeof(MultiAnimFrame*) * m_dwDrawFrameNum );
            m_dwWorkingPaletteSize = proxy.m_dwWorkingPaletteSize; proxy.m_dwWorkingPaletteSize = 0;
            std::swap( m_amxWorkingPalette, proxy.m_amxWorkingPalette );
            m_dwMaxSkinningVertices = proxy.m_dwMaxSkinningVertices;
            m_Size = dwSize;
        }//if.. else..
    }
    return S_OK;

}//CKTDXDeviceXSkinMesh::Load()



HRESULT CKTDXDeviceXSkinMesh::_UnLoad()
{
	////CKTDXThread::CLocker locker( g_pKTDXApp->GetDeviceManager()->GetDeviceLock() );

	CMultiAnimAllocateHierarchy AH(m_strMoveBoneNameMB);

	m_dwWorkingPaletteSize = 0;
	SAFE_DELETE_ARRAY( m_amxWorkingPalette );
	SAFE_RELEASE( m_pAC );

	if( m_pFrameRoot != NULL )
	{
		D3DXFrameDestroy( m_pFrameRoot, &AH );
		m_pFrameRoot = NULL;
	}

#ifndef _KSMTOOL

    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollision, m_CollisionDataList )
    {
		SAFE_DELETE( pCollision );
    }//
	m_CollisionDataList.clear();

    BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pAttack, m_AttackDataList )
    {
        SAFE_DELETE( pAttack );
    }//
	m_AttackDataList.clear();

#endif // #ifndef _KSMTOOL

#ifndef _KSMTOOL
    if( NULL != g_pKTDXApp || true != g_pKTDXApp->GetAppDestroy() )
	{
		SAFE_CLOSE( m_pXET );
	}
#endif // #ifndef _KSMTOOL

	return S_OK;
}


//{{ seojt // 2008-10-15, 14:50
void CKTDXDeviceXSkinMesh::PreLoad()
{
    for( DWORD i = 0; i < m_dwFrameNum; i++ )
    {
        if ( m_apFrameList[i] == NULL )
            continue;

        if ( m_apFrameList[i]->pMeshContainer != NULL )
        {
            ( (MultiAnimMC*)m_apFrameList[i]->pMeshContainer )->PreLoad();
        }//if
    }//for
}//CKTDXDeviceXSkinMesh::PreLoad()
//}} seojt // 2008-10-15, 14:50


HRESULT CKTDXDeviceXSkinMesh::XSkinMeshProxy::SetupFrameList( MultiAnimFrame* pFrame )
{
//{{ robobeg : 2008-01-17
    // 프레임 개수가 MAX_FRAME_NUM을 넘는 메쉬 파일은 처리하지 못한다.
    // 아마도 일반 메쉬를 스킨 메쉬로 잘못 읽은 경우일 것이다.
    ASSERT( pFrame != NULL );
    ASSERT( m_dwFrameNum < MAX_FRAME_NUM );
    if ( m_dwFrameNum >= MAX_FRAME_NUM )
    {
        ErrorLogMsg(KEM_ERROR348, m_DeviceID.c_str());
        return E_FAIL;
    }//if
//}} robobeg : 2008-01-17

    HRESULT hr;
    m_apFrameList[m_dwFrameNum] = pFrame;
    pFrame->m_iIndex = m_dwFrameNum;

    // 엘소드는 Frame당 Mesh가 최대 한 개인 것으로 코딩되어 있어서
    // 여러 개인 경우는 고려하지 않음. 추후 확장할 것!!!
    if ( pFrame->pMeshContainer != NULL )
    {
        if(pFrame->pMeshContainer->pNextMeshContainer != NULL)    
            ErrorLogMsg(KEM_ERROR212, m_DeviceID.c_str());
        ASSERT( pFrame->pMeshContainer->pNextMeshContainer == NULL );
    }//if

	m_dwFrameNum++;

	if( pFrame->pFrameSibling )
	{
		// recursive call
		hr = SetupFrameList( (MultiAnimFrame *) pFrame->pFrameSibling );
        if ( FAILED( hr ) )
            return hr;
	}//if

	if( pFrame->pFrameFirstChild )
	{
		// recursive call
		hr = SetupFrameList( (MultiAnimFrame *) pFrame->pFrameFirstChild );
        if ( FAILED( hr ) )
            return E_FAIL;
	}//if

    return S_OK;
}


CKTDXDeviceXSkinMesh::MultiAnimFrame* CKTDXDeviceXSkinMesh::FindFrame( const char* pszInName_ ) const
{
    if ( pszInName_ == NULL )
        return NULL;
    for ( DWORD i = 0; i < m_dwFrameNum; i++ )
    {
        if(m_apFrameList[ i ] == NULL)    
            ErrorLogMsg(KEM_ERROR213, m_DeviceID.c_str());
        ASSERT( m_apFrameList[ i ] != NULL );
        if ( m_apFrameList[ i ]->Name != NULL
            && strcmp( m_apFrameList[ i ]->Name, pszInName_ ) == 0 )
            return m_apFrameList[ i ];
    }//for

    return NULL;
}//CKTDXDeviceXSkinMesh::FindFrame()


HRESULT CKTDXDeviceXSkinMesh::XSkinMeshProxy::SetupBonePtrs()
{
	HRESULT hr;

    for( DWORD i = 0; i < m_dwFrameNum; i++ )
    {
        if(m_apFrameList[ i ] == NULL)    
            ErrorLogMsg(KEM_ERROR214, m_DeviceID.c_str());
        ASSERT( m_apFrameList[ i ] != NULL );
        if ( m_apFrameList[ i ]->pMeshContainer != NULL )
        {
            hr = ( (MultiAnimMC *) m_apFrameList[ i ]->pMeshContainer )->SetupBonePtrs( m_pFrameRoot );
            if ( FAILED( hr ) )
                return hr;
        }//if
    }//for

	return S_OK;
}//CKTDXDeviceXSkinMesh::SetupBonePtrs()


void    CKTDXDeviceXSkinMesh::XSkinMeshProxy::EstimateDrawFrameWorkingPaletteSizeAndBoneNum()
{
    if(m_amxWorkingPalette != NULL)    
        ErrorLogMsg(KEM_ERROR215, m_DeviceID.c_str());
    ASSERT( m_amxWorkingPalette == NULL );
    //m_dwBoneNum = 0;
    m_dwWorkingPaletteSize = 0;
    m_amxWorkingPalette = NULL;

    for( DWORD i = 0; i < m_dwDrawFrameNum; i++ )
    {
        if( m_apDrawFrameList[ i ] == NULL || m_apDrawFrameList[ i ]->pMeshContainer == NULL)
            ErrorLogMsg(KEM_ERROR216, m_DeviceID.c_str());
        ASSERT( m_apDrawFrameList[ i ] != NULL && m_apDrawFrameList[ i ]->pMeshContainer != NULL );
        if ( m_apDrawFrameList[ i ]->pMeshContainer != NULL )
        {
            MultiAnimMC* pMC = static_cast<MultiAnimMC*>( m_apDrawFrameList[ i ]->pMeshContainer );

            //if( m_dwBoneNum < pMC->m_dwNumBones )
			    //m_dwBoneNum = (int) pMC->m_dwNumBones;

            if ( m_dwWorkingPaletteSize < pMC->m_dwNumPaletteEntries )
                m_dwWorkingPaletteSize = (int) pMC->m_dwNumPaletteEntries;
        }//if
    }//for

	m_amxWorkingPalette		= new D3DXMATRIX[ m_dwWorkingPaletteSize ];
    if(m_amxWorkingPalette == NULL)    
        ErrorLogMsg(KEM_ERROR218, m_DeviceID.c_str());
    ASSERT( m_amxWorkingPalette != NULL );

}//CKTDXDeviceXSkinMesh::EstimateDrawFrameWorkingPaletteSizeAndBoneNum()


void    CKTDXDeviceXSkinMesh::XSkinMeshProxy::SetupDrawFrameList( MultiAnimFrame* pFrame )
{
    int iNameLen = 0;
    if ( pFrame->Name != NULL )
        iNameLen = strlen( pFrame->Name );
  

	if( pFrame->pMeshContainer )
	{
		if( ( iNameLen < 9 || memcmp( pFrame->Name, "COLLISION", 9 ) != 0 )
			&& ( iNameLen < 6 || memcmp( pFrame->Name, "ATTACK", 6 ) != 0 )
			&& ( iNameLen < 15 || memcmp( pFrame->Name, "Bip01_Footsteps", 15 ) != 0 ) )
		{
			m_apDrawFrameList[m_dwDrawFrameNum] = pFrame;
			m_dwDrawFrameNum++;
		}
	}

	if( pFrame->pFrameSibling )
		SetupDrawFrameList( (MultiAnimFrame *) pFrame->pFrameSibling );

	if( pFrame->pFrameFirstChild )
	{
		if( ( iNameLen >= 9 && memcmp( pFrame->Name, "COLLISION", 9 ) == 0 )
			|| ( iNameLen >= 6 && memcmp( pFrame->Name, "ATTACK", 6 ) == 0 )
			|| ( iNameLen >= 15 && memcmp( pFrame->Name, "Bip01_Footsteps", 15 ) == 0 ) )
		{
			return;
		}
		SetupDrawFrameList( (MultiAnimFrame *) pFrame->pFrameFirstChild );
	}
}

void CKTDXDeviceXSkinMesh::XSkinMeshProxy::ComputeCollisionData( bool bInValidRadius_ )
{
    MultiAnimFrame* pFrame;
    int iNameLen;
    for( DWORD dwFrame = 0; dwFrame < m_dwFrameNum; dwFrame++ )
    {
        pFrame = m_apFrameList[ dwFrame ];
        if(pFrame == NULL)    
            ErrorLogMsg(KEM_ERROR219, m_DeviceID.c_str());
        ASSERT( pFrame != NULL );
        iNameLen = 0;
        if ( pFrame->Name != NULL )
            iNameLen = strlen( pFrame->Name );
        if ( ( iNameLen >= 9 && memcmp( pFrame->Name, "COLLISION", 9 ) == 0 )
             || ( iNameLen >= 6 && memcmp( pFrame->Name, "ATTACK", 6 ) == 0 ) )
        {
            AddCollisionData( pFrame, bInValidRadius_ );
        }//if
    }//for
}//CKTDXDeviceXSkinMesh::ComputeCollisionData()


void CKTDXDeviceXSkinMesh::XSkinMeshProxy::AddCollisionData( MultiAnimFrame* pFrame, bool bInValidRadius_ )
{
    if ( pFrame->Name == NULL || pFrame->Name[0] == NULL )
        return ;

	//COLLISION_LINE_START0_NAME
	//COLLISION_LINE_END0
	//COLLISION_SPHERE0_NAME

	//ATTACK_LINE_START0_NAME
	//ATTACK_LINE_END0
	//ATTACK_SPHERE0_NAME

	WCHAR charBuffer[10] = {0,};
	std::wstring frameName;
	ConvertCharToWCHAR( frameName, pFrame->Name );

	if( frameName.compare( 0, 20, L"COLLISION_LINE_START" ) == 0 )
	{
        std::wstring    wstrName = L"";

        int i;
		for( i = 20; i < (int)frameName.size(); i++ )
		{
			WCHAR chr = *(frameName.c_str()+i);
			if( chr == L'_' )
			{
				wstrName = frameName.c_str() + (i+1); // 이름 넣기
				break;
			}
            ASSERT( i-20 < ARRAY_SIZE( charBuffer ) );
			charBuffer[i-20] = chr;
		}
        ASSERT( i-20 < ARRAY_SIZE( charBuffer ) );
        charBuffer[i-20] = 0;
		int collisionNum = _wtoi( charBuffer );

		WCHAR endLineFrameName[100] = {0,};

#ifdef	CONVERSION_VS
		swprintf_s( endLineFrameName, _countof(endLineFrameName), L"COLLISION_LINE_END%d", collisionNum );
#else	CONVERSION_VS
		swprintf( endLineFrameName, L"COLLISION_LINE_END%d", collisionNum );
#endif	CONVERSION_VS
		
		
        if ( bInValidRadius_ )
        {
            if ( pFrame->fBoundingRadius == 0.0f )
                return;
        }
        else
        {
		    std::string strBuffer;
		    ConvertWCHARToChar( strBuffer, endLineFrameName );
		    CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameEnd = (CKTDXDeviceXSkinMesh::MultiAnimFrame*)D3DXFrameFind( m_pFrameRoot, strBuffer.c_str() );
		    if( pFrameEnd == NULL )
			    return;
		    D3DXVECTOR3 vCenter;
		    D3DXFrameCalculateBoundingSphere( pFrame, &vCenter, &pFrame->fBoundingRadius );
		    if( pFrame->fBoundingRadius == 0.0f )
			    return;
        }//if.. else..

#ifndef _KSMTOOL

	    CKTDXCollision::CollisionData* pCollisionData = new CKTDXCollision::CollisionData();
        ASSERT( pCollisionData != NULL );
		pCollisionData->m_CollisionType = CKTDXCollision::CT_LINE;
        pCollisionData->m_Name = wstrName;
		pCollisionData->m_FrameName = frameName;
		pCollisionData->m_FrameNameLineEnd		= endLineFrameName;
		pCollisionData->m_pDeviceMesh			= m_pThis;
		pCollisionData->m_fRadius				= pFrame->fBoundingRadius;

		m_CollisionDataList.push_back( pCollisionData );

#endif // #ifndef _KSMTOOL
	}
	else if( frameName.compare( 0, 16, L"COLLISION_SPHERE" ) == 0 )
	{

        std::wstring    wstrName = L"";

        int i;
		for( i = 16; i < (int)frameName.size(); i++ )
		{
			WCHAR chr = *(frameName.c_str()+i);
			if( chr == L'_' )
			{
				wstrName = frameName.c_str() + (i+1); // 이름 넣기
				break;
			}
            ASSERT( i-16 < ARRAY_SIZE( charBuffer ) );
			charBuffer[i-16] = chr;
		}
        ASSERT( i-16 < ARRAY_SIZE( charBuffer ) );
        charBuffer[i-16] = 0;
		int collisionNum = _wtoi( charBuffer );

        if ( bInValidRadius_ )
        {
            if ( pFrame->fBoundingRadius == 0.0f )
                return;
        }
        else
        {
		    D3DXVECTOR3 vCenter;
		    D3DXFrameCalculateBoundingSphere( pFrame, &vCenter, &pFrame->fBoundingRadius );
		    if( pFrame->fBoundingRadius == 0.0f )
			    return;
        }//if.. else..

#ifndef _KSMTOOL

	    CKTDXCollision::CollisionData* pCollisionData = new CKTDXCollision::CollisionData();
        ASSERT( pCollisionData != NULL );
		pCollisionData->m_CollisionType = CKTDXCollision::CT_SPHERE;
        pCollisionData->m_Name = wstrName;
		pCollisionData->m_FrameName = frameName;
		pCollisionData->m_pDeviceMesh			= m_pThis;
		pCollisionData->m_fRadius				= pFrame->fBoundingRadius;

		m_CollisionDataList.push_back( pCollisionData );

#endif // #ifndef _KSMTOOL
	}
    else if( frameName.compare( 0, 15, L"COLLISION_GUARD" ) == 0 )
    {
        std::wstring    wstrName = L"";

        int i;
        for( i = 16; i < (int)frameName.size(); i++ )
        {
            WCHAR chr = *(frameName.c_str()+i);
            if( chr == L'_' )
            {
                wstrName = frameName.c_str() + (i+1); // 이름 넣기
                break;
            }
            ASSERT( i-16 < ARRAY_SIZE( charBuffer ) );
            charBuffer[i-16] = chr;
        }
        ASSERT( i-16 < ARRAY_SIZE( charBuffer ) );
        charBuffer[i-16] = 0;
        int collisionNum = _wtoi( charBuffer );

        if ( bInValidRadius_ )
        {
            if ( pFrame->fBoundingRadius == 0.0f )
            {
                D3DXVECTOR3 vCenter;
                D3DXFrameCalculateBoundingSphere( pFrame, &vCenter, &pFrame->fBoundingRadius );
                if( pFrame->fBoundingRadius == 0.0f )
                    return;
            }
        }
        else
        {
            D3DXVECTOR3 vCenter;
            D3DXFrameCalculateBoundingSphere( pFrame, &vCenter, &pFrame->fBoundingRadius );
            if( pFrame->fBoundingRadius == 0.0f )
                return;
        }//if.. else..

#ifndef _KSMTOOL

        CKTDXCollision::CollisionData* pCollisionData = new CKTDXCollision::CollisionData();
        ASSERT( pCollisionData != NULL );
        pCollisionData->m_CollisionType = CKTDXCollision::CT_GUARD;
        pCollisionData->m_Name = wstrName;
        pCollisionData->m_FrameName = frameName;
        pCollisionData->m_pDeviceMesh			= m_pThis;
        pCollisionData->m_fRadius				= pFrame->fBoundingRadius;
        
        m_CollisionDataList.insert(m_CollisionDataList.begin(), pCollisionData);
        //m_CollisionDataList.push_back( pCollisionData );

#endif // #ifndef _KSMTOOL
    }
	else if( frameName.compare( 0, 17, L"ATTACK_LINE_START" ) == 0 )
	{
        std::wstring    wstrName = L"";

        int i;
		for( i = 17; i < (int)frameName.size(); i++ )
		{
			WCHAR chr = *(frameName.c_str()+i);
			if( chr == L'_' )
			{
				wstrName = frameName.c_str() + (i+1); // 이름 넣기
				break;
			}
            ASSERT( i-17 < ARRAY_SIZE( charBuffer ) );
			charBuffer[i-17] = chr;
		}		
        ASSERT( i-17 < ARRAY_SIZE( charBuffer ) );
        charBuffer[i-17] = 0;
		int collisionNum = _wtoi( charBuffer );
		WCHAR endLineFrameName[100] = {0,};

#ifdef	CONVERSION_VS
		swprintf( endLineFrameName, _countof(endLineFrameName), L"ATTACK_LINE_END%d", collisionNum );
#else	CONVERSION_VS
		swprintf( endLineFrameName, L"ATTACK_LINE_END%d", collisionNum );
#endif	CONVERSION_VS
		
        if ( bInValidRadius_ )
        {
            if ( pFrame->fBoundingRadius == 0.0f )
                return;
        }
        else
        {
		    std::string strBuffer;
		    ConvertWCHARToChar( strBuffer, endLineFrameName );
		    CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameEnd = (CKTDXDeviceXSkinMesh::MultiAnimFrame*)D3DXFrameFind( m_pFrameRoot, strBuffer.c_str() );
		    if( pFrameEnd == NULL )
			    return;
    		
		    D3DXVECTOR3 vCenter;
		    D3DXFrameCalculateBoundingSphere( pFrame, &vCenter, &pFrame->fBoundingRadius );
		    if( pFrame->fBoundingRadius == 0.0f )
			    return;
        }//if.. else..

#ifndef _KSMTOOL

	    CKTDXCollision::CollisionData* pCollisionData = new CKTDXCollision::CollisionData();
        ASSERT( pCollisionData != NULL );
		pCollisionData->m_CollisionType = CKTDXCollision::CT_LINE;
        pCollisionData->m_Name = wstrName;
		pCollisionData->m_FrameName = frameName;
		pCollisionData->m_FrameNameLineEnd		= endLineFrameName;
		pCollisionData->m_pDeviceMesh				= m_pThis;
		pCollisionData->m_fRadius					= pFrame->fBoundingRadius;

		m_AttackDataList.push_back( pCollisionData );

#endif // #ifndef _KSMTOOL
	}
	else if( frameName.compare( 0, 13, L"ATTACK_SPHERE" ) == 0 )
	{
        std::wstring    wstrName = L"";

        int i;
        for( i = 13; i < (int)frameName.size(); i++ )
		{
			WCHAR chr = *(frameName.c_str()+i);
			if( chr == L'_' )
			{
				wstrName = frameName.c_str() + (i+1); // 이름 넣기
				break;
			}
            ASSERT( i-13 < ARRAY_SIZE( charBuffer ) );
			charBuffer[i-13] = chr;
		}
        ASSERT( i-13 < ARRAY_SIZE( charBuffer ) );
    	charBuffer[i-13] = 0;
		int collisionNum = _wtoi( charBuffer );

        if ( bInValidRadius_ )
        {
            if ( pFrame->fBoundingRadius == 0.0f )
                return;
        }
        else
        {
            D3DXVECTOR3 vCenter;
		    D3DXFrameCalculateBoundingSphere( pFrame, &vCenter, &pFrame->fBoundingRadius );
		    if( pFrame->fBoundingRadius == 0.0f )
			    return;
        }//if.. else..


#ifndef _KSMTOOL

	    CKTDXCollision::CollisionData* pCollisionData = new CKTDXCollision::CollisionData();
        ASSERT( pCollisionData != NULL );
		pCollisionData->m_CollisionType = CKTDXCollision::CT_SPHERE;
        pCollisionData->m_Name = wstrName;
		pCollisionData->m_FrameName = frameName;
		pCollisionData->m_pDeviceMesh			= m_pThis;
		pCollisionData->m_fRadius				= pFrame->fBoundingRadius;

		m_AttackDataList.push_back( pCollisionData );

#endif // #ifndef _KSMTOOL
	}//if.. else..
}

HRESULT CKTDXDeviceXSkinMesh::XSkinMeshProxy::ConvertDrawFrameToSkinnedMesh()
{
	HRESULT hr;

    for( DWORD i = 0; i < m_dwDrawFrameNum; i++ )
    {
        if(m_apDrawFrameList[ i ] == NULL || m_apDrawFrameList[ i ]->pMeshContainer == NULL)    
            ErrorLogMsg(KEM_ERROR220, m_DeviceID.c_str());
        ASSERT( m_apDrawFrameList[ i ] != NULL && m_apDrawFrameList[ i ]->pMeshContainer != NULL );
        if ( m_apDrawFrameList[ i ]->pMeshContainer != NULL )
        {
            MultiAnimMC* pMC = static_cast<MultiAnimMC*>( m_apDrawFrameList[ i ]->pMeshContainer );
            hr = pMC->ConvertToSkinnedMesh();
            if ( FAILED( hr ) )
                return hr;
            if ( m_dwMaxSkinningVertices < pMC->m_dwMaxSkinningVertices )
                m_dwMaxSkinningVertices = pMC->m_dwMaxSkinningVertices;
        }//if
    }//for

    return S_OK;
}//CKTDXDeviceXSkinMesh::ConvertDrawFrameToSkinnedMesh()


void    CKTDXDeviceXSkinMesh::XSkinMeshProxy::OptimizeDrawFrameMesh()
{   
    for( DWORD i = 0; i < m_dwDrawFrameNum; i++ )
    {
        if(m_apDrawFrameList[ i ] == NULL || m_apDrawFrameList[ i ]->pMeshContainer == NULL)    
            ErrorLogMsg(KEM_ERROR221, m_DeviceID.c_str());
        ASSERT( m_apDrawFrameList[ i ] != NULL && m_apDrawFrameList[ i ]->pMeshContainer != NULL );
        CKTDXDeviceXSkinMesh::MultiAnimMC* pMC = static_cast<CKTDXDeviceXSkinMesh::MultiAnimMC*>( m_apDrawFrameList[ i ]->pMeshContainer );
        pMC->OptimizeMesh();
    }//for
}//CKTDXDeviceXSkinMesh::OptimizeDrawFrameMesh()



#ifdef _KSMTOOL


HRESULT CKTDXDeviceXSkinMesh::WriteToFileSerializer( KFileSerializer& kInOutFile_ ) const
{
    if ( !m_bLoading )
        return E_FAIL;

    DWORD   dwStructSize = sizeof(KSMHeader) + m_dwFrameNum * sizeof(KSMFrame);
    DWORD   dwOffset_Start = kInOutFile_.AppendSpace( dwStructSize );

    KSMHeader*  pksmHeader = (KSMHeader*) malloc( dwStructSize );
    pksmHeader->m_dwMagic = KOGSKINNEDMESH_FORMAT_MAGIC;
    pksmHeader->m_dwVersion = KOGSKINNEDMESH_FORMAT_VERSION;
    pksmHeader->m_dwOffset_AnimationController = NULL;
    pksmHeader->m_vCenter = m_vCenter;
    pksmHeader->m_fMaxBoundingRadius = m_fMaxBoundingRadius;
    pksmHeader->m_dwNumFrames = m_dwFrameNum;

    DWORD   dwOffset_FrameStart = dwOffset_Start + sizeof(KSMHeader);
    HRESULT hr;
    for( DWORD iFrame = 0; iFrame < m_dwFrameNum; iFrame++ )
    {
        DWORD   dwOffset_Frame = dwOffset_FrameStart + iFrame * sizeof(KSMFrame);
        MultiAnimFrame* pFrame = m_apFrameList[ iFrame ];
        if(pFrame == NULL)    
            ErrorLogMsg(KEM_ERROR222, m_DeviceID.c_str());
        ASSERT( pFrame != NULL );
        hr = pFrame->WriteToFileSerializer( kInOutFile_, dwOffset_Frame );
        if(FAILED(hr))    
            ErrorLogMsg(KEM_ERROR223, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
    }//for

    if ( m_pAC != NULL )
    {
        pksmHeader->m_dwOffset_AnimationController = kInOutFile_.GetSize();
        KFileSerializer kACFile;
        hr = KTDXAnimationController::WriteToFileSerializer( kACFile, this, m_pAC );
        if(FAILED(hr))    
            ErrorLogMsg(KEM_ERROR224, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
        void* pData = kACFile.Lock();
        kInOutFile_.AppendData( kACFile.GetSize(), pData );
        kACFile.Unlock();
    }//if

    kInOutFile_.CopyData( dwOffset_Start, sizeof(KSMHeader), pksmHeader );
    SAFE_FREE( pksmHeader );

    return S_OK;
}//CKTDXDeviceXSkinMesh::WriteToFileSerializer()

#endif // #ifdef _KSMTOOL


HRESULT CKTDXDeviceXSkinMesh::XSkinMeshProxy::LoadKSkinnedMesh( const void* pInData_, DWORD dwInSize_ )
{
    if(dwInSize_ != 0 && pInData_ == NULL)    
        ErrorLogMsg(KEM_ERROR225, m_DeviceID.c_str());
    ASSERT( dwInSize_ == 0 || pInData_ != NULL );
    HRESULT hr;

    KMemoryDeserializer kMemory( dwInSize_, pInData_ );
    KSMHeader   ksmHeader;
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
    hr = kMemory.RetrieveData( &ksmHeader, 0, sizeof(KSMHeader) );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//    hr = kMemory.RetrieveData( 0, sizeof(KSMHeader), &ksmHeader );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
    if(FAILED(hr))    
        ErrorLogMsg(KEM_ERROR226, m_DeviceID.c_str());
    ASSERT( SUCCEEDED( hr ) );
    if ( FAILED( hr ) )
        return E_FAIL;

    if ( ksmHeader.m_dwMagic != KOGSKINNEDMESH_FORMAT_MAGIC )
    {        
        ErrorLogMsg(KEM_ERROR227, m_DeviceID.c_str());
        ASSERT( !"Invalid Format" );
        return E_FAIL;
    }//if
    if ( ksmHeader.m_dwVersion != KOGSKINNEDMESH_FORMAT_VERSION )
    {
        ErrorLogMsg(KEM_ERROR228, m_DeviceID.c_str());
        ASSERT( !"Invalid Version" );
        return E_FAIL;
    }//if

	m_dwDrawFrameNum = 0;
    m_dwWorkingPaletteSize = 0;
    //m_dwBoneNum = 0;
    m_amxWorkingPalette = NULL;

    m_vCenter = ksmHeader.m_vCenter;
    m_fMaxBoundingRadius = ksmHeader.m_fMaxBoundingRadius;

	CMultiAnimAllocateHierarchy AH(m_strMoveBoneNameMB);
	//AH.m_wstrMoveBoneName		= m_wstrMoveBoneName;
	//AH.m_strMoveBoneNameMB		= m_strMoveBoneNameMB;

    m_dwFrameNum = ksmHeader.m_dwNumFrames;
    if(m_dwFrameNum > MAX_FRAME_NUM)    
        ErrorLogMsg(KEM_ERROR229, m_DeviceID.c_str());
    ASSERT( m_dwFrameNum <= MAX_FRAME_NUM );
    if ( m_dwFrameNum > MAX_FRAME_NUM )
    {           
        ErrorLogMsg(KEM_ERROR230, m_DeviceID.c_str());
        ASSERT( !"The number of frames exceeds the limit." );
        return E_FAIL;
    }//if

    // AH 내부 동작을 흉내내어 먼저 D3DXFRAME 구조체를 생성한다.
    // 단, array로 먼저 읽어들인 뒤 valid한 tree 계층구조인지를 확인한다.

    DWORD   adwFrameRefCount[MAX_FRAME_NUM];
    ZeroMemory( adwFrameRefCount, sizeof(DWORD) * MAX_FRAME_NUM );
    const KSMFrame* pKSMFrame = static_cast<const KSMFrame*>( kMemory.AccessData( sizeof(KSMHeader), m_dwFrameNum * sizeof(KSMFrame) ) );
    if(pKSMFrame == NULL)    
        ErrorLogMsg(KEM_ERROR231, m_DeviceID.c_str());
    ASSERT( pKSMFrame != NULL );
    if ( pKSMFrame == NULL )
    {
        ErrorLogMsg(KEM_ERROR232, m_DeviceID.c_str());
        ASSERT( !"Frame data space is not enough" );
        return E_FAIL;
    }//if

    ZeroMemory( m_apFrameList, sizeof(MultiAnimFrame*) * MAX_FRAME_NUM );
    MultiAnimFrame* pAnimFrame;
    for( DWORD dwFrame = 0; dwFrame < m_dwFrameNum; dwFrame++ )
    {
        const KSMFrame& ksmFrame = pKSMFrame[ dwFrame ];
        const char* pszName = NULL;
        if ( ksmFrame.m_dwOffset_Name != NULL )
        {
            pszName = kMemory.RetrieveDupString( ksmFrame.m_dwOffset_Name );
            if(pszName == NULL)    
                ErrorLogMsg(KEM_ERROR233, m_DeviceID.c_str());
            ASSERT( pszName != NULL );
        }//if
        LPD3DXFRAME pd3dxFrame = NULL;
        hr = AH.CreateFrame( pszName, &pd3dxFrame );
        SAFE_DELETE_ARRAY( pszName );
        if ( FAILED( hr ) )
        {
            ErrorLogMsg(KEM_ERROR234, m_DeviceID.c_str());
            ASSERT( !"CreateFrame failed" );
            if(pd3dxFrame != NULL)    
                ErrorLogMsg(KEM_ERROR235, m_DeviceID.c_str());
            ASSERT( pd3dxFrame == NULL );
            goto delete_frames;
        }//if
        if(pd3dxFrame == NULL)    
            ErrorLogMsg(KEM_ERROR236, m_DeviceID.c_str());
        ASSERT( pd3dxFrame != NULL );
        m_apFrameList[ dwFrame ] = pAnimFrame = static_cast<MultiAnimFrame*>( pd3dxFrame );
        pAnimFrame->m_iIndex = (int) dwFrame;
        pAnimFrame->TransformationMatrix = ksmFrame.m_d3dmTransformationMatrix;
        pAnimFrame->fBoundingRadius = ksmFrame.m_fBoundingRadius;
    }//for
    // 프레임간의 sibling, child 관계를 만들어준다.
    for( DWORD dwFrame = 0; dwFrame < m_dwFrameNum; dwFrame++ )
    {
        const KSMFrame& ksmFrame = pKSMFrame[ dwFrame ];
        pAnimFrame = m_apFrameList[ dwFrame ];
        if ( ksmFrame.m_iIndex_FrameSibling < 0 )
            pAnimFrame->pFrameSibling = NULL;
        else if ( ksmFrame.m_iIndex_FrameSibling < (int) m_dwFrameNum )
        {
            pAnimFrame->pFrameSibling = m_apFrameList[ ksmFrame.m_iIndex_FrameSibling ];
            adwFrameRefCount[ ksmFrame.m_iIndex_FrameSibling ]++;
        }
        else
        {
            ErrorLogMsg(KEM_ERROR237, m_DeviceID.c_str());
            ASSERT( !"Invalid frame index" );
            goto delete_frames;
        }//if.. else..
        if ( ksmFrame.m_iIndex_FrameFirstChild < 0 )
            pAnimFrame->pFrameFirstChild = NULL;
        else if ( ksmFrame.m_iIndex_FrameFirstChild < (int) m_dwFrameNum )
        {
            pAnimFrame->pFrameFirstChild = m_apFrameList[ ksmFrame.m_iIndex_FrameFirstChild ];
            adwFrameRefCount[ ksmFrame.m_iIndex_FrameFirstChild ]++;
        }
        else
        {
            ErrorLogMsg(KEM_ERROR238, m_DeviceID.c_str());
            ASSERT( !"Invalid frame index" );
            goto delete_frames;
        }//if.. else..
    }//for
    if ( adwFrameRefCount[ 0 ] != 0 )
    {
        ErrorLogMsg(KEM_ERROR239, m_DeviceID.c_str());
        ASSERT( !"root frame is referenced by other frames" );
        goto delete_frames;
    }//if
    for( DWORD dwFrame = 1; dwFrame < m_dwFrameNum; dwFrame++ )
    {
        if ( adwFrameRefCount[ dwFrame ] != 1 )
        {
            ErrorLogMsg(KEM_ERROR240, m_DeviceID.c_str());
            ASSERT( !"invalid frame tree hierarchy" );
            goto delete_frames;
        }//if
    }//for

    // 이 이후부터는 오류발생시 D3DXFrameDestroy( m_pFrameRoot, &AH ); 를 사용해도 된다.
    m_pFrameRoot = m_apFrameList[ 0 ];

    // AH 내부 동작을 흉내내어 먼저 D3DXMESHCONTAINER 구조체를 생성한다.
    // 단, CMultiAnimAllocateHierarchy::CreateMeshContainer 대신MultiAnimMC::ReadKSM 을 이용한다.

    for( DWORD dwFrame = 0; dwFrame < m_dwFrameNum; dwFrame++ )
    {
        const KSMFrame& ksmFrame = pKSMFrame[ dwFrame ];
        if ( ksmFrame.m_dwOffset_MeshContainer != NULL )
        {
            MultiAnimMC* pMC = new MultiAnimMC;
            if(pMC == NULL)    
                ErrorLogMsg(KEM_ERROR241, m_DeviceID.c_str());
            ASSERT( pMC != NULL );
            hr = pMC->ReadKSM( *this, kMemory, ksmFrame.m_dwOffset_MeshContainer );
            if ( FAILED( hr ) )
            {
                ErrorLogMsg(KEM_ERROR242, m_DeviceID.c_str());
                ASSERT( !"reading MultiAnimMC failed" );
                SAFE_DELETE( pMC );
                return E_FAIL;
            }//if
            m_apFrameList[ dwFrame ]->pMeshContainer = pMC;
        }//if
    }//for

    // D3DXAnimationController 자료를 읽음

    if(m_pAC != NULL)    
        ErrorLogMsg(KEM_ERROR243, m_DeviceID.c_str());
    ASSERT( m_pAC == NULL );
    if ( ksmHeader.m_dwOffset_AnimationController != NULL )
    {
        DWORD   dwSize = kMemory.GetSize() - ksmHeader.m_dwOffset_AnimationController;
        const void* pACData = kMemory.AccessData( ksmHeader.m_dwOffset_AnimationController, dwSize );
        if(pACData == NULL)    
            ErrorLogMsg(KEM_ERROR244, m_DeviceID.c_str());
        ASSERT( pACData != NULL );
        if ( pACData == NULL )
        { 
            ErrorLogMsg(KEM_ERROR245, m_DeviceID.c_str());
            ASSERT( !"invalid KSMHeader::m_dwOffset_AnimationController" );
            return E_FAIL;
        }//if

        hr = KTDXAnimationController::ReadFromMemory( dwSize, pACData, *this, &m_pAC );
        if ( FAILED( hr ) )
        {
            ErrorLogMsg(KEM_ERROR246, m_DeviceID.c_str());
            ASSERT( !"Loading D3DXAnimationController failed" );
            return E_FAIL;
        }//if
    }//if

    // 텍스쳐를 로딩한다.

	SetupDrawFrameList( m_pFrameRoot );
    EstimateDrawFrameWorkingPaletteSizeAndBoneNum();
	ComputeCollisionData( true );

//{{ robobeg : 2008-01-03
    // 렌더링 메쉬에 대한 메쉬 최적화를 별도 함수로 옮긴다.
    OptimizeDrawFrameMesh();

#ifndef _KSMTOOL
    // 메쉬 최적화 후 Skinned Mesh를 생성한다.
    hr = ConvertDrawFrameToSkinnedMesh();
    if ( FAILED( hr ) )
    {
        ErrorLogMsg(KEM_ERROR247, m_DeviceID.c_str());
        ASSERT( !"Skin mesh conversion failed" );
        return hr;
    }//if

    ReleaseUnusedInfo();

#endif // #ifndef _KSMTOOL

//}} robobeg : 2008-01-03

    return S_OK;

delete_frames:

    // 아직 프레임 트리 hierarchy의 validity가 체크되지 않은 상태이므로 직접 프레임들을 지운다.
    for( DWORD dwFrame = 0; dwFrame < m_dwFrameNum; dwFrame++ )
    {
        if ( m_apFrameList[ dwFrame ] != NULL )
        {
            hr = AH.DestroyFrame( m_apFrameList[ dwFrame ] );
            if(FAILED(hr))    
                ErrorLogMsg(KEM_ERROR248, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
            m_apFrameList[ dwFrame ] = NULL;
        }//if
    }//for

    return E_FAIL;

}//CKTDXDeviceXSkinMesh::LoadKSkinnedMesh()


HRESULT CKTDXDeviceXSkinMesh::XSkinMeshProxy::LoadD3DXSkinnedMesh( const void* pInData_, DWORD dwInSize_ )
{
    if(dwInSize_ != 0 && pInData_ == NULL)    
        ErrorLogMsg(KEM_ERROR249, m_DeviceID.c_str());
    ASSERT( dwInSize_ == 0 || pInData_ != NULL );

	HRESULT hr;
	CMultiAnimAllocateHierarchy AH(m_strMoveBoneNameMB);
	//AH.m_wstrMoveBoneName		= m_wstrMoveBoneName;
	//AH.m_strMoveBoneNameMB		= m_strMoveBoneNameMB;
//{{ robobeg : 2008-01-03
	//AH.m_dwFVF				= m_dwFVF;
//}} robobeg : 2008-01-03

	//m_Size = Info->size;

	hr = D3DXLoadMeshHierarchyFromXInMemory( pInData_, dwInSize_,
												D3DXMESH_MANAGED,
                                                GET_D3DDEVICE(),
												&AH,
												NULL,
												(LPD3DXFRAME *) &m_pFrameRoot,
												&m_pAC );

	if( FAILED( hr ) )
	{

		std::wstring strName = L"";
		WCHAR wszErrorMsg[64] = L"";
		if( NULL != pInData_ )
		{
			StringCchPrintfW( wszErrorMsg, ARRAY_SIZE(wszErrorMsg), L"%c%c%c%c_%ld(%ld)", 
				((char*)pInData_)[0],
				((char*)pInData_)[1],
				((char*)pInData_)[2],
				((char*)pInData_)[3],
				(long) hr,
				(long) dwInSize_ );
		}
		else
		{
			StringCchPrintfW( wszErrorMsg, ARRAY_SIZE(wszErrorMsg), L"_%ld(%ld)", 
				(long) hr,
				(long) dwInSize_ );
		}

        strName += L"hresult=";
        strName += wszErrorMsg;

		//if ( hr == D3DERR_INVALIDCALL )
		//{
		//	strName += L"D3DXLoadMeshHierarchyFromXInMemory - D3DERR_INVALIDCALL ";
		//}
		//else if ( hr == E_OUTOFMEMORY )
		//{
		//	strName += L"D3DXLoadMeshHierarchyFromXInMemory - E_OUTOFMEMORY ";
		//}
		
		strName += L" ";
		strName += m_DeviceID;
		ErrorLogMsg( KEM_ERROR343, strName.c_str());


        hr = E_FAIL;


		goto e_Exit;
	}

	m_dwFrameNum = 0;
	m_dwDrawFrameNum = 0;
    m_dwWorkingPaletteSize = 0;
    //m_dwBoneNum = 0;
    m_amxWorkingPalette = NULL;
    // 최대 m_dwWorkingPaletteSize, m_dwBoneNum 크기 찾기는 EstimateWorkingPaletteSizeAndBoneNum 에서 하도록 수정
	hr = SetupFrameList( m_pFrameRoot );
    if ( FAILED( hr ) )
    {
		std::wstring strName = L"";
    	strName += L"LoadD3DXSkinnedMesh - more than 256 frames or multiple meshes per frame is not supported ";
		strName += L" ";
		strName += m_DeviceID;
		ErrorLogMsg( KEM_ERROR344, strName.c_str());

        goto e_Exit;
    }//if
	SetupBonePtrs();


	D3DXFrameCalculateBoundingSphere( m_pFrameRoot, &m_vCenter, &m_fMaxBoundingRadius );
	SetupDrawFrameList( m_pFrameRoot );
    EstimateDrawFrameWorkingPaletteSizeAndBoneNum();
	ComputeCollisionData( false );

//{{ robobeg : 2008-01-03
    // 렌더링 메쉬에 대한 메쉬 최적화를 별도 함수로 옮긴다.
    OptimizeDrawFrameMesh();

#ifndef _KSMTOOL
    // 메쉬 최적화 후 Skinned Mesh를 생성한다.
    hr = ConvertDrawFrameToSkinnedMesh();
    if ( FAILED( hr ) )
    {
        ErrorLogMsg(KEM_ERROR250, m_DeviceID.c_str());
        ASSERT( !"Skin mesh conversion failed" );
        return hr;
    }//if

    ReleaseUnusedInfo();
#endif // #ifndef _KSMTOOL

//}} robobeg : 2008-01-03

//{{ robobeg : 2008-10-08
    //if ( m_pAC != NULL )
    //{
    //    LPD3DXANIMATIONCONTROLLER pNewAC = KTDXAnimationController::CloneOptimizedAC( this, m_pAC );
    //    if ( pNewAC != NULL )
    //    {
    //        SAFE_RELEASE( m_pAC );
    //        m_pAC = pNewAC;
    //    }//if
    //}//if
//}} robobeg : 2008-10-08

e_Exit:

	return hr;
}//CKTDXDeviceXSkinMesh::LoadD3DXSkinnedMesh()


#ifndef _KSMTOOL

//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-12
HRESULT CKTDXDeviceXSkinMesh::XSkinMeshProxy::Load_LuaData(DWORD dwParam_)
{
	WCHAR XETFileName[255] = {0,};

#ifdef	CONVERSION_VS
	wcscat_s( XETFileName, _countof(XETFileName), m_DeviceID.c_str() );
	wcscat_s( XETFileName, _countof(XETFileName), L"ET" );
#else	CONVERSION_VS
	wcscat( XETFileName, m_DeviceID.c_str() );
	wcscat( XETFileName, L"ET" );
#endif	CONVERSION_VS
	
	m_pXET = g_pKTDXApp->GetDeviceManager()->OpenXET( XETFileName );
    return m_pXET != NULL ? S_OK : E_FAIL;
	return S_OK;
}//CKTDXDeviceXSkinMesh::Load_LuaData()


void    CKTDXDeviceXSkinMesh::XSkinMeshProxy::LoadTextures()
{
    for( DWORD i = 0; i < m_dwFrameNum; i++ )
    {
        if(m_apFrameList[ i ] == NULL)    
            ErrorLogMsg(KEM_ERROR251, m_DeviceID.c_str());
        ASSERT( m_apFrameList[ i ] != NULL );
        if ( m_apFrameList[ i ]->pMeshContainer != NULL )
        {
            MultiAnimMC* pMC = static_cast<MultiAnimMC*>( m_apFrameList[ i ]->pMeshContainer );
            pMC->LoadTextures();
        }//if
    }//for
}//CKTDXDeviceXSkinMesh::LoadTextures()


void    CKTDXDeviceXSkinMesh::XSkinMeshProxy::ReleaseUnusedInfo()
{
    for( DWORD i = 0; i < m_dwFrameNum; i++ )
    {
        if(m_apFrameList[ i ] == NULL)    
            ErrorLogMsg(KEM_ERROR252, m_DeviceID.c_str());
        ASSERT( m_apFrameList[ i ] != NULL );
        if ( m_apFrameList[ i ]->pMeshContainer != NULL )
        {
            MultiAnimMC* pMC = static_cast<MultiAnimMC*>( m_apFrameList[ i ]->pMeshContainer );
            pMC->ReleaseUnusedInfo();
        }//if
    }//for
}//CKTDXDeviceXSkinMesh::ReleaseUnusedInfo()

#endif // #ifndef _KSMTOOL



//}} robobeg : 2008-01-02



//#ifdef MONSTER_DIE_PARTICLE_TEST
//	bool CKTDXDeviceXSkinMesh::GetSurfaceVertices( std::vector<D3DXVECTOR3>& vecVertices )
//	{
//		vecVertices.resize(0);
//
//		for( DWORD i = 0; i < m_dwDrawFrameNum; i++ )
//		{
//			if ( m_apDrawFrameList[ i ]->pMeshContainer != NULL )
//			{
//				MultiAnimFrame* pFrame = m_apDrawFrameList[i];
//				MultiAnimMC* pMC = static_cast<MultiAnimMC*>( pFrame->pMeshContainer );
//				if( NULL == pMC )
//					continue;
//
//				DWORD dwSize = pMC->MeshData.pMesh->GetNumBytesPerVertex();
//				DWORD dwFVF = pMC->MeshData.pMesh->GetFVF();
//				DWORD dwNumVertices = pMC->MeshData.pMesh->GetNumVertices();
//
//				void* pVertex = NULL;
//				HRESULT hr = pMC->MeshData.pMesh->LockVertexBuffer( 0, &pVertex );
//				if ( FAILED( hr ) )
//				{
//					ErrorLog(KEM_ERROR266);
//					ASSERT( !"LockVertexBuffer failed" );
//					continue;
//				}
//				BYTE* pBVertex = (BYTE*) pVertex;
//
//
//				D3DXMATRIX matWorld = pFrame->combineMatrix;
//				if( pMC->m_dwNumAttrGroups > 0 &&
//					pMC->m_dwNumPaletteEntries > 0 )
//				{
//					DWORD dwMatrixIndex = pMC->GetBoneId( 0, 0 );
//					if( dwMatrixIndex != UINT_MAX )
//					{
//						D3DXMatrixMultiply( &matWorld, &( pMC->m_amxBoneOffsets[ dwMatrixIndex ] ), &pMC->m_ppBoneFrames[ dwMatrixIndex ]->combineMatrix );
//					}
//				}
//
//				for( DWORD dwVertex = 0; dwVertex < dwNumVertices; dwVertex++, pBVertex += dwSize )
//				{
//					float* pfVertex = (float*) ( pBVertex );
//					D3DXVECTOR3 vVertex( pfVertex[0], pfVertex[1], pfVertex[2] );
//
//					//D3DXVec3TransformCoord( &vVertex, &vVertex, &pFrame->combineMatrix );
//					D3DXVec3TransformCoord( &vVertex, &vVertex, &matWorld );
//
//					vecVertices.push_back( vVertex );
//				}
//
//				hr = pMC->MeshData.pMesh->UnlockVertexBuffer();
//			}
//		}
//		return true;
//	}
//#endif MONSTER_DIE_PARTICLE_TEST




///////////////////////////////////////////////////////////////
// Frame
///////////////////////////////////////////////////////////////

#ifdef _KSMTOOL

//{{ robobeg : 2008-01-03
HRESULT CKTDXDeviceXSkinMesh::MultiAnimFrame::WriteToFileSerializer( KFileSerializer& kInOutFile_, DWORD dwInOffset_ ) const
{
    KSMFrame    ksmFrame;
    ksmFrame.m_dwOffset_Name = NULL;
    ksmFrame.m_dwOffset_MeshContainer = NULL;
    ksmFrame.m_d3dmTransformationMatrix = TransformationMatrix;
    ksmFrame.m_fBoundingRadius = fBoundingRadius;
    ksmFrame.m_iIndex_FrameSibling = ( pFrameSibling == NULL ) ? -1
        : static_cast<const MultiAnimFrame*>( pFrameSibling )->m_iIndex;
    ksmFrame.m_iIndex_FrameFirstChild = ( pFrameFirstChild == NULL ) ? -1
        : static_cast<const MultiAnimFrame*>( pFrameFirstChild )->m_iIndex;
    if ( Name != NULL )
    {
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
		ksmFrame.m_dwOffset_Name = kInOutFile_.AppendString( false, false, Name );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//        ksmFrame.m_dwOffset_Name = kInOutFile_.AppendString( Name );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
    }//if
    if ( pMeshContainer != NULL )
    {
        const MultiAnimMC* pMC = static_cast<const MultiAnimMC*>( pMeshContainer );
        ksmFrame.m_dwOffset_MeshContainer = kInOutFile_.GetSize();
        HRESULT hr = pMC->WriteToFileSerializer( kInOutFile_ );
        if(FAILED(hr))    
            ErrorLogMsg(KEM_ERROR253, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
    }//if
    return  kInOutFile_.CopyData( dwInOffset_, sizeof(KSMFrame), &ksmFrame );
}//

#endif // #ifdef _KSMTOOL


HRESULT CKTDXDeviceXSkinMesh::MultiAnimMC::ReadKSM( CKTDXDeviceXSkinMesh::XSkinMeshProxy& proxy_, const KMemoryDeserializer& kInMem_, DWORD dwInOffset_ )
{
/*    if(pInSkinMesh_ == NULL)    
        ErrorLog(KEM_ERROR254);
    ASSERT( pInSkinMesh_ != NULL )*/;

    // CreateMeshContainer의 작업을 흉내내어 KSM 데이타로부터 MultiAnimMC를 채우는 부분이다.
    // 단, this->pAdjacency, this->pSkinInfo는 게임에서 사용하지 않도록 수정될 것이기 때문에
    // 별도로 생성하지 않는다. ( KSM 포맷 자체에 정보가 저장되지 않음 )

    HRESULT hr;
    ZeroMemory( this, sizeof(MultiAnimMC) );

    const KSMMeshContainer* pksmContainer = static_cast<const KSMMeshContainer*>
        ( kInMem_.AccessData( dwInOffset_, sizeof(KSMMeshContainer) ) );
    if ( pksmContainer == NULL )
    {
        ErrorLog(KEM_ERROR255);
        ASSERT( !"Invalid KSMMeshContainer space" );
        return E_FAIL;
    }//if

    // set this->Name

    if ( pksmContainer->m_dwOffset_Name == NULL )
    {
        this->Name = NULL;
    }
    else
    {
        this->Name = kInMem_.RetrieveDupString( pksmContainer->m_dwOffset_Name );
        if ( this->Name == NULL )
        {                
            ErrorLog(KEM_ERROR257);
            ASSERT( !"Invalid KSMMeshContainer::m_dwOffset_Name" );
            return E_FAIL;
        }//if
    }//if.. else..

    this->NumMaterials = 0;
    // set materials
    if ( pksmContainer->m_dwNumMaterials == 0 || pksmContainer->m_dwOffset_Materials == NULL )
    {
        ErrorLog(KEM_ERROR258);
        ASSERT( !"KSMMeshContainer::m_dwNumMaterials should be greater than 0" );
        return E_FAIL;
    }
    else
    {
        const KSMMaterial* pksmMaterial = static_cast<const KSMMaterial*>
            ( kInMem_.AccessData( pksmContainer->m_dwOffset_Materials, sizeof(KSMMaterial) * pksmContainer->m_dwNumMaterials ) );
        if ( pksmMaterial == NULL )
        {
            ErrorLog(KEM_ERROR259);
            ASSERT( !"invalid KSMMeshContainer:m_dwOffset_Materials" );
            return E_FAIL;
        }//if
        
        this->NumMaterials = pksmContainer->m_dwNumMaterials;
        this->pMaterials = new D3DXMATERIAL[ pksmContainer->m_dwNumMaterials ];
        if(this->pMaterials == NULL)    
            ErrorLog(KEM_ERROR260);
        ASSERT( this->pMaterials != NULL );
        for( DWORD dwMat = 0; dwMat < pksmContainer->m_dwNumMaterials; dwMat++ )
            this->pMaterials[ dwMat ].pTextureFilename = NULL;

#ifndef _KSMTOOL

        this->m_apTextures = new CKTDXDeviceTexture*[ pksmContainer->m_dwNumMaterials ];
        ZeroMemory( this->m_apTextures, sizeof(CKTDXDeviceTexture*) * pksmContainer->m_dwNumMaterials );

#endif // #ifndef _KSMTOOL

        for( DWORD dwMat = 0; dwMat < pksmContainer->m_dwNumMaterials; dwMat++ )
        {
            this->pMaterials[ dwMat ].MatD3D = pksmMaterial[ dwMat ].m_d3dMat;
            this->pMaterials[ dwMat ].pTextureFilename = NULL;
            if ( pksmMaterial[ dwMat ].m_dwOffset_TextureFilename != NULL )
            {
                this->pMaterials[ dwMat ].pTextureFilename = kInMem_.RetrieveDupString(
                    pksmMaterial[ dwMat ].m_dwOffset_TextureFilename );
                if ( this->pMaterials[ dwMat ].pTextureFilename == NULL )
                {                      
                    ErrorLog(KEM_ERROR261);
                    ASSERT( !"invalid KSMMeshContainer::m_dwOffset_TextureFilename" );
                    return E_FAIL;
                }//if
            }//if
        }//for
    }//if.. else..


    // create mesh

    this->MeshData.Type = D3DXMESHTYPE_MESH;
    this->MeshData.pMesh = NULL;
    
    hr = D3DXCreateMeshFVF( pksmContainer->m_dwNumFaces
        , pksmContainer->m_dwNumVertices
        , ( pksmContainer->m_dwOptions & D3DXMESH_32BIT ) | D3DXMESH_MANAGED
        , pksmContainer->m_dwFVF
        , GET_D3DDEVICE()
        , &this->MeshData.pMesh );
    if ( FAILED( hr ) )
    {
        ErrorLog(KEM_ERROR262);
        ASSERT( !"mesh creation failed" );
        this->MeshData.pMesh = NULL;
        return hr;
    }//if

    // fill vertex buffer 
    if ( pksmContainer->m_dwNumVertices == 0 || pksmContainer->m_dwOffset_Vertices == NULL )
    {
        ErrorLog(KEM_ERROR263);
        ASSERT( !"KSMMeshContainer::m_dwNumVertices should be greater than 0" );
        return E_FAIL;
    }
    else
    {
        DWORD dwVertexSize = this->MeshData.pMesh->GetNumBytesPerVertex();
        DWORD dwFVFVertexSize = D3DXGetFVFVertexSize( pksmContainer->m_dwFVF );
        if ( dwVertexSize != dwFVFVertexSize )
        {
            ErrorLog(KEM_ERROR264);
            ASSERT( !"vertex size mismatch" );
            return E_FAIL;
        }//if

        const void* pKSMVertex = kInMem_.AccessData( pksmContainer->m_dwOffset_Vertices, pksmContainer->m_dwNumVertices * dwVertexSize );
        if ( pKSMVertex == NULL )
        {
            ErrorLog(KEM_ERROR265);
            ASSERT( !"invalid KSMMeshContainer::m_dwOffset_Vertices" );
            return E_FAIL;
        }//if
        void* pVertex = NULL;
        hr = this->MeshData.pMesh->LockVertexBuffer( 0, &pVertex );
        if ( FAILED( hr ) )
        {
            ErrorLog(KEM_ERROR266);
            ASSERT( !"LockVertexBuffer failed" );
            return E_FAIL;
        }//if
        CopyMemory( pVertex, pKSMVertex, pksmContainer->m_dwNumVertices * dwVertexSize );
        hr = this->MeshData.pMesh->UnlockVertexBuffer();
        if(FAILED(hr))    
            ErrorLog(KEM_ERROR267);
        ASSERT( SUCCEEDED( hr ) );
    }//if.. else..

    // fill index buffer
    if ( pksmContainer->m_dwNumFaces == 0 || pksmContainer->m_dwOffset_Indices == NULL )
    {
        ErrorLog(KEM_ERROR268);
        ASSERT( !"KSMMeshContainer::m_dwNumFaces should be greater than 0" );
        return E_FAIL;
    }
    else
    {
        DWORD   dwIndexSize = ( pksmContainer->m_dwOptions & D3DXMESH_32BIT ) ? sizeof(DWORD) : sizeof(WORD);

        const void* pKSMIndex = kInMem_.AccessData( pksmContainer->m_dwOffset_Indices, pksmContainer->m_dwNumFaces * 3 * dwIndexSize );
        if ( pKSMIndex == NULL )
        {
            ErrorLog(KEM_ERROR269);
            ASSERT( !"invalid KSMMeshContainer::m_dwOffset_Indices" );
            return E_FAIL;
        }//if
        void* pIndex = NULL;
        hr = this->MeshData.pMesh->LockIndexBuffer( 0, &pIndex );
        if ( FAILED( hr ) )
        {
            ErrorLog(KEM_ERROR270);
            ASSERT( !"LockIndexBuffer failed" );
            return E_FAIL;
        }//if
        CopyMemory( pIndex, pKSMIndex, pksmContainer->m_dwNumFaces * 3 * dwIndexSize );
        hr = this->MeshData.pMesh->UnlockIndexBuffer();
        if(FAILED(hr))    
            ErrorLog(KEM_ERROR271);
        ASSERT( SUCCEEDED( hr ) );
    }//if.. else..

    // fill attribute buffer
    if ( pksmContainer->m_dwOffset_Attributes == NULL )
    {
        ErrorLog(KEM_ERROR272);
        ASSERT( !"Invalid KSMMeshContainer::m_dwOffset_Attributes" );
        return E_FAIL;
    }
    else
    {
        const void* pKSMAttrib = kInMem_.AccessData( pksmContainer->m_dwOffset_Attributes, pksmContainer->m_dwNumFaces * sizeof(DWORD) );
        if ( pKSMAttrib == NULL )
        {
            ErrorLog(KEM_ERROR273);
            ASSERT( !"invalid KSMMeshContainer::m_dwOffset_Attributes" );
            return E_FAIL;
        }//if
        DWORD* pdwAttrib = NULL;
        hr = this->MeshData.pMesh->LockAttributeBuffer( 0, &pdwAttrib );
        if ( FAILED( hr ) )
        {
            ErrorLog(KEM_ERROR274);
            ASSERT( !"LockAttributeBuffer failed" );
            return E_FAIL;
        }//if
        CopyMemory( pdwAttrib, pKSMAttrib, pksmContainer->m_dwNumFaces * sizeof(DWORD) );
        hr = this->MeshData.pMesh->UnlockAttributeBuffer();
        if(FAILED(hr))    
            ErrorLog(KEM_ERROR275);
        ASSERT( SUCCEEDED( hr ) );
    }//if.. else..

    this->pAdjacency = new DWORD[ 3 * pksmContainer->m_dwNumFaces ];
    hr = this->MeshData.pMesh->GenerateAdjacency( 0.f, this->pAdjacency );
    if ( FAILED( hr ) )
    {
        ErrorLog(KEM_ERROR276);
        ASSERT( !"ID3DXMesh::GenerateAdjacency failed" );
        return E_FAIL;
    }//if

    // 여기서부터 skinned mesh ( pksmContainer->m_dwNumBones > 0 ) 와 일반 mesh의 생성이 다르게 처리된다.

    if ( pksmContainer->m_dwNumBones > 0 )
    {
        // 스킨 메쉬의 경우

        if ( pksmContainer->m_dwOffset_Bones == NULL )
        {
            ErrorLog(KEM_ERROR277);
            ASSERT( !"invalid KSMMeshContainer::m_dwOffset_Bones" );
            return E_FAIL;
        }
        const KSMBone* pKSMBone = static_cast< const KSMBone* >(
            kInMem_.AccessData( pksmContainer->m_dwOffset_Bones
            , pksmContainer->m_dwNumBones * sizeof(KSMBone) ) );
        if ( pKSMBone == NULL )
        {
            ErrorLog(KEM_ERROR278);
            ASSERT( !"invalid KSMMeshContainer::m_dwOffset_Bones" );
            return E_FAIL;
        }//if
        
        hr = D3DXCreateSkinInfoFVF( 
            pksmContainer->m_dwNumVertices
            , pksmContainer->m_dwFVF
            , pksmContainer->m_dwNumBones
            , &this->pSkinInfo );
        if ( FAILED( hr ) )
        {
            this->pSkinInfo = NULL;
            
            ErrorLog(KEM_ERROR279);
            ASSERT( !"D3DXCreateSkinInfoFVF failed." );
            return E_FAIL;
        }//if

        m_dwNumBones = pksmContainer->m_dwNumBones;
		m_amxBoneOffsets = new D3DXMATRIX[ m_dwNumBones ];
        if(m_amxBoneOffsets == NULL)    
            ErrorLog(KEM_ERROR280);
        ASSERT( m_amxBoneOffsets != NULL );
        m_ppBoneFrames = new MultiAnimFrame*[ m_dwNumBones ];
        if(m_ppBoneFrames == NULL)    
            ErrorLog(KEM_ERROR281);
        ASSERT( m_ppBoneFrames != NULL );
        for( DWORD dwBone = 0; dwBone < m_dwNumBones; dwBone++ )
        {
            m_amxBoneOffsets[ dwBone ] = pKSMBone[ dwBone ].m_d3dmBoneOffset;
            if ( pKSMBone[ dwBone ].m_iIndex_BoneFrame < 0
                || pKSMBone[ dwBone ].m_iIndex_BoneFrame >= (int) proxy_.GetFrameNum() )
            {
                ErrorLog(KEM_ERROR282);
                ASSERT( !"invalid KSMMeshContainer::m_iIndex_BoneFrame" );
                return E_FAIL;
            }//if
            m_ppBoneFrames[ dwBone ] = proxy_.GetFrame( pKSMBone[ dwBone ].m_iIndex_BoneFrame );
            if(m_ppBoneFrames[ dwBone ] == NULL)    
                ErrorLog(KEM_ERROR283);
            ASSERT( m_ppBoneFrames[ dwBone ] != NULL );
            if ( m_ppBoneFrames[ dwBone ]->Name != NULL )
            {
                hr = this->pSkinInfo->SetBoneName( dwBone, m_ppBoneFrames[ dwBone ]->Name );
                if(FAILED(hr))    
                    ErrorLog(KEM_ERROR284);
                ASSERT( SUCCEEDED( hr ) );
            }//if
            hr = this->pSkinInfo->SetBoneOffsetMatrix( dwBone, &m_amxBoneOffsets[ dwBone ] );
            if(FAILED(hr))    
                ErrorLog(KEM_ERROR285);
            ASSERT( SUCCEEDED( hr ) );
            if ( pKSMBone[ dwBone ].m_dwNumInfluences > 0 )
            {
                if ( pKSMBone[ dwBone ].m_dwOffset_Influences == NULL )
                {
                    ErrorLog(KEM_ERROR286);
                    ASSERT( !"invalid KSMBone::m_dwOffset_Influences" );
                    return E_FAIL;
                }//if
                const void* pInfluence = kInMem_.AccessData( pKSMBone[ dwBone ].m_dwOffset_Influences
                    , ( sizeof( DWORD ) + sizeof( float ) ) * pKSMBone[ dwBone ].m_dwNumInfluences );
                if ( pInfluence == NULL )
                {
                    ErrorLog(KEM_ERROR287);
                    ASSERT( !"invalid KSMBone::m_dwOffset_Influences" );
                    return E_FAIL;
                }//if
                const DWORD* pdwVertices = (const DWORD*) pInfluence;
                const float * pfWeights = (const float*) ( pdwVertices + pKSMBone[ dwBone ].m_dwNumInfluences );
                hr = pSkinInfo->SetBoneInfluence( dwBone, pKSMBone[ dwBone ].m_dwNumInfluences
                    , pdwVertices, pfWeights );
                if(FAILED(hr))    
                    ErrorLog(KEM_ERROR288);
                ASSERT( SUCCEEDED( hr ) );
            }//if
        }//for

        m_dwNumPaletteEntries = pksmContainer->m_dwNumPaletteEntries;


    }
    else
    {
        // 일반 메쉬의 경우
        m_dwNumBones = 0;
		m_amxBoneOffsets = NULL;
        m_ppBoneFrames = NULL;
        m_dwNumPaletteEntries = 0;
    }//if.. else..



    return S_OK;
}//CKTDXDeviceXSkinMesh::MultiAnimMC::ReadKSM()

#ifdef _KSMTOOL

HRESULT CKTDXDeviceXSkinMesh::MultiAnimMC::WriteToFileSerializer( KFileSerializer& kInOutFile_ ) const
{
    if(this->MeshData.Type != D3DXMESHTYPE_MESH)    
        ErrorLogMsg(KEM_ERROR289, m_DeviceID.c_str());
    ASSERT( this->MeshData.Type == D3DXMESHTYPE_MESH );
    if(this->MeshData.pMesh == NULL)    
        ErrorLogMsg(KEM_ERROR290, m_DeviceID.c_str());
    ASSERT( this->MeshData.pMesh != NULL );
    if ( this->MeshData.pMesh == NULL )
        return E_FAIL;
    if(m_dwNumBones != 0 && this->pSkinInfo == NULL)    
        ErrorLogMsg(KEM_ERROR291, m_DeviceID.c_str());
    ASSERT( m_dwNumBones == 0 || this->pSkinInfo != NULL );
    if ( !( m_dwNumBones == 0 || this->pSkinInfo != NULL ) )
        return E_FAIL;

    DWORD   dwMCOffset = kInOutFile_.AppendSpace( sizeof(KSMMeshContainer) );

    HRESULT hr;
    KSMMeshContainer    ksmContainer;

    ksmContainer.m_dwOffset_Name = NULL;
    ksmContainer.m_dwOffset_Materials = NULL;
    ksmContainer.m_dwOffset_Vertices = NULL;
    ksmContainer.m_dwOffset_Indices = NULL;
    ksmContainer.m_dwOffset_Attributes = NULL;
    ksmContainer.m_dwOffset_Bones = NULL;
    ksmContainer.m_dwOffset_NextMeshContainer = NULL;

    if ( this->Name != NULL )
    {
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
		ksmContainer.m_dwOffset_Name = kInOutFile_.AppendString( false, false, this->Name );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//        ksmContainer.m_dwOffset_Name = kInOutFile_.AppendString( this->Name );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
    }//if

    ksmContainer.m_dwNumMaterials = this->NumMaterials;
    if ( ksmContainer.m_dwNumMaterials > 0 )
    {
        if(this->pMaterials == NULL)    
            ErrorLogMsg(KEM_ERROR292, m_DeviceID.c_str());
        ASSERT( this->pMaterials != NULL );
        ksmContainer.m_dwOffset_Materials = kInOutFile_.AppendSpace( sizeof(KSMMaterial) * ksmContainer.m_dwNumMaterials );
        KSMMaterial* pKSMMaterials = new KSMMaterial[ksmContainer.m_dwNumMaterials];
        if(pKSMMaterials == NULL)    
            ErrorLogMsg(KEM_ERROR293, m_DeviceID.c_str());
        ASSERT( pKSMMaterials != NULL );
        for( DWORD dwMat = 0; dwMat < ksmContainer.m_dwNumMaterials; dwMat++ )
        {
            pKSMMaterials[ dwMat ].m_d3dMat = this->pMaterials[ dwMat ].MatD3D;
            pKSMMaterials[ dwMat ].m_dwOffset_TextureFilename = NULL;
            if ( this->pMaterials[ dwMat ].pTextureFilename != NULL )
            {
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
                pKSMMaterials[ dwMat ].m_dwOffset_TextureFilename = kInOutFile_.AppendString( false, false, this->pMaterials[ dwMat ].pTextureFilename );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//                pKSMMaterials[ dwMat ].m_dwOffset_TextureFilename = kInOutFile_.AppendString( this->pMaterials[ dwMat ].pTextureFilename );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
            }//if
        }//for
        hr = kInOutFile_.CopyData( ksmContainer.m_dwOffset_Materials, sizeof(KSMMaterial) * ksmContainer.m_dwNumMaterials, pKSMMaterials );
        if(FAILED(hr))    
            ErrorLogMsg(KEM_ERROR294, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
        SAFE_DELETE_ARRAY( pKSMMaterials );
    }//if


    ksmContainer.m_dwOptions = this->MeshData.pMesh->GetOptions() & D3DXMESH_32BIT;
    if(ksmContainer.m_dwOptions != 0)    
        ErrorLogMsg(KEM_ERROR295, m_DeviceID.c_str());
    ASSERT( ksmContainer.m_dwOptions == 0 );
    ksmContainer.m_dwFVF = this->MeshData.pMesh->GetFVF();
    ksmContainer.m_dwNumVertices = this->MeshData.pMesh->GetNumVertices();
    ksmContainer.m_dwNumFaces = this->MeshData.pMesh->GetNumFaces();

    if ( ksmContainer.m_dwNumVertices > 0 )
    {   
        void*   pVertices = NULL;
        hr = this->MeshData.pMesh->LockVertexBuffer( D3DLOCK_READONLY, &pVertices );
        if(FAILED(hr))    
            ErrorLogMsg(KEM_ERROR296, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
        if(pVertices == NULL)    
            ErrorLogMsg(KEM_ERROR297, m_DeviceID.c_str());
        ASSERT( pVertices != NULL );
        DWORD   dwBytePerVertex = this->MeshData.pMesh->GetNumBytesPerVertex();
        if(dwBytePerVertex != D3DXGetFVFVertexSize( this->MeshData.pMesh->GetFVF() ))    
            ErrorLogMsg(KEM_ERROR298, m_DeviceID.c_str());
        ASSERT( dwBytePerVertex == D3DXGetFVFVertexSize( this->MeshData.pMesh->GetFVF() ) );
        ksmContainer.m_dwOffset_Vertices = kInOutFile_.AppendData( ksmContainer.m_dwNumVertices * dwBytePerVertex, pVertices );
        hr = this->MeshData.pMesh->UnlockVertexBuffer();
        if(FAILED(hr))    
            ErrorLogMsg(KEM_ERROR299, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
    }//if

    if ( ksmContainer.m_dwNumFaces > 0 )
    {
        // index buffer
        {
            if(( this->MeshData.pMesh->GetOptions() & D3DXMESH_32BIT ))    
                ErrorLogMsg(KEM_ERROR300, m_DeviceID.c_str());
            ASSERT( !( this->MeshData.pMesh->GetOptions() & D3DXMESH_32BIT ) );
            void*   pIndices = NULL;
            hr = this->MeshData.pMesh->LockIndexBuffer( D3DLOCK_READONLY, &pIndices);
            if(FAILED(hr))    
                ErrorLogMsg(KEM_ERROR301, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
            if(pIndices == NULL)    
                ErrorLogMsg(KEM_ERROR302, m_DeviceID.c_str());
            ASSERT( pIndices != NULL );
            DWORD   dwIndexSize = ( ksmContainer.m_dwOptions & D3DXMESH_32BIT ) ? sizeof(DWORD) : sizeof(WORD);
            ksmContainer.m_dwOffset_Indices = kInOutFile_.AppendData( ksmContainer.m_dwNumFaces * dwIndexSize * 3, pIndices );
            hr = this->MeshData.pMesh->UnlockIndexBuffer();
            if(FAILED(hr))    
                ErrorLogMsg(KEM_ERROR303, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
        }
        // attribute buffer
        {
            DWORD*  pdwAttributes = NULL;
            hr = this->MeshData.pMesh->LockAttributeBuffer( D3DLOCK_READONLY, &pdwAttributes );
            if(FAILED(hr))    
                ErrorLogMsg(KEM_ERROR304, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
            if(pdwAttributes == NULL)    
                ErrorLogMsg(KEM_ERROR305, m_DeviceID.c_str());
            ASSERT( pdwAttributes != NULL );
            ksmContainer.m_dwOffset_Attributes = kInOutFile_.AppendData( ksmContainer.m_dwNumFaces * sizeof(DWORD), pdwAttributes );
            hr = this->MeshData.pMesh->UnlockAttributeBuffer();
            if(FAILED(hr))    
                ErrorLogMsg(KEM_ERROR306, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
        }
    }//if

    std::vector<DWORD>  vecVertices;
    std::vector<float>  vecWeights;
    ksmContainer.m_dwNumBones = m_dwNumBones;
    if ( ksmContainer.m_dwNumBones > 0 )
    {
        {
            if(m_amxBoneOffsets == NULL)    
                ErrorLogMsg(KEM_ERROR306, m_DeviceID.c_str());
            ASSERT( m_amxBoneOffsets != NULL );
            if(m_ppBoneFrames == NULL)    
                ErrorLogMsg(KEM_ERROR307, m_DeviceID.c_str());
            ASSERT( m_ppBoneFrames != NULL );
            ksmContainer.m_dwOffset_Bones = kInOutFile_.AppendSpace( ksmContainer.m_dwNumBones * sizeof( KSMBone ) );
            KSMBone* pKSMBone = new KSMBone[ ksmContainer.m_dwNumBones ];
            if(pKSMBone == NULL)    
                ErrorLogMsg(KEM_ERROR308, m_DeviceID.c_str());
            ASSERT( pKSMBone != NULL );
            DWORD   dwNumInfluences;
            for( DWORD dwBone = 0; dwBone < ksmContainer.m_dwNumBones; dwBone++ )
            {
                if(m_ppBoneFrames[ dwBone ] == NULL)    
                    ErrorLogMsg(KEM_ERROR309, m_DeviceID.c_str());
                ASSERT( m_ppBoneFrames[ dwBone ] != NULL );
                pKSMBone[ dwBone ].m_d3dmBoneOffset = m_amxBoneOffsets[ dwBone ];
                pKSMBone[ dwBone ].m_iIndex_BoneFrame = ( m_ppBoneFrames[ dwBone ] != NULL ) 
                    ? m_ppBoneFrames[ dwBone ]->m_iIndex : -1;
                dwNumInfluences = pKSMBone[ dwBone ].m_dwNumInfluences = this->pSkinInfo->GetNumBoneInfluences( dwBone );
                pKSMBone[ dwBone ].m_dwOffset_Influences = NULL;
                if ( dwNumInfluences > 0 )
                {
                    vecVertices.resize( dwNumInfluences );
                    vecWeights.resize( dwNumInfluences );
                    hr = this->pSkinInfo->GetBoneInfluence( dwBone, &vecVertices[0], &vecWeights[0] );
                    if(FAILED(hr))
                        ErrorLogMsg(KEM_ERROR310, m_DeviceID.c_str());
                    ASSERT( SUCCEEDED( hr ) );
                    pKSMBone[ dwBone ].m_dwOffset_Influences = kInOutFile_.AppendData( sizeof(DWORD) * dwNumInfluences, &vecVertices[0] );
                    kInOutFile_.AppendData( sizeof(float) * dwNumInfluences, &vecWeights[0] );
                }//if
            }//for
            hr = kInOutFile_.CopyData( ksmContainer.m_dwOffset_Bones , ksmContainer.m_dwNumBones * sizeof( KSMBone ), pKSMBone );
            if(FAILED(hr))
                ErrorLogMsg(KEM_ERROR311, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
            SAFE_DELETE_ARRAY( pKSMBone );
        }
        if(ksmContainer.m_dwOffset_Bones == NULL)
            ErrorLogMsg(KEM_ERROR312, m_DeviceID.c_str());
        ASSERT( ksmContainer.m_dwOffset_Bones != NULL );
        if(m_dwNumPaletteEntries <= 0)
            ErrorLogMsg(KEM_ERROR313, m_DeviceID.c_str());
        ASSERT( m_dwNumPaletteEntries > 0 );
        ksmContainer.m_dwNumPaletteEntries = m_dwNumPaletteEntries;

    }
    else
    {
        ksmContainer.m_dwNumPaletteEntries = 0;
    }//if.. else..

    return kInOutFile_.CopyData( dwMCOffset, sizeof(KSMMeshContainer), &ksmContainer );
}//CKTDXDeviceXSkinMesh::MultiAnimMC::WriteToFileSerializer()

#endif // #ifdef _KSMTOOL


HRESULT CKTDXDeviceXSkinMesh::MultiAnimMC::OptimizeMesh()
{
	// 일반메쉬 데이터일 경우만 최적화를 한다.
	// 추후 패치/프로그래시브 등 다른 메쉬스타일이 추가 될경우
	// 해당 메쉬스타일에 맞는 최적화를 추가해야함.

	HRESULT d3dHr = S_OK;
    if(this->MeshData.pMesh == NULL)
        ErrorLog(KEM_ERROR314);
    ASSERT( this->MeshData.pMesh != NULL );
    if(this->pAdjacency == NULL)
        ErrorLog(KEM_ERROR315);
    ASSERT( this->pAdjacency != NULL );

	LPD3DXMESH pMesh = NULL;

    m_dwNumAttrGroups = 0;

	DWORD dwNumFaces = this->MeshData.pMesh->GetNumFaces();
	DWORD* pNewAdjacency = new DWORD[ 3 * dwNumFaces ];
    if(pNewAdjacency == NULL)
        ErrorLog(KEM_ERROR316);
    ASSERT( pNewAdjacency != NULL );
    LPD3DXBUFFER ppVertexRemap = NULL;

    if(this->MeshData.pMesh == NULL)
        ErrorLog(KEM_ERROR317);
    ASSERT( this->MeshData.pMesh != NULL );
    DWORD   dwFlags = D3DXMESH_MANAGED;

#ifndef _KSMTOOL
    dwFlags |= D3DXMESHOPT_VERTEXCACHE;
#endif// #ifndef _KSMTOOL

#ifdef KSM_ATTRIBUTE_SORT
    if ( this->pSkinInfo == NULL )
        dwFlags |= D3DXMESHOPT_ATTRSORT;
#endif // #ifdef KSM_ATTRIBUTE_SORT


    d3dHr = this->MeshData.pMesh->Optimize( dwFlags,
        this->pAdjacency, pNewAdjacency, NULL, ( this->pSkinInfo != NULL ) ? &ppVertexRemap : NULL, &pMesh );
	if( d3dHr == D3D_OK )
	{
        if ( this->pSkinInfo )
        {
            if(ppVertexRemap == NULL)
                ErrorLog(KEM_ERROR318);
            ASSERT( ppVertexRemap != NULL );

            if(pMesh->GetNumVertices() > ppVertexRemap->GetBufferSize() / sizeof(DWORD))
                ErrorLog(KEM_ERROR319);
            ASSERT( pMesh->GetNumVertices() <= ppVertexRemap->GetBufferSize() / sizeof(DWORD) );
            d3dHr = this->pSkinInfo->Remap( pMesh->GetNumVertices(), (DWORD*) ppVertexRemap->GetBufferPointer() );
            if(d3dHr != D3D_OK)
                ErrorLog(KEM_ERROR320);
            ASSERT( d3dHr == D3D_OK );
            if ( d3dHr != D3D_OK )
            {
                SAFE_RELEASE( pMesh );
                goto optFail;
            }//if
        }
        else
        {
            SAFE_DELETE_ARRAY( m_pAttributeRange );
            SAFE_DELETE_ARRAY( m_pdwBoneCombinationExtra );

#ifdef KSM_ATTRIBUTE_SORT

            DWORD   dwTableSize = 0;
            HRESULT hr = pMesh->GetAttributeTable( NULL, &dwTableSize );
            if ( hr == D3D_OK )
            {
                if(dwTableSize <= 0)
                    ErrorLog(KEM_ERROR321);
                ASSERT( dwTableSize > 0 );
                m_dwNumAttrGroups = dwTableSize;
                m_pAttributeRange = new D3DXATTRIBUTERANGE[ dwTableSize ];
                if(m_pAttributeRange == NULL)
                    ErrorLog(KEM_ERROR322);
                ASSERT( m_pAttributeRange != NULL );
                hr = pMesh->GetAttributeTable( m_pAttributeRange, NULL );
                if(FAILED(hr))
                    ErrorLog(KEM_ERROR323);
                ASSERT( SUCCEEDED( hr ) );
            }//if

#endif // #ifdef KSM_ATTRIBUTE_SORT
        }//if

		SAFE_RELEASE( this->MeshData.pMesh );
		this->MeshData.pMesh = pMesh;
		pMesh = NULL;
        DWORD* pOldAdjacency = this->pAdjacency;
        this->pAdjacency = pNewAdjacency;
        pNewAdjacency = pOldAdjacency;
	}//if
optFail:
    SAFE_DELETE_ARRAY( pNewAdjacency );
    SAFE_RELEASE( ppVertexRemap );
    return  d3dHr;

}//CKTDXDeviceXSkinMesh::MultiAnimMC::OptimizeMesh()



HRESULT CKTDXDeviceXSkinMesh::MultiAnimMC::ConvertToSkinnedMesh()
{
    if(this->MeshData.pMesh == NULL)
        ErrorLog(KEM_ERROR324);
    ASSERT( this->MeshData.pMesh != NULL );

    if ( this->MeshData.pMesh == NULL )
        return E_FAIL;

    HRESULT hr;
        
    if ( this->pSkinInfo == NULL )
    {
	    DWORD dwOldFVF = this->MeshData.pMesh->GetFVF();
        DWORD dwNewFVF = D3DFVF_XYZB4 |  D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;

		LPD3DXMESH pMesh = NULL;
		hr = this->MeshData.pMesh->CloneMeshFVF( ( this->MeshData.pMesh->GetOptions() & D3DXMESH_32BIT ) | D3DXMESH_MANAGED,
												dwNewFVF,
                                                GET_D3DDEVICE(),
												&pMesh );
		if( FAILED( hr ) )
            return hr;

		this->MeshData.pMesh->Release();
		this->MeshData.pMesh = pMesh;
        DWORD   dwSize = this->MeshData.pMesh->GetNumBytesPerVertex();
        DWORD   dwNumVertices = this->MeshData.pMesh->GetNumVertices();

        void* pData;

        hr = this->MeshData.pMesh->LockVertexBuffer( 0, &pData );
        ASSERT( SUCCEEDED( hr ) );
        BYTE* pbVertex = (BYTE*) pData;

        for( DWORD dwVertex = 0; dwVertex < dwNumVertices; dwVertex++, pbVertex += dwSize )
        {
            float* pfVertex = (float*) ( pbVertex );
            pfVertex[ 3 ] = 1.f;
            pfVertex[ 4 ] = 0.f;
            pfVertex[ 5 ] = 0.f;
            pbVertex[ 4 * ( 3 + 3 ) ] = pbVertex[ 4 * ( 3 + 3 ) + 1 ] 
            = pbVertex[ 4 * ( 3 + 3 ) + 2 ] = pbVertex[ 4 * ( 3 + 3 ) + 3 ] = 0;
        }//for

        this->MeshData.pMesh->UnlockVertexBuffer();

    }
    else
    {
        m_dwNumBones = this->pSkinInfo->GetNumBones();
        if(m_dwNumBones <= 0)
            ErrorLog(KEM_ERROR326);
        ASSERT( m_dwNumBones > 0 );

        if(this->MeshData.pMesh->GetFVF() != this->pSkinInfo->GetFVF())
            ErrorLog(KEM_ERROR327);
        ASSERT( this->MeshData.pMesh->GetFVF() == this->pSkinInfo->GetFVF() );
	    DWORD dwNumFaces = this->MeshData.pMesh->GetNumFaces();
	    DWORD* pNewAdjacency = new DWORD[ 3 * dwNumFaces ];
        if(pNewAdjacency == NULL)
            ErrorLog(KEM_ERROR328);
        ASSERT( pNewAdjacency != NULL );
        LPD3DXBUFFER    pBufBoneCombos;

        {
            LPD3DXMESH  pMesh = NULL;
	        // generate the skinned mesh - creates a mesh with blend weights and indices
	        hr = this->pSkinInfo->ConvertToIndexedBlendedMesh( this->MeshData.pMesh,
										        ( this->MeshData.pMesh->GetOptions() & D3DXMESH_32BIT ) | D3DXMESH_MANAGED,
										        m_dwNumPaletteEntries,
										        this->pAdjacency,
                                                pNewAdjacency,
										        NULL,
										        NULL,
										        &m_dwMaxSkinningVertices,
										        &m_dwNumAttrGroups,
										        &pBufBoneCombos,
										        &pMesh );
	        if( FAILED( hr ) )
            {
                SAFE_DELETE_ARRAY( pNewAdjacency );
                return hr;
            }//if

            ASSERT( m_dwMaxSkinningVertices > 0 );
            if ( m_dwMaxSkinningVertices == 0 )
            {
                m_dwNumAttrGroups = 0;
                SAFE_RELEASE( pMesh );
                SAFE_RELEASE( pBufBoneCombos );
                SAFE_DELETE_ARRAY( pNewAdjacency );
                return S_OK;
            }//if

	        this->MeshData.pMesh->Release();
	        this->MeshData.pMesh = pMesh;
        }

        {
            DWORD* pOld = this->pAdjacency;
            this->pAdjacency = pNewAdjacency;
            SAFE_DELETE_ARRAY( pOld );
            pNewAdjacency = NULL;
        }

	    DWORD dwOldFVF = this->MeshData.pMesh->GetFVF();
        DWORD dwPositionFVF = dwOldFVF & D3DFVF_POSITION_MASK;
        switch( m_dwMaxSkinningVertices )
        {
        case 1:
            ASSERT( dwPositionFVF == D3DFVF_XYZB1 );
            break;
        case 2:
            ASSERT( dwPositionFVF == D3DFVF_XYZB2 );
            break;
        case 3:
            ASSERT( dwPositionFVF == D3DFVF_XYZB3 );
            break;
        default: // 4 이상 
            ASSERT( dwPositionFVF == D3DFVF_XYZB4 );
            m_dwMaxSkinningVertices = 4;
        }//switch

	    DWORD dwNewFVF = D3DFVF_XYZB4 | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
        if ( dwOldFVF != dwNewFVF )
	    {
		    LPD3DXMESH pMesh = NULL;
		    hr = this->MeshData.pMesh->CloneMeshFVF( ( this->MeshData.pMesh->GetOptions() & D3DXMESH_32BIT ) | D3DXMESH_MANAGED,
												    dwNewFVF,
                                                    GET_D3DDEVICE(),
												    &pMesh );
		    if( FAILED( hr ) )
            {
                return hr;
            }//if

		    this->MeshData.pMesh->Release();
		    this->MeshData.pMesh = pMesh;

            if ( m_dwMaxSkinningVertices < 4 )
            {
                DWORD   dwSize = this->MeshData.pMesh->GetNumBytesPerVertex();
                DWORD   dwNumVertices = this->MeshData.pMesh->GetNumVertices();

                void* pData;

                hr = this->MeshData.pMesh->LockVertexBuffer( 0, &pData );
                ASSERT( SUCCEEDED( hr ) );
                BYTE* pbVertex = (BYTE*) pData;

                for( DWORD dwVertex = 0; dwVertex < dwNumVertices; dwVertex++, pbVertex += dwSize )
                {
                    float* pfVertex = (float*) ( pbVertex );
                    switch( m_dwMaxSkinningVertices )
                    {
                    case 1:
                        pfVertex[ 3 ] = 1.f;
                        pfVertex[ 4 ] = 0.f;
                        pfVertex[ 5 ] = 0.f;
                        pbVertex[ 4 * ( 3 + 3 ) + 1 ] = pbVertex[ 4 * ( 3 + 3 ) + 2 ] = pbVertex[ 4 * ( 3 + 3 ) + 3 ] = 0;
                        break;
                    case 2:
                        pfVertex[ 4 ] = 1.f - pfVertex[ 3 ];
                        pfVertex[ 5 ] = 0.f;
                        pbVertex[ 4 * ( 3 + 3 ) + 2 ] = pbVertex[ 4 * ( 3 + 3 ) + 3 ] = 0;
                        break;
                    default: // case 3:
                        pfVertex[ 5 ] = 1.f - pfVertex[ 3 ] - pfVertex[ 4 ];
                        pbVertex[ 4 * ( 3 + 3 ) + 3 ] = 0;
                    }//switch
                }//for

                this->MeshData.pMesh->UnlockVertexBuffer();

            }//if
	    }//if


        SAFE_DELETE_ARRAY( m_pAttributeRange );
        SAFE_DELETE_ARRAY( m_pdwBoneCombinationExtra );

        if(pBufBoneCombos == NULL || m_dwNumAttrGroups <= 0)
            ErrorLog(KEM_ERROR329);
        ASSERT( pBufBoneCombos != NULL && m_dwNumAttrGroups > 0 );
        const D3DXBONECOMBINATION* pBoneCombosPointer = static_cast<const D3DXBONECOMBINATION*>( pBufBoneCombos->GetBufferPointer() );
        if(pBoneCombosPointer == NULL)
            ErrorLog(KEM_ERROR330);
        ASSERT( pBoneCombosPointer != NULL );
        m_pAttributeRange = new D3DXATTRIBUTERANGE[ m_dwNumAttrGroups ];
        if(m_pAttributeRange == NULL)
            ErrorLog(KEM_ERROR331);
        ASSERT( m_pAttributeRange != NULL );
        m_pdwBoneCombinationExtra = new DWORD[ m_dwNumAttrGroups * m_dwNumPaletteEntries ];
        if(m_pdwBoneCombinationExtra == NULL)
            ErrorLog(KEM_ERROR332);
        ASSERT( m_pdwBoneCombinationExtra != NULL );
        for( DWORD dwAttr = 0; dwAttr < m_dwNumAttrGroups; dwAttr++ )
        {
            CopyMemory( &m_pAttributeRange[ dwAttr ], &pBoneCombosPointer[ dwAttr ], sizeof(D3DXATTRIBUTERANGE) );
            if(pBoneCombosPointer[ dwAttr ].BoneId == NULL)
                ErrorLog(KEM_ERROR333);
            ASSERT( pBoneCombosPointer[ dwAttr ].BoneId != NULL );
            CopyMemory( &m_pdwBoneCombinationExtra[ dwAttr * m_dwNumPaletteEntries ], pBoneCombosPointer[ dwAttr ].BoneId
                , m_dwNumPaletteEntries * sizeof(DWORD) );
        }//for
        SAFE_RELEASE( pBufBoneCombos );

    }//if.. else..

	D3DVERTEXELEMENT9 pDecl[ MAX_FVF_DECL_SIZE ];
	D3DVERTEXELEMENT9 * pDeclCur;
	hr = this->MeshData.pMesh->GetDeclaration( pDecl );
	if( FAILED( hr ) )
        return hr;

	pDeclCur = pDecl;
	while( pDeclCur->Stream != 0xff )
	{
		if( ( pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES ) && ( pDeclCur->UsageIndex == 0 ) )
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	hr = this->MeshData.pMesh->UpdateSemantics( pDecl );
	if( FAILED( hr ) )
        return hr;

//#ifdef PROGRESSIVE_X_SKIN_MESH
//
//	//거시기 되는가? 
//	// Verify validity of mesh for simplification
//	if( !FAILED( hr = D3DXValidMesh( this->MeshData.pMesh, (DWORD*)this->pAdjacency, NULL ) ) )
//	{
//		LPD3DXPMESH			pPMesh = NULL;
//
//		hr = D3DXGeneratePMesh( this->MeshData.pMesh, (DWORD*)this->pAdjacency,
//			NULL, NULL, 1, D3DXMESHSIMP_VERTEX, &pPMesh );
//
//		if( FAILED(hr) )
//		{
//			/*
//			if ( hr == D3DXERR_CANNOTATTRSORT )
//			{
//				ErrorLogMsg( KEM_ERROR356, L"D3DXERR_CANNOTATTRSORT" );
//			}
//			else if ( hr == D3DERR_INVALIDCALL )
//			{
//				ErrorLogMsg( KEM_ERROR356, L"D3DERR_INVALIDCALL" );
//			}
//			else if ( hr == E_OUTOFMEMORY )
//			{
//				ErrorLogMsg( KEM_ERROR356, L"E_OUTOFMEMORY" );
//			}
//			else
//			{
//				ErrorLogMsg( KEM_ERROR356, GetDeviceID().c_str() );
//			}	
//			*/
//
//			//I expect that this code be must success.
//			//goto End2;
//		}
//
//
//
//		DWORD cVerticesMin = pPMesh->GetMinVertices();
//		DWORD cVerticesMax = pPMesh->GetMaxVertices();
//
//		DWORD cVerticesPerMesh = ( cVerticesMax - cVerticesMin + 10 ) / 10;
//
//		m_cPMeshes = max( 1, (DWORD)ceil( (cVerticesMax - cVerticesMin + 1) / (float)cVerticesPerMesh ) );
//		m_ppPMeshes = new LPD3DXPMESH[m_cPMeshes];
//		if( m_ppPMeshes == NULL )
//		{
//			hr = E_OUTOFMEMORY;
//
//			//goto End2;
//		}
//		ZeroMemory( m_ppPMeshes, sizeof(LPD3DXPMESH) * m_cPMeshes );
//
//		// Clone full size pmesh
//		hr = pPMesh->ClonePMeshFVF( CUSTOM_D3D_MESH_MANAGED | D3DXMESH_VB_SHARE, pPMesh->GetFVF(), GET_D3DDEVICE(), &m_pPMeshFull );
//		if( FAILED(hr) )
//		{
//			//goto End2;
//		}
//
//		// Clone all the separate pmeshes
//		for( UINT iPMesh = 0; iPMesh < m_cPMeshes; iPMesh++ )
//		{
//			hr = pPMesh->ClonePMeshFVF( CUSTOM_D3D_MESH_MANAGED | D3DXMESH_VB_SHARE, pPMesh->GetFVF(), GET_D3DDEVICE(), &m_ppPMeshes[iPMesh] );
//			if( FAILED(hr) )
//			{
//				//goto End2;
//			}
//
//			// Trim to appropriate space
//			hr = m_ppPMeshes[iPMesh]->TrimByVertices( cVerticesMin + cVerticesPerMesh * iPMesh, cVerticesMin + cVerticesPerMesh * (iPMesh+1), NULL, NULL);
//			if( FAILED(hr) )
//			{
//				//goto End2;
//			}
//
//			hr = m_ppPMeshes[iPMesh]->OptimizeBaseLOD( 
//				D3DXMESHOPT_VERTEXCACHE
//
//				, 
//				NULL );
//
//			if( FAILED(hr) )
//			{
//				//goto End2;
//			}
//		}
//
//		// Set current to be maximum number of vertices
//		m_iPMeshCur = m_cPMeshes - 1;
//		hr = m_ppPMeshes[m_iPMeshCur]->SetNumVertices( cVerticesMax );
//		if( FAILED(hr) )
//		{
//			//goto End2;
//		}
//
//		hr = m_pPMeshFull->SetNumVertices( cVerticesMax );
//
//		SAFE_RELEASE( pPMesh );
//		
//	}
//	
//#endif //PROGRESSIVE_X_SKIN_MESH

    return S_OK;
}//CKTDXDeviceXSkinMesh::MultiAnimMC::ConvertToSkinnedMesh()


#ifndef _KSMTOOL

void    CKTDXDeviceXSkinMesh::MultiAnimMC::LoadTextures()
{
    WCHAR wszTexName[ MAX_PATH ];
    for( DWORD dwMat = 0; dwMat < this->NumMaterials; dwMat++ )
    {
		if( this->pMaterials[ dwMat ].pTextureFilename != NULL
            && this->pMaterials[ dwMat ].pTextureFilename[0] != NULL )
		{
			if( MultiByteToWideChar( CP_ACP, 0, this->pMaterials[ dwMat ].pTextureFilename, -1, wszTexName, MAX_PATH ) != 0 )
			{
				if( wcslen( wszTexName) > 0 )
				{
					this->m_apTextures[ dwMat ] = g_pKTDXApp->GetDeviceManager()->OpenTexture( wszTexName );
				}
			}
		}//if
		else
        {
            this->m_apTextures[ dwMat ] = NULL;
        }//if.. else..
    }//for
}//CKTDXDeviceXSkinMesh::MultiAnimMC::LoadTextures()


void    CKTDXDeviceXSkinMesh::MultiAnimMC::ReleaseUnusedInfo()
{
	SAFE_DELETE_ARRAY( this->Name );   // mem leak?
    SAFE_RELEASE( this->pSkinInfo );
}//CKTDXDeviceXSkinMesh::MultiAnimMC::ReleaseUnusedInfo()

#endif // #ifndef _KSMTOOL



//CKTDXDeviceXSkinMesh::CMultiAnimAllocateHierarchy::CMultiAnimAllocateHierarchy()
//{
//}

HRESULT CKTDXDeviceXSkinMesh::CMultiAnimAllocateHierarchy::CreateFrame( THIS_ LPCSTR Name, LPD3DXFRAME * ppFrame )
{
	*ppFrame = NULL;
	MultiAnimFrame* pFrame = new MultiAnimFrame;
    if(pFrame == NULL)
        ErrorLog(KEM_ERROR334);
    ASSERT( pFrame != NULL );
	//ZeroMemory( pFrame, sizeof( MultiAnimFrame ) );
	D3DXMatrixIdentity( &pFrame->combineMatrix );

    if( Name != NULL && Name[0] != NULL )
	{
		pFrame->Name = (CHAR *) StrHeapCopy( (CHAR *) Name );
	}
	else
		pFrame->Name = (CHAR *) StrHeapCopy( "<no_name>" );

	int iNameLen = strlen( pFrame->Name );
	
	if( ( iNameLen < 9 || memcmp( pFrame->Name, "COLLISION", 9 ) != 0 )
		&& ( iNameLen < 6 || memcmp( pFrame->Name, "ATTACK", 6 ) != 0 )
		&& ( iNameLen < 15 || memcmp( pFrame->Name, "Bip01_Footsteps", 15 ) != 0 ) )
	{
		pFrame->m_bDraw = true;

		if( pFrame->Name != NULL && strcmp( m_strMoveBoneNameMB.c_str(), pFrame->Name ) == 0 )
		{
			pFrame->m_bMoveBone = true;
		}
		else
		{
			pFrame->m_bMoveBone = false;
		}
	}
	else
	{
		pFrame->m_bDraw		= false;
		pFrame->m_bMoveBone	= false;
	}

	*ppFrame = pFrame;

	return S_OK;
}

HRESULT CKTDXDeviceXSkinMesh::CMultiAnimAllocateHierarchy::CreateMeshContainer( THIS_
																LPCSTR Name,
																CONST D3DXMESHDATA *pMeshData,
																CONST D3DXMATERIAL *pMaterials,
																CONST D3DXEFFECTINSTANCE *pEffectInstances,
																DWORD NumMaterials,
																CONST DWORD *pAdjacency,
																LPD3DXSKININFO pSkinInfo, 
																LPD3DXMESHCONTAINER *ppNewMeshContainer )
{
	HRESULT hr = S_OK;

	*ppNewMeshContainer = NULL;    

	MultiAnimMC * pMC = new MultiAnimMC;
    ASSERT( pMC != NULL );
	ZeroMemory( pMC, sizeof( MultiAnimMC ) );

	//스킨 메쉬 데이터만 취급한다
	/*
	if( pSkinInfo == NULL )
	{
		hr = S_OK;
		goto e_Exit;
	}
	*/



	// only support mesh type
	if( pMeshData->Type != D3DXMESHTYPE_MESH )
	{ 
		hr = E_FAIL; 
		goto e_Exit; 
	}

	if( Name )
		pMC->Name = (CHAR *) StrHeapCopy( (CHAR *) Name );
	else
		pMC->Name = (CHAR *) StrHeapCopy( "<no_name>" );

	// copy the mesh over
	pMC->MeshData.Type	= pMeshData->Type;
	pMC->MeshData.pMesh = pMeshData->pMesh;

	pMC->MeshData.pMesh->AddRef();

	// copy adjacency over
	{
		DWORD dwNumFaces = pMC->MeshData.pMesh->GetNumFaces();
		pMC->pAdjacency = new DWORD[ 3 * dwNumFaces ];
        ASSERT( pMC->pAdjacency != NULL );
		CopyMemory( pMC->pAdjacency, pAdjacency, 3 * sizeof( DWORD ) * dwNumFaces );
	}

	pMC->pEffects = NULL;

	// alloc and copy materials
	pMC->NumMaterials	= max( 1, NumMaterials );
	pMC->pMaterials		= new D3DXMATERIAL			[ pMC->NumMaterials ];
    if(pMC->pMaterials == NULL)
        ErrorLog(KEM_ERROR335);
    ASSERT( pMC->pMaterials != NULL );

#ifndef _KSMTOOL

    pMC->m_apTextures = new CKTDXDeviceTexture*[ pMC->NumMaterials ];
    ASSERT( pMC->m_apTextures != NULL );
    ZeroMemory( pMC->m_apTextures, sizeof(CKTDXDeviceTexture*) * pMC->NumMaterials );

#endif // #ifndef _KSMTOOL

	if( NumMaterials > 0 )
	{
		//CopyMemory( pMC->pMaterials, pMaterials, NumMaterials * sizeof( D3DXMATERIAL ) );

		for( DWORD i = 0; i < NumMaterials; ++ i )
		{
            pMC->pMaterials[ i ].MatD3D = pMaterials[ i ].MatD3D;
            if ( pMaterials[ i ].pTextureFilename != NULL )
                pMC->pMaterials[ i ].pTextureFilename = StrHeapCopy( pMaterials[ i ].pTextureFilename );
            else
                pMC->pMaterials[ i ].pTextureFilename = NULL;
		}//for
	}
	else    // mock up a default material and set it
	{
		ZeroMemory( & pMC->pMaterials[ 0 ].MatD3D, sizeof( D3DMATERIAL9 ) );
		pMC->pMaterials[ 0 ].MatD3D.Diffuse.r = 0.5f;
		pMC->pMaterials[ 0 ].MatD3D.Diffuse.g = 0.5f;
		pMC->pMaterials[ 0 ].MatD3D.Diffuse.b = 0.5f;
		pMC->pMaterials[ 0 ].MatD3D.Specular = pMC->pMaterials[ 0 ].MatD3D.Diffuse;
		pMC->pMaterials[ 0 ].pTextureFilename = NULL;
	}

//{{ robobeg : 2008-01-03
    pMC->m_dwNumBones = 0;
    // 스킨 메쉬 생성은 Frame Hierarchy 생성이 끝난 뒤에 함 
    if ( pSkinInfo != NULL )
    {
        pMC->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();
        DWORD   dwNumBones = pSkinInfo->GetNumBones();
        if(dwNumBones <= 0)
            ErrorLog(KEM_ERROR336);
        ASSERT( dwNumBones > 0 );
        if ( dwNumBones == 0 )
        {
            hr = E_FAIL;
            goto e_Exit;
        }//if

		//본 오프셋 매트릭스 
		pMC->m_amxBoneOffsets = new D3DXMATRIX[ dwNumBones ];
		{
			for( DWORD i = 0; i < dwNumBones ; ++ i )
				pMC->m_amxBoneOffsets[ i ] = * (D3DXMATRIX *) pSkinInfo->GetBoneOffsetMatrix( i );
		}

		if( dwNumBones > MATRIX_PALETTE_SIZE_DEFAULT )
			pMC->m_dwNumPaletteEntries = MATRIX_PALETTE_SIZE_DEFAULT;
		else
			pMC->m_dwNumPaletteEntries = dwNumBones;
    }//if
//}} robobeg : 2008-01-03

e_Exit:

	if( FAILED( hr ) )
	{
		ErrorLogMsg( KEM_ERROR101, Name );

		if( pMC != NULL )
			DestroyMeshContainer( pMC );
	}
	else
		* ppNewMeshContainer = pMC;



	return hr;
}

HRESULT CKTDXDeviceXSkinMesh::CMultiAnimAllocateHierarchy::DestroyFrame( THIS_ LPD3DXFRAME pFrameToFree )
{
	MultiAnimFrame * pFrame = (MultiAnimFrame *) pFrameToFree;

	//SAFE_DELETE_ARRAY( pFrame->Name );
	SAFE_DELETE( pFrame );

	return S_OK;
}

HRESULT CKTDXDeviceXSkinMesh::CMultiAnimAllocateHierarchy::DestroyMeshContainer( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree )
{
	MultiAnimMC * pMC = (MultiAnimMC *) pMeshContainerToFree;
	if ( pMC == NULL )
		return E_FAIL;

	SAFE_DELETE_ARRAY( pMC->Name );
	SAFE_RELEASE( pMC->MeshData.pMesh );
	
	//DWORD iPMesh;
	//SAFE_RELEASE( pMC->m_pPMeshFull );
	//for (iPMesh = 0; iPMesh < pMC->m_cPMeshes; iPMesh++)
	//{
	//	SAFE_RELEASE( pMC->m_ppPMeshes[iPMesh] );
	//}
	//pMC->m_cPMeshes = 0;
	//SAFE_DELETE_ARRAY( pMC->m_ppPMeshes );

	SAFE_DELETE_ARRAY( pMC->pAdjacency );
	
	for( DWORD i = 0; i < pMC->NumMaterials; ++ i )
	{
		SAFE_DELETE_ARRAY( pMC->pMaterials[i].pTextureFilename );

#ifndef _KSMTOOL

		if( g_pKTDXApp != NULL && g_pKTDXApp->GetAppDestroy() != true )
		{
			if( pMC->m_apTextures[ i ] != NULL )
				SAFE_CLOSE( pMC->m_apTextures[ i ] );
		}//if

#endif // #ifndef _KSMTOOL
	}

	SAFE_DELETE_ARRAY( pMC->pMaterials );
    pMC->NumMaterials = 0;

#ifndef _KSMTOOL

	SAFE_DELETE_ARRAY( pMC->m_apTextures );

#endif // #ifndef _KSMTOOL

	SAFE_RELEASE( pMC->pSkinInfo );
	SAFE_DELETE_ARRAY( pMC->m_amxBoneOffsets );

	pMC->m_dwNumPaletteEntries = 0;
	pMC->m_dwMaxSkinningVertices = 0;
	pMC->m_dwNumAttrGroups = 0;

    SAFE_DELETE_ARRAY( pMC->m_pAttributeRange );
    SAFE_DELETE_ARRAY( pMC->m_pdwBoneCombinationExtra );
    SAFE_DELETE_ARRAY( pMC->m_ppBoneFrames );
	SAFE_DELETE( pMeshContainerToFree );

	return S_OK;
}

HRESULT CKTDXDeviceXSkinMesh::MultiAnimMC::SetupBonePtrs( D3DXFRAME * pFrameRoot )
{
	if( pSkinInfo )
	{
        SAFE_DELETE_ARRAY( m_ppBoneFrames );

		DWORD dwNumBones = pSkinInfo->GetNumBones();

        m_ppBoneFrames = new MultiAnimFrame* [ dwNumBones ];
        ZeroMemory( m_ppBoneFrames, sizeof(MultiAnimFrame*) * dwNumBones );

		for( DWORD i = 0; i < dwNumBones; ++ i )
		{			
			MultiAnimFrame *pFrame = (MultiAnimFrame *) D3DXFrameFind( pFrameRoot, pSkinInfo->GetBoneName( i ) );
			if( pFrame == NULL )
			{
				return E_FAIL;
			}

            m_ppBoneFrames[ i ] = pFrame;
		}
	}

	return S_OK;
}


//{{ seojt // 2008-10-15, 14:54
// mesh가 사용하는 vertex buffer와 texture를 local memory로 preload하는
// 함수의 구현 - jintaeks on 2008-10-15, 15:00
void CKTDXDeviceXSkinMesh::MultiAnimMC::PreLoad()
{
    HRESULT                 hr;

    if ( this->MeshData.pMesh != NULL )
    {
        LPDIRECT3DVERTEXBUFFER9 pVertices = NULL;
        hr = this->MeshData.pMesh->GetVertexBuffer( &pVertices );
        if ( SUCCEEDED(hr) && pVertices )
        {
            pVertices->PreLoad();
            SAFE_RELEASE( pVertices );
        }//if
    }//if

    // 아래의 두 변수에 대해서는 정확한 사용법을 확인하지 않아서, preload처리하지 않다.
    // TODO: 
	//m_ppPMeshes;
	//m_pPMeshFull;

#ifndef _KSMTOOL
    if ( m_apTextures )
    {
        for ( DWORD dw = 0; dw < __super::NumMaterials; ++dw )
        {
            if ( m_apTextures[dw] )
                m_apTextures[dw]->PreLoad();
        }//for
    }//if
#endif // _KSMTOOL
}//CKTDXDeviceXSkinMesh::MultiAnimMC::PreLoad()
//}} seojt // 2008-10-15, 14:54


HRESULT CKTDXDeviceXSkinMesh::Reset_Animation( LPD3DXANIMATIONCONTROLLER pAnimSet)
{
	if( m_pAC != NULL)
	{
		SAFE_RELEASE( m_pAC);
		m_pAC = pAnimSet;

		return S_OK;
	}

	std::string strName;
	ConvertWCHARToChar( strName, m_DeviceID.c_str() );
	ErrorLogMsg( KEM_ERROR102, strName.c_str());

	return E_FAIL;
}
