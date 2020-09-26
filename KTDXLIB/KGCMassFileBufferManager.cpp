/** @file   KGCMassFileBufferManager.cpp
    @desc   implementation of KGCMassFileBufferManager
*/

#include "StdAfx.h" // 엘소드는 PCH(precompiled header)로 고정된 이름 stdafx.h를 사용한다.
#include "KGCMassFileBufferManager.h"
#include "AssertTrace.h"


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif    

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

#pragma	intrinsic(_BitScanReverse)



///-----------------------------------------------------------------------------
// KGCMassFileBuffer관련 멤버들
//
// constructor.

KGCMassFileBuffer::KGCMassFileBuffer( int iSize, KGCMassFileBufferManager* pManager, int iBucketIndex )
: m_nRefCount( 0 )
, m_pData( NULL )
, m_iMemorySize( iSize )
, m_pManager( pManager )
, m_iBucketIndex( iBucketIndex )
, m_iOffset( 0 )
{
    if ( m_iMemorySize < 0 )
        m_iMemorySize = 0;

    if ( m_iMemorySize > 0 )
    {
        m_pData = new char[ m_iMemorySize ];
        ASSERT( m_pData );
    }//if

}//KGCMassFileBuffer::KGCMassFileBuffer()


// destructor.
KGCMassFileBuffer::~KGCMassFileBuffer()
{
	SAFE_DELETE_ARRAY( m_pData );
}//KGCMassFileBuffer::~KGCMassFileBuffer()


void    KGCMassFileBuffer::AddRef()
{
    ::InterlockedIncrement( &m_nRefCount );
}//KGCMassFileBuffer::AddRef()


void    KGCMassFileBuffer::Release()
{
    LONG refCount = ::InterlockedDecrement( &m_nRefCount );
	if( refCount == 0 )
	{
        KGCMassFileBuffer* pThis = this;


        if ( m_pManager == NULL )
        {
            SAFE_DELETE( pThis );
        }
        else
        {
            m_pManager->ReturnBuffer( pThis );
        }//if.. else..


	}//if
}//CKTDXAnimationSetCacher::AddRef()


char*	KGCMassFileBuffer::DetachMemoryBuffer()
{
	char* pReturn = m_pData;
	m_pData = NULL;
	m_iMemorySize = 0;
	m_iBucketIndex = -1;
	m_iOffset = 0;
	return	pReturn;
}


///-----------------------------------------------------------------------------
// KGCMassFileBufferManager 관련 멤버들
//
KGCMassFileBufferManager::KGCMassFileBufferManager()

: m_aBuckets()
//, m_fTimer( 0.f )
//, m_iFlushBucket( BUCKET_64K )

{
}//KGCMassFileBufferManager::KGCMassFileBufferManager()


KGCMassFileBufferManager::~KGCMassFileBufferManager()
{
	ReleaseAllBuffers();
}//KGCMassFileBufferManager::~KGCMassFileBufferManager()


KGCMassFileBufferPtr KGCMassFileBufferManager::GetBuffer( int iSize )
{

    if ( iSize <= 0 )
    {
        KGCMassFileBufferPtr spBuffer( new KGCMassFileBuffer( 0 ) );
        ASSERT( spBuffer );
        return  spBuffer;
    }

	unsigned long	ulIndex = 0;
	_BitScanReverse( &ulIndex, (unsigned long) iSize );
	int	iBufferSize = 1<<ulIndex;
	if ( iSize > iBufferSize )
	{
		ulIndex++;
		iBufferSize = 1<<ulIndex;
	}//if

	if ( ulIndex >= BUCKET_NUMS )
	{
        KGCMassFileBufferPtr spBuffer( new KGCMassFileBuffer( iSize ) );
        ASSERT( spBuffer );
        return  spBuffer;
	}

    {
        BufferBucket&   bucket = m_aBuckets[ ulIndex ];


        KGCMassFileBuffer* pBuffer = NULL;
        {
            CSLock lock( bucket.m_cs );

            if ( bucket.m_listBuffer.empty() == false )
            {
                pBuffer = &bucket.m_listBuffer.front();
                bucket.m_listBuffer.pop_front();
            }
        }
        if ( pBuffer != NULL )
        {
            //ASSERT( pBuffer->GetManager() == this && pBuffer->GetMemorySize() == iBufferSize
            //    && pBuffer->GetBucketIndex() == eBucket );

            // 버퍼의 앞뒤를 번갈아가면서 모두 사용하게 하여 물리 메모리 상주 가능성을 높이는 트릭
            if ( pBuffer->GetOffset() != 0 )
                pBuffer->SetOffset( 0 );
            else
			{
				int	iOffset = ( iBufferSize - iSize ) & ( ~0x3 );
                pBuffer->SetOffset( iOffset );  
			}
            return KGCMassFileBufferPtr( pBuffer );
        }//if

        KGCMassFileBufferPtr spBuffer( new KGCMassFileBuffer( iBufferSize, this, ulIndex ) );
        ASSERT( spBuffer );
        return  spBuffer;
    }



}//KGCMassFileBufferManager::GetBuffer()


//void    KGCMassFileBufferManager::OnFrameMove( float fElapsedTime )
//{
//    m_fTimer += fElapsedTime;
//    if ( m_fTimer >= KGCMASSFILEBUFFERMANAGER_CHECK_INTERVAL_IN_SEC )
//    {
//        m_fTimer = 0.f;
//        if ( false == ( m_iFlushBucket >= BUCKET_64K && m_iFlushBucket < BUCKET_NUMS ) )
//            m_iFlushBucket = BUCKET_64K;
//
//        DWORD   dwTimestamp = KSystemTimer::GetApproxSystemTimeInMilisecond();
//
//        BufferBucket& bucket = m_aBuckets[ m_iFlushBucket ];
//        {
//            KGCMassFileBuffer* pBufferToFree = NULL;
//            {
//                CSLock lock( bucket.m_cs );
//                for( BUFFER_LIST::reverse_iterator riter = bucket.m_listBuffer.rbegin(); riter != bucket.m_listBuffer.rend();  )
//                {
//                    KGCMassFileBuffer* pBuffer = &(*riter);
//                    ASSERT( pBuffer != NULL );
//                    // 일정 시간이 지난 버퍼를 삭제한다.
//                    // 오래된 버퍼는 하드디스크 스왑스페이스로 옮겨지게 되어
//                    // 나중에 재사용될 때 새로 할당받는 것보다 성능을 떨어뜨릴 가능성이 크다.
//                    if ( (LONG) ( dwTimestamp - pBuffer->GetUnusedTimestamp() )
//                            >= KGCMASSFILEBUFFERMANAGER_DELETE_INTERVAL_IN_MILISEC )
//                    {
//                        // reverse_iterator -> iterator 변환은 Effective STL 항목 26 참조
//                        bucket.m_listBuffer.erase( (++riter).base() );
//                        pBufferToFree = pBuffer;
//                        break;
//                    }
//                    else
//                    {
//                        riter++;
//                    }//if.. else..
//                }//for
//            }
//            if ( pBufferToFree != NULL )
//            {
//                SAFE_DELETE( pBufferToFree );
//            }//if
//        }
//        m_iFlushBucket = m_iFlushBucket + 1;
//        if ( m_iFlushBucket >= BUCKET_NUMS )
//            m_iFlushBucket = BUCKET_64K;
//    }//if
//
//}//KGCMassFileBufferManager::OnFrameMove()


void    KGCMassFileBufferManager::ReturnBuffer( KGCMassFileBuffer* pBuffer )
{
    if ( pBuffer == NULL || pBuffer->GetManager() != this
        || pBuffer->GetRefCount() != 0 )
        return;

    int iBucketIndex = pBuffer->GetBucketIndex();

    if ( false == ( iBucketIndex >= 0 && iBucketIndex < BUCKET_NUMS )
        || pBuffer->GetMemorySize() != ( 1 << iBucketIndex ) )
    {
        SAFE_DELETE( pBuffer );
        return;
    }//if

    BufferBucket& bucket = m_aBuckets[ iBucketIndex ];
	{
	    CSLock lock( bucket.m_cs );
		bucket.m_listBuffer.push_front( *pBuffer );
	}
}//KGCMassFileBufferManager::ReturnBuffer()


void	KGCMassFileBufferManager::ReleaseAllBuffers()
{
	for( int i = 0; i < BUCKET_NUMS; i++ )
	{
		BUFFER_LIST	listBuffer;
		BufferBucket& bucket = m_aBuckets[ i ];
		{
			CSLock lock( bucket.m_cs );
			listBuffer.swap( bucket.m_listBuffer );
		}
		while( listBuffer.empty() == false )
		{
			KGCMassFileBuffer* pBuffer = &listBuffer.front();
			listBuffer.pop_front();
			SAFE_DELETE( pBuffer );
		}
	}
}


//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//
//
//
/////-----------------------------------------------------------------------------
//// KGCMassFileBuffer관련 멤버들
////
//// constructor.
//KGCMassFileBuffer::KGCMassFileBuffer()
//{
//    m_pData         = NULL;
//    m_iMemorySize   = 0;//1024 * 1024;        //1MB
//    m_pData         = NULL;
//    //m_pData         = new char[m_iMemorySize];
//    //ASSERT( m_pData );
//}//KGCMassFileBuffer::KGCMassFileBuffer()
//
//
//// destructor.
//KGCMassFileBuffer::~KGCMassFileBuffer()
//{
//    Flush();
//}//KGCMassFileBuffer::~KGCMassFileBuffer()
//
//
//void KGCMassFileBuffer::Flush()
//{
//    SAFE_DELETE_ARRAY( m_pData );
//    m_iMemorySize = 0;
//}//KGCMassFileBuffer::Flush()
//
//
//char* KGCMassFileBuffer::GetBuffer(int iSize)
//{
//    if ( m_iMemorySize < iSize )
//    {
//        SAFE_DELETE_ARRAY( m_pData );
//
//        // 처음에 0으로 초기화되어 있을 경우, 무한 루프를 방지하기 위해
//        // 1024로 초기화한다.
//        if ( m_iMemorySize < 1024 )
//            m_iMemorySize = 1024;
//
//        while ( (m_iMemorySize *= 2) < iSize )
//        {
//        }//while
//
//        m_pData = new char[m_iMemorySize];
//        ASSERT( m_pData );
//    }//if
//
//    //ZeroMemory( m_pData, sizeof(char) * m_iMemorySize );
//
//    return m_pData;
//}//KGCMassFileBuffer::GetBuffer()
//
//
/////-----------------------------------------------------------------------------
//// KGCMassFileBufferManager 관련 멤버들
////
//KGCMassFileBufferManager::KGCMassFileBufferManager()
//{
//    m_uiCursor = 0;
//}//KGCMassFileBufferManager::KGCMassFileBufferManager()
//
//
//KGCMassFileBufferManager::~KGCMassFileBufferManager()
//{
//}//KGCMassFileBufferManager::~KGCMassFileBufferManager()
//
//
//KGCMassFileBufferPtr KGCMassFileBufferManager::GetBuffer()
//{
//    CSLock lock( m_csVecBuffers );
//
//    size_t  uiSize = m_vecBuffers.size();
//    size_t  uiCounter = 0;
//    while ( uiCounter < uiSize )
//    {
//        // 아직 아무곳에도 사용하고 있지 않은 버퍼를 찾는다.
//        // 아무곳에도 사용하고 있지 않으면, KGCMassFileBufferManager가 관리하는
//        // 곳에서만 reference하고 있기 때문에 reference counter값이 1이다.
//        // unique()는 ref counter가 1이면 true를 리턴한다.
//        // - jintaeks on 2008-10-16, 19:47
//        if ( m_vecBuffers[m_uiCursor].unique() )
//		{
//            return m_vecBuffers[m_uiCursor];
//		}
//
//        m_uiCursor = (m_uiCursor + 1 ) % uiSize;
//        ++uiCounter;
//    }//while
//
//    // 사용 가능한 버퍼가 없으면 새로 만든다.
//    // 
//    m_vecBuffers.push_back( KGCMassFileBufferPtr(new KGCMassFileBuffer) );
//    ASSERT( m_vecBuffers.size() == uiSize + 1 );
//    TRACEA( "KGCMassFileBufferManager size = %i\r\n", m_vecBuffers.size() );
//	
//#ifdef HEAVY_LOG_TEST
//	// 현재 사용중인 버퍼수와 각 버퍼의 레퍼런스 카운트 로그를 출력한다.
//	WCHAR wszText[256] = L"";
//	StringCchPrintfW( wszText, ARRAY_SIZE( wszText ), L"KGCMassFileBufferManager size : %d.", (int) m_vecBuffers.size() );
//	StateLog( wszText );
//	for(UINT i=0; i<m_vecBuffers.size(); ++i)
//	{		
//		StringCchPrintfW( wszText, ARRAY_SIZE( wszText ), L"KGCMassFileBufferManager buffref : %d, %d (%d).", (int) i, m_vecBuffers[i].use_count(), m_vecBuffers[i]->GetBufferSize() );
//		StateLog( wszText );
//	}	
//#endif HEAVY_LOG_TEST
//
//
//
//    /** 현재 - jintaeks on 2008-10-17, 15:42 - single thread를 사용하는 엘소드 클라이언트가
//        최대로 사용하는 buffer의 개수는 5개 입니다.
//        "DLG_Map_Village_Ruben_Back.lua"를 로드하는 경우 처음 5개를 사용하는데,
//        1개의 compressed data를 위한 버퍼, 4개의 uncompressed data를 위한 버퍼를
//        사용하고 있습니다.
//
//        멀티 쓰레드로 되면, loading thread가 1set가 될 것이므로, buffer의 수는 10까지
//        늘어날 것으로 예상됩니다. - jintaeks on 2008-10-17, 15:43 */
//    //BREAK( m_vecBuffers.size() == 5 );
//    return m_vecBuffers[uiSize];
//}//KGCMassFileBufferManager::GetBuffer()
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER