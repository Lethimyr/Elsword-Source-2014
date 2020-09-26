#include "StdAfx.h"
#include ".\x2statetraininggame.h"






#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#include "./X2GUElsword_SwordMan.h"
#include "./X2GUArme_VioletMage.h"
#include "./X2GULire_ElvenRanger.h"
#include "./X2GURaven.h"
#include "./X2GuEve.h"

//{{ kimhc // 2010.11.25 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#include "./X2GUChung.h"
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.25 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
#include "./X2GUAra.h"
#endif
#ifdef NEW_CHARACTER_EL
#include "./X2GUEl.h"
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#include "./X2GUAdd.h"
#endif //SERV_9TH_NEW_CHARACTER


#endif REDUCED_PRECOMPILED_HEADER_TEST





CX2StateTrainingGame::CX2StateTrainingGame(void) :
// 2009.01.09 김태완 : 초기화 코드정리
m_fRemainTimeToGameEnd(0.0f),
#ifdef OLD_COMBO_LIST_UI
	m_pDLGComboList(NULL),
#endif OLD_COMBO_LIST_UI
m_pUINPC(NULL),
m_bSend_EGS_END_TC_GAME_REQ(false)
{
	// 초기화 ////////////////////////////////////////////////////////////////////////
// 	m_fRemainTimeToGameEnd = 0.f;
// 	m_bSend_EGS_END_TC_GAME_REQ = false;
// 	m_pUINPC = NULL;

	//////////////////////////////////////////////////////////////////////////

	if( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if( NULL != pDungeonGame )
			pDungeonGame->CreateTrainingGameUI( (CX2State*) this );
	}

	if( false == g_pMain->GetIsPlayingFreeTraining() )
	{
		m_pUINPC = g_pData->GetUIUnitManager()->GetUIUnit( CX2UnitManager::NUI_LOW );
		if( m_pUINPC != NULL )
		{
			g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUINPC );
			m_pUINPC->SetShowObject( true );
			m_pUINPC->SetCameIn( true );
			m_pUINPC->StateChange( "LOW_TRAINING_IN" );
			m_pUINPC->SetLayer( XL_MINIMAP );
		}
	}

	// 09.03.11 김태완
	SetShowMenu(false);
#ifdef REFORM_UI_SKILLSLOT
	g_pData->GetUIManager()->SetShowSkillSlot(true);
	g_pData->GetUIManager()->SetShowQuickSlot(false);
#endif //REFORM_UI_SKILLSLOT

}

CX2StateTrainingGame::~CX2StateTrainingGame(void)
{
#ifdef OLD_COMBO_LIST_UI
	SAFE_DELETE_DIALOG( m_pDLGComboList );
#endif OLD_COMBO_LIST_UI

	if( m_pUINPC != NULL )
	{
		g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pUINPC );
		m_pUINPC->SetShowObject( false );
		m_pUINPC->SetLayer( 0 );
	}
}

HRESULT CX2StateTrainingGame::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2StateDungeonGame::OnFrameMove( fTime, fElapsedTime );


	if( m_pUINPC != NULL )
	{
		m_pUINPC->SetShowObject( true );
		m_pUINPC->OnFrameMove( fTime, fElapsedTime );
	}


	if( m_fRemainTimeToGameEnd > 0.f )
	{
		m_fRemainTimeToGameEnd -= fElapsedTime;
		if( m_fRemainTimeToGameEnd <= 0.f )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_738 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_TRAINING_RESULT, 0, false );
		}
	}

#ifndef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		for(UINT i=0; i<g_pData->GetPetManager()->GetPetNum(); ++i)
		{
			CX2PET *pPet = g_pData->GetPetManager()->GetPetInx(i);
			if( pPet != NULL )
			{
				pPet->InitGamePet();
			}
		}
	}
#endif

#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	return S_OK;
}



bool CX2StateTrainingGame::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//if( NULL != g_pX2Game )
	//{
	//	CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	//	if( NULL != pDungeonGame->GetTrainingGameUI() )
	//	{
	//		if( true == pDungeonGame->GetTrainingGameUI()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
	//			return true;
	//	}
	//}

	if( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if( NULL != pDungeonGame )
		{
			switch( wParam )
			{
			case STGUCM_NPC_TYPE:
				{
					CKTDGUIComboBox* pCombo = (CKTDGUIComboBox*)lParam;
					CX2UnitManager::NPC_UNIT_ID iNpcID = pDungeonGame->GetTrainingGameUI()->GetNpcIdByIndex( pCombo->GetSelectedItemIndex() );
					pDungeonGame->KillNPC( pDungeonGame->GetTrainingGameUI()->GetFTNPCID(), 100 );

					pDungeonGame->GetTrainingGameUI()->SetFTNPCID( iNpcID );
				} break;

			case STGUCM_NPC_COUNT:
				{
					CKTDGUIComboBox* pCombo = (CKTDGUIComboBox*)lParam;

					pDungeonGame->GetTrainingGameUI()->SetFTNPCCount( pCombo->GetSelectedItemIndex()+1 );

					return true;
				} break;

			case STGUCM_NPC_LEVEL:
				{
					CKTDGUIComboBox* pCombo = (CKTDGUIComboBox*)lParam;
					int iLevel = g_pData->GetSelectUnitLevel();
					iLevel += (pCombo->GetSelectedItemIndex() - 10);
					pDungeonGame->GetTrainingGameUI()->SetFTNPCLevel( iLevel );
					return true;
				} break;

			case STGUCM_NPC_AGGRESIVE:
				{
					CKTDGUIComboBox* pCombo = (CKTDGUIComboBox*)lParam;

					if( pDungeonGame == NULL )
						return true;


					switch( pCombo->GetSelectedItemIndex() )
					{
					case 0:
						{
							pDungeonGame->GetTrainingGameUI()->SetFTEnableNPCAttack( true );
						} break;

					case 1:
						{
							pDungeonGame->GetTrainingGameUI()->SetFTEnableNPCAttack( false );
						} break;
					}
					return true;
				} break;

			case STGUCM_RESET:
				{
					ResetUnit();
					return true;
				} break;


			case STGUCM_COMBO_LIST:
				{
#ifdef OLD_COMBO_LIST_UI
					//{{AFX
					if( true == GetIsOpenComboListWindow() )
					{
						OpenComboListWindow( false );
					}
					else
					{
						OpenComboListWindow( true );
					}
					//}}AFX
#else OLD_COMBO_LIST_UI

					g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_COMBO_TREE );

#endif OLD_COMBO_LIST_UI

					return true;
				} break;



			case STGUCM_EXIT:
				{
					QuitGame();
					return true;
				} break;
			}
		}
	}
	

	return CX2StateDungeonGame::UICustomEventProc(  hWnd, uMsg, wParam, lParam );

}

bool CX2StateTrainingGame::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	switch( wParam )
	{
	case EGS_LEAVE_ROOM_ACK:
		{
			return Handler_EGS_LEAVE_ROOM_ACK( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_PLAY_TIME_OUT_NOT:
		{
			return Handler_EGS_PLAY_TIME_OUT_NOT( hWnd, uMsg, wParam, lParam );

		} break; 

	case EGS_END_TC_GAME_ACK:
		{
			return Handler_EGS_END_TC_GAME_ACK( hWnd, uMsg, wParam, lParam );
			
		} break;

	case EGS_REMAINING_PLAY_TIME_NOT:
		{
			return Handler_EGS_REMAINING_PLAY_TIME_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_SET_TC_REMAINING_TIME_ACK:
		{
			return Handler_EGS_SET_TC_REMAINING_TIME_ACK( hWnd, uMsg, wParam, lParam ); 

		} break;
	}

	if( CX2StateDungeonGame::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;		

	return false;
}

bool CX2StateTrainingGame::QuitGame()
{
	if ( g_pX2Game != NULL &&
		 g_pX2Game->GetCanUseEscFlag() == true )
	{	
		OpenExitDLG( GET_STRING( STR_ID_562 ) );
		m_pCursor->SetShow(true);
	}

	return true;
}


bool CX2StateTrainingGame::Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pX2Game->Handler_EGS_LEAVE_ROOM_ACK( kEvent ) == true )
	{
		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_739 ) );
		
#ifndef HEAP_BROKEN_BY_ROOM
		g_pData->DeleteDungeonRoom();
#endif // HEAP_BROKEN_BY_ROOM


		if(g_pData->GetStateArg() == 1)
		{			
			g_pData->SetStateArg(0);
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_TRAINING_SCHOOL, CX2LocationManager::HI_LOW_TRAINNING_ZONE, false );
		}
		else
		{
			g_pData->SetStateArg(0);
			Handler_EGS_STATE_CHANGE_FIELD_REQ();
		}		
	}

	return true;
}


bool CX2StateTrainingGame::Handler_EGS_PLAY_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 타임아웃 됐을때 user가 살아있으면 승리인 조건도 있을 수 있다
	if( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);

		if( NULL != pDungeonGame &&
			NULL != pDungeonGame->GetDungeon() &&
			NULL != pDungeonGame->GetDungeon()->GetNowStage() &&
			NULL != pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage() )
		{
			pDungeonGame->EnableAllNPCEventProcess( false );
			pDungeonGame->EnableAllNPCAI( false );

			if( pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetEndGameDelay() != 0.f )
			{
				// substage가 이미 클리어 되어서 결과창으로 넘어가길 기다리는 중이다
				return true;
			}

			bool bWinWhenTimeOut = pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetWinWhenTimeOut();
			if( true == bWinWhenTimeOut )
			{
				return Handler_EGS_END_TC_GAME_REQ( true );
			}
		}
	}

	return Handler_EGS_END_TC_GAME_REQ( false );
}



bool CX2StateTrainingGame::Handler_EGS_REMAINING_PLAY_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	float kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pX2Game->SetAllowFrameCount( (int)(85 * 5.3) );

	if( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if( NULL != pDungeonGame )
			pDungeonGame->SetRemainPlayTime( (float) kEvent );
	}

	return true;
}


bool CX2StateTrainingGame::Handler_EGS_END_TC_GAME_REQ( bool bSuccess )
{
	if ( m_bSend_EGS_END_TC_GAME_REQ == true )
		return true;

	KEGS_END_TC_GAME_REQ kPacket;
	kPacket.m_bIsSuccess = bSuccess;


	g_pData->GetServerProtocol()->SendPacket( EGS_END_TC_GAME_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_END_TC_GAME_ACK );

	m_bSend_EGS_END_TC_GAME_REQ = true;

	return true;
}

bool CX2StateTrainingGame::Handler_EGS_END_TC_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_END_TC_GAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	
	if( g_pMain->DeleteServerPacket( EGS_END_TC_GAME_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->SetTrainingSuccess( kEvent.m_bIsSuccess );

			if( true == kEvent.m_bIsSuccess )
			{
				int iEDDifference = kEvent.m_kUnitInfo.m_iED - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
				int iEXPDifference = kEvent.m_kUnitInfo.m_iEXP - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;

				g_pMain->SetTrainingRewardED( iEDDifference );
				g_pMain->SetTrainingRewardEXP( iEXPDifference );
					
				g_pData->GetMyUser()->GetSelectUnit()->Reset( kEvent.m_kUnitInfo );
			}
			else
			{
				g_pMain->SetTrainingRewardED( 0 );
				g_pMain->SetTrainingRewardEXP( 0 );
			}

			
			m_fRemainTimeToGameEnd = 5.f;		// 5초후에 결과화면으로 넘어간다

			if( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
			{
				CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
				if( NULL != pDungeonGame )
				{
					pDungeonGame->SetGameEnd( true );

					CX2GameUnit* pMyUserUnit = pDungeonGame->GetMyUnit();

					if( NULL != pMyUserUnit )
					{
						if( true == kEvent.m_bIsSuccess )
						{
							pMyUserUnit->Win();
						}
						else
						{
							pMyUserUnit->Lose();
						}
					}
				}
			}

			return true;
		}
	}

	return false;
}


/*virtual*/ bool CX2StateTrainingGame::Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_END_GAME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	return Handler_EGS_END_TC_GAME_REQ( false );
}



bool CX2StateTrainingGame::Handler_EGS_SET_TC_REMAINING_TIME_REQ( bool bEnable )
{
	if( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if( NULL != pDungeonGame )
			pDungeonGame->SetEnableRemainTime( bEnable );

		KEGS_SET_TC_REMAINING_TIME_REQ kPacket;
		kPacket.m_bEnable = bEnable;

		g_pData->GetServerProtocol()->SendPacket( EGS_SET_TC_REMAINING_TIME_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_SET_TC_REMAINING_TIME_ACK );
	}

	return true;
}

bool CX2StateTrainingGame::Handler_EGS_SET_TC_REMAINING_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SET_TC_REMAINING_TIME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_SET_TC_REMAINING_TIME_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}



#ifdef OLD_COMBO_LIST_UI
//{{AFX
void CX2StateTrainingGame::OpenComboListWindow( bool bOpen )
{
	if( true == bOpen )
	{
		if( NULL == m_pDLGComboList )
		{
			m_pDLGComboList = new CKTDGUIDialog( this, L"DLG_Free_Training_Combo_List.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGComboList );

			CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGComboList->GetControl( L"Static_Combo_List" );
			if( NULL != pStatic )
			{
				if( NULL != g_pX2Game &&
					NULL != g_pX2Game->GetMyUnit() &&
					NULL != g_pX2Game->GetMyUnit()->GetUnit() )
				{
					CX2Unit::UNIT_TYPE eUnitType = g_pX2Game->GetMyUnit()->GetUnit()->GetType();
					CX2Unit::UNIT_CLASS eUnitClass = g_pX2Game->GetMyUnit()->GetUnit()->GetClass();
					CreateComboListUI( pStatic, eUnitType, eUnitClass );
				}
			}
		}

		m_pDLGComboList->SetShowEnable( true, true );

	}
	else
	{
		if( NULL != m_pDLGComboList )
		{
			m_pDLGComboList->SetShowEnable( false, false );
		}
	}
	
}
//}}AFX

#endif OLD_COMBO_LIST_UI

void CX2StateTrainingGame::ResetUnit()
{
	if( NULL == g_pX2Game )
		return;


	CX2GUUser* pGUUser = g_pX2Game->GetMyUnit();
	if( NULL != pGUUser )
	{
		pGUUser->InitPosition( true, 0 );
		pGUUser->StateChange( pGUUser->GetWaitStateID(), true );
	}

	if( CX2Game::GT_DUNGEON != g_pX2Game->GetGameType() )
		return;

	CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	if( NULL != pDungeonGame->GetTrainingGameUI() &&
		pDungeonGame->GetTrainingGameUI()->GetFTNPCID() == CX2UnitManager::NUI_MUSHROOM_WOODEN )
	{
		int iCount = 1;
		for( int i=0; i < g_pX2Game->GetNPCUnitListSize(); i++ )
		{
			CX2GUNPC* pGUNPC = g_pX2Game->GetNPCUnit( i );
			if( NULL == pGUNPC )
				continue;

			// 위치초기화시 소환몹은 제외한다.
			if( pGUNPC != NULL && pGUNPC->GetNPCAI()->GetAIType() == CX2NPCAI::NAT_ALLY )
				continue;

			pGUNPC->InitPosition( false, iCount );
			pGUNPC->StateChange( pGUNPC->GetWaitStateID(), true );
			iCount += 1;
		}
	}
	else
	{
		pDungeonGame->KillNPC( pDungeonGame->GetTrainingGameUI()->GetFTNPCID(), 100 );
	}

	g_pX2Game->PauseNPCAI( 5.f );
}







#ifdef OLD_COMBO_LIST_UI

//{{AFX
void CX2StateTrainingGame::CreateComboListUI( CKTDGUIStatic* pStatic, CX2Unit::UNIT_TYPE eUnitType, CX2Unit::UNIT_CLASS eUnitClass )
{
	if( NULL == pStatic )
		return;

	D3DXVECTOR2 vPos(0, 0);
	D3DXVECTOR2 vSize( 36.f, 36.f );
	D3DXVECTOR2 vGap( 4.f, 8.f );


	switch( eUnitType )
	{
	case CX2Unit::UT_ELSWORD:
		{
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_Z4, CX2TrainingGameUI::CP_Z, vPos, vSize );
			
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_Z4a, CX2TrainingGameUI::CP_X, vPos, vSize );
			
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_X3, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_COMBO_X3a, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );


		} break;

	case CX2Unit::UT_ARME:
		{

			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_Z4up, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X3, CX2TrainingGameUI::CP_X, vPos, vSize );

			//vPos.x = 0.f;
			//vPos.y += vSize.y + vGap.y;


			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X3down, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X3front, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );




			//vPos.x = 0.f;
			//vPos.y += vSize.y + vGap.y;

			//vSize.x *= 2.f;			
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//vSize.x *= 0.5f;
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2a, CX2TrainingGameUI::CP_X, vPos, vSize );

			//vPos.x = 0.f;
			//vPos.y += vSize.y + vGap.y;

			//vSize.x *= 2.f;			
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//vSize.x *= 0.5f;
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUUser::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUUser::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );



			//vPos.x = 0.f;
			//vPos.y += vSize.y + vGap.y;

			//vSize.x *= 2.f;			
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//vSize.x *= 0.5f;
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );


		} break;

	case CX2Unit::UT_LIRE:
		{
			vSize *= 0.8f;
			vGap *= 0.8f;

			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z4, CX2TrainingGameUI::CP_Z, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z4UP, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;





			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_Z4a, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X3, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X4, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;



			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X3, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X4UP, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X3, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBO_X4DOWN, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;



			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			AddComboPicture( pStatic, (int)CX2GUUser::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUUser::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUUser::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DOUBLE_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DOUBLE_JUMP_ATTACK_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_JUMP_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_JUMP_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_JUMP_COMBO_X3, CX2TrainingGameUI::CP_X, vPos, vSize );


		} break;

	case CX2Unit::UT_RAVEN:
		{
			
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZZX, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;



			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZXX, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;



			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_XX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_XXX, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;






			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			
			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_JUMP_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_JUMP_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );

		} break;


	case CX2Unit::UT_EVE:
		{

			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZX, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XXX, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;



			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XXfrontX, CX2TrainingGameUI::CP_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XXfrontX, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XXdownX, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XXdownX, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

		} break;
	}


	switch( eUnitClass )
	{
	case CX2Unit::UC_ARME_VIOLET_MAGE:
		{

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2a, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;


			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X3down, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );

		} break;

	case CX2Unit::UC_ELSWORD_SWORDMAN:
		{


		} break;

	case CX2Unit::UC_LIRE_ELVEN_RANGER:
		{


		} break;

	case CX2Unit::UC_RAVEN_FIGHTER:
		{
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

		} break;

	case CX2Unit::UC_EVE_NASOD:
		{
			// ENSI_COMBO_Z ~ ZZZ_DOWN_ZZ
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZdownZ, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZdownZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			// ENSI_DASH_COMBO_Z
			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			// ENSI_DASH_COMBO_X
			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;
		}
		break;
	}


	// note!! 전직 추가되면 수정
	// 1차 전직 스킬 추가
	switch( eUnitClass )
	{
	case CX2Unit::UC_ELSWORD_KNIGHT:
		{
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3a, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X3, CX2TrainingGameUI::CP_X, vPos, vSize );



		} break;

	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
		{
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3a, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X2a, CX2TrainingGameUI::CP_Z, vPos, vSize );

		} break;

	case CX2Unit::UC_LIRE_COMBAT_RANGER:
		{

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUUser::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DOUBLE_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DOUBLE_JUMP_ATTACK_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP, CX2TrainingGameUI::CP_Z, vPos, vSize );



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3, CX2TrainingGameUI::CP_Z, vPos, vSize );

		} break;

	case CX2Unit::UC_LIRE_SNIPING_RANGER:
		{
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DASH_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3, CX2TrainingGameUI::CP_X, vPos, vSize );



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUUser::USI_JUMP_UP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_DOUBLE_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );

		} break;

	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
		{
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_LEFT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X3back, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X2, CX2TrainingGameUI::CP_X, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X2, CX2TrainingGameUI::CP_X, vPos, vSize );

		} break;

	case CX2Unit::UC_ARME_DARK_MAGICIAN:
		{
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_LEFT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X3back, CX2TrainingGameUI::CP_X, vPos, vSize );



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X1, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X2, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)-1, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X3down, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_COMBO_X4down, CX2TrainingGameUI::CP_Z, vPos, vSize );


			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z1, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2a, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z3up, CX2TrainingGameUI::CP_X, vPos, vSize );

		} break;

	case CX2Unit::UC_RAVEN_SOUL_TAKER:
		{

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_SOUL_TAKER_DASH_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );




			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_SOUL_TAKER_COMBO_ZZZupZ, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_SOUL_TAKER_COMBO_ZZZupZ, CX2TrainingGameUI::CP_Z, vPos, vSize );



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_XX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZZ8, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZZ8Z, CX2TrainingGameUI::CP_SUCCESSIVE_HITS, vPos, vSize );

		} break;


	case CX2Unit::UC_RAVEN_OVER_TAKER:
		{
			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vGap.x *= 0.25f;


			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_XX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;

			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownX, CX2TrainingGameUI::CP_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;

			vSize.x *= 2.f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownX, CX2TrainingGameUI::CP_X, vPos, vSize );	
			vPos.x += vSize.x + vGap.x;

			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownX, CX2TrainingGameUI::CP_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;

			vSize.x *= 2.f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownX, CX2TrainingGameUI::CP_X, vPos, vSize );	
			vPos.x += vSize.x + vGap.x;

			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownXdownX, CX2TrainingGameUI::CP_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;

			vSize.x *= 2.f;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownXdownX, CX2TrainingGameUI::CP_X, vPos, vSize );	


			vGap.x *= 4.f;



			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_COMBO_XX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_COMBO_XXfrontX, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_COMBO_XXfrontX, CX2TrainingGameUI::CP_X, vPos, vSize );




			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_DASH_JUMP_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX, CX2TrainingGameUI::CP_X, vPos, vSize );

		} break;
        
	case CX2Unit::UC_EVE_EXOTIC_GEAR:
		{
			// ENSI_DASH_JUMP_COMBO_Z -> ZX
			//vSize.x *= 2.f;			
			//AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//vSize.x *= 0.5f;
			//AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP_COMBO_ZX, CX2TrainingGameUI::CP_X, vPos, vSize );

			//vPos.x = 0.f;
			//vPos.y += vSize.y + vGap.y;

			// ENSI_COMBO_Z ~ ZZZ_DOWN_ZZ
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZdownZ, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZdownZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZdownZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			// ENSI_DASH_COMBO_Z -> Z -> Z
			vSize.x *= 2.f;			
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			// ENSI_DASH_COMBO_X -> Z
			vSize.x *= 2.f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_XZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;
		}
		break;

	case CX2Unit::UC_EVE_ARCHITECTURE:
		{
			// ENSI_COMBO_X -> X -> XXZ
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XX, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_XXZ4, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			// ENSI_COMBO_Z ~ ZZZ_DOWN_ZZ
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZdownZ, CX2TrainingGameUI::CP_DOWN, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZdownZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_COMBO_ZZZdownZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;

			// ENSI_DASH_COMBO_Z -> Z -> Z
			//vSize.x *= 2.f;			
			//AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//vSize.x *= 0.5f;
			//AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_Z, CX2TrainingGameUI::CP_Z, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_ZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );
			//vPos.x += vSize.x + vGap.x;
			//AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_COMBO_ZZZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			//vPos.x = 0.f;
			//vPos.y += vSize.y + vGap.y;

			// ENSI_DASH_JUMP_COMBO_X -> Z
			vSize.x *= 2.f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH, CX2TrainingGameUI::CP_DASH_RIGHT, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			vSize.x *= 0.5f;
			AddComboPicture( pStatic, (int)CX2GUUser::USI_DASH_JUMP, CX2TrainingGameUI::CP_UP, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP_COMBO_X, CX2TrainingGameUI::CP_X, vPos, vSize );
			vPos.x += vSize.x + vGap.x;
			AddComboPicture( pStatic, (int)CX2GUEve::ENSI_DASH_JUMP_COMBO_XZ, CX2TrainingGameUI::CP_Z, vPos, vSize );

			vPos.x = 0.f;
			vPos.y += vSize.y + vGap.y;
		}
		break;
	}



}

void CX2StateTrainingGame::AddComboPicture( CKTDGUIStatic* pStatic, int iStateID, CX2TrainingGameUI::COMMAND_PICTURE eCommandKey, D3DXVECTOR2 vPos, D3DXVECTOR2 vSize )
{
	if( NULL == pStatic )
		return;

	CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData;
	pStatic->AddPicture( pPicture );
	SetPictureTex( pPicture, eCommandKey );
	pPicture->SetSize( vSize );
	pPicture->SetPos( vPos );

	if( iStateID != -1 )
	{
		m_mapComboList[ iStateID ] = pStatic->GetPictureNum()-1;
	}
}



void CX2StateTrainingGame::SetPictureTex( CKTDGUIStatic::CPictureData* pPicture, CX2TrainingGameUI::COMMAND_PICTURE eKey )
{
	if( NULL == pPicture )
		return;


	switch( eKey )
	{
	case CX2TrainingGameUI::CP_LEFT:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_LEFT" );
		break;

	case CX2TrainingGameUI::CP_RIGHT:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_RIGHT" );
		break;

	case CX2TrainingGameUI::CP_UP:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_UP" );
		break;

	case CX2TrainingGameUI::CP_DOWN:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_DOWN" );
		break;

	case CX2TrainingGameUI::CP_Z:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_Z" );
		break;

	case CX2TrainingGameUI::CP_X:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_X" );
		break;

	case CX2TrainingGameUI::CP_C:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_C" );
		break;

	case CX2TrainingGameUI::CP_S:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_S" );
		break;

		//case CX2TrainingGameUI::CP_A:
		//	pPicture->SetTex( L"DLG_KEYS.TGA", "KEY_RIGHT" );
		//	break;

	case CX2TrainingGameUI::CP_BLANK:
		pPicture->SetTex( L"NoAlphaImage.dds" );
		break;


	case CX2TrainingGameUI::CP_0:
		pPicture->SetTex( L"HQ_RankPC_0.tga" );
		break;

	case CX2TrainingGameUI::CP_1:
		pPicture->SetTex( L"HQ_RankPC_1.tga" );
		break;

	case CX2TrainingGameUI::CP_2:
		pPicture->SetTex( L"HQ_RankPC_2.tga" );
		break;

	case CX2TrainingGameUI::CP_3:
		pPicture->SetTex( L"HQ_RankPC_3.tga" );
		break;

	case CX2TrainingGameUI::CP_4:
		pPicture->SetTex( L"HQ_RankPC_4.tga" );
		break;

	case CX2TrainingGameUI::CP_5:
		pPicture->SetTex( L"HQ_RankPC_5.tga" );
		break;

	case CX2TrainingGameUI::CP_6:
		pPicture->SetTex( L"HQ_RankPC_6.tga" );
		break;

	case CX2TrainingGameUI::CP_7:
		pPicture->SetTex( L"HQ_RankPC_7.tga" );
		break;

	case CX2TrainingGameUI::CP_8:
		pPicture->SetTex( L"HQ_RankPC_8.tga" );
		break;

	case CX2TrainingGameUI::CP_9:
		pPicture->SetTex( L"HQ_RankPC_9.tga" );
		break;

	case CX2TrainingGameUI::CP_OPEN_PARENTHESIS:
		pPicture->SetTex( L"PC_(.tga" );
		break;

	case CX2TrainingGameUI::CP_CLOSE_PARENTHESIS:
		pPicture->SetTex( L"PC_).tga" );
		break;

	case CX2TrainingGameUI::CP_SLASH:	
		pPicture->SetTex( L"PC_I.tga" );
		break;

	case CX2TrainingGameUI::CP_ED:
		pPicture->SetTex( L"DLG_Common_Texture01.tga", L"ED_COIN" );
		break;

	case CX2TrainingGameUI::CP_MINUS:
		pPicture->SetTex( L"PC_-.tga" );
		break;

	case CX2TrainingGameUI::CP_OR:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"Or" );
		break;

	case CX2TrainingGameUI::CP_PRESSED:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"Down_Condition" );
		break;

	case CX2TrainingGameUI::CP_SPECIAL_ATTACK:
		pPicture->SetTex( L"DLG_Common_Texture08.TGA", L"Hard_Death_Skill" );
		break;

	case CX2TrainingGameUI::CP_DASH_RIGHT:
		pPicture->SetTex( L"DLG_Common_Texture08.TGA", L"Dash" );
		break;

	case CX2TrainingGameUI::CP_DASH_LEFT:
		pPicture->SetTex( L"DLG_Common_Texture08.TGA", L"Dash_Left" );
		break;

	case CX2TrainingGameUI::CP_SIEGE:
		pPicture->SetTex( L"DLG_Common_Texture08.TGA", L"S" );
		break;

	case CX2TrainingGameUI::CP_BURNING_RUSH:
		pPicture->SetTex( L"DLG_Common_Texture13.TGA", L"Burning_Rush" );
		break;

	case CX2TrainingGameUI::CP_SUCCESSIVE_HITS:
		pPicture->SetTex( L"DLG_KEYS2.TGA", L"Combo_Hit" );
		break;

	default:
		pPicture->SetTex( L"FadeInOut.dds" );
		break;
	}
}


//}}AFX



#endif OLD_COMBO_LIST_UI


void CX2StateTrainingGame::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	CX2StateDungeonGame::PopTalkBox( iUnitUID_, pWstrMsg_, coTextColor_, coOutColor_, coBackColor_ );
}