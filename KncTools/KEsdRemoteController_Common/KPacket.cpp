// Precompile Header
#include "stdafx.h"

#include "KPacket.h"

#define PACKET_RESERVE_SIZE 256

KImplementMemPool(KPacket);

KPacket::KPacket()
: KSerializer(PACKET_RESERVE_SIZE)
{
	Clear();
}

KPacket::KPacket(PACKETID kPacket)
: KSerializer(PACKET_RESERVE_SIZE)
{
	Clear();
	SetID( kPacket );
}

KPacket::KPacket( char* pcBuffer, unsigned int uiSize )
: KSerializer(PACKET_RESERVE_SIZE)
{
	SetBuffer( pcBuffer, uiSize );
}

KPacket::KPacket( KPacket& kPacket )
: KSerializer(PACKET_RESERVE_SIZE)
{
	SetBuffer( kPacket.GetPosition(0), kPacket.GetSize() );
	m_sSockAddr = kPacket.m_sSockAddr;
}

KPacket::~KPacket(void)
{
}

void KPacket::Clear()
{
	KSerializer::Clear();

    m_pkPacketID = (PACKETID*)GetPosition(GetDataCheckSize());
	m_pkPacketSize = (PACKETSIZE*)GetPosition(GetDataCheckSize() * 2 + sizeof(PACKETID));

	(*m_pkPacketSize) = m_uiCurReadPos = m_uiCurWritePos = sizeof(PACKETID) + sizeof(PACKETSIZE) + GetDataCheckSize() * 2;
}

void KPacket::SetAddress( SOCKADDR_IN& sSockAddr )
{
	m_sSockAddr = sSockAddr;
}

SOCKADDR_IN& KPacket::GetAddress()
{
	return m_sSockAddr;
}
