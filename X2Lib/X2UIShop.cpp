#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI

#include ".\X2UIShop.h"

CX2UIShop::CX2UIShop( CKTDXStage* pNowStage, const WCHAR* pFileName )
// : CX2UIDragable( pNowStage, NULL )
: CX2ItemSlotManager( pNowStage, NULL ),
m_bShow(false),
m_MovedPosition(0,0),					// D3DXVECTOR2
m_HouseID(CX2LocationManager::HI_INVALID),
m_DLGPosition(0,0),						// D3DXVECTOR2
m_DLGSize(0,0),							// D3DXVECTOR2
m_pDLGShop(NULL),
m_pDLGSellItemNum(NULL),
m_pDLGSellItemConfirm(NULL),
m_pDLGRepairItemConfirm(NULL),
m_pDLGBuyItemNum(NULL),
m_pDLGBuyItemConfirm(NULL),
m_pDLGEnchantItem(NULL),
m_pDLGEnchantItemResult(NULL),
m_pDLGAttribEnchantItem(NULL),
m_pDLGAttribEnchantItemConfirm(NULL),
//m_vecBuySlot.clear();
m_nNowPage(1),
m_nMaxPage(1),
m_bShowAllCharItem(false),
m_BuyItemNum(0),
m_BuyItemSlotNum(-1),
//m_vecItemList[i].clear();
m_NowBuyItemCategory(BIC_COUNT),
m_SellItemUID(0),
m_SellItemNum(0),
m_bQuickSell(false),
m_RepairItemUID(0),
//m_vecAllEquippingItem.clear();
m_EnchantItemUID(0),
m_bUseEnchantAdjuvant(false),
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	m_bUseEnchantPlus(false),
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}
	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	m_bUseDestroyGuard(false),
#endif // SERV_DESTROY_GUARD_ITEM
	//}}
m_EnchantLevelBefore(0),
m_AttributeEnchantItemUID(0),
m_AttribEnchantID(-1),
m_AttribEnchantSlotID(-1),
m_SumDelta(0)
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT	
, m_iAddEnchantLevel(0)	/// MAGIC_ENCHANT_LEVEL_LIMIT 기본 강화 제한 레벨의 추가 값(이벤트 용)
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
{
	m_vecBuySlot.clear();
	for(int i=0; i<BIC_COUNT; ++i)
	{
		m_vecItemList[i].clear();
	}
	m_vecAllEquippingItem.clear();

	//////////////////////////////////////////////////////////////////////////
	
}


CX2UIShop::~CX2UIShop(void)
{
	for(UINT i = 0; i<m_vecBuySlot.size(); i++)
	{
		BuySlot* pBuySlot = m_vecBuySlot[i];
		SAFE_DELETE(pBuySlot);
	}
	m_vecBuySlot.clear();

	for( UINT iS = 0; iS < m_SlotList.size(); iS++ )
	{
#ifdef SERV_CATCH_HACKUSER_INFO
		{
			std::wstringstream strstm;
			strstm << " i = " << iS << " j = " << m_SlotList.size();
			ErrorLogMsg( 0, strstm.str().c_str() );
		}

		if( m_SlotList.size() > 1000000 )
		{
			KEGS_CATCH_HACKUSER_INFO_NOT kPacket;
			kPacket.m_iUserUID = g_pData->GetMyUser()->GetUID();
			kPacket.m_iUnitUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
			kPacket.m_iCrashType = 2;

			Handler_EGS_CATCH_HACKUSER_INFO_NOT(kPacket);
		}		
#endif SERV_CATCH_HACKUSER_INFO

		CX2SlotItem* pkSlotItem = (CX2SlotItem*)m_SlotList[iS];
		if ( pkSlotItem != NULL )
		{
			pkSlotItem->DestroyItemUI();
			pkSlotItem->SetShow(false);
			SAFE_DELETE(pkSlotItem);
		}
	}
	m_SlotList.clear();

	SAFE_DELETE_DIALOG(m_pDLGSellItemNum);
	SAFE_DELETE_DIALOG(m_pDLGSellItemConfirm);
	SAFE_DELETE_DIALOG(m_pDLGRepairItemConfirm);
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	if( m_pDLGShop != NULL )
	{
		SAFE_DELETE_DIALOG(m_pDLGShop);
	}
#else
	SAFE_DELETE_DIALOG(m_pDLGShop);
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	SAFE_DELETE_DIALOG(m_pDLGBuyItemNum);
	SAFE_DELETE_DIALOG(m_pDLGBuyItemConfirm);
	
	SAFE_DELETE_DIALOG(m_pDLGEnchantItem);
	SAFE_DELETE_DIALOG(m_pDLGEnchantItemResult);

	SAFE_DELETE_DIALOG(m_pDLGAttribEnchantItem);
	SAFE_DELETE_DIALOG(m_pDLGAttribEnchantItemConfirm);

}



HRESULT CX2UIShop::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();


	if( m_pDLGShop != NULL && m_pDLGShop->GetIsMouseOver() == true )
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

bool CX2UIShop::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
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
			if ( false == m_pDLGShop->GetIsMouseOver())
			{
				m_pDLGSelectedItem->SetShow( false );
				InvalidSlotDesc();
				return false;
			}

			bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDLGShop->GetIsMouseOver())
			{
				m_pDLGSelectedItem->SetShow( false );
				InvalidSlotDesc();
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_SHOP, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}

	POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
	D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
	
	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			// 다이얼로그 자동 클릭되는 현상을 막자
			m_NowMousePos = mousePos;
		}
		break;
	case WM_MOUSEMOVE:
		{
			m_NowMousePos = mousePos;
			MouseMove( mousePos );
		}
		break;

	case WM_RBUTTONUP:
		{
			if ( true == MouseRButtonUp( mousePos ) ) return true;
			m_NowMousePos = mousePos;
		}

	}

	return bFlag;
}

bool CX2UIShop::MouseRButtonUp( D3DXVECTOR2 mousePos )
{
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	if( (CX2LocationManager::HOUSE_ID)m_HouseID == CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
	{
		return false;
	}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	CX2SlotItem* pSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if(pSlot == NULL) return false;
	
	// 슬롯 마우스오버 이미지를 지워주고
	m_pDLGSelectedItem->SetShow( false );
	InvalidSlotDesc();

	m_BuyItemSlotNum = pSlot->GetSlotID();
	BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];
	if(pSlotInfo->m_pItemTemplet == NULL) return false;

	//{{ kimhc // 2009-09-16 // 길드 창단 아이템 구입
#ifdef	GUILD_MANAGEMENT

    DWORD   dwItemID = pSlotInfo->m_pItemTemplet->GetItemID();

	// 길드 창단허가서를 구입하려고 하는데 살수 있는 조건을 만족 하지 않으면 return false 됨
	if ( dwItemID == GUILD_CREATE_ITEM_ID &&
		 CanBuyItemToCreateGuild() == false )
		 return false;

	// 길드 인원 확장권을 구입 할수있는 조건을 만족 하지 않으면 false
	if ( dwItemID == GUILD_EXPANSION_ITEM_ID &&
		CanBuyItemToExpandGuild() == false )
		return false;
#ifdef EVENT_GUILD_ITEM
	// 길드 인원 확장권을 구입 할수있는 조건을 만족 하지 않으면 false
	if ( dwItemID == EVENT_GUILD_EXPANSION_ITEM_ID &&
		CanBuyItemToExpandGuild() == false )
		return false;
#endif //EVENT_GUILD_ITEM
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-09-16 // 길드 창단 아이템 구입


	// 수량성인지 아닌지 봐서 수량성이면
	if( CX2Item::PT_QUANTITY == pSlotInfo->m_pItemTemplet->GetPeriodType() )
	{
		// 수량을 결정하자
		OpenItemBuyNumDLG( g_pKTDXApp->MouseConvertByResolution( mousePos ) );
	}
	else	// 아니면
	{
		m_BuyItemNum = 1;		// 1개 사자
		OpenBuyItemConfirmDLG();
	}

	return true;
	
}

bool CX2UIShop::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case USCM_EXIT:
		{			
			SetShow(false);
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
			if( g_pInstanceData != NULL )
			{
				g_pInstanceData->SetAdamsEventShopUIShow(false); //상점 UI가 꺼짐!!!
				g_pData->GetUIManager()->SetShowPartyMenu(true);
				g_pData->GetUIManager()->SetShowQucikQuest(true);
				CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( NULL != pNowState )
					pNowState->SetEnableShortCutKey(true);
			}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
			if(g_pTFieldGame != NULL)
			{
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
				CX2TFieldNpc *pJoinNpc = NULL;
				if( (CX2LocationManager::HOUSE_ID)m_HouseID != CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
				{
					pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
				}
				else
				{
					int iTempHouseID = (int)CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP;
					pJoinNpc = g_pTFieldGame->GetHouseFieldNPC(iTempHouseID);
				}
#else
				CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
				if(pJoinNpc != NULL)
				{
					pJoinNpc->GetNpcShop()->SetKeyEvent();
				}
			}
		}
		return true;
	case USCM_CATEGORY_WEAPON:
		{
			m_NowBuyItemCategory = BIC_WEAPON;
			ResetBuySlotList( 1 );

		}
		return true;
	case USCM_CATEGORY_COAT:
		{
			m_NowBuyItemCategory = BIC_COAT;
			ResetBuySlotList( 1 );
		}
		return true;
	case USCM_CATEGORY_PANTS:
		{
			m_NowBuyItemCategory = BIC_TROUSERS;
			ResetBuySlotList( 1 );
		}
		return true;
	case USCM_CATEGORY_GLOVE:
		{
			m_NowBuyItemCategory = BIC_HAND;
			ResetBuySlotList( 1 );
		}
		return true;
	case USCM_CATEGORY_SHOES:
		{
			m_NowBuyItemCategory = BIC_FOOTWEAR;
			ResetBuySlotList( 1 );
		}
		return true;
	case USCM_CATEGORY_SPECIAL:
		{
			m_NowBuyItemCategory = BIC_SPECIAL;
			ResetBuySlotList( 1 );
		}
		return true;
	case USCM_CATEGORY_ACCESSORY:
		{
			m_NowBuyItemCategory = BIC_ACCESSORY;
			ResetBuySlotList( 1 );
		}
		return true;
	case USCM_CATEGORY_QUICKSLOT:
		{
			m_NowBuyItemCategory = BIC_QUICK_SLOT;
			ResetBuySlotList( 1 );
		}
		return true;

	case USCM_SHOW_OTHER_CHARACTER_EQUIP_ON:
		{
			if(m_bShowAllCharItem == false)
			{
				m_bShowAllCharItem = true;
				BuyUISetting();
				ResetBuySlotList( 1 );

			}
		}
		return true;
	case USCM_SHOW_OTHER_CHARACTER_EQUIP_OFF:
		{
			if(m_bShowAllCharItem == true)
			{
				m_bShowAllCharItem = false;
				BuyUISetting();
				ResetBuySlotList( 1 );
			}

		}
		return true;
	case USCM_PREV_PAGE:
		{
			if( m_nNowPage > 1 ) m_nNowPage--;
			ResetBuySlotList( m_nNowPage );
		}
		return true;
	case USCM_NEXT_PAGE:
		{
			if( m_nNowPage < m_nMaxPage ) m_nNowPage++;
			ResetBuySlotList( m_nNowPage );
		}
		return true;
		//////////////////////////////////////////////////////////////////////////
	case USCM_BUY:
		{

			// 커서 보통으로 바꿔주자
			CX2State* pState = (CX2State*)g_pMain->GetNowState();	// DownCast니까 안심
			if(pState != NULL)
			{
				CX2Cursor* pCursor = pState->GetCursor();
				if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
				{
					pCursor->ChangeCursorState(CX2Cursor::XCS_NORMAL);
				}
			}

			CKTDGUIControl* pBuyButton = (CKTDGUIControl*)lParam;
			if ( pBuyButton != NULL && m_pDLGShop != NULL )
			{
				m_BuyItemSlotNum = pBuyButton->GetDummyInt(0);
				BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];
				
				//{{ kimhc // 2009-09-16 // 길드 창단 아이템 구입
#ifdef	GUILD_MANAGEMENT

				if ( pSlotInfo->m_pItemTemplet == NULL )
				{
					ASSERT( !"ItemTemplet is NULL" );
					return false;
				}

                DWORD   dwItemID = pSlotInfo->m_pItemTemplet->GetItemID();

				// 길드 창단허가서를 구입하려고 하는데 살수 있는 조건을 만족 하지 않으면 return false 됨
				if ( dwItemID == GUILD_CREATE_ITEM_ID &&
					CanBuyItemToCreateGuild() == false )
					return true;		

				if ( dwItemID == GUILD_EXPANSION_ITEM_ID &&
					CanBuyItemToExpandGuild() == false )
					return true;

#ifdef EVENT_GUILD_ITEM
				if ( dwItemID == EVENT_GUILD_EXPANSION_ITEM_ID &&
					CanBuyItemToExpandGuild() == false )
					return true;
#endif //EVENT_GUILD_ITEM
#endif	GUILD_MANAGEMENT
				//}} kimhc // 2009-09-16 // 길드 창단 아이템 구입

				// 수량성인지 아닌지 봐서 수량성이면
				if( CX2Item::PT_QUANTITY == pSlotInfo->m_pItemTemplet->GetPeriodType() )
				{
					// 수량을 결정하자
					OpenItemBuyNumDLG( pSlotInfo->m_pBuyButton->GetPos() + m_pDLGShop->GetPos() );
				}
				else	// 아니면
				{
					m_BuyItemNum = 1;		// 1개 사자
					OpenBuyItemConfirmDLG();
				}

				return true;
			}

		}
		return true;
	case USCM_BUY_ITEM_CONFIRM_OK:
		{
			if(m_pDLGBuyItemConfirm != NULL) 
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyItemConfirm, NULL, false );
				m_pDLGBuyItemConfirm = NULL;
				return Handler_EGS_BUY_ED_ITEM_REQ();
			}
		}
		return true;
	case USCM_BUY_ITEM_CONFIRM_CANCLE:
		{
			if ( m_pDLGBuyItemConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyItemConfirm, NULL, false );

			m_pDLGBuyItemConfirm = NULL;

			m_BuyItemNum = 0;
			m_BuyItemSlotNum = -1;

		}
		return true;
	case USCM_BUY_ITEM_QUANTITY_CONFIRM:
		{
			if ( m_pDLGBuyItemNum != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyItemNum, NULL, false );
				m_pDLGBuyItemNum = NULL;
				OpenBuyItemConfirmDLG();
			}

						
		}
		return true;
	case USCM_BUY_ITEM_QUANTITY_CANCLE:
		{
			if ( m_pDLGBuyItemNum != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyItemNum, NULL, false );

			m_pDLGBuyItemNum = NULL;

			m_BuyItemNum = 0;
			m_BuyItemSlotNum = -1;
		}
		return true;
	case USCM_BUY_ITEM_QUANTITY_PLUS:
		{
			m_BuyItemNum++;
			UpdateItemBuyNumDLG( false );
					}
		return true;
	case USCM_BUY_ITEM_QUANTITY_MINUS:
		{
			m_BuyItemNum--;
			UpdateItemBuyNumDLG( false );
		}
		return true;
	case USCM_BUY_ITEM_QUANTITY_UPDATE:
		{
			UpdateItemBuyNumDLG( true );
		}
		return true;
		//////////////////////////////////////////////////////////////////////////
	case USCM_SELL_ITEM_CONFIRM_OK:
		{
			return Handler_EGS_SELL_ITEM_REQ();
		}
		break;
	case USCM_SELL_ITEM_CONFIRM_CANCLE:
		{
			if ( m_pDLGSellItemConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSellItemConfirm, NULL, false );

			m_pDLGSellItemConfirm = NULL;

			m_SellItemNum = 0;
			m_SellItemUID = 0;

			return true;			
		}
		break;
	case USCM_SELL_ITEM_QUANTITY_CONFIRM:
		{
			if ( m_pDLGSellItemNum != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSellItemNum, NULL, false );

			m_pDLGSellItemNum = NULL;

			OpenSellItemConfirmDLG();
			return true;
		}
		break;
	case USCM_SELL_ITEM_QUANTITY_CANCLE:
		{
			if ( m_pDLGSellItemNum != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSellItemNum, NULL, false );

			m_pDLGSellItemNum = NULL;

			m_SellItemNum = 0;
			m_SellItemUID = 0;
			return true;
		}
		break;
	case USCM_SELL_ITEM_QUANTITY_PLUS:
		{
			m_SellItemNum++;
			UpdateItemSellNumDLG( false );
			return true;
		}
		break;
	case USCM_SELL_ITEM_QUANTITY_MINUS:
		{
			m_SellItemNum--;
			UpdateItemSellNumDLG( false );
			return true;
		}
		break;
	case USCM_SELL_ITEM_QUANTITY_UPDATE:
		{
			UpdateItemSellNumDLG( true );
			return true;
		}
		break;		

		//////////////////////////////////////////////////////////////////////////

	case USCM_REPAIR_ITEM_CONFIRM_OK:
		{
			return Handler_EGS_REPAIR_ITEM_REQ( m_RepairItemUID );
		}
		break;
	case USCM_REPAIR_ITEM_ALL:
		{
			RepairAllEquippedItem();
			return true;
		}
		break;
	case USCM_REPAIR_ITEM_ALL_CONFIRM_OK:
		{
			AllEquippingItemRepairREQ();
			return true;
		}
		break;
	case USCM_REPAIR_ITEM_ALL_CONFIRM_CANCLE:
		{
			if ( m_pDLGRepairItemConfirm != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRepairItemConfirm, NULL, false );
				m_pDLGRepairItemConfirm = NULL;
			}
			return true;
		}
		break;
	//////////////////////////////////////////////////////////////////////////

	case USCM_ENCHANT_ITEM_EXIT:
		{
			if ( m_pDLGEnchantItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGEnchantItem, NULL, false );
				m_pDLGEnchantItem = NULL;
			}
		}
		return true;
	case USCM_ENCHANT_ITEM_OLD_OK:
		{
			if(g_pData->GetUIManager()->GetDLGFlag() == false)
			{
				g_pData->GetUIManager()->SetDLGFlag(true);
				return true;
			}
			return Handler_EGS_ENCHANT_ITEM_REQ( false );
		}
		return true;
	case USCM_ENCHANT_ITEM_NEW_OK:
		{
			if(g_pData->GetUIManager()->GetDLGFlag() == false)
			{
				g_pData->GetUIManager()->SetDLGFlag(true);
				return true;
			}
			return Handler_EGS_ENCHANT_ITEM_REQ( true );
		}
		return true;
	case USCM_ENCHANT_ITEM_RESULT_OK:
		{
			if(g_pData->GetUIManager()->GetDLGFlag() == false)
			{
				g_pData->GetUIManager()->SetDLGFlag(true);
				return true;
			}

			if ( m_pDLGEnchantItemResult != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGEnchantItemResult, NULL, false );
				m_pDLGEnchantItemResult = NULL;
			}
		}
		return true;
	case USCM_ENCHANT_ADJUVANT_CHECKBOX:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
			m_bUseEnchantAdjuvant = pCheckBox->GetChecked();

			if( true == m_bUseEnchantAdjuvant )
			{
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnchantItemUID );
				if( NULL != pItem )
				{
					vector<int> vecAdjuvantItemIdList;
					GetRequiredEnchantAdjuvantItemID( pItem->GetItemTemplet()->GetUseLevel(), vecAdjuvantItemIdList );
					const int iAdjuvantCount  = ( true == vecAdjuvantItemIdList.empty() ? 0 : GetNumOfRequiredEnchantAdjuvantItem( vecAdjuvantItemIdList ) );

					if( iAdjuvantCount <= 0 )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_897 ), g_pMain->GetNowState() );
						m_bUseEnchantAdjuvant = false;
						pCheckBox->SetChecked( false );
					}
				}
			}
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
			Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_REQ();
#else
			UpdateEnchantWindow();
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
		}
		return true;
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	case USCM_ENCHANT_PLUS_CHECKBOX:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
			m_bUseEnchantPlus = pCheckBox->GetChecked();

			if( true == m_bUseEnchantPlus )
			{
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnchantItemUID );
				if( NULL != pItem )
				{
					vector<int> vecEnchantPlusItemIdList;
					GetRequiredEnchantPlusItemID( pItem->GetItemTemplet()->GetUseLevel(), vecEnchantPlusItemIdList );
					const int iEnchantPlusItemCount  = ( true == vecEnchantPlusItemIdList.empty() ? 0 : GetNumOfRequiredEnchantPlusItem( vecEnchantPlusItemIdList ) );
					if( iEnchantPlusItemCount <= 0 )
					{
						// 지헌 : 스트링 - 강화 성공 증가 아이템 없음!
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_12295 ), g_pMain->GetNowState() );
						m_bUseEnchantPlus = false;
						pCheckBox->SetChecked( false );
					}
				}
			}
			UpdateEnchantWindow();
		}
		return true;
#endif // SERV_ENCHANT_PLUS_ITEM
		//}}
		//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	case USCM_ENCHANT_DESTROY_GUARD_CHECKBOX:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
			m_bUseDestroyGuard = pCheckBox->GetChecked();

			if( true == m_bUseDestroyGuard )
			{
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnchantItemUID );
				if( NULL != pItem )
				{
					vector<int> vecDestroyGuardItemIdList;
					GetRequiredDestroyGuardItemID( pItem->GetItemTemplet()->GetUseLevel(), vecDestroyGuardItemIdList );
					const int iDestroyGuardItemCount  = ( true == vecDestroyGuardItemIdList.empty() ? 0 : GetNumOfRequiredDestroyGuardItem( vecDestroyGuardItemIdList ) );

					if( iDestroyGuardItemCount <= 0 )
					{
						// 지헌 : 스트링 - 파괴 방지 아이템 없음!
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_12296 ), g_pMain->GetNowState() );
						m_bUseDestroyGuard = false;
						pCheckBox->SetChecked( false );
					}
				}
			}
			UpdateEnchantWindow();
		}
		return true;
#endif // SERV_DESTROY_GUARD_ITEM
		//}}
	//////////////////////////////////////////////////////////////////////////
	case USCM_ENCHANT_ATTRIBUTE_ITEM_EXIT:
		{
			if ( m_pDLGAttribEnchantItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGAttribEnchantItem, NULL, false );
				m_pDLGAttribEnchantItem = NULL;
			}

		}
		return true;

	case USCM_ENCHANT_ATTRIBUTE_ITEM_SELECT:
		{
			if(g_pData->GetUIManager()->GetDLGFlag() == false)
			{
				g_pData->GetUIManager()->SetDLGFlag(true);
				return true;
			}

			//Send Packet gogo
			CKTDGUIControl* pControlItem = (CKTDGUIControl*)lParam;
			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttributeEnchantItemUID );
			if ( pItem != NULL )
			{

				if( 0 == pItem->GetItemData().m_EnchantedAttribute.m_aEnchantedType[0] )
				{
					m_AttribEnchantSlotID = CX2EnchantItem::ESI_SLOT_1;
				}
				else if( 0 == pItem->GetItemData().m_EnchantedAttribute.m_aEnchantedType[1] )
				{
					m_AttribEnchantSlotID = CX2EnchantItem::ESI_SLOT_2;
				}
#ifdef TRIPLE_ENCHANT_TEST
				else if( 0 == pItem->GetItemData().m_EnchantedAttribute.m_aEnchantedType[2] )
				{
					m_AttribEnchantSlotID = CX2EnchantItem::ESI_SLOT_3;
				}
#endif TRIPLE_ENCHANT_TEST



				m_AttribEnchantID = pControlItem->GetDummyInt(0);

				bool bWeapon = false;
				int eDForEnchant = 0;
				if ( pItem != NULL )
				{
					if ( pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_WEAPON )
					{
						bWeapon = true;
					}
					else
					{
						bWeapon = false;
					}

					g_pData->GetEnchantItem()->GetAttribEnchantRequireED( bWeapon, pItem->GetItemData().GetAttribEnchantedCount(), 
						pItem->GetItemTemplet()->GetUseLevel(), pItem->GetItemTemplet()->GetItemGrade(), eDForEnchant );
					m_pDLGAttribEnchantItemConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_REPLACED_STRING( ( STR_ID_898, "i", eDForEnchant ) ), USCM_ENCHANT_ATTRIBUTE_ITEM_CONFIRM, g_pMain->GetNowState() );
				}
			}
		}
		return true;
	case USCM_ENCHANT_ATTRIBUTE_ITEM_CONFIRM:
		{
			if ( m_pDLGAttribEnchantItemConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGAttribEnchantItemConfirm, NULL, false );
			m_pDLGAttribEnchantItemConfirm = NULL;

			return Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( m_AttribEnchantSlotID, m_AttribEnchantID, false );
		}
		return true;
	case USCM_ENCHANT_ATTRIBUTE_ITEM_REMOVE:
		{
			if(g_pData->GetUIManager()->GetDLGFlag() == false)
			{
				g_pData->GetUIManager()->SetDLGFlag(true);
				return true;
			}

			//remove_slot
			CKTDGUIControl* pControlItem = (CKTDGUIControl*)lParam;

			m_AttribEnchantSlotID = pControlItem->GetDummyInt(0);
			m_AttribEnchantID = CX2EnchantItem::ET_NONE;

			bool bWeapon = false;
			int eDForEnchant = 0;
			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttributeEnchantItemUID );
			if ( pItem != NULL )
			{
				if ( pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_WEAPON )
				{
					bWeapon = true;
				}
				else
				{
					bWeapon = false;
				}
				
				g_pData->GetEnchantItem()->GetAttribEnchantRequireED( bWeapon, pItem->GetItemData().GetAttribEnchantedCount(), 
					pItem->GetItemTemplet()->GetUseLevel(), pItem->GetItemTemplet()->GetItemGrade(), eDForEnchant );

                m_pDLGAttribEnchantItemConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_REPLACED_STRING( ( STR_ID_899, "i", eDForEnchant ) ), USCM_ENCHANT_ATTRIBUTE_ITEM_REMOVE_CONFIRM, g_pMain->GetNowState() );
			}
		}
		return true;
	case USCM_ENCHANT_ATTRIBUTE_ITEM_REMOVE_CONFIRM:
		{
			if ( m_pDLGAttribEnchantItemConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGAttribEnchantItemConfirm, NULL, false );
			m_pDLGAttribEnchantItemConfirm = NULL;

			return Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( m_AttribEnchantSlotID, m_AttribEnchantID, false );
		}
		return true;
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	case USCM_EVENT_EXCHANGE_ITEM:
		{
			//여기서 아이템 정보 얻어가지고 교환쪽으로 넘겨주자
			//위험하다...이런거 안했으면 좋겠다.
			// 커서 보통으로 바꿔주자
			IF_EVENT_ENABLED( CEI_EVENT_ADAMS_SHOP )
			{
				CX2State* pState = (CX2State*)g_pMain->GetNowState();	// DownCast니까 안심
				if(pState != NULL)
				{
					CX2Cursor* pCursor = pState->GetCursor();
					if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
					{
						pCursor->ChangeCursorState(CX2Cursor::XCS_NORMAL);
					}
				}
				CKTDGUIControl* pBuyButton = (CKTDGUIControl*)lParam;
				if ( pBuyButton != NULL && m_pDLGShop != NULL )
				{
					m_BuyItemSlotNum = pBuyButton->GetDummyInt(0);
					BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];

					//{{ kimhc // 2009-09-16 // 길드 창단 아이템 구입
					if ( pSlotInfo->m_pItemTemplet == NULL )
					{
						ASSERT( !"ItemTemplet is NULL" );
						return false;
					}
					///행운 코인 아이디를 박자
					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().GetItemByTID(141000887);
					int iGetItemNum = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().GetNumItemByTID(141000887);
					if( pItem == NULL)
					{
						//아이템이 존재 하지 않을 때
						//ASSERT( !"ItemTemplet is NULL" );
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_30014) , g_pMain->GetNowState() );
						return false;
					}
					else if( iGetItemNum < pSlotInfo->m_pItemTemplet->GetPrice() ) //보유하고 있는 아이템 갯수가 교환 갯수보다 적다
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_836) , g_pMain->GetNowState() );
						return false;
					}
					else
					{
						KEGS_ITEM_EXCHANGE_REQ kPacket;

						kPacket.m_iSourceItemID			= 141000887;
						kPacket.m_iSourceItemUID		= 0;//수량성의 경우 아이템 UID를 0으로 넣어준다.
						kPacket.m_iSourceQuantity		= pSlotInfo->m_pItemTemplet->GetPrice();
						kPacket.m_iHouseID				= (int)CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP;
						kPacket.m_iDestItemID			= pSlotInfo->m_pItemTemplet->GetItemID();

						g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_EXCHANGE_REQ, kPacket ); 
						g_pMain->AddServerPacket( EGS_ITEM_EXCHANGE_ACK, 60.f );
						return true;
					}
				}
			}
		}
		return true;
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	//////////////////////////////////////////////////////////////////////////
	default:
		break;
	}
	return false;
}

bool CX2UIShop::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_BUY_ED_ITEM_ACK:
		{
			return Handler_EGS_BUY_ED_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} 
		break;
	case EGS_SELL_ED_ITEM_ACK:
		{
			return Handler_EGS_SELL_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_REPAIR_ITEM_ACK:
		{
			return Handler_EGS_REPAIR_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_ENCHANT_ITEM_ACK:
		{
			return Handler_EGS_ENCHANT_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_ATTRIB_ENCHANT_ITEM_ACK:
		{
			return Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	case EGS_SUPPORT_MATERIAL_EVENT_TIME_ACK:
		{
			return Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	case EGS_ITEM_EXCHANGE_ACK:
			{
				return Handler_EGS_ITEM_EXCHANGE_ACK(hWnd, uMsg, wParam, lParam );
			}break;
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	default:
		break;
	}

	return false;
}


void CX2UIShop::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);
	
	if(m_pDLGShop != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGShop, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGShop, false );
	}
		//m_pDLGShop->SetLayer(layer);
	
}

void CX2UIShop::SetPosition(D3DXVECTOR2 vec)
{
	// 슬롯 다이얼로그 유닛뷰어
	if(m_pDLGShop != NULL) m_pDLGShop->SetPos(vec);
	
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

void CX2UIShop::SetShow(bool val)
{
	
	if(val)	// 켜질 때 처리해야 할 부분
	{
		if(m_HouseID == NULL) return;

		RegisterLuaBind();
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		if( m_pDLGShop != NULL )
		{
			for( UINT iS = 0; iS < m_SlotList.size(); iS++ )
			{
				CX2SlotItem* pkSlotItem = (CX2SlotItem*)m_SlotList[iS];
				if ( pkSlotItem != NULL )
				{
					pkSlotItem->DestroyItemUI();
					pkSlotItem->SetShow(false);
					SAFE_DELETE(pkSlotItem);
				}
			}
			m_SlotList.clear();
			SAFE_DELETE_DIALOG( m_pDLGShop );
		}
		if( (CX2LocationManager::HOUSE_ID)m_HouseID == CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
		{
			m_pDLGShop = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Shop_AdamsEvent.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGShop );
		}
		else
		{
			m_pDLGShop = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Shop.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGShop );
		}
#else
		if(m_pDLGShop == NULL)
		{
			m_pDLGShop = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Shop.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGShop );
		}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
		m_pDLGShop->SetShowEnable(true, true);
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		if( (CX2LocationManager::HOUSE_ID)m_HouseID != CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGShop->GetControl( L"CheckBox_ShowAll" );
			if ( pCheckBox != NULL )
				pCheckBox->SetChecked( m_bShowAllCharItem );
		}
#else
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGShop->GetControl( L"CheckBox_ShowAll" );
		if ( pCheckBox != NULL )
			pCheckBox->SetChecked( m_bShowAllCharItem );
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
		m_NowBuyItemCategory = CX2UIShop::BIC_WEAPON;

		// 하우스 템플릿을 받자
		CX2LocationManager::HouseTemplet* pHouseTemplet = g_pData->GetLocationManager()->GetHouseTemplet( (CX2LocationManager::HOUSE_ID)m_HouseID );
		// 여기서 강화랑 속성강화가 가능한지 bool 값을 정해주자
		
		// 하우스 템플릿이 없으면 : 망했다
		if(pHouseTemplet == NULL)
		{
			ASSERT( !"null house templet" );
			return;
		}
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		if( (CX2LocationManager::HOUSE_ID)m_HouseID != CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
		{
			// 상점 종류별로 할 수 있는 일이 다름 : 설정해 주자
			CKTDGUIButton* pButton_Repair = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_Repair" );
			CKTDGUIButton* pButton_RepairAll = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_RepairAll" );
			CKTDGUIButton* pButton_Sell = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_Sell" );
			CKTDGUIButton* pButton_Enchant = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_Enchant" );
			CKTDGUIButton* pButton_AttributeEnchant = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_EnchantAttribute" );

// 			CKTDGUIStatic* pStatic_Repair = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_RepairButton" );
// 			CKTDGUIStatic* pStatic_RepairAll = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_RepairAllButton" );
// 			CKTDGUIStatic* pStatic_Sell = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_SellButton" );
// 			CKTDGUIStatic* pStatic_Enchant = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_EnchantButton" );
//	 		CKTDGUIStatic* pStatic_AttributeEnchant = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_EnchantAttributeButton" );

			pButton_Repair->SetShowEnable(pHouseTemplet->m_bRepair, pHouseTemplet->m_bRepair);
			pButton_RepairAll->SetShowEnable(pHouseTemplet->m_bRepair, pHouseTemplet->m_bRepair);
			pButton_Sell->SetShowEnable(pHouseTemplet->m_bSell, pHouseTemplet->m_bSell);
			pButton_Enchant->SetShowEnable(pHouseTemplet->m_bEnchant, pHouseTemplet->m_bEnchant);
			pButton_AttributeEnchant->SetShowEnable(pHouseTemplet->m_bAttribute, pHouseTemplet->m_bAttribute);
		}
#else
		// 상점 종류별로 할 수 있는 일이 다름 : 설정해 주자
		CKTDGUIButton* pButton_Repair = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_Repair" );
		CKTDGUIButton* pButton_RepairAll = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_RepairAll" );
		CKTDGUIButton* pButton_Sell = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_Sell" );
		CKTDGUIButton* pButton_Enchant = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_Enchant" );
		CKTDGUIButton* pButton_AttributeEnchant = (CKTDGUIButton*) m_pDLGShop->GetControl( L"Button_EnchantAttribute" );

// 		CKTDGUIStatic* pStatic_Repair = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_RepairButton" );
// 		CKTDGUIStatic* pStatic_RepairAll = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_RepairAllButton" );
// 		CKTDGUIStatic* pStatic_Sell = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_SellButton" );
// 		CKTDGUIStatic* pStatic_Enchant = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_EnchantButton" );
// 		CKTDGUIStatic* pStatic_AttributeEnchant = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_EnchantAttributeButton" );
		
		pButton_Repair->SetShowEnable(pHouseTemplet->m_bRepair, pHouseTemplet->m_bRepair);
		pButton_RepairAll->SetShowEnable(pHouseTemplet->m_bRepair, pHouseTemplet->m_bRepair);
		pButton_Sell->SetShowEnable(pHouseTemplet->m_bSell, pHouseTemplet->m_bSell);
		pButton_Enchant->SetShowEnable(pHouseTemplet->m_bEnchant, pHouseTemplet->m_bEnchant);
		pButton_AttributeEnchant->SetShowEnable(pHouseTemplet->m_bAttribute, pHouseTemplet->m_bAttribute);
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

		// 카데고리 버튼 설정
		// 일단 다 끄고
		for(int j = (int)BIC_WEAPON; j < (int)BIC_COUNT; j++)
		{
			CKTDGUIRadioButton* pRadioButton = GetRadioButtonNameByCategory( (BUY_ITEM_CATEGORY)j );
			if(pRadioButton != NULL) pRadioButton->SetShow(false);
		}

		//{{ kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가
#ifndef	ADD_HOUSE_ID_TO_MANUFACTURE
		// 켜자
		if ( pHouseTemplet != NULL )
		{
			int MAGIC_CONTROL_WIDTH = 0;
			bool bFlag = false;
			for ( int i = 0; i < (int)pHouseTemplet->m_ShopCategoryTypeList.size(); i++ )
			{
				CX2UIShop::BUY_ITEM_CATEGORY buyItemCategory = pHouseTemplet->m_ShopCategoryTypeList[i];
				CKTDGUIRadioButton* pRadioButton = GetRadioButtonNameByCategory( buyItemCategory );

				if ( pRadioButton != NULL )
				{
					if ( false == bFlag )
					{
						m_NowBuyItemCategory = buyItemCategory;						 
						pRadioButton->SetChecked( true );
						bFlag = true;
					}
					pRadioButton->SetOffsetPos( D3DXVECTOR2( (float)MAGIC_CONTROL_WIDTH, 0 ) );
					pRadioButton->SetShow( true );
					pRadioButton->SetEnable( true );
					MAGIC_CONTROL_WIDTH += pRadioButton->GetDummyInt(0);		// Width를 LUA에 AddDummyInt로 만들어 넣어 둘 것!
				}
			}
		}
		//}} kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가
#endif	ADD_HOUSE_ID_TO_MANUFACTURE

		// 구매 슬롯 설정 초기화
		for(UINT i = 0; i<m_vecBuySlot.size(); i++)
		{
			BuySlot* pBuySlot = m_vecBuySlot[i];
			SAFE_DELETE(pBuySlot);
		}
		m_vecBuySlot.clear();

		// 새로 만들어 주자
		const int MAGIC_MAX_NUM_BUY_ITEM_SLOT = m_SlotList.size();
		for(int i = 0; i<MAGIC_MAX_NUM_BUY_ITEM_SLOT; i++)
		{
			WCHAR buff[256] = {0,};
			BuySlot* pBuySlot = new BuySlot();
			//wsprintf( buff, L"Button_Buy%d", i);
			StringCchPrintf( buff, 256, L"Button_Buy%d", i);
			pBuySlot->m_pBuyButton = (CKTDGUIButton*) m_pDLGShop->GetControl( buff );
			
			//wsprintf( buff, L"Static_List_EDOnly%d", i);
			StringCchPrintf(  buff, 256, L"Static_List_EDOnly%d", i);
			pBuySlot->m_pEDStatic = (CKTDGUIStatic*) m_pDLGShop->GetControl( buff );
						
			//wsprintf( buff, L"Static_List_EDVP%d", i);
			StringCchPrintf(  buff, 256, L"Static_List_EDVP%d", i);
			pBuySlot->m_pEDVPStatic = (CKTDGUIStatic*) m_pDLGShop->GetControl( buff );
			
			m_vecBuySlot.push_back(pBuySlot);
		}

		BuyUISetting();
		
		//{{ kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
		// 켜자
		int MAGIC_CONTROL_WIDTH = 0;
		bool bFlag = false;
		for ( int i = 0; i < static_cast<int>( BIC_COUNT ); i++ )
		{
			if ( m_vecItemList[i].empty() == false )
			{
				CKTDGUIRadioButton* pRadioButton = GetRadioButtonNameByCategory( BUY_ITEM_CATEGORY(i) );

				if ( pRadioButton != NULL )
				{
					if ( false == bFlag )
					{
						m_NowBuyItemCategory = BUY_ITEM_CATEGORY(i);						 
						pRadioButton->SetChecked( true );
						bFlag = true;
					}
					pRadioButton->SetOffsetPos( D3DXVECTOR2( (float)MAGIC_CONTROL_WIDTH, 0 ) );
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
					if( (CX2LocationManager::HOUSE_ID)m_HouseID != CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
					{
						pRadioButton->SetShow( true );
						pRadioButton->SetEnable( true );
					}
					else
					{
						pRadioButton->SetShow( false );
						pRadioButton->SetEnable( false );
					}
#else
					pRadioButton->SetShow( true );
					pRadioButton->SetEnable( true );
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
					MAGIC_CONTROL_WIDTH += pRadioButton->GetDummyInt(0);		// Width를 LUA에 AddDummyInt로 만들어 넣어 둘 것!
				}
			}
		}

		ResetBuySlotList( 1 );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		//}} kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가

		D3DXVECTOR3 tmp;
		tmp = m_pDLGShop->GetDummyPos(0);
		m_DLGPosition.x = tmp.x;
		m_DLGPosition.y = tmp.y;
		tmp = m_pDLGShop->GetDummyPos(1);
		m_DLGSize.x = tmp.x;
		m_DLGSize.y = tmp.y;

		m_bShow = true;
		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_SHOP);
		
	}
	else	// 꺼질 때 처리해야 할 부분
	{

		CX2State* pState = (CX2State*)g_pMain->GetNowState();	// DownCast니까 안심
		if(pState != NULL)
		{
			CX2Cursor* pCursor = pState->GetCursor();
			if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
			{
				pCursor->ChangeCursorState(CX2Cursor::XCS_NORMAL);
			}
		}

		for( UINT iS = 0; iS < m_SlotList.size(); iS++ )
		{
			CX2SlotItem* pkSlotItem = (CX2SlotItem*)m_SlotList[iS];
			if ( pkSlotItem != NULL )
			{
				pkSlotItem->DestroyItemUI();
				pkSlotItem->SetShow(false);
				pkSlotItem->SetEnable(false);
			}
		}
		// 슬롯 마우스오버 이미지를 지워주고
		m_pDLGSelectedItem->SetShow( false );
		InvalidSlotDesc();

		// 하위 다이얼로그를 전부 지우자
		SAFE_DELETE_DIALOG(m_pDLGSellItemNum);
		m_pDLGSellItemNum = NULL;
		SAFE_DELETE_DIALOG(m_pDLGSellItemConfirm);
		m_pDLGSellItemConfirm = NULL;
		SAFE_DELETE_DIALOG(m_pDLGRepairItemConfirm);
		m_pDLGRepairItemConfirm = NULL;
		SAFE_DELETE_DIALOG(m_pDLGBuyItemNum);
		m_pDLGBuyItemNum = NULL;
		SAFE_DELETE_DIALOG(m_pDLGBuyItemConfirm);
		m_pDLGBuyItemConfirm = NULL;
		SAFE_DELETE_DIALOG(m_pDLGEnchantItem);
		m_pDLGEnchantItem = NULL;
		SAFE_DELETE_DIALOG(m_pDLGEnchantItemResult);
		m_pDLGEnchantItemResult = NULL;
		SAFE_DELETE_DIALOG(m_pDLGAttribEnchantItem);
		m_pDLGAttribEnchantItem = NULL;
		SAFE_DELETE_DIALOG(m_pDLGAttribEnchantItemConfirm);
		m_pDLGAttribEnchantItemConfirm = NULL;

		m_bShow = false;
        if( m_pDLGShop != NULL )
		    m_pDLGShop->SetShow(false);
		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_SHOP);
	}

	//g_pKTDXApp->SkipFrame();
}


// 상품 리스트 받아오기
void CX2UIShop::BuyUISetting()
{
	for ( int i = 0; i < BIC_COUNT; i++ )
	{
		m_vecItemList[i].resize(0);
	}

    vector<const CX2Item::ItemTemplet*> vecShopItemList;

	// g_pData->GetItemManager()->GetShopItemList( g_pMain->GetPrevVillageStateID(), vecShopItemList );
	if(g_pTFieldGame != NULL)
	{
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		int npcId = 0;
		if( (CX2LocationManager::HOUSE_ID)m_HouseID != CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
		{
			npcId = g_pTFieldGame->GetJoinNpcId();
		}
		else
		{
			npcId = (int)CX2UnitManager::NPC_UNIT_ID::NUI_EVENT_ADAMS_UI_SHOP;
		}
#else
		int npcId = g_pTFieldGame->GetJoinNpcId();
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
		if( g_pData->GetItemManager()->GetNpcShopItemList( g_pData->GetLocationManager()->GetCurrentVillageID(), npcId, vecShopItemList ) == false)
			g_pData->GetItemManager()->GetShopItemList( g_pData->GetLocationManager()->GetCurrentVillageID(), vecShopItemList );
	}
	else
	{
		g_pData->GetItemManager()->GetShopItemList( g_pData->GetLocationManager()->GetCurrentVillageID(), vecShopItemList );
	}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	std::set< int > setBanBuyItem;
	if ( g_pData != NULL && g_pData->GetUIManager() != NULL )
	{
		setBanBuyItem = g_pData->GetUIManager()->GetBanBuyItemList();
	}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	for( UINT i=0; i<vecShopItemList.size(); i++ )
	{
        const CX2Item::ItemTemplet* pItemTemplet = vecShopItemList[i];

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
		if ( setBanBuyItem.find(pItemTemplet->GetItemID()) != setBanBuyItem.end())
			continue;
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

		if ( m_bShowAllCharItem == false )
		{
			if ( IsPossibleUsedByMyCharacter( pItemTemplet ) == false )
				continue;
		}
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		if( (CX2LocationManager::HOUSE_ID)m_HouseID == CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
		{
			m_vecItemList[ BIC_QUICK_SLOT ].push_back( pItemTemplet );
		}
		else
		{
			switch( pItemTemplet->GetEqipPosition() ) 
			{
				//무기
			case CX2Unit::EP_WEAPON_HAND:			//무기
			case CX2Unit::EP_WEAPON_TEMP1:		//임시1
			case CX2Unit::EP_WEAPON_TEMP2:		//임시2
			case CX2Unit::EP_WEAPON_TEMP3:		//임시3
				m_vecItemList[ BIC_WEAPON ].push_back( pItemTemplet );
				break;

				//방어구
			case CX2Unit::EP_DEFENCE_HAIR:		//헤어스타일
			case CX2Unit::EP_DEFENCE_FACE:		//얼굴
			case CX2Unit::EP_DEFENCE_BODY:		//상의
				m_vecItemList[ BIC_COAT ].push_back( pItemTemplet );
				break;

			case CX2Unit::EP_DEFENCE_LEG:			//하의
				m_vecItemList[ BIC_TROUSERS ].push_back( pItemTemplet );
				break;

			case CX2Unit::EP_DEFENCE_HAND:		//장갑
				m_vecItemList[ BIC_HAND ].push_back( pItemTemplet );
				break;

			case CX2Unit::EP_DEFENCE_FOOT:		//신발
				m_vecItemList[ BIC_FOOTWEAR ].push_back( pItemTemplet );
				break;

			case CX2Unit::EP_QUICK_SLOT:
				{
					m_vecItemList[ BIC_QUICK_SLOT ].push_back( pItemTemplet );
				}
				break;

#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT	// 여기 필요 없지 싶다.
				//		case CX2Unit::EP_ONEPIECE_FASHION:
				//			{
				//				m_vecItemList[ BIC_COAT ].push_back( pItemTemplet );
				//			}
				//			break;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

				//액세서리
			case CX2Unit::EP_AC_TITLE:			//칭호
			case CX2Unit::EP_AC_HAIR:				//헤어
			case CX2Unit::EP_AC_FACE1:				
			case CX2Unit::EP_AC_FACE2:			
			case CX2Unit::EP_AC_FACE3:			
			case CX2Unit::EP_AC_BODY:				//상의
			case CX2Unit::EP_AC_ARM:				//팔
			case CX2Unit::EP_AC_LEG:				//다리
			case CX2Unit::EP_AC_RING:				//반지
			case CX2Unit::EP_AC_NECKLESS:			//목걸이
			case CX2Unit::EP_AC_WEAPON:			//	무기 악세사리
			case CX2Unit::EP_AC_TEMP2:			//임시2
			case CX2Unit::EP_AC_TEMP3:			//임시3
			case CX2Unit::EP_AC_TEMP4:			//임시4
			case CX2Unit::EP_AC_TEMP5:			//임시5
				m_vecItemList[ BIC_ACCESSORY ].push_back( pItemTemplet );
				break;

			case CX2Unit::EP_DEFENCE_TEMP1:		//임시1
			case CX2Unit::EP_DEFENCE_TEMP2:		//임시2
			case CX2Unit::EP_DEFENCE_TEMP3:		//임시3

				//필살기
			case CX2Unit::EP_SKILL_1:				//1단계 필살기
			case CX2Unit::EP_SKILL_2:				//2단계 필살기
			case CX2Unit::EP_SKILL_3:				//3단계 필살기
			case CX2Unit::EP_SKILL_TEMP1:			//임시1
			case CX2Unit::EP_SKILL_TEMP2:			//임시2
			case CX2Unit::EP_SKILL_TEMP3:			//임시3
			default:
				m_vecItemList[ BIC_SPECIAL ].push_back( pItemTemplet );
				break;
			}
		}
#else
		switch( pItemTemplet->GetEqipPosition() ) 
		{

			//무기
		case CX2Unit::EP_WEAPON_HAND:			//무기
		case CX2Unit::EP_WEAPON_TEMP1:		//임시1
		case CX2Unit::EP_WEAPON_TEMP2:		//임시2
		case CX2Unit::EP_WEAPON_TEMP3:		//임시3
			m_vecItemList[ BIC_WEAPON ].push_back( pItemTemplet );
			break;

			//방어구
		case CX2Unit::EP_DEFENCE_HAIR:		//헤어스타일
		case CX2Unit::EP_DEFENCE_FACE:		//얼굴
		case CX2Unit::EP_DEFENCE_BODY:		//상의
			m_vecItemList[ BIC_COAT ].push_back( pItemTemplet );
			break;

		case CX2Unit::EP_DEFENCE_LEG:			//하의
			m_vecItemList[ BIC_TROUSERS ].push_back( pItemTemplet );
			break;

		case CX2Unit::EP_DEFENCE_HAND:		//장갑
			m_vecItemList[ BIC_HAND ].push_back( pItemTemplet );
			break;

		case CX2Unit::EP_DEFENCE_FOOT:		//신발
			m_vecItemList[ BIC_FOOTWEAR ].push_back( pItemTemplet );
			break;

		case CX2Unit::EP_QUICK_SLOT:
			{
				m_vecItemList[ BIC_QUICK_SLOT ].push_back( pItemTemplet );
			}
			break;

			//액세서리
		case CX2Unit::EP_AC_TITLE:			//칭호
		case CX2Unit::EP_AC_HAIR:				//헤어
		case CX2Unit::EP_AC_FACE1:				
		case CX2Unit::EP_AC_FACE2:			
		case CX2Unit::EP_AC_FACE3:			
		case CX2Unit::EP_AC_BODY:				//상의
		case CX2Unit::EP_AC_ARM:				//팔
		case CX2Unit::EP_AC_LEG:				//다리
		case CX2Unit::EP_AC_RING:				//반지
		case CX2Unit::EP_AC_NECKLESS:			//목걸이
		case CX2Unit::EP_AC_WEAPON:			//	무기 악세사리
		case CX2Unit::EP_AC_TEMP2:			//임시2
		case CX2Unit::EP_AC_TEMP3:			//임시3
		case CX2Unit::EP_AC_TEMP4:			//임시4
		case CX2Unit::EP_AC_TEMP5:			//임시5
			m_vecItemList[ BIC_ACCESSORY ].push_back( pItemTemplet );
			break;

		case CX2Unit::EP_DEFENCE_TEMP1:		//임시1
		case CX2Unit::EP_DEFENCE_TEMP2:		//임시2
		case CX2Unit::EP_DEFENCE_TEMP3:		//임시3



			//필살기
		case CX2Unit::EP_SKILL_1:				//1단계 필살기
		case CX2Unit::EP_SKILL_2:				//2단계 필살기
		case CX2Unit::EP_SKILL_3:				//3단계 필살기
		case CX2Unit::EP_SKILL_TEMP1:			//임시1
		case CX2Unit::EP_SKILL_TEMP2:			//임시2
		case CX2Unit::EP_SKILL_TEMP3:			//임시3
		default:
			m_vecItemList[ BIC_SPECIAL ].push_back( pItemTemplet );
			break;

		}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	}

	//{{ kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가
#ifndef	ADD_HOUSE_ID_TO_MANUFACTURE
	ResetBuySlotList( 1 );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
	//}} kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가
}

// 상품 리스트는 그대로. 페이지 / 카데고리가 바뀔 때
void CX2UIShop::ResetBuySlotList( int nPage )
{
    if( m_pDLGShop == NULL )
    {
        return;
    }//if

	// 슬롯 갯수에 따른 max page 설정
	const int MAGIC_MAX_NUM_BUY_ITEM_SLOT = m_SlotList.size();
	m_nNowPage = nPage;
	m_nMaxPage = (int)(m_vecItemList[m_NowBuyItemCategory].size()+MAGIC_MAX_NUM_BUY_ITEM_SLOT-1)/MAGIC_MAX_NUM_BUY_ITEM_SLOT;
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	if( (CX2LocationManager::HOUSE_ID)m_HouseID != CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
	{
		CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_PageNumber" );
		wstringstream wstrmPageNum;
		wstrmPageNum << m_nNowPage << L"/" << m_nMaxPage;
		pStaticPageNum->GetString(0)->msg = wstrmPageNum.str();
	}
#else
	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*) m_pDLGShop->GetControl( L"Static_PageNumber" );
	wstringstream wstrmPageNum;
	wstrmPageNum << m_nNowPage << L"/" << m_nMaxPage;
	pStaticPageNum->GetString(0)->msg = wstrmPageNum.str();
#endif ALWAYS_EVENT_ADAMS_UI_SHOP	
	// 전체 상품의 인덱스
	int iStartIndex = (m_nNowPage-1) * m_vecBuySlot.size();
	int iEndIndex = min( m_nNowPage*m_vecBuySlot.size(), (int)m_vecItemList[m_NowBuyItemCategory].size() );

	// const int MAGIC_MAX_NUM_BUY_ITEM_SLOT = m_pDLGShop->GetDummyInt(0);

	// 슬롯 데이터 초기화
	for( UINT iS = 0; iS < m_SlotList.size(); iS++ )
	{
		CX2SlotItem* pkSlotItem = (CX2SlotItem*)m_SlotList[iS];
		if ( pkSlotItem != NULL )
		{
			pkSlotItem->DestroyItemUI();
			pkSlotItem->SetShow(false);
			pkSlotItem->SetEnable(false);
		}
	}

	// 끄고
	for( unsigned int iS = 0; iS < m_vecBuySlot.size(); ++iS )
	{
		BuySlot* pBuySlot = m_vecBuySlot[iS];		
		if ( pBuySlot->m_pEDStatic != NULL )
			pBuySlot->m_pEDStatic->SetShow(false);
		if ( pBuySlot->m_pEDVPStatic != NULL )
			pBuySlot->m_pEDVPStatic->SetShow(false);
		if ( pBuySlot->m_pBuyButton != NULL )
			pBuySlot->m_pBuyButton->SetShow(false);
	}

	// Slot Index
	int iSlotIndex = 0;
	// 슬롯 데이터 셋팅
	for( int iS = iStartIndex; iS < iEndIndex; iS++ )
	{
		CX2SlotItem* pkSlotItem = (CX2SlotItem*) m_SlotList[iSlotIndex];
		BuySlot* pBuySlot = m_vecBuySlot[iSlotIndex];	
		pBuySlot->m_pItemTemplet = m_vecItemList[m_NowBuyItemCategory][iS];
		
		// 슬롯에 아이템 쨘
		if ( pkSlotItem != NULL && pBuySlot->m_pItemTemplet != NULL )
		{
			pkSlotItem->CreateItemUI( pBuySlot->m_pItemTemplet );
			pkSlotItem->SetShow(true);
			pkSlotItem->SetEnable(true);
		}

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		int tempMAGIC_STRING_WIDTH = 0;
		if( (CX2LocationManager::HOUSE_ID)m_HouseID != CX2LocationManager::HOUSE_ID::HI_EVENT_ADAMS_UI_SHOP )
			tempMAGIC_STRING_WIDTH = 147;
		else
			tempMAGIC_STRING_WIDTH = 118;

		const int MAGIC_STRING_WIDTH = tempMAGIC_STRING_WIDTH;
#else // ALWAYS_EVENT_ADAMS_UI_SHOP
		const int MAGIC_STRING_WIDTH = 147;
#endif // ALWAYS_EVENT_ADAMS_UI_SHOP

		// 구매에 VP가 필요한지 아닌지 확인하고
		if( pBuySlot->m_pItemTemplet->GetShopPriceType() == CX2Item::SPT_GP && pBuySlot->m_pItemTemplet->GetPricePvPPoint() > 0 ) // 필요하면
		{
			CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pBuySlot->m_pEDVPStatic->GetString(0)->fontIndex );
            wstring wstrItemName = pBuySlot->m_pItemTemplet->GetFullName_();
			if(pFont != NULL )
			{
				if( pFont->GetWidth( wstrItemName.c_str() ) > MAGIC_STRING_WIDTH*g_pKTDXApp->GetResolutionScaleX() )
				{
					int width = (int)((MAGIC_STRING_WIDTH - 3*pFont->GetWidth( L'.'))*g_pKTDXApp->GetResolutionScaleX() );
					UINT uSlideStartPos = 0;
					UINT uMaxStringLength = wstrItemName.length();
					int iStringSize = 0;

					bool bFindPos = false;
					for( UINT i=uSlideStartPos; i<uMaxStringLength; ++i )
					{
						WCHAR wChar = wstrItemName[i];
						iStringSize += pFont->GetWidth( wChar );

						if( iStringSize >= width )
						{
							wstrItemName = wstrItemName.substr(uSlideStartPos, i-uSlideStartPos);
							wstrItemName += L"...";
							break;
						}
					}
				}
				
			}

			pBuySlot->m_pEDVPStatic->GetString(0)->msg = wstrItemName;
			wstringstream wstrmPrice;
			wstrmPrice << pBuySlot->m_pItemTemplet->GetPrice() << L"\n " << pBuySlot->m_pItemTemplet->GetPricePvPPoint();
			pBuySlot->m_pEDVPStatic->GetString(1)->msg = wstrmPrice.str();
			pBuySlot->m_pEDVPStatic->SetShow(true);
		
		}
		else	// 필요없으면 ( m_pricetype이 SPT_GP가 아닌 경우도 포함 )
		{
			CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pBuySlot->m_pEDVPStatic->GetString(0)->fontIndex );
            wstring wstrItemName = pBuySlot->m_pItemTemplet->GetFullName_();
			if(pFont != NULL )
			{
				//int itemNameWidth = (int)( pFont->GetWidth( pBuySlot->m_pItemTemplet->GetFullName_().c_str() ) / g_pKTDXApp->GetResolutionScaleX() );
				//int iLine = CX2Main::LineBreak( wstrItemName, pFont, (int)((float)width*g_pKTDXApp->GetResolutionScaleX()), L"", true );

 				if( pFont->GetWidth( wstrItemName.c_str() ) > MAGIC_STRING_WIDTH*g_pKTDXApp->GetResolutionScaleX() )
 				{
					int width = (int)((MAGIC_STRING_WIDTH - 3*pFont->GetWidth( L'.'))*g_pKTDXApp->GetResolutionScaleX() );
					UINT uSlideStartPos = 0;
					UINT uMaxStringLength = wstrItemName.length();
					int iStringSize = 0;

					bool bFindPos = false;
					for( UINT i=uSlideStartPos; i<uMaxStringLength; ++i )
					{
						WCHAR wChar = wstrItemName[i];
						iStringSize += pFont->GetWidth( wChar );

						if( iStringSize >= width )
						{
							wstrItemName = wstrItemName.substr(uSlideStartPos, i-uSlideStartPos);
							wstrItemName += L"...";
							break;
						}
					}
				}
			}

			pBuySlot->m_pEDStatic->GetString(0)->msg = wstrItemName;
#ifdef SIMPLE_BUG_FIX
			pBuySlot->m_pEDStatic->GetString(1)->msg = g_pMain->GetEDString( pBuySlot->m_pItemTemplet->GetPrice() ).c_str();
#else SIMPLE_BUG_FIX			
			wstringstream wstrmPrice;
            wstrmPrice << pBuySlot->m_pItemTemplet->GetPrice();
			pBuySlot->m_pEDStatic->GetString(1)->msg = wstrmPrice.str();
#endif SIMPLE_BUG_FIX
			pBuySlot->m_pEDStatic->SetShow(true);
		
		}

		pBuySlot->m_pBuyButton->SetShowEnable(true, true);

		iSlotIndex++;
	}

	// 미리보기
	// ResetBuySlotItemEquiped( nPage );
}

void CX2UIShop::SellItem( UidType sellItemUID, D3DXVECTOR2 pos )
{
	//void OpenItemSellNumDLG( D3DXVECTOR2 pos )
	//void OpenSellItemConfirmDLG(  );
	
	m_SellItemUID = sellItemUID;
	
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pSellItem = kInventory.GetItem( m_SellItemUID );
//{{ robobeg : 2013-11-04
    if ( pSellItem == NULL || pSellItem->GetItemTemplet() == NULL )
    {
        return;
    }
//}} robobeg : 2013-11-04

	if ( pSellItem->GetItemTemplet()->GetShopPriceType() == CX2Item::SPT_NONE )
	{
		CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_900 ), g_pMain->GetNowState() );
		pMsgBox->SetEnableMoveByDrag_LUA(false);
		
		return;
	}

#ifdef SERV_IMPOSSIBLE_SELL_ITEM_ACCESSORY
	if ( pSellItem->GetItemTemplet()->GetPeriodType() == CX2Item::SPT_GP && pSellItem->GetItemTemplet()->GetItemType() == CX2Item::IT_ACCESSORY && pSellItem->GetItemData().m_Period != 0 )
	{
		CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_21663 ), g_pMain->GetNowState() );
		pMsgBox->SetEnableMoveByDrag_LUA(false);
		return;
	}
#endif // SERV_IMPOSSIBLE_SELL_ITEM_ACCESSORY

	//{{ kimhc // 2009-09-08 // 봉인된 아이템 상점 판매 불가
#ifdef	SEAL_ITEM

	if ( pSellItem->GetItemData().m_bIsSealed == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4477 ), g_pMain->GetNowState() );
		return; 
	}
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-08 // 봉인된 아이템 상점 판매 불가

	// 수량성인지 아닌지 봐서 수량성이 아니면
	const CX2Item::ItemTemplet* pItemTemplet = pSellItem->GetItemTemplet();
	if( CX2Item::PT_QUANTITY != pItemTemplet->GetPeriodType() )
	{
		m_SellItemNum = 1;
		OpenSellItemConfirmDLG();
	}
	else		// 수량성이면
	{
		m_SellItemNum = pSellItem->GetItemData().m_Quantity;

		if(true == m_bQuickSell)	// 수량묻지않기
		{			
			OpenSellItemConfirmDLG();
		}
		else
		{
			OpenItemSellNumDLG( pos );
		}		
	}

	return;
}


void CX2UIShop::OpenSellItemConfirmDLG()
{
	int popUpSizeX = 425;
	int popUpSizeY = 147;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SellItemUID );
	if ( pItem != NULL )
	{
		if ( pItem->GetItemTemplet() != NULL )
		{
			m_pDLGSellItemConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), 
																	GET_REPLACED_STRING( ( STR_ID_901, "i", pItem->GetEDToSell() * m_SellItemNum ) ), USCM_SELL_ITEM_CONFIRM_OK , g_pMain->GetNowState(), USCM_SELL_ITEM_CONFIRM_CANCLE );
			m_pDLGSellItemConfirm->SetEnableMoveByDrag_LUA(false);

			CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGSellItemConfirm->GetControl( L"MsgBoxOkAndCancelOkButton" );
			if ( pButton != NULL )
			{
				pButton->ChangeMouseUpSound( L"Item_Sell.ogg" );
				m_pDLGSellItemConfirm->RequestFocus(pButton);

			}
		}
	}
}


// 이 코드는 앞으로 자주 쓰게 될 것이야
void CX2UIShop::OpenItemSellNumDLG( D3DXVECTOR2 pos )
{

	SAFE_DELETE_DIALOG(m_pDLGSellItemNum);
	m_pDLGSellItemNum = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_An.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSellItemNum );
// 	pos = pos - D3DXVECTOR2(75, 20);
// 	if(pos.x > 865 )
// 		pos.x = 865;
// 	if(pos.y > 633 )
// 		pos.y = 633;
// 	m_pDLGSellItemNum->SetPos( pos );
	m_pDLGSellItemNum->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-865, 768-633), D3DXVECTOR2(75,20)) );

	CKTDGUIStatic* pStaticPrice = (CKTDGUIStatic*) m_pDLGSellItemNum->GetControl( L"Static_Price" );

	CKTDGUIButton* pButtonOK = (CKTDGUIButton*) m_pDLGSellItemNum->GetControl( L"Button_OK" );
	CKTDGUIButton* pButtonCancle = (CKTDGUIButton*) m_pDLGSellItemNum->GetControl( L"Button_Cancle" );
	CKTDGUIButton* pButtonPlus = (CKTDGUIButton*) m_pDLGSellItemNum->GetControl( L"Button_Plus" );
	CKTDGUIButton* pButtonMinus = (CKTDGUIButton*) m_pDLGSellItemNum->GetControl( L"Button_Minus" );
	CKTDGUIIMEEditBox* pQuantity = (CKTDGUIIMEEditBox*) m_pDLGSellItemNum->GetControl( L"IME_Edit_Quantity" );

	pButtonOK->SetCustomMsgMouseUp(USCM_SELL_ITEM_QUANTITY_CONFIRM);
	pButtonCancle->SetCustomMsgMouseUp(USCM_SELL_ITEM_QUANTITY_CANCLE);
	pButtonPlus->SetCustomMsgMouseUp(USCM_SELL_ITEM_QUANTITY_PLUS);
	pButtonMinus->SetCustomMsgMouseUp(USCM_SELL_ITEM_QUANTITY_MINUS);
	pQuantity->SetCustomMsgChange(USCM_SELL_ITEM_QUANTITY_UPDATE);
	pQuantity->SetCustomMsgEnter(USCM_SELL_ITEM_QUANTITY_CONFIRM);
	m_pDLGSellItemNum->SetCloseCustomUIEventID(USCM_SELL_ITEM_QUANTITY_CANCLE);

	UpdateItemSellNumDLG(false, true);
	pQuantity->RequestFocus();
	pQuantity->OnFocusIn();

}

void CX2UIShop::UpdateItemSellNumDLG( bool bReadIME, bool bTextSelected )
{
	CKTDGUIIMEEditBox* pQuantity = (CKTDGUIIMEEditBox*) m_pDLGSellItemNum->GetControl( L"IME_Edit_Quantity" );

	if(bReadIME)
	{
		m_SellItemNum = g_pMain->GetEDFromString( pQuantity->GetText() );
	}	

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SellItemUID );
	int MaxItemNum = 100;
	if(pItem != NULL)
	{
		MaxItemNum = pItem->GetItemTemplet()->GetQuantity();
	}
	else
	{
		ASSERT( !"Item Sell Process Error!" );
	}

	if ( m_SellItemNum <= 0 )
	{
		if(bReadIME)				// 입력창으로 받아온 경우
			m_SellItemNum = 0;
		else						// << 를 누른 경우 : 0 아래면 max로 순환시킨다.
			m_SellItemNum = MaxItemNum;
	}
	
	if ( m_SellItemNum >= pItem->GetItemData().m_Quantity )
		m_SellItemNum = pItem->GetItemData().m_Quantity;

	if ( m_SellItemNum >= MaxItemNum )
		m_SellItemNum = MaxItemNum;

	WCHAR buff[256] = {0};
	_itow( m_SellItemNum, buff, 10 );
	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str(), bTextSelected );

	// 판매가
	CKTDGUIStatic* pStaticPrice = (CKTDGUIStatic*) m_pDLGSellItemNum->GetControl( L"Static_Price" );
	pStaticPrice->GetString(0)->msg = g_pMain->GetEDString(m_SellItemNum * pItem->GetEDToSell() );

}


bool CX2UIShop::Handler_EGS_SELL_ITEM_REQ()
{
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	CX2Item* pItem = kInventory.GetItem( m_SellItemUID );
	if( NULL == pItem )
		return false;

	// 아이템을 팔려고 시도했을 때, 결과가 20억이 넘는지 확인한다!
	INT64 EDTotalAfterSell = 0;
	EDTotalAfterSell += pItem->GetEDToSell() * m_SellItemNum;
	EDTotalAfterSell += g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
	if(EDTotalAfterSell < 0 || EDTotalAfterSell > MAX_ED_FOR_PLAYER)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2591 ), g_pMain->GetNowState() );
		return false;
	}


	KEGS_SELL_ED_ITEM_REQ kPacket;
	kPacket.m_iItemUID = pItem->GetItemData().m_ItemUID;
	kPacket.m_iQuantity = m_SellItemNum;

	g_pData->GetServerProtocol()->SendPacket( EGS_SELL_ED_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SELL_ED_ITEM_ACK );

	m_SellItemUID = 0;
	m_SellItemNum = 0;
	return true;
}
bool CX2UIShop::Handler_EGS_SELL_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 다이얼로그 처리
	if ( m_pDLGSellItemConfirm != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSellItemConfirm, NULL, false );

	m_pDLGSellItemConfirm = NULL;

	// 패킷 처리
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELL_ED_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_SELL_ED_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			std::vector< KInventoryItemInfo > vecInventoryItemInfo;
			vecInventoryItemInfo.push_back( kEvent.m_kInventorySlotInfo );
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( vecInventoryItemInfo );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED		= kEvent.m_iED;		

			if(g_pData->GetUIManager()->GetUIInventory() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( vecInventoryItemInfo );
			}
			
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}

			//m_pStateMenu->OnChangedInventory();

			return true;
		}
	}

	return false;
}


// 수리
void CX2UIShop::RepairItem( UidType RepairItemUID )
{
	m_RepairItemUID = RepairItemUID;
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( RepairItemUID );
	if(m_pDLGRepairItemConfirm != NULL) SAFE_DELETE_DIALOG(m_pDLGRepairItemConfirm);

	if ( pItem != NULL )
	{
		if ( pItem->GetItemTemplet()->GetPeriodType() == CX2Item::PT_ENDURANCE )
		{
			if ( pItem->GetItemData().m_Endurance >= pItem->GetItemTemplet()->GetEndurance() )
			{
				CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_902 ), g_pMain->GetNowState() );
				pMsgBox->SetEnableMoveByDrag_LUA(false);
			}
			else
			{
// 				m_TempSortType = ((CX2SlotItem*)pSlot)->GetSortType();
// 				m_TempSlotID = ((CX2SlotItem*)pSlot)->GetSlotID();

				WCHAR buff[256] = {0,};
				int repairEDCost = pItem->GetEDToRepair();
				int repairVPCost = pItem->GetVPToRepair(); 
				
				float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
				if( fDiscountRate > 0.f )
				{
#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-04-19
					repairEDCost = static_cast<int>( repairEDCost * fDiscountRate );		/// 수리비 증가 버프로 인한 공식 수정
#else // SERV_NEW_DEFENCE_DUNGEON
					repairEDCost = (int) ( repairEDCost * ( 1.f - fDiscountRate ) );
#endif // SERV_NEW_DEFENCE_DUNGEON
				}

#ifdef ITEM_RECOVERY_TEST
				if ( repairVPCost == 0 )
				{
					if(true == pItem->IsDisabled()
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
					&& false == pItem->GetIsEvaluation() 					
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
						)
						StringCchPrintf( buff, 256, GET_REPLACED_STRING( ( STR_ID_903, "i", repairEDCost ) ) );
					else
						wsprintf( buff, GET_REPLACED_STRING( ( STR_ID_904, "i", repairEDCost ) ) );
				}
				else
				{
					if(true == pItem->IsDisabled() 
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
						&& false == pItem->GetIsEvaluation() 					
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
						)
						StringCchPrintf( buff, 256, GET_REPLACED_STRING( ( STR_ID_905, "ii", repairEDCost, repairVPCost ) ) );
					else
					StringCchPrintf( buff, 256, GET_REPLACED_STRING( ( STR_ID_906, "ii", repairEDCost, repairVPCost ) ) );
				}
#else
				if ( repairVPCost == 0 )
					//wsprintf( buff, GET_REPLACED_STRING( ( STR_ID_904, "i", repairEDCost ) ) );
					StringCchPrintf( buff, 256, GET_REPLACED_STRING( ( STR_ID_904, "i", repairEDCost ) ) );
				else
					//wsprintf( buff, GET_REPLACED_STRING( ( STR_ID_906, "ii", repairEDCost, repairVPCost ) ) );
					StringCchPrintf( buff, 256, GET_REPLACED_STRING( ( STR_ID_906, "ii", repairEDCost, repairVPCost ) ) );
#endif
				
				m_pDLGRepairItemConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), buff, USCM_REPAIR_ITEM_CONFIRM_OK , g_pMain->GetNowState() );
				m_pDLGRepairItemConfirm->SetEnableMoveByDrag_LUA(false);
			}
		}
		else
		{
			CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_907 ), g_pMain->GetNowState() );
			pMsgBox->SetEnableMoveByDrag_LUA(false);
		}
	}
}

bool CX2UIShop::Handler_EGS_REPAIR_ITEM_REQ( UidType ItemUID )
{
	if ( m_pDLGRepairItemConfirm != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRepairItemConfirm, NULL, false );

	m_pDLGRepairItemConfirm = NULL;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ItemUID );
	if ( pItem != NULL )
	{
		int edToRepair = pItem->GetEDToRepair();
		int vpToRepair = pItem->GetVPToRepair();

		float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
		if( fDiscountRate > 0.f )
		{
#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-04-19
			edToRepair = static_cast<int>( edToRepair * fDiscountRate );		/// 수리비 증가 버프로 인한 공식 수정
#else // SERV_NEW_DEFENCE_DUNGEON
			edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
#endif // SERV_NEW_DEFENCE_DUNGEON
		}

		if ( edToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_908 ), g_pMain->GetNowState() );
			return true;
		}
#ifdef SERV_PVP_NEW_SYSTEM
		if ( vpToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_909 ), g_pMain->GetNowState() );
			return true;
		}
#else
		if ( vpToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_909 ), g_pMain->GetNowState() );
			return true;
		}
#endif

		//UidType itemUID = pItem->GetUID();
		KEGS_REPAIR_ITEM_REQ kPacket;
		kPacket.m_vecItemUID.push_back( ItemUID );

		g_pData->GetServerProtocol()->SendPacket( EGS_REPAIR_ITEM_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_REPAIR_ITEM_ACK );
	}

	m_RepairItemUID = 0;
	return true;
}

bool CX2UIShop::Handler_EGS_REPAIR_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REPAIR_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	//// 다이얼로그 처리
	//if ( m_pDLGRepairItemConfirm != NULL )
	//{
	//	g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRepairItemConfirm, NULL, false );
	//	m_pDLGRepairItemConfirm = NULL;
	//}

	SAFE_DELETE_DIALOG( m_pDLGRepairItemConfirm );



	////{{ dmlee 2009.3.31 크래시 디버깅을 위해서 로그 남기기
	//{
	//	WCHAR wszText[512] = L"";


	//	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
	//	if( NULL != pUnit )
	//	{
	//		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"--EquipNormalCount: %d", pUnit->GetNormalEqipNum() );
	//		StateLog( wszText );

	//		const CX2Inventory& kInventory = pUnit->GetInventory();
	//		{
	//			for( int i=0; i< pUnit->GetNormalEqipNum(); i++ )
	//			{
	//				UidType uidEquip = pUnit->GetNormalEqipUID( i );
	//				CX2Item* pItem = kInventory.GetItem( uidEquip );
	//				if( NULL != pItem )
	//				{
	//					StateLog( pItem->GetFullName().c_str() );
	//				}
	//				else
	//				{
	//					StateLog( L"NULL item" );
	//				}
	//			}
	//		}
	//	}


	//	if( true == g_pData->GetPartyManager()->DoIHaveParty() )
	//	{
	//		if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
	//		{
	//			StateLog( L"--PartyLeader" );
	//		}
	//		else
	//		{
	//			StateLog( L"--NotPartyLeader" );
	//		}

	//		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"--PartyMemberCount: %d", (int) g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() );
	//		StateLog( wszText );
	//	}
	//	else
	//	{
	//		StateLog( L"--NotParty" );
	//	}

	//	StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"--RepairItemCount: %d", (int) kEvent.m_vecInventorySlotInfo.size() );
	//	StateLog( wszText );


	//	if( NULL != pUnit )
	//	{
	//		const CX2Inventory& kInventory = pUnit->GetInventory();
	//		{
	//			BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInventoryItemInfo, kEvent.m_vecInventorySlotInfo )
	//			{
	//				CX2Item* pItem = kInventory.GetItem( kInventoryItemInfo.m_iItemUID );
	//				if( NULL != pItem )
	//				{
	//					StateLog( pItem->GetFullName().c_str() );
	//				}
	//				else
	//				{
	//					StateLog( L"NULL item" );
	//				}
	//			}
	//		}
	//	}
	//}
	////}} dmlee 2009.3.31 크래시 디버깅을 위해서 로그 남기기






	g_pMain->DeleteServerPacket( EGS_REPAIR_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
			if ( pUnit != NULL )
			{
				pUnit->AccessUnitData().m_ED = kEvent.m_iED;
#ifdef SERV_PVP_NEW_SYSTEM
				pUnit->AccessUnitData().m_iAPoint = kEvent.m_iAPoint;
#else
				pUnit->AccessUnitData().m_VSPoint = kEvent.m_iVP;
#endif

				if( NULL != g_pData->GetUIManager()->GetUIInventory() )
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
				
				if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
				{
					g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
					g_pData->GetUIManager()->GetUICharInfo()->ResetNowEquipUI();
				}
				//m_pStateMenu->OnResetStateAndEquip();

				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_910 ), g_pMain->GetNowState() );

				if ( g_pTFieldGame != NULL )
				{
					g_pTFieldGame->GetMyUnit()->GetUnitViewer()->UpdateEqip( true );
				}
			}	
		}
	}

	return true;
}

void CX2UIShop::RepairAllEquippedItem()
{
	int edToRepair = 0;
	int vpToRepair = 0;
	bool bAllfull = true;
	m_vecAllEquippingItem.clear();
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	for ( int i = 0; i < kInventory.GetItemMaxNum( CX2Inventory::ST_E_EQUIP ); i++ )
	{
		CX2Item* pItem = kInventory.GetItem( CX2Inventory::ST_E_EQUIP, i );
		if ( pItem == NULL || 
            pItem->GetItemTemplet() == NULL || 
            pItem->GetItemTemplet()->GetPeriodType() != CX2Item::PT_ENDURANCE )
			continue;

		if(pItem->GetItemTemplet()->GetEndurance() != pItem->GetItemData().m_Endurance) bAllfull = false;

		edToRepair += pItem->GetEDToRepair();
		vpToRepair += pItem->GetVPToRepair();
		m_vecAllEquippingItem.push_back( pItem->GetUID() );
	}

	// 커서 보통으로 바꿔주자
	CX2State* pState = (CX2State*)g_pMain->GetNowState();	// DownCast니까 안심
	if(pState != NULL)
	{
		// StateMenu를 상속한 Class가 State일 때만 사용된다고 가정
		CX2Cursor* pCursor = pState->GetCursor();
		if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
		{
			pCursor->ChangeCursorState(CX2Cursor::XCS_NORMAL);
		}
	}

	if (true == bAllfull)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_911 ), g_pMain->GetNowState() );
		return;
	}


	// 할인율 적용
	float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
	if( fDiscountRate > 0.f )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-04-19
		edToRepair = static_cast<int>( edToRepair * fDiscountRate );		/// 수리비 증가 버프로 인한 공식 수정
#else // SERV_NEW_DEFENCE_DUNGEON
		edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
#endif // SERV_NEW_DEFENCE_DUNGEON
	}


	WCHAR buff[256] = {0,};
	if ( vpToRepair == 0 )
		//wsprintf( buff, GET_REPLACED_STRING( ( STR_ID_912, "i", edToRepair ) ) );
		StringCchPrintf( buff, 256, GET_REPLACED_STRING( ( STR_ID_912, "i", edToRepair ) ) );
	else
		//wsprintf( buff, GET_REPLACED_STRING( ( STR_ID_913, "ii", edToRepair, vpToRepair ) ) );
		StringCchPrintf( buff, 256, GET_REPLACED_STRING( ( STR_ID_913, "ii", edToRepair, vpToRepair ) ) );

	m_pDLGRepairItemConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), buff, USCM_REPAIR_ITEM_ALL_CONFIRM_OK, g_pMain->GetNowState(), USCM_REPAIR_ITEM_ALL_CONFIRM_CANCLE );
}


void CX2UIShop::AllEquippingItemRepairREQ()
{
	if ( m_pDLGRepairItemConfirm != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRepairItemConfirm, NULL, false );

	m_pDLGRepairItemConfirm = NULL;

	KEGS_REPAIR_ITEM_REQ kPacket;
	kPacket.m_vecItemUID = m_vecAllEquippingItem;

	int edToRepair = 0;
	int vpToRepair = 0;

	for ( int i = 0; i < (int)m_vecAllEquippingItem.size(); i++ )
	{
		UidType itemUID = m_vecAllEquippingItem[i];
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID );
		if ( pItem == NULL || 
            pItem->GetItemTemplet() == NULL || 
            pItem->GetItemTemplet()->GetPeriodType() != CX2Item::PT_ENDURANCE )
			continue;

		edToRepair += pItem->GetEDToRepair();
		vpToRepair += pItem->GetVPToRepair();
	}

	// 할인율 적용
	float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
	if( fDiscountRate > 0.f )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-04-19
		edToRepair = static_cast<int>( edToRepair * fDiscountRate );		/// 수리비 증가 버프로 인한 공식 수정
#else // SERV_NEW_DEFENCE_DUNGEON
		edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
#endif // SERV_NEW_DEFENCE_DUNGEON
	}


	if ( edToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_908 ), g_pMain->GetNowState() );
		return;
	}
#ifdef SERV_PVP_NEW_SYSTEM
	if ( vpToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_909 ), g_pMain->GetNowState() );
		return;
	}
#else
	if ( vpToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_909 ), g_pMain->GetNowState() );
		return;
	}
#endif

	g_pData->GetServerProtocol()->SendPacket( EGS_REPAIR_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_REPAIR_ITEM_ACK );

	m_pDLGRepairItemConfirm = NULL;
}

// 주의 : 사용시 NULL CHECK!
CKTDGUIRadioButton* CX2UIShop::GetRadioButtonNameByCategory( BUY_ITEM_CATEGORY buyItemCategory )
{
    if( m_pDLGShop == NULL )
        return NULL;

	CKTDGUIRadioButton* pRButton = NULL;
	switch(buyItemCategory)
	{
	case BIC_WEAPON:
		pRButton = (CKTDGUIRadioButton*) m_pDLGShop->GetControl( L"RadioButton_Weapon" );
		break;
	case BIC_COAT:
		pRButton = (CKTDGUIRadioButton*) m_pDLGShop->GetControl( L"RadioButton_Coat" );
		break;
	case BIC_TROUSERS:
		pRButton = (CKTDGUIRadioButton*) m_pDLGShop->GetControl( L"RadioButton_Pants" );
		break;
	case BIC_HAND:
		pRButton = (CKTDGUIRadioButton*) m_pDLGShop->GetControl( L"RadioButton_Glove" );
		break;
	case BIC_FOOTWEAR:
		pRButton = (CKTDGUIRadioButton*) m_pDLGShop->GetControl( L"RadioButton_Shoes" );
		break;
	case BIC_SPECIAL:
		pRButton = (CKTDGUIRadioButton*) m_pDLGShop->GetControl( L"RadioButton_Special" );
		break;
	case BIC_ACCESSORY:
		pRButton = (CKTDGUIRadioButton*) m_pDLGShop->GetControl( L"RadioButton_Accessory" );
		break;
	case BIC_QUICK_SLOT:
		pRButton = (CKTDGUIRadioButton*) m_pDLGShop->GetControl( L"RadioButton_QuickSlot" );
		break;
	default:
		break;
	}
	return pRButton;
}

bool CX2UIShop::IsPossibleUsedByMyCharacter( const CX2Item::ItemTemplet* pItemTemplet )
{
	if ( pItemTemplet == NULL )
		return false;

	return CX2Unit::CanUse( 
        pItemTemplet->GetItemID(), 
        g_pData->GetMyUser()->GetSelectUnit() );
}

//////////////////////////////////////////////////////////////////////////
// 구매
//////////////////////////////////////////////////////////////////////////
int CX2UIShop::PreCheckShopBuyItem()
{
	BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];
	int price = pSlotInfo->m_pItemTemplet->GetPrice();
	int myED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	unsigned int iEmptyInvenSlot = 0;		// 인벤토리 빈 공간

#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	int iCurrItemNum = 0;			/// 해당 아이템 소지 갯수
	int iCurrSlotNum = 0;			/// 해당 아이템이 차지하고 있는 슬롯 갯수
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	int iAddItemNum = 0;			// 아이템이 존재할 경우 추가적으로 더해질 아이템 수

	{
		// 아이템 정보로 인벤토리의 정확한 탭(7개중) 빈 슬롯 개수 얻기
		CX2Inventory::SORT_TYPE eInvenType = GetCurrInventoryType( pSlotInfo->m_pItemTemplet->GetItemType() );
		iEmptyInvenSlot = kInventory.GetItemMaxNum( eInvenType ) - kInventory.GetUsedSlotNum( eInvenType );

		//{{ mauntain : 김태환 [2012.07.16] 인벤토리 및 슬롯을 고려한 아이템 구입 갯수 연산
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		if( CX2Inventory::ST_QUICK_SLOT == eInvenType )
		{
			if( NULL != g_pData->GetUIManager() )
			{
				CX2UIQuickSlot* pQuickSlot = g_pData->GetUIManager()->GetUIQuickSlot();

				if( NULL != pQuickSlot )	/// 소비 아이템일 때만 퀵슬롯 빈칸 추가 연산
				{
					iEmptyInvenSlot += pQuickSlot->GetUsedSlotMaxNum() - pQuickSlot->GetUsedSlotNum();
				}
			}
		}
        iCurrItemNum = kInventory.GetNumItemByTID( pSlotInfo->m_pItemTemplet->GetItemID() );	/// 해당 아이템 소지 갯수
		iCurrSlotNum = kInventory.GetNumSlotByTID( pSlotInfo->m_pItemTemplet->GetItemID() );	/// 해당 아이템이 차지하고 있는 슬롯 갯수
#else  SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM

        int iCurrItemNum = kInventory.GetNumItemByTID( pSlotInfo->m_pItemTemplet->GetItemID() );

		// 인벤토리 내에 구입할 아이템과 동일한 아이템이 있을 경우 개수 파악 후 추가적으로 구입할 수 있는 개수 파악
		// 구입할 아이템이 인벤토리에 있는 개수
		if ( iCurrItemNum > 0 )
		{
			if ( iCurrItemNum < pSlotInfo->m_pItemTemplet->GetQuantity() )
			{
				iAddItemNum = pSlotInfo->m_pItemTemplet->GetQuantity() - iCurrItemNum;
			}
			else if ( iCurrItemNum > pSlotInfo->m_pItemTemplet->GetQuantity() )
			{
				iAddItemNum = iCurrItemNum;

				while ( iAddItemNum > pSlotInfo->m_pItemTemplet->GetQuantity() )
				{
					iAddItemNum -= pSlotInfo->m_pItemTemplet->GetQuantity();
				}
				iAddItemNum = pSlotInfo->m_pItemTemplet->GetQuantity() - iAddItemNum;
			}
			else
			{
				iAddItemNum = 0;
			}
		}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}
	}

	//{{ mauntain : 김태환 [2012.07.16] 인벤토리 및 슬롯을 고려한 아이템 구입 갯수 연산
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	iAddItemNum = ( pSlotInfo->m_pItemTemplet->GetQuantity() * iEmptyInvenSlot ) + ( pSlotInfo->m_pItemTemplet->GetQuantity() * iCurrSlotNum - iCurrItemNum );
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

	// 소지금 부족
	if( price > myED )
		return 1;

	// 인벤 부족
	if( iEmptyInvenSlot <= 0 && iAddItemNum <= 0 )
		return 2;

	// Can Buy
	return 0;
}
// 이 코드는 앞으로 자주 쓰게 될 것이야
void CX2UIShop::OpenItemBuyNumDLG( D3DXVECTOR2 pos )
{

	int iRet = PreCheckShopBuyItem();
	if( iRet > 0 )
	{
		BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];
		const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

		if( iRet == 1 )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_918 ), g_pMain->GetNowState() );
			return;
		}
		else if( iRet == 2 )
		{
			wstring invenSortTypeName = L"";
			{
				invenSortTypeName = kInventory.GetInvenSortTypeName( GetCurrInventoryType( pSlotInfo->m_pItemTemplet->GetItemType() ) );
			}

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_REPLACED_STRING( ( STR_ID_11373, "L", invenSortTypeName ) ), g_pMain->GetNowState() );
			return;
		}
	}
	

	SAFE_DELETE_DIALOG(m_pDLGBuyItemNum);
	m_pDLGBuyItemNum = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_An.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBuyItemNum );
// 	pos = pos - D3DXVECTOR2(75, 20);
// 	if(pos.x > 865 )
// 		pos.x = 865;
// 	if(pos.y > 633 )
// 		pos.y = 633;
// 	m_pDLGBuyItemNum->SetPos( pos );
	m_pDLGBuyItemNum->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-865, 768-633), D3DXVECTOR2(75,20)) );

	CKTDGUIStatic* pStaticPrice = (CKTDGUIStatic*) m_pDLGBuyItemNum->GetControl( L"Static_Price" );

	CKTDGUIButton* pButtonOK = (CKTDGUIButton*) m_pDLGBuyItemNum->GetControl( L"Button_OK" );
	CKTDGUIButton* pButtonCancle = (CKTDGUIButton*) m_pDLGBuyItemNum->GetControl( L"Button_Cancle" );
	CKTDGUIButton* pButtonPlus = (CKTDGUIButton*) m_pDLGBuyItemNum->GetControl( L"Button_Plus" );
	CKTDGUIButton* pButtonMinus = (CKTDGUIButton*) m_pDLGBuyItemNum->GetControl( L"Button_Minus" );
	CKTDGUIIMEEditBox* pQuantity = (CKTDGUIIMEEditBox*) m_pDLGBuyItemNum->GetControl( L"IME_Edit_Quantity" );
	m_BuyItemNum = 1;

	pButtonOK->SetCustomMsgMouseUp(USCM_BUY_ITEM_QUANTITY_CONFIRM);
	pButtonCancle->SetCustomMsgMouseUp(USCM_BUY_ITEM_QUANTITY_CANCLE);
	pButtonPlus->SetCustomMsgMouseUp(USCM_BUY_ITEM_QUANTITY_PLUS);
	pButtonMinus->SetCustomMsgMouseUp(USCM_BUY_ITEM_QUANTITY_MINUS);
	pQuantity->SetCustomMsgChange(USCM_BUY_ITEM_QUANTITY_UPDATE);
	pQuantity->SetCustomMsgEnter(USCM_BUY_ITEM_QUANTITY_CONFIRM);
	m_pDLGBuyItemNum->SetCloseCustomUIEventID(USCM_BUY_ITEM_QUANTITY_CANCLE);

	UpdateItemBuyNumDLG(false, true);

	pQuantity->RequestFocus();
	pQuantity->OnFocusIn();

}

void CX2UIShop::UpdateItemBuyNumDLG( bool bReadIME, bool bTextSelected )
{
	ASSERT( m_pDLGBuyItemNum != NULL );
	if( m_pDLGBuyItemNum == NULL )
		return; 


	CKTDGUIIMEEditBox* pQuantity = (CKTDGUIIMEEditBox*) m_pDLGBuyItemNum->GetControl( L"IME_Edit_Quantity" );

	BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];
	int price = pSlotInfo->m_pItemTemplet->GetPrice();
	int priceVP = pSlotInfo->m_pItemTemplet->GetPricePvPPoint();
	int myED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
#ifdef SERV_PVP_NEW_SYSTEM
	int myVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint;
#else
	int myVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
#endif
	int MaxBuyItemNum = 0;
	int MaxBuyItemVP = 0;
	//{{ JHKang / 강정훈 / 2010/10/11 / 현재 아이템이 들어갈 인벤토리의 빈 슬롯 개수 구하기
#ifdef MODIFY_SHOP_BUY_ITEM
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	unsigned int iEmptyInvenSlot = 0;		// 인벤토리 빈 공간

#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	int iCurrItemNum = 0;			/// 해당 아이템 소지 갯수
	int iCurrSlotNum = 0;			/// 해당 아이템이 차지하고 있는 슬롯 갯수
#else  SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	int iAddItemNum = 0;			// 아이템이 존재할 경우 추가적으로 더해질 아이템 수
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	
	{
		// 아이템 정보로 인벤토리의 정확한 탭(7개중) 빈 슬롯 개수 얻기
		CX2Inventory::SORT_TYPE eInvenType = GetCurrInventoryType( pSlotInfo->m_pItemTemplet->GetItemType() );
		iEmptyInvenSlot = kInventory.GetItemMaxNum( eInvenType ) - kInventory.GetUsedSlotNum( eInvenType );

		//{{ mauntain : 김태환 [2012.07.16] 인벤토리 및 슬롯을 고려한 아이템 구입 갯수 연산
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		if( CX2Inventory::ST_QUICK_SLOT == eInvenType )
		{
			if( NULL != g_pData->GetUIManager() )
			{
				CX2UIQuickSlot* pQuickSlot = g_pData->GetUIManager()->GetUIQuickSlot();

				if( NULL != pQuickSlot )	/// 소비 아이템일 때만 퀵슬롯 빈칸 추가 연산
				{
					iEmptyInvenSlot += pQuickSlot->GetUsedSlotMaxNum() - pQuickSlot->GetUsedSlotNum();
				}
			}
		}

		iCurrItemNum = kInventory.GetNumItemByTID( pSlotInfo->m_pItemTemplet->GetItemID() );	/// 해당 아이템 소지 갯수
		iCurrSlotNum = kInventory.GetNumSlotByTID( pSlotInfo->m_pItemTemplet->GetItemID() );	/// 해당 아이템이 차지하고 있는 슬롯 갯수

#else  SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM

        int iCurrItemNum = kInventory.GetNumItemByTID( pSlotInfo->m_pItemTemplet->GetItemID() );

		// 인벤토리 내에 구입할 아이템과 동일한 아이템이 있을 경우 개수 파악 후 추가적으로 구입할 수 있는 개수 파악
		// 구입할 아이템이 인벤토리에 있는 개수
		if ( iCurrItemNum > 0 )
		{
			if ( iCurrItemNum < pSlotInfo->m_pItemTemplet->GetQuantity() )
			{
				iAddItemNum = pSlotInfo->m_pItemTemplet->GetQuantity() - iCurrItemNum;
			}
			else if ( iCurrItemNum > pSlotInfo->m_pItemTemplet->GetQuantity() )
			{
				iAddItemNum = iCurrItemNum;
				
				while ( iAddItemNum > pSlotInfo->m_pItemTemplet->GetQuantity() )
				{
					iAddItemNum -= pSlotInfo->m_pItemTemplet->GetQuantity();
				}
				iAddItemNum = pSlotInfo->m_pItemTemplet->GetQuantity() - iAddItemNum;
			}
			else
			{
				iAddItemNum = 0;
			}
		}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}

	}
#endif MODIFY_SHOP_BUY_ITEM
	//}} JHKang / 강정훈 / 2010/10/11 / 현재 아이템이 들어갈 인벤토리의 빈 슬롯 개수 구하기

	if( price != 0 )
	{
		//{{ JHKang / 강정훈 / 2010/10/11 / 현재 ED와 인벤토리 빈 슬롯을 고려한 구입 개수
#ifdef MODIFY_SHOP_BUY_ITEM

		//{{ mauntain : 김태환 [2012.07.16] 인벤토리 및 슬롯을 고려한 아이템 구입 갯수 연산
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		/// 빈 슬롯에 들어갈 수 있는 갯수 + 해당 아이템이 차지하고 있는 슬롯에 들어갈 수 있는 갯수
		MaxBuyItemNum = ( pSlotInfo->m_pItemTemplet->GetQuantity() * iEmptyInvenSlot ) + ( pSlotInfo->m_pItemTemplet->GetQuantity() * iCurrSlotNum - iCurrItemNum );
#else  SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		MaxBuyItemNum = pSlotInfo->m_pItemTemplet->GetQuantity() * iEmptyInvenSlot + iAddItemNum;
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}

		const int iItemNumWhichICanBuy = static_cast<int>( myED / price );
		if ( MaxBuyItemNum > iItemNumWhichICanBuy )
			MaxBuyItemNum = iItemNumWhichICanBuy;
#else
		MaxBuyItemNum = min(pSlotInfo->m_pItemTemplet->GetQuantity(), myED / price );
#endif MODIFY_SHOP_BUY_ITEM
		//}} JHKang / 강정훈 / 2010/10/11 / 현재 ED와 인벤토리 빈 슬롯을 고려한 구입 개수
	}
	else
	{
		MaxBuyItemNum = pSlotInfo->m_pItemTemplet->GetQuantity();
	}
	
	if( priceVP != 0 )
	{
		MaxBuyItemVP = min(pSlotInfo->m_pItemTemplet->GetQuantity(), myVP / priceVP );		
	}
	else
	{
		MaxBuyItemVP = pSlotInfo->m_pItemTemplet->GetQuantity();
	}

	//{{ JHKang / 강정훈 / 2010/10/12 / 구입 개수 변경에 따른 VP 고려
#ifdef MODIFY_SHOP_BUY_ITEM
	int MaxBuyItem = MaxBuyItemNum;

	if ( pSlotInfo->m_pItemTemplet->GetShopPriceType() == CX2Item::SPT_GP && pSlotInfo->m_pItemTemplet->GetPricePvPPoint() > 0 )
		MaxBuyItem = min(MaxBuyItemNum, MaxBuyItemVP);
#else
	int MaxBuyItem = min(MaxBuyItemNum, MaxBuyItemVP);
#endif MODIFY_SHOP_BUY_ITEM
	//}} JHKang / 강정훈 / 2010/10/12 / 구입 개수 변경에 따른 VP 고려
	
	if(bReadIME)
	{
		m_BuyItemNum = g_pMain->GetEDFromString( pQuantity->GetText() );
	}	
	
	if ( m_BuyItemNum <= 0 )
	{
		if(bReadIME)				// 입력창으로 받아온 경우
			m_BuyItemNum = 0;
		else						// << 를 누른 경우 : 0 아래면 max로 순환시킨다.
			m_BuyItemNum = MaxBuyItem;	
	}
	
	if ( m_BuyItemNum >= MaxBuyItem )
		m_BuyItemNum = MaxBuyItem;

	WCHAR buff[256] = {0};
	_itow( m_BuyItemNum, buff, 10 );
	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str(), bTextSelected );

	// 구매가
	CKTDGUIStatic* pStaticPrice = (CKTDGUIStatic*) m_pDLGBuyItemNum->GetControl( L"Static_Price" );
	pStaticPrice->GetString(0)->msg = g_pMain->GetEDString(m_BuyItemNum * price );

}

void CX2UIShop::OpenBuyItemConfirmDLG()
{
	if(m_pDLGBuyItemConfirm != NULL)
    {
#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
        CKTDGUIDialog::Destroy( m_pDLGBuyItemConfirm.GetHandle() );
        m_pDLGBuyItemConfirm.Invalidate();
#else
        SAFE_DELETE(m_pDLGBuyItemConfirm);
#endif // DIALOG_HANDLE_TEST // 2009-8-19
    }

	int popUpSizeX = 425;
	int popUpSizeY = 147;

	if ( m_BuyItemSlotNum == -1 )
		return;

	BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];
	int price = pSlotInfo->m_pItemTemplet->GetPrice();
	
	if ( pSlotInfo != NULL )
	{
		if ( pSlotInfo->m_pItemTemplet != NULL )
		{

			wstringstream wstrstm;
            wstring itemFullName = pSlotInfo->m_pItemTemplet->GetFullName_();
			int price = pSlotInfo->m_pItemTemplet->GetPrice();

			wstrstm << GET_REPLACED_STRING( ( STR_ID_914, "LIi", itemFullName,
                KHanSungPostWordUnicodeWrapper( itemFullName.c_str(), STR_ID_198, STR_ID_199 ), m_BuyItemNum ) );

			// 구매에 VP가 필요한지 아닌지 확인하고
			if( pSlotInfo->m_pItemTemplet->GetShopPriceType() == CX2Item::SPT_GP && pSlotInfo->m_pItemTemplet->GetPricePvPPoint() > 0 ) // 필요하면
			{
#ifdef SERV_PVP_NEW_SYSTEM
				wstrstm << GET_REPLACED_STRING( ( STR_ID_915, "LLLL",
					g_pMain->GetEDString( pSlotInfo->m_pItemTemplet->GetPricePvPPoint() * m_BuyItemNum ),
					g_pMain->GetEDString( price * m_BuyItemNum ),
					g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint ),
					g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#else
                wstrstm << GET_REPLACED_STRING( ( STR_ID_915, "LLLL",
				            g_pMain->GetEDString( pSlotInfo->m_pItemTemplet->GetPricePvPPoint() * m_BuyItemNum ),
                            g_pMain->GetEDString( price * m_BuyItemNum ),
                            g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint ),
                            g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#endif

			}
			else	// 필요없으면 ( m_pricetype이 SPT_GP가 아닌 경우도 포함 )
			{
				wstrstm << GET_REPLACED_STRING( ( STR_ID_916, "LL",
                    g_pMain->GetEDString( price * m_BuyItemNum ),
                    g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
			}

			m_pDLGBuyItemConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), 
				wstrstm.str().c_str(), USCM_BUY_ITEM_CONFIRM_OK , g_pMain->GetNowState(), USCM_BUY_ITEM_CONFIRM_CANCLE );
			m_pDLGBuyItemConfirm->SetEnableMoveByDrag_LUA(false);
	
		}
	}
}



bool CX2UIShop::Handler_EGS_BUY_ED_ITEM_REQ()
{
	if ( m_BuyItemSlotNum == -1 )
		return false;

	BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];
	if( pSlotInfo == NULL)
	{
		return false;
	}

	const CX2Item::ItemTemplet* pkItemTemplet = pSlotInfo->m_pItemTemplet;	
	if( NULL == pkItemTemplet )
	{
		return false;
	}
	int BuyItemTID = pkItemTemplet->GetItemID();

	if( NULL == g_pData ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return false;
	}

	int iCurGP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
	if( m_BuyItemNum < 1)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_917 ), g_pMain->GetNowState() );
		m_BuyItemNum = 0;
		m_BuyItemSlotNum = 0;

		return true;
	}
	if ( iCurGP < (pkItemTemplet->GetPrice()*m_BuyItemNum) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_918 ), g_pMain->GetNowState() );
		m_BuyItemNum = 0;
		m_BuyItemSlotNum = 0;

		return true;
	}

#ifdef SERV_PVP_NEW_SYSTEM
	int iCurVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint;
#else
	int iCurVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
#endif
	if ( iCurVP < (pkItemTemplet->GetPricePvPPoint()*m_BuyItemNum) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_919 ), g_pMain->GetNowState() );

		m_BuyItemNum = 0;
		m_BuyItemSlotNum = 0;
		return true;
	}


	KEGS_BUY_ED_ITEM_REQ kPacket;
	kPacket.m_bDebug = false;
	kPacket.m_mapItem.insert( std::make_pair( BuyItemTID, m_BuyItemNum ) );

	g_pData->GetServerProtocol()->SendPacket( EGS_BUY_ED_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BUY_ED_ITEM_ACK );

	return true;
}

bool CX2UIShop::Handler_EGS_BUY_ED_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BUY_ED_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_BUY_ED_ITEM_ACK ) )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED		= kEvent.m_iED;
#ifdef SERV_PVP_NEW_SYSTEM
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_iAPoint = kEvent.m_iAPoint;
#else
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_VSPoint = kEvent.m_iVSPoint;
#endif
			if(g_pData->GetUIManager()->GetUIInventory() != NULL)
			{		
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			}


			if( m_BuyItemSlotNum >= 0 && m_BuyItemSlotNum < (int)m_vecBuySlot.size() )
			{
				BuySlot* pSlotInfo = m_vecBuySlot[m_BuyItemSlotNum];
				if(pSlotInfo != NULL)
				{
					const CX2Item::ItemTemplet* pkItemTemplet = pSlotInfo->m_pItemTemplet;
					if(pkItemTemplet != NULL)
					{
						wstring wstrMsg;
						if(CX2Item::PT_QUANTITY == pkItemTemplet->GetPeriodType())
						{
                            wstrMsg = GET_REPLACED_STRING( ( STR_ID_920, "Si", pkItemTemplet->GetFullName_(), m_BuyItemNum ) );
						}
						else
						{
                            wstrMsg = GET_REPLACED_STRING( ( STR_ID_921, "S", pkItemTemplet->GetFullName_() ) );
						}

						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999,-999 ), wstrMsg.c_str(), g_pMain->GetNowState() );
					}				
				}		
			}

			m_BuyItemNum = 0;
			m_BuyItemSlotNum = 0;

			// 미리보기 관련해 구 UI에 있던 코드.
// 			m_pkShopCharStatus->ResetStateAndEquip();
// 			m_pkShopCharStatus->ResetCharacterRoomMyInfo();
		}

		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////

void CX2UIShop::EnchantItem( UidType EnchantItemUID, D3DXVECTOR2 pos )
{
	// 고급강화 저장되지 않게
	m_bUseEnchantAdjuvant = false;

	m_EnchantItemUID = EnchantItemUID;

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pItem = kInventory.GetItem( m_EnchantItemUID );

    if( NULL == pItem || pItem->GetItemTemplet() == NULL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_922 ), g_pMain->GetNowState() );
		return; 
	}

	if( pItem->GetItemTemplet()->GetItemType() != CX2Item::IT_WEAPON && pItem->GetItemTemplet()->GetItemType() != CX2Item::IT_DEFENCE )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_798 ), g_pMain->GetNowState() );
		return;
	}
	
	if( pItem->GetItemTemplet()->GetFashion() == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_923 ), g_pMain->GetNowState() );
		return;
	}

#ifdef ITEM_RECOVERY_TEST
	// 강화 레벨 검사 (사용불능인가?)
	if(true == pItem->IsDisabled())
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_800 ), g_pMain->GetNowState() );
		return; 
	}
#endif //ITEM_RECOVERY_TEST
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	else if ( false == pItem->GetIsEvaluation() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_24617 ), g_pMain->GetNowState() );
		return; 
	}
#endif  //SERV_NEW_ITEM_SYSTEM_2013_05 

	//{{ kimhc // 2009-09-08 // 봉인된 아이템 강화 불가
#ifdef	SEAL_ITEM
	if ( pItem->GetItemData().m_bIsSealed == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4476 ), g_pMain->GetNowState() );
		return; 
	}
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-08 // 봉인된 아이템 강화 불가

	//wstring normalButtonDesc	= L"";
	//wstring newButtonDesc		= L"";
	WCHAR wszEnchantED[256]		= L"";

	int normalStoneCount	= 0;
	int newStoneCount		= 0;
	int iNewStoneLevel		= GetEnchantStoneLevel( pItem->GetItemTemplet()->GetUseLevel() );
	
	//{{ kimhc // 2010-01-06 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
	if ( pItem->GetItemTemplet()->GetIsPcBang() == true )
	{
		g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
		return;
	}
#endif	PC_BANG_WORK
	//}} kimhc // 2010-01-06 // PC방 프리미엄 서비스

	switch( pItem->GetItemTemplet()->GetItemType() )
	{
	case CX2Item::IT_WEAPON:
		{
			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
#ifdef SERV_BLESSED_RURIEL_ENCHANT_STONE_EVENT
			normalStoneCount += g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( 152000121 );
#endif // SERV_BLESSED_RURIEL_ENCHANT_STONE_EVENT
			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );

			if ( normalStoneCount <= 0 && newStoneCount <= 0 )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
					GET_STRING( STR_ID_924 ), g_pMain->GetNowState() );

				return;
			}

		} break;

	case CX2Item::IT_DEFENCE:
		{
			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
#ifdef SERV_BLESSED_RURIEL_ENCHANT_STONE_EVENT
			normalStoneCount += g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( 152000122 );
#endif // SERV_BLESSED_RURIEL_ENCHANT_STONE_EVENT
			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );

			if ( normalStoneCount <= 0 && newStoneCount <= 0 )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
					GET_STRING( STR_ID_925 ), g_pMain->GetNowState() );

				return;
			}
		} break;

	default:
		{
			return;
		} break;
	}

	OpenEnchantDLG( pos );

}

void CX2UIShop::OpenEnchantDLG(D3DXVECTOR2 pos)
{
	// 다이얼로그 자동 클릭되는 현상을 막자
	g_pData->GetUIManager()->SetDLGFlag(false);

	if ( m_pDLGEnchantItem != NULL )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGEnchantItem, NULL, false );
	}

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pItem = kInventory.GetItem( m_EnchantItemUID );
	//wstring normalButtonDesc	= L"";
	//wstring newButtonDesc		= L"";
	WCHAR wszEnchantED[256]		= L"";
	
	m_pDLGEnchantItem = new CKTDGUIDialog(  g_pMain->GetNowState(), L"DLG_UI_Item_Upgrade.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEnchantItem );

	//위치 잡아주고
// 	pos = pos - D3DXVECTOR2( 185, 265 );
// 	if(pos.x > 656) 
// 		pos.x = 656;
// 	if(pos.y > 406)
// 		pos.y = 406;
// 	m_pDLGEnchantItem->SetPos( pos );
	m_pDLGEnchantItem->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-656, 768-406), D3DXVECTOR2(185,265)) );

	// 강화 비용
	int iCost = 0;
	switch( pItem->GetItemTemplet()->GetItemType() )
	{
	case CX2Item::IT_WEAPON:
		{
			iCost = (int)( ( (float)pItem->GetItemTemplet()->GetPrice() * 0.1f ) * 0.5f );
		}
		break;

	case CX2Item::IT_DEFENCE:
		{
			iCost = (int)( ( (float)pItem->GetItemTemplet()->GetPrice() * 0.1f ) * 0.1f );
		}
		break;
	}

	StringCchPrintfW( wszEnchantED, ARRAY_SIZE(wszEnchantED), L"%d", iCost );
	CKTDGUIStatic* pStaticEnchantED = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Enchant_Price" );
	if ( pStaticEnchantED != NULL && pStaticEnchantED->GetString(0) != NULL )
	{
		pStaticEnchantED->GetString(0)->msg = g_pMain->GetEDString( wszEnchantED );  
	}


	// 강화 대상 아이템 이름
	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Item_Name" );
	if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
	{
		wstringstream wstrstm;
		wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << 
            pItem->GetItemTemplet()->GetFullName_()
            ;
#ifdef FIX_TOOLTIP
		wstring wstrTemp;
		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );
		int strWidth = (int)( pItemDescFont->GetWidth( wstrstm.str().c_str() ) / g_pKTDXApp->GetResolutionScaleX() );
#ifdef CLIENT_GLOBAL_LINEBREAK

#ifdef LINE_COUNT_FOR_BR
		if ( strWidth > 140 )
		{
			wstrTemp = CWordLineHandler::GetStrByLineBreakColorInX2Main( wstrstm.str().c_str(), 140, pStaticItemName->GetString(0)->fontIndex );
		}
#else // LINE_COUNT_FOR_BR
		if ( strWidth > 190 )	// 2011.01.03 조효진 140-> 190 으로 수정
		{
			//{{ 2010/10/28 조효진	LineBreak 정리 작업
#ifdef UNIQUENESS_EU_ONLY
			wstrTemp = CWordLineHandler::GetStrByLineBreakColorInX2Main( wstrstm.str().c_str(), 200, pStaticItemName->GetString(0)->fontIndex );
#else //UNIQUENESS_EU_ONLY
			wstrTemp = CWordLineHandler::GetStrByLineBreakColorInX2Main( wstrstm.str().c_str(), 190, pStaticItemName->GetString(0)->fontIndex );
#endif 	//UNIQUENESS_EU_ONLY			
		}
#endif // LINE_COUNT_FOR_BR			
#else //CLIENT_GLOBAL_LINEBREAK
		if ( strWidth > 140 )
		{
			wstrTemp = g_pMain->GetStrByLineBreakColor( wstrstm.str().c_str(), 140, SLOT_MANAGER_FONT_INDEX );
			pStaticItemName->SetOffsetPos(D3DXVECTOR2(0, -13));
		}
#endif //CLIENT_GLOBAL_LINEBREAK
		else
		{
			wstrTemp = wstrstm.str().c_str();
		}

		pStaticItemName->SetString( 0, wstrTemp.c_str() );
#else
		pStaticItemName->GetString(0)->msg = wstrstm.str().c_str();
#endif FIX_TOOLTIP
	}

	// 강화 대상 아이템 그림
	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"g_pStatic_item_Slot_Image" );
	if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
	{
        const wchar_t* pwszShopImage = pItem->GetItemTemplet()->GetShopImage();
		if ( pwszShopImage[0] != NULL )
			pStaticSlot->GetPicture(0)->SetTex( pwszShopImage );
		else
			pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
	}

	// 강화 대상 아이템 현재 강화 레벨 표시
	// 일단 다 꺼주고
	CKTDGUIStatic* pStaticNum;
	pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_UpgradeLevel_SingleDigit" );
	if ( pStaticNum != NULL )
	{
		int PicNum = pStaticNum->GetDummyInt(0);
		for ( int i = 0; i <= PicNum; i++ )
		{
			if ( pStaticNum->GetPicture( i ) != NULL )
			{
				pStaticNum->GetPicture( i )->SetShow( false );
			}
		}
	}
	pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_UpgradeLevel_DoubleDigit_1" );
	if ( pStaticNum != NULL )
	{
		int PicNum = pStaticNum->GetDummyInt(0);
		for ( int i = 0; i <= PicNum; i++ )
		{
			if ( pStaticNum->GetPicture( i ) != NULL )
			{
				pStaticNum->GetPicture( i )->SetShow( false );
			}
		}
	}
	pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_UpgradeLevel_DoubleDigit_10" );
	if ( pStaticNum != NULL )
	{
		int PicNum = pStaticNum->GetDummyInt(0);
		for ( int i = 0; i <= PicNum; i++ )
		{
			if ( pStaticNum->GetPicture( i ) != NULL )
			{
				pStaticNum->GetPicture( i )->SetShow( false );
			}
		}
	}


	if( pItem->GetItemData().m_EnchantLevel < 10)	// 현재 10렙 아래일 때 : 1자리 스태틱으로 해결 보자
	{
		pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_UpgradeLevel_SingleDigit" );
		if( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
			pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );		
	}
	else
	{
		CKTDGUIStatic* pStaticNum1 = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_UpgradeLevel_DoubleDigit_1" );
		CKTDGUIStatic* pStaticNum10 = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_UpgradeLevel_DoubleDigit_10" );
		if( pStaticNum10->GetPicture( (pItem->GetItemData().m_EnchantLevel)/10 ) != NULL )
			pStaticNum10->GetPicture( (pItem->GetItemData().m_EnchantLevel)/10 )->SetShow( true );
		if( pStaticNum1->GetPicture( (pItem->GetItemData().m_EnchantLevel)%10 ) != NULL )
			pStaticNum1->GetPicture( (pItem->GetItemData().m_EnchantLevel)%10 )->SetShow( true );
	}
	// 파괴를 대비(^^;)해서 몇레벨 템이었는지 적어두고
	m_EnchantLevelBefore = pItem->GetItemData().m_EnchantLevel;

	// 개발자용 치트 체크박스
	CKTDGUICheckBox* pCheckDev = (CKTDGUICheckBox*)m_pDLGEnchantItem->GetControl( L"CheckBox_DevCheat" );
	pCheckDev->SetShowEnable(false, false);
	CKTDGUIStatic* pStaticCheckDev = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_DevCheat" );
	pStaticCheckDev->SetShowEnable(false, false);
	if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
	{	
		pCheckDev->SetShowEnable(true, true);	
		pCheckDev->SetCheckedPure(false);
		pStaticCheckDev->SetShowEnable(true, true);
	}

	// 해당하는 강화 버튼을 활성화 해준다
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_REQ();
#else
	UpdateEnchantWindow();
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT



}


// 내 인벤토리에 있는 강화 보조제 개수 
void CX2UIShop::GetRequiredEnchantAdjuvantItemID( IN const int iItemLevel_, OUT vector<int>& vecAdjuvantIdList_ )
{
	if( iItemLevel_ <= 20 )
	{
		vecAdjuvantIdList_.push_back( 206730 );
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 60005520 );	// 이벤트용 플루오르 스톤 레벨 1
		vecAdjuvantIdList_.push_back( 85001937 );	// 이벤트용 플루오르 스톤 레벨 1
		vecAdjuvantIdList_.push_back( 70001910 );	// 플루오르 스톤 이벤트용 레벨 1 JP		
		vecAdjuvantIdList_.push_back( 83000140 ); // 2012.07.24 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 1
		vecAdjuvantIdList_.push_back( 83000190 ); // 2012.12.13 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 1
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
#ifdef CHILDRENS_DAY_EVENT_ITEM
		vecAdjuvantIdList_.push_back( 160786 ); // 플루오르 스톤 이벤트용 레벨 1
#endif //CHILDRENS_DAY_EVENT_ITEM
	}
	else if( iItemLevel_ <= 30 )
	{
		vecAdjuvantIdList_.push_back( 206740 );
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 60005530 );	// 이벤트용 플루오르 스톤 레벨 2
		vecAdjuvantIdList_.push_back( 85001938 );	// 이벤트용 플루오르 스톤 레벨 2
		vecAdjuvantIdList_.push_back( 70001911 );	// 플루오르 스톤 이벤트용 레벨 2	JP
		vecAdjuvantIdList_.push_back( 83000141 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 2
		vecAdjuvantIdList_.push_back( 83000191 ); // 2012.12.13 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 1
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
#ifdef CHILDRENS_DAY_EVENT_ITEM
		vecAdjuvantIdList_.push_back( 160787 ); // 플루오르 스톤 이벤트용 레벨 2
#endif //CHILDRENS_DAY_EVENT_ITEM
	}
	else if( iItemLevel_ <= 40 )
	{
		vecAdjuvantIdList_.push_back( 206750 );
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 60005540 );	// 이벤트용 플루오르 스톤 레벨 3
		vecAdjuvantIdList_.push_back( 85001939 );	// 이벤트용 플루오르 스톤 레벨 3
		vecAdjuvantIdList_.push_back( 70001912 );	// 플루오르 스톤 이벤트용 레벨 3 JP
		vecAdjuvantIdList_.push_back( 83000142 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 3
		vecAdjuvantIdList_.push_back( 83000192 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 3
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
#ifdef CHILDRENS_DAY_EVENT_ITEM
		vecAdjuvantIdList_.push_back( 160788 ); // 플루오르 스톤 이벤트용 레벨 3
#endif //CHILDRENS_DAY_EVENT_ITEM
	}
	else if( iItemLevel_ <= 50 )
	{
		// 현재는 순서 상관없음
		vecAdjuvantIdList_.push_back( 206760 ); // 플루오르 스톤 레벨 4
		vecAdjuvantIdList_.push_back( 160368 );	// 플루오르 스톤 이벤트용 레벨 4
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 60005550 );	// 이벤트용 플루오르 스톤 레벨 4
		vecAdjuvantIdList_.push_back( 85001940 );	// 이벤트용 플루오르 스톤 레벨 4
		vecAdjuvantIdList_.push_back( 70001913 );	// 플루오르 스톤 이벤트용 레벨 4	JP
		vecAdjuvantIdList_.push_back( 83000143 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 4
		vecAdjuvantIdList_.push_back( 83000193 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 4
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
	}
	else if( iItemLevel_ <= 60 )
	{
		// 현재는 순서 상관없음
		vecAdjuvantIdList_.push_back( 206770 );	// 플루오르 스톤 레벨 5
		vecAdjuvantIdList_.push_back( 160369 ); // 플루오르 스톤 이벤트용 레벨 4
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 60005560 );	// 이벤트용 플루오르 스톤 레벨 5
		vecAdjuvantIdList_.push_back( 85001941 );	// 이벤트용 플루오르 스톤 레벨 5
		vecAdjuvantIdList_.push_back( 70001914 ); 	// 플루오르 스톤 이벤트용 레벨 5 JP
		vecAdjuvantIdList_.push_back( 83000144 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 5
		vecAdjuvantIdList_.push_back( 83000194 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 5
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
	}
	else if( iItemLevel_ <= 70 )
	{
		vecAdjuvantIdList_.push_back( 206780 );
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 67006182 );	// 이벤트용 플루오르 스톤 레벨 6
		vecAdjuvantIdList_.push_back( 70001915 );	// 플루오르 스톤 이벤트용 레벨 6 JP
		vecAdjuvantIdList_.push_back( 83000145 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 6
		vecAdjuvantIdList_.push_back( 83000195 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 6
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
#ifdef CHILDRENS_DAY_EVENT_ITEM
		vecAdjuvantIdList_.push_back( 160789 ); // 플루오르 스톤 이벤트용 레벨 6
#endif //CHILDRENS_DAY_EVENT_ITEM
	}
	else if( iItemLevel_ <= 80 )
	{
		vecAdjuvantIdList_.push_back( 206790 );
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 70001916 );	// 플루오르 스톤 이벤트용 레벨 7
		vecAdjuvantIdList_.push_back( 83000146 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 7
		vecAdjuvantIdList_.push_back( 83000196 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 7
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
	}
	else if( iItemLevel_ <= 90 )
	{
		vecAdjuvantIdList_.push_back( 206800 );
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 70001917 );	// 플루오르 스톤 이벤트용 레벨 8
		vecAdjuvantIdList_.push_back( 83000147 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 8
		vecAdjuvantIdList_.push_back( 83000197 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 8
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
	}
	else if( iItemLevel_ <= 100 )
	{
		vecAdjuvantIdList_.push_back( 206810 );
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		vecAdjuvantIdList_.push_back( 70001918 );	// 플루오르 스톤 이벤트용 레벨 9
		vecAdjuvantIdList_.push_back( 83000148 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 9
		vecAdjuvantIdList_.push_back( 83000198 ); // 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 플루오르 스톤 레벨 9
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
	}
	else
	{
		ASSERT( !"invalid item level" );
	}
}

int CX2UIShop::GetNumOfRequiredEnchantAdjuvantItem( IN const vector<int>& vecAdjuvantIdList_ )
{
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	
	int iSum = 0;
	auto GetItemCountFunc = [&iSum, &kInventory]( const int iItemId_ ) {
		iSum += kInventory.GetNumItemByTID( iItemId_ );
	};

	std::for_each( vecAdjuvantIdList_.begin(), vecAdjuvantIdList_.end(), GetItemCountFunc );

	return iSum;
}

	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가 - 우와, 클라는 아이템 아뒤를 다 박아놨다 ㄷㄷ
#ifdef SERV_ENCHANT_PLUS_ITEM
void CX2UIShop::GetRequiredEnchantPlusItemID( const int iItemLevel, OUT vector<int>& vecPlusIdList_ )
{
	if( iItemLevel <= 20 )
	{
		vecPlusIdList_.push_back(75000300);
	}
	else if( iItemLevel <= 30 )
	{
		vecPlusIdList_.push_back(75000301);
	}
	else if( iItemLevel <= 40 )
	{
		vecPlusIdList_.push_back(75000302);
	}
	else if( iItemLevel <= 50 )
	{
		vecPlusIdList_.push_back(75000303);
	}
	else if( iItemLevel <= 60 )
	{
		vecPlusIdList_.push_back(75000304);
	}
	else if( iItemLevel <= 70 )
	{
		vecPlusIdList_.push_back(75000305);
	}
	else if( iItemLevel <= 80 )
	{
		vecPlusIdList_.push_back(75000306);
	}
	else if( iItemLevel <= 90 )
	{
		vecPlusIdList_.push_back(75000307);
	}
	else if( iItemLevel <= 100 )
	{
		vecPlusIdList_.push_back(75000308);
	}
	else
	{
		ASSERT( !"invalid item level" );
	}
}

int CX2UIShop::GetNumOfRequiredEnchantPlusItem( IN const vector<int>& vecPlusIdList_ )
{
	const CX2Inventory& kMyInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	int iSum = 0;
	auto GetItemCountFunc = [&iSum, &kMyInventory]( const int iItemId_ ) {
		iSum += kMyInventory.GetNumItemByTID( iItemId_ );
	};

	std::for_each( vecPlusIdList_.begin(), vecPlusIdList_.end(), GetItemCountFunc );

	return iSum;
}
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}

	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가 우와, 클라는 아이템 아뒤를 다 박아놨다 ㄷㄷ
#ifdef SERV_DESTROY_GUARD_ITEM
void CX2UIShop::GetRequiredDestroyGuardItemID( const int iItemLevel, OUT vector<int>& vecGuardIdList_)
{
	if( iItemLevel <= 20 )
	{
		vecGuardIdList_.push_back(75000310);
	}
	else if( iItemLevel <= 30 )
	{
		vecGuardIdList_.push_back(75000311);
	}
	else if( iItemLevel <= 40 )
	{
		vecGuardIdList_.push_back(75000312);
	}
	else if( iItemLevel <= 50 )
	{
		vecGuardIdList_.push_back(75000313);
	}
	else if( iItemLevel <= 60 )
	{
		vecGuardIdList_.push_back(75000314);
	}
	else if( iItemLevel <= 70 )
	{
		vecGuardIdList_.push_back(75000315);
	}
	else if( iItemLevel <= 80 )
	{
		vecGuardIdList_.push_back(75000316);
	}
	else if( iItemLevel <= 90 )
	{
		vecGuardIdList_.push_back(75000317);
	}
	else if( iItemLevel <= 100 )
	{
		vecGuardIdList_.push_back(75000318);
	}
	else
	{
		ASSERT( !"invalid item level" );
	}
}

int CX2UIShop::GetNumOfRequiredDestroyGuardItem( IN const vector<int>& vecGuardIdList_ )
{
	const CX2Inventory& kMyInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	int iSum = 0;
	auto GetItemCountFunc = [&iSum, &kMyInventory]( const int iItemId_ ) {
		iSum += kMyInventory.GetNumItemByTID( iItemId_ );
	};

	std::for_each( vecGuardIdList_.begin(), vecGuardIdList_.end(), GetItemCountFunc );

	return iSum;
}
#endif // SERV_DESTROY_GUARD_ITEM
//}}
void CX2UIShop::UpdateEnchantWindow()
{
	if( m_pDLGEnchantItem == NULL )
		return;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnchantItemUID );
	if( NULL == pItem )
		return;

	int normalStoneCount	= 0;
	int newStoneCount		= 0;
	int iNewStoneLevel		= GetEnchantStoneLevel( pItem->GetItemTemplet()->GetUseLevel() );

	vector<int> vecAdjuvantItemIdList;
	GetRequiredEnchantAdjuvantItemID( pItem->GetItemTemplet()->GetUseLevel(), vecAdjuvantItemIdList );
	const int iAdjuvantCount  = ( true == vecAdjuvantItemIdList.empty() ? 0 : GetNumOfRequiredEnchantAdjuvantItem( vecAdjuvantItemIdList ) );
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	vector<int> vecPlusIdList;
	GetRequiredEnchantPlusItemID( pItem->GetItemTemplet()->GetUseLevel(), vecPlusIdList );
	const int iEnchantPlusItemCount  = ( true == vecPlusIdList.empty() ? 0 : GetNumOfRequiredEnchantPlusItem( vecPlusIdList ) );
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}
	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	vector<int> vecGuardIdList;
	GetRequiredDestroyGuardItemID( pItem->GetItemTemplet()->GetUseLevel(), vecGuardIdList );
	const int iDestroyGuardItemCount  = ( true == vecPlusIdList.empty() ? 0 : GetNumOfRequiredDestroyGuardItem( vecGuardIdList ) );
#endif // SERV_DESTROY_GUARD_ITEM
	//}}
		
	// 강화보조제 적용
	CKTDGUICheckBox* pCheck = (CKTDGUICheckBox*)m_pDLGEnchantItem->GetControl( L"g_pCheckBoxsecret" );
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	CKTDGUICheckBox* pCheckPlus = (CKTDGUICheckBox*)m_pDLGEnchantItem->GetControl( L"g_pCheckBoxsecret_EnchantPlus" );
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}
	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	CKTDGUICheckBox* pCheckDestroyGuard = (CKTDGUICheckBox*)m_pDLGEnchantItem->GetControl( L"g_pCheckBoxsecret_DestroyGuard" );
#endif // SERV_DESTROY_GUARD_ITEM
	//}}

#ifdef SERV_ENCHANT_EVENT_USING_DB
	IF_EVENT_ENABLED( CEI_11_ENCHANT_EVENT )
	{
		if( pItem->GetItemData().m_EnchantLevel >= MAGIC_ENCHANT_LEVEL_LIMIT + 1 )
		{
			pCheck->SetShowEnable(false, false);
			CKTDGUIStatic* pStaticCheck = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_CheckBox" );
			pStaticCheck->SetShowEnable(false, false);
			m_bUseEnchantAdjuvant = false; 
		}
	}
	else
#endif SERV_ENCHANT_EVENT_USING_DB
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT	
	if( pItem->GetItemData().m_EnchantLevel >= ( MAGIC_ENCHANT_LEVEL_LIMIT + m_iAddEnchantLevel ) )
#else
	if( pItem->GetItemData().m_EnchantLevel >= MAGIC_ENCHANT_LEVEL_LIMIT )
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	{ 
		pCheck->SetShowEnable(false, false);
		CKTDGUIStatic* pStaticCheck = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_CheckBox" );
		pStaticCheck->SetShowEnable(false, false);
		m_bUseEnchantAdjuvant = false; 
		//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
		pCheckPlus->SetShowEnable(false, false);
		CKTDGUIStatic* pStaticCheckPlus = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_CheckBox2" );
		pStaticCheckPlus->SetShowEnable(false, false);
		m_bUseEnchantPlus = false;
#endif // SERV_ENCHANT_PLUS_ITEM
		//}}
		//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
		pCheckDestroyGuard->SetShowEnable(false, false);
		CKTDGUIStatic* pStaticCheckDestroyGuard = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_CheckBox3" );
		pStaticCheckDestroyGuard->SetShowEnable(false, false);
		m_bUseDestroyGuard = false;
#endif // SERV_DESTROY_GUARD_ITEM
		//}}
	} 

	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	if( iEnchantPlusItemCount <= 0 )
	{
		m_bUseEnchantPlus = false;
	}
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}
	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	if( iDestroyGuardItemCount <= 0 )
	{
		m_bUseDestroyGuard = false;
	}
#endif // SERV_DESTROY_GUARD_ITEM
	//}}
	//강화보조제 있는지 확인해서 없으면 강제 false 있으면 그냥 현행유지 (초기값은 false)
	if( iAdjuvantCount <= 0 )
	{
		m_bUseEnchantAdjuvant = false;
	}
	// 마지막으로 고급강화를 했나 안 했나에 따르게 하자..
	pCheck->SetCheckedPure( m_bUseEnchantAdjuvant );
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	pCheckPlus->SetCheckedPure( m_bUseEnchantPlus );
#endif // SERV_ENCHANT_PLUS_ITEM
	///}}
	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	pCheckDestroyGuard->SetCheckedPure( m_bUseDestroyGuard );
#endif // SERV_DESTROY_GUARD_ITEM
	//}}

	wstring normalButtonDesc	= L"";
	wstring newButtonDesc		= L"";

	switch( pItem->GetItemTemplet()->GetItemType() )
	{
	case CX2Item::IT_WEAPON:
		{
			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
#ifdef SERV_BLESSED_RURIEL_ENCHANT_STONE_EVENT
			normalStoneCount += g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( 152000121 );
#endif // SERV_BLESSED_RURIEL_ENCHANT_STONE_EVENT
			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );

			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
			if ( pItemTemplet != NULL )
			{
				normalButtonDesc = pItemTemplet->GetFullName_();
			}

			pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
			if ( pItemTemplet != NULL )
			{
				newButtonDesc = pItemTemplet->GetFullName_();
			}

		} break;

	case CX2Item::IT_DEFENCE:
		{
			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
#ifdef SERV_BLESSED_RURIEL_ENCHANT_STONE_EVENT
			normalStoneCount += g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( 152000122 );
#endif // SERV_BLESSED_RURIEL_ENCHANT_STONE_EVENT
			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
			if ( pItemTemplet != NULL )
			{
				normalButtonDesc = pItemTemplet->GetFullName_();
			}

			pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
			if ( pItemTemplet != NULL )
			{
				newButtonDesc = pItemTemplet->GetFullName_();
			}
		} break;

	default:
		{
			return;
		} break;
	}

    wstring wstrNormalButtonItem = normalButtonDesc;
    wstring wstrNewButtonItem = newButtonDesc;

	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
	//	파괴 방지랑 중복 부분. 따로 쓰고 싶으면 작업 해야 함.
#ifdef SERV_ENCHANT_PLUS_ITEM
	if( true == m_bUseEnchantAdjuvant || true == m_bUseEnchantPlus || true == m_bUseDestroyGuard) 
	{
		if( true == m_bUseEnchantAdjuvant && false == m_bUseEnchantPlus && false == m_bUseDestroyGuard )// 플루오르
		{
			normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_926, "L", wstrNormalButtonItem ) );
			newButtonDesc = GET_REPLACED_STRING( ( STR_ID_926, "L", wstrNewButtonItem ) );
		}
		else if( true == m_bUseEnchantAdjuvant && true == m_bUseEnchantPlus && false == m_bUseDestroyGuard )// 플루오르, 셀레네
		{
			normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_12310, "L", wstrNormalButtonItem ) );
			newButtonDesc = GET_REPLACED_STRING( ( STR_ID_12310, "L", wstrNewButtonItem ) );
		}
		else if( true == m_bUseEnchantAdjuvant && false == m_bUseEnchantPlus && true == m_bUseDestroyGuard )// 플루오르, 불카누스
		{
			normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_12311, "L", wstrNormalButtonItem ) );
			newButtonDesc = GET_REPLACED_STRING( ( STR_ID_12311, "L", wstrNewButtonItem ) );
		}
		else if( true == m_bUseEnchantAdjuvant && true == m_bUseEnchantPlus && true == m_bUseDestroyGuard )// 플루오르, 셀레네, 불카누스
		{
			normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_12313, "L", wstrNormalButtonItem ) );
			newButtonDesc = GET_REPLACED_STRING( ( STR_ID_12313, "L", wstrNewButtonItem ) );
		}
		else if( false == m_bUseEnchantAdjuvant && true == m_bUseEnchantPlus && false == m_bUseDestroyGuard )// 셀레네
		{
			normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_12308  , "L", wstrNormalButtonItem ) );
			newButtonDesc = GET_REPLACED_STRING( ( STR_ID_12308  , "L", wstrNewButtonItem ) );
		}
		else if( false == m_bUseEnchantAdjuvant && true == m_bUseEnchantPlus && true == m_bUseDestroyGuard )// 셀레네, 불카누스
		{
			normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_12312, "L", wstrNormalButtonItem ) );
			newButtonDesc = GET_REPLACED_STRING( ( STR_ID_12312, "L", wstrNewButtonItem ) );
		}
		else if( false == m_bUseEnchantAdjuvant && false == m_bUseEnchantPlus && true == m_bUseDestroyGuard )// 불카누스
		{
			normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_12309, "L", wstrNormalButtonItem ) );
			newButtonDesc = GET_REPLACED_STRING( ( STR_ID_12309, "L", wstrNewButtonItem ) );
		}
	}
#else // SERV_ENCHANT_PLUS_ITEM
	if( true == m_bUseEnchantAdjuvant )
	{
		normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_926, "L", wstrNormalButtonItem ) );
		newButtonDesc = GET_REPLACED_STRING( ( STR_ID_926, "L", wstrNewButtonItem ) );
	}
#endif // SERV_ENCHANT_PLUS_ITEM
		//}}
	else
	{
		normalButtonDesc = GET_REPLACED_STRING( ( STR_ID_927, "L", wstrNormalButtonItem ) );
		newButtonDesc = GET_REPLACED_STRING( ( STR_ID_927, "L", wstrNewButtonItem ) );
	}

	wstring normalButtonName = L"";
	wstring normalStaticInactiveName = L"";
	WCHAR newButtonName[128] = L"";
	WCHAR newStaticName[128] = L"";
	CKTDGUIControl* pButton = NULL;
	CKTDGUIStatic* pStatic = NULL;

	// 레벨 없는 기존 강화석 버튼
	pButton = m_pDLGEnchantItem->GetControl( L"Button_Old" );
	pStatic = (CKTDGUIStatic*) m_pDLGEnchantItem->GetControl( L"Static_Inactive_Old" );
	if( NULL != pButton )
	{
		pButton->SetShowEnable( false, false );
		pButton->SetGuideDesc( normalButtonDesc.c_str() );
	}
	if( NULL != pStatic )
		pStatic->SetShow(false);

	// 기존 강화석 고급강화 버튼
	pButton = m_pDLGEnchantItem->GetControl( L"Button_Old_High" );
	pStatic = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Inactive_Old_High" );
	if( NULL != pButton )
	{
		pButton->SetShowEnable( false, false );
		pButton->SetGuideDesc( normalButtonDesc.c_str() );
	}
	if( NULL != pStatic )
		pStatic->SetShow(false);

	// 가능회수
	CKTDGUIStatic* pStaticItemNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Possible_Old" );
	if( pStaticItemNum != NULL && pStaticItemNum->GetString( 0 ) != NULL )
	{
		//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
		//	파괴 방지랑 중복 부분. 따로 쓰고 싶으면 작업 해야 함.
#ifdef SERV_ENCHANT_PLUS_ITEM
		wstringstream wstrstm;
		int	iEnchantCount = normalStoneCount;
		if( true == m_bUseEnchantAdjuvant ) 
				iEnchantCount =  min( iEnchantCount, iAdjuvantCount );
		if( true == m_bUseDestroyGuard )
			iEnchantCount =  min( iEnchantCount, iDestroyGuardItemCount );
		if( true == m_bUseEnchantPlus )
			iEnchantCount =  min( iEnchantCount, iEnchantPlusItemCount );
		wstrstm << iEnchantCount;
#else // SERV_ENCHANT_PLUS_ITEM
		wstringstream wstrstm;
		if( true == m_bUseEnchantAdjuvant ) 
		{
			wstrstm << min( normalStoneCount, iAdjuvantCount );
		}
		else
		{
			wstrstm << normalStoneCount;
		}
#endif // SERV_ENCHANT_PLUS_ITEM
		//}}
		pStaticItemNum->GetString(0)->msg = wstrstm.str().c_str();
	}

	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
	//	파괴 방지랑 중복 부분. 따로 쓰고 싶으면 작업 해야 함.
#ifdef SERV_ENCHANT_PLUS_ITEM
	if( true == m_bUseEnchantAdjuvant || true == m_bUseEnchantPlus || true == m_bUseDestroyGuard) 
#else // SERV_ENCHANT_PLUS_ITEM
	if( true == m_bUseEnchantAdjuvant )
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}
	{
		normalButtonName = L"Button_Old_High";
		normalStaticInactiveName = L"Static_Inactive_Old_High";
	}
	else
	{
		normalButtonName = L"Button_Old";
		normalStaticInactiveName = L"Static_Inactive_Old";
	}


	pButton = m_pDLGEnchantItem->GetControl( normalButtonName.c_str() );
	pStatic = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( normalStaticInactiveName.c_str() );
	if( NULL != pButton )
	{
		if( normalStoneCount >= 1 )
		{
			pButton->SetShowEnable( true, true );
		}
		else
		{
			if( NULL != pStatic )
				pStatic->SetShow(true);			
			pButton->SetShowEnable( false, false );
		}
		pButton->SetGuideDesc( normalButtonDesc.c_str() );
	}
	

	// 레벨붙은 새 강화석 버튼
	const int MAGIC_NEW_ENCHANT_BUTTON_COUNT = 6;
	for( int i=0; i<MAGIC_NEW_ENCHANT_BUTTON_COUNT; i++ )
	{
		StringCchPrintfW( newButtonName, ARRAY_SIZE(newButtonName), L"Button_Weapon_High%d", i+1 );		
		pButton = m_pDLGEnchantItem->GetControl( newButtonName );
		if( NULL != pButton )
		{
			pButton->SetShowEnable( false, false );
		}

		StringCchPrintfW( newButtonName, ARRAY_SIZE(newButtonName), L"Button_Weapon_Normal%d", i+1 );		
		pButton = m_pDLGEnchantItem->GetControl( newButtonName );
		if( NULL != pButton )
		{
			pButton->SetShowEnable( false, false );
		}


		StringCchPrintfW( newButtonName, ARRAY_SIZE(newButtonName), L"Button_Def_High%d", i+1 );		
		pButton = m_pDLGEnchantItem->GetControl( newButtonName );
		if( NULL != pButton )
		{
			pButton->SetShowEnable( false, false );
		}

		StringCchPrintfW( newButtonName, ARRAY_SIZE(newButtonName), L"Button_Def_Normal%d", i+1 );		
		pButton = m_pDLGEnchantItem->GetControl( newButtonName );
		if( NULL != pButton )
		{
			pButton->SetShowEnable( false, false );
		}
		
		//비활성 스태튁
		StringCchPrintfW( newStaticName, ARRAY_SIZE(newStaticName), L"Static_Inactive%d", i+1 );		
		pStatic = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( newStaticName );
		if( NULL != pButton )
		{
			pStatic->SetShowEnable( false, false );
		}

		//비활성 스태튁
		StringCchPrintfW( newStaticName, ARRAY_SIZE(newStaticName), L"Static_Inactive_High%d", i+1 );		
		pStatic = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( newStaticName );
		if( NULL != pButton )
		{
			pStatic->SetShowEnable( false, false );
		}

	}


	pStaticItemNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Possible_New" );
	if ( pStaticItemNum != NULL && pStaticItemNum->GetString( 0 ) != NULL )
	{
		//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
		//	파괴 방지랑 중복 부분. 따로 쓰고 싶으면 작업 해야 함.
#ifdef SERV_ENCHANT_PLUS_ITEM
		wstringstream wstrstm;
		int	iEnchantCount = newStoneCount;
		if( true == m_bUseEnchantAdjuvant ) 
			iEnchantCount =  min( iEnchantCount, iAdjuvantCount );
		if( true == m_bUseDestroyGuard )
			iEnchantCount =  min( iEnchantCount, iDestroyGuardItemCount );
		if( true == m_bUseEnchantPlus )
			iEnchantCount =  min( iEnchantCount, iEnchantPlusItemCount );
		wstrstm << iEnchantCount;
#else // SERV_ENCHANT_PLUS_ITEM
		wstringstream wstrstm;
		if( true == m_bUseEnchantAdjuvant ) 
		{
			wstrstm << min( newStoneCount, iAdjuvantCount );
		}
		else
		{
			wstrstm << newStoneCount;
		}
#endif // SERV_ENCHANT_PLUS_ITEM
		//}}
		pStaticItemNum->GetString(0)->msg = wstrstm.str().c_str();
	}

	switch( pItem->GetItemTemplet()->GetItemType() )
	{
	case CX2Item::IT_WEAPON:
		{
			//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
			//	파괴 방지랑 중복 부분. 따로 쓰고 싶으면 작업 해야 함.
#ifdef SERV_ENCHANT_PLUS_ITEM
			if( true == m_bUseEnchantAdjuvant || true == m_bUseEnchantPlus || true == m_bUseDestroyGuard) 
#else // SERV_ENCHANT_PLUS_ITEM
			if( true == m_bUseEnchantAdjuvant )
#endif // SERV_ENCHANT_PLUS_ITEM
				//}}
			{
				StringCchPrintfW( newButtonName, ARRAY_SIZE(newButtonName), L"Button_Weapon_High%d", iNewStoneLevel+1 );
				StringCchPrintfW( newStaticName, ARRAY_SIZE(newStaticName), L"Static_Inactive_High%d", iNewStoneLevel+1 );		
			}
			else
			{
				StringCchPrintfW( newButtonName, ARRAY_SIZE(newButtonName), L"Button_Weapon_Normal%d", iNewStoneLevel+1 );
				StringCchPrintfW( newStaticName, ARRAY_SIZE(newStaticName), L"Static_Inactive%d", iNewStoneLevel+1 );		
			}
		} break;

	case CX2Item::IT_DEFENCE:
		{
			//	파괴 방지랑 중복 부분. 따로 쓰고 싶으면 작업 해야 함.
#ifdef SERV_ENCHANT_PLUS_ITEM
			if( true == m_bUseEnchantAdjuvant || true == m_bUseEnchantPlus || true == m_bUseDestroyGuard) 
#else // SERV_ENCHANT_PLUS_ITEM
			if( true == m_bUseEnchantAdjuvant ) 
#endif // SERV_ENCHANT_PLUS_ITEM
			{
				StringCchPrintfW( newButtonName, ARRAY_SIZE(newButtonName), L"Button_Def_High%d", iNewStoneLevel+1 );
				StringCchPrintfW( newStaticName, ARRAY_SIZE(newStaticName), L"Static_Inactive_High%d", iNewStoneLevel+1 );		
			}
			else
			{
				StringCchPrintfW( newButtonName, ARRAY_SIZE(newButtonName), L"Button_Def_Normal%d", iNewStoneLevel+1 );
				StringCchPrintfW( newStaticName, ARRAY_SIZE(newStaticName), L"Static_Inactive%d", iNewStoneLevel+1 );		
			}
		} break;
	}



	pButton = m_pDLGEnchantItem->GetControl( newButtonName );
	pStatic = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( newStaticName );
	if( NULL != pButton )
	{
		if( newStoneCount >= 1 )
		{
			pButton->SetShowEnable( true, true );
		}
		else
		{
			pButton->SetShowEnable( false, false );
			pStatic->SetShowEnable(true, true);
		}
		pButton->SetGuideDesc( newButtonDesc.c_str() );
	}

}


int CX2UIShop::GetEnchantStoneLevel( int iItemLevel )
{

	if( iItemLevel <= 20 )
	{
		return 0;
	}
	else if( iItemLevel <= 30 )
	{
		return 1;
	}
	else if( iItemLevel <= 40 )
	{
		return 2;
	}
	else if( iItemLevel <= 50 )
	{
		return 3;
	}
	else if( iItemLevel <= 60 )
	{
		return 4;
	}
	else if( iItemLevel <= 70 )
	{
		return 5;
	}
	else if( iItemLevel <= 80 )
	{
		return 6;
	}
	else if( iItemLevel <= 90 )
	{
		return 7;
	}
	else
	{
		return 8;
	}
}


bool CX2UIShop::Handler_EGS_ENCHANT_ITEM_REQ( bool bSpecialEnchant )
{
	bool bDevCheat = false;
	if ( m_pDLGEnchantItem != NULL )
	{
		// 개발자용 강화 치트
		if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGEnchantItem->GetControl( L"CheckBox_DevCheat" );
			if ( pCheckBox != NULL )
			{
				if ( pCheckBox->GetChecked() == true )
				{
					bDevCheat = true;
				}
			}
		}

		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGEnchantItem, NULL, false );
	}
	m_pDLGEnchantItem = NULL;


	KEGS_ENCHANT_ITEM_REQ kPacket;

	kPacket.m_iItemUID		= m_EnchantItemUID;
	kPacket.m_bIsRareEnchantStone = false;		// obsolete!! 사용안하는 변수
	kPacket.m_bIsNewEnchant = bSpecialEnchant;
	kPacket.m_bDebug		= false;
	//강화보조제
	kPacket.m_bIsSupportMaterial	= m_bUseEnchantAdjuvant;		// 강화 보조제

	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	kPacket.m_bIsEnchantPlus =  m_bUseEnchantPlus;
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}
	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	kPacket.m_bIsDestroyGuard =  m_bUseDestroyGuard;
#endif // SERV_DESTROY_GUARD_ITEM
	//}}

	if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		kPacket.m_bDebug = bDevCheat;

	g_pData->GetServerProtocol()->SendPacket( EGS_ENCHANT_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ENCHANT_ITEM_ACK );

	return true;
}

bool CX2UIShop::Handler_EGS_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ENCHANT_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_ENCHANT_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			OpenEnchantResultWindow( kEvent );

			return true;
		}		
	}

	return false;

}

void CX2UIShop::OpenEnchantResultWindow( KEGS_ENCHANT_ITEM_ACK& kEGS_ENCHANT_ITEM_ACK )
{
	//업데이트 해주기전에..
	//아이템 체크해서.. 업그레이드 됐는지, 초기화 됐는지, 변화없는지, 다운됐는지, 사라졌는지..

	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEGS_ENCHANT_ITEM_ACK.m_iED;

	if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
	{
		g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
	}
	
	if(g_pData->GetUIManager()->GetUIInventory() != NULL)
	{		
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEGS_ENCHANT_ITEM_ACK.m_vecInventorySlotInfo );
	}

	SAFE_DELETE_DIALOG( m_pDLGEnchantItemResult );

	// 패킷 처리 : 결과를 보여주자
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );

	//강화 성공. 
	if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_00 )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Success.ogg", false, false );
		m_pDLGEnchantItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Item_Upgrade_Result_Success.lua" );

		if ( pItem != NULL )
		{
			CKTDGUIStatic* pStaticComment = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_comment" );
			if ( pStaticComment != NULL && pStaticComment->GetString(0) != NULL )
			{
				wstringstream wstrstm;
				wstrstm << GET_REPLACED_STRING( ( STR_ID_928, "i", pItem->GetItemData().m_EnchantLevel ) );
				pStaticComment->GetString(0)->msg = wstrstm.str().c_str();
			}
		}
	}
	//변화 없음.
	else if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_01 )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_NoChange.ogg", false, false );

		m_pDLGEnchantItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Item_Upgrade_Result_Unchange.lua" );
	}
	//1단계 하락.
	else if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_02 )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_DownGrade.ogg", false, false );

		m_pDLGEnchantItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Item_Upgrade_Result_Fail_Down.lua" );
	}
	//강화 단계 초기화.
	else if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_03 )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Initial.ogg", false, false );

		m_pDLGEnchantItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Item_Upgrade_Result_Format.lua" );
	}
	//아이템 분해.
	else if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_04 )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Disappear.ogg", false, false );

		m_pDLGEnchantItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Item_Upgrade_Result_broken.lua" );
	}
	else
	{
		// 장치를 정지합니다 안 돼 행보관이 내 말을 듣지 않았어
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"Internal Error", g_pMain->GetNowState() );
		return;
	}


	if ( m_pDLGEnchantItemResult != NULL )
	{
		CKTDGUIControl* pControl = (CKTDGUIControl*)m_pDLGEnchantItemResult->GetControl( L"Enchant_OK_Button" );
		if ( pControl != NULL )
		{
			pControl->RequestFocus();
			pControl->OnFocusIn();
		}

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEnchantItemResult );

		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemID );
		if ( pItemTemplet != NULL )
		{
			//슬롯 이미지 표시 해주기.
			CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"g_pStatic_item_Slot_Image" );
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
			{
                const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
				if ( pwszShopImage[0] != NULL )
					pStaticSlot->GetPicture(0)->SetTex( pwszShopImage );
				else
					pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_Item_Name" );
			if ( pStaticName != NULL && pStaticName->GetString(0) != NULL )
			{
                const wchar_t* pwszFullName = pItemTemplet->GetFullName_();
#ifdef FIXED_DIALOG_UPGRADE_ITEM_SPLIT_STRING	
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
				if ( NULL != pItem )
				{
					wstringstream wstrstm;
					
					if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_04 )
					{
						wstrstm << L"+" << m_EnchantLevelBefore << L" " << pwszFullName;
					}
					
					else
					{
						wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pwszFullName;
					}		
					
					SplitEnchantDLGStringSet ( pStaticName, wstrstm.str().c_str( ) );
				}
#else	// FIXED_DIALOG_UPGRADE_ITEM_SPLIT_STRING
				if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_04 )
				{
					// 이전 강화 레벨을 적어주자 ^^;
					wstringstream wstrstm;
					wstrstm << L"+" << m_EnchantLevelBefore << L" " << pwszFullName;
#ifdef CLIENT_GLOBAL_LINEBREAK
					int iLimitWidth = 190;
					std::wstring wstr_tmp = CWordLineHandler::GetStrByLineBreakInX2Main( wstrstm.str().c_str(), iLimitWidth, pStaticName->GetString(0)->fontIndex );
					pStaticName->GetString(0)->msg = wstr_tmp.c_str();
#else //CLIENT_GLOBAL_LINEBREAK
					pStaticName->GetString(0)->msg = wstrstm.str().c_str();
#endif //CLIENT_GLOBAL_LINEBREAK
				}					
			
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
				if ( pItem != NULL )
				{
					wstringstream wstrstm;
					wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pwszFullName;
#ifdef CLIENT_GLOBAL_LINEBREAK
					int iLimitWidth = 190;
					std::wstring wstr_tmp = CWordLineHandler::GetStrByLineBreakInX2Main( wstrstm.str().c_str(), iLimitWidth, pStaticName->GetString(0)->fontIndex );
					pStaticName->GetString(0)->msg = wstr_tmp.c_str();
#else //CLIENT_GLOBAL_LINEBREAK
					pStaticName->GetString(0)->msg = wstrstm.str().c_str();
#endif //CLIENT_GLOBAL_LINEBREAK
				}
#endif // FIXED_DIALOG_UPGRADE_ITEM_SPLIT_STRING
					
				else
				{
#ifdef CLIENT_GLOBAL_LINEBREAK
					int iLimitWidth = 190;
					std::wstring wstr_tmp = CWordLineHandler::GetStrByLineBreakInX2Main( pwszFullName, iLimitWidth, pStaticName->GetString(0)->fontIndex );
					pStaticName->GetString(0)->msg = wstr_tmp.c_str();
#else //CLIENT_GLOBAL_LINEBREAK
					pStaticName->GetString(0)->msg = pwszFullName;
#endif //CLIENT_GLOBAL_LINEBREAK	
				}
			}
		}
	}

	// 결과 숫자를 표시하자
	switch(kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult)
	{
	case NetError::ERR_ENCHANT_RESULT_00:	// 성공
	case NetError::ERR_ENCHANT_RESULT_01:	// 변화없음
	case NetError::ERR_ENCHANT_RESULT_02:	// -1
		{
			// 강화 대상 아이템 결과 강화 레벨 표시
			// 일단 다 꺼주고
			CKTDGUIStatic* pStaticNum;
			pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_UpgradeLevel_SingleDigit" );
			if ( pStaticNum != NULL )
			{
				int PicNum = pStaticNum->GetDummyInt(0);
				for ( int i = 0; i <= PicNum; i++ )
				{
					if ( pStaticNum->GetPicture( i ) != NULL )
					{
						pStaticNum->GetPicture( i )->SetShow( false );
					}
				}
			}
			pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_UpgradeLevel_DoubleDigit_1" );
			if ( pStaticNum != NULL )
			{
				int PicNum = pStaticNum->GetDummyInt(0);
				for ( int i = 0; i <= PicNum; i++ )
				{
					if ( pStaticNum->GetPicture( i ) != NULL )
					{
						pStaticNum->GetPicture( i )->SetShow( false );
					}
				}
			}
			pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_UpgradeLevel_DoubleDigit_10" );
			if ( pStaticNum != NULL )
			{
				int PicNum = pStaticNum->GetDummyInt(0);
				for ( int i = 0; i <= PicNum; i++ )
				{
					if ( pStaticNum->GetPicture( i ) != NULL )
					{
						pStaticNum->GetPicture( i )->SetShow( false );
					}
				}
			}
			if( pItem->GetItemData().m_EnchantLevel < 10)	// 현재 10렙 아래일 때 : 1자리 스태틱으로 해결 보자
			{
				pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_UpgradeLevel_SingleDigit" );
				if( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
					pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );		
			}
			else
			{
				CKTDGUIStatic* pStaticNum1 = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_UpgradeLevel_DoubleDigit_1" );
				CKTDGUIStatic* pStaticNum10 = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_UpgradeLevel_DoubleDigit_10" );
				if( pStaticNum10->GetPicture( (pItem->GetItemData().m_EnchantLevel)/10 ) != NULL )
					pStaticNum10->GetPicture( (pItem->GetItemData().m_EnchantLevel)/10 )->SetShow( true );
				if( pStaticNum1->GetPicture( (pItem->GetItemData().m_EnchantLevel)%10 ) != NULL )
					pStaticNum1->GetPicture( (pItem->GetItemData().m_EnchantLevel)%10 )->SetShow( true );
			}

		}
	case NetError::ERR_ENCHANT_RESULT_03:	// 리셋
	case NetError::ERR_ENCHANT_RESULT_04:	// 파괴
		// 이 두 경우는 그냥 나와있는대로 쓰면 될 듯
	default:
		break;
	}

	// 위치 잡아주자
// 	m_NowMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
// 	m_NowMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
// 	D3DXVECTOR2 pos = g_pKTDXApp->MouseConvertByResolution( m_NowMousePos ) - D3DXVECTOR2(185, 255);
// 	if (pos.x > 668)
// 		pos.x = 668;
// 	if (pos.y > 425)
// 		pos.y = 425;
// 	m_pDLGEnchantItemResult->SetPos( pos );
	m_pDLGEnchantItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-668, 768-425), D3DXVECTOR2(185,255)) );

}

#ifdef FIXED_DIALOG_UPGRADE_ITEM_SPLIT_STRING
void CX2UIShop::SplitEnchantDLGStringSet ( CKTDGUIStatic* pStatic, wstring strItemName  )
{
	if ( pStatic == NULL )
		return ;
	
	
	if ( NULL != g_pKTDXApp && 
		NULL != g_pKTDXApp->GetDGManager() && 
		NULL != g_pKTDXApp->GetDGManager()->GetDialogManager() )
	{
		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );

		if ( NULL != pItemDescFont )
		{
			int strWidth = static_cast<int> ( pItemDescFont->GetWidth( strItemName.c_str() ) / g_pKTDXApp->GetResolutionScaleX() );
			wstring wstr = L"";					

			if ( strWidth > 140 )		// 문자열 길이가 해상도 대비 140 이상 일 때 자른다.
			{
#ifdef CLIENT_GLOBAL_LINEBREAK
				wstr = CWordLineHandler::GetStrByLineBreakColorInX2Main( strItemName.c_str(), 140, SLOT_MANAGER_FONT_INDEX );
#else //CLIENT_GLOBAL_LINEBREAK
				wstr = g_pMain->GetStrByLineBreakColor( strItemName.c_str(), 140, SLOT_MANAGER_FONT_INDEX );
#endif //CLIENT_GLOBAL_LINEBREAK
				pStatic->SetOffsetPos(D3DXVECTOR2(0, -13));
			}
			else
			{
				wstr = strItemName.c_str();
			}
			pStatic->SetString( 0, wstr.c_str() );
		}
	}

	
}
#endif 

//////////////////////////////////////////////////////////////////////////
void CX2UIShop::EnchantAttributeItem( UidType AttributeEnchantItemUID, D3DXVECTOR2 pos )
{
	// 다이얼로그 자동 클릭되는 현상을 막자
	g_pData->GetUIManager()->SetDLGFlag(false);

	m_AttributeEnchantItemUID = AttributeEnchantItemUID;

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pItem = kInventory.GetItem( m_AttributeEnchantItemUID );
    const CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();

	if( NULL == pItem )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_922 ), g_pMain->GetNowState() );
		return; 
	}

	if ( pItemTemplet != NULL )
	{
		if( true == pItemTemplet->GetFashion() )
			return; 

		switch( pItemTemplet->GetItemType() )
		{
		case CX2Item::IT_WEAPON:
		case CX2Item::IT_DEFENCE:
			{
				//{{ kimhc // 2010-01-06 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
				if ( pItemTemplet->GetIsPcBang() == true )
				{
					g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
					return;
				}
#endif	PC_BANG_WORK
				//}} kimhc // 2010-01-06 // PC방 프리미엄 서비스

			} break;
		default:
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_929 ), g_pMain->GetNowState() );
				return;
			} break;
		}

		if( 0 == pItemTemplet->GetMaxAttribEnchantCount() )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4574 ), g_pMain->GetNowState() );
			return;
		}
	}

#ifdef ITEM_RECOVERY_TEST
	// (사용불능인가?)
	if(true == pItem->IsDisabled())
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_800 ), g_pMain->GetNowState() );
		return; 
	}
#endif //ITEM_RECOVERY_TEST
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	else if ( false == pItem->GetIsEvaluation() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_24617 ), g_pMain->GetNowState() );
		return; 
	}
#endif  //SERV_NEW_ITEM_SYSTEM_2013_05 

	int UnknownStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
	int RedStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_RED );
	int BlueStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_BLUE );
	int GreenStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_GREEN );
	int WindStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_WIND );
	int LightStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_LIGHT );
	int DarkStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_DARK );
	
	// 속성석 하나도 없어여
	if ( UnknownStone <= 0 &&
		 RedStone <= 0 &&
		 BlueStone <= 0 &&
		 GreenStone <= 0 &&
		 WindStone <= 0 &&
		 LightStone <= 0 &&
		 DarkStone <= 0 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_930 ), g_pMain->GetNowState() );
		return;
	}


	if ( pItem != NULL 
        && pItem->GetItemTemplet() != NULL 
        )
	{
		SAFE_DELETE_DIALOG( m_pDLGAttribEnchantItem );
		m_pDLGAttribEnchantItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Attribute_Item.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttribEnchantItem );
// 		pos = pos - D3DXVECTOR2(177, 192);
// 		if( pos.x > 668) 
// 			pos.x = 668;
// 		if( pos.y > 190) 
// 			pos.y = 190;
// 		m_pDLGAttribEnchantItem->SetPos( pos );
		m_pDLGAttribEnchantItem->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-668, 768-190), D3DXVECTOR2(177,192)) );
		
		ResetAtrribEnchantWindow();
	}

}





void CX2UIShop::ResetAtrribEnchantWindow()
{
	if ( m_pDLGAttribEnchantItem == NULL )
		return;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttributeEnchantItemUID ); 
	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
		return;

	// 무기/방어구에 상관없이 처리해야 하는 부분

	//이미지 셋팅
	CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_item_Slot_Image" );
	if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
	{
        const wchar_t*  pwszShoImage = pItem->GetItemTemplet()->GetShopImage();
		pStaticItemImage->GetPicture(0)->SetTex( pwszShoImage );
	}

	//{{ kimhc // 2009-09-08 // 봉인된 아이템 이미지 출력
#ifdef	SEAL_ITEM
	if ( pStaticItemImage->GetPicture( 1 ) != NULL )
	{
		if ( pItem->GetItemData().m_bIsSealed == true )
			pStaticItemImage->GetPicture( 1 )->SetShow( true );
		else
			pStaticItemImage->GetPicture( 1 )->SetShow( false );
	}		
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-08 // 봉인된 아이템 이미지 출력

	//이름
	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Name" );
	if ( pStaticItemName != NULL )
	{
		wstringstream wstrstm;
		wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << 
            pItem->GetItemTemplet()->GetFullName_()
            ;
#ifdef FIX_TOOLTIP
		wstring wstrTemp;
		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );
		int strWidth = (int)( pItemDescFont->GetWidth( wstrstm.str().c_str() ) / g_pKTDXApp->GetResolutionScaleX() );
		if ( strWidth > 200 )
		{
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstrTemp = CWordLineHandler::GetStrByLineBreakColorInX2Main( wstrstm.str().c_str(), 200, SLOT_MANAGER_FONT_INDEX );
#else //CLIENT_GLOBAL_LINEBREAK
			wstrTemp = g_pMain->GetStrByLineBreakColor( wstrstm.str().c_str(), 200, SLOT_MANAGER_FONT_INDEX );
#endif //CLIENT_GLOBAL_LINEBREAK	
			pStaticItemName->SetOffsetPos(D3DXVECTOR2(0, -13));
		}
		else
		{
			wstrTemp = wstrstm.str().c_str();
		}

		pStaticItemName->SetString( 0, wstrTemp.c_str() );
#else
		pStaticItemName->SetString( 0, wstrstm.str().c_str() );
#endif FIX_TOOLTIP
	}


	CKTDGUIStatic* pStaticUpgradeNotice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Upgrade_Notice" );
	if ( pStaticUpgradeNotice != NULL )
	{
		pStaticUpgradeNotice->SetString(0, L"");
		pStaticUpgradeNotice->SetString(1, L"");
	}



	const int iMaxEnchantCount = pItem->GetItemTemplet()->GetMaxAttribEnchantCount();
	const int iCurrEnchantedCount = pItem->GetItemData().GetAttribEnchantedCount();
	
	bool bFullyEnchanted = true;
	if( iMaxEnchantCount - iCurrEnchantedCount > 0 )
	{
		bFullyEnchanted = false;
	}
	

	CX2DamageManager::EXTRA_DAMAGE_TYPE currExtraDamageType = g_pData->GetEnchantItem()->GetExtraDamageType( pItem->GetItemData().m_EnchantedAttribute );



	// 무기/방어구에 따라서 다르게 처리해야 하는 부분
	bool bIsWeapon = false;
	if(pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_WEAPON)
	{
		bIsWeapon = true;
		// 현재 속성 이름 셋팅 해주고..
		CKTDGUIStatic* pStaticAttribName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Current_Attribute" );
		if ( pStaticAttribName != NULL )
		{
			pStaticAttribName->SetString( 0, g_pData->GetDamageManager()->GetExtraDamageName( currExtraDamageType ).c_str() );

#ifdef FIX_TOOLTIP
			if ( true == bFullyEnchanted )
				pStaticAttribName->SetOffsetPos( D3DXVECTOR2(20, 0) );
#endif FIX_TOOLTIP
		}
	}
	else if(pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_DEFENCE)
	{
		bIsWeapon = false;
		// 현재 속성 이름 셋팅 해주고..
		CKTDGUIStatic* pStaticAttribName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Current_Attribute" );
		if ( pStaticAttribName != NULL )
		{
			pStaticAttribName->SetString( 0, g_pData->GetEnchantItem()->GetEnchantResistName( currExtraDamageType ).c_str() );

#ifdef FIX_TOOLTIP
			if ( true == bFullyEnchanted )
				pStaticAttribName->SetOffsetPos( D3DXVECTOR2(20, 0) );
#endif FIX_TOOLTIP
		}
	}



	CKTDGUIStatic* pStatic_First_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_First_Attribute_Notice" );
	CKTDGUIStatic* pStatic_El_Stone_Num = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );

	// 버튼 세팅해주자..
	wstringstream wstrstm;
	wstringstream wstrstm2;
	CKTDGUIButton* pButtonSlot;
	// 엘의 조각이 몇개 필요한지 보고
	int needItemNum = 0;

#ifdef PVP_SEASON2_SOCKET
	int iAttributeLevel = 0;
	
	if ( 0 < pItem->GetItemTemplet()->GetAttributeLevel() )
		iAttributeLevel = pItem->GetItemTemplet()->GetAttributeLevel();
	else
		iAttributeLevel = pItem->GetItemTemplet()->GetUseLevel();

	g_pData->GetEnchantItem()->GetAttribEnchantRequireMagicStoneCount( bIsWeapon, pItem->GetItemData().GetAttribEnchantedCount(), 
		iAttributeLevel, pItem->GetItemTemplet()->GetItemGrade(), needItemNum );
#else
	g_pData->GetEnchantItem()->GetAttribEnchantRequireMagicStoneCount( bIsWeapon, pItem->GetItemData().GetAttribEnchantedCount(), 
		pItem->GetItemTemplet()->GetUseLevel(), pItem->GetItemTemplet()->GetItemGrade(), needItemNum );
#endif

	wstrstm.str( L"" );
    if( true == bFullyEnchanted )
	{
		switch( iCurrEnchantedCount )
		{
		case 3:
			{
				pStaticUpgradeNotice->GetString(0)->msg = GET_STRING( STR_ID_4573 );
#ifdef FIX_TOOLTIP
				pStaticUpgradeNotice->SetOffsetPos( D3DXVECTOR2(40, 0) );
#endif FIX_TOOLTIP
			} break;

		case 2:
			{
				pStaticUpgradeNotice->GetString(0)->msg = GET_STRING( STR_ID_931 );
#ifdef FIX_TOOLTIP
				pStaticUpgradeNotice->SetOffsetPos( D3DXVECTOR2(40, 0) );
#endif FIX_TOOLTIP
			} break;

		case 1:
			{
				pStaticUpgradeNotice->GetString(0)->msg = L"";
			} break;
		}
	}
	else
	{
		wstrstm << GET_REPLACED_STRING( ( STR_ID_932, "i", needItemNum ) );
		pStaticUpgradeNotice->SetString(1, wstrstm.str().c_str() );
	}



	for ( int i = 0; i <= 6; i++ )
	{
		wstrstm.str( L"" );
		wstrstm2.str( L"" );
		wstrstm << L"Button_El_" << i;

		pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm.str().c_str() );

		int itemID;
		if(i == 0)
		{
			itemID = CX2EnchantItem::ATI_UNKNOWN;
		}
		else
		{
			itemID = g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i );
		}
		int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID );
		



		// 버튼 켜고 꺼주고
		if( itemNum >= needItemNum && 
			false == bFullyEnchanted &&
			true == CX2EnchantItem::CanEnchantAttribute( pItem->GetItemData().m_EnchantedAttribute, (CX2EnchantItem::ENCHANT_TYPE)i ) )
		{
			pButtonSlot->SetShowEnable( true, true );
			wstrstm2 << needItemNum << L"(" << itemNum << L")";

			CX2DamageManager::EXTRA_DAMAGE_TYPE eNewEnchantedExtraDamage = g_pData->GetEnchantItem()->GetExtraDamageType( pItem->GetItemData().m_EnchantedAttribute, (CX2EnchantItem::ENCHANT_TYPE)i );

			if( true == bIsWeapon )
			{
				// 버튼에 들어가는 extra damage name
				if(i == 0)
					pStatic_First_Attribute_Notice->SetString( i, GET_STRING( STR_ID_933 ) );
				else
					pStatic_First_Attribute_Notice->SetString( i, g_pData->GetDamageManager()->GetExtraDamageName( eNewEnchantedExtraDamage, false ).c_str() );

				// 버튼 툴팁
				if(i != 0)
#ifdef FIX_TOOLTIP
					pButtonSlot->SetGuideDesc( g_pData->GetEnchantItem()->GetToolTip( eNewEnchantedExtraDamage, NULL, true, false ).c_str() );
#else
					pButtonSlot->SetGuideDesc( g_pData->GetEnchantItem()->GetToolTip( eNewEnchantedExtraDamage, NULL ).c_str() );
#endif FIX_TOOLTIP
			}
			else
			{
				// 버튼에 들어가는 extra damage name
				if(i == 0)				
					pStatic_First_Attribute_Notice->SetString( i, GET_STRING( STR_ID_933 ) );
				else
					pStatic_First_Attribute_Notice->SetString( i, g_pData->GetEnchantItem()->GetEnchantResistName( eNewEnchantedExtraDamage ).c_str() );

				if(i != 0)
					pButtonSlot->SetGuideDesc( g_pData->GetEnchantItem()->GetEnchantResistToolTip( eNewEnchantedExtraDamage ).c_str()  );
			}
		}
		else
		{
			// 속성 인챈트 버튼 비활성화
			wstrstm2 << L"(" << itemNum << L")";
			
			pButtonSlot->SetShowEnable( true, false );
			pStatic_First_Attribute_Notice->SetString( i, GET_STRING( STR_ID_383 ) );
		}
		
		// 몇개 필요한지 몇개 있는지 써주고
		pStatic_El_Stone_Num->SetString(i, wstrstm2.str().c_str() );
	}









	// 아래쪽 설치된 속성 표시 static 및 속성 제거하기 버튼 
	CKTDGUIStatic* pStatic_IfOneSlot = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_SeeWhenArmor" );
	CKTDGUIStatic* pStatic_IfTwoSlot = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_DoNotSeeWhenArmor" );
	CKTDGUIStatic* pStatic_IfThreeSlot = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_ExtraSlot" );


	CKTDGUIStatic* pStatic_Slot_Gray_Back_2 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Gray_Back_Slot4" );
	CKTDGUIStatic* pStatic_remove_noactive2 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_remove_noactive2" );

	CKTDGUIStatic* pStatic_Gray_Back_Slot3 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Gray_Back_Slot5" );
	CKTDGUIStatic* pStatic_remove_noactive3 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Extra_Button" );


	switch( iMaxEnchantCount )
	{
	case 0:
		{
			ASSERT( !"NOT GOOD" );
		} break;

	case 1:
		{
			if( NULL != pStatic_IfOneSlot )
				pStatic_IfOneSlot->SetShow( true );
			if( NULL != pStatic_IfTwoSlot )
				pStatic_IfTwoSlot->SetShow( false );
			if( NULL != pStatic_IfThreeSlot )
				pStatic_IfThreeSlot->SetShow( false );



			if( NULL != pStatic_Slot_Gray_Back_2 )
				pStatic_Slot_Gray_Back_2->SetShow( false );
			if( NULL != pStatic_remove_noactive2 )
				pStatic_remove_noactive2->SetShow( false );

			if( NULL != pStatic_Gray_Back_Slot3 )
				pStatic_Gray_Back_Slot3->SetShow( false );
			if( NULL != pStatic_remove_noactive3 )
				pStatic_remove_noactive3->SetShow( false );



		} break;

	case 2:
		{
			if( NULL != pStatic_IfOneSlot )
				pStatic_IfOneSlot->SetShow( false );
			if( NULL != pStatic_IfTwoSlot )
				pStatic_IfTwoSlot->SetShow( true );
			if( NULL != pStatic_IfThreeSlot )
				pStatic_IfThreeSlot->SetShow( false );


			if( NULL != pStatic_Slot_Gray_Back_2 )
				pStatic_Slot_Gray_Back_2->SetShow( true );
			if( NULL != pStatic_remove_noactive2 )
				pStatic_remove_noactive2->SetShow( true );

			if( NULL != pStatic_Gray_Back_Slot3 )
				pStatic_Gray_Back_Slot3->SetShow( false );
			if( NULL != pStatic_remove_noactive3 )
				pStatic_remove_noactive3->SetShow( false );


		} break;

	case 3:
		{
			if( NULL != pStatic_IfOneSlot )
				pStatic_IfOneSlot->SetShow( false );
			if( NULL != pStatic_IfTwoSlot )
				pStatic_IfTwoSlot->SetShow( true );
			if( NULL != pStatic_IfThreeSlot )
				pStatic_IfThreeSlot->SetShow( true );


			if( NULL != pStatic_Slot_Gray_Back_2 )
				pStatic_Slot_Gray_Back_2->SetShow( true );
			if( NULL != pStatic_remove_noactive2 )
				pStatic_remove_noactive2->SetShow( true );

			if( NULL != pStatic_Gray_Back_Slot3 )
				pStatic_Gray_Back_Slot3->SetShow( true );
			if( NULL != pStatic_remove_noactive3 )
				pStatic_remove_noactive3->SetShow( true );


		} break;
	}



	//속성 제거하기 버튼이랑 현재 박힌 속성 세팅
	CKTDGUIStatic* pStaticEmptySlot[3] = { NULL, };
	CKTDGUIButton* pButtonRemoveSlot[3] = { NULL, };
	CKTDGUIStatic* pStaticNoticeSlot[3] = { NULL, };

	WCHAR wszControlName[256] = L"";
	for( int i=0; i<3; i++) 
	{
		StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"g_pStatic_Empty_Slot%d", i+1 );
		pStaticEmptySlot[i] = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( wszControlName );
		if( NULL != pStaticEmptySlot[i] )
		{
			pStaticEmptySlot[i]->SetShow( false );
		}

		StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"Button_Remove%d", i+1 );
		pButtonRemoveSlot[i] = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wszControlName );
		if( NULL != pButtonRemoveSlot[i] )
		{
			pButtonRemoveSlot[i]->SetShowEnable( false, false );
		}

		StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"g_pStatic_Enchant%d", i+1 );
		pStaticNoticeSlot[i] = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( wszControlName );
		if( NULL != pStaticNoticeSlot[i] )
		{
			pStaticNoticeSlot[i]->SetShowEnable( false, false );
		}

		if( i > iMaxEnchantCount - 1 )
		{
			continue;
		}		


		if( pItem->GetItemData().m_EnchantedAttribute.m_aEnchantedType[i] <= 0 )
		{
			if ( pStaticEmptySlot[i] != NULL )
				pStaticEmptySlot[i]->SetShow( true );
		}
		else
		{
			CX2EnchantItem::ENCHANT_TYPE enchantedType = (CX2EnchantItem::ENCHANT_TYPE) pItem->GetItemData().m_EnchantedAttribute.m_aEnchantedType[i];
			CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType = g_pData->GetEnchantItem()->GetExtraDamageType( enchantedType );

			if( NULL != pStaticNoticeSlot[i] )
			{
				if( true == bIsWeapon )
				{
					if( NULL != pStaticNoticeSlot[i] )
						pStaticNoticeSlot[i]->SetString( 0, g_pData->GetDamageManager()->GetExtraDamageName( extraDamageType ).c_str() );
				}
				else
				{
					if( NULL != pStaticNoticeSlot[i] )
						pStaticNoticeSlot[i]->SetString( 0, g_pData->GetEnchantItem()->GetEnchantResistName( extraDamageType ).c_str() );
				}
				pStaticNoticeSlot[i]->SetShowEnable(true, true);
			}

			if ( pButtonRemoveSlot[i] != NULL )
				pButtonRemoveSlot[i]->SetShowEnable( true, true );
		}
	}


}

bool CX2UIShop::Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( int slotNum, int enchantID, bool bDebug )
{
	KEGS_ATTRIB_ENCHANT_ITEM_REQ kPacket;
	kPacket.m_iItemUID = m_AttributeEnchantItemUID;
	kPacket.m_cAttribEnchantSlotNo = slotNum;
	kPacket.m_cAttribEnchantID = enchantID;

	g_pData->GetServerProtocol()->SendPacket( EGS_ATTRIB_ENCHANT_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK );

	return true;
}

bool CX2UIShop::Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ATTRIB_ENCHANT_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );



	if ( g_pMain->DeleteServerPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//특수 처리 고고싱
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			if(g_pData->GetUIManager()->GetUIInventory() != NULL)
			{		
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			}
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}

			ResetAtrribEnchantWindow();

		}

		return true;
	}

	return false;
}


#ifdef AUTH_AUTO_SELL_ITEM
void CX2UIShop::AuthAutoSellItem( CX2Inventory::SORT_TYPE SortType )
{
	if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel()
#ifdef CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
		|| CX2User::XUAL_OPERATOR == g_pData->GetMyUser()->GetAuthLevel()
#endif //CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
		)
	{
		if(g_pData->GetUIManager()->GetUIShop() == NULL)
		{
			g_pData->GetUIManager()->CreateUIShop();
		}

		const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

		for ( int i = 0; i < kInventory.GetItemMaxNum( SortType ); i++ )
		{
			CX2Item* pItem = kInventory.GetItem( SortType, i );
			if ( pItem == NULL || 
                pItem->GetItemTemplet() == NULL 
                )
				continue;

			if(pItem->GetItemTemplet()->GetShopPriceType() == CX2Item::SPT_GP)
			{
				m_SellItemUID = pItem->GetUID();
				m_SellItemNum = pItem->GetItemData().m_Quantity;
				g_pData->GetUIManager()->GetUIShop()->Handler_EGS_SELL_ITEM_REQ();	
			}

#ifdef JUNK_AVATAR
			if ( CX2Item::SPT_NONE == pItem->GetItemTemplet()->GetShopPriceType() )
			{
				m_SellItemUID = pItem->GetUID();
				m_SellItemNum = pItem->GetItemData().m_Quantity;
				g_pData->GetUIManager()->GetUIInventory()->AuthDeleteItem( m_SellItemUID, m_SellItemNum );
			}
#endif
		}
	}
}

#endif

//{{ kimhc // 2009-09-16 // 길드
#ifdef	GUILD_MANAGEMENT
bool CX2UIShop::CanBuyItemToCreateGuild() const
{
	if ( g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
	{
		ASSERT( !"GetMyUser or GetSelectUnit is NULL" );
		return false;
	}

	// 이미 길드에 가입되어 있지는 않은가?
	if ( g_pData->GetGuildManager()->DidJoinGuild() == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4498 ), g_pMain->GetNowState() );
		return false;
	}

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();


	// 길드창단허가서가 이미 있지는 않은가?
	if ( kInventory.GetItemByTID( GUILD_CREATE_ITEM_ID, true ) != NULL ) 
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4497 ), g_pMain->GetNowState() );
		return false;
	}

	// 길드 퀘스트를 클리어 했는가?
	if ( g_pData->GetQuestManager()->GetUnitCompleteQuest( GUILD_QUEST_ID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4496 ), g_pMain->GetNowState() );
		return false;
	}


	// 구입 가능
	return true;
}

bool CX2UIShop::CanBuyItemToExpandGuild() const
{
	if ( g_pData->GetGuildManager() == NULL )
	{
		ASSERT( !L"GuildManager is NULL" );
		return false;
	}

	if ( g_pData->GetGuildManager()->DidJoinGuild() == false ||
		 g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade != CX2GuildManager::GUG_MASTER )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4596 ), g_pMain->GetNowState() );
		return false;
	}

	if ( g_pData->GetGuildManager()->GetGuildInfo()->m_usMaxNumOfGuildMember >= _CONST_GUILD_MANAGER_::g_usMaxNumOfGuildMembers )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4597 ), g_pMain->GetNowState() );
		return false;
	}

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	// 이미 있지는 않은가?
	if ( kInventory.GetItemByTID( GUILD_EXPANSION_ITEM_ID, true ) != NULL ) 
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4614 ), g_pMain->GetNowState() );
		return false;
	}

#ifdef EVENT_GUILD_ITEM
	else if ( kInventory.GetItemByTID( EVENT_GUILD_EXPANSION_ITEM_ID, true ) != NULL ) 
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4614 ), g_pMain->GetNowState() );
		return false;
	}
#endif //EVENT_GUILD_ITEM

	return true;
}
#endif	GUILD_MANAGEMENT
//}} kimhc // 2009-09-16 // 길드

//{{ JHKang / 강정훈 / 2010/10/12 / 현재 아이템이 들어갈 인벤토리의 타입 구하기
#ifdef MODIFY_SHOP_BUY_ITEM
CX2Inventory::SORT_TYPE CX2UIShop::GetCurrInventoryType( CX2Item::ITEM_TYPE type )
{
	switch ( type )
	{
	case CX2Item::IT_WEAPON:
	case CX2Item::IT_DEFENCE:
		return CX2Inventory::ST_EQUIP;
		break;
	case CX2Item::IT_ACCESSORY:
		return CX2Inventory::ST_ACCESSORY;
		break;
	case CX2Item::IT_QICK_SLOT:
		return CX2Inventory::ST_QUICK_SLOT;
		break;
	case CX2Item::IT_MATERIAL:
		return CX2Inventory::ST_MATERIAL;
		break;
	case CX2Item::IT_SPECIAL:
	case CX2Item::IT_SKILL:
	case CX2Item::IT_SKILL_MEMO:
		return CX2Inventory::ST_SPECIAL;
		break;
	case CX2Item::IT_QUEST:
		return CX2Inventory::ST_QUEST;
		break;
	case CX2Item::IT_NONE:
	default:
		return CX2Inventory::ST_NONE;
		break;
	}
}
#endif MODIFY_SHOP_BUY_ITEM
//}} JHKang / 강정훈 / 2010/10/12 / 현재 아이템이 들어갈 인벤토리의 타입 구하기

#endif

#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT	
bool CX2UIShop::Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_REQ()
{	
	g_pData->GetServerProtocol()->SendID( EGS_SUPPORT_MATERIAL_EVENT_TIME_REQ );
	g_pMain->AddServerPacket( EGS_SUPPORT_MATERIAL_EVENT_TIME_ACK );

	return true;
}

bool CX2UIShop::Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SUPPORT_MATERIAL_EVENT_TIME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_SUPPORT_MATERIAL_EVENT_TIME_ACK ) == true )
	{
		m_iAddEnchantLevel = ( true == kEvent ) ? 1 : 0 ;
		UpdateEnchantWindow();
	}

	return true;
}
#endif // SERV_SUPPORT_MATERIAL_ENCHANT_EVENT	

#ifdef SERV_CATCH_HACKUSER_INFO
bool CX2UIShop::Handler_EGS_CATCH_HACKUSER_INFO_NOT( KEGS_CATCH_HACKUSER_INFO_NOT& kEGS_CATCH_HACKUSER_INFO_NOT )
{
	g_pData->GetServerProtocol()->SendPacket( EGS_CATCH_HACKUSER_INFO_NOT, kEGS_CATCH_HACKUSER_INFO_NOT );

	return true;
}
#endif SERV_CATCH_HACKUSER_INFO

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
bool CX2UIShop::Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ITEM_EXCHANGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ITEM_EXCHANGE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				IF_EVENT_ENABLED( CEI_EVENT_ADAMS_SHOP )
				{
#ifdef SERV_GROW_UP_SOCKET
					g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetGrowUpPoint( CX2Unit::GUT_EXCHANGE_COUNT, kEvent.m_iExchangeCount, g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
#endif SERV_GROW_UP_SOCKET
					g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

					if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
						g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
#ifdef RANDOM_EXCHANGE_RESULT_VIEW 
					// 더미 : OpenDecompositionResultWindow 안에서 인벤토리 정리를 안 하기 위한 빈 벡터 
					std::vector<KInventoryItemInfo> vecDummyInfo;
					vecDummyInfo.clear();
					g_pData->GetUIManager()->GetUIInventory()->SetItemObtainResultType(CX2UIInventory::IORT_RANDOM_EXCHANGE);
					g_pData->GetUIManager()->GetUIInventory()->OpenResolveResultWindow( kEvent.m_mapResultItem, vecDummyInfo, false ); 
#endif RANDOM_EXCHANGE_RESULT_VIEW
				}
			}
		return true;
		}
	}
	return false;
}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP