#pragma once
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/timer.hpp>



//////////////////////////////////////////////////////////////////////////
class KRepeatEventFilter
{
public:
	KRepeatEventFilter(void);
	~KRepeatEventFilter(void);

	bool	RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID );

	bool	CheckReqEvent( unsigned short usEventID );
	void	CheckAckEvent( unsigned short usEventID );

	int		GetReqEventCount()	{ return m_mapCheckRepeatEventReq.size(); }
	int		GetAckEventCount()	{ return m_mapCheckRepeatEventAck.size(); }
	int		GetRegCount()		{ return m_iRegCount; }

private:
	typedef std::map< unsigned short, boost::shared_ptr< bool > > KRepeatEventMap;

	KRepeatEventMap m_mapCheckRepeatEventReq;
	KRepeatEventMap m_mapCheckRepeatEventAck;

	int				m_iRegCount;
};




//////////////////////////////////////////////////////////////////////////
class KRepeatEventFilterNew
{
public:
	KRepeatEventFilterNew(void);
	~KRepeatEventFilterNew(void);

	bool	CheckReqEvent( unsigned short usReqEventID, unsigned short usAckEventID );
	void	CheckAckEvent( unsigned short usAckEventID );

	int		GetReqEventCount()	{ return m_mapCheckRepeatEventReq.size(); }
	int		GetAckEventCount()	{ return m_mapCheckRepeatEventAck.size(); }
	int		GetRegCount()		{ return m_iRegCount; }

//	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
//#ifdef SERV_GUILD_CHANGE_NAME
	bool JustVerifyReqEvent( unsigned short usReqEventID );
	bool NegativeCheckReqEvent( unsigned short usReqEventID );
	void RemoveReqEvent( unsigned short usReqEventID, unsigned short usAckEventID );
//#endif SERV_GUILD_CHANGE_NAME
//	//}}

private:
	bool	RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID );

private:
	typedef std::map< unsigned short, boost::shared_ptr< bool > > KRepeatEventMap;

	KRepeatEventMap m_mapCheckRepeatEventReq;
	KRepeatEventMap m_mapCheckRepeatEventAck;

	int				m_iRegCount;
};




//////////////////////////////////////////////////////////////////////////
//{{ 2010. 8. 31	최육사	중복 패킷 이벤트 카운트
// SERV_EVENT_FILTER_COUNT
class KRepeatEventCountFilter
{
public:
	struct KEventFilterInfo
	{
        bool			m_bRecvReq;
		int				m_iFilteringCount;
		boost::timer	m_kTimer;

		KEventFilterInfo()
		{
			m_bRecvReq = 0;
			m_iFilteringCount = 0;
		}
	};

public:
	KRepeatEventCountFilter(void);
	~KRepeatEventCountFilter(void);

	bool	RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID );

	bool	CheckReqEvent( unsigned short usEventID );
	void	CheckAckEvent( unsigned short usEventID );

	int		GetReqEventCount()			{ return m_mapCheckRepeatEventReq.size(); }
	int		GetAckEventCount()			{ return m_mapCheckRepeatEventAck.size(); }
	int		GetRegCount()				{ return m_iRegCount; }

	unsigned short GetCheckedEventID()	{ return m_usCheckedEventID; }
	int		GetCheckedFilteringCount()	{ return m_iCheckedFilteringCount; }
	double	GetCheckedFilteringTime()	{ return m_fFilteringTime; }

private:
	typedef std::map< unsigned short, boost::shared_ptr< KEventFilterInfo > > KRepeatEventFilterMap;

	KRepeatEventFilterMap	m_mapCheckRepeatEventReq;
	KRepeatEventFilterMap	m_mapCheckRepeatEventAck;

	int						m_iRegCount;
	unsigned short			m_usCheckedEventID;
	int						m_iCheckedFilteringCount;
	double					m_fFilteringTime;
};
// SERV_EVENT_FILTER_COUNT
//}}




//////////////////////////////////////////////////////////////////////////
class KTransactionGuarantor
{
	
public:
	bool RegisterMutualExclusiveEvent( unsigned short usReqEventID1, unsigned short usAckEventID1, unsigned short usReqEventID2, unsigned short usAckEventID2 )
	{
		if( usReqEventID1 == usReqEventID2 )
			return false;

		if( usAckEventID1 == usAckEventID2 )
			return false;

		m_mapMutualExclusion[ usReqEventID1 ].push_back( usReqEventID2 );
		m_mapMutualExclusion[ usReqEventID2 ].push_back( usReqEventID1 );




		boost::shared_ptr< bool > spEventCheckA( new bool );
		if( spEventCheckA.get() == NULL )
			return false;

		*spEventCheckA = false;
		m_mapInTransaction.insert( std::make_pair( usReqEventID1, spEventCheckA ) );
		m_mapInTransaction.insert( std::make_pair( usAckEventID1, spEventCheckA ) );


		boost::shared_ptr< bool > spEventCheckB( new bool );
		if( spEventCheckB.get() == NULL )
			return false;

		*spEventCheckB = false;
		m_mapInTransaction.insert( std::make_pair( usReqEventID2, spEventCheckB ) );
		m_mapInTransaction.insert( std::make_pair( usAckEventID2, spEventCheckB ) );
		

		return true;
	}



	bool IsTransactionEnabled( unsigned short usEventID, unsigned short& usBlockingEventID )
	{
		KMutualExclusionMap::iterator mit = m_mapMutualExclusion.find( usEventID );
		if( mit != m_mapMutualExclusion.end() )
		{
			std::vector<unsigned short>& vecExclusiveEvent = mit->second;
			for( int i=0; i<(int)vecExclusiveEvent.size(); i++ )
			{
				KInTransactionMap::iterator mitInTransaction = m_mapInTransaction.find( vecExclusiveEvent[i] );
				if( mitInTransaction != m_mapInTransaction.end() )
				{
					if( (*mitInTransaction->second) == true )
					{
						// REQ가 걸려있다면 event를 통과시키지 않는다.
						usBlockingEventID = mitInTransaction->first;
						return false;
					}
				}
			}
		}

		return true;
	}


	void CheckReqEvent( unsigned short usEventID )
	{
		KInTransactionMap::iterator mitInTransaction = m_mapInTransaction.find( usEventID );
		if( mitInTransaction != m_mapInTransaction.end() )
		{
			*mitInTransaction->second = true;
		}
	}
		
	void CheckAckEvent( unsigned short usEventID )
	{
		KInTransactionMap::iterator mitInTransaction = m_mapInTransaction.find( usEventID );
		if( mitInTransaction != m_mapInTransaction.end() )
		{
			*mitInTransaction->second = false;
		}
	}


private:
	typedef std::map< unsigned short, boost::shared_ptr< bool > >		KInTransactionMap;
	typedef std::map< unsigned short, std::vector<unsigned short> >		KMutualExclusionMap;		// <event_id, vector<event_id> >

	KInTransactionMap m_mapInTransaction;
	KMutualExclusionMap m_mapMutualExclusion;
};