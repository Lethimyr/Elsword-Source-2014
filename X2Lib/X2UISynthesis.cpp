#include "StdAfx.h"

#ifdef SERV_SYNTHESIS_AVATAR

#include ".\X2UISynthesis.h"

CX2UISynthesisItem::CX2UISynthesisItem( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2UIDragable( pNowStage, NULL ),
m_bShow(false),
m_MovedPosition(0,0),					// D3DXVECTOR2
m_DLGPosition(0,0),						// D3DXVECTOR2
m_DLGSize(0,0),							// D3DXVECTOR2
m_pDLGSynthesis(NULL),
m_pDLGSynthesisItemResult(NULL),
m_pDLGOpenEffect(NULL),
m_pItemData(NULL),
m_iKeyData(0),
m_iComboBoxIndex(0),
m_iSocketNo(0),
m_fPlayTime(0),
m_bPlaySynthesisItem(false)
{
	m_mapItemData.clear();
	m_mapAvatarSocketOptionGroupID.clear();
	
	m_mapSynthesisResultInsertedItem.clear();
	m_vecSynthesisSlot.clear();
}


CX2UISynthesisItem::~CX2UISynthesisItem(void)
{
	DeleteItem();

	SAFE_DELETE_DIALOG(m_pDLGSynthesis);
	SAFE_DELETE_DIALOG(m_pDLGOpenEffect);
}


HRESULT CX2UISynthesisItem::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if(m_bShow == true)
	{
		g_pTFieldGame->SetEnableKeyProcess(false);
	}

	if ( m_bPlaySynthesisItem == true )
	{
		m_fPlayTime += fElapsedTime;

		const float fResolveItemPlayTime = 2.0f;
		if ( m_fPlayTime >= fResolveItemPlayTime)
		{
			m_bPlaySynthesisItem = false;
			m_fPlayTime = 0.0f;

			UpdateOpenSynthesisItemAck();
		}
	}

	if(m_pDLGSynthesis != NULL && m_pDLGSynthesis->GetIsMouseOver() == true)
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

bool CX2UISynthesisItem::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_bShow == false)
		return false;

	CX2SlotManager::MsgProc( hWnd, uMsg, wParam, lParam );

	bool bFlag = false;
	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	//case WM_MOUSEWHEEL:
		{
			if ( false == m_pDLGSynthesis->GetIsMouseOver())
			{
				return false;
			}
//			bFlag = true;
		}
		break;
	default:
		break;
	}

	switch( uMsg )
	{
	case WM_RBUTTONUP:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			bFlag = MouseRButtonUp( mousePos );
		}
		break;
	default:
		break;
	}

	return bFlag;
}

bool CX2UISynthesisItem::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case USICM_SYNTHESIS_ITEM_RESULT_OK:
		{
			if ( m_pDLGSynthesisItemResult != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSynthesisItemResult, NULL, false );
			}
			m_pDLGSynthesisItemResult = NULL;
			
			return true;
		}
		break;

	case USICM_SYNTHESIS_ITEM_CANCLE:
		{
			SetShow( false );

			return true;
		}
		break;

	case USICM_SYNTHESIS_ITEM_OK:
		{
			if(m_iKeyData != 0 && m_mapItemData.size() == 4 && m_iComboBoxIndex != 0)
			{
				SelectSocketNo();
				OpenEffect();
			}
			else if(m_mapItemData.size() != 4)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
					GET_STRING( STR_ID_12157 ), g_pMain->GetNowState() );
			}
			else if(m_iKeyData == 0)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
					GET_STRING( STR_ID_12156 ), g_pMain->GetNowState() );
			}
			else if(m_iComboBoxIndex == 0)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
					GET_STRING( STR_ID_37 ), g_pMain->GetNowState() );	
			}
		}
		break;

	case USICM_SYNTHESIS_ITEM_OPTION_CHANGED:
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)lParam;
			ChangeItemOption( pComboBox->GetDialog(), pComboBox->GetSelectedItemIndex() );
		}
		break;
	default:
		break;
	}

	return false;
}

bool CX2UISynthesisItem:: UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_SYNTHESIS_ITEM_ACK:
		{
			return Handler_EGS_SYNTHESIS_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
	case EGS_SYNTHESIS_SOCKET_GROUPID_ACK:
		{
			return Handler_EGS_SYNTHESIS_SOCKET_GROUPID_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	}
	return false;
}

void CX2UISynthesisItem::SetShow(bool bShow)
{
	if( g_pData->GetUIManager()->GetUIResolveItem() != NULL && g_pData->GetUIManager()->GetUIResolveItem()->GetShow() == true )
	{
		return;
	}

	SAFE_DELETE_DIALOG(m_pDLGSynthesisItemResult);
	m_pDLGSynthesisItemResult = NULL;
    
	m_bShow	= bShow;

	g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();

	if(bShow)
	{
		RegisterLuaBind();

		if(m_pDLGSynthesis == NULL)
		{
		m_pDLGSynthesis = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_SYNTHESIS.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSynthesis );
		}
		
		if(g_pData->GetUIManager()->GetUIResolveItem() == NULL)
		{
//			g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_SYNTHESIS);
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
		}
		else if(g_pData->GetUIManager()->GetUIResolveItem()->GetShow() ==false)
		{
//			g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_SYNTHESIS);
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
		}

		// 서버에 세팅 옵션 정보 요청
		Handler_EGS_SYNTHESIS_SOCKET_GROUPID_REQ();

	}
	else	// 꺼질 때 처리해야 할 부분
	{
		DeleteItem();
//		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_SYNTHESIS);
//		CX2ItemSlotManager::InvalidSelectedItem();
//		CX2ItemSlotManager::InvalidSlotDesc();
	}

	/*
	if(g_pData->GetUIManager()->GetUIResolveItem() != NULL)
	{
		bShow = false;
		m_bShow	= bShow;
	}

	if(g_pData->GetUIManager()->GetUIResolveItem() != NULL)
	{
		if(g_pData->GetUIManager()->GetUIResolveItem()->GetShow() ==true)
		{
			bShow = false;
			m_bShow	= bShow;
		}
		else
		{
			bShow = true;
			m_bShow	= bShow;
		}
	}*/

	m_pDLGSynthesis->SetShowEnable(bShow, bShow);

}

void CX2UISynthesisItem::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);
	if(m_pDLGSynthesis != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGSynthesis, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGSynthesis, false );	}
}

void CX2UISynthesisItem::SetPosition(D3DXVECTOR2 vec)
{
	// 슬롯과 다이얼로그 위치를 조절해 준다

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


bool CX2UISynthesisItem::Handler_EGS_SYNTHESIS_SOCKET_GROUPID_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_SYNTHESIS_SOCKET_GROUPID_REQ );
	g_pMain->AddServerPacket( EGS_SYNTHESIS_SOCKET_GROUPID_ACK );
	return true;
}


bool CX2UISynthesisItem::Handler_EGS_SYNTHESIS_SOCKET_GROUPID_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SYNTHESIS_SOCKET_GROUPID_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_SYNTHESIS_SOCKET_GROUPID_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_mapAvatarSocketOptionGroupID = kEvent.m_mapAvatarSocketOptionGroupID;
			return true;
		}		
	}
	return false;
}

bool CX2UISynthesisItem::Handler_EGS_SYNTHESIS_ITEM_REQ()
{
	if(m_mapItemData.size() != 4)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
			GET_STRING( STR_ID_12157 ), g_pMain->GetNowState() );
		return false;
	}
	else if(m_iKeyData == 0)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
			GET_STRING( STR_ID_12156 ), g_pMain->GetNowState() );
		return false;
	}
	else if(m_iSocketNo == 0)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
			GET_STRING( STR_ID_37 ), g_pMain->GetNowState() );	
		return false;
	}

	KEGS_SYNTHESIS_ITEM_REQ kPacket;

	kPacket.m_mapSynthesisData = m_mapItemData;
	kPacket.m_iKeyData = m_iKeyData;
	kPacket.m_iSocketNo = m_iSocketNo;

	g_pData->GetServerProtocol()->SendPacket( EGS_SYNTHESIS_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SYNTHESIS_ITEM_ACK );

	DeleteItem();

	return true;
}


bool CX2UISynthesisItem::Handler_EGS_SYNTHESIS_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SYNTHESIS_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_SYNTHESIS_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			OpenSynthesisResultWindow( kEvent.m_mapInsertedItem, kEvent.m_vecKInventorySlotInfo);
			return true;
		}		
	}
	return false;
}


wstring CX2UISynthesisItem::GetSlotItemDesc()
{
	std::wstring slotItemDesc = L"";

	if ( m_pNowOverItemSlot != NULL && m_bShowRBDownMessage == true )
	{
		if ( m_pNowOverItemSlot->GetDialog() == NULL )
		{
			slotItemDesc = m_pNowOverItemSlot->GetSlotDesc();
		}
		else
		{	
			CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( 
				m_pNowOverItemSlot->GetItemUID() );
			if ( pkItem != NULL )
				slotItemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID(), false );
			else
			{
				CX2Item* pItem = NULL;
				map< int, int >::iterator mit;

				if(m_pNowOverItemSlot->GetPeriod() != 0)
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
						slotItemDesc = GetSlotItemDescByTID( pItem, m_pNowOverItemSlot->GetItemTID(), false );
						SAFE_DELETE( pItem );
					}
				}
				else
				{
					slotItemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID() );
				}
			}
		}

		slotItemDesc += GET_STRING( STR_ID_399 );
	}

	return slotItemDesc;
}

void CX2UISynthesisItem::RegisterSynthesisItem(CX2SlotItem* pItemSlot)
{
	if(pItemSlot == NULL)
		return;

	if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
		g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2650 ), g_pMain->GetNowState() );		
		return;
	}

	CX2Item* pItem;

	if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
	{
		pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );

#ifdef ENABLE_RESOLVE_FASHION

		if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_STRING( STR_ID_821 ), g_pMain->GetNowState() );

			return;
		}

		if(pItem->GetItemTemplet()->GetItemID() == 65000550)
		{
			SetInventorySort();

			//m_pSynthesisSlot = pItemSlot;

			std::vector< SynthesisItemSlot >::iterator target;
//			std::map<SynthesisItemSlot, CX2SlotItem*>::iterator target;
	//		for(target = m_mapSynthesisSlot.begin(); target != m_mapSynthesisSlot.end(); ++ target )
			for(target = m_vecSynthesisSlot.begin(); target != m_vecSynthesisSlot.end(); ++ target )
			{
// 				CX2SlotItem* pSlot = target->second;
// 				if ( pSlot == NULL )
// 					continue;

				//
				if( target->m_iItemUID == pItemSlot->GetItemUID()  )
				{
					return;
				}
			}

			SynthesisItemSlot sItemSlot;
			sItemSlot.m_SORT_TYPE = pItemSlot->GetSortType();
			sItemSlot.m_iItemUID = pItemSlot->GetItemUID();
			sItemSlot.m_pImteSlot = pItemSlot;

			m_vecSynthesisSlot.push_back(sItemSlot);
//			m_mapSynthesisSlot.insert( std::make_pair(sItemSlot, pItemSlot));
			
			AttachSelectedPicture( pItemSlot );

			SetSlotItem(pItem);

			if(m_iKeyData != 0 && m_mapItemData.size() == 4)
			{
				AddOption();
			}

			return;
		}

		//{{ kimhc // 2010-01-06 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
		if ( pItem->GetItemTemplet()->GetIsPcBang() == true )
		{
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
			return;
		}
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-06 // PC방 프리미엄 서비스

		//{{ kimhc // 2009-09-08 // 봉인된 아이템 분해 불가
#ifdef	SEAL_ITEM
		if ( pItem->GetItemData().m_bIsSealed == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4477 ), g_pMain->GetNowState() );
			return; 
		}
#endif	SEAL_ITEM
		//}} kimhc // 2009-09-08 // 봉인된 아이템 분해 불가

		switch(pItem->GetItemTemplet()->GetItemType())
		{
		case CX2Item::IT_WEAPON:
		case CX2Item::IT_DEFENCE:
			{
				if ( pItem->GetItemTemplet()->GetFashion() == true )
				{
					if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY &&
						pItem->GetItemData().m_Period > 0 )
					{
						//기간제로 판명.
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
							GET_STRING( STR_ID_822 ), g_pMain->GetNowState() );

						return;
					}
				}
			} break;
		case CX2Item::IT_ACCESSORY:
			{
				if ( pItem->GetItemTemplet()->GetFashion() != true )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
						GET_STRING( STR_ID_12255 ), g_pMain->GetNowState() );

					return;
				}

				if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY &&
					pItem->GetItemData().m_Period > 0 )
				{
					//기간제로 판명.
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
						GET_STRING( STR_ID_822 ), g_pMain->GetNowState() );

					return;
				}

			} break;
		default:
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
					GET_STRING( STR_ID_12255 ), g_pMain->GetNowState() );

				return;
			}
		}


#else //ENABLE_RESOLVE_FASHION

		if ( m_NowInventorySortType != CX2Inventory::ST_EQUIP )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_STRING( STR_ID_12255 ), g_pMain->GetNowState() );

			return;
		}

#endif //ENABLE_RESOLVE_FASHION

#ifdef ITEM_RECOVERY_TEST
		if ( pItem == NULL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_STRING( STR_ID_821 ), g_pMain->GetNowState() );

			return;
		}
		if(true == pItem->IsDisabled())
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_800 ), g_pMain->GetNowState() );
			return; 
		}
#endif // ITEM_RECOVERY_TEST

	}		

	/* 임규수 일본 제거 청 합성~
	if(pItem->GetItemTemplet()->m_UnitType == CX2Unit::UT_CHUNG )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
			GET_STRING( STR_ID_12255 ), g_pMain->GetNowState() );

		return;
	}*/

	// 설정
	if(	 pItem->GetItemTemplet()->GetFashion() == true )	
	{
		if(  pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_WEAPON  ||
			 pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_DEFENCE  )
		{

			if( pItem->GetItemTemplet()->GetItemGrade() == CX2Item::IG_NORMAL)
			{
				if( pItem->GetItemTemplet()->GetSetID() ==0)
				{
					if(CheckSameUnitEquip(pItem))
					{

						if( m_mapItemData.size() >= 4 )
							return;

						SetInventorySort();

//						std::map<SynthesisItemSlot, CX2SlotItem*>::iterator target;
	//					for(target = m_mapSynthesisSlot.begin(); target != m_mapSynthesisSlot.end(); ++ target )
						std::vector< SynthesisItemSlot >::iterator target;
						for(target = m_vecSynthesisSlot.begin(); target != m_vecSynthesisSlot.end(); ++ target )
						{
//							CX2SlotItem* pSlot = target->second;
	//						if ( pSlot == NULL )
		//						continue;

							//  pSlot->GetItemUID() 
							if(target->m_iItemUID == pItemSlot->GetItemUID()  )
							{
								return;
							}
						}

						SynthesisItemSlot sItemSlot;
						sItemSlot.m_SORT_TYPE = pItemSlot->GetSortType();
						sItemSlot.m_iItemUID = pItemSlot->GetItemUID();
						sItemSlot.m_pImteSlot = pItemSlot;
						m_vecSynthesisSlot.push_back(sItemSlot);

						//m_mapSynthesisSlot.insert( std::make_pair(sItemSlot, pItemSlot));

						AttachSelectedPicture( pItemSlot );

						SetSlotItem(pItem);

						if(m_iKeyData != 0 && m_mapItemData.size() == 4)
						{
							AddOption();
						}
					}
				}
				else
				{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_12158 ), g_pMain->GetNowState() );		
				}

			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_12158 ), g_pMain->GetNowState() );
			}
		}
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_12255 ), g_pMain->GetNowState() );
		}
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_12255 ), g_pMain->GetNowState() );
	}
}

bool CX2UISynthesisItem::CheckSameUnitEquip( CX2Item* pkItem )
{
	for(std::map<int, UidType>::iterator target = m_mapItemData.begin()  ; target != m_mapItemData.end() ; ++target)
	{
		if(target == m_mapItemData.end())
		{
			return true;
		}

			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( target->second );

			if(pItem->GetItemTemplet()->GetUnitType() != pkItem->GetItemTemplet()->GetUnitType()
				|| pItem->GetItemTemplet()->GetEqipPosition() != pkItem->GetItemTemplet()->GetEqipPosition() )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
					GET_STRING( STR_ID_12155 ), g_pMain->GetNowState() );

				return false;
			}
	}

	return true;
}

void CX2UISynthesisItem::SetSlotItem(CX2Item *pkItem)
{
	m_pItemData = pkItem;

	if(m_pItemData->GetItemTemplet()->GetItemID() == 65000550)
	{
		KeyItem();
	}
	else
	{
		AttachItem();
	}
}

bool CX2UISynthesisItem::KeyItem()
{
		// 미리보기 아이템 셋팅

		CX2SlotItem* pToSlotItem	= NULL;

		pToSlotItem	= static_cast< CX2SlotItem* >( this->GetSlot( 4 ) );

		if ( pToSlotItem == NULL )
		{
			ASSERT( !"UIResolve Slot Is NULL" );
			return false;
		}

		pToSlotItem->DestroyItemUI();     

		if ( m_pItemData != NULL )
		{
		pToSlotItem->CreateItemUI( m_pItemData, 1 );
		m_iKeyData = pToSlotItem->GetItemUID();
		}

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );    

		return true;

}

bool CX2UISynthesisItem::AttachItem()
{
	// 미리보기 아이템 셋팅

	CX2SlotItem* pToSlotItem	= NULL;	

	for ( int i = 0; i < 4 ; i++ )
	{
		std::map<int, UidType>::iterator target = m_mapItemData.find(i);
		if(target != m_mapItemData.end())
		{
		}
		else
		{
			pToSlotItem	= static_cast< CX2SlotItem* >( this->GetSlot( i ) );

			if ( pToSlotItem == NULL )
			{
				ASSERT( !"UIResolve Slot Is NULL" );
				return false;
			}

			if ( m_mapItemData.empty() == false )
			{
				for(std::map<int, UidType>::iterator it = m_mapItemData.begin(); it != m_mapItemData.end(); ++it)
				{
					if(m_pItemData->GetUID() == it->second)
						return false;
				}
			}

			pToSlotItem->DestroyItemUI();        

			if ( m_pItemData != NULL )
			{
				pToSlotItem->CreateItemUI( m_pItemData, 1 );
				m_mapItemData.insert( std::make_pair(i, (int)pToSlotItem->GetItemUID()));
			}

			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );    

			return true;
		}
	}

	return false;

}


bool CX2UISynthesisItem::DeleteItem()
{
	CX2SlotItem* pToSlotItem	= NULL;

	for ( int i = 0; i < 5 ; i++ )
	{
		pToSlotItem	= static_cast< CX2SlotItem* >( this->GetSlot( i ) );

		if ( pToSlotItem == NULL )
		{
			ASSERT( !"UIResolve Slot Is NULL" );
			return false;
		}

		pToSlotItem->DestroyItemUI();        

		std::map<int, UidType>::iterator target = m_mapItemData.find(i);
		if(target != m_mapItemData.end())
		{
			m_mapItemData.erase(i);
		}
	}

	CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGSynthesis->GetControl( L"Option_ComboBox" );

	if ( pComboBox != NULL )
	{
		pComboBox->SetEnable( true );
		pComboBox->SetShow( true );
		pComboBox->RemoveAllItems();
	}


//	std::map<SynthesisItemSlot, CX2SlotItem*>::iterator target;
//	for(target = m_mapSynthesisSlot.begin(); target != m_mapSynthesisSlot.end(); ++ target )
	std::vector< SynthesisItemSlot >::iterator target;
	for(target = m_vecSynthesisSlot.begin(); target != m_vecSynthesisSlot.end(); ++ target )
	{
		CX2SlotItem* pSlot = target->m_pImteSlot;
		if ( pSlot == NULL )
			continue;

		CKTDGUIDialogType pkDialog = pSlot->GetDialog();
		if ( pkDialog == NULL )
			continue;

		CKTDGUIStatic* pkStatic = pkDialog->GetStatic_LUA( "StaticRoot_ItemSlot" );
		if ( pkStatic == NULL )
			continue;

		pkStatic->RemovePictureByIndex( 2 );
	}

	m_mapItemData.clear();
	m_mapSynthesisResultInsertedItem.clear();
	//m_mapSynthesisSlot.clear();
	m_vecSynthesisSlot.clear();

	m_iKeyData = 0;
	m_iComboBoxIndex = 0;
	m_iSocketNo = 0;

	return true;
}

bool CX2UISynthesisItem::SetInventorySort()
{
	if(g_pData->GetUIManager()->GetUIInventory() == NULL)
	{
		g_pData->GetUIManager()->CreateUIInventory();
	}

	bool bResult = g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();

	return bResult;
}


void CX2UISynthesisItem::OpenSynthesisResultWindow( std::map< int, int >& mapInsertedItem, 
											   std::vector< KInventoryItemInfo >& vecKInventorySlotInfo)
{
	SAFE_DELETE_DIALOG( m_pDLGSynthesisItemResult );

	m_mapSynthesisResultInsertedItem = mapInsertedItem;

	m_pDLGSynthesisItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Avatar_Synthesis_Result.lua" );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSynthesisItemResult );

	CKTDGUIButton* pControl = (CKTDGUIButton*) m_pDLGSynthesisItemResult->GetControl( L"Button_OK" );
	if ( pControl != NULL )
	{
		pControl->RequestFocus();
	}

	std::map< int, int >::iterator i = mapInsertedItem.begin();
	int resultItemID = i->first;

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGSynthesisItemResult->GetControl( L"Success_Window_Item_Slot" );
	if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
	{		
		if ( pStatic->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() ) == false )
		{
			pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
		}
	}

	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGSynthesisItemResult->GetControl( L"Static_Item_Name1" );
	if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
	{
		pStaticItemName->GetString(0)->msg = pItemTemplet->GetFullName_();
	}

	if(g_pData->GetUIManager()->GetUIInventory() != NULL)
	{		
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( vecKInventorySlotInfo );
	}
}

bool CX2UISynthesisItem::UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo)
{
	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
		if ( kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
		{
			// 은행 공유 char -> short 로 변경
			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
			if ( pItem != NULL )
			{				
				RemoveEqip( pItem->GetUID() );
			}
		}
	}

	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
		// 은행 공유 char -> short 로 변경
		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().RemoveItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
	}

	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
		if ( kInventorySlotInfo.m_iItemUID > 0 )
		{
			//CX2Item::ItemData* pItemData = new CX2Item::ItemData( kInventorySlotInfo );
			// 은행 공유 char -> short 로 변경
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().AddItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID, CX2Item::ItemData( kInventorySlotInfo ) );

			if ( kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
			{
				// 은행 공유 char -> short 로 변경
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
				if ( pItem != NULL )
				{
					AddEqip( pItem->GetUID() );
				}
			}
		}
	}

#if 0 
	if ( SetInventorySort( m_NowInventorySortType ) == false )
		return false;
#endif



	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}

	return true;
}

void CX2UISynthesisItem::RemoveEqip(UidType ItemUID)
{
	g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( ItemUID );
}

void CX2UISynthesisItem::AddEqip(UidType ItemUID)
{
	g_pData->GetMyUser()->GetSelectUnit()->AddEqip( ItemUID );
}

void CX2UISynthesisItem::AddOption()
{
	CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGSynthesis->GetControl( L"Option_ComboBox" );

	if ( pComboBox != NULL )
	{
		pComboBox->SetEnable( true );
		pComboBox->SetShow( true );
		pComboBox->RemoveAllItems();

		pComboBox->AddItem( GET_STRING( STR_ID_37 ), NULL, false );


		std::map<int, UidType>::iterator target = m_mapItemData.begin();
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( target->second );
		if(pItem != NULL)
		{
			const CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
			if(pItemTemplet == NULL)
				return;

			std::map< int, int >::iterator find_it = m_mapAvatarSocketOptionGroupID.find( (int)pItemTemplet->GetEqipPosition() );

			if ( find_it != m_mapAvatarSocketOptionGroupID.end() )
			{
				CX2SocketItem* pSocketItem = g_pData->GetSocketItem();
				if ( NULL != pSocketItem )
				{
					// 아이템 착용 레벨이 0 면 유저의 레벨을
					// 0보다 크면 아이템 레벨을 대입
					const int iLevel = 
						( 0 < pItemTemplet->GetUseLevel() ? pItemTemplet->GetUseLevel() : g_pData->GetSelectUnitLevel() );

					const int iSocketGroupID = find_it->second;

					vector<int> vecSocketIdList;	/// iSocketGroupID 에 해당하는 SocketID 들을 얻어옴
					if ( 0 != pSocketItem->GetSocketIdListForCashAvatar( iSocketGroupID, vecSocketIdList, pItemTemplet->GetItemType() ) )
					{
						// 얻어왔으면, 차례로 넣는다.
						BOOST_FOREACH( const int iSocketID, vecSocketIdList )
						{
							const CX2SocketItem::SocketData* pSocketData = pSocketItem->GetSocketData( iSocketID );
							if ( NULL != pSocketData )
								pComboBox->AddItem( pSocketData->GetSocketDesc( iLevel ).c_str(), NULL, false );
						}
					}
				}
			}
		}


		if ( pComboBox->GetNumItems() == 0 )
		{	
			pComboBox->SetEnable( false );
			pComboBox->SetShow( false );
		}
	}
}

void CX2UISynthesisItem::ChangeItemOption( CKTDGUIDialogType pDialog, int comboBoxIndex )
{
	m_iComboBoxIndex = 0;

	if(m_iKeyData != 0 && m_mapItemData.size() == 4)
	{
		m_iComboBoxIndex = comboBoxIndex;
	}
}

void CX2UISynthesisItem::SelectSocketNo()
{
	m_iSocketNo = 0;

	std::map<int, UidType>::iterator target = m_mapItemData.begin();
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( target->second );
	if(pItem != NULL)
	{
		const CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
		if ( pItemTemplet != NULL )
		{
			std::map< int, int >::iterator find_it = m_mapAvatarSocketOptionGroupID.find( (int)pItemTemplet->GetEqipPosition() );
			if ( find_it != m_mapAvatarSocketOptionGroupID.end() )
			{
				int iTmpGroupID = find_it->second;

				std::vector<int> vecSocketId;
				g_pData->GetSocketItem()->GetSocketIdListForCashAvatar( iTmpGroupID, vecSocketId, pItemTemplet->GetItemType() );

				if (vecSocketId.size() > m_iComboBoxIndex-1)
					m_iSocketNo = vecSocketId[m_iComboBoxIndex-1];
			}
		}
	}
}

void CX2UISynthesisItem::OpenEffect()
{
	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, false);
	m_pDLGOpenEffect = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Avatar_Synthesis_Effect.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOpenEffect );

	m_bPlaySynthesisItem = true;
}

void CX2UISynthesisItem::UpdateOpenSynthesisItemAck()
{
	if ( m_pDLGOpenEffect != NULL )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOpenEffect, NULL, false );
	}
	m_pDLGOpenEffect = NULL;

	Handler_EGS_SYNTHESIS_ITEM_REQ();
}

// 소켓 강화에 연관되는 아이템인지 판별하는 함수
bool CX2UISynthesisItem::CheckUseItem(UidType Uid)
{

	for(std::map<int, UidType>::iterator it = m_mapItemData.begin(); it != m_mapItemData.end(); ++it)
	{
		if(m_pItemData->GetUID() == Uid)
			return true;
	}

	return false;
}

bool CX2UISynthesisItem::MouseRButtonUp( D3DXVECTOR2 mousePos )
{    
	*m_pSlotBeforeDragging = NULL;
	*m_DraggingItemUID = -1;
	m_DraggingItemTID = -1;

	CX2SlotItem* pItemSlot = NULL;
	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL )
		return false;

	if( m_bPlaySynthesisItem == true )
		return false;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );

	if( pItem == NULL )
		return false; 

	if(pItem->GetItemTemplet()->GetItemID() == 65000550)
	{
		m_iKeyData = 0;
	}
	else
	{
		std::map<int, UidType>::iterator target = m_mapItemData.find(pItemSlot->GetSlotID());
		if(target != m_mapItemData.end())
		{
			m_mapItemData.erase(pItemSlot->GetSlotID());
		}
	}

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );

	//std::map<SynthesisItemSlot, CX2SlotItem*>::iterator target;
	//for(target = m_mapSynthesisSlot.begin(); target != m_mapSynthesisSlot.end();  )
	std::vector< SynthesisItemSlot >::iterator target;
	for(target = m_vecSynthesisSlot.begin(); target != m_vecSynthesisSlot.end(); )
	{
		CX2SlotItem* pSlot = target->m_pImteSlot;
		if ( pSlot == NULL )
			continue;

		// pSlot->GetItemUID()
		if( target->m_iItemUID == pItemSlot->GetItemUID()  )
		{
			SetShowSelectedPicture(pSlot, false);

			//m_mapSynthesisSlot.erase( target++ );
			m_vecSynthesisSlot.erase( target );
		}
		else
		{
			++ target;
		}
	}


	pItemSlot->DestroyItemUI();  

	CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGSynthesis->GetControl( L"Option_ComboBox" );

	if ( pComboBox != NULL )
	{
		pComboBox->SetEnable( true );
		pComboBox->SetShow( true );
		pComboBox->RemoveAllItems();
	}

	m_iComboBoxIndex = 0;
	m_iSocketNo = 0;

	return true;
}

void CX2UISynthesisItem::AttachSelectedPicture( CX2SlotItem* pItemSlot )
{
	CKTDGUIDialogType pkDialog = pItemSlot->GetDialog();
	if ( pkDialog == NULL )
		return;

	CKTDGUIStatic* pkStatic = pkDialog->GetStatic_LUA( "StaticRoot_ItemSlot" );
	if ( pkStatic == NULL )
		return;

	CKTDGUIControl::CPictureData* pPicture = pkStatic->GetPicture(2);
	if(pPicture != NULL)
	{
		pPicture->SetShow(true);
	}
	else
	{
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
		pPicture->SetTex( L"DLG_UI_Common_Texture13.tga", L"Check" );
		pPicture->SetPoint();
		pPicture->SetPos( D3DXVECTOR2( -1, -1 ) );
		pkStatic->AddPictureAt( 2, pPicture );
	}

	
	//pkStatic->AddPicture( pPicture );

	/*CKTDGUIControl::CPictureData* pPictureEquiped = new CKTDGUIControl::CPictureData();
	pPictureEquiped->SetTex( L"HQ_ShopEqipedBorder.tga" );
	pPictureEquiped->SetPoint( new CKTDGUIControl::UIPointData );
	pPictureEquiped->SetPos( D3DXVECTOR2( 0, 0 ) );
	pPictureEquiped->SetSize( pItemSlot->GetSize() );
	pPictureEquiped->SetShow( false );
	pPictureEquiped->SetColor( D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f ));

	pkStatic->AddPictureAt( 2, pPictureEquiped );*/
}

void CX2UISynthesisItem::SetShowSelectedPicture( CX2SlotItem* pItemSlot, bool bShow )
{
	CKTDGUIDialogType pkDialog = pItemSlot->GetDialog();
	if ( pkDialog == NULL )
		return;

	CKTDGUIStatic* pkStatic = pkDialog->GetStatic_LUA( "StaticRoot_ItemSlot" );
	if ( pkStatic == NULL )
		return;

	CKTDGUIControl::CPictureData* pPictureEquiped = pkStatic->GetPicture(2);
	if ( pPictureEquiped == NULL )
		return;

//	CKTDGUIControl::CPictureData* pPictureImpossible = pkStatic->GetPicture(1);
	//if ( pPictureImpossible == NULL )
		//return;

	pPictureEquiped->SetShow( bShow );

	/*if(false == bShow)
	{
		if ( g_pData->GetMyUser()->GetSelectUnit()->EqipAbility( pItemSlot->GetItemTID() ) == true )
		{
			pPictureImpossible->SetShow( false );	
		}
		else
		{
			pPictureImpossible->SetShow( true );
		}
	}
	else
	{
		pPictureImpossible->SetShow( false );
	}*/
}


bool CX2UISynthesisItem::CheckSynthesisUseItem(UidType Uid)
{

	//std::map<SynthesisItemSlot, CX2SlotItem*>::iterator target;
	//for(target = m_mapSynthesisSlot.begin(); target != m_mapSynthesisSlot.end(); ++ target )
	std::vector< SynthesisItemSlot >::iterator target;
	for(target = m_vecSynthesisSlot.begin(); target != m_vecSynthesisSlot.end(); ++ target )
	{
//		CX2SlotItem* pSlot = target->second;
	//	if ( pSlot == NULL )
		//	continue;

		// pSlot->GetItemUID() 
		if( target->m_iItemUID == Uid  )
		{
			return true;
		}
	}

	return false;
}

void CX2UISynthesisItem::ShowSynthesisCheckPicture( CX2Inventory::SORT_TYPE sortType )
{

//	std::map<SynthesisItemSlot, CX2SlotItem*>::iterator target;
//	for(target = m_mapSynthesisSlot.begin(); target != m_mapSynthesisSlot.end(); ++ target )
	std::vector< SynthesisItemSlot >::iterator target;
	for(target = m_vecSynthesisSlot.begin(); target != m_vecSynthesisSlot.end(); ++ target )
	{
		CX2SlotItem* pSlot = target->m_pImteSlot;
		if ( pSlot == NULL )
			continue;

		if( target->m_SORT_TYPE == sortType )	
			AttachSelectedPicture( pSlot );
	}
}

void CX2UISynthesisItem::OffSynthesisCheckPicture()
{
	//std::map<SynthesisItemSlot, CX2SlotItem*>::iterator target;
	//for(target = m_mapSynthesisSlot.begin(); target != m_mapSynthesisSlot.end(); ++ target )
	std::vector< SynthesisItemSlot >::iterator target;
	for(target = m_vecSynthesisSlot.begin(); target != m_vecSynthesisSlot.end(); ++ target )
	{
		CX2SlotItem* pSlot = target->m_pImteSlot;
		if ( pSlot == NULL )
			continue;

		SetShowSelectedPicture(pSlot, false);
	}
}

#endif
