#include <StdAfx.h>

#include ".\ktdxdevicexmesh.h"

#define KME_ATTRIBUTE_SORT

#ifdef _KSMTOOL
#define GET_D3DDEVICE()     DXUTGetD3DDevice()
#else
#define GET_D3DDEVICE()     g_pKTDXApp->GetDevice()
#endif // #ifdef _KSMTOOL

#pragma warning (disable: 4200)

#pragma pack( push, 1 )

struct  KMEHeader
{
    DWORD       m_dwMagic;
    DWORD       m_dwVersion;
    DWORD       m_dwFVF;
    DWORD       m_dwOptions;
    DWORD       m_dwNumMaterials;
    DWORD       m_dwOffset_Materials;
    DWORD       m_dwNumVertices;
    DWORD       m_dwOffset_Vertices;
    DWORD       m_dwNumFaces;
    DWORD       m_dwOffset_Indices;
    DWORD       m_dwOffset_Attributes;
    D3DXVECTOR3 m_vCenter;
    float       m_fRadius;
};//struct  KMEHeader

struct  KMEMaterial
{
    D3DMATERIAL9    m_d3dMat;
    DWORD           m_dwOffset_TextureFilename;
};//struct  KMEMaterial

#pragma pack( pop )


CKTDXDeviceXMesh::CKTDXDeviceXMesh( LPDIRECT3DDEVICE9 pd3dDevice, std::wstring fileName )
					:CKTDXDevice( fileName, DT_XMESH )
{
	m_vCenter.x			= 0.0f;
	m_vCenter.y			= 0.0f;
	m_vCenter.z			= 0.0f;

	m_fRadius			= 0.0f;

	m_dwNumMaterials	= 0;
	m_pdxMaterials	    = NULL;

#ifndef _KSMTOOL

	m_ppTextures		= NULL;

#endif // #ifndef _KSMTOOL

    m_dwNumAttrGroups = 0;
    m_pAttributeRange = NULL;

	m_pMesh				= NULL;

#ifndef _KSMTOOL

	m_pXET				= NULL;

#endif // #ifndef _KSMTOOL

}

CKTDXDeviceXMesh::~CKTDXDeviceXMesh(void)
{
	_UnLoad();
}

/*virtual*/ HRESULT CKTDXDeviceXMesh::_Load( bool bSkipStateCheck
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD			
	)
{
    // qff
	////CKTDXThread::CLocker locker( g_pKTDXApp->GetDeviceManager()->GetDeviceLock() );

    HRESULT hr = S_OK;

    DWORD   dwSize = 0;
    const void* pData = NULL;
	char* pTempBuf = NULL;

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
        ErrorLogMsg(KEM_ERROR144, m_DeviceID.c_str());    
    ASSERT( pData != NULL );

#else

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( m_DeviceID );
	if( Info == NULL )
	{
		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR98, strName.c_str());

		return E_FAIL;
	}


	dwSize = Info->size;
	pData = Info->pRealData;


#endif // #ifdef _KSMTOOL

    XMeshProxy  proxy( GetDeviceID() );

    if ( dwSize >= sizeof(DWORD) && *static_cast<const DWORD*>( pData ) == KOGMESH_FORMAT_MAGIC )
        hr = proxy.LoadKMEMesh( pData, dwSize );
    else
        hr = proxy.LoadD3DXMesh( pData, dwSize );

#ifdef _KSMTOOL

    kFile.Unlock();

#endif // #ifdef _KSMTOOL

    if ( FAILED( hr ) )
        return hr;
        
#ifndef _KSMTOOL

    proxy.LoadTextures();
//#ifdef  BACKGROUND_LOADING_TEST // 2008-12-12
	proxy.LoadXET();
//#endif  BACKGROUND_LOADING_TEST // 2008-12-12
    
#endif // #ifndef _KSMTOOL

    {
        CSLock  locker( m_csDeviceState );

        if (  bSkipStateCheck == false )
        {
		    switch( m_eDeviceState )
		    {
		    case DEVICE_STATE_INIT:
		    //case DEVICE_STATE_PENDING:
		    default:
			    ASSERT( 0 );
		    case DEVICE_STATE_LOADING:
		    case DEVICE_STATE_FAILED:
                std::swap( m_pMesh, proxy.m_pMesh );
                std::swap( m_dwNumMaterials, proxy.m_dwNumMaterials );
                std::swap( m_pdxMaterials, proxy.m_pdxMaterials );
#ifndef _KSMTOOL
                std::swap( m_ppTextures, proxy.m_ppTextures );
                std::swap( m_pXET, proxy.m_pXET );
#endif  _KSMTOOL
                m_dwNumAttrGroups = proxy.m_dwNumAttrGroups;  proxy.m_dwNumAttrGroups = 0L;
                std::swap( m_pAttributeRange, proxy.m_pAttributeRange );
                m_vCenter = proxy.m_vCenter;
                m_fRadius = proxy.m_fRadius;
#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
				std::swap( m_vecSubsetCullingInfo, proxy.m_vecSubsetCullingInfo );
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
                m_Size = dwSize;
		        m_eDeviceState = DEVICE_STATE_LOADED;
			    break;
		    case DEVICE_STATE_LOADED:
			    break;
		    }//switch
	    }
        else
        {
            std::swap( m_pMesh, proxy.m_pMesh );
            std::swap( m_dwNumMaterials, proxy.m_dwNumMaterials );
            std::swap( m_pdxMaterials, proxy.m_pdxMaterials );
#ifndef _KSMTOOL
            std::swap( m_ppTextures, proxy.m_ppTextures );
            std::swap( m_pXET, proxy.m_pXET );
#endif  _KSMTOOL
            m_dwNumAttrGroups = proxy.m_dwNumAttrGroups;  proxy.m_dwNumAttrGroups = 0L;
            std::swap( m_pAttributeRange, proxy.m_pAttributeRange );
            m_vCenter = proxy.m_vCenter;
            m_fRadius = proxy.m_fRadius;
#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
			std::swap( m_vecSubsetCullingInfo, proxy.m_vecSubsetCullingInfo );
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
            m_Size = dwSize;
        }//if.. else..
    }

	return S_OK;
}//CKTDXDeviceXMesh::Load()


HRESULT CKTDXDeviceXMesh::_UnLoad()
{
	////CKTDXThread::CLocker locker( g_pKTDXApp->GetDeviceManager()->GetDeviceLock() );

#ifndef _KSMTOOL

	if( g_pKTDXApp != NULL && g_pKTDXApp->GetAppDestroy() != true )
	{
		for( UINT i = 0; i < m_dwNumMaterials; i++ )
		{
			SAFE_CLOSE( m_ppTextures[ i ] );
		}
		SAFE_CLOSE( m_pXET );
	}
	SAFE_DELETE_ARRAY( m_ppTextures );

#endif // #ifndef _KSMTOOL


//{{ robobeg : 2008-01-06
    
	for( DWORD i = 0; i < m_dwNumMaterials; i++ )
        SAFE_DELETE_ARRAY( m_pdxMaterials[ i ].pTextureFilename );
	SAFE_DELETE_ARRAY( m_pdxMaterials );
	m_dwNumMaterials = 0L;

    SAFE_DELETE_ARRAY( m_pAttributeRange );
    m_dwNumAttrGroups = 0;

	SAFE_RELEASE( m_pMesh );
	return S_OK;
}//CKTDXDeviceXMesh::UnLoad()


//{{ seojt // 2008-10-15, 15:24
void CKTDXDeviceXMesh::PreLoad()
{
#ifndef _KSMTOOL
    if ( m_ppTextures )
    {
	    for ( UINT i = 0; i < m_dwNumMaterials; ++i )
	    {
		    if ( m_ppTextures[i] )
                m_ppTextures[i]->PreLoad();
	    }
    }//if
#endif // _KSMTOOL
}//CKTDXDeviceXMesh::PreLoad()
//}} seojt // 2008-10-15, 15:24


#ifndef _KSMTOOL
#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
HRESULT	CKTDXDeviceXMesh::Render( CKTDXDeviceXET* pTexChangeXET, CKTDXDeviceXET* pMultiTexXET, 
	const CKTDXDeviceXET::AniData* pAniData, float fAniTime, const std::vector<bool>* pvecDrawSubset )
#else//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
HRESULT	CKTDXDeviceXMesh::Render( CKTDXDeviceXET* pTexChangeXET, CKTDXDeviceXET* pMultiTexXET, 
	const CKTDXDeviceXET::AniData* pAniData, float fAniTime )
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
{
	if ( IsLoaded() == false )
		return S_OK;

	D3DMATERIAL9 temp;
	HRESULT hr;
	hr = GET_D3DDEVICE()->GetMaterial( &temp );
	// Set and draw each of the materials in the mesh

    DWORD   dwNumSubsets = ( m_dwNumAttrGroups == 0 ) ? m_dwNumMaterials : m_dwNumAttrGroups;
    DWORD   dwAttribId = 0;

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	//pvecDrawSubset 사이즈랑 같이 않다면 pvecDrawSubset 정보는 무시하도록 한다.
	if( pvecDrawSubset && pvecDrawSubset->size() != dwNumSubsets )
		pvecDrawSubset = NULL;
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

	for( DWORD i = 0; i < dwNumSubsets; i++ )
	{
        if ( m_dwNumAttrGroups == 0 )
        {
            dwAttribId = i;
        }
        else
        {
		    if(m_pAttributeRange == NULL)            
                ErrorLogMsg(KEM_ERROR145, m_DeviceID.c_str());
            ASSERT( m_pAttributeRange != NULL );
            dwAttribId = m_pAttributeRange[ i ].AttribId;

            if(dwAttribId >= m_dwNumMaterials)
                ErrorLogMsg(KEM_ERROR146, m_DeviceID.c_str());
            ASSERT( dwAttribId < m_dwNumMaterials );
        }//if.. else..

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
		//그려야 할 subset인지 확인...
		if( pvecDrawSubset && (*pvecDrawSubset)[i] == false )
			continue;		
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

        hr = GET_D3DDEVICE()->SetMaterial( &m_pdxMaterials[ dwAttribId ].MatD3D );
		CKTDXDeviceBaseTexture* pResultTexture0 = NULL;
		CKTDXDeviceBaseTexture* pResultTexture1 = NULL;
		CKTDXDeviceBaseTexture* pResultTexture2 = NULL;

		{
			pResultTexture0 = SetNowTexture( (CKTDXDeviceBaseTexture*) m_ppTextures[ dwAttribId ], 0, pTexChangeXET, pMultiTexXET, pAniData, fAniTime );
			if( pResultTexture0 != NULL )
			{
				pResultTexture0->SetDeviceTexture( 0 );
			}

			pResultTexture1 = SetNowTexture( (CKTDXDeviceBaseTexture*)m_ppTextures[ dwAttribId ], 1, pTexChangeXET, pMultiTexXET, pAniData, fAniTime );
			if( pResultTexture1 != NULL )
			{
				CKTDGStateManager::PushTextureStageState( 1, D3DTSS_COLOROP
					, pMultiTexXET->GetMultiTex( 1 )->colorOP );
				pResultTexture1->SetDeviceTexture( 1 );
			}
			pResultTexture2 = SetNowTexture( (CKTDXDeviceBaseTexture*) m_ppTextures[ dwAttribId ], 2, pTexChangeXET, pMultiTexXET, pAniData, fAniTime );
			if( pResultTexture2 != NULL )
			{
				CKTDGStateManager::PushTextureStageState( 2, D3DTSS_COLOROP
					, pMultiTexXET->GetMultiTex( 2 )->colorOP );
				pResultTexture2->SetDeviceTexture( 2 );
			}
		}	

		if ( m_pMesh )
        {
			hr = m_pMesh->DrawSubset( dwAttribId );
        }

		{
			if( pResultTexture2 != NULL )
			{
				CKTDGStateManager::PopTextureStageState( 2, D3DTSS_COLOROP );
			}
			if( pResultTexture1 != NULL )
			{
				CKTDGStateManager::PopTextureStageState( 1, D3DTSS_COLOROP );
			}		
		}		
	}
	hr = GET_D3DDEVICE()->SetMaterial( &temp );

	return S_OK;
}

LPD3DXBASEMESH CKTDXDeviceXMesh::GetMesh()
{
	if ( IsLoaded() == false )
		return NULL;

	return m_pMesh;
}


CKTDXDeviceBaseTexture* CKTDXDeviceXMesh::SetNowTexture( CKTDXDeviceBaseTexture* orgTex, int stage,
													CKTDXDeviceXET* pTexChangeXET, CKTDXDeviceXET* pMultiTexXET, 
													const CKTDXDeviceXET::AniData* pAniData, float fAniTime )
{
	if( orgTex == NULL )
		return NULL;

	//우선순위
	//1. XET 에니메이션 체인지 텍스쳐 데이타
	//2. XET 멀티 텍스쳐 데이타
	//3. XET 체인지 텍스쳐 데이타

	const WCHAR* pOrgTexName = orgTex->GetDeviceID().c_str();

	CKTDXDeviceBaseTexture* pResultTexture = NULL;

	if( stage == 0 )
	{
		if( pAniData != NULL )
		{
			pResultTexture = pAniData->GetAniTexture( fAniTime, pOrgTexName );
			if( NULL == pResultTexture )
			{
				pResultTexture = orgTex;
			}
		}
		else if( pTexChangeXET != NULL )
		{
			pResultTexture = pTexChangeXET->GetChangeTexture( pOrgTexName );
		}
		else
		{
			pResultTexture = orgTex;
		}
	}
	else if( pMultiTexXET != NULL )
	{
		const CKTDXDeviceXET::MultiTexureData* pMultiTexureData = pMultiTexXET->GetMultiTex( stage );
		if( pMultiTexureData != NULL )
		{
			pResultTexture = pMultiTexureData->pTexture;
		}
	}

	return pResultTexture;
}


#endif // #ifndef _KSMTOOL





HRESULT         CKTDXDeviceXMesh::WriteToFileSerializer( KFileSerializer& kInOutFile_ ) const
{
	if ( IsLoaded() == false )
		return E_FAIL;

    HRESULT hr = S_OK;

    if(m_pMesh == NULL)    
        ErrorLogMsg(KEM_ERROR147, m_DeviceID.c_str());
    ASSERT( m_pMesh != NULL );

    DWORD   dwOffset_Start = kInOutFile_.AppendSpace( sizeof(KMEHeader) );

    KMEHeader   kmeHeader;
    kmeHeader.m_dwMagic = KOGMESH_FORMAT_MAGIC;
    kmeHeader.m_dwVersion = KOGMESH_FORMAT_VERSION;
    kmeHeader.m_dwFVF = m_pMesh->GetFVF();
    kmeHeader.m_dwOptions = m_pMesh->GetOptions() & D3DXMESH_32BIT;
    kmeHeader.m_dwNumMaterials = m_dwNumMaterials;
    kmeHeader.m_dwOffset_Materials = NULL;
    kmeHeader.m_dwNumVertices = m_pMesh->GetNumVertices();
    kmeHeader.m_dwOffset_Vertices = NULL;
    kmeHeader.m_dwNumFaces = m_pMesh->GetNumFaces();
    kmeHeader.m_dwOffset_Indices = NULL;
    kmeHeader.m_dwOffset_Attributes = NULL;
    kmeHeader.m_vCenter = m_vCenter;
    kmeHeader.m_fRadius = m_fRadius;

    if ( m_dwNumMaterials > 0 )
    {
        if(m_pdxMaterials == NULL)
            ErrorLogMsg(KEM_ERROR148, m_DeviceID.c_str());
        ASSERT( m_pdxMaterials != NULL );
        kmeHeader.m_dwOffset_Materials = kInOutFile_.AppendSpace( sizeof(KMEMaterial) * m_dwNumMaterials );
        KMEMaterial* pKMEMaterials = new KMEMaterial[m_dwNumMaterials];
        if(pKMEMaterials == NULL)
            ErrorLogMsg(KEM_ERROR149, m_DeviceID.c_str());
        ASSERT( pKMEMaterials != NULL );
        for( DWORD dwMat = 0; dwMat < m_dwNumMaterials; dwMat++ )
        {
            pKMEMaterials[ dwMat ].m_d3dMat = m_pdxMaterials[ dwMat ].MatD3D;
            pKMEMaterials[ dwMat ].m_dwOffset_TextureFilename = NULL;
            if ( m_pdxMaterials[ dwMat ].pTextureFilename != NULL )
            {
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
				pKMEMaterials[ dwMat ].m_dwOffset_TextureFilename = kInOutFile_.AppendString( false, false, m_pdxMaterials[ dwMat ].pTextureFilename );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//                pKMEMaterials[ dwMat ].m_dwOffset_TextureFilename = kInOutFile_.AppendString( m_pdxMaterials[ dwMat ].pTextureFilename );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
            }//if
        }//for
        hr = kInOutFile_.CopyData( kmeHeader.m_dwOffset_Materials, sizeof(KMEMaterial) * m_dwNumMaterials, pKMEMaterials );
        if(FAILED(hr))
            ErrorLogMsg(KEM_ERROR150, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
        SAFE_DELETE_ARRAY( pKMEMaterials );
    }//if

    if ( kmeHeader.m_dwNumVertices > 0 )
    {   
        void*   pVertices = NULL;
        hr = m_pMesh->LockVertexBuffer( D3DLOCK_READONLY, &pVertices );
        if(FAILED(hr))
            ErrorLogMsg(KEM_ERROR151, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
        if(pVertices == NULL)
            ErrorLogMsg(KEM_ERROR152, m_DeviceID.c_str());
        ASSERT( pVertices != NULL );
        DWORD   dwBytePerVertex = m_pMesh->GetNumBytesPerVertex();
        if(dwBytePerVertex != D3DXGetFVFVertexSize(kmeHeader.m_dwFVF))
            ErrorLogMsg(KEM_ERROR153, m_DeviceID.c_str());
        ASSERT( dwBytePerVertex == D3DXGetFVFVertexSize( kmeHeader.m_dwFVF ) );
        kmeHeader.m_dwOffset_Vertices = kInOutFile_.AppendData( kmeHeader.m_dwNumVertices * dwBytePerVertex, pVertices );
        hr = m_pMesh->UnlockVertexBuffer();
        if(FAILED(hr))
            ErrorLogMsg(KEM_ERROR154, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
    }//if

    if ( kmeHeader.m_dwNumFaces > 0 )
    {
        // index buffer
        {
            void*   pIndices = NULL;
            hr = m_pMesh->LockIndexBuffer( D3DLOCK_READONLY, &pIndices);
            if(FAILED(hr))
                ErrorLogMsg(KEM_ERROR155, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
            if(pIndices == NULL)
                ErrorLogMsg(KEM_ERROR156, m_DeviceID.c_str());
            ASSERT( pIndices != NULL );
            DWORD   dwIndexSize = ( kmeHeader.m_dwOptions & D3DXMESH_32BIT ) ? sizeof(DWORD) : sizeof(WORD);
            kmeHeader.m_dwOffset_Indices = kInOutFile_.AppendData( kmeHeader.m_dwNumFaces * dwIndexSize * 3, pIndices );
            hr = m_pMesh->UnlockIndexBuffer();
            if(FAILED(hr))
                ErrorLogMsg(KEM_ERROR157, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
        }
        // attribute buffer
        {
            DWORD*  pdwAttributes = NULL;
            hr = m_pMesh->LockAttributeBuffer( D3DLOCK_READONLY, &pdwAttributes );
            if(FAILED(hr))
                ErrorLogMsg(KEM_ERROR158, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
            if(pdwAttributes == NULL)
                ErrorLogMsg(KEM_ERROR159, m_DeviceID.c_str());
            ASSERT( pdwAttributes != NULL );
            kmeHeader.m_dwOffset_Attributes = kInOutFile_.AppendData( kmeHeader.m_dwNumFaces * sizeof(DWORD), pdwAttributes );
            hr = m_pMesh->UnlockAttributeBuffer();
            if(FAILED(hr))
                ErrorLogMsg(KEM_ERROR160, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
        }
    }//if

    return  kInOutFile_.CopyData( dwOffset_Start, sizeof(KMEHeader), &kmeHeader );
}//CKTDXDeviceXMesh::WriteToFileSerializer()


void CKTDXDeviceXMesh::SetLOD( int lodPercent )
{
//     ASSERT( m_bLoadProgressiveMesh );
// 
// 	if ( lodPercent > 100 )
// 		lodPercent = 100;
// 
// 	if ( lodPercent < 0 )
// 		lodPercent = 0;
// 
// 	m_iPMeshCur = (int)((m_cPMeshes - 1)  * ( lodPercent / 100.0f ));
//     if ( m_iPMeshCur <= 0 )
//         m_iPMeshCur = 0;
}//CKTDXDeviceXMesh::SetLOD()


CKTDXDeviceXMesh::XMeshProxy::XMeshProxy(  const std::wstring& strDeviceID_  )
: m_DeviceID( strDeviceID_ )
, m_pMesh( NULL )
, m_dwNumMaterials( 0L )
, m_pdxMaterials( NULL )
#ifndef _KSMTOOL
, m_ppTextures( NULL )
, m_pXET( NULL )
#endif  _KSMTOOL
, m_dwNumAttrGroups( 0L )
, m_pAttributeRange( NULL )
, m_vCenter( 0, 0, 0 )
, m_fRadius( 0.f )
{
}//CKTDXDeviceXMesh::XMeshProxy::XMeshProxy()
CKTDXDeviceXMesh::XMeshProxy::~XMeshProxy()
{
#ifndef _KSMTOOL

	if( g_pKTDXApp != NULL && g_pKTDXApp->GetAppDestroy() != true )
	{
		for( UINT i = 0; i < m_dwNumMaterials; i++ )
		{
			SAFE_CLOSE( m_ppTextures[ i ] );
		}
		SAFE_CLOSE( m_pXET );
	}
	SAFE_DELETE_ARRAY( m_ppTextures );

#endif // #ifndef _KSMTOOL


    for( DWORD i = 0; i < m_dwNumMaterials; i++ )
        SAFE_DELETE_ARRAY( m_pdxMaterials[ i ].pTextureFilename );
	SAFE_DELETE_ARRAY( m_pdxMaterials );
	m_dwNumMaterials = 0L;

    SAFE_DELETE_ARRAY( m_pAttributeRange );
    m_dwNumAttrGroups = 0;

	SAFE_RELEASE( m_pMesh );

}//CKTDXDeviceXMesh::XMeshProxy::~XMeshProxy()


HRESULT         CKTDXDeviceXMesh::XMeshProxy::LoadD3DXMesh( const void* pInData_, DWORD dwInSize_ )
{
    ASSERT( dwInSize_ == 0 || pInData_ != NULL );

    if(dwInSize_ != 0 && pInData_ == NULL)
    {
        ErrorLogMsg(KEM_ERROR161, m_DeviceID.c_str());
        return E_FAIL;
    }//if

    ASSERT( m_pdxMaterials == NULL );
    ASSERT( m_pAttributeRange == NULL );
#ifndef _KSMTOOL
    ASSERT( m_ppTextures == NULL );
#endif // #ifndef _KSMTOOL
    ASSERT( m_pMesh == NULL );


	HRESULT				hr = E_FAIL;
	HRESULT             d3dHr = E_FAIL;
	//unsigned int		i;

	LPD3DXBUFFER		pAdjacencyBuffer	= NULL;
	LPD3DXBUFFER		pD3DXMtrlBuffer		= NULL;

//{{ robobeg : 2008-01-06
	//LPD3DXPMESH			pPMesh = NULL;
	//DWORD				iPMesh;
//}} robobeg : 2008-01-06

	DWORD				dw32BitFlag = 0;


	//LPD3DXPMESH			pPMesh = NULL;

#ifdef _KSMTOOL

	//메쉬를 로드하여 pMesh버퍼에 저장한다
    hr = D3DXLoadMeshFromXInMemory( pInData_, dwInSize_,
							D3DXMESH_MANAGED, 
                            DXUTGetD3DDevice(),
							&pAdjacencyBuffer, &pD3DXMtrlBuffer, NULL,
							&m_dwNumMaterials, &m_pMesh );
	if( FAILED( hr ) )

#else

	//메쉬를 로드하여 pMesh버퍼에 저장한다
    hr = D3DXLoadMeshFromXInMemory( pInData_, dwInSize_,
							D3DXMESH_MANAGED, 
                            GET_D3DDEVICE(),
							&pAdjacencyBuffer, &pD3DXMtrlBuffer, NULL,
							&m_dwNumMaterials, &m_pMesh );
	if( FAILED( hr ) )

#endif // #ifdef _KSMTOOL
	{
		// hide error so that device changes will not cause exit, shows blank screen instead
		goto End;
	}

	//로드된 매터리얼 정보가 없으면 실패
	if( m_dwNumMaterials == 0 )
    {
        hr = E_FAIL;
		goto End;
    }
	
	//이 메쉬의 생성시에 유효하게 되어 있는 메쉬 옵션을 얻어온다(메쉬 플래그)
	dw32BitFlag = (m_pMesh->GetOptions() & D3DXMESH_32BIT);

	//매터리얼과 텍스쳐 정보 로드.
	// Allocate a material/texture arrays

    {

	    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	    m_pdxMaterials      = new D3DXMATERIAL[m_dwNumMaterials];
        //if(m_pdxMaterials == NULL)
        //    ErrorLogMsg(KEM_ERROR162, m_DeviceID.c_str());
        ASSERT( m_pdxMaterials != NULL );

#ifndef _KSMTOOL

	    m_ppTextures		= new CKTDXDeviceTexture*[m_dwNumMaterials];
        //if(m_ppTextures == NULL)
        //    ErrorLogMsg(KEM_ERROR163, m_DeviceID.c_str());
        ASSERT( m_ppTextures != NULL );
	    ZeroMemory( m_ppTextures, sizeof(CKTDXDeviceTexture*) * m_dwNumMaterials );

#endif // #ifndef _KSMTOOL

	    // Copy the materials and load the textures
	    for( DWORD i=0; i<m_dwNumMaterials; i++ )
	    {
		    m_pdxMaterials[i].MatD3D = d3dxMaterials[i].MatD3D;
		    m_pdxMaterials[i].MatD3D.Ambient = m_pdxMaterials[i].MatD3D.Diffuse;
            m_pdxMaterials[i].pTextureFilename = NULL;
		    if( d3dxMaterials[i].pTextureFilename != NULL )
                m_pdxMaterials[ i ].pTextureFilename = StrHeapCopy( d3dxMaterials[i].pTextureFilename );
	    }//for
    }

    SAFE_RELEASE( pD3DXMtrlBuffer );


    if ( m_pMesh->GetFVF() != D3DFVF_MESHVERTEX )
    {
	    LPD3DXMESH			pTempMesh = NULL;

        //복제된 메쉬를 만들어낸다
#ifdef _KSMTOOL

		hr = m_pMesh->CloneMeshFVF( dw32BitFlag|D3DXMESH_MANAGED, D3DFVF_MESHVERTEX, 
			DXUTGetD3DDevice(), &pTempMesh );

#else

		hr = m_pMesh->CloneMeshFVF( dw32BitFlag|D3DXMESH_MANAGED, D3DFVF_MESHVERTEX, 
			GET_D3DDEVICE(), &pTempMesh );

#endif // #ifndef _KSMTOOL

	    if (FAILED(hr))
		    goto End;

	    //원래 매쉬를 파기하고 정규화된 메쉬를 이용한다.
        SAFE_RELEASE( m_pMesh );
        m_pMesh = pTempMesh;
    }

	//메쉬의 각 정점의 법선 정보를 계산한다.(정규화 시킴)
	//D3DXComputeNormals( pTempMesh, NULL );

    {
	    MESH_VERTEX*		pMeshVertices = NULL;
	    //DWORD*				pAttribs = NULL;
	    DWORD				Face;

        DWORD   dwNumVertices = m_pMesh->GetNumVertices();
        DWORD   dwVertexSize = m_pMesh->GetNumBytesPerVertex();
	    //Lock the vertex buffer, but allow writing.
	    d3dHr = m_pMesh->LockVertexBuffer( 0, (void**)&pMeshVertices );
        if(d3dHr != D3D_OK)
            ErrorLogMsg(KEM_ERROR164, m_DeviceID.c_str());
        ASSERT( d3dHr == D3D_OK );

        DWORD   dwNumFaces = m_pMesh->GetNumFaces();
        void*   pIndices = NULL;
	    //We only need to read the indices
	    d3dHr = m_pMesh->LockIndexBuffer( D3DLOCK_READONLY, &pIndices );
        if(d3dHr != D3D_OK)
            ErrorLogMsg(KEM_ERROR165, m_DeviceID.c_str());
        ASSERT( d3dHr == D3D_OK );

	    //The attribute buffer maps the materials to each face.
	    //d3dHr = m_pMesh->LockAttributeBuffer(D3DLOCK_READONLY, &pAttribs);
    //   if(d3dHr != D3D_OK)
    //       ErrorLogMsg(KEM_ERROR166, m_DeviceID.c_str());
    //   ASSERT( d3dHr == D3D_OK );


        if ( dw32BitFlag )
        {
            DWORD*  pdwIndices = (DWORD*) pIndices;
	        //Loop through each face and set the vertex color based on the material.
	        //This is a pretty simple example, but you could also use this to preload
	        //other data, such as converting colors to data that the vertex shader
	        //may use in computations.
	        for( Face = 0; Face < dwNumFaces; Face++ )
	        {
		        //memcpy(&Diffuse,(D3DXCOLOR*)&m_pMaterials[pAttribs[Face]].Diffuse, sizeof(D3DXCOLOR));
                //D3DXCOLOR Diffuse = m_pdxMaterials[pAttribs[Face]].MatD3D.Diffuse;

		        pMeshVertices[pdwIndices[Face * 3 + 0]].color = 0xffffffff;//Diffuse;
		        pMeshVertices[pdwIndices[Face * 3 + 1]].color = 0xffffffff;//Diffuse;
		        pMeshVertices[pdwIndices[Face * 3 + 2]].color = 0xffffffff;//Diffuse;
	        }//for
        }
        else
        {
            WORD*  pwIndices = (WORD*) pIndices;
	        //Loop through each face and set the vertex color based on the material.
	        //This is a pretty simple example, but you could also use this to preload
	        //other data, such as converting colors to data that the vertex shader
	        //may use in computations.
	        for( Face = 0; Face < dwNumFaces; Face++ )
	        {
		        //memcpy(&Diffuse,(D3DXCOLOR*)&m_pMaterials[pAttribs[Face]].Diffuse, sizeof(D3DXCOLOR));
                //D3DXCOLOR Diffuse = m_pdxMaterials[pAttribs[Face]].MatD3D.Diffuse;

		        pMeshVertices[pwIndices[Face * 3 + 0]].color = 0xffffffff;//Diffuse;
		        pMeshVertices[pwIndices[Face * 3 + 1]].color = 0xffffffff;//Diffuse;
		        pMeshVertices[pwIndices[Face * 3 + 2]].color = 0xffffffff;//Diffuse;
	        }//for
        }//if.. else..

        D3DXComputeBoundingSphere( (D3DXVECTOR3*)pMeshVertices, dwNumVertices,
		    dwVertexSize,
		    &m_vCenter, &m_fRadius );

	    //Give back all of our buffers.
	    //The attribute buffer maps the materials to each face.

	    //d3dHr = m_pMesh->UnlockAttributeBuffer();
    //   if(d3dHr != D3D_OK)
    //       ErrorLogMsg(KEM_ERROR167, m_DeviceID.c_str());
    //   ASSERT( d3dHr == D3D_OK );

	    d3dHr = m_pMesh->UnlockIndexBuffer();
        if(d3dHr != D3D_OK)
            ErrorLogMsg(KEM_ERROR168, m_DeviceID.c_str());
        ASSERT( d3dHr == D3D_OK );
	    d3dHr = m_pMesh->UnlockVertexBuffer();
        if(d3dHr != D3D_OK)
            ErrorLogMsg(KEM_ERROR169, m_DeviceID.c_str());
        ASSERT( d3dHr == D3D_OK );

    }

	// note:
	// Optimized 함수를 이용하여 메쉬파일 최적화를 한다.
	// 옵션 설정은 고사양을위한 것이 아닌 최저사양 포함옵션을 생각하여
	// Mesh Type : VCache optimized
	// Primitive : Triangle list
	// 와 같이 고정설정한다.

//{{ robobeg : 2008-01-06
	//d3dHr = m_pMesh->Optimize( D3DXMESHOPT_VERTEXCACHE | CUSTOM_D3D_MESH_MANAGED,
	//				 (DWORD*)pAdjacencyBuffer->GetBufferPointer(),
	//				 NULL, NULL, NULL, &m_pMesh );



	

#ifndef _KSMTOOL
	D3DXWELDEPSILONS Epsilons;
	ZeroMemory( &Epsilons, sizeof(D3DXWELDEPSILONS) );
	if( FAILED( hr = D3DXWeldVertices( m_pMesh, 0, &Epsilons,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL ) ) )
	{
		ErrorLog( KEM_ERROR357 );
		goto End;
	}
#endif
	

	//DWORD* pdwAdjacency2 = new DWORD[ m_pMesh->GetNumFaces() * 3 ];

    //ASSERT( m_pMesh == NULL );

    {
	    LPD3DXMESH			pTempMesh = NULL;        

		d3dHr = m_pMesh->Optimize( D3DXMESH_MANAGED
#ifndef _KSMTOOL
                        | D3DXMESHOPT_VERTEXCACHE
#endif // #ifndef _KSMTOOL
#ifdef KME_ATTRIBUTE_SORT
                        | D3DXMESHOPT_ATTRSORT
#endif // #ifdef KME_ATTRIBUTE_SORT
                        ,
					    (DWORD*)pAdjacencyBuffer->GetBufferPointer(),
					    NULL, NULL, NULL, &pTempMesh );
    //}} robobeg : 2008-01-06

	    if( d3dHr == D3D_OK )
	    {
		    SAFE_RELEASE( m_pMesh );
            m_pMesh = pTempMesh;
	    }//if
    }

//{{ robobeg : 2008-01-06
// AttributeTable 읽어오기 기능 추가
    m_dwNumAttrGroups = 0;
    m_pAttributeRange = NULL;

#ifdef KME_ATTRIBUTE_SORT

    d3dHr = m_pMesh->GetAttributeTable( NULL, &m_dwNumAttrGroups );
    if ( d3dHr == D3D_OK )
    {
        if(m_dwNumAttrGroups <= 0)    
            ErrorLogMsg(KEM_ERROR197, m_DeviceID.c_str());
        ASSERT( m_dwNumAttrGroups > 0 );
        if ( m_dwNumAttrGroups > 0 )
        {
            m_pAttributeRange = new D3DXATTRIBUTERANGE[ m_dwNumAttrGroups ];
            if(m_pAttributeRange == NULL)    
                ErrorLogMsg(KEM_ERROR198, m_DeviceID.c_str());
            ASSERT( m_pAttributeRange != NULL );
            d3dHr = m_pMesh->GetAttributeTable( m_pAttributeRange, NULL );
            if(FAILED(d3dHr))    
                ErrorLogMsg(KEM_ERROR199, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( d3dHr  ) );
        }//if
    }
    else
    {
        m_dwNumAttrGroups = 0;
    }//if.. else..

#endif // #ifdef KME_ATTRIBUTE_SORT

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	//subset 단위로 bounding sphere 저장
	void* pVertex = NULL;
	hr = m_pMesh->LockVertexBuffer( D3DLOCK_READONLY, &pVertex );
	if ( SUCCEEDED( hr ) )
	{
		D3DXVECTOR3 vSubsetCenter;
		float fSubsetRadius;
        m_vecSubsetCullingInfo.resize( m_dwNumAttrGroups );
		for( int i = 0; i < (int) m_dwNumAttrGroups; i++ )
		{
			hr = D3DXComputeBoundingSphere( (D3DXVECTOR3*)((MESH_VERTEX*)pVertex + m_pAttributeRange[i].VertexStart), m_pAttributeRange[i].VertexCount,
				m_pMesh->GetNumBytesPerVertex(),
				&vSubsetCenter, &fSubsetRadius );
			if( SUCCEEDED( hr ) )
            {
                m_vecSubsetCullingInfo[ i ].m_vCenter = vSubsetCenter;
                m_vecSubsetCullingInfo[ i ].m_fRadius = fSubsetRadius;
            }
            else
            {
                m_vecSubsetCullingInfo[ i ].m_vCenter = D3DXVECTOR3(0,0,0);
                m_vecSubsetCullingInfo[ i ].m_fRadius = 0.f;
            }
		}
		m_pMesh->UnlockVertexBuffer();
	}
    else
    {
        m_vecSubsetCullingInfo.resize(0);
    }
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET


    hr = S_OK;


End:

	SAFE_RELEASE( pAdjacencyBuffer );
	SAFE_RELEASE( pD3DXMtrlBuffer );

    if ( FAILED( hr ) )
    {
        for( DWORD i = 0; i < m_dwNumMaterials; i++ )
            SAFE_DELETE_ARRAY( m_pdxMaterials[ i ].pTextureFilename );
	    SAFE_DELETE_ARRAY( m_pdxMaterials );
	    m_dwNumMaterials = 0L;

        SAFE_DELETE_ARRAY( m_pAttributeRange );
        m_dwNumAttrGroups = 0;

	    SAFE_RELEASE( m_pMesh );
    }//if
    
    return hr;

}//CKTDXDeviceXMesh::XMeshProxy::LoadD3DXMesh()


HRESULT         CKTDXDeviceXMesh::XMeshProxy::LoadKMEMesh( const void* pInData_, DWORD dwInSize_ )
{
    ASSERT( dwInSize_ == 0 || pInData_ != NULL );
    if(dwInSize_ != 0 && pInData_ == NULL)
    {
        ErrorLogMsg(KEM_ERROR170, m_DeviceID.c_str());
        return E_FAIL;
    }//if

    HRESULT     hr = E_FAIL;
    KMemoryDeserializer kMemory( dwInSize_, pInData_ );
    KMEHeader   kmeHeader;

	//LPD3DXPMESH  pPMesh = NULL;

//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
	hr = kMemory.RetrieveData( &kmeHeader, 0, sizeof(KMEHeader) );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//    hr = kMemory.RetrieveData( 0, sizeof(KMEHeader), &kmeHeader );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
    if(FAILED(hr))
        ErrorLogMsg(KEM_ERROR171, m_DeviceID.c_str());
    ASSERT( SUCCEEDED( hr ) );
    if ( FAILED( hr ) )
        return E_FAIL;


    if ( kmeHeader.m_dwMagic != KOGMESH_FORMAT_MAGIC )
    {
        ErrorLogMsg(KEM_ERROR172, m_DeviceID.c_str());     
        ASSERT( !"Invalid Format" );
        return E_FAIL;
    }//if
    if ( kmeHeader.m_dwVersion != KOGMESH_FORMAT_VERSION )
    {
        ErrorLogMsg(KEM_ERROR173, m_DeviceID.c_str());        
        ASSERT( !"Invalid Version" );
        return E_FAIL;
    }//if
    if ( kmeHeader.m_dwFVF != D3DFVF_MESHVERTEX )
    {
        ErrorLogMsg(KEM_ERROR174, m_DeviceID.c_str());     
        ASSERT( !"Invalid mesh format" );
        return E_FAIL;
    }//if

    if ( kmeHeader.m_dwNumMaterials == 0 || kmeHeader.m_dwOffset_Materials == NULL )
    {
        ErrorLogMsg(KEM_ERROR179, m_DeviceID.c_str());
        ASSERT( !"KMEHeader::m_dwNumMaterials should be greater than 0" );
        return E_FAIL;
    }

    if ( kmeHeader.m_dwNumVertices == 0 || kmeHeader.m_dwOffset_Vertices == NULL )
    {
        ErrorLogMsg(KEM_ERROR184, m_DeviceID.c_str());
        ASSERT( !"KMEHeader::m_dwNumVertices should be greater than 0" );
        return E_FAIL;
    }

    if ( kmeHeader.m_dwNumFaces == 0 || kmeHeader.m_dwOffset_Indices == NULL )
    {
        ErrorLogMsg(KEM_ERROR189, m_DeviceID.c_str());
        ASSERT( !"KMEHeader::m_dwNumFaces should be greater than 0" );
        return E_FAIL;
    }

    if ( kmeHeader.m_dwOffset_Attributes == NULL )
    {
        ErrorLogMsg(KEM_ERROR193, m_DeviceID.c_str());
        ASSERT( !"Invalid KMEHeader::m_dwOffset_Attributes" );
        return E_FAIL;
    }

	ASSERT( m_pdxMaterials == NULL );
	ASSERT( m_pAttributeRange == NULL );
#ifndef _KSMTOOL
	ASSERT( m_ppTextures == NULL );
#endif // #ifndef _KSMTOOL
	ASSERT( m_pMesh == NULL );

    m_vCenter = kmeHeader.m_vCenter;
    m_fRadius = kmeHeader.m_fRadius;

    m_dwNumMaterials = 0;

    {
        const KMEMaterial* pkmeMaterial = static_cast<const KMEMaterial*>
            ( kMemory.AccessData( kmeHeader.m_dwOffset_Materials, sizeof(KMEMaterial) * kmeHeader.m_dwNumMaterials ) );
        if ( pkmeMaterial == NULL )
        {
            ErrorLogMsg(KEM_ERROR180, m_DeviceID.c_str());
            ASSERT( !"invalid KMEHeader::m_dwOffset_Materials" );
            return E_FAIL;
        }//if

        m_dwNumMaterials = kmeHeader.m_dwNumMaterials;
        m_pdxMaterials = new D3DXMATERIAL[ m_dwNumMaterials ];
        
        ASSERT( m_pdxMaterials != NULL );
        for( DWORD dwMat = 0; dwMat < m_dwNumMaterials; dwMat++ )
            m_pdxMaterials[ dwMat ].pTextureFilename = NULL;

#ifndef _KSMTOOL

        m_ppTextures = new CKTDXDeviceTexture*[ m_dwNumMaterials ];
        ZeroMemory( m_ppTextures, sizeof(CKTDXDeviceTexture*) * m_dwNumMaterials );

#endif // #ifndef _KSMTOOL

        for( DWORD dwMat = 0; dwMat < m_dwNumMaterials; dwMat++ )
        {
            m_pdxMaterials[ dwMat ].MatD3D = pkmeMaterial[ dwMat ].m_d3dMat;
            if ( pkmeMaterial[ dwMat ].m_dwOffset_TextureFilename != NULL )
            {
                m_pdxMaterials[ dwMat ].pTextureFilename = kMemory.RetrieveDupString(
                    pkmeMaterial[ dwMat ].m_dwOffset_TextureFilename );
                if ( m_pdxMaterials[ dwMat ].pTextureFilename == NULL )
                {
                    ErrorLogMsg(KEM_ERROR182, m_DeviceID.c_str());
                    ASSERT( !"invalid KMEMeshContainer::m_dwOffset_TextureFilename" );
                    hr = E_FAIL;
                    goto End;
                }//if
            }//if
        }//for
    }//if.. else..

    hr = D3DXCreateMeshFVF( kmeHeader.m_dwNumFaces
        , kmeHeader.m_dwNumVertices
        , ( kmeHeader.m_dwOptions & D3DXMESH_32BIT ) | D3DXMESH_MANAGED
        , kmeHeader.m_dwFVF
#ifdef _KSMTOOL
        , DXUTGetD3DDevice()
#else
        , GET_D3DDEVICE()
#endif // #ifdef _KSMTOOL
        , &m_pMesh );
    if ( FAILED( hr ) )
    {
        ErrorLogMsg(KEM_ERROR183, m_DeviceID.c_str());
        ASSERT( !"mesh creation failed" );
        goto End;
    }//if

    // fill vertex buffer 
    {
        DWORD dwVertexSize = m_pMesh->GetNumBytesPerVertex();
        DWORD dwFVFVertexSize = D3DXGetFVFVertexSize( kmeHeader.m_dwFVF );
        if ( dwVertexSize != dwFVFVertexSize )
        {
            ErrorLogMsg(KEM_ERROR185, m_DeviceID.c_str());
            ASSERT( !"vertex size mismatch" );
            hr = E_FAIL;
            goto End;
        }//if
        const void* pKSMVertex = kMemory.AccessData( kmeHeader.m_dwOffset_Vertices, kmeHeader.m_dwNumVertices * dwVertexSize );
        if ( pKSMVertex == NULL )
        {
            ErrorLogMsg(KEM_ERROR186, m_DeviceID.c_str());
            ASSERT( !"invalid KMEHeader::m_dwOffset_Vertices" );
            hr = E_FAIL;
            goto End;
        }//if
        void* pVertex = NULL;
        hr = m_pMesh->LockVertexBuffer( 0, &pVertex );
        if ( FAILED( hr ) )
        {
            ErrorLogMsg(KEM_ERROR187, m_DeviceID.c_str());
            ASSERT( !"LockVertexBuffer failed" );
            hr = E_FAIL;
            goto End;
        }//if
        CopyMemory( pVertex, pKSMVertex, kmeHeader.m_dwNumVertices * dwVertexSize );
        hr = m_pMesh->UnlockVertexBuffer();
        if(FAILED(hr))            
            ErrorLogMsg(KEM_ERROR188, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
    }//if.. else..

    // fill index buffer

    {
        DWORD   dwIndexSize = ( kmeHeader.m_dwOptions & D3DXMESH_32BIT ) ? sizeof(DWORD) : sizeof(WORD);

        const void* pKSMIndex = kMemory.AccessData( kmeHeader.m_dwOffset_Indices, kmeHeader.m_dwNumFaces * 3 * dwIndexSize );
        if ( pKSMIndex == NULL )
        {
            ErrorLogMsg(KEM_ERROR190, m_DeviceID.c_str());
            ASSERT( !"invalid KMEHeader::m_dwOffset_Indices" );
            hr = E_FAIL;
            goto End;
        }//if
        void* pIndex = NULL;
        hr = m_pMesh->LockIndexBuffer( 0, &pIndex );
        if ( FAILED( hr ) )
        {
            ErrorLogMsg(KEM_ERROR191, m_DeviceID.c_str());
            ASSERT( !"LockIndexBuffer failed" );
            goto End;
        }//if
        CopyMemory( pIndex, pKSMIndex, kmeHeader.m_dwNumFaces * 3 * dwIndexSize );
        hr = m_pMesh->UnlockIndexBuffer();
        if(FAILED(hr))            
            ErrorLogMsg(KEM_ERROR192, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
    }//if.. else..

    // fill attribute buffer

    {
        const void* pKSMAttrib = kMemory.AccessData( kmeHeader.m_dwOffset_Attributes, kmeHeader.m_dwNumFaces * sizeof(DWORD) );
        if ( pKSMAttrib == NULL )
        {
            ErrorLogMsg(KEM_ERROR194, m_DeviceID.c_str());
            ASSERT( !"invalid KMEHeader::m_dwOffset_Attributes" );
            hr = E_FAIL;
            goto End;
        }//if
        DWORD* pdwAttrib = NULL;
        hr = m_pMesh->LockAttributeBuffer( 0, &pdwAttrib );
        if ( FAILED( hr ) )
        {
            ErrorLogMsg(KEM_ERROR195, m_DeviceID.c_str());
            ASSERT( !"LockAttributeBuffer failed" );
            goto End;
        }//if
        CopyMemory( pdwAttrib, pKSMAttrib, kmeHeader.m_dwNumFaces * sizeof(DWORD) );
        hr = m_pMesh->UnlockAttributeBuffer();
        if(FAILED(hr))            
            ErrorLogMsg(KEM_ERROR196, m_DeviceID.c_str());
        ASSERT( SUCCEEDED( hr ) );
    }//if.. else..

    m_dwNumAttrGroups = 0;
    m_pAttributeRange = NULL;

#if defined(KME_ATTRIBUTE_SORT) || !defined(_KSMTOOL)

    {

        DWORD*  pdwAdjacency = new DWORD[ kmeHeader.m_dwNumFaces * 3 ];
        if(pdwAdjacency == NULL)    
            ErrorLogMsg(KEM_ERROR200, m_DeviceID.c_str());
        ASSERT( pdwAdjacency != NULL );

        hr = m_pMesh->GenerateAdjacency( 0.f, pdwAdjacency );
        if ( FAILED( hr ) )
        {
            SAFE_DELETE_ARRAY( pdwAdjacency );
            hr = S_OK;
            goto End;
        }//if

		DWORD   dwFlag = D3DXMESH_MANAGED;

#ifdef KME_ATTRIBUTE_SORT
        dwFlag |= D3DXMESHOPT_ATTRSORT;
#endif // #ifdef KME_ATTRIBUTE_SORT

#ifndef _KSMTOOL
        dwFlag |= D3DXMESHOPT_VERTEXCACHE;
#endif // #ifndef _KSMTOOL

#ifndef _KSMTOOL
	    D3DXWELDEPSILONS Epsilons;
	    ZeroMemory( &Epsilons, sizeof(D3DXWELDEPSILONS) );
        hr = D3DXWeldVertices( m_pMesh, 0, &Epsilons,
		    (DWORD*)pdwAdjacency,
		    (DWORD*)pdwAdjacency, NULL, NULL );
	    if( FAILED( hr ) )
	    {
		    SAFE_DELETE_ARRAY( pdwAdjacency );
		    ErrorLog( KEM_ERROR357 );
            hr = S_OK;
            goto End;
	    }
#endif

        LPD3DXMESH  pMesh = NULL;
	    DWORD* pdwAdjacency2 = new DWORD[ kmeHeader.m_dwNumFaces * 3 ];
	    if(pdwAdjacency2 == NULL)    
		    ErrorLogMsg(KEM_ERROR200, m_DeviceID.c_str());
	    ASSERT( pdwAdjacency2 != NULL );

	    hr = m_pMesh->Optimize( dwFlag ,
					    pdwAdjacency,
					    pdwAdjacency2, NULL, NULL, &pMesh );
        SAFE_DELETE_ARRAY( pdwAdjacency );
        if ( FAILED( hr ) )
        {
		    SAFE_DELETE_ARRAY( pdwAdjacency2 );
            hr = S_OK;
            goto End;
        }//if

        if( pMesh == NULL)    
            ErrorLogMsg(KEM_ERROR201, m_DeviceID.c_str());
        ASSERT( pMesh != NULL );
        SAFE_RELEASE( m_pMesh );
        m_pMesh = pMesh;
    }

#ifdef KME_ATTRIBUTE_SORT

    hr = m_pMesh->GetAttributeTable( NULL, &m_dwNumAttrGroups );
    if ( SUCCEEDED( hr ) )
    {
        if(m_dwNumAttrGroups <= 0)    
            ErrorLogMsg(KEM_ERROR202, m_DeviceID.c_str());
        ASSERT( m_dwNumAttrGroups > 0 );
        if ( m_dwNumAttrGroups > 0 )
        {
            m_pAttributeRange = new D3DXATTRIBUTERANGE[ m_dwNumAttrGroups ];
            if(m_pAttributeRange == NULL)    
                ErrorLogMsg(KEM_ERROR203, m_DeviceID.c_str());
            ASSERT( m_pAttributeRange != NULL );
            hr = m_pMesh->GetAttributeTable( m_pAttributeRange, &m_dwNumAttrGroups );
            if(FAILED(hr))    
                ErrorLogMsg(KEM_ERROR204, m_DeviceID.c_str());
            ASSERT( SUCCEEDED( hr ) );
        }//if
    }
    else
    {
        m_dwNumAttrGroups = 0;
    }//if.. else..

#endif // #ifdef KME_ATTRIBUTE_SORT

#endif // #if defined(KME_ATTRIBUTE_SORT) || !defined(_KSMTOOL)

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	//subset 단위로 bounding sphere 저장
	void* pVertex = NULL;
	hr = m_pMesh->LockVertexBuffer( D3DLOCK_READONLY, &pVertex );
	if ( SUCCEEDED( hr ) )
	{
		D3DXVECTOR3 vSubsetCenter;
		float fSubsetRadius;
        m_vecSubsetCullingInfo.resize( m_dwNumAttrGroups );
		for( int i = 0; i < (int) m_dwNumAttrGroups; i++ )
		{
			hr = D3DXComputeBoundingSphere( (D3DXVECTOR3*)((MESH_VERTEX*)pVertex + m_pAttributeRange[i].VertexStart), m_pAttributeRange[i].VertexCount,
				m_pMesh->GetNumBytesPerVertex(),
				&vSubsetCenter, &fSubsetRadius );
			if( SUCCEEDED( hr ) )
            {
                m_vecSubsetCullingInfo[ i ].m_vCenter = vSubsetCenter;
                m_vecSubsetCullingInfo[ i ].m_fRadius = fSubsetRadius;
            }
            else
            {
                m_vecSubsetCullingInfo[ i ].m_vCenter = D3DXVECTOR3(0,0,0);
                m_vecSubsetCullingInfo[ i ].m_fRadius = 0.f;
            }
		}
		m_pMesh->UnlockVertexBuffer();
	}
    else
    {
        m_vecSubsetCullingInfo.resize(0);
    }
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

    hr = S_OK;

End:

    if ( FAILED( hr ) )
    {
        for( DWORD i = 0; i < m_dwNumMaterials; i++ )
            SAFE_DELETE_ARRAY( m_pdxMaterials[ i ].pTextureFilename );
	    SAFE_DELETE_ARRAY( m_pdxMaterials );
	    m_dwNumMaterials = 0L;

        SAFE_DELETE_ARRAY( m_pAttributeRange );
        m_dwNumAttrGroups = 0;

	    SAFE_RELEASE( m_pMesh );
    }//if
    
    return hr;


}//CKTDXDeviceXMesh::XMeshProxy::LoadKMEMesh()


#ifndef _KSMTOOL

void            CKTDXDeviceXMesh::XMeshProxy::LoadTextures()
{
    for( DWORD dwMat = 0; dwMat < m_dwNumMaterials; dwMat++ )
    {
        if ( m_pdxMaterials[ dwMat ].pTextureFilename != NULL 
            && m_pdxMaterials[ dwMat ].pTextureFilename[0] != NULL )
        {
			WCHAR wszTexName[ MAX_PATH ];
			if( MultiByteToWideChar( CP_ACP, 0, m_pdxMaterials[ dwMat ].pTextureFilename, -1, wszTexName, MAX_PATH ) != 0 )
				m_ppTextures[ dwMat ] = g_pKTDXApp->GetDeviceManager()->OpenTexture( wszTexName );
        }
        else
            m_ppTextures[ dwMat ] = NULL;
    }//for
}//CKTDXDeviceXMesh::LoadTextures()


//#ifdef BACKGROUND_LOADING_TEST // 2008-12-12
HRESULT CKTDXDeviceXMesh::XMeshProxy::LoadXET()
{
	//안쓰는 kom파일 없애는거(지금은 이름 바꾸는거) 넣은 부분임
	int strIndex = m_DeviceID.rfind( L"." );
	if ( strIndex != -1 )
	{
		wstring fName = m_DeviceID.substr( 0, strIndex );
		fName += L".XET";
		m_pXET = g_pKTDXApp->GetDeviceManager()->OpenXET( fName );
	}
	else
	{
		wstring fName = m_DeviceID;
		fName += L"XET";
		m_pXET = g_pKTDXApp->GetDeviceManager()->OpenXET( fName );
	}//if.. else..

	return m_pXET != NULL ? S_OK : E_FAIL;

}//CKTDXDeviceXMesh::LoadXET()
//#endif // BACKGROUND_LOADING_TEST // 2008-12-12

#endif // #ifndef _KSMTOOL