#include "StdAfx.h"
#ifdef DEF_TRADE_BOARD // oasis907 : 김상윤 [2010.3.12] // 

#include ".\X2UIPersonalShopBoard.h"
#include < iomanip >

CX2UIPersonalShopBoard::CX2UIPersonalShopBoard(CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2ItemSlotManager( pNowStage, NULL )
, m_bShow( false )
, m_bOldApplyDelete( false )
, m_bRegMyGuildAd( false )
, m_pNowStage( pNowStage )
, m_pDlgPersonalShopBoard( NULL )
, m_uiNowPage( 1 )
, m_uiMaxPage( 0 )
, m_uiFirstPageinCurrentBoard( 1 )
, m_iComboFirstindex( 0 )
, m_iComboSecondindex( 0 )
, m_iComboCharacterindex( 0 )
, m_iComboClassindex( 0 )
, m_iIMEEditMinLevel( 0 )
, m_iIMEEditMaxLevel( 50 )
, m_fCurrentTime( 0.f )
, m_fLastSearchTime( 0.f )
, m_vPosDlg( 0, 0 )
, m_vSizeDlg( 0, 0 )
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
, m_pListboxSearchKeyword( NULL )
, m_iCloseSearchKeywordListTime( 0 )
, m_iFocusOutSearchKeywordBoxTime( 0 )
, m_iFocusInSearchKeywordBoxTime( 0 )
, m_fActiveSearchKeywordListTime( 0 )
, m_bIsStrictSearchOption( false )
#endif //SERV_UPGRADE_TRADE_SYSTEM
{
	RegisterLuaBind();


	m_pDlgPersonalShopBoard = new CKTDGUIDialog( const_cast< CKTDXStage* >( pNowStage ), pFileName );
	if ( m_pDlgPersonalShopBoard == NULL )
	{
		ASSERT( false );
		return;
	}
 
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgPersonalShopBoard );
	m_pDlgPersonalShopBoard->SetDisableUnderWindow( true );

	m_vPosDlg.x			= m_pDlgPersonalShopBoard->GetDummyPos( 0 ).x;		// 0은 posion
	m_vPosDlg.y			= m_pDlgPersonalShopBoard->GetDummyPos( 0 ).y;
	
	m_vSizeDlg.x		= m_pDlgPersonalShopBoard->GetDummyPos( 1 ).x;		// 1은 size
	m_vSizeDlg.y		= m_pDlgPersonalShopBoard->GetDummyPos( 1 ).y;




	m_pButtonPrevBoard = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( L"ButtonPrevBoard" ) );
	m_pButtonPrevPage = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( L"ButtonPrevPage" ) );

	m_pButtonNextPage = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( L"ButtonNextPage" ) );
	m_pButtonNextBoard = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( L"ButtonNextBoard" ) );


	m_pButtonSearchItem = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( L"ButtonSearchItem" ) );
	m_pButtonSearchItem->SetEnable(false);

#ifndef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	m_pButtonRefresh = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( L"ButtonRefresh" ) );
#endif //SERV_UPGRADE_TRADE_SYSTEM

	m_pButtonInitSearchCondition = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( L"ButtonInitSearchCondition" ) );
	//m_pButtonBuyItem = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( L"ButtonBuyItem" ) );


	m_pComboFirst = static_cast< CKTDGUIComboBox* >( m_pDlgPersonalShopBoard->GetControl( L"ComboFirst" ) );



	m_pComboSecond = static_cast< CKTDGUIComboBox* >( m_pDlgPersonalShopBoard->GetControl( L"ComboSecond" ) );



	m_pComboCharacter = static_cast< CKTDGUIComboBox* >( m_pDlgPersonalShopBoard->GetControl( L"ComboCharacter" ) );



	m_pComboClass = static_cast< CKTDGUIComboBox* >( m_pDlgPersonalShopBoard->GetControl( L"ComboClass" ) );

	m_pCheckUnique = static_cast< CKTDGUICheckBox* >( m_pDlgPersonalShopBoard->GetControl( L"CheckUnique" ) );
	m_pCheckElite = static_cast< CKTDGUICheckBox* >( m_pDlgPersonalShopBoard->GetControl( L"CheckElite" ) );
	m_pCheckRare = static_cast< CKTDGUICheckBox* >( m_pDlgPersonalShopBoard->GetControl( L"CheckRare" ) );
	m_pCheckNormal = static_cast< CKTDGUICheckBox* >( m_pDlgPersonalShopBoard->GetControl( L"CheckNormal" ) );
	m_pCheckLow = static_cast< CKTDGUICheckBox* >( m_pDlgPersonalShopBoard->GetControl( L"CheckLow" ) );
	m_pCheckUsable = static_cast< CKTDGUICheckBox* >( m_pDlgPersonalShopBoard->GetControl( L"CheckUsable" ) );
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	m_pCheckALL = static_cast< CKTDGUICheckBox* >( m_pDlgPersonalShopBoard->GetControl( L"CheckAll" ) );
#endif //SERV_UPGRADE_TRADE_SYSTEM

	m_pIMEEditItemName = static_cast< CKTDGUIIMEEditBox* >( m_pDlgPersonalShopBoard->GetControl( L"IMEEditItemName" ) );
	m_pIMEEditMinLevel = static_cast< CKTDGUIIMEEditBox* >( m_pDlgPersonalShopBoard->GetControl( L"IMEEditMinLevel" ) );
	m_pIMEEditMaxLevel = static_cast< CKTDGUIIMEEditBox* >( m_pDlgPersonalShopBoard->GetControl( L"IMEEditMaxLevel" ) );

	m_pStaticPageNum = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl( L"StaticPageNum" ) );

	if ( m_pStaticPageNum != NULL )
	{
		m_pStaticPageNum->SetString( 0, L"1" );
		m_pStaticPageNum->SetString( 1, L"2" );
		m_pStaticPageNum->SetString( 2, L"3" );		
		m_pStaticPageNum->SetString( 3, L"4" );				
		m_pStaticPageNum->SetString( 4, L"5" );		
		m_pStaticPageNum->SetString( 5, L"6" );		
		m_pStaticPageNum->SetString( 6, L"7" );	
		m_pStaticPageNum->SetString( 7, L"8" );
		m_pStaticPageNum->SetString( 8, L"9" );
		m_pStaticPageNum->SetString( 9, L"10" );
	}

	for ( int i = 0; i < _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard; i++ )
	{
		m_pButtonPageNum[i] = static_cast< CKTDGUIButton* >( m_pDlgPersonalShopBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"ButtonPageNum" ), i + 1 ) ) ) );
		m_pButtonPageNum[i]->SetShowEnable(false, false);
	}

	for ( int i = 0; i < _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage; i++ )
	{
		m_pItemList[i] = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Personal_Shop_Board_ItemList.lua" );
		D3DXVECTOR3 tempPos1 = m_pDlgPersonalShopBoard->GetDummyPos(i+2);
		D3DXVECTOR3 tempPos2 = m_pDlgPersonalShopBoard->GetPos();
		m_pItemList[i]->SetPos_LUA(tempPos1.x + tempPos2.x, tempPos1.y + tempPos2.y);

		CKTDGUIButton *pButton = static_cast< CKTDGUIButton* >( m_pItemList[i]->GetControl( L"ButtonBuyItem" ) );
		pButton->AddDummyInt(i);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pItemList[i] );
		//m_pItemList[i]->SetLayer( XDL_GAME_EDIT );
		m_pItemList[i]->SetShowEnable(false, false);
	}


	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		pItemSlot->LostItemUI();
		pItemSlot->SetShow( false );
		pItemSlot->SetEnable( false );
	}



	m_iSelectedPersonalShopUid = -1;

	UpdateItemList();
	InitSearchCondition();

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	/// 자동 완성창 객체 생성 및 설정
	m_pDLGAutoSearchKeyword			= new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Personal_Shop_Board_Auto_Search_Keyword.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAutoSearchKeyword );	

	if ( NULL != m_pDLGAutoSearchKeyword )
	{
		m_pDLGAutoSearchKeyword->SetShow( true );

		m_pListboxSearchKeyword		= static_cast<CKTDGUIListBox*>( m_pDLGAutoSearchKeyword->GetControl( L"ListboxSearchKeyword" ) );
	}

	m_pStrictSearchOption			= static_cast< CKTDGUICheckBox* >( m_pDlgPersonalShopBoard->GetControl( L"CheckStrictSearchOption" ) );

	SetShowSearchKeywordList( false );

	/// 개인 상점 객체 생성
	if ( NULL != g_pData &&
		NULL != g_pData->GetUIManager() )
		g_pData->GetUIManager()->CreateUIPersonalShop();

	/// 검색어 자동 완성 창에 나오면 않되는 아이템 리스트 설정
	OpenScriptFile( L"PersonalShopAutoSearchBanList.lua" );
#endif //SERV_UPGRADE_TRADE_SYSTEM
}

CX2UIPersonalShopBoard::~CX2UIPersonalShopBoard()
{
	SAFE_DELETE_DIALOG( m_pDlgPersonalShopBoard );

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	/// 검색어 자동 완성창 삭제
	SAFE_DELETE_DIALOG( m_pDLGAutoSearchKeyword );

	if ( NULL != g_pData &&
		 NULL != g_pData->GetUIManager() )
		g_pData->GetUIManager()->DestroyPeronalShopUI();
#endif //SERV_UPGRADE_TRADE_SYSTEM
}

void CX2UIPersonalShopBoard::SetIMMEditMinLevel(int iMinLevel)
{
#ifdef SERV_NEW_EVENT_TYPES
	int iMaxEXP = g_pData->GetEXPTable()->GetEXPData( _CONST_X2GAME_::g_iMaxLevel ).m_nTotalExp;
	if( g_pInstanceData != NULL )
		iMaxEXP = g_pData->GetEXPTable()->GetEXPData( g_pInstanceData->GetMaxLevel() ).m_nTotalExp;
#else SERV_NEW_EVENT_TYPES
	const int iMaxEXP = g_pData->GetEXPTable()->GetEXPData( _CONST_X2GAME_::g_iMaxLevel ).m_nTotalExp;
#endif SERV_NEW_EVENT_TYPES

	if(iMinLevel < 1)
	{
		iMinLevel = 1;
	}
#ifdef SERV_NEW_EVENT_TYPES
	else if( g_pInstanceData != NULL )
	{
		if ( iMinLevel > g_pInstanceData->GetMaxLevel() )
			iMinLevel = g_pInstanceData->GetMaxLevel();
	}
#endif SERV_NEW_EVENT_TYPES
	else if(iMinLevel > _CONST_X2GAME_::g_iMaxLevel )	// 해외팀 변경 LIMIT_MAX_LEVEL
	{
		iMinLevel = _CONST_X2GAME_::g_iMaxLevel;		// 해외팀 변경 LIMIT_MAX_LEVEL
	}
	
	WCHAR buff[256] = {0};
	_itow( iMinLevel, buff, 10 );
	m_pIMEEditMinLevel->SetText(buff, false);


	if(iMinLevel == 1)
	{
		m_iIMEEditMinLevel = 0;
	}
	else
	{
		m_iIMEEditMinLevel = iMinLevel;
	}
	return;
}

void CX2UIPersonalShopBoard::SetIMMEditMaxLevel(int iMaxLevel)
{
	if(iMaxLevel < 1)
	{
		iMaxLevel = 1;
	}
#ifdef SERV_NEW_EVENT_TYPES
	else if( g_pInstanceData != NULL )
	{
		if ( iMaxLevel > g_pInstanceData->GetMaxLevel() )
			iMaxLevel = g_pInstanceData->GetMaxLevel();
	}
#endif SERV_NEW_EVENT_TYPES
	else if(iMaxLevel > _CONST_X2GAME_::g_iMaxLevel )	// 해외팀 변경 LIMIT_MAX_LEVEL
	{
		iMaxLevel = _CONST_X2GAME_::g_iMaxLevel;		// 해외팀 변경 LIMIT_MAX_LEVEL
	}

	WCHAR buff[256] = {0};
	_itow( iMaxLevel, buff, 10 );
	m_pIMEEditMaxLevel->SetText(buff, false);

	if(iMaxLevel == 1)
	{
		m_iIMEEditMaxLevel = 0;
	}
	else
	{
		m_iIMEEditMaxLevel = iMaxLevel;
	}
	return;
}

HRESULT	CX2UIPersonalShopBoard::OnFrameMove( double fTime, float fElapsedTime )
{	
	UpdateButtonSearchItem();

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	/// 검색어 자동 완성창 닫기까지 대기 시간 처리 ( 프레임 단위 )
	if ( 0 < m_iCloseSearchKeywordListTime )
	{
		--m_iCloseSearchKeywordListTime;

		if ( 0 >= m_iCloseSearchKeywordListTime )
		{
			m_iCloseSearchKeywordListTime = 0;

			/// 검색어 자동 완성창 숨겨 주세요
			SetShowSearchKeywordList( false );
		}
	}

	/// 검색어 입력창 포커스 이탈시, 관련 처리까지의 대기 시간 ( 프레임 단위 )
	if ( 0 < m_iFocusOutSearchKeywordBoxTime )
	{
		--m_iFocusOutSearchKeywordBoxTime;

		if ( 0 >= m_iFocusOutSearchKeywordBoxTime )
		{
			m_iFocusOutSearchKeywordBoxTime = 0;

			/// 검색어 자동 완성창 숨겨 주세요
			SetFocusOutSearchKeyword();
		}
	}

	/// 검색어 입력창 포커스 이탈시, 관련 처리까지의 대기 시간 ( 프레임 단위 )
	if ( 0 < m_iFocusInSearchKeywordBoxTime )
	{
		--m_iFocusInSearchKeywordBoxTime;

		if ( 0 >= m_iFocusInSearchKeywordBoxTime )
		{
			m_iFocusInSearchKeywordBoxTime = 0;

			if ( NULL != m_pIMEEditItemName )
				m_pIMEEditItemName->RequestFocus();
		}
	}

	/// 검색어 자동 완성창 동작 간격 ( 초 단위 )
	if ( 0 < m_fActiveSearchKeywordListTime )
	{
		m_fActiveSearchKeywordListTime -= fElapsedTime;

		if ( 0 >= m_fActiveSearchKeywordListTime )
		{
			m_fActiveSearchKeywordListTime = 0;

			/// 자동 완성 최소 쿨타임 지난 후, 검색창에 포커스가 있을 때, 동작
			if ( NULL != m_pIMEEditItemName && true == m_pIMEEditItemName->CanHaveFocus() )
			{	
				UpdateSearchKewordList( m_pIMEEditItemName->GetNowText() );
			}
		}
	}
#endif //SERV_UPGRADE_TRADE_SYSTEM

	if(m_pDlgPersonalShopBoard != NULL && m_pDlgPersonalShopBoard->GetIsMouseOver() == true)
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

bool CX2UIPersonalShopBoard::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgPersonalShopBoard ) )
		return false;

	bool bFlag	= false;

	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
		//{{ kimhc // 2011-03-27 // 상점검색에서 미리 입어보기
#ifndef	PRE_EQUIP_PERSONAL_SHOP_BOARD
	case WM_RBUTTONUP:
#endif	PRE_EQUIP_PERSONAL_SHOP_BOARD
		//}} kimhc // 2011-03-27 // 상점검색에서 미리 입어보기
	
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		//case WM_MOUSEWHEEL:
		{
			if ( false == m_pDlgPersonalShopBoard->GetIsMouseOver())
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
			if ( false == m_pDlgPersonalShopBoard->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_PERSONAL_SHOP_BOARD, true);
			bFlag = true;			

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			/// 검색어 자동 완성 창이 활성중인 상태에서 다른 곳 클릭시, 자동 완성창 숨겨버리자
			if ( NULL != m_pListboxSearchKeyword && 
				 false == m_pListboxSearchKeyword->IsOnMouseOver() )
			{
				/// 너무 빨리 사라져서, 리스트 박스 클릭 메시지를 못받는다...
				/// 5프레임 후에 닫도록 하자.
				m_iFocusOutSearchKeywordBoxTime = _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNextActiveIntervalFrame;
			}
#endif //SERV_UPGRADE_TRADE_SYSTEM
		}
		break;

		//{{ kimhc // 2011-03-27 // 상점검색에서 미리 입어보기
#ifdef	PRE_EQUIP_PERSONAL_SHOP_BOARD
	case WM_RBUTTONUP:
		{
			POINT mousePoint = { short( LOWORD(lParam) ), short( HIWORD(lParam) ) };
			if ( false == MouseRButtonUp( D3DXVECTOR2( static_cast<float>( mousePoint.x ), static_cast<float>( mousePoint.y) ) ) )
				return false;

			bFlag = true;

		} break;
#endif	PRE_EQUIP_PERSONAL_SHOP_BOARD
		//}} kimhc // 2011-03-27 // 상점검색에서 미리 입어보기
	default:
		break;
	}

	switch( uMsg )
	{
	case WM_LBUTTONUP:
/*
		{			
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlotInMousePos( mousePos );

			// 			if ( pSlotItem != NULL && pSlotItem->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW )
			// 			{
			// 
			// 				if ( pSlotItem->GetSlotID() >= 0 && pSlotItem->GetSlotID() < (int)m_vecRecipeSlot.size() )
			// 				{
			// 					ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[ pSlotItem->GetSlotID() ];
			// 					if ( pRecipeSlot != NULL )
			// 					{
			// 						g_pKTDXApp->GetDeviceManager()->PlaySound( L"X2_Button_Mouse_Up.ogg" );
			// 						SelectRecipe( pSlotItem->GetSlotID() );
			// 					}
			// 				}
			// 			}
			return true;

		}
*/
		break;
	case WM_MOUSEMOVE:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			MouseMove( mousePos );
			return true;
		}
		break;

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_UP:
				{
					/// 검색어 자동 완성 창이 활성중이라면, 한단계 위의 아이템을 선택한다.
					if ( NULL != m_pListboxSearchKeyword && 
						 true == m_pListboxSearchKeyword->GetShow() &&
						 true == m_pListboxSearchKeyword->CanHaveFocus() )
					{
						int iSelectedIndex = m_pListboxSearchKeyword->GetSelectedIndex();

						m_pListboxSearchKeyword->SelectItem( --iSelectedIndex, false );
					}
				} break;

			case VK_DOWN:
				{
					/// 검색어 자동 완성 창이 활성중일 때
					if ( NULL != m_pListboxSearchKeyword && true == m_pListboxSearchKeyword->GetShow() )
					{
						/// 자동 완성창에 포커스가 적용중이라면, 한단계 아래의 아이템을 선택한다.
						if ( true == m_pListboxSearchKeyword->GetHaveFocusIn() )
						{
							int iSelectedIndex = m_pListboxSearchKeyword->GetSelectedIndex();

							m_pListboxSearchKeyword->SelectItem( ++iSelectedIndex, false );
						}
						/// 검색창에 포커스가 적용중이라면, 자동 완성창으로 포커스 이동
						else
						{
							if ( NULL != m_pIMEEditItemName && true == m_pIMEEditItemName->GetHaveFocusIn()  )
							{
								/// 자동 완성창으로 포커스 이동
								m_pListboxSearchKeyword->RequestFocus();
								/// 첫번째 아이템 선택
								m_pListboxSearchKeyword->SelectItem( 0, false );
								/// 검색창 갱신은 하지 말아 주세요.
								m_fActiveSearchKeywordListTime = 0.f;
							}
						}
					}
				} break;

			case VK_RETURN:
				{
					/// 검색어 자동 완성 창이 활성중인 상태에서 엔터 입력시, 검색창에 검색어 설정 후 닫기
					if ( NULL != m_pIMEEditItemName && 
						 false == m_pIMEEditItemName->GetHaveFocusIn() &&
						 NULL != m_pListboxSearchKeyword && 
						 true == m_pListboxSearchKeyword->GetShow() )
					{
						/// 검색창에 검색어 설정
						SetSearchKeywordBySelectedItem();
						/// 검색창으로 포커스 이동 ( 검색창 Enter 입력을 피하기 위해, 일정 시간 대기 후 포커스 이동 )
						m_iFocusInSearchKeywordBoxTime = _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNextActiveIntervalFrame;
						/// 너무 빨리 사라져서, 리스트 박스 클릭 메시지를 못받는다...
						/// 5프레임 후에 닫도록 하자.
						m_iCloseSearchKeywordListTime = _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNextActiveIntervalFrame;
					}
				} break;

			case VK_TAB:
				{
					if ( NULL != m_pIMEEditMinLevel && NULL != m_pIMEEditMaxLevel )
					{
						if ( true == m_pIMEEditMinLevel->GetHaveFocusIn() )
							m_pIMEEditMaxLevel->RequestFocus();
						else if ( true == m_pIMEEditMaxLevel->GetHaveFocusIn() )
							m_pIMEEditMinLevel->RequestFocus();
					}
				}

			}
		}
		break;
#endif //SERV_UPGRADE_TRADE_SYSTEM
	}


	return bFlag;
}

bool CX2UIPersonalShopBoard::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	int tmp_min = 0;
	int tmp_max = 0;

	switch ( wParam )
	{
	case UPSBCM_EXIT:
		{
			SetShow( false );

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			if ( NULL != g_pData &&
				 NULL != g_pData->GetUIManager() &&
				 NULL != g_pData->GetUIManager()->GetPersonalShop() )
			{
				CX2UIPersonalShop* pUIPersonalShop = g_pData->GetUIManager()->GetPersonalShop();

				/// 개인 상점에 접속중이라면, 쫓아내자.
				if ( NULL != pUIPersonalShop )
				{
					if ( -1 != pUIPersonalShop->GetSelectedItemID() )
						g_pData->GetUIManager()->GetPersonalShop()->Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();

					g_pData->GetUIManager()->GetPersonalShop()->SetSelectedItemID( -1 );
				}
			}
#endif //SERV_UPGRADE_TRADE_SYSTEM

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
//-------------------------------------------------------------------------------------------//
//--------------------- 해외팀 하드코딩 스트링으로 변경 시작 -----------------------------------//
//--------------------- 나중에 아라도 바꿔 줘야 한다~~~~~   -----------------------------------//
//-------------------------------------------------------------------------------------------//
	case UPSBCM_COMBO_FIRST:
		{
			m_iComboFirstindex = m_pComboFirst->GetSelectedItemIndex();
			switch(m_iComboFirstindex)
			{
			case 0:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5144 ), NULL, false);		// 부위(전체)
				m_pComboSecond->SetEnable(false);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5145 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->SetEnable(false);
				m_iComboCharacterindex = 0;

				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_5146 ), NULL, false);			// 클래스(전체)
				m_pComboClass->SetEnable(false);
				m_iComboClassindex = 0;
				break;
			case 1:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5148 ), NULL, false);		// 부위(전체)
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5149 ), NULL, false);		// 무기
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5150 ), NULL, false);		// 상의
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5151 ), NULL, false);		// 하의
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5152 ), NULL, false);		// 헤어
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5153 ), NULL, false);		// 장갑
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5154 ), NULL, false);		// 신발
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
				m_pComboSecond->AddItem(GET_STRING( STR_ID_28368 ), NULL, false);		// 한벌
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
				m_pComboSecond->SetEnable(true);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5155 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5156 ), NULL, false);		// 엘소드
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5157 ), NULL, false);		// 레나
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5158 ), NULL, false);		// 아이샤
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_RAVEN )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5159 ), NULL, false);		// 레이븐
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_EVE )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5160 ), NULL, false);		// 이브
#ifdef NEW_CHARACTER_CHUNG
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_CHUNG )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_12773 ), NULL, false);		// 청
#endif NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ARA )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_25372 ), NULL, false);
#endif
#ifdef NEW_CHARACTER_EL
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ELESIS )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_29334 ), NULL, false);
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ADD )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(L"#C0000FF애드#CX", NULL, false);		/// 변경이 필요 하다.
#endif //SERV_9TH_NEW_CHARACTER
				m_pComboCharacter->SetEnable(true);
				m_iComboCharacterindex = 0;
				break;
			case 2:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5148 ), NULL, false);		// 부위(전체)
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5149 ), NULL, false);		// 무기
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5150 ), NULL, false);		// 상의
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5151 ), NULL, false);		// 하의
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5153 ), NULL, false);		// 장갑
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5154 ), NULL, false);		// 신발
				m_pComboSecond->SetEnable(true);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5155 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5156 ), NULL, false);		// 엘소드
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5157 ), NULL, false);		// 레나
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5158 ), NULL, false);		// 아이샤
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_RAVEN )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5159 ), NULL, false);		// 레이븐
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_EVE )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5160 ), NULL, false);		// 이브
#ifdef NEW_CHARACTER_CHUNG
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_CHUNG )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_12773 ), NULL, false);		// 청
#endif NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ARA )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_25372 ), NULL, false);	// 아라
#endif
#ifdef NEW_CHARACTER_EL
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ELESIS )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_29334 ), NULL, false);	// 엘리시스
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ADD )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(L"#C0000FF애드#CX", NULL, false);		/// 변경이 필요 하다.
#endif //SERV_9TH_NEW_CHARACTER
				m_pComboCharacter->SetEnable(true);
				m_iComboCharacterindex = 0;
				break;
			case 3:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5148 ), NULL, false);		// 부위(전체)
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5149 ), NULL, false);		// 무기
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5161 ), NULL, false);		// 얼굴(상)
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5162 ), NULL, false);		// 얼굴(중)
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5163 ), NULL, false);		// 얼굴(하)
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5150 ), NULL, false);		// 상의
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5151 ), NULL, false);		// 하의
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5164 ), NULL, false);		// 팔
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5165 ), NULL, false);		// 반지
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5166 ), NULL, false);		// 목걸이
				m_pComboSecond->SetEnable(true);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5155 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5156 ), NULL, false);		// 엘소드
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5157 ), NULL, false);		// 레나
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5158 ), NULL, false);		// 아이샤
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_RAVEN )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5159 ), NULL, false);		// 레이븐
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_EVE )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5160 ), NULL, false);		// 이브
#ifdef NEW_CHARACTER_CHUNG
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_CHUNG )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_12773 ), NULL, false);		// 청
#endif NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ARA )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_25372 ), NULL, false);		// 아라
#endif
#ifdef NEW_CHARACTER_EL
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ELESIS )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_29334 ), NULL, false);		// 엘리시스
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ADD )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(L"#C0000FF애드#CX", NULL, false);		/// 변경이 필요 하다.
#endif //SERV_9TH_NEW_CHARACTER

				m_pComboCharacter->SetEnable(true);
				m_iComboCharacterindex = 0;
				break;
			case 4:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5147 ), NULL, false);		// 없음
				m_pComboSecond->SetEnable(true);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5155 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5156 ), NULL, false);		// 엘소드
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5157 ), NULL, false);		// 레나
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5158 ), NULL, false);		// 아이샤
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_RAVEN )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5159 ), NULL, false);		// 레이븐
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_EVE )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5160 ), NULL, false);		// 이브
#ifdef NEW_CHARACTER_CHUNG
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_CHUNG )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_12773 ), NULL, false);		// 청
#endif NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ARA )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_25372 ), NULL, false);		// 아라
#endif
#ifdef NEW_CHARACTER_EL
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ELESIS )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_29334 ), NULL, false);		// 엘리시스
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ADD )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(L"#C0000FF애드#CX", NULL, false);
#endif //SERV_9TH_NEW_CHARACTER

				m_pComboCharacter->SetEnable(true);
				m_iComboCharacterindex = 0;
				break;
			case 5:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5147 ), NULL, false);		// 없음
				m_pComboSecond->SetEnable(true);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5155 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5156 ), NULL, false);		// 엘소드
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5157 ), NULL, false);		// 레나
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5158 ), NULL, false);		// 아이샤
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_RAVEN )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5159 ), NULL, false);		// 레이븐
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_EVE )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5160 ), NULL, false);		// 이브
#ifdef NEW_CHARACTER_CHUNG
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_CHUNG )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_12773 ), NULL, false);		// 청
#endif NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ARA )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_25372 ), NULL, false);		// 아라
#endif
#ifdef NEW_CHARACTER_EL
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ELESIS )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_29334 ), NULL, false);		// 엘리시스
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ADD )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(L"#C0000FF애드#CX", NULL, false);		/// 변경이 필요 하다.
#endif //SERV_9TH_NEW_CHARACTER

				m_pComboCharacter->SetEnable(true);
				m_iComboCharacterindex = 0;
				break;
			case 6:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5147 ), NULL, false);		// 없음
				m_pComboSecond->SetEnable(true);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5155 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5156 ), NULL, false);		// 엘소드
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5157 ), NULL, false);		// 레나
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5158 ), NULL, false);		// 아이샤
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_RAVEN )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5159 ), NULL, false);		// 레이븐
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_EVE )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5160 ), NULL, false);		// 이브
#ifdef NEW_CHARACTER_CHUNG
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_CHUNG )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_12773 ), NULL, false);		// 청
#endif NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ARA )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_25372 ), NULL, false);		// 아라
#endif
#ifdef NEW_CHARACTER_EL
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ELESIS )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_29334 ), NULL, false);		// 엘리시스
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ADD )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(L"#C0000FF애드#CX", NULL, false);		/// 변경이 필요 하다.
#endif //SERV_9TH_NEW_CHARACTER

				m_pComboCharacter->SetEnable(true);
				m_iComboCharacterindex = 0;
				break;
			case 7:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5147 ), NULL, false);		// 없음
				m_pComboSecond->SetEnable(true);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5155 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5156 ), NULL, false);		// 엘소드
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5157 ), NULL, false);		// 레나
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5158 ), NULL, false);		// 아이샤
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_RAVEN )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5159 ), NULL, false);		// 레이븐
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_EVE )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5160 ), NULL, false);		// 이브
#ifdef NEW_CHARACTER_CHUNG
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_CHUNG )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_12773 ), NULL, false);		// 청
#endif NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ARA )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_25372 ), NULL, false);		// 아라
#endif
#ifdef NEW_CHARACTER_EL
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ELESIS )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_29334 ), NULL, false);		// 엘리시스
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#ifdef SERV_LIMIT_UNIT
				if( static_cast< CX2Unit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType ) >= CX2Unit::UT_ADD )
#endif //SERV_LIMIT_UNIT
				m_pComboCharacter->AddItem(L"#C0000FF애드#CX", NULL, false);		/// 변경이 필요 하다.
#endif //SERV_9TH_NEW_CHARACTER

				m_pComboCharacter->SetEnable(true);
				m_iComboCharacterindex = 0;
				break;
			default:
				m_pComboSecond->RemoveAllItems();
				m_pComboSecond->AddItem(GET_STRING( STR_ID_5144 ), NULL, false);		// 부위(전체)
				m_pComboSecond->SetEnable(false);
				m_iComboSecondindex = 0;

				m_pComboCharacter->RemoveAllItems();
				m_pComboCharacter->AddItem(GET_STRING( STR_ID_5145 ), NULL, false);		// 케릭터(전체)
				m_pComboCharacter->SetEnable(false);
				m_iComboCharacterindex = 0;

				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_5146 ), NULL, false);			// 클래스(전체)
				m_pComboClass->SetEnable(false);
				m_iComboClassindex = 0;
				break;
			}
			UpdateButtonSearchItem();
			return true;
		}
		break;
	case UPSBCM_COMBO_SECOND:
		{
			m_iComboSecondindex = m_pComboSecond->GetSelectedItemIndex();
		}
		break;
	case UPSBCM_COMBO_CHARACTER:
		{
			m_iComboCharacterindex = m_pComboCharacter->GetSelectedItemIndex();
			switch(m_iComboCharacterindex)
			{
			case 0:
				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);		// 클래스(전체)
				m_pComboClass->SetEnable(false);
				m_iComboClassindex = 0;
				break;
			case 1:
				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);		// 클래스(전체)
				m_pComboClass->AddItem(GET_STRING( STR_ID_9692 ), NULL, false);		// 로드 나이트
				m_pComboClass->AddItem(GET_STRING( STR_ID_9693 ), NULL, false);		// 룬 슬레이어
#ifdef SERV_ELSWORD_INFINITY_SWORD
#ifdef SHOPBOARD_NO_SEARCH_ELSWORD_INFINITY_SWORD
#else //SHOPBOARD_NO_SEARCH_ELSWORD_INFINITY_SWORD
				m_pComboClass->AddItem(GET_STRING( STR_ID_21287 ), NULL, false);
#endif //SHOPBOARD_NO_SEARCH_ELSWORD_INFINITY_SWORD
#endif
				m_pComboClass->SetEnable(true);
				m_iComboClassindex = 0;
				break;
			case 2:
				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);		// 클래스(전체)
				m_pComboClass->AddItem(GET_STRING( STR_ID_9694 ), NULL, false);		// 그랜드 아처
				m_pComboClass->AddItem(GET_STRING( STR_ID_9695 ), NULL, false);		// 윈드 스니커
#ifdef SERV_RENA_NIGHT_WATCHER
#ifdef SHOPBOARD_NO_SEARCH_RENA_NIGHT_WATCHER
#else //SHOPBOARD_NO_SEARCH_RENA_NIGHT_WATCHER
				m_pComboClass->AddItem(GET_STRING( STR_ID_21289 ), NULL, false);
#endif //SHOPBOARD_NO_SEARCH_RENA_NIGHT_WATCHER
#endif
				m_pComboClass->SetEnable(true);
				m_iComboClassindex = 0;
				break;
			case 3:
				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);		// 클래스(전체)
				m_pComboClass->AddItem(GET_STRING( STR_ID_9696 ), NULL, false);		// 엘리멘탈 마스터
				m_pComboClass->AddItem(GET_STRING( STR_ID_9697 ), NULL, false);		// 보이드 프린세스
#ifdef SERV_ARME_DIMENSION_WITCH
#ifdef SHOPBOARD_NO_SEARCH_ARME_DIMENSION_WITCH
#else //SHOPBOARD_NO_SEARCH_ARME_DIMENSION_WITCH
				m_pComboClass->AddItem(GET_STRING( STR_ID_21288 ), NULL, false);
#endif //SHOPBOARD_NO_SEARCH_ARME_DIMENSION_WITCH
#endif
				m_pComboClass->SetEnable(true);
				m_iComboClassindex = 0;
				break;
			case 4:
				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);		// 클래스(전체)
#ifdef SHOPBOARD_NO_SEARCH_RAVEN
#else //SHOPBOARD_NO_SEARCH_RAVEN
				m_pComboClass->AddItem(GET_STRING( STR_ID_9698 ), NULL, false);		// 블레이드 마스터
				m_pComboClass->AddItem(GET_STRING( STR_ID_9699 ), NULL, false);		// 레크리스 피스트
#ifdef SERV_RAVEN_VETERAN_COMMANDER
#ifdef SHOPBOARD_NO_SEARCH_RAVEN_VETERAN_COMMANDER
#else //SHOPBOARD_NO_SEARCH_RAVEN_VETERAN_COMMANDER
				m_pComboClass->AddItem(GET_STRING( STR_ID_21290 ), NULL, false);
#endif //SHOPBOARD_NO_SEARCH_RAVEN_VETERAN_COMMANDER
#endif
#endif //SHOPBOARD_NO_SEARCH_RAVEN
				m_pComboClass->SetEnable(true);
				m_iComboClassindex = 0;
				break;
			case 5:
				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);		// 클래스(전체)
#ifdef SHOPBOARD_NO_SEARCH_EVE
#else //SHOPBOARD_NO_SEARCH_EVE
				m_pComboClass->AddItem(GET_STRING( STR_ID_9700 ), NULL, false);		// 네메시스
				m_pComboClass->AddItem(GET_STRING( STR_ID_9701 ), NULL, false);		// 엠프레스
#ifdef SERV_EVE_BATTLE_SERAPH
#ifdef SHOPBOARD_NO_SEARCH_EVE_BATTLE_SERAPH
#else //SHOPBOARD_NO_SEARCH_EVE_BATTLE_SERAPH
				m_pComboClass->AddItem(GET_STRING( STR_ID_21291 ), NULL, false);
#endif //SHOPBOARD_NO_SEARCH_EVE_BATTLE_SERAPH
#endif
#endif //SHOPBOARD_NO_SEARCH_EVE
				m_pComboClass->SetEnable(true);
				m_iComboClassindex = 0;
				break;
#ifdef NEW_CHARACTER_CHUNG
			case 6:
				m_pComboClass->RemoveAllItems();
#ifdef CHUNG_SECOND_CLASS_CHANGE
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);		// 클래스(전체)
#ifdef SHOPBOARD_NO_SEARCH_CHUNG
#else //SHOPBOARD_NO_SEARCH_CHUNG
				m_pComboClass->AddItem(GET_STRING( STR_ID_15051 ), NULL, false);
				m_pComboClass->AddItem(GET_STRING( STR_ID_15052 ), NULL, false);
#ifdef SERV_CHUNG_TACTICAL_TROOPER
#ifdef SHOPBOARD_NO_SEARCH_CHUNG_TACTICAL_TROOPER
#else //SHOPBOARD_NO_SEARCH_CHUNG_TACTICAL_TROOPER
				m_pComboClass->AddItem(GET_STRING( STR_ID_21292 ), NULL, false);
#endif //SHOPBOARD_NO_SEARCH_CHUNG_TACTICAL_TROOPER
#endif
#endif //SHOPBOARD_NO_SEARCH_CHUNG
				m_pComboClass->SetEnable(true);
#endif CHUNG_SECOND_CLASS_CHANGE
				m_iComboClassindex = 0;
				break;
#endif NEW_CHARACTER_CHUNG

#ifndef SERV_NO_ARA
#ifdef ARA_CHARACTER_BASE
			case 7:
				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);
	#ifdef ARA_CHANGE_CLASS_FIRST
				m_pComboClass->AddItem(GET_STRING( STR_ID_25373 ), NULL, false);
				m_pComboClass->SetEnable(true);
	#endif //ARA_CHANGE_CLASS_FIRST
	#ifdef SERV_ARA_CHANGE_CLASS_SECOND
#if defined (CLIENT_COUNTRY_TH) || defined (CLIENT_COUNTRY_ID) || defined (CLIENT_COUNTRY_BR) || defined (CLIENT_COUNTRY_PH)
#else //CLIENT_COUNTRY_XX
				m_pComboClass->AddItem(GET_STRING( STR_ID_29385 ), NULL, false);
				m_pComboClass->SetEnable(true);
#endif //CLIENT_COUNTRY_XX
	#endif //SERV_ARA_CHANGE_CLASS_SECOND
				m_iComboClassindex = 0;
				break;
#endif
#endif // SERV_NO_ARA
#ifdef NEW_CHARACTER_EL

#ifndef SHOPBOARD_NO_ELESIS
			case 8:
				{
					m_pComboClass->RemoveAllItems();
					m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);
					m_pComboClass->SetEnable(true);
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	#if defined (CLIENT_COUNTRY_TWHK) || defined (CLIENT_COUNTRY_JP) || defined (CLIENT_COUNTRY_EU) || defined (CLIENT_COUNTRY_US) || defined (CLIENT_COUNTRY_CN) || defined (CLIENT_COUNTRY_TH) || defined (CLIENT_COUNTRY_ID) || defined (CLIENT_COUNTRY_BR) || defined (CLIENT_COUNTRY_PH)
	#else //CLIENT_COUNTRY_XX
					m_pComboClass->AddItem( GET_STRING( STR_ID_29957 ), NULL, false );
					m_pComboClass->SetEnable(true);
					m_pComboClass->AddItem( GET_STRING( STR_ID_29958 ), NULL, false );
					m_pComboClass->SetEnable(true);
	#endif //CLIENT_COUNTRY_XX
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

				} break;

#endif SHOPBOARD_NO_ELESIS
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			case 9:
				{
					m_pComboClass->RemoveAllItems();
					m_pComboClass->AddItem(L"#C0000FF클래스(전체)#CX", NULL, false);
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
					m_pComboClass->AddItem(L"#C0000FF루나틱사이커#CX", NULL, false );
					m_pComboClass->SetEnable(true);
#endif //SERV_ADD_LUNATIC_PSYKER
					m_pComboClass->SetEnable(true);
				} break;
#endif //SERV_9TH_NEW_CHARACTER
			default:
				m_pComboClass->RemoveAllItems();
				m_pComboClass->AddItem(GET_STRING( STR_ID_9691 ), NULL, false);		// 클래스(전체)
				m_pComboClass->SetEnable(false);
				m_iComboClassindex = 0;
				break;
			}

			return true;
		}
		break;
//-------------------------------------------------------------------------------------------//
//--------------------- 해외팀 하드코딩 스트링으로 변경 끝 -----------------------------------//
//-------------------------------------------------------------------------------------------//		
		
	case UPSBCM_COMBO_CLASS:
		{
			m_iComboClassindex = m_pComboClass->GetSelectedItemIndex();
		}
		break;
	// 체크 박스
	case UPSBCM_CHECK_UNIQUE:
		{
			m_bCheckUnique = m_pCheckUnique->GetChecked();
			UpdateCheckBoxColor();
		}
		break;
	case UPSBCM_CHECK_ELITE:
		{
			m_bCheckElite = m_pCheckElite->GetChecked();
			UpdateCheckBoxColor();
		}
		break;
	case UPSBCM_CHECK_RARE:
		{
			m_bCheckRare = m_pCheckRare->GetChecked();
			UpdateCheckBoxColor();
		}
		break;
	case UPSBCM_CHECK_NORMAL:
		{
			m_bCheckNormal = m_pCheckNormal->GetChecked();
			UpdateCheckBoxColor();
		}
		break;
	case UPSBCM_CHECK_LOW:
		{
			m_bCheckLow = m_pCheckLow->GetChecked();
			UpdateCheckBoxColor();
		}
		break;
	case UPSBCM_CHECK_USABLE:
		{
			m_bCheckUsable = m_pCheckUsable->GetChecked();
			if(m_bCheckUsable)
			{
				UsableFilter();
			}
			else
			{
				UsableUnFilter();
			}
		}
		break;

	case UPSBCM_ENTER_MIN_LEVEL:
		{
			if(m_pIMEEditMinLevel->GetTextLength() == 0)
			{
				m_iIMEEditMinLevel = 0;
			}
			else
			{
				int iIMEEditMinLevel= (int) wcstol( m_pIMEEditMinLevel->GetText(), NULL, 10 );
				SetIMMEditMinLevel(iIMEEditMinLevel);
			}
		}
		break;
	case UPSBCM_ENTER_MAX_LEVEL:
		{
			if(m_pIMEEditMaxLevel->GetTextLength() == 0)
			{
				m_iIMEEditMaxLevel = 0;
			}
			else
			{
				int iIMEEditMaxLevel= (int) wcstol( m_pIMEEditMaxLevel->GetText(), NULL, 10 );
				SetIMMEditMaxLevel(iIMEEditMaxLevel);
			}
		}
		break;
	case UPSBCM_ENTER_ITEM_NAME:
		{
			if(m_pIMEEditItemName->GetTextLength() == 0)
			{
				m_wstrFinder = L"";
			}
			if(m_pIMEEditItemName->GetTextLength() == 1) // 2자 이상 입력 체크
			{
				m_wstrFinder = L"";
				m_pIMEEditItemName->SetText(L"", false);
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),  GET_STRING(STR_ID_4982), g_pMain->GetNowState() );
			}
			else
			{
				m_wstrFinder = m_pIMEEditItemName->GetText();

				m_fCurrentTime = (float)DXUTGetGlobalTimer()->GetTime();

				if( m_fCurrentTime > m_fLastSearchTime + 3.0f)
				{
					if(m_iComboFirstindex != 0 || m_wstrFinder != L"")
					{
		#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
						/// 검색 실행시 자동 완성창이 표시 되어 있다면, 숨기자.
						if ( NULL != m_pListboxSearchKeyword && true == m_pListboxSearchKeyword->GetShow() )
							SetShowSearchKeywordList( false );

						/// 검색 리스트 동작 대기중이라면, 중지 시켜 버리세요.
						m_fActiveSearchKeywordListTime = 0.f;						
		#endif // SERV_UPGRADE_TRADE_SYSTEM
						m_fLastSearchTime = (float)DXUTGetGlobalTimer()->GetTime();
						Handler_EGS_SEARCH_TRADE_BOARD_REQ();
					}
				}
			}
			UpdateButtonSearchItem(); // 검색버튼 활성, 비활성화
		}
		break;

#ifdef SERV_UPGRADE_TRADE_SYSTEM		// 해외팀 오류 수정
	/// 일치하는 단어 검색 기능 설정
	case UPSBCM_CHECK_STRICT_SEARCH_OPTION:
		{
			if ( NULL != m_pStrictSearchOption )
			{
				/// 체크 박스에 설정된 내용으로 적용
				m_bIsStrictSearchOption = m_pStrictSearchOption->GetChecked();
			}
		}
		break;
#else SERV_UPGRADE_TRADE_SYSTEM // 김태환
	case UPSBCM_MOUSE_OUT_ITEM_NAME:
		{
			if(m_pIMEEditItemName->GetTextLength() == 0)
			{
				m_wstrFinder = L"";
			}
			if(m_pIMEEditItemName->GetTextLength() == 1) // 2자 이상 입력 체크
			{
				m_wstrFinder = L"";
				m_pIMEEditItemName->SetText(L"", false);
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),  GET_STRING(STR_ID_4982), g_pMain->GetNowState() );
			}
			else
			{
				m_wstrFinder = m_pIMEEditItemName->GetText();
			}

			UpdateButtonSearchItem(); // 검색버튼 활성, 비활성화
		}
		break;
#endif // SERV_UPGRADE_TRADE_SYSTEM

	case UPSBCM_PUSH_SEARCH_ITEM:
		{
			m_fLastSearchTime = (float)DXUTGetGlobalTimer()->GetTime();
			Handler_EGS_SEARCH_TRADE_BOARD_REQ();
		}
		break;

	case UPSBCM_PUSH_REFRESH:
		{
			m_fCurrentTime = (float)DXUTGetGlobalTimer()->GetTime();

			if( m_fCurrentTime > m_fLastSearchTime + 3.0f)
			{
				if(m_iComboFirstindex != 0 || m_wstrFinder != L"")
				{
					m_fLastSearchTime = (float)DXUTGetGlobalTimer()->GetTime();
					Handler_EGS_SEARCH_TRADE_BOARD_REQ();
				}
			}
		}
		break;
	case UPSBCM_PUSH_INIT_SEARCH_CONDITION:
		{
			InitSearchCondition();
			m_vecItemInfo.clear();
			m_uiNowPage = 1;
			m_uiFirstPageinCurrentBoard = 1;
			m_uiMaxPage = 0;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PREV_BOARD:
		{
			m_uiFirstPageinCurrentBoard = m_uiFirstPageinCurrentBoard - _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard;
			m_uiNowPage = m_uiFirstPageinCurrentBoard;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PREV_PAGE:
		{
			m_uiNowPage--;
			if(	m_uiNowPage	< m_uiFirstPageinCurrentBoard )
			{	
				m_uiFirstPageinCurrentBoard = m_uiFirstPageinCurrentBoard - _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard;
			}
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_NEXT_PAGE:
		{
			m_uiNowPage++;
			if(	m_uiNowPage	>= m_uiFirstPageinCurrentBoard + _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard )
			{	
				m_uiFirstPageinCurrentBoard = m_uiFirstPageinCurrentBoard + _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard;
			}
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_NEXT_BOARD:
		{
			m_uiFirstPageinCurrentBoard = m_uiFirstPageinCurrentBoard + _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard;
			m_uiNowPage = m_uiFirstPageinCurrentBoard;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM1:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM2:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 1;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM3:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 2;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM4:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 3;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM5:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 4;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM6:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 5;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM7:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 6;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM8:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 7;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM9:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 8;
			UpdateItemList();
		}
		break;
	case UPSBCM_PUSH_PAGE_NUM10:
		{
			m_uiNowPage = m_uiFirstPageinCurrentBoard + 9;
			UpdateItemList();
		}
		break;

	case UPSBCM_PUSH_BUY_ITEM:
		{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			/// 개인 상점UI를 거치지 않고, 개인 상점 정보를 가져오자.
			if ( NULL != g_pData &&
				 NULL != g_pData->GetUIManager()  )
			{
				/// 개인 상점 객체 반환
				CX2UIPersonalShop* pUIPersonalShop = g_pData->GetUIManager()->GetPersonalShop();

				if ( NULL != pUIPersonalShop )
				{
					/// 선택한 아이템 UID 반환
					UidType uidSelectedItemUID = pUIPersonalShop->GetSelectedItemID();

					/// 만약 뭔가 아이템이 설정되어 있다면, 더이상 진행하지 말자.
					/// ( 아이템 선택 이후, 구입 처리 전에 다른 아이템을 선택해서 해당 UID가 갱신 되는 것을 막음 )
					if ( -1 != uidSelectedItemUID )
						break;

					/// 개인 상점의 UID 반환
					CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
					int iButtonIdx = pButton->GetDummyInt(0);

					if( iButtonIdx < 0 || iButtonIdx >= _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage )
						break;

					/// 개인 상점 아이디 저장
					m_iSelectedPersonalShopUid = m_iPersonalShopUidList[iButtonIdx];

					/// 선택한 아이템 UID 저장
					CX2SlotItem* pItemSlot = static_cast<CX2SlotItem*>( GetSlot( iButtonIdx ) );

					if ( NULL != pItemSlot )
						pUIPersonalShop->SetSelectedItemID( pItemSlot->GetItemUID() );

					/// 개인 상점 입장 요청
					if ( CX2UIPersonalShop::XPSS_BUY == pUIPersonalShop->GetPersonalShopState() )
						pUIPersonalShop->Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();
					else
						pUIPersonalShop->Handler_EGS_JOIN_PERSONAL_SHOP_FROM_BOARD_REQ();
				}
			}
#else //SERV_UPGRADE_TRADE_SYSTEM
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			//int iTargetPage = pButton->GetDummyInt(0);
			//GotoPage(iTargetPage);
			int iButtonIdx = pButton->GetDummyInt(0);
			
			if( iButtonIdx < 0 || iButtonIdx >= _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage )
				break;

			m_iSelectedPersonalShopUid = m_iPersonalShopUidList[iButtonIdx];
			
			// 임시 처리
			if(m_iSelectedPersonalShopUid != -1 )
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

		#ifdef SERV_PSHOP_AGENCY
				if( m_iShopOwnerUidList[iButtonIdx] == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
					g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, CX2UIPersonalShop::OPENMYSHOPFROMBOARD);
				else
					g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, CX2UIPersonalShop::OPENFROMBOARD);
		#else //SERV_PSHOP_AGENCY
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, CX2UIPersonalShop::OPENFROMBOARD);
		#endif //SERV_PSHOP_AGENCY

			}
#endif //SERV_UPGRADE_TRADE_SYSTEM

		}
		break;

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	case UPSBCM_CHANGE_ITEM_NAME:			/// 검색어 입력시 실시간 갱신
		{
			/// 특정 대기 시간 이후, 갱신 처리
			/// ( 반복 입력시 생기는 부하를 줄이기 위해, 해당 동작을 통해 갱신되어 버린 리스트의 선택을 방지하기 위해 )
			m_fActiveSearchKeywordListTime = _CONST_UIPERSONALSHOPBOARD_INFO_::g_fSearchKeywordIntervalTime;
		}
		break;

	case UPSBCM_SELECT_SEARCH_KEYWORD:		/// 검색어 자동 완성 리스트에서 목록을 클릭했을 때
		{
			/// 검색어 자동 완성창 동작 쿨타임 초기화
			m_fActiveSearchKeywordListTime = 0.f;

			/// 검색창에 검색어 설정
			SetSearchKeywordBySelectedItem();

			/// 검색어 자동 완성 리스트 해제
			SetShowSearchKeywordList( false );

			/// 검색창으로 포커스 이동
			if ( NULL != m_pIMEEditItemName )
				m_pIMEEditItemName->RequestFocus();
		}
		break;

	/// 모든 아이템 등급 체크 혹은 체크 해제
	case UPSBCM_CHECK_ALL:
		{
			if ( NULL != m_pCheckLow &&
				 NULL != m_pCheckNormal &&
				 NULL != m_pCheckRare &&
				 NULL != m_pCheckElite &&
				 NULL != m_pCheckUnique &&
				 NULL != m_pCheckALL )
			{
				const bool bCheckAll = m_pCheckALL->GetChecked();

				m_pCheckLow->SetChecked( bCheckAll );
				m_pCheckNormal->SetChecked( bCheckAll );
				m_pCheckRare->SetChecked( bCheckAll );
				m_pCheckElite->SetChecked( bCheckAll );
				m_pCheckUnique->SetChecked( bCheckAll );

				m_bCheckUnique = m_bCheckElite = m_bCheckRare = m_bCheckNormal = m_bCheckLow = bCheckAll;
			}
		}
		break;
#endif //SERV_UPGRADE_TRADE_SYSTEM

	default:
		break;
	}

	return false;
}

void CX2UIPersonalShopBoard::UsableFilter()
{
	UINT j = 0;
	m_vecItemInfoBackup.clear();
	m_vecItemInfoBackup.resize( m_vecItemInfo.size() );
	std::copy( m_vecItemInfo.begin(), m_vecItemInfo.end(), m_vecItemInfoBackup.begin() );

	for(UINT i = 0; i< m_vecItemInfo.size(); i++)
	{
		int iItemID = m_vecItemInfo[i].m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
		
		if( CX2Unit::CanUse(iItemID, g_pData->GetMyUser()->GetSelectUnit(), 0) == true )
		{
			m_vecItemInfo[j] =  m_vecItemInfo[i];
			j++;
		}
	}
	m_vecItemInfo.resize(j);

	m_uiNowPage = 1;
	m_uiFirstPageinCurrentBoard = 1;
	if(m_vecItemInfo.size() == 0)
	{
		m_uiMaxPage = 0;
	}
	else
	{
		m_uiMaxPage = static_cast< int >( (m_vecItemInfo.size() - 1) / _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage) + 1;
	}

	UpdateItemList();
	return;
}

void CX2UIPersonalShopBoard::UsableUnFilter()
{
	m_vecItemInfo.clear();
	m_vecItemInfo.resize( m_vecItemInfoBackup.size() );
	std::copy( m_vecItemInfoBackup.begin(), m_vecItemInfoBackup.end(), m_vecItemInfo.begin() );

	m_uiNowPage = 1;
	m_uiFirstPageinCurrentBoard = 1;
	if(m_vecItemInfo.size() == 0)
	{
		m_uiMaxPage = 0;
	}
	else
	{
		m_uiMaxPage = static_cast< int >( (m_vecItemInfo.size() - 1) / _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage) + 1;
	}

	UpdateItemList();
	return;
}

wstring CX2UIPersonalShopBoard::GetSlotItemDesc()
{
	KTDXPROFILE();

	wstring itemDesc=  L"";


	if ( m_pNowOverItemSlot != NULL )
	{
		BOOST_TEST_FOREACH( KTradeBoardItemInfo, ItemInfo, m_vecItemInfo )
		{
			if ( ItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID == m_pNowOverItemSlot->GetItemUID() )
			{
				CX2Item::ItemData kItemData( ItemInfo.m_kSellItemInfo.m_kInventoryItemInfo );
				CX2Item* pItem = new CX2Item( kItemData, NULL );
				itemDesc = GetSlotItemDescByTID( pItem, pItem->GetItemData().m_ItemID, false );
				SAFE_DELETE( pItem );
				break;
			}
		}
	}


	return itemDesc;
}


bool CX2UIPersonalShopBoard::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_SEARCH_TRADE_BOARD_ACK:
		{
			return Handler_EGS_SEARCH_TRADE_BOARD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	default:
		break;
	}

	return false;
}

void CX2UIPersonalShopBoard::InitSearchCondition()
{
	m_iComboFirstindex = 0;
	m_iComboSecondindex = 0;
	m_iComboCharacterindex = 0;
	m_iComboClassindex = 0;

//-------------------------------------------------------------------------------------------//
//--------------------- 해외팀 하드코딩 스트링으로 변경 시작 -----------------------------------//
//-------------------------------------------------------------------------------------------//

	m_pComboFirst->RemoveAllItems();
	m_pComboFirst->AddItem(GET_STRING( STR_ID_5167 ), NULL, false);				// 종류(전체)
	m_pComboFirst->AddItem(GET_STRING( STR_ID_5168 ), NULL, false);				// 아바타
	m_pComboFirst->AddItem(GET_STRING( STR_ID_5169 ), NULL, false);				// 장비
	m_pComboFirst->AddItem(GET_STRING( STR_ID_5170 ), NULL, false);				// 악세사리
	m_pComboFirst->AddItem(GET_STRING( STR_ID_5171 ), NULL, false);				// 소비
	m_pComboFirst->AddItem(GET_STRING( STR_ID_5172 ), NULL, false);				// 특수
	m_pComboFirst->AddItem(GET_STRING( STR_ID_5173 ), NULL, false);				// 재료
	m_pComboFirst->AddItem(GET_STRING( STR_ID_5174 ), NULL, false);				// 퀘스트

	m_pComboSecond->RemoveAllItems();
	m_pComboSecond->AddItem(GET_STRING( STR_ID_5144 ), NULL, false);
	m_pComboSecond->SetEnable(false);

	m_pComboCharacter->RemoveAllItems();
	m_pComboCharacter->AddItem(GET_STRING( STR_ID_5145 ), NULL, false);
	m_pComboCharacter->SetEnable(false);

	m_pComboClass->RemoveAllItems();
	m_pComboClass->AddItem(GET_STRING( STR_ID_5146 ), NULL, false);
	m_pComboClass->SetEnable(false);

	m_iIMEEditMinLevel = 0;
	m_iIMEEditMaxLevel = _CONST_X2GAME_::g_iMaxLevel;	// 해외팀 변경

//-------------------------------------------------------------------------------------------//
//--------------------- 해외팀 하드코딩 스트링으로 변경 끝 -----------------------------------//
//-------------------------------------------------------------------------------------------//

	m_pIMEEditMinLevel->SetText(L"", false);
	m_pIMEEditMaxLevel->SetText(L"", false);


	m_bCheckUnique = true;
	m_bCheckElite = true;
	m_bCheckRare = true;
	m_bCheckNormal = true;
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	m_bCheckLow = true;
#else // SERV_UPGRADE_TRADE_SYSTEM
	m_bCheckLow = false;
#endif // SERV_UPGRADE_TRADE_SYSTEM

	m_bCheckUsable = false;

	m_pCheckUnique->SetChecked(true);
	m_pCheckElite->SetChecked(true);
	m_pCheckRare->SetChecked(true);
	m_pCheckNormal->SetChecked(true);

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	m_pCheckLow->SetChecked(true);
	m_pCheckALL->SetChecked(true);
#else // SERV_UPGRADE_TRADE_SYSTEM
	m_pCheckLow->SetChecked(false);
#endif // SERV_UPGRADE_TRADE_SYSTEM

	m_pCheckUsable->SetChecked(false);

	UpdateCheckBoxColor();

	m_wstrFinder = L"";
	m_pIMEEditItemName->SetText(L"", false);

	UpdateButtonSearchItem();

	return;
}

bool CX2UIPersonalShopBoard::Handler_EGS_SEARCH_TRADE_BOARD_REQ()
{
	KEGS_SEARCH_TRADE_BOARD_REQ kPacket;


	CX2Inventory::SORT_TYPE nowSortType = CX2Inventory::ST_NONE;
	switch(m_iComboFirstindex)
	{
	case 0:
		nowSortType = CX2Inventory::ST_NONE;
		break;
	case 1:
		nowSortType = CX2Inventory::ST_AVARTA;
		break;
	case 2:
		nowSortType = CX2Inventory::ST_EQUIP;
		break;
	case 3:
		nowSortType = CX2Inventory::ST_ACCESSORY;
		break;
	case 4:
		nowSortType = CX2Inventory::ST_QUICK_SLOT;
		break;
	case 5:
		nowSortType = CX2Inventory::ST_SPECIAL;
		break;
	case 6:
		nowSortType = CX2Inventory::ST_MATERIAL;
		break;
	case 7:
		nowSortType = CX2Inventory::ST_QUEST;
		break;
	default:
		nowSortType = CX2Inventory::ST_NONE;
		break;
	}

	if(nowSortType == CX2Inventory::ST_NONE)
	{
		kPacket.m_cSlotCategory = -1;
	}
	else
	{
		kPacket.m_cSlotCategory	= static_cast< char >( nowSortType );
	}


	CX2Unit::EQIP_POSITION nowEquipPosition = CX2Unit::EP_NONE;
	switch(nowSortType)
	{
	case CX2Inventory::ST_NONE:
		nowEquipPosition = CX2Unit::EP_NONE;
		break;
	case CX2Inventory::ST_AVARTA:
		{
			switch(m_iComboSecondindex)
			{
			case 0:
				nowEquipPosition = CX2Unit::EP_NONE;
				break;
			case 1:
				nowEquipPosition = CX2Unit::EP_WEAPON_HAND;
				break;
			case 2:
				nowEquipPosition = CX2Unit::EP_DEFENCE_BODY;
				break;
			case 3:
				nowEquipPosition = CX2Unit::EP_DEFENCE_LEG;
				break;
			case 4:
				nowEquipPosition = CX2Unit::EP_DEFENCE_HAIR;
				break;
			case 5:
				nowEquipPosition = CX2Unit::EP_DEFENCE_HAND;
				break;
			case 6:
				nowEquipPosition = CX2Unit::EP_DEFENCE_FOOT;
				break;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
			case 7:
				nowEquipPosition = CX2Unit::EP_ONEPIECE_FASHION;
				break;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
			default:
				nowEquipPosition = CX2Unit::EP_NONE;
				break;
			}
		}
		break;
	case CX2Inventory::ST_EQUIP:
		{
			switch(m_iComboSecondindex)
			{
			case 0:
				nowEquipPosition = CX2Unit::EP_NONE;
				break;
			case 1:
				nowEquipPosition = CX2Unit::EP_WEAPON_HAND;
				break;
			case 2:
				nowEquipPosition = CX2Unit::EP_DEFENCE_BODY;
				break;
			case 3:
				nowEquipPosition = CX2Unit::EP_DEFENCE_LEG;
				break;
			case 4:
				nowEquipPosition = CX2Unit::EP_DEFENCE_HAND;
				break;
			case 5:
				nowEquipPosition = CX2Unit::EP_DEFENCE_FOOT;
				break;
			default:
				nowEquipPosition = CX2Unit::EP_NONE;
				break;
			}
		}
		break;
	case CX2Inventory::ST_ACCESSORY:
		{
			switch(m_iComboSecondindex)
			{
			case 0:
				nowEquipPosition = CX2Unit::EP_NONE;
				break;
			case 1:
				nowEquipPosition = CX2Unit::EP_AC_WEAPON;
				break;
			case 2:
				nowEquipPosition = CX2Unit::EP_AC_FACE1; // 얼굴 어디?
				break;
			case 3:
				nowEquipPosition = CX2Unit::EP_AC_FACE2; // 얼굴 어디?
				break;
			case 4:
				nowEquipPosition = CX2Unit::EP_AC_FACE3; // 얼굴 어디?
				break;
			case 5:
				nowEquipPosition = CX2Unit::EP_AC_BODY;
				break;
			case 6:
				nowEquipPosition = CX2Unit::EP_AC_LEG;
				break;
			case 7:
				nowEquipPosition = CX2Unit::EP_AC_ARM;
				break;
			case 8:
				nowEquipPosition = CX2Unit::EP_AC_RING;
				break;
			case 9:
				nowEquipPosition = CX2Unit::EP_AC_NECKLESS;
				break;
			default:
				nowEquipPosition = CX2Unit::EP_NONE;
				break;
			}
		}
		break;
	case CX2Inventory::ST_QUICK_SLOT:
		nowEquipPosition = CX2Unit::EP_NONE;
		break;
	case CX2Inventory::ST_SPECIAL:
		nowEquipPosition = CX2Unit::EP_NONE;
		break;
	case CX2Inventory::ST_MATERIAL:
		nowEquipPosition = CX2Unit::EP_NONE;
		break;
	case CX2Inventory::ST_QUEST:
		nowEquipPosition = CX2Unit::EP_NONE;
		break;
	default:
		nowEquipPosition = CX2Unit::EP_NONE;
		break;
	}

	if(nowEquipPosition == CX2Unit::EP_NONE)
	{
		kPacket.m_cEqipPosition = -1;
	}
	else
	{
		kPacket.m_cEqipPosition	= static_cast< char >( nowEquipPosition );
	}


	CX2Unit::UNIT_TYPE nowUnitType = CX2Unit::UT_NONE;
	switch(m_iComboCharacterindex)
	{
	case 0:
		nowUnitType = CX2Unit::UT_NONE;
		break;
	case 1:
		nowUnitType = CX2Unit::UT_ELSWORD;
		break;
	case 2:
		nowUnitType = CX2Unit::UT_LIRE;
		break;
	case 3:
		nowUnitType = CX2Unit::UT_ARME;
		break;
	case 4:
		nowUnitType = CX2Unit::UT_RAVEN;
		break;
	case 5:
		nowUnitType = CX2Unit::UT_EVE;
		break;
#ifdef NEW_CHARACTER_CHUNG
	case 6:
		nowUnitType = CX2Unit::UT_CHUNG;
		break;
#endif NEW_CHARACTER_CHUNG

#ifndef SERV_NO_ARA
#ifdef ARA_CHARACTER_BASE
	case 7:
		nowUnitType = CX2Unit::UT_ARA;
		break;
#endif
#endif // SERV_NO_ARA
#ifdef NEW_CHARACTER_EL
#ifndef SHOPBOARD_NO_ELESIS
	case 8:
		{
			nowUnitType = CX2Unit::UT_ELESIS;
		} break;
#endif //SHOPBOARD_NO_ELESIS
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case 9:
		{
			nowUnitType = CX2Unit::UT_ADD;
		} break;
#endif //SERV_9TH_NEW_CHARACTER
	default:
		nowUnitType = CX2Unit::UT_NONE;
		break;
	}

	if(nowUnitType == CX2Unit::UT_NONE)
	{
		kPacket.m_cUnitType = -1;
	}
	else
	{
		kPacket.m_cUnitType	= static_cast< char >( nowUnitType );
	}

	CX2Unit::UNIT_CLASS nowUnitClass = CX2Unit::UC_NONE;
	switch(nowUnitType)
	{
	case CX2Unit::UT_NONE:
		nowUnitClass = CX2Unit::UC_NONE;
		break;
	case CX2Unit::UT_ELSWORD:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			case 1:
				nowUnitClass = CX2Unit::UC_ELSWORD_LORD_KNIGHT;
				break;
			case 2:
				nowUnitClass = CX2Unit::UC_ELSWORD_RUNE_SLAYER;
				break;
			case 3:
				nowUnitClass = CX2Unit::UC_ELSWORD_INFINITY_SWORD;
				break;
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		}
		break;
	case CX2Unit::UT_LIRE:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			case 1:
				nowUnitClass = CX2Unit::UC_LIRE_GRAND_ARCHER;
				break;
			case 2:
				nowUnitClass = CX2Unit::UC_LIRE_WIND_SNEAKER;
				break;
			case 3:
				nowUnitClass = CX2Unit::UC_LIRE_NIGHT_WATCHER;
				break;
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		}
		break;
	case CX2Unit::UT_ARME:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			case 1:
				nowUnitClass = CX2Unit::UC_ARME_ELEMENTAL_MASTER;
				break;
			case 2:
				nowUnitClass = CX2Unit::UC_ARME_VOID_PRINCESS;
				break;
			case 3:
				nowUnitClass = CX2Unit::UC_ARME_DIMENSION_WITCH;
				break;
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		}
		break;
	case CX2Unit::UT_RAVEN:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			case 1:
				nowUnitClass = CX2Unit::UC_RAVEN_BLADE_MASTER;
				break;
			case 2:
				nowUnitClass = CX2Unit::UC_RAVEN_RECKLESS_FIST;
				break;
			case 3:
				nowUnitClass = CX2Unit::UC_RAVEN_VETERAN_COMMANDER;
				break;
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		}
		break;
	case CX2Unit::UT_EVE:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			case 1:
				nowUnitClass = CX2Unit::UC_EVE_CODE_NEMESIS;
				break;
			case 2:
				nowUnitClass = CX2Unit::UC_EVE_CODE_EMPRESS;
				break;
			case 3:
				nowUnitClass = CX2Unit::UC_EVE_BATTLE_SERAPH;
				break;
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		}
		break;
#ifdef NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
#ifdef CHUNG_SECOND_CLASS_CHANGE
			case 1:
				nowUnitClass = CX2Unit::UC_CHUNG_IRON_PALADIN;
				break;
			case 2:
				nowUnitClass = CX2Unit::UC_CHUNG_DEADLY_CHASER;
				break;
#endif CHUNG_SECOND_CLASS_CHANGE
			case 3:
				nowUnitClass = CX2Unit::UC_CHUNG_TACTICAL_TROOPER;
				break;
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		}
		break;
#endif NEW_CHARACTER_CHUNG

#ifndef SERV_NO_ARA
#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			case 1:
				nowUnitClass = CX2Unit::UC_ARA_SAKRA_DEVANAM;
				break;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
			case 2:
				nowUnitClass = CX2Unit::UC_ARA_YAMA_RAJA;
				break;
#endif //SERV_ARA_CHANGE_CLASS_SECOND
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		} break;
#endif
#endif // SERV_NO_ARA
#ifdef NEW_CHARACTER_EL
#ifndef SHOPBOARD_NO_ELESIS
	case CX2Unit::UT_ELESIS:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
			case 1:
				nowUnitClass = CX2Unit::UC_ELESIS_GRAND_MASTER;
				break;
			case 2:
				nowUnitClass = CX2Unit::UC_ELESIS_BLAZING_HEART;
				break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		} break;
#endif //SHOPBOARD_NO_ELESIS
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CX2Unit::UT_ADD:
		{
			switch(m_iComboClassindex)
			{
			case 0:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			case 1:
				nowUnitClass = CX2Unit::UC_ADD_LUNATIC_PSYKER;
				break;
#endif //SERV_ADD_LUNATIC_PSYKER
			default:
				nowUnitClass = CX2Unit::UC_NONE;
				break;
			}
		} break;
#endif //SERV_9TH_NEW_CHARACTER
	default:
		nowUnitClass = CX2Unit::UC_NONE;
		break;
	}

	if(nowUnitClass == CX2Unit::UC_NONE)
	{
		kPacket.m_cUnitClass = -1;
	}
	else
	{
		kPacket.m_cUnitClass	= static_cast< char >( nowUnitClass );
	}


	int uMinLevel = 0;
	int uMaxLevel = 0;

	// 해외팀 변경
	if( m_iIMEEditMinLevel == 0 && m_iIMEEditMaxLevel == 0 ||
		m_iIMEEditMinLevel == 0 && m_iIMEEditMaxLevel == _CONST_X2GAME_::g_iMaxLevel ||
		m_iIMEEditMinLevel == _CONST_X2GAME_::g_iMaxLevel && m_iIMEEditMaxLevel == 0)
	{
		uMinLevel = -1;
		uMaxLevel = -1;
	}
	else if (m_iIMEEditMinLevel <= m_iIMEEditMaxLevel)
	{
		uMinLevel = m_iIMEEditMinLevel;
		uMaxLevel = m_iIMEEditMaxLevel;
	}
	else
	{
		uMinLevel = m_iIMEEditMaxLevel;
		uMaxLevel = m_iIMEEditMinLevel;
	}
	
	kPacket.m_cMinLevel =  static_cast< char >( uMinLevel );
	kPacket.m_cMaxLevel =  static_cast< char >( uMaxLevel );

	// 아이템등급 필요함

	kPacket.m_vecItemGrade.clear();


	if(m_bCheckUnique)
	{
		kPacket.m_vecItemGrade.push_back(  static_cast<char> (CX2Item::IG_UNIQUE)  );
	}

	if(m_bCheckElite)
	{
		kPacket.m_vecItemGrade.push_back(  static_cast<char> (CX2Item::IG_ELITE)  );
	}

	if(m_bCheckRare)
	{
		kPacket.m_vecItemGrade.push_back(  static_cast<char> (CX2Item::IG_RARE)  );
	}

	if(m_bCheckNormal)
	{
		kPacket.m_vecItemGrade.push_back(  static_cast<char> (CX2Item::IG_NORMAL)  );
	}

	if(m_bCheckLow)
	{
		kPacket.m_vecItemGrade.push_back(  static_cast<char> (CX2Item::IG_LOW)  );
	}

	kPacket.m_wstrFinder = m_wstrFinder;

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	/// 일치하는 단어 검색 기능 설정
	kPacket.m_bStrictSearchOption = m_bIsStrictSearchOption;
#else //SERV_UPGRADE_TRADE_SYSTEM
	kPacket.m_bStrictSearchOption = false;
#endif //SERV_UPGRADE_TRADE_SYSTEM

	g_pData->GetServerProtocol()->SendPacket( EGS_SEARCH_TRADE_BOARD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_SEARCH_TRADE_BOARD_ACK); 

	return true;
}
bool CX2UIPersonalShopBoard::Handler_EGS_SEARCH_TRADE_BOARD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	
	KEGS_SEARCH_TRADE_BOARD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_SEARCH_TRADE_BOARD_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_vecItemInfo.clear();
			m_vecItemInfo.resize( kEvent.m_vecItemInfo.size() );
			std::copy( kEvent.m_vecItemInfo.begin(), kEvent.m_vecItemInfo.end(), m_vecItemInfo.begin() );

			m_uiNowPage = 1;
			m_uiFirstPageinCurrentBoard = 1;
			if(m_vecItemInfo.size() == 0)
			{
				m_uiMaxPage = 0;
			}
			else
			{
				m_uiMaxPage = static_cast< int >( (m_vecItemInfo.size() - 1) / _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage) + 1;
			}
		
			if(GetShow() == true)
			{
				if(m_bCheckUsable == true)
				{
					UsableFilter();
				}
				else
				{
					UpdateItemList();
				}
			}
		}
		else
		{
			m_vecItemInfo.clear();
			m_uiNowPage = 1;
			m_uiFirstPageinCurrentBoard = 1;
			m_uiMaxPage = 0;
			if(GetShow() == true)
			{
				UpdateItemList();
			}
		}
	}
	return true;
}

void CX2UIPersonalShopBoard::UpdateItemList()
{
	int uiFirstItemidx = _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage * (m_uiNowPage - 1);
	int uiLastItemidx = m_vecItemInfo.size() - 1;

	WCHAR buff[256] = {0,};

	for ( int i = 0; i < _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage; i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);


		if( uiFirstItemidx + i > uiLastItemidx)
		{
			if ( pItemSlot != NULL )
			{
				pItemSlot->DestroyItemUI();
				pItemSlot->SetShow(false);
				pItemSlot->SetEnable(false);
			}

			m_pItemList[i]->SetShowEnable(false, false);
			m_iPersonalShopUidList[i] = -1;
#ifdef SERV_PSHOP_AGENCY
			m_iShopOwnerUidList[i] = -1;
#endif
		}
		else
		{
			// 아이템 정보를 리스트에 담는다.
			KTradeBoardItemInfo& kTradeBoardItemInfo = m_vecItemInfo[uiFirstItemidx + i];
	
			UidType uItemUID = kTradeBoardItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID;
			int iItemID = kTradeBoardItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
			int iItemQuantity = kTradeBoardItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity;
			int iED = kTradeBoardItemInfo.m_kSellItemInfo.m_iPricePerOne;

#ifndef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	/// 게시판 구매 수수료 제게
	#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
			if( kTradeBoardItemInfo.m_kSellItemInfo.m_cPShopItemType == KSellPersonalShopItemInfo::SPIT_PERSONAL_SHOP )
			{
				int iCommissionED = static_cast<int> (kTradeBoardItemInfo.m_kSellItemInfo.m_iPricePerOne * _CONST_UIPERSONALSHOPBOARD_INFO_::g_dPersonalShopBoardFee);
				iED += iCommissionED;
			}
	#else
			int iCommissionED = static_cast<int> (kTradeBoardItemInfo.m_kSellItemInfo.m_iPricePerOne * _CONST_UIPERSONALSHOPBOARD_INFO_::g_dPersonalShopBoardFee);
			iED += iCommissionED;
	#endif
#endif //SERV_UPGRADE_TRADE_SYSTEM

			wstring wstrSellerNickName = kTradeBoardItemInfo.m_wstrSellerNickName;

			CX2Item::ITEM_GRADE iGrade = g_pData->GetItemManager()->GetItemTemplet( iItemID )->GetItemGrade();
			m_iSelectedPersonalShopUid = kTradeBoardItemInfo.m_iPersonalShopUID;

			m_iPersonalShopUidList[i] = kTradeBoardItemInfo.m_iPersonalShopUID;
#ifdef SERV_PSHOP_AGENCY
			m_iShopOwnerUidList[i] = kTradeBoardItemInfo.m_iHostUnitUID;
#endif

			if ( pItemSlot != NULL )
			{

				//CX2Item::ItemData* pItemData = new CX2Item::ItemData( kTradeBoardItemInfo.m_kSellItemInfo.m_kInventoryItemInfo);
				//CX2Item* pItem = new CX2Item( pItemData, NULL );

				pItemSlot->CreateItemUI( iItemID );
				pItemSlot->SetItemUID( uItemUID );
				pItemSlot->SetShow(true);
				pItemSlot->SetEnable(true);
			}

            CX2Item* pItem = NULL;
            {
			    CX2Item::ItemData kItemData( kTradeBoardItemInfo.m_kSellItemInfo.m_kInventoryItemInfo );
			    pItem = new CX2Item( kItemData, NULL );
            }
		
			CKTDGUIStatic* pStaticItemName = NULL;
			pStaticItemName = static_cast< CKTDGUIStatic* >( m_pItemList[i]->GetControl(L"StaticItemName") );


			wstring wstrItemName = g_pData->GetItemManager()->GetItemTemplet( iItemID )->GetNameColor_();

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			wstrItemName += pItem->GetFullName( true );		/// 접두어 추가해서 반환
			wstrItemName += L"#CX";

			/// 만약 이름이 너무 길어서 2줄로 나눌 땐, 텍스트 위치 약간 위로 올려주자
			const wstring wstrLineBreakItemName = g_pMain->GetStrByLineBreakColor( wstrItemName.c_str(), 230, static_cast<int>( XUF_DODUM_13_SEMIBOLD ) );

			if ( wstrItemName != wstrLineBreakItemName )
			{
				const D3DXVECTOR2 vecItemNameOffSet = pStaticItemName->GetOffsetPos();
				pStaticItemName->SetOffsetPos( D3DXVECTOR2( 0.f, -8.f ) );
				pStaticItemName->GetString(0)->msg = wstrLineBreakItemName;
			}
			else
			{
				pStaticItemName->SetOffsetPos( D3DXVECTOR2( 0.f, 0.f ) );
				pStaticItemName->GetString(0)->msg = wstrItemName;
			}

#else // SERV_UPGRADE_TRADE_SYSTEM
			wstrItemName += pItem->GetFullName();
			wstrItemName += L"#CX";
			
#ifdef CLIENT_GLOBAL_LINEBREAK
			// 임홍락 07.24 상점검색창 아이템 이름 길어 UI를 벗어나는 문제 수정
			wstrItemName = CWordLineHandler::GetStrByLineBreakColorInX2Main(wstrItemName.c_str(), 250, SLOT_MANAGER_FONT_INDEX);
#endif //CLIENT_GLOBAL_LINEBREAK

			pStaticItemName->GetString(0)->msg = wstrItemName;
#endif // SERV_UPGRADE_TRADE_SYSTEM
	
			CKTDGUIStatic* pStaticNum = NULL;
			pStaticNum = static_cast< CKTDGUIStatic* >( m_pItemList[i]->GetControl(L"StaticNum") );
			//wsprintf( buff, L"%d개", iItemQuantity );
			// 해외팀 하드코딩 스트링으로 변경
			StringCchPrintf( buff, 256, GET_STRING( STR_ID_5175 ), iItemQuantity );

			pStaticNum->SetString(0, buff);


			CKTDGUIStatic* pStaticED = NULL;
			pStaticED = static_cast< CKTDGUIStatic* >( m_pItemList[i]->GetControl(L"StaticED") );

			wstring wstrED = g_pMain->GetEDString(iED);
			wstrED += L" ED";
			pStaticED->GetString(0)->msg = wstrED;
			pStaticED->SetColor( g_pMain->GetPriceColor( iED ) );


			CKTDGUIStatic* pStaticSeller = NULL;
			pStaticSeller = static_cast< CKTDGUIStatic* >( m_pItemList[i]->GetControl(L"g_pStatic_User") );
			pStaticSeller->SetString(0, wstrSellerNickName.c_str());


			if ( pItem->GetItemData().m_bIsSealed == true )
			{
				pItemSlot->SetShowSealedImage( true );
			}
			else
			{
				pItemSlot->SetShowSealedImage( false );
			}

	#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			/// 만약 내가 판매하는 아이템 이라면, 구입 버튼 숨겨버리자
			CKTDGUIButton* pButton			= static_cast< CKTDGUIButton* >( m_pItemList[i]->GetControl( L"ButtonBuyItem" ) );		/// 구매 버튼
			CKTDGUIStatic* pButtonName		= static_cast< CKTDGUIStatic* >( m_pItemList[i]->GetControl( L"g_pStatic_BUTTON" ) );	/// 구매 버튼 문구
			CKTDGUIStatic* pStaticMyItem	= static_cast< CKTDGUIStatic* >( m_pItemList[i]->GetControl(L"StaticMyItem") );			/// 내가 등록한 아이템 안내 문구

			if ( NULL != pButton && NULL != pButtonName && NULL != pStaticMyItem )
			{
				bool bMyItem = false;		/// 내 아이템 여부

				/// 내가 등록한 아이템인지 검사
				if ( NULL != g_pData &&
					 NULL != g_pData->GetMyUser() &&
					 NULL != g_pData->GetMyUser()->GetSelectUnit() &&
					 g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kTradeBoardItemInfo.m_iHostUnitUID )
				{
					bMyItem = true;		/// 내가 올린 아이템이네!
				}

				pStaticMyItem->SetShow( bMyItem );		/// 내가 등록한 아이템 안내 문구 설정	( 내가 올린 아이템일 때 표시 )
				pButton->SetShow( !bMyItem );			/// 구매 버튼 설정 ( 내가 올린 아이템이 아닐 때 표시 )
				pButtonName->SetShow( !bMyItem );		/// 구매 버튼 문구 설정	( 내가 올린 아이템이 아닐 때 표시 )
			}
	#endif //SERV_UPGRADE_TRADE_SYSTEM



			SAFE_DELETE( pItem );

			m_pItemList[i]->SetShowEnable(true, true);

		}
	}

	UINT uiFirstPage = m_uiFirstPageinCurrentBoard;
	int iNumDisplayPageButton = 0;

	// 모든 페이지를 표기해야하는 경우, 즉 페이지 버튼의 이동이 필요없는 경우 
	if( uiFirstPage + _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard - 1 <= m_uiMaxPage )
	{
		iNumDisplayPageButton = _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard;
	}
	else
	{
		iNumDisplayPageButton = m_uiMaxPage - uiFirstPage + 1;
	}
	
	float fWidthPageButton = (_CONST_UIPERSONALSHOPBOARD_INFO_::g_fWidthPage) / (_CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard);
	float fOffsetPageButton = (-fWidthPageButton * iNumDisplayPageButton) + (fWidthPageButton * _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard);

	// 페이지 버튼을 업데이트 한다.
	for( UINT i = 0; i < _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard; i++)
	{
		if( uiFirstPage + i > m_uiMaxPage) // 최대 페이지를 넘어선 경우 페이지 스트링 출력 X 
		{
			m_pStaticPageNum->SetString( i, L"");
			m_pButtonPageNum[i]->SetShowEnable(false, false);
		}
		else if( uiFirstPage + i == m_uiNowPage ) // 현재 페이지 버튼
		{
			_itow( uiFirstPage + i, buff, 10 );
			m_pStaticPageNum->SetString( i, buff );
			m_pStaticPageNum->GetString( i )->color = D3DXCOLOR(0.6f,0.0f,0.0f,1.0f);
			m_pButtonPageNum[i]->SetShowEnable(true, false);
			
		}
		else
		{
			_itow( uiFirstPage + i, buff, 10 );
			m_pStaticPageNum->SetString( i, buff );
			m_pStaticPageNum->GetString( i )->color = D3DXCOLOR(0.4f,0.4f,0.4f,1.0f);
			m_pButtonPageNum[i]->SetShowEnable(true, true);
			
		}
		m_pButtonPageNum[i]->SetOffsetPos_LUA(fOffsetPageButton, 0.f);
	}
	m_pStaticPageNum->SetOffsetPos_LUA(fOffsetPageButton, 0.f);

	// 화살표 버튼 업데이트, UI event의 예외 처리 역할도 함
	// <<- //

	
	if(iNumDisplayPageButton == 0)
	{
		m_pButtonPrevBoard->SetShowEnable(false, false);
		m_pButtonPrevPage->SetShowEnable(false, false);
		m_pButtonNextPage->SetShowEnable(false, false);
		m_pButtonNextBoard->SetShowEnable(false, false);
		return;
	}


	float fOffsetArrow = 10.f;
	if(m_uiFirstPageinCurrentBoard == 1)
	{
		m_pButtonPrevBoard->SetShowEnable(true, false);
	}
	else
	{
		m_pButtonPrevBoard->SetShowEnable(true, true);
	}
	m_pButtonPrevBoard->SetOffsetPos_LUA(fOffsetPageButton - fOffsetArrow, 0.f);
	// <- //
	if(m_uiNowPage == 1)
	{
		m_pButtonPrevPage->SetShowEnable(true, false);
	}
	else
	{
		m_pButtonPrevPage->SetShowEnable(true, true);
	}
	m_pButtonPrevPage->SetOffsetPos_LUA(fOffsetPageButton - fOffsetArrow, 0.f);
	// -> //
	if(m_uiNowPage >= m_uiMaxPage)
	{
		m_pButtonNextPage->SetShowEnable(true, false);
	}
	else
	{
		m_pButtonNextPage->SetShowEnable(true, true);
	}
	m_pButtonNextPage->SetOffsetPos_LUA(-fOffsetPageButton + fOffsetArrow, 0.f);
	// ->> //
	if(m_uiMaxPage < m_uiFirstPageinCurrentBoard + _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard )
	{
		m_pButtonNextBoard->SetShowEnable(true, false);
	}
	else
	{
		m_pButtonNextBoard->SetShowEnable(true, true);
	}
	m_pButtonNextBoard->SetOffsetPos_LUA(-fOffsetPageButton + fOffsetArrow, 0.f);

	
	if( m_uiMaxPage < _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard + 1 )
	{
		m_pButtonPrevBoard->SetShow(false);
		m_pButtonNextBoard->SetShow(false);
	}
	return;
}


void CX2UIPersonalShopBoard::UpdateCheckBoxColor()
{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	if ( NULL != m_pCheckALL )
	{
		/// 다 체크 되어 있으면, 전체 항목도 체크 해주세요.
		if ( true == m_bCheckLow && true == m_bCheckNormal && true == m_bCheckRare && true == m_bCheckElite && true == m_bCheckUnique )
			m_pCheckALL->SetChecked( true );
		/// 하나라도 체크 풀리면, 전체 항목도 체크 해제 해주세요.
		else
			m_pCheckALL->SetChecked( false );
	}
#else // SERV_UPGRADE_TRADE_SYSTEM
	if(m_bCheckLow)
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );
		pStaticGrade->GetString(4)->color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );
		pStaticGrade->GetString(4)->color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	}
	if(m_bCheckNormal)
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );

		pStaticGrade->GetString(3)->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );
		pStaticGrade->GetString(3)->color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	}
	if(m_bCheckRare)
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );

		pStaticGrade->GetString(2)->color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );
		pStaticGrade->GetString(2)->color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	}
	if(m_bCheckElite)
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );
		pStaticGrade->GetString(1)->color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );
		pStaticGrade->GetString(1)->color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	}
	if(m_bCheckUnique)
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );
		pStaticGrade->GetString(0)->color = D3DXCOLOR(1.0f, 0.8f, 0.56f, 1.0f);
	}
	else
	{
		CKTDGUIStatic* pStaticGrade = NULL;
		pStaticGrade = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticGrade") );
		pStaticGrade->GetString(0)->color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	}
#endif // SERV_UPGRADE_TRADE_SYSTEM

	return;
}



void CX2UIPersonalShopBoard::EnableButtonSearchItem(bool bEnable)
{
	if(bEnable)
	{
		CKTDGUIStatic* pStaticSearch = NULL;
		pStaticSearch = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticSearch") );
		pStaticSearch->GetString(0)->color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		pStaticSearch->GetString(1)->color = D3DXCOLOR(0.0f, 0.9f, 1.0f, 1.0f);
		m_pButtonSearchItem->SetEnable(true);
	}
	else
	{
		CKTDGUIStatic* pStaticSearch = NULL;
		pStaticSearch = static_cast< CKTDGUIStatic* >( m_pDlgPersonalShopBoard->GetControl(L"StaticSearch") );
		pStaticSearch->GetString(0)->color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		pStaticSearch->GetString(1)->color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
		m_pButtonSearchItem->SetEnable(false);
	}
	return;
}


void CX2UIPersonalShopBoard::UpdateButtonSearchItem()
{
	m_fCurrentTime = (float)DXUTGetGlobalTimer()->GetTime();

	if( m_fCurrentTime > m_fLastSearchTime + 3.0f)
	{
		// 검색 조건에 따라 검색 버튼 활성, 비활성화
		if(m_iComboFirstindex != 0)
		{
			EnableButtonSearchItem(true);
		}
		else if (m_wstrFinder != L"") // 검색어 존재하면 첫번째 카테고리 없어도 검색 가능
		{
			EnableButtonSearchItem(true);
		}
		else
		{
			EnableButtonSearchItem(false);
		}
	}
	else
	{
		EnableButtonSearchItem(false);
	}

	return;
}


bool CX2UIPersonalShopBoard::GetShow() const
{
	return m_bShow;
}

void CX2UIPersonalShopBoard::SetShow( bool bShow )
{
	m_bShow = bShow;

	if ( bShow )
	{
		g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_MENU_PERSONAL_SHOP_BOARD );
		UpdateItemList();
	}
	else
	{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
		/// 창 숨길 때, 검색어 자동 완성창도 같이 숨기자.
		SetShowSearchKeywordList( false );

		CX2UIPersonalShop* pUIPersonalShop = g_pData->GetUIManager()->GetPersonalShop();

		/// 개인 상점에 접속중이라면, 쫓아내자.
		if ( NULL != pUIPersonalShop )
		{
			pUIPersonalShop->SetSelectedItemID( -1 );

			if ( CX2UIPersonalShop::XPSS_BUY == pUIPersonalShop->GetPersonalShopState() )
				g_pData->GetUIManager()->GetPersonalShop()->Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();
		}
#endif //SERV_UPGRADE_TRADE_SYSTEM

		g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_MENU_PERSONAL_SHOP_BOARD );
	
		for ( int i = 0; i < _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage; i++ )
		{
			m_pItemList[i]->SetShowEnable(false, false);
		}


		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
			if ( pItemSlot != NULL )
			{
				pItemSlot->DestroyItemUI();
				pItemSlot->SetShow(false);
				pItemSlot->SetEnable(false);
			}
		}
		InvalidSelectedItem();
		InvalidSlotDesc();

	}

	m_pDlgPersonalShopBoard->SetShowEnable( m_bShow, m_bShow );
	
}

D3DXVECTOR2	CX2UIPersonalShopBoard::GetDLGSize() const
{
	return m_vSizeDlg;		// 임시
}

void CX2UIPersonalShopBoard::SetPosition( D3DXVECTOR2 vec )
{
	m_vPosDlg = vec;
	m_pDlgPersonalShopBoard->SetPos( m_vPosDlg );
}

void CX2UIPersonalShopBoard::SetLayer( X2_DIALOG_LAYER layer )
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);


	for ( int i = 0; i < _CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage; i++ )
	{
		if(m_pItemList[i] != NULL)
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pItemList[i], layer);
		}	
	}

	if( m_pDlgPersonalShopBoard != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgPersonalShopBoard, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgPersonalShopBoard, false );
	}
}

//{{ kimhc // 2011-03-27 // 상점검색에서 미리 입어보기
#ifdef	PRE_EQUIP_PERSONAL_SHOP_BOARD

bool CX2UIPersonalShopBoard::MouseRButtonUp( const D3DXVECTOR2& v2MousePos )
{
	CX2SlotItem* pSlotItem = static_cast<CX2SlotItem*>( GetSlotInMousePos( v2MousePos ) );
	if ( NULL == pSlotItem )
		return false;

	const int iItemID = pSlotItem->GetItemTID();
    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );

	// 장착 가능한 타입이 아니면
	if ( CX2Item::UT_NONE == pItemTemplet->GetUseType() )
		return false;

	const CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();

	// 아이템의 장착 조건을 확인
	switch ( pItemTemplet->GetUseCondition() )
	{
	case CX2Item::UC_ANYONE:		// 누구나 장착할 수 있다면 통과
		break;
		
	case CX2Item::UC_ONE_UNIT:		// 유닛의 타입에 따라 장착 할 수 있다면
		{
			// 유닛 타입만 체크한 후, 타입이 다르면 return false
			if ( pMyUnit->GetType() != pItemTemplet->GetUnitType() )
				return false;

			// 타입이 같으면 통과
		} break;

	case CX2Item::UC_ONE_CLASS:		// 한 클래스만 장착 할 수 있다면
		{
			// 클래스를 체크한 후, 클래스가 다르면 return false
			if ( pMyUnit->GetClass() != pItemTemplet->GetUnitClass() )
				return false;

			// 클래스가 같으면 통과
		} break;

	default:
		return false;
		break;
	}	

	g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_CHARINFO, true );

	CX2UICharInfo* pUICharInfo = g_pData->GetUIManager()->GetUICharInfo();
	pUICharInfo->GetUnitViewerUI()->AddEquipByTID( iItemID );

	return true;
}

#endif	PRE_EQUIP_PERSONAL_SHOP_BOARD
//}} kimhc // 2011-03-27 // 상점검색에서 미리 입어보기



#endif DEF_TRADE_BOARD

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
/** @function	: UpdateAutoWriteSearchKewordBox
	@brief		: 검색어 자동 완성 창 갱신
	#param		: 검색어
*/
void CX2UIPersonalShopBoard::UpdateSearchKewordList( IN const wstring& wstrSearchKeyword_ )
{
	/// 객체 없으면 리턴
	if ( NULL == m_pListboxSearchKeyword )
		return;

	/// 객체가 없거나, 검색어가 두글자 이하라면 자동 완성창 숨김
	if ( NULL == m_pListboxSearchKeyword || 
		 2 > wstrSearchKeyword_.length() )
	{
		SetShowSearchKeywordList( false );

		return;
	}

	vector<wstring> vecSearchKeywordList;		/// 검색 키워드 리스트
	
	/// 검색 수행 및 결과값 출력
	/// TODO : 만약 검색 로직을 바꾸고자 한다면, 아래 함수에서 변경하면 됩니다!!!
	SetSearchKeywordList( wstrSearchKeyword_, vecSearchKeywordList );

	/// 검색 결과가 없으면 자동 완성창 숨김
	if ( true == vecSearchKeywordList.empty() )
	{
		SetShowSearchKeywordList( false );

		return;
	}


	/// 검색어 리스트 초기화
	m_pListboxSearchKeyword->RemoveAllItems();

	/// 리스트에 10개만 저장
	BOOST_FOREACH( const wstring& wstrSearchKeyword, vecSearchKeywordList )
	{
		m_pListboxSearchKeyword->AddItem( wstrSearchKeyword.c_str(), NULL );
	}

	/// 자동 완성창 활성
	SetShowSearchKeywordList( true );
}

/** @function	: SearchKeywordList
	@brief		: 검색어를 통해 검색 연산 수행 및 결과값 출력 함수
	@param		: 검색어, 검색 결과값 저장 컨테이너
*/
///TODO : 검색 로직을 바꾸고자 한다면, 날 수정해 주세요!
void CX2UIPersonalShopBoard::SetSearchKeywordList( IN const wstring wstrSearchKeyword, OUT vector<wstring>& vevSearchKeyword_ )
{
	vevSearchKeyword_.resize( 0 );

	/// 등록된 아이템 항목 순회
	unsigned int uiNumItems = g_pData->GetItemManager()->GetItemNum();

	for( unsigned i = 0; i != uiNumItems; ++i )
	{	
		const CX2Item::ItemTemplet* pItemTemplet = NULL;

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		pItemTemplet = g_pData->GetItemManager()->GetItemTempletFromIndex( i );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

		if ( NULL == pItemTemplet )
			return;

		/// 아이템 이름 검색 ( 순회를 최소화 시키기 위해, 스트링 검색부터 하자. )
		const wstring wstrTempItemName = pItemTemplet->GetName();

		if( NULL == wcsstr( wstrTempItemName.c_str(), wstrSearchKeyword.c_str() ) )
			continue;

		/// 귀속이면서, 봉인횟수가 0 이라면 검색에서 제외
		if ( true == pItemTemplet->GetVested() && 0 >= pItemTemplet->GetMaxSealCount() )
			continue;

		/// PC방 아이템은 제외
		if ( true == pItemTemplet->GetIsPcBang() )
			continue;

		/// 순간 소비성 아이템은 제외
		if ( CX2Item::IT_OUTLAY == pItemTemplet->GetItemType() ) 
			continue;

		/// 카테고리에 설정한 내용만 추가
		if ( false == CheckSelectCategoryOption( pItemTemplet ) )
			continue;

		/// 일치하는 항목이 있다면, 저장
		vevSearchKeyword_.push_back( wstrTempItemName );
	}


	/// 혹시 출력하면 않되는 아이템이 있다면 걸러내자. ( PersonalShopAutoSearchBanList.lua )
	for ( UINT i = 0; i < vevSearchKeyword_.size(); ++i )
	{
		const wstring wstrItemName = vevSearchKeyword_[i];

		/// 스크립트에서 받아온 "출력하면 않되는 아이템 이름 리스트"
		BOOST_FOREACH( wstring wstrBanItemName, m_vecBanItemList )
		{
			/// 해당 이름이 포함된 아이템이 있다면, 자동 완성 리스트에서 제거
			if( NULL != wcsstr( wstrItemName.c_str(), wstrBanItemName.c_str() ) )
			{
				vevSearchKeyword_.erase( vevSearchKeyword_.begin() + i );

				--i;
				break;
			}
		}
	}


	/// 오름차순으로 정렬
	auto SearchKeywordSortFunc = [](const wstring& lhs_, const wstring& rhs_ )->bool
	{
		return rhs_.length() > lhs_.length();
	};

	std::sort( vevSearchKeyword_.begin(), vevSearchKeyword_.end(), SearchKeywordSortFunc );
}

/** @function	: SetShowSearchKeywordList
	@brief		: 검색어 자동 완성창 표시 여부 설정
	@param		: 표시 여부
*/
void CX2UIPersonalShopBoard::SetShowSearchKeywordList( IN bool bShow_ )
{
	if ( NULL != m_pDLGAutoSearchKeyword )
	{
		/// 만약 숨길꺼면, 리스트 박스에 저장되어 있는 모든 정보도 같이 지우자
		if ( false == bShow_ )
		{
			m_pListboxSearchKeyword->RemoveAllItems();

			/// 혹시 닫기 대기중이라면, 무효 시키자. ( 어짜피 여기서 닫으니까... )
			m_iCloseSearchKeywordListTime	= 0;

			/// 숨겼으니, 자동 완성 동작도 그만 하세요.
			m_fActiveSearchKeywordListTime	= 0.f;

			/// 숨겼으니, 포커스 넘어갔을 때 처리하는 기능도 그만 해요.
			m_iFocusOutSearchKeywordBoxTime = 0;
		}

		m_pDLGAutoSearchKeyword->SetShow( bShow_ );
	}
}

/** @function	: SetSearchKeywordBySelectedItem
	@brief		: 리스트 박스 내 선택된 아이템을 검색어로 지정
*/
void CX2UIPersonalShopBoard::SetSearchKeywordBySelectedItem()
{
	if ( NULL != m_pListboxSearchKeyword && true == m_pListboxSearchKeyword->GetShow() )
	{
		CKTDGUIListBox::ListBoxItem* pItem = m_pListboxSearchKeyword->GetSelectedItem();

		if( NULL != pItem )
		{
			/// 검색어로 설정
			m_wstrFinder = pItem->strText;

			/// 검색창에 설정
			if ( NULL != m_pIMEEditItemName )
				m_pIMEEditItemName->SetText( pItem->strText );
		}
	}
}

/** @function	: SetFocusOutSearchKeyword
	@brief		: 검색어 입력창 포커스 이탈시 처리 함수
*/
void CX2UIPersonalShopBoard::SetFocusOutSearchKeyword()
{
	if(m_pIMEEditItemName->GetTextLength() == 0)	//// 입력 내용이 있는지 검사
	{
		m_wstrFinder = L"";
	}	
	if(m_pIMEEditItemName->GetTextLength() == 1)	/// 2자 이상 입력 체크
	{
		m_wstrFinder = L"";
		m_pIMEEditItemName->SetText(L"", false);
		/// 2자 이상 입력하시죠!
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),  GET_STRING(STR_ID_4982), g_pMain->GetNowState() );
	}
	else
	{
		wstring wstrSearchKeyword = m_wstrFinder = m_pIMEEditItemName->GetNowText();
	}

	UpdateButtonSearchItem();			/// 검색버튼 활성, 비활성화

	SetShowSearchKeywordList( false );	/// 검색어 자동 완성창 숨김

	/// 혹시 해당 함수 처리 대기중이라면, 무효 시키자. ( 어짜피 여기서 처리 니까... )
	m_iFocusOutSearchKeywordBoxTime = 0;
}

/** @function	: CheckItemGrade
	@brief		: 현재 아이템 요구 레벨이 선택한 아이템 조건과 맞는지 검사
	@param		: 아이템 요구 레벨
	@return		: 조건 만족 여부
*/
bool CX2UIPersonalShopBoard::CheckSelectCategoryOption( IN const CX2Item::ItemTemplet* pItemTemplet_ )
{
	if ( NULL == pItemTemplet_ )
		return false;

	/// 종류
	const CX2Item::ITEM_TYPE eITemType = pItemTemplet_->GetItemType();

	switch( m_iComboFirstindex )
	{
	case 1:		/// ST_AVARTA
		{
			/// 패션 아이템이 아니면, 아바타가 아니다.
			if ( false == pItemTemplet_->GetFashion() )
				return false;
		} break;
	case 2:		/// ST_EQUIP
		{
			if ( CX2Item::IT_WEAPON != eITemType && CX2Item::IT_DEFENCE != eITemType )
				return false;
		} break;
	case 3:		/// ST_ACCESSORY
		{
			if ( CX2Item::IT_ACCESSORY != eITemType )
				return false;
		} break;
	case 4:		/// ST_QUICK_SLOT
		{
			if ( CX2Item::IT_QICK_SLOT != eITemType )
				return false;
		} break;
	case 5:		/// ST_SPECIAL
		{
			if ( CX2Item::IT_SPECIAL != eITemType )
				return false;
		} break;
	case 6:		/// ST_MATERIAL
		{
			if ( CX2Item::IT_MATERIAL != eITemType )
				return false;
		} break;
	case 7:		/// ST_QUEST
		{
			if ( CX2Item::IT_QUEST != eITemType )
				return false;
		} break;
	}
	

	/// 부위
	const CX2Unit::EQIP_POSITION eItemEquipPosition = pItemTemplet_->GetEqipPosition();

	switch( m_iComboFirstindex )
	{
	case 1:		/// ST_AVATAR
	case 2:		/// ST_EQIP
		{
			switch( m_iComboSecondindex )
			{
			case 1:
				if ( eItemEquipPosition != CX2Unit::EP_WEAPON_HAND )
					return false;
				break;
			case 2:
				if ( eItemEquipPosition != CX2Unit::EP_DEFENCE_BODY )
					return false;
				break;
			case 3:
				if ( eItemEquipPosition != CX2Unit::EP_DEFENCE_LEG )
					return false;
				break;
			case 4:
				if ( eItemEquipPosition != CX2Unit::EP_DEFENCE_HAIR )
					return false;
				break;
			case 5:
				if ( eItemEquipPosition != CX2Unit::EP_DEFENCE_HAND )
					return false;
				break;
			case 6:
				if ( eItemEquipPosition != CX2Unit::EP_DEFENCE_FOOT )
					return false;
				break;
			}
		} break;

	case 3:		/// ST_ACCESSORY
		{
			switch( m_iComboSecondindex )
			{
			case 1:
				if ( eItemEquipPosition != CX2Unit::EP_AC_WEAPON )
					return false;
				break;
			case 2:
				if ( eItemEquipPosition != CX2Unit::EP_AC_FACE1 )
					return false;
				break;
			case 3:
				if ( eItemEquipPosition != CX2Unit::EP_AC_FACE2 )
					return false;
				break;
			case 4:
				if ( eItemEquipPosition != CX2Unit::EP_AC_FACE3 )
					return false;
				break;
			case 5:
				if ( eItemEquipPosition != CX2Unit::EP_AC_BODY )
					return false;
				break;
			case 6:
				if ( eItemEquipPosition != CX2Unit::EP_AC_LEG )
					return false;
			case 7:
				if ( eItemEquipPosition != CX2Unit::EP_AC_ARM )
					return false;
				break;
			case 8:
				if ( eItemEquipPosition != CX2Unit::EP_AC_RING )
					return false;
				break;
			case 9:
				if ( m_iComboSecondindex != CX2Unit::EP_AC_NECKLESS )
					return false;
				break;
			}
		}
	}

	/// 사용 가능 케릭터 및 사용 가능 클래스
	const CX2Unit::UNIT_TYPE	eUnitType	= pItemTemplet_->GetUnitType();
	const CX2Unit::UNIT_CLASS	eUnitClass	= pItemTemplet_->GetUnitClass();

	switch ( m_iComboCharacterindex )
	{
	case 1:
		{
			if ( eUnitType != CX2Unit::UT_ELSWORD )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_ELSWORD_LORD_KNIGHT )
							return false;
					} break;
				case 2:
					{
						if ( eUnitClass != CX2Unit::UC_ELSWORD_RUNE_SLAYER )
							return false;
					} break;
				case 3:
					{
						if ( eUnitClass != CX2Unit::UC_ELSWORD_INFINITY_SWORD )
							return false;
					} break;
				} break;
			}
		} break;
	case 2:
		{
			if ( eUnitType != CX2Unit::UT_LIRE )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_LIRE_GRAND_ARCHER )
							return false;
					} break;
				case 2:
					{
						if ( eUnitClass != CX2Unit::UC_LIRE_WIND_SNEAKER )
							return false;
					} break;
				case 3:
					{
						if ( eUnitClass != CX2Unit::UC_LIRE_NIGHT_WATCHER )
							return false;
					} break;
				} break;
			}
		} break;
	case 3:
		{
			if ( eUnitType != CX2Unit::UT_ARME )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_ARME_ELEMENTAL_MASTER )
							return false;
					} break;
				case 2:
					{
						if ( eUnitClass != CX2Unit::UC_ARME_VOID_PRINCESS )
							return false;
					} break;
				case 3:
					{
						if ( eUnitClass != CX2Unit::UC_ARME_DIMENSION_WITCH )
							return false;
					} break;
				} break;
			}
		} break;
	case 4:
		{
			if ( eUnitType != CX2Unit::UT_RAVEN )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_RAVEN_BLADE_MASTER )
							return false;
					} break;
				case 2:
					{
						if ( eUnitClass != CX2Unit::UC_RAVEN_RECKLESS_FIST )
							return false;
					} break;
				case 3:
					{
						if ( eUnitClass != CX2Unit::UC_RAVEN_VETERAN_COMMANDER )
							return false;
					} break;
				} break;
			}
		} break;
	case 5:
		{
			if ( eUnitType != CX2Unit::UT_EVE )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_EVE_CODE_NEMESIS )
							return false;
					} break;
				case 2:
					{
						if ( eUnitClass != CX2Unit::UC_EVE_CODE_EMPRESS )
							return false;
					} break;
				case 3:
					{
						if ( eUnitClass != CX2Unit::UC_EVE_BATTLE_SERAPH )
							return false;
					} break;
				} break;
			}
		} break;
	case 6:
		{
			if ( eUnitType != CX2Unit::UT_CHUNG )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_CHUNG_IRON_PALADIN )
							return false;
					} break;
				case 2:
					{
						if ( eUnitClass != CX2Unit::UC_CHUNG_DEADLY_CHASER )
							return false;
					} break;
				case 3:
					{
						if ( eUnitClass != CX2Unit::UC_CHUNG_TACTICAL_TROOPER )
							return false;
					} break;
				} break;
			}
		} break;
	case 7:
		{
			if ( eUnitType != CX2Unit::UT_ARA )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_ARA_SAKRA_DEVANAM )
							return false;
					} break;
				case 2:
					{
						if ( eUnitClass != CX2Unit::UC_ARA_YAMA_RAJA )
							return false;
					} break;
				} break;
			}
		} break;
	case 8:
		{
			if ( eUnitType != CX2Unit::UT_ELESIS )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_ELESIS_GRAND_MASTER )
							return false;
					} break;
				case 2:
					{
						if ( eUnitClass != CX2Unit::UC_ELESIS_BLAZING_HEART )
							return false;
					} break;
				} break;
			}
		} break;
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case 9:
		{
			if ( eUnitType != CX2Unit::UT_ADD )
				return false;
			else
			{
				switch(m_iComboClassindex)
				{
				case 1:
					{
						if ( eUnitClass != CX2Unit::UC_ADD_LUNATIC_PSYKER )
							return false;
					} break;
				} break;
			}
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
	}


	/// 사용 가능 레벨
	const UINT iUseLevel = pItemTemplet_->GetUseLevel();
	if ( m_iIMEEditMinLevel > iUseLevel && m_iIMEEditMaxLevel < iUseLevel )
		return false;



	/// 등급
	const CX2Item::ITEM_GRADE eItemGrade_ = pItemTemplet_->GetItemGrade();
	if ( CX2Item::IG_LOW == eItemGrade_ && true != m_bCheckLow )
		return false;
	else if ( CX2Item::IG_NORMAL == eItemGrade_ && true != m_bCheckNormal  )
		return false;
	else if ( CX2Item::IG_RARE == eItemGrade_ && true != m_bCheckRare )
		return false;
	else if ( CX2Item::IG_ELITE == eItemGrade_ && true != m_bCheckElite )
		return false;
	else if ( CX2Item::IG_UNIQUE == eItemGrade_ && true != m_bCheckUnique )
		return false;
	
	return true;
}

/** @function	: OpenScriptFile
	@brief		: 검색어 자동 완성 창에 나오면 않되는 아이템 리스트 스크립트 파싱 함수
	@param		: 스크립트 이름
	@return		: 파싱 여부
*/
bool CX2UIPersonalShopBoard::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIPersonalShopBoard", this );

	if( NULL != g_pKTDXApp &&
		NULL != g_pKTDXApp->GetDeviceManager() &&
		false == g_pKTDXApp->LoadLuaTinker( pFileName ) )
	{
		ASSERT( !"PersonalShopAutosearchBanList Is Read Fail!!!" );

		return false;
	}

	return true;
}

/** @function	: AddBanItemNameList
	@brief		: 검색어 자동 완성 창에 나오면 않되는 아이템 리스트 추가
*/
void CX2UIPersonalShopBoard::AddBanItemNameList_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );

	m_vecBanItemList.resize( 0 );

	int		iTableIndex		= 1;		/// 테이블 인덱스 ( Lua는 테이블 첫번째 인덱스가 1 )
	wstring wstrValue		= L"";		/// 전달값

	while( luaManager.GetValue( iTableIndex, wstrValue ) == true )
	{
		m_vecBanItemList.push_back( wstrValue );				

		++iTableIndex;
	}
}
#endif //SERV_UPGRADE_TRADE_SYSTEM