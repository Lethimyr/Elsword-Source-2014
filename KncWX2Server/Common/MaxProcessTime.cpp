#include "MaxProcessTime.h"

#include <dbg/dbg.hpp>

KMaxProcessTime::KMaxProcessTime()
{
    m_iNumRecord = 1;
}

KMaxProcessTime::KMaxProcessTime( int iNumRecord )
{
    m_iNumRecord = iNumRecord;
}

KMaxProcessTime::~KMaxProcessTime()
{
}

void KMaxProcessTime::CheckMaxProcessTime( const std::wstring& wstrMsg, float fElapsedTime )
{
    KLocker lock( m_cs );

    std::list< KElapsedTimeRecord >::iterator lit;
    for( lit = m_listElapsedTimeRank.begin(); lit != m_listElapsedTimeRank.end(); lit++ )
    {
        if( fElapsedTime > lit->m_fElapsedTime )
        {
            break;
        }
    }

    if( lit == m_listElapsedTimeRank.end() && ( ( int )m_listElapsedTimeRank.size() >= m_iNumRecord ) )
    {
        return;
    }

    KElapsedTimeRecord kRecord;
    kRecord.m_wstrMsg = wstrMsg;
    kRecord.m_fElapsedTime = fElapsedTime;
    kRecord.m_cTime = CTime::GetCurrentTime();

    m_listElapsedTimeRank.insert( lit, kRecord );

    while( ( int )m_listElapsedTimeRank.size() > m_iNumRecord )
    {
        if( m_listElapsedTimeRank.empty() )
        {
            START_LOG( cerr, L"리스트가 비었음. 발생할 수 없는 상황." )
                << BUILD_LOG( m_iNumRecord )
                << END_LOG;

            break;
        }

        m_listElapsedTimeRank.pop_back();
    }

    Dump();
}

void KMaxProcessTime::Dump()
{
    KLocker lock( m_cs );

    dbg::cout << L"--> 최대 처리 시간. 기록 수 : " << m_iNumRecord << L"개" << dbg::endl << dbg::endl;

    std::list< KElapsedTimeRecord >::iterator lit;
    for( lit = m_listElapsedTimeRank.begin(); lit != m_listElapsedTimeRank.end(); lit++ )
    {
        std::wstring wstrTime = ( lit->m_cTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) ).GetString();
#ifndef SERV_PRIVACY_AGREEMENT
        dbg::cout << L"    이름 : " << lit->m_wstrMsg << dbg::endl
#endif SERV_PRIVACY_AGREEMENT
                  << L"    소요 시간 : " << lit->m_fElapsedTime << dbg::endl
                  << L"    발생 시각 : " << wstrTime.c_str() << dbg::endl << dbg::endl;
    }

    dbg::cout << dbg::endl;
}