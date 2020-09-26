// X2ScriptPreprocessingTool.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//



#include <StdAfx.h>


#if defined(DEBUG) || defined(_DEBUG)
    #pragma comment( lib, "luaLibD.lib" )
#else
	#pragma comment( lib, "luaLib.lib" )
#endif


CKTDXApp*   g_pKTDXApp = NULL;
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

void    _ErrorFile( const wchar_t* pwszFile )
{
    FILE* pFile = ( g_pFile != NULL ) ? g_pFile : stderr;
    fwprintf( pFile, L"\"%s\" ", pwszFile );
}


bool    _SaveFile( const wchar_t* pwszFilename, std::vector<BYTE>& vecByte )
{
    KFileSerializer kFileSave;
    kFileSave.Swap( vecByte );
    HRESULT hr = kFileSave.SaveFile( pwszFilename );
    return  SUCCEEDED( hr );
}


int _tmain(int argc, _TCHAR* argv[])
{
    int iErrorCode = 1;

    g_pKTDXApp = new CKTDXApp;
    //char aszError[MAX_PATH];
    wchar_t awszError[1024];

    std::wstring    wstrLog;
    std::vector<std::wstring>   vecExec;
    std::vector<std::wstring>   vecTET;
    std::vector<std::wstring>   vecXET;

    for( int i = 1; i < argc; ++i )
    {
        if ( _wcsicmp( argv[i], L"-exec" ) == 0 && i + 1 < argc )
        {
            vecExec.push_back( argv[i+1] );
            ++i;
        }
        else if ( _wcsicmp( argv[i], L"-log" ) == 0 && i + 1 < argc )
        {
            wstrLog = argv[i+1];
            ++i;
        }
        else if ( argv[i][0] != 0 )
        {
	        wchar_t awszExt[_MAX_EXT];
	        _wsplitpath_s( argv[i], NULL, 0, NULL, 0, NULL, 0, awszExt, _MAX_EXT );
            if ( _wcsicmp( awszExt, L".xet" ) == 0 )
            {
                vecXET.push_back( argv[i] );
            }
            else if ( _wcsicmp( awszExt, L".tet" ) == 0 )
            {
                vecTET.push_back( argv[i] );
            }
            else
            { 
                swprintf_s( awszError, ARRAY_SIZE(awszError), L"unsupported file format: \"%s\"", argv[i] );
                ErrorLogMsg( XEM_ERROR146, awszError );
                goto out;
            }
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
        bool bRet = g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncrypt( (const char*)pFile, dwSize );
        if ( bRet == false )
        {
            swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot execute: -exec \"%s\"", vecExec[u].c_str() );
            ErrorLogMsg( XEM_ERROR146, awszError );
            goto out;
        }
    }//for

    iErrorCode = 0;

    {

        CKTDXDeviceTET_Preprocessing    tetPreprocessing;
        CKTDXDeviceXET_Preprocessing    xetPreprocessing;
        std::vector<BYTE>   vecbData;
        BOOST_FOREACH( const std::wstring& wstrTET, vecTET )
        {
            tetPreprocessing.Release();
            vecbData.resize( 0 );

            {
                KFileSerializer kReadFile;
                HRESULT hr = kReadFile.LoadFile( wstrTET.c_str() );
                if ( FAILED( hr ) )
                {
                    _ErrorFile( wstrTET.c_str() );
                    iErrorCode = 1;
                    continue;
                }
                kReadFile.Swap( vecbData );
            }
            if ( vecbData.empty() == true )
            {
                _ErrorFile( wstrTET.c_str() );
                iErrorCode = 1;
                continue;
            }
            bool    bOK = tetPreprocessing.OpenFile( &vecbData[0], (int) vecbData.size(), L"" );
            if ( bOK == false )
            {
                _ErrorFile( wstrTET.c_str() );
                iErrorCode = 1;
                continue;
            }
            vecbData.resize( 0 );
            tetPreprocessing.Swap( vecbData );
            bOK = _SaveFile( wstrTET.c_str(), vecbData );
            if ( bOK == false )
            {
                _ErrorFile( wstrTET.c_str() );
                iErrorCode = 1;
            }
        }

        BOOST_FOREACH( const std::wstring& wstrXET, vecXET )
        {
            xetPreprocessing.Release();
            int     iSize = 0;
            bool    bOK = xetPreprocessing.OpenFile( wstrXET.c_str(), iSize );
            if ( bOK == false )
            {
                _ErrorFile( wstrXET.c_str() );
                iErrorCode = 1;
                continue;
            }
            vecbData.resize( 0 );
            xetPreprocessing.Swap( vecbData );
            bOK = _SaveFile( wstrXET.c_str(), vecbData );
            if ( bOK == false )
            {
                _ErrorFile( wstrXET.c_str() );
                iErrorCode = 1;
            }
        }

    }

out:

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
