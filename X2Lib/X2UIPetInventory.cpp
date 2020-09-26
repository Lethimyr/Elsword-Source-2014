#include "StdAfx.h"

#ifdef SERV_PET_SYSTEM
//{{ oasis907 : 김상윤 [2010.8.31] // 펫 인벤토리
#include ".\X2UIPetInventory.h"

CX2UIPetInventory::CX2UIPetInventory( const CKTDXStage* const pNowStage, int iLayer )
: CX2UIDragable(  const_cast< CKTDXStage* >( pNowStage ), NULL ),
m_pDlgPetInventory( NULL ),
m_bShow( false ),
NowPositionState(NORMAL),
m_vDlgSize( 0, 0 )
{
	InitUIPetInventory(iLayer);
}

CX2UIPetInventory::~CX2UIPetInventory()
{
	DestoryPetInventory();
}

bool CX2UIPetInventory::InitUIPetInventory(int iLayer)
{
	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
	m_pTalkBoxManager->SetOverUI( true );

	const CX2Inventory* pInventory	= NULL;
	pInventory	= GetMyInventory();

	if ( pInventory == NULL )
		return false;					// ASSERT 처리는 위의 GetInventory에서 해줌

	RegisterLuaBind();

	int MyPetID = g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_PetId;
	int EvolutionStep = g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_Evolution_Step;
	
	int Invenslot_Width_Size = g_pData->GetPetManager()->GetPetInventorySlotWidthSize((CX2PetManager::PET_UNIT_ID) MyPetID, EvolutionStep);
	int Invenslot_Height_Size = g_pData->GetPetManager()->GetPetInventorySlotHeightSize((CX2PetManager::PET_UNIT_ID) MyPetID, EvolutionStep);

	SetDummyValue( static_cast<int>( Invenslot_Height_Size ) );



	SAFE_DELETE_DIALOG(m_pDlgPetInventory);

	switch(Invenslot_Width_Size)
	{
	case 1:
		m_pDlgPetInventory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_Inventory1x4.lua" );
		break;
	case 2:
		m_pDlgPetInventory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_Inventory2x4.lua" );
		break;
	case 3:
		m_pDlgPetInventory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_Inventory3x4.lua" );
		break;
	case 4:
		m_pDlgPetInventory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_Inventory4x4.lua" );
		break;
	default:
		m_pDlgPetInventory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_Inventory4x4.lua" );
		break;
	}
	
	CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDlgPetInventory->GetControl(L"g_pCheckBoxAutoFeed");
	if ( pCheckBox != NULL )
	{
		bool bAutoFeed = g_pData->GetUIManager()->GetUIInventory()->GetAutoFeed();
		pCheckBox->SetChecked(bAutoFeed);
	}
	
	if ( m_pDlgPetInventory == NULL )
	{
		ASSERT( !"DlgPetInventory is NULL" );
		return false;
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgPetInventory );
	m_pDlgPetInventory->SetDisableUnderWindow( true );

	SetLayer(iLayer + 1);

	return true;
}

void CX2UIPetInventory::DestoryPetInventory()
{
	SAFE_DELETE_DIALOG( m_pDlgPetInventory );
	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );
}

HRESULT CX2UIPetInventory::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();


	MoveDlgPosition();




	if ( m_pDlgPetInventory != NULL && m_pDlgPetInventory->GetIsMouseOver() == true )
	{
		return CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
	}
	else
	{
		CX2ItemSlotManager::InvalidSelectedItem();
		CX2ItemSlotManager::InvalidSlotDesc();
	}
	
	return	S_OK;
}

bool CX2UIPetInventory::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgPetInventory ) )
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
			if ( false == m_pDlgPetInventory->GetIsMouseOver() )
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
			if ( false == m_pDlgPetInventory->GetIsMouseOver() )
			{
				return false;
			}
			bFlag = true;			
		}
		break;
	default:
		break;
	}

	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseUp( mousePos );

		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			if ( MouseDown( mousePos ) == true )
				return true;

		}
		break;

	case WM_MOUSEMOVE:
		{

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
		}
		break;

	case WM_RBUTTONDOWN:
		{
			SetNowDragItemReturn();
		}
		break;

	case WM_RBUTTONUP:
		{
			SetNowDragItemReturn();

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			MouseRButtonUp( mousePos );

		}
		break;
	}

	return bFlag;

}

bool CX2UIPetInventory::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case PIUCM_EXIT:
		{
			SetShow( false );
			return true;
		}
		break;


	case PIUCM_CHECK_AUTO_FEED:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDlgPetInventory->GetControl(L"g_pCheckBoxAutoFeed");
			if ( pCheckBox != NULL )
			{
				Handler_EGS_SET_AUTO_FEED_PETS_REQ(pCheckBox->GetChecked());
			}
		}
		break;


	case PIUCM_AUTO_FEED_MOUSE_OVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f;
			// oasis907 : 김상윤 [2010.9.16] // 스트링 DB에 넣으면 자꾸 끝 글자 잘립니다 ㅠㅜ
#ifdef CLIENT_GLOBAL_COMMON
			ShowUIDesc( true, GET_STRING( STR_ID_5613 ), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT);
#else CLIENT_GLOBAL_COMMON
			ShowUIDesc( true, L"포만도가 80%이하가 되면 아래 순서대로 섭취합니다.\n1.저가 장비\n2.고가 장비\n3.엘의 나무 씨앗\n4.엘의 나무 열매", pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT);
#endif CLIENT_GLOBAL_COMMON			
			//ShowUIDesc( true, GET_STRING( STR_ID_5613 ), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT);
		}
		return true;
	case PIUCM_AUTO_FEED_MOUSE_OUT:
		{
			ShowUIDesc(false);
		}
		return true;
	}

	return false;
}

bool		CX2UIPetInventory::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case EGS_SET_AUTO_FEED_PETS_ACK:
		{
			return Handler_EGS_SET_AUTO_FEED_PETS_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	}
	return false;
}


void CX2UIPetInventory::SetShow( bool bShow )
{
	m_bShow	= bShow;

	if ( bShow )
	{
		UpdateUI();

	}
	else
	{
		LostItemSlotList();
		CX2ItemSlotManager::InvalidSelectedItem();
		CX2ItemSlotManager::InvalidSlotDesc();

		ShowUIDesc(false);

	}

	m_pDlgPetInventory->SetShowEnable( bShow, bShow );
}

void CX2UIPetInventory::SetLayer( int layer )
{
	SetSlotManagerLayer( layer );
	SetSlotManagerChangeSequence( false );
	if( m_pDlgPetInventory != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgPetInventory, layer );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgPetInventory, false );
	}

}

void CX2UIPetInventory::MoveDlgPosition()
{
	POSITION_STATE NextPositionState = NowPositionState;

	if(g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetShow(CX2UIManager::UI_TEMP_INVENTORY) == true)
	{
		NextPositionState = UITEMPINVENTORY;
	}
	else if(g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIInventory() != NULL &&
		g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL &&
		g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->GetShow() == true)
	{
		NextPositionState = UISOCKET;
	}
	else
	{
		NextPositionState = NORMAL;
	}

	// oasis907 : 김상윤 [2010.9.13] //  위치 변경
	if(NextPositionState != NowPositionState)
	{
		const float MagicDlgUISocketSizeX = 297.f;
		const float MagicDlgUITempInvenSizeX = 132.f;

		// oasis907 : 김상윤 [2010.9.13] // 기본 위치가 아닐경우 원위치로
		if(NowPositionState == UISOCKET)
			MovePosition( D3DXVECTOR2(MagicDlgUISocketSizeX, 0) );
		else if(NowPositionState == UITEMPINVENTORY)
			MovePosition( D3DXVECTOR2(MagicDlgUITempInvenSizeX, 0) );

		// oasis907 : 김상윤 [2010.9.13] // 위치 이동
		if(NextPositionState == UISOCKET)
			MovePosition( D3DXVECTOR2(-MagicDlgUISocketSizeX, 0) );
		else if(NextPositionState == UITEMPINVENTORY)
			MovePosition( D3DXVECTOR2(-MagicDlgUITempInvenSizeX, 0) );

		// oasis907 : 김상윤 [2010.9.13] // 상태 저장
		NowPositionState = NextPositionState;
	}
	return;
}

void CX2UIPetInventory::MovePosition( D3DXVECTOR2 vec )
{
	m_pDlgPetInventory->SetPos( m_pDlgPetInventory->GetPos() + vec );

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem*	pItemSlot	= static_cast< CX2SlotItem* >( m_SlotList[i] );
		D3DXVECTOR2		pos			= pItemSlot->GetPos();

		pos += vec;

		pItemSlot->SetPos(pos);
	}
}

const CX2Inventory*	CX2UIPetInventory::GetMyInventory() const
{
	if ( g_pData->GetMyUser() == NULL )
	{
		ASSERT( !"User Is NULL");
		return NULL;
	}

	if ( g_pData->GetMyUser()->GetSelectUnit() == NULL )
	{
		ASSERT( !"SelectUnit Is NULL");
		return NULL;
	}

	return &g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
}

CX2Inventory*	CX2UIPetInventory::AccessMyInventory()
{
	if ( g_pData->GetMyUser() == NULL )
	{
		ASSERT( !"User Is NULL");
		return NULL;
	}

	if ( g_pData->GetMyUser()->GetSelectUnit() == NULL )
	{
		ASSERT( !"SelectUnit Is NULL");
		return NULL;
	}

	return &g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
}


void	CX2UIPetInventory::UpdateSlotList()
{
	CX2Item*		pItem			=	NULL;
	CX2SlotItem*	pSlotItem		=	NULL;
	const CX2Inventory*	pInventory		=	NULL;

	pInventory		=	GetMyInventory();

	if ( pInventory == NULL )
	{
		ASSERT( !"Inventory Is NULL " );
		return;
	}
	
	for ( UINT slotID = 0; slotID < m_SlotList.size(); slotID++ )
	{
		pSlotItem	=	static_cast< CX2SlotItem * >( GetSlot( CX2Slot::ST_PET, slotID ) );
		if ( pSlotItem == NULL )
			continue;

		pItem	= pInventory->GetItem( CX2Inventory::ST_PET, slotID );

		if ( pItem != NULL )
		{
			if ( pSlotItem->IsCreatedItemUI() == true )
				pSlotItem->ResetItemUI( pItem );
			else
				pSlotItem->CreateItemUI( pItem );
		}
		else
		{
			pSlotItem->DestroyItemUI();
		}
		
		pSlotItem->SetEnable( true );
		pSlotItem->SetShow( true );

		pItem		= NULL;
		pSlotItem	= NULL;		
	}
}

void	CX2UIPetInventory::UpdateUI()
{
	if(m_bShow == false)
		return;

	UpdateSlotList();
}

void	CX2UIPetInventory::LostItemSlotList( bool bShow /* = false */, bool bEnable /* = false */ )
{
	for ( UINT i = 0; i < m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = static_cast< CX2SlotItem* >( m_SlotList[i] );
		if ( pSlotItem != NULL )
		{
			pSlotItem->LostItemUI();
			pSlotItem->SetShow( bShow );
			pSlotItem->SetEnable( bEnable );
		}
	}
}

bool	CX2UIPetInventory::MouseDown( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pSlotItem	= NULL;
	pSlotItem				= static_cast< CX2SlotItem* >( GetSlotInMousePos( mousePos ) );

	if ( pSlotItem == NULL )	// 마우스가 슬롯위에 있는지 검사
		return false;

	if ( pSlotItem->IsResetItemUI() == false )
	{
		return false;
	}

	if ( pSlotItem->GetSlotType() != CX2SlotItem::ST_PET )
		return false;

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pSlotItem->GetItemTID() );
	if ( pItemTemplet == NULL )
		return false;

	CX2State* pState = static_cast< CX2State* >( g_pMain->GetNowState() );
	if( pState != NULL )
	{
		CX2Cursor* pCursor = pState->GetCursor();
		if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
		{
			return true;
		}
	}

	*m_pSlotBeforeDragging = pSlotItem;
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), GetDraggingItemLayer() );
	(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );

	*m_DraggingItemUID = pSlotItem->GetItemUID();
	
	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == false )
	{
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_INVEN, true );
	}
	
	if ( GetMyInventory() != NULL
        && g_pData->GetUIManager()->GetUIInventory()->GetSortType() != GetMyInventory()->GetSortTypeByItemTemplet( pItemTemplet ) )
	{
		g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( GetMyInventory()->GetSortTypeByItemTemplet( pItemTemplet ) );
	}
	
	
	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );

	return true;
}

bool	CX2UIPetInventory::MouseUp( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pSlot	= NULL;
	pSlot				= static_cast< CX2SlotItem* >( GetSlotInMousePos( mousePos ) );

	if ( pSlot == NULL )
		return false;

	if ( *m_pSlotBeforeDragging	!= NULL )	// 드래그 앤 드랍 일때
	{
		CX2SlotItem*	pSlotItem	= static_cast< CX2SlotItem* >( *m_pSlotBeforeDragging );
		bool			bCheck		= false;
		//{{ kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기
#ifdef	REAL_TIME_ELSWORD
		if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
		{
			if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
				return false;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			if( g_pX2Game->GetMyUnit()->IsTransformed() )
				return false;
#endif

			if ( g_pX2Game->GetMyUnit()->GetNowHp() <= 0 )
			{
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );

				return false;
			}

			if ( g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetWaitStateID() &&
				g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetChargeMpStateID() )
				return false;
		}
#endif	REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기
		bCheck			=	OnDropAnyItem( mousePos );	// 재정의 해야함

		if ( *m_pSlotBeforeDragging	!= NULL && (*m_pSlotBeforeDragging)->GetDialog() != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( 
				(*m_pSlotBeforeDragging)->GetDialog(), (*m_pSlotBeforeDragging)->GetNormalLayer() );
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR( 1, 1, 1, 1 ) );
		}

		if ( bCheck )
		{
			*m_pSlotBeforeDragging	= NULL;
			*m_DraggingItemUID		= 1;
			return true;
		}
	}
	else									// 드래그 앤 드랍이 아닐때
	{
		const CX2Inventory*	pInventory	= NULL;	
		CX2Item*		pItem		= NULL;

		pInventory		= GetMyInventory();

		if ( pInventory == NULL )
			return false;

		pItem			= pInventory->GetItem( pSlot->GetItemUID(), true );

		if ( pItem == NULL )
			return false;

		CX2State*	pState	=	NULL;
		pState				=	static_cast< CX2State* >( g_pMain->GetNowState() );

		if ( pState	!= NULL )
		{
			CX2Cursor*	pCursor	= NULL;
			pCursor				=	pState->GetCursor();

			if ( pCursor !=	NULL &&	pCursor->GetCurorState() !=	CX2Cursor::XCS_NORMAL )
			{
				
				switch ( pCursor->GetCurorState() )
				{
				case CX2Cursor::XCS_PET_FOOD:
					{
						g_pData->GetUIManager()->GetUIInventory()->SetPetFoodSlot(pSlot);
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5352 ), 
							CX2UIInventory::UIM_PET_FOOD_OK, (CKTDXStage*) g_pMain->GetNowState() ); 
						return true;
					}
					break;
				case CX2Cursor::XCS_SOCKET:			// 소켓
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5606 ), g_pMain->GetNowState() );
						return true;
					}
					break;

				case CX2Cursor::XCS_DELETE_ITEM:	// 버리기
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5607 ), g_pMain->GetNowState() );
						return true;
					}
					break;

				case CX2Cursor::XCS_DECOMPOSITION:	// 분해
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5608 ), g_pMain->GetNowState() );
						return true;
					}
					break;

				default:
					break;
				}
			}
		}
	}

	return false;
}

void CX2UIPetInventory::DrawSlotMouseOverImage()
{
	if ( GetCheckOperationCondition() == false )
	{
		m_pDLGSelectedItem->SetShow( false );
		return;
	}

	bool bCheck = false;

	if ( m_pNowOverItemSlot != NULL )
	{
		m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
		m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

		bCheck = true;
	}
	
	if ( bCheck == true )
	{
		m_pDLGSelectedItem->SetShow( true );
	}
	else
	{
		m_pDLGSelectedItem->SetShow( false );
	}

}

bool CX2UIPetInventory::OnDropAnyItem( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pSlotItem = NULL;
	pSlotItem	= static_cast< CX2SlotItem* >( GetSlotInMousePos( mousePos ) );

	if ( pSlotItem == NULL || pSlotItem->GetEnable() == false )
		return false;

	if ( pSlotItem == *m_pSlotBeforeDragging )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
		SetNowDragItemReturn();
		return true;
	}

	if ( (*m_pSlotBeforeDragging)->IsItem() == false )
		return false;

	const CX2Inventory* pInventory = GetMyInventory();

	//{{ kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기
#ifdef	REAL_TIME_ELSWORD

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
				return false;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			if( g_pX2Game->GetMyUnit()->IsTransformed() )
				return false;
#endif

			if ( g_pX2Game->GetMyUnit()->GetNowHp() <= 0 )
			{
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );

				return false;
			}

			// 게임내에서 바쁜 상태인 경우 불가능
			if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
				return false;

		} break;

	default:
		break;
	}
#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기

	switch ( (*m_pSlotBeforeDragging)->GetSlotType() )
	{
	case CX2Slot::ST_INVENTORY:
	case CX2Slot::ST_PET:
	case CX2Slot::ST_EQUIPPED:
		{
			//{{kimch // 2010-12-18 // 던전 또는 대전의 레디 상태에서 펫 인벤 <-> 유저 인벤 안되도록 예외처리 추가
			if ( (*m_pSlotBeforeDragging)->GetSlotType() != CX2Slot::ST_PET )
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReady( STR_ID_9874 );
					SetNowDragItemReturn();
					return true;
				}
			} // if
			//}}kimch // 2010-12-18 // 던전 또는 대전의 레디 상태에서 펫 인벤 <-> 유저 인벤 안되도록 예외처리 추가

			const CX2Item::ItemTemplet*	pFromItemTemplet		= NULL;
			const CX2Item::ItemTemplet*	pToItemTemplet			= NULL;
			pFromItemTemplet	= g_pData->GetItemManager()->GetItemTemplet( pSlotItem->GetItemTID() );
			pToItemTemplet		= g_pData->GetItemManager()->GetItemTemplet( static_cast< CX2SlotItem* >( (*m_pSlotBeforeDragging) )->GetItemTID() );

			// pFromItemTemplet은 NULL 일 수 있음(빈슬롯인경우)
			if ( pToItemTemplet == NULL )
			{
				ASSERT( !"X2UIPrivateBank OnDropAnyItem" );
				return false;
			}

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
			// 펫인벤토리와 유저인벤토리, 은행과 유저 인벤토리 시, 뱃지 이동 차단
			if ( (*m_pSlotBeforeDragging)->GetSlotType() == CX2Slot::ST_INVENTORY &&
				pToItemTemplet->GetItemID() == UNLIMITED_SECOND_CHANGE_JOB_ITEM_ID )
			{
				// 현재 아이템 떨구는 곳이 인벤토리가 아닌 경우
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_29779 ), g_pMain->GetNowState() ); //18422
				SetNowDragItemReturn();
				return true;
			}	
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_EVENT_TEAR_OF_ELWOMAN //pItemSlot->GetSlotType()
			// 펫인벤토리와 유저인벤토리, 은행과 유저 인벤토리 시, 엘의 여인의 눈물 이동 차단
			if ( (*m_pSlotBeforeDragging)->GetSlotType() == CX2Slot::ST_INVENTORY )
			{
				if( NULL == pToItemTemplet->GetItemID() )
					return false;

				if( pToItemTemplet->GetItemID() == TEAR_OF_ELWOMAN_ITEM_ID )
				{
					// 현재 아이템 떨구는 곳이 인벤토리가 아닌 경우
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_18422 ), g_pMain->GetNowState() ); //18422
					SetNowDragItemReturn();
					return true;
				}	
			}
#endif SERV_EVENT_TEAR_OF_ELWOMAN

			//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK

			if ( pToItemTemplet->GetIsPcBang() == true )
			{
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
				SetNowDragItemReturn();
				return true;
			}
#endif	PC_BANG_WORK
			//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스


			// oasis907 : 김상윤 [2010.9.8] // 소켓 강화 사용 아이템 예외 처리, 펫 인벤으로 이동 못하게
			if( g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL &&
				g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->GetShow() == true &&
				g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->CheckSocketUseItem( static_cast< CX2SlotItem* >( (*m_pSlotBeforeDragging) )->GetItemUID() ) )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5112 ), g_pMain->GetNowState() );
				SetNowDragItemReturn();
				return true;
			}


            if ( pInventory != NULL )
            {
			    if ( ( pInventory->GetSortTypeByItemTemplet( pFromItemTemplet ) == pInventory->GetSortTypeByItemTemplet( pToItemTemplet ) ) 
    #ifdef PET_INVENTORY_BUG_FIX_01
				    || ( ( pSlotItem->GetSlotType() == CX2SlotItem::ST_PET ) && ( (*m_pSlotBeforeDragging)->GetSlotType() == CX2SlotItem::ST_PET ) ) 
    #endif PET_INVENTORY_BUG_FIX_01
				    || ( pInventory->IsPossibleAddItem( pInventory->GetSortTypeByItemTemplet( pFromItemTemplet ) ) == true ) )
			    {
				    g_pData->GetUIManager()->GetUIInventory()->Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( static_cast< CX2SlotItem* >( *m_pSlotBeforeDragging ),
					    pSlotItem );	// 확실히 열려 잇으므로 예외처리 안함

				    CX2SlotItem*	pSlotBeforeDragging = static_cast< CX2SlotItem* >( *m_pSlotBeforeDragging );
				    pSlotBeforeDragging->DestroyItemUI();
			    }
			    else
			    {
				    g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3860 ), g_pMain->GetNowState() );
				    SetNowDragItemReturn();
			    }
            }
			
		}
		return true;
	}
	
	return false;
}

bool CX2UIPetInventory::OnRClickedItem( D3DXVECTOR2 mousePos )
{
	// oasis907 : 김상윤 [2010.9.13] // 우클릭 처리 막음
	/*
	CX2SlotItem* pSlotItem = NULL;
	pSlotItem = static_cast< CX2SlotItem* >( GetSlotInMousePos( mousePos ) );
	
	if ( pSlotItem == NULL )
		return false;

	if ( pSlotItem->IsResetItemUI() == false )
		return false;

	CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pSlotItem->GetItemTID() );
	if ( pItemTemplet == NULL )
		return false;

	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == false )
	{
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_INVEN, true );
	}
	//g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTabByUid( pSlotItem->GetItemUID() );
	

	// oasis907 : 김상윤 [2010.9.8] // 소켓 강화 사용 아이템 예외 처리, 펫 인벤에서 인벤으로 이동 못하게
	if( g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL &&
		g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->GetShow() == true &&
		g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->CheckSocketUseItem( pSlotItem->GetItemUID() ) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5112 ), g_pMain->GetNowState() );
		return false;
	}

	CX2Inventory::SORT_TYPE	nowInvenSortType	= CX2Inventory::ST_NONE;
	CX2Inventory*			pInventory			= NULL;

	//nowInvenSortType	=	g_pData->GetUIManager()->GetUIInventory()->GetSortType();
	pInventory			=	GetInventory();

	if ( pInventory == NULL )
		return false;

	nowInvenSortType	= pInventory->GetSortTypeByItemTemplet( pItemTemplet );
	g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTabByUid( pSlotItem->GetItemUID() );

	
	CX2Item*	pItem = NULL;
	for ( int i = 0; i < pInventory->GetItemMaxNum( nowInvenSortType ); i++ )
	{
		pItem	=	pInventory->GetItem( nowInvenSortType, i );
		if ( pItem	== NULL )
		{
			g_pData->GetUIManager()->GetUIInventory()->Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pSlotItem->GetSortType(), 
				pSlotItem->GetSlotID(), nowInvenSortType, i );

			pSlotItem->DestroyItemUI();
			InvalidSlotDesc();
			return true;
		}
		pItem	= NULL;
	}

	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3860 ), g_pMain->GetNowState() );
	*/
	return false;	// 음...다이얼로그??
}

void CX2UIPetInventory::Handler_EGS_SET_AUTO_FEED_PETS_REQ( bool bAutoFeed )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
            g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		    g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		)
		return; 
#ifdef FIX_PET_AUTO_FEED_CEHCK_BUG
	if( NULL != g_pData && NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUIInventory() )
	{
		//저장된 값이 전송하려는 값과 같다면 return;
		if( g_pData->GetUIManager()->GetUIInventory()->GetAutoFeed() == bAutoFeed )
			return;
	}
#endif //FIX_PET_AUTO_FEED_CEHCK_BUG
	KEGS_SET_AUTO_FEED_PETS_REQ kPacket;
	kPacket.m_bAutoFeed = bAutoFeed;

	g_pData->GetServerProtocol()->SendPacket( EGS_SET_AUTO_FEED_PETS_REQ, kPacket );

	return;
}

bool CX2UIPetInventory::Handler_EGS_SET_AUTO_FEED_PETS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SET_AUTO_FEED_PETS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{		
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDlgPetInventory->GetControl(L"g_pCheckBoxAutoFeed");
		if ( pCheckBox != NULL )
		{
			g_pData->GetUIManager()->GetUIInventory()->SetAutoFeed(kEvent.m_bAutoFeed);
			pCheckBox->SetChecked(kEvent.m_bAutoFeed);
		}
		return true;
	}
	return false;
}


wstring CX2UIPetInventory::GetSlotItemDesc()
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

void CX2UIPetInventory::ShowUIDesc( bool bShow, wstring wstr /*= L""*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT /*= CX2TalkBoxManagerImp::TBT_FROM_UP_RIGHT*/, D3DXCOLOR coTextColor /*= D3DXCOLOR(0,0,0,1)*/, D3DXCOLOR coBackColor /*= D3DXCOLOR(1, 0.96f, 0.6f, 1 )*/)
{
	m_pTalkBoxManager->Clear();

	if(bShow)
	{
		CX2TalkBoxManagerImp::TalkBox talkBox;

		talkBox.m_vPos				= pos;
		talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
		talkBox.m_fRemainTime		= 2.0f;

		talkBox.m_wstrTalkContent	= wstr.c_str();
		talkBox.m_bTraceUnit		= false;
		talkBox.m_TalkBoxType		= TBT;
		talkBox.m_coTextColor		= coTextColor;
		talkBox.m_BackTexColor		= coBackColor;

		m_pTalkBoxManager->Push( talkBox );

	}
}


//}} oasis907 : 김상윤 [2010.8.31] // 펫 인벤토리
#endif SERV_PET_SYSTEM
