// DevSideAutomaion.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "DevSideAutomaion.h"
#include "TinyXml/tinyxml.h"

#include <DbgHelp.h>
#pragma comment( lib, "Dbghelp.lib" )

#include "IniReader.h"
#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

std::map<char,int> mapHexToDec; //16진수 한글 주소를 10진수로 바꾸기위한 map

void InitHex()
{  
    mapHexToDec['0'] = 0;
    mapHexToDec['1'] = 1;
    mapHexToDec['2'] = 2;
    mapHexToDec['3'] = 3;
    mapHexToDec['4'] = 4;
    mapHexToDec['5'] = 5;
    mapHexToDec['6'] = 6;
    mapHexToDec['7'] = 7;
    mapHexToDec['8'] = 8;
    mapHexToDec['9'] = 9;
    mapHexToDec['A'] = 10;
    mapHexToDec['B'] = 11;
    mapHexToDec['C'] = 12;
    mapHexToDec['D'] = 13;
    mapHexToDec['E'] = 14;
    mapHexToDec['F'] = 15;
}

std::string DecodeURL(std::string strURL)
{
    std::string strURLConvert;

    for( std::string::iterator sit = strURL.begin() ; sit != strURL.end() ; ++sit )
    {
        //UTF-8문자는 % 뒤에 두 문자이므로 아래와같이 연산한다.
        if( *sit == '%' )
        {
            char cRes = 0;
            cRes = mapHexToDec[*(++sit)] * 16;
            cRes += mapHexToDec[*(++sit)];
            strURLConvert.append( 1, cRes );
        }
        else
        {
            strURLConvert.append( 1, *sit );
        }
    }

    return strURLConvert;
}

bool GetTimeStamp( IN const char* strSourceFile, OUT std::string& strTimeStamp )
{
    HANDLE hFile;
    HANDLE hFMap;

    CFileStatus fileStatus;
    if( !CFile::GetStatus( strSourceFile, fileStatus ) )
        return false;

    hFile = CreateFile( fileStatus.m_szFullName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile == INVALID_HANDLE_VALUE )
    {
        return false;
    }//end if
    
    // 파일 맵핑 오브젝트를 만든다.
    hFMap = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, 0, NULL );
    // 주소 공간에 맵한다.
    HMODULE hModule;
    hModule = ( HMODULE )MapViewOfFile( hFMap, FILE_MAP_READ, 0, 0, 0 );
    CTime time( GetTimestampForLoadedLibrary( hModule ) );

    strTimeStamp = time.Format( "%y%m%d_%H%M" );

    UnmapViewOfFile( hModule );
    CloseHandle( hFMap );
    CloseHandle( hFile );
    return true;
}

#define SECTION argv[1]
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{       
    // MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    {
        // TODO: 오류 코드를 필요에 따라 수정합니다.
        _tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
        return 1;
    }
    if( argc != 2 )
        return 1;
    
    InitHex();

    CIniReader iniReader( ".\\GetTagVersion.ini" );

    char CurrentDirectory[MAX_PATH] = {0};
    GetCurrentDirectoryA( MAX_PATH, CurrentDirectory );
    
    //ini에서 가변적인 데이터 읽어오기(파일이름, 폴더이름, SVNURL이름..)
    std::string strSourceFile = CurrentDirectory;
    std::string strSVNURL;
    std::string strNowFolder;
    strSourceFile += "\\";
    strSourceFile += iniReader.ReadString( SECTION, "SourceFolder_0", NULL );
    strSourceFile += iniReader.ReadString( SECTION, "SourceFile_0", NULL );
    strSourceFile += ".exe";
    strSVNURL     = iniReader.ReadString( SECTION, "SVNURL", NULL ); //svn depot 주소..제일 상위까지    
    
    //tag얻기...
    std::string strTimeStamp;
    if( !GetTimeStamp( strSourceFile.c_str(), strTimeStamp ) )
        return 1;

    std::stringstream stm;

    stm << "@echo off\n";

    int nFileNum = iniReader.ReadInteger( SECTION, "FileNum", 0 );
    int i;
    for( i = 0 ; i < nFileNum ; ++i )
    {
        char szfolder[256];
        char szfile[256];
        sprintf( szfolder,"SourceFolder_%d", i );
        sprintf( szfile,"SourceFile_%d", i );

        stm << "if not exist " << CurrentDirectory << "\\" 
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".exe " << "goto _noexist" << "\n";

        stm << "if not exist " << CurrentDirectory << "\\" 
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".pdb " << "goto _noexist" << "\n";
    }

    stm << "echo 일괄작업 스크립트가 동작합니닷!" << "\n"
        << "echo 중단하실려면 Ctrl + C 를 눌러 멈추세염 -ㅅ-/" << "\n"
        << "pause" << "\n";

    for( i = 0 ; i < nFileNum ; ++i )
    {
        char szfolder[256];
        char szfile[256];
        char szDestFolder[256];
        sprintf( szfolder,"SourceFolder_%d", i );
        sprintf( szfile,"SourceFile_%d", i );
        sprintf( szDestFolder, "DestFolder_%d", i);

        stm << "copy " << CurrentDirectory << "\\" 
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".exe" <<" " << iniReader.ReadString( SECTION, szDestFolder, NULL ) << "\n";
    }

    //xml 만들어서 현재 작업 svn폴더 알아내기..
    std::string strXMLPath;     //xml 파일 이름 경로    
    strXMLPath.clear();    
    char cAddXML[] = "\\svn.xml";
    strXMLPath.append( CurrentDirectory, strlen(CurrentDirectory) );
    strXMLPath.append( cAddXML, sizeof(cAddXML) );
     
    //xml에서 필요한 데이터 뽑아오기...url, revision
    TiXmlDocument docXML;       
    TiXmlNode* nodeInfo;   
    TiXmlNode* nodeEntry;
    TiXmlNode* nodeURL;

    std::string strRevision;
    std::string strURL;            //url 읽는 변수
    std::string strURLConvertBAT;  //읽은 URL의 %를 %%로 바꾸는 변수..bat에서 %인식을 위해...
    strURLConvertBAT.clear();
    strRevision.clear();
    strURL.clear();
    
    docXML.LoadFile( strXMLPath );
    nodeInfo  = docXML.FirstChild( "info" );
    nodeEntry = nodeInfo->FirstChild( "entry" );
    nodeURL   = nodeEntry->FirstChild( "url" );
       
    strRevision.append( nodeEntry->ToElement()->Attribute( "revision" ) );
    strURL.append( nodeURL->FirstChild()->Value() );
    strURL.append( "/" );
    strURLConvertBAT = strURL;
    boost::replace_all( strURLConvertBAT, "%", "%%" );    
    
    int iFolderLoc = strURLConvertBAT.rfind( "/", strURLConvertBAT.length() - 2 );

    for(iFolderLoc += 1 ; iFolderLoc < strURLConvertBAT.length() - 1 ; ++iFolderLoc )
    {
        strNowFolder += strURLConvertBAT[iFolderLoc];
    }

    ////////////////////////////////////////////////////////////

    //실제로 commit을 하기위한 과정을 stream에 쓴다.
    //과정
    /*
    1. 원본폴더 exe를 -> 사내 폴더로 copy
    2. SVN경로 + tag로 tag에 만들 폴더 생성
    3. 원본(작업폴더) -> tag로 copy
    4. 원본폴더 commit
    5. tag/Game..._0701../trunk로 이동(switch)
    6. exe, pdb add 후 -> UTF-8로 commit
    7. 원본 폴더로 switch
    8. out..
    */

    stm << "@echo on" << "\n"
        << "echo Tag Version : "<< SECTION << "_"<< strTimeStamp.c_str() << "\n" // <--tag버전 기록 ex)GAMESERVER_070102_0612
        << "svn mkdir " << strSVNURL << "/tags/" << SECTION << "_"<< strTimeStamp.c_str() << " --message \"태그 저장을 위한 폴더 생성\"" << "\n"
        << "svn copy "<< strURLConvertBAT << " " << strSVNURL << "/tags/" << SECTION << "_"<< strTimeStamp.c_str() << " --message \"현재폴더 에서 tag로 파일 복사\"" << "\n"
        << "svn commit " << "--message \"Now Commit...\" " << "\n"
        << "svn switch " << strSVNURL << "/tags/" << SECTION << "_"<< strTimeStamp.c_str() << "/" << strNowFolder << "\n";
    
    std::string strDecodedURL = DecodeURL( strURL );

    std::ofstream ofstmFile;
    ofstmFile.open("Comment.txt", std::ios::binary | std::ios::out );
    char cBom[3];
    cBom[0] = char(0xEF);
    cBom[1] = char(0xBB);
    cBom[2] = char(0xBF);

    ofstmFile.write( cBom, 3);
    ofstmFile << "pdb, exe Commit" << "\r\n";
    ofstmFile << "URL : " << strDecodedURL << "\r\n";
    ofstmFile << "Revision : " << strRevision;
    ofstmFile.close();

    for( i = 0 ; i < nFileNum ; ++i )
    {
        char szfolder[256];
        char szfile[256];
        sprintf( szfolder,"SourceFolder_%d", i );
        sprintf( szfile,"SourceFile_%d", i );

        stm << "svn add "            
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()            
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".exe" << "\n";

        stm << "svn add "            
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()            
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".pdb" << "\n";
    }

    // 070207. kkurrung. 태그 생성시 코멘트 수정 가능하도록 notepad로 연다.
    stm << "notepad comment.txt" << "\n";
    stm << "svn commit --encoding UTF-8 --file Comment.txt" << "\n" //UTF-8로 인코딩해서 커밋..
        << "svn switch " << strURLConvertBAT << "\n"
        << ":Repeat_0 "<< "\n"
        << "del Comment.txt" << "\n"
        << "if exist Comment.txt goto Repeat_0" << "\n"
        << ":Repeat_1 "<< "\n"
        << "del svn.xml" << "\n"
        << "if exist svn.xml goto Repeat_1" << "\n"
        << "goto _out" << "\n"
        << ":_noexist" << "\n"
        << "echo 파일이 존재하지 않습니다." << "\n"
        << "goto _out" << "\n"
        << ":_out" << "\n"
        << "echo 종료합니다." << "\n"
        << "pause" << "\n"
        << std::endl;

    std::cout << stm.str();

    return 0;
}
