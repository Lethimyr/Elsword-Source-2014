#pragma once

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <atltime.h>
#include <boost/timer.hpp>
#include "CommonPacket.h"
#include "ServerPacket.h"


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_MORNITORING
//////////////////////////////////////////////////////////////////////////


class KMornitoringManager
{
	DeclareRefreshSingleton( KMornitoringManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum ERR_STAT_TYPE
	{
		EST_ONE_DAY = 0,
		EST_5_MINUTES,

		EST_MAX,
	};

	enum MORNITORING_ENUM
	{
		ME_SMS_SEND_TIME = 10,
	};

	enum ERR_TYPE
	{
		ET_DISCONNECT_GAME_CN_PROXY = 0,
		ET_DISCONNECT_GAME_LOGIN_PROXY,
		ET_DISCONNECT_GAME_BILLING,
		ET_DISCONNECT_LOGIN_AUTH,
		ET_DISCONNECT_DB_CONNECTION,
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		ET_DISCONNECT_GAME_GLOBAL_PROXY,
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
				
		ET_MAX,
	};

	struct KMornitoringData
	{
		int				m_iCount;

		KMornitoringData()
		{
			m_iCount = 0;
		}
	};

	KMornitoringManager(void);
	~KMornitoringManager(void);

	// for lua
	void SetSendSMS_LUA( bool bVal );
	void SetSendMail_LUA( bool bVal );
	void AddPhoneNum_LUA( const char* pszPhoneNum );
	void AddServerNetworkMailAddr_LUA( const char* pszMailAddr );
	//{{ 2011. 03. 30	최육사	서버 정보 수집
#ifdef SERV_ROOM_TICK_DUMP
	void AddServerDumpMailAddr_LUA( const char* pszMailAddr );
	void AddDumpServerUID_LUA( int iServerUID );
#endif SERV_ROOM_TICK_DUMP
	//}}

	// Tick function
	void Tick_GSSImLayer();
	//{{ 2011. 03. 30	최육사	서버 정보 수집
#ifdef SERV_ROOM_TICK_DUMP
	void Tick_CnSimLayer();
#endif SERV_ROOM_TICK_DUMP
	//}}	
	void Tick_LoginSImLayer();	

	// Check function
	void CheckDisconnectAndGetMail_GameServer( OUT KEMAIL_SEND_EMAIL_NOT& kPacketNot );
	bool CheckDisconnectAndGet5MinutesMessage_LoginServer( OUT std::wstring& wstrMessage );
	void CheckDisconnectAndGetOneDayMessage_LoginServer( OUT std::wstring& wstrMessage );

	bool IncreaseDisconnectCount( IN ERR_TYPE eType );
	void ReserveSMSMessage( IN const KDBE_SEND_PHONE_MSG_NOT& kNot );

	//{{ 2011. 03. 30	최육사	서버 정보 수집
#ifdef SERV_ROOM_TICK_DUMP
	void CheckServerInfoToMail();
#endif SERV_ROOM_TICK_DUMP
	//}}

	//{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
	void GetCheckExistModule( std::map< std::wstring, bool >& mapModule );
	void AddCheckModuleList_LUA( const char*  pszModule );
	void CheckModuleList_X2Module_LUA( bool bRet );
	bool CheckModuleList_X2Module() { return m_bCheckExistModule; }
#endif SERV_CHECK_X2_EXE_MODULE
	//}}

	

protected:
	template < class T > void SendToSMSDB( unsigned short usEventID, const T& data );
	void	SendToSMSDB( unsigned short usEventID );

	inline void MakeUpperCase(std::wstring &str) 
	{
		for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
			*i = (WCHAR)towupper(*i);
	}

private:
	std::map< ERR_TYPE, int >				m_mapErrStatistics[EST_MAX];
	std::vector< std::wstring >				m_vecPhoneNumList;
	std::vector< std::wstring >				m_vecServerNetworkMailList;
	//{{ 2011. 03. 30	최육사	서버 정보 수집
#ifdef SERV_ROOM_TICK_DUMP
	std::vector< std::wstring >				m_vecServerDumpMailList;
#endif SERV_ROOM_TICK_DUMP
	//}}
	std::vector< KDBE_SEND_PHONE_MSG_NOT >	m_vecReservedSMSMessage;
	boost::timer							m_tTimer;
	CTime									m_NextSendTime;
	bool									m_bSendSMS;
	bool									m_bSendMail;
	//{{ 2011. 03. 30	최육사	서버 정보 수집
#ifdef SERV_ROOM_TICK_DUMP	
	CTime									m_tDumpSendTime;
	std::set< UidType >						m_setDumpServerUIDList;
#endif SERV_ROOM_TICK_DUMP
	//}}

	//{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
	bool									m_bCheckExistModule;
	std::map< std::wstring, bool >			m_mapExistModule;
#endif SERV_CHECK_X2_EXE_MODULE
	//}}

};

DefRefreshSingletonInline( KMornitoringManager );

template < class T >
void KMornitoringManager::SendToSMSDB( unsigned short usEventID, const T& data )
{
	if( m_bSendSMS == false )
	{
		START_LOG( cout, L"SMS문자 전송 차단!" );
		return;
	}

	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_SMS_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_MORNITORING
//////////////////////////////////////////////////////////////////////////


