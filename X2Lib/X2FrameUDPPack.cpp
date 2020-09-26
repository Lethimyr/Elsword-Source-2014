#include "StdAfx.h"
#include ".\X2FrameUDPPack.h"

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void    CX2FrameUDPPack::UpdateUDPMaxSize()
{
    //m_vecSyncUserList.resize(0);
    m_wUDPMaxSize = 0;
    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL )
    {
        int iData = g_pData->GetGameUDP()->EstimateEffectiveMTUSize();
        if ( iData >= sizeof(WORD) )
        {
            m_wUDPMaxSize = (WORD) iData;
        }
    }
}

//void    CX2FrameUDPPack::UpdateSyncUserListAndUDPMaxSize( const std::vector<UidType>& vecSyncUserList )
//{
//    m_vecSyncUserList.resize(0);
//    m_vecSyncUserList.reserve( vecSyncUserList.size() );
//    m_wUDPMaxSize = 0;
//    unsigned    uNumRelays = 0;
//    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL )
//    {
//        UidType MyUID = g_pData->GetGameUDP()->GetMyUID();
//        BOOST_FOREACH( UidType uidType, vecSyncUserList )
//        {
//            const CKTDNUDP::Peer* pPeer = g_pData->GetGameUDP()->GetPeer( uidType );
//            if ( pPeer != NULL && pPeer->m_UID != MyUID )
//            {
//                if ( pPeer->m_bUseRelay )
//                    uNumRelays++;
//                m_vecSyncUserList.push_back( uidType );
//            }
//        }
//        int iData = 0;
//        if ( uNumRelays == 0 )
//        {
//            iData = (int) CKTDNUDP_MTU_MAX - 1;
//        }
//        else
//        {
//            iData = (int) CKTDNUDP_MTU_MAX - (int) ( 3 + uNumRelays * sizeof(__int64) );
//        }
//        if ( iData >= sizeof(WORD) )
//        {
//            m_wUDPMaxSize = (WORD) iData;
//        }
//    }
//}

bool    CX2FrameUDPPack::AddFrameUDPPack( const char ID, const void* pData, WORD wSize, bool bReturnFalseWhenPacketFull )
{
    ASSERT( wSize == 0 || pData != NULL );
    if ( wSize != 0 && pData == NULL )
        return false;

    WORD   wCurMaxDataSize = m_wUDPMaxSize;

    ASSERT( m_wFrameUDPPackCurOffset <= wCurMaxDataSize );
    ASSERT( m_wFrameUDPPackCurIDOffset <= wCurMaxDataSize );
    ASSERT( m_wFrameUDPPackEndOffset <= wCurMaxDataSize );
    ASSERT( m_wFrameUDPPackCurOffset <= m_wFrameUDPPackCurIDOffset );
    ASSERT( m_wFrameUDPPackCurOffset <= m_wFrameUDPPackEndOffset );
    ASSERT( m_wFrameUDPPackCurIDOffset <= m_wFrameUDPPackEndOffset );
    if (m_wFrameUDPPackCurOffset > wCurMaxDataSize
        || m_wFrameUDPPackCurIDOffset > wCurMaxDataSize
        || m_wFrameUDPPackEndOffset > wCurMaxDataSize
        || m_wFrameUDPPackCurOffset > m_wFrameUDPPackCurIDOffset
        || m_wFrameUDPPackCurOffset > m_wFrameUDPPackEndOffset
        || m_wFrameUDPPackCurIDOffset > m_wFrameUDPPackEndOffset )
    {
        ResetFrameUDPPack( false );
        return false;
    }//if

    if ( m_wFrameUDPPackCurOffset + sizeof(WORD) <= m_wFrameUDPPackEndOffset )
    {
        WORD* pwSize = (WORD*) &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset ];
        ASSERT( m_wFrameUDPPackCurOffset + sizeof(WORD) + *pwSize == m_wFrameUDPPackEndOffset );
        if ( m_wFrameUDPPackEndOffset + 1 + wSize > wCurMaxDataSize )
        {
            BYTE    szDstBuffer[CKTDNUDP_MTU_MAX];
            uLongf ulDstSize = CKTDNUDP_MTU_MAX;
            int iError = compress( (BYTE*)szDstBuffer, &ulDstSize, (BYTE*)&m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ], *pwSize );
            if ( iError != Z_OK || ulDstSize >= *pwSize )
            {
                m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
                FlushFrameUDPPack();
                if ( bReturnFalseWhenPacketFull == true )
                    return false;
            }
		    else
		    {
                *pwSize = ( (WORD) ulDstSize ) | 0x8000;
                CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ], szDstBuffer, ulDstSize );
                m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset + sizeof(WORD) + (WORD) ulDstSize;
                m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
		    }//if.. else..
        }
        else
        {
            *pwSize += 1 + wSize;
            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = ID;
            m_wFrameUDPPackEndOffset++;
            CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pData, wSize );
            m_wFrameUDPPackEndOffset += wSize;
            m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
            return true;
        }
    }

    if ( m_wFrameUDPPackCurOffset == m_wFrameUDPPackEndOffset )
    {
        if ( m_wFrameUDPPackEndOffset + sizeof(WORD) + 1 + wSize > wCurMaxDataSize )
        {
            FlushFrameUDPPack();
            if ( bReturnFalseWhenPacketFull == true )
                return false;
        }
        ASSERT( m_wFrameUDPPackCurOffset == m_wFrameUDPPackEndOffset );
        if ( m_wFrameUDPPackEndOffset + sizeof(WORD) + 1 + wSize <= wCurMaxDataSize )
        {
            m_wFrameUDPPackCurOffset = m_wFrameUDPPackEndOffset;
            WORD* pwSize = (WORD*) &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset ];
            *pwSize = 1 + wSize;
            m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ] = ID;
            CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) + 1 ], pData, wSize );
            m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset + sizeof(WORD) + 1 + wSize;
            m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
            return true;
        }
    }

    ResetFrameUDPPack( false );
    return false;

}//CX2FrameUDPPack::AddFrameUDPPack()


bool    CX2FrameUDPPack::AddFrameUDPPack_IDPack( const char ID, const void* pData, WORD wSize, bool bReturnFalseWhenPacketFull )
{
    ASSERT( wSize == 0 || pData != NULL );
    if ( wSize != 0 && pData == NULL )
        return false;

    WORD   wCurMaxDataSize = m_wUDPMaxSize;

    ASSERT( m_wFrameUDPPackCurOffset <= wCurMaxDataSize );
    ASSERT( m_wFrameUDPPackCurIDOffset <= wCurMaxDataSize );
    ASSERT( m_wFrameUDPPackEndOffset <= wCurMaxDataSize );
    ASSERT( m_wFrameUDPPackCurOffset <= m_wFrameUDPPackCurIDOffset );
    ASSERT( m_wFrameUDPPackCurOffset <= m_wFrameUDPPackEndOffset );
    ASSERT( m_wFrameUDPPackCurIDOffset <= m_wFrameUDPPackEndOffset );
    if (m_wFrameUDPPackCurOffset > wCurMaxDataSize
        || m_wFrameUDPPackCurIDOffset > wCurMaxDataSize
        || m_wFrameUDPPackEndOffset > wCurMaxDataSize
        || m_wFrameUDPPackCurOffset > m_wFrameUDPPackCurIDOffset
        || m_wFrameUDPPackCurOffset > m_wFrameUDPPackEndOffset
        || m_wFrameUDPPackCurIDOffset > m_wFrameUDPPackEndOffset )
    {
        ResetFrameUDPPack( false );
        return false;
    }//if

    if ( m_wFrameUDPPackCurOffset + sizeof(WORD) <= m_wFrameUDPPackEndOffset )
    {
        WORD* pwSize = (WORD*) &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset ];
        ASSERT( m_wFrameUDPPackCurOffset + sizeof(WORD) + *pwSize == m_wFrameUDPPackEndOffset );
        ASSERT( m_wFrameUDPPackCurOffset + sizeof(WORD) <= m_wFrameUDPPackCurIDOffset );
        if ( m_wFrameUDPPackCurIDOffset + 2 <= m_wFrameUDPPackEndOffset )
        {
            if ( m_abFrameUDPPack[ m_wFrameUDPPackCurIDOffset ] == ID
                && m_abFrameUDPPack[ m_wFrameUDPPackCurIDOffset + 1 ] < 255
                && m_wFrameUDPPackEndOffset + wSize <= wCurMaxDataSize )
            {
                *pwSize += wSize;
                m_abFrameUDPPack[ m_wFrameUDPPackCurIDOffset + 1 ]++;
                CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pData, wSize );
                m_wFrameUDPPackEndOffset += wSize;
                return true;
            }
        }
        if ( m_wFrameUDPPackEndOffset + 2 + wSize > wCurMaxDataSize )
        {
            BYTE    szDstBuffer[CKTDNUDP_MTU_MAX];
            uLongf ulDstSize = CKTDNUDP_MTU_MAX;
            int iError = compress( (BYTE*)szDstBuffer, &ulDstSize, (BYTE*)&m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ], *pwSize );
            if ( iError != Z_OK || ulDstSize >= *pwSize )
            {
                m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
                FlushFrameUDPPack();
                if ( bReturnFalseWhenPacketFull == true )
                    return false;
            }
		    else
		    {
                *pwSize = ( (WORD) ulDstSize ) | 0x8000;
                CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ], szDstBuffer, ulDstSize );
                m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset + sizeof(WORD) + (WORD) ulDstSize;
                m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
		    }//if.. else..
        }
        else
        {
            *pwSize += 2 + wSize;
            m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = ID;
            m_wFrameUDPPackEndOffset++;
            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = 1;
            m_wFrameUDPPackEndOffset++;
            CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pData, wSize );
            m_wFrameUDPPackEndOffset += wSize;
            return true;
        }
    }

    if ( m_wFrameUDPPackCurOffset == m_wFrameUDPPackEndOffset )
    {
        if ( m_wFrameUDPPackEndOffset + sizeof(WORD) + 2 + wSize > wCurMaxDataSize )
        {
            FlushFrameUDPPack();
            if ( bReturnFalseWhenPacketFull == true )
                return false;
        }
        ASSERT( m_wFrameUDPPackCurOffset == m_wFrameUDPPackEndOffset );
        if ( m_wFrameUDPPackEndOffset + sizeof(WORD) + 2 + wSize <= wCurMaxDataSize )
        {
            m_wFrameUDPPackCurOffset = m_wFrameUDPPackEndOffset;
            WORD* pwSize = (WORD*) &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset ];
            *pwSize = 2 + wSize;
            m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset + 2;
            m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = ID;
            m_wFrameUDPPackEndOffset++;
            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = 1;
            m_wFrameUDPPackEndOffset++;
            CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pData, wSize );
            m_wFrameUDPPackEndOffset += wSize;
            return true;
        }
    }

    ResetFrameUDPPack( false );
    return false;
}


//bool    CX2FrameUDPPack::AddFrameUDPPack_IDInfoPack( const char ID, const void* pData, WORD wSize, const void* pInfo, WORD wInfoSize )
//{
//    ASSERT( wSize == 0 || pData != NULL );
//    ASSERT( wInfoSize == 0 || pInfo != NULL );
//    if ( wSize != 0 && pData == NULL 
//        || wInfoSize != 0 && pInfo == NULL )
//        return false;
//
//    WORD   wCurMaxDataSize = m_wUDPMaxSize;
//
//    ASSERT( m_wFrameUDPPackCurOffset <= wCurMaxDataSize );
//    ASSERT( m_wFrameUDPPackCurIDOffset <= wCurMaxDataSize );
//    ASSERT( m_wFrameUDPPackEndOffset <= wCurMaxDataSize );
//    ASSERT( m_wFrameUDPPackCurOffset <= m_wFrameUDPPackCurIDOffset );
//    ASSERT( m_wFrameUDPPackCurOffset <= m_wFrameUDPPackEndOffset );
//    ASSERT( m_wFrameUDPPackCurIDOffset <= m_wFrameUDPPackEndOffset );
//    if (m_wFrameUDPPackCurOffset > wCurMaxDataSize
//        || m_wFrameUDPPackCurIDOffset > wCurMaxDataSize
//        || m_wFrameUDPPackEndOffset > wCurMaxDataSize
//        || m_wFrameUDPPackCurOffset > m_wFrameUDPPackCurIDOffset
//        || m_wFrameUDPPackCurOffset > m_wFrameUDPPackEndOffset
//        || m_wFrameUDPPackCurIDOffset > m_wFrameUDPPackEndOffset )
//    {
//        ResetFrameUDPPack();
//        return false;
//    }//if
//
//    if ( m_wFrameUDPPackCurOffset + sizeof(WORD) <= m_wFrameUDPPackEndOffset )
//    {
//        WORD* pwSize = (WORD*) &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset ];
//        ASSERT( m_wFrameUDPPackCurOffset + sizeof(WORD) + *pwSize == m_wFrameUDPPackEndOffset );
//        ASSERT( m_wFrameUDPPackCurOffset + sizeof(WORD) <= m_wFrameUDPPackCurIDOffset );
//        if ( m_wFrameUDPPackCurIDOffset + 2 + wInfoSize <= m_wFrameUDPPackEndOffset )
//        {
//            if ( m_abFrameUDPPack[ m_wFrameUDPPackCurIDOffset ] == ID
//                && m_abFrameUDPPack[ m_wFrameUDPPackCurIDOffset + 1 ] > 0
//                && m_abFrameUDPPack[ m_wFrameUDPPackCurIDOffset + 1 ] < 255
//                && m_wFrameUDPPackEndOffset + wSize <= wCurMaxDataSize )
//            {
//                *pwSize += wSize;
//                m_abFrameUDPPack[ m_wFrameUDPPackCurIDOffset + 1 ]++;
//                CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pData, wSize );
//                m_wFrameUDPPackEndOffset += wSize;
//                return true;
//            }
//        }
//        if ( m_wFrameUDPPackEndOffset + 2 + wInfoSize + wSize > wCurMaxDataSize )
//        {
//            BYTE    szDstBuffer[CKTDNUDP_MTU_MAX];
//            uLongf ulDstSize = CKTDNUDP_MTU_MAX;
//            int iError = compress( (BYTE*)szDstBuffer, &ulDstSize, (BYTE*)&m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ], *pwSize );
//            if ( iError != Z_OK || ulDstSize >= *pwSize )
//            {
//                m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
//                FlushFrameUDPPack();
//            }
//		    else
//		    {
//                *pwSize = ( (WORD) ulDstSize ) | 0x8000;
//                CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ], szDstBuffer, ulDstSize );
//                m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset + sizeof(WORD) + (WORD) ulDstSize;
//                m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
//		    }//if.. else..
//        }
//        else
//        {
//            *pwSize += 2 + wInfoSize + wSize;
//            m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
//            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = ID;
//            m_wFrameUDPPackEndOffset++;
//            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = 1;
//            m_wFrameUDPPackEndOffset++;
//            CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pInfo, wInfoSize );
//            m_wFrameUDPPackEndOffset += wInfoSize;
//            CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pData, wSize );
//            m_wFrameUDPPackEndOffset += wSize;
//            return true;
//        }
//    }
//
//    if ( m_wFrameUDPPackCurOffset == m_wFrameUDPPackEndOffset )
//    {
//        if ( m_wFrameUDPPackEndOffset + sizeof(WORD) + 2 + wInfoSize + wSize > wCurMaxDataSize )
//            FlushFrameUDPPack();
//        ASSERT( m_wFrameUDPPackCurOffset == m_wFrameUDPPackEndOffset );
//        if ( m_wFrameUDPPackEndOffset + sizeof(WORD) + 2 + wInfoSize + wSize <= wCurMaxDataSize )
//        {
//            m_wFrameUDPPackCurOffset = m_wFrameUDPPackEndOffset;
//            WORD* pwSize = (WORD*) &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset ];
//            *pwSize = 2 + wInfoSize + wSize;
//            m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset + 2;
//            m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
//            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = ID;
//            m_wFrameUDPPackEndOffset++;
//            m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ] = 1;
//            m_wFrameUDPPackEndOffset++;
//            CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pInfo, wInfoSize );
//            m_wFrameUDPPackEndOffset += wInfoSize;
//            CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackEndOffset ], pData, wSize );
//            m_wFrameUDPPackEndOffset += wSize;
//            return true;
//        }
//    }
//
//    ResetFrameUDPPack();
//    return false;
//}



bool    CX2FrameUDPPack::FlushFrameUDPPack()
{
    WORD   wCurMaxDataSize = m_wUDPMaxSize;

    if ( m_wFrameUDPPackEndOffset < sizeof(XPT_UDP_PACKET_PACK_PREFIX) + sizeof(WORD) )
    {
        ResetFrameUDPPack( false );
        return false;
    }

    ASSERT( m_wFrameUDPPackCurOffset <= wCurMaxDataSize );
    ASSERT( m_wFrameUDPPackEndOffset <= wCurMaxDataSize );
    ASSERT( m_wFrameUDPPackCurOffset <= m_wFrameUDPPackCurIDOffset );
    ASSERT( m_wFrameUDPPackCurOffset <= m_wFrameUDPPackEndOffset );
    if (m_wFrameUDPPackCurOffset > wCurMaxDataSize
        || m_wFrameUDPPackEndOffset > wCurMaxDataSize
        || m_wFrameUDPPackCurOffset > m_wFrameUDPPackEndOffset )
    {
        ResetFrameUDPPack( false );
        return false;
    }//if

    if ( m_wFrameUDPPackCurOffset + sizeof(WORD) <= m_wFrameUDPPackEndOffset )
    {
        WORD* pwSize = (WORD*) &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset ];
        WORD wSize = *pwSize & 0x7fff;
        bool bCompressed = ( *pwSize & 0x8000 ) != 0;
        if ( wSize == 0 )
        {
            m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset;
        }
        else if ( m_wFrameUDPPackCurOffset + sizeof(WORD) + wSize > m_wFrameUDPPackEndOffset )
        {
            ResetFrameUDPPack( false );
            return false;
        }
        if ( bCompressed == false )
        {
            BYTE    szDstBuffer[CKTDNUDP_MTU_MAX];
            uLongf ulDstSize = CKTDNUDP_MTU_MAX;
            int iError = compress( (BYTE*)szDstBuffer, &ulDstSize, &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ], *pwSize );
            if ( iError != Z_OK || ulDstSize >= *pwSize )
            {
                m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
            }
		    else
		    {
                *pwSize = ( (WORD) ulDstSize ) | 0x8000;
                CopyMemory( &m_abFrameUDPPack[ m_wFrameUDPPackCurOffset + sizeof(WORD) ], szDstBuffer, ulDstSize );
                m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset + sizeof(WORD) + (WORD) ulDstSize;
                m_wFrameUDPPackCurOffset = m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackEndOffset;
		    }
        }
    }

    //if ( m_vecSyncUserList.empty() == true )
    //{
        g_pData->GetGameUDP()->BroadCast( XPT_UDP_PACKET_PACK, &m_abFrameUDPPack[0], m_wFrameUDPPackEndOffset );
    //}
    //else
    //{
    //    g_pData->GetGameUDP()->BroadCast( m_vecSyncUserList, XPT_UDP_PACKET_PACK, &m_abFrameUDPPack[0], m_wFrameUDPPackEndOffset );
    //}
    ResetFrameUDPPack( true );

    return true;

}//CX2FrameUDPPack::FlushFrameUDPPack()


void    CX2FrameUDPPack::ResetFrameUDPPack( bool bUpdateUDPMaxSize = false )
{
    XPT_UDP_PACKET_PACK_PREFIX* pkPrefix = ( (XPT_UDP_PACKET_PACK_PREFIX*) &m_abFrameUDPPack[0] );
    pkPrefix->m_iUnitUID = m_pidUnitUID;
    pkPrefix->m_dwFrameMoveCount = m_dwFrameMoveCount;
    m_wFrameUDPPackCurOffset = sizeof(XPT_UDP_PACKET_PACK_PREFIX);
    m_wFrameUDPPackCurIDOffset = m_wFrameUDPPackCurOffset;
    m_wFrameUDPPackEndOffset = m_wFrameUDPPackCurOffset;

    if ( bUpdateUDPMaxSize == true )
    {
        UpdateUDPMaxSize();
    }
}//CX2FrameUDPPack::ResetFrameUDPPack()

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK