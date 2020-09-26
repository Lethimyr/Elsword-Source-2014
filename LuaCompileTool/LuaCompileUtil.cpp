#include "StdAfx.h"
#include ".\luacompileutil.h"
#include <windows.h>
#include <process.h>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>

#include <string>
#include <iostream>

using namespace std;


//extern char g_szBinDirectory[512];


extern WCHAR g_szBinDirectory[512];



CLuaCompileUtil::CLuaCompileUtil(void)
{
}

CLuaCompileUtil::~CLuaCompileUtil(void)
{
}



// szPath는 file 이름이 아닌 directory~. 끝이 \로 끝나야 함
void CLuaCompileUtil::Compile( const WCHAR* szPath, bool bSubDir, bool bSplitPath /*= true*/ )
{
	if( szPath == NULL )
		return;
	
	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;

	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";	 
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";


	wstring strRootDir = L"";
	if( true == bSplitPath )
	{
		_wsplitpath( szPath, drive, dir, fname, ext );
		strRootDir = drive;
		strRootDir += dir;
	}
	else
	{
		strRootDir = szPath;
	}
	

	wstring strFindPath = strRootDir;
	strFindPath += L"*";

	hSearch = FindFirstFile( strFindPath.c_str(), &fd );
	if( hSearch == INVALID_HANDLE_VALUE )
		return;

	do
	{
		if( 0 != wcscmp(fd.cFileName, L".") && 0 != wcscmp(fd.cFileName, L"..") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{	
				if( bSubDir == true )
				{
					wstring strSubDir = strRootDir;
					strSubDir += fd.cFileName;
					strSubDir += L"\\";
					Compile( strSubDir.c_str(), bSubDir );
				}
			}
			else
			{
				//_chdir( szPath );

				_wsplitpath( fd.cFileName, NULL, NULL, fname, ext );
				MakeUpperCase( ext );

				if( wcscmp( ext, L".LUA" ) == 0 )
				{
					wstring strSrcFileName = strRootDir;
					strSrcFileName += fd.cFileName;
					
					wstring strBinFileName = strRootDir;
					strBinFileName += fname;
					strBinFileName += L".luac";

					wstring luac_command = g_szBinDirectory;
					luac_command += L"\\luac.exe";
					
					_wspawnlp( _P_WAIT, luac_command.c_str(), L"luac.exe", L"-o", strBinFileName.c_str(), strSrcFileName.c_str(), NULL );

					_wchmod( strSrcFileName.c_str(), _S_IREAD | _S_IWRITE ); // 파일 삭제를 위해서
				}			
			}			
		}
	} while( FindNextFile( hSearch, &fd ) );

	
	// 소스 파일 삭제
	hSearch = FindFirstFile( strFindPath.c_str(), &fd );
	if( hSearch == INVALID_HANDLE_VALUE )
		return;

	do
	{
		if( 0 != wcscmp(fd.cFileName, L".") && 0 != wcscmp(fd.cFileName, L"..") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{	
				;
			}
			else
			{
				_wsplitpath( fd.cFileName, NULL, NULL, fname, ext );
				MakeUpperCase( ext );

				if( wcscmp( ext, L".LUA" ) == 0 )
				{
					wstring strSrcFileName = strRootDir;
					strSrcFileName += fd.cFileName;

					wstring strBinFileName = strRootDir;
					strBinFileName += fname;
					strBinFileName += L".luac";

					DeleteFile( strSrcFileName.c_str() );
					MoveFile( strBinFileName.c_str(), strSrcFileName.c_str() );
				}
			}			
		}
	} while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
}