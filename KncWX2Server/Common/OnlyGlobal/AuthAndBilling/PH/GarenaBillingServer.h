#pragma once 

#include <WinSock2.h>
#include "ToString.h"
#include "KNCSingleton.h"
#include "Thread/Thread.h"
#include "RTTI.h"
#include "Socket/Accepter.h"
#include "GarenaBillingUserManager.h"
#include <queue>
#include "Event.h"
#include "ThreadManager.h"
#include <boost/foreach.hpp>



#ifdef SERV_COUNTRY_PH

#include "LoginSimLayer.h"
#include "UserList.h"


class KGarenaBillingThreadManager : public KThreadManager
{
	DeclPfID;

public:
	KGarenaBillingThreadManager() {}
	virtual ~KGarenaBillingThreadManager() {}

protected:
	virtual KThread* CreateThread();
};


class KGarenaBillingServer
{
	DeclareSingleton( KGarenaBillingServer );
	DeclDump;
	DeclToStringW;

protected:
	KGarenaBillingServer(void);
public:
	virtual ~KGarenaBillingServer(void);	

	void	RegToLua();
	bool	Init();
	void	ShutDown();
	
	void	SetWorkerThreadCount( int iWorkerThreadCount ) { m_iWorkerThreadCount = iWorkerThreadCount; }

	void	QueueingEvent( const KEventPtr& spEvent );

	// TCP Relay accepter thread ¸¸ Á¢±Ù.
	static	void	OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote );

	KGarenaBillingUserManager&	GetGarenaBillingUserManager() { return m_kGarenaBillingUserManager; }

	void	Tick();

	void SetmapChangeGameCurrencyFailed( KEJSON_GN_CHANGE_GAME_CURRENCY_ACK kPacket );

	void GetmapChangeGameCurrencyFailed( IN KEJSON_GN_TRANSACTION_REQ kPacket, OUT KEJSON_GN_TRANSACTION_ACK& kPacketAck );

	bool GetCheckUserInfoByPublisherUID( IN unsigned int uiPublisherUID, OUT KUserList::KGSUserInfo& kInfo );

protected:
	void	OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote );	

protected:

	struct KMaxTimeInfo 
	{
		mutable KncCriticalSection  m_cs;
		std::wstring                m_strIP;
		DWORD                       m_dwTerm;
		CTime                       m_cTime;
	} m_kMaxTimeInfo;

	KAccepter						m_kAccepter;
	KGarenaBillingUserManager			m_kGarenaBillingUserManager;
	KGarenaBillingThreadManager		m_kThreadManager;

	int								m_iWorkerThreadCount;


	mutable KncCriticalSection  m_csGameCurrencyFailed;

	mutable KncCriticalSection  m_csCheckUserInfoByPublisherUID;


	std::map<std::string, KEJSON_GN_CHANGE_GAME_CURRENCY_ACK>		m_mapChangeGameCurrencyFailed;
};

DefSingletonInline( KGarenaBillingServer );


#endif SERV_COUNTRY_PH
