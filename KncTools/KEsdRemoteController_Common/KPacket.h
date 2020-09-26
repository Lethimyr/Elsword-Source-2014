#pragma once

#include <WinSock2.h>
#include "KSerializer.h"

class KPacket : public KSerializer
{

	KDeclareMemPool(KPacket);

public:
	typedef unsigned short PACKETID;
	typedef unsigned short PACKETSIZE;

public:
	KPacket();
	KPacket( PACKETID kPacket );
	KPacket( KPacket& kPacket );
	KPacket( char* pcBuffer, unsigned int uiSize );
	virtual ~KPacket(void);

	void InitPool() 
	{
		Clear();
	}

	PACKETID GetID()
	{
		return (*m_pkPacketID);
	}

	void SetID(PACKETID kID)
	{
		(*m_pkPacketID) = kID;
	}

	PACKETSIZE GetSize()
	{
		return (*m_pkPacketSize);
	}

	virtual void WriteBytes( const void* pvBuf, unsigned int uiSize )
	{
		KSerializer::WriteBytes( pvBuf, uiSize );
		(*m_pkPacketSize) += uiSize;
	}

	virtual void ExtendBuffer(unsigned int uiSize)
	{
		KSerializer::ExtendBuffer(uiSize);

		m_pkPacketID = (PACKETID*)GetPosition( GetDataCheckSize() );
		m_pkPacketSize = (PACKETSIZE*)GetPosition( GetDataCheckSize() * 2 + sizeof(PACKETID) );
	}

	KPacket& operator = (KPacket& kValue)
	{
		SetBuffer(kValue.GetPosition(0), kValue.GetSize());
		return *this;
	}

	void Clear();

	// UDP 패킷용
	void SetAddress( SOCKADDR_IN& sSockAddr );
	SOCKADDR_IN& GetAddress();

protected:
    PACKETID* m_pkPacketID;
    PACKETSIZE* m_pkPacketSize;

	// UDP Packet 의 경우, Recv Address
	SOCKADDR_IN m_sSockAddr;

};
