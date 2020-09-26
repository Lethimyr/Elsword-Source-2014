#include <WinSock2.h>
#include <iostream>

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "odbc32" )
#include "vld.h"

#include "CenterServer.h"
//
//BOOL CALLBACK Handler( DWORD dwEvent )
//{
//    Sleep(2000);
//    return TRUE;
//}
//
//void main()
//{
//    SetConsoleCtrlHandler( Handler,TRUE );
//
//    KBaseServer* pkServer = SiKCnServer();
//
//    if( pkServer->Init( L"config_cn" ) )
//    {
//        pkServer->Run( true );
//    }
//
//    pkServer->ShutDown();
//
//    KCnServer::ReleaseInstance();
//}
