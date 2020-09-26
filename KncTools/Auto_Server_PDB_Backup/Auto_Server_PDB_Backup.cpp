// Auto_Server_PDB_Backup.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <boost/format.hpp>
using namespace std;

#define RELEASE_STR		  "Release"
#define DEBUG_STR		  "Debug"

#define LOGIN_SERVER_STR  "LoginServer"
#define CENTER_SERVER_STR "CenterServer"
#define GAME_SERVER_STR	  "GameServer"

#define FORMAT( x ) boost::str( boost::format x )

int _tmain(int argc, _TCHAR* argv[])
{
	bool bIsDebug = false;

	if( argc > 1 )
	{
		cout<< argv[1] <<endl;
		if( strncmp( argv[1], "debug", 5 ) == 0 )
		{
			bIsDebug = true;
		}
	}

	
	SYSTEMTIME sysTime;
	GetSystemTime( &sysTime );

	sysTime.wHour += 9;
	
	std::string dirName;
	
	if( bIsDebug == true )
	{
		dirName = FORMAT( ( "%04d%02d%02d_%02d%02d_%s" )
			% (int)sysTime.wYear
			% (int)sysTime.wMonth
			% (int)sysTime.wDay
			% (int)sysTime.wHour
			% (int)sysTime.wMinute
			% DEBUG_STR );
	}
	else
	{
		dirName = FORMAT( ("%04d%02d%02d_%02d%02d_%s" )
			% (int)sysTime.wYear
			% (int)sysTime.wMonth
			% (int)sysTime.wDay
			% (int)sysTime.wHour
			% (int)sysTime.wMinute
			% RELEASE_STR );
	}	

	std::string makeDir = "md ";
	std::string copyPath = "D:\\ServerBackup\\";
	makeDir += copyPath += dirName;

	system( makeDir.c_str() );
	cout<<makeDir<<endl;

	//////////////////////////////////////////////////////////////////////////
		
	std::string LoginServerPath	 = "D:\\projectx2\\X2Project\\KncWX2Server\\LoginServer";
	std::string CenterServerPath = "D:\\projectx2\\X2Project\\KncWX2Server\\CenterServer";
	std::string GameServerPath	 = "D:\\projectx2\\X2Project\\KncWX2Server\\GameServer";
	
	

	//////////////////////////////////////////////////////////////////////////
	
	std::string copyStr;
	std::string sourceStr;
	std::string destiStr;


	if( bIsDebug == true )
	{
		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%sD.exe" ) % LOGIN_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% LoginServerPath
			% DEBUG_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%sD.pdb" ) % LOGIN_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% LoginServerPath
			% DEBUG_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%sD.exe" ) % CENTER_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% CenterServerPath
			% DEBUG_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%sD.pdb" ) % CENTER_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% CenterServerPath
			% DEBUG_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%sD.exe" ) % GAME_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% GameServerPath
			% DEBUG_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%sD.pdb" ) % GAME_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% GameServerPath
			% DEBUG_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;		
	}
	else
	{
		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%s.exe" ) % LOGIN_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% LoginServerPath
			% RELEASE_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%s.pdb" ) % LOGIN_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% LoginServerPath
			% RELEASE_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%s.exe" ) % CENTER_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% CenterServerPath
			% RELEASE_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%s.pdb" ) % CENTER_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% CenterServerPath
			% RELEASE_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%s.exe" ) % GAME_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% GameServerPath
			% RELEASE_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;

		sourceStr.clear(); copyStr.clear();
		sourceStr = FORMAT( ( "%s.pdb" ) % GAME_SERVER_STR );
		copyStr = FORMAT( ( "copy %s\\%s\\%s %s\\%s" )
			% GameServerPath
			% RELEASE_STR
			% sourceStr
			% copyPath
			% sourceStr
			);

		system( copyStr.c_str() );
		cout<<copyStr<<endl;
	}

	return 0;
}

