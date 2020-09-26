#include "stdafx.h"

#include "StdAfx.h"
#include ".\X2PartyUI.h"

const double CX2PartyUI::PARTY_LIST_REFRESH_INTERVAL = 5.0;		// 파티 리스트 갱신 시간간격
 

CX2PartyUI::CX2PartyUI( CKTDXStage* pNowStage ) :
m_pCurrStage( NULL ), 
m_pDLGParty( NULL ),
//m_pDLGPartyMember( NULL ),
m_pDLGPartyListMeberPreview( NULL ),
m_pDLGWarningAllReady(NULL),
m_pDLGPartyFever( NULL ),
m_ePartyListLocalMapID( CX2LocationManager::LMI_INVALID ),
m_iPartyListDungeonID( 0 ),
m_iPartyListPageNumber( 1 ),
m_iMaxPartyListPageNumber( 1 ),
m_iPickedPartyListSlotIndex( -1 ),
m_pLocalMapUI( new CX2LocalMapUI ),
m_iNoviceGuideStep(0),
m_fElapsedTime(3.f)
#ifdef SERV_PVP_NEW_SYSTEM
, m_SumDelta(0)
, m_bPartyDungeon(true)
, m_bPartyPVP(false)
, m_pDLGPartyMenu(NULL)
, m_pDLGPartyPVP(NULL)
, m_pDLGPartyPVPSetting(NULL)
, m_pDLGPartyPVPSettingButton(NULL)
, m_pDlgPartyPvpMapSetting(NULL)
//, m_pDlgPartyPvpEtcSetting(NULL)
, m_pDlgPvpConfirm(NULL)
, m_fWaitPvpTime(0.f)
, m_iEstimatedTime(0)
#ifdef SERV_EVENT_VALENTINE_DUNGEON		/// 발렌타인 던전 안내 이펙트
, m_hValentineHeart1( INVALID_PARTICLE_SEQUENCE_HANDLE )
, m_hValentineHeart2( INVALID_PARTICLE_SEQUENCE_HANDLE )
#endif SERV_EVENT_VALENTINE_DUNGEON
#endif

#ifdef FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN
, m_lParam (0)
#endif // FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN

#ifdef NEW_DEFENCE_DUNGEON
//어둠의 문 난이도가 변경 되었을 때, EGS_PARTY_CHANGE_DUNGEON_REQ 를 호출하도록 주는 신호
, m_bChangePartyUI(false) 
#endif NEW_DEFENCE_DUNGEON
, m_pDLGQuestRewardItemInfo(NULL)

#ifdef 	FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
, m_pDLGQuestRewardItemInfoAdd(NULL)
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION


, CX2ItemSlotManager( pNowStage, NULL)
, m_bShowQuestMark(true)
, m_pDungeonCheckBox(NULL)
, m_pPVPCheckBox(NULL)
, m_eDungeonButton(BUTTON_START)
, m_ePVPButton(BUTTON_START)
{
#ifdef SERV_PVP_NEW_SYSTEM
	m_vecDefaultMap.clear();
	InitPvpSetting();
#endif
}

/*virtual*/ CX2PartyUI::~CX2PartyUI(void)
{
#ifdef SERV_PVP_NEW_SYSTEM	
	SAFE_DELETE_DIALOG( m_pDLGPartyPVP );
	SAFE_DELETE_DIALOG( m_pDlgPartyPvpMapSetting );
	//SAFE_DELETE_DIALOG( m_pDlgPartyPvpEtcSetting );
	SAFE_DELETE_DIALOG( m_pDLGPartyPVPSetting );
	SAFE_DELETE_DIALOG( m_pDLGPartyPVPSettingButton );
	SAFE_DELETE_DIALOG( m_pDLGPartyMenu );
	SAFE_DELETE_DIALOG( m_pDlgPvpConfirm );
#endif
	SAFE_DELETE_DIALOG( m_pDLGQuestRewardItemInfo );

#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION	

	SAFE_DELETE_DIALOG( m_pDLGQuestRewardItemInfoAdd );

#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

	SAFE_DELETE_DIALOG( m_pDLGParty );
	//SAFE_DELETE_DIALOG( m_pDLGPartyMember );
	SAFE_DELETE_DIALOG( m_pDLGPartyListMeberPreview );
	SAFE_DELETE_DIALOG( m_pDLGWarningAllReady );
	SAFE_DELETE_DIALOG( m_pDLGPartyFever );

	SAFE_DELETE( m_pLocalMapUI );

#ifdef SERV_EVENT_VALENTINE_DUNGEON		/// 발렌타인 던전 안내 이펙트 해제
	if( NULL != g_pData->GetUIMajorParticle() )		
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hValentineHeart1 );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hValentineHeart2 );
	}
#endif SERV_EVENT_VALENTINE_DUNGEON
}

/*virtual*/ HRESULT CX2PartyUI::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fElapsedTime += fElapsedTime;
	if(m_fElapsedTime >= 3.f)
		m_fElapsedTime = 3.f;

	if( NULL != m_pLocalMapUI )
	{
		m_pLocalMapUI->OnFrameMove( fTime, fElapsedTime );
	}
	ShortCutKeyProcess();

	UpdateMatchingUI(fElapsedTime);

#ifdef REFORM_NOVICE_GUIDE
	UpdateNoviceGuide();
#endif //REFORM_NOVICE_GUIDE

#ifdef QUEST_GUIDE
	UpdateQuestGuide();
#endif //QUEST_GUIDE

	return S_OK;
}

/*virtual*/ bool CX2PartyUI::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	const float fPartyChangeTime = 0.5f;

	if( NULL != m_pLocalMapUI )
	{
		if( true == m_pLocalMapUI->UICustomEventProc(hWnd, uMsg, wParam, lParam) )
			return true;
	}


	switch(wParam)
	{
	case PUCM_PARTY_NAME:
		{
			CKTDGUIIMEEditBox*	pEditBox = (CKTDGUIIMEEditBox*) lParam;
			if( NULL != pEditBox->GetDialog() )
				pEditBox->GetDialog()->ClearFocus();
			
			
			wstring partyName = g_pMain->GetStringFilter()->FilteringChatString( pEditBox->GetText(), L'♡' );

			if( true == partyName.empty() )
			{
				pEditBox->SetText( g_pData->GetPartyManager()->GetMyPartyData()->m_wstrPartyTitle.c_str() );
				return true;
			}

			if( partyName.length() > 20 )
			{
				partyName = partyName.substr( 0, 20 );
			}

			if(g_pData->GetPartyManager()->DoIHaveParty() == true && m_fElapsedTime <= fPartyChangeTime)
				return true;
			m_fElapsedTime = 0.f;
			
			if( true == g_pData->GetPartyManager()->DoIHaveParty() )
			{
				if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
				{
					if( 0 != g_pData->GetPartyManager()->GetMyPartyData()->m_wstrPartyTitle.compare( partyName ) )
					{
						ASSERT( !L"EGS_PARTY_CHANGE_NAME_REQ" );
					}
				}
			}
			else
			{
				g_pData->GetPartyManager()->GetMyPartyData()->m_wstrPartyTitle = partyName;
				UpdatePartyDLG();
			}

			return true;
		} break;

	case PUCM_PARTY_QUICK_PARTY:
		{
			// 게임내에서 바쁜 상태인 경우 던전을 시작 할 수 없음
			if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
				return true;

			if( false == static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() )
				return true;

			// 빠른파티
			if( m_fElapsedTime <= fPartyChangeTime )
				return true;
			m_fElapsedTime = 0.f;

#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
			if(g_pData != NULL && g_pData->GetPartyManager() != NULL)
			{
				g_pData->GetPartyManager()->Handler_EGS_AUTO_PARTY_DUNGEON_GAME_REQ();
			}
#else
			if(g_pData != NULL && g_pData->GetPartyManager() != NULL)
				g_pData->GetPartyManager()->Handler_EGS_PARTY_QUICK_JOIN_REQ( *g_pData->GetPartyManager()->GetMyPartyData() );
#endif SERV_DUNGEON_DIFFICULTY_REVISION


#ifdef REFORM_NOVICE_GUIDE
			if( NULL != g_pData && NULL != g_pData->GetPlayGuide() 
				&& true == g_pData->GetPlayGuide()->GetActiveNoviceGuide() )
			{
				switch( g_pData->GetPlayGuide()->GetGuideStep() )
				{
				case CX2PlayGuide::NGS_STEP_6:
				case CX2PlayGuide::NGS_STEP_7:
				case CX2PlayGuide::NGS_STEP_8:
					{
						if( NULL != m_pDLGParty )
						{
							m_pDLGParty->SetShow(false);
						}
						g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_END );
					} break;
				}
			}
#endif //REFORM_NOVICE_GUIDE

			return true;

		} break;
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	case PUCM_CREATE_LEVEL_UNLIMIT_PARTY_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
#ifdef SERV_PVP_NEW_SYSTEM
			g_pData->GetPartyManager()->GetMyPartyData()->m_bPvpParty = false;
#endif
			CX2PartyManager::PartyData partydata = *g_pData->GetPartyManager()->GetMyPartyData();
			partydata.m_bIsLimitLevel = false;
			ASSERT( !L"EGS_CREATE_PARTY_REQ" );
			return true;
		} break;
	case PUCM_CREATE_LEVEL_UNLIMIT_PARTY_CANCEL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			return true;
		} break;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	case PUCM_DUNGEON_PARTY_MATCH_CANCEL:
		{
			g_pData->GetPartyManager()->Handler_EGS_CANCEL_AUTO_PARTY_MAKING_REQ();			
		} break;

	case PUCM_PARTY_VIEW_PARTY_LIST:
		{
			ASSERT( !L"PUCM_PARTY_VIEW_PARTY_LIST" );
		} break;

	case PUCM_PARTY_GAME_START:
		{
			GameStartCurrentMember();
			return true;
		} break;

	case PUCM_PARTY_CLOSE_WINDOW:
		{
#ifdef SERV_PVP_NEW_SYSTEM
			if( g_pData->GetPartyManager()->GetProcessPvpMatch() == true )
				return false;
#endif
			OpenPartyDLG( false );
			return true;
		} break;

	case PUCM_PARTY_PUBLIC:
		{
			CKTDGUICheckBox* pCheck_NonPublic = (CKTDGUICheckBox*) lParam;
			if( NULL == pCheck_NonPublic )
				return false;

			if(g_pData->GetPartyManager()->DoIHaveParty() == true && m_fElapsedTime <= fPartyChangeTime)
			{
				pCheck_NonPublic->SetChecked(!pCheck_NonPublic->GetChecked());
				return true;
			}

			m_fElapsedTime = 0.f;

			if( true == g_pData->GetPartyManager()->DoIHaveParty() )
			{
				if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
				{
					ASSERT( !L"EGS_PARTY_CHANGE_PUBLIC_REQ" );
				}
			}
			else
			{
				g_pData->GetPartyManager()->GetMyPartyData()->m_bPublic = !pCheck_NonPublic->GetChecked();
				UpdatePartyDLG();
			}
			return true;
		} break;
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	case PUCM_PARTY_LEVEL_LIMIT:
		{
			CKTDGUICheckBox* pCheck_LevelLimit = (CKTDGUICheckBox*) lParam;
			if( NULL == pCheck_LevelLimit )
				return false;

			if(g_pData->GetPartyManager()->DoIHaveParty() == true && m_fElapsedTime <= fPartyChangeTime)
			{
				pCheck_LevelLimit->SetChecked(!pCheck_LevelLimit->GetChecked());
				return true;
			}

			m_fElapsedTime = 0.f;

			if( true == g_pData->GetPartyManager()->DoIHaveParty() )
			{
				if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
				{
// 					if(pCheck_LevelLimit->GetChecked())
// 					{
// 						// fix : 켜는 경우에 따로 처리해줘야 할 일을 여기서 처리
// 
// 
// 					}

					g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ( pCheck_LevelLimit->GetChecked() );
				}
			}
			else
			{
				g_pData->GetPartyManager()->GetMyPartyData()->m_bIsLimitLevel = pCheck_LevelLimit->GetChecked();
				UpdatePartyDLG();
			}
			return true;
		} break;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	case PUCM_PARTY_LEAVE_PARTY:
		{
			if(g_pData->GetPartyManager()->DoIHaveParty() == true && m_fElapsedTime <= fPartyChangeTime)
				return true;
			
			m_fElapsedTime = 0.f;

			if( false == g_pInstanceData->GetIsDungeonLounge() )
			{
				g_pData->GetPartyManager()->Handler_EGS_LEAVE_PARTY_REQ( (int) NetError::NOT_LEAVE_ROOM_REASON_00 );	
			}
			else 
			{
				if( true == g_pData->GetPartyManager()->DoIHaveParty() &&
					false == g_pData->GetPartyManager()->AmIPartyLeader() ) 
				{
					CX2StateField* pStateField = (CX2StateField*) g_pMain->GetNowState();
					pStateField->Handler_EGS_LEAVE_ROOM_REQ();
				}
			}

#ifndef REFORM_NOVICE_GUIDE
			// 초심자 가이드
			if( g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL )
			{
				if( g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true &&
					g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP6 )
				{
					g_pTFieldGame->GetNoviceGuide()->InitPartyGuide(false);
					//g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_COMPLETE);
				}
			}
#endif //REFORM_NOVICE_GUIDE
			return true;
		} break;


	case PUCM_PARTY_OPEN_LOCAL_MAP:
		{
			if(g_pData->GetPartyManager()->DoIHaveParty() == true && m_fElapsedTime <= fPartyChangeTime)
				return true;
			m_fElapsedTime = 0.f;

			if( NULL != m_pLocalMapUI )
			{
				//CX2LocationManager::LOCAL_MAP_ID eLocalMapID = 
				//	g_pData->GetLocationManager()->GetLocalMapID( (SEnum::DUNGEON_ID) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID );


				CX2LocationManager::LOCAL_MAP_ID eLocalMapID = 
					( CX2LocationManager::LMI_INVALID != m_pLocalMapUI->GetPickedLocalMapID() ? 
					m_pLocalMapUI->GetPickedLocalMapID() : g_pData->GetLocationManager()->GetLocalMapID( g_pData->GetLocationManager()->GetCurrentVillageID() ) );
				
				if( CX2LocationManager::LMI_INVALID == eLocalMapID )
					eLocalMapID = CX2LocationManager::LMI_RUBEN;

				m_pLocalMapUI->OpenLocalMapDLG( true, eLocalMapID );
				m_pLocalMapUI->UpdateLocalMapDLG();

#ifndef REFORM_NOVICE_GUIDE
				// 초심자 가이드
				if( g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL && g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true )
				{
					if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP6_1 )
					{						
						g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_COMPLETE);
					}
					else if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP6_3 )
					{
						g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_ACCEPT);
					}					
				}
#endif //REFORM_NOVICE_GUIDE
			}

			return true;
		} break;

	case PUCM_PARTY_LEAVE_DUNGEON_LOUNGE:
		{
			if( g_pData->GetPartyManager()->DoIHaveParty() == true && m_fElapsedTime <= fPartyChangeTime )
				return true;

			m_fElapsedTime = 0.f;

			if( true == g_pInstanceData->GetIsDungeonLounge() )
			{
// 				if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
// 				{
// 					ASSERT( !L"EGS_PARTY_TO_FIELD_MAP_REQ" );
// 				}
// 				else
				{
					CX2StateField* pStateField = (CX2StateField*) g_pMain->GetNowState();
					pStateField->Handler_EGS_LEAVE_ROOM_REQ();
				}
			}

			return true;
		} break;

	case PUCM_PARTY_ITEM_GET_RANDOM:
		{
			if( true == g_pData->GetPartyManager()->DoIHaveParty() )
			{
				if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
				{
					g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ( CX2Room::DGIT_RANDOM );
				}
			}
			else
			{
				g_pData->GetPartyManager()->GetMyPartyData()->m_eItemLootingMode = CX2Room::DGIT_RANDOM;
				UpdatePartyDLG();
			}
			
			return true;
		} break;

	case PUCM_PARTY_ITEM_GET_EACH:
		{
			if( true == g_pData->GetPartyManager()->DoIHaveParty() )
			{
				if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
				{
					g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ( CX2Room::DGIT_PERSON );
				}
			}
			else
			{
				g_pData->GetPartyManager()->GetMyPartyData()->m_eItemLootingMode = CX2Room::DGIT_PERSON;
				UpdatePartyDLG();
			}

			return true;
		} break;

	case PUCM_PARTY_LEADER_ACCEPT_JOIN_REQUEST:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			CX2Main::TimedMessagePopUp* pTimedPopup = g_pMain->GetTimedMessagePopup( pButton->GetDialog() );

			if( NULL != pTimedPopup )
			{
				pTimedPopup->m_fTimeLeft = 0.f;

				ASSERT( !L"EGS_JOIN_PARTY_INVITE_REPLY_NOT_accept" );
			}

			return true;
		} break;

	case PUCM_PARTY_LEADER_REJECT_JOIN_REQUEST:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			CX2Main::TimedMessagePopUp* pTimedPopup = g_pMain->GetTimedMessagePopup( pButton->GetDialog() );
			
			if( NULL != pTimedPopup )
			{
				pTimedPopup->m_fTimeLeft = 0.f;

				ASSERT( !L"EGS_JOIN_PARTY_INVITE_REPLY_NOT_reject" );

			}

			return true;
		} break;


	case PUCM_ACCEPT_INVITE_REQUEST:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( NULL == pButton )
				return true;

			CX2Main::TimedMessagePopUp* pTimedPopup = g_pMain->GetTimedMessagePopup( pButton->GetDialog() );

			if( NULL != pTimedPopup )
			{
				pTimedPopup->m_fTimeLeft = 0.f;
				//{{ 2010. 03. 10  최육사	채널 통합 파티 - PartyUID는 UidType임.
#ifdef SERV_INVITE_PARTY_REPLY_REASON 
				g_pData->GetPartyManager()->Send_EGS_INVITE_PARTY_REPLY_NOT( KEGS_INVITE_PARTY_REPLY_NOT::RR_ACCEPT, pTimedPopup->m_UserData.iOpponentUID );
#else
				g_pData->GetPartyManager()->Send_EGS_INVITE_PARTY_REPLY_NOT( true, pTimedPopup->m_UserData.iOpponentUID );
#endif SERV_INVITE_PARTY_REPLY_REASON
				//}}
			}

			return true;
		} break;

	case PUCM_REJECT_INVITE_REQUEST:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( NULL == pButton )
				return true;

			CX2Main::TimedMessagePopUp* pTimedPopup = g_pMain->GetTimedMessagePopup( pButton->GetDialog() );

			if( NULL != pTimedPopup )
			{
				pTimedPopup->m_fTimeLeft = 0.f;
				//{{ 2010. 03. 10  최육사	채널 통합 파티 - PartyUID는 UidType임.
#ifdef SERV_INVITE_PARTY_REPLY_REASON
				g_pData->GetPartyManager()->Send_EGS_INVITE_PARTY_REPLY_NOT( KEGS_INVITE_PARTY_REPLY_NOT::RR_NOT_ACCEPT, pTimedPopup->m_UserData.iOpponentUID );
#else
				g_pData->GetPartyManager()->Send_EGS_INVITE_PARTY_REPLY_NOT( false, pTimedPopup->m_UserData.iOpponentUID );
#endif SERV_INVITE_PARTY_REPLY_REASON
				//}}
			}

			return true;
		} break;

	case PUCM_PARTY_MEMBER_CLICKED:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			
			if( NULL != g_pData->GetUserMenu() )
			{
				CX2PartyManager::PartyMemberData* pMemberData =
					g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( pButton->GetDummyInt(0), g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
				if( NULL != pMemberData )
				{
					CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
					bool bIsParty = false;

					if ( NULL != pPartyManager && true == pPartyManager->DoIHaveParty() )
						bIsParty = true;

					g_pData->GetUserMenu()->OpenUserPopupMenu( pMemberData->m_iUnitUID, bIsParty );
				}
			}

			return true;
		} break;


	case PUCM_PARTY_PLAY_SOLO:
		{
#ifdef SERV_PVP_NEW_SYSTEM
			g_pData->GetPartyManager()->GetMyPartyData()->m_bPvpParty = false;
#endif

			CX2PartyManager::PartyData partydata = *g_pData->GetPartyManager()->GetMyPartyData();
			partydata.m_iMaxPartyMemberCount = 1;
			partydata.m_bPublic = false;
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
			partydata.m_bIsLimitLevel = false,
#endif SERV_DUNGEON_DIFFICULTY_REVISION

			ASSERT( !L"EGS_CREATE_PARTY_REQ" );

#ifndef REFORM_NOVICE_GUIDE
			// 초심자 가이드
			if(g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL)
			{
				if( g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true &&
					g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP5 )
				{
					g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_COMPLETE);
				}
			}
#endif //REFORM_NOVICE_GUIDE
			return true;
		} break;

#ifdef SERV_PVP_NEW_SYSTEM
	case PUCM_SELECT_DUNGEON_MENU:
		{
			if( false == static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() )
				return true;

			bool bChecked = false;

			if( NULL != m_pDungeonCheckBox)
			{
				bChecked = m_pDungeonCheckBox->GetChecked();
			}

			// 토글시 재활성
 			if( GetShow() == true && bChecked == false )
 			{
 				if( NULL != m_pDungeonCheckBox &&
					g_pData->GetPartyManager()->DoIHaveParty() ||
					g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true )
 				{
 					m_pDungeonCheckBox->SetChecked(true);
 					return true;
 				}
 			}

			if( g_pData->GetPartyManager()->GetProcessPvpMatch() == true &&
				NULL != m_pPVPCheckBox &&
				NULL != m_pDungeonCheckBox )
			{
				// 매칭중에는 던전파티변경 불가
				m_pDungeonCheckBox->SetChecked(m_bPartyDungeon);
				m_pPVPCheckBox->SetChecked(m_bPartyPVP);
				return true;
			}

			if( m_pPVPCheckBox != NULL )
				m_pPVPCheckBox->SetChecked( false );
			
			m_bPartyDungeon = false;
			m_bPartyPVP = false;

			if( bChecked == true )
			{
				m_bPartyDungeon = true;
			}

			if( g_pData->GetPartyManager()->DoIHaveParty() == true && g_pData->GetPartyManager()->GetMyPartyData()->m_bPvpParty == true )
			{
				// 던전파티로 변경
				if( g_pData->GetPartyManager()->ChangePartyType(false) == false )
				{
					// 던전파티 변경불가
				}
			}

			OpenPartyDLG( bChecked );
			UpdatePartyDLG();
			return true;
		}
		break;

	case PUCM_SELECT_PVP_MENU:
		{
			if( false == static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() )
				return true;

			bool bChecked = false;
			if( m_pPVPCheckBox != NULL )
				bChecked = m_pPVPCheckBox->GetChecked();

			// 토글시 재활성
 			if( GetShow() == true && bChecked == false )
 			{
 				if( NULL != m_pPVPCheckBox &&
					g_pData->GetPartyManager()->DoIHaveParty() ||
					g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true )
 				{
 					m_pPVPCheckBox->SetChecked(true);
 					return true;
 				}
 			}

			if( m_pDungeonCheckBox != NULL )
				m_pDungeonCheckBox->SetChecked( false );

			m_bPartyDungeon = false;
			m_bPartyPVP = false;

			if( bChecked == true )
				m_bPartyPVP = true;

			if( g_pData->GetPartyManager()->DoIHaveParty() == true && g_pData->GetPartyManager()->GetMyPartyData()->m_bPvpParty == false )
			{
				if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() >= 4 &&
					m_pDungeonCheckBox != NULL &&
					m_pDungeonCheckBox != NULL )
				{
					m_pDungeonCheckBox->SetChecked( true );
					m_pPVPCheckBox->SetChecked( false );

					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12715 ), m_pCurrStage );					
					return true;
				}
				for(int iParty=0; iParty<g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount(); ++iParty)
				{
					if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iParty ) != NULL &&
						g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iParty )->m_iUnitLevel < 10 )
					{

						if( m_pDungeonCheckBox != NULL )
							m_pDungeonCheckBox->SetChecked(true );
						if( m_pPVPCheckBox != NULL )
							m_pPVPCheckBox->SetChecked( false );

						// 10렙 미만인 파티원이 있어 대전파티로 변경이 불가합니다.
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_13548 ), (CKTDXStage*)g_pMain->GetNowState() );					
						return true;
					}
				}

				// 대전파티로 변경
				if( g_pData->GetPartyManager()->ChangePartyType(true) == false )
				{
					// 대전파티 변경불가
				}
			}

			OpenPartyDLG( bChecked );
			return true;
		}
		break;
	case PUCM_PVP_SETTING:
		{
			OpenPvpSetting( true );
			return true;
		}
		break;
	case PUCM_PVP_SETTING_CLOSE:
		{
			ResetePvpMapSetting();
			OpenPvpSetting( false );
			return true;
		}
		break;
	case PUCM_PVP_MAP_SETTING:
		{
			OpenPvpSetting( true, true );
			return true;
		}
		break;
	case PUCM_PVP_ETC_SETTING:
		{
			OpenPvpSetting( true, false );
			return true;
		}
		break;
	case PUCM_PVP_SETTING_SAVE_CLOSE:
		{
			// 저장후 종료
			SavePvpMapSetting();
			OpenPvpSetting( false );
			return true;
		}
		break;
	case PUCM_PVP_MAP_SELECT:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( pButton != NULL )
			{
				int iMapIndex = pButton->GetDummyInt(0);
				UpdateMapInfo(iMapIndex);
			}
			return true;
		}
		break;
	case PUCM_PVP_MAP_CHECK:
		{
			if( m_pDlgPartyPvpMapSetting != NULL && m_pDlgPartyPvpMapSetting->GetShow() == true )
			{
				CKTDGUICheckBox* pChecked = (CKTDGUICheckBox*) lParam;
				int mapIndex = pChecked->GetDummyInt(0);

				int iCheckedMap = 0;
				for(int i=0; i<(int)m_vecPvpMapInfo.size(); ++i)
				{
					WCHAR wMapId[256] = {0,};
					StringCchPrintf(wMapId, 256, L"Map_CheckBox_%d", i);
					CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDlgPartyPvpMapSetting->GetControl( wMapId );
					if( pCheckBox->GetChecked() == true )
					{
						++iCheckedMap;
					}
				}

				if( iCheckedMap > 2 )
				{
					// 맵설정 취소 알림			
					pChecked->SetChecked( false );
					--iCheckedMap;
					
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12717 ), m_pCurrStage );

				}
				else
				{
					if( pChecked->GetChecked() == true )
					{
						// 맵 설정 알림
						wstring wstrMapSet = GET_REPLACED_STRING( ( STR_ID_12716, "L", m_vecPvpMapInfo[mapIndex].m_MapName ) );
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrMapSet.c_str(), m_pCurrStage );
					}					
				}

				CKTDGUIStatic *pViewImgStatic = (CKTDGUIStatic*)m_pDlgPartyPvpMapSetting->GetControl(L"SETTING_MAP");
				pViewImgStatic->GetString(1)->msg = GET_REPLACED_STRING( ( STR_ID_12353, "i", 2 - iCheckedMap ) );
			}	
			return true;
		}
		break;
		
	case PUCM_PVP_SETTING_1:
		{				
			//g_pData->GetPartyManager()->Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ( GetPvpMatchMode() );
			ShowPvpModeDesc();
			return true;
		}
		break;
	case PUCM_PVP_CREATE_PARTY:
		{
			if(g_pData->GetPartyManager()->DoIHaveParty() == true && m_fElapsedTime <= fPartyChangeTime)
				return true;
			m_fElapsedTime = 0.f;

			if(g_pData != NULL && g_pData->GetPartyManager() != NULL)
			{
				g_pData->GetPartyManager()->GetMyPartyData()->m_bPvpParty = true;
				g_pData->GetPartyManager()->GetMyPartyData()->m_pvpMode = 0;

				ASSERT( !L"EGS_CREATE_PARTY_REQ" );
			}

			return true;
		}
		break;
	case PUCM_PVP_START_MATCH:
		{
#ifdef FIX_JOIN_OFFICIAL_PVP_ROOM // 김태환
			/// 현재 연습 대전 대기실 혹은 방 이라면, 공식 대전 신청 못함
			if ( NULL != g_pMain && 
				 ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_LOBBY ||
				   g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM ) )
				return true;
#endif //FIX_JOIN_OFFICIAL_PVP_ROOM

			// 게임내에서 바쁜 상태인 경우 던전을 시작 할 수 없음
			if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
				return true;

			//10레벨 이하의 유저는 버튼을 볼 수 없기 때문에 예외 처리는 파티원에 대해서만


			if( true == g_pData->GetPartyManager()->DoIHaveParty() && false == g_pData->GetPartyManager()->AmIPartyLeader() )
			{
				return false;
			}


			if( g_pData->GetPartyManager()->DoIHaveParty() == true )
			{
				for(int iParty=0; iParty<g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount(); ++iParty)
				{
					if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iParty ) != NULL &&
						g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iParty )->m_iUnitLevel < 10 )
					{						
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_13548 ), (CKTDXStage*)g_pMain->GetNowState() );					
						return true;
					}
				}
			}

			char cPlayNum = GetPvpMatchPlayerNum();
			char cPlayPvpMode = GetPvpMatchMode();

			g_pData->GetPartyManager()->Handler_EGS_REQUEST_MATCH_MAKING_REQ( cPlayNum, cPlayPvpMode );
			StartToWaitForPvpMatching();
			return true;
		}
		break;
	case PUCM_PVP_CANCEL_MATCH:
		{
			CancelToWaitForPvpMatching();
			if( NULL != g_pData && NULL != g_pData->GetPartyManager() )
				g_pData->GetPartyManager()->Handler_EGS_CANCEL_MATCH_MAKING_REQ();
			return true;
		}
		break;
	case PUCM_PVP_CHECK_START_OK:
		{
			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PVP_START, userData );
			g_pData->GetPartyManager()->Handler_EGS_MATCH_MAKING_SUCCESS_REPLY_NOT(true);

			//매칭 확인 이후 단축키 사용 못하도록 변경
			CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
			if ( NULL != pNowState )
				pNowState->SetEnableShortCutKey(false);

			if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SKILL ) == true )
			{
				g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL );
			}

			return true;
		}
		break;
	case PUCM_PVP_CHECK_START_CANCEL:
		{
			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PVP_START, userData );
			g_pData->GetPartyManager()->Handler_EGS_MATCH_MAKING_SUCCESS_REPLY_NOT(false);
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12720 ), m_pCurrStage );
			return true;
		}
		break;	
	case PUCM_PVP_LEAVE_PARTY:
		{
			if( m_pDlgPvpConfirm != NULL )
			{
				SAFE_DELETE_DIALOG( m_pDlgPvpConfirm );
				m_pDlgPvpConfirm = NULL;
			}

			if( g_pData->GetPartyManager()->DoIHaveParty() == true )
			{
				if( g_pData->GetPartyManager()->GetProcessPvpMatch() == true )
				{
					m_pDlgPvpConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12712 ), PUCM_PVP_LEAVE_PARTY_OK, 
						g_pMain->GetNowState(), PUCM_PVP_LEAVE_PARTY_CANCEL );
				}
				else
				{
					m_pDlgPvpConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_666 ), PUCM_PVP_LEAVE_PARTY_OK, 
						g_pMain->GetNowState(), PUCM_PVP_LEAVE_PARTY_CANCEL );					
				}				
			}
			return true;
			
		}
		break;
	case PUCM_PVP_LEAVE_PARTY_OK:
		{
			if( m_pDlgPvpConfirm != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgPvpConfirm, NULL, false );
				m_pDlgPvpConfirm = NULL;
			}

			if( true == g_pData->GetPartyManager()->DoIHaveParty() ) 
			{
				g_pData->GetPartyManager()->Handler_EGS_LEAVE_PARTY_REQ( (int) NetError::NOT_LEAVE_ROOM_REASON_00 );
			}
			return true;
		}
		break;
	case PUCM_PVP_LEAVE_PARTY_CANCEL:
		{
			if( m_pDlgPvpConfirm != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgPvpConfirm, NULL, false );
				m_pDlgPvpConfirm = NULL;
			}			
			return true;
		}
		break;
	case PUCM_PVP_CANCEL_MATCH_OK:
		{
			if( m_pDlgPvpConfirm != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgPvpConfirm, NULL, false );
				m_pDlgPvpConfirm = NULL;
			}

			g_pData->GetPartyManager()->Handler_EGS_CANCEL_MATCH_MAKING_REQ();
			UpdatePvpPartyDlg();
			return true;
		}
		break;
	case PUCM_PVP_CANCEL_MATCH_CANCEL:
		{
			if( m_pDlgPvpConfirm != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgPvpConfirm, NULL, false );
				m_pDlgPvpConfirm = NULL;
			}
			return true;
		}
		break;
#endif

	case PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_OK:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( NULL == pButton )
				return true;

			CX2Main::TimedMessagePopUp* pTimedPopup = g_pMain->GetTimedMessagePopup( pButton->GetDialog() );

			if( NULL != pTimedPopup )
			{
				//취소메세지 받았을 때 해당 다이얼로그 제거
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				userData.iMyUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
				g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_READY_TO_PLAY_DUNGEON_REQUEST, userData );
				g_pData->GetPartyManager()->Send_EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT( true );
			}

			//매칭 확인 이후 단축키 사용 못하도록 변경 
			CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
			if ( NULL != pNowState )
				pNowState->SetEnableShortCutKey(false);

			if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SKILL ) == true )
			{
				g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL );
			}
			return true;
		} break;

	case PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_CANCEL:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( NULL != pButton )
			{
				CX2Main::TimedMessagePopUp* pTimedPopup = g_pMain->GetTimedMessagePopup( pButton->GetDialog() );
				if( NULL != pTimedPopup )
				{
					//취소메세지 받았을 때 해당 다이얼로그 제거
					CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
					userData.iMyUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
					g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_READY_TO_PLAY_DUNGEON_REQUEST, userData );
				}
			}
			g_pData->GetPartyManager()->Send_EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT( false );
			return true;

		} break;
	case PUCM_REWARD_ITEM_MOUSE_OVER:
		{
			CKTDGUIControl* pControl = reinterpret_cast <CKTDGUIButton*> (lParam);
			CKTDGUIDialogType pDialog =static_cast <CKTDGUIDialogType> (pControl->GetDialog() ); 
						
			int ItemID = pControl->GetDummyInt(0);

			CKTDGUIButton* pButton = reinterpret_cast <CKTDGUIButton*> (lParam);
			D3DXVECTOR2 vPos = pDialog->GetPos() + pButton->GetPos();			

			std::map<wstring, int>::iterator mit;
			mit = m_mapBossItemList.find( pButton->GetName());
			if (mit == m_mapBossItemList.end())
				return false;

#ifdef FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN
			 m_lParam = lParam;		// 해당 lParam 의 데이터 ( Button 주소 ) 를 담아뒀다 비교하는데 사용함
#endif // FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN

			OpenRewardItemInfoPopup( true, vPos, mit->second);
			return true;

		} break;
	case PUCM_REWARD_ITEM_MOUSE_OUT:
		{

#ifdef FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN
			if ( m_lParam == lParam )
			{
				OpenRewardItemInfoPopup( false, D3DXVECTOR2(0, 0), -1);
			}	
#else // FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN
			OpenRewardItemInfoPopup( false, D3DXVECTOR2(0, 0), -1);
#endif // FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN

			return true;
		} 
		
		break;

	case PUCM_DUNGEON_DIFFICULTY_CHECKED:
		{				
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
			if( NULL == pCheckBox )
				return true;

			if( m_fElapsedTime <= fPartyChangeTime)
			{
				pCheckBox->SetChecked(false);
				return true;
			}

			m_fElapsedTime = 0.f;
			const int iSelectDungeonDifficulty = pCheckBox->GetDummyInt(0);
			if( false == SelectDungeonDifficulty(iSelectDungeonDifficulty) )
			{
				pCheckBox->SetChecked(false);
			}
			return true;
		} break;
	case PUCM_DUNGEON_DIFFICULTY_UNCHECKED:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
			if( NULL != pCheckBox )
			{
				pCheckBox->SetChecked(true);
			}
			return true;
		} break;
	}

	return false;
}

void CX2PartyUI::ShortCutKeyProcess()
{
	bool bShowParty = false;
	bool bShowPVP = false;

	if( NULL != m_pDLGParty && true == m_pDLGParty->GetShow() )
	{
		bShowParty = true;
	}

	if ( m_pDLGPartyPVP != NULL && m_pDLGPartyPVP->GetShow() == true )
	{
		bShowPVP = true;
	}

	if( false == bShowParty && false == bShowPVP )
	{
		return;
	}

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F1) == TRUE )
	{
		if( m_fElapsedTime > 0.5f)
		{
			m_fElapsedTime = 0.f;
			SelectDungeonDifficulty( CX2Dungeon::DL_NORMAL );
		}
	}
	else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F2) == TRUE )
	{
		if( m_fElapsedTime > 0.5f)
		{
			m_fElapsedTime = 0.f;
			SelectDungeonDifficulty( CX2Dungeon::DL_HARD );
		}
	}
	else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F3) == TRUE )
	{
		if( m_fElapsedTime > 0.5f)
		{
			m_fElapsedTime = 0.f;
			SelectDungeonDifficulty( CX2Dungeon::DL_EXPERT );
		}
	}
	else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F5) == TRUE )
	{
		if( m_fElapsedTime > 0.5f)
		{
			m_fElapsedTime = 0.f;
			GameStartCurrentMember();
		}
	}
#ifdef SERV_KEY_MAPPING_INT  
	else if( GET_KEY_STATE( GA_PARTYREADY ) == TRUE || g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F8) == TRUE )
#else // SERV_KEY_MAPPING_INT
	else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F8) == TRUE )
#endif // SERV_KEY_MAPPING_INT
	{
		if( true == bShowParty &&
			true == IsReadyPossible() && false == g_pData->GetPartyManager()->GetProcessDungeonMatch() )
		{
			//{{ kimhc // 2011-03-02 // when skill window's been opened, you can't input the key, F8
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			CX2UISkillTreeNew* pUISkillTree = g_pData->GetUIManager()->GetUISkillTree();
		#else // UPGRADE_SKILL_SYSTEM_2013
			CX2UISkillTree* pUISkillTree = g_pData->GetUIManager()->GetUISkillTree();
		#endif // UPGRADE_SKILL_SYSTEM_2013

			if ( NULL == pUISkillTree ||
				( NULL != pUISkillTree && false == pUISkillTree->GetShow() ) )
			{			
				//{{ kimhc // 2011-02-21 // 드래그앤드랍 중에 F8 키를 누를 수 없도록 수정
				CX2SlotManager::CX2Slot** ppSlotBeforeDragging = g_pData->GetSlotBeforeDragging();

				// 드래깅중인게 없으면
				if ( NULL == *ppSlotBeforeDragging )
				{
		#ifdef FIX_JOIN_OFFICIAL_PVP_ROOM // 김태환
					/// 현재 연습 대전 대기실 혹은 방 이라면, 신청 못함
					if ( NULL != g_pMain && 
						( g_pMain->GetNowStateID() == CX2Main::XS_PVP_LOBBY ||
						  g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM ) )
						return;
		#endif //FIX_JOIN_OFFICIAL_PVP_ROOM

					// 게임내에서 바쁜 상태인 경우 던전을 시작 할 수 없음
					if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
						return ;			

					//던전 신청
					if( NULL != g_pData && NULL != g_pData->GetPartyManager() )
					{
						g_pData->GetPartyManager()->Handler_EGS_AUTO_PARTY_DUNGEON_GAME_REQ();
					}
				} // if
				//}} kimhc // 2011-02-21 // 드래그앤드랍 중에 F8 키를 누를 수 없도록 수정
			}
			//}} kimhc // 2011-03-02 // when skill window's opend, you can't input the key, F8
		}
		
		if ( true == bShowPVP && g_pData->GetPartyManager()->GetProcessPvpMatch() == false )
		{
			char cPlayNum = GetPvpMatchPlayerNum();
			char cPlayPvpMode = GetPvpMatchMode();

			g_pData->GetPartyManager()->Handler_EGS_REQUEST_MATCH_MAKING_REQ( cPlayNum, cPlayPvpMode );
			StartToWaitForPvpMatching();
			OpenPvpSetting(false);
			UpdatePvpPartyDlg();
		}
	}

}
#ifdef REFORM_NOVICE_GUIDE
void CX2PartyUI::UpdateNoviceGuide()
{
	if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
	{
		if( true == g_pData->GetPlayGuide()->GetActiveNoviceGuide() )
		{
			switch( g_pData->GetPlayGuide()->GetGuideStep() )
			{
			case CX2PlayGuide::NGS_STEP_5:
				{
					//현재 선택된 던전이 엘의나무가 아니라면 엘의나무 가리키도록 설정.
					if( true == m_bPartyDungeon && true == GetShow() )
					{
						bool bGuideDungeonButton = false;

						CX2QuestManager::QuestInst* pQuestInst = NULL;
						pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( CX2PlayGuide::TQI_VISIT_RUIN_OF_ELF );
						if( NULL != pQuestInst )//'윌리엄 주제에 건방지다'퀘스트를 가지고 있으면
						{
							//30080 (숲속의 폐허) 던전이 선택되지 않았다면
							if( 30080 != g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )
							{
								g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_8 );
								bGuideDungeonButton = true;
							}
						}
						else 
						{
							// 3000 (엘의 나무) 던전이 선택되지 않았다면
							if( 30000 == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )
							{
								g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_6 );
								bGuideDungeonButton = true;
							}
						}
						//특정 던전이 선택되어 있다면 
						if( false == bGuideDungeonButton )
						{
							g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_7 );
						}
					}		
				} break;
			case CX2PlayGuide::NGS_STEP_7:
				{
					//현재 선택된 던전이 퀘스트 진행 던전이 아니라면, 퀘스트 진행 던전 가리키도록

					if( true == m_bPartyDungeon && true == GetShow() )
					{
						CX2QuestManager::QuestInst* pQuestInst = NULL;
						pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( CX2PlayGuide::TQI_VISIT_RUIN_OF_ELF );
						if( NULL != pQuestInst )//'윌리엄 주제에 건방지다'퀘스트를 가지고 있으면
						{
							if( 30080 != g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )
							{
								g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_8 );
							}
						}
						else
						{
							if( 30000 != g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )
							{
								g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_6 );
							}
						}
					}
				} break;
			}
		}

		if ( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
		{
			if( false == g_pData->GetPlayGuide()->GetShowDungeonMenu() )
			{
				if( NULL != m_pDLGPartyMenu )
				{   //노비스가이드 활성화 중엔 던전도구장 보이지 않도록 수정
					m_pDLGPartyMenu->SetShowEnable(false, false);
				}
			}
		}
	}
}
#endif //REFORM_NOVICE_GUIDE
#ifdef QUEST_GUIDE
void CX2PartyUI::UpdateQuestGuide()
{
	if( NULL == m_pDLGPartyMenu )
		return;

	CKTDGUIStatic *pStaticGuide = static_cast<CKTDGUIStatic*>(m_pDLGPartyMenu->GetControl(L"ButtonQuestGuide"));
	if( NULL != pStaticGuide )
	{
		if( true == g_pData->GetQuestManager()->GetHasDungeonQuest() ) //던전 수행 퀘스트를 보유 하고 있다면
		{
			if( (NULL != m_pDLGParty && false == m_pDLGParty->GetShow()) && //파티메뉴가 열려 있지 않고
				true == m_bShowQuestMark ) // 던전시작 버튼이 보일 때
			{
				pStaticGuide->SetShow( true );	
			}
			else
			{
				pStaticGuide->SetShow( false );	
			}
		}
		else
		{
			pStaticGuide->SetShow( false );
		}
	}
}
#endif //QUEST_GUIDE
void CX2PartyUI::UpdateMatchingUI(float fElapsedTime_)
{
	if( NULL == m_pDLGPartyMenu )
		return;

	m_fWaitPvpTime += fElapsedTime_;

	if(	m_pDLGPartyMenu->GetShow() == true )
	{
		CKTDGUIStatic *pStaticStay = NULL;
		if( g_pData->GetPartyManager()->GetProcessPvpMatch() == true ) //대전
		{
			pStaticStay = (CKTDGUIStatic*)m_pDLGPartyMenu->GetControl(L"PVP_STAY");
		}
		else if( g_pData->GetPartyManager()->GetProcessDungeonMatch() == true ) //던전
		{
			pStaticStay = (CKTDGUIStatic*)m_pDLGPartyMenu->GetControl(L"DUNGEON_STAY");
		}

		if( NULL != pStaticStay && true == pStaticStay->GetShow() )
		{
			const int iBufSizeMax = 100;
			WCHAR wBuf1[iBufSizeMax];

			int iEstimatedTime = (m_iEstimatedTime / 60);
			if( iEstimatedTime < 1 )
			{
#ifdef HARDCODING_STRING_TO_INDEX
				StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%s", GET_STRING(STR_ID_24587) );
#else
				StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%s", L"1분미만(예상)/" ); //STR_ID_13616
#endif HARDCODING_STRING_TO_INDEX
			}
			else if( iEstimatedTime >= 30)
			{
#ifdef HARDCODING_STRING_TO_INDEX
				StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%s", GET_STRING(STR_ID_24588) );
#else
				StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%s", L"알수없음(예상)/" ); //STR_ID_13617
#endif HARDCODING_STRING_TO_INDEX
			}
			else
			{
#ifdef HARDCODING_STRING_TO_INDEX
				StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%02d:%02d%s", (int)(m_iEstimatedTime / 60), (int)(m_iEstimatedTime % 60), GET_STRING(STR_ID_24589) );
#else
				StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%02d:%02d%s", (int)(m_iEstimatedTime / 60), (int)(m_iEstimatedTime % 60), L"(예상) /" );
#endif HARDCODING_STRING_TO_INDEX
			}

#if defined(CLIENT_GLOBAL_LINEBREAK) && defined(CLIENT_COUNTRY_US)
			pStaticStay->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2Main( wBuf1, 150, pStaticStay->GetString(0)->fontIndex );
#else
			pStaticStay->GetString(0)->msg = wBuf1; // 예상시간
#endif //defined(CLIENT_GLOBAL_LINEBREAK) && defined(CLIENT_COUNTRY_US)

			StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%02d:%02d", (int)((int)m_fWaitPvpTime / 60), (int)((int)m_fWaitPvpTime % 60) );
			pStaticStay->GetString(1)->msg = wBuf1;
		}
	}
}

void CX2PartyUI::SetCurrentWaitUserCount( int iWaitUserCount_, bool bShow_ /*= true*/ )
{
	CKTDGUIStatic *pStaticStay = NULL;	
	if( g_pData->GetPartyManager()->GetProcessPvpMatch() == true ) //대전
	{
		pStaticStay = (CKTDGUIStatic*)m_pDLGPartyMenu->GetControl(L"PVP_STAY");
	}
	else if( g_pData->GetPartyManager()->GetProcessDungeonMatch() == true ) //던전
	{
		pStaticStay = (CKTDGUIStatic*)m_pDLGPartyMenu->GetControl(L"DUNGEON_STAY");
	}

	if( NULL != pStaticStay )
	{
		if( false == bShow_ )
		{
			pStaticStay->GetString(2)->msg = L"";
			pStaticStay->GetString(3)->msg = L"";
		}
		else
		{
#ifdef HARDCODING_STRING_TO_INDEX
			pStaticStay->GetString(2)->msg = GET_STRING(STR_ID_24590);
#else
			pStaticStay->GetString(2)->msg = L"   대기자 수   /";
#endif HARDCODING_STRING_TO_INDEX
			pStaticStay->GetString(3)->msg = GET_REPLACED_STRING( (STR_ID_2632, "i", iWaitUserCount_ ));
		}
	}

}

void CX2PartyUI::OpenPartyDLG( bool bOpen )
{
	if( false == bOpen )
	{
		if( true == g_pData->GetPartyManager()->GetProcessDungeonMatch() ||
			true == g_pData->GetPartyManager()->GetProcessPvpMatch())
			return;

		if( NULL != m_pDLGParty )
		{
			m_pDLGParty->SetShowEnable( bOpen, bOpen );
#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
			SetShowEnableRewardItemInfoDlg ( bOpen, bOpen, false );

#else // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
			if ( NULL != m_pDLGQuestRewardItemInfo )
				m_pDLGQuestRewardItemInfo->SetShow ( bOpen );
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
		}

#ifdef SERV_PVP_NEW_SYSTEM
		if( m_pDLGPartyPVP != NULL )
			m_pDLGPartyPVP->SetShowEnable( bOpen, bOpen );
		OpenPvpSetting( false );

		m_bPartyDungeon = false;
		m_bPartyPVP = false;
#endif
		// 09.04.01 태완
		if( NULL != m_pDLGWarningAllReady )
		{
			m_pDLGWarningAllReady->SetShowEnable(false, false);
		}
#ifdef REFORM_NOVICE_GUIDE
		if( NULL != g_pData && NULL != g_pData->GetPlayGuide() 
			&& true == g_pData->GetPlayGuide()->GetActiveNoviceGuide() )
		{
			switch( g_pData->GetPlayGuide()->GetGuideStep() )
			{
			case CX2PlayGuide::NGS_STEP_6:
			case CX2PlayGuide::NGS_STEP_7:
			case CX2PlayGuide::NGS_STEP_8:
				{
					g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_5 );
				} break;
			}
		}
#endif //REFORM_NOVICE_GUIDE

#ifdef SERV_EVENT_VALENTINE_DUNGEON		/// 발렌타인 던전 안내 이펙트 해제
		if( NULL != g_pData->GetUIMajorParticle() )		
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hValentineHeart1 );
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hValentineHeart2 );
		}
#endif SERV_EVENT_VALENTINE_DUNGEON
	}
	else
	{
		CreatePartyDlg();
		if( m_bPartyPVP == true )
		{
			m_pDLGParty->SetShowEnable( false, false );
			m_pDLGPartyPVP->SetShowEnable( true, true );

#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

			SetShowEnableRewardItemInfoDlg( false, false, false );

#else // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

			if( m_pDLGQuestRewardItemInfo != NULL )
			{
				m_pDLGQuestRewardItemInfo->SetShow( false );
			}

#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
			
			
			UpdatePvpPartyDlg();
		}
		else
		{
			m_bPartyDungeon = true;
			m_pDLGParty->SetShowEnable( true, true );
			m_pDLGPartyPVP->SetShowEnable( false, false );
		}

#ifdef REFORM_QUEST
		//퀵퀘스트 완료 파티클이 파티UI를 가리면 제거하도록 수정
		if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
		{
			g_pData->GetUIManager()->GetUIQuestNew()->ClearQuestCompleteParticle();
		}
#endif //REFORM_QUEST

#ifdef SERV_EVENT_VALENTINE_DUNGEON
#ifndef SERV_EVENT_VALENTINE_DUNGEON_INT
		if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )		/// 발렌타인 던전 안내 이펙트
		{
			m_hValentineHeart1 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"UI_Heart_Valentine_Arrow", 
				316, 74, 0, 9999, 9999, -1, 1, -1.0f, true, 1.2f, false );

			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hValentineHeart1 )
			{
				CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hValentineHeart1 );
				if( pParticle != NULL )
				{
					pParticle->SetOverUI( true );
				}
			}

			m_hValentineHeart2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"UI_Heart_Valentine_Arrow_Dest", 
				316, 74, 0, 9999, 9999, -1, 1, -1.0f, true, 1.2f, false );

			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hValentineHeart2 )
			{
				CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hValentineHeart2 );
				if( pParticle != NULL )
				{
					pParticle->SetOverUI( true );
				}
			}
		}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
#endif SERV_EVENT_VALENTINE_DUNGEON
	}

#ifdef SERV_PVP_NEW_SYSTEM
	if( m_pDungeonCheckBox != NULL )
		m_pDungeonCheckBox->SetChecked(m_bPartyDungeon);
	if( m_pPVPCheckBox != NULL )
		m_pPVPCheckBox->SetChecked(m_bPartyPVP);
#endif

	OpenLocalMapDlg();
}

void CX2PartyUI::OpenLocalMapDlg()
{
	//던전도구와 로컬맵 같이 열고 닫히도록 수정
	if( NULL != m_pLocalMapUI )
	{
		if( true == m_bPartyDungeon )
		{
			CX2LocationManager::LOCAL_MAP_ID eLocalMapID = m_pLocalMapUI->GetPickedLocalMapID();
			if( CX2LocationManager::LMI_INVALID == eLocalMapID  )
			{
				if( NULL != g_pData && NULL != g_pData->GetLocationManager() )
				{
					switch ( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_BATTLE_FIELD:
						{	
							CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
							UINT uiVillageId = g_pData->GetBattleFieldManager().GetReturnVillageId( battleFieldManager.GetBattleFieldIdWhereIam()  );
							eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( static_cast<SEnum::VILLAGE_MAP_ID>(uiVillageId) );
						} break;
					default:
						{
							eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( g_pData->GetLocationManager()->GetCurrentVillageID() );
						} break;
					}
				}
			}

			if( CX2LocationManager::LMI_INVALID == eLocalMapID )
			{
				eLocalMapID = CX2LocationManager::LMI_RUBEN;
			}

			m_pLocalMapUI->OpenLocalMapDLG( true, eLocalMapID );
			m_pLocalMapUI->UpdateLocalMapDLG();
		}
		else
		{
			m_pLocalMapUI->OpenLocalMapDLG(false);
		}
	}
}

void CX2PartyUI::CreatePartyDlg()
{
	if( NULL == m_pDLGParty )
	{
#ifdef SERV_PVP_NEW_SYSTEM
		m_pDLGParty = new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_PARTY_LENGTH_NEW.lua" );
#else 
		m_pDLGParty = new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_Party.lua" );
#endif //SERV_PVP_NEW_SYSTEM
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGParty );
		m_pDLGParty->SetDisableUnderWindow(true);
	}
#ifdef SERV_PVP_NEW_SYSTEM
	if( m_pDLGPartyPVP == NULL )
	{
		m_pDLGPartyPVP = new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_PVP_NEW.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPartyPVP );
		m_pDLGPartyPVP->SetDisableUnderWindow( true );

		CKTDGUIRadioButton* pChecked1_3 = (CKTDGUIRadioButton*)m_pDLGPartyPVP->GetControl(L"pvp_player_check3");			
		pChecked1_3->SetChecked(true);
		ShowPvpModeDesc();
	}
#endif

}

// @bRenamePartyTitle : 던전을 바꿀 경우 파티 이름을 다시 지정한다. 
void CX2PartyUI::UpdatePartyDLG( bool bRenamePartyTitle /*= false*/ )
{
#ifdef SERV_PVP_NEW_SYSTEM

#ifdef NEW_DEFENCE_DUNGEON		// mauntain : 김태환 [2012.05.29] 리소스가 Load 되었을 때 동작시킨다. ( UpdatePartyDLG 크래시 방지용 )
	if( m_bPartyPVP == true && NULL != m_pDLGPartyPVP )
#else  NEW_DEFENCE_DUNGEON
	if( m_bPartyPVP == true )
#endif NEW_DEFENCE_DUNGEON
	{
		UpdatePvpPartyDlg();
		return;
	}
#endif

	if( NULL == m_pDLGParty )
		return;

	if( false == m_pDLGParty->GetShow() )
		return;

	CX2PartyManager::PartyData* pPartyData = g_pData->GetPartyManager()->GetMyPartyData();

#ifdef SERV_NEW_EVENT_TYPES
	if( false == g_pData->GetDungeonManager()->IsDungeonEnable( pPartyData->m_iDungeonID ) )
	{
		pPartyData->m_iDungeonDifficulty	= CX2Dungeon::DL_NORMAL;
		pPartyData->m_iDungeonMode			= CX2Dungeon::DM_INVALID;
		pPartyData->m_iDungeonID			= SEnum::DI_RUBEN_EL_TREE_NORMAL;
	}
#endif SERV_NEW_EVENT_TYPES

//{{ mauntain : 김태환 [2012.05.08] 어둠의 문 개장 시간 종료시 이전 던전으로 강제 변경
#ifdef NEW_DEFENCE_DUNGEON
	//어둠의 문이 선택중이지만, 현재 열려있지 않을 때,
	if( true == g_pData->GetDungeonManager()->IsDefenceDungeon( pPartyData->m_iDungeonID ) )
	{

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-16
		/// 어둠의 문이 선택 되었을 때, 가장 최근에 입장했던 던전으로 변경
		if ( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			const int iLastEnterDungeonID = g_pData->GetMyUser()->GetSelectUnit()->GetRecentEnterDungeonID();

			pPartyData->m_iDungeonID		= (iLastEnterDungeonID > 0 ? iLastEnterDungeonID : SEnum::DI_RUBEN_EL_TREE_NORMAL);
		}
		else
			pPartyData->m_iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;

#else // SERV_NEW_DEFENCE_DUNGEON

		if( NULL != g_pData->GetWorldMissionManager() && false == g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() )
		{
			pPartyData->m_iDungeonDifficulty	= CX2Dungeon::DL_NORMAL;
			pPartyData->m_iDungeonMode			= CX2Dungeon::DM_INVALID;

			//그 외 - 엘의 나무
			pPartyData->m_iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;

			if( true == g_pData->GetPartyManager()->DoIHaveParty() )
			{
				if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
				{
					g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_DUNGEON_REQ( pPartyData->m_iDungeonID, pPartyData->m_iDungeonDifficulty, pPartyData->m_iDungeonMode );
				}
			}
		}
#endif // SERV_NEW_DEFENCE_DUNGEON

	}
#endif NEW_DEFENCE_DUNGEON
//}} mauntain : 김태환 [2012.05.08] 어둠의 문 개장 시간 종료시 이전 던전으로 강제 변경


	SEnum::DUNGEON_ID eDungeonIDWithDifficulty = 
		(SEnum::DUNGEON_ID) ( pPartyData->m_iDungeonID + g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty );

	const CX2Dungeon::DungeonData* pDungeonData			= g_pData->GetDungeonManager()->GetDungeonData( eDungeonIDWithDifficulty );
	const CX2Dungeon::DungeonData* pDungeonData_Normal	= g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID );


	ASSERT( NULL != pDungeonData );
	ASSERT( NULL != pDungeonData_Normal );

	if ( NULL == pDungeonData || NULL == pDungeonData_Normal )
		return;

	UpdateAdequateItemLevel(pDungeonData->m_RequireItemLevel);
	UpdateDungeonThumbnailQuestMark();

	// party data 갱신
	if( true == bRenamePartyTitle )
	{
		// 파티 이름 자동 생성
		if( NULL != pDungeonData_Normal )
		{
			if( true == pDungeonData_Normal->m_UIData.m_vecAutoRoomTitle.empty() )
			{
				g_pData->GetPartyManager()->GetMyPartyData()->m_wstrPartyTitle = g_pData->GetDungeonManager()->GetDefaultRoomTitle();
			}
			else
			{
				int iRand = rand() % static_cast<int>( pDungeonData_Normal->m_UIData.m_vecAutoRoomTitle.size() + 1 );
				if( iRand == static_cast<int>(pDungeonData_Normal->m_UIData.m_vecAutoRoomTitle.size()) )
				{
					g_pData->GetPartyManager()->GetMyPartyData()->m_wstrPartyTitle = g_pData->GetDungeonManager()->GetDefaultRoomTitle();
				}
				else
				{
					g_pData->GetPartyManager()->GetMyPartyData()->m_wstrPartyTitle = pDungeonData_Normal->m_UIData.m_vecAutoRoomTitle[iRand];
				}
			}
		}
	}

	// UI update


	CKTDGUIButton* pButton_GameStart		= (CKTDGUIButton*) m_pDLGParty->GetControl( L"CurrentMemberStart" );	//현재인원으로 시작

	//현재 인원으로 시작
	if ( NULL != pButton_GameStart )
		pButton_GameStart->SetShowEnable( true, true );
	//던전 신청
	if( NULL != g_pData && NULL != g_pData->GetPartyManager() && NULL != g_pData->GetPartyManager()->GetMyPartyData())
	{
		g_pData->GetPartyManager()->UpdateSelectedDungeon();
		//영웅모집던전 정보 요청
		g_pData->GetPartyManager()->Send_EGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT();
	}
	CX2LocationManager::LOCAL_MAP_ID eCurrLocalMapID = 
		g_pData->GetLocationManager()->GetLocalMapID( g_pData->GetLocationManager()->GetCurrentVillageID() );


	//////////////////////////////////////////////////////////////////////////
	// 버튼외에 기타 정보 update
	CKTDGUIStatic* pStatic_DungeonImage		= static_cast<CKTDGUIStatic*> (m_pDLGParty->GetControl( L"image" ));

	CKTDGUIStatic* pStatic_AdequateLevel		= static_cast<CKTDGUIStatic*>( m_pDLGParty->GetControl( L"need_level" ));	// 적정 레벨
	if( NULL != pStatic_AdequateLevel )
	{
		pStatic_AdequateLevel->GetString(0)->msg = GET_STRING(STR_ID_1104);			//적정레벨
	}
	CKTDGUIStatic* pStatic_FieldName		= static_cast<CKTDGUIStatic*> (m_pDLGParty->GetControl( L"Dungeon_Zone" ));	// 던전게이트 이름

	CKTDGUIStatic* pStaticPvpStayTime = (CKTDGUIStatic*) m_pDLGPartyMenu->GetControl( L"PVP_STAY" ); //pvp예상시간
	if( NULL != pStaticPvpStayTime )
		pStaticPvpStayTime->SetShow( false );

	CKTDGUIStatic* pStaticDungeonStayTime = (CKTDGUIStatic*) m_pDLGPartyMenu->GetControl( L"DUNGEON_STAY" ); //던전예상시간
	if( NULL != pStaticDungeonStayTime )
		pStaticDungeonStayTime->SetShow( false );

	CKTDGUIStatic* pStatic_DungeonDescription	= (CKTDGUIStatic*) m_pDLGParty->GetControl( L"DungeonDescription" ); //던전 설명
	if( NULL != pStatic_DungeonDescription )
	{
		if( NULL != pDungeonData )
		{
#ifdef INTEGRATE_TOOLTIP
			bool bEllipse = false;
			
			pStatic_DungeonDescription->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse( pDungeonData->m_DungeonDescription.c_str(), 170, pStatic_DungeonDescription->GetString(0)->fontIndex, 3, bEllipse);
			CKTDGUIStatic* DungeonDescriptionToolTip = (CKTDGUIStatic*) m_pDLGParty->GetControl(L"DungeonDescriptionToolTip");
			if(bEllipse == true)
			{
				if(DungeonDescriptionToolTip != NULL)
				{
					DungeonDescriptionToolTip->SetGuideDesc(CWordLineHandler::GetStrByLineBreakInX2Main( pDungeonData->m_DungeonDescription.c_str(), 170, pStatic_DungeonDescription->GetString(0)->fontIndex ).c_str());
					DungeonDescriptionToolTip->SetShow(true);
				}
			}
			else
			{
				if(DungeonDescriptionToolTip != NULL)
				{
					DungeonDescriptionToolTip->SetShow(false);
				}
			}
#else
			pStatic_DungeonDescription->GetString(0)->msg = g_pMain->GetStrByLienBreak( pDungeonData->m_DungeonDescription.c_str(), 170, pStatic_DungeonDescription->GetString(0)->fontIndex );
#endif
		}
	}



	if ( NULL != pStatic_FieldName )
	{
		for( int i=0; i< pStatic_FieldName->GetPictureNum(); i++ )
		{
			if( NULL != pStatic_FieldName->GetPicture(i) )
			{
				pStatic_FieldName->GetPicture(i)->SetShow( false );
			}
		}

		if ( false == CX2Dungeon::IsHenirDungeon( pDungeonData->m_DungeonID ) && 
			 false == CX2Dungeon::IsEventDungeon( pDungeonData->m_DungeonID ) )

		{
			CX2LocationManager::LOCAL_MAP_ID eLocalIDForDungeon = 
				g_pData->GetLocationManager()->GetLocalMapID( (SEnum::DUNGEON_ID) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID );

			switch( eLocalIDForDungeon )
			{
			case CX2LocationManager::LMI_RUBEN:
				{
					if( NULL != pStatic_FieldName->GetPicture(0) )
					{
						pStatic_FieldName->GetPicture(0)->SetShow( true );
					}
				} break;
			case CX2LocationManager::LMI_ELDER:
				{
#ifdef SERV_HALLOWEEN_DUNGEON
					// oasis907 : 김상윤 [2011.10.26] 할로윈 던전 예외 처리
					int DungeonId = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID;
					if( DungeonId  == (int)SEnum::DI_ELDER_HALLOWEEN_NORMAL ) 
					{
						pStatic_FieldName->GetPicture(1)->SetShow( false );
					}
					else
#endif SERV_HALLOWEEN_DUNGEON
						if( NULL != pStatic_FieldName->GetPicture(1) )
						{
							pStatic_FieldName->GetPicture(1)->SetShow( true );
						}
				} break;
			case CX2LocationManager::LMI_BESMA:
				{
					if( NULL != pStatic_FieldName->GetPicture(2) )
					{
						pStatic_FieldName->GetPicture(2)->SetShow( true );
					}
				} break;
			case CX2LocationManager::LMI_ALTERA_ISLAND:
				{
					if( NULL != pStatic_FieldName->GetPicture(3) )
					{
						pStatic_FieldName->GetPicture(3)->SetShow( true );
					}
				} break;
			case CX2LocationManager::LMI_PEITA:
				{
					if( NULL != pStatic_FieldName->GetPicture(4) )
					{
						pStatic_FieldName->GetPicture(4)->SetShow( true );
					}
				} break;

				//{{ kimhc // 2009-10-27 // 벨더 추가
			case CX2LocationManager::LMI_VELDER:
				{
					if( NULL != pStatic_FieldName->GetPicture(5) )
					{
						pStatic_FieldName->GetPicture(5)->SetShow( true );
					}
				} break;
				//}} kimhc // 2009-10-27 // 벨더 추가
#ifdef ADD_HAMEL_VILLAGE
			case CX2LocationManager::LMI_HAMEL:
				{
					if( NULL != pStatic_FieldName->GetPicture(6) )
					{
						pStatic_FieldName->GetPicture(6)->SetShow( true );
					}

				} break;	
#endif
			case CX2LocationManager::LMI_SANDER:
				{
					if( NULL != pStatic_FieldName->GetPicture(6) )
					{
						pStatic_FieldName->GetPicture(7)->SetShow( true );
					}

				} break;	
#ifdef SERV_CHINA_ADVENTURE	
			case CX2LocationManager::LMI_CHINA:
				{
					if( NULL != pStatic_FieldName->GetPicture( 6 ) )
					{
						pStatic_FieldName->GetPicture( 7 )->SetShow( true );
					}
				} break;
#endif SERV_CHINA_ADVENTURE
			default:
				{
					ASSERT( !"invalid dungeon ID" );
				} break;
			}	// switch
		} // if 
	} // if


	if( NULL != pDungeonData && NULL != pDungeonData_Normal )
	{

//{{ kimhc // 2010.3.19 //	비밀 던전 개편
#ifdef	SERV_SECRET_HELL
		// 던전및 그 난이도에 따라 파티창(좌측상단)의 난이도 스트링 변경

		CX2PartyManager::PartyData* pPartyData = g_pData->GetPartyManager()->GetMyPartyData();	
		switch ( pDungeonData_Normal->m_eDungeonType )
		{
		case CX2Dungeon::DT_NORMAL:
			{

			//{{ kimhc // 2011-04-26 // 디펜스 던전
#ifdef	SERV_INSERT_GLOBAL_SERVER
				if ( true == g_pData->GetDungeonManager()->IsDefenceDungeon( pDungeonData_Normal->m_DungeonID ) )
				{
					if ( NULL != pStatic_AdequateLevel )
						pStatic_AdequateLevel->GetString(1)->msg = L"";
				}
				else
#endif	SERV_INSERT_GLOBAL_SERVER
					//}} kimhc // 2011-04-26 // 디펜스 던전
				{
					if ( NULL != pStatic_AdequateLevel )
					{
						pStatic_AdequateLevel->GetString(1)->msg = GET_REPLACED_STRING( ( STR_ID_16458, "ii", pDungeonData->m_MinLevel, pDungeonData->m_MaxLevel ) );
						pStatic_AdequateLevel->GetString(1)->color = g_pData->GetDungeonManager()->GetDifficultyColor( 
							pPartyData->m_iDungeonID, pPartyData->m_iDungeonDifficulty, 
							g_pData->GetSelectUnitLevel() );
					}
				}
#ifdef NEW_DEFENCE_DUNGEON_LEVEL
				//개편된 어둠의 문 난이도는 엘의 수정이 지켜진 수에 따라 정해진다.
				if( true == g_pData->GetDungeonManager()->IsDefenceDungeon( pDungeonData_Normal->m_DungeonID ) )
				{
					int			iDifficulty		= 0;
					int			iDungeonMode	= CX2Dungeon::DT_INVALID;
					const int	iDefenceDungeonLevel = g_pData->GetWorldMissionManager()->GetNowDefenceDungeonLevel();

					UpdateDifficultyUI(static_cast<CX2Dungeon::DIFFICULTY_LEVEL>(iDefenceDungeonLevel), 
								pDungeonData_Normal->m_eDungeonType,
								pDungeonData_Normal->m_bNormalOnly);
					pPartyData->m_iDungeonDifficulty				= iDefenceDungeonLevel;
					pPartyData->m_iDungeonMode	= iDungeonMode;

					iDifficulty					= pPartyData->m_iDungeonDifficulty;
					//던전 난이도가 상승할 때만 호출
					if( m_bChangePartyUI == true )
					{
						if( true == g_pData->GetPartyManager()->DoIHaveParty() )
						{
							if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
							{
								g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_DUNGEON_REQ( pPartyData->m_iDungeonID, iDifficulty, iDungeonMode );
							}
						}
					}
				}
				else
#endif NEW_DEFENCE_DUNGEON_LEVEL
				{
					UpdateDifficultyUI(static_cast<CX2Dungeon::DIFFICULTY_LEVEL>(pPartyData->m_iDungeonDifficulty), 
						pDungeonData_Normal->m_eDungeonType, pDungeonData_Normal->m_bNormalOnly);
				}
			} break;

			case CX2Dungeon::DT_HENIR:
				{
					if ( NULL != pStatic_AdequateLevel )
						pStatic_AdequateLevel->GetString(1)->msg = L"1";

					switch( pPartyData->m_iDungeonMode )
					{
					case CX2Dungeon::DM_HENIR_PRACTICE:
						{
							UpdateDifficultyUI( CX2Dungeon::DL_NORMAL, CX2Dungeon::DT_HENIR,
										pDungeonData_Normal->m_bNormalOnly);
						} break;

					case CX2Dungeon::DM_HENIR_CHALLENGE:
						{
							UpdateDifficultyUI( CX2Dungeon::DL_HARD, CX2Dungeon::DT_HENIR,
										pDungeonData_Normal->m_bNormalOnly);
						} break;

					default:
						break;
					}
				} break;

			case CX2Dungeon::DT_SECRET:
				{
					if ( NULL != pStatic_AdequateLevel )
					{
						pStatic_AdequateLevel->GetString(1)->msg = GET_REPLACED_STRING( ( STR_ID_16458, "ii", pDungeonData->m_MinLevel, pDungeonData->m_MaxLevel ) );
						pStatic_AdequateLevel->GetString(1)->color = g_pData->GetDungeonManager()->GetDifficultyColor( 
							pPartyData->m_iDungeonID, 0, g_pData->GetSelectUnitLevel() );
					}

					switch( pPartyData->m_iDungeonMode )
					{
					default:
					case CX2Dungeon::DM_SECRET_NORMAL:
						{
							UpdateDifficultyUI( CX2Dungeon::DL_NORMAL, CX2Dungeon::DT_SECRET,
									pDungeonData_Normal->m_bNormalOnly);								
						} break;

					case CX2Dungeon::DM_SECRET_HELL:
						{
							UpdateDifficultyUI( CX2Dungeon::DL_HARD, CX2Dungeon::DT_SECRET,
								pDungeonData_Normal->m_bNormalOnly);	
						} break;
					}
				} break;
			}
#else	SERV_SECRET_HELL
	//{{AFX
	#ifdef HENIR_TEST
		if( CX2Dungeon::DT_HENIR == pDungeonData_Normal->m_eDungeonType )
		{
			pStatic_AdequateLevel->GetString(0)->msg = L"";
			pStatic_AdequateMemberCount->GetString(0)->msg = L"";

			pStatic_DifficultyImage->SetShowEnable( true, true );
			//pStatic_RankingImage->SetShowEnable( false, false );

			switch( (CX2Dungeon::DIFFICULTY_LEVEL) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
			{
			case CX2Dungeon::DM_HENIR_PRACTICE:
				{
					pStatic_Difficulty->GetString(0)->msg = GET_STRING( STR_ID_938 );
				} break;

			case CX2Dungeon::DM_HENIR_CHALLENGE:
				{
					pStatic_Difficulty->GetString(0)->msg = GET_STRING( STR_ID_3747 );
				} break;

			default:
				{
					pStatic_Difficulty->GetString(0)->msg = L"";
				} break;
			}
		}
		else if( true == pDungeonData->m_UIData.m_bArcadeMode )
	#else HENIR_TEST
		if( true == pDungeonData->m_UIData.m_bArcadeMode )
	#endif HENIR_TEST
		{
			pButton_DifficultyLeft->SetShowEnable( false, false );
			pButton_DifficultyRight->SetShowEnable( false, false );


			pStatic_AdequateLevel->GetString(0)->msg = L"";
			pStatic_AdequateMemberCount->GetString(0)->msg = L"";

			pStatic_DifficultyImage->SetShowEnable( false, false );
			//pStatic_RankingImage->SetShowEnable( true, true );

			if( true == g_pData->GetPartyManager()->IsRankableArcadeDungeon( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) )
			{
				pStatic_Difficulty->GetString(0)->msg = GET_STRING( STR_ID_382 );
			}
			else
			{
				pStatic_Difficulty->GetString(0)->msg = GET_STRING( STR_ID_383 );
			}


		}
		else
		{
			pStatic_AdequateLevel->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_384, "ii", pDungeonData->m_MinLevel, pDungeonData->m_MaxLevel ) );
			pStatic_AdequateLevel->GetString(0)->color = g_pData->GetDungeonManager()->GetDifficultyColor( 
				g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID,
				g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty, 
				g_pData->GetSelectUnitLevel() );


			pStatic_DifficultyImage->SetShowEnable( true, true );
			//pStatic_RankingImage->SetShowEnable( false, false );



			switch( (CX2Dungeon::DIFFICULTY_LEVEL) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty )
			{
			default:
			case CX2Dungeon::DL_NORMAL:
				{
					pStatic_Difficulty->GetString(0)->msg = GET_STRING( STR_ID_385 );
					pStatic_AdequateMemberCount->GetString(0)->msg = L"";
				} break;

			case CX2Dungeon::DL_HARD:
				{
					pStatic_Difficulty->GetString(0)->msg = GET_STRING( STR_ID_386 );
					pStatic_AdequateMemberCount->GetString(0)->msg = GET_STRING( STR_ID_387 );

					if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() >= 2 )
					{
						pStatic_AdequateMemberCount->GetString(0)->color = D3DXCOLOR( 0,1,0,1 );
					}
					else
					{
						pStatic_AdequateMemberCount->GetString(0)->color = D3DXCOLOR( 1,0,0,1  );
					}
				} break;

			case CX2Dungeon::DL_EXPERT:
				{
					pStatic_Difficulty->GetString(0)->msg = GET_STRING( STR_ID_388 );
					pStatic_AdequateMemberCount->GetString(0)->msg = GET_STRING( STR_ID_389 );

					if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() >= 3 )
					{
						pStatic_AdequateMemberCount->GetString(0)->color = D3DXCOLOR( 0,1,0,1 );
					}
					else
					{
						pStatic_AdequateMemberCount->GetString(0)->color = D3DXCOLOR( 1,0,0,1  );
					}

				} break;
			}
		}
	//}}AFX
#endif	SERV_SECRET_HELL
//}} kimhc // 2010.3.19 //	비밀 던전 개편

		// 선택된 던전 그림

		UpdateBossItemList(pDungeonData->m_mapBossDropItem, (pPartyData->m_iDungeonMode == CX2Dungeon::DM_HENIR_CHALLENGE) );

		if( NULL != pStatic_DungeonImage && NULL != pStatic_DungeonImage->GetPicture(0) )
		{
			pStatic_DungeonImage->GetPicture(0)->SetTex( pDungeonData_Normal->m_UIData.m_TextureName.c_str(), pDungeonData_Normal->m_UIData.m_PieceName.c_str() );
		}
	}
#ifdef NEW_DEFENCE_DUNGEON
	m_bChangePartyUI = false;
#endif NEW_DEFENCE_DUNGEON
}




// void CX2PartyUI::OpenPartyMemberDLG( bool bOpen )
// {
// 	if( false == bOpen )
// 	{
// 		if( NULL != m_pDLGPartyMember )
// 		{
// 			m_pDLGPartyMember->SetShowEnable( bOpen, bOpen );
// 		}
// 	}
// 	else
// 	{
// 		if( NULL == m_pDLGPartyMember )
// 		{
// 			m_pDLGPartyMember = new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_Party_Comrade.lua" );	
// 			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPartyMember );
// 			m_pDLGPartyMember->SetDisableUnderWindow(true);
// 		}
// 
// 		if(g_pData->GetPartyManager()->DoIHaveParty() == true)
// 			m_pDLGPartyMember->SetShowEnable( bOpen, bOpen );
// 		else
// 			m_pDLGPartyMember->SetShowEnable( false, false );
// 	}
// 
// }
// 
// 
// 
// void CX2PartyUI::UpdatePartyMemberDLG()
// {
// 	if( NULL == m_pDLGPartyMember )
// 		return;
// 
// 	if( false == m_pDLGPartyMember->GetShow() )
// 		return;
// 
// 	if( false == g_pData->GetPartyManager()->DoIHaveParty() )
// 	{
// 		m_pDLGPartyMember->SetShowEnable( false, false );
// 		return;
// 	}
// 
// 	//{{ 09.04.01 태완
// 	if ( true == g_pData->GetPartyManager()->AmIPartyLeader() &&					// 내가 파티장이고
// 		g_pData->GetPartyManager()->GetMyPartyData()->m_vecPartyMemberData.size() > 1 &&	// 파티원이 2명 이상이고(나 혼자가 아니고)
// 		g_pData->GetPartyManager()->IsAllReady() )									// 전원 레디 중이면
// 	{
// 		PlayWarningAllReady();
// 	}
// 	else
// 	{
// 		StopWarningAllReady();
// 	}
// 	//}}
// 
// 
// 	WCHAR wszText[128] = L"";
// 
// 
// 	CKTDGUIButton* pButton_Member[3] 		= { NULL, };
// 	CKTDGUIStatic* pStatic_Member[3] 		= { NULL, };
// 	CKTDGUIStatic* pStatic_MemberReady[3]	= { NULL, };
// 	CKTDGUIStatic* pStatic_MemberPlace[3] 	= { NULL, };
// 	CKTDGUIStatic* pStatic_MemberLevel[3] 	= { NULL, };
// 	CKTDGUIStatic* pStatic_MemberName[3]	= { NULL, };
// 	// oasis907 : 김상윤 [2010.3.9] // 
// 	CKTDGUIStatic* pStatic_MemberChannel[3] = { NULL, };
// #ifdef COME_BACK_REWARD
// 	CKTDGUIStatic* pStatic_MemberComeBack[3] = { NULL, };
// #endif
// 
// #ifdef SERV_INTEGRATION // oasis907 : 김상윤 [2010.5.12] // 
// 	CKTDGUIStatic* pStatic_MemberServer[3] = { NULL, };
// #endif SERV_INTEGRATION
// 	for( int i=0; i<3; i++ )
// 	{
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"g_pButtoncomrade%d", i+1 );
// 		pButton_Member[i] = (CKTDGUIButton*) m_pDLGPartyMember->GetControl( wszText );
// 
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"g_pStatic_party%d", i+1 );
// 		pStatic_Member[i] = (CKTDGUIStatic*) m_pDLGPartyMember->GetControl( wszText );
// 
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"place%d", i+1 );
// 		pStatic_MemberPlace[i] = (CKTDGUIStatic*) m_pDLGPartyMember->GetControl( wszText );
// 
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"level%d", i+1 );
// 		pStatic_MemberLevel[i] = (CKTDGUIStatic*) m_pDLGPartyMember->GetControl( wszText );
// 
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"name%d", i+1 );
// 		pStatic_MemberName[i] = (CKTDGUIStatic*) m_pDLGPartyMember->GetControl( wszText );
// 
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"g_pStatic_Ready%d", i+1 );
// 		pStatic_MemberReady[i] = (CKTDGUIStatic*) m_pDLGPartyMember->GetControl( wszText );
// 	
// 		// oasis907 : 김상윤 [2010.3.9] // 
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"channel%d", i+1 );
// 		pStatic_MemberChannel[i] = (CKTDGUIStatic*) m_pDLGPartyMember->GetControl( wszText );
// 
// #ifdef SERV_INTEGRATION // oasis907 : 김상윤 [2010.5.12] // 
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"Static_SERVER%d", i+1 );
// 		pStatic_MemberServer[i] = (CKTDGUIStatic*) m_pDLGPartyMember->GetControl( wszText );
// #endif SERV_INTEGRATION
// #ifdef COME_BACK_REWARD
// 		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"Power%d", i+1 );
// 		pStatic_MemberComeBack[i] = (CKTDGUIStatic*) m_pDLGPartyMember->GetControl( wszText );
// #endif
// 		CX2PartyManager::PartyMemberData* pMemberData =
// 			g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( i, g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
// 		
// 		if( NULL == pMemberData )
// 		{
// 			pButton_Member[i]->SetShowEnable( false, false ); 		
// 			pStatic_Member[i]->SetShowEnable( false, false );		
// 			pStatic_MemberPlace[i]->SetShowEnable( false, false ); 	
// 			pStatic_MemberLevel[i]->SetShowEnable( false, false ); 	
// 			pStatic_MemberName[i]->SetShowEnable( false, false );	
// 			pStatic_MemberReady[i]->SetShowEnable(false, false);
// 			// oasis907 : 김상윤 [2010.3.9] // 
// 			pStatic_MemberChannel[i]->SetShowEnable( false, false );
// 
// #ifdef SERV_INTEGRATION
// 			pStatic_MemberServer[i]->SetShowEnable( false, false );
// #endif SERV_INTEGRATION
// #ifdef COME_BACK_REWARD
// 			pStatic_MemberComeBack[i]->SetShowEnable( false, false );
// #endif
// 		}
// 		else
// 		{
// 			pButton_Member[i]->SetShowEnable( true, true ); 		
// 			pStatic_Member[i]->SetShowEnable( true, true );		
// 			pStatic_MemberPlace[i]->SetShowEnable( true, true ); 	
// 			pStatic_MemberLevel[i]->SetShowEnable( true, true ); 	
// 			pStatic_MemberName[i]->SetShowEnable( true, true );	
// 			//pStatic_MemberReady[i]->SetShowEnable(true, true);
// 			// oasis907 : 김상윤 [2010.3.9] // 
// 			pStatic_MemberChannel[i]->SetShowEnable( true, true );	
// 
// #ifdef SERV_INTEGRATION
// 			pStatic_MemberServer[i]->SetShowEnable( true, true );	
// #endif SERV_INTEGRATION
// 
// #ifdef COME_BACK_REWARD
// 			if ( NULL != g_pData->GetPremiumBuffManager() && pMemberData->m_bComeBackUser )
// 				g_pData->GetPremiumBuffManager()->SetComeBackUserInParty( true );
// 			else
// 				g_pData->GetPremiumBuffManager()->SetComeBackUserInParty( false );
// 
// 			pStatic_MemberComeBack[i]->SetShowEnable( pMemberData->m_bComeBackUser, pMemberData->m_bComeBackUser );
// #endif
// 			wstring fileName;
// 			wstring pieceName;
// 			if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pMemberData->m_eUnitClass , CX2Data::CIT_50by50 ) )
// 			{
// 				pStatic_Member[i]->GetPicture(1)->SetTex( fileName.c_str(), pieceName.c_str() );
// 			}
// 			else
// 			{
// 				ASSERT( !"NO" );
// 			}
// 
// 
// 
// 			if( true == pMemberData->m_bPartyLeader )
// 			{
// 				// 파티장 별표
// 				pStatic_Member[i]->GetPicture(2)->SetShow( true );
// 
// 				// 레디 표시
// 				switch( pMemberData->m_eState ) 
// 				{
// 				case CX2Unit::CUS_FIELD_MAP:
// 					{
// 						CX2LocationManager::LOCAL_MAP_ID localID = 
// 							g_pData->GetLocationManager()->GetLocalMapID( (SEnum::VILLAGE_MAP_ID) pMemberData->m_iStateCode );
// 
// 						if( true == g_pData->GetLocationManager()->IsDungeonGate( (SEnum::VILLAGE_MAP_ID) pMemberData->m_iStateCode ) &&
// 							true == g_pData->GetPartyManager()->IsDungeonInLocal( localID, g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) )
// 						{
// 							const CX2Dungeon::DungeonData* pDungeonData_Normal = 
// 								g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID );
// 
// 
// 							int iRequireItemID = 0;
// 							int iRequireItemCount = 0;
// 							if( NULL != pDungeonData_Normal )
// 							{
// #ifdef HENIR_TEST
// 
// 								switch( pDungeonData_Normal->m_eDungeonType )
// 								{
// 								default:
// 									{
// 										iRequireItemID = pDungeonData_Normal->m_RequireItemID;
// 										iRequireItemCount = pDungeonData_Normal->m_RequireItemCount;
// 									} break;
// 
// 								case CX2Dungeon::DT_HENIR:
// 									{
// 										switch( (CX2Dungeon::DUNGEON_MODE) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
// 										{
// 										case CX2Dungeon::DM_HENIR_PRACTICE:
// 											{
// 												iRequireItemID = pDungeonData_Normal->m_RequireItemID;
// 												iRequireItemCount = pDungeonData_Normal->m_RequireItemCount;
// 											} break;
// 
// 										case CX2Dungeon::DM_HENIR_CHALLENGE:
// 											{
// 												iRequireItemID = pDungeonData_Normal->m_RequireItemID2;
// 												iRequireItemCount = pDungeonData_Normal->m_RequireItemCount2;
// 											} break;
// 										}
// 									} break;
// 								}
// #else HENIR_TEST
// 								//{{AFX
// 								iRequireItemID = pDungeonData_Normal->m_RequireItemID;
// 								iRequireItemCount = pDungeonData_Normal->m_RequireItemCount;
// 								//}}AFX
// #endif HENIR_TEST
// 							}
// 
// 
// 							if( iRequireItemCount > 0 )
// 							{
// 								if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iRequireItemID ) >= iRequireItemCount )
// 								{
// 									pStatic_MemberReady[i]->SetShowEnable(true, true);
// 								}
// 								else
// 								{
// 									pStatic_MemberReady[i]->SetShowEnable(false, false);
// 								}
// 							}
// 							else
// 							{
// 								pStatic_MemberReady[i]->SetShowEnable(true, true);
// 							}
// 
// 						}
// 						else
// 						{
// 							pStatic_MemberReady[i]->SetShowEnable(false, false);
// 							//pStatic_Member[i]->GetPicture(2)->SetShow( false );
// 						}
// 					} break;
// 				default:
// 					{
// 						pStatic_MemberReady[i]->SetShowEnable(false, false);
// 						//pStatic_Member[i]->GetPicture(2)->SetShow( false );
// 					} break;
// 				}
// 			}
// 			else
// 			{
// 				// 파티장 별표
// 				pStatic_Member[i]->GetPicture(2)->SetShow( false );
// 
// 				// 레디 표시
// 				if( true == pMemberData->m_bGameReady )
// 				{
// 					pStatic_MemberReady[i]->SetShowEnable(true, true);					
// 					//pStatic_Member[i]->GetPicture(2)->SetShow( true );
// 				}
// 				else
// 				{
// 					pStatic_MemberReady[i]->SetShowEnable(false, false);
// 					//pStatic_Member[i]->GetPicture(2)->SetShow( false );
// 				}
// 			}
// 
// #ifdef SERV_PVP_NEW_SYSTEM
// 			if( g_pData->GetPartyManager()->GetMyPartyData()->m_bPvpParty == true )
// 			{
// 				pStatic_MemberReady[i]->SetShowEnable(false, false);
// 			}
// #endif
// 
// 			
// 			switch( pMemberData->m_eState ) 
// 			{
// 			case CX2Unit::CUS_FIELD_MAP:
// 				{
// 					SEnum::VILLAGE_MAP_ID eVillageID = (SEnum::VILLAGE_MAP_ID ) pMemberData->m_iStateCode;
// 					
// 					if( true == g_pData->GetLocationManager()->IsVillage( eVillageID ) ||
// 						true == g_pData->GetLocationManager()->IsDungeonGate( eVillageID ) )
// 					{
// 						CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eVillageID );
// 						if( NULL != pVillageTemplet )
// 						{
// 							pStatic_MemberPlace[i]->GetString(0)->msg = pVillageTemplet->m_Name;
// 						}
// 					}
// 					else if( true == g_pData->GetLocationManager()->IsDungeonLounge( eVillageID ) )
// 					{
// 						CX2LocationManager::LOCAL_MAP_ID eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( eVillageID );
// 						SEnum::VILLAGE_MAP_ID eDungeonGateID = (SEnum::VILLAGE_MAP_ID) g_pData->GetLocationManager()->GetDungeonGateID( eLocalMapID );
// 						CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eDungeonGateID );
// 						if( NULL != pVillageTemplet )
// 						{
// 							pStatic_MemberPlace[i]->GetString(0)->msg = pVillageTemplet->m_Name;
// 						}
// 					}
// 					else
// 					{
// 						ASSERT( !"invalid" );
// 					}
// 				} break;
// 
// 
// 			case CX2Unit::CUS_DUNGEON_PLAY:
// 			case CX2Unit::CUS_ARCADE_PLAY:
// 				{
// 					SEnum::VILLAGE_MAP_ID eVillageID = g_pData->GetLocationManager()->GetCurrentVillageID();
// 					CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eVillageID );
// 					if( NULL != pVillageTemplet )
// 					{
// 						pStatic_MemberPlace[i]->GetString(0)->msg = pVillageTemplet->m_Name;
// 					}
// 				} break;
// 
// 			default:
// 			case CX2Unit::CUS_MARKET:
// 			case CX2Unit::CUS_PVP_WAIT:
// 			case CX2Unit::CUS_PVP_PLAY:
// 			case CX2Unit::CUS_TC_PLAY:
// 				{
// 					ASSERT( !"invalid" );
// 				} break;
// 			}
// 
// 			StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"Lv.%d", pMemberData->m_iUnitLevel );
// 			pStatic_MemberLevel[i]->GetString(0)->msg = wszText;
// 
// 			pStatic_MemberName[i]->GetString(0)->msg = pMemberData->m_wstrNickName;
// 
// #ifdef SERV_INTEGRATION
// 			SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
// 
// 
// 			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(pMemberData->m_iUnitUID);
// 
// 			if( eServerGroupID == SGI_SOLES)
// 			{
// 				pStatic_MemberServer[i]->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture11.tga", L"Soles");
// 				pStatic_MemberServer[i]->SetShowEnable( true, true );	
// 			}
// 			else if( eServerGroupID == SGI_GAIA)
// 			{
// 				pStatic_MemberServer[i]->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture11.tga", L"Gaia");
// 				pStatic_MemberServer[i]->SetShowEnable( true, true );	
// 			}
// 			else
// 			{
// 				pStatic_MemberServer[i]->SetShowEnable( false, false );	
// 			}
// #endif SERV_INTEGRATION
// 
// 
// #ifdef SERV_CHANNEL_PARTY
// 			std::wstring wstrMemberChannel;
// 
// #ifdef SERVER_INTEGRATION_CHANNEL_NAME_FIX
// 			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
// 			{
// 				wstrMemberChannel = g_pInstanceData->GetChannelServerName(pMemberData->m_iChannelID, eServerGroupID);
// 			}
// 			else
// 			{
// 				wstrMemberChannel = g_pInstanceData->GetChannelServerName(pMemberData->m_iChannelID);
// 			}
// 			wstrMemberChannel += L":";
// 			pStatic_MemberChannel[i]->GetString(0)->msg = wstrMemberChannel;
// #else
// 			wstrMemberChannel = g_pInstanceData->GetChannelServerName(pMemberData->m_iChannelID);
// 			wstrMemberChannel += L":";
// 			pStatic_MemberChannel[i]->GetString(0)->msg = wstrMemberChannel;
// #endif SERVER_INTEGRATION_CHANNEL_NAME_FIX
// 
// #endif SERV_CHANNEL_PARTY
// 
// 
// 
// 		}
// 	}						
// 
// }


void CX2PartyUI::OpenPartyListMemberPreviewDLG( bool bOpen, int iPartyIndex )
{
	if( iPartyIndex < 0 )
		return;

	if(m_aPartyData[iPartyIndex].m_iPartyUID <= 0)
	{
		bOpen = false;
	}

	if( false == bOpen )
	{
		if( NULL != m_pDLGPartyListMeberPreview )
		{
			m_pDLGPartyListMeberPreview->SetShowEnable( bOpen, bOpen );
		}
	}
	else
	{
		if( NULL == m_pDLGPartyListMeberPreview )
		{
			m_pDLGPartyListMeberPreview = new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_Party_Comrade_View.lua" );	
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPartyListMeberPreview );
			m_pDLGPartyListMeberPreview->SetDisableUnderWindow(true);
		}
		m_pDLGPartyListMeberPreview->SetShowEnable( bOpen, bOpen );

		UpdatePartyListMemberPreviewDLG(iPartyIndex);		
	}
}


void CX2PartyUI::UpdatePartyListMemberPreviewDLG( int iPartyIndex )
{
	if( iPartyIndex < 0 )
		return;

	if( NULL == m_pDLGPartyListMeberPreview )
		return;

	if( false == m_pDLGPartyListMeberPreview->GetShow() )
		return;

	D3DXVECTOR2 DLGPos;
	DLGPos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	DLGPos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();	
	DLGPos = g_pKTDXApp->MouseConvertByResolution( DLGPos );
	DLGPos = DLGPos + D3DXVECTOR2(10, 10);		// Left-top pos
	m_pDLGPartyListMeberPreview->SetPos(DLGPos);

	// 여기서 업데이트
	const int MAGIC_MAX_PARTY_MEMBER = 4;
	CX2PartyManager::PartyData& refPartyData = m_aPartyData[iPartyIndex];

	for(int i=0; i<MAGIC_MAX_PARTY_MEMBER; i++)
	{
		WCHAR buf[64];
		StringCchPrintf( buf, 64, L"Image%d", i+1 );
		//wsprintf( buf, L"Image%d", i+1 );
		CKTDGUIStatic* pStatic_Image = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );

		StringCchPrintf( buf, 64, L"Captain_Sign%d", i+1 );
		/*wsprintf( buf, L"Captain_Sign%d", i+1 );*/
		CKTDGUIStatic* pStatic_CaptainSign = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );

		StringCchPrintf( buf, 64, L"Static_LV%d", i+1 );
		//wsprintf( buf, L"Static_LV%d", i+1 );
		CKTDGUIStatic* pStatic_LV = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );

		StringCchPrintf( buf, 64, L"Static_NAME%d", i+1 );
		//wsprintf( buf, L"Static_NAME%d", i+1 );
		CKTDGUIStatic* pStatic_Name = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );

// 		wsprintf( buf, L"Party_Empty%d", i );
// 		CKTDGUIStatic* pStatic_Empty = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );
// 		
// 		wsprintf( buf, L"Party_Close%d", i );
// 		CKTDGUIStatic* pStatic_Close = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );

		StringCchPrintf( buf, 64, L"Static_CHANNEL%d", i+1 );
		//wsprintf( buf, L"Static_CHANNEL%d", i+1 );
		CKTDGUIStatic* pStatic_Channel = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );		

		StringCchPrintf( buf, 64, L"Static_PLACE%d", i+1 );
		//wsprintf( buf, L"Static_PLACE%d", i+1 );
		CKTDGUIStatic* pStatic_Place = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );	

#ifdef SERV_INTEGRATION
		StringCchPrintf( buf, 64, L"Static_SERVER%d", i+1 );
		//wsprintf( buf, L"Static_SERVER%d", i+1 );
		CKTDGUIStatic* pStatic_Server = (CKTDGUIStatic*) m_pDLGPartyListMeberPreview->GetControl( buf );	
#endif SERV_INTEGRATION
		if(i > refPartyData.m_iMaxPartyMemberCount)
		{
			// 닫힌 슬롯
			pStatic_Image->SetShow(false);
			pStatic_CaptainSign->SetShow(false);
			pStatic_LV->SetShow(false);
			pStatic_Name->SetShow(false);
// 			pStatic_Empty->SetShow(false);
// 			pStatic_Close->SetShow(true);
			pStatic_Channel->SetShow(false);
			pStatic_Place->SetShow(false);

#ifdef SERV_INTEGRATION
			pStatic_Server->SetShow(false);
#endif SERV_INTEGRATION
		}
		else
		{
			CX2PartyManager::PartyMemberData* pMemberData = refPartyData.GetPartyMemberData( i );

			if(pMemberData == NULL)
			{
				// 열린 슬롯
				pStatic_Image->SetShow(false);
				pStatic_CaptainSign->SetShow(false);
				pStatic_LV->SetShow(false);
				pStatic_Name->SetShow(false);
// 				pStatic_Empty->SetShow(true);
// 				pStatic_Close->SetShow(false);
				pStatic_Channel->SetShow(false);
				pStatic_Place->SetShow(false);

#ifdef SERV_INTEGRATION
				pStatic_Server->SetShow(false);
#endif SERV_INTEGRATION
			}
			else
			{
				
								
				// 사람 있음
				pStatic_CaptainSign->SetShow(pMemberData->m_bPartyLeader);
				
				wstring fileName;
				wstring pieceName;
				if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pMemberData->m_eUnitClass, CX2Data::CIT_50by50 ) )
				{
					pStatic_Image->GetPicture(0)->SetTex( fileName.c_str(), pieceName.c_str() );
				}
				else
				{
					ASSERT( !"NO" );
				}
				pStatic_Image->SetShow(true);

				WCHAR buff[64];
				StringCchPrintf( buff, 64, L"%d", pMemberData->m_iUnitLevel );
				//wsprintf( buff, L"%d", pMemberData->m_iUnitLevel );
				pStatic_LV->GetString(0)->msg = buff;
				pStatic_LV->SetShow(true);


				pStatic_Name->SetShow(true);
				pStatic_Name->GetString(0)->msg = pMemberData->m_wstrNickName.c_str();

// 				pStatic_Empty->SetShow(false);
// 				pStatic_Close->SetShow(false);



#ifdef SERV_INTEGRATION
#ifdef EXTEND_SERVER_GROUP_MASK
				int iServerGroupID = -1;

				iServerGroupID = (int) g_pMain->ExtractServerGroupID(pMemberData->m_iUnitUID);
#else
				SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;

				eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(pMemberData->m_iUnitUID);
#endif // EXTEND_SERVER_GROUP_MASK

#ifndef REMOVE_KR_SERVER_TEXTURE
				if( eServerGroupID == SGI_SOLES)
				{
					pStatic_Server->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture11.tga", L"Soles");
					pStatic_Server->SetShow(true);
				}
				else if( eServerGroupID == SGI_GAIA)
				{
					pStatic_Server->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture11.tga", L"Gaia");
					pStatic_Server->SetShow(true);
				}
				else
#endif // REMOVE_KR_SERVER_TEXTURE
				{
					pStatic_Server->SetShow(false);
				}
#endif SERV_INTEGRATION

#ifdef SERV_CHANNEL_PARTY

#ifdef SERVER_INTEGRATION_CHANNEL_NAME_FIX
				std::wstring wstrStatic_Channel;
#ifdef EXTEND_SERVER_GROUP_MASK
				if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
				{
					wstrStatic_Channel = g_pInstanceData->GetChannelServerName(pMemberData->m_iChannelID, iServerGroupID);
				}
#else
				if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
				{
					wstrStatic_Channel = g_pInstanceData->GetChannelServerName(pMemberData->m_iChannelID, eServerGroupID);
				}
#endif // EXTEND_SERVER_GROUP_MASK
				else
				{
					wstrStatic_Channel = g_pInstanceData->GetChannelServerName(pMemberData->m_iChannelID);
				}
				wstrStatic_Channel += L":";
				pStatic_Channel->GetString(0)->msg = wstrStatic_Channel;
				pStatic_Channel->SetShow(true);
#else
				std::wstring wstrStatic_Channel;
				wstrStatic_Channel = g_pInstanceData->GetChannelServerName(pMemberData->m_iChannelID);
				wstrStatic_Channel += L":";				

				pStatic_Channel->GetString(0)->msg = wstrStatic_Channel;
				pStatic_Channel->SetShow(true);
#endif SERVER_INTEGRATION_CHANNEL_NAME_FIX


				switch( pMemberData->m_eState ) 
				{
				case CX2Unit::CUS_FIELD_MAP:
					{
						SEnum::VILLAGE_MAP_ID eVillageID = (SEnum::VILLAGE_MAP_ID ) pMemberData->m_iStateCode;

						if( true == g_pData->GetLocationManager()->IsVillage( eVillageID ) ||
							true == g_pData->GetLocationManager()->IsDungeonGate( eVillageID ) )
						{
							CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eVillageID );
							if( NULL != pVillageTemplet )
							{
								pStatic_Place->GetString(0)->msg = pVillageTemplet->m_Name;
							}
						}
						else if( true == g_pData->GetLocationManager()->IsDungeonLounge( eVillageID ) )
						{
							CX2LocationManager::LOCAL_MAP_ID eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( eVillageID );
							SEnum::VILLAGE_MAP_ID eDungeonGateID = (SEnum::VILLAGE_MAP_ID) g_pData->GetLocationManager()->GetDungeonGateID( eLocalMapID );
							CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eDungeonGateID );
							if( NULL != pVillageTemplet )
							{
								pStatic_Place->GetString(0)->msg = pVillageTemplet->m_Name;
							}
						}
						else
						{
							ASSERT( !"invalid" );
						}
					} break;

				default:
					break;
				}
				pStatic_Place->SetShow(true);
#endif SERV_CHANNEL_PARTY




			}

		}

	}

}






#ifdef HENIR_TEST
void CX2PartyUI::OpenPartyFeverDLG( bool bOpen )
{
	if( false == bOpen )
	{
		if( NULL != m_pDLGPartyFever )
		{
			m_pDLGPartyFever->SetShowEnable( bOpen, bOpen );
		}
	}
	else
	{
		if( NULL == m_pDLGPartyFever )
		{
			m_pDLGPartyFever = new CKTDGUIDialog( m_pCurrStage, L"DLG_Fever.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPartyFever );
		}

		m_pDLGPartyFever->SetShowEnable( bOpen, bOpen );
	}
}



void CX2PartyUI::UpdatePartyFeverDLG()
{
	if( NULL == m_pDLGPartyFever )
		return;

#ifdef HEAVY_LOG_TEST
	ASSERT( NULL != g_pKTDXApp->GetDialogManager_LUA()->GetDialog( L"DLG_Fever" ) );
	if( NULL == g_pKTDXApp->GetDialogManager_LUA()->GetDialog( L"DLG_Fever" ) )
	{
		StateLog( L"null fever dlg" );
		return;
	}

	ASSERT( m_pDLGPartyFever == g_pKTDXApp->GetDialogManager_LUA()->GetDialog( L"DLG_Fever" ) );
	if( m_pDLGPartyFever != g_pKTDXApp->GetDialogManager_LUA()->GetDialog( L"DLG_Fever" ) )
	{
		StateLog( L"invalid fever dlg" );
		return;
	}
#endif HEAVY_LOG_TEST


	CX2PartyManager::PartyData* pMyPartyData = g_pData->GetPartyManager()->GetMyPartyData();

	if( pMyPartyData->GetPartyMemberCount() >= 2 && pMyPartyData->GetPartyMemberCount() <= 4 )
	{
		int iCurrPartyFever = (int) pMyPartyData->m_iPartyFever;
		if( iCurrPartyFever > MAX_PARTY_FEVER )
			iCurrPartyFever = MAX_PARTY_FEVER;

		float fPartyFeverRate = static_cast<float>(pMyPartyData->m_iPartyFever) / static_cast<float>(MAX_PARTY_FEVER);

		CKTDGUIStatic* pStatic_FeverNumber = static_cast<CKTDGUIStatic*>( m_pDLGPartyFever->GetControl( L"Static_Fever_Gauge" ) );
		if( NULL != pStatic_FeverNumber && 
			NULL != pStatic_FeverNumber->GetString(0) )
		{
			WCHAR wszText[64] = L"";
			StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"%d/%d", static_cast<int>(fPartyFeverRate*100), 100 );
			pStatic_FeverNumber->GetString(0)->msg = wszText;
		}



		const float MAGIC_FEVER_GAGE_LENGTH = 228.f; 
		CKTDGUIStatic* pStatic_FeverGage = (CKTDGUIStatic*) m_pDLGPartyFever->GetControl( L"g_pStaticFever" );
		if( NULL != pStatic_FeverGage )
		{
			if( fPartyFeverRate < 1.f )
			{
				pStatic_FeverGage->GetPicture(0)->SetShow( true );
				pStatic_FeverGage->GetPicture(0)->SetSizeX( MAGIC_FEVER_GAGE_LENGTH * fPartyFeverRate );

				pStatic_FeverGage->GetPicture(1)->SetShow( false );
				pStatic_FeverGage->GetPicture(1)->SetSizeX( MAGIC_FEVER_GAGE_LENGTH * fPartyFeverRate );

				pStatic_FeverGage->GetPicture(2)->SetShow( true );
				pStatic_FeverGage->GetPicture(3)->SetShow( false );
			}
			else
			{
				pStatic_FeverGage->GetPicture(0)->SetShow( false );
				pStatic_FeverGage->GetPicture(0)->SetSizeX( MAGIC_FEVER_GAGE_LENGTH * fPartyFeverRate );

				pStatic_FeverGage->GetPicture(1)->SetShow( true );
				pStatic_FeverGage->GetPicture(1)->SetSizeX( MAGIC_FEVER_GAGE_LENGTH * fPartyFeverRate );

				pStatic_FeverGage->GetPicture(2)->SetShow( false );
				pStatic_FeverGage->GetPicture(3)->SetShow( true );
			}
		}
	}
	else
	{
		OpenPartyFeverDLG( false );
	}
}
#endif HENIR_TEST





// 마을간 이동할 때 업데이트 해줘야하는 것들 update
void CX2PartyUI::OnChangeState()
{
	UpdatePartyMenu();
	SetStage( (CKTDXStage*) g_pMain->GetNowState() );
}

void CX2PartyUI::UpdatePartyMenu()
{
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_BATTLE_FIELD:
		{						
#ifdef FIELD_BOSS_RAID // 던전/대전 버튼 제거
			CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
			if( true == battleFieldManager.GetIsBossRaidCurrentField() )
			{
				OpenPartyMenu( false );
				OpenPartyDLG( false );
			}
			else
#endif // FIELD_BOSS_RAID
			{
				OpenPartyMenu( true );
			}

			if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() >= 2 ) 
			{
				OpenPartyFeverDLG( true );
			}
		} break;

	case CX2Main::XS_DUNGEON_GAME:
		{
			OpenPartyMenu( false );
			OpenPartyDLG( false );
			if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() >= 2 ) 
			{
				OpenPartyFeverDLG( true );
			}
		} break;
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
	case CX2Main::XS_DUNGEON_RESULT:
#ifdef SERV_PVP_NEW_SYSTEM
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_RESULT:	
	case CX2Main::XS_TRAINING_GAME:
#endif
		{
			OpenPartyMenu( false );
			OpenPartyDLG( false );
			OpenPartyFeverDLG( false );
		} break;
	}

	UpdatePartyDLG();
	m_timerPartyListRefresh.restart();
}



void CX2PartyUI::OnJoinParty()
{
	if( m_iPickedPartyListSlotIndex < 0 ||
		m_iPickedPartyListSlotIndex >= MAX_PARTY_COUNT_PER_PAGE )
	{
		return;
	}

	if( m_aPartyData[ m_iPickedPartyListSlotIndex ].m_iPartyUID <= 0 )
	{
		return;
	}

	if( m_aPartyData[ m_iPickedPartyListSlotIndex ].m_iMaxPartyMemberCount == m_aPartyData[ m_iPickedPartyListSlotIndex ].m_iPartyMemberCount )
	{
		return;
	}

	// 레벨 제한 체크
	if( m_aPartyData[ m_iPickedPartyListSlotIndex ].m_bIsLimitLevel == true )
	{

#ifdef SERV_DUNGEON_DIFFICULTY_REVISION

		SEnum::DUNGEON_ID eDungeonIDWithDifficulty = 
			(SEnum::DUNGEON_ID) ( m_aPartyData[m_iPickedPartyListSlotIndex].m_iDungeonID + m_aPartyData[m_iPickedPartyListSlotIndex].m_iDungeonDifficulty );

		const CX2Dungeon::DungeonData* pDungeonData	= g_pData->GetDungeonManager()->GetDungeonData( eDungeonIDWithDifficulty );

		if( g_pData->GetSelectUnitLevel() < pDungeonData->m_MinLevel - 2)
#else
		const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) m_aPartyData[m_iPickedPartyListSlotIndex].m_iDungeonID );

		if( g_pData->GetSelectUnitLevel() < pDungeonData->m_MinLevel)
#endif SERV_DUNGEON_DIFFICULTY_REVISION
		{
			// 레벨이 너무 낮다
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_390 ), m_pCurrStage );
			return;
		}
	}

	ASSERT( !L"EGS_JOIN_PARTY_REQ ON JOIN PARTY" );
	
	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;

	g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_REQUESTING_JOIN_PARTY, 
		CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 7.f, GET_STRING( STR_ID_42 ), 
		(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
}

// 08.12.14 김태완 : UIManager 연결작업
void CX2PartyUI::SetLayer(X2_DIALOG_LAYER layer)
{
	if(m_pDLGParty != NULL) g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGParty, layer);
}

bool CX2PartyUI::GetShow()
{
#ifdef SERV_PVP_NEW_SYSTEM
	if( m_bPartyDungeon == true )
	{
		if(m_pDLGParty != NULL)
			return m_pDLGParty->GetShow();
	}
	if( m_bPartyPVP == true )
	{
		if( m_pDLGPartyPVP != NULL )
			return m_pDLGPartyPVP->GetShow();
	}
#else
	if(m_pDLGParty != NULL)
		return m_pDLGParty->GetShow();
#endif
	return false;
}

#ifndef REFORM_NOVICE_GUIDE
//{{ 초심자 가이드 
void CX2PartyUI::SetNoviceGuideStep(int iStep)
{
	m_iNoviceGuideStep = iStep;

	if(m_iNoviceGuideStep == 0 || g_pTFieldGame == NULL || g_pTFieldGame->GetNoviceGuide() == NULL)
		return;

	bool bShowDialog = false;
	if( g_pData != NULL && g_pData->GetUIManager() != NULL &&			
		((g_pData->GetUIManager()->GetUICharInfo() != NULL && 
		g_pData->GetUIManager()->GetUICharInfo()->GetShow() == true) ||
#ifdef SERV_EPIC_QUEST
		(g_pData->GetUIManager()->GetUIQuestNew() != NULL &&
		g_pData->GetUIManager()->GetUIQuestNew()->GetShow() == true) ||
#else
		(g_pData->GetUIManager()->GetUIQuest() != NULL &&
		g_pData->GetUIManager()->GetUIQuest()->GetShow() == true) ||
#endif SERV_EPIC_QUEST

	if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() != CX2NoviceGuide::NGS_STEP5 &&
		g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() != CX2NoviceGuide::NGS_STEP6_1 &&
		g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() != CX2NoviceGuide::NGS_STEP6_2 &&
		g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() != CX2NoviceGuide::NGS_STEP6_3 &&
		g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() != CX2NoviceGuide::NGS_STEP6 )
	{
		m_iNoviceGuideStep = 3;
	}	

	switch(m_iNoviceGuideStep)
	{
	case 0:	// 없음	
		break;
	case 1:	// 시작하기/준비하기
		if( m_pDLGParty != NULL && m_pDLGParty->GetShow() == true && 
#ifndef NEW_SKILL_TREE
			g_pInstanceData != NULL && g_pInstanceData->GetSkillTreeUI() != NULL && g_pInstanceData->GetSkillTreeUI()->GetShow() == false &&
#endif			
			g_pData != NULL &&
			( m_pLocalMapUI == NULL || (m_pLocalMapUI != NULL && m_pLocalMapUI->GetShowLocalMap() == false) ) &&
			g_pData->GetLocationManager() &&
			g_pData->GetLocationManager()->GetCurrentVillageID() == SEnum::VMI_DUNGEON_GATE_RUBEN  &&
			bShowDialog == false )
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(7);
		else
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(0);
		break;

	case 2:	// 던전나가기
		if( m_pDLGParty != NULL && m_pDLGParty->GetShow() == true && 
#ifndef NEW_SKILL_TREE
			g_pInstanceData != NULL && g_pInstanceData->GetSkillTreeUI() != NULL && g_pInstanceData->GetSkillTreeUI()->GetShow() == false &&
#endif			
			bShowDialog == false && 
			( m_pLocalMapUI == NULL || (m_pLocalMapUI != NULL && m_pLocalMapUI->GetShowLocalMap() == false) ) ) 
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(8);
		else
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(0);
		break;
		break;
	case 3:	// 가이드 제거
		g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(0);
		m_iNoviceGuideStep = 0;
		break;

	case 4:	// 맵선택
		if( m_pDLGParty != NULL && m_pDLGParty->GetShow() == true && 
#ifndef NEW_SKILL_TREE
			g_pInstanceData != NULL && g_pInstanceData->GetSkillTreeUI() != NULL && g_pInstanceData->GetSkillTreeUI()->GetShow() == false &&
#endif			
			g_pData != NULL &&
			g_pData->GetPartyManager()->DoIHaveParty() == false &&			
			g_pData->GetLocationManager() &&
			g_pData->GetLocationManager()->GetCurrentVillageID() == SEnum::VMI_DUNGEON_GATE_RUBEN &&
			bShowDialog == false )
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(9);
		else
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(0);
		break;
	case 5:	// 던전선택
		if( 
#ifndef NEW_SKILL_TREE
			g_pInstanceData != NULL && g_pInstanceData->GetSkillTreeUI() != NULL && g_pInstanceData->GetSkillTreeUI()->GetShow() == false &&
#endif
			m_pDLGParty != NULL && m_pDLGParty->GetShow() == true && 
			g_pData != NULL && 
			g_pData->GetPartyManager()->DoIHaveParty() == false &&
			g_pData->GetLocationManager() &&
			g_pData->GetLocationManager()->GetCurrentVillageID() == SEnum::VMI_DUNGEON_GATE_RUBEN &&
			bShowDialog == false )
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(10);
		else
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(0);
		break;
	case 6:	// 파티만들기
		if( 
#ifndef NEW_SKILL_TREE
			g_pInstanceData != NULL && g_pInstanceData->GetSkillTreeUI() != NULL && g_pInstanceData->GetSkillTreeUI()->GetShow() == false &&
#endif
			m_pDLGParty != NULL && m_pDLGParty->GetShow() == true && 
			g_pData != NULL && 
			g_pData->GetPartyManager()->DoIHaveParty() == false &&
			g_pData->GetLocationManager() &&
			g_pData->GetLocationManager()->GetCurrentVillageID() == SEnum::VMI_DUNGEON_GATE_RUBEN &&
			bShowDialog == false )
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(11);
		else
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(0);
		break;
	}
}
#endif //REFORM_NOVICE_GUIDE

// @bHaveParty : true이면 파티장이 게임 시작할 수 있는지 검사
// @bReadyOnly : true이면 파티원이 레디할 수 있는지만 검사
bool CX2PartyUI::IsGameStartable( CX2LocationManager::LOCAL_MAP_ID eCurrLocalMapID, const CX2Dungeon::DungeonData* pDungeonData, bool bHaveParty, bool bPartyLeader )
{
	// 운영자 이상은 모든 던전 그냥 시작할 수 있게
	if( NULL != g_pData->GetMyUser() &&
		g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		return true;
	}
	
	bool bDungeonGate = g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() );
	bool bDungeonLounge = g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() );
	bool bPlayPossileDungeon = bDungeonGate || bDungeonLounge;

	// 모든 파티원이 레디상태이고 던전에 맞는 던전게이트에 있다면 게임을 시작할 수 있다
	bool bGameStartable = true;

	if( true == bGameStartable )
	{
		if( false == g_pData->GetPartyManager()->IsDungeonInLocal( eCurrLocalMapID, g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) )
		{
			bGameStartable = false;
		}
	}

	if( true == bGameStartable )
	{
		if( bPlayPossileDungeon == false )
		{
			bGameStartable = false;
		}
	}

	//if( true == bHaveParty &&
	//	true == bPartyLeader &&
	//	true == bGameStartable &&
	//	false == g_pData->GetPartyManager()->IsAllReady() )
	//{
	//	bGameStartable = false;
	//}


	if( (false == bHaveParty || true == bPartyLeader) &&
		true == bGameStartable &&
		false == g_pData->GetDungeonManager()->IsActiveDungeon( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID, g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty ) )
	{
		bGameStartable = false;
	}

#if 0 // 입장권여부로 시작하기/준비하기 버튼 비활성화 시키지 않는다.
	if( true == bGameStartable &&
		pDungeonData->m_RequireItemCount > 0 &&
		g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( pDungeonData->m_RequireItemID ) < pDungeonData->m_RequireItemCount )
	{
		bGameStartable = false;
	}
#endif

	return bGameStartable;
}

//{{ 09.03.06 김태완
void CX2PartyUI::EnableDialog(bool bEnable)
{
	//if(NULL != m_pDLGPartyList) m_pDLGPartyList->SetEnable(bEnable);
	if(NULL != m_pDLGParty) m_pDLGParty->SetEnable(bEnable);
	//if(NULL != m_pDLGPartyMember) m_pDLGPartyMember->SetEnable(bEnable);
   
}
//}}

bool CX2PartyUI::GetShowLocalMap()
{
	if(m_pLocalMapUI == NULL)
		return false;

	return m_pLocalMapUI->GetShowLocalMap();
}

void CX2PartyUI::PlayWarningAllReady()
{
	if( m_pDLGWarningAllReady == NULL )
	{
		//m_vecDungeonID.reserve( 16 );
		m_pDLGWarningAllReady	= new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_Party_All_Ready.lua", 0.0f );
        g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWarningAllReady );
		m_pDLGWarningAllReady->SetColor( D3DXCOLOR(1,1,1,0) );
	}

	if ( m_pDLGWarningAllReady != NULL )
	{
		m_pDLGWarningAllReady->SetShowEnable(true, true);

		CKTDGUIStatic* pStatic_AllReady = (CKTDGUIStatic*) m_pDLGWarningAllReady->GetControl( L"Party_All_Ready" );
		if( pStatic_AllReady != NULL && pStatic_AllReady->GetPicture(2) != NULL )
			pStatic_AllReady->GetPicture(2)->SetShow(false);

		if ( m_pDLGWarningAllReady->GetIsMoving() == false )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGWarningAllReady, true );
			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,1), 0.5f, true );
			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,1), 2.0f );
			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,0), 1.0f );
			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,0), 4.0f );
		}
	}
}

void CX2PartyUI::StopWarningAllReady()
{
	if ( m_pDLGWarningAllReady != NULL )
	{
		m_pDLGWarningAllReady->SetShowEnable(false, false);
		m_pDLGWarningAllReady->MoveStop();
		m_pDLGWarningAllReady->SetColor( D3DXCOLOR( 1,1,1,0 ) );
	}
}

bool CX2PartyUI::IsReadyPossible()
{
	// 개인거래창이 열려있으면 레디 불가
	if(g_pData->GetUIManager() != NULL)
	{
		if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE) )
			return false;
	}
#ifndef NEW_SKILL_TREE
	// 스킬창이 열려있으면 레디 불가
	if(g_pInstanceData->GetSkillTreeUI() != NULL && 
		g_pInstanceData->GetSkillTreeUI()->GetShow() == true)
	{
		return false;
	}
#endif
	// 캐쉬샵이 열려있으면 레디 불가
	if( NULL != g_pData->GetCashShop() && g_pData->GetCashShop()->GetOpen() )
	{
		return false;
	}

	// NPC와 대화중이면 레디 불가
	if(g_pTFieldGame != NULL &&
		g_pTFieldGame->GetJoinNpc() == true )
	{
		return false;
	}

#ifdef ATTRACTION_ITEM_TEST
	// 가열기 열렸으면 레디 불가
	if(g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL)
	{
		if(g_pData->GetUIManager()->GetUIInventory()->GetPlayAttractionItem() == true)
		{
			return false;
		}
	}
#endif
#ifdef SERV_TREASURE_BOX_ITEM
	// 트레져박스 열렸으면 레디 불가
	if(g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL)
	{
		if(g_pData->GetUIManager()->GetUIInventory()->GetPlayTreasureBox() == true)
		{
			return false;
		}
	}
#endif SERV_TREASURE_BOX_ITEM

	return true;
}

void CX2PartyUI::SetStage( CKTDXStage* val )
{
	m_pCurrStage = val; 

	if( NULL != m_pDLGParty )
		m_pDLGParty->SetStage( val );

	if( NULL != m_pDLGPartyListMeberPreview )
		m_pDLGPartyListMeberPreview->SetStage( val );

	if( NULL != m_pDLGWarningAllReady )
		m_pDLGWarningAllReady->SetStage( val );

	if( NULL != m_pDLGPartyFever )
		m_pDLGPartyFever->SetStage( val );


	if( NULL != m_pLocalMapUI )
		m_pLocalMapUI->SetStage( val );
}

#ifdef SERV_PVP_NEW_SYSTEM
void CX2PartyUI::OpenPartyMenu(bool bOpen)
{
	if( m_pDLGPartyMenu == NULL )
	{
		m_pDLGPartyMenu = new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_PARTY_BOTTON_NEW.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPartyMenu );
		m_pDLGPartyMenu->SetDisableUnderWindow(true);
#ifdef REFORM_QUEST
		//던전 시작 버튼 위의 Q마크
		CKTDGUIStatic *pStaticGuide = static_cast<CKTDGUIStatic*>(m_pDLGPartyMenu->GetControl(L"ButtonQuestGuide"));
		if( NULL != pStaticGuide && NULL != pStaticGuide->GetPicture(0) )
		{
			pStaticGuide->GetPicture(0)->SetFlicker( 0.5f, 1.0f, 0.1f );
		}
#endif //REFORM_QUEST
	}	

	if( NULL == m_pDungeonCheckBox )
		m_pDungeonCheckBox = static_cast<CKTDGUICheckBox*>(m_pDLGPartyMenu->GetControl(L"g_pCheckBox_DUNGEON"));

	if( NULL == m_pPVPCheckBox )
		m_pPVPCheckBox = static_cast<CKTDGUICheckBox*>(m_pDLGPartyMenu->GetControl(L"g_pCheckBox_PVP"));

	m_pDLGPartyMenu->SetShowEnable( bOpen, bOpen );

	if( false == bOpen )
		return;

	if( true != g_pData->GetPartyManager()->GetProcessDungeonMatch() &&
		true != g_pData->GetPartyManager()->GetProcessPvpMatch() )
	{
		SetDungeonButton(BUTTON_START, false);
		SetPVPButton(BUTTON_START, false);
	}
}
void CX2PartyUI::OpenPvpSetting(bool bOpen, bool bMap)
{
	if( m_pDlgPartyPvpMapSetting == NULL )
	{
		m_pDlgPartyPvpMapSetting = new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_PVP_SETTING_MAP_NEW.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgPartyPvpMapSetting );
		m_pDlgPartyPvpMapSetting->SetDisableUnderWindow( true );

		// 대전맵 리스트 로드&셋팅
		LoadPvpMapUI();	
	}
// 	if( m_pDlgPartyPvpEtcSetting == NULL )
// 	{
// 		m_pDlgPartyPvpEtcSetting = new CKTDGUIDialog( m_pCurrStage, L"DLG_UI_PVP_SETTING_ETC.lua" );
// 		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgPartyPvpEtcSetting );
// 		m_pDlgPartyPvpEtcSetting->SetDisableUnderWindow( true );
// 		m_pDlgPartyPvpEtcSetting->SetPos( D3DXVECTOR2( 180.f, 0.f ) );
// 	}

	if( bOpen == false )
	{
		m_pDlgPartyPvpMapSetting->SetShowEnable( false, false );

		if( m_pDLGPartyMenu != NULL )
			m_pDLGPartyMenu->SetEnable(true);
		if( m_pDLGPartyPVP != NULL )
			m_pDLGPartyPVP->SetEnable(true);
		
	}
	else
	{
		if( m_pDLGPartyMenu != NULL )
			m_pDLGPartyMenu->SetEnable(false);
		if( m_pDLGPartyPVP != NULL )
			m_pDLGPartyPVP->SetEnable(false);

		if( bMap == true )
		{		
			m_pDlgPartyPvpMapSetting->SetShowEnable( bOpen, bOpen );
			UpdateMapInfo(0);
		}
		else
		{
			m_pDlgPartyPvpMapSetting->SetShowEnable( !bOpen, !bOpen );
		}
	}
}

void CX2PartyUI::ResetePvpMapSetting()
{
	if( m_pDlgPartyPvpMapSetting == NULL )
		return;

	for(int i=0; i<(int)m_vecPvpMapInfo.size(); ++i)
	{
		WCHAR wMapId[256] = {0,};
		StringCchPrintf(wMapId, 256, L"Map_CheckBox_%d", i);
		CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDlgPartyPvpMapSetting->GetControl( wMapId );
		pCheckBox->SetChecked(false);
		for(int j=0; j<(int)g_pMain->GetGameOption().GetOptionList().m_vecPvpMap.size(); ++j)
		{
			if( m_vecPvpMapInfo[i].m_WorldID == g_pMain->GetGameOption().GetOptionList().m_vecPvpMap[j] )
			{
				pCheckBox->SetChecked(true);
				break;
			}
		}
	}
}

void CX2PartyUI::SavePvpMapSetting()
{
	g_pMain->GetGameOption().GetOptionList().m_vecPvpMap.clear();
	for(int i=0; i<(int)m_vecPvpMapInfo.size(); ++i)
	{
		WCHAR wMapId[256] = {0,};
		StringCchPrintf(wMapId, 256, L"Map_CheckBox_%d", i);
		CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDlgPartyPvpMapSetting->GetControl( wMapId );
		if( pCheckBox->GetChecked() == true )
		{
			g_pMain->GetGameOption().GetOptionList().m_vecPvpMap.push_back( m_vecPvpMapInfo[i].m_WorldID );
		}
	}

	g_pMain->GetGameOption().SaveScriptFile();
}

void CX2PartyUI::LoadPvpMap()
{	
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	g_pKTDXApp->LoadAndDoMemory( &luaManager, L"PVP_Map_List.lua"	);

	//KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	//TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	if( luaManager.BeginTable( "PVP_MAP_LIST" ) == true )
	{
		m_vecPvpMapInfo.clear();

		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			PVP_MAP_INFO stPvpMapInfo;

			LUA_GET_VALUE( luaManager, "m_WorldID",				stPvpMapInfo.m_WorldID,				0 );
			
			int iStringIndex;
			LUA_GET_VALUE( luaManager, "m_MapName",				iStringIndex,           			STR_ID_EMPTY );
			stPvpMapInfo.m_MapName = GET_STRING( iStringIndex );
			LUA_GET_VALUE( luaManager, "m_MapSize",				iStringIndex,           			STR_ID_EMPTY );
			stPvpMapInfo.m_MapSize = GET_STRING( iStringIndex );
			LUA_GET_VALUE( luaManager, "m_MapDesc",				iStringIndex,           			STR_ID_EMPTY );
			stPvpMapInfo.m_MapDesc = GET_STRING( iStringIndex );
			LUA_GET_VALUE( luaManager, "m_MapTexName",			stPvpMapInfo.m_MapTexName,			L"" );
			LUA_GET_VALUE( luaManager, "m_MapTexKeyName",		stPvpMapInfo.m_MapTexKeyName,		L"" );

			m_vecPvpMapInfo.push_back(stPvpMapInfo);

			++index;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}
}

void CX2PartyUI::LoadPvpMapUI()
{
	if( m_pDlgPartyPvpMapSetting == NULL )
		return;

	CKTDGUIContolList *pControlList = (CKTDGUIContolList*)m_pDlgPartyPvpMapSetting->GetControl(L"PvpMapControlList");
	if( pControlList != NULL )
	{
		for(int i=0; i<(int)m_vecPvpMapInfo.size(); ++i)
		{
			AddPvpMapList( pControlList, i, i );
		}
		pControlList->SetIndex(0, 0);

		//한번에 표현할 수 있다면 스크롤바 보여주지 않기
		if( 12 >= (int)m_vecPvpMapInfo.size() )
			pControlList->SetShowEnableYScroolBar(false,false);
	}

	ResetePvpMapSetting();
}

void CX2PartyUI::AddPvpMapList(CKTDGUIContolList *pControlList, int row, int pvpMapInfoIndex)
{
	if( m_pDlgPartyPvpMapSetting == NULL )
		return;

	WCHAR wMapId[256] = {0,};
	
	PVP_MAP_INFO stPvpMapInfo = m_vecPvpMapInfo[pvpMapInfoIndex];

	m_pDlgPartyPvpMapSetting->OpenScriptFile(L"DLG_PvpMap_ControlList.lua");
	CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDlgPartyPvpMapSetting->GetControl( L"MAP_NAME" );	
	CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDlgPartyPvpMapSetting->GetControl( L"ListControl_Button" );
	CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDlgPartyPvpMapSetting->GetControl( L"Map_CheckBox" );
	
	StringCchPrintf(wMapId, 256, L"Map_Static_%d", pvpMapInfoIndex);
	pStatic->SetName( wMapId );
	pStatic->AddDummyInt(pvpMapInfoIndex);

#ifdef ELLIPSE_GLOBAL
	bool bEllipse = false;

#ifdef LINE_COUNT_FOR_BR
	pStatic->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(stPvpMapInfo.m_MapName.c_str(), 120, pStatic->GetString(0)->fontIndex, 1, bEllipse);
#else // LINE_COUNT_FOR_BR
	pStatic->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(stPvpMapInfo.m_MapName.c_str(), 160, pStatic->GetString(0)->fontIndex, 1, bEllipse);
#endif // LINE_COUNT_FOR_BR

	if( pButton != NULL )
	{
		if(bEllipse == true)
		{
				pButton->SetGuideDesc( stPvpMapInfo.m_MapName.c_str() );
		}
	}
#else
	pStatic->GetString(0)->msg = stPvpMapInfo.m_MapName; // 대전맵 이름
#endif ELLIPSE_GLOBAL
	StringCchPrintf(wMapId, 256, L"Map_Button_%d", pvpMapInfoIndex);
	pButton->SetName( wMapId );
	pButton->AddDummyInt(pvpMapInfoIndex);
	pButton->SetDownStateAtNormal(false);
	StringCchPrintf(wMapId, 256, L"Map_CheckBox_%d", pvpMapInfoIndex);
	pCheckBox->SetName( wMapId );
	pCheckBox->AddDummyInt(pvpMapInfoIndex);	
	
	pControlList->InsertItem( pStatic,	row, 0 );
	pControlList->InsertItem( pButton,	row, 1 );
	pControlList->InsertItem( pCheckBox,row, 2 );	
}

void CX2PartyUI::UpdateMapInfo(int iMapIndex)
{
	if( m_pDlgPartyPvpMapSetting == NULL || m_pDlgPartyPvpMapSetting->GetShow() == false )
		return;

	wstring wstrTexName, wstrTexKeyName, wstrMapName, wstrMapSize, wstrMapDesc;
	if( iMapIndex < 0 || iMapIndex >= (int)m_vecPvpMapInfo.size() )
	{
		wstrTexName = L"pvpbg.jpg";
		wstrTexKeyName = L"";
		wstrMapName = L"";
		wstrMapSize = L"";
		wstrMapDesc = L"";
	}
	else
	{
		wstrTexName =m_vecPvpMapInfo[iMapIndex].m_MapTexName;
		wstrTexKeyName =  m_vecPvpMapInfo[iMapIndex].m_MapTexKeyName;
		wstrMapName =  m_vecPvpMapInfo[iMapIndex].m_MapName;
		wstrMapSize =  m_vecPvpMapInfo[iMapIndex].m_MapSize;		
		wstrMapDesc =  m_vecPvpMapInfo[iMapIndex].m_MapDesc;
	}

	CKTDGUIStatic *pViewImgStatic = (CKTDGUIStatic*)m_pDlgPartyPvpMapSetting->GetControl(L"SETTING_MAP");
	if( pViewImgStatic != NULL )
	{
		CKTDGUIControl::CPictureData *pPicture = pViewImgStatic->GetPictureIndex(1);
		if( pPicture != NULL )
		{
			if( wstrTexKeyName.empty() == false )
				pPicture->SetTex( wstrTexName.c_str(), wstrTexKeyName.c_str() );
			else
				pPicture->SetTex( wstrTexName.c_str() );
		}
		pViewImgStatic->GetString(0)->msg = GET_STRING(STR_ID_12342);
	}

	CKTDGUIStatic *pViewViewStatic = (CKTDGUIStatic*)m_pDlgPartyPvpMapSetting->GetControl(L"VIEW");
	if( pViewViewStatic != NULL )
	{
		pViewViewStatic->GetString(0)->msg = GET_STRING(STR_ID_12343);
		pViewViewStatic->GetString(1)->msg = GET_STRING(STR_ID_12344);		
		pViewViewStatic->GetString(2)->msg = GET_STRING(STR_ID_12345);
#ifdef HARDCODING_STRING_TO_INDEX
		pViewViewStatic->GetString(3)->msg = GET_STRING(STR_ID_24586);
#else
		pViewViewStatic->GetString(3)->msg = L"맵설정";
#endif HARDCODING_STRING_TO_INDEX
	}

	CKTDGUIStatic *pViewBodyStatic = (CKTDGUIStatic*)m_pDlgPartyPvpMapSetting->GetControl(L"BODY");
	if( pViewBodyStatic != NULL )
	{
#ifdef CLIENT_GLOBAL_LINEBREAK
		pViewBodyStatic->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2Main( wstrMapName.c_str(), 145, pViewBodyStatic->GetString(0)->fontIndex );
		pViewBodyStatic->GetString(1)->msg = wstrMapSize;
		
#ifdef LINE_COUNT_FOR_BR
		pViewBodyStatic->GetString(2)->msg = CWordLineHandler::GetStrByLineBreakInX2Main( wstrMapDesc.c_str(), 165, pViewBodyStatic->GetString(2)->fontIndex );
#else // LINE_COUNT_FOR_BR
		pViewBodyStatic->GetString(2)->msg = CWordLineHandler::GetStrByLineBreakInX2Main( wstrMapDesc.c_str(), 185, pViewBodyStatic->GetString(2)->fontIndex );
#endif // LINE_COUNT_FOR_BR		

#else //CLIENT_GLOBAL_LINEBREAK
		pViewBodyStatic->GetString(0)->msg = wstrMapName;
		pViewBodyStatic->GetString(1)->msg = wstrMapSize;		
		pViewBodyStatic->GetString(2)->msg = g_pMain->GetStrByLienBreak( wstrMapDesc.c_str(), 110, pViewBodyStatic->GetString(2)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
	}

	int iCheckedCount = 0;
	for(int i=0; i<(int)m_vecPvpMapInfo.size(); ++i)
	{
		WCHAR wMapId[256] = {0,};
		StringCchPrintf(wMapId, 256, L"Map_CheckBox_%d", i);
		CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDlgPartyPvpMapSetting->GetControl( wMapId );
		if( pCheckBox->GetChecked() == true )
		{
			++iCheckedCount;
		}
	}
	pViewImgStatic->GetString(1)->msg = GET_REPLACED_STRING( ( STR_ID_12353, "i", 2 - iCheckedCount ) );
}

void CX2PartyUI::UpdatePvpPartyDlg()
{
	CreatePartyDlg();

	if( NULL == m_pDLGPartyPVP )
		return;


	CKTDGUIButton *pButtonStartMatch = (CKTDGUIButton*)m_pDLGPartyPVP->GetControl(L"StartMatch"); 
	if( NULL != pButtonStartMatch )
	{
		pButtonStartMatch->SetShowEnable(true, true);
	}	
	CKTDGUIButton *pButtonCloseWindow = (CKTDGUIButton*)m_pDLGPartyPVP->GetControl(L"Close_Window"); 
	if( NULL != pButtonCloseWindow )
	{
		pButtonCloseWindow->SetShowEnable(true, true);
	}
}

bool CX2PartyUI::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( m_pDlgPartyPvpMapSetting != NULL && m_pDlgPartyPvpMapSetting->GetShow() == true )
		//g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgPartyPvpMapSetting ) == true )
	{
#ifdef FREE_WARP
		switch( uMsg )
		{
		case WM_MOUSEWHEEL:
			{ 
				if( m_pDlgPartyPvpMapSetting != NULL && m_pDlgPartyPvpMapSetting->GetIsMouseOver() ) 
				{
					CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDlgPartyPvpMapSetting->GetControl( L"PvpMapControlList" );
					if( pControlList == NULL )
						return false;
					CKTDGUIScrollBar* pScroolBarY = pControlList->GetYScroolBar();
					if(pScroolBarY == NULL)
						return false;

					short zDelta = (short)HIWORD(wParam);
					m_SumDelta += zDelta;

					while (abs(m_SumDelta) >= WHEEL_DELTA)
					{
						if(m_SumDelta>0)
						{
							pScroolBarY->Scroll(-1);
							m_SumDelta -= WHEEL_DELTA;
						}
						else
						{	
							pScroolBarY->Scroll(1);
							m_SumDelta += WHEEL_DELTA;
						}
					}
				}
			}break;
		}
#endif
		return false;
	}

	return false;
}

void CX2PartyUI::CheckPvpStart()
{
	g_pData->GetPartyManager()->RemovePvpPartyMsg();

	if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SKILL ) == true )
	{
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL );
	}

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PVP_START,
		CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 10.f, GET_STRING( STR_ID_12718 ), 
		(CKTDXStage*) g_pMain->GetNowState(), 
		CX2PartyUI::PUCM_PVP_CHECK_START_OK, CX2PartyUI::PUCM_PVP_CHECK_START_CANCEL, CX2PartyUI::PUCM_PVP_CHECK_START_CANCEL );
}

void CX2PartyUI::GetSelectedPvpMap(std::set<short> &selectedMap)
{
	selectedMap.clear();

	if( m_pDlgPartyPvpMapSetting == NULL )
	{
		// default map load
		for(int iSelectIndex=0; iSelectIndex<(int)m_vecDefaultMap.size(); ++iSelectIndex)
		{
			int iDefaultMapIndex = m_vecDefaultMap[iSelectIndex];
			for(int i=0; i<(int)m_vecPvpMapInfo.size(); ++i)
			{
				if( i == iDefaultMapIndex )
				{
					PVP_MAP_INFO &pvpMapInfo = m_vecPvpMapInfo[i];
					selectedMap.insert( (short)(pvpMapInfo.m_WorldID) );
					break;
				}
			}			
		}

		return;
	}

	for(int i=0; i<(int)m_vecPvpMapInfo.size(); ++i)
	{
		WCHAR wMapId[256] = {0,};
		StringCchPrintf(wMapId, 256, L"Map_CheckBox_%d", i);
		CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDlgPartyPvpMapSetting->GetControl( wMapId );
		if( pCheckBox->GetChecked() == true )
		{
			PVP_MAP_INFO &pvpMapInfo = m_vecPvpMapInfo[i];
			selectedMap.insert( (short)(pvpMapInfo.m_WorldID) );
		}
	}
}

char CX2PartyUI::GetPvpMatchMode()
{
	if( m_pDLGPartyPVP == NULL )
		return 0;

	if( GetPvpMatchPlayerNum() > 1 )
	{
		return 1;
	}
		
	return 0;
}

char CX2PartyUI::GetPvpMatchPlayerNum()
{
	if( m_pDLGPartyPVP == NULL )
		return 0;

	CKTDGUIRadioButton* pChecked1_1 = (CKTDGUIRadioButton*)m_pDLGPartyPVP->GetControl(L"pvp_player_check1");
#ifndef PVP_SEASON2
	CKTDGUIRadioButton* pChecked1_2 = (CKTDGUIRadioButton*)m_pDLGPartyPVP->GetControl(L"pvp_player_check2");
#endif
	CKTDGUIRadioButton* pChecked1_3 = (CKTDGUIRadioButton*)m_pDLGPartyPVP->GetControl(L"pvp_player_check3");

	if( pChecked1_1->GetChecked() == true )
		return 1;
#ifndef PVP_SEASON2
	else if( pChecked1_2->GetChecked() == true )
		return 2;
#endif
	else if( pChecked1_3->GetChecked() == true )
		return 3;

	return 1;
}

void CX2PartyUI::SetPvpMatchPlayerNum(int iVal)
{
	if( NULL == m_pDLGPartyPVP )
		return;

	switch(iVal)
	{
	case 1:
		{
			CKTDGUIRadioButton* pChecked = (CKTDGUIRadioButton*)m_pDLGPartyPVP->GetControl(L"pvp_player_check1");
			if( NULL != pChecked )
				pChecked->SetChecked( true );
		}
		break;
	case 2:
#ifndef PVP_SEASON2
		{
			CKTDGUIRadioButton* pChecked = (CKTDGUIRadioButton*)m_pDLGPartyPVP->GetControl(L"pvp_player_check2");
			if( NULL != pChecked )
				pChecked->SetChecked( true );
		}
		break;
#endif
	case 3:
		{
			CKTDGUIRadioButton* pChecked = (CKTDGUIRadioButton*)m_pDLGPartyPVP->GetControl(L"pvp_player_check3");
			if( NULL != pChecked)
				pChecked->SetChecked( true );
		}
		break;
	default:
		break;
	}

	ShowPvpModeDesc();
}

void CX2PartyUI::ShowPvpModeDesc()
{
	int iNum = GetPvpMatchPlayerNum();
	wstring wstrPvpModeDesc = L"";

	switch(iNum)
	{
	case 1:
		wstrPvpModeDesc = GET_STRING( STR_ID_13717 );
		break;
#ifndef PVP_SEASON2
	case 2:
		wstrPvpModeDesc = GET_STRING( STR_ID_13718 );
		break;
#endif
	case 3:
		wstrPvpModeDesc = GET_STRING( STR_ID_13719 );
		break;
	default:
		break;
	}
	CKTDGUIStatic *pViewBodyStatic = (CKTDGUIStatic*)m_pDLGPartyPVP->GetControl(L"PVP");
	if( pViewBodyStatic != NULL )
	{
#ifdef PVP_SEASON2
#ifdef CLIENT_GLOBAL_LINEBREAK
		pViewBodyStatic->GetString(3)->msg = CWordLineHandler::GetStrByLineBreakInX2Main( wstrPvpModeDesc.c_str(), 140, pViewBodyStatic->GetString(1)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
		pViewBodyStatic->GetString(3)->msg = g_pMain->GetStrByLienBreak( wstrPvpModeDesc.c_str(), 180, pViewBodyStatic->GetString(1)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
#else
#ifdef CLIENT_GLOBAL_LINEBREAK
		pViewBodyStatic->GetString(4)->msg = CWordLineHandler::GetStrByLineBreakInX2Main( wstrPvpModeDesc.c_str(), 140, pViewBodyStatic->GetString(2)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
		pViewBodyStatic->GetString(4)->msg = g_pMain->GetStrByLienBreak( wstrPvpModeDesc.c_str(), 140, pViewBodyStatic->GetString(2)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
#endif
	}
}

void CX2PartyUI::InitPvpSetting()
{	
	LoadPvpMap();
}

#endif //SERV_PVP_NEW_SYSTEM


void CX2PartyUI::StartToWaitForDungeonPartyMatching()
{
	if( NULL != m_pDLGParty)
		m_pDLGParty->SetShowEnable( false, false );

#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

	SetShowEnableRewardItemInfoDlg( false, false, false );

#else  // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	if( NULL != m_pDLGQuestRewardItemInfo )
	{
		m_pDLGQuestRewardItemInfo->SetShow ( false );
	}
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

	m_pLocalMapUI->OpenLocalMapDLG(false);
	m_bShowQuestMark = false;	//던전시작 버튼 출력 여부에 따라 같이 붙는 Q마크도 설정

	SetDungeonButton(BUTTON_WAIT); //던전대기	
	SetPVPButton(BUTTON_CANCEL);   //대기취소
	SetCurrentWaitUserCount( 1 );
}

void CX2PartyUI::CancelToWaitForDungeonPartyMatching()
{
	//던전시작 버튼 출력 여부에 따라 같이 붙는 Q마크도 설정
	m_bShowQuestMark = true;

	SetDungeonButton(BUTTON_START, false); //던전시작
	SetPVPButton(BUTTON_START, false); //대전시작;
}

void CX2PartyUI::StartToWaitForPvpMatching()
{
	m_pDLGPartyPVP->SetShowEnable( false, false );
	//던전시작 버튼 출력 여부에 따라 같이 붙는 Q마크도 설정
	m_bShowQuestMark = false;

	SetPVPButton(BUTTON_WAIT);		// 대전 대기
	SetDungeonButton(BUTTON_CANCEL);// 대기 취소	
	SetCurrentWaitUserCount( -1, false );
}

void CX2PartyUI::CancelToWaitForPvpMatching()
{
	SetDungeonButton(BUTTON_START, false); //던전시작
	SetPVPButton(BUTTON_START, false); //대전시작;
}


#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

CKTDGUIDialogType CX2PartyUI::CreateRewardItemInfoDlg ( const WCHAR * wDlgName )
{
	CKTDGUIDialogType tempDialog = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_SlotManager.lua" );
	if ( NULL != tempDialog )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( tempDialog );

		CKTDGUIStatic* pStatic = new CKTDGUIStatic();
		pStatic->SetName( wDlgName );
		tempDialog->AddControl( pStatic );
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
		pStatic->AddPicture( pPicture );
		pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
		pPicture->SetTex( L"FadeInOut.dds" );


		CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
		pStatic->AddString( pString );
		pString->fontIndex = XUF_DODUM_11_NORMAL;
		pString->color = D3DXCOLOR( 1,1,1,1 );
		pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
		pString->msg = L"";
		pString->fontStyle = CKTDGFontManager::FS_NONE;
		pString->sortFlag = DT_LEFT;
		pString->pos = D3DXVECTOR2( 20, 20 );

		return tempDialog;
	}
	return NULL;
}



void CX2PartyUI::SetShowEnableRewardItemInfoDlg ( bool bOpenMajor, bool bOpenMinor, bool bEnableFlag )
{
	if ( NULL != m_pDLGQuestRewardItemInfo )
	{
		if ( true == bEnableFlag )
		{
			m_pDLGQuestRewardItemInfo->SetShowEnable( bOpenMajor, bOpenMajor );
		}
		else
		{
			m_pDLGQuestRewardItemInfo->SetShow ( bOpenMajor );
		}
	}

	if ( NULL != m_pDLGQuestRewardItemInfoAdd )
	{
		if ( true == bEnableFlag )
		{
			m_pDLGQuestRewardItemInfoAdd->SetShowEnable ( bOpenMinor, bOpenMinor );
		}
		else
		{
			m_pDLGQuestRewardItemInfoAdd->SetShow ( bOpenMinor );
		}
	}
}

void CX2PartyUI::ChangeRewardItemInfoDlgLayer ( bool bOpen, int iLayerToChange )
{
	if ( NULL != m_pDLGQuestRewardItemInfo && NULL != m_pDLGQuestRewardItemInfoAdd )
	{		
		if( true == bOpen )
		{
			SetShowEnableRewardItemInfoDlg ( bOpen, bOpen, true );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGQuestRewardItemInfo, iLayerToChange );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGQuestRewardItemInfoAdd, iLayerToChange );
		}
		else		// false == bOpen 
		{

		}
	}
}


#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

void CX2PartyUI::OpenRewardItemInfoPopup ( bool bOpen, D3DXVECTOR2 vPos, int iItemID )
{
	if( false == m_pDLGParty->GetShow()  && true == bOpen )
	{
		return;
	}
	// 
	try
	{		
	
#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

		if ( true == bOpen) 
		{
			if( NULL == m_pDLGQuestRewardItemInfo )
			{
				m_pDLGQuestRewardItemInfo = CreateRewardItemInfoDlg(L"ItemDesc");
			}

			if ( NULL == m_pDLGQuestRewardItemInfoAdd )
			{
				m_pDLGQuestRewardItemInfoAdd = CreateRewardItemInfoDlg(L"ItemDescAdd");
			}		
		}	
		
		if( NULL != m_pDLGQuestRewardItemInfo && NULL != m_pDLGQuestRewardItemInfoAdd )
		{
			SetShowEnableRewardItemInfoDlg ( bOpen, bOpen, true );

			if( true == bOpen )
			{
				int iLayer = XDL_POP_UP + 2;
				ChangeRewardItemInfoDlgLayer( bOpen, iLayer );
			}
		}		
		else
		{
			return;
		}

#else // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
		if( m_pDLGQuestRewardItemInfo == NULL && true == bOpen )
		{
			m_pDLGQuestRewardItemInfo = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_SlotManager.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGQuestRewardItemInfo );

			CKTDGUIStatic* pStatic = new CKTDGUIStatic();
			pStatic->SetName( L"ItemDesc" );
			m_pDLGQuestRewardItemInfo->AddControl( pStatic );
			CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
			pStatic->AddPicture( pPicture );
			pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
			pPicture->SetTex( L"FadeInOut.dds" );


			CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
			pStatic->AddString( pString );
			pString->fontIndex = XUF_DODUM_13_SEMIBOLD;
			pString->color = D3DXCOLOR( 1,1,1,1 );
			pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
			pString->msg = L"";
			pString->fontStyle = CKTDGFontManager::FS_NONE;
			pString->sortFlag = DT_LEFT;
			pString->pos = D3DXVECTOR2( 20, 20 );
		}

		if( m_pDLGQuestRewardItemInfo != NULL )
		{
			m_pDLGQuestRewardItemInfo->SetShowEnable( bOpen, bOpen );

			if( true == bOpen )
			{
				int iLayer = XDL_POP_UP + 2;
				g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGQuestRewardItemInfo, iLayer );
			}
		}
		else
		{
			return;
		}

#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	}

	catch ( ... )
	{
		StateLog( L"OpenRewardItemInfoPopup First Try Catch" );
	}

	if( false == bOpen )
		return;

    CX2Item* pItem = NULL;
    {
	    CX2Item::ItemData kItemData;
		kItemData.m_ItemID = iItemID;
		
#ifdef FIXED_DIALOG_REWARD_ITEM_ENDURANCE						
		if ( NULL != g_pData && NULL != g_pData->GetItemManager()  )
		{
            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );		// 아이템 템플릿에서 아이템 ID 얻어옴
			if( NULL != pItemTemplet ) 
			{
				kItemData.m_Endurance = pItemTemplet->GetEndurance();											// 아이템 템플릿의 내구도 복사
			}
		}
#endif	// FIXED_DIALOG_REWARD_ITEM_ENDURANCE

	    pItem = new CX2Item( kItemData, NULL );
        ASSERT( pItem != NULL );
    }
	wstring wstrItemDesc = L"";

	if ( NULL != pItem )
	{
		wstrItemDesc = g_pData->GetUIManager()->GetUIQuestNew()->GetSlotItemDescByTID( pItem, iItemID , false );
	}

	try
	{
		SAFE_DELETE( pItem );

		CKTDGUIStatic* pStatic_ItemDesc = (CKTDGUIStatic*) m_pDLGQuestRewardItemInfo->GetControl(L"ItemDesc");
		if( NULL == pStatic_ItemDesc )
			return; 

		CKTDGUIControl::UIStringData* pString_ItemDesc = pStatic_ItemDesc->GetString(0);
		if( NULL == pString_ItemDesc )
			return;
	}
	catch ( ... )
	{
		StateLog( L"OpenRewardItemInfoPopup Second Try Catch" );
	}

#ifdef FIX_TOOLTIP	// 슬롯 형태가 아닌 버튼 형태로 되어 있어서 툴팁 자르기를 따로 수행(X2ItemSlotMananger::DrawSlotDesc)

	bool bItemDescAdd = false;			// 선택된 아이템의 설명 나누기

	const int iWidth = DXUTGetBackBufferSurfaceDesc()->Width;
	const int iHeight = DXUTGetBackBufferSurfaceDesc()->Height;
	const int SIZE_Y = 40;

	int itemDescWidth = static_cast<int>( iWidth / 3 / g_pKTDXApp->GetResolutionScaleX() );
	int itemDescHeight = 0;
	int itemDescFontHeight = g_pKTDXApp->GetResolutionScaleY() > 1.1 ? 15 : 16;	// XUF_DODUM_13_SEMIBOLD의 크기
	wstring wstrItemDescTemp = wstrItemDesc;
	wstrItemDesc = g_pMain->GetStrByLineBreakColor(wstrItemDesc.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX);

#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION 

	CKTDGUIStatic * pRewardItemInfoDlgStatic = NULL;
	CKTDGUIButton::CPictureData * pRewardItemInfoPictureData = NULL;
	if ( NULL != m_pDLGQuestRewardItemInfo )
	{
		pRewardItemInfoDlgStatic = m_pDLGQuestRewardItemInfo->GetStatic_LUA( "ItemDesc" );
		if ( NULL != pRewardItemInfoDlgStatic )
		{
			pRewardItemInfoPictureData = pRewardItemInfoDlgStatic->GetPicture(0);
		}
	}

	CKTDGUIStatic * pRewardItemInfoDlgStaticAdd = NULL;
	CKTDGUIButton::CPictureData * pRewardItemInfoAddPictureData = NULL;
	if ( NULL != m_pDLGQuestRewardItemInfo )
	{
		pRewardItemInfoDlgStaticAdd = m_pDLGQuestRewardItemInfoAdd->GetStatic_LUA( "ItemDescAdd" );
		if ( NULL != pRewardItemInfoDlgStaticAdd )
		{
			pRewardItemInfoAddPictureData = pRewardItemInfoDlgStaticAdd->GetPicture(0);
		}
	}

#else // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	CKTDGUIStatic * pRewardItemInfoDlgStatic = NULL;
	CKTDGUIButton::CPictureData * pRewardItemInfoPictureData = NULL;
	if ( NULL != m_pDLGQuestRewardItemInfo )
	{
		pRewardItemInfoDlgStatic = m_pDLGQuestRewardItemInfo->GetStatic_LUA( "ItemDesc" );
		if ( NULL != pRewardItemInfoDlgStatic )
		{
			pRewardItemInfoPictureData = pRewardItemInfoDlgStatic->GetPicture(0);
		}
	}


#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

	try
	{
		SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, false );

		CKTDGUIStatic* pItemDesc = (CKTDGUIStatic*) m_pDLGItemDesc->GetControl(L"ItemDesc");
		if( NULL != pItemDesc && 
			NULL != pItemDesc->GetString(0) )
		{
			const int MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT = static_cast<int>( iHeight / g_pKTDXApp->GetResolutionScaleY() );
			if ( NULL != pRewardItemInfoDlgStatic )
			{
				CKTDGUIControl::UIStringData* pGUIString = pRewardItemInfoDlgStatic->GetString(0);
				if ( NULL != pGUIString )
				{
					if( itemDescHeight > MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT )
					{
#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION			
						pGUIString->fontIndex = XUF_DODUM_11_NORMAL;				
#else // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
						pItemDesc->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION				
						itemDescFontHeight = g_pKTDXApp->GetResolutionScaleY() > 1.1 ? 13 : 14;	// XUF_DODUM_11_NORMAL의 크기
						wstrItemDesc = g_pMain->GetStrByLineBreakColor(wstrItemDescTemp.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
						SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX_SMALL, false );
					}
		
					else
					{
#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
						pGUIString->fontIndex = XUF_DODUM_13_SEMIBOLD;
#else // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
						pItemDesc->GetString(0)->fontIndex = XUF_DODUM_13_SEMIBOLD;
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
					}
				}
			}
		}
	}
	
	catch ( ... )
	{
		StateLog( L"OpenRewardItemInfoPopup Third Try Catch" );
	}

#ifndef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	int offsetSize = 0;
	int lineNum = 0;
	offsetSize = wstrItemDesc.find( L"\n", offsetSize );
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	try
	{

#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
		const int MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT = static_cast<int> ( iHeight / g_pKTDXApp->GetResolutionScaleY() );
		if ( itemDescHeight > MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT )
		{
			bItemDescAdd = true;
			itemDescWidth = static_cast<int>( iWidth / 4 / g_pKTDXApp->GetResolutionScaleX() );
			itemDescHeight = static_cast<int>( iHeight / g_pKTDXApp->GetResolutionScaleY() );
	
			wstrItemDesc = g_pMain->GetStrByLineBreakColor(wstrItemDescTemp.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
			SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX_SMALL, false );

			wstrItemDescTemp = L"";		
			SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX_SMALL, true );
		}


#else // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
		//{{ 폰트를 작게 설정했는데도 화면 높이보다 클 경우는 분할

		while( offsetSize != -1 )
		{
			offsetSize++;
			lineNum++;
			offsetSize = wstrItemDesc.find( L"\n", offsetSize );
		}

		itemDescHeight += lineNum;
		
		int itemDescAddHeight = 0;
		if ( itemDescHeight > iHeight / g_pKTDXApp->GetResolutionScaleY() )
		{
			itemDescWidth = static_cast<int>( iWidth / 4 / g_pKTDXApp->GetResolutionScaleX() );

			wstrItemDesc = g_pMain->GetStrByLineBreakColor(wstrItemDescTemp.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);

			SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX_SMALL, false );
		
			itemDescAddHeight = static_cast<int>( itemDescHeight - iHeight / g_pKTDXApp->GetResolutionScaleY() + itemDescFontHeight * 2 );
			itemDescHeight = static_cast<int>( iHeight / g_pKTDXApp->GetResolutionScaleY() );
			bItemDescAdd = true;
			wstrItemDescTemp = L"";
			SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX_SMALL, true );
		}
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

	}
	
	catch ( ... )
	{
		StateLog( L"OpenRewardItemInfoPopup Fourth Try Catch" );
	}
#endif // FIX_TOOLTIP		
	
	try
	{
#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

		if ( NULL != pRewardItemInfoPictureData )
		{
			pRewardItemInfoPictureData->SetSize( D3DXVECTOR2( static_cast<float> (itemDescWidth), static_cast<float> (itemDescHeight) ) );
			pRewardItemInfoPictureData->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );
		}
		
		if ( NULL != pRewardItemInfoDlgStatic ) 
		{
			CKTDGUIStatic::UIStringData * pStaticString = pRewardItemInfoDlgStatic->GetString(0);			
			if ( NULL != pStaticString )
			{
				pStaticString->msg = wstrItemDesc.c_str();
			}
		}
		if ( NULL != m_pDLGQuestRewardItemInfo && 
			NULL != m_pDLGQuestRewardItemInfoAdd )				
		{
			if ( true == bItemDescAdd )			
			{
		
				if ( NULL != pRewardItemInfoAddPictureData &&
					NULL != pRewardItemInfoDlgStaticAdd )
				{
					SplitDescHeight( &wstrItemDescTemp, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX_SMALL, false);

					D3DXVECTOR2 vAddRewardItemDlgSize = D3DXVECTOR2 ( static_cast<float> ( itemDescWidth ), static_cast<float> ( itemDescHeight ) );
					D3DXVECTOR2 vAddRewardItemDlgPos = D3DXVECTOR2 ( ( static_cast<float> ( iWidth ) / g_pKTDXApp->GetResolutionScaleX() ) - itemDescWidth, 0.f );
					D3DXVECTOR2 vRewardItemDlgPos	 = vAddRewardItemDlgPos;
					vRewardItemDlgPos.x -= ( vAddRewardItemDlgSize.x );

					m_pDLGQuestRewardItemInfo->SetPos ( vRewardItemDlgPos );

					if ( NULL != pRewardItemInfoAddPictureData )
					{
						pRewardItemInfoAddPictureData->SetSize( vAddRewardItemDlgSize );

						CKTDGUIControl::UIPointData * pRewardItemAddPicturePointData = pRewardItemInfoAddPictureData->pPoint;
						if ( pRewardItemAddPicturePointData )
						{
							pRewardItemInfoAddPictureData->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );
						}
					}

					CKTDGUIStatic::UIStringData * pAddStaticString = pRewardItemInfoDlgStaticAdd->GetString(0);
				
					if ( NULL != pAddStaticString )
					{
						pAddStaticString->msg = wstrItemDescTemp.c_str();	
					}					

					m_pDLGQuestRewardItemInfoAdd->SetShow(true);
					m_pDLGQuestRewardItemInfoAdd->SetPos( vAddRewardItemDlgPos );	
				}
			}
		

			else
			{
				m_pDLGQuestRewardItemInfoAdd->SetShow(false);

				D3DXVECTOR2 vDialogPos = vPos;
				vDialogPos.y -= static_cast<float> ( itemDescHeight );
				vDialogPos.x -= static_cast<float> ( itemDescWidth );

				if( vDialogPos.y < 0 )
					vDialogPos.y = 0;

				m_pDLGQuestRewardItemInfo->SetPos( vDialogPos );
			}
		}
#else // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
		CKTDGUIStatic * pRewardItemInfoDlg = m_pDLGQuestRewardItemInfo->GetStatic_LUA( "ItemDesc" );
		CKTDGUIButton::CPictureData * pRewardItemInfoPictureData = pRewardItemInfoDlg->GetPicture(0);

		pRewardItemInfoPictureData->SetSize( D3DXVECTOR2( static_cast<float> (itemDescWidth), static_cast<float> (itemDescHeight) ) );
		pRewardItemInfoPictureData->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );
		pRewardItemInfoDlg->GetString(0)->msg = wstrItemDesc.c_str();


		D3DXVECTOR2 vDialogPos = vPos;
		vDialogPos.y -= (float) itemDescHeight;
		vDialogPos.x -= (float) itemDescWidth;

		if( vDialogPos.y < 0 )
			vDialogPos.y = 0;

		m_pDLGQuestRewardItemInfo->SetPos( vDialogPos );
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	
	}
	catch ( ... )
	{
		StateLog( L"OpenRewardItemInfoPopup Fifth Try Catch" );
	}
}

/** @function : SetBossDropItemTexture
@breif : 던전도구창의 보스드랍 아이템 슬롯 텍스쳐 설정
@praram : 슬롯 인덱스(iIndex_), 드랍아이템ID(iItemID_), 출력여부(bShow_)

*/
void CX2PartyUI::SetBossDropItemTexture(int iIndex_, int iItemID_ /* = -1 */, bool bShow_ /* = false */)
{
	if( NULL == m_pDLGParty || 
		NULL == g_pData || 
		NULL == g_pData->GetItemManager())
		return;

	WCHAR strIndexID[256] = {0,};
	StringCchPrintf( strIndexID, 256, L"ITEM%d", iIndex_+1);
	m_mapBossItemList.insert( std::make_pair(strIndexID, iItemID_));				

	CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>(m_pDLGParty->GetControl( strIndexID ));	
	if( NULL != pButton )
	{
		pButton->SetShowEnable(bShow_, bShow_);
	}

	CKTDGUIControl::CPictureData* pPicture = static_cast<CKTDGUIStatic*>(m_pDLGParty->GetControl( L"ITEM"))->GetPicture(iIndex_); // 보상 아이템
	if( NULL != pPicture )
	{
		if( true == bShow_ )
		{
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet(  iItemID_ );
			if( NULL != pItemTemplet )
			{
				pPicture->SetTex(pItemTemplet->GetShopImage());
			}
		}
		pPicture->SetShow(bShow_);
	}
}
void CX2PartyUI::UpdateDungeonRequestButton()
{
	if( NULL != m_pDLGParty )
	{
		CKTDGUIButton* pRadio_PartyRequest		= (CKTDGUIButton*) m_pDLGParty->GetControl( L"PartyRequest" );	//던전 신청
		CKTDGUIButton* pRadio_BonusPartyRequest	= (CKTDGUIButton*) m_pDLGParty->GetControl( L"BonusPartyRequest" ); //던전신청(영웅모집공고)

		if( NULL != g_pData && NULL != g_pData->GetPartyManager() && NULL != g_pData->GetPartyManager()->GetMyPartyData())
		{
			//현재 선택된 던전 ID 얻기
			int iCurrentDungeonID = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID;
			int iDungeonDifficulty = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty;
			//영웅모집공고 활성화 여부 얻기
			bool bActiveBonus = g_pData->GetPartyManager()->IsHeroRecruitDungeon(iCurrentDungeonID+iDungeonDifficulty) ;

			if ( NULL != pRadio_PartyRequest )
				pRadio_PartyRequest->SetShowEnable( !bActiveBonus, !bActiveBonus );
			if( NULL != pRadio_BonusPartyRequest)
				pRadio_BonusPartyRequest->SetShowEnable( bActiveBonus, bActiveBonus );
		}
	}
}


/** @function : UpdateDifficultyUI
	@brief	: 던전도구 난이도 설정
	@param : 난이도 ( eDifficultyLevel_ ), 헤니르, 비던 ( eDungeonMode_ ) 
*/
void CX2PartyUI::UpdateDifficultyUI(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_, CX2Dungeon::DUNGEON_TYPE eDungeonType_/*= CX2Dungeon::DT_NORMAL*/, bool pNormalOnly /*= false*/)
{
	UpdateDungeonTypeButton(eDungeonType_);	//비던, 시공에 대한 버튼 종류 설정
	if( CX2Dungeon::DT_NORMAL == eDungeonType_ )
	{
		UpdateDifficultyButton(eDifficultyLevel_);	//보통, 어려움, 매우어려움 버튼 설정
	}
	else
	{
		if( CX2Dungeon::DL_NORMAL != eDifficultyLevel_ )
		{
			eDifficultyLevel_ = CX2Dungeon::DL_HARD;
		}
		UpdateDifficultyButtonForSpecialDungeon(eDifficultyLevel_, eDungeonType_); //헬, 도전 설정
	}
	UpdateAdequateMemberCount(eDungeonType_,eDifficultyLevel_); //던전 적정 인원 설정
	UpdateNormalOnly(eDungeonType_, pNormalOnly);
}

/** @function : UpdateDifficultyButton
	@brief	: 던전도구 난이도 버튼 설정
			UpdateDifficultyUI에서 불려지는 함수
	@param : 난이도 ( eDifficultyLevel_ )
*/
void CX2PartyUI::UpdateDifficultyButton(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_)
{
	if( NULL == m_pDLGParty )
		return;

	CKTDGUICheckBox* pCheckBox_Normal = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"NORMAL"));		//보통
	CKTDGUICheckBox* pCheckBox_Hard = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"HARD"));			//어려움
	CKTDGUICheckBox* pCheckBox_Expert = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"EXPERT"));		//매우어려움

	if( NULL == pCheckBox_Normal || NULL == pCheckBox_Hard || NULL == pCheckBox_Expert )		
		return;
	pCheckBox_Normal->SetChecked(false);
	pCheckBox_Hard->SetChecked(false);
	pCheckBox_Expert->SetChecked(false);

	switch(eDifficultyLevel_)
	{
	case CX2Dungeon::DL_NORMAL:
		pCheckBox_Normal->SetChecked(true);
		break;
	case CX2Dungeon::DL_HARD:
		pCheckBox_Hard->SetChecked(true);
		break;
	case CX2Dungeon::DL_EXPERT:
		pCheckBox_Expert->SetChecked(true);
		break;
	default:
		break;
	}
}

/** @function : UpdateDifficultyButtonForSpecialDungeon
	@brief	: 던전도구 난이도 버튼 설정(비던, 시공 용)
			UpdateDifficultyUI에서 불려지는 함수
	@param : 난이도 ( eDifficultyLevel_ ) 던전타입(eDungeonType_)
*/
void CX2PartyUI::UpdateDifficultyButtonForSpecialDungeon(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_, CX2Dungeon::DUNGEON_TYPE eDungeonType_)
{
	CKTDGUICheckBox* pCheckBox_Normal_Ex = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"NORMAL_EX"));//일반(비던, 헤니르 공통)
	CKTDGUICheckBox* pCheckBox_Hell = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"HELL"));			//헬(비던)
	CKTDGUICheckBox* pCheckBox_Challenge = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"CHALLENGE"));//도전(헤니르)

	if( NULL == pCheckBox_Normal_Ex || NULL == pCheckBox_Hell || NULL == pCheckBox_Challenge )
		return;

	pCheckBox_Normal_Ex->SetChecked(false);
	pCheckBox_Hell->SetChecked(false);
	pCheckBox_Challenge->SetChecked(false);

	switch(eDifficultyLevel_)
	{
	case CX2Dungeon::DL_NORMAL:
		pCheckBox_Normal_Ex->SetChecked(true);
		break;
	case CX2Dungeon::DL_HARD:
		{
			if( CX2Dungeon::DT_SECRET == eDungeonType_ )
			{
				pCheckBox_Hell->SetChecked(true);
			}
			else
			{
				pCheckBox_Challenge->SetChecked(true);
			}
		} break;
	default:
		break;
	}

}
/** @function :UpdateDungeonTypeButton
	@brief : 던전도구 던전타입 설정(비던, 헤니르)
*/
void CX2PartyUI::UpdateDungeonTypeButton(CX2Dungeon::DUNGEON_TYPE eDungeonType_)
{
	if( NULL == m_pDLGParty )
		return;

	CKTDGUICheckBox* pCheckBox_Normal = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"NORMAL"));
	CKTDGUICheckBox* pCheckBox_Hard = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"HARD"));
	CKTDGUICheckBox* pCheckBox_Expert = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"EXPERT"));

	CKTDGUICheckBox* pCheckBox_Normal_Ex = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"NORMAL_EX"));//일반(비던, 헤니르 공통)
	CKTDGUICheckBox* pCheckBox_Hell = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"HELL"));			//헬(비던)
	CKTDGUICheckBox* pCheckBox_Challenge = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"CHALLENGE"));//도전(헤니르)

	if( NULL == pCheckBox_Normal_Ex || NULL == pCheckBox_Hell || NULL == pCheckBox_Challenge ||
		 NULL == pCheckBox_Normal || NULL == pCheckBox_Hard || NULL == pCheckBox_Expert )
		return;

	pCheckBox_Normal->SetShowEnable(false, false);
	pCheckBox_Hard->SetShowEnable(false, false);
	pCheckBox_Expert->SetShowEnable(false, false);
	pCheckBox_Normal_Ex->SetShowEnable(false, false);
	pCheckBox_Hell->SetShowEnable(false, false);
	pCheckBox_Challenge->SetShowEnable(false, false);

	switch(eDungeonType_)
	{
	case CX2Dungeon::DT_INVALID:
	case CX2Dungeon::DT_NORMAL:
		{
			pCheckBox_Normal->SetShowEnable(true, true);
			pCheckBox_Hard->SetShowEnable(true, true);
			pCheckBox_Expert->SetShowEnable(true, true);
		} break;
	case CX2Dungeon::DT_HENIR:
		{
			pCheckBox_Normal_Ex->SetShowEnable(true, true);
			pCheckBox_Challenge->SetShowEnable(true, true);
		} break;
	case CX2Dungeon::DT_SECRET:
		{
			pCheckBox_Normal_Ex->SetShowEnable(true, true);
			pCheckBox_Hell->SetShowEnable(true, true);
		} break;
	default:
		break;
	}
}

/** @function :UpdateAdequateMemberCount
	@brief : 던전 난이도 별 적정인원 설정
	@param : 던전타입(eDungeonType_) 난이도(eDifficultyLevel_)
*/
void CX2PartyUI::UpdateAdequateMemberCount(CX2Dungeon::DUNGEON_TYPE eDungeonType_, CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_)
{
	CKTDGUIStatic* pStatic_AdequateMemberCount	= static_cast<CKTDGUIStatic*>(m_pDLGParty->GetControl( L"need_user" ));		// 적정 인원
	if( NULL == pStatic_AdequateMemberCount )
		return;

	pStatic_AdequateMemberCount->GetString(0)->msg = GET_STRING(STR_ID_983);	//적정인원

	if( CX2Dungeon::DT_NORMAL != eDungeonType_ && CX2Dungeon::DL_HARD == eDifficultyLevel_)
	{//던전타입이 일반이 아닐 경우 HARD는 모두 EXPERT값으로 보여주기.
		eDifficultyLevel_ = CX2Dungeon::DL_EXPERT;
	}

	CX2PartyManager::PartyData* pPartyData = g_pData->GetPartyManager()->GetMyPartyData();
	if( NULL != pPartyData )
	{
		switch(eDifficultyLevel_)
		{
		case CX2Dungeon::DL_NORMAL:
			{
#ifdef SERV_EVENT_VALENTINE_DUNGEON
				if( SEnum::DI_EVENT_VALENTINE_DAY == pPartyData->m_iDungeonID )
				{
					pStatic_AdequateMemberCount->GetString(1)->msg = L"3~4";
				}
				else
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
				if( SEnum::DI_EVENT_VALENTINE_DUNGEON_INT == pPartyData->m_iDungeonID )
				{
					pStatic_AdequateMemberCount->GetString(1)->msg = L"3~4";
				}
				else
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
				if( SEnum::DI_EVENT_HALLOWEEN_DAY == pPartyData->m_iDungeonID )
				{
					pStatic_AdequateMemberCount->GetString(1)->msg = L"3~4";
				}
				else
#endif //SERV_HALLOWEEN_EVENT_2013
				pStatic_AdequateMemberCount->GetString(1)->msg = L"1";
			} break;
		case CX2Dungeon::DL_HARD:
			{
				pStatic_AdequateMemberCount->GetString(1)->msg	= L"2~3";

				if( pPartyData->GetPartyMemberCount() >= 2 )
					pStatic_AdequateMemberCount->GetString(1)->color = D3DXCOLOR( 0,1,0,1 );
				else
					pStatic_AdequateMemberCount->GetString(1)->color = D3DXCOLOR( 1,0,0,1  );

			} break;
		case CX2Dungeon::DL_EXPERT:
			{
				pStatic_AdequateMemberCount->GetString(1)->msg	= L"3~4";

				if( pPartyData->GetPartyMemberCount() >= 3 )
					pStatic_AdequateMemberCount->GetString(1)->color = D3DXCOLOR( 0,1,0,1 );
				else
					pStatic_AdequateMemberCount->GetString(1)->color = D3DXCOLOR( 1,0,0,1  );

			} break;
		default:
			break;
		}
	}
}

/** @function :UpdateNormalOnly
	@brief : 보통만 있는 던전에 대한 버튼 활성화 설정
	@param : 던전타입(eDungeonType_), 보통던전여부(bNormalOnly)
*/
void CX2PartyUI::UpdateNormalOnly(CX2Dungeon::DUNGEON_TYPE eDungeonType_, bool bNormalOnly_)
{
	switch(eDungeonType_)
	{
	case CX2Dungeon::DT_INVALID:
	case CX2Dungeon::DT_NORMAL:
		{
			CKTDGUICheckBox* pCheckBox_Hard = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"HARD"));
			CKTDGUICheckBox* pCheckBox_Expert = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"EXPERT"));
			if( NULL != pCheckBox_Hard && NULL != pCheckBox_Expert)
			{
				pCheckBox_Hard->SetEnable(!bNormalOnly_);
				pCheckBox_Expert->SetEnable(!bNormalOnly_);
			}
		} break;
	case CX2Dungeon::DT_HENIR:
		{
			CKTDGUICheckBox* pCheckBox_Challenge = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"CHALLENGE"));//도전(헤니르)
			if( NULL != pCheckBox_Challenge )
			{
				pCheckBox_Challenge->SetEnable(!bNormalOnly_);
			}
		} break;
	case CX2Dungeon::DT_SECRET:
		{
			CKTDGUICheckBox* pCheckBox_Hell = static_cast<CKTDGUICheckBox*>(m_pDLGParty->GetControl(L"HELL"));			//헬(비던)
			if( NULL != pCheckBox_Hell )
			{
				pCheckBox_Hell->SetEnable(!bNormalOnly_);
			}
		} break;
	default:
		break;
	}
}


/** @function :UpdateAdequateItemLevel
	@brief : 던전 별 적정 아이템 레벨 갱신
	@param : dungeondata에 정의되어 있는 적정아이템 레벨(iItemLevel_)
*/
void CX2PartyUI::UpdateAdequateItemLevel(int iItemLevel_)
{
	CKTDGUIStatic* pStatic_AdequateItemLevel = static_cast<CKTDGUIStatic*>(m_pDLGParty->GetControl( L"AdequateItemLevel" ));	// 적정 아이템 레벨
	if( NULL != pStatic_AdequateItemLevel )
	{
		pStatic_AdequateItemLevel->GetString(0)->msg =GET_STRING(20124);
		pStatic_AdequateItemLevel->GetString(1)->msg = GET_REPLACED_STRING( ( STR_ID_2632, "i", iItemLevel_ ) );
		pStatic_AdequateItemLevel->GetString(1)->color = D3DXCOLOR( 0,1,0,1 );
	}
}


void CX2PartyUI::UpdateBossItemList(const CX2Dungeon::DungeonData::mapBossDropItem& BossDropItemList_, bool bIsHenirChallenge /*= false*/)
{
	CKTDGUIStatic* pStaticItem = static_cast<CKTDGUIStatic*>(m_pDLGParty->GetControl( L"ITEM")); // 보상 아이템
	if( NULL != pStaticItem )
	{
		pStaticItem->GetString(0)->msg = GET_STRING(STR_ID_402);
	}

	// 아이템 슬롯 설정
	m_mapBossItemList.clear();
	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		int iBossDropItemIndex = 0;
		CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();	

		CX2Dungeon::DungeonData::mapBossDropItem::const_iterator it = BossDropItemList_.find(CX2Unit::UT_NONE);
		if( it != BossDropItemList_.end() )
		{
			for(unsigned int i = 0; i < it->second.size(); ++i)
			{
				if( iBossDropItemIndex >= 6 )
				{
					break;
				}
				SetBossDropItemTexture( iBossDropItemIndex++, it->second[i], true );
			}			
		}

		it = BossDropItemList_.find(eUnitType);
		if( it != BossDropItemList_.end() )
		{
			for(unsigned int i = 0; i < it->second.size(); ++i)
			{
				if( iBossDropItemIndex >= 6 )
				{
					break;
				}
				SetBossDropItemTexture( iBossDropItemIndex++, it->second[i], true );

#ifdef _IN_HOUSE_//	//예상 보스드랍 아이템이 직업과 맞지 않게 설정되어 있을 때 팝업창 추가
				if( NULL != g_pData && NULL != g_pData->GetItemManager() )
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet(it->second[i]);
					if( NULL != pItemTemplet )
					{
						if( CX2Unit::UT_NONE != pItemTemplet->GetUnitType() &&
							eUnitType != pItemTemplet->GetUnitType() )
						{
							WCHAR strTemp[255];
							StringCchPrintfW( strTemp, 255, L"보스아이템 UNIT_TYPE오류\n(ItemID : %d)", it->second[i] );

							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), strTemp, (CKTDXStage*)g_pMain->GetNowState() );			
						}
					}
				}
#endif //_IN_HOUSE_
			}			
		}

#ifdef NEW_HENIR_DUNGEON // 시공 헬 모드 보상 아이템 추가
		if( true == bIsHenirChallenge && iBossDropItemIndex < 6)
		{
			SetBossDropItemTexture( iBossDropItemIndex++, 61538, true ); // 시공간의 오오라 조각
		}
#endif // NEW_HENIR_DUNGEON

		for( int i = iBossDropItemIndex; i < 6; ++i )
		{
			SetBossDropItemTexture(i);
		}
	}
}

/** @function : UpdateDungeonThumbnailQuestMark
	@breifh : 던전도구창의 던전 썸네일 위에 표시되는 퀘스트 마크 갱신
*/
void CX2PartyUI::UpdateDungeonThumbnailQuestMark()
{
	CKTDGUIStatic* pStatic_LocalQuest		= static_cast<CKTDGUIStatic*> (m_pDLGParty->GetControl( L"StaticLocalQuest" ));	// 로컬 퀘스트
	if( NULL == pStatic_LocalQuest )
		return;

	pStatic_LocalQuest->GetPicture(0)->SetFlicker( 0.5f, 1.0f, 0.1f );

	
	pStatic_LocalQuest->SetShow(false);

	if(g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIQuestNew() != NULL)
	{
		const int iDungeonID = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID;
		const int iDungeonDifficulty = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty;
		const int iDungeonMode = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode;

		if( true == g_pData->GetUIManager()->GetUIQuestNew()->GetPartyUIQuestIcon(iDungeonID, iDungeonDifficulty, iDungeonMode) )
		{
			pStatic_LocalQuest->SetShow(true);
		}
	}
}

bool CX2PartyUI::SelectDungeonDifficulty(const int iSelectDungeonDifficulty_)
{
	CX2PartyManager::PartyData* pPartyData = g_pData->GetPartyManager()->GetMyPartyData();
	const CX2Dungeon::DungeonData* pDungeonData_Normal	= g_pData->GetDungeonManager()->GetDungeonData( static_cast<SEnum::DUNGEON_ID>( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) );
	if ( NULL != pDungeonData_Normal && NULL != pPartyData )
	{
		if( true == g_pData->GetDungeonManager()->IsDefenceDungeon( pDungeonData_Normal->m_DungeonID ) )
		{
			return false;
		}

		if( true == pDungeonData_Normal->m_bNormalOnly && CX2Dungeon::DL_NORMAL != iSelectDungeonDifficulty_ )
		{
			return false;
		}

		pPartyData->m_iDungeonDifficulty = CX2Dungeon::DL_NORMAL;
		pPartyData->m_iDungeonMode = CX2Dungeon::DM_INVALID;

		switch ( pDungeonData_Normal->m_eDungeonType )
		{
		case CX2Dungeon::DT_NORMAL: //일반 던전
			{
				pPartyData->m_iDungeonDifficulty = iSelectDungeonDifficulty_;
			} break;
		case CX2Dungeon::DT_HENIR: //헤니르의 시공
			{
				if( CX2Dungeon::DL_NORMAL == iSelectDungeonDifficulty_ )
				{
					pPartyData->m_iDungeonMode = CX2Dungeon::DM_HENIR_PRACTICE;
				}
				else
				{
					pPartyData->m_iDungeonMode = CX2Dungeon::DM_HENIR_CHALLENGE;
				}
#ifdef NEW_HENIR_DUNGEON
				if( NULL != GetLocalMapUI() )
					GetLocalMapUI()->UpdateLocalMapDLG();
#endif // NEW_HENIR_DUNGEON

			} break;
		case CX2Dungeon::DT_SECRET:
			{
				if( CX2Dungeon::DL_NORMAL == iSelectDungeonDifficulty_ )
				{
					pPartyData->m_iDungeonMode = CX2Dungeon::DM_SECRET_NORMAL;
				}
				else
				{
					pPartyData->m_iDungeonMode = CX2Dungeon::DM_SECRET_HELL;
					pPartyData->m_iDungeonDifficulty = CX2Dungeon::DL_HARD;
				}
			} break;
		default:
			break;
		}


		UpdateDifficultyUI( static_cast<CX2Dungeon::DIFFICULTY_LEVEL>(iSelectDungeonDifficulty_), pDungeonData_Normal->m_eDungeonType);

		SEnum::DUNGEON_ID eDungeonIDWithDifficulty = 
			static_cast<SEnum::DUNGEON_ID>( pPartyData->m_iDungeonID + pPartyData->m_iDungeonDifficulty );

		const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( eDungeonIDWithDifficulty );
		if( NULL != pDungeonData )
		{
			UpdateAdequateItemLevel(pDungeonData->m_RequireItemLevel);
			UpdateBossItemList(pDungeonData->m_mapBossDropItem, (pPartyData->m_iDungeonMode == CX2Dungeon::DM_HENIR_CHALLENGE) );
		}
		UpdateDungeonThumbnailQuestMark();
	}

	if( true == g_pData->GetPartyManager()->DoIHaveParty() )
	{
		g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_DUNGEON_REQ( pPartyData->m_iDungeonID, 
			pPartyData->m_iDungeonDifficulty, 
			pPartyData->m_iDungeonMode);
	}
	if ( NULL != g_pData && NULL != g_pData->GetPartyManager() )
	{
		g_pData->GetPartyManager()->UpdateSelectedDungeon();
	}

	return true;

}

void CX2PartyUI::GameStartCurrentMember()
{
	// 게임내에서 바쁜 상태인 경우 던전을 시작 할 수 없음
	if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
		return;

	if( false == static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() )
		return;

	OpenPartyDLG(false);

	// 파티가 아니거나, 파티일 때는 파티장인 경우만
	CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
	if ( false == pPartyManager->DoIHaveParty() || true == pPartyManager->AmIPartyLeader() )
	{
		const CX2PartyManager::PartyData* pPartydata = g_pData->GetPartyManager()->GetMyPartyData();

		SEnum::DUNGEON_ID eDungeonIDWithDifficulty = 
			static_cast<SEnum::DUNGEON_ID>( pPartydata->m_iDungeonID + pPartydata->m_iDungeonDifficulty );

		const CX2Dungeon::DungeonData* pDungeonData	= g_pData->GetDungeonManager()->GetDungeonData( eDungeonIDWithDifficulty );
		if ( NULL != pDungeonData )
			g_pData->GetPartyManager()->Handler_EGS_QUICK_START_DUNGEON_GAME_REQ( pPartydata );

#ifdef REFORM_NOVICE_GUIDE
		if( NULL != g_pData && NULL != g_pData->GetPlayGuide() 
			&& true == g_pData->GetPlayGuide()->GetActiveNoviceGuide() )
		{
			switch( g_pData->GetPlayGuide()->GetGuideStep() )
			{
			case CX2PlayGuide::NGS_STEP_6:
			case CX2PlayGuide::NGS_STEP_7:
			case CX2PlayGuide::NGS_STEP_8:
				{
					g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_END );
				} break;
			}
		}
#endif //REFORM_NOVICE_GUIDE
	}	
}

void CX2PartyUI::SetPVPButton(GAME_TOOL_BUTTON_STATE eButtonState_ /*= BUTTON_START*/, bool bChecked_/* = true*/, bool bForceDisable /*= false*/)
{
	m_ePVPButton = eButtonState_;

	CKTDGUIButton* pMatchWait	= static_cast<CKTDGUIButton*>(m_pDLGPartyMenu->GetControl( L"PvpMatchWaiting" )); //대전 대기중
	CKTDGUIButton* pMatchCancel	= static_cast<CKTDGUIButton*>(m_pDLGPartyMenu->GetControl( L"DungeonMatchCancel" )); //대기 취소(던전)

	CKTDGUIStatic* pStaticStayTime = (CKTDGUIStatic*) m_pDLGPartyMenu->GetControl( L"PVP_STAY" ); // 대기 예상 시간 활성화

	if ( NULL == m_pPVPCheckBox || NULL == pMatchWait || NULL == pMatchCancel ||
		NULL == pStaticStayTime)
		return;

	pMatchCancel->SetShow(false);
	m_pPVPCheckBox->SetShow(false);
	pMatchWait->SetShow(false);
	pStaticStayTime->SetShow(false);

	bool bEnable = true;	
	if( false == g_pData->GetPartyManager()->AmIPartyLeader() &&
		true == g_pData->GetPartyManager()->DoIHaveParty() )
	{
		bEnable = false;
		bChecked_ = false;
	}
	
	bool bShowPvp = true;
	if( NULL != g_pData )
	{
		bShowPvp = (g_pData->GetSelectUnitLevel() >= 10) ;
	}

	if( true == bForceDisable )
	{
		bEnable = false;
		bChecked_ = false;
	}

	switch(eButtonState_)
	{
	case BUTTON_START:
		{
			if ( true == bShowPvp )
			{
				m_pPVPCheckBox->SetShowEnable(true, bEnable);
				m_pPVPCheckBox->SetChecked(bChecked_);
			}
			if( NULL != m_pDLGParty &&
				NULL != g_pMain && NULL != g_pMain->GetPartyUI())
				m_pDLGParty->SetShow(false);
		} break;
	case BUTTON_WAIT:
		{
			if ( true == bShowPvp )
			{
				pMatchWait->SetShowEnable(true, bEnable);
				pStaticStayTime->SetShowEnable(true, true);//대기중이라면 대기예상시간은 무조건 출력

				if( NULL != m_pDLGParty && NULL != g_pMain->GetPartyUI() )
					m_pDLGParty->SetShow(false);
			}

		} break;
	case BUTTON_CANCEL:
		{
			pMatchCancel->SetShowEnable(bEnable, bEnable); //취소버튼은 사용 할 수 없을 때 보이지도 않게 수정
		} break;
	}
}

void CX2PartyUI::SetDungeonButton(GAME_TOOL_BUTTON_STATE eButtonState_ /*= BUTTON_START*/, bool bChecked_ /*= true*/, bool bForceDisable /*= false*/)
{
	if( NULL == m_pDLGPartyMenu )
		return;

	m_eDungeonButton = eButtonState_;

	CKTDGUIButton* pMatchWait	= static_cast<CKTDGUIButton*>(m_pDLGPartyMenu->GetControl( L"DungeonMatchWaiting" )); //던전 대기중
	CKTDGUIButton* pMatchCancel	= static_cast<CKTDGUIButton*>(m_pDLGPartyMenu->GetControl( L"PvpMatchCancel" )); //대기 취소(대전)

	CKTDGUIStatic* pStaticStayTime = (CKTDGUIStatic*) m_pDLGPartyMenu->GetControl( L"DUNGEON_STAY" ); // 대기 예상 시간 활성화
	if ( NULL == m_pDungeonCheckBox || NULL == pMatchWait || NULL == pMatchCancel ||
		NULL == pStaticStayTime)
		return;

	pMatchCancel->SetShow(false);
	m_pDungeonCheckBox->SetShow(false);
	pMatchWait->SetShow(false);
	pStaticStayTime->SetShow(false);


	bool bEnable = true;
	//파티장이 아닐 경우에 버튼 비활성화
	if( false == g_pData->GetPartyManager()->AmIPartyLeader() &&
		true == g_pData->GetPartyManager()->DoIHaveParty() )
	{
		bEnable = false;
		bChecked_ = false;
	}

	if( true == bForceDisable )
	{
		bEnable = false;
		bChecked_ = false;
	}

	switch(eButtonState_)
	{
	case BUTTON_START:
		{
			m_pDungeonCheckBox->SetShowEnable(true, bEnable);
			m_pDungeonCheckBox->SetChecked(bChecked_);
			
			if( NULL != m_pDLGParty &&
				NULL != g_pMain && NULL != g_pMain->GetPartyUI() &&
				NULL != g_pMain->GetPartyUI()->GetLocalMapUI() &&
				false == g_pMain->GetPartyUI()->GetLocalMapUI()->GetShowLocalMap() )
				m_pDLGParty->SetShow(false);
		} break;
	case BUTTON_WAIT:
		{
			pMatchWait->SetShowEnable(true, bEnable);
			pStaticStayTime->SetShowEnable(true, true);//대기중이라면 대기예상시간은 무조건 출력

			if( NULL != m_pDLGParty && NULL != g_pMain->GetPartyUI() )
				m_pDLGParty->SetShow(false);
			if( NULL != g_pMain->GetPartyUI() && NULL != g_pMain->GetPartyUI()->GetLocalMapUI() )
				g_pMain->GetPartyUI()->GetLocalMapUI()->OpenLocalMapDLG(false);
		} break;
	case BUTTON_CANCEL:
		{
			pMatchCancel->SetShowEnable(bEnable, bEnable); //취소버튼은 사용 할 수 없을 때 보이지도 않게 수정 
		} break;
	}
}

#ifdef SERV_NEW_EVENT_TYPES
void CX2PartyUI::ClearLocalMapDLGs()
{
	if( m_pLocalMapUI != NULL )
	{
		bool bShowLocalMap = m_pLocalMapUI->GetShowLocalMap();
		m_pLocalMapUI->ClearLocalMapDLGs();

		// 열려져 있었다면 다시 연다.
		if( bShowLocalMap == true )
			OpenLocalMapDlg();
	}

// 해외팀 필드 파티리스트 제거로 인해 주석 처리
/*
	if( m_pDLGPartyList != NULL )
	{
		bool bShowPartyList = m_pDLGPartyList->GetShow();
		SAFE_DELETE_DIALOG( m_pDLGPartyList );

		/ 열려져 있었다면 다시 연다.
		if( bShowPartyList == true )
			OpenPartyListDLG( true, eLocalMapID );
	}
*/
}
#endif SERV_NEW_EVENT_TYPES
