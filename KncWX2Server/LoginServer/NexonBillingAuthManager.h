#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include "NexonBillingAuthPacket.h"

class KNexonBillingAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KNexonBillingAuthManager );

public:
	KNexonBillingAuthManager();
	virtual ~KNexonBillingAuthManager();

	void RegToLua();

	void InitNexonBillingAuthInfo(
		const char*     szNexonAuthIP,
		unsigned short  usPort,
		int				iDomain,
		const char*		szDomainName );

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
    virtual void Tick();

    void QueueingSendPacket( const KNexonBillingAuthPacketPtr& spPacket );

protected:
	void Recv();
	void Send();

	bool Connect();
	bool IsConnected();
	void KeepConnection();
	void CheckConnection();

    bool GetSendPacket( KNexonBillingAuthPacketPtr& spPacket );
    void MakeEventFromReceivedPacket();
    void DumpBuffer( BYTE* buffer, bool bError );

public:

protected:
	struct KNexonBillingAuthInfo
	{
		std::string         m_strIP;
		unsigned short      m_usPort;
		int                 m_iDomain;
		std::wstring		m_wstrDomainName;
	} m_kNexonBillingAuthInfo;
	DWORD				m_dwLastHeartBeatTick;
	DWORD				m_dwLastConnectionCheckTick;
	static DWORD		ms_dwHeartBeatGap;
	static DWORD		ms_dwConnectionCheckGap;

	SOCKET				m_sock;

	boost::shared_ptr< KTThread< KNexonBillingAuthManager > >	m_spThreadRecv;
	boost::shared_ptr< KTThread< KNexonBillingAuthManager > >	m_spThreadSend;

	std::queue< KNexonBillingAuthPacketPtr >					m_kSendQueue;
	mutable KncCriticalSection									m_csSendQueue;

    char                                                        m_cRecvBuffer[MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH];
    int                                                         m_iRecvCP;
};

DefSingletonInline( KNexonBillingAuthManager );