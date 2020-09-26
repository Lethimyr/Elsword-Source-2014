#pragma once

//#include "Performer.h"
//#include <thread/thread.h>
//#include <atltime.h> // CTime
//#include <dbg/dbg.hpp>

class KThreadManager : public KPerformer
{
    DeclToStringW;
    NiDeclareRTTI;

protected:
    KThreadManager(void);
public:
    virtual ~KThreadManager(void);

public:
    virtual void Init( int nThreadNum );
    virtual void BeginThread();
    virtual void EndThread( DWORD dwTimeOut = 10000 );   // derive to Child...

    size_t GetThreadNum() const { KLocker lock( m_csVecThread ); return m_vecThread.size(); }
    void SetThreadNum( size_t nThreadNum ); // 실시간에 스레드 갯수를 조절하려고 할 때 호출.

    // 큐잉된 이벤트를 얻어옴. 큐가 비어있으면 이벤트 없이 즉시 리턴.
    bool GetKEvent( KEventPtr& spEvent );

    int GetQueueSize()
    {
        KLocker lock( m_csEventQueue );
        return m_queEvent.size();
    }

    void DumpMaxProcessInfo( std::wostream& stm ) const;
    void ResetMaxProcessingInfo();

    void CheckMaxProcessingInfo( DWORD dwElapTime, const KEventPtr spEventPtr )
    {
        KLocker lock( m_kMaxProcessingInfo.m_cs );

        if( dwElapTime > m_kMaxProcessingInfo.m_dwTerm )
        {
            //m_kMaxProcessingInfo.m_nSenderUID  = kEvent.m_nSenderUID;
            m_kMaxProcessingInfo.m_strEventID  = spEventPtr->GetIDStr();
            m_kMaxProcessingInfo.m_cTime       = CTime::GetCurrentTime();
            m_kMaxProcessingInfo.m_dwTerm      = dwElapTime;

            START_LOG( clog2, L"이벤트 처리 최대 소요시간 갱신. Sender : "/* << spEventPtr->m_nSenderUID*/ )
                << BUILD_LOG( m_kMaxProcessingInfo.m_strEventID )
                << BUILD_LOG( m_kMaxProcessingInfo.m_dwTerm );
        }
    }

    // derived from KPerformer
    virtual void ProcessEvent( IN KEvent* ) {}

protected:      
    // 060113. florist. Factory Method 패턴을 활용한 객체 생성. 이를 위해 생성할 객체마다 
    // KThreadManager를 상속받는 새 클래스를 정의해야 하지만, 이것으로 인해 독립성이 강해져
    // singleton으로 매니저 객체를 사용한다거나 하는 등에 유용하다. template을 활용해 상속없이
    // thread 객체를 생성하게 했었으나, 다시 상속을 활용한 예전방식으로 회귀하였다. 매번 상속하는게 낫다.
    virtual KThread* CreateThread() = 0;

    bool                                m_bTerminateReserved;
    size_t                              m_nTerminateCount;  // 종료된 스레드 갯수를 센다.
    std::vector< boost::shared_ptr<KThread> >   m_vecThread;
    mutable KncCriticalSection                  m_csVecThread;

    struct KMaxProcessingInfo {  // 처리시간이 가장 오래 걸린 이벤트에 대한 정보를 기억.
        mutable KncCriticalSection  m_cs;
        //UidType                     m_nSenderUID;
        std::wstring                m_strEventID;
        DWORD                       m_dwTerm;       // 이벤트 하나를 처리완료하기까지 걸린 시간.
        CTime                       m_cTime;
    } m_kMaxProcessingInfo;
};

template< typename T >
class KTThreadManager : public KThreadManager
{
    NiDeclareRTTI;
    DeclToStringW;

protected:
    KTThreadManager(void) {}
public:
    virtual ~KTThreadManager(void) {}

    static KTThreadManager<T>* GetInstance()
    {
        if( ms_selfInstance == NULL )
            ms_selfInstance = new KTThreadManager<T>;
        return ms_selfInstance;
    }
    static void ReleaseInstance()
    {
        if( ms_selfInstance != NULL )
        {
            delete ms_selfInstance;
            ms_selfInstance = NULL;
        }
    }

protected:
    // util function
    virtual KThread* CreateThread()
    {
        T* pkThread( new T );
        pkThread->SetThreadManager( this );
        return pkThread;
    }

    static KTThreadManager<T>*   ms_selfInstance;
};

template < typename T > KTThreadManager<T>* KTThreadManager<T>::ms_selfInstance = NULL;
template < typename T > NiImplementRTTI( KTThreadManager<T>, KThreadManager );
template < typename T > ImplOstmOperatorW2A( KTThreadManager<T> );
template < typename T > ImplToStringW( KTThreadManager<T> )
{
    return START_TOSTRING_PARENTW( KThreadManager );
}
