#include "StdAfx.h"

#include ".\X2ItemSlotManager.h"


CX2ItemSlotManager::CX2ItemSlotManager( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2SlotManager( pNowStage, pFileName )
{
	//{{ 비교할 수 있는 착용 중인 아이템
	m_pDLGItemDescCompare = new CKTDGUIDialog( pNowStage, L"DLG_SlotManager.lua", 0.07f, GetItemDescLayer() );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemDescCompare );
	m_pDLGItemDescCompare->SetShow( false );
	m_pDLGItemDescCompare->SetFront( true );

	CKTDGUIStatic* pStatic = new CKTDGUIStatic();
	pStatic->SetName( L"ItemDesc" );
	m_pDLGItemDescCompare->AddControl( pStatic );
	CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
	pStatic->AddPicture( pPicture );
	pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
	pPicture->SetTex( L"FadeInOut.dds" );
	
	CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
	pStatic->AddString( pString );
	pString->fontIndex = SLOT_MANAGER_FONT_INDEX;
	pString->color = D3DXCOLOR( 1,1,1,1 );
	pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
	pString->msg = L"";
	pString->fontStyle = CKTDGFontManager::FS_NONE;
	pString->sortFlag = DT_LEFT;
	pString->pos = D3DXVECTOR2( 20, 20 );
	//}} 비교할 수 있는 착용 중인 아이템
	 
	//{{ 마우스 커서가 위치하고 있는 아이템
	m_pDLGItemDesc = new CKTDGUIDialog( pNowStage, L"DLG_SlotManager.lua", 0.07f, GetItemDescLayer() );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemDesc );
	m_pDLGItemDesc->SetShow( false );
	m_pDLGItemDesc->SetFront( true );

	pStatic = new CKTDGUIStatic();
	pStatic->SetName( L"ItemDesc" );
	m_pDLGItemDesc->AddControl( pStatic );
	pPicture = new CKTDGUIControl::CPictureData();
	pStatic->AddPicture( pPicture );
	pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
	pPicture->SetTex( L"FadeInOut.dds" );

	pString = new CKTDGUIControl::UIStringData();
	pStatic->AddString( pString );
	pString->fontIndex = SLOT_MANAGER_FONT_INDEX;
	pString->color = D3DXCOLOR( 1,1,1,1 );
	pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
	pString->msg = L"";
	pString->fontStyle = CKTDGFontManager::FS_NONE;
	pString->sortFlag = DT_LEFT;
	pString->pos = D3DXVECTOR2( 20, 20 );
	//{{ 마우스 커서가 위치하고 있는 아이템


	m_pDLGSelectedItem = new CKTDGUIDialog( pNowStage, L"DLG_SlotManager.lua", 0.07f, GetSelectedItemLayer() );
	m_pDLGSelectedItem->SetShow( false );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSelectedItem );
	CKTDGUIStatic* pStatic2 = new CKTDGUIStatic();
	pStatic2->SetName( L"SelectedItem" );
	m_pDLGSelectedItem->AddControl( pStatic2 );
	CKTDGUIControl::CPictureData* pPicture2 = new CKTDGUIControl::CPictureData();
	pStatic2->AddPicture( pPicture2 );
	pPicture2->SetTex( L"DLG_Common_Texture01.tga", L"LIGHT" );	
	

	m_DraggingItemTID = -1;
#ifdef NEW_VILLAGE_UI
	m_DraggingItemUID = g_pData->GetpDraggingItemUID();
#else
	m_DraggingItemUID = -1;
#endif

	m_NowMousePos = D3DXVECTOR2( 0, 0 );

	m_MyInfoSlotItemReactLeftTop = D3DXVECTOR2( 
		m_pDLGSelectedItem->GetDummyPos(0).x, m_pDLGSelectedItem->GetDummyPos(0).y );
	m_MyInfoSlotItemReactRightBottom = D3DXVECTOR2( 
		m_pDLGSelectedItem->GetDummyPos(1).x, m_pDLGSelectedItem->GetDummyPos(1).y );

	m_bShowRBDownMessage = true;

	//{{ JHKang / 강정훈 / 2010. 9. 20 / 아이템 설명이 세로 크기보다 클 경우 추가될 다이얼로그
#ifdef FIX_TOOLTIP
	m_pDLGItemDescAdd = new CKTDGUIDialog( pNowStage, L"DLG_SlotManager.lua", 0.07f, GetItemDescLayer() );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemDescAdd );
	m_pDLGItemDescAdd->SetShow( false );
	m_pDLGItemDescAdd->SetFront( true );

	pStatic = new CKTDGUIStatic();
	pStatic->SetName( L"ItemDesc" );
	m_pDLGItemDescAdd->AddControl( pStatic );
	pPicture = new CKTDGUIControl::CPictureData();
	pStatic->AddPicture( pPicture );
	pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
	pPicture->SetTex( L"FadeInOut.dds" );

	pString = new CKTDGUIControl::UIStringData();
	pStatic->AddString( pString );
	pString->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;
	pString->color = D3DXCOLOR( 1,1,1,1 );
	pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
	pString->msg = L"";
	pString->fontStyle = CKTDGFontManager::FS_NONE;
	pString->sortFlag = DT_LEFT;
	pString->pos = D3DXVECTOR2( 20, 20 );

	m_pDLGItemDescCompareAdd = new CKTDGUIDialog( pNowStage, L"DLG_SlotManager.lua", 0.07f, GetItemDescLayer() );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemDescCompareAdd );
	m_pDLGItemDescCompareAdd->SetShow( false );
	m_pDLGItemDescCompareAdd->SetFront( true );

	pStatic = new CKTDGUIStatic();
	pStatic->SetName( L"ItemDesc" );
	m_pDLGItemDescCompareAdd->AddControl( pStatic );
	pPicture = new CKTDGUIControl::CPictureData();
	pStatic->AddPicture( pPicture );
	pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
	pPicture->SetTex( L"FadeInOut.dds" );

	pString = new CKTDGUIControl::UIStringData();
	pStatic->AddString( pString );
	pString->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;
	pString->color = D3DXCOLOR( 1,1,1,1 );
	pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
	pString->msg = L"";
	pString->fontStyle = CKTDGFontManager::FS_NONE;
	pString->sortFlag = DT_LEFT;
	pString->pos = D3DXVECTOR2( 20, 20 );
#endif FIX_TOOLTIP
	//}} JHKang / 강정훈 / 2010. 9. 20 / 아이템 설명이 세로 크기보다 클 경우 추가될 Dialog
#ifdef SERV_CUBE_IN_ITEM_MAPPING
	m_mapToolTipUseOpenRandomID.clear();
#endif //SERV_CUBE_IN_ITEM_MAPPING
}

CX2ItemSlotManager::~CX2ItemSlotManager( void )
{
	//SAFE_DELETE_DIALOG( m_pDLGItemDesc );
	//SAFE_DELETE_DIALOG( m_pDLGItemDescCompare );
	SAFE_DELETE_DIALOG( m_pDLGSelectedItem );
}

void CX2ItemSlotManager::SetSlotManagerLayer( int slotManagerLayer )
{
	CX2SlotManager::SetSlotManagerLayer( slotManagerLayer );	

	if ( m_pDLGItemDescCompare != NULL )
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGItemDescCompare, GetItemDescLayer() ); 

	if ( m_pDLGItemDesc != NULL )
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGItemDesc, GetItemDescLayer() ); 

#ifdef FIX_TOOLTIP
	if ( m_pDLGItemDescAdd != NULL )
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGItemDescAdd, GetItemDescLayer() ); 
	if ( m_pDLGItemDescCompareAdd != NULL )
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGItemDescCompareAdd, GetItemDescLayer() );
#endif FIX_TOOLTIP

#ifdef NEW_VILLAGE_UI
	if ( *m_pSlotBeforeDragging != NULL && (*m_pSlotBeforeDragging)->GetDialog() != NULL )
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), GetDraggingItemLayer() );
#else
	if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetDraggingItemLayer() );
#endif

	if ( m_pDLGSelectedItem != NULL )
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGSelectedItem, GetSelectedItemLayer() ); 

}

CX2SlotItem* CX2ItemSlotManager::GetSlotByID( int iSlotID )
{
	for ( unsigned int iS = 0; iS < m_SlotList.size(); ++iS )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[iS];
		if ( pSlotItem->GetSlotID() == iSlotID )
		{
			return pSlotItem;
		}
	}

	return NULL;
}

HRESULT CX2ItemSlotManager::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	DrawSlotMouseOverImage();
	//DrawSlotDesc();

	return S_OK;
}

void CX2ItemSlotManager::MouseMove( D3DXVECTOR2 mousePos )
{
	m_NowMousePos = mousePos;

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

bool CX2ItemSlotManager::MouseDown( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pSlot = (CX2SlotItem*)GetSlotInMousePos( mousePos );

	if ( pSlot == NULL )
		return false;

	if ( pSlot->GetDialog() == NULL )
		return false;

	if( false == pSlot->GetClickable() )
		return false;
#ifdef NEW_VILLAGE_UI
	*m_pSlotBeforeDragging = pSlot;
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), GetDraggingItemLayer() );
	(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );

	*m_DraggingItemUID = pSlot->GetItemUID();
#else
	m_pSlotBeforeDragging = pSlot;
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetDraggingItemLayer() );
	m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );

	m_DraggingItemUID = pSlot->GetItemUID();
#endif

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );

	return true;
}

void CX2ItemSlotManager::DrawSlotMouseOverImage()
{
	KTDXPROFILE();


	if ( GetCheckOperationCondition() == false )
	{
		m_pDLGSelectedItem->SetShow( false );
		return;
	}
	
	bool bCheck = false;

#ifdef NEW_VILLAGE_UI
	if ( *m_pSlotBeforeDragging != NULL )
#else
	if ( m_pSlotBeforeDragging != NULL )
#endif
	{
		bCheck = DrawSlotMouseOverImageInEquipRect();
	}
	else
	{
		if ( m_pNowOverItemSlot != NULL )
		{
			// 상점의 구입모드 : 빈 슬롯일 경우 예외 처리

			CX2Item* pkItem = NULL;

			if( NULL != g_pData && 
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_pNowOverItemSlot->GetItemUID() );
			}

            const CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
			if ( pkItem == NULL && 
                pkItemTemplet == NULL && 
				m_pNowOverItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_SHOP_BUY )
			{
			}
			else
			{
				//m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() + GetPos() );
				//m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

				D3DXVECTOR2 vPosSelectedItem = m_pNowOverItemSlot->GetPos() + GetPos();
//				vPosSelectedItem.x += 1.f;
//				vPosSelectedItem.y -= 1.f;
				m_pDLGSelectedItem->SetPos( vPosSelectedItem );


				
				
				D3DXVECTOR2 vSizeSelectedItem = m_pNowOverItemSlot->GetSize();
//				vSizeSelectedItem.x -= 2.f;
//				vSizeSelectedItem.y -= 2.f;
				m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( vSizeSelectedItem );

				bCheck = true;
			}
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




void CX2ItemSlotManager::DrawSlotDesc()
{

	KTDXPROFILE();


	if ( GetCheckOperationCondition() == false )
	{
		m_pDLGItemDesc->SetShow( false );
		m_pDLGItemDescCompare->SetShow( false );
#ifdef FIX_TOOLTIP
		m_pDLGItemDescAdd->SetShow( false );
		m_pDLGItemDescCompareAdd->SetShow( false );
#endif FIX_TOOLTIP
		return;
	}

	bool bCheckDrawItemDesc = false;

#ifdef FIX_TOOLTIP
	bool bItemDescAdd = false;			// 선택된 아이템의 설명 나누기
	bool bItemDescAddComp = false;		// 비교될 아이템의 설명 나누기

	const int iWidth = DXUTGetBackBufferSurfaceDesc()->Width;
	const int iHeight = DXUTGetBackBufferSurfaceDesc()->Height;
	const int MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT = static_cast<int>( iHeight / g_pKTDXApp->GetResolutionScaleY() );
	const int SIZE_Y = 40;
#endif FIX_TOOLTIP

#ifdef NEW_VILLAGE_UI
	if ( *m_pSlotBeforeDragging == NULL && 
		m_pNowOverItemSlot != NULL )
#else
	if ( m_pSlotBeforeDragging == NULL && 
		m_pNowOverItemSlot != NULL )
#endif
	{
		// kimhc // 2009-09-29 // 캐릭터 선택하지 않았을때 실행되는 경우에 대한 예외처리
		if ( g_pData->GetMyUser() == NULL || 
			 g_pData->GetMyUser()->GetSelectUnit() == NULL )
			 return;

		// 상점의 구입모드 : 빈 슬롯일 경우 예외 처리
		CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_pNowOverItemSlot->GetItemUID() );
        const CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
		if ( pkItem == NULL && 
            pkItemTemplet == NULL &&
			m_pNowOverItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_SHOP_BUY)
		{
			m_pDLGItemDesc->SetShow( false );
			m_pDLGItemDescCompare->SetShow( false );
#ifdef FIX_TOOLTIP
			m_pDLGItemDescAdd->SetShow( false );
			m_pDLGItemDescCompareAdd->SetShow( false );
#endif FIX_TOOLTIP
			return;
		}
		wstring wstrItemDesc = L"";
		bool bCheckEquipping = false;
		CX2Item* pItem = GetCompareItem( m_pNowOverItemSlot->GetItemTID(), m_pNowOverItemSlot->GetItemUID(), bCheckEquipping );
		//현재 장착중인 아이템이 있다면 장착중인 아이템에 대한 설명 추가
		if ( bCheckEquipping == false && pItem != NULL && 
			GetIsShowCompareItemDescBySlotType( m_pNowOverItemSlot->GetSlotType() ) == true )
		{
			wstrItemDesc = GET_STRING( STR_ID_256 ) + GetSlotItemDescByUID( pItem->GetUID(), false );
		}
		else
		{
			wstrItemDesc = GetSlotItemDesc();
		}

		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );

		//{{ JHKang / 강정훈 / 2010.9.17 / 툴팁 생성 줄바꿈 관련 개선
		// 툴팁 수정에 따른 가로 크기 변경 (현재 가로 해상도 크기 / 4), 가로 세로 크기 조절
		// 수정 전 : 문자열 크기 / 해상도 비율 + 상수
#ifdef FIX_TOOLTIP
		int itemDescWidth = static_cast<int>( iWidth / 3 / g_pKTDXApp->GetResolutionScaleX() );
		int itemDescHeight = 0;
		int itemDescFontHeight = g_pKTDXApp->GetResolutionScaleY() > 1.1 ? 15 : 16;	// XUF_DODUM_13_SEMIBOLD의 크기

#ifdef USE_FREE_TYPE
		itemDescFontHeight += m_iDescFontHeightOffset;
#endif //USE_FREE_TYPE

		wstring wstrItemDescTemp = wstrItemDesc;

#ifdef CLIENT_GLOBAL_LINEBREAK
		wstrItemDesc = CWordLineHandler::GetStrByLineBreakColorInX2Main(wstrItemDesc.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX);
#else //CLIENT_GLOBAL_LINEBREAK
		wstrItemDesc = g_pMain->GetStrByLineBreakColor(wstrItemDesc.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX);
#endif //CLIENT_GLOBAL_LINEBREAK

		SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, false );
#else
		int itemDescWidth = (int)( pItemDescFont->GetWidth( wstrItemDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
		int itemDescHeight = (int)( pItemDescFont->GetHeight( wstrItemDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;
#endif FIX_TOOLTIP
		//}} JHKang / 강정훈 / 2010.9.17 / 툴팁 생성 줄바꿈 관련 개선



		// 아이템 툴팁의 길이가 너무 길면 폰트 크기를 줄이자
		CKTDGUIStatic* pItemDesc = (CKTDGUIStatic*)m_pDLGItemDesc->GetControl(L"ItemDesc");
		if( NULL != pItemDesc && 
			NULL != pItemDesc->GetString(0) )
		{
#ifdef FIX_TOOLTIP
#else
			const int MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT = 600;
#endif FIX_TOOLTIP
			
			if( itemDescHeight > MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT )
			{
				pItemDesc->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;
				pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX_SMALL );
#ifdef FIX_TOOLTIP
				itemDescFontHeight = g_pKTDXApp->GetResolutionScaleY() > 1.1 ? 13 : 14;	// XUF_DODUM_11_NORMAL의 크기
#ifdef USE_FREE_TYPE
				itemDescFontHeight += m_iDescFontHeightOffset;
#endif //USE_FREE_TYPE

#ifdef CLIENT_GLOBAL_LINEBREAK
				wstrItemDesc = CWordLineHandler::GetStrByLineBreakColorInX2Main(wstrItemDescTemp.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
#else //CLIENT_GLOBAL_LINEBREAK
				wstrItemDesc = g_pMain->GetStrByLineBreakColor(wstrItemDescTemp.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
#endif //CLIENT_GLOBAL_LINEBREAK
				
				SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, false );
				
#else
				itemDescWidth = (int)( pItemDescFont->GetWidth( wstrItemDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
				itemDescHeight = (int)( pItemDescFont->GetHeight( wstrItemDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;
#endif FIX_TOOLTIP
			}
			else
			{
				pItemDesc->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX;
			}
		}

#ifndef FIX_TOOLTIP
		int offsetSize = 0;
		int lineNum = 0;
		offsetSize = wstrItemDesc.find( L"\n", offsetSize );
		while( offsetSize != -1 )
		{
			offsetSize++;
			lineNum++;
			offsetSize = wstrItemDesc.find( L"\n", offsetSize );
		}

		itemDescHeight += lineNum;
#endif FIX_TOOLTIP

		//{{ 폰트를 작게 설정했는데도 화면 높이보다 클 경우는 분할
#ifdef FIX_TOOLTIP
		int itemDescAddHeight = 0;
		if ( itemDescHeight > MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT )
		{
			itemDescWidth = static_cast<int>( iWidth / 4 / g_pKTDXApp->GetResolutionScaleX() );

#ifdef CLIENT_GLOBAL_LINEBREAK
			wstrItemDesc = CWordLineHandler::GetStrByLineBreakColorInX2Main(wstrItemDescTemp.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
#else //CLIENT_GLOBAL_LINEBREAK
			wstrItemDesc = g_pMain->GetStrByLineBreakColor(wstrItemDescTemp.c_str(), itemDescWidth - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
#endif //CLIENT_GLOBAL_LINEBREAK

			SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, false );

			itemDescAddHeight = static_cast<int>( itemDescHeight - MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT + itemDescFontHeight * 2 );
			itemDescHeight = static_cast<int>( MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT );
			bItemDescAdd = true;

			SplitDescHeight( &wstrItemDesc, &wstrItemDescTemp, itemDescHeight, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, true );
		}
#endif FIX_TOOLTIP
		//{{ 폰트를 작게 설정했는데도 화면 높이보다 클 경우는 분할

		int itemDescPosX = 0;
		int itemDescPosY = 0;

		if ( m_pNowOverItemSlot->GetPos().x + GetPos().x + ( m_pNowOverItemSlot->GetSize().x / 2 ) - ( itemDescWidth / 2 ) >= 0 ) //왼쪽에 공간이 충분한 경우
		{
#ifdef FIX_TOOLTIP
			//{{ 화면 너비를 넘어갈 경우
			if ( m_pNowOverItemSlot->GetPos().x + GetPos().x + ( m_pNowOverItemSlot->GetSize().x / 2 ) - ( itemDescWidth / 2 ) + itemDescWidth > iWidth / g_pKTDXApp->GetResolutionScaleX() )
			{
				itemDescPosX = (int)(iWidth / g_pKTDXApp->GetResolutionScaleX() - itemDescWidth);

				if (true == bItemDescAdd)
					itemDescPosX = (int)(iWidth / g_pKTDXApp->GetResolutionScaleX() - itemDescWidth * 2);
			}
#else
			if ( m_pNowOverItemSlot->GetPos().x + GetPos().x + ( m_pNowOverItemSlot->GetSize().x / 2 ) - ( itemDescWidth / 2 ) + itemDescWidth > 1024 )
			{
				itemDescPosX = (int)(1024 - itemDescWidth);
			}
#endif FIX_TOOLTIP
			else
			{
				itemDescPosX = (int)(m_pNowOverItemSlot->GetPos().x + GetPos().x + ( m_pNowOverItemSlot->GetSize().x / 2 ) - ( itemDescWidth / 2 ));
			}
		}
		else
		{
			itemDescPosX = 0;
		}

		if ( m_pNowOverItemSlot->GetPos().y + GetPos().y - itemDescHeight >= 0 ) //슬롯위로 표시할 수 있는경우
		{
			itemDescPosY = (int)(m_pNowOverItemSlot->GetPos().y + GetPos().y - itemDescHeight);
		}
		else
		{
#ifdef FIX_TOOLTIP
			if( m_pNowOverItemSlot->GetPos().y + GetPos().y < MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT / 2 )
			{
				itemDescPosY = (int)(m_pNowOverItemSlot->GetPos().y + GetPos().y + m_pNowOverItemSlot->GetSize().y);
				int overSize = static_cast<int>( MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT - (itemDescPosY + itemDescHeight) );

				if ( overSize < 0 )
				{
					itemDescPosY += overSize;
				}
			}
			else
			{
				itemDescPosY = 0;
			}
#else
			itemDescPosY = (int)(m_pNowOverItemSlot->GetPos().y + GetPos().y + m_pNowOverItemSlot->GetSize().y);
			int overSize = 768 - (itemDescPosY + itemDescHeight);

			if ( overSize < 0 )
			{
				itemDescPosY += overSize;
			}
#endif FIX_TOOLTIP
		}


		if ( m_pNowOverItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
		{
			if ( m_pNowOverItemSlot->GetDialog() != NULL )
			{
				if ( wstrItemDesc.empty() == false )
				{
					bCheckDrawItemDesc = true;
				}		
			}
		}
		else
		{
			if ( wstrItemDesc.empty() == false )
			{
				bCheckDrawItemDesc = true;
			}
		}



		// 아이템 설명 위부분이 화면 밖으로 나가지 않도록 제한
		if( itemDescPosY < 0 )
			itemDescPosY = 0;

		if ( bCheckDrawItemDesc == true )
		{
			m_pDLGItemDesc->SetPos( D3DXVECTOR2( (float)itemDescPosX, (float)itemDescPosY ) );
			m_pDLGItemDesc->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)itemDescHeight) );
			m_pDLGItemDesc->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );

			//{{ 분할된 설명 보이기
#ifdef FIX_TOOLTIP
			if ( true == bItemDescAdd )
			{
				m_pDLGItemDescAdd->GetStatic_LUA( "ItemDesc" )->GetString(0)->msg = wstrItemDescTemp.c_str();
			}
			else
			{
				m_pDLGItemDescAdd->SetShow( false );
			}

#endif FIX_TOOLTIP
			//}} 분할된 설명 보이기

			//{{ 장착중인 아이템이 있을 경우 선택된 아이템에 대한 설명 추가
			if ( bCheckEquipping == false && pItem != NULL && 
				GetIsShowCompareItemDescBySlotType( m_pNowOverItemSlot->GetSlotType() ) == true )
			{
				wstring itemDescCompare = GetSlotItemDesc();
				CKTDGFontManager::CUKFont* pItemDescFontCompare = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );
#ifdef FIX_TOOLTIP
				int itemDescWidthCompare = 0;
				int itemDescHeightCompare = 0;

				itemDescWidthCompare = static_cast<int>( iWidth / 3 / g_pKTDXApp->GetResolutionScaleX() );
				
				itemDescFontHeight = g_pKTDXApp->GetResolutionScaleY() > 1.1 ? 15 : 16;
#ifdef USE_FREE_TYPE
				itemDescFontHeight += m_iDescFontHeightOffset;
#endif //USE_FREE_TYPE
				wstring wstrItemDescCompTemp = itemDescCompare;

#ifdef CLIENT_GLOBAL_LINEBREAK
				itemDescCompare = CWordLineHandler::GetStrByLineBreakColorInX2Main(itemDescCompare.c_str(), itemDescWidthCompare - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX);
#else //CLIENT_GLOBAL_LINEBREAK
				itemDescCompare = g_pMain->GetStrByLineBreakColor(itemDescCompare.c_str(), itemDescWidthCompare - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX);
#endif //CLIENT_GLOBAL_LINEBREAK

				SplitDescHeight( &itemDescCompare, &wstrItemDescCompTemp, itemDescHeightCompare, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, false );
#else
				int itemDescWidthCompare = (int)( pItemDescFontCompare->GetWidth( itemDescCompare.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
				int itemDescHeightCompare = (int)( pItemDescFontCompare->GetHeight( itemDescCompare.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;
#endif FIX_TOOLTIP



				// 비교 아이템 툴팁의 길이가 너무 길면 폰트 크기를 줄이자
				CKTDGUIStatic* pItemDescCompare = (CKTDGUIStatic*) m_pDLGItemDescCompare->GetControl(L"ItemDesc");
				if( NULL != pItemDescCompare && 
					NULL != pItemDescCompare->GetString(0) )
				{
#ifdef FIX_TOOLTIP
#else
					const int MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT = 600;
#endif FIX_TOOLTIP

					if( itemDescHeightCompare > MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT )
					{
						pItemDescCompare->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;
						pItemDescFontCompare = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX_SMALL );
#ifdef FIX_TOOLTIP
						itemDescFontHeight = g_pKTDXApp->GetResolutionScaleY() > 1.1 ? 13 : 14;	// XUF_DODUM_11_NORMAL의 크기
#ifdef USE_FREE_TYPE
						itemDescFontHeight += m_iDescFontHeightOffset;
#endif //USE_FREE_TYPE
#ifdef CLIENT_GLOBAL_LINEBREAK
						itemDescCompare = CWordLineHandler::GetStrByLineBreakColorInX2Main(wstrItemDescCompTemp.c_str(), itemDescWidthCompare - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
#else //CLIENT_GLOBAL_LINEBREAK
						itemDescCompare = g_pMain->GetStrByLineBreakColor(wstrItemDescCompTemp.c_str(), itemDescWidthCompare - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
#endif //CLIENT_GLOBAL_LINEBREAK

						SplitDescHeight( &itemDescCompare, &wstrItemDescCompTemp, itemDescHeightCompare, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, false );
#else
						itemDescWidthCompare = (int)( pItemDescFontCompare->GetWidth( itemDescCompare.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
						itemDescHeightCompare = (int)( pItemDescFontCompare->GetHeight( itemDescCompare.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;
#endif FIX_TOOLTIP
					}
					else
					{
						pItemDescCompare->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX;
					}
				}

#ifndef FIX_TOOLTIP
				int offsetSize = 0;
				int lineNum = 0;
				offsetSize = itemDescCompare.find( L"\n", offsetSize );
				while( offsetSize != -1 )
				{
					offsetSize++;
					lineNum++;
					offsetSize = itemDescCompare.find( L"\n", offsetSize );
				}
				itemDescHeightCompare += lineNum;
#endif
				
				//{{ 폰트를 작게 설정했는데도 화면 높이보다 클 경우는 분할
#ifdef FIX_TOOLTIP
				int itemDescAddHeightComp = 0;
				if ( itemDescHeightCompare > MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT )
				{
					itemDescWidthCompare = static_cast<int>( iWidth / 4 / g_pKTDXApp->GetResolutionScaleX() );

#ifdef CLIENT_GLOBAL_LINEBREAK
					itemDescCompare = CWordLineHandler::GetStrByLineBreakColorInX2Main(wstrItemDescCompTemp.c_str(), itemDescWidthCompare - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
#else //CLIENT_GLOBAL_LINEBREAK
					itemDescCompare = g_pMain->GetStrByLineBreakColor(wstrItemDescCompTemp.c_str(), itemDescWidthCompare - ITEM_DESC_ADD_SIZE_X, SLOT_MANAGER_FONT_INDEX_SMALL);
#endif //CLIENT_GLOBAL_LINEBREAK

					SplitDescHeight( &itemDescCompare, &wstrItemDescCompTemp, itemDescHeightCompare, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, false );

					itemDescAddHeightComp = static_cast<int>( itemDescHeightCompare - MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT + itemDescFontHeight * 2 );
					itemDescHeightCompare = MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT;
					bItemDescAddComp = true;

					SplitDescHeight( &itemDescCompare, &wstrItemDescCompTemp, itemDescHeightCompare, itemDescFontHeight, SLOT_MANAGER_FONT_INDEX, true );
				}
#endif FIX_TOOLTIP
				//{{ 폰트를 작게 설정했는데도 화면 높이보다 클 경우는 분할
				
#ifndef FIX_TOOLTIP
				int adjustValue = 0;

				if ( itemDescPosX + itemDescWidth + itemDescWidthCompare > 1024 )
				{
					adjustValue = 1024 - ( itemDescPosX + itemDescWidth + itemDescWidthCompare ); 
				}
#endif FIX_TOOLTIP

#ifdef FIX_TOOLTIP
				//{{ 위치 조정
				int itemDescPosXCompare = static_cast<int>( iWidth / g_pKTDXApp->GetResolutionScaleX() - itemDescWidthCompare );
				int itemDescPosYCompare = itemDescPosY;

				// 아이템의 위치가 아래를 파고들면 올려주자
				int overSize = static_cast<int>( MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT - ( itemDescPosYCompare + itemDescHeightCompare ) );
				if ( overSize < 0 )
					itemDescPosYCompare += overSize;

				if (itemDescPosYCompare < 0)
					itemDescPosYCompare = 0;
				
				if ( itemDescPosYCompare < itemDescPosY )
					itemDescPosY = itemDescPosYCompare;

				//{{ 비교 아이템이 분할되었을 때
				if ( true == bItemDescAddComp )
				{
					itemDescPosY = itemDescPosYCompare = 0;
					itemDescPosXCompare = static_cast<int>( iWidth / g_pKTDXApp->GetResolutionScaleX() - itemDescWidthCompare * 2 + 2 );

					/// 비교 아이템 분할된 정보 표시
					m_pDLGItemDescCompareAdd->SetPos( D3DXVECTOR2( static_cast<float>(iWidth / g_pKTDXApp->GetResolutionScaleX() - itemDescWidthCompare), 0.f ) );
					m_pDLGItemDescCompareAdd->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidthCompare, (float)(itemDescAddHeightComp + SIZE_Y / g_pKTDXApp->GetResolutionScaleY()) ) );
					m_pDLGItemDescCompareAdd->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );
					m_pDLGItemDescCompareAdd->GetStatic_LUA( "ItemDesc" )->GetString(0)->msg = wstrItemDescCompTemp.c_str();
					m_pDLGItemDescCompareAdd->SetShow( true );

					if ( true == bItemDescAdd )	/// 선택 아이템이 분할되었을 때
					{
						itemDescPosX = itemDescPosXCompare - itemDescWidth * 2 + 4;
					}
				}
				//{{ 비교 아이템이 분할되지 않았을 때
				else
				{
					itemDescPosXCompare = static_cast<int>( iWidth / g_pKTDXApp->GetResolutionScaleX() - itemDescWidthCompare );

					m_pDLGItemDescCompareAdd->SetShow( false );

					if ( true == bItemDescAdd )	/// 선택 아이템이 분할되었을 때
						itemDescPosX = itemDescPosXCompare - itemDescWidth * 2 + 4;
				}

				if ( false == bItemDescAdd )	/// 선택 아이템이 분할되지 않았을 때
				{
					itemDescPosX = itemDescPosXCompare - itemDescWidth + 2;
					m_pDLGItemDescAdd->SetShow( false );
				}
				//}} 위치 조정

				m_pDLGItemDescCompare->SetPos( D3DXVECTOR2( static_cast<float>(itemDescPosXCompare), static_cast<float>(itemDescPosYCompare) ) );

#else
				int itemDescPosYCompare = itemDescPosY;

				int overSize = 768 - (itemDescPosYCompare + itemDescHeightCompare);
				
				if ( overSize < 0 )
				{
					itemDescPosYCompare += overSize;
				}

				m_pDLGItemDescCompare->SetPos( D3DXVECTOR2( (float)itemDescPosX + itemDescWidth + adjustValue , (float)itemDescPosYCompare ) );
				m_pDLGItemDesc->SetPos( D3DXVECTOR2( (float)itemDescPosX + (float)adjustValue , (float)itemDescPosY ) );
#endif FIX_TOOLTIP
				m_pDLGItemDescCompare->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidthCompare, (float)itemDescHeightCompare) );
				m_pDLGItemDescCompare->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );
				m_pDLGItemDescCompare->GetStatic_LUA( "ItemDesc" )->GetString(0)->msg = itemDescCompare.c_str();
				m_pDLGItemDescCompare->SetShow( true );
			}
#ifdef FIX_TOOLTIP
			else
			{
				m_pDLGItemDescCompare->SetShow( false );
				m_pDLGItemDescCompareAdd->SetShow( false );
			}
			//}} 장착중인 아이템이 있을 경우 선택된 아이템에 대한 설명 추가			

			if ( true == bItemDescAdd )
			{
				if (itemDescPosX > iWidth / g_pKTDXApp->GetResolutionScaleX() / 2)
					itemDescPosX = static_cast<int>( iWidth / g_pKTDXApp->GetResolutionScaleX() - itemDescWidth * 2 + 2 );
				itemDescPosY = 0;
				m_pDLGItemDescAdd->SetPos(  D3DXVECTOR2( static_cast<float>(itemDescPosX + itemDescWidth) - 2, 0.f ) );
				m_pDLGItemDescAdd->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)(itemDescAddHeight + SIZE_Y / g_pKTDXApp->GetResolutionScaleY()) ));
				m_pDLGItemDescAdd->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );
				m_pDLGItemDescAdd->SetShow( true );
			}
			else
			{
				m_pDLGItemDescAdd->SetShow( false );
			}

			m_pDLGItemDesc->SetPos( D3DXVECTOR2( (float)itemDescPosX, (float)itemDescPosY ) );
#else
			else
			{
				m_pDLGItemDescCompare->SetShow( false );
			}
#endif
			m_pDLGItemDesc->GetStatic_LUA( "ItemDesc" )->GetString(0)->msg = wstrItemDesc.c_str();
			m_pDLGItemDesc->SetShow( true );
		}
	}

	if ( bCheckDrawItemDesc == false )
	{
		m_pDLGItemDesc->SetShow( false );
		m_pDLGItemDescCompare->SetShow( false );
#ifdef FIX_TOOLTIP
		m_pDLGItemDescAdd->SetShow( false );
		m_pDLGItemDescCompareAdd->SetShow( false );
#endif FIX_TOOLTIP
	}
}

void CX2ItemSlotManager::InvalidSlotDesc()
{
	SetNowOverItemSlotAndDesc( NULL );

	if ( m_pDLGItemDesc != NULL )
		m_pDLGItemDesc->SetShow( false );

	if ( m_pDLGItemDescCompare != NULL )
		m_pDLGItemDescCompare->SetShow( false );

#ifdef FIX_TOOLTIP
	if ( m_pDLGItemDescAdd != NULL )
		m_pDLGItemDescAdd->SetShow( false );

	if ( m_pDLGItemDescCompareAdd != NULL )
		m_pDLGItemDescCompareAdd->SetShow( false );
#endif FIX_TOOLTIP
}

// 08.12.15 김태완
void CX2ItemSlotManager::InvalidSelectedItem()
{
	if( m_pDLGSelectedItem != NULL )
		m_pDLGSelectedItem->SetShow(false);
}

wstring CX2ItemSlotManager::GetSlotItemDesc()
{
	KTDXPROFILE();

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
				itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
#ifdef SERV_EXCHANGE_PERIOD_ITEM
			else if( m_pNowOverItemSlot->GetPeriod() != 0 )
			{
				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
				if( pItemTemplet != NULL )
				{
					CX2Item::ItemData kItemData;
					kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
					kItemData.m_Period = m_pNowOverItemSlot->GetPeriod();
					kItemData.m_Endurance = pItemTemplet->GetEndurance();
					kItemData.m_ItemID = m_pNowOverItemSlot->GetItemTID();
					CX2Item* pkItem = new CX2Item( kItemData, NULL );
					itemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID() );
					SAFE_DELETE( pkItem );
				}
			}
#endif //SERV_EXCHANGE_PERIOD_ITEM
			else
				itemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID() );
		}

	}

	return itemDesc;
}

std::wstring CX2ItemSlotManager::GetSlotItemDescByUID( UidType itemUID, bool bItemDescExtra /* = true */ )
{
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID, true );
	if ( pItem == NULL )
		return L"";

	return GetSlotItemDescByTID( pItem, 
        pItem->GetItemTemplet()->GetItemID(), 
        bItemDescExtra, true);
}

std::wstring CX2ItemSlotManager::GetSlotItemDescByTID( CX2Item* pItem, int itemTID, bool bItemDescExtra /*= true*/, bool bMyUnitItem_ /*= false*/ )
{
	wstring slotItemDesc = L"";
#ifdef FIX_LONG_ITEM_DESCRIPTION
	WCHAR buff[1024] = {0};
#else FIX_LONG_ITEM_DESCRIPTION
	WCHAR buff[256] = {0};
#endif FIX_LONG_ITEM_DESCRIPTION
    wstring wstrBuff;

	CX2Unit* pkUnit = g_pData->GetMyUser()->GetSelectUnit();
    const CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );
    if ( pkItemTemplet == NULL )
        return  slotItemDesc;

	//{{ kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
#ifdef	SEAL_ITEM
	const CX2Item::ItemData*	pItemData		= NULL;
	if ( pItem != NULL )
		pItemData = &pItem->GetItemData();
#endif	SEAL_ITEM
	//}} kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경

	//이 부분이 들어가게 되면 상점에서 팔게 되는 아이템에서 +0이 붙게 된다
	//정책을 정해서.. 
	/*
	if( pkItemTemplet->GetItemType() == CX2Item::IT_WEAPON || pkItemTemplet->GetItemType() == CX2Item::IT_DEFENCE )
	{
		if( pkItemTemplet->GetFashion() == false )
		{
			if( pItem != NULL )
			{
				wstringstream wstrstm;
				wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" ";
				slotItemDesc += wstrstm.str().c_str();
			}
			else
			{
				wstringstream wstrstm;
				wstrstm << L"+0" << L" ";
				slotItemDesc += wstrstm.str().c_str();
			}
		}
	}
	*/

	slotItemDesc += pkItemTemplet->GetNameColor_();

	// 강화 레벨
	if ( ( pkItemTemplet->GetItemType() == CX2Item::IT_WEAPON || pkItemTemplet->GetItemType() == CX2Item::IT_DEFENCE ) &&
		pkItemTemplet->GetFashion() == false && pItem != NULL )
	{
		//kimhc // 2009-09-09 // pItem 예외처리 추가
		if( pItem != NULL )
		{
#ifdef ITEM_RECOVERY_TEST
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"+%d ", abs(pItem->GetItemData().m_EnchantLevel) );
#else
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"+%d ", pItem->GetItemData().m_EnchantLevel );
#endif
			slotItemDesc += buff;
		}
			
		/*
		if ( pkItemTemplet->GetItemType() == CX2Item::IT_WEAPON )
		{
			vector< int > vecEnchantType;
			vecEnchantType.push_back( pItem->GetItemData().m_EnchantOption1 );
			vecEnchantType.push_back( pItem->GetItemData().m_EnchantOption2 );
			wstring tempDamageName = g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ) );
			if ( tempDamageName.empty() == false )
			{
				slotItemDesc += tempDamageName;
				slotItemDesc += L" ";
			}
		}
		else
		{
			wstring tempDamageName = g_pData->GetEnchantItem()->GetEnchantResistName( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption1 );
			if ( tempDamageName.empty() == false )
			{
				slotItemDesc += tempDamageName;
				slotItemDesc += L" ";
			}
				
		}
		*/
			
	}

	// 아이템 이름

#ifdef ADDED_RELATIONSHIP_SYSTEM
	// 청첩장 아이템에 대한 예외 처리 중
	// 해당 아이템 ID 가 청첩장, 초대장 일 경우를 검사한다.
	// ItemSlotManager 의 Item UID 와 WeddingItemInfo 구조체 맵에서
	// UID 로 검색하여 WeddingItemInfo 데이터를 가져온다.
	// WeddingItemInfo 에는 신랑 아이디, 신부 아이디, 주례사 ID, 월드 ID, 일시, 편지 글귀 가 담겨 있다. 
	if ( NULL != m_pNowOverItemSlot)
	{
		if ( pkItemTemplet->GetItemID() == INVITE_WEDDING_LETTER_ITEM_ID ||
				pkItemTemplet->GetItemID() == RESERVED_WEDDING_LETTER_ITEM_ID
			)
		{
			if ( NULL != g_pData->GetRelationshipManager() )
			{
				if ( FALSE == g_pMain->GetPostBox()->GetDialogShow() )
				{
					//					CX2RelationshipManager::WeddingCoupleNickName CoupleNickName= g_pData->GetRelationshipManager()->GetWeddingInviteLetterCoupleNickNameByItemUID( m_pNowOverItemSlot->GetItemUID() );
					CX2RelationshipManager::WeddingCoupleNickName CoupleNickName = 
						g_pData->GetRelationshipManager()->GetWeddingInviteLetterCoupleNickNameByItemUID
						( pItem->GetUID() );
						
					if ( CoupleNickName.m_wstrWeddingFemaleNickname != L"" && 
							CoupleNickName.m_wstrWeddingMaleNickname != L"" )
					{
						

#ifdef SERV_RELATIONSHIP_SYSTEM_INT
						slotItemDesc = CoupleNickName.m_wstrWeddingMaleNickname + L" & " + CoupleNickName.m_wstrWeddingFemaleNickname + L" ";
#else
						// 부사를 계산, @1 등장하였습니다. ( 와 / 과 )					
						wstring tempEx = GET_STRING ( KHanSungPostWordUnicodeWrapper( ( WCHAR* )CoupleNickName.m_wstrWeddingMaleNickname.c_str(), STR_ID_453, STR_ID_454 ) );
						GET_REPLACED_STRING( ( STR_ID_25076, "LLL", slotItemDesc += CoupleNickName.m_wstrWeddingMaleNickname, tempEx, CoupleNickName.m_wstrWeddingFemaleNickname ) );
#endif SERV_RELATIONSHIP_SYSTEM_INT
					}
				}
			}
		}
	}


#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX
	slotItemDesc += pkItemTemplet->GetFullName_();
#endif // EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05	// 소켓 옵션에 따라 아이템 접두어 설정하기
	if( NULL != pItem && 
		true == pItem->GetItemData().m_bIsEvaluation &&
		false == pItem->GetItemData().m_vecRandomSocket.empty() &&
		NULL != g_pData->GetSocketItem() )
	{
		int iSocketID = pItem->GetItemData().m_vecRandomSocket.front();
#ifdef EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX
		g_pData->GetSocketItem()->GetSocketPostfix( iSocketID , slotItemDesc );
#else // EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX
		g_pData->GetSocketItem()->GetSocketPrefix( iSocketID , slotItemDesc );
#endif // EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX
	}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifndef EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX
	slotItemDesc += pkItemTemplet->GetFullName_();
#endif // EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX

	//{{ kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
#ifdef	SEAL_ITEM
	if ( pItemData != NULL &&
			pItemData->m_bIsSealed == true )
	{
		slotItemDesc += GET_STRING( STR_ID_4420 );
	}
		
#endif	SEAL_ITEM
	//}} kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경

	slotItemDesc += L"#CX\n";
        
		
	// 아이템 등급
	switch( pkItemTemplet->GetItemGrade() )
	{
	case CX2Item::IG_UNIQUE:
		{
			slotItemDesc += GET_STRING( STR_ID_257 );
		} break;
	case CX2Item::IG_ELITE:
		{
			slotItemDesc += GET_STRING( STR_ID_258 );
		} break;
	case CX2Item::IG_RARE:
		{
			slotItemDesc += GET_STRING( STR_ID_259 );
		} break;
	case CX2Item::IG_NORMAL:
		{
			slotItemDesc += GET_STRING( STR_ID_260 );
		} break;
	case CX2Item::IG_LOW:
		{
			slotItemDesc += GET_STRING( STR_ID_261 );
		} break;
	default:
		{
			// nothing
		} break;
	}

#ifdef FIELD_BOSS_RAID // 소켓 확장 여부 툴팁
	if( NULL != pItemData &&
		pItemData->m_byAddedSocketSlot > 0 )
	{
		slotItemDesc += L"  :#CFFFF33";
		slotItemDesc += GET_REPLACED_STRING( ( STR_ID_29315, "i", static_cast<int>(pItemData->m_byAddedSocketSlot) ));
	}
	slotItemDesc += L"#CX\n\n";

	if( NULL != pItemData )
	{
		bool bUsable = true;
	#ifdef ITEM_RECOVERY_TEST
		if( true == pItem->IsDisabled())
		{
			slotItemDesc += L" ";
			slotItemDesc += GET_STRING( STR_ID_262 );
			bUsable = false;
		}			
	#endif //ITEM_RECOVERY_TEST
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		if( false == pItem->GetIsEvaluation() )
		{
			slotItemDesc += L"#CFF0000";
			slotItemDesc += GET_STRING( STR_ID_4419 );	// (착용 불가)
			slotItemDesc += L"#CX";
			bUsable = false;
		}			
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
		if( false == bUsable )
		{
			slotItemDesc += L"#CX\n";
		}
	}
#else // FIELD_BOSS_RAID
	if( NULL != pItemData )
	{
	#ifdef ITEM_RECOVERY_TEST
		if( true == pItem->IsDisabled())
		{
			slotItemDesc += L" ";
			slotItemDesc += GET_STRING( STR_ID_262 );
		}			
	#endif //ITEM_RECOVERY_TEST
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		if( false == pItem->GetIsEvaluation() )
		{
			slotItemDesc += L"#CFF0000";
			slotItemDesc += GET_STRING( STR_ID_4419 );	// (착용 불가)
			slotItemDesc += L"#CX";
		}			
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
	}

	slotItemDesc += L"#CX\n\n";
#endif // FIELD_BOSS_RAID

#ifdef SERV_SKILL_NOTE
	if( pkItemTemplet->GetItemType() == CX2Item::IT_SKILL_MEMO )
	{
		const CX2Unit::UnitTemplet* pUnitTemplet = g_pData->GetUnitManager()->GetUnitTemplet( pkItemTemplet->GetUnitClass() );
		if( NULL != pUnitTemplet )
		{
			StringCchPrintfW( buff, ARRAY_SIZE(buff), 
				GET_REPLACED_STRING( ( STR_ID_4987, "L", pUnitTemplet->m_Description ) ) 
				);
			slotItemDesc += buff;
		}

		slotItemDesc += L"\n";
		wstrBuff = GET_REPLACED_STRING( ( STR_ID_279, "i", (int)pkItemTemplet->GetUseLevel() ) );
		if ( pkUnit->GetUnitData().m_Level < pkItemTemplet->GetUseLevel() ) 
		{
			slotItemDesc += L"#CFF0000";
			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		else
		{
			slotItemDesc += wstrBuff;
		}
	}		
#endif

#ifdef SERV_GOLD_TICKET_SEAL_COUNT
	if(IsGoldTicket(itemTID) == true)
	{
		if ( pItemData != NULL && pkItemTemplet->GetMaxSealCount() != 0 )
		{
			int num = pkItemTemplet->GetMaxSealCount() - pItemData->m_ucTimesToBeSealed;

			if ( pItemData->m_Period > 0 || num <= 0)
			{
				slotItemDesc += GET_STRING( STR_ID_4473 );
			}
			else
			{
#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
				if( pkItemTemplet->GetMaxSealCount() == CX2Inventory::SEAL_COUNT_MAX )
					slotItemDesc += GET_STRING( STR_ID_28121 );
				else
#endif //SERV_REMOVE_SEAL_COUNT_DECREASE
				slotItemDesc += GET_REPLACED_STRING( ( STR_ID_4423, "i", num ) );
			}
		}
	}
#endif SERV_GOLD_TICKET_SEAL_COUNT

    CX2Item::ITEM_TYPE eItemType = pkItemTemplet->GetItemType();
	if ( eItemType == CX2Item::IT_WEAPON || 
		eItemType == CX2Item::IT_DEFENCE ||
		eItemType == CX2Item::IT_ACCESSORY )
	{
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// 장착 위치
		wstring equipPos;
			
		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(pkItemTemplet->GetEqipPosition())
		{
		case CX2Unit::EP_DEFENCE_HAIR:
			{
				equipPos = GET_STRING( STR_ID_263 );
			} break;
		case CX2Unit::EP_DEFENCE_BODY:
			{
				equipPos = GET_STRING( STR_ID_264 );
			} break;
		case CX2Unit::EP_AC_BODY:
			{	
				equipPos = GET_STRING( STR_ID_265 );
			} break;
		case CX2Unit::EP_DEFENCE_LEG:
			{
				equipPos = GET_STRING( STR_ID_266 );
			} break;
		case CX2Unit::EP_AC_LEG:
			{
				equipPos = GET_STRING( STR_ID_267 );
			} break;
		case CX2Unit::EP_DEFENCE_HAND:
			{
				equipPos = GET_STRING( STR_ID_268 );
			} break;
		case CX2Unit::EP_DEFENCE_FOOT:
			{
				equipPos = GET_STRING( STR_ID_269 );
			} break;
		case CX2Unit::EP_WEAPON_HAND:
			{
				equipPos = GET_STRING( STR_ID_270 );
			} break;
		case CX2Unit::EP_AC_FACE1:
			{
				equipPos = GET_STRING( STR_ID_271 );
			} break;
		case CX2Unit::EP_AC_FACE2:
			{
				equipPos = GET_STRING( STR_ID_272 );
			} break;
		case CX2Unit::EP_AC_FACE3:
			{
				equipPos = GET_STRING( STR_ID_273 );
			} break;
		case CX2Unit::EP_AC_RING:
			{
				equipPos = GET_STRING( STR_ID_274 );
			} break;
		case CX2Unit::EP_AC_NECKLESS:
			{
				equipPos = GET_STRING( STR_ID_275 );
			} break;
		case CX2Unit::EP_AC_ARM:
			{
				equipPos = GET_STRING( STR_ID_276 );
			} break;
		case CX2Unit::EP_AC_WEAPON:
			{
				equipPos = GET_STRING( STR_ID_277 );
			} break;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		case CX2Unit::EP_ONEPIECE_FASHION:
			{
				equipPos = GET_STRING( STR_ID_28362 );
			} break;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
		default:
			break;
		}//}} elseif -> switch
						
		wstring fashionItem = L"";
		if ( pkItemTemplet->GetFashion() == true )
		{
			fashionItem = GET_STRING( STR_ID_251 );
            fashionItem += L" ";
		}

		
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// 유닛 클래스
		const CX2Unit::UnitTemplet* pUnitTemplet = g_pData->GetUnitManager()->GetUnitTemplet( pkItemTemplet->GetUnitClass() );
		if( NULL != pUnitTemplet )
		{
            StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_278, "LLL", pUnitTemplet->m_Description, fashionItem, equipPos ) ) );
		}
		else
		{
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%s%s", fashionItem.c_str(), equipPos.c_str() );
		}
			
		
		if ( pkItemTemplet->GetUnitClass() != CX2Unit::UC_NONE
			&& GetX2UnitClassCompatibility( pkUnit->GetClass(), pkItemTemplet->GetUnitClass() ) == false )
		{
			slotItemDesc += L"#CFF0000";
			slotItemDesc += buff;
			slotItemDesc += L"#CX";
		}
		else
		{
			slotItemDesc += buff;
		}

		//{{ kimhc // 2009-08-27 // 아이템 봉인 시 디스크립션 변경
#ifdef	SEAL_ITEM
		if ( pItemData != NULL 
				&& pkItemTemplet->GetMaxSealCount() != 0 )
		{
			int num = pkItemTemplet->GetMaxSealCount() - pItemData->m_ucTimesToBeSealed;

			if ( pItemData->m_Period > 0 || num <= 0)
			{
				slotItemDesc += GET_STRING( STR_ID_4473 );
			}
			else
			{					
#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
				if( pkItemTemplet->GetMaxSealCount() == CX2Inventory::SEAL_COUNT_MAX )
					slotItemDesc += GET_STRING( STR_ID_28121 );
				else
#endif //SERV_REMOVE_SEAL_COUNT_DECREASE
				slotItemDesc += GET_REPLACED_STRING( ( STR_ID_4423, "i", num ) );
			}
		}

#endif	SEAL_ITEM
		//}} kimhc // 2009-08-27 // 아이템 봉인 시 디스크립션 변경

		//{{ 허상형 : [2009/7/29] //	전직업 착용 불가 장비 아이템일 경우 (사용 불가) 추가
		if( pkItemTemplet->GetNoEquip() == true )
		{
			slotItemDesc += L"\n";
			slotItemDesc += L"#CFF0000";
			slotItemDesc += GET_STRING( STR_ID_4419 );	// (착용 불가)
			slotItemDesc += L"#CX";
		}
		//}} 허상형 : [2009/7/29] //	전직업 착용 불가 장비 아이템일 경우 (사용 불가) 추가

		// 아이템 레벨
		if ( 0 != pkItemTemplet->GetItemLevel() )
		{
			int iIncreasedLevelByEnchant = ( NULL != pItem ? pItem->GetIEchantedItemLevel() : 0 );
			slotItemDesc += L"\n";
			slotItemDesc += GET_REPLACED_STRING( (STR_ID_19229, "i", pkItemTemplet->GetItemLevel() + iIncreasedLevelByEnchant ) );
		}


		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// 스탯 관련
		slotItemDesc += L"\n";
        wstrBuff = GET_REPLACED_STRING( ( STR_ID_279, "i", (int)pkItemTemplet->GetUseLevel() ) );
		if ( pkUnit->GetUnitData().m_Level < pkItemTemplet->GetUseLevel() ) 
		{
			slotItemDesc += L"#CFF0000";
			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		else
		{
			slotItemDesc += wstrBuff;
		}

#ifdef SERV_PVP_NEW_SYSTEM
		// 랭크 구입 제한			
		if( pkItemTemplet->GetBuyPvpRankCondition() != CX2PVPEmblem::PVPRANK_NONE && NULL != g_pMain->GetPVPEmblem() )
		{ 				
#ifdef PVP_SEASON2
			WriteStringForRestrictionPurchaseToDesc( pkItemTemplet->GetBuyPvpRankCondition(), pkUnit->GetPvpRank(), slotItemDesc );
#else
			wstrBuff = GET_REPLACED_STRING( ( STR_ID_13598, "L", g_pMain->GetPVPEmblem()->GetPVPEmblemData( g_pMain->GetPVPEmblem()->GetEmblemID(pkItemTemplet->GetBuyPvpRankCondition()) )->m_EmblemName ) );
			if( pkUnit->GetPvpRank() < pkItemTemplet->GetBuyPvpRankCondition() )
			{
				slotItemDesc += L"#CFF0000";
				slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
			}
			else
			{
				slotItemDesc += wstrBuff;
			}
			slotItemDesc += L"\n\n";
#endif
				
		}
#endif

		//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		// 요구 레벨이 없고, 착용 할 수 있으면
		if ( 0 == pkItemTemplet->GetUseLevel() && false == pkItemTemplet->GetNoEquip() )
		{
			// 연동레벨을 보여줌
			slotItemDesc += GET_REPLACED_STRING( ( STR_ID_12772, "i", static_cast<int>( pkUnit->GetUnitData().m_Level ) ) );
		}
		//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
			

		if ( pkItemTemplet->GetPeriodType() == CX2Item::PT_ENDURANCE )
		{
			if ( pItem != NULL )
			{
				int endurancePer = (int)( (pItem->GetItemData().m_Endurance / (float)pkItemTemplet->GetEndurance() )  * 100.0f );

				//{{ kimhc // 실시간 엘소드 중 실시간 내구도 소모
#ifdef	REAL_TIME_ELSWORD
				if ( endurancePer == 0 && pItem->GetItemData().m_Endurance > 0 )
					endurancePer = 1;
#endif	REAL_TIME_ELSWORD
				//}} kimhc // 실시간 엘소드 중 실시간 내구도 소모

				wstrBuff = GET_REPLACED_STRING( ( STR_ID_280, "i", endurancePer ) );
				if ( endurancePer <= 10 )
				{
					slotItemDesc += L"#CFF0000";
				}
				slotItemDesc += wstrBuff;
			}
			else
			{
				slotItemDesc += GET_REPLACED_STRING( ( STR_ID_280, "i", 100 ) );
			}
		}
		slotItemDesc += L"#CX";


		bool bCheckIsEqiupping = false;

		CX2Item* pCompareItem = NULL;

		pCompareItem = GetCompareItem( pkItemTemplet, pItem, bCheckIsEqiupping );

		//임시로 만약에 능력치 보려고 하는 아이템이 
		//자기 아이템 타입의 아이템이 아니라면 장비중인걸로 표시해버리자

		if ( pkItemTemplet->GetUseCondition() != CX2Item::UC_ANYONE &&
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType != pkItemTemplet->GetUnitType() )
			bCheckIsEqiupping = true;
			
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
// 스탯테이블을 통해 스탯 계산
		// 스탯 추가 레벨 소켓 옵션 적용
		CX2SocketItem::KItemStatRelLVData kItemStatRelLVData;
		if( NULL != pItem && true == pItem->GetIsEvaluation() )
			g_pData->GetItemStatCalculator().GetSocketOptionStatRelLV(kItemStatRelLVData, pItem->GetItemData(), pkItemTemplet);

		CX2Item::KItemFormatStatData kCalculateStat;
		g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pkItemTemplet, kItemStatRelLVData);

		const CX2Item::KItemFormatStatData& kTempletStat = kCalculateStat;
#else
        const CX2Item::KItemFormatStatData& kTempletStat = pkItemTemplet->GetStatData();
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        const CX2Stat::Stat& kTempletStat = pkItemTemplet->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		if ( pItem != NULL && pItem->GetStat().m_fAtkPhysic != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_281, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)pItem->GetStat().m_fAtkPhysic > (int)pCompareItem->GetStat().m_fAtkPhysic  )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
					}
					else if ( (int)pCompareItem->GetStat().m_fAtkPhysic == (int)pItem->GetStat().m_fAtkPhysic )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_283, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
					}
					else
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
					}

				}
				else
				{
					if(pItem->GetStat().m_fAtkPhysic < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
					}						
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		else if ( kTempletStat.m_fAtkPhysic != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_281, "h", ( int )kTempletStat.m_fAtkPhysic ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{

					if ( (int)kTempletStat.m_fAtkPhysic > (int)pCompareItem->GetStat().m_fAtkPhysic  )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", ( int )kTempletStat.m_fAtkPhysic ) );

					}
					else if ( (int)pCompareItem->GetStat().m_fAtkPhysic == (int)kTempletStat.m_fAtkPhysic )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_283, "h", ( int )kTempletStat.m_fAtkPhysic ) );
					}
					else
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", ( int )kTempletStat.m_fAtkPhysic ) );
					}

				}
				else
				{
					if(kTempletStat.m_fAtkPhysic < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", ( int )kTempletStat.m_fAtkPhysic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", ( int )kTempletStat.m_fAtkPhysic ) );
					}	
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}

		if ( pItem != NULL && pItem->GetStat().m_fAtkMagic != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_285, "h", ( int )pItem->GetStat().m_fAtkMagic ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)pItem->GetStat().m_fAtkMagic > (int)pCompareItem->GetStat().m_fAtkMagic )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", ( int )pItem->GetStat().m_fAtkMagic ) );

					}
					else if ( (int)pCompareItem->GetStat().m_fAtkMagic == (int)pItem->GetStat().m_fAtkMagic )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_287, "h", ( int )pItem->GetStat().m_fAtkMagic ) );

					}
					else
					{	
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", ( int )pItem->GetStat().m_fAtkMagic ) );

					}
				}
				else
				{
					if(pItem->GetStat().m_fAtkMagic < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", ( int )pItem->GetStat().m_fAtkMagic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", ( int )pItem->GetStat().m_fAtkMagic ) );
					}						
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		else if ( kTempletStat.m_fAtkMagic != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_285, "h", ( int )kTempletStat.m_fAtkMagic ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)kTempletStat.m_fAtkMagic > (int)pCompareItem->GetStat().m_fAtkMagic )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", ( int )kTempletStat.m_fAtkMagic ) );

					}
					else if ( (int)pCompareItem->GetStat().m_fAtkMagic == (int)kTempletStat.m_fAtkMagic )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_287, "h", ( int )kTempletStat.m_fAtkMagic ) );

					}
					else
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", ( int )kTempletStat.m_fAtkMagic ) );
					}
				}
				else
				{
					if(kTempletStat.m_fAtkMagic < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", ( int )kTempletStat.m_fAtkMagic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", ( int )kTempletStat.m_fAtkMagic ) );
					}
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}

		if ( pItem != NULL && pItem->GetStat().m_fDefPhysic != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_289, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)pItem->GetStat().m_fDefPhysic > (int)pCompareItem->GetStat().m_fDefPhysic )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
					}
					else if ( (int)pCompareItem->GetStat().m_fDefPhysic == (int)pItem->GetStat().m_fDefPhysic )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_291, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
					}
					else
					{	
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
					}
				}
				else
				{
					if(pItem->GetStat().m_fDefPhysic < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
					}
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		else if ( kTempletStat.m_fDefPhysic != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_289, "h", ( int )kTempletStat.m_fDefPhysic ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)kTempletStat.m_fDefPhysic > (int)pCompareItem->GetStat().m_fDefPhysic )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", ( int )kTempletStat.m_fDefPhysic ) );

					}
					else if ( (int)pCompareItem->GetStat().m_fDefPhysic == (int)kTempletStat.m_fDefPhysic )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_291, "h", ( int )kTempletStat.m_fDefPhysic ) );

					}
					else
					{	
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", ( int )kTempletStat.m_fDefPhysic ) );
					}
				}
				else
				{
					if(kTempletStat.m_fDefPhysic < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", ( int )kTempletStat.m_fDefPhysic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", ( int )kTempletStat.m_fDefPhysic ) );
					}						
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}

		if ( pItem != NULL && pItem->GetStat().m_fDefMagic != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_293, "h", ( int )pItem->GetStat().m_fDefMagic ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)pItem->GetStat().m_fDefMagic > (int)pCompareItem->GetStat().m_fDefMagic )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", ( int )pItem->GetStat().m_fDefMagic ) );
					}
					else if ( (int)pCompareItem->GetStat().m_fDefMagic == (int)pItem->GetStat().m_fDefMagic )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_295, "h", ( int )pItem->GetStat().m_fDefMagic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", ( int )pItem->GetStat().m_fDefMagic ) );
					}
				}
				else
                {
					if(pItem->GetStat().m_fDefMagic < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", ( int )pItem->GetStat().m_fDefMagic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", ( int )pItem->GetStat().m_fDefMagic ) );
					}
                }
            }

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		else if ( kTempletStat.m_fDefMagic != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_293, "h", ( int )kTempletStat.m_fDefMagic ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)kTempletStat.m_fDefMagic > (int)pCompareItem->GetStat().m_fDefMagic )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", ( int )kTempletStat.m_fDefMagic ) );

					}
					else if ( (int)pCompareItem->GetStat().m_fDefMagic == (int)kTempletStat.m_fDefMagic )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_295, "h", ( int )kTempletStat.m_fDefMagic ) );

					}
					else
					{	
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", ( int )kTempletStat.m_fDefMagic ) );
					}
				}
				else
                {
					if(kTempletStat.m_fDefMagic < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", ( int )kTempletStat.m_fDefMagic ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", ( int )kTempletStat.m_fDefMagic ) );
					}
				}
            }

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}

		if ( pItem != NULL && pItem->GetStat().m_fBaseHP != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_297, "h", ( int )pItem->GetStat().m_fBaseHP ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)pItem->GetStat().m_fBaseHP > (int)pCompareItem->GetStat().m_fBaseHP )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", ( int )pItem->GetStat().m_fBaseHP ) );

					}
					else if ( (int)pCompareItem->GetStat().m_fBaseHP == (int)pItem->GetStat().m_fBaseHP )
					{
                        wstrBuff = GET_REPLACED_STRING( ( STR_ID_299, "h", ( int )pItem->GetStat().m_fBaseHP ) );
					}
					else
					{	
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", ( int )pItem->GetStat().m_fBaseHP ) );
					}
				}
				else
				{
					if(pItem->GetStat().m_fBaseHP < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", ( int )pItem->GetStat().m_fBaseHP ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", ( int )pItem->GetStat().m_fBaseHP ) );
					}
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		else if ( kTempletStat.m_fBaseHP != 0 )
		{
			if ( bCheckIsEqiupping == true )
			{
                wstrBuff = GET_REPLACED_STRING( ( STR_ID_297, "h", ( int )kTempletStat.m_fBaseHP ) );
			}
			else
			{
				if ( pCompareItem != NULL )
				{
					if ( (int)kTempletStat.m_fBaseHP > (int)pCompareItem->GetStat().m_fBaseHP )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", ( int )kTempletStat.m_fBaseHP ) );

					}
					else if ( (int)pCompareItem->GetStat().m_fBaseHP == (int)kTempletStat.m_fBaseHP )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_299, "h", ( int )kTempletStat.m_fBaseHP ) );

					}
					else
					{	
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", ( int )kTempletStat.m_fBaseHP ) );
					}
				}
				else
				{
					if(kTempletStat.m_fBaseHP < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", ( int )kTempletStat.m_fBaseHP ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", ( int )kTempletStat.m_fBaseHP ) );
					}
				}
			}
			
			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}

		//{{ kimhc // 2009-10-26 // 최대 MP 증가 값 비교 추가
		if ( pItem != NULL && 
            pItem->GetItemTemplet() != NULL &&
            pItem->GetItemTemplet()->GetAddMaxMP() > 0.f )
		{
			if ( bCheckIsEqiupping == true )
			{
				wstrBuff = GET_REPLACED_STRING( ( STR_ID_4630, "h", static_cast< int >( pItem->GetItemTemplet()->GetAddMaxMP() ) ) );
			}
			else
			{
				if ( pCompareItem != NULL && 
                    pCompareItem->GetItemTemplet() != NULL 
                    )
				{
					if ( static_cast< int >( pItem->GetItemTemplet()->GetAddMaxMP() ) > static_cast< int >( pCompareItem->GetItemTemplet()->GetAddMaxMP() ) )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4646, "h", static_cast< int >( pItem->GetItemTemplet()->GetAddMaxMP() ) ) );

					}
					else if ( static_cast< int >( pCompareItem->GetItemTemplet()->GetAddMaxMP() ) == static_cast< int >( pItem->GetItemTemplet()->GetAddMaxMP() ) )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4647, "h", static_cast< int >( pItem->GetItemTemplet()->GetAddMaxMP() ) ) );
					}
					else
					{	
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4648, "h", static_cast< int >( pItem->GetItemTemplet()->GetAddMaxMP() ) ) );
					}
				}
				else
				{
					if(pItem->GetStat().m_fBaseHP < 0.f)
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4648, "h", static_cast< int >( pItem->GetItemTemplet()->GetAddMaxMP() ) ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4646, "h", static_cast< int >( pItem->GetItemTemplet()->GetAddMaxMP() ) ) );
					}
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		else if ( pkItemTemplet->GetAddMaxMP() > 0.f )
		{
			if ( bCheckIsEqiupping == true )
			{
				wstrBuff = GET_REPLACED_STRING( ( STR_ID_4630, "h", static_cast< int >( pkItemTemplet->GetAddMaxMP() ) ) );
			}
			else
			{
				if ( pCompareItem != NULL && 
                    pCompareItem->GetItemTemplet() != NULL 
                    )
				{
					if ( static_cast< int >( pkItemTemplet->GetAddMaxMP() ) > static_cast< int >( pCompareItem->GetItemTemplet()->GetAddMaxMP() ) )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4646, "h", static_cast< int >( pkItemTemplet->GetAddMaxMP() ) ) );

					}
					else if ( static_cast< int >( pCompareItem->GetItemTemplet()->GetAddMaxMP() ) == static_cast< int >( pkItemTemplet->GetAddMaxMP() ) )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4647, "h", static_cast< int >( pkItemTemplet->GetAddMaxMP() ) ) );

					}
					else
					{	
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4648, "h", static_cast< int >( pkItemTemplet->GetAddMaxMP() ) ) );
					}
				}
				else
				{
					if( pkItemTemplet->GetAddMaxMP() < 0.f ) 
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4648, "h", static_cast< int >( pkItemTemplet->GetAddMaxMP() ) ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_4646, "h", static_cast< int >( pkItemTemplet->GetAddMaxMP() ) ) );
					}
				}
			}

			slotItemDesc += wstrBuff;
			slotItemDesc += L"#CX";
		}
		//}} kimhc // 2009-10-19 // 최대 MP 증가 값 비교 추가

		//{{ kimhc // 2009-10-19 // 최대 MP 증가 값 추가
		/*if ( pItem != NULL && pItem->GetItemTemplet() != NULL && pItem->GetItemTemplet()->GetAddMaxMP() > 0.0f )
		{


		}
		else *//*if ( pkItemTemplet->GetAddMaxMP() > 0.0f )
		{
			wstrBuff = GET_REPLACED_STRING( ( STR_ID_4630, "h", static_cast< int >( pkItemTemplet->GetAddMaxMP() ) ) );
			slotItemDesc += wstrBuff;
		}*/
		//}} kimhc // 2009-10-19 // 최대 MP 증가 값 추가
	}
	else
	{
		if( pkItemTemplet->GetBuyPvpRankCondition() != CX2PVPEmblem::PVPRANK_NONE )
			WriteStringForRestrictionPurchaseToDesc( pkItemTemplet->GetBuyPvpRankCondition(), pkUnit->GetPvpRank(), slotItemDesc );
	}

	slotItemDesc += L"#CX";


	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업

#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
    std::vector<int> vecSocketOption;
    pkItemTemplet->GetSocketOptions( vecSocketOption );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
    const std::vector<int>& vecSocketOption = pkItemTemplet->m_vecSocketOption;
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

	// 아이템의 소켓 레벨 정의
	// 아이템에 요구레벨이 있는 경우에는 소켓 레벨을 아이템의 요구레벨로 하고
	// 아이템에 요구레벨이 없는 경우에는 소켓 레벨을 유저(타겟유저)의 레벨 함
		
	const int iSocketLevel = 0 < pkItemTemplet->GetUseLevel() ? pkItemTemplet->GetUseLevel() : pkUnit->GetUnitData().m_Level;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 미감정 아이템 툴팁
		if( NULL != pItem && false == pItem->GetIsEvaluation() )
		{
			slotItemDesc += L"\n#C00FF00";
			slotItemDesc += GET_STRING( STR_ID_301 );
			slotItemDesc += L"#CFF0000 : ";
			slotItemDesc += GET_STRING( STR_ID_24609 );
			slotItemDesc += L"\n\n#C00FF00";
			slotItemDesc += GET_STRING( STR_ID_24610 );
			slotItemDesc += L"\n";
		}
		else if( NULL == pItem && 0 < pkItemTemplet->GetNumRandomSocketOption() )
		{
			slotItemDesc += L"\n#C00FF00";
			slotItemDesc += GET_STRING( STR_ID_301 );
			slotItemDesc += L"#CFF0000 : ";
			slotItemDesc += GET_STRING( STR_ID_24609 );
			slotItemDesc += L"\n";
		}
		else
		{
			std::vector<int> vecNormalAndRandomSocketOption;// 일반 소켓과 랜덤 옵션을 같이 닮을 컨테이너
			// 랜덤 소켓 옵션 담기
			bool bHasRandomSocket = false;
			if( NULL != pItem )
			{
				if( false == pItem->GetItemData().m_vecRandomSocket.empty() )
				{
					vecNormalAndRandomSocketOption.assign( pItem->GetItemData().m_vecRandomSocket.begin(), 
						pItem->GetItemData().m_vecRandomSocket.end() );

					bHasRandomSocket = true;
				}
			}
			// 일반 소켓 옵션 담기
			std::vector<int> vecNormalSocketOption;
			pkItemTemplet->GetSocketOptions(vecNormalSocketOption);
			vecNormalAndRandomSocketOption.insert( vecNormalAndRandomSocketOption.end(), vecNormalSocketOption.begin(), vecNormalSocketOption.end() );
				
			if( true == bHasRandomSocket )
				slotItemDesc += GetSocketDesc( vecNormalAndRandomSocketOption,
					GET_STRING( STR_ID_25081 ), iSocketLevel );
			else
				slotItemDesc += GetSocketDesc( vecNormalAndRandomSocketOption,
				GET_STRING( STR_ID_301 ), iSocketLevel );
		}
#else
		slotItemDesc += GetSocketDesc( 
		vecSocketOption, 
		GET_STRING( STR_ID_301 ), iSocketLevel );
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		
	slotItemDesc += L"#CX";


	if( NULL != pItem )
	{
		//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		slotItemDesc += GetSocketDesc( pItem->GetItemData().m_SocketOption, GET_STRING( STR_ID_302 ), iSocketLevel );
		//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
			
		slotItemDesc += L"#CX";
	}

	//{{ JHKang // 강정훈 // 2010. 10. 6 // 자동 줄나누기 두 번 실행 방지
#ifdef FIX_TOOLTIP
	slotItemDesc += GetEnchantDesc( pItem, true );
#else
	slotItemDesc += GetEnchantDesc( pItem );
#endif FIX_TOOLTIP
	//}} JHKang // 강정훈 // 2010. 10. 6 // 자동 줄나누기 두 번 실행 방지
	slotItemDesc += L"#CX";

	//스페셜 어빌리티 능력치 표시
    DWORD   dwItemID = pkItemTemplet->GetItemID();

	slotItemDesc += GetSpecialAbilityDesc( dwItemID );

#ifdef BUFF_TEMPLET_SYSTEM
	//소모 아이템에 소켓 옵션이 들어있는 경우의 툴팁 ( 비약, 영약 )
	slotItemDesc += GetSocketOptionItemDesc( dwItemID );
#endif BUFF_TEMPLET_SYSTEM

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	// 소켓을 적용할 수 있고, 착용자 연동 레벨 아이템이면
	if ( 0 == pkItemTemplet->GetUseLevel() && 
			(CX2Item::IT_WEAPON == pkItemTemplet->GetItemType() || CX2Item::IT_DEFENCE == pkItemTemplet->GetItemType() ) )
	{
		slotItemDesc += L"\n";
		slotItemDesc += GET_STRING( STR_ID_13628 );
	}			
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

	//{{ megagame : 박교현 : [2010-04-18] (마우스 오른쪽 버튼으로 사용)
    const wchar_t* pwszDescription = pkItemTemplet->GetDescription();
	if ( pwszDescription[0] != NULL )
	{
#ifdef FIX_LONG_ITEM_DESCRIPTION
		StringCchPrintf( buff, 1024, L"\n\n%s", pwszDescription );
#else FIX_LONG_ITEM_DESCRIPTION
		StringCchPrintf( buff, 256, L"\n\n%s", pwszDescription );
#endif FIX_LONG_ITEM_DESCRIPTION
		//wsprintf( buff, L"\n\n%s", pkItemTemplet->m_Description.c_str() );
		slotItemDesc += buff;
		slotItemDesc += L"\n";
	}

	if( pkItemTemplet->GetCanUseInventory() == true )
	{
		slotItemDesc += L"\n";
		slotItemDesc += GET_STRING( STR_ID_5016 );
		slotItemDesc += L"\n";
	}

	wstring tempCashItemDesc = GetSlotCashItemDesc( dwItemID );
	if ( tempCashItemDesc.empty() == false )
	{
		slotItemDesc += L"\n";
		slotItemDesc += tempCashItemDesc;
		slotItemDesc += L"\n";
	}
	//}} megagame : 박교현 : [2010-04-18]

	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( dwItemID );
	if ( pManufactureData != NULL )
	{
		slotItemDesc += GetManufactureMaterialDesc( pManufactureData );//, pkItemTemplet );
	}

	slotItemDesc += L"#CX\n";

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-16
	bool bIsSummonMonsterSlot = false;		/// 카드 소환 슬롯의 아이템인가?

	/// 카드 소환 슬롯이 적용중인지 확인
	if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
		bIsSummonMonsterSlot = g_pData->GetUIManager()->GetUIQuickSlot()->GetIsSummonCardSlot();

	if ( false == bIsSummonMonsterSlot )	/// 카드 소환 슬롯의 아이템이 아닐 때만 수량, 캐시 등의 정보 표시
#endif // SERV_NEW_DEFENCE_DUNGEON
	{
		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(pkItemTemplet->GetShopPriceType() )
		{
		case CX2Item::SPT_GP:
			{
				if ( pkItemTemplet->GetPeriodType() == CX2Item::PT_ENDURANCE && pItem != NULL )
				{
					wstrBuff = GET_REPLACED_STRING( ( STR_ID_303, "L", g_pMain->GetEDString( pItem->GetEDToSell() ) ) );
				}
				else
				{
					wstrBuff = GET_REPLACED_STRING( ( STR_ID_303, "L", g_pMain->GetEDString( ( int )( pkItemTemplet->GetPrice() * 0.2f ) ) ) );
				}
				slotItemDesc += wstrBuff;
			} break;
		case CX2Item::SPT_CASH:
			{
				wstrBuff = GET_REPLACED_STRING( ( STR_ID_304, "i", pkItemTemplet->GetPrice() ) );
				slotItemDesc += wstrBuff;
			} break;
		case CX2Item::SPT_NONE:
			{
				wstrBuff = GET_STRING( STR_ID_305 );
				slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
			} break;
		default:
			{
				ASSERT( !"Unexpected Price Type" );
			} break;
		}//}} elseif -> switch
	}
		

	//{{ kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
#ifdef	SEAL_ITEM
	if ( pItemData != NULL &&
			pItemData->m_bIsSealed == true )
	{
		slotItemDesc += GET_STRING( STR_ID_4421 );
	}
	else

#endif	SEAL_ITEM
	//}} kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
	if( pkItemTemplet->GetVested() == true ||
		( NULL != pItem &&
			pItem->GetItemData().m_Period > 0 ) )
	{
		slotItemDesc += GET_STRING( STR_ID_306 );
	}

	slotItemDesc += L"#CX";


	slotItemDesc += GetSlotCashItemPeriodDesc( dwItemID );

	slotItemDesc += GetExpirationDateDesc( pItem );		// 주의!! KTDXCommonFunc.h에 정의되어 있는것과 같은 이름이나 쓰임이 다름

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	slotItemDesc += GetSetItemDesc( pItem, dwItemID, pkUnit->GetUnitData().m_Level );
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		
	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		StringCchPrintf( buff, 256, L"\nItemID = %d\n", dwItemID );
		//wsprintf( buff, L"\nItemID = %d\n", pkItemTemplet->m_ItemID );
		slotItemDesc += buff;

#ifdef SHOW_ITEM_TYPE_AT_TOOLTIP
		wstrBuff = L"ITEM_TYPE : ";
		switch( pkItemTemplet->GetItemType() )
		{		
		case CX2Item::IT_WEAPON:	/// 무기
			{
				wstrBuff += GET_STRING(STR_ID_270);
			} break;
		case CX2Item::IT_DEFENCE:	/// 방어구
			{
				wstrBuff += GET_STRING(STR_ID_17818);
			} break;
		default:
			{
				wstrBuff = L"";
			} break;
		}
		slotItemDesc += wstrBuff ;
#endif //SHOW_ITEM_TYPE_AT_TOOLTIP

#ifdef _IN_HOUSE_
		if( NULL != pkItemTemplet )
		{
			slotItemDesc += L"ModelName :";
				
			const wchar_t* pCIMName = pkItemTemplet->GetCommonItemModelName( g_pData->GetSelectUnitType() );
			if ( NULL == pCIMName[0] )
			{
				slotItemDesc +=	pkItemTemplet->GetModelName(0);
			}
			else
			{
				slotItemDesc +=	pCIMName;
			}
			slotItemDesc += L"\n";
		}
#endif //_IN_HOUSE_

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		if( NULL != pkItemTemplet )
		{
			g_pData->GetItemStatCalculator().GetItemDescDev( slotItemDesc, pkItemTemplet );
		}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef SERV_CUBE_IN_ITEM_MAPPING

		// 맵에서 ID 추출할 부분
		std::map< int,std::map<int, kRandomItemMappingToInfoClient> >::iterator iterMappingInfo = m_mapToolTipUseOpenRandomID.find(dwItemID);

		// 여기에다 운영자 이상 아이템 디스크립션 추가할 부분
		if(iterMappingInfo != m_mapToolTipUseOpenRandomID.end())
		{
			slotItemDesc += L"\n<<CUBE_IN_ITEM_MAPPING>>";

			std::map< int, kRandomItemMappingToInfoClient>::iterator mitToInfoC = iterMappingInfo->second.begin();

			// 현재 그룹 아이디 출력
			bool bCheckMapping = false;
			int iCurrentGroupID = 0;
			for(mitToInfoC = iterMappingInfo->second.begin();mitToInfoC != iterMappingInfo->second.end(); mitToInfoC++)
			{
				kRandomItemMappingToInfoClient kInfoC = mitToInfoC->second;
				if(kInfoC.m_bCheckMapping)
				{
					bCheckMapping = true;
					iCurrentGroupID = kInfoC.m_iAfterGroupID;
					break;
				}
				else
				{
					iCurrentGroupID = kInfoC.m_iBeforeGroupID;
				}
			}
				
			if(bCheckMapping)
			{
				StringCchPrintf( buff, 256, L"#CFFFF00\nNow ID : %d #CFFFFFF", iCurrentGroupID );
				slotItemDesc += buff;
			}
			else
			{
				StringCchPrintf( buff, 256, L"\nNow ID : %d ", iCurrentGroupID );
				slotItemDesc += buff;
			}

			// 여기부터는 map 전체출력
			for(mitToInfoC = iterMappingInfo->second.begin() ;mitToInfoC != iterMappingInfo->second.end(); mitToInfoC++)
			{
				kRandomItemMappingToInfoClient kInfoC = mitToInfoC->second;
				if(kInfoC.m_bCheckMapping)
				{
					StringCchPrintf( buff, 256, L"#CFFFF00\n============================\nIndexID : %d", mitToInfoC->first );
					slotItemDesc += buff;
					slotItemDesc += L"\nChange : OK ^^!";
				}
				else
				{
					StringCchPrintf( buff, 256, L"#CFFFFFF\n============================\nIndexID : %d", mitToInfoC->first );
					slotItemDesc += buff;
					slotItemDesc += L"\nChange : NO T__T";
				}

				StringCchPrintf( buff, 256, L"\nBefore : %d", kInfoC.m_iBeforeGroupID );
				slotItemDesc += buff;
				StringCchPrintf( buff, 256, L"\nAfter : %d", kInfoC.m_iAfterGroupID );
				slotItemDesc += buff;
				slotItemDesc += L"\nStart : ";
				slotItemDesc += kInfoC.m_tStartTime;
				slotItemDesc += L"\nEnd : ";
				slotItemDesc += kInfoC.m_tEndTime;
			}
		}
#endif //SERV_CUBE_IN_ITEM_MAPPING
	}

	if( true == bItemDescExtra )
	{
		slotItemDesc += GetSlotItemDescExtra_RBtn( itemTID, pItem );
	}

	return slotItemDesc;
}

std::wstring CX2ItemSlotManager::GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem )
{
	if( NULL == pItem )
		return L"";

	std::wstring slotItemDesc = L"";
	if ( m_bShowRBDownMessage == true )
	{
		if( true == pItem->IsDisabled() )
		{
			return slotItemDesc;
		}
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		else if( false == pItem->GetIsEvaluation() )
		{
			return slotItemDesc;
		}			
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
		else
		{
            const CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );
			if( pkItemTemplet != NULL )
			{
                switch( pkItemTemplet->GetItemType() )
                {
                case CX2Item::IT_WEAPON:
                case CX2Item::IT_DEFENCE:
                case CX2Item::IT_ACCESSORY:
                case CX2Item::IT_QICK_SLOT:
				    {
					    slotItemDesc += L"\n";

			    //{{ kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
#ifdef	SEAL_ITEM
					    if ( pItem != NULL &&
						     pItem->GetItemData().m_bIsSealed == true )
					    {
						    slotItemDesc += GET_STRING( STR_ID_4422 );
					    }
					    else
#endif	SEAL_ITEM
			    //}} kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
					    slotItemDesc += GET_STRING( STR_ID_307 );
				    }
                }//switch
#ifdef SERV_GOLD_TICKET_SEAL_COUNT
				if(IsGoldTicket(itemTID) == true)
				{
					slotItemDesc += L"\n";
#ifdef	SEAL_ITEM
					if ( pItem != NULL &&
						pItem->GetItemData().m_bIsSealed == true )
					{
						slotItemDesc += GET_STRING( STR_ID_4422 );
					}
					else
#endif	SEAL_ITEM
						slotItemDesc += GET_STRING( STR_ID_307 );
				}
#endif SERV_GOLD_TICKET_SEAL_COUNT
			}
		}
	}

	return slotItemDesc;
}

wstring CX2ItemSlotManager::GetManufactureMaterialDesc( CX2ItemManager::ManufactureData* pManufactureData) //, CX2Item::ItemTemplet* pItemTemplet )
{
	wstring manufactureMaterialDesc = L"";

	if ( pManufactureData == NULL )
		return manufactureMaterialDesc;

	if ( g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return manufactureMaterialDesc;


	//manufactureMaterialDesc += L"#CX";

	//manufactureMaterialDesc += L"\n*재료 리스트*";

	//CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
	//WCHAR buff[256] = {0};
	//for ( int i = 0; i < (int)pManufactureData->m_vecMaterials.size(); i++ )
	//{
	//	CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[i];
	//	CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( materialData.m_MaterialItemID );
	//	if ( pItemTemplet == NULL )
	//		continue;

	//	int nowItemCount = min( pMyUnit->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID ), materialData.m_MaterialCount );
	//	int needItemCount = materialData.m_MaterialCount;

	//	if ( nowItemCount >= needItemCount )
	//		wsprintf( buff, L"\n%s (%d/%d)", pItemTemplet->m_Name.c_str(), nowItemCount, needItemCount );
	//	else
	//		wsprintf( buff, L"\n#CFF0000%s (%d/%d)", pItemTemplet->m_Name.c_str(), nowItemCount, needItemCount );

	//	manufactureMaterialDesc += L"#CX";
	//	manufactureMaterialDesc += buff;
	//}

	//manufactureMaterialDesc += L"\n";

	return manufactureMaterialDesc;
}




#ifdef FIX_TOOLTIP
wstring CX2ItemSlotManager::GetEnchantDesc( CX2Item* pItem, bool bTotal )
#else
wstring CX2ItemSlotManager::GetEnchantDesc( CX2Item* pItem )
#endif FIX_TOOLTIP
{
	wstring tempDesc = L"";
	if ( pItem == NULL || 
        pItem->GetItemTemplet() == NULL 
        )
		return tempDesc;

	CX2DamageManager::EXTRA_DAMAGE_TYPE eItemExtraDamageType = g_pData->GetEnchantItem()->GetExtraDamageType( pItem->GetItemData().m_EnchantedAttribute );


	if ( pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_WEAPON )
	{
#ifdef FIX_TOOLTIP
		wstring tempEnchantToolTip = g_pData->GetEnchantItem()->GetToolTip( eItemExtraDamageType, pItem, true, bTotal );
#else
		wstring tempEnchantToolTip = g_pData->GetEnchantItem()->GetToolTip( eItemExtraDamageType, pItem );
#endif FIX_TOOLTIP

		if ( tempEnchantToolTip.empty() == false )
		{
			tempEnchantToolTip = L"\n#C00FF00" + tempEnchantToolTip;

			int offsetIndex = 0;
			while( offsetIndex != -1 )
			{
				if ( offsetIndex == 0 )
				{
					tempEnchantToolTip.insert( offsetIndex, L"#C00FF00" );
				}
				else
					tempEnchantToolTip.insert( offsetIndex + 1, L"#C00FF00" );

				offsetIndex = tempEnchantToolTip.find( L"\n", offsetIndex + 9 );
			}

			tempDesc += tempEnchantToolTip;
		}

		
	}
	else if ( pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_DEFENCE )
	{
		wstring tempEnchantName = g_pData->GetEnchantItem()->GetEnchantResistToolTip( eItemExtraDamageType );

		if ( tempEnchantName.empty() == false )
		{
			tempDesc += L"\n";
			tempDesc += GET_STRING( STR_ID_308 );

			tempDesc += L"#C00FF00";
			tempDesc += tempEnchantName;
			tempDesc += L"\n";
		}
	}

	return tempDesc;
}


//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 오현빈
	/*virtual*/ wstring CX2ItemSlotManager::GetSocketDesc( const vector<int>& vecSocketOption, const wstring& wstrTitle, const int iSocketLevel_,  CX2Unit::UNIT_CLASS eUnitClass_ /*= CX2Unit::UC_NONE */)
	#else
	/*virtual*/ wstring CX2ItemSlotManager::GetSocketDesc( const vector<int>& vecSocketOption, const wstring& wstrTitle, const int iSocketLevel_ )
	#endif // SERV_NEW_ITEM_SYSTEM_2013_05
//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
{
	if( vecSocketOption.empty() )
		return L"";

	wstring tempDesc = L"";
	bool bCheckFirst = false;


	int indexForLastExtraDamage = -1;
	for ( int i = 0; i < (int)vecSocketOption.size(); i++ )
	{
		int socketOption = vecSocketOption[i];
		if( 0 == socketOption )
			continue;


		const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
		if( pSocketData == NULL )
			continue;

		if( CX2DamageManager::EDT_NONE != pSocketData->m_SocketExtraDamage.m_ExtraDamageType &&
			pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
		{
			indexForLastExtraDamage = i;
		}
	}

	

	for ( int i = 0; i < (int)vecSocketOption.size(); i++ )
	{
		int socketOption = vecSocketOption[i];
		if( 0 == socketOption )
			continue;


		const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
		if( pSocketData == NULL )
			continue;
		
		
		if ( bCheckFirst == false )
		{
			bCheckFirst = true;
			tempDesc += L"\n";
			tempDesc += L"#C00FF00";
			tempDesc += wstrTitle;
			tempDesc += L"\n";
		}

		if( CX2DamageManager::EDT_NONE != pSocketData->m_SocketExtraDamage.m_ExtraDamageType &&
			pSocketData->m_SocketExtraDamage.m_fRate > 0.0f )
		{
			if ( indexForLastExtraDamage != i )
				continue;
		}


		//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 오현빈
		wstring socketDesc = pSocketData->GetSocketDesc( iSocketLevel_, false, eUnitClass_ );
	#else
		wstring socketDesc = pSocketData->GetSocketDesc( iSocketLevel_ );
	#endif // SERV_NEW_ITEM_SYSTEM_2013_05
		//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		
#ifndef FIX_TOOLTIP
		socketDesc = g_pMain->GetStrByLienBreak( socketDesc.c_str(), 230, SLOT_MANAGER_FONT_INDEX );
#endif FIX_TOOLTIP
		int offsetIndex = 0;
		while( offsetIndex != -1 )
		{
			if ( offsetIndex == 0 )
			{
				socketDesc.insert( offsetIndex, L"#C00FF00" );
			}
			else
			{
				socketDesc.insert( offsetIndex + 1, L"#C00FF00" );
			}

			offsetIndex = socketDesc.find( L"\n", offsetIndex + 9 );
		}

		tempDesc += socketDesc;
		tempDesc += L"\n";
	}

	return tempDesc;
}




wstring CX2ItemSlotManager::GetExpirationDateDesc( CX2Item* pItem )		// 주의!! KTDXCommonFunc.h에 정의되어 있는것과 같은 이름이나 쓰임이 다름
{
	wstring tempDesc = L"";

	if ( pItem != NULL )
	{
		if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY &&
			pItem->GetItemData().m_Period > 0 )
		{
			tempDesc = GET_STRING( STR_ID_309 );
			//tempDesc = L"만료 시간 : ";

			
			CTime cTime;
			KncUtil::ConvertStringToCTime( pItem->GetItemData().m_wstrExpirationDate, cTime );

			wstringstream wstrstm;

			//wstrstm << cTime.GetYear() << L"년 " << cTime.GetMonth() << L"월 " << cTime.GetDay() << L"일 " << cTime.GetHour() << L"시 " << cTime.GetMinute() << L"분"; 
			//tempDesc += wstrstm.str().c_str();
			//tempDesc += L"\n";

			wstrstm.str( L"" );

#ifdef QUEST_REWARD_PERIOD
			if(cTime == 0 && pItem->GetItemData().m_Period > 0)
			{
				wstrstm << pItem->GetItemData().m_Period << GET_STRING( STR_ID_14 );
				tempDesc += wstrstm.str().c_str();
				tempDesc += L"\n";
				return tempDesc;
			}
#endif QUEST_REWARD_PERIOD

			//time_t t0 = time(0);
			__time64_t t0 = g_pData->GetServerCurrentTime();
			
			double fDiffTime = difftime( KncUtil::TimeToInt( cTime ), (time_t)t0 );

			int remainDay =  (int)(fDiffTime / ( 60 * 60 * 24 ) );
			if ( remainDay > 0 )
				wstrstm << remainDay << GET_STRING( STR_ID_14 ) << L" ";

			int remainHour = ((time_t)fDiffTime % ( 60 * 60 * 24 )) / (60 * 60);
			if ( remainHour < 0 )
				remainHour = 0;

			wstrstm << remainHour << GET_STRING( STR_ID_310 ) << L" ";

			int remainMinute = ((time_t)fDiffTime % ( 60 * 60 )) / (60 );
			
			if ( remainDay <= 0 )
			{
				if ( remainMinute <= 0 )
					remainMinute = 0;
				wstrstm << remainMinute << GET_STRING( STR_ID_311 ) << L" ";
			}

			tempDesc += wstrstm.str().c_str();
			

			tempDesc += L"\n";
			

		}
	}

	return tempDesc;
}

//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
/*virtual*/ wstring CX2ItemSlotManager::GetSetItemDesc( CX2Item* pItem, int itemTID, const int iUserLevel_ )
//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
{
	wstring tempDesc = L"";

    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );
	const CX2ItemManager::SetItemData* pSetItemData = NULL;
    

	if ( pItemTemplet != NULL )
	{
#ifdef IN_ITEM_KIM_USE_HIDE_SET_DESC
		if( NULL != g_pData && NULL != g_pData->GetItemManager())
		{
			if(g_pData->GetItemManager()->GetHideSetDescItem(pItemTemplet->GetItemID()) && g_pData->GetMyUser()->GetHideSetDesc() )
			{
				return tempDesc;
			}
		}
#else //IN_ITEM_KIM_USE_HIDE_SET_DESC
#ifdef HIDE_SET_DESCRIPTION
		if(pItemTemplet->GetHideSetDesc() && g_pData->GetMyUser()->GetHideSetDesc() )
		{
			return tempDesc;
		}
#endif HIDE_SET_DESCRIPTION	
#endif //IN_ITEM_KIM_USE_HIDE_SET_DESC
	
        pSetItemData = g_pData->GetItemManager()->GetSetItem( pItemTemplet->GetSetID() );
		//세트 효과 나열해볼까...


		if ( pSetItemData != NULL )
		{

			tempDesc += L"\n";


			//적용 되는 옵션 번호들을 입력해놓고 
			//맨 마지막 옵션 리스트들을 불러와서 비교해보고 적용되면 불켜주고 아니면 불꺼주고.. ㅇㅋ?
			//특정 세트아이템이 모였을 때만 적용되는 옵션들로 분리해야 할 듯.. 아 영웅형 ..ㅠㅠㅠㅠㅠㅠㅠㅠㅠ


			map< int, vector<int> > mapItemNumAndOptions;
            map< int, vector< int > >::const_iterator mit;
			vector< int > vecEffectOption;
			wstringstream wstrstm;

#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

			std::vector< int > tempVecOptions;
			std::vector< int > tempVecOptionsNoDup;
            std::vector< int > tempVecOptionsPrev;
            const CX2Item::KItemForamtNeedPartsNumAndOption* pBegin = NULL;
            const CX2Item::KItemForamtNeedPartsNumAndOption* pEnd = NULL;
            pSetItemData->GetSetItemNeedPartsNumNOptions( pBegin, pEnd );
            bool    bShowColor = false;
            DWORD equippedSetItemNum = static_cast<DWORD>(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetEqiuppingSetItemNum( pItemTemplet->GetSetID() ));
            if ( pBegin != pEnd )
            {
                DWORD dwNeedPartsNum = pBegin->m_dwNeedPartsNum;
                for( ; pBegin != pEnd; ++pBegin )
                {
                    if ( pBegin->m_dwNeedPartsNum != dwNeedPartsNum )
                    {
                        mapItemNumAndOptions.insert( std::make_pair( dwNeedPartsNum, tempVecOptionsNoDup ) ); 

                        if ( bShowColor == false 
                            && equippedSetItemNum >= dwNeedPartsNum
                            && pBegin->m_dwNeedPartsNum > equippedSetItemNum )
                        {
                            bShowColor = true;
                            vecEffectOption = tempVecOptions;
                        }
                        tempVecOptionsPrev.swap( tempVecOptions );
                        tempVecOptions.resize(0);
                        tempVecOptionsNoDup.resize(0);
                        dwNeedPartsNum = pBegin->m_dwNeedPartsNum;


                    }
                    tempVecOptions.push_back( pBegin->m_iOption );
                    if ( std::find( tempVecOptionsPrev.begin(), tempVecOptionsPrev.end(), pBegin->m_iOption )
                         == tempVecOptionsPrev.end() )
                        tempVecOptionsNoDup.push_back( pBegin->m_iOption );
                }//for
                mapItemNumAndOptions.insert( std::make_pair( dwNeedPartsNum, tempVecOptionsNoDup ) ); 
                if ( bShowColor == false
                    && equippedSetItemNum >= dwNeedPartsNum )
                {
                    bShowColor = true;
                    vecEffectOption = tempVecOptions;
                }
            }

#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

			vector< int > tempVecOptions;
			mit = pSetItemData->m_mapNeedPartsNumNOptions.begin();
			for ( ; mit != pSetItemData->m_mapNeedPartsNumNOptions.end(); mit++ )
			{
				if ( mit == pSetItemData->m_mapNeedPartsNumNOptions.begin() )
				{
					tempVecOptions = mit->second;
				}
				else
				{
					map< int, vector< int > >::const_iterator mit2 = mit;
					mit2--;
					tempVecOptions = mit->second;

					for ( int i = 0; i < (int)mit2->second.size(); i++ )
					{
						tempVecOptions.erase( std::remove( tempVecOptions.begin(), tempVecOptions.end(), mit2->second[i] ), tempVecOptions.end() );
					}
				}

				mapItemNumAndOptions.insert( std::make_pair( mit->first, tempVecOptions ) ); 
			}

			mit = pSetItemData->m_mapNeedPartsNumNOptions.begin();
			for ( ; mit != pSetItemData->m_mapNeedPartsNumNOptions.end(); mit++ )
			{
				bool bShowColor = false;

				int equippedSetItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetEqiuppingSetItemNum( pItemTemplet->GetSetID() );
				if ( equippedSetItemNum >= mit->first )
				{
					map< int, vector< int > >::const_iterator tempMit = mit;
					tempMit++;
					if ( tempMit == pSetItemData->m_mapNeedPartsNumNOptions.end() )
					{
						bShowColor = true;
					}
					else if ( tempMit->first > equippedSetItemNum )
					{
						bShowColor = true;
					}
				}

				if ( bShowColor == true )
				{

					vecEffectOption = mit->second;
					break;
				}
				
			}

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING


			if ( vecEffectOption.empty() == true )
			{
				wstrstm << GET_STRING( STR_ID_312 );
			}
			else
			{
				wstrstm << GET_STRING( STR_ID_313 );
			}


			tempDesc += wstrstm.str().c_str();


			mit = mapItemNumAndOptions.begin();
			for ( ; mit != mapItemNumAndOptions.end(); mit++ )
			{
				bool bShowColor = false;

				int equippedSetItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetEqiuppingSetItemNum( pItemTemplet->GetSetID() );
				if ( equippedSetItemNum >= mit->first )
				{
					bShowColor = true;
				}

				wstrstm.str( L"" );

				if ( bShowColor == true )
				{
                    wstrstm << L"#C00FF00-" << mit->first << GET_STRING( STR_ID_24 ) << L"\n";
				}
				else
				{
					wstrstm << L"#CAAAAAA-" << mit->first << GET_STRING( STR_ID_24 ) << L"\n";
				}

				tempDesc += wstrstm.str().c_str();

				for ( int i = 0; i < (int)mit->second.size(); i++ )
				{

					

					const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( mit->second[i] );
					if ( pSocketData != NULL )
					{
						//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
						const int iSocketLevel 
							= ( 0 < pSetItemData->m_iMaxLevel ? pSetItemData->m_iMaxLevel : iUserLevel_ );
						wstring socketDesc = pSocketData->GetSocketDesc( iSocketLevel );
						//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
						
#ifndef FIX_TOOLTIP
						socketDesc = g_pMain->GetStrByLienBreak( socketDesc.c_str(), 230, SLOT_MANAGER_FONT_INDEX );
#endif FIX_TOOLTIP

						if ( bShowColor == true )
						{
							socketDesc = L"#C00FF00*" + socketDesc;
						}
						else
						{
							socketDesc = L"#CAAAAAA*" + socketDesc;
						}

						if ( bShowColor == true )
						{
							int offsetIndex = 0;
							while( offsetIndex != -1 )
							{
								if ( offsetIndex == 0 )
								{
									socketDesc.insert( offsetIndex, L"#C00FF00" );
								}
								else
									socketDesc.insert( offsetIndex + 1, L"#C00FF00" );

								offsetIndex = socketDesc.find( L"\n", offsetIndex + 9 );
							}
						}
						else
						{
							int offsetIndex = 0;
							while( offsetIndex != -1 )
							{
								if ( offsetIndex == 0 )
								{
									socketDesc.insert( offsetIndex, L"#CAAAAAA" );
								}
								else
									socketDesc.insert( offsetIndex + 1, L"#CAAAAAA" );

								offsetIndex = socketDesc.find( L"\n", offsetIndex + 9 );
							}
						}

						tempDesc += socketDesc;
						tempDesc += L"\n";
					}
				}
			}



			//tempDesc += L"\n";

			//wstringstream wstrstm;
			//map< int, vector< int > >::iterator mit = pSetItemData->m_mapNeedPartsNumNOptions.begin();
			//for ( ; mit != pSetItemData->m_mapNeedPartsNumNOptions.end(); mit++ )
			//{
			//	bool bShowColor = false;

			//	int equippedSetItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetEqiuppingSetItemNum( pItemTemplet->m_SetID );
			//	if ( equippedSetItemNum >= mit->first )
			//	{
			//		map< int, vector< int > >::iterator tempMit = mit;
			//		tempMit++;
			//		if ( tempMit == pSetItemData->m_mapNeedPartsNumNOptions.end() )
			//		{
			//			bShowColor = true;
			//		}
			//		else if ( tempMit->first > equippedSetItemNum )
			//		{
			//			bShowColor = true;
			//		}
			//	}

			//	wstrstm.str( L"" );
			//	
			//	if ( bShowColor == true )
			//	{
			//		wstrstm << L"#C00FF99";
			//	}
			//	
			//	wstrstm << L"[" << mit->first << L"세트 효과]\n";
			//	tempDesc += wstrstm.str().c_str();
			//	
			//	for ( int i = 0; i < (int)mit->second.size(); i++ )
			//	{
			//		const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( mit->second[i] );
			//		if ( pSocketData != NULL )
			//		{
			//			wstring socketDesc = pSocketData->GetSocketDesc();
			//			socketDesc = g_pMain->GetStrByLienBreak( socketDesc.c_str(), 230, SLOT_MANAGER_FONT_INDEX );

			//			if ( bShowColor == true )
			//			{
			//				socketDesc = L"#C00FF99*" + socketDesc;
			//			}
			//			else
			//			{
			//				socketDesc = L"*" + socketDesc;
			//			}

			//			if ( bShowColor == true )
			//			{
			//				int offsetIndex = 0;
			//				while( offsetIndex != -1 )
			//				{
			//					if ( offsetIndex == 0 )
			//					{
			//						socketDesc.insert( offsetIndex, L"#C00FF99" );
			//					}
			//					else
			//						socketDesc.insert( offsetIndex + 1, L"#C00FF99" );

			//					offsetIndex = socketDesc.find( L"\n", offsetIndex + 9 );
			//				}
			//			}

			//			tempDesc += socketDesc;
			//			tempDesc += L"\n";
			//		}
			//	}
			//}




		}
	}

	if ( pItemTemplet != NULL )
	{
#ifdef IN_ITEM_KIM_USE_HIDE_SET_DESC
		if( NULL != g_pData && NULL != g_pData->GetItemManager())
		{
			if(g_pData->GetItemManager()->GetHideSetDescItem(pItemTemplet->GetItemID()) && g_pData->GetMyUser()->GetHideSetDesc() )
			{
				return tempDesc;
			}
		}
#else //IN_ITEM_KIM_USE_HIDE_SET_DESC
#ifdef HIDE_SET_DESCRIPTION
		if(pItemTemplet->GetHideSetDesc() && g_pData->GetMyUser()->GetHideSetDesc() )
		{
			return tempDesc;
		}
#endif HIDE_SET_DESCRIPTION
#endif //IN_ITEM_KIM_USE_HIDE_SET_DESC

		//세트 아이템 리스트...
		if ( pSetItemData != NULL )
		{
			//g_pData->GetItemManager()->

			tempDesc += L"\n#CFFFF00[";
			tempDesc += pSetItemData->GetSetName();
			tempDesc += L"]\n";

#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            std::vector<DWORD>  vecSetItemID;
            pSetItemData->GetSetItemList( vecSetItemID );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
			vector< int > vecSetItemID;
            g_pData->GetItemManager()->GetSetItemList( pItemTemplet->GetSetID(), vecSetItemID );
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
			for ( int i = 0; i < (int)vecSetItemID.size(); i++ )
			{
				const CX2Item::ItemTemplet* pSetItemTemplet = g_pData->GetItemManager()->GetItemTemplet( vecSetItemID[i] );
				if ( pSetItemTemplet != NULL )
				{
					if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().CheckEquippingItem( 
                        pSetItemTemplet->GetItemID()
                        ) == true )
					{
						tempDesc += L"#CFFFF00";
					}
					else
					{
						tempDesc += L"#CAAAAAA";
					}
                    tempDesc += pSetItemTemplet->GetName();
					tempDesc += L"\n";
				}
			}

			tempDesc += L"\n";
		}
	}

	return tempDesc;
}

const CX2Item::ItemTemplet* CX2ItemSlotManager::GetNowOverSlotItemTemplet()
{
	const CX2Item::ItemTemplet* pItemTemplet = NULL;

	if ( m_pNowOverItemSlot != NULL )
	{
		CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( 
			m_pNowOverItemSlot->GetItemUID() );
		if ( pkItem != NULL )
			pItemTemplet = pkItem->GetItemTemplet();
		else
			pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
	}
	
	return pItemTemplet;
}

bool CX2ItemSlotManager::GetCheckOperationCondition()
{
	return (g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckModalDlg() == false);
}

void CX2ItemSlotManager::FindSlotBySlotType( CX2SlotManager::CX2Slot::SLOT_TYPE eSlotType )
{
	m_FindedSlotList.clear();

	for ( unsigned int iS = 0; iS < m_SlotList.size(); ++iS )
	{
		CX2SlotItem* pkSlotItem = (CX2SlotItem*) GetSlot( iS );
		if ( pkSlotItem->GetSlotType() == eSlotType )
		{			
			m_FindedSlotList.push_back( pkSlotItem );
		}
	}
}

CX2SlotItem* CX2ItemSlotManager::GetSlotFindRes( int iIdx )
{
	if ( m_FindedSlotList.size() <= (unsigned int) iIdx )
		return NULL;

	return m_FindedSlotList[iIdx];
}

int CX2ItemSlotManager::GetSlotFindResCount()
{
	return m_FindedSlotList.size();
}

bool CX2ItemSlotManager::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID )
{
	if ( fromSortType == toSortType && fromSlotID == toSlotID )
		return false;

	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ;

	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType	= fromSortType;
	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= fromSlotID;
	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType	= toSortType;
	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= toSlotID;

#ifdef SERV_SHARING_BANK_TEST
	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iShareUnitUID = -1;
	if( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIPrivateBank() != NULL )
	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iShareUnitUID = g_pData->GetUIManager()->GetUIPrivateBank()->GetShareUnitUID();
	}
#endif //SERV_SHARING_BANK_TEST

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK );

	return true;
}

#ifdef NEW_VILLAGE_UI
void CX2ItemSlotManager::SetNowDragItemReturn()
{
	if ( *m_pSlotBeforeDragging != NULL )
	{
		(*m_pSlotBeforeDragging)->ResetPos();

		if ( (*m_pSlotBeforeDragging)->GetDialog() != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), (*m_pSlotBeforeDragging)->GetNormalLayer() );
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
		}
	}



	*m_pSlotBeforeDragging = NULL;
	*m_DraggingItemUID = -1;
}
#else
void CX2ItemSlotManager::SetNowDragItemReturn()
{
	if ( m_pSlotBeforeDragging != NULL )
	{
		m_pSlotBeforeDragging->ResetPos();

		if ( m_pSlotBeforeDragging->GetDialog() != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), (*m_pSlotBeforeDragging)->GetNormalLayer() );
			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
		}
	}



	m_pSlotBeforeDragging = NULL;
	m_DraggingItemUID = -1;
}

#endif

bool CX2ItemSlotManager::IsInEquippedItemSlotReact( D3DXVECTOR2 mousePos )
{
	D3DXVECTOR2 m_ConvertLeftTop		= g_pKTDXApp->ConvertByResolution( m_MyInfoSlotItemReactLeftTop );
	D3DXVECTOR2 m_ConvertRightBottom	= g_pKTDXApp->ConvertByResolution( m_MyInfoSlotItemReactRightBottom );

	if ( mousePos.x >= m_ConvertLeftTop.x && mousePos.x <= m_ConvertRightBottom.x &&
		mousePos.y >= m_ConvertLeftTop.y && mousePos.y <= m_ConvertRightBottom.y )
	{
		return true;
	}

	return false;
}

wstring CX2ItemSlotManager::GetSpecialAbilityDesc( int itemID )
{
	wstring slotDesc = L"";
	bool bCheckFirst = true;

    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
	if ( pItemTemplet != NULL )
	{
        unsigned uNumSA = pItemTemplet->GetNumSpecialAbility();
		for ( int i = 0; i < (int)uNumSA; i++ )
		{
			bool bCheck = false;
			wstringstream wstrstm;
			const CX2Item::SpecialAbility& specialAbility = pItemTemplet->GetSpecialAbility(i);
			
			//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
			switch(specialAbility.GetType())
			{
			case CX2Item::SAT_HP_UP:
				{
					//specialAbility.m_Value1
					bCheck = true;

					if ( bCheckFirst == true )
					{
						wstrstm << L"\n";
					}

					wstrstm << GET_REPLACED_STRING( ( STR_ID_314, "i", specialAbility.m_Value1 ) );
					wstrstm << GET_REPLACED_STRING( ( STR_ID_315, "i", ( int )pItemTemplet->GetCoolTime() ) );
				} break;
			case CX2Item::SAT_MP_UP:
				{
					bCheck = true;

					if ( bCheckFirst == true )
					{
						wstrstm << L"\n";

					}

					wstrstm << GET_REPLACED_STRING( ( STR_ID_316, "i", specialAbility.m_Value1 ) );
					wstrstm << GET_REPLACED_STRING( ( STR_ID_315, "i", ( int )pItemTemplet->GetCoolTime() ) );
				} break;
			case CX2Item::SAT_HP_PERCENT_UP:
				{
					bCheck = true;

					if ( bCheckFirst == true )
					{
						wstrstm << L"\n";

					}

					wstrstm << GET_REPLACED_STRING( ( STR_ID_317, "i", specialAbility.m_Value1 ) );
					wstrstm << GET_REPLACED_STRING( ( STR_ID_315, "i", ( int )pItemTemplet->GetCoolTime() ) );
				} break;
				//{{ 09.06.08 태완 : MP Percent UP
			case CX2Item::SAT_MP_PERCENT_UP:
				{
					bCheck = true;

					if ( bCheckFirst == true )
					{
						wstrstm << L"\n";

					}

					wstrstm << GET_REPLACED_STRING( ( STR_ID_2682, "i", specialAbility.m_Value1 ) );
					wstrstm << GET_REPLACED_STRING( ( STR_ID_315, "i", ( int )pItemTemplet->GetCoolTime() ) );
				} break;
				//}}
			case CX2Item::SAT_TEAM_HP_UP:
				{
					bCheck = true;
					if ( bCheckFirst == true )
					{
						wstrstm << L"\n";
					}

					wstrstm << GET_REPLACED_STRING( ( STR_ID_318, "i", specialAbility.m_Value1 ) );
					wstrstm << GET_REPLACED_STRING( ( STR_ID_315, "i", ( int )pItemTemplet->GetCoolTime() ) );
				} break;
			case CX2Item::SAT_TEAM_MP_UP:
				{
					bCheck = true;
					if ( bCheckFirst == true )
					{
						wstrstm << L"\n";
					}

					wstrstm << GET_REPLACED_STRING( ( STR_ID_319, "i", specialAbility.m_Value1 ) );
					wstrstm << GET_REPLACED_STRING( ( STR_ID_315, "i", ( int )pItemTemplet->GetCoolTime() ) );
				} break;
			default:
				break;
			}//}} elseif -> switch
			
			if ( bCheck == true )
			{
				bCheckFirst = true;
			}

			slotDesc += wstrstm.str().c_str();
		}
/*
		if ( bCheckFirst == true )
		{
			slotDesc += L"\n";
		}
		*/
	}

	return slotDesc;
}

#ifdef BUFF_TEMPLET_SYSTEM
wstring CX2ItemSlotManager::GetSocketOptionItemDesc( int itemID )
{
	wstring socketDesc = L"";

    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
	if ( pItemTemplet != NULL )
	{	
		CX2Unit*		pkUnit		= g_pData->GetMyUser()->GetSelectUnit();
		const int		iBuffID	    = (int)pItemTemplet->GetBuffID();

		//아이템의 BuffID에 해당하는 BuffTemplet을 가져옴
		CX2PremiumBuffTempletPtr ptrTemplet = g_pData->GetPremiumBuffManager()->GetPremiumBuffTempletPtr( static_cast<CX2PremiumBuffTemplet::BUFF_ID>( iBuffID ) );

		if(ptrTemplet == NULL)
			return socketDesc;

		//해당 BuffTemplet에 들어있는 SocketOption를 가져옴
		vector<int> vSocketOptionList = ptrTemplet->GetSocketOptionsList();
		
		//한칸 띄움
		socketDesc += L"\n";

		//SocketOption
		for ( int i = 0; i < (int)vSocketOptionList.size(); ++i )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( vSocketOptionList[i] );

			if( NULL == pSocketData )
				continue;

			//소켓 능력치 출력
			socketDesc += pSocketData->GetSocketDesc( pkUnit->GetUnitData().m_Level );

			socketDesc += L"\n";
		}
		//한칸 띄움
		socketDesc += L"\n";

		//지속 시간 출력
		int iActiveTime =  ( int )ptrTemplet->GetBuffPeriodSecond();
		if( iActiveTime >= 60 )
		{
			wstringstream wstrstm;
			iActiveTime /= 60;
			wstrstm << GET_REPLACED_STRING( ( STR_ID_16566, "i", iActiveTime ) ) <<  GET_STRING( STR_ID_311 );	//분
			socketDesc += wstrstm.str().c_str();
		}
		else
		{
			wstringstream wstrstm;
			wstrstm << GET_REPLACED_STRING( ( STR_ID_16566, "i", iActiveTime ) ) << GET_STRING( STR_ID_180 );	//초
			socketDesc += wstrstm.str().c_str();
		}

		//한칸 띄움
		socketDesc += L"\n";
	}
	return socketDesc;
}
#endif BUFF_TEMPLET_SYSTEM

CX2Item* CX2ItemSlotManager::GetCompareItem( IN int itemID, IN UidType itemUID, OUT bool& bCheckIsEquipping )
{
	return GetCompareItem( g_pData->GetItemManager()->GetItemTemplet( itemID ), g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID ), bCheckIsEquipping );
}

CX2Item* CX2ItemSlotManager::GetCompareItem( 
    IN const CX2Item::ItemTemplet* pkItemTemplet, 
    IN CX2Item* pItem, OUT bool& bCheckIsEquipping )
{
	bCheckIsEquipping = false;
	CX2Item* pCompareItem = NULL;
	
	//자기 종류의 아이템만 비교해불자잉~
	if ( pkItemTemplet != NULL )
	{
		if ( pkItemTemplet->GetUseCondition() != CX2Item::UC_ANYONE &&
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType != pkItemTemplet->GetUnitType() )
			return pCompareItem;
	}

	const CX2Inventory& kMyInven = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	

	{
		if ( pItem != NULL )
		{
			CX2Inventory::SORT_TYPE sortType = kMyInven.GetItemSortType( pItem->GetUID() );
			if ( sortType == CX2Inventory::ST_E_EQUIP )
			{
				bCheckIsEquipping = true;
			}
		}

		if ( pkItemTemplet != NULL )
		{
			pCompareItem = kMyInven.GetEquippingItemByEquipPos( pkItemTemplet->GetEqipPosition(), pkItemTemplet->GetFashion() );
		}
	}




	return pCompareItem;
}

bool CX2ItemSlotManager::GetIsShowCompareItemDescBySlotType( CX2Slot::SLOT_TYPE slotType )
{
	switch( slotType )
	{
	case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
	case CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND:
	case CX2Slot::ST_CASH_SHOP_SUB:
	case CX2Slot::ST_CASH_SHOP_SUB_SPECIAL:
	case CX2Slot::ST_CASH_SHOP_DEPOSIT:
		{
			return false;
		} break;
	}

	return true;
}

#ifdef FIX_TOOLTIP
void CX2ItemSlotManager::SplitDescHeight( wstring* pOrgStr, wstring* pTempStr, int &pHeight, int &pFontHeight, int fontIndex, bool bSplit )
{
	const int iHeight = DXUTGetBackBufferSurfaceDesc()->Height;
	const int SIZE_Y = 40;

	int offsetSize = 0;
	int lineNum = 0;

	if ( true == bSplit )
	{
		while( (offsetSize = pOrgStr->find( L"\n", offsetSize ) ) != -1 )
		{
			offsetSize++;
			lineNum++;
			//offsetSize = pOrgStr->find( L"\n", offsetSize );

			//{{2011.09.15 이준호 substr error check 
#ifdef PREVENT_SUBSTR_ERROR
			if( offsetSize == -1 )
				break;
#endif //PREVENT_SUBSTR_ERROR
			//}}

			if ( lineNum * pFontHeight + pFontHeight * 2 + SIZE_Y / g_pKTDXApp->GetResolutionScaleY() > iHeight / g_pKTDXApp->GetResolutionScaleY() )
			{
				wstring wstrDesc = pOrgStr->substr(0, offsetSize);
				wstring wstrDescTemp = pOrgStr->substr(offsetSize, pOrgStr->length());
				pTempStr->replace(pTempStr->begin(), pTempStr->end(), wstrDescTemp.c_str());
				pOrgStr->replace(pOrgStr->begin(), pOrgStr->end(), wstrDesc.c_str());
				break;
			}
		}
	}
	else
	{
		while( ( offsetSize = pOrgStr->find( L"\n", offsetSize ) ) != -1 )
		{
			offsetSize++;
			lineNum++;
			//offsetSize = pOrgStr->find( L"\n", offsetSize );
		}

		if (lineNum < 1)
			lineNum++;

		pHeight = static_cast<int>( lineNum * pFontHeight + SIZE_Y );
	}
}
#endif FIX_TOOLTIP

#ifdef	PVP_SEASON2
/** @function : WriteStringForRestrictionPurchaseToDesc
	@brief : 아이템 툴팁에 pvp 랭크에 따른 구매제한 스트링 표시
	@param : 아이템을 구입할 수 있는 랭크(ePvpRankConditionForPurchase_), 유닛의 랭크(eMyPvpRank_), 툴팁의 레퍼런스(& wstrSlotItemDesc_)
*/
void CX2ItemSlotManager::WriteStringForRestrictionPurchaseToDesc( const CX2PVPEmblem::PVP_RANK ePvpRankConditionForPurchase_, const CX2PVPEmblem::PVP_RANK eMyPvpRank_, wstring& wstrSlotItemDesc_ )
{
	wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_13598, "L", g_pMain->GetPVPEmblem()->GetPVPEmblemData( ePvpRankConditionForPurchase_ )->m_EmblemName ) );
	
	if ( eMyPvpRank_ < ePvpRankConditionForPurchase_ )
	{
		wstrSlotItemDesc_ += L"#CFF0000";
		wstrSlotItemDesc_ += wstrBuff;
		wstrSlotItemDesc_ += L"#CX";
	}
	else
	{
		wstrSlotItemDesc_ += wstrBuff;
	}
	wstrSlotItemDesc_ += L"\n\n";
}
#endif	PVP_SEASON2