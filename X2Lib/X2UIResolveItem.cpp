#include "StdAfx.h"

#ifdef SERV_MULTI_RESOLVE

#include ".\X2UIResolveItem.h"

CX2UIResolveItem::CX2UIResolveItem( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2UIDragable( pNowStage, NULL ),
m_DecompositionWindowNowPageNum(0),
m_bShow(false),
m_bEnable(true),
m_DLGSize(0,0),
m_vItemSlotPos(0,0),
m_pDLGResolveItem(NULL),
m_pDLGResolveItemResult(NULL),
m_pItemData(NULL),
m_pDLGOpenMoru(NULL),
m_fPlayTime(0),
m_bPlayResolveItem(false),
m_bInvenCheck(false),
m_iTotalCostED(0),
m_hMeshInstMoru(INVALID_MESH_INSTANCE_HANDLE)
{

	m_mapItemData.clear();
	m_mapResolveResultInsertedItem.clear();
	m_vecResolveSlot.clear();

	RegisterLuaBind();
	
	m_pDLGResolveItem = new CKTDGUIDialog( pNowStage, pFileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItem );

	if( g_pTFieldGame == NULL )
	{
//		m_pDLGResolveItem->SetPos(D3DXVECTOR2( 0, 200 )); // 던전내에서 분해창 필드상태랑 동일하게 위치시키기 (김석근)

		CX2SlotItem* pToSlotItem	= NULL;

		for ( int i = 0; i < 6 ; i++ )
		{
			std::map<int, UidType>::iterator target = m_mapItemData.find(i);
			if(target != m_mapItemData.end())
			{
			}
			else
			{
				pToSlotItem	= static_cast< CX2SlotItem* >( this->GetSlot( i ) );
//				pToSlotItem->SetPos(D3DXVECTOR2( pToSlotItem->GetPos().x, pToSlotItem->GetPos().y+200 )); // 던전내에서 분해창 필드상태랑 동일하게 위치시키기. (김석근)
				pToSlotItem->SetPos(D3DXVECTOR2( pToSlotItem->GetPos().x, pToSlotItem->GetPos().y ));
			}
		}
	}

	SetShow(true);
}

CX2UIResolveItem::~CX2UIResolveItem(void)
{
	DeleteItem();

	SAFE_DELETE_DIALOG(m_pDLGResolveItem);
	SAFE_DELETE_DIALOG(m_pDLGOpenMoru);

	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstMoru )
	{
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMoru );
	}
}


HRESULT CX2UIResolveItem::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if(m_bShow == true && g_pTFieldGame != NULL)
	{
		g_pTFieldGame->SetEnableKeyProcess(false);
	}

	if( m_hMeshInstMoru != INVALID_MESH_INSTANCE_HANDLE && 
		g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMoru ) == true )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMoru );
		if(NULL != pMeshInst)
		{			
			if( pMeshInst->GetXSkinAnim() != NULL )
			{
				pMeshInst->GetXSkinAnim()->GetRenderParam()->fOutLineWide = 1.2f;
			}
		}
	}

	if ( m_bPlayResolveItem == true )
	{
		m_fPlayTime += fElapsedTime;

		const float fResolveItemPlayTime = 2.0f;
		if ( m_fPlayTime >= fResolveItemPlayTime)
		{
			m_bPlayResolveItem = false;
			m_fPlayTime = 0.0f;

			if ( m_hMeshInstMoru != INVALID_MESH_INSTANCE_HANDLE )
			{
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMoru );
				if( NULL != m_pDLGOpenMoru )
				{
					m_pDLGOpenMoru->SetHasUnit(NULL);
				}
			}

			UpdateOpenResolveItemAck();
		}
	}

	if ( m_pDLGResolveItem != NULL && m_pDLGResolveItem->GetIsOverControl() == true )
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

bool CX2UIResolveItem::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
			if ( false == m_pDLGResolveItem->GetIsMouseOver())
			{
				return false;
			}
//			bFlag = true;		분해 창 열고 닫고 열면 인벤 Slot Move 안됨
		}
		break;
	default:
		break;
	}
	
	switch( uMsg )
	{
	/*case WM_MOUSEMOVE:
		{

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
		}
		break;*/
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

bool CX2UIResolveItem::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case URICM_RESOLVE_ITEM_RESULT_OK:
		{
			if ( m_pDLGResolveItemResult != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGResolveItemResult, NULL, false );
			}
			m_pDLGResolveItemResult = NULL;
			
			DeleteItem();

			return true;
		}
		break;
	case URICM_RESOLVE_ITEM_RESULT_PREV_PAGE:
		{
			ChangeResolveResultPage( false );
			return true;
		}
		break;
	case URICM_RESOLVE_ITEM_RESULT_NEXT_PAGE:
		{
			ChangeResolveResultPage( true );
			return true;
		}
		break;

	case URICM_RESOLVE_ITEM_CANCLE:
		{
			SetShow( false );

			return true;
		}
		break;

	case URICM_RESOLVE_ITEM_OK:
		{
			if(m_mapItemData.empty())
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_12154 ), g_pMain->GetNowState() );		

				break;
			}
			OpenMoru();
		}
		break;
	}

	return false;
}


bool CX2UIResolveItem::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{

	case EGS_RESOLVE_ITEM_ACK:
		{
			return Handler_EGS_RESOLVE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	}
	return false;
}

void CX2UIResolveItem::SetShow(bool bShow)
{
#ifdef SERV_SYNTHESIS_AVATAR
	if( g_pData->GetUIManager()->GetUISynthesisItem() != NULL && g_pData->GetUIManager()->GetUISynthesisItem()->GetShow() ==true )
	{
		return;
	}
#endif //SERV_SYNTHESIS_AVATAR

	m_bShow	= bShow;

	g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();

	if ( bShow )
	{
		//			g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_MENU_RESOLVE );
	}
	else
	{
		//			g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_MENU_RESOLVE );
		//			CX2ItemSlotManager::InvalidSelectedItem();
		//			CX2ItemSlotManager::InvalidSlotDesc();
		DeleteItem();
	}	

	m_pDLGResolveItem->SetShowEnable( bShow, bShow );

}

bool CX2UIResolveItem::Handler_EGS_RESOLVE_ITEM_REQ()
{
	KEGS_RESOLVE_ITEM_AVATAR_REQ kPacket;

	kPacket.m_mapResolveDataAvatar  = m_mapItemData;

	if(kPacket.m_mapResolveDataAvatar.empty())
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_12154 ), g_pMain->GetNowState() );
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_RESOLVE_ITEM_AVATAR_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RESOLVE_ITEM_ACK );

	DeleteItem();

	return true;
}

bool CX2UIResolveItem::Handler_EGS_RESOLVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESOLVE_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_RESOLVE_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			OpenResolveResultWindow( kEvent.m_mapInsertedItem, kEvent.m_vecKInventorySlotInfo, kEvent.m_bJackpot );

			//특수 처리 고고싱
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}

			return true;
		}		
	}

	m_bInvenCheck = false;

	return false;
}



bool CX2UIResolveItem::OnDropAnyItem( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
		return false;

	if ( pItemSlot == *m_pSlotBeforeDragging )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
		SetNowDragItemReturn();
		return true;
	}

	//{{ 2008.11.13 김태완 : UI 예외처리
	if(!(*m_pSlotBeforeDragging)->IsItem()) return false;

	CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
	CX2Item* pItem = NULL;
	pItem = kInventory.GetItem( *m_DraggingItemUID, true );

	if ( pItem == NULL )
	{
		ASSERT( !"GetItem Is NULL" );
		return false;
	}
	
	// 같은 아이템 분류 타입인지 보고..
	if ( m_NowInventorySortType != kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) ) return false;

	switch((*m_pSlotBeforeDragging)->GetSlotType())
	{
	case CX2Slot::ST_INVENTORY:
		{
			RegisterResolveItem(pItemSlot);
			CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) *m_pSlotBeforeDragging;
			pkSlotBeforeDragging->DestroyItemUI();
			return true;
		}
	default:
		break;
	}

	// 처리가 안 된 경우 
	return false;
}

bool CX2UIResolveItem::OnRClickedItem( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pItemSlot = NULL;
	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL )
		return false;

	const CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
	if ( pkItemTemplet == NULL )
		return false;

/*	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	if ( pInventory != NULL )
	{
		switch(g_pMain->GetNowStateID())
		{
		default:
			{
				for ( int i = 0; i < pInventory->GetItemMaxNum( CX2Inventory::ST_QUICK_SLOT ); i++ )
				{
					CX2Item* pItem = pInventory->GetItem( CX2Inventory::ST_QUICK_SLOT , i );
					if ( pItem == NULL )
					{
						g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);;
						g_pData->GetUIManager()->GetUIInventory()->SetInventorySort( CX2Inventory::ST_QUICK_SLOT );

						Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pItemSlot->GetSortType(), pItemSlot->GetSlotID(),
							CX2Inventory::ST_QUICK_SLOT , i );

						pItemSlot->DestroyItemUI();
						return true;
					}
				}

			} break;
		}

	}*/

	return false;
}

void CX2UIResolveItem::ChangeResolveResultPage( bool bNextPage )
{
	if ( m_pDLGResolveItemResult != NULL )
	{
		if ( bNextPage == false )
		{
			if ( m_DecompositionWindowNowPageNum <= 1 )
				return;

			m_DecompositionWindowNowPageNum--;
		}

		int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
		if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
		{
			maxPage += 1;
		}

		if ( bNextPage == true )
		{
			if ( m_DecompositionWindowNowPageNum >= maxPage )
				return;

			m_DecompositionWindowNowPageNum++;
		}

		wstringstream wstrstm;
		wstrstm << m_DecompositionWindowNowPageNum << L"/" << maxPage;
		CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"Static_Page_Number");
		if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
		{
			pStaticPageNum->GetString(0)->msg = wstrstm.str().c_str();
		}

		CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_Slot_Pic" );
		CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotName" );
		CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotNum" );

		std::map< int, int >::iterator i;
		int j = 0;
		i = m_mapResolveResultInsertedItem.begin();
		for ( int nextNum = 0; nextNum < (m_DecompositionWindowNowPageNum - 1) * RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; nextNum++ )
		{
			i++;
		}

		std::map< int, int >::iterator maxiter = m_mapResolveResultInsertedItem.begin();
		for ( int nextNum = 0; nextNum < ( RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM*m_DecompositionWindowNowPageNum ); nextNum++ )
		{
			maxiter++;
		}

		//일단 초기화 부터 시켜줘야할꺼 같다 후덜덜
		for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; slotNum++ )
		{
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
			{
				pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
			}

			if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
			{
				pStaticNum->GetString(slotNum)->msg = L"";
			}
		}

		for ( ; i != m_mapResolveResultInsertedItem.end() && i != maxiter ; i++, j++ )
		{
			int resultItemID = i->first;

			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
			if ( pItemTemplet != NULL )
			{
				if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
				{
					if ( pStaticSlot->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() ) == false )
					{
						pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
					}
				}

				if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
				{
					wstringstream wstrstm;
					wstrstm  << i->second << GET_STRING( STR_ID_24 );
					pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
				}
			}
		}
	}
}

std::wstring CX2UIResolveItem::GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem )
{
	std::wstring slotItemDesc = L"";

	if ( m_bShowRBDownMessage == true )
	{        
			slotItemDesc = GET_STRING( STR_ID_399 );
	}

	return slotItemDesc;
}

void CX2UIResolveItem::DrawSlotMouseOverImage()
{
	CX2ItemSlotManager::DrawSlotMouseOverImage();
}

/*
bool CX2UIResolveItem::MouseUp( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pSlot = (CX2SlotItem*)GetSlotInMousePos( mousePos );
	if(pSlot == NULL) 
		return false;

CX2State* pState = (CX2State*)g_pMain->GetNowState();
	if(pState != NULL)
	{
		CX2Cursor* pCursor = pState->GetCursor();
		if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
		{
			UidType ItemUID = pSlot->GetItemUID();
			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ItemUID );

			if(pItem == NULL )
				return false;


			g_pData->GetUIManager()->SetDLGFlag(false);
			switch(pCursor->GetCurorState())
			{
			case CX2Cursor::XCS_ATTACH_ATTRIB_ITEM:
				break;
			default:
				break;
			}
			return true;
		}
	}

	if ( *m_pSlotBeforeDragging != NULL )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*) *m_pSlotBeforeDragging;
		bool bCheck = false;

		bCheck = OnDropAnyItem(mousePos);		// 인벤토리 슬롯에 떨어져쪄요

		if ( *m_pSlotBeforeDragging != NULL && (*m_pSlotBeforeDragging)->GetDialog() != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), (*m_pSlotBeforeDragging)->GetNormalLayer() );
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
		}

		if(bCheck)
		{
			*m_pSlotBeforeDragging = NULL;
			*m_DraggingItemUID = -1;
			return true;
		}
	}

	return false;
}*/

wstring CX2UIResolveItem::GetSlotItemDesc()
{

	wstring itemDesc=  L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->IsResetItemUI() == false )
		{
			itemDesc = m_pNowOverItemSlot->GetSlotDesc();
		}
		else
		{	
			itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
		}
	}

	return itemDesc;
}


void CX2UIResolveItem::OpenResolveResultWindow( std::map< int, int >& mapInsertedItem, 
											 std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
											 bool bJackpot )
{

	SAFE_DELETE_DIALOG( m_pDLGResolveItemResult );

	m_DecompositionWindowNowPageNum = 1;
	m_mapResolveResultInsertedItem = mapInsertedItem;

	m_pDLGResolveItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Resolve_Result.lua" );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItemResult );

	m_pDLGResolveItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 446, 376 ), D3DXVECTOR2( 222, 288) ) );

	CKTDGUIButton* pControl = (CKTDGUIButton*) m_pDLGResolveItemResult->GetControl( L"Button_OK" );
	if ( pControl != NULL )
	{
		pControl->RequestFocus();
	}

	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_Slot_Pic" );
	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotName" );
	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotNum" );


	//일단 초기화 부터 시켜줘야할꺼 같다 후덜덜
	for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; slotNum++ )
	{
		if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
		{
			pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
		}

		if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
		{
			pStaticName->GetString(slotNum)->msg = L"";
		}

		if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
		{
			pStaticNum->GetString(slotNum)->msg = L"";
		}
	}

	std::map< int, int >::iterator i;
	int j = 0;
	for ( i = mapInsertedItem.begin(); i != mapInsertedItem.end(); i++, j++ )
	{
		int resultItemID = i->first;

		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
		if ( pItemTemplet != NULL )
		{
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
			{
				if ( pStaticSlot->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() ) == false )
				{
					pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
				}
			}

			if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
			{
				pStaticName->GetString(j)->msg = pItemTemplet->GetFullName_();
			}

			if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
			{
				wstringstream wstrstm;
				wstrstm  << i->second << GET_STRING( STR_ID_24 );
				pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
			}
		}
		// 아이템이 없는 경우
		else
		{
			pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			pStaticName->GetString(j)->msg = L"";
			pStaticNum->GetString(j)->msg = L"";
		}
	}

	int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
	if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
	{
		maxPage += 1;
	}

	wstringstream wstrstm;
	wstrstm << m_DecompositionWindowNowPageNum << L"/" << maxPage;
	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"Static_Page_Number");
	if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
	{
		pStaticPageNum->GetString(0)->msg = wstrstm.str().c_str();
	}

	UpdateInventorySlotList( vecKInventorySlotInfo );
}

bool CX2UIResolveItem::UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo)
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

void CX2UIResolveItem::RemoveEqip(UidType ItemUID)
{
	g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( ItemUID );
}

void CX2UIResolveItem::AddEqip(UidType ItemUID)
{
	g_pData->GetMyUser()->GetSelectUnit()->AddEqip( ItemUID );


}

bool CX2UIResolveItem::SetInventorySort()
{
	if(g_pData->GetUIManager()->GetUIInventory() == NULL)
	{
		g_pData->GetUIManager()->CreateUIInventory();
	}

	bool bResult = g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();

	return bResult;
}

void CX2UIResolveItem::RegisterResolveItem(CX2SlotItem* pItemSlot)
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
						GET_STRING( STR_ID_823 ), g_pMain->GetNowState() );

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
					GET_STRING( STR_ID_823 ), g_pMain->GetNowState() );

				return;
			}
		}


#else //ENABLE_RESOLVE_FASHION

		if ( m_NowInventorySortType != CX2Inventory::ST_EQUIP )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_STRING( STR_ID_823 ), g_pMain->GetNowState() );

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

	if( m_mapItemData.size() >= 6 )
		return;

	SetInventorySort();

	//m_pResolveSlot = pItemSlot;

//	std::multimap<CX2Inventory::SORT_TYPE, CX2SlotItem*>::iterator target;
//	for(target = m_mapResolveSlot.begin(); target != m_mapResolveSlot.end(); ++ target )
	std::vector< ResolveItemSlot >::iterator target;
	for(target = m_vecResolveSlot.begin(); target != m_vecResolveSlot.end(); ++ target )
	{
//		CX2SlotItem* pSlot = target->second;
	//	if ( pSlot == NULL )
		//	continue;

		if( target->m_iItemUID == pItemSlot->GetItemUID()  )
		{
			return;
		}
	}

	ResolveItemSlot sItemSlot;
	sItemSlot.m_SORT_TYPE = pItemSlot->GetSortType();
	sItemSlot.m_iItemUID = pItemSlot->GetItemUID();
	sItemSlot.m_pImteSlot = pItemSlot;

	m_vecResolveSlot.push_back(sItemSlot);
	//m_vecResolveSlot.insert( std::make_pair(pItemSlot->GetSortType(), pItemSlot));

	AttachSelectedPicture( pItemSlot );

	SetSlotItem(pItem);
}


void CX2UIResolveItem::SetSlotItem(CX2Item *pkItem)
{
	m_pItemData = pkItem;
	AttachItem();
}

bool CX2UIResolveItem::AttachItem()
{
		// 미리보기 아이템 셋팅

		CX2SlotItem* pToSlotItem	= NULL;

		for ( int i = 0; i < 6 ; i++ )
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

				// 설정
				if(	m_pItemData->GetItemTemplet()->GetFashion() == true )	
				{
					if( m_pItemData->GetItemTemplet()->GetItemType() == CX2Item::IT_WEAPON  ||
						m_pItemData->GetItemTemplet()->GetItemType() == CX2Item::IT_DEFENCE  )
					{
						if(m_pItemData->GetItemTemplet()->GetItemGrade() == CX2Item::IG_RARE)
						{
							m_iTotalCostED += 15000;
							// 레어 20% 15000ED
						}
						else if(m_pItemData->GetItemTemplet()->GetItemGrade() == CX2Item::IG_NORMAL)
						{
							if(m_pItemData->GetItemTemplet()->GetSetID() ==0)
							{
								m_iTotalCostED += 5000;
								// 일반	5% 5000ED
							}
							else
							{
								m_iTotalCostED += 10000;
								// 한정	10 10000ED
							}
						}
					}
				}
				//

				if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED < m_iTotalCostED)
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
						GET_STRING( STR_ID_5072 ), g_pMain->GetNowState() );
			
					return false;
				}

				if ( m_pItemData != NULL )
				{
					pToSlotItem->CreateItemUI( m_pItemData, 1 );
					m_mapItemData.insert( std::make_pair(i, pToSlotItem->GetItemUID()));

					CKTDGUIStatic* pStatic_ED = static_cast< CKTDGUIStatic* >(m_pDLGResolveItem->GetControl( L"g_pStatic_ED" ));
					WCHAR buff[256] = {0};
					_itow( m_iTotalCostED, buff, 10 );
					pStatic_ED->GetString(0)->msg = buff;
				}

				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );    

				return true;
			}
		}

	return false;

}



bool CX2UIResolveItem::DeleteItem()
{
	m_iTotalCostED = 0;

	CKTDGUIStatic* pStatic_ED = static_cast< CKTDGUIStatic* >(m_pDLGResolveItem->GetControl( L"g_pStatic_ED" ));
	WCHAR buff[256] = {0};
	pStatic_ED->GetString(0)->msg = buff;

	CX2SlotItem* pToSlotItem	= NULL;

	for ( int i = 0; i < 6 ; i++ )
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

	std::vector< ResolveItemSlot >::iterator target;
	for(target = m_vecResolveSlot.begin(); target != m_vecResolveSlot.end(); ++ target )
	//std::multimap<CX2Inventory::SORT_TYPE, CX2SlotItem*>::iterator target;
//	for(target = m_mapResolveSlot.begin(); target != m_mapResolveSlot.end(); ++ target )
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
	m_mapResolveResultInsertedItem.clear();
	m_vecResolveSlot.clear();

	return true;
}

void CX2UIResolveItem::OpenMoru()
{
	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, false);
//	g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_MENU_RESOLVE );
//	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_RESOLVE, false);
//	SetShow(false);

	m_pDLGOpenMoru = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Item_Reslove_Effect.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOpenMoru );

	// 중간에 모델 그려주자
	D3DXVECTOR3 boxPos = m_pDLGOpenMoru->GetDummyPos( 0 );
	CKTDGXMeshPlayer::CXMeshInstance* pMeshInstMoru = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"Moru", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
	g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInstMoru);

	if( NULL != m_pDLGOpenMoru )
	{
		m_pDLGOpenMoru->SetHasUnit( pMeshInstMoru );
	}

	m_hMeshInstMoru = pMeshInstMoru->GetHandle();

	m_bPlayResolveItem = true;
}

void CX2UIResolveItem::UpdateOpenResolveItemAck()
{
	SAFE_DELETE_DIALOG( m_pDLGOpenMoru );
	m_pDLGOpenMoru = NULL;

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	g_pData->GetUIManager()->GetUIInventory()->SetItemObtainResultType(CX2UIInventory::IORT_ITEM_RESOLVE);
#endif // RANDOM_EXCHANGE_RESULT_VIEW

	Handler_EGS_RESOLVE_ITEM_REQ();
}

bool CX2UIResolveItem::MouseRButtonUp( D3DXVECTOR2 mousePos )
{    
	*m_pSlotBeforeDragging = NULL;
	*m_DraggingItemUID = -1;
	m_DraggingItemTID = -1;

	CX2SlotItem* pItemSlot = NULL;
	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL )
		return false;

	if( m_bPlayResolveItem == true )
		return false;

	CX2Item* m_pItemData = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );

	if( m_pItemData == NULL)
		return false;

	// 설정
	if(	m_pItemData->GetItemTemplet()->GetFashion() == true )	
	{
		if( m_pItemData->GetItemTemplet()->GetItemType() == CX2Item::IT_WEAPON  ||
			m_pItemData->GetItemTemplet()->GetItemType() == CX2Item::IT_DEFENCE  )
		{
			if(m_pItemData->GetItemTemplet()->GetItemGrade() == CX2Item::IG_RARE)
			{
				m_iTotalCostED -= 15000;
				// 레어 20% 15000ED
			}
			else if(m_pItemData->GetItemTemplet()->GetItemGrade() == CX2Item::IG_NORMAL)
			{
				if(m_pItemData->GetItemTemplet()->GetSetID()==0)
				{
					m_iTotalCostED -= 5000;
					// 일반	5% 5000ED
				}
				else
				{
					m_iTotalCostED -= 10000;
					// 한정	10 10000ED
				}
			}
		}
	}
	//

	if ( m_pItemData != NULL )
	{
		CKTDGUIStatic* pStatic_ED = static_cast< CKTDGUIStatic* >(m_pDLGResolveItem->GetControl( L"g_pStatic_ED" ));
		WCHAR buff[256] = {0};
		_itow( m_iTotalCostED, buff, 10 );
		pStatic_ED->GetString(0)->msg = buff;
	}

	std::map<int, UidType>::iterator target = m_mapItemData.find(pItemSlot->GetSlotID());
	if(target != m_mapItemData.end())
	{
		m_mapItemData.erase(pItemSlot->GetSlotID());
	}

	std::vector< ResolveItemSlot >::iterator target2;
	for(target2= m_vecResolveSlot.begin(); target2 != m_vecResolveSlot.end(); )
//	std::multimap<CX2Inventory::SORT_TYPE, CX2SlotItem*>::iterator it;
	//for(it = m_mapResolveSlot.begin(); it != m_mapResolveSlot.end(); ++it )
	{
		CX2SlotItem* pSlot = target2->m_pImteSlot;
		if ( pSlot == NULL )
			continue;

		if( target2->m_iItemUID == pItemSlot->GetItemUID()  )
		{
			SetShowSelectedPicture(pSlot, false);

			m_vecResolveSlot.erase( target2 );
		}
		else
		{
			 ++ target2;
		}
	}

	pItemSlot->DestroyItemUI();  

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );  

	return true;

}

void CX2UIResolveItem::AttachSelectedPicture( CX2SlotItem* pItemSlot )
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

}

void CX2UIResolveItem::SetShowSelectedPicture( CX2SlotItem* pItemSlot, bool bShow )
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

	pPictureEquiped->SetShow( bShow );
}


bool CX2UIResolveItem::CheckResolveUseItem(UidType Uid)
{

	std::vector< ResolveItemSlot >::iterator target;
	for(target = m_vecResolveSlot.begin(); target != m_vecResolveSlot.end(); ++ target )
	{
		//CX2SlotItem* pSlot = target->second;
		//if ( pSlot == NULL )
//			continue;

		if( target->m_iItemUID == Uid  )
		{
			return true;
		}
	}

	return false;
}

void CX2UIResolveItem::ShowResolveCheckPicture( CX2Inventory::SORT_TYPE sortType )
{

	std::vector< ResolveItemSlot >::iterator target;
	for(target = m_vecResolveSlot.begin(); target != m_vecResolveSlot.end(); ++ target )
	{
		CX2SlotItem* pSlot = target->m_pImteSlot;
		if ( pSlot == NULL )
			continue;

		if( target->m_SORT_TYPE == sortType )	
			AttachSelectedPicture( pSlot );
	}
}

void CX2UIResolveItem::OffResolveCheckPicture()
{
	std::vector< ResolveItemSlot >::iterator target;
	for(target = m_vecResolveSlot.begin(); target != m_vecResolveSlot.end(); ++ target )
	{
		CX2SlotItem* pSlot = target->m_pImteSlot;
		if ( pSlot == NULL )
			continue;

		SetShowSelectedPicture(pSlot, false);
	}
}


#endif