#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "GiantCommonPacket.h"

#include <WinSock2.h>

#include "GiantCommonPacket.h"

#include "Socket/NetCommon.h"
#include "NetLayer.h"
#include "KncSend.h"
#include "BaseServer.h"

class KGiantBaseManager : public KThreadManager
{
	DeclDump;
	//DeclToStringW;

public:
	struct KGiantServerConnectInfo
	{
		std::string         m_strIP;
		unsigned short      m_usPort;

		KGiantServerConnectInfo()
		: m_usPort(0)
		{
		}	
	};

	struct KGiantServerInfo
	{
		unsigned short		m_usZone;
		unsigned short		m_usGame;
		std::wstring		m_strZoneName;
		unsigned char		m_byteNetType;
	};

protected:
	KGiantBaseManager()	
	{
		m_sock = INVALID_SOCKET;

		m_dwLastHeartBeatTick = ::GetTickCount();
		m_dwLastConnectionCheckTick = ::GetTickCount();
		m_iRecvCP = 0;

		m_dwHeartBeatInterval = 30 * 1000;				// 기본 30초
		m_dwConnectionCheckInterval	= 10 * 1000;		// 기본 10초
	}

public:
	virtual ~KGiantBaseManager()	{}

	static void RegToLuaBase();
	virtual void RegToLua() = 0;

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual void BeginThread();
	virtual void EndThread();
    virtual void Tick();

	void SetAddress(const char* szServerIP, unsigned short usPort);
	void SetHeartBitInterval(int time);
	void SetConnectionCheckInterval(int time);

	const KGiantServerInfo& GetServerInfo() const		{ return m_kGiantServerInfo; }



	typedef boost::shared_ptr<KGiantCommonPacket> TPacketPtr;
    void QueueingSendPacket( const TPacketPtr& spPacket );


protected:
	void Send();
	void Recv();
	
	void ClearSendQueue();

	bool Connect();
	bool IsConnected();
	void KeepConnection();
	void CheckConnection();

    bool GetSendPacket( TPacketPtr& spPacket );
    void DumpBuffer( BYTE* buffer, bool bError );

	virtual void MakeEventFromReceivedPacket() = 0;
	virtual void Handle_OnCommonPacket(KGiantCommonPacket& kReceivedPacket) = 0;	

protected:
	KGiantServerConnectInfo		m_kGiantConnectInfo;
	KGiantServerInfo			m_kGiantServerInfo;

	DWORD						m_dwLastHeartBeatTick;
	DWORD						m_dwLastConnectionCheckTick;
	DWORD						m_dwHeartBeatInterval;
	DWORD						m_dwConnectionCheckInterval;

	SOCKET						m_sock;

	boost::shared_ptr< KTThread< KGiantBaseManager > >	m_spThreadRecv;
    boost::shared_ptr< KTThread< KGiantBaseManager > >	m_spThreadSend;

	std::queue< TPacketPtr >							m_kSendQueue;
    mutable KncCriticalSection                          m_csSendQueue;

    char                                                m_cRecvBuffer[MAX_PACKET_SIZE_OF_GIANT_AUTH];
    int                                                 m_iRecvCP;

};


#define GPCASE(command, paracommand, id)	\
	if(kReceivedPacket.GetCommand() == command && kReceivedPacket.GetParaCommand() == paracommand)	\
{ \
	K##id kPacket; \
	if( !ks.Get(kPacket) ) \
{	\
	START_LOG( cerr, L"Deserialze failed. " L#command L" / " L#paracommand L" / " L#id ); \
}	\
		else ON_##id( kPacket );	\
		return true; \
}
#define DECL_ON_GP(id)			void ON_##id( K##id& kPacket )
#define IMPL_ON_GP(id)			void CLASS_TYPE::ON_##id( K##id& kPacket_ )
