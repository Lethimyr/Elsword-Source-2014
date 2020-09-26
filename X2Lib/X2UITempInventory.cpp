#include "StdAfx.h"

#ifdef	REAL_TIME_ELSWORD

#include ".\X2UITempInventory.h"

// 생성자, 소멸자 선언 부분
CX2UITempInventory::CX2UITempInventory( const CKTDXStage* const pNowStage, const WCHAR* const pFileName )
: CX2ItemSlotManager( const_cast< CKTDXStage* >( pNowStage ), NULL ),
m_iNowItemIndex( 0 ),
m_iNumberOfItem( 0 ),
m_bInventoryShow( false ),
m_bShow( false ),
m_vSizeDlg( 0, 0 ),
m_vPosDlg( 0, 0 ),
m_pDlgTempInventory( NULL )
{
	CreateTempInventory( pNowStage, pFileName );
}

CX2UITempInventory::~CX2UITempInventory()
{
	if ( GetShow() == true )	// 임시 인벤토리가 켜져 있는 상태면
	{
		SetShow( false );
	}

	DesotryTempInventory();
	m_SlotList.clear();
	SAFE_DELETE_DIALOG( m_pDlgTempInventory );
}

// 멤버 함수
HRESULT CX2UITempInventory::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();


	if ( IsInventoryChangedShowState() == true )
	{
		D3DXVECTOR2 vec( 0, 0 );
		vec.x += g_pData->GetUIManager()->GetUIInventory()->GetDLGSize().x;
#ifdef SERV_PET_SYSTEM
		// oasis907 : 김상윤 [2010.9.13] // 펫 인벤토리 추가로 임시 인벤토리 y 위치 변경
		vec.y = 178.f;
#endif SERV_PET_SYSTEM

		if ( m_bInventoryShow == true )		// 인벤토리가 열려있는 상태면
		{
			vec *= -1;
		}

		MovePosition( vec );
	}

	if( m_pDlgTempInventory != NULL && m_pDlgTempInventory->GetIsMouseOver() == true )
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

bool CX2UITempInventory::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgTempInventory ) )
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
		{
			if ( m_pDlgTempInventory->GetIsMouseOver() == false )
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
			if ( m_pDlgTempInventory->GetIsMouseOver() == false )
			{
				return false;
			}

			g_pData->GetUIManager()->SetUILayerIndex( CX2UIManager::UI_TEMP_INVENTORY, true );
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
			POINT mousePoint = { static_cast< short >( LOWORD( lParam ) ), static_cast< short >( HIWORD( lParam ) ) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( static_cast< float >( mousePoint.x ), 
				static_cast< float >( mousePoint.y ) );
			
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
		}
		break;

	}

    return bFlag;
}

bool CX2UITempInventory::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case UTICM_GET_ITEM:
		{
			Hander_EGS_GET_TEMP_ITEM_REQ();
			return true;
		}
		break;

	case UTICM_EXIT:			// EXIT와 DELETE는 같은 동작을 하도록 처리
	case UTICM_DELETE_ITEM:
		{
			Hander_EGS_DELETE_TEMP_ITEM_REQ();
			return true;
		}
		break;

	case UTICM_PREV_ITEM:
		{
			MovePrevItem();
			UpdateSlotList();
			UpdatePageUI();

			return true;
		}
		break;

	case UTICM_NEXT_ITEM:
		{
			MoveNextItem();
			UpdateSlotList();
			UpdatePageUI();

			return true;
		}
		break;
	}

	return false;
}

void CX2UITempInventory::SetShow( bool val )
{
	m_bShow = val;

	if ( val == true )
	{
		g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_TEMP_INVENTORY );

		if ( m_pDlgTempInventory == NULL )
		{		
			CreateTempInventory( g_pMain->GetNowState(), L"DLG_Item_Box.lua" );
		}

		UpdateSlotList();
		UpdatePageUI();
		DisplayTitle();
	}
	else
	{
		g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_TEMP_INVENTORY );

		CX2ItemSlotManager::InvalidSelectedItem();
		CX2ItemSlotManager::InvalidSlotDesc();
		LostTempInventory();
	}

	m_pDlgTempInventory->SetShowEnable( m_bShow, m_bShow );

}

void CX2UITempInventory::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence( false );
	if( m_pDlgTempInventory != NULL ) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgTempInventory, layer );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgTempInventory, false );
	}
}

void CX2UITempInventory::AddTempItem( const KTempInventoryItemInfo& kTempInventoryItemInfo )
{
	for ( UINT i = 0; i < m_vecTempInventoryInfo.size(); i++ )
	{
		// 수량성 아이템일 경우에만 UID가 같을 수 있음(비 수량성인 경우는 무조건 UID가 다름)
		if ( kTempInventoryItemInfo.m_iTempItemUID == m_vecTempInventoryInfo[i].m_iTempItemUID )
		{
			m_vecTempInventoryInfo[i].m_iQuantity = kTempInventoryItemInfo.m_iQuantity;
			UpdateSlotList();
			
			return;
		}
	}

	m_vecTempInventoryInfo.push_back( kTempInventoryItemInfo );
	m_iNumberOfItem = m_vecTempInventoryInfo.size();

	UpdatePageUI();
}

void CX2UITempInventory::AddItemToInventory( const std::vector< KInventoryItemInfo >& vecKInventoryItem )
{
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInventoryItemInfo, vecKInventoryItem )
	{
		CX2Item::ItemData kItemData( kInventoryItemInfo );
		
		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().AddItem( 
			static_cast< CX2Inventory::SORT_TYPE >( kInventoryItemInfo.m_cSlotCategory ), 
			kInventoryItemInfo.m_sSlotID, kItemData );
	}

	// 켜져있는지 확인 후 업데이트 
	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
	}


	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}

}

bool CX2UITempInventory::RemoveTempItem( int iItemUID )
{
	for ( UINT i = 0; i < m_vecTempInventoryInfo.size(); i++ )
	{
		if ( m_vecTempInventoryInfo[i].m_iTempItemUID == iItemUID )
		{
			m_vecTempInventoryInfo.erase( m_vecTempInventoryInfo.begin() + i );

			m_iNumberOfItem--;

			if ( m_iNumberOfItem == 0 )	// 더이상 아이템이 없으면
			{
				SetShow( false );
			}
			else
			{
				if ( m_iNowItemIndex >= m_iNumberOfItem )
					m_iNowItemIndex = m_iNumberOfItem - 1;

				UpdateSlotList();
				UpdatePageUI();
			}

			return true;
		}
	}

	return false;
}

void CX2UITempInventory::CreateTempInventory(  const CKTDXStage* const pNowStage, const WCHAR* const pFileName )
{
	RegisterLuaBind();

	m_pDlgTempInventory = new CKTDGUIDialog( const_cast< CKTDXStage* >( pNowStage ), pFileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgTempInventory );
	
	m_pDlgTempInventory->SetDisableUnderWindow( true );
	
	m_vPosDlg.x = m_pDlgTempInventory->GetDummyPos( 0 ).x;	
	m_vPosDlg.y = m_pDlgTempInventory->GetDummyPos( 0 ).y;

	m_vSizeDlg.x = m_pDlgTempInventory->GetDummyPos( 1 ).x;
	m_vSizeDlg.y = m_pDlgTempInventory->GetDummyPos( 1 ).y;

	if ( IsInventoryChangedShowState() == true )
	{
		D3DXVECTOR2 vec( 0, 0 );
		vec.x += g_pData->GetUIManager()->GetUIInventory()->GetDLGSize().x;

		if ( m_bInventoryShow == true )		// 인벤토리가 열려있는 상태면
		{
			vec *= -1;						// 임시인벤토리를 왼쪽으로 이동
		}

		MovePosition( vec );
	}
}

void CX2UITempInventory::ResetTempInventoryInfo()
{
	m_vecTempInventoryInfo.resize( 0 );

	m_iNowItemIndex		= 0;
	m_iNumberOfItem		= 0;
}

void CX2UITempInventory::DesotryTempInventory()
{
	m_vecTempInventoryInfo.clear();
	m_iNowItemIndex		= 0;
	m_iNumberOfItem		= 0;

	for( UINT i = 0; i < m_SlotList.size(); i++ )
	{
		CX2SlotItem* pkSlotItem = static_cast< CX2SlotItem* >( m_SlotList[i] );
		if ( pkSlotItem != NULL )
		{
			pkSlotItem->DestroyItemUI();
			pkSlotItem->SetShow( false );
			pkSlotItem->SetEnable( false, false );
			SAFE_DELETE(pkSlotItem);
		}
	}
}

void CX2UITempInventory::LostTempInventory()
{
	for( UINT i = 0; i < m_SlotList.size(); i++ )
	{
		CX2SlotItem* pkSlotItem = static_cast< CX2SlotItem* >( m_SlotList[i] );
		if ( pkSlotItem != NULL )
		{
			pkSlotItem->LostItemUI();
			pkSlotItem->SetShow( false );
			pkSlotItem->SetEnable( false, false );
		}
	}
}


bool CX2UITempInventory::IsInventoryChangedShowState()
{
	if ( g_pData->GetUIManager()->GetUIInventory() == NULL )
	{
		if ( m_bInventoryShow == true )		// 켜져있는 상태였다가 NULL 이 됬으면
			return true;					// 변경 된것 임으로
		else
			return false;
	}

	if ( m_bInventoryShow != g_pData->GetUIManager()->GetUIInventory()->GetShow() ) // 이전의 상태와 다르면
	{
		m_bInventoryShow = g_pData->GetUIManager()->GetUIInventory()->GetShow();

		return true;
	}
	else	// 이전의 상태와 같으면
	{
		return false;
	}
	
}

bool CX2UITempInventory::UpdateSlotList()
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = static_cast< CX2SlotItem* >( GetSlot( i ) );
		if ( pSlotItem == NULL )
			continue;

		if ( m_iNowItemIndex >= m_iNumberOfItem )
		{
			ASSERT( false );
			return false;
		}

		int itemID			= m_vecTempInventoryInfo[m_iNowItemIndex].m_iItemID;
		int itemQuantity	= m_vecTempInventoryInfo[m_iNowItemIndex].m_iQuantity;


		if ( pSlotItem->GetEnable() == false )
		{
			pSlotItem->SetEnable( true );
			pSlotItem->SetShow( true );
		}

		if ( pSlotItem->IsCreatedItemUI() == true )
			pSlotItem->ResetItemUI( itemID, itemQuantity  );
		else
			pSlotItem->CreateItemUI( itemID, itemQuantity );

		break;
	}

	return true;
}

void CX2UITempInventory::UpdatePageUI() const
{
	if ( m_pDlgTempInventory != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)( m_pDlgTempInventory->GetControl( L"Item_Box_Page_Num" ) );
		
		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << m_iNowItemIndex + 1 // index는 0 부터 이므로 + 1을 해줌
				<< L"/" << m_iNumberOfItem;
			pStatic->SetString( 0, wstrstm.str().c_str() );
		}
	}
}

void CX2UITempInventory::DisplayTitle() const
{
	if ( m_pDlgTempInventory != NULL )
	{
		CKTDGUIStatic* pStaticTitle2 = (CKTDGUIStatic*)( m_pDlgTempInventory->GetControl( L"g_pStaticTitle2" ) );
		CKTDGUIStatic* pStaticTitle1 = (CKTDGUIStatic*)( m_pDlgTempInventory->GetControl( L"g_pStaticTitle1" ) );

		wstringstream wstrstm;
		wstrstm << GET_STRING( STR_ID_2681 );

		if ( pStaticTitle2 != NULL )
		{
			pStaticTitle2->SetString( 0, wstrstm.str().c_str() );
		}

		if ( pStaticTitle1 != NULL )
		{
			pStaticTitle1->SetString( 0, wstrstm.str().c_str() );
		}
		
	}
}

void CX2UITempInventory::MovePrevItem()
{
	m_iNowItemIndex--;

	if ( m_iNowItemIndex < 0 )
		m_iNowItemIndex = 0;
}

void CX2UITempInventory::MoveNextItem()
{
	m_iNowItemIndex++;

	if ( m_iNowItemIndex >= m_iNumberOfItem )
		m_iNowItemIndex = m_iNumberOfItem - 1;
}

void CX2UITempInventory::MovePosition( D3DXVECTOR2 vec )
{
	m_pDlgTempInventory->SetPos( m_pDlgTempInventory->GetPos() + vec );

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem*	pItemSlot	= static_cast< CX2SlotItem* >( m_SlotList[i] );
		D3DXVECTOR2		pos			= pItemSlot->GetPos();

		pos += vec;

		pItemSlot->SetPos(pos);
	}
}

bool CX2UITempInventory::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_GET_TEMP_ITEM_ACK:
		{
			return Hander_EGS_GET_TEMP_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_DELETE_TEMP_ITEM_ACK:
		{
			return Hander_EGS_DELETE_TEMP_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
 ///								핸들러
//////////////////////////////////////////////////////////////////////////
bool CX2UITempInventory::Hander_EGS_GET_TEMP_ITEM_REQ()
{
	KEGS_GET_TEMP_ITEM_REQ kPacket;

	kPacket.m_iTempItemUID = m_vecTempInventoryInfo[m_iNowItemIndex].m_iTempItemUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_TEMP_ITEM_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_GET_TEMP_ITEM_ACK, 60.f );

	return true;
}

bool CX2UITempInventory::Hander_EGS_DELETE_TEMP_ITEM_REQ()
{
	KEGS_DELETE_TEMP_ITEM_REQ kPacket;
	
	kPacket.m_iTempItemUID = m_vecTempInventoryInfo[m_iNowItemIndex].m_iTempItemUID;
	
	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_TEMP_ITEM_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_DELETE_TEMP_ITEM_ACK, 60.f ); 

	return true;
}

bool CX2UITempInventory::Hander_EGS_GET_TEMP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_TEMP_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_TEMP_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			AddItemToInventory( kEvent.m_vecInventorySlotInfo );
			RemoveTempItem( kEvent.m_iTempItemUID );

			BOOST_TEST_FOREACH( const KInventoryItemInfo&, itemInfo, kEvent.m_vecInventorySlotInfo  )
			{
				if( itemInfo.m_iItemUID > 0 && 
					g_pData->GetQuestManager() != NULL )
				{
					g_pData->GetQuestManager()->CheckItemCollection( itemInfo.m_kItemInfo.m_iItemID );
				}
			}
#ifdef SERV_EPIC_QUEST
			if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST_NEW ) == true )
			{
				g_pData->GetUIManager()->GetUIQuestNew()->ResetQuestUI();
			}

			if ( g_pData->GetUIManager() == NULL ||
				g_pData->GetUIManager()->GetUIQuestNew() == NULL )
				return g_pMain->IsValidPacket( kEvent.m_iOK );

			g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
#else
			if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST ) == true )
			{
				g_pData->GetUIManager()->GetUIQuest()->UpdateQuestUI();
			}

			if ( g_pData->GetUIManager() == NULL ||
				g_pData->GetUIManager()->GetUIQuest() == NULL )
				return g_pMain->IsValidPacket( kEvent.m_iOK );

			if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST ) == true )
			{
				g_pData->GetUIManager()->GetUIQuest()->UpdateQuestUI();
			}

			if ( g_pData->GetUIManager()->GetUIQuest()->GetShowQuickQuestDLG() == true )
			{
				g_pData->GetUIManager()->GetUIQuest()->UpdateQuickQuestDLG();
			}
#endif SERV_EPIC_QUEST
		}
		else
		{
			//{{ kimhc // 2009-11-20
#ifdef	TEMP_INVENTORY_TEST
			// 켜져있는지 확인 후 업데이트 
			if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == false )
			{
				g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_INVEN );
			}

            const CX2Item::ItemTemplet* pItemTemplet	= 
                g_pData->GetItemManager()->GetItemTemplet( kEvent.m_iItemID );
			CX2Inventory::SORT_TYPE eSortType	= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetSortTypeByItemTemplet( pItemTemplet );
			g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( eSortType );

			if( g_pChatBox != NULL )
			{
				wstring wstrColor = L"#CFFFF00";				
				D3DXCOLOR coTextColor(1.f, 1.f, 0.f, 1.f);	
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_2670 ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
			}
#else	TEMP_INVENTORY_TEST
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2670 ), g_pMain->GetNowState() );
#endif	TEMP_INVENTORY_TEST
			//}} kimhc // 2009-11-20
			
		}

		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}
	
	return false;
}

bool CX2UITempInventory::Hander_EGS_DELETE_TEMP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_TEMP_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_DELETE_TEMP_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			RemoveTempItem( kEvent.m_iTempItemUID );
		}
		
		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}

	return false;
}
#endif	REAL_TIME_ELSWORD