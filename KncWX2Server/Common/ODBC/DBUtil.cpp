#include "DBUtil.h"


KStoredProcedureProfiler::KStoredProcedureProfiler(void)
{
}

KStoredProcedureProfiler::~KStoredProcedureProfiler(void)
{
}

void KStoredProcedureProfiler::Clear()
{
    m_mapProfileInfo.clear();
}

bool KStoredProcedureProfiler::CheckUpdateTime( double fCheckTime /*= 1800.f*/ )
{
	if( m_tDBUpdateTimer.elapsed() > fCheckTime )
	{
		m_tDBUpdateTimer.restart();
		return true;
	}

	return false;
}

void KStoredProcedureProfiler::Check( std::wstring wstrQuery, unsigned int iDurationTime, bool bQueryFail )
{
	int iPos = 0;
	iPos = wstrQuery.find( L" ", iPos );
	if( iPos != -1 )
	{
		iPos = wstrQuery.find( L" ", ++iPos );
	}

	if( iPos != -1 )
	{
		wstrQuery = wstrQuery.substr( 0, iPos );
	}

	std::map< std::wstring, KSPInfo >::iterator mit = m_mapProfileInfo.find( wstrQuery );
	if( mit == m_mapProfileInfo.end() )
	{
		// 쿼리가 1000개가 넘는건 비정상이다!
		if( m_mapProfileInfo.size() > 1000 )
			return;

		KSPInfo kInfo;
		kInfo.m_wstrQuery = wstrQuery;
		if( bQueryFail )
		{
			kInfo.m_iTotalTime = 0;
			kInfo.m_iMaxTime = 0;
			kInfo.m_iMinTime = 0;
			kInfo.m_iQueryCount = 0;
			kInfo.m_iQueryFail = 1;
		}
		else
		{
			kInfo.m_iTotalTime = iDurationTime;
			kInfo.m_iMaxTime = iDurationTime;
			kInfo.m_iMinTime = iDurationTime;
			kInfo.m_iQueryCount = 1;
			kInfo.m_iQueryFail = 0;
		}

		if( iDurationTime > 1000 )
		{
			kInfo.m_iOver1Sec = 1;
		}
		m_mapProfileInfo.insert( std::make_pair( wstrQuery, kInfo ) );
	}
	else
	{
		if( bQueryFail )
		{
			++mit->second.m_iQueryFail;
		}
		else
		{
			if( mit->second.m_iMaxTime < iDurationTime )
				mit->second.m_iMaxTime = iDurationTime;

			if( mit->second.m_iMinTime > iDurationTime )
				mit->second.m_iMinTime = iDurationTime;

			mit->second.m_iTotalTime += iDurationTime;
			++mit->second.m_iQueryCount;
		}

		if( iDurationTime > 1000 )
		{
			mit->second.m_iOver1Sec += 1;
		}
	}
}

void KStoredProcedureProfiler::GetDump( std::vector< KSPInfo >& vecDump )
{
	vecDump.clear();

	std::map< std::wstring, KSPInfo >::const_iterator mit;
	for( mit = m_mapProfileInfo.begin(); mit != m_mapProfileInfo.end(); ++mit )
	{
		vecDump.push_back( mit->second );
	}
}


