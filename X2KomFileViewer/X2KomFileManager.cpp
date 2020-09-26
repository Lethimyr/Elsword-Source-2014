#include "StdAfx.h"
#include ".\x2komfilemanager.h"
#include "FileCrypt.h"
#include "X2ModifyKomMemberDlg.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 수정자: 김현철
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <strsafe.h>
#define _MODIFY_VERSION_01
#pragma warning(disable: 4995)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CX2KomFileManager::CX2KomFileManager(void)
{
	m_bIsCompareKom		  = false;
	/*m_wstrLuacCommand	  = L"C:\\projectx2\\Bin\\루아컴파일툴\\luac.exe";
	m_wstrXConvertCommand = L"C:\\projectx2\\bin\\XMeshConvert\\kogskinmesh.exe";*/

	m_iKFS_MIN_SIZE		  = 1024; // kilobyte (kb)
	m_iKFS_MAX_SIZE		  = 1024; // kilobyte (kb)	
	m_iKFS_AVERAGE_SIZE	  = 0;

	m_iThreadCount		  = 3;

	//{{ hyunchul // 2009-03-31, 11:16
	// 벡터 초기화
	m_vecKomFileList.resize(0);
	m_vecFolderList.resize(0);
	//}} hyunchul // 2009-10-14, 11:16

	InitializeCriticalSection( &m_csNewMemberForMT );
}

CX2KomFileManager::~CX2KomFileManager(void)
{
	if( m_ofs.is_open() )
		m_ofs.close();

	DeleteCriticalSection( &m_csNewMemberForMT );
}

bool CX2KomFileManager::CheckSameFiles( std::wstring& wstrNewPath, std::map< std::wstring, std::vector< std::wstring > >& vecSameFiles )	// vecSameFiles이기 보다는 mapSameFiles
{
	// log기록 파일 오픈
	if( !m_ofs.is_open() )
		m_ofs.open( "KomLog.txt" );
	
	WCHAR drive[_MAX_DRIVE]	= L"";
	WCHAR dir[_MAX_DIR]		= L"";
	WCHAR fname[_MAX_FNAME]	= L"";
	WCHAR ext[_MAX_EXT]		= L"";

	std::vector< std::wstring > vecMemberFileList;
	WCHAR wstrNewFolder[128];
	wcscpy( wstrNewFolder, wstrNewPath.c_str() );

	FindFolderFile( wstrNewFolder, vecMemberFileList );
	if( vecMemberFileList.empty() == true )
		return false;
	
	vecSameFiles.clear();
	std::map< std::wstring, std::vector< std::wstring > >::iterator mit;
	std::wstring wstrFileName;
	std::vector< std::wstring >::iterator vit = vecMemberFileList.begin();		// 신버전 폴더안의 파일경로들
	for( ; vit != vecMemberFileList.end(); vit++ )
	{
		_wsplitpath( (*vit).c_str(), drive, dir, fname, ext);
		
		wstrFileName = fname;
		wstrFileName += ext;

		mit = vecSameFiles.find( wstrFileName );		// 같을 파일명이 있는지 찾은 후
		if( mit == vecSameFiles.end() )					// 같은게 없으면
		{
			vecSameFiles.insert( std::make_pair( wstrFileName, std::vector< std::wstring >() ) ); // 파일명을 키값으로 MAP생성 (key:fileName, T: path)
			mit = vecSameFiles.find( wstrFileName );
		}

		mit->second.push_back( *vit );					// 해당 파일명을 키값으로 중복되는 파일들의 경로를 넣음
	}

	int iCount = 0;
	for( mit = vecSameFiles.begin(); mit != vecSameFiles.end(); mit++ )		// 로그 기록
	{
		if( mit->second.size() > 1 )
		{
			std::vector< std::wstring >::iterator vit;
			for( vit = mit->second.begin(); vit != mit->second.end(); vit++ )
			{
				std::string temp;
				ConvertWCHARToChar( temp, vit->c_str() );				
				m_ofs << GetFileSize( temp.c_str() ) / 1024 << L" kb   :   " << *vit << std::endl;
				++iCount;
			}

			m_ofs << std::endl;
		}
	}

	m_ofs.close();
	return true;
}

bool CX2KomFileManager::CompileLuaScript( IN std::wstring& wstrNewPath, OUT std::map< std::wstring, MemberFileInfo >& mapLuaFiles )
{
	mapLuaFiles.clear();

	return CLuaCompileUtil::Compile( wstrNewPath, m_wstrLuacCommand, m_wstrAnsiToUTF8Command, true, mapLuaFiles );
}

bool CX2KomFileManager::CompileLuaScript( IN std::vector< std::wstring >& vecNewFolderPaths, OUT std::map< std::wstring, MemberFileInfo >& mapLuaFiles )
{
	mapLuaFiles.clear();

	std::vector< std::wstring >::iterator vItr = vecNewFolderPaths.begin();
	bool bIsCompile	= false;

	while ( vItr != vecNewFolderPaths.end() )
	{
		if ( CLuaCompileUtil::Compile( (*vItr), m_wstrLuacCommand, m_wstrAnsiToUTF8Command, true, mapLuaFiles ) )
			bIsCompile = true;
		vItr++;
	}

	return bIsCompile;
}

bool CX2KomFileManager::PrepareTextFileEncrypt( IN std::wstring& wstrNewPath, OUT std::map< std::wstring, MemberFileInfo >& mapTextFiles )
{
	return CLuaCompileUtil::TextFileCopy( wstrNewPath, true, mapTextFiles );
}

bool CX2KomFileManager::PrepareTextFileEncrypt( IN std::vector< std::wstring >& vecNewFolderPaths, OUT std::map< std::wstring, MemberFileInfo >& mapTextFiles )
{
	std::vector< std::wstring >::iterator vItr = vecNewFolderPaths.begin();
	bool bIsEncrypt = false;

	while ( vItr != vecNewFolderPaths.end() )
	{
		if ( CLuaCompileUtil::TextFileCopy( (*vItr), true, mapTextFiles ) )
			bIsEncrypt = true;
		vItr++;
	}

	return bIsEncrypt;
}

bool CX2KomFileManager::RemoveCompiledLuaScript( IN std::map< std::wstring, MemberFileInfo >& mapLuaFiles )
{
	if( mapLuaFiles.empty() )
		return true;

	std::map< std::wstring, MemberFileInfo >::iterator mit = mapLuaFiles.begin();
	for( ; mit != mapLuaFiles.end(); mit++ )
	{
		MemberFileInfo& memberFileInfo = mit->second;

		DeleteFile( memberFileInfo.m_wstrRealPath.c_str() );
	}

	WCHAR drive[_MAX_DRIVE]	= L"";
	WCHAR dir[_MAX_DIR]		= L"";
	WCHAR fname[_MAX_FNAME]	= L"";
	WCHAR ext[_MAX_EXT]		= L"";

	mit = mapLuaFiles.begin();
	MemberFileInfo& memberFileInfo = mit->second;
	_wsplitpath( memberFileInfo.m_wstrRealPath.c_str(), drive, dir, fname, ext);

	std::wstring wstrRemoveDirectory = drive;
	wstrRemoveDirectory += dir;
	RemoveDirectory( wstrRemoveDirectory.c_str() );

	return true;
}

bool CX2KomFileManager::EncryptLuaScript( std::map< std::wstring, MemberFileInfo >& mapLuaFiles )
{
	if( m_vecEncryptionKey.empty() )
		return false;

	bool bIsSuccess = false;
	std::string strTemp;

	std::map< std::wstring, MemberFileInfo >::iterator mit = mapLuaFiles.begin();
	for( ; mit != mapLuaFiles.end(); mit++ )
	{
		MemberFileInfo& fileInfo = mit->second;
		ConvertWCHARToChar( strTemp, fileInfo.m_wstrRealPath.c_str() );
		bIsSuccess = FileCrypt::FileEncrypt( m_vecEncryptionKey, strTemp );

		if ( bIsSuccess == false )
		{
			return false;
		}
	}

	return bIsSuccess;
}

bool CX2KomFileManager::ParsingKomFileAndMemberFile( std::wstring& wstrOldPath, std::wstring& wstrNewPath, IN const std::map< std::wstring, MemberFileInfo >& mapLuaFiles, IN bool bIsLuaScriptComplile /*= false*/, IN bool bIsFileCrypt /*= false*/ )
{
	m_bIsCompareKom = true;

	m_wstrOldPath = wstrOldPath;
	m_wstrNewPath = wstrNewPath;
	
	WCHAR drive[_MAX_DRIVE]	= L"";
	WCHAR dir[_MAX_DIR]		= L"";
	WCHAR fname[_MAX_FNAME]	= L"";
	WCHAR ext[_MAX_EXT]		= L"";

	m_mapOldMemberFile.clear();
	m_mapNewMemberFile.clear();
	m_vecInsertedFile.clear();
	m_vecDeletedFile.clear();

	//////////////////////////////////////////////////////////////////////////
	// Old Path -> kom file load
	std::vector< std::wstring > vecKomFileList;
	WCHAR wstrOldFolder[MAX_PATH];
	wcscpy( wstrOldFolder, wstrOldPath.c_str() );

    FindFolderFile( wstrOldFolder, vecKomFileList );
	if( vecKomFileList.empty() == true )
		return false;

	//////////////////////////////////////////////////////////////////////////	
	// verify kom file list
	int iTotalFileCount = 0;
	std::vector< std::wstring >::iterator vit;
	for( vit = vecKomFileList.begin(); vit != vecKomFileList.end(); vit++ )
	{
		_wsplitpath( (*vit).c_str(), drive, dir, fname, ext);

		// 확장자 검사
		std::wstring wstrExt = ext;
		MakeUpperCase( wstrExt );

		// kom파일이 아니면 continue
		if( wstrExt != L".KOM" )
			continue;

		// kom file load
		CMassFile massFile;
		CHAR strTemp[256] = "";
		WideCharToMultiByte( CP_ACP, 0, (*vit).c_str(), -1, strTemp, MAX_PATH, NULL, NULL );
		
		massFile.LoadMassFile( strTemp ); // mass file load

		CMassFile::MASSFILE_MEMBERFILEINFO Info;
		ZeroMemory( &Info, sizeof( Info ) );
		WCHAR wstrTemp[256] = L"";

		for( UINT i = 0; i < massFile.GetFileCount(); ++i )
		{
			massFile.GetMemberFile(i,&Info);

			MultiByteToWideChar( CP_ACP, 0, Info.strFileName, -1, wstrTemp, MAX_PATH );
		
			// kom 등록 확인
			MemberFileIterator mit;
			mit = m_mapOldMemberFile.find( std::wstring( fname ) );
			if( mit == m_mapOldMemberFile.end() )
			{
				m_mapOldMemberFile.insert( std::make_pair( std::wstring( fname ), std::vector< MemberFileInfo >() ) );
				mit = m_mapOldMemberFile.find( std::wstring( fname ) );
			}

			// 멤버 파일 저장
			MemberFileInfo memberFile;
			memberFile.m_wstrFileName = wstrTemp;
			memberFile.m_iFileState	  = FS_NONE;
			memberFile.m_iFileSize	  = Info.size / 1024; // byte -> kilobyte

			mit->second.push_back( memberFile );

			// for log
			++iTotalFileCount;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// New Path -> naked member file load
	std::vector< std::wstring > vecNakedMemberFileList;
	WCHAR wstrNewFolder[MAX_PATH];
	wcscpy( wstrNewFolder, wstrNewPath.c_str() );

	FindFolderFile( wstrNewFolder, vecNakedMemberFileList );
	if( vecNakedMemberFileList.empty() == true )
		return false;

	//////////////////////////////////////////////////////////////////////////
	// verify naked member file list
	WCHAR wstrNewKom[256] = L"data0";

	for( vit = vecNakedMemberFileList.begin(); vit != vecNakedMemberFileList.end(); vit++ )
	{
		_wsplitpath( (*vit).c_str(), drive, dir, fname, ext);

		// 확장자 검사
		std::wstring wstrExt = ext;
		MakeUpperCase( wstrExt );

		if( wstrExt == L".KOM" )
		{
			::MessageBox( NULL, L"member파일중에 kom파일이 섞여있습니다!", L"에러!", MB_OK );
			return false;
		}

		// mp3 파일은 포함안함.
		if( wstrExt == L".MP3"  ||  wstrExt == L".X_OLD"  ||  wstrExt == L".Y_OLD" )
			continue;

		// File full name
		std::wstring wstrFileName;
		wstrFileName = fname;
		wstrFileName += ext;

		//////////////////////////////////////////////////////////////////////////
		// 중복 압축 방지
		bool bIsNoSame = false;
		MemberFileIterator mitNoSame = m_mapNewMemberFile.begin();
		std::vector< MemberFileInfo >::iterator vitNoSame;
		for( ; mitNoSame != m_mapNewMemberFile.end(); mitNoSame++ )
		{
			vitNoSame = std::find_if( mitNoSame->second.begin(), mitNoSame->second.end(), MemberFileInfo::FIND_IF( wstrFileName ) );
			if( vitNoSame != mitNoSame->second.end() )
				bIsNoSame = true;
		}
		if( bIsNoSame == true )
			continue;
		//////////////////////////////////////////////////////////////////////////
		// File full path
		std::string strFileNameForSize;
		ConvertWCHARToChar( strFileNameForSize, (*vit).c_str() );

		//////////////////////////////////////////////////////////////////////////
		// 스크립트라면..
		if( wstrExt == L".TXT"  ||  wstrExt == L".LUA" )
		{
			// Lua Script Compile Mode Check
			if( bIsLuaScriptComplile == true  &&  wstrExt == L".LUA" )
				continue;

			// Lua & Text File Encrypt Mode Check
			if( bIsFileCrypt == true )
				continue;

			MemberFileInfo memberFile;
			memberFile.m_wstrFileName = wstrFileName;
			memberFile.m_iFileSize	  = GetFileSize( strFileNameForSize.c_str() ) / 1024; // byte -> kilobyte
			memberFile.m_iFileState	  = FS_NONE;
			memberFile.m_wstrRealPath = (*vit).c_str(); // 파일이 원래 있던 경로

			// 평균값 안넘을시 kom넣기
			MemberFileIterator mit = m_mapNewMemberFile.find( std::wstring( wstrNewKom ) );
			if( mit == m_mapNewMemberFile.end() )
			{
				m_mapNewMemberFile.insert( std::make_pair( std::wstring( wstrNewKom ), std::vector< MemberFileInfo >() ) );
				mit = m_mapNewMemberFile.find( std::wstring( wstrNewKom ) );
			}
			mit->second.push_back( memberFile );
			continue;
		}
		//////////////////////////////////////////////////////////////////////////		

		bool bIsFindSucc = false;
		std::vector< MemberFileInfo >::iterator vit2;
		MemberFileIterator mitNew;
		MemberFileIterator mit = m_mapOldMemberFile.begin();
		for( ; mit != m_mapOldMemberFile.end(); mit++ )
		{
			vit2 = std::find_if( mit->second.begin(), mit->second.end(), MemberFileInfo::FIND_IF( wstrFileName ) ); // 구폴더 kom 압축파일내에 해당 파일명과
																													// 같은 파일을 찾는다.
			// 기존의 kom압축내에 있던 파일이면..
			if( vit2 != mit->second.end() )		// 찾았으면
			{
				mitNew = m_mapNewMemberFile.find( mit->first );	// new 멤버파일에 해당 kom 파일이 있는지 찾고 (이미 넣은 kom인지)
				if( mitNew == m_mapNewMemberFile.end() )		// 없으면
				{
					m_mapNewMemberFile.insert( std::make_pair( mit->first, std::vector< MemberFileInfo >() ) ); // 해당 kom 파일 명으로 insert함
					mitNew = m_mapNewMemberFile.find( mit->first );
				}

				MemberFileInfo memberFile;
				memberFile.m_wstrFileName = wstrFileName;
				memberFile.m_wstrRealPath = (*vit).c_str(); // 파일이 원래 있던 경로
				int iFileSize = GetFileSize( strFileNameForSize.c_str() );
				if( iFileSize == -1 )
					return false;

				memberFile.m_iFileSize	  = iFileSize / 1024 ; // byte -> kilobyte

				if( vit2->m_iFileSize == memberFile.m_iFileSize )	// old 파일 사이즈와 new 파일 사이즈를 비교해서 같으면 
				{
					memberFile.m_iFileState	  = FS_NONE;
				}
				else
				{
					memberFile.m_iFileState	  = FS_CHANGED;
				}

				mitNew->second.push_back( memberFile );
				bIsFindSucc = true;
				break;
			}
		}

		// 신규 파일이면..
		if( bIsFindSucc == false )
		{
			MemberFileInfo memberFile;
			memberFile.m_wstrFileName = wstrFileName;
			memberFile.m_iFileSize	  = GetFileSize( strFileNameForSize.c_str() ) / 1024; // byte -> kilobyte
			memberFile.m_iFileState	  = FS_INSERTED;
			memberFile.m_wstrRealPath = (*vit).c_str(); // 파일이 원래 있던 경로

			m_vecInsertedFile.push_back( memberFile );
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// verify deleted files		
	MemberFileIterator mitOld = m_mapOldMemberFile.begin();
	for( ; mitOld != m_mapOldMemberFile.end(); mitOld++ )
	{
		std::vector< MemberFileInfo >::iterator vit = mitOld->second.begin();
		for( ; vit != mitOld->second.end(); vit++ )
		{
			MemberFileInfo& memberFile = *vit;

			if( IsExistFileInNewMember( memberFile.m_wstrFileName ) == false )
			{
				memberFile.m_iFileState	   = FS_DELETED;
				memberFile.m_wstrKomString = mitOld->first;

				m_vecDeletedFile.push_back( memberFile );
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 컴파일된 Lua Script 파일들의 정보를 넣는다
	if( bIsLuaScriptComplile == true || bIsFileCrypt == true )
	{
		std::wstring wstrKomName = L"data0";

		std::map< std::wstring, MemberFileInfo >::const_iterator mit = mapLuaFiles.begin();
		for( ; mit != mapLuaFiles.end(); mit++ )
		{
			MemberFileIterator mitNew = m_mapNewMemberFile.find( wstrKomName );
			if( mitNew == m_mapNewMemberFile.end() )
			{
				m_mapNewMemberFile.insert( std::make_pair( wstrKomName, std::vector< MemberFileInfo >() ) );
				mitNew = m_mapNewMemberFile.find( wstrKomName );
			}

			mitNew->second.push_back( mit->second );
		}
	}
	
	return true;
}

bool CX2KomFileManager::ParsingMemberFile( IN std::wstring& wstrMemberFilePath, IN const std::map< std::wstring, MemberFileInfo >& mapLuaFiles, IN bool bIsLuaScriptComplile/* = false */, IN bool bIsFileCrypt /*= false*/ )
{
	m_bIsCompareKom = false;
	
	m_wstrNewPath = wstrMemberFilePath;

	WCHAR drive[_MAX_DRIVE]	= L"";
	WCHAR dir[_MAX_DIR]		= L"";
	WCHAR fname[_MAX_FNAME]	= L"";
	WCHAR ext[_MAX_EXT]		= L"";

	m_mapOldMemberFile.clear();
	m_mapNewMemberFile.clear();
	m_vecInsertedFile.clear();
	m_vecDeletedFile.clear();
	m_mapKomSize.clear();

	//////////////////////////////////////////////////////////////////////////
	// New Path -> naked member file load
	std::vector< std::wstring > vecMemberFileList;
	WCHAR wstrNewFolder[MAX_PATH];
	wcscpy( wstrNewFolder, wstrMemberFilePath.c_str() );

	FindFolderFile( wstrNewFolder, vecMemberFileList );
	if( vecMemberFileList.empty() == true )
		return false;

	//////////////////////////////////////////////////////////////////////////
	// verify naked member file list
	int iTotalSize = 0;
	int iNewIndex = 1;
	WCHAR wstrNewKom[256] = L"";
	std::vector< std::wstring >::iterator vit;

	for( vit = vecMemberFileList.begin(); vit != vecMemberFileList.end(); vit++ )
	{
		_wsplitpath( (*vit).c_str(), drive, dir, fname, ext);

		// 확장자 검사
		std::wstring wstrExt = ext;
		MakeUpperCase( wstrExt );

		if( wstrExt == L".KOM" )
		{
			::MessageBox( NULL, L"member파일중에 kom파일이 섞여있습니다!", L"에러!", MB_OK );
			return false;
		}
		
		// MP3 파일은 포함안함.
		if( wstrExt == L".MP3"  ||  wstrExt == L".X_OLD"  ||  wstrExt == L".Y_OLD" )
			continue;

		// File full name
		std::wstring wstrFileName;
		wstrFileName = fname;
		wstrFileName += ext;

		//////////////////////////////////////////////////////////////////////////
		// 중복 압축 방지
		bool bIsNoSame = false;
		MemberFileIterator mitNoSame = m_mapNewMemberFile.begin();
		std::vector< MemberFileInfo >::iterator vitNoSame;
		for( ; mitNoSame != m_mapNewMemberFile.end(); mitNoSame++ )
		{
			vitNoSame = std::find_if( mitNoSame->second.begin(), mitNoSame->second.end(), MemberFileInfo::FIND_IF( wstrFileName ) );
			if( vitNoSame != mitNoSame->second.end() )
			{
				bIsNoSame = true;
			}
		}
		if( bIsNoSame == true )
			continue;
		//////////////////////////////////////////////////////////////////////////

		// File full path
		std::string strFileNameForSize;
		ConvertWCHARToChar( strFileNameForSize, (*vit).c_str() );

		// reorganize member files
		MemberFileInfo memberFile;
		memberFile.m_wstrFileName = wstrFileName;
		memberFile.m_iFileState	  = FS_INSERTED;
		memberFile.m_wstrRealPath = (*vit).c_str(); // 파일이 원래 있던 경로
		int iFileSize = GetFileSize( strFileNameForSize.c_str() );
		if( iFileSize == -1 )
			return false;

		memberFile.m_iFileSize	  =  iFileSize / 1024; // byte -> kilobyte

		// 스크립트의 경우
		if( wstrExt == L".TXT"  ||  wstrExt == L".LUA" )
		{
			if( bIsLuaScriptComplile == true  &&  wstrExt == L".LUA" )
				continue;

			if( bIsFileCrypt == true )
				continue;

			wsprintf( wstrNewKom, L"data0" );

			// 평균값 안넘을시 kom넣기
			MemberFileIterator mit = m_mapNewMemberFile.find( std::wstring( wstrNewKom ) );
			if( mit == m_mapNewMemberFile.end() )
			{
				m_mapNewMemberFile.insert( std::make_pair( std::wstring( wstrNewKom ), std::vector< MemberFileInfo >() ) );
				mit = m_mapNewMemberFile.find( std::wstring( wstrNewKom ) );
			}
			mit->second.push_back( memberFile );
		}
		else
		{
			// member file 크기 검사
			iTotalSize += memberFile.m_iFileSize;
			if( iTotalSize > m_iKFS_AVERAGE_SIZE )
			{
				m_mapKomSize.insert( std::make_pair( std::wstring( wstrNewKom ), iTotalSize ) );

				// 조건값 초기화
				iTotalSize = 0;
				++iNewIndex;
			}

			wsprintf( wstrNewKom, L"data%d", iNewIndex );

			// 평균값 안넘을시 kom넣기
			MemberFileIterator mit = m_mapNewMemberFile.find( std::wstring( wstrNewKom ) );
			if( mit == m_mapNewMemberFile.end() )
			{
				m_mapNewMemberFile.insert( std::make_pair( std::wstring( wstrNewKom ), std::vector< MemberFileInfo >() ) );
				mit = m_mapNewMemberFile.find( std::wstring( wstrNewKom ) );
			}
			mit->second.push_back( memberFile );
		}		
	}

	m_mapKomSize.insert( std::make_pair( std::wstring( wstrNewKom ), iTotalSize ) );	// 위치가 여기 맞나?

	//////////////////////////////////////////////////////////////////////////
	// 컴파일된 Lua Script 파일들의 정보를 넣는다
	if( bIsLuaScriptComplile == true || bIsFileCrypt == true )
	{
		std::wstring wstrKomName = L"data0";

		std::map< std::wstring, MemberFileInfo >::const_iterator mit = mapLuaFiles.begin();
		for( ; mit != mapLuaFiles.end(); mit++ )
		{
			MemberFileIterator mitNew = m_mapNewMemberFile.find( wstrKomName );
			if( mitNew == m_mapNewMemberFile.end() )
			{
				m_mapNewMemberFile.insert( std::make_pair( wstrKomName, std::vector< MemberFileInfo >() ) );
				mitNew = m_mapNewMemberFile.find( wstrKomName );
			}

			mitNew->second.push_back( mit->second );
		}
	}

	return true;
}

bool CX2KomFileManager::ParsingKomFile( IN std::wstring& wstrKomFilePath )
{
	m_bIsCompareKom = false;

	m_wstrOldPath = wstrKomFilePath;

	WCHAR drive[_MAX_DRIVE]	= L"";
	WCHAR dir[_MAX_DIR]		= L"";
	WCHAR fname[_MAX_FNAME]	= L"";
	WCHAR ext[_MAX_EXT]		= L"";

	m_mapOldMemberFile.clear();
	m_mapKomRealPath.clear();

	//////////////////////////////////////////////////////////////////////////
	// Old Path -> kom file load
	std::vector< std::wstring > vecKomFileList;
	WCHAR wstrOldFolder[MAX_PATH];
	wcscpy( wstrOldFolder, wstrKomFilePath.c_str() );

	FindFolderFile( wstrOldFolder, vecKomFileList );
	if( vecKomFileList.empty() == true )
		return false;

	//////////////////////////////////////////////////////////////////////////	
	// verify kom file list
	int iTotalFileCount = 0;
	std::vector< std::wstring >::iterator vit;
	for( vit = vecKomFileList.begin(); vit != vecKomFileList.end(); vit++ )
	{
		_wsplitpath( (*vit).c_str(), drive, dir, fname, ext);

		// 확장자 검사
		std::wstring wstrExt = ext;
		MakeUpperCase( wstrExt );

		// kom파일이 아니면 continue
		if( wstrExt != L".KOM" )
			continue;

		// kom real path insert
		m_mapKomRealPath.insert( std::make_pair( std::wstring( fname ), *vit ) );

		// kom file load
		CMassFile massFile;
		CHAR strTemp[256] = "";
		WideCharToMultiByte( CP_ACP, 0, (*vit).c_str(), -1, strTemp, MAX_PATH, NULL, NULL );

		massFile.LoadMassFile( strTemp ); // mass file load

		CMassFile::MASSFILE_MEMBERFILEINFO Info;
		ZeroMemory( &Info, sizeof( Info ) );
		WCHAR wstrTemp[256] = L"";

		for( UINT i = 0; i < massFile.GetFileCount(); ++i )
		{
			massFile.GetMemberFile(i,&Info);

			MultiByteToWideChar( CP_ACP, 0, Info.strFileName, -1, wstrTemp, MAX_PATH );

			// kom 등록 확인
			MemberFileIterator mit;
			mit = m_mapOldMemberFile.find( std::wstring( fname ) );
			if( mit == m_mapOldMemberFile.end() )
			{
				m_mapOldMemberFile.insert( std::make_pair( std::wstring( fname ), std::vector< MemberFileInfo >() ) );
				mit = m_mapOldMemberFile.find( std::wstring( fname ) );
			}

			// 멤버 파일 저장
			MemberFileInfo memberFile;
			memberFile.m_wstrFileName = wstrTemp;
			memberFile.m_iFileState	  = FS_NONE;
			memberFile.m_iFileSize	  = Info.size / 1024; // byte -> kilobyte

			mit->second.push_back( memberFile );

			// for log
			++iTotalFileCount;
		}
	}

	return true;
}

bool CX2KomFileManager::ReorganizeMemberFile()
{
	if( m_vecInsertedFile.empty() == true )
		return true;

	//////////////////////////////////////////////////////////////////////////
	// new kom file member's size checking
	std::vector< std::pair< std::wstring, int > > vecReposTargetMin;	// key: kom 파일명, T: kom 안의 TotalSize
	std::vector< std::pair< std::wstring, int > > vecReposTargetMax;
	MemberFileIterator mitSizeCheck = m_mapNewMemberFile.begin();
	for( ; mitSizeCheck != m_mapNewMemberFile.end(); mitSizeCheck++ )
	{
		// script kom file의 경우 처리안함.
		if( mitSizeCheck->first == std::wstring( L"data0" ) )
			continue;

		int iTotalSize = 0;

		std::vector< MemberFileInfo >::iterator vit = mitSizeCheck->second.begin();
		for( ; vit != mitSizeCheck->second.end(); vit++ )
		{
			MemberFileInfo& memberFile = *vit;		// kom 파일 안의 파일
			iTotalSize += memberFile.m_iFileSize;	// 사이즈 들
		}

		if( iTotalSize < m_iKFS_MIN_SIZE )			// 같을 경우는 별로 없지만 같으면??
		{
			vecReposTargetMin.push_back( std::make_pair( mitSizeCheck->first, iTotalSize ) );
		}
		else if( iTotalSize > m_iKFS_MAX_SIZE )
		{
			vecReposTargetMax.push_back( std::make_pair( mitSizeCheck->first, iTotalSize ) );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// reorganize max values	
	std::vector< std::pair< std::wstring, int > >::iterator vitMax = vecReposTargetMax.begin();
	for( ; vitMax != vecReposTargetMax.end(); vitMax++ )
	{
		MemberFileIterator mit = m_mapNewMemberFile.find( vitMax->first );	// kom 파일을 찾음
		std::vector< MemberFileInfo >& vecMemberFileInfo = mit->second;		// 해당 kom에 있는 파일들

		while( vitMax->second - vecMemberFileInfo.back().m_iFileSize > m_iKFS_AVERAGE_SIZE ) // 기준 사이즈(now:30MB) 보다 작아질때까지
		{
			vitMax->second -= vecMemberFileInfo.back().m_iFileSize;			// 뒤에것을 빼서
			m_vecInsertedFile.push_back( vecMemberFileInfo.back() );		// insertedFile 뒤에다가 넣음
			vecMemberFileInfo.pop_back();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// reorganize min values
	std::vector< MemberFileInfo > vecInsertedFileClone = m_vecInsertedFile;
	if( vecInsertedFileClone.empty() == false )			// 추가파일(insertedFile)이 있으면
	{		
		std::vector< std::pair< std::wstring, int > >::iterator vitMin = vecReposTargetMin.begin();		// key: kom 파일명, T: size
		for( ; vitMin != vecReposTargetMin.end(); vitMin++ )
		{
			MemberFileIterator mit = m_mapNewMemberFile.find( vitMin->first );			// 크기가 작은 해당 kom파일을 찾는다

			do
			{
				if( vecInsertedFileClone.empty() == true )
					break;

				MemberFileInfo temp = vecInsertedFileClone.back();		// 추가파일(insertedFile)을 뒤에서 부터 빼서
				vecInsertedFileClone.pop_back();

				mit->second.push_back( temp );							// 크기가 작은 kom 파일뒤에 붙임
				vitMin->second += temp.m_iFileSize;

			} while( vitMin->second < m_iKFS_AVERAGE_SIZE );			// kom 파일의 사이즈가 기준 사이즈(now: 30MB) 보다 같거나 커질때 까지
		}
	}	

	//////////////////////////////////////////////////////////////////////////
	// reorganize Inserted File
	if( vecInsertedFileClone.empty() == false )
	{
		int iTotalSize = 0;
		int iNewIndex = m_mapNewMemberFile.size(); // 다음 인덱스
		WCHAR wstrNewKom[256] = L"";

		std::vector< MemberFileInfo >::iterator vitRepos = vecInsertedFileClone.begin();
		for( ; vitRepos != vecInsertedFileClone.end(); vitRepos++ )
		{
			iTotalSize += vitRepos->m_iFileSize;

			if( iTotalSize > m_iKFS_AVERAGE_SIZE )
			{
				// 조건값 초기화
				iTotalSize = 0;
				++iNewIndex;
			}

			wsprintf( wstrNewKom, L"data%d", iNewIndex );

			// 평균값 안넘을시 kom넣기
			MemberFileIterator mit = m_mapNewMemberFile.find( std::wstring( wstrNewKom ) );
			if( mit == m_mapNewMemberFile.end() )
			{
				m_mapNewMemberFile.insert( std::make_pair( std::wstring( wstrNewKom ), std::vector< MemberFileInfo >() ) );
				mit = m_mapNewMemberFile.find( std::wstring( wstrNewKom ) );
			}
			mit->second.push_back( *vitRepos );
		}
	}

	return true;
}

bool CX2KomFileManager::CreateKomFile( IN std::wstring& wstrSavePath, IN std::map< std::wstring, std::vector< std::wstring > >& mapPackingFiles )
{
	// MassFile에 만들 kom file member 저장
	std::string strKomName;
	std::string strCreatePath;
	CMassFile massFile;
	std::vector< char* > vecKomMemeber;

	ConvertWCHARToChar( strCreatePath, wstrSavePath.c_str() );

	std::map< std::wstring, std::vector< std::wstring > >::iterator mit = mapPackingFiles.begin();
	for( ; mit != mapPackingFiles.end(); mit++ )
	{
		std::vector< std::wstring >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end(); vit++ )
		{
			// 문자열 타입 변환
			char* strTemp = new char[MAX_PATH];
			WideCharToMultiByte( CP_ACP, 0, vit->c_str(), -1, strTemp, MAX_PATH, NULL, NULL );
			vecKomMemeber.push_back( strTemp );
		}

		ConvertWCHARToChar( strKomName, mit->first.c_str() );

		std::string strCreatePathTemp = strCreatePath;
		strCreatePathTemp += "\\";
		strCreatePathTemp += strKomName;
		strCreatePathTemp += ".kom";

		if( massFile.ModifyMassFile( &massFile, strCreatePathTemp.c_str(), &vecKomMemeber ) == false )
			return false;

		std::vector< char* >::iterator vitc = vecKomMemeber.begin();
		for( ; vitc != vecKomMemeber.end(); vitc++ )
			delete [](*vitc);
		vecKomMemeber.clear();
	}

	return true;
}

bool CX2KomFileManager::CreateKomFile( IN std::wstring& wstrSavePath, IN bool bIsMultiThreadMode /*= false */ )
{
	if( bIsMultiThreadMode )
	{
		return CreateKomFileMultiThread( wstrSavePath );
	}

	// MassFile에 만들 kom file member 저장
	std::string strKomName;
	std::string strCreatePath;
	CMassFile massFile;
	std::vector< char* > vecKomMemeber;

	ConvertWCHARToChar( strCreatePath, wstrSavePath.c_str() );
	
	MemberFileIterator mit = m_mapNewMemberFile.begin();
	for( ; mit != m_mapNewMemberFile.end(); mit++ )
	{
		std::vector< MemberFileInfo >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end(); vit++ )
		{
			MemberFileInfo& memberFile = *vit;

			// 문자열 타입 변환
			char* strTemp = new char[MAX_PATH];
			WideCharToMultiByte( CP_ACP, 0, memberFile.m_wstrRealPath.c_str(), -1, strTemp, MAX_PATH, NULL, NULL );
			vecKomMemeber.push_back( strTemp );
		}

		ConvertWCHARToChar( strKomName, mit->first.c_str() );

		std::string strCreatePathTemp = strCreatePath;
		strCreatePathTemp += "\\";
		strCreatePathTemp += strKomName;
		strCreatePathTemp += ".kom";

		if( massFile.ModifyMassFile( &massFile, strCreatePathTemp.c_str(), &vecKomMemeber ) == false )
			return false;

		std::vector< char* >::iterator vitc = vecKomMemeber.begin();
		for( ; vitc != vecKomMemeber.end(); vitc++ )
			delete [](*vitc);
		vecKomMemeber.clear();
	}

	return true;
}

DWORD WINAPI KomPackingThread( void* pVoid )
{
	CX2KomFileManager* pThisPtr = reinterpret_cast<CX2KomFileManager*>( pVoid );	

	// MassFile에 만들 kom file member 저장
	std::string strKomName;
	std::string strCreatePath;
	CMassFile massFile;
	std::vector< char* > vecKomMemeber;

	ConvertWCHARToChar( strCreatePath, pThisPtr->GetSavePathForMultiThreading().c_str() );

	std::wstring wstrKomName;
	std::vector< MemberFileInfo > vecMemberFiles;

	while( pThisPtr->GetPackingDataForMultiThreading( wstrKomName, vecMemberFiles ) )
	{
		std::vector< MemberFileInfo >::iterator vit = vecMemberFiles.begin();
		for( ; vit != vecMemberFiles.end(); vit++ )
		{
			MemberFileInfo& memberFile = *vit;

			// 문자열 타입 변환
			char* strTemp = new char[MAX_PATH];
			WideCharToMultiByte( CP_ACP, 0, memberFile.m_wstrRealPath.c_str(), -1, strTemp, MAX_PATH, NULL, NULL );
			vecKomMemeber.push_back( strTemp );
		}

		ConvertWCHARToChar( strKomName, wstrKomName.c_str() );

		std::string strCreatePathTemp = strCreatePath;
		strCreatePathTemp += "\\";
		strCreatePathTemp += strKomName;
		strCreatePathTemp += ".kom";

		if( massFile.ModifyMassFile( &massFile, strCreatePathTemp.c_str(), &vecKomMemeber ) == false )
			return false;

		std::vector< char* >::iterator vitc = vecKomMemeber.begin();
		for( ; vitc != vecKomMemeber.end(); vitc++ )
			delete [](*vitc);
		vecKomMemeber.clear();
	}

	return 0;
}

bool CX2KomFileManager::CreateKomFileMultiThread( IN std::wstring& wstrSavePath )
{
	// 초기화 및 데이터 백업
	m_mapNewMemberForMT.clear();
	m_mapNewMemberForMT = m_mapNewMemberFile;
	m_wstrSavePathForMT = wstrSavePath;

	HANDLE* pArrTH = new HANDLE[m_iThreadCount];
    for( int idx=0; idx < m_iThreadCount; idx++ )
	{
		pArrTH[idx] = CreateThread( NULL, 0, &KomPackingThread, reinterpret_cast<void*>(this), 0, NULL );
	}

	WaitForMultipleObjects( m_iThreadCount, pArrTH, TRUE, INFINITE );

	delete[] pArrTH;
	return true;
}

bool CX2KomFileManager::GetPackingDataForMultiThreading( OUT std::wstring& wstrKomName, OUT std::vector< MemberFileInfo >& vecMemberFiles )
{
	wstrKomName.clear();
	vecMemberFiles.clear();
	bool bResult;

	EnterCriticalSection( &m_csNewMemberForMT );
	///////////////////////////////////////////////////////// CRITICAL_SECTION

	if( m_mapNewMemberForMT.empty() )
	{
		bResult = false;
	}
	else
	{
		MemberFileIterator mit = m_mapNewMemberForMT.begin();

		wstrKomName	   = mit->first;
		vecMemberFiles = mit->second;

		m_mapNewMemberForMT.erase( mit );
		bResult = true;
	}

	//////////////////////////////////////////////////////////////////////////	
	LeaveCriticalSection( &m_csNewMemberForMT );

	return bResult;
}

bool CX2KomFileManager::Convert_X_File( IN std::wstring& wstrNewPath, IN bool bIsRestore /*= false*/ )
{    	
	if( bIsRestore == false )
	{
		_wspawnlp( _P_WAIT, m_wstrXConvertCommand.c_str(), L"kogskinmesh.exe", wstrNewPath.c_str(), NULL );
	}
	else
	{
		_wspawnlp( _P_WAIT, m_wstrXConvertCommand.c_str(), L"kogskinmesh.exe", wstrNewPath.c_str(), L"/r", NULL );
	}

	return true;
}

bool CX2KomFileManager::Convert_X_File( IN std::vector< std::wstring >& vecNewFolderPaths, IN bool bIsRestore /*= false*/ )
{
	std::vector< std::wstring >::iterator vItr = vecNewFolderPaths.begin();

	while ( vItr != vecNewFolderPaths.end() )
	{
		if( bIsRestore == false )
		{
			_wspawnlp( _P_WAIT, m_wstrXConvertCommand.c_str(), L"kogskinmesh.exe", (*vItr).c_str(), NULL );
		}
		else
		{
			_wspawnlp( _P_WAIT, m_wstrXConvertCommand.c_str(), L"kogskinmesh.exe", (*vItr).c_str(), L"/r", NULL );
		}
	}

	return true;
}

void CX2KomFileManager::FindFolderFile( OUT WCHAR* wstrFolder, OUT std::vector< std::wstring >& vecFileList )
{
	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;
	WCHAR				wstrSearchPath[256] = L"";

	wcscpy( wstrSearchPath, wstrFolder );
	wcscat( wstrSearchPath, L"\\*.*" );

	hSearch = FindFirstFile( wstrSearchPath, &fd );

	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if( wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") && wcscmp(fd.cFileName, L".svn") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				WCHAR	wstrNewSearchDir[256];

				wcscpy( wstrNewSearchDir, wstrFolder );
				wcscat( wstrNewSearchDir, L"\\");
				wcscat( wstrNewSearchDir, fd.cFileName );

				FindFolderFile( wstrNewSearchDir, vecFileList );
			}
			else
			{
				WCHAR			wstrNewSearchFile[256];
				std::wstring	wstrFile;

				wcscpy( wstrNewSearchFile, wstrFolder );
				wcscat( wstrNewSearchFile, L"\\");
				wcscat( wstrNewSearchFile, fd.cFileName );

				wstrFile = wstrNewSearchFile;

				vecFileList.push_back( wstrFile );
			}
		}

	}while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
}

bool CX2KomFileManager::MemberFileFind( IN std::wstring& wstrMemberFileName, IN bool bIsOld, OUT std::wstring& wstrKomString, OUT int& iFindIndex )
{
	if( bIsOld )
	{
		MemberFileIterator mit = m_mapOldMemberFile.begin();
		for( ; mit != m_mapOldMemberFile.end(); ++mit )
		{
			iFindIndex = 0;

			std::vector< MemberFileInfo >::const_iterator vit;
			for( vit = mit->second.begin(); vit != mit->second.end(); ++vit, ++iFindIndex )
			{
				const MemberFileInfo& memberFileInfo = *vit;
				if( memberFileInfo.m_wstrFileName == wstrMemberFileName )
				{
					wstrKomString = mit->first;
					return true;
				}				
			}
		}
	}
	else
	{
		MemberFileIterator mit = m_mapNewMemberFile.begin();
		for( ; mit != m_mapNewMemberFile.end(); ++mit )
		{
			iFindIndex = 0;

			std::vector< MemberFileInfo >::const_iterator vit;
			for( vit = mit->second.begin(); vit != mit->second.end(); ++vit, ++iFindIndex )
			{
				const MemberFileInfo& memberFileInfo = *vit;
				if( memberFileInfo.m_wstrFileName == wstrMemberFileName )
				{
					wstrKomString = mit->first;
					return true;
				}				
			}
		}		
	}
	
	return false;
}

bool CX2KomFileManager::IsExistFileInNewMember( IN std::wstring& wstrFileName ) // 신폴더에 구폴더의 파일이 존재 하는가?
{
	MemberFileIterator mitNew = m_mapNewMemberFile.begin();
	for( ; mitNew != m_mapNewMemberFile.end(); mitNew++ )
	{
		std::vector< MemberFileInfo >::iterator vit;
		vit = std::find_if( mitNew->second.begin(), mitNew->second.end(), MemberFileInfo::FIND_IF( wstrFileName ) );
		if( vit != mitNew->second.end() )
		{
			return true;		// 존재 하면
		}
	}
	return false;
}

long CX2KomFileManager::GetFileSize(const char* strFileName)
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

int CX2KomFileManager::GetKomSize( std::wstring& komName )
{
	std::map< std::wstring, int >::iterator mit = m_mapKomSize.find( komName );
	if( mit == m_mapKomSize.end() )
		return 0;

	return mit->second;
}

bool CX2KomFileManager::UncompressKomFile( IN std::wstring& wstrUncompressPath, 
										   IN std::vector< std::wstring >& vecSeletedFileList, 
										   IN std::wstring& wstrCurKomName, 
										   IN UNCOMPRESS_MODE eMode,
										   IN bool bResultFullPath, 
										   OUT std::map< std::wstring, std::vector< std::wstring > >& mapResultFullPath )
{
	if( UM_SEL_FILE == eMode )
	{
		if( vecSeletedFileList.empty() )
			return false;

		std::map< std::wstring, std::wstring >::const_iterator mitPath;
		mitPath = m_mapKomRealPath.find( wstrCurKomName );
		if( mitPath == m_mapKomRealPath.end() )
			return false;

		// kom file real path
		std::wstring wstrKomFilePath = mitPath->second;

		//CMassFile을 이용해 파일을 읽어냄
		char  strTemp[MAX_PATH] = "";
		TCHAR tstrTemp[MAX_PATH] = _T("");

		WideCharToMultiByte( CP_ACP, 0, wstrKomFilePath.c_str(), -1, strTemp, MAX_PATH, NULL, NULL );

		CMassFile massFile;
		massFile.LoadMassFile( strTemp );
		UINT FileNo = massFile.GetFileCount();
		CMassFile::MASSFILE_MEMBERFILEINFO FileInfo;
		memset( &FileInfo, 0, sizeof( FileInfo ) );
		FileInfo.dwFlag = MFI_REALDATA;

		CString OutFileName;

		std::vector< std::wstring >::const_iterator vit;
		for( vit = vecSeletedFileList.begin(); vit != vecSeletedFileList.end(); ++vit )
		{
			WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)(*vit).c_str(), -1, strTemp, MAX_PATH, NULL, NULL );
			massFile.GetMemberFile( strTemp, &FileInfo );

			MultiByteToWideChar( CP_ACP, 0, FileInfo.strFileName, -1, tstrTemp, MAX_PATH );
			OutFileName.Format( _T("%s\\%s"), wstrUncompressPath.c_str(), tstrTemp );

			WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)OutFileName, -1, strTemp, MAX_PATH, NULL, NULL );
			FILE* file = fopen( strTemp, "wb" );

			if( file == NULL )
				return false;
			
			fwrite( FileInfo.pRealData, 1, FileInfo.size, file );
			fclose( file );

			FileInfo.pRealData = NULL;			
		}
	}
	else
	{
		MemberFileIterator mit;
		for( mit = m_mapOldMemberFile.begin(); mit != m_mapOldMemberFile.end(); ++mit )
		{
			std::map< std::wstring, std::wstring >::const_iterator mitPath;
			mitPath = m_mapKomRealPath.find( mit->first );
			if( mitPath == m_mapKomRealPath.end() )
				return false;

			// kom file real path
			std::wstring wstrKomFilePath = mitPath->second;

			// target folder
			std::wstring wstrUncompressFolder = wstrUncompressPath;

			switch( eMode )
			{
			case UM_ALL_KOM:
				{
					wstrUncompressFolder += L"\\";
					wstrUncompressFolder += mit->first;
					CreateDirectory( wstrUncompressFolder.c_str(), NULL );
				}
				break;

			case UM_SEL_KOM:
				{
					std::vector< std::wstring >::const_iterator vitKom;
					vitKom = std::find( vecSeletedFileList.begin(), vecSeletedFileList.end(), mit->first );
					if( vitKom == vecSeletedFileList.end() )
						continue;

					//wstrUncompressFolder += L"\\";
					wstrUncompressFolder += mit->first;
					CreateDirectory( wstrUncompressFolder.c_str(), NULL );
				}
				break;

			default:
				break;
			}

			//CMassFile을 이용해 파일을 읽어냄
			char  strTemp[MAX_PATH] = "";
			TCHAR tstrTemp[MAX_PATH] = _T("");

			WideCharToMultiByte( CP_ACP, 0, wstrKomFilePath.c_str(), -1, strTemp, MAX_PATH, NULL, NULL );

			CMassFile massFile;
			massFile.LoadMassFile( strTemp );
			UINT FileNo = massFile.GetFileCount();
			CMassFile::MASSFILE_MEMBERFILEINFO FileInfo;
			memset( &FileInfo, 0, sizeof( FileInfo ) );
			FileInfo.dwFlag = MFI_REALDATA;

			CString OutFileName;

			for(int i = 0; i < (int)FileNo; i++)
			{
				massFile.GetMemberFile( i, &FileInfo );

				MultiByteToWideChar( CP_ACP, 0, FileInfo.strFileName, -1, tstrTemp, MAX_PATH );
				OutFileName.Format( _T("%s\\%s"), wstrUncompressFolder.c_str(), tstrTemp );

				WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)OutFileName, -1, strTemp, MAX_PATH, NULL, NULL );
				FILE* file = fopen( strTemp, "wb" );

				if( file == NULL )
					return false;

				fwrite( FileInfo.pRealData, 1, FileInfo.size, file );
				fclose( file );

				FileInfo.pRealData = NULL;

				//////////////////////////////////////////////////////////////////////////				
				// 압축이 풀린 파일의 경로저장
				if( bResultFullPath )
				{					
					std::map< std::wstring, std::vector< std::wstring > >::iterator mitFullPath;
					mitFullPath = mapResultFullPath.find( mit->first );
					if( mitFullPath == mapResultFullPath.end() )
					{
						std::vector< std::wstring > vecTemp;
						vecTemp.push_back( std::wstring( OutFileName.GetBuffer() ) );
						mapResultFullPath.insert( std::make_pair( mit->first, vecTemp ) );
					}
					else
					{
						mitFullPath->second.push_back( std::wstring( OutFileName.GetBuffer() ) );
					}
				}
				//////////////////////////////////////////////////////////////////////////				
			}
		}
	}

	return true;
}

void CX2KomFileManager::LoadConfig()
{
	//lua_tinker::class_<CX2KomFileManager>("CX2KomFileManager")
	//	.def("InitKomFileViewerOption",		CX2KomFileManager::InitKomFileViewerOption_Lua)
	//	.def("AddEncryptionKey",			CX2KomFileManager::AddEncryptionKey_Lua)
	//	.def("SetThreadCount",				CX2KomFileManager::SetThreadCount_Lua)
	//	;

	//lua_tinker::decl( "CX2KomFileManager", this );

	lua_tinker::class_add<CX2KomFileManager>( g_pLua, "CX2KomFileManager" );
	lua_tinker::class_def<CX2KomFileManager>( g_pLua, "InitKomFileViewerOption",	&CX2KomFileManager::InitKomFileViewerOption_Lua );
	lua_tinker::class_def<CX2KomFileManager>( g_pLua, "AddEncryptionKey",			&CX2KomFileManager::AddEncryptionKey_Lua );
	lua_tinker::class_def<CX2KomFileManager>( g_pLua, "SetThreadCount",				&CX2KomFileManager::SetThreadCount_Lua );
	lua_tinker::class_def<CX2KomFileManager>( g_pLua, "SetPathsForCommand",			&CX2KomFileManager::SetPathsForCommand_Lua );	

#ifdef _MODIFY_VERSION_01
	lua_tinker::class_def<CX2KomFileManager>( g_pLua, "InitNewFolderPaths",			&CX2KomFileManager::InitNewFolderPaths_Lua );
#endif

	lua_tinker::decl( g_pLua, "CX2KomFileManager", this );

	char acCurPath[_MAX_PATH] = {0};
	GetCurrentDirectoryA(_MAX_PATH, acCurPath);
	std::string strPath = acCurPath;
	strPath += "\\";
	strPath += "Config.lua";

	if( 0 != luaL_dofile( g_pLua, strPath.c_str() ) )
	{
		::MessageBox( NULL, L"Config.lua open failed!", L"", MB_OK );
	}
//*
#ifdef _MODIFY_VERSION_01
	strPath = acCurPath;
	strPath += "\\";
	strPath += "InitNewFolderPaths.lua";

	if ( luaL_dofile( g_pLua, strPath.c_str() ) != 0 )		// 실행되지 않으면
	{
		::MessageBox( NULL, L"InitNewFolderPaths.lua open failed!", L"", MB_OK );
	}
#endif	
//*/
}

bool CX2KomFileManager::InitKomFileViewerOption_Lua()
{
	KLuaManager luaManager( g_pLua );

	int KFS_MIN_SIZE;
	int KFS_MAX_SIZE;

	LUA_GET_VALUE_RETURN(	luaManager,	"m_iKFS_MIN_SIZE",		KFS_MIN_SIZE,		0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iKFS_MAX_SIZE",		KFS_MAX_SIZE,		0,	goto LoadFail; );

	m_iKFS_MIN_SIZE *= KFS_MIN_SIZE;
	m_iKFS_MAX_SIZE *= KFS_MAX_SIZE;

	m_iKFS_AVERAGE_SIZE = ( m_iKFS_MIN_SIZE + m_iKFS_MAX_SIZE ) / 2;

	return true;

LoadFail:
	::MessageBox( NULL, L"InitKomFileViewerOption 파싱 실패", L"", MB_OK );

	return false;
}

bool CX2KomFileManager::AddEncryptionKey_Lua()
{
	KLuaManager luaManager( g_pLua );

	int iEncryptionKey;

	LUA_GET_VALUE_RETURN(	luaManager,	"m_iEncryptionKey",		iEncryptionKey,		0,	goto LoadFail; );

	m_vecEncryptionKey.push_back( iEncryptionKey );

	return true;

LoadFail:
	::MessageBox( NULL, L"AddEncryptionKey 파싱 실패", L"", MB_OK );

	return false;
}

bool CX2KomFileManager::SetPathsForCommand_Lua()
{
	KLuaManager luaManager( g_pLua );

	if ( luaManager.BeginTable( "m_wstrDepotPath" ) == false )
	{
		::MessageBox( NULL, L"m_wstrDepotPath를 Lua Stack으로 가져오지 못했습니다.", L"", MB_OK );

		return false;
	}

	luaManager.GetValue( 1, m_wstrDepotPath );
	luaManager.EndTable();

	if ( luaManager.BeginTable( "m_wstrLuacCommand" ) == false )
	{
		::MessageBox( NULL, L"m_wstrLuacCommand를 Lua Stack으로 가져오지 못했습니다.", L"", MB_OK );

		return false;
	}

	luaManager.GetValue( 1, m_wstrLuacCommand );
	luaManager.EndTable();


    if ( luaManager.BeginTable( "m_wstrAnsiToUTF8Command" ) == false )
    {
        ::MessageBox( NULL, L"m_wstrAnsiToUTF8Command를 Lua Stack으로 가져오지 못했습니다.", L"", MB_OK );

        return false;
    }

    luaManager.GetValue( 1, m_wstrAnsiToUTF8Command );
    luaManager.EndTable();




	if ( luaManager.BeginTable( "m_wstrXConvertCommand" ) == false )
	{
		::MessageBox( NULL, L"m_wstrXConvertCommand를 Lua Stack으로 가져오지 못했습니다.", L"", MB_OK );

		return false;
	}

	luaManager.GetValue( 1, m_wstrXConvertCommand );
	luaManager.EndTable();

	return true;	
}


#define ERR_MessageBox( msg ) MessageBox( NULL, msg, L"에러!", MB_OK );

bool CX2KomFileManager::RepackagingKomFile( IN std::wstring& wstrKomSavePath, IN std::map< std::wstring, ModifyData >& mapModifyData, IN bool bLuaCompile /*= false*/, IN bool bLuaEncrypt /*= false*/ )
{
	std::vector< std::wstring > vecSeletedFileList;							 // 압축을 풀 kom파일 리스트
	std::map< std::wstring, std::vector< std::wstring > > mapResultFullPath; // 압축풀린 파일들의 fullpath
	std::map< std::wstring, ModifyData >::const_iterator mit;
	std::vector< std::wstring > vecModifyScriptFiles;						 // 스크립트 리스트
	bool bSuccess = true;

	// 1. 임시 폴더 생성	
	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";
	_wsplitpath( wstrKomSavePath.c_str(), drive, dir, fname, ext );

	std::wstring wstrUncompressPath = drive;
	wstrUncompressPath += L"\\RepackagingTemp\\";
	
	CreateDirectory( wstrUncompressPath.c_str(), NULL );

	// 2. kom파일 압축풀기
	for( mit = mapModifyData.begin(); mit != mapModifyData.end(); ++mit )
	{
		if( vecSeletedFileList.end() != std::find( vecSeletedFileList.begin(), vecSeletedFileList.end(), mit->second.m_wstrKomString ) )
			continue;

		vecSeletedFileList.push_back( mit->second.m_wstrKomString );
	}
	 
	if( UncompressKomFile( wstrUncompressPath, vecSeletedFileList, std::wstring( L"dummy" ), UM_SEL_KOM, true, mapResultFullPath ) == false )
	{
		ERR_MessageBox( L"kom파일 압축해제 실패!" );
		bSuccess = false;
		goto end_proc;
	}

	// 3. 해당 스크립트 컴파일
	if( bLuaCompile )
	{
		for( mit = mapModifyData.begin(); mit != mapModifyData.end(); ++mit )
		{
			_wsplitpath( mit->second.m_wstrFileFullPath.c_str(), NULL, NULL, NULL, ext );
			MakeUpperCase( ext );
			if( wcscmp( ext, L".LUA" ) == 0 )
			{
				std::wstring wstrResultPath = wstrUncompressPath;
				wstrResultPath += mit->second.m_wstrKomString;

				if( !CLuaCompileUtil::Compile( mit->second.m_wstrFileFullPath, wstrResultPath, m_wstrLuacCommand, m_wstrAnsiToUTF8Command ) )
				{
					std::wstring wstrErr = L"Lua 스크립트 컴파일 실패!   :   ";
					wstrErr += mit->second.m_wstrFileFullPath;
					ERR_MessageBox( wstrErr.c_str() );
					bSuccess = false;
					goto end_proc;
				}
			}
		}
	}

	// 4. 해당 스크립트 및 리소스 교체	
	for( mit = mapModifyData.begin(); mit != mapModifyData.end(); ++mit )
	{
		_wsplitpath( mit->second.m_wstrFileFullPath.c_str(), drive, dir, fname, ext );

		std::wstring wstrCopyPath = wstrUncompressPath;
		wstrCopyPath += mit->second.m_wstrKomString;
		wstrCopyPath += L"\\";
		wstrCopyPath += fname;
		wstrCopyPath += ext;

		// 스크립트라면 리스트에 저장
		//if( mit->second.m_wstrKomString == L"data0" )
		//{
		MakeUpperCase( ext );
		if ( (wcscmp( ext, L".TXT" ) == 0) ||  (wcscmp( ext, L".LUA" ) == 0) )
		{
			vecModifyScriptFiles.push_back( wstrCopyPath );
		}
		//}

		std::map< std::wstring, std::vector< std::wstring > >::iterator mItrResultFullPath =
			mapResultFullPath.find( mit->second.m_wstrKomString );

		if ( std::find( mItrResultFullPath->second.begin(), mItrResultFullPath->second.end(), wstrCopyPath ) == mItrResultFullPath->second.end() )
			mItrResultFullPath->second.push_back( wstrCopyPath );
	

		// 컴파일 옵션이라면 continue
		if( bLuaCompile )
		{
			MakeUpperCase( ext );
			if( wcscmp( ext, L".LUA" ) == 0 )
				continue;
		}

		// 파일 삭제를 위해서 파일 속성 변경
		_wchmod( wstrCopyPath.c_str(), _S_IREAD | _S_IWRITE );

		// 세번째 인자가 false면 기존파일을 덮어쓴다.
		if( CopyFileW( mit->second.m_wstrFileFullPath.c_str(), wstrCopyPath.c_str(), FALSE ) == FALSE )
		{
			std::wstring wstrErr = L"파일 교체 실패!   :  ";
			wstrErr += mit->second.m_wstrFileFullPath;
			wstrErr += L" ---> ";
			wstrErr += wstrCopyPath;
            ERR_MessageBox( wstrErr.c_str() );
			bSuccess = false;
			goto end_proc;
		}
	}

	// 5. 해당 스크립트 암호화
	if( bLuaEncrypt )
	{
		std::vector< std::wstring >::iterator vit = vecModifyScriptFiles.begin();
		for( ; vit != vecModifyScriptFiles.end(); ++vit )
		{
			std::string strEncryptFilePath;
			ConvertWCHARToChar( strEncryptFilePath, vit->c_str() );
			if( !FileCrypt::FileEncrypt( m_vecEncryptionKey, strEncryptFilePath ) )
			{
				std::wstring wstrErr = L"스크립트 암호화 실패!   :   ";
				wstrErr += *vit;
				ERR_MessageBox( wstrErr.c_str() );
				bSuccess = false;
				goto end_proc;
			}
		}
	}

	// 6. kom파일 재압축
	if( !CreateKomFile( wstrKomSavePath, mapResultFullPath ) )
	{
		ERR_MessageBox( L"kom파일 재압축 실패!" );
		bSuccess = false;
		goto end_proc;
	}

	// 7. 임시 파일 및 폴더 삭제
end_proc:
	std::map< std::wstring, std::vector< std::wstring > >::iterator mitDel = mapResultFullPath.begin();
	for( ; mitDel != mapResultFullPath.end(); ++mitDel )
	{
		std::vector< std::wstring >::iterator vitDel = mitDel->second.begin();
		for( ; vitDel != mitDel->second.end(); ++vitDel )
		{
			// 파일 삭제를 위해서 파일 속성 변경
			_wchmod( vitDel->c_str(), _S_IREAD | _S_IWRITE );
			DeleteFile( vitDel->c_str() );
		}
		std::wstring wstrTemp = wstrUncompressPath;
		wstrTemp += mitDel->first;
		RemoveDirectory( wstrTemp.c_str() );
	}
	RemoveDirectory( wstrUncompressPath.c_str() );

	return bSuccess;
}


BOOL CX2KomFileManager::GetKomFileListFromLuaScript()			// 루아 스크립트로 부터 기존의 kom 파일의 경로들을 받아오기(구버전 폴더만 열었을때)
{
	m_mapOldMemberFile.clear();
	m_mapNewMemberFile.clear();
	m_vecInsertedFile.clear();
	m_vecDeletedFile.clear();
	m_mapKomSize.clear();
	////////////////////////////////////////////////////////////////////////////////////////////

	m_bIsCompareKom = false;
	//m_bIsParsingOnly = true; Dlg에서 처리

	//CHAR strTemp[MAX_PATH];
	WCHAR wstrOldFolder[MAX_PATH];
	WCHAR folderName[_MAX_FNAME] = L"";
	std::vector< std::wstring >::iterator vit = m_vecKomFileList.begin();

	for ( ; vit != m_vecKomFileList.end(); vit++ )
	{		
		StringCchCopyW( wstrOldFolder, MAX_PATH, vit->c_str() );
		_wsplitpath( vit->c_str(), 0, 0, folderName, 0);
		FindFolderKomFile( wstrOldFolder );//, vecKomFileList );

	}
	
	return TRUE;
	
}

BOOL CX2KomFileManager::GetFolderToCompressListFromLuaScript( std::vector< std::wstring >& vecNewFolderPaths, std::vector< std::wstring >& vecKomNameList )		// 루아 스크립트로 부터 kom으로 압축할 새로운 폴더들의 경로 받아오기(신버전 폴더만 열었을때)
{
	// 상황 알림 쓰레드 실행
	// 중복 파일 체크
	// 동일 파일 리스트 띄우기
	// 루아 스크립트 컴파일 여부 체크 및 컴파일
	// 루아 스크립트 암호화 여부 체크 및 암호화

	m_mapOldMemberFile.clear();
	m_mapNewMemberFile.clear();
	m_vecInsertedFile.clear();
	m_vecDeletedFile.clear();
	m_mapKomSize.clear();
	////////////////////////////////////////////////////////////////////////////////////////////

	m_bIsCompareKom = false;

	//CHAR strTemp[MAX_PATH];
	WCHAR wstrNewFolder[MAX_PATH] = L"";
	//WCHAR drive[_MAX_DRIVE]	= L"";
	//WCHAR dir[_MAX_DIR]		= L"";
	WCHAR folderName[_MAX_FNAME] = L"";
	WCHAR wstrTemp[_MAX_FNAME]	= L"";;

	std::vector< std::wstring >::iterator vit = m_vecFolderList.begin();
	//std::vector< std::wstring >::iterator vItrForFolderPathsFromLua = vecNewFolderPaths.begin();
	std::vector< std::wstring >::iterator vItrForKomNameListFromLua = vecKomNameList.begin();

	for ( ; vit != m_vecFolderList.end(); vit++, /*vItrForFolderPathsFromLua++,*/ vItrForKomNameListFromLua++ )
	{
		std::vector< MemberFileInfo > vecMemberFileList;
		
		StringCchCopyW( wstrNewFolder, MAX_PATH, vit->c_str() );
		//StringCchCopyW( wstrTemp, _MAX_FNAME, vItrForFolderPathsFromLua->c_str() );
		StringCchCopyW( folderName, _MAX_FNAME, vItrForKomNameListFromLua->c_str() );

		std::vector< std::wstring >::iterator vItrUpdatedFolderPath = theApp.m_komFileManager.m_vecPathListToCompress.begin();

		for ( ; vItrUpdatedFolderPath != theApp.m_komFileManager.m_vecPathListToCompress.end(); vItrUpdatedFolderPath++ )
		{
			StringCchCopyW( wstrTemp, _MAX_FNAME, vItrUpdatedFolderPath->c_str() );
			WCHAR* p = NULL;
			p = StrStrW( vItrUpdatedFolderPath->c_str(), wstrNewFolder );

			if ( p != NULL )	// 변경된 폴더
				break;
		}

		if ( theApp.m_komFileManager.m_vecPathListToCompress.empty() == false &&
			vItrUpdatedFolderPath == theApp.m_komFileManager.m_vecPathListToCompress.end() )	// 변경된 폴더가 아니면
		{
			theApp.m_komFileManager.m_vecPathListToCopy.push_back( std::wstring( folderName ) + L".kom" );
		}
		else
		{
			FindFolderFile( wstrNewFolder, vecMemberFileList, folderName, FS_INSERTED );	// FS_INSERTED는 지금 TEST용

			if( vecMemberFileList.empty() == true )
			{
				::MessageBox( NULL, wstrNewFolder, L"에러!", MB_OK );
				return FALSE;
			}

			m_mapNewMemberFile.insert( std::make_pair( std::wstring(folderName), vecMemberFileList ) );
		}
	}
	
	return TRUE;
}

BOOL CX2KomFileManager::GetKomFileAndFolderToCompressListFromLuaScript( std::vector< std::wstring >& vecNewFolderPaths, std::vector< std::wstring >& vecKomNameList, bool bIsLuaScriptComplile, bool bIsFileCrypt,
																	   std::map< std::wstring, MemberFileInfo >& mapLuaFiles )
{
	// 상황 알림 쓰레드 실행
	// 중복 파일 체크
	// 동일 파일 리스트 띄우기
	// 루아 스크립트 컴파일 여부 체크 및 컴파일
	// 루아 스크립트 암호화 여부 체크 및 암호화

	
	////////////////////////////////////////////////////////////////////////////////////////////
	m_mapOldMemberFile.clear();
	m_mapNewMemberFile.clear();
	m_vecInsertedFile.clear();
	m_vecDeletedFile.clear();
	m_mapKomSize.clear();


	///////////////////////////////////////////////////////////////////////////////////////////////// oldKom 처리
	GetKomFileListFromLuaScript();
	m_bIsCompareKom = true;
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////// newFolder 처리
	//CHAR strTemp[MAX_PATH];
	WCHAR wstrFolder[MAX_PATH];
	//WCHAR drive[_MAX_DRIVE]	= L"";
	//WCHAR dir[_MAX_DIR]		= L"";
	WCHAR folderName[_MAX_FNAME] = L"";
	//WCHAR wstrTemp[_MAX_FNAME] = L"";

	std::vector< std::wstring >::iterator vit = m_vecFolderList.begin();
	std::vector< std::wstring >::iterator vItrForFolderPathsFromLua = vecNewFolderPaths.begin();
	std::vector< std::wstring >::iterator vItrForKomNameListFromLua = vecKomNameList.begin();

	for ( ; vit != m_vecFolderList.end(); vit++, /*vItrForFolderPathsFromLua++,*/ vItrForKomNameListFromLua++ )
	{
		std::vector< MemberFileInfo > vecMemberFileList;

		StringCchCopyW(wstrFolder, MAX_PATH, vit->c_str() );
		//StringCchCopyW( wstrTemp, _MAX_FNAME, vItrForFolderPathsFromLua->c_str() );
		StringCchCopyW( folderName, _MAX_FNAME, vItrForKomNameListFromLua->c_str() );
		/*
		WCHAR* wstrItrPrev = wstrTemp;
		WCHAR* wstrItrNext = wstrTemp;

		const int MAGIC_NUMBER = 1; // "\\" 부분 만큼 포인터 건너 띄어주기 위한 매직넘버

		if ( ( wstrItrNext = wcsstr( wstrItrPrev, L"\\" ) ) != NULL )
		{
			StringCchCopyNW( folderName, _MAX_FNAME, wstrItrPrev, wstrItrNext - wstrItrPrev );
			while ( wstrItrNext != NULL )
			{
				StrCatW( folderName, L"_" );

				wstrItrPrev = wstrItrNext + MAGIC_NUMBER;

				if ( ( wstrItrNext = wcsstr( wstrItrPrev, L"\\" ) ) != NULL )
					StrNCatW( folderName, wstrItrPrev, wstrItrNext - wstrItrPrev + MAGIC_NUMBER );
				else
					StrNCatW( folderName, wstrItrPrev, wcslen( wstrItrPrev ) + MAGIC_NUMBER );
			}
		}
		else
			StringCchCopyW( folderName, _MAX_FNAME, wstrTemp );
		*/

		MemberFileIterator mit = m_mapOldMemberFile.find( folderName );

		if ( mit != m_mapOldMemberFile.end() )			// 폴더 이름과 같은 kom 파일이 있으면
		{
			FindFolderFileAndSetFileState( wstrFolder, vecMemberFileList, folderName, mit, TRUE, bIsLuaScriptComplile, bIsFileCrypt, mapLuaFiles ); //FS_NONE or FS_CHANGED

		}
		else		// 같지 않으면
		{
			FindFolderFileAndSetFileState( wstrFolder, vecMemberFileList, folderName, NULL, FALSE, bIsLuaScriptComplile, bIsFileCrypt, mapLuaFiles );	//FS_INSERTED
		}

		//*
		if( vecMemberFileList.empty() == true )
		{
			::MessageBox( NULL, wstrFolder, L"에러!", MB_OK );
			return FALSE;
		}
		//*/

		m_mapNewMemberFile.insert( std::make_pair( std::wstring(folderName), vecMemberFileList ) );
	}

	
	//////////////////////////////////////////////////////////////////////////	
	// verify deleted files		
	MemberFileIterator mitOld = m_mapOldMemberFile.begin();
	for( ; mitOld != m_mapOldMemberFile.end(); mitOld++ )
	{
		std::vector< MemberFileInfo >::iterator vit = mitOld->second.begin();
		for( ; vit != mitOld->second.end(); vit++ )
		{
			MemberFileInfo& memberFile = *vit;

			if( IsExistFileInNewMember( memberFile.m_wstrFileName ) == false )
			{
				memberFile.m_iFileState	   = FS_DELETED;
				memberFile.m_wstrKomString = mitOld->first;

				m_vecDeletedFile.push_back( memberFile );
			}
		}
	}

	return TRUE;
}
void CX2KomFileManager::FindFolderFile( IN WCHAR* wstrFolder, OUT std::vector< MemberFileInfo >& vecMemberFileInfoList, WCHAR* pwstrFolderName, FILE_STATE enumState )
{
	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;
	WCHAR				wstrSearchPath[256] = L"";
	WCHAR				fname[_MAX_FNAME] = L"";
	WCHAR				ext[_MAX_EXT]		= L"";

	wcscpy( wstrSearchPath, wstrFolder );
	wcscat( wstrSearchPath, L"\\*.*" );

	hSearch = FindFirstFileW( wstrSearchPath, &fd );

	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if( wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") && wcscmp(fd.cFileName, L".svn") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				WCHAR	wstrNewSearchDir[256];

				wcscpy( wstrNewSearchDir, wstrFolder );
				wcscat( wstrNewSearchDir, L"\\");
				wcscat( wstrNewSearchDir, fd.cFileName );

				FindFolderFile( wstrNewSearchDir, vecMemberFileInfoList, pwstrFolderName, enumState );
			}
			else
			{
				WCHAR			wstrNewSearchFile[256];
				wcscpy( wstrNewSearchFile, wstrFolder );
				wcscat( wstrNewSearchFile, L"\\");
				wcscat( wstrNewSearchFile, fd.cFileName );

				_wsplitpath( wstrNewSearchFile, 0, 0, 0, ext);
				std::wstring wstrExt = ext;
				MakeUpperCase( wstrExt );

				if( wstrExt == L".KOM" )
				{
					::MessageBox( NULL, L"member파일중에 kom파일이 섞여있습니다!", L"에러!", MB_OK );
					return;
				}

				// MP3 파일은 포함안함.
				if( wstrExt == L".MP3"  ||  wstrExt == L".X_OLD"  ||  wstrExt == L".Y_OLD" )
					continue;

				// File full name
				std::wstring wstrFileName;
				wstrFileName = fname;
				wstrFileName += ext;

				//////////////////////////////////////////////////////////////////////////
				// 중복 압축 방지
				bool bIsNoSame = false;
				MemberFileIterator mitNoSame = m_mapNewMemberFile.begin();
				std::vector< MemberFileInfo >::iterator vitNoSame;
				for( ; mitNoSame != m_mapNewMemberFile.end(); mitNoSame++ )
				{
					vitNoSame = std::find_if( mitNoSame->second.begin(), mitNoSame->second.end(), MemberFileInfo::FIND_IF( wstrFileName ) );
					if( vitNoSame != mitNoSame->second.end() )
					{
						bIsNoSame = true;
					}
				}
				if( bIsNoSame == true )
					continue;
				//////////////////////////////////////////////////////////////////////////

				MemberFileInfo memberFile;
				memberFile.m_wstrFileName = fd.cFileName;
				memberFile.m_iFileState	  = enumState;
				memberFile.m_wstrRealPath = wstrNewSearchFile; // 파일이 원래 있던 경로
				memberFile.m_wstrKomString = pwstrFolderName;
				// file size = (fd.nFileSizeHigh * (MAXDWORD +1)) + fd.nFileSizeLow
				memberFile.m_iFileSize	  = ( (fd.nFileSizeHigh * (MAXDWORD +1)) + fd.nFileSizeLow ) / 1024; 
//
				vecMemberFileInfoList.push_back( memberFile );
			}
		}

	}while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
}

void CX2KomFileManager::FindFolderKomFile( IN WCHAR* wstrFolder )//, OUT std::vector< std::wstring >& vecFileList )
{
	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;
	WCHAR				wstrSearchPath[256] = L"";
	WCHAR				komFileName[_MAX_FNAME];
	WCHAR				ext[_MAX_EXT];

	wcscpy( wstrSearchPath, wstrFolder );
	wcscat( wstrSearchPath, L"\\*.*" );

	hSearch = FindFirstFile( wstrSearchPath, &fd );

	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if( wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") && wcscmp(fd.cFileName, L".svn") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				WCHAR	wstrNewSearchDir[256];

				wcscpy( wstrNewSearchDir, wstrFolder );
				wcscat( wstrNewSearchDir, L"\\");
				wcscat( wstrNewSearchDir, fd.cFileName );

				FindFolderKomFile( wstrNewSearchDir );//, vecFileList );
			}
			else
			{
				WCHAR			wstrNewSearchFile[256];
				std::wstring	wstrFile;

				wcscpy( wstrNewSearchFile, wstrFolder );
				wcscat( wstrNewSearchFile, L"\\");
				wcscat( wstrNewSearchFile, fd.cFileName );

				wstrFile = wstrNewSearchFile;

				_wsplitpath( wstrNewSearchFile, 0, 0, komFileName, ext);

				// 확장자 검사
				std::wstring wstrExt = ext;
				MakeUpperCase( wstrExt );

				// kom파일이 아니면 continue
				if( wstrExt != L".KOM" )
					continue;

				//vecFileList.push_back( wstrFile );
				m_mapKomRealPath.insert( std::make_pair( std::wstring( komFileName ), wstrFile ) );

				CMassFile massFile;
				CHAR strTemp[256] = "";
				WideCharToMultiByte( CP_ACP, 0, wstrFile.c_str(), -1, strTemp, MAX_PATH, NULL, NULL );

				massFile.LoadMassFile( strTemp ); // mass file load

				CMassFile::MASSFILE_MEMBERFILEINFO Info;
				ZeroMemory( &Info, sizeof( Info ) );
				WCHAR wstrTemp[256] = L"";

				for( UINT i = 0; i < massFile.GetFileCount(); ++i )
				{
					massFile.GetMemberFile(i,&Info);

					MultiByteToWideChar( CP_ACP, 0, Info.strFileName, -1, wstrTemp, MAX_PATH );

					// kom 등록 확인
					MemberFileIterator mit;
					mit = m_mapOldMemberFile.find( std::wstring( komFileName ) );
					if( mit == m_mapOldMemberFile.end() )
					{
						m_mapOldMemberFile.insert( std::make_pair( std::wstring( komFileName ), std::vector< MemberFileInfo >() ) );
						mit = m_mapOldMemberFile.find( std::wstring( komFileName ) );
					}

					// 멤버 파일 저장
					MemberFileInfo memberFile;
					memberFile.m_wstrFileName = wstrTemp;
					memberFile.m_iFileState	  = FS_NONE;
					memberFile.m_iFileSize	  = Info.size / 1024; // byte -> kilobyte

					mit->second.push_back( memberFile );

					// for log
					//++iTotalFileCount;
				}
			}
		}

	}while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);

}

void CX2KomFileManager::FindFolderFileAndSetFileState( IN WCHAR* wstrFolder, OUT std::vector< MemberFileInfo >& vecMemberFileInfoList, WCHAR* pwstrFolderName,
													  MemberFileIterator mit, BOOL bIsSameFolder, bool bIsLuaScriptComplile, bool bIsFileCrypt, 
													  std::map< std::wstring, MemberFileInfo >& mapLuaFiles )
{
	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;
	WCHAR				wstrSearchPath[256] = L"";
	WCHAR				fname[_MAX_FNAME];
	WCHAR				ext[_MAX_EXT] = {0,};			
	std::vector< MemberFileInfo >::iterator vit;

	wcscpy( wstrSearchPath, wstrFolder );
	wcscat( wstrSearchPath, L"\\*.*" );

	hSearch = FindFirstFile( wstrSearchPath, &fd );

	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if( wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") && wcscmp(fd.cFileName, L".svn") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				WCHAR	wstrNewSearchDir[256];

				wcscpy( wstrNewSearchDir, wstrFolder );
				wcscat( wstrNewSearchDir, L"\\");
				wcscat( wstrNewSearchDir, fd.cFileName );

				FindFolderFileAndSetFileState( wstrNewSearchDir, vecMemberFileInfoList, pwstrFolderName, mit, bIsSameFolder, bIsLuaScriptComplile, bIsFileCrypt, mapLuaFiles );
			}
			else
			{
				WCHAR			wstrNewSearchFile[256];
				//std::wstring	wstrFile;
				wcscpy( wstrNewSearchFile, wstrFolder );
				wcscat( wstrNewSearchFile, L"\\");
				wcscat( wstrNewSearchFile, fd.cFileName );

				// .kom, mp3, x_old, y_old 확장자 검사 필요
				//wstrFile = wstrNewSearchFile;
				_wsplitpath( wstrNewSearchFile, 0, 0, fname, ext);

				std::wstring wstrExt = ext;
				MakeUpperCase( wstrExt );

				if( wstrExt == L".KOM" )
				{
					::MessageBox( NULL, L"member파일중에 kom파일이 섞여있습니다!", L"에러!", MB_OK );
					return;
				}

				// mp3 파일은 포함안함.
				if( wstrExt == L".MP3"  ||  wstrExt == L".X_OLD"  ||  wstrExt == L".Y_OLD" )
					continue;

				// File full name
				std::wstring wstrFileName;
				wstrFileName = fname;
				wstrFileName += ext;

				//////////////////////////////////////////////////////////////////////////
				// 중복 압축 방지
				bool bIsNoSame = false;
				MemberFileIterator mitNoSame = m_mapNewMemberFile.begin();
				std::vector< MemberFileInfo >::iterator vitNoSame;
				for( ; mitNoSame != m_mapNewMemberFile.end(); mitNoSame++ )
				{
					vitNoSame = std::find_if( mitNoSame->second.begin(), mitNoSame->second.end(), MemberFileInfo::FIND_IF( wstrFileName ) );
					if( vitNoSame != mitNoSame->second.end() )
						bIsNoSame = true;
				}
				if( bIsNoSame == true )
					continue;
				//////////////////////////////////////////////////////////////////////////
								
				MemberFileInfo memberFile;
				memberFile.m_wstrFileName = fd.cFileName;
				if( wstrExt == L".TXT"  ||  wstrExt == L".LUA" )
				{
					std::map< std::wstring, MemberFileInfo >::iterator mItr = mapLuaFiles.find( fd.cFileName );
					if ( mItr != mapLuaFiles.end() )
					{
						memberFile.m_wstrRealPath = mItr->second.m_wstrRealPath;
					}
				}
				else
				{
					memberFile.m_wstrRealPath = wstrNewSearchFile; // 파일이 원래 있던 경로
				}
				memberFile.m_wstrKomString = pwstrFolderName;
				// file size = (fd.nFileSizeHigh * (MAXDWORD +1)) + fd.nFileSizeLow
				memberFile.m_iFileSize	  = ( (fd.nFileSizeHigh * (MAXDWORD +1)) + fd.nFileSizeLow ) / 1024; 
				//

				if ( bIsSameFolder == TRUE )		// 폴더명과 같은 kom 파일이 있으면
				{
					vit = std::find_if( mit->second.begin(), mit->second.end(), MemberFileInfo::FIND_IF( memberFile.m_wstrFileName ) );

					if ( vit != mit->second.end() )
					{
						if ( vit->m_iFileSize == memberFile.m_iFileSize ) // old 파일 사이즈와 new 파일 사이즈를 비교해서 같으면
						{
							memberFile.m_iFileState = FS_NONE;
						}
						else
						{
							memberFile.m_iFileState = FS_CHANGED;
						}
					}
					else		// vit == mit->second.end()
					{
						memberFile.m_iFileState = FS_INSERTED;
						m_vecInsertedFile.push_back( memberFile );
					}
					
				}
				else // bIsSameFolder == FALSE							// 없으면
				{
					memberFile.m_iFileState	  = FS_INSERTED;
					m_vecInsertedFile.push_back( memberFile );
				}
				
				vecMemberFileInfoList.push_back( memberFile );
			}
		}

	}while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
	//*/
}

// 중복파일 체크
bool CX2KomFileManager::CheckSameFiles( OUT std::map< std::wstring, std::vector< std::wstring > >& vecSameFiles )
{

	// log기록 파일 오픈
	if( !m_ofs.is_open() )
		m_ofs.open( "KomLog.txt" );

	WCHAR drive[_MAX_DRIVE]	= L"";
	WCHAR dir[_MAX_DIR]		= L"";
	WCHAR fname[_MAX_FNAME]	= L"";
	WCHAR ext[_MAX_EXT]		= L"";

	std::vector< std::wstring > vecMemberFileList;
	WCHAR wstrNewFolder[MAX_PATH];
	std::vector< std::wstring >::iterator vit = m_vecFolderList.begin();

	for ( ; vit != m_vecFolderList.end(); vit++ )
	{
		//mbstowcs(wstrNewFolder, vit->c_str(), MAX_PATH);
		StringCchCopyW( wstrNewFolder, MAX_PATH, vit->c_str() );
		FindFolderFile( wstrNewFolder, vecMemberFileList);
	}
		
	if( vecMemberFileList.empty() == true )
		return false;

	vecSameFiles.clear();
	std::map< std::wstring, std::vector< std::wstring > >::iterator mit;
	std::wstring wstrFileName;
	std::vector< std::wstring >::iterator vit2 = vecMemberFileList.begin();		// 신버전 폴더안의 파일경로들
	for( ; vit2 != vecMemberFileList.end(); vit2++ )
	{
		_wsplitpath( (*vit2).c_str(), drive, dir, fname, ext);

		wstrFileName = fname;
		wstrFileName += ext;

		mit = vecSameFiles.find( wstrFileName );		// 같을 파일명이 있는지 찾은 후
		if( mit == vecSameFiles.end() )					// 같은게 없으면
		{
			vecSameFiles.insert( std::make_pair( wstrFileName, std::vector< std::wstring >() ) ); // 파일명을 키값으로 MAP생성 (key:fileName, T: path)
			mit = vecSameFiles.find( wstrFileName );
		}

		mit->second.push_back( *vit2 );					// 해당 파일명을 키값으로 중복되는 파일들의 경로를 넣음
	}

	int iCount = 0;
	for( mit = vecSameFiles.begin(); mit != vecSameFiles.end(); mit++ )		// 로그 기록
	{
		if( mit->second.size() > 1 )
		{
			std::vector< std::wstring >::iterator vit2;
			for( vit2 = mit->second.begin(); vit2 != mit->second.end(); vit2++ )
			{
				std::string temp;
				ConvertWCHARToChar( temp, vit2->c_str() );				
				m_ofs << GetFileSize( temp.c_str() ) / 1024 << L" kb   :   " << *vit2 << std::endl;
				++iCount;
			}

			m_ofs << std::endl;
		}
	}

	m_ofs.close();
	return true;
}

BOOL CX2KomFileManager::InitNewFolderPaths_Lua()//( const std::wstring wstrNewPath )
{
	KLuaManager luaManager( g_pLua );
	int iIndex = 1;
	std::wstring wstrNewPath = L"";
	std::wstring wstrKomName = L"";

	if ( luaManager.BeginTable( "m_strNewFolderPaths" ) == false )
	{
		::MessageBox( NULL, L"m_strNewFolderPaths를 Lua Stack으로 가져오지 못했습니다.", L"", MB_OK );
		luaManager.EndTable();

		return FALSE;
	}

	while ( true )
	{
		if ( luaManager.GetValue( iIndex, wstrNewPath ) == false )
			break;
		iIndex++;

		if ( luaManager.GetValue( iIndex, wstrKomName ) == false )
			break;
		iIndex++;

		m_vecFolderList.push_back( wstrNewPath );
		m_vecKomNameList.push_back( wstrKomName );
		
	}

	if ( m_vecFolderList.empty() == TRUE )
	{
		::MessageBox( NULL, L"InitNewFolderPaths.lua 내에 폴더 경로가 지정되어 있지 않습니다.", L"", MB_OK );
		luaManager.EndTable();

		return FALSE;
	}

	luaManager.EndTable();
	return TRUE;
}

std::vector< std::wstring > CX2KomFileManager::GetNewFolderList()
{
	return m_vecFolderList;
}

const std::wstring&			CX2KomFileManager::GetDepotPath() const
{
	return m_wstrDepotPath;
}