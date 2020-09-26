#include "StdAfx.h"
#include ".\x2community.h"

CX2Community::CX2Community(void) : 
m_pDlgGuild( NULL )
//{{ kimhc // 2009-09-17 // 초기화
#ifdef	GUILD_MANAGEMENT
, m_eNowGuildViewInfo(CX2Community::GVI_LOCATION )
, m_uiNowGuildPage( 1 )
, m_uiMaxGuildPage( 1 )
, m_uiNowNumberOfGuildMembersPlayingGame( 0 )
, m_uiAllNumberOfGuildMembers( 0 )
#endif	GUILD_MANAGEMENT
//}} kimhc // 2009-09-17 // 초기화

//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
, m_pDlgReferee( NULL ) // 추천인 관련 다이얼로그
, m_uiNowRefereePage( 1 )
, m_uiMaxRefereePage( 1 )
#endif	VIEW_REFEREE_LIST
//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM
, m_pDlgRelation ( NULL )
, m_pDlgRelationOnlyNotCouple ( NULL )
, m_pDlgRelationOnlyPartnerShowing ( NULL )
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FIX_INVITE_PVP_PLAYER // 김태환
, m_fInviteCoolTime( 0.f )
#endif // FIX_INVITE_PVP_PLAYER
#ifdef SERV_RECRUIT_EVENT_BASE
, m_pDlgRecruit( NULL )
#endif SERV_RECRUIT_EVENT_BASE
{
	m_pStage = g_pMain->GetNowState();


	m_pDlgBack			= NULL;
	m_pDlgField			= NULL;
	m_pDlgFriend		= NULL;
	//m_pDlgGuild			= NULL;
	m_pDlgDisciple		= NULL;
	m_pDLGCreateGroup	= NULL;
	m_pDLGGroupMove		= NULL;
	m_pDLGAddFriend		= NULL;
	m_pDLGFriendReqWait	= NULL;
	m_pDLGSearchFriend	= NULL;
	m_pDLGSearchFriendResult	= NULL;
	m_pDLGSearchFriendResultCharIcon = NULL;

	m_pDLGUserMenu		= NULL;	
	m_pDLGChangeGroupName = NULL;

	m_pDLGQuery			= NULL;

	for(int i=0; i<8; ++i)
		m_pFieldUserIcon[i] = NULL;
	
	m_pMessengerUserList	= new CX2Community::MessengerUserList();

	m_pFriendReqMsg			= NULL;
	m_pOpendChatSession		= NULL;

	m_bRequestChat			= false;

	m_CurrentTrackPos		= 0;

	m_pTalkBoxManager		= NULL;

	m_MessageSenderNickName = L"";
	m_bReceiveMessage		= false;


	m_NowPageNum			= 1;
	m_MaxChatSessionByPage	= 4;

	m_ChatSessionOffsetPos	= D3DXVECTOR2( 0, 0 );
	m_ChatSessionHeight		= 0;

	m_pSoundAlarm			= g_pKTDXApp->GetDeviceManager()->OpenSound( L"MessengerAlarm.ogg" );

	m_iPickedUserUID		= 0;
	m_iPickedUserIndex		= 0;

	m_iMaxFieldPage			= 1;
	m_iNowFieldPage			= 1;

	m_bSortGroup			= false;
	m_bUpdateFieldUser		= false;
	m_bOpenGroupMenu		= false;

	m_fFieldUserTimer		= 0.f;
	m_bHideWindow			= false;	
	
	m_bRefresh = false;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	m_vecUnitViewerUI.clear();
	m_pRelationUnit = NULL;
#endif // ADDED_RELATIONSHIP_SYSTEM
	InitUI();

	SetOpen( false );	
	m_MessengerUserTab = XMUT_FIELD;

	m_eNowLayer = XDL_NORMAL_3;
	m_bSearchUnitUnitInfoValid = false;

	m_iChangeChannelId = -1;

	m_SelectedGuildMemberUID = 0;
#ifndef NEW_MESSENGER
	m_RequestedNMFriendKey = NMFriendKey();
#endif  NEW_MESSENGER

#ifdef FIX_INVITE_PVP_PLAYER // 김태환
	m_vecInviteRoomUID.clear();
#endif // FIX_INVITE_PVP_PLAYER
}

CX2Community::~CX2Community(void)
{

	ClearChatSessionList();

	ClearUserList();

	SAFE_DELETE( m_pFriendReqMsg );

	SAFE_DELETE( m_pMessengerUserList );

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );


	SAFE_DELETE_DIALOG( m_pDlgBack );
	SAFE_DELETE_DIALOG( m_pDlgField );
	SAFE_DELETE_DIALOG( m_pDlgFriend );
	SAFE_DELETE_DIALOG( m_pDlgGuild );
	SAFE_DELETE_DIALOG( m_pDlgDisciple );

	//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	SAFE_DELETE_DIALOG( m_pDlgReferee );
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM
	BOOST_FOREACH ( CX2UnitViewerUI * pUnitViewerUI, m_vecUnitViewerUI )
		SAFE_DELETE_KTDGOBJECT ( pUnitViewerUI );
	SAFE_DELETE_DIALOG ( m_pDlgRelation );
	SAFE_DELETE_DIALOG ( m_pDlgRelationOnlyPartnerShowing );
	SAFE_DELETE_DIALOG ( m_pDlgRelationOnlyNotCouple );
	m_vecUnitViewerUI.clear();
	SAFE_DELETE ( m_pRelationUnit );
#endif // ADDED_RELATIONSHIP_SYSTEM

	SAFE_DELETE_DIALOG( m_pDLGCreateGroup );
	SAFE_DELETE_DIALOG( m_pDLGGroupMove );
	SAFE_DELETE_DIALOG( m_pDLGAddFriend );
	SAFE_DELETE_DIALOG( m_pDLGSearchFriend );
	SAFE_DELETE_DIALOG( m_pDLGSearchFriendResult );
	SAFE_DELETE_DIALOG( m_pDLGSearchFriendResultCharIcon );
	SAFE_DELETE_DIALOG( m_pDLGFriendReqWait );
	SAFE_DELETE_DIALOG( m_pDLGUserMenu );
	SAFE_DELETE_DIALOG( m_pDLGChangeGroupName );

	for(int i=0; i<8; ++i)
		SAFE_DELETE_DIALOG( m_pFieldUserIcon[i] )

	SAFE_CLOSE( m_pSoundAlarm );

	g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
	m_pDLGQuery = NULL;

	//{{ kimhc // 2009-09-30 // 길드원들의 UID와 등급을 가진 map 제거
#ifdef	GUILD_MANAGEMENT
	m_vecGuildMemberInfo.clear();
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-09-30 // 길드원들의 UID와 등급을 가진 map 제거

	//{{ kimhc // 2009-01-12 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	m_vecRecommendUserInfo.clear();
#endif	VIEW_REFEREE_LIST
	//}} kimhc // 2009-01-12 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef FIX_INVITE_PVP_PLAYER // 김태환
	m_vecInviteRoomUID.clear();
#endif // FIX_INVITE_PVP_PLAYER

#ifdef	SERV_RECRUIT_EVENT_BASE
	SAFE_DELETE_DIALOG( m_pDlgRecruit );
	m_vecRecruitUnitInfo.clear();
	m_vecRecruiterUnitInfo.clear();
#endif	SERV_RECRUIT_EVENT_BASE
}

bool CX2Community::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bFlag = false;
	
	switch( uMsg )
	{
		// 마우스 메시지들에 대해서만. 마우스 메시지가 아니면 bFlag이 true가 되지 않게.. (처리된 메시지가 아니니까)
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			bool bOverDlg = false;

			if(true == m_pDlgBack->GetIsOverControl())
			{
				bOverDlg = true;
			}

			for(UINT i=0; i<m_vecChatSession.size(); ++i)
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];

				if( pChatSession->m_pDialog != NULL && 
					pChatSession->m_bHideWindow == false &&
					pChatSession->m_bShowWindow == true && 
					pChatSession->m_pDialog->GetIsOverControl() == true )
					bOverDlg = true;
			}

			if(bOverDlg == false)
				return false;
		}
		

		bFlag = true;
		break;
	default:
		break;
	}
	

	
#if 1 
	if ( m_pTalkBoxManager != NULL && 
		m_pTalkBoxManager->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{
			UidType unitUID = 0;

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			if ( m_pTalkBoxManager != NULL && m_pTalkBoxManager->CheckMousePointInTalkbox( mousePos, unitUID ) == true )
			{
				m_pTalkBoxManager->Clear();

			}
		}
		break;
	}
#endif

	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgBack ) )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		{
			//POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			//D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			// 대화창 관련 처리
			for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];

				if( pChatSession->m_pDialog != NULL && 
					pChatSession->m_bHideWindow == false &&
					pChatSession->m_bShowWindow == true && 
					pChatSession->m_pDialog->GetIsOverControl() == true )
				{						
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pChatSession->m_pDialog, true );
					m_vecChatSession.erase( m_vecChatSession.begin() + i);
					m_vecChatSession.push_back(pChatSession);

					//if(pChatSession->m_bMaxWindow == true)
					pChatSession->m_bAlarmReceiveMessage = false;
					return true;
				}
			}

			if ( false == m_pDlgBack->GetIsOverControl() )
			{
				return false;
			}
			else
			{
				g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_COMMUNITY, true);
				bFlag = true;
			}			
		}
		break;
	case WM_MOUSEMOVE:
		{
			//POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			//D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			// 대화창 관련 처리
			for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];

				if( pChatSession->m_pDialog != NULL && pChatSession->m_pDialog->GetIsMouseOver() == true )
				{	
					pChatSession->m_pDialog->SetIsOverControl(true);
					return true;
				}
			}
		}
		break;
	}

	return bFlag;
}

bool CX2Community::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case XCCUM_CLOSE:
		SetOpen(false);
		break;
	case XCCUM_TAB_FIELD:
		SetTab(XMUT_FIELD);		
		break;
	case XCCUM_TAB_FRIEND:
		SetTab(XMUT_FRIEND);
#ifdef NEW_MESSENGER
		ResetUserListUI();
#else
		ResetUserList();
#endif		
		break;
	case XCCUM_TAB_GUILD:
		SetTab(XMUT_GUILD);
		break;
	case XCCUM_TAB_DISCIPLE:
		SetTab(XMUT_DISCIPLE);
		UpdateUserList(1);
		break;
		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	case XCCUM_TAB_REFEREE:
		{
			SetTab( XMUT_REFEREE );
		}
		break;

	case XCCUM_REFEREE_PREV_PAGE:
		{
			if ( m_uiNowRefereePage > 1 )
			{
				--m_uiNowRefereePage;

				return Handler_EGS_GET_RECOMMEND_USER_LIST_REQ( m_uiNowRefereePage );
			}
		}
		break;

	case XCCUM_REFEREE_NEXT_PAGE:
		{
			if ( m_uiNowRefereePage < m_uiMaxRefereePage )
			{
				++m_uiNowRefereePage;

				return Handler_EGS_GET_RECOMMEND_USER_LIST_REQ( m_uiNowRefereePage );
			}
		}
		break;

#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

	case XCCUM_MOUSE_L_UP:
		{
			if( g_pData != NULL && g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetUIInventory() != NULL &&
				( g_pData->GetUIManager()->GetUIInventory()->GetOverMouseInven() == true ||
#ifdef SERV_EPIC_QUEST
				( g_pData->GetUIManager()->GetUIQuestNew() != NULL && g_pData->GetUIManager()->GetUIQuestNew()->GetOverMouseQuest() == true )) )
#else
				( g_pData->GetUIManager()->GetUIQuest() != NULL && g_pData->GetUIManager()->GetUIQuest()->GetOverMouseQuest() == true ))	)
#endif SERV_EPIC_QUEST
				break;
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			CheckButton( pButton );			
		}
		break;
	case XCCUM_MOUSE_R_UP:
		{
			if( g_pData != NULL && g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetUIInventory() != NULL &&
				( g_pData->GetUIManager()->GetUIInventory()->GetOverMouseInven() == true ||
#ifdef SERV_EPIC_QUEST
				( g_pData->GetUIManager()->GetUIQuestNew() != NULL && g_pData->GetUIManager()->GetUIQuestNew()->GetOverMouseQuest() == true )) )
#else
				( g_pData->GetUIManager()->GetUIQuest() != NULL && g_pData->GetUIManager()->GetUIQuest()->GetOverMouseQuest() == true ))	)
#endif SERV_EPIC_QUEST
				break;

			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;	
			CheckButton( pButton );

			
			//{{ 허상형 : [2009/9/21] //	자기 자신일 경우 메뉴를 띄우지 않기위해 조건식 추가
			if( m_vecUserUID[ pButton->GetDummyInt(0) - 1 ] != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
				OpenUserMenu( pButton->GetDummyInt(0) );
			}
			
			//}} 허상형 : [2009/9/21] //	자기 자신일 경우 메뉴를 띄우지 않기위해 조건식 추가
		}		
		break;
	case XCCUM_USERMENU_CLOSE:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);
		}
		break;

	case XCCUM_USERMENU_DELETE:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;

			if(m_MessengerUserTab == XMUT_FRIEND)
			{
				DeleteFriendTab();
				return true;
			}

			if(m_MessengerUserTab == XMUT_DISCIPLE)
			{
				if( NULL != g_pMain->GetTutorSystem() )
				{
					const CX2TutorSystem::TutorUnit* pTutorUnit = g_pMain->GetTutorSystem()->GetTutorUnit( m_iPickedUserUID );
					if( NULL == pTutorUnit )
						return true;

					m_pDLGQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
						GET_REPLACED_STRING( ( STR_ID_63, "L", pTutorUnit->m_wstrNickName ) ), XCCUM_BAN_TUTOR_UNIT, (CKTDXStage*)m_pStage );
				}
				return true;
			}
			
		} break;

	case XCCUM_USERMENU_CHARINFO:
		{		
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

//#ifdef SERV_INTEGRATION //2013.4.11 다른 서버군도 프로필 정보가 출력되도록 주석 처리
//			SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
//			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
//			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
//			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
//			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5129 ), g_pMain->GetNowState() );
//				return true;
//			}
//			//}}
//#endif SERV_INTEGRATION

#ifdef SERV_LOCAL_RANKING_SYSTEM
			CX2State* pState = static_cast<CX2State*>( m_pStage );
			if( NULL != pState )
				pState->Handler_EGS_LOCAL_RANKING_WATCH_UNIT_REQ( m_iPickedUserUID );
#else
			CX2State* pState = (CX2State*)m_pStage;
			if ( pState != NULL )
				pState->Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( ConvertOrgName( GetSelectedUnitName().c_str() ).c_str() );
#endif //SERV_LOCAL_RANKING_SYSTEM
		}
		break;

	case XCCUM_BAN_TUTOR_UNIT:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
			m_pDLGQuery = NULL;

			if( NULL != g_pMain->GetTutorSystem() )
			{
				g_pMain->GetTutorSystem()->Handler_EGS_DEL_TUTORIAL_REQ( m_iPickedUserUID );
			}

			UpdateUserList(1);
			return true;

		} break;

	case XCCUM_USER_LISTBOX_ITEM_LBUTTON:
		{
			return SelectUserListItem( CKTDGUIListBox::LBEM_SELECTION );
		}
		break;
	case XCCUM_USER_LISTBOX_ITEM_LBUTTON_DBLCLK:
		{
			return SelectUserListItem( CKTDGUIListBox::LBEM_ITEM_DBLCLK );
		}
		break;

	case XCCUM_USER_LISTBOX_ITEM_RBUTTON:
		{
			return SelectUserListItem( CKTDGUIListBox::LBEM_SELECTION_BY_RBUTTON );
		}
		break;

	case XCCUM_FRIEND_GROUP_SORT:
		{
			m_setOpenedGroupName.clear();
			m_bSortGroup = true;
			ResetUserListUI();
		}
		break;
	case XCCUM_FRIEND_STATUS_SORT:
		{
			m_setOpenedGroupName.clear();
			m_setOpenedGroupName.insert( GET_STRING( STR_ID_64 ) );
			m_bSortGroup = false;
			ResetUserListUI();
		}
		break;
	case XCCUM_FRIEND:		
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
#ifdef EXTEND_SERVER_GROUP_MASK
			int iServerGroupID = -1;
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
			iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
			if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
			SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
#endif // EXTEND_SERVER_GROUP_MASK
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5126 ), g_pMain->GetNowState() );
				return true;
			}
			//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

			if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;

			// 체험 아이디 제한 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}
			if( m_iPickedUserUID > 0 && m_iPickedUserUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
#ifdef NEW_MESSENGER
				wstring addFriendMessage = GET_STRING( STR_ID_74 );
				if ( m_pDLGAddFriend != NULL )
				{
					CKTDGUIIMEEditBox* pIMEEditMessage = (CKTDGUIIMEEditBox*)m_pDLGAddFriend->GetControl( L"IMEMessage" );
					if ( pIMEEditMessage != NULL )
						addFriendMessage = pIMEEditMessage->GetText();
				}
				Handler_EGS_REQUEST_FRIEND_REQ( GetSelectedUnitName(), addFriendMessage );
#else
				Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( GetSelectedUnitName().c_str() );				
#endif
			}
			return true;		

		} 
		break;
	case XCCUM_PARTY:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

#ifdef EXTEND_SERVER_GROUP_MASK
			int iServerGroupID  = -1;
			iServerGroupID = g_pMain->ExtractServerGroupID( m_iPickedUserUID );
#else EXTEND_SERVER_GROUP_MASK
			SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
#endif EXTEND_SERVER_GROUP_MASK
			
			if( NULL != g_pData->GetPartyManager() )
			{
#ifdef EXTEND_SERVER_GROUP_MASK
				if( g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ(  ConvertOrgName( GetSelectedUnitName().c_str() ).c_str(), iServerGroupID ) == true )
#else EXTEND_SERVER_GROUP_MASK
				if( g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ(  ConvertOrgName( GetSelectedUnitName().c_str() ).c_str(), eServerGroupID ) == true )
#endif EXTEND_SERVER_GROUP_MASK
				{
					CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
					//{{ kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
					userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
					userData.iOpponentUID	= m_iPickedUserUID;
					//}} kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
					g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_LEADER_INVITE_PARTY,
						CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 7.f, GET_REPLACED_STRING( ( STR_ID_43, "L", GetSelectedUnitName() ) ), 
						(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
				}
			}
			return true;
		}
		break;
	case XCCUM_PARTY_DEMAND:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			if( m_iPickedUserIndex == 0 || m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;

			if(g_pData != NULL && g_pData->GetPartyManager() != NULL && g_pTFieldGame != NULL)
			{
				UidType iPartyUid = m_vecFieldUser[m_iPickedUserIndex-1].GetPartyUID(); //g_pTFieldGame->GetUserPartyUid(m_iPickedUserUID);
				if(iPartyUid > 0)
				{
					ASSERT( !L"EGS_JOIN_PARTY_REQ" );
					
					CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
					g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_REQUESTING_JOIN_PARTY, 
						CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 7.f, GET_STRING( STR_ID_42 ), 
						(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
				}
			}	
			return true;
		}
	case XCCUM_DISCIPLE:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
#ifdef EXTEND_SERVER_GROUP_MASK
			int iServerGroupID = -1;
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
			iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
			if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
			SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
#endif EXTEND_SERVER_GROUP_MASK
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5128 ), g_pMain->GetNowState() );
				return true;
			}
			//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

			if( m_iPickedUserUID == 0 || m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;

			// 체험 아이디 제한 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}

			bool bLevelCheckOK = true;
			int iLevel = GetSelectedUnitLevel();

			if(iLevel >= 10)
			{
				bLevelCheckOK = false;
			}

			if( false == bLevelCheckOK )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_41 ), g_pMain->GetNowState() );
				return true;
			}


			if( NULL != g_pMain->GetTutorSystem() )
			{
				g_pMain->GetTutorSystem()->SetTutorInviteeName( GetSelectedUnitName() );
				g_pMain->GetTutorSystem()->Handler_EGS_REQUEST_TUTORIAL_REQ( m_iPickedUserUID );
			}
			return true;
		}
		break;

	// 친구추가 관련
	case XCCUM_ADD_FRIEND_REQ_CONFIRM:
		{
			return ProcessFriendReqMsg( true );
		}
		break;

	case XCCUM_ADD_FRIEND_REQ_NOT_CONFIRM:
		{
			return ProcessFriendReqMsg( false );
		}
		break;

	case XCCUM_ADD_FRIEND_OPEN:
		{
			return OpenAddFriendWindow( true );
		}
		break;
	//{{ 09.04.20 태완
	case XCCUM_SEARCH_FRIEND_OPEN:
		{
			return OpenSearchFriendWindow( true );
		} break;
	case XCCUM_SEARCH_FRIEND_CLOSE:
		{
			return OpenSearchFriendWindow( false );
		} break;
	case XCCUM_SEARCH_FRIEND_OK:
		{
			return SearchFriendReq();
		} break;
	case XCCUM_SEARCH_FRIEND_RESULT_CLOSE:
		{
			return CloseSearchFriendResultWindow();
		} break;
	case XCCUM_SEARCH_FRIEND_RESULT_ADD_FRIEND:
		{
			if(m_bSearchUnitUnitInfoValid)
			{
#ifdef NEW_MESSENGER
				wstring addFriendMessage = GET_STRING( STR_ID_74 );
				if ( m_pDLGAddFriend != NULL )
				{
					CKTDGUIIMEEditBox* pIMEEditMessage = (CKTDGUIIMEEditBox*)m_pDLGAddFriend->GetControl( L"IMEMessage" );
					if ( pIMEEditMessage != NULL )
						addFriendMessage = pIMEEditMessage->GetText();
				}
				Handler_EGS_REQUEST_FRIEND_REQ( m_SearchUnitUnitInfo.m_wstrCharName, addFriendMessage );
#else
				m_pDLGFriendReqWait = g_pMain->KTDGUIMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_69 ), m_pStage );
				return Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( m_SearchUnitUnitInfo.m_wstrCharName.c_str() );
#endif
			}	
			return false;

		} break;
	case XCCUM_SEARCH_FRIEND_RESULT_PARTY_INVITE:
		{
			if(m_bSearchUnitUnitInfoValid)
			{
				m_iPickedUserUID = m_SearchUnitUnitInfo.m_iUnitUID;

				if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
					return true;

				if( g_pData->GetPartyManager() != NULL && g_pData->GetPartyManager()->DoIHaveParty() == true && m_iPickedUserUID > 0 )
				{
#ifdef EXTEND_SERVER_GROUP_MASK
					int iServerGroupID = -1;
					iServerGroupID = g_pMain->ExtractServerGroupID( m_iPickedUserUID );
#else EXTEND_SERVER_GROUP_MASK
					SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
					eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
#endif EXTEND_SERVER_GROUP_MASK

#ifdef SERV_PVP_NEW_SYSTEM
#ifdef EXTEND_SERVER_GROUP_MASK
					if( g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ( m_iPickedUserUID, m_SearchUnitUnitInfo.m_wstrCharName, iServerGroupID ) == true )
#else EXTEND_SERVER_GROUP_MASK
					if( g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ( m_iPickedUserUID, m_SearchUnitUnitInfo.m_wstrCharName, eServerGroupID) == true )
#endif EXTEND_SERVER_GROUP_MASK
#else
					g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ( m_iPickedUserUID );
#endif
					{
						CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
						//{{ kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
						userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
						userData.iOpponentUID	= m_iPickedUserUID;
						//}} kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
						g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_LEADER_INVITE_PARTY,
							CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 7.f, GET_REPLACED_STRING( ( STR_ID_43, "L", m_SearchUnitUnitInfo.m_wstrCharName ) ), 
							(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
					}
				}
				return true;

				
			}		
			return false;

		} break;
	case XCCUM_SEARCH_FRIEND_RESULT_PARTY_REQUEST:
		{
			if(m_bSearchUnitUnitInfoValid)
			{
				if(g_pData != NULL && g_pData->GetPartyManager() != NULL && g_pTFieldGame != NULL)
				{
					UidType iPartyUid = m_SearchUnitUnitInfo.m_iPartyUID;
					if(iPartyUid > 0)
					{
						ASSERT( !L"EGS_JOIN_PARTY_REQ" );

						CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
						g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_REQUESTING_JOIN_PARTY, 
							CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 7.f, GET_STRING( STR_ID_42 ), 
							(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
					}
				}	
				return true;
			}			
			return false;

		} break;
	
	case XCCUM_SEARCH_FRIEND_RESULT_PVP_INVITE:
		{
			if(g_pMain->GetNowStateID() != CX2Main::XS_PVP_ROOM)
				return false;
			if(m_bSearchUnitUnitInfoValid)
			{
				// 같은 방에 있으면 초대 안 되게
				if(g_pData->GetPVPRoom() != NULL && -1 != g_pData->GetPVPRoom()->GetIndexByUnitUID(m_SearchUnitUnitInfo.m_iUnitUID))
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2538 ), g_pMain->GetNowState() );
					return true;
				}

				// 다시 한번 확인해서 초대 메시지를 보내자
				switch(m_SearchUnitUnitInfo.m_cState)
				{
				case CX2Unit::CUS_PVP_LOBBY:
				case CX2Unit::CUS_PVP_WAIT:
					{
						m_iPickedUserUID = m_SearchUnitUnitInfo.m_iUnitUID;

						if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
							return true;

						return Handler_EGS_INVITE_PVP_ROOM_REQ( m_SearchUnitUnitInfo.m_iUnitUID );
					} break;
				default:
					break;
				}			
			}
			return false;
		} break;
	case XCCUM_SEARCH_FRIEND_RESULT_PVP_PLAYTOGETHER:
		{
			// 이미 초대를 수락했거나 해서 스테이트를 이동중이면 다른 초대에 대한 수락 메세지는 처리하지 않는다
			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
			{
				return true;
			}
			
			if(m_bSearchUnitUnitInfoValid)
			{
				switch(g_pMain->GetNowStateID())
				{
				case CX2Main::XS_PVP_LOBBY:
				case CX2Main::XS_PVP_ROOM:
					{
						// 같은 방에 있으면 안 따라가게
						if(g_pData->GetPVPRoom() != NULL && -1 != g_pData->GetPVPRoom()->GetIndexByUnitUID(m_SearchUnitUnitInfo.m_iUnitUID))
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2538 ), g_pMain->GetNowState() );
							return true;
						}
									
						// 해당 유저의 방으로 이동하자
						CX2StateAutoChanger::TARGET_DETAIL targetDetail;
						targetDetail.m_iChannelID = (int)m_SearchUnitUnitInfo.m_iRoomListID;
						targetDetail.m_iRoomUID = m_SearchUnitUnitInfo.m_iRoomUID;

						g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_PVP_ROOM, targetDetail );	
						return true;
					} break;
				default:
					break;
				}
			}
			return false;

		} break;
	case XCCUM_PVP_INVITE_ACCEPT:
		{
			// 이미 초대를 수락했거나 해서 스테이트를 이동중이면 다른 초대에 대한 수락 메세지는 처리하지 않는다
			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
			{
				return true;
			}

			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( NULL == pButton )
				return true;

			CX2Main::TimedMessagePopUp* pTimedPopup = g_pMain->GetTimedMessagePopup( pButton->GetDialog() );

			if( NULL != pTimedPopup )
			{
				pTimedPopup->m_fTimeLeft = 0.f;

				switch(g_pMain->GetNowStateID())
				{
				case CX2Main::XS_PVP_LOBBY:
				case CX2Main::XS_PVP_ROOM:
					{
						CX2StateAutoChanger::TARGET_DETAIL targetDetail;
						targetDetail.m_iChannelID = pTimedPopup->m_UserData.iTargetPVPChannel;
						targetDetail.m_iRoomUID = pTimedPopup->m_UserData.iOpponentUID;				
#ifdef SERV_NEW_PVPROOM_PROCESS
						targetDetail.m_bInvitation = true;
#endif SERV_NEW_PVPROOM_PROCESS

						//g_pMain->ClearTimedMessagePopup();
						g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_PVP_ROOM, targetDetail );	
						return true;
					} break;
				default:
					break;
				}
			}
		} break;
	case XCCUM_PVP_INVITE_REJECT:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( NULL == pButton )
				return true;

			CX2Main::TimedMessagePopUp* pTimedPopup = g_pMain->GetTimedMessagePopup( pButton->GetDialog() );

			if( NULL != pTimedPopup )
			{
				pTimedPopup->m_fTimeLeft = 0.f;

#ifdef FIX_INVITE_PVP_PLAYER // 김태환
				/// 초대 룸 리스트에서 해당 룸 아이디를 제거
				vector<UidType>::iterator it;

				for( it = m_vecInviteRoomUID.begin(); it != m_vecInviteRoomUID.end(); ++it )
				{
					if ( pTimedPopup->m_UserData.iOpponentUID == *it )
						m_vecInviteRoomUID.erase( it );

					break;
				}
#endif // FIX_INVITE_PVP_PLAYER
			}
			return true;

		} break;
	//}}
	case XCCUM_ADD_FRIEND_CANCEL:
		{
			return OpenAddFriendWindow( false );
		}
		break;

	case XCCUM_ADD_FRIEND_REQ:
		{
			return FriendReq();
		}
		break;

	// 그룹관련
	case XCCUM_CREATE_GROUP_OPEN:
		{
			return OpenCreateGroupWindow( true );
		}
		break;

	case XCCUM_CREATE_GROUP_CLOSE:
		{
			return OpenCreateGroupWindow( false );
		}
		break;

	case XCCUM_CREATE_GROUP_REQ:
		{
			CreateGroup();
			return OpenCreateGroupWindow( false );
		}
		break;
	case XCCUM_MGR_GROUP_MOVE_OPEN:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);
			return OpenGroupMoveWindow( true );
		}
		break;
	case XCCUM_MGR_GROUP_MOVE_OK:
		{
			MoveFriendGroup();
			return OpenGroupMoveWindow( false );
		}
		break;

	case XCCUM_MGR_GROUP_MOVE_EXIT:
		{
			return OpenGroupMoveWindow( false );
		}
		break;

	// 전체탭 새로고침
	case XCCUM_UPDATE_FIELD_USER:		
		{
			m_iPickedUserIndex = 0;
			m_iPickedUserUID = 0;

			CheckButton(NULL);

			m_fFieldUserTimer = 0.f;
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_SQUARE_GAME:
				{
					UpdateUserList(1);
					return true;
				} break;
			default:
				break;
			}

			return Handler_EGS_COMMUNITY_USER_LIST_REQ(1);
		}
		break;		
	// 전체탭 페이지 이동
	case XCCUM_FIELD_LIST_PREV:
		{
			m_iPickedUserIndex = 0;
			m_iPickedUserUID = 0;

			m_fFieldUserTimer = 0.f;
			CheckButton(NULL);

			if( m_iNowFieldPage <= 1 )
				m_iNowFieldPage = 1000;
			else if( m_iNowFieldPage <= m_iMaxFieldPage )
				--m_iNowFieldPage;

			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_SQUARE_GAME:
				{
					UpdateUserList(m_iNowFieldPage);
					return true;
				} break;
			default:
				break;
			}
			
			Handler_EGS_COMMUNITY_USER_LIST_REQ(m_iNowFieldPage);
		}
		break;
	case XCCUM_FIELD_LIST_NEXT:
		{
			m_iPickedUserIndex = 0;
			m_iPickedUserUID = 0;

			m_fFieldUserTimer = 0.f;
			CheckButton(NULL);

			if( m_iNowFieldPage >= m_iMaxFieldPage )
				m_iNowFieldPage = 1;
			else if( m_iNowFieldPage >= 1 )
				++m_iNowFieldPage;

			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_SQUARE_GAME:
				{
					UpdateUserList(m_iNowFieldPage);
					return true;
				} break;
			default:
				break;
			}

			Handler_EGS_COMMUNITY_USER_LIST_REQ(m_iNowFieldPage);
		}
		break;
	case XCCUM_USERMENU_BLOCKING:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			CutOnOffFriend();
			return true;
		}
		break;
	// 그룹이름바꾸기
	case XCCUM_MGR_CHANGE_GROUP_NAME_OPEN:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);
			return OpenGroupNameChangeWindow( true );
		}
		break;
	case XCCUM_MGR_CHANGE_GROUP_NAME_EXIT:
		{
			return OpenGroupNameChangeWindow( false );
		}
		break;

	case XCCUM_MGR_CHANGE_GROUP_NAME_OK:
		{
			ChangeGroupName();
			return OpenGroupNameChangeWindow( false );
		}
		break;
	// 귓속말
	case XCCUM_USERMENU_WHISPER:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
#ifdef EXTEND_SERVER_GROUP_MASK
			int iServerGroupID = -1;
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
			iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
			if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
			SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
#endif // EXTEND_SERVER_GROUP_MASK
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5130 ), g_pMain->GetNowState() );
				return true;
			}
			//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

			if(g_pChatBox != NULL)
			{
				g_pChatBox->SetPickedUser(m_iPickedUserUID, GetSelectedUnitName());
				return g_pChatBox->OnOpenWhisperChatMode();
			}			
		}
		break;
	// 친구 대화하기
	case XCCUM_MGR_CHAT_ENTER:
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)lParam;

			for(UINT i=0; i<m_vecChatSession.size(); ++i)
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
				if ( pChatSession != NULL )
				{
					CKTDGUIIMEEditBox *pChatBox = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
					if(pIMEEditBox == pChatBox)
					{
						m_pOpendChatSession = pChatSession;
						SendChatMessage( pIMEEditBox->GetText() );
						pIMEEditBox->ClearText();
						return true;
					}
				}
			}
		}
		break;
	// 채팅박스 닫기
	case XCCUM_DIALOG_CHATBOX_CLOSE:
		{
#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
			//CKTDGUIDialogType pChatBox( CKTDGUIDialog::MakeValidHandle( lParam ) );
			// added by wonpok. 20090820.
			CKTDGUIDialog *pDialog = (CKTDGUIDialog*)lParam;
			if( pDialog == NULL )
				return false;
			CKTDGUIDialogType pChatBox( CKTDGUIDialog::MakeValidHandle( pDialog->GetUID().GetHandlePart(), pDialog->GetUID().GetMagicPart() ) );			
#else
			CKTDGUIDialogType pChatBox = (CKTDGUIDialogType)lParam;	// note!! control의 custom event message가 아닌 dialog의 custom event message이므로 dialog를 인자로 받는다
#endif // DIALOG_HANDLE_TEST // 2009-8-19
			SetOpenChatSession( pChatBox, false );
			return true;
		}
		break;
	case XCCUM_CHATBOX_CLOSE:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			SetOpenChatSession( pButton->GetDialog(), false );
			return true;
		}
		break;
	// 채팅창 최대/최소화
	case XCCUM_CHATBOX_MAX:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;

			for(UINT i=0; i<m_vecChatSession.size(); ++i)
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
				if ( pChatSession != NULL )
				{
					CKTDGUIButton *pChatBox = (CKTDGUIButton*)pChatSession->m_pDialog->GetControl(L"Button_Maximum");
					if(pButton == pChatBox)
					{						
						pChatSession->m_bMaxWindow = true;
						SetChatBoxShow(pChatSession, true);

						// 포커스 이동
						CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
						if ( pIMEEdit != NULL && pChatSession->m_bMaxWindow == true)
						{
							pIMEEdit->RequestFocus();
						}

						return true;
					}
				}
			}
		}
		break;
	case XCCUM_CHATBOX_MINI:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;

			for(UINT i=0; i<m_vecChatSession.size(); ++i)
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
				if ( pChatSession != NULL )
				{
					CKTDGUIButton *pChatBox = (CKTDGUIButton*)pChatSession->m_pDialog->GetControl(L"Button_Mini");
					if(pButton == pChatBox)
					{						
						pChatSession->m_bMaxWindow = false;
						SetChatBoxShow(pChatSession, true);
						return true;
					}
				}
			}
		}
		break;	
	case XCCUM_CHATBOX_SIZE_TOGGLE:
		{
			CKTDGUIStatic* pChatBox = (CKTDGUIStatic*)lParam;

			for(UINT i=0; i<m_vecChatSession.size(); ++i)
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
				if ( pChatSession != NULL )
				{
					CKTDGUIStatic *pChatBox1 = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl(L"Cha");
					CKTDGUIStatic *pChatBox2 = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl(L"Mini_Window");
					if(pChatBox == pChatBox1 || pChatBox == pChatBox2)
					{
						pChatSession->m_bMaxWindow = !pChatSession->m_bMaxWindow;
						SetChatBoxShow(pChatSession, true);

						// 포커스 이동
						CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
						if ( pIMEEdit != NULL && pChatSession->m_bMaxWindow == true)
						{
							pIMEEdit->RequestFocus();
						}

						return true;
					}
				}
			}
			return true;
		}
		break;
	case XCCUM_MGR_CHAT_SEND:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;

			for(UINT i=0; i<m_vecChatSession.size(); ++i)
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
				if ( pChatSession != NULL )
				{
					CKTDGUIButton *pChatBox = (CKTDGUIButton*)pChatSession->m_pDialog->GetControl(L"Button_Send");
					if(pButton == pChatBox)
					{		
						CKTDGUIIMEEditBox *pIMEEditBox = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
						m_pOpendChatSession = pChatSession;
						SendChatMessage( pIMEEditBox->GetText() );
						pIMEEditBox->ClearText();
						pIMEEditBox->RequestFocus();
						return true;
					}
				}
			}
		}
		break;
	case XCCUM_TRADE: // 개인거래
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			// 체험 아이디 제한 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}

			if(g_pData->GetUIManager() != NULL)
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_TRADE, true, m_iPickedUserUID);
		}
		break;
	case XCCUM_MARKET_SHOP: // 개인상점 보기
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			// 체험 아이디 제한 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}

			if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;


			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_SQUARE_GAME:
				{
					CX2SquareUnit* pCX2SquareUnit = g_pSquareGame->GetSquareUnitByUID( m_iPickedUserUID );
					if ( pCX2SquareUnit != NULL && 
						pCX2SquareUnit->GetUnit() != NULL )
					{
						if ( pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_SHOP )
						{
							if(g_pData->GetUIManager() != NULL)
								g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, m_iPickedUserUID);
						}
						else if ( pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_WAIT )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_65 ), g_pMain->GetNowState() );
						}
						else
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_66 ), g_pMain->GetNowState() );
						}
					}
				} break;
			default:
				break;
			}
		}
		break;
	// 채팅창 클릭시 edit로 포커스 강제이동
	case XCCUM_MGR_CHAT_FOCUS:
		{
			CKTDGUIStatic* pChatBox = (CKTDGUIStatic*)lParam;

			for(UINT i=0; i<m_vecChatSession.size(); ++i)
			{
				CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
				if ( pChatSession != NULL )
				{
					CKTDGUIStatic *pChatBox1 = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl(L"Cha");
					if(pChatBox == pChatBox1 )
					{
						if( pChatSession->m_bHideWindow == false && 
							pChatSession->m_bMaxWindow == true &&
							pChatSession->m_bShowWindow == true )
						{
							// 포커스 이동
							CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
							if ( pIMEEdit != NULL )
							{
								pIMEEdit->RequestFocus();
							}
						}
						return true;
					}
				}
			}
			return true;
		}
		break;
	// 사제탭 사제끊기 버튼
	case XCCUM_DISCIPLE_CUT:
		{
			if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;
			
			if(m_MessengerUserTab == XMUT_DISCIPLE)
			{
				if( NULL != g_pMain->GetTutorSystem() )
				{
					const CX2TutorSystem::TutorUnit* pTutorUnit = g_pMain->GetTutorSystem()->GetTutorUnit( m_iPickedUserUID );
					if( NULL == pTutorUnit )
						return true;

					m_pDLGQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
                        GET_REPLACED_STRING( ( STR_ID_63, "L", pTutorUnit->m_wstrNickName ) ), XCCUM_BAN_TUTOR_UNIT, (CKTDXStage*)m_pStage );
				}
				return true;
			}
		}

		// 전체탭 초대하기 버튼
	case XCCUM_PVP_INVITE:
		{
			if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;

			if(g_pMain->GetNowStateID() != CX2Main::XS_PVP_ROOM)
				return false;			

			if(m_iPickedUserIndex > 0)
			{
				KCommunityUserInfo userNode = m_vecFieldUser[m_iPickedUserIndex-1];

				// 같은 방에 있으면 초대 안 되게
				if(g_pData->GetPVPRoom() != NULL && g_pData->GetPVPRoom()->GetIndexByUnitUID(m_iPickedUserUID) != -1)
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2538 ), g_pMain->GetNowState() );
					return true;
				}

				// 다시 한번 확인해서 초대 메시지를 보내자
				switch(userNode.m_cState)
				{
				case KCommunityUserInfo::US_PVP_LOBBY:
				case KCommunityUserInfo::US_PVP_WAIT:
					{
						//m_iPickedUserUID = userNode.m_iUnitUID;					

						return Handler_EGS_INVITE_PVP_ROOM_REQ( m_iPickedUserUID );
					} break;
				default:
					break;
				}			
			}
			return false;
		}
		break;
		// 전체탭 같이하기 버튼
	case XCCUM_PVP_TOGETHER:
		{
			if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;

			// 이미 초대를 수락했거나 해서 스테이트를 이동중이면 다른 초대에 대한 수락 메세지는 처리하지 않는다
			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
			{
				return true;
			}

			if(m_iPickedUserIndex > 0)
			{
				KCommunityUserInfo userNode = m_vecFieldUser[m_iPickedUserIndex-1];

				switch(g_pMain->GetNowStateID())
				{
				case CX2Main::XS_PVP_LOBBY:
				case CX2Main::XS_PVP_ROOM:
					{
						// 같은 방에 있으면 안 따라가게
						if(g_pData->GetPVPRoom() != NULL && g_pData->GetPVPRoom()->GetIndexByUnitUID(m_iPickedUserUID) != -1)
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2538 ), g_pMain->GetNowState() );
							return true;
						}

						// 해당 유저의 방으로 이동하자
						CX2StateAutoChanger::TARGET_DETAIL targetDetail;
						targetDetail.m_iChannelID = userNode.GetRoomListID();
						targetDetail.m_iRoomUID = userNode.m_iRoomUID;

						g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_PVP_ROOM, targetDetail );	
						return true;
					} break;
				default:
					break;
				}
			}
			return false;
		}
		break;

		// 채널이동
#ifdef CHANGE_CHANNEL
	case XCCUM_SEARCH_FRIEND_RESULT_CHANNEL_MOVE:
		{
			// Not Yet
			if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP &&
				(true == g_pData->GetLocationManager()->IsVillage( g_pData->GetLocationManager()->GetCurrentVillageID() ) ||
				true == g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID()) ) )
			{
				if(m_bSearchUnitUnitInfoValid)
				{
					//return 채널_이동_하는_함수( m_SearchUnitUnitInfoEvent.m_kCUnitInfo.m_iChannelID );
					m_iChangeChannelId = m_SearchUnitUnitInfo.m_iChannelID;
					if(g_pInstanceData != NULL)
					{
						wstring wstrBuf = GET_REPLACED_STRING( ( STR_ID_2651, "S", g_pInstanceData->GetChannelServerName(m_iChangeChannelId).c_str() ) );
						m_pDLGQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrBuf.c_str(), 
							XCCUM_CHANGE_CHANNEL_ACCEPT, g_pMain->GetNowState(), XCCUM_CHANGE_CHANNEL_CANCEL );
						return true;
					}

				}		
			}			
			return false;
		} break;
	case XCCUM_CHANGE_CHANNEL_ACCEPT:
		{
			if ( m_pDLGQuery != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
				m_pDLGQuery = NULL;
			}

			// 채널이동승낙
			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			pState->Handler_EGS_CHECK_CHANNEL_CHANGE_REQ(m_iChangeChannelId);
			return true;
		}
		break;
	case XCCUM_CHANGE_CHANNEL_CANCEL:
		{
			if ( m_pDLGQuery != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
				m_pDLGQuery = NULL;
			}

			// 채널이동취소
			m_iChangeChannelId = 0;
			return true;
		}
		break;
#endif

		//{{ kimhc // 2009-09-18 // 길드 관련 CustomMSG 처리
#ifdef	GUILD_MANAGEMENT
	case XCCUM_PUSH_BUTTON_VIEW_LOACATION:	// 위치보기 버튼 클릭 시
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !"Wrong Path" );
				return true;
			}

			if ( g_pData->GetGuildManager()->DidJoinGuild() == false )
				return true;


			if ( m_eNowGuildViewInfo != GVI_LOCATION )
			{
				m_eNowGuildViewInfo	= GVI_LOCATION;

				UpdateInfoTypeString();
				//Handler_EGS_GET_GUILD_USER_LIST_REQ();
				UpdateGuildTabList();
			}
			
			return true;
		}
		break;

	case XCCUM_PUSH_BUTTON_VIEW_GUILD_MEMBER_GRADE:	// 등급보기 버튼 클릭 시
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !"Wrong Path" );
				return true;
			}

			if ( g_pData->GetGuildManager()->DidJoinGuild() == false )
				return true;

			if ( m_eNowGuildViewInfo != GVI_MEMBER_GRADE )
			{
				m_eNowGuildViewInfo = GVI_MEMBER_GRADE;

				UpdateInfoTypeString();
				//Handler_EGS_GET_GUILD_USER_LIST_REQ();
				UpdateGuildTabList();
			}

			return true;
		}
		break;

	case XCCUM_PUSH_BUTTON_ADD_GUILD_MEMBER:
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !L"GuildManager is NULL" );
				return false;
			}

			if ( g_pData->GetGuildManager()->DidJoinGuild() == false )
				return true;

			if ( g_pData->GetGuildManager()->GetUIGuild() == NULL )
			{
				ASSERT( !L"UIGuild is NULL" );
				return false;
			}

			if ( g_pData->GetGuildManager()->CanInviteMember() == true )
				g_pData->GetGuildManager()->GetUIGuild()->SetShowInviteGuildMemberDlg( true );
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4586 ), 
					g_pMain->GetNowState() );
			}

			return true;			
		}
		break;

	case XCCUM_PUSH_BUTTON_GUILD_INFO:
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !"Wrong Path" );
				return true;
			}

			if ( g_pData->GetGuildManager()->DidJoinGuild() == false )
				return true;

			//{{ 허상형 : [2009/9/28] //	길드 정보 UI 출력
			if( g_pData->GetGuildManager()->GetUIGuild() != NULL )
			{
				g_pData->GetGuildManager()->GetUIGuild()->SetShowGuildInfo( true );
			}

			return true;
			//}} 허상형 : [2009/9/28] //	길드 정보 UI 출력
		}
		break;

	case XCCUM_GUILD_PREV_PAGE:
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !"Wrong Path" );
				return true;
			}

			if ( g_pData->GetGuildManager()->DidJoinGuild() == false )
				return true;

			if ( m_uiNowGuildPage > 1 )
			{
				m_uiNowGuildPage--;

				Handler_EGS_GET_GUILD_USER_LIST_REQ();
			}

			return true;
		}
		break;

	case XCCUM_GUILD_NEXT_PAGE:
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !"Wrong Path" );
				return true;
			}

			if ( g_pData->GetGuildManager()->DidJoinGuild() == false )
				return true;

			if ( m_uiNowGuildPage < m_uiMaxGuildPage )
			{
				m_uiNowGuildPage++;

				Handler_EGS_GET_GUILD_USER_LIST_REQ();
			}

			return true;
		}
		break;

	case XCCUM_MOUSE_R_UP_ON_GUILD_MEMBER_GRADE:
		{
			CKTDGUIButton* pButton = NULL;
			pButton = reinterpret_cast< CKTDGUIButton* >( lParam );	
			
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !L"GuildManager is NULL" );
				return false;
			}

			if ( g_pData->GetGuildManager()->GetUIGuild() == NULL )
			{
				ASSERT( !L"UIGuild is NULL" );
				return false;
			}

			g_pData->GetGuildManager()->GetUIGuild()->SetShowChangeGradeMenu( false );

			if ( m_pDLGUserMenu != NULL )
				m_pDLGUserMenu->SetShow( false );

			if ( pButton != NULL )
			{
				CX2GuildManager::GUILD_USER_GRADE	selecterMemberGrade = CX2GuildManager::GUG_INVALID;
				
				if ( g_pData->GetMyUser() == NULL ||
					 g_pData->GetMyUser()->GetSelectUnit() == NULL )
					 return false;

				UidType	myUid = 0;
				myUid = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

				CheckButton( pButton );
				selecterMemberGrade		= CX2GuildManager::GUILD_USER_GRADE( GetGuildMemberGradeByUIDInThisList( m_iPickedUserUID ) );

				if ( selecterMemberGrade == CX2GuildManager::GUG_INVALID )
				{
					ASSERT( !L"selecter GuildMember Is Wrong!" );
					return false;
				}

				if( myUid == m_iPickedUserUID )	//	상형 : 자기 자신은 메뉴 띄워주지 않음
				{
					m_iPickedUserUID = 0;
					return true;
				}

				if ( g_pData->GetGuildManager() == NULL )
				{
					ASSERT( !L"GuildManager is NULL" );
					return false;
				}

				if ( g_pData->GetGuildManager()->GetUIGuild() == NULL )
				{
					ASSERT( !L"UIGuild is NULL" );
					return false;
				}

				m_SelectedGuildMemberUID = m_iPickedUserUID;
				g_pData->GetGuildManager()->GetUIGuild()->SetShowChangeGradeMenu( true, selecterMemberGrade );
			}

			return true;
		}
		break;

	case XCCUM_INVITE_GUILD_MEMBER:		// 길드 초대
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !L"GuildManager is NULL" );
				return false;
			}

			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

#ifdef SERV_USER_WATCH_NEW
			KEGS_USER_COMMUNITY_SURVEY_NEW_REQ kPacket;

			kPacket.m_wUnitNickName = ConvertOrgName( GetSelectedUnitName().c_str() ).c_str(); //2011.05.05 lygan_조성욱 // 커뮤니티 유닛 정보 안보이는것 수정 
			kPacket.m_cCommunity_Tab_Type = KEGS_USER_COMMUNITY_SURVEY_NEW_REQ::INVITE_GUILD;
			g_pData->GetServerProtocol()->SendPacket( EGS_USER_COMMUNITY_SURVEY_NEW_REQ, kPacket );
			g_pMain->AddServerPacket( EGS_USER_COMMUNITY_SURVEY_NEW_ACK  );

			return true;
#else //SERV_USER_WATCH_NEW
#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
			SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5127 ), g_pMain->GetNowState() );
				return true;
			}
			//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

			return g_pData->GetGuildManager()->Handler_EGS_INVITE_GUILD_REQ( m_iPickedUserUID );
#endif //SERV_USER_WATCH_NEW

		}
		break;

	case XCCUM_KICK_GUILD_MEMBER:		// 길드 추방
		{

			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			std::wstring wstrGuildMemberNickName = GetGuildMemberNickName( GetSelectedGuildMemberUID() );

			if ( wstrGuildMemberNickName.empty() == true )
			{
				ASSERT( !L"Wrong path" );
				return false;
			}
			
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999, -999), GET_REPLACED_STRING( ( STR_ID_4578, "L", wstrGuildMemberNickName ) ), 
				XCCUM_KICK_GUILD_MEMBER_OK, g_pMain->GetNowState(), XCCUM_KICK_GUILD_MEMBER_CANCEL );
			
			return true;
		}
		break;

	case XCCUM_KICK_GUILD_MEMBER_OK:
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !L"GuildManager is NULL" );
				return false;
			}

			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return g_pData->GetGuildManager()->Handler_EGS_KICK_GUILD_MEMBER_REQ( GetSelectedGuildMemberUID() );
		}
		break;

	case XCCUM_KICK_GUILD_MEMBER_CANCEL:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		}
		break;

#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-18 // 길드 관련 CustomMSG 처리
	case XCCUM_USERMENU_WATCH:
		{		
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);
#ifdef SERV_USER_WATCH_NEW
			KEGS_USER_COMMUNITY_SURVEY_NEW_REQ kPacket;

			kPacket.m_wUnitNickName = ConvertOrgName( GetSelectedUnitName().c_str() ).c_str(); //2011.05.05 lygan_조성욱 // 커뮤니티 유닛 정보 안보이는것 수정 
			kPacket.m_cCommunity_Tab_Type = KEGS_USER_COMMUNITY_SURVEY_NEW_REQ::USER_WATCH;
			g_pData->GetServerProtocol()->SendPacket( EGS_USER_COMMUNITY_SURVEY_NEW_REQ, kPacket );
			g_pMain->AddServerPacket( EGS_USER_COMMUNITY_SURVEY_NEW_ACK  );

#else //SERV_USER_WATCH_NEW
			if( m_iPickedUserUID > 0 && m_iPickedUserUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WATCH, true, m_iPickedUserUID);
			}			
#endif //SERV_USER_WATCH_NEW
		}
		break;

#ifdef SERV_ED_MONITORING_IN_GAME
	case XCCUM_USERMENU_IMAGE:
		{
			if( g_pInstanceData != NULL && g_pInstanceData->GetMoneyMonitoring() == true &&
				g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
			{
				if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
					return true;

				Handler_EGS_ED_MONITORING_BLOCK_REQ( m_iPickedUserUID, 1 );
			}
			return true;
		}
		break;
	case XCCUM_USERMENU_USER_BLOCK:
		{
			if( g_pInstanceData != NULL && g_pInstanceData->GetMoneyMonitoring() == true &&
				g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
			{
				if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
					return true;

				Handler_EGS_ED_MONITORING_BLOCK_REQ( m_iPickedUserUID, 2 );
			}
			return true;
		}
		break;
	case XCCUM_USERMENU_MACHINE_BLOCK:
		{
			if( g_pInstanceData != NULL && g_pInstanceData->GetMoneyMonitoring() == true &&
				g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
			{
				if( m_iPickedUserUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
					return true;

				Handler_EGS_ED_MONITORING_BLOCK_REQ( m_iPickedUserUID, 3 );
			}
			return true;
		}
		break;
#endif //SERV_ED_MONITORING_IN_GAME

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case XCCUM_TAB_RELATION :
		if ( NULL != g_pData->GetRelationshipManager() )
		{
			CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();
			if ( NULL != pRelationshipManager->GetMyRelationshipInfo() )
			{
				if ( pRelationshipManager->GetMyRelationshipInfo()->m_eRelationStateType !=
					SEnum::RT_SOLO )
				{
					if ( TRUE == pRelationshipManager->GetIsPossibleRefreshData() )
					{
						pRelationshipManager->SetIsPossibleRefreshData( FALSE );
						pRelationshipManager->Send_EGS_RELATIONSHIP_INFO_REQ();	
						return true;
					}
				}
				else
				{
					SetTab ( XMUT_RELATION );
					ResetRelationshipTabUI ();
					return true;
				}
			}
		}
		SetTab ( XMUT_RELATION );
		ResetRelationshipTabUI ();
		break;
	
	case XCCUM_INVITE_COUPLE   :
		{
			wstring wstrSeletedUnitName = GetSelectedUnitName();
			g_pData->GetRelationshipManager()->Send_EGS_COUPLE_PROPOSE_REQ ( wstrSeletedUnitName );
		}
		break;

	case XCCUM_SUMMON_COUPLE :
		{
			if(m_pDLGUserMenu != NULL)
				m_pDLGUserMenu->SetShowEnable(false, false);

			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP :
				{
					CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );	/// 패킷 핸들러가 정의된 CX2State 반환

					if ( NULL != pX2State && 
						 NULL != g_pTFieldGame && 
						 NULL != g_pTFieldGame->GetMyUnit() )
					{
						CX2SquareUnit* pMyUnit = g_pTFieldGame->GetMyUnit();	/// 마을에서의 내 유닛

						if( NULL != pMyUnit && NULL != pMyUnit->GetUnit() )
						{
							int				iMapID					= pMyUnit->GetUnit()->GetUnitData().m_nMapID;	/// 현재 유닛이 있는 맵 아이디
							D3DXVECTOR3		vMyPos					= pMyUnit->GetPos();							/// 현재 유닛의 위치
							unsigned char	ucLastTouchLineIndex	= pMyUnit->GetLastTouchLineIndex();				/// 현재 유닛이 가장 마지막에 접근한 라인맵 인덱스

							pX2State->Handler_EGS_CALL_MY_LOVER_REQ( iMapID, vMyPos, ucLastTouchLineIndex );		/// 위의 세가지 정보를 서버로 전송
						}
					}
				}
				break;

			case CX2Main::XS_BATTLE_FIELD :
				{
					CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );	/// 패킷 핸들러가 정의된 CX2State 반환

					if ( NULL != pX2State && 
						 NULL != g_pX2Game && 
						 NULL != g_pX2Game->GetMyUnit() )
					{
						CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();	/// 필드에서의 내 유닛

						if( NULL != pMyUnit && NULL != pMyUnit->GetUnit() )
						{
							int				iMapID					= pMyUnit->GetUnit()->GetUnitData().m_nMapID;	/// 현재 유닛이 있는 맵 아이디
							D3DXVECTOR3		vMyPos					= pMyUnit->GetPos();							/// 현재 유닛의 위치
							unsigned char	ucLastTouchLineIndex	= pMyUnit->GetLastTouchLineIndex();				/// 현재 유닛이 가장 마지막에 접근한 라인맵 인덱스

							pX2State->Handler_EGS_CALL_MY_LOVER_REQ( iMapID, vMyPos, ucLastTouchLineIndex );		/// 위의 세가지 정보를 서버로 전송
						}
					}
				}
				break;
			}
		}
		break;

#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_RECRUIT_EVENT_BASE
	case XCCUM_TAB_RECRUIT:
		{
			SetTab( XMUT_RECRUIT );
		}
		break;
#endif SERV_RECRUIT_EVENT_BASE
	default:
		break;
	}

	return false;
}

bool CX2Community::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
#ifdef NEW_MESSENGER
	case EGS_REQUEST_FRIEND_ACK:
		{
			return Handler_EGS_REQUEST_FRIEND_ACK( hWnd, uMsg, wParam, lParam ); 
		} break;
	case EGS_REQUEST_FRIEND_NOT:
		{
			return Handler_EGS_REQUEST_FRIEND_NOT( hWnd, uMsg, wParam, lParam ); 
		} break;
	case EGS_ACCEPT_FRIEND_ACK:
		{
			return Handler_EGS_ACCEPT_FRIEND_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ACCEPT_FRIEND_NOT:
		{
			return Handler_EGS_ACCEPT_FRIEND_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_DENY_FRIEND_ACK:
		{
			return Handler_EGS_DENY_FRIEND_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_DENY_FRIEND_NOT:
		{
			return Handler_EGS_DENY_FRIEND_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_BLOCK_FRIEND_ACK:
		{
			return Handler_EGS_BLOCK_FRIEND_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_BLOCK_FRIEND_NOT:
		{
			return Handler_EGS_BLOCK_FRIEND_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_UNBLOCK_FRIEND_ACK:
		{
			return Handler_EGS_UNBLOCK_FRIEND_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_UNBLOCK_FRIEND_NOT:
		{
			return Handler_EGS_UNBLOCK_FRIEND_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_DELETE_FRIEND_ACK:
		{
			return Handler_EGS_DELETE_FRIEND_ACK( hWnd, uMsg, wParam, lParam );
		} break;		
	case EGS_DELETE_FRIEND_NOT:
		{
			return Handler_EGS_DELETE_FRIEND_NOT( hWnd, uMsg, wParam, lParam );
		} break;		
	case EGS_MOVE_FRIEND_ACK:
		{
			return Handler_EGS_MOVE_FRIEND_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_MAKE_FRIEND_GROUP_ACK:
		{
			return Handler_EGS_MAKE_FRIEND_GROUP_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_RENAME_FRIEND_GROUP_ACK:
		{
			return Handler_EGS_RENAME_FRIEND_GROUP_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_DELETE_FRIEND_GROUP_ACK:
		{
			return Handler_EGS_DELETE_FRIEND_GROUP_ACK( hWnd, uMsg, wParam, lParam );		
		} break;	
	case EGS_FRIEND_POSITION_UPDATE_NOT:
		{
			return Handler_EGS_FRIEND_POSITION_UPDATE_NOT( hWnd, uMsg, wParam, lParam);
		}

#else NEW_MESSENGER
	case EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK:
		{
			return Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif NEW_MESSENGER

	case EGS_COMMUNITY_USER_LIST_ACK:
		{
			return Handler_EGS_COMMUNITY_USER_LIST_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#ifndef NEW_MESSENGER
	case EGS_UPDATE_FRIEND_INFO_ACK:
		{
			return Handler_EGS_UPDATE_FRIEND_INFO_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //#ifndef NEW_MESSENGER
	case EGS_INVITE_PVP_ROOM_NOT:
		{
			return Handler_EGS_INVITE_PVP_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_INVITE_PVP_ROOM_ACK:
		{
			return Handler_EGS_INVITE_PVP_ROOM_ACK( hWnd, uMsg, wParam, lParam );
		} break;

		//{{ kimhc // 2009-09-24 // 길드 관련 핸들러
#ifdef	GUILD_MANAGEMENT
	case EGS_GET_GUILD_USER_LIST_ACK:	// 길드원의 정보
		{
			return Handler_EGS_GET_GUILD_USER_LIST_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-24 // 길드 관련 핸들러

		//{{ kimhc // 2009-01-12 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	case EGS_GET_RECOMMEND_USER_LIST_ACK:	// 길드원의 정보
		{
			return Handler_EGS_GET_RECOMMEND_USER_LIST_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_RECOMMEND_USER_INFO_NOT:	// 길드원의 정보
		{
			return Handler_EGS_RECOMMEND_USER_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-12 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef SERV_ED_MONITORING_IN_GAME
	case EGS_ED_MONITORING_BLOCK_ACK:
		{
			return Handler_EGS_ED_MONITORING_BLOCK_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif
		// 2011.05.05 lygan_조성욱 // 중국용 커뮤니티탭 유저 정보 보기
#ifdef SERV_USER_WATCH_NEW
	case EGS_USER_COMMUNITY_SURVEY_NEW_ACK:
		{
			return Handler_EGS_USER_WATCH_NEW_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_USER_WATCH_NEW

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case EGS_RELATIONSHIP_INFO_ACK:
		{
			return Handler_EGS_RELATIONSHIP_INFO_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_RECRUIT_EVENT_BASE
	case EGS_GET_RECRUIT_RECRUITER_LIST_ACK:
		{
			return Handler_EGS_GET_RECRUIT_RECRUITER_LIST_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_RECRUIT_RECRUITER_INFO_NOT:
		{
			return Handler_EGS_RECRUIT_RECRUITER_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_RECRUIT_EVENT_BASE
	}

	return false;
}

void CX2Community::Reset()
{
	m_mapFriendUid.clear();
	ClearUserList();
	m_setOpenedGroupName.clear();
}

void CX2Community::SetOpen( bool bCheck )
{
	// 체험 아이디 제한 
	if( bCheck == true && true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
		return;
	}
	
	if( m_MessengerUserTab == XMUT_FIELD &&
		(g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME || g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME) )
	{
		m_MessengerUserTab = XMUT_FRIEND;
		//kimhc // 라디오 버튼이 번경 되지 않는 문제 수정
		CKTDGUIRadioButton* pRadioButton = static_cast< CKTDGUIRadioButton* >( m_pDlgBack->GetControl( L"friend" ) );
		pRadioButton->SetChecked( true );
	}

	if(m_bHideWindow == false)
		m_bOpen = bCheck;

	if ( bCheck == true )
	{
		//거시기.. ()포함된게 있으면 변경하라고 팝업띄워보자.


		m_bReceiveMessage = false;
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgBack, true );		

		
#ifdef ADDED_RELATIONSHIP_SYSTEM
		SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;
// 소스 정리 필요함		

		if( m_MessengerUserTab == XMUT_RELATION && eRelationType > SEnum::RT_SOLO )
		{
			if ( NULL != g_pData->GetRelationshipManager() )
				g_pData->GetRelationshipManager()->Send_EGS_RELATIONSHIP_INFO_REQ ();
		}
		else
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			for(int i=0; i<8; ++i)
			{
				if(m_pFieldUserIcon[i] != NULL)
				{
					m_pFieldUserIcon[i]->SetShowEnable(true, true);
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pFieldUserIcon[i], true );		
				}
			}

			SetTab(m_MessengerUserTab);

			if(m_MessengerUserTab == XMUT_FIELD)
				Handler_EGS_COMMUNITY_USER_LIST_REQ(1);		
		}		
					
		if(g_pData != NULL && g_pData->GetUIManager() != NULL)
		{
			g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_COMMUNITY);
#ifdef GUIDE_QUICK_QUEST_COMPLETE
			if( NULL != g_pData->GetUIManager()->GetUIQuestNew() )
				g_pData->GetUIManager()->GetUIQuestNew()->ShowUIDesc(false);
#endif //GUIDE_QUICK_QUEST_COMPLETE

#ifdef REFORM_QUEST
			//퀵퀘스트 완료 파티클이 UI를 가리면 제거하도록 수정
			if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
			{
				g_pData->GetUIManager()->GetUIQuestNew()->ClearQuestCompleteParticle();
			}
#endif //REFORM_QUEST
		}		
	}
	else
	{
		if ( m_pDLGUserMenu != NULL )
		{
			m_pDLGUserMenu->SetShowEnable( false, false );
		}

		if( m_pDLGAddFriend != NULL )
		{
			m_pDLGAddFriend->SetShowEnable( false, false );
		}


		if ( m_MessengerUserTab ==  XMUT_FRIEND)
		{
			m_CurrentTrackPos = GetListBox()->GetScrollBar()->GetTrackPos();
		}

		OpenAddFriendWindow( false );
		OpenCreateGroupWindow( false );
		OpenGroupMoveWindow( false );
		OpenGroupNameChangeWindow( false );
		OpenSearchFriendWindow(false);
		CloseSearchFriendResultWindow();

		if ( m_pDlgBack != NULL )
			m_pDlgBack->ClearFocus();

		SetOpenFriendReqMsgBox( false );

		m_pDlgField->SetShowEnable( false, false );
		m_pDlgFriend->SetShowEnable( false, false );

		//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
		m_pDlgGuild->SetShowEnable( false, false );
		if(g_pData->GetGuildManager() != NULL && g_pData->GetGuildManager()->GetUIGuild() != NULL)
		{
			g_pData->GetGuildManager()->GetUIGuild()->SetShowChangeGradeMenu( false );
		}
#endif	GUILD_MANAGEMENT
		//{{ kimhc // 2009-09-17 // 길드 탭 UI
		m_pDlgDisciple->SetShowEnable( false, false );

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
		if ( m_pDlgReferee != NULL )
			m_pDlgReferee->SetShowEnable( false, false );
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef	SERV_RECRUIT_EVENT_BASE
		if ( m_pDlgRecruit != NULL )
			m_pDlgRecruit->SetShowEnable( false, false );
#endif	SERV_RECRUIT_EVENT_BASE

		for(int i=0; i<8; ++i)
		{
			if(m_pFieldUserIcon[i] != NULL)
				m_pFieldUserIcon[i]->SetShowEnable(false, false);
		}

#ifdef ADDED_RELATIONSHIP_SYSTEM
		
		BOOST_FOREACH ( CX2UnitViewerUI * pUnitViewer, m_vecUnitViewerUI )
		{
			pUnitViewer->SetShowObject( false );
		}

		m_pDlgRelation->SetShowEnable( false, false );
		m_pDlgRelationOnlyPartnerShowing->SetShowEnable( false, false );
		m_pDlgRelationOnlyNotCouple->SetShowEnable ( false, false );
		if ( NULL != g_pData->GetUIManager() ) 
		{
			g_pData->GetUIManager()->SetShowSkillDesc( false );
		}
#endif // ADDED_RELATIONSHIP_SYSTEM

		if(g_pData != NULL && g_pData->GetUIManager() != NULL)
			g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_COMMUNITY);
	}


	//SetShowChatSessionList( m_bOpen );
	
	m_pDlgBack->SetShowEnable( bCheck, bCheck );
		
}

bool CX2Community::ChangeUserListTab( const WCHAR* pUserListTabName )
{
#if 0 
	if ( wcscmp( L"RadioButtonMessenger_Elsword_Friend", pUserListTabName ) == 0 )
	{
		SetTab( XMUT_ELSWORD_FRIEND );
	}
#endif
	return true;
}


void CX2Community::SetStage( CKTDXStage* pStage )
{
	m_pStage = pStage;

	m_pDlgBack->SetStage( m_pStage );
	

	m_pDLGUserMenu->SetStage( m_pStage );
	//m_pDLGGroupMenu->SetStage( m_pStage );

	X2_DIALOG_LAYER iLayer = XDL_MESSENGER;
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_PVP_GAME:
		{
			iLayer = XDL_POP_UP;
		} break;		
	default:
		iLayer = XDL_MESSENGER;
		break;
	}

#if 1
	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		if ( pChatSession->m_pDialog != NULL )
		{
			pChatSession->m_pDialog->SetStage( m_pStage );
			pChatSession->m_pDialog->SetLayer(iLayer);
		}
	}
#endif
	if ( m_pFriendReqMsg != NULL && m_pFriendReqMsg->m_pDialog != NULL )
	{
		m_pFriendReqMsg->m_pDialog->SetStage( pStage );
	}


	for(int i=0; i<8; ++i)
	{
		if(m_pFieldUserIcon[i] != NULL)
			m_pFieldUserIcon[i]->SetStage( pStage );
	}

}



void CX2Community::OnFrameMove( double fTime, float fElapsedTime )
{
	bool bCheckFriendReq = true;
	
	if ( g_pX2Game != NULL )
		bCheckFriendReq = false;

	if ( g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true )
		bCheckFriendReq = false;

	if( bCheckFriendReq == true )
	{
		CheckFriendReq();
	}
	
	if ( m_bOpen == true )
	{
		m_fFieldUserTimer += fElapsedTime;

		// 커뮤니티 창이 최상위로 올라왔을경우 인벤슬롯 disable시킨다.


		// 전체목록 자동 갱신(5초)
		if(m_fFieldUserTimer >= 5.f)
		{

			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_SQUARE_GAME:
				{
					if(m_pDLGUserMenu != NULL && m_pDLGUserMenu->GetShow() == false)
					{
						UpdateUserList(m_iNowFieldPage);
						m_fFieldUserTimer = 0.f;
					}
					else
					{
						m_fFieldUserTimer = 5.f;
					}
				} break;
			default:
				{
					if(m_MessengerUserTab == XMUT_FIELD && m_pDLGUserMenu != NULL && m_pDLGUserMenu->GetShow() == false )
					{
						if( m_pDLGSearchFriendResult == NULL || (m_pDLGSearchFriendResult != NULL && m_pDLGSearchFriendResult->GetShow() == false) )
						{
							Handler_EGS_COMMUNITY_USER_LIST_REQ(m_iNowFieldPage);
						}

						m_fFieldUserTimer = 0.f;
					}
					//{{ kimhc // 2009-09-25 // 길드 탭일 경우에도 자동으로 5초가 지나면 클릭했을때 생기는 테두리 지워주기
#ifdef	GUILD_MANAGEMENT
					else if( m_MessengerUserTab == XMUT_GUILD && m_pDLGUserMenu != NULL && m_pDLGUserMenu->GetShow() == false )
					{
						CheckButton( NULL );
						m_fFieldUserTimer = 0.f;
					}
#endif	GUILD_MANAGEMENT
					//}} kimhc // 2009-09-25 // 길드 탭일 경우에도 자동으로 5초가 지나면 클릭했을때 생기는 테두리 지워주기
					else
					{
						m_fFieldUserTimer = 5.f;
					}
				}
				break;
			}	
		}

		float startPosX = 400.f;
		float startPosY = 100.f;
		float stepX = 40.f;
		float stepY = 40.f;
		int iNewChatBox = 0;
		for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
		{
			CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
			if ( pChatSession->m_pDialog != NULL )
			{
				if(pChatSession->m_pDialog->GetPos().x == 400.f && pChatSession->m_pDialog->GetPos().y == 100.f)
				{
					pChatSession->m_pDialog->SetPos( D3DXVECTOR2(startPosX - (stepX*iNewChatBox), startPosY + (stepY*iNewChatBox)) );
					if(iNewChatBox <= 7)
					++iNewChatBox;
				}

				//pChatSession->m_pDialog->SetShow( false );
				//pChatSession->m_pDialog->SetEnable( false );
				SetChatBoxShow(pChatSession, true);
			}
		}		
#if 0 
		int dialogNum = 0;
		for ( int i = ((m_NowPageNum - 1 )*m_MaxChatSessionByPage); i < (m_NowPageNum*m_MaxChatSessionByPage) && i < (int)m_vecChatSession.size(); i++ )
		{
			CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
			if ( pChatSession->m_pDialog != NULL )
			{
				pChatSession->m_pDialog->SetShow( true );
				pChatSession->m_pDialog->SetEnable( true );

				//pChatSession->m_pDialog->SetPos( D3DXVECTOR2(  m_pDLGMgr->GetPos().x + m_ChatSessionOffsetPos.x, m_pDLGMgr->GetPos().y + m_ChatSessionOffsetPos.y + ( dialogNum * m_ChatSessionHeight ) ) );
				//pChatSession->m_pDialog->SetPos( D3DXVECTOR2(100.f, 100.f)  );

				if ( pChatSession->m_bAlarmReceiveMessage == true )
				{
					if ( pChatSession->m_pDialog->GetIsMoving() == false )
					{
						pChatSession->m_pDialog->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.5f );
						pChatSession->m_pDialog->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,0,0,1), 0.2f );
						pChatSession->m_pDialog->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.5f );
					}
				}
				else
				{
					pChatSession->m_pDialog->MoveStop();
					pChatSession->m_pDialog->SetColor( D3DXCOLOR(1,1,1,1) );
				}


				dialogNum++;

			}
		}
#endif

		CKTDGUIButton* pButtonParty = NULL;
		CKTDGUIButton* pButtonDisciple = NULL;
		CKTDGUIButton* pButtonInvite = NULL;	// 초대하기
		CKTDGUIButton* pButtonTogether = NULL;	// 같이하기

		if(m_MessengerUserTab == XMUT_FIELD)
		{
			pButtonParty		= (CKTDGUIButton*) m_pDlgField->GetControl( L"party" );
			pButtonInvite		= (CKTDGUIButton*) m_pDlgField->GetControl( L"Invite" );
			pButtonTogether		= (CKTDGUIButton*) m_pDlgField->GetControl( L"Together" );
#ifdef SERV_NO_DISCIPLE
			pButtonDisciple		= (CKTDGUIButton*) m_pDlgField->GetControl( L"disciple" );
			pButtonDisciple->SetShowEnable( false , false );
			pButtonDisciple		= NULL;
#else SERV_NO_DISCIPLE
			pButtonDisciple		= (CKTDGUIButton*) m_pDlgField->GetControl( L"disciple" );
#endif SERV_NO_DISCIPLE

			if(g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM || g_pMain->GetNowStateID() == CX2Main::XS_PVP_LOBBY)
			{
				if(m_iPickedUserIndex > 0)
				{
					KCommunityUserInfo userNode = m_vecFieldUser[m_iPickedUserIndex-1];

					if(pButtonInvite != NULL)
					{
						if( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM &&
							(userNode.m_cState == KCommunityUserInfo::US_PVP_LOBBY || userNode.m_cState == KCommunityUserInfo::US_PVP_WAIT) )
						{
							// 자신이 대전방에 있고 타유저가 로비나 대전방에 있을경우 초대하기 가능
							pButtonInvite->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
							pButtonInvite->SetShowEnable(true, true);
						}
						else
						{
							// 자신이 대전방에 없거나 타유저가 게임중이면 초대하기 불가
							pButtonInvite->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.3f));
							pButtonInvite->SetShowEnable(true, false);
						}
					}

					if(pButtonTogether != NULL)
					{
						if( userNode.m_cState == KCommunityUserInfo::US_PVP_WAIT ) 
						{
							// 타유저가 대전방에 있으면 같이하기 가능
							pButtonTogether->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
							pButtonTogether->SetShowEnable(true, true);
						}
						else
						{
							// 타유저가 대전방에 없으면 같이하기 불가
							pButtonTogether->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.3f));
							pButtonTogether->SetShowEnable(true, false);
						}
					}
				}
				else
				{
					if(pButtonInvite != NULL)
					{
						pButtonInvite->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.3f));
						pButtonInvite->SetShowEnable(true, false);
					}
					if(pButtonTogether != NULL)
					{
						pButtonTogether->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.3f));
						pButtonTogether->SetShowEnable(true, false);
					}
				}

				if(pButtonParty != NULL)
					pButtonParty->SetShowEnable(false, false);
			}
			else
			{
				if(pButtonParty != NULL)
				{
					pButtonParty->SetShow(true);

					if( false == g_pData->GetPartyManager()->DoIHaveParty() ||	//파티가 없거나
						 true == g_pData->GetPartyManager()->AmIPartyLeader() ) //파티장일 때
					{
						pButtonParty->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
						pButtonParty->SetEnable(true);
					}
					else
					{
						pButtonParty->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.3f));
						pButtonParty->SetEnable(false);
					}
				}		
				if(pButtonInvite != NULL)
					pButtonInvite->SetShowEnable(false, false);
				if(pButtonTogether != NULL)
					pButtonTogether->SetShowEnable(false, false);
			}
			
			if(pButtonDisciple != NULL)
			{
				if( g_pMain->GetTutorSystem()->GetNumberOfStudent() >= 3 || 
					g_pData->GetSelectUnitLevel() < 20)
				{
					pButtonDisciple->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.3f));
					pButtonDisciple->SetEnable(false);
				}
				else
				{
					pButtonDisciple->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					pButtonDisciple->SetEnable(true);
				}
			}
		}		
	}
	else
	{
		m_fFieldUserTimer = 0.f;
	}

#if 1 
	if ( m_bReceiveMessage == true )
	{
		wstring wstrMyNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
		wstrMyNickName += GET_STRING( STR_ID_67 );

		if( m_MessageSenderNickName.compare( wstrMyNickName ) != 0 )
		{	
			AlarmReceiveMessage( GET_REPLACED_STRING( ( STR_ID_68, "L", m_MessageSenderNickName ) ) );
		}		
		m_MessageSenderNickName = L"";
		m_bReceiveMessage = false;
	}

	m_pTalkBoxManager->OnFrameMove( fTime, fElapsedTime );
#endif

#if 1
	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl( L"IMEChat" );
		if ( pIMEEdit != NULL )
		{
			if ( pIMEEdit->GetHaveFocusIn() == true )
			{
#ifdef SERV_KEY_MAPPING_INT
				GET_KEY_STATE(GA_RETURN);
				g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN);
#else // SERV_KEY_MAPPING_INT
				g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN);
#endif // SERV_KEY_MAPPING_INT
				pChatSession->m_bAlarmReceiveMessage = false;
			}
		}

		if( pChatSession->m_pDialog != NULL && pChatSession->m_bShowWindow == true )
		{
			SetChatBoxShow(pChatSession, true);
		}
	}	
#endif	

#ifdef ADDED_RELATIONSHIP_SYSTEM
	
	if ( m_MessengerUserTab == XMUT_RELATION )
	{
		BOOST_FOREACH ( CX2UnitViewerUI* pUnitViewer, m_vecUnitViewerUI )
		{
			if ( NULL != pUnitViewer && true == pUnitViewer->GetShowObject() )
			{
				pUnitViewer->OnFrameMove(fTime, fElapsedTime);
			}
		}
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FIX_INVITE_PVP_PLAYER // 김태환
	/// 해당 쿨타임이 종료되면, 초대 룸 리스트를 초기화 한다.
	if ( 0.f < m_fInviteCoolTime )
	{
		m_fInviteCoolTime -= fElapsedTime;

		if ( 0.f >= m_fInviteCoolTime )
			m_vecInviteRoomUID.clear();
	}
#endif // FIX_INVITE_PVP_PLAYER
}

void CX2Community::OnFrameRender()
{
	m_pTalkBoxManager->OnFrameRender();
}
#ifndef NEW_MESSENGER
void CX2Community::Handler_EGS_UPDATE_FRIEND_INFO_REQ(wstring &wstrNickName, bool bDelete)
{
	// 친구목록에서 친구삭제시 친구정보 갱신
	KEGS_UPDATE_FRIEND_INFO_REQ kPacket;
	kPacket.m_wstrFriendNickName = wstrNickName;
	kPacket.m_bDelete = bDelete;

	std::map<wstring, UidType>::iterator mit;
	mit = m_mapFriendUid.find( wstrNickName );

	if( mit != m_mapFriendUid.end() )
	{
		m_mapFriendUid.erase(mit);
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_FRIEND_INFO_REQ, kPacket );

}

void CX2Community::Handler_EGS_UPDATE_FRIEND_INFO_REQ(CNMFriendInfo &friendInfo, bool bFirstSync)
{
	// 친구목록 변경시 서버로 친구정보를 보내고 uid를 얻어온다.
	KEGS_UPDATE_FRIEND_INFO_REQ kPacket;
	wstring wstrNickName = CX2Community::ConvertOrgName( friendInfo.szNickName );
	kPacket.m_wstrFriendNickName = wstrNickName;

	if( IS_ONLINE( friendInfo.uStatus ) )
	{		
		if ( friendInfo.bIsVirtualOnline &&
			friendInfo.keyFriend.keyVirtual.uGameCode != kGameCode_nexoncom )
		{
			kPacket.m_bDelete = false;	// 게임 접속중
		}
		else
		{
			kPacket.m_bDelete = true;	// 온라인
		}
	}
	else
	{
		kPacket.m_bDelete = true;	// 오프라인
	}	

	if(bFirstSync == true && kPacket.m_bDelete == true)
		return;

	g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_FRIEND_INFO_REQ, kPacket );
	//g_pMain->AddServerPacket( EGS_UPDATE_FRIEND_INFO_ACK );
}

bool CX2Community::Handler_EGS_UPDATE_FRIEND_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_FRIEND_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	//if( g_pMain->DeleteServerPacket( EGS_UPDATE_FRIEND_INFO_ACK ) == true )
	{
		if( kEvent.m_iOK == NetError::NET_OK )
		{
			if( GetFriendUID(kEvent.m_wstrFriendNickName) == 0)
			{
				m_mapFriendUid.insert(make_pair(kEvent.m_wstrFriendNickName, kEvent.m_iFriendUnitUID));
			}

			MessengerUser *pMessengerUser = GetMessengerUser(kEvent.m_wstrFriendNickName.c_str());
			if(pMessengerUser != NULL)
			{
				pMessengerUser->m_UnitUID = kEvent.m_iFriendUnitUID;

#ifdef ADDED_RELATIONSHIP_SYSTEM			
				if ( NULL != g_pData->GetRelationshipManager() &&
					 NULL != g_pData->GetRelationshipManager()->GetUIRelationship() )
				{
					g_pData->GetRelationshipManager()->GetUIRelationship()->GetFriendListForInvitingWeddingUser();
				}
#endif // ADDED_RELATIONSHIP_SYSTEM
				return true;
			}
		}		
	}

	return false;
}


void CX2Community::SyncFriendList()
{
	ClearUserList();

	if ( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return;	

	NMCategoryUserList userList;	//그룹에 속해있는 친구 리스트
	NMFriendList friendList;		// 그룹에 속해있지 않은 친구 리스트
	if ( CNMCOClientObject::GetInstance().GetFriendList( userList, friendList, TRUE ) == FALSE )
	{
		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), L"친구 목록 얻어오기 실패", m_pStage );
		return;
	}

	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
	NMVirtualKey myNMVirtualKey;
	myNMVirtualKey.uGameCode = NEXON_KOREA_ELSWORD_GAMECODE;
	myNMVirtualKey.uVirtualIDCode = pMyUnit->GetUnitData().m_iNMKSerialNum;

	//그룹에 속해있지 않은 친구 리스트 등록
	for ( int i = 0; i < (int)friendList.size(); i++ )
	{
		CNMFriendInfo& friendInfo = friendList[i];

		if ( friendInfo.keyFriend.keyOwner == myNMVirtualKey )
		{
			CX2Community::MessengerUser* pMessengerUser = new CX2Community::MessengerUser();

			pMessengerUser->Update( friendInfo, m_bRefresh );
			pMessengerUser->m_uCategoryCode = 0;

			m_pMessengerUserList->m_vecDefaultGroupUser.push_back( pMessengerUser );
			Handler_EGS_UPDATE_FRIEND_INFO_REQ(friendInfo, true);
		}
	}

	//그룹에 속해있는 친구 리스트 등록
	for ( int i = 0; i < (int)userList.size(); i++ )
	{
		CNMCateFriendInfo& cateFriendInfo = userList[i];

		if ( cateFriendInfo.keyOwner == myNMVirtualKey )
		{
			CX2Community::MessengerUserGroup* pMgrUserGroup = new CX2Community::MessengerUserGroup();
			pMgrUserGroup->m_GroupName = cateFriendInfo.szCategoryName;
			pMgrUserGroup->m_uCategoryCode = cateFriendInfo.uCategoryCode;

			m_pMessengerUserList->m_vecUserGroup.push_back( pMgrUserGroup );

			for ( int i = 0; i < (int)cateFriendInfo.aFriends.size(); i++ )
			{
				CNMFriendInfo& friendInfo = cateFriendInfo.aFriends[i];

				if ( friendInfo.keyFriend.keyOwner == myNMVirtualKey )
				{
					CX2Community::MessengerUser* pMessengerUser = new CX2Community::MessengerUser();
					pMessengerUser->Update( friendInfo, m_bRefresh );
					pMessengerUser->m_uCategoryCode = cateFriendInfo.uCategoryCode;

					pMgrUserGroup->m_vecUser.push_back( pMessengerUser );
					Handler_EGS_UPDATE_FRIEND_INFO_REQ(friendInfo, true);
				}
			}

			
		}
	}
}

void CX2Community::ResetUserList()
{
	if ( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return;

	ClearUserList();

	NMCategoryUserList userList;	//그룹에 속해있는 친구 리스트
	NMFriendList friendList;		// 그룹에 속해있지 않은 친구 리스트
	if ( CNMCOClientObject::GetInstance().GetFriendList( userList, friendList, TRUE ) == FALSE )
	{
		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), L"친구 목록 얻어오기 실패", m_pStage );
	}

	if ( m_MessengerUserTab == XMUT_FRIEND )
		m_CurrentTrackPos = GetListBox()->GetScrollBar()->GetTrackPos();

	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
	NMVirtualKey myNMVirtualKey;
	myNMVirtualKey.uGameCode = NEXON_KOREA_ELSWORD_GAMECODE;
	myNMVirtualKey.uVirtualIDCode = pMyUnit->GetUnitData().m_iNMKSerialNum;

	//그룹에 속해있지 않은 친구 리스트 등록
	for ( int i = 0; i < (int)friendList.size(); i++ )
	{
		CNMFriendInfo& friendInfo = friendList[i];

		if ( friendInfo.keyFriend.keyOwner == myNMVirtualKey )
		{
			CX2Community::MessengerUser* pMessengerUser = new CX2Community::MessengerUser();
			
			pMessengerUser->Update( friendInfo, m_bRefresh );
			pMessengerUser->m_uCategoryCode = 0;

			m_pMessengerUserList->m_vecDefaultGroupUser.push_back( pMessengerUser );
		}
	}

	//그룹에 속해있는 친구 리스트 등록
	for ( int i = 0; i < (int)userList.size(); i++ )
	{
		CNMCateFriendInfo& cateFriendInfo = userList[i];

		if ( cateFriendInfo.keyOwner == myNMVirtualKey )
		{
			CX2Community::MessengerUserGroup* pMgrUserGroup = new CX2Community::MessengerUserGroup();
			pMgrUserGroup->m_GroupName = cateFriendInfo.szCategoryName;
			pMgrUserGroup->m_uCategoryCode = cateFriendInfo.uCategoryCode;

			for ( int i = 0; i < (int)cateFriendInfo.aFriends.size(); i++ )
			{
				CNMFriendInfo& friendInfo = cateFriendInfo.aFriends[i];

				if ( friendInfo.keyFriend.keyOwner == myNMVirtualKey )
				{
					CX2Community::MessengerUser* pMessengerUser = new CX2Community::MessengerUser();
					pMessengerUser->Update( friendInfo, m_bRefresh );
					pMessengerUser->m_uCategoryCode = cateFriendInfo.uCategoryCode;

					pMgrUserGroup->m_vecUser.push_back( pMessengerUser );
				}
			}

			m_pMessengerUserList->m_vecUserGroup.push_back( pMgrUserGroup );
		}
	}

	ResetUserListUI();
}
#endif // ifndef NEW_MESSENGER


bool CX2Community::OpenSearchFriendWindow( bool bOpen )
{
	if ( bOpen == true )
	{	
		SAFE_DELETE_DIALOG( m_pDLGSearchFriend );
		m_pDLGSearchFriend = new CKTDGUIDialog( m_pStage, L"DLG_Search_Window.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSearchFriend );
		return true;
	}
	else
	{
		if(m_pDLGSearchFriend != NULL)
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSearchFriend, NULL, false );
		m_pDLGSearchFriend = NULL;
		return true;
	}	

	return false;

}

bool CX2Community::SearchFriendReq()
{
	if( NULL != m_pDLGSearchFriend )
	{
		CKTDGUIIMEEditBox* pIMEEditID = (CKTDGUIIMEEditBox*)m_pDLGSearchFriend->GetControl( L"IMEEditBoxName" );

		if ( pIMEEditID != NULL )
		{
			
			CX2State* pState = (CX2State*)m_pStage;
			if ( pState != NULL )
			{
				wstring nickName = pIMEEditID->GetText();
#ifdef SIMPLE_BUG_FIX
				if( g_pData != NULL && g_pData->GetMyUser() != NULL)
				{
					if( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName == nickName )
					{
#ifdef HARDCODING_STRING_TO_INDEX
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_2539), g_pMain->GetNowState() );
#else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"자기 자신입니다.", g_pMain->GetNowState() );
#endif HARDCODING_STRING_TO_INDEX
						return false;
					}
					else if( NULL != g_pData->GetMyUser()->GetUnitByNickName(nickName.c_str()) )
					{
#ifdef HARDCODING_STRING_TO_INDEX
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_4883), g_pMain->GetNowState() );
#else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"같은 계정의 캐릭터는 친구로 등록할수 없습니다.", g_pMain->GetNowState() );
#endif HARDCODING_STRING_TO_INDEX
						return false;
					}
				}
#endif // SIMPLE_BUG_FIX
				return pState->Handler_EGS_SEARCH_UNIT_REQ( nickName.c_str() );
			}
		}
	}
	return false;
}

bool CX2Community::CloseSearchFriendResultWindow()
{
	if(m_pDLGSearchFriendResult != NULL)
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSearchFriendResult, NULL, false );
	m_pDLGSearchFriendResult = NULL;
	SAFE_DELETE_DIALOG(m_pDLGSearchFriendResultCharIcon);
	m_bSearchUnitUnitInfoValid = false;
	return true;
}

bool CX2Community::OpenSearchFriendResultWindow( KEGS_SEARCH_UNIT_ACK kEvent )
{
	if(kEvent.m_kCUnitInfo.m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2539 ), m_pStage );
		return true;
	}

	// 패킷 정보를 저장해 둔다.
	m_SearchUnitUnitInfo = kEvent.m_kCUnitInfo;
	m_bSearchUnitUnitInfoValid = true;
	SAFE_DELETE_DIALOG(m_pDLGSearchFriend);

	SAFE_DELETE_DIALOG( m_pDLGSearchFriendResult );
	SAFE_DELETE_DIALOG( m_pDLGSearchFriendResultCharIcon );
	m_pDLGSearchFriendResult = new CKTDGUIDialog( m_pStage, L"DLG_Search_Friend.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSearchFriendResult );

	if(NULL != m_pDLGSearchFriendResult)
	{
		CKTDGUIStatic* pStatic_NickName = (CKTDGUIStatic*) m_pDLGSearchFriendResult->GetControl( L"Static_NickName" );
		CKTDGUIStatic* pStatic_Channel = (CKTDGUIStatic*) m_pDLGSearchFriendResult->GetControl( L"Static_channel_name" );
		CKTDGUIStatic* pStatic_State = (CKTDGUIStatic*) m_pDLGSearchFriendResult->GetControl( L"Static_State" );

		CKTDGUIButton* pButton_AddFriend = (CKTDGUIButton*) m_pDLGSearchFriendResult->GetControl( L"Button_Friend_add" );
		CKTDGUIButton* pButton_MoveChannel = (CKTDGUIButton*) m_pDLGSearchFriendResult->GetControl( L"Button_Channel_Move" );
		CKTDGUIButton* pButton_PartyInvite = (CKTDGUIButton*) m_pDLGSearchFriendResult->GetControl( L"Button_Party_Invnte" );
		CKTDGUIButton* pButton_PartyRequest = (CKTDGUIButton*) m_pDLGSearchFriendResult->GetControl( L"Button_Party_Request" );
		CKTDGUIButton* pButton_PVPInvite = (CKTDGUIButton*) m_pDLGSearchFriendResult->GetControl( L"Button_PVP_Invite" );
		CKTDGUIButton* pButton_PVPPlayTogether = (CKTDGUIButton*) m_pDLGSearchFriendResult->GetControl( L"Button_PVP_Together" );
		
		// 캐릭터 아이콘
		m_pDLGSearchFriendResultCharIcon = new CKTDGUIDialog( m_pStage, L"DLG_UI_Character_Icon.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSearchFriendResultCharIcon );
		D3DXVECTOR2 pos = m_pDLGSearchFriendResult->GetPos();
		pos.x += m_pDLGSearchFriendResult->GetDummyPos(0).x;
		pos.y += m_pDLGSearchFriendResult->GetDummyPos(0).y;
		m_pDLGSearchFriendResultCharIcon->SetPos(pos);
		
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGSearchFriendResultCharIcon, m_pDLGSearchFriendResult->GetLayer());
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGSearchFriendResultCharIcon, true );

		CKTDGUIStatic* pStaticClass = (CKTDGUIStatic*) m_pDLGSearchFriendResultCharIcon->GetControl( L"user" );	// 직업아이콘
		CKTDGUIStatic* pStaticLv10	= (CKTDGUIStatic*) m_pDLGSearchFriendResultCharIcon->GetControl( L"ten" );	// 레벨 10자리
		CKTDGUIStatic* pStaticLv1	= (CKTDGUIStatic*) m_pDLGSearchFriendResultCharIcon->GetControl( L"one" );	// 레벨 1자리

		for( int i=0; i< pStaticLv10->GetPictureNum(); i++ )
		{
			if( NULL != pStaticLv10->GetPicture(i) )
			{
				pStaticLv10->GetPicture(i)->SetShow( false );
			}

			if( NULL != pStaticLv1->GetPicture(i) )
			{
				pStaticLv1->GetPicture(i)->SetShow( false );
			}
		}
		for( int i=0; i< pStaticClass->GetPictureNum(); i++ )
		{
			if( NULL != pStaticClass->GetPicture(i) )
			{
				pStaticClass->GetPicture(i)->SetShow( false );
			}
		}
		

		int unitClass = (int)m_SearchUnitUnitInfo.m_cUnitClass;
		int unitLv = (int)m_SearchUnitUnitInfo.m_ucLevel;
		int unitLv10 = unitLv / 10;
		int unitLv1 = unitLv % 10;
		
		
		if( NULL != pStaticClass && 
			NULL != pStaticClass->GetPicture(0) )
		{
			CKTDGUIControl::CPictureData* pPicture = pStaticClass->GetPicture(0);

			wstring fileName;
			wstring pieceName;
			if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS) unitClass, CX2Data::CIT_20by20 ) )
			{
				pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
			}
			else
			{
				ASSERT( !"NO" );
			}
		}


		pStaticClass->GetPicture(0)->SetShow( true );
		pStaticLv10->GetPicture(unitLv10)->SetShow(true);
		pStaticLv1->GetPicture(unitLv1)->SetShow(true);



		if(NULL != pStatic_NickName)
		{
			pStatic_NickName->GetString(0)->msg = m_SearchUnitUnitInfo.m_wstrCharName;
		}

		// 채널 이름을 집어넣자
		wstringstream wstrmChannelInfo;
		if( g_pInstanceData != NULL && m_SearchUnitUnitInfo.m_iChannelID > 0 )
		{
			wstrmChannelInfo << g_pInstanceData->GetChannelServerName( m_SearchUnitUnitInfo.m_iChannelID );
		}			

		bool bTargetPlayerisInSameChannelWithMe = false;
		if( m_SearchUnitUnitInfo.m_iChannelID == g_pInstanceData->GetConnectChannelServerID())
		{
			bTargetPlayerisInSameChannelWithMe = true;
		}

		wstring wstrState = L"";
		switch( m_SearchUnitUnitInfo.m_cState )
		{
		case CX2Unit::CUS_FIELD_MAP:
			{
				// 마을 이름을 집어넣자
				SEnum::VILLAGE_MAP_ID eVillageID = (SEnum::VILLAGE_MAP_ID ) m_SearchUnitUnitInfo.m_iStateCode;

				if( true == g_pData->GetLocationManager()->IsVillage( eVillageID ) ||
					true == g_pData->GetLocationManager()->IsDungeonGate( eVillageID ) )
				{
					CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eVillageID );
					if( NULL != pVillageTemplet )
					{
						wstrmChannelInfo << L"(" << pVillageTemplet->m_Name << L")";
					}
				}
				else if( true == g_pData->GetLocationManager()->IsDungeonLounge( eVillageID ) )
				{
					CX2LocationManager::LOCAL_MAP_ID eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( eVillageID );
					SEnum::VILLAGE_MAP_ID eDungeonGateID = (SEnum::VILLAGE_MAP_ID) g_pData->GetLocationManager()->GetDungeonGateID( eLocalMapID );
					CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eDungeonGateID );
					if( NULL != pVillageTemplet )
					{
						wstrmChannelInfo << L"(" << pVillageTemplet->m_Name << L")";
					}
				}
				else
				{
					ASSERT( !"invalid" );
				}
				// 마을 이름 받았음

				if(m_SearchUnitUnitInfo.m_iPartyUID == 0)
				{
					// 파티 없음
					wstrState = L"";
				}
				else
				{
					wstrState = GET_STRING( STR_ID_106 );
				}

			} break;

		case CX2Unit::CUS_MARKET:
			{
				wstrmChannelInfo << GET_STRING( STR_ID_2540 );
			} break;
		
		case CX2Unit::CUS_PVP_LOBBY:
			{
				wstrmChannelInfo << GET_STRING( STR_ID_2541 );
				wstrState = GET_STRING( STR_ID_2542 );
			} break;
		case CX2Unit::CUS_PVP_WAIT:
			{
				wstrmChannelInfo << GET_STRING( STR_ID_2541 );
				wstrState = GET_STRING( STR_ID_2543 );
			} break;
		case CX2Unit::CUS_PVP_PLAY:
			{
				wstrmChannelInfo << GET_STRING( STR_ID_2541 );
				wstrState = GET_STRING( STR_ID_2544 );
			} break;
		case CX2Unit::CUS_TC_PLAY:
			{
				wstrState = GET_STRING( STR_ID_2545 );
			} break;
        case CX2Unit::CUS_DUNGEON_PLAY:
			{
				wstrState = GET_STRING( STR_ID_2546 );
			} break;
		case CX2Unit::CUS_ARCADE_PLAY:
			{
				wstrState = GET_STRING( STR_ID_2547 );
			} break;
		default:
			{
			} break;

		}
		
		if( NULL != pStatic_State )
		{
			pStatic_State->GetString(0)->msg = wstrState.c_str();
		}

		if(NULL != pStatic_Channel)
		{
			pStatic_Channel->GetString(0)->msg = wstrmChannelInfo.str().c_str();
		}

		// 기본 버튼의 상태를 지정해 주자
		if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE) == false &&
			g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP &&
			(true == g_pData->GetLocationManager()->IsVillage( g_pData->GetLocationManager()->GetCurrentVillageID() ) ||
			true == g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID()) ) )
			pButton_MoveChannel->SetShowEnable(true, true);
		else
			pButton_MoveChannel->SetShowEnable(true, false);


		// state 별로 다르게 나올 버튼들의 상태를 지정해 주자
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_SQUARE_GAME:
			{
				if(NULL != pButton_PartyInvite)
				{
					if( true == bTargetPlayerisInSameChannelWithMe &&	// 같은 채널
						m_SearchUnitUnitInfo.m_cState == CX2Unit::CUS_FIELD_MAP &&							// 상대가 필드에 있고
						NULL != g_pData->GetPartyManager() && g_pData->GetPartyManager()->DoIHaveParty() &&	// 내가 파티가 있고
						m_SearchUnitUnitInfo.m_iPartyUID == 0 )												// 상대는 파티가 없다
					{
						pButton_PartyInvite->SetShowEnable(true, true);
					}
					else
					{
						pButton_PartyInvite->SetShowEnable(true, false);
					}
				}

				if(NULL != pButton_PartyRequest)
				{
					if( true == bTargetPlayerisInSameChannelWithMe &&	// 같은 채널
						m_SearchUnitUnitInfo.m_cState == CX2Unit::CUS_FIELD_MAP &&										// 상대가 필드에 있고
						m_SearchUnitUnitInfo.m_iPartyUID != 0 &&														// 상대가 파티에 있고
						NULL != g_pData->GetPartyManager() && false == g_pData->GetPartyManager()->DoIHaveParty() )		// 나는 파티가 없다
					{
						pButton_PartyRequest->SetShowEnable(true, true);
					}
					else
					{
						pButton_PartyRequest->SetShowEnable(true, false);
					}
				}
				if(NULL != pButton_PVPInvite)
					pButton_PVPInvite->SetShowEnable(false, false);
				if(NULL != pButton_PVPPlayTogether)
					pButton_PVPPlayTogether->SetShowEnable(false, false);

			} break;
		case CX2Main::XS_PVP_LOBBY:
			{
				if(NULL != pButton_PartyInvite)
					pButton_PartyInvite->SetShowEnable(false, false);
				if(NULL != pButton_PartyRequest)
					pButton_PartyRequest->SetShowEnable(false, false);
				if(NULL != pButton_PVPInvite)
				{
					pButton_PVPInvite->SetShowEnable(true, false);
				}
				if(NULL != pButton_PVPPlayTogether)
				{
					if(m_SearchUnitUnitInfo.m_cState == CX2Unit::CUS_PVP_WAIT)
						pButton_PVPPlayTogether->SetShowEnable(true, true);
					else
						pButton_PVPPlayTogether->SetShowEnable(true, false);
				}

			} break;
		case CX2Main::XS_PVP_ROOM:
			{
				if(NULL != pButton_PartyInvite)
					pButton_PartyInvite->SetShowEnable(false, false);
				if(NULL != pButton_PartyRequest)
					pButton_PartyRequest->SetShowEnable(false, false);
				if(NULL != pButton_PVPInvite)
				{
					if( m_SearchUnitUnitInfo.m_cState == CX2Unit::CUS_PVP_WAIT ||
						m_SearchUnitUnitInfo.m_cState == CX2Unit::CUS_PVP_LOBBY )
					{
						pButton_PVPInvite->SetShowEnable(true, true);
					}					
					else
					{
						pButton_PVPInvite->SetShowEnable(true, false);
					}
				}
				if(NULL != pButton_PVPPlayTogether)
				{
					if(m_SearchUnitUnitInfo.m_cState == CX2Unit::CUS_PVP_WAIT)
						pButton_PVPPlayTogether->SetShowEnable(true, true);
					else
						pButton_PVPPlayTogether->SetShowEnable(true, false);
				}
			} break;
		default:
			{
				if(NULL != pButton_PartyInvite)
					pButton_PartyInvite->SetShowEnable(true, false);
				if(NULL != pButton_PartyRequest)
					pButton_PartyRequest->SetShowEnable(true, false);
				if(NULL != pButton_PVPInvite)
					pButton_PVPInvite->SetShowEnable(false, false);
				if(NULL != pButton_PVPPlayTogether)
					pButton_PVPPlayTogether->SetShowEnable(false, false);

			} break;
		}
	}

	return true;
}


bool CX2Community::OpenAddFriendWindow( bool bCheck )
{
	if ( bCheck == true )
	{	
		SAFE_DELETE_DIALOG( m_pDLGAddFriend );
		m_pDLGAddFriend = new CKTDGUIDialog( m_pStage, L"DLG_UI_Friend_Make.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAddFriend );
	}
	else
	{
		if(m_pDLGAddFriend != NULL)
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGAddFriend, NULL, false );
		m_pDLGAddFriend = NULL;
	}	

	return true;
}

bool CX2Community::FriendReq()
{
	if ( m_pDLGAddFriend != NULL )
	{
		CKTDGUIIMEEditBox* pIMEEditID = (CKTDGUIIMEEditBox*)m_pDLGAddFriend->GetControl( L"IMEID" );



		if ( pIMEEditID != NULL )
		{
			

			wstring nickName = pIMEEditID->GetText();

#ifdef NEW_MESSENGER
			wstring addFriendMessage = GET_STRING( STR_ID_74 );
			if ( m_pDLGAddFriend != NULL )
			{
				CKTDGUIIMEEditBox* pIMEEditMessage = (CKTDGUIIMEEditBox*)m_pDLGAddFriend->GetControl( L"IMEMessage" );
				if ( pIMEEditMessage != NULL )
					addFriendMessage = pIMEEditMessage->GetText();
			}
			Handler_EGS_REQUEST_FRIEND_REQ( nickName, addFriendMessage );
#else
			m_pDLGFriendReqWait = g_pMain->KTDGUIMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_69 ), m_pStage );
			Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( nickName.c_str() );
#endif

			return true;
		}
		else
		{

			return false;
		}
	}
	
	return false;
}

#ifndef NEW_MESSENGER
bool CX2Community::OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent )
{
	if ( m_pFriendReqMsg != NULL )
		return false;

	if ( pEvent->rnfData.keyToVirtual.uGameCode != NEXON_KOREA_ELSWORD_GAMECODE )
		return false;

	CX2Community::FriendReqMsg* pNewFriendReqMsg = new CX2Community::FriendReqMsg();
	pNewFriendReqMsg->m_ToUnitNMSN = pEvent->rnfData.keyToVirtual.uVirtualIDCode;
	pNewFriendReqMsg->m_FromNickName = pEvent->rnfData.szFromNickName;
	pNewFriendReqMsg->m_ToNickName = pEvent->rnfData.szRequestID;
	pNewFriendReqMsg->m_Msg = pEvent->rnfData.szMessage;
	pNewFriendReqMsg->m_uSerialNo = pEvent->rnfData.uSerialNo;

	m_pFriendReqMsg = pNewFriendReqMsg;

	return true;
}
#endif

void CX2Community::CheckFriendReq()
{
	if ( m_pFriendReqMsg != NULL  )
	{
#ifdef NEW_MESSENGER
		if ( m_pFriendReqMsg->m_bPopUpCheck == false && g_pMain->GetNowStateID() > CX2Main::XS_CREATE_UNIT )
#else
		if ( m_pFriendReqMsg->m_bPopUpCheck == false )
#endif NEW_MESSENGER
		{
			m_pFriendReqMsg->m_pDialog = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_70, "LL", m_pFriendReqMsg->m_FromNickName, m_pFriendReqMsg->m_Msg ) ), 
				CX2Community::XCCUM_ADD_FRIEND_REQ_CONFIRM, m_pStage, CX2Community::XCCUM_ADD_FRIEND_REQ_NOT_CONFIRM );

			m_pFriendReqMsg->m_bPopUpCheck = true;
		}

	}
#ifdef NEW_MESSENGER
	else if( false == m_vecFriendMessageInfo.empty() )
	{				
		std::vector< KFriendMessageInfo >::reverse_iterator rit = m_vecFriendMessageInfo.rbegin();
		KFriendMessageInfo& Message = *rit;

		switch((KFriendMessageInfo::MESSAGE_TYPE)Message.m_cMessageType)
		{
		case KFriendMessageInfo::MT_REQUEST_FRIEND:
			{
				CX2Community::FriendReqMsg* pNewFriendReqMsg = new CX2Community::FriendReqMsg();
				pNewFriendReqMsg->m_FromNickName = Message.m_wstrNickName;
				pNewFriendReqMsg->m_Msg = Message.m_wstrMessage;
				pNewFriendReqMsg->m_FromUnitUid = Message.m_iUnitUID;

				m_vecFriendMessageInfo.pop_back();
				m_pFriendReqMsg = pNewFriendReqMsg;
			} break;
		default:
			break;
		}
	}
#endif
}

bool CX2Community::ProcessFriendReqMsg( bool bConfirm )
{

	if ( m_pFriendReqMsg != NULL )
	{
#ifdef NEW_MESSENGER
		if ( bConfirm == true )
		{
			Handler_EGS_ACCEPT_FRIEND_REQ( m_pFriendReqMsg->m_FromUnitUid );			
		}
		else
		{
			Handler_EGS_DENY_FRIEND_REQ( m_pFriendReqMsg->m_FromUnitUid );
		}
#else NEW_MESSENGER
		if ( bConfirm == true )
		{
			if ( CNMCOClientObject::GetInstance().ConfirmNewFriend( m_pFriendReqMsg->m_uSerialNo, kConfirmOK ) == FALSE )
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_71 ), g_pMain->GetNowState() );
		}
		else
		{
			if ( CNMCOClientObject::GetInstance().ConfirmNewFriend( m_pFriendReqMsg->m_uSerialNo, kConfirmDenied ) == FALSE )
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_72 ), g_pMain->GetNowState() );
		}
#endif
	}

	SAFE_DELETE( m_pFriendReqMsg );

	return true;

}


#ifndef NEW_MESSENGER
void CX2Community::OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent )
{
	CX2Community::MessengerUser* pMessengerUser = GetMessengerUser( /*pEvent->fiFriendInfo.uIDCode*/CX2Community::ConvertOrgName( pEvent->fiFriendInfo.szNickName ).c_str() );
	if ( pMessengerUser != NULL )
	{
		SetRefresh();
		pMessengerUser->Update( pEvent->fiFriendInfo, m_bRefresh );

		// 유저 접속 서버에 알림
		Handler_EGS_UPDATE_FRIEND_INFO_REQ(pEvent->fiFriendInfo);		
	}

	if( true == m_bOpen )
		ResetUserListUI();
	

	//here coding after check keyOwner in virtual friend key

	//일단 임시 처리
//	ResetUserList();
}
#endif // #ifndef NEW_MESSENGER

#ifndef NEW_MESSENGER
// 친구 등록 신청
bool CX2Community::Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( const WCHAR* pNickName )
{

	KEGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ kPacket;
	kPacket.m_wstrUnitNickName = pNickName;

	if ( kPacket.m_wstrUnitNickName.find( L"__DELETED__") != -1 )
	{
		SAFE_DELETE_DIALOG( m_pDLGFriendReqWait );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_73 ), m_pStage );
		return true;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK );

	return true;

}

bool CX2Community::Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK ) == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGFriendReqWait );
#ifdef SERV_INTEGRATION
		//{{ oasis907 : 김상윤 [2010.5.19] // 대전 던전 서버군 통합
		if ( kEvent.m_iOK == NetError::ERR_KNM_02 )
		{
			wstring wstrServerName = L"";
#ifdef SERVER_GROUP_UI_ADVANCED // 2012.07.09 lygan_조성욱 // 서버군 확장 용의하게 추가
			wstrServerName = g_pInstanceData->GetServerGroupName();
#else //SERVER_GROUP_UI_ADVANCED
			switch(g_pInstanceData->GetServerGroupID())
			{
			case SGI_SOLES:
				wstrServerName = GET_STRING( STR_ID_4836 );
				break;
			case SGI_GAIA:
				wstrServerName = GET_STRING( STR_ID_4835 );
				break;
			default:
				wstrServerName = GET_STRING( STR_ID_5131 );
				break;
			}
#endif SERVER_GROUP_UI_ADVANCED			
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_5132, "L", wstrServerName ) ), g_pMain->GetNowState() );
			OpenAddFriendWindow( false );
			return true;
		}
		//}}
#endif SERV_INTEGRATION

		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			wstring addFriendMessage = GET_STRING( STR_ID_74 );
			if ( m_pDLGAddFriend != NULL )
			{
				CKTDGUIIMEEditBox* pIMEEditMessage = (CKTDGUIIMEEditBox*)m_pDLGAddFriend->GetControl( L"IMEMessage" );
				if ( pIMEEditMessage != NULL )
					addFriendMessage = pIMEEditMessage->GetText();
			}

			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

			if (  pUnit != NULL )
			{

				wstring tempNickName = ConvertFullName( kEvent.m_wstrUnitNickName.c_str() );
				if ( CNMCOClientObject::GetInstance().RequestNewFriend( tempNickName.c_str(), addFriendMessage.c_str(), 
					FALSE, NEXON_KOREA_ELSWORD_GAMECODE, NEXON_KOREA_ELSWORD_GAMECODE, 
					(_UInt32_)pUnit->GetUnitData().m_iNMKSerialNum, kEvent.m_uiKNMSerialNum) == FALSE )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_75 ), m_pStage );
				}
				else
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_76 ), m_pStage );
				}

				OpenAddFriendWindow( false );
			}
		}
		else
		{
			OpenAddFriendWindow( false );
		}
	}

	return false;
}



bool CX2Community::Handler_EGS_KNM_TOGETHER_REQ( wstring nickName )
{
	if ( g_pX2Game != NULL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_77 ), m_pStage );
		return true;
	}

	KEGS_KNM_TOGETHER_REQ kPacket;
	kPacket = nickName;

	g_pData->GetServerProtocol()->SendPacket( EGS_KNM_TOGETHER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_KNM_TOGETHER_ACK );

	return true;
}

bool CX2Community::Handler_EGS_KNM_TOGETHER_ACK( KEGS_KNM_TOGETHER_ACK& kEvent )
{
	if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		int iState = CX2Main::XS_INVALID; 

	
		CX2StateAutoChanger::TARGET_DETAIL targetDetail;
		targetDetail.m_iDungeonID = (int) kEvent.m_kUnitInfo.m_iDungeonID;
		targetDetail.m_iRoomUID	= kEvent.m_kUnitInfo.m_iRoomUID;

		g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), 
			iState,
			targetDetail,
			g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
	}

	return true;
}
#endif //#ifndef NEW_MESSENGER

void CX2Community::SetOpenFriendReqMsgBox( bool bOpen )
{
	if ( bOpen == true )
	{
		if ( m_pFriendReqMsg != NULL && m_pFriendReqMsg->m_pDialog != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pFriendReqMsg->m_pDialog, true );
			m_pFriendReqMsg->m_pDialog->SetShow( true );
			m_pFriendReqMsg->m_pDialog->SetEnable( true );
		}
	}
	else
	{
		if ( m_pFriendReqMsg != NULL && m_pFriendReqMsg->m_pDialog != NULL )
		{
			m_pFriendReqMsg->m_pDialog->SetShow( false );
			m_pFriendReqMsg->m_pDialog->SetEnable( false );
		}
	}
}


bool CX2Community::OpenCreateGroupWindow( bool bCheck )
{
	if ( bCheck == true )
	{	
		SAFE_DELETE_DIALOG( m_pDLGCreateGroup );
		m_pDLGCreateGroup = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Group_Make.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCreateGroup );
	}
	else
	{
		if ( m_pDLGCreateGroup != NULL )
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCreateGroup, NULL, false );
			m_pDLGCreateGroup = NULL;
		}
	}	

	return true;
}

bool CX2Community::CreateGroup()
{
	if ( m_pDLGCreateGroup == NULL )
		return true;

	CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)m_pDLGCreateGroup->GetControl( L"IMEEditBoxGroupName" );
	if ( pIMEEdit != NULL )
	{
		wstring newGroupName = pIMEEdit->GetText();

		if ( newGroupName.empty() == true || 
			true == IsOnlySpaceString( newGroupName ) ||
			FilteringGroupName( newGroupName.c_str() ) == false )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_78 ), m_pStage );
			return true;
		}

		
#ifdef NEW_MESSENGER
		return Handler_EGS_MAKE_FRIEND_GROUP_REQ( newGroupName );
#else NEW_MESSENGER
		NMVirtualKey nmVirtualKey;
		nmVirtualKey.uGameCode = NEXON_KOREA_ELSWORD_GAMECODE;
		nmVirtualKey.uVirtualIDCode = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iNMKSerialNum;
		if ( CNMCOClientObject::GetInstance().AddCategory( nmVirtualKey, newGroupName.c_str() ) == FALSE )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_79 ), m_pStage );
		}
#endif NEW_MESSENGER
	}

	return true;
}

bool CX2Community::ChangeGroupName()
{
	if ( m_pDLGChangeGroupName == NULL )
		return true;

	wstring oldGroupName;
	oldGroupName = GetSelectedGroupName();
	
	CX2Community::MessengerUserGroup* pMessengerUserGroup = GetMgrUserGroup( oldGroupName.c_str() );
	if ( pMessengerUserGroup != NULL )
	{
		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGChangeGroupName->GetControl( L"IMEEditBoxChangeGroupName" );
		if ( pIMEEditBox != NULL )
		{
			wstring newGroupName = pIMEEditBox->GetText();

			if ( newGroupName.empty() == true ||
				true == IsOnlySpaceString( newGroupName ) ||
				FilteringGroupName( newGroupName.c_str() ) == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_78 ), m_pStage );
				return true;
			}

			
#ifdef NEW_MESSENGER
			// 그룹이름 변경
			return Handler_EGS_RENAME_FRIEND_GROUP_REQ( pMessengerUserGroup->m_uCategoryCode, newGroupName );			
#else
			if ( CNMCOClientObject::GetInstance().ChangeCategoryName( pMessengerUserGroup->m_uCategoryCode, newGroupName.c_str() ) == FALSE )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_80 ), m_pStage );
			}
#endif
		}
	}

	return true;
}

bool CX2Community::OpenGroupMoveWindow( bool bOpen )
{
	if ( bOpen == true )
	{
		if ( m_pMessengerUserList->m_vecUserGroup.empty() == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_81 ), m_pStage );
			return true;
		}

		SAFE_DELETE_DIALOG( m_pDLGGroupMove );
		m_pDLGGroupMove = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Group_Move.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGroupMove );

		CKTDGUIComboBox* pCombo_Group_Name = (CKTDGUIComboBox*)m_pDLGGroupMove->GetControl( L"Combo_Group_Name" );
		for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
		{
			MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];
			if ( pMessengerUserGroup != NULL )
			{
				pCombo_Group_Name->AddItem( pMessengerUserGroup->m_GroupName.c_str(), NULL );
			}
		}
	}
	else
	{
		if ( m_pDLGGroupMove != NULL )
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGGroupMove, NULL, false );
			m_pDLGGroupMove = NULL;
		}
	}

	return true;
}

bool CX2Community::OpenGroupNameChangeWindow( bool bOpen )
{
	if ( bOpen == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGChangeGroupName );
		m_pDLGChangeGroupName = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Group_Name_Change.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChangeGroupName );
	}
	else
	{
		if ( m_pDLGChangeGroupName != NULL )
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGChangeGroupName, NULL, false );
			m_pDLGChangeGroupName = NULL;
		}
	}

	return true;
}

void CX2Community::MoveFriendGroup()
{
	if ( m_pDLGGroupMove == NULL )
		return;

	CKTDGUIComboBox* pCombo_Group_Name = (CKTDGUIComboBox*)m_pDLGGroupMove->GetControl( L"Combo_Group_Name" );
	if ( pCombo_Group_Name == NULL )
		return;

	CKTDGUIComboBox::ComboBoxItem* pComboBoxItem = pCombo_Group_Name->GetSelectedItem();
	if ( pComboBoxItem == NULL )
		return;

	CX2Community::MessengerUser* pMessengerUser = GetMessengerUser( GetSelectedUnitName().c_str() );
	if ( pMessengerUser == NULL )
		return;

	CX2Community::MessengerUserGroup* pMGRUserGroup = GetMgrUserGroup( pComboBoxItem->strText );
	if ( pMGRUserGroup == NULL )
		return;

#ifdef NEW_MESSENGER
	// 친구 이동
	if( pMessengerUser->m_uCategoryCode == pMGRUserGroup->m_uCategoryCode )
	{
		//** g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( 같은 그룹이라 안됨 ㅋ ), m_pStage );
	}
	else
	{
		Handler_EGS_MOVE_FRIEND_REQ( pMessengerUser->m_UnitUID, pMGRUserGroup->m_uCategoryCode );
	}	

#else NEW_MESSENGER
	if ( pMessengerUser->m_uCategoryCode == 0 )
	{
		if ( CNMCOClientObject::GetInstance().AddFriendToCategory( pMessengerUser->m_NMFriendKey, 
																pMGRUserGroup->m_uCategoryCode ) == FALSE )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_82 ), m_pStage );

		}
	}
	else
	{
		if ( CNMCOClientObject::GetInstance().MoveFriendCategory( pMessengerUser->m_NMFriendKey, 
															pMessengerUser->m_uCategoryCode, 
															pMGRUserGroup->m_uCategoryCode ) == FALSE )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_83 ), m_pStage );
		}
	}
#endif NEW_MESSENGER
}

bool CX2Community::SelectUserListItem( CKTDGUIListBox::LIST_BOX_EVENT_MSG listBoxEventMsg )
{
	if ( m_MessengerUserTab == XMUT_FRIEND )
	{
		CKTDGUIListBox* pListBoxUserList = GetListBox();
		if ( pListBoxUserList != NULL )
		{
			CKTDGUIListBox::ListBoxItem* pListBoxItem = pListBoxUserList->GetSelectedItem();
			if ( pListBoxItem != NULL )
			{
				if ( listBoxEventMsg == CKTDGUIListBox::LBEM_SELECTION )
				{
					if ( pListBoxItem->itemType == CKTDGUIListBox::LBIT_GROUP_PLUS )
					{
						wstring groupName = GetGroupName( pListBoxItem->strText );
						m_setOpenedGroupName.insert( groupName.c_str() );
						
						if ( m_MessengerUserTab == XMUT_FRIEND )
							m_CurrentTrackPos = pListBoxUserList->GetScrollBar()->GetTrackPos();

						ResetUserListUI(); //UI만 바꿔도 될꺼 같은데?
					}
					else if ( pListBoxItem->itemType == CKTDGUIListBox::LBIT_GROUP_MINUS )
					{
						wstring groupName = GetGroupName( pListBoxItem->strText );
						set< wstring >::iterator sit;
						sit = m_setOpenedGroupName.find( groupName.c_str() );

						if ( sit != m_setOpenedGroupName.end() )
							m_setOpenedGroupName.erase( sit );
						
						if ( m_MessengerUserTab == XMUT_FRIEND )
							m_CurrentTrackPos = pListBoxUserList->GetScrollBar()->GetTrackPos();

						ResetUserListUI(); //UI만 바꿔도 될꺼 같은데?
					}
				}
				else if ( listBoxEventMsg == CKTDGUIListBox::LBEM_ITEM_DBLCLK )
				{
					//채팅 신청 기능 쑤셔 넣어야함, 물론 대화 가능한 유저한테만, 그룹 노노
					if ( pListBoxItem->itemType == CKTDGUIListBox::LBIT_DEFAULT )
					{
						CX2Community::MessengerUser* pMessengerUser = GetMessengerUser( GetSelectedUnitName().c_str() );
#ifdef NEW_MESSENGER
						if( pMessengerUser != NULL )
						{
							switch(pMessengerUser->m_eStatus)
							{
							case MessengerUser::FS_NORMAL:
								{
									return TryTalk( pMessengerUser );
								} break;
							default:
								{
									return true;
								} break;
							}

						}
#else NEW_MESSENGER
						return TryTalk( pMessengerUser );		
#endif NEW_MESSENGER
					}
				}
				else if ( listBoxEventMsg == CKTDGUIListBox::LBEM_SELECTION_BY_RBUTTON )
				{
					if ( pListBoxItem->itemType == CKTDGUIListBox::LBIT_DEFAULT || 
						pListBoxItem->itemType == CKTDGUIListBox::LBIT_NOT_ENABLE)
					{
						if(StrCmpW(pListBoxItem->strText, GET_STRING( STR_ID_84 )) != 0)
							return OpenUserMenu();
					}
					else if ( pListBoxItem->itemType == CKTDGUIListBox::LBIT_GROUP_PLUS || 
						pListBoxItem->itemType == CKTDGUIListBox::LBIT_GROUP_MINUS )
					{
						return OpenGroupMenu();
					}
				}
			}
		}
	}

	return true;
}

int CX2Community::GetSelectedUnitLevel()
{
	int iLevel = 0;

	switch(m_MessengerUserTab)
	{
	case XMUT_FIELD:		
	case XMUT_DISCIPLE:
#ifdef SERV_RECRUIT_EVENT_BASE
	case XMUT_RECRUIT:
#endif SERV_RECRUIT_EVENT_BASE
		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	case XMUT_REFEREE:
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

		if(m_iPickedUserIndex > 0)
			iLevel = m_vecUserLv[m_iPickedUserIndex-1];
		break;

	default:
		break;
	}

	return iLevel;
}

wstring CX2Community::GetSelectedUnitName()
{
	wstring nickName = L"";
	if ( m_pDlgBack == NULL )
		return nickName;

	switch(m_MessengerUserTab)
	{
	case XMUT_FIELD:
		{
			WCHAR wszText[64] = L"";
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"name%d", m_iPickedUserIndex );
			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*) m_pDlgField->GetControl( wszText );
			if(pStaticName != NULL)
				nickName = pStaticName->GetString(0)->msg;
		}
		break;
	case XMUT_FRIEND:
		{
			CKTDGUIListBox* pListBox = GetListBox();
			if ( pListBox != NULL )
			{
				CKTDGUIListBox::ListBoxItem* pListBoxItem = pListBox->GetSelectedItem();
				if ( pListBoxItem != NULL )
				{
					wstring tempNickName = pListBoxItem->strText;
					int index = tempNickName.find( L" " );
					if ( index == -1 )
						index = tempNickName.size();

					nickName = tempNickName.substr( 0, index );
				}
			}
		}
		break;
	case XMUT_GUILD:
		{
#ifdef GUILD_MANAGEMENT
			std::wstring wstrGuildMemberNickName = GetGuildMemberNickName( GetSelectedGuildMemberUID() );

			if ( wstrGuildMemberNickName.empty() == false )
			{
				nickName = wstrGuildMemberNickName;
			}			
#endif	//	GUILD_MANAGEMENT
		}
		break;
	case XMUT_DISCIPLE:
		{
			WCHAR wszText[64] = L"";
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"name%d", m_iPickedUserIndex );
			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*) m_pDlgDisciple->GetControl( wszText );
			if(pStaticName != NULL)
				nickName = pStaticName->GetString(0)->msg;
		}
		break;

#ifdef SERV_RECRUIT_EVENT_BASE
	case XMUT_RECRUIT:
		{
			if ( m_pDlgRecruit != NULL )
			{
				WCHAR wszText[64] = L"";
				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"name%d", m_iPickedUserIndex );
				CKTDGUIStatic* pStaticName = (CKTDGUIStatic*) m_pDlgRecruit->GetControl( wszText );
				if(pStaticName != NULL)
					nickName = pStaticName->GetString(0)->msg;
			}
		} break;
#endif SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	case XMUT_REFEREE:
		{
			if ( m_pDlgReferee != NULL )
			{
				WCHAR wszText[64] = L"";
				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"name%d", m_iPickedUserIndex );
				CKTDGUIStatic* pStaticName = (CKTDGUIStatic*) m_pDlgReferee->GetControl( wszText );
				if(pStaticName != NULL)
					nickName = pStaticName->GetString(0)->msg;
			}
		}
		break;
#endif	VIEW_REFEREE_LIST

#ifdef ADDED_RELATIONSHIP_SYSTEM

	case XMUT_RELATION :
		{
			if ( m_pDlgRelation != NULL )
			{
				if ( NULL != g_pData->GetRelationshipManager() &&
					 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{					
					return g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname;
				}
			}
		}
		break;

#endif // ADDED_RELATIONSHIP_SYSTEM

		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
	}

	return nickName;
}




bool CX2Community::ProcessUserMenuCommand( const WCHAR* pCommandName )
{
	wstring nickName = GetSelectedUnitName();
	
	if ( nickName.empty() == true )
	{
		return true;
	}

	CX2Community::MessengerUser* pMessengerUser = GetMessengerUser( nickName.c_str() );
	if ( pMessengerUser == NULL )
		return true;

	if ( wcscmp( pCommandName, L"Invite" ) == 0 )
	{
		wstring frinedName = ConvertOrgName( GetSelectedUnitName().c_str() );
		Handler_EGS_KNM_INVITE_REQ( frinedName );
	}
#ifndef NEW_MESSENGER
	else if ( wcscmp( pCommandName, L"Together" ) == 0 )
	{
		wstring frinedName = ConvertOrgName( GetSelectedUnitName().c_str() );
		Handler_EGS_KNM_TOGETHER_REQ( frinedName );
	}
#endif //#ifndef NEW_MESSENGER
	else if ( wcscmp( pCommandName, L"Talk" ) == 0 )
	{
		TryTalk( pMessengerUser );
	}
	else if ( wcscmp( pCommandName, L"Blocking" ) == 0 )
	{
#ifdef NEW_MESSENGER
		// 차단요청
		if( pMessengerUser->IsBlockable() )
		{
			if( false == pMessengerUser->IsBlocked() )
			{
				Handler_EGS_BLOCK_FRIEND_REQ( pMessengerUser->m_UnitUID );
			}
			else
			{
				Handler_EGS_UNBLOCK_FRIEND_REQ( pMessengerUser->m_UnitUID );
			}
		}
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_3718 ), m_pStage );
		}
		
#else
		if ( CNMCOClientObject::GetInstance().BlockFriend( pMessengerUser->m_NMFriendKey, !pMessengerUser->m_bBlocked ) == FALSE )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_85 ), m_pStage );
		}
#endif
		
	}
	else if ( wcscmp( pCommandName, L"Delete" ) == 0 )
	{
		CX2Community::MessengerUser* pMessengerUser = GetMessengerUser( nickName.c_str() );
		//그룹이 없는 녀석일 경우 카테고리 코드를 0으로 보내고 있는데 확실한가..?
#ifdef NEW_MESSENGER
		// 삭제요청
		Handler_EGS_DELETE_FRIEND_REQ( pMessengerUser->m_UnitUID );
#else
		if ( CNMCOClientObject::GetInstance().DeleteFriendFromCategory( pMessengerUser->m_NMFriendKey, pMessengerUser->m_uCategoryCode ) == FALSE )
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_86 ), m_pStage );
#endif
	}
	else if ( wcscmp( pCommandName, L"CharInfo" ) == 0 )
	{
		// fix!! state menu를 상속받은 state에서만 하거나, Handler_EGS_SEARCH_UNIT_REQ 함수를 x2state로 옮겨야한다. 일단 게임중에는 안되게
		if( NULL != g_pX2Game )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_87 ), m_pStage );
		}
		else
		{
			
#ifdef CHECK_INHERIT_STATE_MENU
			if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
			{// 스테이트 메뉴에 있는 걸 그대로 사용한다.
				CX2StateMenu* pStateMenu = (CX2StateMenu*)m_pStage;
				if ( pStateMenu != NULL )
					pStateMenu->Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( ConvertOrgName( GetSelectedUnitName().c_str() ).c_str() );
			}
		}
	}
	else if ( wcscmp( pCommandName, L"GroupMove" ) == 0 )
	{
		OpenGroupMoveWindow( true );
	}
	else if ( wcscmp( pCommandName, L"Party" ) == 0 )
	{
		// fix!!
		//파티 신청
		//if ( g_pPartyMsgerUI != NULL )
		//{
		//	//if ( pMessengerUser->m_bLogin == TRUE )
		//	//	g_pPartyMsgerUI->Handler_EGS_INVITE_PARTY_REQ( pMessengerUser->m_UnitUID, pMessengerUser->m_NickName, false );
		//	//else
		//	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"게임접속중인 친구에게만 가능합니다.", g_pMain->GetNowState() );
		//}
	}

	if ( m_pDLGUserMenu != NULL )
	{
		m_pDLGUserMenu->SetShow( false );
		m_pDLGUserMenu->SetEnable( false );
	}

	return true;
}

bool CX2Community::ProcessGroupMenuCommand( const WCHAR* pCommandName )
{
	wstring groupName = GetSelectedGroupName();

	if ( groupName.empty() == true )
	{
		return true;
	}

	CX2Community::MessengerUserGroup* pMgrUserGroup = GetMgrUserGroup( groupName.c_str() );
	if ( pMgrUserGroup == NULL )
		return true;

	if ( wcscmp( pCommandName, L"NameChange" ) == 0 )
	{
		//CNMCOClientObject::GetInstance().ChangeCategoryName( pMgrUserGroup->m_uCategoryCode,  )
		//바꿀 이름을 등록할 UI만들기
		OpenGroupNameChangeWindow( true );
	}
	else if ( wcscmp( pCommandName, L"Delete" ) == 0 )
	{
		//DeleteCategory()
		//카데고리 삭제
#ifdef NEW_MESSENGER
		Handler_EGS_DELETE_FRIEND_GROUP_REQ( pMgrUserGroup->m_uCategoryCode );
#else NEW_MESSENGER
		if ( CNMCOClientObject::GetInstance().DeleteCategory( pMgrUserGroup->m_uCategoryCode ) == FALSE )
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_88 ), m_pStage );
#endif
	}

	return true;
}


bool CX2Community::SetOpenChatSession( CKTDGUIDialogType pDialog, bool bOpen )
{
	if ( pDialog == NULL )
		return true;

	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		if ( pChatSession != NULL )
		{
			if ( pChatSession->m_pDialog == pDialog )
			{				
				if ( bOpen == true )
				{
					if(pChatSession->m_bMaxWindow == true)
						m_pOpendChatSession = pChatSession;					

					//SetMode( CX2Community::XMM_CHAT_MODE );					
					//pChatSession->m_bAlarmReceiveMessage = false;						
				}
				else
				{
					SetChatHistory(pChatSession);
					SetChatBoxShow(pChatSession, false);

					if ( m_pOpendChatSession == pChatSession )
						m_pOpendChatSession = NULL;

					SAFE_DELETE( pChatSession );
					m_vecChatSession.erase( m_vecChatSession.begin() + i );
					i--;
					//세션 종료시킨다.
					//SetMode( CX2Community::XMM_USER_LIST_MODE );					
				}

				return true;
			}
		}
	}

	return true;
}



void CX2Community::ClearChatSessionList()
{
	//if ( m_pOpendChatSession != NULL )
	//	SetOpenChatSession( m_pOpendChatSession->m_pDialog, false );

	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];

		//if(pChatSession != NULL)
		//	SetOpenChatSession( pChatSession->m_pDialog, false );

		if ( m_pOpendChatSession == pChatSession )
			m_pOpendChatSession = NULL;
		SAFE_DELETE( pChatSession );
	}
	m_vecChatSession.clear();
}


#ifdef NEW_MESSENGER
bool CX2Community::SendChatMessage( const wstring& ChatMsg )
{
	if ( ChatMsg.empty() == true || m_pOpendChatSession == NULL || 
		m_pOpendChatSession->m_UnitUid == -1 )
	{
		return true;
	}

	wstring tempChatMsg = ChatMsg;
	if ( tempChatMsg.empty() == true )
		return true;

	if(g_pChatBox != NULL )
	{
		g_pChatBox->Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_MESSENGER, CX2Room::RCT_WHISPER, 
			m_pOpendChatSession->m_UnitUid, tempChatMsg, m_pOpendChatSession->m_wstrTargetNickName.c_str() );
	}
	else
	{
		return false;
	}
	return true;	
}

bool CX2Community::ReceiveChatMessage( UidType FromUnitUid, const wstring& ChatMsg, const wstring& wstrFromUnitName )
{
	
	// TODO : 메시지 화면에 노출.
	// 	보낸 사람 : wstrFromUnitName
	// 메시지 : pChatMsg

	CX2Community::ChatSession* pChatSession = FindChatSession( FromUnitUid );
	if ( pChatSession == NULL )
	{
		// 새로 하나 만든다
		SetOpenChatSession( FromUnitUid, wstrFromUnitName );
		pChatSession = FindChatSession( FromUnitUid );		
		if(pChatSession == NULL)
		{
			ASSERT( !"Create Session Failed!" );
			return false;
		}
	}
	else
	{
		if ( m_pOpendChatSession != pChatSession )
		{
			int sessionIndex = GetChatSessionIndex( FromUnitUid );
			m_NowPageNum = (sessionIndex / m_MaxChatSessionByPage) + 1;
		}
	}	

	wstring tempChatMsg;
	tempChatMsg = wstrFromUnitName;
	tempChatMsg += L":";
	AddChatMsg( pChatSession, tempChatMsg.c_str(), true, L"#C990000" );

	tempChatMsg = ChatMsg;
	AddChatMsg( pChatSession, tempChatMsg.c_str() );

	CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
	if ( pIMEEdit != NULL )
	{
		if( pIMEEdit->GetHaveFocusIn() == false )
			pChatSession->m_bAlarmReceiveMessage = true;
	}

	//if ( m_pOpendChatSession != pChatSession || pChatSession->m_bMaxWindow == false)
	//{
	//	pChatSession->m_bAlarmReceiveMessage = true;
	//}

	//if ( m_bOpen == true )
	if( pChatSession->m_pDialog->GetShow() == true )
	{
		SetChatBoxShow(pChatSession, true);
		//pChatSession->m_pDialog->SetShow( true );
		//pChatSession->m_pDialog->SetEnable( true );
	}

	//if ( m_bOpen == false )
	if(pChatSession->m_bShowWindow == false || pChatSession->m_bHideWindow == true || pChatSession->m_bMaxWindow == false)
	{
		if ( CheckSendMsgByChatbox() == true )
		{
			wstring wstrSenderName = wstrFromUnitName;
			if( NULL != g_pChatBox )
			{
				// 자신의 캐릭터가 보낸 메시지면 동작안함
				wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
				//wstrNickName += GET_STRING( STR_ID_67 );
				if( wstrSenderName.compare( wstrNickName ) != 0 )
				{
					g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_93, "L", wstrSenderName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				}					
			}
		}
		else
		{
			m_MessageSenderNickName = wstrFromUnitName;
			m_bReceiveMessage = true;
		}
	}
	return true;

}

bool CX2Community::SetOpenChatSession( UidType FromUnitUid, const wstring& wstrFromUnitName )
{

	CX2Community::ChatSession* pChatSession = NULL;
	pChatSession = FindChatSession( FromUnitUid );

	X2_DIALOG_LAYER iLayer = XDL_MESSENGER;
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_PVP_GAME:
		{
			iLayer = XDL_POP_UP;
		} break;		
	default:
		iLayer = XDL_MESSENGER;
		break;
	}

	// TODO1 : 세션의 Uid로 이미 생성된 세션인지 확인.	    
	if ( pChatSession != NULL )
	{
		// 이미 생성되어 있는 세션 인스턴스 사용

		wstring wstrNickName			= g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
		wstring wstrOppositeNickName	= pChatSession->m_wstrTargetNickName;
		if(FindChatHistory(wstrNickName, wstrOppositeNickName) == -1)
		{
			CreateChatHistory(wstrNickName, wstrOppositeNickName);
		}
		else
		{
			GetChatHistory(pChatSession);
		}

		pChatSession->m_pDialog->SetLayer(iLayer);

	}
	else
	{
		// 새로 만든다

		pChatSession = new CX2Community::ChatSession();
		pChatSession->m_UnitUid = FromUnitUid;
		pChatSession->m_wstrTargetNickName = wstrFromUnitName;

		pChatSession->m_pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_Chat_Window.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pChatSession->m_pDialog );
		pChatSession->m_pDialog->SetPos(D3DXVECTOR2(400.f, 100.f));		
		pChatSession->m_pDialog->SetDisableUnderWindow(true);
		
		pChatSession->m_pDialog->SetLayer(iLayer);

		wstring wstrNickName			= g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
		wstring wstrOppositeNickName	= wstrFromUnitName;
		if(FindChatHistory(wstrNickName, wstrOppositeNickName) == -1)
		{
			CreateChatHistory(wstrNickName, wstrOppositeNickName);
		}
		else
		{
			GetChatHistory(pChatSession);
		}


		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl( L"StaticMessenger_NickName" );			
		if ( pStatic != NULL && pStatic->GetString(0) != NULL )
			pStatic->GetString(0)->msg = wstrFromUnitName;

		CKTDGUIStatic* pStaticMini = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl( L"Mini_ID" );
		if ( pStaticMini != NULL && pStaticMini->GetString(0) != NULL )
			pStaticMini->GetString(0)->msg = wstrFromUnitName;

		m_vecChatSession.push_back( pChatSession );

		m_NowPageNum = (((int)m_vecChatSession.size() - 1) / m_MaxChatSessionByPage ) + 1;

	}

	if ( m_bOpen == false )
	{
		SetChatBoxShow(pChatSession, false);
	}
	return true;
}

#else NEW_MESSENGER
//{{AFX

bool CX2Community::SendChatMessage( const WCHAR* pChatMsg )
{
	if ( pChatMsg == NULL || m_pOpendChatSession == NULL || 
		m_pOpendChatSession->m_pCNMCSChatSession == NULL )
	{
		return true;
	}

	wstring tempChatMsg = pChatMsg;
	if ( tempChatMsg.empty() == true )
		return true;

	CNMChatMessage	chatMsg;

	::_tcscpy( chatMsg.szSenderName, m_pOpendChatSession->m_pCNMCSChatSession->GetMyNickName( TRUE ) ); 
	::_tcsncpy( chatMsg.szChatMessage, pChatMsg, CHATMSG_SIZE - 1 );

	if ( m_pOpendChatSession->m_pCNMCSChatSession->SendChatMessage( chatMsg ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_89, "S", chatMsg.szChatMessage ) ), m_pStage );
	}

	return true;
}

void CX2Community::OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent )
{
	if( kSessionType_Chat == pEvent->siInfo.uType )
	{
		CX2Community::ChatSession* pChatSession = NULL;
		pChatSession = FindChatSession( pEvent->uSerialKey );

		// TODO1 : 세션의 시리얼 키로 이미 생성된 세션인지 확인.	    
		if ( pChatSession != NULL )
		{
			// 이미 생성되어 있는 세션 인스턴스 사용
			/*
			m_pOpendChatSession = pChatSession;
			SetOpenedChatSessionHistory();
			*/
			wstring wstrNickName			= pChatSession->m_pCNMCSChatSession->GetMyNickName();
			wstring wstrOppositeNickName	= pChatSession->m_pCNMCSChatSession->GetOppositeNickName();
			if(FindChatHistory(wstrNickName, wstrOppositeNickName) == -1)
			{
				CreateChatHistory(wstrNickName, wstrOppositeNickName);
			}
			else
			{
				GetChatHistory(pChatSession);
			}
		}
		else
		{
			pChatSession = new CX2Community::ChatSession();
			pChatSession->m_pCNMCSChatSession = new CNMCSChatSession();
			pChatSession->m_pCNMCSChatSession->SetSerialKey( pEvent->uSerialKey );
			pChatSession->m_pCNMCSChatSession->GetSessionInfo();

			pChatSession->m_pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_Chat_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pChatSession->m_pDialog );
			pChatSession->m_pDialog->SetPos(D3DXVECTOR2(400.f, 100.f));		
			pChatSession->m_pDialog->SetDisableUnderWindow(true);
			            
			wstring wstrNickName			= pChatSession->m_pCNMCSChatSession->GetMyNickName();
			wstring wstrOppositeNickName	= pChatSession->m_pCNMCSChatSession->GetOppositeNickName();
			if(FindChatHistory(wstrNickName, wstrOppositeNickName) == -1)
			{
				CreateChatHistory(wstrNickName, wstrOppositeNickName);
			}
			else
			{
				GetChatHistory(pChatSession);
			}


			CKTDGUIStatic* pStatic = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl( L"StaticMessenger_NickName" );			
			if ( pStatic != NULL && pStatic->GetString(0) != NULL )
				pStatic->GetString(0)->msg = CX2Community::ConvertOrgName( pChatSession->m_pCNMCSChatSession->GetOppositeNickName() );

			CKTDGUIStatic* pStaticMini = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl( L"Mini_ID" );
			if ( pStaticMini != NULL && pStaticMini->GetString(0) != NULL )
				pStaticMini->GetString(0)->msg = CX2Community::ConvertOrgName( pChatSession->m_pCNMCSChatSession->GetOppositeNickName() );

			if ( !pChatSession->m_pCNMCSChatSession->RegisterCallback( g_pKTDXApp->GetHWND(), XGM_NEXON_CALLBACK ) )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"NMChatSession.RegisterCallback failed", m_pStage );
				SAFE_DELETE( pChatSession );
				return;
			}

			m_vecChatSession.push_back( pChatSession );
			
			m_NowPageNum = (((int)m_vecChatSession.size() - 1) / m_MaxChatSessionByPage ) + 1;
			
			// TODO2 : 새로운 세션 인스턴스 생성(pEvent->uSerialKey 사용)
		}

		if ( m_bOpen == false )
		{
			//pChatSession->m_pDialog->SetShow( false );
			//pChatSession->m_pDialog->SetEnable( false );
			SetChatBoxShow(pChatSession, false);
		}

		if ( m_bRequestChat == true && 
			m_RequestedNMFriendKey.keyVirtual == pChatSession->m_pCNMCSChatSession->GetOppositeMemberInfo().keyVirtual )
		{
			m_bRequestChat = false;
			m_pOpendChatSession = pChatSession;
			//SetMode( XMM_CHAT_MODE );
		}

	}

}

void CX2Community::OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent )
{
	RefreshChatInfo( pEvent->uSerialKey );
}

void CX2Community::OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent )
{
	RefreshChatInfo( pEvent->uSerialKey );

	

	CX2Community::ChatSession* pChatSession = FindChatSession( pEvent->uSerialKey );
	if ( pChatSession != NULL )
	{
		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"Session Established Success", m_pStage );
		pChatSession->m_bSettingComplete = true;

		wstring wstrNickName = pChatSession->m_pCNMCSChatSession->GetMyNickName();
		wstring wstrOppositeNickName = pChatSession->m_pCNMCSChatSession->GetOppositeNickName();
		if(FindChatHistory(wstrNickName, wstrOppositeNickName) >= 0)
		{
			GetChatHistory(pChatSession);
		}

		AddChatMsg( pChatSession, GET_STRING( STR_ID_90 ) );
		AddChatMsg( pChatSession, L" " );
		m_pOpendChatSession = pChatSession;
		//SetOpenedChatSessionHistory();

		//pChatSession->m_vecTalk.push_back( L"상대방과 연결 되었습니다." );
		//if ( pChatSession == m_pOpendChatSession )
		//{
		//	CKTDGUIListBox* pListBox = GetListBox();
		//	pListBox->AddItem( L"상대방과 연결 되었습니다.", NULL );
		//}
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_91 ), m_pStage );
	}
}

void CX2Community::OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent )
{
	if ( _tcsicmp( pEvent->cmMessage.szSenderName, _T( "_SYSTEM_" ) ) != 0 
		&& _tcsicmp( pEvent->cmMessage.szSenderName, _T( "_NOTICE_" ) ) != 0 )
	{
		// TODO : 메시지 화면에 노출.
		// 	보낸 사람 : pEvent->cmMessage.szSenderName
		// 메시지 : pEvent->cmMessage.szChatMessage
		
		CX2Community::ChatSession* pChatSession = FindChatSession( pEvent->GetSerialKey() );
		if ( pChatSession == NULL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_REPLACED_STRING( ( STR_ID_92, "SS", pEvent->cmMessage.szSenderName, pEvent->cmMessage.szChatMessage ) ), m_pStage );
			
			return;
		}
		else
		{
			if ( m_pOpendChatSession != pChatSession )
			{
				int sessionIndex = GetChatSessionIndex( pEvent->GetSerialKey() );
				m_NowPageNum = (sessionIndex / m_MaxChatSessionByPage) + 1;
			}
		}	

		wstring tempChatMsg;
		tempChatMsg = CX2Community::ConvertOrgName( pEvent->cmMessage.szSenderName );
		tempChatMsg += L":";
		AddChatMsg( pChatSession, tempChatMsg.c_str(), true, L"#C990000" );

		tempChatMsg = pEvent->cmMessage.szChatMessage;
		AddChatMsg( pChatSession, tempChatMsg.c_str() );

		CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
		if ( pIMEEdit != NULL )
		{
			if( pIMEEdit->GetHaveFocusIn() == false )
				pChatSession->m_bAlarmReceiveMessage = true;
		}

		//if ( m_pOpendChatSession != pChatSession || pChatSession->m_bMaxWindow == false)
		//{
		//	pChatSession->m_bAlarmReceiveMessage = true;
		//}

		//if ( m_bOpen == true )
		if( pChatSession->m_pDialog->GetShow() == true )
		{
			SetChatBoxShow(pChatSession, true);
			//pChatSession->m_pDialog->SetShow( true );
			//pChatSession->m_pDialog->SetEnable( true );
		}

		//if ( m_bOpen == false )
		if(pChatSession->m_bShowWindow == false || pChatSession->m_bHideWindow == true || pChatSession->m_bMaxWindow == false)
		{
			if ( CheckSendMsgByChatbox() == true )
			{
				wstring wstrSenderName = CX2Community::ConvertOrgName( pEvent->cmMessage.szSenderName );
				if( NULL != g_pChatBox )
				{
					// 자신의 캐릭터가 보낸 메시지면 동작안함
					wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
					wstrNickName += GET_STRING( STR_ID_67 );
					if( wstrSenderName.compare( wstrNickName ) != 0 )
					{
						g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_93, "L", wstrSenderName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
					}					
				}
			}
			else
			{
				m_MessageSenderNickName = CX2Community::ConvertOrgName( pEvent->cmMessage.szSenderName );
				m_bReceiveMessage = true;
			}
		}
	}
	else
	{
		// 시스템 메시지 & 알림 메시지
		if ( _tcslen( pEvent->cmMessage.szChatMessage ) > 0 )
		{
			// TODO : 메시지 화면에 노출
			// 알림 메시지는 실제 메시지 내용이 없이 다른 용도로 전송되는 경우가 있으므로,
			// 메시지의 길이가 0 이상일 때만 내용을 출력한다.


			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), pEvent->cmMessage.szChatMessage, g_pMain->GetNowState() );

		}
	}
}

void CX2Community::OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent )
{
	//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"OnGSSessionClosedEvent", m_pStage );
	DisconnectChatSession( pEvent->uSerialKey );
}

void CX2Community::OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent )
{
	//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"OnGSSessionFailedEvent", m_pStage );
	DisconnectChatSession( pEvent->uSerialKey );
}

//}}AFX
#endif NEW_MESSENGER


void CX2Community::GetFriendNameList( vector< wstring >& vecFriendNameList )
{
	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecDefaultGroupUser.size(); i++ )
	{
		CX2Community::MessengerUser* pMessenger = m_pMessengerUserList->m_vecDefaultGroupUser[i];
		if ( pMessenger != NULL )
		{
			vecFriendNameList.push_back( pMessenger->m_NickName );
		}
	}

	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];
		if ( pMessengerUserGroup != NULL )
		{
			for ( int j = 0; j < (int)pMessengerUserGroup->m_vecUser.size(); j++ )
			{
				CX2Community::MessengerUser* pMessenger = pMessengerUserGroup->m_vecUser[j];
				if ( pMessenger != NULL )
				{
					vecFriendNameList.push_back( pMessenger->m_NickName );
				}
			}
		}
	}
}

#ifdef FIXED_RELATIONSHIP_SYSTEM

void CX2Community::GetFriendNameListAndUID ( map<wstring, UidType> & mapFriendInfoList )
{
	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecDefaultGroupUser.size(); i++ )
	{
		CX2Community::MessengerUser* pMessenger = m_pMessengerUserList->m_vecDefaultGroupUser[i];
		if ( pMessenger != NULL )
		{
			mapFriendInfoList.insert ( make_pair( pMessenger->m_NickName, pMessenger->m_UnitUID ) );
		}
	}

	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];
		if ( pMessengerUserGroup != NULL )
		{
			for ( int j = 0; j < (int)pMessengerUserGroup->m_vecUser.size(); j++ )
			{
				CX2Community::MessengerUser* pMessenger = pMessengerUserGroup->m_vecUser[j];
				if ( pMessenger != NULL )
				{
					mapFriendInfoList.insert( make_pair ( pMessenger->m_NickName, pMessenger->m_UnitUID ) );
				}
			}
		}
	}
}

#endif // FIXED_RELATIONSHIP_SYSTEM

wstring CX2Community::ConvertOrgName( const WCHAR* pFullName )
{
	wstring tempName;

	tempName = pFullName;

	int index = tempName.find( L"]" );
	index++;
	wstring orgName = tempName.substr( index, tempName.size() );

	return orgName;
}

wstring CX2Community::ConvertFullName( const WCHAR* pNickName )
{
	wstring tempName;

	//수정해야함 나중에 서버군 이름 넣어줘야하는 식으로.. 아직은 솔레스 밖에 없으므로 일단 요로코롬 작업

#ifdef EXTEND_SERVER_GROUP_MASK
	tempName = pNickName;
#else
#ifdef _SERVICE_
	#ifdef _OPEN_TEST_
        tempName = GET_REPLACED_STRING( ( STR_ID_94, "S", pNickName ) );
	#else
		#ifdef ADD_SERVER_GROUP
				if(g_pInstanceData != NULL)
				{
					SERVER_GROUP_ID iServer = g_pInstanceData->GetServerGroupID();
					switch(iServer)
					{
					case SGI_SOLES:
						tempName = GET_REPLACED_STRING( ( STR_ID_95, "S", pNickName ) );
						break;
					case SGI_GAIA:
						tempName = GET_REPLACED_STRING( ( STR_ID_4827, "S", pNickName ) );
						break;
					default:
						tempName = pNickName;
						break;
					}
				}	
		#else
				tempName = GET_REPLACED_STRING( ( STR_ID_95, "S", pNickName ) );
		#endif
	#endif
	
#else
	#ifdef _USE_NEXON_MSG_INHOUSE
		tempName = GET_REPLACED_STRING( ( STR_ID_95, "S", pNickName ) );
	#else
		#ifdef ADD_SERVER_GROUP
			if(g_pInstanceData != NULL)
			{
				SERVER_GROUP_ID iServer = g_pInstanceData->GetServerGroupID();
				switch(iServer)
				{
				case SGI_SOLES:
					tempName = GET_REPLACED_STRING( ( STR_ID_95, "S", pNickName ) );
					break;
				case SGI_GAIA:
					tempName = GET_REPLACED_STRING( ( STR_ID_4827, "S", pNickName ) );
					break;
				default:
					tempName = pNickName;
					break;
				}
			}	
		#else
				tempName = GET_REPLACED_STRING( ( STR_ID_96, "S", pNickName ) );
		#endif
	#endif	
#endif
#endif // EXTEND_SERVER_GROUP_MASK
	return tempName;
}

void CX2Community::InitUI()
{
	D3DXVECTOR2 vPos = D3DXVECTOR2(710.f, 140.f);
	m_pDlgBack = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Back.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgBack );
	m_pDlgBack->SetPos(vPos);
	m_pDlgBack->SetShowEnable(true, true);
	m_pDlgBack->SetDisableUnderWindow(true);

	m_pDlgField = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Whole.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgField );
	m_pDlgField->SetPos(vPos);
	m_pDlgField->SetShowEnable(false, false);
	m_pDlgField->SetDisableUnderWindow(true);

	m_pDlgFriend = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Friend.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgFriend );
	m_pDlgFriend->SetPos(vPos);
	m_pDlgFriend->SetShowEnable(false, false);
	m_pDlgFriend->SetDisableUnderWindow(true);

	//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
	m_pDlgGuild = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Guild.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgGuild );
	m_pDlgGuild->SetPos(vPos);
	m_pDlgGuild->SetShowEnable(false, false);
	m_pDlgGuild->SetDisableUnderWindow(true);

	InitGuildUI();

#ifdef GUILD_RADIO_NULL_CHECK
	CKTDGUIRadioButton* pRadioButtonGuild = m_pDlgBack->GetRadioButton_LUA( "guild" );
	if( NULL != pRadioButtonGuild )
		pRadioButtonGuild->SetEnable( true );
#else
	m_pDlgBack->GetRadioButton_LUA( "guild" )->SetEnable( true );
#endif //GUILD_RADIO_NULL_CHECK

#endif	GUILD_MANAGEMENT
	//{{ kimhc // 2009-09-17 // 길드 탭 UI

	m_pDlgDisciple = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Disciple.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgDisciple );
	m_pDlgDisciple->SetPos(vPos);
	m_pDlgDisciple->SetShowEnable(false, false);
	m_pDlgDisciple->SetDisableUnderWindow(true);

#ifdef SERV_NO_DISCIPLE
	if(m_pDlgBack != NULL)
	{
		CKTDGUIControl* pDiscipleButton = m_pDlgBack->GetControl(L"disciple");
		if(pDiscipleButton != NULL)
		{
			pDiscipleButton->SetShowEnable(false,false);
		}
	}
#endif SERV_NO_DISCIPLE

	//m_pDLGCreateGroup = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Group_Make.lua", 0.07f );
	//g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCreateGroup );
	//m_pDLGCreateGroup->SetShowEnable(false, false);

	//m_pDLGGroupMove = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Group_Move.lua", 0.07f );
	//g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGroupMove );
	//m_pDLGGroupMove->SetShowEnable(false, false);

	m_pDLGUserMenu = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Menu.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUserMenu );
	m_pDLGUserMenu->SetShowEnable(false, false);
	m_pDLGUserMenu->SetDisableUnderWindow(true);

	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
	m_pTalkBoxManager->SetOverUI( true );

	CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"all");
	pRadioButton->SetChecked(true);

	for(int i=0; i<8; ++i)
	{
		m_pFieldUserIcon[i] = new CKTDGUIDialog( m_pStage, L"DLG_UI_Character_Icon.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pFieldUserIcon[i] );
		m_pFieldUserIcon[i]->SetShowEnable(false, false);
	}	


#ifdef	ADDED_RELATIONSHIP_SYSTEM
	
	m_pDlgRelation = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Relation.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgRelation );
	m_pDlgRelation->SetPos(vPos);
	m_pDlgRelation->SetShowEnable(false, false);
	m_pDlgRelation->SetDisableUnderWindow(true);

	m_pDlgRelationOnlyPartnerShowing = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Relation_Only_Partner_Showing.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgRelationOnlyPartnerShowing );
	m_pDlgRelationOnlyPartnerShowing->SetPos(vPos);
	m_pDlgRelationOnlyPartnerShowing->SetShowEnable(false, false);
	m_pDlgRelationOnlyPartnerShowing->SetDisableUnderWindow(true);

	m_pDlgRelationOnlyNotCouple = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Relation_Only_Not_Couple.lua", 0.07f );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgRelationOnlyNotCouple );
	m_pDlgRelationOnlyNotCouple->SetPos(vPos);
	m_pDlgRelationOnlyNotCouple->SetShowEnable(false, false);
	m_pDlgRelationOnlyNotCouple->SetDisableUnderWindow(true);

#endif	// ADDED_RELATIONSHIP_SYSTEM

}

#if 0 
void CX2Community::SetMode( CX2Community::X2_COMMUNITY_TAB messengerMode )
{
	m_MessengerMode = messengerMode;

	if ( m_MessengerMode == XMM_USER_LIST_MODE )
	{
		m_pOpendChatSession = NULL;

		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGMgr->GetControl( L"StaticMessenger_Chat_2" );
		if ( pStatic != NULL )
			pStatic->SetShow( false );

		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGMgr->GetControl( L"ButtonAdd_Friend" );
		if ( pButton != NULL )
		{
			pButton->SetShow( true );
			pButton->SetEnable( true );
		}

		pButton = (CKTDGUIButton*)m_pDLGMgr->GetControl( L"ButtonAdd_Group" );
		if ( pButton != NULL )
		{
			pButton->SetShow( true );
			pButton->SetEnable( true );
		}



		CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)m_pDLGMgr->GetControl( L"IMEChat" );
		if ( pIMEEdit != NULL )
		{
			pIMEEdit->SetShow( false );
			pIMEEdit->SetEnable( false );
		}

		ResetUserListUI();
	}
	else if ( m_MessengerMode == XMM_CHAT_MODE )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGMgr->GetControl( L"StaticMessenger_Chat_2" );
		if ( pStatic != NULL )
			pStatic->SetShow( true );

		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGMgr->GetControl( L"ButtonAdd_Friend" );
		if ( pButton != NULL )
		{
			pButton->SetShow( false );
			pButton->SetEnable( false );
		}

		pButton = (CKTDGUIButton*)m_pDLGMgr->GetControl( L"ButtonAdd_Group" );
		if ( pButton != NULL )
		{
			pButton->SetShow( false );
			pButton->SetEnable( false );
		}



		CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)m_pDLGMgr->GetControl( L"IMEChat" );
		if ( pIMEEdit != NULL )
		{
			pIMEEdit->SetShow( true );
			pIMEEdit->SetEnable( true );
			pIMEEdit->RequestFocus();
		}

		CKTDGUIListBox* pListBox = GetListBox();
		if ( pListBox != NULL )
		{
			pListBox->RemoveAllItems();
		}

		SetOpenedChatSessionHistory();

		// 일단 comment out
		//m_CurrentTrackPos = GetListBox()->GetScrollBar()->GetTrackPos();
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_97 ), m_pStage );
	}
}
#endif


void CX2Community::SetTab( CX2Community::X2_COMMUNITY_TAB messengerTab )
{
	m_MessengerUserTab = messengerTab;
	m_fFieldUserTimer = 0.f;

	if(m_bOpen == false)
	{
		m_pDlgField->SetShowEnable(false, false);
		m_pDlgFriend->SetShowEnable(false, false);
		//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
		m_pDlgGuild->SetShowEnable(false, false);
#endif	GUILD_MANAGEMENT
		//{{ kimhc // 2009-09-17 // 길드 탭 UI

		m_pDlgDisciple->SetShowEnable(false, false);

#ifdef	SERV_RECRUIT_EVENT_BASE
		if ( m_pDlgRecruit != NULL )
			m_pDlgRecruit->SetShowEnable( false, false );
#endif	SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
		if ( m_pDlgReferee != NULL )
			m_pDlgReferee->SetShowEnable( false, false );
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
	}

	m_iPickedUserIndex = 0;
	m_iPickedUserUID = 0;
	CheckButton(NULL);

	//SetMode( CX2Community::XMM_USER_LIST_MODE );
#ifdef COMMUNITY_ALL_TAB_BUG_FIX
	CKTDGUIRadioButton *pRadioAll = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"all");
#endif COMMUNITY_ALL_TAB_BUG_FIX

	switch(messengerTab)
	{
	case XMUT_NONE:		
		break;
	case XMUT_FIELD:
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgField, true );
			m_pDlgField->SetShowEnable(true, true);
			m_pDlgFriend->SetShowEnable(false, false);
			m_pDlgDisciple->SetShowEnable(false, false);
			//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
			m_pDlgGuild->SetShowEnable(false, false);
#endif	GUILD_MANAGEMENT
			//}} kimhc // 2009-09-17 // 길드 탭 UI

#ifdef	SERV_RECRUIT_EVENT_BASE
			if ( m_pDlgRecruit != NULL )
				m_pDlgRecruit->SetShowEnable( false, false );
#endif	SERV_RECRUIT_EVENT_BASE

			//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
			if ( m_pDlgReferee != NULL )
				m_pDlgReferee->SetShowEnable( false, false );
#endif	VIEW_REFEREE_LIST
			//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM			
			m_pDlgRelation->SetShowEnable (false, false );
			m_pDlgRelationOnlyPartnerShowing->SetShowEnable (false, false );
			m_pDlgRelationOnlyNotCouple->SetShowEnable( false, false );
#endif // ADDED_RELATIONSHIP_SYSTEM
			
			for(int i=0; i<8; ++i)
			{
				if(m_pFieldUserIcon[i] != NULL)
				{
					m_pFieldUserIcon[i]->SetShowEnable(true, true);
				}
			}

			// 채널표시
			if( g_pInstanceData != NULL && g_pInstanceData->GetConnectChannelServerID() > 0 )
			{
				wstring wstrLocalInfo = g_pInstanceData->GetConnectChannelServerName();
				CX2LocationManager::VillageTemplet *pVillage = g_pData->GetLocationManager()->GetVillageMapTemplet( g_pData->GetLocationManager()->GetCurrentVillageID() );
				if( NULL != pVillage )
				{
					wstrLocalInfo += L" (";
					wstrLocalInfo += pVillage->m_Name;
					wstrLocalInfo += L")";

					CKTDGUIStatic* pStaticName = (CKTDGUIStatic*) m_pDlgField->GetControl( L"channel_name" );
#ifdef INTEGRATE_TOOLTIP
					bool bEllipse = false;
					wstring tempName = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(wstrLocalInfo.c_str(), 160, pStaticName->GetString(0)->fontIndex, 1, bEllipse);

					if(bEllipse == true)
					{
						CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDlgField->GetControl( L"ChannelTip_ext0" );

						if(pButton != NULL)
						{
							pButton->SetGuideDesc(wstrLocalInfo.c_str());
							pButton->SetShow(true);
						}
					}
					else
					{
						CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDlgField->GetControl( L"ChannelTip_ext0" );

						if(pButton != NULL)
						{
							pButton->SetShow(false);
						}
					}

					pStaticName->GetString(0)->msg = tempName;
#else
					pStaticName->GetString(0)->msg = wstrLocalInfo;	
#endif INTEGRATE_TOOLTIP
				}
			}			

			UpdateUserList(1);
		}
		break;
	case XMUT_FRIEND:
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgFriend, true );
			m_pDlgField->SetShowEnable(false, false);
			m_pDlgFriend->SetShowEnable(true, true);

			//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
			m_pDlgGuild->SetShowEnable(false, false);
#endif	GUILD_MANAGEMENT
			//}} kimhc // 2009-09-17 // 길드 탭 UI
			
			m_pDlgDisciple->SetShowEnable(false, false);

#ifdef	SERV_RECRUIT_EVENT_BASE
			if ( m_pDlgRecruit != NULL )
				m_pDlgRecruit->SetShowEnable( false, false );
#endif	SERV_RECRUIT_EVENT_BASE

			//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
			if ( m_pDlgReferee != NULL )
				m_pDlgReferee->SetShowEnable( false, false );
#endif	VIEW_REFEREE_LIST
			//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM
			m_pDlgRelation->SetShowEnable (false, false );
			m_pDlgRelationOnlyPartnerShowing->SetShowEnable (false, false );
			m_pDlgRelationOnlyNotCouple->SetShowEnable( false, false );
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifndef COMMUNITY_ALL_TAB_BUG_FIX
			CKTDGUIRadioButton *pRadioAll = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"all");		
#endif COMMUNITY_ALL_TAB_BUG_FIX
			//CKTDGUIRadioButton *pRadioDisciple = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"disciple");
			//CKTDGUIRadioButton *pRadioGuild = m_pDlgBack->GetControl(L"guild");

			// 게임중 커뮤니티창이 열렸을시 전체탭은 활성화시키지 못하도록 한다.
			if( m_pDlgBack != NULL && (g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME || g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME) )
			{
				pRadioAll->SetEnable(false);
				//pRadioDisciple->SetEnable(false);
			}
			else
			{
				pRadioAll->SetEnable(true);
				//pRadioDisciple->SetEnable(true);
			}


			for(int i=0; i<8; ++i)
			{
				if(m_pFieldUserIcon[i] != NULL)
				{
					m_pFieldUserIcon[i]->SetShowEnable(false, false);
				}
			}
			UpdateUserList(1);
		}
		break;
	case XMUT_GUILD:
		//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgGuild, true );
		m_pDlgGuild->SetShowEnable(true, true);
#endif	GUILD_MANAGEMENT
		//{{ kimhc // 2009-09-17 // 길드 탭 UI

		m_pDlgField->SetShowEnable(false, false);
		m_pDlgFriend->SetShowEnable(false, false);
		m_pDlgDisciple->SetShowEnable(false, false);

#ifdef	SERV_RECRUIT_EVENT_BASE
		if ( m_pDlgRecruit != NULL )
			m_pDlgRecruit->SetShowEnable( false, false );
#endif	SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
		if ( m_pDlgReferee != NULL )
			m_pDlgReferee->SetShowEnable( false, false );
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM
		m_pDlgRelation->SetShowEnable (false, false );
		m_pDlgRelationOnlyPartnerShowing->SetShowEnable (false, false );
		m_pDlgRelationOnlyNotCouple->SetShowEnable( false, false );
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef COMMUNITY_ALL_TAB_BUG_FIX
		// 게임중 커뮤니티창이 열렸을시 전체탭은 활성화시키지 못하도록 한다.
		if( m_pDlgBack != NULL && (g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME || g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME) )
		{
			pRadioAll->SetEnable(false);
			//pRadioDisciple->SetEnable(false);
		}
		else
		{
			pRadioAll->SetEnable(true);
			//pRadioDisciple->SetEnable(true);
		}
#endif COMMUNITY_ALL_TAB_BUG_FIX

		for(int i=0; i<8; ++i)
		{
			if(m_pFieldUserIcon[i] != NULL)
			{
				m_pFieldUserIcon[i]->SetShowEnable(false, false);
			}
		}

		//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
		ResetGuildTabUI();
#endif	GUILD_MANAGEMENT
		//{{ kimhc // 2009-09-17 // 길드 탭 UI

		UpdateUserList(1);
		break;
	case XMUT_DISCIPLE:
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgDisciple, true );
		m_pDlgField->SetShowEnable(false, false);
		m_pDlgFriend->SetShowEnable(false, false);
		//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
		m_pDlgGuild->SetShowEnable(false, false);
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-17 // 길드 탭 UI

		m_pDlgDisciple->SetShowEnable(true, true);	

#ifdef	SERV_RECRUIT_EVENT_BASE
		if ( m_pDlgRecruit != NULL )
			m_pDlgRecruit->SetShowEnable( false, false );
#endif	SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
		if ( m_pDlgReferee != NULL )
			m_pDlgReferee->SetShowEnable( false, false );
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM
		m_pDlgRelation->SetShowEnable (false, false );
		m_pDlgRelationOnlyPartnerShowing->SetShowEnable (false, false );
		m_pDlgRelationOnlyNotCouple->SetShowEnable( false, false );

#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef COMMUNITY_ALL_TAB_BUG_FIX
		// 게임중 커뮤니티창이 열렸을시 전체탭은 활성화시키지 못하도록 한다.
		if( m_pDlgBack != NULL && (g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME || g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME) )
		{
			pRadioAll->SetEnable(false);
			//pRadioDisciple->SetEnable(false);
		}
		else
		{
			pRadioAll->SetEnable(true);
			//pRadioDisciple->SetEnable(true);
		}
#endif COMMUNITY_ALL_TAB_BUG_FIX

		for(int i=0; i<8; ++i)
		{
			if(m_pFieldUserIcon[i] != NULL)
			{
				m_pFieldUserIcon[i]->SetShowEnable(false, false);
			}
		}
		UpdateUserList(1);
		break;

#ifdef SERV_RECRUIT_EVENT_BASE
	case XMUT_RECRUIT:
		{
			if ( m_pDlgRecruit != NULL )
			{
				g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRecruit, true );
				m_pDlgField->SetShowEnable(false, false);
				m_pDlgFriend->SetShowEnable(false, false);
				//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
				m_pDlgGuild->SetShowEnable(false, false);
#endif	GUILD_MANAGEMENT
				//}} kimhc // 2009-09-17 // 길드 탭 UI
#ifndef NO_DISCIPLE
				m_pDlgDisciple->SetShowEnable(false, false);	
#endif	NO_DISCIPLE

#ifdef VIEW_REFEREE_LIST
				if ( m_pDlgReferee != NULL )
					m_pDlgReferee->SetShowEnable( false, false );
#endif VIEW_REFEREE_LIST

#ifdef ADDED_RELATIONSHIP_SYSTEM
				m_pDlgRelation->SetShowEnable (false, false );
				m_pDlgRelationOnlyPartnerShowing->SetShowEnable (false, false );
				m_pDlgRelationOnlyNotCouple->SetShowEnable( false, false );
#endif // ADDED_RELATIONSHIP_SYSTEM

				m_pDlgRecruit->SetShowEnable(true, true);	

				for(int i=0; i<8; ++i)
				{
					if(m_pFieldUserIcon[i] != NULL)
					{
						m_pFieldUserIcon[i]->SetShowEnable(false, false);
					}
				}
				UpdateUserList(1);
			}
		} break;
#endif SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	case XMUT_REFEREE:
		{
			if ( m_pDlgReferee != NULL )
			{
				g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgReferee, true );
#ifdef SERV_RECRUIT_EVENT_BASE
				m_pDlgRecruit->SetShowEnable(false, false);
#endif SERV_RECRUIT_EVENT_BASE
				m_pDlgField->SetShowEnable(false, false);
				m_pDlgFriend->SetShowEnable(false, false);
				//{{ kimhc // 2009-09-17 // 길드 탭 UI
#ifdef	GUILD_MANAGEMENT
				m_pDlgGuild->SetShowEnable(false, false);
#endif	GUILD_MANAGEMENT
				//}} kimhc // 2009-09-17 // 길드 탭 UI
				m_pDlgDisciple->SetShowEnable(false, false);	

#ifdef ADDED_RELATIONSHIP_SYSTEM
				m_pDlgRelation->SetShowEnable (false, false );
				m_pDlgRelationOnlyPartnerShowing->SetShowEnable (false, false );
				m_pDlgRelationOnlyNotCouple->SetShowEnable( false, false );

#endif // ADDED_RELATIONSHIP_SYSTEM

				m_pDlgReferee->SetShowEnable(true, true);	

				for(int i=0; i<8; ++i)
				{
					if(m_pFieldUserIcon[i] != NULL)
					{
						m_pFieldUserIcon[i]->SetShowEnable(false, false);
					}
				}
				UpdateUserList(1);
			}
		}
		break;
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

	case XMUT_RELATION :
#ifdef ADDED_RELATIONSHIP_SYSTEM
		m_pDlgRelation->SetShowEnable(false, false);
		m_pDlgRelationOnlyPartnerShowing->SetShowEnable(false, false);
		m_pDlgRelationOnlyNotCouple->SetShowEnable( false, false);
		m_pDlgField->SetShowEnable(false, false);
		m_pDlgFriend->SetShowEnable(false, false);
		m_pDlgGuild->SetShowEnable(false, false);
		m_pDlgDisciple->SetShowEnable(false, false);	
#ifdef	VIEW_REFEREE_LIST
		if ( NULL != m_pDlgReferee )
			m_pDlgReferee->SetShowEnable(false, false);	
#endif //VIEW_REFEREE_LIST
#ifdef	SERV_RECRUIT_EVENT_BASE
		if ( m_pDlgRecruit != NULL )
			m_pDlgRecruit->SetShowEnable( false, false );
#endif	SERV_RECRUIT_EVENT_BASE
		
#ifdef COMMUNITY_ALL_TAB_BUG_FIX
		// 게임중 커뮤니티창이 열렸을시 전체탭은 활성화시키지 못하도록 한다.
		if( m_pDlgBack != NULL && (g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME || g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME) )
		{
			pRadioAll->SetEnable(false);
			//pRadioDisciple->SetEnable(false);
		}
		else
		{
			pRadioAll->SetEnable(true);
			//pRadioDisciple->SetEnable(true);
		}
#endif COMMUNITY_ALL_TAB_BUG_FIX

		if ( NULL != g_pData->GetRelationshipManager() &&
			NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType !=
				SEnum::RT_SOLO )
			{
				if ( NULL != m_pDlgRelation ) 
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRelation, true );
					m_pDlgRelation->SetShowEnable ( true, true );
				}
				if ( NULL != m_pDlgRelationOnlyPartnerShowing ) 
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRelationOnlyPartnerShowing, true );		
					m_pDlgRelationOnlyPartnerShowing->SetShowEnable (true, true );
				}	
			}
			else
			{
				if ( NULL != m_pDlgRelationOnlyNotCouple )
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRelationOnlyNotCouple, true );
					m_pDlgRelationOnlyNotCouple->SetShowEnable ( true, true );
				}
			}
		}

		for(int i=0; i<8; ++i)
		{
			if(m_pFieldUserIcon[i] != NULL)
			{
				m_pFieldUserIcon[i]->SetShowEnable(false, false);
			}
		}


		

		//{{ kimhc // 2009-09-17 // 길드 탭 UI

		// UpdateUserList(1);
#endif // ADDED_RELATIONSHIP_SYSTEM

		break;
	
	default:
		break;
	}		
}



void CX2Community::ClearUserList()
{
	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecDefaultGroupUser.size(); i++ )
	{
		CX2Community::MessengerUser* pMessengerUser = m_pMessengerUserList->m_vecDefaultGroupUser[i];
		SAFE_DELETE( pMessengerUser );
	}
	m_pMessengerUserList->m_vecDefaultGroupUser.clear();

	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];

		for ( int j = 0; j < (int)pMessengerUserGroup->m_vecUser.size(); j++ )
		{
			CX2Community::MessengerUser* pMessengerUser = pMessengerUserGroup->m_vecUser[j];
			SAFE_DELETE( pMessengerUser );
		}
		pMessengerUserGroup->m_vecUser.clear();

		SAFE_DELETE( pMessengerUserGroup );
	}
	m_pMessengerUserList->m_vecUserGroup.clear();	
}

void CX2Community::ResetUserListUI()
{
#ifdef ELLIPSE_GLOBAL
	bool bEllipse = false;
#endif ELLIPSE_GLOBAL

#ifdef NEW_MESSENGER
	if ( m_bOpen == false )
		return;
#endif NEW_MESSENGER
	if ( m_MessengerUserTab != XMUT_FRIEND )
		return;

	vector<MessengerUser*> vecOnUser, vecOffUser, vecEtcUser, vecPlayUser;

	CKTDGUIListBox* pListBox = GetListBox();
	pListBox->RemoveAllItems();
	pListBox->InvalidSelectedItem();

	if(m_bSortGroup == true)
	{
		// 그룹별 소트
		//그룹에 속해있지 않은 친구 리스트 등록
		for ( int i = 0; i < (int)m_pMessengerUserList->m_vecDefaultGroupUser.size(); i++ )
		{
			CX2Community::MessengerUser* pMessengerUser = m_pMessengerUserList->m_vecDefaultGroupUser[i];

			if ( pMessengerUser == NULL )
				continue;

			wstring tempExplantion = pMessengerUser->m_NickName;
			tempExplantion += L" ";
			tempExplantion += GetStatusString( pMessengerUser );

			//tempExplantion += pMessengerUser->m_Position;
#ifdef ELLIPSE_GLOBAL
			bEllipse = false;
			tempExplantion = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempExplantion.c_str(), 240, pListBox->GetString()->fontIndex, 1, bEllipse);
#endif ELLIPSE_GLOBAL

#ifdef NEW_MESSENGER
			// 그룹별 소트
			if ( pMessengerUser->IsOnline() )
				pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
			else
				pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#else
			if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
				pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
			else
				pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#endif NEW_MESSENGER
		}



		//그룹에 속해있는 친구 리스트 등록
		for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
		{
			CX2Community::MessengerUserGroup* pMgrUserGroup = m_pMessengerUserList->m_vecUserGroup[i];

			if ( pMgrUserGroup == NULL )
				continue;

			// 내 친구 (1/3) 이런 수치 정보를 표시해주기 위한 루틴
			int onlineFriend = 0;
			for ( int i = 0; i < (int)pMgrUserGroup->m_vecUser.size(); i++ )
			{
				CX2Community::MessengerUser* pMessengerUser = pMgrUserGroup->m_vecUser[i];
#ifdef NEW_MESSENGER
				if( pMessengerUser->IsOnline())
					onlineFriend++;
#else
				if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
					onlineFriend++;
#endif NEW_MESSENGER
			}

			WCHAR buff[512];

			StringCchPrintf( buff, 512 ,  L"%s (%d/%d)", pMgrUserGroup->m_GroupName.c_str(), onlineFriend, (int)pMgrUserGroup->m_vecUser.size() );
			//wsprintf( buff, L"%s (%d/%d)", pMgrUserGroup->m_GroupName.c_str(), onlineFriend, (int)pMgrUserGroup->m_vecUser.size() );

			set< wstring >::iterator sit;
			sit = m_setOpenedGroupName.find( pMgrUserGroup->m_GroupName );
			if ( sit != m_setOpenedGroupName.end() )
			{
				pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_MINUS );
				pMgrUserGroup->m_bIsOpen = true;
			}
			else
			{
				pMgrUserGroup->m_bIsOpen = false;
				pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_PLUS );
			}

			for ( int i = 0; i < (int)pMgrUserGroup->m_vecUser.size(); i++ )
			{
				CX2Community::MessengerUser* pMessengerUser = pMgrUserGroup->m_vecUser[i];

				if ( pMessengerUser == NULL )
					continue;

				if ( pMgrUserGroup->m_bIsOpen == true )
				{
					wstring tempExplantion = CX2Community::ConvertOrgName( pMessengerUser->m_NickName.c_str() );
					tempExplantion += L" ";
					tempExplantion += GetStatusString( pMessengerUser );

					//tempExplantion += pMessengerUser->m_Position;
#ifdef ELLIPSE_GLOBAL
					bEllipse = false;
					tempExplantion = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempExplantion.c_str(), 240, pListBox->GetString()->fontIndex, 1, bEllipse);
#endif ELLIPSE_GLOBAL

#ifdef NEW_MESSENGER
					if ( pMessengerUser->IsOnline() )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#else
					if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#endif NEW_MESSENGER
				}
			}
		}
	}
	else
	{
		// 상태별 소트
		for ( int i = 0; i < (int)m_pMessengerUserList->m_vecDefaultGroupUser.size(); ++i )
		{
			CX2Community::MessengerUser* pMessengerUser = m_pMessengerUserList->m_vecDefaultGroupUser[i];

			if ( pMessengerUser == NULL )
				continue;

			wstring tempExplantion = pMessengerUser->m_NickName;
			tempExplantion += L" ";
			tempExplantion += GetStatusString( pMessengerUser );

			switch(GetStatusEnum(pMessengerUser))
			{
			case FS_ONLINE:
				vecOnUser.push_back(pMessengerUser);
				break;
			case FS_OFFLINE:
				vecOffUser.push_back(pMessengerUser);
				break;
			case FS_ETC:
				vecEtcUser.push_back(pMessengerUser);
				break;
			case FS_GAME_CONNECTED:
				vecPlayUser.push_back(pMessengerUser);
				break;
			default:
				vecEtcUser.push_back(pMessengerUser);
				break;
			}
		}

		for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
		{
			CX2Community::MessengerUserGroup* pMgrUserGroup = m_pMessengerUserList->m_vecUserGroup[i];

			if ( pMgrUserGroup == NULL )
				continue;

			for ( int i = 0; i < (int)pMgrUserGroup->m_vecUser.size(); i++ )
			{
				CX2Community::MessengerUser* pMessengerUser = pMgrUserGroup->m_vecUser[i];
				{
					switch(GetStatusEnum(pMessengerUser))
					{
					case FS_ONLINE:
						vecOnUser.push_back(pMessengerUser);
						break;
					case FS_OFFLINE:
						vecOffUser.push_back(pMessengerUser);
						break;
					case FS_ETC:
						vecEtcUser.push_back(pMessengerUser);
						break;
					case FS_GAME_CONNECTED:
						vecPlayUser.push_back(pMessengerUser);
						break;
					default:
						vecEtcUser.push_back(pMessengerUser);
						break;
					}
				}
			}
		}

		int totalUser = 0;
		totalUser = vecPlayUser.size() + vecOnUser.size() + vecOffUser.size() + vecEtcUser.size();

		// 게임접속중
		WCHAR buff[512];
		StringCchPrintf( buff, 512, L"%s (%d/%d)", GET_STRING( STR_ID_64 ), vecPlayUser.size(), totalUser );
		//wsprintf( buff, L"%s (%d/%d)", GET_STRING( STR_ID_64 ), vecPlayUser.size(), totalUser );

		set< wstring >::iterator sit;
		sit = m_setOpenedGroupName.find( GET_STRING( STR_ID_64 ) );
		if ( sit != m_setOpenedGroupName.end() )
		{
			pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_MINUS );
			if(vecPlayUser.size() == 0)
			{
				pListBox->AddItem( GET_STRING( STR_ID_84 ), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
			}
			else
			{
				for(UINT i=0; i<vecPlayUser.size(); ++i)
				{
					CX2Community::MessengerUser* pMessengerUser = vecPlayUser[i];
					if ( pMessengerUser == NULL )
						continue;
					wstring tempExplantion = CX2Community::ConvertOrgName( pMessengerUser->m_NickName.c_str() );
					tempExplantion += L" ";
					tempExplantion += GetStatusString( pMessengerUser );

					//tempExplantion += pMessengerUser->m_Position;
#ifdef ELLIPSE_GLOBAL
				    bEllipse = false;
					tempExplantion = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempExplantion.c_str(), 240, pListBox->GetString()->fontIndex, 1, bEllipse);
#endif ELLIPSE_GLOBAL

#ifdef NEW_MESSENGER
					if ( pMessengerUser->IsOnline() )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#else
					if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#endif NEW_MESSENGER
				}
			}			
		}
		else
		{
			pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_PLUS );
		}		

		// 온라인
		StringCchPrintf( buff, 512 , L"%s (%d/%d)", GET_STRING( STR_ID_98 ), vecOnUser.size(), totalUser );
		//wsprintf( buff, L"%s (%d/%d)", GET_STRING( STR_ID_98 ), vecOnUser.size(), totalUser );

		sit = m_setOpenedGroupName.find( GET_STRING( STR_ID_98 ) );
		if ( sit != m_setOpenedGroupName.end() )
		{
			pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_MINUS );
			if(vecOnUser.size() == 0)
			{
				pListBox->AddItem( GET_STRING( STR_ID_84 ), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
			}
			else
			{
				for(UINT i=0; i<vecOnUser.size(); ++i)
				{
					CX2Community::MessengerUser* pMessengerUser = vecOnUser[i];
					if ( pMessengerUser == NULL )
						continue;
					wstring tempExplantion = CX2Community::ConvertOrgName( pMessengerUser->m_NickName.c_str() );
					tempExplantion += L" ";
					tempExplantion += GetStatusString( pMessengerUser );

					//tempExplantion += pMessengerUser->m_Position;
#ifdef ELLIPSE_GLOBAL
					bEllipse = false;
					tempExplantion = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempExplantion.c_str(), 240, pListBox->GetString()->fontIndex, 1, bEllipse);
#endif ELLIPSE_GLOBAL

#ifdef NEW_MESSENGER
					if ( pMessengerUser->IsOnline() )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#else
					if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#endif NEW_MESSENGER
				}
			}			
		}
		else
		{
			pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_PLUS );
		}

		// 오프라인
		StringCchPrintf( buff, 512,  L"%s (%d/%d)", GET_STRING( STR_ID_99 ), vecOffUser.size(), totalUser );
		//wsprintf( buff, L"%s (%d/%d)", GET_STRING( STR_ID_99 ), vecOffUser.size(), totalUser );

		
		sit = m_setOpenedGroupName.find( GET_STRING( STR_ID_99 ) );
		if ( sit != m_setOpenedGroupName.end() )
		{
			pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_MINUS );
			if(vecOffUser.size() == 0)
			{
				pListBox->AddItem( GET_STRING( STR_ID_84 ), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
			}
			else
			{
				for(UINT i=0; i<vecOffUser.size(); ++i)
				{
					CX2Community::MessengerUser* pMessengerUser = vecOffUser[i];
					if ( pMessengerUser == NULL )
						continue;
					wstring tempExplantion = CX2Community::ConvertOrgName( pMessengerUser->m_NickName.c_str() );
					tempExplantion += L" ";
					tempExplantion += GetStatusString( pMessengerUser );

					//tempExplantion += pMessengerUser->m_Position;
#ifdef ELLIPSE_GLOBAL
					bEllipse = false;
					tempExplantion = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempExplantion.c_str(), 240, pListBox->GetString()->fontIndex, 1, bEllipse);
#endif ELLIPSE_GLOBAL

#ifdef NEW_MESSENGER
					if ( pMessengerUser->IsOnline() )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#else
					if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#endif NEW_MESSENGER
				}
			}			
		}
		else
		{
			pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_PLUS );
		}

		// 기타

		StringCchPrintf( buff, 512, L"%s (%d/%d)", GET_STRING( STR_ID_100 ), vecEtcUser.size(), totalUser );
		//wsprintf( buff, L"%s (%d/%d)", GET_STRING( STR_ID_100 ), vecEtcUser.size(), totalUser );

		sit = m_setOpenedGroupName.find( GET_STRING( STR_ID_100 ) );
		if ( sit != m_setOpenedGroupName.end() )
		{
			pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_MINUS );
			if(vecEtcUser.size() == 0)
			{
				pListBox->AddItem( GET_STRING( STR_ID_84 ), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
			}
			else
			{
				for(UINT i=0; i<vecEtcUser.size(); ++i)
				{
					CX2Community::MessengerUser* pMessengerUser = vecEtcUser[i];
					if ( pMessengerUser == NULL )
						continue;
					wstring tempExplantion = CX2Community::ConvertOrgName( pMessengerUser->m_NickName.c_str() );
					tempExplantion += L" ";
					tempExplantion += GetStatusString( pMessengerUser );

					//tempExplantion += pMessengerUser->m_Position;
#ifdef ELLIPSE_GLOBAL
					bEllipse = false;
					tempExplantion = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempExplantion.c_str(), 240, pListBox->GetString()->fontIndex, 1, bEllipse);
#endif ELLIPSE_GLOBAL

#ifdef NEW_MESSENGER
					if ( pMessengerUser->IsOnline() )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#else
					if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1 );
					else
						pListBox->AddItem( tempExplantion.c_str(), NULL, 1, CKTDGUIListBox::LBIT_NOT_ENABLE );
#endif NEW_MESSENGER
				}
			}			
		}
		else
		{
			pListBox->AddItem( buff, NULL, 0, CKTDGUIListBox::LBIT_GROUP_PLUS );
		}
		
		

		
	}
	
	
	GetListBox()->GetScrollBar()->SetTrackPos( m_CurrentTrackPos );
	m_CurrentTrackPos = GetListBox()->GetScrollBar()->GetTrackPos();

	//{{ dmlee 2008.03.17 크래시 디버깅 용도 
	if( m_CurrentTrackPos < 0 || m_CurrentTrackPos > GetListBox()->GetScrollBar()->GetTrackEndPos() )
	{
		WCHAR wszText[256] = L"";
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"MSGER %d_%d", m_CurrentTrackPos, GetListBox()->GetScrollBar()->GetTrackPos() );
		StateLog( wszText );
	}
	//}} dmlee 2008.03.17 크래시 디버깅 용도

}

CX2Community::MessengerUser* CX2Community::GetMessengerUser( const WCHAR* pNickName )
{
	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecDefaultGroupUser.size(); i++ )
	{
		CX2Community::MessengerUser* pMessengerUser = m_pMessengerUserList->m_vecDefaultGroupUser[i];

		if ( pMessengerUser->m_NickName.compare( pNickName ) == 0 )
			return pMessengerUser;
	}
	

	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];

		for ( int j = 0; j < (int)pMessengerUserGroup->m_vecUser.size(); j++ )
		{
			CX2Community::MessengerUser* pMessengerUser = pMessengerUserGroup->m_vecUser[j];
			if ( pMessengerUser->m_NickName.compare( pNickName ) == 0 )
				return pMessengerUser;	
		}		
	}

	return NULL;
}

#ifdef NEW_MESSENGER
CX2Community::MessengerUser* CX2Community::GetMessengerUser( UidType UnitUID )
{
	for( vector<MessengerUser*>::iterator it = m_pMessengerUserList->m_vecDefaultGroupUser.begin(); it != m_pMessengerUserList->m_vecDefaultGroupUser.end(); ++it)
	{
		CX2Community::MessengerUser* pMessengerUser = *it;
		if ( pMessengerUser->m_UnitUID == UnitUID )
			return pMessengerUser;
	}

	for( vector<MessengerUserGroup*>::iterator itGp = m_pMessengerUserList->m_vecUserGroup.begin(); itGp != m_pMessengerUserList->m_vecUserGroup.end(); ++itGp)
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = *itGp;
		for( vector<MessengerUser*>::iterator itUsr = pMessengerUserGroup->m_vecUser.begin(); itUsr != pMessengerUserGroup->m_vecUser.end(); ++itUsr)
		{
			CX2Community::MessengerUser* pMessengerUser = *itUsr;
			if( pMessengerUser->m_UnitUID == UnitUID )
				return pMessengerUser;
		}

	}

	return NULL;
}

CX2Community::ChatSession* CX2Community::FindChatSession( UidType UnitUid )
{
	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		if ( pChatSession->m_UnitUid == UnitUid )
			return pChatSession;
	}

	return NULL;
}

int	CX2Community::GetChatSessionIndex( UidType UnitUid )
{
	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		if ( pChatSession->m_UnitUid == UnitUid )
			return i;
	}

	return 0;
}

#else NEW_MESSENGER
//{{AFX
CX2Community::MessengerUser* CX2Community::GetMessengerUser( NMIDCode uIDCode )
{
	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecDefaultGroupUser.size(); i++ )
	{
		CX2Community::MessengerUser* pMessengerUser = m_pMessengerUserList->m_vecDefaultGroupUser[i];

		if ( pMessengerUser->m_NMFriendKey.uIDCode == uIDCode )
			return pMessengerUser;
	}


	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];

		for ( int j = 0; j < (int)pMessengerUserGroup->m_vecUser.size(); j++ )
		{
			CX2Community::MessengerUser* pMessengerUser = pMessengerUserGroup->m_vecUser[j];
			if ( pMessengerUser->m_NMFriendKey.uIDCode == uIDCode )
				return pMessengerUser;	
		}		
	}

	return NULL;
}

CX2Community::ChatSession* CX2Community::FindChatSession( _UInt32_ uSerialKey )
{
	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		CNMCSChatSession* pCNMCSChatSession = pChatSession->m_pCNMCSChatSession;
		if ( pCNMCSChatSession->GetSerialKey() == uSerialKey )
			return pChatSession;
	}

	return NULL;
}


int	CX2Community::GetChatSessionIndex( _UInt32_ uSerialKey )
{
	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		CNMCSChatSession* pCNMCSChatSession = pChatSession->m_pCNMCSChatSession;
		if ( pCNMCSChatSession->GetSerialKey() == uSerialKey )
			return i;
	}

	return 0;
}



//}}AFX
#endif NEW_MESSENGER

CX2Community::MessengerUserGroup* CX2Community::GetMgrUserGroup( const WCHAR* pGroupName )
{
	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];
		if ( pMessengerUserGroup == NULL )
			continue;
		if ( pMessengerUserGroup->m_GroupName.compare( pGroupName ) == 0 )
			return pMessengerUserGroup;
	}
	
	return NULL;
}

CX2Community::MessengerUserGroup* CX2Community::GetMgrUserGroup( int iGroupCode )
{
	if(iGroupCode == 0)
		return NULL;		// Default를 위해 예약된 값이다

	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];
		if ( pMessengerUserGroup == NULL )
			continue;
		if ( pMessengerUserGroup->m_uCategoryCode == iGroupCode )
			return pMessengerUserGroup;
	}

	return NULL;
}


void CX2Community::SetShowChatSessionList( bool bShow )
{
	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		if ( pChatSession->m_pDialog != NULL )
		{
			//pChatSession->m_pDialog->SetShow( bShow );
			//pChatSession->m_pDialog->SetEnable( bShow );


			SetChatBoxShow(pChatSession, bShow);

			//if ( bShow == true )
			//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pChatSession->m_pDialog, true );
		}
	}
}

bool CX2Community::AddChatMsg( ChatSession* pChatSession, const WCHAR* pChatMsg,
							  bool bApplyColor /* = false */, const WCHAR* chatColor /* = L"#CFFFFFF" */ )
{
	if ( pChatMsg == NULL || pChatSession == NULL )
		return false;

	wstring tempChatMsg = pChatMsg;
#ifdef CLIENT_GLOBAL_LINEBREAK
	wstring tempChatColor = chatColor;

	bool bIsSameChatSession = false;
	if( pChatSession != NULL && m_pOpendChatSession != NULL && pChatSession == m_pOpendChatSession )
		bIsSameChatSession = true;

	CKTDGUIListBox* pListBoxMessenger = (CKTDGUIListBox*)pChatSession->m_pDialog->GetControl(L"ListBoxChat"); //GetListBox();
	if(pListBoxMessenger == NULL)
		return false;
		
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBoxMessenger->GetString()->fontIndex );
	CWordLineHandler::LineBreakInX2Community(tempChatMsg, pFont, tempChatColor, pChatSession->m_vecTalk, 
		pListBoxMessenger, bApplyColor, bIsSameChatSession);
#else //CLIENT_GLOBAL_LINEBREAK
	int iUniCharSize = 0;		// pixel 단위??
	int iStringSize = 0;		// pixel 단위??
	wstring tempString = L"";

	//정해진 크기(m_Size)에 따라 문장을 여러 행으로 나눈다

	CKTDGUIListBox* pListBoxMessenger = (CKTDGUIListBox*)pChatSession->m_pDialog->GetControl(L"ListBoxChat"); //GetListBox();
	if(pListBoxMessenger == NULL)
		return false;
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBoxMessenger->GetString()->fontIndex );

	for( UINT i=0; i<tempChatMsg.length(); i++ )
	{
		WCHAR wChar = tempChatMsg[i];

		if ( wChar == '\n' )
		{
			if ( bApplyColor == true )
			{
				wstring colorString = chatColor;
				colorString += tempString;
				pChatSession->m_vecTalk.push_back( colorString );
			}
			else
				pChatSession->m_vecTalk.push_back( tempString );

			if ( pChatSession == m_pOpendChatSession )
			{
				if ( bApplyColor == true )
				{
					wstring colorString = chatColor;
					colorString += tempString;
					pListBoxMessenger->AddItem( colorString.c_str(), NULL );
				}
				else
				{
					pListBoxMessenger->AddItem( tempString.c_str(), NULL );
				}
				pListBoxMessenger->SetScrollBarEndPos();
			}
			tempString.clear();
			continue;
		}

		tempString += wChar;

		
		iUniCharSize = pFont->GetWidth( wChar );

		iStringSize += iUniCharSize;

		const int iStringWidth = (int)(238 * g_pKTDXApp->GetResolutionScaleX());

		if( iStringSize >= iStringWidth  )
		{
			if ( bApplyColor == true )
			{
				wstring colorString = chatColor;
				colorString += tempString;
				pChatSession->m_vecTalk.push_back( colorString );
			}
			else
				pChatSession->m_vecTalk.push_back( tempString );

			//if ( pChatSession == m_pOpendChatSession )
			{
				if ( bApplyColor == true )
				{
					wstring colorString = chatColor;
					colorString += tempString;
					pListBoxMessenger->AddItem( colorString.c_str(), NULL );
				}
				else
				{
					pListBoxMessenger->AddItem( tempString.c_str(), NULL );
				}
				pListBoxMessenger->SetScrollBarEndPos();
			}
			tempString.clear();


			iStringSize = 0;
		}
	}

	if ( tempString.empty() == false )
	{
		if ( bApplyColor == true )
		{
			wstring colorString = chatColor;
			colorString += tempString;
			pChatSession->m_vecTalk.push_back( colorString );
		}
		else
			pChatSession->m_vecTalk.push_back( tempString );
		
		//if ( pChatSession == m_pOpendChatSession )
		{
			if ( bApplyColor == true )
			{
				wstring colorString = chatColor;
				colorString += tempString;
				pListBoxMessenger->AddItem( colorString.c_str(), NULL );
			}
			else
			{
				pListBoxMessenger->AddItem( tempString.c_str(), NULL );
			}
			pListBoxMessenger->SetScrollBarEndPos();
		}
		tempString.clear();
	}
#endif //CLIENT_GLOBAL_LINEBREAK
	return true;
}

#ifndef NEW_MESSENGER
bool CX2Community::DisconnectChatSession( _UInt32_ uSerialKey )
{

	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		if ( pChatSession != NULL )
		{
			if ( pChatSession->m_pCNMCSChatSession->GetSerialKey() == uSerialKey )
			{
				if ( pChatSession->m_pDialog != NULL )
				{
					/*
					if ( m_pOpendChatSession != pChatSession )
					{
						SAFE_DELETE( pChatSession );
						m_vecChatSession.erase( m_vecChatSession.begin() + i );
						i--;
						//세션 종료시킨다.
						SetTab( m_MessengerUserTab );
					}
					else
					{
					*/
					SetChatHistory(pChatSession);
					AddChatMsg( pChatSession, GET_STRING( STR_ID_101 ), true, L"#CFF0000" );
					//}
				}
				return true;
			}
		}
	}

	return true;
}
#endif // #ifndef NEW_MESSENGER

wstring CX2Community::GetSelectedGroupName()
{
	wstring groupName = L"";
	if ( m_pDlgBack == NULL )
		return groupName;

	CKTDGUIListBox* pListBox = GetListBox();
	if ( pListBox != NULL )
	{
		CKTDGUIListBox::ListBoxItem* pListBoxItem = pListBox->GetSelectedItem();
		if ( pListBoxItem != NULL )
		{
			wstring tempGroupName = pListBoxItem->strText;
			groupName = GetGroupName( tempGroupName.c_str() );
		}
	}

	return groupName;
}

wstring CX2Community::GetGroupName( const WCHAR* pName )
{
	wstring groupName = L"";
	wstring tempGroupName = pName;

	int index = tempGroupName.rfind( L"(" );
	if ( index == -1 )
		index = tempGroupName.size();
	else
	{
		index--;
	}

	groupName = tempGroupName.substr( 0, index );

	return groupName;
}

bool CX2Community::FilteringGroupName( const WCHAR* pName )
{
	if ( pName == NULL )
		return false;

	wstring tempGroupName = pName;

	int index = -1;
	index = tempGroupName.find( L"(" );
	if ( index != -1 )
		return false;

	index = tempGroupName.find( L")" );
	if ( index != -1 )
		return false;

	if ( tempGroupName.size() > 6 )
		return false;

	return true;
}

#ifndef NEW_MESSENGER
LPCTSTR CX2Community::GetStatusString( CNMFriendInfo* pFriendInfo )
{
#if 0 
	if ( IS_WAITCONFIRM( pFriendInfo->uFlag ) )
	{
		return _T( "(등록대기중)" );
	}

	if ( IS_BLOCKED( pFriendInfo->uFlag ) )
	{
		return _T( "(차단됨)" );
	}

	if ( IS_ONLINE( pFriendInfo->uStatus ) )
	{
		if ( pFriendInfo->bIsVirtualOnline && 
			pFriendInfo->keyFriend.keyVirtual.uGameCode != kGameCode_nexoncom )
		{
			return _T( "(게임접속중)" );
		}
		else
		{
			return _T( "(온라인)" ); 
		}
	}
	else
	{
		return _T( "(오프라인)" );
	}
#endif
	return _T( "(기타)" );
}
#endif

wstring CX2Community::GetStatusString( CX2Community::MessengerUser* pMessengerUser )
{
#ifdef NEW_MESSENGER
	if( pMessengerUser->IsWaiting() )
	{
		return GET_STRING( STR_ID_3714 );	// 허가요청중
	}
	else if ( pMessengerUser->IsRequested() )
	{
		return GET_STRING( STR_ID_3715 );	// 친구신청중
	}
#else
	if ( IS_WAITCONFIRM( pMessengerUser->m_uFlag ) )
	{
		return GET_STRING( STR_ID_3717 );	// 등록대기중
	}
#endif

#ifdef NEW_MESSENGER
	if( pMessengerUser->IsBlocked() )
#else
	if ( IS_BLOCKED( pMessengerUser->m_uFlag ) )
#endif
	{
		return GET_STRING( STR_ID_3716 );	// 차단됨
	}

#ifdef NEW_MESSENGER
	//** 차후 확장할 것
	switch(pMessengerUser->m_ePosition)
	{
	case MessengerUser::FP_ON_LINE:
		{
			wstring wstr;
			wstr += L"(";
			wstr += GET_STRING(STR_ID_98);
			wstr += L")";
			
			return wstr;
		} break;
	default:
	case MessengerUser::FP_OFF_LINE:
		{
			wstring wstr;
			wstr += L"(";
			wstr += GET_STRING(STR_ID_99);
			wstr += L")";

			return wstr;
		} break;
	}	
#else NEW_MESSENGER
	if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
	{
		if ( TRUE == pMessengerUser->m_bLogin && 
			pMessengerUser->m_NMFriendKey.keyVirtual.uGameCode != kGameCode_nexoncom )
		{
			return _T( "(게임접속중)" );
		}
		else
		{
			return _T( "(온라인)" ); 
		}
	}
	else
	{
		return _T( "(오프라인)" );
	}
#endif NEW_MESSENGER
}

CX2Community::FRIEND_STATUS CX2Community::GetStatusEnum( MessengerUser *pMessengerUser )
{
#ifdef NEW_MESSENGER
	if( pMessengerUser->IsRequested() || pMessengerUser->IsWaiting() )
#else NEW_MESSENGER
	if ( IS_WAITCONFIRM( pMessengerUser->m_uFlag ) )
#endif NEW_MESSENGER
	{
		return FS_ETC;
	}

#ifdef NEW_MESSENGER
	if( pMessengerUser->IsBlocked() )
#else NEW_MESSENGER
	if ( IS_BLOCKED( pMessengerUser->m_uFlag ) )
#endif
	{
		return FS_ETC;
	}

#ifdef NEW_MESSENGER
	//** 수정 필요. 차후 확장할 것
	if(pMessengerUser->IsOnline())
	{
		return FS_ONLINE;
	}
	else
	{
		return FS_OFFLINE;
	}
#else NEW_MESSENGER
	if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
	{
		if ( TRUE == pMessengerUser->m_bLogin && 
			pMessengerUser->m_NMFriendKey.keyVirtual.uGameCode != kGameCode_nexoncom )
		{
			return FS_GAME_CONNECTED;
		}
		else
		{
			return FS_ONLINE; 
		}
	}
	else
	{
		return FS_OFFLINE;
	}	
#endif NEW_MESSENGER
}





#ifndef NEW_MESSENGER
void CX2Community::RefreshChatInfo( _UInt32_ uSerialKey )
{
	NMSessionMemberList		aMembers;
	CNMSessionInfo			siInfo;
	CString					strTitle;

	CX2Community::ChatSession* pChatSession = FindChatSession( uSerialKey );
	if ( pChatSession == NULL )
		return;

	pChatSession->m_pCNMCSChatSession->GetSessionInfo();
	pChatSession->m_pCNMCSChatSession->GetMemberList( aMembers );
	siInfo = pChatSession->m_pCNMCSChatSession->GetSIInfo();

	if ( aMembers.size() == 2 )
	{


		if ( pChatSession->m_pDialog != NULL )
		{
			CKTDGUIStatic* pStaticOpponentName = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl( L"StaticMessenger_NickName" );
			pStaticOpponentName->GetString( 0 )->msg = CX2Community::ConvertOrgName( pChatSession->m_pCNMCSChatSession->GetOppositeNickName() );
		}

		if ( m_bRequestChat == true && 
			m_RequestedNMFriendKey.keyVirtual == pChatSession->m_pCNMCSChatSession->GetOppositeMemberInfo().keyVirtual )
		{
			m_bRequestChat = false;
			m_pOpendChatSession = pChatSession;
			//SetMode( XMM_CHAT_MODE );
		}
	}
	


	/*
	if ( siInfo.uStatus == CNMSessionInfo::kStatus_WaitingForInviteUser )
	{
	strTitle = _T( "상대방을 기다리고 있습니다" );
	}
	else if ( siInfo.uStatus == CNMSessionInfo::kStatus_ConnectionFailed
	|| aMembers.size() < 2 )
	{
	strTitle = _T( "대화하실 상대가 없습니다." );
	}
	else if ( aMembers.size() == 2 )
	{
	strTitle.Format( _T( "%s 님과의 대화창" ), this->m_NMChatSession.GetOppositeNickName( TRUE ) );
	}
	else
	{
	strTitle.Format( _T( "%d명 참가중" ), aMembers.size() );
	}

	this->SetWindowText( strTitle );
	*/
}
#endif // #ifndef NEW_MESSENGER

void CX2Community::SetOpenedChatSessionHistory()
{
#if 0 
	if ( m_pOpendChatSession == NULL )
		return;

	CKTDGUIListBox* pListBox = GetListBox();
	if ( pListBox != NULL )
	{
		pListBox->RemoveAllItems();
		for ( int i = 0; i < (int)m_pOpendChatSession->m_vecTalk.size(); i++ )
		{
			pListBox->AddItem( m_pOpendChatSession->m_vecTalk[i].c_str(), NULL );
		}
		pListBox->SetScrollBarEndPos();
	}

	CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)m_pDLGMgr->GetControl( L"IMEChat" );
	if ( pIMEEdit != NULL )
	{
		pIMEEdit->RequestFocus();
	}
#endif
}

CKTDGUIListBox* CX2Community::GetListBox()
{
	if(m_pDlgFriend != NULL)
	{
		return (CKTDGUIListBox*)m_pDlgFriend->GetControl( L"ListBoxFriend" );	
	}
	return NULL;
}

void CX2Community::UpdateUserList(int iPage)
{
// 	m_iPickedUserIndex = 0;
// 	m_iPickedUserUID = 0;
	CheckButton(NULL);

	switch(m_MessengerUserTab)
	{
	case XMUT_FIELD:		
		{
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_SQUARE_GAME:
				{
					// 커뮤니티 거래광장(전체)
					UpdateMarketUserList(iPage);
				} break;
			default:
				{
					// 커뮤니티 필드(전체)
					UpdateFieldUserList(iPage);
				} break;
			}
		} break;
		break;
	case XMUT_FRIEND:
		// 커뮤니티 친구
		// ResetUserList()가 우선적으로 처리되어야 함(친구목록 생성)
		ResetUserListUI();
		break;
	case XMUT_GUILD:
		break;
	case XMUT_DISCIPLE:		
		// 커뮤니티 사제
		UpdateDiscipleUserList();		
		break;

#ifdef SERV_RECRUIT_EVENT_BASE
	case XMUT_RECRUIT:
		{
			Handler_EGS_GET_RECRUIT_RECRUITER_LIST_REQ();
		} break;
#endif SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	case XMUT_REFEREE:
		{
			Handler_EGS_GET_RECOMMEND_USER_LIST_REQ( iPage );
		}
		break;
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

	case XMUT_RELATION :
#ifdef ADDED_RELATIONSHIP_SYSTEM 
		{

		}
#endif // DLG_UI_Community_Relation_Only_Partner_Showing
		break;

	default:
		break;
	}
}

bool CX2Community::OpenUserMenu(int idummy)
{
	if ( m_pDLGUserMenu == NULL )
		return true;

	m_bOpenGroupMenu = false;

	//{{ kimhc // 2009-09-25 // 길드탭에서 openUserMenu를 했을때 GuildUserMenu는 보이지 않게하기
#ifdef	GUILD_MANAGEMENT
	if ( g_pData->GetGuildManager() == NULL )
	{
		ASSERT( !L"GuildManager is NULL" );
		return false;
	}

	if ( g_pData->GetGuildManager()->GetUIGuild() == NULL )
	{
		ASSERT( !L"UIGuild is NULL" );
		return false;
	}

	g_pData->GetGuildManager()->GetUIGuild()->SetShowChangeGradeMenu( false );
#endif	GUILD_MANAGEMENT
	//{{ kimhc // 2009-09-25 // 길드탭에서 openUserMenu를 했을때 GuildUserMenu는 보이지 않게하기

	

	if(idummy > 0 && (int)m_vecUserUID.size() >= idummy)
	{
		m_iPickedUserIndex = idummy;
		m_iPickedUserUID = m_vecUserUID[idummy-1];
	}
	 
#ifdef EXTEND_SERVER_GROUP_MASK
	if( m_MessengerUserTab == XMUT_FRIEND )
	{
		CX2Community::MessengerUser* pMessengerUser = GetMessengerUser( GetSelectedUnitName().c_str() );
		if( pMessengerUser != NULL )
		{
			m_iPickedUserUID = pMessengerUser->m_UnitUID;
		}
	}
#endif EXTEND_SERVER_GROUP_MASK
		

	int countUserMenu = 0;	
	CKTDGUIButton *pUserMenu[USER_MENU_MAX];
	float menuPosX = 0.f; //60.f;
	float menuPosY = 0.f; //22.f;
	float menuHegith = 22.f;

	CKTDGUIStatic *pStaticUser1 = (CKTDGUIStatic*)m_pDLGUserMenu->GetControl(L"user");
	CKTDGUIControl::CPictureData*	m_pPicMiddle1 = pStaticUser1->GetPictureIndex(1);
	CKTDGUIControl::CPictureData*	m_pPicBottom1 = pStaticUser1->GetPictureIndex(2);
	CKTDGUIControl::CPictureData*	m_pPicMiddle2 = pStaticUser1->GetPictureIndex(3);
	CKTDGUIControl::CPictureData*	m_pPicBottom2 = pStaticUser1->GetPictureIndex(4);

	pUserMenu[0] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"invite" );			// 파티초대
	pUserMenu[1] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"party" );			// 파티요청
	pUserMenu[2] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"talk" );			// 대화하기
	pUserMenu[3] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"cut_cancle" );		// 차단/해제
	pUserMenu[4] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"delete" );			// 삭제
	pUserMenu[5] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"character_info" );	// 캐릭터정보	
	pUserMenu[6] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"group_move" );		// 그룹이동
	pUserMenu[7] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"friend" );			// 친구요청
	pUserMenu[8] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"disciple" );		// 사제요청
	pUserMenu[9] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"trade" );			// 개인거래
	pUserMenu[10] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"shop_look" );			// 상점보기
	pUserMenu[11] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"name_change" );			// 이름바꾸기
	pUserMenu[12] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"whisper" );			// 귓속말
	pUserMenu[13] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Invite" );			// 초대하기(대전)
	pUserMenu[14] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Together" );			// 같이하기(대전)
//{{ 허상형 : [2009/9/18] //	길드 메뉴 추가
	pUserMenu[15] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"InviteGuild" );			// 길드 초대
	pUserMenu[16] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"BanishGuild" );			// 길드 추방
//}} 허상형 : [2009/9/18] //	길드 메뉴 추가
	pUserMenu[17] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"watch" );				// 살펴보기
	
	//SERV_ED_MONITORING_IN_GAME
	pUserMenu[18] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Menu_Image" );				// 살펴보기
	pUserMenu[19] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Menu_Brick" );				// 살펴보기
	pUserMenu[20] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Menu_Machine_Brick" );				// 살펴보기
#ifdef ADDED_RELATIONSHIP_SYSTEM
	pUserMenu[21] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"couple" );					// 커플 신청
	pUserMenu[22] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"summon" );					// 소환하기
#endif // ADDED_RELATIONSHIP_SYSTEMㄴ

	for(int i=0; i<USER_MENU_MAX; ++i)
	{		
		pUserMenu[i]->SetShowEnable(false, false);
	}

	bool bHaveParty = false;
	if(g_pData != NULL && g_pData->GetPartyManager() != NULL)
		bHaveParty = g_pData->GetPartyManager()->DoIHaveParty();

	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUserMenu, true );
	m_pDLGUserMenu->SetShowEnable( true, true );

	switch(m_MessengerUserTab)
	{
		//{{ 허상형 : [2009/9/25] //	길드 메뉴 전체 탭과 동일하게
#ifdef GUILD_MANAGEMENT
	case XMUT_GUILD:
		{
			int countMenuHeight			= 0;
			countUserMenu				= 0;

			if( ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ) ||
				( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD ) )
			{	
				if( false == bHaveParty ||//파티가 없거나
					true == g_pData->GetPartyManager()->AmIPartyLeader() ) //파티장 일 때만 
				{
					++countUserMenu;
					pUserMenu[0]->SetShowEnable(true, true);
					pUserMenu[0]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));					
					++countMenuHeight;
				}
			}

			m_SelectedGuildMemberUID	= m_iPickedUserUID;	// 길드 탭일 경우에만 저장 되도록 하기위해 break 처리 안함

			++countUserMenu;
			pUserMenu[12]->SetShowEnable(true, true);
			pUserMenu[12]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;
			++countUserMenu;
			pUserMenu[5]->SetShowEnable(true, true);
			pUserMenu[5]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;
			++countUserMenu;
			pUserMenu[17]->SetShowEnable(true, true);
			pUserMenu[17]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;

#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;

				switch ( eRelationType )
				{
				case SEnum::RT_SOLO:
					{
						++countUserMenu;
						pUserMenu[21]->SetShowEnable(true, true);
						pUserMenu[21]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
						++countMenuHeight;
					} break;
				case SEnum::RT_MARRIED:
					{
						/// 마을 이거나 필드일 때만 소환하기 추가
						if ( NULL != g_pMain &&
							( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() ||
							CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() ) )
						{
							++countUserMenu;
							pUserMenu[22]->SetShowEnable(true, true);
							pUserMenu[22]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
							++countMenuHeight;
						}
					} break;
				}
			}
#endif // ADDED_RELATIONSHIP_SYSTEM


			++countUserMenu;
			pUserMenu[7]->SetShowEnable(true, true);
			pUserMenu[7]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));

#ifndef SERV_NO_DISCIPLE
			if( g_pMain->GetTutorSystem()->GetNumberOfStudent() < 3 &&
				g_pData->GetSelectUnitLevel() >= 20 )
			{
				++countUserMenu;
				++countMenuHeight;
				pUserMenu[8]->SetShowEnable(true, true);
				pUserMenu[8]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			}			
#endif SERV_NO_DISCIPLE

			//// 거래광장
			//if( m_iPickedUserIndex > 0 && m_iPickedUserUID > 0 &&
			//	g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME && 
			//	m_vecFieldUser[m_iPickedUserIndex-1].GetPartyUID() > 0 &&
			//	m_iPickedUserUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			//{
			//	++countUserMenu;
			//	++countMenuHeight;
			//	pUserMenu[10]->SetShowEnable(true, true);
			//	pUserMenu[10]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));				
			//}

			if ( g_pData->GetMyUser() == NULL ||
				g_pData->GetMyUser()->GetSelectUnit() == NULL )
				return false;

			BYTE byMyGrade = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;

			switch( byMyGrade )
			{
			case CX2GuildManager::GUG_MASTER:
			case CX2GuildManager::GUG_SYSOP:
				{
					if ( GetGuildMemberGradeByUIDInThisList( m_iPickedUserUID ) > byMyGrade &&
						 g_pData->GetGuildManager()->IsMyGuildUser( m_iPickedUserUID ) == true )
					{
						++countUserMenu;
						pUserMenu[16]->SetShowEnable(true, true);
						pUserMenu[16]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
						++countMenuHeight;
					}
				}
				break;
			}
			

		}
		break;
#endif	GUILD_MANAGEMENT
		//}} 허상형 : [2009/9/25] //	길드 메뉴 전체 탭과 동일하게
	case XMUT_FIELD:
		{
// 			bool bPickUserHasParty = false;
// 			for(UINT i=0; i<m_vecFieldUser.size(); ++i)
// 			{
// 				if(m_vecFieldUser[i].m_iUnitUID == m_iPickedUserUID && m_vecFieldUser[i].GetPartyUID() > 0)
// 				{
// 					bPickUserHasParty = true;
// 					break;
// 				}				
// 			}

			int countMenuHeight = 0;
			countUserMenu = 0;

			if( ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ) ||
				( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD ) )
			{				
				if( false == bHaveParty ||//파티가 없거나
					true == g_pData->GetPartyManager()->AmIPartyLeader() ) //파티장 일 때만 
				{
					++countUserMenu;
					pUserMenu[0]->SetShowEnable(true, true);
					pUserMenu[0]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));					
					++countMenuHeight;
				}
			}
			else if( m_iPickedUserIndex > 0 )				
			{
				// 대전로비/대전방에서 유저메뉴
				KCommunityUserInfo userNode = m_vecFieldUser[m_iPickedUserIndex-1];

				if(userNode.m_iUnitUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID())
				{
					// 초대하기
					if( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM &&
						(userNode.m_cState == KCommunityUserInfo::US_PVP_LOBBY || userNode.m_cState == KCommunityUserInfo::US_PVP_WAIT) )
					{
						++countUserMenu;
						pUserMenu[13]->SetShowEnable(true, true);
						pUserMenu[13]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));					
						++countMenuHeight;
					}

					// 같이하기
					if(userNode.m_cState == KCommunityUserInfo::US_PVP_WAIT)
					{
						++countUserMenu;
						pUserMenu[14]->SetShowEnable(true, true);
						pUserMenu[14]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));					
						++countMenuHeight;
					}
				}				
			}
			
			++countUserMenu;
			pUserMenu[12]->SetShowEnable(true, true);
			pUserMenu[12]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;
			++countUserMenu;
			pUserMenu[5]->SetShowEnable(true, true);
			pUserMenu[5]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;
			++countUserMenu;
			pUserMenu[17]->SetShowEnable(true, true);
			pUserMenu[17]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;
			++countUserMenu;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;

				switch ( eRelationType )
				{
				case SEnum::RT_SOLO:
					{
						++countUserMenu;
						pUserMenu[21]->SetShowEnable(true, true);
						pUserMenu[21]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
						++countMenuHeight;
					} break;
#ifdef REMOVE_SUMMON_ON_WHOLE_TAB
#else REMOVE_SUMMON_ON_WHOLE_TAB
				case SEnum::RT_MARRIED:
					{
						/// 마을 이거나 필드일 때만 소환하기 추가
						if ( NULL != g_pMain &&
							( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() ||
							CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() ) )
						{
							++countUserMenu;
							pUserMenu[22]->SetShowEnable(true, true);
							pUserMenu[22]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
							++countMenuHeight;
						}
					} break;
#endif REMOVE_SUMMON_ON_WHOLE_TAB
				}
			}
#endif // ADDED_RELATIONSHIP_SYSTEM

			pUserMenu[7]->SetShowEnable(true, true);
			pUserMenu[7]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));

#ifndef SERV_NO_DISCIPLE
			if( g_pMain->GetTutorSystem()->GetNumberOfStudent() < 3 &&
				g_pData->GetSelectUnitLevel() >= 20 )
			{
				++countUserMenu;
				++countMenuHeight;
				pUserMenu[8]->SetShowEnable(true, true);
				pUserMenu[8]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			}			
#endif SERV_NO_DISCIPLE
			
			if(g_pMain->GetNowStateID() != CX2Main::XS_PVP_LOBBY && g_pMain->GetNowStateID() != CX2Main::XS_PVP_ROOM)
			{
				++countUserMenu;
				++countMenuHeight;
				pUserMenu[9]->SetShowEnable(true, true);
				pUserMenu[9]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			}
			

			// 거래광장
			if( m_iPickedUserIndex > 0 && m_iPickedUserUID > 0 &&
				g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME && 
				m_vecFieldUser[m_iPickedUserIndex-1].GetPartyUID() > 0 &&
				m_iPickedUserUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
				++countUserMenu;
				++countMenuHeight;
				pUserMenu[10]->SetShowEnable(true, true);
				pUserMenu[10]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));				
			}			
			
			//{{ 허상형 : [2009/9/18] //	길드 초대 메뉴 추가
#ifdef GUILD_MANAGEMENT
			if( g_pData->GetGuildManager()->CanInviteMember() == true )	//	같은 길드 인지 검사
			{
				if( g_pData->GetGuildManager()->IsMyGuildUser(m_iPickedUserUID) == false )
				{
#ifdef MODFIY_INVITE_GUILD_MENU
					// 길드에 가입한 유저인지 체크 후 메뉴 추가
					bool bIsGuildMember = true;
					if( NULL != g_pX2Game )
					{
						CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( m_iPickedUserUID );
						if( NULL != pGUUser && 
							NULL != pGUUser->GetUnit())
						{
							if( false == pGUUser->GetUnit()->GetUnitData().m_wstrGuildName.empty() )
							{
								bIsGuildMember = false;
							}
						}
					}
					else if( NULL != g_pTFieldGame )
					{
						CX2Unit* pUnit = g_pTFieldGame->GetSquareUnitUnitByUID( m_iPickedUserUID );
						if( NULL != pUnit )
						{
							if( false == pUnit->GetUnitData().m_wstrGuildName.empty() )
							{
								bIsGuildMember = false;
							}
						}
					}

					if( true == bIsGuildMember )
#endif // MODFIY_INVITE_GUILD_MENU
					{
						++countUserMenu;
						pUserMenu[15]->SetShowEnable(true, true);
						pUserMenu[15]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
						++countMenuHeight;
					}
				}
				else	//	내 길드원이고 현재 길드 탭일 경우 추방 메뉴 추가
				{
					if( m_MessengerUserTab == XMUT_GUILD)
					{
						++countUserMenu;
						pUserMenu[16]->SetShowEnable(true, true);
						pUserMenu[16]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
						++countMenuHeight;
					}
				}
			}

#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/18] //	길드 초대 메뉴 추가

			// 개발자 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
			if( g_pInstanceData != NULL && g_pInstanceData->GetMoneyMonitoring() == true &&
				g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
			{
				++countUserMenu;
				pUserMenu[18]->SetShowEnable(true, true);
				pUserMenu[18]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
				++countMenuHeight;
				++countUserMenu;
				pUserMenu[19]->SetShowEnable(true, true);
				pUserMenu[19]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
				++countMenuHeight;
				++countUserMenu;
				pUserMenu[20]->SetShowEnable(true, true);
				pUserMenu[20]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
				++countMenuHeight;
			}
#endif //SERV_ED_MONITORING_IN_GAME
		}
		break;		
	case XMUT_FRIEND:
		{
			int countMenuHeight = 0;
			countUserMenu = 6;

			if( ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ) ||
				( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD ) )
			{	
				if( false == bHaveParty ||//파티가 없거나
					true == g_pData->GetPartyManager()->AmIPartyLeader() ) //파티장 일 때만 
				{
					++countUserMenu;
					pUserMenu[0]->SetShowEnable(true, true);
					pUserMenu[0]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));					
					++countMenuHeight;
				}
			}

			pUserMenu[2]->SetShowEnable(true, true);
			pUserMenu[2]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[3]->SetShowEnable(true, true);
			pUserMenu[3]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[4]->SetShowEnable(true, true);
			pUserMenu[4]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[5]->SetShowEnable(true, true);
			pUserMenu[5]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[17]->SetShowEnable(true, true);
			pUserMenu[17]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;

				switch ( eRelationType )
				{
				case SEnum::RT_SOLO:
					{
						++countUserMenu;
						pUserMenu[21]->SetShowEnable(true, true);
						pUserMenu[21]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
						++countMenuHeight;
					} break;
				case SEnum::RT_MARRIED:
					{
						/// 마을 이거나 필드일 때만 소환하기 추가
						if ( NULL != g_pMain &&
							( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() ||
							CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() ) )
						{
							++countUserMenu;
							pUserMenu[22]->SetShowEnable(true, true);
							pUserMenu[22]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
							++countMenuHeight;
						}
					} break;
				}
			}
#endif // ADDED_RELATIONSHIP_SYSTEM
			pUserMenu[6]->SetShowEnable(true, true);
			pUserMenu[6]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	

			//{{ 허상형 : [2009/9/18] //	길드 초대 메뉴 추가
#ifdef GUILD_MANAGEMENT
			if( g_pData->GetGuildManager()->CanInviteMember() == true )	//	같은 길드 인지 검사
			{
				if( g_pData->GetGuildManager()->IsMyGuildUser(m_iPickedUserUID) == false )
				{
					++countUserMenu;
					pUserMenu[15]->SetShowEnable(true, true);
					pUserMenu[15]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
					++countMenuHeight;
				}
				else	//	내 길드원이고 현재 길드 탭일 경우 추방 메뉴 추가
				{
					if( m_MessengerUserTab == XMUT_GUILD)
					{
						++countUserMenu;
						pUserMenu[16]->SetShowEnable(true, true);
						pUserMenu[16]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
						++countMenuHeight;
					}
				}
			}
#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/18] //	길드 초대 메뉴 추가
		}		
		break;
	case XMUT_DISCIPLE:
		{
			int countMenuHeight = 0;
			countUserMenu = 5;
#if 0 
			if(bHaveParty == true)
			{
				pUserMenu[0]->SetShowEnable(true, true);
				pUserMenu[0]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
				++countUserMenu;
				++countMenuHeight;
			}				
#endif		
			pUserMenu[12]->SetShowEnable(true, true);
			pUserMenu[12]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[4]->SetShowEnable(true, true);
			pUserMenu[4]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[5]->SetShowEnable(true, true);
			pUserMenu[5]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[17]->SetShowEnable(true, true);
			pUserMenu[17]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;

#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;

				switch ( eRelationType )
				{
				case SEnum::RT_SOLO:
					{
						++countUserMenu;
						pUserMenu[21]->SetShowEnable(true, true);
						pUserMenu[21]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
						++countMenuHeight;
					} break;
				case SEnum::RT_MARRIED:
					{
						/// 마을 이거나 필드일 때만 소환하기 추가
						if ( NULL != g_pMain &&
							( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() ||
							CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() ) )
						{
							++countUserMenu;
							pUserMenu[22]->SetShowEnable(true, true);
							pUserMenu[22]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
							++countMenuHeight;
						}
					} break;
				}
			}
#endif // ADDED_RELATIONSHIP_SYSTEM

			pUserMenu[7]->SetShowEnable(true, true);
			pUserMenu[7]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));

			//{{ 허상형 : [2009/9/18] //	길드 초대 메뉴 추가
#ifdef GUILD_MANAGEMENT
			if( g_pData->GetGuildManager()->CanInviteMember() == true )	//	같은 길드 인지 검사
			{
				if( g_pData->GetGuildManager()->IsMyGuildUser(m_iPickedUserUID) == false )
				{
					++countUserMenu;
					pUserMenu[15]->SetShowEnable(true, true);
					pUserMenu[15]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
					++countMenuHeight;
				}
				else	//	내 길드원이고 현재 길드 탭일 경우 추방 메뉴 추가
				{
					if( m_MessengerUserTab == XMUT_GUILD)
					{
						++countUserMenu;
						pUserMenu[16]->SetShowEnable(true, true);
						pUserMenu[16]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
						++countMenuHeight;
					}
				}
			}

#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/18] //	길드 초대 메뉴 추가
		}		
		break;

#ifdef SERV_RECRUIT_EVENT_BASE
	case XMUT_RECRUIT:
#endif SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-12 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	case XMUT_REFEREE:
		{
			int countMenuHeight = 0;
			countUserMenu = 4;
#if 0 
			if(bHaveParty == true)
			{
				pUserMenu[0]->SetShowEnable(true, true);
				pUserMenu[0]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
				++countUserMenu;
				++countMenuHeight;
			}				
#endif		
			pUserMenu[12]->SetShowEnable(true, true);
			pUserMenu[12]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[5]->SetShowEnable(true, true);
			pUserMenu[5]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			pUserMenu[17]->SetShowEnable(true, true);
			pUserMenu[17]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;
			
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;

				switch ( eRelationType )
				{
				case SEnum::RT_SOLO:
					{
						++countUserMenu;
						pUserMenu[21]->SetShowEnable(true, true);
						pUserMenu[21]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
						++countMenuHeight;
					} break;
				case SEnum::RT_MARRIED:
					{
						/// 마을 이거나 필드일 때만 소환하기 추가
						if ( NULL != g_pMain &&
							( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() ||
							CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() ) )
						{
							++countUserMenu;
							pUserMenu[22]->SetShowEnable(true, true);
							pUserMenu[22]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
							++countMenuHeight;
						}
					} break;
				}

			}
#endif // ADDED_RELATIONSHIP_SYSTEM

			pUserMenu[7]->SetShowEnable(true, true);
			pUserMenu[7]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));

			if( static_cast<UINT>( m_iPickedUserIndex ) <= m_vecRecommendUserInfo.size() &&
				m_vecRecommendUserInfo[m_iPickedUserIndex - 1].m_bIsOnline == true && 
				g_pMain->GetTutorSystem()->GetNumberOfStudent() < 3 &&
				g_pData->GetSelectUnitLevel() >= 20 )
			{
				++countUserMenu;
				++countMenuHeight;
				pUserMenu[8]->SetShowEnable(true, true);
				pUserMenu[8]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			}

			//{{ 허상형 : [2009/9/18] //	길드 초대 메뉴 추가
#ifdef GUILD_MANAGEMENT
			if( g_pData->GetGuildManager()->CanInviteMember() == true )	//	같은 길드 인지 검사
			{
				if( g_pData->GetGuildManager()->IsMyGuildUser(m_iPickedUserUID) == false )
				{
					++countUserMenu;
					pUserMenu[15]->SetShowEnable(true, true);
					pUserMenu[15]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
					++countMenuHeight;
				}
				else	//	내 길드원이고 현재 길드 탭일 경우 추방 메뉴 추가
				{
					if( m_MessengerUserTab == XMUT_GUILD)
					{
						++countUserMenu;
						pUserMenu[16]->SetShowEnable(true, true);
						pUserMenu[16]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));	
						++countMenuHeight;
					}
				}
			}

#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/18] //	길드 초대 메뉴 추가
		}
		break;
		
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-12 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case XMUT_RELATION:
		{
			int countMenuHeight			= 0;
			countUserMenu				= 0;

			if( ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ) ||
				( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD ) )
			{	
				if( false == bHaveParty ||//파티가 없거나
					true == g_pData->GetPartyManager()->AmIPartyLeader() ) //파티장 일 때만 
				{
					++countUserMenu;
					pUserMenu[0]->SetShowEnable(true, true);
					pUserMenu[0]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));					
					++countMenuHeight;
				}
			}

			m_SelectedGuildMemberUID	= m_iPickedUserUID;	// 길드 탭일 경우에만 저장 되도록 하기위해 break 처리 안함

			++countUserMenu;
			pUserMenu[12]->SetShowEnable(true, true);
			pUserMenu[12]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;
			++countUserMenu;
			pUserMenu[5]->SetShowEnable(true, true);
			pUserMenu[5]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;
			++countUserMenu;
			pUserMenu[17]->SetShowEnable(true, true);
			pUserMenu[17]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
			++countMenuHeight;

			++countUserMenu;
			pUserMenu[7]->SetShowEnable(true, true);
			pUserMenu[7]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
			++countMenuHeight;

#ifndef SERV_NO_DISCIPLE
			if( g_pMain->GetTutorSystem()->GetNumberOfStudent() < 3 &&
				g_pData->GetSelectUnitLevel() >= 20 )
			{
				++countUserMenu;
				pUserMenu[8]->SetShowEnable(true, true);
				pUserMenu[8]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
				++countMenuHeight;
			}			
#endif //SERV_NO_DISCIPLE
			if ( g_pData->GetMyUser() == NULL ||
				g_pData->GetMyUser()->GetSelectUnit() == NULL )
				return false;

			if ( NULL != g_pData->GetRelationshipManager() &&
				 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{
					/// 연인 타입
					SEnum::RELATIONSHIP_TYPE eRelationStateType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;

					switch ( eRelationStateType )
					{
					case SEnum::RT_MARRIED:
						{
							/// 마을 이거나 필드일 때만 소환하기 추가
							if ( NULL != g_pMain &&
								 ( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() ||
								   CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() ) )
							{
								++countUserMenu;
								pUserMenu[22]->SetShowEnable(true, true);
								pUserMenu[22]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));			
								++countMenuHeight;
							}
						} break;
					}
				}
			}
			break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		break;
	}

	m_pPicMiddle1->SetSizeY(countUserMenu * menuHegith);
	m_pPicMiddle2->SetSizeY(countUserMenu * menuHegith);

	D3DXVECTOR2 offsetPos;
	offsetPos = m_pPicMiddle1->GetPos();
	offsetPos.y += (countUserMenu * menuHegith);
	m_pPicBottom1->SetPos(offsetPos);
	
	offsetPos = m_pPicMiddle2->GetPos();
	offsetPos.y += (countUserMenu * menuHegith);
	m_pPicBottom2->SetPos(offsetPos);

	D3DXVECTOR2 vMousePos;
	vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
	vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

	m_pDLGUserMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y-30.f ) );
	

	return true;
}

bool CX2Community::OpenGroupMenu()
{

	if ( m_pDLGUserMenu == NULL || m_bSortGroup == false)
		return true;
	
	m_bOpenGroupMenu = true;

	m_iPickedUserIndex = 0;
	m_iPickedUserUID = 0;
	

	int countMenuHeight = 0;
	int countUserMenu = 2;	
	CKTDGUIButton *pUserMenu[USER_MENU_MAX] = { NULL, };
	float menuPosX = 0.f; //60.f;
	float menuPosY = 0.f; //22.f;
	float menuHegith = 22.f;

	CKTDGUIStatic *pStaticUser1 = (CKTDGUIStatic*)m_pDLGUserMenu->GetControl(L"user");
	CKTDGUIControl::CPictureData*	m_pPicMiddle1 = pStaticUser1->GetPictureIndex(1);
	CKTDGUIControl::CPictureData*	m_pPicBottom1 = pStaticUser1->GetPictureIndex(2);
	CKTDGUIControl::CPictureData*	m_pPicMiddle2 = pStaticUser1->GetPictureIndex(3);
	CKTDGUIControl::CPictureData*	m_pPicBottom2 = pStaticUser1->GetPictureIndex(4);

	pUserMenu[0] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"invite" );			// 파티초대
	pUserMenu[1] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"party" );			// 파티요청
	pUserMenu[2] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"talk" );			// 대화하기
	pUserMenu[3] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"cut_cancle" );		// 차단/해제
	pUserMenu[4] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"delete" );			// 삭제
	pUserMenu[5] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"character_info" );	// 캐릭터정보
	pUserMenu[6] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"group_move" );		// 그룹이동
	pUserMenu[7] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"friend" );			// 친구요청
	pUserMenu[8] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"disciple" );		// 사제요청
	pUserMenu[9] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"trade" );			// 개인거래
	pUserMenu[10] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"shop_look" );			// 상점보기
	pUserMenu[11] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"name_change" );			// 이름바꾸기
	pUserMenu[12] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"whisper" );			// 귓속말
	pUserMenu[13] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Invite" );			// 초대하기(대전)
	pUserMenu[14] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Together" );			// 같이하기(대전)
	//{{ 허상형 : [2009/9/19] //	길드 컨트롤 불러오기
	pUserMenu[15] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"InviteGuild" );			// 길드 초대
	pUserMenu[16] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"BanishGuild" );			// 길드 초대	
	//}} 허상형 : [2009/9/19] //	길드 컨트롤 불러오기
	pUserMenu[17] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"watch" );				// 살펴보기

	//SERV_ED_MONITORING_IN_GAME
	pUserMenu[18] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Menu_Image" );				// 살펴보기
	pUserMenu[19] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Menu_Brick" );				// 살펴보기
	pUserMenu[20] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"Menu_Machine_Brick" );				// 살펴보기
#ifdef ADDED_RELATIONSHIP_SYSTEM
	pUserMenu[21] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"couple" );					// 커플 신청
	pUserMenu[22] = (CKTDGUIButton*) m_pDLGUserMenu->GetControl( L"summon" );					// 커플 신청
#endif // ADDED_RELATIONSHIP_SYSTEMㄴ

	for(int i=0; i<USER_MENU_MAX; ++i)
	{		
		if(pUserMenu[i] != NULL)
			pUserMenu[i]->SetShowEnable(false, false);
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUserMenu, true );
	m_pDLGUserMenu->SetShowEnable( true, true );

	countMenuHeight = 0;
	pUserMenu[4]->SetShowEnable(true, true);
	pUserMenu[4]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
	++countMenuHeight;
	pUserMenu[11]->SetShowEnable(true, true);
	pUserMenu[11]->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHegith * countMenuHeight));
	++countMenuHeight;

	m_pPicMiddle1->SetSizeY(countUserMenu * menuHegith);
	m_pPicMiddle2->SetSizeY(countUserMenu * menuHegith);

	D3DXVECTOR2 offsetPos;
	offsetPos = m_pPicMiddle1->GetPos();
	offsetPos.y += (countUserMenu * menuHegith);
	m_pPicBottom1->SetPos(offsetPos);

	offsetPos = m_pPicMiddle2->GetPos();
	offsetPos.y += (countUserMenu * menuHegith);
	m_pPicBottom2->SetPos(offsetPos);

	D3DXVECTOR2 vMousePos;
	vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
	vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

	m_pDLGUserMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y-30.f ) );


	return true;
}

bool CX2Community::Handler_EGS_KNM_INVITE_REQ( wstring nickName )
{
	if ( g_pX2Game != NULL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_102 ), m_pStage );
		return true;
	}

	KEGS_KNM_INVITE_REQ kPacket;
	kPacket = nickName;

	g_pData->GetServerProtocol()->SendPacket( EGS_KNM_INVITE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_KNM_INVITE_ACK );

	return true;
}

void CX2Community::AlarmReceiveMessage( const WCHAR* wszMessage )
{
	if ( wszMessage == NULL )
		return;

	m_pTalkBoxManager->Clear();

	CX2TalkBoxManagerImp::TalkBox talkBox;


	talkBox.m_vPos				= D3DXVECTOR2( 936,702 );
	talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
	talkBox.m_fRemainTime		= 3.0f;

	talkBox.m_wstrTalkContent	= wszMessage;
	talkBox.m_bTraceUnit		= false;
	talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_RIGHT;
	talkBox.m_coTextColor		= D3DXCOLOR(0,0,0,1);

	if( m_pSoundAlarm != NULL )
		m_pSoundAlarm->Play(false, false);
	//g_pKTDXApp->GetDeviceManager()->PlaySound( L"MessengerAlarm.ogg", false, false );

	m_pTalkBoxManager->Push( talkBox );
}

bool CX2Community::TryTalk( CX2Community::MessengerUser* pMessengerUser )
{
	if ( pMessengerUser == NULL )
		return false;
#ifdef NEW_MESSENGER
	if( pMessengerUser->IsOnline() )
	{
		SetOpenChatSession( pMessengerUser->m_UnitUID, pMessengerUser->m_NickName );
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_104 ), m_pStage );
	}

#else NEW_MESSENGER
	if ( IS_ONLINE( pMessengerUser->m_uStatus ) )
	{
		if ( CNMCOClientObject::GetInstance().RequestChatSession( pMessengerUser->m_NMFriendKey ) == FALSE )
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_103 ), m_pStage );
		else
		{
			m_bRequestChat = true;
			m_RequestedNMFriendKey = pMessengerUser->m_NMFriendKey;
		}
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_104 ), m_pStage );
	}
#endif NEW_MESSENGER

	return true;

}


bool CX2Community::CheckSendMsgByChatbox()
{
	bool bCheckChatMsg = false;		//alarm by chat msg 

	if ( g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true )
		bCheckChatMsg = true;

	if ( g_pX2Game != NULL )
		bCheckChatMsg = true;

	return bCheckChatMsg;
}

UidType CX2Community::GetFriendUID(wstring &nickName)
{
	std::map<wstring, UidType>::iterator mit;
	mit = m_mapFriendUid.find( nickName );

	if( mit != m_mapFriendUid.end() )
	{
		return mit->second;
	}
	else
	{
		return 0;
	}

	return 0;
}

#ifdef NEW_MESSENGER
void CX2Community::MessengerUser::Update( const KFriendInfo& friendInfo, bool &bRefresh )
{
	m_UnitUID = friendInfo.m_iUnitUID;
	m_NickName = CX2Community::ConvertOrgName( friendInfo.m_wstrNickName.c_str() );
	m_uCategoryCode = (int)friendInfo.m_cGroupID;
	m_eStatus = (FRIEND_STATUS)friendInfo.m_cStatus;
	m_ePosition = (FRIEND_POSITION)friendInfo.m_cPosition;

	// m_bBlocked = IS_BLOCKED( friendInfo.uFlag );			// m_eStatus에 포함

// 	if ( m_bLogin == false && friendInfo.bIsVirtualOnline == TRUE && bRefresh == true )
// 	{		
// 		wstringstream wstrmText;
// 		wstrmText << GET_REPLACED_STRING( ( STR_ID_105, "L", m_NickName ) );
// 
// 		if ( g_pData->GetMessenger() != NULL && g_pData->GetMessenger()->CheckSendMsgByChatbox() == false )
// 		{
// 			if ( g_pData->GetMessenger() != NULL )
// 				g_pData->GetMessenger()->AlarmReceiveMessage( wstrmText.str().c_str() );
// 		}
// 		else
// 		{
// 			if( NULL != g_pChatBox )
// 			{
// 				g_pChatBox->AddChatLog( wstrmText.str().c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
// 			}
// 		}
// 
// 		bRefresh = false;
// 	}

//	m_bLogin = friendInfo.bIsVirtualOnline;

//	m_uFlag = friendInfo.uFlag;
//	m_uStatus = friendInfo.uStatus;

}
#else NEW_MESSENGER
void CX2Community::MessengerUser::Update( const CNMFriendInfo& friendInfo, bool &bRefresh )
{
	m_NickName = CX2Community::ConvertOrgName( friendInfo.szNickName );
	m_Position = friendInfo.szPosition;

	m_NMFriendKey = friendInfo.keyFriend;
	
	m_bBlocked = IS_BLOCKED( friendInfo.uFlag );
	
	if(g_pData != NULL && g_pData->GetMessenger() != NULL)
		m_UnitUID = g_pData->GetMessenger()->GetFriendUID(m_NickName);
	else
		m_UnitUID = 0;

	if ( FALSE == m_bLogin && TRUE == friendInfo.bIsVirtualOnline && true == bRefresh )
	{		
		wstringstream wstrmText;
		wstrmText << GET_REPLACED_STRING( ( STR_ID_105, "L", m_NickName ) );

		if ( g_pData->GetMessenger() != NULL && g_pData->GetMessenger()->CheckSendMsgByChatbox() == false )
		{
			if ( g_pData->GetMessenger() != NULL )
				g_pData->GetMessenger()->AlarmReceiveMessage( wstrmText.str().c_str() );
		}
		else
		{
			if( NULL != g_pChatBox )
			{
				g_pChatBox->AddChatLog( wstrmText.str().c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
		}

		bRefresh = false;
	}

	m_bLogin = friendInfo.bIsVirtualOnline;

	m_uFlag = friendInfo.uFlag;
	m_uStatus = friendInfo.uStatus;
/*
	m_NMFriendKey;
	m_uCategoryCode;

	m_UnitUID;
	m_NickName;
	m_bLogin;
	m_Position;
	m_Level;

	m_bBlocked;
*/
}
#endif


// 스페이스로만 이루어진 문자열인지 검사
bool CX2Community::IsOnlySpaceString( const wstring wstrText )
{
	if( true == wstrText.empty() )
		return true;

	for( int i=0; i< (int) wstrText.length(); i++ )
	{
		if( wstrText[i] != ' ' )
			return false;
	}

	return true;
}

#ifdef SERV_ED_MONITORING_IN_GAME
bool CX2Community::Handler_EGS_COMMUNITY_USER_LIST_MONITORING_MODE_REQ(int iPage)
{
	if(m_bUpdateFieldUser == true)
		return true;

	m_bUpdateFieldUser = true;
	KEGS_COMMUNITY_USER_LIST_MONITORING_MODE_REQ kPacket;
	kPacket.m_uiViewPage = iPage;
	kPacket.m_vecUnitUIDList.clear();

	for(UINT i=0; i<m_vecFieldUser.size(); ++i)
	{
		KCommunityUserInfo &fieldUser = m_vecFieldUser[i];
		kPacket.m_vecUnitUIDList.push_back( fieldUser.m_iUnitUID );
	}	

	g_pData->GetServerProtocol()->SendPacket( EGS_COMMUNITY_USER_LIST_MONITORING_MODE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_COMMUNITY_USER_LIST_ACK );

	return true;
}
#endif

bool CX2Community::Handler_EGS_COMMUNITY_USER_LIST_REQ(int iPage)
{	

#ifdef SERV_ED_MONITORING_IN_GAME
	if( g_pInstanceData != NULL && g_pInstanceData->GetMoneyMonitoring() == true &&
		g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
	{
		return Handler_EGS_COMMUNITY_USER_LIST_MONITORING_MODE_REQ(iPage);
	}
#endif

	if(m_bUpdateFieldUser == true)
		return true;

	m_bUpdateFieldUser = true;
	KEGS_COMMUNITY_USER_LIST_REQ kPacket;
	kPacket.m_uiViewPage = iPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_COMMUNITY_USER_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_COMMUNITY_USER_LIST_ACK );

	return true;
}

bool CX2Community::Handler_EGS_COMMUNITY_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_COMMUNITY_USER_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bUpdateFieldUser = false;

	if( g_pMain->DeleteServerPacket( EGS_COMMUNITY_USER_LIST_ACK ) == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGFriendReqWait );
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if(m_MessengerUserTab == XMUT_FIELD)
			{
				m_vecFieldUser.clear();
				m_vecFieldUser = kEvent.m_vecUserList;

				m_iMaxFieldPage = kEvent.m_uiTotalPage;
				m_iNowFieldPage = kEvent.m_uiViewPage;

				UpdateUserList(m_iNowFieldPage);			
				return true;
			}			
		}	
	}

	return false;
}

void CX2Community::CheckButton(CKTDGUIButton* pButton)
{	
	int idummy			= 0;
	
	if(pButton != NULL)
	{
		idummy = pButton->GetDummyInt(0);

		if(idummy > 0 && (int)m_vecUserUID.size() >= idummy)
		{
			m_iPickedUserIndex = idummy;
			m_iPickedUserUID = m_vecUserUID[idummy-1];
		}
	}
	else
	{
		m_iPickedUserIndex = 0;
		m_iPickedUserUID = 0;
	}

	WCHAR wszText[64] = L"";
	int maxSlot = 0;
	switch(m_MessengerUserTab)
	{
	case XMUT_FIELD:
		{
			maxSlot = 8;
	
			for(int i=0; i<maxSlot; ++i)
			{
				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"User%d", i+1 );
				CKTDGUIButton* pUserButton = (CKTDGUIButton*) m_pDlgField->GetControl( wszText );
				pUserButton->SetDownStateAtNormal(false);
			}
			
			if(pButton != NULL)
				pButton->SetDownStateAtNormal(true);
		}
		
		break;
	case XMUT_FRIEND:
		break;
	case XMUT_GUILD:
		//{{ kimhc // 2009-09-17 // 길드탭에서 유저리스트 클릭 처리
#ifdef	GUILD_MANAGEMENT
		{
			maxSlot = 8;

			for(int i=0; i<maxSlot; ++i)
			{
				swprintf( wszText, sizeof(wszText) / sizeof(WCHAR)-1, L"Guild_Rank%d", i+1 );
				CKTDGUIButton* pUserButton = static_cast< CKTDGUIButton* >( m_pDlgGuild->GetControl( wszText ) );
				pUserButton->SetDownStateAtNormal(false);
			}

			for(int i=0; i<maxSlot; ++i)
			{
				swprintf( wszText, sizeof(wszText) / sizeof(WCHAR)-1, L"User%d", i+1 );
				CKTDGUIButton* pUserButton = static_cast< CKTDGUIButton* >( m_pDlgGuild->GetControl( wszText ) );
				pUserButton->SetDownStateAtNormal(false);
			}

			if(pButton != NULL)
				pButton->SetDownStateAtNormal(true);
		}
		break;
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-17
		
	case XMUT_DISCIPLE:
		{
			maxSlot = 3;
						
			for(int i=0; i<maxSlot; ++i)
			{
				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"Square_User_List%d", i );
				CKTDGUIButton* pUserButton = (CKTDGUIButton*) m_pDlgDisciple->GetControl( wszText );
				pUserButton->SetDownStateAtNormal(false);
			}
			
			if(pButton != NULL)
				pButton->SetDownStateAtNormal(true);
		}
		break;

#ifdef SERV_RECRUIT_EVENT_BASE
	case XMUT_RECRUIT:
		{
			if ( m_pDlgRecruit != NULL )
			{
				maxSlot = 8;

				for(int i=0; i<maxSlot; ++i)
				{
					swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"User%d", i+1 );
					CKTDGUIButton* pUserButton = (CKTDGUIButton*) m_pDlgRecruit->GetControl( wszText );
					pUserButton->SetDownStateAtNormal(false);
				}

				if(pButton != NULL)
					pButton->SetDownStateAtNormal(true);
			}
		} break;
#endif SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	case XMUT_REFEREE:
		{
			if ( m_pDlgReferee != NULL )
			{
				maxSlot = 8;

				for(int i=0; i<maxSlot; ++i)
				{
					swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"User%d", i+1 );
					CKTDGUIButton* pUserButton = (CKTDGUIButton*) m_pDlgReferee->GetControl( wszText );
					pUserButton->SetDownStateAtNormal(false);
				}

				if(pButton != NULL)
					pButton->SetDownStateAtNormal(true);
			}
		}
		break;
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case XMUT_RELATION:
		{
			if ( NULL != g_pData->GetRelationshipManager() &&
 				 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
				if ( pRelationshipInfo->m_eRelationStateType != SEnum::RT_SOLO )
				{
					if ( m_pDlgRelation != NULL )
					{
						for(int i=0; i<_CONST_RELATIONSHIP_UI_::g_iMaxRelationMemberNumber; ++i)
						{
							swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"User%d", i+1 );
							CKTDGUIButton* pUserButton = static_cast<CKTDGUIButton*> ( m_pDlgRelation->GetControl( wszText ) );
							pUserButton->SetDownStateAtNormal(false);
						}

						if(pButton != NULL)
							pButton->SetDownStateAtNormal(true);
					}
				}
			}
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	}
}

void CX2Community::UpdateFieldUserList(int iPage)
{
	const int MAGIC_MAX_USER_PER_PAGE = 8;
	WCHAR wszText[64] = L"";	

	m_vecUserLv.clear();
	m_vecUserUID.clear();
	for(UINT i=0; i<m_vecFieldUser.size(); ++i)
	{
		m_vecUserUID.push_back(m_vecFieldUser[i].m_iUnitUID);
		m_vecUserLv.push_back(m_vecFieldUser[i].m_ucLevel);
	}

	for( UINT i=0; i<MAGIC_MAX_USER_PER_PAGE; ++i )
	{
		if( m_pFieldUserIcon[i] != NULL)
			m_pFieldUserIcon[i]->SetShowEnable(true, true);
	
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"User%d", i+1 );
		CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDlgField->GetControl( wszText );
		if( NULL != pButton )
		{
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"name%d", i+1 );
			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*) m_pDlgField->GetControl( wszText );

			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"state%d", i+1 );
			CKTDGUIStatic* pStaticState = (CKTDGUIStatic*) m_pDlgField->GetControl( wszText );

			if(i < m_vecFieldUser.size())
			{
				KCommunityUserInfo userNode = m_vecFieldUser[i];

				pButton->SetShowEnable(true, true);

#ifdef SERV_ED_MONITORING_IN_GAME
				if( g_pInstanceData != NULL && g_pInstanceData->GetMoneyMonitoring() == true &&
					g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
				{
					WCHAR wBuff[256] = {0,};
					StringCchPrintf(wBuff, 256, L"%s (%s)", userNode.m_wstrNickName.c_str(), g_pMain->GetEDString(userNode.m_iED).c_str() );
					pStaticName->GetString(0)->msg = wBuff;
				}
				else
				{
					pStaticName->GetString(0)->msg = userNode.m_wstrNickName;
				}				
#else
				pStaticName->GetString(0)->msg = userNode.m_wstrNickName;
#endif
				
				pStaticName->SetShowEnable(true, true);				

				if(g_pMain->GetNowStateID() == CX2Main::XS_PVP_LOBBY || g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM)
				{
					// 대전로비/룸일때 유저 리스트
					switch(userNode.m_cState)
					{
					case KCommunityUserInfo::US_PVP_LOBBY:
						pStaticState->GetString(0)->msg = GET_STRING( STR_ID_2542 );
						pStaticState->SetShowEnable(true, true);
						break;
					case KCommunityUserInfo::US_PVP_WAIT:
						pStaticState->GetString(0)->msg = GET_STRING( STR_ID_2543 );
						pStaticState->SetShowEnable(true, true);
						break;
					case KCommunityUserInfo::US_PVP_PLAY:
						pStaticState->GetString(0)->msg = GET_STRING( STR_ID_2544 );
						pStaticState->SetShowEnable(true, true);
						break;
					default:
						pStaticState->SetShowEnable(false, false);
						break;
					}
				}
				else
				{
					// 그외(필드등)에서의 유저 리스트
					if(userNode.GetPartyUID() > 0)
					{							
						pStaticState->GetString(0)->msg = GET_STRING( STR_ID_106 );
						pStaticState->SetShowEnable(true, true);
					}
					else
					{
						pStaticState->SetShowEnable(false, false);
					}
				}
				

				

				SetFieldUserIcon(i, true);
			}
			else
			{
				pButton->SetShowEnable(false, false);
				pStaticName->SetShowEnable(false, false);
				pStaticState->SetShowEnable(false, false);

				SetFieldUserIcon(i, false);
			}
		}
	}

	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d/%d", m_iNowFieldPage, m_iMaxFieldPage );
	CKTDGUIStatic* pStaticState = (CKTDGUIStatic*) m_pDlgField->GetControl( L"page_number" );
	if(pStaticState != NULL)
		pStaticState->GetString(0)->msg = wszText;

}

void CX2Community::UpdateMarketUserList(int iPage)
{
	if( NULL == g_pSquareGame )
	return; 

	const int MAGIC_MAX_USER_PER_PAGE = 8;
	WCHAR wszText[64] = L"";

	// 거래광장 유저리스트 등록	
	m_iNowFieldPage = iPage;
	g_pSquareGame->GetUnitPageInfo(m_iMaxFieldPage, MAGIC_MAX_USER_PER_PAGE);

	if(m_iNowFieldPage > m_iMaxFieldPage)
		m_iNowFieldPage = 1;
	if(m_iNowFieldPage < 1)
		m_iNowFieldPage = m_iMaxFieldPage;

	vector<CX2SquareUnit*>& vecUnitList = g_pSquareGame->GetUnitList( m_iNowFieldPage, MAGIC_MAX_USER_PER_PAGE );
	
	m_vecUserLv.clear();
	m_vecUserUID.clear();

	m_vecFieldUser.clear();
	for( UINT i=0; i<vecUnitList.size(); i++ )
	{		
		KCommunityUserInfo userNode;
		CX2SquareUnit* pCX2SquareUnit = vecUnitList[i];
		CX2Unit* pUnit = pCX2SquareUnit->GetUnit();

		userNode.m_iUnitUID = pUnit->GetUnitData().m_UnitUID;
		userNode.m_wstrNickName = pUnit->GetUnitData().m_NickName;
		userNode.m_cUnitClass = pUnit->GetUnitData().m_UnitClass;
		userNode.m_ucLevel = pUnit->GetUnitData().m_Level;

		// 거래광장에서는 m_bIsPartyUser를 상점개설여부로 사용한다.
		userNode.m_iStateCode = 0; 
		if ( pCX2SquareUnit != NULL && pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_SHOP )
		{
			userNode.m_iStateCode = 1;
		}

#ifdef SERV_ED_MONITORING_IN_GAME
		userNode.m_iED = pCX2SquareUnit->GetED();
#endif

		m_vecUserUID.push_back( pUnit->GetUnitData().m_UnitUID );
		m_vecUserLv.push_back( pUnit->GetUnitData().m_Level );

		m_vecFieldUser.push_back(userNode);		
	}

	// 거래광장 유저리스트 출력
	CX2SquareUnit* pCX2SquareUnit = NULL;
	for( UINT i=0; i<MAGIC_MAX_USER_PER_PAGE; ++i )
	{
		if(i < vecUnitList.size())
			pCX2SquareUnit = vecUnitList[i];

		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"User%d", i+1 );
		CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDlgField->GetControl( wszText );
		if( NULL != pButton )
		{
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"name%d", i+1 );
			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*) m_pDlgField->GetControl( wszText );
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"state%d", i+1 );
			CKTDGUIStatic* pStaticState = (CKTDGUIStatic*) m_pDlgField->GetControl( wszText );

			if(i < m_vecFieldUser.size() && pCX2SquareUnit != NULL)
			{
				pButton->SetShowEnable(true, true);

#ifdef SERV_ED_MONITORING_IN_GAME
				if( g_pInstanceData != NULL && g_pInstanceData->GetMoneyMonitoring() == true &&
					g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
				{					
					WCHAR wBuff[256] = {0,};
					StringCchPrintf(wBuff, 256, L"%s (%s)", m_vecFieldUser[i].m_wstrNickName.c_str(), g_pMain->GetEDString(m_vecFieldUser[i].m_iED).c_str() );
					pStaticName->GetString(0)->msg = wBuff;
				}
				else
				{
					pStaticName->GetString(0)->msg = m_vecFieldUser[i].m_wstrNickName;
				}				
#else
				pStaticName->GetString(0)->msg = m_vecFieldUser[i].m_wstrNickName;
#endif
				pStaticName->SetShowEnable(true, true);

				if ( pCX2SquareUnit != NULL && pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_SHOP )
				{
					pStaticState->GetString(0)->msg = GET_STRING( STR_ID_107 );
					pStaticState->SetShowEnable(true, true);
				}
				else if ( pCX2SquareUnit != NULL && pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_WAIT )
				{
					pStaticState->GetString(0)->msg = GET_STRING( STR_ID_108 );
					pStaticState->SetShowEnable(true, true);
				}
				else
				{
					pStaticState->SetShowEnable(false, false);
				}

				SetFieldUserIcon(i, true);
			}
			else
			{
				pButton->SetShowEnable(false, false);
				pStaticName->SetShowEnable(false, false);
				pStaticState->SetShowEnable(false, false);

				SetFieldUserIcon(i, false);
			}
		}
	}

	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d/%d", m_iNowFieldPage, m_iMaxFieldPage );
	CKTDGUIStatic* pStaticState = (CKTDGUIStatic*) m_pDlgField->GetControl( L"page_number" );
	if(pStaticState != NULL)
		pStaticState->GetString(0)->msg = wszText;
}

void CX2Community::UpdateDiscipleUserList()
{
	if( g_pMain->GetTutorSystem() == NULL )
	return;			

	const int MAGIC_MAX_USER_PER_PAGE = 3;
	WCHAR wszText[64] = L"";
	int nNumPartyUser = g_pMain->GetTutorSystem()->GetNumberOfStudent();			

	m_vecUserLv.clear();
	m_vecUserUID.clear();

	// page 번호
	int iMaxUserListPage = 1;
	int iNowUserListPage = 1;

	const map< UidType, CX2TutorSystem::TutorUnit >& mapTutorUnit = g_pMain->GetTutorSystem()->GetMapTutorStudent();
	map< UidType, CX2TutorSystem::TutorUnit >::const_iterator it_tutor = mapTutorUnit.begin();

	for( UINT i=0; i<MAGIC_MAX_USER_PER_PAGE; ++i )
	{
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"Square_User_List%d", i );
		CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDlgDisciple->GetControl( wszText );
		if( NULL != pButton )
		{
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"name%d", i+1 );
			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*) m_pDlgDisciple->GetControl( wszText );
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"state%d", i+1 );
			CKTDGUIStatic* pStaticState = (CKTDGUIStatic*) m_pDlgDisciple->GetControl( wszText );
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"User%d_lv", i+1 );
			CKTDGUIStatic* pStaticLv = (CKTDGUIStatic*) m_pDlgDisciple->GetControl( wszText );			


			if( nNumPartyUser > 0 && it_tutor != mapTutorUnit.end() )
			{
				const CX2TutorSystem::TutorUnit& tutorUnit = it_tutor->second;

				pButton->SetShow(true);
				pButton->SetEnable(true);
				pStaticName->SetShowEnable(true, true);
				pStaticState->SetShowEnable(true, true);
				pStaticLv->SetShowEnable(true, true);


				pStaticName->GetString(0)->msg = tutorUnit.m_wstrNickName;


				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"Lv.%d", tutorUnit.m_iLevel );
				pStaticLv->GetString(0)->msg = wszText;


				if( true == tutorUnit.m_bIsOnline )
				{
					swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, GET_STRING( STR_ID_98 ) );
				}
				else
				{
					swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, GET_STRING( STR_ID_99 ) );
				}
				pStaticState->GetString(0)->msg = wszText;

				m_vecUserUID.push_back( tutorUnit.m_UnitUID );
				m_vecUserLv.push_back( tutorUnit.m_iLevel );

				it_tutor++;
			}
			else
			{
				pButton->SetShow(false);
				pButton->SetEnable(false);
				pStaticName->SetShowEnable(false, false);
				pStaticState->SetShowEnable(false, false);
				pStaticLv->SetShowEnable(false, false);
			}
		}
	}
}

//{{ kimhc // 2009-09-17 // 길드 UI
#ifdef	GUILD_MANAGEMENT
void CX2Community::InitGuildUI()
{
	if ( m_pDlgGuild != NULL )
	{
		for ( int i = 0; i < _CONST_COMMUNITY_UI_::g_byNumOfGuildMemberPerOnePage; i++ )
		{
			// 닉네임
			m_GuildUserListUIConstrols[i].m_pStaticNickName		= 
				static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"name" ), i + 1 ) ) ) );
			if( NULL != m_GuildUserListUIConstrols[i].m_pStaticNickName )
			{
				m_GuildUserListUIConstrols[i].m_pStaticNickName->SetShowEnable( false, false );
			}

			m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan	=
				static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"state" ), i + 1 ) ) ) );
			if( NULL != m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan )
			{
				m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetShowEnable( false, false );
			}

			m_GuildUserListUIConstrols[i].m_pStaticChannel	=
				static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"channel" ), i + 1 ) ) ) );
			if( NULL != m_GuildUserListUIConstrols[i].m_pStaticChannel )
			{
				m_GuildUserListUIConstrols[i].m_pStaticChannel->SetShowEnable( false, false );
			}

			m_GuildUserListUIConstrols[i].m_pButtonNickName		=
				static_cast< CKTDGUIButton* >( m_pDlgGuild->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"User" ), i + 1 ) ) ) );
			if( NULL != m_GuildUserListUIConstrols[i].m_pButtonNickName )
			{
				m_GuildUserListUIConstrols[i].m_pButtonNickName->SetShowEnable( false, false );
			}

			m_GuildUserListUIConstrols[i].m_pButtonGrade		=
				static_cast< CKTDGUIButton* >( m_pDlgGuild->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Guild_Rank" ), i + 1 ) ) ) );
			if( NULL != m_GuildUserListUIConstrols[i].m_pButtonGrade )
			{
				m_GuildUserListUIConstrols[i].m_pButtonGrade->SetShowEnable( false, false );
			}
#ifdef INTEGRATE_TOOLTIP
			m_GuildUserListUIConstrols[i].m_pButtonToolTip		=
				static_cast< CKTDGUIButton* >( m_pDlgGuild->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"GuildMemberLocaTip" ), i + 1 ) ) ) );
			if( NULL != m_GuildUserListUIConstrols[i].m_pButtonToolTip )
			{
				m_GuildUserListUIConstrols[i].m_pButtonToolTip->SetShowEnable( false, false );
			}
#endif INTEGRATE_TOOLTIP
		}

	
		CKTDGUIStatic* pPageStatic		= static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( L"page_number" ) );
		CKTDGUIStatic* pNumStatic		= static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( L"numOfGuildMember" ) );

		if ( pPageStatic != NULL )
			pPageStatic->SetShow( false );

		if ( pNumStatic != NULL )
			pNumStatic->SetShow( false );
	}

	UpdateGuildName();
	UpdateInfoTypeString();
}

void CX2Community::ResetGuildTabUI()
{
	if ( g_pData->GetGuildManager() == NULL )
	{
		ASSERT( !"Wrong Path" );
		return;
	}

	CX2GuildManager*	pGuildManager		= g_pData->GetGuildManager();
	m_eNowGuildViewInfo						= CX2Community::GVI_LOCATION;
	m_uiNowGuildPage						= 1;	

	CKTDGUIButton*			pButtonAddMember		= m_pDlgGuild->GetButton_LUA( "Button_Guild_Member_add" );
	CKTDGUIButton*			pButtonGuildInfo		= m_pDlgGuild->GetButton_LUA( "GuildInfo" );
	CKTDGUIRadioButton*		pRadioButtonLocation	= static_cast< CKTDGUIRadioButton* >( m_pDlgGuild->GetControl( L"RadioButton_LOCATION" ) );
	CKTDGUIRadioButton*		pRadioButtonGrade		= static_cast< CKTDGUIRadioButton* >( m_pDlgGuild->GetControl( L"RadioButton_GRADE" ) );

	if ( pGuildManager->DidJoinGuild() == true )
	{
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_PVP_GAME:
			{
				pButtonAddMember->SetEnable( false );
			}
			break;

		default:
			{
				pButtonAddMember->SetEnable( true );
			}
			break;
		}		

		pButtonGuildInfo->SetEnable( true );
		pRadioButtonLocation->SetEnable( true );
		pRadioButtonGrade->SetEnable( true );

		pRadioButtonLocation->SetChecked( true );

		Handler_EGS_GET_GUILD_USER_LIST_REQ();
	}
	else
	{
		pButtonAddMember->SetEnable( false );
		pButtonGuildInfo->SetEnable( false );
		pRadioButtonLocation->SetEnable( false );
		pRadioButtonGrade->SetEnable( false );
		pRadioButtonLocation->SetChecked( false );
		pRadioButtonGrade->SetChecked( false );

		InitGuildUI();
	}	
}

void CX2Community::OnGuildTabUIButton()
{
	CKTDGUIButton*			pButtonAddMember		= m_pDlgGuild->GetButton_LUA( "Button_Guild_Member_add" );
	CKTDGUIButton*			pButtonGuildInfo		= m_pDlgGuild->GetButton_LUA( "GuildInfo" );
	CKTDGUIRadioButton*		pRadioButtonLocation	= static_cast< CKTDGUIRadioButton* >( m_pDlgGuild->GetControl( L"RadioButton_LOCATION" ) );
	CKTDGUIRadioButton*		pRadioButtonGrade		= static_cast< CKTDGUIRadioButton* >( m_pDlgGuild->GetControl( L"RadioButton_GRADE" ) );

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
		{
			pButtonAddMember->SetEnable( false );
		}
		break;

	default:
		{
			pButtonAddMember->SetEnable( true );
		}
		break;
	}		

	pButtonGuildInfo->SetEnable( true );
	pRadioButtonLocation->SetEnable( true );
	pRadioButtonGrade->SetEnable( true );

	pRadioButtonLocation->SetChecked( true );
}

void CX2Community::UpdateGuildTabUI()
{
	UpdateGuildName();
	UpdateInfoTypeString();
	UpdateGuildTabPageUI();
	UpdateNumOfGuildMembers();
}

void CX2Community::InitGuildTabListUI()
{
	const D3DXCOLOR defaultColor( 0.0f, 0.0f, 0.0f, 1.0f );

	for ( int i = 0; i < _CONST_COMMUNITY_UI_::g_byNumOfGuildMemberPerOnePage; i++ )
	{
		m_GuildUserListUIConstrols[i].m_pStaticNickName->GetString( 0 )->color = defaultColor;
		m_GuildUserListUIConstrols[i].m_pStaticNickName->SetShowEnable( false, false );		
		m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetShowEnable( false, false );
		m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->GetString( 0 )->color = defaultColor;
		m_GuildUserListUIConstrols[i].m_pStaticChannel->SetShowEnable( false, false );
		m_GuildUserListUIConstrols[i].m_pButtonNickName->SetShowEnable( false, false );
		m_GuildUserListUIConstrols[i].m_pButtonGrade->SetShowEnable( false, false );
#ifdef INTEGRATE_TOOLTIP
		m_GuildUserListUIConstrols[i].m_pButtonToolTip->SetShowEnable( false, false );
#endif INTEGRATE_TOOLTIP

		if ( m_pFieldUserIcon[i] != NULL )
			m_pFieldUserIcon[i]->SetShowEnable( false, false );
	}
}

void CX2Community::UpdateGuildName()
{
	if ( m_pDlgGuild != NULL )
	{
		CKTDGUIStatic* pStatic = static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( L"Guild_name1" ) );

		if ( pStatic != NULL )
		{
			wstring wstrGuildName; 

			// 길드에 가입되어 있으면
			if ( g_pData->GetGuildManager() != NULL && g_pData->GetGuildManager()->DidJoinGuild() == true )
			{
				wstrGuildName = g_pData->GetGuildManager()->GetDisplayMyGuildName();
			}
			else
			{
				wstrGuildName = L"";
			}

			pStatic->SetString( 0, wstrGuildName.c_str() );			
			pStatic->SetString( 1, wstrGuildName.c_str() );			
		}
	}
}

void CX2Community::UpdateInfoTypeString()
{
	// 길드명
	if ( m_pDlgGuild != NULL )
	{
		CKTDGUIStatic* pStaticGuildInfoType	= static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( L"GuildInfoType" ) );

		switch ( m_eNowGuildViewInfo )
		{
		case CX2Community::GVI_LOCATION:
			{

				if ( pStaticGuildInfoType != NULL )
				{
					pStaticGuildInfoType->GetPicture( 1 )->SetShow( true );		// 위치라는 글씨 Texture
					pStaticGuildInfoType->GetPicture( 2 )->SetShow( false );	// 등급이라는 글씨 Texture
				}
				

			}
			break;

		case CX2Community::GVI_MEMBER_GRADE:
			{

				if ( pStaticGuildInfoType != NULL )
				{
					pStaticGuildInfoType->GetPicture( 1 )->SetShow( false );
					pStaticGuildInfoType->GetPicture( 2 )->SetShow( true );
				}
			}
			break;

		default:
			break;
		}
	}

	// 버튼 스트링
	// 실제 보일 데이타
	// 페이지
	// 접속인원/최대인원

}

void CX2Community::UpdateGuildTabList()
{
	int i = 0;
	const D3DXCOLOR colorForLogOff( 0.8f, 0.8f, 0.8f, 1.0f );

	InitGuildTabListUI();

	CX2GuildManager* pGulldManager	= NULL;
	pGulldManager	= g_pData->GetGuildManager();

	if ( pGulldManager == NULL )
		return;

	switch ( m_eNowGuildViewInfo )
	{
	case CX2Community::GVI_LOCATION:
		{
			UpdateGuildPositionInfo();
		}
		break;

	case CX2Community::GVI_MEMBER_GRADE:
		{
			UpdateGuildGradeInfo();
		}
		break;
	}
	
}

void CX2Community::UpdateGuildPositionInfo()
{
	int	i = 0;
	const D3DXCOLOR colorForLogOff( 0.57f, 0.57f, 0.57f, 1.0f );

	CX2GuildManager* pGulldManager	= NULL;
	pGulldManager	= g_pData->GetGuildManager();

	if ( pGulldManager == NULL )
		return;

	BOOST_TEST_FOREACH( const KGuildMemberInfo&, kGuildMemberInfo, m_vecGuildMemberInfo )
	{
		// 닉네임
		m_GuildUserListUIConstrols[i].m_pStaticNickName->SetString( 0, kGuildMemberInfo.m_wstrNickName.c_str() );
		m_vecUserUID.push_back( kGuildMemberInfo.m_iUnitUID );

		if ( kGuildMemberInfo.m_tLogOutTime == 0 ) // 접속중
		{
			switch ( kGuildMemberInfo.m_cPosition )
			{
			case CX2Room::RT_PVP:
				{
					m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0,
						GET_STRING( STR_ID_4522 ) );
				}
				break;

			case CX2Room::RT_DUNGEON:
				{
					m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0,
						GET_STRING( STR_ID_2546 ) );
				}
				break;

			case CX2Room::RT_SQUARE:
				{
					m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0,
						GET_STRING( STR_ID_4521 ) );
				}
				break;
				
			case CX2Room::RT_BATTLE_FIELD:
				{
					SEnum::VILLAGE_MAP_ID eMapID = static_cast< SEnum::VILLAGE_MAP_ID >( kGuildMemberInfo.m_iMapID );
					const WCHAR* pwszBattleFieldName = g_pData->GetBattleFieldManager().GetBattleFieldNameByBattleFieldId( static_cast<int>( eMapID ) );

					if ( NULL != pwszBattleFieldName )
					{
						wstring wstrBattleFieldName = pwszBattleFieldName;
						if ( wstrBattleFieldName.size() > 4 )	/// 글자 사이즈를 4개로 제한
						{
							wstrBattleFieldName.resize( 4 );
							wstrBattleFieldName += L"..";
						}

						m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0, 
							wstrBattleFieldName.c_str() );
					}
				} break;

			default:	// 마을
				{
					SEnum::VILLAGE_MAP_ID eMapID = static_cast< SEnum::VILLAGE_MAP_ID >( kGuildMemberInfo.m_iMapID );
					if( g_pData->GetLocationManager()->IsVillage( eMapID ) == true ||
						g_pData->GetLocationManager()->IsDungeonGate( eMapID ) == true )
					{
						CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eMapID );
						if( NULL != pVillageTemplet )
						{
#ifdef INTEGRATE_TOOLTIP
							bool bEllipse = false;
							wstring wstrName = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(pVillageTemplet->m_Name.c_str(), 100, m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->GetString(0)->fontIndex, 1, bEllipse);

							m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0, 
								wstrName.c_str() );

							if(bEllipse == true)
							{
								m_GuildUserListUIConstrols[i].m_pButtonToolTip->SetGuideDesc(pVillageTemplet->m_Name.c_str());
								m_GuildUserListUIConstrols[i].m_pButtonToolTip->SetShowEnable(true, true);
							}
							else
							{
								m_GuildUserListUIConstrols[i].m_pButtonToolTip->SetShowEnable(false, false);
							}
#else
							m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0, 
								pVillageTemplet->m_Name.c_str() );
#endif INTEGRATE_TOOLTIP
						}
					}
				}
				break;
			}

			// 채널
			m_GuildUserListUIConstrols[i].m_pStaticChannel->SetString( 0, 
				GET_REPLACED_STRING( ( STR_ID_4523, "L", g_pInstanceData->GetChannelServerName( kGuildMemberInfo.m_iChannelID ) ) ) );
			m_GuildUserListUIConstrols[i].m_pStaticChannel->GetString( 0 )->color = 
				GetChannelColor( kGuildMemberInfo.m_iChannelID, g_pInstanceData->GetServerGroupID() );

			m_GuildUserListUIConstrols[i].m_pStaticChannel->SetShow( true );
			
			SetGuildMemberNickNameColor( kGuildMemberInfo, m_GuildUserListUIConstrols[i].m_pStaticNickName );
		}
		else	// 비접속중 (로그아웃 경과 시간 출력)
		{
			CTime cTime( kGuildMemberInfo.m_tLogOutTime );
			m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0, 
				GetWStrAboutTimeFromLogOutToNow( cTime ).c_str() );

			m_GuildUserListUIConstrols[i].m_pStaticChannel->SetShow( false );
			m_GuildUserListUIConstrols[i].m_pStaticNickName->GetString( 0 )->color = colorForLogOff;

		}

		m_GuildUserListUIConstrols[i].m_pStaticNickName->SetShow( true );
		m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetShow( true );

		m_GuildUserListUIConstrols[i].m_pButtonNickName->SetShowEnable( true, true );
		m_GuildUserListUIConstrols[i].m_pButtonGrade->SetShowEnable( false, false );

		SetGuildMemberIcon( i, true, kGuildMemberInfo.m_cUnitClass, kGuildMemberInfo.m_ucLevel );
		i++;
	}	
}

void CX2Community::UpdateGuildGradeInfo()
{
	int	i = 0;
	const D3DXCOLOR colorForLogOff( 0.57f, 0.57f, 0.57f, 1.0f );

	CX2GuildManager* pGulldManager	= NULL;
	pGulldManager	= g_pData->GetGuildManager();

	if ( pGulldManager == NULL )
		return;

	BOOST_TEST_FOREACH( const KGuildMemberInfo&, kGuildMemberInfo, m_vecGuildMemberInfo )
	{
		m_vecUserUID.push_back( kGuildMemberInfo.m_iUnitUID );

		m_GuildUserListUIConstrols[i].m_pStaticNickName->SetString( 0, kGuildMemberInfo.m_wstrNickName.c_str() );
		m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0, 
			pGulldManager->GetWStringGuildMemberGrade( kGuildMemberInfo.m_ucMemberShipGrade ).c_str() );

		m_GuildUserListUIConstrols[i].m_pStaticNickName->SetShow( true );
		m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->SetShow( true );
		m_GuildUserListUIConstrols[i].m_pStaticLocationOrGradeOrTimeSpan->GetString( 0 )->color =
			pGulldManager->GetGradeColor( static_cast< CX2GuildManager::GUILD_USER_GRADE >( kGuildMemberInfo.m_ucMemberShipGrade ) );
		m_GuildUserListUIConstrols[i].m_pStaticChannel->SetShow( false );

		m_GuildUserListUIConstrols[i].m_pButtonNickName->SetShowEnable( true, true );
		m_GuildUserListUIConstrols[i].m_pButtonGrade->SetShowEnable( true, true );

		if ( kGuildMemberInfo.m_tLogInTime == 0 )
			m_GuildUserListUIConstrols[i].m_pStaticNickName->GetString( 0 )->color = colorForLogOff;
		else
		{
			SetGuildMemberNickNameColor( kGuildMemberInfo, m_GuildUserListUIConstrols[i].m_pStaticNickName );
		}

		SetGuildMemberIcon( i, true, kGuildMemberInfo.m_cUnitClass, kGuildMemberInfo.m_ucLevel );

		i++;
	}
}

void CX2Community::SetGuildMemberNickNameColor( const KGuildMemberInfo& kGuildMemberInfo, CKTDGUIStatic* pStaticNickName )
{
	CX2GuildManager* pGulldManager	= NULL;
	pGulldManager	= g_pData->GetGuildManager();
	
	switch ( static_cast< CX2GuildManager::GUILD_USER_GRADE >( kGuildMemberInfo.m_ucMemberShipGrade ) )
	{
	case CX2GuildManager::GUG_MASTER:
	case CX2GuildManager::GUG_SYSOP:
		pStaticNickName->GetString( 0 )->color = 
			pGulldManager->GetGradeColor( static_cast< CX2GuildManager::GUILD_USER_GRADE >( kGuildMemberInfo.m_ucMemberShipGrade ) );
		break;

	default:
		pStaticNickName->GetString( 0 )->color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
		break;
	}
}

void CX2Community::UpdateGuildTabPageUI()
{
	if ( g_pData->GetGuildManager() == NULL )
	{
		ASSERT( !"Wrong Path" );
		return;
	}

	if ( m_pDlgGuild != NULL )
	{
		CKTDGUIStatic* pStatic	= static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( L"page_number" ) );

		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << m_uiNowGuildPage
				<< L"/" << m_uiMaxGuildPage;
			pStatic->SetString( 0, wstrstm.str().c_str() );

			if ( pStatic->GetShow() == false )
				pStatic->SetShow( true );
		}
	}
}

void CX2Community::UpdateNumOfGuildMembers()
{
	if ( g_pData->GetGuildManager() == NULL )
	{
		ASSERT( !"Wrong Path" );
		return;
	}

	if ( m_pDlgGuild != NULL )
	{
		m_uiAllNumberOfGuildMembers = g_pData->GetGuildManager()->GetNowNumOfGuildMember();
		CKTDGUIStatic* pStatic		= static_cast< CKTDGUIStatic* >( m_pDlgGuild->GetControl( L"numOfGuildMember" ) );

		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << m_uiNowNumberOfGuildMembersPlayingGame
				<< L"/" << m_uiAllNumberOfGuildMembers;
			pStatic->SetString( 0, wstrstm.str().c_str() );

			if ( pStatic->GetShow() == false )
				pStatic->SetShow( true );
		}
	}
}

// 2011.05.05 lygan_조성욱 // 중국용 커뮤니티탭 유저 정보 보기
#ifdef SERV_USER_WATCH_NEW
bool CX2Community::Handler_EGS_USER_WATCH_NEW_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_COMMUNITY_SURVEY_NEW_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_USER_COMMUNITY_SURVEY_NEW_ACK ) == true )
	{
		m_iPickedUserUID = kEvent.unitUID;
#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
#ifdef EXTEND_SERVER_GROUP_MASK
		int iServerGroupID = -1;
		//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
		iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
		// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
		if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
		SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
		//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
		eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iPickedUserUID);
		// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
		if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
#endif // EXTEND_SERVER_GROUP_MASK
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4851 ), g_pMain->GetNowState() );
			return true;
		}
		//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

		switch ( kEvent.m_cCommunity_Tab_Type )
		{
		case KEGS_USER_COMMUNITY_SURVEY_NEW_ACK::USER_WATCH:
			{
				if( m_iPickedUserUID > 0 && m_iPickedUserUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				{
					g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WATCH, true, m_iPickedUserUID);
					return true;
				}
			}
			break;
		case KEGS_USER_COMMUNITY_SURVEY_NEW_ACK::INVITE_GUILD :
			{
				g_pData->GetGuildManager()->Handler_EGS_INVITE_GUILD_REQ( m_iPickedUserUID );
				return true;
			}
			break;
		}
	}

	return false;
}
#endif //SERV_USER_WATCH_NEW

#ifdef EXTEND_SERVER_GROUP_MASK
D3DXCOLOR CX2Community::GetChannelColor( int iChannelID, int ServerID /*= SGI_SOLES*/ ) const
#else
D3DXCOLOR CX2Community::GetChannelColor( int iChannelID, SERVER_GROUP_ID ServerID /*= SGI_SOLES*/ ) const
#endif EXTEND_SERVER_GROUP_MASK
{
	D3DXCOLOR color;
	color.a = 1.0f; 

#ifdef REMOVE_KR_SERVER_TEXTURE
	switch ( iChannelID )
	{
	case 1:		// 루벤
	case 3:
	case 5:
		{
			color.r	= 0.32f;
			color.g = 0.7f;
			color.b = 0.07f;
		}
		break;

	case 2:		// 엘더
	case 4:
	case 6:
		{
			color.r	= 0.01f;
			color.g = 0.64f;
			color.b = 0.82f;
		}
		break;

	case 7:		// 베스마
	case 9:
	case 11:
	case 13:
		{
			color.r	= 0.94f;
			color.g = 0.35f;
			color.b = 0.06f;
		}
		break;

	case 8:		// 알테라
	case 10:
	case 12:
		{
			color.r	= 0.71f;
			color.g = 0.18f;
			color.b = 0.72f;
		}
		break;

	default:
		{
			color.r = 0.0f;
			color.g = 0.0f;
			color.b = 0.0f;
		}
		break;			
	}
#else // REMOVE_KR_SERVER_TEXTURE
	switch( ServerID )
	{
	case SGI_SOLES:
		{
			switch( iChannelID )
			{
			case 1:		// 루벤
			case 3:
				{
					color.r	= 0.32f;
					color.g = 0.7f;
					color.b = 0.07f;
				}
				break;
			case 2:		// 알테라
			case 13:
				{
					color.r	= 0.71f;
					color.g = 0.18f;
					color.b = 0.72f;
				}
				break;
			case 4:		// 벨더
			case 6:
			case 8:
				{
					color.r	= 0.81f;
					color.g = 0.28f;
					color.b = 0.21f; 
				}
				break;
			case 5:		// 엘더
			case 7:
				{
					color.r	= 0.01f;
					color.g = 0.64f;
					color.b = 0.82f;
				}
				break;
			case 9:		// 베스마
			case 11:
				{
					color.r	= 0.94f;
					color.g = 0.35f;
					color.b = 0.06f;
				}
				break;
			case 10:	// 하멜
			case 12:
			case 14:
				{
					color.r	= 0.04f;
					color.g = 0.53f;
					color.b = 1.0f;  
				}
				break;
			default:
				{
					color.r = 0.0f;
					color.g = 0.0f;
					color.b = 0.0f;
				}
				break;
			}
		}
		break;
	case SGI_GAIA:
		{
			switch( iChannelID )
			{
			case 1:		// 루벤
			case 3:
			case 5:
				{
					color.r	= 0.32f;
					color.g = 0.7f;
					color.b = 0.07f;
				}
				break;
			case 2:		// 알테라
			case 4:
				{
					color.r	= 0.71f;
					color.g = 0.18f;
					color.b = 0.72f;
				}
				break;
			case 6:		// 벨더
			case 8:
				{
					color.r	= 0.81f;
					color.g = 0.28f;
					color.b = 0.21f; 
				}
				break;
			case 7:		// 엘더
			case 9:
				{
					color.r	= 0.01f;
					color.g = 0.64f;
					color.b = 0.82f;
				}
				break;
			case 10:	// 하멜
			case 12:
				{
					color.r	= 0.04f;
					color.g = 0.53f;
					color.b = 1.0f;  
				}
				break;
			case 11:	// 베스마
			case 13:
				{
					color.r	= 0.94f;
					color.g = 0.35f;
					color.b = 0.06f;
				}
				break;
			default:
				{
					color.r = 0.0f;
					color.g = 0.0f;
					color.b = 0.0f;
				}
				break;
			}
		}
		break;
	default:
		{
			color.r = 0.0f;
			color.g = 0.0f;
			color.b = 0.0f;
		}
		break;
	}
#endif // REMOVE_KR_SERVER_TEXTURE

	return color;

}

std::wstring	CX2Community::GetWStrAboutTimeFromLogOutToNow( const CTime& ctLogOutTime )
{
	const BYTE	HOUR_IN_A_DAY						= 24;

	CTime		ctCurrentTime				= CTime::GetCurrentTime();
	CTimeSpan	ctsTimeFromLogOutToNow		= ctCurrentTime - ctLogOutTime;

	LONGLONG	l64TotalHour				= ctsTimeFromLogOutToNow.GetTotalHours();
	LONGLONG	l64TotalDay					= l64TotalHour / HOUR_IN_A_DAY;

	std::wstringstream	wstrStreamTimeSpan;

	wstrStreamTimeSpan << GET_STRING( STR_ID_99 );

	std::wstringstream wstrStreamTemp;

	if ( l64TotalDay > 0 )	// day
	{
		wstrStreamTemp << l64TotalDay << GET_STRING( STR_ID_14 );
	}
	else if( l64TotalHour > 0 ) // hour
	{
		wstrStreamTemp << l64TotalHour << GET_STRING( STR_ID_310 );
	}
	else	// 1 시간 미만
	{
		wstrStreamTemp << GET_STRING( STR_ID_4520 );
	}

#ifdef CUSTOMIZE_FOR_LONG_LANGUAGE
	wstrStreamTimeSpan << L"\n" << GET_REPLACED_STRING( ( STR_ID_4523, "L", wstrStreamTemp.str() ) );
#else // CUSTOMIZE_FOR_LONG_LANGUAGE
	wstrStreamTimeSpan << GET_REPLACED_STRING( ( STR_ID_4523, "L", wstrStreamTemp.str() ) );
#endif // CUSTOMIZE_FOR_LONG_LANGUAGE

    return wstrStreamTimeSpan.str();
}

void CX2Community::SetGuildMemberIcon( int i, bool bShow, int iUnitClass, int iUnitLv  )
{
	if( i < 0 || i >= _CONST_COMMUNITY_UI_::g_byNumOfGuildMemberPerOnePage )
		return;

	if( m_bHideWindow == true )
	{
		if(m_pFieldUserIcon[i] != NULL)
		{
			m_pFieldUserIcon[i]->SetShowEnable( false, false );
			return;
		}
	}

	if( bShow == true )
	{
		if(m_pFieldUserIcon[i] == NULL)
		{
			m_pFieldUserIcon[i] = new CKTDGUIDialog( m_pStage, L"DLG_UI_Character_Icon.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pFieldUserIcon[i] );			
		}
		m_pFieldUserIcon[i]->SetShowEnable( true, true );
	}
	else
	{
		if(m_pFieldUserIcon[i] != NULL)
		{
			m_pFieldUserIcon[i]->SetShowEnable( false, false );
		}
	}

	if( m_pFieldUserIcon[i] == NULL )
		return;

	CKTDGUIStatic* pStaticClass		= static_cast< CKTDGUIStatic* >( m_pFieldUserIcon[i]->GetControl( L"user" ) );	// 직업아이콘
	CKTDGUIStatic* pStaticLv10		= static_cast< CKTDGUIStatic* >( m_pFieldUserIcon[i]->GetControl( L"ten" ) );	// 레벨 10자리
	CKTDGUIStatic* pStaticLv1		= static_cast< CKTDGUIStatic* >( m_pFieldUserIcon[i]->GetControl( L"one" ) );	// 레벨 1자리
	
	int unitLv10, unitLv1;

	unitLv10	= iUnitLv / 10;
	unitLv1		= iUnitLv % 10;
	
	
	ClearUserIcon(i);

	if( NULL != pStaticClass && 
		NULL != pStaticClass->GetPicture(0) )
	{
		CKTDGUIControl::CPictureData* pPicture = pStaticClass->GetPicture(0);

		wstring fileName;
		wstring pieceName;
		if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS) iUnitClass, CX2Data::CIT_20by20 ) )
		{
			pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
		}
		else
		{
			ASSERT( !"NO" );
		}
	}

	pStaticLv10->GetPicture( unitLv10 )->SetShow(true);
	pStaticLv1->GetPicture( unitLv1 )->SetShow(true);

	if(m_pDlgGuild != NULL)
	{
		D3DXVECTOR3 tempPos1 = m_pDlgGuild->GetDummyPos(i);
		D3DXVECTOR3 tempPos2 = m_pDlgBack->GetPos();
		m_pFieldUserIcon[i]->SetPos_LUA(tempPos1.x + tempPos2.x, tempPos1.y + tempPos2.y);
	}	
}

void CX2Community::SetTabByShortCutKey( X2_COMMUNITY_TAB eCommunityTab )
{
	CKTDGUIRadioButton* pRadioButton	= NULL;
	m_iPickedUserIndex	= 0;
	m_iPickedUserUID	= 0;
	CheckButton(NULL);

	switch ( eCommunityTab )
	{
	case XMUT_FIELD:
		{
			pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"all");
		}
		break;

	case XMUT_FRIEND:
		{
			pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"friend");
		}
		break;

	case XMUT_DISCIPLE:
		{
			pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"disciple");
		}
		break;

	case XMUT_GUILD:
		{
			pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"guild");
		}
		break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case XMUT_RELATION:
		{
			if ( NULL != g_pData->GetRelationshipManager() )
			{
				/// 결혼시스템의 텝은 3종류 (솔로, 커플, 결혼) 이기 때문에
				/// 커플 상태에 따라 다른 탭 이름을 얻어 오도록 한다.
				const SEnum::RELATIONSHIP_TYPE eType 
					= g_pData->GetRelationshipManager()->GetMyRelationInfoType();
				const WCHAR* pRadioButtonName = GetRelationTabName( eType );
				pRadioButton 
					= static_cast<CKTDGUIRadioButton*>( m_pDlgBack->GetControl( pRadioButtonName ) );
			}
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM


	default:
		break;
	}

	if ( pRadioButton != NULL )
	{
		m_MessengerUserTab = eCommunityTab;
		pRadioButton->SetChecked(true);	
	}
}

BYTE	CX2Community::GetGuildMemberGradeByUIDInThisList( UidType unitUID ) const
{
	BOOST_TEST_FOREACH( const KGuildMemberInfo&, kGuildMemberInfo, m_vecGuildMemberInfo )
	{
		if ( kGuildMemberInfo.m_iUnitUID == unitUID )
			return kGuildMemberInfo.m_ucMemberShipGrade;
	}

	return 0;	// GUG_INVALID
}

std::wstring CX2Community::GetGuildMemberNickName( UidType unitUID ) const
{
	BOOST_TEST_FOREACH( const KGuildMemberInfo&, kGuildMemberInfo, m_vecGuildMemberInfo )
	{
		if ( kGuildMemberInfo.m_iUnitUID == unitUID )
			return kGuildMemberInfo.m_wstrNickName;
	}
	
	return L"";
}

#endif	GUILD_MANAGEMENT
//}} kimhc // 2009-09-17 // 길드 UI

//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
void CX2Community::SetRefereeUI( bool bShow )
{
	if ( bShow == true )
	{
		if ( m_pDlgReferee == NULL )
		{
			D3DXVECTOR2 vPos = D3DXVECTOR2(710.f, 140.f);

			m_pDlgReferee = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Referee.lua", 0.07f );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgReferee );
			m_pDlgReferee->SetPos( vPos );
			m_pDlgReferee->SetShowEnable( false, false );
			m_pDlgReferee->SetDisableUnderWindow( true );

			CKTDGUIRadioButton *pRadioButton = static_cast< CKTDGUIRadioButton * >( m_pDlgBack->GetControl( L"Referee" ) );
			pRadioButton->SetShowEnable( true, true );

			m_uiNowRefereePage = 1;
			m_uiMaxRefereePage = 1;
		}
	}
	else
	{
		CKTDGUIRadioButton *pRadioButton = static_cast< CKTDGUIRadioButton * >( m_pDlgBack->GetControl( L"Referee" ) );
		pRadioButton->SetShowEnable( false, false );

		SAFE_DELETE_DIALOG( m_pDlgReferee );
	}
}

void CX2Community::UpdateRefereeList()
{
	if ( m_pDlgReferee == NULL )
		return;

	const int MAGIC_MAX_REFEREE_PER_PAGE = 8;
	WCHAR wszText[64] = L"";	

	m_vecUserLv.clear();
	m_vecUserUID.clear();
	for( UINT i = 0; i< m_vecRecommendUserInfo.size(); ++i )
	{
		m_vecUserUID.push_back( m_vecRecommendUserInfo[i].m_iUnitUID );
		m_vecUserLv.push_back( m_vecRecommendUserInfo[i].m_ucLevel );
	}

	for( UINT i = 0; i < MAGIC_MAX_REFEREE_PER_PAGE; ++i )
	{
		swprintf( wszText, sizeof(wszText) / (sizeof(WCHAR) - 1), L"User%d", i+1 );
		CKTDGUIButton* pButton = static_cast< CKTDGUIButton* >( m_pDlgReferee->GetControl( wszText ) );

		if( NULL != pButton )
		{
			swprintf( wszText, sizeof(wszText) / (sizeof(WCHAR) - 1), L"User%d_lv", i+1 );
			CKTDGUIStatic* pStaticLv = static_cast< CKTDGUIStatic* >( m_pDlgReferee->GetControl( wszText ) );

			swprintf( wszText, sizeof(wszText) / (sizeof(WCHAR) - 1), L"name%d", i+1 );
			CKTDGUIStatic* pStaticName = static_cast< CKTDGUIStatic* >( m_pDlgReferee->GetControl( wszText ) );

			swprintf( wszText, sizeof(wszText) / (sizeof(WCHAR) - 1), L"state%d", i+1 );
			CKTDGUIStatic* pStaticState = static_cast< CKTDGUIStatic* >( m_pDlgReferee->GetControl( wszText ) );

			if( i < m_vecRecommendUserInfo.size() )
			{
				const KRecommendUserInfo& kRecommenduserInfo = m_vecRecommendUserInfo[i];

				pButton->SetShowEnable( true, true );

				pStaticLv->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kRecommenduserInfo.m_ucLevel ) ) );
				pStaticLv->SetShowEnable( true, true );

				pStaticName->SetString( 0,  kRecommenduserInfo.m_wstrNickName.c_str() );
				pStaticName->SetShowEnable( true, true );

				if ( kRecommenduserInfo.m_bIsOnline == true )
				{
					pStaticState->SetString( 0, GET_STRING( STR_ID_98 ) );
				}
				else
				{
					pStaticState->SetString( 0, GET_STRING( STR_ID_99 ) );
				}
				pStaticState->SetShowEnable( true, true );
			}
			else
			{
				pButton->SetShowEnable( false, false );
				pStaticLv->SetShowEnable( false, false );
				pStaticName->SetShowEnable( false, false );
				pStaticState->SetShowEnable( false, false );
			}
		}
	}
}

bool CX2Community::Handler_EGS_GET_RECOMMEND_USER_LIST_REQ( UINT uiNowPage )
{
	KEGS_GET_RECOMMEND_USER_LIST_REQ kPacket;

	kPacket.m_uiViewPage	= uiNowPage;
	m_uiNowRefereePage		= uiNowPage;

	if ( m_pDlgReferee->GetShow() == true )
		m_pDlgReferee->SetShow( false );

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_RECOMMEND_USER_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_RECOMMEND_USER_LIST_ACK );

	return true;
}

bool CX2Community::Handler_EGS_GET_RECOMMEND_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_RECOMMEND_USER_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_RECOMMEND_USER_LIST_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( m_pDlgReferee != NULL )
			{
				m_pDlgReferee->SetShow( true );

				m_uiMaxRefereePage  = kEvent.m_uiTotalPage;
				m_uiNowRefereePage	= kEvent.m_uiViewPage;

				m_vecRecommendUserInfo = kEvent.m_vecRecommendUserList;

				UpdateRefereeList();

				CKTDGUIStatic* pStatic	= static_cast< CKTDGUIStatic* >( m_pDlgReferee->GetControl( L"page_number" ) );

				if ( pStatic != NULL )
				{
					wstringstream wstrstm;
					wstrstm << m_uiNowRefereePage
						<< L"/" << m_uiMaxRefereePage;
					pStatic->SetString( 0, wstrstm.str().c_str() );

					if ( pStatic->GetShow() == false )
						pStatic->SetShow( true );
				}
			}
			return true;
		}
	}

	return false;
}

bool CX2Community::Handler_EGS_RECOMMEND_USER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RECOMMEND_USER_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pData == NULL ||
		g_pData->GetMessenger() == NULL ||
		g_pData->GetMessenger()->GetOpen() == false ||
		g_pData->GetMessenger()->GetOpenTab() != XMUT_REFEREE )
		return true;

	BOOST_TEST_FOREACH( KRecommendUserInfo&, value, m_vecRecommendUserInfo )
	{
		if ( value.m_iUnitUID == kEvent.m_iRecommendUnitUID )
		{
			switch ( kEvent.m_cType )
			{
			case KRecommendUserInfo::RUIT_LEVEL_UP:
				{
					value.m_ucLevel = static_cast< u_char >( kEvent.m_iData );
				}
				break;

			case KRecommendUserInfo::RUIT_LOGIN:
				{
					value.m_bIsOnline = true;
				}
				break;

			case KRecommendUserInfo::RUIT_LOGOUT:
				{
					value.m_bIsOnline = false;
				}
				break;

			default:
				{
					ASSERT( !L"Wrong Path!" );
					return false;
				}
				break;
			}

			break;
		}
	}

	UpdateRefereeList();	

	return true;
}

#endif	VIEW_REFEREE_LIST
//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

void CX2Community::DeleteFriendTab()
{
	bool bSelectedUser = false;

	if(m_bOpenGroupMenu == false)
	{
		bSelectedUser = true;
	}

	if( bSelectedUser == true)
	{
		wstring nickName = GetSelectedUnitName();
		if( nickName.empty() == true )
			return;

		CX2Community::MessengerUser* pMessengerUser = GetMessengerUser( nickName.c_str() );
		if( pMessengerUser == NULL )
			return;
#ifdef NEW_MESSENGER
		// 친구 삭제
		Handler_EGS_DELETE_FRIEND_REQ( pMessengerUser->m_UnitUID );
		return;
#else NEW_MESSENGER
		//그룹이 없는 녀석일 경우 카테고리 코드를 0으로 보내고 있는데 확실한가..?
		if ( CNMCOClientObject::GetInstance().DeleteFriendFromCategory( pMessengerUser->m_NMFriendKey, pMessengerUser->m_uCategoryCode ) == FALSE )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_86 ), m_pStage );
		}
		else
		{
			Handler_EGS_UPDATE_FRIEND_INFO_REQ(pMessengerUser->m_NickName);
		}
#endif NEW_MESSENGER
		
	}
	else
	{
		wstring groupName = GetSelectedGroupName();
		if( groupName.empty() == true )
			return;

		CX2Community::MessengerUserGroup* pMgrUserGroup = GetMgrUserGroup( groupName.c_str() );
		if(pMgrUserGroup == NULL)
			return;

		// 카데고리 삭제
#ifdef NEW_MESSENGER
		Handler_EGS_DELETE_FRIEND_GROUP_REQ( pMgrUserGroup->m_uCategoryCode );
#else NEW_MESSENGER
		if ( CNMCOClientObject::GetInstance().DeleteCategory( pMgrUserGroup->m_uCategoryCode ) == FALSE )
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_88 ), m_pStage );
#endif NEW_MESSENGER
	}
}

void CX2Community::CutOnOffFriend()
{
	wstring nickName = GetSelectedUnitName();

	if( nickName.empty() == true )
	{
		return;
	}

	CX2Community::MessengerUser* pMessengerUser = GetMessengerUser( nickName.c_str() );
	if( pMessengerUser == NULL )
		return;

#ifdef NEW_MESSENGER
	// 차단
	if( pMessengerUser->IsBlockable() )
	{
		if( false == pMessengerUser->IsBlocked() )
		{
			Handler_EGS_BLOCK_FRIEND_REQ( pMessengerUser->m_UnitUID );
		}
		else
		{
			Handler_EGS_UNBLOCK_FRIEND_REQ( pMessengerUser->m_UnitUID );
		}
	}
	else
	{		
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_3718 ), m_pStage );
	}
#else NEW_MESSENGER
	if ( CNMCOClientObject::GetInstance().BlockFriend( pMessengerUser->m_NMFriendKey, !pMessengerUser->m_bBlocked ) == FALSE )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_85 ), m_pStage );
	}
#endif
}


void CX2Community::SetFieldUserIcon(int i, bool bShow)
{
	if(i < 0 || i >= 8)
		return;

	if(m_bHideWindow == true)
	{
		if(m_pFieldUserIcon[i] != NULL)
		{
			m_pFieldUserIcon[i]->SetShowEnable(false, false);
			return;
		}
	}

	if(bShow == true)
	{
		if(m_pFieldUserIcon[i] == NULL)
		{			
			m_pFieldUserIcon[i] = new CKTDGUIDialog( m_pStage, L"DLG_UI_Character_Icon.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pFieldUserIcon[i] );			
		}
		m_pFieldUserIcon[i]->SetShowEnable(true, true);
	}
	else
	{
		if(m_pFieldUserIcon[i] != NULL)
		{
			m_pFieldUserIcon[i]->SetShowEnable(false, false);
		}
	}
	//else
	//{
	//	if(m_pFieldUserIcon[i] != NULL)
	//		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pFieldUserIcon[i], NULL, false );		
	//	m_pFieldUserIcon[i] = NULL;
	//	return;
	//}

	if(m_pFieldUserIcon[i] == NULL || i >= (int)m_vecFieldUser.size())
		return;

	//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pFieldUserIcon[i], true );

	CKTDGUIStatic* pStaticClass = (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"user" );	// 직업아이콘
	CKTDGUIStatic* pStaticLv10	= (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"ten" );	// 레벨 10자리
	CKTDGUIStatic* pStaticLv1	= (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"one" );	// 레벨 1자리
		
	int unitClass = 0;
	int unitLv = 0;
	int unitLv10, unitLv1;

    unitClass = m_vecFieldUser[i].m_cUnitClass;
	unitLv = m_vecFieldUser[i].m_ucLevel;
	unitLv10 = unitLv / 10;
	unitLv1 = unitLv % 10;

	ClearUserIcon(i);

	if( NULL != pStaticClass && 
		NULL != pStaticClass->GetPicture(0) )
	{
		CKTDGUIControl::CPictureData* pPicture = pStaticClass->GetPicture(0);

		wstring fileName;
		wstring pieceName;
		if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS)unitClass, CX2Data::CIT_20by20 ) )
		{
			pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
		}
		else
		{
			ASSERT( !"NO" );
		}
	}



	pStaticLv10->GetPicture(unitLv10)->SetShow(true);
	pStaticLv1->GetPicture(unitLv1)->SetShow(true);

	if(m_pDlgField != NULL)
	{
		D3DXVECTOR3 tempPos1 = m_pDlgField->GetDummyPos(i);
		D3DXVECTOR3 tempPos2 = m_pDlgBack->GetPos();
		m_pFieldUserIcon[i]->SetPos_LUA(tempPos1.x + tempPos2.x, tempPos1.y + tempPos2.y);
	}	
}

void CX2Community::ClearUserIcon(int i)
{	
	
	if(m_pFieldUserIcon[i] == NULL)
		return;

	CKTDGUIStatic* pStaticClass = (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"user" );	// 직업아이콘
	CKTDGUIStatic* pStaticLv10	= (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"ten" );	// 레벨 10자리
	CKTDGUIStatic* pStaticLv1	= (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"one" );	// 레벨 1자리

	if(pStaticClass == NULL || pStaticLv10 == NULL || pStaticLv1 == NULL)
		return;

	//pStaticClass->SetShowEnable( false, false );

	for(int i=0; i<10; ++i)
	{
		pStaticLv10->GetPicture(i)->SetShow(false);
		pStaticLv1->GetPicture(i)->SetShow(false);
	}
}

bool CX2Community::GetOverDialog()
{
	if(m_pDlgBack != NULL && m_pDlgBack->GetShow() == true)
		return m_pDlgBack->GetIsMouseOver();

	return false;
}

void CX2Community::SetChatBoxShow(CX2Community::ChatSession* pChatSession, bool bShow)
{
	ASSERT( NULL != pChatSession );
	if( NULL == pChatSession )
		return; 

	
	ASSERT( NULL != pChatSession->m_pDialog );
	if( NULL == pChatSession->m_pDialog )
		return; 

	X2_DIALOG_LAYER iLayer = XDL_MESSENGER;
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_PVP_GAME:
		{
			iLayer = XDL_POP_UP;
		} break;		
	default:
		iLayer = XDL_MESSENGER;
		break;
	}

	if( pChatSession->m_pDialog != NULL )
	{
		pChatSession->m_pDialog->SetLayer( iLayer );
	}


	bool bMax = pChatSession->m_bMaxWindow;

	// max
	CKTDGUIStatic* pStaticCha = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl(L"Cha");
	CKTDGUIStatic* pStaticMaxId = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl(L"StaticMessenger_NickName");
	CKTDGUIListBox* pListBoxChat = (CKTDGUIListBox*)pChatSession->m_pDialog->GetControl(L"ListBoxChat");
	CKTDGUIIMEEditBox *pChatBox = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
	CKTDGUIButton* pButtonSend = (CKTDGUIButton*)pChatSession->m_pDialog->GetControl(L"Button_Send");
	CKTDGUIButton* pButtonExit = (CKTDGUIButton*)pChatSession->m_pDialog->GetControl(L"Button_Exit");
	CKTDGUIButton* pButtonMini = (CKTDGUIButton*)pChatSession->m_pDialog->GetControl(L"Button_Mini");		

	// mini
	CKTDGUIStatic* pStaticMini = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl(L"Mini_Window");
	CKTDGUIStatic* pStaticMiniId = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl(L"Mini_ID");
	CKTDGUIButton* pButtonMiniExit = (CKTDGUIButton*)pChatSession->m_pDialog->GetControl(L"Button_Mini_Win_Exit");
	CKTDGUIButton* pButtonMiniWin = (CKTDGUIButton*)pChatSession->m_pDialog->GetControl(L"Button_Maximum");	

	// mini alarm
	CKTDGUIStatic* pStaticMiniAlarm = (CKTDGUIStatic*)pChatSession->m_pDialog->GetControl(L"Mini_Flickering");
	

	if(bShow == true && pChatSession->m_bHideWindow == false)
	{		
		pChatSession->m_bShowWindow = bShow;

		pChatSession->m_pDialog->SetShowEnable(bShow, bShow);
		if(bMax == true)
		{
			pStaticCha->SetShowEnable(true, true);
			pStaticMaxId->SetShowEnable(true, true);
			pListBoxChat->SetShowEnable(true, true);
			pChatBox->SetShowEnable(true, true);
			pButtonSend->SetShowEnable(true, true);
			pButtonExit->SetShowEnable(true, true);
			pButtonMini->SetShowEnable(true, true);

			pStaticMini->SetShowEnable(false, false);
			pStaticMiniId->SetShowEnable(false, false);
			pButtonMiniExit->SetShowEnable(false, false);
			pButtonMiniWin->SetShowEnable(false, false);


			if(pChatSession->m_pDialog->GetFront() == true)
			{
				SetOpenChatSession(pChatSession->m_pDialog, true);
			}			
		}
		else
		{
			pStaticCha->SetShowEnable(false, false);
			//pStaticMaxId->SetShowEnable(false, false);
			pListBoxChat->SetShowEnable(false, false);
			pChatBox->SetShowEnable(false, false);
			pButtonSend->SetShowEnable(false, false);
			pButtonExit->SetShowEnable(false, false);
			pButtonMini->SetShowEnable(false, false);

			pStaticMini->SetShowEnable(true, true);
			//pStaticMiniId->SetShowEnable(true, true);
			pButtonMiniExit->SetShowEnable(true, true);
			pButtonMiniWin->SetShowEnable(true, true);	

			if( pChatSession != NULL && pChatSession->m_pDialog != NULL )
			{
				CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
				if ( pIMEEdit != NULL && pIMEEdit->GetHaveFocusIn() == true)
				{
					// 포커스 잃음
					pChatSession->m_pDialog->ClearFocus();	
				}				
			}
			
		}

		if(pChatSession->m_bAlarmReceiveMessage == true)
		{			
			pStaticMiniAlarm->SetShowEnable(true, true);
		}
		else
		{
			pStaticMiniAlarm->SetShowEnable(false, false);
		}

	}
	else
	{
		pChatSession->m_pDialog->SetShowEnable(false, false);
	}
}

void CX2Community::SetHideWindow(bool val)
{
	m_bHideWindow = val;

	if( m_bOpen == true && m_bHideWindow == false)
	{
		SetOpen(true);
	}
	else
	{
		SetOpen(false);
	}

	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		pChatSession->m_bHideWindow = val;

		if ( pChatSession->m_pDialog != NULL )
		{						
			SetChatBoxShow(pChatSession, true);			
		}
	}	
}

void CX2Community::SetLayer(X2_DIALOG_LAYER layer)
{		
	m_eNowLayer = layer;

	if(m_pDLGQuery != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGQuery, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGQuery, false );
	}	

	for(int i=0; i<8; ++i)
	{
		if(m_pFieldUserIcon[i] != NULL)
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pFieldUserIcon[i], layer);
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pFieldUserIcon[i], false );
		}
	}


	if(m_pDlgField != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgField, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgField, false );
	}
	if(m_pDlgFriend != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgFriend, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgFriend, false );
	}
	if(m_pDlgGuild != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgGuild, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgGuild, false );
	}
	if(m_pDlgDisciple != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgDisciple, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgDisciple, false );
	}
	
	//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
	if( m_pDlgReferee != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgReferee, layer );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgReferee, false );
	}
#endif	VIEW_REFEREE_LIST
	//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if(m_pDlgRelation != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgRelation, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRelation, false );
	}
	if(m_pDlgRelationOnlyPartnerShowing != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgRelationOnlyPartnerShowing, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRelationOnlyPartnerShowing, false );
	}
	if(m_pDlgRelationOnlyNotCouple != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgRelationOnlyNotCouple, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRelationOnlyNotCouple, false );
	}

#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef	SERV_RECRUIT_EVENT_BASE
	if( m_pDlgRecruit != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgRecruit, layer );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRecruit, false );
	}
#endif	SERV_RECRUIT_EVENT_BASE

	if(m_pDlgBack != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgBack, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgBack, false );
	}	

	// 모든 채팅세션의 레이어를 현재 스테이트에 맞게 재조정한다.
	X2_DIALOG_LAYER iLayer = XDL_MESSENGER;
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_PVP_GAME:
		{
			iLayer = XDL_POP_UP;
		} break;		
	default:
		iLayer = XDL_MESSENGER;
		break;
	}

	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];
		if ( pChatSession->m_pDialog != NULL )
		{						
			pChatSession->m_pDialog->SetLayer(iLayer);					
		}
	}
	
}

bool CX2Community::GetOpenChatBox()
{
	for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
	{
		CX2Community::ChatSession* pChatSession = m_vecChatSession[i];

		if ( pChatSession->m_pDialog != NULL && pChatSession->m_bHideWindow == false && pChatSession->m_bShowWindow == true)
		{						
			return true;
		}
	}

	return false;
}

void CX2Community::SetFriendTab(bool val) 
{
	m_iPickedUserIndex = 0;
	m_iPickedUserUID = 0;
	CheckButton(NULL);

	if(val == true)
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"friend");
		pRadioButton->SetChecked(true);		

		m_MessengerUserTab = XMUT_FRIEND;
#if 0 
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgFriend, true );
		m_pDlgField->SetShowEnable(false, false);
		m_pDlgFriend->SetShowEnable(true, true);
		//m_pDlgGuild->SetShowEnable(false, false);
		m_pDlgDisciple->SetShowEnable(false, false);
#endif
	}
	else
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"all");
		pRadioButton->SetChecked(true);

		m_MessengerUserTab = XMUT_FIELD;
#if 0 
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgField, true );
		m_pDlgField->SetShowEnable(true, true);
		m_pDlgFriend->SetShowEnable(false, false);
		//m_pDlgGuild->SetShowEnable(false, false);
		m_pDlgDisciple->SetShowEnable(false, false);
#endif
	}	
}
//커뮤니티에서 길드를 분리하기 위해, 외부에서 탭을 수정할 수 있도록 함수 추가
void CX2Community::SetFieldTab(bool val)
{
	m_iPickedUserIndex = 0;
	m_iPickedUserUID = 0;
	CheckButton(NULL);

	if(val == true)
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"all");
		pRadioButton->SetChecked(true);	

		m_MessengerUserTab = XMUT_FIELD;
	}
	else
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"all");
		pRadioButton->SetChecked(true);

		m_MessengerUserTab = XMUT_FIELD;
	}	
}
void CX2Community::SetGuildTab(bool val)
{
	m_iPickedUserIndex = 0;
	m_iPickedUserUID = 0;
	CheckButton(NULL);

	if(val == true)
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"guild");
		pRadioButton->SetChecked(true);	

		m_MessengerUserTab = XMUT_GUILD;
	}
	else
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDlgBack->GetControl(L"all");
		pRadioButton->SetChecked(true);

		m_MessengerUserTab = XMUT_FIELD;
	}	
}

bool CX2Community::IsMyFriend(UidType iUid)
{
	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecDefaultGroupUser.size(); i++ )
	{
		CX2Community::MessengerUser* pMessenger = m_pMessengerUserList->m_vecDefaultGroupUser[i];
		if( pMessenger != NULL && pMessenger->m_UnitUID == iUid )
		{
			return true;	
		}
	}

	for ( int i = 0; i < (int)m_pMessengerUserList->m_vecUserGroup.size(); i++ )
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = m_pMessengerUserList->m_vecUserGroup[i];
		if ( pMessengerUserGroup != NULL )
		{
			for ( int j = 0; j < (int)pMessengerUserGroup->m_vecUser.size(); j++ )
			{
				CX2Community::MessengerUser* pMessenger = pMessengerUserGroup->m_vecUser[j];
				if( pMessenger != NULL && pMessenger->m_UnitUID == iUid )
				{
					return true;	
				}
			}
		}
	}

	return false;
}

bool CX2Community::GetNickNameByUID(wstring &wstrNickName, UidType iUid)
{	
	if(iUid == 0)
		return false;

	for(UINT i=0; i<m_vecFieldUser.size(); ++i)
	{
		if( m_vecFieldUser[i].m_iUnitUID == iUid )
		{
			wstrNickName = m_vecFieldUser[i].m_wstrNickName;
			return true;
		}		
	}

	return false;
}

int CX2Community::FindChatHistory(wstring &wstrNickName, wstring &wstrOppositeNickName)
{	
	for(UINT i=0; i<m_vecChatHistory.size(); ++i)
	{
		if( //m_vecChatHistory[i].m_wstrNickName.compare(wstrNickName) == 0 &&
			m_vecChatHistory[i].m_wstrOppositeNickName.compare(wstrOppositeNickName) == 0 )
		{
			return i;
		}
	}

	return -1;
}

void CX2Community::CreateChatHistory(wstring &wstrNickName, wstring &wstrOppositeNickName)
{
	ChatHistory chatHistory;

	chatHistory.m_wstrNickName = wstrNickName;
	chatHistory.m_wstrOppositeNickName = wstrOppositeNickName;
	chatHistory.m_vecTalkHistory.clear();
		
	m_vecChatHistory.push_back(chatHistory);
}

void CX2Community::SetChatHistory(CX2Community::ChatSession* pChatSession)
{
#ifdef NEW_MESSENGER
	if(pChatSession != NULL )
	{
		wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
		wstring wstrOppositeNickName = pChatSession->m_wstrTargetNickName;

		int iChatHistory = FindChatHistory( wstrNickName, wstrOppositeNickName );
		if(iChatHistory >= 0)
		{
			//m_vecChatHistory[iChatHistory].m_vecTalkHistory.clear();					
			m_vecChatHistory[iChatHistory].m_vecTalkHistory = pChatSession->m_vecTalk;
		}
		else
		{
			CreateChatHistory(wstrNickName, wstrOppositeNickName);
			m_vecChatHistory[m_vecChatHistory.size()-1].m_vecTalkHistory = pChatSession->m_vecTalk;
		}
	}
#else NEW_MESSENGER
//{{AFX
	if(pChatSession != NULL && pChatSession->m_pCNMCSChatSession != NULL)
	{
		wstring wstrNickName = pChatSession->m_pCNMCSChatSession->GetMyNickName();
		wstring wstrOppositeNickName = pChatSession->m_pCNMCSChatSession->GetOppositeNickName();		

		int iChatHistory = FindChatHistory( wstrNickName, wstrOppositeNickName );
		if(iChatHistory >= 0)
		{
			//m_vecChatHistory[iChatHistory].m_vecTalkHistory.clear();					
			m_vecChatHistory[iChatHistory].m_vecTalkHistory = pChatSession->m_vecTalk;
		}
		else
		{
			CreateChatHistory(wstrNickName, wstrOppositeNickName);
			m_vecChatHistory[m_vecChatHistory.size()-1].m_vecTalkHistory = pChatSession->m_vecTalk;
		}
	}
//}}AFX
#endif NEW_MESSENGER
}

void CX2Community::GetChatHistory(CX2Community::ChatSession* pChatSession)
{
#ifdef NEW_MESSENGER
	if(pChatSession != NULL)
	{
		wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
		wstring wstrOppositeNickName = pChatSession->m_wstrTargetNickName;

		int iChatHistory = FindChatHistory( wstrNickName, wstrOppositeNickName );
		if( iChatHistory >= 0 )
		{
			pChatSession->m_vecTalk.clear();
			pChatSession->m_vecTalk = m_vecChatHistory[iChatHistory].m_vecTalkHistory;
		}

		CKTDGUIListBox* pListBox = (CKTDGUIListBox*)pChatSession->m_pDialog->GetControl(L"ListBoxChat"); //GetListBox();
		if ( pListBox != NULL )
		{
			pListBox->RemoveAllItems();
			for ( UINT i = 0; i < pChatSession->m_vecTalk.size(); ++i )
			{
				pListBox->AddItem( pChatSession->m_vecTalk[i].c_str(), NULL );
			}
			pListBox->SetScrollBarEndPos();
		}

		CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
		if ( pControl != NULL )
		{
			if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
				pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
			{
				return;
			}
		}
	}	
#else NEW_MESSENGER
	if(pChatSession != NULL && pChatSession->m_pCNMCSChatSession != NULL)
	{
		wstring wstrNickName = pChatSession->m_pCNMCSChatSession->GetMyNickName();
		wstring wstrOppositeNickName = pChatSession->m_pCNMCSChatSession->GetOppositeNickName();

		int iChatHistory = FindChatHistory( wstrNickName, wstrOppositeNickName );
		if( iChatHistory >= 0 )
		{
			pChatSession->m_vecTalk.clear();
			pChatSession->m_vecTalk = m_vecChatHistory[iChatHistory].m_vecTalkHistory;
		}
		
		CKTDGUIListBox* pListBox = (CKTDGUIListBox*)pChatSession->m_pDialog->GetControl(L"ListBoxChat"); //GetListBox();
		if ( pListBox != NULL )
		{
			pListBox->RemoveAllItems();
			for ( UINT i = 0; i < pChatSession->m_vecTalk.size(); ++i )
			{
				pListBox->AddItem( pChatSession->m_vecTalk[i].c_str(), NULL );
			}
			pListBox->SetScrollBarEndPos();
		}

		CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
		if ( pControl != NULL )
		{
			if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
				pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
			{
				return;
			}
		}

		//CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)pChatSession->m_pDialog->GetControl(L"IMEChat");
		//if ( pIMEEdit != NULL )
		//{
		//	pIMEEdit->RequestFocus();
		//}		
	}	
#endif NEW_MESSENGER
}

//{{ kimhc // 2009-09-18 // 길드 탭에 출력할 정보를 얻어오기 위한 핸들러
#ifdef	GUILD_MANAGEMENT

bool CX2Community::Handler_EGS_GET_GUILD_USER_LIST_REQ()
{
	KEGS_GET_GUILD_USER_LIST_REQ kPacket;

	kPacket.m_uiViewPage			= m_uiNowGuildPage;
	kPacket.m_cGuildUserListType	= static_cast < char >( m_eNowGuildViewInfo );

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_GUILD_USER_LIST_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_GET_GUILD_USER_LIST_ACK, 60.f );

	return true;	

}

bool CX2Community::Handler_EGS_GET_GUILD_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_GUILD_USER_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		m_uiNowNumberOfGuildMembersPlayingGame = kEvent.m_iLoginedMemberCount;

		switch ( kEvent.m_cGuildUserListType )
		{
		case GVI_LOCATION:
		case GVI_MEMBER_GRADE:
		case GVI_GUILD_MEMBER_INFO:
			{
				if ( g_pMain->DeleteServerPacket( EGS_GET_GUILD_USER_LIST_ACK ) == false )
					return false;
			}
			break;

		default:
			return false;
			break;
		}

		//{{ 허상형 : [2009/10/8] //	길드원 정보일 경우 먼저 처리
		if( kEvent.m_cGuildUserListType == GVI_GUILD_MEMBER_INFO )
		{
			if( g_pData->GetGuildManager() != NULL || g_pData->GetGuildManager()->GetUIGuild() != NULL )
			{
				g_pData->GetGuildManager()->GetUIGuild()->UpdateGuildMemberInfo( kEvent );
			}

			return true;
		}
		//}} 허상형 : [2009/10/8] //	길드원 정보일 경우 먼저 처리

		m_uiMaxGuildPage = static_cast< BYTE >( kEvent.m_uiTotalPage );
		m_uiNowGuildPage = static_cast< BYTE >( kEvent.m_uiViewPage );

		switch ( kEvent.m_cGuildUserListType )
		{
		case GVI_LOCATION:
		case GVI_MEMBER_GRADE:
			{
				m_vecUserUID.resize( 0 );
				m_vecGuildMemberInfo.resize( 0 );
				
				m_vecGuildMemberInfo = kEvent.m_vecMemberPositionGradeList;

				UpdateGuildTabList();
				UpdateGuildTabUI();
			}
			break;

		/*switch ( kEvent.m_cGuildUserListType )
		{
		case GVI_LOCATION:
			{
				m_vecUserUID.resize( 0 );
                UpdateGuildTabList( kEvent.m_vecMemberPositionList );
				UpdateGuildTabUI();
			}
			break;

		case GVI_MEMBER_GRADE:
			{
				m_vecUserUID.resize( 0 );
				UpdateGuildTabList( kEvent.m_vecMemberGradeList );
				UpdateGuildTabUI();
			}
			break;*/



		default:
			return false;
			break;
		}

		return true;
	}

	return false;
}

#endif	GUILD_MANAGEMENT
//}} kimhc // 2009-09-18 // 길드 탭에 출력할 정보를 얻어오기 위한 핸들러


bool CX2Community::Handler_EGS_INVITE_PVP_ROOM_REQ( UidType UnitUid )
{
	KEGS_INVITE_PVP_ROOM_REQ kPacket;
	kPacket.m_iUnitUID = UnitUid;

	g_pData->GetServerProtocol()->SendPacket( EGS_INVITE_PVP_ROOM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_INVITE_PVP_ROOM_ACK );
	return true;
}
#ifdef SERV_COMMAND_LINE_INVITE_PVP_ROOM
bool CX2Community::Handler_EGS_INVITE_PVP_ROOM_REQ( wstring wszNickName )
{
	KEGS_INVITE_PVP_ROOM_REQ kPacket;
	kPacket.m_iUnitUID = -1;
	kPacket.m_wstrNickName = wszNickName;

	g_pData->GetServerProtocol()->SendPacket( EGS_INVITE_PVP_ROOM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_INVITE_PVP_ROOM_ACK );
	return true;
}
#endif //SERV_COMMAND_LINE_INVITE_PVP_ROOM

bool CX2Community::Handler_EGS_INVITE_PVP_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INVITE_PVP_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_INVITE_PVP_ROOM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_REPLACED_STRING( ( STR_ID_2590, "S", kEvent.m_wstrNickName.c_str() ) ), g_pMain->GetNowState() );
// 			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
// 			g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PVP_INVITE,
// 				CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 7.f, wszText, 
// 				(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
			
			return true;
		}
	}

	return false;

}

bool CX2Community::Handler_EGS_INVITE_PVP_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 해당 state가 아니면 무시
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
		{
		} break;
	default:
		return true;
	}

#ifdef FIX_INVITE_PVP_PLAYER // 김태환
	if ( NULL != g_pMain )
	{
		/// 옵션 설정값 반환
		CX2GameOption::OptionList& kOptionList = g_pMain->GetGameOption().GetOptionList();

		/// 대전 초대 거부를 설정하였다면, 초대 중지
		if ( true == kOptionList.m_bRefuseInvitePVP )
			return true;
	}
#endif // FIX_INVITE_PVP_PLAYER

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INVITE_PVP_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

#ifdef FIX_INVITE_PVP_PLAYER // 김태환
	/// 이미 초대받은 룸 아이디라면, 리턴
	BOOST_FOREACH ( UidType uidRoomUID, m_vecInviteRoomUID )
	{
		if ( uidRoomUID == kEvent.m_kRoomInfo.m_RoomUID )
			return true;
	}
#endif // FIX_INVITE_PVP_PLAYER

	//kEvent.m_wstrNickName;

	wstring wstrRoomInfo = L"";
	
	wstring wstrText = L"";

	wstrText = GET_REPLACED_STRING( ( STR_ID_2548, "L", kEvent.m_wstrNickName ) );

	//WCHAR wszText[256] = L"";
	//StringCchPrintfW( wszText, sizeof(wszText), L"[%s] 파티에서 초대하였습니다. 수락하시겠습니까?", kEvent.m_wstrPartyName.c_str() );

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	
	userData.iTargetPVPChannel = kEvent.m_kRoomInfo.m_uiRoomListID;
	userData.iOpponentUID = kEvent.m_kRoomInfo.m_RoomUID;				// 목표 RoomUID를 OpponenetUID에 저장해 두었다. 착오 없게!
	

	//g_pKTDXApp->GetDeviceManager()->PlaySound( L"PartyInvited.ogg", false, false );  

	g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PVP_DECIDE_INVITE_REQUEST,
		CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 7.f, wstrText.c_str(), 
		(CKTDXStage*) g_pMain->GetNowState(), 
		XCCUM_PVP_INVITE_ACCEPT, XCCUM_PVP_INVITE_REJECT, XCCUM_PVP_INVITE_REJECT );

#ifdef FIX_INVITE_PVP_PLAYER // 김태환
	m_fInviteCoolTime = 10.f;		/// 초대 룸 리스트 초기화 쿨타임 설정 ( 해당 쿨타임이 끝나면, 모든 초대 룸 리스트를 지운다. )

	m_vecInviteRoomUID.push_back( kEvent.m_kRoomInfo.m_RoomUID );	/// 초대 룸 리스트 저장
#endif // FIX_INVITE_PVP_PLAYER

	return true;
}

#ifdef SERV_ED_MONITORING_IN_GAME
void CX2Community::Handler_EGS_ED_MONITORING_BLOCK_REQ(UidType unituid, unsigned char ucType)
{
	if( g_pInstanceData != NULL && g_pInstanceData->GetMoneyMonitoring() == true &&
		g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
	{
		KEGS_ED_MONITORING_BLOCK_REQ kPacket;

		kPacket.m_iUnitUID = unituid;
		kPacket.m_ucType = ucType;

		g_pData->GetServerProtocol()->SendPacket( EGS_ED_MONITORING_BLOCK_REQ, kPacket ); 
		g_pMain->AddServerPacket( EGS_ED_MONITORING_BLOCK_ACK, 60.f );
	}
}
bool CX2Community::Handler_EGS_ED_MONITORING_BLOCK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ED_MONITORING_BLOCK_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ED_MONITORING_BLOCK_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}	
	}

	return false;
}
#endif


#ifdef NEW_MESSENGER
void CX2Community::SetFriendList( KMessengerInfo& refMessengerInfo )
{
	ClearUserList();

	if ( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return;	

	// 일단 그룹 리스트를 만들어주고
	for(std::map< char, std::wstring >::iterator itGroup = refMessengerInfo.m_mapGroup.begin(); itGroup != refMessengerInfo.m_mapGroup.end(); ++itGroup )
	{
		int iCategoryCode = (int)itGroup->first;
		wstring wstrGroupName = itGroup->second;

		CX2Community::MessengerUserGroup* pMgrUserGroup = new CX2Community::MessengerUserGroup();
		pMgrUserGroup->m_GroupName = wstrGroupName;
		pMgrUserGroup->m_uCategoryCode = iCategoryCode;

		m_pMessengerUserList->m_vecUserGroup.push_back( pMgrUserGroup );
	}
	// 유저 리스트를 만들자
	for(std::map< UidType, KFriendInfo >::iterator itUser = refMessengerInfo.m_mapFriendInfo.begin(); itUser != refMessengerInfo.m_mapFriendInfo.end(); ++itUser)
	{
		UidType UnitUid = itUser->first;
		KFriendInfo& friendInfo = itUser->second;

		CX2Community::MessengerUser* pMessengerUser = new CX2Community::MessengerUser();


		pMessengerUser->Update( friendInfo, m_bRefresh );
		if( pMessengerUser->m_uCategoryCode == 0 )
		{
			m_pMessengerUserList->m_vecDefaultGroupUser.push_back( pMessengerUser );
		}
		else
		{
			// 해당하는 그룹을 찾아서 넣자
			bool bInserted = false;
			for(vector<MessengerUserGroup*>::iterator it = m_pMessengerUserList->m_vecUserGroup.begin(); it < m_pMessengerUserList->m_vecUserGroup.end(); ++it )
			{
				MessengerUserGroup* pMessengerUserGroup = *it;
				if(pMessengerUser->m_uCategoryCode == pMessengerUserGroup->m_uCategoryCode)
				{
					pMessengerUserGroup->m_vecUser.push_back(pMessengerUser);
					bInserted = true;
					break;
				}
			}
			if(false == bInserted)
			{
				// 해당하는 그룹이 없는 상황. 뭔가 잘못됐음. 디폴트로 넣어 준다.
				ASSERT( !"GroupID Not Exists!" );
				m_pMessengerUserList->m_vecDefaultGroupUser.push_back( pMessengerUser );
			}
		}
	}
	// 내가 접속하지 않은 동안 와 있었던 초대 메시지들을 저장해주자
	m_vecFriendMessageInfo.insert(m_vecFriendMessageInfo.end(), refMessengerInfo.m_vecFriendMessageInfo.begin(), refMessengerInfo.m_vecFriendMessageInfo.end() );
}




// 친구 요청
bool CX2Community::Handler_EGS_REQUEST_FRIEND_REQ( wstring wstrNickName, wstring wstrMessage )
{
	KEGS_REQUEST_FRIEND_REQ kPacket;
	kPacket.m_wstrNickName = wstrNickName;
	kPacket.m_wstrMessage = wstrMessage;

	if ( kPacket.m_wstrNickName.find( L"__DELETED__") != -1 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_73 ), m_pStage );
		return true;
	}

	// 같은 계정내 아이디 검사
	//m_vecMyUserName.clear();
	if( g_pData != NULL && g_pData->GetMyUser() != NULL)
	{
#ifdef SIMPLE_BUG_FIX
		if( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName == wstrNickName )
		{
#ifdef HARDCODING_STRING_TO_INDEX
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_2539), g_pMain->GetNowState() );
#else
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"자기 자신입니다.", g_pMain->GetNowState() );
#endif HARDCODING_STRING_TO_INDEX
			return true;
		}
		else if( NULL != g_pData->GetMyUser()->GetUnitByNickName(wstrNickName.c_str()) )
		{
#ifdef HARDCODING_STRING_TO_INDEX
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_4883), g_pMain->GetNowState() );
#else
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"같은 계정의 캐릭터는 친구로 등록할수 없습니다.", g_pMain->GetNowState() );
#endif HARDCODING_STRING_TO_INDEX
			return true;
		}
#else // SIMPLE_BUG_FIX
		for(int i=0; i<g_pData->GetMyUser()->GetUnitNum(); ++i)
		{
			CX2Unit *pUnit = g_pData->GetMyUser()->GetUnitByIndex(i);
			if(pUnit != NULL)
			{
				if( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName != wstrNickName &&
					pUnit->GetUnitData().m_NickName == wstrNickName ) 
				{
#ifdef HARDCODING_STRING_TO_INDEX
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_4883), g_pMain->GetNowState() );
#else
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"같은 계정의 캐릭터는 친구로 등록할수 없습니다.", g_pMain->GetNowState() );
#endif HARDCODING_STRING_TO_INDEX
					
					return true;
				}
				
			}
		}
#endif // SIMPLE_BUG_FIX
	}

	// 이미 친구목록에 있는지 검사
	if( NULL != GetMessengerUser(wstrNickName.c_str() ) )
	{		
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3719 ), g_pMain->GetNowState() );
		return true;
	}

	SAFE_DELETE_DIALOG( m_pDLGFriendReqWait );
	m_pDLGFriendReqWait = g_pMain->KTDGUIMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_69 ), m_pStage );

	g_pData->GetServerProtocol()->SendPacket( EGS_REQUEST_FRIEND_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_REQUEST_FRIEND_ACK );
	return true;
}

bool CX2Community::Handler_EGS_REQUEST_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REQUEST_FRIEND_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGFriendReqWait );
	
	if( g_pMain->DeleteServerPacket( EGS_REQUEST_FRIEND_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{				
			// 친구를 등록 신청 상태로 추가해 둔다.
			MessengerUser* pNewUser = new MessengerUser;
			pNewUser->m_UnitUID = kEvent.m_iUnitUID;
			pNewUser->m_NickName = kEvent.m_wstrNickName;
			pNewUser->m_uCategoryCode = 0;		// 일단 분류 없음에 넣자
			pNewUser->m_eStatus = MessengerUser::FS_REQUESTED;
			pNewUser->m_ePosition = MessengerUser::FP_OFF_LINE;

			m_pMessengerUserList->m_vecDefaultGroupUser.push_back(pNewUser);
			
			OpenAddFriendWindow(false);
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING(( STR_ID_3720, "L", kEvent.m_wstrNickName )), g_pMain->GetNowState() );
			
			ResetUserListUI();
			return true;
		}
	}

	return false;
}

bool CX2Community::Handler_EGS_REQUEST_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REQUEST_FRIEND_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	// 누군가 나에게 친구등록 신청을 해 왔다
	// 1. 등록 대기중 상태로 등록한다.
	MessengerUser* pNewUser = new MessengerUser;
	pNewUser->m_UnitUID = kEvent.m_iUnitUID;
	pNewUser->m_NickName = kEvent.m_wstrNickName;
	pNewUser->m_uCategoryCode = 0;		// 일단 분류 없음에 넣자
	pNewUser->m_eStatus = MessengerUser::FS_WAITING;
	pNewUser->m_ePosition = MessengerUser::FP_OFF_LINE;

	m_pMessengerUserList->m_vecDefaultGroupUser.push_back(pNewUser);
	// 2. 알림 메시지를 만든다.
	{
		if ( m_pFriendReqMsg != NULL )
			return false;

		CX2Community::FriendReqMsg* pNewFriendReqMsg = new CX2Community::FriendReqMsg();
		pNewFriendReqMsg->m_FromNickName = kEvent.m_wstrNickName;
		pNewFriendReqMsg->m_Msg = kEvent.m_wstrMessage;
		pNewFriendReqMsg->m_FromUnitUid = kEvent.m_iUnitUID;

		m_pFriendReqMsg = pNewFriendReqMsg;
	}
	ResetUserListUI();

	return true;
}

// 수락 대기 중인 캐릭터에 대해 친구 수락
bool CX2Community::Handler_EGS_ACCEPT_FRIEND_REQ( UidType AcceptID )
{
	KEGS_ACCEPT_FRIEND_REQ kPacket;
	kPacket = AcceptID;
	
	g_pData->GetServerProtocol()->SendPacket( EGS_ACCEPT_FRIEND_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ACCEPT_FRIEND_ACK );
	return true;
}

bool CX2Community::Handler_EGS_ACCEPT_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ACCEPT_FRIEND_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ACCEPT_FRIEND_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			//wstring wstrNickName = kEvent.m_wstrNickName;
			// 친구 추가를 허가했다.
			//** 한번 리프레쉬 해 줘야 한다
			MessengerUser* pMessengerUser = GetMessengerUser( kEvent.m_iUnitUID );
			if(pMessengerUser != NULL)
			{
				pMessengerUser->m_eStatus = MessengerUser::FS_NORMAL;
			}
			else
			{
				ASSERT( !"Friend Accepted without Getting EGS_REQUEST_FRIEND_NOT" );
// 				MessengerUser* pNewUser = new MessengerUser;
// 				pNewUser->m_UnitUID = kEvent.m_iUnitUID;
// 				pNewUser->m_NickName = kEvent.m_wstrNickName;
// 				pNewUser->m_uCategoryCode = 0;		// 일단 분류 없음에 넣자
// 				pNewUser->m_eStatus = MessengerUser::FS_NORMAL;
// 				pNewUser->m_ePosition = MessengerUser::FP_OFF_LINE;
			}
			

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_3721, "L", pMessengerUser->m_NickName ) ), g_pMain->GetNowState() );

			ResetUserListUI();
			return true;
		}
	}

	return false;
}

bool CX2Community::Handler_EGS_ACCEPT_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ACCEPT_FRIEND_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//UidType UnitUid = kEvent.m_iUnitUID;
	//wstring wstrNickName = kEvent.m_wstrNickName;

	
	//** g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"kEvent.m_wstrNickName님이 당신의 친구 요청을 받아들였습니다", g_pMain->GetNowState() );
	//** 한번 리프레쉬 해 줘야 한다
	MessengerUser* pMessengerUser = GetMessengerUser( kEvent.m_iUnitUID );
	if(pMessengerUser != NULL)
	{
		pMessengerUser->m_eStatus = MessengerUser::FS_NORMAL;
	}
	else
	{
		ASSERT( !"Friend Accepted without My Request" );
// 		MessengerUser* pNewUser = new MessengerUser;
// 		pNewUser->m_UnitUID = kEvent.m_iUnitUID;
// 		pNewUser->m_NickName = kEvent.m_wstrNickName;
// 		pNewUser->m_uCategoryCode = 0;		// 일단 분류 없음에 넣자
// 		pNewUser->m_eStatus = MessengerUser::FS_NORMAL;
// 		pNewUser->m_ePosition = MessengerUser::FP_OFF_LINE;
	}

	ResetUserListUI();


	return true;

}

// 수락 대기 중인 캐릭터에 대해 친구 거절
bool CX2Community::Handler_EGS_DENY_FRIEND_REQ( UidType DenyID )
{
	KEGS_DENY_FRIEND_REQ kPacket;
	kPacket = DenyID;

	g_pData->GetServerProtocol()->SendPacket( EGS_DENY_FRIEND_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DENY_FRIEND_ACK );
	return true;
}
bool CX2Community::Handler_EGS_DENY_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DENY_FRIEND_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_DENY_FRIEND_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			//UidType UnitUid = kEvent.m_iUnitUID;
			//wstring wstrNickName = kEvent.m_wstrNickName;

			// TODO
			//** g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"친구 요청을 거절했습니다.", g_pMain->GetNowState() );
			// 임시등록 되어 있던 걸 삭제해준다
			// 삭제 처리 해주자
			DeleteFriend( kEvent.m_iUnitUID );

			ResetUserListUI();
			return true;
		}
	}

	return false;
}

bool CX2Community::Handler_EGS_DENY_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DENY_FRIEND_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	UidType UnitUid = kEvent.m_iUnitUID;
	//wstring wstrNickName = kEvent.m_wstrNickName;
	
	
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_3713, "L", kEvent.m_wstrNickName ) ), g_pMain->GetNowState() );
	// 임시등록 되어 있던 걸 삭제해준다
	// 삭제 처리 해주자
	DeleteFriend( kEvent.m_iUnitUID );
	ResetUserListUI();

	return true;

}

// 친구 차단 요청
bool CX2Community::Handler_EGS_BLOCK_FRIEND_REQ( UidType BlockID )
{
	KEGS_BLOCK_FRIEND_REQ kPacket;
	kPacket = BlockID;

	g_pData->GetServerProtocol()->SendPacket( EGS_BLOCK_FRIEND_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BLOCK_FRIEND_ACK );
	return true;
}
bool CX2Community::Handler_EGS_BLOCK_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BLOCK_FRIEND_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_BLOCK_FRIEND_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			UidType iUnitUid = kEvent.m_iUnitUID;
			//** g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_63, "L", pTutorUnit->m_wstrNickName ) ), g_pMain->GetNowState() );
			MessengerUser* pMessengerUser = GetMessengerUser( kEvent.m_iUnitUID );
			if(pMessengerUser != NULL)
			{
				pMessengerUser->m_eStatus = MessengerUser::FS_BLOCKED;
			}
			else
			{
				ASSERT( !"Friend Block ACK'ed but not in my friend list" );
			}

			ResetUserListUI();
			return true;
		}
	}

	return false;
}
bool CX2Community::Handler_EGS_BLOCK_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BLOCK_FRIEND_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	UidType UnitUid = kEvent.m_iUnitUID;
	//** g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"너 차단당함 ㅋ", g_pMain->GetNowState() );

	return true;

}


// 친구 차단 해제
bool CX2Community::Handler_EGS_UNBLOCK_FRIEND_REQ( UidType UnblockID )
{
	KEGS_UNBLOCK_FRIEND_REQ kPacket;
	kPacket = UnblockID;

	g_pData->GetServerProtocol()->SendPacket( EGS_UNBLOCK_FRIEND_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_UNBLOCK_FRIEND_ACK );
	return true;
}
bool CX2Community::Handler_EGS_UNBLOCK_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UNBLOCK_FRIEND_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_UNBLOCK_FRIEND_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			UidType iUnitUid = kEvent.m_iUnitUID;
			
			//** g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"차단 해제해뜸", g_pMain->GetNowState() );
			MessengerUser* pMessengerUser = GetMessengerUser( kEvent.m_iUnitUID );
			if(pMessengerUser != NULL)
			{
				pMessengerUser->m_eStatus = MessengerUser::FS_NORMAL;
			}
			else
			{
				ASSERT( !"Friend Unblock ACK'ed but not in my friend list" );
			}

			ResetUserListUI();		
			return true;
		}
	}

	return false;
}
bool CX2Community::Handler_EGS_UNBLOCK_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UNBLOCK_FRIEND_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	UidType UnitUid = kEvent.m_iUnitUID;
	//** TODO
	//** g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"이번엔 봐준다", g_pMain->GetNowState() );

	return true;

}

// 친구 삭제 요청
bool CX2Community::Handler_EGS_DELETE_FRIEND_REQ( UidType DeleteID )
{
	KEGS_DELETE_FRIEND_REQ kPacket;
	kPacket = DeleteID;

	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_FRIEND_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DELETE_FRIEND_ACK );
	return true;
}
bool CX2Community::Handler_EGS_DELETE_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_FRIEND_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_DELETE_FRIEND_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			// 삭제 처리 해주자
			DeleteFriend( kEvent.m_iUnitUID );
			ResetUserListUI();
			return true;
		}
	}

	return false;
}
bool CX2Community::Handler_EGS_DELETE_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_FRIEND_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	UidType UnitUid = kEvent.m_iUnitUID;
	// TODO : 해당 유저를 삭제한다
	DeleteFriend( kEvent.m_iUnitUID );
	ResetUserListUI();
	// g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"넌 버림받았어", g_pMain->GetNowState() );

	return true;

}

// 친구 그룹 이동 요청
bool CX2Community::Handler_EGS_MOVE_FRIEND_REQ( UidType UnitUid, int TargetGroupID )
{
	KEGS_MOVE_FRIEND_REQ kPacket;
	kPacket.m_iUnitUID = UnitUid;
	kPacket.m_cTargetGroupID = (char)TargetGroupID;

	g_pData->GetServerProtocol()->SendPacket( EGS_MOVE_FRIEND_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_MOVE_FRIEND_ACK );
	return true;
}
bool CX2Community::Handler_EGS_MOVE_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MOVE_FRIEND_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_MOVE_FRIEND_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{				
			// 1. 원래 있던 데서 삭제하고 포인터만 살짝 가져와서
			CX2Community::MessengerUser* pTargetUser = NULL;
			for( vector<MessengerUser*>::iterator it = m_pMessengerUserList->m_vecDefaultGroupUser.begin(); it != m_pMessengerUserList->m_vecDefaultGroupUser.end(); ++it)
			{
				CX2Community::MessengerUser* pMessengerUser = *it;
				if ( pMessengerUser->m_UnitUID == kEvent.m_iUnitUID )
				{
					pTargetUser = pMessengerUser;
					m_pMessengerUserList->m_vecDefaultGroupUser.erase(it);
					break;
				}					
			}

			if(pTargetUser == NULL)
			{
				for( vector<MessengerUserGroup*>::iterator itGp = m_pMessengerUserList->m_vecUserGroup.begin(); itGp != m_pMessengerUserList->m_vecUserGroup.end(); ++itGp)
				{
					CX2Community::MessengerUserGroup* pMessengerUserGroup = *itGp;
					for( vector<MessengerUser*>::iterator itUsr = pMessengerUserGroup->m_vecUser.begin(); itUsr != pMessengerUserGroup->m_vecUser.end(); ++itUsr)
					{
						CX2Community::MessengerUser* pMessengerUser = *itUsr;
						if( pMessengerUser->m_UnitUID == kEvent.m_iUnitUID )
						{
							pTargetUser = pMessengerUser;
							pMessengerUserGroup->m_vecUser.erase(itUsr);
							break;
						}
					}
				}
			}

			// 2. 해당 포인터를 목표 그룹으로 옮겨준다
			UidType UnitUid = kEvent.m_iUnitUID;
			int TargetGroupID = (int)kEvent.m_cTargetGroupID;
			if(pTargetUser != NULL)
			{
				pTargetUser->m_uCategoryCode = (int)kEvent.m_cTargetGroupID;
				if(kEvent.m_cTargetGroupID == 0)
				{
					m_pMessengerUserList->m_vecDefaultGroupUser.push_back(pTargetUser);
				}
				else
				{
					MessengerUserGroup* pMessengerGroup = GetMgrUserGroup(pTargetUser->m_uCategoryCode);
					if( pMessengerGroup != NULL )
					{
						pMessengerGroup->m_vecUser.push_back(pTargetUser);
					}
				}
			}
		}
		ResetUserListUI();
		return true;
	}

	return false;
}

// 그룹 생성 요청
bool CX2Community::Handler_EGS_MAKE_FRIEND_GROUP_REQ( wstring wstrGroupName )
{
	KEGS_MAKE_FRIEND_GROUP_REQ kPacket;
	kPacket.m_wstrGroupName = wstrGroupName;

	g_pData->GetServerProtocol()->SendPacket( EGS_MAKE_FRIEND_GROUP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_MAKE_FRIEND_GROUP_ACK );
	return true;
}
bool CX2Community::Handler_EGS_MAKE_FRIEND_GROUP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MAKE_FRIEND_GROUP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_MAKE_FRIEND_GROUP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{				
			// 그룹을 추가해 주자
			MessengerUserGroup* pMessengerGroup = new MessengerUserGroup;
			pMessengerGroup->m_GroupName = kEvent.m_wstrGroupName;
			pMessengerGroup->m_uCategoryCode = (int)kEvent.m_cGroupID;

			m_pMessengerUserList->m_vecUserGroup.push_back(pMessengerGroup);			

			ResetUserListUI();

			return true;
		}
	}

	return false;
}

// 그룹 이름 바꾸기 요청
bool CX2Community::Handler_EGS_RENAME_FRIEND_GROUP_REQ( int GroupID, wstring wstrGroupName )
{
	if( GroupID == 0 )
	{
		//** 변경할 수 없다는 경고문을 띄울 것
		return false;
	}

	KEGS_RENAME_FRIEND_GROUP_REQ kPacket;
	kPacket.m_cGroupID = (char)GroupID;
	kPacket.m_wstrGroupName = wstrGroupName;
	
	g_pData->GetServerProtocol()->SendPacket( EGS_RENAME_FRIEND_GROUP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RENAME_FRIEND_GROUP_ACK );
	return true;

}
bool CX2Community::Handler_EGS_RENAME_FRIEND_GROUP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RENAME_FRIEND_GROUP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_RENAME_FRIEND_GROUP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{				
			// 그룹 이름을 바꿔 주자
			MessengerUserGroup* pMessengerGroup = GetMgrUserGroup( (int)kEvent.m_cGroupID );
			if(pMessengerGroup != NULL)
			{
				pMessengerGroup->m_GroupName = kEvent.m_wstrGroupName;
				ResetUserListUI();
				return true;
			}
			
#ifndef _SERVICE_
			// 패킷은 맞는데 그룹을 못 찾았습니다
			WCHAR buf[256] = {0};
			StringCchPrintf(buf, 256, L"Handler_EGS_RENAME_FRIEND_GROUP_ACK\nPacket Valid, Group Not Found.\nGroupID : %d", (int)kEvent.m_cGroupID);
			//wsprintf(buf, L"Handler_EGS_RENAME_FRIEND_GROUP_ACK\nPacket Valid, Group Not Found.\nGroupID : %d", (int)kEvent.m_cGroupID);
			MessageBox( g_pKTDXApp->GetHWND(), buf, L"Error", MB_OK );
#endif

		}
	}

	return false;
}


// 그룹 삭제 요청
bool CX2Community::Handler_EGS_DELETE_FRIEND_GROUP_REQ( int GroupId )
{
	if( GroupId == 0 )
	{
		//** 삭제할 수 없다는 경고문을 띄울 것
		return false;
	}

	KEGS_DELETE_FRIEND_GROUP_REQ kPacket;
	kPacket.m_cGroupID = (char) GroupId;

	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_FRIEND_GROUP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DELETE_FRIEND_GROUP_ACK );
	return true;
}
bool CX2Community::Handler_EGS_DELETE_FRIEND_GROUP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_FRIEND_GROUP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_DELETE_FRIEND_GROUP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
			int GroupID = (int)kEvent.m_cGroupID;

			for( vector<MessengerUserGroup*>::iterator itGrp = m_pMessengerUserList->m_vecUserGroup.begin(); itGrp != m_pMessengerUserList->m_vecUserGroup.end(); ++itGrp )
			{
				// 해당 그룹을 찾아서
				MessengerUserGroup* pMessengerGroup = *itGrp;
				if(pMessengerGroup != NULL && pMessengerGroup->m_uCategoryCode == GroupID)
				{
					for(vector<MessengerUser*>::iterator it = pMessengerGroup->m_vecUser.begin(); it != pMessengerGroup->m_vecUser.end(); ++it)
					{
						// 해당 그룹의 친구들을 디폴트 그룹으로 옮긴다
						MessengerUser* pMessengerUser = *it;
						if(pMessengerUser != NULL)
						{
							pMessengerUser->m_uCategoryCode = 0;
							m_pMessengerUserList->m_vecDefaultGroupUser.push_back(pMessengerUser);
						}
					}
					pMessengerGroup->m_vecUser.clear();
					m_pMessengerUserList->m_vecUserGroup.erase(itGrp);
					SAFE_DELETE(pMessengerGroup);

					ResetUserListUI();

					return true;
				}
			}

#ifndef _SERVICE_
			// 패킷은 맞는데 그룹을 못 지웠습니다. 어라?
			WCHAR buf[256] = {0};
			StringCchPrintf(buf, 256, L"EGS_DELETE_FRIEND_GROUP_ACK\nPacket Valid, Group Delete Failed.\nGroupID : %d", GroupID);
			//wsprintf(buf, L"EGS_DELETE_FRIEND_GROUP_ACK\nPacket Valid, Group Delete Failed.\nGroupID : %d", GroupID);
			MessageBox( g_pKTDXApp->GetHWND(), buf, L"Error", MB_OK );
#endif
		}

		
	}

	return false;
}

bool CX2Community::Handler_EGS_FRIEND_POSITION_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_FRIEND_POSITION_UPDATE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	// TODO : 유저정보 업데이트
	for( std::map< UidType, char >::iterator it = kEvent.m_mapFriendPosition.begin(); it != kEvent.m_mapFriendPosition.end(); ++it)
	{
		UidType UnitUid = it->first;
		MessengerUser::FRIEND_POSITION pos = (MessengerUser::FRIEND_POSITION) it->second;
		CX2Community::MessengerUser* pMessengerUser = GetMessengerUser(UnitUid);
		if(pMessengerUser != NULL)
		{
			pMessengerUser->m_ePosition = pos;
		}
	}
	ResetUserListUI();
	
	return true;

}

bool CX2Community::DeleteFriend( UidType UnitUid )
{
#ifdef CLOSE_MESSENGER_WHEN_DELETE_FRIEND
	CX2Community::ChatSession* pFriendChatSession = FindChatSession( UnitUid );
	if( pFriendChatSession != NULL )
	{
		for ( int i = 0; i < (int)m_vecChatSession.size(); i++ )
		{
			if( pFriendChatSession == m_vecChatSession[i] )
			{
				SetChatHistory(pFriendChatSession);
				SetChatBoxShow(pFriendChatSession, false);

				if( m_pOpendChatSession == pFriendChatSession )
				{
					m_pOpendChatSession = NULL;
				}

				SAFE_DELETE( pFriendChatSession );
				m_vecChatSession.erase( m_vecChatSession.begin() + i );
				i--;
			}
		}
	}
#endif CLOSE_MESSENGER_WHEN_DELETE_FRIEND

	for( vector<MessengerUser*>::iterator it = m_pMessengerUserList->m_vecDefaultGroupUser.begin(); it != m_pMessengerUserList->m_vecDefaultGroupUser.end(); ++it)
	{
		CX2Community::MessengerUser* pMessengerUser = *it;
		if( pMessengerUser->m_UnitUID == UnitUid )
		{
			SAFE_DELETE(pMessengerUser);
			m_pMessengerUserList->m_vecDefaultGroupUser.erase(it);
			return true;
		}
	}

	for( vector<MessengerUserGroup*>::iterator itGp = m_pMessengerUserList->m_vecUserGroup.begin(); itGp != m_pMessengerUserList->m_vecUserGroup.end(); ++itGp)
	{
		CX2Community::MessengerUserGroup* pMessengerUserGroup = *itGp;
		for( vector<MessengerUser*>::iterator itUsr = pMessengerUserGroup->m_vecUser.begin(); itUsr != pMessengerUserGroup->m_vecUser.end(); ++itUsr)
		{
			CX2Community::MessengerUser* pMessengerUser = *itUsr;
			if( pMessengerUser->m_UnitUID == UnitUid )
			{
				SAFE_DELETE(pMessengerUser);
				pMessengerUserGroup->m_vecUser.erase(itUsr);
				return true;
			}
		}
	}
	return false;
}
#endif NEW_MESSENGER

#ifdef ADDED_RELATIONSHIP_SYSTEM
void CX2Community::InitRelationUI()
{

}
void CX2Community::SetRelationshipUI ( SEnum::RELATIONSHIP_TYPE eType_ )
{
	CKTDGUIRadioButton *pRadioButtonSolo = static_cast< CKTDGUIRadioButton * >( m_pDlgBack->GetControl( L"relationSolo" ) );
	pRadioButtonSolo->SetShowEnable( false, false );
	CKTDGUIRadioButton *pRadioButtonCouple = static_cast< CKTDGUIRadioButton * >( m_pDlgBack->GetControl( L"relationCouple" ) );
	pRadioButtonCouple->SetShowEnable( false, false );
	CKTDGUIRadioButton *pRadioButtonMarried = static_cast< CKTDGUIRadioButton * >( m_pDlgBack->GetControl( L"relationMarried" ) );
	pRadioButtonMarried->SetShowEnable( false, false );


	switch ( eType_ )
	{
	case SEnum::RT_SOLO :		
		pRadioButtonSolo->SetShowEnable( true, true );
		break;

	case SEnum::RT_COUPLE :
		pRadioButtonCouple->SetShowEnable( true, true );
		break;

	case SEnum::RT_MARRIED :
		pRadioButtonMarried->SetShowEnable( true, true );
		break;
	}


}


void CX2Community::UpdateCoupleNameAndChannel ()
{
	if ( NULL != g_pData->GetRelationshipManager() &&
		NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() &&
		NULL != m_pDlgRelation )
	{
		const D3DXCOLOR colorForLogOff( 0.4f, 0.4f, 0.4f, 0.8f );
		const D3DXCOLOR colorForLogOn( 0.f, 0.f, 0.f, 1.0f );
		CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();
		CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();


		if ( NULL != pRelationshipManager && NULL != pRelationshipInfo )
		{
			for ( int i = 0; i < _CONST_RELATIONSHIP_UI_::g_iMaxRelationMemberNumber; ++i )
			{
				m_arrRelationUIControls[i].m_pStaticNickName = 
					static_cast< CKTDGUIStatic* >( m_pDlgRelation->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"name" ), i + 1 ) ) ) );
				m_arrRelationUIControls[i].m_pStaticNickName->SetShow(false);
				m_arrRelationUIControls[i].m_pStaticNickName->GetString(0)->color = colorForLogOff;
				

				m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan	=
					static_cast< CKTDGUIStatic* >( m_pDlgRelation->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"state" ), i + 1 ) ) ) );
				m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetShow(false);
				m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->GetString(0)->color = colorForLogOff;
				
				m_arrRelationUIControls[i].m_pStaticChannel	=
					static_cast< CKTDGUIStatic* >( m_pDlgRelation->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"channel" ), i + 1 ) ) ) );		
				m_arrRelationUIControls[i].m_pStaticChannel->SetShow(false);
				m_arrRelationUIControls[i].m_pStaticChannel->GetString(0)->color = colorForLogOff;
			}

			if ( pRelationshipManager->GetMyRelationshipInfo()->m_eRelationStateType != SEnum::RT_SOLO )
			{
				if ( NULL != g_pData->GetMyUser() &&
					 NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					m_vecUserUID.clear();
					m_vecUserUID.push_back( g_pData->GetMyUser()->GetSelectUnit()->GetUID () );
					m_vecUserUID.push_back( pRelationshipInfo->m_iRelationTargetUserUid );
				}

				WCHAR wszText[64] = L"";	
				for ( int i = 0; i < _CONST_RELATIONSHIP_UI_::g_iMaxRelationMemberNumber; ++i )
				{
					swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"User%d", i+1 );
					CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDlgRelation->GetControl( wszText );
					if( NULL != pButton )
					{
						pButton->SetShowEnable( true, true );						
					}
				}


				vector<CX2RelationshipManager::RelationshipCommunityInfo> & vecInfoData = pRelationshipManager->GetRelationshipCommunityInfo();
				
				int i = 0;
				BOOST_FOREACH ( CX2RelationshipManager::RelationshipCommunityInfo RelationCommunityInfoData, vecInfoData )
				{
					if ( NULL != m_arrRelationUIControls[i].m_pStaticNickName &&
						NULL != m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan &&
						 NULL != m_arrRelationUIControls[i].m_pStaticChannel )
					{					
						m_arrRelationUIControls[i].m_pStaticNickName->SetString ( 0, RelationCommunityInfoData.m_wstrNickName.c_str() );
						m_arrRelationUIControls[i].m_pStaticChannel->SetString( 0, RelationCommunityInfoData.m_wstrChannelName.c_str() );

						if ( TRUE == RelationCommunityInfoData.m_bIsConnect )
						{
							switch ( RelationCommunityInfoData.m_cPosition )
							{
							case CX2Room::RT_PVP:
								{
									m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0,
										GET_STRING( STR_ID_4522 ) );
								}
								break;

							case CX2Room::RT_DUNGEON:
								{
									m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0,
										GET_STRING( STR_ID_2546 ) );
								}
								break;

							case CX2Room::RT_SQUARE:
								{
									m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0,
										GET_STRING( STR_ID_4521 ) );
								}
								break;

							case CX2Room::RT_BATTLE_FIELD:
								{
									SEnum::VILLAGE_MAP_ID eMapID = static_cast< SEnum::VILLAGE_MAP_ID >( RelationCommunityInfoData.m_iMapID );
									const WCHAR* pwszBattleFieldName = g_pData->GetBattleFieldManager().GetBattleFieldNameByBattleFieldId( static_cast<int>( eMapID ) );

									if ( NULL != pwszBattleFieldName )
									{
										wstring wstrBattleFieldName = pwszBattleFieldName;
										if ( wstrBattleFieldName.size() > 4 )	/// 글자 사이즈를 4개로 제한
										{
											wstrBattleFieldName.resize( 4 );
											wstrBattleFieldName += L"..";
										}

										m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0, 
											wstrBattleFieldName.c_str() );
									}
								} break;

							default:	// 마을
								{
									SEnum::VILLAGE_MAP_ID eMapID = static_cast< SEnum::VILLAGE_MAP_ID >( RelationCommunityInfoData.m_iMapID );
									if( g_pData->GetLocationManager()->IsVillage( eMapID ) == true ||
										g_pData->GetLocationManager()->IsDungeonGate( eMapID ) == true )
									{
										CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eMapID );
										if( NULL != pVillageTemplet )
										{
											m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0, 
												pVillageTemplet->m_Name.c_str() );
										}
									}
								}
								break;
							}


							m_arrRelationUIControls[i].m_pStaticNickName->SetShow ( true );
							m_arrRelationUIControls[i].m_pStaticNickName->GetString(0)->color = colorForLogOn;
							m_arrRelationUIControls[i].m_pStaticChannel->SetShow ( true );
							m_arrRelationUIControls[i].m_pStaticChannel->GetString(0)->color = colorForLogOn;
							m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetShow ( true );
							m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->GetString(0)->color = colorForLogOn;							

						}

						else
						{

							CTime cTime( RelationCommunityInfoData.m_tLastLogOutTime );
							m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetString( 0, 
								GetWStrAboutTimeFromLogOutToNow( cTime ).c_str() );

							m_arrRelationUIControls[i].m_pStaticChannel->SetShow( false );
						}		
						m_arrRelationUIControls[i].m_pStaticNickName->SetShow( true );
						m_arrRelationUIControls[i].m_pStaticLocationOrGradeOrTimeSpan->SetShow( true );
					}
					i += 1;
				}
			}
		}
	}
}

void CX2Community::UpdateCoupleStateAndDurationDay()
{
	if ( m_pDlgRelation != NULL )
	{
		CKTDGUIStatic* pStatic = static_cast< CKTDGUIStatic* >( m_pDlgRelation->GetControl( L"couple_state" ) );

		if ( pStatic != NULL )
		{
			wstring wstrRelationType = L"";
			wstring wstrTextNotice= L"";
			
			if ( NULL != g_pData->GetRelationshipManager() && 
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				CX2RelationshipManager::RelationshipInfo * pMyRealtionInfo = 
					g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
				
				switch ( pMyRealtionInfo->m_eRelationStateType )
				{
					case SEnum::RT_COUPLE :
						wstrRelationType = GET_STRING ( STR_ID_24492 );
						wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_24463, "i", pMyRealtionInfo->GetDurationDay()) );
						break;
					case SEnum::RT_MARRIED :
						wstrRelationType = GET_STRING ( STR_ID_24493 );
						wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_24463, "i", pMyRealtionInfo->GetDurationDay()) );
						break;
					default :		// 인연이 없는 경우도 여기에 포함된다.
						break;
				}

#ifdef CLIENT_COUNTRY_EU
				// 유럽은 뒤에 붙는 글짜가 너무 길어서 따로 표시
				wstrTextNotice = wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_22727, "i", pMyRealtionInfo->GetDurationDay()) );
#endif  CLIENT_COUNTRY_EU
			}

			pStatic->SetString( 0, wstrRelationType.c_str() );			
			pStatic->SetString( 1, wstrTextNotice.c_str() );		
			pStatic->SetShowEnable( true, true );
		}
	}
}



void CX2Community::ResetRelationshipTabUI()
{
	if ( g_pData->GetRelationshipManager() == NULL )
	{
		ASSERT( !"Wrong Path" );
		return;
	}

	if ( NULL != g_pData->GetRelationshipManager() &&
		NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
	{
		CX2RelationshipManager::RelationshipInfo * pRelationInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ;

		if ( pRelationInfo->m_eRelationStateType == SEnum::RT_SOLO )
		{
			CKTDGUIButton*			pButtonBuyCoupleRing		= m_pDlgRelation->GetButton_LUA( "Button_Propose" );
			pButtonBuyCoupleRing->SetColor ( D3DXCOLOR ( 1.0f, 1.0f, 1.0f, 1.0f ) );
			pButtonBuyCoupleRing->SetShowEnable( true, true );

		}
		else
		{		
			vector<CX2RelationshipManager::RelationshipCommunityInfo> & vecMyInfo = g_pData->GetRelationshipManager()->GetRelationshipCommunityInfo();
			
			CKTDGUIButton*			pButtonPropose		= m_pDlgRelation->GetButton_LUA( "Button_Propose" );
			CKTDGUIButton*			pButtonTeleport		= m_pDlgRelation->GetButton_LUA( "Button_Teleport" );
			CKTDGUIButton*			pButtonKick			= m_pDlgRelation->GetButton_LUA( "Button_Kick" );
			CKTDGUIButton*			pButtonRefresh		= m_pDlgRelation->GetButton_LUA( "Return" );
			UpdateCoupleNameAndChannel();
			UpdateCoupleStateAndDurationDay();

			pButtonPropose->SetColor ( D3DXCOLOR ( 0.4f, 0.4f,0.4f, 0.4f ) );
			pButtonKick->SetColor ( D3DXCOLOR ( 0.4f, 0.4f,0.4f, 0.4f ) );
			pButtonRefresh->SetColor  ( D3DXCOLOR ( 0.4f, 0.4f,0.4f, 0.4f ) );
			pButtonPropose->SetShowEnable( true, false );
			pButtonTeleport->SetShowEnable( false, false );
			pButtonKick->SetShowEnable( true, false );
			pButtonRefresh->SetShowEnable( true, false );
			
			if ( FALSE == vecMyInfo.empty() )
			{
				
				// Get Relation Dummy Unit
				if ( NULL == m_pRelationUnit )
				{
					BOOST_FOREACH ( CX2RelationshipManager::RelationshipCommunityInfo RelationMyInfo, vecMyInfo )
					{
						wstring TargetNickname = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname;
						if ( RelationMyInfo.m_wstrNickName == TargetNickname )
						{
							AllocateRelationDummyUnit ( TargetNickname, RelationMyInfo.m_cUnitClass );				
						}
					}
				}

				// Get Unit Viewer
				if ( true == m_vecUnitViewerUI.empty () )
				{	
					int iPhase = 0;
					BOOST_FOREACH ( CX2RelationshipManager::RelationshipCommunityInfo RelationMyInfo, vecMyInfo )
					{
						CX2UnitViewerUI * pUnitViewer = CX2UnitViewerUI::CreateUnitViewerUI();
						if ( iPhase == 0 ) 
						{
							pUnitViewer->SetUnit ( g_pData->GetMyUser()->GetSelectUnit(),  CX2UnitViewerUI::UVS_WATCH );
						}
						else			  
						{
							if ( NULL != m_pRelationUnit )
							{
								pUnitViewer->SetUnit ( m_pRelationUnit, CX2UnitViewerUI::UVS_WATCH );
							}
						}

						m_vecUnitViewerUI.push_back( pUnitViewer );
						iPhase += 1;
					}
				}

				for ( int iPhase = 0; iPhase < _CONST_RELATIONSHIP_UI_::g_iMaxRelationMemberNumber; ++iPhase )
				{
					if ( static_cast <int> ( m_vecUnitViewerUI.size() ) > iPhase )
					{
						CX2UnitViewerUI * pUnitViewer = m_vecUnitViewerUI[iPhase];
						CKTDGUIDialogType * pDialogType = NULL;
						if ( iPhase == 0 )		pDialogType = &m_pDlgRelation;
						else					pDialogType = &m_pDlgRelationOnlyPartnerShowing;

						if ( NULL != pDialogType )
						{	
							if ( iPhase == 0 )
							{
								pUnitViewer->UpdateEqip(true);
							}
							else
							{
								CX2RelationshipManager::RelationshipInfo * pMyRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
								if ( NULL != pMyRelationshipInfo )
								{			
									pUnitViewer->RemoveAllEquips();
									pUnitViewer->ReplaceEmptyToBasic();
									map< int, KInventoryItemInfo >::iterator it = pMyRelationshipInfo->m_mapOtherUnitEquippedItem.begin();
									for (; it != pMyRelationshipInfo->m_mapOtherUnitEquippedItem.end(); ++it )
									{						
										pUnitViewer->AddEquipByTID( it->second.m_kItemInfo.m_iItemID );
									}
								}		
							}
							pUnitViewer->SetFixed( true );
							if ( iPhase == 0 )
								pUnitViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
							else
								pUnitViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
							pUnitViewer->GetXSkinAnim()->SetApplyMotionOffset( false );
							pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT ); 
							pUnitViewer->SetHideSelectUnitWeapon();
							D3DXVECTOR3 UnitviewerPos = (*pDialogType)->GetDummyPos(2);
							pUnitViewer->SetPositionOnScr( UnitviewerPos.x, UnitviewerPos.y, UnitviewerPos.z, UnitviewerPos.z );		// 유닛뷰어 위치는 스크립트에 있음	
							

							D3DXVECTOR3 UnitviewerLightPos = (*pDialogType)->GetDummyPos(3);					
							pUnitViewer->SetLightPos( UnitviewerLightPos );
							(*pDialogType)->SetHasUnit( (CKTDGObject *) pUnitViewer );
							pUnitViewer->SetShowObject( true );
							pUnitViewer->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
						}
				
						SetRelationshipMemberIcon( iPhase, true, vecMyInfo[iPhase].m_cUnitClass, vecMyInfo[iPhase].m_ucLevel );
					}
				}
			}

			pButtonKick->SetColor ( D3DXCOLOR ( 1.f, 1.f, 1.f, 1.f ) );
			pButtonKick->SetShowEnable( true, true );

			pButtonRefresh->SetColor ( D3DXCOLOR ( 1.f, 1.f, 1.f, 1.f ) );
			pButtonRefresh->SetShowEnable( true, true );

			if ( pRelationInfo->m_eRelationStateType == SEnum::RT_COUPLE )
			{
				pButtonPropose->SetColor ( D3DXCOLOR ( 1.f, 1.f, 1.f, 1.f ) );
				pButtonPropose->SetShowEnable( true, true );
				pButtonTeleport->SetShowEnable( false, false );
			}
			else // RST_MARRIED_COUPLE
			{		
				pButtonTeleport->SetColor ( D3DXCOLOR ( 1.f, 1.f, 1.f, 1.f ) );
				pButtonTeleport->SetShowEnable( true, true );
				pButtonPropose->SetShowEnable( false, false );
			}
		}
	}
}
bool CX2Community::IsRelatioshipSkillID ( int eSkillID )
{
	if ( eSkillID == ( CX2SkillTree::SKILL_ID ) _CONST_RELATIONSHIP_UI_::g_iRelationSkillID )
		return true;
	return false;
}

bool CX2Community::Handler_EGS_RELATIONSHIP_INFO_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_RELATIONSHIP_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if(g_pMain->DeleteServerPacket( EGS_RELATIONSHIP_INFO_ACK ) )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();
			if( NULL != pRelationshipManager )
			{
				if ( NULL != pRelationshipManager->GetMyRelationshipInfo() )
				{
					pRelationshipManager->GetMyRelationshipInfo()->m_mapOtherUnitEquippedItem = kEvent.m_mapOtherUnitEquippedItem;
				}

				vector<CX2RelationshipManager::RelationshipCommunityInfo> & vecRelationCommunityInfo = pRelationshipManager->GetRelationshipCommunityInfo();
				vecRelationCommunityInfo.clear();

				CX2RelationshipManager::RelationshipCommunityInfo MyInfo;
				MyInfo.m_bIsConnect = true;
				MyInfo.m_cPosition =  kEvent.m_cMyPosition;
				MyInfo.m_cUnitClass =  kEvent.m_cMyUnitClass;
				MyInfo.m_iMapID =  kEvent.m_iMyMapID;
				MyInfo.m_tLastLogOutTime = 0;
				MyInfo.m_ucLevel = kEvent.m_ucMyLevel;
				MyInfo.m_wstrChannelName = kEvent.m_wstrMyChannelName;

				if ( NULL != g_pData->GetMyUser() &&
					NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{ 
					MyInfo.m_wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
				}
				vecRelationCommunityInfo.push_back( MyInfo );

				CX2RelationshipManager::RelationshipCommunityInfo PartnerInfo;
				PartnerInfo.m_bIsConnect = kEvent.m_bIsConnect;
				PartnerInfo.m_cPosition = kEvent.m_cPosition;
				PartnerInfo.m_cUnitClass = kEvent.m_cUnitClass;
				PartnerInfo.m_iMapID = kEvent.m_iMapID;
				PartnerInfo.m_tLastLogOutTime = kEvent.m_tLastLogOutTime;
				PartnerInfo.m_ucLevel = kEvent.m_ucLevel;
				PartnerInfo.m_wstrChannelName = kEvent.m_wstrChannelName;
				if ( NULL != pRelationshipManager->GetMyRelationshipInfo() )
					PartnerInfo.m_wstrNickName = pRelationshipManager->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname;

				vecRelationCommunityInfo.push_back( PartnerInfo );

				SetTab ( XMUT_RELATION );
				ResetRelationshipTabUI();

			}
		}
	}
	return true;
}

void CX2Community::ResetRelationDummyUnit ()
{
	SAFE_DELETE ( m_pRelationUnit );
	
	BOOST_FOREACH (CX2UnitViewerUI * pUnitViewerUI, m_vecUnitViewerUI )
	{
		SAFE_DELETE_KTDGOBJECT ( pUnitViewerUI );
	}
	m_vecUnitViewerUI.clear();
	
	m_pDlgRelation->SetShowEnable(false, false);
	m_pDlgRelationOnlyPartnerShowing->SetShowEnable(false, false);
}

void CX2Community::AllocateRelationDummyUnit ( wstring wstrTargetNickName, char cClassType )
{
	KUnitInfo kUnitInfo;
	kUnitInfo.Init();
	kUnitInfo.m_wstrNickName = wstrTargetNickName;
	kUnitInfo.m_cUnitClass = cClassType;
	m_pRelationUnit = new CX2Unit ( kUnitInfo );
}


void CX2Community::UpdateRelationshipList ()
{

}

void CX2Community::SetRelationshipMemberIcon( int i, bool bShow, int iUnitClass, int iUnitLv  )
{
	if( i < 0 || i >= _CONST_RELATIONSHIP_UI_::g_iMaxRelationMemberNumber )
		return;

	if( m_bHideWindow == true )
	{
		if(m_pFieldUserIcon[i] != NULL)
		{
			m_pFieldUserIcon[i]->SetShowEnable( false, false );
			return;
		}
	}

	if( bShow == true )
	{
		if(m_pFieldUserIcon[i] == NULL)
		{
			m_pFieldUserIcon[i] = new CKTDGUIDialog( m_pStage, L"DLG_UI_Character_Icon.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pFieldUserIcon[i] );			
		}
		m_pFieldUserIcon[i]->SetShowEnable( true, true );
	}
	else
	{
		if(m_pFieldUserIcon[i] != NULL)
		{
			m_pFieldUserIcon[i]->SetShowEnable( false, false );
		}
	}

	if( m_pFieldUserIcon[i] == NULL )
		return;

	CKTDGUIStatic* pStaticClass		= static_cast< CKTDGUIStatic* >( m_pFieldUserIcon[i]->GetControl( L"user" ) );	// 직업아이콘
	CKTDGUIStatic* pStaticLv10		= static_cast< CKTDGUIStatic* >( m_pFieldUserIcon[i]->GetControl( L"ten" ) );	// 레벨 10자리
	CKTDGUIStatic* pStaticLv1		= static_cast< CKTDGUIStatic* >( m_pFieldUserIcon[i]->GetControl( L"one" ) );	// 레벨 1자리

	int unitLv10, unitLv1;

	unitLv10	= iUnitLv / 10;
	unitLv1		= iUnitLv % 10;


	ClearUserIcon(i);

	if( NULL != pStaticClass && 
		NULL != pStaticClass->GetPicture(0) )
	{
		CKTDGUIControl::CPictureData* pPicture = pStaticClass->GetPicture(0);

		wstring fileName;
		wstring pieceName;
		if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS) iUnitClass, CX2Data::CIT_20by20 ) )
		{
			pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
		}
		else
		{
			ASSERT( !"NO" );
		}
	}

	pStaticLv10->GetPicture( unitLv10 )->SetShow(true);
	pStaticLv1->GetPicture( unitLv1 )->SetShow(true);

	if(m_pDlgRelation != NULL)
	{
		D3DXVECTOR3 tempPos1 = m_pDlgRelation->GetDummyPos(i);
		D3DXVECTOR3 tempPos2 = m_pDlgBack->GetPos();
		m_pFieldUserIcon[i]->SetPos_LUA(tempPos1.x + tempPos2.x, tempPos1.y + tempPos2.y);
	}	
}
void CX2Community::ClearUnitViewerUIInRelationshipTab ( void )
{
	BOOST_FOREACH ( CX2UnitViewerUI * pUnitViewerUI, m_vecUnitViewerUI )
	{
		SAFE_DELETE_KTDGOBJECT ( pUnitViewerUI );
	}
	m_vecUnitViewerUI.clear();
}





const WCHAR* CX2Community::GetRelationTabName( const SEnum::RELATIONSHIP_TYPE eType_ ) const
{
	switch ( eType_ )
	{
	case SEnum::RT_SOLO:
		{
			// 장착중인 인연 스킬 장착 해제
			if( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				g_pData->GetSkillTree()->UnequipRelationSkill();
			}
			return L"relationSolo";
		}
		break;

	case SEnum::RT_COUPLE:
		return L"relationCouple";
		break;

	case SEnum::RT_MARRIED:
		return L"relationMarried";
		break;

	default:
		ASSERT( !L"This RelationType is Invalid" );
		StateLog( "This RelationType is Invalid" );
		return L"relationSolo";
		break;
	}
}


void CX2Community::SetRelationTab( const bool bOpen_ )
{
	m_iPickedUserIndex = 0;
	m_iPickedUserUID = 0;
	CheckButton(NULL);

	if ( bOpen_ )
	{
		if ( NULL != g_pData->GetRelationshipManager() )
		{
			/// 결혼시스템의 텝은 3종류 (솔로, 커플, 결혼) 이기 때문에
			/// 커플 상태에 따라 다른 탭 이름을 얻어 오도록 한다.
			const SEnum::RELATIONSHIP_TYPE eType 
				= g_pData->GetRelationshipManager()->GetMyRelationInfoType();
			const WCHAR* pRadioButtonName = GetRelationTabName( eType );
			CKTDGUIRadioButton* pRadioButton 
				= static_cast<CKTDGUIRadioButton*>( m_pDlgBack->GetControl( pRadioButtonName ) );

			pRadioButton->SetChecked(true);	
			m_MessengerUserTab = XMUT_RELATION;
		}
	}
	else
	{
		CKTDGUIRadioButton* pRadioButton 
			= static_cast<CKTDGUIRadioButton*>( m_pDlgBack->GetControl(L"all") );
		
		pRadioButton->SetChecked(true);
		m_MessengerUserTab = XMUT_FIELD;
	}	
}


#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_RECRUIT_EVENT_BASE

void CX2Community::SetRecruitUI( bool bShow )
{
	if ( bShow == true )
	{
		if ( m_pDlgRecruit == NULL )
		{
			D3DXVECTOR2 vPos = D3DXVECTOR2(710.f, 140.f);

			m_pDlgRecruit = new CKTDGUIDialog( m_pStage, L"DLG_UI_Community_Recruit.lua", 0.07f );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgRecruit );
			m_pDlgRecruit->SetPos( vPos );
			m_pDlgRecruit->SetShowEnable( false, false );
			m_pDlgRecruit->SetDisableUnderWindow( true );

			CKTDGUIRadioButton *pRadioButton = static_cast< CKTDGUIRadioButton * >( m_pDlgBack->GetControl( L"Recruit" ) );
			pRadioButton->SetShowEnable( true, true );
		}
	}
	else
	{
		CKTDGUIRadioButton *pRadioButton = static_cast< CKTDGUIRadioButton * >( m_pDlgBack->GetControl( L"Recruit" ) );
		pRadioButton->SetShowEnable( false, false );

		SAFE_DELETE_DIALOG( m_pDlgRecruit );
	}
}

void CX2Community::UpdateRecruitList()
{
	if ( m_pDlgRecruit == NULL )
		return;

	const int MAGIC_MAX_REFEREE_PER_PAGE = 8;
	WCHAR wszText[64] = L"";

	std::vector< KRecommendUserInfo >* pvecRecommendUserInfo = &m_vecRecruitUnitInfo;	
	if( pvecRecommendUserInfo->empty() )
		pvecRecommendUserInfo = &m_vecRecruiterUnitInfo;

	m_vecUserLv.clear();
	m_vecUserUID.clear();
	for( UINT i=0; i < pvecRecommendUserInfo->size(); ++i )
	{
		m_vecUserUID.push_back( ( *pvecRecommendUserInfo )[i].m_iUnitUID );
		m_vecUserLv.push_back( ( *pvecRecommendUserInfo )[i].m_ucLevel );
	}

	for( UINT i = 0; i < MAGIC_MAX_REFEREE_PER_PAGE; ++i )
	{
		swprintf( wszText, sizeof(wszText) / (sizeof(WCHAR) - 1), L"User%d", i+1 );
		CKTDGUIButton* pButton = static_cast< CKTDGUIButton* >( m_pDlgRecruit->GetControl( wszText ) );

		if( NULL != pButton )
		{
			swprintf( wszText, sizeof(wszText) / (sizeof(WCHAR) - 1), L"User%d_lv", i+1 );
			CKTDGUIStatic* pStaticLv = static_cast< CKTDGUIStatic* >( m_pDlgRecruit->GetControl( wszText ) );

			swprintf( wszText, sizeof(wszText) / (sizeof(WCHAR) - 1), L"name%d", i+1 );
			CKTDGUIStatic* pStaticName = static_cast< CKTDGUIStatic* >( m_pDlgRecruit->GetControl( wszText ) );

			swprintf( wszText, sizeof(wszText) / (sizeof(WCHAR) - 1), L"state%d", i+1 );
			CKTDGUIStatic* pStaticState = static_cast< CKTDGUIStatic* >( m_pDlgRecruit->GetControl( wszText ) );

			if( i < pvecRecommendUserInfo->size() )
			{
				const KRecommendUserInfo& kRecommenduserInfo = ( *pvecRecommendUserInfo )[i];

				pButton->SetShowEnable( true, true );

				// 캐릭터 레벨을 이미지로 표시+클래스 표시하는 기능
				if(m_pFieldUserIcon[i] == NULL)
				{			
					m_pFieldUserIcon[i] = new CKTDGUIDialog( m_pStage, L"DLG_UI_Character_Icon.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pFieldUserIcon[i] );			
				}

				if ( g_pData == NULL ||
					g_pData->GetMessenger() == NULL ||
					g_pData->GetMessenger()->GetOpen() == false ||
					g_pData->GetMessenger()->GetOpenTab() != XMUT_RECRUIT ||
					true == m_bHideWindow )
					m_pFieldUserIcon[i]->SetShowEnable(false, false);
				else
					m_pFieldUserIcon[i]->SetShowEnable(true, true);

				CKTDGUIStatic* pStaticClass = (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"user" );	// 직업아이콘
				CKTDGUIStatic* pStaticLv10	= (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"ten" );	// 레벨 10자리
				CKTDGUIStatic* pStaticLv1	= (CKTDGUIStatic*) m_pFieldUserIcon[i]->GetControl( L"one" );	// 레벨 1자리

				int unitClass = 0;
				int unitLv10, unitLv1;

				unitClass = kRecommenduserInfo.m_cUnitClass;
				unitLv10 = kRecommenduserInfo.m_ucLevel / 10;
				unitLv1 = kRecommenduserInfo.m_ucLevel % 10;

				ClearUserIcon(i);

				if( NULL != pStaticClass && 
					NULL != pStaticClass->GetPicture(0) )
				{
					CKTDGUIControl::CPictureData* pPicture = pStaticClass->GetPicture(0);

					wstring fileName;
					wstring pieceName;
					if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS)unitClass, CX2Data::CIT_20by20 ) )
					{
						pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
					}
					else
					{
						pPicture->SetTex( L"DLG_Common_Texture01.TGA", L"white" );
						ASSERT( !"NO" );
					}
				}

				pStaticLv10->GetPicture(unitLv10)->SetShow(true);
				pStaticLv1->GetPicture(unitLv1)->SetShow(true);

				if(m_pDlgField != NULL)
				{
					D3DXVECTOR3 tempPos1 = m_pDlgField->GetDummyPos(i);
					D3DXVECTOR3 tempPos2 = m_pDlgBack->GetPos();
					m_pFieldUserIcon[i]->SetPos_LUA(tempPos1.x + tempPos2.x, tempPos1.y + tempPos2.y);
				}

				// lv 스트링 끄기
				pStaticLv->SetShowEnable( false, false );

				//pStaticLv->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kRecommenduserInfo.m_ucLevel ) ) );
				//pStaticLv->SetShowEnable( true, true );

				std::wstring wstrNickName = kRecommenduserInfo.m_wstrNickName;
				if( wstrNickName.find( L'_' ) != std::string::npos )
				{
					wstrNickName = L"#CFF0000" + wstrNickName.substr( 0, wstrNickName.find( L'_' ) ) + L"#CX";

				}

				pStaticName->SetString( 0,  wstrNickName.c_str() );
				pStaticName->SetShowEnable( true, true );

				if ( kRecommenduserInfo.m_bIsOnline == true )
				{
					pStaticState->SetString( 0, GET_STRING( STR_ID_98 ) );
				}
				else
				{
					pStaticState->SetString( 0, GET_STRING( STR_ID_99 ) );
				}
				pStaticState->SetShowEnable( true, true );
			}
			else
			{
				pButton->SetShowEnable( false, false );
				pStaticLv->SetShowEnable( false, false );
				pStaticName->SetShowEnable( false, false );
				pStaticState->SetShowEnable( false, false );
			}
		}
	}
}

bool CX2Community::Handler_EGS_GET_RECRUIT_RECRUITER_LIST_REQ()
{
	if ( m_pDlgRecruit->GetShow() == true )
		m_pDlgRecruit->SetShow( false );

	g_pData->GetServerProtocol()->SendID( EGS_GET_RECRUIT_RECRUITER_LIST_REQ );
	g_pMain->AddServerPacket( EGS_GET_RECRUIT_RECRUITER_LIST_ACK );

	return true;
}

bool CX2Community::Handler_EGS_GET_RECRUIT_RECRUITER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_RECRUIT_RECRUITER_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_RECRUIT_RECRUITER_LIST_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( m_pDlgRecruit != NULL )
			{
				m_pDlgRecruit->SetShow( true );

				m_vecRecruitUnitInfo = kEvent.m_vecRecruitUnitInfo;
				m_vecRecruiterUnitInfo = kEvent.m_vecRecruiterUnitInfo;

				UpdateRecruitList();

				CKTDGUIStatic* pStatic	= static_cast< CKTDGUIStatic* >( m_pDlgRecruit->GetControl( L"page_number" ) );

				if ( pStatic != NULL )
				{
					if ( pStatic->GetShow() == true )
						pStatic->SetShow( false );
				}
			}
			return true;
		}
	}

	return false;
}

bool CX2Community::Handler_EGS_RECRUIT_RECRUITER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RECRUIT_RECRUITER_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	std::vector< KRecommendUserInfo >* pvecRecommendUserInfo = &m_vecRecruitUnitInfo;	
	if( pvecRecommendUserInfo->empty() )
		pvecRecommendUserInfo = &m_vecRecruiterUnitInfo;
	BOOST_TEST_FOREACH( KRecommendUserInfo&, value, *pvecRecommendUserInfo )
	{
		if ( value.m_iUnitUID == kEvent.m_iSenderUnitUID )
		{
			switch ( kEvent.m_cType )
			{
			case KRecommendUserInfo::RUIT_LEVEL_UP:
				{
					value.m_ucLevel = static_cast< u_char >( kEvent.m_iData );
				}
				break;

			case KRecommendUserInfo::RUIT_LOGIN:
				{
					value.m_bIsOnline = true;
				}
				break;

			case KRecommendUserInfo::RUIT_LOGOUT:
				{
					value.m_bIsOnline = false;
				}
				break;

			default:
				{
					ASSERT( !L"Wrong Path!" );
					return false;
				}
				break;
			}

			break;
		}
	}

	UpdateRecruitList();

	return true;
}
#endif SERV_RECRUIT_EVENT_BASE