


#include "StdAfx.h"
#include ".\x2partymanager.h"


CX2PartyManager::CX2PartyManager(void) :
m_pDLGPartyQuickJoin(NULL)
#ifdef SERV_PVP_NEW_SYSTEM
, m_bProcessPvpMatch(false)
, m_iMatchUID(0)
, m_pDlgPvpPartyMsg(NULL)
#endif
, m_bChangeMap(false)
, m_bStartGame(false)
, m_bProcessDungeonMatch(false)
, m_iCurrentDungeonIDWithDifficulty(0)
{	
}

CX2PartyManager::~CX2PartyManager(void)
{
}


/*virtual*/ bool CX2PartyManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_REGROUP_PARTY_NOT:
		{
			return Handler_EGS_REGROUP_PARTY_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_JOIN_PARTY_NOT:
		{
			Handler_EGS_JOIN_PARTY_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_LEAVE_PARTY_ACK:
		{
			Handler_EGS_LEAVE_PARTY_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_LEAVE_PARTY_NOT:
		{
			Handler_EGS_LEAVE_PARTY_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;




	case EGS_PARTY_CHANGE_DUNGEON_ACK:
		{
			Handler_EGS_PARTY_CHANGE_DUNGEON_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_PARTY_CHANGE_DUNGEON_NOT:
		{
			Handler_EGS_PARTY_CHANGE_DUNGEON_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_UPDATE_PARTY_USER_INFO_NOT:
		{
			Handler_EGS_UPDATE_PARTY_USER_INFO_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_PARTY_GAME_START_ACK:
		{
			Handler_EGS_PARTY_GAME_START_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_QUICK_START_DUNGEON_GAME_ACK:
		{
			Handler_EGS_QUICK_START_DUNGEON_GAME_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
		
	case EGS_PARTY_GAME_START_NOT:
		{
			Handler_EGS_PARTY_GAME_START_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-07

	case EGS_DEFENCE_DUNGEON_GAME_START_NOT:
		{
			Handler_EGS_DEFENCE_DUNGEON_GAME_START_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

#endif // SERV_NEW_DEFENCE_DUNGEON

	case EGS_INVITE_PARTY_ACK:
		{
			Handler_EGS_INVITE_PARTY_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_INVITE_PARTY_NOT:
		{
			Handler_EGS_INVITE_PARTY_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_INVITE_PARTY_DENY_NOT:
		{
			Handler_EGS_INVITE_PARTY_DENY_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	case EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK:
		{
			Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;


	case EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT:
		{
			Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
#endif SERV_DUNGEON_DIFFICULTY_REVISION


	case EGS_PARTY_CHANGE_HOST_ACK:
		{
			Handler_EGS_PARTY_CHANGE_HOST_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;


	case EGS_PARTY_CHANGE_HOST_NOT:
		{
			Handler_EGS_PARTY_CHANGE_HOST_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;


	case EGS_PARTY_BAN_USER_ACK:
		{
			Handler_EGS_PARTY_BAN_USER_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;


	case EGS_PARTY_BAN_USER_NOT:
		{
			Handler_EGS_PARTY_BAN_USER_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;


	case EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK:
		{
			Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT:
		{
			Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT( hWnd, uMsg, wParam, lParam );
			return true;

		} break;

	case EGS_PARTY_CHANGE_NUM_OF_PER_ACK:
		{
			Handler_EGS_PARTY_CHANGE_NUM_OF_PER_ACK( hWnd, uMsg, wParam, lParam );
			return true;

		} break;

	case EGS_PARTY_CHANGE_NUM_OF_PER_NOT:
		{
			Handler_EGS_PARTY_CHANGE_NUM_OF_PER_NOT( hWnd, uMsg, wParam, lParam );
			return true;

		} break;

	case EGS_PARTY_FEVER_POINT_NOT:
		{
			Handler_EGS_PARTY_FEVER_POINT_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;


	case EGS_CHANGE_PARTY_FEVER_STATE_NOT:
		{
			Handler_EGS_CHANGE_PARTY_FEVER_STATE_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
#ifdef SERV_PVP_NEW_SYSTEM
	case EGS_CHANGE_PARTY_TYPE_ACK:
		{
			Handler_EGS_CHANGE_PARTY_TYPE_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_CHANGE_PARTY_TYPE_NOT:
		{
			Handler_EGS_CHANGE_PARTY_TYPE_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_REQUEST_MATCH_MAKING_ACK:
		{
			Handler_EGS_REQUEST_MATCH_MAKING_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT:
		{
			Handler_EGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_CANCEL_MATCH_MAKING_ACK:
		{
			Handler_EGS_CANCEL_MATCH_MAKING_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_UNREG_MATCH_WAIT_LIST_NOT:
		{
			Handler_EGS_UNREG_MATCH_WAIT_LIST_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_MATCH_MAKING_SUCCESS_NOT:
		{
			Handler_EGS_MATCH_MAKING_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_GAME_START_PVP_MATCH_NOT:
		{
			Handler_EGS_GAME_START_PVP_MATCH_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		}
	case EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK:
		{
			Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT:
		{
			Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
	case EGS_MATCH_CLOSE_NOT:
		{			
			Handler_EGS_MATCH_CLOSE_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_CLOSE_PARTY_NOT:
		{
			Handler_EGS_CLOSE_PARTY_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;

	case EGS_PARTY_GAME_START_OPINION_CHECK_NOT:
		{
			return Handler_EGS_PARTY_GAME_START_OPINION_CHECK_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_PARTY_GAME_START_FAILED_NOT:
		{
			return Handler_EGS_PARTY_GAME_START_FAILED_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_AUTO_PARTY_DUNGEON_GAME_ACK:	// 던전 자동파티 등록 응답
		{
			return Handler_EGS_AUTO_PARTY_DUNGEON_GAME_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT: // 자동파티 등록 성공시 오는 패킷
		{
			return Handler_EGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CANCEL_AUTO_PARTY_MAKING_ACK:	// 던전 자동 파티 취소 응답 핸들러
		{
			return Handler_EGS_CANCEL_AUTO_PARTY_MAKING_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT:	// 자동 파티 취소 성공시 오는 패킷
		{
			return Handler_EGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_AUTO_PARTY_MAKING_SUCCESS_NOT:		// 자동 파티 매칭 시 오는 패킷
		{
			return Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_AUTO_PARTY_CLOSE_NOT:	// 자동 파티 매칭이 닫힐때 오는 패킷
		{
			return Handler_EGS_AUTO_PARTY_CLOSE_NOT( hWnd, uMsg, wParam, lParam	);
		} break;
#endif

	case EGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT:
		{
			return Handler_EGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	case EGS_AUTO_PARTY_WAIT_USER_COUNT_NOT: 
		{
			return Handler_EGS_AUTO_PARTY_WAIT_USER_COUNT_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif //SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW

#ifdef SERV_NEW_EVENT_TYPES
	case EGS_UPDATE_DUNGEON_STATUS_NOT:
		{
			Handler_EGS_UPDATE_DUNGEON_STATUS_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		}
#endif //SERV_NEW_EVENT_TYPES

	default:
		return false;
	}

	return false;
}

bool CX2PartyManager::Handler_EGS_CLOSE_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_CLOSE_PARTY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//{{ kimhc // 2010.4.5 // 비밀던전 작업(헬던전)
#ifdef  SERV_SECRET_HELL
	// 파티 탈퇴 성공
	GetMyPartyData()->ClearPartyMemberData();
	GetMyPartyData()->m_iPartyUID = -1;

	// 파티탈퇴 될때 던전외에 파티설정 옵션을 default로 되돌린다.
	GetMyPartyData()->m_iDungeonDifficulty = (int) CX2Dungeon::DL_NORMAL;
	GetMyPartyData()->m_iPartyMemberCount = 0;
	GetMyPartyData()->m_iMaxPartyMemberCount = 4;
	GetMyPartyData()->m_eItemLootingMode = CX2Room::DGIT_RANDOM;
	GetMyPartyData()->m_bPublic = true;
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	GetMyPartyData()->m_bIsLimitLevel = true;
#endif SERV_DUNGEON_DIFFICULTY_REVISION

#ifdef COME_BACK_REWARD
	GetMyPartyData()->m_bComeBackPower = false;
#endif
	CX2GageManager::GetInstance()->ClearPartyMemberUI();

#ifdef NEW_MAIL_LOG
	AddClosePartyLog( "EGS_CLOSE_PARTY_NOT", kEvent.m_iReason );
#endif // NEW_MAIL_LOG

	Handler_EGS_CANCEL_MATCH_MAKING_REQ();
	Handler_EGS_CANCEL_AUTO_PARTY_MAKING_REQ();
#ifdef HENIR_TEST
	const CX2Dungeon::DungeonData* pDungeonData = 
		g_pData->GetDungeonManager()->GetDungeonData( static_cast<SEnum::DUNGEON_ID>( GetMyPartyData()->m_iDungeonID ) );

	if ( pDungeonData != NULL )
	{
		switch ( pDungeonData->m_eDungeonType )
		{
		case CX2Dungeon::DT_HENIR:
			GetMyPartyData()->m_iDungeonMode = CX2Dungeon::DM_HENIR_PRACTICE;
			break;

		case CX2Dungeon::DT_SECRET:
			GetMyPartyData()->m_iDungeonMode = CX2Dungeon::DM_SECRET_NORMAL;
			break;

		default:
			GetMyPartyData()->m_iDungeonMode = CX2Dungeon::DM_INVALID;
			break;
		}
	}
#endif HENIR_TEST

#else	SERV_SECRET_HELL
	// 파티 탈퇴 성공
	g_pData->GetPartyManager()->GetMyPartyData()->ClearPartyMemberData();
	g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyUID = -1;

	// 파티탈퇴 될때 던전외에 파티설정 옵션을 default로 되돌린다.
	g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty = (int) CX2Dungeon::DL_NORMAL;
	g_pData->GetPartyManager()->GetMyPartyData()->m_iMaxPartyMemberCount = 4;
	g_pData->GetPartyManager()->GetMyPartyData()->m_eItemLootingMode = CX2Room::DGIT_RANDOM;
	g_pData->GetPartyManager()->GetMyPartyData()->m_bPublic = true;
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	g_pData->GetPartyManager()->GetMyPartyData()->m_bIsLimitLevel = true;
#endif SERV_DUNGEON_DIFFICULTY_REVISION

#ifdef HENIR_TEST
	g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode = (int) CX2Dungeon::DM_HENIR_PRACTICE;
#endif HENIR_TEST

#endif  SERV_SECRET_HELL
	//}} kimhc // 2010.4.5 // 비밀던전 작업(헬던전)

	if( NULL != g_pMain->GetPartyUI() )
	{
		g_pMain->GetPartyUI()->UpdatePartyDLG();
//		g_pMain->GetPartyUI()->OpenPartyMemberDLG( false );
#ifdef HENIR_TEST
		g_pMain->GetPartyUI()->OpenPartyFeverDLG( false );
#endif HENIR_TEST
		g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START);
		g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START);
	}

	//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	if ( true == g_pData->GetPremiumBuffManager()->IsTooniLandChannelingEvent() )
	{
		if ( NULL != GetMyPartyData() )
			GetMyPartyData()->m_bHaveTooniLandEventTitle = false;

		g_pData->GetPremiumBuffManager()->SetTooniLandChannelingEvent( false );
	}
#endif	SERV_TOONILAND_CHANNELING_EVENT
	//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

	return true;
}

bool CX2PartyManager::Handler_EGS_JOIN_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_PARTY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		if( true == g_pData->GetPartyManager()->DoIHaveParty() )
		{
			// 내 파티에 다른 사람이 참가한 경우
			for( UINT i=0; i<kEvent.m_vecPartyUserInfo.size(); i++ )
			{
				const KPartyUserInfo& kPartyUserInfo = kEvent.m_vecPartyUserInfo[i];
				GetMyPartyData()->AddPartyMemberData( kPartyUserInfo );

				CX2GageManager* pGageManager = CX2GageManager::GetInstance();
				if ( pGageManager->CanThisMemberInsert( kPartyUserInfo.m_iUnitUID ) )
				{
					CX2GameUnit* pGameUnitPartyMember 
						= ( NULL != g_pX2Game ? g_pX2Game->GetUserUnitByUID( kPartyUserInfo.m_iUnitUID ) : NULL );

					pGageManager->InsertPartyMemberUI( kPartyUserInfo, pGameUnitPartyMember );
					pGageManager->UpdatePartyMemberGageData( kPartyUserInfo.m_iUnitUID, kPartyUserInfo.m_kGamePlayStatus );
				}
				
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				//{{ kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
				userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
				userData.iOpponentUID	= kPartyUserInfo.m_iUnitUID;
				//}} kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
				g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_LEADER_INVITE_PARTY, userData );

				if(g_pTFieldGame != NULL)
				{
					// 파티 참여 알림 말풍선
					CX2TalkBoxManagerImp::TalkBox talkBox;
					talkBox.m_vPos				= D3DXVECTOR2( 936,702 );
					talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
					talkBox.m_fRemainTime		= 2.0f;

					talkBox.m_wstrTalkContent	= GET_REPLACED_STRING( ( STR_ID_375, "iL", ( int )kPartyUserInfo.m_ucLevel, kPartyUserInfo.m_wstrNickName ) );
					talkBox.m_bTraceUnit		= false;
					talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_RIGHT;
					talkBox.m_coTextColor		= D3DXCOLOR(0,0,0,1);

					g_pTFieldGame->GetTalkBoxManager()->Push( talkBox );
				}

				g_pKTDXApp->GetDeviceManager()->PlaySound( L"PartyJoin.ogg", false, false ); 

				//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
				if ( false == GetMyPartyData()->m_bHaveTooniLandEventTitle 
					&& TOONILAND_CHANNELING_EVENT_TITLE == kPartyUserInfo.m_iEquippedTitleID )
				{
					GetMyPartyData()->m_bHaveTooniLandEventTitle = true;

					if ( NULL != g_pTFieldGame )
						g_pTFieldGame->OpenBuffName( false, 124 );	// 나중에 enum으로 바꿔야 할 듯

					if ( NULL != g_pData->GetPremiumBuffManager() )
						g_pData->GetPremiumBuffManager()->SetTooniLandChannelingEvent( true );
				}
#endif	SERV_TOONILAND_CHANNELING_EVENT
				//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
						
			}
		}
		else
		{
			// 어떤 파티에 내가 들어간 경우, 또는 파티가 처음 만들어 졌을 때
			ASSERT( false == kEvent.m_vecPartyInfo.empty() );

			//{{ kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드
// #ifdef	SERV_CHOOSE_FASTEST_HOST
// 			g_pData->GetGameUDP()->SetMyUIDAndRelayIpAndPort( g_pData->GetMyUser()->GetSelectUnit()->GetUID(),
// 				kEvent.m_wstrUDPRelayIP.c_str(),
// 				static_cast<int>( kEvent.m_usUDPRelayPort ) );
// #endif	SERV_CHOOSE_FASTEST_HOST
			//}} kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드

			if( false == kEvent.m_vecPartyInfo.empty() )
			{
				GetMyPartyData()->Init();
				GetMyPartyData()->Set_KPartyInfo( kEvent.m_vecPartyInfo[0] );

				if ( NULL != g_pData->GetMyUser() &&
					NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					const int iLastClearDungeonID = g_pData->GetMyUser()->GetSelectUnit()->GetLastClearDungeonID();
					GetMyPartyData()->m_iDungeonID = (iLastClearDungeonID > 0 ? iLastClearDungeonID : SEnum::DI_RUBEN_EL_TREE_NORMAL);
				}
				else
					GetMyPartyData()->m_iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;
			}

			for( UINT i=0; i< kEvent.m_vecPartyUserInfo.size(); i++ )
			{
				const KPartyUserInfo& kPartyUserInfo = kEvent.m_vecPartyUserInfo[i];
				GetMyPartyData()->AddPartyMemberData( kPartyUserInfo );

				CX2GageManager* pGageManager = CX2GageManager::GetInstance();
				if ( pGageManager->CanThisMemberInsert( kPartyUserInfo.m_iUnitUID ) )
				{
					CX2GameUnit* pGameUnitPartyMember 
						= ( NULL != g_pX2Game ? g_pX2Game->GetUserUnitByUID( kPartyUserInfo.m_iUnitUID ) : NULL );

					pGageManager->InsertPartyMemberUI( kPartyUserInfo, pGameUnitPartyMember );
					pGageManager->UpdatePartyMemberGageData( kPartyUserInfo.m_iUnitUID, kPartyUserInfo.m_kGamePlayStatus );
				}

				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				//{{ kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
				userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
				userData.iOpponentUID	= kPartyUserInfo.m_iUnitUID;
				//}} kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
				g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_LEADER_INVITE_PARTY, userData );

				//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
				if ( false == GetMyPartyData()->m_bHaveTooniLandEventTitle 
					 && TOONILAND_CHANNELING_EVENT_TITLE == kPartyUserInfo.m_iEquippedTitleID )
				{
					GetMyPartyData()->m_bHaveTooniLandEventTitle = true;

					if ( NULL != g_pTFieldGame )
						g_pTFieldGame->OpenBuffName( false, 124 );	// 나중에 enum으로 바꿔야 할 듯
					
					if ( NULL != g_pData->GetPremiumBuffManager() )
						g_pData->GetPremiumBuffManager()->SetTooniLandChannelingEvent( true );
				}
#endif	SERV_TOONILAND_CHANNELING_EVENT
				//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
				
			}

			GetMyPartyData()->UpdatePartyLeaderUID();

			g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START);
			g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START);
#ifdef SIMPLE_BUG_FIX
			g_pMain->GetPartyUI()->OpenPartyDLG(false);
#endif//SIMPLE_BUG_FIX
		}
		
		if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() >= 2 )
		{
			g_pMain->GetPartyUI()->OpenPartyFeverDLG( true );
			g_pMain->GetPartyUI()->UpdatePartyFeverDLG();
		}

		if (false == GetProcessPvpMatch() && false == GetProcessDungeonMatch())
		{
			g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START);
			g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START);
		}
	}
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetMyPartyData()->m_bPvpParty == true )
		g_pMain->GetPartyUI()->SetPvpMatchPlayerNum( GetMyPartyData()->GetPartyMemberCount() );
#endif

	return true;
}

bool CX2PartyManager::Handler_EGS_LEAVE_PARTY_REQ( int iReason )
{
	KEGS_LEAVE_PARTY_REQ kPacket;
	kPacket.m_iReason = iReason;

	g_pData->GetServerProtocol()->SendPacket( EGS_LEAVE_PARTY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_LEAVE_PARTY_ACK ); 

	return true;
}

bool CX2PartyManager::Handler_EGS_LEAVE_PARTY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_PARTY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_LEAVE_PARTY_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			GetMyPartyData()->Init();
			GetMyPartyData()->ClearPartyMemberData();
			
			CX2GageManager::GetInstance()->ClearPartyMemberUI();

			if ( NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				const int iLastClearDungeonID = g_pData->GetMyUser()->GetSelectUnit()->GetLastClearDungeonID();
				GetMyPartyData()->m_iDungeonID = (iLastClearDungeonID > 0 ? iLastClearDungeonID : SEnum::DI_RUBEN_EL_TREE_NORMAL);
			}
			else
				GetMyPartyData()->m_iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;
				
			GetMyPartyData()->m_iPartyMemberCount = 0;
			// 파티 탈퇴 성공

			if (false == GetProcessPvpMatch() && false == GetProcessDungeonMatch())
			{
				g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START);
				g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START);
			}

			if( NULL != g_pMain->GetPartyUI() )
			{
				g_pMain->GetPartyUI()->UpdatePartyDLG();
//				g_pMain->GetPartyUI()->OpenPartyMemberDLG( false );

#ifdef HENIR_TEST
				g_pMain->GetPartyUI()->OpenPartyFeverDLG( false );
#endif HENIR_TEST

			}

			//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
			if ( true == g_pData->GetPremiumBuffManager()->IsTooniLandChannelingEvent() )
			{
				if ( NULL != GetMyPartyData() )
					GetMyPartyData()->m_bHaveTooniLandEventTitle = false;

				g_pData->GetPremiumBuffManager()->SetTooniLandChannelingEvent( false );
			}
#endif	SERV_TOONILAND_CHANNELING_EVENT
			//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

		}
	}

	return true;
}

bool CX2PartyManager::Handler_EGS_LEAVE_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_PARTY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	// 파티 탈퇴 알림 말풍선
	PartyMemberData *pPartyMember = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData(kEvent.m_iLeaveUnitUID);
	if( NULL != pPartyMember )
	{
        wstring wstrJoinPartyMember = GET_REPLACED_STRING( ( STR_ID_376, "iL", pPartyMember->m_iUnitLevel, pPartyMember->m_wstrNickName ) );
		if( NULL != g_pData->GetMessenger() )
		{
			g_pData->GetMessenger()->AlarmReceiveMessage( wstrJoinPartyMember.c_str() );
		}
	}	

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"PartyLeave.ogg", false, false );  	

	g_pData->GetPartyManager()->GetMyPartyData()->RemovePartyMemberData( kEvent.m_iLeaveUnitUID );
	CX2GageManager::GetInstance()->RemovePartyMemberUIByUserUid( kEvent.m_iLeaveUnitUID );

#ifdef NEW_MAIL_LOG
	#ifdef LOG_PARTY_BREAK
		AddClosePartyLog( "EGS_LEAVE_PARTY_NOT", kEvent.m_iReason );
	#else
		AddClosePartyLog( "EGS_LEAVE_PARTY_NOT", 0 );
	#endif // LOG_PARTY_BREAK
#endif // NEW_MAIL_LOG

	CX2PartyManager::PartyMemberData* pNewPartyLeader = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( kEvent.m_iNewHostUID );
	if( NULL != pNewPartyLeader )
	{
		pNewPartyLeader->m_bPartyLeader = true;
		g_pData->GetPartyManager()->GetMyPartyData()->UpdatePartyLeaderUID();

		g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START);
		g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START);
	}

	if( NULL != g_pMain->GetPartyUI() )
	{
		g_pMain->GetPartyUI()->UpdatePartyDLG();
		g_pMain->GetPartyUI()->UpdatePartyFeverDLG();

	}
	if( true == AmIPartyLeader() )
	{
		Handler_EGS_CANCEL_MATCH_MAKING_REQ();
		Handler_EGS_CANCEL_AUTO_PARTY_MAKING_REQ();
	}

#ifdef COME_BACK_REWARD
	if ( NULL != GetMyPartyData() )
	{
		bool bComeBackUserInParty = false;

		BOOST_TEST_FOREACH( const CX2PartyManager::PartyMemberData&, value, GetMyPartyData()->m_vecPartyMemberData )
		{
			if ( true == value.m_bComeBackUser )
			{
				bComeBackUserInParty = true;
				break;
			}
		}

		if ( NULL != g_pData->GetPremiumBuffManager() )
		{
			g_pData->GetPremiumBuffManager()->SetComeBackUserInParty( bComeBackUserInParty );
		}
	}
#endif

	//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	if ( NULL != GetMyPartyData() )
	{
		bool bPartyHaveTooniLandEventTitle = false;
		BOOST_TEST_FOREACH( const CX2PartyManager::PartyMemberData&, value, GetMyPartyData()->m_vecPartyMemberData )
		{
			if ( value.m_iEquippedTitleID == TOONILAND_CHANNELING_EVENT_TITLE )
			{
				bPartyHaveTooniLandEventTitle = true;
				break;
			}
		}

		// 변경 되었으면
		if ( GetMyPartyData()->m_bHaveTooniLandEventTitle != bPartyHaveTooniLandEventTitle )
		{
			if ( true == bPartyHaveTooniLandEventTitle && NULL != g_pTFieldGame )
				g_pTFieldGame->OpenBuffName( false, 124 );

			GetMyPartyData()->m_bHaveTooniLandEventTitle = bPartyHaveTooniLandEventTitle;
			g_pData->GetPremiumBuffManager()->SetTooniLandChannelingEvent( bPartyHaveTooniLandEventTitle );
		}			
	}
#endif	SERV_TOONILAND_CHANNELING_EVENT
	//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

	return true;
}



bool CX2PartyManager::Handler_EGS_INVITE_PARTY_REQ( UidType inviteUID , const wstring& wstrInviteName_, int iServerGroupID_/* = -1*/ )
{
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetMyPartyData()->m_bPvpParty == true && GetMyPartyData()->GetPartyMemberCount() > 3 )
	{
		// 대전파티는 최대 파티원수는 3인임.
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_13568 ), g_pMain->GetNowState() );
		return false;
	}
	if( GetMyPartyData()->m_bPvpParty == true && GetProcessPvpMatch() == true )
	{
		// 매칭중에는 파티초대 안됨
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_13569 ), g_pMain->GetNowState() );
		return false;
	}
#endif

	if( true == static_cast<CX2State*>(g_pMain->GetNowState())->GetWaitGameStart() )//게임 시작 전 카운트 상태일 때
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_13569 ), g_pMain->GetNowState() );
		return false;
	}


	if( GetProcessDungeonMatch() == true )
	{
		// 매칭중에는 파티초대 안됨
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_13569 ), g_pMain->GetNowState() );
		return false;
	}

	if ( wstrInviteName_.length() == 0 )	// 해외팀 버그 수정
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_29349 ), g_pMain->GetNowState() );
		return false;
	}

	KEGS_INVITE_PARTY_REQ kPacket;
	kPacket.m_iReceiverUID = inviteUID;
	kPacket.m_iServerGroupID = iServerGroupID_;
	kPacket.m_wstrNickName = wstrInviteName_;
	kPacket.m_kCreatePartyInfo.m_bCheckLowLevel = GetMyPartyData()->m_bIsLimitLevel;
	kPacket.m_kCreatePartyInfo.m_cDungeonMode = static_cast<char>( GetMyPartyData()->m_iDungeonMode );
	kPacket.m_kCreatePartyInfo.m_cGetItemType = static_cast<char>( GetMyPartyData()->m_eItemLootingMode );
	kPacket.m_kCreatePartyInfo.m_cPartyType = ( true == GetMyPartyData()->m_bPvpParty ? KPartyInfo::PT_PVP_PARTY : KPartyInfo::PT_DUNGEON_PARTY );
	kPacket.m_kCreatePartyInfo.m_cPVPGameType = GetMyPartyData()->m_pvpMode;
	kPacket.m_kCreatePartyInfo.m_DifficultyLevel = GetMyPartyData()->m_iDungeonDifficulty;
	kPacket.m_kCreatePartyInfo.m_iDungeonID = GetMyPartyData()->m_iDungeonID;


#ifdef EXTEND_SERVER_GROUP_MASK
#else EXTEND_SERVER_GROUP_MASK
	switch(kPacket.m_iServerGroupID)
	{
	case SGI_INVALID:
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_43, "L", kPacket.m_wstrNickName ) ), 
				KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		} break;
	case SGI_SOLES:
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_20323, "L", kPacket.m_wstrNickName ) ), 
				KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		} break;
	case SGI_GAIA:
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_20324, "L", kPacket.m_wstrNickName ) ), 
				KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		} break;

	}
#endif EXTEND_SERVER_GROUP_MASK

	g_pData->GetServerProtocol()->SendPacket( EGS_INVITE_PARTY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_INVITE_PARTY_ACK ); 
	
	return true;
}

bool CX2PartyManager::Handler_EGS_INVITE_PARTY_REQ( const wstring& wstrInviteName_, int iServerGroupID_ /* = -1 */)
{
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetMyPartyData()->m_bPvpParty == true && GetMyPartyData()->GetPartyMemberCount() > 3 )
	{
		// 대전파티는 최대 파티원수는 3인임.
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_13568 ), g_pMain->GetNowState() );
		return false;
	}
	if( GetMyPartyData()->m_bPvpParty == true && GetProcessPvpMatch() == true )
	{
		// 매칭중에는 파티초대 안됨
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_13569 ), g_pMain->GetNowState() );
		return false;
	}
#endif
	if( true == static_cast<CX2State*>(g_pMain->GetNowState())->GetWaitGameStart() )//게임 시작 전 카운트 상태일 때
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_13569 ), g_pMain->GetNowState() );
		return false;
	}

	if( GetProcessDungeonMatch() == true )
	{
		// 매칭중에는 파티초대 안됨
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_13569 ), g_pMain->GetNowState() );
		return false;
	}

	if ( wstrInviteName_.length() == 0 )	// 해외팀 버그 수정
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_29349 ), g_pMain->GetNowState() );
		return false;
	}

	KEGS_INVITE_PARTY_REQ kPacket;
	kPacket.m_wstrNickName = wstrInviteName_;
	kPacket.m_iServerGroupID = iServerGroupID_;

	kPacket.m_kCreatePartyInfo.m_bCheckLowLevel = GetMyPartyData()->m_bIsLimitLevel;
	kPacket.m_kCreatePartyInfo.m_cDungeonMode = static_cast<char>( GetMyPartyData()->m_iDungeonMode );
	kPacket.m_kCreatePartyInfo.m_cGetItemType = static_cast<char>( GetMyPartyData()->m_eItemLootingMode );
	kPacket.m_kCreatePartyInfo.m_cPartyType = ( true == GetMyPartyData()->m_bPvpParty ? KPartyInfo::PT_PVP_PARTY : KPartyInfo::PT_DUNGEON_PARTY );
	kPacket.m_kCreatePartyInfo.m_cPVPGameType = GetMyPartyData()->m_pvpMode;
	kPacket.m_kCreatePartyInfo.m_DifficultyLevel = GetMyPartyData()->m_iDungeonDifficulty;
	kPacket.m_kCreatePartyInfo.m_iDungeonID = GetMyPartyData()->m_iDungeonID;
	
#ifdef EXTEND_SERVER_GROUP_MASK
#else EXTEND_SERVER_GROUP_MASK
	switch(kPacket.m_iServerGroupID)
	{
	case SGI_INVALID:
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_43, "L", kPacket.m_wstrNickName ) ), 
				KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		} break;
	case SGI_SOLES:
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_20323, "L", kPacket.m_wstrNickName ) ), 
				KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		} break;
	case SGI_GAIA:
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_20324, "L", kPacket.m_wstrNickName ) ), 
				KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		} break;

	}
#endif EXTEND_SERVER_GROUP_MASK

	g_pData->GetServerProtocol()->SendPacket( EGS_INVITE_PARTY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_INVITE_PARTY_ACK ); 

	return true;
}

bool CX2PartyManager::Handler_EGS_INVITE_PARTY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_INVITE_PARTY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_INVITE_PARTY_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//{{ kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드
// #ifdef	SERV_CHOOSE_FASTEST_HOST
// 			g_pData->GetGameUDP()->SetMyUIDAndRelayIpAndPort( g_pData->GetMyUser()->GetSelectUnit()->GetUID(),
// 				kEvent.m_wstrUDPRelayIP.c_str(),
// 				static_cast<int>( kEvent.m_usUDPRelayPort ) );
// #endif	SERV_CHOOSE_FASTEST_HOST
			//}} kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드
		}
		else
		{
			if( NULL != g_pMain->GetPartyUI() )
			{
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				//{{ kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
				userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
				userData.iOpponentUID	= kEvent.m_iReceiverUID;
				//}} kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
				g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_LEADER_INVITE_PARTY, userData );
			}
		}
	}

	return true;
}

bool CX2PartyManager::Handler_EGS_INVITE_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INVITE_PARTY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	 //{{ 김상훈 : 2010. 10. 04  파티 초대 관련 UI 개선
	 //다른 용무 중인 경우( ex : 게시판 사용 중,  NPC 와 대화 중 등 .. )일 경우 나에게 파티 초대가 올 경우 무시하고
	 //상대방에게 거절 메시지를 보낸다.


#ifdef SERV_INVITE_PARTY_REPLY_REASON 
	if ( g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIPersonalShopBoard() != NULL &&
		g_pData->GetUIManager()->GetUIPersonalShopBoard()->GetShow() == true )  // 거래 게시판이 떠 있는 경우
	{
		Send_EGS_INVITE_PARTY_REPLY_NOT( KEGS_INVITE_PARTY_REPLY_NOT::RR_BUSY , kEvent.m_iPartyUID );		
		return true;
	}
#endif SERV_INVITE_PARTY_REPLY_REASON 
    //}} 김상훈 : 2010. 10. 04  파티 초대 관련 UI 개선
	
	if( true == static_cast<CX2State*>(g_pMain->GetNowState())->GetWaitGameStart() )//게임 시작 전 카운트 상태일 때
	{
		Send_EGS_INVITE_PARTY_REPLY_NOT( KEGS_INVITE_PARTY_REPLY_NOT::RR_BUSY , kEvent.m_iPartyUID );		
		return true;
	}
		

	wstring wstrText = GET_STRING( STR_ID_377 );

	if(g_pData != NULL && g_pData->GetDungeonManager() != NULL)
	{
#ifdef SERV_PVP_NEW_SYSTEM
		if( kEvent.m_cPartyType == KPartyInfo::PT_DUNGEON_PARTY )
		{
			wstrText = GET_REPLACED_STRING( ( STR_ID_378, "LL", kEvent.m_wstrHostNickName, kEvent.m_wstrHostNickName ) );	
		}
		else if( kEvent.m_cPartyType == KPartyInfo::PT_PVP_PARTY )
		{
			wstrText = GET_REPLACED_STRING( ( STR_ID_12784, "L", kEvent.m_wstrHostNickName ) );	
		}
#else
		wstring wstrDungeonName = g_pData->GetDungeonManager()->MakeDungeonNameString( ( SEnum::DUNGEON_ID )kEvent.m_iDungeonID, 
			(CX2Dungeon::DIFFICULTY_LEVEL) kEvent.m_cDifficultyLevel, 
			(CX2Dungeon::DUNGEON_MODE) kEvent.m_cDungeonMode );

        wstrText = GET_REPLACED_STRING( ( STR_ID_378, "LL", wstrDungeonName, kEvent.m_wstrNickName ) );	
#endif
	}	

	//{{ kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드
// #ifdef	SERV_CHOOSE_FASTEST_HOST
// 	g_pData->GetGameUDP()->SetMyUIDAndRelayIpAndPort( g_pData->GetMyUser()->GetSelectUnit()->GetUID(),
// 		kEvent.m_wstrUDPRelayIP.c_str(),
// 		static_cast<int>( kEvent.m_usUDPRelayPort ) );
// #endif	SERV_CHOOSE_FASTEST_HOST
	//}} kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	userData.iOpponentUID = kEvent.m_iPartyUID;

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"PartyInvited.ogg", false, false );  

	g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_DECIDE_INVITE_REQUEST,
		CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 7.f, wstrText.c_str(), 
		(CKTDXStage*) g_pMain->GetNowState(), 
		CX2PartyUI::PUCM_ACCEPT_INVITE_REQUEST, CX2PartyUI::PUCM_REJECT_INVITE_REQUEST, CX2PartyUI::PUCM_REJECT_INVITE_REQUEST );
	return true;
}

//{{ 2010. 03. 10  최육사	채널 통합 파티 - PartyUID는 UidType임.



//{{ 김상훈 : 2010. 10. 04 파티 초대 관련 UI 개선
#ifdef SERV_INVITE_PARTY_REPLY_REASON 
bool CX2PartyManager::Send_EGS_INVITE_PARTY_REPLY_NOT( char Reason, UidType partyUID )
#else
bool CX2PartyManager::Send_EGS_INVITE_PARTY_REPLY_NOT( bool bAccept, UidType partyUID )
#endif
//}}
{
	KEGS_INVITE_PARTY_REPLY_NOT kPacket;
#ifdef SERV_INVITE_PARTY_REPLY_REASON 
	kPacket.m_cReplyReason = Reason;
#else
	kPacket.m_bAccept = bAccept;
#endif 
	kPacket.m_iPartyUID = partyUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_INVITE_PARTY_REPLY_NOT, kPacket );

	return true;
}
//}} 김상훈 : 2010. 10. 04 파티 초대 관련 UI 개선

bool CX2PartyManager::Handler_EGS_INVITE_PARTY_DENY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INVITE_PARTY_DENY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	// 김상훈 : 2010. 10. 04 파티 초대 관련 UI 개선

#ifndef SERV_INVITE_PARTY_REPLY_REASON 	
	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
#endif
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		//{{ kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
		userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		userData.iOpponentUID	= kEvent.m_iUnitUID;
		//}} kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_LEADER_INVITE_PARTY, userData );

#ifdef SERV_INVITE_PARTY_REPLY_REASON 	
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStrF(kEvent.m_iDenyReasonNetError), g_pMain->GetNowState() );
#endif SERV_INVITE_PARTY_REPLY_REASON 	
	}
	return true;

}








bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_DUNGEON_REQ( int iDungeonID, int iDifficultyLevel, int iDungeonMode )
{
	if( m_bStartGame == true )
	{
		m_bChangeMap = false;
		return true;
	}

	KEGS_PARTY_CHANGE_DUNGEON_REQ kPacket;
	kPacket.m_iDungeonID		= iDungeonID;
	kPacket.m_DifficultyLevel	= (char) iDifficultyLevel;
#ifdef HENIR_TEST
	kPacket.m_cDungeonMode = (char) iDungeonMode;
#endif HENIR_TEST

	g_pData->GetServerProtocol()->SendPacket( EGS_PARTY_CHANGE_DUNGEON_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PARTY_CHANGE_DUNGEON_ACK ); 

	m_bChangeMap = true;

	return true;
}


bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_DUNGEON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_CHANGE_DUNGEON_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PARTY_CHANGE_DUNGEON_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing
		}
	}

	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_DUNGEON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	if( m_bStartGame == true )
	{
		m_bChangeMap = false;
		return true;
	}

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_CHANGE_DUNGEON_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == DoIHaveParty() )
	{
		GetMyPartyData()->m_iDungeonID			= kEvent.m_iDungeonID;
		GetMyPartyData()->m_iDungeonDifficulty	= kEvent.m_DifficultyLevel;
#ifdef HENIR_TEST
		GetMyPartyData()->m_iDungeonMode	= kEvent.m_cDungeonMode;
#endif HENIR_TEST
	}

	CX2PartyUI* pPartyUI = g_pMain->GetPartyUI();
	if ( NULL != pPartyUI )
		pPartyUI->UpdatePartyDLG();
	m_bChangeMap = false;
	m_bStartGame = false;

	return true;
}

bool CX2PartyManager::Handler_EGS_UPDATE_PARTY_USER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_PARTY_USER_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == DoIHaveParty() )
	{
		CX2PartyManager::PartyMemberData* pMemberData = GetMyPartyData()->GetPartyMemberData( kEvent.m_iUnitUID );
		if( NULL != pMemberData )
		{
			pMemberData->m_eState		= (CX2Unit::CONNECT_UNIT_STATE) kEvent.m_cState;
			pMemberData->m_iStateCode	= kEvent.m_iStateCode;
			pMemberData->m_eUnitClass	= (CX2Unit::UNIT_CLASS) kEvent.m_cUnitClass;
			pMemberData->m_iUnitLevel	= (int) kEvent.m_ucLevel;

			//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
			pMemberData->m_iEquippedTitleID = kEvent.m_iEquippedTitleID;
#endif	SERV_TOONILAND_CHANNELING_EVENT
			//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
		}

		if( NULL != g_pMain->GetPartyUI() )
		{
			g_pMain->GetPartyUI()->UpdatePartyDLG();
//			g_pMain->GetPartyUI()->UpdatePartyMemberDLG();
		}

		//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		if ( NULL != GetMyPartyData() )
		{
			bool bPartyHaveTooniLandEventTitle = false;
			BOOST_TEST_FOREACH( const CX2PartyManager::PartyMemberData&, value, GetMyPartyData()->m_vecPartyMemberData )
			{
				if ( value.m_iEquippedTitleID == TOONILAND_CHANNELING_EVENT_TITLE )
				{
					bPartyHaveTooniLandEventTitle = true;
					break;
				}
			}

			// 변경 되었으면
			if ( GetMyPartyData()->m_bHaveTooniLandEventTitle != bPartyHaveTooniLandEventTitle )
			{
				if ( true == bPartyHaveTooniLandEventTitle && NULL != g_pTFieldGame )
					g_pTFieldGame->OpenBuffName( false, 124 );

				GetMyPartyData()->m_bHaveTooniLandEventTitle = bPartyHaveTooniLandEventTitle;
				g_pData->GetPremiumBuffManager()->SetTooniLandChannelingEvent( bPartyHaveTooniLandEventTitle );
			}			
		}
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

		CX2GageManager::GetInstance()->UpdatePartyMemberLevel( kEvent.m_iUnitUID, kEvent.m_ucLevel );
	}

	return true;
}


void CX2PartyManager::Handler_EGS_QUICK_START_DUNGEON_GAME_REQ( const CX2PartyManager::PartyData* pPartyData_ )
{
	KEGS_QUICK_START_DUNGEON_GAME_REQ kPacket;

	kPacket.m_iDungeonID		= pPartyData_->m_iDungeonID;
	kPacket.m_DifficultyLevel	= pPartyData_->m_iDungeonDifficulty;
	
	ASSERT( 128 > pPartyData_->m_iDungeonMode );
	kPacket.m_cDungeonMode		= static_cast<char>( pPartyData_->m_iDungeonMode );
//	kPacket.m_bCheckLowLevel	= false;
	kPacket.m_cGetItemType		= 0;
	
	g_pData->GetServerProtocol()->SendPacket( EGS_QUICK_START_DUNGEON_GAME_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_QUICK_START_DUNGEON_GAME_ACK ); 

	//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
	// Change_state_Game_start_req를 전송하면 직후에는 던전 게임이 시작 할 때 까지스킬슬롯을 바꿀수 없도록 함
	g_pInstanceData->SetSendGameStartReqPacket( true );
#endif MODIFY_INFINITE_SKILL_BUG
	//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정
}

bool CX2PartyManager::Handler_EGS_QUICK_START_DUNGEON_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_QUICK_START_DUNGEON_GAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_QUICK_START_DUNGEON_GAME_ACK ) == true )
	{
		//{{ oasis907 : 김상윤 [2010.5.6] // 		
		if( kEvent.m_iOK == NetError::ERR_DUNGEON_REQUIRE_ITEM_00)
		{
			// nothing - 에러메시지는 EGS_ERROR_POPUP_NOT에서 처리 
		}
		//}}
		else if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//현재 인원으로 시작 후 단축키 사용 못하도록 변경
			CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
			if ( NULL != pNowState )
				pNowState->SetEnableShortCutKey(false);
			// nothing
		}
		
		if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SKILL ) == true )
		{
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL );
		}
	}
	
	//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
		g_pInstanceData->SetSendGameStartReqPacket( false );
#endif MODIFY_INFINITE_SKILL_BUG
	//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정


	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_GAME_START_REQ()
{
	if( m_bChangeMap == true )
	{
		// 현재 맵 변경중이라 게임 시작할 수 없다.
		return false;
	}
	KEGS_PARTY_GAME_START_REQ kPacket;
	//kPacket.m_sWorldID = (short) iWorldID;


#ifndef _IN_HOUSE_
	if( SEnum::DI_MONSTER_TEST_NORMAL == GetMyPartyData()->m_iDungeonID )
	{
		return false;
	}
#endif _IN_HOUSE_

#ifndef SERV_NEW_DEFENCE_DUNGEON
	//{{ 허상형 : [2011/3/24/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
	  if( true == g_pData->GetDungeonManager()->IsDefenceDungeon( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )  )
	  {
		  if( false == g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() )
		  {
			  //	던전이 닫힌상태면 시작안됨
			  g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_11394 ), g_pMain->GetNowState() );

			  return false;
		  }

	  }
	  

#endif SERV_INSERT_GLOBAL_SERVER
	//}} 허상형 : [2011/3/24/] //	월드 미션
#endif //SERV_NEW_DEFENCE_DUNGEON

	// 게임 시작 직후에 skill slot 바꾸기를 시도하면 안되게 막는다. 버그 플레이 때문에. 
	// 장착스킬 정보를 방에 들어왔을 때 받고, 게임 시작 직후에 다시 받는데, 
	// 스킬 사용시에 MP 소모량 정보가 게임시작전에는 변경이 되는데 게임시작 후에는 갱신이 안되기 때문에 생기는 문제
	if( false == g_pInstanceData->IsElapsedPacketTiming() )
	{
		return false;
	}
	g_pInstanceData->GetTimerPacketTiming().restart();

	//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
	// Change_state_Game_start_req를 전송하면 직후에는 던전 게임이 시작 할 때 까지스킬슬롯을 바꿀수 없도록 함
	g_pInstanceData->SetSendGameStartReqPacket( true );
#endif MODIFY_INFINITE_SKILL_BUG
	//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정

// 빌드 에러 나서 이부분만 일시적으로 주석처리 임홍락
//#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
//	AHNHS_PROTECT_FUNCTION
//#endif

	g_pData->GetServerProtocol()->SendPacket( EGS_PARTY_GAME_START_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PARTY_GAME_START_ACK ); 

	m_bStartGame = true;


	return true;
}


bool CX2PartyManager::Handler_EGS_PARTY_GAME_START_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_PARTY_GAME_START_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PARTY_GAME_START_ACK ) == true )
	{
		//{{ oasis907 : 김상윤 [2010.5.6] // 		
		if( kEvent.m_iOK == NetError::ERR_DUNGEON_REQUIRE_ITEM_00)
		{
			// nothing - 에러메시지는 EGS_ERROR_POPUP_NOT에서 처리 
			m_bStartGame = false;
		}
		//}}
		else if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing
		}
		//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
		else
		{
			g_pInstanceData->SetSendGameStartReqPacket( false );
			m_bStartGame = false;
		}
#endif MODIFY_INFINITE_SKILL_BUG
		//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정
	}

	return true;
}


bool CX2PartyManager::Handler_EGS_PARTY_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_GAME_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	// 비밀 던전 입장권 소모 관련 인벤토리 갱신
	if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
	}
	

	//if( true == DoIHaveParty() )
	{
		m_bStartGame = true;

#ifdef HEAP_BROKEN_BY_ROOM
		CX2Room::InitializeRoomPacketData();
		CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
			kEvent.m_vecObserverSlot, kEvent.m_wstrCNIP );
#else	HEAP_BROKEN_BY_ROOM
		//방 데이터 제작
		CX2DungeonRoom* pDungeonRoom = g_pData->ResetDungeonRoom();
		pDungeonRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
		pDungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );						// fix!! bUnitInfo == false??? 
		pDungeonRoom->Set_KRoomObserverSlotInfoList( kEvent.m_vecObserverSlot );
		pDungeonRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort ); // relay connect 
		pDungeonRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );
#endif // HEAP_BROKEN_BY_ROOM

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
            g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_DUNGEON ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


		RememberComeBackInfoBeforeInstanceGameStart();

 		GetMyPartyData()->m_iDungeonID			= kEvent.m_RoomInfo.m_iDungeonID;
 		GetMyPartyData()->m_iDungeonDifficulty	= kEvent.m_RoomInfo.m_DifficultyLevel;
	#ifdef HENIR_TEST
 		GetMyPartyData()->m_iDungeonMode	= kEvent.m_RoomInfo.m_cDungeonMode;
	#endif HENIR_TEST

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-16
		/// 최근에 입장한 던전 아이디 저장
		if ( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			g_pData->GetMyUser()->GetSelectUnit()->SetRecentEnterDungeonID( kEvent.m_RoomInfo.m_iDungeonID );
#endif // SERV_NEW_DEFENCE_DUNGEON

		const CX2Dungeon::DungeonData* pDungeonData 
			= g_pData->GetDungeonManager()->GetDungeonData( static_cast<SEnum::DUNGEON_ID>( kEvent.m_RoomInfo.m_iDungeonID ) );

		ASSERT( NULL != pDungeonData );
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_GAME, NULL, false );
		
		SetProcessDungeonMatch(false);
	}

	return true;
}

#ifdef SERV_NEW_DEFENCE_DUNGEON

/** @function	: Handler_EGS_DEFENCE_DUNGEON_GAME_START_NOT
	@brief		: 어둠의 문 이동 패킷 처리 함수
	@param		: Event Message
	@return		: 처리 여부
*/
bool CX2PartyManager::Handler_EGS_DEFENCE_DUNGEON_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_GAME_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	/// 몬스터 소환 카드용 6 ~ 9 번째 퀵슬롯 아이템 설정
	if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
	}
	

	//if( true == DoIHaveParty() )
	{
		m_bStartGame = true;

#ifdef HEAP_BROKEN_BY_ROOM
		CX2Room::InitializeRoomPacketData();
		CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
			kEvent.m_vecObserverSlot, kEvent.m_wstrCNIP );
#else	HEAP_BROKEN_BY_ROOM
		//방 데이터 제작
		CX2DungeonRoom* pDungeonRoom = g_pData->ResetDungeonRoom();
		pDungeonRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
		pDungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );						// fix!! bUnitInfo == false??? 
		pDungeonRoom->Set_KRoomObserverSlotInfoList( kEvent.m_vecObserverSlot );
		pDungeonRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort ); // relay connect 
		pDungeonRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );
#endif // HEAP_BROKEN_BY_ROOM

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
            g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_DUNGEON ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		/// 입장 위치는 이전 던전에서 설정하였으므로, 갱신해 줄 필요가 없다.
		//RememberComeBackInfoBeforeInstanceGameStart();

 		GetMyPartyData()->m_iDungeonID			= kEvent.m_RoomInfo.m_iDungeonID;
 		GetMyPartyData()->m_iDungeonDifficulty	= kEvent.m_RoomInfo.m_DifficultyLevel;
	#ifdef HENIR_TEST
 		GetMyPartyData()->m_iDungeonMode		= kEvent.m_RoomInfo.m_cDungeonMode;
	#endif HENIR_TEST

		const CX2Dungeon::DungeonData* pDungeonData 
			= g_pData->GetDungeonManager()->GetDungeonData( static_cast<SEnum::DUNGEON_ID>( kEvent.m_RoomInfo.m_iDungeonID ) );

		ASSERT( NULL != pDungeonData );

		/// 페이드 아웃 효과 설정 및 1초 후 어둠의 문 스테이트 변경 시작
		if( NULL != g_pKTDXApp->GetDGManager() && NULL != g_pKTDXApp->GetDGManager() )
		{
			g_pKTDXApp->GetDGManager()->StartFadeOut( 1.0f );

			/// 시작전까지 1초 대기 설정
			if ( NULL != g_pMain && CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() )
			{

				CX2StateDungeonGame* pState = (CX2StateDungeonGame*) g_pMain->GetNowState();
				if( NULL != pState )
				{
					pState->SetWaitDefenceDungeonStartTime( 1.f );
				}
			}
		}
		else	/// 만약 페이드 아웃을 동작하는데 문제가 있다면, 바로 스테이트 변경 시작 ( 사실 이쪽이 동작될 일은 없다. )
		{
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_GAME, NULL, false );
		}
		
		SetProcessDungeonMatch(false);
	}

	return true;
}

#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ( bool bLimit )
{
	KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ kPacket;
	kPacket = bLimit;

	g_pData->GetServerProtocol()->SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK ); 
	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing
		}
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		else
		{
			if( NULL != g_pMain->GetPartyUI() )
			{
				g_pMain->GetPartyUI()->UpdatePartyDLG();
			}
		}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	}

	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == DoIHaveParty() )
	{
		g_pData->GetPartyManager()->GetMyPartyData()->m_bIsLimitLevel = kEvent;

		if( NULL != g_pMain->GetPartyUI() )
		{
			g_pMain->GetPartyUI()->UpdatePartyDLG();
		}

	}

	return true;
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION



bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_HOST_REQ( UidType unitUID )
{
	KEGS_PARTY_CHANGE_HOST_REQ kPacket;
	kPacket = unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_PARTY_CHANGE_HOST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PARTY_CHANGE_HOST_ACK ); 
	return true;
}


bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_HOST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_PARTY_CHANGE_HOST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PARTY_CHANGE_HOST_ACK ) == true )
	{
		//{{ oasis907 : 김상윤 [2010.5.6] // 		
		if( kEvent.m_iOK == NetError::ERR_DUNGEON_REQUIRE_ITEM_00)
		{
			// nothing - 에러메시지는 EGS_ERROR_POPUP_NOT에서 처리 
		}
	}

	return true;
}


bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_HOST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_CHANGE_HOST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == DoIHaveParty() )
	{
		CX2PartyManager::PartyMemberData* pOldHost = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( kEvent.m_iBeforeUID );
		if( NULL != pOldHost )
		{
			pOldHost->m_bPartyLeader = false;
		}

		CX2PartyManager::PartyMemberData* pNewHost = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( kEvent.m_iAfterUID );
		if( NULL != pNewHost )
		{
			pNewHost->m_bPartyLeader = true;

			//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
			// 재결성 파티가 아닐때만 팝업을 띄운다
			if( kEvent.m_bRegroupParty == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
			//}}
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_379, "L", pNewHost->m_wstrNickName ) ), g_pMain->GetNowState() );
			}
		}
		g_pData->GetPartyManager()->GetMyPartyData()->UpdatePartyLeaderUID();

		if( NULL != g_pMain->GetPartyUI() )
		{
			g_pMain->GetPartyUI()->UpdatePartyDLG();
			//			g_pMain->GetPartyUI()->UpdatePartyMemberDLG();
			g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START);
			g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START);
		}
	}

	return true;
}




bool CX2PartyManager::Handler_EGS_PARTY_BAN_USER_REQ( UidType unitUID )
{
	KEGS_PARTY_BAN_USER_REQ kPacket;
	kPacket = unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_PARTY_BAN_USER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PARTY_BAN_USER_ACK ); 

#ifdef NEW_MAIL_LOG
	if( CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() )
	{
		CTime NowTime = CTime::GetCurrentTime();
		char Buf1[256];
		StringCchPrintfA( Buf1, 256, "%d:%d:%d, %s", NowTime.GetHour(), NowTime.GetMinute(), NowTime.GetSecond(), "PARTY_BAN_USER_REQ" );
		//CX2MailLogManager::GetInstance()->AddMailLog( CX2MailLogManager::MLI_CLOSE_PARTY_BUG, Buf1 );
	}
#endif // NEW_MAIL_LOG

	return true;
}


bool CX2PartyManager::Handler_EGS_PARTY_BAN_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_PARTY_BAN_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PARTY_BAN_USER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing
		}
	}

	return true;
}


bool CX2PartyManager::Handler_EGS_PARTY_BAN_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 내가 강퇴 당한 경우
	if( true == DoIHaveParty() )
	{
		GetMyPartyData()->ClearPartyMemberData();
		GetMyPartyData()->m_iPartyMemberCount = 0;
		GetMyPartyData()->m_iPartyUID = 0;

		if( NULL != g_pMain->GetPartyUI() )
		{
			g_pMain->GetPartyUI()->UpdatePartyDLG();

#ifdef HENIR_TEST
			g_pMain->GetPartyUI()->OpenPartyFeverDLG( false );
#endif HENIR_TEST
			g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START);
			g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START);
		}

		CX2GageManager::GetInstance()->ClearPartyMemberUI();

#ifdef NEW_MAIL_LOG
		AddClosePartyLog( "EGS_PARTY_BAN_USER_NOT", 2);
#endif // NEW_MAIL_LOG
	}


	return true;
}


bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ( CX2Room::DUNGEON_GET_ITEM_TYPE eMode )
{
	KEGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ kPacket;
	kPacket = (char) eMode;

	g_pData->GetServerProtocol()->SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK ); 
	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing
		}
	}

	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == DoIHaveParty() )
	{
		g_pData->GetPartyManager()->GetMyPartyData()->m_eItemLootingMode = (CX2Room::DUNGEON_GET_ITEM_TYPE) kEvent;

		if( NULL != g_pMain->GetPartyUI() )
		{
			g_pMain->GetPartyUI()->UpdatePartyDLG();
		}
	}

	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_NUM_OF_PER_REQ( int iMaxSlot )
{
	KEGS_PARTY_CHANGE_NUM_OF_PER_REQ kPacket;
	kPacket = (char) iMaxSlot;

	g_pData->GetServerProtocol()->SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PARTY_CHANGE_NUM_OF_PER_ACK ); 
	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_NUM_OF_PER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_PARTY_CHANGE_NUM_OF_PER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PARTY_CHANGE_NUM_OF_PER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing
		}
	}

	return true;
}

bool CX2PartyManager::Handler_EGS_PARTY_CHANGE_NUM_OF_PER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_CHANGE_NUM_OF_PER_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == DoIHaveParty() )
	{
		g_pData->GetPartyManager()->GetMyPartyData()->m_iMaxPartyMemberCount = (int) kEvent;

		if( NULL != g_pMain->GetPartyUI() )
		{
			g_pMain->GetPartyUI()->UpdatePartyDLG();
		}
	}

	return true;
}

#ifdef HENIR_TEST

	bool CX2PartyManager::Handler_EGS_PARTY_FEVER_POINT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		KSerBuffer* pBuff = (KSerBuffer*)lParam;
		KEGS_PARTY_FEVER_POINT_NOT kEvent;
		DeSerialize( pBuff, &kEvent );

		if( true == DoIHaveParty() )
		{
			GetMyPartyData()->m_iPartyFever = kEvent.m_iPartyFever;
			
			if( NULL != g_pMain->GetPartyUI() )
			{
				g_pMain->GetPartyUI()->UpdatePartyDLG();

				g_pMain->GetPartyUI()->OpenPartyFeverDLG( true );
				g_pMain->GetPartyUI()->UpdatePartyFeverDLG();
			}
		}

		return true;
	}



	bool CX2PartyManager::Handler_EGS_CHANGE_PARTY_FEVER_STATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{

		KSerBuffer* pBuff = (KSerBuffer*)lParam;
		KEGS_CHANGE_PARTY_FEVER_STATE_NOT kEvent;
		DeSerialize( pBuff, &kEvent );

		if( true == DoIHaveParty() )
		{
			g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyFever = kEvent.m_iPartyFever;


			if( NULL != g_pMain->GetPartyUI() )
			{
				g_pMain->GetPartyUI()->UpdatePartyDLG();

				g_pMain->GetPartyUI()->OpenPartyFeverDLG( true );
				g_pMain->GetPartyUI()->UpdatePartyFeverDLG();
			}


			if( true == kEvent.m_bIsStart )
			{
				if( NULL != g_pX2Game )
				{
					if( NULL != g_pX2Game->GetMyUnit() )
					{
						const float MAGIC_HYPER_MODE_TIME = 30.f;
						g_pX2Game->PartyFeverAllUser( MAGIC_HYPER_MODE_TIME );
					}
				}
			}
		}

		return true;
	}




#endif HENIR_TEST

#ifdef SERV_PVP_NEW_SYSTEM
bool CX2PartyManager::ChangePartyType(bool bPvp, char cPvpMode)
{
	if( DoIHaveParty() == true )
	{
		GetMyPartyData()->m_bPvpParty = bPvp;
		GetMyPartyData()->m_pvpMode = cPvpMode;
		GetMyPartyData()->m_bIsLimitLevel = false;
		GetMyPartyData()->m_bPublic = true;

		//Handler_EGS_CHANGE_PARTY_TYPE_REQ( *GetMyPartyData() );
	}

	return true;
}
bool CX2PartyManager::Handler_EGS_CHANGE_PARTY_TYPE_REQ( const CX2PartyManager::PartyData& partyData )
{
	KEGS_CHANGE_PARTY_TYPE_REQ kPacket;

	wstring wstrPartyName = g_pMain->GetStringFilter()->FilteringChatString( partyData.m_wstrPartyTitle.c_str(), L'♡' );

	kPacket.m_iPartyUID			= -1;
	kPacket.m_wstrPartyName		= wstrPartyName;
	kPacket.m_iDungeonID		= partyData.m_iDungeonID;
	kPacket.m_DifficultyLevel	= (char) partyData.m_iDungeonDifficulty;
	kPacket.m_JoinSlot			= (char) partyData.m_iPartyMemberCount;
	kPacket.m_MaxSlot			= (char) partyData.m_iMaxPartyMemberCount;
	kPacket.m_bPublic			= true; //partyData.m_bPublic;
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	kPacket.m_bCheckLowLevel	= false; //partyData.m_bIsLimitLevel;
#endif SERV_DUNGEON_DIFFICULTY_REVISION

#ifdef COME_BACK_REWARD
	kPacket.m_bComeBackBuff		= partyData.m_bComeBackPower;
#endif

#ifdef HENIR_TEST
	kPacket.m_cDungeonMode = (char) partyData.m_iDungeonMode;
#endif HENIR_TEST
	
#ifdef SERV_PVP_NEW_SYSTEM
	if( partyData.m_bPvpParty == false )
	{
		kPacket.m_cPartyType = KPartyInfo::PT_DUNGEON_PARTY;
		kPacket.m_bPublic = true;
	}
	else
	{
		kPacket.m_cPartyType = KPartyInfo::PT_PVP_PARTY;
		kPacket.m_bPublic = false;
	}
	kPacket.m_cPVPGameType = partyData.m_pvpMode;	
#endif

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PARTY_TYPE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_PARTY_TYPE_ACK ); 
		
	return true;
}
bool CX2PartyManager::Handler_EGS_CHANGE_PARTY_TYPE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_CHANGE_PARTY_TYPE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_PARTY_TYPE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}
bool CX2PartyManager::Handler_EGS_CHANGE_PARTY_TYPE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PARTY_TYPE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
		
	GetMyPartyData()->Set_KPartyInfo( kEvent.m_kPartyInfo );

	if( NULL != g_pMain->GetPartyUI() )
	{
		if( GetMyPartyData()->m_bPvpParty == true )
		{
			g_pMain->GetPartyUI()->SetPvpPartyMenu(true);
		}
		else
		{
			g_pMain->GetPartyUI()->SetDungeonPartyMenu(true);
		}

		g_pMain->GetPartyUI()->OpenPartyDLG(true);
		g_pMain->GetPartyUI()->UpdatePartyDLG();

		if( GetMyPartyData()->m_bPvpParty == true )
			g_pMain->GetPartyUI()->SetPvpMatchPlayerNum( GetMyPartyData()->GetPartyMemberCount() );
	}

	return true;
}
bool CX2PartyManager::Handler_EGS_REQUEST_MATCH_MAKING_REQ(char cPlayerNum, char cPvpMode)
{
	if( GetProcessPvpMatch() == false )
	{
		KEGS_REQUEST_MATCH_MAKING_REQ kPacket;

		kPacket.m_cPvpPlayerCount = cPlayerNum;
		kPacket.m_cPvpGameType = cPvpMode;

		g_pData->GetServerProtocol()->SendPacket( EGS_REQUEST_MATCH_MAKING_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_REQUEST_MATCH_MAKING_ACK ); 
	}
	
	return true;
}
bool CX2PartyManager::Handler_EGS_REQUEST_MATCH_MAKING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_REQUEST_MATCH_MAKING_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_REQUEST_MATCH_MAKING_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}
bool CX2PartyManager::Handler_EGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	SetProcessPvpMatch( true );
	g_pMain->GetPartyUI()->SetEstimatedTime( kEvent.m_iEstimatedTime );	
	g_pMain->GetPartyUI()->UpdatePvpPartyDlg();

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Battle_Atena_Start.ogg", false, false ); 
	g_pMain->GetPartyUI()->StartToWaitForPvpMatching();
	return true;
}

bool CX2PartyManager::Handler_EGS_MATCH_MAKING_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MATCH_MAKING_SUCCESS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

#ifdef FIELD_BOSS_RAID // 레이드 필드에서는 던전/대전 시작 불가
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		Handler_EGS_MATCH_MAKING_SUCCESS_REPLY_NOT(false);
		return true;
	}
#endif // FIELD_BOSS_RAID

	SetMatchUid( kEvent.m_iMatchUID );

	if( g_pMain->GetPartyUI() != NULL )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Battle_Atena_Success.ogg", false, false ); 

		g_pMain->GetPartyUI()->CheckPvpStart();
	}

	return true;
}
bool CX2PartyManager::Handler_EGS_CANCEL_MATCH_MAKING_REQ()
{
	if( GetProcessPvpMatch() == true )
	{
		g_pData->GetServerProtocol()->SendID( EGS_CANCEL_MATCH_MAKING_REQ );
		g_pMain->AddServerPacket( EGS_CANCEL_MATCH_MAKING_ACK ); 

		g_pMain->GetPartyUI()->CancelToWaitForPvpMatching();
	}

	return true;
}
bool CX2PartyManager::Handler_EGS_CANCEL_MATCH_MAKING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_CANCEL_MATCH_MAKING_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CANCEL_MATCH_MAKING_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2PartyManager::Handler_EGS_UNREG_MATCH_WAIT_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UNREG_MATCH_WAIT_LIST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	
	
	if( kEvent.m_iReason != NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 )
	{
		// 매칭중 파티원 탈퇴로 인한 대기열 삭제
		if( kEvent.m_iReason == NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 )
		{
			if( GetProcessPvpMatch() == true && 
				kEvent.m_wstrCancelUnitNickName.empty() == false &&
				g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName != kEvent.m_wstrCancelUnitNickName )
			{	
				wstring wstrLeavePartyMember = GET_REPLACED_STRING( ( STR_ID_12713, "L", kEvent.m_wstrCancelUnitNickName ) ); 
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrLeavePartyMember.c_str(), g_pMain->GetNowState() );
			}	
		}

		g_pMain->GetPartyUI()->CancelToWaitForPvpMatching();
		g_pMain->GetPartyUI()->CancelToWaitForDungeonPartyMatching();
		SetProcessPvpMatch(false);
		g_pMain->GetPartyUI()->UpdatePvpPartyDlg();

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Battle_Atena_Fail.ogg", false, false ); 
	}

	return true;
}

bool CX2PartyManager::Handler_EGS_MATCH_CLOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MATCH_CLOSE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	

	// 매칭 취소 성공
	SetProcessPvpMatch(false);
	g_pMain->GetPartyUI()->UpdatePvpPartyDlg();
	g_pMain->GetPartyUI()->CancelToWaitForPvpMatching();

	//매칭 실패했다면 단축키 사용 할 수 있도록 변경
	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState )
		pNowState->SetEnableShortCutKey(true);

	switch( kEvent.m_iReason )
	{
	case NetError::NOT_LEAVE_MATCH_REASON_00:// 게임시작으로 인한 매치 종료
	case NetError::NOT_LEAVE_MATCH_REASON_02://대전 매칭이 취소되었습니다.
		{				
			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PVP_START, userData );
			//g_pData->GetPartyManager()->Handler_EGS_MATCH_MAKING_SUCCESS_REPLY_NOT(false);
			g_pMain->GetPartyUI()->CancelToWaitForPvpMatching();
			SetProcessPvpMatch(false);
		}
		break;
	case NetError::NOT_LEAVE_MATCH_REASON_01://대전 매칭 대상자 중 누군가가 참여하지 않았습니다.
		{
			if( kEvent.m_iCancelUnitUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
				// 대전 매칭 대상자 중 누군가가 참여하지 않았습니다.
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PVP_START, userData );				
				RemovePvpPartyMsg();
				m_pDlgPvpPartyMsg = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12719 ), g_pMain->GetNowState() );
				g_pMain->GetPartyUI()->StartToWaitForPvpMatching();
			}
		}
		break;
	case NetError::NOT_LEAVE_MATCH_REASON_03:
		{
			// 파티원이 대전에 참여하지 않아서 대전 매칭이 취소되었습니다.
			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PVP_START, userData );
			RemovePvpPartyMsg();
			m_pDlgPvpPartyMsg = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12721 ), g_pMain->GetNowState() );
			g_pMain->GetPartyUI()->CancelToWaitForPvpMatching();
			SetProcessPvpMatch(false);
		}
		break;
	default:
		break;
	}	

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Battle_Atena_Fail.ogg", false, false ); 
	

	return true;
}

bool CX2PartyManager::Handler_EGS_MATCH_MAKING_SUCCESS_REPLY_NOT( bool bStartAgree )
{
	KEGS_MATCH_MAKING_SUCCESS_REPLY_NOT kPacket;

 	kPacket.m_bGameStartAgree = bStartAgree;
 	kPacket.m_bPvpItemMode = false;
	kPacket.m_bPvpPowerMode = false;
 	g_pMain->GetPartyUI()->GetSelectedPvpMap(kPacket.m_setSelectedPvpMap);

	g_pData->GetServerProtocol()->SendPacket( EGS_MATCH_MAKING_SUCCESS_REPLY_NOT, kPacket );


	if( NULL != g_pMain->GetPartyUI() )
	{
		// 게임 시작 대기중 던전/대전 시작 버튼 비활성화
		if( true == kPacket.m_bGameStartAgree )
		{
			g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START, false, true);
			g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START, false, true);
		}
		else
		{
			g_pMain->GetPartyUI()->CancelToWaitForPvpMatching();
		}
	}
	return true;
}
bool CX2PartyManager::Handler_EGS_GAME_START_PVP_MATCH_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 매칭된 데이타로 실제 pvp 게임 시작
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GAME_START_PVP_MATCH_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	// 현재 위치 저장
// 	if( g_pTFieldGame != NULL )
// 	{
// 		g_pData->GetLocationManager()->SetLastPos( g_pTFieldGame->GetMyUnit()->GetLandPosition() );
// 		g_pData->GetLocationManager()->SetLastPosLineIndex( g_pTFieldGame->GetMyUnit()->GetLastTouchLineIndex() );
// 	}	

	RememberComeBackInfoBeforeInstanceGameStart();

#ifdef HEAP_BROKEN_BY_ROOM
	CX2Room::InitializeRoomPacketData();
	CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
		kEvent.m_vecObserverSlot, wstring( L"" ) );
	CX2Room::SetOfficialPvp( true );
	CX2Room::SetPvpNpcSlotInfo( kEvent.m_mapPvpNpcInfo );
#else	HEAP_BROKEN_BY_ROOM
	//방 데이터 제작
	CX2PVPRoom* pCX2PVPRoom = g_pData->ResetPVPRoom();
	pCX2PVPRoom->SetOfficialPvp( true );
	pCX2PVPRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
	pCX2PVPRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );
	pCX2PVPRoom->Set_KRoomObserverSlotInfoList( kEvent.m_vecObserverSlot );
	pCX2PVPRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
	pCX2PVPRoom->Set_KRoomSlotInfoNpc( kEvent.m_mapPvpNpcInfo );
#endif // HEAP_BROKEN_BY_ROOM

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
        g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_PVP ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



	g_pMain->SetConnectedChannelID( kEvent.m_RoomInfo.m_iPVPChannelClass );

	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_GAME, NULL, false );

	g_pMain->GetPartyUI()->OpenPartyDLG(false);
	SetProcessPvpMatch(false);

	g_pData->GetUIManager()->CloseAll();

	return true;
}

bool CX2PartyManager::Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ( char cPvpType )
{
	KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ kPacket;

	kPacket.m_cPvpGameType = cPvpType;

	g_pData->GetServerProtocol()->SendPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK ); 

	return true;
}
bool CX2PartyManager::Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}
bool CX2PartyManager::Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	
		
	g_pMain->GetPartyUI()->UpdatePvpPartyDlg();

	return true;
}
#endif

//////////////////////////////////////////////////////////////////////////
CX2PartyManager::PartyMemberData* CX2PartyManager::GetMyPartyMemberData()
{
	if( NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return NULL;

	return m_MyPartyData.GetPartyMemberData( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
}

bool CX2PartyManager::DoIHaveParty()
{
	if( m_MyPartyData.m_iPartyUID <= 0 )
	{
		return false;
	}

	return true;
}
bool CX2PartyManager::AmIPartyLeader()
{
	if( false == DoIHaveParty() )
		return false;

	if( NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() &&
		m_MyPartyData.m_iPartyLeaderUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
	{
		return true;
	}

	return false;
}


bool CX2PartyManager::IsMyPartyMember(UidType iUid)
{
	if( m_MyPartyData.GetPartyMemberData(iUid) != NULL )
		return true;

	return false;
}

bool CX2PartyManager::IsAllReady()
{
	for( UINT i=0; i<m_MyPartyData.m_vecPartyMemberData.size(); i++ )
	{
		const PartyMemberData& memberData = m_MyPartyData.m_vecPartyMemberData[i];

		if( false == memberData.m_bPartyLeader &&
			false == memberData.m_bGameReady )
			return false;

	}

	return true;
}




// 특정 지역(마을, 던전게이트)에 속하는 던전인지 검사해줌
bool CX2PartyManager::IsDungeonInLocal( int iLocalMapID, int iDungeonID )
{
	CX2LocationManager::LocalMapTemplet* pTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( (CX2LocationManager::LOCAL_MAP_ID) iLocalMapID );
	if( NULL == pTemplet )
		return false;

	for( UINT i=0; i< pTemplet->m_DungeonList.size(); i++ )
	{
		if( pTemplet->m_DungeonList[i] == iDungeonID )
			return true;
	}

	return false;
}


bool CX2PartyManager::IsRankableArcadeDungeon( int iDungeonID )
{
	const CX2Dungeon::DungeonData* pDungeonData = 
		g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) iDungeonID );

	if( NULL == pDungeonData )
		return false;

// 	if( false == pDungeonData->m_UIData.m_bArcadeMode )
// 		return false;


	for( int i=0; i<GetMyPartyData()->GetPartyMemberCount(); i++ )
	{
		PartyMemberData* pMemberData = GetMyPartyData()->GetPartyMemberData( i );
		if( NULL == pMemberData )
			continue;

		if( pMemberData->m_iUnitLevel > pDungeonData->m_MaxLevel )
		{
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// @exceptUnitUID: 나를 제외한 파티원들을 순서대로 접근하는 경우가 있어서, 특정 unituid를 제외한 다른 파티원 중 index 번째 파티원 정보를 return 
CX2PartyManager::PartyMemberData* CX2PartyManager::PartyData::GetPartyMemberData( const int index, UidType exceptUnitUID /*= -1 */ )
{
	if( index < 0 || index >= (int) m_vecPartyMemberData.size() )
		return false;

	if( -1 != exceptUnitUID )
	{
		int iPartyMemberIndex = 0;
		for( UINT i=0; i<m_vecPartyMemberData.size(); i++ )
		{
			PartyMemberData& memberData = m_vecPartyMemberData[i];

			if( memberData.m_iUnitUID == exceptUnitUID )
				continue;

			if( iPartyMemberIndex == index )
			{
				return &memberData;
			}

			iPartyMemberIndex++;
		}

		return NULL;
	}
	else
	{
		return &m_vecPartyMemberData[index];
	}
}

CX2PartyManager::PartyMemberData* CX2PartyManager::PartyData::GetPartyMemberData( const UidType iUnitUID )
{
	for( UINT i=0; i< m_vecPartyMemberData.size() ; i++ )
	{
		if( iUnitUID == m_vecPartyMemberData[i].m_iUnitUID )
		{
			return &m_vecPartyMemberData[i];
		}
	}

	return NULL;
}

CX2PartyManager::PartyMemberData* CX2PartyManager::PartyData::GetPartyMemberData( const wstring& nickName )
{
	for( UINT i=0; i< m_vecPartyMemberData.size() ; i++ )
	{
		if( 0 == nickName.compare( m_vecPartyMemberData[i].m_wstrNickName ) )
		{
			return &m_vecPartyMemberData[i];
		}
	}
	return NULL;
}
bool CX2PartyManager::Handler_EGS_PARTY_GAME_START_OPINION_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG 
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_PARTY_GAME_START_OPINION_CHECK_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
#endif //SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG 

#ifdef FIELD_BOSS_RAID // 레이드 필드에서는 던전/대전 시작 불가
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		Send_EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT( false );
		return true;
	}
#endif // FIELD_BOSS_RAID
	if ( !AmIPartyLeader() )
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Battle_Atena_Success.ogg", false, false ); 

#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
		SEnum::DUNGEON_ID eDungeonID				= static_cast<SEnum::DUNGEON_ID>(kEvent.m_iDungeonID);
		CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel	= static_cast<CX2Dungeon::DIFFICULTY_LEVEL>(kEvent.m_cDifficultyLevel);
		CX2Dungeon::DUNGEON_MODE eDungeonMode			= static_cast<CX2Dungeon::DUNGEON_MODE>(kEvent.m_cDungeonMode);
#else
		SEnum::DUNGEON_ID eDungeonID				= (SEnum::DUNGEON_ID )m_MyPartyData.m_iDungeonID;
		CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel	= (CX2Dungeon::DIFFICULTY_LEVEL) m_MyPartyData.m_iDungeonDifficulty;
		CX2Dungeon::DUNGEON_MODE eDungeonMode			= (CX2Dungeon::DUNGEON_MODE) m_MyPartyData.m_iDungeonMode;
#endif //SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
				
		wstring wstrDungeonName = 
			g_pData->GetDungeonManager()->MakeDungeonNameString( eDungeonID, eDifficultyLevel, eDungeonMode);
		
		g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_READY_TO_PLAY_DUNGEON_REQUEST,
			CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 10.f, 
			GET_REPLACED_STRING( ( STR_ID_21304, "Li", wstrDungeonName, m_MyPartyData.GetPartyMemberCount() ) ), 
			(CKTDXStage*) g_pMain->GetNowState(), 
			CX2PartyUI::PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_OK, CX2PartyUI::PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_CANCEL, CX2PartyUI::PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_CANCEL);
	}
	return true;
}

void CX2PartyManager::Send_EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT( const bool bAccept_ )
{
	KEGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT kPacket;
	kPacket.m_bAccept = bAccept_;

	g_pData->GetServerProtocol()->SendPacket( EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, kPacket );
}

bool CX2PartyManager::Handler_EGS_PARTY_GAME_START_FAILED_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PARTY_GAME_START_FAILED_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	
	
	const UidType uidOfMine = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	// 자신이 취소한 유저가 아니면
	if ( uidOfMine != kEvent.m_iFailUnitUID )
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID = uidOfMine;
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_READY_TO_PLAY_DUNGEON_REQUEST, userData );
#ifdef HARDCODING_STRING_TO_INDEX
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING(STR_ID_24596), g_pMain->GetNowState() );
#else
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"파티구성원 중 취소한 유저가 있습니다.", g_pMain->GetNowState() );
#endif HARDCODING_STRING_TO_INDEX
	}

	//매칭 실패했다면 단축키 사용 할 수 있도록 변경
	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState )
		pNowState->SetEnableShortCutKey(true);
	
	return true;
}

void CX2PartyManager::RememberComeBackInfoBeforeInstanceGameStart()
{
	static_cast<CX2State*>( g_pMain->GetNowState() )->Send_EGS_UPDATE_PLAY_STATUS_NOT();

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:	// 던전의 시작 위치가 배틀필드라면
		{
			CX2LocationManager* pLocationManager = g_pData->GetLocationManager();
			pLocationManager->SetX2StateBeforeDungeonStart( static_cast<const UINT>( g_pMain->GetNowStateID() ) );

			ASSERT( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() );
			if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
			{
				static_cast<CX2BattleFieldGame*>(g_pX2Game)->Send_EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT();
			}
		} break;

	case CX2Main::XS_SERVER_SELECT: 
		{	//튜토리얼의 경우 XS_SERVER_SELECT에서 시작
			CX2LocationManager* pLocationManager = g_pData->GetLocationManager();
			pLocationManager->SetX2StateBeforeDungeonStart( static_cast<const UINT>( g_pMain->GetNowStateID() ) );


			const CX2Unit::UnitData* pUnitData = 	&g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

			if ( SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01 <= pUnitData->m_nMapID && 
				SEnum::VMI_BATTLE_FIELD_END > pUnitData->m_nMapID )
			{
				//이전 접속 위치가 필드
			}
			else
			{
				//이전 접속 위치 마을
				// 로그아웃했던 위치를 계산한다.
#ifdef REMEMBER_LOGOUT_POSITION_TEST
				int iMapId = 0;
				if(g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL)
					iMapId = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_nMapID;				

				int startPos = 0;
				startPos = g_pData->GetLocationManager()->GetLoginPos( iMapId );

				D3DXVECTOR3 vStartPos(0, 0, 0);

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

								pLocationManager->SetVillageIdBeforeDungeonStart( iMapId );
								pLocationManager->SetLastTouchedIndexInVillageBeforeDungeonStart( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ucLastTouchLineIndex );
								pLocationManager->SetPositionInVillageBeforeDungeonStart( vStartPos );
							}
						}
						SAFE_DELETE( pWorld );
					}
				}
#endif REMEMBER_LOGOUT_POSITION_TEST
			}

		} break;
	case CX2Main::XS_VILLAGE_MAP:
		{
			// 던전의 시작 위치가 마을이라면
			if( false == g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) )
			{
				CX2LocationManager* pLocationManager = g_pData->GetLocationManager();
				pLocationManager->SetX2StateBeforeDungeonStart( static_cast<const UINT>( g_pMain->GetNowStateID() ) );
				pLocationManager->SetVillageIdBeforeDungeonStart( static_cast<const UINT>( pLocationManager->GetCurrentVillageID() ) );

				ASSERT( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetMyUnit() );
				if ( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetMyUnit() )
				{
					pLocationManager->SetLastTouchedIndexInVillageBeforeDungeonStart( g_pTFieldGame->GetMyUnit()->GetLastTouchLineIndex() );
					pLocationManager->SetPositionInVillageBeforeDungeonStart( g_pTFieldGame->GetMyUnit()->GetLandPosition() );
				}
			}
			else	// 던전의 시작 위치가 라운지라면
			{
				// 아무것도 안 함
			}
		} break;

	default:
		{
			StateLog( L"던전 시작 위치가 여기여도 되나?" );
			ASSERT( !L"던전 시작 위치가 여기여도 되나?" );
		} break;
	}
}

void CX2PartyManager::Handler_EGS_AUTO_PARTY_DUNGEON_GAME_REQ()
{
	//파티원이 4인 미만일 때만 매칭 가능하도록 변경.
	KEGS_AUTO_PARTY_DUNGEON_GAME_REQ kPacket;
	kPacket.m_cDungeonMode = static_cast<char>( GetMyPartyData()->m_iDungeonMode );
	kPacket.m_cGetItemType = static_cast<char>( GetMyPartyData()->m_eItemLootingMode );
	kPacket.m_DifficultyLevel = GetMyPartyData()->m_iDungeonDifficulty;
	kPacket.m_iDungeonID = GetMyPartyData()->m_iDungeonID;


	g_pData->GetServerProtocol()->SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK ); 
}

bool CX2PartyManager::Handler_EGS_AUTO_PARTY_DUNGEON_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK ) == true )
	{
		
		switch ( kEvent.m_iOK )
		{
		case NetError::NET_OK:
			return true;
			break;

			// 2012-10-10 김민성
		case NetError::ERR_DUNGEON_REQUIRE_ITEM_00:	/// 유저 레벨 제한 조건 만족 안함
			{
				// nothing - 에러메시지는 EGS_ERROR_POPUP_NOT에서 처리 
			} break;

		case NetError::ERR_DUNGEON_GAME_START_00:	/// 유저 레벨 제한 조건 만족 안함
			{
#ifdef HARDCODING_STRING_TO_INDEX
				wstring wstrErrorMessage = GET_REPLACED_STRING( ( STR_ID_24592, "L", kEvent.m_wstrFailUserNickName ) );
#else
				wstring wstrErrorMessage = L"#CFF0000\'";
				wstrErrorMessage += kEvent.m_wstrFailUserNickName;
				wstrErrorMessage += L"\'님이#CX\n";
				wstrErrorMessage += L"레벨 제한 조건을 만족하지 않습니다.\n자동파티 기능을 사용하실 수 없습니다.";
#endif HARDCODING_STRING_TO_INDEX
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), wstrErrorMessage.c_str(), static_cast<CKTDXStage*>( g_pMain->GetNowState() ) ); 

				return true;
			} break;

		case NetError::ERR_DUNGEON_GAME_START_01:	/// 아이템 레벨 제한 조건 만족 안함
			{
#ifdef HARDCODING_STRING_TO_INDEX
				wstring wstrErrorMessage = GET_REPLACED_STRING( ( STR_ID_24593, "L", kEvent.m_wstrFailUserNickName ) );
#else
				wstring wstrErrorMessage = L"#CFF0000\'";
				wstrErrorMessage += kEvent.m_wstrFailUserNickName;
				wstrErrorMessage += L"\'님이#CX\n";
				wstrErrorMessage += L"아이템 레벨 제한 조건을 만족하지 않습니다.\n자동파티 기능을 사용하실 수 없습니다.";
#endif HARDCODING_STRING_TO_INDEX
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), wstrErrorMessage.c_str(), static_cast<CKTDXStage*>( g_pMain->GetNowState() ) ); 

				return true;
			} break;

		case NetError::ERR_PARTY_61:		/// 대전 매칭 성공 MAX 인원으로 신청했을 때
			{
				/// 현재 인원으로 시작을 누른것 과 같이 처리 해준다.
				g_pMain->GetPartyUI()->GameStartCurrentMember();
				return true;
			} break;
		default:
			{
				if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
					return true;
			} break;
		}
	}

	return false;
}

#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
bool CX2PartyManager::Handler_EGS_AUTO_PARTY_WAIT_USER_COUNT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_AUTO_PARTY_WAIT_USER_COUNT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->GetPartyUI()->SetCurrentWaitUserCount(kEvent.m_iAutoPartyWaitUserCount);

	return true;
}
#endif //SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
bool CX2PartyManager::Handler_EGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT kEvent;

	DeSerialize( pBuff, &kEvent );

	// 자동 파티 재등록
	if ( true == kEvent.m_bRemakingAutoParty )
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	
		// 던전매칭에 의한 게임 시작을 묻는 팝업 삭제
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_DUNGEON_MATCH_START_REQUEST, userData );
		
		g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_DUNGEON_MATCH_REMAKING,
			CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 5.f, 
#ifdef HARDCODING_STRING_TO_INDEX
			GET_STRING(STR_ID_24595),
#else
			L"파티구성원 중 취소한 유저가 있습니다.\n최우선 순위로 파티를 다시 구성합니다.\n다음 시간 후 창이 닫힙니다.", 
#endif HARDCODING_STRING_TO_INDEX
			(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
	}

	g_pMain->GetPartyUI()->SetEstimatedTime( kEvent.m_iEstimatedTime );	
	g_pMain->GetPartyUI()->StartToWaitForDungeonPartyMatching();
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	g_pMain->GetPartyUI()->SetCurrentWaitUserCount(kEvent.m_iAutoPartyWaitUserCount);
#endif //SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	SetProcessDungeonMatch(true);
	return true;
}

void CX2PartyManager::Handler_EGS_CANCEL_AUTO_PARTY_MAKING_REQ()
{
	if( GetProcessDungeonMatch() == true )
	{
		g_pData->GetServerProtocol()->SendID( EGS_CANCEL_AUTO_PARTY_MAKING_REQ );
		g_pMain->AddServerPacket( EGS_CANCEL_AUTO_PARTY_MAKING_ACK ); 

		g_pMain->GetPartyUI()->CancelToWaitForDungeonPartyMatching();
	}
}

bool CX2PartyManager::Handler_EGS_CANCEL_AUTO_PARTY_MAKING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_CANCEL_AUTO_PARTY_MAKING_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CANCEL_AUTO_PARTY_MAKING_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			SetProcessDungeonMatch(false);
			return true;
		}
	}

	return false;
}

bool CX2PartyManager::Handler_EGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	kEvent.m_iReason;
	kEvent.m_wstrCancelUnitNickName;

	g_pMain->GetPartyUI()->CancelToWaitForDungeonPartyMatching();	
	SetProcessDungeonMatch(false);

	return true;
}

bool CX2PartyManager::Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_AUTO_PARTY_MAKING_SUCCESS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

#ifdef FIELD_BOSS_RAID // 레이드 필드에서는 던전/대전 시작 불가
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT( false );
		return true;
	}
#endif // FIELD_BOSS_RAID

	SEnum::DUNGEON_ID eDungeonID				= static_cast<SEnum::DUNGEON_ID>( kEvent.m_iDungeonID );
	CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel	= static_cast<CX2Dungeon::DIFFICULTY_LEVEL>(kEvent.m_cDifficultyLevel);
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
	CX2Dungeon::DUNGEON_MODE eDungeonMode			= static_cast<CX2Dungeon::DUNGEON_MODE>( kEvent.m_cDungeonMode );
#else
	CX2Dungeon::DUNGEON_MODE eDungeonMode			= static_cast<CX2Dungeon::DUNGEON_MODE>( GetMyPartyData()->m_iDungeonMode );
#endif //SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
	
	wstring wstrDungeonName = 
		g_pData->GetDungeonManager()->MakeDungeonNameString( eDungeonID, eDifficultyLevel, eDungeonMode);
	
	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	userData.iMyUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Battle_Atena_Success.ogg", false, false ); 
	g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_DUNGEON_MATCH_START_REQUEST,
		CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 10.f, 
		GET_REPLACED_STRING( ( STR_ID_21304, "Li", wstrDungeonName, kEvent.m_iMemberCount ) ), 
		(CKTDXStage*) g_pMain->GetNowState(), 
		CX2StateMenu::SMUCM_DUNGEON_MATCH_GAME_ACCEPT, CX2StateMenu::SMUCM_DUNGEON_MATCH_GAME_DENY, CX2StateMenu::SMUCM_DUNGEON_MATCH_GAME_DENY );
	return true;
}

void CX2PartyManager::Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT( const bool bAccept_ )
{
	KEGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT kEvent;
	kEvent.m_bGameStartAgree = bAccept_;

	g_pData->GetServerProtocol()->SendPacket( EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT, kEvent );

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	userData.iMyUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

	g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_DUNGEON_MATCH_START_REQUEST, userData );

	if( NULL != g_pMain->GetPartyUI() )
	{
		// 게임 시작 대기중 던전/대전 시작 버튼 비활성화
		if( true == kEvent.m_bGameStartAgree )
		{
			g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START, false, true);
			g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START, false, true);
		}
		else
		{
			g_pMain->GetPartyUI()->CancelToWaitForDungeonPartyMatching();
		}
	}
}

bool CX2PartyManager::Handler_EGS_AUTO_PARTY_CLOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_AUTO_PARTY_CLOSE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	kEvent.m_iCancelUnitUID;
	kEvent.m_iReason;


		switch( kEvent.m_iReason )
		{
		case NetError::NOT_LEAVE_AUTO_PARTY_REASON_00: //던전 게임 시작으로 인한 자동 파티 종료.
		case NetError::NOT_LEAVE_AUTO_PARTY_REASON_02: //자동 파티 취소 요청에 의해 성공적으로 종료되었습니다.
		case NetError::NOT_LEAVE_AUTO_PARTY_REASON_03: //파티원이 자동파티 게임시작에 동의하지 않아 자동파티가 취소되었습니다.
			{
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				userData.iMyUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
				g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_DUNGEON_MATCH_START_REQUEST, userData );
				g_pMain->GetPartyUI()->CancelToWaitForDungeonPartyMatching();
				SetProcessDungeonMatch(false);
			} break;
		case NetError::NOT_LEAVE_AUTO_PARTY_REASON_01://자동 파티 대상자 중 누군가가 게임 시작에 동의하지 않았습니다.
			{
				if( kEvent.m_iCancelUnitUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				{
					// 던전매칭에 의한 게임 시작을 묻는 팝업 삭제
					CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
					userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
					g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_DUNGEON_MATCH_START_REQUEST, userData );
					g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_DUNGEON_MATCH_REMAKING,
						CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 5.f, 
#ifdef HARDCODING_STRING_TO_INDEX
						GET_STRING(STR_ID_24595),
#else
						L"파티구성원 중 취소한 유저가 있습니다.\n최우선 순위로 파티를 다시 구성합니다.\n다음 시간 후 창이 닫힙니다.", 
#endif HARDCODING_STRING_TO_INDEX
						(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
					g_pMain->GetPartyUI()->StartToWaitForDungeonPartyMatching();
				}	
			} break;
		default:
			break;
		}

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Battle_Atena_Fail.ogg", false, false ); 

	//매칭 실패했다면 단축키 사용 할 수 있도록 변경
	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState )
		pNowState->SetEnableShortCutKey(true);

	return true;
}

bool CX2PartyManager::Handler_EGS_REGROUP_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_REGROUP_PARTY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	GetMyPartyData()->Init();
	GetMyPartyData()->Set_KPartyInfo( kEvent.m_kPartyInfo );

	ASSERT( SEnum::DI_NONE != GetMyPartyData()->m_iDungeonID );

	if ( SEnum::DI_NONE == GetMyPartyData()->m_iDungeonID )
		GetMyPartyData()->m_iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;

	const UidType uidMine = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

	for ( UINT i = 0; i < kEvent.m_vecPartyUserInfo.size(); ++i )
	{
		const KPartyUserInfo& kPartyUserInfo = kEvent.m_vecPartyUserInfo[i];

		// 자신이 아닌 경우에만
		
		GetMyPartyData()->AddPartyMemberData( kPartyUserInfo );

		if ( uidMine != kPartyUserInfo.m_iUnitUID )
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();
			if ( pGageManager->CanThisMemberInsert( kPartyUserInfo.m_iUnitUID ) )
			{
				CX2GameUnit* pGameUnitPartyMember 
					= ( NULL != g_pX2Game ? g_pX2Game->GetUserUnitByUID( kPartyUserInfo.m_iUnitUID ) : NULL );

				pGageManager->InsertPartyMemberUI( kPartyUserInfo, pGameUnitPartyMember );
				pGageManager->UpdatePartyMemberGageData( kPartyUserInfo.m_iUnitUID, kPartyUserInfo.m_kGamePlayStatus );
			}
		}		
		//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		if ( false == GetMyPartyData()->m_bHaveTooniLandEventTitle 
			&& TOONILAND_CHANNELING_EVENT_TITLE == kPartyUserInfo.m_iEquippedTitleID )
		{
			GetMyPartyData()->m_bHaveTooniLandEventTitle = true;

			if ( NULL != g_pTFieldGame )
				g_pTFieldGame->OpenBuffName( false, 124 );	// 나중에 enum으로 바꿔야 할 듯

			if ( NULL != g_pData->GetPremiumBuffManager() )
				g_pData->GetPremiumBuffManager()->SetTooniLandChannelingEvent( true );
		}
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

	}

	GetMyPartyData()->UpdatePartyLeaderUID();
	g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START);
	g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START);

	return true;
}
bool CX2PartyManager::Handler_EGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*) lParam;
	KEGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_mapAutoPartyDungeonBonusInfo.clear();
	m_mapAutoPartyDungeonBonusInfo = kEvent.m_mapDungeonBonusList;

	if( NULL != g_pMain && NULL != g_pMain->GetPartyUI() && NULL != g_pMain->GetPartyUI()->GetLocalMapUI() )
	{//영웅모집공고 버튼 갱신
		g_pMain->GetPartyUI()->UpdateDungeonRequestButton();
		g_pMain->GetPartyUI()->GetLocalMapUI()->UpdateHeroRecruitButton();
	}

	return true;
}

void CX2PartyManager::Send_EGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT()
{
	KEGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT kPacket;

	//현재 선택된 로컬맵의 탭
	if( NULL != g_pMain && NULL != g_pMain->GetPartyUI() && NULL != g_pMain->GetPartyUI()->GetLocalMapUI() )
	{
		if( true == g_pMain->GetPartyUI()->GetShowLocalMap() )
		{
			kPacket.m_iVillageMapID	= g_pMain->GetPartyUI()->GetLocalMapUI()->GetPickedLocalMapID();
		}
		else
		{
			kPacket.m_iVillageMapID = CX2LocationManager::LMI_INVALID;
		}
	}

	//현재 선택된 던전 정보
	KDungeonGameSimpleInfo kSimpleInfo;
	kSimpleInfo.m_iDungeonID = m_iCurrentDungeonIDWithDifficulty;
	kPacket.m_vecDungeonList.push_back(kSimpleInfo);

	g_pData->GetServerProtocol()->SendPacket( EGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT, kPacket );
}

void CX2PartyManager::UpdateSelectedDungeon()
{
	if( NULL !=g_pData && NULL != g_pData->GetPartyManager() && NULL != g_pData->GetPartyManager()->GetMyPartyData() )
	{
		int iDungeonID = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID;
		int iDungeonDifficulty = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty;
		m_iCurrentDungeonIDWithDifficulty = iDungeonID + iDungeonDifficulty;
	}
}

std::map<wstring, bool> CX2PartyManager::GetHeroRecruitDungeonList()
{
	std::map<wstring, bool> mapDungeonName;

	if( true == m_mapAutoPartyDungeonBonusInfo.empty() )
		return mapDungeonName;

	map<int, KAutoPartyDungeonBonusInfo>::iterator it = m_mapAutoPartyDungeonBonusInfo.begin();
	for( ; it != m_mapAutoPartyDungeonBonusInfo.end(); it++)
	{
		if( NULL != g_pData && NULL != g_pData->GetDungeonManager() )
		{
			const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( static_cast<SEnum::DUNGEON_ID>((it->second.m_iDungeonID) ));

			if( NULL != pDungeonData )
			{
				if( true == g_pData->GetDungeonManager()->IsActiveDungeon( it->second.m_iDungeonID , 0 ) && //현재 활성화된 던전일 경우
					true == it->second.m_bActivatedBonus) //영웅모집 주화 획득 가능한 던전일경우 insert.
				{
					mapDungeonName.insert( pair<wstring,bool>( pDungeonData->m_DungeonName , it->second.m_bActivatedBonus));
				}
			}
		}
	}

	return mapDungeonName;
}

bool CX2PartyManager::IsHeroRecruitDungeon(int iDungeoID )
{
	map<int, KAutoPartyDungeonBonusInfo>::iterator it = m_mapAutoPartyDungeonBonusInfo.find(iDungeoID);
	
	//맵에 해당 던전ID에 대한 정보가 없다면 무조건 false 리턴
	if (it == m_mapAutoPartyDungeonBonusInfo.end() )
		return false;

	return it->second.m_bActivatedBonus;
}

//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
HRESULT CX2PartyManager::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	NetworkProcess( fElapsedTime );

	return S_OK;
}

void CX2PartyManager::NetworkProcess( IN const float fElapsedTime )
{
	KTDXPROFILE();

//#ifndef X2OPTIMIZE_PARTY_UDP_CONNECTION_BUG_FIX
//
//	if( true == m_MyPartyData.m_vecPartyMemberData.empty() )
//		return;
//
//	//방 유저들을 피어로 유지시킨다.
//	bool bAddNewPeer = false;
//	BOOST_TEST_FOREACH( const PartyMemberData&, sPartyMemberData, m_MyPartyData.m_vecPartyMemberData )
//	{
//		//슬롯에 있는 유저를 피어에 등록한다
//		const CKTDNUDP::Peer* pPeer = g_pData->GetGameUDP()->GetPeer( sPartyMemberData.m_iUnitUID );
//		if( pPeer == NULL )
//		{
//			bAddNewPeer = true;
//			g_pData->GetGameUDP()->AddPeer( sPartyMemberData.m_iUnitUID, 
////#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				CKTDNUDP::ConvertIPToAddress( sPartyMemberData.m_IP.c_str() ),
////#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////				sPartyMemberData.m_IP.c_str(), 
////#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				sPartyMemberData.m_Port, 
////#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				CKTDNUDP::ConvertIPToAddress( sPartyMemberData.m_InternalIP.c_str() ),
////#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////				sPartyMemberData.m_InternalIP.c_str(), 
////#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				sPartyMemberData.m_InternalPort );
//		}
//	}
//
//	//파티가 없으면서 룸이 있는 경우(대전)도 있기 때문에 아래 부분 주석. 안 그러면 매번 udp 연결을 지우게 되는 문제가 생긴다. by 박진웅
//	//g_pData->GetGameUDP()->RemoveOtherPeer( slotUIDList );
//
//	if( bAddNewPeer == true )
//	{
//		g_pData->GetGameUDP()->ConnectTestToPeer();
//	}
//
//#endif  X2OPTIMIZE_PARTY_UDP_CONNECTION_BUG_FIX
}
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//}}

#ifdef NEW_MAIL_LOG
/** @function : AddClosePartyLog
	@brief : 파티 해제가 정상적이지 않은 경우에 대한 확인을 위해 남기는 로그
*/
void CX2PartyManager::AddClosePartyLog( const string& strPacketName_, int iReason_ )
{
	if( NULL == g_pX2Game || CX2Game::GT_DUNGEON != g_pX2Game->GetGameType() )
		return;

	if( true == g_pX2Game->GetGameEnd() )
		return;

	if( NULL == GetMyPartyData() )
		return;

	int iUserUnitNum = g_pX2Game->GetUserUnitNum();

	// 현재 인원 수가 1명이하라면 체크 할 필요 없음.
	if( 1 < iUserUnitNum )
	{
		int iPartyUnitNum = static_cast<int>(GetMyPartyData()->m_vecPartyMemberData.size());

		if( iUserUnitNum > iPartyUnitNum )
		{// 게임 내 유저 수가 파티원 수 보다 많다면 오류!! 로그 전송

			// 패킷 명
			{
				CTime NowTime = CTime::GetCurrentTime();
				char Buf1[256];
				StringCchPrintfA( Buf1, 256, "%d:%d:%d, %s", NowTime.GetHour(), NowTime.GetMinute(), NowTime.GetSecond(), strPacketName_.c_str() );

#ifdef LOG_PARTY_BREAK
				// 현재 로그를 통해 알 수 있는 정보가 없어서 주석 처리 함
				// 문제를 찾기 위한 코드 분석 새로 해야 함.
				//CX2MailLogManager::GetInstance()->AddMailLog( CX2MailLogManager::MLI_CLOSE_PARTY_BUG, Buf1 );
#endif // LOG_PARTY_BREAK
			}
			
			// 인원 정보
			{
				char Buf1[256];
				StringCchPrintfA( Buf1, 256, "%d(유저 인원:%d, 파티 인원:%d, 현재 스테이트 : %d)\n", iReason_, iUserUnitNum, iPartyUnitNum, g_pMain->GetNowStateID() );
#ifdef LOG_PARTY_BREAK
				// 현재 로그를 통해 알 수 있는 정보가 없어서 주석 처리 함
				// 문제를 찾기 위한 코드 분석 새로 해야 함.
				//CX2MailLogManager::GetInstance()->AddMailLog( CX2MailLogManager::MLI_CLOSE_PARTY_BUG, Buf1 );
#endif // LOG_PARTY_BREAK
			}
		}
	}
}
#endif // NEW_MAIL_LOG

#ifdef SERV_NEW_EVENT_TYPES
bool CX2PartyManager::Handler_EGS_UPDATE_DUNGEON_STATUS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_DUNGEON_STATUS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pData->GetDungeonManager() != NULL )
	{
		g_pData->GetDungeonManager()->SetDungeonStatus( kEvent.m_mapEnableDungeon );

		if( g_pMain->GetPartyUI() != NULL )
		{
			g_pMain->GetPartyUI()->ClearLocalMapDLGs();
			g_pMain->GetPartyUI()->UpdatePartyDLG();
		}
	}

	return true;
}
#endif //SERV_NEW_EVENT_TYPES