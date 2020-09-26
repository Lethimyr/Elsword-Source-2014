#include "StdAfx.h"
#include ".\ktdntransfer.h"

#ifdef CONVERSION_VS
#else CONVERSION_VS
CKTDNTransfer::CKTDNTransfer()
{
	m_NetworkType	= NT_DIRECT_PLAY;
	m_pKTDNDP		= NULL;
	m_MyLatencyTime	= 0.0f;
	m_MergePacketList.reserve( 128 );

	m_ReceivePacketList.reserve( 128 );
	m_NowSendList.reserve( 128 );

	InitializeCriticalSection( &m_PacketLock );

	m_fElapsedTime	= 0.0f;
}

CKTDNTransfer::~CKTDNTransfer(void)
{
	EnterCriticalSection( &m_PacketLock );
	for( int i = 0; i < (int)m_ReceivePacketList.size(); i++ ) 
	{
		SAFE_DELETE( m_ReceivePacketList[i] );
	}
	m_ReceivePacketList.clear();
	LeaveCriticalSection( &m_PacketLock );

	DeleteCriticalSection( &m_PacketLock );
}

HRESULT CKTDNTransfer::OnFrameMove( double fTime, float fElapsedTime )
{
	EnterCriticalSection( &m_PacketLock );
	m_fElapsedTime = fElapsedTime;

	bool bHandling = false;
	for( int i = 0; i < (int)m_ReceivePacketList.size(); i++ ) 
	{
		CKTDNPacket::PacketData* pPacketData = m_ReceivePacketList[i];
		bHandling = false;

		switch( m_NetworkType )
		{
			case NT_DIRECT_PLAY:
				{
					if( pPacketData->bLocal == true )
					{					
						if( pPacketData->fLatencyTime >= m_MyLatencyTime )
						{
							bHandling = true;
						}
						pPacketData->fLatencyTime += fElapsedTime;
					}
					else
					{
						bHandling = true;
					}
				}
				break;

			case NT_TCP_RELAY:
				{
					bHandling = true;
				}
				break;
		}

		if( bHandling == true )
		{
			PacketHandler( pPacketData );
			m_ReceivePacketList.erase( m_ReceivePacketList.begin()+i );
			i--;
		}
	}
	
	FlushPacket();
	
	LeaveCriticalSection( &m_PacketLock );

	return S_OK;
}

void CKTDNTransfer::Receive( int myID, int senderID, int size, CKTDNPacket::SPT_BASIC* pData )
{
	EnterCriticalSection( &m_PacketLock );
	//패킷 데이타를 만든다
	CKTDNPacket::PacketData* pPacketData = new CKTDNPacket::PacketData();
	pPacketData->SetData( myID, senderID, size, pData );

	//패킷을 큐에 넣는다
	switch( pPacketData->packetType )
	{
		case CKTDNPacket::PT_MERGE:
			{
				//스플릿 패킷
				PacketSplit( pPacketData );
			}
			break;

		default:
			{
				m_ReceivePacketList.push_back( pPacketData );
			}
			break;
	}
	LeaveCriticalSection( &m_PacketLock );
}

void CKTDNTransfer::SetNetwork( CKTDNDP* pKTDNDP )
{
	m_pKTDNDP		= pKTDNDP;
	m_NetworkType	= NT_DIRECT_PLAY;
}

void CKTDNTransfer::SetNetwork( NETWORK_TYPE networkType )
{
	m_NetworkType	= networkType;
}

void CKTDNTransfer::Send( const int userID, const int size, CKTDNPacket::SPT_BASIC* pPacket )
{
	switch( m_NetworkType )
	{
		case NT_DIRECT_PLAY:
			m_pKTDNDP->Send( userID, pPacket, size );
			break;

		case NT_TCP_RELAY:
			//g_pKTDXApp->SendGameMessage( CKTDXApp::KM_TCP_RELAY, (WPARAM)userID, (LPARAM)pPacket, true );
			break;
	}
}

void CKTDNTransfer::BroadCast( const int size, CKTDNPacket::SPT_BASIC* pPacket )
{
	switch( m_NetworkType )
	{
		case NT_DIRECT_PLAY:
			m_pKTDNDP->BroadCast( pPacket, size );
			break;

		case NT_TCP_RELAY:
			g_pKTDXApp->SendGameMessage( CKTDXApp::KM_TCP_RELAY, (WPARAM)size, (LPARAM)pPacket, true );
			break;
	}
}

void CKTDNTransfer::PushSend( const int userID, const int size, CKTDNPacket::SPT_BASIC* pPacket )
{
	MergePacketData* pMergePacketData	= new MergePacketData();
	pMergePacketData->broadCast			= false;
	pMergePacketData->destUserID		= userID;
	pMergePacketData->SetData( pPacket, size );

	m_MergePacketList.push_back( pMergePacketData );
}

void CKTDNTransfer::PushBroadCast( const int size, CKTDNPacket::SPT_BASIC* pPacket )
{
	MergePacketData* pMergePacketData	= new MergePacketData();
	pMergePacketData->broadCast			= true;
	pMergePacketData->destUserID		= -1;
	pMergePacketData->SetData( pPacket, size );

	m_MergePacketList.push_back( pMergePacketData );
}

void CKTDNTransfer::PacketHandler( CKTDNPacket::PacketData* pPacketData )
{
	g_pKTDXApp->SendGameMessage( CKTDXApp::KM_RECEIVE_FROM_P2P, pPacketData->packetType , (LPARAM)pPacketData, false );
}

void CKTDNTransfer::FlushPacket()
{
	PacketMergeSend( true, -1 );

	while( (int)m_MergePacketList.size() != 0 )
	{
		MergePacketData* pMergePacketData = m_MergePacketList[0];
		PacketMergeSend( false, pMergePacketData->destUserID );
	}
}

void CKTDNTransfer::PacketSplit( CKTDNPacket::PacketData* pPacketData )
{
	CKTDNPacket::SPT_MERGE* pMergePacket = (CKTDNPacket::SPT_MERGE*)pPacketData->pPacket;
	BYTE* pIndex	= (BYTE*)pMergePacket;
	pIndex			+= sizeof(CKTDNPacket::SPT_MERGE);

	for( int i = 0; i < pMergePacket->packetCount ; i++ )
	{
		//사이즈 구하기
		int size	= (int)(*pIndex);
		pIndex		+= sizeof(int);

		CKTDNPacket::PacketData* pPacketData = new CKTDNPacket::PacketData();
		pPacketData->SetData( pPacketData->bLocal, pPacketData->senderID, size, (CKTDNPacket::SPT_BASIC*)pIndex );
		m_ReceivePacketList.push_back( pPacketData );
		pIndex		+= size;
	}

	SAFE_DELETE( pPacketData );
}

void CKTDNTransfer::PacketMergeSend( const bool bBroadCast, const int destUserID )
{
	if( (int)m_MergePacketList.size() == 0 )
		return;

	for( int i = 0; i < (int)m_MergePacketList.size(); i++ )
	{
		MergePacketData* pMergePacketData = m_MergePacketList[i];

		if( bBroadCast == true && pMergePacketData->broadCast == false )
		{
			continue;
		}
		else if( bBroadCast == false && pMergePacketData->broadCast == true )
		{
			continue;
		}

		m_NowSendList.push_back( pMergePacketData );
		m_MergePacketList.erase( m_MergePacketList.begin() + i );
		i--;
	}
	if( m_NowSendList.size() == 0 )
		return;

	int fullSize = 0;
	for( int i = 0; i < (int)m_NowSendList.size(); i++ )
	{
		MergePacketData* pMergePacketData = m_NowSendList[i];
		fullSize += pMergePacketData->size;
		fullSize += sizeof(int);
	}

	CKTDNPacket::SPT_MERGE mergePacket;
	mergePacket.packetCount = (int)m_NowSendList.size();
	fullSize += sizeof(CKTDNPacket::SPT_MERGE);

	BYTE* pBuffer = new BYTE[fullSize];
	BYTE* pIndex = pBuffer;

	memcpy( pIndex, &mergePacket, sizeof(CKTDNPacket::SPT_MERGE) );
	pIndex += sizeof(CKTDNPacket::SPT_MERGE);

	for( int i = 0; i < (int)m_NowSendList.size(); i++ )
	{
		MergePacketData* pMergePacketData = m_NowSendList[i];
		memcpy( pIndex, &pMergePacketData->size, sizeof(int) );
		pIndex += sizeof(int);
		memcpy( pIndex, pMergePacketData->pData, pMergePacketData->size );
		pIndex += pMergePacketData->size;
	}

	if( bBroadCast == true )
	{
		BroadCast( fullSize, (CKTDNPacket::SPT_BASIC*)pBuffer );
	}
	else
	{
		Send( destUserID, fullSize, (CKTDNPacket::SPT_BASIC*)pBuffer );
	}

	SAFE_DELETE_ARRAY( pBuffer );
	for( int i = 0; i < (int)m_NowSendList.size(); i++ )
	{
		MergePacketData* pMergePacketData = m_NowSendList[i];
		SAFE_DELETE( pMergePacketData );
	}
	m_NowSendList.erase( m_NowSendList.begin(), m_NowSendList.end() );
}

#endif CONVERSION_VS