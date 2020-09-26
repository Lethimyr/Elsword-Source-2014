#include "StdAfx.h"
#include ".\KTDNUDPStatisticData.h"


void    CKTDNUDPStatisticData::Init( unsigned uPingWindowSize )
{
    m_uNumSent = 0;
    m_uNumReceived = 0;
    m_uNumLost = 0;
    m_dSumOfRoundtripTime = 0.;
    m_dSumOfRoundtripTimeSquare = 0.;
    m_fMaxRoundtripTime = 0.f;
    m_fMinRoundtripTime = 0.f;

    m_cbufPingSentInfo.resize(0);
    m_cbufPingSentInfo.set_capacity( uPingWindowSize );
    m_cbufPingRecvInfo.resize(0);
    m_cbufPingRecvInfo.set_capacity( uPingWindowSize + 1 );
    KPingRecvInfo   kRecvInfo;
    kRecvInfo.m_dwDiffSum = 0;
    kRecvInfo.m_dwDiffSquareSum = 0;
    kRecvInfo.m_uLostCount = 0;
    m_cbufPingRecvInfo.push_back( kRecvInfo );
}//CKTDNUDPStatisticData::Init()


void    CKTDNUDPStatisticData::RemoveAllPendingPingSends()
{
    if ( m_cbufPingSentInfo.empty() == true )
        return;
    unsigned    uNumHalts = m_cbufPingSentInfo.size();
    m_cbufPingSentInfo.resize(0);
    if ( m_uNumSent >= uNumHalts )
    {
        m_uNumSent -= uNumHalts;
    }
    else
    {
        m_uNumSent = 0;
        m_uNumLost = 0;
        m_uNumReceived = 0;
    }
}//CKTDNUDPStatisticData::EmptyAllPingSends()


void    CKTDNUDPStatisticData::AddPingSend( DWORD dwMySendTimestamp_ )
{
    if ( m_cbufPingSentInfo.empty() == true )
    {
        m_cbufPingSentInfo.push_back( dwMySendTimestamp_ );
        m_uNumSent++;
    }
    else
    {
        unsigned    uLostCount = 0;
        while( m_cbufPingSentInfo.empty() == false )
        {
            DWORD   dwDiff = dwMySendTimestamp_ - m_cbufPingSentInfo.front();
            if ( dwDiff < KTDNUDP_PING_EXPIRE_TIME )
                break;
            m_cbufPingSentInfo.pop_front();
            m_uNumLost++;
            uLostCount++;
        }
        if ( m_cbufPingSentInfo.full() == false )
        {
            m_cbufPingSentInfo.push_back( dwMySendTimestamp_ );
            m_uNumSent++;
        }
        else
        {
            m_cbufPingSentInfo.push_back( dwMySendTimestamp_ );
            m_uNumSent++;
            m_uNumLost++;
            uLostCount++;
        }
        if ( uLostCount != 0 && m_cbufPingRecvInfo.empty() == false )
        {
            m_cbufPingRecvInfo.back().m_uLostCount += uLostCount;
        }
    }

}//CKTDNUDPStatisticData::AddPingSend()


void    CKTDNUDPStatisticData::ProcessPingReceive( DWORD dwMySendTimestamp_, DWORD dwMyReceiveTimestamp_ )
{
    if ( m_cbufPingSentInfo.empty() == true )
        return;

    bool        bFound = false;
    for( boost::circular_buffer<DWORD>::iterator iter = m_cbufPingSentInfo.begin();
        iter != m_cbufPingSentInfo.end(); ++iter )
    {
        if ( *iter == dwMySendTimestamp_ )
        {
            if ( iter == m_cbufPingSentInfo.begin() )
            {
                m_cbufPingSentInfo.pop_front();
            }
            else
            {
                m_cbufPingSentInfo.erase( iter );
            }
            bFound = true;
            break;
        }
    }
    if ( bFound == false )
        return;

    if ( m_cbufPingRecvInfo.empty() == true )
    {
        KPingRecvInfo   kRecvInfo;
        kRecvInfo.m_dwDiffSum = 0;
        kRecvInfo.m_dwDiffSquareSum = 0;
        kRecvInfo.m_uLostCount = 0;
        m_cbufPingRecvInfo.push_back( kRecvInfo );
    }

    LONG    lRoundtripMilisec = (LONG) ( dwMyReceiveTimestamp_ - dwMySendTimestamp_ );
    if ( lRoundtripMilisec < 0 || lRoundtripMilisec >= KTDNUDP_PING_EXPIRE_TIME )
    {
        m_cbufPingRecvInfo.back().m_uLostCount++;
        m_uNumLost++;
        return;
    }

    m_uNumReceived++;

    DWORD   dwRoundtripMilisec = (DWORD) lRoundtripMilisec;
    float   fRoundtripSec = dwRoundtripMilisec * ( 1.f / 1000.f );
    if ( m_uNumReceived == 1 )
    {
        m_fMinRoundtripTime = m_fMaxRoundtripTime = fRoundtripSec;
    }
    else     
    {
        m_fMaxRoundtripTime = __max( m_fMaxRoundtripTime, fRoundtripSec );
        m_fMinRoundtripTime = __min( m_fMinRoundtripTime, fRoundtripSec );
    }
    m_dSumOfRoundtripTime += fRoundtripSec;
    m_dSumOfRoundtripTimeSquare += fRoundtripSec * fRoundtripSec;


    {
        KPingRecvInfo kRecvInfo;
        {
            KPingRecvInfo& kLastInfo = m_cbufPingRecvInfo.back();
            kRecvInfo.m_dwDiffSum = kLastInfo.m_dwDiffSum + dwRoundtripMilisec;
            kRecvInfo.m_dwDiffSquareSum = kLastInfo.m_dwDiffSquareSum + ( dwRoundtripMilisec * dwRoundtripMilisec );
            kRecvInfo.m_uLostCount = kLastInfo.m_uLostCount;
            if ( m_cbufPingRecvInfo.size() == 1 )
                kLastInfo.m_uLostCount = 0;
        }
        m_cbufPingRecvInfo.push_back( kRecvInfo );
    }

}//CKTDNUDPStatisticData::AccumulateStatisticData()



float   CKTDNUDPStatisticData::GetRoundtripTime_StandardDeviation() const  
{ 
    if ( GetNumReceived() > 0 )
    {
        float   fAverage = GetRoundtripTime_Average();
        float   fVariance = (float) ( m_dSumOfRoundtripTimeSquare / (double) GetNumReceived() ) - fAverage * fAverage;
        if ( fVariance < 0.f )
            fVariance = 0.f;
        return (float) sqrt( fVariance ); 
    }//if
    return 0.f;
}


float   CKTDNUDPStatisticData::GetWindowedRoundtripTime_Average()
{
    unsigned    uSize = m_cbufPingRecvInfo.size();
    if ( uSize < 2 )
        return 0.f;
    DWORD   dwDiffSum = m_cbufPingRecvInfo.back().m_dwDiffSum - m_cbufPingRecvInfo.front().m_dwDiffSum;
    return ( float) dwDiffSum * ( 1.f/1000.f) / (float) ( uSize - 1 );
}

float   CKTDNUDPStatisticData::GetWindowedRoundtripTime_Variance()
{
    unsigned    uSize = m_cbufPingRecvInfo.size();
    if ( uSize < 2 )
        return 0.f;

    float fAverage = GetWindowedRoundtripTime_Average();
    DWORD   dwDiffSquareSum = m_cbufPingRecvInfo.back().m_dwDiffSquareSum - m_cbufPingRecvInfo.front().m_dwDiffSquareSum;
    float fVariance = ( float) dwDiffSquareSum * (1.f/1000000.f) / (float) ( uSize - 1 ) - fAverage * fAverage;
    if ( fVariance < 0.f )
        fVariance = 0.f;
    return fVariance;
}


float   CKTDNUDPStatisticData::GetWindowedRoundtripTime_LossRatio()
{
    unsigned    uSize = m_cbufPingRecvInfo.size();
    if ( uSize < 1 )
    {
        return 0.f;
    }
    else if ( uSize == 1 )
    {
        if ( m_cbufPingRecvInfo.front().m_uLostCount == 0 )
            return 0.f;
        else
            return 1.f;
    }
    else
    {
        unsigned uLostCount = m_cbufPingRecvInfo.back().m_uLostCount - m_cbufPingRecvInfo.front().m_uLostCount;
        return (float) uLostCount / ( float ) ( uLostCount + uSize - 1 );
    }
}
