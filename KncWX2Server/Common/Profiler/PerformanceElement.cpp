#include "PerformanceElement.h"

TickType KPerformanceElement::m_sttTickFrequency = KPerformanceElement::GetTickFrequency();

KPerformanceElement::KPerformanceElement()
{
}

KPerformanceElement::~KPerformanceElement()
{
}

bool KPerformanceElement::BeginCheck()
{
    if( m_bIsChecking )
    {
        return false;
    }

    m_bIsChecking = true;
    m_ttStart = GetTickCount();
}

bool KPerformanceElement::EndCheck()
{
    if( !m_bIsChecking )
    {
        return false;
    }

    m_ulCount++;
    TickType ttCurrentTick = GetTickCount();
    TickType ttElapsed = ttCurrentTick - m_ttStart;
    m_ttTotal += ttElapsed;

    if( ttElapsed < m_ttMin )
    {
        m_ttMin = ttElapsed;
    }

    if( ttElapsed > m_ttMax )
    {
        m_ttMax = ttElapsed;
    }

    m_bIsChecking = false;
}

double KPerformanceElement::GetAvg()
{
    if( m_ulCount <= 0 )
    {
        return 0.0;
    }

    if( m_sttTickFrequency <= 0 )
    {
        return 0.0;
    }

    return ( double )m_ttTotal / ( double )m_sttTickFrequency / ( double )m_ulCount;
}

double KPerformanceElement::GetMin()
{
    if( m_sttTickFrequency <= 0 )
    {
        return 0.0;
    }

    return ( double )m_ttMin / ( double )m_sttTickFrequency;
}

double KPerformanceElement::GetMax()
{
    if( m_sttTickFrequency <= 0 )
    {
        return 0.0;
    }

    return ( double )m_ttMax / ( double )m_sttTickFrequency;
}

TickType KPerformanceElement::GetTickFrequency()
{
    LARGE_INTEGER liFrequency;
    if( ::QueryPerformanceFrequency( &liFrequency ) )
    {
        return liFrequency.QuadPart;
    }
    else
    {
        return ( TickType )1000;
    }
}

TickType KPerformanceElement::GetTickCount()
{
    LARGE_INTEGER liCount;
    if( ::QueryPerformanceCounter( &liCount ) )
    {
        return liCount.QuadPart;
    }
    else
    {
        return ( TickType )::GetTickCount();
    }
}