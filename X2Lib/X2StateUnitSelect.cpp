#include "StdAfx.h"
#include ".\x2stateunitselect.h"

/*
CX2StateUnitSelect::CX2StateUnitSelect(void)
{
	if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // 체험 아이디 제한
	{
		if( g_pMain->GetNexonLoginMessenger() == true && g_pMain->GetNexonVirtualLogin() == true )
		{
			if ( CNMCOClientObject::GetInstance().LogoutVirtual() == false )
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"넥슨 버추얼 로그아웃 실패", this );

			g_pMain->SetNexonVirtualLogin( false );
		}
	}

	if ( g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->Reset();


	g_pData->DeletePVPRoom();
	g_pData->DeleteDungeonRoom();

	g_pMain->GetInformerManager()->InvalidInfo();
	g_pMain->GetMemoryHolder()->ReleaseLocalMapDLG();

	g_pData->DeleteCharacterRoom();
	g_pMain->GetMemoryHolder()->RelaseCommonDLG();

	m_pDLGUnitSelectBack = new CKTDGUIDialog( this, L"DLG_Unit_Select_Back.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUnitSelectBack );

	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_Unit_Select_Front.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUnitSelectFront );


	m_pDLGDeleteUnitCheck = NULL;
	m_pSelectUnit	= NULL;
	m_pSelectButton = NULL;
	m_pDLGLoadingState = NULL;

	m_pDLGRenameMsgBox = NULL;
	m_pDLGTutorialMsgBox = NULL;
	m_pDLGGoToFirstDungeon = NULL;
	m_pDLGRecommend = NULL;
	m_pDLGRecommendReward = NULL;
	m_pDLGRecommendSuccess = NULL;

	m_pDLGTutorReward		= NULL;
	m_pDLGGuestUserReward	= NULL;


	m_bReserveEnterTutorial = false;
	m_bCheckPostItem = false;
	m_bCheckRecommend = false;
	m_bCheckRecommendSuccess = false;

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
		g_pKTDXApp->GetDGManager()->GetFar(),
		false );





	m_bPlayDownAnim = false;
	m_fDownAnimTime = 0.0f;

	m_bUnitClicked = false;
	m_fMouseSensitivity = 0.01f;

	m_fRemainRotY = 0.0f;
	m_bRotateReturn = true;

	m_pOveredButton = NULL;

	m_bOneTimeInit = true;
	m_bStateChangingToUnitCreate = false;

	m_MaxUnitNum	= g_pData->GetMyUser()->GetUserData().maxUnitCount;

	m_NowPage		= 1;
	m_MaxPage		= (m_MaxUnitNum / UNIT_SELECT_UI_MAX_UNIT_NUM);
	if ( m_MaxUnitNum % UNIT_SELECT_UI_MAX_UNIT_NUM > 0 )
		m_MaxPage++;

	CreateUnitButton();

	//SetShowCharacterNameForSelectedUnit();

	if( NULL != g_pMain->GetTutorSystem() )
	{
		g_pMain->GetTutorSystem()->SetBeenUnitSelect( true );
	}

	if( true == g_pMain->GetIsExitingTutorial() )
	{
		switch( g_pMain->GetStateIDBeforeTutorial() )
		{
		case CX2Main::XS_SERVER_SELECT:
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

	g_pData->ResetPartyManager();


	g_pData->GetMyUser()->SelectUnit( NULL );


	g_pKTDXApp->SkipFrame();
}

CX2StateUnitSelect::~CX2StateUnitSelect(void)
{
	ClearUnitButton();

	SAFE_DELETE_DIALOG( m_pDLGUnitSelectBack );
	SAFE_DELETE_DIALOG( m_pDLGUnitSelectFront );

	SAFE_DELETE( m_pDLGLoadingState ); // note: 얘는 다이얼로그 매니저에서 관리되는 애가 아니라서~ 요렇게


	SAFE_DELETE_DIALOG( m_pDLGRenameMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGTutorialMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGGoToFirstDungeon );
	SAFE_DELETE_DIALOG( m_pDLGRecommend );
}

HRESULT CX2StateUnitSelect::OnFrameMove( double fTime, float fElapsedTime )
{

	if( true == m_bOneTimeInit )
	{
		m_bOneTimeInit = false;

		if( 0 == g_pData->GetMyUser()->GetUnitNum() )
		{
			g_pMain->CreateStateChangeDLG( L"캐릭터 생성창으로 이동중입니다." );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CREATE_UNIT, NULL, false );
			m_bStateChangingToUnitCreate = true;
		}
	}
	
	CX2State::OnFrameMove( fTime, fElapsedTime );

	static bool bAutoStart = true;

	if ( bAutoStart == true && g_pMain->m_bAutoStart == true )
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



	if ( g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->CheckFriendReq();

	if ( m_bCheckPostItem == true )
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGRecommendReward ) == false &&
			g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGTutorReward ) == false &&
			g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGGuestUserReward ) == false )
		{
			m_bCheckPostItem = false;
			Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
		}
	}

	if ( m_bCheckRecommendSuccess == true )
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGRecommendSuccess ) == false )
		{
			m_bCheckRecommendSuccess = false;
			Handler_EGS_GET_POST_LETTER_LIST_REQ();
		}
	}

	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
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
		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RIGHT ) == TRUE )
		{
			if ( m_NowPage < m_MaxPage )
			{
				if ( (m_NowPage * UNIT_SELECT_UI_MAX_UNIT_NUM) <= g_pData->GetMyUser()->GetUnitNum() )
				{	
					ClearBeforeUnit( m_pSelectUnit );
					ClearUnitButton();

					m_NowPage++;

					CreateUnitButton();
				}
			}
		}

	}

	return S_OK;
}






bool CX2StateUnitSelect::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{

			m_bUnitClicked = false;
		}
		break;
	}

	return CX2State::MsgProc( hWnd, uMsg, wParam, lParam );
}

bool CX2StateUnitSelect::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch(wParam)
	{
		
	}

	return false;
}

bool CX2StateUnitSelect::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch( wParam )
	{


	}
	return false;
}





bool CX2StateUnitSelect::AnimateCharacterName( CKTDGUIButton* pButton )
{
	if ( m_pOveredButton == pButton )
		return false;

	//SetShowCharacterName( false );

	for( int i = 0; i < g_pData->GetMyUser()->GetUnitNum(); i++ )
	{
		CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( i );
		if( pUnit == NULL )
			continue;

		if ( wcscmp( pUnit->GetNickName(), pButton->GetName() ) == 0 && pUnit != m_pSelectUnit )
		{
			CKTDGUIStatic* pUnitName = NULL;

			// note!! 전직 추가되면 수정, 혹은 unit_type으로
			if ( pUnit->GetClass() == CX2Unit::UC_ELSWORD_SWORDMAN )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_ELSWORD_KNIGHT )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_ELSWORD_MAGIC_KNIGHT )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_ARME_VIOLET_MAGE )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Arme" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_ARME_HIGH_MAGICIAN )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Arme" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_ARME_DARK_MAGICIAN )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Arme" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_LIRE_ELVEN_RANGER )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Lire" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_LIRE_COMBAT_RANGER )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Lire" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_LIRE_SNIPING_RANGER )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Lire" );
			}
			else if ( pUnit->GetClass() == CX2Unit::UC_RAVEN_FIGHTER )
			{
				pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword" );
			}

			D3DXVECTOR3 dummyPos = m_pDLGUnitSelectBack->GetDummyPos(i);
			D3DXVECTOR2 namePos = D3DXVECTOR2( dummyPos.x - 120, dummyPos.y );
			pUnitName->SetShow( true );
			pUnitName->SetOffsetPos( namePos );
			pUnitName->SetColor( D3DXCOLOR(0,0,0,0) );

			D3DXVECTOR2 namePosToMove = namePos;
			namePosToMove.x += 120;
			pUnitName->Move( namePosToMove, D3DXCOLOR(1,1,1,1), 0.15f, true, false );
			namePosToMove.x -= 10;
			pUnitName->Move( namePosToMove, D3DXCOLOR(1,1,1,1), 0.05f, false, false );

			m_pOveredName	= pUnitName;
			m_pOveredButton = pButton;
		}

	}

	return true;
}

void CX2StateUnitSelect::SetShowCharacterName( bool bShow )
{
	
	m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword" )->SetShow( bShow );
	m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Arme" )->SetShow( bShow );
	m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Lire" )->SetShow( bShow );
	
}


	
void CX2StateUnitSelect::ResetCrashReport()
{
	g_pMain->ResetCrashReport( L"" );
}

void CX2StateUnitSelect::SetShowCharacterNameForSelectedUnit()
{
	
	m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword_Selected" )->SetShow( false );
	m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Arme_Selected" )->SetShow( false );
	m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Lire_Selected" )->SetShow( false );

	if ( m_pSelectUnit != NULL )
	{
		for( int i = 0; i < g_pData->GetMyUser()->GetUnitNum(); i++ )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( i );
			if( pUnit == m_pSelectUnit )
			{
				CKTDGUIStatic* pUnitName = NULL;

				// note!! 전직 추가되면 수정, 혹은 unit_type으로
				if ( pUnit->GetClass() == CX2Unit::UC_ELSWORD_SWORDMAN )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_ELSWORD_KNIGHT )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_ELSWORD_MAGIC_KNIGHT )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_ARME_VIOLET_MAGE )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Arme_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_ARME_HIGH_MAGICIAN )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Arme_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_ARME_DARK_MAGICIAN )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Arme_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_LIRE_ELVEN_RANGER )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Lire_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_LIRE_COMBAT_RANGER )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Lire_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_LIRE_SNIPING_RANGER )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Lire_Selected" );
				}
				else if ( pUnit->GetClass() == CX2Unit::UC_RAVEN_FIGHTER )
				{
					pUnitName = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Unit_Select_Name_Elsword_Selected" );
				}

				if ( pUnitName != NULL )
				{
					D3DXVECTOR3 dummyPos = m_pDLGUnitSelectBack->GetDummyPos(i);
					pUnitName->SetOffsetPos( D3DXVECTOR2( dummyPos.x - 10, dummyPos.y ) );
					pUnitName->SetShow( true );
				}
			}
		}
	}
	
}


*/