/** @file   KFileVerifier.cpp
    @desc   implementation of KFileVerifier
*/
//
//#define _TOSTR1(x)    #x
//#define _TOSTR(x)     _TOSTR1(x)
//#include _TOSTR(PROJECT_PCH)


#include "StdAfx.h"
#include "KFileVerifier.h"
#include <sstream>
#include "tinyxml/tinyxml.h"
#include "CRC_32.h"
#include <strsafe.h>
//#include "KDebugConsole.h"

#undef super
#define super   KJohnThread


///-----------------------------------------------------------------------------
// Ver.xml 파일 helper functions
//
/// @brief  <Signature> element에 대한 포인터를 얻는다.
static const TiXmlElement* GetSignElement(const TiXmlDocument& xmlDoc_) // by parkch
{
    /* 
        <PatchInfo>
            <Signature>
                <Item Name="c07b9188" />
            </Signature>
    /**/

    const TiXmlNode* node = NULL;
    node = xmlDoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild( "Signature" );
    //if ( NULL == node )
    //{
    //    TiXmlElement item( "Signature" );
    //    xmlDoc_.ToElement()->InsertEndChild( item );
    //    node = xmlDoc_.FirstChild( "PatchInfo" )->FirstChild( "Signature" );
    //}//if
    ASSERT( node );
    return node->ToElement();
}//GetSignElement()


static const std::string GetFileListStr(const TiXmlDocument& xmlDoc_) // by parkch
{
    std::ostringstream xmlStream;
    xmlStream << xmlDoc_;

    /** 전체 .xml 문서에서 '<File>'로 시작하고, '</File>'로 끝나는 substring을 리턴한다.
        실제 substring은 "<File> ... <"(마지막 '/File>'은 제외)일 것이다.
        - jintaeks on 2008-01-05, 13:48 */
    std::string             strStream = xmlStream.str();
    std::string::size_type  beginPos = strStream.find( "<File>" );
    std::string::size_type  endPos   = strStream.find( "</File>" );

    if ( std::string::npos == beginPos || std::string::npos == endPos )
    {
        strStream.erase();
        return strStream;
    }//if

    return strStream.substr(beginPos, endPos);
}//GetFileListStr()


/// @brief  파일이 변경되지 않았는지 <Signature>에 명시된 checksum을 자체 테스트한다.
static bool Verify(const TiXmlDocument& xmlDoc_) // by parkch
{
    const TiXmlElement*   pxmlSignElement = GetSignElement( xmlDoc_ );
    std::string     strFileList     = GetFileListStr( xmlDoc_ );

    if ( strFileList.empty() ) return false;

    std::string strOrgSign = CRC_32::GetCRCFromString( strFileList );
    const TiXmlNode*  node = pxmlSignElement->FirstChild( "Item" );
    //if ( NULL == node )
    //{
    //    TiXmlElement item( "Item" );
    //    item.SetAttribute( "Name", "" );
    //    pxmlSignElement->InsertEndChild( item );
    //    node = pxmlSignElement->FirstChild( "Item" );
    //}//if
    ASSERT( node );
    std::string strCurSign = node->ToElement()->Attribute( "Name" );

    return (strOrgSign == strCurSign);
}//Verify()


///-----------------------------------------------------------------------------
// KFileVerifier members
//
// constructor.
KFileVerifier::KFileVerifier()
{
    m_szProgramPath[0] = NULL;
    //m_fileMap;      ///< 기존의 <File> 섹션에 명시된 파일의 리스트
    //m_komFileMap;   ///< 새롭게 <KomFile> 섹션에 명시된 파일의 리스트
    //m_hackFileArr;

    m_hThreadEvent = NULL;
    m_bThreadSuspended = false;
    //m_cs;
    m_dwThreadUserParam = 0L;
    m_fpPreEndThreadCallback = NULL;
    m_bForceExistThread = false;

    _InitializeObject();
}//KFileVerifier::KFileVerifier()


// destructor.
KFileVerifier::~KFileVerifier()
{
    // 종료되어 있지 않을 수 있는 thread를 강제로 종료시킨다.
    m_bForceExistThread = true;
    __super::EndThread( 3000 );
    _FinalizeObject(); // thread 내부에서 사용하는 object가 있으므로, thread 종료후에 호출해야 한다.

    Clear(); // container의 내용을 모두 clear한다.
}//KFileVerifier::~KFileVerifier()


///-----------------------------------------------------------------------------
// thread related functions
//
/*virtual*/ DWORD KFileVerifier::RunThread()
{
    ASSERT( m_hackFileArr.empty() );

    std::string     strChecksum;
    char            szFullPathFilename[1024];

    synchronized( m_cs )
    {
        FileInfoMap::const_iterator mitor = m_fileMap.begin();
        while ( mitor != m_fileMap.end() )
        {
            ::WaitForSingleObjectEx( m_hThreadEvent, INFINITE, FALSE );

			if( true == mitor->first.empty() )
			{
				StringCchPrintfA( szFullPathFilename, 1024, "%s", m_szProgramPath );
			}
			else
			{
				StringCchPrintfA( szFullPathFilename, 1024, "%s%s", m_szProgramPath, mitor->first.c_str() );
			}

            //BREAK( mitor->first == "data2.kom" );

            // file의 checksum을 구한다.
            strChecksum = CRC_32::GetCRCFromFile( szFullPathFilename );
            if ( m_bForceExistThread )
                break;
//{{ todo // 2008-1-17
            // TODO: 접속하자 마자 서버에 접속할 수 없습니다. 메시지 박스가 뜬 후
            // 게임이 종료하는 경우, 아래 문장에서 에러가 발생
            // - jintaeks on 2008-01-17, 13:53
            if ( strChecksum != mitor->second.m_strCheckSum )
//}} todo // 2008-1-17
            {
                m_hackFileArr.push_back( mitor->second );
            }//if

            ++mitor;
        }//while
    }//synchronized( m_cs )

    // thread를 탈출하기 전에 사용자가 설치한 콜백 함수가 있으면 호출한다.
    //
    if ( m_fpPreEndThreadCallback )
    {
        m_fpPreEndThreadCallback( m_dwThreadUserParam, this );
    }//if

    return 0L;
}//KFileVerifier::RunThread()


bool KFileVerifier::BeginThreadWithPreEndCB(
    PREENDTHREAD_CALLBACK   pPreEndThreadCallback_,
    DWORD                   dwUserParam_ /*=0L*/)
{
    // 'm_fpPreEndThreadCallback'를 설정한다.
    _SetPreEndThreadCallback( pPreEndThreadCallback_, dwUserParam_ );
    return __super::BeginThread( NULL, NULL );
        //                     ^ 반드시 NULL 파라미터를 전달해야 한다.
}//KFileVerifier::BeginThreadWithPreEndCB()


void KFileVerifier::_InitializeObject()
{
    ASSERT( m_hThreadEvent == NULL );
    m_hThreadEvent = ::CreateEvent( NULL, TRUE, TRUE, NULL );
    ASSERT( m_hThreadEvent );
    m_bThreadSuspended = false;
}//KFileVerifier::_InitializeObject()


void KFileVerifier::_FinalizeObject()
{
    if ( m_hThreadEvent != NULL )
    {
        if ( m_bThreadSuspended )
            ResumeThread();
        ::CloseHandle( m_hThreadEvent );
        m_hThreadEvent = NULL;
    }//if
}//KFileVerifier::_FinalizeObject()


void KFileVerifier::_SetPreEndThreadCallback(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_)
{
    m_fpPreEndThreadCallback = pPreEndThreadCallback_;
    m_dwThreadUserParam = dwUserParam_;
}//KFileVerifier::_SetPreEndThreadCallback()


void KFileVerifier::SuspendThread()
{
    ::ResetEvent( m_hThreadEvent );
    m_bThreadSuspended = true;
}//KFileVerifier::SuspendThread()


void KFileVerifier::ResumeThread()
{
    ::SetEvent( m_hThreadEvent );
    m_bThreadSuspended = false;
}//KFileVerifier::ResumeThread()


///-----------------------------------------------------------------------------
// KFileVerifier interfaces
//
bool KFileVerifier::LoadFileList(const char* pszFullPathFilename_)
{
    ASSERT( pszFullPathFilename_ );
    if ( pszFullPathFilename_ == NULL )
        return false; // 파일 이름이 지정되지 않음

    Clear();

    // 이 함수가 호출된 시점의 현재 폴더경로(게임 클라이언트의 폴더경로)를 구한다.
	GetCurrentDirectoryA( _MAX_PATH, m_szProgramPath );
	
	StringCchCatA( m_szProgramPath, _MAX_PATH, "\\" );
    //strcat( m_szProgramPath, "\\" );

    // .xml 파일을 읽는다.
    //
    TiXmlDocument   docLocal( pszFullPathFilename_ );
//{{ robobeg : 2008-01-17
    //docLocal.LoadFile();
    if ( !docLocal.LoadFile() )
        return false; // 파일을 로드하는데 실패함
//}} robobeg : 2008-01-17

    // Ods306에서 에러가 생겨서 잠시 주석처리하다.
    // 엘소드에서 사용할 경우 comment를 풀어 주어야 한다.
    // - seojt // 2008-1-9, 15:03
    //
    if ( !Verify( docLocal ) )
        return false; // .xml 파일의 <Signature>에 명시된 checksum이 틀림

    // <File> 부분을 찾는다.
    //
    TiXmlNode*      nodeLocal = NULL;
    TiXmlElement*   fileElementLocal = NULL;

    nodeLocal = docLocal.FirstChild( "PatchInfo" );
    ASSERT( nodeLocal );
    fileElementLocal = nodeLocal->FirstChild( "File" )->ToElement();
    ASSERT( fileElementLocal );

    /** <PatchInfo>
            <File>
                <Item ...>
        에 명시된 모든 element를 iterate 한다.
        - jintaeks on 2008-01-05, 14:10 */
    KFileInfo   fileInfo;
    int         iTemp;
    int         iDebugMapSize = m_fileMap.size();
    nodeLocal = fileElementLocal->FirstChild( "Item" );
    while ( nodeLocal != NULL )
    {
        // fileInfo를 구성한다.
        //
        fileInfo.m_strFilename = nodeLocal->ToElement()->Attribute( "Name" );
        nodeLocal->ToElement()->Attribute( "Size", &iTemp );
        fileInfo.m_dwSize = iTemp;
        nodeLocal->ToElement()->Attribute( "Version", &iTemp );
        fileInfo.m_usVersion = (USHORT)iTemp;
        fileInfo.m_strCheckSum = nodeLocal->ToElement()->Attribute( "CheckSum" );

        // 맵에 insert한다.
        //
        if ( !fileInfo.m_strCheckSum.empty() )
        {
            m_fileMap.insert( std::pair<std::string,KFileInfo>( fileInfo.m_strFilename,fileInfo) );
            ASSERT( (iDebugMapSize+1) == (int)m_fileMap.size() );
            iDebugMapSize = m_fileMap.size();
        }//if

        // sibling을 iterate한다.
        nodeLocal = nodeLocal->NextSibling( "Item" );
    }//while

    return true; // 리스트 구성 성공
}//KFileVerifier::LoadFileList()


void KFileVerifier::Clear()
{
    m_fileMap.clear();
    m_komFileMap.clear();
    m_hackFileArr.clear();
}//KFileVerifier::Clear()


///-----------------------------------------------------------------------------
// debug functions
//
void KFileVerifier::Debug_DumpToOutputWindow()
{
#ifndef _SERVICE_

	ChecksumLog( "[KFileVerifier::Debug_DumpToOutputWindow()\r\n" );
	debugPrintf( "[KFileVerifier::Debug_DumpToOutputWindow()\r\n" );

    // m_fileMap의 내용을 모두 dump한다.
    FileInfoMap::const_iterator mitor = m_fileMap.begin();
    while ( mitor != m_fileMap.end() )
    {
		stringstream strm;
		strm.str( "" );
		strm << "Name = " << mitor->first << ", Size = " << mitor->second.m_dwSize << ", Version = " << mitor->second.m_usVersion;
		strm << ", checksum = " << mitor->second.m_strCheckSum << std::endl;
		ChecksumLog( strm.str().c_str() );
		debugPrintf( "%s", strm.str().c_str() );

        ++mitor;
    }//while

#endif //_SERVICE_

}//KFileVerifier::Debug_DumpToOutputWindow()


void KFileVerifier::Debug_DumpHackFiles()
{
#ifndef _SERVICE_

	ChecksumLog( "[KFileVerifier::Debug_DumpHackFiles()\r\n" );
	debugPrintf( "[KFileVerifier::Debug_DumpHackFiles()\r\n" );

	// 잘못된 파일 목록
	for (int i=0; i < (int)m_hackFileArr.size(); ++i)
	{
		stringstream strm;
		strm.str( "" );
		strm << "HACK!!    Name = " << m_hackFileArr[i].m_strFilename << ", Size = " << m_hackFileArr[i].m_dwSize;
		strm << ", Version = " << m_hackFileArr[i].m_usVersion;
		strm << ", checksum = " << m_hackFileArr[i].m_strCheckSum << std::endl;
		ChecksumLog( strm.str().c_str() );
		debugPrintf( "%s", strm.str().c_str() );
	}//for

	if ( m_hackFileArr.empty() )
	{
		ChecksumLog( "  no hacked files\r\n" );
		debugPrintf( "  no hacked files\r\n" );

	}//if

#endif //_SERVICE_

}//KFileVerifier::Debug_DumpHackFiles()
