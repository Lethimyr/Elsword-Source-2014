#include "StdAfx.h"

#include ".\X2CharPopupMenu.h"

CX2CharPopupMenu::CX2CharPopupMenu()
: CX2PopupUIBase(),
m_pDlgMenu( NULL ),
m_iUid( -1 ),
m_wstrName( L"" ),
m_eUnitClass( CX2Unit::UC_NONE ),
m_pPicCharacter( NULL ),
//m_pPicLevelTen(),
//m_pPicLevelOne(),
m_pStaticName( NULL ),
m_pButtonSimpleInfo(NULL),
m_pButtonUserInfo( NULL ),
m_pButtonWatch( NULL ),
m_pButtonwhisper( NULL ),
m_pButtonfriend( NULL ),
m_pButtondisciple( NULL ),
m_pButtonleader( NULL ),
m_pButtonparty( NULL ),
m_pButtoninvite( NULL ),
m_pButtonout( NULL ),
m_pButtontrade( NULL ),
m_pButtonInviteGuild( NULL )	//{{ 허상형 : [2009/9/18] // 길드 버튼 추가
#ifdef ADDED_RELATIONSHIP_SYSTEM
,m_pButtonInviteCouple ( NULL )
,m_pButtonSummonCouple ( NULL )
#endif // 
, m_pPicMiddle1( NULL )
, m_pPicBottom1( NULL )
, m_pPicMiddle2( NULL )
, m_pPicBottom2( NULL )
{   
	// 변수 초기화
	for(UINT i=0; i < ARRAY_SIZE(m_pPicLevelTen); ++i)
	{
		m_pPicLevelTen[i] = NULL;
	}
	for(UINT i=0; i < ARRAY_SIZE(m_pPicLevelOne); ++i)
	{
		m_pPicLevelOne[i] = NULL;
	}




	// 변수 값 할당
	m_pDlgMenu = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_User_Menu_NEW.lua" );
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMenu );    
    m_pDlgMenu->SetShowEnable(false, false);
    

#ifdef REMOVE_INVITE_PARTY_SERVER_BUTTON
	CKTDGUIRadioButton* pRadioGaia = static_cast<CKTDGUIRadioButton*>(m_pDlgMenu->GetControl(L"Gaia"));
	pRadioGaia->SetShowEnable( false , false );
	CKTDGUIRadioButton* pRadioSoles = static_cast<CKTDGUIRadioButton*>(m_pDlgMenu->GetControl(L"Soles"));
	pRadioSoles->SetShowEnable( false , false );
#endif // REMOVE_INVITE_PARTY_SERVER_BUTTON

	// 캐릭터명
	m_pStaticName = m_pDlgMenu->GetStatic_LUA("_name");	

	//{{ seojt // 2009-1-8, 20:48
    // array의 boundary를 벗어나서 배열을 indexing하는 버그 수정
    // - jintaeks on 2009-01-08, 20:49
    //
	// 레벨
	//10자리 ( 1 ~ 9 )
	CKTDGUIStatic *pStaticDlgTen = m_pDlgMenu->GetStatic_LUA("ten");
	if( NULL != pStaticDlgTen )
	{
		for(UINT i=0; i < ARRAY_SIZE(m_pPicLevelTen); ++i)
		{
			m_pPicLevelTen[i] = pStaticDlgTen->GetPictureIndex(i+1);
		}
	}
	//1자리 ( 0 ~ 9 )
	CKTDGUIStatic *pStaticDlgOne = m_pDlgMenu->GetStatic_LUA("one");
	if( NULL != pStaticDlgOne )
	{
		for(UINT i=0; i < ARRAY_SIZE(m_pPicLevelOne); ++i)
		{
			m_pPicLevelOne[i] = pStaticDlgOne->GetPictureIndex(i+1);
		}
	}
	//}} seojt // 2009-1-8, 20:48

    //{{ seojt // 2009-8-21, 12:21
    /** GetControl()로 얻은 포인터를 보관해 두었다가 사용하는 것은 위험하다.
        즉시(on the fly) 사용하는 구조로 바꾸든지,
        handle을 저장하도록 수정해야 할 것이다.
        - jintaeks on 2009-08-21, 11:50 */
    //}} seojt // 2009-8-21, 12:21	
	m_pButtonSimpleInfo = (CKTDGUIButton*) m_pDlgMenu->GetControl( L"character_info" );
	m_pButtonUserInfo	= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"info" );
	m_pButtonWatch		= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"watch" );
	m_pButtonwhisper	= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"whisper" );
	m_pButtonfriend		= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"friend" );
	m_pButtondisciple	= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"disciple" );
	m_pButtonleader		= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"leader" );
	m_pButtonparty		= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"party" );
	m_pButtoninvite		= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"invite" );
	m_pButtonout		= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"out" );
	m_pButtontrade		= (CKTDGUIButton*) m_pDlgMenu->GetControl( L"trade" );
	//{{ 허상형 : [2009/9/18] //	길드 초대 버튼
	m_pButtonInviteGuild = (CKTDGUIButton*) m_pDlgMenu->GetControl( L"inviteguild" );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	m_pButtonInviteCouple = static_cast<CKTDGUIButton*> ( m_pDlgMenu->GetControl( L"relation" ) );
	m_pButtonSummonCouple = static_cast<CKTDGUIButton*> ( m_pDlgMenu->GetControl( L"summon" ) );
#endif // ADDED_RELATIONSHIP_SYSTEM

}

CX2CharPopupMenu::~CX2CharPopupMenu(void)
{
    SAFE_DELETE_DIALOG( m_pDlgMenu );
    m_pNowState = NULL;
}
void CX2CharPopupMenu::SetUnit( UidType uidUnitUID, bool bPartyMember /*= false*/ )
{	
	//m_pUnit = NULL;	
	m_wstrName = L"";
	m_iUnitLevel = 0;
	m_eUnitClass = CX2Unit::UC_NONE;


	if( true == bPartyMember )
	{
		if( true == g_pData->GetPartyManager()->DoIHaveParty() )
		{
			CX2PartyManager::PartyMemberData* pMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( uidUnitUID );
			if( NULL != pMemberData )
			{
				m_wstrName = pMemberData->m_wstrNickName;
				m_iUnitLevel = pMemberData->m_iUnitLevel;
				m_eUnitClass = pMemberData->m_eUnitClass;
			}
			else
			{
				switch(g_pMain->GetNowStateID())
				{
				case CX2Main::XS_VILLAGE_MAP: // 새마을
					if(g_pTFieldGame != NULL)
					{
						CX2SquareUnit *pUnit = NULL; 
						pUnit = g_pTFieldGame->GetSquareUnitByUID( uidUnitUID );
						ASSERT( NULL != pUnit );
						if( NULL != pUnit )
						{
							m_wstrName = pUnit->GetUnit()->GetNickName();

							m_eUnitClass = pUnit->GetUnit()->GetClass();
							m_iUnitLevel = pUnit->GetUnit()->GetUnitData().m_Level;
						}
						//m_pUnit = pUnit->GetUnit();
					}
					break;
				case CX2Main::XS_SQUARE_GAME:
					if(g_pSquareGame != NULL)
					{
						CX2SquareUnit *pUnit = NULL;
						pUnit = g_pSquareGame->GetSquareUnitByUID( uidUnitUID );
						ASSERT( NULL != pUnit );
						if( NULL != pUnit )
						{
							m_wstrName = pUnit->GetUnit()->GetNickName();

							m_eUnitClass = pUnit->GetUnit()->GetClass();
							m_iUnitLevel = pUnit->GetUnit()->GetUnitData().m_Level;
						}
						//m_pUnit = pUnit->GetUnit();
					}
					break;
				case CX2Main::XS_PVP_ROOM:
					{
						CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( uidUnitUID );
						if( NULL != pSlotData && NULL != pSlotData->m_pUnit )
						{
							m_eUnitClass = pSlotData->m_pUnit->GetClass();
							m_iUnitLevel = pSlotData->m_pUnit->GetUnitData().m_Level;
							m_wstrName = pSlotData->m_pUnit->GetNickName();
							//m_pUnit = pSlotData->m_pUnit;
						}
					}
					break;

				case CX2Main::XS_BATTLE_FIELD:
					{
						CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( uidUnitUID );
						ASSERT( NULL != pGUUser );
						if ( NULL != pGUUser )
						{
							m_wstrName		= pGUUser->GetUnitName();
							m_eUnitClass	= pGUUser->GetUnitClass();
							m_iUnitLevel	= pGUUser->GetUnitLevel();
						}

					} break;
				}
			}
		}
	}
	else
	{
		switch(g_pMain->GetNowStateID())
		{
		case CX2Main::XS_VILLAGE_MAP: // 새마을
			if(g_pTFieldGame != NULL)
			{
				CX2SquareUnit *pUnit = NULL; 
				pUnit = g_pTFieldGame->GetSquareUnitByUID( uidUnitUID );
				ASSERT( NULL != pUnit );
				if( NULL != pUnit )
				{
					m_wstrName = pUnit->GetUnit()->GetNickName();

					m_eUnitClass = pUnit->GetUnit()->GetClass();
					m_iUnitLevel = pUnit->GetUnit()->GetUnitData().m_Level;
				}
				//m_pUnit = pUnit->GetUnit();
			}
			break;
		case CX2Main::XS_SQUARE_GAME:
			if(g_pSquareGame != NULL)
			{
				CX2SquareUnit *pUnit = NULL;
				pUnit = g_pSquareGame->GetSquareUnitByUID( uidUnitUID );
				ASSERT( NULL != pUnit );
				if( NULL != pUnit )
				{
					m_wstrName = pUnit->GetUnit()->GetNickName();

					m_eUnitClass = pUnit->GetUnit()->GetClass();
					m_iUnitLevel = pUnit->GetUnit()->GetUnitData().m_Level;
				}
				//m_pUnit = pUnit->GetUnit();
			}
			break;
		case CX2Main::XS_PVP_ROOM:
			{
				CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( uidUnitUID );
				if( NULL != pSlotData && NULL != pSlotData->m_pUnit )
				{
					m_eUnitClass = pSlotData->m_pUnit->GetClass();
					m_iUnitLevel = pSlotData->m_pUnit->GetUnitData().m_Level;
					m_wstrName = pSlotData->m_pUnit->GetNickName();
					//m_pUnit = pSlotData->m_pUnit;
				}
			}
			break;

		case CX2Main::XS_BATTLE_FIELD:
			{
				CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( uidUnitUID );
				ASSERT( NULL != pGUUser );
				if ( NULL != pGUUser )
				{
					m_wstrName		= pGUUser->GetUnitName();
					m_eUnitClass	= pGUUser->GetUnitClass();
					m_iUnitLevel	= pGUUser->GetUnitLevel();
				}
				
			} break;
		}	
	}

	//if(m_pUnit != NULL)
	//	m_wstrName = m_pUnit->GetNickName();
	//else
	//	m_wstrName = L"";

	g_pChatBox->SetPickedUser( uidUnitUID, m_wstrName );
}


void CX2CharPopupMenu::SetMode( USER_MENU eMode, bool bPartyMember/* = false*/ )
{
    //{{ seojt // 2009-8-21, 11:51
    // invalid한 dialog handle인 경우, control pointer를 사용하면 안 된다.
    // - jintaeks on 2009-08-21, 11:51
    if( m_pDlgMenu == NULL )
    {
        return;
    }//if
    //}} seojt // 2009-8-21, 11:51

	float fButtonWidth = 60.f;
	float fButtonHeight = 21.f;
	D3DXVECTOR2 offsetPos = D3DXVECTOR2(0.f, 0.f);

	m_nMenuCount = 0;


	CKTDGUIStatic* pStaticTextParty = (CKTDGUIStatic*)m_pDlgMenu->GetControl( L"PartyPopUpGage" );
	CKTDGUIStatic* pStaticBar = (CKTDGUIStatic*)m_pDlgMenu->GetControl( L"PopUpBar" );

	if ( NULL != pStaticTextParty && NULL != pStaticBar )
	{
		pStaticBar->SetShowEnable( true, true );

		m_pButtonUserInfo->SetShowEnable( true, true );
		pStaticTextParty->SetShowEnable( true, true );
		pStaticTextParty->GetString( 0 )->msg = GET_STRING( STR_ID_20845 );
		pStaticTextParty->GetString( 0 )->pos.y = m_pButtonUserInfo->GetPos().y + 4;
		++m_nMenuCount;

#ifdef SERV_LOCAL_RANKING_SYSTEM
		++m_nMenuCount;
		m_pButtonSimpleInfo->SetShowEnable( true, true );
		offsetPos.y += fButtonHeight;
		m_pButtonSimpleInfo->SetOffsetPos(offsetPos);
		pStaticTextParty->GetString( 8 )->msg = GET_STRING( STR_ID_2730 );
		pStaticTextParty->GetString( 8 )->pos.y = m_pButtonSimpleInfo->GetPos().y + 4;
#endif //SERV_LOCAL_RANKING_SYSTEM

		m_pButtonleader->SetShowEnable( false, false );
		m_pButtonout->SetShowEnable( false, false );
		pStaticTextParty->GetString( 2 )->msg = L"";
		pStaticTextParty->GetString( 1 )->msg = L"";
		

		if(g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM && // 대전 대기방이고			
		   m_iUid != g_pData->GetMyUser()->GetSelectUnit()->GetUID() ) //자기 캐릭터가 아니면
		{		
			CX2PVPRoom* pPvpRoom = g_pData->GetPVPRoom();
			if( NULL != pPvpRoom  && NULL != pPvpRoom->GetMySlot() )
			{
				if( pPvpRoom->IsRoomMaster( pPvpRoom->GetMySlot() ) == true ) //방장이라면
				{
					++m_nMenuCount;
					m_pButtonout->SetShowEnable(true, true);
					offsetPos.y += fButtonHeight;
					m_pButtonout->SetOffsetPos(offsetPos);
					pStaticTextParty->GetString( 2 )->msg = GET_STRING( STR_ID_2731 );
					pStaticTextParty->GetString( 2 )->pos.y = m_pButtonout->GetPos().y + 4;
				}
			}
		}

		switch(eMode)
		{	
		case UM_NORMAL_NORMAL:
		case UM_PARTYL_NORMAL:		// 파티리더와
			{
				m_pButtoninvite->SetShowEnable( true, true );
				offsetPos.y += fButtonHeight;
				m_pButtoninvite->SetOffsetPos(offsetPos);
				pStaticTextParty->GetString( 1 )->msg = GET_STRING( STR_ID_20846 );
				pStaticTextParty->GetString( 1 )->pos.y = m_pButtoninvite->GetPos().y + 4;
				++m_nMenuCount;
			} break;

		case UM_PARTY_PARTY:
		case UM_PARTY_PARTYL:
			break;

		case UM_PARTYL_PARTY:
			{
				m_pButtonleader->SetShowEnable( true, true );
				offsetPos.y += fButtonHeight;
				m_pButtonleader->SetOffsetPos(offsetPos);
				pStaticTextParty->GetString( 1 )->msg = GET_STRING( STR_ID_20847 );
				pStaticTextParty->GetString( 1 )->pos.y = m_pButtonleader->GetPos().y + 4;

				m_pButtoninvite->SetShowEnable( false, false );

				m_pButtonout->SetShowEnable( true, true );
				offsetPos.y += fButtonHeight;
				m_pButtonout->SetOffsetPos(offsetPos);
				pStaticTextParty->GetString( 2 )->msg = GET_STRING( STR_ID_20848 );
				pStaticTextParty->GetString( 2 )->pos.y = m_pButtonout->GetPos().y + 4;

				m_nMenuCount += 2;
			} break;
		default:
		case UM_NONE:
			break;
		}
	
		m_pButtonwhisper->SetShowEnable( true, true );	
		offsetPos.y += fButtonHeight;
		m_pButtonwhisper->SetOffsetPos(offsetPos);
		pStaticTextParty->GetString( 3 )->msg = GET_STRING( STR_ID_2729 );
		pStaticTextParty->GetString( 3 )->pos.y = m_pButtonwhisper->GetPos().y + 4;
		++m_nMenuCount;

		// 이미 친구등록된 유저인지 확인
		bool bFind = false;
		vector<wstring> vecFriendName;
		if( g_pData->GetMessenger() != NULL)
		{	
			g_pData->GetMessenger()->GetFriendNameList(vecFriendName);
			for(UINT i=0; i<vecFriendName.size(); ++i)
			{
				if(m_wstrName.compare(vecFriendName[i]) == 0)
				{
					bFind = true;
					break;
				}
			}		
		}
		else
		{
			bFind = true;
		}

		if( bFind == true )
		{
			m_pButtonfriend->SetShowEnable( false, false );
			pStaticTextParty->GetString( 4 )->msg = L"";
		}
		else
		{
			m_pButtonfriend->SetShowEnable( true, true );
			offsetPos.y += fButtonHeight;
			m_pButtonfriend->SetOffsetPos(offsetPos);
			pStaticTextParty->GetString( 4 )->msg = GET_STRING( STR_ID_20850 );
			pStaticTextParty->GetString( 4 )->pos.y = m_pButtonfriend->GetPos().y + 4;
			++m_nMenuCount;
		}

#ifdef SERV_NO_DISCIPLE
		m_pButtondisciple->SetShowEnable( false, false );
		pStaticTextParty->GetString( 5 )->msg = L"";
#else SERV_NO_DISCIPLE
		// 이미 사제등록되어있는 유저인지 확인
		bFind = false;
	
		if( m_iUnitLevel < 10)
		{
			m_pButtondisciple->SetShowEnable( true, true );
			offsetPos.y += fButtonHeight;
			m_pButtondisciple->SetOffsetPos(offsetPos);
			pStaticTextParty->GetString( 5 )->msg = GET_STRING( STR_ID_20851 );
			pStaticTextParty->GetString( 5 )->pos.y = m_pButtondisciple->GetPos().y + 4;
			++m_nMenuCount;
		}
		else
		{
			m_pButtondisciple->SetShowEnable( false, false );
			pStaticTextParty->GetString( 5 )->msg = L"";
		}
#endif SERV_NO_DISCIPLE

		m_pButtontrade->SetShowEnable( true, true );
		offsetPos.y += fButtonHeight;
		m_pButtontrade->SetOffsetPos(offsetPos);
		pStaticTextParty->GetString( 6 )->msg = GET_STRING( STR_ID_20852 );
		pStaticTextParty->GetString( 6 )->pos.y = m_pButtontrade->GetPos().y + 4;
		++m_nMenuCount;	
	
		//{{ 허상형 : [2009/9/18] //	길드 초대 버튼 추가
	#ifdef GUILD_MANAGEMENT
		m_pButtonInviteGuild->SetShowEnable( false, false );

		if( g_pData->GetGuildManager()->CanInviteMember() == true )	//	길드 초대 할 수 있는 등급인지 검사
		{
			int iMyGuild = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;
			//	상대 유저의 길드와 내 길드가 다른지 여부 조건식 추가
			if( g_pData->GetGuildManager()->IsMyGuildUser(m_iUid) == false )
			{
#ifdef MODFIY_INVITE_GUILD_MENU
				// 길드에 가입한 유저인지 체크 후 메뉴 추가
				pStaticTextParty->GetString( 7 )->msg = L"";
				bool bIsGuildMember = true;
				if( NULL != g_pX2Game )
				{
					CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( m_iUid );
					if( NULL != pGUUser && 
						NULL != pGUUser->GetUnit() )
					{
						if( false == pGUUser->GetUnit()->GetUnitData().m_wstrGuildName.empty() )
						{
							bIsGuildMember = false;
						}
					}
				}
				else if( NULL != g_pTFieldGame )
				{
					CX2Unit* pUnit = g_pTFieldGame->GetSquareUnitUnitByUID( m_iUid );
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
					m_pButtonInviteGuild->SetShowEnable( true, true );
					offsetPos.y += fButtonHeight;
					m_pButtonInviteGuild->SetOffsetPos(offsetPos);	
					pStaticTextParty->GetString( 7 )->msg = GET_STRING( STR_ID_20853 );
					pStaticTextParty->GetString( 7 )->pos.y = m_pButtonInviteGuild->GetPos().y + 4;
					++m_nMenuCount;	
				}
			}
			else
			{
				pStaticTextParty->GetString( 7 )->msg = L"";
			}
		}
		else
		{
			pStaticTextParty->GetString( 7 )->msg = L"";
		}
	#endif


#ifdef ADDED_RELATIONSHIP_SYSTEM
		
		m_pButtonInviteCouple->SetShowEnable( false, false );
		m_pButtonSummonCouple->SetShowEnable(false, false );
		pStaticTextParty->GetString( 9 )->msg = L"";
		pStaticTextParty->GetString( 10 )->msg = L"";

		// 인연 시스템 소환 하기 버튼 추가 로직
		if ( NULL != g_pData->GetRelationshipManager() &&
			 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			SEnum::RELATIONSHIP_TYPE eRelationStateType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;

			switch ( eRelationStateType )
			{
			case SEnum::RT_SOLO:		/// 신청하기 출력
				{
					m_pButtonInviteCouple->SetShowEnable( true, true );
					offsetPos.y += fButtonHeight;
					m_pButtonInviteCouple->SetOffsetPos(offsetPos);	
					pStaticTextParty->GetString( 9 )->msg = GET_STRING( STR_ID_24452 );
					pStaticTextParty->GetString( 9 )->pos.y = m_pButtonInviteCouple->GetPos().y + 4;
					++m_nMenuCount;
				} break;

			case SEnum::RT_MARRIED:		/// 소환하기 출력
				{
					if ( m_iUid == g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_iRelationTargetUserUid )
					{
						m_pButtonSummonCouple->SetShowEnable(true, true );
						offsetPos.y += fButtonHeight;
						m_pButtonSummonCouple->SetOffsetPos(offsetPos);	
						pStaticTextParty->GetString( 10 )->msg = GET_STRING( STR_ID_24538 );
						pStaticTextParty->GetString( 10 )->pos.y = m_pButtonSummonCouple->GetPos().y + 4;
						++m_nMenuCount;
					}
				} break;
			}
		}
#endif // ADDED_RELATIONSHIP_SYSTEM


		CX2GageManager* pGageManager = CX2GageManager::GetInstance();

		if ( NULL != pGageManager )
		{
			const CX2MyGageUI* pGageUI = pGageManager->GetMyGageUI();

			if ( NULL != pGageUI )
			{
				CKTDGUIStatic* pStaticText = (CKTDGUIStatic*)m_pDlgMenu->GetControl( L"MyPopUp" );
				CKTDGUIButton* pButtonMovetoParty = (CKTDGUIButton*)m_pDlgMenu->GetControl( L"MovetoParty" );
				CKTDGUIButton* pButtonAddParty = (CKTDGUIButton*)m_pDlgMenu->GetControl( L"AddParty" );
				CKTDGUIButton* pButtonDropOut = (CKTDGUIButton*)m_pDlgMenu->GetControl( L"DropOut" );
				ChangeTopImage( false );

				if ( NULL != pStaticText )
					pStaticText->SetShowEnable( false, false );
				if ( NULL != pButtonMovetoParty )
					pButtonMovetoParty->SetShowEnable( false, false );
				if ( NULL != pButtonAddParty )
					pButtonAddParty->SetShowEnable( false, false );
					
				if ( NULL != pButtonDropOut )
					pButtonDropOut->SetShowEnable( false, false );

				if ( true == pGageUI->GetOnPopup() )
				{
					pStaticTextParty->SetShowEnable( false, false );
					pStaticBar->SetShowEnable( false, false );
					m_pStaticName->SetShowEnable( false, false );
					m_pButtonSimpleInfo->SetShowEnable( false, false );
					m_pButtonUserInfo->SetShowEnable( false, false );
					m_pButtonWatch->SetShowEnable( false, false );
					m_pButtonwhisper->SetShowEnable( false, false );
					m_pButtonfriend->SetShowEnable( false, false );
					m_pButtondisciple->SetShowEnable( false, false );
					m_pButtonleader->SetShowEnable( false, false );
					m_pButtonparty->SetShowEnable( false, false );
					m_pButtoninvite->SetShowEnable( false, false );
					m_pButtonout->SetShowEnable( false, false );
					m_pButtontrade->SetShowEnable( false, false );
#ifdef ADDED_RELATIONSHIP_SYSTEM
					m_pButtonInviteCouple->SetShowEnable ( false, false );
#endif // ADDED_RELATIONSHIP_SYSTEM

					if ( false == bPartyMember )
					{
						if ( NULL != pStaticText )
						{
							pStaticText->SetShowEnable( true, true );
							pStaticText->GetString( 0 )->msg = GET_STRING( STR_ID_20855 );
							pStaticText->GetString( 1 )->msg.clear();
							pStaticText->GetString( 2 )->msg.clear();
							pStaticText->SetOffsetPos_LUA( 2.f, -5.f );
						}
						if ( NULL != pButtonAddParty )
						{
							pButtonAddParty->SetShowEnable( true, true );
							pButtonAddParty->SetOffsetPos( D3DXVECTOR2( 0.f, -26.f ) );
						}
						
						fButtonWidth = 58.f;
						m_nMenuCount = 1;

						ChangeTopImage( true );
					}
					else
					{
						if ( NULL != pStaticText )
						{
							pStaticText->SetShowEnable( true, true );
							pStaticText->SetOffsetPos_LUA( 0.f, 0.f );
							pStaticText->GetString( 0 )->msg = GET_STRING( STR_ID_20854 );
							pStaticText->GetString( 1 )->msg = GET_STRING( STR_ID_20855 );
							pStaticText->GetString( 2 )->msg = GET_STRING( STR_ID_20856 );
						}
						if ( NULL != pButtonMovetoParty )
							pButtonMovetoParty->SetShowEnable( true, true );
						if ( NULL != pButtonAddParty )
						{
							pButtonAddParty->SetShowEnable( true, true );
							pButtonAddParty->SetOffsetPos( D3DXVECTOR2( 2.f, 3.f ) );
						}
						if ( NULL != pButtonDropOut )
							pButtonDropOut->SetShowEnable( true, true );

						fButtonWidth = 58.f;
						m_nMenuCount = 2;
					}
				}
				else
				{
					if ( NULL != pStaticText )
						pStaticText->SetShowEnable( false, false );
					if ( NULL != pButtonMovetoParty )
						pButtonMovetoParty->SetShowEnable( false, false );
					if ( NULL != pButtonAddParty )
						pButtonAddParty->SetShowEnable( false, false );
					if ( NULL != pButtonDropOut )
						pButtonDropOut->SetShowEnable( false, false );
				}
			}
		}
	}
	
	--m_nMenuCount;

	for ( int i = 0; i <= m_nMenuCount; ++i )
		pStaticBar->GetPicture( i )->SetShow( true );

	m_pPicCenterTop->SetSizeX( fButtonWidth );
	m_pPicLeftMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
	m_pPicCenterMiddle->SetSizeX( fButtonWidth );
	m_pPicCenterMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
	m_pPicRightMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
	m_pPicCenterBottom->SetSizeX( fButtonWidth );

	offsetPos = m_pPicCenterTop->GetPos();
	offsetPos.x += fButtonWidth;
	m_pPicRightTop->SetPos( offsetPos );

	offsetPos = m_pPicCenterMiddle->GetPos();
	offsetPos.x += fButtonWidth;
	m_pPicRightMiddle->SetPos( offsetPos );

	offsetPos = m_pPicLeftMiddle->GetPos();
	offsetPos.y += (m_nMenuCount * fButtonHeight);
	m_pPicLeftBottom->SetPos( offsetPos );

	offsetPos = m_pPicCenterMiddle->GetPos();
	offsetPos.y += (m_nMenuCount * fButtonHeight);
	m_pPicCenterBottom->SetPos( offsetPos );

	if ( 0 == m_nMenuCount )
	{
		offsetPos = m_pPicLeftBottom->GetPos();
		offsetPos.y -= 33.f;
		m_pPicLeftBottom->SetPos( offsetPos );

		offsetPos = m_pPicCenterBottom->GetPos();
		offsetPos.y -= 33.f;
		m_pPicCenterBottom->SetPos( offsetPos );

		offsetPos = m_pPicRightBottom->GetPos();
		offsetPos.y -= 33.f;
		m_pPicRightBottom->SetPos( offsetPos );
	}

	offsetPos = m_pPicCenterBottom->GetPos();
	offsetPos.x += fButtonWidth;
	m_pPicRightBottom->SetPos( offsetPos );
}

bool CX2CharPopupMenu::SetPopupMenu( UidType iUnitUID, bool bPartyMember/* = false*/ )
{
	CKTDGUIStatic* pStaticFaceImage = (CKTDGUIStatic*) m_pDlgMenu->GetControl( L"character_face_image" );
	if( NULL != pStaticFaceImage && 
		NULL != pStaticFaceImage->GetPicture(0) )
	{
		CKTDGUIControl::CPictureData* pPicture = pStaticFaceImage->GetPicture(0);

		wstring fileName;
		wstring pieceName;
		if( true == CX2Data::GetCharacterImageName( fileName, pieceName, m_eUnitClass, CX2Data::CIT_20by20 ) )
		{
			pPicture->SetSize( D3DXVECTOR2( 20, 20) );
			pPicture->SetTex( fileName.c_str(), pieceName.c_str() );

			CX2GageManager* pGageManager = CX2GageManager::GetInstance();

			if ( NULL != pGageManager )
			{
				const CX2MyGageUI* pGageUI = pGageManager->GetMyGageUI();

				if ( NULL != pGageUI )
				{
					if ( true == (pGageUI)->GetOnPopup() )
						pPicture->SetShow( false );
					else
						pPicture->SetShow( true );
				}
			}
		}
		else
		{
			ASSERT( !"NO" );
		}
	}
	

	// 현재 일반모드/파티/파티장인지 구분하여 현재 모드를 설정한다.		
	USER_MENU eMode;
	eMode= UM_NORMAL_NORMAL;			// 일반->일반
	//m_eMode = UM_NORAML_PARTY;		// 일반->파티원
	//m_eMode = UM_NORMAL_PARTYL;		// 일반->파티장
	//m_eMode = UM_PARTY_NORMAL;		// 파티원->일반
	//m_eMode = UM_PARTY_PARTY,			// 파티원->파티원
	//m_eMode = UM_PARTY_PARTYL;		// 파티원->파티장
	//m_eMode = UM_PARTYL_NORMAL;		// 파티장->일반
	//m_eMode = UM_PARTYL_PARTY;		// 파티장->파티원	


	if( NULL != g_pData->GetPartyManager() )
	{
		if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
		{
			if( NULL != g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iUnitUID ) )
			{
				eMode = UM_PARTYL_PARTY;
			}
			else
			{
				eMode = UM_PARTYL_NORMAL;
			}
		}
		else if( true == g_pData->GetPartyManager()->DoIHaveParty() )
		{
			if( NULL != g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iUnitUID ) )
				eMode = UM_PARTY_PARTY;
			else
				eMode = UM_PARTY_NORMAL;
		}
		else
		{
			eMode = UM_NORMAL_NORMAL;
		}
	}
	else
	{
		eMode = UM_NORMAL_NORMAL;
	}


	SetMode( eMode, bPartyMember );
	return true;
}

bool CX2CharPopupMenu::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef SERV_INTEGRATION
#ifdef EXTEND_SERVER_GROUP_MASK
	int	iServerGroupID = -1;
#else
	SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
#endif // EXTEND_SERVER_GROUP_MASK
#endif SERV_INTEGRATION
	switch(wParam)
	{
	case UMUI_INFO:
		ClosePopupMenu();
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WATCH, true, m_iUid);
		return true;
	case UMUI_WATCH:	
		ClosePopupMenu();
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WATCH, true, m_iUid);
		return true;
	case UMUI_WHISPER:	
		ClosePopupMenu();

#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
		//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
#ifdef EXTEND_SERVER_GROUP_MASK
		iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iUid);
		// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
		if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
		eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iUid);
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

		g_pChatBox->OnOpenWhisperChatMode();
		return true;

	case UMUI_FRIEND:		
		{

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
			ClosePopupMenu();
#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
#ifdef EXTEND_SERVER_GROUP_MASK
			iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iUid);
			if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5126 ), g_pMain->GetNowState() );
				return true;
			}
#else
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iUid);
			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5126 ), g_pMain->GetNowState() );
				return true;
			}
#endif // EXTEND_SERVER_GROUP_MASK
			//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK
			// 체험 아이디 제한 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}
			if( m_iUid != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
#ifdef NEW_MESSENGER
				wstring addFriendMessage = GET_STRING( STR_ID_74 );
				g_pData->GetMessenger()->Handler_EGS_REQUEST_FRIEND_REQ( m_wstrName.c_str(), addFriendMessage );
#else
				g_pData->GetMessenger()->Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( m_wstrName.c_str() );
#endif
				g_pData->GetMessenger()->SetOpen( true );
			}
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP

			return true;		

		} break;

	case UMUI_DISCIPLE:	
		{
			ClosePopupMenu();

			if( m_iUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;

#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
#ifdef EXTEND_SERVER_GROUP_MASK
			iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iUid);
			if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iUid);
			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
#endif // EXTEND_SERVER_GROUP_MASK
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5128 ), g_pMain->GetNowState() );
				return true;
			}
			//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

			// 체험 아이디 제한 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}

			bool bLevelCheckOK = true;

			if( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM ||
				g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_ROOM )
			{
				switch( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_PVP_ROOM:
				case CX2Main::XS_DUNGEON_ROOM:
					{
						CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( m_iUid );

						if( NULL != pSlotData &&
							NULL != pSlotData->m_pUnit )
						{
							if( pSlotData->m_pUnit->GetUnitData().m_Level >= 10 )
							{
								bLevelCheckOK = false;
							}
						}
					} break;
				}
			}
			else
			{
				CX2SquareUnit* pSquareUnit = NULL;
				if(g_pSquareGame != NULL)
				{
					g_pSquareGame->GetSquareUnitByUID( m_iUid );
				}
				else if(g_pTFieldGame != NULL)
				{
					g_pTFieldGame->GetSquareUnitByUID( m_iUid );
				}
				if( NULL != pSquareUnit && NULL != pSquareUnit->GetUnit() )
				{
					if( pSquareUnit->GetUnit()->GetUnitData().m_Level >= 10 )
					{
						bLevelCheckOK = false;
					}
				}				
				if( NULL != g_pMain->GetTutorSystem() )
				{
					const CX2TutorSystem::TutorUnit* pTutorUnit = g_pMain->GetTutorSystem()->GetTutorUnit( m_iUid );
					if( NULL != pTutorUnit )
					{
						if( pTutorUnit->m_iLevel >= 10 )
						{
							bLevelCheckOK = false;
						}
					}
				}	
#if 0 
				CX2PartyManager::PartyMemberData* pPartyUserInfo = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( m_iUid );
				if( NULL != pPartyUserInfo )
				{
					if( pPartyUserInfo->m_iUnitLevel >= 10 )
					{
						bLevelCheckOK = false;
					}
				}
#endif
			}

			if( false == bLevelCheckOK )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_41 ), g_pMain->GetNowState() );
				return true;
			}


			if( NULL != g_pMain->GetTutorSystem() )
			{
				g_pMain->GetTutorSystem()->SetTutorInviteeName( m_wstrName );
				g_pMain->GetTutorSystem()->Handler_EGS_REQUEST_TUTORIAL_REQ( m_iUid );
			}
			return true;
		}
		break;	
	case UMUI_LEADER:
		if( NULL != g_pData->GetPartyManager() )
		{
			g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_HOST_REQ( m_iUid );
		}
		
		ClosePopupMenu();
		return true;
	case UMUI_PARTY:
		
		if(g_pData != NULL && g_pData->GetPartyManager() != NULL && g_pTFieldGame != NULL)
		{
			UidType iPartyUid = g_pTFieldGame->GetUserPartyUid(m_iUid);
			if(iPartyUid > 0)
			{
				ASSERT( !L"EGS_JOIN_PARTY_REQ" );

				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_REQUESTING_JOIN_PARTY, 
					CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 7.f, GET_STRING( STR_ID_42 ), 
					(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
			}			
		}	

		ClosePopupMenu();
		return true;

	case UMUI_INVITE:

		if( NULL != g_pData->GetPartyManager() )
		{
#ifdef EXTEND_SERVER_GROUP_MASK
			int iServerGroudID = -1;
			iServerGroudID = g_pMain->ExtractServerGroupID( m_iUid );

			if( g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ( m_iUid, m_wstrName , iServerGroudID ) == true )
#else EXTEND_SERVER_GROUP_MASK
			SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iUid);

			if( g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ( m_iUid, m_wstrName , eServerGroupID ) == true )
#endif EXTEND_SERVER_GROUP_MASK
			{
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				//{{ kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
				userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
				userData.iOpponentUID	= m_iUid;
				//}} kimhc // 2010-05-06 // 파티초대 실패 시 팝업창 삭제(UID 값을 비교)
				g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_PARTY_LEADER_INVITE_PARTY,
					CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 7.f, GET_REPLACED_STRING( ( STR_ID_43, "L", m_wstrName ) ), 
					(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
			}			
		}
		ClosePopupMenu();
		return true;

	case UMUI_OUT:
		ClosePopupMenu();
		if( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM)
		{
			// 방에서 강퇴(파티아님)
			if( m_iUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				return true;

			CX2StatePVPRoom* pkPVPRoom = (CX2StatePVPRoom*) g_pMain->GetNowState();
			pkPVPRoom->OnBanUnit();
		}
		else
		{
			if( NULL != g_pData->GetPartyManager() )
			{
				g_pData->GetPartyManager()->Handler_EGS_PARTY_BAN_USER_REQ( m_iUid );
			}
		}				
		return true;
	case UMUI_TRADE:
		ClosePopupMenu();

#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
		//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
#ifdef EXTEND_SERVER_GROUP_MASK
		iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iUid);
		if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
		eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iUid);
		if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
#endif // EXTEND_SERVER_GROUP_MASK
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5125 ), g_pMain->GetNowState() );
			return true;
		}
		//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

		// 체험 아이디 제한 
		if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
			return true;
		}
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_TRADE, true, m_iUid);
		return true;

		//{{ kimhc // 2009-09-29 // 길드 초대
#ifdef	GUILD_MANAGEMENT
	case UMUI_INVITE_GUILD_MEMBER:
		{
			if ( g_pData->GetGuildManager() == NULL )
			{
				ASSERT( !L"GuildManager is NULL" );
				return false;
			}

			ClosePopupMenu();

#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
#ifdef EXTEND_SERVER_GROUP_MASK
			iServerGroupID = (int) g_pMain->ExtractServerGroupID(m_iUid);
			if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iUid);
			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
#endif // EXTEND_SERVER_GROUP_MASK
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5127 ), g_pMain->GetNowState() );
				return true;
			}
			//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

			return g_pData->GetGuildManager()->Handler_EGS_INVITE_GUILD_REQ( m_iUid );
		}
		break;
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-29 // 길드 초대
	case UMUI_INFO_SIMPLE:
		{
			ClosePopupMenu();

//#ifdef SERV_INTEGRATION //2013.4.11 다른 서버군도 프로필창이 보이도록 주석처리
//			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
//			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(m_iUid);
//			// 우클릭한 대상 유닛의 서버가 자신의 서버와 다를 경우
//			if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5129 ), g_pMain->GetNowState() );
//				return true;
//			}
//			//}}
//#endif SERV_INTEGRATION

#ifdef SERV_LOCAL_RANKING_SYSTEM
			CX2State* pState = static_cast<CX2State*>( g_pMain->GetNowState() );
			if( pState != NULL )
				pState->Handler_EGS_LOCAL_RANKING_WATCH_UNIT_REQ( m_iUid );
#else
			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			if ( pState != NULL )
				pState->Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( m_wstrName.c_str() );
#endif //SERV_LOCAL_RANKING_SYSTEM
		}
		break;


	case UMUI_MOVE_TO_PARTY:	/// 파티원 있는 곳으로 이동
		{
			CX2PartyManager::PartyData* pPartyData = g_pData->GetPartyManager()->GetMyPartyData();
			if ( NULL != pPartyData && pPartyData->GetPartyMemberCount() > 1 )	/// 파티가 있는 겨우
			{
				CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
				switch ( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_BATTLE_FIELD:
					{
						if ( NULL != g_pX2Game )
						{
							const CX2GUUser* pMyGUUser = g_pX2Game->GetMyUnit();
							if ( NULL != pMyGUUser && pMyGUUser->GetNowHp() > 0 )
							{
								CX2State* pState = static_cast<CX2State*>( g_pMain->GetNowState() );
								if ( pState != NULL )
									pState->Handler_EGS_JOIN_BATTLE_FIELD_REQ( battleFieldManager.GetBattleFieldIdWhereIam(), 
									battleFieldManager.GetBattleFieldPositionIndexWhereIShouldBe(),
									battleFieldManager.GetBattleFieldPositionValue(), true );
							}
						}					
					} break;

				default:
					break;
				}	/// switch
			}	/// if

		} break;

	case UMUI_INPUT_PARTY:
		{
			OnPartyInput( true );
		} break;

	case UMUI_DROP_OUT:
		{
			ClosePopupMenu();

			if( g_pData->GetPartyManager()->DoIHaveParty() == false )
				return true;

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

	case UMUI_INPUT_EXIT:
		{
			OnPartyInput( false );
		} break;
		
	case UMUI_INVITE_PARTY_NAME:
		{
			if ( NULL != m_pDlgMenu )
			{
				CKTDGUIIMEEditBox*	pIMEPartyName = NULL;
				pIMEPartyName	= (CKTDGUIIMEEditBox*)m_pDlgMenu->GetControl( L"IMEPartyName" );

				if ( NULL != pIMEPartyName )
				{
#ifdef EXTEND_SERVER_GROUP_MASK
					int iServerGroupID = -1;
#else EXTEND_SERVER_GROUP_MASK
					int iServerGroupID = SGI_SOLES;
#endif EXTEND_SERVER_GROUP_MASK

#ifndef REMOVE_INVITE_PARTY_SERVER_BUTTON
					CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*>(m_pDlgMenu->GetControl(L"Gaia"));
					if( NULL != pRadio )
					{
						if( true == pRadio->GetChecked() )
						{
							iServerGroupID = SGI_GAIA;
						}
					}
#endif // REMOVE_INVITE_PARTY_SERVER_BUTTON

#ifdef SERV_SEPARATE_SERVERSET_US_USER
					iServerGroupID = g_pInstanceData->GetServerGroupID();
					// 이 변수의 의미는 '초대하는 상대방의 서버군' 이다. - 1 넣어서 서버군 체크 하지 않도록 하자
#ifdef EXTEND_SERVER_GROUP_MASK
					iServerGroupID = -1;
#else //EXTEND_SERVER_GROUP_MASK
					iServerGroupID = SGI_INVALID;
#endif //EXTEND_SERVER_GROUP_MASK
#endif //SERV_SEPARATE_SERVERSET_US_USER 

					wstring wstrTargetNickName = pIMEPartyName->GetText();
					if ( wstrTargetNickName.length() > 0 )
						g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ( wstrTargetNickName, iServerGroupID );
				}

				OnPartyInput( false );
			}
		} break;
	case UMUI_SELECT_INVITE_MEMBER_SEVER:
		{
			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;
			if( NULL != pRadioButton )
			{
				pRadioButton->SetChecked(true);
			}
			return true;
		} break;

		//{{ kimhc // 2010-03-15 // 포커스 잃은경우 Exit 되도록 처리
	case UMUI_EXIT:
		{
			ClosePopupMenu();
		}
		break;
		//}} kimhc // 2010-03-15 // 포커스 잃은경우 Exit 되도록 처리

#ifdef ADDED_RELATIONSHIP_SYSTEM	
	case UMUI_COUPLE :
		{	
			CX2RelationshipManager * pRelationManager = g_pData->GetRelationshipManager();
			if ( NULL != pRelationManager )
			{
				if ( NULL != pRelationManager->GetMyRelationshipInfo() )
				{
					if ( SEnum::RT_SOLO == pRelationManager->GetMyRelationshipInfo()->m_eRelationStateType )
					{						
						if ( NULL != g_pData->GetRelationshipManager() )
						{

							g_pData->GetRelationshipManager()->Send_EGS_COUPLE_PROPOSE_REQ(m_wstrName);
							ClosePopupMenu ();
							
							return true;
						}
					}
				}
			}
			return true;
		}
		break;

	/// mauntain // 2013-05-03 // 결혼 시스템 배우자 소환 기능
	case UMUI_SUMMON :
		{
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

			ClosePopupMenu ();
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM	
	}

	return false;
}

bool CX2CharPopupMenu::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;
}

void CX2CharPopupMenu::OpenUserPopupMenu( UidType iUnitUID, bool bPartyMember /*= false*/ )
{
	m_iUid = iUnitUID;	

	SetOnPopup();

	SetUnit( iUnitUID, bPartyMember );
	if( SetPopupMenu( iUnitUID, bPartyMember ) == false )
	{
		m_pDlgMenu->SetShowEnable(false, false);
		m_bShow = false;
		m_pDlgPopup->SetShowEnable( false, false );
		return;
	}

	for(int i=0; i<9; ++i)
		m_pPicLevelTen[i]->SetShow(false);
	for(int i=0; i<=9; ++i)
		m_pPicLevelOne[i]->SetShow(false);

	//if(m_pUnit != NULL)

	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	if ( NULL != pGageManager )
	{
		const CX2MyGageUI* pGageUI = pGageManager->GetMyGageUI();

		if ( NULL != pGageUI )
		{
			CKTDGUIStatic *pStaticDlgTen = m_pDlgMenu->GetStatic_LUA("ten");
			CKTDGUIStatic *pStaticDlgOne = m_pDlgMenu->GetStatic_LUA("one");

			if ( false == (pGageUI)->GetOnPopup() )
			{
				if ( NULL != pStaticDlgTen )
					pStaticDlgTen->SetShowEnable( true, true );
				if ( NULL != pStaticDlgOne )
					pStaticDlgOne->SetShowEnable( true, true );

				m_pStaticName->GetString(0)->msg = m_wstrName;
				m_pStaticName->SetShow( true );

				int tenL, oneL;
				tenL = m_iUnitLevel / 10;
				oneL = m_iUnitLevel % 10;

				if(tenL > 0)
					m_pPicLevelTen[tenL-1]->SetShow(true);
				else if(tenL == 0)
					m_pPicLevelTen[9]->SetShow(true);
				if(oneL == 0)
					oneL = 10;
				m_pPicLevelOne[oneL-1]->SetShow(true);
			}
			else
			{
				if ( NULL != pStaticDlgTen )
					pStaticDlgTen->SetShowEnable( false, false );
				if ( NULL != pStaticDlgOne )
					pStaticDlgOne->SetShowEnable( false, false );
			}
		}
	}

	m_pDlgPopup->SetFront(true);

	//m_pDlgMenu->SetModal(false);
	m_pDlgMenu->SetFront(true);

	// 팝업메뉴 띄울 위치 지정
	D3DXVECTOR2 vMousePos;
	vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
	vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

	
	// 팝업 메뉴가 화면 밖으로 벗어나지 않게 수정
	if( vMousePos.x > 860.f )
	{
		vMousePos.x = 860.f;
	}

	if( vMousePos.x < 0.f )
	{
		vMousePos.x = 0.f;
	}



	if( vMousePos.y > 768.f - (float) m_nMenuCount * 30.f )
	{
		vMousePos.y = 768.f - (float) m_nMenuCount * 30.f;
	}

	if( vMousePos.y < 0.f )
	{
		vMousePos.y = 0.f;
	}
	

	m_pDlgMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x, (float)vMousePos.y ) );


	m_pDlgPopup->SetPos( D3DXVECTOR2( (float)vMousePos.x, (float)vMousePos.y ) );
	m_pDlgPopup->SetShowEnable( true, true );

	// 일반/파티/파티장 모드 설정
	m_pDlgMenu->SetShowEnable(true, true);

	
	m_bShow = true;
}

void CX2CharPopupMenu::ClosePopupMenu()
{
	if ( NULL != m_pDlgPopup )
		m_pDlgPopup->SetShowEnable(false, false);

	OnPartyInput( false );
	m_pDlgMenu->SetShowEnable(false, false);

	m_bShow = false;
	//m_pUnit = NULL;
	m_nMenuCount = 0;
	//m_iUid = -1;
}

void CX2CharPopupMenu::OnPartyInput( bool bOn_ )
{
	if ( NULL != m_pDlgMenu )
	{
		CKTDGUIStatic *pStaticDlgInput = m_pDlgMenu->GetStatic_LUA("user_party");
		
		if ( NULL != pStaticDlgInput )
		{
			pStaticDlgInput->SetShowEnable( bOn_, bOn_ );
			CKTDGUIButton* pExitButton = (CKTDGUIButton*)m_pDlgMenu->GetControl( L"Exit" );
			if ( NULL != pExitButton )
				pExitButton->SetShowEnable( bOn_, bOn_ );
			
			CKTDGUIIMEEditBox* pIMEPartyName = (CKTDGUIIMEEditBox*)m_pDlgMenu->GetControl( L"IMEPartyName" );
			if ( NULL != pIMEPartyName )
			{
				pIMEPartyName->ClearText();
				pIMEPartyName->SetShowEnable( bOn_, bOn_ );
			}
		}

#ifndef REMOVE_INVITE_PARTY_SERVER_BUTTON
		CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*>(m_pDlgMenu->GetControl(L"Soles"));
		if( NULL != pRadio )
		{
			pRadio->SetChecked(true);
			pRadio->SetShowEnable(bOn_, bOn_);
		}

		pRadio = static_cast<CKTDGUIRadioButton*>(m_pDlgMenu->GetControl(L"Gaia"));
		if( NULL != pRadio )
		{
			pRadio->SetShowEnable(bOn_, bOn_);
			if( true == bOn_ && SGI_GAIA == g_pInstanceData->GetServerGroupID())
			{
				pRadio->SetChecked(true);
			}
		}
#endif // REMOVE_INVITE_PARTY_SERVER_BUTTON

		m_pDlgMenu->GetControl(L"IMEPartyName")->RequestFocus();
	}
}

void CX2CharPopupMenu::SetOnPopup()
{
	if( NULL != CX2GageManager::GetInstance() && 
		NULL != CX2GageManager::GetInstance()->GetMyGageUI() )
	{
		const CX2MyGageUI* pGageUI = CX2GageManager::GetInstance()->GetMyGageUI();

		if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
		{
			if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
			{
				if( m_iUid == g_pX2Game->GetMyUnit()->GetUnitUID() )
				{
					const_cast<CX2MyGageUI*>(pGageUI)->SetOnPopup(true);
				}
			}
		}
		else if( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
		{
			if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetMyUnit())
			{
				if( m_iUid == g_pTFieldGame->GetMyUnit()->GetUID() )
				{
					const_cast<CX2MyGageUI*>(pGageUI)->SetOnPopup(true);
				}
			}
		}
	}
}
