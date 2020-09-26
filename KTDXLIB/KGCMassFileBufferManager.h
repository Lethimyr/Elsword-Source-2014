/** @file   KGCMassFileBufferManager.h
    @desc   interface of KGCMassFileBufferManager
    @author seojt@kogstudios.com
    @since  2008-10-16, 16:56
            - 엘소드 리소스 백그라운드 로딩의 작업의 일부로 KGCMassFileManager가 사용하는
                메모리 관리자를 thread sensitive하게 얻기 위해 처음으로 작성을 시작하다.
*/

#pragma once

#include "CSLock.h"
#include <vector>

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER


#include <boost/intrusive_ptr.hpp>
#include <boost/intrusive/list.hpp>


/** @class  KGCMassFileBuffer  
    @brief  KGCMassFileManager가 내부적으로 사용하는 버퍼 매니저를 외부로 가져왔다.
            이름 충돌을 피하기 위해 원래 이름 KGCBufferManager을 KGCMassFileBuffer로 바꾸었다.

    @desc   KGCMassFileBuffer는 원하는 크기의 heap 메모리를 얻기 위해 사용한다.

            KGCMassFileBuffer는 내부적으로 critical section을 사용하지 않는다. 이것은
            KGCMassFileBufferManager가 사용할 버퍼를 결정할 때, critical section을 사용하며,
            이렇게 결정된 버퍼는 하나의 thread에서 사용된다는 보장이 되기 때문이다.

    @date   jintaeks on 2008-10-16, 16:59 */


class KGCMassFileBuffer;
namespace boost
{
    void intrusive_ptr_add_ref(KGCMassFileBuffer * p);
    void intrusive_ptr_release(KGCMassFileBuffer * p);
}


class KGCMassFileBufferManager;

class KGCMassFileBuffer : public boost::intrusive::list_base_hook<>
{
public:
    /// constructor.



    /// 메모리 바이트 크기가 iSize인 메모리 블록에 대한 시작 주소를 얻는다.
    /// 현재 버퍼의 크기가 iSize보다 작다면, 함수 내부에서 버퍼의 크기는 자동으로 커진다.
    /// @return int: buffer size in bytes

    char*   GetBuffer()     { return m_pData + m_iOffset; }


	int		GetMemorySize() { return m_iMemorySize; }
	int		GetOffset()		{ return m_iOffset; }
	char*	GetMemoryBuffer() { return m_pData; }
	char*	DetachMemoryBuffer();


private:

    friend class    KGCMassFileBufferManager;
	friend void ::boost::intrusive_ptr_add_ref(KGCMassFileBuffer * p);
	friend void ::boost::intrusive_ptr_release(KGCMassFileBuffer * p);

    KGCMassFileBuffer( int iSize, KGCMassFileBufferManager* pManager = NULL, int iBucketIndex = -1 );

    /// destructor.
    ~KGCMassFileBuffer();

    void    AddRef();
    void    Release();
    LONG    GetRefCount() const { return m_nRefCount; }

    KGCMassFileBufferManager*   GetManager() const { return m_pManager; }
    int     GetBucketIndex() const          { return m_iBucketIndex; }
    int     GetOffset() const               { return m_iOffset; }
    void    SetOffset( int iOffset )        { if ( iOffset >= 0 && iOffset < m_iMemorySize ) m_iOffset = iOffset; }

private:

    volatile LONG               m_nRefCount;
    char*						m_pData;
    int                         m_iMemorySize;  ///< buffer size in bytes

    KGCMassFileBufferManager*   m_pManager;
    int                         m_iBucketIndex;
    int                         m_iOffset;


};//class KGCMassFileBuffer

inline void ::boost::intrusive_ptr_add_ref(KGCMassFileBuffer * p)
{
    if ( p != NULL )
        p->AddRef();
}//
inline void ::boost::intrusive_ptr_release(KGCMassFileBuffer * p)
{
    if ( p != NULL )
        p->Release();
}//


/** boost::shared_ptr<>로 관리되는 포인터는 reference counter를 사용하여
    smart pointer이다. KGCMassFileBufferManager가 KGCMassFileBufferPtr를 리턴하면
    KGCMassFileBufferPtr를 리턴받은 곳에서 reference counter가 2가 되며(KGCMassFileBufferManager가
    관리하므로 +1, 리턴받는 곳에 복사되었으므로 +1), 이렇게 reference counter가 >= 2인
    버퍼는 이미 사용하고 있다고 판단한다. KGCMassFileBufferPtr가 block scope를 벋어 날 때
    reference counter는 자동으로 감소하며, reference counter가 1인 버퍼는 다시 사용대상이
    된다. - jintaeks on 2008-10-18, 11:49 */
typedef boost::intrusive_ptr<KGCMassFileBuffer> KGCMassFileBufferPtr;


/** @class  KGCMassFileBufferManager
    @brief  KGCMassFileManager가 사용하는 버퍼를 관리한다.
            현재 사용 가능한 KGCMassFileBuffer를 찾아서 리턴한다.

    @desc   현재 할당된 모든 버퍼가 사용되고 있으면 새로운 KGCMassFileBuffer를 리턴한다.
            현재 - jintaeks on 2008-10-18, 11:50 - 엘소드의 single thread에서 사용하고
            있는 버퍼의 수는 모두 5개 였다.

    @date   jintaeks on 2008-10-16, 17:10 */
class KGCMassFileBufferManager
{
public:
    /// constructor.
    KGCMassFileBufferManager();
    /// destructor.
    ~KGCMassFileBufferManager();

    /// 사용 가능한 새로운 하나의 KGCMassFileBuffer객체를 얻는다.
    /// @note   KGCMassFileBufferPtr를 리턴 받은 곳에서 이 값을 복사해서 사용해야 한다.
    ///         그러면 GetBuffer()를 호출한 쪽에서 reference counter 값은 2가 된다.
    KGCMassFileBufferPtr    GetBuffer( int iSize );

    //void                    OnFrameMove( float fElapsedTime );
    void                    ReturnBuffer( KGCMassFileBuffer* pBuffer );
	void					ReleaseAllBuffers();

private:

    typedef boost::intrusive::list<KGCMassFileBuffer>   BUFFER_LIST;
	static const unsigned int BUCKET_NUMS = 23;

    struct      BufferBucket
    {
        MemberCriticalSection   m_cs;
        BUFFER_LIST             m_listBuffer;
    };//struct      BufferBucket

    boost::array< BufferBucket, BUCKET_NUMS >   
                                m_aBuckets;

    //float                       m_fTimer;
    //int                         m_iFlushBucket;


};//class KGCMassFileBufferManager






//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//
//#include <boost/shared_ptr.hpp>
//
//
//
//
///** @class  KGCMassFileBuffer  
//    @brief  KGCMassFileManager가 내부적으로 사용하는 버퍼 매니저를 외부로 가져왔다.
//            이름 충돌을 피하기 위해 원래 이름 KGCBufferManager을 KGCMassFileBuffer로 바꾸었다.
//
//    @desc   KGCMassFileBuffer는 원하는 크기의 heap 메모리를 얻기 위해 사용한다.
//
//            KGCMassFileBuffer는 내부적으로 critical section을 사용하지 않는다. 이것은
//            KGCMassFileBufferManager가 사용할 버퍼를 결정할 때, critical section을 사용하며,
//            이렇게 결정된 버퍼는 하나의 thread에서 사용된다는 보장이 되기 때문이다.
//
//    @date   jintaeks on 2008-10-16, 16:59 */
//
//
//
//class KGCMassFileBuffer
//{
//public:
//    /// constructor.
//    KGCMassFileBuffer();
//    /// destructor.
//    ~KGCMassFileBuffer();
//    /// 메모리 바이트 크기가 iSize인 메모리 블록에 대한 시작 주소를 얻는다.
//    /// 현재 버퍼의 크기가 iSize보다 작다면, 함수 내부에서 버퍼의 크기는 자동으로 커진다.
//    char* GetBuffer(int iSize);
//    /// @return int: buffer size in bytes
//    int   GetBufferSize() { return m_iMemorySize; }
//    /// 버퍼를 모두 지운다.
//    void  Flush();
//
//private:
//    int     m_iMemorySize;  ///< buffer size in bytes
//    char*   m_pData;        ///< start address of buffer
//    // KGCMassFileBuffer의 하나의 instance에 대한 사용은 하나의 thread로 한정해서 사용한다고
//    // 가정하므로, 별도의 critical section은 필요없다.
//    //MemberCriticalSection   m_cs;
//};//class KGCMassFileBuffer
//
//
///** boost::shared_ptr<>로 관리되는 포인터는 reference counter를 사용하여
//    smart pointer이다. KGCMassFileBufferManager가 KGCMassFileBufferPtr를 리턴하면
//    KGCMassFileBufferPtr를 리턴받은 곳에서 reference counter가 2가 되며(KGCMassFileBufferManager가
//    관리하므로 +1, 리턴받는 곳에 복사되었으므로 +1), 이렇게 reference counter가 >= 2인
//    버퍼는 이미 사용하고 있다고 판단한다. KGCMassFileBufferPtr가 block scope를 벋어 날 때
//    reference counter는 자동으로 감소하며, reference counter가 1인 버퍼는 다시 사용대상이
//    된다. - jintaeks on 2008-10-18, 11:49 */
//typedef boost::shared_ptr<KGCMassFileBuffer>    KGCMassFileBufferPtr;
//
//
///** @class  KGCMassFileBufferManager
//    @brief  KGCMassFileManager가 사용하는 버퍼를 관리한다.
//            현재 사용 가능한 KGCMassFileBuffer를 찾아서 리턴한다.
//
//    @desc   현재 할당된 모든 버퍼가 사용되고 있으면 새로운 KGCMassFileBuffer를 리턴한다.
//            현재 - jintaeks on 2008-10-18, 11:50 - 엘소드의 single thread에서 사용하고
//            있는 버퍼의 수는 모두 5개 였다.
//
//    @date   jintaeks on 2008-10-16, 17:10 */
//class KGCMassFileBufferManager
//{
//    typedef std::vector<KGCMassFileBufferPtr>   BUFFER_VECTOR;
//
//private:
//    BUFFER_VECTOR               m_vecBuffers;
//    /// 가장 최근에 사용한 버퍼의 index이다. 다음 사용 가능한 버퍼를 빠르게 찾기 위해 사용한다.
//    size_t                      m_uiCursor;
//    // KGCMassFileBufferManager 객체는 여러 thread가 접근하지 않으므로, critical section이
//    // 필요없다. - jintaeks on 2008-10-16, 19:46
//    MemberCriticalSection       m_csVecBuffers; ///< m_mapBuffers를 thread safe하게 접근하기 위해 사용한다.
//
//public:
//    /// constructor.
//    KGCMassFileBufferManager();
//    /// destructor.
//    ~KGCMassFileBufferManager();
//
//    /// 사용 가능한 새로운 하나의 KGCMassFileBuffer객체를 얻는다.
//    /// @note   KGCMassFileBufferPtr를 리턴 받은 곳에서 이 값을 복사해서 사용해야 한다.
//    ///         그러면 GetBuffer()를 호출한 쪽에서 reference counter 값은 2가 된다.
//    KGCMassFileBufferPtr    GetBuffer();
//    /// 현재 할당된 버퍼의 개수를 리턴한다.
//    size_t                  GetNumBuffers() const { return m_vecBuffers.size(); }
//};//class KGCMassFileBufferManager
//
//
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER


/** @example    KGCMassFileBufferManager

    #include "AssertTrace.h"
    #include <string>
    #include <boost/shared_ptr.hpp>
    #include "KGCMassFileBufferManager.h"


    KGCMassFileBufferManager    g_bufMan;


    void TestInner( KGCMassFileBufferPtr p )
    {
        printf( "use_count = %i\r\n", p.use_count() );
    }//TestInner()

    void Test()
    {
        KGCMassFileBufferPtr p = g_bufMan.GetBuffer();
        printf( "use_count = %i\r\n", p.use_count() );
        KGCMassFileBufferPtr p2 = g_bufMan.GetBuffer();
        TestInner( p2 );
        printf( "use_count = %i\r\n", p.use_count() );

        printf( "NumBuffers = %i\r\n", g_bufMan.GetNumBuffers() );
    }//Test()


    void main()
    {
        Test();
        Test();

        // output:
        //use_count = 2
        //use_count = 3
        //use_count = 2
        //NumBuffers = 2
        //use_count = 2
        //use_count = 3
        //use_count = 2
        //NumBuffers = 2
        //Press any key to continue
    }//main()
*/
