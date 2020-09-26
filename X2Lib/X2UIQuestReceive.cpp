
#include "StdAfx.h"

#ifndef SERV_EPIC_QUEST
#include ".\x2uiQuestReceive.h"

CX2UIQuestReceive::CX2UIQuestReceive( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2ItemSlotManager( pNowStage, pFileName ),
m_iQuestPerPage(0),
m_iRewardSlotNum(0),
m_bShow(false),
m_MovedPosition(0,0),						// D3DXVECTOR2
m_DLGPosition(0,0),							// D3DXVECTOR2
m_DLGSize(0,0),								// D3DXVECTOR2
m_pDLGUIQuestReceive(NULL),
m_pDLGUIQuestClear(NULL),
m_pDLGQuestRewardItemInfo(NULL),
m_pDLGMsgBox(NULL),
m_eNPCID(CX2UnitManager::NUI_NONE),
m_iTopIndex(0),
// m_vecPrevNotAssignedQuestID.clear();
// m_vecNotAssignedQuestID.clear();
// m_vecAvailableQuestID.clear();
// m_vQuestList.clear();
m_iPickedQuestID(0),
//m_mapNewQuest.clear();
m_bAdminQuestComplete(false),
m_bLastRewardWasSelection(false),
m_SumDelta(0),
//m_vecQuestDesc,
m_iQuestDescIndex(0),
m_iQuestDescLinePerPage(1),
m_vQuestDescSize(0,0),
m_byNumOfItemThatICanGet( 0 )	// kimhc // 2009-12-18 // 가질수 있는 선택 보상 아이템 갯수
{

	m_vecPrevNotAssignedQuestID.clear();		// 새로 갱신된 퀘스트 리스트가 있는지 확인하기 위해서
	m_vecNotAssignedQuestID.clear();
	m_vecAvailableQuestID.clear();
	m_vQuestList.clear();
	m_mapNewQuest.clear();					// 새로 생긴 퀘스트 목록

	//////////////////////////////////////////////////////////////////////////

	RegisterLuaBind();	

	m_pDLGUIQuestReceive = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Request.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIQuestReceive );
	m_pDLGUIQuestReceive->SetShowEnable(false, false);

	m_iQuestPerPage = m_pDLGUIQuestReceive->GetDummyInt(0);
	m_iRewardSlotNum = m_pDLGUIQuestReceive->GetDummyInt(1);

	D3DXVECTOR3 tmp;
	tmp = m_pDLGUIQuestReceive->GetDummyPos(0);
	m_DLGPosition.x = tmp.x;
	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGUIQuestReceive->GetDummyPos(1);
	m_DLGSize.x = tmp.x;
	m_DLGSize.y = tmp.y;
}


CX2UIQuestReceive::~CX2UIQuestReceive(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUIQuestReceive );	
	SAFE_DELETE_DIALOG( m_pDLGUIQuestClear );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGQuestRewardItemInfo );

	m_vQuestList.clear();
// 	for(UINT i=0; i<m_vQuestList.size(); i++)
// 	{
// 		QuestListSlot* pQuestSlot = m_vQuestList[i];
// 		SAFE_DELETE(pQuestSlot);
// 	}
}

void CX2UIQuestReceive::SetLayer( X2_DIALOG_LAYER layer )
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);

	if(m_pDLGUIQuestReceive != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIQuestReceive, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIQuestReceive, false );
	}
}

void CX2UIQuestReceive::SetPosition( D3DXVECTOR2 vec )
{
	// 슬롯 다이얼로그 유닛뷰어
	if(m_pDLGUIQuestReceive != NULL) m_pDLGUIQuestReceive->SetPos(vec);

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		D3DXVECTOR2 pos;
		pos = pItemSlot->GetPos();
		
		pos -= m_MovedPosition;
		pos += vec;

		pItemSlot->SetPos(pos);
	}
	m_MovedPosition = vec;
}

HRESULT CX2UIQuestReceive::OnFrameMove( double fTime, float fElapsedTime )
{

	if(m_pDLGUIQuestReceive != NULL && m_pDLGUIQuestClear == NULL 
		&& false == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGUIQuestReceive ) )	// 앞에 모달 다이얼로그가 없으면
	{
		if ( GET_KEY_STATE( GA_UP ) == TRUE )
		{
			SelectNextQuest(false);
		}
		else ( GET_KEY_STATE( GA_DOWN ) == TRUE )
		{
			SelectNextQuest(true);
		}
	}
	if( (m_pDLGUIQuestReceive != NULL && m_pDLGUIQuestReceive->GetIsMouseOver() == true) ||
		(m_pDLGQuestRewardItemInfo != NULL && m_pDLGQuestRewardItemInfo->GetIsMouseOver() == true) )
	{
		return CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
	}
	else
	{
		CX2ItemSlotManager::InvalidSelectedItem();
		CX2ItemSlotManager::InvalidSlotDesc();
	}

	return S_OK;
}

bool CX2UIQuestReceive::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGUIQuestReceive ) )
		return false;

	bool bFlag = false;
	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		//case WM_MOUSEWHEEL:
		{
			if ( false == m_pDLGUIQuestReceive->GetIsMouseOver())
			{
				return false;
			}
			bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDLGUIQuestReceive->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_QUEST_RECEIVE, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}


	switch( uMsg )
	{
	case WM_MOUSEMOVE:
		{

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
		}
		break;
	case WM_MOUSEWHEEL:
		{ 
			return OnMouseWheel(hWnd, uMsg, wParam,lParam);
		}break;
	}



	return bFlag;
}

bool CX2UIQuestReceive::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case UQRCM_EXIT:
		{
			SetShow(false);
		}
		return true;
	case UQRCM_QUEST_SELECT:
		{
			CKTDGUIRadioButton* pQuestButton = (CKTDGUIRadioButton*)lParam;
			int QuestIndex = m_iTopIndex + pQuestButton->GetDummyInt(0);

			if( QuestIndex < (int)m_vecAvailableQuestID.size() )
				SelectQuest( m_vecAvailableQuestID[QuestIndex] );			
		}
		return true;
	case UQRCM_QUEST_ACCEPT_CONFIRM:
		{
			if( m_pDLGMsgBox != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}

			if( m_iPickedQuestID != -1 )
			{
				Handler_EGS_NEW_QUEST_REQ( m_iPickedQuestID );
			}

			// 초보자 가이드
			if(g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL && g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true)
			{				
				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_ACCEPT);
				g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(0);
			}

			return true;

		}
		return true;
	case UQRCM_QUEST_ACCEPT_CANCLE:
		{
			if( m_pDLGMsgBox != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}
		}
		return true;
	case UQRCM_QUEST_ACCEPT:
		{
			if( m_iPickedQuestID != -1 )
			{
				const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );
				if( NULL == pQuestInst )
				{
					if( m_pDLGMsgBox != NULL )
					{
						g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
						m_pDLGMsgBox = NULL;
					}

					m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_891 ), UQRCM_QUEST_ACCEPT_CONFIRM, g_pMain->GetNowState(), UQRCM_QUEST_ACCEPT_CANCLE );
					
					if( m_iPickedQuestID == 11010 && 
						(g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP3 &&
						g_pTFieldGame->GetJoinNpcId() == CX2UnitManager::NUI_HAGERS) )
					{
						g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(6);
					}
				}
			}

		}
		return true;
	case UQRCM_QUEST_COMPLETE:
		{
			if( m_iPickedQuestID != -1 )
			{
				m_bAdminQuestComplete = false;
				const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
				if( NULL != pQuestTemplet )
				{
					bool bThereIsSelectionReward = false;

					CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
					CX2Unit::UNIT_CLASS eUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();
					for( UINT i = 0; i < pQuestTemplet->m_SelectReward.m_vecSelectItem.size(); i++)
					{
						const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_SelectReward.m_vecSelectItem[i];
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
						if( pItemTemplet == NULL )
							continue;

						else if( pItemTemplet->GetUnitType() != CX2Unit::UT_NONE )
						{
							if( pItemTemplet->GetUnitType() != eUnitType )
							{
								continue;
							}
						}

						bThereIsSelectionReward = true;
						if( true == bThereIsSelectionReward )
							break;
					}

					//g_pKTDXApp->GetDeviceManager()->PlaySound( L"CompleteQuest.ogg", false, false );
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"QuestReward.ogg", false, false );
					
					OpenQuestRewardWindow( m_iPickedQuestID, bThereIsSelectionReward );
				}
			}
			else
			{
				return true;
			}

		}
		return true;
	case UQRCM_QUEST_ADMIN_COMPLETE:
		{
			if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel() )
			{
				m_bAdminQuestComplete = true;
				if( m_iPickedQuestID != -1 )
				{
					const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
					if( NULL != pQuestTemplet )
					{
						bool bThereIsSelectionReward = false;

						CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
						CX2Unit::UNIT_CLASS eUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();
						for( UINT i = 0; i < pQuestTemplet->m_SelectReward.m_vecSelectItem.size(); i++)
						{
							const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_SelectReward.m_vecSelectItem[i];
							const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
							if( pItemTemplet == NULL )
								continue;

							else if( pItemTemplet->GetUnitType() != CX2Unit::UT_NONE )
							{
								if( pItemTemplet->GetUnitType() != eUnitType )
								{
									continue;
								}
							}

							bThereIsSelectionReward = true;
							if( true == bThereIsSelectionReward )
								break;
						}

						//g_pKTDXApp->GetDeviceManager()->PlaySound( L"CompleteQuest.ogg", false, false );
						g_pKTDXApp->GetDeviceManager()->PlaySound( L"QuestReward.ogg", false, false );

						OpenQuestRewardWindow( m_iPickedQuestID, bThereIsSelectionReward );
					}
				}
				else
				{
					return true;
				}
			}

		}
		return true;
	case UQRCM_QUEST_GIVE_UP:
		{
			if( m_iPickedQuestID != -1 )
			{
				const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );
				if( NULL != pQuestInst )
				{
					if( m_pDLGMsgBox != NULL )
					{
						g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
						m_pDLGMsgBox = NULL;
					}

					m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_888 ), UQRCM_QUEST_GIVE_UP_CONFIRM, g_pMain->GetNowState(), UQRCM_QUEST_GIVE_UP_CANCLE );
					CKTDGUIButton* pButton_Cancle = (CKTDGUIButton*) m_pDLGMsgBox->GetControl( L"MsgBoxOkAndCancelCancelButton" );
					pButton_Cancle->RequestFocus();


				}
			}

			return true;

		}
		return true;
	case UQRCM_QUEST_GIVE_UP_CONFIRM:
		{
			if( m_pDLGMsgBox != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}

			if( m_iPickedQuestID != -1 )
			{
				const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );
				if( NULL != pQuestInst )
				{
					// 초심자 가이드
					if( m_iPickedQuestID == 11010 && 
						g_pTFieldGame != NULL && 
						g_pTFieldGame->GetNoviceGuide() != NULL && 
						g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true )
					{			
						g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_PREV);
					}

					return Handler_EGS_GIVE_UP_QUEST_REQ( m_iPickedQuestID );
				}
				else
				{
					UncheckAllQuestList();
				}				
			}
			return true;
		}
		return true;
	case UQRCM_QUEST_GIVE_UP_CANCLE:
		{
			if( m_pDLGMsgBox != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}
		}
		return true;
	case UQRCM_QUESTCLEAR_OK:
		{
			// 선택보상 선택 여부에서 완료 후 닫기까지..
			map<int, int> mapSelectedItem;
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );

			if( true == m_bLastRewardWasSelection )
			{
				// 선택된 보상 아이템들의 ID를 서버로 보낸다
				if( m_pDLGUIQuestClear != NULL )
				{

					if( NULL == pQuestTemplet )
					{
						return false;
					}					

					CKTDGUIRadioButton* pRadioButton1 = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( L"Selection_Reward_Slot_1" );
					CKTDGUIRadioButton* pRadioButton2 = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( L"Selection_Reward_Slot_2" );
					CKTDGUIRadioButton* pRadioButton3 = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( L"Selection_Reward_Slot_3" );

					vector<int> veciCheckedItemTID;
					if( true == pRadioButton1->GetChecked() )
					{
						veciCheckedItemTID.push_back( pRadioButton1->GetDummyInt(0) );
					}
					if( true == pRadioButton2->GetChecked() )
					{
						veciCheckedItemTID.push_back( pRadioButton2->GetDummyInt(0) );
					}
					if( true == pRadioButton3->GetChecked() )
					{
						veciCheckedItemTID.push_back( pRadioButton3->GetDummyInt(0) );
					}

					// 선택 보상이 있다고 되어 있는데 선택을 하나도 안 한 경우. User가 선택 취소를 하는 방법은 없으므로 서버에서 0을 보낸 경우이다.
					// 따라서 현재는 완료할 수 없는 퀘스트.
					if( 0 == pQuestTemplet->m_SelectReward.m_iSelectionCount )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_892 ), g_pMain->GetNowState() );
						// 완료할 수 없는 퀘스트. 퀘스트 완료창을 닫아준다.
						CloseQuestRewardWindow();

						return true;
					}

					if( veciCheckedItemTID.size() != pQuestTemplet->m_SelectReward.m_iSelectionCount )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_893 ), g_pMain->GetNowState() );

						CKTDGUIStatic* pStaticSelectionReward = (CKTDGUIStatic*) m_pDLGUIQuestClear->GetControl( L"Static_SelectionRewardSlot" );

						for ( int i = 0; i < m_byNumOfItemThatICanGet; i++ )
						{
							pStaticSelectionReward->GetPicture( i )->SetFlicker( 0.5f, 1.4f, 0.2f );
						}

						return true;
					}


					for( UINT i=0; i<veciCheckedItemTID.size(); i++ )
					{
						// 선택된 아이템의 TID를 맵에 넣어 준다.
						mapSelectedItem[ veciCheckedItemTID[i] ] = veciCheckedItemTID[i];
					}
				}
			}

			if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel() && true == m_bAdminQuestComplete )
			{
				Handler_EGS_ADMIN_QUEST_COMPLETE_REQ( m_eNPCID, m_iPickedQuestID, mapSelectedItem );
			}
			else
			{
				Handler_EGS_QUEST_COMPLETE_REQ( m_eNPCID, m_iPickedQuestID, mapSelectedItem );
			}

			CloseQuestRewardWindow();

			// 초보자 가이드	
			if(g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL && g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true)
			{				
				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_COMPLETE);
			}
		}
		return true;
	case UQRCM_SCROOL_UP:
		{
			m_iTopIndex--;
			UpdateQuestUI();

		}
		return true;
	case UQRCM_SCROOL_DOWN:
		{
			m_iTopIndex++;
			UpdateQuestUI();

		}
		return true;
	case UQRCM_REWARD_CHECK_ITEM:
		{
		}
		return true;
	case UQRCM_REWARD_UNCHECK_ITEM:
		{
		}
		return true;
	case UQRCM_REWARD_MOUSE_OVER:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*) lParam;
			CKTDGUIDialogType pDialog = (CKTDGUIDialogType)pControl->GetDialog(); 

			int ItemID = pControl->GetDummyInt(0);
			int SocketID = pControl->GetDummyInt(1);
#ifdef QUEST_REWARD_PERIOD
			int Period = pControl->GetDummyInt(2);
#endif QUEST_REWARD_PERIOD

			if( 0 == wcscmp( pControl->GetName(), L"Need_Slot1" ) )
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				D3DXVECTOR2 vPos = pDialog->GetPos() + pButton->GetPos();
				OpenRewardItemInfoPopup( true, vPos, ItemID );
			}
			else if( 0 == wcscmp( pControl->GetName(), L"Reward_Slot_1" ) )
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				D3DXVECTOR2 vPos = pDialog->GetPos() + pButton->GetPos();
#ifdef QUEST_REWARD_PERIOD
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID, Period );
#else
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID );
#endif QUEST_REWARD_PERIOD
			}
			else if( 0 == wcscmp( pControl->GetName(), L"Reward_Slot_2" ) )
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				D3DXVECTOR2 vPos = pDialog->GetPos() + pButton->GetPos();
#ifdef QUEST_REWARD_PERIOD
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID, Period );
#else
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID );
#endif QUEST_REWARD_PERIOD
			}
			else if( 0 == wcscmp( pControl->GetName(), L"Reward_Slot_3" ) )
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				D3DXVECTOR2 vPos = pDialog->GetPos() + pButton->GetPos();
#ifdef QUEST_REWARD_PERIOD
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID, Period );
#else
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID );
#endif QUEST_REWARD_PERIOD
			}

			else if( 0 == wcscmp( pControl->GetName(), L"Selection_Reward_Slot_1" ) )
			{
				CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;

				D3DXVECTOR2 vPos = pDialog->GetPos() + pRadioButton->GetPos();

#ifdef QUEST_REWARD_PERIOD
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID, Period );
#else
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID );
#endif QUEST_REWARD_PERIOD
			}
			else if( 0 == wcscmp( pControl->GetName(), L"Selection_Reward_Slot_2" ) )
			{
				CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;


				D3DXVECTOR2 vPos = pDialog->GetPos() + pRadioButton->GetPos();

#ifdef QUEST_REWARD_PERIOD
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID, Period );
#else
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID );
#endif QUEST_REWARD_PERIOD
			}
			else if( 0 == wcscmp( pControl->GetName(), L"Selection_Reward_Slot_3" ) )
			{
				CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;


				D3DXVECTOR2 vPos = pDialog->GetPos() + pRadioButton->GetPos();

#ifdef QUEST_REWARD_PERIOD
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID, Period );
#else
				OpenRewardItemInfoPopup( true, vPos, ItemID, SocketID );
#endif QUEST_REWARD_PERIOD
			}
		}
		return true;
	case UQRCM_REWARD_MOUSE_OUT:
		{
			OpenRewardItemInfoPopup( false, D3DXVECTOR2(0, 0), -1 );
		}
		return true;
	case UQRCM_NAVI_LCLICKED:
	case UQRCM_NAVI_DRAGGING:		// 둘 다 처리할 건 어차피 같으니까..
		{
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			// 0.68 : Navi Size에 따른 값. MAGIC VALUE
			m_iTopIndex = (int)((vRelativePos.y / 0.68f) * (m_vecAvailableQuestID.size() - m_iQuestPerPage));

			UpdateQuestUI( false );
		}
		return true;

	case UQRCM_QUEST_DESC_UP:
		{
			m_iQuestDescIndex--;
			UpdateQuestDescPage();
		} return true;
	case UQRCM_QUEST_DESC_DOWN:
		{
			m_iQuestDescIndex++;
			UpdateQuestDescPage();

		} return true;
	default:
		break;
	}
	return false;
}

bool CX2UIQuestReceive::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_TALK_WITH_NPC_ACK:
		{
			return Handler_EGS_TALK_WITH_NPC_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_GIVE_UP_QUEST_ACK:
		{
			return Handler_EGS_GIVE_UP_QUEST_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_QUEST_COMPLETE_ACK:
		{
			return Handler_EGS_QUEST_COMPLETE_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_NEW_QUEST_ACK:
		{
			return Handler_EGS_NEW_QUEST_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_NEW_QUEST_NOT:
		{
			return Handler_EGS_NEW_QUEST_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	}
	return false;
}

bool CX2UIQuestReceive::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	D3DXVECTOR2 mousePos;

	mousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	mousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();

	short zDelta = (short)HIWORD(wParam);
	m_SumDelta += zDelta;
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		if(m_SumDelta>0)
		{
			if( MouseOnQuestListArea(mousePos) )
			{
				m_iTopIndex--;
				UpdateQuestUI();
			}
			else if(MouseOnQuestCommentArea(mousePos))
			{
				m_iQuestDescIndex--;
				UpdateQuestDescPage();
			}
			m_SumDelta -= WHEEL_DELTA;
		}
		else
		{
			if( MouseOnQuestListArea(mousePos) )
			{
				m_iTopIndex++;
				UpdateQuestUI();
			}
			else if(MouseOnQuestCommentArea(mousePos))
			{
				m_iQuestDescIndex++;
				UpdateQuestDescPage();
			}
			m_SumDelta += WHEEL_DELTA;

		}	

	}
	return true;
}


bool CX2UIQuestReceive::Handler_EGS_TALK_WITH_NPC_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID )
{
	KEGS_TALK_WITH_NPC_REQ kPacket;
	kPacket.m_iNPCID	= (int)questNPCID;

	if(questNPCID == CX2UnitManager::NUI_MOON)
		return true;

	g_pData->GetServerProtocol()->SendPacket( EGS_TALK_WITH_NPC_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_TALK_WITH_NPC_ACK );

	return true;
}



bool CX2UIQuestReceive::Handler_EGS_TALK_WITH_NPC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TALK_WITH_NPC_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_TALK_WITH_NPC_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			if(g_pData->GetUIManager()->GetUIQuest() != NULL)
			{
				g_pData->GetUIManager()->GetUIQuest()->ResetQuestUI();
			}

			if(m_bShow)
			{
				UpdateQuestList();
				UpdateQuestUI();
			}
			
			return true;
		}
	}

	return false;
}


bool CX2UIQuestReceive::Handler_EGS_ADMIN_QUEST_COMPLETE_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID, int questID, std::map<int,int> mapSelectItemID )
{
	KEGS_QUEST_COMPLETE_REQ kPacket;
	kPacket.m_iTalkNPCID	= (int)questNPCID;
	kPacket.m_iQuestID		= questID;
	kPacket.m_mapSelectItemID	= mapSelectItemID;

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_QUEST_COMPLETE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_QUEST_COMPLETE_ACK );

	m_bAdminQuestComplete = false;

	return true;
}


bool CX2UIQuestReceive::Handler_EGS_QUEST_COMPLETE_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID, int questID, std::map<int,int> mapSelectItemID )
{
	KEGS_QUEST_COMPLETE_REQ kPacket;
	kPacket.m_iTalkNPCID	= (int)questNPCID;
	kPacket.m_iQuestID		= questID;
	kPacket.m_mapSelectItemID	= mapSelectItemID;

	g_pData->GetServerProtocol()->SendPacket( EGS_QUEST_COMPLETE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_QUEST_COMPLETE_ACK );

	return true;
}

//{{ kimhc // 2010.4.5 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST

bool CX2UIQuestReceive::Handler_EGS_QUEST_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_QUEST_COMPLETE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_QUEST_COMPLETE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetQuestManager()->RemoveUnitQuest( kEvent.m_kCompleteQuestInfo.m_iQuestID, true );

			g_pData->GetMyUser()->GetSelectUnit()->Reset( kEvent.m_kUpdateUnitInfo.m_kUnitInfo );
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_kUpdateUnitInfo.m_vecKInventorySlotInfo );
			if( NULL != g_pData->GetUIManager()->GetUIInventory() )
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_kUpdateUnitInfo.m_vecKInventorySlotInfo );
			}

			m_mapNewQuest[ kEvent.m_kCompleteQuestInfo.m_iQuestID ] = false;
			UpdateQuestList();

			// 반복 퀘스트가 아니라면 들고 있는 퀘스트 ID를 -1로 리셋해주자.
			// 반복 퀘스트이면서 목록에 있는 퀘스트라면 선택된 퀘스트 ID를 리셋하지 않는다 : 엔터 연타로 되게.

			if( CX2QuestManager::QRT_REPEAT == g_pData->GetQuestManager()->GetQuestTemplet(kEvent.m_kCompleteQuestInfo.m_iQuestID)->m_eRepeatType &&
				find( m_vecAvailableQuestID.begin(), m_vecAvailableQuestID.end(), kEvent.m_kCompleteQuestInfo.m_iQuestID ) != m_vecAvailableQuestID.end() )
			{
				m_iPickedQuestID = kEvent.m_kCompleteQuestInfo.m_iQuestID;
			}
			else
			{
				m_iPickedQuestID = -1;
			}

			UpdateQuestUI();

			// 사운드
			//g_pKTDXApp->GetDeviceManager()->PlaySound( L"QuestNew.ogg", false, false );

			//NotifyQuestListUpdate();
			//UpdateQuestListPage( m_iNowQuestListPage );
			//CloseSubQuestList();
			//CloseQuestDesc();

			// 미니맵 아이콘 업뎃해주고
			UpdateNewQuestNotice();

			// 전직 퀘스트라면 : 스킬트리 업데이트
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( kEvent.m_kCompleteQuestInfo.m_iQuestID );
			if( CX2QuestManager::QT_CHANGE_JOB == pQuestTemplet->m_eQuestType && CX2Unit::UC_NONE != pQuestTemplet->m_Reward.m_eChangeUnitClass )
			{
				// 스킬트리 업데이트. 새로 초기화 시켜 준다.
				g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();

				//g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_894 ) );
				//g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CLASS_CHANGE, NULL, false );

				g_pData->GetClassChangePopup().SetShow( true );

				CX2State* pState = (CX2State*) g_pMain->GetNowState();
				pState->ResetUnitViewerInFieldSquare( g_pData->GetMyUser()->GetSelectUnit()->GetUID(), g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
			}

			if(g_pData->GetUIManager()->GetUIQuest() != NULL)
			{
				g_pData->GetUIManager()->GetUIQuest()->ResetQuestUI();
			}



			return true;
		}
	}

	return false;
}

#else	SERV_DAILY_QUEST

bool CX2UIQuestReceive::Handler_EGS_QUEST_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_QUEST_COMPLETE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_QUEST_COMPLETE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetQuestManager()->RemoveUnitQuest( kEvent.m_iQuestID, true );

			g_pData->GetMyUser()->GetSelectUnit()->Reset( kEvent.m_kUpdateUnitInfo.m_kUnitInfo );
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_kUpdateUnitInfo.m_vecKInventorySlotInfo );
			if( NULL != g_pData->GetUIManager()->GetUIInventory() )
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_kUpdateUnitInfo.m_vecKInventorySlotInfo );
			}

			m_mapNewQuest[ kEvent.m_iQuestID ] = false;
			UpdateQuestList();

			// 반복 퀘스트가 아니라면 들고 있는 퀘스트 ID를 -1로 리셋해주자.
			// 반복 퀘스트이면서 목록에 있는 퀘스트라면 선택된 퀘스트 ID를 리셋하지 않는다 : 엔터 연타로 되게.

			if( true == g_pData->GetQuestManager()->GetQuestTemplet(kEvent.m_iQuestID)->m_bRepeat &&
				find( m_vecAvailableQuestID.begin(), m_vecAvailableQuestID.end(), kEvent.m_iQuestID ) != m_vecAvailableQuestID.end() )
			{
				m_iPickedQuestID = kEvent.m_iQuestID;
			}
			else
			{
				m_iPickedQuestID = -1;
			}

			UpdateQuestUI();



			// 사운드
			//g_pKTDXApp->GetDeviceManager()->PlaySound( L"QuestNew.ogg", false, false );

			//NotifyQuestListUpdate();
			//UpdateQuestListPage( m_iNowQuestListPage );
			//CloseSubQuestList();
			//CloseQuestDesc();

			// 미니맵 아이콘 업뎃해주고
			UpdateNewQuestNotice();

			// 전직 퀘스트라면 : 스킬트리 업데이트
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( kEvent.m_iQuestID );
			if( CX2QuestManager::QT_CHANGE_JOB == pQuestTemplet->m_eQuestType && CX2Unit::UC_NONE != pQuestTemplet->m_Reward.m_eChangeUnitClass )
			{
				// 스킬트리 업데이트. 새로 초기화 시켜 준다.
				g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();

				//g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_894 ) );
				//g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CLASS_CHANGE, NULL, false );

				g_pData->GetClassChangePopup().SetShow( true );

				CX2State* pState = (CX2State*) g_pMain->GetNowState();
				pState->ResetUnitViewerInFieldSquare( g_pData->GetMyUser()->GetSelectUnit()->GetUID(), g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
			}

			if(g_pData->GetUIManager()->GetUIQuest() != NULL)
			{
				g_pData->GetUIManager()->GetUIQuest()->ResetQuestUI();
			}



			return true;
		}
	}

	return false;
}

#endif	SERV_DAILY_QUEST
//}} kimhc // 2010.4.5 // 비밀던전 작업(일일퀘스트 시스템)


bool CX2UIQuestReceive::Handler_EGS_GIVE_UP_QUEST_REQ( int questID )
{
	KEGS_GIVE_UP_QUEST_REQ kPacket;
	kPacket.m_iQuestID	= questID;

	g_pData->GetServerProtocol()->SendPacket( EGS_GIVE_UP_QUEST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GIVE_UP_QUEST_ACK );

	return true;
}

bool CX2UIQuestReceive::Handler_EGS_GIVE_UP_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GIVE_UP_QUEST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GIVE_UP_QUEST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetQuestManager()->RemoveUnitQuest( kEvent.m_iQuestID, false );
			
			UpdateNewQuestNotice();

			if(m_bShow)
			{
				m_iPickedQuestID = -1;
				UncheckAllQuestList();

				UpdateQuestList();
				UpdateQuestUI();
				// NPC 대사도..
				UpdateNPCQuestDesc();
			}

			if(g_pData->GetUIManager()->GetUIQuest() != NULL)
			{
				g_pData->GetUIManager()->GetUIQuest()->ResetQuestUI( false );
			}
			return true;
		}
	}

	return false;
}


bool CX2UIQuestReceive::Handler_EGS_NEW_QUEST_REQ( int iQuestID )
{
	KEGS_NEW_QUEST_REQ kPacket;
	kPacket.m_iTalkNPCID	= (int) m_eNPCID;
	kPacket.m_iQuestID		= iQuestID;

	g_pData->GetServerProtocol()->SendPacket( EGS_NEW_QUEST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_NEW_QUEST_ACK );

	return true;
}


bool CX2UIQuestReceive::Handler_EGS_NEW_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NEW_QUEST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_NEW_QUEST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
		}
		else
		{
			//UncheckAllQuestList();
		}

		// Handler_EGS_NEW_QUEST_NOT에서 처리
		//UpdateQuestList();
		//UpdateQuestUI();

// 		if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST))
// 		{
// 			g_pData->GetUIManager()->GetUIQuest()->ResetQuestUI();
// 		}

		return true;
	}

	return false;
}

bool CX2UIQuestReceive::Handler_EGS_NEW_QUEST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NEW_QUEST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	g_pData->GetQuestManager()->CreateUnitQuest( kEvent.m_QuestInst.m_iID, kEvent.m_QuestInst );
	m_iPickedQuestID = kEvent.m_QuestInst.m_iID;

	m_mapNewQuest[ kEvent.m_QuestInst.m_iID ] = false;

	// 사운드
	g_pKTDXApp->GetDeviceManager()->PlaySound( L"QuestAccept.ogg", false, false );
		
	UpdateQuestList();
	UpdateQuestUI();
	// NPC 대사도..
	UpdateNPCQuestDesc();
	UpdateNewQuestNotice();

	//m_iPickedQuestID = -1;
	//UncheckAllQuestList();

		//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
	if( CX2QuestManager::QRT_REPEAT != 
		g_pData->GetQuestManager()->GetQuestTemplet(kEvent.m_QuestInst.m_iID)->m_eRepeatType )
#else	SERV_DAILY_QUEST
	if(false == g_pData->GetQuestManager()->GetQuestTemplet(kEvent.m_QuestInst.m_iID)->m_bRepeat)
#endif	SERV_DAILY_QUEST
		//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
	{
		UncheckAllQuestList();
	}

	if(g_pData->GetUIManager()->GetUIQuest() != NULL)
	{
		g_pData->GetUIManager()->GetUIQuest()->ResetQuestUI();
	}

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );
	if(g_pData->GetUIManager()->GetUIInventory() != NULL)
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );
	}			
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
		
	//npc대화 퀘스트라면 다시한번 npc talk msg를 보낸다.			
	if( m_iPickedQuestID != -1 )
	{
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet(m_iPickedQuestID);
		if( pQuestTemplet != NULL )
		{
			for(UINT i=0; i<pQuestTemplet->m_vecSubQuest.size(); ++i)
			{
				const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet(pQuestTemplet->m_vecSubQuest[i]);
				if( pSubQuestTemplet!=NULL && pSubQuestTemplet->m_eClearType ==  CX2QuestManager::SQT_NPC_TALK )
				{
					Handler_EGS_TALK_WITH_NPC_REQ( m_eNPCID );
					return true;
				}
			}
		}
	}

	// 	UpdateQuestListPage( m_iNowQuestListPage );
	// 
	// 	CloseSubQuestList();
	// 	CloseQuestDesc();

	// note!! special case
	// 초심자 숲 가기 퀘스트
	// 	int MAGIC_QUEST_ID = 11; 
	// 	if( MAGIC_QUEST_ID == kEvent.m_QuestInst.m_iID )
	// 	{
	// 		CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
	// 		if( NULL != pStateMenu )
	// 		{
	// 			pStateMenu->CreateMouseClickUI( (CKTDXStage*) this, D3DXVECTOR2(960, 30), 0.5f, 1000, 
	// 				CX2State::MouseClickUI::MCS_FROM_LEFT_BOTTOM ); // EXIT 버튼 
	// 		}
	// 	}
	// 
	// 	MAGIC_QUEST_ID = 3100; 
	// 	if( MAGIC_QUEST_ID == kEvent.m_QuestInst.m_iID )
	// 	{
	// 		std::wstring packetExplain;
	// 		packetExplain += L"퀘스트를 수락하였습니다.\n내 퀘스트창에서 확인 하실 수 있습니다.";
	// 		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), packetExplain.c_str(), (CKTDXStage*)g_pMain->GetNowState() );
	// 	}

	return true;
}

void CX2UIQuestReceive::SetShow(bool val)
{
	m_bShow = val;
	m_pDLGUIQuestReceive->SetShowEnable(m_bShow, m_bShow);

	if(val)	// 켜질 때 처리해야 할 부분
	{
		// 일단 퀘 리스트를 받아 오고
		//UpdateQuestList( m_eNPCID );

		// UI의 퀘 리스트를 세팅해 주고
		ResetQuestUI();

		// 개발자용 완료버튼
		if( m_pDLGUIQuestReceive != NULL )
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Quest_Admin_Complete" );
			if( NULL != pButton )
			{
				if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel() )
				{
					pButton->SetShowEnable( true, true );
				}
				else
				{
					pButton->SetShowEnable( false, false );
				}
			}
		}

		//g_pKTDXApp->SkipFrame();
		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_QUEST_RECEIVE);
				
	}
	else	// 꺼질 때 처리해야 할 부분
	{
		// 아이템 마우스 오버 이미지를 지우자
		m_pDLGSelectedItem->SetShow( false ); 
		InvalidSlotDesc();

		// 슬롯을 끄자
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			pItemSlot->LostItemUI();
			pItemSlot->SetShow( false );
			//pItemSlot->SetEnable( false );
		}

		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_QUEST_RECEIVE);
	}

}

void CX2UIQuestReceive::UpdateQuestList( CX2UnitManager::NPC_UNIT_ID eNPCID )
{
	if( CX2UnitManager::NUI_NONE == eNPCID )
	{
		eNPCID = m_eNPCID;
	}

	vector<int> vecQuestID;
	vector<int> vecCompletableQuestID;
	vector<int> vecOnGoingQuestID;
	g_pData->GetQuestManager()->GetAvailableQuest( eNPCID, vecQuestID );
	g_pData->GetQuestManager()->GetCompletableQuest( eNPCID, vecCompletableQuestID );
	g_pData->GetQuestManager()->GetOnGoingQuest( eNPCID, vecOnGoingQuestID );


	// 아직 받지 않는 퀘스트 목록을 정렬하자
	vector<int> vecChangeJobQuestID;
	vector<int> vecEventQuestID;
	vector<int> vecNotAssignedQuestID;
	vector<int> vecNotAssignedQuestID2; // 퀘스트 적정레벨이 내 레벨보다 5레벨 이하인것들 (회색)


	for( UINT i=0; i<vecQuestID.size(); i++ )
	{
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( vecQuestID[i] );
		if( NULL == pQuestTemplet )
			continue;

		const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( vecQuestID[i] );

//{{ kimhc // 2010.2.1 //	PC방이 아닌곳에서는 받지 않은 PC방퀘스트 목록에서 보이지 않도록
#ifdef	PC_BANG_QUEST
		if ( pQuestTemplet->m_bIsPcBang == true && g_pData->GetMyUser()->GetIsPCRoom() == false
			 && pQuestInst == NULL )
			 continue;
#endif	PC_BANG_QUEST
//}} kimhc // 2010.2.1 //	PC방이 아닌곳에서는 받지 않은 PC방퀘스트 목록에서 보이지 않도록
		
		//{{ kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
#ifdef	SERV_DAY_QUEST
		if ( pQuestTemplet->m_bIsTimeEvent == true )
			continue;
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		if( IsNewUserOnlyQuest( vecAvailQuestID[i] ) == true &&
			g_pInstanceData->IsRecruit() == false )
			continue;
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

		if( CX2QuestManager::QT_CHANGE_JOB == pQuestTemplet->m_eQuestType && NULL == pQuestInst )
		{
			vecChangeJobQuestID.push_back( vecQuestID[i] );
		}
		else if( ( CX2QuestManager::QT_EVENT == pQuestTemplet->m_eQuestType 
			//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
			|| CX2QuestManager::QT_PCBANG == pQuestTemplet->m_eQuestType
#endif	PC_BANG_QUEST
			//}} kimhc // 2010.2.1 //	PC방 퀘스트
			)
			&& NULL == pQuestInst )
		{
			vecEventQuestID.push_back( vecQuestID[i] );
		}
		else
		{
			int iLevelDiff = g_pData->GetSelectUnitLevel() - pQuestTemplet->m_iFairLevel;
			if( iLevelDiff > 5 )
			{
				vecNotAssignedQuestID2.push_back( vecQuestID[i] );
			}
			else
			{
				vecNotAssignedQuestID.push_back( vecQuestID[i] );
			}
		}
	}


	m_vecPrevNotAssignedQuestID = m_vecNotAssignedQuestID;
	m_vecNotAssignedQuestID = vecEventQuestID;
	m_vecNotAssignedQuestID.insert( m_vecNotAssignedQuestID.end(), vecChangeJobQuestID.begin(), vecChangeJobQuestID.end() );
	m_vecNotAssignedQuestID.insert( m_vecNotAssignedQuestID.end(), vecNotAssignedQuestID.begin(), vecNotAssignedQuestID.end() );
	m_vecNotAssignedQuestID.insert( m_vecNotAssignedQuestID.end(), vecNotAssignedQuestID2.begin(), vecNotAssignedQuestID2.end() );


	m_vecAvailableQuestID.resize(0);

	m_vecAvailableQuestID.insert( m_vecAvailableQuestID.end(), vecCompletableQuestID.begin(), vecCompletableQuestID.end() ); 
	m_vecAvailableQuestID.insert( m_vecAvailableQuestID.end(), vecEventQuestID.begin(), vecEventQuestID.end() ); 
	m_vecAvailableQuestID.insert( m_vecAvailableQuestID.end(), vecChangeJobQuestID.begin(), vecChangeJobQuestID.end() ); 
	m_vecAvailableQuestID.insert( m_vecAvailableQuestID.end(), vecNotAssignedQuestID.begin(), vecNotAssignedQuestID.end() ); 
	m_vecAvailableQuestID.insert( m_vecAvailableQuestID.end(), vecNotAssignedQuestID2.begin(), vecNotAssignedQuestID2.end() );
	m_vecAvailableQuestID.insert( m_vecAvailableQuestID.end(), vecOnGoingQuestID.begin(), vecOnGoingQuestID.end() ); 









}

// 말 그대로 리~셋
void CX2UIQuestReceive::ResetQuestUI()
{
	if(!m_bShow)
	{
		return;
	}
	// 보상 EXP/ED/SP String 0으로 만들어 주고
	CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_ED" );
	pStaticED->GetString(0)->msg = L"";
	CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_Exp" );
	pStaticExp->GetString(0)->msg = L"";
	CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_SP" );
	pStaticSP->GetString(0)->msg = L"";

	// 슬롯 다 안 보이게 만들고
	for(UINT i=0; i<m_SlotList.size(); i++)
	{
		CX2SlotItem* pSlot = (CX2SlotItem*) m_SlotList[i];
		pSlot->SetShow(false);
	}
	// 퀘스트 슬롯리스트 싹 리셋해주고
	// 1. 일단 다 지운다
// 	for(UINT i=0; i<m_vQuestList.size(); i++)
// 	{
// 		QuestListSlot* pQuestList = m_vQuestList[i];
// 		SAFE_DELETE(pQuestList);
// 	}
	m_vQuestList.clear();
	//2. 새로 만든-_-다
	WCHAR buff[256] = {0,};
	for(int i=0; i<m_iQuestPerPage; i++)
	{
		QuestListSlot QuestList;

		// 버튼 세팅해줄것
		//wsprintf( buff, L"RadioButton_List%d", i);
		StringCchPrintf( buff, 256, L"RadioButton_List%d", i);
		QuestList.m_pButton = (CKTDGUIRadioButton*) m_pDLGUIQuestReceive->GetControl( buff );
		QuestList.m_pButton->SetShow(false);
		// 스태틱 세팅해줄것
		//wsprintf( buff, L"Static_QuestList%d", i);
		StringCchPrintf( buff, 256, L"Static_QuestList%d", i);
		QuestList.m_pStatic = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( buff );
		QuestList.m_pStatic->SetShow(false);

		QuestList.m_QuestID = -1;
		m_vQuestList.push_back(QuestList);
	}

	m_iTopIndex = 0;
	// 퀘 리스트 받아오고
	UpdateQuestList();

	// 선택된 상태를 취소해주고
	UncheckAllQuestList();

	// 퀘 UI를 리셋해주면 끗
	UpdateQuestUI();

}

void CX2UIQuestReceive::UpdateQuestUI( bool bUpdateNavi )
{
	if(false == m_bShow)
		return;

	if (m_iTopIndex > (int)m_vecAvailableQuestID.size() - m_iQuestPerPage) m_iTopIndex = m_vecAvailableQuestID.size() - m_iQuestPerPage;
	if (m_iTopIndex < 0) m_iTopIndex = 0;

	for(int i = 0; i<m_iQuestPerPage; i++)
	{
		QuestListSlot& QuestList = m_vQuestList[i];

		if( i+m_iTopIndex < (int)m_vecAvailableQuestID.size() )
		{
			QuestList.m_QuestID = m_vecAvailableQuestID[i+m_iTopIndex];

			// 퀘 ID를 이용해서 퀘스트 정보를 얻고
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( QuestList.m_QuestID );

			//제목 넣고
			QuestList.m_pStatic->GetString(0)->msg = pQuestTemplet->m_wstrTitle;
			// 색깔 넣고
			QuestList.m_pStatic->GetString(0)->color = g_pData->GetQuestManager()->GetQuestColor( QuestList.m_QuestID, g_pData->GetSelectUnitLevel() );
			// 종류 정하고
			QuestList.m_pButton->SetShow(true);
			QuestList.m_pStatic->SetShow(true);
			QuestList.m_pStatic->GetPicture(0)->SetShow(false);
			QuestList.m_pStatic->GetPicture(1)->SetShow(false);
			QuestList.m_pStatic->GetPicture(2)->SetShow(false);
			QuestList.m_pStatic->GetPicture(3)->SetShow(false);

			QuestList.m_pStatic->GetPicture(4)->SetShow(false);	// 완료
			QuestList.m_pStatic->GetPicture(5)->SetShow(false);	// 진행중
			QuestList.m_pStatic->GetPicture(6)->SetShow(false);	// NEW
			//{{ kimhc // 2009-08-03 // 스킬 picture 추가
			QuestList.m_pStatic->GetPicture( 7 )->SetShow( false );	// skill
			//}} kimhc // 2009-08-03 // 스킬 picture 추가
			
			//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
			QuestList.m_pStatic->GetPicture( 8 )->SetShow( false );
#endif	PC_BANG_QUEST
			//}} kimhc // 2010.2.1 //	PC방 퀘스트

			//{{ kimhc // 2010.4.27 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef SERV_SECRET_HELL
			QuestList.m_pStatic->GetPicture( 9 )->SetShow( false );
#endif SERV_SECRET_HELL
			//}} kimhc // 2010.4.27 // 비밀던전 작업(일일퀘스트 시스템)


			switch(pQuestTemplet->m_eQuestType)
			{
			case CX2QuestManager::QT_NORMAL:
				{
					//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
				#ifdef	SERV_DAILY_QUEST
					switch ( pQuestTemplet->m_eRepeatType )
					{
					case  CX2QuestManager::QRT_NORMAL:
						QuestList.m_pStatic->GetPicture(0)->SetShow(true);
						break;

					case CX2QuestManager::QRT_REPEAT:
						QuestList.m_pStatic->GetPicture(3)->SetShow(true);
						break;

					case CX2QuestManager::QRT_DAY:
						QuestList.m_pStatic->GetPicture(9)->SetShow(true);
						break;

					default:
						break;
					}
				#else	SERV_DAILY_QUEST
					if(pQuestTemplet->m_bRepeat == false)	// 일반퀘
					{
						QuestList.m_pStatic->GetPicture(0)->SetShow(true);

					}
					else	// 반복퀘
					{
						QuestList.m_pStatic->GetPicture(3)->SetShow(true);
					}
				#endif	SERV_DAILY_QUEST
					//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)

				}break;
			case CX2QuestManager::QT_CHANGE_JOB:
				{
					QuestList.m_pStatic->GetPicture(1)->SetShow(true);
				}break;
			case CX2QuestManager::QT_EVENT:
				{
					QuestList.m_pStatic->GetPicture(2)->SetShow(true);
				}break;

				//{{ kimhc // 2009-08-03 // 스킬 picture 추가
			case CX2QuestManager::QT_SKILL:
				{
					QuestList.m_pStatic->GetPicture( 7 )->SetShow(true);
				}break;
				//}} kimhc // 2009-08-03 // 스킬 picture 추가

				//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
			case CX2QuestManager::QT_PCBANG:
				{
					QuestList.m_pStatic->GetPicture( 8 )->SetShow( true );
				} break;
#endif	PC_BANG_QUEST
					//}} kimhc // 2010.2.1 //	PC방 퀘스트
			default:
				break;
			}

			// 켜주고
			if(QuestList.m_QuestID == m_iPickedQuestID && m_iPickedQuestID != -1 )
				QuestList.m_pButton->SetChecked(true);
			else if(true == QuestList.m_pButton->GetChecked())
			{
				QuestList.m_pButton->SetChecked(false);
			}

		}
		else
		{
			QuestList.m_pButton->SetShow(false);
			QuestList.m_pStatic->SetShow(false);
			QuestList.m_QuestID = -1;

		}

		const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( QuestList.m_QuestID );

		if( NULL == pQuestInst && false == g_pData->GetQuestManager()->GetUnitCompleteQuest( QuestList.m_QuestID ) )
		{
			QuestList.m_pStatic->GetPicture(6)->SetShow(true);
		}

		if( NULL != pQuestInst )
		{
			if( true == pQuestInst->IsComplete() )
			{
				// 완료된 퀘스트
				QuestList.m_pStatic->GetPicture(4)->SetShow(true);	// 완료
			}
			else
			{
				// 진행중인 퀘스트
				QuestList.m_pStatic->GetPicture(5)->SetShow(true);	// 진행중
			}
		}
	}

	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	if( pQuestTemplet != NULL )
	{
		// 골라진 퀘 상태에 따라서 버튼 바꾸고
		CKTDGUIButton* pButtonGiveup = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Giveup" );
		CKTDGUIButton* pButtonAccept = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Accept" );
		CKTDGUIButton* pButtonComplete = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Complete" );
		CKTDGUIButton* pButtonNext = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Next" );
		pButtonGiveup->SetShow(false);
		pButtonAccept->SetShow(false);
		pButtonComplete->SetShow(false);
		pButtonNext->SetShow(false);

		const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );
		if( NULL != pQuestInst )
		{
			if( true == pQuestInst->IsComplete() )
			{
				// 완료된 퀘스트
				//pStatic_Complete->SetShowEnable( true, true );	
				pButtonComplete->SetShow(true);
				pButtonComplete->RequestFocus();
			}
			else
			{
				// 진행중인 퀘스트
				//pStatic_Incomplete->SetShowEnable( true, true );	
				pButtonGiveup->SetShow(true);
				pButtonGiveup->RequestFocus();
			}
		}
		else
		{
			// 안 받은 퀘스트
			CKTDGUIButton* pButtonAccept = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Accept" );
			pButtonAccept->SetShow(true);
			pButtonAccept->RequestFocus();
		}

		// 보상 정보 넣어주고
		WCHAR wszText[64] = {0,};
		CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_ED" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iED );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iED );
		pStaticED->GetString(0)->msg = wszText;

		CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_Exp" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iEXP );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iEXP );
		pStaticExp->GetString(0)->msg = wszText;

		CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_SP" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iSP );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iSP );
		pStaticSP->GetString(0)->msg = wszText;		

		// 보상 아이템 슬롯 넣어주고
		SetRewardItemSlot();

	}
	else
	{
		// 골라진 퀘가 없다
		CKTDGUIButton* pButtonGiveup = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Giveup" );
		CKTDGUIButton* pButtonAccept = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Accept" );
		CKTDGUIButton* pButtonComplete = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Complete" );
		CKTDGUIButton* pButtonNext = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Next" );
		pButtonAccept->SetShow(false);
		pButtonAccept->SetShow(false);
		pButtonComplete->SetShow(false);
		pButtonNext->SetShow(false);

		// 보상 정보를 비우자
		CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_ED" );
		pStaticED->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_Exp" );
		pStaticExp->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_SP" );
		pStaticSP->GetString(0)->msg = L"";

		// 보상 아이템 정보를 비우자
		SetRewardItemSlot();
	}

	// 퀘스트 설명을 만들고
	const wstring wstrDesc = CreateQuestDesc();
	UpdateQuestDesc( wstrDesc );
	m_iQuestDescIndex = 0;
	UpdateQuestDescPage();
	
	{
		UpdateNavi();
	}
}

void CX2UIQuestReceive::UncheckAllQuestList()
{
	m_iPickedQuestID = -1;

	for(int i = 0; i<m_iQuestPerPage; i++)
	{
		QuestListSlot& QuestList = m_vQuestList[i];
		QuestList.m_pButton->SetChecked(false);
	}
	// 보상 EXP/ED/SP String 0으로 만들어 주자
	CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_ED" );
	pStaticED->GetString(0)->msg = L"";
	CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_Exp" );
	pStaticExp->GetString(0)->msg = L"";
	CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_SP" );
	pStaticSP->GetString(0)->msg = L"";

	// 슬롯 꺼주자
	// 슬롯 다 안 보이게 만들고
	for(UINT i=0; i<m_SlotList.size(); i++)
	{
		CX2SlotItem* pSlot = (CX2SlotItem*) m_SlotList[i];
		pSlot->SetShow(false);
	}

	// 퀘스트 완료포기버튼 등등을 다 꺼주자
	CKTDGUIButton* pButtonGiveup = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Giveup" );
	CKTDGUIButton* pButtonAccept = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Accept" );
	CKTDGUIButton* pButtonComplete = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Complete" );
	CKTDGUIButton* pButtonNext = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Next" );
	pButtonAccept->SetShow(false);
	pButtonAccept->SetShow(false);
	pButtonComplete->SetShow(false);
	pButtonNext->SetShow(false);

	UpdateQuestUI();
}


void CX2UIQuestReceive::SelectQuest( int SelectedQuestID )
{
	m_iPickedQuestID = SelectedQuestID;
	UpdateQuestUI();

	// 초보자 가이드	
	bool bHasGuideQuest = false;	
	if( g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL && g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true )
	{
		for(UINT i=0; i<m_vecAvailableQuestID.size(); ++i)
		{
			if(m_vecAvailableQuestID[i] == 11000 || m_vecAvailableQuestID[i] == 11010)
			{
				bHasGuideQuest = true;				
			}
		}

		if( (g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP2 && 
			g_pTFieldGame->GetJoinNpcId() == CX2UnitManager::NUI_ARIEL) ||
			(g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP3 &&
			g_pTFieldGame->GetJoinNpcId() == CX2UnitManager::NUI_HAGERS) ||
			(g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP8 &&
			g_pTFieldGame->GetJoinNpcId() == CX2UnitManager::NUI_HAGERS) )
		{
			if(bHasGuideQuest == true && (SelectedQuestID == 11000 || SelectedQuestID == 11010))
			{
				g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(4);
			}
			else if(bHasGuideQuest == true && (SelectedQuestID != 11000 && SelectedQuestID != 11010))
			{
				g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(3);
			}		
		}		
	}

	UpdateNPCQuestDesc();

}


void CX2UIQuestReceive::SetRewardItemSlot()
{
	m_mapItemIDAndSocketID.clear();

	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	if( pQuestTemplet != NULL )
	{
		for(int i=0; i<m_iRewardSlotNum; i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[i];

			if( i < (int)pQuestTemplet->m_Reward.m_vecItem.size() )
			{
				// 보상슬롯
				const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_Reward.m_vecItem[i];
                const CX2Item::ItemTemplet* pItemTemplet = 
                    g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
				if ( itemData.m_iSocketOption1 != 0 )
				{
					m_mapItemIDAndSocketID.insert( std::make_pair( itemData.m_iItemID, itemData.m_iSocketOption1 ) );
				}
				if(pItemTemplet != NULL)
				{
					CX2Item::ItemData kItemData;
					kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
					kItemData.m_SocketOption.push_back(itemData.m_iSocketOption1);
					kItemData.m_ItemID = itemData.m_iItemID;
					kItemData.m_Period = itemData.m_iPeriod;
					kItemData.m_Endurance = pItemTemplet->GetEndurance();
					CX2Item* pItem = new CX2Item( kItemData, NULL);
					if ( pItem != NULL )
					{
						if ( pSlotItem != NULL )
						{
							pSlotItem->CreateItemUI( pItem, itemData.m_iQuantity );
							pSlotItem->SetShow(true);
							pSlotItem->SetEnable(true);
						}
					}
					SAFE_DELETE(pItem);
				}

			}
			else
			{
				// 비우자
				if(pSlotItem != NULL)
				{
					pSlotItem->DestroyItemUI();
					pSlotItem->SetShow(false);
				}
			}
		}

		// 선택보상
		int SlotIndex = 0;
		for(int i=0; SlotIndex < m_iRewardSlotNum; i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[SlotIndex+m_iRewardSlotNum];

			if( i < (int)pQuestTemplet->m_SelectReward.m_vecSelectItem.size())
			{
				// 선택보상슬롯
				const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_SelectReward.m_vecSelectItem[i];
                const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
				//{{10.01.14 김정협 아이템 없을 시 문제 처리
				if(pItemTemplet == NULL)
					continue;
				//}}10.01.14 김정협 아이템 없을 시 문제 처리
            

				// fix!! 일단 선택보상에서는 unit_type만 체크
				if( pItemTemplet->GetUnitType() != CX2Unit::UT_NONE )
				{
					CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
					if( pItemTemplet->GetUnitType() != eUnitType )
					{
						continue;
					}
				}
				if ( itemData.m_iSocketOption1 != 0 )
				{
					m_mapItemIDAndSocketID.insert( std::make_pair( itemData.m_iItemID, itemData.m_iSocketOption1 ) );
				}
				if(pItemTemplet != NULL)
				{
					CX2Item::ItemData kItemData;
					kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
					kItemData.m_SocketOption.push_back(itemData.m_iSocketOption1);
					kItemData.m_ItemID = itemData.m_iItemID;
					kItemData.m_Period = itemData.m_iPeriod;
					kItemData.m_Endurance = pItemTemplet->GetEndurance();
					CX2Item* pItem = new CX2Item( kItemData, NULL);
					if ( pItem != NULL )
					{
						if ( pSlotItem != NULL )
						{
							pSlotItem->CreateItemUI( pItem, itemData.m_iQuantity );
							pSlotItem->SetShow(true);
							pSlotItem->SetEnable(true);
						}
					}
					SAFE_DELETE(pItem);

				}
			}
			else
			{
				// 비우자
				if(pSlotItem != NULL)
				{
					pSlotItem->DestroyItemUI();
					pSlotItem->SetShow(false);
				}
			}
			SlotIndex++;
		}

	}
	else
	{
		// 다 비워버려 다 미워
		for(UINT i=0; i<m_SlotList.size(); i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[i];
			pSlotItem->DestroyItemUI();
			pSlotItem->SetShow(false);
		}
	}
}


std::wstring CX2UIQuestReceive::GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem )
{
	std::wstring slotItemDesc = L"";
	return slotItemDesc;
}

wstring CX2UIQuestReceive::GetSlotItemDesc()
{
	wstring itemDesc = L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetDialog() == NULL )
		{
			itemDesc = m_pNowOverItemSlot->GetSlotDesc();
		}
		else
		{	
			CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( 
				m_pNowOverItemSlot->GetItemUID() );
			if ( pkItem != NULL )
				itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
			else
			{
				CX2Item* pItem = NULL;
				map< int, int >::iterator mit;
				mit = m_mapItemIDAndSocketID.find( m_pNowOverItemSlot->GetItemTID() );
				if ( mit != m_mapItemIDAndSocketID.end() )
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
					if ( pItemTemplet != NULL )
					{
					    CX2Item::ItemData kItemData;
						kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
						kItemData.m_Endurance = pItemTemplet->GetEndurance();
						int socketID = mit->second; 
						kItemData.m_SocketOption.push_back( (short)socketID );
						kItemData.m_ItemID = m_pNowOverItemSlot->GetItemTID();
#ifdef QUEST_REWARD_PERIOD
						if(m_pNowOverItemSlot->GetPeriod() != 0)
						{
							kItemData.m_Period = m_pNowOverItemSlot->GetPeriod();
						}
#endif QUEST_REWARD_PERIOD
						CX2Item* pItem = new CX2Item( kItemData, NULL );
						itemDesc = GetSlotItemDescByTID( pItem, m_pNowOverItemSlot->GetItemTID() );
						SAFE_DELETE( pItem );
					}

				}
#ifdef QUEST_REWARD_PERIOD	
				else if(m_pNowOverItemSlot->GetPeriod() != 0)
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
					if ( pItemTemplet != NULL )
					{
					    CX2Item::ItemData kItemData;
						kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
						kItemData.m_Period = m_pNowOverItemSlot->GetPeriod();
						kItemData.m_Endurance = pItemTemplet->GetEndurance();
						kItemData.m_ItemID = m_pNowOverItemSlot->GetItemTID();
						CX2Item* pItem = new CX2Item( kItemData, NULL );
						itemDesc = GetSlotItemDescByTID( pItem, m_pNowOverItemSlot->GetItemTID() );
						SAFE_DELETE( pItem );
					}
				}
#endif QUEST_REWARD_PERIOD
				else
				{
					itemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID() );
				}

			}
		}

	}

	return itemDesc;	
}

void CX2UIQuestReceive::OpenQuestRewardWindow( int iQuestID, bool bEnableSelection )
{
	if(iQuestID == -1) return;

	m_bLastRewardWasSelection = bEnableSelection;

	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( iQuestID );
	if( NULL == pQuestTemplet )
		return;

	if( m_pDLGUIQuestClear == NULL )
	{
		m_pDLGUIQuestClear = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Quest_Clear.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIQuestClear );
	}

	// 돈 경험치 SP
	WCHAR wszText[64] = {0,};
	CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuestClear->GetControl( L"Static_ED" );
	CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuestClear->GetControl( L"Static_Exp" );
	CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuestClear->GetControl( L"Static_SP" );

	//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iED );
	StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iED );
	pStaticED->GetString(0)->msg = wszText;
	//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iEXP );
	StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iEXP );
	pStaticExp->GetString(0)->msg = wszText;
	//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iSP );
	StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iSP );
	pStaticSP->GetString(0)->msg = wszText;		

	// 일반보상 세팅
	CKTDGUIStatic* pStaticReward = (CKTDGUIStatic*) m_pDLGUIQuestClear->GetControl( L"Static_RewardSlot" );

	for(int i=0; i<m_iRewardSlotNum; i++)
	{
		if( i < (int)pQuestTemplet->m_Reward.m_vecItem.size() )
		{
			// 보상슬롯
			const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_Reward.m_vecItem[i];
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
			if(pItemTemplet != NULL)
			{
				pStaticReward->GetPicture(i)->SetTex( 
                    pItemTemplet->GetShopImage() 
                    );

				// 보상 버튼에 DummyInt로 ItemID를 달아 주자!
				// 더미인트 0 : ItemID, 1 : SlotID
				WCHAR buff[256] = {0,};
				//wsprintf( buff, L"Reward_Slot_%d", i+1);
				StringCchPrintf( buff, 256, L"Reward_Slot_%d", i+1);
				CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGUIQuestClear->GetControl( buff );				
				pButton->AddDummyInt( itemData.m_iItemID );
				pButton->AddDummyInt( itemData.m_iSocketOption1 );
#ifdef QUEST_REWARD_PERIOD
				pButton->AddDummyInt( itemData.m_iPeriod );
#endif QUEST_REWARD_PERIOD
			}

		}
		else
		{
			WCHAR buff[256] = {0,};
			//wsprintf( buff, L"Reward_Slot_%d", i+1);
			StringCchPrintf( buff, 256, L"Reward_Slot_%d", i+1);
			CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGUIQuestClear->GetControl( buff );
			pButton->SetShowEnable(false, false);
			pButton->AddDummyInt( 0 );
			pButton->AddDummyInt( 0 );
#ifdef QUEST_REWARD_PERIOD
			pButton->AddDummyInt( 0 );
#endif QUEST_REWARD_PERIOD

		}
	}



	// 선택보상 세팅
	CKTDGUIStatic* pStaticSelectionReward = (CKTDGUIStatic*) m_pDLGUIQuestClear->GetControl( L"Static_SelectionRewardSlot" );

	// 	CKTDGUIRadioButton* pRadioButton1 = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( L"Selection_Reward_Slot_1" );
	// 	CKTDGUIRadioButton* pRadioButton2 = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( L"Selection_Reward_Slot_2" );
	// 	CKTDGUIRadioButton* pRadioButton3 = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( L"Selection_Reward_Slot_3" );



	int SlotIndex = 0;
	m_byNumOfItemThatICanGet = 0;

	for(int i=0; SlotIndex<m_iRewardSlotNum; i++)
	{
		if( i < (int)pQuestTemplet->m_SelectReward.m_vecSelectItem.size())
		{
			// 선택보상슬롯
			const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_SelectReward.m_vecSelectItem[i];
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
			//{{10.01.14 김정협 아이템 없을 시 문제 처리
			if(pItemTemplet == NULL)
				continue;
			//}}10.01.14 김정협 아이템 없을 시 문제 처리

			// fix!! 일단 선택보상에서는 unit_type만 체크
			if( pItemTemplet->GetUnitType() != CX2Unit::UT_NONE )
			{
				CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
				if( pItemTemplet->GetUnitType() != eUnitType )
				{
					continue;
				}
			}
			//if(pItemTemplet != NULL)
			{
				// 보상 버튼에 DummyInt로 ItemID를 달아 주자!
				// 더미인트 0 : ItemID, 1 : SlotID
				WCHAR buff[256] = {0,};
				//wsprintf( buff, L"Selection_Reward_Slot_%d", SlotIndex+1);
				StringCchPrintf( buff, 256, L"Selection_Reward_Slot_%d", SlotIndex+1);
				CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( buff );
				pRadioButton->AddDummyInt( itemData.m_iItemID );
				pRadioButton->AddDummyInt( itemData.m_iSocketOption1 );
#ifdef QUEST_REWARD_PERIOD
				pRadioButton->AddDummyInt( itemData.m_iPeriod );
#endif QUEST_REWARD_PERIOD
				pStaticSelectionReward->GetPicture(SlotIndex)->SetTex( 
                    pItemTemplet->GetShopImage()
                    );

				++m_byNumOfItemThatICanGet;

			}
		}
		else
		{
			WCHAR buff[256] = {0,};
			//wsprintf( buff, L"Selection_Reward_Slot_%d", SlotIndex+1);
			StringCchPrintf( buff, 256, L"Selection_Reward_Slot_%d", SlotIndex+1);
			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( buff );
			pRadioButton->SetShowEnable( false, false );
			pRadioButton->AddDummyInt( 0 );
			pRadioButton->AddDummyInt( 0 );
#ifdef QUEST_REWARD_PERIOD
			pRadioButton->AddDummyInt( 0 );
#endif QUEST_REWARD_PERIOD
		}
		SlotIndex++;
	}

	if ( m_byNumOfItemThatICanGet == 1 )	// 보상 받을 수 있는 물품이 한가지인 경우에는 무조건 선택 되도록!!
	{
		// note!! 임시로 첫번째 아이템이 무조건 선택되어 있게
		CKTDGUIRadioButton* pRadioButton1 = (CKTDGUIRadioButton*) m_pDLGUIQuestClear->GetControl( L"Selection_Reward_Slot_1" );
		if(true == pRadioButton1->GetShow())
		{
			pRadioButton1->SetChecked( true );
		}
	}
}

void CX2UIQuestReceive::CloseQuestRewardWindow()
{
	if( m_pDLGUIQuestClear != NULL )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUIQuestClear, NULL, false );
		m_pDLGUIQuestClear = NULL;
	}

	if( m_pDLGMsgBox != NULL )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
		m_pDLGMsgBox = NULL;
	}

	if( m_pDLGQuestRewardItemInfo != NULL )
	{
		OpenRewardItemInfoPopup( false, D3DXVECTOR2(0,0), -1 );
	}


}

#ifdef QUEST_REWARD_PERIOD
void CX2UIQuestReceive::OpenRewardItemInfoPopup( bool bOpen, D3DXVECTOR2 vPos, int iItemID, int socketID , int Period )
#else
void CX2UIQuestReceive::OpenRewardItemInfoPopup( bool bOpen, D3DXVECTOR2 vPos, int iItemID, int socketID )
#endif QUEST_REWARD_PERIOD
{
	//{{ 허상형 : [2009/9/29] //	퀘스트 종료 후에도 툴팁이 남아있는 버그 수정을 위한 조건식 추가
	if( m_pDLGUIQuestClear == NULL && true == bOpen )
	{
		return;
	}

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
			int iLayer = XDL_POP_UP;
			if( m_pDLGUIQuestClear != NULL )
			{
				iLayer = m_pDLGUIQuestClear->GetLayer() + 1;
			}
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGQuestRewardItemInfo, iLayer );
		}
	}
	else
	{
		return;
	}

	if( false == bOpen )
		return;


	CX2Item* pItem = NULL;

	if ( socketID != 0 )
	{
		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );
		if ( pItemTemplet != NULL )
		{
			CX2Item::ItemData kItemData;
			kItemData.m_Endurance = pItemTemplet->GetEndurance();
			kItemData.m_ItemID = iItemID;
#ifdef QUEST_REWARD_PERIOD
			if( Period != 0 )
			{
				kItemData.m_Period = Period;
			}
#endif QUEST_REWARD_PERIOD
			kItemData.m_SocketOption.push_back( socketID );
			pItem = new CX2Item( kItemData, NULL );
		}
	}
#ifdef QUEST_REWARD_PERIOD
	else if( Period != 0 )
	{
		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );
		if ( pItemTemplet != NULL )
		{
			CX2Item::ItemData kItemData;
			kItemData.m_Endurance = pItemTemplet->GetEndurance();
			kItemData.m_ItemID = iItemID;
			kItemData.m_Period = Period;
			pItem = new CX2Item( kItemData, NULL );
		}
	}
#endif QUEST_REWARD_PERIOD



	wstring wstrItemDesc = GetSlotItemDescByTID( pItem, iItemID, false );
	SAFE_DELETE( pItem );

	CKTDGUIStatic* pStatic_ItemDesc = (CKTDGUIStatic*) m_pDLGQuestRewardItemInfo->GetControl(L"ItemDesc");
	if( NULL == pStatic_ItemDesc )
		return; 

	CKTDGUIControl::UIStringData* pString_ItemDesc = pStatic_ItemDesc->GetString(0);
	if( NULL == pString_ItemDesc )
		return;

	CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pString_ItemDesc->fontIndex );
	int itemDescWidth = (int)( pItemDescFont->GetWidth( wstrItemDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + 50;
	int itemDescHeight = (int)( pItemDescFont->GetHeight( wstrItemDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + 50;

	m_pDLGQuestRewardItemInfo->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)itemDescHeight) );
	m_pDLGQuestRewardItemInfo->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );
	m_pDLGQuestRewardItemInfo->GetStatic_LUA( "ItemDesc" )->GetString(0)->msg = wstrItemDesc.c_str();

	D3DXVECTOR2 vDialogPos = vPos;
	vDialogPos.y -= (float) itemDescHeight;
	m_pDLGQuestRewardItemInfo->SetPos( vDialogPos );

}

void CX2UIQuestReceive::UpdateNavi()
{
	if(m_pDLGUIQuestReceive != NULL)
	{
		CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGUIQuestReceive->GetControl( L"Navi_QuestList" );
		if( NULL != pNavi )
		{
			if( (int)m_vecAvailableQuestID.size() <= m_iQuestPerPage )
			{
				pNavi->SetShow(false);
				return;
			}

			pNavi->SetShow(true);

			// 위치 정해주고~
			D3DXVECTOR2 vPos;
			vPos.x = 0.0f;
			// 0.68 : Navi Size에 따른 값. MAGIC VALUE
			vPos.y = (float)m_iTopIndex / ((float)m_vecAvailableQuestID.size() - m_iQuestPerPage ) * 0.68f;

			pNavi->SetRelativeWindowPos( vPos );
		}
	}
}

void CX2UIQuestReceive::UpdateQuestDesc( const wstring& wstrDesc )
{
	// 일단 wstrDesc를 만들어내자-_-
	// 클리어 조건 -> 
	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

	m_vQuestDescSize = D3DXVECTOR2( (float) m_pDLGUIQuestReceive->GetDummyInt( 2 ), (float) m_pDLGUIQuestReceive->GetDummyInt( 3 ) );
	m_vQuestDescSize = g_pKTDXApp->ConvertByResolution( m_vQuestDescSize );

	CKTDGUIStatic* pStatic_QuestDesc = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_Clearcondition1" );
	if( NULL == pStatic_QuestDesc )
		return;

	CKTDGFontManager::CUKFont* pFont 
		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_QuestDesc->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	const int MAGIC_MAX_WIDTH = (int) m_vQuestDescSize.x;

	m_vecQuestDesc.resize(0);

	if( wstrDesc.length() == 0 )
	{
		m_iQuestDescIndex = 0;
		return;
	}

	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowCharIndex = 0;
	wstring wstrLineBuf = L"";
	wstring wstrPageBuf = L"";
	wstring wstrColorCode = L"";

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 

	int iDescLength = (int) wstrDesc.length();
	while( iNowCharIndex < iDescLength )
	{
		WCHAR wszCharBuf = wstrDesc[iNowCharIndex];
		wstring wstrCharBuf = wstrDesc.substr( iNowCharIndex, 1 ); 

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP;
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrPageBuf += wstrLineBuf;
					wstrLineBuf = L"";
					iNowLineWidth = 0;

					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}
					if(false == bIsFirstCharOfLine)
					{
						m_vecQuestDesc.push_back( wstrPageBuf );
					}
					bIsFirstCharOfLine = true;
					wstrPageBuf = L"";

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
				else if( 0 == wstrCharBuf.compare( L"m" ) )
				{
					wstrLineBuf.erase( wstrLineBuf.end()-1 ); // '#'을 지우자
					wstrPageBuf += wstrLineBuf;
					if(false == bIsFirstCharOfLine)
					{
						m_vecQuestDesc.push_back( wstrPageBuf );
					}
					wstrLineBuf = L"";
					wstrPageBuf = L"";
					bIsFirstCharOfLine = true;
					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}

					eNowCharState = Q_NORMAL;
				}
				else
				{
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
					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					wstrColorCode = L"#C";
					wstrColorCode += wstrDesc.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrDesc.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

			} break;
		}

		if( iNowLineWidth >= MAGIC_MAX_WIDTH && Q_NORMAL == eNowCharState )
		{
			wstrLineBuf += L"\n";
			wstrPageBuf += wstrLineBuf;
			wstrLineBuf = L"";
			iNowLineWidth = 0;
			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}

			if(false == bIsFirstCharOfLine)
			{
				m_vecQuestDesc.push_back( wstrPageBuf );
			}
			bIsFirstCharOfLine = true;
			wstrPageBuf = L"";
		}

		iNowCharIndex++;
	}

	if( wstrLineBuf.length() > 0 )
	{
		wstrPageBuf += wstrLineBuf;
	}

	if( wstrPageBuf.length() > 0 )
	{
		m_vecQuestDesc.push_back( wstrPageBuf );
	}

	m_iQuestDescIndex = 0;

}

void CX2UIQuestReceive::UpdateQuestDescPage()
{
	if(m_iQuestDescIndex > (int)m_vecQuestDesc.size() - m_iQuestDescLinePerPage - 1) m_iQuestDescIndex = m_vecQuestDesc.size() - m_iQuestDescLinePerPage - 1;
	if(m_iQuestDescIndex < 0) m_iQuestDescIndex = 0;


	CKTDGUIStatic* pStatic_QuestDesc = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( L"Static_Clearcondition1" );
	if( NULL == pStatic_QuestDesc )
		return;

	CKTDGFontManager::CUKFont* pFont 
		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_QuestDesc->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	m_iQuestDescLinePerPage = (int) (m_vQuestDescSize.y / (float) pFont->GetHeight() * 0.5f);


	wstringstream wstrm;

	if( m_vecQuestDesc.size() == 0 )
	{
		m_iQuestDescIndex = 0;

	}
	else
	{
		int j = 0;
		for(UINT i=m_iQuestDescIndex; i<m_vecQuestDesc.size(); i++, j++)
		{
			wstrm << m_vecQuestDesc[i];
			if(j>m_iQuestDescLinePerPage) break;
		}

	}

	pStatic_QuestDesc->GetString(0)->msg = wstrm.str().c_str();

	if((int)m_vecQuestDesc.size() <= m_iQuestDescLinePerPage)
	{
		CKTDGUIButton* pButtonDescDown = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_QuestDesc_Down" );
		CKTDGUIButton* pButtonDescUp = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_QuestDesc_Up" );
		pButtonDescDown->SetShow(false);
		pButtonDescUp->SetShow(false);
	}
	else
	{
		CKTDGUIButton* pButtonDescDown = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_QuestDesc_Down" );
		CKTDGUIButton* pButtonDescUp = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_QuestDesc_Up" );
		pButtonDescDown->SetShow(true);
		pButtonDescUp->SetShow(true);
	}


}

void CX2UIQuestReceive::ShowNextButton(bool val)
{
	CKTDGUIButton* pButtonNext = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Next" );
	CKTDGUIButton* pButtonGiveup = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Giveup" );
	CKTDGUIButton* pButtonAccept = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Accept" );
	CKTDGUIButton* pButtonComplete = (CKTDGUIButton*) m_pDLGUIQuestReceive->GetControl( L"Button_Complete" );

	if(val)		// NEXT 버튼 보여라
	{
		if(pButtonNext != NULL)
		{
			pButtonNext->SetShow(true);
			pButtonNext->RequestFocus();
		}

		if(pButtonGiveup != NULL)
			pButtonGiveup->SetShow(false);
		if(pButtonAccept != NULL)
			pButtonAccept->SetShow(false);
		if(pButtonComplete != NULL)
			pButtonComplete->SetShow(false);		
	}
	else
	{
		pButtonNext->SetShow(false);

		const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );
		if( NULL != pQuestInst )
		{
			if( true == pQuestInst->IsComplete() )
			{
				// 완료된 퀘스트
				pButtonComplete->SetShow(true);
				pButtonComplete->RequestFocus();
			}
			else
			{
				// 진행중인 퀘스트
				pButtonGiveup->SetShow(true);
				pButtonGiveup->RequestFocus();
			}
		}
		else
		{
			// 안 받은 퀘스트
			pButtonAccept->SetShow(true);
			pButtonAccept->RequestFocus();
		}
	}
}

void CX2UIQuestReceive::UpdateNPCQuestDesc()
{
	// 퀘스트 대사 처리
	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	if( NULL == pQuestTemplet )
		return;
	const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );

	if( NULL == pQuestInst )
	{
		g_pTFieldGame->SetQuestDesc((int)m_eNPCID, pQuestTemplet->m_wstrTitle, pQuestTemplet->m_wstrMainText);
	}
	else
	{
		if( pQuestTemplet->m_eEndNPCID == m_eNPCID )
		{
			if( true == pQuestInst->IsComplete() )
			{
				g_pTFieldGame->SetQuestDesc((int)m_eNPCID, pQuestTemplet->m_wstrTitle, pQuestTemplet->m_wstrThanksText);
			}
			else
			{
				if( pQuestTemplet->m_eStartNPCID == m_eNPCID )
				{
					g_pTFieldGame->SetQuestDesc((int)m_eNPCID, pQuestTemplet->m_wstrTitle, pQuestTemplet->m_wstrDissClearText);
				}
				else
				{
					g_pTFieldGame->SetQuestDesc((int)m_eNPCID, L"", L"");

				}
			}
		}
		else
		{
			g_pTFieldGame->SetQuestDesc((int)m_eNPCID, pQuestTemplet->m_wstrTitle, pQuestTemplet->m_wstrDissClearText);
		}
	}

}

void CX2UIQuestReceive::SelectNextQuest( bool bNext )
{
	if(false == m_vecAvailableQuestID.empty())
	{
		// 		if(m_iPickedQuestID == -1)
		// 		{
		// 			// Empty가 아니니까
		// 			int QuestID = m_vecAvailableQuestID.first();
		// 			SelectQuest(QuestID);
		// 			return;
		// 		}

		vector<int>::iterator it = find(m_vecAvailableQuestID.begin(), m_vecAvailableQuestID.end(), m_iPickedQuestID);
		if(it != m_vecAvailableQuestID.end())
		{
			// 있으면
			if(!bNext && it != m_vecAvailableQuestID.begin())
			{
				if(true == TargetQuestisOnScreenList( m_iPickedQuestID ))
				{
					// 제일 위 퀘스트였으면 한 칸 올리고
					QuestListSlot& refQuestSlot = m_vQuestList[0];
					if( refQuestSlot.m_QuestID == m_iPickedQuestID )
					{
						m_iTopIndex--;
					}

					// 앞의 거 받기 && 첫번째 원소가 아니었으면
					--it;
					SelectQuest(*it);
				}
				else
				{
					SelectQuest(m_vecAvailableQuestID[m_iTopIndex]);
				}
	
				
			}
			else if( bNext && ++it != m_vecAvailableQuestID.end() )
			{
				if(true == TargetQuestisOnScreenList( m_iPickedQuestID ))
				{
					// 제일 아래 퀘스트였으면 한 칸 내리고
					QuestListSlot& refQuestSlot = m_vQuestList[m_iQuestPerPage-1];
					if( refQuestSlot.m_QuestID == m_iPickedQuestID )
					{
						m_iTopIndex++;
					}

					// 다음 거 받기 && 마지막 원소가 아니었으면
					SelectQuest(*it);
				}
				else
				{
					SelectQuest(m_vecAvailableQuestID[m_iTopIndex]);
				}

				

			}
		}
		else
		{
			// 없으면
			//it = m_vecAvailableQuestID.begin();
			//SelectQuest(*it);
			SelectQuest(m_vecAvailableQuestID[m_iTopIndex]);
		}

	}

	return;	
}

void CX2UIQuestReceive::UpdateNewQuestNotice()
{
	// 일단 미니맵이 있는지부터 검사좀하고
	if( g_pInstanceData == NULL ||
		g_pInstanceData->GetMiniMapUI() == NULL )
	{
		return;
	}

	// 준비작업
	const map<int, int>& mapQuestClear = g_pData->GetQuestManager()->GetMapCompleteQuest(); // 이미 완료, 보상받은 퀘스트들  
	int iQuestNum = 0;
	int TopQuestID = 0;
	SEnum::VILLAGE_MAP_ID VID = SEnum::VMI_RUBEN;

	// 자기가 만날 수 있는 NPC의 리스트를 만들어두자.
	// 09.03.17 변경 : 특정 지역에 있을 때 해당하는 NPC로 기획변경.
	// 일단 여기서 NPC 필터링하면 정상작동할테니 그렇게 만들어두고 나서 나중에 코드를 정리하는 쪽으로 가자..
	std::map<CX2UnitManager::NPC_UNIT_ID, SEnum::VILLAGE_MAP_ID> mapNPCID;
	mapNPCID.clear();
	
	for(int LID = (int)CX2LocationManager::LMI_RUBEN; 
		g_pData->GetLocationManager()->GetLocalMapTemplet((CX2LocationManager::LOCAL_MAP_ID)LID) != NULL; 
		++LID)
	{
		// 내가 갈 수 있는 지역인지 테스트
		CX2LocationManager::LocalMapTemplet* pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet((CX2LocationManager::LOCAL_MAP_ID)LID);
		if( pLocalMapTemplet->m_vecRequireClearDungeonID.size() > 0 )
		{
			if( false == g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( pLocalMapTemplet->m_vecRequireClearDungeonID ) ||
				g_pData->GetSelectUnitLevel() < pLocalMapTemplet->m_RequireUnitLevel )
			{
				continue;
			}
		}

#if 0
		// 내가 지금 있는 지역과 목표로 하는 마을이 "가까운지" 테스트
		// 내가 속한 지역의 퀘스트만 받는다.
		if( false == IsNearVillage(g_pData->GetLocationManager()->GetCurrentVillageID(), pLocalMapTemplet->m_VillageID) )
		{
			continue;
		}		
#endif

        // 해서 해당하는 NPC들의 정보를 저장하자.
		CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( pLocalMapTemplet->m_VillageID );
		if(pVillageTemplet != NULL)
		{
			for(std::vector<CX2LocationManager::HOUSE_ID>::iterator it = pVillageTemplet->m_HouseList.begin(); it < pVillageTemplet->m_HouseList.end(); it++)
			{
				CX2LocationManager::HouseTemplet* pHouseTemplet = g_pData->GetLocationManager()->GetHouseTemplet(*it);
				if(pHouseTemplet != NULL)
				{
					for(std::vector<CX2UnitManager::NPC_UNIT_ID>::iterator itNPC = pHouseTemplet->m_NPCList.begin(); itNPC < pHouseTemplet->m_NPCList.end(); itNPC++)
					{
						mapNPCID.insert(std::make_pair(*itNPC, pLocalMapTemplet->m_VillageID) );
					}
				}			
			}
		}
	}
	
	


	//전직퀘스트 중에서 한쪽은 안보이게 하자
	const CX2QuestManager::ChangeJobQuest& changeJobQuest = 
		g_pData->GetQuestManager()->GetChangeJobQuest( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );


	// 가지고 있거나 완료한 전직 퀘스트중에서 해당하는 것을 찾는다.
	bool bHasClassChangeQuest[ CX2QuestManager::CJT_JOB_END ] = { false };
	for( int i=CX2QuestManager::CJT_JOB_A; i<CX2QuestManager::CJT_JOB_END; i++ )
	{
		const std::set<int>& setClassChangeQuestID = changeJobQuest.m_setQuestID[ CX2QuestManager::CHANGE_JOB_TYPE(i) ];
		BOOST_TEST_FOREACH( int, iClassChangeQuestID, setClassChangeQuestID )
		{
			if( NULL != g_pData->GetQuestManager()->GetUnitQuest( iClassChangeQuestID ) ||
				mapQuestClear.end() != mapQuestClear.find( iClassChangeQuestID ) )
			{
				bHasClassChangeQuest[ CX2QuestManager::CHANGE_JOB_TYPE(i) ] = true;
				break;
			}
		}
	}

	CX2QuestManager::CHANGE_JOB_TYPE ePickedChangeJobType = CX2QuestManager::CJT_NONE;
	if( true == bHasClassChangeQuest[CX2QuestManager::CJT_JOB_A] )
	{
		ePickedChangeJobType = CX2QuestManager::CJT_JOB_A;
	}
	else if( true == bHasClassChangeQuest[CX2QuestManager::CJT_JOB_B] )
	{
		ePickedChangeJobType = CX2QuestManager::CJT_JOB_B;
	}
	else if( true == bHasClassChangeQuest[CX2QuestManager::CJT_JOB_D] )
	{
		ePickedChangeJobType = CX2QuestManager::CJT_JOB_D;
	}
	else if( true == bHasClassChangeQuest[CX2QuestManager::CJT_JOB_E] )
	{
		ePickedChangeJobType = CX2QuestManager::CJT_JOB_E;
	}
	else if( true == bHasClassChangeQuest[CX2QuestManager::CJT_JOB_C] )
	{
		ePickedChangeJobType = CX2QuestManager::CJT_JOB_C;
	}
	else if( true == bHasClassChangeQuest[CX2QuestManager::CJT_JOB_F] )
	{
		ePickedChangeJobType = CX2QuestManager::CJT_JOB_F;
	}
	else if( true == bHasClassChangeQuest[CX2QuestManager::CJT_JOB_G] )
	{
		ePickedChangeJobType = CX2QuestManager::CJT_JOB_G;
	}


	// 접근 가능한 모든 퀘스트에 대해-_-;
	const map<int, CX2QuestManager::QuestTemplet*> vMapQuest = g_pData->GetQuestManager()->GetMapQuestTemplet();
	map<int, CX2QuestManager::QuestTemplet*>::const_iterator it;
	for( it = vMapQuest.begin(); it != vMapQuest.end(); it++ )
	{
		int QuestID = it->first;
		CX2QuestManager::QuestTemplet* pQuestTemplet = (CX2QuestManager::QuestTemplet*) it->second;
		if(pQuestTemplet == NULL) continue;

#ifdef ADD_SERVER_GROUP
		if( pQuestTemplet->m_iServerGroupID != -1 && pQuestTemplet->m_iServerGroupID != g_pInstanceData->GetServerGroupID() )
			continue;
#endif

		// 0. 이벤트 퀘스트는 뺀다.
		if( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_EVENT
			//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
			|| pQuestTemplet->m_eQuestType == CX2QuestManager::QT_PCBANG
#endif	PC_BANG_QUEST
			//}} kimhc // 2010.2.1 //	PC방 퀘스트
			)
			continue;
	
		// 1. 이미 받은 퀘스트면 뺀다.
		if(g_pData->GetQuestManager()->GetUnitQuest(QuestID) != NULL)
			continue;

		// 2. 접근 가능한 퀘스트인지 확인한다.
		if( false == CanIAcceptQuest(QuestID) )
			continue;

		// 3. 이미 클리어한 퀘스트면 뺀다.
		if(mapQuestClear.find(QuestID) != mapQuestClear.end())
			continue;
				
		// 4. 클리어하지 않은 쪽의 전직 퀘스트는 뺀다.
		switch( ePickedChangeJobType )
		{
		case CX2QuestManager::CJT_JOB_A:
			{
				// B, C, D, E 퀘스트는 빼자
				bool bFound = false;
				for( int j=CX2QuestManager::CJT_JOB_B; j<=CX2QuestManager::CJT_JOB_E; j++ )
				{
					const std::set<int>& setClassChangeQuestID = changeJobQuest.m_setQuestID[ CX2QuestManager::CHANGE_JOB_TYPE(j) ];
					if( setClassChangeQuestID.find( QuestID ) != setClassChangeQuestID.end() )
					{
						bFound = true;
						break;
					}
				}

				if( true == bFound )
					continue;

			} break;

		case CX2QuestManager::CJT_JOB_B:
			{
				// A, C, D, E 퀘스트는 빼자
				const std::set<int>& setClassChangeQuestID_A = changeJobQuest.m_setQuestID[ CX2QuestManager::CJT_JOB_A ];
				if( setClassChangeQuestID_A.find( QuestID ) != setClassChangeQuestID_A.end() )
				{
					continue;
				}

				bool bFound = false;
				for( int j=CX2QuestManager::CJT_JOB_C; j<=CX2QuestManager::CJT_JOB_E; j++ )
				{
					const std::set<int>& setClassChangeQuestID = changeJobQuest.m_setQuestID[ CX2QuestManager::CHANGE_JOB_TYPE(j) ];
					if( setClassChangeQuestID.find( QuestID ) != setClassChangeQuestID.end() )
					{
						bFound = true;
						break;
					}
				}
				if( true == bFound )
					continue;

			} break;

		case CX2QuestManager::CJT_JOB_C:
			{
				// A, B 퀘스트는 빼자
				bool bFound = false;
				for( int j=CX2QuestManager::CJT_JOB_A; j<=CX2QuestManager::CJT_JOB_B; j++ )
				{
					const std::set<int>& setClassChangeQuestID = changeJobQuest.m_setQuestID[ CX2QuestManager::CHANGE_JOB_TYPE(j) ];
					if( setClassChangeQuestID.find( QuestID ) != setClassChangeQuestID.end() )
					{
						bFound = true;
						break;
					}
				}	
				if( true == bFound )
					continue;

			} break;

		case CX2QuestManager::CJT_JOB_D:
			{
				// A, B, E 퀘스트는 빼자
				bool bFound = false;
				for( int j=CX2QuestManager::CJT_JOB_A; j<=CX2QuestManager::CJT_JOB_B; j++ )
				{
					const std::set<int>& setClassChangeQuestID = changeJobQuest.m_setQuestID[ CX2QuestManager::CHANGE_JOB_TYPE(j) ];
					if( setClassChangeQuestID.find( QuestID ) != setClassChangeQuestID.end() )
					{
						bFound = true;
						break;
					}
				}
				if( true == bFound )
					continue;

				const std::set<int>& setClassChangeQuestID_E = changeJobQuest.m_setQuestID[ CX2QuestManager::CJT_JOB_E ];
				if( setClassChangeQuestID_E.find( QuestID ) != setClassChangeQuestID_E.end() )
				{
					continue;
				}

			} break;			

		case CX2QuestManager::CJT_JOB_E:
			{
				// A, B, D 퀘스트는 빼자
				bool bFound = false;
				for( int j=CX2QuestManager::CJT_JOB_A; j<=CX2QuestManager::CJT_JOB_B; j++ )
				{
					const std::set<int>& setClassChangeQuestID = changeJobQuest.m_setQuestID[ CX2QuestManager::CHANGE_JOB_TYPE(j) ];
					if( setClassChangeQuestID.find( QuestID ) != setClassChangeQuestID.end() )
					{
						bFound = true;
						break;
					}
				}
				if( true == bFound )
					continue;

				const std::set<int>& setClassChangeQuestID_D = changeJobQuest.m_setQuestID[ CX2QuestManager::CJT_JOB_D ];
				if( setClassChangeQuestID_D.find( QuestID ) != setClassChangeQuestID_D.end() )
				{
					continue;
				}

			} break;			


		case CX2QuestManager::CJT_JOB_F:
			{
				// G 퀘스트는 빼자
				const std::set<int>& setClassChangeQuestID_G = changeJobQuest.m_setQuestID[ CX2QuestManager::CJT_JOB_G ];
				if( setClassChangeQuestID_G.find( QuestID ) != setClassChangeQuestID_G.end() )
				{
					continue;
				}

			} break;


		case CX2QuestManager::CJT_JOB_G:
			{
				// F 퀘스트는 빼자
				const std::set<int>& setClassChangeQuestID_F = changeJobQuest.m_setQuestID[ CX2QuestManager::CJT_JOB_F ];
				if( setClassChangeQuestID_F.find( QuestID ) != setClassChangeQuestID_F.end() )
				{
					continue;
				}

			} break;

		}


		




		// 5. 레벨이 너무 낮으면 뺀다. (전직 퀘스트 제외)
		if( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_NORMAL || pQuestTemplet->m_eQuestType == CX2QuestManager::QT_SKILL )
		{
			int iLevelGap = g_pData->GetSelectUnitLevel() - pQuestTemplet->m_iFairLevel;
			if( iLevelGap > 5 )
			{
				continue;
			}
		}

		// 6. 해당 NPC가 있는 마을에 갈 수 있는지 검사한다.
		std::map<CX2UnitManager::NPC_UNIT_ID, SEnum::VILLAGE_MAP_ID>::iterator itNPCID = mapNPCID.find(pQuestTemplet->m_eStartNPCID);
		if(itNPCID == mapNPCID.end())
		{
			continue;
		}




				
		
		// 7. 그 수와 최고레벨 퀘스트를 구한다.
		const CX2QuestManager::QuestTemplet* pTopQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( TopQuestID );
		iQuestNum++;
		if(pTopQuestTemplet == NULL || pQuestTemplet->m_iFairLevel >= pTopQuestTemplet->m_iFairLevel )
		{
			TopQuestID = QuestID;
			VID = itNPCID->second;
		}
		
	}

	// 8. 미니맵 쪽으로 쏴준다.	(포인터 유효성은 시작할 때 검사)

}

bool CX2UIQuestReceive::CanIAcceptQuest( int iQuestID )
{

	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( iQuestID );
	if( NULL == pQuestTemplet )
		return false;


	if( true == g_pData->GetQuestManager()->IsForbiddenQuest( iQuestID ) )
		return false;


	for( int iC = 0; iC < (int)pQuestTemplet->m_Condition.m_vecBeforeQuestID.size(); ++iC )
	{
		if( pQuestTemplet->m_Condition.m_vecBeforeQuestID[iC] != 0 &&
			false == g_pData->GetQuestManager()->GetUnitCompleteQuest( pQuestTemplet->m_Condition.m_vecBeforeQuestID[iC] ) )
		{
			return false;
		}
	}

	if( pQuestTemplet->m_Condition.m_iLevel != 0 &&
		g_pData->GetSelectUnitLevel() < pQuestTemplet->m_Condition.m_iLevel )  
	{
		return false;
	}

	if( -1 != pQuestTemplet->m_Condition.m_iConditionItemID && pQuestTemplet->m_Condition.m_iConditionItemNum > 0 )
	{
		int iItemCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( pQuestTemplet->m_Condition.m_iConditionItemID );
		if( iItemCount < pQuestTemplet->m_Condition.m_iConditionItemNum )
		{
			return false;
		}
	}



	// 피해야할 퀘스트를 완료한 퀘스트는 받을 수 없다
	if( -1 != pQuestTemplet->m_Condition.m_iNotBeforeQuestID )
	{
		const map<int, int>& mapQuestClear = g_pData->GetQuestManager()->GetMapCompleteQuest();			// 이미 완료, 보상 받은 퀘스트
		if( mapQuestClear.end() != mapQuestClear.find( pQuestTemplet->m_Condition.m_iNotBeforeQuestID ) )
		{
			return false;
		}
	}



	return true;
}

bool CX2UIQuestReceive::IsNearVillage( int CurrentVillageID, int TargetVillageID )	// 퀘스트 알리미에서 "PC가 속한 지역"인지를 확인하는 함수.
{
	// 주의 : 새 마을이 추가되면 이 부분을 수정해 줘야 합니다.
	//** 나중에 스크립트로 빼서 초기화할 때 읽어오게 하자!
	if(m_mulmapNearVillageData.empty())
	{
		// 현재 묶인 단위 :
		//  (루벤마을,루벤외곽)
		//	(엘더마을,엘더외곽)
		//	(베스마마을,베스마산맥,페이타)
		//	(알테라마을,알테라섬,페이타)
		// CurrentVillageID랑 TargetVillageID가 서로 묶여있으면 return true
		// 퀘스트 주는 곳은 마을 뿐이니까 마을 :: 던전게이트 pair로 묶어준다

		//  (루벤마을,루벤외곽)
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_RUBEN, (int)SEnum::VMI_DUNGEON_GATE_RUBEN));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_RUBEN, (int)SEnum::VMI_DUNGEON_LOUNGE_RUBEN));
		
		//	(엘더마을,엘더외곽)
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_ELDER, (int)SEnum::VMI_DUNGEON_GATE_ELDER));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_ELDER, (int)SEnum::VMI_DUNGEON_LOUNGE_ELDER));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_ELDER, (int)SEnum::VMI_BATTLE_FIELD_ELDER_REST_00));

		//	(베스마마을,베스마산맥,페이타)
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_BESMA, (int)SEnum::VMI_DUNGEON_GATE_BESMA));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_BESMA, (int)SEnum::VMI_PEITA));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_BESMA, (int)SEnum::VMI_DUNGEON_LOUNGE_BESMA));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_BESMA, (int)SEnum::VMI_DUNGEON_LOUNGE_PEITA));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_BESMA, (int)SEnum::VMI_BATTLE_FIELD_BESMA_REST_00));
		//m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_PEITA, (int)SEnum::VMI_PEITA));

		//	(알테라마을,알테라섬,페이타)
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_ALTERA, (int)SEnum::VMI_DUNGEON_GATE_ALTERA));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_ALTERA, (int)SEnum::VMI_PEITA));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_ALTERA, (int)SEnum::VMI_DUNGEON_LOUNGE_ALTERA));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_ALTERA, (int)SEnum::VMI_DUNGEON_LOUNGE_PEITA));
		//m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_DUNGEON_GATE_ALTERA, (int)SEnum::VMI_PEITA));

		//  (벨더근교,벨더수도)
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_VELDER, (int)SEnum::VMI_DUNGEON_GATE_VELDER));
		m_mulmapNearVillageData.insert( std::make_pair((int)SEnum::VMI_VELDER, (int)SEnum::VMI_DUNGEON_LOUNGE_VELDER));
	}

	// 두 ID가 같으면 무조건 맞고
	if( CurrentVillageID == TargetVillageID )
	{
		return true;
	}

	// 한 VID로 다른 VID와 매치되는지 검사
	// 위에서 호출할 때 마을 ID가 앞에 오는 경우가 많으니 이 검사를 아래쪽 것보다 먼저 하게 하자
	{
		std::pair<std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> range = m_mulmapNearVillageData.equal_range(TargetVillageID);
		for (std::multimap<int,int>::iterator it = range.first; it != range.second; ++it)
		{
			if(it->second == CurrentVillageID)
				return true;
		}
	}
	
	// 반대로도 검사
	{
		std::pair<std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> range = m_mulmapNearVillageData.equal_range(CurrentVillageID);
		for (std::multimap<int,int>::iterator it = range.first; it != range.second; ++it)
		{
			if(it->second == TargetVillageID)
				return true;
		}
	}
	
	// 같은 묶음이 아니다
	return false;
}


wstring CX2UIQuestReceive::CreateQuestDesc()
{
	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );

	wstringstream wstrm;

	// 퀘스트 완료조건 넣는 부분
	if( pQuestTemplet != NULL )
	{
		// 선택된 퀘가 있으면~
		bool bThereWasNPCTalkSubQuest = false;

		// sub-quest
		for( UINT i=0; i<pQuestTemplet->m_vecSubQuest.size(); i++ )
		{
			const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );
			if( NULL == pSubQuestTemplet )
				continue;

			const CX2QuestManager::ClearCondition& clearCond = pSubQuestTemplet->m_ClearCondition;
			if( CX2QuestManager::SQT_NPC_TALK == pSubQuestTemplet->m_eClearType )
			{
				bThereWasNPCTalkSubQuest = true;
			}

			wstring wstrSubQuestDesc = L"";
			wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription, pSubQuestTemplet, pQuestInst );

			if( 0 != wstrSubQuestDesc.length() )
			{
				if( 
#ifndef FIX_TALK_QUEST_COMPLETE_COLOR
					CX2QuestManager::SQT_NPC_TALK != pSubQuestTemplet->m_eClearType &&
#endif
					NULL != pQuestInst && true == pQuestInst->GetSubQuest(pQuestTemplet->m_vecSubQuest[i])->IsComplete() )
				{
					// 완료된 거
					wstrm << L"#CAA0000" << wstrSubQuestDesc << L"#CX\n";
				}
				else
				{
					// 아~직
					wstrm << L"#C281CC0" << wstrSubQuestDesc << L"#CX\n";;
				}
			}
		}
	}

	return wstrm.str().c_str();
}

bool CX2UIQuestReceive::MouseOnQuestListArea( D3DXVECTOR2 mousePos )
{
	// 퀘스트 목록의 위치와 크기
	D3DXVECTOR2 m_ConvertPos	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(15, 75) + m_MovedPosition );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(409, 172) );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;

}

bool CX2UIQuestReceive::MouseOnQuestCommentArea( D3DXVECTOR2 mousePos )
{
	// 퀘스트 내용 부분의 위치와 크기
	D3DXVECTOR2 m_ConvertPos	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(15, 270) + m_MovedPosition );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(406, 60) );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;


}

bool CX2UIQuestReceive::TargetQuestisOnScreenList( int TargetQuestID )
{
	for(int i=0; i<m_iQuestPerPage; ++i)
	{
		int NowIndex = m_iTopIndex+i;
		if( NowIndex > (int)m_vecAvailableQuestID.size() )
			return false;

		int QuestID = m_vecAvailableQuestID[NowIndex];
		if(QuestID == TargetQuestID)
			return true;
	}

	return false;
}
#endif SERV_EPIC_QUEST
