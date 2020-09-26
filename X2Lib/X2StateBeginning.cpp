
#include "StdAfx.h"

// 2013.02.27 조효진	현재 해당 코드 사용 안함. Beginning 쪽 사용하게 되면 인증 코드 다시 점검해야 함 
#ifdef ELSWORD_NEW_BEGINNING
#include ".\X2StateBeginning.h"
bool CX2StateBeginning::m_sbSelectedServerSet = false;

// dmlee 2009.03.11
// 기존의 로그인 과정은 로그인 -> 게임서버 접속 -> 계정인증 -> 로딩 100% -> 마을 진입
// 새로 바뀐 로그인 과정은 로그인 -> 로딩 100% -> 채널서버 접속 -> 게임서버선택 -> 게임서버접속 -> 계정인증 -> 마을

// 게임서버 접속 관련 패킷 순서
// connect_req( nexon passport를 갱신받는다) -> verify_account -> port check -> state change server select req -> current time req -> Handler_EGS_SELECT_SERVER_SET_REQ -> Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ


// 게임서버에 연결이 되면 채널서버 연결을 끊기 위한 REQ를 보낸다. 채널서버에 연결하기전에 게임서버 연결을 끊기 위한 REQ를 보낸다

namespace		_CONST_STATE_BEGINNING_
{
	const float		g_fTimeServerSelectDlgFadeStart		= 2.f;
	const float		g_fTimeServerSelectDlgFadeIn		= 1.f;
	const float		g_fTimeServerSelectWhiteIn			= 2.f;
	const float		g_fTimeChannelSelectWhiteOut		= 0.3f;
	const float		g_fTimeChannelSelectWhiteIn			= 0.4f;


}



CX2StateBeginning::CX2StateBeginning(void) :
m_pDLGServerSelectBack(NULL),
m_pDLGCheckHack(NULL),
//m_vecUnitSlot;
#ifndef ELSWORD_NEW_BEGINNING
m_pDLGUnitSelectBack(NULL),
#endif ELSWORD_NEW_BEGINNING
m_pDLGUnitSelectFront(NULL),
m_pDLGDeleteUnitCheck(NULL),
m_pDLGLoadingState(NULL),
m_pDLGRenameMsgBox(NULL),
m_pDLGRenameConfirmMsgBox( NULL ),
m_pDLGRenameSuccessMsgBox( NULL ),
m_pDLGTutorialMsgBox(NULL),
m_pDLGGoToFirstDungeon(NULL),
m_pDLGRecommend(NULL),
m_pDLGRecommendSuccess(NULL),
//m_UnitButtonList;
//m_UnitViewerUIList;
m_pSelectUnit(NULL),
m_pSelectButton(NULL),
m_bUnitClicked(false),
m_fMouseSensitivity(0.01f),
m_fRemainRotY(0.0f),
m_bRotateReturn(true),
m_fRotSpeed(false),
m_pOveredButton(NULL),
m_pOveredName(NULL),
m_bOneTimeInit(true),
m_bStateChangingToUnitCreate(false),
m_bPlayDownAnim(false),
m_fDownAnimTime(0.0f),
m_MaxUnitNum(0),
m_bReserveEnterTutorial(false),
m_bCheckRecommend(false),
m_bCheckRecommendSuccess(false),
m_NowPage(1),
m_MaxPage(1),		// 아래쪽에서 다시 연산
m_bPortCheckWait( false ),
m_fPortCheckWaitTime( 0.f ),
m_PortCheckRetryTime( 0 ),
m_bENX_USER_LOGIN_NOT( false ),
m_bEGS_VERIFY_ACCOUNT_ACK( false ),
m_bIsGameBang( false ),
m_bPortCheckReq( false ),
m_bEGS_VERIFY_ACCOUNT_REQ( false ),
m_fEGS_VERIFY_ACCOUNT_ACK_WaitTime( 0.f ),
m_bReceivedChannelList( false ),
m_bSentEGS_CONNECT_REQ( false ),
m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK( false ),
m_bReservedConnectionToChannelServer( false ), 
m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( false ),
m_iTryConnectChannelID( -1 ), 
m_bJustChangedNickName( false ), 
m_NickNameChangeUnitUID( -1 ),
m_NickNameChangeUnitNickName( L"" ),
m_bRemoveUnitAfterReceiveNMSerialNum( false )
//{{ kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
#ifdef	ADD_SERVER_GROUP
, m_pDLGChannelSelectBack( NULL )
, m_iChannelNum( 0 )
#endif	ADD_SERVER_GROUP
//}}  kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
#ifdef ELSWORD_NEW_BEGINNING
, m_eNowState(BG_STATE_SERVER_SELECT)
, m_bServerSelectDlgFadeIn(false)
, m_bServerSelectWhiteIn(false)
, m_fServerSelectDlgFadeInElapsedTime(0.f)
, m_bChannelSelectWhiteOut(false)
, m_fChannelSelectWhiteOutElapsedTime(0.f)
#endif ELSWORD_NEW_BEGINNING
{
#ifdef ELSWORD_NEW_BEGINNING
	// oasis907 : 김상윤 [2010.11.11] // 
	CreateGame();

	m_eNowState = BG_STATE_SERVER_SELECT;
#endif ELSWORD_NEW_BEGINNING

#ifndef NEW_SKILL_TREE
	g_pInstanceData->DeleteSkillTreeUI();
#endif


#ifdef ELSWORD_NEW_BEGINNING
	g_pBeginningGame->RenderLoadingFace(0);
#endif ELSWORD_NEW_BEGINNING

#ifndef ELSWORD_NEW_BEGINNING
#ifndef NEW_VILLAGE_UI
	m_pDLGUnitSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Back.lua" );
#else
	m_pDLGUnitSelectBack = new CKTDGUIDialog( this, L"DLG_Unit_Select_Back.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUnitSelectBack );	
#endif ELSWORD_NEW_BEGINNING


#ifdef ELSWORD_NEW_BEGINNING
	g_pBeginningGame->RenderLoadingFace(1);
#endif ELSWORD_NEW_BEGINNING

#ifdef NEW_VILLAGE_UI

#ifdef ELSWORD_NEW_BEGINNING
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Server_Chanal_Selection_TEST.lua" );
#else
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Server_Chanal_Selection.lua" );
#endif ELSWORD_NEW_BEGINNING

#else
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_Server_Select_Back.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGServerSelectBack );

#ifdef ELSWORD_NEW_BEGINNING



	m_pDLGServerSelectBack->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.0f));

	g_pData->GetEventScene()->BlackLayOutMove( false, 1.0f, false, true, 
											_CONST_STATE_BEGINNING_::g_fTimeServerSelectWhiteIn, 1.0f );
#endif ELSWORD_NEW_BEGINNING

#ifdef ELSWORD_NEW_BEGINNING
	g_pBeginningGame->RenderLoadingFace(2);
#endif ELSWORD_NEW_BEGINNING
	
	wstring wstrClientVersion = g_pMain->GetClientVersion();
	wstrClientVersion += L"_";
	wstrClientVersion += g_pMain->GetSubClientVersion_LUA();
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGServerSelectBack->GetControl( L"Static_Client_Version" );
	if( pStatic != NULL )
	{
		pStatic->GetString(0)->msg = wstrClientVersion;
	}


	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), true );

	//GetServerSetDataReq();
	g_pData->PlayLobbyBGM( L"Lobby.ogg", true );


#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
	//유닛 선택창

	// fix!! 이 부분 수정해야 합니다. 기존에는 GetMyuser가 NULL되는 경우가 없었으나, 신마을 클라이언트에서는 최초 접속시 getmyuser가 NULL이 됩니다.
	if( NULL != g_pData->GetMyUser() &&
		false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // 체험 아이디 제한
	{
		if( g_pMain->GetNexonLoginMessenger() == true && g_pMain->GetNexonVirtualLogin() == true )
		{
			if ( CNMCOClientObject::GetInstance().LogoutVirtual() == FALSE )
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_699 ), this );

			g_pMain->SetNexonVirtualLogin( false );
		}
	}
#endif // #ifndef NEW_MESSENGER
	if ( g_pData->GetMessenger() != NULL )
	{		
		//g_pData->GetMessenger()->ClearChatSessionList();
		g_pData->GetMessenger()->Reset();
	}

#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP

#ifdef ELSWORD_NEW_BEGINNING
	g_pBeginningGame->RenderLoadingFace(3);
#endif ELSWORD_NEW_BEGINNING

#ifndef HEAP_BROKEN_BY_ROOM
	g_pData->DeletePVPRoom();
	g_pData->DeleteDungeonRoom();
	g_pData->DeleteBattleFieldRoom();
#endif // HEAP_BROKEN_BY_ROOM


	g_pMain->GetInformerManager()->InvalidInfo();
	g_pMain->GetMemoryHolder()->ReleaseLocalMapDLG();

	//g_pData->DeleteCharacterRoom();
	g_pMain->GetMemoryHolder()->RelaseCommonDLG();


	
#ifdef NEW_VILLAGE_UI

#ifdef ELSWORD_NEW_BEGINNING
	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Back_TEST.lua" );
#else
	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Back.lua" );
#endif ELSWORD_NEW_BEGINNING

#else
	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_Unit_Select_Front.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUnitSelectFront );	
	

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
		g_pKTDXApp->GetDGManager()->GetFar(), true );



	if( NULL != g_pData->GetMyUser() )
	{
		m_MaxUnitNum	= g_pData->GetMyUser()->GetUserData().maxUnitCount;

		m_NowPage		= 1;
		const int SLOT_COUNT_FOR_CREATE_UNIT = 1;

		if( g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
		{
			m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+SLOT_COUNT_FOR_CREATE_UNIT+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
		}
		else
		{
			m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
		}
	}

#ifdef ELSWORD_NEW_BEGINNING
	g_pBeginningGame->RenderLoadingFace(4);
#endif ELSWORD_NEW_BEGINNING

	//CreateUnitButton();

	if ( m_sbSelectedServerSet == true )
	{
		ClearBeforeUnit( m_pSelectUnit );
		ClearUnitButton();
		CreateUnitButton();

		m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK = true;

		m_pDLGServerSelectBack->SetShowEnable(false, false);		
		m_pDLGUnitSelectFront->SetShowEnable(true, true);	
#ifdef ELSWORD_NEW_BEGINNING
		g_pBeginningGame->PlayWorldCamera(2);
#endif ELSWORD_NEW_BEGINNING
	}


	if( NULL != g_pMain->GetTutorSystem() )
	{
		g_pMain->GetTutorSystem()->SetBeenUnitSelect( true );
	}

	if( true == g_pMain->GetIsExitingTutorial() )
	{
		switch( g_pMain->GetStateIDBeforeTutorial() )
		{
#ifdef ELSWORD_NEW_BEGINNING
		case CX2Main::XS_BEGINNING:
#endif ELSWORD_NEW_BEGINNING
			{
				g_pMain->SetIsExitingTutorial( false );
			} break;
		}
	}

	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua" );
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );
		// 랜덤 로딩 화면 
		wstringstream wstmStaticName;
		wstmStaticName << L"Static_Loading_BG";
		wstmStaticName << g_pMain->GetTutorialRandomLoadingBG();
		m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() )->SetShow( true );

		m_pSelectUnit = g_pData->GetMyUser()->GetUnitByIndex( g_pData->GetMyUser()->GetUnitNum() - 1 );
		g_pMain->SetStateIDBeforeTutorial( g_pMain->GetNowStateID() );
		m_bCheckRecommend = true;
		EnterTutorial();
	}


#ifdef ELSWORD_NEW_BEGINNING
	g_pBeginningGame->RenderLoadingFace(5);
#endif ELSWORD_NEW_BEGINNING
	g_pData->ResetPartyManager(); // 캐릭선택창에서 캐릭터를 바꾸는 경우에 파티매니저 갱신

	if( NULL != g_pData->GetMyUser() )
	{
		g_pData->GetMyUser()->SelectUnit( NULL );
	}

	//{{ kimhc // 2009-12-15 // 이전에 플레이했던 채널 서버군
#ifdef	ADD_SERVER_GROUP
	if ( g_pInstanceData->GetServerGroupID() == SGI_INVALID )
    {
		//OpenScriptServerGroupFile();
        g_pInstanceData->OpenScriptServerGroupFile();
    }

	UpdateServerSelectButton( g_pInstanceData->GetServerGroupID() );
	
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-15 // 이전에 플레이했던 채널 서버군
	UpdateChannelListUI();


//#ifdef _SERVICE_
//	OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck5" );
//#endif

	//if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_DISAGREE_HACK_USER )
	//{
	//	m_pDLGCheckHack = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
	//	L"회사가 제공하지 아니한 프로그램 등을 사용하는 경우, 회사는 해당 프로그램 등의 정보를 수집할 수 있으며 회원은 약관 및 운영정책에 따라 서비스 이용이 제한될 수 있습니다. 동의 하시겠습니까?", 
	//	SSSUCM_HACK_USER_PROCESS_CHECK_OK, this, SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL );
	//}
	//else if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
	//{
	//	g_pMain->UpdateProcessList();
	//	g_pMain->SendHackMail();
	//}



	g_pData->GetServerProtocol()->SendTRID( ETR_UNIT_DISCONNECT );



	// 게임서버에 접속했다가 시스템 메뉴의 "서버선택" 버튼을 눌러서 돌아온 경우 아직 게임서버와의 연결이 끊어지지 않았다.
	if( false == m_sbSelectedServerSet && 		
		g_pData->GetServerProtocol()->IsConnected() == true )
	{
		m_bENX_USER_LOGIN_NOT		= false;
		m_bEGS_VERIFY_ACCOUNT_REQ	= false;
		m_bEGS_VERIFY_ACCOUNT_ACK	= false;
		m_bPortCheckReq				= false;

		m_bSentEGS_CONNECT_REQ 		= false;
		m_bReceivedChannelList 		= false;
		

		// 게임서버와의 접속을 끊는다
		Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();
	}
	
	if( false == m_sbSelectedServerSet )
	{
		ConnectToChannelServer();
	}


#ifdef ELSWORD_NEW_BEGINNING
	g_pBeginningGame->RenderLoadingFace(6);
#endif ELSWORD_NEW_BEGINNING



#ifdef ELSWORD_NEW_BEGINNING
	g_pBeginningGame->RenderLoadingFace(7);
#endif ELSWORD_NEW_BEGINNING

	// oasis907 : 김상윤 [2010.11.16] // 
	g_pKTDXApp->SkipFrame();
	
	
}

CX2StateBeginning::~CX2StateBeginning(void)
{
	//{{ kimhc // 2009-12-15 // 이전에 플레이했던 채널 서버군
#ifdef	ADD_SERVER_GROUP
	SAFE_DELETE_DIALOG( m_pDLGChannelSelectBack );
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-15 // 이전에 플레이했던 채널 서버군

	SAFE_DELETE_DIALOG( m_pDLGServerSelectBack );

	SAFE_DELETE_DIALOG( m_pDLGCheckHack );

	ClearUnitButton();

#ifndef ELSWORD_NEW_BEGINNING
	SAFE_DELETE_DIALOG( m_pDLGUnitSelectBack );
#endif ELSWORD_NEW_BEGINNING

	SAFE_DELETE_DIALOG( m_pDLGUnitSelectFront );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
	SAFE_DELETE( m_pDLGLoadingState ); // note: 얘는 다이얼로그 매니저에서 관리되는 애가 아니라서~ 요렇게
#endif // DIALOG_HANDLE_TEST // 2009-8-19


	SAFE_DELETE_DIALOG( m_pDLGRenameMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGRenameConfirmMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGRenameSuccessMsgBox );

	SAFE_DELETE_DIALOG( m_pDLGTutorialMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGGoToFirstDungeon );
	SAFE_DELETE_DIALOG( m_pDLGRecommend );

}

HRESULT CX2StateBeginning::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2State::OnFrameMove( fTime, fElapsedTime );


	OnFrameMove_GameServerConnect( fTime, fElapsedTime );

	if( true == m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK &&
		true == m_bOneTimeInit )
	{
		m_bOneTimeInit = false;

		if( 0 == g_pData->GetMyUser()->GetUnitNum() )
		{
			//{{ kimhc // 2009-12-23 // 서버군에 유닛이 없는 경우의 이동 수정
#ifdef	ADD_SERVER_GROUP
			UnitSelectExit();
#else	ADD_SERVER_GROUP
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CREATE_UNIT, NULL, false );
			m_bStateChangingToUnitCreate = true;
#endif	ADD_SERVER_GROUP
			//}} kimhc // 2009-12-23 // 서버군에 유닛이 없는 경우의 이동 수정
		}
	}


	if ( m_pDLGUnitSelectFront != NULL )
	{
#ifdef ELSWORD_NEW_BEGINNING
		if( g_pBeginningGame != NULL && false == g_pBeginningGame->GetEnableCreateUnit() )
		{
			if( m_eNowState == BG_STATE_UNIT_CREATE)
			{
				ReInitUnitSelect();
				m_eNowState = BG_STATE_UNIT_SELECT;
				g_pBeginningGame->SetEnableCreateUnit(false);
				SetShowUnitButton( true );
				g_pBeginningGame->PlayWorldCamera(2);

			}
		}
		if( g_pBeginningGame != NULL && true == g_pBeginningGame->GetEnableCreateUnit() )
 		{

 			m_pDLGServerSelectBack->SetShowEnable( false, false );
 			m_pDLGUnitSelectFront->SetShowEnable( false, false );
 		}
 		else
#endif ELSWORD_NEW_BEGINNING
		if ( m_sbSelectedServerSet == true )
		{
			m_pDLGServerSelectBack->SetShowEnable(false, false);
			m_pDLGUnitSelectFront->SetShowEnable( true, true );
			//{{ kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
#ifdef	ADD_SERVER_GROUP
			m_pDLGChannelSelectBack->SetShowEnable( false, false );
#endif	ADD_SERVER_GROUP
			//}}  kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리

			// 채널번호 표시
			if( g_pInstanceData != NULL && g_pInstanceData->GetConnectChannelServerID() > 0 )
			{
				CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Channel_Name" );
				if( NULL != pStatic &&
					NULL != pStatic->GetString(0) )
				{
					pStatic->GetString(0)->msg = g_pInstanceData->GetConnectChannelServerName();
				}
			}
		}
		else
		{
			m_pDLGServerSelectBack->SetShowEnable(true, true);
#ifdef ELSWORD_NEW_BEGINNING
			if(false == m_bServerSelectDlgFadeIn)
			{
				CKTDGUIButton *pServerButtonSoles		= static_cast< CKTDGUIButton* >( m_pDLGServerSelectBack->GetControl( L"sever_select_01" ) ); // 솔레스
				CKTDGUIButton *pServerButtonGaia		= static_cast< CKTDGUIButton* >( m_pDLGServerSelectBack->GetControl( L"sever_select_02" ) );	// 신서버

				m_pDLGServerSelectBack->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.0f));
				m_fServerSelectDlgFadeInElapsedTime += fElapsedTime;

				pServerButtonSoles->SetEnable(false);
				pServerButtonGaia->SetEnable(false);

				if(m_fServerSelectDlgFadeInElapsedTime > _CONST_STATE_BEGINNING_::g_fTimeServerSelectDlgFadeStart)
				{
					
					m_pDLGServerSelectBack->Move(m_pDLGServerSelectBack->GetPos(), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 
												_CONST_STATE_BEGINNING_::g_fTimeServerSelectDlgFadeIn, false);
					m_bServerSelectDlgFadeIn = true;
					m_fServerSelectDlgFadeInElapsedTime = 0.f;

					pServerButtonSoles->SetEnable(true);
					pServerButtonGaia->SetEnable(true);



				}

				if(false == m_bServerSelectWhiteIn)
				{
					//g_pData->GetEventScene()->BlackLayOutMove( false, 1.0f, false, true, 3.0f, 1.0f );
					m_bServerSelectWhiteIn = true;
				}
			}

#endif ELSWORD_NEW_BEGINNING

			m_pDLGUnitSelectFront->SetShowEnable( false, false );
			//{{ kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
#ifdef	ADD_SERVER_GROUP
			m_pDLGChannelSelectBack->SetShowEnable( true, true );
#endif	ADD_SERVER_GROUP
			//}}  kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
		}
	}

	static bool bAutoStart = true;

	if ( bAutoStart == true && g_pMain->m_bAutoStart == true && NULL != g_pData->GetMyUser() )
	{
		bAutoStart = false;

		CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( g_pMain->m_AutoLoginUnitIndex );

		if( pUnit == NULL )
			return true;

		UnitButtonUp( pUnit );


		Handler_EGS_SELECT_UNIT_REQ( pUnit->GetUID(), pUnit->GetUnitData().m_Level );
	}

	//	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );



	UnitHandling( fTime, fElapsedTime );
	DownAnimFrameMove( fTime, fElapsedTime );

	if ( m_pOveredButton != NULL && m_pOveredName != NULL )
	{
		if ( m_pOveredButton->GetMouseOver() == false )
		{
			if ( m_pOveredButton->GetButtonControlState() != CKTDGUIButton::BCS_MOUSEDOWN )
			{
				//SetShowCharacterName( false );
				m_pOveredName->Move( m_pOveredName->GetOffsetPos(), D3DXCOLOR(0,0,0,0), 0.2f, true, false );
				m_pOveredButton = NULL;
				m_pOveredName = NULL;
			}
		}
	}

	//SetShowCharacterNameForSelectedUnit();


#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP

	if ( g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->CheckFriendReq();

#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP



	//if ( m_bCheckPostItem == true )
	//{
	//	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGRecommendReward ) == false &&
	//		g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGTutorReward ) == false &&
	//		g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGGuestUserReward ) == false )
	//	{
	//		m_bCheckPostItem = false;
	//		Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
	//	}
	//}

	if ( m_bCheckRecommendSuccess == true )
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGRecommendSuccess ) == false )
		{
			m_bCheckRecommendSuccess = false;
			//Handler_EGS_GET_POST_LETTER_LIST_REQ();
			Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
		}
	}

	if ( NULL != g_pData->GetMyUser() &&
		g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
	{
		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_LEFT ) == TRUE )
		{
			if ( m_NowPage > 1 )
			{
				ClearBeforeUnit( m_pSelectUnit );
				ClearUnitButton();

				m_NowPage--;


				CreateUnitButton();

			}
		}
		else if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RIGHT ) == TRUE )
		{
			if ( m_NowPage < m_MaxPage )
			{
				if( (m_NowPage * UNIT_SELECT_UI_MAX_UNIT_NUM) <= g_pData->GetMyUser()->GetUnitNum()+1 )
				{	
					ClearBeforeUnit( m_pSelectUnit );
					ClearUnitButton();

					m_NowPage++;

					CreateUnitButton();
				}
			}
		}

	}

	// oasis907 : 김상윤 [2010.11.11] // 
	if( NULL != g_pBeginningGame )
	{
		g_pBeginningGame->OnFrameMove(fTime, fElapsedTime);
	}

#ifdef ELSWORD_NEW_BEGINNING
	if(m_bChannelSelectWhiteOut)
	{
		m_fChannelSelectWhiteOutElapsedTime += fElapsedTime;
		if(m_fChannelSelectWhiteOutElapsedTime >= _CONST_STATE_BEGINNING_::g_fTimeChannelSelectWhiteOut)
		{
			m_bChannelSelectWhiteOut = false;
			m_fChannelSelectWhiteOutElapsedTime = 0.f;
			Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ();
		}
	}
#endif ELSWORD_NEW_BEGINNING



	return S_OK;
}



void CX2StateBeginning::OnFrameMove_GameServerConnect( double fTime, float fElapsedTime )
{
	if ( m_bPortCheckReq == false && 
		g_pData->GetMyUser() != NULL &&
		m_bENX_USER_LOGIN_NOT == true &&
		m_bEGS_VERIFY_ACCOUNT_ACK == true )
	{
		m_bPortCheckReq = true;


		g_pData->GetMyUser()->SetPCRoom( m_bIsGameBang );

		if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
		{

#ifndef _SERVICE_
			g_pData->GetServerProtocol()->SendID( E_DISABLE_HB_CHECK_REQ );

			if( NULL == g_fpStream && NULL == g_fpStreamError ) 
			{
				AllocConsole();
				g_fpStream = freopen("CONOUT$", "a", stdout); // redirect printf to console
				g_fpStreamError = freopen("CONOUT$", "a", stderr); // redirect printf to console
				dbg::logfile::SetLogLevel(2);
			}
#endif
		}
		else 
		{
#ifndef _SERVICE_
			FreeConsole();
			if ( g_fpStream != NULL )
				fclose( g_fpStream );
			if ( g_fpStreamError != NULL )
				fclose( g_fpStreamError );
#endif
		}


		Handler_KXPT_PORT_CHECK_REQ();
	}


	if ( m_bEGS_VERIFY_ACCOUNT_REQ == true && m_bEGS_VERIFY_ACCOUNT_ACK == false )
	{
		m_fEGS_VERIFY_ACCOUNT_ACK_WaitTime += fElapsedTime;
		if ( m_fEGS_VERIFY_ACCOUNT_ACK_WaitTime > 180.f )
		{
			//{{ kimhc // 2010-05-07 // 크래시가 아니라 정상 종료 하도록 수정
			// 	 int* x = NULL;	
			// 	 *x = 1;
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
			//}} kimhc // 2010-05-07 // 크래시가 아니라 정상 종료 하도록 수정
		}
	}



	if( m_bPortCheckWait == true )
	{
		m_fPortCheckWaitTime -= fElapsedTime;
		if( m_fPortCheckWaitTime < 0.0f )
		{
			m_PortCheckRetryTime++;
			if( m_PortCheckRetryTime < 10 )
			{
				Handler_KXPT_PORT_CHECK_REQ();
			}
			else
			{
				m_bPortCheckWait			= false;
				m_fPortCheckWaitTime		= 0.0f;
				m_PortCheckRetryTime		= 0;

				if( g_pData->GetGameUDP()->IncreaseMyIPIndex() == true )
				{
					Handler_KXPT_PORT_CHECK_REQ();
				}
				else
				{
					//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"포트 체크 응답이 없습니다", this );
					ErrorLogMsg( XEM_ERROR89, L"PortCheckFail" );

					KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    kXPT_PORT_CHECK_ACK.m_IPAddress	= g_pData->GetGameUDP()->GetMyIPAddress();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					kXPT_PORT_CHECK_ACK.m_IP	= g_pData->GetGameUDP()->GetMyIP();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					kXPT_PORT_CHECK_ACK.m_Port	= g_pData->GetGameUDP()->GetMyPort();
					Handler_KXPT_PORT_CHECK_ACK( kXPT_PORT_CHECK_ACK );
				}				
			}
		}		
	}

	CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
	while( pRecvData != NULL )
	{
		switch( pRecvData->m_ID )
		{
		//case XPT_CONNECT_SERVER_ACK:
		//	{
		//		//if( m_bLoadBalancerWait == true )
		//		//{
		//		//	KSerBuffer ksBuff;
		//		//	ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
		//		//	KXPT_CONNECT_SERVER_ACK kXPT_CONNECT_SERVER_ACK;
		//		//	DeSerialize( &ksBuff, &kXPT_CONNECT_SERVER_ACK );
		//		//	Handler_KXPT_CONNECT_SERVER_ACK( &kXPT_CONNECT_SERVER_ACK );
		//		//	
		//		//}
		//	}
		//	break;

		case XPT_PORT_CHECK_ACK:
			{
				if( m_bPortCheckWait == true )
				{
					KSerBuffer ksBuff;
					ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
					KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
					DeSerialize( &ksBuff, &kXPT_PORT_CHECK_ACK );
					Handler_KXPT_PORT_CHECK_ACK( &kXPT_PORT_CHECK_ACK );
				}
			}
			break;
		}

		SAFE_DELETE( pRecvData );
		pRecvData = g_pData->GetGameUDP()->PopRecvData();
	}


}

HRESULT CX2StateBeginning::OnFrameRender()
{
// 	if( true == m_bStateChangingToUnitCreate )
// 	{
// 		return S_OK;
// 	}


	if( NULL != g_pBeginningGame )
	{
		//g_pBeginningGame->OnFrameRender();
	}

	CX2State::OnFrameRender();

	return S_OK;
}


bool CX2StateBeginning::EnterTutorial()
{
	if( NULL != m_pSelectUnit )
	{
		m_bReserveEnterTutorial = true;
		Handler_EGS_SELECT_UNIT_REQ( m_pSelectUnit->GetUID(), m_pSelectUnit->GetUnitData().m_Level );	
#ifdef ELSWORD_NEW_BEGINNING
		SAFE_DELETE(g_pBeginningGame);
#endif ELSWORD_NEW_BEGINNING
		return true;
	}

	return false;
}


bool CX2StateBeginning::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if( g_pBeginningGame != NULL)
	{
		if( g_pBeginningGame->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

	switch(wParam)
	{
		case SUSUCM_UNIT_BUTTON_OVER:
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				return false;
				//return AnimateCharacterName( pButton );				
			}
			break;

		case SUSUCM_UNIT_BUTTON_UP:
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;

				pButton->SetDownStateAtNormal( true );
				if ( m_pSelectButton != NULL )
					m_pSelectButton->SetDownStateAtNormal( false );

				if ( m_pSelectButton != pButton )
				{
					m_pSelectButton = pButton;
					m_pSelectButton->SetDownStateAtNormal( true );
				}

				wstring tempButtonName = pButton->GetName();
				tempButtonName = tempButtonName.substr( 12 );
				UidType unitUID = _wtoi64( tempButtonName.c_str() ); 
				CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( unitUID );

				if( pUnit == NULL )
					return true;

				UnitButtonUp( pUnit );

				//SetShowCharacterName( false );

				//SetShowCharacterNameForSelectedUnit();

			}
			break;

		case SUSUCM_BACK_TO_LOGIN:
			{
				
			}
			break;

		case SUSUCM_CREATE_UNIT:

			if ( m_sbSelectedServerSet == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_700 ), this );
				return true;
			}

			if ( g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
			{
#ifdef ELSWORD_NEW_BEGINNING
				m_eNowState = BG_STATE_UNIT_CREATE;
				ClearBeforeUnit( m_pSelectUnit );
				SetShowUnitButton( false );
				m_pDLGUnitSelectFront->SetShowEnable(false, false);
				g_pBeginningGame->SetEnableCreateUnit(true);
				g_pBeginningGame->PlayWorldCamera(3);
#else

#ifdef REFORM_ENTRY_POINT
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ), L"DLG_UI_Selection_MessageBox_No_Button.lua", L"UI_PopUp_Positive_01.ogg"  );
#else  // REFORM_ENTRY_POINT
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ) );
#endif // REFORM_ENTRY_POINT
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CREATE_UNIT, NULL, false );
#endif ELSWORD_NEW_BEGINNING
			}
			else
			{
				//if( m_MaxUnitNum == 6 )
				//{
				//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"캐릭터 슬롯이 부족하여\n더 이상 캐릭터를 생성할 수 없습니다.", this );
				//}
				//else
				//{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_702 ), this );
				//}
			}
			break;	

		case SUSUCM_SELECT_UNIT:
			{
				if( NULL != m_pSelectUnit )
				{
					m_bReserveEnterTutorial = false;
					Handler_EGS_SELECT_UNIT_REQ( m_pSelectUnit->GetUID(), m_pSelectUnit->GetUnitData().m_Level );
				}
				return true;
			}

			break;
		case SUSUCM_DELETE_UNIT:
			{	
				if ( m_pSelectUnit != NULL )
				{
					//DeleteUnitReq();

					wstring tempEx = GET_REPLACED_STRING( ( STR_ID_703, "SI", m_pSelectUnit->GetNickName(), KHanSungPostWordUnicodeWrapper( (WCHAR*) m_pSelectUnit->GetNickName(), STR_ID_198, STR_ID_199 ) ) );
					m_pDLGDeleteUnitCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), tempEx.c_str(), SUSUCM_DELETE_UNIT_CHECK ,this );
				}
				else
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_704 ), this );
				}

			}

			break;

		case SUSUCM_DELETE_UNIT_CHECK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGDeleteUnitCheck, NULL, false );
				m_pDLGDeleteUnitCheck = NULL;
				
#ifdef REMOVE_USER_ONLY_IF_REMOVED_NM_VIRTUAL_USER
				if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
				{
					return Handler_EGS_DELETE_UNIT_REQ();
				}
				else
				{
					m_bRemoveUnitAfterReceiveNMSerialNum = true;
					return Handler_EGS_GET_MY_MESSENGER_SN_REQ( m_pSelectUnit->GetUID() );
				}
#else REMOVE_USER_ONLY_IF_REMOVED_NM_VIRTUAL_USER
				//{{AFX
				return Handler_EGS_DELETE_UNIT_REQ();
				//}}AFX
#endif REMOVE_USER_ONLY_IF_REMOVED_NM_VIRTUAL_USER
				
			}
			break;

		case SUSUCM_UNIT_ANIMATION_DOWN:
			m_bUnitClicked = true;
			break;

		case SUSUCM_UNIT_ANIMATION_UP:
			{
				m_bUnitClicked = false;
			}
			break;

		case SUSUCM_UNIT_ANIMATION_OVER:
			break;

		case SUSUCM_TUTORIAL:
			{
				if( NULL != m_pSelectUnit )
				{
					g_pMain->SetStateIDBeforeTutorial( g_pMain->GetNowStateID() );
					EnterTutorial();
				}
				else
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_705 ), this );
				}
			}
			break;

		case SUSUCM_RENAME_OK:
			{
				if( m_pSelectUnit != NULL && NULL != m_pDLGRenameMsgBox )
				{
					CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) m_pDLGRenameMsgBox->GetControl( L"IME_NewNick" );
					if( NULL != pEditBox )
					{
						m_NickNameChangeUnitUID = m_pSelectUnit->GetUID();
						m_NickNameChangeUnitNickName = pEditBox->GetText();

						wstring wstrErrorMessage	= L"";
						if( true == CX2Main::IsInvalidNickName( m_NickNameChangeUnitNickName, wstrErrorMessage ) )
						{
							SetRenameMsgBoxError( wstrErrorMessage );
							return true;
						}

						Handler_EGS_CHANGE_NICK_NAME_REQ( m_NickNameChangeUnitUID, m_NickNameChangeUnitNickName.c_str(), true );
					}
				}
				return true;

			} break;

		case SUSUCM_RENAME_CANCEL:
			{
				OpenRenameMsgBox( false );
				return true;
			} break;

		case SUSUCM_RENAME_CONFIRM_OK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRenameConfirmMsgBox, NULL, false );
				m_pDLGRenameConfirmMsgBox = NULL;

				Handler_EGS_CHANGE_NICK_NAME_REQ( m_NickNameChangeUnitUID, m_NickNameChangeUnitNickName.c_str(), false );
				return true;
			} break;


		case SUSUCM_RENAME_CONFIRM_CANCEL:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRenameConfirmMsgBox, NULL, false );
				m_pDLGRenameConfirmMsgBox = NULL;
				return true;
			} break;


		case SUSUCM_RENAME_SUCCESS_OK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRenameSuccessMsgBox, NULL, false );
				m_pDLGRenameSuccessMsgBox = NULL;

				
				m_bJustChangedNickName = true;
				
				const int MAGIC_UNIT_LEVEL = -1;
				Handler_EGS_SELECT_UNIT_REQ( m_NickNameChangeUnitUID, MAGIC_UNIT_LEVEL );
				return true;
			} break;

		case SUSUCM_START_TUTORIAL_OK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGTutorialMsgBox, NULL, false );
				m_pDLGTutorialMsgBox = NULL;


				g_pMain->SetStateIDBeforeTutorial( g_pMain->GetNowStateID() );
				EnterTutorial();
				return true;
			} break;

		case SUSUCM_START_TUTORIAL_CANCEL:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGTutorialMsgBox, NULL, false );
				m_pDLGTutorialMsgBox = NULL;

				g_pMain->SetIsPlayingTutorial( false );

				m_pDLGGoToFirstDungeon = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_706 ), 
					SUSUCM_START_FIRST_DUNGEON_OK, this, SUSUCM_START_FIRST_DUNGEON_CANCEL );


				return true;
			} break;


		case SUSUCM_START_FIRST_DUNGEON_OK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGGoToFirstDungeon, NULL, false );
				m_pDLGGoToFirstDungeon = NULL;

				if( NULL == m_pSelectUnit )
					return true;

				CX2StateAutoChanger::TARGET_DETAIL targetDetail;
				targetDetail.m_iChannelID = (int) 104; // fix!!! 채널번호 일단 하드코딩
				targetDetail.m_iDungeonID = (int) SEnum::DI_EL_FOREST_GATE_NORMAL;
				targetDetail.m_iRoomUID = -1;

				g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_DUNGEON_GAME, targetDetail, m_pSelectUnit->GetUID() );

				return true;
			} break;

		case SUSUCM_START_FIRST_DUNGEON_CANCEL:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGGoToFirstDungeon, NULL, false );
				m_pDLGGoToFirstDungeon = NULL;


				if( NULL == m_pSelectUnit )
					return true;

				CX2StateAutoChanger::TARGET_DETAIL targetDetail;
				g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_VILLAGE_MAP, targetDetail, m_pSelectUnit->GetUID() );

				return true;

			} break;

		case SUSUCM_RECOMMEND_OK:
			{
				//REQ날려보자
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGRecommend->GetControl( L"g_pIMEEditBoxName" );
				if ( pIMEEditBox != NULL )
				{
					return Handler_EGS_RECOMMEND_USER_REQ( pIMEEditBox->GetText() );
				}
			}
			break;

		case SUSUCM_RECOMMEND_CANCEL:
			{
				//그냥 패스 마을맵으로.
				OpenRecommendMsgBox( false );
				//Handler_EGS_GET_POST_LETTER_LIST_REQ();

				return Handler_EGS_RECOMMEND_USER_REQ( L"", true );				
			}
			break;

#ifdef SERV_RECOMMEND_LIST_EVENT
		case SUSUCM_RECOMMEND_NEXT:
			{
				OpenRecommendMsgBox( false );
				return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
			}
			break;
#endif //SERV_RECOMMEND_LIST_EVENT

		case SUSUCM_PREV:
			{
				if ( m_NowPage > 1 )
				{
					ClearBeforeUnit( m_pSelectUnit );
					ClearUnitButton();

					--m_NowPage;


					CreateUnitButton();
				}
				//else
				//{
				//	ClearBeforeUnit( m_pSelectUnit );
				//	ClearUnitButton();

				//	m_NowPage = m_MaxPage;

				//	CreateUnitButton();
				//}
			}
			break;

		case SUSUCM_NEXT:
			{
				if ( m_NowPage < m_MaxPage )
				{
					if( (m_NowPage * UNIT_SELECT_UI_MAX_UNIT_NUM) <= g_pData->GetMyUser()->GetUnitNum() + 1)
					{	
						ClearBeforeUnit( m_pSelectUnit );
						ClearUnitButton();

						++m_NowPage;

						CreateUnitButton();
					}
				}
				//else
				//{
				//	ClearBeforeUnit( m_pSelectUnit );
				//	ClearUnitButton();

				//	m_NowPage = 1;

				//	CreateUnitButton();
				//}
			}
			break;

		case SSSUCM_SERVER_CONNECT0:	// 게임 서버 접속
			{	
				// 다른 게임서버와의 연결이 끊어지길 기다리는 중이라면 게임서버에 접속하지 않고 기다리도록 한다.
				if( true == m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK )		
					return true;

				
				
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				int iChannelIndex = pButton->GetDummyInt(0)-1;

#ifdef CHANGE_CHANNEL
				const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[ iChannelIndex ];
#else
				const KChannelInfo& channelInfo = m_vecChannelInfo[ iChannelIndex ];
#endif
				m_iTryConnectChannelID = channelInfo.m_iChannelID;
				
				return Handler_EGS_CONNECT_REQ( channelInfo.m_wstrIP, channelInfo.m_usMasterPort );
				
			} break;

		case SSSUCM_SELECT_SERVER_GROUP:  // 채널 서버 선택 및 접속
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;							// fix!! 라디오 버튼으로 변경 해야함 
				int iNewChannelServerIndex = pButton->GetDummyInt(0);



				//{{ kimhc // 2009-12-15 // 서버 그룹 추가
#ifdef	ADD_SERVER_GROUP
				// 선택한 서버군이 바뀌었다면 갱신
				if ( g_pInstanceData->GetServerGroupID() != SERVER_GROUP_ID( iNewChannelServerIndex ) )
				{
					g_pInstanceData->SetServerGroupID( SERVER_GROUP_ID( iNewChannelServerIndex ) );
					UpdateServerSelectButton( g_pInstanceData->GetServerGroupID() );			

					if( 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        g_pData->GetServerProtocol() != NULL && g_pData->GetServerProtocol()->IsChUserProxyValid() == true
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        g_pData->GetServerProtocol()->GetChUserProxy() != NULL 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        )
					{
						// 기존에 연결된 채널 서버 연결을 끊고
						// 새로 선택된 채널 서버에 연결한다. 
						Handler_ECH_DISCONNECT_REQ();
						m_bReservedConnectionToChannelServer = true;
					}
					else
					{
						ConnectToChannelServer();
					}
				}
#ifdef ELSWORD_NEW_BEGINNING			
				// oasis907 : 김상윤 [2010.11.15] // 
				m_eNowState = BG_STATE_CHANNEL_SELECT;
				SetShowEnableServerSelectControl(false);
				UpdateChannelListUI();
#endif ELSWORD_NEW_BEGINNING

#else	ADD_SERVER_GROUP
				if( g_pMain->GetPickedChannelServerIPIndex() == iNewChannelServerIndex )
				{
					return true;
				}
				else
				{
					if( true == m_sbSelectedServerSet && 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        g_pData->GetServerProtocol() != NULL && g_pData->GetServerProtocol()->IsChUserProxyValid() == true
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        g_pData->GetServerProtocol()->GetChUserProxy() != NULL 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        )
					{
						// 기존에 연결된 채널 서버 연결을 끊고
						// 새로 선택된 채널 서버에 연결한다. 
						Handler_ECH_DISCONNECT_REQ();
					}
					else
					{
						ConnectToChannelServer();
					}
				}
#endif	ADD_SERVER_GROUP
				//}} kimhc // 2009-12-15 // 서버 그룹 추가
								
				return true;

			} break;

			//사용하지 않고 있음.
		//case SSSUCM_SERVER_CONNECT1:
		//	{
		//		return Handler_EGS_SELECT_SERVER_SET_REQ();
		//	}
		//	break;

		//case SSSUCM_SERVER_CONNECT2:
		//	{
		//		return Handler_EGS_SELECT_SERVER_SET_REQ();
		//	}
		//	break;

#ifndef DISABLE_DISAGREE_HACK_USER
		case SSSUCM_HACK_USER_PROCESS_CHECK_OK:
			{
				Handler_EGS_AGREE_HACK_USER_REQ();
			}
			break;

		case SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL:
			{
				g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
			}
			break;
#endif DISABLE_DISAGREE_HACK_USER
		case SSSUCM_SERVER_SELECT_EXIT:
			{
				CX2State::QuitGame();
			}
			break;

		case SSSUCM_UNIT_SELECT_EXIT:
			{
				UnitSelectExit();
#ifdef ELSWORD_NEW_BEGINNING
				m_eNowState = BG_STATE_SERVER_SELECT;
				SetShowEnableServerSelectControl(true);
				UpdateChannelListUI();
#endif ELSWORD_NEW_BEGINNING
			}
			break;
	}

	return false;
}

bool CX2StateBeginning::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pBeginningGame != NULL)
	{
		if( g_pBeginningGame->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

	if ( CX2State::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#ifdef POSTBOX
    if( g_pMain->GetPostBox() != NULL)
    {
        if( g_pMain->GetPostBox()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
            return true;
    }
#endif

	switch( wParam )
	{
	case EGS_SELECT_UNIT_ACK:
		return Handler_EGS_SELECT_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		break;
#if 0 
	case EGS_STATE_CHANGE_VILLAGE_MAP_ACK:
		return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( hWnd, uMsg, wParam, lParam );
		break;
#endif

	case EGS_DELETE_UNIT_ACK:
		return Handler_EGS_DELETE_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		break;


	case EGS_CHANGE_NICK_NAME_ACK:
		{
			return Handler_EGS_CHANGE_NICK_NAME_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_RECOMMEND_USER_ACK:
		{
			return Handler_EGS_RECOMMEND_USER_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	//case EGS_GET_POST_LETTER_LIST_ACK:
	//	{
	//		return Handler_EGS_GET_POST_LETTER_LIST_ACK( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;

	// remove!!		
	//case EGS_ITEM_EXPIRATION_NOT:
	//	{
	//		return Handler_EGS_ITEM_EXPIRATION_NOT( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;


	case EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK:
		return Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_AGREE_HACK_USER_ACK:
		return Handler_EGS_AGREE_HACK_USER_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_FIELD_ACK:
		return Handler_EGS_STATE_CHANGE_FIELD_ACK( hWnd, uMsg, wParam, lParam );
	//case EGS_NEW_USER_LIST_IN_SECTOR_NOT:
	//	return Handler_EGS_NEW_USER_LIST_IN_SECTOR_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_CHANGE_USER_LIST_IN_SECTOR_NOT:
		return Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_CREATE_TUTORIAL_ROOM_ACK:
		return Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( hWnd, uMsg, wParam, lParam );

//#ifdef SERV_KOG_OTP_VERIFY -- 2009-08-31  최육사 : 주석처리함
	case ECH_VERIFY_ACCOUNT_ACK:
		return Handler_ECH_VERIFY_ACCOUNT_ACK(hWnd, uMsg, wParam, lParam);
//#endif


	case EGS_CONNECT_ACK:
		{
			return Handler_EGS_CONNECT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_VERIFY_ACCOUNT_ACK:
		{
			return Handler_EGS_VERIFY_ACCOUNT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	//case EGS_GET_SERVER_SET_DATA_ACK:
	//	{
	//		return Handler_EGS_GET_SERVER_SET_DATA_ACK( hWnd, uMsg, wParam, lParam );
	//	} break;

	case ENX_USER_LOGIN_NOT:
		{
			return Handler_ENX_USER_LOGIN_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CURRENT_TIME_ACK:
		{
			return Handler_EGS_CURRENT_TIME_ACK( hWnd, uMsg, wParam, lParam );
		} break;


	case ECH_GET_CHANNEL_LIST_ACK:
		{
			return Handler_ECH_GET_CHANNEL_LIST_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case ECH_GET_CHANNEL_LIST_NOT:
		{
			return Handler_ECH_GET_CHANNEL_LIST_NOT( hWnd, uMsg, wParam, lParam );
		} break;


	case ECH_DISCONNECT_ACK:
		{
			return Handler_ECH_DISCONNECT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_DISCONNECT_FOR_SERVER_SELECT_ACK:
		{
			return Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_GET_MY_MESSENGER_SN_ACK:
		{
			return Handler_EGS_GET_MY_MESSENGER_SN_ACK( hWnd, uMsg, wParam, lParam );
		} break;

		//{{ kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할
#ifdef	PC_BANG_WORK
	case EGS_SELECT_UNIT_INVENTORY_INFO_NOT:
		{
			return Handler_EGS_SELECT_UNIT_INVENTORY_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_GET_MY_INVENTORY_ACK:
		{
			return Handler_EGS_GET_MY_INVENTORY_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할
	}

	return false;
}





#ifdef ELSWORD_NEW_BEGINNING
void CX2StateBeginning::CreateUnitButton( bool bLoadingAnim /* = false */)
#else
void CX2StateBeginning::CreateUnitButton()
#endif ELSWORD_NEW_BEGINNING
{
	//유닛 개수를 확인한다.
	//각 유닛 종류에 맞춰 UnitButton을 생성한다.
	//버튼 이름을 유닛 닉네임으로 바꾼다.
	//인덱스에 따라 위치를 바꾼다.
#if 1 
	CKTDGUIDialogType pUnitSlot = NULL;
	CKTDGUIButton* pButton = NULL;

	D3DXVECTOR3 dummyPos;
	bool bAllSlotOccupiedInAPage = false;	// 한번에 볼 수 있는 캐릭터 슬롯수 만큼 캐릭터가 모두 있는지, 없다면 마지막 빈곳에 캐릭터 생성슬롯을 만들기 위해서

#ifdef ELSWORD_NEW_BEGINNING
	if(true == bLoadingAnim)
		g_pBeginningGame->RenderLoadingFace(0);
#endif ELSWORD_NEW_BEGINNING


	for(UINT i=0; i<m_vecUnitSlot.size(); ++i)
	{
		CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];
		SAFE_DELETE_DIALOG(pUnitSlot);
	}
	m_vecUnitSlot.resize(0);
	
	//if(g_pData->GetMyUser()->GetUnitNum() > 0)
	//{
	//	m_MaxPage = ((g_pData->GetMyUser()->GetUnitNum()-1) / UNIT_SELECT_UI_MAX_UNIT_NUM) + 1;
	//}
	//else
	//{
	//	m_MaxPage = 1;
	//}

	CKTDGUIStatic* pStaticUnitPage = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"number" );
	WCHAR buff[256] = {0,};
	//wsprintf( buff, L"%d / %d", m_NowPage, m_MaxPage );	
	StringCchPrintf( buff, 256, L"%d / %d", m_NowPage, m_MaxPage );	
	pStaticUnitPage->GetString(0)->msg = buff;



	for( int i = (m_NowPage - 1)*UNIT_SELECT_UI_MAX_UNIT_NUM, j = 0; i < g_pData->GetMyUser()->GetUnitNum() && j < UNIT_SELECT_UI_MAX_UNIT_NUM; i++, j++ )
	{
		CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( i );
		if( pUnit == NULL )
			continue;	

		dummyPos = m_pDLGUnitSelectFront->GetDummyPos( j );


#ifdef ELSWORD_NEW_BEGINNING
		pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Slot_TEST.lua" );
#else
		pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Slot.lua" );
#endif ELSWORD_NEW_BEGINNING
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pUnitSlot );
		pUnitSlot->SetShowEnable(true, true);
		m_vecUnitSlot.push_back(pUnitSlot);		

		CKTDGUIStatic* pStaticEmblem = (CKTDGUIStatic*)pUnitSlot->GetControl( L"UnitEmblem" );
		pStaticEmblem->GetPicture(0)->SetShow( true );
		CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnit->GetPVPEmblem() );
		if ( pPVPEmblemData != NULL )
		{
			pStaticEmblem->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
		}


		CX2UnitViewerUI* pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
		//pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
		pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
		g_pKTDXApp->GetDGManager()->AddObjectChain( pUnitViewerUI );


#ifdef ELSWORD_NEW_BEGINNING		
		D3DXVECTOR3 UnitDummyPos = m_pDLGUnitSelectFront->GetDummyPos( j + 6 );
		D3DXVECTOR3 vUnitPos = UnitDummyPos;
		vUnitPos.x -= 400.f; // 222
		vUnitPos.y -= 110.f; // 302
#else


		D3DXVECTOR3 vUnitPos = dummyPos;
		vUnitPos.x -= 400.f; // 222
		vUnitPos.y -= 110.f; // 302
#endif ELSWORD_NEW_BEGINNING



#ifdef RAVEN_SECOND_CLASS_CHANGE
		// 레이븐 기럭지가 너무 길어서 이름을 가리는 관계로 레이븐만 약간 위로 올려준다. 
		if( pUnit != NULL && (pUnit->GetType() == CX2Unit::UT_RAVEN || pUnit->GetType() == CX2Unit::UT_EVE) )
			vUnitPos.y += 5.f;
#endif

		//{{ kimhc // 2010-01-06 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
        if ( pUnit != NULL )
		    pUnit->AccessInventory().RemovePCBangEquips();
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-06 // PC방 프리미엄 서비스

#ifdef SEPARATION_MOTION
		pUnitViewerUI->SetUnit( pUnit, CX2UnitViewerUI::UVS_SELECTUNIT ); 
#else
		pUnitViewerUI->SetUnit( pUnit );
#endif
		//pUnitViewerUI->SetOverUI(true);
		pUnitViewerUI->GetMatrix().Move( vUnitPos );
		pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.2f, 1.2f, 1.2f ) );



#ifdef ELSWORD_NEW_BEGINNING
		switch(j)
		{
		case 0:
			pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(30),D3DXToRadian(0));
			break;
		case 1:
			pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(15),D3DXToRadian(0));
			break;
		case 2:
			pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			break;
		case 3:
			pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(-10),D3DXToRadian(0));
			break;
		}
#else
		pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
#endif ELSWORD_NEW_BEGINNING


		//pUnitViewerUI->PlayAnim( L"LobbyFullDown", CKTDGXSkinAnim::XAP_ONE_WAIT );

		pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );

		pUnitViewerUI->SetShowObject( false );

		m_UnitViewerUIList.push_back( pUnitViewerUI );

		pButton = (CKTDGUIButton*)pUnitSlot->GetControl( L"character1" );

		switch( pUnit->GetType() )
		{
		case CX2Unit::UT_ELSWORD:
			{
				//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Slot_Elsword.lua" );
				//pButton = (CKTDGUIButton*)m_pDLGUnitSelectBack->GetControl( L"ButtonSlot_Elsword" );

			} break;

		case CX2Unit::UT_LIRE:
			{
				//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Slot_Lire.lua" );
				//pButton = (CKTDGUIButton*)m_pDLGUnitSelectBack->GetControl( L"ButtonSlot_Lire" );
				//pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
			} break;

		case CX2Unit::UT_ARME:
			{
				//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Slot_Arme.lua" );
				//pButton = (CKTDGUIButton*)m_pDLGUnitSelectBack->GetControl( L"ButtonSlot_Arme" );
				//pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
			} break;

		case CX2Unit::UT_RAVEN:
			{
				//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Slot_Elsword.lua" );				// note!! 엘소드 꺼로 해서 버튼 이름을 아래에서 바꾼다
				//pButton = (CKTDGUIButton*)m_pDLGUnitSelectBack->GetControl( L"ButtonSlot_Elsword" );

			} break;


		case CX2Unit::UT_EVE:
			{
				//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Slot_Elsword.lua" );				// note!! 엘소드 꺼로 해서 버튼 이름을 아래에서 바꾼다
				//pButton = (CKTDGUIButton*)m_pDLGUnitSelectBack->GetControl( L"ButtonSlot_Elsword" );

			} break;


		default:
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_707 ), this );
			}break;
		}
		XSkinMeshReady( pUnit->GetUnitTemplet()->m_MotionFile.c_str() );


		wstringstream buttonName;
		buttonName << L"SLOT_BUTTON_" << pUnit->GetUID();
		if( NULL != pButton )
		{
			pButton->SetName( buttonName.str().c_str() );
		}
		

		WCHAR buff[256] = {0,};
		//wsprintf( buff, L"%d", (int)pUnit->GetUnitData().m_Level );
		StringCchPrintf( buff, 256, L"%d", (int)pUnit->GetUnitData().m_Level );
		CKTDGUIStatic* pStaticLVNum = (CKTDGUIStatic*)pUnitSlot->GetControl( L"StaticUnitSelectStringLVNum" );
		pStaticLVNum->GetString(0)->msg = buff;





		CKTDGUIStatic* pStaticID = (CKTDGUIStatic*)pUnitSlot->GetControl( L"StaticUnitSelectStringID" );
		CKTDGUIStatic* pStaticChangeNickEmblem = (CKTDGUIStatic*) pUnitSlot->GetControl( L"ChangeNick" );
		CKTDGUIStatic* pStaticChangeNickMessage = (CKTDGUIStatic*) pUnitSlot->GetControl( L"standby" );

		wstring tempNickName = pUnit->GetNickName();
		if ( tempNickName.find( L"__DELETED__") != -1 )
		{
			pStaticID->GetString(0)->msg = L"     ?";

			pStaticChangeNickEmblem->SetShowEnable( true, true );
			pStaticChangeNickMessage->SetShowEnable( true, true );
		}
		else
		{
			pStaticID->GetString(0)->msg = pUnit->GetNickName();

			pStaticChangeNickEmblem->SetShowEnable( false, false );
			pStaticChangeNickMessage->SetShowEnable( false, false );
		}

		
		D3DXVECTOR2 buttonPos = D3DXVECTOR2( dummyPos.x, dummyPos.y );

		/*
		D3DXVECTOR2 buttonPos = D3DXVECTOR2( 0, 0 );
		if ( i == 0 )
		{
		buttonPos = D3DXVECTOR2( 378, 123 );
		}
		else if ( i == 1 )
		{
		buttonPos = D3DXVECTOR2( 586, 123 );
		}
		else if ( i == 2 )
		{
		buttonPos = D3DXVECTOR2( 795, 123 );
		}
		else if ( i == 3 )
		{
		buttonPos = D3DXVECTOR2( 378, 404 );
		}
		else if ( i == 4 )
		{
		buttonPos = D3DXVECTOR2( 586, 404 );
		}
		else if ( i == 5 )
		{
		buttonPos = D3DXVECTOR2( 795, 404 );
		}
		*/
		/*
		m_pDLGUnitSelectFront->OpenScriptFile( L"DLG_Unit_Select_Unit_Info.lua" );
		wstring tempName;
		tempName = pUnit->GetNickName();
		tempName += L"UnitSelectUnitInfo";

		CKTDGUIStatic* pStaticUnitInfo = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"UnitSelectUnitInfo" );
		pStaticUnitInfo->SetName( tempName.c_str() );
		pStaticUnitInfo->GetString(0)->msg = pUnit->GetNickName();
		pStaticUnitInfo->GetString(1)->msg = L"콤보의제왕님";

		WCHAR arLV[8] = {0,};
		//wsprintf(arLV, 7, L"%d", pUnit->GetUnitData().m_Level );
		StringCchPrintf( buff, 8, L"%d", pUnit->GetUnitData().m_Level );
		pStaticUnitInfo->GetString(2)->msg = arLV;
		*/
		//		buttonPos.x -= 12;
		//		buttonPos.y -= 13;

		//if( NULL != pButton )
		//	pButton->SetOffsetPos( buttonPos );
		//D3DXVECTOR2 staticPos( buttonPos.x + 5, buttonPos.y );

		if( pUnitSlot != NULL)
			pUnitSlot->SetPos(buttonPos);
		if ( j == UNIT_SELECT_UI_MAX_UNIT_NUM - 1 )
			bAllSlotOccupiedInAPage = true;
		//		pStaticUnitInfo->SetOffsetPos( staticPos );
	}



	dummyPos = m_pDLGUnitSelectFront->GetDummyPos( j );
	D3DXVECTOR2 buttonPos = D3DXVECTOR2( dummyPos.x, dummyPos.y );
	/*
	D3DXVECTOR2 buttonPos = D3DXVECTOR2( 0, 0 );
	if ( i == 0 )
	{
	buttonPos = D3DXVECTOR2( 378, 123 );
	}
	else if ( i == 1 )
	{
	buttonPos = D3DXVECTOR2( 586, 123 );
	}
	else if ( i == 2 )
	{
	buttonPos = D3DXVECTOR2( 795, 123 );
	}
	else if ( i == 3 )
	{
	buttonPos = D3DXVECTOR2( 378, 404 );
	}
	else if ( i == 4 )
	{
	buttonPos = D3DXVECTOR2( 586, 404 );
	}
	else if ( i == 5 )
	{
	buttonPos = D3DXVECTOR2( 795, 404 );
	}
	*/
	if( false == bAllSlotOccupiedInAPage &&
		g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
	{
#ifdef ELSWORD_NEW_BEGINNING
		pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Make_TEST.lua" );
#else
		pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Make.lua" );
#endif ELSWORD_NEW_BEGINNING
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pUnitSlot );
		pUnitSlot->SetShowEnable(true, true);
		m_vecUnitSlot.push_back(pUnitSlot);	

		//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_UI_Character_Selection_Slot.lua" );
		pButton = (CKTDGUIButton*)pUnitSlot->GetControl( L"character2" );
		pButton->SetName( L"EmptySlot" );
		pUnitSlot->SetPos(buttonPos);
	}


	PlayDownAnim();

	

	g_pKTDXApp->SkipFrame();
#endif
}

#ifdef ELSWORD_NEW_BEGINNING
bool CX2StateBeginning::SetShowUnitButton( bool bShow )
{
	for(UINT i=0; i<m_vecUnitSlot.size(); ++i)
	{
		CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];
		pUnitSlot->SetShowEnable(bShow, bShow);
	}
	return true;
}
#endif ELSWORD_NEW_BEGINNING

bool CX2StateBeginning::ClearUnitButton()
{
	for(UINT i=0; i<m_vecUnitSlot.size(); ++i)
	{
		CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];
		SAFE_DELETE_DIALOG(pUnitSlot);
	}
	m_vecUnitSlot.clear();

	ClearUnitViewerList();

	return true;
}

void CX2StateBeginning::ClearBeforeUnit( CX2Unit* pUnit )
{
	if( pUnit == NULL )
		return;
	
	m_pSelectUnit = NULL;
}

void CX2StateBeginning::UnitButtonUp( CX2Unit* pUnit )
{
	if ( m_pSelectUnit == pUnit )
	{
		m_bReserveEnterTutorial = false;
		Handler_EGS_SELECT_UNIT_REQ( m_pSelectUnit->GetUID(), m_pSelectUnit->GetUnitData().m_Level );
		return;
	}

	if ( m_pSelectUnit != NULL )
	{
		CX2UnitViewerUI* pUnitViewer = GetUnitViewer( m_pSelectUnit );
		if ( pUnitViewer != NULL )
		{
			pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
		}
	}

	ClearBeforeUnit( m_pSelectUnit );



	m_pSelectUnit = pUnit;

/*
	CKTDGUIStatic* pStaticCharBG = (CKTDGUIStatic*)m_pDLGUnitSelectBack->GetControl(L"CharBG" );
	if ( pStaticCharBG != NULL )
	{
		for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
		{
			if ( pStaticCharBG->GetPicture(i) != NULL )
				pStaticCharBG->GetPicture(i)->SetShow( false );
		}

		// note!! 전직 추가되면 수정, 혹은 unit_type으로
		switch( pUnit->GetClass() )
		{
		case CX2Unit::UC_ELSWORD_SWORDMAN:
		case CX2Unit::UC_ELSWORD_KNIGHT:
		case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
			{
				if ( pStaticCharBG->GetPicture(0) != NULL )
					pStaticCharBG->GetPicture(0)->SetShow( true );
			}
			break;

		case CX2Unit::UC_LIRE_ELVEN_RANGER:
		case CX2Unit::UC_LIRE_COMBAT_RANGER:
		case CX2Unit::UC_LIRE_SNIPING_RANGER:
			{
				if ( pStaticCharBG->GetPicture(2) != NULL )
					pStaticCharBG->GetPicture(2)->SetShow( true );
			}
			break;

		case CX2Unit::UC_ARME_VIOLET_MAGE:
		case CX2Unit::UC_ARME_HIGH_MAGICIAN:
		case CX2Unit::UC_ARME_DARK_MAGICIAN:
			{
				if ( pStaticCharBG->GetPicture(1) != NULL )
					pStaticCharBG->GetPicture(1)->SetShow( true );
			}
			break;

		case CX2Unit::UC_RAVEN_FIGHTER:
			{
				if ( pStaticCharBG->GetPicture(3) != NULL )
					pStaticCharBG->GetPicture(3)->SetShow( true );
			}
			break;

		}
	}
*/


	CX2UnitViewerUI* pUnitViewer = GetUnitViewer( pUnit );
	if ( pUnitViewer != NULL  )
	{
		pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
	}
	/*
	D3DXVECTOR3 unitSize = D3DXVECTOR3( 1.6f, 1.6f, 1.6f );


	//	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGUnitSelectBack->GetControl( L"StaticUnit_Select_UnitInfo" );
	//	pStatic->SetOffsetPos( D3DXVECTOR2( -200, 200 ) );
	//	pStatic->Move( D3DXVECTOR2( 0, 0 ), D3DXCOLOR(1,1,1,1), 0.1f, true );

	switch( pUnit->GetClass() )
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:
	//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_ElSword_BG.lua" );

	//pStatic->GetPicture(0)->SetShow( true );

	m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );

	m_pUnitViewerUI->SetUnit( pUnit );
	m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250, -70, 200 ) );
	m_pUnitViewerUI->GetMatrix().Scale( unitSize );
	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
	m_pUnitViewerUI->PlayAnim( L"LobbyFullDown", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );

	break;

	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Lire_BG.lua" );
	//pStatic->GetPicture(2)->SetShow( true );

	m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );

	m_pUnitViewerUI->SetUnit( pUnit );
	m_pUnitViewerUI->GetMatrix().Move( D3DXVECTOR3( -250, -70, 200 ) );
	m_pUnitViewerUI->GetMatrix().Scale( unitSize );
	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
	m_pUnitViewerUI->PlayAnim( L"LobbyFullDown", CKTDGXSkinAnim::XAP_ONE_WAIT );
	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );

	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
	break;

	case CX2Unit::UC_ARME_VIOLET_MAGE:
	//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Arme_BG.lua" );
	//pStatic->GetPicture(1)->SetShow( true );
	m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );

	m_pUnitViewerUI->SetUnit( pUnit );
	m_pUnitViewerUI->GetMatrix().Move( D3DXVECTOR3( -250, -70, 200 ) );
	m_pUnitViewerUI->GetMatrix().Scale( unitSize );
	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
	m_pUnitViewerUI->PlayAnim( L"LobbyFullDown", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );

	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
	break;

	}




	g_pKTDXApp->SkipFrame();
	*/
}


bool CX2StateBeginning::Handler_EGS_SELECT_UNIT_REQ( UidType unitUID, int iUnitLevel )
{
	// 체험 아이디 제한 
	if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
		iUnitLevel >= 20 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
		return true;
	}

	KEGS_SELECT_UNIT_REQ	kPacket;
	kPacket.m_iUnitUID		= unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_SELECT_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SELECT_UNIT_ACK, 30.0f );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_709 ), this );

	return true;
}

bool CX2StateBeginning::Handler_EGS_SELECT_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( false == g_pMain->DeleteServerPacket( EGS_SELECT_UNIT_ACK ) )
	{
		StateLog( L"invalid sel char ack" );
		return false;
	}

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{

		// 휴면유저인지 확인후 팝업을 띄운다. -- 현재 쓰이지 않는 이벤트
		//switch(kEvent.m_iSelectUnitEvent)
		//{
		//case 0:	// 무시
		//	break;
		//case 1:	// 컴백유저
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2634 ), g_pMain->GetNowState() );
		//	break;
		//case 2:	// 컴백인데 재접속한 유저
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2635 ), g_pMain->GetNowState() );
		//	break;
		//case 3:	// 기존유저 보상
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2636 ), g_pMain->GetNowState() );
		//	break;
		//case 4: // 신규 재접속
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2640 ), g_pMain->GetNowState() );
		//	break;
		//default:
		//	break;
		//}

		if( NULL != g_pMain->GetTutorSystem() )
		{
			g_pMain->GetTutorSystem()->ResetTutorStudent( kEvent.m_vecTutorialDBUnitInfo );
		}

		if ( m_bReserveEnterTutorial == true )
		{
			g_pMain->SetIsPlayingTutorial( true );
		}

		CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_kUnitInfo.m_nUnitUID );
		g_pData->GetMyUser()->SelectUnit( pUnit );
		pUnit->SetBlackList( kEvent.m_vecChatBlackList ); 


#ifdef ADD_SERVER_GROUP
		if( g_pData != NULL && g_pInstanceData->GetServerGroupID() != g_pInstanceData->GetServerGroupIDScript() )
		{
			g_pData->LoadItemManager();
			g_pInstanceData->SetServerGroupIDScript( g_pInstanceData->GetServerGroupID() );
		}		
#endif


#ifdef POSTBOX
		g_pMain->GetPostBox()->InitPostBox();
#endif


		
		pUnit->Reset( kEvent.m_kUnitInfo, true );

		//{{ kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할
#ifdef	PC_BANG_WORK
		pUnit->ResetInventorySize( kEvent.m_mapInventorySlotSize );
		pUnit->ResetInventoryItems( kEvent.m_mapItem ); // 장착중인 장비만 먼저 Reset
#else	PC_BANG_WORK
		//kimhc // 2008-08-04 // m_mapInventorySlotSize에서 키값이 ST_BANK인 
		// value는 은행슬롯의 사이즈 이고 이 사이즈로 회원 등급을 구분한다.
		pUnit->ResetInventory( kEvent.m_mapInventorySlotSize, kEvent.m_mapItem );
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할

		pUnit->SetResurrectionStoneNum( kEvent.m_iNumResurrectionStone );

		//{{ 허상형 : [2009/10/14] //	자동 결제 부활석
#ifdef AUTO_PAYMENT
		pUnit->SetAutoResStoneNum( kEvent.m_iNumAutoPaymentResStone );
#endif	//	AUTO_PAYMENT
		//}} 허상형 : [2009/10/14] //	자동 결제 부활석

		// skill list
		pUnit->AccessUnitData().m_UserSkillTree.SetUnitClass( (int) kEvent.m_kUnitInfo.m_cUnitClass );
		pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredSkill( kEvent.m_vecSkillAcquired );
		pUnit->AccessUnitData().m_UserSkillTree.SetUnsealedSkill( kEvent.m_vecSkillUnsealed );
		pUnit->AccessUnitData().m_UserSkillTree.SetEquippedSkill( kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill, kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB );
		pUnit->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( (CX2UserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE) kEvent.m_kUnitInfo.m_UnitSkillData.m_cSkillSlotBExpirationState );
		pUnit->AccessUnitData().m_UserSkillTree.SetSkillSlotBEndDateString( kEvent.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate );


		if( g_pData->GetUIManager() != NULL)
		{
			g_pData->GetUIManager()->CreateUISkillTree();
			g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();
		}
#ifdef GUILD_SKILL
		//{{  [11/11/2009 : oasis907 ]

		// 임시: 서버에게서 GuildUserSkillTree 정보를 받았다 가정
/*
		// skill list
		pUnit->AccessUnitData().m_UserSkillTree.SetGuildClass(0);
		pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredGuildSkill( kEvent.m_vecSkillAcquired ); // 함수 임시 변경
*/		
		
		//pUnit->AccessUnitData().m_GuildUserSkillTree.SetUnsealedSkill( kEvent.m_vecSkillUnsealed );


		// 함수 임시 변경
		//pUnit->AccessUnitData().m_UserSkillTree.SetEquippedSkill( kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill, kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB );

		//pUnit->AccessUnitData().m_GuildUserSkillTree.SetSkillSlotBExpirationState( (CX2GuildUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE) kEvent.m_kUnitInfo.m_UnitSkillData.m_cSkillSlotBExpirationState );
		//pUnit->AccessUnitData().m_GuildUserSkillTree.SetSkillSlotBEndDateString( kEvent.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate );

/*
		if( g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() != NULL)
		{
			g_pData->GetGuildManager()->GetUIGuild()->CreateUIGuildSkillTree();
			g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->InitSkillTreeUI();
		}
		//}}  [11/11/2009 : oasis907 ]
*/
#endif GUILD_SKILL

#ifdef SERV_SKILL_NOTE		
		pUnit->SetSkillNote( kEvent.m_mapSkillNote, kEvent.m_cSkillNoteMaxPageNum );
		pUnit->AccessUnitData().m_UserSkillTree.SetEqipSkillMemo( kEvent.m_kUnitInfo.m_UnitSkillData.m_vecSkillNote );
#endif

//#ifdef TITLE_SYSTEM
#ifdef SERV_TITLE_DATA_SIZE
		pUnit->AccessUnitData().m_iTitleId = kEvent.m_kUnitInfo.m_iTitleID;
#else
		pUnit->AccessUnitData().m_iTitleId = kEvent.m_kUnitInfo.m_sTitleID;
#endif

		if(g_pData != NULL && g_pData->GetTitleManager() != NULL)
		{
			g_pData->GetTitleManager()->InitRecord();
			g_pData->GetTitleManager()->AttachTitle(pUnit->GetUnitData().m_iTitleId);

			g_pData->GetTitleManager()->TakeTitle(kEvent.m_vecTitle);     
			g_pData->GetTitleManager()->UpdateMission(kEvent.m_vecMission);
		}                
//#endif

#ifdef SERV_PET_SYSTEM

		pUnit->SetSummonPetUid( kEvent.m_iSummonedPetUID );
		//if( g_pData != NULL && g_pData->GetPetManager() != NULL )
		//	g_pData->GetPetManager()->Handler_EGS_SUMMON_PET_REQ( kEvent.m_iSummonedPetUID );
		if( kEvent.m_vecPetList.size() > 0 )
			pUnit->SetFullPetInfo(kEvent.m_vecPetList[0]);
		
		if( g_pData != NULL && g_pData->GetPetManager() != NULL )
		{
			g_pData->GetPetManager()->AddMyPetList( kEvent.m_vecCanEvolutionPetList );
			g_pData->GetPetManager()->AddNeverSummonPetList( kEvent.m_vecNeverSummonPetList, kEvent.m_iSummonedPetUID );

		}
		
		// oasis907 : 김상윤 [2010.9.7] // 
		if( kEvent.m_vecPetList.size() > 0 )
		{
			KPetInfo kPetInfo = kEvent.m_vecPetList[0];
			int EvolutionStep = kPetInfo.m_cEvolutionStep;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kPetInfo.m_iPetID, EvolutionStep);
#else //SERV_PETID_DATA_TYPE_CHANGE
			int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kPetInfo.m_cPetID, EvolutionStep);
#endif //SERV_PETID_DATA_TYPE_CHANGE

			pUnit->AccessInventory().SetItemMaxNum(CX2Inventory::ST_PET, iInvenSlotSize);
		
		

			// oasis907 : 김상윤 [2010.9.8] // 자동 먹이 주기 기능
			if( g_pData->GetUIManager()->GetUIInventory() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->SetAutoFeed(kPetInfo.m_bAutoFeed);
			}
			//}}
		}
		//}}
#endif


		g_pMain->GetGameOption().SetDenyInviteGuild( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyInviteGuild );				
		g_pMain->GetGameOption().SetDenyPersonalTrade( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyPersonalTrade );
		g_pMain->GetGameOption().SetDenyFriendship( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyFriendShip );
		g_pMain->GetGameOption().SetDenyParty( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyParty );

		g_pMain->GetGameOption().SetRefuseParty( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyParty );
		g_pMain->GetGameOption().SetRefuseFriend( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyFriendShip );
		g_pMain->GetGameOption().SetRefusePersonalTrade( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyPersonalTrade );
		//{{ kimhc // 2009-10-12 // 길드 초대 거부 추가
#ifdef	GUILD_MANAGEMENT
		g_pMain->GetGameOption().SetRefuseGuildInvitation( static_cast< CX2GameOption::BlackListDenyState >( kEvent.m_kDenyOptions.m_cDenyInviteGuild ) );
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-10-12 // 길드 초대 거부 추가
		
		//g_pMain->GetGameOption().SetBlackList( kEvent.m_mapBlackList );
		


		// 클리어한 던전중에서 가장 높은 레벨의 던전을 자동 선택
		if( NULL != g_pData->GetPartyManager() )
		{
			g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID = g_pData->GetMyUser()->GetSelectUnit()->GetLastClearDungeonID();

			// 클리어한 던전이 하나도 없을 경우에 엘의 나무 서쪽 normal을 선택해준다
			switch( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )
			{
			case -1:
			case SEnum::DI_NONE:
			case SEnum::DI_EL_FOREST_GATE_NORMAL:
				//{{ kimhc // 2010-07-09 // 루벤던전 개편으로 추가
			case SEnum::DI_EL_FOREST_NORTH_NORMAL:
			case SEnum::DI_EL_FOREST_WEST_NORMAL:
			case SEnum::DI_EL_FOREST_HELL_NORMAL:
			case SEnum::DI_RUBEN_SECRET_COMMON:
				//}} kimc	// 2010-07-09 // 루벤던전 개편으로 추가
				{
					g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;
				} break;
			}

		//{{ kimhc // 2010.5.2 // 비밀던전 작업(파티리스트)
		#ifdef SERV_SECRET_HELL
			switch ( ( g_pData->GetDungeonManager()->GetDungeonType( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) )
			{
			case CX2Dungeon::DT_SECRET:
				{
					g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode = CX2Dungeon::DM_SECRET_NORMAL;
				} break;

			default:
				break;
			}
		#endif SERV_SECRET_HELL
		//}} kimhc // 2010.5.2 // 비밀던전 작업(파티리스트)
		}

	//{{ kimhc // 2010.4.12 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef SERV_SECRET_HELL
		g_pInstanceData->SetPreCheckTimeForDailyQuest( g_pData->GetServerCurrentTime() );
#endif SERV_SECRET_HELL
	//}} kimhc // 2010.4.12 // 비밀던전 작업(일일퀘스트 시스템)

		g_pData->GetQuestManager()->SetUnitQuest( kEvent.m_vecQuest, kEvent.m_vecCompletQuest );
		
		//{{ kimhc // GiveUpForbiddenQuest()를 EGS_GET_MY_INVENTORY_ACK로 이동함
	#ifndef	SERV_EPIC_QUEST
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	#endif	SERV_EPIC_QUEST
		//}} kimhc // GiveUpForbiddenQuest()를 EGS_GET_MY_INVENTORY_ACK로 이동함


#ifdef	SERV_EPIC_QUEST
		// oasis907 : 김상윤 [2010.7.21] // 퀵퀘스트 창 열기 초기화
		if(g_pData->GetUIManager() != NULL &&
			g_pData->GetUIManager()->GetUIQuestNew() != NULL)
		{
			g_pData->GetUIManager()->GetUIQuestNew()->InitUserShowQuickQuestDLG(true);
		}
#endif	SERV_EPIC_QUEST

		g_pMain->GetInformerManager()->Reset();
		g_pMain->GetLVUpEventMgr()->Reset( pUnit->GetUnitData().m_Level );

		if ( g_pData->GetCashShop() != NULL )
			g_pData->GetCashShop()->ResetUnitViewer( pUnit );

		g_pMain->ResetCrashReport( L"" );


#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
		if ( g_pMain->GetNexonLoginMessenger() == true )
		{
			wstring tempNickName = CX2Community::ConvertFullName( pUnit->GetNickName() );
			switch( g_pMain->GetPublisher() )
			{
#ifdef _NEXON_KR_
				case CX2Main::XP_NEXON_KOREA:
					{
#ifdef _USE_NEXON_MSG_INHOUSE // 접속할 캐릭터의 serialnum을 하드코딩시켜준후 넥슨로그인한다.
						pUnit->AccessUnitData().m_iNMKSerialNum = 52890;	// "지끄무" 캐릭터
						//pUnit->AccessUnitData().m_iNMKSerialNum = 3712941;	// "피똥싼이브" 캐릭터
						//pUnit->AccessUnitData().m_iNMKSerialNum = 1928754;	// "툩" 캐릭터
#endif

						if( CNMCOClientObject::GetInstance().LoginVirtual( tempNickName.c_str(), (_UInt32_)pUnit->GetUnitData().m_iNMKSerialNum, GET_STRING( STR_ID_0 ) ) == FALSE )						
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_710 ), g_pMain->GetNowState() );
						}
						else
						{
							g_pMain->SetNexonVirtualLogin( true );							
						}

						CNMCOClientObject::GetInstance().ChangeMyLevel( ( (UINT32)pUnit->GetUnitData().m_UnitClass << 24 ) | (UINT32)pUnit->GetUnitData().m_Level );



						// 닉네임 바꾼 직후라면 닉네임이 바뀌었다고 넥슨메신저에 이벤트 전송
						if( true == m_bJustChangedNickName )
						{
							m_bJustChangedNickName = false;

							NMVirtualKey nmVirtualKey;
							nmVirtualKey.uGameCode		= NEXON_KOREA_ELSWORD_GAMECODE;
							nmVirtualKey.uVirtualIDCode = (UINT32) pUnit->GetUnitData().m_iNMKSerialNum;

							if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // 체험 아이디 제한
							{
								if ( CNMCOClientObject::GetInstance().ChangeNickname( nmVirtualKey, tempNickName.c_str() ) == FALSE )
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_711 ), this );
								}
							}
						}
					}
					break;
#endif _NEXON_KR_
			}
		}
#endif NEW_MESSENGER
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP


		Handler_EGS_CHECK_BALANCE_REQ();


		//{{ kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할
#ifdef	PC_BANG_WORK
		g_pData->GetServerProtocol()->SendID( EGS_GET_MY_INVENTORY_REQ );
		g_pMain->AddServerPacket( EGS_GET_MY_INVENTORY_ACK  );
#else	PC_BANG_WORK
		
		// 체험 아이디 제한 
		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
			kEvent.m_bIsRecommend == false && m_bCheckRecommend == true )
		{
			OpenRecommendMsgBox( true );
		}
		else
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_PartyTalkBoxInfo.m_iPartyUID = 0;

			if(g_pMain->GetIsPlayingTutorial() == true)
			{
				// 튜토리얼 진입요청
				Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
			}
			else
			{
				// 새마을로 진입요청
				Handler_EGS_STATE_CHANGE_FIELD_REQ();
			}
		}

#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할

		


		//{{ kimhc // 2009-12-04 // 크리스마스 이벤트
#ifndef	CHRISTMAS_TREE
		// 마케팅 이벤트 시계 표시
		g_pData->GetMyUser()->GetSelectUnit()->GetTimerGuestUser().restart();
		g_pData->GetMyUser()->GetSelectUnit()->SetReceivedGuestUserReward( true );
		g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventTime( -1.f );

		std::map< int, float >::iterator it;
		for( it = kEvent.m_mapEventTime.begin() ; it != kEvent.m_mapEventTime.end(); it++ )
		{
			g_pData->GetMyUser()->GetSelectUnit()->SetReceivedGuestUserReward( false );
			if( it->second > g_pData->GetMyUser()->GetSelectUnit()->GetMarketingEventTime() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventID( it->first );
				g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventTime( it->second );
			}
		}

#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-04 // 크리스마스 이벤트


		// 친구채팅창 히스토리 초기화 및 친구리스트 초기 설정
		if(g_pData != NULL && g_pData->GetMessenger() != NULL)
		{
			g_pData->GetMessenger()->ClearChatSessionList();
			g_pData->GetMessenger()->Reset();		
			g_pData->GetMessenger()->ResetHistory();
#ifdef NEW_MESSENGER
			g_pData->GetMessenger()->SetFriendList( kEvent.m_kMessengerInfo );
#else		
			g_pData->GetMessenger()->SyncFriendList();
#endif NEW_MESSENGER
		}




		if ( g_pInstanceData != NULL )
		{
			g_pInstanceData->SetShowCSPandSlotBDate( true );
#ifdef SERV_HACKING_TOOL_LIST
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
            g_pInstanceData->ReSetHackList_MainThread();
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
			g_pInstanceData->SetChangeHackList(true);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
#endif
		}


		//{{ 허상형 : [2009/11/3] //	채팅 박스 초기화
		if( g_pChatBox != NULL )
		{
			g_pChatBox->ClearAllChatLog();
			g_pChatBox->ChangeChatWindowAndIMEEditBox( CX2ChatBox::CWM_TOTAL );
		}
	}
	else if( kEvent.m_iOK == NetError::ERR_SELECT_UNIT_06 )
	{
		//닉네임이 없는 캐릭터는 닉네임을 새로 입력하라는 팝업창을 띄운다.
		OpenRenameMsgBox( true );
		SetRenameMsgBoxError( L"" );
	}
	//{{ 2009. 4. 29  최육사	잘못된 상태에서 SELECT_UNIT_REQ를 보내면 돌아오는 ACK 에러 코드에 대한 처리
	else if( kEvent.m_iOK == NetError::ERR_WRONG_STATE_00 )
	{
		// 정상처리?
		return true;
	}
	//}} 2009. 4. 29  최육사
	else
	{
		if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
		{
			g_pMain->GetStateAutoChanger().EndStateChange();
		}
	}


	m_bJustChangedNickName = false;


	// kimhc // 2010-02-03 // PC방인 경우 PC방 탭이 디폴드가 되도록 변경
	if ( g_pInstanceData->m_bFirstSelect == false &&
		 g_pData->GetMyUser() != NULL &&
		 g_pData->GetMyUser()->GetIsPCRoom() == true )
	{
		if ( g_pData->GetUIManager() != NULL &&
			g_pData->GetUIManager()->GetUIInventory() != NULL )
			g_pData->GetUIManager()->GetUIInventory()->SetSortType( CX2Inventory::ST_PC_BANG );
	}

	g_pInstanceData->m_bFirstSelect = true;


	//{{ kimhc // 2009-01-12 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	if ( g_pData != NULL &&
		 g_pData->GetMessenger() != NULL )
	{
		switch ( g_pInstanceData->GetServerGroupID() )
		{
		case SGI_GAIA:
			{
				g_pData->GetMessenger()->SetRefereeUI( true );
			}
			break;

		default:
			{
				g_pData->GetMessenger()->SetRefereeUI( false );
			}
			break;
		}
	}
#endif	VIEW_REFEREE_LIST
	//}} kimhc // 2009-01-12 // 가이아 서버에만 추천인 리스트 보이는 기능

	return true;
}

//bool CX2StateUnitSelect::SelectUnitTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"서버에 응답이 없습니다\n유닛을 선택할 수 없습니다", this );
//return true;
//}

bool CX2StateBeginning::Handler_EGS_DELETE_UNIT_REQ()
{
	if( m_pSelectUnit == NULL )
		return true;

	KEGS_DELETE_UNIT_REQ kPacket;
	kPacket.m_iUnitUID		= m_pSelectUnit->GetUID();

	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DELETE_UNIT_ACK  );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_712 ), this );

	return true;
}

bool CX2StateBeginning::Handler_EGS_DELETE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_DELETE_UNIT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_iUnitUID );
			if ( pUnit == NULL )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_713 ), this );
				return true;
			}

			ClearBeforeUnit( pUnit );
			ClearUnitButton();
			g_pData->GetMyUser()->DeleteUnitByUID(kEvent.m_iUnitUID);
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_714 ), this );
			CreateUnitButton();
		}
	}
	return true;
}

//bool CX2StateUnitSelect::DeleteUnitTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"서버에 응답이 없습니다\n유닛을 지울 수 없습니다.", this );
//
//return true;
//}


bool CX2StateBeginning::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ()
{
	if ( g_pMain->GetIsPlayingTutorial() == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGMsgBox );
		m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_716 ), this );
		Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
	}
	else
	{
		SAFE_DELETE_DIALOG( m_pDLGMsgBox );
		m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_545 ), this );
		Handler_EGS_STATE_CHANGE_FIELD_REQ();
	}

	return true;
}

bool CX2StateBeginning::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//#if 0 
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KStateChangeAck kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_VILLAGE_MAP_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			g_pMain->CreateStateChangeDLG( L"마을맵으로 이동중입니다." );
//			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, kEvent.m_nMapID, false );
//		}
//	}
//#endif


	return true;
}






bool CX2StateBeginning::Handler_EGS_CHANGE_NICK_NAME_REQ( UidType uid, const WCHAR* wszNickName, bool bCheckOnly )
{
	if( NULL == wszNickName )
		return false;

	KEGS_CHANGE_NICK_NAME_REQ kPacket;
	kPacket.m_iUnitUID		= uid;
	kPacket.m_wstrNickName	= wszNickName;
	kPacket.m_bCheckOnly	= bCheckOnly;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_NICK_NAME_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_NICK_NAME_ACK );

	return true;
}


bool CX2StateBeginning::Handler_EGS_CHANGE_NICK_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_CHANGE_NICK_NAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_CHANGE_NICK_NAME_ACK ) == true )
	{
		m_bJustChangedNickName = false;



		if( true == kEvent.m_bCheckOnly )
		{
			switch( kEvent.m_iOK )
			{
			case NetError::ERR_RESTORE_NICK_NAME_00:			
			case NetError::ERR_RESTORE_NICK_NAME_01:			
			case NetError::ERR_RESTORE_NICK_NAME_02:			
			case NetError::ERR_RESTORE_NICK_NAME_03:			
			case NetError::ERR_RESTORE_NICK_NAME_04:			
			case NetError::ERR_RESTORE_NICK_NAME_05:			
			case NetError::ERR_RESTORE_NICK_NAME_06:			
				{
					SetRenameMsgBoxError( NetError::GetErrStrF( kEvent.m_iOK ) );
					return true;
				} break;
			}


			if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{
				// 닉네임 변경 가능 확인 완료. 진짜 바꿀지 확인 팝업창 띄우기
				SAFE_DELETE_DIALOG( m_pDLGRenameConfirmMsgBox );
				m_pDLGRenameConfirmMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_2556, "S", m_NickNameChangeUnitNickName.c_str() ) ), SUSUCM_RENAME_CONFIRM_OK, this, SUSUCM_RENAME_CONFIRM_CANCEL );

				SetRenameMsgBoxError( L"" );
			}
		}
		else
		{
			if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{
				// 닉네임 변경 완료. 캐릭터 선택해서 게임입장.
				OpenRenameMsgBox( false );

				m_pDLGRenameSuccessMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_2557, "S", kEvent.m_wstrNickName.c_str() ) ), this, SUSUCM_RENAME_SUCCESS_OK );
			}
		}
	}

	return true;
}

//{{ kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할
#ifdef	PC_BANG_WORK
bool CX2StateBeginning::Handler_EGS_SELECT_UNIT_INVENTORY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_INVENTORY_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pData == NULL || g_pData->GetMyUser() == NULL )
	{
		ASSERT( !L"g_pData or g_pData->GetMyUser In EGS_SELECT_UNIT_INVENTORY_INFO_NOT is WRONG" );
		return false;
	}

	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();

	if ( pMyUnit != NULL )
	{
		pMyUnit->AccessInventory().ResetItems( kEvent.m_mapItem );
		return true;
	}

	ASSERT( !L"pMyUnit or GetInventory In EGS_SELECT_UNIT_INVENTORY_INFO_NOT is WRONG" );
	return false;	
}

bool CX2StateBeginning::Handler_EGS_GET_MY_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_MY_INVENTORY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( false == g_pMain->DeleteServerPacket( EGS_GET_MY_INVENTORY_ACK ) )
	{
		StateLog( L"Invalid EGS_GET_MY_INVENTORY_ACK" );
		return false;
	}
	

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
#ifdef SERV_RECOMMEND_LIST_EVENT
		g_pData->Set_ShowRecommendUI( !kEvent.m_bIsRecommend );
#endif //SERV_RECOMMEND_LIST_EVENT		
		// 체험 아이디 제한 
		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
			kEvent.m_bIsRecommend == false && m_bCheckRecommend == true )

		{
			OpenRecommendMsgBox( true );
		}
		else
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_PartyTalkBoxInfo.m_iPartyUID = 0;

			if(g_pMain->GetIsPlayingTutorial() == true)
			{
				// 튜토리얼 진입요청
				Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
			}
			else
			{
				// 새마을로 진입요청
				Handler_EGS_STATE_CHANGE_FIELD_REQ();
			}
		}

		//{{ kimhc // GiveUpForbiddenQuest()를 EGS_SELECT_UNIT_ACK에서 이동함
	#ifdef	SERV_EPIC_QUEST
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	#endif	SERV_EPIC_QUEST
		//}} kimhc // GiveUpForbiddenQuest()를 EGS_SELECT_UNIT_ACK에서 이동함

		return true;
	}

	return false;

}
#endif	PC_BANG_WORK
//}} kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할


void CX2StateBeginning::SetRenameMsgBoxError( const wstring& wstrErrorMessage )
{
	if( NULL == m_pDLGRenameMsgBox )
		return;


	CKTDGUIStatic* pStaticErrorMessage = (CKTDGUIStatic*) m_pDLGRenameMsgBox->GetControl( L"NickNameErrorMessage" );
	if( NULL == pStaticErrorMessage )
		return;


	if( NULL == pStaticErrorMessage->GetString(0) )
		return;

	
	pStaticErrorMessage->GetString(0)->msg = wstrErrorMessage;
}


void CX2StateBeginning::OpenRenameMsgBox( bool bOpen )
{
	if( true == bOpen )
	{
		if( NULL == m_pDLGRenameMsgBox )
		{
			m_pDLGRenameMsgBox = new CKTDGUIDialog( this, L"DLG_Change_Nickname.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRenameMsgBox );
		}

		m_pDLGRenameMsgBox->SetShowEnable( true, true );
	}
	else
	{
		if( NULL != m_pDLGRenameMsgBox )
		{
			m_pDLGRenameMsgBox->SetShowEnable( false, false );
		}
	}
}


bool CX2StateBeginning::Handler_EGS_RECOMMEND_USER_REQ( const WCHAR* wszNickName, bool bCancel )
{
	KEGS_RECOMMEND_USER_REQ kPacket;
	kPacket.m_wstrNickName = wszNickName;
	kPacket.m_wstrNickName = kPacket.m_wstrNickName.substr( 0, 20 );
	kPacket.m_bCancel = bCancel;

	if ( kPacket.m_bCancel == false && g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, kPacket.m_wstrNickName.c_str() ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_717 ), this );
		return true;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_RECOMMEND_USER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RECOMMEND_USER_ACK );

	return true;
}

bool CX2StateBeginning::Handler_EGS_RECOMMEND_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_RECOMMEND_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_RECOMMEND_USER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_RECOMMEND_LIST_EVENT
			g_pData->Set_ShowRecommendUI( false );
#endif //SERV_RECOMMEND_LIST_EVENT	
			OpenRecommendMsgBox( false );
			g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() + kEvent.m_iRewardResurrectionStone );
			if ( kEvent.m_iRewardResurrectionStone > 0 )
			{
				m_bCheckRecommendSuccess = true;
				wstringstream wstrstm;
				wstrstm << GET_REPLACED_STRING( ( STR_ID_718, "i", kEvent.m_iRewardResurrectionStone ) );
				m_pDLGRecommendSuccess = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this );
			}
			else
			{
				Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
				//Handler_EGS_GET_POST_LETTER_LIST_REQ();
			}

			return true;
		}
	}

	return false;
}

void CX2StateBeginning::OpenRecommendMsgBox( bool bOpen )
{
	if ( bOpen == true )
	{
		if ( m_pDLGRecommend != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRecommend, NULL, false );

		m_pDLGRecommend = NULL;
#ifdef SERV_RECOMMEND_LIST_EVENT
		m_pDLGRecommend = new CKTDGUIDialog( this, L"DLG_Recommend2.lua" );
#else
		m_pDLGRecommend = new CKTDGUIDialog( this, L"DLG_Recommend.lua" );
#endif //SERV_RECOMMEND_LIST_EVENT
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRecommend );

		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGRecommend->GetControl( L"g_pIMEEditBoxName" );
		if ( pIMEEditBox != NULL )
		{
			pIMEEditBox->RequestFocus();
		}
	}
	else
	{
		if ( m_pDLGRecommend != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRecommend, NULL, false );

		m_pDLGRecommend = NULL;
	}
}





bool CX2StateBeginning::Handler_EGS_SELECT_SERVER_SET_REQ()
{
	if ( m_sbSelectedServerSet == true )
		return true;

	KEGS_SELECT_SERVER_SET_REQ kPacket;
	//kPacket.m_ServerSetID = iServerSetIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_SELECT_SERVER_SET_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SELECT_SERVER_SET_ACK );

	return true;	
}

bool CX2StateBeginning::Handler_EGS_SELECT_SERVER_SET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_SELECT_SERVER_SET_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	
	if( g_pMain->DeleteServerPacket( EGS_SELECT_SERVER_SET_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )	
		{
			m_sbSelectedServerSet = true;
#ifdef ELSWORD_NEW_BEGINNING
			m_bChannelSelectWhiteOut = true;
			g_pData->GetEventScene()->BlackLayOutMove( true, 0.f, false, true, 
												_CONST_STATE_BEGINNING_::g_fTimeChannelSelectWhiteOut, 1.0f );
			m_fChannelSelectWhiteOutElapsedTime = 0.f;
#else
			Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ();
#endif ELSWORD_NEW_BEGINNING
		}
		else
		{
			SAFE_DELETE_DIALOG( m_pDLGMsgBox );

			switch( g_pData->GetMyUser()->GetAuthLevel() )
			{
			case -2:
			case -3:
				//{{ 2010. 06. 30  최육사	계정블럭추가
			case -5:
			case -6:
				//}}
				//{{ 계정 제재 일수 상태 추가 - 김민성
//#ifdef SERV_ADD_BLOCK_DAY
			case -7:
			case -8:
			case -9:
//#endif SERV_ADD_BLOCK_DAY
				//}}
				{
                    wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_719, "L", kEvent.m_kAccountBlockInfo.m_wstrEndTime ) );
					wstrMsg += kEvent.m_kAccountBlockInfo.m_wstrBlockReason;
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,150), wstrMsg.c_str(), (CKTDXStage*)this );
				} break;

			case -4:
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,150), kEvent.m_kAccountBlockInfo.m_wstrBlockReason.c_str(), (CKTDXStage*)this );
				} break;
			default:
#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-04	// 박세훈
				if( ( SEnum::HUCT_NONE < kEvent.m_kAccountBlockInfo.m_byteBlockType ) &&
					( kEvent.m_kAccountBlockInfo.m_byteBlockType < SEnum::HUCT_MAX )
					)
				{
					wstring wstrMsg;
					if( kEvent.m_kAccountBlockInfo.m_wstrBlockEndDate.empty() == false )
					{
						wstrMsg = GET_REPLACED_STRING( ( STR_ID_719, "L", kEvent.m_kAccountBlockInfo.m_wstrBlockEndDate ) );

						if( kEvent.m_kAccountBlockInfo.m_wstrBlockReason2.empty() == false )
						{
							wstrMsg += L"\n";
							wstrMsg += kEvent.m_kAccountBlockInfo.m_wstrBlockReason2;
						}

						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,150), wstrMsg.c_str(), (CKTDXStage*)this );
					}
					else if( kEvent.m_kAccountBlockInfo.m_wstrBlockReason2.empty() == false )
					{
						wstrMsg = kEvent.m_kAccountBlockInfo.m_wstrBlockReason2;
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,150), wstrMsg.c_str(), (CKTDXStage*)this );
					}
				}
#endif // SERV_HACKING_USER_CHECK_COUNT
				break;
			}
#ifdef CLIENT_COUNTRY_TWHK
			// 올바른 빈펀 계정이 아니었다는 NetError인지 여기서 검사해서 클라를 강제종료시킨다
#endif CLIENT_COUNTRY_TWHK
		}
	}

	// 서버선택 및 채널 선택시 서버/채널선택창은 사라지고 유닛선택창이 나온다.
	m_pDLGServerSelectBack->SetShowEnable(false, false);
	m_pDLGUnitSelectFront->SetShowEnable(true, true);

	//{{ kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
#ifdef	ADD_SERVER_GROUP
	m_pDLGChannelSelectBack->SetShowEnable( false, false );
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리

	// 채널번호 표시
	if( g_pInstanceData->GetConnectChannelServerID() > 0 )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Channel_Name" );
		pStatic->GetString(0)->msg = g_pInstanceData->GetConnectChannelServerName();
	}
	

	return true;
}

bool CX2StateBeginning::Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ()
{
	/*
	KEVENT_X2_SERVER_UNIT_INFO_LIST_REQ	kEVENT_X2_SERVER_UNIT_INFO_LIST_REQ;
	kEVENT_X2_SERVER_UNIT_INFO_LIST_REQ.userUID = g_pData->GetMyUser()->GetUID();
*/
	g_pData->GetServerProtocol()->SendID( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ );
	g_pMain->AddServerPacket( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_720 ), this );
	return true;
}

bool CX2StateBeginning::Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	
	if( g_pMain->DeleteServerPacket( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			ClearBeforeUnit( m_pSelectUnit );
			ClearUnitButton();

			g_pData->GetMyUser()->DeleteAllUnit();

			g_pData->GetMyUser()->AccessUserData().maxUnitCount = kEvent.m_nUnitSlot; //생성가능 유닛 슬롯?

			m_MaxUnitNum	= kEvent.m_nUnitSlot;



            for( int i =0; i < (int)kEvent.m_vecUnitInfo.size(); i++ )
			{	 

				KUnitInfo kUnitInfo = kEvent.m_vecUnitInfo[i];
				CX2Unit* pUnit = new CX2Unit( kUnitInfo );

				pUnit->SetOwnerUserUID( g_pData->GetMyUser()->GetUID() );
				wstring nickNameToChange = L"__DELETED__";
				if ( nickNameToChange == pUnit->GetUnitData().m_NickName )
				{
					wstringstream tempNickName;
					tempNickName << nickNameToChange.c_str() << i;
					pUnit->AccessUnitData().m_NickName = tempNickName.str().c_str();
				}
				g_pData->GetMyUser()->AddUnit( pUnit );
			}


			m_NowPage		= 1;
			const int SLOT_COUNT_FOR_CREATE_UNIT = 1;
			if( g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
			{
				m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+SLOT_COUNT_FOR_CREATE_UNIT+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
			}
			else
			{
				m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
			}



			
			CreateUnitButton(true);

			m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK = true;

#ifdef ELSWORD_NEW_BEGINNING	
			g_pData->GetEventScene()->BlackLayOutMove( false, 0.5f, false, true, 
														_CONST_STATE_BEGINNING_::g_fTimeChannelSelectWhiteIn, 1.0f );
			g_pBeginningGame->PlayWorldCamera(2);
#endif ELSWORD_NEW_BEGINNING
			
		}
		else
		{
			g_pData->DeleteServerProtocol();
			g_pData->DeleteMyUser();
		}
	}

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	return true;
}




bool CX2StateBeginning::Handler_EGS_AGREE_HACK_USER_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_AGREE_HACK_USER_REQ );
	g_pMain->AddServerPacket( EGS_AGREE_HACK_USER_ACK, 500.0f );

	return true;
}

bool CX2StateBeginning::Handler_EGS_AGREE_HACK_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_AGREE_HACK_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	
	if( g_pMain->DeleteServerPacket( EGS_AGREE_HACK_USER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifndef PROCESSLIST
			g_pMain->UpdateProcessList();
#endif
			g_pData->GetMyUser()->AccessUserData().hackingUserType = (CX2User::HACKING_USER_TYPE)kEvent.m_cHackingUserType;
			SAFE_DELETE_DIALOG( m_pDLGCheckHack );
		}
		else
		{
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
		}
	}
	return true;
}
//{{ kimhc // 2009-12-15 // 이전에 플레이했던 채널 서버군
#ifdef	ADD_SERVER_GROUP

void CX2StateBeginning::UpdateChannelListUI()
{
	SAFE_DELETE_DIALOG( m_pDLGChannelSelectBack );


	switch ( g_pInstanceData->GetServerGroupID() )
	{
	case SGI_GAIA:
		{
#ifdef ELSWORD_NEW_BEGINNING
			m_pDLGChannelSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection02_TEST.lua" );
#else
			m_pDLGChannelSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection02.lua" );
#endif ELSWORD_NEW_BEGINNING
		}
		break;

	case SGI_SOLES:
	default:
		{
#ifdef ELSWORD_NEW_BEGINNING
			m_pDLGChannelSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection01_TEST.lua" ); //new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection01.lua" );
#else
			m_pDLGChannelSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection01.lua" ); //new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection01.lua" );
#endif ELSWORD_NEW_BEGINNING
		}
		break;
	}

	if ( m_pDLGChannelSelectBack == NULL )
		return;
	
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChannelSelectBack );

	// 여기서 부터 채널 리스트 셋팅

	const int MAX_CHANNEL_COUNT = m_iChannelNum;	// 채널 갯수

	// 채널 리스트 버튼 초기화
	WCHAR wszControlName[32] = L"";

	// 채널 혼잡도 게이지
	CKTDGUIStatic* pStaticCHCongestionGage	= static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( L"chanal_line" ) );		
	// 채널 회색 박스
	CKTDGUIStatic* pStaticCHBox				= static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( L"chanal_state" ) );		
	// 채널이름 ex)루벤1
	CKTDGUIStatic* pStaticCHName			= static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( L"chanal_num" ) );		

	// 전체 채널 다 보이지 않도록 초기화
	for ( int i = 0; i < MAX_CHANNEL_COUNT; ++i )
	{
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"chanal%d", i + 1 );
		CKTDGUIButton *pChannelButton = static_cast< CKTDGUIButton* >( m_pDLGChannelSelectBack->GetControl( wszControlName ) );
		
		if( pChannelButton != NULL )
			pChannelButton->SetShowEnable(false, false);		

		// 채널 혼잡도 스트링 ex) 보통, 원활
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"statechanal%d", i + 1);
		CKTDGUIStatic* pStaticCHCongestionStr = static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( wszControlName ) );
		
		if(pStaticCHCongestionStr != NULL)
			pStaticCHCongestionStr->SetShowEnable(false, false);			

		pStaticCHCongestionGage->GetPicture( i * 3 )->SetShow(false);
		pStaticCHCongestionGage->GetPicture( i * 3 + 1 )->SetShow(false);
		pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetShow(false);

		pStaticCHBox->GetPicture( i * 3 )->SetShow(false);
		pStaticCHBox->GetPicture( i * 3 + 1 )->SetShow(false);
		pStaticCHBox->GetPicture( i * 3 + 2 )->SetShow(false);

		pStaticCHName->GetPicture( i )->SetShow(false);
	}

#ifdef ELSWORD_NEW_BEGINNING

	// 반투명 회색 배경도 끈다.
// 	CKTDGUIStatic* pStaticChannelBG	= static_cast< CKTDGUIStatic* >( m_pDLGServerSelectBack->GetControl( L"chanal" ) );		
// 	pStaticChannelBG->SetShow(false);

	// 채널 바도 끈다
	CKTDGUIStatic* pStaticBar = static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( L"chanal_bar" ) );		
	pStaticBar->SetShow(false);
#endif ELSWORD_NEW_BEGINNING


	if( false == m_bReceivedChannelList )	// 채널리스트를 받지 않았으면 리턴
		return;

#ifdef ELSWORD_NEW_BEGINNING
	// oasis907 : 김상윤 [2010.11.15] // 
	if( m_eNowState != BG_STATE_CHANNEL_SELECT)
	{
		return;
	}
#endif ELSWORD_NEW_BEGINNING

#ifdef CHANGE_CHANNEL
	if( g_pInstanceData == NULL || g_pInstanceData->m_vecChannelInfo.empty() == true)
		return;
#else
	if( true == m_vecChannelInfo.empty() )
		return;
#endif

	// 채널 게이지 표기법 변경
	float			CHANNEL_CONGESTION_SCOPE_TEMP[] = { 0.1f,	0.2f,	0.3f,	0.999f, };
	const float		CHANNEL_CONGESTION_SCOPE[]		= { 0.1f,	0.2f,	0.3f,	0.999f, };//{ 0.2f,	0.4f,	0.6f,	0.999f, };
	const float		CHANNEL_CONGESTION_DISPLAY[]	= { 0.25f,	0.5f,	0.75f,	0.99f,	};
	
#ifdef ELSWORD_NEW_BEGINNING
	// 반투명 회색 배경 켜기.
	//pStaticChannelBG->SetShow(true);
	// 채널 바 켜기
	pStaticBar->SetShow(true);
#endif ELSWORD_NEW_BEGINNING


	// 채널 리스트 정보 갱신
#ifdef CHANGE_CHANNEL
	for( int i = 0; i < static_cast< int >( g_pInstanceData->m_vecChannelInfo.size() )&& i < MAX_CHANNEL_COUNT; i++ )
#else
	for( int i = 0; i < static_cast< int >( m_vecChannelInfo.size() ) && i < MAX_CHANNEL_COUNT; i++ )
#endif
	{	
#ifdef CHANNEL_DOWN_EMERGENCY
		// oasis907 : 김상윤 [2010.10.28] // 특정채널 임시로 빼기
		if(i == 10 && g_pInstanceData->GetServerGroupID() == SGI_GAIA)
		{
			continue;
		}
#endif CHANNEL_DOWN_EMERGENCY


		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"chanal%d", i + 1 );
		CKTDGUIButton *pChannelButton = static_cast< CKTDGUIButton* >( m_pDLGChannelSelectBack->GetControl( wszControlName ) );

		// 채널 혼잡도 스트링 ex) 보통, 원활
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"statechanal%d", i + 1);
		CKTDGUIStatic* pStaticCHCongestionStr = static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( wszControlName ) );

		g_pInstanceData->SetChannelDistributionByServerGroup( CHANNEL_CONGESTION_SCOPE_TEMP, CHANNEL_CONGESTION_SCOPE, i );
		// Show 설정
		pChannelButton->SetShowEnable(true, true);
		pStaticCHCongestionStr->SetShowEnable(true, true);
		pStaticCHBox->GetPicture( i * 3 )->SetShow(true);
		pStaticCHBox->GetPicture( i * 3 + 1 )->SetShow(true);
		pStaticCHBox->GetPicture( i * 3 + 2 )->SetShow(true);
		pStaticCHName->GetPicture( i )->SetShow(true);
		
#ifdef CHANGE_CHANNEL
		const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[i];
#else
		const KChannelInfo& channelInfo = m_vecChannelInfo[i];
#endif
		float fChannelCongestionRate = static_cast< float >( channelInfo.m_iCurrentUser ) / static_cast< float >( channelInfo.m_iMaxUser );
		float fChannelCongestionRateDisplay = 0.f;
		if( fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[3] )
		{
			fChannelCongestionRateDisplay = 1.f;
		}
		else if( fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[2] )
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[3]; // 혼잡
		}
		else if( fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[1] )
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[2]; // 보통
		}
		else if( fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[0] )
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[1]; // 원활
		}
		else
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[0]; // 원활
		}

		if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[1])
		{
			pStaticCHCongestionGage->GetPicture( i * 3 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*3)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 3 )->SetShow( true );				
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_721 );
		}
		else if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[2])
		{
			pStaticCHCongestionGage->GetPicture( i * 3 + 1 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*3+1)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 3 + 1)->SetShow( true );
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_722 );
		}
		else if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[3])
		{
			pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*3+2)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetShow( true );
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_723 );
		}
		else
		{
			pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*3+2)->GetOriginalSize().x );
			pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetShow( true );
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_724 );

			pChannelButton->SetShowEnable(true, false);
		}
	}
}

#else	ADD_SERVER_GROUP

void CX2StateBeginning::UpdateChannelListUI()
{
	if( NULL == m_pDLGServerSelectBack )
		return;

	
	// 채널추가
	const int MAX_CHANNEL_COUNT = 16;	


	// 채널 리스트 버튼 초기화
	WCHAR wszControlName[32] = L"";

	CKTDGUIStatic* pStatic1 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"chanal_line" );		// 상태바(3)
	CKTDGUIStatic* pStatic3 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"chanal_state" );	// 3
	CKTDGUIStatic* pStatic4 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"chanal_num" );		// 2
	CKTDGUIStatic* pStaticChannelNum = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"channel_num" );

	for(int i=0; i<MAX_CHANNEL_COUNT; ++i)
	{
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"chanal%d", i+1);
		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGServerSelectBack->GetControl( wszControlName );
		if(pButton != NULL)
			pButton->SetShowEnable(false, false);		

		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"statechanal%d", i+1);
		CKTDGUIStatic* pStatic2 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( wszControlName );		// 상태명
		if(pStatic2 != NULL)
			pStatic2->SetShowEnable(false, false);			

		pStatic1->GetPicture(i*3)->SetShow(false);
		pStatic1->GetPicture(i*3+1)->SetShow(false);
		pStatic1->GetPicture(i*3+2)->SetShow(false);
		pStatic3->GetPicture(i*3)->SetShow(false);
		pStatic3->GetPicture(i*3+1)->SetShow(false);
		pStatic3->GetPicture(i*3+2)->SetShow(false);
		pStatic4->GetPicture(i)->SetShow(false);
		//pStatic4->GetPicture(i*2+1)->SetShow(false);
	}				





	if( false == m_bReceivedChannelList )
		return;

#ifdef CHANGE_CHANNEL
	if( g_pInstanceData == NULL || g_pInstanceData->m_vecChannelInfo.empty() == true)
		return;
#else
	if( true == m_vecChannelInfo.empty() )
		return;
#endif

	// 채널/서버 게이지 표기법 변경
	float CHANNEL_CONGESTION_SCOPE_TEMP[] = {0.1f, 0.2f, 0.3f, 0.999f, };
	const float CHANNEL_CONGESTION_SCOPE[] = {0.2f, 0.4f, 0.6f, 0.999f, };
	const float CHANNEL_CONGESTION_DISPLAY[] = {0.25f, 0.5f, 0.75f, 0.99f, };

	const float CHANNEL_CONGESTION_CRITERION[] = { 0.3f, 0.6f, 0.999f, };			// 서버 혼잡 상태 경계값 30%, 60%, 100%
	int numChannel = 0;
	//int serverLevel = 0; 

	int nMaxNum = 0;
	int nCurrentNum = 0;

	// 채널 리스트 정보 갱신
#ifdef CHANGE_CHANNEL
	for( int i=0; i<(int)g_pInstanceData->m_vecChannelInfo.size() && i<MAX_CHANNEL_COUNT; i++ )
#else
	for( int i=0; i<(int)m_vecChannelInfo.size() && i<MAX_CHANNEL_COUNT; i++ )
#endif
	{			
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"chanal%d", i+1);
		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGServerSelectBack->GetControl( wszControlName );
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"statechanal%d", i+1);
		CKTDGUIStatic* pStatic2 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( wszControlName ); // 상태명

		if(i == 0 || i == 1 || i == 6 || i == 7)
		{
			// 채널그룹중 첫채널은 분포도 다르게 표시
			CHANNEL_CONGESTION_SCOPE_TEMP[0] = 0.1f;
			CHANNEL_CONGESTION_SCOPE_TEMP[1] = 0.2f;
			CHANNEL_CONGESTION_SCOPE_TEMP[2] = 0.3f;
			CHANNEL_CONGESTION_SCOPE_TEMP[3] = 0.999f;
		}
		else
		{
			CHANNEL_CONGESTION_SCOPE_TEMP[0] = CHANNEL_CONGESTION_SCOPE[0];
			CHANNEL_CONGESTION_SCOPE_TEMP[1] = CHANNEL_CONGESTION_SCOPE[1];
			CHANNEL_CONGESTION_SCOPE_TEMP[2] = CHANNEL_CONGESTION_SCOPE[2];
			CHANNEL_CONGESTION_SCOPE_TEMP[3] = CHANNEL_CONGESTION_SCOPE[3];
		}
		

		// Show 설정
		pButton->SetShowEnable(true, true);
		pStatic2->SetShowEnable(true, true);
		pStatic3->GetPicture(i*3)->SetShow(true);
		pStatic3->GetPicture(i*3+1)->SetShow(true);
		pStatic3->GetPicture(i*3+2)->SetShow(true);
		pStatic4->GetPicture(i)->SetShow(true);
		//pStatic4->GetPicture(i*2+1)->SetShow(true);

		numChannel++;

#ifdef CHANGE_CHANNEL
		const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[i];
#else
		const KChannelInfo& channelInfo = m_vecChannelInfo[i];
#endif
		float fChannelCongestionRate = (float) channelInfo.m_iCurrentUser / (float) channelInfo.m_iMaxUser;
		float fChannelCongestionRateDisplay = 0.f;
		if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[3])
		{
			fChannelCongestionRateDisplay = 1.f;
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[2])
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[3]; // 혼잡
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[1])
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[2]; // 보통
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[0])
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[1]; // 원활
		}
		else
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[0]; // 원활
		}


		nMaxNum += channelInfo.m_iMaxUser;
		nCurrentNum += channelInfo.m_iCurrentUser;

		//if( fChannelCongestionRate < CHANNEL_CONGESTION_CRITERION[0] )
		if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[1])
		{
			//serverLevel += 1;
			pStatic1->GetPicture(i*3)->SetSizeX(pStatic1->GetPicture(i*3)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStatic1->GetPicture(i*3)->SetShow( true );				
			pStatic2->GetString(0)->msg = GET_STRING( STR_ID_721 );
		}
		//else if( fChannelCongestionRate < CHANNEL_CONGESTION_CRITERION[1] )
		else if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[2])
		{
			//serverLevel += 2;
			pStatic1->GetPicture(i*3+1)->SetSizeX(pStatic1->GetPicture(i*3+1)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStatic1->GetPicture(i*3+1)->SetShow( true );
			pStatic2->GetString(0)->msg = GET_STRING( STR_ID_722 );
		}
		//else if( fChannelCongestionRate < CHANNEL_CONGESTION_CRITERION[2] )
		else if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[3])
		{
			//serverLevel += 3;
			pStatic1->GetPicture(i*3+2)->SetSizeX(pStatic1->GetPicture(i*3+2)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStatic1->GetPicture(i*3+2)->SetShow( true );
			pStatic2->GetString(0)->msg = GET_STRING( STR_ID_723 );
		}
		else
		{
			//serverLevel += 4;
			pStatic1->GetPicture(i*3+2)->SetSizeX(pStatic1->GetPicture(i*3+2)->GetOriginalSize().x );
			pStatic1->GetPicture(i*3+2)->SetShow( true );
			pStatic2->GetString(0)->msg = GET_STRING( STR_ID_724 );

			pButton->SetShowEnable(true, false);
		}

#if 0
		// 개발자모드에서만 채널상태 보이도록 한다.
		if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		{
			WCHAR staticBuff[64] = {0,};
			//wsprintf(staticBuff, L"%d/%d", channelInfo.m_iCurrentUser, channelInfo.m_iMaxUser);
			StringCchPrintf( buff, 64, L"%d/%d", channelInfo.m_iCurrentUser, channelInfo.m_iMaxUser);
			pStaticChannelNum->GetString(i)->msg = staticBuff;
		}
#endif
	}





	// 서버군 UI 갱신
	const WCHAR SOLRES_GAGE_CONTROL_NAME[] = L"Sever_Soless_Server_Gauge";	// 솔레스 서버군 static control 이름 


	//if( numChannel != 0 )
	//{
	//	serverLevel = serverLevel / numChannel;
	//}

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( SOLRES_GAGE_CONTROL_NAME );
	if ( pStatic != NULL )
	{
		for(int i=0; i<6; ++i)
		{
			pStatic->GetPicture(i)->SetShow(false);
		}

		float  fServerCongestionRate = (float)nCurrentNum / (float)nMaxNum;
		float fServerCongestionRateDisplay = 0.f;
		if(fServerCongestionRate > CHANNEL_CONGESTION_SCOPE[3])
		{
			fServerCongestionRateDisplay = 1.f;
		}
		else if(fServerCongestionRate > CHANNEL_CONGESTION_SCOPE[2])
		{
			fServerCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[3];
		}
		else if(fServerCongestionRate > CHANNEL_CONGESTION_SCOPE[1])
		{
			fServerCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[2];
		}
		else if(fServerCongestionRate > CHANNEL_CONGESTION_SCOPE[0])
		{
			fServerCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[1];
		}
		else
		{
			fServerCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[0];
		}
		 
		//if( fServerCongestionRate < CHANNEL_CONGESTION_CRITERION[0] )
		if(fServerCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[1])
		{
			pStatic->GetPicture(4)->SetSizeX(pStatic->GetPicture(4)->GetOriginalSize().x * fServerCongestionRateDisplay);
			pStatic->GetPicture(4)->SetShow( true );
			pStatic->GetPicture(5)->SetShow( true );
		}
		//else if( fServerCongestionRate < CHANNEL_CONGESTION_CRITERION[1] )
		else if(fServerCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[2])
		{
			pStatic->GetPicture(2)->SetSizeX(pStatic->GetPicture(2)->GetOriginalSize().x * fServerCongestionRateDisplay);
			pStatic->GetPicture(2)->SetShow( true );
			pStatic->GetPicture(3)->SetShow( true );
		}
		//else if( fServerCongestionRate < CHANNEL_CONGESTION_CRITERION[2] )
		else if(fServerCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[3])
		{
			pStatic->GetPicture(0)->SetSizeX(pStatic->GetPicture(0)->GetOriginalSize().x * fServerCongestionRateDisplay);
			pStatic->GetPicture(0)->SetShow( true );
			pStatic->GetPicture(1)->SetShow( true );
		}
		else
		{
			pStatic->GetPicture(0)->SetSizeX(pStatic->GetPicture(0)->GetOriginalSize().x * fServerCongestionRateDisplay);
			pStatic->GetPicture(0)->SetShow( true );
			pStatic->GetPicture(1)->SetShow( true );
		}

#if 0 
		// 개발자모드에서만 채널상태 보이도록 한다.
		if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		{
			CKTDGUIStatic* pStaticChannelNum = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"server_user_num" );

			WCHAR staticBuff[64] = {0,};
			//wsprintf(staticBuff, L"%d/%d", nCurrentNum, nMaxNum);
			StringCchPrintf( buff, 64, L"%d/%d", nCurrentNum, nMaxNum);
			pStaticChannelNum->GetString(0)->msg = staticBuff;
		}
#endif
#if 0 
		switch( serverLevel )
		{
		case KServerSetData::UCL_FREE:
			{
				pStatic->GetPicture(4)->SetSizeX(pStatic->GetPicture(4)->GetOriginalSize().x * 0.3f);
				pStatic->GetPicture(4)->SetShow( true );
				pStatic->GetPicture(5)->SetShow( true );
			} break;
		case KServerSetData::UCL_NORMAL:
			{
				pStatic->GetPicture(4)->SetSizeX(pStatic->GetPicture(4)->GetOriginalSize().x * 0.6f);
				pStatic->GetPicture(2)->SetShow( true );
				pStatic->GetPicture(3)->SetShow( true );
			}
		case KServerSetData::UCL_BUSY:
			{
				pStatic->GetPicture(4)->SetSizeX(pStatic->GetPicture(4)->GetOriginalSize().x * 0.9f);
				pStatic->GetPicture(0)->SetShow( true );
				pStatic->GetPicture(1)->SetShow( true );
			}
		case KServerSetData::UCL_FULL:
			{
				pStatic->GetPicture(0)->SetShow( true );
				pStatic->GetPicture(1)->SetShow( true );
			}
		}
#endif
	}


	CKTDGUIButton *pServerButton = (CKTDGUIButton*)m_pDLGServerSelectBack->GetControl( L"sever_select" );
	pServerButton->SetDownStateAtNormal(true);
}
#endif	ADD_SERVER_GROUP
// kimhc // 2009-12-15 // 이전에 플레이했던 채널 서버군


void CX2StateBeginning::UnitHandling( double fTime, float fElapsedTime )
{

	for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];

		if ( pUnitViewerUI == NULL )
			continue;

		pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );

		if ( pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
		{
			if ( m_pSelectUnit != NULL )
			{
				if ( pUnitViewerUI == GetUnitViewer( m_pSelectUnit ) )
					continue;
			}
			pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
		}


	}

}




void CX2StateBeginning::ClearUnitViewerList()
{
	for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];
		CX2UnitViewerUI::DeleteKTDGObject( pUnitViewerUI );
	}
	m_UnitViewerUIList.resize(0);
}

CX2UnitViewerUI* CX2StateBeginning::GetUnitViewer( CX2Unit* pUnit )
{
	for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];
		if ( pUnitViewerUI->GetUnit() == pUnit )
			return pUnitViewerUI;
	}

	return NULL;
}




void CX2StateBeginning::DownAnimFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bPlayDownAnim == true )
	{
		m_fDownAnimTime += fElapsedTime;

		for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
		{
			CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];
			if ( pUnitViewerUI != NULL && pUnitViewerUI->GetShowObject() == false )
			{
				if ( m_fDownAnimTime > (i * 0.4f) )
				{
					pUnitViewerUI->SetShowObject( true );
					pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING );
					pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
				}
			}
		}

		if ( m_fDownAnimTime >= ( (float)g_pData->GetMyUser()->GetUnitNum() * 0.5f + 1.0f ) )
		{
			m_bPlayDownAnim = false;
		}
	}
}

void CX2StateBeginning::PlayDownAnim()
{
	m_bPlayDownAnim = true;
	m_fDownAnimTime = 0.0f;
}


bool CX2StateBeginning::Handler_EGS_STATE_CHANGE_FIELD_REQ(bool bForceMove)
{
	KEGS_STATE_CHANGE_FIELD_REQ kPacket;
	D3DXVECTOR3 vStartPos(0, 0, 0);
	int startPos = 0;

	int iMapId = 0;
	if(g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL)
		iMapId = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_nMapID;

	if( g_pData->GetLocationManager()->GetVillageMapTemplet((SEnum::VILLAGE_MAP_ID)iMapId) == NULL || 
		bForceMove == true )
	{
		// 강제 루벤이동
		iMapId = SEnum::VMI_RUBEN;
		//vStartPos = D3DXVECTOR3(-16.f, 1445.f, -2873.f);
		startPos = 1;
	}	
	else
	{
		if( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP <= 0 )
		{
			// 초기진입으로 본다.

			// 강제 루벤이동
			iMapId = SEnum::VMI_RUBEN;
			startPos = 1;
		}
		else
		{
			startPos = g_pData->GetLocationManager()->GetLoginPos( iMapId );

			// 로그아웃했던 위치를 계산한다.
#ifdef REMEMBER_LOGOUT_POSITION_TEST
			const CX2World::WorldData* pWorldData = g_pData->GetWorldManager()->GetWorldData( (CX2World::WORLD_ID) g_pData->GetLocationManager()->GetWorldID( (SEnum::VILLAGE_MAP_ID) iMapId ) );
			if( NULL != pWorldData )
			{
				CX2World* pWorld = g_pData->GetWorldManager()->CreateWorld( pWorldData->worldID ); // fix!! 월드 생성하지 않고 라인맵 정보 가져올 수 있게 수정해야함
				if( NULL != pWorld )
				{
					pWorld->SetShowObject( false );
					CKTDGLineMap* pLineMap = pWorld->GetLineMap();
					if( NULL != pLineMap )
					{
						const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ucLastTouchLineIndex );
						if( NULL != pLineData )
						{
							float fLastPosValue = halfToFloat( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_usLastPosValue );
							if( fLastPosValue < 0.f )
								fLastPosValue = 0.f;
							if( fLastPosValue > 1.f )
								fLastPosValue = 1.f;

							vStartPos = pLineData->startPos * (1.f - fLastPosValue) + pLineData->endPos * fLastPosValue;
							vStartPos = pLineMap->GetLandPosition( vStartPos, LINE_RADIUS, (int*) NULL );

							startPos = -1;
						}
					}
					SAFE_DELETE( pWorld );
				}
			}
#endif REMEMBER_LOGOUT_POSITION_TEST
		}		
	}	


	if( startPos >= 0 )
	{
		vStartPos = g_pData->GetLocationManager()->GetStartPosLoc(startPos);	
	}

	g_pData->GetLocationManager()->SetLastPos(vStartPos);
	//g_pData->GetLocationManager()->SetLastPosLineIndex();


	kPacket.m_iMapID =  (int)iMapId;	


	g_pData->GetLocationManager()->SetVillage( (SEnum::VILLAGE_MAP_ID)kPacket.m_iMapID, vStartPos );

	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_FIELD_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_FIELD_ACK );
	

	return true;
}

bool CX2StateBeginning::Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_FIELD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_FIELD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef REFORM_ENTRY_POINT
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ), L"DLG_UI_Selection_MessageBox_No_Button.lua" );
#else //REFORM_ENTRY_POINT
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ) );			
#endif //REFORM_ENTRY_POINT

			bool bCanCreateWorld = true;
			CX2World::WORLD_ID	m_iWorldId;
			SEnum::VILLAGE_MAP_ID m_villageId;
			D3DXVECTOR3 m_vPos;	

			// 현재 설정된 village를 생성한다.	
			if(g_pData != NULL && g_pData->GetLocationManager() != NULL)
			{
				g_pData->GetLocationManager()->GetVillage(m_villageId, m_vPos);
				m_iWorldId = (CX2World::WORLD_ID)g_pData->GetLocationManager()->GetWorldID(m_villageId);		
			}
			else
			{
				// 마을을 생성할 수 없음.
				bCanCreateWorld = false;
			}

			if(m_iWorldId <= CX2World::WI_NONE || m_iWorldId >= CX2World::WI_END)
			{
				// 마을을 생성할 수 없음
				bCanCreateWorld = false;
			}

			// 마을 생성		
			if(bCanCreateWorld == true)
			{
//#if 0
//				g_pTFieldGame = new CX2TFieldGame(m_iWorldId);
//				
//				if(g_pTFieldGame != NULL)
//				{
//					g_pTFieldGame->AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
//					g_pTFieldGame->SetAddedMyUnit(true);					
//				}
//#endif

				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, NULL, false );
			}
		}
		else
		{			
			// 강제 루벤이동
			Handler_EGS_STATE_CHANGE_FIELD_REQ(true);

			// 이전위치로 복원
			//g_pData->GetLocationManager()->RestoreVillage();
		}

	}
	return true;
}

bool CX2StateBeginning::Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_USER_LIST_IN_SECTOR_NOT kEvent;

	DeSerialize( pBuff, &kEvent );		

	if(g_pTFieldGame != NULL)
	{
		for(int i=0; i<(int)kEvent.m_vecNewUserList.size(); ++i)
		{			
			g_pTFieldGame->JoinFieldUnit( &kEvent.m_vecNewUserList[i] );
		}			
	}

	return true;
}







void CX2StateBeginning::Handler_EGS_CREATE_TUTORIAL_ROOM_REQ()
{
	KEGS_CREATE_TUTORIAL_ROOM_REQ kEGS_CREATE_ROOM_REQ;

	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomType			= CX2Room::RT_DUNGEON;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomName			= L"tutorial";
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bPublic			= false;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_wstrPassword		= L"a*h97lk2";
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_MaxSlot			= 1;

	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_DifficultyLevel	= CX2Dungeon::DL_NORMAL;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime			= 19999.0f;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_cGetItemType		= CX2DungeonRoom::DGIT_RANDOM;


	switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
	{
	default:
	case CX2Unit::UT_ELSWORD:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_TUTORIAL_ELSWORD;
		} break;

	case CX2Unit::UT_ARME:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_TUTORIAL_ARME;
		} break;

	case CX2Unit::UT_LIRE:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_TUTORIAL_LIRE;
		} break;

	case CX2Unit::UT_RAVEN:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_TUTORIAL_RAVEN;
		} break;

	case CX2Unit::UT_EVE:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_TUTORIAL_EVE;		
		} break;
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_TUTORIAL_CHUNG;
		} break;
#endif	NEW_CHARACTER_CHUNG

	}

	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_TUTORIAL_ROOM_REQ, kEGS_CREATE_ROOM_REQ );
	g_pMain->AddServerPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, 40.f );	
}

bool CX2StateBeginning::Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_TUTORIAL_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	
	//CX2PacketLog::PrintLog( &kEvent );

	//SAFE_DELETE_DIALOG( m_pDLGCreateDungeonRoom );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_CREATE_TUTORIAL_ROOM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			int startPos = 0;
			int iMapId;			
			if(g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL)
				iMapId = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_nMapID;

			if(g_pData->GetLocationManager()->GetVillageMapTemplet((SEnum::VILLAGE_MAP_ID)iMapId) == NULL)
			{
				// 강제 루벤이동
				iMapId = SEnum::VMI_RUBEN;
				//vStartPos = D3DXVECTOR3(-16.f, 1445.f, -2873.f);
				startPos = 1;
			}	
			else
			{
				startPos = g_pData->GetLocationManager()->GetLoginPos( iMapId );		
			}	
	
			D3DXVECTOR3 vStartPos = g_pData->GetLocationManager()->GetStartPosLoc(startPos);
			g_pData->GetLocationManager()->SetLastPos(vStartPos);
			//g_pData->GetLocationManager()->SetLastPosLineIndex();

			g_pData->GetLocationManager()->SetVillage((SEnum::VILLAGE_MAP_ID)iMapId, vStartPos);

#ifdef HEAP_BROKEN_BY_ROOM
			CX2Room::InitializeRoomPacketData();
			CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
				vector<KRoomSlotInfo>(), kEvent.m_wstrCNIP );

			g_pData->DeleteAllRooms();
			g_pData->ResetDungeonRoom();
			if ( NULL != g_pX2Room && CX2Room::IsNewDataSet() )		/// 새로운 데이터가 셋팅 되었으면
				g_pX2Room->ApplyRoomPacketData();
#else	HEAP_BROKEN_BY_ROOM
			//방 데이터 제작
			CX2DungeonRoom* pCX2DungeonRoom = g_pData->ResetDungeonRoom();
			pCX2DungeonRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
			pCX2DungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );
			pCX2DungeonRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
			pCX2DungeonRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
           if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_DUNGEON ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif // HEAP_BROKEN_BY_ROOM



			for ( int i = 0; i < 3; i++ )
			{
				pCX2DungeonRoom->SetRecommendLevel( i, D3DXVECTOR2(0, 0) );
			}			

			//방으로 스테이트 이동
			if(g_pInstanceData != NULL)
				g_pInstanceData->SetFirstJoinVillage(true);

			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_645 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_ROOM, NULL, false );	
			

			return true;
		}
	}

	if( true == g_pMain->GetPlayFieldDungeon() )
		g_pMain->SetPlayFieldDungeon( false );

	return false;
}








//////////////////////////////////////////////////////////////////////////
// statelogin에 있던 코드 옮겨온 것
bool CX2StateBeginning::Handler_EGS_CONNECT_REQ( const wstring& wstrGameServerIP, const int iGameServerPort )
{

	if( true == wstrGameServerIP.empty() )
		return false;

	if( true == m_bSentEGS_CONNECT_REQ )
		return false;




	// game server에 접속하기전에 nexon passport를 갱신받아서 접속한다.
	if( false == g_pMain->GetManualLogin() )
	{
		switch( g_pMain->GetPublisher() )
		{
#ifdef _NEXON_KR_
		case CX2Main::XP_NEXON_KOREA:
			{
				WCHAR passPortStr[PASSPORT_SIZE] = { 0, };
				CNMCOClientObject::GetInstance().GetNexonPassport( passPortStr );
				const WCHAR* pLoginID = CNMCOClientObject::GetInstance().GetMyLoginId();

				if( 0 == wcslen( passPortStr ) && 
					0 != g_pNexonPassport.length() )
				{
					g_pInstanceData->SetUserPassword( g_pNexonPassport.c_str() );	// 체험 아이디인 경우
				}
				else
				{
					g_pInstanceData->SetUserPassword( passPortStr );		// 일반 넥슨 계정인 경우
				}

				g_pInstanceData->SetUserID( pLoginID );

				//{{ kimhc // 2010-06-24 // OTP 작업
			#ifdef	SERV_OTP_AUTH
				// 유저의 하드웨어 ID를 획득
				WCHAR lpNxHWID[NXHWID_SIZE] = { 0, };
				CNMCOClientObject::GetInstance().GetNxHWID( lpNxHWID );
				g_pInstanceData->SetNxHWID( lpNxHWID );
			#endif	SERV_OTP_AUTH
				//}} kimhc // 2010-06-24 // OTP 작업

			} break;
#endif _NEXON_KR_
#ifdef CLIENT_COUNTRY_TWHK
		case CX2Main::XP_GAMANIA_TW:
		case CX2Main::XP_GAMANIA_HK:
			{
				//대만 접속 : ID/Pass는 이미 인스턴스 데이터에 세트되어 있다.
				//g_pInstanceData->SetUserID( g_pMain->GetArgvID() );
				//g_pInstanceData->SetUserPassword( g_pMain->GetArgvPass() );
			} break;
#endif CLIENT_COUNTRY_TWHK
		}
	}





	if( LoginErrorCheck() == true )
	{
		//접속시도
		//g_pData->ResetServerProtocol();


		if( true == g_pData->GetServerProtocol()->ConnectedToGameServer( wstrGameServerIP.c_str(), iGameServerPort ) )
		{
			g_pMain->SetConnectedGameServerIP( wstrGameServerIP );
			g_pMain->SetConnectedGameServerPort( iGameServerPort );

			KEGS_CONNECT_REQ kPacket;
			kPacket.m_wstrVersion = g_pMain->GetClientVersion();

			g_pData->GetServerProtocol()->SendPacket( EGS_CONNECT_REQ, kPacket );
			g_pMain->AddServerPacket( EGS_CONNECT_ACK, 60.0f );

			m_bSentEGS_CONNECT_REQ = true;

			SAFE_DELETE_DIALOG( m_pDLGMsgBox );

#ifdef REFORM_ENTRY_POINT
			m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_725 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT
			m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_725 ), this );
#endif	// REFORM_ENTRY_POINT

			g_pInstanceData->SetConnectChannelServerID( m_iTryConnectChannelID );
			m_iTryConnectChannelID = -1;

	
			// 현재 접속접속한 game_server ip를 저장한다. (마을진입시 TRServer 접속용)
			if(g_pMain != NULL)
			{
				g_pMain->SetTRServerIp( wstrGameServerIP );
			}

			// 채널 서버와의 접속을 끊는다.
			Handler_ECH_DISCONNECT_REQ();
		}
		else
		{
#ifdef _SERVICE_
			m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_726 ), this );
#else
			m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_726 ), this );
#endif _SERVICE_

			//m_bFailToConnectServer = true;
		}		
	}
	else
	{
		//g_pData->DeleteServerProtocol();
		//g_pData->DeleteMyUser();		
		g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_INVALID_ID_OR_PASSWORD );
		g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
	}
	return true;
}

bool CX2StateBeginning::Handler_EGS_CONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CONNECT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	
	if( g_pMain->DeleteServerPacket( EGS_CONNECT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
//#ifdef _SERVICE_
//			OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck3" );
//#endif
			g_pMain->SetServerUDPPort( kEvent.m_UDPPort );

			Handler_EGS_VERIFY_ACCOUNT_REQ();
		}
		else
		{
			//g_pData->DeleteServerProtocol();
			//g_pData->DeleteMyUser();
			switch( kEvent.m_iOK )
			{
			default:
				{
					g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				} break;

			case NetError::ERR_CONNECT_00:
				{
					MessageBox( g_pKTDXApp->GetHWND(), GET_STRING( STR_ID_727 ), L"Quit", MB_OK );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				} break;

			case NetError::ERR_CONNECT_04:
				{
					m_bSentEGS_CONNECT_REQ = false;
					m_bConnectFullChannel = true;
					//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"접속인원이 많아서 접속 할 수 없습니다.", this );
				} break;
			}
		}
	}
	return true;
}

bool CX2StateBeginning::Handler_EGS_VERIFY_ACCOUNT_REQ()
{
	KEGS_VERIFY_ACCOUNT_REQ kPacket;

	kPacket.m_wstrUserID = g_pInstanceData->GetUserID();
	kPacket.m_wstrPasswd = g_pInstanceData->GetUserPassword();
	
	//{{ kimhc // 2010-06-24 // OTP 작업
#ifdef	SERV_OTP_AUTH
	kPacket.m_wstrHWID = g_pInstanceData->GetNxHWID();
#endif	SERV_OTP_AUTH
	//}} kimhc // 2010-06-24 // OTP 작업

#ifdef ARGUMENT_LOGIN

#ifdef _SERVICE_MANUAL_LOGIN_
	kPacket.m_bDebugAuth = g_pMain->GetManualLogin();		
#else _SERVICE_MANUAL_LOGIN_
#if defined( _IN_HOUSE_ ) || defined( _IN_HOUSE_SERVICE_READY_QA_ )
	kPacket.m_bDebugAuth = true;						// 사내는 true
#else 
	kPacket.m_bDebugAuth = false;						// 자동/수동 로그인을 모두 유저가 사용하므로 서비스 버전의 경우 무조건 false
#endif // _IN_HOUSE_ || _IN_HOUSE_SERVICE_READY_QA_
	
#endif _SERVICE_MANUAL_LOGIN_

#else ARGUMENT_LOGIN
	kPacket.m_bDebugAuth = g_pMain->GetManualLogin();
#endif ARGUMENT_LOGIN

	m_bEGS_VERIFY_ACCOUNT_REQ = true;

	g_pData->GetServerProtocol()->SendPacket( EGS_VERIFY_ACCOUNT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_VERIFY_ACCOUNT_ACK, 60 );

	return true;	
}

bool CX2StateBeginning::Handler_EGS_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_VERIFY_ACCOUNT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_VERIFY_ACCOUNT_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
//#ifdef _SERVICE_
//			OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck4" );
//#endif


#ifdef _SERVICE_
#ifdef _NEXON_KR_
			//OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck5" );
			OpenGlobalURL( L"http://elsword.nexon.com/wiselog/gameinstall.aspx?section=check5" );
#endif
#endif
			//////////////////////////////////////////////////////////////////////////
			//{{ 2009. 7. 25  최육사	체크메시지
#ifdef HENIR_TEST
			if( kEvent.m_kAccountInfo.m_iAuthLevel == CX2User::XUAL_DEV )
			{
				//MessageBox( g_pKTDXApp->GetHWND(), L"헤니르 시공 테스트 모드입니다! 현재 버전은 본섭패치하면 안되는 테스트 버전입니다!", L"알림!", MB_OK );
			}
#endif HENIR_TEST
			//}}
			//////////////////////////////////////////////////////////////////////////

			wstringstream wstrstm;
			wstrstm << GET_REPLACED_STRING( ( STR_ID_728, "L", kEvent.m_kAccountInfo.m_wstrID ) );
			g_pMain->SetMailNameToFindHack( wstrstm.str().c_str() );
#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
            {
                std::string strID;
                ConvertWCHARToChar( strID, kEvent.m_kAccountInfo.m_wstrID );
                g_pMain->SetUserIdToFindHack_ThreadSafe( strID );
            }
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
			g_pMain->SetUserIdToFindHack( kEvent.m_kAccountInfo.m_wstrID.c_str() );
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX

#ifdef SERV_CLIENT_PUBLIC_IP
			g_pMain->SetPublicIp( kEvent.m_wstrClientIP );
#endif

			if(kEvent.m_cHackingUserType == CX2User::HUT_AGREE_HACK_USER)
				g_pMain->SetHackingUser(true);

			m_bEGS_VERIFY_ACCOUNT_ACK = true;
            {
			    CX2User::UserData kUserData;
			    kUserData = kEvent;
			    g_pData->ResetMyUser( kUserData );
            }

			//m_MaxUnitNum	= g_pData->GetMyUser()->GetUserData().maxUnitCount;
			//const int SLOT_COUNT_FOR_CREATE_UNIT = 1;
			//m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+SLOT_COUNT_FOR_CREATE_UNIT+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;



#ifndef DISABLE_DISAGREE_HACK_USER
			if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_DISAGREE_HACK_USER )
			{
				m_pDLGCheckHack = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
					GET_STRING( STR_ID_729 ), 
					SSSUCM_HACK_USER_PROCESS_CHECK_OK, this, SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL );
			}
//#if 0	// 핵 의심유저라면 접속시 무조건 메일을 날리므로 제거
			else if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
			{
#ifndef PROCESSLIST
				g_pMain->UpdateProcessList();
#endif
				g_pMain->SendHackMail();
			}
#endif DISABLE_DISAGREE_HACK_USER




			g_pMain->GetGameOption().SetPlayGuide( kEvent.m_kAccountInfo.m_kAccountOption.m_bPlayGuide, false );

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
			if( false == pUserData->m_bIsGuestUser ) // 체험 아이디 제한
			{
				if( false == g_pMain->GetNexonLoginMessenger() )
				{


					if ( g_pMain->GetNexonLogin() == false )				// 사내접속이면, 즉, ID, password 직접 입력하고 로그인한 경우
					{
						if( false == g_pMain->GetNexonLoginMessenger() )
						{
							NMLoginAuthReplyCode resultAuth = CNMCOClientObject::GetInstance().LoginAuth( g_pInstanceData->GetUserID().c_str(), g_pInstanceData->GetUserPassword().c_str() );
							
							switch( resultAuth )
							{
							case kLoginAuth_OK:
								{
									UINT32 supportFlag = 0;

									supportFlag |= kSupportFlag_ChatSession;
									supportFlag |= kSupportFlag_AddFriendConfirm;
									supportFlag |= kSupportFlag_RequestSession;
									supportFlag |= kSupportFlag_ServerMessage;
									if ( CNMCOClientObject::GetInstance().LoginMessenger( kSupportFlag_All  ) == FALSE )
									{
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_730 ), this );
									}
									else
									{
										g_pMain->SetNexonLoginMessenger( true );
									}
								}
								break;


							case kLoginAuth_WrongID:
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_731 ), g_pMain->GetNowState() );
								}
								break;

							case kLoginAuth_WrongPassword:
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_732 ), g_pMain->GetNowState() );
								}
								break;

							case kLoginAuth_ServiceShutdown:
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_733 ), g_pMain->GetNowState() );
								}
								break;

							default:
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_734 ), g_pMain->GetNowState() );
								}
								break;
							}
						}
					}
					else // 서비스, 오픈테섭 버전 접속이면 
					{
						UINT32 supportFlag = 0;

						supportFlag |= kSupportFlag_ChatSession;
						supportFlag |= kSupportFlag_AddFriendConfirm;
						supportFlag |= kSupportFlag_RequestSession;
						supportFlag |= kSupportFlag_ServerMessage;
						if ( CNMCOClientObject::GetInstance().LoginMessenger( kSupportFlag_All  ) == FALSE )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_730 ), this );
						}
						else
						{
							g_pMain->SetNexonLoginMessenger( true );
						}
					}
				}
			}
#endif // #ifndef NEW_MESSENGER
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP




#ifndef NO_GAMEGUARD
			if ( CheckNPHackUser() == false )
			{
				CloseNPGameMon();
			}

#ifdef ALWAYS_INTERNAL_NPGE_PACKING
			if( KNP_Init( g_pKTDXApp->GetHInstance(), g_pMain->GetClinetArg() ) == false )
#else //ALWAYS_INTERNAL_NPGE_PACKING
			if( KNP_Init( g_pKTDXApp->GetHInstance() ) == false )
#endif ALWAYS_INTERNAL_NPGE_PACKING
			{
				g_pKTDXApp->SetFindHacking( true );
			}

			KNP_SetHwnd( g_pKTDXApp->GetHWND() );

			//접속성공을 하면 nProtect에 아이디를 등록시킨다.
			KNP_Send( g_pInstanceData->GetUserID().c_str() );
			//게임가드 완료
#endif // NO_GAMEGUARD

		}
		else
		{
			StateLog( L"요거" );

			//g_pData->DeleteServerProtocol();
			//g_pData->DeleteMyUser();
			if ( kEvent.m_iOK == NetError::ERR_VERIFY_00 ||
				kEvent.m_iOK == NetError::ERR_VERIFY_01 )
			{
				g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_DOUBLE_CONNECT );
			}
			else
				g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_VERIFY_ACCOUNT );

			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
		}
	}

	return true;
}

bool CX2StateBeginning::Handler_ENX_USER_LOGIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KENX_USER_LOGIN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bENX_USER_LOGIN_NOT = true;
	m_bIsGameBang = kEvent.m_bIsGameBang;

	return true;
}

bool CX2StateBeginning::Handler_KXPT_PORT_CHECK_REQ()
{
	m_bPortCheckWait			= true;
	m_fPortCheckWaitTime		= 0.5f;

	KXPT_PORT_CHECK_REQ kXPT_PORT_CHECK_REQ;
	kXPT_PORT_CHECK_REQ.m_UserUID = g_pData->GetMyUser()->GetUID();
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    kXPT_PORT_CHECK_REQ.m_InternalIPAddress = g_pData->GetGameUDP()->GetMyIPAddress();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	kXPT_PORT_CHECK_REQ.m_wstrInternalIP = g_pData->GetGameUDP()->GetMyIP();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	kXPT_PORT_CHECK_REQ.m_usInternalPort = g_pData->GetGameUDP()->GetMyPort();
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    return g_pData->GetGameUDP()->SendToIP( CKTDNUDP::ConvertIPToAddress( g_pMain->GetConnectedGameServerIP() ), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, 
        &kXPT_PORT_CHECK_REQ, sizeof(kXPT_PORT_CHECK_REQ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	KSerBuffer buff;
//	Serialize( &buff, &kXPT_PORT_CHECK_REQ );
//	return g_pData->GetGameUDP()->Send( g_pMain->GetConnectedGameServerIP(), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, (char*)buff.GetData(), buff.GetLength() );
//	//return g_pData->GetGameUDP()->Send( g_pMain->GetServerIP(), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, (char*)buff.GetData(), buff.GetLength() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

bool CX2StateBeginning::Handler_KXPT_PORT_CHECK_ACK( const KXPT_PORT_CHECK_ACK& kXPT_PORT_CHECK_ACK )
{
	dbg::clog << L"MY IP : " 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        << CKTDNUDP::ConvertAddressToIP( kXPT_PORT_CHECK_ACK.m_IPAddress )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        << kXPT_PORT_CHECK_ACK.m_IP.c_str() 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        << dbg::endl;
	dbg::clog << L"MY Port : " << g_pMain->GetGameP2PPort() << dbg::endl;
	dbg::clog << L"MY Ext Port : " << kXPT_PORT_CHECK_ACK.m_Port << dbg::endl;

	m_bPortCheckWait			= false;
	m_fPortCheckWaitTime		= 0.0f;
	m_PortCheckRetryTime		= 0;

	g_pData->GetGameUDP()->SetMyExtPort( kXPT_PORT_CHECK_ACK.m_Port );

	Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
	return true;
}

bool CX2StateBeginning::Handler_EGS_CURRENT_TIME_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_CURRENT_TIME_REQ );
	g_pMain->AddServerPacket( EGS_CURRENT_TIME_ACK );

	return true;
}

bool CX2StateBeginning::Handler_EGS_CURRENT_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CURRENT_TIME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CURRENT_TIME_ACK ) == true )
	{
		g_pData->SetServerCurrentTime( kEvent.m_wstrCurrentTime );
		//StateChangeLoading();

		Handler_EGS_SELECT_SERVER_SET_REQ();

		return true;
	}

	return false;
}


bool CX2StateBeginning::LoginErrorCheck()
{

	//if( m_UserID.size() < 3 )
	//{
	//	//ID를 4자 미만으로 입력한 경우
	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"아이디를 4자 이상 넣으세요.", this );
	//	return false;
	//}
	//if( m_UserPassword.size() < 4 )
	//{
	//	//Password를 4자 미만으로 입력한 경우
	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"패스워드를 4자 이상 넣으세요.", this );
	//	return false;
	//}


	return true;
}



bool CX2StateBeginning::Handler_ECH_GET_CHANNEL_LIST_REQ()
{
	g_pData->GetServerProtocol()->SendChID( ECH_GET_CHANNEL_LIST_REQ );
	g_pMain->AddServerPacket( ECH_GET_CHANNEL_LIST_ACK, 100.0f );

	return true;
}


bool CX2StateBeginning::Handler_ECH_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KECH_GET_CHANNEL_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( ECH_GET_CHANNEL_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing;
		}
	}
	return true;
}

bool CX2StateBeginning::Handler_ECH_GET_CHANNEL_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KECH_GET_CHANNEL_LIST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bReceivedChannelList = true;

#ifdef CHANGE_CHANNEL
	g_pInstanceData->m_vecChannelInfo.resize(0);
#else
	m_vecChannelInfo.resize( 0 );
#endif

	std::map< int, KChannelInfo >::const_iterator cit;
	for( cit = kEvent.m_mapChannelList.begin() ; cit != kEvent.m_mapChannelList.end(); cit++ )
	{
		const KChannelInfo& channelInfo = cit->second;

#ifdef CHANGE_CHANNEL
		if(g_pInstanceData != NULL)
			g_pInstanceData->m_vecChannelInfo.push_back( channelInfo );
#else
		m_vecChannelInfo.push_back( channelInfo );
#endif
	}

	//std::sort( m_vecChannelInfo.begin(), m_vecChannelInfo.end(), LessChannelIndex() );

	//{{ kimhc // 2009-12-15 // 서버군 추가 작업
#ifdef	ADD_SERVER_GROUP
	g_pInstanceData->GetMiniMapUI()->SetChannelComboBox();
	m_bReservedConnectionToChannelServer = false;
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // 서버군 추가 작업

	// 채널 리스트 UI를 Update한다
	UpdateChannelListUI();

	return true;
}


bool CX2StateBeginning::Handler_ECH_DISCONNECT_REQ()
{

	if( 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
            g_pData->GetServerProtocol() != NULL
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
             g_pData->GetServerProtocol()->GetChUserProxy() != NULL 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->ChUserProxy_SetIntendedDisconnect( true );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetChUserProxy()->SetIntendedDisconnect( true );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}


	g_pData->GetServerProtocol()->SendChID( ECH_DISCONNECT_REQ );
	g_pMain->AddServerPacket( ECH_DISCONNECT_ACK, 100.0f );


	return true;
}


bool CX2StateBeginning::Handler_ECH_DISCONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pMain->DeleteServerPacket( ECH_DISCONNECT_ACK ) == true )
	{
		g_pData->GetServerProtocol()->DisconnectFromChannelServer();
	}

	if( true == m_bReservedConnectionToChannelServer )
	{
		ConnectToChannelServer();
	}

	return true;
}


bool CX2StateBeginning::Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ()
{

	if( 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol() != NULL
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() != NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->UserProxy_SetIntendedDisconnect( true );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy()->SetIntendedDisconnect( true );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}

	m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK = true;
	
	g_pData->GetServerProtocol()->SendID( EGS_DISCONNECT_FOR_SERVER_SELECT_REQ );
	g_pMain->AddServerPacket( EGS_DISCONNECT_FOR_SERVER_SELECT_ACK, 100.0f );


	return true;
}


bool CX2StateBeginning::Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pMain->DeleteServerPacket( EGS_DISCONNECT_FOR_SERVER_SELECT_ACK ) == true )
	{
		m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK = false;

		g_pData->GetServerProtocol()->DisconnectFromGameServer();


		g_pMain->SetConnectedGameServerIP( L"" );
		g_pMain->SetConnectedGameServerPort( 0 );


	}
	return true;
}







bool CX2StateBeginning::Handler_EGS_GET_MY_MESSENGER_SN_REQ( UidType unitUID )
{
	KEGS_GET_MY_MESSENGER_SN_REQ kPacket;
	kPacket = unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_MY_MESSENGER_SN_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_MY_MESSENGER_SN_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2558 ), this );

	return true;
}

bool CX2StateBeginning::Handler_EGS_GET_MY_MESSENGER_SN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_GET_MY_MESSENGER_SN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_GET_MY_MESSENGER_SN_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( true == m_bRemoveUnitAfterReceiveNMSerialNum )
			{
				m_bRemoveUnitAfterReceiveNMSerialNum = false;

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
				NMVirtualKey nmVirtualKey;
				nmVirtualKey.uGameCode = NEXON_KOREA_ELSWORD_GAMECODE;
				nmVirtualKey.uVirtualIDCode = kEvent.m_uiKNMSerialNum;

				if( true == g_pMain->GetNexonLoginMessenger() )
				{
					if( 0 == kEvent.m_uiKNMSerialNum )	// nmserial number가 없는 경우에는 바로 지운다~
					{
						// 2009.05.13.hoons. 바로삭제 조치가 변경되어 팝업으로 메세지만 전달
						//Handler_EGS_DELETE_UNIT_REQ();
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2559 ), this );
						// hoons
					}
					else
					{
						if( CNMCOClientObject::GetInstance().RemoveMyVirtualUser( nmVirtualKey ) == TRUE )
						{
							Handler_EGS_DELETE_UNIT_REQ();
						}
						else
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2560 ), this );
						}
					}
				}
				else
				{
					Handler_EGS_DELETE_UNIT_REQ();
				}
#endif // #ifndef NEW_MESSENGER
#else OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifdef NEW_MESSENGER
				Handler_EGS_DELETE_UNIT_REQ();
#endif
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP

			}
		}
		else
		{
			if( true == m_bRemoveUnitAfterReceiveNMSerialNum )
			{
				m_bRemoveUnitAfterReceiveNMSerialNum = false;
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_2561 ), this );
			}
		}
		return true;
	}

	return false;
}



bool CX2StateBeginning::ConnectToChannelServer()
{
	ASSERT( NULL != g_pData->GetServerProtocol() );

	if( g_pData->GetServerProtocol()->ConnectedToChannelServer( g_pMain->GetPickedChannelServerIP().c_str(), g_pMain->GetChannelServerPort() ) == true )
	{
		//{{ 09.08. 태완 : 서버-클라 접속시 패킷 변경.
//#ifdef SERV_KOG_OTP_VERIFY
		return Handler_ECH_VERIFY_ACCOUNT_REQ();
// #else
// 		return Handler_ECH_GET_CHANNEL_LIST_REQ();
// #endif
		//}}
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_735 ), this );	

		g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );									// fix!! id 변경
		g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
	}	

	return false;
}

bool CX2StateBeginning::Handler_ECH_VERIFY_ACCOUNT_REQ()
{
	KECH_VERIFY_ACCOUNT_REQ kPacket;
	kPacket.m_wstrID = g_pInstanceData->GetUserID();
	kPacket.m_wstrPassword = g_pInstanceData->GetUserPassword();
#ifdef SERV_KOG_OTP_VERIFY
	kPacket.m_bIDVerified = !(g_pInstanceData->IsIDVerified());
#else SERV_KOG_OTP_VERIFY
    kPacket.m_bIDVerified = true;
#endif SERV_KOG_OTP_VERIFY
	g_pData->GetServerProtocol()->SendChPacket( ECH_VERIFY_ACCOUNT_REQ, kPacket );
	g_pMain->AddServerPacket( ECH_VERIFY_ACCOUNT_ACK );
	
	return true;
}

bool CX2StateBeginning::Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KECH_VERIFY_ACCOUNT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( ECH_VERIFY_ACCOUNT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
#ifdef SERV_KOG_OTP_VERIFY
			// 최초 Gash 인증 후로는 서버에서 발급받은 Pass를 계속 이용하게 되므로(게임서버 접속도), 아예 변경해 버린다.
			g_pInstanceData->SetIDVerified(true);
			g_pInstanceData->SetUserPassword( kEvent.m_wstrPassport );	
#endif SERV_KOG_OTP_VERIFY
			return Handler_ECH_GET_CHANNEL_LIST_REQ();
		}
		else
		{			
#ifdef SERV_KOG_OTP_VERIFY
			switch( kEvent.m_iOK )
			{
			case NetError::ERR_VERIFY_20:
				{
					g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				} break;
			default:
				{

				} break;
			}
#endif SERV_KOG_OTP_VERIFY
		}
	}

	return false;
}




/*virtual*/ bool CX2StateBeginning::LastMsgByESC()
{
#ifdef ELSWORD_NEW_BEGINNING
	if( m_eNowState == BG_STATE_UNIT_CREATE )
	{
		OpenLastMsgPopUp( GET_STRING( STR_ID_736 ) );
	}
	else
#endif ELSWORD_NEW_BEGINNING
	if( true == CX2StateBeginning::m_sbSelectedServerSet )
	{
		OpenLastMsgPopUp( GET_STRING( STR_ID_736 ) );
	}
	else
	{
		return QuitGame();
	}

	return true;
}

/*virtual*/ void CX2StateBeginning::LastMsgPopOk()
{
	if ( m_pDLGLastMsgPopUp != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLastMsgPopUp, NULL, false );
	m_pDLGLastMsgPopUp = NULL;

#ifdef ELSWORD_NEW_BEGINNING
	if( m_eNowState == BG_STATE_UNIT_CREATE )
	{
		m_eNowState = BG_STATE_UNIT_SELECT;
		g_pBeginningGame->SetEnableCreateUnit(false);
		SetShowUnitButton( true );
		ReInitUnitSelect();
		g_pBeginningGame->PlayWorldCamera(2);
	}
	else
	{
		g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, SSSUCM_UNIT_SELECT_EXIT, NULL, false );
	}
#else
	g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, SSSUCM_UNIT_SELECT_EXIT, NULL, false );
#endif ELSWORD_NEW_BEGINNING
}

void CX2StateBeginning::UnitSelectExit()
{
	ClearBeforeUnit( m_pSelectUnit );
	ClearUnitButton();
	m_pDLGServerSelectBack->SetShowEnable(true, true);
	m_pDLGUnitSelectFront->SetShowEnable(false, false);

	//{{ kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
#ifdef	ADD_SERVER_GROUP
	m_pDLGChannelSelectBack->SetShowEnable( true, true );
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리

	m_pSelectUnit = NULL;
	m_sbSelectedServerSet = false;

	m_bENX_USER_LOGIN_NOT		= false;
	m_bEGS_VERIFY_ACCOUNT_REQ	= false;
	m_bEGS_VERIFY_ACCOUNT_ACK	= false;
	m_bPortCheckReq				= false;

	m_bSentEGS_CONNECT_REQ 		= false;
	m_bReceivedChannelList 		= false;

	if( NULL != g_pData->GetServerProtocol() )
	{
		g_pData->GetServerProtocol()->SendTRID( ETR_UNIT_DISCONNECT );
	}
	// 게임서버와의 접속을 끊는다
	Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();

	// 채널서버에 접속한다.
	ConnectToChannelServer();  

	g_pKTDXApp->SkipFrame();
}

//{{ kimhc // 2009-12-15 // 이전에 플레이 했던 서버군 읽기
#ifdef	ADD_SERVER_GROUP
//bool CX2StateBeginning::OpenScriptServerGroupFile()
//{
//	string			strFileName;
//	SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
//	bool			bParsingOK		= false;
//
//	ConvertWCHARToChar( strFileName, g_pData->GetSavedServerGroupFileName() );
//	ConvertFileAnsiToUTF8( strFileName, strFileName );
//
//	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//
//	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
//	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( g_pData->GetSavedServerGroupFileName() );
//	if( Info != NULL )
//	{
//		if( true == g_pKTDXApp->LoadLuaTinker( g_pData->GetSavedServerGroupFileName().c_str(), false ) )
//		{
//			if( true == g_pKTDXApp->LoadAndDoMemory( &luaManager, g_pData->GetSavedServerGroupFileName().c_str(), false ) )
//			{
//				LUA_GET_VALUE_ENUM( luaManager, "SERVER_GROUP", 			eServerGroupID,			SERVER_GROUP_ID,		SGI_INVALID	);
//			}
//		}
//	}
//#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
//
//	switch ( eServerGroupID )
//	{
//	case SGI_SOLES:
//	case SGI_GAIA:
//		{
//			g_pInstanceData->SetServerGroupID( eServerGroupID );
//			bParsingOK = true;
//		}
//		break;
//
//	default:
//		{
//#ifdef RANDOM_SERVER
//			if( g_pMain->GetDefaultChannelServerIPIndex() == SGI_INVALID )
//			{
//				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( (rand() % 2) ) );
//			}
//			else
//			{
//				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
//			}
//#else
//			g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
//#endif
//		}
//		break;
//
//	}
//
//	return bParsingOK;
//
//}

void CX2StateBeginning::UpdateServerSelectButton( SERVER_GROUP_ID eID )
{
	CKTDGUIButton *pServerButtonSoles		= static_cast< CKTDGUIButton* >( m_pDLGServerSelectBack->GetControl( L"sever_select_01" ) ); // 솔레스
	CKTDGUIButton *pServerButtonGaia		= static_cast< CKTDGUIButton* >( m_pDLGServerSelectBack->GetControl( L"sever_select_02" ) );	// 신서버
	

	if ( pServerButtonSoles		== NULL ||
		 pServerButtonGaia	== NULL )
		 return;

	// 모두 다 false로 초기화
	pServerButtonSoles->SetDownStateAtNormal( false );		// 솔레스
	pServerButtonGaia->SetDownStateAtNormal( false );		// 신서버

	switch ( eID )
	{
	case SGI_GAIA:
		{
			 
			//pServerButtonGaia->SetDownStateAtNormal(true); // oasis907 : 김상윤 [2010.11.15] //
			m_iChannelNum	= pServerButtonGaia->GetDummyInt( 1 );		// 채널 갯수
			//g_pMain->m_iPickedChannelServerIPIndex = 1; // 임시
		}
		break;

	case SGI_SOLES:
	default:
		{
			//pServerButtonSoles->SetDownStateAtNormal(true); 
			m_iChannelNum	= pServerButtonSoles->GetDummyInt( 1 );		// 채널 갯수
			//g_pMain->m_iPickedChannelServerIPIndex = 0; // 임시
		}
		break;		
	}

	g_pMain->SetPickedChannelServerIPIndex( static_cast< int >( eID ) );
	
}
#endif	ADD_SERVER_GROUP
//}}  kimhc // 2009-12-15 // 이전에 플레이 했던 서버군 읽기



#ifdef ELSWORD_NEW_BEGINNING
void CX2StateBeginning::CreateGame()
{
	if(g_pBeginningGame == NULL)
	{
		g_pBeginningGame = new CX2BeginningGame();
		g_pBeginningGame->LoadWorld(CX2World::WI_RUBEN_EL_TREE_STAGE1);
		g_pBeginningGame->PlayWorldCamera(1);

		//g_pBeginningGame->GetWorld()->SetWorldColor( 0xffffffff );
		//g_pBeginningGame->GetWorld()->FadeWorldColor( g_pBeginningGame->GetWorld()->GetOriginColor(), 1.0f );

		//g_pData->GetEventScene()->BlackLayOutMove( false, 1.0f, false, true, 3.0f, 1.0f );

	}
	return;
}

bool CX2StateBeginning::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if( NULL != g_pBeginningGame )
	{
		if( true == g_pBeginningGame->MsgProc(hWnd, uMsg, wParam, lParam))
			return true;
	}

	// 개발자 모드일 경우
	return false;
}


void CX2StateBeginning::SetShowEnableServerSelectControl(bool bEnable)
{
	CKTDGUIButton *pServerButtonSoles		= static_cast< CKTDGUIButton* >( m_pDLGServerSelectBack->GetControl( L"sever_select_01" ) ); // 솔레스
	CKTDGUIButton *pServerButtonGaia		= static_cast< CKTDGUIButton* >( m_pDLGServerSelectBack->GetControl( L"sever_select_02" ) );	// 신서버

	CKTDGUIStatic *pServerStaticSoles		= static_cast< CKTDGUIStatic* >( m_pDLGServerSelectBack->GetControl( L"server_01" ) ); // 솔레스
	CKTDGUIStatic *pServerStaticGaia		= static_cast< CKTDGUIStatic* >( m_pDLGServerSelectBack->GetControl( L"server_02" ) );	// 신서버

	CKTDGUIStatic *pServerStaticNameSoles		= static_cast< CKTDGUIStatic* >( m_pDLGServerSelectBack->GetControl( L"server_name_01" ) ); // 솔레스
	CKTDGUIStatic *pServerStaticNameGaia		= static_cast< CKTDGUIStatic* >( m_pDLGServerSelectBack->GetControl( L"server_name_02" ) );	// 신서버


	//CKTDGUIStatic *pServerStaticBG		= static_cast< CKTDGUIStatic* >( m_pDLGServerSelectBack->GetControl( L"server_static_BG" ) );

	pServerButtonSoles->SetShowEnable(bEnable, bEnable);
	pServerButtonGaia->SetShowEnable(bEnable, bEnable);

	pServerStaticSoles->SetShowEnable(bEnable, bEnable);
	pServerStaticGaia->SetShowEnable(bEnable, bEnable);

	pServerStaticNameSoles->SetShowEnable(bEnable, bEnable);
	pServerStaticNameGaia->SetShowEnable(bEnable, bEnable);

	//pServerStaticBG->SetShow(bEnable);
	return;
}

void CX2StateBeginning::ReInitUnitSelect()
{
	//g_pBeginningGame->RenderLoadingFace(4);

/*
	if( NULL != g_pData->GetMyUser() )
	{
		m_MaxUnitNum	= g_pData->GetMyUser()->GetUserData().maxUnitCount;

		m_NowPage		= 1;
		const int SLOT_COUNT_FOR_CREATE_UNIT = 1;

		if( g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
		{
			m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+SLOT_COUNT_FOR_CREATE_UNIT+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
		}
		else
		{
			m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
		}
	}


	ClearBeforeUnit( m_pSelectUnit );
	ClearUnitButton();
	CreateUnitButton();
*/
	return;
}


#endif ELSWORD_NEW_BEGINNING


#endif ELSWORD_NEW_BEGINNING