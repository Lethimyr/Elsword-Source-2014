#include "stdafx.h"

#include "KEsdRemoteController_Server.h"
#include "KEsdRemoteController_ServerImp.h"
#include "KPacket.h"
#include "KPacketID.h"
#include "KFolderTraverse.h"
#include "KNetUtil.h"
#include "SimpleIOCP.h"
#include "DBConnectionDlg.h"

#include <map>
#include <fstream>
using namespace std;


KEsdRemoteController_ServerImp::KEsdRemoteController_ServerImp()
{
	m_bInitialized			= false;
	m_bLaunching			= false;
	m_bShutdowning			= false;

	m_bLaunchAutoRestart	= false;	
	m_bLoginAutoRestart		= false;
	m_bChannelAutoRestart	= false;
	m_bGlobalAutoRestart	= false;

	m_sPhoneNum.m_bEnable	= false;

	std::string strDSNName = KGetCurFolder();
	strDSNName += "\\";
	strDSNName += "SMS.dsn";
	m_kQuery.Connect( KQuery::eConnectType_SQL, (char*) strDSNName.c_str(), NULL, NULL ); 
}

KEsdRemoteController_ServerImp::~KEsdRemoteController_ServerImp()
{
	Destroy();
}

bool KEsdRemoteController_ServerImp::Initialize()
{
	LoadConfig();

	// init accepter
	m_kAccepter.Init( 6005, KEsdRemoteController_ServerImp::OnAccept );

	// init iocp
	SiKSimpleIOCP()->Init( 2 );
	SiKSimpleIOCP()->BeginThread();
	
	m_bInitialized = m_kAccepter.Begin();

	// init Server info
	m_kServerInfo[SI_LOGIN_SERVER].Init( "LoginServer" );
	m_kServerInfo[SI_CENTER_SERVER].Init( "CenterServer" );
	m_kServerInfo[SI_GAME_SERVER].Init( "GameServer" );
	m_kServerInfo[SI_CHANNEL_SERVER].Init( "ChannelServer" );
	m_kServerInfo[SI_GLOBAL_SERVER].Init( "GlobalServer" );

	return m_bInitialized;
}

void KEsdRemoteController_ServerImp::Destroy()
{
    m_kAccepter.End();

	SiKSimpleIOCP()->EndThread();
	Sleep( 100 );
	KSimpleIOCP::ReleaseInstance();

	m_bInitialized = false;
}

void KEsdRemoteController_ServerImp::CheckAlreadyOnServer()
{
	m_hServer[SI_LOGIN_SERVER]	 = FindWindow( NULL, L"LoginServer_Ready" );
	m_hServer[SI_CENTER_SERVER]	 = FindWindow( NULL, L"CenterServer_Ready" );
	m_hServer[SI_GAME_SERVER]	 = FindWindow( NULL, L"GameServer_Ready" );
	m_hServer[SI_CHANNEL_SERVER] = FindWindow( NULL, L"ChannelServer_Ready" );
	m_hServer[SI_GLOBAL_SERVER]	 = FindWindow( NULL, L"GlobalServer_Ready" );

	// 로그인 서버
	if( m_hServer[SI_LOGIN_SERVER] != NULL )
	{
		OnEsdLaunch_LoginServer();
	}

	// 센터서버&게임서버
	if( m_hServer[SI_CENTER_SERVER] != NULL )
	{
		if( m_hServer[SI_GAME_SERVER] != NULL )
		{
			OnEsdLaunch();
		}
		else
		{
			OnEsdLaunch_CenterServer();
		}
	}

	// 채널서버
	if( m_hServer[SI_CHANNEL_SERVER] != NULL )
	{
        OnEsdLaunch_ChannelServer();
	}

	// 글로벌서버
	if( m_hServer[SI_GLOBAL_SERVER] != NULL )
	{
		OnEsdLaunch_GlobalServer();
	}
}

void KEsdRemoteController_ServerImp::Update()
{
	// Pre Process Check Server
	bool bLastLgReady = false;
	bool bLastCnReady = false;
	bool bLastGsReady = false;
	bool bLastChReady = false;
	bool bLastGbReady = false;

	if( m_hServer[SI_LOGIN_SERVER] != NULL )
	{
		bLastLgReady = true;
	}
	if( m_hServer[SI_CENTER_SERVER] != NULL )
	{
		bLastCnReady = true;
	}
	if( m_hServer[SI_GAME_SERVER] != NULL )
	{
		bLastGsReady = true;
	}
	if( m_hServer[SI_CHANNEL_SERVER] != NULL )
	{
		bLastChReady = true;
	}
	if( m_hServer[SI_GLOBAL_SERVER] != NULL )
	{
		bLastGbReady = true;
	}

	// Check Server Status
	{
		m_hServer[SI_LOGIN_SERVER]	 = FindWindow( NULL, L"LoginServer_Ready" );
		m_hServer[SI_CENTER_SERVER]	 = FindWindow( NULL, L"CenterServer_Ready" );
		m_hServer[SI_GAME_SERVER]	 = FindWindow( NULL, L"GameServer_Ready" );
		m_hServer[SI_CHANNEL_SERVER] = FindWindow( NULL, L"ChannelServer_Ready" );
		m_hServer[SI_GLOBAL_SERVER]	 = FindWindow( NULL, L"GlobalServer_Ready" );

		if( m_hBeforeServerState[SI_LOGIN_SERVER] != m_hServer[SI_LOGIN_SERVER]  ||
			m_hBeforeServerState[SI_CENTER_SERVER] != m_hServer[SI_CENTER_SERVER]  ||
			m_hBeforeServerState[SI_GAME_SERVER] != m_hServer[SI_GAME_SERVER]  ||
			m_hBeforeServerState[SI_CHANNEL_SERVER] != m_hServer[SI_CHANNEL_SERVER]  ||
			m_hBeforeServerState[SI_GLOBAL_SERVER] != m_hServer[SI_GLOBAL_SERVER] )
		{
			ServerStateBroadCast();
		}

		m_hBeforeServerState[SI_LOGIN_SERVER] = m_hServer[SI_LOGIN_SERVER];
		m_hBeforeServerState[SI_CENTER_SERVER] = m_hServer[SI_CENTER_SERVER];
		m_hBeforeServerState[SI_GAME_SERVER] = m_hServer[SI_GAME_SERVER];
		m_hBeforeServerState[SI_CHANNEL_SERVER] = m_hServer[SI_CHANNEL_SERVER];
		m_hBeforeServerState[SI_GLOBAL_SERVER] = m_hServer[SI_GLOBAL_SERVER];
	}

	// 종료 중일 경우 처리
	if ( m_bShutdowning == true )
	{
		if ( m_hServer[SI_LOGIN_SERVER] == NULL  &&
			 m_hServer[SI_CENTER_SERVER] == NULL  &&
			 m_hServer[SI_GAME_SERVER] == NULL  &&
			 m_hServer[SI_CHANNEL_SERVER] == NULL  &&
			 m_hServer[SI_GLOBAL_SERVER] == NULL )
		{
			m_bShutdowning = false;
		}

		return;
	}

	// Post Process Server Status
	// 도중에 서버가 종료되었을 경우 처리
	if ( bLastLgReady == true && m_hServer[SI_LOGIN_SERVER] == NULL )
	{
		DoQuerySrvWarningMsg( SI_LOGIN_SERVER );
		ServerCrashBroadCast( SI_LOGIN_SERVER );
	}
	if ( bLastCnReady == true && m_hServer[SI_CENTER_SERVER] == NULL )
	{
		DoQuerySrvWarningMsg( SI_CENTER_SERVER );
		ServerCrashBroadCast( SI_CENTER_SERVER );
	}
	if ( bLastGsReady == true && m_hServer[SI_GAME_SERVER] == NULL )
	{
		DoQuerySrvWarningMsg( SI_GAME_SERVER );
		ServerCrashBroadCast( SI_GAME_SERVER );
	}
	if ( bLastChReady == true && m_hServer[SI_CHANNEL_SERVER] == NULL )
	{
		DoQuerySrvWarningMsg( SI_CHANNEL_SERVER );
		ServerCrashBroadCast( SI_CHANNEL_SERVER );
	}
	if ( bLastGbReady == true && m_hServer[SI_GLOBAL_SERVER] == NULL )
	{
		DoQuerySrvWarningMsg( SI_GLOBAL_SERVER );
		ServerCrashBroadCast( SI_GLOBAL_SERVER );
	}

	UpdateLaunching();
}

void KEsdRemoteController_ServerImp::DoQuerySrvWarningMsg( int iSrvIndex )
{
	if( m_sPhoneNum.m_bEnable == true )
	{
		std::wstring wstrChannelName;
		std::wstring wstrIP = KncUtil::toWideString( _KNetUtil()->GetHostIP() );
		std::map< std::wstring, KUserData >::const_iterator mitUD = m_mapUserData.find( wstrIP );
		if( mitUD != m_mapUserData.end() )
		{
			wstrChannelName = mitUD->second.m_name;
		}

		for ( unsigned int iP = 0; iP < m_sPhoneNum.m_vecPhoneNum.size(); ++iP )
		{
			std::wstring strPhone = m_sPhoneNum.m_vecPhoneNum[iP];
			std::wstring strMsg = L"[ELSWORD DOWN]\n";

			// 채널 이름
			strMsg += wstrChannelName;
			strMsg += L"\n";

			switch( iSrvIndex )
			{
			case SI_CENTER_SERVER:
				strMsg += L"Center";
				break;
			case SI_LOGIN_SERVER:
				strMsg += L"Login";
				break;
			case SI_GAME_SERVER:
				strMsg += L"Game";
				break;
			case SI_CHANNEL_SERVER:
				strMsg += L"Channel";
				break;
			case SI_GLOBAL_SERVER:
				strMsg += L"Global";
				break;
			default:
				break;
			}
			strMsg += L"_Svr_Info\n";
			strMsg += KncUtil::toWideString( GetTimeTag() );
			strMsg += L"\n";
			strMsg += KncUtil::toWideString( _KNetUtil()->GetHostIP() );

			std::wostringstream os;
			os << L"insert into em_tran ( tran_phone, tran_callback, tran_status, tran_date, tran_msg) ";
			os << L"values ( \'" << strPhone << L"\', \'" << strPhone << L"\', \'1\', getdate(), \'" << strMsg.c_str() << L"\' )";
			m_kQuery.Exec( os.str().c_str() );
		}
	}

	if( m_bLaunchAutoRestart == true )
	{
		OnEsdLaunch();
	}

	if( m_bLoginAutoRestart == true )
	{
		OnEsdLaunch_LoginServer();
	}	

	if( m_bChannelAutoRestart == true )
	{
		OnEsdLaunch_ChannelServer();
	}

	if( m_bGlobalAutoRestart == true )
	{
		OnEsdLaunch_GlobalServer();
	}	
}

std::string KEsdRemoteController_ServerImp::GetTimeTag()
{
	time_t t0 = time(0);
	tm* t = localtime(&t0);

	std::ostringstream os;
	os
		<< 1900 + t->tm_year	<< "_"
		<< t->tm_mon + 1		<< "_"
		<< t->tm_mday			<< "_"
		<< t->tm_hour			<< "_"
		<< t->tm_min;

	return os.str();
}

void KEsdRemoteController_ServerImp::LoadConfig()
{
	lua_tinker::class_add<KEsdRemoteController_ServerImp>( g_pLua, "KEsdRemoteController_ServerImp" );
	lua_tinker::class_def<KEsdRemoteController_ServerImp>( g_pLua, "AddProcessData",		&KEsdRemoteController_ServerImp::AddProcessData_Lua );
	lua_tinker::class_def<KEsdRemoteController_ServerImp>( g_pLua, "AddUserData",			&KEsdRemoteController_ServerImp::AddUserData_Lua );
	lua_tinker::class_def<KEsdRemoteController_ServerImp>( g_pLua, "AddPhoneNum",			&KEsdRemoteController_ServerImp::AddPhoneNum_Lua );
	lua_tinker::class_def<KEsdRemoteController_ServerImp>( g_pLua, "AddHSBData",			&KEsdRemoteController_ServerImp::AddHSBData_Lua );
	lua_tinker::class_def<KEsdRemoteController_ServerImp>( g_pLua, "AddSVNData",			&KEsdRemoteController_ServerImp::AddSVNData_Lua );
	lua_tinker::class_def<KEsdRemoteController_ServerImp>( g_pLua, "SetWindowsUpdateUrl",	&KEsdRemoteController_ServerImp::SetWindowsUpdateUrl_Lua );
	lua_tinker::class_def<KEsdRemoteController_ServerImp>( g_pLua, "SetVirusProgramData",	&KEsdRemoteController_ServerImp::SetVirusProgramData_Lua );

	lua_tinker::decl( g_pLua, "ServerImp", this );

	std::string strPath = KGetCurFolder();
	strPath += "\\";
	strPath += "Config.lua";

	if( 0 != luaL_dofile( g_pLua, strPath.c_str() ) )
	{
		KDbgStm << "config.lua open failed.!";
	}
}

void KEsdRemoteController_ServerImp::UpdateLaunching()
{
	if( m_bLaunching == true )
	{	
		if ( m_hServer[SI_CENTER_SERVER] != NULL )
		{
			if ( m_hServer[SI_GAME_SERVER] == NULL )
			{
				HWND hProcessGameServer = FindWindow( NULL, L"GameServer_Loading" ); 
				if ( hProcessGameServer == NULL )
				{
					OnCreateProcess( GetServerFile( SI_GAME_SERVER ), GetServerPath( SI_GAME_SERVER ) );
				}
			}

			m_bLaunching = false;
		}
	}	
}

void KEsdRemoteController_ServerImp::ServerStateBroadCast()
{
	KPacket kPacket( eESD_PID_SC_STATUS );
	kPacket << _KNetUtil()->GetHostIP() << m_hServer[SI_LOGIN_SERVER] << m_hServer[SI_CENTER_SERVER] << m_hServer[SI_GAME_SERVER] << m_hServer[SI_CHANNEL_SERVER];
	// 글로벌 서버 정보는 아직 넣지 않는다.

	SiKSimpleIOCP()->BroadCastPacket( kPacket );
}

void KEsdRemoteController_ServerImp::ServerCrashBroadCast( SERVER_INDEX eServerIndex )
{
	int iPacketID;

	switch( eServerIndex ) 
	{
	case SERVER_INDEX::SI_LOGIN_SERVER:
		iPacketID = eESD_PID_SC_LG_CRASH;
		break;

	case SERVER_INDEX::SI_CENTER_SERVER:
		iPacketID = eESD_PID_SC_CN_CRASH;
		break;

	case SERVER_INDEX::SI_GAME_SERVER:
		iPacketID = eESD_PID_SC_GS_CRASH;
		break;

	case SERVER_INDEX::SI_CHANNEL_SERVER:
		iPacketID = eESD_PID_SC_CH_CRASH;
		break;

	case SERVER_INDEX::SI_GLOBAL_SERVER:
		//iPacketID = eESD_PID_SC_CH_CRASH; // 글로벌 서버 정보는 아직 넣지 않는다.
		return;

	default:
		return;
	}

	KPacket kPacket( iPacketID );
	kPacket << _KNetUtil()->GetHostIP();

	SiKSimpleIOCP()->BroadCastPacket( kPacket );
}

void KEsdRemoteController_ServerImp::OnRecv( char* pPacketBuffer, DWORD dwRecvSize )
{
	KPacket kPacket;
	kPacket.SetBuffer( pPacketBuffer, dwRecvSize );

	int iPacketID = kPacket.GetID();
	switch ( iPacketID )
	{
	case eESD_PID_CS_LAUNCH:
		{
			KDbgStm << "Center & Game Launch Packet!";

			OnEsdLaunch();
		}
		break;

	case eESD_PID_CS_LAUNCH_CO:
		{
			KDbgStm << "Center Only Packet!";

			OnEsdLaunch_CenterServer();
		}
		break;

	case eESD_PID_CS_LAUNCH_LG:
		{
			KDbgStm << "Login Only Packet!";

			OnEsdLaunch_LoginServer();
		}
		break;

	case eESD_PID_CS_LAUNCH_CH:
		{
			KDbgStm << "Channel Only Packet!";

			OnEsdLaunch_ChannelServer();
		}
		break;

	case eESD_PID_CS_SVN_UPDATE:
		{
			KDbgStm << "SVN Update Packet!";

			OnEsdSVNUpdate();
		}
		break;

	case eESD_PID_CS_SHUTDOWN:
		{
			KDbgStm << "Server Shutdown Packet!";

			int iShutDownCode = 0;
			kPacket >> iShutDownCode;

			// 서버 셧다운 코드를 검사합니다
			// 셧다운 코드가 없다면 정상적인 패킷이 아니므로 서버를 종료하지 않습니다.
			if( iShutDownCode != static_cast<int>(eESD_PID_SHUTDOWN_CODE) )
			{
				// 주석을 남기자!
				KDbgStm << "Server Shutdown Code Auth Fail !!!!!";
                break;
			}

			OnEsdShutdown_Recv();
		}
		break;

	case eESD_PID_CS_HSB_UPDATE:
		{
			KDbgStm << "HSB Update Packet!";

			OnEsdHSBUpdate();
		}
		break;

	case eESD_PID_CS_WINDOWS_UPDATE:
		{
			KDbgStm << "Windows Update Packet!";

			OnEsdWindowsUpdate();
		}
		break;

	case eESD_PID_CS_VIRUS_CHECK:
		{
			KDbgStm << "Virus Check Packet!";

			OnEsdVirusCheck();
		}
		break;

	case eESD_PID_CS_DBCONNECTION_IDPW:
		{
			KDbgStm << "DB Connection ID&PW Packet!";

			std::string strID;
			std::string strPW;
			kPacket >> strID;
			kPacket >> strPW;
			OnEsdDBConnectionIDPW( strID, strPW );
		}
		break;
	}
}

void KEsdRemoteController_ServerImp::OnEsdLaunch()
{
	OnEsdLaunch_CenterServer();

	m_bLaunching = true;
	m_bLaunchAutoRestart = true;
}

void KEsdRemoteController_ServerImp::OnEsdShutdown()
{
	if( ::MessageBox( NULL, L"정말 서버를 끄시겠습니까?", L"!!!경고!!!", MB_OKCANCEL ) == IDCANCEL )
	{
		return;
	}

	OnEsdShutdown_Recv();
}

void KEsdRemoteController_ServerImp::OnEsdShutdown_Recv()
{
	if ( m_hServer[SI_LOGIN_SERVER] != NULL )
		SendMessage( m_hServer[SI_LOGIN_SERVER], WM_CLOSE, 1, 1 );

	if ( m_hServer[SI_CENTER_SERVER] != NULL )
		SendMessage( m_hServer[SI_CENTER_SERVER], WM_CLOSE, 1, 1 );

	if ( m_hServer[SI_GAME_SERVER] != NULL )
		SendMessage( m_hServer[SI_GAME_SERVER], WM_CLOSE, 1, 1 );

	if ( m_hServer[SI_CHANNEL_SERVER] != NULL )
		SendMessage( m_hServer[SI_CHANNEL_SERVER], WM_CLOSE, 1, 1 );

	if ( m_hServer[SI_GLOBAL_SERVER] != NULL )
		SendMessage( m_hServer[SI_GLOBAL_SERVER], WM_CLOSE, 1, 1 );

	// 로딩중인 서버도 종료
	HWND shutDownWnd = FindWindow( NULL, L"LoginServer_Loading" );
	if( shutDownWnd != NULL )
		SendMessage( shutDownWnd, WM_CLOSE, 1, 1 );

	shutDownWnd = FindWindow( NULL, L"CenterServer_Loading" );
	if( shutDownWnd != NULL )
		SendMessage( shutDownWnd, WM_CLOSE, 1, 1 );

	shutDownWnd = FindWindow( NULL, L"GameServer_Loading" );
	if( shutDownWnd != NULL )
		SendMessage( shutDownWnd, WM_CLOSE, 1, 1 );

	shutDownWnd = FindWindow( NULL, L"ChannelServer_Loading" );
	if( shutDownWnd != NULL )
		SendMessage( shutDownWnd, WM_CLOSE, 1, 1 );

	shutDownWnd = FindWindow( NULL, L"GlobalServer_Loading" );
	if( shutDownWnd != NULL )
		SendMessage( shutDownWnd, WM_CLOSE, 1, 1 );

	m_hServer[SI_LOGIN_SERVER]	 = NULL;
	m_hServer[SI_CENTER_SERVER]	 = NULL;
	m_hServer[SI_GAME_SERVER]	 = NULL;
	m_hServer[SI_CHANNEL_SERVER] = NULL;
	m_hServer[SI_GLOBAL_SERVER] = NULL;

	m_bLaunchAutoRestart	= false;
	m_bShutdowning			= true;
}

void KEsdRemoteController_ServerImp::OnEsdWindowsUpdate()
{
	// Launch Patcher
	PROCESS_INFORMATION pi;
	STARTUPINFOW si = { 0, };
	si.cb = sizeof( STARTUPINFOW );

	wchar_t* pcPath = L"c:\\";

	std::wstring wstrWindowsUpdate = L"explorer.exe ";
	wstrWindowsUpdate += m_wstrWindowsUpdateUrl;

	BOOL bRet = ::CreateProcessW( NULL, (wchar_t*) wstrWindowsUpdate.c_str(), NULL, NULL, FALSE, 0, NULL, pcPath, &si, &pi );
	if( FALSE == bRet )
	{
		KDbgStm << "windows update Process failed.!";
		KDbgStm << KncUtil::toNarrowString(wstrWindowsUpdate);
		return;
	}

	// 성공했을경우에도 로그를 남기도록 하자!    
}

void KEsdRemoteController_ServerImp::OnEsdSVNUpdate()
{
	std::vector< KSVNPath >::const_iterator vit;
	for( vit = m_vecSVNPath.begin(); vit != m_vecSVNPath.end(); ++vit )
	{
		PROCESS_INFORMATION pi;
		STARTUPINFOW si = { 0, };
		si.cb = sizeof( STARTUPINFOW );

		wchar_t* pcPath = NULL;

		if( !vit->m_wstrSVNPath.empty() )
		{
			pcPath = (wchar_t*) vit->m_wstrSVNPath.c_str();
		}

		BOOL bRet = ::CreateProcessW( NULL, (wchar_t*) vit->m_wstrSVNFile.c_str(), NULL, NULL, FALSE, 0, NULL, pcPath, &si, &pi );
		if( FALSE == bRet )
		{
			KDbgStm << "SVN update Process failed.!";
			KDbgStm << KncUtil::toNarrowString(vit->m_wstrSVNFile);
			KDbgStm << KncUtil::toNarrowString(vit->m_wstrSVNPath);
			return;
		}

		// 성공했을경우에도 로그를 남기도록 하자!
	}
}

void KEsdRemoteController_ServerImp::OnEsdHSBUpdate()
{
	// Launch Patcher
	PROCESS_INFORMATION pi;
	STARTUPINFOW si = { 0, };
	si.cb = sizeof( STARTUPINFOW );

	wchar_t* pcPath = NULL;

	if( !m_wstrHSBPath.empty() )
	{
		pcPath = (wchar_t*) m_wstrHSBPath.c_str();
	}

	BOOL bRet = ::CreateProcessW( NULL, (wchar_t*) m_wstrHSBFile.c_str(), NULL, NULL, FALSE, 0, NULL, pcPath, &si, &pi );
	if( FALSE == bRet )
	{
		KDbgStm << "HSB update Process failed.!";
		KDbgStm << KncUtil::toNarrowString(m_wstrHSBFile);
		KDbgStm << KncUtil::toNarrowString(m_wstrHSBPath);
		return;
	}

	// 성공했을경우에도 로그를 남기도록 하자!
}

void KEsdRemoteController_ServerImp::OnEsdVirusCheck()
{
	// Launch Patcher
	PROCESS_INFORMATION pi;
	STARTUPINFOW si = { 0, };
	si.cb = sizeof( STARTUPINFOW );

	wchar_t* pcPath = NULL;

	if( !m_wstrVirusPath.empty() )
	{
		pcPath = (wchar_t*) m_wstrVirusPath.c_str();
	}

	BOOL bRet = ::CreateProcessW( NULL, (wchar_t*) m_wstrVirusFile.c_str(), NULL, NULL, FALSE, 0, NULL, pcPath, &si, &pi );
	if( FALSE == bRet )
	{
		KDbgStm << "Virus Check Process failed.!";
		KDbgStm << KncUtil::toNarrowString(m_wstrVirusFile);
		KDbgStm << KncUtil::toNarrowString(m_wstrVirusPath);
		return;
	}

	// 성공했을경우에도 로그를 남기도록 하자!    
}

void KEsdRemoteController_ServerImp::OnEsdDBConnectionIDPW( const std::string& strID, const std::string& strPW )
{
	KClearString kIDPW;
	kIDPW += L" ";
	kIDPW += KncUtil::toWideString( strID );
	kIDPW += L" ";
	kIDPW += KncUtil::toWideString( strPW );

	m_kDBConnStr.Init( kIDPW.GetString() );
}

bool KEsdRemoteController_ServerImp::OnCreateProcess( std::wstring wstrCmdline, std::wstring wstrPath )
{
	// Launch Patcher
	PROCESS_INFORMATION pi;
	STARTUPINFOW si = { 0, };
	si.cb = sizeof( STARTUPINFOW );

	//{{ 2009. 6. 17  최육사	DBConnStr
	KClearString kCmdLine;
	kCmdLine = wstrCmdline;

	if( IsDBConnStr() )
	{
		if( m_kDBConnStr.IsEmpty() )
		{
			// 다이얼로그를 열어서 ID와 PW를 입력받는다.
			if( OpenDBConnectionDlg() == false )
				return false;
		}
		//}}
		
		// ID와 PW를 실행라인 전달인자로 넘김
		kCmdLine += m_kDBConnStr.GetClearString();
	}
	//}}

	wchar_t* pcPath = NULL;
	if( !wstrPath.empty() )
	{
		pcPath = (wchar_t*) wstrPath.c_str();
	}

	BOOL bRet = ::CreateProcessW( NULL, (wchar_t*) kCmdLine.GetString().c_str(),	NULL, NULL, FALSE, 0, NULL, pcPath, &si, &pi );
	if( FALSE == bRet )
	{
		return false;
	}

	return true;
}

void KEsdRemoteController_ServerImp::OnEsdLaunch_ChannelServer()
{
	if( m_hServer[SI_CHANNEL_SERVER] == NULL )
	{
		if( 
			( GetServerFile( SI_CHANNEL_SERVER, true ).empty() || GetServerPath( SI_CHANNEL_SERVER, true ).empty() )  &&
			( GetServerFile( SI_CHANNEL_SERVER ).empty() || GetServerPath( SI_CHANNEL_SERVER ).empty() )
			)
		{
			return;
		}

		HWND hProcessChannelServer = FindWindow( NULL, L"ChannelServer_Loading" );
		if( hProcessChannelServer == NULL )
		{
			OnCreateProcess( GetServerFile( SI_CHANNEL_SERVER ), GetServerPath( SI_CHANNEL_SERVER ) );
		}
	}

	m_bChannelAutoRestart = true;
}

void KEsdRemoteController_ServerImp::OnEsdLaunch_LoginServer()
{
	if( m_hServer[SI_LOGIN_SERVER] == NULL )
	{
		if( 
			( GetServerFile( SI_LOGIN_SERVER, true ).empty() || GetServerPath( SI_LOGIN_SERVER, true ).empty() )  &&
			( GetServerFile( SI_LOGIN_SERVER ).empty() || GetServerPath( SI_LOGIN_SERVER ).empty() )
			)
		{
			return;
		}

		HWND hProcessLoginServer = FindWindow( NULL, L"LoginServer_Loading" );
		if( hProcessLoginServer == NULL )
		{
			OnCreateProcess( GetServerFile( SI_LOGIN_SERVER ), GetServerPath( SI_LOGIN_SERVER ) );
		}
	}

	m_bLoginAutoRestart = true;
}

void KEsdRemoteController_ServerImp::OnEsdLaunch_CenterServer()
{
	if ( m_hServer[SI_CENTER_SERVER] == NULL )
	{
		if( 
			GetServerFile( SI_CENTER_SERVER ).empty() || GetServerPath( SI_CENTER_SERVER ).empty() ||
			GetServerFile( SI_GAME_SERVER ).empty() || GetServerPath( SI_GAME_SERVER ).empty()
			)
		{
			return;
		}

		HWND hProcessCenterServer = FindWindow( NULL, L"CenterServer_Loading" ); 
		if ( hProcessCenterServer == NULL )
		{
			OnCreateProcess( GetServerFile( SI_CENTER_SERVER ), GetServerPath( SI_CENTER_SERVER ) );
		}
	}
}

void KEsdRemoteController_ServerImp::OnEsdLaunch_GlobalServer()
{
	if ( m_hServer[SI_GLOBAL_SERVER] == NULL )
	{
		if( 
			( GetServerFile( SI_GLOBAL_SERVER, true ).empty() || GetServerPath( SI_GLOBAL_SERVER, true ).empty() )  &&
			( GetServerFile( SI_GLOBAL_SERVER ).empty() || GetServerPath( SI_GLOBAL_SERVER ).empty() )
			)
		{
			return;
		}

		HWND hProcessCenterServer = FindWindow( NULL, L"GlobalServer_Loading" ); 
		if ( hProcessCenterServer == NULL )
		{
			OnCreateProcess( GetServerFile( SI_GLOBAL_SERVER ), GetServerPath( SI_GLOBAL_SERVER ) );
		}
	}

	m_bGlobalAutoRestart = true;
}

bool KEsdRemoteController_ServerImp::AddProcessData_Lua()
{
	KLuaManager luaManager( g_pLua );

	KProcessData sData;

	LUA_GET_VALUE_RETURN(	luaManager,	L"m_iID",		sData.m_iID,		0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strFile",	sData.m_wstrFile,	L"",	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strPath",	sData.m_wstrPath,	L"",	goto LoadFail; );	

	m_mapProcessData.insert( std::make_pair( sData.m_iID, sData ) );
	return true;

LoadFail:
	KDbgStm << "Process Data 파싱 실패" << trace;
	return false;
}

bool KEsdRemoteController_ServerImp::AddUserData_Lua()
{
	KLuaManager luaManager( g_pLua );

	KUserData sData;

	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strIP",		sData.m_wstrIP,		L"",	goto LoadFail; );
	LUA_GET_VALUE(			luaManager,	L"m_name",		sData.m_name,		L"" );
	
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_LG",		sData.m_iProcessID[SI_LOGIN_SERVER],	-1 );
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_CN",		sData.m_iProcessID[SI_CENTER_SERVER],	-1 );
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_GS",		sData.m_iProcessID[SI_GAME_SERVER],		-1 );
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_CH",		sData.m_iProcessID[SI_CHANNEL_SERVER],	-1 );
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_GB",		sData.m_iProcessID[SI_GLOBAL_SERVER],	-1 );
	

	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_LG_DBG",	sData.m_iProcessID_DBG[SI_LOGIN_SERVER],	-1 );
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_CN_DBG",	sData.m_iProcessID_DBG[SI_CENTER_SERVER],	-1 );
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_GS_DBG",	sData.m_iProcessID_DBG[SI_GAME_SERVER],		-1 );
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_CH_DBG",	sData.m_iProcessID_DBG[SI_CHANNEL_SERVER],	-1 );
	LUA_GET_VALUE(	luaManager,	L"PROCESS_ID_GB_DBG",	sData.m_iProcessID_DBG[SI_GLOBAL_SERVER],	-1 );

	LUA_GET_VALUE(	luaManager,	L"m_bDBConnStr",			sData.m_bDBConnStr,	false );

	m_mapUserData.insert( std::make_pair( sData.m_wstrIP, sData ) );
	return true;

LoadFail:
	KDbgStm << "User Data 파싱 실패" << trace;
	return false;
}

bool KEsdRemoteController_ServerImp::AddPhoneNum_Lua()
{
	KLuaManager luaManager( g_pLua );

	int iTemp = 0;
	std::wstring strPhoneNum;
	wchar_t szTitle[MAX_PATH] = L"";

	for( int i = 1;; ++i )
	{
		wsprintfW( szTitle, L"NUM_%d", i );

		LUA_GET_VALUE(	luaManager,	szTitle,	strPhoneNum,	L"" );

		if( strPhoneNum.compare( L"" ) == 0 )
		{
			break;
		}

		m_sPhoneNum.m_vecPhoneNum.push_back( strPhoneNum );
	}

	return true;
}

bool KEsdRemoteController_ServerImp::AddHSBData_Lua()
{
	KLuaManager luaManager( g_pLua );

	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strHSBFile",		m_wstrHSBFile,		L"",	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strHSBPath",		m_wstrHSBPath,		L"",	goto LoadFail; );
	return true;

LoadFail:
	KDbgStm << "HSB Data 파싱 실패" << trace;
	return false;
}

bool KEsdRemoteController_ServerImp::AddSVNData_Lua()
{
	KLuaManager luaManager( g_pLua );

	KSVNPath kInfo;

	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strSVNFile",		kInfo.m_wstrSVNFile,		L"",	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strSVNPath",		kInfo.m_wstrSVNPath,		L"",	goto LoadFail; );

	m_vecSVNPath.push_back( kInfo );
	return true;

LoadFail:
	KDbgStm << "SVN Data 파싱 실패" << trace;
	return false;
}

bool KEsdRemoteController_ServerImp::SetWindowsUpdateUrl_Lua()
{
	KLuaManager luaManager( g_pLua );

	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strWindowsUpdateUrl",		m_wstrWindowsUpdateUrl,		L"",	goto LoadFail; );	
	return true;

LoadFail:
	KDbgStm << "Windows Update Url 파싱 실패" << trace;
	return false;
}

bool KEsdRemoteController_ServerImp::SetVirusProgramData_Lua()
{
	KLuaManager luaManager( g_pLua );

	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strVirusFile",		m_wstrVirusFile,	L"",	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strVirusPath",		m_wstrVirusPath,	L"",	goto LoadFail; );
	return true;

LoadFail:
	KDbgStm << "Virus Program Data 파싱 실패" << trace;
	return false;
}

std::wstring KEsdRemoteController_ServerImp::GetServerFile(SERVER_INDEX eServerIndex, bool bIsDebug /* = false  */)
{
	const KProcessData* pData = GetProcessData( eServerIndex, bIsDebug );

	if( pData != NULL )
	{
		return pData->m_wstrFile;
	}

	KDbgStm << "User의 정보찾기 실패." << _KNetUtil()->GetHostIP() << trace;

	return L"";
}

std::wstring KEsdRemoteController_ServerImp::GetServerPath(SERVER_INDEX eServerIndex, bool bIsDebug /* = false  */)
{
	const KProcessData* pData = GetProcessData( eServerIndex, bIsDebug );

	if( pData != NULL )
	{
		return pData->m_wstrPath;
	}

	KDbgStm << "User의 정보찾기 실패." << _KNetUtil()->GetHostIP() << trace;

	return L"";
}

const KEsdRemoteController_ServerImp::KProcessData* KEsdRemoteController_ServerImp::GetProcessData( SERVER_INDEX eServerIndex, bool bIsDebug )
{
	std::map< std::wstring, KUserData >::iterator mit;
	mit = m_mapUserData.find( KncUtil::toWideString( _KNetUtil()->GetHostIP() ) );

	if( 
		mit != m_mapUserData.end() &&
		( eServerIndex >= SI_CENTER_SERVER && eServerIndex < SI_TOTAL_NUM )
		)
	{
		std::map<int,KProcessData>::iterator mit2;

		if( bIsDebug == true )
		{
			mit2 = m_mapProcessData.find( mit->second.m_iProcessID_DBG[eServerIndex] );

			if( mit2 != m_mapProcessData.end() )
			{
				return &(mit2->second);
			}
		}
		else
		{
			mit2 = m_mapProcessData.find( mit->second.m_iProcessID[eServerIndex] );

			if( mit2 != m_mapProcessData.end() )
			{
				return &(mit2->second);
			}
		}
	}

	return NULL;
}

bool KEsdRemoteController_ServerImp::IsDBConnStr()
{
	std::map< std::wstring, KUserData >::iterator mit;
	mit = m_mapUserData.find( KncUtil::toWideString( _KNetUtil()->GetHostIP() ) );
	if( mit == m_mapUserData.end() )
		return false;

	return mit->second.m_bDBConnStr;
}

bool KEsdRemoteController_ServerImp::OpenDBConnectionDlg()
{
	std::vector< KClearString > vecDBConnIDPW;

	DBConnectionDlg dbConnDlg;
	dbConnDlg.SetEditList( &vecDBConnIDPW );
	dbConnDlg.DoModal();

	// IP와 PW가 제대로 들어오지 않았다면 false
	if( vecDBConnIDPW.size() < 2 )
		return false;

	KClearString kIDPW;
	kIDPW += L" ";
	kIDPW += vecDBConnIDPW[0];
	kIDPW += L" ";
	kIDPW += vecDBConnIDPW[1];

	m_kDBConnStr.Init( kIDPW.GetString() );

	return true;
}

void KEsdRemoteController_ServerImp::OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
	theApp.m_kImp.OnAccept_( sock, sinRemote );
}

void KEsdRemoteController_ServerImp::OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
	KSimpleActor* pkSockObj = new KSimpleActor;

	pkSockObj->SetSocketInfo( sock, sinRemote );

	SiKSimpleIOCP()->AssociateSocket( pkSockObj );

	pkSockObj->InitRecv();
	pkSockObj->OnAcceptConnection();

	DWORD dwElapTime = ::GetTickCount();  // 040428. 처리 소요시간을 구한다.

	//KActorManager::GetKObj()->ReserveAdd( spActor );

	dwElapTime = ::GetTickCount() - dwElapTime;

	// 접속하자마자 현재 서버 상태를 전송함.
	KPacket kPacket( eESD_PID_SC_STATUS );
	kPacket << _KNetUtil()->GetHostIP() << m_hServer[SI_LOGIN_SERVER] << m_hServer[SI_CENTER_SERVER] << m_hServer[SI_GAME_SERVER] << m_hServer[SI_CHANNEL_SERVER];
	// 글로벌 서버 정보는 아직 보내지 않습니다!
	pkSockObj->Send( kPacket.GetBuffer(), kPacket.GetSize() );
}
