#include "SwapPerformer.h"
#include <Serializer/Serializer.h>
#include <dbg/dbg.hpp>

//{{ 2012. 04. 17	최육사	스왑 퍼포머
//#ifdef SERV_SWAP_PERFORMER


NiImplementRTTI( KSwapPerformer, KSimObject );

KSwapPerformer::KSwapPerformer(void)
{
	InitializeCriticalSection( &m_csEnqueueEvent );
    InitializeCriticalSection( &m_csDequeueEvent );
}

KSwapPerformer::~KSwapPerformer(void)
{
    {
		KLocker lockEn( m_csEnqueueEvent );
		KLocker lockDe( m_csDequeueEvent );

		if( !m_pEnqueueEvent )
		{
			if( !m_pEnqueueEvent->empty() )
			{
				START_LOG( cwarn, L"Object destroy with un-processed Events. Name : " << m_strName )
					<< BUILD_LOG( m_pEnqueueEvent->size() )
					<< END_LOG;
			}
		}

		if( m_pDequeueEvent )
		{
			if( !m_pDequeueEvent->empty() )
			{
				START_LOG( cwarn, L"Object destroy with un-processed Events. Name : " << m_strName )
					<< BUILD_LOG( m_pDequeueEvent->size() )
					<< END_LOG;
			}
		}
    }

    DeleteCriticalSection( &m_csEnqueueEvent );
	DeleteCriticalSection( &m_csDequeueEvent );
}

//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
ImplToStringW( KSwapPerformer )
{
	{
		KLocker lockEn( m_csEnqueueEvent );
		KLocker lockDe( m_csDequeueEvent );

		START_TOSTRING_PARENTW( KSimObject )
			<< TOSTRINGW( GetEnqueueSize() )
			<< TOSTRINGW( GetDequeueSize() )
			<< TOSTRINGW( m_kMaxQueueSize.m_QueueSize )
			<< TOSTRINGW( m_kMaxQueueSize.GetMaxQueueSizeRegDate() );
	}

	return stm_;
}
//}}

void KSwapPerformer::Tick()
{
    //////////////////////////////////////////////////////////////////////////
    // consume event queue

    // traverse all event
    KEventPtr spEvent;
    while( GetKEvent( spEvent ) )
    {
        // 050220. 이벤트 포인터가 NULL인지 감지한다.
        if( !spEvent )
        {
            START_LOG( cerr, L"이벤트 포인터가 NULL 임." )
                << END_LOG;

            continue;
        }

        ProcessEvent( spEvent );
    }
}

void KSwapPerformer::QueueingEvent( const KEventPtr& spEvent_ )
{
    dbg::clog << L"ⓡ " << spEvent_->GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

    KLocker lock( m_csEnqueueEvent );

	_JIF( !m_pEnqueueEvent, return; );

    m_pEnqueueEvent->push( spEvent_ );

	// 덤프
	if( m_pEnqueueEvent->size() > m_kMaxQueueSize.m_QueueSize )
	{
		m_kMaxQueueSize.m_QueueSize		 = m_pEnqueueEvent->size();
		m_kMaxQueueSize.m_tRegTime		 = CTime::GetCurrentTime();
	}
}

bool KSwapPerformer::GetKEvent( KEventPtr& spEvent )
{
	if( GetDequeueSize() == 0 )
	{
		// check and swap
		if( CheckAndSwapQueue() == false )
			return false;
	}

	{
		KLocker lock( m_csDequeueEvent );

		_JIF( !m_pDequeueEvent, return false; );

		if( m_pDequeueEvent->empty() )
			return false;

		spEvent = m_pDequeueEvent->front();
		m_pDequeueEvent->pop();
	}
    return true;
}

bool KSwapPerformer::CheckAndSwapQueue()
{
	KLocker lockEn( m_csEnqueueEvent );
	KLocker lockDe( m_csDequeueEvent );

	_JIF( !m_pEnqueueEvent, return false; );
	_JIF( !m_pDequeueEvent, return false; );
	
	if( m_pEnqueueEvent->empty() )
		return false;

	// swap
	KEventQueue* pTemp = m_pEnqueueEvent;
	m_pEnqueueEvent = m_pDequeueEvent;
	m_pDequeueEvent = pTemp;
	return true;
}


//#endif SERV_SWAP_PERFORMER
//}}

