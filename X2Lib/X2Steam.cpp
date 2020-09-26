#include "stdafx.h"
#include "X2Steam.h"

#ifdef SERV_STEAM

#include "steam_api.h"

#ifdef STEAM_CEG
// Steam DRM header file
#include "cegclient.h"
#else
#define Steamworks_InitCEGLibrary() (true)
#define Steamworks_TermCEGLibrary()
#define Steamworks_TestSecret()
#define Steamworks_SelfCheck()
#endif

void MiniDumpFunction( unsigned int nExceptionCode, EXCEPTION_POINTERS *pException )
{
	// You can build and set an arbitrary comment to embed in the minidump here,
	// maybe you want to put what level the user was playing, how many players on the server,
	// how much memory is free, etc...
	SteamAPI_SetMiniDumpComment( "Minidump comment: SteamworksExample.exe\n" );

	// The 0 here is a build ID, we don't set it
	SteamAPI_WriteMiniDump( nExceptionCode, pException, 0 );
}

extern "C" void __cdecl SteamAPIDebugTextHook( int nSeverity, const char *pchDebugText )
{
	// if you're running in the debugger, only warnings (nSeverity >= 1) will be sent
	// if you add -debug_steamapi to the command-line, a lot of extra informational messages will also be sent
	::OutputDebugStringA( pchDebugText );

	if ( nSeverity >= 1 )
	{
		// place to set a breakpoint for catching API errors
		int x = 3;
		x = x;
	}
}


bool CX2Steam::InitSteam()
{
	if ( SteamAPI_RestartAppIfNecessary( k_uAppIdInvalid ) )
	{
		return false;
	}

	// Init Steam CEG
	if ( !Steamworks_InitCEGLibrary() )
	{
		return false;
	}

	if ( !SteamAPI_Init() )
	{
		//MessageBox(NULL, L"SteamAPI_Init Failed!", L"STEAM", MB_OK);
		return false;
	}
	
	SteamUtils()->SetOverlayNotificationPosition( k_EPositionTopRight );

	if(!SteamUser()){
		EndSteam();
		return false;
	}

	if(SteamUser()->BLoggedOn() == false){
		EndSteam();
		return false;
	}

	//MessageBox(NULL, L"Run by Steam Client!", L"Steam", MB_OK);

	_set_se_translator( MiniDumpFunction );
	SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );

	return true;
}


void CX2Steam::RunSteamCallback()
{
	SteamAPI_RunCallbacks();
}


bool CX2Steam::IsSteamUser()
{
	if( SteamUser() != NULL)
		return SteamUser()->BLoggedOn();
	else
		return false;
}


bool CX2Steam::IsValidSteamUser( const std::wstring wstrPW_ )
{
	if( wstrPW_ == L"J2380DE81621XZ")
	{
		return IsSteamUser();
	}
	else
	{
		// 스팀 비번 사용 안하면 스팀 유저가 아니므로 true
		return true;
	}
}


bool CX2Steam::GetSteamUserIDAndPass( OUT std::wstring& wstrID_, OUT std::wstring& wstrPw_ )
{
	if( SteamUser() && SteamUser()->BLoggedOn() )
	{
		CSteamID steamID = SteamUser()->GetSteamID();
		wchar_t wcharID[128] = {0};
		_itow(steamID.GetAccountID(), wcharID, 10);
		wstrID_ = wcharID;						
		wstrPw_ = L"J2380DE81621XZ";

		//MessageBox(NULL, wstrID_.c_str(), L"Steam ID", MB_OK);

		return true;
	}
	else
		return false;
}


std::wstring CX2Steam::GetSteamUserID()
{
	if( SteamUser() )
	{
		CSteamID steamID = SteamUser()->GetSteamID();
		wchar_t wcharID[128] = {0};
		_itow(steamID.GetAccountID(), wcharID, 10);
		return wcharID;
	}
	else
		return L"";
}

void CX2Steam::OpenSteamOverlayWebPage( IN std::wstring& wstrURL_ )
{
	if( SteamUser() )
	{
		std::string strURL = KncUtil::toNarrowString(wstrURL_);
		SteamFriends()->ActivateGameOverlayToWebPage( strURL.c_str() );
	}
	else
		wstrURL_ = L"Error";
}

void CX2Steam::EndSteam()
{
	SteamAPI_Shutdown();
}

#endif //SERV_STEAM