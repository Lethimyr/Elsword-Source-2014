#include "StdAfx.h"
#include ".\x2stateclasschange.h"



//
//CX2StateClassChange::CX2StateClassChange(void) :
//m_pUnitViewerUI( NULL ),
//m_pDLGClassChange( NULL )
//{
//
//	// note!! 전직 추가되면 수정
//	switch(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass)
//	{
//	case CX2Unit::UC_ELSWORD_KNIGHT:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_Knight.lua" );
//		} break;
//	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_Magic_Knight.lua" );
//		} break;
//	case CX2Unit::UC_LIRE_COMBAT_RANGER:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_Combat_Ranger.lua" );
//		} break;
//	case CX2Unit::UC_LIRE_SNIPING_RANGER:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_Sniping_Ranger.lua" );
//		} break;
//	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_High_Magician.lua" );
//		} break;
//	case CX2Unit::UC_ARME_DARK_MAGICIAN:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_Dark_Magician.lua" );
//		} break;
//	case CX2Unit::UC_RAVEN_SOUL_TAKER:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_SwordTaker.lua" );
//		} break;
//	case CX2Unit::UC_RAVEN_OVER_TAKER:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_OverTaker.lua" );
//		} break;
//	case CX2Unit::UC_EVE_EXOTIC_GEAR:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_CodeExotic.lua" );
//		} break;
//	case CX2Unit::UC_EVE_ARCHITECTURE:
//		{
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_CodeArchitecture.lua" );
//		} break;
//	default:
//		{
//			ASSERT( !"ClassChange State error : Unexpected UnitClass" );
//			m_pDLGClassChange = new CKTDGUIDialog( this, L"DLG_Class_Change_Knight.lua" );
//		} break;
//
//	}
//	
//#ifdef _NEXON_KR_
//	if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )	// 체험 아이디 제한	
//	{
//		CNMCOClientObject::GetInstance().ChangeMyLevel( 
//			( (UINT32)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass << 24 ) | (UINT32)g_pData->GetSelectUnitLevel(), kUserFlag_ClassChanged );
//	}
//#endif
//
//
//	if ( m_pDLGClassChange != NULL )
//	{
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGClassChange );
//	}
//	
//	m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
//	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
//	m_pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
//	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );
//
//	m_pUnitViewerUI->SetUnit( g_pData->GetMyUser()->GetSelectUnit() );
//	m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250, -130, 200 ) );
//	m_pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.5f, 1.5f, 1.5f ) );
//	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
//	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
//	m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
//
//	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
//
//	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
//	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
//		g_pKTDXApp->GetDGManager()->GetFar(), false );
//
//	// 마을을 벗어났으므로 패킷을 날린다.
//	g_pData->GetServerProtocol()->SendID( EGS_LEAVE_FIELD_NOT );
//
//	g_pKTDXApp->SkipFrame();
//
//}
//
//CX2StateClassChange::~CX2StateClassChange(void)
//{
//	SAFE_DELETE_DIALOG( m_pDLGClassChange );
//	SAFE_DELETE_KTDGOBJECT( m_pUnitViewerUI );
//}
//
//HRESULT CX2StateClassChange::OnFrameMove( double fTime, float fElapsedTime )
//{
//	CX2StateCommonBG::OnFrameMove( fTime, fElapsedTime );
//
//	if ( m_pUnitViewerUI != NULL )
//	{
//		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );
//	}
//
//	return S_OK;
//}
//
//bool CX2StateClassChange::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	if( CX2StateCommonBG::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
//	{
//		return true;
//	}
//
//	switch( wParam )
//	{
//	case SCCUCM_STATE_CHANGE_VILLAGE:
//		{
//			Handler_EGS_STATE_CHANGE_FIELD_REQ();
//			return true;
//		}
//		break;
//	}
//
//	return false;
//}
//
//
//bool CX2StateClassChange::LastMsgByESC()
//{
//	OpenLastMsgPopUp( GET_STRING( STR_ID_554 ) );
//	return true;
//}
//
//void CX2StateClassChange::LastMsgPopOk()
//{
//	Handler_EGS_STATE_CHANGE_FIELD_REQ();
//}
//
//

//////////////////////////////////////////////////////////////////////////
CX2ClassChangePopup::CX2ClassChangePopup( void )
: m_bShow( false )
, m_pUnitViewerUI( NULL )
, m_pDLGClassChange( NULL )
{
}

CX2ClassChangePopup::~CX2ClassChangePopup( void )
{
	SAFE_DELETE_DIALOG( m_pDLGClassChange );
	SAFE_DELETE_KTDGOBJECT( m_pUnitViewerUI );
}

HRESULT CX2ClassChangePopup::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_pUnitViewerUI != NULL )
	{
		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );
	}


	return S_OK;
}



bool CX2ClassChangePopup::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case CCUCM_STATE_CHANGE_VILLAGE:
		{
			SetShow( false );		
			switch ( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_BATTLE_FIELD:
				{
					CX2State* pState = static_cast<CX2State*>(g_pMain->GetNowState());
					if( NULL != pState )
					{
						pState->Handler_EGS_STATE_CHANGE_FIELD_REQ(g_pData->GetBattleFieldManager().GetReturnVillageId());

#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT	//김창한
						//필드에서 전직시 아래 조건들을 true로 돌려주지 않아 이펙트가 제대로 나오지 않는 문제 수정.
						if( NULL != g_pX2Game )
						{
							if( NULL != g_pX2Game->GetMajorParticle() )
								g_pX2Game->GetMajorParticle()->SetRenderEffectMadeByGameUnit( true );

							if( NULL != g_pX2Game->GetMinorParticle() )
								g_pX2Game->GetMinorParticle()->SetRenderEffectMadeByGameUnit( true );

							if( NULL != g_pX2Game->GetMajorXMeshPlayer() )
								g_pX2Game->GetMajorXMeshPlayer()->SetRenderEffectMadeByGameUnit( true );

							if( NULL != g_pX2Game->GetMinorXMeshPlayer() )
								g_pX2Game->GetMinorXMeshPlayer()->SetRenderEffectMadeByGameUnit( true );
						}
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
					}
				} break;
			default:
				{			
					CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>(g_pMain->GetNowState());
					if( NULL != pStateMenu )
					{
						if ( NULL != g_pData->GetCashShop() &&
							true == g_pData->GetCashShop()->GetOpen() )
						{
							pStateMenu->ToggleCashShop();
						}
					}
				} break;
			
			}	
			return true;
		}
		break;
	}


	return false;
}

#ifdef SERV_SHARING_BANK_QUEST_CASH
bool CX2ClassChangePopup::SetShow( bool bShow, int iType )
{
	m_bShow = bShow;

	if( m_pDLGClassChange != NULL )
	{
		m_pDLGClassChange->SetHasUnit( NULL );

		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGClassChange, NULL, false );
		m_pDLGClassChange = NULL;
	}

	if( NULL != m_pUnitViewerUI )
	{
		SAFE_DELETE_KTDGOBJECT( m_pUnitViewerUI );
	}

	if( true == bShow )
	{
		switch(iType)
		{
		case HPT_SHARE_BANK:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Share_Bank_Tutorial.lua" );
			}
			break;
		default:
			// 없는 타입이네;
			return false;
		}

		if ( m_pDLGClassChange != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGClassChange );
		}
	}
	return true;
}
#endif // SERV_SHARING_BANK_QUEST_CASH

void CX2ClassChangePopup::SetShow( bool bShow )
{
	m_bShow = bShow;


	if( m_pDLGClassChange != NULL )
	{
		m_pDLGClassChange->SetHasUnit( NULL );

		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGClassChange, NULL, false );
		m_pDLGClassChange = NULL;
	}

	if( NULL != m_pUnitViewerUI )
	{
		SAFE_DELETE_KTDGOBJECT( m_pUnitViewerUI );
	}

	if( true == bShow )
	{

#ifdef _NEXON_KR_
		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )	// 체험 아이디 제한	
		{
			CNMCOClientObject::GetInstance().ChangeMyLevel( 
				( (UINT32)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass << 24 ) | (UINT32)g_pData->GetSelectUnitLevel(), kUserFlag_ClassChanged );
		}
#endif

		// note!! 전직 추가되면 수정
		switch(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass)
		{
		case CX2Unit::UC_ELSWORD_KNIGHT:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Knight.lua" );
			} break;
		case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Magic_Knight.lua" );
			} break;
#ifdef ELSWORD_SHEATH_KNIGHT
		case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_SheathKnight.lua" );
			} break;
#endif ELSWORD_SHEATH_KNIGHT
		case CX2Unit::UC_LIRE_COMBAT_RANGER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Combat_Ranger.lua" );
			} break;
		case CX2Unit::UC_LIRE_SNIPING_RANGER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Sniping_Ranger.lua" );
			} break;
#ifdef SERV_TRAPPING_RANGER_TEST
		case CX2Unit::UC_LIRE_TRAPPING_RANGER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_TrappingRanger.lua" );
			} break;
#endif SERV_TRAPPING_RANGER_TEST
		case CX2Unit::UC_ARME_HIGH_MAGICIAN:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_High_Magician.lua" );
			} break;
		case CX2Unit::UC_ARME_DARK_MAGICIAN:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Dark_Magician.lua" );
			} break;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Battle_Magician.lua" );
			} break;
#endif
		case CX2Unit::UC_RAVEN_SOUL_TAKER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_SwordTaker.lua" );
			} break;
		case CX2Unit::UC_RAVEN_OVER_TAKER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_OverTaker.lua" );
			} break;
#ifdef RAVEN_WEAPON_TAKER
		case CX2Unit::UC_RAVEN_WEAPON_TAKER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_WeaponTaker.lua" );
			} break;
#endif RAVEN_WEAPON_TAKER
		case CX2Unit::UC_EVE_EXOTIC_GEAR:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_CodeExotic.lua" );
			} break;
		case CX2Unit::UC_EVE_ARCHITECTURE:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_CodeArchitecture.lua" );
			} break;
#ifdef EVE_ELECTRA
		case CX2Unit::UC_EVE_ELECTRA:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Electra.lua" );
			} break;
#endif EVE_ELECTRA

		case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Lord_Knight.lua" );
			} break;
		case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Rune_Slayer.lua" );
			} break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
		case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Elsword_IS.lua" );
			} break;
#endif

		case CX2Unit::UC_LIRE_WIND_SNEAKER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Rena_WS.lua" );
			} break;
		case CX2Unit::UC_LIRE_GRAND_ARCHER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Rena_GA.lua" );
			} break;
#ifdef SERV_RENA_NIGHT_WATCHER
		case CX2Unit::UC_LIRE_NIGHT_WATCHER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Night_Watcher.lua" );
			} break;
#endif



		case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Aisha_EM.lua" );
			} break;
		case CX2Unit::UC_ARME_VOID_PRINCESS:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Aisha_VP.lua" );
			} break;
#ifdef SERV_ARME_DIMENSION_WITCH
		case CX2Unit::UC_ARME_DIMENSION_WITCH:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Aisha_DW.lua" );
			} break;
#endif

			// 전직 팝업창에 캐릭터만 보이고 UI가 나오지 않는다면 UTF-8 포맷이 맞는지 확인을 먼저 해봅니다.

#ifdef RAVEN_SECOND_CLASS_CHANGE
		case CX2Unit::UC_RAVEN_BLADE_MASTER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Raven_BM.lua" );
			} break;
		case CX2Unit::UC_RAVEN_RECKLESS_FIST:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Raven_RF.lua" );
			} break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
		case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_VeteranCommander.lua" );
			} break;
#endif

		//{{ kimhc // 2010.1.22 //
#ifdef EVE_SECOND_CLASS_CHANGE
		case CX2Unit::UC_EVE_CODE_NEMESIS:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Eve_CN.lua" );
			} break;
		case CX2Unit::UC_EVE_CODE_EMPRESS:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Eve_CE.lua" );
			} break;
#endif EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.1.22 //
#ifdef SERV_EVE_BATTLE_SERAPH
		case CX2Unit::UC_EVE_BATTLE_SERAPH:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Battle_Seraph.lua" );
			} break;
#endif

		//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE //chung임시
	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Chung_FG.lua" );
			} break;
		case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Chung_SG.lua" );
			} break;
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.3 // 청 1차 전직

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Shelling_Guardian.lua" );
			} break;
#endif

			//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
		case CX2Unit::UC_CHUNG_IRON_PALADIN:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Chung_IP.lua" );
			} break;
		case CX2Unit::UC_CHUNG_DEADLY_CHASER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Chung_DC.lua" );
			} break;
#endif
				//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
		case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Chung_TT.lua" );
			} break;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
		case CX2Unit::UC_ARA_LITTLE_HSIEN:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Ara_LH.lua" );
			} break;
		case CX2Unit::UC_ARA_SAKRA_DEVANAM:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Ara_SD.lua" );
			} break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef NEW_CHARACTER_EL
		case CX2Unit::UC_ELESIS_SABER_KNIGHT:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Elesis_SK.lua" );
			} break;
		case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Elesis_PK.lua" );
			} break;
#endif // NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		case CX2Unit::UC_ARA_LITTLE_DEVIL:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Ara_LD.lua" );
			} break;
		case CX2Unit::UC_ARA_YAMA_RAJA:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Ara_YR.lua" );
			} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		case CX2Unit::UC_ELESIS_GRAND_MASTER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Elesis_GM.lua" );
			} break;
		case CX2Unit::UC_ELESIS_BLAZING_HEART:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Elesis_BH.lua" );
			} break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
		case CX2Unit::UC_ADD_PSYCHIC_TRACER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Add_PT.lua" );
			} break;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		case CX2Unit::UC_ADD_LUNATIC_PSYKER:
			{
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Add_LP.lua" );
			} break;
#endif //SERV_ADD_LUNATIC_PSYKER

		default:
			{
				ASSERT( !"ClassChange State error : Unexpected UnitClass" );
				m_pDLGClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Class_Change_Knight.lua" );
			} break;
		}

		if ( m_pDLGClassChange != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGClassChange );
		}


		m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
		//m_pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
		m_pUnitViewerUI->SetLightPos( -250, 100, -600 );	// 캐릭터뷰어 라이트 위치 변경
		//g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );
#ifdef FIX_CLASS_CHANGE_MESH
		m_pUnitViewerUI->SetClassChange( true );
#endif

#ifdef SEPARATION_MOTION
		m_pUnitViewerUI->SetUnit( g_pData->GetMyUser()->GetSelectUnit(), CX2UnitViewerUI::UVS_CLASSCHANGE ); 
#else
		m_pUnitViewerUI->SetUnit( g_pData->GetMyUser()->GetSelectUnit() );
#endif
		//m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250, -130, 200 ) );

		
		m_pDLGClassChange->SetHasUnit( (CKTDGObject*)m_pUnitViewerUI );
		
		m_pUnitViewerUI->SetFixed( true );
		D3DXVECTOR3 vUnitPos = m_pDLGClassChange->GetDummyPos(0);
		m_pUnitViewerUI->SetPositionOnScr( vUnitPos.x, vUnitPos.y, vUnitPos.z, 1.5f );

		m_pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.5f, 1.5f, 1.5f ) );
		m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		//m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
			

		m_pUnitViewerUI->SetShowObject( true, true );
		m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(), g_pKTDXApp->GetElapsedTime() );
		m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );


		//g_pKTDXApp->SkipFrame();

#ifdef ADD_PLAY_SOUND //김창한
		if( NULL != g_pData->GetMyUser() &&
			NULL != g_pData->GetMyUser()->GetSelectUnit() )
			PlaySoundChangeClass(g_pData->GetMyUser()->GetSelectUnit()->GetType() );
#endif //ADD_PLAY_SOUND
	}

}

#ifdef ADD_PLAY_SOUND //김창한
void CX2ClassChangePopup::PlaySoundChangeClass( CX2Unit::UNIT_TYPE eUnitType )
{
	if( g_pKTDXApp->GetDeviceManager() == NULL )
		return;

	if( eUnitType <= CX2Unit::UT_NONE || eUnitType >= CX2Unit::UT_END )
		return;

	wstring wstrSoundName = L"";
	int iRandom = -1;

	switch( eUnitType )
	{
	case CX2Unit::UT_ELSWORD: // 2
		{
			wstrSoundName = L"ElswordVoice_ClassUpFinish0";
			iRandom = RandomInt() % 2 + 1;
		} break;

	case CX2Unit::UT_ARME: // 1
		{
			wstrSoundName = L"AishaVoice_ClassUpFinish01.ogg";
		} break;

	case CX2Unit::UT_LIRE: // 1
		{
			wstrSoundName = L"LenaVoice_ClassUpFinish01.ogg";
		} break;

	case CX2Unit::UT_RAVEN: // 1
		{
			wstrSoundName = L"RavenVoice_ClassUpFinish01.ogg";
		} break;

	case CX2Unit::UT_EVE: // 1
		{
			wstrSoundName = L"EveVoice_ClassUpFinish01.ogg";
		} break;

	case CX2Unit::UT_CHUNG: // 2
		{
			wstrSoundName = L"ChungVoice_ClassUpFinish0";
			iRandom = RandomInt() % 2 + 1;
		} break;

	case CX2Unit::UT_ARA: // 2
		{
			wstrSoundName = L"AraVoice_ClassUpFinish0";
			iRandom = RandomInt() % 2 + 1;
		} break;

	case CX2Unit::UT_ELESIS: // 2
		{
			wstrSoundName = L"ElesisVoice_ClassUpFinish0";
			iRandom = RandomInt() % 2 + 1;
		} break;

	case CX2Unit::UT_ADD: // 2
		{
			wstrSoundName =L"AddVoice_ClassUpFinish0";
			iRandom = RandomInt() % 2 + 1;
		} break;

	default:
		break;
	}

	if( iRandom > -1 )
	{
		WCHAR buff[6];
		StringCchPrintf( buff, ARRAY_SIZE(buff), L"%d.ogg ", iRandom );
		wstrSoundName += std::wstring(buff);
	}

	if( wstrSoundName.compare(L"") != 0 )
		g_pKTDXApp->GetDeviceManager()->PlaySound(wstrSoundName.c_str(), false, false );
}
#endif //ADD_PLAY_SOUND