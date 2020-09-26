#include "StdAfx.h"

//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP

#include ".\X2UIItemExchangeShop.h"

CX2UIItemExchangeShop::CX2UIItemExchangeShop( const CKTDXStage* const pNowStage, const WCHAR* const pFileName )
: CX2ItemSlotManager( const_cast< CKTDXStage* >( pNowStage ), NULL ),
m_bShow( false ),
m_pNowStage( NULL ),
m_iHouseID( CX2LocationManager::HI_INVALID ),				// X2LocationManager::HOUSE_ID
m_pDlgBackground( NULL ),									// 아이템 교환 다이얼로그
//m_pDlgSelectedSlot( NULL ),
m_vSizeDlg( 0, 0 ),
m_vPosDlg( 0, 0 ),
m_uiNowPage( 1 ),
m_uiMaxPage( 1 ),
m_uiSelectedSlotID( 0 ),
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
m_iLastCustomMsg( 0 ),
m_iLastSrcItemID( 0 ),
m_iLastSelectSrcItemUID( 0 ),
m_pDLGWarningSelectExchange( NULL ),
#endif SERV_ITEM_EXCHANGE_NEW
m_pDlgExchangeConfirm( NULL )
#ifdef SERV_EVENT_MONEY
,m_iFeatherResultQuantity ( -1 )
#endif //SERV_EVENT_MONEY
#ifdef EXCHANGE_OPEN_IMAGE
, m_iSrcItemUID( 0 )
, m_iSrcItemID( 0 )
, m_iSrcQuantity( 0 )
, m_iDestItemID( 0 )
#endif EXCHANGE_OPEN_IMAGE
{
	RegisterLuaBind();

	m_pDlgBackground	= new CKTDGUIDialog( const_cast< CKTDXStage* >( pNowStage ), pFileName );

	if ( m_pDlgBackground == NULL )
	{
		ASSERT( m_pDlgBackground != NULL );
		return;
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgBackground );
	m_pDlgBackground->SetDisableUnderWindow( true );

	m_vPosDlg.x		= m_pDlgBackground->GetDummyPos( 0 ).x;
	m_vPosDlg.y		= m_pDlgBackground->GetDummyPos( 0 ).y;

	m_vSizeDlg.x	= m_pDlgBackground->GetDummyPos( 1 ).x;
	m_vSizeDlg.y	= m_pDlgBackground->GetDummyPos( 1 ).y;

	CKTDGUIStatic*		pStaticForFirstString		=	NULL;
	pStaticForFirstString							=	
		static_cast< CKTDGUIStatic* >( m_pDlgBackground->GetControl( L"helpString1" ) );
	
	if ( pStaticForFirstString != NULL )
		pStaticForFirstString->SetString( 0, GET_STRING( STR_ID_3790 ) );

	CKTDGUIStatic*		pStaticForSecondString		=	NULL;
	pStaticForSecondString							=	
		static_cast< CKTDGUIStatic* >( m_pDlgBackground->GetControl( L"helpString2" ) );

	if ( pStaticForSecondString != NULL )
		pStaticForSecondString->SetString( 0, GET_STRING( STR_ID_3791 ) );


	CKTDGUIStatic*		pStaticSelected		=	NULL;
	pStaticSelected							=	
		static_cast< CKTDGUIStatic* >( m_pDlgBackground->GetControl( L"staticSlotSelected" ) );

	if ( pStaticSelected != NULL )
	{
		D3DXVECTOR2 vSize = pStaticSelected->GetPicture( 0 )->GetSize();
		vSize.x += 2.0f;				// 2.0f는 크기 보정 값
		vSize.y += 2.0f;

		pStaticSelected->GetPicture( 0 )->SetSize( vSize );
		pStaticSelected->SetShowEnable( false, false );
	}

	for ( int i = 0; i < _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet; i++ )
	{
		m_SlotSet[i].m_pStaticSlotSet	= 
			static_cast< CKTDGUIStatic* >( m_pDlgBackground->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"staticSlotSet" ), i + 1) ) ) );

		m_SlotSet[i].m_pButtonForExchange =
			static_cast< CKTDGUIButton* >( m_pDlgBackground->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"ButtonChange_Item" ), i + 1 ) ) ) );

		if ( m_SlotSet[i].m_pStaticSlotSet		== NULL ||
			m_SlotSet[i].m_pButtonForExchange	== NULL )
		{
			ASSERT( false );
			return;
		}

		m_SlotSet[i].m_pButtonForExchange->SetShowEnable( false, false );
	}
}

CX2UIItemExchangeShop::~CX2UIItemExchangeShop()
{

	for ( UINT i = 0; i < m_vecItemExchangeData.size(); i++ )
	{
        m_vecItemExchangeData[i].clear();
	}
    
	m_vecItemExchangeData.clear();
	//m_SlotList.clear();	// 상위 클래스에서 이미 처리됨

#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
	SAFE_DELETE_DIALOG( m_pDLGWarningSelectExchange );
#endif SERV_ITEM_EXCHANGE_NEW
	SAFE_DELETE_DIALOG( m_pDlgExchangeConfirm );
	SAFE_DELETE_DIALOG( m_pDlgBackground );
}

HRESULT		CX2UIItemExchangeShop::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();


	if ( m_pDlgBackground != NULL && m_pDlgBackground->GetIsMouseOver() == true )
	{
//		m_pDlgSelectedSlot->SetShow( true );
		return CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
	}
	else
	{
//		m_pDlgSelectedSlot->SetShow( false );
		CX2ItemSlotManager::InvalidSelectedItem();
		CX2ItemSlotManager::InvalidSlotDesc();
	}

	return S_OK;
}

bool		CX2UIItemExchangeShop::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgBackground ) )
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
			if ( m_pDlgBackground->GetIsMouseOver() == false )
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
			if ( m_pDlgBackground->GetIsMouseOver() == false )
			{
				return false;
			}

			g_pData->GetUIManager()->SetUILayerIndex( CX2UIManager::UI_MENU_ITEM_EXCHANGE_SHOP, true );
			bFlag = true;			
		}
		break;
	default:
		break;
	}

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			if ( MouseDown( mousePos) == true )
				return true;
		}
		break;

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

	return	bFlag;
}

bool		CX2UIItemExchangeShop::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case IESCM_EXIT:
		{
			SetShow( false );

			if ( g_pTFieldGame != NULL )
			{
				CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
				if ( pJoinNpc != NULL )
				{
					if ( pJoinNpc->GetNpcShop() != NULL )
						pJoinNpc->GetNpcShop()->SetKeyEvent();
				}
			}

			return true;
		}
		break;

	case IESCM_PUSH_PREV_PAGE:
		{
			if ( m_uiNowPage > 1 )
			{
				m_uiNowPage--;

				UpdatePageString();
				UpdatePageUI();
				UpdateSlotList();
			}
		}
		break;

	case IESCM_PUSH_NEXT_PAGE:
		{
			if ( m_uiNowPage < m_uiMaxPage )
			{
				m_uiNowPage++;

				UpdatePageString();
				UpdatePageUI();
				UpdateSlotList();
			}
		}
		break;

	case IESCM_FIRST_EXCHANGE_BUTTON:
	case IESCM_SECOND_EXCHANGE_BUTTON:
	case IESCM_THIRD_EXCHANGE_BUTTON:
	case IESCM_FOURTH_EXCHANGE_BUTTON:
	case IESCM_FIFTH_EXCHANGE_BUTTON:
		{
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
			// 중복 아이템 있는지, 수량성인지 여부 판단
			// eCustomMsg가 IESCM_FIRST_EXCHANGE_BUTTON 경우에 eCustomMsg는 0이 됨
			int iCustomMsg = static_cast< int >( wParam ); 
			int iIndex = 0;
			m_iLastCustomMsg = iCustomMsg; 

			iCustomMsg	-=	static_cast< int >( CX2UIItemExchangeShop::IESCM_FIRST_EXCHANGE_BUTTON );
			iIndex		=	( m_uiNowPage - 1 ) * _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet;
			iIndex		+=	iCustomMsg;
			std::vector< CX2ItemManager::ItemExchangeData >& vecItemExchangeData = m_vecItemExchangeData[iIndex];

			m_iLastSrcItemID = vecItemExchangeData[0].m_iSrcItemID;

			int iItemCount = SearchItemInUserInventory( m_iLastSrcItemID );

			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_iLastSrcItemID );
			if ( pItemTemplet != NULL )
			{
				if ( pItemTemplet->GetPeriodType() != CX2Item::PT_QUANTITY && iItemCount > 1)
				{
					CX2Inventory::SORT_TYPE eSortType = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetSortTypeByItemTemplet(pItemTemplet);
					CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
					CX2Cursor*	pCursor = pState->GetCursor();

					g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
					g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( eSortType );		
					pCursor->ChangeCursorState( CX2Cursor::XCS_ITEM_EXCHANGE );
#ifdef FIX_WARNING_SELECT_EXCHANGE
					if( CX2Cursor::XCS_NORMAL != pCursor->GetCurorState() )
					{
						PlayWarningSelectExchange();
					}
					else
					{
						StopWarningSelectExchange();
						InvalidLastSelectExchange();
					}
#else
					PlayWarningSelectExchange();
#endif //FIX_WARNING_SELECT_EXCHANGE
				}
				else if(pItemTemplet->GetPeriodType() != CX2Item::PT_QUANTITY && iItemCount == 1)
				{
					// 오현빈 // 2013-08-21 // 장착 중인 장비는 포함하지 않도록 변경
					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID(m_iLastSrcItemID, false, false);
					if(pItem != NULL)
					{
						m_iLastSelectSrcItemUID = pItem->GetUID();
						PushedExchangeButton( static_cast< int >( wParam ) );
					}
				}
				else
				{
					m_iLastSelectSrcItemUID = 0;
					PushedExchangeButton( static_cast< int >( wParam ) );
				}
			}
#else
			PushedExchangeButton( static_cast< int >( wParam ) );
#endif SERV_ITEM_EXCHANGE_NEW
		}
		break;

	case IESCM_OK:
		{
			int iSrcItemID	=	0;
			int iDestItemID	=	0;
			iSrcItemID		=	m_pDlgExchangeConfirm->GetDummyInt( 0 );

#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
			UidType iSrcItemUID	= 0;
			int iSrcQuantity = 0;
			iSrcItemUID = m_iLastSelectSrcItemUID;
			iSrcQuantity = m_pDlgExchangeConfirm->GetDummyInt( 1 );
#endif SERV_ITEM_EXCHANGE_NEW

			if ( m_pDlgExchangeConfirm != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgExchangeConfirm, NULL, false );
				m_pDlgExchangeConfirm = NULL;
			}

			if ( m_uiSelectedSlotID != 0 )
			{
				CX2SlotItem* pSlotItem = NULL;
				pSlotItem	= static_cast< CX2SlotItem * >( GetSlot( CX2Slot::ST_INVENTORY, m_uiSelectedSlotID ) );

				if ( pSlotItem == NULL )
				{
					ASSERT( false );
					return false;
				}

				iDestItemID = pSlotItem->GetItemTID();
			}
#ifdef SERV_EVENT_MONEY
			//EGS_UPDATE_EVENT_MONEY_NOT의 순서를 보장 할 수 없기 때문에 깃털 개수 따로 기록(UI 갱신 목적)
			m_iFeatherResultQuantity = -1;
			if( ANGELS_FEATHER_ITEM_ID == iSrcItemID )
			{
				if( NULL != g_pData && NULL != g_pData->GetMyUser() )
				{	//교환 후의 깃털 개수 저장
					m_iFeatherResultQuantity = g_pData->GetMyUser()->GetUserAngelsFeather() - iSrcQuantity;
				}
			}
#endif //SERV_EVENT_MONEY

#ifdef EXCHANGE_OPEN_IMAGE
			if( g_pData->GetCubeOpenImageManager() != NULL &&
				g_pData->GetCubeOpenImageManager()->IsEventCube( iSrcItemID ) == true &&
				g_pData->GetUIManager()->GetUIInventory() != NULL )
			{
				g_pData->GetUIManager()->GetUIInventory()->OpenRandomItem( iSrcItemID, CX2UIInventory::IORT_RANDOM_EXCHANGE );

				m_iSrcItemUID	= iSrcItemUID;
				m_iSrcItemID	= iSrcItemID;
				m_iSrcQuantity	= iSrcQuantity;
				m_iDestItemID	= iDestItemID;
			}
			else
#endif // EXCHANGE_OPEN_IMAGE
#ifdef SERV_ITEM_EXCHANGE_NEW  // 디파인 잘 못 두른 것 해외팀 수정
			Handler_EGS_ITEM_EXCHANGE_REQ( iSrcItemUID, iSrcItemID, iSrcQuantity, iDestItemID);
#else
			Handler_EGS_ITEM_EXCHANGE_REQ( iSrcItemID, iDestItemID );
#endif SERV_ITEM_EXCHANGE_NEW
		}
		break;

	case IESCM_CANCEL:
		{
			if ( m_pDlgExchangeConfirm != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgExchangeConfirm, NULL, false );
				m_pDlgExchangeConfirm = NULL;
			}
			if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
#ifdef FIX_WARNING_SELECT_EXCHANGE
			InvalidLastSelectExchange();
#endif //FIX_WARNING_SELECT_EXCHANGE
		}
		break;

	default:
		break;
	}

	return	false;
}

bool		CX2UIItemExchangeShop::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_ITEM_EXCHANGE_ACK:
		{
			return Handler_EGS_ITEM_EXCHANGE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	}

	return	false;
}

bool		CX2UIItemExchangeShop::GetShow() const
{
	return	m_bShow;
}

void		CX2UIItemExchangeShop::SetShow( bool bShow )
{
	m_bShow	= bShow;

	if ( bShow )
	{
		if ( InitItemExchangeData() == false	||
			 InitNowPageAndMaxPage() == false	||
			 UpdatePageString()	== false )
		{
			ASSERT( false );
		}

		UpdatePageUI();
		UpdateSlotList();

		g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_MENU_ITEM_EXCHANGE_SHOP );
	}
	else
	{
		g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_MENU_ITEM_EXCHANGE_SHOP );

		for ( UINT i = 0; i < m_vecItemExchangeData.size(); i++ )
		{
			m_vecItemExchangeData[i].clear();
		}

		m_vecItemExchangeData.clear();
		LostItemSlotList();
		CX2ItemSlotManager::InvalidSelectedItem();
		CX2ItemSlotManager::InvalidSlotDesc();
	}
#ifdef SERV_ITEM_EXCHANGE_NEW  // 디파인 잘 못 두른 것 해외팀 수정
	InvalidLastSelectExchange();
#endif SERV_ITEM_EXCHANGE_NEW
	m_pDlgBackground->SetShowEnable( m_bShow, m_bShow );
}

D3DXVECTOR2		CX2UIItemExchangeShop::GetDLGSize() const
{
	return m_vSizeDlg;
}

void		CX2UIItemExchangeShop::SetPosition( D3DXVECTOR2 vec )
{
	m_vPosDlg		= vec;
}

void	CX2UIItemExchangeShop::SetLayer( X2_DIALOG_LAYER layer )
{
	//if ( m_pDlgSelectedSlot != NULL )
	//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgSelectedSlot, GetSelectedItemLayer() ); 

	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);

	if( m_pDlgBackground != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgBackground, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgBackground, false );
	}
}

void	CX2UIItemExchangeShop::SetHouseID( int iHouseID )
{
	m_iHouseID		= iHouseID;
}

//bool	CX2UIItemExchangeShop::CompareVector( std::vector< CX2ItemManager::ItemExchangeData >& vFirst, std::vector< CX2ItemManager::ItemExchangeData >& vSecond )
//{
//	if ( vFirst[0].m_iSrcItemID > vSecond[0].m_iSrcItemID )
//		return true;
//	else
//		return false;
//}

bool	CX2UIItemExchangeShop::InitItemExchangeData()
{
	std::vector< CX2ItemManager::ItemExchangeData > vecTempItemExchangeData;
	std::vector < std::vector< CX2ItemManager::ItemExchangeData > > vecItemExchangeDataNotHave;

	if ( g_pData->GetItemManager()->GetItemExchangeData( m_iHouseID, vecTempItemExchangeData ) == true )
	{
		int		iItemID			=	-1;
		CX2ItemManager::ITEM_EXCHANGE_TYPE	eType	= CX2ItemManager::IET_INVALID;
#ifdef MODIFY_ITEM_EXCHANGE_TABLE
		int		iQuantity		=	-1;
#endif //MODIFY_ITEM_EXCHANGE_TABLE

		BOOST_TEST_FOREACH( CX2ItemManager::ItemExchangeData&, tempItemExchangeData, vecTempItemExchangeData )
		{
#ifdef EXCHANGE_ITEM_SORT_FIX
			int iCount = SearchItemInUserInventory( tempItemExchangeData.m_iSrcItemID );
			if ( iCount == -1 )
#else //EXCHANGE_ITEM_SORT_FIX
			// 유저가 가지고 있는 것과 가지고 있지 않는 것을 구별해서 vector에 넣어줌
			if ( SearchItemInUserInventory( tempItemExchangeData.m_iSrcItemID ) == -1 )
#endif //EXCHANGE_ITEM_SORT_FIX
			{
				// 안가지고 있음!
				if ( tempItemExchangeData.m_iSrcItemID != iItemID || tempItemExchangeData.m_eExchangeType != eType 
#ifdef MODIFY_ITEM_EXCHANGE_TABLE
					|| tempItemExchangeData.m_iSrcQuantity != iQuantity 
#endif //MODIFY_ITEM_EXCHANGE_TABLE
					)
				{
					// kimhc // 2010-11-12 // warning!! // 만약 m_eExchangeType이 순서대로 되있지 않으면 오동작 하게 되어있음.. 수정요망
					vecItemExchangeDataNotHave.push_back( std::vector< CX2ItemManager::ItemExchangeData >() );
					iItemID		=	tempItemExchangeData.m_iSrcItemID;
					eType		=	tempItemExchangeData.m_eExchangeType;
#ifdef MODIFY_ITEM_EXCHANGE_TABLE
					iQuantity	=	tempItemExchangeData.m_iSrcQuantity;
#endif //MODIFY_ITEM_EXCHANGE_TABLE
				}
				vecItemExchangeDataNotHave.rbegin()->push_back( tempItemExchangeData );
			}
			else
			{
#ifdef EXCHANGE_ITEM_SORT_FIX
				if( iCount >= tempItemExchangeData.m_iSrcQuantity ) //교환 가능한 숫자인지 체크
				{
#endif //EXCHANGE_ITEM_SORT_FIX
				// 가지고 있음!
				if ( tempItemExchangeData.m_iSrcItemID != iItemID || tempItemExchangeData.m_eExchangeType != eType
#ifdef MODIFY_ITEM_EXCHANGE_TABLE
					|| tempItemExchangeData.m_iSrcQuantity != iQuantity 
#endif //MODIFY_ITEM_EXCHANGE_TABLE
					)
				{
					// kimhc // 2010-11-12 // warning!! // 만약 m_eExchangeType이 순서대로 되있지 않으면 오동작 하게 되어있음.. 수정요망
					m_vecItemExchangeData.push_back( std::vector< CX2ItemManager::ItemExchangeData >() );
					iItemID		=	tempItemExchangeData.m_iSrcItemID;
					eType		=	tempItemExchangeData.m_eExchangeType;
#ifdef MODIFY_ITEM_EXCHANGE_TABLE
					iQuantity	=	tempItemExchangeData.m_iSrcQuantity;
#endif //MODIFY_ITEM_EXCHANGE_TABLE
				}

				m_vecItemExchangeData.rbegin()->push_back( tempItemExchangeData );
#ifdef EXCHANGE_ITEM_SORT_FIX
				}
				else //가지고 있지만 교환 가능하지 않은 숫자.
				{
					if ( tempItemExchangeData.m_iSrcItemID != iItemID || tempItemExchangeData.m_eExchangeType != eType 
#ifdef MODIFY_ITEM_EXCHANGE_TABLE
						|| tempItemExchangeData.m_iSrcQuantity != iQuantity 
#endif //MODIFY_ITEM_EXCHANGE_TABLE
						)
					{
						// kimhc // 2010-11-12 // warning!! // 만약 m_eExchangeType이 순서대로 되있지 않으면 오동작 하게 되어있음.. 수정요망
						vecItemExchangeDataNotHave.push_back( std::vector< CX2ItemManager::ItemExchangeData >() );
						iItemID		=	tempItemExchangeData.m_iSrcItemID;
						eType		=	tempItemExchangeData.m_eExchangeType;
#ifdef MODIFY_ITEM_EXCHANGE_TABLE
						iQuantity	=	tempItemExchangeData.m_iSrcQuantity;
#endif //MODIFY_ITEM_EXCHANGE_TABLE
					}
					vecItemExchangeDataNotHave.rbegin()->push_back( tempItemExchangeData );
				}
#endif //EXCHANGE_ITEM_SORT_FIX
			}
		}

// 		int iVecExchangeDataSize			= 0;
// 		int iVecExchangeDataNotHaveSize		= 0;
// 
// 		iVecExchangeDataSize				= m_vecItemExchangeData.size();
// 		iVecExchangeDataNotHaveSize			= vecItemExchangeDataNotHave.size();

		// 가지고 있는 것들 뒤에 안가지고 있는 것들을 복사

		if ( false == vecItemExchangeDataNotHave.empty() )
			m_vecItemExchangeData.insert( m_vecItemExchangeData.end(), vecItemExchangeDataNotHave.begin(), vecItemExchangeDataNotHave.begin() + vecItemExchangeDataNotHave.size() );

		//m_vecItemExchangeData.resize( iVecExchangeDataSize + iVecExchangeDataNotHaveSize, vecItemExchangeDataNotHave );
		
// 		std::copy( vecItemExchangeDataNotHave.begin(), 
// 			vecItemExchangeDataNotHave.begin() + iVecExchangeDataNotHaveSize, 
// 			m_vecItemExchangeData.begin() + iVecExchangeDataSize );

		vecItemExchangeDataNotHave.clear();
		vecTempItemExchangeData.clear();
		return true;
	}
	
	return false;
}

bool	CX2UIItemExchangeShop::InitNowPageAndMaxPage()
{

	if ( m_vecItemExchangeData.size() == 0 )
	{
		ASSERT( false );
		return false;
	}


	m_uiMaxPage = m_vecItemExchangeData.size()	/	_CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet;

	if ( ( m_vecItemExchangeData.size()	%	_CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet ) != 0 ) // 나머지가 있으면
	{
		m_uiMaxPage += 1;
	}
	m_uiNowPage	= 1;

	return true;
}

bool	CX2UIItemExchangeShop::UpdatePageString()
{
	if ( m_pDlgBackground != NULL )
	{
		CKTDGUIStatic* pStatic = static_cast< CKTDGUIStatic* >( m_pDlgBackground->GetControl( L"Page_Num" ) );

		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << m_uiNowPage
				<< L"/" << m_uiMaxPage;
			pStatic->SetString( 0, wstrstm.str().c_str() );

			return true;
		}
	}

	ASSERT( false );
	return false;
}

bool	CX2UIItemExchangeShop::UpdatePageUI()
{
	for ( int i = 0; i < _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet; i++ )
	{
		UpdateSlotSetUI( m_SlotSet[i].m_pStaticSlotSet, CX2ItemManager::IET_INVALID );
		m_SlotSet[i].m_pButtonForExchange->SetShowEnable( false, false );
		m_SlotSet[i].m_bUserHave	= false;
		m_SlotSet[i].m_eType		= 0;
		m_SlotSet[i].m_iSrcItemID	= -1;
	}

	UINT	 uiStartIndex	= 0;
	UINT	 uiEndIndex		= 0;

	uiStartIndex	=	( m_uiNowPage - 1 ) * _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet;
	
	if ( uiStartIndex + _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet < m_vecItemExchangeData.size() )
	{
		uiEndIndex = uiStartIndex + _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet;
	}
	else
	{
		uiEndIndex = m_vecItemExchangeData.size();
	}

	for ( UINT i = uiStartIndex; i < uiEndIndex; i++ )
	{
		CX2ItemManager::ItemExchangeData itemExchangeData = m_vecItemExchangeData[i][0];

		UpdateSlotSetUI( m_SlotSet[i - uiStartIndex].m_pStaticSlotSet, itemExchangeData.m_eExchangeType );	// i-uiStarIndex는 0
		m_SlotSet[i - uiStartIndex].m_eType = static_cast< int >( itemExchangeData.m_eExchangeType );
	}

	return true;
}

bool	CX2UIItemExchangeShop::UpdateSlotSetUI( CKTDGUIStatic* pStaticSlotSet, CX2ItemManager::ITEM_EXCHANGE_TYPE eType )
{

	switch ( eType )
	{
	case CX2ItemManager::IET_ALL:
	case CX2ItemManager::IET_RANDOM:
	case CX2ItemManager::IET_SELECT:
		{
			for	( int i = 0; i < pStaticSlotSet->GetPictureNum(); i++ )
			{
				D3DXCOLOR color( 1.0f, 1.0f, 1.0f, 1.0f );
				pStaticSlotSet->GetPicture( i )->SetColor( color );
			}

			pStaticSlotSet->GetPicture( 6 )->SetColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.7f ) ); // All, Select 등의 글자 뒤에 있는 texture
		}
		break;

	default:
		{
			for	( int i = 0; i < pStaticSlotSet->GetPictureNum(); i++ )
			{
				D3DXCOLOR color( 1.0f, 1.0f, 1.0f, 0.6f );
				pStaticSlotSet->GetPicture( i )->SetColor( color );
			}

			pStaticSlotSet->GetPicture( 6 )->SetColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.3f ) ); // All, Select 등의 글자 뒤에 있는 texture
		}
		break;
	}

	switch ( eType )
	{
	case CX2ItemManager::IET_ALL:
		{
			pStaticSlotSet->GetPicture( 3 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_all_left" );
			pStaticSlotSet->GetPicture( 4 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_all_center" );
			pStaticSlotSet->GetPicture( 5 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_all_right");
			pStaticSlotSet->SetString( 0, GET_STRING( STR_ID_3793 ) );
			pStaticSlotSet->GetString( 0 )->color = D3DXCOLOR( 0.0f, 0.5f, 1.0f, 1.0f );
		}
		break;

	case CX2ItemManager::IET_RANDOM:
		{
			pStaticSlotSet->GetPicture( 3 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_random_left" );
			pStaticSlotSet->GetPicture( 4 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_random_center" );
			pStaticSlotSet->GetPicture( 5 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_random_right");
			pStaticSlotSet->SetString( 0, GET_STRING( STR_ID_3794 ) );
			pStaticSlotSet->GetString( 0 )->color = D3DXCOLOR( 1.0f, 0.2f, 0.2f, 1.0f );
		}
		break;

	case CX2ItemManager::IET_SELECT:
		{
			pStaticSlotSet->GetPicture( 3 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_select_left" );
			pStaticSlotSet->GetPicture( 4 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_select_center" );
			pStaticSlotSet->GetPicture( 5 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_select_right");
			pStaticSlotSet->SetString( 0, GET_STRING( STR_ID_3795 ) );
			pStaticSlotSet->GetString( 0 )->color = D3DXCOLOR( 0.2f, 0.8f, 0.2f, 1.0f );
		}
		break;

	default:
		{
			pStaticSlotSet->GetPicture( 3 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_normal_left" );
			pStaticSlotSet->GetPicture( 4 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_normal_center" );
			pStaticSlotSet->GetPicture( 5 )->SetTex( L"DLG_UI_Common_Texture10.TGA", L"slot_normal_right");
			pStaticSlotSet->SetString( 0, L"" );
		}
		break;
	}

	return true;
}

bool	CX2UIItemExchangeShop::UpdateSlotList()
{
	LostItemSlotList();

	/// g_iNumOfSlotInSlotSet 은 자신의 아이템슬롯 을 제외한 갯수이기 때문에 + 1을 함
	const	int	numOfSlotInOneSet = _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotInSlotSet + 1;		// SlotSet 한줄 당 6개의 아이템슬롯 존재(0, 6, 12, 18, 24)번은 교환할 ItemSlot

	UINT uiStartIndex		= 0;
	UINT uiEndIndex			= 0;
	UINT uiOffset			= 0;

	uiOffset				=	( m_uiNowPage - 1 ) * _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet;
	uiStartIndex			= uiOffset;

	if ( uiStartIndex + _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet < m_vecItemExchangeData.size() )
	{
		uiEndIndex	= uiStartIndex + _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet;
	}
	else
	{
		uiEndIndex	= m_vecItemExchangeData.size();
	}

	CX2SlotItem*	pSlotItem	= NULL;
	int				iItemCount	= -1;

	for ( UINT	i = uiStartIndex; i < uiEndIndex; i++ )
	{
			pSlotItem =	static_cast< CX2SlotItem * >( GetSlot( ( i - uiOffset ) * numOfSlotInOneSet ) );

		if ( pSlotItem != NULL )
		{
			CX2ItemManager::ItemExchangeData& myItemExchangeData = m_vecItemExchangeData[i][0];
			iItemCount		=	SearchItemInUserInventory( myItemExchangeData.m_iSrcItemID );


#ifdef SERV_ITEM_EXCHANGE_NEW  // 디파인 잘 못 두른 것 해외팀 수정
            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( myItemExchangeData.m_iSrcItemID );
			int iMaterialItemCount = iItemCount;
			if( iMaterialItemCount == -1 )
				iMaterialItemCount = 0;
			pSlotItem->DestroyItemUI();
			pSlotItem->CreateMaterialItemUI( pItemTemplet, iMaterialItemCount, myItemExchangeData.m_iSrcQuantity );
#else
			if ( pSlotItem->IsCreatedItemUI() == true )
				pSlotItem->ResetItemUI( myItemExchangeData.m_iSrcItemID, iItemCount, true, true );
			else
				pSlotItem->CreateItemUI( myItemExchangeData.m_iSrcItemID, iItemCount, true, true );
#endif SERV_ITEM_EXCHANGE_NEW

			pSlotItem->SetEnable( true );
			pSlotItem->SetShow( true );
			pSlotItem->SetClickable( false );

#ifdef SERV_ITEM_EXCHANGE_NEW  // 디파인 잘 못 두른 것 해외팀 수정
			if ( iMaterialItemCount < myItemExchangeData.m_iSrcQuantity )
			{
				m_SlotSet[i - uiOffset].m_bUserHave = false;
				m_SlotSet[i - uiOffset].m_pButtonForExchange->SetShowEnable( true, false );
			}
			else
			{
				m_SlotSet[i - uiOffset].m_bUserHave = true;
				m_SlotSet[i - uiOffset].m_pButtonForExchange->SetShowEnable( true, true );
			}
#else
			if ( iItemCount == -1 )
			{
				pSlotItem->SetShowImpossiblePicture( true );
				m_SlotSet[i - uiOffset].m_bUserHave = false;
				m_SlotSet[i - uiOffset].m_pButtonForExchange->SetShowEnable( true, false );
			}
			else
			{
				pSlotItem->SetShowImpossiblePicture( false );
				m_SlotSet[i - uiOffset].m_bUserHave = true;
				m_SlotSet[i - uiOffset].m_pButtonForExchange->SetShowEnable( true, true );
			}
#endif SERV_ITEM_EXCHANGE_NEW
			m_SlotSet[i - uiOffset].m_iSrcItemID = myItemExchangeData.m_iSrcItemID;

			pSlotItem = NULL;
		}
		else
		{
			ASSERT( false );
			return false;
		}

	}

	for ( UINT	i = uiStartIndex; i < uiEndIndex; i++ )
	{
		int index = 1;

		BOOST_TEST_FOREACH( CX2ItemManager::ItemExchangeData&, itemExchangeData, m_vecItemExchangeData[i] )
		{
			CX2SlotItem* pSlotItem		=		
				static_cast< CX2SlotItem * >( GetSlot( ( i - uiOffset ) * numOfSlotInOneSet + index ) );		// 교환될 slot의 ID는 1~5, 11~15, 21~25, 31~35, 41~45 이다.
			
			if ( pSlotItem != NULL )
			{
				// 유저가 가지고 있으면(m_bUserHave == true), false를 넣기 위해 ! 연산자 사용
				if ( pSlotItem->IsCreatedItemUI() == true )
					pSlotItem->ResetItemUI( itemExchangeData.m_iDestItemID, itemExchangeData.m_iQuantity, true );
				else
					pSlotItem->CreateItemUI( itemExchangeData.m_iDestItemID, itemExchangeData.m_iQuantity, true );

				pSlotItem->SetShowImpossiblePicture( !(m_SlotSet[i - uiOffset].m_bUserHave) );

				if ( itemExchangeData.m_eExchangeType == CX2ItemManager::IET_SELECT && 
					m_SlotSet[i - uiOffset].m_bUserHave == true )
				{
					pSlotItem->SetClickable( true );
				}
				else
				{
					pSlotItem->SetClickable( false );
				}

#ifdef SERV_EXCHANGE_PERIOD_ITEM
				pSlotItem->SetPeriod( static_cast< int >( itemExchangeData.m_sPeriod ) );
#endif //SERV_EXCHANGE_PERIOD_ITEM

				pSlotItem->SetEnable( true );
				pSlotItem->SetShow( true );
				
			}
			else
			{
				ASSERT( false );
				return false;
			}

			index++;
		}
	}

	return true;
}

void	CX2UIItemExchangeShop::LostItemSlotList()
{
	for ( UINT i = 0; i < m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = static_cast< CX2SlotItem* >( m_SlotList[i] );
		if ( pSlotItem != NULL )
		{
			pSlotItem->LostItemUI();
			pSlotItem->SetShow( false );
			pSlotItem->SetEnable( false, false );
		}
	}
}

bool	CX2UIItemExchangeShop::MouseDown( D3DXVECTOR2 mousePos )
{

	// 10은 한 라인의 슬롯셋에 속해 있는 슬롯과 다음 라인의 슬롯셋에 속해있는 슬롯의 ID 차이 값
	if ( m_SlotSet[ m_uiSelectedSlotID / 10].m_pButtonForExchange->GetMouseOver() == true )	
		return true;

	CX2SlotItem* pSlot = NULL;
	
	pSlot	= (CX2SlotItem*)GetSlotInMousePos( mousePos );

	CKTDGUIStatic*		pStaticSelected		=	NULL;
	pStaticSelected							=	
		static_cast< CKTDGUIStatic* >( m_pDlgBackground->GetControl( L"staticSlotSelected" ) );

	if ( pStaticSelected == NULL )
	{
		m_uiSelectedSlotID	= 0;
		return false;
	}

	if ( pSlot == NULL )
	{
		m_uiSelectedSlotID	= 0;
		pStaticSelected->SetShowEnable( false, false );
		return false;
	}

	if ( pSlot->GetDialog() == NULL )
	{
		m_uiSelectedSlotID	= 0;
		pStaticSelected->SetShowEnable( false, false );
		return false;
	}

	if( false == pSlot->GetClickable() )
	{
		m_uiSelectedSlotID	= 0;
		pStaticSelected->SetShowEnable( false, false );
		return false;
	}

	m_uiSelectedSlotID	= pSlot->GetSlotID();
	D3DXVECTOR2 vPos	= pSlot->GetPos();
	vPos.x -= 3.0f;		// 3.0f는 위치 보정값
	vPos.y -= 3.0f;

	
	pStaticSelected->GetPicture( 0 )->SetPos( vPos );
	pStaticSelected->SetShowEnable( true, true );

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );

	return true;
}

//void		CX2UIItemExchangeShop::DestoryUIItemExchangeShop()
//{
//	for ( UINT i = 0; i < m_vecItemExchangeData.size(); i++ )
//	{
//		m_vecItemExchangeData[i].clear();
//	}
//
//	m_vecItemExchangeData.clear();
//
//	for ( UINT i = 0; i < m_SlotList.size(); i++ )
//	{
//		CX2SlotItem* pSlotItem = static_cast< CX2SlotItem* >( m_SlotList[i] );
//		if ( pSlotItem != NULL )
//		{
//			pSlotItem->SetShow( false );
//			pSlotItem->SetEnable( false, false );
//			pSlotItem->DestroyItemUI();
//		}
//	}
//	m_SlotList.clear();
//
//	SAFE_DELETE_DIALOG( m_pDlgExchangeConfirm );
//	SAFE_DELETE_DIALOG( m_pDlgBackground );
//}

int			CX2UIItemExchangeShop::SearchItemInUserInventory( int iItemID )
{
	int				iCount				= 0;

#ifdef SERV_EVENT_MONEY
	if( ANGELS_FEATHER_ITEM_ID == iItemID )
	{
		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{  //천사의 깃털은 예외적으로 인벤토리가 아닌 유저객체에서 얻어오도록 수정
			iCount = g_pData->GetMyUser()->GetUserAngelsFeather();
		}
	}
	else
	{
		iCount				= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iItemID, true );
	}
#else

	iCount				= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iItemID, true );
#endif //SERV_EVENT_MONEY

	if ( iCount == 0 )	// 장착중인 장비 제외
		return -1;
	else
		return iCount;
}

#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
bool		CX2UIItemExchangeShop::PushedExchangeButton( int iCustomMsg, bool bPopupCenter /*= false*/ )
#else
bool		CX2UIItemExchangeShop::PushedExchangeButton( int iCustomMsg )
#endif SERV_ITEM_EXCHANGE_NEW
{

	
	std::wstringstream		wstrStream;
	int						iIndex			= 0;

	// eCustomMsg가 IESCM_FIRST_EXCHANGE_BUTTON 경우에 eCustomMsg는 0이 됨
	iCustomMsg	-=	static_cast< int >( CX2UIItemExchangeShop::IESCM_FIRST_EXCHANGE_BUTTON );
	iIndex		=	( m_uiNowPage - 1 ) * _CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet;
	iIndex		+=	iCustomMsg;

	std::vector< CX2ItemManager::ItemExchangeData >& vecItemExchangeData = m_vecItemExchangeData[iIndex];
	

	switch ( vecItemExchangeData[0].m_eExchangeType )
	{
	case CX2ItemManager::IET_ALL:
		{
            wstrStream << GET_REPLACED_STRING( ( STR_ID_3827, "S", GetItemFullName_( vecItemExchangeData[0].m_iSrcItemID ) ) );
		
			UINT i;
			for ( i = 0; i < vecItemExchangeData.size() - 1; i++ )
			{
                wstrStream << GET_REPLACED_STRING( ( STR_ID_3828, "S", GetItemFullName_( vecItemExchangeData[i].m_iDestItemID ) ) );
				wstrStream << L", ";
			}
            wstrStream << GET_REPLACED_STRING( ( STR_ID_3828, "S", GetItemFullName_( vecItemExchangeData[i].m_iDestItemID ) ) );
			wstrStream << GET_STRING( STR_ID_3829 );	
		}
		break;

	case CX2ItemManager::IET_RANDOM:
		{
            wstrStream << GET_REPLACED_STRING( ( STR_ID_3827, "S", GetItemFullName_( vecItemExchangeData[0].m_iSrcItemID ) ) );

			UINT i;
			for ( i = 0; i < vecItemExchangeData.size() - 1; i++ )
			{
                wstrStream << GET_REPLACED_STRING( ( STR_ID_3828, "S", GetItemFullName_( vecItemExchangeData[i].m_iDestItemID ) ) );
				wstrStream << L", ";
			}
            wstrStream << GET_REPLACED_STRING( ( STR_ID_3828, "S", GetItemFullName_( vecItemExchangeData[i].m_iDestItemID ) ) );
			wstrStream << GET_STRING( STR_ID_3830 );	
		}
		break;

	case CX2ItemManager::IET_SELECT:
		{
			if ( m_uiSelectedSlotID != 0 )
			{
				CX2SlotItem* pSlotItem = NULL;
				pSlotItem	= static_cast< CX2SlotItem * >( GetSlot( CX2Slot::ST_INVENTORY, m_uiSelectedSlotID ) );

				if ( pSlotItem == NULL )
				{
					ASSERT( false );
					return false;
				}
				wstrStream << GET_REPLACED_STRING( ( STR_ID_3827, "S", GetItemFullName_( vecItemExchangeData[0].m_iSrcItemID ) ) );
				wstrStream << GET_REPLACED_STRING( ( STR_ID_3828, "S", GetItemFullName_( pSlotItem->GetItemTID() ) ) );
				
				wstrStream << GET_STRING( STR_ID_3831 );	
			}
			else
			{
#ifdef FIX_WARNING_SELECT_EXCHANGE
				InvalidLastSelectExchange();
#endif //FIX_WARNING_SELECT_EXCHANGE

#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
				if(bPopupCenter == true)
					g_pMain->KTDGUIOKMsgBox(  D3DXVECTOR2(250,300), GET_STRING( STR_ID_3832 ), g_pMain->GetNowState() );
				else
#endif SERV_ITEM_EXCHANGE_NEW
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_3832 ), g_pMain->GetNowState() );
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
				return false;
#else
				return true;
#endif SERV_ITEM_EXCHANGE_NEW
			}
		}
		break;

	default:
		{
			ASSERT( false );
			return false;
		}
		break;
	}
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
	if(bPopupCenter == true)
		m_pDlgExchangeConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrStream.str().c_str(), IESCM_OK, 
		g_pMain->GetNowState(), IESCM_CANCEL );
	else
#endif SERV_ITEM_EXCHANGE_NEW
	m_pDlgExchangeConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), wstrStream.str().c_str(), IESCM_OK, 
		g_pMain->GetNowState(), IESCM_CANCEL );

	m_pDlgExchangeConfirm->AddDummyInt( vecItemExchangeData[0].m_iSrcItemID );
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
	m_pDlgExchangeConfirm->AddDummyInt( vecItemExchangeData[0].m_iSrcQuantity );
#endif SERV_ITEM_EXCHANGE_NEW

	return true;

}

const wchar_t* CX2UIItemExchangeShop::GetItemFullName_( int iItemID )
{
	const CX2Item::ItemTemplet*	pItemTemplet =
	    g_pData->GetItemManager()->GetItemTemplet( iItemID );
	if ( pItemTemplet == NULL )
	{
        return  L"";
	}
	return pItemTemplet->GetFullName_();
}
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
bool		CX2UIItemExchangeShop::Handler_EGS_ITEM_EXCHANGE_REQ( UidType iSrcItemUID, int iSrcItemID, int iSrcQuantity, int iSelectedItemID /* = 0 */ )
{
	KEGS_ITEM_EXCHANGE_REQ kPacket;

	kPacket.m_iSourceItemID			= iSrcItemID;
	kPacket.m_iSourceItemUID		= iSrcItemUID;
	kPacket.m_iSourceQuantity		= iSrcQuantity;
	kPacket.m_iHouseID				= m_iHouseID;
	kPacket.m_iDestItemID			= iSelectedItemID;

	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_EXCHANGE_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_ITEM_EXCHANGE_ACK, 60.f );
	InvalidLastSelectExchange();

	return true;
}
#endif SERV_ITEM_EXCHANGE_NEW
/////////////////////////////////////////////////////////////////
//////// 핸들러
/////////////////////////////////////////////////////////////////
bool		CX2UIItemExchangeShop::Handler_EGS_ITEM_EXCHANGE_REQ( int iItemID, int iSelectedItemID /* = 0 */ )
{
	KEGS_ITEM_EXCHANGE_REQ kPacket;

	kPacket.m_iSourceItemID			= iItemID;
	kPacket.m_iHouseID				= m_iHouseID;
	kPacket.m_iDestItemID			= iSelectedItemID;

	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_EXCHANGE_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_ITEM_EXCHANGE_ACK, 60.f );

	return true;
}
bool		CX2UIItemExchangeShop::Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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

#ifdef SERV_EVENT_MONEY
			//EGS_UPDATE_EVENT_MONEY_NOT의 순서를 보장 할 수 없기 때문에 깃털 개수 따로 설정
			if( -1 != m_iFeatherResultQuantity )
			{
				if( NULL != g_pData && NULL != g_pData->GetMyUser() )
				{
					int iFeatherQuantity = g_pData->GetMyUser()->GetUserAngelsFeather();
					if( m_iFeatherResultQuantity != iFeatherQuantity )
					{
						g_pData->GetMyUser()->SetUserAngelsFeather(m_iFeatherResultQuantity);
					}
				}
				m_iFeatherResultQuantity = -1;
			}
#endif //SERV_EVENT_MONEY
			UpdatePageString();
			UpdatePageUI();
			UpdateSlotList();

			return true;
		}
	}


	return false;
}
#ifdef SERV_ITEM_EXCHANGE_NEW
bool CX2UIItemExchangeShop::SelectExchangeSrcItem( UidType iSrcItemUID )
{
	m_iLastSelectSrcItemUID = iSrcItemUID;
	return PushedExchangeButton( m_iLastCustomMsg, true );
}
bool CX2UIItemExchangeShop::CheckExchangeUseItem( int itemID )
{
	if(m_iLastSrcItemID == itemID)
	{
		return true;
	}
	return false;
}
void CX2UIItemExchangeShop::InvalidLastSrcItem()
{
	m_iLastSrcItemID = 0;
	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
	return;
}

void CX2UIItemExchangeShop::InvalidLastSelectExchange()
{
	m_iLastSelectSrcItemUID = 0;
	m_iLastSrcItemID = 0;
	m_iLastCustomMsg = 0;
	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
	return;
}

void CX2UIItemExchangeShop::PlayWarningSelectExchange()
{
	if( m_pDLGWarningSelectExchange == NULL )
	{
		m_pDLGWarningSelectExchange	= new CKTDGUIDialog( NULL, L"DLG_UI_Warning_Select_Exchange.lua", 0.0f );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWarningSelectExchange );
		m_pDLGWarningSelectExchange->SetColor( D3DXCOLOR(1,1,1,0) );
	}

	if ( m_pDLGWarningSelectExchange != NULL )
	{
		m_pDLGWarningSelectExchange->SetShowEnable(true, true);

		CKTDGUIStatic* pStatic_WarningSelectExchange = (CKTDGUIStatic*) m_pDLGWarningSelectExchange->GetControl( L"WarningSelectExchange" );
		if( pStatic_WarningSelectExchange != NULL && pStatic_WarningSelectExchange->GetPicture(2) != NULL )
			pStatic_WarningSelectExchange->GetPicture(2)->SetShow(false);

		if ( m_pDLGWarningSelectExchange->GetIsMoving() == false )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGWarningSelectExchange, true );
			m_pDLGWarningSelectExchange->Move( m_pDLGWarningSelectExchange->GetPos(), D3DXCOLOR(1,1,1,1), 0.5f, true );
			m_pDLGWarningSelectExchange->Move( m_pDLGWarningSelectExchange->GetPos(), D3DXCOLOR(1,1,1,1), 2.0f );
			m_pDLGWarningSelectExchange->Move( m_pDLGWarningSelectExchange->GetPos(), D3DXCOLOR(1,1,1,0), 1.0f );
			m_pDLGWarningSelectExchange->Move( m_pDLGWarningSelectExchange->GetPos(), D3DXCOLOR(1,1,1,0), 4.0f );
		}
	}
}

void CX2UIItemExchangeShop::StopWarningSelectExchange()
{
	if ( m_pDLGWarningSelectExchange != NULL )
	{
		m_pDLGWarningSelectExchange->SetShowEnable(false, false);
		m_pDLGWarningSelectExchange->MoveStop();
		m_pDLGWarningSelectExchange->SetColor( D3DXCOLOR( 1,1,1,0 ) );
	}
}
#endif SERV_ITEM_EXCHANGE_NEW
#endif	ITEM_EXCHANGE_SHOP
//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업