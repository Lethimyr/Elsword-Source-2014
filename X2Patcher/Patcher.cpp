#include "Patcher.h"

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <io.h>

//#define SKIP_PATCH // 대회용으로 사용할 때 (패치 과정을 스킵한다.)


#define MAX_RETRY           30                          // 최대 재시도 횟수
#define MAX_DELAY           500                         // 재시도 지연 간격

// Variable declarations.
KPatcher*	g_pPatcher          = NULL;  ///< Patcher instance.
bool		g_bTestMode  = true; ///< 사내 테스트 모드인가?



#ifdef DMLEE_PATCHER_TEST
extern bool g_bPatchLaunchButtonEnabled;
#endif // DMLEE_PATCHER_TEST

// Function declarations.
void Log( const char* szLogMsg );
void LogError( const char* szLogMsg );


/// Default constructor.
/// @param hWnd handle to download window.
KPatcher::KPatcher( HWND hWnd )
{
    m_hWnd       = hWnd;
    m_bIsPatched = false;
    m_dwByteFile = 0;
    m_dwRateDown = 0;

	m_dwCurDownAllFileSize = 0;
	m_dwDownAllFileSize = 0;

	m_strProgramPath = "";
}

/// Default destructor.
KPatcher::~KPatcher( void )
{
    // empty
}

void KPatcher::AddCurDownAllFileSize( DWORD dwSize ) 
{ 
	m_dwCurDownAllFileSize += dwSize;
}

/// Create directory.
/// @param szPath_ 생성할 디렉토리 경로
/// @return 성공하면 true, 실패하면 false
bool KPatcher::CreateDirectory( const char* szPath_ ) const // by ctkim
{
    char* szToken   = NULL;
    char* szToken2  = NULL;
    char szDirName[20][64];

    int len = (int)strlen(szPath_);
    int ct = 0;
    int numDir = -1;
    while( ct < len )
    {
        if( szPath_[ct] == '\\' )
        {
            // 가장 처음에 발견되면 무시
            if( numDir == -1 )
            {
                numDir++;
            }
            else
            {
                memcpy( szDirName[numDir], szPath_, (ct+1) );
                szDirName[numDir][ct+1] = '\0';
                numDir++;
            } // else
        } // if
        ct++;
    } // while

    // Create a new directory.
    ct = 0;
    while( ct < numDir )
    {
        ::CreateDirectoryA( szDirName[ct], NULL );
        ct++;
    } // while
    return true;
} // KPatcher::CreateDirectory()

/// 주어진 소스 경로로부터 폴더 경로를 제외한 파일이름을 얻는다.
/// @param szDest output parameter
/// @param szSrc source path
/// @return void
void KPatcher::ExceptFolder( char* szDest, const char* szSrc ) const
{
    for ( int iDestFeed = 0, iSrcFeed = 0;
          NULL != szSrc[iSrcFeed];
          iDestFeed++, iSrcFeed++ )
    {
        szDest[iDestFeed] = szSrc[iSrcFeed];
        if ( '\\' == szSrc[iSrcFeed] ) iDestFeed = -1;
    }
    szDest[iDestFeed] = '\0';
}

/// 패치경로 파일로부터 현재 버전의 패치 경로를 읽는다.
/// @param szSrcPath output parameter
/// @return 성공하면 true, 실패하면 false
bool KPatcher::ReadPatchPathFile( char* szSrcPath ) const
{
    HANDLE hFile    = NULL;
    DWORD  dwRead   = 0;
    BYTE   curByte  = 0;
    int    iFeed    = -1;
    bool   bSuccess = false;

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, PATCH_PATH_FILE, -1, wcharBuf, 255 );
	
    if ( INVALID_HANDLE_VALUE == ( hFile = ::CreateFile( wcharBuf,
                                                         GENERIC_READ,
                                                         0,
                                                         NULL,
                                                         OPEN_EXISTING,
                                                         FILE_ATTRIBUTE_NORMAL,
                                                         NULL ) ) )
    {
        return false;
    }

    while ( FALSE != ::ReadFile( hFile, &curByte, 1, &dwRead, NULL ) )
    {
        if ( 0 == dwRead ) break;

        switch ( curByte )
        {
            case '\n': case '\t': case ' ': case '\0':                 // skip
                break;
            case '<': if (-1 == iFeed) iFeed = 0; else goto Exit;      // begin
                break;
            case '>': if (-1 != iFeed) { bSuccess = true; goto Exit; } // end
                break;
            default:  if (-1 != iFeed) szSrcPath[iFeed++] = curByte;   // read
                break;
        }
    }

Exit:
    ::CloseHandle( hFile );
    return bSuccess;
}

/// 패치경로 파일로부터 현재 버전의 패치 경로를 읽는다.
/// @param strSrcPath output parameter
/// @return 성공하면 true, 실패하면 false
bool KPatcher::ReadPatchPathFile( std::string& strSrcPath ) const
{
    char szSrcPath[MAX_PATH] = { 0, };
    bool bRet  = ReadPatchPathFile( szSrcPath );
    strSrcPath = szSrcPath;
    return bRet;
}

/// 파일을 반드시 지운다.
/// @param szFileName 파일 이름
/// @return void
void KPatcher::DeleteFileAbsolutely( const char* szFileName ) const
{
    DWORD dwAtt;
	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szFileName, -1, wcharBuf, 255 );

    if ( 0xffffffff != ( dwAtt = ::GetFileAttributes( wcharBuf ) ) )
    {
        ::SetFileAttributes( wcharBuf, dwAtt & (~FILE_ATTRIBUTE_READONLY) );
    }
    ::DeleteFile( wcharBuf );
}

/// 구버전 목록 파일(Ver.xml)을 신버전 목록 파일(NewVer.xml)로 갱신한다.
/// @param void
/// @return 성공하면 true, 실패하면 false
bool KPatcher::UpdateVersionFile( void ) const
{
    // NewVer.xml의 name을 Ver.xml로 변경.

    DeleteFileAbsolutely( "Ver.xml" );

    if ( FALSE == ::MoveFile( L"NewVer.xml", L"Ver.xml" ) )
    {
        Log( "Ver.xml 파일 업데이트에 실패했습니다." );


        KMsgBox::Open( "버전 파일 업데이트 실패",
                       "다시 시도해 주십시오.",
                       KMsgBox::MBOX_OK );

        return false;
    }
    Log( "Ver.xml 파일 업데이트에 성공했습니다." );

    return true;
}

/// 다운로드 창을 열거나 닫는다.
/// @param bOpen 열 것인지 닫을 것인지 결정
/// @return void
void KPatcher::OpenDownloadWindow( bool bOpen ) const
{
    if ( true == bOpen )
    {
        // 다운로드 창을 띄운다.
        ::ShowWindow( m_hWnd, SW_SHOW );
        ::SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
        ::SetFocus( m_hWnd );
    }
    else
    {
        // 다운로드 창을 숨긴다.
        ::SetFocus( NULL );
        ::ShowWindow( m_hWnd, SW_HIDE );
    }
}

/// 문자열 끝에 '\\'가 있는지 확인하여 없으면 추가한다.
/// @param str output parameter
/// @return void
void KPatcher::AppendBackSlash( std::string& str ) const
{
    if ( '\\' != str[str.size() - 1] )
    {
        str.append( "\\" );
    }
}

/// 문자열 끝에 '/'가 있는지 확인하여 없으면 추가한다.
/// @param str output parameter
/// @return void
void KPatcher::AppendSlash( std::string& str ) const
{
    if ( '/' != str[str.size() - 1] )
    {
        str.append( "/" );
    }
}

/// 게임 클라이언트 프로그램을 체크하여 패치할 파일이 있는지 확인하여 패치한다.
/// @param void
/// @return 패치할 파일이 없거나 패치에 성공하면 true, 실패하면 false
bool KPatcher::CheckProgram( void )
{
    KRegistry       kReg;
    KHttpWrapper    kHttp;
    KEnumFile       kef;

    TiXmlNode*      node        = NULL;
    TiXmlElement*   verElement  = NULL;
    TiXmlElement*   dirElement  = NULL;
    TiXmlElement*   fileElement = NULL;

    DWORD           dwCurFileNum   = 1;
    DWORD           dwTotalFileNum = 0;
    DWORD           dwRetryCount   = 0;
    char            szStringBuffer[MAX_PATH] = { 0, };

    KMsgBox::Close();

    KMsgBox::Open( "패치 작업 수행 중입니다.",
                   "패치 경로 파일 다운로드 중" );


	if ( g_bTestMode == true )
    {
        Log("테스트 모드로 패치 프로그램이 시작됩니다.");

		
    }

	// 현재 디렉토리를 프로그램의 경로로 지정한다.
	::GetCurrentDirectoryA( MAX_PATH, szStringBuffer );
	m_strProgramPath = szStringBuffer;
	m_strProgramPath += "\\Data";
	Log( m_strProgramPath.c_str() );

    // 경로 끝부분에 '\\'가 빠져 있다면 추가해준다.
    AppendBackSlash( m_strProgramPath );

    // 프로그램 경로 상에 있는 디렉토리들을 만들어낸다.
    CreateDirectory( m_strProgramPath.c_str() );

    // 프로그램의 경로를 현재 디렉토리로 설정한다.
    ::SetCurrentDirectoryA( m_strProgramPath.c_str() );
#ifndef SKIP_PATCH
	// Http를 연다.
    if ( false == kHttp.Open() )
    {
        Log( "HTTP 열기에 실패했습니다." );

        KMsgBox::Close();

        KMsgBox::Open( "패치 서버 접속 실패",
                       "다시 시도해 주십시오.",
                       KMsgBox::MBOX_OK );

        return false;
	}
    Log( "HTTP 열기에 성공했습니다." );

    // 패치 서버 경로를 만든다.
    std::string strSrcPath( ( true == g_bTestMode ) ?
                            ( 5 == __argc ? __argv[2] : PATCH_TEST_SRC ) :
                            ( PATCH_ORG_SRC ) );

    // 패치 서버 경로 끝부분에 '/'가 빠져 있다면 추가해준다.
    AppendSlash( strSrcPath );

    // 패치 경로 파일( PATCH_PATH_FILE )을 다운로드한다.
    while ( false == kHttp.Get( strSrcPath.c_str(),
                                PATCH_PATH_FILE,
                                m_strProgramPath.c_str() ) )
    {
        // MAX_RETRY번 시도 끝에 결국 안되면 종료한다.
        if ( MAX_RETRY == ++dwRetryCount )
        {
            Log( PATCH_PATH_FILE " 파일 다운로드에 실패했습니다." );

            KMsgBox::Close();

            KMsgBox::Open( "경로 파일 받기 실패",
                           "다시 시도해 주십시오.",
                           KMsgBox::MBOX_OK );

            return false;
        }
        ::Sleep( MAX_DELAY );
        kHttp.Reset();
    }
    Log( PATCH_PATH_FILE " 파일 다운로드에 성공했습니다." );

    // 패치 파일들이 있는 곳의 경로를 읽는다.
    if ( false == ReadPatchPathFile( strSrcPath ) )
    {
        Log( PATCH_PATH_FILE " 파일 읽기에 실패했습니다." );

        KMsgBox::Close();

        KMsgBox::Open( "경로 파일 읽기 실패",
                       "다시 시도해 주십시오.",
                       KMsgBox::MBOX_OK );


        return false;
    }
    Log( PATCH_PATH_FILE " 파일 읽기에 성공했습니다." );

    // 패치 파일 경로 끝부분에 '/'가 빠져 있다면 추가해준다.
    AppendSlash( strSrcPath );

    // 다운로드한 패치 경로 파일( PATCH_PATH_FILE )을 삭제한다.
    DeleteFileAbsolutely( PATCH_PATH_FILE );

    KMsgBox::Close();


    KMsgBox::Open( "패치 작업 수행 중입니다.",
                   "버전 파일 다운로드 중" );

    // 패치 서버로부터 새 목록 파일( NewVer.xml )을 다운로드 받는다.
    dwRetryCount = 0;
    while ( false == kHttp.Get( strSrcPath.c_str(),
                                "NewVer.xml",
                                m_strProgramPath.c_str() ) )
    {
        // MAX_RETRY번 시도 끝에 결국 안되면 종료한다.
        if ( MAX_RETRY == ++dwRetryCount )
        {
            Log( "NewVer.xml 파일 다운로드에 실패했습니다." );

            KMsgBox::Close();


            KMsgBox::Open( "버전 파일 받기 실패",
                           "다시 시도해 주십시오.",
                           KMsgBox::MBOX_OK );

            return false;
        }
        ::Sleep( MAX_DELAY );
        kHttp.Reset();
    }
    Log( "NewVer.xml 파일 다운로드에 성공했습니다." );

    // NewVer.xml의 <Directory>를 참조하여 디렉토리 생성.
    TiXmlDocument docServer;
	
	std::string tempPath = m_strProgramPath + "NewVer.xml";
    docServer.LoadFile( tempPath.c_str() );
    if ( NULL == ( node = docServer.FirstChild( "PatchInfo" ) ) ||
         NULL == ( dirElement = node->FirstChild( "Directory" )->ToElement() ) )
    {
        Log( "NewVer.xml 파일 포맷이 잘못되어 있습니다." );

        KMsgBox::Close();

        KMsgBox::Open( "버전 파일 읽기 실패",
                       "다시 시도해 주십시오.",
                       KMsgBox::MBOX_OK );


        return false;
    }

    // 참조한 <Directory>로부터 디렉토리 생성.
    for ( node = dirElement->FirstChild( "Item" ); NULL != node; )
    {
        std::string strDir( m_strProgramPath );
        strDir.append( node->ToElement()->Attribute( "Name" ) );

		WCHAR wcharBuf[255] = {0,};
		MultiByteToWideChar( CP_ACP, 0, strDir.c_str(), -1, wcharBuf, 255 );

        ::CreateDirectory( wcharBuf, NULL );
        node = node->NextSibling( "Item" );
    }

    KMsgBox::Close();


    KMsgBox::Open( "새로운 버전과 설치된 버전을",
                   "비교하고 있는 중입니다." );



    // local의 "Ver.xml"과 server에서 받은 "NewVer.xml"을 비교하여
    // 다운로드해야 할 파일리스트( updateFilelist.xml )를 생성.
    TiXmlDocument xmlUpdateDoc( "updateFilelist.xml" );



	// 두 개의 .xml 파일을 비교하여 변경된 부분만 .xml로 생성.
    if ( false == kef.CreateUpdateFileList( "Ver.xml",    // local file
                                            "NewVer.xml", // server file
                                            xmlUpdateDoc ) )
    {
        Log( "updateFileList.xml 파일 만들기에 실패했습니다." );

        KMsgBox::Close();


        KMsgBox::Open( "버전 비교 작업 실패",
                       "다시 시도해 주십시오.",
                       KMsgBox::MBOX_OK );

		return false;
    }
    Log( "updateFileList.xml 파일 만들기에 성공했습니다." );

    node        = xmlUpdateDoc.FirstChild( "PatchInfo" );
    verElement  = node->FirstChild( "PatchVersion" )->ToElement();
    dirElement  = node->FirstChild( "Directory" )->ToElement();
    fileElement = node->FirstChild( "File" )->ToElement();

    // append "PatchVersion"
    TiXmlElement verItem( "Item" );
    verItem.SetAttribute( "Name", strSrcPath.c_str() );
    verElement->InsertEndChild( verItem );

	DWORD tempAllDownFileSize = 0;
    // 다운로드 받을 파일 개수를 계산한다.
    for ( node = fileElement->FirstChild( "Item" ); NULL != node; )
    {
        dwTotalFileNum++;
		m_dwDownAllFileSize += ::atol( node->ToElement()->Attribute( "Size" ) );

        node = node->NextSibling( "Item" );
    }

    KMsgBox::Close(); // 열어두었던 메세지 박스를 닫는다.

	if ( 0 != dwTotalFileNum ) // 다운로드 받을 파일이 하나라도 있는 경우.
    {
    // 패치를 하겠냐고 묻는 다이얼로그를 띄우지 않음.
/*
#ifdef _JAPANESE
        if ( IDCANCEL == KMsgBox::Open( "You have to patch new version.",
                                        "Continue?",
                                        KMsgBox::MBOX_OKCANCEL ) )
#else // _KOREAN
        if ( IDCANCEL == KMsgBox::Open( "새로운 버전으로 패치해야 합니다.",
                                        "계속 진행 하시겠습니까?",
                                        KMsgBox::MBOX_OKCANCEL ) )
#endif
        {
            Log( "프로그램 패치가 취소되었습니다." );

            return false;
        }
        Log( "프로그램 패치가 승인되었습니다." );
*/
        m_bIsPatched = true; // 패치( O )

        // 다운로드 창을 띄운다.
        OpenDownloadWindow( true );
	}
    else // 다운로드 받을 파일이 없는 경우.
    {
        Log( "패치할 파일이 없습니다." );

        m_bIsPatched = false; // 패치( X )

#ifdef DMLEE_PATCHER_TEST
		SetRateDownload( 100 );
		SetCurFileDownloadPer( 100 );

		// 다운로드 창을 띄운다.
		OpenDownloadWindow( true );
#else
        return UpdateVersionFile(); // update Ver.xml
#endif // DMLEE_PATCHER_TEST
    }

#ifdef DMLEE_PATCHER_TEST
	if ( 0 != dwTotalFileNum )
#endif //DMLEE_PATCHER_TEST
	{
		// updateFilelist.xml이 존재한다면 이어받기를 해야한다.
		if ( 0 == ::_access( "updateFileList.xml", 0 ) )
		{
			TiXmlNode*    tmpNode;
			TiXmlElement* verElmt;
			TiXmlDocument oldUpdateDoc;

			oldUpdateDoc.LoadFile( "updateFileList.xml" );

			// 존재한다면 파일 유효성 체크와 버전 체크를 한다.
			if ( NULL != ( tmpNode = oldUpdateDoc.FirstChild( "PatchInfo" )             ) &&
				NULL != ( verElmt = tmpNode->FirstChild( "PatchVersion" )->ToElement() ) &&
				NULL != (           tmpNode->FirstChild( "Directory" )->ToElement()    ) &&
				NULL != (           tmpNode->FirstChild( "File" )->ToElement()         ) &&
				0 == strSrcPath.compare( verElmt->FirstChild( "Item" )->ToElement()->Attribute( "Name" ) ) )
			{
				// 이미 존재하는 updateFileList.xml로부터 정보를 빼온다.
				xmlUpdateDoc.LoadFile( "updateFileList.xml" );
				node        = xmlUpdateDoc.FirstChild( "PatchInfo" );
				verElement  = node->FirstChild( "PatchVersion" )->ToElement();
				dirElement  = node->FirstChild( "Directory" )->ToElement();
				fileElement = node->FirstChild( "File" )->ToElement();

				// 파일 개수를 다시 계산한다.
				dwTotalFileNum = 0;
				for ( node = fileElement->FirstChild( "Item" ); NULL != node; )
				{
					dwTotalFileNum++;
					node = node->NextSibling( "Item" );
				}
			}
		}

		// updateFileList.xml을 저장한다.
		xmlUpdateDoc.SaveFile();

		// updateFilelist.xml에 <File>를 참조하여 file을 download 함.
		dwRetryCount = 0;
		for ( node = fileElement->FirstChild( "Item" ); NULL != node; )
		{
			m_dwByteFile = ::atol( node->ToElement()->Attribute( "Size" ) );

			::ZeroMemory( szStringBuffer, MAX_PATH );
			ExceptFolder( szStringBuffer, node->ToElement()->Attribute( "Name" ) );

			std::ostringstream strmCurFileDesc;

			//        strmCurFileDesc << szStringBuffer << " (" << dwCurFileNum << '/'
			//                        << dwTotalFileNum << ')';

			//		strmCurFileDesc << szStringBuffer << " ("
			//                        << static_cast<int>( 100 * dwCurFileNum / dwTotalFileNum ) << "%)";

			strmCurFileDesc << szStringBuffer << " 파일 복사중.." ;

			m_strCurFileDesc = strmCurFileDesc.str();

			std::ostringstream strmCurFileDownloadPer;

			strmCurFileDownloadPer  << g_pPatcher->GetCurFileDownloadPer() << "%";

			m_strCurFileDownloadPer = strmCurFileDownloadPer.str();

			if ( false == kHttp.Get( strSrcPath.c_str(),
				node->ToElement()->Attribute( "Name" ),
				m_strProgramPath.c_str() ) )
			{
				// MAX_RETRY번 시도 끝에 결국 안되면 종료한다.
				if ( MAX_RETRY == ++dwRetryCount )
				{
					Log( "파일 다운로드에 실패했습니다." );

					KMsgBox::Open( "패치 파일 받기 실패",
						"다시 시도해 주십시오.",
						KMsgBox::MBOX_OK );


					return false;
				}
				::Sleep( MAX_DELAY );
				kHttp.Reset();
			}
			else // 다운로드에 성공했을 때
			{
				// 다운로드에 성공한 파일은 updateFileList.xml에서 제거한다.
				TiXmlNode* next = node->NextSibling( "Item" );
				fileElement->RemoveChild( node );
				node = next;
				xmlUpdateDoc.SaveFile();
				dwCurFileNum++;
				dwRetryCount = 0;

				m_strCurFileDesc = "";
			}
		}

		// updateFileList.xml을 제거한다.
		DeleteFileAbsolutely( "updateFileList.xml" );

#ifdef DMLEE_PATCHER_TEST
		KMsgBox::Close();
		KMsgBox::Open( "설치가 완료 되었습니다.",
			"\'실행\'버튼을 눌러 게임을 시작해주세요.",
			KMsgBox::MBOX_OK );
#endif // DMLEE_PATCHER_TEST
	}

#ifndef DMLEE_PATCHER_TEST
    // 다운로드 창을 가린다.
    OpenDownloadWindow( false );
#endif // DMLEE_PATCHER_TEST


	bool bOK = UpdateVersionFile(); // update Ver.xml

#ifdef DMLEE_PATCHER_TEST

	m_dwCurDownAllFileSize = m_dwDownAllFileSize;
	::InvalidateRect( m_hWnd, NULL, FALSE );

	if( true == bOK )
	{
		g_bPatchLaunchButtonEnabled = true;
	}
		
#endif

	return bOK;

#else
    KMsgBox::Close();
    return true;
#endif // SKIP_PATCH
}

/// 게임 클라이언트 프로그램을 실행시킨다.
/// @param void
/// @return 게임 클라이언트 실행에 성공하거나 유저가 거부하면 true, 실행에 실패하면 false
bool KPatcher::Launch( void ) const
{
	WCHAR wcharBuf[3000] = {0,};
	MultiByteToWideChar( CP_ACP, 0, m_strProgramPath.c_str(), -1, wcharBuf, 3000 );
    ::SetCurrentDirectory( wcharBuf );

    std::string strCmdLine( m_strProgramPath + "X2.exe " );

	if( __argv[1] != NULL )
		strCmdLine.append( __argv[1] );

    // 게임 클라이언트를 실행시킨다.
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { 0, };
    si.cb = sizeof( STARTUPINFO );

	WCHAR wcharBuf2[3000] = {0,};
	MultiByteToWideChar( CP_ACP, 0, strCmdLine.c_str(), -1, wcharBuf, 3000 );
	MultiByteToWideChar( CP_ACP, 0, m_strProgramPath.c_str(), -1, wcharBuf2, 3000 );

    if ( FALSE == ::CreateProcess( NULL, const_cast<WCHAR*>(wcharBuf ),
                                   NULL, NULL, FALSE, 0, NULL,
                                   wcharBuf2, &si, &pi ) )
    {
        Log( "엘소드 실행에 실패했습니다." );

        return false;
    }
    Log( "엘소드 실행에 성공했습니다." );

    return true;
}

/// 패치 프로그램을 종료시킨다.
/// @param void
/// @return void
void KPatcher::Exit( void ) const
{
    ::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );

    Log( "패치 프로그램을 종료합니다." );
}