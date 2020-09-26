#include "StdAfx.h"
#include ".\luacompileutil.h"
#include <windows.h>
#include <process.h>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>

#include <string>
#include <iostream>

long GetFileSize(const char* strFileName)
{
	FILE* file = fopen(strFileName,"rb");
	if(file == NULL)
		return -1;
	long size = 0;
	fseek(file,0,SEEK_END);
	size = ftell(file);
	fclose(file);

	return size;
}

using namespace std;


CLuaCompileUtil::CLuaCompileUtil(void)
{	
}

CLuaCompileUtil::~CLuaCompileUtil(void)
{
}

bool CLuaCompileUtil::Compile( const std::wstring& wstrFromFilePath, const std::wstring& wstrToFilePath, const std::wstring& wstrLuacCommand, const std::wstring& wstrAnsiToUTF8Command )
{
	if( wstrFromFilePath.empty() )
		return false;

	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";

	_wsplitpath( wstrFromFilePath.c_str(), drive, dir, fname, ext );

	// 컴파일 결과물 출력 경로
	std::wstring wstrToFileFullPath = wstrToFilePath;	
	wstrToFileFullPath += L"\\";
	wstrToFileFullPath += fname;
	wstrToFileFullPath += ext;
	
	MakeUpperCase( ext );
	if( wcscmp( ext, L".LUA" ) != 0 )
		return false;

	// 기존의 파일의 삭제를 위해서
	_wchmod( wstrToFileFullPath.c_str(), _S_IREAD | _S_IWRITE );

	// 기존 파일 삭제
	DeleteFile( wstrToFileFullPath.c_str() );

	// lua 컴파일	
	_wspawnlp( _P_WAIT, wstrLuacCommand.c_str(), L"luac.exe", L"-o", wstrToFileFullPath.c_str(), wstrFromFilePath.c_str(), NULL );
	//_wspawnlp( _P_WAIT, wstrLuacCommand.c_str(), L"luac.exe", L"-o", strBinFileName.c_str(), strSrcFileName.c_str(), NULL );



    // UTF-8 포맷이므로 파일 앞에 BOM을 붙인다
    _wspawnlp( _P_WAIT, wstrAnsiToUTF8Command.c_str(), L"ansitoutf8.exe", L"-f", wstrToFileFullPath.c_str(), NULL );





	// 컴파일 제대로 됐는지 확인
#include <fstream>
	std::string strTemp;
	ConvertWCHARToChar( strTemp, wstrToFileFullPath.c_str() );
	std::wifstream ifs( strTemp.c_str() );
	return ifs.good();
}

// szPath는 file 이름이 아닌 directory~. 끝이 \로 끝나야 함
bool CLuaCompileUtil::Compile( const std::wstring& szPath, 
							   const std::wstring& wstrLuacCommand, 
                               const std::wstring& wstrAnsiToUTF8Command,
							   bool bSubDir, 
                               std::map< std::wstring, MemberFileInfo >& mapLuaFiles, 
							   bool bCopyFile /*= false*/ )
{
	if( szPath.empty() )
		return false;
	
	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;

	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";

	_wsplitpath( szPath.c_str(), drive, dir, fname, ext );

	// 컴파일 결과물 임시폴더
	std::wstring strLuaTempPath = drive;
	strLuaTempPath += L"\\LuaTemp\\";

	// 임시폴더 생성
	CreateDirectory( strLuaTempPath.c_str(), NULL );

	std::wstring strRootDir = szPath;
	strRootDir += L"\\";

	std::wstring strFindPath = strRootDir;
	strFindPath += L"*.*";

	hSearch = FindFirstFile( strFindPath.c_str(), &fd );
	if( hSearch == INVALID_HANDLE_VALUE )
		return false;

	do
	{
		if( 0 != wcscmp(fd.cFileName, L".") && 0 != wcscmp(fd.cFileName, L"..") && wcscmp(fd.cFileName, L".svn") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( bSubDir == true )
				{
					wstring strSubDir = strRootDir;
					strSubDir += fd.cFileName;
					//strSubDir += L"\\";
					Compile( strSubDir.c_str(), wstrLuacCommand, wstrAnsiToUTF8Command, bSubDir, mapLuaFiles, bCopyFile );
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

					wstring strBinFileName = strLuaTempPath;
					strBinFileName += fname;
					strBinFileName += L".lua";

					// 컴파일 할것인가? 카피 할것인가?
					if( bCopyFile == false )
                    {
						_wspawnlp( _P_WAIT, wstrLuacCommand.c_str(), L"luac.exe", L"-o", strBinFileName.c_str(), strSrcFileName.c_str(), NULL );

                        _wspawnlp( _P_WAIT, wstrAnsiToUTF8Command.c_str(), L"ansitoutf8.exe", L"-f", strBinFileName.c_str(), NULL );
                    }
					else
						CopyFileW( strSrcFileName.c_str(), strBinFileName.c_str(), false );

					std::string strFileNameForSize;
					ConvertWCHARToChar( strFileNameForSize, strBinFileName.c_str() );

					// 컴파일된 Lua Script 파일 정보 얻기
					MemberFileInfo memberFile;
					memberFile.m_wstrFileName = fd.cFileName;
					memberFile.m_iFileSize	  = GetFileSize( strFileNameForSize.c_str() ) / 1024; // byte -> kilobyte
					memberFile.m_iFileState	  = FS_NONE;
					memberFile.m_wstrRealPath = strBinFileName; // 파일이 원래 있던 경로

					mapLuaFiles.insert( std::make_pair( memberFile.m_wstrFileName, memberFile ) );

					_wchmod( strBinFileName.c_str(), _S_IREAD | _S_IWRITE ); // 파일 삭제를 위해서
				}							
			}			
		}
	} while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
	return true;
}

bool CLuaCompileUtil::TextFileCopy( const std::wstring& szPath, bool bSubDir, std::map< std::wstring, MemberFileInfo >& mapTextFiles )
{
	if( szPath.empty() )
		return false;

	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;

	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";

	_wsplitpath( szPath.c_str(), drive, dir, fname, ext);

	// Text파일 복사 임시폴더
	std::wstring strLuaTempPath = drive;
	strLuaTempPath += L"\\LuaTemp\\";

	// 임시폴더 생성
	CreateDirectory( strLuaTempPath.c_str(), NULL );

	std::wstring strRootDir = szPath;
	strRootDir += L"\\";

	std::wstring strFindPath = strRootDir;
	strFindPath += L"\\*";

	hSearch = FindFirstFile( strFindPath.c_str(), &fd );
	if( hSearch == INVALID_HANDLE_VALUE )
		return false;

	do
	{
		if( 0 != wcscmp(fd.cFileName, L".") && 0 != wcscmp(fd.cFileName, L"..") && wcscmp(fd.cFileName, L".svn") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( bSubDir == true )
				{
					wstring strSubDir = strRootDir;
					strSubDir += fd.cFileName;
					strSubDir += L"\\";
					TextFileCopy( strSubDir.c_str(), bSubDir, mapTextFiles );
				}
			}
			else
			{
				//_chdir( szPath );

				_wsplitpath( fd.cFileName, NULL, NULL, fname, ext );
				MakeUpperCase( ext );

				if( wcscmp( ext, L".TXT" ) == 0 )
				{
					wstring strSrcFileName = strRootDir;
					strSrcFileName += fd.cFileName;

					wstring strBinFileName = strLuaTempPath;
					strBinFileName += fname;
					strBinFileName += L".txt";

					// Text파일 카피
					CopyFileW( strSrcFileName.c_str(), strBinFileName.c_str(), false );

					std::string strFileNameForSize;
					ConvertWCHARToChar( strFileNameForSize, strBinFileName.c_str() );

					// Text 파일 정보 얻기
					MemberFileInfo memberFile;
					memberFile.m_wstrFileName = fd.cFileName;
					memberFile.m_iFileSize	  = GetFileSize( strFileNameForSize.c_str() ) / 1024; // byte -> kilobyte
					memberFile.m_iFileState	  = FS_NONE;
					memberFile.m_wstrRealPath = strBinFileName; // 파일이 원래 있던 경로

					mapTextFiles.insert( std::make_pair( memberFile.m_wstrFileName, memberFile ) );

					_wchmod( strBinFileName.c_str(), _S_IREAD | _S_IWRITE ); // 파일 삭제를 위해서
				}				
			}			
		}
	} while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
	return true;
}