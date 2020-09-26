#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_TH

class KAsiaSoftAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KAsiaSoftAuthManager );

public:
	enum ASIASOFT_PORT
	{
		ASP_AUTH,
		ASP_IPBONUS,

		ASP_NUM,
	};

	KAsiaSoftAuthManager();
	virtual ~KAsiaSoftAuthManager();

	struct RequestInfo
	{
		RequestInfo() 
		{
			m_bIsLogout = false;
		}

		RequestInfo(UidType server_uid, UidType user_uid, std::wstring user_id, std::wstring domain, std::wstring ip = L"", std::wstring akey = L"")
			: ServerUID(server_uid)
			, UserUID(user_uid)
			, wstrUserID(user_id)
			, wstrDomain(domain)
			, wstrIP(ip)
			, wstrAkey(akey)

		{
			m_bIsLogout = false;
		}

		UidType			ServerUID;
		UidType			UserUID;
		std::wstring	wstrUserID;
		std::wstring	wstrDomain;

		std::wstring	wstrIP;

		std::wstring	wstrAkey;

		bool			m_bIsLogout;
	};

	void RegToLua();

	void InitAsiaSoftAuthValue( const char* szServiceCode, const char* szServiceRegion );
	void InitAsiaSoftAuthAddress( const char* szGconsoleIP,	unsigned short  usPort );
	void InitAsiaSoftAKeyAddress( const char* szGconsoleIP, unsigned short  usPort );
	void InitAsiaSoftIPBonusAddress( const char* szGconsoleIP,	unsigned short  usPort );

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();

	void RegisterRequestInfo( IN int iPortEnum, std::wstring& wstrIndex, RequestInfo iUserUID );
	RequestInfo UnregisterRequestInfo( IN int iPortEnum, std::wstring wstrSockID );
	void ClearRequestInfo( IN int iPortEnum );
	//{{ 2011.3.16 최초 계정 생성 시 IP 넣기 (HK/TW)
#ifdef SERV_INSERT_ACCOUNT_IP
	void RegisterUserIDUserIP( std::string& strUserID, std::string& strUserIP );
	std::string UnregisterUserIDUserIP( std::string& strUserID );
#endif
	//}}
	void QueueingSendPacket( int iPortSelect, const boost::shared_ptr< std::string >& spPacket );

	std::string GetServiceCode()            { return m_kAsiaSoftAuthInfo.m_strServiceCode; }
	std::string GetServiceRegion()          { return m_kAsiaSoftAuthInfo.m_strServiceRegion; }

	//{{ 허상형 : [2012/6/21] //	AsiaSoft Auth
	std::string GetLoginPacket( IN KEPUBLISHER_AUTHENTICATION_REQ kPacket_ );
	std::string GetIPBonusPacket( IN KEAS_GET_IPBONUS_REQ kPacket_ );
	std::string GetAuthAkeyPacket( IN KEAS_AUTH_AKEY_REQ kPacket_ );
	bool		ExtractLoginPacket( IN std::string strPacket, OUT KEAS_AUTH_LOGIN_ACK& kPacketAck );
	bool		ExtractIPBonusPacket( IN std::string strPacket, OUT KEAS_GET_IPBONUS_ACK& kPacketAck );
	bool		ExtractAkeyPacket( IN std::string strPacket, OUT KEAS_AUTH_AKEY_ACK& kPacketAck );

	std::wstring	MakeMasterID( IN std::wstring wstrDomain, IN std::wstring wstrGameID )
	{
		return wstrDomain + L"." + wstrGameID;
	}
	void			GetIDDomain( IN std::wstring wstrMasterID, OUT std::wstring &wstrUserID, OUT std::wstring &wstrDomain );
	//}} 허상형 : [2012/6/21] //	AsiaSoft Auth

	//{{ 허상형 : [2012/10/9] //
	std::string		GetAKeyIP()		{	return  m_strAKeyIP; }
	unsigned short	GetAKeyPort()	{	return m_usAKeyPort; }
	//}} 허상형 : [2012/10/9] //
	

protected:
	void RecvFromAuth();
	void SendToAuth();
	void RecvFromAKey();
	void SendToAKey();
	void RecvFromIPBonus();
	void SendToIPBonus();

	void SendTo( int iPortSelect );
	void RecvFrom( int iPortSelect );

	bool Connect( int iPortSelect );
	void CheckConnection();
	void KeepConnection();

	bool GetSendPacket( int iPortSelect, boost::shared_ptr< std::string >& spPacket );

	void MakeEventFromReceived(int iPortEnum, std::string& strPacket);
	void DumpRecvBuffer( bool bError );

	inline void MakeLowerCase(std::wstring &str) 
	{
		for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
			*i = (WCHAR)tolower(*i);
	}

	inline void MakeUpperCase(std::wstring &str) 
	{
		for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
			*i = (WCHAR)towupper(*i);
	}

public:
	bool IsConnected( int iPortSelect );

protected:
	struct KAsiaSoftAuthInfo
	{
		std::string         m_strServiceCode;
		std::string         m_strServiceRegion;

		std::string         m_strIP[ASP_NUM];
		unsigned short      m_usAsiaSoft_Port[ASP_NUM];

		KAsiaSoftAuthInfo()
		{
			for( int i = 0; i < ASP_NUM; i++ )
			{
				m_strIP[i] = "";
				m_usAsiaSoft_Port[i] = 0;
			}
		}

	} m_kAsiaSoftAuthInfo;

	DWORD				m_dwLastConnectionCheckTick[ASP_NUM];
	static DWORD		ms_dwConnectionCheckGap[ASP_NUM];

	DWORD				m_dwLastKeepConnectionTick[ASP_NUM];
	static DWORD		ms_dwKeepConnectionGap[ASP_NUM];

	SOCKET				m_sock[ASP_NUM];

	boost::shared_ptr< KTThread< KAsiaSoftAuthManager > >	m_spThreadRecv[ASP_NUM];
	boost::shared_ptr< KTThread< KAsiaSoftAuthManager > >	m_spThreadSend[ASP_NUM];

	std::queue< boost::shared_ptr< std::string > >      m_kSendQueue[ASP_NUM];
	mutable KncCriticalSection                          m_csSendQueue[ASP_NUM];

	char                                                m_cRecvBuffer[ASP_NUM][MAX_PACKET_SIZE_OF_GASH_AUTH];
	int                                                 m_iRecvCP[ASP_NUM];

	mutable KncCriticalSection                          m_csUserIDUserUID;
	std::map< std::wstring, RequestInfo >               m_mapRequestInfo[ASP_NUM];	//	sockID, requestID
	//{{ 2011.3.16 최초 계정 생성 시 IP 넣기 (HK/TW)
#ifdef SERV_INSERT_ACCOUNT_IP
	std::map< std::string, std::string >                m_mapUserIDUserIP;
#endif
	//}}

	//{{ 허상형 : [2012/10/9] //	AKey는 별도의 매니저에서 관리한다.
	std::string         m_strAKeyIP;
	unsigned short      m_usAKeyPort;
	//}} 허상형 : [2012/10/9] //	AKey는 별도의 매니저에서 관리한다.
};

DefSingletonInline( KAsiaSoftAuthManager );

#endif SERV_COUNTRY_TH