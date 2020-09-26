#include "StdAfx.h"

#include ".\x2statedungeonroom.h"


CX2StateDungeonRoom::CX2StateDungeonRoom(void) :
m_pCX2DungeonRoom( NULL ),	
m_pDLGDungeonRoomBack( NULL ),
m_pDLGDungeonRoomFront( NULL ),
m_pDLGLoadingState( NULL ),
m_pDLGTalkBox( NULL ),
m_pTalkBoxMgr( NULL ),
m_bTempEventSkip( false ),
//m_DungeonRoomUserBoxList[MAX_DUNGEON_ROOM_USER_NUM];
m_pFontForIPAndPort( NULL ),
m_fMotionChangeTime( 0.f ),
m_fMotionChangeRefreshTime( 0.f ),
m_bEnableMotionChange( 0.f ),
m_iUserPopupSlotIdx( 0 ),
m_bBaned( false ),
m_pDLGMsgBoxForBan( NULL ),
m_UnitUIDToBan( 0 ),
m_fTimeLeftStartReq( 0.f ),
m_fElapsedIdleTime( 0.f ),
m_bCheckSendLeaveRoomByRequireLevel( false ),
m_pDLG_Dungeon_Help( NULL ),
m_bReserveCashShopOpen( NULL )
{
	DialogLog( "\nCX2StateDungeonRoom::CX2StateDungeonRoom Start\n" );


	// 변수 초기화
	ZeroMemory( m_DungeonRoomUserBoxList, sizeof(m_DungeonRoomUserBoxList) );



	// 변수 값 할당
	g_pMain->ResetDungeonRewardItem();
	m_pCX2DungeonRoom	= g_pData->GetDungeonRoom();

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
				g_pKTDXApp->GetDGManager()->GetFar(), false );

	
	m_fMotionChangeRefreshTime = (float)RandomNumber( 10, 17 );

	m_bChangeDifficultyLower = false;


	UISetting();

	m_pFontForIPAndPort = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );

	SoundReady( L"Difficulty_Open.ogg" );

	OpenDungeonHelpPopUp();


	g_pKTDXApp->SkipFrame();

	DialogLog( "\nCX2StateDungeonRoom::CX2StateDungeonRoom End\n" );
}

CX2StateDungeonRoom::~CX2StateDungeonRoom(void)
{

	DialogLog( "\nCX2StateDungeonRoom::~CX2StateDungeonRoom Start\n" );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
	SAFE_DELETE( m_pDLGLoadingState ); // note: 얘는 다이얼로그 매니저에서 관리되는 애가 아니라서~ 요렇게
#endif // DIALOG_HANDLE_TEST // 2009-8-19


	if( NULL != m_pDLGDungeonRoomBack )
	{
		m_pDLGDungeonRoomBack->SetShowEnable( false, false );
	}

	if( NULL != m_pDLGDungeonRoomFront )
	{
		m_pDLGDungeonRoomFront->SetShowEnable( false, false );
	}


	SAFE_DELETE_DIALOG( m_pDLGTalkBox );


	SAFE_DELETE_DIALOG( m_pDLGMsgBoxForBan );


	for ( int i = 0; i < MAX_DUNGEON_ROOM_USER_NUM; i++ )
	{
		Dungeon_ROOM_USER_BOX* pDungeonRoomUserBox = m_DungeonRoomUserBoxList[i];
		SAFE_DELETE( pDungeonRoomUserBox );

		m_DungeonRoomUserBoxList[i] = NULL;
	}


	SAFE_DELETE_DIALOG( m_pDLG_Dungeon_Help );

/*
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
		g_pKTDXApp->GetDGManager()->GetFar(), true );
*/
//	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForIPAndPort );

	if ( m_bReserveCashShopOpen == true )
	{
		if ( g_pData->GetCashShop() != NULL )
			g_pData->GetCashShop()->SetAutoOpen( true );		
	}

	DialogLog( "\nCX2StateDungeonRoom::~CX2StateDungeonRoom End\n" );
}


HRESULT CX2StateDungeonRoom::OnFrameMove( double fTime, float fElapsedTime )
{

	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );

	if( true == g_pMain->GetIsPlayingTutorial() ||
		true == g_pMain->GetIsExitingTutorial() ||
		true == g_pMain->GetPlayFieldDungeon() ||
		true == g_pMain->GetExitFieldDungeon())
		return S_OK;
	


	// 일정기간 입력이 없으면 방에서 나간다
	const float MAGIC_IDLE_TIME = 600.f; 
	m_fElapsedIdleTime += fElapsedTime;
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetNumKeyPresses() > 0 )
	{
		m_fElapsedIdleTime = 0.f;
	}
	if( 0 != g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() ||
		0 != g_pKTDXApp->GetDIManager()->GetMouse()->GetYDelta() )
	{
		m_fElapsedIdleTime = 0.f;
	}
	if( m_fElapsedIdleTime > MAGIC_IDLE_TIME )
	{
		m_fElapsedIdleTime = 0.0f;
		Handler_EGS_LEAVE_ROOM_REQ();
	}
/*
	if ( m_bCheckSendLeaveRoomByRequireLevel == false && g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
	{
		const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( g_pData->GetDungeonRoom()->GetDungeonID() );
		if ( pDungeonData != NULL )
		{
			if ( g_pData->GetSelectUnitLevel() < pDungeonData->m_RequireLevel )
			{
				Handler_EGS_LEAVE_ROOM_REQ();
				m_bCheckSendLeaveRoomByRequireLevel = true;
			}
		}
	}
*/	


//	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );
	m_pCX2DungeonRoom->OnFrameMove( fTime, fElapsedTime );
	
	//////////////////////////////////////////////////////////////////////////

	m_fMotionChangeTime += fElapsedTime;

	if ( m_fMotionChangeTime >= m_fMotionChangeRefreshTime )
	{
		m_fMotionChangeTime = 0;
		m_fMotionChangeRefreshTime = (float)RandomNumber( 10, 17  );

		ChangeMotion();
	}

	//////////////////////////////////////////////////////////////////////////



	if( m_fTimeLeftStartReq > 0.f )
	{
		m_fTimeLeftStartReq -= fElapsedTime;
		if( m_fTimeLeftStartReq < 0.f )
		{
			m_fTimeLeftStartReq = 0.f;
		}
	}

	UIFrameMove();

#ifdef POSTBOX
    //// 우편함처리
    //if(g_pMain->GetPostBox()->GetDialogShow() == true && g_pMain->GetPostBox()->GetCheckPitIn() == false)
    //{
    //    if(g_pMain->GetPostBox()->GetSwitchState() == false)
    //    {
    //        if ( g_pData->GetDungeonRoom()->GetMySlot()->m_bReady == true )
    //        {
    //            Handler_EGS_CHANGE_READY_REQ();
    //        }

    //        Handler_EGS_CHANGE_PITIN_REQ();
    //    }
    //    g_pMain->GetPostBox()->SetCheckPitIn(true);
    //}

    //if(g_pMain->GetPostBox()->GetDialogShow() == false && g_pMain->GetPostBox()->GetCheckPitIn() == true)
    //{
    //    if ( g_pData->GetDungeonRoom()->GetMySlot()->m_bReady == true )
    //    {
    //        Handler_EGS_CHANGE_READY_REQ();
    //    }

    //    Handler_EGS_CHANGE_PITIN_REQ();
    //    g_pMain->GetPostBox()->SetCheckPitIn(false);
    //}    
#endif

	//if ( m_pCharacterRoom->GetOpen() == true )
#ifndef NEW_SKILL_TREE
	if( true == g_pInstanceData->GetSkillTreeUI()->GetShow() )
	{
		for ( int i = 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );
			if ( pSlotData == NULL || pSlotData->m_pUnitViewer == NULL)
				continue;

			pSlotData->m_pUnitViewer->SetShowObject( false );
		}

		for ( int i = 0; i < MAX_DUNGEON_ROOM_USER_NUM; i++ )
		{
			Dungeon_ROOM_USER_BOX* pDungeonRoomUserBox = m_DungeonRoomUserBoxList[i];
			if ( pDungeonRoomUserBox->pDialog == NULL )
				continue;

			pDungeonRoomUserBox->SetShow( false );
		}
	}
	else
#endif
	{
		for ( int i = 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );
			SetUserBox(  pSlotData );
		}
	}


#ifdef _SERVICE_
#ifdef SERV_KEY_MAPPING_INT
	if( ( GET_KEY_STATE( GA_PARTYREADY )  == TRUE
		|| g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F8) == TRUE )
#else // SERV_KEY_MAPPING_INT
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F8) == TRUE 
#endif // SERV_KEY_MAPPING_INT
#ifdef DIALOG_SHOW_TOGGLE
		&& g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false
#endif
		)

	{
		if( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
		{
#ifndef NOT_APPLY_F8
			Handler_EGS_STATE_CHANGE_GAME_START_REQ();
#endif
		}
		else
		{
			//{{ 2007. 10. 5  최육사  던전룸에서 내 정보 보기 설정 (에러창 안띄우기)
			g_pData->GetMyUser()->GetSelectUnit()->SetMyInfoInDungeonRoom( true );
			//}}
			Handler_EGS_CHANGE_READY_REQ();
		}
	}
#endif





	if ( g_pX2Room != NULL )
	{
		if ( g_pX2Room->GetBan() == true )
		{
			g_pX2Room->SetBan( false );

			m_bBaned = true;
			SAFE_DELETE_DIALOG( m_pDLGMsgBoxForBan );
			m_pDLGMsgBoxForBan = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_623 ), this );
		}

	}

	if ( m_bBaned == true )
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGMsgBoxForBan ) == false )
		{
			m_pDLGMsgBoxForBan = NULL;
			m_bBaned = false;
			Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();
		}
	}

	if ( g_pData->GetDungeonRoom() != NULL && g_pData->GetDungeonRoom()->GetNotEnoughSpirit() == true )
	{
		g_pData->GetDungeonRoom()->SetNotEnoughSpirit( false );
		Handler_EGS_LEAVE_ROOM_REQ();
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_624 ), this );
	}






	//마우스 체크
	if( g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_RBUTTON ) == TRUE
#ifndef NEW_SKILL_TREE
	  	&& false == g_pInstanceData->GetSkillTreeUI()->GetShow() 
#endif
		/*m_pCharacterRoom->GetOpen() == false */
#ifdef POSTBOX
        //&& (g_pMain != NULL && g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetDialogShow() == false)
#endif
        )
	{
		POINT mousePoint;
		GetCursorPos( &mousePoint );
		ScreenToClient( g_pKTDXApp->GetHWND(), &mousePoint );
		D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

		int selectSlot = m_pCX2DungeonRoom->GetClickSlot( mousePoint.x, mousePoint.y );
		if( selectSlot != -1 )
		{
			CX2Room::SlotData* pkSlotData = m_pCX2DungeonRoom->GetSlotData( selectSlot );
			if( pkSlotData != NULL && pkSlotData->m_UnitUID > 0 && pkSlotData->m_pUnit != NULL )
			{
				m_iUserPopupSlotIdx = selectSlot;

				if(g_pData->GetUserMenu() != NULL)
				{
					g_pData->GetUserMenu()->OpenUserPopupMenu(pkSlotData->m_UnitUID);
				}
				//g_pPartyMsgerUI->ChangeUserListWindowMode( CX2PartyMsgerUI::ULWM_TOTAL );
				//g_pPartyMsgerUI->OpenUserPopupMenu( pkSlotData->m_UnitUID, pkSlotData->m_pUnit->GetNickName(), true );
			}
		}
	}

	return S_OK;
}

bool CX2StateDungeonRoom::IsInUnitReact( int iIdx, D3DXVECTOR2 mousePos )
{
	return false;
	D3DXVECTOR3 kSlotPos_ = m_pDLGDungeonRoomBack->GetDummyPos( iIdx );
	kSlotPos_.x += 150.f;

	kSlotPos_.y  -= 40.f;

	D3DXVECTOR2 kSlotPos( kSlotPos_.x, kSlotPos_.y );
	D3DXVECTOR2 kSlotSize( 130.f, 180.f );

	D3DXVECTOR2 kConvertPos = g_pKTDXApp->ConvertByResolution( kSlotPos );
	D3DXVECTOR2 kConvertSize = g_pKTDXApp->ConvertByResolution( kSlotSize );

	if ( mousePos.x >= kConvertPos.x  && mousePos.x <= kConvertPos.x + kConvertSize.x &&
		mousePos.y >= kConvertPos.y && mousePos.y <= kConvertPos.y + kConvertSize.y )
	{
		return true;
	}

	return false;
}


HRESULT CX2StateDungeonRoom::OnFrameRender()
{

	if( true == g_pMain->GetIsPlayingTutorial() ||
		true == g_pMain->GetIsExitingTutorial() ||
		true == g_pMain->GetPlayFieldDungeon() ||
		true == g_pMain->GetExitFieldDungeon() )
	{
		if( m_pDLGLoadingState != NULL )
			m_pDLGLoadingState->OnFrameRender();
		return S_OK;
	}

	CX2StateMenu::OnFrameRender();


	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if ( g_pX2Room != NULL )
		{
			m_pFontForIPAndPort->OutTextMultiline( 800, 10, g_pX2Room->GetCenterServerIP().c_str(),
				D3DXCOLOR( 1, 1, 1, 1 ), CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
		}
	}
	

	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )
	{
		if ( g_pX2Room != NULL )
		{
			for ( int i = 0; i < g_pX2Room->GetSlotNum(); i++ )
			{
				CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( i );
				if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
					continue;

				wstring ipAndPort = pSlotData->m_pUnit->GetUnitData().m_IP + L":";
				WCHAR szPort[256] = {0};
				swprintf( szPort, L"%d", pSlotData->m_pUnit->GetUnitData().m_Port );
				ipAndPort += szPort;
				swprintf( szPort, L"\n%d", (int)pSlotData->m_PingTime );
				ipAndPort += szPort;

				if( true == pSlotData->m_pUnit->GetUnitData().m_bMan )
				{
                    ipAndPort += L" ";
                    ipAndPort += GET_STRING( STR_ID_535 );
				}
				else
				{
					ipAndPort += L" ";
                    ipAndPort += GET_STRING( STR_ID_536 );
				}

				ipAndPort += GET_REPLACED_STRING( ( STR_ID_537, "i", pSlotData->m_pUnit->GetUnitData().m_Age ) );




				
				D3DXVECTOR3 tempPos = m_pDLGDungeonRoomBack->GetDummyPos( i );

				m_pFontForIPAndPort->OutTextMultiline( (int)tempPos.x, (int)tempPos.y - 100, ipAndPort.c_str(), D3DXCOLOR( 1, 1, 1, 1 ),  
					CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
			}
		}
	}

	return S_OK;
}



bool CX2StateDungeonRoom::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	return false;
}



bool CX2StateDungeonRoom::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
	{
		return true;
	}


	switch( wParam )
	{
	case DRUCM_READY:
		{
			Handler_EGS_CHANGE_READY_REQ();
			
		}
		break;

	case DRUCM_START:
		Handler_EGS_STATE_CHANGE_GAME_START_REQ();
		break;

	//case DRUCM_BACK_TO_LOCAL_MAP:
		//ClickMenuBackButton();  
		//Handler_EGS_LEAVE_ROOM_REQ();
	//	break;

	case DRUCM_OPEN_OR_CLOSE_BUTTON:
		{
			if( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				Handler_EGS_CHANGE_SLOT_OPEN_REQ( pButton->GetName() );
			}
		} break;

	case DRUCM_DIFFICULTY_LOWER:
		{
			if ( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
			{
				CheckChangeDifficulty( true );
			}								
		} break;

	case DRUCM_DIFFICULTY_HIGHER:
		{
			if ( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
			{
				CheckChangeDifficulty( false );
			}						
		} break;

	case DRUCM_EVENT_SKIP:
		{
			if ( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
			{
				ChangeEventSkip( true );
			}
		} break;

	case DRUCM_NO_EVENT_SKIP:
		{
			if ( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
			{
				ChangeEventSkip( false );
			}	
		} break;

	case DRUCM_BAN_UNIT:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			CKTDGUIDialogType pDialog = pButton->GetDialog();
			if ( pDialog != NULL )
			{
				CKTDGUIStatic* pStaticSlotIndex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticDungeon_Room_SlotIndex" );
				if ( pStaticSlotIndex != NULL )
				{
					int slotIndex = _wtoi( pStaticSlotIndex->GetString(0)->msg.c_str() );
					OnBanUnit( slotIndex );
				}				
			}
		}
		break;

	case DRUCM_BAN_UNIT_OK:
		{
			g_pX2Room->Handler_EGS_BAN_USER_REQ( m_UnitUIDToBan );
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
		}
		break;

	case DRUCM_WHISPER:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			CKTDGUIDialogType pDialog = pButton->GetDialog();
			if ( pDialog == NULL )
				break;

			CKTDGUIStatic* pStaticSlotIndex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticDungeon_Room_SlotIndex" );
			if ( pStaticSlotIndex == NULL )
				break;

			int slotIndex = _wtoi( pStaticSlotIndex->GetString(0)->msg.c_str() );
			if ( slotIndex == -1 )
				break;

			m_iUserPopupSlotIdx = slotIndex;

			CX2Room::SlotData* pkSlotData = m_pCX2DungeonRoom->GetSlotData( slotIndex );
			if ( pkSlotData == NULL )
				break;

			g_pChatBox->SetPickedUser( pkSlotData->m_UnitUID, pkSlotData->m_pUnit->GetNickName() );

			return g_pChatBox->OnOpenWhisperChatMode();

		} break;

	case DRUCM_USER_INFO:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			CKTDGUIDialogType pDialog = pButton->GetDialog();
			if ( pDialog == NULL )
				break;

			CKTDGUIStatic* pStaticSlotIndex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticDungeon_Room_SlotIndex" );
			if ( pStaticSlotIndex == NULL )
				break;

			int slotIndex = _wtoi( pStaticSlotIndex->GetString(0)->msg.c_str() );
			if ( slotIndex == -1 )
				break;

			CX2Room::SlotData* pkSlotData = m_pCX2DungeonRoom->GetSlotData( slotIndex );
			if ( pkSlotData == NULL )
				break;

			if ( pkSlotData->m_pUnit == NULL )
				break;

			return Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( pkSlotData->m_pUnit->GetNickName() );

		} break;
//#ifndef NOT_USE_DICE_ROLL
//	case DRUCM_CHANGE_GET_ITEM_TYPE:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//			{
//				if ( wcscmp( pControl->GetName(), L"RadioButtonRandom" ) == 0 )
//				{
//					Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_REQ( CX2DungeonRoom::DGIT_RANDOM );
//				}
//				else if ( wcscmp( pControl->GetName(), L"RadioButtonPersonal" ) == 0 )
//				{
//					Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_REQ( CX2DungeonRoom::DGIT_PERSON );
//				}
//			}
//			return true;
//		}
//		break;
//#endif //NOT_USE_DICE_ROLL

	case DRUCM_CHALLANGE_NEXT_DIFFICULTY:
		{
			if ( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
			{
				ChangeDifficulty( false );
			}

			if ( m_pDLG_Dungeon_Help != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLG_Dungeon_Help, NULL, false );

			m_pDLG_Dungeon_Help = NULL;
			return true;
		}
		break;

	case DRUCM_CLOSE_DUNGEON_GUIDE:
		{
			if ( m_pDLG_Dungeon_Help != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLG_Dungeon_Help, NULL, false );

			m_pDLG_Dungeon_Help = NULL;
			return true;
		}
		break;

	case DRUCM_CHANGE_DIFFICULTY_OK:
		{			
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			m_pCX2DungeonRoom->SetCharLvLimit(false);
			ChangeDifficulty(m_bChangeDifficultyLower);
			return true;
		}
		break;
	case DRUCM_BAN_CHANGE_DIFFICULTY_OK:
		{			
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			ChangeDifficulty(m_bChangeDifficultyLower);
			return true;
		}
		break;
	}

	return false;
}

void CX2StateDungeonRoom::OnBanUnit( int slotIndex )
{
	if ( slotIndex == -1 )
		slotIndex = m_iUserPopupSlotIdx;

	if ( g_pData->GetDungeonRoom()->GetMySlot()->m_bHost == true || g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( slotIndex );
		if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			//g_pX2Room->Handler_EGS_BAN_USER_REQ( pSlotData->m_pUnit->GetUID() );
			m_UnitUIDToBan = pSlotData->m_pUnit->GetUID();
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_628 ), DRUCM_BAN_UNIT_OK, this );
		}
	}
}

bool CX2StateDungeonRoom::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;


	bool bRetVal = false;
	switch( wParam )
	{
	case EGS_JOIN_ROOM_NOT:
		bRetVal = Handler_EGS_JOIN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		break;


	case EGS_CHANGE_SLOT_OPEN_ACK:
		bRetVal = Handler_EGS_CHANGE_SLOT_OPEN_ACK(  hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_SLOT_OPEN_NOT:
		bRetVal = Handler_EGS_CHANGE_SLOT_OPEN_NOT(  hWnd, uMsg, wParam, lParam );
		break;

	case EGS_LEAVE_ROOM_ACK:
		bRetVal = Handler_EGS_LEAVE_ROOM_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_LEAVE_ROOM_NOT:
		bRetVal = Handler_EGS_LEAVE_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		break;
#if 0
	case EGS_STATE_CHANGE_LOCAL_MAP_ACK:
		bRetVal = Handler_EGS_STATE_CHANGE_LOCAL_MAP_ACK( hWnd, uMsg, wParam, lParam );
		break;
#endif
	case EGS_STATE_CHANGE_GAME_START_ACK:
		bRetVal = Handler_EGS_STATE_CHANGE_GAME_START_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_GAME_START_NOT:
		bRetVal = Handler_EGS_STATE_CHANGE_GAME_START_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_READY_ACK:
		bRetVal = Handler_EGS_CHANGE_READY_ACK(  hWnd, uMsg, wParam, lParam );
		break;
	case EGS_CHANGE_READY_NOT:
		bRetVal = Handler_EGS_CHANGE_READY_NOT(  hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_DUNGEON_DIFFICULTY_ACK:
		bRetVal = Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_DUNGEON_DIFFICULTY_NOT:
		bRetVal = Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_MOTION_ACK:
		bRetVal = Handler_EGS_CHANGE_MOTION_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_MOTION_NOT:
		bRetVal = Handler_EGS_CHANGE_MOTION_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_REPAIR_ITEM_NOT:
	case EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT:
		bRetVal = Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_BAN_USER_ACK:
		{
			if ( g_pX2Room != NULL )
				bRetVal = g_pX2Room->Handler_EGS_BAN_USER_ACK( hWnd, uMsg, wParam, lParam );

			bRetVal = true;
		}
		break;

	case EGS_BAN_USER_NOT:
		{
			if ( g_pX2Room != NULL )
				g_pX2Room->SetBan( true );
			return true;

		}
		break;

	case EGS_CHANGE_PITIN_ACK:
		{
			return Handler_EGS_CHANGE_PITIN_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_CHANGE_PITIN_NOT:
		{
			return Handler_EGS_CHANGE_PITIN_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
//#ifndef NOT_USE_DICE_ROLL
//	case EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_ACK:
//		{
//			return Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_ACK( hWnd, uMsg, wParam, lParam );
//		}
//		break;
//
//	case EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_NOT:
//		{
//			return Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_NOT( hWnd, uMsg, wParam, lParam );
//		}
//		break;
//#endif //NOT_USE_DICE_ROLL

	case EGS_CHANGE_TRADE_NOT:
		{
			return Handler_EGS_CHANGE_TRADE_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_RESTORE_SPIRIT_NOT:
		{
			return Handler_EGS_RESTORE_SPIRIT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

		//{{ 2008. 10. 29  최육사		
	case EGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT:
		{
			return Handler_EGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT( hWnd, uMsg, wParam, lParam );
		}
		//}}
	}
	return false;	
}

//bool CX2StateDungeonRoom::ClickMenuBackButton()
//{
//	return Handler_EGS_LEAVE_ROOM_REQ();
//}

void CX2StateDungeonRoom::SetShowStateDLG( bool bShow )
{
	/*
	if ( m_pCharacterRoom->GetOpen() == true )
	{
		m_pDLGDungeonRoomBack->SetShow( false );
		m_pDLGDungeonRoomFront->SetShow( false );


		m_pDLGDungeonRoomBack->SetEnable( false );
		m_pDLGDungeonRoomFront->SetEnable( false );


		m_pTalkBoxMgr->SetShow( false );
	}
	else
	{
		m_pDLGDungeonRoomBack->SetShow( true );
		m_pDLGDungeonRoomFront->SetShow( true );
		

		m_pDLGDungeonRoomBack->SetEnable( true );
		m_pDLGDungeonRoomFront->SetEnable( true );


		m_pTalkBoxMgr->SetShow( true );
	}
	*/

	m_pDLGDungeonRoomBack->SetShow( bShow );
	m_pDLGDungeonRoomFront->SetShow( bShow );


	m_pDLGDungeonRoomBack->SetEnable( bShow );
	m_pDLGDungeonRoomFront->SetEnable( bShow );


	m_pTalkBoxMgr->SetShow( bShow );
}


bool CX2StateDungeonRoom::ToggleLobbyMenuWindow()
{
	m_bShowWindowMenu = !m_bShowWindowMenu;

	if ( m_bShowWindowMenu == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
		m_pDLGMenuWindow = new CKTDGUIDialog( this, L"DLG_Menu_Window.lua", 0.07f );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMenuWindow );
	}
	else
	{
		SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
	}



	return true;
}


//bool CX2StateDungeonRoom::ToggleMyInfo(  CX2CharacterRoom::CHARACTER_ROOM_STATE charRoomState )
//{
//	if ( m_pCX2DungeonRoom->GetRecievePitInNot() == false )
//		return true;
//
//	CX2StateMenu::ToggleMyInfo( charRoomState  );
//
//	for ( int i = 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
//	{
//		CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );
//		if ( pSlotData == NULL || pSlotData->m_pUnitViewer == NULL)
//			continue;
//
//		if ( m_pCharacterRoom->GetOpen() == true )
//		{
//			pSlotData->m_pUnitViewer->SetShowObject( false );
//		}
//		else
//		{
//			pSlotData->m_pUnitViewer->SetShowObject( true );
//		}
//	}
//
//
//	if ( g_pData->GetDungeonRoom()->GetMySlot()->m_bReady == true )
//	{
//		Handler_EGS_CHANGE_READY_REQ();
//	}
//
//	for ( int i = 0; i < MAX_DUNGEON_ROOM_USER_NUM; i++ )
//	{
//		Dungeon_ROOM_USER_BOX* pDungeonRoomUserBox = m_DungeonRoomUserBoxList[i];
//		if ( pDungeonRoomUserBox->pDialog == NULL )
//			continue;
//
//		if ( m_pCharacterRoom->GetOpen() == true )
//		{
//			pDungeonRoomUserBox->SetShow( false );
//		}
//		else
//		{
//			if ( g_pData->GetDungeonRoom()->GetSlotData(i)->m_pUnit != NULL )
//			{
//				pDungeonRoomUserBox->SetShow( true );
//			}
//		}
//	}
//	
//	if ( g_pData->GetDungeonRoom()->GetMySlot()->m_bPitIn == false && m_pCharacterRoom->GetOpen() == true )
//    {
//        Handler_EGS_CHANGE_PITIN_REQ();
//    }	
//	else if ( g_pData->GetDungeonRoom()->GetMySlot()->m_bPitIn == true && m_pCharacterRoom->GetOpen() == false )
//    {
//#ifdef POSTBOX
//        if(g_pMain->GetPostBox()->GetSwitchState() == false)
//        	Handler_EGS_CHANGE_PITIN_REQ();
//#else
//        Handler_EGS_CHANGE_PITIN_REQ();
//#endif
//    }
//	
//
//	//이렇게 바꾸면 안되고 나중에 패킷 추가 해서 not로 바꿔줘야한다.
//	//장착중이면 레디 풀리게끔 하거나 준비된 상태가 아니어서 시작이 안되어야한다. 팀전에서..
//	//g_pData->GetDungeonRoom()->GetMySlot()->m_bPitIn = !g_pData->GetDungeonRoom()->GetMySlot()->m_bPitIn;
//
//	return true;
//}


bool CX2StateDungeonRoom::LastMsgByESC()
{
	OpenLastMsgPopUp( GET_STRING( STR_ID_629 ) );
	return true;
}

void CX2StateDungeonRoom::LastMsgPopOk()
{
	Handler_EGS_LEAVE_ROOM_REQ();
}

bool CX2StateDungeonRoom::Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	bool bRetVal = m_pCX2DungeonRoom->Handler_EGS_JOIN_ROOM_NOT( kEvent );
	if ( true == bRetVal )
	{
		CX2Room::SlotData* pSlotData = m_pCX2DungeonRoom->GetSlotData( kEvent.m_JoinSlot.m_Index );
		if( pSlotData == NULL )
			return false;


		pSlotData->MakeUnitViewer();
		pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING );

		return SetUserBox( pSlotData );
	}

	return false;
}

bool CX2StateDungeonRoom::Handler_EGS_LEAVE_ROOM_REQ()
{
	if ( m_bBaned == true )
	{
		Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();
        
		return true;
	}

	m_bEnableMotionChange = false;

	m_pCX2DungeonRoom->Handler_EGS_LEAVE_ROOM_REQ();

	SAFE_DELETE_DIALOG( m_pDLGMsgBox ); 
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_630 ), this );

	return true;
}

bool CX2StateDungeonRoom::Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	bool bRetVal = false;

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( NetError::ERR_ROOM_34 == kPacket.m_iOK )
	{
		//bRetVal = Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();
		bRetVal = Handler_EGS_STATE_CHANGE_FIELD_REQ();
	}
	else
	{
		if( g_pMain->DeleteServerPacket( EGS_LEAVE_ROOM_ACK ) == true )
		{
			if( g_pMain->IsValidPacket(kPacket.m_iOK) == true )
			{
				//bRetVal = Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();
				bRetVal = Handler_EGS_STATE_CHANGE_FIELD_REQ();
			}
		}
	}

	return bRetVal;
}

bool CX2StateDungeonRoom::Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );


	bool bRetVal = m_pCX2DungeonRoom->Handler_EGS_LEAVE_ROOM_NOT( kEvent );
	if( true == bRetVal )
	{


		CX2DungeonRoom::SlotData* pSlotData = m_pCX2DungeonRoom->GetMySlot();
		if( pSlotData->m_bHost == true )
		{
			m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Ready")->SetEnable(false);
			m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Ready")->SetShow(false);
			m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Start")->SetEnable(true);
			m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Start")->SetShow(true);
		}
		else
		{
			m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Ready")->SetEnable(true);
			m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Ready")->SetShow(true);
			m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Start")->SetEnable(false);
			m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Start")->SetShow(false);
		}

		for ( int i = 0; i < MAX_DUNGEON_ROOM_USER_NUM; i++ )
		{
			Dungeon_ROOM_USER_BOX* pDungeonRoomUserBox = m_DungeonRoomUserBoxList[ i ];

			if ( pDungeonRoomUserBox->unitUID == kEvent.m_iUnitUID )
			{
				pDungeonRoomUserBox->unitUID = 0;
				pDungeonRoomUserBox->SetShow( false );
				return true;
			}
		}
	}

	return false;
}


bool CX2StateDungeonRoom::Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ()
{
#if 0 
	KStateChangeReq kPacket;
	kPacket.m_nMapID = g_pMain->GetPrevLocalStateID();	
	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_LOCAL_MAP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_LOCAL_MAP_ACK, 20.f );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_565 ), this );
#endif
	Handler_EGS_STATE_CHANGE_FIELD_REQ();

	return true;
}

bool CX2StateDungeonRoom::Handler_EGS_STATE_CHANGE_LOCAL_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#if 0
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KStateChangeAck kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_LOCAL_MAP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_566 ) );
			g_pData->DeleteDungeonRoom(); 
			DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_LOCAL_MAP, kEvent.m_nMapID, false " );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_LOCAL_MAP, kEvent.m_nMapID, false );			
		}
	}
#endif
	return true;
}




bool CX2StateDungeonRoom::Handler_EGS_CHANGE_SLOT_OPEN_REQ( wstring buttonName )
{
	int slotIndex = 0;
	if( buttonName == L"Dungeon_Room_BlueO_1" || buttonName == L"Dungeon_Room_BlueX_1" )
	{
		slotIndex = 0;
	}
	else if( buttonName == L"Dungeon_Room_BlueO_2" || buttonName == L"Dungeon_Room_BlueX_2" )
	{
		slotIndex = 1;
	}
	else if( buttonName == L"Dungeon_Room_BlueO_3" || buttonName == L"Dungeon_Room_BlueX_3" )
	{
		slotIndex = 2;
	}
	else if( buttonName == L"Dungeon_Room_BlueO_4" || buttonName == L"Dungeon_Room_BlueX_4" )
	{
		slotIndex = 3;
	}
	else
		return false;

	return m_pCX2DungeonRoom->Handler_EGS_CHANGE_SLOT_OPEN_REQ( slotIndex );
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_SLOT_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_SLOT_OPEN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	//SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	bool bRetVal = false;

	bRetVal = m_pCX2DungeonRoom->Handler_EGS_CHANGE_SLOT_OPEN_ACK( kEvent );

	return bRetVal;
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_SLOT_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_SLOT_OPEN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	//SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	bool bRetVal = m_pCX2DungeonRoom->Handler_EGS_CHANGE_SLOT_OPEN_NOT( kEvent );
	SetUIOXButton();

	return bRetVal;
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_REQ( CX2Dungeon::DIFFICULTY_LEVEL difficulty )
{
#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_CHANGE_DUNGEON_DIFFICULTY_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST


	KEGS_CHANGE_DUNGEON_DIFFICULTY_REQ kPacket;
	kPacket = difficulty;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_DUNGEON_DIFFICULTY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_DUNGEON_DIFFICULTY_ACK );

	return true;
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_GAME_START_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_DUNGEON_DIFFICULTY_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}
	return false;
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_DUNGEON_DIFFICULTY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	m_pCX2DungeonRoom->SetDifficulty( (CX2Dungeon::DIFFICULTY_LEVEL)kEvent );



	SEnum::DUNGEON_ID dungeonID = (SEnum::DUNGEON_ID) ( m_pCX2DungeonRoom->GetDungeonID() + m_pCX2DungeonRoom->GetDifficulty() );
	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( dungeonID );
	if( NULL != pDungeonData &&
		pDungeonData->m_fTimeLimit > 0.f )
	{
		m_pCX2DungeonRoom->SetPlayTimeLimit( pDungeonData->m_fTimeLimit );
	}
	

	// note!!! special case
	if( m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_NORMAL )
	{
		if( m_pCX2DungeonRoom->GetDifficulty() == CX2Dungeon::DL_NORMAL )
		{
			// 초심자 숲 가기 퀘스트
			int MAGIC_QUEST_ID = 11;
			CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( MAGIC_QUEST_ID );
			if( NULL != pQuestInst && false == pQuestInst->IsComplete() )
			{
				CreateMouseClickUI( (CKTDXStage*) this, D3DXVECTOR2( 805, 215 ) );
			}
		}
		else
		{
			DeleteMouseClickUI();
		}
	}

	return true;
}

bool CX2StateDungeonRoom::Handler_EGS_STATE_CHANGE_GAME_START_REQ()
{
	CX2DungeonRoom::SlotData* pSlotData = m_pCX2DungeonRoom->GetMySlot();
	if( pSlotData == NULL || pSlotData->m_bHost == false || pSlotData->m_bTrade == true )
		return false;


	if ( g_pData->GetDungeonManager()->IsActiveDungeon( 
		m_pCX2DungeonRoom->GetDungeonID(), m_pCX2DungeonRoom->GetDifficulty() ) == false &&
		g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_631 ), this );

		return false;
	}

	if( m_fTimeLeftStartReq > 0.f )
	{
		return false;
	}

	KEGS_STATE_CHANGE_GAME_START_REQ kPacket;
	ASSERT( NULL != m_pCX2DungeonRoom );
	if( NULL != m_pCX2DungeonRoom )
	{
		kPacket.m_iDungeonID = (int)m_pCX2DungeonRoom->GetDungeonID() + (int) m_pCX2DungeonRoom->GetDifficulty();
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_GAME_START_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_GAME_START_ACK );
	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_632 ), this );

	m_fTimeLeftStartReq = 1.5f;

	return true;
}

bool CX2StateDungeonRoom::Handler_EGS_STATE_CHANGE_GAME_START_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_GAME_START_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2StateDungeonRoom::Handler_EGS_STATE_CHANGE_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_GAME_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );


	if( g_pData->GetGameUDP()->ConnectRelayTestResult() == true || m_pCX2DungeonRoom->GetUserNum() == 1 )
	{
		if( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true	|| m_pCX2DungeonRoom->GetMySlot()->m_bReady == true )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_632 ) );
		}

		m_pCX2DungeonRoom->SetRoomState( (CX2Room::ROOM_STATE)kEvent.m_cRoomState );
		m_pCX2DungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );
		//{{ 2007. 10. 5  최육사  감소된 근성도 받기
		g_pData->GetMyUser()->GetSelectUnit()->SetSpirit( kEvent.m_iSpirit );
		//}}

		if( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true	|| m_pCX2DungeonRoom->GetMySlot()->m_bReady == true )
		{
			DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_GAME, NULL, false" );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_GAME, NULL, false );
		}
	}
	else
	{
		Handler_EGS_LEAVE_ROOM_REQ();
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_633 ), this );
		ErrorLogMsg( XEM_ERROR92, L"Dungeon" );
	}

	return true;
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_READY_REQ()
{
	CX2PVPRoom::SlotData* pSlotData = g_pX2Room->GetMySlot();

	if ( pSlotData != NULL && pSlotData->GetIsReady() == false && 
		g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

		for ( int i = (int)(CX2Inventory::ST_EQUIP); i < (int)( CX2Inventory::ST_AVARTA ); i++ )
		{
			CX2Inventory::SORT_TYPE sortType = (CX2Inventory::SORT_TYPE)i;
			int usedSlotNum = kInventory.GetUsedSlotNum( sortType );
			if ( kInventory.GetItemMaxNum( sortType ) - usedSlotNum < 7 )
			{
				wstring invenSortTypeName = kInventory.GetInvenSortTypeName( sortType );
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_634, "L", invenSortTypeName ) ), this );
				break;
			}
		}
	}



	return m_pCX2DungeonRoom->Handler_EGS_CHANGE_READY_REQ();
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_READY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_READY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	bool bRetVal = m_pCX2DungeonRoom->Handler_EGS_CHANGE_READY_ACK( kEvent );

	return bRetVal;
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_READY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_READY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	bool bRetVal =  m_pCX2DungeonRoom->Handler_EGS_CHANGE_READY_NOT( kEvent );
	if( true == bRetVal )
	{
		CX2Room::SlotData* pSlotData = m_pCX2DungeonRoom->GetSlotDataByUnitUID( kEvent.m_UnitUID );

		if( pSlotData != NULL && pSlotData->m_pUnitViewer != NULL )
		{
			if( kEvent.m_bReady == true )
				pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );	
			else
            {
				pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
            }

			return true;
		}
	}

	return bRetVal;
}




bool CX2StateDungeonRoom::Handler_EGS_CHANGE_PITIN_REQ()
{
	return m_pCX2DungeonRoom->Handler_EGS_CHANGE_PITIN_REQ();
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_PITIN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PITIN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	return m_pCX2DungeonRoom->Handler_EGS_CHANGE_PITIN_ACK( kEvent );
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PITIN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	return m_pCX2DungeonRoom->Handler_EGS_CHANGE_PITIN_NOT( kEvent );

}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_MOTION_REQ( KEGS_CHANGE_MOTION_REQ& kEGS_CHANGE_MOTION_REQ )
{
	if ( m_bEnableMotionChange == false )
		return true;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_MOTION_REQ, kEGS_CHANGE_MOTION_REQ );
	//g_pMain->AddServerPacket( EGS_CHANGE_MOTION_ACK );

	return true;
}
bool CX2StateDungeonRoom::Handler_EGS_CHANGE_MOTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_MOTION_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	//if( g_pMain->DeleteServerPacket( EGS_CHANGE_MOTION_ACK ) == true )
	//{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
		}
	//}

	return true;
}
bool CX2StateDungeonRoom::Handler_EGS_CHANGE_MOTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_MOTION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pX2Room != NULL )
	{
		for ( int i = 0; i < (int)g_pX2Room->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( i );
			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL || pSlotData->m_pUnitViewer == NULL )
				continue;




			if ( pSlotData->m_pUnit->GetUID() == kEvent.m_UnitUID )
			{
				if ( pSlotData->m_bHost == true || pSlotData->GetSlotState() == CX2Room::SS_WAIT && pSlotData->m_bReady == false )
				{
					//wstring motionName = g_pX2Room->GetMotionName( (CX2Room::ROOM_MOTION)kEvent.m_cMotionID );
					//pSlotData->m_pUnitViewer->PlayAnim(  motionName.c_str(), CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					g_pX2Room->PlayMotionByID( (CX2Room::ROOM_MOTION)kEvent.m_cMotionID, pSlotData->m_pUnitViewer );
					break;

				}
			}
		}
	}

	

	return true;
}

bool CX2StateDungeonRoom::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	return m_pCX2DungeonRoom->Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( kEvent );
}

//#ifndef NOT_USE_DICE_ROLL
//bool CX2StateDungeonRoom::Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_REQ( CX2DungeonRoom::DUNGEON_GET_ITEM_TYPE getItemType )
//{
//	KEGS_CHANGE_DUNGEON_GET_ITEM_TYPE_REQ kPacket;
//	kPacket.m_cGetItemType = (char)getItemType;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_ACK );
//
//	return true;
//}
//
//bool CX2StateDungeonRoom::Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_CHANGE_DUNGEON_GET_ITEM_TYPE_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if( g_pMain->DeleteServerPacket( EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_ACK ) == true )
//	{
//		return g_pMain->IsValidPacket( kEvent.m_iOK );
//	}
//
//	return true;
//}
//
//bool CX2StateDungeonRoom::Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_CHANGE_DUNGEON_GET_ITEM_TYPE_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	CKTDGUIRadioButton* pRadioButton = NULL;
//
//	if ( (CX2DungeonRoom::DUNGEON_GET_ITEM_TYPE)kEvent.m_cGetItemType == CX2DungeonRoom::DGIT_RANDOM )
//	{
//		pRadioButton = (CKTDGUIRadioButton*)m_pDLGDungeonRoomFront->GetControl( L"RadioButtonRandom" );
//	}
//	else if ( (CX2DungeonRoom::DUNGEON_GET_ITEM_TYPE)kEvent.m_cGetItemType == CX2DungeonRoom::DGIT_PERSON )
//	{
//		pRadioButton = (CKTDGUIRadioButton*)m_pDLGDungeonRoomFront->GetControl( L"RadioButtonPersonal" );
//	}
//
//	if ( pRadioButton != NULL )
//	{
//		pRadioButton->SetChecked( true );
//	}
//
//	if( NULL != m_pCX2DungeonRoom )
//	{
//		m_pCX2DungeonRoom->SetGetItemType( (CX2DungeonRoom::DUNGEON_GET_ITEM_TYPE)kEvent.m_cGetItemType );
//	}
//	
//	return true;
//}
//#endif //NOT_USE_DICE_ROLL


bool CX2StateDungeonRoom::Handler_EGS_CHANGE_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_TRADE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pX2Room )
	{
		CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
		if( NULL != pSlotData )
		{
			pSlotData->m_bTrade = kEvent.m_bTrade;
		}
	}

	return true;
}

bool CX2StateDungeonRoom::Handler_EGS_RESTORE_SPIRIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESTORE_SPIRIT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( m_pCX2DungeonRoom != NULL )
	{
		CX2Room::SlotData* pSlotData = m_pCX2DungeonRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
		if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			pSlotData->m_pUnit->AccessUnitData().m_iSpirit = kEvent.m_iSpirit;
		}
	}

	return true;
}

//{{ 2008. 10. 29  최육사
bool CX2StateDungeonRoom::Handler_EGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( m_pCX2DungeonRoom != NULL )
	{
		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}

	return true;
}
//}}

void CX2StateDungeonRoom::ChangeMotion()
{
	if ( m_bBaned == true )
	{
		return;
	}

	KEGS_CHANGE_MOTION_REQ kPacket;
	int motionID = RandomNumber( 0, CX2Room::RM_END - 1 );
	kPacket.m_cMotionID = motionID;

	Handler_EGS_CHANGE_MOTION_REQ( kPacket );
}

void CX2StateDungeonRoom::CheckChangeDifficulty( bool bLower )
{
	m_bChangeDifficultyLower = bLower;

	if(m_pCX2DungeonRoom->GetCharLvLimit() == true)
	{
		// 던전 레벨제한이 걸려있는 경우
		CX2Dungeon::DIFFICULTY_LEVEL difficulty = m_pCX2DungeonRoom->GetDifficulty();

		if( true == bLower )
		{		
			switch( difficulty )
			{
			case CX2Dungeon::DL_NORMAL:
				return;
				break;
			case CX2Dungeon::DL_HARD:
				difficulty = CX2Dungeon::DL_NORMAL;
				break;
			case CX2Dungeon::DL_EXPERT:
				difficulty = CX2Dungeon::DL_HARD;
				break;
			}
		}
		else
		{
			switch( difficulty )
			{
			case CX2Dungeon::DL_NORMAL:
				difficulty = CX2Dungeon::DL_HARD;
				break;
			case CX2Dungeon::DL_HARD:
				difficulty = CX2Dungeon::DL_EXPERT;
				break;
			case CX2Dungeon::DL_EXPERT:
				return;
				break;
			}
		}	

		if ( g_pData->GetDungeonManager()->IsActiveDungeon( m_pCX2DungeonRoom->GetDungeonID(), difficulty ) == false &&
			g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
			return;

		m_bChangeDifficultyLower = bLower;

		// 던전 레벨제한		
		ASSERT( !L"Wrong Path" );
		int iRequireLv = 0;//(int)(m_pCX2DungeonRoom->GetRecommendLevel(difficulty).x);
		int iMyCharLv = g_pData->GetSelectUnitLevel();
		if(iMyCharLv < iRequireLv)
		{
			// 던전방 레벨제한이 풀리는 경우
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_635 ), DRUCM_CHANGE_DIFFICULTY_OK, this );
			return;
		}	

		// 레벨제한에 걸려 강퇴될 유저가 있는지 검사
		for(int i=0; i<m_pCX2DungeonRoom->GetSlotNum(); ++i)
		{
			CX2DungeonRoom::SlotData *pSlotData = m_pCX2DungeonRoom->GetSlotData(i);
			if( pSlotData != NULL && pSlotData->GetIsHost() == false && pSlotData->m_pUnit != NULL )
			{
				int userLv = pSlotData->m_pUnit->GetUnitData().m_Level;
				if(userLv < iRequireLv)
				{
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_636 ), DRUCM_BAN_CHANGE_DIFFICULTY_OK, this );
					return;
				}
			}
		}
	}	

	// 위조건에 걸리지 않으면 정상적으로 난이도 조절한다.
	ChangeDifficulty(m_bChangeDifficultyLower);
	return;
}

bool CX2StateDungeonRoom::ChangeDifficulty( bool bLower )
{
	CX2Dungeon::DIFFICULTY_LEVEL difficulty = m_pCX2DungeonRoom->GetDifficulty();

	if( true == bLower )
	{		
		switch( difficulty )
		{
		case CX2Dungeon::DL_NORMAL:
			return false;
			break;
		case CX2Dungeon::DL_HARD:
			difficulty = CX2Dungeon::DL_NORMAL;
			break;
		case CX2Dungeon::DL_EXPERT:
			difficulty = CX2Dungeon::DL_HARD;
			break;
		}
	}
	else
	{
		switch( difficulty )
		{
		case CX2Dungeon::DL_NORMAL:
			difficulty = CX2Dungeon::DL_HARD;
			break;
		case CX2Dungeon::DL_HARD:
			difficulty = CX2Dungeon::DL_EXPERT;
			break;
		case CX2Dungeon::DL_EXPERT:
			return false;
			break;
		}
	}	

	if ( g_pData->GetDungeonManager()->IsActiveDungeon( m_pCX2DungeonRoom->GetDungeonID(), difficulty ) == false &&
		g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
		return false;

	bool bRetVal = Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_REQ( difficulty );

	return bRetVal;
}

bool CX2StateDungeonRoom::ChangeEventSkip( bool bEventSkip )
{
	m_bTempEventSkip = bEventSkip;
	return true;
}

void CX2StateDungeonRoom::UnitViewerProcess( CX2UnitViewerUI* pViewer, int slotNum )
{
	if( NULL == pViewer )
		return;

	//pViewer->SetLightPos( 1000, 1000, -200 );
	//pViewer->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pViewer->SetLightPos( -250, 100, -600 );	// 캐릭터뷰어 라이트 위치 변경

	//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
	switch(slotNum)
	{
	case 0:
		{
			D3DXVECTOR3 vPos = m_pDLGDungeonRoomFront->GetDummyPos(slotNum);
			pViewer->GetMatrix().Move( vPos );
			pViewer->GetMatrix().Scale( 1.6f, 1.6f, 1.6f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 1:
		{
			D3DXVECTOR3 vPos = m_pDLGDungeonRoomFront->GetDummyPos(slotNum);
			pViewer->GetMatrix().Move( vPos );
			pViewer->GetMatrix().Scale( 1.6f,1.6f,1.6f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 2:
		{
			D3DXVECTOR3 vPos = m_pDLGDungeonRoomFront->GetDummyPos(slotNum);
			pViewer->GetMatrix().Move( vPos );
			pViewer->GetMatrix().Scale( 1.6f,1.6f,1.6f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
		} break;
	case 3:
		{
			D3DXVECTOR3 vPos = m_pDLGDungeonRoomFront->GetDummyPos(slotNum);
			pViewer->GetMatrix().Move( vPos );
			pViewer->GetMatrix().Scale( 1.6f, 1.6f, 1.6f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
		} break;
	default:
		break;

	}//}}
	
	pViewer->OnFrameMove( m_fTime, m_fElapsedTime );
}

void CX2StateDungeonRoom::UISetting()
{
	if( NULL == m_pCX2DungeonRoom )
		return;

	if( (g_pMain->GetIsPlayingTutorial() == false && g_pMain->GetPlayFieldDungeon() == false) &&
		(true == g_pMain->GetIsExitingTutorial() || g_pMain->GetExitFieldDungeon() == true) )
	{
		m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua" );
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );

		CKTDGUIStatic* pStatic = 
			static_cast<CKTDGUIStatic*>( m_pDLGLoadingState->GetControl( L"Static_Loading_Dungeon_BG" ) );

		// 그림 두장
		if( pStatic != NULL )
		{
			pStatic->SetShow( true );

			if( pStatic->GetPicture(0) != NULL )
				pStatic->GetPicture(0)->SetShow(false);

			if ( pStatic->GetPicture(1) != NULL )
				pStatic->GetPicture(1)->SetTex( L"HQ_Loading_RUBEN_PROLOG_01.tga" );

			if (pStatic->GetPicture(2) != NULL )
				pStatic->GetPicture(2)->SetTex( L"HQ_Loading_RUBEN_PROLOG_02.tga" );
		}

		
		g_pMain->SetPlayFieldDungeon(false);
		g_pMain->SetIsPlayingTutorial(false);		

		Handler_EGS_LEAVE_ROOM_REQ();


		return;
	}

	if( true == g_pMain->GetIsPlayingTutorial() || g_pMain->GetPlayFieldDungeon() == true)
	{
		m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua" );
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );

		CKTDGUIStatic* pStatic = 
			static_cast<CKTDGUIStatic*>( m_pDLGLoadingState->GetControl( L"Static_Loading_Dungeon_BG" ) );

		// 그림 두장
		if( pStatic != NULL )
		{
			pStatic->SetShow( true );

			if( pStatic->GetPicture(0) != NULL )
				pStatic->GetPicture(0)->SetShow(false);
			
			if ( pStatic->GetPicture(1) != NULL )
				pStatic->GetPicture(1)->SetTex( L"HQ_Loading_RUBEN_PROLOG_01.tga" );

			if (pStatic->GetPicture(2) != NULL )
				pStatic->GetPicture(2)->SetTex( L"HQ_Loading_RUBEN_PROLOG_02.tga" );
		}		

		Handler_EGS_STATE_CHANGE_GAME_START_REQ();		

		return;
	}

	m_pDLGDungeonRoomBack = g_pMain->GetMemoryHolder()->GetDungeonRoomBackDLG( this );
	//g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDungeonRoomBack );

	for ( int i = 0; i < MAX_DUNGEON_ROOM_USER_NUM; i++ )
	{
		Dungeon_ROOM_USER_BOX* pDungeonRoomUserBox = new Dungeon_ROOM_USER_BOX();
		m_DungeonRoomUserBoxList[ i ] = pDungeonRoomUserBox;
		pDungeonRoomUserBox->unitUID = 0;
		pDungeonRoomUserBox->pDialog = new CKTDGUIDialog( this, L"DLG_Dungeon_Room_UserInfo.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDungeonRoomUserBox->pDialog );

		D3DXVECTOR3 offsetPos = m_pDLGDungeonRoomBack->GetDummyPos( i );
		D3DXVECTOR2 pos = D3DXVECTOR2( offsetPos.x,  offsetPos.y );

		pDungeonRoomUserBox->pDialog->SetPos( pos );
		

		pDungeonRoomUserBox->pDialogUserBack = new CKTDGUIDialog( this, L"DLG_PVP_Result_Card_Back.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDungeonRoomUserBox->pDialogUserBack );
		offsetPos = m_pDLGDungeonRoomBack->GetDummyPos( i + MAX_DUNGEON_ROOM_USER_NUM );
		pos = D3DXVECTOR2( offsetPos.x,  offsetPos.y );
		pDungeonRoomUserBox->pDialogUserBack->SetPos( pos );
	
		pDungeonRoomUserBox->SetShow( false );
	}



	m_pDLGDungeonRoomFront = g_pMain->GetMemoryHolder()->GetDungeonRoomFrontDLG( this );
	if(m_pDLGDungeonRoomFront != NULL)
	{
		m_pDLGDungeonRoomFront->GetControl( L"RadioButtonRandom" )->SetShowEnable(true, true);
		m_pDLGDungeonRoomFront->GetControl( L"RadioButtonPersonal" )->SetShowEnable(true, true);
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->SetShowEnable(true, true);
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->SetShowEnable(true, true);
	}
	//g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDungeonRoomFront );




	m_pDLGTalkBox = new CKTDGUIDialog( this, L"DLG_Room_Talk_Box.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTalkBox );

	m_pTalkBoxMgr = new CX2TalkBoxManagerUI();
	m_pDLGTalkBox->AddControl( m_pTalkBoxMgr );


/*
	CKTDGUIStatic* pStaticLock = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"Dungeon_Room_Lock" );
	if ( m_pCX2DungeonRoom->GetRoomPublic() == true )
		pStaticLock->GetPicture(0)->SetShow( false ); // 자물쇠그림
	else
		pStaticLock->GetPicture(0)->SetShow( true );
*/
	
	CKTDGUIStatic* pStatic_Dungeon_EX = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl( L"Static_Dungeon_EX" );
	wstring wstrDungeonDesc = m_pCX2DungeonRoom->GetDungeonEX();
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Dungeon_EX->GetString(0)->fontIndex );
	if( NULL != pFont )
	{
		LineBreak( wstrDungeonDesc, 290, L"", true, pFont );
	}
	pStatic_Dungeon_EX->GetString(0)->msg = wstrDungeonDesc;
	

	CX2Dungeon::DIFFICULTY_LEVEL difficulty = m_pCX2DungeonRoom->GetDifficulty();
	CKTDGUIStatic* pStaticDiff = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl(L"Difficulty");
	CKTDGUIStatic* pStaticDiffStar = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl(L"Difficulty_Star");
	CKTDGUIStatic* pStaticDifficulty = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl( L"DungeonRoom_DungeonDifficulty" );
	switch( difficulty )
	{
	case CX2Dungeon::DL_NORMAL:
		pStaticDifficulty->GetString(0)->msg = GET_STRING( STR_ID_385 );
		pStaticDiff->GetString( 0 )->msg = L"";

		pStaticDiffStar->GetString( 0 )->msg = L"";
		break;

	case CX2Dungeon::DL_HARD:
		pStaticDifficulty->GetString(0)->msg = GET_STRING( STR_ID_386 );

		if ( m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_NORMAL ||
			m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_HARD || 
			m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_EXPERT )
		{
			pStaticDiff->GetString( 0 )->msg = L"";
			pStaticDiffStar->GetString( 0 )->msg = L"";
		}
		else
		{
			pStaticDiff->GetString( 0 )->msg = GET_STRING( STR_ID_620 );
            pStaticDiff->GetString( 0 )->msg += L" : ";
			pStaticDiffStar->GetString( 0 )->msg = GET_STRING( STR_ID_621 );
		}
		break;
		
	case CX2Dungeon::DL_EXPERT:
		pStaticDifficulty->GetString(0)->msg = GET_STRING( STR_ID_388 );

		if ( m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_NORMAL ||
			m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_HARD || 
			m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_EXPERT )
		{
			pStaticDiff->GetString( 0 )->msg = L"";
			pStaticDiffStar->GetString( 0 )->msg = L"";
		}
		else
		{
			pStaticDiff->GetString( 0 )->msg = GET_STRING( STR_ID_620 );
            pStaticDiff->GetString( 0 )->msg += L" : ";
			pStaticDiffStar->GetString( 0 )->msg = GET_STRING( STR_ID_622 );
		}
	
		break;
	}
	
	ASSERT( !L"Wrong Path" );
	D3DXCOLOR diffColor = g_pData->GetDungeonManager()->GetDifficultyColor( g_pData->GetDungeonRoom()->GetDungeonID(),
																			g_pData->GetDungeonRoom()->GetDifficulty(),
																			g_pData->GetSelectUnitLevel() );

	D3DXCOLOR limitLevelColor = g_pData->GetDungeonManager()->GetLimitLevelColor( 
		g_pData->GetDungeonRoom()->GetDungeonID(), 
		g_pData->GetDungeonRoom()->GetDifficulty(),
		g_pData->GetSelectUnitLevel() );

	CKTDGUIStatic* pStaticRecommendLevel = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl(L"Static_Dungeon_RecommendLevel");
// 	pStaticRecommendLevel->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_384, "ii",
//         ( int )m_pCX2DungeonRoom->GetRecommendLevel( ( int )difficulty ).x,
//         ( int )m_pCX2DungeonRoom->GetRecommendLevel( ( int )difficulty ).y ) );
	pStaticRecommendLevel->GetString( 0 )->color = diffColor; 

	int requireLevel = 0;
	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)( g_pData->GetDungeonRoom()->GetDungeonID() + g_pData->GetDungeonRoom()->GetDifficulty() ) );
	if ( pDungeonData != NULL )
	{
		requireLevel = pDungeonData->m_MinLevel;
	}
	
	//wsprintf( buff, L"제한레벨 : %d", requireLevel );
	//CKTDGUIStatic* pStaticRequireLevel = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl(L"requireLevel");
	//pStaticRequireLevel->GetString( 0 )->msg = buff;
	//pStaticRequireLevel->GetString( 0 )->color = limitLevelColor; 

	pStaticDiff->GetString( 0 )->color = diffColor; 

	pStaticDiffStar->GetString( 0 )->color = diffColor;

	if( NULL != m_pDLGDungeonRoomBack )
	{
		CKTDGUIStatic* pStatic_PlaySoloMsg = (CKTDGUIStatic*) m_pDLGDungeonRoomBack->GetControl( L"g_pStaticPlaySolo" );
		if( NULL != pStatic_PlaySoloMsg )
		{
			if( SEnum::DI_EL_FOREST_GATE_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() )
			{
				pStatic_PlaySoloMsg->SetShow( true ); // 초심자 숲은 혼자서만
			}
			else
			{

				pStatic_PlaySoloMsg->SetShow( false );
			}
		}
	}


	// 방번호, 방이름
	////임시로 아이디에 있는 스트링 정보 값을 쓰고있다..
	//CKTDGUIStatic* pStaticRoomNumber = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"StaticRoomNumber" );
	//WCHAR tempRoomNum[10] = {0};
	//wsprintf(tempRoomNum, L"%d", m_pCX2DungeonRoom->GetRoomUID() );
	//pStaticRoomNumber->GetString(0)->msg = tempRoomNum;



	//켜자켜자 거시기 아케이드쪽에서 껐을꺼다 아마도 ㅡ,ㅡㅋ
	
	CKTDGUIControl* pControls = m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Title");
	if ( pControls != NULL )
		pControls->SetShow( true );

	pControls = m_pDLGDungeonRoomFront->GetControl(L"DungeonRoom_RoomName");
	if ( pControls != NULL )
		pControls->SetShow( true );

	CKTDGUIStatic* pStaticRoomName = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"DungeonRoom_RoomName" );
	pStaticRoomName->GetString(0)->msg = m_pCX2DungeonRoom->GetRoomName();

	
	for( int i=0; i < m_pCX2DungeonRoom->GetSlotNum(); i++ )
	{
		CX2DungeonRoom::SlotData* pSlotData = m_pCX2DungeonRoom->GetSlotData( i );

		if ( pSlotData == NULL )
			continue;

		CKTDGUIButton* pTempOButton = NULL;
		CKTDGUIButton* pTempXButton = NULL;

		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(i)
		{
		case 0:
			{
				pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueO_1");
				pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueX_1");
			} break;
		case 1:
			{
				pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueO_2");
				pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueX_2");
			} break;
		case 2:
			{
				pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueO_3");
				pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueX_3");
			} break;
		case 3:
			{
				pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueO_4");
				pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueX_4");
			} break;
		default:
			{
				ASSERT( !"Unexpected Room Slot Num" );
				continue; // error
			} break;

		}//}}
		
		switch( pSlotData->m_SlotState )
		{
		case CX2Room::SS_EMPTY:
			{
				pTempOButton->SetShow( true );
				pTempOButton->SetEnable( true );
				pTempXButton->SetShow( false );
				pTempXButton->SetEnable( false );
			}
			break;

		case CX2Room::SS_CLOSE:
			{
				pTempOButton->SetShow( false );
				pTempOButton->SetEnable( false );
				pTempXButton->SetShow( true );
				pTempXButton->SetEnable( true );
			}
			break;

		case CX2Room::SS_WAIT:
			{
				if( pSlotData->m_pUnitViewer == NULL )
					pSlotData->MakeUnitViewer();				
				pSlotData->m_pUnitViewer->SetShowObject( true );

				if( pSlotData->m_bMySlot == true )
				{
					if( pSlotData->m_bReady == false )
						pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING );
					else
						pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );	
				}
				else
				{
					if( pSlotData->m_bReady == false )
                    {
						pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
                    }
					else
						pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );	
				}

				pTempOButton->SetShow( false );
				pTempOButton->SetEnable( false );
				pTempXButton->SetShow( false );
				pTempXButton->SetEnable( false );
			}
			break;

		case CX2Room::SS_LOADING:
			{
				if( pSlotData->m_pUnitViewer == NULL )
					pSlotData->MakeUnitViewer();
				pSlotData->m_pUnitViewer->SetShowObject( true );

				pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );	

				pTempOButton->SetShow( false );
				pTempOButton->SetEnable( false );
				pTempXButton->SetShow( false );
				pTempXButton->SetEnable( false );
			}
			break;

		case CX2Room::SS_PLAY:
			{
				if( pSlotData->m_pUnitViewer == NULL )
					pSlotData->MakeUnitViewer();
				pSlotData->m_pUnitViewer->SetShowObject( true );

				pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );	

				pTempOButton->SetShow( false );
				pTempOButton->SetEnable( false );
				pTempXButton->SetShow( false );
				pTempXButton->SetEnable( false );
			}
			break;
		}

		SetUserBox( pSlotData );
	}

#ifdef CONVERSION_VS
	int i = 0; // init by picture of Ready, Host, Equip
#endif CONVERSION_VS

	// 초기화
	CKTDGUIStatic* pStaticReady = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl(L"Static_Dungeon_Room_Ready");
	pStaticReady->SetEnable(false);
	for ( i = 0; i < 4; i++ )
	{
		CKTDGUIControl::CPictureData* pTempPicture = pStaticReady->GetPicture(i);
		pTempPicture->SetShow(false);
	}

	CKTDGUIStatic* pStaticHost = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Host");
	pStaticHost->SetEnable(false);
	for ( i = 0; i < 4; i++ )
	{
		CKTDGUIControl::CPictureData* pTempPicture = pStaticHost->GetPicture(i);
		pTempPicture->SetShow(false);
	}

	CKTDGUIStatic* pStaticEquip = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Equip");
	pStaticEquip->SetEnable(false);
	for ( i = 0; i < 4; i++ )
	{
		CKTDGUIControl::CPictureData* pTempPicture = pStaticEquip->GetPicture(i);
		pTempPicture->SetShow(false);
	}


	CX2DungeonRoom::SlotData* pMySlotData = m_pCX2DungeonRoom->GetMySlot();
	ASSERT( NULL != pMySlotData );
	



	CKTDGUIStatic* pStaticDungeon_Room_Map = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl( L"StaticDungeon_Room_Map" );
	CKTDGUIControl::UITextureData* pTexture = new CKTDGUIControl::UITextureData();

// 	pTexture->pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( m_pCX2DungeonRoom->GetMapTextureName() );
// 	if( pTexture->pTexture == NULL )
// 	{
// 		SAFE_DELETE( pTexture );
// 		return;
// 	}

// 	wstring pieceName = m_pCX2DungeonRoom->GetMapTexturePieceName();
// 	MakeUpperCase(pieceName);
// 	CKTDXDeviceTexture::TEXTURE_UV* pTexUV = pTexture->pTexture->GetTexUV( pieceName );
// 	if( pTexUV != NULL )
// 	{
// 		pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= pTexUV->leftTop;
// 		pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;
// 		pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= pTexUV->leftBottom;
// 		pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= pTexUV->rightBottom;
// 		pTexture->SetTextureUV();
// 	}
// 	pStaticDungeon_Room_Map->GetPicture( 0 )->SetTex( pTexture );



	if( NULL != pMySlotData &&
		pMySlotData->m_bHost == false )
	{
		CKTDGUIButton* pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueO_1");
		CKTDGUIButton* pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueX_1");
		pTempOButton->SetEnable( false );
		pTempXButton->SetEnable( false );

		pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueO_2");
		pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueX_2");
		pTempOButton->SetEnable( false );
		pTempXButton->SetEnable( false );

		pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueO_3");
		pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueX_3");
		pTempOButton->SetEnable( false );
		pTempXButton->SetEnable( false );

		pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueO_4");
		pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl(L"Dungeon_Room_BlueX_4");
		pTempOButton->SetEnable( false );
		pTempXButton->SetEnable( false );
	}


//#ifndef NOT_USE_DICE_ROLL
//	if ( m_pCX2DungeonRoom->GetDungeonGetItemType() == CX2DungeonRoom::DGIT_RANDOM )
//	{
//		CKTDGUIRadioButton* pRadioButtonRandom = (CKTDGUIRadioButton*)m_pDLGDungeonRoomFront->GetControl( L"RadioButtonRandom" );
//		if ( pRadioButtonRandom != NULL )
//		{
//			pRadioButtonRandom->SetChecked( true );
//		}
//	}
//	else if ( m_pCX2DungeonRoom->GetDungeonGetItemType() == CX2DungeonRoom::DGIT_PERSON )
//	{
//		CKTDGUIRadioButton* pRadioButtonPersonal = (CKTDGUIRadioButton*)m_pDLGDungeonRoomFront->GetControl( L"RadioButtonPersonal" );
//		if ( pRadioButtonPersonal != NULL )
//		{
//			pRadioButtonPersonal->SetChecked( true );
//		}
//	}
//#endif //NOT_USE_DICE_ROLL


	CKTDGUIControl* pControlLock = m_pDLGDungeonRoomFront->GetControl( L"Dungeon_Room_State_Lock" );
	if ( pControlLock != NULL )
	{
		if ( m_pCX2DungeonRoom->GetRoomPublic() == true )
		{	
			pControlLock->SetShow( false );
		}
		else
		{
			pControlLock->SetShow( true );
		}
	}


	// note!!! special case
	if( m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_NORMAL )
	{
		// 초심자 숲 가기 퀘스트
		int MAGIC_QUEST_ID = 11; 
		CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( MAGIC_QUEST_ID );
		if( NULL != pQuestInst && false == pQuestInst->IsComplete() )
		{
			CreateMouseClickUI( (CKTDXStage*) this, D3DXVECTOR2( 805, 215 ) );
		}
	}
}

void CX2StateDungeonRoom::SetUIOXButton()
{
	//for( int i = 0; i < (int)m_pCX2DungeonRoom->GetSlotNum(); i++ )
	//{
	//	WCHAR keyO[100] = {0,};
	//	WCHAR keyX[100] = {0,};
	//	CKTDGUIButton* pTempOButton = NULL;
	//	CKTDGUIButton* pTempXButton = NULL;

	//	wsprintf( keyO, L"Dungeon_Room_YellowO_%d", (i+1) );
	//	wsprintf( keyX, L"Dungeon_Room_YellowX_%d", (i+1) );

	//	pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomFront->GetControl( keyO );
	//	pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomFront->GetControl( keyX );

	//	if( pTempOButton == NULL || pTempXButton == NULL )
	//		return; // error

	//	if( false == m_pCX2DungeonRoom->GetMySlot()->GetIsHost() )
	//	{
	//		pTempOButton->SetEnable( false );
	//		pTempXButton->SetEnable( false );
	//	}
	//	else
	//	{
	//		pTempOButton->SetEnable( true );
	//		pTempXButton->SetEnable( true );
	//	}
	//}


	for( int i = 0; i < (int)m_pCX2DungeonRoom->GetSlotNum(); i++ )
	{
		WCHAR keyO[100] = {0,};
		WCHAR keyX[100] = {0,};
		CKTDGUIButton* pTempOButton = NULL;
		CKTDGUIButton* pTempXButton = NULL;

		StringCchPrintf( keyO, 100, L"Dungeon_Room_BlueO_%d", (i+1) );
		StringCchPrintf( keyX, 100,  L"Dungeon_Room_BlueX_%d", (i+1) );

		/*wsprintf( keyO, L"Dungeon_Room_BlueO_%d", (i+1) );
		wsprintf( keyX, L"Dungeon_Room_BlueX_%d", (i+1) );*/

		pTempOButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl( keyO );
		pTempXButton = (CKTDGUIButton*)m_pDLGDungeonRoomBack->GetControl( keyX );

		if( pTempOButton == NULL || pTempXButton == NULL )
			return; // error


		CX2Room::SlotData* pSlotData = m_pCX2DungeonRoom->GetSlotData( i );
		if( pSlotData != NULL )
		{
			if( pSlotData->m_SlotState == CX2Room::SS_CLOSE )
			{
				pTempOButton->SetShow( false );
				pTempOButton->SetEnable( false );
				pTempXButton->SetShow( true );
				pTempXButton->SetEnable( true );
			}
			else if( pSlotData->m_SlotState == CX2Room::SS_EMPTY )
			{
				pTempOButton->SetShow( true );
				pTempOButton->SetEnable( true );
				pTempXButton->SetShow( false );
				pTempXButton->SetEnable( false );
			}
			else
			{
				pTempOButton->SetShow( false );
				pTempOButton->SetEnable( false );
				pTempXButton->SetShow( false );
				pTempXButton->SetEnable( false );
			}
		}

		if( NULL != m_pCX2DungeonRoom->GetMySlot() &&
			false == m_pCX2DungeonRoom->GetMySlot()->GetIsHost() )
		{
			pTempOButton->SetEnable( false );
			pTempXButton->SetEnable( false );
		}
		//else
		//{
		//	pTempOButton->SetShow( false );
		//	pTempOButton->SetEnable( false );
		//	pTempXButton->SetShow( false );
		//	pTempXButton->SetEnable( false );
		//}
	}
}

void CX2StateDungeonRoom::UIFrameMove()
{
	SEnum::DUNGEON_ID dungeonID = g_pData->GetDungeonRoom()->GetDungeonID();
#ifndef NEW_VILLAGE_UI
	if ( dungeonID == SEnum::DI_EL_FOREST_GATE_NORMAL ||
		dungeonID == SEnum::DI_EL_FOREST_GATE_HARD || 
		dungeonID == SEnum::DI_EL_FOREST_GATE_EXPERT )
	{
		if ( m_pDLGMenu != NULL && m_pDLGMenu->GetControl(L"ButtonBack")->GetIsMoving() == false )
		{
			m_pDLGMenu->GetControl(L"ButtonBack")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,0,0,1), 0.5f );
			m_pDLGMenu->GetControl(L"ButtonBack")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.2f );
		}
	}
#endif

	////방 번호, 이름 찍기
	//CKTDGUIControl::UIStringData* pStringData = NULL;
	//CKTDGUIControl::UIStringData* pStringUnitDot = NULL;
	//CKTDGUIControl::UIStringData* pStringUnitLVNum = NULL;
	//CKTDGUIControl::UIStringData* pStringUnitLV = NULL;
	//CKTDGUIControl::UIStringData* pStringUnitTitle = NULL;



	//CKTDGUIStatic* pStaticUnitID = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"StaticDungeonRoomUnitID" );
	//CKTDGUIStatic* pStaticUnitDot = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"StaticUnitDot" );
	//CKTDGUIStatic* pStaticUnitLVNum = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"StaticUnitLVNum" );
	//CKTDGUIStatic* pStaticUnitLV = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"StaticUnitLV" );
	//CKTDGUIStatic* pStaticUnitTitle = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"StaticUnitTitle" );

	
	CKTDGUIStatic* pStaticReady		= (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl(L"Static_Dungeon_Room_Ready");
	CKTDGUIStatic* pStaticHost		= (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Host");
	CKTDGUIStatic* pStaticEquip		= (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Equip");
	CKTDGUIStatic* pStaticPCRoom	= (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl(L"Static_Dungeon_Room_PCRoom");
	CKTDGUIStatic* pStaticTrade		= (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl(L"StaticTrade");


	CKTDGUIStatic* pStaticEmblem200			= (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"g_pStaticDungeon_Room_200Emblem" );
	CKTDGUIStatic* pStaticEmblemGoldMedal	= (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"g_pStaticDungeon_Room_Gold_Emblem" );



	
	for( int i = 0; i < m_pCX2DungeonRoom->GetSlotNum(); i++ )
	{
		
		CKTDGUIControl::CPictureData* pPictureReady = pStaticReady->GetPicture(i);
		pPictureReady->SetShow(false);
		
		CKTDGUIControl::CPictureData* pPictureHost = pStaticHost->GetPicture(i);
		pPictureHost->SetShow(false);

		CKTDGUIControl::CPictureData* pPictureEquip = pStaticEquip->GetPicture(i);
		pPictureEquip->SetShow(false);

		CKTDGUIControl::CPictureData* pPicturePCRoom = pStaticPCRoom->GetPicture(i);
		pPicturePCRoom->SetShow(false);

		CKTDGUIControl::CPictureData* pPictureTrade = pStaticTrade->GetPicture(i);
		pPictureTrade->SetShow(false);


		CKTDGUIControl::CPictureData* pPictureEmblem200 = pStaticEmblem200->GetPicture(i);
		pPictureEmblem200->SetShow(false);


		CKTDGUIControl::CPictureData* pPictureEmblemGoldMedal = pStaticEmblemGoldMedal->GetPicture(i);
		pPictureEmblemGoldMedal->SetShow(false);


		
	
	//	CKTDGUIStatic* pPlayerMarkStatic = (CKTDGUIStatic*)m_pDLGDungeonRoomFront->GetControl( L"StaticDungeonRoomPlayerGrade" );
	//	CKTDGUIControl::CPictureData* pTempPlayerMarkPicture = pPlayerMarkStatic->GetPicture(i);

	//	if ( m_pCX2DungeonRoom->GetMySlot()->m_Index == i )
	//		pTempPlayerMarkPicture->SetShow(true);
	//	else
	//		pTempPlayerMarkPicture->SetShow(false);


	//	pStringUnitDot = pStaticUnitDot->GetString( i );
	//	pStringUnitLVNum = pStaticUnitLVNum->GetString( i );
	//	pStringUnitLV = pStaticUnitLV->GetString(i);
	//	pStringUnitTitle = pStaticUnitTitle->GetString(i);

		CX2DungeonRoom::SlotData* pSlotData = m_pCX2DungeonRoom->GetSlotData( i );

		if( pSlotData != NULL && pSlotData->m_SlotState == CX2DungeonRoom::SS_LOADING
			|| pSlotData->m_SlotState == CX2DungeonRoom::SS_PLAY
			|| pSlotData->m_SlotState == CX2DungeonRoom::SS_WAIT )
		{
			if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
			{
				if ( pSlotData->m_pUnit->GetUnitData().m_bIsGameBang == true )
				{
					pPicturePCRoom->SetShow( true );
				}

				// note!! 임시로 머리위에 엠블렘 아이템 표시 
				const int MAGIC_EMBLEM_ITEM_ID				= 129785;
				const int MAGIC_EMBLEM_ITEM_ID_GOLD_MEDAL	= 130002;

				bool bEquippedEmblem = false;

				const CX2Item::ItemTemplet* pItemTempet = g_pData->GetItemManager()->GetItemTemplet( MAGIC_EMBLEM_ITEM_ID );
				if( NULL != pItemTempet )
				{
					CX2Item* pEmblemItem = pSlotData->m_pUnit->GetInventory().GetEquippingItemByEquipPos( pItemTempet->GetEqipPosition(), false );
					if( NULL != pEmblemItem && 
                        pEmblemItem->GetItemTemplet()->GetItemID() == pItemTempet->GetItemID()
                        )
					{
						bEquippedEmblem = true;
						pPictureEmblem200->SetShow( true );
					}
				}
				
				if( false == bEquippedEmblem )
				{
					pItemTempet = g_pData->GetItemManager()->GetItemTemplet( MAGIC_EMBLEM_ITEM_ID_GOLD_MEDAL );
					if( NULL != pItemTempet )
					{
						CX2Item* pEmblemItem = pSlotData->m_pUnit->GetInventory().GetEquippingItemByEquipPos( pItemTempet->GetEqipPosition(), false );
						if( NULL != pEmblemItem && 
                            pEmblemItem->GetItemTemplet()->GetItemID() == pItemTempet->GetItemID()
                            )
						{
							bEquippedEmblem = true;
							pPictureEmblemGoldMedal->SetShow( true );			
						}
					}
				}
			}

	//		pStringUnitDot->msg = L".";

	//		WCHAR tempLevelNum[20] = {0};
	//		wsprintf(tempLevelNum, L"%d", pSlotData->m_pUnit->GetUnitData().m_Level);
	//		pStringUnitLVNum->msg = tempLevelNum;

	//		pStringUnitLV->msg = L"LV";

	//		pStringUnitTitle->msg = L"콤보의 최고봉";
	//		pStringData = pStaticUnitID->GetString( i );

	//		//닉네임을 찍는다
			if( pSlotData->m_TeamNum == CX2DungeonRoom::TN_RED )
			{
	//			//pStringData->color = 0xfff08080;
	//			pStringData->color = 0xffffffff;
				if( pSlotData->m_bHost == true )
				{
					pPictureHost->SetShow(true);
				}
				else if( pSlotData->m_bReady == true )
				{
					pPictureReady->SetShow(true);
				}
				else
				{
					//pStringData->msg = L"";
				}
	//			pStringData->msg = m_pCX2DungeonRoom->GetSlotData( i )->m_pUnit->GetNickName();
			}
			else
			{
	//			//pStringData->color = D3DXCOLOR(0,84,221,255);
	//			pStringData->color = 0xffffffff;
				if( pSlotData->m_bHost == true )
				{
					//pStringData->msg = L"호스트: ";
					pPictureHost->SetShow(true);
				}
				else if( pSlotData->m_bReady == true )
				{
					//pStringData->msg = L"레디: ";
					pPictureReady->SetShow(true);
				}
				else
				{
					//pStringData->msg = L"";
				}
	//			pStringData->msg = m_pCX2DungeonRoom->GetSlotData( i )->m_pUnit->GetNickName();
			}

			if ( pSlotData->m_bPitIn == true || pSlotData->m_bTrade == true )
			{
				pPictureEquip->SetShow( true );
			}
			/*
			if( true == pSlotData->m_bTrade )
			{
				pPictureTrade->SetShow( true ); 
			}
			*/


			//유닛 찍기
			if( pSlotData->m_pUnitViewer != NULL && pSlotData->m_pUnitViewer->GetXSkinAnim() != NULL && 
				pSlotData->m_pUnitViewer->GetXSkinAnim()->GetState() != CKTDGXSkinAnim::XAS_PLAYING )
			{
				if ( pSlotData->m_bHost == true || pSlotData->m_bReady == false )
				{
					pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
				}


				//유닛 찍기
				if( pSlotData->m_pUnitViewer != NULL && pSlotData->m_pUnitViewer->GetXSkinAnim() != NULL )
				{
					if( pSlotData->m_pUnitViewer->GetXSkinAnim()->GetState() != CKTDGXSkinAnim::XAS_PLAYING )
					{
						if ( pSlotData->m_bHost == true || pSlotData->m_bReady == false )
						{
							pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
						}
					}
				}

			}
			UnitViewerProcess( pSlotData->m_pUnitViewer, i );
		}
		else
		{	
	//		pStringData = pStaticUnitID->GetString( i );
	//		pStringData->msg = L"";
	//		pStringUnitDot->msg = L"";
	//		pStringUnitLV->msg = L"";
	//		pStringUnitLVNum->msg = L"";
	//		pStringUnitTitle->msg = L"";
		}
	}


	//CKTDGUIStatic* pStaticMapInfo = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl( L"StaticMapInfo" );

	//for ( int i = 1; i < CX2World::WI_END; i++ )
	//{
	//	if ( pStaticMapInfo->GetPicture(i) != NULL )
	//	{
	//		pStaticMapInfo->GetPicture(i)->SetShow(false);
	//	}
	//}
	//pStaticMapInfo->GetPicture(1)->SetShow(true); 

	//// 난이도 설정 변화
	CX2DungeonRoom::SlotData* pMySlotData = m_pCX2DungeonRoom->GetMySlot();
	if( pMySlotData != NULL && pMySlotData->m_bHost == true )
	{
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Ready")->SetEnable(false);
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Ready")->SetShow(false);

		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->SetEnable(true);
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->SetEnable(true);	

		CX2Dungeon::DIFFICULTY_LEVEL difficultyLevel = g_pData->GetDungeonRoom()->GetDifficulty();
		if ( difficultyLevel == CX2Dungeon::DL_NORMAL )
		{
			if ( m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->GetIsMoving() == false )
			{
				m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,0,0,1), 0.5f );
				m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.2f );
			}
		}
		else if ( difficultyLevel == CX2Dungeon::DL_HARD )
		{
			if ( m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->GetIsMoving() == false )
			{
				m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,0,0,1), 0.5f );
				m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.2f );
			}

			if ( m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->GetIsMoving() == false )
			{
				m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,0,0,1), 0.5f );
				m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.2f );
			}
		}
		else
		{
			if ( m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->GetIsMoving() == false )
			{
				m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,0,0,1), 0.5f );
				m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.2f );
			}
		}

		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Start")->SetEnable(true);
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Start")->SetShow(true);	

		m_pDLGDungeonRoomFront->GetControl(L"RadioButtonRandom")->SetEnable(true);
		m_pDLGDungeonRoomFront->GetControl(L"RadioButtonPersonal")->SetEnable(true);

	}
	else
	{
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Start")->SetEnable(false);
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Start")->SetShow(false);	

		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->SetShowEnable( false, false );
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->SetShowEnable( false, false );
	

		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Ready")->SetEnable(true);
		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Ready")->SetShow(true);

		m_pDLGDungeonRoomFront->GetControl(L"RadioButtonRandom")->SetEnable(false);
		m_pDLGDungeonRoomFront->GetControl(L"RadioButtonPersonal")->SetEnable(false);

	}

	SetUIOXButton();

	CKTDGUIStatic* pStaticDiff = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl(L"Difficulty");
	CKTDGUIStatic* pStaticDifficulty = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl( L"DungeonRoom_DungeonDifficulty" );
	CKTDGUIStatic* pStaticDiffStar = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl(L"Difficulty_Star");
	CX2Dungeon::DIFFICULTY_LEVEL difficulty = m_pCX2DungeonRoom->GetDifficulty();
	switch( difficulty )
	{
	case CX2Dungeon::DL_NORMAL:
		pStaticDiff->GetString(0)->msg = L"";
		pStaticDifficulty->GetString(0)->msg = GET_STRING( STR_ID_385 );
		pStaticDiffStar->GetString(0)->msg = L"";
		break;

	case CX2Dungeon::DL_HARD:

		if ( m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_NORMAL ||
			m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_HARD || 
			m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_EXPERT )
		{
			pStaticDiff->GetString( 0 )->msg = L"";
			pStaticDiffStar->GetString( 0 )->msg = L"";
		}
		else
		{
			pStaticDiff->GetString(0)->msg = GET_STRING( STR_ID_620 );
            pStaticDiff->GetString(0)->msg += L" :";
			pStaticDiffStar->GetString(0)->msg = GET_STRING( STR_ID_621 );
		}

		pStaticDifficulty->GetString(0)->msg = GET_STRING( STR_ID_386 );

		break;

	case CX2Dungeon::DL_EXPERT:

		if ( m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_NORMAL ||
			m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_HARD || 
			m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_EXPERT )
		{
			pStaticDiff->GetString( 0 )->msg = L"";
			pStaticDiffStar->GetString( 0 )->msg = L"";
		}
		else
		{
			pStaticDiff->GetString(0)->msg = GET_STRING( STR_ID_620 );
            pStaticDiff->GetString(0)->msg += L" :";
			pStaticDiffStar->GetString(0)->msg = GET_STRING( STR_ID_622 );
		}

		pStaticDifficulty->GetString(0)->msg = GET_STRING( STR_ID_388 );

		break;
	}

	D3DXCOLOR diffColor = g_pData->GetDungeonManager()->GetDifficultyColor( g_pData->GetDungeonRoom()->GetDungeonID(),
		g_pData->GetDungeonRoom()->GetDifficulty(),
		g_pData->GetSelectUnitLevel() );

	D3DXCOLOR limitLevelColor = g_pData->GetDungeonManager()->GetLimitLevelColor( 
		g_pData->GetDungeonRoom()->GetDungeonID(), 
		g_pData->GetDungeonRoom()->GetDifficulty(),
		g_pData->GetSelectUnitLevel() );

	ASSERT( !L"Wrong Path" );
	CKTDGUIStatic* pStaticRecommendLevel = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl(L"Static_Dungeon_RecommendLevel");
// 	pStaticRecommendLevel->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_384, "ii",
//         ( int )m_pCX2DungeonRoom->GetRecommendLevel( ( int )difficulty ).x,
//         ( int )m_pCX2DungeonRoom->GetRecommendLevel( ( int )difficulty ).y ) );
	pStaticRecommendLevel->GetString( 0 )->color = diffColor; 

	int requireLevel = 0;
	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)( g_pData->GetDungeonRoom()->GetDungeonID() + g_pData->GetDungeonRoom()->GetDifficulty() ) );
	if ( pDungeonData != NULL )
		requireLevel = pDungeonData->m_MinLevel;
	
	//wsprintf( buff, L"제한레벨 : %d", requireLevel );
	//CKTDGUIStatic* pStaticRequireLevel = (CKTDGUIStatic*)m_pDLGDungeonRoomBack->GetControl(L"requireLevel");
	//pStaticRequireLevel->GetString( 0 )->msg = buff;
	//pStaticRequireLevel->GetString( 0 )->color = limitLevelColor; 

	
	pStaticDiff->GetString( 0 )->color = diffColor; 
	pStaticDiffStar->GetString( 0 )->color = diffColor; 



	if( true == m_pCX2DungeonRoom->GetMySlot()->GetIsHost() )
	{
		CKTDGUIButton* pButtonDifficultyLeft = (CKTDGUIButton*) m_pDLGDungeonRoomFront->GetControl( L"Dungeon_LeftArrow" );
		CKTDGUIButton* pButtonDifficultyRight = (CKTDGUIButton*) m_pDLGDungeonRoomFront->GetControl( L"Dungeon_RightArrow" );
		CKTDGUIControl::UIStringData* pStaticDifficultyString = (CKTDGUIControl::UIStringData*) m_pDLGDungeonRoomBack->GetStatic_LUA( "DungeonRoom_DungeonDifficulty" )->GetString(0);

		switch( difficulty )
		{
		case CX2Dungeon::DL_NORMAL:
			pButtonDifficultyLeft->SetShowEnable( true, false );
			pButtonDifficultyRight->SetShowEnable( true, true );
			pStaticDifficultyString->msg = GET_STRING( STR_ID_385 );
			break;
		case CX2Dungeon::DL_HARD:
			pButtonDifficultyLeft->SetShowEnable( true, true );
			pButtonDifficultyRight->SetShowEnable( true, true );
			pStaticDifficultyString->msg = GET_STRING( STR_ID_386 );
			break;
		case CX2Dungeon::DL_EXPERT:
			pButtonDifficultyLeft->SetShowEnable( true, true );
			pButtonDifficultyRight->SetShowEnable( true, false );
			pStaticDifficultyString->msg = GET_STRING( STR_ID_388 );
			break;
		}

		if( NULL != pDungeonData )
		{
			if( true == pDungeonData->m_bNormalOnly )
			{
				pButtonDifficultyLeft->SetShowEnable( false, false );
				pButtonDifficultyRight->SetShowEnable( false, false );
			}
		}
	}
}

bool CX2StateDungeonRoom::SetUserBox( CX2Room::SlotData* pSlotData )
{
	if ( pSlotData == NULL )
		return false;

	if ( pSlotData->m_pUnit != NULL )
	{
		CKTDGUIDialogType pDialogUserBack = m_DungeonRoomUserBoxList[ pSlotData->m_Index ]->pDialogUserBack;
		if ( pDialogUserBack != NULL )
		{
			CKTDGUIStatic* pStatic = (CKTDGUIStatic*)pDialogUserBack->GetControl( L"PVP_Result_Info_Bg_Character" );
			if ( pStatic != NULL )
			{
				pStatic->SetShow( true );
				for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
				{
					if( pStatic->GetPicture(i) != NULL )
					{
						pStatic->GetPicture(i)->SetShow( false );		// note!! 신캐릭터 추가시 스크립트에 추가
					}
				}

				if ( pStatic->GetPicture( pSlotData->m_pUnit->GetUnitTemplet()->m_UnitType - 1 ) != NULL )
				{
					pStatic->GetPicture( pSlotData->m_pUnit->GetUnitTemplet()->m_UnitType - 1 )->SetShow( true );
				}
			}
		}

		CKTDGUIDialogType pDialog = m_DungeonRoomUserBoxList[ pSlotData->m_Index ]->pDialog;
		m_DungeonRoomUserBoxList[ pSlotData->m_Index ]->unitUID = pSlotData->m_pUnit->GetUID();

		m_DungeonRoomUserBoxList[ pSlotData->m_Index ]->SetShow( true );

		D3DXVECTOR3 dummyPos = m_pDLGDungeonRoomBack->GetDummyPos( pSlotData->m_Index );
		pDialog->SetPos( D3DXVECTOR2( dummyPos.x, dummyPos.y ) );
		CKTDGUIStatic* pStaticSlotIndex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticDungeon_Room_SlotIndex" );
		WCHAR buf[256] = {0};
		_itow( pSlotData->m_Index, buf, 10 );
		if ( pStaticSlotIndex != NULL )
			pStaticSlotIndex->GetString( 0 )->msg = buf;

		CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)pDialog->GetControl( L"StaticDungeon_Room_UserInfo" );
		_itow( (int)pSlotData->m_pUnit->GetUnitData().m_Level, buf, 10 );
		if ( pStaticUserInfo != NULL )
		{
			pStaticUserInfo->GetString( 1 )->msg = buf;
			pStaticUserInfo->GetString( 2 )->msg = pSlotData->m_pUnit->GetNickName();
		}
		

		CKTDGUIStatic* pStaticUserInfo_BG = (CKTDGUIStatic*)pDialog->GetControl( L"Dungeon_Room_UserInfo" );
		if ( pStaticUserInfo_BG != NULL && pStaticUserInfo_BG->GetPicture(0) != NULL && pStaticUserInfo_BG->GetPicture(1) != NULL )
		{
			if ( pSlotData != g_pData->GetDungeonRoom()->GetMySlot() )
			{
				pStaticUserInfo_BG->GetPicture(0)->SetShow( true );
				pStaticUserInfo_BG->GetPicture(1)->SetShow( false );
			}
			else
			{
				pStaticUserInfo_BG->GetPicture(0)->SetShow( false );
				pStaticUserInfo_BG->GetPicture(1)->SetShow( true );
			}
		}

		CKTDGUIButton* pButtonBan		= (CKTDGUIButton*) pDialog->GetControl( L"Dungeon_Room_User_exile" );
		CKTDGUIButton* pButtonWhisper	= (CKTDGUIButton*) pDialog->GetControl( L"Dungeon_Room_User_whisper" );


		if ( g_pData->GetDungeonRoom()->GetMySlot() != NULL )
		{
			if ( g_pData->GetDungeonRoom()->GetMySlot()->m_bHost == true || 
				g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				pButtonBan->SetShow( true );
				pButtonBan->SetEnable( true );
				pButtonBan->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, 1.f) );
			}
			else
			{
				pButtonBan->SetShow( true );
				pButtonBan->SetEnable( false );
				pButtonBan->SetColor( D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) );
			}
		}


		pButtonWhisper->SetShow( true );
		pButtonWhisper->SetEnable( true );
		pButtonWhisper->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, 1.f) );

		if ( pSlotData->m_bMySlot == true )
		{
			pButtonWhisper->SetShow( true );
			pButtonWhisper->SetEnable( false );
			pButtonWhisper->SetColor( D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) );

			pButtonBan->SetShow( true );
			pButtonBan->SetEnable( false );
			pButtonBan->SetColor( D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) );
		}



		CKTDGUIStatic* pStatic_Ping = (CKTDGUIStatic*) pDialog->GetControl( L"Dungeon_Room_UserInfo_Ping" );
		if( NULL != pStatic_Ping )
		{
			pStatic_Ping->GetPicture(0)->SetShow( false );
			pStatic_Ping->GetPicture(1)->SetShow( false );
			pStatic_Ping->GetPicture(2)->SetShow( false );
			pStatic_Ping->GetPicture(3)->SetShow( false );
			pStatic_Ping->GetPicture(4)->SetShow( false );


			if( pSlotData->m_fElapsedTimeAfterJoinRoom < 5.f )
			{
				pStatic_Ping->GetPicture(4)->SetShow( true );
			}
			else if( pSlotData->m_PingTime < 60 )
			{
				pStatic_Ping->GetPicture(3)->SetShow( true );
			}
			else if( pSlotData->m_PingTime < 120 )
			{
				pStatic_Ping->GetPicture(2)->SetShow( true );
			}
			else if( pSlotData->m_PingTime < 10000 )
			{
				pStatic_Ping->GetPicture(1)->SetShow( true );
			}
			else
			{
				pStatic_Ping->GetPicture(0)->SetShow( true );
			}
		}

		const CX2Unit::UnitData* pUnitData = &pSlotData->m_pUnit->GetUnitData();
		{
			CKTDGUIStatic* pStaticUserFatigue = (CKTDGUIStatic*)pDialog->GetControl( L"UserInfo_Fatigue" );	
			float _width = (pUnitData->m_iSpirit / (float)pUnitData->m_iSpiritMax ) * pStaticUserFatigue->GetPicture(0)->GetOriginalSize().x;
			pStaticUserFatigue->GetPicture(0)->SetSizeX( _width );
			pStaticUserFatigue->GetPicture(1)->SetSizeX( _width );
			pStaticUserFatigue->GetPicture(2)->SetSizeX( _width );

			float gauagePercentage = (pUnitData->m_iSpirit / (float)pUnitData->m_iSpiritMax );
			if ( gauagePercentage < 0.3 )
			{
				pStaticUserFatigue->GetPicture(0)->SetShow( true );
				pStaticUserFatigue->GetPicture(1)->SetShow( false );
				pStaticUserFatigue->GetPicture(2)->SetShow( false );
			}
			else
			{
				pStaticUserFatigue->GetPicture(0)->SetShow( false );
				pStaticUserFatigue->GetPicture(1)->SetShow( false );
				pStaticUserFatigue->GetPicture(2)->SetShow( true );
			}


			CKTDGUIButton* pButtonSpirit = (CKTDGUIButton*)pDialog->GetControl( L"SpiritButton" );
			if ( pButtonSpirit != NULL )
			{
				pButtonSpirit->SetGuideDesc( GET_REPLACED_STRING( ( STR_ID_2750, "i", ( int )( ( pUnitData->m_iSpirit / ( float )pUnitData->m_iSpiritMax ) * 100 ) ) ) );
			}

			if ( pUnitData->m_iSpirit == 0 )
			{
				CKTDGUIStatic* pStaticUserFatigueXIcon = (CKTDGUIStatic*)pDialog->GetControl( L"Dungeon_Room_UserInfo_X_Icon" );
				if ( pStaticUserFatigueXIcon != NULL )
				{
					pStaticUserFatigueXIcon->SetShow( true );
				}
			}

		}

		

		return true;
	}
	else
	{
		m_DungeonRoomUserBoxList[ pSlotData->m_Index ]->SetShow( false );
	}

	return false;
}

bool CX2StateDungeonRoom::EnterTutorial()
{

	if( NULL == m_pDLGLoadingState )
	{
		m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua" );
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );

		CKTDGUIStatic* pStatic = 
			static_cast<CKTDGUIStatic*>( m_pDLGLoadingState->GetControl( L"Static_Loading_Dungeon_BG" ) );

		// 그림 두장
		if( pStatic != NULL )
		{
			pStatic->SetShow( true );

			if( pStatic->GetPicture(0) != NULL )
				pStatic->GetPicture(0)->SetShow(false);

			if ( pStatic->GetPicture(1) != NULL )
				pStatic->GetPicture(1)->SetTex( L"HQ_Loading_RUBEN_PROLOG_01.tga" );

			if (pStatic->GetPicture(2) != NULL )
				pStatic->GetPicture(2)->SetTex( L"HQ_Loading_RUBEN_PROLOG_02.tga" );
		}

	}

	g_pMain->SetIsPlayingTutorial( true );

	return Handler_EGS_LEAVE_ROOM_REQ();
}




void CX2StateDungeonRoom::OnChatModeOpen()
{
	if( g_pChatBox->GetOpenChatWindow() == false )
	{
		//if( m_pCharacterRoom->GetOpen() == false )
#ifndef NEW_SKILL_TREE
		if( false == g_pInstanceData->GetSkillTreeUI()->GetShow() )
#endif
		{	
			if( g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyUID <= 0 )
				g_pChatBox->ChangeChatWindowAndIMEEditBox( CX2ChatBox::CWM_TOTAL );

			g_pChatBox->OpenChatWindow();
		}
	}
	else
	{
		g_pChatBox->SetFocus();
	}
}

void CX2StateDungeonRoom::OpenDungeonHelpPopUp()
{
	if ( m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_NORMAL ||
		m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_HARD ||
		m_pCX2DungeonRoom->GetDungeonID() == SEnum::DI_EL_FOREST_GATE_EXPERT )
	{
		bool bCheckNewDungeon = false;
		bool bCheckNextDifficulty = false;

		wstring newDungeonName = L"";
		wstring newDungeonDifficulty = L"";

		//////////////////////////////////////////////////////////////////////////
		// 새로 열린 던전에 대한 정보를 알려준다
		vector<int> vecNewlyOpened = g_pData->GetMyUser()->GetSelectUnit()->GetVecNewlyOpenedDungeon();
		if( !vecNewlyOpened.empty() )
		{
			g_pData->GetMyUser()->GetSelectUnit()->ClearNewlyOpenedDungeon();
			wstringstream wstrmDungeonOpened;

			for( UINT i=0; i<vecNewlyOpened.size(); i++ )
			{
				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) vecNewlyOpened[i] );
				if( NULL == pDungeonData )
					continue;

				//CX2Dungeon::DIFFICULTY_LEVEL eDiffculty = (CX2Dungeon::DIFFICULTY_LEVEL) (vecNewlyOpened[i] % 10 );
				CX2Dungeon::DIFFICULTY_LEVEL eDiffculty = (CX2Dungeon::DIFFICULTY_LEVEL) pDungeonData->m_eDifficulty;

				if ( eDiffculty == CX2Dungeon::DL_NORMAL )
				{
					g_pData->GetMyUser()->GetSelectUnit()->AddNewlyOpenedDungeonAtLocalMap( pDungeonData->m_DungeonID );

                    newDungeonName = GET_REPLACED_STRING( ( STR_ID_638, "L", pDungeonData->m_DungeonName ) );
					bCheckNewDungeon = true;
				}
				else
				{
					if ( eDiffculty == CX2Dungeon::DL_HARD )
					{
						newDungeonDifficulty = GET_STRING( STR_ID_639 );
					}
					else
					{
						newDungeonDifficulty = GET_STRING( STR_ID_640 );
					}

					bCheckNextDifficulty = true;
				}
			}
		}

		if ( bCheckNextDifficulty == true || bCheckNewDungeon == true )
		{
			m_pDLG_Dungeon_Help = new CKTDGUIDialog( this, L"DLG_Dungeon_Room_New_Dungeon_Guide.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLG_Dungeon_Help );

			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Difficulty_Open.ogg", false, false );

			if ( bCheckNewDungeon == true )
			{
				if ( newDungeonName.empty() == false )
				{
					CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLG_Dungeon_Help->GetControl( L"DungeonName" );
					if ( pStaticName != NULL && pStaticName->GetString(0) != NULL )
					{
						pStaticName->GetString(0)->msg = newDungeonName;
					}
				}
			}

			if ( bCheckNextDifficulty == true )
			{
				if ( newDungeonDifficulty.empty() == false )
				{
					CKTDGUIStatic* pStaticDifficulty = (CKTDGUIStatic*)m_pDLG_Dungeon_Help->GetControl( L"NextDungeonDifficulty" );
					if ( pStaticDifficulty != NULL && pStaticDifficulty->GetString(0) != NULL )
					{
						pStaticDifficulty->GetString(0)->msg = newDungeonDifficulty;
					}
				}
			}

			if ( bCheckNextDifficulty == true && bCheckNewDungeon == true )
			{
				//두개다 뚤리는 경우
				CKTDGUIControl* pControl = m_pDLG_Dungeon_Help->GetControl( L"StaticNew_Dungeon_Popup_Shadow_2Open" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				pControl = m_pDLG_Dungeon_Help->GetControl( L"StaticNew_Dungeon_Difficuty_Glow" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				pControl = m_pDLG_Dungeon_Help->GetControl( L"StaticNew_Dungeon_Back_Glow" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				pControl = m_pDLG_Dungeon_Help->GetControl( L"DungeonDifficulyTalkBox" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				if ( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
				{
					pControl = m_pDLG_Dungeon_Help->GetControl( L"g_pNew_Dungeon_Next_Difficulty" );
					if ( pControl != NULL )
						pControl->SetEnable( true );
				}

				pControl = m_pDLG_Dungeon_Help->GetControl( L"g_pNew_Dungeon_Next_Go" );
				if ( pControl != NULL )
					pControl->SetEnable( true );

			}
			else if ( bCheckNextDifficulty == true )
			{
				//다음 난이도만 번쩍이는 경우

				CKTDGUIControl* pControl = m_pDLG_Dungeon_Help->GetControl( L"StaticNew_Dungeon_Popup_Shadow_Open_Difficulty" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				pControl = m_pDLG_Dungeon_Help->GetControl( L"StaticNew_Dungeon_Difficuty_Glow" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				pControl = m_pDLG_Dungeon_Help->GetControl( L"DungeonDifficulyTalkBox" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				if ( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true )
				{
					pControl = m_pDLG_Dungeon_Help->GetControl( L"g_pNew_Dungeon_Next_Difficulty" );
					if ( pControl != NULL )
						pControl->SetEnable( true );
				}		
			}
			else if ( bCheckNewDungeon == true )
			{
				//뒤로 가기 버튼만 번쩍이는 경우

				CKTDGUIControl* pControl = m_pDLG_Dungeon_Help->GetControl( L"StaticNew_Dungeon_Popup_Shadow_Open_Back" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				pControl = m_pDLG_Dungeon_Help->GetControl( L"StaticNew_Dungeon_Back_Glow" );
				if ( pControl != NULL )
					pControl->SetShow( true );

				pControl = m_pDLG_Dungeon_Help->GetControl( L"g_pNew_Dungeon_Next_Go" );
				if ( pControl != NULL )
					pControl->SetEnable( true );
			}
		}
	}
	else
	{

		//////////////////////////////////////////////////////////////////////////
		// 새로 열린 던전에 대한 정보를 알려준다
		vector<int> vecNewlyOpened = g_pData->GetMyUser()->GetSelectUnit()->GetVecNewlyOpenedDungeon();
		if( vecNewlyOpened.size() > 0 )
		{
			g_pData->GetMyUser()->GetSelectUnit()->ClearNewlyOpenedDungeon();
			wstringstream wstrmDungeonOpened;

			wstrmDungeonOpened << GET_STRING( STR_ID_641 ) << std::endl;

			for( UINT i=0; i<vecNewlyOpened.size(); i++ )
			{
				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) vecNewlyOpened[i] );
				if( NULL == pDungeonData )
					continue;


				CX2Dungeon::DIFFICULTY_LEVEL eDiffculty = (CX2Dungeon::DIFFICULTY_LEVEL) pDungeonData->m_eDifficulty;
				if ( eDiffculty == CX2Dungeon::DL_NORMAL )
				{
					g_pData->GetMyUser()->GetSelectUnit()->AddNewlyOpenedDungeonAtLocalMap( pDungeonData->m_DungeonID );
				}

				wstrmDungeonOpened << g_pData->GetDungeonManager()->MakeDungeonNameString( (SEnum::DUNGEON_ID) vecNewlyOpened[i], eDiffculty, CX2Dungeon::DM_INVALID ) << std::endl;
			}
			
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), wstrmDungeonOpened.str().c_str(), this, -1, 3.f );
		}
	}	
}


int CX2StateDungeonRoom::LineBreak( wstring& wstrText, int iTextWidth, const wstring& wstrTextColor, bool bEnableColor, CKTDGFontManager::CUKFont* pFont )
{
	KTDXPROFILE();

	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

	if( NULL == pFont )
		return 0;

	int iNowCharWidth 		= 0;
	int iNowLineWidth 		= 0;
	int iNowLineCount 		= 0;
	int iNowCharIndex 		= 0;
	wstring wstrLineBuf		= L"";
	wstring wstrColorCode	= L"";

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 


	if( 0 != wstrTextColor.length() )
	{
		wstrLineBuf += wstrTextColor;
	}

	while( iNowCharIndex < (int) wstrText.length() )
	{
		WCHAR wszCharBuf = wstrText[iNowCharIndex];
		wstring wstrCharBuf = wstrText.substr( iNowCharIndex, 1 );

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					if( true == bEnableColor )
					{
						wstrLineBuf += wstrCharBuf;
						eNowCharState = Q_SHARP;
					}
					else
					{
						wstrLineBuf += L"*"; 
						iNowCharWidth = pFont->GetWidth( L"*" );
						iNowLineWidth += iNowCharWidth;
					}
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					if( bIsFirstCharOfLine == false )
					{
						wstrLineBuf += wstrCharBuf;
						iNowLineCount++;
						iNowLineWidth = 0;
						bIsFirstCharOfLine = true;

						if( 0 != wstrTextColor.length() )
						{
							wstrLineBuf += wstrTextColor;
						}

						if( 0 != wstrColorCode.length() )
						{
							wstrLineBuf += wstrColorCode;
						}
					}
				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;					
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;
					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else
				{
					if( true == bIsFirstCharOfLine )
						bIsFirstCharOfLine = false;

					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;
				}
			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrColorCode = L"";

					if( 0 != wstrTextColor.length() )
					{
						wstrLineBuf += wstrTextColor;
					}

					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					// fix!! color code가 올바른지 체크하는 코드 있어야 됨. 일단 color code는 무조건 올바른것으로 가정
					wstrColorCode = L"#C";
					wstrColorCode += wstrText.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrText.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

			} break;
		}

		if( iNowLineWidth >= iTextWidth && Q_NORMAL == eNowCharState )
		{
			wstrLineBuf += L"\n";
			iNowLineCount++;
			iNowLineWidth = 0;
			bIsFirstCharOfLine = true;

			if( 0 != wstrTextColor.length() )
			{
				wstrLineBuf += wstrTextColor;
			}

			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}
		}

		iNowCharIndex++;
	}


	wstrText = wstrLineBuf;

	return iNowLineCount;

}

void CX2StateDungeonRoom::ToggleCashShop()
{
	m_bReserveCashShopOpen = true;
	Handler_EGS_LEAVE_ROOM_REQ();
}


///*virtual*/ void CX2StateDungeonRoom::SetSkillUseModeRadioButtonChecked( CX2GameOption::SKILL_USE_MODE eMode )
//{
//	if( NULL == m_pDLGDungeonRoomFront )
//		return;
//
//	CKTDGUIRadioButton* pRadio_ImmediateMode	= (CKTDGUIRadioButton*) m_pDLGDungeonRoomFront->GetControl( L"short" );
//	CKTDGUIRadioButton* pRadio_ChargeMode		= (CKTDGUIRadioButton*) m_pDLGDungeonRoomFront->GetControl( L"hold" );
//
//
//	switch( eMode )
//	{
//	case CX2GameOption::SUM_IMMEDIATE:
//		{
//			pRadio_ImmediateMode->SetChecked( true );
//			pRadio_ChargeMode->SetChecked( false );
//		} break;
//
//	default:
//	case CX2GameOption::SUM_CHARGE:
//		{
//			pRadio_ImmediateMode->SetChecked( false );
//			pRadio_ChargeMode->SetChecked( true );
//		} break;
//	}
//}
