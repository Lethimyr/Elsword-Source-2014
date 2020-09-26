#include "AfterWorkStorage.h"

#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// 작업날짜: 2013-12-21	// 박세훈
#include "GameSysVal/GameSysVal.h"
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS

KAfterWorkStorage::KAfterWorkStorage()
: m_iKeyIndex( 0 )
{
	;
}

KAfterWorkStorage::~KAfterWorkStorage()
{
	;
}

void KAfterWorkStorage::Tick( void )
{
	if( m_kUpdateTimer.elapsed() < 5.0 )
	{
		return;
	}
	m_kUpdateTimer.restart();

	CTimeSpan tGab;
#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// 작업날짜: 2013-12-21	// 박세훈
	if( SiKGameSysVal()->GetAwsCriterionNum() <= static_cast<int>( m_mapStockedData.size() ) )
	{
		tGab = CTimeSpan( SiKGameSysVal()->GetAwsAboveOrEqualTerm() );
	}
	else
	{
		tGab = CTimeSpan( SiKGameSysVal()->GetAwsUnderTerm() );
	}
#else // SERV_FIX_AFTER_WORK_STORAGE_CLASS
	if( 100 <= m_mapStockedData.size() )
	{
		tGab = CTimeSpan( 0, 0, 0, 1 );
	}
	else
	{
		tGab = CTimeSpan( 0, 0, 0, 60 );
	}
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS

	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map<int, KAfterWorkStorageInfo>::iterator it;
	for( it = m_mapStockedData.begin(); it != m_mapStockedData.end(); )
	{
		if( tGab <= ( tCurrentTime - it->second.m_tRecordTime ) )
		{
			it = m_mapStockedData.erase( it );
		}
		else
		{
			++it;
		}
	}
}

int KAfterWorkStorage::InsertData( IN const KEventPtr& spEvent, OUT int& iKey )
{
	int iOK = GetStockedKey( iKey );

	if( iOK != AWS_SUCCEED )
	{
		return iOK;
	}

	m_mapStockedData.insert( std::map<int, KAfterWorkStorageInfo>::value_type( iKey, KAfterWorkStorageInfo( spEvent, CTime::GetCurrentTime() ) ) );

	return AWS_SUCCEED;
}

int KAfterWorkStorage::RetrieveData( IN const int iKey, OUT KEventPtr& spEvent )
{
	std::map<int, KAfterWorkStorageInfo>::iterator it = m_mapStockedData.find( iKey );
	if( it == m_mapStockedData.end() )
	{
		spEvent = KEventPtr();
#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// 작업날짜: 2013-12-21	// 박세훈
		return AWS_DATA_IS_NOT_EXIST;
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS
	}
	else
	{
		spEvent = it->second.m_spEvent;
		m_mapStockedData.erase( it );
		m_vecStockedKeyIndex.push_back( iKey );
	}

	return AWS_SUCCEED;
}

int KAfterWorkStorage::GetStockedKey( OUT int& iKey )
{
	if( m_vecStockedKeyIndex.empty() == false )
	{
		iKey = m_vecStockedKeyIndex.back();
		m_vecStockedKeyIndex.pop_back();
		return AWS_SUCCEED;
	}

	for( int i=0; i < AWS_CHECK_KEY_MAX_COUNT; ++i )
	{
		++m_iKeyIndex;
		
		if( m_iKeyIndex == 0 )
		{
			++m_iKeyIndex;
		}

		if( m_mapStockedData.find( m_iKeyIndex ) == m_mapStockedData.end() )
		{
			iKey = m_iKeyIndex;
			return AWS_SUCCEED;
		}
	}

	return AWS_NOTKEY;
}

void KAfterWorkStorage::DeleteData( IN const int iKey )
{
	m_mapStockedData.erase( iKey );
	m_vecStockedKeyIndex.push_back( iKey );
}