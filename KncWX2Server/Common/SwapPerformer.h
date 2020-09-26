#pragma once
#include "Event.h"
#include "SimObject.h"
#include <queue>
#include <Serializer/SerBuffer.h>

#include <windows.h>    // CRITICAL_SECTION
#include "Thread/Locker.h"


//{{ 2012. 04. 17	최육사	스왑 퍼포머
//#ifdef SERV_SWAP_PERFORMER


enum ePerformerID;
class KSwapPerformer : public KSimObject
{
    NiDeclareRTTI;
    DeclToStringW;

public:
	struct KMaxQueueSize
	{
		KMaxQueueSize()
		{
			Clear();
		}

		void Clear()
		{
			m_QueueSize = 0;
		}

		std::wstring GetMaxQueueSizeRegDate() const
		{
			return ( std::wstring )( m_tRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );
		}

		size_t		m_QueueSize;
		CTime		m_tRegTime;
	};

    KSwapPerformer(void);
    virtual ~KSwapPerformer(void);

    // 이곳에 위치한 virtual은 모두 최초 정의. 상속 아님
    void    QueueingEvent( const KEventPtr& spEvent );
    size_t  GetTotalQueueSize() const
    {
        KLocker lockEn( m_csEnqueueEvent );
		KLocker lockDe( m_csDequeueEvent );
		
		_JIF( !m_pEnqueueEvent, return -1; );
		_JIF( !m_pDequeueEvent, return -1; );

        return ( m_pEnqueueEvent->size() + m_pDequeueEvent->size() );
    }
	size_t  GetEnqueueSize() const
	{
		KLocker lockEn( m_csEnqueueEvent );
		_JIF( !m_pEnqueueEvent, return -1; );
		return m_pEnqueueEvent->size();
	}
	size_t  GetDequeueSize() const
	{
		KLocker lockEn( m_csDequeueEvent );
		_JIF( !m_pDequeueEvent, return -1; );
		return m_pDequeueEvent->size();
	}
    virtual void            Tick();     // queue consumming

    virtual int GetPfID() = 0;
	
	KMaxQueueSize GetMaxQueueSize() const
	{
		KLocker lockEn( m_csEnqueueEvent );
		KLocker lockDe( m_csDequeueEvent );

		return m_kMaxQueueSize;
	}
	KMaxQueueSize GetMaxQueueSizeAndClear()
	{
		KLocker lockEn( m_csEnqueueEvent );
		KLocker lockDe( m_csDequeueEvent );

		KMaxQueueSize kTemp = m_kMaxQueueSize;
		m_kMaxQueueSize.Clear();
		return kTemp;
	}

protected:
    bool            GetKEvent( KEventPtr& spEvent );
	bool            CheckAndSwapQueue();
    virtual void    ProcessEvent( const KEventPtr& spEvent ) = 0;
    virtual bool    RoutePacket( const KEvent* pkEvent )   { return false; }
	friend void KncSend( DWORD dwPIFrom, UidType nFrom, KEventPtr spEvent );

protected:    
	typedef std::queue<KEventPtr>		KEventQueue;
	KEventQueue*								m_pEnqueueEvent;
	mutable CRITICAL_SECTION                    m_csEnqueueEvent;

	KEventQueue*								m_pDequeueEvent;
	mutable CRITICAL_SECTION                    m_csDequeueEvent;

	KMaxQueueSize								m_kMaxQueueSize;	// for dump
};

#define DeclPfID \
public: \
    const static ePerformerID ms_ePfID; \
    virtual int GetPfID() { return ms_ePfID; }

#define ImplPfID( classname, id ) \
    const ePerformerID classname::ms_ePfID( id )


//#endif SERV_SWAP_PERFORMER
//}}


