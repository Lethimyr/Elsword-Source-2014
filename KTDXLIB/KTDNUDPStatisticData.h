#pragma once

#define KTDNUDP_PING_WINDOW_SIZE            12
#define KTDNUDP_PING_EXPIRE_TIME            5000

class CKTDNUDPStatisticData
{
public:

    CKTDNUDPStatisticData()
    {
        Init();
    }//StatisticData()

    void    Init( unsigned uPingWindowSize = KTDNUDP_PING_WINDOW_SIZE );

    void    AddPingSend( DWORD dwMySendTimestamp_ );
    void    RemoveAllPendingPingSends();
    void    ProcessPingReceive( DWORD dwMySendTimestamp, DWORD dwMyReceiveTimestamp );

    unsigned GetNumSent() const                 { return m_uNumSent; }
    unsigned GetNumCounted() const              { return m_uNumLost + m_uNumReceived; }
    unsigned GetNumLost() const                 { return m_uNumLost; }
    unsigned GetNumReceived() const             { return m_uNumReceived; }
    float   GetPacketLossRatio() const          { return ( GetNumCounted() > 0 ) ? 1.f - ( (float) m_uNumReceived / (float) GetNumCounted() ) : 0.f; }
    float   GetRoundtripTime_Average() const    { return ( GetNumReceived() > 0 ) ? (float) ( m_dSumOfRoundtripTime / (double) GetNumReceived() ) : 0.f; }
    float   GetRoundtripTime_StandardDeviation() const; 
    float   GetRoundtripTime_Max() const        { return m_fMaxRoundtripTime; }
    float   GetRoundtripTime_Min() const        { return m_fMinRoundtripTime; }

    float   GetWindowedRoundtripTime_Average();
    float   GetWindowedRoundtripTime_Variance();
    float   GetWindowedRoundtripTime_LossRatio();

private:

    unsigned    m_uNumSent;
    unsigned    m_uNumReceived;
    unsigned    m_uNumLost;
    double      m_dSumOfRoundtripTime;
    double      m_dSumOfRoundtripTimeSquare;
    float       m_fMaxRoundtripTime;
    float       m_fMinRoundtripTime;

    boost::circular_buffer<DWORD>           m_cbufPingSentInfo;

    struct KPingRecvInfo
    {
        DWORD   m_dwDiffSum;
        DWORD   m_dwDiffSquareSum;
        unsigned m_uLostCount;

        KPingRecvInfo()
            : m_dwDiffSum(0)
            , m_dwDiffSquareSum(0)
            , m_uLostCount(0)
        {
        }
    };//struct KPingRecvInfo

    boost::circular_buffer<KPingRecvInfo>   m_cbufPingRecvInfo;

};//class CKTDNUDPStatisticData