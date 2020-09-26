#pragma once

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

class   CX2FrameUDPPack
{
public:

    CX2FrameUDPPack() 
    { 
        m_wUDPMaxSize = CKTDNUDP_MTU_MAX;
        m_pidUnitUID = 0;
        m_dwFrameMoveCount = 0;
        ResetFrameUDPPack( false );
    }

    void           SetUnitUID( UidType uidUnitUID )    { m_pidUnitUID = uidUnitUID; }
    UidType        GetUnitUID() { return m_pidUnitUID; }

    void           SetFrameMoveCount( DWORD dwFrameMoveCount ) { m_dwFrameMoveCount = dwFrameMoveCount; }
    DWORD          GetFrameMoveCount() { return m_dwFrameMoveCount; }
    void           IncreaseFrameMoveCount() { m_dwFrameMoveCount++; }

    void           UpdateUDPMaxSize();
    //void           UpdateSyncUserListAndUDPMaxSize( const std::vector<UidType>& vecSyncUserList );

    bool           AddFrameUDPPack( const char ID, const void* pData, WORD wSize, bool bReturnFalseWhenPacketFull = false );
    bool           AddFrameUDPPack_IDPack( const char ID, const void* pData, WORD wSize, bool bReturnFalseWhenPacketFull = false );
    //bool           AddFrameUDPPack_IDInfoPack( const char ID, const void* pData, WORD wSize, const void* pInfo, WORD wInfoSize );

    bool           FlushFrameUDPPack();
    void           ResetFrameUDPPack( bool bUpdateUDPMaxSize );

private:
    //std::vector<UidType>    m_vecSyncUserList;

    UidType     m_pidUnitUID;
    DWORD       m_dwFrameMoveCount;
    WORD        m_wUDPMaxSize;
    WORD        m_wFrameUDPPackCurOffset;
    WORD        m_wFrameUDPPackCurIDOffset;
    WORD        m_wFrameUDPPackEndOffset;
    BYTE        m_abFrameUDPPack[ CKTDNUDP_MTU_MAX ];


};//struct CX2FrameUDPPack


//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK