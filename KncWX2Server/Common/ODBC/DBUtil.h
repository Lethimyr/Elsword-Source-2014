#pragma once
#include <map>
#include <vector>
#include <string>
#include <boost/timer.hpp>


#define SP_PROFILER_DUMP_TIME	14400.0


class KStoredProcedureProfiler
{
public:
	struct KSPInfo
	{
		std::wstring	m_wstrQuery;
		unsigned int	m_iMinTime;
		unsigned int	m_iMaxTime;
		unsigned int	m_iTotalTime;
		unsigned int	m_iQueryCount;
		unsigned int	m_iQueryFail;
		unsigned int	m_iOver1Sec;

		KSPInfo()
		{
			m_iMinTime = 0;
			m_iMaxTime = 0;
			m_iTotalTime = 0;
			m_iQueryCount = 0;
			m_iQueryFail = 0;
			m_iOver1Sec = 0;
		}
	};

public:
	KStoredProcedureProfiler(void);
	~KStoredProcedureProfiler(void);

	void Clear();
	bool CheckUpdateTime( double fCheckTime = 1800.f );
	void Check( std::wstring wstrQuery, unsigned int iDurationTime, bool bQueryFail );
	void GetDump( std::vector< KSPInfo >& vecDump );

private:
	std::map< std::wstring, KSPInfo >	m_mapProfileInfo;
	boost::timer						m_tDBUpdateTimer;
};


class KSPProfileHelper
{
public:
	KSPProfileHelper( KStoredProcedureProfiler& kSPP, const wchar_t* pQuery )
	{
		m_pSPP = &kSPP;
		m_pQuery = pQuery;
		m_bQueryFail = false;

		m_kTimer.restart();
	}

	~KSPProfileHelper()
	{
		if( m_pSPP == NULL  ||  m_pQuery == NULL )
			return;

		unsigned int iDurationTime = static_cast<unsigned int>( m_kTimer.elapsed() * 1000.0 );

		m_pSPP->Check( m_pQuery, iDurationTime, m_bQueryFail );
	}

	void QueryFail() { m_bQueryFail = true; }

private:
	KStoredProcedureProfiler*	m_pSPP;
	const wchar_t*				m_pQuery;
	boost::timer				m_kTimer;
	bool						m_bQueryFail;
};


