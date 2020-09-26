#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_TWHK

class KGASHAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KGASHAuthManager );

public:
    enum GASH_PORT
    {
        GP_AUTHENTICATION,
        GP_AUTHORIZATION,
        GP_ACCOUNTING,

        GP_NUM,
    };

	KGASHAuthManager();
	virtual ~KGASHAuthManager();

	void RegToLua();

    void InitGASHAuthValue( const char* szServiceCode, const char* szServiceRegion );

	void InitGASHAuthAddress(
		const char*     szGconsoleIP,
		unsigned short  usAuthenticationPort,
        unsigned short  usAuthorizationPort,
        unsigned short  usAccountingPort );

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
    virtual void Tick();

    void RegisterUserIDUserUID( std::string& strUserID, UidType iUserUID );
    UidType UnregisterUserIDUserUID( std::string& strUserID );
	void RegisterUserIDUserIP( std::string& strUserID, std::string& strUserIP );
	std::string UnregisterUserIDUserIP( std::string& strUserID );

    void QueueingSendPacket( int iPortSelect, const boost::shared_ptr< std::string >& spPacket );

    std::string GetServiceCode()            { return m_kGASHAuthInfo.m_strServiceCode; }
    std::string GetServiceRegion()          { return m_kGASHAuthInfo.m_strServiceRegion; }

protected:
	void RecvFromAuthentication();
	void SendToAuthentication();
	void RecvFromAuthorization();
	void SendToAuthorization();
	void RecvFromAccounting();
	void SendToAccounting();

	void SendTo(int iPortEnum);
	void RecvFrom(int iPortEnum);

	bool Connect( int iPortSelect );
	bool IsConnected( int iPortSelect );
	void CheckConnection();

    bool GetSendPacket( int iPortSelect, boost::shared_ptr< std::string >& spPacket );
	
	void MakeEventFromReceived(int iPortEnum, std::string& strPacket);
    void DumpRecvBuffer( bool bError );

public:

protected:
	struct KGASHAuthInfo
	{
        std::string         m_strServiceCode;
        std::string         m_strServiceRegion;

		std::string         m_strIP;
		unsigned short      m_usGash_Port[GP_NUM];

        KGASHAuthInfo()
        {
			for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
			{
				m_usGash_Port[iPortNum] = 0;
			}
        }

	} m_kGASHAuthInfo;

	DWORD				m_dwLastConnectionCheckTick[GP_NUM];
	static DWORD		ms_dwConnectionCheckGap[GP_NUM];

	SOCKET				m_sock[GP_NUM];

    boost::shared_ptr< KTThread< KGASHAuthManager > >	m_spThreadRecv[GP_NUM];
    boost::shared_ptr< KTThread< KGASHAuthManager > >	m_spThreadSend[GP_NUM];

    std::queue< boost::shared_ptr< std::string > >      m_kSendQueue[GP_NUM];
    mutable KncCriticalSection                          m_csSendQueue[GP_NUM];

    char                                                m_cRecvBuffer[GP_NUM][MAX_PACKET_SIZE_OF_GASH_AUTH];
    int                                                 m_iRecvCP[GP_NUM];

    mutable KncCriticalSection                          m_csUserIDUserUID;
    std::map< std::string, UidType >                    m_mapUserIDUserUID;
	std::map< std::string, std::string >                m_mapUserIDUserIP;
};

DefSingletonInline( KGASHAuthManager );

#endif // SERV_COUNTRY_TWHK
