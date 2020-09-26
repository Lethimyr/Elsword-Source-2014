#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#pragma comment( lib, "ws2_32" )

#include "KncRobotManager.h"
//#include "KncRobotThreadManager.h"
//#include "TRUserManager.h"

BOOL CtrlHandler( DWORD dwCtrlType ) 
{ 
    switch ( dwCtrlType ) 
    {
    // Handle the CTRL+C signal.
    case CTRL_C_EVENT:
        MessageBox( NULL, L"Ctrl + C", L"¸Þ·Õ", MB_OK );
        return FALSE;

    // CTRL+CLOSE: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
        return FALSE;

    // Pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    default:
        return FALSE;
    }
}

void main()
{
    if( !SetConsoleCtrlHandler( ( PHANDLER_ROUTINE )CtrlHandler, TRUE ) )
    {
        return;
    }

	//SiKTRUserManager()->Init();
	//SiKTRUserManager()->BeginThread();
	//SiKTRUserManager()->lua_inter();
	//SiKTRUserManager()->EndThread();
	//SiKTRUserManager()->ShutDown();

    //SiKncRobotThreadManager()->Init();
    //SiKncRobotThreadManager()->Run( true );
    //SiKncRobotThreadManager()->ShutDown();

	SiKncRobotManager()->Init();
	SiKncRobotManager()->Run( true );
	SiKncRobotManager()->ShutDown();

    /*
    KNetAddress kInfo;
    kInfo.m_strIP = L"59.25.186.176";
    kInfo.m_usPort = 9410;

    SiKncClient()->Connect( kInfo );

    system( "pause" );

    //KEGS_VERIFY_ACCOUNT_REQ kPacket;
    //kPacket.m_strUserID = L"florist";
    //kPacket.m_strPasswd = L"florist_";
    //SiKncClient()->Send( EGS_VERIFY_ACCOUNT_REQ, kPacket );

	KEGS_CONNECT_REQ kPacket_Connect;
	kPacket_Connect.m_ucMainVersion		= 0;
	kPacket_Connect.m_ucPatchVersion	= 0;
	kPacket_Connect.m_ucSubVersion		= 0;
	SiKncClient()->SendPacket( EGS_CONNECT_REQ, kPacket_Connect );

    system( "pause" );

	//KEGS_NEW_USER_JOIN_REQ kPacket_JoinUser;
	//kPacket_JoinUser.m_wstrID		= L"demiahun";
	//kPacket_JoinUser.m_wstrPassword	= L"2589";
	//kPacket_JoinUser.m_wstrName		= L"ÀåÈÆ";
	//kPacket_JoinUser.m_wstrNickName	= L"Å©¸£¸ª~ÈÆ¾²";
	//
	//SiKncClient()->Send( EGS_NEW_USER_JOIN_REQ, kPacket_JoinUser );

	KEGS_VERIFY_ACCOUNT_REQ kPacket_Account;
	kPacket_Account.m_strUserID	= L"demiahun";
	kPacket_Account.m_strPasswd	= L"2589";

	SiKncClient()->SendPacket( EGS_VERIFY_ACCOUNT_REQ, kPacket_Account );

	system( "pause" );

	SiKncClient()->SendID( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );

	//system( "pause" );

	//KEGS_CREATE_UNIT_REQ kPacket_CreateUnit;
	//kPacket_CreateUnit.m_iClass			= 2; //¾Æ¸£¸Þ
	//kPacket_CreateUnit.m_wstrNickName	= L"Å©¸£¸ª~¾Ë¸Þ";

	//SiKncClient()->Send( EGS_CREATE_UNIT_REQ, kPacket_CreateUnit );

	system( "pause" );

	//KEGS_DELETE_UNIT_REQ kPacket_DelUnit;
	//kPacket_DelUnit.m_iUnitUID = 5;//SiKncClient()->m_iUnitUID;

	//SiKncClient()->Send( EGS_DELETE_UNIT_REQ, kPacket_DelUnit );

	//system( "pause" );

    KncClient::ReleaseInstance();

    system( "pause" );
*/
}
