#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI
//#define UNIT_VIEWER_ADD

#include ".\X2UIDragable.h"


CX2UIDragable::CX2UIDragable( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2ItemSlotManager( pNowStage, NULL )
//: CX2ItemSlotManager( pNowStage, pFileName )
{
}

CX2UIDragable::~CX2UIDragable(void)
{
}


HRESULT CX2UIDragable::OnFrameMove( double fTime, float fElapsedTime )
{

	CX2ItemSlotManager::OnFrameMove(fTime, fElapsedTime);

	return S_OK;
}


bool CX2UIDragable::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			MouseUp( mousePos );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			if ( MouseDown( mousePos) == true )
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
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			SetNowDragItemReturn();
		}
		break;

	case WM_RBUTTONUP:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			SetNowDragItemReturn();
			MouseRButtonUp( mousePos );
		}

	}

	return false;
}



bool CX2UIDragable::MouseDown( D3DXVECTOR2 mousePos )
{

	CX2SlotItem* pSlot = (CX2SlotItem*)GetSlotInMousePos( mousePos );

	if ( pSlot == NULL )
		return false;

	if ( pSlot->IsResetItemUI() == false )
	{
		return false;
	}
	*m_pSlotBeforeDragging = pSlot;
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), GetDraggingItemLayer() );
	(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );

	*m_DraggingItemUID = pSlot->GetItemUID();

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );

	// 만일 인벤토리가 열려 있으면 탭을 바꿔 주자
	if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN) )
		g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTabByUid( pSlot->GetItemUID() );

	return true;
}


bool CX2UIDragable::MouseUp( D3DXVECTOR2 mousePos )
{
	if(GetSlotInMousePos( mousePos ) == NULL) return false;
	// 해당 다이얼로그에 슬롯이 있는 경우
	if ( *m_pSlotBeforeDragging != NULL )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*) *m_pSlotBeforeDragging;

		// 해당 다이얼로그에 뭔가 드래그 되었다는 신호를 보낸다
		OnDropAnyItem( mousePos );

		// 뒷정리 : "드래그 중" 이라고 표시된 아이템을 삭제
		SetNowDragItemReturn();

		return true;
	}


	if ( *m_pSlotBeforeDragging != NULL && (*m_pSlotBeforeDragging)->GetDialog() != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), (*m_pSlotBeforeDragging)->GetNormalLayer() );
		(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
	}

	//*m_pSlotBeforeDragging = NULL;
	//*m_DraggingItemUID = -1;

	return false;
}

void CX2UIDragable::MouseMove( D3DXVECTOR2 mousePos )
{
	bool bCheck = false;
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
		if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
			continue;

		if ( pItemSlot->IsInMousePoint( mousePos ) == true )
		{
			SetNowOverItemSlotAndDesc( pItemSlot );
			bCheck = true;
		}
	}

	if ( bCheck == false )
	{
		SetNowOverItemSlotAndDesc( NULL );
	}

	CX2SlotManager::MouseMove( mousePos );
}



bool CX2UIDragable::MouseRButtonUp( D3DXVECTOR2 mousePos )
{
	if(GetSlotInMousePos( mousePos ) == NULL) return false;
	//*m_pSlotBeforeDragging = NULL;	
	//*m_DraggingItemUID = -1;

	return OnRClickedItem( mousePos );
}


void CX2UIDragable::SetNowDragItemReturn()
{
	if ( *m_pSlotBeforeDragging != NULL )
	{
		// 혹시 장비면
		if ( (*m_pSlotBeforeDragging)->GetSlotType() == CX2SlotManager::CX2Slot::ST_EQUIPPED )
		{
			//{{ 2008.11.13 김태완 : UI 예외처리
			UidType DraggingItemUID = ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetItemUID();
			//}}
			if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( DraggingItemUID ) == true 
				&& g_pData->GetUIManager()->GetUICharInfo() != NULL )
			{
				g_pData->GetUIManager()->GetUICharInfo()->EquipChanged();						
			}
		}
		(*m_pSlotBeforeDragging)->ResetPos();
		if ( (*m_pSlotBeforeDragging)->GetDialog() != NULL )
		{
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), (*m_pSlotBeforeDragging)->GetNormalLayer() );
		}

	}
	*m_pSlotBeforeDragging = NULL;

}


void CX2UIDragable::DrawSlotMouseOverImage()
{
	if ( GetCheckOperationCondition() == false )
	{
		m_pDLGSelectedItem->SetShow( false );
		return;
	}

	bool bCheck = false;

	if ( *m_pSlotBeforeDragging != NULL )
	{
		if ( IsInEquippedItemSlotReact( m_NowMousePos ) == true && (*m_pSlotBeforeDragging)->IsItem() )
		{
			//{{ 2008.11.13 김태완 : UI 예외처리
			//*m_DraggingItemUID = ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetItemUID();
			//}}
			const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

			// 예외처리 추가
			if( kInventory.GetItem( *m_DraggingItemUID, true ) == NULL || 
				kInventory.GetItem( *m_DraggingItemUID, true )->GetItemTemplet() == NULL
                )
				return;

			CX2Item::ITEM_TYPE itemType = kInventory.GetItem( *m_DraggingItemUID, true )->GetItemTemplet()->GetItemType();
			if (  itemType == CX2Item::IT_WEAPON || itemType == CX2Item::IT_DEFENCE || itemType == CX2Item::IT_ACCESSORY )
			{
				CX2Unit::EQIP_POSITION equipPosition = kInventory.GetItem( *m_DraggingItemUID, true )->GetItemTemplet()->GetEqipPosition();

				for ( int i = 0; i < (int)m_SlotList.size(); i++ )
				{
					CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
					if ( pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED && pItemSlot->GetEquipPos() == equipPosition && 
						( itemType == CX2Item::IT_ACCESSORY || pItemSlot->GetFashion() == kInventory.GetItem( *m_DraggingItemUID, true )->GetItemTemplet()->GetFashion() ))
					{
						m_pDLGSelectedItem->SetPos( pItemSlot->GetPos() );
						m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( pItemSlot->GetSize() );

						bCheck = true;
					}
				}
			}
			else
			{
				if ( m_pNowOverItemSlot != NULL )
				{
					m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
					m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

					bCheck = true;
				}
			}
		}
		else
		{
			if ( m_pNowOverItemSlot != NULL )
			{
				m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
				m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

				bCheck = true;
			}
		}
	}
	else
	{
		if ( m_pNowOverItemSlot != NULL )
		{
			m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
			m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

			bCheck = true;
		}
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

wstring CX2UIDragable::GetSlotItemDesc()
{

	wstring itemDesc=  L"";

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
				itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID(), false );
			else
				itemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID(), false );
		}

	}

	return itemDesc;

}


#endif
