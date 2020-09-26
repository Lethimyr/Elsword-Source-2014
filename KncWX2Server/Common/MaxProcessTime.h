#pragma once

#include <list>
#include <atltime.h>
#include <Thread/Locker.h>

class KMaxProcessTime
{
public:
    struct KElapsedTimeRecord
    {
        std::wstring    m_wstrMsg;
        float           m_fElapsedTime;
        CTime           m_cTime;

        KElapsedTimeRecord()
        {
            m_fElapsedTime = 0.f;
            m_cTime = CTime::GetCurrentTime();
        }
		KElapsedTimeRecord( const KElapsedTimeRecord& t ) { *this = t; }    // copy constructor
		KElapsedTimeRecord& operator=( const KElapsedTimeRecord& t )       // assign operator
		{
			m_wstrMsg = t.m_wstrMsg;
			m_fElapsedTime = t.m_fElapsedTime;
			m_cTime = t.m_cTime;
			return *this;
		}
    };

    KMaxProcessTime();
    KMaxProcessTime( int iNumRecord );
    virtual ~KMaxProcessTime();

    void CheckMaxProcessTime( const std::wstring& wstrMsg, float fElapsedTime );
    void Dump();

protected:

public:
protected:
    mutable KncCriticalSection          m_cs;
    int                                 m_iNumRecord;
    std::list< KElapsedTimeRecord >     m_listElapsedTimeRank;
};