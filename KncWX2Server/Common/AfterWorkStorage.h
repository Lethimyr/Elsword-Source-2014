#pragma once
#include "Event.h"
#include <boost/timer.hpp>
#include "ServerDefine.h"

class KAfterWorkStorage
{
public:
	enum AfterWorkStorageResult
	{
		AWS_SUCCEED				= 0,
		AWS_FAILED				= 1,
		AWS_NOTKEY				= 2,
		AWS_DESERIALZE_FAILED	= 3,
#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// 작업날짜: 2013-12-21	// 박세훈
		AWS_DATA_IS_NOT_EXIST	= 4,
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS
	};

	enum AfterWorkStorageEnum
	{
		AWS_CHECK_KEY_MAX_COUNT = 10,
	};

	struct KAfterWorkStorageInfo
	{
		KEventPtr	m_spEvent;
		CTime		m_tRecordTime;

		KAfterWorkStorageInfo( IN const KEventPtr spEvent, IN const CTime tRecordTime )
			: m_spEvent( spEvent )
			, m_tRecordTime( tRecordTime )
		{
		}
	};

public:
	KAfterWorkStorage();
	~KAfterWorkStorage();

	void	Tick( void );

	template <class T>	int	InsertData( IN const DWORD dwPITo_, IN const UidType nTo_, IN const UidType anTrace_[], IN const u_short usEventID_, IN const T& data_, OUT int& iKey );
	template <class T>	int	InsertData( IN const T& data_, OUT int& iKey );

	int		InsertData( IN const KEventPtr& spEvent, OUT int& iKey );
	int		RetrieveData( IN const int iKey, OUT KEventPtr& spEvent );

	template <class T>	int	RetrieveData( IN const int iKey, OUT T& kData );

	void	DeleteData( IN const int iKey );

private:
	int		GetStockedKey( OUT int& iKey );

private:
	int										m_iKeyIndex;
	std::vector<int>						m_vecStockedKeyIndex;
	std::map<int, KAfterWorkStorageInfo>	m_mapStockedData;
	boost::timer							m_kUpdateTimer;
};

template <class T>
int KAfterWorkStorage::InsertData( IN const DWORD dwPITo_, IN const UidType nTo_, IN const UidType anTrace_[], IN const u_short usEventID_, IN const T& data_, OUT int& iKey )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( dwPITo_, anTrace_, usEventID_, data_ );
	LIF( spEvent->m_kDestPerformer.AddUID( nTo_ ) );

	return InsertData( spEvent, iKey );
}

template <class T>
int KAfterWorkStorage::InsertData( IN const T& data_, OUT int& iKey )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( 0, NULL, 0, data_ );
	return InsertData( spEvent, iKey );
}

template <class T>
int	KAfterWorkStorage::RetrieveData( IN const int iKey, OUT T& kData )
{
	KEventPtr spEvent = KEventPtr();
	int iOK = RetrieveData( iKey, spEvent );
	
	if( iOK != AWS_SUCCEED )
	{
		return iOK;
	}

	KSerializer ks;
	ks.BeginReading( &spEvent->m_kbuff );
	if( ks.Get( kData ) == false )
	{
		START_LOG( cerr, L"deserialze failed." )
			<< END_LOG;
		return AWS_DESERIALZE_FAILED;
	}
	
	spEvent->m_kbuff.Reset();
	return AWS_SUCCEED;
}
