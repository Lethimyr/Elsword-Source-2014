#include "Performer.h"
#include <Serializer/Serializer.h>
#include <dbg/dbg.hpp>

NiImplementRTTI( KPerformer, KSimObject );

KPerformer::KPerformer(void)
{
    InitializeCriticalSection( &m_csEventQueue );
}

KPerformer::~KPerformer(void)
{
    {
        KLocker lock( m_csEventQueue );

        if( !m_queEvent.empty() )
        {
            START_LOG( cwarn, L"Object destroy with un-processed Events. Name : " << m_strName )
                << BUILD_LOG( m_queEvent.size() )
                << END_LOG;
        }
    }

    DeleteCriticalSection( &m_csEventQueue );
}

//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
ImplToStringW( KPerformer )
{
	{
		KLocker lock( m_csEventQueue );

		START_TOSTRING_PARENTW( KSimObject )
			<< TOSTRINGW( m_queEvent.size() )
			<< TOSTRINGW( m_kMaxQueueSize.m_QueueSize )
			<< TOSTRINGW( m_kMaxQueueSize.GetMaxQueueSizeRegDate() );
	}

	return stm_;
}
//}}

void KPerformer::Tick()
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

void KPerformer::QueueingEvent( const KEventPtr& spEvent_ )
{
    dbg::clog << L"ⓡ " << spEvent_->GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

    KLocker lock( m_csEventQueue );

    m_queEvent.push( spEvent_ );

	//{{ 2009. 9. 16  최육사	덤프
	if( m_queEvent.size() > m_kMaxQueueSize.m_QueueSize )
	{
		m_kMaxQueueSize.m_QueueSize		 = m_queEvent.size();
		m_kMaxQueueSize.m_tRegTime		 = CTime::GetCurrentTime();
	}
	//}}
}
/*
void KPerformer::QueueingID( UidType nSenderUID_, unsigned short usEventID_ )
{
    KEventPtr spEvent( new KEvent );

    spEvent->m_usEventID    = usEventID_;
    spEvent->m_nSenderUID   = nSenderUID_;

    KLocker lock( m_csEventQueue );
    m_queEvent.push( spEvent );
}
*/

bool KPerformer::GetKEvent( KEventPtr& spEvent )
{
    KLocker lock( m_csEventQueue );

    if( m_queEvent.empty() )
    {
        return false;
    }

    spEvent = m_queEvent.front();
    m_queEvent.pop();

    return true;
}