/**@file	X2DeparturePrevention.h
   @breif	이탈 유저 방지 시스템 
*/

#include "StdAfx.h"
#include "X2DeparturePrevention.h"

#ifdef DEPARTURE_PREVENTION_SYSTEM

/** @function 	: CX2DeparturePrevention
	@brief 		: 생성자
	@param		: 없음
	@return		: 없음
*/
CX2DeparturePrevention::CX2DeparturePrevention(void)
{
}

/** @function 	: ~CX2DeparturePrevention
	@brief 		: 소멸자
	@param		: 없음
	@return		: 없음
*/
CX2DeparturePrevention::~CX2DeparturePrevention(void)
{
	SAFE_DELETE_DIALOG( m_pDLGLeaveUserReward );
	SAFE_DELETE_DIALOG( m_pDLGLeaveUserResult );
	SAFE_DELETE_DIALOG( m_pMsgBoxReQuitGame );
}

/** @function 	: Clear
	@brief 		: 멤버 초기화
	@param		: 없음
	@return		: 없음
*/
void CX2DeparturePrevention::Clear()
{
	m_pDLGLeaveUserReward	= NULL;
	m_pDLGLeaveUserResult	= NULL;
	m_pMsgBoxReQuitGame		= NULL;

	m_ePrevState			= SDPPS_NONE;
}

/** @function 	: QuitGame
	@brief 		: 게임 종료
	@param		: 없음
	@return		: 없음
*/
void CX2DeparturePrevention::QuitGame()
{
	g_pMain->SetQuitType( NetError::ERR_CLIENT_QUIT_00 );
	g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_ESC_GAME );
	g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
}

/** @function 	: SetPrevState
	@brief 		: 이탈 유저 확인 전 요청 상태 값 저장
	@param		: STATE_DEPARTURE_PREVENTION_PREV_STATE 상태
	@return		: 없음
*/
void CX2DeparturePrevention::SetPrevState( IN STATE_DEPARTURE_PREVENTION_PREV_STATE eVal_ )
{
	m_ePrevState = eVal_;
}

/** @function 	: UICustomEventProc
	@brief 		: UI 이벤트
	@param		: procedure
	@return		: bool
*/
bool CX2DeparturePrevention::UICustomEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	switch ( wParam )
	{
	case SDPRUCM_DEPARTURE_PREVENTION_REWARD_EXIT:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeaveUserReward, NULL, false );

			m_pMsgBoxReQuitGame = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_13688 ), SDPRUCM_DEPARTURE_PREVENTION_NOREWARD_EXIT, g_pMain->GetNowState() );
		} break;

	case SDPRUCM_DEPARTURE_PREVENTION_NOREWARD_EXIT:
		{
			static_cast<CX2State*>( g_pMain->GetNowState() )->Send_EGS_UPDATE_PLAY_STATUS_NOT();

			if ( NULL != g_pX2Game && CX2Game::GT_BATTLE_FIELD == g_pX2Game->GetGameType() )
				static_cast<CX2BattleFieldGame*>(g_pX2Game)->Send_EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT();

			switch ( m_ePrevState )
			{
			case SDPPS_EXIT:
				{
					QuitGame();
				} break;

			case SDPPS_CHARACTER_SELECT:
				{
					CX2StateAutoChanger::TARGET_DETAIL targetDetail;
					g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)
						CX2Main::XS_SERVER_SELECT, targetDetail );
				} break;

			case SDPPS_SERVER_SELECT:
				{
					CX2StateAutoChanger::TARGET_DETAIL targetDetail;
					g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)
						CX2Main::XS_SERVER_SELECT, targetDetail );
				} break;

			default:
				return false;
			}
		} break;

	case SDPRUCM_DEPARTURE_PREVENTION_REWARD_ONE:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeaveUserReward, NULL, false );

			return Handler_EGS_RETAINING_SELECT_REWARD_REQ( 1 );
		} break;

	case SDPRUCM_DEPARTURE_PREVENTION_REWARD_TWO:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeaveUserReward, NULL, false );

			return Handler_EGS_RETAINING_SELECT_REWARD_REQ( 2 );
		} break;

	case SDPRUCM_DEPARTURE_PREVENTION_REWARD_THREE:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeaveUserReward, NULL, false );

			return Handler_EGS_RETAINING_SELECT_REWARD_REQ( 3 );
		} break;

	case SDPRUCM_DEPARTURE_PREVENTION_REWARD_FOUR:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeaveUserReward, NULL, false );

			return Handler_EGS_RETAINING_SELECT_REWARD_REQ( 4 );
		} break;

	case SDPRUCM_DEPARTURE_PREVENTION_REWARD_FIVE:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeaveUserReward, NULL, false );

			return Handler_EGS_RETAINING_SELECT_REWARD_REQ( 5 );
		} break;

	case SDPRUCM_DEPARTURE_PREVENTION_RESULT_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeaveUserResult, NULL, false );
		} break;
	}

	return false;
}

/** @function 	: UICustomEventProc
	@brief 		: 서버 이벤트
	@param		: procedure
	@return		: bool
*/
bool CX2DeparturePrevention::UIServerEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	KTDXPROFILE();
	switch( wParam )
	{
	case EGS_OUT_USER_RETAINING_ACK:
		{
			return Handler_EGS_OUT_USER_RETAINING_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_RETAINING_SELECT_REWARD_ACK:
		{
			return Handler_EGS_RETAINING_SELECT_REWARD_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	default:
		break;
	}
	return false;
}

/** @function 	: OnFrameMove
	@return		: 프레임 당 처리
*/
HRESULT CX2DeparturePrevention::OnFrameMove( IN double fTime, IN float fElapsedTime )
{
	return S_OK;
}


/** @function 	: Handler_EGS_OUT_USER_RETAINING_REQ
	@brief 		: 종료 패킷 전에 이탈 방지 시스템 확인
	@return		: bool
*/
bool CX2DeparturePrevention::Handler_EGS_OUT_USER_RETAINING_REQ()
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
		QuitGame();

		return false;
	}	

	KEGS_OUT_USER_RETAINING_REQ kPacket;
	kPacket.m_iUnitUID = g_pData->GetMyUser()->GetUID();

	g_pData->GetServerProtocol()->SendPacket( EGS_OUT_USER_RETAINING_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_OUT_USER_RETAINING_ACK );

	return true;
}

/** @function 	: Handler_EGS_OUT_USER_RETAINING_ACK
	@brief 		: 이탈 유저 확인 true, false
	@param		: procedure
	@return		: bool
*/
bool CX2DeparturePrevention::Handler_EGS_OUT_USER_RETAINING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_OUT_USER_RETAINING_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pMain && g_pMain->DeleteServerPacket( EGS_OUT_USER_RETAINING_ACK ) == true )
	{
		if( true == kEvent.m_bDepartureUser )
		{
			if ( NULL == g_pKTDXApp || NULL == g_pMain )
				return false;

			// 이탈 유저 보상 관련 대화상자 출력 및 아이템 받아 오기
			SAFE_DELETE_DIALOG( m_pDLGLeaveUserReward );
			m_pDLGLeaveUserReward = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Departure_Reward.lua" );

			if ( NULL != m_pDLGLeaveUserReward )
			{
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGLeaveUserReward );
				UpdateRewardUI( kEvent );
			}
			else
			{
				return false;
			}
		}
		else		// 이탈 유저가 아니면 종료
		{
			static_cast<CX2State*>( g_pMain->GetNowState() )->Send_EGS_UPDATE_PLAY_STATUS_NOT();

			if ( NULL != g_pX2Game && CX2Game::GT_BATTLE_FIELD == g_pX2Game->GetGameType() )
				static_cast<CX2BattleFieldGame*>(g_pX2Game)->Send_EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT();
			
			switch ( m_ePrevState )
			{
			case SDPPS_EXIT:
				{
					QuitGame();
				} break;

			case SDPPS_CHARACTER_SELECT:
				{
					CX2StateAutoChanger::TARGET_DETAIL targetDetail;
					g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)
						CX2Main::XS_SERVER_SELECT, targetDetail );
				} break;

			case SDPPS_SERVER_SELECT:
				{
					CX2StateAutoChanger::TARGET_DETAIL targetDetail;
					g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)
						CX2Main::XS_SERVER_SELECT, targetDetail );
				} break;

			default:
				return false;
			}
		}
	}

	return false;
}

/** @function 	: UpdateRewardUI
	@brief 		: 보상 아이템 갱신
	@param		: 패킷
	@return		: 없음
*/
void CX2DeparturePrevention::UpdateRewardUI( IN KEGS_OUT_USER_RETAINING_ACK &kEvent_ )
{
	if ( NULL == m_pDLGLeaveUserReward )
		return;

	wstring itemName = L"";

	if ( false == kEvent_.m_mapSlotItem.empty() )
	{
		std::map< int, KRewardItem >::iterator itMap;
		for ( itMap = kEvent_.m_mapSlotItem.begin(); itMap != kEvent_.m_mapSlotItem.end(); ++itMap )
		{
            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itMap->second.m_iItemID );
			if ( pItemTemplet != NULL )
			{
				wstring slotName = L"Item_Slot" + g_pMain->GetEDString( itMap->first );
								
				CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGLeaveUserReward->GetControl( slotName.c_str() );
				if ( pStatic != NULL && pStatic->GetPicture(1) != NULL )
				{
                    itemName = g_pMain->GetStrByLineBreakColor( pItemTemplet->GetFullName_(), 80, 1 );
					pStatic->GetString(0)->msg = itemName.c_str();
					pStatic->GetString(1)->msg = g_pMain->GetEDString( itMap->second.m_iQuantity );

                    const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
					if ( pwszShopImage[0] != NULL )
						pStatic->GetPicture(1)->SetTex( pwszShopImage );
					else
						pStatic->GetPicture(1)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
				}
			}
		}
	}
}

/** @function 	: Handler_EGS_RETAINING_SELECT_REWARD_REQ
	@brief 		: 보상 완료
	@param		: 없음
	@return		: bool
*/
bool CX2DeparturePrevention::Handler_EGS_RETAINING_SELECT_REWARD_REQ( IN int iVal_ )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
		return false; 
	}	

	KEGS_RETAINING_SELECT_REWARD_REQ kPacket;
	kPacket.m_iSlotID = iVal_;

	g_pData->GetServerProtocol()->SendPacket( EGS_RETAINING_SELECT_REWARD_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RETAINING_SELECT_REWARD_ACK );

	return true;
}

/** @function 	: Handler_EGS_RETAINING_SELECT_REWARD_ACK
	@brief 		: 
	@param		: procedure
	@return		: bool
*/
bool CX2DeparturePrevention::Handler_EGS_RETAINING_SELECT_REWARD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RETAINING_SELECT_REWARD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pMain && g_pMain->DeleteServerPacket( EGS_RETAINING_SELECT_REWARD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEvent.m_kRewardItem.m_iItemID );
			if ( NULL != pItemTemplet )
			{
				// 이탈 유저 보상 확인
				SAFE_DELETE_DIALOG( m_pDLGLeaveUserResult );
				m_pDLGLeaveUserResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Departure_Result.lua" );

				if ( NULL != m_pDLGLeaveUserResult )
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGLeaveUserResult );

					CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGLeaveUserResult->GetControl( L"TalkBox" );

                    wstring rewardDesc = std::wstring(pItemTemplet->GetFullName_())
                        + GET_REPLACED_STRING( ( STR_ID_13691, "h", kEvent.m_kRewardItem.m_iQuantity ) );
					
					pStatic->GetString(0)->msg = rewardDesc.c_str();
					
					if ( 127030 ==  kEvent.m_kRewardItem.m_iItemID )
					{
						g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( kEvent.m_iResurrectionStone );
						g_pData->GetUIManager()->GetUICharInfo()->ResetResurrectionStoneUI();
					}
					else
					{
						if ( true == kEvent.m_bIsPost )
							pStatic->GetString(1)->msg = GET_STRING( STR_ID_13693 );
						else
							pStatic->GetString(1)->msg = GET_STRING( STR_ID_13692 );						
					}
				}

				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

				if(g_pData->GetUIManager()->GetUIInventory() != NULL)
				{		
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
				}
			}

			return true;
		}
	}

	return false;
}

#endif