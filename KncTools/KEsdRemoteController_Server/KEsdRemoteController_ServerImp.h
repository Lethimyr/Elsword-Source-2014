#pragma once

#include <vector>
#include <map>

#include "KSimpleActor.h"
#include "KQuery.h"
#include "KSimpleAccepter.h"
#include "SecurityString.h"
#include "ServerInfo.h"

class KEsdRemoteController_ServerImp
{
	enum SERVER_INDEX
	{
		SI_CENTER_SERVER = 0,
		SI_LOGIN_SERVER,
		SI_GAME_SERVER,
		SI_CHANNEL_SERVER,
		SI_GLOBAL_SERVER,

		SI_TOTAL_NUM,
	};

	struct KProcessData
	{
		int				m_iID;		//key
		std::wstring	m_wstrFile;
		std::wstring	m_wstrPath;
	};

	struct KUserData
	{
		std::wstring	m_wstrIP;	//key
		std::wstring	m_name;
		int				m_iProcessID[SI_TOTAL_NUM];
		int				m_iProcessID_DBG[SI_TOTAL_NUM];
		bool			m_bDBConnStr;
	};

	struct KPhoenNumberInfo
	{
		bool						m_bEnable;
		std::vector< std::wstring >	m_vecPhoneNum;
	};

	struct KSVNPath
	{
		std::wstring	m_wstrSVNFile;
		std::wstring	m_wstrSVNPath;
	};

public:
	KEsdRemoteController_ServerImp();
	virtual ~KEsdRemoteController_ServerImp();
	
	bool Initialize();	
	bool GetInitialized() { return m_bInitialized; }
	void Destroy();
	void LoadConfig();

	void CheckAlreadyOnServer();
	void Update();
	void UpdateLaunching();
	void ServerStateBroadCast();
	void ServerCrashBroadCast( SERVER_INDEX eServerIndex );
	
	void DoQuerySrvWarningMsg( int iSrvIndex );
	std::string GetTimeTag();

	// 서버 켜기
	void OnEsdLaunch();
	void OnEsdLaunch_ChannelServer();
	void OnEsdLaunch_LoginServer();
	void OnEsdLaunch_CenterServer();
	void OnEsdLaunch_GlobalServer();

	void OnEsdShutdown();

	void OnEsdWindowsUpdate();
	void OnEsdSVNUpdate();
	void OnEsdHSBUpdate();
    void OnEsdVirusCheck();
	void OnEsdDBConnectionIDPW( const std::string& strID, const std::string& strPW );

	bool OnCreateProcess( std::wstring wstrCmdline, std::wstring wstrPath = L"" );

	bool AddProcessData_Lua();
	bool AddUserData_Lua();
	bool AddPhoneNum_Lua();
	bool AddHSBData_Lua();
	bool AddSVNData_Lua();
	bool SetWindowsUpdateUrl_Lua();
	bool SetVirusProgramData_Lua();

	void SetPhoneEnable( bool bEnable ) { m_sPhoneNum.m_bEnable = bEnable; }	

	// OnAccept
	static void OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote );
	void OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote );

	// OnRecv
	void OnRecv( char* pPacketBuffer, DWORD dwRecvSize );

	bool IsDBConnStr();
	bool OpenDBConnectionDlg();

protected:
	std::wstring GetServerFile( SERVER_INDEX eServerIndex, bool bIsDebug = false );
	std::wstring GetServerPath( SERVER_INDEX eServerIndex, bool bIsDebug = false );

	const KProcessData* GetProcessData( SERVER_INDEX eServerIndex, bool bIsDebug );	

	void OnEsdShutdown_Recv();

protected:
	bool 	m_bInitialized;
	bool 	m_bShutdowning;
	bool 	m_bLaunching;
	
	bool 	m_bLaunchAutoRestart;
	bool 	m_bLoginAutoRestart;
	bool 	m_bChannelAutoRestart;
	bool 	m_bGlobalAutoRestart;

	HWND	m_hServer[SI_TOTAL_NUM];
	HWND	m_hBeforeServerState[SI_TOTAL_NUM];

	std::wstring						m_wstrHSBFile;
	std::wstring						m_wstrHSBPath;
	std::vector< KSVNPath >				m_vecSVNPath;
	std::wstring						m_wstrWindowsUpdateUrl;
	std::wstring						m_wstrVirusFile;
	std::wstring						m_wstrVirusPath;

	std::map< int, KProcessData >		m_mapProcessData;
	std::map< std::wstring, KUserData >	m_mapUserData;
	//{{ 2009. 6. 17  최육사	DBConnStr
	KSecurityString						m_kDBConnStr;
	//}}
	KPhoenNumberInfo					m_sPhoneNum;

	KQuery								m_kQuery;

	// accepter
	KSimpleAccepter						m_kAccepter;

	//{{ 2009. 6. 22  최육사	서버정보통합
	KServerInfo							m_kServerInfo[SI_TOTAL_NUM];
	//}}
};


