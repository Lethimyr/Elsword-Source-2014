#include ".\repeateventfilter.h"
#include <dbg/dbg.hpp>
#include "Event.h"

//////////////////////////////////////////////////////////////////////////
KRepeatEventFilter::KRepeatEventFilter(void)
{
	m_iRegCount = 0;
}

KRepeatEventFilter::~KRepeatEventFilter(void)
{
}

bool KRepeatEventFilter::RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	++m_iRegCount; // 등록 시도 카운트

	boost::shared_ptr< bool > spRepeatEvent( new bool );

	if( spRepeatEvent.get() == NULL )
		return false;

	*spRepeatEvent = false;
	m_mapCheckRepeatEventReq.insert( std::make_pair( usReqEventID, spRepeatEvent ) );
	m_mapCheckRepeatEventAck.insert( std::make_pair( usAckEventID, spRepeatEvent ) );
	return true;
}

bool KRepeatEventFilter::CheckReqEvent( unsigned short usEventID )
{
	// req 이벤트 부터 찾아보자
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( (*mit->second) == false )
		{
			// REQ가 걸려있지 않다면 true로 바꾸고 event를 통과 시킨다.
            *mit->second = true;
			return true;
		}
		else
		{
			// REQ가 걸려있다면 event를 통과시키지 않는다.
			return false;
		}
	}

	// 등록된게 없다면 그냥 통과시킨다
	return true;
}

void KRepeatEventFilter::CheckAckEvent( unsigned short usEventID )
{
	// req에 등록된게 없으니 ack에서 이벤트를 찾아보자
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventAck.find( usEventID );
	if( mit != m_mapCheckRepeatEventAck.end() )
	{
		// ack이벤트 발견! false값으로 변경
		*mit->second = false;
	}
}



//////////////////////////////////////////////////////////////////////////
KRepeatEventFilterNew::KRepeatEventFilterNew(void)
{
	m_iRegCount = 0;
}

KRepeatEventFilterNew::~KRepeatEventFilterNew(void)
{
}

bool KRepeatEventFilterNew::RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	++m_iRegCount; // 등록 시도 카운트

	boost::shared_ptr< bool > spRepeatEvent( new bool );

	if( spRepeatEvent.get() == NULL )
		return false;

	*spRepeatEvent = true; // 초기값은 true
	m_mapCheckRepeatEventReq.insert( std::make_pair( usReqEventID, spRepeatEvent ) );
	m_mapCheckRepeatEventAck.insert( std::make_pair( usAckEventID, spRepeatEvent ) );
	return true;
}

bool KRepeatEventFilterNew::CheckReqEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	// req 이벤트 부터 찾아보자
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usReqEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( (*mit->second) == false )
		{
			// REQ가 걸려있지 않다면 true로 바꾸고 event를 통과 시킨다.
			*mit->second = true;
			return true;
		}
		else
		{
			// REQ가 걸려있다면 event를 통과시키지 않는다.
			return false;
		}
	}

	// 등록된게 없다면 새로 등록한다!
	return RegRepeatEvent( usReqEventID, usAckEventID );
}

void KRepeatEventFilterNew::CheckAckEvent( unsigned short usEventID )
{
	// req에 등록된게 없으니 ack에서 이벤트를 찾아보자
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventAck.find( usEventID );
	if( mit != m_mapCheckRepeatEventAck.end() )
	{
		// ack이벤트 발견! false값으로 변경
		*mit->second = false;
	}
}

////{{ 2012. 02. 22	박세훈	길드 이름 변경권
//#ifdef SERV_GUILD_CHANGE_NAME
bool KRepeatEventFilterNew::JustVerifyReqEvent( unsigned short usReqEventID )
{
	// req 이벤트 부터 찾아보자
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usReqEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( (*mit->second) == false )
		{
			// REQ가 걸려있지 않다면 event를 통과시킨다.
			return true;
		}
		else
		{
			// REQ가 걸려있다면 event를 통과시키지 않는다.
			return false;
		}
	}

	// 등록된게 없다면 통과시킨다.
	return true;
}

bool KRepeatEventFilterNew::NegativeCheckReqEvent( unsigned short usReqEventID )
{
	// req 이벤트 부터 찾아보자
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usReqEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( (*mit->second) == false )
		{
			// REQ가 걸려있지 않다면 event를 통과시키지 않는다.
			return false;
		}
		else
		{
			// REQ가 걸려있다면 event를 통과시킨다.
			return true;
		}
	}

	// 등록된게 없다면 통과시키지 않는다
	return false;
}

void KRepeatEventFilterNew::RemoveReqEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	// req 이벤트 부터 찾아보자
	m_mapCheckRepeatEventReq.erase( usReqEventID );
	m_mapCheckRepeatEventAck.erase( usAckEventID );
}
//#endif SERV_GUILD_CHANGE_NAME
////}}


//////////////////////////////////////////////////////////////////////////
KRepeatEventCountFilter::KRepeatEventCountFilter(void)
{
	m_iRegCount = 0;
	m_iCheckedFilteringCount = 0;
	m_fFilteringTime = 0.0;
}

KRepeatEventCountFilter::~KRepeatEventCountFilter(void)
{
}

bool KRepeatEventCountFilter::RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	++m_iRegCount; // 등록 시도 카운트

	boost::shared_ptr< KEventFilterInfo > spRepeatEvent( new KEventFilterInfo );

	if( spRepeatEvent.get() == NULL )
		return false;

	m_mapCheckRepeatEventReq.insert( std::make_pair( usReqEventID, spRepeatEvent ) );
	m_mapCheckRepeatEventAck.insert( std::make_pair( usAckEventID, spRepeatEvent ) );
	return true;
}

bool KRepeatEventCountFilter::CheckReqEvent( unsigned short usEventID )
{
	// req 이벤트 부터 찾아보자
	KRepeatEventFilterMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( mit->second == NULL )
		{
			// 포인터값이 이상하면 event를 통과시키지 않는다.
			return false;
		}

		if( mit->second->m_bRecvReq == false )
		{
			// REQ가 걸려있지 않다면 true로 바꾸고 event를 통과 시킨다.
			mit->second->m_bRecvReq = true;
			return true;
		}
		else
		{
			// 필터링 카운트가 0이라면 타이머를 초기화 시킨다!
			if( mit->second->m_iFilteringCount == 0 )
			{
				mit->second->m_kTimer.restart();
			}

			// 필터링 카운트롤 증가시킨다.
			++mit->second->m_iFilteringCount;

			// 카운트된 값을 따로 저장한다.
			m_usCheckedEventID			= usEventID;
			m_iCheckedFilteringCount	= mit->second->m_iFilteringCount;
			m_fFilteringTime			= mit->second->m_kTimer.elapsed();

			// REQ가 걸려있다면 event를 통과시키지 않는다.
			return false;
		}
	}

	// 등록된게 없다면 그냥 통과시킨다
	return true;
}

void KRepeatEventCountFilter::CheckAckEvent( unsigned short usEventID )
{
	// req에 등록된게 없으니 ack에서 이벤트를 찾아보자
	KRepeatEventFilterMap::iterator mit;
	mit = m_mapCheckRepeatEventAck.find( usEventID );
	if( mit != m_mapCheckRepeatEventAck.end() )
	{
		if( mit->second == NULL )
		{
			// 포인터값이 이상하면 바로 리턴.
			return;
		}

		// ack이벤트 발견! false값으로 변경
		mit->second->m_bRecvReq = false;

		// 필터링 카운트값도 0으로 바꾼다.
		mit->second->m_iFilteringCount = 0;
	}
}



