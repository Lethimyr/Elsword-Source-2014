#include "StdAfx.h"


#include ".\x2statejoin.h"
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
#include ".\x2stateOption.h"
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER


CX2StateJoin::CX2StateJoin(void)
{
	m_pDLGJoinBack		= new CKTDGUIDialog( this, L"DLG_Join_Back.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGJoinBack );

	m_pDLGJoinFront		= new CKTDGUIDialog( this, L"DLG_Join_Front.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGJoinFront );

#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	m_bServerG			= true;
	m_bServerS			= false;
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
}

CX2StateJoin::~CX2StateJoin(void)
{
	SAFE_DELETE_DIALOG( m_pDLGJoinBack );
	SAFE_DELETE_DIALOG( m_pDLGJoinFront );

	g_pData->DeleteServerProtocol();
}

bool CX2StateJoin::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
		case SJUCM_MEMBER_JOIN:
			{
				return ServerConnectReq( hWnd, uMsg, wParam, lParam );
			}
			break;

		case SJUCM_BACK_TO_LOGIN:
			{
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_LOGIN, NULL, false );
				return true;
			}
			break;
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
		case CX2StateOption::STATE_OPTION_UI_CUSTOM_MSG::SOUCM_SERVER_G:
			{
				m_bServerG = true;
				m_bServerS = false;
			}
			break;

		case CX2StateOption::STATE_OPTION_UI_CUSTOM_MSG::SOUCM_SERVER_S:
			{
				m_bServerG = false;
				m_bServerS = true;
			}
			break;
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	}

	return false;
}

bool CX2StateJoin::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
		case EGS_CONNECT_ACK:
			{
				return ServerConnectAck( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_NEW_USER_JOIN_ACK:
			{
				return NewUserJoinAck( hWnd, uMsg, wParam, lParam );
			}
			break;
	}
	return false;
}
/*

bool CX2StateJoin::UIServerTimeOutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{   	
		case EGS_CONNECT_ACK:
			{
				return ServerConnectTimeOutAck( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_NEW_USER_JOIN_ACK:
			{
				return NewUserJoinTimeOutAck( hWnd, uMsg, wParam, lParam );
			}
			break;
	}
	return false;
}
*/



bool CX2StateJoin::ServerConnectReq( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CKTDGUIIMEEditBox*	pIDBox			= NULL;
	CKTDGUIEditBox*		pPasswordBox	= NULL;
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	CKTDGUIEditBox*	pNameBox		= NULL;
	CKTDGUIRadioButton* pServerG	= NULL;
	CKTDGUIRadioButton* pServerS	= NULL;
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	CKTDGUIIMEEditBox*	pNameBox		= NULL;
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	
	CKTDGUIIMEEditBox*	pNickNameBox	= NULL;
	pIDBox			= (CKTDGUIIMEEditBox*)m_pDLGJoinBack->GetControl( L"IMEEditBoxJoinID" );
	pPasswordBox	= (CKTDGUIEditBox*)m_pDLGJoinBack->GetControl( L"EditBoxJoinPassword" );
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	pNameBox		= (CKTDGUIEditBox*)m_pDLGJoinBack->GetControl( L"IMEEditBoxJoinName" );
	pServerG		= (CKTDGUIRadioButton*)m_pDLGJoinBack->GetControl( L"low" );
	pServerS		= (CKTDGUIRadioButton*)m_pDLGJoinBack->GetControl( L"middle" );
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	pNameBox		= (CKTDGUIIMEEditBox*)m_pDLGJoinBack->GetControl( L"IMEEditBoxJoinName" );
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	pNickNameBox	= (CKTDGUIIMEEditBox*)m_pDLGJoinBack->GetControl( L"IMEEditBoxJoinNickName" );

	m_UserID			= pIDBox->GetText();
	m_UserPassword		= pPasswordBox->GetText();
	m_UserName			= pNameBox->GetText();
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	m_UserNickName		= pNickNameBox->GetText();
	//m_bServerG			= pServerG->GetChecked();
	//m_bServerS			= pServerS->GetChecked();
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	//m_UserNickName		= pNickNameBox->GetText();
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER

	if( JoinErrorCheck() == true )
	{
		//접속시도
		g_pData->DeleteServerProtocol();
		g_pData->ResetServerProtocol();

//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//		g_pData->GetGameUDP()->SetServerProtocol( g_pData->GetServerProtocol() );
//#endif//X2OPTIMIZE_TCP_RELAY_TEST

// 09.07.03 태완 : 아이디 생성 사내에서는 가능하게 수정
#if !defined(_SERVICE_) || defined (SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER)
// 		KEVENT_X2_SERVER_CONNECT_REQ kEVENT_X2_SERVER_CONNECT_REQ;
// 		kEVENT_X2_SERVER_CONNECT_REQ.gameMainVersion	= 0;
// 		kEVENT_X2_SERVER_CONNECT_REQ.gameSubVersion		= 0;
// 		kEVENT_X2_SERVER_CONNECT_REQ.gamePatchVersion	= 0;

		//{{ dmlee 2009.5.13 fix!! 채널서버 접속 방식으로 바뀌면서 임시로 막아놓음, 나중에 수정해야함
		if( g_pData->GetServerProtocol()->ConnectedToGameServer( g_pMain->GetGameServerIPForCreateID().c_str(), g_pMain->GetGameServerPortForCreateID() ) == true )
		{
			KEGS_CONNECT_REQ kPacket;
			kPacket.m_wstrVersion = g_pMain->GetClientVersion();

			g_pData->GetServerProtocol()->SendPacket( EGS_CONNECT_REQ, kPacket );
			g_pMain->AddServerPacket( EGS_CONNECT_ACK );

			m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_647 ), this );
		}
		else
		{
			m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_648 ), this );
		}
		//}} dmlee 2009.5.13 fix!! 채널서버 접속 방식으로 바뀌면서 임시로 막아놓음, 나중에 수정해야함
#endif // !_SERVICE_ || SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
		
	}
	return true;
}

bool CX2StateJoin::ServerConnectAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_CONNECT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//g_pData->GetServerProtocol()->SetConnectAccept();
			NewUserJoinReq( NULL,NULL,NULL,NULL );
		}
		else
		{


			g_pData->DeleteServerProtocol();
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_648 ), this );
		}
	}
	return true;
}
/*
bool CX2StateJoin::ServerConnectTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	g_pData->DeleteServerProtocol();

	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_648 ), this );
	return true;
}
*/

bool CX2StateJoin::NewUserJoinReq( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KEGS_NEW_USER_JOIN_REQ	kEGS_NEW_USER_JOIN_REQ;
	kEGS_NEW_USER_JOIN_REQ.m_wstrID			= m_UserID;
	kEGS_NEW_USER_JOIN_REQ.m_wstrPassword	= m_UserPassword;
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	kEGS_NEW_USER_JOIN_REQ.m_wstrName		= m_UserNickName;

	if (m_bServerG && !m_bServerS)
	{
		kEGS_NEW_USER_JOIN_REQ.m_byteGuestUser = 1;
	}
	else if(!m_bServerG && m_bServerS)
	{
		kEGS_NEW_USER_JOIN_REQ.m_byteGuestUser = 0;
	}
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	kEGS_NEW_USER_JOIN_REQ.m_wstrName		= m_UserName;
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
//	kEGS_NEW_USER_JOIN_REQ.m_wstrNickName	= m_UserNickName;

	g_pData->GetServerProtocol()->SendPacket( EGS_NEW_USER_JOIN_REQ, kEGS_NEW_USER_JOIN_REQ );
	g_pMain->AddServerPacket( EGS_NEW_USER_JOIN_ACK );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_649 ), this );
	return true;
}

bool CX2StateJoin::NewUserJoinAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NEW_USER_JOIN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_NEW_USER_JOIN_ACK ) == true )
	{
		if(  g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_650 ), this );

#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
			// 가입 성공 시 Login 페이지로 보낸다
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_LOGIN, NULL, false );
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
		}

	}
//	g_pData->DeleteServerProtocol();
	return true;
}

//bool CX2StateJoin::NewUserJoinTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"서버에 응답이 없습니다", this );
//	g_pData->DeleteServerProtocol();
//	return true;
//}











bool CX2StateJoin::JoinErrorCheck()
{
	string multyByteUserID;
	ConvertWCHARToChar( multyByteUserID, m_UserID.c_str() );

	string multyByteUserPassword;
	ConvertWCHARToChar( multyByteUserPassword, m_UserPassword.c_str() );

	string multyByteUserName;
	ConvertWCHARToChar( multyByteUserName, m_UserName.c_str() );

#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	string multyByteUserNickName;
	ConvertWCHARToChar( multyByteUserNickName, m_UserNickName.c_str() );
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	//string multyByteUserNickName;
	//ConvertWCHARToChar( multyByteUserNickName, m_UserNickName.c_str() );
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER

	if( multyByteUserID.size() < 4 || multyByteUserID.size() > 20 )
	{
		//ID를 4자 미만으로 입력한 경우
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_651 ), this );
	}
	else if( multyByteUserPassword.size() < 4 || multyByteUserPassword.size() > 20 )
	{
		//Password를 4자 미만으로 입력한 경우
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_652 ), this );
	}
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	else if( multyByteUserName.size() < 4 || multyByteUserName.size() > 20 )
	{
		//Password를 4자 미만으로 입력한 경우
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_652 ), this ); // 비밀번호 재입력 범위가 잘못된 경우
	}
	else if(strcmp(multyByteUserPassword.c_str(),multyByteUserName.c_str()) != 0 )
	{
		//Password를 4자 미만으로 입력한 경우
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_653 ), this); //GET_STRING( STR_ID_653 ) // 비밀번호와 비밀번호 확인이 다른 경우
	}
//	else if( multyByteUserNickName.size() < 1 || multyByteUserNickName.size() > 25 )
//	{
//		//캐릭터 ID를 2자 미민으로 입력한 경우
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_653 ), this ); // 캐릭터 ID 입력범위가 잘못된 경우
//	}
	else if(!m_bServerG && !m_bServerS)
	{

	}
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	else if( multyByteUserName.size() < 1 || multyByteUserName.size() > 16 )
	{
		//Password를 4자 미만으로 입력한 경우
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_653 ), this );
	}
	//else if( multyByteUserNickName.size() < 1 || multyByteUserNickName.size() > 16 )
	//{
	//	//Password를 4자 미만으로 입력한 경우
	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_654 ), this );
	//}
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	else
	{
		return true;
	}
	return false;
}

