#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include "NexonAuthPacket.h"


//{{ 2010. 06. 04  √÷¿∞ªÁ	≥ÿΩºPCπÊ ¿Œ¡ı º≠πˆ ∞≥∆Ì
#ifdef SERV_PCBANG_AUTH_NEW


class KNexonAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KNexonAuthManager );

public:
	KNexonAuthManager();
	virtual ~KNexonAuthManager();

	void RegToLua();

	void InitNexonAuthInfo_LUA(
		const char*     szNexonAuthIP,
		unsigned short  usPort,
		int				iDomain,
		const char*		szDomainName,
		int				iSynchronizeType,
		int				iSynchronizeCount );

	void DisconnectAuthServer_LUA();

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
    virtual void Tick();

    void QueueingSendPacket( const KNexonAuthPacketPtr& spPacket );

protected:
	void Recv();
	void Send();

	bool Connect();
	bool IsConnected();
	void KeepConnection();
	void CheckConnection();

    bool GetSendPacket( KNexonAuthPacketPtr& spPacket );
    void MakeEventFromReceivedPacket();
    void DumpBuffer( BYTE* buffer, bool bError );

public:

protected:
	struct KNexonAuthInfo
	{
		std::string         m_strIP;
		unsigned short      m_usPort;
		int                 m_iDomain;
		std::wstring		m_wstrDomainName;
		int					m_iSynchronizeType;
		int					m_iSynchronizeCount;

		KNexonAuthInfo()
		{
			m_usPort			= 0;
			m_iDomain			= 0;			
			m_iSynchronizeType	= 0;
			m_iSynchronizeCount = 0;
		}
	} m_kNexonAuthInfo;

	DWORD				m_dwLastHeartBeatTick;
	DWORD				m_dwLastConnectionCheckTick;
	static DWORD		ms_dwHeartBeatGap;
	static DWORD		ms_dwConnectionCheckGap;

	SOCKET				m_sock;

	boost::shared_ptr< KTThread< KNexonAuthManager > >	m_spThreadRecv;
	boost::shared_ptr< KTThread< KNexonAuthManager > >	m_spThreadSend;

	std::queue< KNexonAuthPacketPtr >					m_kSendQueue;
	mutable KncCriticalSection							m_csSendQueue;

    char                                                m_cRecvBuffer[MAX_PACKET_SIZE_OF_NEXON_AUTH];
    int                                                 m_iRecvCP;
};

DefSingletonInline( KNexonAuthManager );


#endif SERV_PCBANG_AUTH_NEW
//}}