// X2ScriptPreprocessingTool.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//



#include <StdAfx.h>

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "luaLibD.lib" )
#else
#pragma comment( lib, "luaLib.lib" )
#endif


CKTDXApp*   g_pKTDXApp = NULL;
CX2Data*    g_pData = NULL;
FILE*       g_pFile = NULL;

void    _ErrorLogMsg( int errnum, const char* pszMsg, const char* pszFile, const char* pszFunction, int line )
{
    FILE* pFile = ( g_pFile != NULL ) ? g_pFile : stderr;
    fprintf( pFile, "ErrorEnum( %d ), ErrorMsg( \"%s\" ), %s, %s, %d\n", errnum, pszMsg, pszFile, pszFunction, line );
}

void    _ErrorLogMsg( int errnum, const wchar_t* pwszMsg, const char* pszFile, const char* pszFunction, int line )
{
    FILE* pFile = ( g_pFile != NULL ) ? g_pFile : stderr;
    fwprintf( pFile, L"ErrorEnum( %d ), ErrorMsg( \"%s\" ), %S, %S, %d\n", errnum, pwszMsg, pszFile, pszFunction, line );
}

int _tmain(int argc, _TCHAR* argv[])
{
    int iErrorCode = 1;

    g_pKTDXApp = new CKTDXApp;
    g_pData = new CX2Data;
    char aszError[MAX_PATH];
    wchar_t awszError[1024];

    std::wstring    wstrOut;
    std::wstring    wstrLog;
    std::vector<std::wstring>   vecExec;
    std::vector<std::wstring>   vecIn;

    for( int i = 1; i < argc; ++i )
    {
        if ( _wcsicmp( argv[i], L"-exec" ) == 0 && i + 1 < argc )
        {
            vecExec.push_back( argv[i+1] );
            ++i;
        }
        else if ( _wcsicmp( argv[i], L"-in" ) == 0 && i + 1 < argc )
        {
            vecIn.push_back( argv[i+1] );
            ++i;
        }
        else if ( _wcsicmp( argv[i], L"-out" ) == 0 && i + 1 < argc )
        {
            wstrOut = argv[i+1];
            ++i;
        }
        else if ( _wcsicmp( argv[i], L"-log" ) == 0 && i + 1 < argc )
        {
            wstrLog = argv[i+1];
            ++i;
        }
    }

    if ( wstrLog.empty() == false )
    {
        g_pFile = _wfopen( wstrLog.c_str(), L"w" );
        if ( g_pFile == NULL )
        {
            ErrorLogMsg( XEM_ERROR146, "CANNOT_OPEN_LOG_FILE" );
            goto out;
        }
    }//if

    if ( wstrOut.empty() == true )
    {
        sprintf_s( aszError, ARRAY_SIZE(aszError), "missing: -out outputfile" );
        ErrorLogMsg( XEM_ERROR146, aszError );
        goto out;
    }
    if ( vecIn.empty() == true )
    {
        sprintf_s( aszError, ARRAY_SIZE(aszError), "missing: -in inputfile" );
        ErrorLogMsg( XEM_ERROR146, aszError );
        goto out;
    }


    for( unsigned u = 0; u != vecExec.size(); ++u )
    {
        KFileSerializer kFile;
        HRESULT hr = kFile.LoadFile( vecExec[u].c_str() );
        if ( FAILED( hr ) )
        {
            swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot open: -exec \"%s\"", vecExec[u].c_str() );
            ErrorLogMsg( XEM_ERROR146, awszError );
            goto out;
        }
        DWORD dwSize = kFile.GetSize();
        const void* pFile = kFile.AccessData( 0, dwSize );
        if( g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncrypt( (const char*)pFile, dwSize ) == false )
        {
            swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot execute: -exec \"%s\"", vecExec[u].c_str() );
            ErrorLogMsg( XEM_ERROR146, awszError );
            goto out;
        }
    }//for

    bool bOK = g_pData->GetItemManager()->OpenItemScriptFile( wstrOut.c_str(), vecIn );
    if ( bOK == false )
    {
        swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot generate: -out \"%s\"", wstrOut.c_str() );
        ErrorLogMsg( XEM_ERROR146, awszError );
        goto out;
    }
    
    iErrorCode = 0;

out:

    if ( g_pData != NULL )
    {
        delete  g_pData;
        g_pData = NULL;
    }
    if ( g_pKTDXApp != NULL )
    {
        delete  g_pKTDXApp;
        g_pKTDXApp = NULL;
    }
    if ( g_pFile != NULL )
    {
        fclose( g_pFile );
        g_pFile = NULL;
    }

	return iErrorCode;
}