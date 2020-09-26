#include "StdAfx.h"
#include ".\ktdgparticlesystem.h"


#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
CKTDGParticleSystem::CParticleEventSequenceHandle CKTDGParticleSystem::s_iNextParticleEventSequenceHandle = 0;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_CULLING_PARTICLE
 bool   CKTDGParticleSystem::ms_bParticleCullingEnabled = false;
#endif  X2OPTIMIZE_CULLING_PARTICLE


//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{		
		KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
		KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
		KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),		
		KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
};//s_akStates[]
//}} robobeg : 2008-10-13

//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akRHWRenderStates[] = 
{
		KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
		KRenderState( D3DRS_ZENABLE,			FALSE ),
		KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
		KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),
		KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
};//s_akStates[]
//}} robobeg : 2008-10-13

//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akMeshRenderStates[] = 
{		
		KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
		KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),
		KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
};//s_akStates[]
//}} robobeg : 2008-10-13





static const CKTDGStateManager::KState s_akSkinMeshRenderStates[] = 
{		
	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),
	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
};




CKTDGParticleSystem::CKTDGParticleSystem( LPDIRECT3DDEVICE9 pd3dDevice
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            , unsigned char ucSystemID
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK    
    )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    : m_coParticleSequence( SEQUENCELIST_NUM, 512 )
    , m_coParticleList( PARTICLELIST_NUM, 2048 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    m_ucSystemID = ucSystemID & 0x3;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    m_bInCriticalLoop = false;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

    m_vecVERTEX_PARTICLE.reserve( 1024 );
    m_vecVERTEX_PARTICLE_RHW.reserve( 1024 );

//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//	m_Offset		= 0;   
//	m_OffsetRHW		= 0;	
//	m_BatchSize		= 20; 
//
//	m_vbParticles		= NULL;
//	m_vbParticlesRHW	= NULL;
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

	m_Priority		= 0;
	m_bEnable		= true;

#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_InstanceSequences.reserve( 256 );
	m_InstanceSequencesReady.reserve( 256 );
	InitializeCriticalSection( &m_csParticleLock );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE



//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//	m_bUseDynamicTexture = false;
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB


//	HRESULT hr;
	 //create vertex buffer


/*
	const D3DCAPS9* pD3DCAPS9 = DXUTGetDeviceCaps();
	if ( pD3DCAPS9 != NULL && (  pD3DCAPS9->Caps2 & D3DCAPS2_DYNAMICTEXTURES ) != 0 )
	{
		m_bUseDynamicTexture = true;

		m_iVBSize		= 100;

		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer(m_iVBSize * 6 * sizeof(VERTEX_PARTICLE), 
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
			D3DFVF_PARTICLE, D3DPOOL_DEFAULT,
			&m_vbParticles, NULL ))) 
		{
			ErrorLogMsg( KEM_ERROR27, "Fail: CKTDGParticleSystem::CreateVertexBuffer" );
			wcout << L"Fail: CKTDGParticleSystem::CreateVertexBuffer" << std::endl;
			return;
		}

		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer(m_iVBSize * 6 * sizeof(VERTEX_PARTICLE_RHW), 
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
			D3DFVF_PARTICLE_RHW, D3DPOOL_DEFAULT, 
			&m_vbParticlesRHW, NULL ))) 
		{
			ErrorLogMsg( KEM_ERROR28, "Fail: CKTDGParticleSystem::CreateVertexBufferRHW" );
			wcout << L"Fail: CKTDGParticleSystem::CreateVertexBufferRHW" << std::endl;
			return;
		}	
	}
	else
*/
//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//	{
//		m_iVBSize		= 100;
//
//		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer(m_iVBSize * 6 * sizeof(VERTEX_PARTICLE), 
//			D3DUSAGE_WRITEONLY, 
//			D3DFVF_PARTICLE, D3DPOOL_MANAGED , 
//			&m_vbParticles, NULL ))) 
//		{
//			ErrorLogMsg( KEM_ERROR27, "Fail: CKTDGParticleSystem::CreateVertexBuffer" );
//			wcout << L"Fail: CKTDGParticleSystem::CreateVertexBuffer" << std::endl;
//			return;
//		}
//
//		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer(m_iVBSize * 6 * sizeof(VERTEX_PARTICLE_RHW), 
//			D3DUSAGE_WRITEONLY, 
//			D3DFVF_PARTICLE_RHW, D3DPOOL_MANAGED, 
//			&m_vbParticlesRHW, NULL ))) 
//		{
//			ErrorLogMsg( KEM_ERROR28, "Fail: CKTDGParticleSystem::CreateVertexBufferRHW" );
//			wcout << L"Fail: CKTDGParticleSystem::CreateVertexBufferRHW" << std::endl;
//			return;
//		}
//	}
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB


//{{ robobeg : 2008-10-13

	m_RenderStateID = s_akStates;
	m_RHWRenderStateID = s_akRHWRenderStates;
	m_MeshRenderStateID = s_akMeshRenderStates;
	m_SkinMeshRenderStateID = s_akSkinMeshRenderStates;

//}} robobeg : 2008-10-13

#ifdef EFFECT_TOOL
	m_strCurrentFileName = L"";
#endif //EFFECT_TOOL

#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
	m_bRenderEffectMadeByGameUnit = true;
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
}

CKTDGParticleSystem::~CKTDGParticleSystem(void)
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    ASSERT( m_bInCriticalLoop == false );
    m_bInCriticalLoop = false;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
	DestroyAllInstances();

	map<wstring, CParticleEventSequence*>::iterator i;
	CParticleEventSequence* pSeq;

#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	EnterCriticalSection( &m_csParticleLock );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	for ( i = m_TempletSequences.begin(); i != m_TempletSequences.end(); i++ )
	{
		pSeq = (CParticleEventSequence*)i->second;
		SAFE_DELETE_KTDGOBJECT( pSeq );
	}
    m_TempletSequences.clear();
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	LeaveCriticalSection( &m_csParticleLock );
	DeleteCriticalSection( &m_csParticleLock );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    for( unsigned u = 0; u != m_coParticleList.storage_size(); u++ )
    {
        KParticleHandleInfo& info = m_coParticleList.data( u );
        SAFE_DELETE( info.m_pParticle );
    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//	SAFE_RELEASE( m_vbParticles );
//	SAFE_RELEASE( m_vbParticlesRHW );
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

}

HRESULT CKTDGParticleSystem::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	////CKTDXThread::CLocker locker( m_csParticleLock );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    ASSERT( m_bInCriticalLoop == false );
    m_bInCriticalLoop = true;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

	KTDXPROFILE_BEGIN( "m_InstanceSequences loop" );
	//if( false == m_InstanceSequences.empty() )
	{

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        KSequenceHandleList::iterator iterNext;
        KSequenceHandleList::iterator iterEnd = m_coParticleSequence.end(SEQUENCELIST_LIVE);
        for( KSequenceHandleList::iterator iter = m_coParticleSequence.begin(SEQUENCELIST_LIVE); 
            iter != iterEnd; 
            iter = iterNext )
        {
            iterNext = iter;    ++iterNext;
            KSequenceHandleInfo& info = *iter;
            CParticleEventSequence* pSeq = info.m_pSequence;
            if ( pSeq == NULL )
            {
                info.m_eType = SEQUENCELIST_FREE;
                m_coParticleSequence.splice( m_coParticleSequence.begin( SEQUENCELIST_FREE ), iter );
            }
            else if ( pSeq->GetDelete() == true )
            {
				KTDXPROFILE_BEGIN( "delete pseq" );	
                SAFE_DELETE_KTDGOBJECT( pSeq );
                info.m_pSequence = NULL;
                info.m_eType = SEQUENCELIST_FREE;
                m_coParticleSequence.splice( m_coParticleSequence.begin( SEQUENCELIST_FREE ), iter );
				KTDXPROFILE_END();
            }
            else
            {
				KTDXPROFILE_BEGIN( "pSeq onframemove" );	
				pSeq->OnFrameMove( fTime, fElapsedTime );
				KTDXPROFILE_END();
            }
        }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		for( int i=0; i<(int)m_InstanceSequences.size(); i++ )
		{
			CParticleEventSequence* pSeq = m_InstanceSequences[i];
			ASSERT( NULL != pSeq );
			if( NULL == pSeq )
				continue;

			if( pSeq->GetDelete() == true )
			{
				KTDXPROFILE_BEGIN( "delete pseq" );	


				m_mapInstanceSequences.erase( pSeq->GetHandle() );
				SAFE_DELETE_KTDGOBJECT( pSeq );
				m_InstanceSequences.erase( m_InstanceSequences.begin() + i );
				--i;

				KTDXPROFILE_END();
			}
			else
			{
				KTDXPROFILE_BEGIN( "pSeq onframemove" );	
				pSeq->OnFrameMove( fTime, fElapsedTime );
				KTDXPROFILE_END();
			}
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}
	KTDXPROFILE_END();




	KTDXPROFILE_BEGIN( "Ready" );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_coParticleSequence.empty( SEQUENCELIST_READY ) == false )
    {
        KSequenceHandleList::iterator iterNext;
        KSequenceHandleList::iterator iterEnd = m_coParticleSequence.end(SEQUENCELIST_READY);
        for( KSequenceHandleList::iterator iter = m_coParticleSequence.begin(SEQUENCELIST_READY); 
            iter != iterEnd; 
            iter = iterNext )
        {
            iterNext = iter;    ++iterNext;
            KSequenceHandleInfo& info = *iter;
            if ( info.m_pSequence == NULL )
            {
                info.m_eType = SEQUENCELIST_FREE;
                m_coParticleSequence.splice( m_coParticleSequence.begin( SEQUENCELIST_FREE ), iter );
            }
            else
            {
                info.m_eType = SEQUENCELIST_LIVE;
            }
        }
        m_coParticleSequence.splice_list( m_coParticleSequence.end(SEQUENCELIST_LIVE), SEQUENCELIST_READY );
    }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( false == m_InstanceSequencesReady.empty() )
	{
		m_InstanceSequences.insert( m_InstanceSequences.end(), m_InstanceSequencesReady.begin(), m_InstanceSequencesReady.end() );
		m_InstanceSequencesReady.resize(0);
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    m_bInCriticalLoop = false;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

	KTDXPROFILE_END();

	return S_OK;
}




HRESULT CKTDGParticleSystem::OnResetDevice()
{

//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB	
//	if ( m_bUseDynamicTexture == true )
//	{
//		SAFE_RELEASE( m_vbParticles );
//		SAFE_RELEASE( m_vbParticlesRHW );
//
//		HRESULT hr;
//
//		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer(m_iVBSize * 6 * sizeof(VERTEX_PARTICLE), 
//			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
//			D3DFVF_PARTICLE, D3DPOOL_DEFAULT,
//			&m_vbParticles, NULL ))) 
//		{
//			ErrorLogMsg( KEM_ERROR27, "Fail: CKTDGParticleSystem::CreateVertexBuffer" );
//			wcout << L"Fail: CKTDGParticleSystem::CreateVertexBuffer" << std::endl;
//			return hr;
//		}
//
//		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer(m_iVBSize * 6 * sizeof(VERTEX_PARTICLE_RHW), 
//			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
//			D3DFVF_PARTICLE_RHW, D3DPOOL_DEFAULT, 
//			&m_vbParticlesRHW, NULL ))) 
//		{
//			ErrorLogMsg( KEM_ERROR28, "Fail: CKTDGParticleSystem::CreateVertexBufferRHW" );
//			wcout << L"Fail: CKTDGParticleSystem::CreateVertexBufferRHW" << std::endl;
//			return hr; 
//		}
//
//	}
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
	
	return S_OK;
}

HRESULT CKTDGParticleSystem::OnLostDevice()
{

//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//	if ( m_bUseDynamicTexture == true )
//	{
//		SAFE_RELEASE( m_vbParticles );
//		SAFE_RELEASE( m_vbParticlesRHW );
//	}
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
	return S_OK;
}

void CKTDGParticleSystem::OpenScriptFile( const WCHAR* pFilename )
{
	////CKTDXThread::CLocker locker( m_csParticleLock );

	if( NULL == pFilename || pFilename[0] == NULL )
		return;

	StateLog( pFilename );	

#ifdef EFFECT_TOOL
	m_strCurrentFileName = pFilename;
#endif //EFFECT_TOOL

    bool bRet = g_pKTDXApp->LoadAndDoMemory( this, pFilename );
    if ( bRet == false )
    {
		ErrorLogMsg( KEM_ERROR29, pFilename );
#ifdef EFFECT_TOOL
		m_strCurrentFileName = L"";
#endif //EFFECT_TOOL
		return;
    }

}

bool CKTDGParticleSystem::Compile( const char *pScript, const int iLength /*= -1*/ )
{
	m_strLastError = "0 error(s), you're good to go!";

	ASSERT( NULL != pScript );
	if( NULL == pScript )
	{
		StateLog( L"particle compile null script" );
		return false;
	}


	try 
	{
		CParticleEmitterTokenizer tokenizer;

		// parse the character string into tokens.
		tokenizer.Tokenize( pScript, iLength );

		std::vector<CParticleEmitterToken>::iterator TokenIter	= tokenizer.m_TokenVector.begin();
		std::vector<CParticleEmitterToken>::iterator EndIter	= tokenizer.m_TokenVector.end();

		if (tokenizer.m_TokenVector.size() < 2) throw("This script is too small to be valid.");

		// the first three tokens out of the gate should be
		// ParticleSystem, followed by a name and version number, then
		// an open brace.
		if (TokenIter->m_Type != KeywordParticleSystem) { throw("First word must be ParticleSystem"); }
		CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
		if (TokenIter->m_Type != Quote) { throw("Must name particle system"); }
		m_strName = RemoveQuotes(TokenIter->m_strValue);
		CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

		if (TokenIter->m_Type != RealNumber) { throw("Must have version number"); }
		CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
		if (TokenIter->m_Type != OpenBrace) { throw("Missing opening brace for ParticleSystem block"); }

		// kimhc // 2011-07-22 // 이 부분 에서 템플릿을 만든다.
		ProcessParticleSystemBlock(TokenIter, EndIter);
	} 
	catch(char *e) 
	{
		m_strLastError = e; 

		WCHAR wFileName[255] = {0,};
		MultiByteToWideChar( CP_ACP, 0, m_strLastError.c_str(), -1, wFileName, 255 );

		OutputDebugString(wFileName);
#ifndef _SERVICE_
		MessageBox( NULL, wFileName, NULL, NULL );
#endif 
		
		goto particle_compile_error;

	} 
	catch(const char *e) 
	{
		m_strLastError = e; 
		WCHAR wFileName[255] = {0,};
		MultiByteToWideChar( CP_ACP, 0, m_strLastError.c_str(), -1, wFileName, 255 );

		OutputDebugString(wFileName);
#ifndef _SERVICE_
		MessageBox( NULL, wFileName, NULL, NULL );
#endif 
		
		goto particle_compile_error;

	} 
	catch(WCHAR *pErrString) 
	{
		wstring lastError = pErrString;
		SAFE_DELETE_ARRAY( pErrString )

#ifndef _SERVICE_
		MessageBox( NULL, lastError.c_str(), NULL, NULL );
#endif 
		
		goto particle_compile_error;

	} 
	catch (...) 
	{
		m_strLastError = "Unhandled exception in CParticleEmitter::Compile.";
		WCHAR wFileName[255] = {0,};
		MultiByteToWideChar( CP_ACP, 0, m_strLastError.c_str(), -1, wFileName, 255);

		OutputDebugString(wFileName);

#ifndef _SERVICE_
		MessageBox( NULL, wFileName, NULL, NULL );
#endif 
				
		goto particle_compile_error;
	}

	return true;



//{{ dmlee 2008.03.17 디버깅 용도
particle_compile_error:
	
	wstring wstrLastError = L"";
	ConvertCharToWCHAR( wstrLastError, m_strLastError.c_str() );

	WCHAR wszText[1024] = L"";
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"particle_compile_error_%s", wstrLastError.c_str() );
	StateLog( wszText );

	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"pScript: %p", pScript );
	StateLog( wszText );

	//swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"token vector size: %d", (int)tokenizer.m_TokenVector.size() );
	//StateLog( wszText );

	return false;
//}} dmlee 2008.03.17 디버깅 용도

}

#ifdef EVENT_SCENE
//{{ oasis907 : 김상윤 [2010.6.10] // 
// txt에서 파싱된 Templet들이 등록된 TempletMap과 상관없이 동작할 수 있게금 하였습니다.
// 대신 CParticleEventSequence의 내용은 다른 곳에서 수동으로 입력해야합니다.

// not need: pSeq 외의 다른 매개변수는 필요 없을 듯
CKTDGParticleSystem::CParticleEventSequenceHandle CKTDGParticleSystem::CreateInstanceNonTemplet( CParticleEventSequence* pSeq, D3DXVECTOR3 pos, D3DXVECTOR2 emitRate, D3DXVECTOR2 trigger )
{
	// 핸들 등록
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( pSeq == NULL )
        return INVALID_PARTICLE_SEQUENCE_HANDLE;
    {
        KSequenceHandleList::iterator iterEmpty = m_coParticleSequence.begin(SEQUENCELIST_FREE);
        if ( iterEmpty == m_coParticleSequence.end(SEQUENCELIST_FREE) )
        {
            if ( m_coParticleSequence.storage_size() >= 0x10000 )
            {
                return INVALID_PARTICLE_SEQUENCE_HANDLE;
            }
            m_coParticleSequence.push_back_default(SEQUENCELIST_FREE);
            iterEmpty = m_coParticleSequence.begin(SEQUENCELIST_FREE);
            ASSERT( iterEmpty != m_coParticleSequence.end(SEQUENCELIST_FREE) );
        }
        m_coParticleSequence.splice( m_coParticleSequence.end(SEQUENCELIST_LIVE), iterEmpty );
        iterEmpty->m_pSequence = pSeq;
        iterEmpty->m_eType = SEQUENCELIST_LIVE;
        WORD   wIndex = (WORD) iterEmpty.GetIndex();
        CParticleEventSequenceHandle    handle;
        DWORD   dwHandle = 0;
        do
        {
            ++iterEmpty->m_wStamp;
            dwHandle = ComposeHandle( wIndex, iterEmpty->m_wStamp );
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            handle.SetValue( (int) dwHandle );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            handle = (int) dwHandle;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        } while( handle == INVALID_PARTICLE_SEQUENCE_HANDLE );
        pSeq->SetHandle( handle );
    }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	pSeq->SetHandle( s_iNextParticleEventSequenceHandle );
	++s_iNextParticleEventSequenceHandle;
	if( s_iNextParticleEventSequenceHandle > INT_MAX-2 ) 
	{
		s_iNextParticleEventSequenceHandle = 0;
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	// 텍스쳐 리소스 로드
	CParticleEvent* pEvent = NULL;
	CKTDXDeviceTexture* pTexture = NULL;
	int i = 0;
	BOOST_TEST_FOREACH( CParticleEvent*, pEvent, pSeq->m_EventList )
	{
		switch( pEvent->GetEventType() )
		{
		case ET_TEXTURE:
			{
				CParticleEvent_Texture* pEventTexture = (CParticleEvent_Texture*)pEvent;
				pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( pEventTexture->GetTextureName() );
				pSeq->m_TextureMap.insert( std::make_pair(i,pTexture) );
				pEventTexture->SetTextureID( i );
			}
			break;
		}
		i++;
	}

	pSeq->SetParticleEventSequence(pSeq);


	// m_mapInstanceSequences 및 m_InstanceSequences에 등록하여, framemove, framerender 가능하게
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	ASSERT( m_mapInstanceSequences.find( pSeq->GetHandle() ) == m_mapInstanceSequences.end() );
	m_mapInstanceSequences[ pSeq->GetHandle() ] = pSeq;
	m_InstanceSequences.push_back( pSeq );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


	pSeq->SetPosition( pos );

	switch( pSeq->GetParticleType() )
	{
	// oasis907 : 김상윤 [2010.6.21] // EventScene에서 XMesh의 좌표와 Particle의 좌표를 통일하기 위해
	// Particle을 PT_3D_PLANE으로 동작시킴
	case PT_3D_PLANE:
		pSeq->SetRenderStateID( m_RenderStateID );		
		break;
	case PT_2D_PLANE:
		pSeq->SetRenderStateID( m_RHWRenderStateID );
		break;
	case PT_MESH:
		pSeq->SetRenderStateID( m_MeshRenderStateID );
		break;
	case PT_SKINMESH:
		pSeq->SetRenderStateID( m_SkinMeshRenderStateID );		
		break;	
	}

	pSeq->InChain();

	return pSeq->GetHandle();
}
//}}
#endif EVENT_SCENE

CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::CreateSequence( CKTDGObject* pKTDGObject, 
																				const WCHAR* pSequenceName, 
																				 float x, float y, float z, 
																				 float minEmitrate, float maxEmitrate, int drawCount, 
																				 int triggerCount, float triggerTime,
																				 bool bCullCheck, float latency, bool bNextFrame )
{
	KTDXPROFILE();	

	//CKTDXThread::CLocker locker( m_csParticleLock );

	ASSERT( NULL != pSequenceName );

#ifdef NO_MORE_ERROR_TEST
	if( NULL == pSequenceName )
	{
		InHouseLog( "이름이 없는 파티클 생성 시도" );
	}
#endif NO_MORE_ERROR_TEST


	if( NULL == pSequenceName )
	{
		return NULL;
	}

	if( m_bEnable == false )
	{
		return NULL;
	}


	map<wstring, CParticleEventSequence*>::iterator i = m_TempletSequences.find( pSequenceName );
	if( i == m_TempletSequences.end() )
	{
		return NULL;
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    KSequenceHandleList::iterator iterEmpty = m_coParticleSequence.begin(SEQUENCELIST_FREE);
    if ( iterEmpty == m_coParticleSequence.end(SEQUENCELIST_FREE) )
    {
        if ( m_coParticleSequence.storage_size() >= 0x10000L )
        {
            return NULL;
        }
        m_coParticleSequence.push_back_default(SEQUENCELIST_FREE);
        iterEmpty = m_coParticleSequence.begin(SEQUENCELIST_FREE);
        ASSERT( iterEmpty != m_coParticleSequence.end(SEQUENCELIST_FREE) );
    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	CParticleEventSequence* retval = i->second->GetCloneSequence();

#ifdef FIX_EFFECT_REFLECT
	if ( NULL == retval )
		return NULL;
#endif

	// handle 발급
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    iterEmpty->m_pSequence = retval;
    WORD    wIndex = (WORD) iterEmpty.GetIndex();
    CParticleEventSequenceHandle    handle;
    DWORD   dwHandle = 0;
    do
    {
        ++iterEmpty->m_wStamp;
        dwHandle = ComposeHandle( wIndex, iterEmpty->m_wStamp );
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        handle.SetValue( (int) dwHandle );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        handle = (int) dwHandle;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    } while ( handle == INVALID_PARTICLE_SEQUENCE_HANDLE );
    retval->SetHandle( handle );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	retval->SetHandle( s_iNextParticleEventSequenceHandle );
	++s_iNextParticleEventSequenceHandle;
	if( s_iNextParticleEventSequenceHandle > INT_MAX-2 ) 
	{
		s_iNextParticleEventSequenceHandle = 0;
	}

	ASSERT( m_mapInstanceSequences.find( retval->GetHandle() ) == m_mapInstanceSequences.end() );
	m_mapInstanceSequences[ retval->GetHandle() ] = retval;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE



#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
	if( NULL != pKTDGObject )
	{
		switch( pKTDGObject->GetObjectType() )
		{
		case CKTDGObject::OT_CX2GUUser:
		case CKTDGObject::OT_CX2GUNPC:
			{
				// 게임 유닛에 의해 생성된 이펙트인 경우 타입 지정(거리에 따라서 DRAW 하지 않을 이펙트를 판단 하는데 사용)
				retval->SetObjectType( CKTDGObject::OT_EFFECT_MADE_BY_GAME_UNIT );
				if( pKTDGObject->GetScaleByUnit() != D3DXVECTOR3( 1, 1, 1 ) )
				{
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
					if( retval->GetApplyUnitScale() == true )
						retval->SetScaleByUnit( pKTDGObject->GetScaleByUnit() );
#else
					retval->SetScaleByUnit( pKTDGObject->GetScaleByUnit() );
#endif
				}
			} break;
		}
	}
#endif GIANT_UNIT_GIANT_EFFECT_TEST


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( bNextFrame == false )
    {
        iterEmpty->m_eType = SEQUENCELIST_LIVE;
        m_coParticleSequence.splice( m_coParticleSequence.end(SEQUENCELIST_LIVE), iterEmpty );
    }
    else
    {
        iterEmpty->m_eType = SEQUENCELIST_READY;
        m_coParticleSequence.splice( m_coParticleSequence.end(SEQUENCELIST_READY), iterEmpty );
    }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( bNextFrame == false )
	{
		m_InstanceSequences.push_back( retval );
        
	}
	else
	{
		m_InstanceSequencesReady.push_back( retval );
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	if( maxEmitrate != -1.0f && minEmitrate != -1.0f )
	{
		CMinMax<float> emitrate(minEmitrate,maxEmitrate);
		retval->SetEmitRate( emitrate );
	}	
	
	retval->SetPosition( D3DXVECTOR3( x, y, z ) );

	if( triggerCount != -1 )
		retval->SetTriggerCount( triggerCount );
	if( triggerTime != -1 )
		retval->SetTriggerTime( triggerTime );
	if( drawCount != -1 )
		retval->SetDrawCount( drawCount );
	retval->SetCullCheck( bCullCheck );
	retval->SetLatency( latency );

	switch( retval->GetParticleType() )
	{
	case PT_3D_PLANE:
		retval->SetRenderStateID( m_RenderStateID );		
		break;

	case PT_2D_PLANE:
		retval->SetRenderStateID( m_RHWRenderStateID );
		break;

	case PT_MESH:
		retval->SetRenderStateID( m_MeshRenderStateID );
		break;

	case PT_SKINMESH:
		retval->SetRenderStateID( m_SkinMeshRenderStateID );		
		break;	
	}

	retval->InChain();

	return retval;
}





void CKTDGParticleSystem::DestroyAllInstances()
{
	//CKTDXThread::CLocker locker( m_csParticleLock );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    if ( IsInCriticalLoop() == true )
    {
        KSequenceHandleList::iterator iterEnd = m_coParticleSequence.end(SEQUENCELIST_LIVE);
        for( KSequenceHandleList::iterator iter = m_coParticleSequence.begin(SEQUENCELIST_LIVE);
            iter != iterEnd;
            ++iter )
        {
            CParticleEventSequence* pSeq = iter->m_pSequence;
            if ( pSeq != NULL )
                pSeq->_SetDelete();
        }
        iterEnd = m_coParticleSequence.end(SEQUENCELIST_READY);
        for( KSequenceHandleList::iterator iter = m_coParticleSequence.begin(SEQUENCELIST_READY);
            iter != iterEnd;
            ++iter  )
        {
            CParticleEventSequence* pSeq = iter->m_pSequence;
            if ( pSeq != NULL )
                pSeq->_SetDelete();
        }
    }
    else
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    {
        KSequenceHandleList::iterator iterNext;
        KSequenceHandleList::iterator iterEnd = m_coParticleSequence.end(SEQUENCELIST_LIVE);
        for( KSequenceHandleList::iterator iter = m_coParticleSequence.begin(SEQUENCELIST_LIVE);
            iter != iterEnd;
            iter = iterNext )
        {
            iterNext = iter; ++iterNext;
            CParticleEventSequence* pSeq = iter->m_pSequence;
            iter->m_pSequence = NULL;
            iter->m_eType = SEQUENCELIST_FREE;
            SAFE_DELETE_KTDGOBJECT( pSeq );
        }
        iterEnd = m_coParticleSequence.end(SEQUENCELIST_READY);
        for( KSequenceHandleList::iterator iter = m_coParticleSequence.begin(SEQUENCELIST_READY);
            iter != iterEnd;
            iter = iterNext )
        {
            iterNext = iter; ++iterNext;
            CParticleEventSequence* pSeq = iter->m_pSequence;
            iter->m_pSequence = NULL;
            iter->m_eType = SEQUENCELIST_FREE;
            SAFE_DELETE_KTDGOBJECT( pSeq );
        }
        m_coParticleSequence.splice_list( m_coParticleSequence.begin(SEQUENCELIST_FREE), SEQUENCELIST_LIVE );
        m_coParticleSequence.splice_list( m_coParticleSequence.begin(SEQUENCELIST_FREE), SEQUENCELIST_READY );
    }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	for( int i = 0; i < (int)m_InstanceSequences.size(); i++ )
	{
		SAFE_DELETE_KTDGOBJECT( m_InstanceSequences[i] );
	}	
	m_InstanceSequences.resize( 0 );

	for( int i = 0; i < (int)m_InstanceSequencesReady.size(); i++ )
	{
		SAFE_DELETE_KTDGOBJECT( m_InstanceSequencesReady[i] );
	}
	m_InstanceSequencesReady.resize(0); 
	m_mapInstanceSequences.clear();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CKTDGParticleSystem::DestroyInstance_LUA( CParticleEventSequence* pSeq )
{
    DestroyInstance( pSeq );
}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CKTDGParticleSystem::DestroyInstance( CParticleEventSequence*& pSeq )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CKTDGParticleSystem::DestroyInstance( CParticleEventSequence* pSeq )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
{
	if( pSeq == NULL )
		return ;


    CParticleEventSequenceHandle hSeq = pSeq->GetHandle();

#ifdef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

    if ( hSeq == INVALID_PARTICLE_SEQUENCE_HANDLE )
    {
        pSeq = NULL;
        return;
    }
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq.GetValue() );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    if ( DecomposeHandle( dwHandle, wIndex, wStamp ) == false )
    {
        ASSERT( 0 );
        pSeq = NULL;
        return;
    }
    if ( wIndex >= m_coParticleSequence.storage_size() )
    {
        pSeq = NULL;
        return;
    }
    KSequenceHandleInfo& info = m_coParticleSequence.data( wIndex );
    if ( info.m_pSequence != pSeq || info.m_wStamp != wStamp )
    {
        pSeq = NULL;
        return;
    }
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    if ( IsInCriticalLoop() == true )
    {
        pSeq->_SetDelete();
        pSeq = NULL;
        return;
    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

    info.m_pSequence = NULL;
    info.m_eType = SEQUENCELIST_FREE;
    SAFE_DELETE_KTDGOBJECT( pSeq );
    m_coParticleSequence.splice( m_coParticleSequence.begin( SEQUENCELIST_FREE ), wIndex );
    pSeq = NULL;

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	map< CParticleEventSequenceHandle, CParticleEventSequence* >::iterator itseq 
		= m_mapInstanceSequences.find( hSeq );
	if( itseq == m_mapInstanceSequences.end() )
		return;
	if( pSeq != itseq->second )
		return; 

	UINT nSize = m_InstanceSequences.size();
	for( UINT i=0; i<nSize; ++i )
	{
		CParticleEventSequence* pSeqTemp = m_InstanceSequences[i];
		if( pSeqTemp == pSeq &&
			pSeqTemp->GetHandle() == hSeq )
		{
			m_InstanceSequences.erase( m_InstanceSequences.begin() + i );
			SAFE_DELETE_KTDGOBJECT( pSeq );
			break;
		}
	}	

	m_mapInstanceSequences.erase( hSeq );

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

}

void CKTDGParticleSystem::DestroyInstanceHandle( CParticleEventSequenceHandle& hSeq )
{
	//kimhc // 2009-09-16 // DestroyInstanceHandle 실행시 INVALID_PARTICLE_SEQUENCE_HANDLE 인지 검사
	if ( hSeq == INVALID_PARTICLE_SEQUENCE_HANDLE )
		return;

#ifdef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq.GetValue() );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    if ( DecomposeHandle( dwHandle, wIndex, wStamp ) == false )
    {
        ASSERT( 0 );
        hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
        return;
    }
    if ( wIndex >= m_coParticleSequence.storage_size() )
    {
        hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
        return;
    }
    KSequenceHandleInfo& info = m_coParticleSequence.data( wIndex );
    if ( info.m_pSequence == NULL || info.m_wStamp != wStamp )
    {
        hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
        return;
    }
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    if ( IsInCriticalLoop() == true )
    {
        info.m_pSequence->_SetDelete();
        hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
        return;
    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

    CParticleEventSequence* pSeq = info.m_pSequence;
    info.m_pSequence = NULL;
    info.m_eType = SEQUENCELIST_FREE;
    SAFE_DELETE_KTDGOBJECT( pSeq );
    m_coParticleSequence.splice( m_coParticleSequence.begin( SEQUENCELIST_FREE ), wIndex );
    hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
#else//X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	map< CParticleEventSequenceHandle, CParticleEventSequence* >::iterator itseq 
		= m_mapInstanceSequences.find( hSeq );
	if( itseq == m_mapInstanceSequences.end() )
		return;

	CParticleEventSequence* pSeq = itseq->second;
	
	UINT nSize = m_InstanceSequences.size();
	for( UINT i=0; i<nSize; ++i )
	{
		CParticleEventSequence* pSeqTemp = m_InstanceSequences[i];
		if( pSeqTemp == pSeq &&
			pSeqTemp->GetHandle() == hSeq )
		{
			m_InstanceSequences.erase( m_InstanceSequences.begin() + i );
			SAFE_DELETE_KTDGOBJECT( pSeq );
			break;
		}
	}	

	m_mapInstanceSequences.erase( hSeq );

	hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif//X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

}


bool CKTDGParticleSystem::IsLiveInstance( CParticleEventSequence* pSeq )
{
	if( NULL == pSeq )
		return false;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CParticleEventSequenceHandle hSeq = pSeq->GetHandle();

	if ( hSeq == INVALID_PARTICLE_SEQUENCE_HANDLE )
		return false;

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq.GetValue() );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    if ( DecomposeHandle( dwHandle, wIndex, wStamp ) == false )
    {
        ASSERT( 0 );
        return false;
    }
    if ( wIndex >= m_coParticleSequence.storage_size() )
        return false;
    KSequenceHandleInfo& info = m_coParticleSequence.data( wIndex );
    if ( info.m_pSequence != pSeq || info.m_wStamp != wStamp || info.m_eType != SEQUENCELIST_LIVE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        || pSeq->GetDelete() == true
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        )
        return false;
    return true;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	return IsLiveInstanceHandle( pSeq->GetHandle() );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}


bool CKTDGParticleSystem::IsLiveInstanceHandle( CParticleEventSequenceHandle hSeq )
{
	if( INVALID_PARTICLE_SEQUENCE_HANDLE == hSeq )
		return false;

#ifdef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq.GetValue() );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    if ( DecomposeHandle( dwHandle, wIndex, wStamp ) == false )
    {
        ASSERT( 0 );
        return false;
    }
    if ( wIndex >= m_coParticleSequence.storage_size() )
        return false;
    KSequenceHandleInfo& info = m_coParticleSequence.data( wIndex );
    if ( info.m_pSequence == NULL || info.m_wStamp != wStamp || info.m_eType != SEQUENCELIST_LIVE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        || info.m_pSequence->GetDelete() == true
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX        
        )
        return false;
    return true;
#else//X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	map< CParticleEventSequenceHandle, CParticleEventSequence* >::iterator itseq 
		= m_mapInstanceSequences.find( hSeq );
	if( itseq != m_mapInstanceSequences.end() )
	{
		return true;
	}
	else
	{
		return false; 
	}
#endif//X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::GetInstanceSequence( CParticleEventSequenceHandle hSeq
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    , bool bLiveOnly
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    )
{
	if( INVALID_PARTICLE_SEQUENCE_HANDLE == hSeq )
		return NULL;

#ifdef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq.GetValue() );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hSeq );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    if ( DecomposeHandle( dwHandle, wIndex, wStamp ) == false )
    {
        ASSERT( 0 );
        return NULL;
    }
    if ( wIndex >= m_coParticleSequence.storage_size() )
        return NULL;
    KSequenceHandleInfo& info = m_coParticleSequence.data( wIndex );
    CParticleEventSequence* pSeq = info.m_pSequence;
    if ( pSeq == NULL || info.m_wStamp != wStamp || bLiveOnly == true && 
        ( info.m_eType != SEQUENCELIST_LIVE 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            || pSeq->GetDelete() == true
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX   
        )
        )
        return NULL;
    return  pSeq;

#else//X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	map< CParticleEventSequenceHandle, CParticleEventSequence* >::iterator itseq 
		= m_mapInstanceSequences.find( hSeq );
	if( itseq != m_mapInstanceSequences.end() )
	{
		return itseq->second;
	}
	else
	{
		return NULL;
	}
#endif//X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

}



const CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::GetTempletSequencesByName( const wstring wstrName )
{
	if( true == wstrName.empty() )
		return NULL;

	map<wstring, CParticleEventSequence*>::iterator it;
	it = m_TempletSequences.find( wstrName );
	if( it != m_TempletSequences.end() )
	{
		return it->second;
	}
	return NULL;
}
#ifdef X2VIEWER
void CKTDGParticleSystem::AddTempletSequence( wstring name, CParticleEventSequence* pNewSeq )
{
	// 똑같은 이름의 particle sequence templet 이 있다면 지우고 덮어쓴다
	std::map<wstring,CParticleEventSequence*>::iterator it = m_TempletSequences.find( name );
	ASSERT( it == m_TempletSequences.end() );
	if( it != m_TempletSequences.end() )
	{
		SAFE_DELETE_KTDGOBJECT( it->second );
	}
	m_TempletSequences[name] = pNewSeq;

}

void CKTDGParticleSystem::DeleteTempletSequence( wstring name )
{
// 	std::map<wstring,CParticleEventSequence*>::iterator it = m_TempletSequences.find( name );
// 	ASSERT( it == m_TempletSequences.end() );
// 	if( it != m_TempletSequences.end() )
// 	{
// 		SAFE_DELETE_KTDGOBJECT( it->second );
// 	}
	m_TempletSequences.erase( name );

}
#endif X2VIEWER

bool CKTDGParticleSystem::ProcessParticleSystemBlock(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
													std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	bool StartedProcessingSequences = false;
	// move past the opening brace...
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	while (TokenIter->m_Type != CloseBrace)
	{
		CParticleEmitterToken savedtoken;	// not need
		savedtoken = *TokenIter;			// not need

		// the first token here can be a SysNumberProperty, SysAlphaBlendModeProperty, SysVectorProperty,
		// or an EventSequence.
		switch (TokenIter->m_Type) 
		{
			case KeywordEventSequence:
				{
					StartedProcessingSequences = true;
					//CParticleEventSequence *newseq = new CParticleEventSequence( this, NULL );
					CParticleEventSequence* newSeq = CParticleEventSequence::CreateParticleEventSequence( this, NULL );

					if( !ProcessEventSequenceBlock( *newSeq, TokenIter, EndIter ) ) 
					{
						m_TempletSequences.erase( newSeq->GetName() );
					}
				}
				break;

			default: 
				{
					char buf[256]; 
		#ifdef	CONVERSION_VS
					_snprintf_s(buf, sizeof(buf), _countof(buf), "Unexpected \"%s\" in Particle System Block!", TokenIter->m_strValue.c_str());
		#else	CONVERSION_VS
					_snprintf(buf, sizeof(buf), "Unexpected \"%s\" in Particle System Block!", TokenIter->m_strValue.c_str());
		#endif	CONVERSION_VS
					
					throw(&buf[0]);
				}
				break;
		}
	}
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	return(true);
}

bool CKTDGParticleSystem::ProcessEventSequenceBlock(CParticleEventSequence &seq,
												 std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	bool StartedProcessingEvents = false;

	// move past the event sequence keyword...
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	// next token should be the name of the sequence...
	if (TokenIter->m_Type != Quote) throw("Must name particle sequence block!");

	WCHAR wFileName[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, RemoveQuotes(TokenIter->m_strValue).c_str(), -1, wFileName, 255 );
	seq.SetName( wFileName );

	// 똑같은 이름의 particle sequence templet 이 있다면 지우고 덮어쓴다
	std::map<wstring,CParticleEventSequence*>::iterator it = m_TempletSequences.find( wFileName );
	ASSERT( it == m_TempletSequences.end() );
	if( it != m_TempletSequences.end() )
	{
		SAFE_DELETE_KTDGOBJECT( it->second );
#ifdef EFFECT_TOOL
		MessageBox( NULL, wFileName , L"Duplication" , NULL );
#endif //EFFECT_TOOL
	}
#ifdef EFFECT_TOOL
	m_mapTempletSequences[m_strCurrentFileName].push_back(&seq);
#endif //EFFECT_TOOL
	m_TempletSequences[wFileName] = &seq;


	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	// next token should be opening brace...
	if (TokenIter->m_Type != OpenBrace) throw("Expected opening brace for particle sequence block!");
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	while (TokenIter->m_Type != CloseBrace) 
	{

		CParticleEmitterToken savedtoken;
		savedtoken = *TokenIter;	// not need: iterator를 가지고 있어도 상관없을듯

		// the first token here can be a SysNumberProperty, SysAlphaBlendModeProperty, SysVectorProperty,
		// or an EventSequence.
		switch (TokenIter->m_Type) 
		{
			case SeqNumericProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					CMinMax<float> number;
					// the next 2 tokens should be an equals, and a number.
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					CParticleEmitterTokenizer::ProcessNumber(number, TokenIter, EndIter);

					if (savedtoken.IsEmitRate())			{ seq.SetEmitRate(number); }
					else if (savedtoken.IsLifeTime())		{ seq.SetLifetime(number); }
					else if (savedtoken.IsNumParticles())	{ seq.SetMaxParticleNum((int)number.m_Max); }
					else if (savedtoken.IsTriggerCount())	{ seq.SetTriggerCount((int)number.GetRandomNumInRange()); }
					else if (savedtoken.IsTriggerTime())	{ seq.SetTriggerTime(number.GetRandomNumInRange()); }
					else if (savedtoken.IsDrawCount())		{ seq.SetDrawCount((int)number.GetRandomNumInRange()); }
					else if (savedtoken.IsLandPos())		{ seq.SetLandPosition(number.GetRandomNumInRange()); }
					else if (savedtoken.IsLayer())			{ seq.SetLayer( (int)number.m_Max ); }
					else if (savedtoken.IsScrew())			{ seq.SetScrewValue( (int)number.m_Max ); }
					else if (savedtoken.IsScrewRotateSpeed()) { seq.SetScrewRotateSpeed( (int)number.m_Max ); }
#ifdef PARTICLE_SPHERICAL_EMIT_TEST
					else if (savedtoken.IsSphericalEmitRadius())	{ seq.SetSphericalEmitRadius(number); }
					else if (savedtoken.IsSphericalEmitAzimuth())	{ number.m_Min *= D3DX_PI/180.f; number.m_Max *= D3DX_PI/180.f; seq.SetSphericalEmitAzimuth(number); }
					else if (savedtoken.IsSpehricalEmitPolar())		{ number.m_Min *= D3DX_PI/180.f; number.m_Max *= D3DX_PI/180.f; seq.SetSphericalEmitPolar(number); }
#endif PARTICLE_SPHERICAL_EMIT_TEST
					else 
					{
						throw("Unknown sequence numeric property!");
					}

#ifdef EFFECT_TOOL
					/// 스크립트에서는 범위형으로 설정하고, 멤버변수에서는 확정값을 설정하는 부분때문에 더미값 추가.
					if (savedtoken.IsTriggerCount())	{ seq.SetTriggerCount_Origin(number); }
					else if (savedtoken.IsTriggerTime())	{ seq.SetTriggerTime_Origin(number); }
					else if (savedtoken.IsDrawCount())		{ seq.SetDrawCount_Origin(number); }
					else if (savedtoken.IsLandPos())		{ seq.SetLandPosition_Origin(number); }
#endif //EFFECT_TOOL
				}
				break;

			case SeqVectorProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					CMinMax<D3DXVECTOR3> v;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					CParticleEmitterTokenizer::ProcessVector(v, TokenIter, EndIter);

					if (savedtoken.IsEmitRadius())			{ seq.SetEmitRadius(v); }
					else if (savedtoken.IsEmitRadiusMin())	{ seq.SetEmitRadiusMin(v); }
					else if (savedtoken.IsGravity())		{ seq.SetGravity(v); }
					else if (savedtoken.IsBlackHolePos())   { seq.SetBlackHolePosition(v.GetRandomNumInRange()); }
					else if (savedtoken.IsLightPos())		{ seq.SetLightPos(v.GetRandomNumInRange()); }
					else if (savedtoken.IsAddRotateRel())	{ seq.SetAddRotateRel(v.GetRandomNumInRange()); }
#ifdef PARTICLE_SPHERICAL_EMIT_TEST
					else if (savedtoken.IsSpehricalEmitRotation())	{ D3DXVECTOR3 vRotation = v.GetRandomNumInRange(); vRotation *= D3DX_PI/180.f; seq.SetSphericalEmitRotation( vRotation ); }
#endif PARTICLE_SPHERICAL_EMIT_TEST
					else 
					{
						throw("Unknown sequence vector property!");
					}
#ifdef EFFECT_TOOL
					/// 스크립트에서는 범위형으로 설정하고, 멤버변수에서는 확정값을 설정하는 부분때문에 더미값 추가.
					if (savedtoken.IsBlackHolePos())   { seq.SetBlackHolePosition_Origin(v); }
					else if (savedtoken.IsLightPos())		{ seq.SetLightPos_Origin(v); }
					else if (savedtoken.IsAddRotateRel())	{ seq.SetAddRotateRel_Origin(v); }
					else if (savedtoken.IsSpehricalEmitRotation())	{ seq.SetvSphericalEmitRotation_Origin( v ); }
#endif //EFFECT_TOOL
				}
				break;

			case SeqAlphaBlendModeProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					int alphablendmode;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					CParticleEmitterTokenizer::ProcessAlphaBlendMode(alphablendmode, TokenIter, EndIter);

					if (savedtoken.IsSrcBlendMode())       { seq.SetSrcBlendMode(alphablendmode); }
					else if (savedtoken.IsDestBlendMode()) { seq.SetDestBlendMode(alphablendmode); }
					else 
					{
						throw("Unknown sequence alpha blending mode property!");
					}
				}
				break;

//{{ 09.05.06 태완 : Cull mode 지정 가능하게
			case SeqCullModeProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					int cullmode;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					CParticleEmitterTokenizer::ProcessCullMode(cullmode, TokenIter, EndIter);

					if (savedtoken.IsCullMode())       { seq.SetCullMode(cullmode); }
					else 
					{
						throw("Unknown sequence cull mode property!");
					}
				}
				break;
//}} 09.05.06 태완 : Cull mode 지정 가능하게

			case SeqParticleTypeProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					PARTICLE_TYPE particleType;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					CParticleEmitterTokenizer::ProcessParticleType(particleType, TokenIter, EndIter);

					if (savedtoken.IsParticleType())       { seq.SetParticleType(particleType); }
					else 
					{
						throw("Unknown sequence particle type property!");
					}
				}
				break;

			case SeqMeshRenderTypeProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					CKTDGXRenderer::RENDER_TYPE renderType;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					CParticleEmitterTokenizer::ProcessMeshRenderType(renderType, TokenIter, EndIter);

					if (savedtoken.IsMeshRenderType())       { seq.SetMeshRenderType(renderType); }
					else 
					{
						throw("Unknown sequence particle type property!");
					}
				}
				break;

			case SeqBillboardTypeProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					CKTDGMatrix::BILLBOARD_TYPE billboardType;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					CParticleEmitterTokenizer::ProcessBillboardType(billboardType, TokenIter, EndIter);

					if (savedtoken.IsBillboardType())       { seq.SetBillBoardType(billboardType); }
					else 
					{
						throw("Unknown sequence particle type property!");
					}
				}
				break;

			case SeqBoolProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					bool bValue;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					CParticleEmitterTokenizer::ProcessBool(bValue, TokenIter, EndIter);

					if (savedtoken.IsUseLand())					{ seq.SetUseLand(bValue); }
					else if (savedtoken.IsTrace())				{ seq.SetTrace(bValue); }
					else if (savedtoken.IsZEnable())			{ seq.SetZEnable(bValue); }
					else if (savedtoken.IsZWriteEnable())		{ seq.SetZWriteEnable(bValue); }
					else if (savedtoken.IsResolutionConvert())	{ seq.SetResolutionConvert(bValue); }
					else if (savedtoken.IsForceLayer())			{ seq.SetForceLayer(bValue); }
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
					else if (savedtoken.IsApplyUnitScale())		{ seq.SetApplyUnitScale(bValue); }
#endif
#ifdef ADD_ALPHATESTENABLE
					else if (savedtoken.IsAlphaTest())			{ seq.SetAlphTest(bValue); }
#endif
					else 
					{
						throw("Unknown sequence particle type property!");
					}
				}
				break;

			case SeqStringProp:
				{
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					wstring str;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					RemoveQuotes(TokenIter->m_strValue);
					CParticleEmitterTokenizer::ProcessString(str, TokenIter, EndIter);

					if (savedtoken.IsMesh())					{ seq.SetMeshName(str.c_str()); }
					else if( savedtoken.IsSkinMesh())			{ seq.SetMeshName(str.c_str()); }
					else if (savedtoken.IsChangeTexXET())		{ seq.SetChangeTexXETName(str.c_str()); }
					else if (savedtoken.IsMultiTexXET())		{ seq.SetMultiTexXETName(str.c_str()); }
					else if (savedtoken.IsAniXET())				{ seq.SetAniXETName(str.c_str()); }
					else if (savedtoken.IsAniName())			{ seq.SetAniName(str.c_str()); }
					else if (savedtoken.IsTraceParticleList())	{ seq.AddTraceSeqName(str.c_str()); }
					else if (savedtoken.IsFinalParticleList())	{ seq.AddFinalSeqName(str.c_str()); }
					else 
					{
						throw("Unknown sequence particle type property!");
					}
				}
				break;

				/*
			case KeywordTexture:
				{
					// TODO
					if (StartedProcessingEvents) 
						throw("Cannot specify any sequence properties after specifying events.");
					CMinMax<D3DXVECTOR3> v;
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
					if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 

					if (TokenIter->m_Type != Quote) throw("Expected filename after texture sequence property.");

					WCHAR wFileName[255] = {0,};
					MultiByteToWideChar( CP_ACP, 0, RemoveQuotes(TokenIter->m_strValue).c_str(), -1, wFileName, 255);
					seq.SetTextureName( wFileName );
					CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
				}
				break;
				*/

			case KeywordFade: 
			case KeywordAt: 
			case KeywordFrom:
			case KeywordTo: 
			case KeywordInitial: 
			case KeywordFinal:
				{
					StartedProcessingEvents = true;

					bool IsFade = false;
					CMinMax<float> TimeRange;

					// parse the time range section of the event line
					CParticleEmitterTokenizer::ProcessTime(TimeRange, IsFade, 0, seq.GetLifeTime().m_Max, TokenIter, EndIter);

					if (TokenIter->m_Type != ParticleNumericProp &&  
						TokenIter->m_Type != ParticleVectorProp &&
						TokenIter->m_Type != ParticleVector2Prop &&
						TokenIter->m_Type != ParticleColorProp &&
						TokenIter->m_Type != ParticleStringProp ) 
					{
						WCHAR* pErrString = new WCHAR[255];

						StringCchPrintf( pErrString, 255,  L"%s, Expecting particle property after time specifier!", seq.GetName() );
						//wsprintf( pErrString, L"%s, Expecting particle property after time specifier!", seq.GetName() );

						throw( pErrString );
					}

					CParticleEvent * NewEvent = NULL;
					try 
					{
						// create the appropriate event
						NewEvent = EventFactory(TokenIter->m_strValue);

						if (!NewEvent) throw("Unknown event type, or there was an error creating this event.");

						// let the event parse the rest of its properties from the token stream.
						if (IsFade && !NewEvent->FadeAllowed()) throw("Fading is not supported on this event.");
						NewEvent->ProcessTokenStream(TokenIter, EndIter);
						NewEvent->SetActualTime(TimeRange);
						NewEvent->SetFade(IsFade);
						seq.m_EventList.push_back(NewEvent);
					}
					catch(char *e) 
					{
						SAFE_DELETE(NewEvent);
						throw(e);
					}
					catch(...) 
					{
						SAFE_DELETE(NewEvent);
						throw("Unhandled exception creating event!");
					}
				}
				break;

			default: 
				{
					WCHAR* pErrString = new WCHAR[255];
					wstring err;
					ConvertCharToWCHAR( err, TokenIter->m_strValue.c_str() );

					StringCchPrintf( pErrString, 255, L"%s, Unexpected \"%s\" in Sequence Block!", seq.GetName(), err.c_str() );
					//wsprintf( pErrString, L"%s, Unexpected \"%s\" in Sequence Block!", seq.GetName(), err.c_str() );
					throw( pErrString );
				}
				break;
		}
	}

	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 

	return(true);
}

CKTDGParticleSystem::CParticleEvent* CKTDGParticleSystem::EventFactory( std::string EventName )
{
	CParticleEvent *event = NULL;

	if (EventName.compare("SIZE") == 0)					event = new CParticleEvent_Size();
	else if (EventName.compare("SCALE") == 0)			event = new CParticleEvent_Scale();
	else if (EventName.compare("EVENTTIMER") == 0)		event = new CParticleEvent_EventTimer();
	else if (EventName.compare("COLOR") == 0)			event = new CParticleEvent_Color();
	else if (EventName.compare("VELOCITY") == 0)		event = new CParticleEvent_Velocity();
	else if (EventName.compare("POSITION") == 0)		event = new CParticleEvent_Position();
	else if (EventName.compare("ROTATE") == 0)			event = new CParticleEvent_Rotate();
	else if (EventName.compare("DIRSPEED") == 0)		event = new CParticleEvent_DirSpeed();
	else if (EventName.compare("BLACKHOLETIME") == 0)	event = new CParticleEvent_BlackHole();
	else if (EventName.compare("RESETCRASH") == 0)		event = new CParticleEvent_ResetCrash();
	else if (EventName.compare("CRASH") == 0)			event = new CParticleEvent_Crash();
	else if (EventName.compare("TEXTURE") == 0)			event = new CParticleEvent_Texture();
	else if (EventName.compare("TEX0UV") == 0)			event = new CParticleEvent_Tex0UV();
	else if (EventName.compare("TEX1UV") == 0)			event = new CParticleEvent_Tex1UV();
	else if (EventName.compare("TEX2UV") == 0)			event = new CParticleEvent_Tex2UV();
#ifdef PARTICLE_STRETCH_TEST
	else if (EventName.compare("STRETCH") == 0)			event = new CParticleEvent_Stretch();
#endif PARTICLE_STRETCH_TEST

	return(event);
}



///////////////////////////////////////////////////////////////////////////////
// Particle Event
///////////////////////////////////////////////////////////////////////////////

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(CMinMax<float> &prop,
											std::vector<CParticleEmitterToken>::iterator &TokenIter, 
											std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// next token should be =
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	if (TokenIter->m_Type != Equals) throw("Expecting = after property.");

	// next token should be a number
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	CParticleEmitterTokenizer::ProcessNumber(prop, TokenIter, EndIter);
}

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(CMinMax<D3DXVECTOR3> &prop,
											std::vector<CParticleEmitterToken>::iterator &TokenIter, 
											std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// next token should be =
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	if (TokenIter->m_Type != Equals) throw("Expecting = after property.");

	// next token should be a number
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	CParticleEmitterTokenizer::ProcessVector(prop, TokenIter, EndIter);
}

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(CMinMax<D3DXVECTOR2> &prop,
											std::vector<CParticleEmitterToken>::iterator &TokenIter, 
											std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// next token should be =
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	if (TokenIter->m_Type != Equals) throw("Expecting = after property.");

	// next token should be a number
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	CParticleEmitterTokenizer::ProcessVector2(prop, TokenIter, EndIter);
}

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(CMinMax<D3DXCOLOR> &prop,
											std::vector<CParticleEmitterToken>::iterator &TokenIter, 
											std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// next token should be =
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	if (TokenIter->m_Type != Equals) throw("Expecting = after property.");

	// next token should be a number
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	CParticleEmitterTokenizer::ProcessColor(prop, TokenIter, EndIter);
}

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(wstring &prop,
																 std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
	if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 

	if (TokenIter->m_Type != Quote) throw("Expected filename after texture sequence property.");

	WCHAR wFileName[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, RemoveQuotes(TokenIter->m_strValue).c_str(), -1, wFileName, 255);

	prop = wFileName;
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
}

bool CKTDGParticleSystem::CParticleEvent_Color::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
											  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("COLOR") 	!= 0) throw("Expecting Color!");
	ProcessPropEqualsValue(m_Color, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Color::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade() ) 
		part.SetColor( m_Color.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( fOld <= m_ActualTime.m_Min )
			part.SetColorFinal( m_Color.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( m_ActualTime.m_Max <= fNew )
			part.SetColor( part.GetColorFinal() );
		else
		{
			// fade
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );
			D3DXCOLOR	remainColor = part.GetColorFinal() - part.GetColor();
			part.SetColor( part.GetColor() + remainColor * fRatio );
		}
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
		part.SetColor( m_Color.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
			part.SetColorFinal( m_Color.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( ( m_ActualTime.m_Max - fEventTimerFix ) < fElapsedTime )
			part.SetColor( part.GetColorFinal() );
		else
		{
			// fade
			D3DXCOLOR	remainColor = part.GetColorFinal() - part.GetColor();
			part.SetColor( part.GetColor() + remainColor * ( fElapsedTime / fRemainTime) );
		}
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_Size::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("SIZE") != 0) throw("Expecting Size!");
	ProcessPropEqualsValue(m_Size, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Size::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade() ) 
		part.SetSize( m_Size.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( fOld <= m_ActualTime.m_Min )
			part.SetSizeFinal( m_Size.GetRandomNumInRange() );
		
		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( m_ActualTime.m_Max <= fNew )
			part.SetSize( part.GetSizeFinal() );
		else
		{
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );
			// fade
			D3DXVECTOR3	remainSize	= part.GetSizeFinal() - part.GetSize();
			part.SetSize( part.GetSize() + remainSize * fRatio );
		}
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
		part.SetSize( m_Size.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
			part.SetSizeFinal( m_Size.GetRandomNumInRange() );
		
		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
			part.SetSize( part.GetSizeFinal() );
		else
		{
			// fade
			D3DXVECTOR3	remainSize	= part.GetSizeFinal() - part.GetSize();
			part.SetSize( part.GetSize() + remainSize * ( fElapsedTime / fRemainTime) );
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_Scale::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("SCALE") != 0) throw("Expecting Scale!");
	ProcessPropEqualsValue( m_Scale, TokenIter, EndIter );
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Scale::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade() ) 
	{
		part.SetSize( part.GetSize() * m_Scale.GetRandomNumInRange() );
	}
	else	// fade 이면서, fRemainTime이 0이 아니면
	{
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( fOld <= m_ActualTime.m_Min )
		{
			part.SetSizeFinal( part.GetSize() * m_Scale.GetRandomNumInRange() );
		}

		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( m_ActualTime.m_Max <= fNew )
		{
			part.SetSize( part.GetSizeFinal() );
		}
		else
		{
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );
			// fade
			D3DXVECTOR3	remainSize	= part.GetSizeFinal() - part.GetSize();
			part.SetSize( part.GetSize() + remainSize * fRatio );
		}
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
	{
		part.SetSize( part.GetSize() * m_Scale.GetRandomNumInRange() );
	}
	else	// fade 이면서, fRemainTime이 0이 아니면
	{
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
		{
			part.SetSizeFinal( part.GetSize() * m_Scale.GetRandomNumInRange() );
		}

		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
		{
			part.SetSize( part.GetSizeFinal() );
		}
		else
		{
			// fade
			D3DXVECTOR3	remainSize	= part.GetSizeFinal() - part.GetSize();
			part.SetSize( part.GetSize() + remainSize * ( fElapsedTime / fRemainTime ) );
		}
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}





bool CKTDGParticleSystem::CParticleEvent_Velocity::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("VELOCITY") != 0) throw("Expecting Velocity!");
	ProcessPropEqualsValue(m_Velocity, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Velocity::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
    part.ResetVelocityToAccumPos();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX

	if( !IsFade() ) 
    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
        part.UpdateVelocityAccumPosAndEventTimer( m_ActualTime.m_Min );
        part.SetVelocity( m_Velocity.GetRandomNumInRange(), false );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
        if ( fOld <= m_ActualTime.m_Min )
            part.AddVelocityToAccumPos( m_ActualTime.m_Min - fOld );
		part.SetVelocity( m_Velocity.GetRandomNumInRange(), false );
        if ( m_ActualTime.m_Min <= fNew )
            part.AddVelocityToAccumPos( fNew - m_ActualTime.m_Min );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
    }
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		

		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( fOld <= m_ActualTime.m_Min )
        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
            part.UpdateVelocityAccumPosAndEventTimer( m_ActualTime.m_Min );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
            part.AddVelocityToAccumPos( m_ActualTime.m_Min - fOld );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
			part.SetVelocityFinal( m_Velocity.GetRandomNumInRange() );
        }

        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
            float   fOld2 = __max( part.GetEventTimerVelocity(), m_ActualTime.m_Min );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
            float   fOld2 = __max( fOld, m_ActualTime.m_Min );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
            float   fNew2 = __min( fNew, m_ActualTime.m_Max );
            if ( fOld2 < fNew2 )
            {
                float fRatio = ( fNew2 - fOld2 ) / ( m_ActualTime.m_Max - fOld2 );
                fRatio = __max( 0.f, __min( 1.f, fRatio ) );
			    // fade
			    D3DXVECTOR3	remainVelocity	= part.GetVelocityFinal() - part.GetVelocity();
			    part.SetVelocity( part.GetVelocity() + remainVelocity * ( fRatio * 0.5f ), false );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                part.UpdateVelocityAccumPosAndEventTimer( fNew2 );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                part.AddVelocityToAccumPos( fNew2 - fOld2 );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
			    part.SetVelocity( part.GetVelocity() + remainVelocity * fRatio, false );
            }
		}

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
        if ( m_ActualTime.m_Max <= fNew )
        {
			part.SetVelocity( part.GetVelocityFinal(), false );
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
            part.AddVelocityToAccumPos( fNew - m_ActualTime.m_Max );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
        }
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
		part.SetVelocity( m_Velocity.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
			part.SetVelocityFinal( m_Velocity.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
			part.SetVelocity( part.GetVelocityFinal() );
		else
		{
			// fade
			D3DXVECTOR3	remainVelocity	= part.GetVelocityFinal() - part.GetVelocity();
			part.SetVelocity( part.GetVelocity() + remainVelocity * (fElapsedTime / fRemainTime) );
		}
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_Position::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																	  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("POSITION") != 0) throw("Expecting position!");
	ProcessPropEqualsValue(m_Position, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Position::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade() ) 
	{
		part.SetPos( m_Position.GetRandomNumInRange() );
        part.SetPositionGap( D3DXVECTOR3(0,0,0) );
	}
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( fOld <= m_ActualTime.m_Min )
		{
			part.SetPositionFinal( m_Position.GetRandomNumInRange() );
		}

		// fade 이면서, fRemainTime이 0이 아니면
		if( m_ActualTime.m_Max <= fNew )
		{
			part.SetPos( m_Position.GetRandomNumInRange() );
			part.SetPositionGap( D3DXVECTOR3(0,0,0) );
		}
		else
		{
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );
			// fade
			D3DXVECTOR3	remainPositionGap	= part.GetPositionFinal() - part.GetPos();
			// not need: 왜 직접 part.m_vPos에 더해주기 보다는 m_vPositionGap을 이용하는 것일까?
			part.SetPositionGap( remainPositionGap * fRatio );
		}
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
	{
		part.SetPos( m_Position.GetRandomNumInRange() );
        part.SetPositionGap( D3DXVECTOR3(0,0,0) );
	}
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
		{
			part.SetPositionFinal( m_Position.GetRandomNumInRange() );
		}

		// fade 이면서, fRemainTime이 0이 아니면
		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
		{
			part.SetPos( m_Position.GetRandomNumInRange() );
			part.SetPositionGap( D3DXVECTOR3(0,0,0) );
		}
		else
		{
			// fade
			D3DXVECTOR3	remainPositionGap	= part.GetPositionFinal() - part.GetPos();
			// not need: 왜 직접 part.m_vPos에 더해주기 보다는 m_vPositionGap을 이용하는 것일까?
			part.SetPositionGap( (remainPositionGap / fRemainTime) * fElapsedTime );
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_Tex0UV::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																	  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("TEX0UV") != 0) throw("Expecting Velocity!");
	ProcessPropEqualsValue(m_TexUV, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Tex0UV::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade() ) 
		part.SetTexStage0UV( m_TexUV.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( fOld <= m_ActualTime.m_Min )
			part.SetTexStage0UVFinal( m_TexUV.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( m_ActualTime.m_Max <= fNew )
			part.SetTexStage0UV( part.GetTexStage0UVFinal() );
		else
		{
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );

			// fade
			D3DXVECTOR2	remainTexUV	= part.GetTexStage0UVFinal() - part.GetTexStage0UV();
			part.SetTexStage0UV( part.GetTexStage0UV() + remainTexUV * fRatio );
		}
	}


#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
		part.SetTexStage0UV( m_TexUV.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
			part.SetTexStage0UVFinal( m_TexUV.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
			part.SetTexStage0UV( part.GetTexStage0UVFinal() );
		else
		{
			// fade
			D3DXVECTOR2	remainTexUV	= part.GetTexStage0UVFinal() - part.GetTexStage0UV();
			part.SetTexStage0UV( part.GetTexStage0UV() + remainTexUV * ( fElapsedTime / fRemainTime ) );
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_Tex1UV::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("TEX1UV") != 0) throw("Expecting Velocity!");
	ProcessPropEqualsValue(m_TexUV, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Tex1UV::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade() ) 
		part.SetTexStage1UV( m_TexUV.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( fOld <= m_ActualTime.m_Min )
			part.SetTexStage1UVFinal( m_TexUV.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( m_ActualTime.m_Max <= fNew )
			part.SetTexStage1UV( part.GetTexStage1UVFinal() );
		else
		{
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );

			// fade
			D3DXVECTOR2	remainTexUV	= part.GetTexStage1UVFinal() - part.GetTexStage1UV();
			part.SetTexStage1UV( part.GetTexStage1UV() + remainTexUV * fRatio );
		}
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
		part.SetTexStage1UV( m_TexUV.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
			part.SetTexStage1UVFinal( m_TexUV.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
			part.SetTexStage1UV( part.GetTexStage1UVFinal() );
		else
		{
			// fade
			D3DXVECTOR2	remainTexUV	= part.GetTexStage1UVFinal() - part.GetTexStage1UV();
			part.SetTexStage1UV( part.GetTexStage1UV() + remainTexUV * ( fElapsedTime / fRemainTime) );
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_Tex2UV::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("TEX2UV") != 0) throw("Expecting Velocity!");
	ProcessPropEqualsValue(m_TexUV, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Tex2UV::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade() ) 
		part.SetTexStage2UV( m_TexUV.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( fOld <= m_ActualTime.m_Min )
			part.SetTexStage2UVFinal( m_TexUV.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( m_ActualTime.m_Max <= fNew )
			part.SetTexStage2UV( part.GetTexStage2UVFinal() );
		else
		{
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );

			// fade
			D3DXVECTOR2	remainTexUV	= part.GetTexStage2UVFinal() - part.GetTexStage2UV();
			part.SetTexStage2UV( part.GetTexStage2UV() + remainTexUV * fRatio );
		}
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
		part.SetTexStage2UV( m_TexUV.GetRandomNumInRange() );
	else	// fade 이면서, fRemainTime이 0이 아니면
	{		
		// 파티클 생성 초반에 final 값을 미리 정해주고
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
			part.SetTexStage2UVFinal( m_TexUV.GetRandomNumInRange() );

		// 파티클이 시간에 의해 종료되기 직전에 final 값을 넣준다.
		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
			part.SetTexStage2UV( part.GetTexStage2UVFinal() );
		else
		{
			// fade
			D3DXVECTOR2	remainTexUV	= part.GetTexStage2UVFinal() - part.GetTexStage2UV();
			part.SetTexStage2UV( part.GetTexStage2UV() + remainTexUV * ( fElapsedTime / fRemainTime) );
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_Rotate::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("ROTATE") != 0) throw("Expecting ROTATE!");
	ProcessPropEqualsValue(m_Rotate, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Rotate::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade()  ) 
	{
//#ifdef LOCAL_ROTATE_EFFECT_P
		part.SetRotateLocal( m_Rotate.GetRandomNumInRange() );
//#else
//		part.m_vRotate = m_Rotate.GetRandomNumInRange();
//#endif
	}
	else
	{
		if( fOld <= m_ActualTime.m_Min )
			part.SetRotateFinal( m_Rotate.GetRandomNumInRange() );

		if( m_ActualTime.m_Max <= fNew )
		{
//#ifdef LOCAL_ROTATE_EFFECT_P
			part.SetRotateLocal( part.GetRotateFinal() );
//#else
//			part.m_vRotate = part.GetRotateFinal();
//#endif
			
		}
		else
		{
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );

//#ifdef LOCAL_ROTATE_EFFECT_P
			D3DXVECTOR3 remainRotate = part.GetRotateFinal() - part.GetRotateLocal();
			part.SetRotateLocal( part.GetRotateLocal() + remainRotate * fRatio );
//#else
//			D3DXVECTOR3 remainRotate = part.GetRotateFinal() - part.m_vRotate;
//			part.m_vRotate += (remainRotate / fRemainTime) * fElapsedTime;
//#endif

			
		}
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;	

	if( !IsFade() || fRemainTime == 0 ) 
	{
//#ifdef LOCAL_ROTATE_EFFECT_P
		part.SetRotateLocal( m_Rotate.GetRandomNumInRange() );
//#else
//		part.m_vRotate = m_Rotate.GetRandomNumInRange();
//#endif
	}
	else
	{
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
			part.SetRotateFinal( m_Rotate.GetRandomNumInRange() );

		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
		{
//#ifdef LOCAL_ROTATE_EFFECT_P
			part.SetRotateLocal( part.GetRotateFinal() );
//#else
//			part.m_vRotate = part.GetRotateFinal();
//#endif
			
		}
		else
		{
//#ifdef LOCAL_ROTATE_EFFECT_P
			D3DXVECTOR3 remainRotate = part.GetRotateFinal() - part.GetRotateLocal();
			part.SetRotateLocal( part.GetRotateLocal() + remainRotate * ( fElapsedTime / fRemainTime ) );
//#else
//			D3DXVECTOR3 remainRotate = part.GetRotateFinal() - part.m_vRotate;
//			part.m_vRotate += (remainRotate / fRemainTime) * fElapsedTime;
//#endif

			
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_DirSpeed::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																	 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("DIRSPEED") != 0) throw("Expecting VelocityZ!");
	ProcessPropEqualsValue(m_DirSpeed, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_DirSpeed::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

	if( !IsFade() ) 
		part.SetDirSpeed( m_DirSpeed.GetRandomNumInRange() );
	else
	{
		if( fOld <= m_ActualTime.m_Min )
			part.SetDirSpeedFinal( m_DirSpeed.GetRandomNumInRange() );

		if( m_ActualTime.m_Max <= fNew )
			part.SetDirSpeed( part.GetDirSpeedFinal() );
		else
		{
            float fRatio = 0.f;
            if ( fOld < m_ActualTime.m_Min )
                fOld = m_ActualTime.m_Min;
            fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
            fRatio = __max( 0.f, __min( 1.f, fRatio ) );

			float fRemainDirSpeed	= part.GetDirSpeedFinal() - part.GetDirSpeed();
			part.SetDirSpeed( part.GetDirSpeed() + fRemainDirSpeed * fRatio );
		}
	}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

	if( !IsFade() || fRemainTime == 0 ) 
		part.SetDirSpeed( m_DirSpeed.GetRandomNumInRange() );
	else
	{
		if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
			part.SetDirSpeedFinal( m_DirSpeed.GetRandomNumInRange() );

		if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
			part.SetDirSpeed( part.GetDirSpeedFinal() );
		else
		{
			float fRemainDirSpeed	= part.GetDirSpeedFinal() - part.GetDirSpeed();
			part.SetDirSpeed( part.GetDirSpeed() + (fRemainDirSpeed / fRemainTime) * fElapsedTime );
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
}

bool CKTDGParticleSystem::CParticleEvent_BlackHole::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												   std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("BLACKHOLETIME") != 0) throw("Expecting BLACKHOLETIME!");
	ProcessPropEqualsValue(m_BlackHole, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_BlackHole::OnFrameMove( CParticle &part, float fElapsedTime )
{
	part.SetBlackHoleTime( m_BlackHole.GetRandomNumInRange() );
}


bool CKTDGParticleSystem::CParticleEvent_Crash::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																   std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("CRASH") != 0) throw("Expecting Crash!");
	ProcessPropEqualsValue(m_vCrash, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Crash::OnFrameMove( CParticle &part, float fElapsedTime )
{
	if( part.GetNowResetCrash() < part.GetResetCrash() )
	{
		return;
	}
	else
	{
		part.SetNowResetCrash( 0.0f );
	}

	if( m_ActualTime.m_Min > part.GetEventTimer() || m_ActualTime.m_Max < part.GetEventTimer() ) 
		part.SetCrash( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	else
	{
		part.SetCrash( m_vCrash.GetRandomNumInRange() );
	}
}

bool CKTDGParticleSystem::CParticleEvent_ResetCrash::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																   std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("RESETCRASH") != 0) throw("Expecting RESETCRASH!");
	ProcessPropEqualsValue(m_ResetCrash, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_ResetCrash::OnFrameMove( CParticle &part, float fElapsedTime )
{
	part.SetResetCrash( m_ResetCrash.GetRandomNumInRange() );
}

bool CKTDGParticleSystem::CParticleEvent_Texture::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
											 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("TEXTURE") != 0) throw("Expecting Texture!");
	ProcessPropEqualsValue(m_TexName, TokenIter, EndIter);

	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Texture::OnFrameMove( CParticle &part, float fElapsedTime )
{
	part.SetTextureID( m_TextureID );
}

bool CKTDGParticleSystem::CParticleEvent_EventTimer::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																		std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.compare("EVENTTIMER") != 0) 
		throw("Expecting EventTimer!");
	ProcessPropEqualsValue(m_EventTimer, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_EventTimer::OnFrameMove( CParticle &part, float fElapsedTime )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
    part.UpdateVelocityAccumPosAndEventTimer( m_ActualTime.m_Min );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
	part.SetEventTimer( m_EventTimer.GetRandomNumInRange() );
}






#ifdef PARTICLE_STRETCH_TEST
	bool CKTDGParticleSystem::CParticleEvent_Stretch::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
																		std::vector<CParticleEmitterToken>::iterator &EndIter)
	{
		if (TokenIter->m_strValue.compare("STRETCH") != 0) throw("Expecting Stretch!");
		ProcessPropEqualsValue(m_StretchScale, TokenIter, EndIter);
		return(true);
	}

	void CKTDGParticleSystem::CParticleEvent_Stretch::OnFrameMove( CParticle &part, float fElapsedTime )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

		if( !IsFade() ) 
			part.SetStretchScale( m_StretchScale.GetRandomNumInRange() );
		else
		{
		    if( fOld <= m_ActualTime.m_Min )
				part.SetStretchScaleFinal( m_StretchScale.GetRandomNumInRange() );

		    if( m_ActualTime.m_Max <= fNew )
				part.SetStretchScale( part.GetStretchScaleFinal() );
			else
			{
                float fRatio = 0.f;
                if ( fOld < m_ActualTime.m_Min )
                    fOld = m_ActualTime.m_Min;
                fRatio = ( fNew - fOld ) / ( m_ActualTime.m_Max - fOld );
                fRatio = __max( 0.f, __min( 1.f, fRatio ) );

				float fRemainStretchScale	= part.GetStretchScaleFinal() - part.GetStretchScale();
				part.SetStretchScale( part.GetStretchScale() + fRemainStretchScale * fRatio );
			}
		}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

		float fRemainTime = m_ActualTime.m_Max - fEventTimerFix;

		if( !IsFade() || fRemainTime == 0 ) 
			part.SetStretchScale( m_StretchScale.GetRandomNumInRange() );
		else
		{
			if( (fEventTimerFix - m_ActualTime.m_Min) <= fElapsedTime )
				part.SetStretchScaleFinal( m_StretchScale.GetRandomNumInRange() );

			if( (m_ActualTime.m_Max - fEventTimerFix) < fElapsedTime )
				part.SetStretchScale( part.GetStretchScaleFinal() );
			else
			{
				float fRemainStretchScale	= part.GetStretchScaleFinal() - part.GetStretchScale();
				part.SetStretchScale( part.GetStretchScale() + (fRemainStretchScale / fRemainTime) * fElapsedTime );
			}
		}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
	}
#endif PARTICLE_STRETCH_TEST





///////////////////////////////////////////////////////////////////////////////
// Particle Event Sequence
///////////////////////////////////////////////////////////////////////////////
CKTDGParticleSystem::CParticleEventSequence::CParticleEventSequence( CKTDGParticleSystem* pParticleSystem, CParticleEventSequence* pTempletSequence )
: m_Handle( INVALID_PARTICLE_SEQUENCE_HANDLE )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_iParticleList( -1 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
{
	SetAlphaObject( true );

	m_pParticleSystem	= pParticleSystem;
	m_pTempletSequence	= pTempletSequence;
	m_Priority			= 0;
	m_ParticleType		= PT_3D_PLANE;
	m_pMathMatrix		= new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	INIT_VECTOR3( m_AxisAngleDegree, 0.0f, 0.0f, 0.0f );

	m_BillBoardType		= CKTDGMatrix::BT_ALL;

	m_bZEnable			= true;
	m_bZWriteEnable		= true;	


#ifdef SKINMESH_PARTICLE_TEST
	
	m_pXSkinMesh = NULL;

#endif SKINMESH_PARTICLE_TEST



	m_vSphericalEmitRotation = D3DXVECTOR3(0, 0, 0 );

	m_MeshName			= L"";
	m_ChangeTexXETName	= L"";
	m_MultiTexXETName	= L"";
	m_AniXETName		= L"";
	m_AniName			= L"";


	m_pXMesh			= NULL;
	m_pChangeTexXET		= NULL;
	m_pMultiTexXET		= NULL;
	m_pAniXET			= NULL;
	m_pAniData			= NULL;

	INIT_VECTOR3( m_LightPos, 0.0f, 0.0f, 0.0f );
	m_MeshRenderType	= CKTDGXRenderer::RT_REAL_COLOR;

	m_bResolutionConvert	= false;
	INIT_VECTOR3( m_Position,			0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_OldPosition,		0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_BlackHolePosition,	0.0f, 0.0f, 0.0f );

	m_ScrewValue		= -1;

	m_fLandPosition		= 0.0f;
	m_bUseLand			= false;
	m_Lifetime.m_Min	= 0.0f;
	m_Lifetime.m_Max	= 0.0f;
	m_EmitRate.m_Min	= 0.0f;
	m_EmitRate.m_Max	= 0.0f;

	INIT_VECTOR3( m_vEmitRadius.m_Min,		0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_vEmitRadius.m_Max,		0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_vEmitRadiusMin.m_Min,	0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_vEmitRadiusMin.m_Max,	0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_vGravity.m_Min,			0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_vGravity.m_Max,			0.0f, 0.0f, 0.0f );

	m_iMaxParticleNum	= 0;
	m_Trace				= false;
//{{ robobeg : 2008-10-28
	//m_bShow				= true;
//}} robobeg : 2008-10-28

	m_iSrcBlendMode		= D3DBLEND_SRCALPHA;
	m_iDestBlendMode	= D3DBLEND_DESTALPHA;

	// 09.05.06 태완 : Cull mode 지정 가능하게
	m_iCullMode			= D3DCULL_NONE;

	m_fNumNewPartsExcess = 0.0f;

	m_TriggerTime		= -1.0f;
	m_TriggerCount		= -1;
	m_Time				= 0.0f;
	m_fElapsedTime		= 0.0f;
	m_Count				= 0;
	m_DrawCount			= 1;

	m_bUseLookPoint		= false;

	INIT_VECTOR3( m_LookPoint,		0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_AddRotate,		0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_AddRotateRel,	0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_ScaleFactor,	1.0f, 1.0f, 1.0f );

//#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
//	INIT_VECTOR3( m_ScaleFactorByUnit,	1.0f, 1.0f, 1.0f );
//#endif GIANT_UNIT_GIANT_EFFECT_TEST





	m_LatencyTime		= 0.0f;
	m_bCullingCheck		= true;
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    m_bRenderCullCheck = true;
#endif  X2OPTIMIZE_CULLING_PARTICLE

	m_bTriggerWait		= false;
	m_bDelete			= false;
	m_bInChain			= false;
	m_bDynamicChain		= true;

	m_ScrewValue		= -1;
	m_ScrewRotateSpeed	= 10;

	m_vVelocity			= D3DXVECTOR3( 0, 0, 0 );

#ifdef PARTICLE_NOTAPPLY_UNITSCALE
	m_bApplyUnitScale = true;
#endif

#ifdef NEW_HENIR_TEST
	m_bUseDXMatrix = false;
#endif NEW_HENIR_TEST

#ifdef ARA_CHANGE_CLASS_FIRST
	m_bReverseY = false;
#endif
#ifdef EFFECT_TOOL
	m_TriggerCount_Origin.m_Max = m_TriggerCount_Origin.m_Min = -1.f;
	m_TriggerTime_Origin.m_Max = m_TriggerTime_Origin.m_Min = -1.f;
	m_DrawCount_Origin.m_Max = m_DrawCount_Origin.m_Min = 1.f;
	m_fLandPosition_Origin.m_Max = m_fLandPosition_Origin.m_Min = 0.f;

	INIT_VECTOR3( m_BlackHolePosition_Origin.m_Max, 0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_BlackHolePosition_Origin.m_Min, 0.0f, 0.0f, 0.0f );

	INIT_VECTOR3( m_LightPos_Origin.m_Max, 0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_LightPos_Origin.m_Min, 0.0f, 0.0f, 0.0f );

	INIT_VECTOR3( m_AddRotateRel_Origin.m_Max, 0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_AddRotateRel_Origin.m_Min, 0.0f, 0.0f, 0.0f );

	INIT_VECTOR3( m_vSphericalEmitRotation_Origin.m_Max, 0.0f, 0.0f, 0.0f );
	INIT_VECTOR3( m_vSphericalEmitRotation_Origin.m_Min, 0.0f, 0.0f, 0.0f );
#endif //EFFECT_TOOL

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
	m_fAccumElapsedTime = 0.0f;
    m_bPerFrameSimulation = false;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	m_pkBoundingSphere = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Bounding_Sphere.x" );
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST

#ifdef ADD_ALPHATESTENABLE
	m_bAlphaTest = false;
#endif
}

CKTDGParticleSystem::CParticleEventSequence::~CParticleEventSequence()
{
	ClearAllParticle();

	BOOST_TEST_FOREACH( CParticleEvent*, pEvent, m_EventList )
	{
		SAFE_DELETE( pEvent );
	}

	m_EventList.clear();	

	map<int,CKTDXDeviceTexture*>::iterator i;
	for( i = m_TextureMap.begin(); i != m_TextureMap.end(); i++ )
	{
		CKTDXDeviceTexture* pTex = i->second;
		SAFE_CLOSE( pTex );
	}
	m_TextureMap.clear();

	SAFE_CLOSE( m_pChangeTexXET );
	SAFE_CLOSE( m_pMultiTexXET );
	SAFE_CLOSE( m_pAniXET );

	SAFE_CLOSE( m_pXMesh );
	SAFE_DELETE( m_pMathMatrix );

#ifdef SKINMESH_PARTICLE_TEST

	SAFE_CLOSE( m_pXSkinMesh );

#endif SKINMESH_PARTICLE_TEST

	m_TraceSeqNameList.clear();
	m_FinalSeqNameList.clear();

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	SAFE_CLOSE( m_pkBoundingSphere );
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
}

void    CKTDGParticleSystem::CParticleEventSequence::ClearAllParticle()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_pParticleSystem != NULL )
    {
        KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
        if ( m_iParticleList >= 0 && m_iParticleList < (int) coParticleList.storage_size() )
        {
            for( KParticleHandleList::iterator iter = coParticleList.begin( m_iParticleList );
                iter != coParticleList.end( m_iParticleList ); ++iter )
            {
                KParticleHandleInfo& info = *iter;
                if ( info.m_pParticle != NULL )
                    info.m_pParticle->Finalize();
            }//for
            if ( coParticleList.data( m_iParticleList ).m_pParticle != NULL )
                coParticleList.data( m_iParticleList ).m_pParticle->Finalize();
            coParticleList.splice_and_merge_list( coParticleList.begin( PARTICLELIST_FREE ), m_iParticleList );
        }
    }
    m_iParticleList = -1;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	for( CParticleList::iterator iter = m_ParticleList.begin(); iter != m_ParticleList.end(); iter++ )
	{
		CParticle* pParticle = *iter;
		ASSERT( pParticle != NULL );
		if ( pParticle == NULL )
			continue;
		SAFE_DELETE( pParticle );
	}//for

	m_ParticleList.clear();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

}//CKTDGParticleSystem::CParticleEventSequence::ClearAllParticle()

void CKTDGParticleSystem::CParticleEventSequence::InChain()
{
	if( m_bInChain == true )
		return;
	g_pKTDXApp->GetDGManager()->AddObjectChain( this );
	m_bInChain = true;
}

void CKTDGParticleSystem::CParticleEventSequence::OutChain()
{
	if( m_bInChain == false || m_bDynamicChain == false )
		return;
	g_pKTDXApp->GetDGManager()->RemoveObjectChain( this );
	m_bInChain = false;
}

HRESULT CKTDGParticleSystem::CParticleEventSequence::OnFrameMove( double fTime, float fElapsedTime )
{	
	KTDXPROFILE();

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	m_fAccumElapsedTime += fElapsedTime;

	if( m_bPerFrameSimulation == false && !g_pKTDXApp->IsFinalFrameOfSimulationLoop() )
		return S_OK;

	fElapsedTime = m_fAccumElapsedTime;
	m_fAccumElapsedTime = 0.0f;

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

	if( m_LatencyTime > 0.0f )
	{
		m_LatencyTime -= fElapsedTime;
		return S_OK;
	}

#ifdef STOP_UNIT_STOP_EFFECT_TEST

	m_StopTime.OnFrameMove( fTime, fElapsedTime );
	if( m_StopTime.m_fTimeLeft > 0.f )
	{
		return S_OK; 
	}

#endif STOP_UNIT_STOP_EFFECT_TEST

	//트레이스 시스템
	D3DXVECTOR3 gapPos;
	if( m_Trace == true )
	{
		gapPos			= m_Position - m_OldPosition;
		m_OldPosition	= m_Position;
	}

	KTDXPROFILE_BEGIN( "CParticleEventSequence onframemove start" );

	if( m_vVelocity != D3DXVECTOR3(0, 0, 0) )
	{
		D3DXMATRIX	mDir;
		D3DXVECTOR4 dirVec4;

		D3DXVECTOR3 vDisplace = m_vVelocity * fElapsedTime;
		D3DXMatrixRotationYawPitchRoll( &mDir, D3DXToRadian(m_AxisAngleDegree.y), D3DXToRadian(m_AxisAngleDegree.x), D3DXToRadian(m_AxisAngleDegree.z) );
		D3DXVec3Transform( &dirVec4, &vDisplace, &mDir );
		vDisplace.x	= dirVec4.x;
		vDisplace.y	= dirVec4.y;
		vDisplace.z	= dirVec4.z;

		m_Position += vDisplace;
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	if( GetTriggerTime() != -1.0f && GetTriggerTime() <= GetTime() )
	{
		SetEmitRate( 0.0f, 0.0f );
		if( GetTriggerWait() == false 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            && ( m_iParticleList < 0 || m_iParticleList >= (int) coParticleList.storage_size() || coParticleList.empty( m_iParticleList ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            && m_ParticleList.empty() 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
           )
			m_bDelete = true;
	}
	if( GetTriggerCount() != -1  && GetTriggerCount() <= GetCount() )
	{
		SetEmitRate( 0.0f, 0.0f );			
		if( GetTriggerWait() == false 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            && ( m_iParticleList < 0 || m_iParticleList >= (int) coParticleList.storage_size() || coParticleList.empty( m_iParticleList ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            && m_ParticleList.empty() 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            )
			m_bDelete = true;
	}

	m_Time += fElapsedTime;
	m_fElapsedTime = fElapsedTime;


	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "m_Particles onframemove" );
	SetBoundingRadius( 0.0f );
	float newBoundingRadius = 0.f;
	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    unsigned    uParticleNum = 0;
    if ( m_iParticleList >= 0 && m_iParticleList < (int) coParticleList.storage_size() )
    for( KParticleHandleList::iterator iter = coParticleList.begin( m_iParticleList );
        iter != coParticleList.end( m_iParticleList ); )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	for( CParticleList::iterator iter = m_ParticleList.begin(); iter != m_ParticleList.end(); )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        KParticleHandleInfo& info = *iter;
        CParticle* pParticle = info.m_pParticle;
        ASSERT( pParticle != NULL );
        if ( pParticle == NULL )
        {
            KParticleHandleList::iterator iterTemp = iter;
            ++iter;
            coParticleList.splice( coParticleList.begin( PARTICLELIST_FREE ), iterTemp );
            continue;
        }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CParticle* pParticle = *iter;
		ASSERT( pParticle != NULL );
		if ( pParticle == NULL )
		{
			iter = m_ParticleList.erase( iter );
			continue;
		}//if
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		if( pParticle->OnFrameMove( fTime, fElapsedTime ) == false )
		{
			for( int i = 0; i < (int)m_FinalSeqNameList.size(); i++ )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->CreateSequence( NULL, m_FinalSeqNameList[i].c_str(), pParticle->GetPos(), -1, -1, -1, -1, -1.0f, true, 0.0f, true );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
				if( NULL != pSeq )
				{
					pSeq->SetScaleByUnit( GetScaleByUnit() );
				}
#endif GIANT_UNIT_GIANT_EFFECT_TEST

			}
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            pParticle->Finalize();
            KParticleHandleList::iterator iterTemp = iter;
            ++iter;
            coParticleList.splice( coParticleList.begin( PARTICLELIST_FREE ), iterTemp );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			iter = m_ParticleList.erase( iter );
			SAFE_DELETE( pParticle );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
		else
		{
			//트레이스 적용
			if( m_Trace == true )
			{
				pParticle->SetPos( pParticle->GetPos() + gapPos );
				pParticle->SetAxisRotateDegree( m_AxisAngleDegree );
			}

			//중력 적용
            D3DXVECTOR3 vGravity = m_vGravity.GetRandomNumInRange();
			pParticle->m_vVelocity += vGravity * fElapsedTime;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            pParticle->m_vVelocityToAccumPos += ( 0.5f * fElapsedTime * fElapsedTime ) * vGravity;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			//블랙홀 포인트
			if( pParticle->GetBlackHoleTime() > 0.0f )
				pParticle->SetBlackHoleSpeed( (m_BlackHolePosition - pParticle->GetPos()) / pParticle->GetBlackHoleTime() );

			if( m_bUseLookPoint == true )
			{
				D3DXVECTOR3 dirVec;
				D3DXVec3Normalize( &dirVec, &(m_LookPoint - pParticle->GetPos()) );

				D3DXMATRIX matRotation, matRotationOrth;

				D3DXMatrixLookAtLH( &matRotation, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &dirVec, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
				D3DXMatrixInverse( &matRotationOrth, NULL, &matRotation );

				float temp = matRotationOrth._11;
				matRotationOrth._11 = matRotationOrth._13;
				matRotationOrth._13 = temp;

				temp = matRotationOrth._21;
				matRotationOrth._21 = matRotationOrth._23;
				matRotationOrth._23 = temp;

				temp = matRotationOrth._31;
				matRotationOrth._31 = matRotationOrth._33;
				matRotationOrth._33 = temp;

				if( matRotationOrth._12 == 0 && matRotationOrth._22 == 0 )
				{
					dirVec.z	= 0.0f;
					dirVec.y	= D3DXToDegree(atan2f( -matRotationOrth._31, matRotationOrth._33 ));
					dirVec.x	= D3DXToDegree(atan2f( -matRotationOrth._21, matRotationOrth._11 ));
				}
				else
				{
					dirVec.z	= D3DXToDegree(asinf( matRotationOrth._32 ));
					dirVec.y	= D3DXToDegree(atan2f( -matRotationOrth._31, matRotationOrth._33 ));
					dirVec.x	= D3DXToDegree(atan2f( -matRotationOrth._12, matRotationOrth._22 ));
				}

				pParticle->SetRotate( dirVec );
			}

			//이벤트 적용
			RunEvent( *pParticle );

			//랜드 포지션
			if( m_bUseLand == true )
			{
				if( pParticle->m_vPos.y < m_fLandPosition )
				{
					pParticle->m_vPos.y = m_fLandPosition;
					if( pParticle->m_vVelocity.y < 0.0f )
					{
						if( pParticle->m_vVelocity.y >= -50.0f
							&& pParticle->m_vVelocity.y <= 50.0f )
                        {
							pParticle->m_vVelocity.y = 0.0f;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                            pParticle->m_vVelocityToAccumPos.y = 0.f;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                        }
						else
                        {
							pParticle->m_vVelocity.y *= -0.5f;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                            pParticle->m_vVelocityToAccumPos.y = pParticle->m_vVelocity.y * fElapsedTime;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                        }
					}
				}
			}
		
		
			if( m_bCullingCheck == true && m_ParticleType != PT_2D_PLANE )
			{
				newBoundingRadius = GetDistance( pParticle->m_vPos, D3DXVECTOR3(0.0f,0.0f,0.0f) );
				if( GetBoundingRadius() < newBoundingRadius )
					SetBoundingRadius( newBoundingRadius );
			}
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            uParticleNum++;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			iter++;
		}
	}
	KTDXPROFILE_END();


	KTDXPROFILE_BEGIN( "create particles" );
	//파티클 생성
	float fEmitRateThisFrame	= m_EmitRate.GetRandomNumInRange();
	int iNumNewParts			= static_cast<int>( fEmitRateThisFrame * fElapsedTime );
	m_fNumNewPartsExcess		+= (float)(fEmitRateThisFrame * fElapsedTime) - iNumNewParts;

	if( m_fNumNewPartsExcess > 1.0f ) 
	{
		iNumNewParts			+= (int)m_fNumNewPartsExcess;
		m_fNumNewPartsExcess	-= (int)m_fNumNewPartsExcess;
	}

	for( int q = 0; q < iNumNewParts ; q++ ) 
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( m_iMaxParticleNum > (int)uParticleNum )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( m_iMaxParticleNum > (int)m_ParticleList.size() )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			if( GetTriggerCount() != -1 )
			{
				if( GetTriggerCount() > GetCount() )
				{
                    CParticle* pParticle = CreateNewParticle( m_Position );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    if ( pParticle != NULL )
                        ++uParticleNum;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				}
				else
					break;
			}
			else
            {
                CParticle* pParticle = CreateNewParticle( m_Position );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                if ( pParticle != NULL )
                    ++uParticleNum;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            }
		}
		else
			break;
	}
	KTDXPROFILE_END();





	KTDXPROFILE_BEGIN( "particle in/out render chain" );
	if( GetShowObject() == false
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        || ( m_iParticleList < 0 || m_iParticleList >= (int) coParticleList.storage_size() || coParticleList.empty( m_iParticleList ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        || m_ParticleList.empty()
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE        
        || m_LatencyTime > 0.0f )
	{
		//OutChain();
	}
	else
	{
		InChain();
	}
	KTDXPROFILE_END();

	return S_OK;
}




//{{ robobeg : 2008-10-15
/*virtual*/ 
RENDER_HINT   CKTDGParticleSystem::CParticleEventSequence::OnFrameRender_Prepare() 
{ 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

    if ( m_LatencyTime > 0.f || 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            ( m_iParticleList < 0 || m_iParticleList >= (int) coParticleList.storage_size() || coParticleList.empty( m_iParticleList ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_ParticleList.empty() 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            )
        return RENDER_HINT_NORENDER;

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	if ( m_pkBoundingSphere != NULL )
	{
		return RENDER_HINT_DEFAULT;
	}//if
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST

    switch( m_ParticleType )
    {
    case PT_MESH:
        return ( m_pXMesh != NULL ) 
            ? RENDER_HINT_XMESH( m_MeshRenderType, false )
            : RENDER_HINT_NORENDER;

#ifdef SKINMESH_PARTICLE_TEST

    case PT_SKINMESH:
        {
        	RENDER_HINT renderHintResult = RENDER_HINT_NORENDER;
            RENDER_HINT renderHint;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( m_iParticleList >= 0 && m_iParticleList < (int) coParticleList.storage_size() )
            for( KParticleHandleList::iterator iter = coParticleList.begin( m_iParticleList );
                iter != coParticleList.end( m_iParticleList ); ++iter)
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    	    BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                KParticleHandleInfo& info = *iter;
                CParticle* pPart = info.m_pParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				// Render each particle a bunch of times to get a blurring effect		
			    ASSERT( pPart != NULL && pPart->m_pXSkinAnim != NULL );
                if ( pPart == NULL || pPart->m_pXSkinAnim == NULL )
                    continue;
                pPart->m_pXSkinAnim->SetInstantShow( false );
                if ( !pPart->m_pXSkinAnim->GetShowObject() )
                    continue;
                renderHint = pPart->m_pXSkinAnim->OnFrameRender_Prepare();
                if ( renderHint.m_eHint != RENDER_HINT::NORENDER )
                {
                    pPart->m_pXSkinAnim->SetInstantShow( true );
                    renderHintResult.MergeRenderHint( renderHint );
                }//if                
            }//for

            return renderHintResult;
        }

#endif SKINMESH_PARTICLE_TEST

    }//switch

    return RENDER_HINT_DEFAULT;
}//OnFrameRender_Prepare()


/*virtual*/
void    CKTDGParticleSystem::CParticleEventSequence::OnFrameRender_Draw()
{
	KTDXPROFILE();

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

    if ( m_LatencyTime > 0.f || 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            ( m_iParticleList < 0 || m_iParticleList >= (int) coParticleList.storage_size() || coParticleList.empty( m_iParticleList ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_ParticleList.empty() 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            )
        return;

	switch( m_ParticleType )
	{
		case PT_3D_PLANE:		
			{
				KTDXPROFILE_BEGIN("PT_3D_PLANE");
				// Set up the vertex buffer to be rendered
//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//				g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_PARTICLE );
//				g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pParticleSystem->GetVB(), 0, sizeof(VERTEX_PARTICLE) );
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

                g_pKTDXApp->ResetWorldTransform();

                KD3DBEGIN()

				    // Set the render states for using point sprites
				    CKTDGStateManager::PushRenderState( D3DRS_SRCBLEND,		GetSrcBlendMode() );
				    CKTDGStateManager::PushRenderState( D3DRS_DESTBLEND,	GetDestBlendMode() );
				    if( m_bZEnable == true )
					    CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	TRUE );
				    else
					    CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );

				    // note!! pt_3d_plane인 경우에 무조건 zwriteenable = false로 rendering되게 되어있음
				    //if( m_bZWriteEnable == true )
				    //{
				    //	CKTDGStateManager::PushRenderState( D3DRS_ZWRITEENABLE,	TRUE );
				    //}

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
                    OnFrameRender_VERTEX_PARTICLE();
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//				    OnFrameRender( m_pParticleSystem->GetVB(), m_pParticleSystem->GetVBSize() );
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
    				
				    //if( m_bZWriteEnable == true )
				    //{
				    //	CKTDGStateManager::PopRenderState( D3DRS_ZWRITEENABLE );
				    //}

                KD3DEND()
				KTDXPROFILE_END();
			}
			break;


		case PT_2D_PLANE:
			{
				KTDXPROFILE_BEGIN("PT_2D_PLANE");
//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//				// Set up the vertex buffer to be rendered
//				g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_PARTICLE_RHW );
//				g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pParticleSystem->GetVBRHW(), 0, sizeof(VERTEX_PARTICLE_RHW) );
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

                g_pKTDXApp->ResetWorldTransform();

				// Set the render states for using point sprites
                KD3DBEGIN()
				    CKTDGStateManager::PushRenderState( D3DRS_SRCBLEND,		GetSrcBlendMode() );
				    CKTDGStateManager::PushRenderState( D3DRS_DESTBLEND,	GetDestBlendMode() );

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
                    OnFrameRender_VERTEX_PARTICLE_RHW();
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//					if ( NULL != m_pParticleSystem->GetVBRHW() )
//						OnFrameRenderRHW( m_pParticleSystem->GetVBRHW(), m_pParticleSystem->GetVBSize() );
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

                KD3DEND()
                KTDXPROFILE_END();
			}
			break;

		case PT_MESH:
			{
				KTDXPROFILE_BEGIN("PT_MESH");
				OnFrameRenderMesh_Draw();
				KTDXPROFILE_END();
			}
			break;

#ifdef SKINMESH_PARTICLE_TEST
		case PT_SKINMESH:
			{
				OnFrameRenderSkinMesh_Draw();

			} break;

#endif SKINMESH_PARTICLE_TEST
	}

//	return S_OK;
}
//}} robobeg : 2008-10-15





//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
void CKTDGParticleSystem::CParticleEventSequence::OnFrameRender_VERTEX_PARTICLE()
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//void CKTDGParticleSystem::CParticleEventSequence::OnFrameRender( LPDIRECT3DVERTEXBUFFER9 pVB, int iVBSize )
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
{

	KTDXPROFILE();
//{{AFX
#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
	if( false == m_pParticleSystem->GetRenderEffectMadeByGameUnit() && 
		CKTDGObject::OT_EFFECT_MADE_BY_GAME_UNIT == GetObjectType() )
		return;

    //bool    bIdentityWorldMatrix = false;
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

    ASSERT( m_pParticleSystem != NULL );
    if ( m_pParticleSystem == NULL || m_DrawCount <= 0 )
        return;

    std::vector<VERTEX_PARTICLE>& vecVERTEX_PARTICLE = m_pParticleSystem->GetVecVERTEX_PARTICLE();
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//	if ( m_pParticleSystem->GetUseDynamicTexture() == true )
//	{
//
//		HRESULT hr;
//
//		VERTEX_PARTICLE*		pVertices;
//
//		// Lock the vertex buffer.  We fill the vertex buffer in small
//		// chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
//		// each chunk, we call DrawPrim, and lock the next chunk.  When
//		// we run out of space in the vertex buffer, we start over at
//		// the beginning, using D3DLOCK_DISCARD.
//		D3DXMATRIX finalMat;
//		D3DXVECTOR4 v4temp;
//		int iCurStart = 0;
//		UINT renderParticle = 0;	
//
//		g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_PARTICLE );
//		g_pKTDXApp->GetDevice()->SetStreamSource(0, pVB, 0, sizeof(VERTEX_PARTICLE));
//
//		//텍스쳐 그룹화 해서 그린다
//		map<int,CKTDXDeviceTexture*>::iterator iTex;
//		for( iTex = m_TextureMap.begin() ; iTex != m_TextureMap.end() ; iTex++ )
//		{
//			CKTDXDeviceTexture* pTex = (CKTDXDeviceTexture*)iTex->second;
//			if( pTex != NULL )
//				pTex->SetDeviceTexture();
//			int texID = iTex->first;
//
//			if( m_pParticleSystem->GetOffsetSize() >= m_pParticleSystem->GetVBSize() )
//				m_pParticleSystem->SetOffsetSize( 0 );
//
//			KTDXPROFILE_BEGIN( "LOCK" );
//			if( FAILED( hr = pVB->Lock( m_pParticleSystem->GetOffsetSize() * 6 * sizeof(VERTEX_PARTICLE), 
//				m_pParticleSystem->GetBatchSize() * 6 * sizeof(VERTEX_PARTICLE), 
//                /** Direct3D9: (ERROR) :Can specify D3DLOCK_DISCARD or D3DLOCK_NOOVERWRITE for only Vertex Buffers created with D3DUSAGE_DYNAMIC
//                    - jintaeks on 2008-10-15, 11:06 */
//                    (void**)&pVertices, m_pParticleSystem->GetUseDynamicTexture() ? (m_pParticleSystem->GetOffsetSize() ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD) : 0L )))
//			{
//				wcout << L"Fail: CKTDGParticleSystem::CParticleEventSequence::OnFrameRender::Lock" << std::endl;
//				ErrorLogMsg( KEM_ERROR358, L"OnFrameRender" );
//				return;
//			}
//			KTDXPROFILE_END();
//
//			DWORD numParticlesInBatch = 0;
//			
//
//			// Render each particle
//			BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
//			{	
//				if ( pPart == NULL )
//					continue;
//				// Render each particle a bunch of times to get a blurring effect		
//				if( texID != pPart->m_TextureID )
//					continue;
//				else
//				{
//					numParticlesInBatch++;
//					renderParticle++;
//					if( renderParticle >= m_ParticleList.size() )
//						iTex = m_TextureMap.end();
//				}
//
//
//				//1-----3,4
//				//
//				//2,5---6
//
//				KTDXPROFILE_BEGIN( "MATH_MATRIX" );
//
//				D3DXVECTOR3 partPos = pPart->m_vPos;
//				partPos.x += pPart->m_vCrash.x;
//				partPos.y += pPart->m_vCrash.y;
//				partPos.z += pPart->m_vCrash.z;
//				pPart->m_vCrash = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
//
//#ifdef LOCAL_ROTATE_EFFECT_P
//				D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x-pPart->GetRotateLocal().x), 
//												D3DXToRadian(pPart->m_vRotate.y-pPart->GetRotateLocal().y), 
//												D3DXToRadian(pPart->m_vRotate.z-pPart->GetRotateLocal().z) );
//#else
//				D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x), D3DXToRadian(pPart->m_vRotate.y), D3DXToRadian(pPart->m_vRotate.z) );
//#endif
//
//				m_pMathMatrix->Move( partPos.x, partPos.y, partPos.z );
//				m_pMathMatrix->Rotate( vRot );
//				//m_pMathMatrix->Scale( pPart->m_vSize.x, pPart->m_vSize.y, 1.0f );
//
//
//				finalMat = m_pMathMatrix->GetMatrix( m_BillBoardType );
//				KTDXPROFILE_END();
//
//				KTDXPROFILE_BEGIN( "SET_VERTEX" );
//
//
//				//wstringstream wstrstm;
//				//wstrstm << numParticlesInBatch << L" " << m_pParticleSystem->GetOffsetSize() << L" " << m_pParticleSystem->GetBatchSize();
//				//LastErrorLog( wstrstm.str().c_str() );
//				
//				//1
//				pVertices->position.x = -pPart->m_vSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = pPart->m_vSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = v4temp.x;
//				pVertices->position.y = v4temp.y;
//				pVertices->position.z = v4temp.z;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//2
//				pVertices->position.x = -pPart->m_vSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = -pPart->m_vSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = v4temp.x;
//				pVertices->position.y = v4temp.y;
//				pVertices->position.z = v4temp.z;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//3
//				pVertices->position.x = pPart->m_vSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = pPart->m_vSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = v4temp.x;
//				pVertices->position.y = v4temp.y;
//				pVertices->position.z = v4temp.z;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//4
//				pVertices->position.x = pPart->m_vSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = pPart->m_vSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = v4temp.x;
//				pVertices->position.y = v4temp.y;
//				pVertices->position.z = v4temp.z;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//5
//				pVertices->position.x = -pPart->m_vSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = -pPart->m_vSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = v4temp.x;
//				pVertices->position.y = v4temp.y;
//				pVertices->position.z = v4temp.z;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//6			
//				pVertices->position.x = pPart->m_vSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = -pPart->m_vSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = v4temp.x;
//				pVertices->position.y = v4temp.y;
//				pVertices->position.z = v4temp.z;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				KTDXPROFILE_END();
//
//				if( numParticlesInBatch == m_pParticleSystem->GetBatchSize() )
//				{
//					// Done filling this chunk of the vertex buffer.  Lets unlock and
//					// draw this portion so we can begin filling the next chunk.
//
//					pVB->Unlock();
//
//					
//					
//					for( int dc = 0; dc < m_DrawCount; dc++ )
//					{
//						if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 
//							m_pParticleSystem->GetOffsetSize() * 6, numParticlesInBatch * 2 )))
//						{
//							ErrorLog( KEM_ERROR30 );
//							return;
//						}
//					}
//					
//
//					m_pParticleSystem->SetOffsetSize( m_pParticleSystem->GetOffsetSize() + m_pParticleSystem->GetBatchSize() );
//
//					if(m_pParticleSystem->GetOffsetSize() >= m_pParticleSystem->GetVBSize() ) 
//						m_pParticleSystem->SetOffsetSize( 0 );
//
//
//					// Lock the next chunk of the vertex buffer.  If we are at the 
//					// end of the vertex buffer, DISCARD the vertex buffer and start
//					// at the beginning.  Otherwise, specify NOOVERWRITE, so we can
//					// continue filling the VB while the previous chunk is drawing.
//
//					KTDXPROFILE_BEGIN( "LOCK_INNER" );
//					if( FAILED( hr = pVB->Lock( m_pParticleSystem->GetOffsetSize() * 6 * sizeof(VERTEX_PARTICLE), 
//						m_pParticleSystem->GetBatchSize() * 6 * sizeof(VERTEX_PARTICLE), 
//                        (void**)&pVertices, m_pParticleSystem->GetUseDynamicTexture() ? (m_pParticleSystem->GetOffsetSize() ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD) : 0L )))
//					{
//						ErrorLogMsg( KEM_ERROR358, L"OnFrameRender2" );
//						return;
//					}
//					KTDXPROFILE_END();
//
//					numParticlesInBatch = 0;
//					renderParticle = 0;
//				}
//			}
//
//			// Unlock the vertex buffer
//			pVB->Unlock();
//
//			
//			
//			// Render any remaining particles
//			KTDXPROFILE_BEGIN( "DrawPrimitive" );
//			if( numParticlesInBatch > 0 )
//			{
//				for( int dc = 0; dc < m_DrawCount; dc++ )
//				{
//					if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 
//						m_pParticleSystem->GetOffsetSize() * 6, numParticlesInBatch * 2 )))
//					{
//						ErrorLog( KEM_ERROR31 );
//						return;
//					}
//				}
//				numParticlesInBatch = 0;
//			}
//
//			m_pParticleSystem->SetOffsetSize( m_pParticleSystem->GetOffsetSize() + m_pParticleSystem->GetBatchSize() );
//
//			KTDXPROFILE_END();		
//		}	
//
//	}
////}}AFX
//	else
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
	{
		HRESULT hr;

		VERTEX_PARTICLE*		pVertices = NULL;

		// Lock the vertex buffer.  We fill the vertex buffer in small
		// chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
		// each chunk, we call DrawPrim, and lock the next chunk.  When
		// we run out of space in the vertex buffer, we start over at
		// the beginning, using D3DLOCK_DISCARD.
		D3DXMATRIX finalMat;
		D3DXVECTOR4 v4temp;
		int iCurStart = 0;
		UINT renderParticle = 0;

#ifdef X2OPTIMIZE_CULLING_PARTICLE
		CKTDGCamera& kCamera = g_pKTDXApp->GetDGManager()->GetCamera();
		const CKTDGFrustum& kFrustum = g_pKTDXApp->GetDGManager()->GetFrustum();
#endif//X2OPTIMIZE_CULLING_PARTICLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		//텍스쳐 그룹화 해서 그린다
		map<int,CKTDXDeviceTexture*>::iterator iTex;
		for( iTex = m_TextureMap.begin() ; iTex != m_TextureMap.end() ; iTex++ )
		{
			CKTDXDeviceTexture* pTex = (CKTDXDeviceTexture*)iTex->second;

			int texID = iTex->first;


//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
			renderParticle = 0;
			vecVERTEX_PARTICLE.resize( 0 );
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//
//			KTDXPROFILE_BEGIN( "LOCK" );
//			if( FAILED( hr = pVB->Lock( 0, iVBSize * 6 * sizeof(VERTEX_PARTICLE), (void**)&pVertices, 0 )))
//			{
//				wcout << L"Fail: CKTDGParticleSystem::CParticleEventSequence::OnFrameRender::Lock" << std::endl;
//				return;
//			}
//			KTDXPROFILE_END();
//
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

			// Render each particle
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( m_iParticleList >= 0 && m_iParticleList < (int) coParticleList.storage_size() )
            for( KParticleHandleList::iterator iter = coParticleList.begin( m_iParticleList );
                iter != coParticleList.end( m_iParticleList ); ++iter)
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    	    BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                KParticleHandleInfo& info = *iter;
                CParticle* pPart = info.m_pParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

				if ( pPart == NULL )
					continue;
				// Render each particle a bunch of times to get a blurring effect		
				if( texID != pPart->m_TextureID )
					continue;
				else
				{
					renderParticle++;
//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//					if( renderParticle >= m_ParticleList.size() )
//						iTex = m_TextureMap.end();
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
				}


				//1-----3,4
				//
				//2,5---6

				KTDXPROFILE_BEGIN( "MATH_MATRIX" );

				D3DXVECTOR3 partPos = pPart->GetPos() + pPart->GetCrash();
				pPart->SetCrash( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );


#ifdef PARTICLE_STRETCH_TEST
				if( 1.f != pPart->GetStretchScale() )
				{
					D3DXVECTOR3 vYAxis(0, 1, 0);
					D3DXVECTOR3 vDirVec = pPart->m_vPos - pPart->m_vPosOld;
					D3DXVECTOR3 vZVec(0, 0, 0 );
					D3DXVec3Cross( &vZVec, &vYAxis, &vDirVec );
					D3DXVec3Normalize( &vZVec, &vZVec );
					D3DXVec3Cross( &vYAxis, &vDirVec, &vZVec );
					D3DXVec3Normalize( &vDirVec, &vDirVec );
					D3DXVec3Normalize( &vYAxis, &vYAxis );


					D3DXMATRIX matRotStretchDir;
					D3DXMatrixIdentity( &matRotStretchDir );
					matRotStretchDir._11 = vDirVec.x;
					matRotStretchDir._12 = vDirVec.y;
					matRotStretchDir._13 = vDirVec.z;

					matRotStretchDir._21 = vYAxis.x;
					matRotStretchDir._22 = vYAxis.y;
					matRotStretchDir._23 = vYAxis.z;

					matRotStretchDir._31 = vZVec.x;
					matRotStretchDir._32 = vZVec.y;
					matRotStretchDir._33 = vZVec.z;

					//D3DXVECTOR3 vXAXis(1, 0, 0 );
					//D3DXVECTOR3 vDirVecTransformed;
					//D3DXMATRIX matRotTemp = matRotTemp1;
					//D3DXVec3TransformCoord( &vDirVecTransformed, &vXAXis, &matRotTemp );
					//D3DXVec3Normalize( &vDirVecTransformed, &vDirVecTransformed );

					m_pMathMatrix->Move( partPos.x, partPos.y, partPos.z );

					finalMat = m_pMathMatrix->GetMatrix( m_BillBoardType, &matRotStretchDir );
				}
				else
#endif PARTICLE_STRETCH_TEST
				{

//#ifdef LOCAL_ROTATE_EFFECT_P

					D3DXVECTOR3 vRotDegree( 0, 0, 0 );

					if( true == m_bUseLookPoint )
					{
						vRotDegree = pPart->GetRotate() + pPart->GetRotateLocal();
					}
					else
					{
						switch( m_BillBoardType )
						{
						default:
						case CKTDGMatrix::BT_ALL:
							{
								float fYDegree = 0.f;

								if ( true == m_bReverseY )
									fYDegree = 180.f;

                                vRotDegree = pPart->GetRotateLocal();
                                vRotDegree.y += fYDegree;

							} break;

						case CKTDGMatrix::BT_X:
							{
                                vRotDegree = pPart->GetRotate() + pPart->GetRotateLocal();
                                vRotDegree.x = pPart->GetRotateLocal().x;
							} break;

						case CKTDGMatrix::BT_Y:
							{
                                vRotDegree = pPart->GetRotate() + pPart->GetRotateLocal();
                                vRotDegree.y = pPart->GetRotateLocal().y;
							} break;

						case CKTDGMatrix::BT_Z:
							{
                                vRotDegree = pPart->GetRotate() + pPart->GetRotateLocal();
                                vRotDegree.z = pPart->GetRotateLocal().z;
							} break;


						case CKTDGMatrix::BT_NONE:
							{
						        vRotDegree = pPart->GetRotate() + pPart->GetRotateLocal();
							} break;
						}
					}
//#else
//					D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x), D3DXToRadian(pPart->m_vRotate.y), D3DXToRadian(pPart->m_vRotate.z) );
//#endif LOCAL_ROTATE_EFFECT_P


					m_pMathMatrix->Move( partPos.x, partPos.y, partPos.z );
					m_pMathMatrix->RotateDegree( vRotDegree );
					//m_pMathMatrix->Scale( pPart->m_vSize.x, pPart->m_vSize.y, 1.0f );


					finalMat = m_pMathMatrix->GetMatrix( m_BillBoardType );

#ifdef EVE_ELECTRA
					if(m_bUseDXMatrix == true)
					{
						finalMat = m_pMathMatrix->GetScaleMatrix() * m_pMathMatrix->GetRotMatrix() * m_DXMATRIX;
					}
#endif EVE_ELECTRA

				}
				
				KTDXPROFILE_END();



				float fStretchScale = 1.f;
#ifdef PARTICLE_STRETCH_TEST
				fStretchScale = pPart->GetStretchScale();
#endif PARTICLE_STRETCH_TEST


				D3DXVECTOR3 vScaleByUnit( 1, 1, 1 );
#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
				
#ifdef VERIFY_STAT_BY_BUFF
				vScaleByUnit = GetVec3ScaleByUnit();
#else	// VERIFY_STAT_BY_BUFF
				vScaleByUnit = GetScaleByUnit();
#endif // VERIFY_STAT_BY_BUFF

#endif GIANT_UNIT_GIANT_EFFECT_TEST



				KTDXPROFILE_BEGIN( "SET_VERTEX" );

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
                unsigned uSize = vecVERTEX_PARTICLE.size();
                vecVERTEX_PARTICLE.resize( uSize + 6 );
                pVertices = &vecVERTEX_PARTICLE[ uSize ];
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

				//1
				pVertices->position.x = -0.5f * pPart->m_vSize.x * m_ScaleFactor.x * fStretchScale * vScaleByUnit.x;
				pVertices->position.y = 0.5f * pPart->m_vSize.y * m_ScaleFactor.y * vScaleByUnit.y;
				pVertices->position.z = 0.0f;
				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
				pVertices->position.x = v4temp.x;
				pVertices->position.y = v4temp.y;
				pVertices->position.z = v4temp.z;
				pVertices->color = (DWORD)pPart->m_Color;
				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
				pVertices++;

				//2
				pVertices->position.x = -0.5f * pPart->m_vSize.x * m_ScaleFactor.x * fStretchScale * vScaleByUnit.x;
				pVertices->position.y = -0.5f * pPart->m_vSize.y * m_ScaleFactor.y * vScaleByUnit.y;
				pVertices->position.z = 0.0f;
				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
				pVertices->position.x = v4temp.x;
				pVertices->position.y = v4temp.y;
				pVertices->position.z = v4temp.z;
				pVertices->color = (DWORD)pPart->m_Color;
				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
				pVertices++;

				//3
				pVertices->position.x = 0.5f * pPart->m_vSize.x * m_ScaleFactor.x * fStretchScale * vScaleByUnit.x;
				pVertices->position.y = 0.5f * pPart->m_vSize.y * m_ScaleFactor.y * vScaleByUnit.y;
				pVertices->position.z = 0.0f;
				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
				pVertices->position.x = v4temp.x;
				pVertices->position.y = v4temp.y;
				pVertices->position.z = v4temp.z;
				pVertices->color = (DWORD)pPart->m_Color;
				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
				pVertices++;

				//4
				*pVertices = *(pVertices-1);
				pVertices++;

				//5
				*pVertices = *(pVertices-3);
				pVertices++;

				//6			
				pVertices->position.x = 0.5f * pPart->m_vSize.x * m_ScaleFactor.x * fStretchScale * vScaleByUnit.x;
				pVertices->position.y = -0.5f * pPart->m_vSize.y * m_ScaleFactor.y * vScaleByUnit.y;
				pVertices->position.z = 0.0f;
				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
				pVertices->position.x = v4temp.x;
				pVertices->position.y = v4temp.y;
				pVertices->position.z = v4temp.z;
				pVertices->color = (DWORD)pPart->m_Color;
				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
				pVertices++;

				KTDXPROFILE_END();

//#ifndef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//
//				if( renderParticle == iVBSize )
//				{
//					// Done filling this chunk of the vertex buffer.  Lets unlock and
//					// draw this portion so we can begin filling the next chunk.
//
//					pVB->Unlock();
//
//					for( int dc = 0; dc < m_DrawCount; dc++ )
//					{
//						if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, renderParticle * 2)))
//						{
//							ErrorLog( KEM_ERROR30 );
//							return;
//						}
//					}
//					renderParticle = 0;
//
//
//					// Lock the next chunk of the vertex buffer.  If we are at the 
//					// end of the vertex buffer, DISCARD the vertex buffer and start
//					// at the beginning.  Otherwise, specify NOOVERWRITE, so we can
//					// continue filling the VB while the previous chunk is drawing.
//
//					KTDXPROFILE_BEGIN( "LOCK_INNER" );
//					if( FAILED( hr = pVB->Lock( 0, iVBSize * 6 * sizeof(VERTEX_PARTICLE), (void**)&pVertices,
//                        m_pParticleSystem->GetUseDynamicTexture() ? D3DLOCK_DISCARD : 0L )))
//					{
//						return;
//					}
//					KTDXPROFILE_END();
//				}
//
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

			}


//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

            //BOOST_STATIC_ASSERT( D3DFVF_PARTICLE == CKTDGDynamicVBManager::D3DFVF_XYZ_DIFFUSE_TEX1 );
            if ( renderParticle > 0 )
            {
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
                if ( ms_bParticleCullingEnabled == true 
                    && m_bRenderCullCheck == true )
                {
		            D3DXVECTOR3 vCenter = *( (D3DXVECTOR3*)&vecVERTEX_PARTICLE[0] );
                    if ( kFrustum.CheckPoint( vCenter ) == false )
                    {
		                float fRadius;
		                hr = ComputeBoundingSphere( (D3DXVECTOR3*)&vecVERTEX_PARTICLE[0], vecVERTEX_PARTICLE.size(),
			                sizeof( VERTEX_PARTICLE ),
			                &vCenter, &fRadius );
                        if ( FAILED( hr ) )
                            continue;
			            if( kFrustum.CheckSphere( vCenter, fRadius ) == false )
				            continue;
                        //_DecideWorldMatrix_Normal( kCamera, vCenter, fRadius, bIdentityWorldMatrix );
                    }
                }
#endif  X2OPTIMIZE_CULLING_PARTICLE

			    if( pTex != NULL )
				    pTex->SetDeviceTexture();

                g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE_TEX1
                    , D3DPT_TRIANGLELIST, renderParticle * 2, &vecVERTEX_PARTICLE[0], m_DrawCount );
            }//if


//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//
//			// Unlock the vertex buffer
//			pVB->Unlock();
//
//			// Render any remaining particles
//			KTDXPROFILE_BEGIN( "DrawPrimitive" );
//			if( renderParticle > 0 )
//			{
//				for( int dc = 0; dc < m_DrawCount; dc++ )
//				{
//					if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, renderParticle * 2 )))
//					{
//						ErrorLog( KEM_ERROR31 );
//						return;
//					}
//				}
//				renderParticle = 0;
//			}
//			KTDXPROFILE_END();		
//
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

		}	

	}
}

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
void CKTDGParticleSystem::CParticleEventSequence::OnFrameRender_VERTEX_PARTICLE_RHW()
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//void CKTDGParticleSystem::CParticleEventSequence::OnFrameRenderRHW( LPDIRECT3DVERTEXBUFFER9 pVB, int iVBSize )
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
{
	KTDXPROFILE();
//{{AFX

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

    ASSERT( m_pParticleSystem != NULL );
    if ( m_pParticleSystem == NULL || m_DrawCount <= 0 )
        return;

    std::vector<VERTEX_PARTICLE_RHW>& vecVERTEX_PARTICLE_RHW = m_pParticleSystem->GetVecVERTEX_PARTICLE_RHW();
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//
//	if ( m_pParticleSystem->GetUseDynamicTexture() == true )
//
//	{
//		HRESULT hr;
//
//		VERTEX_PARTICLE_RHW*	pVertices;
//
//		// Lock the vertex buffer.  We fill the vertex buffer in small
//		// chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
//		// each chunk, we call DrawPrim, and lock the next chunk.  When
//		// we run out of space in the vertex buffer, we start over at
//		// the beginning, using D3DLOCK_DISCARD.
//		D3DXMATRIX finalMat;
//		D3DXVECTOR4 v4temp;
//		int iCurStart = 0;
//		UINT renderParticle = 0;
//
//		g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_PARTICLE_RHW );
//		g_pKTDXApp->GetDevice()->SetStreamSource(0, pVB, 0, sizeof(VERTEX_PARTICLE_RHW));
//
//		//텍스쳐 그룹화 해서 그린다
//		map<int,CKTDXDeviceTexture*>::iterator iTex;
//		for( iTex = m_TextureMap.begin() ; iTex != m_TextureMap.end() ; iTex++ )
//		{		
//			KTDXPROFILE_BEGIN("SetTexture");
//			CKTDXDeviceTexture* pTex = (CKTDXDeviceTexture*)iTex->second;
//			if( pTex != NULL )
//				pTex->SetDeviceTexture();
//			KTDXPROFILE_END();
//			int texID = iTex->first;
//
//			
//
//			if( m_pParticleSystem->GetOffsetRHWSize() >= m_pParticleSystem->GetVBSize() )
//				m_pParticleSystem->SetOffsetRHWSize( 0 );
//
//			KTDXPROFILE_BEGIN("Lock");
//			if( FAILED( hr = pVB->Lock( m_pParticleSystem->GetOffsetRHWSize() * 6 * sizeof(VERTEX_PARTICLE_RHW)
//				,m_pParticleSystem->GetBatchSize() * 6 * sizeof(VERTEX_PARTICLE_RHW), (void**)&pVertices, 
//                m_pParticleSystem->GetUseDynamicTexture() ? (m_pParticleSystem->GetOffsetRHWSize() ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD) : 0L )))
//			{
//				ErrorLogMsg( KEM_ERROR358, L"OnFrameRenderRHW" );
//				return;
//			}
//			KTDXPROFILE_END();
//
//			DWORD numParticlesInBatch = 0;
//
//			// Render each particle
//			BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
//			{	
//				if ( pPart == NULL )
//					continue;
//
//				// Render each particle a bunch of times to get a blurring effect		
//				if( texID != pPart->m_TextureID )
//					continue;
//				else
//				{
//					numParticlesInBatch++;
//					renderParticle++;
//					if( renderParticle >= m_ParticleList.size() )
//						iTex = m_TextureMap.end();
//				}
//
//
//				//1-----3,4
//				//
//				//2,5---6
//
//				D3DXVECTOR3 partPos = pPart->m_vPos;
//				partPos.x += pPart->m_vCrash.x;
//				partPos.y += pPart->m_vCrash.y;
//				partPos.z += pPart->m_vCrash.z;
//				pPart->m_vCrash = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
//
//				D3DXVECTOR3 partSize = pPart->m_vSize;
//
//				KTDXPROFILE_BEGIN("MatrixMath");
//				//해상도 변경 정보를 보간하고 싶으면 계산한다
//				D3DXVECTOR2 temp;
//				if( m_bResolutionConvert == true )
//				{
//					temp = g_pKTDXApp->ConvertByResolution( partPos.x, partPos.y );
//					partPos.x = temp.x;
//					partPos.y = temp.y;				
//				}
//				temp = g_pKTDXApp->ConvertByResolution( partSize.x, partSize.y );
//				partSize.x = temp.x;
//				partSize.y = temp.y;
//
//#ifdef LOCAL_ROTATE_EFFECT_P
//				D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x+pPart->m_vRotateLocal.x), 
//					D3DXToRadian(pPart->m_vRotate.y+pPart->m_vRotateLocal.y), 
//					D3DXToRadian(pPart->m_vRotate.z+pPart->m_vRotateLocal.z) );
//#else
//				D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x), D3DXToRadian(pPart->m_vRotate.y), D3DXToRadian(pPart->m_vRotate.z) );
//#endif
//
//				m_pMathMatrix->Move( partPos.x, partPos.y, partPos.z );
//				m_pMathMatrix->Rotate( vRot );
//				//m_pMathMatrix->Scale( pPart->m_vSize.x, pPart->m_vSize.y, 1.0f );
//
//				KTDXPROFILE_END();
//
//
//				//finalMat = m_pMathMatrix->GetMatrix( m_BillBoardType );
//				finalMat = m_pMathMatrix->GetMatrix( CKTDGMatrix::BT_NONE );
//
//				KTDXPROFILE_BEGIN("SetVertex");
//
//				//wstringstream wstrstm;
//				//wstrstm << numParticlesInBatch << L" " << m_pParticleSystem->GetOffsetSize() << L" " << m_pParticleSystem->GetBatchSize();
//				//LastErrorLog( wstrstm.str().c_str() );
//
//				//1
//				pVertices->position.x = -partSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = partSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = (int)v4temp.x - 0.5f;
//				pVertices->position.y = (int)v4temp.y - 0.5f;
//				pVertices->position.z = v4temp.z;
//				pVertices->rhw		  = 1.0f;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//2
//				pVertices->position.x = -partSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = -partSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = (int)v4temp.x - 0.5f;
//				pVertices->position.y = (int)v4temp.y - 0.5f;
//				pVertices->position.z = v4temp.z;
//				pVertices->rhw		  = 1.0f;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//3
//				pVertices->position.x = partSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = partSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = (int)v4temp.x - 0.5f;
//				pVertices->position.y = (int)v4temp.y - 0.5f;
//				pVertices->position.z = v4temp.z;
//				pVertices->rhw		  = 1.0f;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//4
//				pVertices->position.x = partSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = partSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = (int)v4temp.x - 0.5f;
//				pVertices->position.y = (int)v4temp.y - 0.5f;
//				pVertices->position.z = v4temp.z;
//				pVertices->rhw		  = 1.0f;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//5
//				pVertices->position.x = -partSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = -partSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = (int)v4temp.x - 0.5f;
//				pVertices->position.y = (int)v4temp.y - 0.5f;
//				pVertices->position.z = v4temp.z;
//				pVertices->rhw		  = 1.0f;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				//6			
//				pVertices->position.x = partSize.x * m_ScaleFactor.x / 2.0f;
//				pVertices->position.y = -partSize.y * m_ScaleFactor.y / 2.0f;
//				pVertices->position.z = 0.0f;
//				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
//				pVertices->position.x = (int)v4temp.x - 0.5f;
//				pVertices->position.y = (int)v4temp.y - 0.5f;
//				pVertices->position.z = v4temp.z;
//				pVertices->rhw		  = 1.0f;
//				pVertices->color = (DWORD)pPart->m_Color;
//				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
//				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
//				pVertices++;
//
//				KTDXPROFILE_END();
//
//				if( numParticlesInBatch == m_pParticleSystem->GetBatchSize() )
//				{
//					// Done filling this chunk of the vertex buffer.  Lets unlock and
//					// draw this portion so we can begin filling the next chunk.
//
//					KTDXPROFILE_BEGIN("Unlock");
//					pVB->Unlock();
//					KTDXPROFILE_END();
//
//					for( int dc = 0; dc < m_DrawCount; dc++ )
//					{
//						KTDXPROFILE_BEGIN("Draw");
//						if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 
//							m_pParticleSystem->GetOffsetRHWSize() * 6, numParticlesInBatch * 2)))
//						{
//							ErrorLog( KEM_ERROR32 );
//							return;
//						}
//						KTDXPROFILE_END();
//					}
//
//
//					m_pParticleSystem->SetOffsetRHWSize( m_pParticleSystem->GetOffsetRHWSize() + m_pParticleSystem->GetBatchSize() );
//
//					if(m_pParticleSystem->GetOffsetRHWSize() >= m_pParticleSystem->GetVBSize() ) 
//						m_pParticleSystem->SetOffsetRHWSize( 0 );
//
//
//
//					// Lock the next chunk of the vertex buffer.  If we are at the 
//					// end of the vertex buffer, DISCARD the vertex buffer and start
//					// at the beginning.  Otherwise, specify NOOVERWRITE, so we can
//					// continue filling the VB while the previous chunk is drawing.
//
//					KTDXPROFILE_BEGIN("Lock2");
//					if(FAILED(hr = pVB->Lock(m_pParticleSystem->GetOffsetRHWSize() * 6 * sizeof(VERTEX_PARTICLE_RHW), 
//						m_pParticleSystem->GetBatchSize() * 6 * sizeof(VERTEX_PARTICLE_RHW), 
//                        (void**)&pVertices, m_pParticleSystem->GetUseDynamicTexture() ? (m_pParticleSystem->GetOffsetRHWSize() ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD) : 0L )))
//					{
//						ErrorLogMsg( KEM_ERROR358, L"OnFrameRenderRHW2" );
//						return;
//					}
//					KTDXPROFILE_END();
//
//					numParticlesInBatch = 0;
//					renderParticle = 0;
//
//				}
//			}
//
//			// Unlock the vertex buffer
//			KTDXPROFILE_BEGIN("Unlock");
//			pVB->Unlock();
//			KTDXPROFILE_END();
//
//			// Render any remaining particles
//			if( numParticlesInBatch > 0 )
//			{
//				for( int dc = 0; dc < m_DrawCount; dc++ )
//				{
//					KTDXPROFILE_BEGIN("Draw2");
//					if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 
//						m_pParticleSystem->GetOffsetRHWSize() * 6, numParticlesInBatch * 2 )))
//					{
//						ErrorLog( KEM_ERROR33 );
//						return;
//					}
//					KTDXPROFILE_END();
//				}
//				numParticlesInBatch = 0;
//			}
//		}
//	}
////}}AFX
//	else
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
	{
//		HRESULT hr;

		VERTEX_PARTICLE_RHW*	pVertices;

		// Lock the vertex buffer.  We fill the vertex buffer in small
		// chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
		// each chunk, we call DrawPrim, and lock the next chunk.  When
		// we run out of space in the vertex buffer, we start over at
		// the beginning, using D3DLOCK_DISCARD.
		D3DXMATRIX finalMat;
		D3DXVECTOR4 v4temp;
		int iCurStart = 0;
		UINT renderParticle = 0;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		//텍스쳐 그룹화 해서 그린다
		map<int,CKTDXDeviceTexture*>::iterator iTex;
		for( iTex = m_TextureMap.begin() ; iTex != m_TextureMap.end() ; iTex++ )
		{		
			KTDXPROFILE_BEGIN("SetTexture");
			CKTDXDeviceTexture* pTex = (CKTDXDeviceTexture*)iTex->second;
			if( pTex != NULL )
				pTex->SetDeviceTexture();
			KTDXPROFILE_END();
			int texID = iTex->first;

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
            renderParticle = 0;
            vecVERTEX_PARTICLE_RHW.resize( 0 );
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//
//			KTDXPROFILE_BEGIN("Lock");
//			if( FAILED( hr = pVB->Lock( 0, iVBSize * 6 * sizeof(VERTEX_PARTICLE_RHW), (void**)&pVertices, 0 )))
//			{
//				return;
//			}
//			KTDXPROFILE_END();
//
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

			// Render each particle
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( m_iParticleList >= 0 && m_iParticleList < (int) coParticleList.storage_size() )
            for( KParticleHandleList::iterator iter = coParticleList.begin( m_iParticleList );
                iter != coParticleList.end( m_iParticleList ); ++iter)
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                KParticleHandleInfo& info = *iter;
                CParticle* pPart = info.m_pParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

				if ( pPart == NULL )
					continue;

				// Render each particle a bunch of times to get a blurring effect		
				if( texID != pPart->m_TextureID )
					continue;
				else
				{
					renderParticle++;
//#ifndef X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//					if( renderParticle >= m_ParticleList.size() )
//						iTex = m_TextureMap.end();
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
				}


				//1-----3,4
				//
				//2,5---6

				D3DXVECTOR3 partPos = pPart->m_vPos + pPart->GetCrash();
				pPart->SetCrash( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

				D3DXVECTOR3 partSize = pPart->m_vSize;

				KTDXPROFILE_BEGIN("MatrixMath");
				//해상도 변경 정보를 보간하고 싶으면 계산한다
				D3DXVECTOR2 temp;
				if( m_bResolutionConvert == true )
				{
					temp = g_pKTDXApp->ConvertByResolution( partPos.x, partPos.y );
					partPos.x = temp.x;
					partPos.y = temp.y;				
				}
				temp = g_pKTDXApp->ConvertByResolution( partSize.x, partSize.y );
				partSize.x = temp.x;
				partSize.y = temp.y;

//#ifdef LOCAL_ROTATE_EFFECT_P
				D3DXVECTOR3 vRotDegree = pPart->GetRotate() + pPart->GetRotateLocal();
//#else
//				D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x), D3DXToRadian(pPart->m_vRotate.y), D3DXToRadian(pPart->m_vRotate.z) );
//#endif
//			
				m_pMathMatrix->Move( partPos.x, partPos.y, partPos.z );
				m_pMathMatrix->RotateDegree( vRotDegree );
				//m_pMathMatrix->Scale( pPart->m_vSize.x, pPart->m_vSize.y, 1.0f );

				KTDXPROFILE_END();


				//finalMat = m_pMathMatrix->GetMatrix( m_BillBoardType );
				finalMat = m_pMathMatrix->GetMatrix( CKTDGMatrix::BT_NONE );



				D3DXVECTOR3 vScaleByUnit( 1, 1, 1 );
#ifdef GIANT_UNIT_GIANT_EFFECT_TEST

#ifdef VERIFY_STAT_BY_BUFF
				vScaleByUnit = GetVec3ScaleByUnit();
#else	// VERIFY_STAT_BY_BUFF
				vScaleByUnit = GetScaleByUnit();
#endif // VERIFY_STAT_BY_BUFF

#endif GIANT_UNIT_GIANT_EFFECT_TEST



				KTDXPROFILE_BEGIN("SetVertex");

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
                unsigned uSize = vecVERTEX_PARTICLE_RHW.size();
                vecVERTEX_PARTICLE_RHW.resize( uSize + 6 );
                pVertices = &vecVERTEX_PARTICLE_RHW[ uSize ];
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

				//1
				pVertices->position.x = -0.5f * partSize.x * m_ScaleFactor.x * vScaleByUnit.x;
				pVertices->position.y = 0.5f * partSize.y * m_ScaleFactor.y * vScaleByUnit.y;
				pVertices->position.z = 0.0f;
				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
				pVertices->position.x = (int)v4temp.x - 0.5f;
				pVertices->position.y = (int)v4temp.y - 0.5f;
				pVertices->position.z = v4temp.z;
				pVertices->rhw		  = 1.0f;
				pVertices->color = (DWORD)pPart->m_Color;
				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
				pVertices++;

				//2
				pVertices->position.x = -0.5f * partSize.x * m_ScaleFactor.x * vScaleByUnit.x;
				pVertices->position.y = -0.5f * partSize.y * m_ScaleFactor.y * vScaleByUnit.y;
				pVertices->position.z = 0.0f;
				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
				pVertices->position.x = (int)v4temp.x - 0.5f;
				pVertices->position.y = (int)v4temp.y - 0.5f;
				pVertices->position.z = v4temp.z;
				pVertices->rhw		  = 1.0f;
				pVertices->color = (DWORD)pPart->m_Color;
				pVertices->tex.x = 0.0f + pPart->GetTexStage0UV().x;
				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
				pVertices++;

				//3
				pVertices->position.x = 0.5f * partSize.x * m_ScaleFactor.x * vScaleByUnit.x;
				pVertices->position.y = 0.5f * partSize.y * m_ScaleFactor.y * vScaleByUnit.y;
				pVertices->position.z = 0.0f;
				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
				pVertices->position.x = (int)v4temp.x - 0.5f;
				pVertices->position.y = (int)v4temp.y - 0.5f;
				pVertices->position.z = v4temp.z;
				pVertices->rhw		  = 1.0f;
				pVertices->color = (DWORD)pPart->m_Color;
				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
				pVertices->tex.y = 1.0f + pPart->GetTexStage0UV().y;
				pVertices++;

				//4
				*pVertices = *(pVertices-1);
				pVertices++;

				//5
				*pVertices = *(pVertices-3);
				pVertices++;

				//6			
				pVertices->position.x = 0.5f * partSize.x * m_ScaleFactor.x * vScaleByUnit.x;
				pVertices->position.y = -0.5f * partSize.y * m_ScaleFactor.y * vScaleByUnit.y;
				pVertices->position.z = 0.0f;
				D3DXVec3Transform( &v4temp, &pVertices->position, &finalMat );
				pVertices->position.x = (int)v4temp.x - 0.5f;
				pVertices->position.y = (int)v4temp.y - 0.5f;
				pVertices->position.z = v4temp.z;
				pVertices->rhw		  = 1.0f;
				pVertices->color = (DWORD)pPart->m_Color;
				pVertices->tex.x = 1.0f + pPart->GetTexStage0UV().x;
				pVertices->tex.y = 0.0f + pPart->GetTexStage0UV().y;
				pVertices++;

				KTDXPROFILE_END();

//#ifndef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//
//				if( renderParticle == iVBSize )
//				{
//					// Done filling this chunk of the vertex buffer.  Lets unlock and
//					// draw this portion so we can begin filling the next chunk.
//
//					KTDXPROFILE_BEGIN("Unlock");
//					pVB->Unlock();
//					KTDXPROFILE_END();
//
//					for( int dc = 0; dc < m_DrawCount; dc++ )
//					{
//						KTDXPROFILE_BEGIN("Draw");
//						if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, renderParticle * 2)))
//						{
//							ErrorLog( KEM_ERROR32 );
//							return;
//						}
//						KTDXPROFILE_END();
//					}
//					renderParticle = 0;
//
//
//					// Lock the next chunk of the vertex buffer.  If we are at the 
//					// end of the vertex buffer, DISCARD the vertex buffer and start
//					// at the beginning.  Otherwise, specify NOOVERWRITE, so we can
//					// continue filling the VB while the previous chunk is drawing.
//
//					KTDXPROFILE_BEGIN("Lock2");
//					if(FAILED(hr = pVB->Lock(0, iVBSize * 6 * sizeof(VERTEX_PARTICLE_RHW), (void**) &pVertices, 0)))
//					{
//						return;
//					}
//					KTDXPROFILE_END();
//				}
//
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
			}


//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

            //BOOST_STATIC_ASSERT( D3DFVF_PARTICLE_RHW == CKTDGDynamicVBManager::D3DFVF_XYZRHW_DIFFUSE_TEX1 );
            if ( renderParticle > 0 )
            {
                g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
                    , D3DPT_TRIANGLELIST, renderParticle * 2, &vecVERTEX_PARTICLE_RHW[0], m_DrawCount );
            }//if

//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//
//			// Unlock the vertex buffer
//			KTDXPROFILE_BEGIN("Unlock");
//			pVB->Unlock();
//			KTDXPROFILE_END();
//
//			// Render any remaining particles
//			if( renderParticle > 0 )
//			{
//				for( int dc = 0; dc < m_DrawCount; dc++ )
//				{
//					KTDXPROFILE_BEGIN("Draw2");
//					if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, renderParticle * 2 )))
//					{
//						ErrorLog( KEM_ERROR33 );
//						return;
//					}
//					KTDXPROFILE_END();
//				}
//				renderParticle = 0;
//			}
//
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

		}
	}
}

void CKTDGParticleSystem::CParticleEventSequence::OnFrameRenderMesh_Draw()
{

	KTDXPROFILE();

    if ( m_pXMesh == NULL )
        return;

#ifdef X2OPTIMIZE_CULLING_PARTICLE
    CKTDGCamera& kCamera = g_pKTDXApp->GetDGManager()->GetCamera();
	const CKTDGFrustum& kFrustum = g_pKTDXApp->GetDGManager()->GetFrustum();
#endif//X2OPTIMIZE_CULLING_PARTICLE

	CKTDGXRenderer::RenderParam renderParam;

	// Render each particle
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
    if ( m_iParticleList >= 0 && m_iParticleList < (int) coParticleList.storage_size() )
    for( KParticleHandleList::iterator iter = coParticleList.begin( m_iParticleList );
        iter != coParticleList.end( m_iParticleList ); ++iter)
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        KParticleHandleInfo& info = *iter;
        CParticle* pPart = info.m_pParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		if ( pPart == NULL )
			continue;
		// Render each particle a bunch of times to get a blurring effect		
		pPart->SetPosCrash( pPart->GetPos() + pPart->GetCrash() );
		pPart->SetCrash( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

//#ifdef LOCAL_ROTATE_EFFECT_P
		D3DXVECTOR3 vRotDegree = pPart->GetRotate() + pPart->GetRotateLocal();
//#else
//		D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x), D3DXToRadian(pPart->m_vRotate.y), D3DXToRadian(pPart->m_vRotate.z) );
//#endif

		m_pMathMatrix->Move( pPart->m_vPosCrash.x, pPart->m_vPosCrash.y, pPart->m_vPosCrash.z );
		m_pMathMatrix->RotateDegree( vRotDegree );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
		m_pMathMatrix->Scale( pPart->m_vSize.x * m_ScaleFactor.x * GetScaleByUnit().x,
			pPart->m_vSize.y * m_ScaleFactor.y * GetScaleByUnit().y, 
			pPart->m_vSize.z * m_ScaleFactor.z * GetScaleByUnit().z );
#else GIANT_UNIT_GIANT_EFFECT_TEST
		m_pMathMatrix->Scale( pPart->m_vSize.x * m_ScaleFactor.x, pPart->m_vSize.y * m_ScaleFactor.y, pPart->m_vSize.z * m_ScaleFactor.z );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		renderParam.renderType			= m_MeshRenderType;
//{{ robobeg : 2008-10-24
		//renderParam.worldMatrix			= m_pMathMatrix->GetMatrix( m_BillBoardType );
        D3DXMATRIX  matWorld = m_pMathMatrix->GetMatrix( m_BillBoardType );
//}} robobeg : 2008-10-24

#ifdef NEW_HENIR_TEST
		if(m_bUseDXMatrix == true)
		{
			matWorld = m_pMathMatrix->GetScaleMatrix() * m_DXMATRIX;
		}
#endif NEW_HENIR_TEST
		
#ifdef X2OPTIMIZE_CULLING_PARTICLE
        if ( ms_bParticleCullingEnabled == true && m_bRenderCullCheck == true )
        {
		    D3DXVECTOR3 vCenter;
		    D3DXVECTOR3 vScale;

		    D3DXVec3TransformCoord( &vCenter, &m_pXMesh->GetCenter(), &matWorld );
            vScale = GetDecomposeScale( &matWorld ) * m_pXMesh->GetRadius();
            float fRadius = __max( vScale.x, vScale.y );
            fRadius = __max( fRadius, vScale.z );
		    if( kFrustum.CheckSphere( vCenter, fRadius ) == false )
			    continue;

		    //_DecideWorldMatrix_Mesh( matWorld, kCamera, vCenter, fRadius );
        }
#endif//X2OPTIMIZE_CULLING_PARTICLE

		renderParam.lightPos			= m_LightPos;
		renderParam.color				= pPart->m_Color;
		renderParam.cartoonTexType		= CKTDGXRenderer::CTT_NORMAL;
		if( renderParam.renderType == CKTDGXRenderer::RT_CARTOON_BLACK_EDGE 
			|| renderParam.renderType == CKTDGXRenderer::RT_CARTOON_COLOR_EDGE )
		{
			float fDistance = GetDistance(pPart->m_vPosCrash,g_pKTDXApp->GetDGManager()->GetCamera().GetEye());
			if( fDistance > 700.0f )
				renderParam.fOutLineWide = fDistance / 1000.0f;
			else
				renderParam.fOutLineWide = fDistance / 700.0f;
		}
		renderParam.bAlphaBlend			= true;
		renderParam.bZEnable			= m_bZEnable;
		renderParam.bZWriteEnable		= m_bZWriteEnable;
		renderParam.cullMode			= (D3DCULL)GetCullMode();		// 09.05.06 태완
		renderParam.srcBlend			= (D3DBLEND)GetSrcBlendMode();
		renderParam.destBlend			= (D3DBLEND)GetDestBlendMode();
		renderParam.texOffsetStage0	= pPart->GetTexStage0UV();
		renderParam.texOffsetStage1	= pPart->GetTexStage1UV();
		renderParam.texOffsetStage2 = pPart->GetTexStage2UV();
#ifdef ADD_ALPHATESTENABLE
		renderParam.bAlphaTestEnable = m_bAlphaTest;
#endif

        g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, matWorld, *m_pXMesh, m_pChangeTexXET, m_pMultiTexXET, m_pAniData, pPart->m_fEventTimer, m_DrawCount );

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
		_RenderBoundingSphere( (D3DXVECTOR3)vCenter, abs(vRadius.x) + abs(vRadius.y) + abs(vRadius.z) );
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	}
}


#ifdef SKINMESH_PARTICLE_TEST

	void CKTDGParticleSystem::CParticleEventSequence::OnFrameRenderSkinMesh_Draw()
	{
		KTDXPROFILE();

		// Render each particle
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
        if ( m_iParticleList >= 0 && m_iParticleList < (int) coParticleList.storage_size() )
        for( KParticleHandleList::iterator iter = coParticleList.begin( m_iParticleList );
            iter != coParticleList.end( m_iParticleList ); ++iter)
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	    BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            KParticleHandleInfo& info = *iter;
            CParticle* pPart = info.m_pParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			if ( pPart == NULL )
				continue;

			// Render each particle a bunch of times to get a blurring effect		
			ASSERT( pPart != NULL && pPart->m_pXSkinAnim != NULL );
            if ( pPart == NULL || pPart->m_pXSkinAnim == NULL )
                continue;

            //if ( !pPart->m_pXSkinAnim->GetShowObject() || !pPart->m_pXSkinAnim->IsInstantShow() )
            //    continue;

			CKTDGXRenderer::RenderParam* pRenderParam = pPart->m_pXSkinAnim->GetRenderParam();
            ASSERT( pRenderParam != NULL );

		    pPart->m_vPosCrash = pPart->m_vPos + pPart->GetCrash();
		    pPart->SetCrash( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

//#ifdef LOCAL_ROTATE_EFFECT_P
//			D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x+pPart->m_vRotateLocal.x), 
//				D3DXToRadian(pPart->m_vRotate.y+pPart->m_vRotateLocal.y), 
//				D3DXToRadian(pPart->m_vRotate.z+pPart->m_vRotateLocal.z) );
//#else
//			D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(pPart->m_vRotate.x), D3DXToRadian(pPart->m_vRotate.y), D3DXToRadian(pPart->m_vRotate.z) );
//#endif

			//m_pMathMatrix->Move( pPart->m_vPosCrash.x, pPart->m_vPosCrash.y, pPart->m_vPosCrash.z );
			//m_pMathMatrix->Rotate( vRot );
			//m_pMathMatrix->Scale( pPart->m_vSize.x * m_ScaleFactor.x, pPart->m_vSize.y * m_ScaleFactor.y, pPart->m_vSize.z * m_ScaleFactor.z );
			//pPart->m_pXSkinAnim->GetMatrix().Move( pPart->m_vPosCrash.x, pPart->m_vPosCrash.y, pPart->m_vPosCrash.z );
			//pPart->m_pXSkinAnim->GetMatrix().Rotate( vRot );
			//pPart->m_pXSkinAnim->GetMatrix().Scale( pPart->m_vSize.x * m_ScaleFactor.x, pPart->m_vSize.y * m_ScaleFactor.y, pPart->m_vSize.z * m_ScaleFactor.z );


			pRenderParam->renderType		= m_MeshRenderType;
			pRenderParam->lightPos			= m_LightPos;
			pRenderParam->color				= pPart->m_Color;
			pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
			if( pRenderParam->renderType == CKTDGXRenderer::RT_CARTOON_BLACK_EDGE ||
				pRenderParam->renderType == CKTDGXRenderer::RT_CARTOON_COLOR_EDGE )
			{
				float fDistance = GetDistance(pPart->m_vPosCrash,g_pKTDXApp->GetDGManager()->GetCamera().GetEye());
				if( fDistance > 700.0f )
					pRenderParam->fOutLineWide = fDistance / 1000.0f;
				else
					pRenderParam->fOutLineWide = fDistance / 700.0f;
			}
			pRenderParam->bAlphaBlend		= true;
			pRenderParam->bZEnable			= m_bZEnable;
			pRenderParam->bZWriteEnable		= m_bZWriteEnable;
			pRenderParam->cullMode			= (D3DCULL)GetCullMode();		// 09.05.06 태완
			pRenderParam->srcBlend			= (D3DBLEND)GetSrcBlendMode();
			pRenderParam->destBlend			= (D3DBLEND)GetDestBlendMode();
			pRenderParam->texOffsetStage0	= pPart->GetTexStage0UV();
			pRenderParam->texOffsetStage1	= pPart->GetTexStage1UV();
			pRenderParam->texOffsetStage2	= pPart->GetTexStage2UV();

			for( int i = 0; i < m_DrawCount; i++ )
			{
				pPart->m_pXSkinAnim->OnFrameRender_Draw();
			}
		}
	}

#endif SKINMESH_PARTICLE_TEST

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

CKTDGParticleSystem::CParticle*  CKTDGParticleSystem::GetParticle( CParticleHandle hParticle )
{
    if ( hParticle == INVALID_PARTICLE_HANDLE )
        return NULL;

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hParticle.GetValue() );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hParticle );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    if ( DecomposeHandle( dwHandle, wIndex, wStamp ) == false )
    {
        ASSERT( 0 );
        return NULL;
    }
    if ( wIndex >= m_coParticleList.storage_size() )
        return NULL;
    KParticleHandleInfo& info = m_coParticleList.data( wIndex );
    if ( info.m_wStamp != wStamp || info.m_pParticle == NULL )
        return NULL;
    return  info.m_pParticle;
}

CKTDGParticleSystem::CParticle*  CKTDGParticleSystem::CParticleEventSequence::GetParticle( CParticleHandle hParticle )
{
    if ( hParticle == INVALID_PARTICLE_HANDLE || m_iParticleList < 0 )
        return NULL;
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hParticle.GetValue() );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hParticle );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    if ( m_pParticleSystem->DecomposeHandle( dwHandle, wIndex, wStamp ) == false )
    {
        ASSERT( 0 );
        return NULL;
    }
    if ( wIndex >= coParticleList.storage_size() )
        return NULL;
    KParticleHandleInfo& info = coParticleList.data( wIndex );
    if ( info.m_wStamp != wStamp || info.m_pParticle == NULL || info.m_pParticle->GetMasterSequence() != this )
        return NULL;
    return  info.m_pParticle;
}

int			CKTDGParticleSystem::CParticleEventSequence::GetLiveParticleNum()
{
    if ( m_pParticleSystem == NULL || m_iParticleList < 0 )
    {
        return 0;
    }
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
    if ( m_iParticleList >= (int) coParticleList.storage_size() )
    {
        return 0;
    }
    return  coParticleList.size( m_iParticleList );
}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

void        CKTDGParticleSystem::CParticleEventSequence::ValidateParticlePointer( CParticle*& pParticle )
{
	if ( pParticle == NULL )
		return;

	if ( std::find( m_ParticleList.begin(), m_ParticleList.end(), pParticle ) ==  m_ParticleList.end() )
	{
		pParticle = NULL;
	}//if
}//CKTDGParticleSystem::CParticleEventSequence::ValidateParticlePointer()

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

CKTDGParticleSystem::CParticle* CKTDGParticleSystem::CParticleEventSequence::CreateNewParticle( D3DXVECTOR3 m_vPartSysPos )
{
	KTDXPROFILE();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
    KParticleHandleList::iterator iter;
    if ( m_iParticleList < 0 || m_iParticleList >= (int) coParticleList.storage_size() )
    {
        iter = coParticleList.begin( PARTICLELIST_FREE );
        if ( iter == coParticleList.end( PARTICLELIST_FREE ) )
        {
            if ( coParticleList.storage_size() >= 0x10000 )
                return NULL;
            m_iParticleList = coParticleList.push_back_default( PARTICLELIST_FREE ).GetIndex();
        }
        else
        {
            m_iParticleList = iter.GetIndex();
        }
        coParticleList.make_new_list( m_iParticleList );
    }
    iter = coParticleList.begin( PARTICLELIST_FREE );
    if ( iter == coParticleList.end( PARTICLELIST_FREE ) )
    {
        if ( coParticleList.storage_size() >= 0x10000 )
            return NULL;
        iter = coParticleList.push_back_default( PARTICLELIST_FREE );
    }
    KParticleHandleInfo& info = *iter;
    CParticle* pPart = info.m_pParticle;
    if ( pPart== NULL )
        info.m_pParticle = pPart = new CParticle;
    else
        pPart->Init();
    WORD   wIndex = (WORD) iter.GetIndex();
    CParticleHandle handle;
    DWORD   dwHandle = 0;
    do 
    {
        ++info.m_wStamp;
        dwHandle = m_pParticleSystem->ComposeHandle( wIndex, info.m_wStamp );
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        handle.SetValue( (int) dwHandle );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        handle = (int) dwHandle;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    } while ( handle == INVALID_PARTICLE_HANDLE );
    pPart->m_hHandle = handle;

    coParticleList.splice( coParticleList.end( m_iParticleList ), iter );

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CParticle* pPart = new CParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	pPart->m_pMasterSeq = this;	


	pPart->m_ScrewValue			= m_ScrewValue;
	pPart->m_ScrewRotateSpeed	= m_ScrewRotateSpeed;
	pPart->m_fLifetime			= m_Lifetime.GetRandomNumInRange();

#ifdef PARTICLE_SPHERICAL_EMIT_TEST
	if( 0.f != m_SphericalEmitRadius.m_Max )
	{
		float fRadial = m_SphericalEmitRadius.GetRandomNumInRange();
		float fAzimuth = m_SphericalEmitAzimuth.GetRandomNumInRange();		// 방위각
		float fPolar = m_SphericalEmitPolar.GetRandomNumInRange();			// 천정각

		D3DXVECTOR3 vEmitPositionOffset(0, 0, 0);
		vEmitPositionOffset.x = fRadial * cosf( fAzimuth ) * cosf( fPolar );
		vEmitPositionOffset.y = fRadial * sinf( fPolar );
		vEmitPositionOffset.z = fRadial * sinf( fAzimuth ) * cosf( fPolar );

		if( D3DXVECTOR3(0, 0, 0 ) != m_vSphericalEmitRotation )
		{
			D3DXMATRIX matRotation;
			D3DXMatrixRotationYawPitchRoll( &matRotation, m_vSphericalEmitRotation.y, m_vSphericalEmitRotation.x, m_vSphericalEmitRotation.z );
			D3DXVec3TransformCoord( &vEmitPositionOffset, &vEmitPositionOffset, &matRotation );
		}

		// emit radius도 axis angle외 회전함에 따라 회전 시킨다
		if( D3DXVECTOR3(0, 0, 0) != m_AxisAngleDegree )
		{
			D3DXMATRIX matRotate;
			D3DXVECTOR3 vRotate(0, 0, 0 ); 
			D3DXMatrixRotationYawPitchRoll( &matRotate, D3DXToRadian(m_AxisAngleDegree.y), D3DXToRadian(m_AxisAngleDegree.x), D3DXToRadian(m_AxisAngleDegree.z) );

			D3DXVec3TransformCoord( &vEmitPositionOffset, &vEmitPositionOffset, &matRotate);
		}

		pPart->m_vPos	= m_vPartSysPos + vEmitPositionOffset;
	}
	else
#endif PARTICLE_SPHERICAL_EMIT_TEST
	{
#ifdef PARTICLE_EMIT_REGION_TEST
		if( D3DXVECTOR3(0, 0, 0 ) != m_vEmitRadiusMin.m_Min || 
			D3DXVECTOR3(0, 0, 0 ) != m_vEmitRadiusMin.m_Max )
		{
			D3DXVECTOR3 vEmitPositionOffset(0, 0, 0);
			if( rand()%2 == 0 )
			{
				vEmitPositionOffset.x = RandomFloat( m_vEmitRadius.m_Min.x, m_vEmitRadiusMin.m_Min.x );
			}
			else
			{
				vEmitPositionOffset.x = RandomFloat( m_vEmitRadiusMin.m_Max.x, m_vEmitRadius.m_Max.x );
			}


			if( rand()%2 == 0 )
			{
				vEmitPositionOffset.y = RandomFloat( m_vEmitRadius.m_Min.y, m_vEmitRadiusMin.m_Min.y );
			}
			else
			{
				vEmitPositionOffset.y = RandomFloat( m_vEmitRadiusMin.m_Max.y, m_vEmitRadius.m_Max.y );
			}


			if( rand()%2 == 0 )
			{
				vEmitPositionOffset.z = RandomFloat( m_vEmitRadius.m_Min.z, m_vEmitRadiusMin.m_Min.z );
			}
			else
			{
				vEmitPositionOffset.z = RandomFloat( m_vEmitRadiusMin.m_Max.z, m_vEmitRadius.m_Max.z );
			}


			// emit radius도 axis angle외 회전함에 따라 회전 시킨다
			if( D3DXVECTOR3(0, 0, 0) != m_AxisAngleDegree )
			{
				D3DXMATRIX matRotate;
				D3DXVECTOR3 vRotate(0, 0, 0 ); 
				D3DXMatrixRotationYawPitchRoll( &matRotate, D3DXToRadian(m_AxisAngleDegree.y), D3DXToRadian(m_AxisAngleDegree.x), D3DXToRadian(m_AxisAngleDegree.z) );

				D3DXVec3TransformCoord( &vEmitPositionOffset, &vEmitPositionOffset, &matRotate);
			}

			pPart->m_vPos				= m_vPartSysPos + vEmitPositionOffset;
		}
		else
		{
			D3DXVECTOR3 vEmitPositionOffset = m_vEmitRadius.GetRandomNumInRange();

			// emit radius도 axis angle외 회전함에 따라 회전 시킨다
			if( D3DXVECTOR3(0, 0, 0) != m_AxisAngleDegree )
			{
				D3DXMATRIX matRotate;
				D3DXVECTOR3 vRotate(0, 0, 0 ); 
				D3DXMatrixRotationYawPitchRoll( &matRotate, D3DXToRadian(m_AxisAngleDegree.y), D3DXToRadian(m_AxisAngleDegree.x), D3DXToRadian(m_AxisAngleDegree.z) );

				D3DXVec3TransformCoord( &vEmitPositionOffset, &vEmitPositionOffset, &matRotate);
			}


			pPart->m_vPos				= m_vPartSysPos + vEmitPositionOffset;
		}
#else PARTICLE_EMIT_REGION_TEST
		//{{AFX
		pPart->m_vPos				= m_vPartSysPos + m_vEmitRadius.GetRandomNumInRange();
		if( m_vEmitRadiusMin.m_Max != m_vEmitRadiusMin.m_Min )
		{
			if( pPart->m_vPos.x > m_vEmitRadiusMin.m_Min.x && pPart->m_vPos.x < m_vEmitRadiusMin.m_Max.x )
			{
				if( fabs( pPart->m_vPos.x - m_vEmitRadiusMin.m_Min.x ) > fabs( pPart->m_vPos.x - m_vEmitRadiusMin.m_Max.x ) )
					pPart->m_vPos.x = m_vEmitRadiusMin.m_Min.x;
				else
					pPart->m_vPos.x = m_vEmitRadiusMin.m_Max.x;
			}

			if( pPart->m_vPos.y > m_vEmitRadiusMin.m_Min.y && pPart->m_vPos.y < m_vEmitRadiusMin.m_Max.y )
			{
				if( fabs( pPart->m_vPos.y - m_vEmitRadiusMin.m_Min.y ) > fabs( pPart->m_vPos.y - m_vEmitRadiusMin.m_Max.y ) )
					pPart->m_vPos.y = m_vEmitRadiusMin.m_Min.y;
				else
					pPart->m_vPos.y = m_vEmitRadiusMin.m_Max.y;
			}

			if( pPart->m_vPos.z > m_vEmitRadiusMin.m_Min.z && pPart->m_vPos.z < m_vEmitRadiusMin.m_Max.z )
			{
				if( fabs( pPart->m_vPos.z - m_vEmitRadiusMin.m_Min.z ) > fabs( pPart->m_vPos.z - m_vEmitRadiusMin.m_Max.z ) )
					pPart->m_vPos.z = m_vEmitRadiusMin.m_Min.z;
				else
					pPart->m_vPos.z = m_vEmitRadiusMin.m_Max.z;
			}
		}
		//}}AFX
#endif PARTICLE_EMIT_REGION_TEST
	}

	pPart->SetPosOrg( pPart->GetPos() );
	pPart->SetAxisRotateDegree( m_AxisAngleDegree );

	// process any initial events
	if ( m_pTempletSequence != NULL )
	{
		BOOST_TEST_FOREACH( CParticleEvent*, event, m_pTempletSequence->m_EventList )
		{
			if( NULL != event && event->GetActualTime().m_Min == 0.0f )
			{
				event->OnFrameMove( *pPart, 0.0f );
			}
		}
	}
	

	if( m_bUseLookPoint == true )
	{
		D3DXVECTOR3 dirVec;
		D3DXVec3Normalize( &dirVec, &(m_LookPoint - pPart->m_vPos) );

		D3DXMATRIX matRotation, matRotationOrth;

		D3DXMatrixLookAtLH( &matRotation, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &dirVec, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
		D3DXMatrixInverse( &matRotationOrth, NULL, &matRotation );

		float temp = matRotationOrth._11;
		matRotationOrth._11 = matRotationOrth._13;
		matRotationOrth._13 = temp;

		temp = matRotationOrth._21;
		matRotationOrth._21 = matRotationOrth._23;
		matRotationOrth._23 = temp;

		temp = matRotationOrth._31;
		matRotationOrth._31 = matRotationOrth._33;
		matRotationOrth._33 = temp;

		if( matRotationOrth._12 == 0 && matRotationOrth._22 == 0 )
		{
			dirVec.z	= 0.0f;
			dirVec.y	= D3DXToDegree(atan2f( -matRotationOrth._31, matRotationOrth._33 ));
			dirVec.x	= D3DXToDegree(atan2f( -matRotationOrth._21, matRotationOrth._11 ));
		}
		else
		{
			dirVec.z	= D3DXToDegree(asinf( matRotationOrth._32 ));
			dirVec.y	= D3DXToDegree(atan2f( -matRotationOrth._31, matRotationOrth._33 ));
			dirVec.x	= D3DXToDegree(atan2f( -matRotationOrth._12, matRotationOrth._22 ));
		}

		pPart->SetRotate( dirVec );
	}

	pPart->SetRotate( pPart->GetRotate() + m_AddRotate + m_AddRotateRel );

	for( int i = 0; i < (int)m_TraceSeqNameList.size(); i++ )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->CreateSequence( NULL, m_TraceSeqNameList[i].c_str(), pPart->m_vPos, -1, -1, -1, -1, -1.0f, true, 0.0f, true );

		if( NULL != pSeq )
		{
			pPart->m_TraceSeqList.push_back( pSeq->GetHandle() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
			if( m_bApplyUnitScale == true )
				pSeq->SetScaleByUnit( GetScaleByUnit() );
#else
			pSeq->SetScaleByUnit( GetScaleByUnit() );
#endif
#endif GIANT_UNIT_GIANT_EFFECT_TEST
			
		}

	}

#ifdef SKINMESH_PARTICLE_TEST
	if( PT_SKINMESH == m_ParticleType &&
		NULL != m_pXSkinMesh )
	{
		pPart->m_pXSkinAnim	= CKTDGXSkinAnim::CreateSkinAnimPtr();

		pPart->m_pXSkinAnim->SetAnimXSkinMesh( m_pXSkinMesh, m_pAniXET );
		pPart->m_pXSkinAnim->AddModelXSkinMesh( m_pXSkinMesh, m_pAniXET, m_pMultiTexXET, m_pChangeTexXET );

		pPart->m_pXSkinAnim->ChangeAnim( 0 );						// note!! 애니메이션을 선택할 수 있게 해야함, 일단은 첫번째 애니메이션을 플레이하도록 
		pPart->m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
	}
#endif SKINMESH_PARTICLE_TEST

	m_Count++;

#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_ParticleList.push_back( pPart );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	return pPart;
}

void CKTDGParticleSystem::CParticleEventSequence::OpenTexture()
{
	// process any initial events
	CParticleEvent* pEvent = NULL;
	CKTDXDeviceTexture* pTexture = NULL;
	int i = 0;
	if ( m_pTempletSequence != NULL )
	{
		BOOST_TEST_FOREACH( CParticleEvent*, pEvent, m_pTempletSequence->m_EventList )
		{
			switch( pEvent->GetEventType() )
			{
			case ET_TEXTURE:
				{
					CParticleEvent_Texture* pEventTexture = (CParticleEvent_Texture*)pEvent;
					pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( pEventTexture->GetTextureName() );
					m_TextureMap.insert( std::make_pair(i,pTexture) );
					pEventTexture->SetTextureID( i );
				}
				break;
			}
			i++;
		}
	}
	
}

void CKTDGParticleSystem::CParticleEventSequence::OpenMesh()
{
	m_pXMesh			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_MeshName );
	m_pChangeTexXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexXETName );
	m_pMultiTexXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_MultiTexXETName );
	m_pAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );
	m_pAniData			= NULL;
	if( m_pAniXET != NULL )
		m_pAniData		= m_pAniXET->GetAniData( m_AniName.c_str() );
}


#ifdef SKINMESH_PARTICLE_TEST
	void CKTDGParticleSystem::CParticleEventSequence::OpenSkinMesh()
	{
		m_pXSkinMesh		= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_MeshName );
		m_pChangeTexXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexXETName );
		m_pMultiTexXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_MultiTexXETName );
		m_pAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );
		m_pAniData			= NULL;
		if( m_pAniXET != NULL )
			m_pAniData		= m_pAniXET->GetAniData( m_AniName.c_str() );
	}
#endif SKINMESH_PARTICLE_TEST



#ifdef EQUIP_ATTACHED_PARTICLE_TRACE_TEST
// @bUpdateParticlePos: particle seq의 하위 파티클들의 위치도 particle seq가 옮겨진 만큼 같이 옮길지 말지;    TRACE == true 인 particle seq의 경우에 bUpdateOldPos == true로 해서 m_OldPosition을 덮어써줘야 정상동작한다.
void CKTDGParticleSystem::CParticleEventSequence::SetPosition( const D3DXVECTOR3& pos, bool bUpdateOldPos /*= false*/, bool bUpdateParticlePos /*= false*/ )
{ 
	if( true == bUpdateParticlePos )
	{
		const D3DXVECTOR3 vDisplace = pos - m_Position;
		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        auto AddDisplacementCB = [&vDisplace]( CParticle& particle )
        {
            particle.SetPos( particle.GetPos() + vDisplace );
        };
        ApplyFunctionToParticles( AddDisplacementCB );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	    BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
		{	
			if ( pParticle == NULL )
				continue;
            pParticle->m_vPos += vDisplace;
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	}
		
	m_Position = pos;
	if ( bUpdateOldPos == true )
		m_OldPosition = m_Position;
	GetMatrix().Move( m_Position ); 
}
#else EQUIP_ATTACHED_PARTICLE_TRACE_TEST
//{{AFX
void CKTDGParticleSystem::CParticleEventSequence::SetPosition( const D3DXVECTOR3& pos, bool bUpdateOldPos /*= false*/ )
{ 
	m_Position = pos;
	if ( bUpdateOldPos == true )
		m_OldPosition = m_Position;
	GetMatrix().Move( m_Position ); 
}
//}}AFX
#endif EQUIP_ATTACHED_PARTICLE_TRACE_TEST

void CKTDGParticleSystem::CParticleEventSequence::SetParticlePosition( const D3DXVECTOR3& pos )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    auto SetPositionCB = [&pos]( CParticle& particle )
    {
        particle.SetPos( pos );
    };
    ApplyFunctionToParticles( SetPositionCB );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
	{	
		if ( pParticle == NULL )
			continue;
		pParticle->m_vPos = pos;
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

}

void CKTDGParticleSystem::CParticleEventSequence::RunEvent( CParticle &part )
{
	KTDXPROFILE();

#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
    part.ResetVelocityToAccumPos();
    part.AddVelocityToAccumPos( m_fElapsedTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

    float   fOld = part.GetEventTimerOld();
    float   fNew = part.GetEventTimer();

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX

	if ( m_pTempletSequence != NULL )
	{
		BOOST_TEST_FOREACH( CParticleEvent*, pEvt, m_pTempletSequence->m_EventList )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
            float   fOld = part.GetEventTimerOld();
            float   fNew = part.GetEventTimer();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX

			if( pEvt->IsFade() == true )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                if ( fNew >= pEvt->GetActualTime().m_Min
                    && fOld < pEvt->GetActualTime().m_Max )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				if( fNew >= pEvt->GetActualTime().m_Min 
					&& fNew < pEvt->GetActualTime().m_Max )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				{
					pEvt->OnFrameMove( part, m_fElapsedTime );
				}
			}
			else
			{
				if( (fOld < pEvt->GetActualTime().m_Min 
					&& fNew >= pEvt->GetActualTime().m_Min)
					|| (pEvt->GetActualTime().m_Min == 0.0f && fOld == 0.0f ) )
				{
					pEvt->OnFrameMove( part, m_fElapsedTime );
				}
			}
		}
	}
	



	BOOST_TEST_FOREACH( CParticleEvent*, pEvt, m_EventList )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
        float   fOld = part.GetEventTimerOld();
        float   fNew = part.GetEventTimer();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX

		if( pEvt->IsFade() == true )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            if ( fNew >= pEvt->GetActualTime().m_Min
                && fOld < pEvt->GetActualTime().m_Max )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			if( fNew >= pEvt->GetActualTime().m_Min 
				&& fNew < pEvt->GetActualTime().m_Max )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			{
				pEvt->OnFrameMove( part, m_fElapsedTime );
			}
		}
		else
		{
			if( (fOld < pEvt->GetActualTime().m_Min 
				&& fNew >= pEvt->GetActualTime().m_Min)
				|| (pEvt->GetActualTime().m_Min == 0.0f && fOld == 0.0f ) )
			{
				pEvt->OnFrameMove( part, m_fElapsedTime );
			}
		}
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
    part.UpdateVelocityAccumPosAndEventTimer( part.GetEventTimer() );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
}

CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::CParticleEventSequence::GetCloneSequence()
{
	CParticleEventSequence* clone = new CParticleEventSequence( m_pParticleSystem, this );

#ifdef FIX_EFFECT_REFLECT
	if ( NULL == clone )
		return NULL;
#endif

	clone->SetParticleType( GetParticleType() );
	clone->SetName( GetName() );
	clone->SetPosition( GetPosition() );
	clone->SetLifetime( GetLifeTime() );
	clone->SetEmitRate( GetEmitRate() );
	clone->SetEmitRadius( GetEmitRadius() );
	clone->SetEmitRadiusMin( GetEmitRadiusMin() );

#ifdef PARTICLE_SPHERICAL_EMIT_TEST

	clone->SetSphericalEmitRadius( GetSphericalEmitRadius() );
	clone->SetSphericalEmitAzimuth( GetSphericalEmitAzimuth() );
	clone->SetSphericalEmitPolar( GetSphericalEmitPolar() );
	clone->SetSphericalEmitRotation( GetSphericalEmitRotation() );

#endif PARTICLE_SPHERICAL_EMIT_TEST

	clone->SetGravity( GetGravity() );
	clone->SetMaxParticleNum( GetMaxParticleNum() );
	clone->SetTrace( GetTrace() );
	clone->SetSrcBlendMode( GetSrcBlendMode() );
	clone->SetDestBlendMode( GetDestBlendMode() );
	clone->SetCullMode( GetCullMode() );			// 09.05.06 태완 : Cull mode 지정 가능하게
	clone->SetTriggerCount( GetTriggerCount() );
	clone->SetTriggerTime( GetTriggerTime() );
	clone->SetDrawCount( GetDrawCount() );
	clone->SetBillBoardType( GetBillBoardType() );
	clone->SetBlackHolePosition( GetBlackHolePosition() );
	clone->SetScrewValue( GetScrewValue() );
	clone->SetScrewRotateSpeed( GetScrewRotateSpeed() );
	clone->SetLandPosition( GetLandPosition() );
	clone->SetUseLand( GetUseLand() );
	clone->SetTrace( GetTrace() );
	clone->SetMeshName( GetMeshName() );
	clone->SetChangeTexXETName( GetChangeTexXETName() );
	clone->SetMultiTexXETName( GetMultiTexXETName() );
	clone->SetAniXETName( GetAniXETName() );
	clone->SetAniName( GetAniName() );
	clone->SetZWriteEnable( GetZWriteEnable() );
	clone->SetZEnable( GetZEnable() );
	clone->SetMeshRenderType( GetMeshRenderType() );
	clone->SetResolutionConvert( GetResolutionConvert() );
	clone->SetLightPos( GetLightPos() );
	clone->SetLayer( GetLayer() );
	clone->SetForceLayer( GetForceLayer() );
	clone->SetAddRotateRel( GetAddRotateRel() );

#ifdef PARTICLE_NOTAPPLY_UNITSCALE
	clone->SetApplyUnitScale( GetApplyUnitScale() );
#endif
#ifdef ADD_ALPHATESTENABLE
	clone->SetAlphTest( GetAlphTest() );
#endif

	clone->m_TraceSeqNameList = m_TraceSeqNameList;
	clone->m_FinalSeqNameList = m_FinalSeqNameList;

	switch( m_ParticleType )
	{
	case PT_MESH:
		{
			clone->OpenMesh();
		} break;

#ifdef SKINMESH_PARTICLE_TEST
	case PT_SKINMESH:
		{
			clone->OpenSkinMesh();
		} break;
#endif SKINMESH_PARTICLE_TEST

	default:
		{
			clone->OpenTexture();
		} break;
	}

	return clone;
}

void CKTDGParticleSystem::CParticleEventSequence::SetCameraZoomDistance( float fCameraZoom )
{ 
	D3DXVECTOR3 dir = g_pKTDXApp->GetDGManager()->GetCamera().GetEye() - m_Position;
	D3DXVec3Normalize( &dir, &dir );
	m_Position += dir * fCameraZoom;
	GetMatrix().Move( m_Position );
}

void CKTDGParticleSystem::CParticleEventSequence::ChangeTex( const WCHAR* pTexName )
{ 
	CKTDXDeviceTexture* pNewTex;
	pNewTex = g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexName );
	int key = 0;
	do 
	{
		key = rand() % 1000;
	}
	while( m_TextureMap.find(key) != m_TextureMap.end() );
	m_TextureMap.insert( std::make_pair(key,pNewTex) );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    auto ChangeTexCB = [key]( CParticle& particle )
    {
        particle.SetTextureID( key );
    };
    ApplyFunctionToParticles( ChangeTexCB );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
	{	
		if ( pParticle == NULL )
			continue;
		pParticle->SetTextureID( key );
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

void CKTDGParticleSystem::CParticleEventSequence::ChangeTexForce( const WCHAR* pTexName )
{ 
	CKTDXDeviceTexture* pNewTex = NULL;
	
	map<int,CKTDXDeviceTexture*>::iterator i;
	for( i = m_TextureMap.begin(); i != m_TextureMap.end(); i++ )
	{
		pNewTex = g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexName );
		CKTDXDeviceTexture* pTex = i->second;
		SAFE_CLOSE( pTex );
		i->second = pNewTex;
	}
}

#ifdef ELSWORD_SHEATH_KNIGHT
void CKTDGParticleSystem::CParticleEventSequence::ChangeSizeForce( const D3DXVECTOR3& vNewSize_ )
{ 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

    auto ChangeSizeForceCB = [&vNewSize_]( CParticle& particle )
    {
        particle.SetSize( vNewSize_ );
    };
    ApplyFunctionToParticles( ChangeSizeForceCB );

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
	{	
		if ( pParticle == NULL )
			continue;
		pParticle->m_vSize = vNewSize_;
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

void CKTDGParticleSystem::CParticleEventSequence::ChangeColorForce( const D3DXCOLOR& vColor_ )
{ 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

    auto ChangeColorForceCB = [&vColor_]( CParticle& particle ) 
    {
        particle.SetColor( vColor_ );
    };
    ApplyFunctionToParticles( ChangeColorForceCB );

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	BOOST_TEST_FOREACH( CParticle*, pPart, m_ParticleList )
	{	
		if ( pParticle == NULL )
			continue;
		pParticle->m_Color = vColor_;
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}
#endif ELSWORD_SHEATH_KNIGHT

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
CKTDGParticleSystem::CParticle* CKTDGParticleSystem::CParticleEventSequence::GetFrontParticle()
{
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
    if ( m_iParticleList >= 0 && m_iParticleList < (int) coParticleList.storage_size() )
    {
        KParticleHandleList::iterator iter = coParticleList.begin( m_iParticleList );
        if ( iter != coParticleList.end( m_iParticleList ) )
            return  iter->m_pParticle;
    }
    return NULL;
}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef BOOST_SINGLETON_POOL_TEST
	typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CKTDGParticleSystem::CParticleEventSequence)> CParticleEventSequencePool;

	void* CKTDGParticleSystem::CParticleEventSequence::operator new(size_t s)
	{
		return CParticleEventSequencePool::malloc();
	}

	void CKTDGParticleSystem::CParticleEventSequence::operator delete(void *p)
	{
		CParticleEventSequencePool::free(p);
	}
#endif BOOST_SINGLETON_POOL_TEST


///////////////////////////////////////////////////////////////////////////////
// CParticleEmitterTokenizer
///////////////////////////////////////////////////////////////////////////////

void CKTDGParticleSystem::CParticleEmitterTokenizer::Tokenize( const char *str, const int iLength /*= -1*/ )
{
	enum CurState 
	{
		InWhiteSpace = 1,	// 공백 \n
		InText,				// 공백, \n, ", --, // 이외의 TEXT
		InQuote,			// "
		InComment			// -- or //
	};

	CurState				cs		= InWhiteSpace;
	const char*				p		= str;
	CParticleEmitterToken	token;

//#ifdef _ENCRIPT_SCRIPT_
//	while( (*p) != 0 ) 
//#else
//	ASSERT( -1 != iLength ); 
//	int iCurrPos = 0;
//	while( (*p) != 0 && iCurrPos < iLength ) 
//#endif _ENCRIPT_SCRIPT_
    int iCurrPos = 0;
    while( ( iLength < 0 || iCurrPos < iLength ) && (*p) != 0 ) 
	{
		ASSERT( InComment == cs || *p != ';' );


		switch( cs ) 
		{
		case InWhiteSpace:
			{
				// if this letter is not whitespace,
				if( !isspace(*p) ) 
				{
					// add it to the running buffer
					token.m_strValue = (*p);
					// switch to appropriate case
					if( (*p) == '/' && ( iLength < 0 || iCurrPos +1 < iLength ) && *(p+1) == '/' ) // '//'는 주석
						cs = InComment;
					else if( (*p) == '-' && ( iLength < 0 || iCurrPos +1 < iLength ) && *(p+1) == '-' ) // '--'도 주석
						cs = InComment;
					else 
						cs = (*p == '\"') ? InQuote : InText;
				} // end if
			}
			break;

		case InText:
			{
				// if this letter is whitespace
				if( isspace(*p) ) 
				{
					// add the completed token to the vector
					AddToken(token);
					// switch to whitespace case
					cs = InWhiteSpace;
				} 
				else
				{ 
					// if this letter is a token terminator
					if ((*p) == '(' || (*p) == ')' || (*p) == ',' || (*p) == '\"' || (*p) == '{' || (*p) == '}' || (*p) == '/') 
					{
						if ((*p) == '/' && ( iLength < 0 || iCurrPos +1 < iLength ) && *(p+1) == '/') 
							cs = InComment;
						else 
						{
							// add the completed token to the vector
							DetermineTokenType(token);	// not need
							AddToken(token);

							// if it was a quote, transition to InQuote state
							if ((*p) == '\"') 
								cs = InQuote; 
							// otherwise, process this one char as a token 
							else 
							{
								token.m_strValue = (*p);
								AddToken(token);
								token.m_strValue = "";	
							}
						}
					} 
					else
					{
						// add this letter to the work in progress token
						token.m_strValue += (*p);
					} // end if
				} // end if
			}
			break;

		case InComment:
			{
				// C++ style comments - skip everything until end of line
				if (*p == '\n') 
				{
					token.m_strValue = ""; 
					cs = InWhiteSpace;
				}
			}
			break;

		case InQuote:
			{
				// unconditionally add this letter to the token until we hit a close quote
				token.m_strValue += (*p);
				if (*p == '\"') 
				{
					AddToken(token);
					// go back to whitespace
					cs = InWhiteSpace;
				}
			}
			break;
		}
		p++;

//#ifdef _ENCRIPT_SCRIPT_
//#else
//		iCurrPos++;
//#endif _ENCRIPT_SCRIPT_
        iCurrPos++;

	}
	AddToken(token);
}

void CKTDGParticleSystem::CParticleEmitterTokenizer::AddToken( CParticleEmitterToken token )
{
	if( token.m_strValue.size() ) 
	{
		DetermineTokenType( token );
		m_TokenVector.push_back( token );
	}
}

void CKTDGParticleSystem::CParticleEmitterTokenizer::DetermineTokenType( CParticleEmitterToken &token )
{
	token.m_Type = UnknownToken;

	// these things are easy to see...
	if (isdigit(token.m_strValue[0]) || token.m_strValue[0] == '-') { token.m_Type = RealNumber; return; }
	if (token.m_strValue[0] == '=')   { token.m_Type = Equals; return; }
	if (token.m_strValue[0] == ',')   { token.m_Type = Comma; return; }
	if (token.m_strValue[0] == '(')   { token.m_Type = OpenParen; return; }
	if (token.m_strValue[0] == ')')   { token.m_Type = CloseParen; return; }
	if (token.m_strValue[0] == '{')   { token.m_Type = OpenBrace; return; }
	if (token.m_strValue[0] == '}')   { token.m_Type = CloseBrace; return; }
	if (token.m_strValue[0] == '\"')  { token.m_Type = Quote; return; }

	// if we got here, it's not a quote... so convert it to uppercase.
	MakeUpperCase(token.m_strValue);

	// keywords are easy to figure out, just look for the text...
	if (token.m_strValue.compare("PARTICLESYSTEM")		== 0) { token.m_Type = KeywordParticleSystem;	return; }
	if (token.m_strValue.compare("EVENTSEQUENCE")		== 0) { token.m_Type = KeywordEventSequence;	return; }
	if (token.m_strValue.compare("RANDOM")				== 0) { token.m_Type = KeywordRandom;			return; }
	if (token.m_strValue.compare("XYZ")					== 0) { token.m_Type = KeywordXYZ;				return; }
	if (token.m_strValue.compare("RGBA")				== 0) { token.m_Type = KeywordColor;			return; }
	if (token.m_strValue.compare("FADE")				== 0) { token.m_Type = KeywordFade;				return; }

	if (token.m_strValue.compare("INITIAL")				== 0) { token.m_Type = KeywordInitial;			return; }
	if (token.m_strValue.compare("FINAL")				== 0) { token.m_Type = KeywordFinal;			return; }
	if (token.m_strValue.compare("FROM")				== 0) { token.m_Type = KeywordFrom;				return; }

	// these two keywords are embedded in other words, so we've got to be careful.
	if (token.m_strValue.compare("XY")					== 0) { token.m_Type = KeywordXY; return; }
	if (token.m_strValue.compare("SO")					== 0) { token.m_Type = KeywordSo; return; }
	if (token.m_strValue.compare("AT")					== 0) { token.m_Type = KeywordAt; return; }
	if (token.m_strValue.compare("TO")					== 0) { token.m_Type = KeywordTo; return; }

	// now it gets harder... 
	if (token.m_strValue.find("D3DBLEND_")			== 0) { token.m_Type = AlphaBlendMode; return; }
	if (token.m_strValue.compare("SOURCEBLENDMODE")		== 0 ||
		token.m_strValue.compare("DESTBLENDMODE")		== 0) 
	{ 
		token.m_Type = SeqAlphaBlendModeProp; return; 
	}

	//09.05.06 태완 : Cull Mode 설정 가능하게 추가
	if (token.m_strValue.find("D3DCULL_")			== 0) { token.m_Type = CullMode; return; }
	if (token.m_strValue.compare("CULLMODE")		== 0)		
	{ 
		token.m_Type = SeqCullModeProp; return; 
	}
	

	if (token.m_strValue.compare("PARTICLETYPE")		== 0) 
	{ 
		token.m_Type = SeqParticleTypeProp; return; 
	}

	if (token.m_strValue.compare("MESHRENDERTYPE")		== 0) 
	{ 
		token.m_Type = SeqMeshRenderTypeProp; return; 
	}

	if (token.m_strValue.compare("BILLBOARDTYPE")		== 0) 
	{ 
		token.m_Type = SeqBillboardTypeProp; return; 
	}

	if (token.m_strValue.compare("USELAND")				== 0 ||
		token.m_strValue.compare("TRACE")				== 0 ||
		token.m_strValue.compare("ZWRITEENABLE")		== 0 ||
		token.m_strValue.compare("ZENABLE")				== 0 ||
		token.m_strValue.compare("RESOLUTIONCONVERT")	== 0 ||
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
		token.m_strValue.compare("APPLYUNITSCALE")		== 0 ||
#endif
#ifdef ADD_ALPHATESTENABLE
		token.m_strValue.compare("ALPHATEST")		== 0 ||
#endif
		token.m_strValue.compare("FORCELAYER")	== 0 ) 
	{ 
		token.m_Type = SeqBoolProp; return; 
	}

	if (token.m_strValue.compare("MESH")				== 0 ||
		token.m_strValue.compare("SKINMESH")			== 0 ||
		token.m_strValue.compare("CHANGETEXXET")		== 0 ||
		token.m_strValue.compare("MULTITEXXET")			== 0 ||
		token.m_strValue.compare("ANIXET")				== 0 ||
		token.m_strValue.compare("TRACEPARTICLELIST")	== 0 ||
		token.m_strValue.compare("FINALPARTICLELIST")	== 0 ||
		token.m_strValue.compare("ANINAME")				== 0) 
	{ 
		token.m_Type = SeqStringProp; return; 
	}

	if (token.m_strValue.compare("LIFETIME")			== 0 ||
		token.m_strValue.compare("EMITRATE")			== 0 ||
#ifdef PARTICLE_SPHERICAL_EMIT_TEST
		token.m_strValue.compare("SPHERICAL_EMITRADIUS")	== 0 ||
		token.m_strValue.compare("SPHERICAL_EMITAZIMUTH")	== 0 ||
		token.m_strValue.compare("SPHERICAL_EMITPOLAR")		== 0 ||
#endif PARTICLE_SPHERICAL_EMIT_TEST
		token.m_strValue.compare("NUMPARTICLES")		== 0 ||
		token.m_strValue.compare("TRIGGERCOUNT")		== 0 ||
		token.m_strValue.compare("TRIGGERTIME")			== 0 ||
		token.m_strValue.compare("DRAWCOUNT")			== 0 ||
		token.m_strValue.compare("LANDPOS")				== 0 ||
		token.m_strValue.compare("LAYER")				== 0 ||
		token.m_strValue.compare("SCREWVALUE")			== 0 ||
		token.m_strValue.compare("SCREWROTATESPEED")	== 0)
	{ 
		token.m_Type = SeqNumericProp; return; 
	}

	if (token.m_strValue.compare("GRAVITY")				== 0 || 
		token.m_strValue.compare("EMITRADIUS")			== 0 ||
		token.m_strValue.compare("EMITRADIUSMIN")		== 0 ||
#ifdef PARTICLE_SPHERICAL_EMIT_TEST
		token.m_strValue.compare("SPHERICAL_EMITROTATION")	== 0 ||
#endif PARTICLE_SPHERICAL_EMIT_TEST
		token.m_strValue.compare("BLACKHOLEPOS")		== 0 ||
		token.m_strValue.compare("LIGHTPOS")			== 0 ||
		token.m_strValue.compare("ADDROTATEREL")		== 0 ) 
	{
		token.m_Type = SeqVectorProp; return;
	}

	if (token.m_strValue.compare("DIRSPEED")			== 0 ||
		token.m_strValue.compare("BLACKHOLETIME")		== 0 ||
		token.m_strValue.compare("EVENTTIMER")			== 0 ||
		token.m_strValue.compare("RESETCRASH")			== 0 ||
#ifdef PARTICLE_STRETCH_TEST
		token.m_strValue.compare("STRETCH")				== 0 ||
#endif PARTICLE_STRETCH_TEST
		token.m_strValue.compare("SCALE")				== 0 ) 
	{
		token.m_Type = ParticleNumericProp; return;
	}

	if (token.m_strValue.compare("TEX0UV")			== 0 ||
		token.m_strValue.compare("TEX1UV")			== 0 ||
		token.m_strValue.compare("TEX2UV")			== 0) 
	{
		token.m_Type = ParticleVector2Prop; return;
	}

	if (token.m_strValue.compare("VELOCITY")		== 0 ||
		token.m_strValue.compare("POSITION")		== 0 ||
		token.m_strValue.compare("ROTATE")			== 0 ||
		token.m_strValue.compare("SIZE")			== 0 ||
		token.m_strValue.compare("CRASH")			== 0 )
	{
		token.m_Type = ParticleVectorProp; return;
	}

	if (token.m_strValue.compare("COLOR")			== 0) 
	{
		token.m_Type = ParticleColorProp; return;
	}

	if (token.m_strValue.compare("TEXTURE")			== 0) 
	{
		token.m_Type = ParticleStringProp; return;
	}
}

const int CKTDGParticleSystem::CParticleEmitterTokenizer::NUMBLENDINGMODES = 15;
const CKTDGParticleSystem::BLENDINGMODE CKTDGParticleSystem::CParticleEmitterTokenizer::m_BlendingModes[15] = 
{
	{ "D3DBLEND_ZERO",			D3DBLEND_ZERO			},
	{ "D3DBLEND_ONE",			D3DBLEND_ONE			},
	{ "D3DBLEND_SRCCOLOR",		D3DBLEND_SRCCOLOR		},
	{ "D3DBLEND_INVSRCCOLOR",	D3DBLEND_INVSRCCOLOR	},
	{ "D3DBLEND_SRCALPHA",		D3DBLEND_SRCALPHA		},
	{ "D3DBLEND_INVSRCALPHA",	D3DBLEND_INVSRCALPHA	},
	{ "D3DBLEND_DESTALPHA",		D3DBLEND_DESTALPHA		},
	{ "D3DBLEND_INVDESTALPHA",	D3DBLEND_INVDESTALPHA	},
	{ "D3DBLEND_DESTCOLOR",		D3DBLEND_DESTCOLOR		},
	{ "D3DBLEND_INVDESTCOLOR",	D3DBLEND_INVDESTCOLOR	},
	{ "D3DBLEND_SRCALPHASAT",	D3DBLEND_SRCALPHASAT	},
	{ "D3DBLEND_BOTHSRCALPHA",	D3DBLEND_BOTHSRCALPHA	},
	{ "D3DBLEND_BOTHINVSRCALPHA",	D3DBLEND_BOTHINVSRCALPHA	},
	{ "D3DBLEND_BLENDFACTOR",	D3DBLEND_BLENDFACTOR	},	
	{ "D3DBLEND_INVBLENDFACTOR",D3DBLEND_INVBLENDFACTOR },
};

//{{ 09.05.06 태완 : Cull mode 지정 가능하게
const int CKTDGParticleSystem::CParticleEmitterTokenizer::NUMCULLMODES = 4;
const CKTDGParticleSystem::CULLMODE CKTDGParticleSystem::CParticleEmitterTokenizer::m_CullModes[4] = 
{
	{ "D3DCULL_NONE",			D3DCULL_NONE			},
	{ "D3DCULL_CW",				D3DCULL_CW				},
	{ "D3DCULL_CCW",			D3DCULL_CCW				},
	{ "D3DCULL_FORCE_DWORD",	D3DCULL_FORCE_DWORD		}
};
//}} 09.05.06 태완 : Cull mode 지정 가능하게

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessNumber( CMinMax<float> &number, std::vector<CParticleEmitterToken>::iterator &TokenIter,
																	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// the first token is either the random keyword, or it's an actual number.
	switch(TokenIter->m_Type) 
	{
		case KeywordRandom:
			{
				// parse random number into minmax
				NextToken(TokenIter, EndIter);
				if (TokenIter->m_Type != OpenParen) throw("Expecting opening paren after Random keyword.");
				NextToken(TokenIter, EndIter);
				if (TokenIter->m_Type != RealNumber) throw("Expecting first number within Random(...).");
				number.m_Min = (float)atof(TokenIter->m_strValue.c_str());
				NextToken(TokenIter, EndIter);

				if (TokenIter->m_Type != Comma) throw("Expecting comma within Random(...).");
				NextToken(TokenIter, EndIter);

				if (TokenIter->m_Type != RealNumber) throw("Expecting second number within Random(...).");
				number.m_Max = (float)atof(TokenIter->m_strValue.c_str());
				NextToken(TokenIter, EndIter);
				if (TokenIter->m_Type != CloseParen) throw("Missing close paren on Random(...).");
				NextToken(TokenIter, EndIter);
			}
			break;

		case RealNumber:
			// min and max both equal realnumber
			if (TokenIter->m_Type != RealNumber) throw("Expecting number.");

			number.m_Max = number.m_Min = (float)atof(TokenIter->m_strValue.c_str());
			NextToken(TokenIter, EndIter);
			break;

		default: throw("Expecting either Random(...) or a number value.");
	}
	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessVector(CMinMax<D3DXVECTOR3> &v, 
											  std::vector<CParticleEmitterToken>::iterator &TokenIter,
											  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// this token needs to be a XYZ keyword.
	if (TokenIter->m_Type != KeywordXYZ) throw("Expecting XYZ(...)!");
	NextToken(TokenIter, EndIter);

	if (TokenIter->m_Type != OpenParen) throw("Expecting ( after XYZ!");

	CMinMax<float> x;
	CMinMax<float> y;
	CMinMax<float> z;

	NextToken(TokenIter, EndIter); ProcessNumber(x, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) throw("Vector components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(y, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) throw("Vector components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(z, TokenIter, EndIter);
	if (TokenIter->m_Type != CloseParen) throw("Expecting ) to close vector.");
	NextToken(TokenIter, EndIter);

	v.m_Min = D3DXVECTOR3(x.m_Min, y.m_Min, z.m_Min);
	v.m_Max = D3DXVECTOR3(x.m_Max, y.m_Max, z.m_Max);

	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessVector2(CMinMax<D3DXVECTOR2> &v, 
																   std::vector<CParticleEmitterToken>::iterator &TokenIter,
																   std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// this token needs to be a XYZ keyword.
	if (TokenIter->m_Type != KeywordXY) throw("Expecting XY(...)!");
	NextToken(TokenIter, EndIter);

	if (TokenIter->m_Type != OpenParen) throw("Expecting ( after XY!");

	CMinMax<float> x;
	CMinMax<float> y;

	NextToken(TokenIter, EndIter); ProcessNumber(x, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) throw("Vector components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(y, TokenIter, EndIter);
	if (TokenIter->m_Type != CloseParen) throw("Expecting ) to close vector.");
	NextToken(TokenIter, EndIter);

	v.m_Min = D3DXVECTOR2(x.m_Min, y.m_Min);
	v.m_Max = D3DXVECTOR2(x.m_Max, y.m_Max);

	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessColor(CMinMax<D3DXCOLOR> &c, 
											 std::vector<CParticleEmitterToken>::iterator &TokenIter,
											 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// this token needs to be a RGBA keyword.
	if (TokenIter->m_Type != KeywordColor)
		throw("Expecting RGBA(...)!");
	NextToken(TokenIter, EndIter);

	if (TokenIter->m_Type != OpenParen) throw("Expecting ( after RGBA!");

	CMinMax<float> r;
	CMinMax<float> g;
	CMinMax<float> b;
	CMinMax<float> a;

	NextToken(TokenIter, EndIter); ProcessNumber(r, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) throw("Color components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(g, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) throw("Color components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(b, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) throw("Color components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(a, TokenIter, EndIter);
	if (TokenIter->m_Type != CloseParen) throw("Expecting ) to close vector.");
	NextToken(TokenIter, EndIter);

	c.m_Min = D3DXCOLOR(r.m_Min, g.m_Min, b.m_Min, a.m_Min);
	c.m_Max = D3DXCOLOR(r.m_Max, g.m_Max, b.m_Max, a.m_Max);

	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessAlphaBlendMode(int &alphablendmode, 
													  std::vector<CParticleEmitterToken>::iterator &TokenIter,
													  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_Type != AlphaBlendMode)
		throw("Expecting alpha blend mode (D3DBLEND_ZERO, D3DBLEND_ONE, etc.).");
	for (int q=0; q < NUMBLENDINGMODES; q++) 
	{
		// careful here... must match on both string and size 
		// (i.e., to differentiate between D3DBLEND_SRCALPHA and D3DBLEND_SRCALPHASAT).
		if (TokenIter->m_strValue.find(m_BlendingModes[q].name) != std::string::npos && 
			TokenIter->m_strValue.size() == strlen(m_BlendingModes[q].name)) 
		{
			alphablendmode = m_BlendingModes[q].mode;
			NextToken(TokenIter, EndIter);
			return(true);
		}
	}

	throw("Invalid alpha blending mode!");
	return(false);
}

//{{ 09.05.06 태완 : Cull mode 지정 가능하게
bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessCullMode(int &cullmode, 
																		   std::vector<CParticleEmitterToken>::iterator &TokenIter,
																		   std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_Type != CullMode)
		throw("Expecting cull mode (D3DCULL_NONE, D3DCULL_CCW, etc.).");
	for (int q=0; q < NUMCULLMODES; q++) 
	{
		// Copy & Paste of AlphaBlendMode Code
		if (TokenIter->m_strValue.find(m_CullModes[q].name) != std::string::npos && 
			TokenIter->m_strValue.size() == strlen(m_CullModes[q].name)) 
		{
			cullmode = m_CullModes[q].mode;
			NextToken(TokenIter, EndIter);
			return(true);
		}
	}

	throw("Invalid cull mode!");
	return(false);
}
//}} 09.05.06 태완 : Cull mode 지정 가능하게

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessParticleType(PARTICLE_TYPE &particleType, 
																		   std::vector<CParticleEmitterToken>::iterator &TokenIter,
																		   std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if( TokenIter->m_strValue.compare( "PT_3D_PLANE" ) == 0 )
	{
		particleType = PT_3D_PLANE;
	}
	else if( TokenIter->m_strValue.compare( "PT_2D_PLANE" ) == 0 )
	{
		particleType = PT_2D_PLANE;
	}
	else if( TokenIter->m_strValue.compare( "PT_MESH" ) == 0 )
	{
		particleType = PT_MESH;
	}
#ifdef SKINMESH_PARTICLE_TEST
	else if( TokenIter->m_strValue.compare( "PT_SKINMESH" ) == 0 )
	{
		particleType = PT_SKINMESH;
	}
#endif SKINMESH_PARTICLE_TEST	
	else
	{
		throw("Invalid particle type!");
		return(false);
	}

	NextToken(TokenIter, EndIter);
	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessMeshRenderType(CKTDGXRenderer::RENDER_TYPE &renderType, 
																		 std::vector<CParticleEmitterToken>::iterator &TokenIter,
																		 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if( TokenIter->m_strValue.compare( "RT_REAL_COLOR" ) == 0 )
	{
		renderType = CKTDGXRenderer::RT_REAL_COLOR;
	}
	else if( TokenIter->m_strValue.compare( "RT_CARTOON" ) == 0 )
	{
		renderType = CKTDGXRenderer::RT_CARTOON;
	}
	else if( TokenIter->m_strValue.compare( "RT_CARTOON_BLACK_EDGE" ) == 0 )
	{
		renderType = CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	}
	else if( TokenIter->m_strValue.compare( "RT_CARTOON_COLOR_EDGE" ) == 0 )
	{
		renderType = CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
	}	
	else
	{
		throw("Invalid mesh render type!");
		return(false);
	}

	NextToken(TokenIter, EndIter);
	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessBillboardType(CKTDGMatrix::BILLBOARD_TYPE &billboardType, 
																		 std::vector<CParticleEmitterToken>::iterator &TokenIter,
																		 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if( TokenIter->m_strValue.compare( "BT_ALL" ) == 0 )
	{
		billboardType = CKTDGMatrix::BT_ALL;
	}
	else if( TokenIter->m_strValue.compare( "BT_NONE" ) == 0 )
	{
		billboardType = CKTDGMatrix::BT_NONE;
	}
	else if( TokenIter->m_strValue.compare( "BT_X" ) == 0 )
	{
		billboardType = CKTDGMatrix::BT_X;
	}
	else if( TokenIter->m_strValue.compare( "BT_Y" ) == 0 )
	{
		billboardType = CKTDGMatrix::BT_Y;
	}
	else if( TokenIter->m_strValue.compare( "BT_Z" ) == 0 )
	{
		billboardType = CKTDGMatrix::BT_Z;
	}
	else
	{
		throw("Invalid billboard type!");
		return(false);
	}

	NextToken(TokenIter, EndIter);
	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessBool(bool &bValue, 
																std::vector<CParticleEmitterToken>::iterator &TokenIter,
																std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if( TokenIter->m_strValue.compare( "TRUE" ) == 0 )
	{
		bValue = true;
	}
	else if( TokenIter->m_strValue.compare( "FALSE" ) == 0 )
	{
		bValue = false;
	}
	else
	{
		throw("Invalid bool!");
		return(false);
	}

	NextToken(TokenIter, EndIter);
	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessString(wstring &str, 
																 std::vector<CParticleEmitterToken>::iterator &TokenIter,
																 std::vector<CParticleEmitterToken>::iterator &EndIter)
{

	ConvertCharToWCHAR( str, TokenIter->m_strValue.c_str() );


	NextToken(TokenIter, EndIter);
	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessTime(CMinMax<float> &TimeRange,
											bool &IsFade, const float InitialTime, const float FinalTime,
											std::vector<CParticleEmitterToken>::iterator &TokenIter, 
											std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// determine if this time specification is a fade directive
	if (TokenIter->m_Type == KeywordFade) 
	{
		// it is... the next token must be "so"
		NextToken(TokenIter, EndIter); 

		if (TokenIter->m_Type != KeywordSo) 
			throw("Expecting \"so\" after \"fade\".");

		// flip the fade flag on
		IsFade = true;

		// move to next token (so that we land on "at" for the code below)
		NextToken(TokenIter, EndIter);
	}
	else 
		IsFade = false; // just to be safe

	switch(TokenIter->m_Type) 
	{
		case KeywordFrom:
			{
				// easy, just grab the time
				NextToken(TokenIter, EndIter);
				ProcessNumber(TimeRange, TokenIter, EndIter);

				float startTime = TimeRange.m_Min;

				if (TokenIter->m_Type != KeywordTo)
					throw("Expecting \"To\" after \"From\".");

				// easy, just grab the time
				NextToken(TokenIter, EndIter);
				ProcessNumber(TimeRange, TokenIter, EndIter);

				TimeRange.m_Min = startTime;

                ASSERT( TimeRange.m_Max - TimeRange.m_Min > (1/70) );

			}
			break;

		case KeywordAt:
			{
				// easy, just grab the time
				NextToken(TokenIter, EndIter);
				ProcessNumber(TimeRange, TokenIter, EndIter);
			}
			break;

		case KeywordInitial:
			{
				// use initial time that was passed in.
				if (IsFade) throw("Cannot use \"fade so\" modifier on \"initial\" times.");
				TimeRange.m_Min = InitialTime;
				TimeRange.m_Max = InitialTime;
				CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			}
			break;

		case KeywordFinal:
			{
				// use final time that was passed in.
				TimeRange.m_Min = InitialTime;
				TimeRange.m_Max = FinalTime;
				CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			}
			break;

		default: throw("Expected time specifier: \"[fade so] at\", \"initial\", or \"[fade so] final\"");
	}

	return(true);
}


//#ifdef	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD
bool	CKTDGParticleSystem::AppendToDeviceList( CKTDXDeviceDataList& listInOut_, const WCHAR* pSequenceName, CKTDXDeviceManager::EPriority ePriority )
{
	if ( pSequenceName == NULL || pSequenceName[0] == NULL )
		return false;
	return	AppendToDeviceList( listInOut_, std::wstring(pSequenceName), ePriority );
}

bool	CKTDGParticleSystem::AppendToDeviceList( CKTDXDeviceDataList& listInOut_, const wstring& wstrSequenceName, CKTDXDeviceManager::EPriority ePriority )
{
	if ( wstrSequenceName.empty() == false )
		return false;
	const CKTDGParticleSystem::CParticleEventSequence*	pTemplateSequence = GetTempletSequencesByName( wstrSequenceName );
	if ( pTemplateSequence == NULL )
		return false;

	switch( pTemplateSequence->GetParticleType() )
	{
	case CKTDGParticleSystem::PT_MESH:
		g_pKTDXApp->GetDeviceManager()->AppendXMesh( listInOut_, pTemplateSequence->GetMeshName(), ePriority );
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, pTemplateSequence->GetChangeTexXETName(), ePriority );
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, pTemplateSequence->GetMultiTexXETName(), ePriority );
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, pTemplateSequence->GetAniXETName(), ePriority );
		break;

	case CKTDGParticleSystem::PT_SKINMESH:
		g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplateSequence->GetMeshName(), ePriority );
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, pTemplateSequence->GetChangeTexXETName(), ePriority );
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, pTemplateSequence->GetMultiTexXETName(), ePriority );
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, pTemplateSequence->GetAniXETName(), ePriority );
		break;

	default:

		BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticleEvent*, pEvent, pTemplateSequence->m_EventList )
		{
			switch( pEvent->GetEventType() )
			{
			case CKTDGParticleSystem::ET_TEXTURE:
				CKTDGParticleSystem::CParticleEvent_Texture* pEventTexture = (CKTDGParticleSystem::CParticleEvent_Texture*)pEvent;
				if ( pEventTexture->GetTextureName() != NULL )
					g_pKTDXApp->GetDeviceManager()->AppendTexture( listInOut_, pEventTexture->GetTextureName(), ePriority );
				break;
			}//switch
		}//BOOST_FOREACH)

	}//switch

	return true;
}
//#endif	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD

#ifdef EFFECT_TOOL
bool CKTDGParticleSystem::EraseParticleSequnce( UINT uiParticleIndex_, const WCHAR* wcFileName_ )
{
	if ( true == m_mapTempletSequences.empty() )
		return false;

	if ( L"" == wcFileName_ )
		return false;

	vector<CParticleEventSequence*>::iterator it = m_mapTempletSequences[wcFileName_].begin() + uiParticleIndex_ ;
	m_TempletSequences.erase( (*it)->GetName() );
	m_mapTempletSequences[wcFileName_].erase( it );

	return true;
}
bool CKTDGParticleSystem::GetVecTempletSequences(const WCHAR* strFileName_, OUT vector<CParticleEventSequence*>& vecParticleTemplet_ ) 
{ 
	if ( true == m_mapTempletSequences.empty() )
		return false;

	if( L"" == strFileName_ ) 
		return false;

	vecParticleTemplet_ = m_mapTempletSequences[strFileName_]; 
	return true;
}
CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::CreateEventSequence( const WCHAR* strParticleName_, const WCHAR* strParticleFileName_)
{
	std::map<wstring,CParticleEventSequence*>::iterator it = m_TempletSequences.find( strParticleName_ );
	if( it != m_TempletSequences.end() )
	{
		MessageBox( NULL, strParticleName_ , L"이름 중복_CreateEventSequence" , NULL );
		return NULL;
	}

	CParticleEventSequence* newSeq = CParticleEventSequence::CreateParticleEventSequence( this, NULL );
	newSeq->SetName( strParticleName_ );
	m_mapTempletSequences[strParticleFileName_].push_back(newSeq);
	m_TempletSequences[strParticleName_] = newSeq;
	return newSeq;
}
void CKTDGParticleSystem::MoveParticleTempletFile( const WCHAR* pSrcFileName_, const WCHAR* pDestFileName_ )
{
	vector<CParticleEventSequence*> vecTemp; // 원본 파일의 벡터
	vecTemp.swap( m_mapTempletSequences[pSrcFileName_] );

	BOOST_FOREACH( CParticleEventSequence* pEventSequence, vecTemp ) 
	{
		m_mapTempletSequences[pDestFileName_].push_back( pEventSequence );
	}
}
#endif //EFFECT_TOOL

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
void CKTDGParticleSystem::CParticleEventSequence::_RenderBoundingSphere( const D3DXVECTOR3& vCenter, float fRadius )
{
	if ( m_pkBoundingSphere != NULL )
	{
		DWORD dwOldFillMode = CKTDGStateManager::GetRenderState( D3DRS_FILLMODE );
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		CKTDGStateManager::SetTexture( 0, NULL );
		CKTDGStateManager::SetTexture( 1, NULL );
	
		CKTDGXRenderer::RenderParam renderParam;
		renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;		
		renderParam.color = D3DXCOLOR(0,0,1,1);
		CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	
		matTemp.Scale( fRadius, fRadius, fRadius );
		matTemp.Move( vCenter );	
		D3DXMATRIX worldMatrix = matTemp.GetMatrix();
	
		g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pkBoundingSphere );
	
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, dwOldFillMode );
	}//if
}
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST

//#ifdef X2OPTIMIZE_CULLING_PARTICLE
//void CKTDGParticleSystem::CParticleEventSequence::_DecideWorldMatrix_Normal( CKTDGCamera& kCamera, D3DXVECTOR3& vCenter, float fRadius, bool& bIdentityWorldMatrix )
//{
//	if( fRadius <= 0.0f )
//		return;
//
//	//frustum의 far에 걸치면 앞쪽으로 당기는 트릭을 사용한다.
//	D3DXVECTOR3 dirVec = kCamera.GetLookAt() - kCamera.GetEye();
//	D3DXVec3Normalize( &dirVec, &dirVec );
//
//	D3DXVECTOR3 vEyeToCenter = vCenter - kCamera.GetEye();
//	float fCenterProjLength = D3DXVec3Dot( &vEyeToCenter, &dirVec );
//
//	if( fCenterProjLength > 0.0f )
//	{
//		float fFarLength = g_pKTDXApp->GetDGManager()->GetFar();
//
//		if( fCenterProjLength - fRadius < fFarLength && fCenterProjLength + fRadius > fFarLength )
//		{
//			D3DXMATRIX kWorldMatrix;
//			D3DXMatrixIdentity( &kWorldMatrix );
//
//			D3DXVec3Normalize( &vEyeToCenter, &vEyeToCenter );
//
//			D3DXMATRIX TranslationMatrix1;
//			D3DXMATRIX TranslationMatrix2;
//			D3DXMATRIX TranslationMatrix3;
//			D3DXMATRIX ScaleMatrix;
//
//			//scale을 위해 world space의 원점으로 이동
//			D3DXMatrixTranslation(&TranslationMatrix1, -vCenter.x, -vCenter.y, -vCenter.z );
//
//			float fScale = ( fFarLength - fRadius * 2 ) / fFarLength;//가까워진 만큼 작아진다.
//			D3DXMatrixScaling(&ScaleMatrix, fScale, fScale, fScale );
//
//			//원위치로...
//			D3DXMatrixTranslation(&TranslationMatrix2, vCenter.x, vCenter.y, vCenter.z );
//
//			D3DXVECTOR3 vMove = -vEyeToCenter * ( fRadius * 2 );//지름만큼 강제로 이동
//			D3DXMatrixTranslation(&TranslationMatrix3, vMove.x, vMove.y, vMove.z );
//
//			kWorldMatrix = kWorldMatrix * TranslationMatrix1 * ScaleMatrix * TranslationMatrix2 * TranslationMatrix3;
//
//			g_pKTDXApp->SetWorldTransform( &kWorldMatrix );
//            bIdentityWorldMatrix = false;
//            return;
//        }
//    }
//    if ( bIdentityWorldMatrix == false )
//    {
//		D3DXMATRIX kWorldMatrix;
//		D3DXMatrixIdentity( &kWorldMatrix );
//		g_pKTDXApp->SetWorldTransform( &kWorldMatrix );
//        bIdentityWorldMatrix = true;
//	}
//}
//
//void CKTDGParticleSystem::CParticleEventSequence::_DecideWorldMatrix_Mesh( D3DXMATRIX& kWorldMatrix, CKTDGCamera& kCamera, D3DXVECTOR3& vCenter, float fRadius )
//{
//	if( fRadius <= 0.0f )
//		return;
//
//	//frustum의 far에 걸치면 앞쪽으로 당기는 트릭을 사용한다.
//	D3DXVECTOR3 dirVec = kCamera.GetLookAt() - kCamera.GetEye();
//	D3DXVec3Normalize( &dirVec, &dirVec );
//
//	D3DXVECTOR3 vEyeToCenter = vCenter - kCamera.GetEye();
//	float fCenterProjLength = D3DXVec3Dot( &vEyeToCenter, &dirVec );
//
//	if( fCenterProjLength > 0.0f )
//	{
//		float fFarLength = g_pKTDXApp->GetDGManager()->GetFar();
//
//		if( fCenterProjLength - fRadius < fFarLength && fCenterProjLength + fRadius > fFarLength )
//		{
//			D3DXVec3Normalize( &vEyeToCenter, &vEyeToCenter );
//
//			D3DXMATRIX TranslationMatrix1;
//			D3DXMATRIX TranslationMatrix2;
//			D3DXMATRIX TranslationMatrix3;
//			D3DXMATRIX ScaleMatrix;
//
//			//scale을 위해 world space의 원점으로 이동
//			D3DXMatrixTranslation(&TranslationMatrix1, -vCenter.x, -vCenter.y, -vCenter.z );
//
//			float fScale = ( fFarLength - fRadius * 2 ) / fFarLength;//지름만큼 가까워졌음으로 작아지도록 한다.
//			D3DXMatrixScaling(&ScaleMatrix, fScale, fScale, fScale );
//
//			//원위치로...
//			D3DXMatrixTranslation(&TranslationMatrix2, vCenter.x, vCenter.y, vCenter.z );
//
//			D3DXVECTOR3 vMove = -vEyeToCenter * ( fRadius * 2 );//지름만큼 강제로 이동
//			D3DXMatrixTranslation(&TranslationMatrix3, vMove.x, vMove.y, vMove.z );
//
//			kWorldMatrix = kWorldMatrix * TranslationMatrix1 * ScaleMatrix * TranslationMatrix2 * TranslationMatrix3;
//		}
//	}
//}
//#endif//X2OPTIMIZE_CULLING_PARTICLE


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
UINT    CKTDGParticleSystem::EstimateParticleSequenceNum()
{
    if ( m_coParticleSequence.empty( SEQUENCELIST_LIVE ) == true )
        return 0;
    UINT    uCount = 0;
    KSequenceHandleList::iterator iterEnd = m_coParticleSequence.end(SEQUENCELIST_LIVE);
    for( KSequenceHandleList::iterator iter = m_coParticleSequence.begin( SEQUENCELIST_LIVE );
        iter != iterEnd;
        ++iter )
    {
        uCount++;
    }
    return  uCount;
}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE



