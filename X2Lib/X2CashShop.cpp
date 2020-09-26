#include "StdAfx.h"
#include ".\x2cashshop.h"

#ifdef CLIENT_PURPLE_MODULE	// 임규수 일본 추가
#include "OnlyGlobal/JP/Auth/PurpleForClient.h"
#endif // CLIENT_PURPLE_MODULE

struct CashItemSorter : public std::binary_function< int, int, bool >
{
	bool operator()(const int& lhs, const int& rhs) const
	{
		int leftIndex = 0;
		int rightIndex = 0;
		for ( int i = 0; i < (int)g_pData->GetItemManager()->GetHotCashItemList().size(); i++ )
		{
			int itemID = g_pData->GetItemManager()->GetHotCashItemList()[i];
			if ( itemID == lhs )
			{
				leftIndex = i;
			}
			else if ( itemID == rhs )
			{
				rightIndex = i;
			}
#ifdef CASH_SHOP_HOT_ITEM_LIST_FIX
			else if ( leftIndex != 0 && rightIndex != 0 )
#else //CASH_SHOP_HOT_ITEM_LIST_FIX
			else if ( leftIndex != 0 && leftIndex != 0 )
#endif //CASH_SHOP_HOT_ITEM_LIST_FIX
			{
				break;
			}			
		}

#ifdef CASH_SHOP_HOT_ITEM_LIST_FIX
		if ( leftIndex < rightIndex )
#else //CASH_SHOP_HOT_ITEM_LIST_FIX
		if ( leftIndex > rightIndex )
#endif //CASH_SHOP_HOT_ITEM_LIST_FIX
			return true;
		else
			return false; 		
	}
};


CX2CashShop::CX2CashShop( CKTDXStage* pStage, const WCHAR* wszFileName )
: CX2ItemSlotManager( pStage, wszFileName )
//{{ kimhc // 은행에서 캐시샵의 설치탭으로 바로 가기 위해 추가
#ifdef	PRIVATE_BANK
, m_iMenuTypeCallingCashShop( 0 )
, m_bCheckDialogWarningPlatinumMembership( false )
#endif	PRIVATE_BANK
//}} kimhc // 은행에서 캐시샵의 설치탭으로 바로 가기 위해 추가
#ifdef SERV_EVENT_DAILY_GIFT_BOX
, m_pDlgDailyGiftBox( NULL )	// mauntain : 김태환 [2012.06.12] 매일매일 선물 상자 안내 Dialog
#endif SERV_EVENT_DAILY_GIFT_BOX
#ifdef TAKE_OFF_ALL_ITEM
, m_bShowInvenEquip ( true )
#endif // TAKE_OFF_ALL_ITEM
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
, m_uiSelectPreEquipSet ( 0 )
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
#ifdef PET_PREVIEW
,m_pNPCViewerUI ( NULL )
,m_pDLGPetPreview ( NULL )
,m_iPrePetItemID ( -1 )
#endif //PET_PREVIEW
#ifdef COUPON_SYSTEM
, m_pUICouponBox( NULL )
, m_ulTempSelectedProductNo( 0 )
#endif // COUPON_SYSTEM
#ifdef REFORM_SKILL_NOTE_UI
, m_iItemIDShowBuyUIAfterEnter(0)
#endif // REFORM_SKILL_NOTE_UI
{
	m_pUnitViewerUI		= NULL;
	CX2UnitViewerUI::SetUnitClicked( false );
	m_fMouseSensitivity = 0.01f;
	m_fRemainRotY		= 0.0f;
	m_bRotateReturn		= true;

	m_UnitClickPos		= D3DXVECTOR2(33,75);
	m_UnitClickSize		= D3DXVECTOR2(287,352);

#ifdef _SERVICE_
#ifdef _OPEN_TEST_
	CategoryDataParsing( L"CashShopCategory_Test.lua" );
#else
	CategoryDataParsing( L"CashShopCategory.lua" );
#endif
#else
	CategoryDataParsing( L"CashShopCategory_Test.lua" );
#endif

	m_bOpen				= false;
	m_CashShopState		= CX2CashShop::CSS_MAIN;
	m_CashShopCategory	= CX2CashShop::CSC_NONE;
	m_CashShopSubCategory = CX2CashShop::CSSC_ALL;


	m_pDLGBack			= NULL;
	m_pDLGFront			= NULL;

	m_pDLGMainPage		= NULL;
	m_pDLGSubPage		= NULL;
    //{{ seojt // 2009-1-8, 20:43
    ZeroMemory( m_pDLGSubPageMenu, sizeof(m_pDLGSubPageMenu) );
    //}} seojt // 2009-1-8, 20:43

	m_bAllCharItem		= false;

	m_pMainNewSM		= NULL;
	m_pMainHotSM		= NULL;
	m_pMainRecommendSM	= NULL;

	m_pSubSM			= NULL;
	m_pSubSpeicialSM	= NULL;

	m_pBuyItemPopUp			= NULL;
	m_pCashItemToInvenPopup = NULL;
	m_pPresentItemPopup		= NULL;


	m_iNowDepositPageIndex = 1;
	m_iReqDepositPageIndex = 1;

	m_bAutoOpen = false;

	m_pDLGCheckCashItemToInven			= NULL;
	m_pDLGOverlapCheckCashItemToInven	= NULL;

	m_ChoicedItemMenuItemID = 0;
	m_pDLGChoicedItemMenu = NULL;

	m_bSend_EGS_BUY_CASH_ITEM_REQ = false;

#ifdef CASH_ITEM_REFUND
	m_pDLGCheckCashItemRefund	= NULL;
	m_RefundProductNo			= 0;
	m_RefundOrderNo				= 0;
	m_RefundQuantity			= 0;
#endif CASH_ITEM_REFUND

#ifdef SERV_NOTIFY_BONUS_CASH
	m_bCheckCashEvent = false;
#endif //SERV_NOTIFY_BONUS_CASH

#ifdef SERV_EPAY_SYSTEM
	m_bRunEpay = false;	// 2011.12.13 lygan_조성욱 //epay가 실행중인지 확인하는 값
#endif //SERV_EPAY_SYSTEM

#ifdef SERV_PSHOP_AGENCY
	m_bUsePShopAgencyItem		= false;
#endif SERV_PSHOP_AGENCY

#ifdef SERV_EVENT_BINGO
	m_pBingo = NULL;
#endif //SERV_EVENT_BINGO

#ifdef CASH_INVEN_PICKUP_ALL
	m_bPickUpAll = false;
#endif CASH_INVEN_PICKUP_ALL

	InitUI( pStage ); 
	SetOpen( false );
}//CX2CashShop::CX2CashShop()

CX2CashShop::~CX2CashShop(void)
{
	SAFE_DELETE_KTDGOBJECT( m_pUnitViewerUI );
	SAFE_DELETE_DIALOG( m_pDLGBack );
	SAFE_DELETE_DIALOG( m_pDLGFront );
	SAFE_DELETE_DIALOG( m_pDLGMainPage );
	SAFE_DELETE_DIALOG( m_pDLGSubPage );

	SAFE_DELETE( m_pMainNewSM );
	SAFE_DELETE( m_pMainHotSM );
	SAFE_DELETE( m_pMainRecommendSM );

	SAFE_DELETE( m_pSubSM );
	SAFE_DELETE( m_pSubSpeicialSM );

	SAFE_DELETE( m_pBuyItemPopUp );
	SAFE_DELETE( m_pCashItemToInvenPopup );
	SAFE_DELETE( m_pPresentItemPopup );

#ifdef SERV_EVENT_BINGO
	SAFE_DELETE( m_pBingo );
#endif //SERV_EVENT_BINGO

	SAFE_DELETE_DIALOG( m_pDLGChoicedItemMenu);

	for ( int i = 0; i < (int)m_vecCashShopCateList.size(); i++ )
	{
		CX2CashShop::CashShopCategory* pCashShopCate = m_vecCashShopCateList[i];
		SAFE_DELETE( pCashShopCate );
	}
	m_vecCashShopCateList.clear();

	for ( int i = 0; i < CX2CashShop::CSC_END; i++)
	{
		CKTDGUIDialogType pDialog = m_pDLGSubPageMenu[i];
		SAFE_DELETE_DIALOG( pDialog );
		m_pDLGSubPageMenu[i] = NULL;
	}

//{{ mauntain : 김태환 [2012.06.12] 매일매일 선물 상자 안내 Dialog
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	SAFE_DELETE_DIALOG( m_pDlgDailyGiftBox );
	m_pDlgDailyGiftBox = NULL;
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

#ifdef SERV_EPAY_SYSTEM
	m_bRunEpay = false;
#endif //SERV_EPAY_SYSTEM

#ifdef FIX_MISSING_MAIL
	PushDelayedMails();
#endif FIX_MISSING_MAIL

#ifdef PET_PREVIEW
	SAFE_DELETE_KTDGOBJECT( m_pNPCViewerUI );
	SAFE_DELETE_DIALOG( m_pDLGPetPreview );
#endif //PET_PREVIEW
}

bool CX2CashShop::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef SERV_EVENT_BINGO
	if( NULL != m_pBingo && true == m_pBingo->MsgProc( hWnd, uMsg, wParam, lParam ) )
	{
		return true;
	}
#endif //SERV_EVENT_BINGO

	if ( GetCheckOperationCondition() == false )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{
			if ( m_pUnitViewerUI != NULL )
			{
				D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
				int degree = (int)D3DXToDegree( rot.y );
				m_fRemainRotY = D3DXToRadian( degree % 360 );
			}

			CX2UnitViewerUI::SetUnitClicked( false );

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

			if ( m_bOpen == true && IsInUnitClickReact( mousePos ) == true )
			{
				CX2UnitViewerUI::SetUnitClicked( true );
				return true;
			}
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

	case WM_RBUTTONUP:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			SetNowDragItemReturn();

			return MouseRButtonUp( mousePos );
		}
		break;
	}

	return false;
}

HRESULT CX2CashShop::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bOpen == false )
		return S_OK;

#ifdef SERV_NOTIFY_BONUS_CASH
	if( m_bCheckCashEvent == false )
	{
		CKTDGUIStatic* pStaticCash = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"buy_cash" );
		if(pStaticCash != NULL)
			pStaticCash->SetShowEnable( true, true );

		CKTDGUIStatic* pStaticEventCash = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"buy_cash_evnet" );
		if(pStaticEventCash != NULL)
			pStaticEventCash->SetShowEnable(false,false);
	}
	else
	{
		CKTDGUIStatic* pStaticCash = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"buy_cash" );
		if(pStaticCash != NULL)
			pStaticCash->SetShowEnable( false,false );

		CKTDGUIStatic* pStaticEventCash = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"buy_cash_evnet" );
		if(pStaticEventCash != NULL)
			pStaticEventCash->SetShowEnable(true,true);
	}
#endif //SERV_NOTIFY_BONUS_CASH

#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
	if( g_pMain != NULL )
		g_pMain->SetHSStatusTime( 0.f );
#endif


#ifdef SERV_EVENT_BINGO
	if( NULL != m_pBingo )
	{
		m_pBingo->OnFrameMove( fTime, fElapsedTime );
	}
#endif //SERV_EVENT_BINGO
	CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );

	UnitHandling( fTime, fElapsedTime );
	
	return S_OK;
}

bool CX2CashShop::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef SERV_EVENT_BINGO
	if ( NULL !=  m_pBingo && true == m_pBingo->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
		return true;
#endif //SERV_EVENT_BINGO

#ifdef CASH_ITEM_REFUND
	// oasis907 : 김상윤 [2010.11.23] // 청약 철회 모드시 다른 UICustomEvent 막기
	CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
	CX2Cursor*	pCursor = pState->GetCursor();
	if(pCursor->GetCurorState() == CX2Cursor::XCS_CASH_REFUND)
	{
		if( wParam == CSCUM_CASH_ITEM_REFUND)
		{
			pCursor->ChangeCursorState( CX2Cursor::XCS_NORMAL );
			return true;
		}
		else if( wParam == CSCUM_CASH_ITEM_REFUND_OK )
		{
			if ( m_pDLGCheckCashItemRefund != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCheckCashItemRefund, NULL, false );

			m_pDLGCheckCashItemRefund = NULL;

			Handler_EGS_CASH_ITEM_REFUND_REQ();
			return true;
		}
		return false;
	}
#endif CASH_ITEM_REFUND


	switch( wParam )
	{
#ifdef COUPON_SYSTEM
	case CX2StateMenu::SMUCM_COUPON_INPUT_BOX:
		{
			if ( NULL != m_pUICouponBox )
				m_pUICouponBox->SetOpen( true );
		} break;
#endif // COUPON_SYSTEM


	case CSCUM_CASH_CHARGE:
		{
			//{{ 2013.03.13 조효진	캐시충전 버튼을 누르면 웹페이지를 띄워줌
#ifdef CASH_CHARGE_FROM_WEB
			if( NULL != g_pData && NULL != g_pData->GetMyUser() &&
				true == g_pMain->GetBrowserWrapper()->IsClosed() )
			{
				g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE ); 

				//userID와 PurchaseToken 등을 붙일지 여기서 결정
				std::wstring wstrURL = GetWstrConnectChargePage();

#ifdef SERV_STEAM
				if( CX2Steam::IsSteamUser() == true )
				{
					CX2Steam::OpenSteamOverlayWebPage( wstrURL );
					//std::string strURL = KncUtil::toNarrowString(wstrURL);
					//SteamFriends()->ActivateGameOverlayToWebPage( strURL.c_str() );
				}
				else
				{
#ifdef CASH_CHARGE_IN_GAME_BROWSER
					if( false == g_pMain->GetBrowserWrapper()->IsClosed() )
					{
						g_pMain->GetBrowserWrapper()->CloseBrowser();
					}

					RECT rcWebPage;
					GetWindowRect( g_pKTDXApp->GetHWND(), &rcWebPage );

					POINT ptCenter;
					ptCenter.x = rcWebPage.left + ( rcWebPage.right - rcWebPage.left ) / 2;
					ptCenter.y = rcWebPage.top + ( rcWebPage.bottom - rcWebPage.top ) / 2;

					// 충전 페이지 크기 정해지면 그걸로 다시 잡자
					rcWebPage.left = ptCenter.x - 500;
					rcWebPage.right = ptCenter.x + 500;
					rcWebPage.top = ptCenter.y - 338;
					rcWebPage.bottom = ptCenter.y + 338;

					std::string strURL;
					ConvertWCHARToChar( strURL, wstrURL );
					g_pMain->GetBrowserWrapper()->Create( rcWebPage, g_pKTDXApp->GetHWND() );
					g_pMain->GetBrowserWrapper()->Navigate( strURL.c_str(), NULL );
#else //CASH_CHARGE_IN_GAME_BROWSER
					ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
#endif //CASH_CHARGE_IN_GAME_BROWSER
				}
#else //SERV_STEAM

				// iexplore로 충전 창 열기
#ifdef USE_DEFAULT_BROWSER
				ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
#else USE_DEFAULT_BROWSER
				ShellExecuteW(NULL, _T("open"), _T("iexplore.exe"), wstrURL.c_str(), NULL, SW_SHOW); 
#endif USE_DEFAULT_BROWSER
#endif //SERV_STEAM
				//{{ 2010.12.03 조효진	충전 누를시 케시샵에서 팅기게 하는 코드. 
				CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
				if( NULL != pStateMenu )
				{					
					pStateMenu->ToggleCashShop();
				}
				//}}
			}
#else // CASH_CHARGE_FROM_WEB


// #ifdef SERV_TOONILAND_CHANNELING	// SERV_JAPAN_CHANNELING 때문에 문제 여지 있어서 주석 처리함
// 			if( NULL != g_pData && NULL != g_pData->GetMyUser() )
// 			{
// 				if( g_pData->GetMyUser()->GetUserData().m_uChannelCode == 3 )
// 				{
// 					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_13713 ), g_pMain->GetNowState() );
// 					return true;
// 				}
// 			}
// #end

			if( true == g_pMain->GetBrowserWrapper()->IsClosed() )
			{

#ifdef CJ_ID_WEB_BILLING
				InitCJBillingWebBrowser();
				return true;
#endif //CJ_ID_WEB_BILLING

				// 2011.02.23 lygan_조성욱 // 중국 전용 캐쉬 전환 UI용
#ifdef CHINA_CASH_CHANGE // 2011.02.19 lygan_조성욱 // 중국 전용 캐쉬 전환 UI용
				g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE ); 

				m_bLowCashCheck = false;
				m_bCashlistCheck = false;
				m_uiSubcash = 0;

				m_pDLGCashchange = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_Cash_Change_CN.lua");
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg(m_pDLGCashchange);


				CKTDGUIStatic* pStatic_Elsowrd_Tap = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Elsword_Tap" );
				pStatic_Elsowrd_Tap->SetShowEnable(true,true);

				CKTDGUIStatic* pStatic_Giant_Tap = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Giant_Tap" );
				pStatic_Giant_Tap->SetShowEnable(false,true);

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH_TEST_MODE
				if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
				{
					CKTDGUIRadioButton* pDirect_Charge_TapButton = (CKTDGUIRadioButton*) m_pDLGCashchange->GetControl( L"CashChange_Direct_Charge_tap" );
					pDirect_Charge_TapButton->SetShowEnable(false,false);
				}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH_TEST_MODE
				CKTDGUIStatic* pStatic_Direct_Charge_Tap = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Direct_Charge_Tap" );
				pStatic_Direct_Charge_Tap->SetShowEnable(false,true);

				CKTDGUIButton* pDirect_Charge_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Direct_Charge_ok" );
				pDirect_Charge_OK->SetShowEnable(false, false);
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

				CKTDGUIButton* pCash_Giant_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_ok" );
				pCash_Giant_OK->SetShowEnable(false, false);

				CKTDGUIButton* pCash_Giant_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_cancel" );
				pCash_Giant_CANCEL->SetShowEnable(false, false);

				CKTDGUIStatic* pStaticChangeBingon = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"Exchange_cash_Bingon" );
				pStaticChangeBingon->SetShowEnable(false,false);

				CKTDGUIStatic* pStaticChangeFont_Giantcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GIANTPOINT" );
				pStaticChangeFont_Giantcash->SetShowEnable(true,true);

				CKTDGUIStatic* pStaticChangeFont_Goldcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GOLDCOIN" );
				pStaticChangeFont_Goldcash->SetShowEnable(true,true);

				CKTDGUIStatic* pStaticChangeFONT_AFTER_GIANT = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_GIANT" );
				pStaticChangeFONT_AFTER_GIANT->SetShowEnable(false,false);

				CKTDGUIStatic* pStaticChangeFONT_AFTER_ELS_CASH = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_ELS_CASH" );
				pStaticChangeFONT_AFTER_ELS_CASH->SetShowEnable(false,false);
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
				CKTDGUIStatic* pStaticEventCash = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"buy_cash_evnet" );
				pStaticEventCash->SetShowEnable(true,true);

				CKTDGUIStatic* pStaticCash = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"buy_cash" );
				pStaticCash->SetShowEnable(false,false);
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

				CKTDGUIStatic* pStaticElsowrdCash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_pStaticElswordcash_num" );
				if ( pStaticElsowrdCash != NULL )
				{
					pStaticElsowrdCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalMainCash() ).c_str() );
				}

				CKTDGUIStatic* pStaticGiantCash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_pStaticGiantcash_num" );
				if ( pStaticGiantCash != NULL )
				{
					pStaticGiantCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_CASH ) ).c_str() );
				}

				CKTDGUIStatic* pStaticChangeCash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_pStaticChangecash_num" );


				CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGCashchange->GetControl( L"CashChangeList" );

				pStaticChangeCash->SetString( 0, GET_REPLACED_STRING( ( STR_ID_11307, "i", 0 ) ) );

				pComboBox->RemoveAllItems();

				pComboBox->AddItem(L"0点",NULL,false);
				pComboBox->AddItem(L"100点",NULL,false);
				pComboBox->AddItem(L"200点",NULL,false);
				pComboBox->AddItem(L"500点",NULL,false);
				pComboBox->AddItem(L"1000点",NULL,false);
				pComboBox->AddItem(L"2000点",NULL,false);
				pComboBox->AddItem(L"5000点",NULL,false);
				pComboBox->AddItem(L"10000点",NULL,false);
#else //CHINA_CASH_CHANGE
				g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE ); 

				RECT	rcWebPage;
				string	strURL;

				//{{ kimhc // 2011-08-10 // 채널링 SOAP
#ifdef	SERV_CHANNELING_SOAP
				SetOpenNexonCashChargePage( strURL, rcWebPage );
#else	SERV_CHANNELING_SOAP
				switch ( g_pData->GetMyUser()->GetUserData().m_uChannelCode )
				{
				default:

				case KNexonAccountInfo::CE_NEXON_ACCOUNT:	// 넥슨 접속자
					SetOpenNexonCashChargePage( strURL, rcWebPage );
					break;

				case KNexonAccountInfo::CE_TOONILAND_ACCOUNT:	// 투니랜드 접속자
					//ShellExecute(NULL ,NULL, L"https://member.tooniland.com/membersMember/checkIdPopup2.tl",NULL,NULL,SW_SHOWNA); 
					//return true;
					SetOpenTooniCashChargePage( strURL, rcWebPage );
					break;

#ifdef SERV_NAVER_CHANNELING
				case KNexonAccountInfo::CE_NAVER_ACCOUNT:		// 네이버 접속자
					SetOpenNaverCashChargePage( strURL, rcWebPage );
					break;
#endif // SERV_NAVER_CHANNELING

				}
#endif	SERV_CHANNELING_SOAP
				//}} kimhc // 2011-08-10 // 채널링 SOAP

				D3DXVECTOR2 vResolution = g_pMain->GetGameOption().GetOptionList().m_vResolution;

				if( rcWebPage.right > static_cast<long>( vResolution.x ) )
					rcWebPage.right = static_cast<long>( vResolution.x );

				if( rcWebPage.bottom > static_cast<long>( vResolution.y ) )
					rcWebPage.bottom = static_cast<long>( vResolution.y );

				rcWebPage.left = static_cast<long>( ( vResolution.x - rcWebPage.right ) * 0.5f );
				rcWebPage.top = static_cast<long>( ( vResolution.y - rcWebPage.bottom ) * 0.5f );

				if( rcWebPage.left < 0 )
					rcWebPage.left = 0;

				if( rcWebPage.top < 0 )
					rcWebPage.top = 0;

				rcWebPage.right		+= rcWebPage.left;
				rcWebPage.bottom	+= rcWebPage.top;
				g_pMain->GetBrowserWrapper()->Create( rcWebPage, g_pKTDXApp->GetHWND() );
				g_pMain->GetBrowserWrapper()->Navigate( strURL.c_str(), NULL ); 

				//{{ kimhc // 2009-10-14 // 자동결제 웹 관련 코드
#ifdef	AUTO_PAYMENT
				if ( g_pInstanceData != NULL )
					g_pInstanceData->SetWebType( CSWT_CASH_CHARGE );
#endif	AUTO_PAYMENT
				//}} kimhc // 2009-10-14 // 자동결제 웹 관련 코드

#endif //CHINA_CASH_CHANGE
			}

#endif // CASH_CHARGE_FROM_WEB
			//}}

			return true;
		}
		break;
//////////////////////////////////////////////////////////////////////////
#ifdef CHINA_CASH_CHANGE // 2011.02.23 lygan_조성욱 // 중국 전용 캐쉬 전환 UI용		
		case CSCUM_CASH_CHARGE_TAP_EXIT_CN:
			{

#ifdef SERV_EPAY_SYSTEM
				if(m_EpayTimer.elapsed() > 3)
				{
					if(m_bRunEpay == true)
					{

						CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
						pNowState->Handler_EGS_CHECK_BALANCE_REQ();
						m_bRunEpay = false;
					}
					m_EpayTimer.restart();
#endif //SERV_EPAY_SYSTEM

					m_pDLGCashchange->SetShowEnable(false,false);
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
#else // SERV_DIRECT_CHARGE_ELSWORD_CASH
					CKTDGUIStatic* pStatic_Graphic = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Elsword_Tap" );
					pStatic_Graphic->SetShowEnable(false,false);

					CKTDGUIComboBox* pCombo_cash= (CKTDGUIComboBox*) m_pDLGCashchange->GetControl( L"CashChangeList" );
					pCombo_cash->SetShowEnable(false, false);

					CKTDGUIButton* pCash_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_ok" );
					pCash_OK->SetShowEnable(false, false);

					CKTDGUIButton* pCash_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_cancel" );
					pCash_CANCEL->SetShowEnable(false, false);
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
#ifdef SERV_EPAY_SYSTEM
				}
#endif //SERV_EPAY_SYSTEM
				return true;

				
			}
			break;
		case CSCUM_CASH_CHARGE_TAP_ELSWORD_CN:
			{
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
				SetShowChinaCash_Tap_Elsword(true);
				SetShowChinaCash_Tap_Giant(false);
				SetShowChinaCash_Tap_DirectCharge(false);
#else // SERV_DIRECT_CHARGE_ELSWORD_CASH
				ChinaCash_Tap_Change(true);
				CKTDGUIComboBox* pCombo_cash= (CKTDGUIComboBox*) m_pDLGCashchange->GetControl( L"CashChangeList" );
				
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
			}
			break;
		case CSCUM_CASH_CHARGE_TAP_GIANT_CN:
			{
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
				SetShowChinaCash_Tap_Elsword(false);
				SetShowChinaCash_Tap_Giant(true);
				SetShowChinaCash_Tap_DirectCharge(false);
#else // SERV_DIRECT_CHARGE_ELSWORD_CASH
				ChinaCash_Tap_Change(false);
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

			}
			break;
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
		case CSCUM_CASH_CHARGE_TAP_DIRECT_CHARGE_CN:
			{
				SetShowChinaCash_Tap_Elsword(false);
				SetShowChinaCash_Tap_Giant(false);
				SetShowChinaCash_Tap_DirectCharge(true);
			}
			break;
		case CSCUM_CASH_DIRECT_CHARGE_CN:
			{
				if(m_EpayTimer.elapsed() > 3)
				{
					if(m_bRunEpay == true )
					{
						CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
						pNowState->Handler_EGS_CHECK_BALANCE_REQ();
						m_bRunEpay = false;
					}
					m_EpayTimer.restart();
				}
			}
			break;
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
		case CSCUM_CASH_GIANT_TAP_EXIT_CN:
			{
#ifdef SERV_EPAY_SYSTEM
				if(m_EpayTimer.elapsed() > 3)
				{
#endif //SERV_EPAY_SYSTEM
					m_pDLGCashchange->SetShowEnable(false,false);

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
#else // SERV_DIRECT_CHARGE_ELSWORD_CASH
					CKTDGUIButton* pCash_Giant_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_ok" );
					pCash_Giant_OK->SetShowEnable(false, false);

					CKTDGUIButton* pCash_Giant_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_cancel" );
					pCash_Giant_CANCEL->SetShowEnable(false, false);
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

#ifdef SERV_EPAY_SYSTEM
					if(m_bRunEpay == true )
					{
						CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
						pNowState->Handler_EGS_CHECK_BALANCE_REQ();
						m_bRunEpay = false;
					}
					m_EpayTimer.restart();
#endif //SERV_EPAY_SYSTEM

					return true;
#ifdef SERV_EPAY_SYSTEM
				}
#endif //SERV_EPAY_SYSTEM

			}
			break;

		case  CSCUM_CASH_GIANT_FILLING_CN:
			{

				//ShellExecute(NULL, L"open", L"http://pay.ztgame.com/qfillfrombank.php" /*GET_STRING( STR_ID_14497 )*/, L"", L"", SW_SHOW ); 

				
#ifdef SERV_EPAY_SYSTEM
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
				ExecuteEpaySystem();
#else // SERV_DIRECT_CHARGE_ELSWORD_CASH
				/*if(g_pData->GetMyUser().GetAuthLevel() >= CX2User::XUAL_OPERATOR)
				{*/

					STARTUPINFO stif;
					PROCESS_INFORMATION pi;
					memset(&stif,0,sizeof(STARTUPINFO));
					stif.cb=sizeof(STARTUPINFO);
					stif.dwFlags= STARTF_USESHOWWINDOW;
					stif.wShowWindow=SW_SHOW;



					//std::string command = localpath + patchername + " " + commandline;
					WCHAR	wszFullFilePath[MAX_PATH] = L"";
					WCHAR	wszFilePath[MAX_PATH] = L"";
					WCHAR	wszDriveName[10] = L"";
					WCHAR	wszMsg[MAX_PATH] = L"";
					WCHAR	wszExeFilePath[MAX_PATH] = L"";
					WCHAR	wszCommandLine[MAX_PATH] = L"";

					WCHAR	wszCommandLineZone[MAX_PATH] = L"";
					int tempCommand = 0;
					//tempCommand =  (int)(g_pInstanceData->GetGiantZone() + g_pInstanceData->GetGiantGame());
					tempCommand =  (int)(g_pInstanceData->GetGiantGame() * 65536 +  g_pInstanceData->GetGiantZone());
					int tempUid = 0;
					tempUid = (int)(g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
				
					/*boost::str( boost::wformat( L"%d" ) % GetUID() );*/

					GetModuleFileNameW( NULL, wszFullFilePath, MAX_PATH );
					_wsplitpath(wszFullFilePath, wszDriveName, wszFilePath, NULL, NULL);
					// Data 폴더 패스 
					std::wstring wstrDataFolderFullPath(wszFullFilePath);

					wcscpy( wszFullFilePath, L"" );

					wcscat( wszFullFilePath, wszDriveName );
					wcscat( wszFullFilePath, wszFilePath );
					wcscat( wszFullFilePath, L"\\epay\\" );

					wcscat( wszExeFilePath, wszFullFilePath );
					wcscat( wszExeFilePath, L"\\epay.exe" );

					wcscat(wszCommandLine, L" -id ");
					wcscat(wszCommandLine, g_pInstanceData->GetUserID().c_str());
					wcscat(wszCommandLine, L" -zone ");
					//wcscat(wszCommandLine, L" ");
					_itow(tempCommand,wszCommandLineZone, 10 );
					wcscat(wszCommandLine, wszCommandLineZone);
					wcscat(wszCommandLine, L" -cname ");
					//wcscat(wszCommandLine, L" ");
					wcscat(wszCommandLine, g_pData->GetMyUser()->GetSelectUnit()->GetNickName());
					wcscat(wszCommandLine, L" -cid ");
					//wcscat(wszCommandLine, L" ");
					_itow(tempUid,wszCommandLineZone, 10 );
					wcscat(wszCommandLine, wszCommandLineZone);


					//wszCommandLine = boost::str(boost::wformat(L"-id %s, -zone %d, -cname %s, - cid %d") %L"a" %1 %L"f" %2 ).c_str();
					if(false == CreateProcessW(wszExeFilePath,wszCommandLine,NULL,NULL,0,0,NULL,wszFullFilePath,&stif,&pi))
					{
						ShellExecute(NULL, L"open", GET_STRING( STR_ID_14497 ), L"", L"", SW_SHOW ); 
						m_bRunEpay = true; //2011.12.21 epay 홈페이지가 떠도 충전 닫기 버튼을 누르면 갱신되게 수정
					}
					else
					{
						m_bRunEpay = true;
					}
					

				/*}
				else
				{
					ShellExecute(NULL, L"open", GET_STRING( STR_ID_14497 ), L"", L"", SW_SHOW ); 
				}*/
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
#else //SERV_EPAY_SYSTEM

		ShellExecute(NULL, L"open", GET_STRING( STR_ID_14497 ), L"", L"", SW_SHOW ); 

#endif //SERV_EPAY_SYSTEM


			}
			break;

		case CSCUM_CASH_ELSWORD_CHANGE_CN:
			{
				CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)lParam;

				if ( pComboBox != NULL )
				{
					switch(pComboBox->GetSelectedItemIndex())
					{
					case 0 :			
						ExchangeChinaCash(0);
						break;
					case 1 :
						ExchangeChinaCash(10);
						break;
					case 2 :
						ExchangeChinaCash(20);
						break;
					case 3 :
						ExchangeChinaCash(50);
						break;
					case 4 :
						ExchangeChinaCash(100);
						break;
					case 5 :
						ExchangeChinaCash(200);
						break;
					case 6 :
						ExchangeChinaCash(500);
						break;
					case 7 :
						ExchangeChinaCash(1000);
						break;
					/*case 0 :			
						ExchangeChinaCash(0);
						break;
					case 1 :
						ExchangeChinaCash(100);
						break;
					case 2 :
						ExchangeChinaCash(200);
						break;
					case 3 :
						ExchangeChinaCash(500);
						break;
					case 4 :
						ExchangeChinaCash(1000);
						break;*/
					
					}

				}
			}
			break;

		case CSCUM_CASH_ELSWORD_OK_CN:
			{
				//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 300, 300 ), GET_STRING( STR_ID_2558 ), (CKTDXStage*) m_pNowState );
				if(m_uiSubcash > 0)
				{
					Handler_EGS_EXCHANGE_REQ();
				}

			}
			break;

#endif //CHINA_CASH_CHANGE // 2011.02.23 lygan_조성욱 // 중국 전용 캐쉬 전환 UI용		
//////////////////////////////////////////////////////////////////////////
	case CSCUM_BUY:
		{
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
			g_pInstanceData->SetChoicedItem(false);
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
			// Buy 버튼 누르면 캐시 체크 한번 더 한다. 
			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			if ( NULL != pNowState )
				pNowState->Handler_EGS_CHECK_BALANCE_REQ();

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES

#if defined(CLIENT_COUNTRY_CN) || defined(CLIENT_COUNTRY_PH)
			m_pBuyItemPopUp->SetUseCashType((int)KGlobalCashInfo::GCT_KOG_ELSWORD_CASH);
#elif defined(CLIENT_COUNTRY_TWHK)
			m_pBuyItemPopUp->SetUseCashType((int)KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH);
#else // CLIENT_COUNTRY_TWHK
			m_pBuyItemPopUp->SetUseCashType((int)KGlobalCashInfo::GCT_PUBLISHER_CASH);
#endif // CLIENT_COUNTRY_TWHK
							
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

			//{{ kimhc // 2009-10-06 // 자동결제 탭에서 구매버튼 클릭시 다른 처리
#ifdef	AUTO_PAYMENT
			switch ( m_CashShopCategory )
			{
			case CSC_AUTO_PAYMENT:	// CSC_AUTO_PAYMENT 으로 바꿔야 함
				{
					CKTDGUIControl*	pControl	= reinterpret_cast< CKTDGUIControl* >( lParam );
					
					ProcessAutoPayment( GetItemID( pControl->GetDialog() ) );
				}
				break;

			default:
				{
					CKTDGUIControl* pControl = reinterpret_cast< CKTDGUIControl* >( lParam );
					if ( pControl != NULL )
					{
						int itemID = GetItemID( pControl->GetDialog() );
#ifdef ADDED_RELATIONSHIP_SYSTEM
						// 해당 커플/결혼 아이템을 구입 할 수 없는 상황이라면
						if( true == CheckBuyPossibleRelationItem( itemID ) )
#endif // ADDED_RELATIONSHIP_SYSTEM
						{
							vector< int > vecItemID;
							if ( itemID != 0 )
								vecItemID.push_back( itemID );
							m_pBuyItemPopUp->Reset( vecItemID );
							m_pBuyItemPopUp->SetOpen( true );
							m_pBuyItemPopUp->SetChoicedItemBuy( false );
	#ifdef CASH_ITEM_REFUND
							SetStringBuyItemRefund();
	#endif CASH_ITEM_REFUND
							ShowPopUpGuideByItemID( itemID );
						}
					}
				}
				break;
			}
#else	AUTO_PAYMENT
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				int itemID = GetItemID( pControl->GetDialog() );

#ifdef ADDED_RELATIONSHIP_SYSTEM
				// 해당 커플/결혼 아이템을 구입 할 수 없는 상황이라면
				if( true == CheckBuyPossibleRelationItem( itemID ) )
				{
					vector< int > vecItemID;
					if ( itemID != 0 )
						vecItemID.push_back( itemID );
					m_pBuyItemPopUp->Reset( vecItemID );
					m_pBuyItemPopUp->SetOpen( true );
					m_pBuyItemPopUp->SetChoicedItemBuy( false );

#ifdef CASH_ITEM_REFUND
					SetStringBuyItemRefund();
#endif CASH_ITEM_REFUND

					if( GetClassChangeItemLevel( itemID ) > 0 ) // 전직 캐시 아이템이면
					{
						std::wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_4518, "L", GetClassNameByClassChangeItem( itemID ) ) );
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), g_pMain->GetNowState() );
					}
					else if( true == g_pData->GetSkillTree()->IsUnsealSkillItemID( itemID ) )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4519 ), g_pMain->GetNowState() );
					}

				}

#else //ADDED_RELATIONSHIP_SYSTEM

				vector< int > vecItemID;
				if ( itemID != 0 )
					vecItemID.push_back( itemID );
				m_pBuyItemPopUp->Reset( vecItemID );
				m_pBuyItemPopUp->SetOpen( true );
				m_pBuyItemPopUp->SetChoicedItemBuy( false );

#ifdef CASH_ITEM_REFUND
				SetStringBuyItemRefund();
#endif CASH_ITEM_REFUND

				if( GetClassChangeItemLevel( itemID ) > 0 ) // 전직 캐시 아이템이면
				{
					std::wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_4518, "L", GetClassNameByClassChangeItem( itemID ) ) );
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), g_pMain->GetNowState() );
				}
				else if( true == g_pData->GetSkillTree()->IsUnsealSkillItemID( itemID ) )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4519 ), g_pMain->GetNowState() );
				}
#endif // ADDED_RELATIONSHIP_SYSTEM
			}

#endif	AUTO_PAYMENT
			//{{ kimhc // 2009-10-06 // 자동결제 탭일 경우에 다른 처리

#ifdef FIXED_RELATIONSHIP_SYSTEM
			return true;
#endif // FIXED_RELATIONSHIP_SYSTEM
		}
		break;

	case CSCUM_PRESENT:
		{
#ifdef CLIENT_COUNTRY_PH
			if (g_pData != NULL && g_pData->GetSelectUnitLevel() < 15)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_14852 ), g_pMain->GetNowState() );
				return true;
			}
#endif //CLIENT_COUNTRY_PH

			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				int itemID = GetItemID( pControl->GetDialog() );


#ifdef IMPOSSIBLE_PRESENT_CASH_ITEM
				if( NULL != g_pData && NULL != g_pData->GetItemManager() &&
					true == g_pData->GetItemManager()->IsImpossiblePresentItem(itemID) )
#else
				if ( IsPossiblePresent( itemID ) == false )
#endif //IMPOSSIBLE_PRESENT_CASH_ITEM
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_6 ), g_pMain->GetNowState() );
					return true;
				}

				if ( m_pPresentItemPopup != NULL )
				{
					m_pPresentItemPopup->SetOpen( true, itemID );
					m_pPresentItemPopup->SetChoicedItem( false );
				}
			}
		}
		break;

	case CSCUM_STORE:
		{



			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				int itemID = GetItemID( pControl->GetDialog() );
				if ( itemID != 0 )
				{
					AddChoicedItem( itemID );
				}
			}
			return true;

		}
		break;

	case CSCUM_MAIN_NEW_PREV_PAGE:
		{
			if ( m_pMainNewSM->PrevPage() == true )
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_MAIN_NEW, m_pMainNewSM->GetNowShowItemIDList() );
				ResetMainPageNumUI();
			}
		}
		break;

	case CSCUM_MAIN_NEW_NEXT_PAGE:
		{
			if ( m_pMainNewSM->NextPage() == true )
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_MAIN_NEW, m_pMainNewSM->GetNowShowItemIDList() );
				ResetMainPageNumUI();
			}
		}
		break;

	case CSCUM_MAIN_HOT_PREV_PAGE:
		{
			if ( m_pMainHotSM->PrevPage() == true )
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_MAIN_HOT, m_pMainHotSM->GetNowShowItemIDList() );
				ResetMainPageNumUI();
			}
		}
		break;

	case CSCUM_MAIN_HOT_NEXT_PAGE:
		{
			if ( m_pMainHotSM->NextPage() == true )
			{

				ResetItemSlot( CX2Slot::ST_CASH_SHOP_MAIN_HOT, m_pMainHotSM->GetNowShowItemIDList() );
				ResetMainPageNumUI();
			}
		}
		break;

	case CSCUM_MAIN_RECOMMEND_PREV_PAGE:
		{
			if ( m_pMainRecommendSM->PrevPage() == true )
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND, m_pMainRecommendSM->GetNowShowItemIDList() );
				ResetMainPageNumUI();
			}
		}
		break;

	case CSCUM_MAIN_RECOMMEND_NEXT_PAGE:
		{
			if ( m_pMainRecommendSM->NextPage() == true )
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND, m_pMainRecommendSM->GetNowShowItemIDList() );
				ResetMainPageNumUI();
			}
		}
		break;

	case CSCUM_BUY_POP_UP_PREV_PAGE:
		{
			m_pBuyItemPopUp->PrevPage();
		}
		break;

	case CSCUM_BUY_POP_UP_NEXT_PAGE:
		{
			m_pBuyItemPopUp->NextPage();
		}
		break;
#ifdef COUPON_SYSTEM
	case CSCUM_BUY_POP_UP_TO_FIRST_PAGE:			// 구매하기 UI, 상품 페이지 첫 페이지로 변경
		{
			m_pBuyItemPopUp->FirstPage();
		} break;
	case CSCUM_BUY_POP_UP_TO_LAST_PAGE:			// 구매하기 UI, 상품 페이지 첫 페이지로 변경
		{
			m_pBuyItemPopUp->LastPage();
		} break;
#endif // COUPON_SYSTEM

	case CSCUM_BUY_POP_UP_ALL_CHECK_CHANGED:
		{
			//m_pBuyItemPopUp->
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)lParam;
			if ( pCheckBox != NULL )
				m_pBuyItemPopUp->SetCheckAllItem( pCheckBox->GetChecked() );

			return true;
		}
		break;

	case CSCUM_BUY_POP_UP_EACH_CHECK_CHANGED:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)lParam;
			if ( pCheckBox != NULL )
				m_pBuyItemPopUp->ChangeCheckItem( pCheckBox->GetDialog(), pCheckBox->GetChecked() );

			return true;
		}
		break;

	case CSCUM_BUY_POP_UP_PRICE_CHANGED:
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)lParam;
			if ( pComboBox != NULL )
			{
#ifdef COUPON_SYSTEM // 기간(수량) 변경 시 사용 가능 한 할인 쿠폰 목록 요청
				unsigned long ulProductNo = 
					m_pBuyItemPopUp->ChangeItemPeriod( pComboBox->GetDialog(), pComboBox->GetSelectedItemIndex() );		

				Handler_EGS_DISCOUNT_COUPON_LIST_INQUIRY_REQ( ulProductNo ) ;
#else
				m_pBuyItemPopUp->ChangeItemPeriod( pComboBox->GetDialog(), pComboBox->GetSelectedItemIndex() );		
#endif // COUPON_SYSTEM
			}
			
			return true;
		}
		break;

	case CSCUM_BUY_POP_UP_OK:
		{
			ProcessBuyPopUpOk();
			return true;
		}
		break;

	case CSCUM_BUY_POP_UP_CANCEL:
		{
			if( NULL != m_pBuyItemPopUp )
			{
				m_pBuyItemPopUp->SetOpen( false );
#ifdef COUPON_SYSTEM
				m_pBuyItemPopUp->InitCouponList();
#endif // COUPON_SYSTEM
			}

			return true;
		}
		break;

	case CSCUM_CASH_ITEM_DEPOSIT_PREV_PAGE:
		{
			if ( m_iNowDepositPageIndex > 1 )
			{
#ifdef SERV_GLOBAL_BILLING
				return Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( m_iNowDepositPageIndex - 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#else // SERV_GLOBAL_BILLING
				return Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( m_iNowDepositPageIndex - 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#endif // SERV_GLOBAL_BILLING
			}
		}
		break;

	case CSCUM_CASH_ITEM_DEPOSIT_NEXT_PAGE:
		{
#ifdef SERV_GLOBAL_BILLING
			return Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( m_iNowDepositPageIndex + 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#else // SERV_GLOBAL_BILLING
			return Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( m_iNowDepositPageIndex + 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#endif // SERV_GLOBAL_BILLING
		}
		break;

	case CSCUM_CATEGORY_CHANGE:
		{
			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;
			ChangeCategory( pRadioButton );
			return true;
		}
		break;

	case CSCUM_SUB_CATEGORY_CHANGE:
		{
			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;
			ChangeSubCategory( pRadioButton );
			return true;
		}
		break;

	case CSCUM_SUB_PREV_PAGE:
		{
			//{{ kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
#ifdef	AUTO_PAYMENT
			bool bAutoPayment = false;

			if ( m_CashShopCategory == CSC_AUTO_PAYMENT )
				bAutoPayment = true;

			if ( m_pSubSM->PrevPage( bAutoPayment ) == true )
#else	AUTO_PAYMENT
			if ( m_pSubSM->PrevPage() == true )
#endif	AUTO_PAYMENT
			//}} kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_SUB, m_pSubSM->GetNowShowItemIDList() );
				ResetSubPageNumUI();
			}
		}
		break;

	case CSCUM_SUB_NEXT_PAGE:
		{
			//{{ kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
		#ifdef	AUTO_PAYMENT
			bool bAutoPayment = false;
			
			if ( m_CashShopCategory == CSC_AUTO_PAYMENT )
				bAutoPayment = true;

			if ( m_pSubSM->NextPage( bAutoPayment ) == true )
		#else	AUTO_PAYMENT
			if ( m_pSubSM->NextPage() == true )
		#endif	AUTO_PAYMENT
			//}} kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_SUB, m_pSubSM->GetNowShowItemIDList() );
				ResetSubPageNumUI();
			}
		}
		break;

	case CSCUM_SUB_SEE_ALL_ITEM_OPTION_CHANGE:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)lParam;
			if ( pCheckBox != NULL )
			{
				m_bAllCharItem = pCheckBox->GetChecked();
				vector< int > vecItemID;

				if ( m_CashShopSubCategory == CX2CashShop::CSSC_ALL )
				{
					vecItemID = GetItemByCategory( m_CashShopCategory );
				}
				else
				{
					vecItemID = GetItemByCategory( m_CashShopCategory, m_CashShopSubCategory );
				}

				//{{ kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
#ifdef	AUTO_PAYMENT
				bool bAutoPayment = false;

				if ( m_CashShopCategory == CSC_AUTO_PAYMENT )
					bAutoPayment = true;

				m_pSubSM->Reset( vecItemID, bAutoPayment );
#else	AUTO_PAYMENT
				m_pSubSM->Reset( vecItemID );
#endif	AUTO_PAYMENT
#ifdef SERV_USE_ENABLE_GIFT
				m_pSubSM->SetShowPresentButton();
#endif SERV_USE_ENABLE_GIFT
#ifdef SERV_WISH_LIST_NO_ITEM
				m_pSubSM->SetShowCartButton();
#endif SERV_WISH_LIST_NO_ITEM
				//}} kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
				ResetItemSlot(  CX2Slot::ST_CASH_SHOP_SUB,			m_pSubSM->GetNowShowItemIDList() );

				m_pSubSpeicialSM->Reset( GetHotItemList( vecItemID, CASH_SHOP_SUB_SPECIAL_SLOT_NUM ) );
				ResetItemSlot(  CX2Slot::ST_CASH_SHOP_SUB_SPECIAL,			m_pSubSpeicialSM->GetNowShowItemIDList() );

				ResetSubPageNumUI();
			}
		}
		break;

	case CSCUM_TAKE_OFF_ALL_ITEM:
		{
			if ( m_pUnitViewerUI !=	NULL )
				m_pUnitViewerUI->UpdateEqip( true );
			
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
			ResizePreEquipItemID();
#else
			m_vecPreEquipItemID.resize(0);
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST

			ResetNowEquipUI();
			SetEnableBuyEquippingItem( true );
#ifdef PET_PREVIEW
			ResetPetViewer();
#endif //PET_PREVIEW
		}
		break;

	case CSCUM_ITEM_TO_INVEN_POP_UP_PREV_PAGE:
		{
			m_pCashItemToInvenPopup->PrevPage();
		}
		break;

	case CSCUM_ITEM_TO_INVEN_POP_UP_NEXT_PAGE:
		{
			m_pCashItemToInvenPopup->NextPage();
		}
		break;

	case CSCUM_ITEM_TO_INVEN_POP_UP_OPTION_CHANGED:
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)lParam;
			m_pCashItemToInvenPopup->ChangeItemOption( pComboBox->GetDialog(), pComboBox->GetSelectedItemIndex() );
		}
		break;

	case CSCUM_ITEM_TO_INVEN_POP_UP_OK:
		{
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING
			vector< KBillOrderInfo > vecKBillOrderInfo;
			vecKBillOrderInfo = m_pCashItemToInvenPopup->GetOrderInfoList();
			if ( vecKBillOrderInfo.empty() == false )
			{
				KBillOrderInfo& kKBillOrderInfo = vecKBillOrderInfo[0];
				map< int, int > mapSelectedOptionList;

#ifdef SERV_CASH_ITEM_SOCKET_OPTION
				map<int, int> mapOptionGroupIdList;		/// <SocketId, GroupdID>					
				if ( m_pCashItemToInvenPopup->GetSelectedOptionListAndGroupID( mapSelectedOptionList, mapOptionGroupIdList ) == true )
#else //SERV_CASH_ITEM_SOCKET_OPTION
				if ( m_pCashItemToInvenPopup->GetSelectedOptionList( mapSelectedOptionList ) == true )
#endif //SERV_CASH_ITEM_SOCKET_OPTION
				{
					m_pCashItemToInvenPopup->SetOpen( false );
					// 패키지는 분해되므로 이 경우에 대한 처리는 하지 않는다
#ifdef SERV_GLOBAL_CASH_PACKAGE
					if ( kKBillOrderInfo.m_byteProductKind == 1 )
					{
						vector< KBillPackageInfo > vecKBillPackageInfo;
						m_pCashItemToInvenPopup->GetCheckedPackageInfo( vecKBillPackageInfo );
						if ( vecKBillPackageInfo.empty() == false )
						{
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
							Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( kKBillOrderInfo, mapSelectedOptionList, mapOptionGroupIdList, vecKBillPackageInfo );
#else //SERV_CASH_ITEM_SOCKET_OPTION
							Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( kKBillOrderInfo, mapSelectedOptionList, vecKBillPackageInfo );
#endif //SERV_CASH_ITEM_SOCKET_OPTION
						}
					}
					else
#endif// SERV_GLOBAL_CASH_PACKAGE

					{
						if ( m_pCashItemToInvenPopup->IsEmptyCheckedItemList() == false )
						{
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
							Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( kKBillOrderInfo, mapSelectedOptionList, mapOptionGroupIdList );
#else //SERV_CASH_ITEM_SOCKET_OPTION
							Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( kKBillOrderInfo, mapSelectedOptionList );
#endif //SERV_CASH_ITEM_SOCKET_OPTION
						}
					}
				}
				else
				{
					g_pMain->KTDGUIOKMsgBox(D3DXVECTOR2(250,300), GET_STRING( STR_ID_8 ), g_pMain->GetNowState() );
				}
			}
#else // SERV_GLOBAL_BILLING
			vector< KNXBTOrderInfo > vecKNXBTOrderInfo;
			vecKNXBTOrderInfo = m_pCashItemToInvenPopup->GetOrderInfoList();
			if ( vecKNXBTOrderInfo.empty() == false )
			{
				KNXBTOrderInfo& kKNXBTOrderInfo = vecKNXBTOrderInfo[0];
				map<int, int> mapSelectedOptionList;	/// <ItemID, SocketID>
				
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
				map<int, int> mapOptionGroupIdList;		/// <SocketId, GroupdID>				
				if ( m_pCashItemToInvenPopup->GetSelectedOptionListAndGroupID( mapSelectedOptionList, mapOptionGroupIdList ) == true )
#else	SERV_CASH_ITEM_SOCKET_OPTION
				if ( m_pCashItemToInvenPopup->GetSelectedOptionList( mapSelectedOptionList ) == true )
#endif // SERV_CASH_ITEM_SOCKET_OPTION				
				{
					m_pCashItemToInvenPopup->SetOpen( false );
					if ( kKNXBTOrderInfo.m_byteProductKind == 1 )
					{
						vector< KNXBTPackageInfo > vecKNXBTPackageInfo;
						m_pCashItemToInvenPopup->GetCheckedPackageInfo( vecKNXBTPackageInfo );
						if ( vecKNXBTPackageInfo.empty() == false )
						{
#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
							Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( kKNXBTOrderInfo, mapSelectedOptionList, mapOptionGroupIdList, vecKNXBTPackageInfo );
#else	SERV_CASH_ITEM_SOCKET_OPTION
							Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( kKNXBTOrderInfo, mapSelectedOptionList, vecKNXBTPackageInfo );
#endif	// SERV_CASH_ITEM_SOCKET_OPTION
						}
					}
					else
					{
						if ( m_pCashItemToInvenPopup->IsEmptyCheckedItemList() == false )
						{
#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
							Handler_EGS_GET_PURCHASED_CASH_ITEM_REP( kKNXBTOrderInfo, mapSelectedOptionList, mapOptionGroupIdList );
#else	SERV_CASH_ITEM_SOCKET_OPTION
							Handler_EGS_GET_PURCHASED_CASH_ITEM_REP( kKNXBTOrderInfo, mapSelectedOptionList );
#endif	// SERV_CASH_ITEM_SOCKET_OPTION
						}
					}
				}
				else
				{
					g_pMain->KTDGUIOKMsgBox(D3DXVECTOR2(250,300), GET_STRING( STR_ID_8 ), g_pMain->GetNowState() );
				}
			}
#endif // SERV_GLOBAL_BILLING
//////////////////////////////////////////////////////////////////////////
		}
		break;

	case CSCUM_ITEM_TO_INVEN_POP_UP_CANCEL:
		{
			m_pCashItemToInvenPopup->SetOpen( false );
		}
		break;

	case CSCUM_ITEM_TO_INVEN_CHECK_OK:
		{
			if ( m_pDLGCheckCashItemToInven != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCheckCashItemToInven, NULL, false );

			m_pDLGCheckCashItemToInven = NULL;

			map< int, int > tempMap;
			//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING

#ifdef SERV_CASH_ITEM_SOCKET_OPTION
			Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( m_TempCashItemToInven, tempMap, tempMap );
#else //SERV_CASH_ITEM_SOCKET_OPTION
			Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( m_TempCashItemToInven, tempMap );
#endif //SERV_CASH_ITEM_SOCKET_OPTION

#else // SERV_GLOBAL_BILLING

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REP( m_TempCashItemToInven, tempMap, tempMap );
#else	SERV_CASH_ITEM_SOCKET_OPTION
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REP( m_TempCashItemToInven, tempMap );
#endif	// SERV_CASH_ITEM_SOCKET_OPTION

#endif // SERV_GLOBAL_BILLING
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case CSCUM_ITEM_TO_INVEN_OVERLAP_CHECK_OK:
		{
			if ( m_pDLGOverlapCheckCashItemToInven != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOverlapCheckCashItemToInven, NULL, false );

			m_pDLGOverlapCheckCashItemToInven = NULL;

#ifdef SERV_GLOBAL_BILLING
			Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ();
#else // SERV_GLOBAL_BILLING
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ();
#endif // SERV_GLOBAL_BILLING
		}
		break;

	case CSCUM_ITEM_TO_INVEN_OVERLAP_CHECK_OK_PACKAGE:
		{
			if ( m_pDLGOverlapCheckCashItemToInven != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOverlapCheckCashItemToInven, NULL, false );

			m_pDLGOverlapCheckCashItemToInven = NULL;

			Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ();
		}
		break;

	case CSCUM_BUY_ALL_EQUIPPING_ITEMS:
		{
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
			BuyAllEquippingItems();
#else
#ifdef BUY_ALL_CHECK_BALANCE
			// Buy 버튼 누르면 캐시 체크 한번 더 한다. 
			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			if ( NULL != pNowState )
				pNowState->Handler_EGS_CHECK_BALANCE_REQ();
#endif // BUY_ALL_CHECK_BALANCE
			if ( m_vecPreEquipItemID.empty() == false )
			{
#ifdef ADDED_RELATIONSHIP_SYSTEM
				if( false == CheckBuyPossibleRelationItem( m_vecPreEquipItemID ) )
					return false;
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
				// 모든 다격 표시를 위해서 찜 구매와 같은 처리를 한다
				g_pInstanceData->SetChoicedItem(true);
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
				m_pBuyItemPopUp->Reset( m_vecPreEquipItemID );
				m_pBuyItemPopUp->SetOpen( true );
				m_pBuyItemPopUp->SetChoicedItemBuy( false );
	#ifdef CASH_ITEM_REFUND
				SetStringBuyItemRefund();
	#endif CASH_ITEM_REFUND
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_9 ), g_pMain->GetNowState() );
			}
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
		}
		break;

	case CSCUM_CHOICED_BUY:
		{
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
			g_pInstanceData->SetChoicedItem(true);
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

			vector< int > vecItemID;
#ifdef SERV_USE_ENABLE_GIFT
			// 사려고 하는 캐쉬아이템이 캐쉬아이템 리스트에 없다면...
			CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( m_ChoicedItemMenuItemID );

			if(pCashItem == NULL)
			{
				return false;
			}
#endif SERV_USE_ENABLE_GIFT
			if ( m_ChoicedItemMenuItemID != 0 )
				vecItemID.push_back( m_ChoicedItemMenuItemID );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if( false == CheckBuyPossibleRelationItem( vecItemID ) )
				return false;
#endif // ADDED_RELATIONSHIP_SYSTEM
			m_pBuyItemPopUp->Reset( vecItemID );
			m_pBuyItemPopUp->SetOpen( true );
			m_pBuyItemPopUp->SetChoicedItemBuy( true );
#ifdef CASH_ITEM_REFUND
			SetStringBuyItemRefund();
#endif CASH_ITEM_REFUND

			BOOST_FOREACH( const int& iItemID, vecItemID )
			{
				if ( ShowPopUpGuideByItemID( iItemID ) )
					break;
			}

			if ( m_pDLGChoicedItemMenu != NULL )
				m_pDLGChoicedItemMenu->SetShowEnable( false, false );
		}
		break;

	case CSCUM_CHOICED_PRESENT:
		{
#ifdef IMPOSSIBLE_PRESENT_CASH_ITEM
			if( NULL != g_pData && NULL != g_pData->GetItemManager() &&
				true == g_pData->GetItemManager()->IsImpossiblePresentItem(m_ChoicedItemMenuItemID) )
#else
			if ( IsPossiblePresent( m_ChoicedItemMenuItemID ) == false )
#endif //IMPOSSIBLE_PRESENT_CASH_ITEM
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_6 ), g_pMain->GetNowState() );
				return true;
			}

			if ( m_pPresentItemPopup != NULL )
			{
				m_pPresentItemPopup->SetOpen( true, m_ChoicedItemMenuItemID );
				m_pPresentItemPopup->SetChoicedItem( true );
			}

			if ( m_pDLGChoicedItemMenu != NULL )
				m_pDLGChoicedItemMenu->SetShowEnable( false, false );

			return true;
		}
		break;

	case CSCUM_CHOICED_REMOVE_IN_LIST:
		{
			RemoveChoicedItem( m_ChoicedItemMenuItemID );
			if ( m_pDLGChoicedItemMenu != NULL )
				m_pDLGChoicedItemMenu->SetShowEnable( false, false );
			return true;
		}
		break;

	case CSCUM_CHOICED_PRE_EQUIPPING:
		{
			InsertPreEquipItem( m_ChoicedItemMenuItemID );
			if ( m_pDLGChoicedItemMenu != NULL )
				m_pDLGChoicedItemMenu->SetShowEnable( false, false );

			ResetNowEquipUI();
			return true;
		}
		break;

	case CSCUM_CHOICED_CLOSE:
		{
			if ( m_pDLGChoicedItemMenu != NULL )
				m_pDLGChoicedItemMenu->SetShowEnable( false, false );

		}
		break;

	case CSCUM_CHOICED_BUY_ALL:
		{
			if ( m_vecChoicedItemID.empty() == false )
			{
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
				g_pInstanceData->SetChoicedItem(true);
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
#ifdef ADDED_RELATIONSHIP_SYSTEM
				if( false == CheckBuyPossibleRelationItem( m_vecChoicedItemID ) )
					return false;
#endif // ADDED_RELATIONSHIP_SYSTEM
				m_pBuyItemPopUp->Reset( m_vecChoicedItemID );
				m_pBuyItemPopUp->SetOpen( true );
				m_pBuyItemPopUp->SetChoicedItemBuy( true );
#ifdef CASH_ITEM_REFUND
				SetStringBuyItemRefund();
#endif CASH_ITEM_REFUND

				BOOST_FOREACH( const int& iItemID, m_vecChoicedItemID )
				{
					if ( ShowPopUpGuideByItemID( iItemID ) )
						break;
				}
				
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_10 ), g_pMain->GetNowState() );
			}
		}
		break;

	case CSCUM_PRESENT_POP_UP_CHANGED_CASH_PRODUCT:
		{
			if ( m_pPresentItemPopup != NULL )
			{
				m_pPresentItemPopup->UpdateCash();
			}
		}
		break;

	case CSCUM_PRESENT_POP_UP_CAHNGED_FRIEND:
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)lParam;
			if ( m_pPresentItemPopup != NULL )
			{
				m_pPresentItemPopup->SetSelectFriend( pComboBox->GetSelectedItemIndex() );
			}
		}
		break;

	case CSCUM_PRESENT_OK:
		{
			if ( m_pPresentItemPopup != NULL )
			{
				unsigned long productNo = m_pPresentItemPopup->GetSelectedProductNo();
				if ( productNo <= 0 )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_11 ), g_pMain->GetNowState() );
					return true;
				}

				wstring tempNickName = m_pPresentItemPopup->GetNickNameToPresent();
				if ( tempNickName.empty() == true )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_12 ), g_pMain->GetNowState() );
					return true;
				}

				Handler_EGS_PRESENT_CASH_ITEM_REQ( m_pPresentItemPopup->GetNickNameToPresent().c_str(), m_pPresentItemPopup->GetPresentMsg().c_str(),
					m_pPresentItemPopup->GetPassword().c_str(), m_pPresentItemPopup->GetSelectedProductNo(), m_pPresentItemPopup->GetChoicedItem() );

				m_pPresentItemPopup->SetOpen( false, 0 );
			}
		}
		break;

	case CSCUM_PRESENT_CANCEL:
		{
			if ( m_pPresentItemPopup != NULL )
				m_pPresentItemPopup->SetOpen( false, 0 );
		}
		break;

	case CSCUM_BUY_POP_UP_OVERLAP_OK:
		{
#ifdef DISABLE_REDUDANT_PACKET_CASH
			if( true == g_pMain->IsWaitingServerPacket(EGS_BUY_CASH_ITEM_ACK))
				break;
#endif //DISABLE_REDUDANT_PACKET_CASH

			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			m_pBuyItemPopUp->SetOpen( false );

			g_pData->GetServerProtocol()->SendPacket( EGS_BUY_CASH_ITEM_REQ, m_ReservedKEGS_BUY_CASH_ITEM_REQ );
			g_pMain->AddServerPacket( EGS_BUY_CASH_ITEM_ACK );

			m_bSend_EGS_BUY_CASH_ITEM_REQ = true;

		}
		break;

	case CSCUM_BUY_POP_UP_OVERLAP_CANCEL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			m_pBuyItemPopUp->SetOpen( false );
		}
		break;

	case CSCUM_BUY_PLATINUM_CARD_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			m_bCheckDialogWarningPlatinumMembership	= true;

			ProcessBuyPopUpOk();
		}
		break;

	case CSCUM_BUY_PLATINUM_CARD_CANCEL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			m_bCheckDialogWarningPlatinumMembership	= false;

			m_pBuyItemPopUp->SetOpen( false );
		}
		break;
		/*
		case CSCUM_BUY_POP_UP_ALL_CHECK_CHANGED:
		{
		//m_pBuyItemPopUp->
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)lParam;
		if ( pCheckBox != NULL )
		m_pBuyItemPopUp->SetCheckAllItem( pCheckBox->GetChecked() );

		return true;
		}
		break;

		case CSCUM_BUY_POP_UP_EACH_CHECK_CHANGED:
		{
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)lParam;
		if ( pCheckBox != NULL )
		m_pBuyItemPopUp->ChangeCheckItem( pCheckBox->GetDialog(), pCheckBox->GetChecked() );

		return true;
		}
		break;
		*/
	case CSCUM_ITEM_TO_INVEN_POP_UP_ALL_CHECK_CHANGED:
		{
			//m_pBuyItemPopUp->
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)lParam;
			if ( pCheckBox != NULL )
				m_pCashItemToInvenPopup->SetCheckAllItem( pCheckBox->GetChecked() );

			return true;		
		}
		break;

	case CSCUM_ITEM_TO_INVEN_POP_UP_EACH_CHECK_CHANGED:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)lParam;
			if ( pCheckBox != NULL )
				m_pCashItemToInvenPopup->ChangeCheckItem( pCheckBox->GetDialog(), pCheckBox->GetChecked() );

			return true;
		}
		break;
#ifdef CASH_ITEM_REFUND
	case CSCUM_CASH_ITEM_REFUND:
		{
			CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
			CX2Cursor*	pCursor = pState->GetCursor();
			if(pCursor->GetCurorState() == CX2Cursor::XCS_CASH_REFUND)
				pCursor->ChangeCursorState( CX2Cursor::XCS_NORMAL );
			else
				pCursor->ChangeCursorState( CX2Cursor::XCS_CASH_REFUND );
		}
		break;

	case CSCUM_CASH_ITEM_REFUND_OK:
		{
			if ( m_pDLGCheckCashItemRefund != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCheckCashItemRefund, NULL, false );

			m_pDLGCheckCashItemRefund = NULL;

			Handler_EGS_CASH_ITEM_REFUND_REQ();
		}
		break;

#endif CASH_ITEM_REFUND
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
 	case CSCUM_USE_BONUS_SELECT:
 		{
 			m_pBuyItemPopUp->SetUseCashType( (int)KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT );
 			m_pBuyItemPopUp->UpdateCashUI();
 		}
 		break;
 	case CSCUM_PRESENT_USE_BONUS_SELECT:
 		{
			m_pPresentItemPopup->SetUseCashType( (int)KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT );
 			m_pPresentItemPopup->UpdateCash();
 		}
 		break;
#ifdef SERV_BILLING_ADD_GASH_POINT
	case CSCUM_USE_GASH_SELECT:
		{
			m_pBuyItemPopUp->SetUseCashType(KGlobalCashInfo::GCT_PUBLISHER_CASH);
			m_pBuyItemPopUp->UpdateCashUI();
		}
		break;
	case CSCUM_PRESENT_USE_GASH_SELECT:
		{
			m_pPresentItemPopup->SetUseCashType(KGlobalCashInfo::GCT_PUBLISHER_CASH);
			m_pPresentItemPopup->UpdateCash();
		}
		break;
	case CSCUM_USE_GAMEPOINT_SELECT:
		{
			m_pBuyItemPopUp->SetUseCashType(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH);
			m_pBuyItemPopUp->UpdateCashUI();
		}
		break;
	case CSCUM_PRESENT_USE_GAMEPOINT_SELECT:
		{
			m_pPresentItemPopup->SetUseCashType(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH);
			m_pPresentItemPopup->UpdateCash();
		}
		break;
#else
 	case CSCUM_USE_GAMEPOINT_SELECT:
 		{
			m_pBuyItemPopUp->SetUseCashType( (int)KGlobalCashInfo::GCT_KOG_ELSWORD_CASH );
 			m_pBuyItemPopUp->UpdateCashUI();
 		}
 		break;
 	case CSCUM_PRESENT_USE_GAMEPOINT_SELECT:
 		{
			m_pPresentItemPopup->SetUseCashType( (int)KGlobalCashInfo::GCT_KOG_ELSWORD_CASH );
 			m_pPresentItemPopup->UpdateCash();
 		}
 		break;
#endif SERV_BILLING_ADD_GASH_POINT
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef SERV_NEXON_AUTH_SOAP
	case CSUCM_CHECK_AUTHORITY_OK:
		{
#ifdef SEND_NEXON_WISE_LOG_BY_URL // 김태환
			if( false == g_pMain->GetBrowserWrapper()->IsClosed() )	/// 켜져있으면, 끄자
				g_pMain->GetBrowserWrapper()->CloseBrowser();
#else // SEND_NEXON_WISE_LOG_BY_URL
			if( true == g_pMain->GetBrowserWrapper()->IsClosed() )
#endif // SEND_NEXON_WISE_LOG_BY_URL
			{
				g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE ); 

				RECT	rcWebPage;
				string	strURL;

				SetOpenCheckAuthority( strURL, rcWebPage );

				D3DXVECTOR2 vResolution = g_pMain->GetGameOption().GetOptionList().m_vResolution;

				if( rcWebPage.right > static_cast<long>( vResolution.x ) )
					rcWebPage.right = static_cast<long>( vResolution.x );

				if( rcWebPage.bottom > static_cast<long>( vResolution.y ) )
					rcWebPage.bottom = static_cast<long>( vResolution.y );

				rcWebPage.left = static_cast<long>( ( vResolution.x - rcWebPage.right ) * 0.5f );
				rcWebPage.top = static_cast<long>( ( vResolution.y - rcWebPage.bottom ) * 0.5f );

				if( rcWebPage.left < 0 )
					rcWebPage.left = 0;

				if( rcWebPage.top < 0 )
					rcWebPage.top = 0;

				rcWebPage.right		+= rcWebPage.left;
				rcWebPage.bottom	+= rcWebPage.top;


				g_pMain->GetBrowserWrapper()->Create( rcWebPage, g_pKTDXApp->GetHWND() );
				g_pMain->GetBrowserWrapper()->Navigate( strURL.c_str(), NULL ); 
			}
		}
		break;
#endif //SERV_NEXON_AUTH_SOAP
//{{ mauntain : 김태환 [2012.06.12] 매일매일 선물 상자 정보 Dialog 닫기
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	case CSUCM_CLOSE_DAILY_GIFT_BOX:
		{
			if( m_pDlgDailyGiftBox != NULL )
				m_pDlgDailyGiftBox->SetShow( false );
		}
		break;
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

#ifdef TAKE_OFF_ALL_ITEM
	case CSCUM_TAKE_OFF_ALL_ITEM_AND_INVEN_EQUIP_ITEM:
		{
			m_bShowInvenEquip = !m_bShowInvenEquip;
			m_pUnitViewerUI->SetShowInvenEquip( m_bShowInvenEquip );
			ShowInvenEquipUI( m_bShowInvenEquip );

			if ( m_pUnitViewerUI !=	NULL )
				m_pUnitViewerUI->UpdateEqip( true );

			ResetNowEquipUI();
			PreEquipping();
		} break;
#endif // TAKE_OFF_ALL_ITEM

#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	case CSCUM_RADIO_BUTTON_PRE_EQUIP_SET:
		{
			CKTDGUIRadioButton* pCheckBox = reinterpret_cast<CKTDGUIRadioButton*>(lParam);
			if( NULL != pCheckBox )
			{
				m_uiSelectPreEquipSet = pCheckBox->GetDummyInt(0);
				if( -1 != m_uiSelectPreEquipSet )
				{
					if ( m_pUnitViewerUI !=	NULL )
						m_pUnitViewerUI->UpdateEqip( true );

					ResetNowEquipUI();
					PreEquipping();
				}
			}
		} break;
#endif //SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
#ifdef CASH_INVEN_PICKUP_ALL
	case CSCUM_PICKUP_ALL:
		{
			dbg::cout << L"지헌로그-테스트 : 캐쉬 인벤 연속 가져오기 시작" << dbg::endl;
			PickUpAll();
		}
		break;
#endif CASH_INVEN_PICKUP_ALL
#ifdef COUPON_SYSTEM // 할인 쿠폰 콤보박스 변경
	case CSCUM_BUY_COUPON_COMBOBOX_CHANGED:
		{
			CKTDGUIComboBox* pComboBox = reinterpret_cast<CKTDGUIComboBox*>(lParam);
			if ( NULL != pComboBox )
			{
				m_pBuyItemPopUp->ChangeItemCoupon( pComboBox->GetDialog(), pComboBox->GetSelectedItemIndex() );		
			}
			return true;
		}
		break;
#endif // COUPON_SYSTEM

	}

#ifdef COUPON_SYSTEM
	if ( m_pUICouponBox && 
		 true == m_pUICouponBox->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
		return true;
#endif // COUPON_SYSTEM


	return false;
}

bool CX2CashShop::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef SERV_EVENT_BINGO
	if ( NULL != m_pBingo && true == m_pBingo->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
		return true;
#endif //SERV_EVENT_BINGO

#ifdef COUPON_SYSTEM
	if ( NULL != m_pUICouponBox && 
		true == m_pUICouponBox->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
		return true;
#endif // COUPON_SYSTEM
	switch( wParam )
	{
	case EGS_BUY_CASH_ITEM_ACK:
		{
			Handler_EGS_BUY_CASH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#ifdef SERV_GLOBAL_BILLING
// 	case EBILL_BUY_PRODUCT_ACK:
// 		{
// 			Handler_EBILL_BUY_PRODUCT_ACK( hWnd, uMsg, wParam, lParam );
// 		}
	case EGS_BILL_INVENTORY_INQUIRY_ACK:
		{
			Handler_EGS_BILL_INVENTORY_INQUIRY_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} 
		break;

	case EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK:
		{
			Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} 
		break;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	case EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK:
		{
			return Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_GLOBAL_CASH_PACKAGE
#else SERV_GLOBAL_BILLING
	case EGS_PURCHASED_CASH_ITEM_LIST_ACK:
		{
			Handler_EGS_PURCHASED_CASH_ITEM_LIST_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} 
		break;

	case EGS_GET_PURCHASED_CASH_ITEM_ACK:
		{
			Handler_EGS_GET_PURCHASED_CASH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} 
		break;

	case EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK:
		{
			return Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif // SERV_GLOBAL_BILLING

	case EGS_GET_WISH_LIST_ACK:
		{
			return Handler_EGS_GET_WISH_LIST_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_MODIFY_WISH_LIST_ACK:
		{
			return Handler_EGS_MODIFY_WISH_LIST_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_PRESENT_CASH_ITEM_ACK:
		{
			return Handler_EGS_PRESENT_CASH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_EXPAND_CHAR_SLOT_NOT:
		{
			return Handler_EGS_EXPAND_CHAR_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

		//{{ kimhc // 2009-10-06 // 자동결제 처리
#ifdef	AUTO_PAYMENT
	case EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK:
		{
			return Handler_EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
		//}} kimhc // 2009-10-06 // 자동결제 처리
#endif	AUTO_PAYMENT
#ifdef CASH_ITEM_REFUND
	case EGS_CASH_ITEM_REFUND_ACK:
		{
			return Handler_EGS_CASH_ITEM_REFUND_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif CASH_ITEM_REFUND
#ifdef	CHINA_CASH_CHANGE
	case EGS_EXCHANGE_CASH_ACK:
		{
			return Handler_EGS_EXCHANGE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif	CHINA_CASH_CHANGE
#ifdef SERV_NOTIFY_BONUS_CASH
	case EGS_BONUS_CASH_EVENT_NOT:
		{
			return Handler_EGS_BONUS_CASH_EVENT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_NOTIFY_BONUS_CASH
#ifdef SERV_COUNTRY_PH
	case EGS_EXCHANGE_CASH_CLIENT_NOT: // 2013.08.06 lygan_조성욱 // 동남아시아의 경우 전환할때 EGS_EXCHANGE_CASH_REQ 패킷을 못날린다. 그래서 ACK를 받을 수 없기 때문에 NOT로 대신한다.
		{
			return Handler_EGS_EXCHANGE_CASH_CLIENT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_COUNTRY_PH
#ifdef COUPON_SYSTEM
	case EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK:
		{
			return Handler_EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK(hWnd, uMsg, wParam, lParam);
		} break;
#endif // COUPON_SYSTEM
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	case EGS_CASH_DIRECT_CHARGE_CN_ACK:
		{
			return Handler_EGS_CASH_DIRECT_CHARGE_CN_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_CASH_DIRECT_CHARGE_CN_NOT:
		{

		}
		break;
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
	}

	return false;
}



#ifdef SERV_GLOBAL_BILLING
wstring CX2CashShop::GetSlotCashItemPeriodDesc( int itemTID )
{
	wstring slotDesc = L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetItemUID() > 0 )
			return slotDesc;

		if ( m_pNowOverItemSlot->GetSlotType() != CX2Slot::ST_CASH_SHOP_DEPOSIT )
		{
			CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemTID );
			if ( pCashItem != NULL )
			{
				const KBillProductInfo* pKBillProductInfo = pCashItem->GetGateProduct();
				if ( pKBillProductInfo != NULL )
				{
					slotDesc = L"\n";
					for ( int i = 0; i < (int)pCashItem->m_vecKBillProductInfo.size(); i++ )
					{
						KBillProductInfo& kKBillProductInfo = pCashItem->m_vecKBillProductInfo[i];
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP // 같은 아이템 2종류 팔 때	
						bool TempBool = false;
						if( g_pData->GetItemManager()->CheckDisCountItemList( kKBillProductInfo.m_iProductNo, TempBool ) == false )
						{
							continue;
						}
#endif 

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
						if( m_pNowOverItemSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_CHOICED || m_pNowOverItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
						{
							// 찜목록에서는 모든 가격 다 보여준다
						}
						else if( false == g_pInstanceData->IsCurrentSubCategoryInNowCatagory( kKBillProductInfo.m_cCategoryNo ))
							continue;
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

						slotDesc += pCashItem->GetPeriod( kKBillProductInfo );
						slotDesc += L"\n";
					}
				}
			}
		}
		else if ( m_pNowOverItemSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_DEPOSIT )
		{
			//기간 써주는 부분...
			KBillOrderInfo& kKBillOrderInfo = m_pNowOverItemSlot->GetKBillOrderInfo();
			if ( (int)kKBillOrderInfo.m_kBillProductInfo.m_cPeriod != 0 )
			{
				slotDesc = L"\n";
				wstringstream wstrstm;
				wstrstm << GET_STRING( STR_ID_13 ) << L" : "<< (int)kKBillOrderInfo.m_kBillProductInfo.m_cPeriod << GET_STRING( STR_ID_14 );
				slotDesc += wstrstm.str().c_str();
				slotDesc += L"\n";
			}
		}
	}

	return slotDesc;
}

wstring CX2CashShop::GetSlotCashItemDesc( int itemTID )
{
	std::wstring slotItemDesc = L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetItemUID() > 0 )
			return slotItemDesc;
	}

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );
	if ( pItemTemplet != NULL )
	{
		slotItemDesc += L"\n";
		slotItemDesc += pItemTemplet->GetDescriptionInShop();
	}

	//** 선물 관련
	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetKBillOrderInfo().m_byteType == KBillOrderInfo::BCIT_GIFT )
		{
			wstring tempName = m_pNowOverItemSlot->GetKBillOrderInfo().m_wstrSenderNickName;

			if( tempName.empty() == false )
			{
				//tempName = tempName.substr( 1, tempName.find( L"]") -1 );
				slotItemDesc += L"\n";
				slotItemDesc += GET_REPLACED_STRING( ( STR_ID_15, "L", tempName ) );
				slotItemDesc += L"\n";
			}

			wstring tempMsg = m_pNowOverItemSlot->GetKBillOrderInfo().m_wstrSenderPresentMessage;

			if( tempMsg.empty() == false )
			{
				//tempMsg = tempMsg.substr( tempMsg.find( L"]") + 2, (int)tempMsg.size() );

				slotItemDesc += GET_REPLACED_STRING( ( STR_ID_16, "L", tempMsg ) );
			}
		}
	}

	return slotItemDesc;
}
#else // SERV_GLOBAL_BILLING
wstring CX2CashShop::GetSlotCashItemPeriodDesc( int itemTID )
{
	wstring slotDesc = L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetItemUID() > 0 )
			return slotDesc;

		if ( m_pNowOverItemSlot->GetSlotType() != CX2Slot::ST_CASH_SHOP_DEPOSIT )
		{
			CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemTID );
			if ( pCashItem != NULL )
			{
				const KNXBTProductInfo* pKNXBTProductInfo = pCashItem->GetGateProduct();
				if ( pKNXBTProductInfo != NULL )
				{
					slotDesc = L"\n";
					for ( int i = 0; i < (int)pCashItem->m_vecKNXBTProductInfo.size(); i++ )
					{
						KNXBTProductInfo& kKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[i];
						slotDesc += pCashItem->GetPeriod( kKNXBTProductInfo );
						slotDesc += L"\n";
					}
				}
			}
		}
		else if ( m_pNowOverItemSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_DEPOSIT )
		{
			KNXBTOrderInfo& kKNXBTOrderInfo = m_pNowOverItemSlot->GetKNXBTOrderInfo();
			if ( kKNXBTOrderInfo.GetProductExpire() != 0 )
			{
				slotDesc = L"\n";
				wstringstream wstrstm;
				wstrstm << GET_STRING( STR_ID_13 ) << L" : "<< kKNXBTOrderInfo.GetProductExpire() << GET_STRING( STR_ID_14 );
				slotDesc += wstrstm.str().c_str();
				slotDesc += L"\n";
			}
		}
	}

	return slotDesc;
}

wstring CX2CashShop::GetSlotCashItemDesc( int itemTID )
{
	std::wstring slotItemDesc = L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetItemUID() > 0 )
			return slotItemDesc;
	}

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );
	if ( pItemTemplet != NULL )
	{
		slotItemDesc += L"\n";
		slotItemDesc += pItemTemplet->GetDescriptionInShop();
	}

	//여기다가 표시해주면 좋을꺼 같기도 하다 한번 해보자. 뭐를 거시기.. 패키지 아이템에 딸려오는 거시기들
	//CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemTID );
	//if ( pCashItem != NULL )
	//{
	//	if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
	//	{
	//		KNXBTProductInfo& kKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[0];
	//		vector< CX2ItemManager::PackageItemData > vecPackageItemData;
	//		g_pData->GetItemManager()->GetPackageData( _wtoi( kKNXBTProductInfo.m_wstrProductID.c_str() ), vecPackageItemData );

	//		bool bCheckFirst = false;
	//		for ( int i = 0; i < (int)vecPackageItemData.size(); i++ )
	//		{
	//			CX2ItemManager::PackageItemData kPackageItemData = vecPackageItemData[i];

	//			int itemID = kPackageItemData.m_iItemID;
	//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
	//			if ( pItemTemplet != NULL )
	//			{
	//				if ( bCheckFirst == false )
	//				{
	//					slotItemDesc += L"\n\n";
	//					bCheckFirst = true;
	//				}

	//				slotItemDesc += pItemTemplet->m_Name;

	//				wstringstream wstrstm;

	//				if ( kPackageItemData.m_usProductPieces != 0 )
	//				{
	//					wstrstm << L" / " << kPackageItemData.m_usProductPieces << L"개"; 
	//				}
	//				else
	//				{
	//					wstrstm << L" / " << kKNXBTProductInfo.m_usProductExpire << L"일";
	//				}

	//				slotItemDesc += wstrstm.str().c_str();

	//				if ( i != (int)vecPackageItemData.size() - 1)
	//					slotItemDesc += L"\n";
	//			}		

	//		}
	//	}
	//}
	

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetKNXBTOrderInfo().m_wstrProductID.empty() == false )
		{
			if ( m_pNowOverItemSlot->GetKNXBTOrderInfo().m_byteIsPresent == 1 )
			{
				wstring tempName = m_pNowOverItemSlot->GetKNXBTOrderInfo().m_wstrSenderPresentMessage;
				tempName = tempName.substr( 1, tempName.find( L"]") -1 );
				slotItemDesc += L"\n";
                slotItemDesc += GET_REPLACED_STRING( ( STR_ID_15, "L", tempName ) );
				slotItemDesc += L"\n";

				wstring tempMsg = m_pNowOverItemSlot->GetKNXBTOrderInfo().m_wstrSenderPresentMessage;
				tempMsg = tempMsg.substr( tempMsg.find( L"]") + 2, (int)tempMsg.size() );

				slotItemDesc += GET_REPLACED_STRING( ( STR_ID_16, "L", tempMsg ) );
			}
		}
	}

	return slotItemDesc;
}
#endif // SERV_GLOBAL_BILLING

/*virtual*/ wstring CX2CashShop::GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem )
{
	std::wstring slotItemDesc = L"";

	if ( m_bShowRBDownMessage == true )
	{
		if ( m_pNowOverItemSlot != NULL )
		{

            
			if ( m_pNowOverItemSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_DEPOSIT )
			{
				slotItemDesc += L"\n";
				slotItemDesc += GET_STRING( STR_ID_17 );
			}
			else if ( m_pNowOverItemSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_CHOICED )
			{
				slotItemDesc += L"\n";
				slotItemDesc += GET_STRING( STR_ID_18 );
			}
			else
			{

				if( NULL != pItem
					&& true == pItem->IsDisabled()
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
					&& false == pItem->GetIsEvaluation() 
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
					)
				{
					slotItemDesc = L"";
				}
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
							    slotItemDesc += GET_STRING( STR_ID_19 );
						    }
                        }//switch
					}
				}
			}
		}
	}

	return slotItemDesc;
}

void CX2CashShop::MouseMove( D3DXVECTOR2 mousePos )
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

			if ( m_pNowOverItemSlot != NULL && 
				m_pNowOverItemSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_DEPOSIT )
			{
#ifndef SERV_GLOBAL_BILLING
				// 읽었다고 표시하는 부분
				if ( m_pNowOverItemSlot->GetKNXBTOrderInfo().m_byteIsRead == 0 && 
					m_pNowOverItemSlot->GetKNXBTOrderInfo().m_byteIsPresent == 1 )
				{
					m_pNowOverItemSlot->GetKNXBTOrderInfo().m_byteIsRead = 1;
					Handler_EGS_CHECK_PRESENT_CASH_INVENTORY_NOT( m_pNowOverItemSlot->GetKNXBTOrderInfo().m_ulOrderNo, 
						m_pNowOverItemSlot->GetKNXBTOrderInfo().m_ulProductNo );
				}
#endif // ndef SERV_GLOBAL_BILLING
			}

			break;
		}
	}

	if ( bCheck == false )
	{
		SetNowOverItemSlotAndDesc( NULL );
	}

	CX2SlotManager::MouseMove( mousePos );
}

void CX2CashShop::SetOpen( bool bOpen )
{
#ifdef FIX_MISSING_MAIL
	if( bOpen == false )
	{
		PushDelayedMails();
	}
#endif FIX_MISSING_MAIL

	g_pMain->GetInformerManager()->InvalidInfo();

	m_bOpen = bOpen;

	if( NULL != g_pX2Game && g_pX2Game->GetMyUnit() )
		g_pX2Game->GetMyUnit()->SetEnterCashShop( bOpen );

	if ( m_pDLGBack != NULL )
		m_pDLGBack->SetShowEnable( bOpen, bOpen );

	if ( m_pDLGFront != NULL )
		m_pDLGFront->SetShowEnable( bOpen, bOpen );

	if ( m_pUnitViewerUI != NULL )
		m_pUnitViewerUI->SetShowObject( bOpen );

#ifdef SERV_EVENT_BINGO
	if( NULL != m_pBingo )
	{
		m_pBingo->SetOpen(false);
		m_pBingo->SetShowEventButton(bOpen, true);
	}
#endif //SERV_EVENT_BINGO

#ifdef HSB_ALWAYS_VALID_IN_THE_CASHSHOP
	Send_EGS_VISIT_CASH_SHOP_NOT( bOpen );
#endif // HSB_ALWAYS_VALID_IN_THE_CASHSHOP

	if ( bOpen == true )
	{

#ifdef TAKE_OFF_ALL_ITEM
		m_bShowInvenEquip = true;
		if ( m_pUnitViewerUI != NULL )
			m_pUnitViewerUI->SetShowInvenEquip ( m_bShowInvenEquip );		

		ShowInvenEquipUI ( m_bShowInvenEquip );
		ResetNowEquipUI();

		// 오현빈 // 2013-04-11
		// 아래 구문에서 m_pUnitViewerUI->UpdateEqip( true ); 으로 초기화 하고 있지만 한번 더 해주는 이유 
		// 장비의 UID가 아바타보다 뒷번호 일 경우, 순서에 의해 장비가 입혀짐.
		// 아바타 입히는 동작을 한 번 더 수행하면, 장비 위에 다시 아바타를 입히고 있음.
		// 
		// 옷을 입히는 동장근 CX2UnitViewerUI::AddEqip()에서 처리되는데 해당 함수가
		// UpdateEquip()이라는 공통적으로 쓰이는 함수에서 처리되고 있으므로, 따로 예외처리 해주지 않고
		// 옷 입힌느 동작을 두번 하도록 처리함.
		PreEquipping();
#endif // TAKE_OFF_ALL_ITEM

		m_bSend_EGS_BUY_CASH_ITEM_REQ = false;

		if ( m_pUnitViewerUI != NULL )
		{
			m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING );
			m_pUnitViewerUI->UpdateEqip( true );
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
			InitPreEquipItemID();
#else
			m_vecPreEquipItemID.clear();
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
		}

		//{{ kimhc // 2009-08-10 // 은행
#ifdef	PRIVATE_BANK
		// 플래티넘 회원권 구매시 사용되는 bool 값 초기화
		m_bCheckDialogWarningPlatinumMembership	= false;
		// 은행에서 캐시샵의 설치탭으로 바로 가기 위해 추가
		switch ( m_iMenuTypeCallingCashShop )
		{
		case CX2UIManager::UI_MENU_PRIVATE_BANK:
#ifdef DIRECT_OPEN_BUY_DIALOG
		case CX2UIManager::UI_SKILL_SLOT:
		case CX2UIManager::UI_MENU_QUICK_SLOT:
#endif //DIRECT_OPEN_BUY_DIALOG
			{
				SetCashShopState( CX2CashShop::CSS_SUB, CX2CashShop::CSC_INSTALL );
				CKTDGUIRadioButton* pRadioButton = NULL;
				pRadioButton = static_cast< CKTDGUIRadioButton* >( m_pDLGFront->GetControl( L"tab_Setup" ) );

				if ( pRadioButton != NULL )
				{
					pRadioButton->SetChecked( true );
				}
				
			}
			break;
		case CX2UIManager::UI_SKILL_NOTE:
			{
				// # 메모 구매를 위한 카테고리로 이동 
				SetCashShopState( CX2CashShop::CSS_SUB, CX2CashShop::CSC_CONSUME );
				CKTDGUIRadioButton* pRadioButton = pRadioButton = static_cast< CKTDGUIRadioButton* >( m_pDLGFront->GetControl( L"tab_Use" ) );
				if ( NULL != pRadioButton )
					pRadioButton->SetChecked( true );

				if( NULL != m_pDLGSubPageMenu[CSC_CONSUME] )
				{
					ChangeSubCategory( CSSC_CONSUME_ETC );
					pRadioButton = static_cast< CKTDGUIRadioButton* >(m_pDLGSubPageMenu[CSC_CONSUME]->GetControl(L"Tab_Other"));
					if( NULL != pRadioButton )
						pRadioButton->SetChecked(true);
				}
			} break;

#ifdef LINK_CASH_SHOP_WHEN_JOB_CHANGE
		case CX2LVUpEventMgr::LUEMCUM_ELSWORD_SWORD_KNIGHT:
		case CX2LVUpEventMgr::LUEMCUM_ELSWORD_MAGIC_KNIGHT:
		case CX2LVUpEventMgr::LUEMCUM_AISHA_HIGH_MAGICIAN:
		case CX2LVUpEventMgr::LUEMCUM_AISHA_DARK_MAGICIAN:
		case CX2LVUpEventMgr::LUEMCUM_RENA_SNIPING_RANGER:
		case CX2LVUpEventMgr::LUEMCUM_RENA_COMBAT_RANGER:
		case CX2LVUpEventMgr::LUEMCUM_RAVEN_SWORD_TAKER:
		case CX2LVUpEventMgr::LUEMCUM_RAVEN_OVER_TAKER:
		case CX2LVUpEventMgr::LUEMCUM_EVE_CODE_EXOTIC:
		case CX2LVUpEventMgr::LUEMCUM_EVE_CODE_ARCHITECTURE:
		case CX2LVUpEventMgr::LUEMCUM_CHUNG_FURY_GUARDIAN:
		case CX2LVUpEventMgr::LUEMCUM_CHUNG_SHOOTING_GUARDIAN:
			//	1차 추가 전직
		case CX2LVUpEventMgr::LUEMCUM_ELSWORD_SHEATH_KNIGHT:
		case CX2LVUpEventMgr::LUEMCUM_ARME_BATTLE_MAGICIAN:
		case CX2LVUpEventMgr::LUEMCUM_LIRE_TRAPPING_RANGER:
		case CX2LVUpEventMgr::LUEMCUM_RAVEN_WEAPON_TAKER:
		case CX2LVUpEventMgr::LUEMCUM_EVE_ELECTRA:
		case CX2LVUpEventMgr::LUEMCUM_CHUNG_SHELLING_GUARDIAN:
		case CX2LVUpEventMgr::LUEMCUM_ARA_LITTLE_HSIEN:
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
		case CX2LVUpEventMgr::LUEMCUM_ARA_LITTLE_DEVIL:
#endif SERV_ARA_CHANGE_CLASS_SECOND
#ifdef NEW_CHARACTER_EL
		case CX2LVUpEventMgr::LUEMCUM_ELESIS_SABER_KNIGHT:
		case CX2LVUpEventMgr::LUEMCUM_ELESIS_PYRO_KNIGHT:
#endif NEW_CHARACTER_EL
			{
				JobChangeDirectLink( m_iMenuTypeCallingCashShop );
			} break;
#endif LINK_CASH_SHOP_WHEN_JOB_CHANGE

		default:
			{
				SetCashShopState( CX2CashShop::CSS_MAIN );
			}
			break;
		}

		m_iMenuTypeCallingCashShop	= 0;	// 다시 초기화
#else	PRIVATE_BANK
		SetCashShopState( CX2CashShop::CSS_MAIN );
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-10 // 은행

		
		UpdateCashUI();
		ResetNowEquipUI();

		SetEnableBuyEquippingItem( true );

		DestroyItemSlotByType( CX2Slot::ST_CASH_SHOP_DEPOSIT );
#ifdef SERV_GLOBAL_BILLING
		Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#else // SERV_GLOBAL_BILLING
		Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#endif // SERV_GLOBAL_BILLING

		//{{ 09.03.10 김태완
		// 파티창과 퀵퀘창을 강제로 꺼준다
		if( g_pMain->GetPartyUI() != NULL )
		{
#ifdef SERV_PVP_NEW_SYSTEM
			g_pMain->GetPartyUI()->OpenPartyMenu( false );
#endif
			g_pMain->GetPartyUI()->OpenPartyDLG( false );
//			g_pMain->GetPartyUI()->OpenPartyMemberDLG( false );

			//{{ 허상형 : [2009/10/21] //	피버창 추가
			g_pMain->GetPartyUI()->OpenPartyFeverDLG( false );

			//{{ 허상형 : [2011/3/24/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
			//	월드미션 관련 창 끄기
			//g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowEXPBonusDlg( false );
			g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowTimeDlg( false );
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 허상형 : [2011/3/24/] //	월드 미션
#ifdef SERV_GLOBAL_MISSION_MANAGER
			g_pData->GetGlobalMissionManager()->GetUIGlobalMission()->SetShowGlobalMissionDlg( false );
#endif SERV_GLOBAL_MISSION_MANAGER
		}
		
#ifdef SERV_EPIC_QUEST
		if(g_pData->GetUIManager()->GetUIQuestNew() != NULL)
		{
			g_pData->GetUIManager()->GetUIQuestNew()->SetShowQuickQuestDLG(false);
		}
#else
		if(g_pData->GetUIManager()->GetUIQuest() != NULL)
		{
			g_pData->GetUIManager()->GetUIQuest()->SetShowQuickQuestDLG(false);
		}
#endif SERV_EPIC_QUEST
		//}}

#ifdef QUEST_GUIDE		
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
			{
				if( g_pChatBox->GetOpenChatBoxButton() == false )
				{
					g_pChatBox->SetChatBoxLayerUp(true);
					g_pChatBox->HideChatWindowFrame();
					g_pChatBox->HideChatWindow();
					g_pChatBox->HideChatEditBox();			
					g_pChatBox->CloseChatWindow();
					g_pChatBox->OpenChatWindowButton();
				}		

				if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetNpcIndicator() )
				{
					g_pTFieldGame->GetNpcIndicator()->SetShow(false);
				}
			} break;
		case CX2Main::XS_BATTLE_FIELD:
			{
				if( NULL != g_pX2Game )
				{
#ifndef HSB_ALWAYS_VALID_IN_THE_CASHSHOP
					Send_EGS_VISIT_CASH_SHOP_NOT( bOpen );
#endif // HSB_ALWAYS_VALID_IN_THE_CASHSHOP

					CX2BattleFieldGame* pBattleFieldGame = static_cast<CX2BattleFieldGame*>(g_pX2Game);
					if( NULL != pBattleFieldGame ->GetNpcIndicator() )
						pBattleFieldGame->GetNpcIndicator()->SetShow(false);
#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
					if( NULL != pBattleFieldGame->GetMajorParticle() )
						pBattleFieldGame->GetMajorParticle()->SetRenderEffectMadeByGameUnit( false );

					if( NULL != pBattleFieldGame->GetMinorParticle() )
						pBattleFieldGame->GetMinorParticle()->SetRenderEffectMadeByGameUnit( false );

					if( NULL != pBattleFieldGame->GetMajorXMeshPlayer() )
						pBattleFieldGame->GetMajorXMeshPlayer()->SetRenderEffectMadeByGameUnit( false );

					if( NULL != pBattleFieldGame->GetMinorXMeshPlayer() )
						pBattleFieldGame->GetMinorXMeshPlayer()->SetRenderEffectMadeByGameUnit( false );
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
				}
			} break;
		}
#endif //QUEST_GUIDE

		g_pKTDXApp->SkipFrame();
#ifdef PET_PREVIEW
		UpdateDLGPetViewerUI();
#endif //PET_PREVIEW

#ifdef COUPON_SYSTEM
		if ( NULL == m_pUICouponBox )
			m_pUICouponBox = new CX2UICouponBox;
#endif // COUPON_SYSTEM

#ifdef REFORM_SKILL_NOTE_UI
		OpenBuyPopupByReserve();
#endif // REFORM_SKILL_NOTE_UI

	}
	else
	{
		SetEnable( false );
		SetShow( false );

		m_pDLGSelectedItem->SetShow( false );
		SetNowOverItemSlotAndDesc( NULL );
		InvalidSlotDesc();

		if ( m_pDLGMainPage != NULL )
			m_pDLGMainPage->SetShowEnable( false, false );

		if ( m_pDLGSubPage != NULL )
			m_pDLGSubPage->SetShowEnable( false, false );

		for ( int i = 0; i < CX2CashShop::CSC_END; i++ )
		{
			CKTDGUIDialogType pDialog = m_pDLGSubPageMenu[i];
			if ( pDialog != NULL )
				pDialog->SetShowEnable( false, false );
		}

		m_pCashItemToInvenPopup->SetOpen( false );
		m_pBuyItemPopUp->SetOpen( false );
		m_pPresentItemPopup->SetOpen( false, 0 );
		m_pMainNewSM->SetOpen( false );
		m_pMainHotSM->SetOpen( false );
		m_pMainRecommendSM->SetOpen( false );
		m_pSubSM->SetOpen( false );
		m_pSubSpeicialSM->SetOpen( false );

		if ( m_pDLGChoicedItemMenu != NULL )
			m_pDLGChoicedItemMenu->SetShowEnable( false, false );

#ifdef SERV_PVP_NEW_SYSTEM
		g_pMain->GetPartyUI()->OpenPartyMenu( true );
#endif

		//{{ 09.03.10 김태완
		// 파티창과 퀵퀘창을 켜준다
		if( true == g_pData->GetPartyManager()->DoIHaveParty() ||
			true == g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() )
#ifdef SERV_PVP_NEW_SYSTEM
			|| g_pData->GetPartyManager()->GetProcessPvpMatch() == true
#endif
			)
		{
			//{{ 허상형 : [2009/10/21] //	Fever창 추가
			g_pMain->GetPartyUI()->OpenPartyFeverDLG( true );
			g_pMain->GetPartyUI()->UpdatePartyFeverDLG();
		}

		//NPC와 조우중에 캐시샾 들어갔다 나오면 퀙퀘스트 목록 나오는거 막음
#ifdef CASHSHOP_OUT_QUICKQUEST_OFF_TALKING_NPC	
if(g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == false ) // npc와 조우상태가 아니고 스킬트리가 열려있지 않으면
{
#endif //CASHSHOP_OUT_QUICKQUEST_OFF_TALKING_NPC
			//{{ 허상형 : [2011/3/24/] //	월드 미션
	#ifdef SERV_INSERT_GLOBAL_SERVER
			if( g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() == true )
			{
				g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowTimeDlg( true );
			}
	#endif SERV_INSERT_GLOBAL_SERVER
			//}} 허상형 : [2011/3/24/] //	월드 미션
#ifdef SERV_GLOBAL_MISSION_MANAGER
		if( g_pData->GetGlobalMissionManager()->IsActive() == true )
		{
			if( g_pData->GetGlobalMissionManager()->CheckShowGlobalMissionUI() == true )
			{
				g_pData->GetGlobalMissionManager()->GetUIGlobalMission()->SetShowGlobalMissionDlg( true );
			}
		}
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef SERV_EPIC_QUEST
		if(g_pData->GetUIManager()->GetUIQuestNew() != NULL)
		{
			g_pData->GetUIManager()->GetUIQuestNew()->SetShowQuickQuestDLG(true);
		}
#else	
		if(g_pData->GetUIManager()->GetUIQuest() != NULL)
		{
			g_pData->GetUIManager()->GetUIQuest()->SetShowQuickQuestDLG(true);
		}
#endif SERV_EPIC_QUEST
			//NPC와 조우중에 캐시샾 들어갔다 나오면 퀙퀘스트 목록 나오는거 막음
#ifdef CASHSHOP_OUT_QUICKQUEST_OFF_TALKING_NPC	
}	
#endif //CASHSHOP_OUT_QUICKQUEST_OFF_TALKING_NPC
		if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
		{
			if( (g_pSquareGame != NULL) ||	// 거래광장이거나
				(g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == false )	// npc와 조우상태가 아니고 스킬트리가 열려있지 않으면
#ifndef NEW_SKILL_TREE 
				&& g_pInstanceData != NULL && g_pInstanceData->GetSkillTreeUI()->GetShow() == false
#endif		
				 
				 )
			{
				// 채팅UI 마을에 맞게 초기화 한다.
				g_pChatBox->SetChatBoxLayerUp(false);
				g_pChatBox->OpenChatWindow();		
				g_pChatBox->CloseChatWindowButton();
				g_pChatBox->HideChatEditBox();
			}			
		}	

		//}}

#ifdef QUEST_GUIDE
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
			{
				if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetNpcIndicator() )
				{
					g_pTFieldGame->GetNpcIndicator()->SetShow(true);
				}
			} break;
		case CX2Main::XS_BATTLE_FIELD:
			{
				if( NULL != g_pX2Game )
				{
#ifndef HSB_ALWAYS_VALID_IN_THE_CASHSHOP
					Send_EGS_VISIT_CASH_SHOP_NOT( bOpen );
#endif // HSB_ALWAYS_VALID_IN_THE_CASHSHOP

					CX2BattleFieldGame* pBattleFieldGame = static_cast<CX2BattleFieldGame*>(g_pX2Game);
					if( NULL != pBattleFieldGame ->GetNpcIndicator() )
						pBattleFieldGame->GetNpcIndicator()->SetShow(true);

#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
					if( NULL != pBattleFieldGame->GetMajorParticle() )
						pBattleFieldGame->GetMajorParticle()->SetRenderEffectMadeByGameUnit( true );

					if( NULL != pBattleFieldGame->GetMinorParticle() )
						pBattleFieldGame->GetMinorParticle()->SetRenderEffectMadeByGameUnit( true );

					if( NULL != pBattleFieldGame->GetMajorXMeshPlayer() )
						pBattleFieldGame->GetMajorXMeshPlayer()->SetRenderEffectMadeByGameUnit( true );

					if( NULL != pBattleFieldGame->GetMinorXMeshPlayer() )
						pBattleFieldGame->GetMinorXMeshPlayer()->SetRenderEffectMadeByGameUnit( true );
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
				}
			} break;
		}
#endif //QUEST_GUIDE

#ifdef PET_PREVIEW
		ResetPetViewer();
#endif //PET_PREVIEW

#ifdef COUPON_SYSTEM
		SAFE_DELETE( m_pUICouponBox );
#endif // COUPON_SYSTEM

	}




	// note!! CX2CashShop은 cx2statemenu를 상속받은 클래스에서만 사용된다고 가정

#ifdef CHECK_INHERIT_STATE_MENU
	if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
	{
		CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
		if( NULL != pStateMenu )
		{
			if( true == m_bOpen )
			{
				pStateMenu->SetShowMouseClickUI( false, CX2State::MouseClickUI::HR_CASH_SHOP );
			}
			else
			{
				pStateMenu->SetShowMouseClickUI( true, CX2State::MouseClickUI::HR_CASH_SHOP );
			}
		}
	}
	SetShowOtherUI( bOpen );

#ifdef SERV_EVENT_CHECK_POWER
	g_pMain->GetMemoryHolder()->GetCheckPowerEventDLG( (CKTDXStage*)g_pMain->GetNowState() );
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_BATTLE_FIELD:
	case CX2Main::XS_DUNGEON_GAME:
		{
			if( bOpen == true )
			{
				g_pMain->GetMemoryHolder()->SetShowCheckPowerEvent( false );
			}
			else
			{
				g_pMain->GetMemoryHolder()->SetShowCheckPowerEvent( true );
				g_pMain->GetMemoryHolder()->SetShowCheckPowerEventGuidePage( false );
			}
		}break;
	default:
		{
			g_pMain->GetMemoryHolder()->SetShowCheckPowerEvent( true );
			g_pMain->GetMemoryHolder()->SetShowCheckPowerEventGuidePage( false );
		}break;
	}
	g_pMain->GetMemoryHolder()->UpdateCheckPowerEvent();
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	if( bOpen == true )
	{
		g_pMain->GetMemoryHolder()->SetShow4thEvent( false );
	}
#endif //SERV_4TH_ANNIVERSARY_EVENT
}

void CX2CashShop::ResetUnitViewer( CX2Unit* pUnit )
{
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	ResizePreEquipItemID();
#else
	m_vecPreEquipItemID.resize(0);
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST

	if ( m_pUnitViewerUI == NULL )
	{
		m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
		//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
		//m_pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
		//m_pUnitViewerUI->SetLightPos( -250000, 50000, -500000 );	// 캐릭터뷰어 라이트 위치 변경
		m_pUnitViewerUI->SetLightPos( -250, 100, -600 );	// 캐릭터뷰어 라이트 위치 변경
		
		g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );
		
#ifdef SEPARATION_MOTION
		m_pUnitViewerUI->SetUnit( pUnit, CX2UnitViewerUI::UVS_CASHSHOP ); 
#else
		m_pUnitViewerUI->SetUnit( pUnit );
#endif
		m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250 -105, -60 + 39 + 20 -5000, 200 ) );
		m_pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.5f, 1.5f, 1.5f ) );
		m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );

		m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
	}
	else
	{
#ifdef SEPARATION_MOTION
		m_pUnitViewerUI->SetUnit( pUnit, CX2UnitViewerUI::UVS_CASHSHOP ); 
#else
		m_pUnitViewerUI->SetUnit( pUnit );
#endif
	}



	if ( m_pUnitViewerUI != NULL )
	{
		m_pUnitViewerUI->SetShowObject( m_bOpen, true );
	}
}


void CX2CashShop::ResetUnitViewerEquip()
{
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	ResizePreEquipItemID();
#else
	m_vecPreEquipItemID.resize(0);
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST

	if ( m_pUnitViewerUI != NULL )
	{
		m_pUnitViewerUI->UpdateEqip( true );
		m_pUnitViewerUI->SetShowObject( m_pUnitViewerUI->GetShowObject(), true );
	}
}


void CX2CashShop::UpdateCashUI()
{
	if ( m_pDLGFront != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"g_pStaticcash_num" );
		if ( pStatic != NULL )
		{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalMainCash() ).c_str() );
#else //SERV_SUPPORT_SEVERAL_CASH_TYPES
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetCash() ).c_str() );
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		}
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef SERV_BILLING_ADD_GASH_POINT
		CKTDGUIStatic* pStaticGash = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"g_pStaticgashpoint_num" );
		if ( pStaticGash != NULL )
		{
			pStaticGash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) ).c_str() );
		}
#endif SERV_BILLING_ADD_GASH_POINT
#ifdef CHINA_CASH_CHANGE // 2011.02.24 lygan_조성욱 // 중국 전용 캐쉬 전환 UI용
		CKTDGUIStatic* pStaticGiant = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"g_pStaticcash_numGiant" );
		if ( pStaticGiant != NULL )
		{
			pStaticGiant->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) ).c_str() );
		}
		CKTDGUIStatic* pStaticBonus = (CKTDGUIStatic*)m_pDLGFront->GetControl( L"g_pStaticcash_numBonus" );
		if ( pStaticBonus != NULL )
		{
			pStaticBonus->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT) ).c_str() );
		}
#endif CHINA_CASH_CHANGE
#endif SERV_SUPPORT_SEVERAL_CASH_TYPES
	}

	if ( m_pBuyItemPopUp != NULL )
		m_pBuyItemPopUp->UpdateCashUI();

	if ( m_pPresentItemPopup != NULL )
		m_pPresentItemPopup->UpdateCash();
}

bool CX2CashShop::Handler_EGS_GET_WISH_LIST_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_GET_WISH_LIST_REQ );
	g_pMain->AddServerPacket( EGS_GET_WISH_LIST_ACK );

	return true;
}

#ifdef SERV_GLOBAL_BILLING
bool CX2CashShop::Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( int iPageIndex, int nItemPerPage )
{
	m_iReqDepositPageIndex = iPageIndex;

	KEGS_BILL_INVENTORY_INQUIRY_REQ kPacket;
	kPacket.m_iCurrentPage = iPageIndex;
	kPacket.m_nItemPerPage = nItemPerPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_BILL_INVENTORY_INQUIRY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BILL_INVENTORY_INQUIRY_ACK );

	return true;
}
#else // SERV_GLOBAL_BILLING
bool CX2CashShop::Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( int iPageIndex, int nItemPerPage )
{
	m_iReqDepositPageIndex = iPageIndex;

	KEGS_PURCHASED_CASH_ITEM_LIST_REQ kPacket;
	kPacket.m_iCurrentPage = iPageIndex;
	kPacket.m_nItemPerPage = nItemPerPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_PURCHASED_CASH_ITEM_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PURCHASED_CASH_ITEM_LIST_ACK );

	return true;
}
#endif // SERV_GLOBAL_BILLING


void CX2CashShop::InitUI( CKTDXStage* pStage )
{
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	m_pDLGFront			= new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Right_Front_new.lua" );
#else
#if defined CLIENT_COUNTRY_TW
	m_pDLGFront			= new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Right_Front_TW.lua" );
#elif defined CLIENT_COUNTRY_HK
	m_pDLGFront			= new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Right_Front_HK.lua" );
#elif defined ( CLIENT_COUNTRY_CN )
	m_pDLGFront			= new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Right_Front_CN.lua" );
#else
	m_pDLGFront			= new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Right_Front.lua" );
#endif
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFront );

	//{{ kimhc // 2009-10-08 // 자동결제가 define되있지 않으면 자동결제 탭이 보이지 않도록!!
#ifndef	AUTO_PAYMENT
	if ( m_pDLGFront->GetRadioButton_LUA( "tab_Auto_Account" ) != NULL )
		m_pDLGFront->GetRadioButton_LUA( "tab_Auto_Account" )->SetShowEnable( false, false );
#endif	AUTO_PAYMENT
	//}} kimhc // 2009-10-08 // 자동결제

#ifndef	ADD_CASH_SHOP_CATEGORY_EVENT_2
	if ( m_pDLGFront->GetRadioButton_LUA( "tab_Event_2" ) != NULL )
		m_pDLGFront->GetRadioButton_LUA( "tab_Event_2" )->SetShowEnable( false, false );
#endif	ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef NO_REFUND_BUTTON
	CKTDGUIControl* pRefund = m_pDLGFront->GetControl( L"refund" );

	if( pRefund != NULL )
	{
		pRefund->SetShowEnable( false , false );
	}
#endif // NO_REFUND_BUTTON

	m_pDLGMainPage		= new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Mainpage.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMainPage );

#ifdef CASH_INVEN_PICKUP_ALL
	// 영자만 버튼 보이게
	CKTDGUIButton* pButtonPickUpAll = (CKTDGUIButton*)m_pDLGMainPage->GetControl( L"PickUpAll" );
	if( pButtonPickUpAll != NULL )
		pButtonPickUpAll->SetShowEnable( true, true );
#endif CASH_INVEN_PICKUP_ALL

	m_pDLGSubPage		= new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSubPage );

	for ( int i = 0; i < CX2CashShop::CSC_END; i++ )
	{
		CKTDGUIDialogType pDialog = NULL;
		CX2CashShop::CASH_SHOP_CATEGORY cashShopCate = (CX2CashShop::CASH_SHOP_CATEGORY)i;

		switch( cashShopCate ) 
		{
		case CX2CashShop::CSC_FASHION:
			{
				pDialog = new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage_Fashion.lua" );	
			}
			break;

		case CX2CashShop::CSC_ACCESSORY:
			{
				pDialog = new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage_Accessory.lua" );	
			}
			break;

		case CX2CashShop::CSC_CONSUME:
			{
				pDialog = new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage_Consume.lua" );	
			}
			break;

		case CX2CashShop::CSC_INSTALL:
			{
				pDialog = new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage_Install.lua" );	
			}
			break;

		case CX2CashShop::CSC_PET:
			{
				pDialog = new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage_Pet.lua" );	
				
				
			}
			break;

		case CX2CashShop::CSC_EVENT:
			{
				pDialog = new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage_Event.lua" );	
			}
			break;

			//{{ kimhc // 2009-09-29 // 자동결제 카테고리 추가
#ifdef	AUTO_PAYMENT	
		case CX2CashShop::CSC_AUTO_PAYMENT:
			{
				pDialog = new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage_Auto_Payment.lua" );	
			}
			break;
#endif	AUTO_PAYMENT
				//}} kimhc // 2009-09-29 // 자동결제 카테고리 추가
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
		case CX2CashShop::CSC_EVENT_2:
			{
				pDialog = new CKTDGUIDialog( pStage, L"DLG_Cash_Shop_Subpage_Event_2.lua" );	
			}
			break;
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

		}

		if ( pDialog != NULL )
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialog );

		m_pDLGSubPageMenu[i] = pDialog;
	}

	/*
	vector< ShopSlot* > m_vecMainNewShopSlot;
	vector< ShopSlot* > m_vecMainHotShopSlot;
	vector< ShopSlot* > m_vecMainRecommendShopSlot;
	*/
	m_pMainNewSM		= new CashShopSlotManager( CX2Slot::ST_CASH_SHOP_MAIN_NEW );		
	m_pMainHotSM		= new CashShopSlotManager( CX2Slot::ST_CASH_SHOP_MAIN_HOT );
	m_pMainRecommendSM	= new CashShopSlotManager( CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND );

	m_pSubSM			= new CashShopSlotManager( CX2Slot::ST_CASH_SHOP_SUB );
	m_pSubSpeicialSM	= new CashShopSlotManager( CX2Slot::ST_CASH_SHOP_SUB_SPECIAL );
	
#if defined CLIENT_COUNTRY_TW
	m_pBuyItemPopUp		= new BuyItemPopup( L"DLG_Cash_Shop_Popup_Buy_TW.lua" );
#elif defined CLIENT_COUNTRY_HK
	m_pBuyItemPopUp		= new BuyItemPopup( L"DLG_Cash_Shop_Popup_Buy_HK.lua" );
#elif defined ( CLIENT_COUNTRY_CN )
	m_pBuyItemPopUp		= new BuyItemPopup( L"DLG_Cash_Shop_Popup_Buy_CN.lua" );
#else
	m_pBuyItemPopUp		= new BuyItemPopup( L"DLG_Cash_Shop_Popup_Buy.lua" );
#endif

#ifdef SERV_NOTIFY_BONUS_CASH
	m_fBlinkTime = 0.f;
	m_nowColor = D3DXCOLOR(0.3f, 1.f, 1.f, 1.f);
#endif //SERV_NOTIFY_BONUS_CASH

#ifdef COUPON_SYSTEM
//	m_pBuyItemPopUp		= new BuyItemPopup( L"DLG_Cash_Shop_Popup_Buy_New.lua" ); // 해외팀 제거
	m_pBuyItemPopUp->Init( new BuyItemPopupSlot(), L"DLG_Cash_Shop_Popup_BUY_Slot_New.lua", 2 );
#else
//	m_pBuyItemPopUp		= new BuyItemPopup( L"DLG_Cash_Shop_Popup_Buy.lua" ); // 해외팀 제거
	m_pBuyItemPopUp->Init( new BuyItemPopupSlot(), L"DLG_Cash_Shop_Popup_Buy_Slot.lua", 4 );
#endif //COUPON_SYSTEM
	m_pBuyItemPopUp->SetReverseDLGLayer( true );

	m_pCashItemToInvenPopup = new CashItemToInvenPopup( L"DLG_Cash_Shop_Popup_ItemToInven.lua" );
	m_pCashItemToInvenPopup->Init( new CashItemToInvenPopupSlot(), L"DLG_Cash_Shop_Popup_ItemToInven_Slot.lua", 3 );
	m_pCashItemToInvenPopup->SetReverseDLGLayer( true );

	m_pPresentItemPopup = new PresentItemPopup();

	ResetUnitViewer( g_pData->GetMyUser()->GetSelectUnit() );

#ifdef SERV_EVENT_BINGO
	m_pBingo = new CX2UIBingo(pStage);
#endif //SERV_EVENT_BINGO
}

void CX2CashShop::CategoryDataParsing( const WCHAR* pFileName )
{
//{{ robobeg : 2008-10-28
	//KLuaManager kLuamanager;
    KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28

//{{ robobeg : 2008-10-28
	//g_pKTDXApp->LoadAndDoMemory( &kLuamanager, L"Enum.lua" );
//}} robobeg : 2008-10-28
	g_pKTDXApp->LoadAndDoMemory( &kLuamanager, pFileName );


	if( kLuamanager.BeginTable( "CASH_SHOP_REAL_CATEGORY_ID" ) == true )
	{
		for ( int i = 1; i < CSC_END; i++ )
		{
			if( kLuamanager.BeginTable( i ) == true )
			{	
				int realID = 0;
				vector< D3DXVECTOR2 > vecSubCateID;

				LUA_GET_VALUE( kLuamanager, "REAL_ID", realID, 0 );

				if( kLuamanager.BeginTable( "CASH_SHOP_REAL_SUB_CATEGORY_ID" ) == true )
				{
					int index = 1;
					while( kLuamanager.BeginTable( index ) == true )
					{
						index++;
						D3DXVECTOR2 cateData;
						LUA_GET_VALUE( kLuamanager, 1, cateData.x, 0 );
						LUA_GET_VALUE( kLuamanager, 2, cateData.y, 0 );

						vecSubCateID.push_back( cateData );

						kLuamanager.EndTable();
					}

					kLuamanager.EndTable();	
				}

				kLuamanager.EndTable();

				CX2CashShop::CashShopCategory* pCashShopCategory = new CashShopCategory();
				pCashShopCategory->m_CategoryID = D3DXVECTOR2( (float)i, (float)realID );
				pCashShopCategory->m_vecSubCategoryID = vecSubCateID;
				m_vecCashShopCateList.push_back( pCashShopCategory );
			}
			else
			{
				ErrorLog( XEM_ERROR129 )
#ifndef _SERVICE_
					MessageBox( g_pKTDXApp->GetHWND(), L"CX2LocationManager CASH_SHOP_REAL_CATEGORY_ID 아이디 빠진게 있넹", L"Error", MB_OK );
#endif
			}

		}

		kLuamanager.EndTable();
	}
	else
	{
		ErrorLog( XEM_ERROR128 );
#ifndef _SERVICE_
		MessageBox( g_pKTDXApp->GetHWND(), L"CX2CashShop CASH_SHOP_REAL_CATEGORY_ID BeginTable", L"Error", MB_OK );

#endif
		return;
	}
}
#ifdef SERV_GLOBAL_BILLING
#ifdef COUPON_SYSTEM
bool CX2CashShop::Handler_EGS_BUY_CASH_ITEM_REQ( vector< std::pair<unsigned long, int> > vecProductAndCouponNoList, bool bChoicedItemBuy /*= false*/ )
#else
bool CX2CashShop::Handler_EGS_BUY_CASH_ITEM_REQ( vector< unsigned long >& vecProductNoList, bool bChoicedItemBuy /* = false */ )
#endif // COUPON_SYSTEM
{
#ifdef DISABLE_REDUDANT_PACKET_CASH
	if( true == g_pMain->IsWaitingServerPacket(EGS_BUY_CASH_ITEM_ACK))
		return false;
#else //DISABLE_REDUDANT_PACKET_CASH
	if ( m_bSend_EGS_BUY_CASH_ITEM_REQ == true )
		return false;	
#endif //DISABLE_REDUDANT_PACKET_CASH

	KEGS_BUY_CASH_ITEM_REQ kPacket;
	kPacket.m_bIsWishList = bChoicedItemBuy;
	std::vector< KNXBTPurchaseReqInfo > vecKNXBTPurchaseReqInfo;

#ifdef COUPON_SYSTEM // 구매 시 상품 번호와 쿠폰 번호를 매칭시켜 패킷에 담기
	kPacket.m_bUseCoupon = false;

	// 상품번호 & 쿠폰 번호를 패킷에 같이 담기
	for ( UINT uiIndex = 0; uiIndex < vecProductAndCouponNoList.size(); ++uiIndex )
	{
		KNXBTPurchaseReqInfo kReqInfo;
		// 상품 번호
		kReqInfo.m_ulProductNo = vecProductAndCouponNoList[uiIndex].first;
		// 쿠폰 번호
		kReqInfo.m_iCouponCardNo = vecProductAndCouponNoList[uiIndex].second;
		if( 0 < kReqInfo.m_iCouponCardNo )
		{
			kPacket.m_bUseCoupon = true;
		}
		else
		{
			kReqInfo.m_iCouponCardNo = -1;
		}

		kReqInfo.m_usOrderQuantity = 1;		
		vecKNXBTPurchaseReqInfo.push_back( kReqInfo );
	}
#else
	for ( int i = 0; i < (int)vecProductNoList.size(); i++ )
	{
		unsigned long productNo = vecProductNoList[i];
		KNXBTPurchaseReqInfo kReqInfo;
		kReqInfo.m_ulProductNo = productNo;
		kReqInfo.m_usOrderQuantity = 1;
		vecKNXBTPurchaseReqInfo.push_back( kReqInfo );
	}
#endif // COUPON_SYSTEM

	if ( vecKNXBTPurchaseReqInfo.empty() == true )
		return true;

	kPacket.m_vecPurchaseReqInfo = vecKNXBTPurchaseReqInfo;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	// UI 에서 무슨 캐쉬로 구매할건지 선택한걸 저장한다
	// 아직 UI 없으니 추가는 나중에
	kPacket.m_iUseCashType = m_pBuyItemPopUp->GetUseCashType();
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	//검사해보고 거시기 하면 //
	//if (  )
	vector<int> vecOverlapItemID;
	for ( int i = 0; i < (int)kPacket.m_vecPurchaseReqInfo.size(); i++ )
	{
		KNXBTPurchaseReqInfo kReqInfo = kPacket.m_vecPurchaseReqInfo[i];
		KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( (int)kReqInfo.m_ulProductNo );
		if ( pKBillProductInfo != NULL )
		{
			int itemID = pKBillProductInfo->m_iProductID;

			//{{ 허상형 : [2010/7/9/] //	인벤토리 확장 캐시 막기 코드(임시)
			int iCategory = CX2Inventory::ST_NONE;

			switch( itemID )
			{
			case INVENTORY_SLOT_ADD_ITEM_EQUIP:
				{
					iCategory = CX2Inventory::ST_EQUIP;
					break;
				}
			case INVENTORY_SLOT_ADD_ITEM_ACCESSORY:
				{
					iCategory = CX2Inventory::ST_ACCESSORY;
					break;
				}
			case INVENTORY_SLOT_ADD_ITEM_QUICK_SLOT:
				{
					iCategory = CX2Inventory::ST_QUICK_SLOT;
					break;
				}
			case INVENTORY_SLOT_ADD_ITEM_MATERIAL:
				{
					iCategory = CX2Inventory::ST_MATERIAL;
					break;
				}
			case INVENTORY_SLOT_ADD_ITEM_QUEST:
				{
					iCategory = CX2Inventory::ST_QUEST;
					break;
				}
			case INVENTORY_SLOT_ADD_ITEM_SPECIAL:
				{
					iCategory = CX2Inventory::ST_SPECIAL;
					break;
				}
			default:
				iCategory = CX2Inventory::ST_NONE;
			}

			if( iCategory != CX2Inventory::ST_NONE &&
				//g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL
				//g_pData->GetMyUser()->GetSelectUnit()->GetInventory() != NULL
				)
			{
				//  인벤 최대 공간 체크
				if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum( static_cast<CX2Inventory::SORT_TYPE>(iCategory) ) >= INVENTORY_SLOT_MAX_NUM )
				{
					// 최대 확장 가능한 공간이상이면 실패처리
					m_pBuyItemPopUp->SetOpen( false );
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_5197 ), g_pMain->GetNowState() );
					return false;
				}
			}
			//}} 허상형 : [2010/7/9/] //	인벤토리 확장 캐시 막기 코드(임시)

			//{{ kimhc // 2009-08-11 // PLATINUM 은행 회원권 구매시 예외처리
#ifdef	PRIVATE_BANK
			if ( ( itemID	== PLATINUM_MEMBERSHIP_CARD || itemID == COBO_CREDIT_CARD ) 
				&& m_bCheckDialogWarningPlatinumMembership == false )
			{
				if ( g_pData->GetQuestManager() != NULL )
				{
					// 은행 퀘스트를 클리어 했는가?
					//if ( g_pData->GetQuestManager()->GetMapCompleteQuest().find( _CONST_UI_PRIVATE_BANK_::g_iBankQuestID )
					//	== g_pData->GetQuestManager()->GetMapCompleteQuest().end() )	// 퀘스트를 클리어 하지 않았으면
					if ( g_pData->GetQuestManager()->GetUnitCompleteQuest( _CONST_UI_PRIVATE_BANK_::g_iBankQuestID ) == false )
					{
						if ( itemID	== PLATINUM_MEMBERSHIP_CARD )
						{
#ifdef BUY_COBO_BEGLAUBIGUNG_BUG_FIX
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_20891 ), 
								g_pMain->GetNowState(), CSCUM_BUY_PLATINUM_CARD_OK );
#else //BUY_COBO_BEGLAUBIGUNG_BUG_FIX
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3869 ), CSCUM_BUY_PLATINUM_CARD_OK, 
								g_pMain->GetNowState(), CSCUM_BUY_PLATINUM_CARD_CANCEL );
#endif //BUY_COBO_BEGLAUBIGUNG_BUG_FIX
						}
						else if ( itemID == COBO_CREDIT_CARD )
						{
#ifdef BUY_COBO_BEGLAUBIGUNG_BUG_FIX
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_20891 ), 
								g_pMain->GetNowState(), CSCUM_BUY_PLATINUM_CARD_OK );
#else //BUY_COBO_BEGLAUBIGUNG_BUG_FIX
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4353 ), CSCUM_BUY_PLATINUM_CARD_OK, 
								g_pMain->GetNowState(), CSCUM_BUY_PLATINUM_CARD_CANCEL );
#endif //BUY_COBO_BEGLAUBIGUNG_BUG_FIX
						}



						return false;
					}					
				}
			}
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-11 // PLATINUM 은행 회원권 구매시 예외처리

			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
			if ( pItemTemplet != NULL )
			{
				if ( pItemTemplet->GetItemType() == CX2Item::IT_WEAPON || 
					pItemTemplet->GetItemType() == CX2Item::IT_DEFENCE || 
					pItemTemplet->GetItemType() == CX2Item::IT_ACCESSORY )
				{
					if ( g_pData->GetMyUser()->GetSelectUnit() != NULL
						&& g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( itemID, true ) != NULL )
					{
						vecOverlapItemID.push_back( itemID );
					}
				}
			}
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
			//무제한 2차 전직 아이템 소지 하고 있는 경우 구매하지 못하도록 예외처리
			if( itemID == UNLIMITED_SECOND_CHANGE_JOB_ITEM_ID )
			{
				if( g_pData->GetMyUser()->GetSelectUnit() != NULL
					&& g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( itemID ) != NULL
					|| 2 != g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel()
					)
				{
					//무슨 아이템을 구매 할 수 없습니다. 라는 메시지를 뛰우자
					m_pBuyItemPopUp->SetOpen( false );
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_29780 ), g_pMain->GetNowState() );
					return false;
				}
				else
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_29805 ), g_pMain->GetNowState() );
				}
			}			
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_GOLD_TICKET// 특정 캐쉬로만 골드티켓을 구입할 수 있다.
			switch(itemID)
			{
			case GOLD_TICKET_10_ID:
			case GOLD_TICKET_50_ID:
			case GOLD_TICKET_100_ID:
			case GOLD_TICKET_500_ID:
			case GOLD_TICKET_1000_ID:
			case GOLD_TICKET_2000_ID:
				{
					if( kPacket.m_iUseCashType != KGlobalCashInfo::GCT_PUBLISHER_CASH )
					{
						// 골드 티켓을 ?? 으로 구매 할 수 없습니다! 라는 메시지를 띄우자.
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_15274 ), g_pMain->GetNowState() );
						return false;
					}
				}
				break;
			}
#endif //SERV_GOLD_TICKET
		}
	}

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_iUseCashType = m_pBuyItemPopUp->GetUseCashType();
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	if ( vecOverlapItemID.empty() == false )
	{
		wstring wstrItemNames;
		for ( int i = 0; i < (int)vecOverlapItemID.size(); i++ )
		{
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( vecOverlapItemID[i] );
			if ( pItemTemplet != NULL )
			{
				wstrItemNames += pItemTemplet->GetName();
				if ( i != (int)vecOverlapItemID.size() - 1 )
				{
					wstrItemNames += L", ";
				}
			}

		}
		m_ReservedKEGS_BUY_CASH_ITEM_REQ = kPacket;
		g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_20, "L", wstrItemNames ) ), CSCUM_BUY_POP_UP_OVERLAP_OK, 
			g_pMain->GetNowState(), CSCUM_BUY_POP_UP_OVERLAP_CANCEL );
		return false;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_BUY_CASH_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BUY_CASH_ITEM_ACK );

	m_bSend_EGS_BUY_CASH_ITEM_REQ = true;

	return true;
}

bool CX2CashShop::Handler_EGS_BUY_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BUY_CASH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bSend_EGS_BUY_CASH_ITEM_REQ = false;

	if ( g_pMain->DeleteServerPacket( EGS_BUY_CASH_ITEM_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( kEvent.m_bIsWishList == true )
			{
				vector< int > vecItemID;
				for ( int i = 0; i < (int)kEvent.m_vecPurchaseReqInfo.size(); i++ )
				{
					KNXBTPurchaseReqInfo& kKNXBTPurchaseReqInfo = kEvent.m_vecPurchaseReqInfo[i];
					KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( (int)kKNXBTPurchaseReqInfo.m_ulProductNo );
					if ( pKBillProductInfo != NULL )
					{
						vecItemID.push_back( pKBillProductInfo->m_iProductID );
					}
				}
				Handler_EGS_MODIFY_WISH_LIST_REQ( false, vecItemID );	
			}			

			bool bCheckBuyResurrectionStone		= false;
			bool bCheckBuyInvenSlotAdd			= false;
			//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해
#ifdef	PRIVATE_BANK
			bool bCheckBuyBankMembershipItem	= false;
#endif	PRIVATE_BANK
			//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해

			int rebirthStoneNum = 0;

			for ( int i = 0; i < (int)kEvent.m_vecPurchaseReqInfo.size(); i++ )
			{
				KNXBTPurchaseReqInfo kReqInfo = kEvent.m_vecPurchaseReqInfo[i];
				KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( (int)kReqInfo.m_ulProductNo );
				if ( pKBillProductInfo != NULL )
				{
					int itemID = pKBillProductInfo->m_iProductID;
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
					if ( pItemTemplet != NULL )
					{
						if ( pItemTemplet->GetItemType() == CX2Item::IT_WEAPON || 
							pItemTemplet->GetItemType() == CX2Item::IT_DEFENCE || 
							pItemTemplet->GetItemType() == CX2Item::IT_ACCESSORY )
						{
							if ( g_pData->GetMyUser()->GetSelectUnit() != NULL
								&& g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( itemID, true ) != NULL )
							{

								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_21 ), g_pMain->GetNowState() );
							}
						}
					}
				}
			}

			wstringstream wstrstm;
			for ( int i = 0;  i < (int)kEvent.m_vecPurchaseReqInfo.size(); i++ )
			{
				KNXBTPurchaseReqInfo& kKNXBTPurchaseReqInfo = kEvent.m_vecPurchaseReqInfo[i];
				KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( (int)kKNXBTPurchaseReqInfo.m_ulProductNo );

				if ( pKBillProductInfo != NULL )
				{

					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pKBillProductInfo->m_iProductID );
					CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( pKBillProductInfo->m_iProductID );
					if ( pItemTemplet != NULL && pCashItem != NULL )
					{
						//127030부활석
						if ( 127030 == pItemTemplet->GetItemID() )
						{
							bCheckBuyResurrectionStone = true;
							rebirthStoneNum = (int)pKBillProductInfo->m_cQuantity;
						}

						//200750 소지품 8칸 확장
						if ( INVENTORY_SLOT_ADD_ITEM == pItemTemplet->GetItemID() )
						{
							bCheckBuyInvenSlotAdd = true;
						}

						//{{ kimhc // 2009-08-11 // 은행 회원권 구매시 다이얼로그 처리 다르게 하기위해
#ifdef	PRIVATE_BANK
						switch ( pItemTemplet->GetItemID() )
						{
						case SILVER_MEMBERSHIP_CARD:
						case GOLD_MEMBERSHIP_CARD:
						case EMERALD_MEMBERSHIP_CARD:
						case DIAMOND_MEMBERSHIP_CARD:
						case PLATINUM_MEMBERSHIP_CARD:
						case COBO_CREDIT_CARD:
							bCheckBuyBankMembershipItem		= true;
							break;

						default:
							break;
						}
#endif	PRIVATE_BANK
						//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해
#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
						wstrstm << pItemTemplet->GetName() << L"(" << pCashItem->GetPeriod( *pKBillProductInfo, false ) << L")";
#else
						wstrstm << pItemTemplet->GetName() << L"(" << pCashItem->GetPeriod( *pKBillProductInfo ) << L")";
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
						if ( i != (int)kEvent.m_vecPurchaseReqInfo.size() - 1 )
						{
							wstrstm << L", ";
						}
					}

				}
				if( i > 2 )
				{
					wstrstm << GET_REPLACED_STRING( (STR_ID_4608, "i", kEvent.m_vecPurchaseReqInfo.size()) ) << L"\n";
					break;
				}

				if ( i == (int)kEvent.m_vecPurchaseReqInfo.size() - 1 )
				{
					wstrstm << GET_STRING( STR_ID_22 ) << L"\n";
					if ( bCheckBuyResurrectionStone == true && g_pData->GetMyUser()->GetSelectUnit() != NULL )
						wstrstm << GET_REPLACED_STRING( ( STR_ID_23, "i", g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() + rebirthStoneNum ) ) << L"\n";
					if ( bCheckBuyInvenSlotAdd == true && g_pData->GetMyUser()->GetSelectUnit() != NULL )
						wstrstm << GET_REPLACED_STRING( ( STR_ID_25, "i", g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum(CX2Inventory::ST_EQUIP) + 8 ) );
					//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해
#ifdef	PRIVATE_BANK
					if ( bCheckBuyBankMembershipItem	== true )
						wstrstm	<< GET_STRING( STR_ID_3868 );					
#endif	PRIVATE_BANK
					//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해
				}

			}

#ifdef SERV_EVENT_BUY_FAKE_ITEM
			if( wstrstm.str().empty() )
				wstrstm << GET_STRING( STR_ID_23551 );
#endif //SERV_EVENT_BUY_FAKE_ITEM

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), g_pMain->GetNowState() );
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"아이템 구매 성공!", g_pMain->GetNowState() );

			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->Handler_EGS_CHECK_BALANCE_REQ();

			return Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );

		}
	}
	return false;
}

bool CX2CashShop::Handler_EGS_BILL_INVENTORY_INQUIRY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BILL_INVENTORY_INQUIRY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_BILL_INVENTORY_INQUIRY_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( GetOpen() == true )
			{
				if ( m_iReqDepositPageIndex == 1 || kEvent.m_vecOrderInfo.empty() == false )
				{
					vector< KBillOrderInfo > vecKBillOrderInfo;
					m_vecKBillOrderInfo = kEvent.m_vecOrderInfo;
#ifdef SERV_GLOBAL_CASH_PACKAGE
					m_vecKBillPackageInfo = kEvent.m_vecPackageInfo;			// 패키지는 분해되므로 삭제
#endif //SERV_GLOBAL_CASH_PACKAGE
					m_iNowDepositPageIndex = m_iReqDepositPageIndex;

					for ( int i = 0; i < (int)m_vecKBillOrderInfo.size(); i++ )
					{
						KBillOrderInfo& kKBillOrderInfo = m_vecKBillOrderInfo[i];
						vecKBillOrderInfo.push_back( kKBillOrderInfo );
					}
					ResetDepositItemSlot( CX2Slot::ST_CASH_SHOP_DEPOSIT, vecKBillOrderInfo );
				}
			}

#ifdef CASH_INVEN_PICKUP_ALL
			if( true == m_bPickUpAll )
			{
				PickUpAll();
			}
#endif //CASH_INVEN_PICKUP_ALL
		}
#ifdef CASH_INVEN_PICKUP_ALL
		else if( true == m_bPickUpAll )
		{
			m_bPickUpAll = false;
		}
#endif //CASH_INVEN_PICKUP_ALL
	}

	return false;
}
#else // SERV_GLOBAL_BILLING

#ifdef COUPON_SYSTEM
bool CX2CashShop::Handler_EGS_BUY_CASH_ITEM_REQ( vector< std::pair<unsigned long, int> > vecProductAndCouponNoList, bool bChoicedItemBuy /*= false*/ )
#else
bool CX2CashShop::Handler_EGS_BUY_CASH_ITEM_REQ( vector< unsigned long >& vecProductNoList, bool bChoicedItemBuy /* = false */ )
#endif // COUPON_SYSTEM
{
	if ( m_bSend_EGS_BUY_CASH_ITEM_REQ == true )
		return false;	

	KEGS_BUY_CASH_ITEM_REQ kPacket;
	kPacket.m_bIsWishList = bChoicedItemBuy;
	std::vector< KNXBTPurchaseReqInfo > vecKNXBTPurchaseReqInfo;

#ifdef COUPON_SYSTEM // 구매 시 상품 번호와 쿠폰 번호를 매칭시켜 패킷에 담기
	kPacket.m_bUseCoupon = false;

	// 상품번호 & 쿠폰 번호를 패킷에 같이 담기
	for ( UINT uiIndex = 0; uiIndex < vecProductAndCouponNoList.size(); ++uiIndex )
	{
		KNXBTPurchaseReqInfo kReqInfo;
		// 상품 번호
		kReqInfo.m_ulProductNo = vecProductAndCouponNoList[uiIndex].first;
		// 쿠폰 번호
		kReqInfo.m_iCouponCardNo = vecProductAndCouponNoList[uiIndex].second;
		if( 0 < kReqInfo.m_iCouponCardNo )
		{
			kPacket.m_bUseCoupon = true;
		}
		else
		{
			kReqInfo.m_iCouponCardNo = -1;
		}

		kReqInfo.m_usOrderQuantity = 1;		
		vecKNXBTPurchaseReqInfo.push_back( kReqInfo );
	}
#else
	for ( int i = 0; i < (int)vecProductNoList.size(); i++ )
	{
		unsigned long productNo = vecProductNoList[i];
		KNXBTPurchaseReqInfo kReqInfo;
		kReqInfo.m_ulProductNo = productNo;
		kReqInfo.m_usOrderQuantity = 1;
		vecKNXBTPurchaseReqInfo.push_back( kReqInfo );
	}
#endif // COUPON_SYSTEM

	if ( vecKNXBTPurchaseReqInfo.empty() == true )
		return true;

	kPacket.m_vecPurchaseReqInfo = vecKNXBTPurchaseReqInfo;

	//검사해보고 거시기 하면 //
	//if (  )
	vector<int> vecOverlapItemID;
	for ( int i = 0; i < (int)kPacket.m_vecPurchaseReqInfo.size(); i++ )
	{
		KNXBTPurchaseReqInfo kReqInfo = kPacket.m_vecPurchaseReqInfo[i];
		KNXBTProductInfo* pKNXBTProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kReqInfo.m_ulProductNo );
		if ( pKNXBTProductInfo != NULL )
		{
			int itemID = _wtoi( pKNXBTProductInfo->m_wstrProductID.c_str() );

			//{{ kimhc // 2009-08-11 // PLATINUM 은행 회원권 구매시 예외처리
#ifdef	PRIVATE_BANK
			#pragma region 은행 확장 캐시 구입 관련
			#ifndef GOOD_ELSWORD //JHKang
			if ( ( itemID	== PLATINUM_MEMBERSHIP_CARD || itemID == COBO_CREDIT_CARD ) 
				&& m_bCheckDialogWarningPlatinumMembership == false )
			{
				if ( g_pData->GetQuestManager() != NULL )
				{
					// 은행 퀘스트를 클리어 했는가?
					//if ( g_pData->GetQuestManager()->GetMapCompleteQuest().find( _CONST_UI_PRIVATE_BANK_::g_iBankQuestID )
					//	== g_pData->GetQuestManager()->GetMapCompleteQuest().end() )	// 퀘스트를 클리어 하지 않았으면
					if ( g_pData->GetQuestManager()->GetUnitCompleteQuest( _CONST_UI_PRIVATE_BANK_::g_iBankQuestID ) == false )
					{
						if ( itemID	== PLATINUM_MEMBERSHIP_CARD )
						{
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3869 ), CSCUM_BUY_PLATINUM_CARD_OK, 
								g_pMain->GetNowState(), CSCUM_BUY_PLATINUM_CARD_CANCEL );
						}
						else if ( itemID == COBO_CREDIT_CARD )
						{
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4353 ), CSCUM_BUY_PLATINUM_CARD_OK, 
								g_pMain->GetNowState(), CSCUM_BUY_PLATINUM_CARD_CANCEL );
						}

						return false;
					}
				}
			}
			#endif //GOOD_ELSWORD
			#pragma endregion 은행 퀘스트 처리
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-11 // PLATINUM 은행 회원권 구매시 예외처리
						
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
			if ( pItemTemplet != NULL )
			{
                switch( pItemTemplet->GetItemType() )
                {
                case CX2Item::IT_WEAPON:
                case CX2Item::IT_DEFENCE:
                case CX2Item::IT_ACCESSORY:
				    {
					    if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( itemID, true ) != NULL )
					    {
						    vecOverlapItemID.push_back( itemID );
					    }
				    }
                    break;
                }//switch
			}
		}
	}


	if ( vecOverlapItemID.empty() == false )
	{
        wstring wstrItemNames;
		for ( int i = 0; i < (int)vecOverlapItemID.size(); i++ )
		{
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( vecOverlapItemID[i] );
			if ( pItemTemplet != NULL )
			{
                wstrItemNames += pItemTemplet->GetName();
				if ( i != (int)vecOverlapItemID.size() - 1 )
				{
					wstrItemNames += L", ";
				}
			}

		}
		m_ReservedKEGS_BUY_CASH_ITEM_REQ = kPacket;
		g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_20, "L", wstrItemNames ) ), CSCUM_BUY_POP_UP_OVERLAP_OK, 
			g_pMain->GetNowState(), CSCUM_BUY_POP_UP_OVERLAP_CANCEL );
		return false;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_BUY_CASH_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BUY_CASH_ITEM_ACK );

	m_bSend_EGS_BUY_CASH_ITEM_REQ = true;

	return true;
}

bool CX2CashShop::Handler_EGS_BUY_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BUY_CASH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bSend_EGS_BUY_CASH_ITEM_REQ = false;

	if ( g_pMain->DeleteServerPacket( EGS_BUY_CASH_ITEM_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//kEvent.m_vecOrderInfo 주문한 정보..
			//if ( kEvent.m_vecOrderInfo.empty() == true )
			//{
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"아무것도 얻지 못했습니다.", g_pMain->GetNowState() );
			//}
			//else
			//{
			//wstringstream wstrstm;
			//for ( int i = 0; i < (int)kEvent.m_vecOrderInfo.size(); i++ )
			//{
			//KNXBTOrderInfo& kNXBTOrderInfo = kEvent.m_vecOrderInfo[i];
			//int itemID = _wtoi( kNXBTOrderInfo.m_wstrProductID.c_str() );
			//CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
			//if ( pItemTemplet != NULL )
			//{
			//wstrstm << L"[" << pItemTemplet->m_Name << L"]";
			//if ( i != (int)kEvent.m_vecOrderInfo.size() - 1 )
			//{
			//wstrstm << L", ";
			//}
			//}
			//}
			//wstrstm << L"아이템 구매 성공!";
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), g_pMain->GetNowState() );

			//CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			//pNowState->Handler_EGS_CHECK_BALANCE_REQ();
			//return Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
			//}

			if ( kEvent.m_bIsWishList == true )
			{
				vector< int > vecItemID;
				for ( int i = 0; i < (int)kEvent.m_vecPurchaseReqInfo.size(); i++ )
				{
					KNXBTPurchaseReqInfo& kKNXBTPurchaseReqInfo = kEvent.m_vecPurchaseReqInfo[i];
					KNXBTProductInfo* pKNXBTProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kKNXBTPurchaseReqInfo.m_ulProductNo );
					if ( pKNXBTProductInfo != NULL )
					{
						vecItemID.push_back( _wtoi( pKNXBTProductInfo->m_wstrProductID.c_str() ) );
					}
				}
				Handler_EGS_MODIFY_WISH_LIST_REQ( false, vecItemID );	
			}			

			bool bCheckBuyResurrectionStone		= false;
			bool bCheckBuyInvenSlotAdd			= false;
			//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해
#ifdef	PRIVATE_BANK
			bool bCheckBuyBankMembershipItem	= false;
#endif	PRIVATE_BANK
			//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해
#ifdef SERV_SKILL_NOTE
			bool bSkillNoteItem					= false; // 기술의 노트 아이템인지 판단
#endif

			int rebirthStoneNum = 0;

			for ( int i = 0; i < (int)kEvent.m_vecPurchaseReqInfo.size(); i++ )
			{
				KNXBTPurchaseReqInfo kReqInfo = kEvent.m_vecPurchaseReqInfo[i];
				KNXBTProductInfo* pKNXBTProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kReqInfo.m_ulProductNo );
				if ( pKNXBTProductInfo != NULL )
				{
					int itemID = _wtoi( pKNXBTProductInfo->m_wstrProductID.c_str() );
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
					if ( pItemTemplet != NULL )
					{
                        switch( pItemTemplet->GetItemType() )
                        {
                        case CX2Item::IT_WEAPON:
                        case CX2Item::IT_DEFENCE:
                        case CX2Item::IT_ACCESSORY:
						    {
							    if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( itemID, true ) != NULL )
							    {

								    g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_21 ), g_pMain->GetNowState() );
							    }
						    }
                            break;
                        }
					}
//{{오현빈 // 2012-03-14 // 구매한 아이템의 특이 정보를 알려주는 팝업창 추가
					switch( itemID )
					{
#ifdef SERV_GUILD_CHANGE_NAME
					case CHANGE_GUILD_NAME_ITEM_ID:
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_16229 ), g_pMain->GetNowState() );
						}
						break;
#endif //CHANGE_GUILD_NAME
#ifdef CHANGE_PET_NAME
					case CHANGE_PET_NAME_ITEM_ID:
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_16218 ), g_pMain->GetNowState() );
						}
						break;
#endif //CHANGE_PET_NAME
					default:
						break;
					}
//}}오현빈 // 2012-03-14 // 구매한 아이템의 특이 정보를 알려주는 팝업창 추가
				}
			}

			wstringstream wstrstm;
			for ( int i = 0;  i < (int)kEvent.m_vecPurchaseReqInfo.size(); i++ )
			{
				KNXBTPurchaseReqInfo& kKNXBTPurchaseReqInfo = kEvent.m_vecPurchaseReqInfo[i];
				KNXBTProductInfo* pKNXBTProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kKNXBTPurchaseReqInfo.m_ulProductNo );

				if ( pKNXBTProductInfo != NULL )
				{

					CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( _wtoi(pKNXBTProductInfo->m_wstrProductID.c_str()) );
                    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( _wtoi(pKNXBTProductInfo->m_wstrProductID.c_str()) );
					if ( pItemTemplet != NULL && pCashItem != NULL )
					{
                        DWORD   dwItemID = pItemTemplet->GetItemID();
						//127030부활석
                        if ( 127030 == dwItemID )
						{
							bCheckBuyResurrectionStone = true;
							rebirthStoneNum = pKNXBTProductInfo->m_usProductPieces;
						}

						//200750 소지품 8칸 확장
						if ( INVENTORY_SLOT_ADD_ITEM == dwItemID )
						{
							bCheckBuyInvenSlotAdd = true;
						}

			//{{ kimhc // 2009-08-11 // 은행 회원권 구매시 다이얼로그 처리 다르게 하기위해
#ifdef	PRIVATE_BANK
						switch ( dwItemID )
						{
						case SILVER_MEMBERSHIP_CARD:
						case GOLD_MEMBERSHIP_CARD:
						case EMERALD_MEMBERSHIP_CARD:
						case DIAMOND_MEMBERSHIP_CARD:
						case PLATINUM_MEMBERSHIP_CARD:
						case COBO_CREDIT_CARD:
							bCheckBuyBankMembershipItem		= true;
							break;

						default:
							break;
						}
#endif	PRIVATE_BANK
			//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해

#ifdef SERV_SKILL_NOTE
						if( dwItemID == CASH_SKILL_NOTE_ITEM_ID )
							bSkillNoteItem = true;
#endif

						wstrstm 
                            << pItemTemplet->GetName()
                            << L"(" << pCashItem->GetPeriod( *pKNXBTProductInfo ) << L")";
						if ( i != (int)kEvent.m_vecPurchaseReqInfo.size() - 1 )
						{
							wstrstm << L", ";
						}
					}

				}

				if ( i == (int)kEvent.m_vecPurchaseReqInfo.size() - 1 )
				{
					wstrstm << GET_STRING( STR_ID_22 ) << L"\n";
					if ( bCheckBuyResurrectionStone == true )
						wstrstm << GET_REPLACED_STRING( ( STR_ID_23, "i", g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() + rebirthStoneNum ) ) << L"\n";
#ifndef SERV_REFORM_INVENTORY_TEST
					if ( bCheckBuyInvenSlotAdd == true )
						wstrstm << GET_REPLACED_STRING( ( STR_ID_25, "i", g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum(CX2Inventory::ST_EQUIP) + 8 ) );
#endif SERV_REFORM_INVENTORY_TEST
					//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해
#ifdef	PRIVATE_BANK
					if ( bCheckBuyBankMembershipItem	== true )
						wstrstm	<< GET_STRING( STR_ID_3868 );
#endif	PRIVATE_BANK
					//{{ kimhc // 2009-08-11 // 은행 회원권 구매지 다이얼로그 처리 다르게 하기위해


#ifdef SERV_SKILL_NOTE
					if( bSkillNoteItem == true )
					{
						if( bCheckBuyInvenSlotAdd == true 
#ifdef PRIVATE_BANK
							|| bCheckBuyBankMembershipItem == true 
#endif
						)
						{
							wstrstm << L"\n";
						}

						wstrstm	<< GET_STRING( STR_ID_5059 );
					}
#endif
				}
			}

#ifdef SERV_EVENT_BUY_FAKE_ITEM
			if( wstrstm.str().empty() )
				wstrstm << GET_STRING( STR_ID_23551 );
#endif //SERV_EVENT_BUY_FAKE_ITEM

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), g_pMain->GetNowState() );
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"아이템 구매 성공!", g_pMain->GetNowState() );

			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->Handler_EGS_CHECK_BALANCE_REQ();

			return Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );

		}
		//{{ 2010. 03. 24  최육사	한도초과
		else if( kEvent.m_iOK == NetError::ERR_BUY_CASH_ITEM_42 )
		{
			CTime tCurTime = CTime::GetCurrentTime();
			int iNextMonth = tCurTime.GetMonth() + 1;
			if( iNextMonth == 13 )
				iNextMonth = 1;

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4947, "ii", tCurTime.GetMonth(), iNextMonth ) ), m_pNowState );            
		}
		//}}
	}

	return false;
}


bool CX2CashShop::Handler_EGS_PURCHASED_CASH_ITEM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PURCHASED_CASH_ITEM_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_PURCHASED_CASH_ITEM_LIST_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( GetOpen() == true )
			{
				if ( m_iReqDepositPageIndex == 1 || kEvent.m_vecOrderInfo.empty() == false )
				{

					vector< KNXBTOrderInfo > vecKNXBTOrderInfo;
					m_vecKNXBTOrderInfo = kEvent.m_vecOrderInfo;
					m_vecKNXBTPackageInfo = kEvent.m_vecPackageInfo;
					m_iNowDepositPageIndex = m_iReqDepositPageIndex;

					for ( int i = 0; i < (int)m_vecKNXBTOrderInfo.size(); i++ )
					{
						KNXBTOrderInfo& kKNXBTOrderInfo = m_vecKNXBTOrderInfo[i];
						vecKNXBTOrderInfo.push_back( kKNXBTOrderInfo );
					}
					ResetDepositItemSlot( CX2Slot::ST_CASH_SHOP_DEPOSIT, vecKNXBTOrderInfo );
				}
			}
		}
	}

	return false;
}
#endif // SERV_GLOBAL_BILLING

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( KBillOrderInfo& kKBillOrderInfo, map< int, int >& mapOption, const map<int, int>& mapGroupdIdList_ )
#else //SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( KBillOrderInfo& kKBillOrderInfo, map< int, int >& mapOption )
#endif //SERV_CASH_ITEM_SOCKET_OPTION
{
	m_TempKEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ.m_iTransNo = kKBillOrderInfo.m_iTransNo;
	m_TempKEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ.m_iProductNo = kKBillOrderInfo.m_kBillProductInfo.m_iProductNo;
	m_TempKEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ.m_mapSocketForCash = mapOption;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	m_TempKEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ.m_byteProductKind = kKBillOrderInfo.m_byteProductKind;
#endif //SERV_GLOBAL_CASH_PACKAGE

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
#ifdef SERV_GLOBAL_BILLING
	m_TempKEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ.m_mapSocketGroupIDForCash = mapGroupdIdList_;
#else //SERV_GLOBAL_BILLING
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_mapSocketGroupIDForCash = mapGroupdIdList_;
#endif //SERV_GLOBAL_BILLING
#endif	// SERV_CASH_ITEM_SOCKET_OPTION

	//기간제 아이템이라면 ... 중복되는지 체크한 후 가져올것인지 경고 메시지 옵션도 보여줘야 하나 이런 ㅋㅋ

	bool bCheck = false;

	//KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kKBillOrderInfo.m_iProductNo );
	//if ( pKBillProductInfo != NULL && (int)pKBillProductInfo->m_cPeriod > 0 )
	if ( (int)kKBillOrderInfo.m_kBillProductInfo.m_cPeriod > 0 )
	{
		//int itemID = pKBillProductInfo->m_iProductID;
		//if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID ) > 0 )
		if ( g_pData->GetMyUser()->GetSelectUnit() != NULL
			&& g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( kKBillOrderInfo.m_kBillProductInfo.m_iProductID ) > 0 )
		{
			//경고문 띄워주자.
			m_pDLGOverlapCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_26 ), CSCUM_ITEM_TO_INVEN_OVERLAP_CHECK_OK, g_pMain->GetNowState() );
			bCheck = true;
		}
	}

	if ( bCheck == false )
	{
		Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ();
	}
	return true;
}
bool CX2CashShop::Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ()
{
	KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ kPacket = m_TempKEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ;

	g_pData->GetServerProtocol()->SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK );

	return true;
}
bool CX2CashShop::Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BILL_GET_PURCHASED_CASH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	
#ifdef	SERV_EXPAND_QUICK_SLOT
	bool bExpandQuickSlot = false;
#endif  SERV_EXPAND_QUICK_SLOT

	g_pMain->DeleteServerPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK );

	if ( g_pData == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return false;

	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//need a process 
			Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			bool		bUpdateCashShopState	= false;
			CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
			map< int, int >::iterator mit;
			for ( mit = kEvent.m_mapExpandedCategorySlot.begin(); mit != kEvent.m_mapExpandedCategorySlot.end(); mit++ )
			{
				//if ( pInventory != NULL )
				{
					kInventory.SetItemMaxNum( (CX2Inventory::SORT_TYPE)mit->first, 
						kInventory.GetItemMaxNum((CX2Inventory::SORT_TYPE)mit->first) + mit->second );

					//{{ kimhc // 2009-08-17 // 은행 확장권을 구입했을 때만 업데이트
#ifdef	PRIVATE_BANK
					if ( static_cast< CX2Inventory::SORT_TYPE >( mit->first ) == CX2Inventory::ST_BANK )
						bUpdateCashShopState = true;
#endif	PRIVATE_BANK
					//{{ kimhc // 2009-08-17 // 은행 확장권을 구입했을 때만 업데이트
#ifdef	SERV_EXPAND_QUICK_SLOT
					if (	static_cast< CX2Inventory::SORT_TYPE >( mit->first ) == CX2Inventory::ST_E_QUICK_SLOT )
					{
						if( NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
						{
							g_pData->GetUIManager()->GetUIQuickSlot()->SetExpandQuickSlot( kInventory.GetItemMaxNum( CX2Inventory::ST_E_QUICK_SLOT ) );	// 맥스 슬롯 올리고

							bExpandQuickSlot = true;

#ifdef KEY_MAPPING
							g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAMEACTION_QUICKSLOT4, DIK_4 );
							g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAMEACTION_QUICKSLOT5, DIK_5 );
							g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAMEACTION_QUICKSLOT6, DIK_6 );

							if( g_pKTDXApp->GetDIManager()->GetJoystic() != NULL )
							{
#ifdef SERV_KEY_MAPPING_INT
								g_pKTDXApp->GetDIManager()->GetJoystic()->SetAction( GAMEACTION_QUICKSLOT4, JB_BUTTON14 );
								g_pKTDXApp->GetDIManager()->GetJoystic()->SetAction( GAMEACTION_QUICKSLOT5,	JB_BUTTON15 );
								g_pKTDXApp->GetDIManager()->GetJoystic()->SetAction( GAMEACTION_QUICKSLOT6,	JB_BUTTON16 );
#else //SERV_KEY_MAPPING_INT
								g_pKTDXApp->GetDIManager()->GetJoystic()->SetAction( GAMEACTION_QUICKSLOT4, DIJOFS_BUTTON14 );
								g_pKTDXApp->GetDIManager()->GetJoystic()->SetAction( GAMEACTION_QUICKSLOT5,	DIJOFS_BUTTON15 );
								g_pKTDXApp->GetDIManager()->GetJoystic()->SetAction( GAMEACTION_QUICKSLOT6,	DIJOFS_BUTTON16 );
#endif //SERV_KEY_MAPPING_INT
							}
#endif KEY_MAPPING
						}
					}
#endif  SERV_EXPAND_QUICK_SLOT	
				}
			}

#ifdef SERV_GUILD_CHANGE_NAME
			if ( kEvent.m_iExceptionProcessItemID == CHANGE_GUILD_NAME_ITEM_ID )
			{
				g_pData->GetGuildManager()->GetUIGuild()->SetShowChangeGuildNameDlg(true);
				//아이템 수령 팝업 띄우기 전에 리턴
				return true;
			}
#endif //CHANGE_GUILD_NAME


			if ( g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() == kEvent.m_iNumResurrectionStone &&
				kEvent.m_mapExpandedCategorySlot.empty() == true &&
				kEvent.m_vecInventorySlotInfo.empty() == false )
			{
#ifdef CASH_INVEN_PICKUP_ALL
				if( false == m_bPickUpAll )
#endif
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_109 ), (CKTDXStage*) m_pNowState );
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2684 ), (CKTDXStage*) m_pNowState );
			}

			g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( kEvent.m_iNumResurrectionStone );			

			//{{ kimhc // 2009-08-17 // 은행 확장권을 구입했을 때만 업데이트
#ifdef	PRIVATE_BANK
			if ( GetOpen() == true && bUpdateCashShopState == true )
			{
				SetCashShopState( m_CashShopState, m_CashShopCategory );					

				//{{  [1/6/2010 sulzi]	퀘스트 삭제
				if( NULL != g_pData->GetQuestManager() )
				{
					g_pData->GetQuestManager()->GiveUpForbiddenQuest(); 
				}
				//}}  [1/6/2010 sulzi]
			}
#endif	PRIVATE_BANK
			//{{ kimhc // 2009-08-17 // 은행 확장권을 구입했을 때만 업데이트
#ifdef	SERV_EXPAND_QUICK_SLOT
			else if ( GetOpen() == true && bExpandQuickSlot == true )
			{
				SetCashShopState( m_CashShopState, m_CashShopCategory );		
			}
#endif  SERV_EXPAND_QUICK_SLOT

			return true;
		}
#ifdef CASH_INVEN_PICKUP_ALL
		else
		{
			// 실패 패킷이 오면 PickUpAll 멈춘다.
			m_bPickUpAll = false;
		}
#endif CASH_INVEN_PICKUP_ALL
	}


	return false;
}

#else // SERV_GLOBAL_BILLING

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::Handler_EGS_GET_PURCHASED_CASH_ITEM_REP( const KNXBTOrderInfo& kKNXBTOrderInfo_, const map< int, int >& mapOption_, const map<int, int>& mapGroupdIdList_ )
#else	SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::Handler_EGS_GET_PURCHASED_CASH_ITEM_REP( const KNXBTOrderInfo& kKNXBTOrderInfo_, const map< int, int >& mapOption_ )
#endif	// SERV_CASH_ITEM_SOCKET_OPTION
{
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_ulOrderNo = kKNXBTOrderInfo_.m_ulOrderNo;
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_ulProductNo = kKNXBTOrderInfo_.m_ulProductNo;
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_iQuantity = kKNXBTOrderInfo_.m_usRemainOrderQuantity;
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_mapSocketForCash = mapOption_;

	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_wstrProductID = kKNXBTOrderInfo_.m_wstrProductID;
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_usProductExpire = kKNXBTOrderInfo_.GetProductExpire();
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_usProductPieces = kKNXBTOrderInfo_.m_usProductPieces;
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_byteProductKind = kKNXBTOrderInfo_.m_byteProductKind;

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
	m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ.m_mapSocketGroupIDForCash = mapGroupdIdList_;
#endif	// SERV_CASH_ITEM_SOCKET_OPTION

	//기간제 아이템이라면 ... 중복되는지 체크한 후 가져올것인지 경고 메시지 옵션도 보여줘야 하나 이런 ㅋㅋ

	bool bCheck = false;

	KNXBTProductInfo* pKNXBTProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kKNXBTOrderInfo_.m_ulProductNo );
	if ( pKNXBTProductInfo != NULL && pKNXBTProductInfo->GetProductExpire() > 0 )
	{
		int itemID = _wtoi( pKNXBTProductInfo->m_wstrProductID.c_str() );
		if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID ) > 0 )
		{
			//경고문 띄워주자.
			m_pDLGOverlapCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_26 ), CSCUM_ITEM_TO_INVEN_OVERLAP_CHECK_OK, g_pMain->GetNowState() );
			bCheck = true;
		}
	}

	if ( bCheck == false )
	{
		Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ();
	}

	return true;
}

bool CX2CashShop::Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ()
{
	KEGS_GET_PURCHASED_CASH_ITEM_REQ kPacket = m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_PURCHASED_CASH_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK );

	return true;
}

bool CX2CashShop::Handler_EGS_GET_PURCHASED_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_PURCHASED_CASH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	
#ifdef	SERV_EXPAND_QUICK_SLOT
	bool bExpandQuickSlot = false;
#endif  SERV_EXPAND_QUICK_SLOT
	g_pMain->DeleteServerPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//need a process 
			Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
			CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
			kInventory.UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			bool		bUpdateCashShopState	= false;

			map< int, int >::iterator mit;
			for ( mit = kEvent.m_mapExpandedCategorySlot.begin(); mit != kEvent.m_mapExpandedCategorySlot.end(); mit++ )
			{
				{
					kInventory.SetItemMaxNum( (CX2Inventory::SORT_TYPE)mit->first, 
						kInventory.GetItemMaxNum((CX2Inventory::SORT_TYPE)mit->first) + mit->second );

					//{{ kimhc // 2009-08-17 // 은행 확장권을 구입했을 때만 업데이트
#ifdef	PRIVATE_BANK
					if ( static_cast< CX2Inventory::SORT_TYPE >( mit->first ) == CX2Inventory::ST_BANK )
						bUpdateCashShopState = true;
#endif	PRIVATE_BANK
					//{{ kimhc // 2009-08-17 // 은행 확장권을 구입했을 때만 업데이트
					
#ifdef	SERV_EXPAND_QUICK_SLOT
					if (	static_cast< CX2Inventory::SORT_TYPE >( mit->first ) == CX2Inventory::ST_E_QUICK_SLOT )
					{
						if( NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
						{
							g_pData->GetUIManager()->GetUIQuickSlot()->SetExpandQuickSlot( kInventory.GetItemMaxNum( CX2Inventory::ST_E_QUICK_SLOT ) );	// 맥스 슬롯 올리고

							bExpandQuickSlot = true;
						}
					}
#endif  SERV_EXPAND_QUICK_SLOT					
				}
			}
#ifdef SERV_GUILD_CHANGE_NAME
			if ( kEvent.m_iExceptionProcessItemID == CHANGE_GUILD_NAME_ITEM_ID )
			{
				g_pData->GetGuildManager()->GetUIGuild()->SetShowChangeGuildNameDlg(true);
				//아이템 수령 팝업 띄우기 전에 리턴
				return true;
			}
#endif //CHANGE_GUILD_NAME
#ifdef SERV_PSHOP_AGENCY
			if( true == m_bUsePShopAgencyItem )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_12254 ), (CKTDXStage*) m_pNowState );
			}
			else
#endif SERV_PSHOP_AGENCY
			if ( g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() == kEvent.m_iNumResurrectionStone &&
				kEvent.m_mapExpandedCategorySlot.empty() == true &&
				kEvent.m_vecInventorySlotInfo.empty() == false )
			{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_109 ), (CKTDXStage*) m_pNowState );
			}
			else
			{
#ifdef SERV_EVENT_DAILY_GIFT_BOX		//매일매일 선물상자 전용 팝업
				if( false == SetDialyGiftBoxPopup( kEvent.m_iExceptionProcessItemID ) )
#endif SERV_EVENT_DAILY_GIFT_BOX
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2684 ), (CKTDXStage*) m_pNowState );
			}
			
			g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( kEvent.m_iNumResurrectionStone );			

			//{{ kimhc // 2009-08-17 // 은행 확장권을 구입했을 때만 업데이트
#ifdef	PRIVATE_BANK
			if ( GetOpen() == true && bUpdateCashShopState == true )
				SetCashShopState( m_CashShopState, m_CashShopCategory );					
#endif	PRIVATE_BANK
			//{{ kimhc // 2009-08-17 // 은행 확장권을 구입했을 때만 업데이트
#ifdef	SERV_EXPAND_QUICK_SLOT
			else if ( GetOpen() == true && bExpandQuickSlot == true )
			{
				SetCashShopState( m_CashShopState, m_CashShopCategory );		
			}
#endif  SERV_EXPAND_QUICK_SLOT
			return true;
		}
	}

	return false;
}
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_GLOBAL_CASH_PACKAGE
#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( KBillOrderInfo& kKBillOrderInfo, map< int, int >& mapOption, const map<int, int>& mapGroupdIdList_, vector< KBillPackageInfo >& vecPackageInfo_ )
#else	//SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( KBillOrderInfo& kKBillOrderInfo, map< int, int >& mapOption, vector< KBillPackageInfo >& vecPackageInfo_ )
#endif	//SERV_CASH_ITEM_SOCKET_OPTION
#else // SERV_GLOBAL_CASH_PACKAGE
#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( const KNXBTOrderInfo& kKNXBTOrderInfo_, const map< int, int >& mapOption_, const map<int, int>& mapGroupdIdList_, const vector< KNXBTPackageInfo >& vecPackageInfo_ )
#else	SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( const KNXBTOrderInfo& kKNXBTOrderInfo_, const map< int, int >& mapOption_, const vector< KNXBTPackageInfo >& vecPackageInfo_ )
#endif	// SERV_CASH_ITEM_SOCKET_OPTION
#endif // SERV_GLOBAL_CASH_PACKAGE
{
#ifdef SERV_GLOBAL_CASH_PACKAGE
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_ulOrderNo = kKBillOrderInfo.m_iTransNo;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_ulPackageProductNo = kKBillOrderInfo.m_kBillProductInfo.m_iProductNo;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_byteProductKind = kKBillOrderInfo.m_byteProductKind;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_wstrPackageProductID = kKBillOrderInfo.m_kBillProductInfo.m_wstrProductName;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_mapSocketForCash = mapOption;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_iProductID = kKBillOrderInfo.m_kBillProductInfo.m_iProductID;
#else // SERV_GLOBAL_CASH_PACKAGE
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_ulOrderNo = kKNXBTOrderInfo_.m_ulOrderNo;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_ulPackageProductNo = kKNXBTOrderInfo_.m_ulProductNo;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_byteProductKind = kKNXBTOrderInfo_.m_byteProductKind;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_wstrPackageProductID = kKNXBTOrderInfo_.m_wstrProductID;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_mapSocketForCash = mapOption_;
#endif // SERV_GLOBAL_CASH_PACKAGE

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_mapSocketGroupIDForCash = mapGroupdIdList_;
#endif	// SERV_CASH_ITEM_SOCKET_OPTION

	//각 개별 상품 벡터로 거시기 해불자.
	/*
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_ulProductNo = kKNXBTOrderInfo.m_ulProductNo;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_wstrProductID = kKNXBTOrderInfo.m_wstrProductID;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_usProductExpire = kKNXBTOrderInfo.m_usProductExpire;
	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_usProductPieces = kKNXBTOrderInfo.m_usProductPieces;
	*/

	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_vecPickUpPackageInfo.clear();
	for ( int i = 0; i < (int)vecPackageInfo_.size(); i++ )
	{
#ifdef SERV_GLOBAL_CASH_PACKAGE
		KBillPackageInfo& kKNXBTPackageInfo = vecPackageInfo_[i];
#else // SERV_GLOBAL_CASH_PACKAGE
		const KNXBTPackageInfo& kKNXBTPackageInfo = vecPackageInfo_[i];
#endif // SERV_GLOBAL_CASH_PACKAGE
		KNXBTPickUpPackageInfo kKNXBTPickUpPackageInfo;


		kKNXBTPickUpPackageInfo.m_ulProductNo = kKNXBTPackageInfo.m_ulProductNo;
		kKNXBTPickUpPackageInfo.m_wstrProductID = kKNXBTPackageInfo.m_wstrProductID;
		kKNXBTPickUpPackageInfo.m_sProductExpire = kKNXBTPackageInfo.GetProductExpire();
		kKNXBTPickUpPackageInfo.m_usProductPieces = kKNXBTPackageInfo.m_usProductPieces;
#ifdef SERV_GLOBAL_CASH_PACKAGE
		kKNXBTPickUpPackageInfo.m_iSubProductNo = kKNXBTPackageInfo.m_iProductID;
#endif // SERV_GLOBAL_CASH_PACKAGE


		m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ.m_vecPickUpPackageInfo.push_back( kKNXBTPickUpPackageInfo );
	}


	//기간제 아이템이라면 ... 중복되는지 체크한 후 가져올것인지 경고 메시지 옵션도 보여줘야 하나 이런 ㅋㅋ

	bool bCheck = false;

	for ( int i = 0; i < (int)vecPackageInfo_.size(); i++ )
	{
#ifdef SERV_GLOBAL_CASH_PACKAGE
		const KBillPackageInfo& kKNXBTPackageInfo = vecPackageInfo_[i];
#else // SERV_GLOBAL_CASH_PACKAGE
		const KNXBTPackageInfo& kKNXBTPackageInfo = vecPackageInfo_[i];
#endif // SERV_GLOBAL_CASH_PACKAGE
		
		if ( kKNXBTPackageInfo.GetProductExpire() > 0 )
		{
			int itemID = _wtoi( kKNXBTPackageInfo.m_wstrProductID.c_str() );
			if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID ) > 0 )
			{
				//경고문 띄워주자.
				m_pDLGOverlapCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_26 ), CSCUM_ITEM_TO_INVEN_OVERLAP_CHECK_OK_PACKAGE, g_pMain->GetNowState() );
				bCheck = true;
				break;
			}
		}
	}

	if ( bCheck == false )
	{
		Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ();
	}

	return true;
}

bool CX2CashShop::Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ()
{
	KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ kPacket = m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK );

	return true;
}

bool CX2CashShop::Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//need a process 
#ifdef SERV_GLOBAL_CASH_PACKAGE
			Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#else // SERV_GLOBAL_CASH_PACKAGE
			Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#endif // SERV_GLOBAL_CASH_PACKAGE
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
			kInventory.UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			map< int, int >::iterator mit;
			for ( mit = kEvent.m_mapExpandedCategorySlot.begin(); mit != kEvent.m_mapExpandedCategorySlot.end(); mit++ )
			{
				{
					kInventory.SetItemMaxNum( (CX2Inventory::SORT_TYPE)mit->first, 
						kInventory.GetItemMaxNum((CX2Inventory::SORT_TYPE)mit->first) + mit->second );
				}
			}


			if ( g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() == kEvent.m_iNumResurrectionStone &&
				kEvent.m_mapExpandedCategorySlot.empty() == true 
#ifdef SERV_GLOBAL_CASH_PACKAGE
				&& kEvent.m_vecInventorySlotInfo.empty() == false
#endif //SERV_GLOBAL_CASH_PACKAGE
				)
			{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_109 ), (CKTDXStage*) m_pNowState );
			}
#ifdef SERV_GLOBAL_CASH_PACKAGE
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2684 ), (CKTDXStage*) m_pNowState );
			}
#endif //SERV_GLOBAL_CASH_PACKAGE
			g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( kEvent.m_iNumResurrectionStone );			

			UpdateCashUI();
			return true;
		}
	}

	return false;
}


bool CX2CashShop::Handler_EGS_GET_WISH_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_WISH_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_GET_WISH_LIST_ACK ) == true )
	{
		//if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_vecChoicedItemID.resize(0);

			//vector< int > vecRemoveItemID;

			map<int,int>::iterator mit;
			for ( mit = kEvent.m_mapWishList.begin(); mit != kEvent.m_mapWishList.end(); mit++ )
			{	
#ifdef SERV_WISH_LIST_BUG_FIX
				if ( mit->second <= 0 )			// 아이템 ID가 0이거나 음수이면 무시
				{
					continue;
				}
#endif SERV_WISH_LIST_BUG_FIX
				if ( std::find( m_vecChoicedItemID.begin(), m_vecChoicedItemID.end(), mit->second ) != m_vecChoicedItemID.end() )
				{					
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_32 ), g_pMain->GetNowState() );
					continue;
				}

				if ( (int)m_vecChoicedItemID.size() >= CASH_SHOP_CHOICED_SLOT_NUM )
				{					
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_33 ), g_pMain->GetNowState() );
					continue;
				}

				/*
				if ( g_pData->GetItemManager()->GetCashItem( mit->second ) == NULL )
				{
					vecRemoveItemID.push_back( mit->second );
				}
				*/


					m_vecChoicedItemID.push_back( mit->second );
			}

			if ( m_bOpen == true )
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_CHOICED, m_vecChoicedItemID );
			}

			//if ( vecRemoveItemID.empty() == false )
			//	Handler_EGS_MODIFY_WISH_LIST_REQ( false, vecRemoveItemID );


			return true;
		}
	}

	return false;
}

bool CX2CashShop::Handler_EGS_MODIFY_WISH_LIST_REQ( bool bAdd, vector< int >& vecItemID )
{
	KEGS_MODIFY_WISH_LIST_REQ kPacket;
	kPacket.m_bIsDelete = !bAdd;
	kPacket.m_vecWishListItemID = vecItemID;

	g_pData->GetServerProtocol()->SendPacket( EGS_MODIFY_WISH_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_MODIFY_WISH_LIST_ACK );

	return true;

}

bool CX2CashShop::Handler_EGS_MODIFY_WISH_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MODIFY_WISH_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_MODIFY_WISH_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( kEvent.m_bIsDelete == true )
			{
				for ( int i = 0;  i < (int)kEvent.m_vecWishListItemID.size(); i++ )
				{

					if ( std::find( m_vecChoicedItemID.begin(), m_vecChoicedItemID.end(), kEvent.m_vecWishListItemID[i] ) != m_vecChoicedItemID.end() )
					{
						//일단 찾았고..
						m_vecChoicedItemID.erase( std::remove( m_vecChoicedItemID.begin(), m_vecChoicedItemID.end(), kEvent.m_vecWishListItemID[i] ), m_vecChoicedItemID.end() );

					}
				}


			}
			else
			{
				for ( int i = 0;  i < (int)kEvent.m_vecWishListItemID.size(); i++ )
				{
					if ( std::find( m_vecChoicedItemID.begin(), m_vecChoicedItemID.end(), kEvent.m_vecWishListItemID[i] ) != m_vecChoicedItemID.end() )
					{
						//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"이미 찜한 상품입니다!", g_pMain->GetNowState() );
						StateLog( L"이미 찜한 상품입니다!" );
						continue;
					}

					if ( (int)m_vecChoicedItemID.size() >= CASH_SHOP_CHOICED_SLOT_NUM )
					{
						//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"찜한 아이템 목록이 꽉찼습니다.", g_pMain->GetNowState() );
						StateLog( L"찜한 아이템 목록이 꽉찼습니다." );
						continue;
					}

					m_vecChoicedItemID.push_back( kEvent.m_vecWishListItemID[i] );
				}				
			}

			if ( m_bOpen == true )
			{
				ResetItemSlot( CX2Slot::ST_CASH_SHOP_CHOICED, m_vecChoicedItemID );
			}
			return true;
		}
	}

	return false;
}

bool CX2CashShop::Handler_EGS_PRESENT_CASH_ITEM_REQ( const WCHAR* wszReceiverNickName, const WCHAR* wszMessage,
													const WCHAR* wszPassword, unsigned long ulProductNo, bool bChoicedItem )
{
	KEGS_PRESENT_CASH_ITEM_REQ kPacket;
	kPacket.m_wstrReceiverNickName = wszReceiverNickName;
	kPacket.m_wstrMessage = wszMessage;
	kPacket.m_bIsWishList = bChoicedItem;
	kPacket.m_wstrPassword = wszPassword;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_iUseCashType = m_pPresentItemPopup->GetUseCashType();
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef CLIENT_COUNTRY_PH
	if ( m_pPresentItemPopup != NULL && m_pPresentItemPopup->RestPresentItemCashCheck(ulProductNo) == false )
	{
		m_pPresentItemPopup->SetOpen( false, 0 );

		
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStrF(NetError::ERR_BUY_CASH_ITEM_16) , g_pMain->GetNowState() );

		return false;
	}
#endif //CLIENT_COUNTRY_PH

	KNXBTPurchaseReqInfo kReqInfo;
	kReqInfo.m_ulProductNo = ulProductNo;
	kReqInfo.m_usOrderQuantity = 1;
	kPacket.m_vecPurchaseReqInfo.push_back( kReqInfo );

	g_pData->GetServerProtocol()->SendPacket( EGS_PRESENT_CASH_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PRESENT_CASH_ITEM_ACK );

	return true;
}

bool CX2CashShop::Handler_EGS_PRESENT_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PRESENT_CASH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_PRESENT_CASH_ITEM_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( kEvent.m_bIsWishList == true && kEvent.m_vecPurchaseReqInfo.empty() == false )
			{
				vector< int > vecItemID;
				for ( int i = 0; i < (int)kEvent.m_vecPurchaseReqInfo.size(); i++ )
				{
					KNXBTPurchaseReqInfo& kKNXBTPurchaseReqInfo = kEvent.m_vecPurchaseReqInfo[i];
#ifdef SERV_GLOBAL_BILLING
					KBillProductInfo* pKNXBTProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kKNXBTPurchaseReqInfo.m_ulProductNo );
#else // SERV_GLOBAL_BILLING
					KNXBTProductInfo* pKNXBTProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kKNXBTPurchaseReqInfo.m_ulProductNo );
#endif // SERV_GLOBAL_BILLING
					if ( pKNXBTProductInfo != NULL )
					{
#ifdef SERV_GLOBAL_BILLING
						vecItemID.push_back( pKNXBTProductInfo->m_iProductID );
#else // SERV_GLOBAL_BILLING
						vecItemID.push_back( _wtoi( pKNXBTProductInfo->m_wstrProductID.c_str() ) );
#endif // SERV_GLOBAL_BILLING
					}
				}
				Handler_EGS_MODIFY_WISH_LIST_REQ( false, vecItemID );
			}

			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->Handler_EGS_CHECK_BALANCE_REQ();

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_28 ), g_pMain->GetNowState() );
			return true;
		}
	}

	return false;
}

bool CX2CashShop::Handler_EGS_CHECK_PRESENT_CASH_INVENTORY_NOT( unsigned long ulOrderNo, unsigned long ulProductNo )
{
	KEGS_CHECK_PRESENT_CASH_INVENTORY_NOT kPacket;
	kPacket.m_ulOrderNo = ulOrderNo;
	kPacket.m_ulProductNo = ulProductNo;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHECK_PRESENT_CASH_INVENTORY_NOT, kPacket );
	return true;
}



bool CX2CashShop::Handler_EGS_EXPAND_CHAR_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXPAND_CHAR_SLOT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pData->GetMyUser()->AccessUserData().maxUnitCount = kEvent.m_iCharSlotSize;

	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_29, "i", kEvent.m_iCharSlotSize ) ), g_pMain->GetNowState() );

	return true;
}

#ifdef CASH_ITEM_REFUND

bool CX2CashShop::Handler_EGS_CASH_ITEM_REFUND_REQ()
{
	KEGS_CASH_ITEM_REFUND_REQ kPacket;

	if( static_cast<int>(m_RefundOrderNo) > 0 && static_cast<int>(m_RefundProductNo) > 0 && m_RefundQuantity > 0 )
	{
		kPacket.m_ulOrderNo		= m_RefundOrderNo;
		kPacket.m_ulProductNo	= m_RefundProductNo;
		kPacket.m_iQuantity		= m_RefundQuantity;
	}
	else
	{
		return false;
	}
	g_pData->GetServerProtocol()->SendPacket( EGS_CASH_ITEM_REFUND_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CASH_ITEM_REFUND_ACK );

	return true;
}

bool CX2CashShop::Handler_EGS_CASH_ITEM_REFUND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CASH_ITEM_REFUND_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_CASH_ITEM_REFUND_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->Handler_EGS_CHECK_BALANCE_REQ();
#ifdef SERV_GLOBAL_BILLING
			Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#else // SERV_GLOBAL_BILLING
			Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#endif // SERV_GLOBAL_BILLING
			return true;
		}
	}
	return false;
}
#endif CASH_ITEM_REFUND

bool CX2CashShop::MouseUp( D3DXVECTOR2 mousePos )
{
#ifdef CASH_ITEM_REFUND
	CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
	CX2Cursor*	pCursor = pState->GetCursor();
	if(pCursor->GetCurorState() == CX2Cursor::XCS_CASH_REFUND)
	{
		CX2SlotItem* pItemSlot = NULL;
		pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
		if ( pItemSlot == NULL )
			return false;

		if ( pItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_CASH_SHOP_DEPOSIT )
		{
			for(int i=0; i<ARRAY_SIZE(NON_REFUNDABLE_CASH_ITEM_ID); ++i)
			{
				if( pItemSlot->GetItemTID() == NON_REFUNDABLE_CASH_ITEM_ID[i] )
				{
					// STR_ID_9733
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING(STR_ID_9733), g_pMain->GetNowState() );
					return true;
				}
			}
#ifndef SERV_GLOBAL_BILLING
			m_RefundProductNo	= pItemSlot->GetKNXBTOrderInfo().m_ulProductNo;
			m_RefundOrderNo		= pItemSlot->GetKNXBTOrderInfo().m_ulOrderNo;
			m_RefundQuantity	= static_cast<int> (pItemSlot->GetKNXBTOrderInfo().m_usOrderQuantity);
#endif // ndef SERV_GLOBAL_BILLING

			// STR_ID_9734
			m_pDLGCheckCashItemRefund = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING(STR_ID_9734), CSCUM_CASH_ITEM_REFUND_OK, g_pMain->GetNowState() );
			return true;
		}
		return false;
	}
#endif CASH_ITEM_REFUND

	// 장비 아이템일 경우 처리 : 장착 해제
	CX2SlotItem* pItemSlot = NULL;
	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL )
		return false;

	if ( MouseLRButtonUpEquip( pItemSlot ) == true )
		return true;

	return false;
}

bool CX2CashShop::MouseRButtonUp( D3DXVECTOR2 mousePos )
{
	// 장비 아이템일 경우 처리 : 장착 해제
	CX2SlotItem* pItemSlot = NULL;
	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL )
		return false;

	if ( MouseLRButtonUpEquip( pItemSlot ) == true )
	{
		return true;
	}
	else if ( pItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_CASH_SHOP_CHOICED )
	{
		//RemoveChoicedItem( pItemSlot->GetItemTID() );
		//여기에다 이제 메뉴 버튼 띄워줘부러~
		m_ChoicedItemMenuItemID = pItemSlot->GetItemTID();
		if ( m_pDLGChoicedItemMenu == NULL )
		{
			m_pDLGChoicedItemMenu = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Choiced_Menu_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChoicedItemMenu );
		}

		m_pDLGChoicedItemMenu->SetShow( true );
		m_pDLGChoicedItemMenu->SetEnable( true );

#ifdef SERV_USE_ENABLE_GIFT
		//위시리스트에서 캐쉬아이템을 오른쪽 클릭하였을 때 선물 불가능한 아이템은 선물 버튼이 비활성화 됨.
		CKTDGUIControl* pGift = m_pDLGChoicedItemMenu->GetControl( L"Gift" );

		if( pGift != NULL 
#ifndef NO_PRESENT_BUTTON			
			&& IsPossiblePresent( pItemSlot->GetItemTID() ) == false 
#endif // NO_PRESENT_BUTTON			
			)
		{
			pGift->SetEnable( false );
		}
#endif SERV_USE_ENABLE_GIFT

#ifdef CLIENT_COUNTRY_PH
		if ( pGift != NULL && g_pData != NULL && g_pData->GetSelectUnitLevel() < 15 )
		{
			pGift->SetEnable( false );
		}
#endif //CLIENT_COUNTRY_PH

		D3DXVECTOR2 vMousePos;
		vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
		vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
		vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

		m_pDLGChoicedItemMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y - 40.f ) );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGChoicedItemMenu, true );
	}
	else if ( pItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_CASH_SHOP_DEPOSIT )
	{
#ifdef SERV_GLOBAL_BILLING

#ifdef SERV_GLOBAL_CASH_PACKAGE
		if ( pItemSlot->GetKBillOrderInfo().m_byteProductKind == 1 )
		{
			vector< int > vecItemID;
			vector< KBillOrderInfo > vecKBillTOrderInfo;

			for ( int i = 0; i < (int)m_vecKBillPackageInfo.size(); i++ )
			{	
				KBillPackageInfo& kKBillPackageInfo = m_vecKBillPackageInfo[i];
				if ( /*kKBillPackageInfo.m_ulProductNo == pItemSlot->GetKBillOrderInfo().m_kBillProductInfo.m_iProductNo &&*/
					kKBillPackageInfo.m_iPackageNo == pItemSlot->GetKBillOrderInfo().m_iTransNo )
				{
					//int tempItemID = _wtoi( kKBillPackageInfo.m_wstrProductID.c_str() );
					//int tempItemID = kKBillPackageInfo.m_ulProductNo;
					int tempItemID = kKBillPackageInfo.m_iProductID;

					//if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( tempItemID ) == true )
					{
						if ( std::find( vecItemID.begin(), vecItemID.end(), tempItemID ) == vecItemID.end() )
							vecItemID.push_back( tempItemID );
					}
				}
			}
			vecKBillTOrderInfo.push_back( pItemSlot->GetKBillOrderInfo() );
			m_pCashItemToInvenPopup->Reset( vecItemID, vecKBillTOrderInfo, m_vecKBillPackageInfo );
			m_pCashItemToInvenPopup->SetOpen( true );
		}
		else
		{
#endif //SERV_GLOBAL_CASH_PACKAGE
			const CX2Item::ItemTemplet* pItemTemplate = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
			if ( pItemTemplate != NULL )
			{
				if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( pItemTemplate->GetItemID() ) == true )
				{
					vector< int > vecItemID;
					vector< KBillOrderInfo > vecKBillOrderInfo;
					vecItemID.push_back( pItemTemplate->GetItemID() );
					vecKBillOrderInfo.push_back( pItemSlot->GetKBillOrderInfo() );
					m_pCashItemToInvenPopup->Reset( vecItemID, vecKBillOrderInfo, m_vecKBillPackageInfo );
					m_pCashItemToInvenPopup->SetOpen( true );
				}
				else
				{
					m_TempCashItemToInven = pItemSlot->GetKBillOrderInfo();
					//KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( m_TempCashItemToInven.m_iProductNo );
					//if( NULL != pKBillProductInfo )
					{
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_TempCashItemToInven.m_kBillProductInfo.m_iProductID );
						if ( pItemTemplet != NULL )
						{
#ifdef SERV_GLOBAL_CASH_PACKAGE
							if(m_TempCashItemToInven.m_byteProductKind != 1)
							{
								m_pDLGCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_30, "S", pItemTemplet->GetName() ) ), CSCUM_ITEM_TO_INVEN_CHECK_OK, g_pMain->GetNowState() );
								//팝업 띄워주자.
							}
							else
							{
								m_pDLGCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_30, "S", pItemTemplet->GetName() ) ), CSCUM_ITEM_TO_INVEN_OVERLAP_CHECK_OK_PACKAGE, g_pMain->GetNowState() );
								//팝업 띄워주자.
							}

#else //SERV_GLOBAL_CASH_PACKAGE
							m_pDLGCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_30, "S", pItemTemplet->GetName() ) ), CSCUM_ITEM_TO_INVEN_CHECK_OK, g_pMain->GetNowState() );
							//팝업 띄워주자.
#endif //SERV_GLOBAL_CASH_PACKAGE

						}
					}
				}

#ifdef SERV_GLOBAL_CASH_PACKAGE
			}
#endif //SERV_GLOBAL_CASH_PACKAGE
		}

#else // SERV_GLOBAL_BILLING
		if ( pItemSlot->GetKNXBTOrderInfo().m_byteProductKind == 1 )
		{
			vector< int > vecItemID;
			vector< KNXBTOrderInfo > vecKNXBTOrderInfo;

			for ( int i = 0; i < (int)m_vecKNXBTPackageInfo.size(); i++ )
			{	
				KNXBTPackageInfo& kKNXBTPackageInfo = m_vecKNXBTPackageInfo[i];
				if ( kKNXBTPackageInfo.m_ulPackageProductNo == pItemSlot->GetKNXBTOrderInfo().m_ulProductNo &&
					kKNXBTPackageInfo.m_ulOrderNo == pItemSlot->GetKNXBTOrderInfo().m_ulOrderNo )
				{
					int tempItemID = _wtoi( kKNXBTPackageInfo.m_wstrProductID.c_str() );

					//if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( tempItemID ) == true )
					{
						if ( std::find( vecItemID.begin(), vecItemID.end(), tempItemID ) == vecItemID.end() )
							vecItemID.push_back( tempItemID );
					}
				}
			}
			vecKNXBTOrderInfo.push_back( pItemSlot->GetKNXBTOrderInfo() );
			m_pCashItemToInvenPopup->Reset( vecItemID, vecKNXBTOrderInfo, m_vecKNXBTPackageInfo );
			m_pCashItemToInvenPopup->SetOpen( true );
		}
		else
		{
			const CX2Item::ItemTemplet* pItemTemplate = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
			if ( pItemTemplate != NULL )
			{
                DWORD   dwItemID = pItemTemplate->GetItemID();
#ifdef SERV_PSHOP_AGENCY
				m_bUsePShopAgencyItem = false;
				if( dwItemID == 233210 )
				{
					m_bUsePShopAgencyItem = true;
					m_TempCashItemToInven = pItemSlot->GetKNXBTOrderInfo();
					m_pDLGCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
                        GET_REPLACED_STRING( ( STR_ID_12253, "S", pItemTemplate->GetName() ) ),
                        CSCUM_ITEM_TO_INVEN_CHECK_OK, g_pMain->GetNowState() );
					return false;
				}
#endif SERV_PSHOP_AGENCY


				if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( dwItemID ) == true )
				{
					vector< int > vecItemID;
					vector< KNXBTOrderInfo > vecKNXBTOrderInfo;
					vecItemID.push_back( dwItemID );
					vecKNXBTOrderInfo.push_back( pItemSlot->GetKNXBTOrderInfo() );
					m_pCashItemToInvenPopup->Reset( vecItemID, vecKNXBTOrderInfo, m_vecKNXBTPackageInfo );
					m_pCashItemToInvenPopup->SetOpen( true );
				}
				else
				{
					m_TempCashItemToInven = pItemSlot->GetKNXBTOrderInfo();
                    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( _wtoi( m_TempCashItemToInven.m_wstrProductID.c_str() ) );
					if ( pItemTemplet != NULL )
					{

#ifdef CASH_ITEM_REFUND
                        switch( pItemTemplet->GetItemID() )
                        {
                        case 180035:
                        case 180044:
                        case 160145:
							m_pDLGCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_30, "S", pItemTemplet->GetName() ) ), CSCUM_ITEM_TO_INVEN_CHECK_OK, g_pMain->GetNowState() );
                            break;
                        default:
							m_pDLGCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_9797, "S", pItemTemplet->GetName() ) ), CSCUM_ITEM_TO_INVEN_CHECK_OK, g_pMain->GetNowState() );
						}
#else
						m_pDLGCheckCashItemToInven = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_30, "S", pItemTemplet->GetName() ) ), CSCUM_ITEM_TO_INVEN_CHECK_OK, g_pMain->GetNowState() );
						//팝업 띄워주자.
#endif CASH_ITEM_REFUND
					}
				}
			}

		}
#endif // SERV_GLOBAL_BILLING

	}

	return false;	
}

bool CX2CashShop::MouseLRButtonUpEquip( CX2SlotItem* pItemSlot )
{
	if ( pItemSlot == NULL )
		return false;

	if ( CheckIsPossibleEqiupSlot( pItemSlot->GetSlotType() ) == true )
	{
		bool bCheckPackageItem = false;
		int itemID = pItemSlot->GetItemTID();
		CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
		if ( pCashItem != NULL )
		{
#ifdef SERV_GLOBAL_BILLING
			if ( pCashItem->m_vecKBillProductInfo.empty() == false )
			{
				KBillProductInfo& kKBillProductInfo = pCashItem->m_vecKBillProductInfo[0];
				vector< CX2ItemManager::PackageItemData > vecPackageItemData;			

				//{{ 2013.01.26 최민철 큐브패키지아이템 미리보기기능
#ifdef PACKAGE_IN_QUBE_PREVIEW
				g_pData->GetItemManager()->GetPackageCubeItemData( kKBillProductInfo.m_iProductID, vecPackageItemData );
				if ( vecPackageItemData.empty() == true )
					g_pData->GetItemManager()->GetPackageData( kKBillProductInfo.m_iProductID, vecPackageItemData );
#else PACKAGE_IN_QUBE_PREVIEW
				g_pData->GetItemManager()->GetPackageData( kKBillProductInfo.m_iProductID, vecPackageItemData );
#endif PACKAGE_IN_QUBE_PREVIEW
				//}} 2013.01.26 최민철 큐브패키지아이템 미리보기기능

				if ( vecPackageItemData.empty() == false )
				{
					bCheckPackageItem = true;
					for ( int i = 0; i < (int)vecPackageItemData.size(); i++ )
					{
						InsertPreEquipItem(vecPackageItemData[i].m_iItemID, true );
						SetEnableBuyEquippingItem( false );
					}
				}
#ifdef ICE_HEATER_PRE_VIEW
				vector< int > vecIceHeaterItemData;
				CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE;
				if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType;
				}
				g_pData->GetItemManager()->GetIceHeaterItemData( _wtoi( kKBillProductInfo.m_wstrProductName.c_str()), eUnitType, vecIceHeaterItemData );

				BOOST_FOREACH( int iItemID, vecIceHeaterItemData )
				{
					InsertPreEquipItem( iItemID, true );
					SetEnableBuyEquippingItem( false );					
				}
#endif //ICE_HEATER_PRE_VIEW
			}
#else // SERV_GLOBAL_BILLING
			if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
			{
				KNXBTProductInfo& kKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[0];
				vector< CX2ItemManager::PackageItemData > vecPackageItemData;			
				g_pData->GetItemManager()->GetPackageData( _wtoi( kKNXBTProductInfo.m_wstrProductID.c_str() ), vecPackageItemData );

				if ( vecPackageItemData.empty() == false )
				{
					bCheckPackageItem = true;

					int iPreEquipCount = 0;
					for ( int i = 0; i < (int)vecPackageItemData.size(); i++ )
					{
						if( true == InsertPreEquipItem(vecPackageItemData[i].m_iItemID, true ) )
							++iPreEquipCount;

						SetEnableBuyEquippingItem( false );
					}

					// 1개 이상 미리보기 장착 가능하면, 패키지 아이템에 임시 착용중 표시 하기
					if( iPreEquipCount >= 1 )
						InsertPreEquipItem( itemID, true );
				}
#ifdef ICE_HEATER_PRE_VIEW
				vector< int > vecIceHeaterItemData;
				CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE;
				if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
				{
					eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType;
				}
				g_pData->GetItemManager()->GetIceHeaterItemData( _wtoi( kKNXBTProductInfo.m_wstrProductID.c_str()), eUnitType, vecIceHeaterItemData );

				BOOST_FOREACH( int iItemID, vecIceHeaterItemData )
				{
					InsertPreEquipItem( iItemID, true );
					SetEnableBuyEquippingItem( false );					
				}
#endif //ICE_HEATER_PRE_VIEW
			}
#endif // SERV_GLOBAL_BILLING
		}

		if ( bCheckPackageItem == false )
			InsertPreEquipItem( pItemSlot->GetItemTID() );

		ResetNowEquipUI();
#ifdef PET_PREVIEW
		PetPreview( itemID );
#endif //PET_PREVIEW
		return true;
	}
	else if ( pItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_EQUIPPED )
	{
		RemovePreEquipItemByEquipPos( pItemSlot->GetItemTID() );
		ResetNowEquipUI();
		// Unit Viewer
		PreEquipping();
		return true;
	}

	return false;
}

void CX2CashShop::SetCashShopState( CASH_SHOP_STATE cashShopState, CASH_SHOP_CATEGORY cashShopCate )
{
	m_CashShopState = cashShopState;
	m_CashShopCategory = cashShopCate;

	if ( m_pDLGMainPage != NULL )
		m_pDLGMainPage->SetShowEnable( false, false );

	if ( m_pDLGSubPage != NULL )
		m_pDLGSubPage->SetShowEnable( false, false );

	for ( int i = 0; i < CX2CashShop::CSC_END; i++ )
	{
		CKTDGUIDialogType pDialog = m_pDLGSubPageMenu[i];
		if ( pDialog != NULL )
			pDialog->SetShowEnable( false, false );
	}

	m_pMainNewSM->SetOpen( false );
	m_pMainHotSM->SetOpen( false );
	m_pMainRecommendSM->SetOpen( false );
	m_pSubSM->SetOpen( false );
	m_pSubSpeicialSM->SetOpen( false );

	SetEnable( false );
	SetShow( false );

	SetEnable( CX2Slot::ST_CASH_SHOP_DEPOSIT, true );
	SetShow( CX2Slot::ST_CASH_SHOP_DEPOSIT, true );

	SetEnable( CX2Slot::ST_EQUIPPED, true );
	SetShow( CX2Slot::ST_EQUIPPED, true );
	ResetNowEquipUI();

	SetEnable( CX2Slot::ST_CASH_SHOP_CHOICED, true );
	SetShow( CX2Slot::ST_CASH_SHOP_CHOICED, true );
	ResetItemSlot( CX2Slot::ST_CASH_SHOP_CHOICED, m_vecChoicedItemID );

	ResetSellCashItemByPreEquip();

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
	g_pInstanceData->SetNowSubCategoryList(m_vecCashShopCateList, cashShopCate);
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

	if ( m_CashShopState == CX2CashShop::CSS_MAIN )
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDLGFront->GetControl( L"tab_main" );
		if ( pRadioButton != NULL )
		{
			if ( pRadioButton->GetCheckedControl() != NULL )
				pRadioButton->GetCheckedControl()->SetChecked( false );

			pRadioButton->SetChecked( true );
		}

		if ( m_pDLGMainPage != NULL )
			m_pDLGMainPage->SetShowEnable( true, true );

		//페이지 UI 셋팅해주고..
		//각 내용에 맞게끔 슬롯 리스트 셋팅해줘야겠지?

		m_pMainNewSM->Reset( g_pData->GetItemManager()->GetNewCashItemList(), false, true );
		m_pMainHotSM->Reset( g_pData->GetItemManager()->GetHotCashItemList(), false, true );
		m_pMainRecommendSM->Reset( g_pData->GetItemManager()->GetRecommendCashItemList(), false, true );

		m_pMainNewSM->SetOpen( true );
		m_pMainHotSM->SetOpen( true );
		m_pMainRecommendSM->SetOpen( true );

		SetEnable( CX2Slot::ST_CASH_SHOP_MAIN_NEW, true );
		SetEnable( CX2Slot::ST_CASH_SHOP_MAIN_HOT, true );
		SetEnable( CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND, true );
		SetShow( CX2Slot::ST_CASH_SHOP_MAIN_NEW, true );
		SetShow( CX2Slot::ST_CASH_SHOP_MAIN_HOT, true );
		SetShow( CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND, true );

		ResetItemSlot(  CX2Slot::ST_CASH_SHOP_MAIN_NEW,			m_pMainNewSM->GetNowShowItemIDList() );
		ResetItemSlot(  CX2Slot::ST_CASH_SHOP_MAIN_HOT,			m_pMainHotSM->GetNowShowItemIDList() );
		ResetItemSlot(  CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND,	m_pMainRecommendSM->GetNowShowItemIDList() );

#ifdef SERV_USE_ENABLE_GIFT
		m_pMainHotSM->SetShowPresentButton();
		m_pMainNewSM->SetShowPresentButton();
		m_pMainRecommendSM->SetShowPresentButton();
#endif SERV_USE_ENABLE_GIFT
#ifdef SERV_WISH_LIST_NO_ITEM
		m_pMainHotSM->SetShowCartButton();
		m_pMainNewSM->SetShowCartButton();
		m_pMainRecommendSM->SetShowCartButton();
#endif SERV_WISH_LIST_NO_ITEM
		ResetMainPageNumUI();

	}
	else if ( m_CashShopState == CX2CashShop::CSS_SUB )
	{
		m_CashShopSubCategory = CX2CashShop::CSSC_ALL;
		switch( cashShopCate ) 
		{
		case CX2CashShop::CSC_FASHION:
		case CX2CashShop::CSC_ACCESSORY:
		case CX2CashShop::CSC_CONSUME:
		case CX2CashShop::CSC_INSTALL:
		case CX2CashShop::CSC_PET:
		case CX2CashShop::CSC_EVENT:
			//{{ kimhc // 2009-09-29 // 자동결제
#ifdef	AUTO_PAYMENT
		case CX2CashShop::CSC_AUTO_PAYMENT:
#endif	AUTO_PAYMENT
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
		case CX2CashShop::CSC_EVENT_2:
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
			//}} kimhc // 2009-09-29 // 자동결제
			{
				if ( m_pDLGSubPage != NULL )
					m_pDLGSubPage->SetShowEnable( true, true );

				CKTDGUIDialogType pDialog = m_pDLGSubPageMenu[(int)cashShopCate];
				if ( pDialog != NULL )
				{
					pDialog->SetShowEnable( true, true );
					CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)pDialog->GetControl(L"Totalitem");
					if ( pRadioButton != NULL )
					{
						if ( pRadioButton->GetCheckedControl() != NULL )
						{
							pRadioButton->GetCheckedControl()->SetChecked( false );
						}

						pRadioButton->SetChecked( true );
					}
				}

				vector < int > vecItemID = GetItemByCategory( cashShopCate );

				//{{ kimhc // 2009-10-12 // 자동결제 시 '캐시' 스트링을 '원' 으로 고침
#ifdef	AUTO_PAYMENT
				bool bAutoPayment = false;

				if ( cashShopCate == CX2CashShop::CSC_AUTO_PAYMENT )
					bAutoPayment = true;


				m_pSubSM->SetOpen( true, bAutoPayment );
				m_pSubSM->Reset( vecItemID, bAutoPayment );				
#else	AUTO_PAYMENT
				m_pSubSM->Reset( vecItemID );
				m_pSubSM->SetOpen( true );
#endif	AUTO_PAYMENT
				//}} kimhc // 2009-10-12 // 자동결제 시 '캐시' 스트링을 '원' 으로 고침
				

				SetEnable( CX2Slot::ST_CASH_SHOP_SUB, true );
				SetShow( CX2Slot::ST_CASH_SHOP_SUB, true );
				ResetItemSlot(  CX2Slot::ST_CASH_SHOP_SUB,			m_pSubSM->GetNowShowItemIDList() );

				m_pSubSpeicialSM->Reset( GetHotItemList(vecItemID, CASH_SHOP_SUB_SPECIAL_SLOT_NUM ) );
				m_pSubSpeicialSM->SetOpen( true );
				SetEnable( CX2Slot::ST_CASH_SHOP_SUB_SPECIAL, true );
				SetShow( CX2Slot::ST_CASH_SHOP_SUB_SPECIAL, true );
				ResetItemSlot(  CX2Slot::ST_CASH_SHOP_SUB_SPECIAL,			m_pSubSpeicialSM->GetNowShowItemIDList() );
				
				//카테고리별 아이템 소팅해서 보여주자 잇힝~
			}
			break;
		}

		//{{ kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
#ifdef	AUTO_PAYMENT
		switch ( cashShopCate )
		{
		case CX2CashShop::CSC_AUTO_PAYMENT:
			m_pSubSM->SetShowPresentAndCartButton( false );
			break;

		default:
			m_pSubSM->SetShowPresentAndCartButton( true );
			break;
		}
#endif	AUTO_PAYMENT
		//}} kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가

#ifdef SERV_USE_ENABLE_GIFT
		m_pSubSM->SetShowPresentButton();
#endif SERV_USE_ENABLE_GIFT

#ifdef SERV_WISH_LIST_NO_ITEM
		m_pSubSM->SetShowCartButton();
#endif SERV_WISH_LIST_NO_ITEM

		ResetSubPageNumUI();
	}	
}

void CX2CashShop::ResetItemSlot( CX2Slot::SLOT_TYPE slotType, vector<int>& vecItemID )
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem == NULL )
			continue;

		if ( pSlotItem->GetSlotType() == slotType )
		{
			pSlotItem->SetEnable( false );
			pSlotItem->SetShow( false );
			//pSlotItem->DestroyItemUI();
			pSlotItem->LostItemUI();
		}
	}

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem == NULL )
			continue;

		if ( pSlotItem->GetSlotType() == slotType )
		{
			for ( int j = 0; j < (int)vecItemID.size(); j++ )
			{
				int itemID = vecItemID[j];

				if ( pSlotItem->GetSlotID() == j )
				{
					pSlotItem->SetEnable( true );
					pSlotItem->SetShow( true );
					//pSlotItem->CreateItemUI( itemID );
					if ( pSlotItem->IsCreatedItemUI() == true )
						pSlotItem->ResetItemUI( itemID );
					else
						pSlotItem->CreateItemUI( itemID );

					break;
				}
			}
		}
	}

	ResetSellCashItemByPreEquip();
}

#ifdef SERV_GLOBAL_BILLING
void CX2CashShop::ResetDepositItemSlot( CX2Slot::SLOT_TYPE slotType, vector< KBillOrderInfo >& vecKBillOrderInfo )
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem == NULL )
			continue;

		if ( pSlotItem->GetSlotType() == slotType )
		{
			pSlotItem->SetEnable( false );
			pSlotItem->SetShow( false );
			pSlotItem->DestroyItemUI();
		}
	}

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem == NULL )
			continue;

		if ( pSlotItem->GetSlotType() == slotType )
		{
			for ( int j = 0; j < (int)vecKBillOrderInfo.size(); j++ )
			{
				KBillOrderInfo& kKBillOrderInfo = vecKBillOrderInfo[j];

				if ( pSlotItem->GetSlotID() == j )
				{
					pSlotItem->SetEnable( true );
					pSlotItem->SetShow( true );
					pSlotItem->SetKBillOrderInfo( kKBillOrderInfo );
					pSlotItem->CreateItemUI( kKBillOrderInfo.m_kBillProductInfo.m_iProductID, (int)kKBillOrderInfo.m_kBillProductInfo.m_cQuantity );
					break;
				}
			}
		}
	}
}
#else // SERV_GLOBAL_BILLING
void CX2CashShop::ResetDepositItemSlot( CX2Slot::SLOT_TYPE slotType, vector< KNXBTOrderInfo >& vecKNXBTOrderInfo )
{
	DestroyItemSlotByType( slotType );

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem == NULL )
			continue;

		if ( pSlotItem->GetSlotType() == slotType )
		{
			for ( int j = 0; j < (int)vecKNXBTOrderInfo.size(); j++ )
			{
				KNXBTOrderInfo& kKNXBTOrderInfo = vecKNXBTOrderInfo[j];

				if ( pSlotItem->GetSlotID() == j )
				{
					pSlotItem->SetEnable( true );
					pSlotItem->SetShow( true );
					pSlotItem->SetKNXBTOrderInfo( kKNXBTOrderInfo );
					pSlotItem->CreateItemUI( _wtoi( kKNXBTOrderInfo.m_wstrProductID.c_str() ), kKNXBTOrderInfo.m_usProductPieces );
					break;
				}
			}
		}
	}
}
#endif // SERV_GLOBAL_BILLING

/** @function : DestroyItemSlotByType
	@brief : 인자로 전달된 슬롯의 타입과 같은 슬롯들을 사용하지 않도록 함
	@param : 슬롯타입(eSlotType_)
*/
void CX2CashShop::DestroyItemSlotByType( const CX2Slot::SLOT_TYPE eSlotType_ )
{
	for ( UINT i = 0; i < m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = static_cast<CX2SlotItem*>( m_SlotList[i] );
		if ( pSlotItem != NULL && pSlotItem->GetSlotType() == eSlotType_ )
		{
			pSlotItem->SetEnable( false );
			pSlotItem->SetShow( false );
			pSlotItem->DestroyItemUI();
		}
	}
}

void CX2CashShop::ResetMainPageNumUI()
{
	if ( m_pDLGMainPage != NULL )
	{
		CKTDGUIStatic* pStaticNewItemPage = (CKTDGUIStatic*)m_pDLGMainPage->GetControl( L"g_pStaticNewitem_Font" );
		if ( pStaticNewItemPage != NULL )
		{
			pStaticNewItemPage->SetString( 0, L"" );
			wstringstream wstrstm;
			wstrstm << m_pMainNewSM->GetNowPage() << L"/" << m_pMainNewSM->GetMaxPage();
			pStaticNewItemPage->SetString( 0, wstrstm.str().c_str() );
		}
		CKTDGUIStatic* pStaticHotItemPage = (CKTDGUIStatic*)m_pDLGMainPage->GetControl( L"g_pStaticHotitem_Font" );
		if ( pStaticHotItemPage != NULL )
		{
			pStaticHotItemPage->SetString( 0, L"" );
			wstringstream wstrstm;
			wstrstm << m_pMainHotSM->GetNowPage() << L"/" << m_pMainHotSM->GetMaxPage();
			pStaticHotItemPage->SetString( 0, wstrstm.str().c_str() );
		}
		CKTDGUIStatic* pStaticBestItemPage = (CKTDGUIStatic*)m_pDLGMainPage->GetControl( L"g_pStaticBestitem_Font" );
		if ( pStaticBestItemPage != NULL )
		{
			pStaticBestItemPage->SetString( 0, L"" );
			wstringstream wstrstm;
			wstrstm << m_pMainRecommendSM->GetNowPage() << L"/" << m_pMainRecommendSM->GetMaxPage();
			pStaticBestItemPage->SetString( 0, wstrstm.str().c_str() );
		}
	}
}

void CX2CashShop::ResetSubPageNumUI()
{
	if ( m_pDLGSubPage != NULL )
	{
		CKTDGUIStatic* pStaticPage = (CKTDGUIStatic*)m_pDLGSubPage->GetControl( L"g_pStaticCashshop_Sub_Window_TriangleFont" );
		if ( pStaticPage != NULL )
		{
			pStaticPage->SetString( 0, L"" );
			wstringstream wstrstm;
			wstrstm << m_pSubSM->GetNowPage() << L"/" << m_pSubSM->GetMaxPage();
			pStaticPage->SetString( 0, wstrstm.str().c_str() );
		}
	}
}

int CX2CashShop::GetItemID( CKTDGUIDialogType pDialog )
{
	int itemID = 0;

	if ( pDialog == NULL )
		return itemID;


	itemID = m_pMainNewSM->GetItemID( pDialog );
	if ( itemID != 0 )
		return itemID;

	itemID = m_pMainHotSM->GetItemID( pDialog );
	if ( itemID != 0 )
		return itemID;

	itemID = m_pMainRecommendSM->GetItemID( pDialog );
	if ( itemID != 0 )
		return itemID;

	itemID = m_pSubSM->GetItemID( pDialog );
	if ( itemID != 0 )
		return itemID;

	itemID = m_pSubSpeicialSM->GetItemID( pDialog );
	if ( itemID != 0 )
		return itemID;

	return itemID;
}

void CX2CashShop::UnitHandling( double fTime, float fElapsedTime )
{
#ifdef PET_PREVIEW //함수 분리
	MyUniyHandling(fTime, fElapsedTime); //유저 캐릭터 뷰어
	PetHandling(fTime, fElapsedTime);	 //펫 뷰어
#else
	if( m_pUnitViewerUI != NULL )
	{
		float fRotSpeed = 13.0f;


		if ( CX2UnitViewerUI::GetUnitClicked() == true )
		{
			D3DXVECTOR3 RotateY = D3DXVECTOR3(0, -((float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() * m_fMouseSensitivity), 0);
			m_pUnitViewerUI->GetMatrix().RotateRel(RotateY);
			m_bRotateReturn = false;

			fRotSpeed = 13.0f;
		}
		else
		{
			D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
			m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );



			m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
			int addSpeed = 20;

			if ( m_bRotateReturn == false )
			{
				fRotSpeed -= (fElapsedTime * addSpeed);

				if ( fRotSpeed <= 0 )
				{
					fRotSpeed = fElapsedTime;
				}

				if (m_fRemainRotY >= 0.0f &&  m_fRemainRotY < 3.14f )
				{

					if ( m_fRemainRotY <= 0.0f )
					{
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						m_fRemainRotY = 0.0f;
						m_bRotateReturn = true;
					}
					else
					{
						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY <= 0.0f )
						{
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							m_fRemainRotY = 0.0f;
							m_bRotateReturn = true;
						}
					}
				}
				else if ( m_fRemainRotY > 3.14f && m_fRemainRotY <= 6.18f  )
				{
					if ( m_fRemainRotY >= 6.18f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY += (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY >= 6.18f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
				else if ( m_fRemainRotY >= -3.14f && m_fRemainRotY <= 0.0f )
				{
					if ( m_fRemainRotY >= 0.0f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY += (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY >= 0.0f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
				else if (  m_fRemainRotY >= -6.28f && m_fRemainRotY <= -3.14f )
				{
					if ( m_fRemainRotY <= -6.28f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY <= -6.28f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}

				}
			}
		}

		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );

		if ( m_pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
		{
			m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
		}
	}
#endif //PET_PREVIEW
}

bool CX2CashShop::IsInUnitClickReact( D3DXVECTOR2 mousePos )
{
	D3DXVECTOR2 m_ConvertPos		= g_pKTDXApp->ConvertByResolution( m_UnitClickPos );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( m_UnitClickSize );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;
}

vector<int> CX2CashShop::GetItemByCategory( CASH_SHOP_CATEGORY categoryID )
{
	vector< int > vecAllItem = g_pData->GetItemManager()->GetAllCashItemList();
	vector< int > vecItem;

	for ( int cateIndex = 0; cateIndex < (int)m_vecCashShopCateList.size(); cateIndex++ )
	{
		CX2CashShop::CashShopCategory* pCashShopCategory = m_vecCashShopCateList[cateIndex];
		if ( pCashShopCategory != NULL )
		{
			if ( (int)pCashShopCategory->m_CategoryID.x == (int)categoryID )
			{
				for ( int i = 0; i < (int)vecAllItem.size(); i++ )
				{
					int itemID = vecAllItem[i];

					if ( m_bAllCharItem == false && IsPossibleUsedByMyCharacter(itemID) == false )
						continue;

					//{{ kimhc // 2009-08-10 // 자신의 은행 회원 등급에 맞지 않으면 보이지 않도록 함
#ifdef	PRIVATE_BANK
					if ( IsPossibleUsedByMyBankMembership( itemID ) == false )
						continue;
#endif	PRIVATE_BANK
					//}} kimhc // 2009-08-10 // 자신의 은행 회원 등급에 맞지 않으면 보이지 않도록 함


					int iClassChangeItemLevel = GetClassChangeItemLevel( itemID );
					if( iClassChangeItemLevel > 0 &&															// 전직 캐시 아이템인데
						1 != iClassChangeItemLevel - g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() )	// 현재 캐릭터 클래스가 사용할 수 없는 아이템이면
						continue;

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
					if( itemID == UNLIMITED_SECOND_CHANGE_JOB_ITEM_ID
						&& 2 != g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() )						  //2차 전직 아니면 보이지 않도록 함
						continue;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef PACKAGEITEM_SET_NOT_EQUIP_ITEM
					const CX2Item::ItemTemplet *pItemTempletPackage = g_pData->GetItemManager()->GetItemTemplet( itemID );
					if( pItemTempletPackage == NULL )
						continue;

					if( g_pData == NULL ||
						g_pData->GetMyUser() == NULL ||
						g_pData->GetMyUser()->GetSelectUnit() == NULL ||
						g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet() == NULL)
						continue;

					vector< CX2ItemManager::PackageItemData > vecCheckPackageItemData;

					if ( m_bAllCharItem == false											// 전체 아이템 아니고,
						&& pItemTempletPackage->GetItemType() == CX2Item::IT_SPECIAL			// 스페셜 타입이고,
						&& pItemTempletPackage->GetUnitType() != g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType // unitType 관련해서 내것이 아닌 경우
						&& g_pData->GetItemManager()->GetPackageData(pItemTempletPackage->GetItemID(), vecCheckPackageItemData)
						)
					{
						if (pItemTempletPackage->GetUnitType() != CX2Unit::UNIT_TYPE::UT_NONE)
							continue;
					}
#endif PACKAGEITEM_SET_NOT_EQUIP_ITEM

#ifdef SERV_SKILL_NOTE
					if( itemID == CASH_SKILL_NOTE_ITEM_ID && GetCanByeSkillNoteItem( itemID ) == false )
					{
						continue;
					}
#endif

#ifdef SERV_SHARING_BANK_QUEST_CASH
					if( itemID == SHARING_BANK_OPEN_CASH_ITEM_ID && GetCahBuySharingBankItem( itemID ) == false )
					{
						continue;
					}
#endif

#ifdef UNIT_CLASS_CHANGE_ITEM
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
					if( pItemTemplet == NULL )
						continue;

					int iUnitClassChangeItemLevel = GetUnitClassChangeItemLevel( itemID );
					int iMyClassLevel = g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel();
					int iItemUnitType = pItemTemplet->GetUnitType();
					int iUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
					int iItemUnitClass = pItemTemplet->GetUnitClass();
					int iUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();

					if ( (iUnitClassChangeItemLevel > 0 && iItemUnitType != iUnitType) || iUnitClassChangeItemLevel > iMyClassLevel )
						continue;
					
					if ( iUnitClassChangeItemLevel > 0 && iUnitClassChangeItemLevel < iMyClassLevel )
						continue;

					if ( iUnitClassChangeItemLevel > 0 && iItemUnitClass == iUnitClass )
						continue;

	#ifdef BUY_ONLY_ARA_ITEM
					if( true== IsCanByeOnlyAraItem(itemID) && //아라만 살 수 있는 아이템인데
						CX2Unit::UT_ARA != iUnitType ) //내가 아라가 아니라면
					{
						continue;
					}
	#endif //BUY_ONLY_ARA_ITEM

#endif
#ifdef	SERV_EXPAND_QUICK_SLOT
					if( itemID == EXPAND_QUICK_SLOT_ITEM_ID && GetCahBuyExpandQuickSlotItem( itemID ) == false )
					{
						continue;
					}
#endif  SERV_EXPAND_QUICK_SLOT

					//{{ 허상형 : [2012/12/5] //		특정 아이템 가지고 있어야 캐시샵에서 구매 가능
#ifdef	SERV_KEEP_ITEM_SHOW_CASHSHOP
					if( g_pData != NULL )
					{
						if( g_pData->GetItemManager() != NULL )
						{
							bool bContinue = false;
							std::vector<int> TempItemID;
							TempItemID.clear();
							if( g_pData->GetItemManager()->CheckKeepShowItem( itemID , TempItemID ) == true )
							{
								for( int i = 0; i < TempItemID.size(); ++i )
								{
									if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( TempItemID[i] ) == NULL )
									{
										bContinue = true;
										break;
									}
								}
								if( bContinue == true)
								{
									continue;
								}
							}
						}
					}
#endif  SERV_KEEP_ITEM_SHOW_CASHSHOP
						//}} 허상형 : [2012/12/5] //		특정 아이템 가지고 있어야 캐시샵에서 구매 가능

#ifdef SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM
					if( (itemID == 60007354 || itemID == 60007355 || itemID == 60007356) && CX2Unit::UC_ARA_LITTLE_HSIEN != iUnitClass )
					{
						continue;
					}
#endif SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM
 
#ifdef SERV_BUY_EXCEPTION_ARA
					if( g_pData->GetMyUser()->GetSelectUnit()->GetType() == CX2Unit::UT_ARA && ( itemID == 60007760 || itemID == 60007761 || itemID == 60007762 || itemID == 60007763 ) )
						continue;
#endif SERV_BUY_EXCEPTION_ARA

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
					/// 교본 아이템인데, 요구 클래스 레빌이 현재 전직 클래스 레벨보다 높다면
					if( NULL != g_pData &&
						NULL != g_pData->GetSkillTree() &&
						true == g_pData->GetSkillTree()->IsUnsealSkillItemID( itemID ) &&
						iMyClassLevel < g_pData->GetSkillTree()->GetUnsealSkillItemRequireClass( itemID ) )
					{
						continue;
					}
#endif // UPGRADE_SKILL_SYSTEM_2013

					CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
					if ( pCashItem != NULL )
					{
#ifdef SERV_GLOBAL_BILLING
						// 첫번째 아이템 카테고리로 아이템을 등록한다.
						// 이걸 모든 상품의 카테고리를 다 보도록 하자
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
						std::vector<int> vecGateList = pCashItem->GetGateListOfProduct();
						if ( vecGateList.empty() == false )
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
						const KBillProductInfo* pkKBillProductInfo = pCashItem->GetGateProduct();
						if ( pkKBillProductInfo != NULL )
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
						{
							for ( int j = 0; j < (int)pCashShopCategory->m_vecSubCategoryID.size(); j++ )
							{
								D3DXVECTOR2 subCateID = pCashShopCategory->m_vecSubCategoryID[j];

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
								if ( std::find( vecGateList.begin(), vecGateList.end(),(int)subCateID.y) != vecGateList.end() )
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
								if ( (int)pkKBillProductInfo->m_cCategoryNo == (int)subCateID.y )
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
								{
									vecItem.push_back( pCashItem->m_ItemID );
								}
							}
						}
#else // SERV_GLOBAL_BILLING
						const KNXBTProductInfo* pkKNXBTProductInfo = pCashItem->GetGateProduct();
						if ( pkKNXBTProductInfo != NULL )
						{
							for ( int j = 0; j < (int)pCashShopCategory->m_vecSubCategoryID.size(); j++ )
							{
								D3DXVECTOR2 subCateID = pCashShopCategory->m_vecSubCategoryID[j];

								if ( pkKNXBTProductInfo->m_ulCategoryNo == (unsigned long)subCateID.y )
								{
									vecItem.push_back( pCashItem->m_ItemID );
								}
							}
						}
#endif // SERV_GLOBAL_BILLING
					}
				}

				break;
			}
		}
	}


	return vecItem;
}




vector<int> CX2CashShop::GetItemByCategory( CASH_SHOP_CATEGORY categoryID, CASH_SHOP_SUB_CATEGORY subCategoryID )
{
	vector< int > vecAllItem = g_pData->GetItemManager()->GetAllCashItemList();
	vector< int > vecItem;

	for ( int cateIndex = 0; cateIndex < (int)m_vecCashShopCateList.size(); cateIndex++ )
	{
		CX2CashShop::CashShopCategory* pCashShopCategory = m_vecCashShopCateList[cateIndex];
		if ( pCashShopCategory != NULL )
		{
			if ( (int)pCashShopCategory->m_CategoryID.x == (int)categoryID )
			{
				for ( int i = 0; i < (int)vecAllItem.size(); i++ )
				{
					int itemID = vecAllItem[i];

					if ( m_bAllCharItem == false && IsPossibleUsedByMyCharacter(itemID) == false )
						continue;

					//{{ kimhc // 2009-08-10 // 자신의 은행 회원 등급에 맞지 않으면 보이지 않도록 함
#ifdef	PRIVATE_BANK
					if ( IsPossibleUsedByMyBankMembership( itemID ) == false )
						continue;
#endif	PRIVATE_BANK
					//}} kimhc // 2009-08-10 // 자신의 은행 회원 등급에 맞지 않으면 보이지 않도록 함

					
					int iClassChangeItemLevel = GetClassChangeItemLevel( itemID );
					if( iClassChangeItemLevel > 0 &&															// 전직 캐시 아이템인데
						1 != iClassChangeItemLevel - g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() )	// 현재 캐릭터 클래스가 사용할 수 없는 아이템이면
						continue;

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
					if( itemID == UNLIMITED_SECOND_CHANGE_JOB_ITEM_ID
						&& 2 != g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() )						  //2차 전직 아니면 보이지 않도록 함
						continue;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef PACKAGEITEM_SET_NOT_EQUIP_ITEM
					const CX2Item::ItemTemplet *pItemTempletPackage = g_pData->GetItemManager()->GetItemTemplet( itemID );
					if( pItemTempletPackage == NULL )
						continue;

					if( g_pData == NULL ||
						g_pData->GetMyUser() == NULL ||
						g_pData->GetMyUser()->GetSelectUnit() == NULL ||
						g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet() == NULL)
						continue;

					vector< CX2ItemManager::PackageItemData > vecCheckPackageItemData;

					if ( m_bAllCharItem == false											// 전체 아이템 아니고,
						&& pItemTempletPackage->GetItemType() == CX2Item::IT_SPECIAL			// 스페셜 타입이고,
						&& pItemTempletPackage->GetUnitType() != g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType // unitType 관련해서 내것이 아닌 경우
						&& g_pData->GetItemManager()->GetPackageData(pItemTempletPackage->GetItemID(), vecCheckPackageItemData)
						)
					{
						if (pItemTempletPackage->GetUnitType() != CX2Unit::UNIT_TYPE::UT_NONE)
							continue;
					}
#endif PACKAGEITEM_SET_NOT_EQUIP_ITEM

#ifdef SERV_SKILL_NOTE
					if( itemID == CASH_SKILL_NOTE_ITEM_ID && GetCanByeSkillNoteItem( itemID ) == false )
					{
						continue;
					}
#endif

#ifdef SERV_SHARING_BANK_QUEST_CASH
					if( itemID == SHARING_BANK_OPEN_CASH_ITEM_ID && GetCahBuySharingBankItem( itemID ) == false )
					{
						continue;
					}
#endif

#ifdef UNIT_CLASS_CHANGE_ITEM
					int iUnitClassChangeItemLevel = GetUnitClassChangeItemLevel( itemID );
					int iMyClassLevel = g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel();
					int iItemUnitType = g_pData->GetItemManager()->GetItemTemplet( itemID )->GetUnitType();
					int iUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
					int iItemUnitClass = g_pData->GetItemManager()->GetItemTemplet( itemID )->GetUnitClass();
					int iUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();

					if ( (iUnitClassChangeItemLevel > 0 && iItemUnitType != iUnitType) || iUnitClassChangeItemLevel > iMyClassLevel )
						continue;

					if ( iUnitClassChangeItemLevel > 0 && iUnitClassChangeItemLevel < iMyClassLevel )
						continue;

					if ( iUnitClassChangeItemLevel > 0 && iItemUnitClass == iUnitClass )
						continue;
#endif
#ifdef	SERV_EXPAND_QUICK_SLOT
					if( itemID == EXPAND_QUICK_SLOT_ITEM_ID && GetCahBuyExpandQuickSlotItem( itemID ) == false )
					{
						continue;
					}
#endif  SERV_EXPAND_QUICK_SLOT

#ifdef	SERV_KEEP_ITEM_SHOW_CASHSHOP
					if( g_pData != NULL )
					{
						if( g_pData->GetItemManager() != NULL )
						{
							bool bContinue = false;
							std::vector<int> TempItemID;
							TempItemID.clear();
							if( g_pData->GetItemManager()->CheckKeepShowItem( itemID , TempItemID ) == true )
							{
								for( int i = 0; i < TempItemID.size(); ++i )
								{
									if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( TempItemID[i] ) == NULL )
									{
										bContinue = true;
										break;
									}
								}
								if( bContinue == true)
								{
									continue;
								}
							}
						}
					}
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM
					if( (itemID == 60007354 || itemID == 60007355 || itemID == 60007356) && CX2Unit::UC_ARA_LITTLE_HSIEN != iUnitClass )
					{
						continue;
					}
#endif SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM

#ifdef SERV_BUY_EXCEPTION_ARA
					if( g_pData->GetMyUser()->GetSelectUnit()->GetType() == CX2Unit::UT_ARA && ( itemID == 60007760 || itemID == 60007761 || itemID == 60007762 || itemID == 60007763 ) )
						continue;
#endif SERV_BUY_EXCEPTION_ARA

					CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
					if ( pCashItem != NULL )
					{
#ifdef SERV_GLOBAL_BILLING
						const KBillProductInfo* pkKBillProductInfo = pCashItem->GetGateProduct();
						if ( pkKBillProductInfo != NULL )
						{
							for ( int j = 0; j < (int)pCashShopCategory->m_vecSubCategoryID.size(); j++ )
							{
								D3DXVECTOR2 subCateID = pCashShopCategory->m_vecSubCategoryID[j];

								if ( (int)subCateID.x == (int)subCategoryID )
								{
									if ( (int)pkKBillProductInfo->m_cCategoryNo == (int)subCateID.y )
									{
										vecItem.push_back( pCashItem->m_ItemID );
									}
								}
							}
						}
#else // SERV_GLOBAL_BILLING
						const KNXBTProductInfo* pkKNXBTProductInfo = pCashItem->GetGateProduct();
						if ( pkKNXBTProductInfo != NULL )
						{
							for ( int j = 0; j < (int)pCashShopCategory->m_vecSubCategoryID.size(); j++ )
							{
								D3DXVECTOR2 subCateID = pCashShopCategory->m_vecSubCategoryID[j];

								if ( (int)subCateID.x == (int)subCategoryID )
								{
									if ( pkKNXBTProductInfo->m_ulCategoryNo == (unsigned long)subCateID.y )
									{
										vecItem.push_back( pCashItem->m_ItemID );
									}
								}
							}
						}
#endif // SERV_GLOBAL_BILLING
					}
				}

				break;
			}
		}
	}

	return vecItem;
}

void CX2CashShop::ChangeCategory( CKTDGUIRadioButton* pRadioButton )
{
	if ( pRadioButton == NULL )
		return;

	if ( wcscmp( pRadioButton->GetName(), L"tab_main" ) == 0 )
	{
		SetCashShopState( CX2CashShop::CSS_MAIN );
	}
	else
	{
		SetCashShopState( CX2CashShop::CSS_SUB, (CX2CashShop::CASH_SHOP_CATEGORY)pRadioButton->GetDummyInt(0) );
	}
}
void CX2CashShop::ChangeSubCategory( CX2CashShop::CASH_SHOP_SUB_CATEGORY eSubCategory )
{	
	m_CashShopSubCategory = eSubCategory;

	vector< int > vecItemID;
	if ( m_CashShopSubCategory == CX2CashShop::CSSC_ALL )
	{
		//현재 선택된 카테고리의 모든 물품을 보여주자
		vecItemID = GetItemByCategory(m_CashShopCategory);
	}
	else
	{
		vecItemID = GetItemByCategory(m_CashShopCategory, m_CashShopSubCategory );
	}

	//{{ kimhc // 2009-10-12 // 자동결제 시 '캐시' 스트링을 '원' 으로 고침
	#ifdef	AUTO_PAYMENT
	bool bAutoPayment = false;

	if ( m_CashShopCategory == CSC_AUTO_PAYMENT )
		bAutoPayment = true;

	m_pSubSM->Reset( vecItemID, bAutoPayment );
	#else	AUTO_PAYMENT
	m_pSubSM->Reset( vecItemID );
	#endif	AUTO_PAYMENT
	//}} kimhc // 2009-10-12 // 자동결제 시 '캐시' 스트링을 '원' 으로 고침

#ifdef SERV_USE_ENABLE_GIFT
	m_pSubSM->SetShowPresentButton();
#endif SERV_USE_ENABLE_GIFT

#ifdef SERV_WISH_LIST_NO_ITEM
	m_pSubSM->SetShowCartButton();
#endif SERV_WISH_LIST_NO_ITEM

	ResetItemSlot(  CX2Slot::ST_CASH_SHOP_SUB,			m_pSubSM->GetNowShowItemIDList() );

	m_pSubSpeicialSM->Reset( GetHotItemList( vecItemID, CASH_SHOP_SUB_SPECIAL_SLOT_NUM ) );
	ResetItemSlot(  CX2Slot::ST_CASH_SHOP_SUB_SPECIAL,			m_pSubSpeicialSM->GetNowShowItemIDList() );

	ResetSubPageNumUI();
}
void CX2CashShop::ChangeSubCategory( CKTDGUIRadioButton* pRadioButton )
{
	if ( pRadioButton == NULL )
		return;

	ChangeSubCategory ( static_cast<CX2CashShop::CASH_SHOP_SUB_CATEGORY>(pRadioButton->GetDummyInt(0)) );
}

/*static*/ bool CX2CashShop::IsPossibleUsedByMyCharacter( int itemID )
{	
	return CX2Unit::CanUse( itemID, g_pData->GetMyUser()->GetSelectUnit() );
}

vector<int> CX2CashShop::GetHotItemList( vector<int>& vecItemID, int maxItemNum )
{
	vector< int > vecHotItemID;
#ifdef CASH_SHOP_HOT_ITEM_LIST_FIX
	int iHotItemCheck = 0;
	vector< int > vecTempHotCashItem =  g_pData->GetItemManager()->GetHotCashItemList();
#endif //CASH_SHOP_HOT_ITEM_LIST_FIX

#ifdef CASH_SHOP_HOT_ITEM_LIST_FIX
	for ( int i = 0; i < (int)vecTempHotCashItem.size(); i++ )
	{
		int itemID = vecTempHotCashItem[i];

		CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
		if ( pCashItem == NULL )
			continue;

		if ( pCashItem->GetIsHot() == true )
		{
			if ( std::find(vecItemID.begin(), vecItemID.end(), itemID) != vecItemID.end() )
			{
				vecHotItemID.push_back(itemID);
				if ( (int)vecHotItemID.size() >= maxItemNum )
				{
					break;
				}
			}
		}
	}
#else //CASH_SHOP_HOT_ITEM_LIST_FIX
	for ( int i = 0; i < (int)vecItemID.size(); i++ )
	{
		int itemID = vecItemID[i];
		CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
		if ( pCashItem == NULL )
			continue;

		if ( pCashItem->GetIsHot() == true )
		{
			vecHotItemID.push_back( pCashItem->m_ItemID );
			if ( (int)vecHotItemID.size() >= maxItemNum )
			{
				break;
			}
		}
	}
#endif //CASH_SHOP_HOT_ITEM_LIST_FIX

	std::sort( vecHotItemID.begin(), vecHotItemID.end(), CashItemSorter() );

	return vecHotItemID;
}

bool CX2CashShop::CheckIsPossibleEqiupSlot( CX2Slot::SLOT_TYPE slotType )
{
	if ( slotType == CX2SlotManager::CX2Slot::ST_CASH_SHOP_MAIN_HOT ||
		slotType == CX2SlotManager::CX2Slot::ST_CASH_SHOP_MAIN_NEW ||
		slotType == CX2SlotManager::CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND ||
		slotType == CX2SlotManager::CX2Slot::ST_CASH_SHOP_SUB ||
		slotType == CX2SlotManager::CX2Slot::ST_CASH_SHOP_SUB_SPECIAL )
	{
		return true;
	}

	return false;
}

bool CX2CashShop::CheckPreEqipItem( int itemTID )
{
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	vector<int >::iterator it =
		std::find( m_mapPreEquipItemID[ m_uiSelectPreEquipSet ].begin(), m_mapPreEquipItemID[ m_uiSelectPreEquipSet ].end(), itemTID );

	return it != m_mapPreEquipItemID[ m_uiSelectPreEquipSet ].end() ;
#else
	return std::find( m_vecPreEquipItemID.begin(), m_vecPreEquipItemID.end(), itemTID ) != m_vecPreEquipItemID.end();
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
}

void CX2CashShop::RemovePreEquipItemByEquipPos( int itemTID )
{
	const CX2Item::ItemTemplet* pItemTempletSrc = 
		g_pData->GetItemManager()->GetItemTemplet( itemTID );

	if ( pItemTempletSrc == NULL )
		return;


#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	std::vector< int >::iterator it = m_mapPreEquipItemID[ m_uiSelectPreEquipSet ].begin();
	for( ; it != m_mapPreEquipItemID[ m_uiSelectPreEquipSet ].end(); ++it )
	{
		const CX2Item::ItemTemplet* pItemTemplet = 
			g_pData->GetItemManager()->GetItemTemplet( *it);

		if( NULL != pItemTemplet )
		{
			if ( pItemTemplet->GetEqipPosition() == pItemTempletSrc->GetEqipPosition() && 
				( pItemTemplet->GetItemType() == CX2Item::IT_ACCESSORY || pItemTemplet->GetFashion() == pItemTempletSrc->GetFashion() ))
			{
				m_mapPreEquipItemID[m_uiSelectPreEquipSet].erase( it );
				return;
			}	
		}
	}
#else
	std::vector< int >::iterator iS;
	for ( iS = m_vecPreEquipItemID.begin(); iS != m_vecPreEquipItemID.end(); iS++ )
	{
		int iCuritemTID = (*iS);
        const CX2Item::ItemTemplet* pItemTemplate = 
			g_pData->GetItemManager()->GetItemTemplet( iCuritemTID );

		if ( pItemTemplate->GetEqipPosition() == pItemTempletSrc->GetEqipPosition() && 
			( pItemTemplate->GetItemType() == CX2Item::IT_ACCESSORY || pItemTemplate->GetFashion() == pItemTempletSrc->GetFashion() ))
		{
			m_vecPreEquipItemID.erase( iS );
			return;
		}	
	}
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
}

bool CX2CashShop::InsertPreEquipItem( int itemTID, bool bForce )
{
	// 케릭터 종류에 맞는지 체크
    const CX2Item::ItemTemplet* pItemTemplate = g_pData->GetItemManager()->GetItemTemplet( (int) itemTID );
	if ( pItemTemplate == NULL )
		return false;

	if (g_pData->GetMyUser()->GetSelectUnit()->GetType() != pItemTemplate->GetUnitType()
		&& pItemTemplate->GetUseCondition() != CX2Item::UC_ANYONE )
		return false;



	if ( bForce == true )
	{
		RemovePreEquipItemByEquipPos( itemTID );

#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
		m_mapPreEquipItemID[ m_uiSelectPreEquipSet ].push_back( itemTID );
#else
		m_vecPreEquipItemID.push_back( itemTID );
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	}
	else
	{
		// 다른 아이템일 경우 추가
		if ( CheckPreEqipItem( itemTID ) == false )
		{
			RemovePreEquipItemByEquipPos( itemTID );

#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
			m_mapPreEquipItemID[ m_uiSelectPreEquipSet ].push_back( itemTID );
#else
			m_vecPreEquipItemID.push_back( itemTID );
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
		}
		// 같은 아이템일 경우 해제
		else
		{
			RemovePreEquipItemByEquipPos( itemTID );
		}
	}


	// Unit Viewer
	PreEquipping();

	return true;
}

void CX2CashShop::PreEquipping()
{
	m_pUnitViewerUI->UpdateEqip( true );

#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	BOOST_FOREACH( int iItemTID, m_mapPreEquipItemID[m_uiSelectPreEquipSet] )
	{
		m_pUnitViewerUI->AddEquipByTID( iItemTID );
	}
#else
	for ( unsigned int iT = 0; iT < m_vecPreEquipItemID.size(); ++iT )
	{
		int iItemTID = m_vecPreEquipItemID[iT];
		m_pUnitViewerUI->AddEquipByTID( iItemTID );
	}
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
}

void CX2CashShop::OnRevertItem( int iItemTID )
{
	RemovePreEquipItemByEquipPos( iItemTID );

	ResetNowEquipUI();

	PreEquipping();
}

void CX2CashShop::ResetNowEquipUI()
{
	const CX2Unit::UnitData& kMyUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	// 현재 내정보에 장착된 슬롯 초기화
	ClearNowEquipUI();

	// 내 인벤토리에 맞게 업데이트
	ResetNowEquipUIByInven(kMyUnitData, kInventory);

	// 샵 미리보기 정보에 맞게 업데이트
	ResetNowEquipUIByShopEquip();

	// 슬롯 선택 표시 업데이트
	UpdateShopEquipedPicture();

	ResetSellCashItemByPreEquip();
}

void CX2CashShop::ClearNowEquipUI()
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlot( i );
		if ( pItemSlot != NULL && pItemSlot->GetSlotType() == CX2ItemSlotManager::CX2Slot::ST_EQUIPPED )
		{
			/*
			CKTDGUIDialogType pDialog = pItemSlot->GetDialog();
			SAFE_DELETE_DIALOG( pDialog );
			pItemSlot->SetDialog( NULL );
			*/

				pItemSlot->LostItemUI();
		}
	}
}

void CX2CashShop::ResetNowEquipUIByInven( const CX2Unit::UnitData& kMyUnitData, const CX2Inventory& kInventory)
{
#ifdef TAKE_OFF_ALL_ITEM
	if( false == m_bShowInvenEquip )
		return;
#endif // TAKE_OFF_ALL_ITEM

	{
		for ( int i = 0; i < (int)kMyUnitData.m_NowEqipItemUIDList.size(); i++ )
		{
			UidType uidType = kMyUnitData.m_NowEqipItemUIDList[i];
			CX2Item* pItem = kInventory.GetItem( uidType );
			if ( pItem == NULL )
				continue;

			for ( int j = 0; j < (int)m_SlotList.size(); j++ )
			{
				CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlot( j );
				if ( pItemSlot->GetSlotType() == CX2ItemSlotManager::CX2Slot::ST_EQUIPPED && 
					pItemSlot->GetEquipPos() == pItem->GetItemTemplet()->GetEqipPosition() &&
					( pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_ACCESSORY || pItemSlot->GetFashion() == pItem->GetItemTemplet()->GetFashion() ) )	// 09.06.02 태완 : 패션 악세사리 장착 예외처리
				{
					//pItemSlot->CreateItemUI( pItem );
					if ( pItemSlot->IsCreatedItemUI() == true )
						pItemSlot->ResetItemUI( pItem );
					else
						pItemSlot->CreateItemUI( pItem );

					AttachShopEquipedPicture( pItemSlot );
					break;
				}
			}
		}
	}
}

void CX2CashShop::ResetNowEquipUIByShopEquip()
{

#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	BOOST_FOREACH( int itemTID, m_mapPreEquipItemID[m_uiSelectPreEquipSet] )
	{
#else
	for ( int iS = 0; iS < (int)m_vecPreEquipItemID.size(); ++iS )
	{
		int itemTID = m_vecPreEquipItemID[iS];
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
        const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );

		for ( int j = 0; j < (int)m_SlotList.size(); j++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlot( j );
			if ( pItemSlot->GetSlotType() == CX2ItemSlotManager::CX2Slot::ST_EQUIPPED && 
				pItemSlot->GetEquipPos() == pItemTemplet->GetEqipPosition() &&
				( pItemTemplet->GetItemType() == CX2Item::IT_ACCESSORY || pItemSlot->GetFashion() == pItemTemplet->GetFashion() ) )
			{
				if ( pItemSlot->IsCreatedItemUI() == true )
					pItemSlot->ResetItemUI( itemTID, -1 );
				else
					pItemSlot->CreateItemUI( itemTID, -1 );

				AttachShopEquipedPicture( pItemSlot );
				break;
			}
		}
	}
}

void CX2CashShop::ResetSellCashItemByPreEquip()
{
#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
	BOOST_FOREACH( int itemTID, m_mapPreEquipItemID[m_uiSelectPreEquipSet] )
	{
#else
	for ( int iS = 0; iS < (int)m_vecPreEquipItemID.size(); ++iS )
	{
		int itemTID = m_vecPreEquipItemID[iS];
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
		//CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );

		for ( int j = 0; j < (int)m_SlotList.size(); j++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlot( j );
			if ( CheckIsPossibleEqiupSlot( pItemSlot->GetSlotType() ) == true ||
				pItemSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_CHOICED )
			{
				if ( pItemSlot->IsResetItemUI() == true )
					AttachShopEquipedPicture( pItemSlot );
			}
		}
	}

	for ( int j = 0; j < (int)m_SlotList.size(); j++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlot( j );		
		if ( CheckIsPossibleEqiupSlot( pItemSlot->GetSlotType() ) == true ||
			pItemSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_CHOICED )
		{
			SetShowShopEquipedPicture( pItemSlot, false );
			if ( CheckPreEqipItem( pItemSlot->GetItemTID() ) == true )
			{
				SetShowShopEquipedPicture( pItemSlot, true );
			}
		}		
	}
}

void CX2CashShop::UpdateShopEquipedPicture()
{
	for ( int j = 0; j < (int)m_SlotList.size(); j++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlot( j );		
		if ( pItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_EQUIPPED)
		{
			SetShowShopEquipedPicture( pItemSlot, false );
			if ( CheckPreEqipItem( pItemSlot->GetItemTID() ) == true )
			{
				SetShowShopEquipedPicture( pItemSlot, true );
			}
		}		
	}
}

void CX2CashShop::AttachShopEquipedPicture( CX2SlotItem* pItemSlot )
{
	if( NULL == pItemSlot )
		return; 



	CKTDGUIDialogType pkDialog = pItemSlot->GetDialog();
	if ( pkDialog == NULL )
		return;

	CKTDGUIStatic* pStaticEquipedBorder = NULL;
	if ( pkDialog->CheckControl( L"StaticEquipedBorder" ) == true )
	{
		pStaticEquipedBorder = (CKTDGUIStatic*)pkDialog->GetControl( L"StaticEquipedBorder" );
	}

	CKTDGUIStatic* pStaticEquipedBorder2 = NULL;
	if ( pkDialog->CheckControl( L"StaticEquipedBorder2" ) == true )
	{
		pStaticEquipedBorder2 = (CKTDGUIStatic*)pkDialog->GetControl( L"StaticEquipedBorder2" );
	}

	if ( pStaticEquipedBorder == NULL )
	{
		pStaticEquipedBorder = new CKTDGUIStatic();
		pStaticEquipedBorder->SetShow( false );
		pStaticEquipedBorder->SetName( L"StaticEquipedBorder" );
		pkDialog->AddControl( pStaticEquipedBorder );
		pkDialog->ChangeSequence( pStaticEquipedBorder, false );

		CKTDGUIControl::CPictureData* pPictureEquiped = new CKTDGUIControl::CPictureData();
		pPictureEquiped->SetTex( L"HQ_ShopEqipedBorder2.tga" );
		pPictureEquiped->SetPoint( new CKTDGUIControl::UIPointData );
		
		//{{ 2009.01.19 김태완 : 코드 정리
		switch(pItemSlot->GetSlotType())
		{
		case CX2Slot::ST_EQUIPPED:
		case CX2Slot::ST_CASH_SHOP_SUB_SPECIAL:
		case CX2Slot::ST_CASH_SHOP_CHOICED:
			{
				pPictureEquiped->SetPos( D3DXVECTOR2( -1, -1 ) );

				// 무기 악세사리 슬롯은 크기가 다른 슬롯보다 작아서 예외 처리
				pPictureEquiped->SetSize( pItemSlot->GetSize() + D3DXVECTOR2( 2, 2 ) );
				//if( CX2Unit::EP_AC_WEAPON == pItemSlot->m_EquipPos )
				//{
				//	pPictureEquiped->SetSize( D3DXVECTOR2( 43, 43 ) );
				//}
				//else
				//{
				//	pPictureEquiped->SetSize( D3DXVECTOR2( 34, 34 ) );
				//}
			} break;
		case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
		case CX2Slot::ST_CASH_SHOP_MAIN_HOT:
		case CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND:
		case CX2Slot::ST_CASH_SHOP_SUB:
			{
				pPictureEquiped->SetPos( D3DXVECTOR2( -3, -3 ) );
				pPictureEquiped->SetSize( D3DXVECTOR2( 71 + 6, 71 + 6 ) );

				if ( pStaticEquipedBorder2 == NULL )
				{
					pStaticEquipedBorder2 = new CKTDGUIStatic();
					pStaticEquipedBorder2->SetShow( false );
					pStaticEquipedBorder2->SetName( L"StaticEquipedBorder2" );
					pkDialog->AddControl( pStaticEquipedBorder2 );
					pkDialog->ChangeSequence( pStaticEquipedBorder2, true );

					CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
					pStaticEquipedBorder2->AddString( pString );
					pString->fontStyle = CKTDGFontManager::FS_SHELL;
					pString->fontIndex = SLOT_MANAGER_FONT_INDEX;
					pString->color = D3DXCOLOR( 1,1,0,1 );
					pString->outlineColor = D3DXCOLOR( 0,0,0,1 );
					pString->msg = GET_STRING( STR_ID_31 );
					pString->sortFlag = DT_CENTER;
					pString->pos = D3DXVECTOR2( pItemSlot->GetSize().x / 2.0f + 2, pItemSlot->GetSize().y - 20 );
				}
			}break;
		default:
			break;
		}
		//}}
		pStaticEquipedBorder->AddPictureAt( 0, pPictureEquiped );
	}

}

void CX2CashShop::SetShowShopEquipedPicture( CX2SlotItem* pItemSlot, bool bShow )
{
	CKTDGUIDialogType pkDialog = pItemSlot->GetDialog();
	if ( pkDialog == NULL )
		return;

	CKTDGUIStatic* pStaticEquipedBorder = NULL;
	if ( pkDialog->CheckControl( L"StaticEquipedBorder" ) == true )
	{
		pStaticEquipedBorder = (CKTDGUIStatic*)pkDialog->GetControl( L"StaticEquipedBorder" );
	}

	if ( pStaticEquipedBorder != NULL )
		pStaticEquipedBorder->SetShow( bShow );

	CKTDGUIStatic* pStaticEquipedBorder2 = NULL;
	if ( pkDialog->CheckControl( L"StaticEquipedBorder2" ) == true )
	{
		pStaticEquipedBorder2 = (CKTDGUIStatic*)pkDialog->GetControl( L"StaticEquipedBorder2" );
	}

	if ( pStaticEquipedBorder2 != NULL )
		pStaticEquipedBorder2->SetShow( bShow );
}

void CX2CashShop::AddChoicedItem( int itemID )
{

	if ( std::find( m_vecChoicedItemID.begin(), m_vecChoicedItemID.end(), itemID ) != m_vecChoicedItemID.end() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_32 ), g_pMain->GetNowState() );
		return;
	}

	if ( (int)m_vecChoicedItemID.size() >= CASH_SHOP_CHOICED_SLOT_NUM )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_33 ), g_pMain->GetNowState() );
		return;
	}

	vector< int > vecItemID;
	vecItemID.push_back( itemID );
	Handler_EGS_MODIFY_WISH_LIST_REQ( true, vecItemID );
}

void CX2CashShop::RemoveChoicedItem( int itemID )
{
	/*
	if ( std::find( m_vecChoicedItemID.begin(), m_vecChoicedItemID.end(), itemID ) != m_vecChoicedItemID.end() )
	{
		//일단 찾았고..
		m_vecChoicedItemID.erase( std::remove( m_vecChoicedItemID.begin(), m_vecChoicedItemID.end(), itemID ), m_vecChoicedItemID.end() );
		ResetItemSlot( CX2Slot::ST_CASH_SHOP_CHOICED, m_vecChoicedItemID );
	}
	*/

		vector< int > vecItemID;
	vecItemID.push_back( itemID );
	Handler_EGS_MODIFY_WISH_LIST_REQ( false, vecItemID );
}

void CX2CashShop::SetEnableBuyEquippingItem( bool bEnable )
{
	if ( m_pDLGFront != NULL )
	{
		CKTDGUIControl* pControl = m_pDLGFront->GetControl(L"EquippingAllBuy");
		if ( pControl != NULL )
			pControl->SetEnable( bEnable );
	}
}
#ifndef IMPOSSIBLE_PRESENT_CASH_ITEM
bool CX2CashShop::IsPossiblePresent( int itemID )
{ 
#ifdef SERV_CASHITEM_PURCHASE_RESTRICTION
	if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level <= 20)
	{
		return false;
	}
#endif SERV_CASHITEM_PURCHASE_RESTRICTION

#ifdef SERV_USE_ENABLE_GIFT
	CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );

	if(pCashItem == NULL || pCashItem->m_vecKBillProductInfo.size() < 1)
	{
		return false;
	}

	bool bEnableGift = false;

	for(std::vector<KBillProductInfo>::iterator vit = pCashItem->m_vecKBillProductInfo.begin(); vit != pCashItem->m_vecKBillProductInfo.end(); ++vit)
	{
		bEnableGift = (bEnableGift || vit->m_bEnableGift);
	}

	return bEnableGift;
#else SERV_USE_ENABLE_GIFT
	switch ( itemID )
	{ 	 
	case INVENTORY_SLOT_ADD_ITEM:	// 인벤토리 확장
	
	case SILVER_MEMBERSHIP_CARD:
	case GOLD_MEMBERSHIP_CARD:
	case EMERALD_MEMBERSHIP_CARD:
	case DIAMOND_MEMBERSHIP_CARD:
	case PLATINUM_MEMBERSHIP_CARD:
	case COBO_CREDIT_CARD:

	case CLASS_CHANGE_ELSWORD_KNIGHT_ITEM_ID:
	case CLASS_CHANGE_MAGIC_KNIGHT_ITEM_ID:
#ifdef ELSWORD_SHEATH_KNIGHT
	case CLASS_CHANGE_SHEATH_KNIGHT_ITEM_ID:
#endif ELSWORD_SHEATH_KNIGHT
	case CLASS_CHANGE_COMBAT_RANGER_ITEM_ID: 
	case CLASS_CHANGE_SNIPING_RANGER_ITEM_ID:
#ifdef SERV_TRAPPING_RANGER_TEST
	case CLASS_CHANGE_TRAPPING_RANGER_ITEM_ID:
#endif
	case CLASS_CHANGE_HIGH_MAGICIAN_ITEM_ID: 
	case CLASS_CHANGE_DARK_MAGICIAN_ITEM_ID: 
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CLASS_CHANGE_BATTLE_MAGICIAN_ITEM_ID:
#endif
	case CLASS_CHANGE_SOUL_TAKER_ITEM_ID:    
	case CLASS_CHANGE_OVER_TAKER_ITEM_ID:
#ifdef RAVEN_WEAPON_TAKER
	case CLASS_CHANGE_WEAPON_TAKER_ITEM_ID:
#endif RAVEN_WEAPON_TAKER
	case CLASS_CHANGE_EXOTIC_GEAR_ITEM_ID:   
	case CLASS_CHANGE_ARCHITECTURE_ITEM_ID:  
#ifdef EVE_ELECTRA
	case CLASS_CHANGE_ELECTRA_ITEM_ID:
#endif EVE_ELECTRA
	case CLASS_CHANGE_LORD_NIGHT_ITEM_ID:
	case CLASS_CHANGE_RUNE_SLAYER_ITEM_ID:
	case CLASS_CHANGE_WIND_SNEAKER_ITEM_ID:
	case CLASS_CHANGE_GRAND_ARCHER_ITEM_ID:
	case CLASS_CHANGE_ELEMENTAL_MASTER_ITEM_ID:
	case CLASS_CHANGE_VOID_PRINCESS_ITEM_ID:	
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_BLADE_MASTER_ITEM_ID:
	case CLASS_CHANGE_RECKLESS_FIST_ITEM_ID:
#endif

//{{ kimhc // 2010.2.17 //	이브 2차 전직 아이템 선물 불가
#ifdef	EVE_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_NEMESIS_ITEM_ID:
	case CLASS_CHANGE_EMPRESS_ITEM_ID:
#endif	EVE_SECOND_CLASS_CHANGE
//}} kimhc // 2010.2.17 //	이브 2차 전직 아이템 선물 불가

//{{ kimhc // 2011.1.26 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	case CLASS_CHANGE_FURY_GUARDIAN_ID:
	case CLASS_CHANGE_SHOOTER_GUARDIAN_ID:
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.26 // 청 1차 전직
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CLASS_CHANGE_SHELLING_GUARDIAN_ID:
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_IRON_PALADIN_ID:
	case CLASS_CHANGE_DEADLY_CHASER_ID:
#endif
		//}}

#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CLASS_CHANGE_INFINITY_SWORD_ID:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CLASS_CHANGE_DIMENSION_WITCH_ID:
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CLASS_CHANGE_NIGHT_WATCHER_ID:
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CLASS_CHANGE_VETERAN_COMMANDER_ID:
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CLASS_CHANGE_BATTLE_SERAPH_ID:
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CLASS_CHANGE_TACTICAL_TROOPER_ID:
#endif


#ifdef SERV_SKILL_NOTE
	case CASH_SKILL_NOTE_ITEM_ID:
#endif

#ifdef UNIT_CLASS_CHANGE_ITEM
#pragma region 1차
	case UNIT_CLASS_CHANGE_SWORD_KNIGHT_ITEM_ID:
	case UNIT_CLASS_CHANGE_MAGIC_KNIGHT_ITEM_ID:
	case UNIT_CLASS_CHANGE_SHEATH_KNIGHT_ITEM_ID:
	case UNIT_CLASS_CHANGE_HIGH_MAGICIAN_ITEM_ID:
	case UNIT_CLASS_CHANGE_DARK_MAGICIAN_ITEM_ID:
	case UNIT_CLASS_CHANGE_BATTLE_MAGICIAN_ITEM_ID:
	case UNIT_CLASS_CHANGE_COMBAT_RANGER_ITEM_ID:
	case UNIT_CLASS_CHANGE_SNIPING_RANGER_ITEM_ID:
	case UNIT_CLASS_CHANGE_TRAPPING_RANGER_ITEM_ID:
	case UNIT_CLASS_CHANGE_SOUL_TAKER_ITEM_ID:
	case UNIT_CLASS_CHANGE_OVER_TAKER_ITEM_ID:
	case UNIT_CLASS_CHANGE_WEAPON_TAKER_ITEM_ID:
	case UNIT_CLASS_CHANGE_EXOTIC_GEAR_ITEM_ID:
	case UNIT_CLASS_CHANGE_ARCHITECTURE_ITEM_ID:
	case UNIT_CLASS_CHANGE_ELECTRA_ITEM_ID:
	case UNIT_CLASS_CHANGE_FURY_GUARDIAN_ID:
	case UNIT_CLASS_CHANGE_SHOOTING_GUARDIAN_ID:
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case UNIT_CLASS_CHANGE_SHELLING_GUARDIAN_ID:
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case UNIT_CLASS_CHANGE_LITTLE_DEVIL_ID:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#pragma endregion
		
#pragma region 2차
	case UNIT_CLASS_CHANGE_LORD_KNIGHT_ITEM_ID:
	case UNIT_CLASS_CHANGE_RUNE_SLAYER_ITEM_ID:
	case UNIT_CLASS_CHANGE_ELEMENTAL_MASTER_ITEM_ID:
	case UNIT_CLASS_CHANGE_VOID_PRINCESS_ITEM_ID:
	case UNIT_CLASS_CHANGE_WIND_SNEAKER_ITEM_ID:
	case UNIT_CLASS_CHANGE_GRAND_ARCHER_ITEM_ID:
	case UNIT_CLASS_CHANGE_BLADE_MASTER_ITEM_ID:
	case UNIT_CLASS_CHANGE_RECKLESS_FIST_ITEM_ID:
	case UNIT_CLASS_CHANGE_NEMESIS_ITEM_ID:
	case UNIT_CLASS_CHANGE_EMPRESS_ITEM_ID:
	case UNIT_CLASS_CHANGE_IRON_PALADIN_ID:
	case UNIT_CLASS_CHANGE_DEADLY_CHASER_ID:
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case UNIT_CLASS_CHANGE_INFINITY_SWORD_ITEM_ID:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case UNIT_CLASS_CHANGE_DIMENSION_WITCH_ITEM_ID:
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case UNIT_CLASS_CHANGE_NIGHT_WATCHER_ITEM_ID:
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case UNIT_CLASS_CHANGE_VETERAN_COMMANDER_ITEM_ID:
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case UNIT_CLASS_CHANGE_BATTLE_SERAPH_ITEM_ID:
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case UNIT_CLASS_CHANGE_TACTICAL_TROOPER_ID:
#endif
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case UNIT_CLASS_CHANGE_YAMA_RAJA_ID:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case UNIT_CLASS_CHANGE_GRAND_MASTER_ID:
	case UNIT_CLASS_CHANGE_BLAZING_HEART_ID:
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#pragma endregion
#endif
		{
			return false;
		} break;

	default:
		{
			return true;
		} break;
	}
#endif SERV_USE_ENABLE_GIFT
}
#endif //IMPOSSIBLE_PRESENT_CASH_ITEM

void CX2CashShop::ProcessBuyPopUpOk()
{
#ifdef COUPON_SYSTEM
	vector< std::pair<unsigned long, int> > vecProductAndCouponNoList;
	if ( m_pBuyItemPopUp->GetCheckedProductAndCouponNoList( vecProductAndCouponNoList ) == true )
	{
		//{{ 김상훈 2010.11.1
	#ifdef REST_CASH_CHECK
		if ( m_pBuyItemPopUp->RestCashCheck() == true )
		{
			if ( Handler_EGS_BUY_CASH_ITEM_REQ( vecProductAndCouponNoList, m_pBuyItemPopUp->GetChoicedItemBuy() ) == true )
				m_pBuyItemPopUp->SetOpen( false );
		}
		else
		{
			m_pBuyItemPopUp->SetOpen( false );
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStr(NetError::ERR_BUY_CASH_ITEM_16) , g_pMain->GetNowState() );
		}
	#else
		if ( Handler_EGS_BUY_CASH_ITEM_REQ( vecProductAndCouponNoList, m_pBuyItemPopUp->GetChoicedItemBuy() ) == true )
			m_pBuyItemPopUp->SetOpen( false );
	#endif REST_CASH_CHECK

	}
#else
	vector<unsigned long> vecProductNoList;
	if ( m_pBuyItemPopUp->GetCheckedProductNoList(vecProductNoList) == true )
	{

	//{{ 김상훈 2010.11.1
	#ifdef REST_CASH_CHECK
			if ( m_pBuyItemPopUp->RestCashCheck() == true )
			{
				if ( Handler_EGS_BUY_CASH_ITEM_REQ( vecProductNoList, m_pBuyItemPopUp->GetChoicedItemBuy() ) == true )
					m_pBuyItemPopUp->SetOpen( false );
			}
			else
			{
				m_pBuyItemPopUp->SetOpen( false );
#ifdef SERV_USE_GetErrStrF
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStrF(NetError::ERR_BUY_CASH_ITEM_16) , g_pMain->GetNowState() );
#else SERV_USE_GetErrStrF
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStr(NetError::ERR_BUY_CASH_ITEM_16) , g_pMain->GetNowState() );
#endif SERV_USE_GetErrStrF
			}
	#else
			if ( Handler_EGS_BUY_CASH_ITEM_REQ( vecProductNoList, m_pBuyItemPopUp->GetChoicedItemBuy() ) == true )
				m_pBuyItemPopUp->SetOpen( false );
	#endif REST_CASH_CHECK
//}} 김상훈 2010.11.1
	}
#endif // COUPON_SYSTEM
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_7 ), g_pMain->GetNowState() );
	}
}

//{{ kimhc // 2009-10-06 // 자동결제 처리
#ifdef	AUTO_PAYMENT
void CX2CashShop::ProcessAutoPayment( int iItemID )
{
	CX2ItemManager::CashItem* pCashItem = NULL;
	pCashItem = g_pData->GetItemManager()->GetCashItem( iItemID );

	// 자동결제에서 쓰이는 프로덕트 인덱스는 무조건 0, 수량은 무조건 1 이다.
	const BYTE PRODUCT_INDEX		= 0;
	const BYTE PRODUCT_QUANTITY		= 1;

#ifndef SERV_GLOBAL_BILLING
	if ( pCashItem != NULL )
	{
		Handler_EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_REQ( pCashItem->m_vecKNXBTProductInfo[PRODUCT_INDEX].m_ulProductNo,
			PRODUCT_QUANTITY );
	}
#endif // ndef SERV_GLOBAL_BILLING


	/*	
	for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
	{
		int itemID = m_vecCheckedItem[i];

		map< int, int >::iterator mit;
		mit = m_mapSelectedProductNo.find( itemID );
		if ( mit != m_mapSelectedProductNo.end() )
		{
			if ( mit->second == 0 )
				return false;

			int productIndex = mit->second - 1;

			CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( productIndex >= 0 && (int)pCashItem->m_vecKNXBTProductInfo.size() > productIndex )
				{
					KNXBTProductInfo& kNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[productIndex];
					vecProductNoList.push_back( kNXBTProductInfo.m_ulProductNo );
				}
			}
		}
*/

}

bool CX2CashShop::Handler_EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_REQ( UINT ulProductNo, USHORT usQuantity ) const
{
	KEGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_REQ kPacket;

	kPacket.m_ulProductNo		= ulProductNo;
	kPacket.m_usOrderQuantity	= usQuantity;

	g_pData->GetServerProtocol()->SendPacket( EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK );

	return true;
}

bool CX2CashShop::Handler_EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SEND_NEXON_WISE_LOG_BY_URL // 김태환
			if( false == g_pMain->GetBrowserWrapper()->IsClosed() )	/// 켜져있으면, 끄자
				g_pMain->GetBrowserWrapper()->CloseBrowser();
#else // SEND_NEXON_WISE_LOG_BY_URL
			if( true == g_pMain->GetBrowserWrapper()->IsClosed() )
#endif // SEND_NEXON_WISE_LOG_BY_URL
			{
				if ( g_pData->GetMyUser() == NULL )
				{
					ASSERT( !L"GetMyUser is NULL" );
					return false;
				}

				if ( g_pData->GetMyUser()->GetSelectUnit() == NULL )
				{
					ASSERT( !L"GetSelectUnit is NULL" );
					return false;
				}

				const CX2User::UserData& kUserData = g_pData->GetMyUser()->GetUserData();
				const CX2Unit::UnitData& kUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
				
				std::stringstream strStreamValByPost;
				//string strValuesByPost;		// post 방식으로 넘길 값들에 대한 string을 지정
				string strTemp;				// wstr을 str로 치환할 용도
				string strURL = "https://naps.nexon.com/AutoPayment/AutoPaymentInit";
				
				int iX = 250;
				int iY = 250;
				int iWidth = 475;//490;
				int iHeight = 585;//585;

				D3DXVECTOR2 vResolution = g_pMain->GetGameOption().GetOptionList().m_vResolution;

				g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE ); 

				
				if( iWidth > vResolution.x )
					iWidth = (int)vResolution.x;
				if( iHeight > vResolution.y )
					iHeight = (int)vResolution.y;

				iX = (int) (( vResolution.x - iWidth ) * 0.5f);
				iY = (int) (( vResolution.y - iHeight ) * 0.5f);

				if( iX < 0 )
					iX = 0;
				if( iY < 0 )
					iY = 0;

				RECT rc;
				SetRect( &rc, iX, iY, iX+iWidth, iY+iHeight );
//*
				// 엘소드 채널 ID는 ELSW로 고정
				strStreamValByPost << "channelId=";
				strStreamValByPost <<	"ELSW";

				// userSn
				strStreamValByPost << "&userSn=";
				strStreamValByPost << kEvent.m_uiUserSN;

				// userId
				ConvertWCHARToChar( strTemp, kUserData.userID.c_str() );
				strStreamValByPost << "&userId=";
				strStreamValByPost << strTemp;

				// userName 은 userId로 대체
				strStreamValByPost << "&userName=";
				strStreamValByPost << strTemp;	// 이미 위에서 치환되어있음

				// userAge
				strStreamValByPost << "&userAge=";
				strStreamValByPost << kUnitData.m_Age;

				// gameId 는 userId와 동일하게
				strStreamValByPost << "&gameId=";
				strStreamValByPost << strTemp; // 이미 위에서 치환되어있음

				// masterGameid 는 지원하지 않기때문에 공백
				strStreamValByPost << "&masterGameId=";
				strStreamValByPost << " ";

				// 29로 고정
				strStreamValByPost << "&gameType=";
				strStreamValByPost << "29";

				// 도메인타입(99)
				strStreamValByPost << "&domainType=";
				strStreamValByPost << static_cast< int >( kEvent.m_byDomainType );

				// 선물은 안됨으로 0
				strStreamValByPost << "&isPresent=";
				strStreamValByPost << "0";

				// 2로 고정
				strStreamValByPost << "&reason=";
				strStreamValByPost << "2";

				// 유저의 IP
				ConvertWCHARToChar( strTemp, kUnitData.m_IP.c_str() );
				strStreamValByPost << "&requestIp=";
				strStreamValByPost << strTemp;

				// 트랜잭션ID를 char로 바꿔서 넣음
				ConvertWCHARToChar( strTemp, kEvent.m_wstrTransactionID.c_str() );
				strStreamValByPost << "&transactionId=";
				strStreamValByPost << strTemp;

				// paymentId는 13002로 고정
				strStreamValByPost << "&paymentId=";
				strStreamValByPost << "13002";

				// 자동결제 할 물품의 No
				strStreamValByPost << "&productNo=";
				strStreamValByPost << kEvent.m_ulProductNo;

				// 자동결제 할 물품의 가격
				strStreamValByPost << "&productPrice=";
				strStreamValByPost << kEvent.m_ulSalePrice;

				// 자동결제 할 물품의 수량
				strStreamValByPost << "&OrderQuantity=";
				strStreamValByPost << kEvent.m_usOrderQuantity;

				// 해시키
				CTime ctCurrentTime = CTime::GetCurrentTime();
				std::string strIN;
				strIN = "ELSW";
				
				ConvertWCHARToChar( strTemp, kUserData.userID.c_str() );
				strIN += strTemp;
				
				ConvertWCHARToChar( strTemp, static_cast< const wchar_t * >( ctCurrentTime.Format( L"%Y%m%d%I" ) ) );
				strIN += strTemp;

				strStreamValByPost << "&hashKey=";
				strStreamValByPost << KncUtil::GetMD5( strIN );


				g_pMain->GetBrowserWrapper()->Create( rc, g_pKTDXApp->GetHWND() );
				g_pMain->GetBrowserWrapper()->Navigate( strURL.c_str(), strStreamValByPost.str().c_str() );//strStreamValByPost.str().c_str() ); 

				if ( g_pInstanceData != NULL )
					g_pInstanceData->SetWebType( CSWT_CASH_AUTO_PAYMENT );
			}

			return true;
		}
	}

	return false;

}

#endif	AUTO_PAYMENT
//}} kimhc // 2009-10-06 // 자동결제 처리

//{{ kimhc // 2009-08-10 // 자신의 은행 회원 등급에 맞지 않으면 보이지 않도록 함
#ifdef	PRIVATE_BANK
bool CX2CashShop::IsPossibleUsedByMyBankMembership( int itemID ) const
{
	// 은행 아이템이 아니면 true를 리턴
	switch ( itemID )
	{
	case SILVER_MEMBERSHIP_CARD:
	case GOLD_MEMBERSHIP_CARD:
	case EMERALD_MEMBERSHIP_CARD:
	case DIAMOND_MEMBERSHIP_CARD:
	case PLATINUM_MEMBERSHIP_CARD:
	case COBO_CREDIT_CARD:
		break;

	default:
		return true;
	}

	if ( g_pData->GetMyUser()	!= NULL &&	g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		const CX2Inventory&	kInventory	= g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

		CX2Inventory::MEMBERSHIP_PRIVATE_BANK	eMembershipGrade	= CX2Inventory::MPB_NORMAL;
		eMembershipGrade	= static_cast< CX2Inventory::MEMBERSHIP_PRIVATE_BANK >( kInventory.GetItemMaxNum( CX2Inventory::ST_BANK ) );

		switch ( eMembershipGrade )
		{
		case CX2Inventory::MPB_NORMAL:
			{
				if ( itemID	== SILVER_MEMBERSHIP_CARD )
					return true;
				else if ( itemID == COBO_CREDIT_CARD 
					&&  kInventory.GetItemByTID( _CONST_UI_PRIVATE_BANK_::g_iBankQuestItemID, true ) == NULL )
					return true;
			}
			break;

		case CX2Inventory::MPB_SILVER:
			{
				if ( itemID	== GOLD_MEMBERSHIP_CARD )
					return true;
			}
			break;

		case CX2Inventory::MPB_GOLD:
			{
				if ( itemID	== EMERALD_MEMBERSHIP_CARD )
					return true;
			}
			break;

		case CX2Inventory::MPB_EMERALD:
			{
				if ( itemID	== DIAMOND_MEMBERSHIP_CARD )
					return true;
			}
			break;

		case CX2Inventory::MPB_DIAMOND:
			{
				if ( itemID	== PLATINUM_MEMBERSHIP_CARD 
					&& kInventory.GetItemByTID( _CONST_UI_PRIVATE_BANK_::g_iBankQuestItemID, true ) == NULL )
					return true;
			}
			break;

		case CX2Inventory::MPB_PLATINUM:
			{
				return false;
			}
			break;
		}		
	}
	
	return false;
}
#endif	PRIVATE_BANK
//}} kimhc // 2009-08-10 // 자신의 은행 회원 등급에 맞지 않으면 보이지 않도록 함


/*static*/ int CX2CashShop::GetClassChangeItemLevel( int itemID )
{
	switch( itemID )
	{
	case CLASS_CHANGE_ELSWORD_KNIGHT_ITEM_ID: 
	case CLASS_CHANGE_MAGIC_KNIGHT_ITEM_ID:
#ifdef ELSWORD_SHEATH_KNIGHT
	case CLASS_CHANGE_SHEATH_KNIGHT_ITEM_ID:
#endif ELSWORD_SHEATH_KNIGHT
	case CLASS_CHANGE_COMBAT_RANGER_ITEM_ID:  
	case CLASS_CHANGE_SNIPING_RANGER_ITEM_ID:
#ifdef SERV_TRAPPING_RANGER_TEST
	case CLASS_CHANGE_TRAPPING_RANGER_ITEM_ID:
#endif
	case CLASS_CHANGE_HIGH_MAGICIAN_ITEM_ID:  
	case CLASS_CHANGE_DARK_MAGICIAN_ITEM_ID:  
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CLASS_CHANGE_BATTLE_MAGICIAN_ITEM_ID:
#endif
	case CLASS_CHANGE_SOUL_TAKER_ITEM_ID:     
	case CLASS_CHANGE_OVER_TAKER_ITEM_ID:     
#ifdef RAVEN_WEAPON_TAKER
	case CLASS_CHANGE_WEAPON_TAKER_ITEM_ID:
#endif RAVEN_WEAPON_TAKER
	case CLASS_CHANGE_EXOTIC_GEAR_ITEM_ID:    
	case CLASS_CHANGE_ARCHITECTURE_ITEM_ID:
#ifdef EVE_ELECTRA
	case CLASS_CHANGE_ELECTRA_ITEM_ID:
#endif EVE_ELECTRA
		//{{ kimhc // 2011.1.26 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	case CLASS_CHANGE_FURY_GUARDIAN_ID:
	case CLASS_CHANGE_SHOOTER_GUARDIAN_ID:
#endif	CHUNG_FIRST_CLASS_CHANGE
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CLASS_CHANGE_SHELLING_GUARDIAN_ID:
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN
		//}} kimhc // 2011.1.26 // 청 1차 전직
#ifdef ARA_CHANGE_CLASS_FIRST
	case CLASS_CHANGE_LITTLE_HSIEN_ID:
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef NEW_CHARACTER_EL
	case CLASS_CHANGE_SABER_KNIGHT_ID:
	case CLASS_CHANGE_PYRO_KNIGHT_ID:
#endif // NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CLASS_CHANGE_LITTLE_DEVIL_ID:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CLASS_CHANGE_ADD_PSYCHIC_TRACER_ID:
#endif //SERV_9TH_NEW_CHARACTER
		{
			return 1;
		} break;

	case CLASS_CHANGE_LORD_NIGHT_ITEM_ID:	// 로드 나이트 임명장
	case CLASS_CHANGE_RUNE_SLAYER_ITEM_ID:	// 룬 슬레이어 징표
	case CLASS_CHANGE_WIND_SNEAKER_ITEM_ID:
	case CLASS_CHANGE_GRAND_ARCHER_ITEM_ID:
	case CLASS_CHANGE_ELEMENTAL_MASTER_ITEM_ID:
	case CLASS_CHANGE_VOID_PRINCESS_ITEM_ID:		
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_BLADE_MASTER_ITEM_ID:
	case CLASS_CHANGE_RECKLESS_FIST_ITEM_ID:
#endif
		//{{ kimhc // 2010.2.17 //	이브 2차 전직 아이템
#ifdef	EVE_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_NEMESIS_ITEM_ID:
	case CLASS_CHANGE_EMPRESS_ITEM_ID:
#endif	EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.2.17 //	이브 2차 전직 아이템
		
		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_IRON_PALADIN_ID:
	case CLASS_CHANGE_DEADLY_CHASER_ID:
#endif
		//}}
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CLASS_CHANGE_INFINITY_SWORD_ID:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CLASS_CHANGE_DIMENSION_WITCH_ID:
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CLASS_CHANGE_NIGHT_WATCHER_ID:
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CLASS_CHANGE_VETERAN_COMMANDER_ID:
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CLASS_CHANGE_BATTLE_SERAPH_ID:
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CLASS_CHANGE_TACTICAL_TROOPER_ID:
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
	case CLASS_CHANGE_SAKRA_DEVANAM_ID:
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CLASS_CHANGE_YAMA_RAJA_ID:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CLASS_CHANGE_GRAND_MASTER_ID:
	case CLASS_CHANGE_BLAZING_HEART_ID:
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CLASS_CHANGE_ADD_LUNATIC_PSYKER_ID:
#endif //SERV_ADD_LUNATIC_PSYKER
		{
			return 2;
		} break;
	}

	return -1;
}



const wstring CX2CashShop::GetClassNameByClassChangeItem( int itemID )
{
	switch( itemID )
	{
	case CLASS_CHANGE_ELSWORD_KNIGHT_ITEM_ID: 
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_KNIGHT )->m_Description;
		} break;
	case CLASS_CHANGE_MAGIC_KNIGHT_ITEM_ID:   
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_MAGIC_KNIGHT )->m_Description;
		} break;
#ifdef ELSWORD_SHEATH_KNIGHT
	case CLASS_CHANGE_SHEATH_KNIGHT_ITEM_ID: 
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_SHEATH_KNIGHT )->m_Description;
		} break;
#endif ELSWORD_SHEATH_KNIGHT
	case CLASS_CHANGE_COMBAT_RANGER_ITEM_ID:  
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_COMBAT_RANGER )->m_Description;
		} break;
	case CLASS_CHANGE_SNIPING_RANGER_ITEM_ID: 
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_SNIPING_RANGER )->m_Description;
		} break;
#ifdef SERV_TRAPPING_RANGER_TEST
	case CLASS_CHANGE_TRAPPING_RANGER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_TRAPPING_RANGER )->m_Description;
		} break;
#endif
	case CLASS_CHANGE_HIGH_MAGICIAN_ITEM_ID:  
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_HIGH_MAGICIAN )->m_Description;
		} break;
	case CLASS_CHANGE_DARK_MAGICIAN_ITEM_ID:  
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_DARK_MAGICIAN )->m_Description;
		} break;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CLASS_CHANGE_BATTLE_MAGICIAN_ITEM_ID:  
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_BATTLE_MAGICIAN )->m_Description;
		} break;		
#endif
	case CLASS_CHANGE_SOUL_TAKER_ITEM_ID:     
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_SOUL_TAKER )->m_Description;
		} break;
	case CLASS_CHANGE_OVER_TAKER_ITEM_ID:     
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_OVER_TAKER )->m_Description;
		} break;
#ifdef RAVEN_WEAPON_TAKER
	case CLASS_CHANGE_WEAPON_TAKER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_WEAPON_TAKER )->m_Description;
		} break;		
#endif RAVEN_WEAPON_TAKER
	case CLASS_CHANGE_EXOTIC_GEAR_ITEM_ID:    
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_EXOTIC_GEAR )->m_Description;
		} break;
	case CLASS_CHANGE_ARCHITECTURE_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_ARCHITECTURE )->m_Description;
		} break;
#ifdef EVE_ELECTRA
	case CLASS_CHANGE_ELECTRA_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_ELECTRA )->m_Description;
		} break;
#endif EVE_ELECTRA
	//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	case CLASS_CHANGE_FURY_GUARDIAN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_FURY_GUARDIAN )->m_Description;
		} break;

	case CLASS_CHANGE_SHOOTER_GUARDIAN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN )->m_Description;
		} break;
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.3 // 청 1차 전직

	case CLASS_CHANGE_LORD_NIGHT_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_LORD_KNIGHT )->m_Description;
		} break;

	case CLASS_CHANGE_RUNE_SLAYER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_RUNE_SLAYER )->m_Description;
		} break;


	case CLASS_CHANGE_WIND_SNEAKER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_WIND_SNEAKER )->m_Description;
		} break;

	case CLASS_CHANGE_GRAND_ARCHER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_GRAND_ARCHER )->m_Description;
		} break;

	case CLASS_CHANGE_ELEMENTAL_MASTER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_ELEMENTAL_MASTER )->m_Description;
		} break;

	case CLASS_CHANGE_VOID_PRINCESS_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_VOID_PRINCESS )->m_Description;
		} break;

#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_BLADE_MASTER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_BLADE_MASTER )->m_Description;
		} break;
	case CLASS_CHANGE_RECKLESS_FIST_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_RECKLESS_FIST )->m_Description;
		} break;
#endif

	//{{ kimhc // 2010.1.22 // 이브 2차 전직
#ifdef EVE_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_NEMESIS_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_CODE_NEMESIS )->m_Description;
		} break;

	case CLASS_CHANGE_EMPRESS_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_CODE_EMPRESS )->m_Description;
		} break;
#endif EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.1.22 // 이브 2차 전직

	//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CLASS_CHANGE_IRON_PALADIN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_IRON_PALADIN )->m_Description;
		} break;

	case CLASS_CHANGE_DEADLY_CHASER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_DEADLY_CHASER )->m_Description;
		} break;
#endif
		//}}
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CLASS_CHANGE_SHELLING_GUARDIAN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_SHELLING_GUARDIAN )->m_Description;
		} break;
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CLASS_CHANGE_INFINITY_SWORD_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_INFINITY_SWORD )->m_Description;
		} break;
#endif //SERV_ELSWORD_INFINITY_SWORD
#ifdef SERV_ARME_DIMENSION_WITCH
	case CLASS_CHANGE_DIMENSION_WITCH_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_DIMENSION_WITCH )->m_Description;
		} break;
#endif //SERV_ARME_DIMENSION_WITCH
#ifdef SERV_RENA_NIGHT_WATCHER
	case CLASS_CHANGE_NIGHT_WATCHER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_NIGHT_WATCHER)->m_Description;
		} break;
#endif //SERV_RENA_NIGHT_WATCHER
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CLASS_CHANGE_VETERAN_COMMANDER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_VETERAN_COMMANDER)->m_Description;
		} break;
#endif //SERV_RAVEN_VETERAN_COMMANDER
#ifdef SERV_EVE_BATTLE_SERAPH
	case CLASS_CHANGE_BATTLE_SERAPH_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_BATTLE_SERAPH )->m_Description;
		} break;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CLASS_CHANGE_TACTICAL_TROOPER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_TACTICAL_TROOPER)->m_Description;
		} break;
#endif //SERV_CHUNG_TACTICAL_TROOPER
		//{{ 2013. 1. 22	박세훈	소선, 제천 전직 변경권
#ifdef ARA_CHANGE_CLASS_FIRST
	case CLASS_CHANGE_LITTLE_HSIEN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARA_LITTLE_HSIEN)->m_Description;
		} break;
	
	case CLASS_CHANGE_SAKRA_DEVANAM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARA_SAKRA_DEVANAM)->m_Description;
		} break;
#endif ARA_CHANGE_CLASS_FIRST
		//}}
#ifdef NEW_CHARACTER_EL
	case CLASS_CHANGE_SABER_KNIGHT_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELESIS_SABER_KNIGHT)->m_Description;
		} break;

	case CLASS_CHANGE_PYRO_KNIGHT_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELESIS_PYRO_KNIGHT)->m_Description;
		} break;
#endif // NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CLASS_CHANGE_LITTLE_DEVIL_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARA_LITTLE_DEVIL)->m_Description;
		} break;

	case CLASS_CHANGE_YAMA_RAJA_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARA_YAMA_RAJA)->m_Description;
		} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CLASS_CHANGE_GRAND_MASTER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELESIS_GRAND_MASTER )->m_Description;
		} break;
		
	case CLASS_CHANGE_BLAZING_HEART_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELESIS_BLAZING_HEART )->m_Description;
		} break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CLASS_CHANGE_ADD_PSYCHIC_TRACER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ADD_PSYCHIC_TRACER )->m_Description;
		} break;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CLASS_CHANGE_ADD_LUNATIC_PSYKER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ADD_LUNATIC_PSYKER )->m_Description;
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return L"";
}

#ifdef SERV_SKILL_NOTE
bool CX2CashShop::GetCanByeSkillNoteItem( int itemID )
{
	if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( SKILL_NOTE_ITEM_ID, true ) != NULL )
	{
		return false;
	}

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&			
		g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot() > 0 )
	{
		return false;
	}

	return true;;
}
#endif

#ifdef UNIT_CLASS_CHANGE_ITEM
/** @function 	: GetUnitClassChangeItemLevel
	@brief 		: 전직 변경 아이템인지 확인, 1/2차 확인
	@param		: Item Identify
	@return		: 1차 전직 : 1, 2차 전직 : 2
*/
int CX2CashShop::GetUnitClassChangeItemLevel( int itemID )
{
	switch( itemID )
	{
	case UNIT_CLASS_CHANGE_SWORD_KNIGHT_ITEM_ID:
	case UNIT_CLASS_CHANGE_MAGIC_KNIGHT_ITEM_ID:
	case UNIT_CLASS_CHANGE_SHEATH_KNIGHT_ITEM_ID:
	case UNIT_CLASS_CHANGE_HIGH_MAGICIAN_ITEM_ID:
	case UNIT_CLASS_CHANGE_DARK_MAGICIAN_ITEM_ID:
	case UNIT_CLASS_CHANGE_BATTLE_MAGICIAN_ITEM_ID:
	case UNIT_CLASS_CHANGE_COMBAT_RANGER_ITEM_ID:
	case UNIT_CLASS_CHANGE_SNIPING_RANGER_ITEM_ID:
	case UNIT_CLASS_CHANGE_TRAPPING_RANGER_ITEM_ID:
	case UNIT_CLASS_CHANGE_SOUL_TAKER_ITEM_ID:
	case UNIT_CLASS_CHANGE_OVER_TAKER_ITEM_ID:
	case UNIT_CLASS_CHANGE_WEAPON_TAKER_ITEM_ID:
	case UNIT_CLASS_CHANGE_EXOTIC_GEAR_ITEM_ID:
	case UNIT_CLASS_CHANGE_ARCHITECTURE_ITEM_ID:
	case UNIT_CLASS_CHANGE_ELECTRA_ITEM_ID:
	case UNIT_CLASS_CHANGE_FURY_GUARDIAN_ID:
	case UNIT_CLASS_CHANGE_SHOOTING_GUARDIAN_ID:
	case UNIT_CLASS_CHANGE_SHELLING_GUARDIAN_ID:
	case UNIT_CLASS_CHANGE_SABER_KNIGHT_ID:/// 엘리시스 세이버 나이트
	case UNIT_CLASS_CHANGE_PYRO_KNIGHT_ID: /// 엘리시스 파이로 나이트
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case UNIT_CLASS_CHANGE_LITTLE_HSIEN_ID:
	case UNIT_CLASS_CHANGE_LITTLE_DEVIL_ID:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
		{
			return 1;
		} break;

	case UNIT_CLASS_CHANGE_LORD_KNIGHT_ITEM_ID:
	case UNIT_CLASS_CHANGE_RUNE_SLAYER_ITEM_ID:
	case UNIT_CLASS_CHANGE_ELEMENTAL_MASTER_ITEM_ID:
	case UNIT_CLASS_CHANGE_VOID_PRINCESS_ITEM_ID:
	case UNIT_CLASS_CHANGE_WIND_SNEAKER_ITEM_ID:
	case UNIT_CLASS_CHANGE_GRAND_ARCHER_ITEM_ID:
	case UNIT_CLASS_CHANGE_BLADE_MASTER_ITEM_ID:
	case UNIT_CLASS_CHANGE_RECKLESS_FIST_ITEM_ID:
	case UNIT_CLASS_CHANGE_NEMESIS_ITEM_ID:
	case UNIT_CLASS_CHANGE_EMPRESS_ITEM_ID:
	case UNIT_CLASS_CHANGE_IRON_PALADIN_ID:
	case UNIT_CLASS_CHANGE_DEADLY_CHASER_ID:
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case UNIT_CLASS_CHANGE_INFINITY_SWORD_ITEM_ID:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case UNIT_CLASS_CHANGE_DIMENSION_WITCH_ITEM_ID:
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case UNIT_CLASS_CHANGE_NIGHT_WATCHER_ITEM_ID:
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case UNIT_CLASS_CHANGE_VETERAN_COMMANDER_ITEM_ID:
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case UNIT_CLASS_CHANGE_BATTLE_SERAPH_ITEM_ID:
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case UNIT_CLASS_CHANGE_TACTICAL_TROOPER_ID:
#endif
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case UNIT_CLASS_CHANGE_SAKRA_DEVANAM_ID:
	case UNIT_CLASS_CHANGE_YAMA_RAJA_ID:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case UNIT_CLASS_CHANGE_GRAND_MASTER_ID:
	case UNIT_CLASS_CHANGE_BLAZING_HEART_ID:
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
		{
			return 2;
		} break;
	}

	return -1;
}

/** @function 	: GetClassNameByUnitClassChangeItem
	@brief 		: 
	@param		: 
	@return		: 
*/
const wstring CX2CashShop::GetClassNameByUnitClassChangeItem( int itemID )
{
	switch( itemID )
	{
#pragma region 1차 전직
	case UNIT_CLASS_CHANGE_SWORD_KNIGHT_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_KNIGHT )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_MAGIC_KNIGHT_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_MAGIC_KNIGHT )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_SHEATH_KNIGHT_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_SHEATH_KNIGHT )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_HIGH_MAGICIAN_ITEM_ID:  
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_HIGH_MAGICIAN )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_DARK_MAGICIAN_ITEM_ID:  
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_DARK_MAGICIAN )->m_Description;
		} break;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case UNIT_CLASS_CHANGE_BATTLE_MAGICIAN_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_BATTLE_MAGICIAN )->m_Description;
		} break;
#endif
	case UNIT_CLASS_CHANGE_COMBAT_RANGER_ITEM_ID:  
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_COMBAT_RANGER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_SNIPING_RANGER_ITEM_ID: 
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_SNIPING_RANGER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_TRAPPING_RANGER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_TRAPPING_RANGER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_SOUL_TAKER_ITEM_ID:     
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_SOUL_TAKER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_OVER_TAKER_ITEM_ID:     
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_OVER_TAKER )->m_Description;
		} break;
#ifdef RAVEN_WEAPON_TAKER
	case UNIT_CLASS_CHANGE_WEAPON_TAKER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_WEAPON_TAKER )->m_Description;
		} break;
#endif RAVEN_WEAPON_TAKER
	case UNIT_CLASS_CHANGE_EXOTIC_GEAR_ITEM_ID:    
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_EXOTIC_GEAR )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_ARCHITECTURE_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_ARCHITECTURE )->m_Description;
		} break;
#ifdef EVE_ELECTRA
	case UNIT_CLASS_CHANGE_ELECTRA_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_ELECTRA )->m_Description;
		} break;
#endif
	case UNIT_CLASS_CHANGE_FURY_GUARDIAN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_FURY_GUARDIAN )->m_Description;
		} break;

	case UNIT_CLASS_CHANGE_SHOOTING_GUARDIAN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN )->m_Description;
		} break;
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case UNIT_CLASS_CHANGE_SHELLING_GUARDIAN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_SHELLING_GUARDIAN )->m_Description;
		} break;
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case UNIT_CLASS_CHANGE_SABER_KNIGHT_ID:/// 엘리시스 세이버 나이트
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELESIS_SABER_KNIGHT )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_PYRO_KNIGHT_ID: /// 엘리시스 파이로 나이트
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELESIS_PYRO_KNIGHT)->m_Description;
		} break;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case UNIT_CLASS_CHANGE_LITTLE_HSIEN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARA_LITTLE_HSIEN )->m_Description;
		} break;

	case UNIT_CLASS_CHANGE_LITTLE_DEVIL_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARA_LITTLE_DEVIL )->m_Description;
		} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#pragma endregion

#pragma region 2차 전직
	case UNIT_CLASS_CHANGE_LORD_KNIGHT_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_LORD_KNIGHT )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_RUNE_SLAYER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_RUNE_SLAYER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_ELEMENTAL_MASTER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_ELEMENTAL_MASTER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_VOID_PRINCESS_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_VOID_PRINCESS )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_WIND_SNEAKER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_WIND_SNEAKER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_GRAND_ARCHER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_GRAND_ARCHER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_BLADE_MASTER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_BLADE_MASTER )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_RECKLESS_FIST_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_RECKLESS_FIST )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_NEMESIS_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_CODE_NEMESIS )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_EMPRESS_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_CODE_EMPRESS )->m_Description;
		} break;
#ifdef CHUNG_SECOND_CLASS_CHANGE
	case UNIT_CLASS_CHANGE_IRON_PALADIN_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_IRON_PALADIN )->m_Description;
		} break;
	case UNIT_CLASS_CHANGE_DEADLY_CHASER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_DEADLY_CHASER )->m_Description;
		} break;
#endif
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case UNIT_CLASS_CHANGE_INFINITY_SWORD_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELSWORD_INFINITY_SWORD )->m_Description;
		} break;
#endif //SERV_ELSWORD_INFINITY_SWORD
#ifdef SERV_ARME_DIMENSION_WITCH
	case UNIT_CLASS_CHANGE_DIMENSION_WITCH_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARME_DIMENSION_WITCH )->m_Description;
		} break;
#endif //SERV_ARME_DIMENSION_WITCH
#ifdef SERV_RENA_NIGHT_WATCHER
	case UNIT_CLASS_CHANGE_NIGHT_WATCHER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_LIRE_NIGHT_WATCHER )->m_Description;
		} break;
#endif //SERV_RENA_NIGHT_WATCHER
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case UNIT_CLASS_CHANGE_VETERAN_COMMANDER_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_RAVEN_VETERAN_COMMANDER )->m_Description;
		} break;
#endif //SERV_RAVEN_VETERAN_COMMANDER
#ifdef SERV_EVE_BATTLE_SERAPH
	case UNIT_CLASS_CHANGE_BATTLE_SERAPH_ITEM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_EVE_BATTLE_SERAPH )->m_Description;
		} break;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case UNIT_CLASS_CHANGE_TACTICAL_TROOPER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_CHUNG_TACTICAL_TROOPER )->m_Description;
		} break;
#endif
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case UNIT_CLASS_CHANGE_SAKRA_DEVANAM_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARA_SAKRA_DEVANAM )->m_Description;
		} break;

	case UNIT_CLASS_CHANGE_YAMA_RAJA_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ARA_YAMA_RAJA )->m_Description;
		} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case UNIT_CLASS_CHANGE_GRAND_MASTER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELESIS_GRAND_MASTER )->m_Description;
		} break;

	case UNIT_CLASS_CHANGE_BLAZING_HEART_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ELESIS_BLAZING_HEART )->m_Description;
		} break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case UNIT_CLASS_CHANGE_LUNATIC_PSYKER_ID:
		{
			return g_pData->GetUnitManager()->GetUnitTemplet( CX2Unit::UC_ADD_LUNATIC_PSYKER )->m_Description;
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
#pragma endregion
	}

	return L"";
}
#endif

/** @function : SetOpenNexonCashChargePage
	@brief : 넥슨 캐시 충전 웹페이지
	@param : strURL_( 웹페이지 주소 ), rcWebPage_( 웹페이지 사이즈 )
*/
void CX2CashShop::SetOpenNexonCashChargePage( OUT string& strURL_, OUT RECT& rcWebPage_ )
{
	strURL_ = "https://nxpay.nexon.com/cash/main.aspx?CHID=ELSW&NXID=";

	string strUserID;

	ConvertWCHARToChar( strUserID, g_pData->GetMyUser()->GetUserData().userID.c_str() );

	strURL_ += strUserID;

	string strCtype = "&CTYPE=1&HKEY=";

	strURL_ += strCtype;

	// 해시키 ///////////////////
	std::string strSource;
	std::string strTemp;	
	strSource = "ELSW";

	strSource += strUserID;

	// oasis907 : 김상윤 [2011.1.10] // 서버 시간 가지고 오게 수정
	CTime ctCurrentTime( g_pData->GetServerCurrentTime() );

	ConvertWCHARToChar( strTemp, static_cast< const wchar_t * >( ctCurrentTime.Format( L"%Y%m%d%I" ) ) );
	strSource += strTemp;

	string strMD5 = KncUtil::GetMD5( strSource );


	strURL_ += strMD5;
	////////////////////////////////

	rcWebPage_.left = 250;
	rcWebPage_.top	= 250;
	rcWebPage_.right	= 538;
	rcWebPage_.bottom	= 530;
}

//{{ kimhc // 2011-08-10 // 채널링 SOAP
#ifdef	SERV_TOONILAND_CHANNELING

/** @function : SetOpenTooniCashCargePage
	@brief : 투니 캐시 충전 웹페이지
	@param : strURL_( 웹페이지 주소 ), rcWebPage_( 웹페이지 사이즈 )
*/
void CX2CashShop::SetOpenTooniCashChargePage( OUT string& strURL_, OUT RECT& rcWebPage_ )	
{
	// 아직 투니랜드 아이디를 서버로 부터 받지 못했다면...
	if ( false == g_pData->GetMyUser()->GetUserData().m_wstrTooniLandID.empty() )
	{
		strURL_ = "https://cert.tooniland.com/billing/INIsecureNewStart.tl?type=etc&price=1000&userId=";
		
		string strUserID;
		ConvertWCHARToChar( strUserID, g_pData->GetMyUser()->GetUserData().m_wstrTooniLandID );
		strURL_ += strUserID;
	}
	// 투니랜드 아이디를 서버로 부터 받았다면
	else
	{
		strURL_ = "https://member.tooniland.com/membersMember/checkIdPopup2.tl";
	}
	
	rcWebPage_.left = 250;
	rcWebPage_.top	= 250;
	rcWebPage_.right	= 530;
	rcWebPage_.bottom	= 340;
}

#endif	SERV_TOONILAND_CHANNELING
//}} kimhc // 2011-08-10 // 채널링 SOAP

//{{ 오현빈 // 2012-04-09 // 실명인증
#ifdef SERV_NEXON_AUTH_SOAP
void CX2CashShop::SetOpenCheckAuthority( OUT string& strURL_, OUT RECT& rcWebPage_ )
{
	strURL_ = "https://member.nexon.com/verify/realname.aspx";

	rcWebPage_.left = 250;
	rcWebPage_.top	= 250;
	rcWebPage_.right	= 538;
	rcWebPage_.bottom	= 530;
}
#endif //SERV_NEXON_AUTH_SOAP
//}} 오현빈 // 2012-04-09 // 실명인증

#ifdef	SERV_EXPAND_QUICK_SLOT
bool CX2CashShop::GetCahBuyExpandQuickSlotItem( int itemID )
{
	if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( EXPAND_QUICK_SLOT_ITEM_ID, true ) != NULL )
	{
		return false;
	}

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&			
		g_pData->GetMyUser()->GetSelectUnit()->IsExpandQuickSlot() == true )
	{
		return false;
	}

	return true;
}
#endif  SERV_EXPAND_QUICK_SLOT
CX2CashShop::CashShopSlot::CashShopSlot( CX2Slot::SLOT_TYPE cashShopSlotType, int slotIndex )
{
	m_ItemID			= 0;
	m_pDLGCashShopSlot	= NULL;

	m_SlotType = cashShopSlotType;
	m_SlotIndex = slotIndex;

	//{{ 2009.01.19 김태완 : 코드정리
	switch(cashShopSlotType)
	{
	case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
		{
			m_pDLGCashShopSlot = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Mainpage_Itemslot.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCashShopSlot );
			m_pDLGCashShopSlot->SetShowEnable( false, false );
			
			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(373,138,0) );
			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(582,138,0) );
			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(797,138,0) );

		} break;
	case CX2Slot::ST_CASH_SHOP_MAIN_HOT:
		{
			m_pDLGCashShopSlot = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Mainpage_Itemslot.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCashShopSlot );
			m_pDLGCashShopSlot->SetShowEnable( false, false );

			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(373,303,0) );
			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(582,303,0) );
			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(797,303,0) );

		} break;
	case CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND:
		{
			m_pDLGCashShopSlot = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Mainpage_Itemslot.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCashShopSlot );
			m_pDLGCashShopSlot->SetShowEnable( false, false );

			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(373,469,0) );
			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(582,469,0) );
			m_pDLGCashShopSlot->AddDummyPos( D3DXVECTOR3(797,469,0) );
			
		} break;
	case CX2Slot::ST_CASH_SHOP_SUB:
		{
			m_pDLGCashShopSlot = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Subpage_Itemslot.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCashShopSlot );
		} break;
	case CX2Slot::ST_CASH_SHOP_SUB_SPECIAL:
		{
			m_pDLGCashShopSlot = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Subpage_Hot_Itemslot.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCashShopSlot );
		} break;
	default:
		break;
	}
	//}}

}

CX2CashShop::CashShopSlot::~CashShopSlot()
{
	if ( m_pDLGCashShopSlot != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCashShopSlot, NULL, false );

	m_pDLGCashShopSlot = NULL;
}

void CX2CashShop::CashShopSlot::SetShow( bool bShow )
{
	if ( m_pDLGCashShopSlot != NULL )
		m_pDLGCashShopSlot->SetShowEnable( bShow, bShow );
}

//{{ kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
//#ifdef	AUTO_PAYMENT
void CX2CashShop::CashShopSlot::SetInfo( bool bAutoPayment /*= false*/ )
//#else	AUTO_PAYMENT
//void CX2CashShop::CashShopSlot::SetInfo()
//#endif	AUTO_PAYMENT
//}} kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
{
	if ( m_pDLGCashShopSlot == NULL )
		return;

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_ItemID );
	if ( pItemTemplet == NULL )
		return;

	CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( m_ItemID );
	if ( pCashItem == NULL )
		return;

	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGCashShopSlot->GetControl(L"g_pStaticItemName");
	if ( pStaticName != NULL && pStaticName->GetString(0) != NULL )
	{
		//{{ 2009.01.19 김태완 : 코드정리
		switch(m_SlotType)
		{
		case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
		case CX2Slot::ST_CASH_SHOP_MAIN_HOT:
		case CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND:
			{
#if defined ( CLIENT_COUNTRY_US )
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 17 );
#elif defined ( UNIQUENESS_EU_ONLY )
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 20 );
#elif defined ( CASHSHOP_ITEM_NAME_LONGER )
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 20 );								
#elif defined ( CLIENT_COUNTRY_CN )
#ifdef ELLIPSE_FIX
				pStaticName->GetString(0)->msg = CWordLineHandler::CutStringWithEllipse(pItemTemplet->GetName(), 110, pStaticName->GetString(0)->fontIndex, 1, L"..");
#else // ELLIPSE_FIX
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 9 );
#endif // ELLIPSE_FIX
#else
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 10 );
#endif
			}break;
		case CX2Slot::ST_CASH_SHOP_SUB:
			{
#if defined ( CLIENT_COUNTRY_US )
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 24 );
#elif defined ( UNIQUENESS_EU_ONLY )
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 27 );				
#elif defined ( CASHSHOP_ITEM_NAME_LONGER )
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 27 );				
#elif defined ( CLIENT_COUNTRY_CN )
#ifdef ELLIPSE_FIX
				pStaticName->GetString(0)->msg = CWordLineHandler::CutStringWithEllipse(pItemTemplet->GetName(), 150, pStaticName->GetString(0)->fontIndex, 1, L"..");
#else // ELLIPSE_FIX
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 13 );
#endif // ELLIPSE_FIX
#else
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 14 );
#endif
			}break;
		case CX2Slot::ST_CASH_SHOP_SUB_SPECIAL:
			{
#if defined ( CLIENT_COUNTRY_US )
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 16 );
#elif defined ( UNIQUENESS_EU_ONLY )				
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 19 );
#elif defined ( CASHSHOP_ITEM_NAME_LONGER )
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 19 );
#elif defined ( CLIENT_COUNTRY_CN )
#ifdef ELLIPSE_FIX
				pStaticName->GetString(0)->msg = CWordLineHandler::CutStringWithEllipse(pItemTemplet->GetName(), 95, pStaticName->GetString(0)->fontIndex, 1, L"..");
#else // ELLIPSE_FIX
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 8 );
#endif // ELLIPSE_FIX
#else
				pStaticName->GetString(0)->msg = GetCutString( pItemTemplet->GetName(), 10 );
#endif
			}break;
		default:
			{
                pStaticName->GetString(0)->msg = pItemTemplet->GetName();
			}break;
		}
		//}}
	}

	CKTDGUIStatic* pStaticPrice = (CKTDGUIStatic*)m_pDLGCashShopSlot->GetControl(L"g_pStaticCashNum");
	if ( pStaticPrice != NULL && pStaticPrice->GetString(0) != NULL )
	{
		//{{ kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
#ifdef	AUTO_PAYMENT
		
		if ( bAutoPayment == false )
			pStaticPrice->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_38, "L", g_pMain->GetEDString( pCashItem->GetCash() ) ) );
		else
			pStaticPrice->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_4606, "L", g_pMain->GetEDString( pCashItem->GetCash() ) ) );
#else	AUTO_PAYMENT
		pStaticPrice->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_38, "L", g_pMain->GetEDString( pCashItem->GetCash() ) ) );
#endif	AUTO_PAYMENT
		//}} kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP	// 아이템 가격이 제일 싼 가격부터 표시되서 인증서 없으면 2000엔부터 표시하도록
		 // 같은 아이템 2종류 팔 때
		/*const KBillProductInfo* pkKBillProductInfo = pCashItem->GetGateProduct();
		if ( pkKBillProductInfo != NULL && m_ItemID == ITEM_ID_EVENT_NEW_SKILL_NOTE &&
			NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( ITEM_ID_EVENT_NEW_SPECIAL ) )
		{
			pStaticPrice->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_38, "L", g_pMain->GetEDString( 3000 ) ) );
		}*/
		const KBillProductInfo* pkKBillProductInfo = pCashItem->GetGateProduct();
		std::vector<int> TempList;
		int TempProDuctNo = 0;
		if( pkKBillProductInfo != NULL && (g_pData->GetItemManager()->DisCountItemID_Find(m_ItemID,TempList,TempProDuctNo) == true) )
		{
			for(int i = 0; i < TempList.size(); ++i)
			{
				if( NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( TempList[i] ) )
				{
					int nPrice = g_pData->GetItemManager()->GetChangeCashPoint(TempProDuctNo);
					pStaticPrice->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_38, "L", g_pMain->GetEDString( nPrice ) ) );
					break;
				}
			}
		}
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
	}

	if ( m_SlotType == CX2Slot::ST_CASH_SHOP_SUB_SPECIAL )
	{
		CKTDGUIStatic* pStaticPrice = (CKTDGUIStatic*)m_pDLGCashShopSlot->GetControl(L"g_pStaticHotitem_Slot_Num");
		if ( pStaticPrice != NULL  )
		{
			for ( int i = 0; i < 5; i++ )
			{
				if ( pStaticPrice->GetPicture(i) != NULL )
					pStaticPrice->GetPicture(i)->SetShow( false );
			}

			if ( pStaticPrice->GetPicture(m_SlotIndex) != NULL )
				pStaticPrice->GetPicture( m_SlotIndex )->SetShow( true );
		}
	}
}

wstring CX2CashShop::CashShopSlot::GetCutString( const WCHAR* wszItemName, int maxItemNameLen )
{
	wstring tempName = wszItemName;
	if ( (int)tempName.length() > maxItemNameLen )
	{
		tempName = tempName.substr( 0, maxItemNameLen - 2 );
		tempName += L"..";
	}

	return tempName;
}

//{{ kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
#ifdef	AUTO_PAYMENT
void CX2CashShop::CashShopSlot::SetShowPresentAndCartButton( bool bShow ) // 선물, 찜 버튼
{
	CKTDGUIButton* pPresentButton	= NULL;
	CKTDGUIButton* pCartButton		= NULL;

	pPresentButton	= static_cast< CKTDGUIButton* >( m_pDLGCashShopSlot->GetControl( L"Present" ) );
	pCartButton		= static_cast< CKTDGUIButton* >( m_pDLGCashShopSlot->GetControl( L"Cart" ) );

	if ( pPresentButton != NULL )
		pPresentButton->SetShowEnable( bShow, bShow );

	if ( pCartButton != NULL )
		pCartButton->SetShowEnable( bShow, bShow );
	
}
#endif	AUTO_PAYMENT
//}} kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가

CX2CashShop::CashShopSlotManager::CashShopSlotManager( CX2Slot::SLOT_TYPE cashShopSlotType )
{
	m_NowPage = 1;
	m_MaxPage = 1;

	//{{ 2009.01.19 김태완 : 코드정리
	switch(cashShopSlotType)
	{
	case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
	case CX2Slot::ST_CASH_SHOP_MAIN_HOT:
	case CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND:
		{
			m_MaxSlotNumPerPage = CASH_SHOP_MAIN_SLOT_NUM;
		} break;
	case CX2Slot::ST_CASH_SHOP_SUB:
		{
			m_MaxSlotNumPerPage = 10;
		} break;
	case CX2Slot::ST_CASH_SHOP_SUB_SPECIAL:
		{
			m_MaxSlotNumPerPage = 5;
		} break;
	default:
		{
			ASSERT( !"Unexpected CashShop Slot Type" );
			m_MaxSlotNumPerPage = CASH_SHOP_MAIN_SLOT_NUM;
		}
		break;
	}
	//}}

	for ( int i = 0; i < m_MaxSlotNumPerPage; i++ )
	{
		CX2CashShop::CashShopSlot* pCashShopSlot = new CX2CashShop::CashShopSlot( cashShopSlotType, i );
		if ( pCashShopSlot->m_pDLGCashShopSlot != NULL )
			pCashShopSlot->m_pDLGCashShopSlot->SetPos( D3DXVECTOR2( pCashShopSlot->m_pDLGCashShopSlot->GetDummyPos(i).x, pCashShopSlot->m_pDLGCashShopSlot->GetDummyPos(i).y ) );
		m_vecCashShopSlot.push_back( pCashShopSlot );
	}
	m_bOpen = false;
}
CX2CashShop::CashShopSlotManager::~CashShopSlotManager()
{
	for ( int i = 0; i < (int)m_vecCashShopSlot.size(); i++ )
	{
		CX2CashShop::CashShopSlot* pCashShopSlot = m_vecCashShopSlot[i];
		SAFE_DELETE( pCashShopSlot );
	}
	m_vecCashShopSlot.clear();
}

bool CX2CashShop::CashShopSlotManager::IsPossibleUsedByMyBankMembership( int itemID ) const
{
	// 은행 아이템이 아니면 true를 리턴
	switch ( itemID )
	{
	case SILVER_MEMBERSHIP_CARD:
	case GOLD_MEMBERSHIP_CARD:
	case EMERALD_MEMBERSHIP_CARD:
	case DIAMOND_MEMBERSHIP_CARD:
	case PLATINUM_MEMBERSHIP_CARD:
	case COBO_CREDIT_CARD:
		break;

	default:
		return true;
	}

	if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		const CX2Inventory&	kInventory	= g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

		CX2Inventory::MEMBERSHIP_PRIVATE_BANK	eMembershipGrade	= CX2Inventory::MPB_NORMAL;
		eMembershipGrade	= static_cast< CX2Inventory::MEMBERSHIP_PRIVATE_BANK >( kInventory.GetItemMaxNum( CX2Inventory::ST_BANK ) );

		switch ( eMembershipGrade )
		{
		case CX2Inventory::MPB_NORMAL:
			{
				if ( itemID	== SILVER_MEMBERSHIP_CARD )
					return true;
				else if ( itemID == COBO_CREDIT_CARD 
					&&  kInventory.GetItemByTID( _CONST_UI_PRIVATE_BANK_::g_iBankQuestItemID, true ) == NULL )
					return true;
			}
			break;

		case CX2Inventory::MPB_SILVER:
			{
				if ( itemID	== GOLD_MEMBERSHIP_CARD )
					return true;
			}
			break;

		case CX2Inventory::MPB_GOLD:
			{
				if ( itemID	== EMERALD_MEMBERSHIP_CARD )
					return true;
			}
			break;

		case CX2Inventory::MPB_EMERALD:
			{
				if ( itemID	== DIAMOND_MEMBERSHIP_CARD )
					return true;
			}
			break;

		case CX2Inventory::MPB_DIAMOND:
			{
				if ( itemID	== PLATINUM_MEMBERSHIP_CARD 
					&& kInventory.GetItemByTID( _CONST_UI_PRIVATE_BANK_::g_iBankQuestItemID, true ) == NULL )
					return true;
			}
			break;

		case CX2Inventory::MPB_PLATINUM:
			{
				return false;
			}
			break;
		}		
	}

	return false;
}




// bPossibleItemOnly : 내가 사용할 수 있는 아이템만 보이게 할지
void CX2CashShop::CashShopSlotManager::Reset( vector<int>& vecItemID, bool bAutoPayment /* = false */, bool bPossibleItemOnly /*= false*/ )
{
	m_NowPage = 1;

//	//{{ kimhc // 2009-08-12 // 은행 메인메뉴에서 살수있는 회원권만 보이도록
//#ifdef	PRIVATE_BANK
	m_vecItemID.resize( 0 );
	for ( UINT i = 0; i < vecItemID.size(); i++ )
	{
		if ( IsPossibleUsedByMyBankMembership( vecItemID[i] ) == false )
			continue;

		if( true == bPossibleItemOnly )
		{
			if( CX2CashShop::IsPossibleUsedByMyCharacter( vecItemID[i] ) == false )
				continue;

			int iClassChangeItemLevel = GetClassChangeItemLevel( vecItemID[i] );
			if( iClassChangeItemLevel > 0 &&															// 전직 캐시 아이템인데
				1 != iClassChangeItemLevel - g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() )	// 현재 캐릭터 클래스가 사용할 수 없는 아이템이면
				continue;

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
			if( vecItemID[i] == UNLIMITED_SECOND_CHANGE_JOB_ITEM_ID
				&& 2 != g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() )
				continue;																				//2차 전직 아니면 보이지 않도록 함				
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef PACKAGEITEM_SET_NOT_EQUIP_ITEM
			int itemIDPackage = vecItemID[i];

			const CX2Item::ItemTemplet *pItemTempletPackage = g_pData->GetItemManager()->GetItemTemplet( itemIDPackage );
			if( pItemTempletPackage == NULL )
				continue;

			if( g_pData == NULL ||
				g_pData->GetMyUser() == NULL ||
				g_pData->GetMyUser()->GetSelectUnit() == NULL ||
				g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet() == NULL)
				continue;

			vector< CX2ItemManager::PackageItemData > vecCheckPackageItemData;

			if ( pItemTempletPackage->GetItemType() == CX2Item::IT_SPECIAL			// 스페셜 타입이고,
				&& pItemTempletPackage->GetUnitType() != g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType // unitType 관련해서 내것이 아닌 경우
				&& g_pData->GetItemManager()->GetPackageData(pItemTempletPackage->GetItemID(), vecCheckPackageItemData)
				)
			{
				if (pItemTempletPackage->GetUnitType() != CX2Unit::UNIT_TYPE::UT_NONE)
					continue;
			}
#endif PACKAGEITEM_SET_NOT_EQUIP_ITEM

#ifdef SERV_SKILL_NOTE
			if( vecItemID[i] == CASH_SKILL_NOTE_ITEM_ID && GetCanByeSkillNoteItem( vecItemID[i] ) == false )
			{
				continue;
			}
#endif

#ifdef SERV_BUY_EXCEPTION_ARA
			if( g_pData->GetMyUser()->GetSelectUnit()->GetType() == CX2Unit::UT_ARA && ( vecItemID[i] == 60007760 || vecItemID[i] == 60007761 || vecItemID[i] == 60007762 || vecItemID[i] == 60007763 ) )
				continue;
#endif SERV_BUY_EXCEPTION_ARA

#ifdef SERV_SHARING_BANK_QUEST_CASH
			if( vecItemID[i] == SHARING_BANK_OPEN_CASH_ITEM_ID && GetCahBuySharingBankItem( vecItemID[i] ) == false )
			{
				continue;
			}
#endif

			//{{ 허상형 : [2012/12/5] //		특정 아이템 가지고 있어야 캐시샵에서 구매 가능
#ifdef	SERV_KEEP_ITEM_SHOW_CASHSHOP
			if( g_pData != NULL )
			{
				if( g_pData->GetItemManager() != NULL )
				{
					bool bContinue = false;
					std::vector<int> TempItemID;
					TempItemID.clear();
					if( g_pData->GetItemManager()->CheckKeepShowItem( vecItemID[i] , TempItemID ) == true )
					{
						for( int i = 0; i < TempItemID.size(); ++i )
						{
							if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( TempItemID[i] ) == NULL )
							{
								bContinue = true;
								break;
							}
						}
						if( bContinue == true)
						{
							continue;
						}
					}
				}
			}
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM
			if( (vecItemID[i] == 60007354 || vecItemID[i] == 60007355 || vecItemID[i] == 60007356) && CX2Unit::UC_ARA_LITTLE_HSIEN != g_pData->GetMyUser()->GetSelectUnit()->GetClass() )
			{
				continue;
			}
#endif SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM
			//{{ 허상형 : [2012/12/5] //		특정 아이템 가지고 있어야 캐시샵에서 구매 가능

#ifdef UNIT_CLASS_CHANGE_ITEM
			int iUnitClassChangeItemLevel = GetUnitClassChangeItemLevel( vecItemID[i] );
			int iMyClassLevel = g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel();
			int iItemUnitType = g_pData->GetItemManager()->GetItemTemplet( vecItemID[i] )->GetUnitType();
			int iUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
			int iItemUnitClass = g_pData->GetItemManager()->GetItemTemplet( vecItemID[i] )->GetUnitClass();
			int iUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();

			if ( (iUnitClassChangeItemLevel > 0 && iItemUnitType != iUnitType) || iUnitClassChangeItemLevel > iMyClassLevel )
				continue;

			if ( iUnitClassChangeItemLevel > 0 && iUnitClassChangeItemLevel < iMyClassLevel)
				continue;

			if ( iUnitClassChangeItemLevel > 0 && iItemUnitClass == iUnitClass )
				continue;
#endif
#ifdef	SERV_EXPAND_QUICK_SLOT
			if( vecItemID[i] == EXPAND_QUICK_SLOT_ITEM_ID && GetCahBuyExpandQuickSlotItem( vecItemID[i] ) == false )
			{
				continue;
			}
#endif  SERV_EXPAND_QUICK_SLOT
		}

		m_vecItemID.push_back( vecItemID[i] );
	}
//#else	PRIVATE_BANK
//	m_vecItemID = vecItemID;
//#endif	PRIVATE_BANK
//	//}} kimhc // 2009-08-12 // 은행 메인메뉴에서 살수있는 회원권만 보이도록






	int checkSize	= 0;
	checkSize		= (int)m_vecItemID.size() - 1;

	if ( checkSize <= 0 )
		checkSize = 0;
	m_MaxPage = (checkSize / m_MaxSlotNumPerPage) + 1;

	for ( int i = 0; i < (int)m_vecCashShopSlot.size(); i++ )
	{
		CX2CashShop::CashShopSlot* pCashShopSlot = m_vecCashShopSlot[i];
		if ( pCashShopSlot != NULL )
			pCashShopSlot->SetShow( false );
	}

	if ( m_bOpen == true )
	{
		for ( int i = 0; i < m_MaxSlotNumPerPage && i < (int)m_vecCashShopSlot.size() && i < (int)m_vecItemID.size(); i++ )
		{
			int itemID = m_vecItemID[i];

			if ( IsPossibleUsedByMyBankMembership( itemID ) == false )
			{
			}
			
			CX2CashShop::CashShopSlot* pCashShopSlot = m_vecCashShopSlot[i];
			pCashShopSlot->SetShow( true );
			pCashShopSlot->SetItemID( itemID );

			//{{ kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
#ifdef	AUTO_PAYMENT
			pCashShopSlot->SetInfo( bAutoPayment );
#else	AUTO_PAYMENT
			pCashShopSlot->SetInfo();
#endif	AUTO_PAYMENT
			//}} kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
		}
	}
}

//{{ kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
//#ifdef	AUTO_PAYMENT
void CX2CashShop::CashShopSlotManager::SetOpen( bool bOpen, bool bAutoPayment /*= false*/ )
//#else	AUTO_PAYMENT
//void void CX2CashShop::CashShopSlotManager::SetOpen( bool bOpen )
//#endif	AUTO_PAYMENT
//}} kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
{
	m_bOpen = bOpen;

	if ( m_bOpen == true )
	{
		int j = 0;
		for ( int i = (m_NowPage - 1) * m_MaxSlotNumPerPage; i < (m_NowPage)*m_MaxSlotNumPerPage && j < (int)m_vecCashShopSlot.size() && i < (int)m_vecItemID.size(); i++, j++ )
		{
			int itemID = m_vecItemID[i];
			CX2CashShop::CashShopSlot* pCashShopSlot = m_vecCashShopSlot[j];
			pCashShopSlot->SetShow( true );
			pCashShopSlot->SetItemID( itemID );

			//{{ kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
#ifdef	AUTO_PAYMENT
			pCashShopSlot->SetInfo( bAutoPayment );
#else	AUTO_PAYMENT
			pCashShopSlot->SetInfo();			
#endif	AUTO_PAYMENT
			//}} kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
			
		}		
	}
	else
	{
		for ( int i = 0; i < (int)m_vecCashShopSlot.size(); i++ )
		{
			CX2CashShop::CashShopSlot* pCashShopSlot = m_vecCashShopSlot[i];
			if ( pCashShopSlot == NULL )
				continue;

			pCashShopSlot->SetShow( false );
		}		
	}
}

bool CX2CashShop::CashShopSlotManager::NextPage( bool bAutoPayment /* = false */ )
{
	if ( m_NowPage >= m_MaxPage )
		return false;

	m_NowPage++;

	//{{ kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
#ifdef	AUTO_PAYMENT
	ResetNowPage( bAutoPayment );
#else	AUTO_PAYMENT
	ResetNowPage();
#endif	AUTO_PAYMENT
	//}} kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시

	return true;
}

bool CX2CashShop::CashShopSlotManager::PrevPage( bool bAutoPayment /* = false */ )
{
	if ( m_NowPage <= 1 )
		return false;

	m_NowPage--;

	//{{ kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
#ifdef	AUTO_PAYMENT
	ResetNowPage( bAutoPayment );
#else	AUTO_PAYMENT
	ResetNowPage();
#endif	AUTO_PAYMENT
	//}} kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시

	return true;
}

vector<int> CX2CashShop::CashShopSlotManager::GetNowShowItemIDList()
{
	vector<int> vecItemID;

	int j = 0;
	for ( int i = (m_NowPage-1) * m_MaxSlotNumPerPage; i < (m_NowPage)*m_MaxSlotNumPerPage && j < (int)m_vecCashShopSlot.size() && i < (int)m_vecItemID.size(); i++, j++)
	{
		int itemID = m_vecItemID[i];
		vecItemID.push_back( itemID );
	}

	return vecItemID;
}

int CX2CashShop::CashShopSlotManager::GetItemID( CKTDGUIDialogType pDialog )
{

	for ( int i = 0; i < (int)m_vecCashShopSlot.size(); i++ )
	{
		CX2CashShop::CashShopSlot* pCashShopSlot = m_vecCashShopSlot[i];
		if ( pCashShopSlot == NULL )
			continue;

		if ( pCashShopSlot->m_pDLGCashShopSlot == pDialog )
			return pCashShopSlot->GetItemID();
	}

	return 0;
}

//{{ kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가
#ifdef	AUTO_PAYMENT
void CX2CashShop::CashShopSlotManager::SetShowPresentAndCartButton( bool bShow ) // 선물, 찜 버튼
{
	CX2CashShop::CashShopSlot* pCashShopSlot = NULL;

	for ( UINT i = 0; i < m_vecCashShopSlot.size(); i++ )
	{
		pCashShopSlot = m_vecCashShopSlot[i];
		if ( pCashShopSlot == NULL )
			continue;

		pCashShopSlot->SetShowPresentAndCartButton( bShow );
		pCashShopSlot = NULL;
	}
}
#endif	AUTO_PAYMENT
//}} kimhc // 2009-10-12 // 자동결제 시 선물, 찜 버튼 안보이게 하기 위해 추가

//{{ kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
//#ifdef	AUTO_PAYMENT
void CX2CashShop::CashShopSlotManager::ResetNowPage( bool bAutoPayment )
//#else	AUTO_PAYMENT
//void CX2CashShop::CashShopSlotManager::ResetNowPage()
//#endif	AUTO_PAYMENT
//}} kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
{
	for ( int i = 0; i < (int)m_vecCashShopSlot.size(); i++ )
	{
		CX2CashShop::CashShopSlot* pCashShopSlot = m_vecCashShopSlot[i];
		if ( pCashShopSlot == NULL )
			continue;

		pCashShopSlot->SetShow( false );
	}

	int j = 0;
	for ( int i = (m_NowPage-1) * m_MaxSlotNumPerPage; i < (m_NowPage)*m_MaxSlotNumPerPage && j < (int)m_vecCashShopSlot.size() && i < (int)m_vecItemID.size(); i++, j++ )
	{
		int itemID = m_vecItemID[i];
		CX2CashShop::CashShopSlot* pCashShopSlot = m_vecCashShopSlot[j];
		pCashShopSlot->SetShow( true );
		pCashShopSlot->SetItemID( itemID );

#ifdef SERV_USE_ENABLE_GIFT
		bool bShow = IsPossiblePresent(itemID);
		pCashShopSlot->SetShowPresentButton( bShow );
#endif SERV_USE_ENABLE_GIFT

#ifdef SERV_WISH_LIST_NO_ITEM
		bool bCartShow = IsNotPossibleWishList(itemID);
		pCashShopSlot->SetShowCartButton( !bCartShow );
#endif SERV_WISH_LIST_NO_ITEM

		//{{ kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
#ifdef	AUTO_PAYMENT
		pCashShopSlot->SetInfo( bAutoPayment );
#else	AUTO_PAYMENT
		pCashShopSlot->SetInfo();
#endif	AUTO_PAYMENT
		//}} kimhc // 2009-10-12 // 자동결제 시 캐시 - 원 으로 표시
	}
}
#ifdef REFORM_SKILL_NOTE_UI
void CX2CashShop::CashShopSlotManager::ResetNowPage( int iVal_) 
{ 
	m_NowPage = iVal_; 
	ResetNowPage(); 
}
const CX2CashShop::CashShopSlot* CX2CashShop::CashShopSlotManager::GetCashShopSlot( int iItemID_ )
{
	BOOST_FOREACH( const CX2CashShop::CashShopSlot* pCashShopSlot, m_vecCashShopSlot )
	{
		if( iItemID_ == pCashShopSlot->m_ItemID )
		{
			return pCashShopSlot;
		}
	}

	return NULL;
}
#endif // REFORM_SKILL_NOTE_UI
CX2CashShop::BuyItemPopupSlot::BuyItemPopupSlot() 
{
}
CX2CashShop::BuyItemPopupSlot::BuyItemPopupSlot( int index, const WCHAR* pFileName ) : 
CX2PageMgrItem::CX2PageSlotItem( index, pFileName ) 
{
}
CX2CashShop::BuyItemPopupSlot::~BuyItemPopupSlot() 
{

}



void CX2CashShop::BuyItemPopupSlot::UpdateUI()
{
	if ( m_pDialog == NULL )
		return;

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( GetItemID() );
	if ( pItemTemplet == NULL )
		return;

	CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( GetItemID() );
	if ( pCashItem == NULL )
		return;
#ifdef COUPON_SYSTEM // 구매 팝업 UI 갱신
	// 아이템 아이콘, 이름 설정
	CKTDGUIStatic* pItemIcon = static_cast<CKTDGUIStatic*>(m_pDialog->GetControl( L"Static_ItemIcon" ));
	if( NULL != pItemIcon )
	{
		if ( pItemIcon->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() ) == false )
		{
			pItemIcon->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
		}

		if( NULL != pItemIcon->GetString(0) )
		{
			wstring wstrItemName = pItemTemplet->GetName();
			if( wstrItemName.length() > 15 )
			{
				wstrItemName = wstrItemName.substr(0,15);
				wstrItemName  += L"...";
			}

			pItemIcon->SetString( 0, wstrItemName.c_str() );
		}
	}

	// 아이템 수량(기간) / 가격 표기 콤보 박스
	CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>(m_pDialog->GetControl( L"ComboBox_Price" ));
	if ( pComboBox != NULL )
	{
		pComboBox->RemoveAllItems();
		for ( UINT uiIndex = 0; uiIndex < pCashItem->m_vecKNXBTProductInfo.size(); ++uiIndex )
		{
			if ( uiIndex == 0 )
				pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );

			KNXBTProductInfo& kNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[uiIndex];
			wstringstream wstrstm;

			
			if ( kNXBTProductInfo.GetProductExpire() != 0 )
			{// 기간제
				wstrstm << kNXBTProductInfo.GetProductExpire() << GET_STRING( STR_ID_14 ) << L"/"; 
			}
			else
			{// 1개권
				wstrstm << kNXBTProductInfo.m_usProductPieces << GET_STRING( STR_ID_24 ) << L"/";
			}

			wstrstm << kNXBTProductInfo.m_ulSalePrice << GET_STRING( STR_ID_34 ); 
			pComboBox->AddItem( wstrstm.str().c_str(), NULL, false );
		}
	}

	// 사용 가능 한 할인 쿠폰 리스트 초기화
	pComboBox = static_cast<CKTDGUIComboBox*>(m_pDialog->GetControl( L"ComboBox_Discount" ));
	if ( pComboBox != NULL )
	{
		pComboBox->SetShow(false);
	}
#else
	CKTDGUIStatic* pShopImage = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashShop_BuyWindow_Slot_Image" );

	if ( pShopImage != NULL && pShopImage->GetPicture(0) != NULL )
	{
		if ( pShopImage->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() ) == false )
		{
			pShopImage->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
		}
	}

	CKTDGUIStatic* pItemName = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStatic_Item_ID_1Line" );
	if ( pItemName != NULL && pItemName->GetString(0) != NULL )

	{
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring tempName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetName(), 97, pItemName->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
        wstring tempName = g_pMain->GetStrByLienBreak( pItemTemplet->GetName(), 97, pItemName->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
		if ( tempName.find( L"\n" ) == -1 ) //한줄 짜리
		{
			pItemName->SetString( 0, tempName.c_str() );
		}
		else
		{
			pItemName->SetString( 0, L"" );
		}

	}

	pItemName = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStatic_Item_ID_2Line" );
	if ( pItemName != NULL && pItemName->GetString(0) != NULL )
	{
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring tempName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetName(), 97, pItemName->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
        wstring tempName = g_pMain->GetStrByLienBreak( pItemTemplet->GetName(), 97, pItemName->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
		if ( tempName.find( L"\n" ) == -1 ) //한줄 짜리
		{
			pItemName->SetString( 0, L"" );

		}
		else
		{
			pItemName->SetString( 0, tempName.c_str() );
		}
	}
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP // 콤보박스에서 1000엔 ,2000엔 둘중 하나 제거
	// 같은 아이템 2종류 팔 때
	bool bIdexReset = false;
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
	CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>(m_pDialog->GetControl( L"CashItemList" ));
	if ( pComboBox != NULL )
	{
		pComboBox->RemoveAllItems();
#if defined ( SERV_GLOBAL_BILLING ) && defined ( USE_PERMANENT_INSTEAD_OF_1EA )
		bool bPeriod = false;
		for ( int i = 0; i < (int)pCashItem->m_vecKBillProductInfo.size(); i++ )
		{
			KBillProductInfo& kBillProductInfo = pCashItem->m_vecKBillProductInfo[i];
			if( kBillProductInfo.m_cPeriod != 0 )
			{
				bPeriod = true;
				break;
			}
		}
#endif //  ( SERV_GLOBAL_BILLING ) && defined ( USE_PERMANENT_INSTEAD_OF_1EA )

#ifdef SERV_GLOBAL_BILLING
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
		bool bFirstProduct = true;
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
		for ( int i = 0; i < (int)pCashItem->m_vecKBillProductInfo.size(); i++ )
		{
			KBillProductInfo& kBillProductInfo = pCashItem->m_vecKBillProductInfo[i];
			wstringstream wstrstm;
			//wstrstm << kBillProductInfo
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP	 // 같은 아이템 2종류 팔 때
			/*	if( kBillProductInfo.m_iProductNo == ITEM_ID_EVENT_NEW_SKILL_NOTE_PNO &&
			NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( ITEM_ID_EVENT_NEW_SPECIAL ) )
			{
			bIdexReset = true;
			continue;
			}
			if( kBillProductInfo.m_iProductNo == ITEM_ID_EVENT_NEW_SKILL_NOTE_PNO2 &&
			NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( ITEM_ID_EVENT_NEW_SPECIAL ) )
			{
			continue;
			}*/
			if( g_pData->GetItemManager()->CheckDisCountItemList( kBillProductInfo.m_iProductNo , bIdexReset ) == false )
			{
				continue;
			}
#endif 

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
			if( true == g_pInstanceData->IsChoicedItem() )
			{
				// 찜목록 아이템은 가격 다 보여준다
			}
			else if( false == g_pInstanceData->IsCurrentSubCategoryInNowCatagory(kBillProductInfo.m_cCategoryNo))
				continue;
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

			if ( (int)kBillProductInfo.m_cPeriod != 0 )
			{
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
				if( true == bFirstProduct )
				{
					bFirstProduct = false;
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );
				}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
				if ( i == 0 )
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP  // 같은 아이템 2종류 팔 때
				if( bIdexReset == true )
				{
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );
					bIdexReset = false;
				}
#endif
				wstrstm << (int)kBillProductInfo.m_cPeriod << GET_STRING( STR_ID_14 ) << L"/"; 
			}
			else
			{
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
				if( true == bFirstProduct )
				{
					bFirstProduct = false;
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );
				}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
				if ( i == 0 )
					pComboBox->AddItem( GET_STRING( STR_ID_36 ), NULL, false );
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP  // 같은 아이템 2종류 팔 때
				if( bIdexReset == true )
				{
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );
					bIdexReset = false;
				}
#endif

#ifdef USE_PERMANENT_INSTEAD_OF_1EA
				if( bPeriod == true && kBillProductInfo.m_cQuantity == 1 )
					wstrstm << GET_STRING( STR_ID_13858 ) << L"/";
				else
#endif USE_PERMANENT_INSTEAD_OF_1EA
				wstrstm << (int)kBillProductInfo.m_cQuantity << GET_STRING( STR_ID_24 ) << L"/";
			}

			wstrstm << kBillProductInfo.m_iSalePrice << GET_STRING( STR_ID_34 ); 

			pComboBox->AddItem( wstrstm.str().c_str(), NULL, false );
		}
#else // SERV_GLOBAL_BILLING
		for ( int i = 0; i < static_cast<int>(pCashItem->m_vecKNXBTProductInfo.size()); ++i )
		{
			KNXBTProductInfo& kNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[i];
			wstringstream wstrstm;

			if ( kNXBTProductInfo.GetProductExpire() != 0 )
			{
				if ( i == 0 )
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );

				wstrstm << kNXBTProductInfo.GetProductExpire() << GET_STRING( STR_ID_14 ) << L"/"; 
			}
			else
			{
				if ( i == 0 )
					pComboBox->AddItem( GET_STRING( STR_ID_36 ), NULL, false );

				wstrstm << kNXBTProductInfo.m_usProductPieces << GET_STRING( STR_ID_24 ) << L"/";
			}

			wstrstm << kNXBTProductInfo.m_ulSalePrice << GET_STRING( STR_ID_34 ); 

			//m_usProductPieces

			pComboBox->AddItem( wstrstm.str().c_str(), NULL, false );
		}
#endif // SERV_GLOBAL_BILLING
	}
#endif // COUPON_SYSTEM

}
#ifdef COUPON_SYSTEM  // 아이템 개별 사용 가능한 할인 쿠폰 리스트 갱신
void CX2CashShop::BuyItemPopupSlot::UpdateCouponList( const vector<CouponNoAndName>& vecCouponList_ , const int iSelectedCouponNo_ /*= -1*/ )
{
	if( NULL != m_pDialog )
	{
		CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>(m_pDialog->GetControl( L"ComboBox_Discount" ));
		if ( NULL != pComboBox )
		{
			if( true == vecCouponList_.empty() )
			{
				pComboBox->SetShow(false);
			}
			else
			{
				pComboBox->SetShow(true);
				pComboBox->RemoveAllItems();

				pComboBox->AddItem( GET_STRING( STR_ID_28127 ), IntToPtr(-1), true );
				BOOST_FOREACH( const CouponNoAndName CouponInfo, vecCouponList_ )
				{
					pComboBox->AddItem( CouponInfo.m_wstrCouponName.c_str(), IntToPtr(CouponInfo.m_iCouponNo), false );
				}

				// 기존 선택된 쿠폰 정보가 있다면, 콤보박스 선택 해주기
				if( -1 != iSelectedCouponNo_ )
					pComboBox->SetSelectedByData( IntToPtr(iSelectedCouponNo_) );
			}
		}
	}
}
#endif // COUPON_SYSTEM

void CX2CashShop::BuyItemPopupSlot::SetCheck( bool bCheck )
{
	if ( m_pDialog != NULL )
	{
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDialog->GetControl( L"g_pCheckBoxCashShop_BuyWindow_Slot" );
		if ( pCheckBox != NULL )
		{
			pCheckBox->SetCheckedPure( bCheck );
		}
	}

	//가격 종류에 맞게끔 콤보 박스 데이타 셋팅해주고.. 현재 선택된거에 따라 선택된 항목 보여주자..
}

void CX2CashShop::BuyItemPopupSlot::SetCash( int comboBoxIndex )
{
	if ( m_pDialog != NULL )
	{
#ifdef COUPON_SYSTEM
		// 아이템 수량(기간) / 가격 표기 콤보 박스
		CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>(m_pDialog->GetControl( L"ComboBox_Price" ));
#else
		CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>(m_pDialog->GetControl( L"CashItemList" ));
#endif // COUPON_SYSTEM

		if ( pComboBox != NULL )
		{
			pComboBox->SetSelectedByIndex( comboBoxIndex );
		}
	}
}

CX2CashShop::CashItemToInvenPopupSlot::CashItemToInvenPopupSlot() 
{
}
CX2CashShop::CashItemToInvenPopupSlot::CashItemToInvenPopupSlot( int index, const WCHAR* pFileName ) : 
CX2PageMgrItem::CX2PageSlotItem( index, pFileName ) 
{
}
CX2CashShop::CashItemToInvenPopupSlot::~CashItemToInvenPopupSlot() 
{

}
void CX2CashShop::CashItemToInvenPopupSlot::UpdateUI()
{
	if ( m_pDialog == NULL )
		return;

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( GetItemID() );
	if ( pItemTemplet == NULL )
		return;

	CKTDGUIStatic* pShopImage = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashShop_Move_Inventory_Window_Slot_Image" );
	if ( pShopImage != NULL && pShopImage->GetPicture(0) != NULL )
	{
        if ( pShopImage->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() ) == false )
		{
			pShopImage->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
		}
	}

	CKTDGUIStatic* pItemName = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStatic_Item_ID_1Line" );
	if ( pItemName != NULL && pItemName->GetString(0) != NULL )
	{
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring tempName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetName(), 224, pItemName->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
        wstring tempName = g_pMain->GetStrByLienBreak( pItemTemplet->GetName(), 224, pItemName->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
		if ( tempName.find( L"\n" ) == -1 ) //한줄 짜리
		{
			pItemName->SetString( 0, tempName.c_str() );
		}
		else
		{
			pItemName->SetString( 0, L"" );
		}

	}

	pItemName = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStatic_Item_ID_2Line" );
	if ( pItemName != NULL && pItemName->GetString(0) != NULL )
	{
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring tempName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetName(), 224, pItemName->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
        wstring tempName = g_pMain->GetStrByLienBreak( pItemTemplet->GetName(), 224, pItemName->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
		if ( tempName.find( L"\n" ) == -1 ) //한줄 짜리
		{
			pItemName->SetString( 0, L"" );

		}
		else
		{
			pItemName->SetString( 0, tempName.c_str() );
		}
	}


	CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDialog->GetControl( L"Option_ComboBox" );
	if ( pComboBox != NULL )
	{
		pComboBox->SetEnable( true );
		pComboBox->SetShow( true );
		pComboBox->RemoveAllItems();

#ifdef SERV_GLOBAL_BILLING
		AddItemToComboBox_KOG( pComboBox, pItemTemplet );
#else // SERV_GLOBAL_BILLING
		AddItemToComboBox( pComboBox, pItemTemplet );		
#endif // SERV_GLOBAL_BILLING

		if ( pComboBox->GetNumItems() == 0 )
		{	
			pComboBox->SetEnable( false );
			pComboBox->SetShow( false );
		}
	}


}

#ifdef SERV_GLOBAL_BILLING
void CX2CashShop::CashItemToInvenPopupSlot::AddItemToComboBox_KOG( CKTDGUIComboBox* pComboBox_, const CX2Item::ItemTemplet* pItemTemplet_ )
{
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	if ( NULL != pComboBox_ )
	{
#ifdef SERV_GLOBAL_CASH_PACKAGE
		/// 패키지 아이템 이면			
		if ( m_KBillOrderInfo.m_byteProductKind == 1 )
		{
			if (m_KBillPackageInfo.m_vecSocketOption.size() > 0)
				pComboBox_->AddItem( GET_STRING( STR_ID_37 ), NULL, false );

			for( std::vector<int>::iterator it = m_KBillPackageInfo.m_vecSocketOption.begin(); it < m_KBillPackageInfo.m_vecSocketOption.end(); ++it )
			{
				// 0은 소켓이 아예 세팅 안된것을 의미함
				if (0 != *it)
					AddSocketIdListToComboBox( pComboBox_, pItemTemplet_, *it );
			}
		}
		else	/// 패키지가 아니면
		{
#endif //SERV_GLOBAL_CASH_PACKAGE
			if ( m_KBillOrderInfo.m_vecSocketOption.size() > 0 )
				pComboBox_->AddItem( GET_STRING( STR_ID_37 ), NULL, false );

			for( std::vector<int>::iterator it = m_KBillOrderInfo.m_vecSocketOption.begin(); it < m_KBillOrderInfo.m_vecSocketOption.end(); ++it )
			{
				// 0은 소켓이 아예 세팅 안된것을 의미함
				if (0 != *it)
					AddSocketIdListToComboBox( pComboBox_, pItemTemplet_, *it );
			}

#ifdef SERV_GLOBAL_CASH_PACKAGE
		}
#endif //SERV_GLOBAL_CASH_PACKAGE
	}

#else //SERV_CASH_ITEM_SOCKET_OPTION
#ifdef SERV_GLOBAL_CASH_PACKAGE
	if ( m_KBillOrderInfo.m_byteProductKind == 1 )
	{
		if(m_KBillPackageInfo.m_vecSocketOption.empty() == false)
		{
			int iLevel_ = 
				( 0 < pItemTemplet_->GetUseLevel() ? pItemTemplet_->GetUseLevel() : g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level );

			pComboBox_->AddItem( GET_STRING( STR_ID_37 ), NULL, false );

			for( std::vector<int>::iterator it = m_KBillPackageInfo.m_vecSocketOption.begin(); it < m_KBillPackageInfo.m_vecSocketOption.end(); ++it )
			{
				int iSocketNo = *it;
				CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( iSocketNo );

				if ( pSocketData != NULL )
				{
					pComboBox_->AddItem( pSocketData->GetSocketDesc(iLevel_).c_str(), NULL, false );
				}
			}
		}

	}
	else
	{
#endif //SERV_GLOBAL_CASH_PACKAGE
		if ( m_KBillOrderInfo.m_vecSocketOption.empty() == false )
		{
			int iLevel_ = 
				( 0 < pItemTemplet_->GetUseLevel() ? pItemTemplet_->GetUseLevel() : g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level );

			pComboBox_->AddItem( GET_STRING( STR_ID_37 ), NULL, false );

			for( std::vector<int>::iterator it = m_KBillOrderInfo.m_vecSocketOption.begin(); it < m_KBillOrderInfo.m_vecSocketOption.end(); ++it )
			{
				int iSocketNo = *it;
				CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( iSocketNo );

				if ( pSocketData != NULL )
				{
					pComboBox_->AddItem( pSocketData->GetSocketDesc(iLevel_).c_str(), NULL, false );
				}
			}
		}
#ifdef SERV_GLOBAL_CASH_PACKAGE
	}
#endif //SERV_GLOBAL_CASH_PACKAGE
#endif //SERV_CASH_ITEM_SOCKET_OPTION
}
#else // SERV_GLOBAL_BILLING
void CX2CashShop::CashItemToInvenPopupSlot::AddItemToComboBox( CKTDGUIComboBox* pComboBox_, const CX2Item::ItemTemplet* pItemTemplet_ )
{

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
	if ( NULL != pComboBox_ )
	{
		/// 패키지 아이템 이면			
		if ( m_KNXBTOrderInfo.m_byteProductKind == 1 )
		{
			if ( !m_KNXBTPackageInfo.m_wstrProductAttribute0.empty() )
			{
				pComboBox_->AddItem( GET_STRING( STR_ID_37 ), NULL, false );
				AddSocketIdListToComboBox( pComboBox_, pItemTemplet_, m_KNXBTPackageInfo.m_wstrProductAttribute0.c_str() );
			}
		}
		else	/// 패키지가 아니면
		{
			if ( !m_KNXBTOrderInfo.m_wstrProductAttribute0.empty() )
			{
				pComboBox_->AddItem( GET_STRING( STR_ID_37 ), NULL, false );
				AddSocketIdListToComboBox( pComboBox_, pItemTemplet_, m_KNXBTOrderInfo.m_wstrProductAttribute0.c_str() );
			}
		}
	}

#else	SERV_CASH_ITEM_SOCKET_OPTION
	if ( m_KNXBTOrderInfo.m_byteProductKind == 1 )
	{
		if ( m_KNXBTPackageInfo.m_wstrProductAttribute0.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTPackageInfo.m_wstrProductAttribute0.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( GET_STRING( STR_ID_37 ), NULL, false );
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}

		if ( m_KNXBTPackageInfo.m_wstrProductAttribute1.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTPackageInfo.m_wstrProductAttribute1.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}

		if ( m_KNXBTPackageInfo.m_wstrProductAttribute2.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTPackageInfo.m_wstrProductAttribute2.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}

		if ( m_KNXBTPackageInfo.m_wstrProductAttribute3.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTPackageInfo.m_wstrProductAttribute3.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}
		if ( m_KNXBTPackageInfo.m_wstrProductAttribute4.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTPackageInfo.m_wstrProductAttribute4.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}
	}
	else
	{
		if ( m_KNXBTOrderInfo.m_wstrProductAttribute0.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTOrderInfo.m_wstrProductAttribute0.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( GET_STRING( STR_ID_37 ), NULL, false );
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}

		if ( m_KNXBTOrderInfo.m_wstrProductAttribute1.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTOrderInfo.m_wstrProductAttribute1.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}

		if ( m_KNXBTOrderInfo.m_wstrProductAttribute2.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTOrderInfo.m_wstrProductAttribute2.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}

		if ( m_KNXBTOrderInfo.m_wstrProductAttribute3.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTOrderInfo.m_wstrProductAttribute3.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}
		if ( m_KNXBTOrderInfo.m_wstrProductAttribute4.empty() == false )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( _wtoi( m_KNXBTOrderInfo.m_wstrProductAttribute4.c_str() ) );
			if ( pSocketData != NULL )
			{
				pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel_ ).c_str(), NULL, false );
			}
		}
	}
#endif	SERV_CASH_ITEM_SOCKET_OPTION

}
#endif // SERV_GLOBAL_BILLING

void CX2CashShop::CashItemToInvenPopupSlot::SetOption( int comboBoxIndex )
{

	if ( m_pDialog != NULL )
	{
		CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDialog->GetControl( L"Option_ComboBox" );
		if ( pComboBox != NULL )
		{
			pComboBox->SetSelectedByIndex( comboBoxIndex );
		}
	}

}

void CX2CashShop::CashItemToInvenPopupSlot::SetCheck( bool bCheck )
{
	if ( m_pDialog != NULL )
	{
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDialog->GetControl( L"g_pCheckBoxCashShop_MoveInventory_Slot" );
		if ( pCheckBox != NULL )
		{
			pCheckBox->SetCheckedPure( bCheck );
		}
	}
	//가격 종류에 맞게끔 콤보 박스 데이타 셋팅해주고.. 현재 선택된거에 따라 선택된 항목 보여주자..
}

#ifdef SERV_CASH_ITEM_SOCKET_OPTION

/** @function : AddSocketIdListToComboBox (국내에 해당)
	@brief : wszSocketItemGroupID_ 에 해당하는 그룹에 속해있는 SocketID 들을 콤보박스에 넣는 함수
	@param : 콤보박스(pComboBox_), 현재 가져오려고 하는 아이템 템플릿(pItemTemplet_), 캐시아바타를 위한 소켓옵션그룹(wszSocketItemGroupID_)
*/
#ifdef SERV_GLOBAL_BILLING
void CX2CashShop::CashItemToInvenPopupSlot::AddSocketIdListToComboBox( CKTDGUIComboBox* pComboBox_, const CX2Item::ItemTemplet* pItemTemplet_, const int iSocketItemGroupID_ )
#else //SERV_GLOBAL_BILLING
void CX2CashShop::CashItemToInvenPopupSlot::AddSocketIdListToComboBox( CKTDGUIComboBox* pComboBox_, const CX2Item::ItemTemplet* pItemTemplet_, const WCHAR* wszSocketItemGroupID_ )
#endif //SERV_GLOBAL_BILLING
{
#ifndef SERV_GLOBAL_BILLING
	if ( NULL != wszSocketItemGroupID_ )
#endif //ndef SERV_GLOBAL_BILLING
	{
		CX2SocketItem* pSocketItem = g_pData->GetSocketItem();
		if ( NULL != pSocketItem )
		{
			// 아이템 착용 레벨이 0 면 유저의 레벨을
			// 0보다 크면 아이템 레벨을 대입
			const int iLevel = 
				( 0 < pItemTemplet_->GetUseLevel() ? pItemTemplet_->GetUseLevel() : g_pData->GetSelectUnitLevel() );

#ifdef SERV_GLOBAL_BILLING
			const int iSocketGroupID = iSocketItemGroupID_;
#else //SERV_GLOBAL_BILLING
			const int iSocketGroupID = _wtoi( wszSocketItemGroupID_ );
#endif //SERV_GLOBAL_BILLING

			vector<int> vecSocketIdList;	/// iSocketGroupID 에 해당하는 SocketID 들을 얻어옴
			if ( 0 != pSocketItem->GetSocketIdListForCashAvatar( iSocketGroupID, vecSocketIdList, pItemTemplet_->GetItemType() ) )
			{
				// 얻어왔으면, 차례로 넣는다.
				BOOST_FOREACH( const int iSocketID, vecSocketIdList )
				{
					const CX2SocketItem::SocketData* pSocketData = pSocketItem->GetSocketData( iSocketID );
					if ( NULL != pSocketData )
						pComboBox_->AddItem( pSocketData->GetSocketDesc( iLevel ).c_str(), NULL, false );
				}
			}
		}
	}
}

#endif // SERV_CASH_ITEM_SOCKET_OPTION


void CX2CashShop::BuyItemPopup::SetCheckAllItem( bool bCheck )
{
	if ( bCheck == true )
	{
		m_vecCheckedItem = m_vecItemID;
	}
	else
	{
		m_vecCheckedItem.resize(0);
	}

	ResetNowPage();
}

void CX2CashShop::BuyItemPopup::ChangeCheckItem( CKTDGUIDialogType pDialog, bool bCheck )
{
	if ( pDialog == NULL )
		return;

	CX2CashShop::BuyItemPopupSlot* pBuyItemPopupSlot = (CX2CashShop::BuyItemPopupSlot*)GetPageSlot( pDialog );
	if ( pBuyItemPopupSlot == NULL )
		return;

	int itemIDToFind = pBuyItemPopupSlot->GetItemID();

	if ( bCheck == true )
	{
		bool bFind = false;
		for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
		{
			int itemID = m_vecCheckedItem[i];
			if ( itemID == itemIDToFind )
			{
				bFind = true;
				break;
			}
		}
		if ( bFind == false )
		{
			m_vecCheckedItem.push_back( itemIDToFind );
		}
	}
	else
	{
		for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
		{
			int itemID = m_vecCheckedItem[i];
			if ( itemID == itemIDToFind )
			{
				m_vecCheckedItem.erase( m_vecCheckedItem.begin() + i );
				i--;
				break;
			}
		}
	}

	ResetNowPage();
}

#ifdef COUPON_SYSTEM
unsigned long CX2CashShop::BuyItemPopup::ChangeItemPeriod( CKTDGUIDialogType pDialog, int comboBoxIndex )
#else
void CX2CashShop::BuyItemPopup::ChangeItemPeriod( CKTDGUIDialogType pDialog, int comboBoxIndex )
#endif // COUPON_SYSTEM
{
	CX2CashShop::BuyItemPopupSlot* pBuyItemPopupSlot = (CX2CashShop::BuyItemPopupSlot*)GetPageSlot( pDialog );
	if ( pBuyItemPopupSlot != NULL )
	{
		map< int, int >::iterator mit;
		mit = m_mapSelectedProductNo.find( pBuyItemPopupSlot->GetItemID()  );
		if ( mit != m_mapSelectedProductNo.end() )
		{
			mit->second = comboBoxIndex;
		}
		else
		{
			m_mapSelectedProductNo.insert( std::make_pair( pBuyItemPopupSlot->GetItemID(), comboBoxIndex ) );
		}
#ifdef COUPON_SYSTEM
		// 페이지 슬롯 UI 갱신을 위해 현재 페이지 Dialog 저장
		m_pDialogSelectedPage = pDialog;
		m_ulSelectedProductNo = 0;
#endif //COUPON_SYSTEM
	}
	UpdateCashUI();

#ifdef COUPON_SYSTEM // 사용 가능한 쿠폰 정보 요청 하기 위한 자료 얻기
	if( NULL != pBuyItemPopupSlot )
	{
		CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( pBuyItemPopupSlot->GetItemID() );
		if ( NULL != pCashItem )
		{
			// ChangeItemPeriod는 1부터 시작하는 값을 받기 때문에 -1
			int iVecIndex = comboBoxIndex - 1;

			if ( 0 <= iVecIndex && 
				static_cast<int>(pCashItem->m_vecKNXBTProductInfo.size()) > iVecIndex )
			{
				// 선택된 아이템ID의 상품 번호 저장
				KNXBTProductInfo& kNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[iVecIndex];	
				m_ulSelectedProductNo = kNXBTProductInfo.m_ulProductNo;

				// 서버로 패킷 요청 할 때 사용 할 수 있도록 상품 번호 반환
				return m_ulSelectedProductNo;
			}
		}
	}

	return 0;
#endif // COUPON_SYSTEM
}

#ifdef COUPON_SYSTEM
	bool CX2CashShop::BuyItemPopup::GetCheckedProductAndCouponNoList(OUT vector< std::pair<unsigned long, int> >& vecProductAndCouponNoList_ )
#else
	bool CX2CashShop::BuyItemPopup::GetCheckedProductNoList(OUT vector<unsigned long>& vecProductNoList )
#endif // COUPON_SYSTEM
{
#ifdef COUPON_SYSTEM
	vecProductAndCouponNoList_.clear();
#endif // COUPON_SYSTEM

	// 구매 하기 위해 선택된 아이템 List 순회
	for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
	{
		int itemID = m_vecCheckedItem[i];

		// 기간(수량)에 대한 정보 검색
		map< int, int >::iterator mit = m_mapSelectedProductNo.find( itemID );
		if ( mit != m_mapSelectedProductNo.end() )
		{
			if ( mit->second == 0 )
				return false;

			int productIndex = mit->second - 1;

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP  // 같은 아이템 2종류 팔 때
		/*	if( itemID == ITEM_ID_EVENT_NEW_SKILL_NOTE &&
				NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( ITEM_ID_EVENT_NEW_SPECIAL ) )
			{
				productIndex += 1;
			}*/
			std::vector<int> TempVec;
			int TempProDuctNum = 0;
			if(g_pData->GetItemManager()->DisCountItemID_Find(itemID,TempVec,TempProDuctNum) == true)
			{
				for(int i = 0; i < TempVec.size(); ++i)
				{
					if( NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( TempVec[i] ) )
					{
						productIndex += 1;
						break;
					}
				}
			}
#endif 
			CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
#ifdef SERV_GLOBAL_BILLING
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
				vector< KBillProductInfo > vecBillInfoTemp;
				if( true == GetChoicedItemBuy() || true == g_pInstanceData->IsChoicedItem() )
				{
					vecBillInfoTemp = pCashItem->m_vecKBillProductInfo;
				}
				else
				{
					vecBillInfoTemp = g_pInstanceData->GetCurrentProductInfoListInNowCatagory( pCashItem->m_vecKBillProductInfo );
				}
				
				if ( productIndex >= 0 && (int)vecBillInfoTemp.size() > productIndex )
				{
					KBillProductInfo& kBillProductInfo = vecBillInfoTemp[productIndex];
					vecProductNoList.push_back( kBillProductInfo.m_iProductNo );
				}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
				if ( productIndex >= 0 && (int)pCashItem->m_vecKBillProductInfo.size() > productIndex )
				{
					KBillProductInfo& kBillProductInfo = pCashItem->m_vecKBillProductInfo[productIndex];
					vecProductNoList.push_back( kBillProductInfo.m_iProductNo );
				}
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
#else // SERV_GLOBAL_BILLING
				if ( productIndex >= 0 && (int)pCashItem->m_vecKNXBTProductInfo.size() > productIndex )
				{
					// 선택된 아이템ID의 상품 번호 담기
					KNXBTProductInfo& kNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[productIndex];
 
#ifdef COUPON_SYSTEM // 할인 쿠폰 번호 적용
// 					// 선택된 상품에 맞는 쿠폰을 선택 했다면 쿠폰 정보 포함 시키기
 					int iCouponCardNo = -1;
					map< unsigned long, int >::iterator itCoupon = m_mapSelectedCouponNo.find( kNXBTProductInfo.m_ulProductNo );
 					if( itCoupon != m_mapSelectedCouponNo.end() )
 					{
 						iCouponCardNo = itCoupon->second;
 					}
 
 					vecProductAndCouponNoList_.push_back( std::make_pair( kNXBTProductInfo.m_ulProductNo, iCouponCardNo ));
#else
					vecProductNoList.push_back( kNXBTProductInfo.m_ulProductNo );
#endif // COUPON_SYSTEM
				}
#endif // SERV_GLOBAL_BILLING
			}
		}
		else
		{
			return false;
		}

	}
	return true;
}


void CX2CashShop::BuyItemPopup::UpdateCashUI()
{
	if ( m_pDialog != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Mycash" );
		if ( pStatic != NULL )
		{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalMainCash() ).c_str() );
#else //SERV_SUPPORT_SEVERAL_CASH_TYPES
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetCash() ).c_str() );
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		}
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
#if defined( SERV_BILLING_ADD_GASH_POINT )
		pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Mygashpoint" );
		if ( pStatic != NULL )
		{
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) ).c_str() );
		}
		
		if(GetUseCashType() == static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH))
		{
			// 선택된 글자 크게 변경시키기
			CKTDGUIStatic *pStaticDlg = m_pDialog->GetStatic_LUA("g_pStaticCashShop_BuyWindow");
			CKTDGUIControl::CPictureData *pPicText = pStaticDlg->GetPictureIndex(101);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(111);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(102);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(112);
			pPicText->SetShow(true);

			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_MyrestGashpoint" );
			if ( pStatic != NULL )
				pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) ).c_str() );
			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Myrest" );
		}
		else
		{
			// 선택된 글자 크게 변경시키기
			CKTDGUIStatic *pStaticDlg = m_pDialog->GetStatic_LUA("g_pStaticCashShop_BuyWindow");
			CKTDGUIControl::CPictureData *pPicText = pStaticDlg->GetPictureIndex(101);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(111);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(102);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(112);
			pPicText->SetShow(false);

			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Myrest" );
			if ( pStatic != NULL )
				pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) ).c_str() );
			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_MyrestGashpoint" );
		}
#elif defined( CHINA_CASH_CHANGE )
		pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_MyBonuspoint" );
		if ( pStatic != NULL )
		{
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT ) ).c_str() );
		}

		if( GetUseCashType() == static_cast<int>( KGlobalCashInfo::GCT_KOG_ELSWORD_CASH ) )
		{
			// 선택된 글자 크게 변경시키기
			CKTDGUIStatic *pStaticDlg = m_pDialog->GetStatic_LUA("g_pStaticCashShop_BuyWindow");
			CKTDGUIControl::CPictureData *pPicText = pStaticDlg->GetPictureIndex(101);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(111);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(102);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(112);
			pPicText->SetShow(true);

			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_MyrestBonuspoint" );
			if ( pStatic != NULL )
				pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT ) ).c_str() );
			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Myrest" );
		}
		else
		{
			// 선택된 글자 크게 변경시키기
			CKTDGUIStatic *pStaticDlg = m_pDialog->GetStatic_LUA("g_pStaticCashShop_BuyWindow");
			CKTDGUIControl::CPictureData *pPicText = pStaticDlg->GetPictureIndex(101);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(111);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(102);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(112);
			pPicText->SetShow(false);

			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Myrest" );
			if ( pStatic != NULL )
				pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_CASH ) ).c_str() );
			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_MyrestBonuspoint" );
		}
#else
		pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Myrest" );
#endif 
#endif SERV_SUPPORT_SEVERAL_CASH_TYPES
		if ( pStatic != NULL )
		{
			//wstringstream wstrstm;
			//wstrstm << L"";
			//pStatic->SetString( 0, g_pMain->GetEDString( -10000 ).c_str() );

			int cashToBuy = 0;
			for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
			{
				int itemID = m_vecCheckedItem[i];
				CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
				if ( pCashItem == NULL )
					continue;

				map< int, int >::iterator mit;
				mit = m_mapSelectedProductNo.find( itemID );
				if ( mit != m_mapSelectedProductNo.end() )
				{
					int productIndex = mit->second - 1;
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP  // 같은 아이템 2종류 팔 때
				/*	if( itemID == ITEM_ID_EVENT_NEW_SKILL_NOTE &&
						NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( ITEM_ID_EVENT_NEW_SPECIAL ) &&
						productIndex != -1 )
					{
						productIndex += 1;
					}*/
					std::vector<int> nTempVec;
					int nTempProDuctNum = 0;
					if(g_pData->GetItemManager()->DisCountItemID_Find(itemID,nTempVec,nTempProDuctNum) == true)
					{
						for(int i = 0; i < nTempVec.size(); ++i)
						{
							if( NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( nTempVec[i] ) )
							{
								if( productIndex != -1 )
								{
									productIndex += 1;
									break;
								}
							}
						}
					}
#endif

#ifdef SERV_GLOBAL_BILLING
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
					vector< KBillProductInfo > vecBillInfoTemp;
					if( true == GetChoicedItemBuy() )
					{
						// 찜목록에서는 모든 가격이 다 표시 한다
						vecBillInfoTemp = pCashItem->m_vecKBillProductInfo;
					}
					else
					{
						vecBillInfoTemp = g_pInstanceData->GetCurrentProductInfoListInNowCatagory( pCashItem->m_vecKBillProductInfo );
					}
					
					if ( productIndex >= 0 && (int)vecBillInfoTemp.size() > productIndex )
					{
						KBillProductInfo& kKBillProductInfo = vecBillInfoTemp[productIndex];
						cashToBuy += kKBillProductInfo.m_iSalePrice;
					}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
					if ( productIndex >= 0 && productIndex < (int)pCashItem->m_vecKBillProductInfo.size() )
					{
						KBillProductInfo& kKBillProductInfo = pCashItem->m_vecKBillProductInfo[productIndex];
						cashToBuy += kKBillProductInfo.m_iSalePrice;						
					}
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
#else // SERV_GLOBAL_BILLING
					if ( productIndex >= 0 && productIndex < (int)pCashItem->m_vecKNXBTProductInfo.size() )
					{
						KNXBTProductInfo& kKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[productIndex];
						cashToBuy += kKNXBTProductInfo.m_ulSalePrice;
					}
#endif // SERV_GLOBAL_BILLING
				}
				/*
				else
				{
					if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
					{
						KNXBTProductInfo& kKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[0];
						cashToBuy += kKNXBTProductInfo.m_ulSalePrice;
					}
				}
				*/
			}
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
			int restCash = 0;
			switch(GetUseCashType())
			{
			case KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH:
				restCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) - cashToBuy;
				break;
			case KGlobalCashInfo::GCT_PUBLISHER_CASH:
				restCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) - cashToBuy;
				break;
			case KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT:
				restCash = g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT ) - cashToBuy;
				break;
			default:
				restCash = g_pData->GetMyUser()->GetGlobalMainCash() - cashToBuy;
				break;
			}
#else
			int restCash = g_pData->GetMyUser()->GetCash() - cashToBuy;
#endif SERV_SUPPORT_SEVERAL_CASH_TYPES
			if ( restCash >= 0 )
			{
				pStatic->SetString( 0, g_pMain->GetEDString( restCash ).c_str() );
			}
			else
			{
				restCash = abs( restCash );
				wstringstream wstrstm;
				wstrstm << L"-" << g_pMain->GetEDString( restCash ).c_str();
				pStatic->SetString( 0, wstrstm.str().c_str() );
			}
		}
	}
}



//{{ 김상훈 2010.11.1
#ifdef REST_CASH_CHECK
bool CX2CashShop::BuyItemPopup:: RestCashCheck()
{
	if ( m_pDialog != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Mycash" );
		if ( pStatic != NULL )
		{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalMainCash() ).c_str() );
#else //SERV_SUPPORT_SEVERAL_CASH_TYPES
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetCash() ).c_str() );
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		}
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef SERV_BILLING_ADD_GASH_POINT
		pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Mygashpoint" );
		if ( pStatic != NULL )
		{
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) ).c_str() );
		}

		if(GetUseCashType() == static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_CASH))
		{
			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_MyrestGashpoint" );
		}
		else
		{
			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Myrest" );
		}
#endif SERV_BILLING_ADD_GASH_POINT
#ifdef CHINA_CASH_CHANGE
		pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_MyBonuspoint" );
		if ( pStatic != NULL )
		{
			pStatic->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT) ).c_str() );
		}

		if(GetUseCashType() == static_cast<int>(KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT))
		{
			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_MyrestBonuspoint" );
		}
		else
		{
			pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Myrest" );
		}
#endif CHINA_CASH_CHANGE
		if ( pStatic != NULL )
		{
			int cashToBuy = 0;
			for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
			{
				int itemID = m_vecCheckedItem[i];
				CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
				if ( pCashItem == NULL )
					continue;

				map< int, int >::iterator mit;
				mit = m_mapSelectedProductNo.find( itemID );
				if ( mit != m_mapSelectedProductNo.end() )
				{
					int productIndex = mit->second - 1;

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
					vector< KBillProductInfo > vecBillInfoTemp = g_pInstanceData->GetCurrentProductInfoListInNowCatagory( pCashItem->m_vecKBillProductInfo );
					if ( productIndex >= 0 && (int)vecBillInfoTemp.size() > productIndex )
					{
						KBillProductInfo& kBillProductInfo = vecBillInfoTemp[productIndex];
						cashToBuy += kBillProductInfo.m_iSalePrice;
					}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
					if ( productIndex >= 0 && productIndex < (int)pCashItem->m_vecKBillProductInfo.size() )
					{
						KBillProductInfo& kBillProductInfo = pCashItem->m_vecKBillProductInfo[productIndex];
						cashToBuy += kBillProductInfo.m_iSalePrice;	// 사고자하는 아이템의 가격을 가져온다.
					}
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
				}
			}

			int restCash = 0;
			switch(GetUseCashType())
			{
			case KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH:
				restCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) - cashToBuy;
				break;
			case KGlobalCashInfo::GCT_PUBLISHER_CASH:
				restCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) - cashToBuy;
				break;
			case KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT:
				restCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT) - cashToBuy;
				break;
			default:
				restCash = g_pData->GetMyUser()->GetGlobalMainCash() - cashToBuy;
				break;
			}

#if defined(_IN_HOUSE_) && ( defined(SERV_COUNTRY_US) || defined( CLIENT_COUNTRY_ID ) )
			return true;
#else // defined(_IN_HOUSE_) && ( defined(SERV_COUNTRY_US) || defined( CLIENT_COUNTRY_ID ) )
			if ( restCash >= 0 )
				return true;
#endif // defined(_IN_HOUSE_) &&  ( defined(SERV_COUNTRY_US) || defined( CLIENT_COUNTRY_ID ) )
		}

#else // SERV_SUPPORT_SEVERAL_CASH_TYPES
		pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Myrest" );
		if ( pStatic != NULL )
		{

			int cashToBuy = 0;
			for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
			{
				int itemID = m_vecCheckedItem[i];
				CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
				if ( pCashItem == NULL )
					continue;

				map< int, int >::iterator mit;
				mit = m_mapSelectedProductNo.find( itemID );
				if ( mit != m_mapSelectedProductNo.end() )
				{
					int productIndex = mit->second - 1;

#ifdef SERV_GLOBAL_BILLING
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
					vector< KBillProductInfo > vecBillInfoTemp = GetCurrentProductInfoListInNowCatagory( pCashItem->m_vecKBillProductInfo );
					if ( productIndex >= 0 && (int)vecBillInfoTemp.size() > productIndex )
					{
						KBillProductInfo& kBillProductInfo = vecBillInfoTemp[productIndex];
						cashToBuy += kKBillProductInfo.m_iSalePrice;
					}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
					if ( productIndex >= 0 && productIndex < (int)pCashItem->m_vecKBillProductInfo.size() )
					{
						KBillProductInfo& kBillProductInfo = pCashItem->m_vecKBillProductInfo[productIndex];
						cashToBuy += kBillProductInfo.m_iSalePrice;	// 사고자하는 아이템의 가격을 가져온다.
					}
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
#else // SERV_GLOBAL_BILLING
					if ( productIndex >= 0 && productIndex < (int)pCashItem->m_vecKNXBTProductInfo.size() )
					{
						KNXBTProductInfo& kKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[productIndex];
						cashToBuy += kKNXBTProductInfo.m_ulSalePrice;
					}
#endif // SERV_GLOBAL_BILLING
				}
			}

			int restCash = g_pData->GetMyUser()->GetCash() - cashToBuy;
			if ( restCash >= 0 )
				return true;
		}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	}
	return false;			
}
#endif REST_CASH_CHECK
//}} 김상훈 2010.11.1

#ifdef CASH_ITEM_REFUND
bool CX2CashShop::BuyItemPopup::NonRefundableItemInclude()
{
	for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
	{
		int itemID = m_vecCheckedItem[i];
		for(int i=0; i<ARRAY_SIZE(NON_REFUNDABLE_CASH_ITEM_ID); ++i)
		{
			if( itemID == NON_REFUNDABLE_CASH_ITEM_ID[i] )
				return true;
		}
	}
	return false;
}

void CX2CashShop::BuyItemPopup::SetStringRefundableItem()
{
	if ( m_pDialog != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_BuyWindow_Refund" );
		if ( pStatic != NULL )
		{
			pStatic->SetString( 0, GET_STRING(STR_ID_9794) );
		}
	}
	return;
}
void CX2CashShop::BuyItemPopup::SetStringNonRefundableItem()
{
	if ( m_pDialog != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_BuyWindow_Refund" );
		if ( pStatic != NULL )
		{
			pStatic->SetString( 0, GET_STRING(STR_ID_9795) );
		}
	}
	return;
}
void CX2CashShop::BuyItemPopup::SetStringNonRefundableItemInclude()
{
	if ( m_pDialog != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_BuyWindow_Refund" );
		if ( pStatic != NULL )
		{
			pStatic->SetString( 0, GET_STRING(STR_ID_9796) );
		}
	}
	return;
}

void CX2CashShop::SetStringBuyItemRefund()
{
#ifndef COUPON_SYSTEM
	if(m_pBuyItemPopUp->NonRefundableItemInclude() == true)
	{
		if(m_pBuyItemPopUp->GetCheckedItemListSize() > 1)
		{
			m_pBuyItemPopUp->SetStringNonRefundableItemInclude();
		}
		else
		{
			m_pBuyItemPopUp->SetStringNonRefundableItem();
		}
	}
	else
	{
		m_pBuyItemPopUp->SetStringRefundableItem();
	}
#endif // COUPON_SYSTEM

	return;
}
#endif CASH_ITEM_REFUND

void CX2CashShop::Send_EGS_VISIT_CASH_SHOP_NOT( const bool bOpen_ ) const // 청약 철회 디파인 안에 있어서 해외팀 수정
{
	KEGS_VISIT_CASH_SHOP_NOT kPacket;
	kPacket.m_bEnterCashShop = bOpen_;

	g_pData->GetServerProtocol()->SendPacket( EGS_VISIT_CASH_SHOP_NOT, kPacket );
}

void CX2CashShop::BuyItemPopup::InitbyOpen()
{
	m_vecCheckedItem = m_vecItemID;
	m_mapSelectedProductNo.clear();		//itemID, CashData에 KNXBTProductInfo 벡터 인덱스	
#ifdef COUPON_SYSTEM
	m_mapProductCouponNo.clear();
	m_mapSelectedCouponNo.clear();
#endif // COUPON_SYSTEM

	SetCheckAllItem( true );

	CKTDGUICheckBox* pCheckBoxMoveInven = (CKTDGUICheckBox*)m_pDialog->GetControl( L"g_pCheckBoxCashShop_BuyWindow_Slot" );
	if ( pCheckBoxMoveInven != NULL )
	{
		pCheckBoxMoveInven->SetCheckedPure( true );
	}
}


void CX2CashShop::BuyItemPopup::ResetNowPage()
{
	CX2PageMgrItem::ResetNowPage();

	//여기에 구매 후 남은 캐시량과
	//나의 캐시

	UpdateCashUI();

	if ( m_pDialog != NULL )
	{

#ifdef COUPON_SYSTEM
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDialog->GetControl( L"ByItemPopup_Page" ));
#else
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Buywindow_Font" );
#endif // COUPON_SYSTEM
		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << GetNowPage() << L"/" << GetMaxPage();
			pStatic->SetString( 0, wstrstm.str().c_str() );
		}


	}

	int j = 0;
	for ( int i = (m_NowPage-1) * m_MaxSlotNumPerPage; i < (m_NowPage)*m_MaxSlotNumPerPage && j < (int)m_vecPageSlot.size() && i < (int)m_vecItemID.size(); i++, j++ )
	{
		int itemID = m_vecItemID[i];
		CX2CashShop::BuyItemPopupSlot* pPageSlot = (CX2CashShop::BuyItemPopupSlot*)m_vecPageSlot[j];

		bool bCheck = false;
		for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
		{
			int tempItemID = m_vecCheckedItem[i];
			if ( itemID == tempItemID )
			{
				bCheck = true;
				break;
			}
		}

		if ( bCheck == true )
			pPageSlot->SetCheck( true );
		else
			pPageSlot->SetCheck( false );

		map< int, int >::iterator mit;
		mit = m_mapSelectedProductNo.find( itemID );
		if ( mit != m_mapSelectedProductNo.end() )
		{
			pPageSlot->SetCash( mit->second );
		}
		else
		{
			CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				pPageSlot->SetCash( 0 );
			}
		}
	}
}

#ifdef SERV_GLOBAL_BILLING
void CX2CashShop::CashItemToInvenPopup::Reset( vector< int >& vecItemID, vector< KBillOrderInfo >& vecKBillOrderInfo,
											  vector< KBillPackageInfo >& vecKBillPackageInfo )
{
	m_vecKBillOrderInfo = vecKBillOrderInfo;
	m_vecKBillPackageInfo = vecKBillPackageInfo;

	CX2PageMgrItem::Reset( vecItemID );
}
#else // SERV_GLOBAL_BILLING
void CX2CashShop::CashItemToInvenPopup::Reset( vector< int >& vecItemID, vector< KNXBTOrderInfo >& vecKNXBTOrderInfo, 
											  vector< KNXBTPackageInfo >& vecKNXBTPackageInfo )
{
	m_vecKNXBTOrderInfo = vecKNXBTOrderInfo;
	m_vecKNXBTPackageInfo = vecKNXBTPackageInfo;

	CX2PageMgrItem::Reset( vecItemID );
}
#endif // SERV_GLOBAL_BILLING

void CX2CashShop::CashItemToInvenPopup::ChangeItemOption( CKTDGUIDialogType pDialog, int comboBoxIndex )
{
	CX2CashShop::CashItemToInvenPopupSlot* pItemPopupSlot = (CX2CashShop::CashItemToInvenPopupSlot*)GetPageSlot( pDialog );
	if ( pItemPopupSlot != NULL )
	{
		map< int, int >::iterator mit;
		mit = m_mapSelectedOption.find( pItemPopupSlot->GetItemID()  );
		if ( mit != m_mapSelectedOption.end() )
		{
			mit->second = comboBoxIndex;
		}
		else
		{
			m_mapSelectedOption.insert( std::make_pair( pItemPopupSlot->GetItemID(), comboBoxIndex ) );
		}
	}
}

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
/** @function : GetSelectedOptionListAndGroupID
	@brief : 선택한 소켓옵션의 리스트와 아이템별 그룹아이디 리스트 얻어가는 함수
	@param : 소켓한 소켓옵션 리스트(mapSelectedOptionList), 아이템별 그룹아이디 리스트(mapOptionGroupIdList_)
	@return : 성공하면 true, 실패하면 false
*/
bool CX2CashShop::CashItemToInvenPopup::GetSelectedOptionListAndGroupID( OUT map<int, int>& mapSelectedOptionList_, OUT map<int, int>& mapOptionGroupIdList_ )
#else	SERV_CASH_ITEM_SOCKET_OPTION
bool CX2CashShop::CashItemToInvenPopup::GetSelectedOptionList( map< int, int >& mapSelectedOptionList_ )
#endif	SERV_CASH_ITEM_SOCKET_OPTION
{
#ifdef SERV_GLOBAL_BILLING
	if ( m_vecKBillOrderInfo.empty() == true )
#else // SERV_GLOBAL_BILLING
	if ( m_vecKNXBTOrderInfo.empty() == true )
#endif //SERV_GLOBAL_BILLING
	{
		return false;
	}

#ifdef SERV_CASH_ITEM_SOCKET_OPTION
#ifdef SERV_GLOBAL_BILLING
	KBillOrderInfo& kCashOrderInfo = m_vecKBillOrderInfo[0];
#else //SERV_GLOBAL_BILLING
	KNXBTOrderInfo& kCashOrderInfo = m_vecKNXBTOrderInfo[0];
#endif //SERV_GLOBAL_BILLING

	if ( kCashOrderInfo.m_byteProductKind == 1 )
	{
#ifdef SERV_GLOBAL_BILLING
		vector< KBillPackageInfo > vecPackageInfo;
		unsigned long ulTranNo = kCashOrderInfo.m_iTransNo;
#else //SERV_GLOBAL_BILLING
		vector< KNXBTPackageInfo > vecPackageInfo;
		unsigned long ulTranNo = kCashOrderInfo.m_ulProductNo;
#endif //SERV_GLOBAL_BILLING
		GetPackageInfo( ulTranNo, vecPackageInfo );

		for ( int j = 0; j < (int)vecPackageInfo.size(); j++ )
		{
#ifdef SERV_GLOBAL_BILLING
			KBillPackageInfo& kCashPackageInfo = vecPackageInfo[j];
			int itemID = kCashPackageInfo.m_iProductID;
#else //SERV_GLOBAL_BILLING
			KNXBTPackageInfo& kCashPackageInfo = vecPackageInfo[j];
			int itemID = _wtoi( kCashPackageInfo.m_wstrProductID.c_str() );
#endif //SERV_GLOBAL_BILLING

			bool bCheckedItem = false;
			for ( int k = 0; k < (int)m_vecCheckedItem.size(); k++ )
			{
				int checkedItemID = m_vecCheckedItem[k];
				if ( checkedItemID == itemID )
				{
					bCheckedItem = true;
					break;
				}
			}

			if ( bCheckedItem == false )
				continue;

			if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( itemID ) == false )
				continue;


			map< int, int >::iterator mit;
			mit = m_mapSelectedOption.find( itemID );
#ifdef SERV_GLOBAL_BILLING
			if ( mit != m_mapSelectedOption.end() && !kCashPackageInfo.m_vecSocketOption.empty() )
#else //SERV_GLOBAL_BILLING
			if ( mit != m_mapSelectedOption.end() && !kCashPackageInfo.m_wstrProductAttribute0.empty() )
#endif //SERV_GLOBAL_BILLING
			{
				const UINT uiAttributeIndex = mit->second;

#ifdef SERV_GLOBAL_BILLING
				pair<int,int> pairGroupIdAndSelectedSocketId = GetSelectedOptionList( kCashPackageInfo.m_vecSocketOption[0], uiAttributeIndex, itemID );
#else //SERV_GLOBAL_BILLING
				pair<int,int> pairGroupIdAndSelectedSocketId = GetSelectedOptionList( kCashPackageInfo.m_wstrProductAttribute0.c_str(), uiAttributeIndex, itemID );
#endif //SERV_GLOBAL_BILLING
				
				const int iGroupdID = pairGroupIdAndSelectedSocketId.first;
				const int iSocketOptionToBeSelected = pairGroupIdAndSelectedSocketId.second;				
									
				if ( 0 != iSocketOptionToBeSelected && 0 != iGroupdID )
				{
					mapSelectedOptionList_.insert( std::make_pair( itemID, iSocketOptionToBeSelected ) );
					mapOptionGroupIdList_.insert( std::make_pair( iSocketOptionToBeSelected, iGroupdID ) );
				}
				else			
					return false;				
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
#ifdef SERV_GLOBAL_BILLING
		int itemID =  kCashOrderInfo.m_kBillProductInfo.m_iProductID;
#else //SERV_GLOBAL_BILLING
		int itemID = _wtoi( kCashOrderInfo.m_wstrProductID.c_str() );
#endif //SERV_GLOBAL_BILLING
		if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( itemID ) == true )
		{

			bool bCheckedItem = false;
			for ( int k = 0; k < (int)m_vecCheckedItem.size(); k++ )
			{
				int checkedItemID = m_vecCheckedItem[k];
				if ( checkedItemID == itemID )
				{
					bCheckedItem = true;
					break;
				}
			}

			if ( bCheckedItem == false )
				return true;

			map< int, int >::iterator mit;
			mit = m_mapSelectedOption.find( itemID );
#ifdef SERV_GLOBAL_BILLING
			if ( mit != m_mapSelectedOption.end() && !kCashOrderInfo.m_vecSocketOption.empty() )
#else //SERV_GLOBAL_BILLING
			if ( mit != m_mapSelectedOption.end() && !kCashOrderInfo.m_wstrProductAttribute0.empty() )
#endif //SERV_GLOBAL_BILLING
			{
				const UINT uiAttributeIndex = mit->second;
#ifdef SERV_GLOBAL_BILLING
				pair<int,int> pairGroupIdAndSelectedSocketId = GetSelectedOptionList( kCashOrderInfo.m_vecSocketOption[0], uiAttributeIndex, itemID );
#else //SERV_GLOBAL_BILLING
				pair<int,int> pairGroupIdAndSelectedSocketId = GetSelectedOptionList( kCashOrderInfo.m_wstrProductAttribute0.c_str(), uiAttributeIndex, itemID );
#endif //SERV_GLOBAL_BILLING

				const int iGroupdID = pairGroupIdAndSelectedSocketId.first;
				const int iSocketOptionToBeSelected = pairGroupIdAndSelectedSocketId.second;

				if ( 0 != iSocketOptionToBeSelected && 0 !=iGroupdID )
				{
					mapSelectedOptionList_.insert( std::make_pair( itemID, iSocketOptionToBeSelected ) );
					mapOptionGroupIdList_.insert( std::make_pair( iSocketOptionToBeSelected, iGroupdID ) );
				}
				else			
					return false;			
			}
			else
			{
				return false;
			}
		}
	}
#else	SERV_CASH_ITEM_SOCKET_OPTION

#ifdef SERV_GLOBAL_BILLING
	KBillOrderInfo& kCashOrderInfo = m_vecKBillOrderInfo[0];
#else //SERV_GLOBAL_BILLING
	KNXBTOrderInfo& kCashOrderInfo = m_vecKNXBTOrderInfo[0];
#endif //SERV_GLOBAL_BILLING
	if ( kCashOrderInfo.m_byteProductKind == 1 )
	{
		vector< CX2ItemManager::PackageItemData > vecPackageItemData;
		//g_pData->GetItemManager()->GetPackageData( kCashOrderInfo.m_ulProductNo, vecPackageItemData );

#ifdef SERV_GLOBAL_BILLING
		vector< KBillPackageInfo > vecPackageInfo;
		unsigned long ulTranNo = kCashOrderInfo.m_iTransNo;
#else //SERV_GLOBAL_BILLING
		vector< KNXBTPackageInfo > vecPackageInfo;
		unsigned long ulTranNo = kCashOrderInfo.m_ulProductNo;
#endif //SERV_GLOBAL_BILLING
		GetPackageInfo( ulTranNo, vecPackageInfo );

		for ( int j = 0; j < (int)vecPackageInfo.size(); j++ )
		{
#ifdef SERV_GLOBAL_BILLING
			KBillPackageInfo& kCashPackageInfo = vecPackageInfo[j];
			int itemID = kCashPackageInfo.m_iProductID;
#else //SERV_GLOBAL_BILLING
			KNXBTPackageInfo& kCashPackageInfo = vecPackageInfo[j];
			int itemID = _wtoi( kCashPackageInfo.m_wstrProductID.c_str() );
#endif //SERV_GLOBAL_BILLING

			bool bCheckedItem = false;
			for ( int k = 0; k < (int)m_vecCheckedItem.size(); k++ )
			{
				int checkedItemID = m_vecCheckedItem[k];
				if ( checkedItemID == itemID )
				{
					bCheckedItem = true;
					break;
				}
			}

			if ( bCheckedItem == false )
				continue;

			if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( itemID ) == false )
				continue;


			map< int, int >::iterator mit;
			mit = m_mapSelectedOption.find( itemID );
			if ( mit != m_mapSelectedOption.end() )
			{
				int attributeIndex = mit->second;

#ifdef SERV_GLOBAL_BILLING
				if (attributeIndex == 0)
					return false;

				if (attributeIndex >= 1 && attributeIndex <= 5)
				{
					if ( kCashPackageInfo.m_vecSocketOption.size() >= attributeIndex )
						mapSelectedOptionList_.insert( std::make_pair( itemID, kCashPackageInfo.m_vecSocketOption[attributeIndex-1] ) );
				}
#else //SERV_GLOBAL_BILLING
				//{{ 2009.01.19 김태완 : 코드정리
				switch(attributeIndex)
				{
				case 0:
					{
						return false;
					} break;
				case 1:
					{
						if ( kCashPackageInfo.m_wstrProductAttribute0.empty() == false )
							mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashPackageInfo.m_wstrProductAttribute0.c_str() ) ) );
					}break;
				case 2:
					{
						if ( kCashPackageInfo.m_wstrProductAttribute1.empty() == false )
							mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashPackageInfo.m_wstrProductAttribute1.c_str() ) ) );
					}break;
				case 3:
					{
						if ( kCashPackageInfo.m_wstrProductAttribute2.empty() == false )
							mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashPackageInfo.m_wstrProductAttribute2.c_str() ) ) );
					}break;
				case 4:
					{
						if ( kCashPackageInfo.m_wstrProductAttribute3.empty() == false )
							mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashPackageInfo.m_wstrProductAttribute3.c_str() ) ) );
					}break;
				case 5:
					{
						if ( kCashPackageInfo.m_wstrProductAttribute4.empty() == false )
							mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashPackageInfo.m_wstrProductAttribute4.c_str() ) ) );
					}break;
				default:
					break;
				}
				//}}
#endif //SERV_GLOBAL_BILLING
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
#ifdef SERV_GLOBAL_BILLING
		int itemID =  kCashOrderInfo.m_kBillProductInfo.m_iProductID;
#else //SERV_GLOBAL_BILLING
		int itemID = _wtoi( kCashOrderInfo.m_wstrProductID.c_str() );
#endif //SERV_GLOBAL_BILLING
		if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( itemID ) == true )
		{

			bool bCheckedItem = false;
			for ( int k = 0; k < (int)m_vecCheckedItem.size(); k++ )
			{
				int checkedItemID = m_vecCheckedItem[k];
				if ( checkedItemID == itemID )
				{
					bCheckedItem = true;
					break;
				}
			}

			if ( bCheckedItem == false )
				return true;

			map< int, int >::iterator mit;
			mit = m_mapSelectedOption.find( itemID );
			if ( mit != m_mapSelectedOption.end() )
			{
				int attributeIndex = mit->second;

#ifdef SERV_GLOBAL_BILLING
				if (attributeIndex == 0)
					return false;

				if (attributeIndex >= 1 && attributeIndex <= 5)
				{
					if ( kCashOrderInfo.m_vecSocketOption.size() >= attributeIndex )
						mapSelectedOptionList_.insert( std::make_pair( itemID, kCashOrderInfo.m_vecSocketOption[attributeIndex-1] ) );
				}
#else //SERV_GLOBAL_BILLING
				if ( attributeIndex == 0 )
				{
					return false;
				}
				else if ( attributeIndex == 1 )
				{
					if ( kCashOrderInfo.m_wstrProductAttribute0.empty() == false )
						mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashOrderInfo.m_wstrProductAttribute0.c_str() ) ) );
				}
				else if ( attributeIndex == 2 )
				{
					if ( kCashOrderInfo.m_wstrProductAttribute1.empty() == false )
						mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashOrderInfo.m_wstrProductAttribute1.c_str() ) ) );
				}
				else if ( attributeIndex == 3 )
				{
					if ( kCashOrderInfo.m_wstrProductAttribute2.empty() == false )
						mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashOrderInfo.m_wstrProductAttribute2.c_str() ) ) );
				}
				else if ( attributeIndex == 4 )
				{
					if ( kCashOrderInfo.m_wstrProductAttribute3.empty() == false )
						mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashOrderInfo.m_wstrProductAttribute3.c_str() ) ) );
				}
				else if ( attributeIndex == 5 )
				{
					if ( kCashOrderInfo.m_wstrProductAttribute4.empty() == false )
						mapSelectedOptionList_.insert( std::make_pair( itemID, _wtoi( kCashOrderInfo.m_wstrProductAttribute4.c_str() ) ) );
				}
#endif //SERV_GLOBAL_BILLING
			}
			else
			{
				return false;
			}
		}
	}
#endif // SERV_CASH_ITEM_SOCKET_OPTION

	return true;
}

#ifdef SERV_CASH_ITEM_SOCKET_OPTION
/** @function : GetSelectedOptionList
	@brief : 캐시아바타를 가져올 때 선택한 소켓 옵션을 얻어오는 함수
	@param : 캐시아바타의 소켓 그룹아이디(wszSocketItemGroupID_), 콤보박스에서 지정된 인덱스(iSelectedIndex_), 가져오려는 아이템ID(iItemID_)
	@return : <GroupID,SocketID>
*/
#ifdef SERV_GLOBAL_BILLING
pair<int,int> CX2CashShop::CashItemToInvenPopup::GetSelectedOptionList( const int iSocketItemGroupID_, const int iSelectedIndex_, const int iItemID_ )
#else //SERV_GLOBAL_BILLING
pair<int,int> CX2CashShop::CashItemToInvenPopup::GetSelectedOptionList( const WCHAR* wszSocketItemGroupID_, const int iSelectedIndex_, const int iItemID_ )
#endif //SERV_GLOBAL_BILLING
{
	vector<int> vecSocketId_;
#ifdef SERV_GLOBAL_BILLING
	const int iSocketGroupID = iSocketItemGroupID_;
#else // SERV_GLOBAL_BILLING
	const int iSocketGroupID = _wtoi( wszSocketItemGroupID_ );
#endif // SERV_GLOBAL_BILLING
	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID_ );
	const int iReturnSocketGroupID 
		=  NULL != pItemTemplet ? g_pData->GetSocketItem()->GetSocketIdListForCashAvatar( iSocketGroupID, vecSocketId_, pItemTemplet->GetItemType() ) : 0;

	int iSelectedSocketID = 0;
	if ( 0 >= iSelectedIndex_ )		/// iSelectedIndex_의 유효한 옵션은 1부터 시작됨
		iSelectedSocketID = 0;
	else if ( vecSocketId_.size() >= static_cast<UINT>( iSelectedIndex_ ) )	/// 1부터 시작 하기 때문에
		iSelectedSocketID = vecSocketId_[iSelectedIndex_ - 1];
	
	return pair<int,int>( iReturnSocketGroupID, iSelectedSocketID );
}
#endif // SERV_CASH_ITEM_SOCKET_OPTION


#ifdef SERV_GLOBAL_BILLING
#ifdef SERV_GLOBAL_CASH_PACKAGE
void CX2CashShop::CashItemToInvenPopup::GetCheckedPackageInfo( vector< KBillPackageInfo >& vecPackageInfo )
{
 	if ( m_vecKBillOrderInfo.empty() == false )
 	{
 		KBillOrderInfo& kKBillOrderInfo = m_vecKBillOrderInfo[0];
 
 		for ( int i = 0; i < (int)m_vecKBillPackageInfo.size(); i++ )
 		{
 			KBillPackageInfo& kKBillPackageInfo = m_vecKBillPackageInfo[i];
 			if ( //** 패키지NO  kKBillPackageInfo.m_iOrderNo == kKBillOrderInfo.m_iOrderNo &&
				kKBillPackageInfo.m_iPackageNo == kKBillOrderInfo.m_iTransNo //&&
 				//kKBillPackageInfo.m_ulProductNo == kKBillOrderInfo.m_kBillProductInfo.m_iProductNo
				)
 			{
 				KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( kKBillPackageInfo.m_ulProductNo );


 				if ( std::find( m_vecCheckedItem.begin(), m_vecCheckedItem.end(), kKBillPackageInfo.m_iProductID ) != m_vecCheckedItem.end() )
 				{
 					vecPackageInfo.push_back( kKBillPackageInfo );
 				}
 			}
 		}
 	}
 
 
 }
#endif //SERV_GLOBAL_CASH_PACKAGE
#else // SERV_GLOBAL_BILLING
void CX2CashShop::CashItemToInvenPopup::GetCheckedPackageInfo( vector< KNXBTPackageInfo >& vecPackageInfo )
{
	if ( m_vecKNXBTOrderInfo.empty() == false )
	{
		KNXBTOrderInfo& kKNXBTOrderInfo = m_vecKNXBTOrderInfo[0];

		for ( int i = 0; i < (int)m_vecKNXBTPackageInfo.size(); i++ )
		{
			KNXBTPackageInfo& kKNXBTPackageInfo = m_vecKNXBTPackageInfo[i];
			if ( kKNXBTPackageInfo.m_ulOrderNo == kKNXBTOrderInfo.m_ulOrderNo &&
				kKNXBTPackageInfo.m_ulPackageProductNo == kKNXBTOrderInfo.m_ulProductNo )
			{
				if ( std::find( m_vecCheckedItem.begin(), m_vecCheckedItem.end(), _wtoi( kKNXBTPackageInfo.m_wstrProductID.c_str() ) ) != m_vecCheckedItem.end() )
				{
					vecPackageInfo.push_back( kKNXBTPackageInfo );
				}
			}
		}
	}


}
#endif // SERV_GLOBAL_BILLING

void CX2CashShop::CashItemToInvenPopup::SetCheckAllItem( bool bCheck )
{
	if ( bCheck == true )
	{
		m_vecCheckedItem = m_vecItemID;
	}
	else
	{
		m_vecCheckedItem.clear();
	}

	ResetNowPage();
}

void CX2CashShop::CashItemToInvenPopup::ChangeCheckItem( CKTDGUIDialogType pDialog, bool bCheck )
{
	if ( pDialog == NULL )
		return;

	CX2CashShop::CashItemToInvenPopupSlot* pCashItemToInvenPopupSlot = (CX2CashShop::CashItemToInvenPopupSlot*)GetPageSlot( pDialog );
	if ( pCashItemToInvenPopupSlot == NULL )
		return;

	int itemIDToFind = pCashItemToInvenPopupSlot->GetItemID();

	if ( bCheck == true )
	{
		bool bFind = false;
		for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
		{
			int itemID = m_vecCheckedItem[i];
			if ( itemID == itemIDToFind )
			{
				bFind = true;
				break;
			}
		}
		if ( bFind == false )
		{
			m_vecCheckedItem.push_back( itemIDToFind );
		}
	}
	else
	{
		for ( int i = 0; i < (int)m_vecCheckedItem.size(); i++ )
		{
			int itemID = m_vecCheckedItem[i];
			if ( itemID == itemIDToFind )
			{
				m_vecCheckedItem.erase( m_vecCheckedItem.begin() + i );
				i--;
				break;
			}
		}
	}

	ResetNowPage();
}

void CX2CashShop::CashItemToInvenPopup::InitbyOpen()
{
	m_mapSelectedOption.clear();
	m_vecCheckedItem = m_vecItemID;

	SetCheckAllItem( true );

	CKTDGUICheckBox* pCheckBoxMoveInven = (CKTDGUICheckBox*)m_pDialog->GetControl( L"g_pCheckBoxCashShop_MoveInventory" );
	if ( pCheckBoxMoveInven != NULL )
	{
		pCheckBoxMoveInven->SetCheckedPure( true );
	}
}

void CX2CashShop::CashItemToInvenPopup::ResetNowPage()
{
	if ( m_pDialog != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDialog->GetControl( L"g_pStaticCashshop_Movewindow_Font" );
		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << GetNowPage() << L"/" << GetMaxPage();
			pStatic->SetString( 0, wstrstm.str().c_str() );
		}
	}

	int j = 0;
	for ( int i = (m_NowPage-1) * m_MaxSlotNumPerPage; i < (m_NowPage)*m_MaxSlotNumPerPage && j < (int)m_vecPageSlot.size() && i < (int)m_vecItemID.size(); i++, j++ )
	{
		int itemID = m_vecItemID[i];
		CX2CashShop::CashItemToInvenPopupSlot* pPageSlot = (CX2CashShop::CashItemToInvenPopupSlot*)m_vecPageSlot[j];
		if ( pPageSlot != NULL )
		{
#ifdef SERV_GLOBAL_BILLING
			if ( m_vecKBillOrderInfo.empty() == false )
				pPageSlot->SetKBillOrderInfo( m_vecKBillOrderInfo[0] );
#else // SERV_GLOBAL_BILLING
			if ( m_vecKNXBTOrderInfo.empty() == false )
				pPageSlot->SetKNXBTOrderInfo( m_vecKNXBTOrderInfo[0] );
#endif // SERV_GLOBAL_BILLING
		}

		bool bCheck = false;
		for ( int j = 0; j < (int)m_vecCheckedItem.size(); j++ )
		{
			int tempItemID = m_vecCheckedItem[j];
			if ( itemID == tempItemID )
			{
				bCheck = true;
				break;
			}
		}

		if ( bCheck == true )
			pPageSlot->SetCheck( true );
		else
			pPageSlot->SetCheck( false );

		//검사해서 이 프로덕트에 맞는 패키지 상품 정보를 셋팅..
//검사해서 이 프로덕트에 맞는 패키지 상품 정보를 셋팅..
		//** 패키지는 풀려서 들어오니까...일단 패키지 관련은 주석처리. 나중에 처리하자
#ifdef SERV_GLOBAL_BILLING
#ifdef SERV_GLOBAL_CASH_PACKAGE
 		if ( m_vecKBillOrderInfo.empty() == false )
 		{
 			for ( int packageIndex = 0; packageIndex < (int)m_vecKBillPackageInfo.size(); packageIndex++ )
 			{
 				KBillPackageInfo& kKBillPackageInfo = m_vecKBillPackageInfo[packageIndex];
 				KBillOrderInfo& kKBillOrderInfo = m_vecKBillOrderInfo[0];
 				//KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( (int)kKBillPackageInfo.m_ulProductNo );
				//CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( kKBillOrderInfo.m_kBillProductInfo.m_iProductID );
				KBillProductInfo* pKBillProductInfo = &kKBillOrderInfo.m_kBillProductInfo;
				
				int test = m_vecItemID[packageIndex];

 				if ( NULL != pKBillProductInfo &&
 					//**OrderNo kKBillPackageInfo.m_ulOrderNo == kKBillOrderInfo.m_ulOrderNo && 
					kKBillPackageInfo.m_iPackageNo == kKBillOrderInfo.m_iTransNo && 
 					//kKBillPackageInfo.m_vecProductNo[packageIndex] == kKBillOrderInfo.m_kBillProductInfo.m_iProductNo &&
 					m_vecItemID[i] == kKBillPackageInfo.m_iProductID
					)
 				{
 					pPageSlot->SetKBillPackageInfo( kKBillPackageInfo );
 					break;
 				}
 			}
 		}
#endif //SERV_GLOBAL_CASH_PACKAGE
#else // SERV_GLOBAL_BILLING
		if ( m_vecKNXBTOrderInfo.empty() == false )
		{
			for ( int packageIndex = 0; packageIndex < (int)m_vecKNXBTPackageInfo.size(); packageIndex++ )
			{
				KNXBTPackageInfo& kKNXBTPackageInfo = m_vecKNXBTPackageInfo[packageIndex];
				KNXBTOrderInfo& kKNXBTOrderInfo = m_vecKNXBTOrderInfo[0];
				if ( kKNXBTPackageInfo.m_ulOrderNo == kKNXBTOrderInfo.m_ulOrderNo && 
					kKNXBTPackageInfo.m_ulPackageProductNo == kKNXBTOrderInfo.m_ulProductNo &&
					m_vecItemID[i] == _wtoi( kKNXBTPackageInfo.m_wstrProductID.c_str() ) )
				{
					pPageSlot->SetKNXBTPackageInfo( kKNXBTPackageInfo );
					break;
				}
			}
		}
#endif // SERV_GLOBAL_BILLING

	}

	CX2PageMgrItem::ResetNowPage();

	j = 0;
	for ( int i = (m_NowPage-1) * m_MaxSlotNumPerPage; i < (m_NowPage)*m_MaxSlotNumPerPage && j < (int)m_vecPageSlot.size() && i < (int)m_vecItemID.size(); i++, j++ )
	{
		int itemID = m_vecItemID[i];
		CX2CashShop::CashItemToInvenPopupSlot* pPageSlot = (CX2CashShop::CashItemToInvenPopupSlot*)m_vecPageSlot[j];
		if ( pPageSlot != NULL )
		{
			map< int, int >::iterator mit;
			mit = m_mapSelectedOption.find( itemID );
			if ( mit != m_mapSelectedOption.end() )
			{
				pPageSlot->SetOption( mit->second );
			}
			else
			{
				pPageSlot->SetOption( 0 );
			}
		}
	}
}

#ifdef SERV_GLOBAL_BILLING
#ifdef SERV_GLOBAL_CASH_PACKAGE
void CX2CashShop::CashItemToInvenPopup::GetPackageInfo( unsigned long ulTranNo, vector< KBillPackageInfo >& vecKBillPackageInfo )
{
	for ( int i = 0; i < (int)m_vecKBillPackageInfo.size(); i++ )
	{
		KBillPackageInfo& kKBillPackageInfo = m_vecKBillPackageInfo[i];
		if ( kKBillPackageInfo.m_iPackageNo == ulTranNo )
			vecKBillPackageInfo.push_back( kKBillPackageInfo );
	}
}
#endif //SERV_GLOBAL_CASH_PACKAGE
#else // SERV_GLOBAL_BILLING
void CX2CashShop::CashItemToInvenPopup::GetPackageInfo( unsigned long ulProductNo, vector< KNXBTPackageInfo >& vecKNXBTPackageInfo )
{
	for ( int i = 0; i < (int)m_vecKNXBTPackageInfo.size(); i++ )
	{
		KNXBTPackageInfo& kKNXBTPackageInfo = m_vecKNXBTPackageInfo[i];
		if ( kKNXBTPackageInfo.m_ulPackageProductNo == ulProductNo )
			vecKNXBTPackageInfo.push_back( kKNXBTPackageInfo );
	}
}
#endif // SERV_GLOBAL_BILLING



CX2CashShop::PresentItemPopup::PresentItemPopup()
{
	m_pDLGPresent = NULL;
	m_ItemID = 0;
	m_bOpen = false;

	m_bChoicedItem = false;
	m_SelectedProductNum = 0;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
#if defined (CLIENT_COUNTRY_CN) || defined (CLIENT_COUNTRY_PH)
	m_iUseCashType = KGlobalCashInfo::GCT_KOG_ELSWORD_CASH;
#elif defined (CLIENT_COUNTRY_TWHK)
	m_iUseCashType = KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH;
#else // CLIENT_COUNTRY_XX
	m_iUseCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif // CLIENT_COUNTRY_XX
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
}

CX2CashShop::PresentItemPopup::~PresentItemPopup()
{
	SAFE_DELETE_DIALOG( m_pDLGPresent );
}

void CX2CashShop::PresentItemPopup::SetOpen( bool bOpen, int itemID )
{
	m_bOpen = bOpen;
	m_ItemID = itemID;

	if ( bOpen == true )
	{
		InitUI();
	}
	else
	{
		if ( m_pDLGPresent != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGPresent, NULL, false );

		m_pDLGPresent = NULL;
	}
}

bool CX2CashShop::PresentItemPopup::GetOpen()
{
	return m_bOpen;
}


void CX2CashShop::PresentItemPopup::UpdateCash()
{
	if ( m_pDLGPresent != NULL )
	{
		CKTDGUIStatic* pStaticMyCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Mycash" );
		if ( pStaticMyCash != NULL )
		{
#ifdef ADD_COMMA_TO_CASH_ITEM		/// 선물하기 때 현재 넥슨 캐시량에 콤마 않찍힌 문제 수정
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
			pStaticMyCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalMainCash() ).c_str() );
#else //SERV_SUPPORT_SEVERAL_CASH_TYPES
			pStaticMyCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetCash() ).c_str() );
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
#else  ADD_COMMA_TO_CASH_ITEM
			wstringstream wstrstm;
			wstrstm << g_pData->GetMyUser()->GetCash();
			pStaticMyCash->SetString( 0, wstrstm.str().c_str() );
#endif ADD_COMMA_TO_CASH_ITEM
		}
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
#if defined( SERV_BILLING_ADD_GASH_POINT )
		pStaticMyCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Mygashpoint" );
		if ( pStaticMyCash != NULL )
		{
			pStaticMyCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH)).c_str() );
		}
		
		if( GetUseCashType() == static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) )
		{
			// 선택된 글자 크게 변경시키기
			CKTDGUIStatic *pStaticDlg = m_pDLGPresent->GetStatic_LUA("g_pStaticCashShop_PresentWindow");
			CKTDGUIControl::CPictureData *pPicText = pStaticDlg->GetPictureIndex(101);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(111);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(102);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(112);
			pPicText->SetShow(true);

			pStaticMyCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_MyrestGashpoint" );
			if ( pStaticMyCash != NULL )
				pStaticMyCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_CASH)).c_str() );
			pStaticMyCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Myrest" );
		}
		else
		{
			// 선택된 글자 크게 변경시키기
			CKTDGUIStatic *pStaticDlg = m_pDLGPresent->GetStatic_LUA("g_pStaticCashShop_PresentWindow");
			CKTDGUIControl::CPictureData *pPicText = pStaticDlg->GetPictureIndex(101);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(111);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(102);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(112);
			pPicText->SetShow(false);

			pStaticMyCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Myrest" );
			if ( pStaticMyCash != NULL )
				pStaticMyCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH)).c_str() );
			pStaticMyCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_MyrestGashpoint" );
		}
		
		CKTDGUIStatic* pStaticRemainCash;
		if( GetUseCashType() == static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) )
		{
			 pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_MyrestGashpoint" );
			 if ( pStaticRemainCash != NULL )
				pStaticRemainCash->SetString( 0,g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) ).c_str() );
			 pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Myrest" );
		}
		else
		{
			pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Myrest" );
			if ( pStaticRemainCash != NULL )
				pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) ).c_str() );
			pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_MyrestGashpoint" );
		}
#elif defined( CHINA_CASH_CHANGE )
		pStaticMyCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_MyBonuspoint" );
		if ( pStaticMyCash != NULL )
		{
			pStaticMyCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT ) ).c_str() );
		}

		CKTDGUIStatic* pStaticRemainCash;
		if( GetUseCashType() == static_cast<int>( KGlobalCashInfo::GCT_KOG_ELSWORD_CASH ) )
		{
			// 선택된 글자 크게 변경시키기
			CKTDGUIStatic *pStaticDlg = m_pDLGPresent->GetStatic_LUA("g_pStaticCashShop_PresentWindow");
			CKTDGUIControl::CPictureData *pPicText = pStaticDlg->GetPictureIndex(101);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(111);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(102);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(112);
			pPicText->SetShow(true);

			pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_MyrestBonuspoint" );
			if ( pStaticRemainCash != NULL )
			{
				pStaticRemainCash->SetString( 0,g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT ) ).c_str() );
			}
			pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Myrest" );
			
		}
		else
		{
			// 선택된 글자 크게 변경시키기
			CKTDGUIStatic *pStaticDlg = m_pDLGPresent->GetStatic_LUA("g_pStaticCashShop_PresentWindow");
			CKTDGUIControl::CPictureData *pPicText = pStaticDlg->GetPictureIndex(101);
			pPicText->SetShow(false);
			pPicText = pStaticDlg->GetPictureIndex(111);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(102);
			pPicText->SetShow(true);
			pPicText = pStaticDlg->GetPictureIndex(112);
			pPicText->SetShow(false);

			pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Myrest" );
			if ( pStaticRemainCash != NULL )
			{
				pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalMainCash() ).c_str() );
			}
			pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_MyrestBonuspoint" );
			
		}
#else 
		CKTDGUIStatic* pStaticRemainCash = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashshop_PresentWindow_Myrest" );
#endif
#endif SERV_SUPPORT_SEVERAL_CASH_TYPES
		if ( pStaticRemainCash != NULL )
		{
			//현재 있는 돈에서 - 선택된 아이템의 가격


			unsigned long productNo = GetSelectedProductNo();
			if ( productNo == 0 )
			{
#ifdef ADD_COMMA_TO_CASH_ITEM		/// 선물하기 때 현재 넥슨 캐시량에 콤마 않찍힌 문제 수정
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
				int restCash = 0;
				switch(GetUseCashType())
				{
				case KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH:
					pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH ) ).c_str() );
					break;
				case KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT:
					pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT ) ).c_str() );
					break;
				default:
					pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalMainCash() ).c_str() );
					break;
				}
#else 
				pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) ).c_str() );
#endif SERV_SUPPORT_SEVERAL_CASH_TYPES
#else  ADD_COMMA_TO_CASH_ITEM
				wstringstream wstrstm;
				wstrstm << g_pData->GetMyUser()->GetCash();
				pStaticRemainCash->SetString( 0, wstrstm.str().c_str() );
#endif ADD_COMMA_TO_CASH_ITEM
			}
			else
			{
#ifdef SERV_GLOBAL_BILLING
				KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( productNo );
				if ( pKBillProductInfo != NULL )
				{
					wstringstream wstrstm;
					int remainCash = 0; 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
					switch( GetUseCashType() )
					{
					case KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH:
						remainCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) - pKBillProductInfo->m_iSalePrice;
						break;
					case KGlobalCashInfo::GCT_PUBLISHER_CASH:
						remainCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) - pKBillProductInfo->m_iSalePrice;
						break;
					case KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT:
						remainCash = g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT ) - pKBillProductInfo->m_iSalePrice;
						break;
					default:
						remainCash = g_pData->GetMyUser()->GetGlobalMainCash() - pKBillProductInfo->m_iSalePrice;
						break;
					}

					if ( remainCash < 0 )
					{
						remainCash = abs( remainCash );
						wstrstm << L"-" << g_pMain->GetEDString( remainCash );
					}
					else
					{
						wstrstm << g_pMain->GetEDString( remainCash ); 
					}
					pStaticRemainCash->SetString( 0, wstrstm.str().c_str() );
#endif SERV_SUPPORT_SEVERAL_CASH_TYPES
				}
#else // SERV_GLOBAL_BILLING
				KNXBTProductInfo* pKNXBTProductInfo = g_pData->GetItemManager()->GetCashItemProduct( productNo );
				if ( pKNXBTProductInfo != NULL )
				{
#ifdef ADD_COMMA_TO_CASH_ITEM		/// 선물하기 때 현재 넥슨 캐시량에 콤마 않찍힌 문제 수정
					int remainCash = g_pData->GetMyUser()->GetCash() - pKNXBTProductInfo->m_ulSalePrice;
					if ( remainCash <= 0 )
						remainCash = 0;

					pStaticRemainCash->SetString( 0, g_pMain->GetEDString( remainCash ).c_str() );
#else  ADD_COMMA_TO_CASH_ITEM
					wstringstream wstrstm;
					int remainCash = g_pData->GetMyUser()->GetCash() - pKNXBTProductInfo->m_ulSalePrice;
					if ( remainCash <= 0 )
						remainCash = 0;

					wstrstm << remainCash; 
					pStaticRemainCash->SetString( 0, wstrstm.str().c_str() );
#endif ADD_COMMA_TO_CASH_ITEM
				}
#endif // SERV_GLOBAL_BILLING
				else
				{
#ifdef ADD_COMMA_TO_CASH_ITEM		/// 선물하기 때 현재 넥슨 캐시량에 콤마 않찍힌 문제 수정
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef CLIENT_COUNTRY_TW
					if( GetUseCashType() == static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) )
					{
						pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) ).c_str() );
					}
					else
					{
						pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) ).c_str() );
					}
#else
					pStaticRemainCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalMainCash() ).c_str() );
#endif CLIENT_COUNTRY_TW
#endif SERV_SUPPORT_SEVERAL_CASH_TYPES
#else  ADD_COMMA_TO_CASH_ITEM
					wstringstream wstrstm;
					wstrstm << g_pData->GetMyUser()->GetCash();
					pStaticRemainCash->SetString( 0, wstrstm.str().c_str() );
#endif ADD_COMMA_TO_CASH_ITEM
				}
			}

		}
	}
}


unsigned long CX2CashShop::PresentItemPopup::GetSelectedProductNo()
{
	if ( m_pDLGPresent != NULL )
	{
		CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGPresent->GetControl( L"g_pComboCash_Shop_Present_Option" );
		if ( pComboBox != NULL )
		{
			int selectedIndex = pComboBox->GetSelectedItemIndex();
			if ( selectedIndex <= 0 )
				return 0;
			else
			{
				//여기서 거시기 해부러야제??
				//목록에 맞게끔 인덱싱해서 리턴해줘불자잉~~~


				int productIndex = selectedIndex - 1;

				CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( m_ItemID );
				if ( pCashItem != NULL )
				{
#ifdef SERV_GLOBAL_BILLING
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
					vector< KBillProductInfo > vecBillInfoTemp;
					if( true == g_pInstanceData->IsChoicedItem() )
					{
						vecBillInfoTemp = pCashItem->m_vecKBillProductInfo;
					}
					else
					{
						vecBillInfoTemp = g_pInstanceData->GetCurrentProductInfoListInNowCatagory( pCashItem->m_vecKBillProductInfo );
					}

					if ( productIndex >= 0 && (int)vecBillInfoTemp.size() > productIndex )
					{
						KBillProductInfo& kBillProductInfo = vecBillInfoTemp[productIndex];
						return kBillProductInfo.m_iProductNo;
					}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
					if ( productIndex >= 0 && (int)pCashItem->m_vecKBillProductInfo.size() > productIndex )
					{
						KBillProductInfo& kBillProductInfo = pCashItem->m_vecKBillProductInfo[productIndex];
						return kBillProductInfo.m_iProductNo;
					}
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
#else // SERV_GLOBAL_BILLING
					if ( productIndex >= 0 && (int)pCashItem->m_vecKNXBTProductInfo.size() > productIndex )
					{
						KNXBTProductInfo& kNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[productIndex];
						return kNXBTProductInfo.m_ulProductNo;
					}
#endif // SERV_GLOBAL_BILLING
				}
			}
		}
	}

	return 0;
}

void CX2CashShop::PresentItemPopup::SetSelectFriend( int index )
{
	if ( index == 0 )
		return;

	if ( m_pDLGPresent != NULL )
	{
		CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGPresent->GetControl( L"g_pComboBoxPresent_Select_User" );
		if ( pComboBox != NULL )
		{
			CKTDGUIComboBox::ComboBoxItem* pItem = pComboBox->GetItem( index );
			if ( pItem != NULL )
			{
				// 선물할 유저 텍스트 박스에다가 심어주자 해당 str..pItem->strText

				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGPresent->GetControl( L"IME_Nicname" );
				if ( pIMEEditBox != NULL )
				{
					return pIMEEditBox->SetText( pItem->strText );
				}
			}
		}
	}
}

wstring CX2CashShop::PresentItemPopup::GetNickNameToPresent()
{
	if ( m_pDLGPresent != NULL )
	{
		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGPresent->GetControl( L"IME_Nicname" );
		if ( pIMEEditBox != NULL )
		{
			wstring tempString = pIMEEditBox->GetText();
			tempString = tempString.substr( 0, 20 );

			return tempString.c_str();
		}
	}

	return L"";
}	

wstring CX2CashShop::PresentItemPopup::GetPresentMsg()
{
	if ( m_pDLGPresent != NULL )
	{
		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGPresent->GetControl( L"IME_Present_Message" );
		if ( pIMEEditBox != NULL )
		{
			wstring tempString = pIMEEditBox->GetText();
			tempString = tempString.substr( 0, 20 );

			return tempString.c_str();
		}
	}

	return L"";
}

wstring CX2CashShop::PresentItemPopup::GetPassword()
{
	if ( m_pDLGPresent != NULL )
	{
		CKTDGUIEditBox* pIMEEditBox = (CKTDGUIEditBox*)m_pDLGPresent->GetControl( L"IME_Password" );
		if ( pIMEEditBox != NULL )
		{
			wstring tempString = pIMEEditBox->GetText();
			tempString = tempString.substr( 0, 20 );

			return tempString.c_str();
		}
	}

	return L"";
}

void CX2CashShop::PresentItemPopup::InitUI()
{
	if ( m_pDLGPresent != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGPresent, NULL, false );

	m_pDLGPresent = NULL;
#if defined CLIENT_COUNTRY_TW
	m_pDLGPresent = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Popup_Present_TW.lua" );
#elif defined CLIENT_COUNTRY_HK
	m_pDLGPresent = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Popup_Present_HK.lua" );
#elif defined ( CLIENT_COUNTRY_CN )
	m_pDLGPresent = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Popup_Present_CN.lua" );
#else 
	m_pDLGPresent = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Popup_Present.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPresent );

#if defined(SERV_TOONILAND_CHANNELING) && !defined(SERV_STEAM)
	if( NULL != g_pData && NULL != g_pData->GetMyUser() )
	{
		if( g_pData->GetMyUser()->GetUserData().m_uChannelCode == 3 )
		{
			// 투니랜드 유저라면 선물하기 비번입력창 안보이도록			
			CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashShop_PresentWindow" );
			if( pStatic != NULL )
			{
				CKTDGUIControl::CPictureData *pPicNexonPassword = pStatic->GetPicture(12);
				if( pPicNexonPassword != NULL )
					pPicNexonPassword->SetShow(false);

				pPicNexonPassword = pStatic->GetPicture(17);
				if( pPicNexonPassword != NULL )
					pPicNexonPassword->SetShow(false);
			}
			CKTDGUIEditBox *pPasswordEditBox = (CKTDGUIEditBox*)m_pDLGPresent->GetControl(L"IME_Password");
			if( pPasswordEditBox != NULL )
				pPasswordEditBox->SetShowEnable(false, false);
		}
	}
#endif

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_ItemID );
    bool    bItemTempletValid = pItemTemplet != NULL;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
#if defined(CLIENT_COUNTRY_CN) || defined(CLIENT_COUNTRY_PH)
	SetUseCashType((int)KGlobalCashInfo::GCT_KOG_ELSWORD_CASH);
#elif defined(CLIENT_COUNTRY_TWHK)
	SetUseCashType((int)KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH);
#else // CLIENT_COUNTRY_TWHK
	SetUseCashType((int)KGlobalCashInfo::GCT_PUBLISHER_CASH);
#endif // CLIENT_COUNTRY_TWHK
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	//아이템 이름 셋팅
	CKTDGUIStatic* pItemName = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStatic_Item_ID_1Line" );
	if ( pItemName != NULL && pItemName->GetString(0) != NULL && bItemTempletValid == true )
	{
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring tempName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetName(), 214, pItemName->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
        wstring tempName = g_pMain->GetStrByLienBreak( pItemTemplet->GetName(), 214, pItemName->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
		pItemName->SetString( 0, tempName.c_str() );
	}

	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGPresent->GetControl( L"g_pStaticCashShop_PresentWindow_ItemSlot" );
	if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL && bItemTempletValid == true )
	{
        pStaticSlot->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() );
	}

	CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGPresent->GetControl( L"g_pComboCash_Shop_Present_Option" );
	if ( pComboBox != NULL )
	{
		pComboBox->RemoveAllItems();
		CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( m_ItemID );

#if defined ( SERV_GLOBAL_BILLING ) && defined ( USE_PERMANENT_INSTEAD_OF_1EA )
		bool bPeriod = false;
		for ( int i = 0; i < (int)pCashItem->m_vecKBillProductInfo.size(); i++ )
		{
			KBillProductInfo& kBillProductInfo = pCashItem->m_vecKBillProductInfo[i];
			if( kBillProductInfo.m_cPeriod != 0 )
			{
				bPeriod = true;
				break;
			}
		}
#endif // ( SERV_GLOBAL_BILLING ) && defined ( USE_PERMANENT_INSTEAD_OF_1EA )

#ifdef SERV_GLOBAL_BILLING
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
		bool bFirstProduct = true;
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
		for ( int i = 0; i < (int)pCashItem->m_vecKBillProductInfo.size(); i++ )
		{
			KBillProductInfo& kBillProductInfo = pCashItem->m_vecKBillProductInfo[i];
			wstringstream wstrstm;
			//wstrstm << kBillProductInfo

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
			if( GetChoicedItem() == true )
			{
				// 찜목록 아이템은 가격 다 보여준다
			}
			else if( false == g_pInstanceData->IsCurrentSubCategoryInNowCatagory(kBillProductInfo.m_cCategoryNo) )
				continue;
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

			if ( kBillProductInfo.m_cPeriod != 0 )
			{
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
				if ( true == bFirstProduct )
				{
					bFirstProduct = false;
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );
				}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
				if ( i == 0 )
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

				wstrstm << (int)kBillProductInfo.m_cPeriod << GET_STRING( STR_ID_14 ) << L"/"; 
			}
			else
			{
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
				if ( true == bFirstProduct )
				{
					bFirstProduct = false;
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );
				}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
				if ( i == 0 )
					pComboBox->AddItem( GET_STRING( STR_ID_36 ), NULL, false );
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef USE_PERMANENT_INSTEAD_OF_1EA
				if( bPeriod == true && kBillProductInfo.m_cQuantity == 1 )
					wstrstm << GET_STRING( STR_ID_13858 ) << L"/";
				else
#endif USE_PERMANENT_INSTEAD_OF_1EA
				wstrstm << (int)kBillProductInfo.m_cQuantity << GET_STRING( STR_ID_24 ) << L"/";
			}

			wstrstm << kBillProductInfo.m_iSalePrice << GET_STRING( STR_ID_34 ); 

			pComboBox->AddItem( wstrstm.str().c_str(), NULL, false );
		}
#else // SERV_GLOBAL_BILLING
		for ( int i = 0; i < (int)pCashItem->m_vecKNXBTProductInfo.size(); i++ )
		{
			KNXBTProductInfo& kNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[i];
			wstringstream wstrstm;
			//wstrstm << kNXBTProductInfo

			if ( kNXBTProductInfo.GetProductExpire() != 0 )
			{
				if ( i == 0 )
					pComboBox->AddItem( GET_STRING( STR_ID_35 ), NULL, false );

				wstrstm << kNXBTProductInfo.GetProductExpire() << GET_STRING( STR_ID_14 ) << L"/"; 
			}
			else
			{
				if ( i == 0 )
					pComboBox->AddItem( GET_STRING( STR_ID_36 ), NULL, false );

				wstrstm << kNXBTProductInfo.m_usProductPieces << GET_STRING( STR_ID_24 ) << L"/";
			}

			wstrstm << kNXBTProductInfo.m_ulSalePrice << GET_STRING( STR_ID_34 ); 

			pComboBox->AddItem( wstrstm.str().c_str(), NULL, false );
		}
#endif // SERV_GLOBAL_BILLING
	}


	pComboBox = (CKTDGUIComboBox*)m_pDLGPresent->GetControl( L"g_pComboBoxPresent_Select_User" );
	if ( pComboBox != NULL )
	{
		pComboBox->RemoveAllItems();
		pComboBox->AddItem( GET_STRING( STR_ID_39 ), NULL );

		vector< wstring > vecFriendNameList;
		g_pData->GetMessenger()->GetFriendNameList( vecFriendNameList );
		for ( int i = 0; i < (int)vecFriendNameList.size(); i++ )
		{
			wstring friendName = vecFriendNameList[i];
			pComboBox->AddItem( friendName.c_str(), NULL );
		}
	}

	UpdateCash();
}

#ifdef CHINA_CASH_CHANGE
void CX2CashShop::ExchangeChinaCash(int _icash)
{
	CKTDGUIButton* pCash_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_ok" );
	if(_icash > 0)
	{
		pCash_OK->SetEnable(true);

		CKTDGUIStatic* pStaticElsowrdCash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_pStaticElswordcash_num" );

		CKTDGUIStatic* pStaticGiantCash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_pStaticGiantcash_num" );

		CKTDGUIStatic* pStaticChangeCash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_pStaticChangecash_num" );

		CKTDGUIStatic* pStaticChangeBingon = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"Exchange_cash_Bingon" );
		//pStaticChangeBingon->SetShowEnable(false,false);

		pStaticElsowrdCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_CASH ) ).c_str() );
		pStaticGiantCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_CASH ) ).c_str() );

		CKTDGUIStatic* pStaticChangeFont_Giantcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GIANTPOINT" );
		pStaticChangeFont_Giantcash->SetShowEnable(false,false);

		CKTDGUIStatic* pStaticChangeFont_Goldcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GOLDCOIN" );
		pStaticChangeFont_Goldcash->SetShowEnable(false,false);

		CKTDGUIStatic* pStaticChangeFONT_AFTER_GIANT = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_GIANT" );
		pStaticChangeFONT_AFTER_GIANT->SetShowEnable(true,true);

		CKTDGUIStatic* pStaticChangeFONT_AFTER_ELS_CASH = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_ELS_CASH" );
		pStaticChangeFONT_AFTER_ELS_CASH->SetShowEnable(true,true);

		m_bCashlistCheck = true;

		int restCash = 0;
		int cashsub = g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_CASH ) - (_icash*10); // 자이언트 캐쉬 차감 금액 표시
		if(pStaticElsowrdCash != NULL && pStaticGiantCash != NULL && pStaticChangeCash != NULL)
		{
			pStaticChangeCash->SetString( 0, GET_REPLACED_STRING( ( STR_ID_11307, "i", _icash ) ) );
			if(cashsub < 0)
			{
				pStaticGiantCash->SetColor(D3DXCOLOR(1,0,0,1));
				restCash = g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_CASH ) - (_icash*10);
				restCash = abs(restCash);
				wstringstream wstrstm;
				wstrstm << L"-" << g_pMain->GetEDString( restCash ).c_str();
				pStaticGiantCash->SetString( 0, wstrstm.str().c_str() );
				pStaticChangeBingon->SetShowEnable(true,true);
				m_bLowCashCheck = true;
				m_uiSubcash = 0;

			}
			else
			{
				pStaticGiantCash->SetColor(D3DXCOLOR(1.05f,1,0,1));
				pStaticGiantCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_CASH ) - (_icash*10)).c_str() );
				pStaticChangeBingon->SetShowEnable(false,false);
				m_bLowCashCheck = false;
				m_uiSubcash = _icash*10;
			}

			pStaticElsowrdCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_CASH ) + _icash).c_str() );

		}
	}
	else
	{
		pCash_OK->SetEnable(false);

		CKTDGUIStatic* pStaticElsowrdCash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_pStaticElswordcash_num" );
		CKTDGUIStatic* pStaticGiantCash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_pStaticGiantcash_num" );

		pStaticElsowrdCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_CASH ) ).c_str() );
		pStaticGiantCash->SetString( 0, g_pMain->GetEDString( g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_PUBLISHER_CASH ) ).c_str() );
	}
}

void CX2CashShop::ChinaCash_Tap_Change(bool _bShow)
{
	bool _bshowTemp; 

	CKTDGUIStatic* pStaticChangeFont_Giantcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GIANTPOINT" );
	CKTDGUIStatic* pStaticChangeFont_Goldcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GOLDCOIN" );
	CKTDGUIStatic* pStaticChangeFONT_AFTER_GIANT = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_GIANT" );
	CKTDGUIStatic* pStaticChangeFONT_AFTER_ELS_CASH = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_ELS_CASH" );

	pStaticChangeFont_Giantcash->SetShowEnable(false,false);
	pStaticChangeFont_Goldcash->SetShowEnable(false,false);

	if(_bShow == true)
	{
		_bshowTemp = false;
	}
	else
	{
		_bshowTemp = true;
	}

	if(m_bCashlistCheck == true)
	{
		pStaticChangeFONT_AFTER_GIANT->SetShowEnable(true,true);
		pStaticChangeFONT_AFTER_ELS_CASH->SetShowEnable(true,true);
	}
	else if(m_bCashlistCheck == false)
	{
		pStaticChangeFont_Giantcash->SetShowEnable(true,true);
		pStaticChangeFont_Goldcash->SetShowEnable(true,true);
		pStaticChangeFONT_AFTER_GIANT->SetShowEnable(false,false);
		pStaticChangeFONT_AFTER_ELS_CASH->SetShowEnable(false,false);
	}

	CKTDGUIStatic* pStatic_Graphic = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Elsword_Tap" );
	pStatic_Graphic->SetShowEnable(_bShow,_bShow);
	CKTDGUIStatic* pStatic_Giant_Tap = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Giant_Tap" );
	pStatic_Giant_Tap->SetShowEnable(_bshowTemp,_bshowTemp);

	CKTDGUIComboBox* pCombo_cash= (CKTDGUIComboBox*) m_pDLGCashchange->GetControl( L"CashChangeList" );
	pCombo_cash->SetShowEnable(_bShow, _bShow);

	CKTDGUIButton* pCash_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_ok" );
	pCash_OK->SetShowEnable(_bShow, _bShow);

	CKTDGUIButton* pCash_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_cancel" );
	pCash_CANCEL->SetShowEnable(_bShow, _bShow);

	CKTDGUIButton* pCash_Giant_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_ok" );
	pCash_Giant_OK->SetShowEnable(_bshowTemp, _bshowTemp);

	CKTDGUIButton* pCash_Giant_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_cancel" );
	pCash_Giant_CANCEL->SetShowEnable(_bshowTemp, _bshowTemp);

	CKTDGUIStatic* pStaticChangeBingon = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"Exchange_cash_Bingon" );


	if(m_bLowCashCheck == true && _bShow == true)
	{
		pStaticChangeBingon->SetShow(true);
	}
	else if(m_bLowCashCheck == false && _bShow == true)
	{
		pStaticChangeBingon->SetShow(false);
	}
	else
	{
		pStaticChangeBingon->SetShow(false);
	}
}

bool CX2CashShop::Handler_EGS_EXCHANGE_REQ() // 2011.03.08 lygan_조성욱 // 자이언트 캐쉬 전환 관련
{
	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2( 279, 300 ), GET_STRING( STR_ID_2558 ), (CKTDXStage*) m_pNowState );
	KEGS_EXCHANGE_CASH_REQ kPacket;

	kPacket.m_uiExchangeValue = m_uiSubcash;

	g_pData->GetServerProtocol()->SendPacket( EGS_EXCHANGE_CASH_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_EXCHANGE_CASH_ACK );

	return true;
}

bool CX2CashShop::Handler_EGS_EXCHANGE_ACK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // 2011.03.08 lygan_조성욱 // 자이언트 캐쉬 전환 관련
{
	SAFE_DELETE_DIALOG(m_pDLGMsgBox);

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXCHANGE_CASH_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_EXCHANGE_CASH_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_NOTIFY_BONUS_CASH
			unsigned long ulAddBonus = kEvent.m_AddBonusCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT];

			if( m_bCheckCashEvent == true && ulAddBonus > 0 )	// 이벤트 중인지 아닌지를 체크 && 보너스 포인트 지급되는지 체크
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 279, 300 ), GET_REPLACED_STRING( ( STR_ID_14478, "ii", kEvent.m_iPoint, ulAddBonus ) ), (CKTDXStage*) m_pNowState );
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 279, 300 ), GET_REPLACED_STRING( ( STR_ID_11342, "i", kEvent.m_iPoint ) ), (CKTDXStage*) m_pNowState );
			}
#endif SERV_NOTIFY_BONUS_CASH

			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->Handler_EGS_CHECK_BALANCE_REQ();
			return true;
		}
	}
	return false;
}
#endif //CHINA_CASH_CHANGE

#ifdef FIX_MISSING_MAIL
void CX2CashShop::AddDelayedMail( KEGS_RECEIVE_LETTER_NOT& kEvent )
{
	m_vecDelayedMail.push_back( kEvent );
}

void CX2CashShop::PushDelayedMails()
{
	if( g_pMain && g_pMain->GetPostBox() != NULL )
	{
		for( int i = 0; i < (int)m_vecDelayedMail.size(); ++i )
		{
			KSerBuffer buff;
			Serialize( &buff, &m_vecDelayedMail[i] );

			g_pMain->GetPostBox()->Handler_EGS_RECEIVE_LETTER_NOT( 0, 0, 0, (LPARAM)&buff );
		}
		m_vecDelayedMail.resize( 0 );
	}
}
#endif FIX_MISSING_MAIL

#ifdef SERV_EVENT_DAILY_GIFT_BOX
bool CX2CashShop::SetDialyGiftBoxPopup( int ItemID )
{
	for(int i = 0; i < ARRAY_SIZE( DAILY_GIFT_BOX_ITEM ); ++i)
	{
		if( ItemID == DAILY_GIFT_BOX_ITEM[i] )
		{
			if( ItemID == DAILY_GIFT_BOX_ITEM[1] )			//A. 아리엘의 선물상자
			{
				m_pDlgDailyGiftBox = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_POPUP_ARIEL.lua" );
			}
			else if( ItemID == DAILY_GIFT_BOX_ITEM[0] )		//B. 루리엘의 선물상자
			{
				m_pDlgDailyGiftBox = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_POPUP_RURIEL.lua" );
			}

			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgDailyGiftBox );

			return true;
		}
	}

	return false;
}
#endif SERV_EVENT_DAILY_GIFT_BOX

#ifdef PET_PREVIEW
void CX2CashShop::MyUniyHandling( double fTime, float fElapsedTime )
{
	if( m_pUnitViewerUI != NULL )
	{
		float fRotSpeed = 13.0f;


		if ( CX2UnitViewerUI::GetUnitClicked() == true )
		{
			D3DXVECTOR3 RotateY = D3DXVECTOR3(0, -((float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() * m_fMouseSensitivity), 0);
			m_pUnitViewerUI->GetMatrix().RotateRel(RotateY);
			m_bRotateReturn = false;

			fRotSpeed = 13.0f;
		}
		else
		{
			D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
			m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );



			m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
			int addSpeed = 20;

			if ( m_bRotateReturn == false )
			{
				fRotSpeed -= (fElapsedTime * addSpeed);

				if ( fRotSpeed <= 0 )
				{
					fRotSpeed = fElapsedTime;
				}

				if (m_fRemainRotY >= 0.0f &&  m_fRemainRotY < 3.14f )
				{

					if ( m_fRemainRotY <= 0.0f )
					{
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						m_fRemainRotY = 0.0f;
						m_bRotateReturn = true;
					}
					else
					{
						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY <= 0.0f )
						{
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							m_fRemainRotY = 0.0f;
							m_bRotateReturn = true;
						}
					}
				}
				else if ( m_fRemainRotY > 3.14f && m_fRemainRotY <= 6.18f  )
				{
					if ( m_fRemainRotY >= 6.18f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY += (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY >= 6.18f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
				else if ( m_fRemainRotY >= -3.14f && m_fRemainRotY <= 0.0f )
				{
					if ( m_fRemainRotY >= 0.0f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY += (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY >= 0.0f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
				else if (  m_fRemainRotY >= -6.28f && m_fRemainRotY <= -3.14f )
				{
					if ( m_fRemainRotY <= -6.28f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY <= -6.28f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}

				}
			}
		}

		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );

		if ( m_pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
		{
			m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
		}
	}
}

/** @function : PetHandling
	@breif : 펫 모션 동작을 위한 프레임무브 연결
*/
void CX2CashShop::PetHandling( double fTime, float fElapsedTime )
{
	m_pNPCViewerUI->OnFrameMove( fTime, fElapsedTime );
}

/** @UpdateDLGNpcViewerUI : UpdateDLGNpcViewerUI
	@biref	: 펫 유닛뷰어 객체 생성 및 유닛뷰어 관리할 다이얼로그 생성
*/
void CX2CashShop::UpdateDLGPetViewerUI()
{
	if( NULL == m_pNPCViewerUI )
	{
		m_pNPCViewerUI = CX2NPCUnitViewerUI::Create();
	}

	if( NULL == m_pDLGPetPreview )
	{
		m_pDLGPetPreview = new CKTDGUIDialog( g_pMain->GetNowState(), NULL);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPetPreview );
		m_pDLGPetPreview->SetLayer(XDL_NORMAL_2);
		m_pDLGPetPreview->SetFront(true);
		if( NULL != m_pNPCViewerUI )
		{
			m_pDLGPetPreview->SetHasUnit(static_cast<CKTDGObject*>(m_pNPCViewerUI));
		}
	}
	ResetPetViewer();
}
void CX2CashShop::PetPreview( int iItemID_ )
{
	if ( NULL != g_pData->GetItemManager() )
	{
		int iPetID = g_pData->GetItemManager()->GetPreviewPetIdByItemID( iItemID_ );

		CX2ItemManager::PREVIEW_PET_TYPE ePreviewPetType_ = CX2ItemManager::PPT_PET;		
		// 일반 펫 정보중에 없다면 라이딩펫인지 체크
		if( -1 == iPetID )
		{ 
			iPetID = g_pData->GetItemManager()->GetPreviewRidingPetIdByItemID( iItemID_ );
			// 라이딩펫도 아니라면 불러오지 않기.
			if( -1 == iPetID )
				return;

			ePreviewPetType_ = CX2ItemManager::PPT_RIDING_PET;
		}

		ResetPetViewer( iPetID, iItemID_, ePreviewPetType_);
	}
}

/** @function : ResetPetViewer
	@breif : 선택한 펫으로 펫 유닛 뷰어 갱신
	@param : 펫 ID (iPetID_), 연결된 아이템 ID (iItemID)		 
*/
void CX2CashShop::ResetPetViewer(int iPetID_/* = -1*/, int iItemID /*= -1*/, CX2ItemManager::PREVIEW_PET_TYPE ePreviewPetType_/* = PPT_PET*/)
{
	if( NULL != m_pNPCViewerUI )
	{
		if( iPetID_ < 0 || m_iPrePetItemID == iItemID )
		{
			m_pNPCViewerUI->SetShowObject(false);
			m_iPrePetItemID = -1;
			return;
		}
		else
		{
			m_iPrePetItemID = iItemID; //장착된 펫을 한번 더 누르면 해제시키기 위한 값
			switch( ePreviewPetType_ )
			{
			case CX2ItemManager::PPT_RIDING_PET:
				{
					m_pNPCViewerUI->ResetRidingPet( iPetID_ );
					m_pNPCViewerUI->SetScale( D3DXVECTOR3(0.5f,0.5f,0.5f));
				} break;
			case CX2ItemManager::PPT_PET:
			default:
				{
#ifdef PET_PREVIEW_BUG_FIX
					CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)iPetID_ );
					m_pNPCViewerUI->ResetPet( iPetID_ , pTemplet->m_Evolution_All_Step );
#else
					m_pNPCViewerUI->ResetPet( iPetID_ , 3 );
#endif PET_PREVIEW_BUG_FIX
					m_pNPCViewerUI->SetScale( D3DXVECTOR3(1.f,1.f,1.f));
				} break;
			}
			m_pNPCViewerUI->SetShowObject( true );
			m_pNPCViewerUI->PlayAnim( L"Wait", CKTDGXSkinAnim::XAP_LOOP, true );
			m_pNPCViewerUI->SetOverUI(true);
			m_pNPCViewerUI->SetPosition(D3DXVECTOR3(-270,0,0));
		}
	}
}
#endif //PET_PREVIEW

/** @function : SetShowOtherUI
	@param : 캐쉬샵 열고 닫은 상태에 따라 show상태 변경해야 되는 UI 갱신
*/
void CX2CashShop::SetShowOtherUI( bool bShowCashop_ )
{
	if( NULL != g_pInstanceData && NULL != g_pInstanceData->GetMiniMapUI() )
		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, !bShowCashop_ );

	if( NULL != g_pX2Game && NULL != g_pX2Game->GetDLGMyScore() )
		g_pX2Game->GetDLGMyScore()->SetShow(!bShowCashop_);
}


#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD		
//필드 입장시, 해당 필드 내에 캐쉬샵 입장중인 유저에 대한 처리
//입장 시점에 X2Game이 생성되어 있지 않기 때문에 임시 저장
void CX2CashShop::GetEnterCashShopUserList( OUT std::vector< UidType >& EnterCashShopUserList_ )
{
	EnterCashShopUserList_.clear();
	EnterCashShopUserList_ = m_vecEnterCashShopUser;
}
#endif //SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD

#ifdef TAKE_OFF_ALL_ITEM
void CX2CashShop::ShowInvenEquipUI( bool bVal_ )
{
	// true : 현재 보이는 상태이므로 '숨기기' UI 보여주기
	// false : 현재 안보이는 상태이므로 '보이기' UI 보여주기

	if( NULL != m_pDLGFront )
	{
		CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>(m_pDLGFront->GetControl(L"MyAvatar_Show"));
		if( NULL != pButton )
			pButton->SetShow( !bVal_ );

		pButton = static_cast<CKTDGUIButton*>(m_pDLGFront->GetControl(L"MyAvatar_Hide"));
		if( NULL != pButton )
			pButton->SetShow( bVal_ );
	}
}
#endif // TAKE_OFF_ALL_ITEM

#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
void CX2CashShop::ResizePreEquipItemID()
{
	m_mapPreEquipItemID[m_uiSelectPreEquipSet].resize(0);
}
void CX2CashShop::BuyAllEquippingItems()
{
	if( false == m_mapPreEquipItemID[m_uiSelectPreEquipSet].empty() )
	{
#ifdef ADDED_RELATIONSHIP_SYSTEM
		if( false == CheckBuyPossibleRelationItem( m_mapPreEquipItemID[m_uiSelectPreEquipSet] ) )
			return;
#endif // ADDED_RELATIONSHIP_SYSTEM
		m_pBuyItemPopUp->Reset( m_mapPreEquipItemID[m_uiSelectPreEquipSet] );
		m_pBuyItemPopUp->SetOpen( true );
		m_pBuyItemPopUp->SetChoicedItemBuy( false );
#ifdef CASH_ITEM_REFUND
		SetStringBuyItemRefund();
#endif CASH_ITEM_REFUND
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_9 ), g_pMain->GetNowState() );
	}
}
void CX2CashShop::InitPreEquipItemID()
{
	if( NULL != m_pDLGFront )
	{
		CKTDGUIRadioButton* pRadioButton = static_cast<CKTDGUIRadioButton*>(m_pDLGFront->GetControl(L"PreEquip_Set1"));
		if( NULL != pRadioButton )
		{
			pRadioButton->SetChecked(true);
		}
	}
	m_uiSelectPreEquipSet = 0;
	m_mapPreEquipItemID.clear();	

	vector<int> vecTemp;
	for( int i = 0; i < m_iMaxPreEquipSetNum; ++i )
	{
		m_mapPreEquipItemID[i] = vecTemp;
	}
}
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST


//=======================================================================================//
//--------------------------------- ES_GLOBAL_CODE_AREA ---------------------------------//
//=======================================================================================//


#ifdef SERV_USE_ENABLE_GIFT
void CX2CashShop::CashShopSlot::SetShowPresentButton( bool bShow )
{
	switch(m_SlotType)
	{
	case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
	case CX2Slot::ST_CASH_SHOP_MAIN_HOT:
	case CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND:
		{
			CKTDGUIButton* pPresentSmallButton = reinterpret_cast< CKTDGUIButton* >( m_pDLGCashShopSlot->GetControl( L"MainItemSlot_Present" ) );

			if(pPresentSmallButton != NULL)
			{
#ifdef NO_PRESENT_BUTTON
				pPresentSmallButton->SetShowEnable( false, false );
#else // NO_PRESENT_BUTTON
				pPresentSmallButton->SetShowEnable( true, bShow );
#endif // NO_PRESENT_BUTTON
			}
		}
		break;
	case CX2Slot::ST_CASH_SHOP_SUB:
	case CX2Slot::ST_CASH_SHOP_SUB_SPECIAL:
		{
			CKTDGUIButton* pPresentButton = reinterpret_cast< CKTDGUIButton* >( m_pDLGCashShopSlot->GetControl( L"Present" ) );

			if(pPresentButton != NULL)
			{
#ifdef NO_PRESENT_BUTTON
				pPresentButton->SetShowEnable( false, false );
#else // NO_PRESENT_BUTTON
				pPresentButton->SetShowEnable( true, bShow );
#endif // NO_PRESENT_BUTTON
			}
		}
		break;
	default:
		{
			ASSERT( !"Unexpected CashShop Slot Type" );
		}
		break;
	}
}

bool CX2CashShop::CashShopSlotManager::IsPossiblePresent( int itemID )
{
#ifdef SERV_CASHITEM_PURCHASE_RESTRICTION
	if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level <= 20)
	{
		return false;
	}
#endif SERV_CASHITEM_PURCHASE_RESTRICTION

	bool bEnableGift = false;
	CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );

	if(pCashItem == NULL || pCashItem->m_vecKBillProductInfo.size() < 1)
	{
		return false;
	}

	for(std::vector<KBillProductInfo>::iterator vit = pCashItem->m_vecKBillProductInfo.begin(); vit != pCashItem->m_vecKBillProductInfo.end(); ++vit)
	{
		bEnableGift = (bEnableGift || vit->m_bEnableGift);
	}

	return bEnableGift;
}

void CX2CashShop::CashShopSlotManager::SetShowPresentButton()
{
	CX2CashShop::CashShopSlot* pCashShopSlot = NULL;

	for ( UINT i = 0; i < m_vecCashShopSlot.size(); i++ )
	{
		pCashShopSlot = m_vecCashShopSlot[i];

		if ( pCashShopSlot == NULL )
		{
			continue;
		}

		int nItemID = pCashShopSlot->GetItemID();

#ifdef NO_PRESENT_BUTTON
		pCashShopSlot->SetShowPresentButton( false );
#else
		if(IsPossiblePresent(nItemID) == false)
		{
			pCashShopSlot->SetShowPresentButton( false );
		}
		else
		{
			pCashShopSlot->SetShowPresentButton( true );
		}
#endif // NO_PRESENT_BUTTON

		pCashShopSlot = NULL;
	}
}
#endif SERV_USE_ENABLE_GIFT

#ifdef SERV_WISH_LIST_NO_ITEM
void CX2CashShop::CashShopSlot::SetShowCartButton( bool bShow )
{
	switch(m_SlotType)
	{
	case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
	case CX2Slot::ST_CASH_SHOP_MAIN_HOT:
	case CX2Slot::ST_CASH_SHOP_MAIN_RECOMMEND:
		{
			if(m_pDLGCashShopSlot != NULL)
			{
				CKTDGUIButton* pCartSmallButton = reinterpret_cast< CKTDGUIButton* >( m_pDLGCashShopSlot->GetControl( L"MainItemSlot_Check" ) );

				if(pCartSmallButton != NULL)
				{
					pCartSmallButton->SetShowEnable( true, bShow );
				}
			}
		}
		break;
	case CX2Slot::ST_CASH_SHOP_SUB:
	case CX2Slot::ST_CASH_SHOP_SUB_SPECIAL:
		{
			if(m_pDLGCashShopSlot != NULL)
			{
				CKTDGUIButton* pCartButton = reinterpret_cast< CKTDGUIButton* >( m_pDLGCashShopSlot->GetControl( L"Cart" ) );

				if(pCartButton != NULL)
				{
					pCartButton->SetShowEnable( true, bShow );
				}
			}
		}
		break;
	default:
		{
			ASSERT( !"Unexpected CashShop Slot Type" );
		}
		break;

	}
}

bool CX2CashShop::CashShopSlotManager::IsNotPossibleWishList( int itemID )
{

	bool bNotEnableCart = false;
	CX2ItemManager::CashItem* pCashItem = g_pData->GetItemManager()->GetCashItem( itemID );

	if(pCashItem == NULL )
	{
		return false;
	}

	bNotEnableCart = pCashItem->GetIsWishListNotEnable();

	return bNotEnableCart;
}

void CX2CashShop::CashShopSlotManager::SetShowCartButton()
{
	CX2CashShop::CashShopSlot* pCashShopSlot = NULL;

	for ( UINT i = 0; i < m_vecCashShopSlot.size(); i++ )
	{
		pCashShopSlot = m_vecCashShopSlot[i];

		if ( pCashShopSlot == NULL )
		{
			continue;
		}

		int nItemID = pCashShopSlot->GetItemID();


		if(IsNotPossibleWishList(nItemID) == false)
		{
			pCashShopSlot->SetShowCartButton( true );
		}
		else
		{
			pCashShopSlot->SetShowCartButton( false );
		}


		pCashShopSlot = NULL;
	}
}
#endif SERV_WISH_LIST_NO_ITEM

//{{ 2013.03.13 조효진	캐시충전 버튼을 누르면 웹페이지를 띄워줌
#ifdef CASH_CHARGE_FROM_WEB

std::wstring CX2CashShop::GetWstrConnectChargePage()
{
	std::wstring wstrURL = L"";

#if defined (_SERVICE_) && !defined (_OPEN_TEST_)
	switch(g_pMain->GetNationFlag())
	{
	// 유럽
	case CX2Main::NF_DE:	wstrURL = L"http://de.elsword.gameforge.com/purchase/ingame/";	break;
	case CX2Main::NF_FR:	wstrURL = L"http://fr.elsword.gameforge.com/purchase/ingame/";	break;
	case CX2Main::NF_IT:	wstrURL = L"http://it.elsword.gameforge.com/purchase/ingame/";	break;
	case CX2Main::NF_PL:	wstrURL = L"http://pl.elsword.gameforge.com/purchase/ingame/";	break;
	case CX2Main::NF_ES:	wstrURL = L"http://es.elsword.gameforge.com/purchase/ingame/";	break;
	case CX2Main::NF_EN:	wstrURL = L"http://en.elsword.gameforge.com/purchase/ingame/";	break;
	// 미국
	case CX2Main::NF_US:
		{
#ifdef SERV_STEAM
			if( CX2Steam::IsSteamUser() == true )
			{
				wstrURL = L"https://billing.elswordonline.com/Fillup/Steam/InitTxn.aspx?userno=";
				wstrURL += g_pInstanceData->GetUserID();
			}
			else
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
			if( true == g_pInstanceData->IsAeriaChanneling() )
			{
				wstrURL = L"https://api.elswordonline.com/aeria/auth/billing?SID=";
				wstrURL += g_pInstanceData->GetUserSessionID();
			}
			else
#endif //SERV_CHANNELING_AERIA
#ifdef CASH_CHARGE_IN_GAME_BROWSER
			{
				wstrURL = L"https://billing.elswordonline.com/InGame/Main.aspx?userno=";
				wstrURL += g_pInstanceData->GetUserID();
				wstrURL += L"&gamecode=ES&region=";
				if( g_pInstanceData->GetServerGroupID() == 0 ) // 북미
					wstrURL += L"NA&hkey=";
				else// 남미
					wstrURL += L"LA&hkey=";
				// HKEY
				std::string strHkey;
				strHkey = KncUtil::toNarrowString(g_pInstanceData->GetUserID());// + ID
				strHkey += "ES";// + GameCode
				if( g_pInstanceData->GetServerGroupID() == 0 ) // 북미
					strHkey += "NA";// + region
				else
					strHkey += "LA";// + region
				strHkey += "EA1628437390C1507E315FF9F2AE0909";
				std::string strHkeyMD5 = KncUtil::GetMD5(strHkey);
				// 최종 URL
				wstrURL += KncUtil::toWideString(strHkeyMD5);
			}
#else //CASH_CHARGE_IN_GAME_BROWSER
				wstrURL = L"http://myaccount.elswordonline.com/";
#endif //CASH_CHARGE_IN_GAME_BROWSER
		}
		break;
	// 대만
	case CX2Main::NF_TW:	wstrURL = L"https://tw.event.beanfun.com/ELSWORD/BuyPoint/intro_main.aspx";	break;
	// 홍콩
	case CX2Main::NF_HK:	wstrURL = L"http://hk.beanfun.com/Default.aspx";	break;	
	// 브라질
	case CX2Main::NF_BR:	wstrURL = L"https://loja.levelupgames.com.br/Multivendas/frmVitrine.aspx?cdProvider=4&cdGame=52";	break;
	//동남아시아
	case CX2Main::NF_PH:	wstrURL = L"http://shop.elsword.garena.ph/";	break;

	// 태국
	case CX2Main::NF_TH:	wstrURL = L"https://secure3.playpark.com/refill/refillplaypark/";	break;
	// 일본
#ifdef CASH_CHARGE_URL_JP
	case CX2Main::NF_JP:
		{
			switch( g_pData->GetMyUser()->GetUserData().m_uChannelCode )
			{
			case KNexonAccountInfo::JCE_NHN:	wstrURL = g_pMain->GetNHNCashChargeURL();	break;
			case KNexonAccountInfo::JCE_MSN:	wstrURL = g_pMain->GetMSCashChargeURL();	break;
			case KNexonAccountInfo::JCE_NICO:	wstrURL = g_pMain->GetNICOCashChargeURL();	break;
			case KNexonAccountInfo::JCE_NEXON:	wstrURL = g_pMain->GetNEXONCashChargeURL(); break;
			default:	wstrURL = L"http://elsword.com/";	break;
			}
			break;
		}
#endif //CASH_CHARGE_URL_JP

	default:
		wstrURL = L"http://elsword.com/";
		break;
	}
#else	//--- 테섭 ---//
	switch(g_pMain->GetNationFlag())
	{
	// 유럽
	case CX2Main::NF_DE:	wstrURL = L"http://dev.elsword.de/de/purchase/ingame/";	break;
	case CX2Main::NF_FR:	wstrURL = L"http://dev.elsword.de/fr/purchase/ingame/";	break;
	case CX2Main::NF_IT:	wstrURL = L"http://dev.elsword.de/it/purchase/ingame/";	break;
	case CX2Main::NF_PL:	wstrURL = L"http://dev.elsword.de/pl/purchase/ingame/";	break;
	case CX2Main::NF_ES:	wstrURL = L"http://dev.elsword.de/es/purchase/ingame/";	break;
	case CX2Main::NF_EN:	wstrURL = L"http://dev.elsword.co.uk/purchase/ingame/";	break;
	// 미국
	case CX2Main::NF_US:
		{
#ifdef SERV_STEAM
			if( CX2Steam::IsSteamUser() == true )
			{
				wstrURL = L"http://billingtest.elswordonline.com:84/Fillup/Steam/InitTxn.aspx?userno=";
				wstrURL += g_pInstanceData->GetUserID();
			}
			else
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
			if( true == g_pInstanceData->IsAeriaChanneling() )
			{
				wstrURL = L"http://apitest.elswordonline.com/aeria/auth/billing?SID=";
				wstrURL += g_pInstanceData->GetUserSessionID();
			}
			else
#endif //SERV_CHANNELING_AERIA
#ifdef CASH_CHARGE_IN_GAME_BROWSER
			{
				wstrURL = L"http://billingtest.elswordonline.com:84/InGame/Main.aspx?userno=";
				wstrURL += g_pInstanceData->GetUserID();
				wstrURL += L"&gamecode=ES&region=";
				if( g_pInstanceData->GetServerGroupID() == 0 ) // 북미
					wstrURL += L"NA&hkey=";
				else// 남미
					wstrURL += L"LA&hkey=";
				// HKEY
				std::string strHkey;
				strHkey = KncUtil::toNarrowString(g_pInstanceData->GetUserID());// + ID
				strHkey += "ES";// + GameCode
				if( g_pInstanceData->GetServerGroupID() == 0 ) // 북미
					strHkey += "NA";// + region
				else
					strHkey += "LA";// + region
				strHkey += "EA1628437390C1507E315FF9F2AE0909";
				std::string strHkeyMD5 = KncUtil::GetMD5(strHkey);
				// 최종 URL
				wstrURL += KncUtil::toWideString(strHkeyMD5);
			}
#else //CASH_CHARGE_IN_GAME_BROWSER
				wstrURL = L"http://myaccounttest.elswordonline.com/";
#endif //CASH_CHARGE_IN_GAME_BROWSER
		}
		break;
	// 대만
	case CX2Main::NF_TW:	wstrURL = L"https://ssl.gameastor.com.tw/services/els/intro_main.aspx";	break;
	// 홍콩
	case CX2Main::NF_HK:	wstrURL = L"http://hk.beanfun.com/Default.aspx";	break;	
	// 브라질
	case CX2Main::NF_BR:	wstrURL = L"https://loja.levelupgames.com.br/Multivendas/frmVitrine.aspx?cdProvider=4&cdGame=52";	break;
	//동남아시아
	case CX2Main::NF_PH:	wstrURL = L"http://shoptest.elsword.garena.ph";	break;

		// 태국
	case CX2Main::NF_TH:	wstrURL = L"https://secure3.playpark.com/refill/refillplaypark/";	break;
	// 일본
#ifdef CASH_CHARGE_URL_JP
	case CX2Main::NF_JP:
		{
			switch( g_pData->GetMyUser()->GetUserData().m_uChannelCode )
			{
			case KNexonAccountInfo::JCE_NHN:	wstrURL = g_pMain->GetNHNCashChargeURL();	break;
			case KNexonAccountInfo::JCE_MSN:	wstrURL = g_pMain->GetMSCashChargeURL();	break;
			case KNexonAccountInfo::JCE_NICO:	wstrURL = g_pMain->GetNICOCashChargeURL();	break;
			case KNexonAccountInfo::JCE_NEXON:	wstrURL = g_pMain->GetNEXONCashChargeURL(); break;
			default:	wstrURL = L"http://elsword.com/";	break;
			}
			break;
		}
#endif //CASH_CHARGE_URL_JP
	default:
		wstrURL = L"http://elsword.com/";
		break;
	}
#endif

	// UserID 와 PurchaseToken 붙일지 결정
	switch(g_pMain->GetNationFlag())
	{
	case CX2Main::NF_DE:
	case CX2Main::NF_FR:
	case CX2Main::NF_IT:
	case CX2Main::NF_PL:
	case CX2Main::NF_ES:
	case CX2Main::NF_EN:
		wstrURL += g_pData->GetMyUser()->GetUserData().userID;
		wstrURL += L"/";
#ifdef SERV_PURCHASE_TOKEN
		wstrURL += g_pInstanceData->GetPurchaseToken();
#endif SERV_PURCHASE_TOKEN

		break;
	default:
		break;
	}

	return wstrURL;
}

#endif CASH_CHARGE_FROM_WEB
//}}

#ifdef SERV_SHARING_BANK_QUEST_CASH
bool CX2CashShop::GetCahBuySharingBankItem( int itemID )
{
	if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( SHARING_BANK_OPEN_CASH_ITEM_ID, true ) != NULL )
	{
		return false;
	}
	// 은행 공유 중인지 체크하자.
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&			
		g_pData->GetMyUser()->IsSharingBankOpen() == true )
	{
		return false;
	}

	return true;
}
#endif // SERV_SHARING_BANK_QUEST_CASH

#ifdef LINK_CASH_SHOP_WHEN_JOB_CHANGE
void CX2CashShop::JobChangeDirectLink( int iEventID )
{
	// 소비탭으로 변경
	SetCashShopState( CX2CashShop::CSS_SUB, CX2CashShop::CSC_CONSUME );
	CKTDGUIRadioButton* pRadioButton = NULL;
#ifdef UIRRL_CASH_SHOP
	if( false == m_bCreateDialog )
	{
		break;
	}
#endif //UIRRL_CASH_SHOP
	pRadioButton = static_cast< CKTDGUIRadioButton* >( m_pDLGFront->GetControl( L"tab_Use" ) );

	if ( pRadioButton != NULL )
	{
		pRadioButton->SetChecked( true );
	}

	int itemID = -1;
	switch( iEventID )
	{
	case CX2LVUpEventMgr::LUEMCUM_ELSWORD_SWORD_KNIGHT:
		itemID = CLASS_CHANGE_ELSWORD_KNIGHT_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_ELSWORD_MAGIC_KNIGHT:
		itemID = CLASS_CHANGE_MAGIC_KNIGHT_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_AISHA_HIGH_MAGICIAN:
		itemID = CLASS_CHANGE_HIGH_MAGICIAN_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_AISHA_DARK_MAGICIAN:
		itemID = CLASS_CHANGE_DARK_MAGICIAN_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_RENA_SNIPING_RANGER:
		itemID = CLASS_CHANGE_SNIPING_RANGER_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_RENA_COMBAT_RANGER:
		itemID = CLASS_CHANGE_COMBAT_RANGER_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_RAVEN_SWORD_TAKER:
		itemID = CLASS_CHANGE_SOUL_TAKER_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_RAVEN_OVER_TAKER:
		itemID = CLASS_CHANGE_OVER_TAKER_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_EVE_CODE_EXOTIC:
		itemID = CLASS_CHANGE_EXOTIC_GEAR_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_EVE_CODE_ARCHITECTURE:
		itemID = CLASS_CHANGE_ARCHITECTURE_ITEM_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_CHUNG_FURY_GUARDIAN:
		itemID = CLASS_CHANGE_FURY_GUARDIAN_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_CHUNG_SHOOTING_GUARDIAN:
		itemID = CLASS_CHANGE_SHOOTER_GUARDIAN_ID;
		break;
		//	1차 추가 전직
	case CX2LVUpEventMgr::LUEMCUM_ELSWORD_SHEATH_KNIGHT:
		itemID = CLASS_CHANGE_SHEATH_KNIGHT_ITEM_ID;
		break;		
	case CX2LVUpEventMgr::LUEMCUM_ARME_BATTLE_MAGICIAN:
		itemID = CLASS_CHANGE_BATTLE_MAGICIAN_ITEM_ID;
		break;		
	case CX2LVUpEventMgr::LUEMCUM_LIRE_TRAPPING_RANGER:
		itemID = CLASS_CHANGE_TRAPPING_RANGER_ITEM_ID;
		break;		
	case CX2LVUpEventMgr::LUEMCUM_RAVEN_WEAPON_TAKER:
		itemID = CLASS_CHANGE_WEAPON_TAKER_ITEM_ID;
		break;		
	case CX2LVUpEventMgr::LUEMCUM_EVE_ELECTRA:
		itemID = CLASS_CHANGE_ELECTRA_ITEM_ID;
		break;		
	case CX2LVUpEventMgr::LUEMCUM_CHUNG_SHELLING_GUARDIAN:
		itemID = CLASS_CHANGE_SHELLING_GUARDIAN_ID;
		break;		
	case CX2LVUpEventMgr::LUEMCUM_ARA_LITTLE_HSIEN:
		itemID = CLASS_CHANGE_LITTLE_HSIEN_ID;
		break;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	case CX2LVUpEventMgr::LUEMCUM_ARA_LITTLE_DEVIL:
		itemID = CLASS_CHANGE_LITTLE_DEVIL_ID;
		break;
#endif SERV_ARA_CHANGE_CLASS_SECOND
#ifdef NEW_CHARACTER_EL
	case CX2LVUpEventMgr::LUEMCUM_ELESIS_SABER_KNIGHT:
		itemID = CLASS_CHANGE_SABER_KNIGHT_ID;
		break;
	case CX2LVUpEventMgr::LUEMCUM_ELESIS_PYRO_KNIGHT:
		itemID = CLASS_CHANGE_PYRO_KNIGHT_ID;
		break;
#endif NEW_CHARACTER_EL
	default:
		return;
	}

	vector< int > vecItemID;
	if ( itemID != 0 )
		vecItemID.push_back( itemID );
	m_pBuyItemPopUp->Reset( vecItemID );
	m_pBuyItemPopUp->SetOpen( true );
	m_pBuyItemPopUp->SetChoicedItemBuy( false );

#ifdef CASH_ITEM_REFUND
	SetStringBuyItemRefund();
#endif CASH_ITEM_REFUND

}
#endif LINK_CASH_SHOP_WHEN_JOB_CHANGE

#ifdef SERV_NOTIFY_BONUS_CASH
bool CX2CashShop::Handler_EGS_BONUS_CASH_EVENT_NOT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // 2011.10.05 lygan_조성욱 // 보너스 캐쉬 이벤트 알림용
{
	SAFE_DELETE_DIALOG(m_pDLGMsgBox);

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BONUS_CASH_EVENT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	


	m_bCheckCashEvent = kEvent.m_bEventStart;
	return true;
}
#endif //SERV_NOTIFY_BONUS_CASH

#ifdef CJ_ID_WEB_BILLING
bool CX2CashShop::InitCJBillingWebBrowser()
{
	// 영역 설정
	RECT rcClient;
	GetWindowRect( g_pKTDXApp->GetHWND(), &rcClient );

	const int iWidth = 535;
	const int iHeight = 602;

	RECT rcWebPage;
	rcWebPage.left		= rcClient.left + ( ( rcClient.right - rcClient.left ) - iWidth ) / 2;
	rcWebPage.top		= rcClient.top + ( ( rcClient.bottom - rcClient.top ) - iHeight ) / 2;
	rcWebPage.right		= rcClient.left + ( ( rcClient.right - rcClient.left ) - iWidth ) / 2 + iWidth;
	rcWebPage.bottom	= rcClient.top + ( ( rcClient.bottom - rcClient.top ) - iHeight ) / 2 + iHeight;

	// 열기
	g_pMain->GetNMBrowserWrapper()->Create( rcWebPage, g_pKTDXApp->GetHWND() );

	// 주소 설정
	std::string strURL;
#ifdef _SERVICE_

#ifdef _OPEN_TEST_
	strURL = "http://testbill.netmarble.co.id/BL_InGame/ePin_Request.aspx?p=G0001";
#else _OPEN_TEST_
	strURL = "https://bill.netmarble.co.id/BL_InGame/Main.aspx?p=G0001";
#endif _OPEN_TEST_

#else _SERVICE_
	strURL = "http://testbill.netmarble.co.id/BL_InGame/ePin_Request.aspx?p=G0001";
#endif _SERVICE_

	g_pMain->GetNMBrowserWrapper()->Navigate( strURL.c_str() ); 

	return true;
}
#endif //CJ_ID_WEB_BILLING

#ifdef SERV_COUNTRY_PH
bool CX2CashShop::Handler_EGS_EXCHANGE_CASH_CLIENT_NOT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXCHANGE_CASH_CLIENT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	
	
	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
#ifdef SERV_NOTIFY_BONUS_CASH
		unsigned long ulAddBonus = kEvent.m_AddBonusCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT];

		if( ulAddBonus > 0 )	// 이벤트 중인지 아닌지를 체크 && 보너스 포인트 지급되는지 체크
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 279, 300 ), GET_REPLACED_STRING( ( STR_ID_14478, "ii", kEvent.m_iPoint, ulAddBonus ) ), (CKTDXStage*) m_pNowState );
		}
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 279, 300 ), GET_REPLACED_STRING( ( STR_ID_11342, "i", kEvent.m_iPoint ) ), (CKTDXStage*) m_pNowState );
		}
#endif SERV_NOTIFY_BONUS_CASH

		CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
		pNowState->Handler_EGS_CHECK_BALANCE_REQ();
		return true;
	}
	
	return false;
}

bool CX2CashShop::PresentItemPopup::RestPresentItemCashCheck( int iProductNo ) // 2013.09.17 lygan_조성욱 // 선물하기 전에도 잔액 체크를 해서 잔액이 모자라면 서버로 REQ 보내지 않게 하기
{
	KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( iProductNo );
	if ( pKBillProductInfo != NULL )
	{
		int remainCash = 0; 

		switch( GetUseCashType() )
		{
		case KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH:
			remainCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH) - pKBillProductInfo->m_iSalePrice;
			break;
		case KGlobalCashInfo::GCT_PUBLISHER_CASH:
			remainCash = g_pData->GetMyUser()->GetGlobalCash(KGlobalCashInfo::GCT_PUBLISHER_CASH) - pKBillProductInfo->m_iSalePrice;
			break;
		case KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT:
			remainCash = g_pData->GetMyUser()->GetGlobalCash( KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT ) - pKBillProductInfo->m_iSalePrice;
			break;
		default:
			remainCash = g_pData->GetMyUser()->GetGlobalMainCash() - pKBillProductInfo->m_iSalePrice;
			break;
		}

	if ( remainCash >= 0 )
		return true;
	}

	return false;		
}
#endif //SERV_COUNTRY_PH

//=======================================================================================//
//--------------------------------- ES_GLOBAL_CODE_AREA ---------------------------------//
//=======================================================================================//

#ifdef ADDED_RELATIONSHIP_SYSTEM
/** @function : CheckBuyPossibleRelationItem
	@brief : 해당 아이템이 결혼 아이템이라면 커플 유저만 구입 할 수 있도록 체크
	@return : 구입 불가능할 경우 false 반환 
*/
bool CX2CashShop::CheckBuyPossibleRelationItem( int iItemID )
{
#ifdef FIXED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipManager() &&
		 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
	{
		SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;
		if( eRelationType == SEnum::RT_SOLO ) // 솔로 일때만 체크한다.
		{
			for( int i = 0 ; i < ARRAY_SIZE(ITEM_ID_WEDDING_CASH_ITEM); ++i )
			{
				// 해당 아이템이 Wedding Cash Item 이라면
				// false를 반환하고 MessageBox 를 띄운다.
				if( iItemID == ITEM_ID_WEDDING_CASH_ITEM[i] )
				{
					g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24674 ),g_pMain->GetNowState() );			
					return false;
				}
			}
		}
	}
	return true;



#else // FIXED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipManager() &&
		 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
	{
		SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;
		if( eRelationType != SEnum::RT_SOLO )
			return true;
	}

	bool bWeddingItem = false;
	for( int i = 0 ; i < ARRAY_SIZE(ITEM_ID_WEDDING_CASH_ITEM); ++i )
	{
		if( iItemID == ITEM_ID_WEDDING_CASH_ITEM[i] )
		{
			bWeddingItem = true;
			break;
		}
	}

	if( true == bWeddingItem )
	{
		g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24674 ),g_pMain->GetNowState() );
		return false;
	}

	return true;

#endif // FIXED_RELATIONSHIP_SYSTEM
}
/** @function : CheckBuyPossibleRelationItem
	@brief : 해당 아이템이 결혼 아이템이라면 커플 유저만 구입 할 수 있도록 체크
	@return : 구입 불가능할 경우 false 반환 
*/
bool CX2CashShop::CheckBuyPossibleRelationItem( const vector<int>& vecItemIDList )
{
	if ( NULL != g_pData->GetRelationshipManager() &&
		NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
	{
		SEnum::RELATIONSHIP_TYPE eRelationType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;
		if( eRelationType != SEnum::RT_SOLO )
			return true;
	}

	BOOST_FOREACH( int iItemID, vecItemIDList )
	{
		if( false == CheckBuyPossibleRelationItem( iItemID ) )
			return false;
	}

	return true;
}
#endif //ADDED_RELATIONSHIP_SYSTEM


bool CX2CashShop::ShowPopUpGuideByItemID( const int iItemID_ )
{
	if( GetClassChangeItemLevel( iItemID_ ) > 0 ) // 전직 캐시 아이템이면
	{
		std::wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_4518, "L", GetClassNameByClassChangeItem( iItemID_ ) ) );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), g_pMain->GetNowState() );

		return true;
	}
	else if( true == g_pData->GetSkillTree()->IsUnsealSkillItemID( iItemID_ ) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4519 ), g_pMain->GetNowState() );

		return true;
	}
#ifdef UNIT_CLASS_CHANGE_ITEM
	else if ( GetUnitClassChangeItemLevel( iItemID_ ) > 0 )
	{
		std::wstring wstrMsg;

		wstrMsg = GET_REPLACED_STRING( ( STR_ID_15234, "L", GetClassNameByUnitClassChangeItem( iItemID_ ) ) );

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), g_pMain->GetNowState() );

		return true;
	}
#endif

	return false;
}

#ifdef COUPON_SYSTEM
/** @function : Handler_EGS_DISCOUNT_COUPON_LIST_INQUIRY_REQ
	@brief : 상품 번호 별 사용 가능한 할인 쿠폰 목록 요청
*/
void CX2CashShop::Handler_EGS_DISCOUNT_COUPON_LIST_INQUIRY_REQ( const unsigned long ulProductNo_ )
{
	if( NULL != m_pBuyItemPopUp )
	{
		if( 0 == ulProductNo_ )
		{
			m_pBuyItemPopUp->UpdateCouponList();
			return;
		}
		else
		{
			// 사용 가능한 할인 쿠폰 목록을 이미 가지고 있다면 REQ를 다시 보내지 않아도 됨.
			// (구매 창 닫으면 초기화 되기 때문에 갱신 염려는 없음)
			if( true == m_pBuyItemPopUp->IsHavePrdouctCouponInfo(ulProductNo_) )
			{
				m_pBuyItemPopUp->UpdateCouponList();
				return;
			}
		}
	}

	// 패킷이 Vector로 정의되어 있지만 한번에 1개씩만 보내고 있음.
	// 넥슨 양식에 맞추기 위해 Vector로 처리 했다고 함.
 	KEGS_DISCOUNT_COUPON_LIST_INQUIRY_REQ kPacket;

	// 현재는 m_usOrderQuantity 값은 무조건 1로 설정. 
	// 불필요한 값 같은데 일단 캐시 관련 코드는 넥슨에서 요청한 양식 그대로 처리하고 있어서
	// 1로 주고 있음.
	KDiscountCouponInquriyInfo kDiscountCouponInquriyInfo;
	kDiscountCouponInquriyInfo.m_ulProductNo = ulProductNo_;
	kDiscountCouponInquriyInfo.m_usOrderQuantity = 1;

	kPacket.m_vecDiscountCouponInquiryInfo.push_back( kDiscountCouponInquriyInfo );

	m_ulTempSelectedProductNo = ulProductNo_;
	g_pData->GetServerProtocol()->SendPacket( EGS_DISCOUNT_COUPON_LIST_INQUIRY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK );
}
/** @function : Handler_EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK
	@brief : 상품 번호 별 사용 가능한 할인 쿠폰 목록 설정
*/
bool CX2CashShop::Handler_EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if ( g_pMain->DeleteServerPacket( EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{											 
			m_pBuyItemPopUp->InitProductAndCouponNo( m_ulTempSelectedProductNo );

			if( NULL != m_pBuyItemPopUp )
			{
				BOOST_FOREACH( KDiscountCoupon kDiscountCoupon, kEvent.m_vecDiscountCoupon )
				{
					// 쿠폰 번호 별 상품 이름 등록
					m_pBuyItemPopUp->InsertCouponNoAndName( m_ulTempSelectedProductNo, kDiscountCoupon.m_iCouponCardNo, kDiscountCoupon.m_wstrContractDetailName );
					
					// 쿠폰 번호 별 할인 정보 등록
					{

						wstring::size_type sizeType = 0;
						sizeType = kDiscountCoupon.m_wstrDiscountValue.find( L"%" );
						if( 0 != sizeType && sizeType < kDiscountCoupon.m_wstrDiscountValue.size() )
						{
							kDiscountCoupon.m_wstrDiscountValue.erase( sizeType );

							m_pBuyItemPopUp->InsertCouponNoAndDiscountInfo(kDiscountCoupon.m_iCouponCardNo, 
								CouponDiscountInfo( true, _wtoi( kDiscountCoupon.m_wstrDiscountValue.c_str() )));
						}
						else
						{
							sizeType = kDiscountCoupon.m_wstrDiscountValue.find( GET_STRING(STR_ID_4634) );
							if( 0 != sizeType && sizeType < kDiscountCoupon.m_wstrDiscountValue.size() )
							{
								kDiscountCoupon.m_wstrDiscountValue.erase( sizeType );

								m_pBuyItemPopUp->InsertCouponNoAndDiscountInfo(kDiscountCoupon.m_iCouponCardNo, 
									CouponDiscountInfo( false, _wtoi( kDiscountCoupon.m_wstrDiscountValue.c_str() )));
							}
						}
					}
				}
				m_pBuyItemPopUp->UpdateCouponList();
			}
		}
		m_ulTempSelectedProductNo = 0;
	}

	return true;

}
#endif // COUPON_SYSTEM

#ifdef ADJUST_UNIT_CLASS_CHANGE_ITEM_REPETITION
// 해당 유닛의 전직 아이템을 찜하기 목록에서 삭제함.
void CX2CashShop::RemoveUnitClassChangeItemInChoice( const CX2Unit::UNIT_CLASS eUnitClass_ )
{
	const int ItemID = GetUnitClassChangeItemID( eUnitClass_ );

	if( 0 < ItemID )
	{
		if ( std::find( m_vecChoicedItemID.begin(), m_vecChoicedItemID.end(), ItemID ) != m_vecChoicedItemID.end() )
		{
			RemoveChoicedItem( ItemID );
		}
	}
}
// 전직에 해당하는 전직 아이템 ID를 반환함.
const int CX2CashShop::GetUnitClassChangeItemID( const CX2Unit::UNIT_CLASS eUnitClass_ )
{
	int ItemId = -1;

	switch( eUnitClass_ )
	{
	case CX2Unit::UC_ELSWORD_KNIGHT:
		{
			ItemId = UNIT_CLASS_CHANGE_SWORD_KNIGHT_ITEM_ID;
		}
		break;

	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
		{
			ItemId = UNIT_CLASS_CHANGE_MAGIC_KNIGHT_ITEM_ID;
		}
		break;
	
#ifdef ELSWORD_SHEATH_KNIGHT
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
		{
			ItemId = UNIT_CLASS_CHANGE_SHEATH_KNIGHT_ITEM_ID;
		}
		break;
#endif //ELSWORD_SHEATH_KNIGHT

	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
		{
			ItemId = UNIT_CLASS_CHANGE_HIGH_MAGICIAN_ITEM_ID;
		}
		break;

	case CX2Unit::UC_ARME_DARK_MAGICIAN:
		{
			ItemId = UNIT_CLASS_CHANGE_DARK_MAGICIAN_ITEM_ID;
		}
		break;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
		{
			ItemId = UNIT_CLASS_CHANGE_BATTLE_MAGICIAN_ITEM_ID;
		}
		break;
#endif //SERV_ADD_ARME_BATTLE_MAGICIAN

	case CX2Unit::UC_LIRE_COMBAT_RANGER:
		{
			ItemId = UNIT_CLASS_CHANGE_COMBAT_RANGER_ITEM_ID;
		}
		break;
		
	case CX2Unit::UC_LIRE_SNIPING_RANGER:
		{
			ItemId = UNIT_CLASS_CHANGE_SNIPING_RANGER_ITEM_ID;
		}
		break;

	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
		{
			ItemId = UNIT_CLASS_CHANGE_TRAPPING_RANGER_ITEM_ID;
		}
		break;

	case CX2Unit::UC_RAVEN_SOUL_TAKER:
		{
			ItemId = UNIT_CLASS_CHANGE_SOUL_TAKER_ITEM_ID;
		}
		break;
		
	case CX2Unit::UC_RAVEN_OVER_TAKER:
		{
			ItemId = UNIT_CLASS_CHANGE_OVER_TAKER_ITEM_ID;
		}
		break;
		
#ifdef RAVEN_WEAPON_TAKER
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
		{
			ItemId = UNIT_CLASS_CHANGE_WEAPON_TAKER_ITEM_ID;
		}
		break;
#endif //RAVEN_WEAPON_TAKER

	case CX2Unit::UC_EVE_EXOTIC_GEAR:
		{
			ItemId = UNIT_CLASS_CHANGE_EXOTIC_GEAR_ITEM_ID;
		}
		break;
	
	case CX2Unit::UC_EVE_ARCHITECTURE:
		{
			ItemId = UNIT_CLASS_CHANGE_ARCHITECTURE_ITEM_ID;
		}
		break;
	
#ifdef EVE_ELECTRA
	case CX2Unit::UC_EVE_ELECTRA:
		{
			ItemId = UNIT_CLASS_CHANGE_ELECTRA_ITEM_ID;
		}
		break;
#endif //EVE_ELECTRA

	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
		{
			ItemId = UNIT_CLASS_CHANGE_FURY_GUARDIAN_ID;
		}
		break;
	
	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
		{
			ItemId = UNIT_CLASS_CHANGE_SHOOTING_GUARDIAN_ID;
		}
		break;

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
		{
			ItemId = UNIT_CLASS_CHANGE_SHELLING_GUARDIAN_ID;
		}
		break;
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
		{
			ItemId = UNIT_CLASS_CHANGE_SABER_KNIGHT_ID;
		}
		break;

	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
		{
			ItemId = UNIT_CLASS_CHANGE_PYRO_KNIGHT_ID;
		}
		break;

#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_LITTLE_HSIEN:
		{
			ItemId = UNIT_CLASS_CHANGE_LITTLE_HSIEN_ID;
		}
		break;
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CX2Unit::UC_ARA_LITTLE_DEVIL:
		{
			ItemId = UNIT_CLASS_CHANGE_LITTLE_DEVIL_ID;
		}
		break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
		{
			ItemId = UNIT_CLASS_CHANGE_LORD_KNIGHT_ITEM_ID;
		}
		break;

	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
		{
			ItemId = UNIT_CLASS_CHANGE_RUNE_SLAYER_ITEM_ID;
		}
		break;
			
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
		{
			ItemId = UNIT_CLASS_CHANGE_INFINITY_SWORD_ITEM_ID;
		}
		break;
#endif //SERV_ELSWORD_INFINITY_SWORD

	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{
			ItemId = UNIT_CLASS_CHANGE_VOID_PRINCESS_ITEM_ID;
		}
		break;

	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			ItemId = UNIT_CLASS_CHANGE_ELEMENTAL_MASTER_ITEM_ID;
		}
		break;

#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
		{
			ItemId = UNIT_CLASS_CHANGE_DIMENSION_WITCH_ITEM_ID;
		}
		break;
#endif //SERV_ARME_DIMENSION_WITCH

	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			ItemId = UNIT_CLASS_CHANGE_WIND_SNEAKER_ITEM_ID;
		}
		break;
			
	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{
			ItemId = UNIT_CLASS_CHANGE_GRAND_ARCHER_ITEM_ID;
		}
		break;
	
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
		{
			ItemId = UNIT_CLASS_CHANGE_NIGHT_WATCHER_ITEM_ID;
		}
		break;
#endif //SERV_RENA_NIGHT_WATCHER
			
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
		{
			ItemId = UNIT_CLASS_CHANGE_BLADE_MASTER_ITEM_ID;
		}
		break;
	
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
		{
			ItemId = UNIT_CLASS_CHANGE_RECKLESS_FIST_ITEM_ID;
		}
		break;
	
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
		{
			ItemId = UNIT_CLASS_CHANGE_VETERAN_COMMANDER_ITEM_ID;
		}
		break;
#endif //SERV_RAVEN_VETERAN_COMMANDER

	case CX2Unit::UC_EVE_CODE_NEMESIS:
		{
			ItemId = UNIT_CLASS_CHANGE_NEMESIS_ITEM_ID;
		}
		break;
	
	case CX2Unit::UC_EVE_CODE_EMPRESS:
		{
			ItemId = UNIT_CLASS_CHANGE_EMPRESS_ITEM_ID;
		}
		break;

#ifdef SERV_EVE_BATTLE_SERAPH
	case CX2Unit::UC_EVE_BATTLE_SERAPH:
		{
			ItemId = UNIT_CLASS_CHANGE_BATTLE_SERAPH_ITEM_ID;
		}
		break;
#endif //SERV_EVE_BATTLE_SERAPH

#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_IRON_PALADIN:
		{
			ItemId = UNIT_CLASS_CHANGE_IRON_PALADIN_ID;
		}
		break;
	
	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
		{
			ItemId = UNIT_CLASS_CHANGE_DEADLY_CHASER_ID;
		}
		break;
#endif //CHUNG_SECOND_CLASS_CHANGE

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
		{
			ItemId = UNIT_CLASS_CHANGE_TACTICAL_TROOPER_ID;
		}
		break;
#endif //SERV_CHUNG_TACTICAL_TROOPER

#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_SAKRA_DEVANAM:
		{
			ItemId = UNIT_CLASS_CHANGE_SAKRA_DEVANAM_ID;
		}
		break;
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CX2Unit::UC_ARA_YAMA_RAJA:
		{
			ItemId = UNIT_CLASS_CHANGE_YAMA_RAJA_ID;
		}
		break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	case CX2Unit::UC_ELESIS_GRAND_MASTER:
		{
			ItemId = UNIT_CLASS_CHANGE_GRAND_MASTER_ID;
		}
		break;
			
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
		{
			ItemId = UNIT_CLASS_CHANGE_BLAZING_HEART_ID;
		}
		break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CX2Unit::UC_ADD_LUNATIC_PSYKER:
		{
			ItemId = UNIT_CLASS_CHANGE_LUNATIC_PSYKER_ID;
		}
		break;
#endif //SERV_ADD_LUNATIC_PSYKER

	default:
		break;
	}

	return ItemId;
}
#endif //ADJUST_UNIT_CLASS_CHANGE_ITEM_REPETITION

#ifdef REFORM_SKILL_NOTE_UI
/** @function : OpenBuyPopupByReserve
	@brief : 아이템 구매 팝업 열기.
			 캐시샵 입장과 동시에 구매 팝업 열어주기 위해 사용
*/
void CX2CashShop::OpenBuyPopupByReserve()
{
	// # 현재 페이지를 타겟 아이템이 있는 페이지로 변경하기.
	if( NULL != m_pSubSM )
	{
		const int iBuyItemID = m_iItemIDShowBuyUIAfterEnter;
		vector<int> vecItemList = GetItemByCategory( m_CashShopCategory, m_CashShopSubCategory );
		{
			int iIndex = 0;
			bool bIsChangedPage = false;
			BOOST_FOREACH( const int iItemID, vecItemList )
			{
				if( iBuyItemID == iItemID )
				{
					const int iBuyItemPage = (iIndex / 10) + 1;

					// 직접적인 페이지 변경 구문
					m_pSubSM->ResetNowPage( iBuyItemPage );
					bIsChangedPage = true;
					break;
				}
				++iIndex;
			}

			// # 페이지를 찾지 못했다면 진행 중지
			if( false == bIsChangedPage )
				return;
		}

		// # 구매 팝업 생성
		vector< int > vecItemID;
		vecItemID.push_back( iBuyItemID );
		if( NULL != m_pBuyItemPopUp )
		{
			m_pBuyItemPopUp->Reset( vecItemID );
			m_pBuyItemPopUp->SetOpen( true );
			m_pBuyItemPopUp->SetChoicedItemBuy( false );
		}

		ResetItemSlot( CX2Slot::ST_CASH_SHOP_SUB, m_pSubSM->GetNowShowItemIDList() );
		ResetSubPageNumUI();
	}

	SetItemIDShowBuyUIAfterEnter(0);
}
#endif // REFORM_SKILL_NOTE_UI

#ifdef SERV_NAVER_CHANNELING
void CX2CashShop::SetOpenNaverCashChargePage( OUT string& strURL_, OUT RECT& rcWebPage_ )
{
	strURL_ = "https://cash.nexon.game.naver.com/Main.aspx?chid=ELSW&ukey=";

	
	char strUkey[MAX_PATH];
	ZeroMemory( strUkey, MAX_PATH );

	UINT uiNexonSN = g_pData->GetMyUser()->GetNexonSN();
	_itoa_s( uiNexonSN, strUkey, 10 );

	strURL_ += strUkey;

	string strCtype = "&CTYPE=29&HKEY=";
	strURL_ += strCtype;

	// 해시키 ///////////////////
	std::string strSource;
	std::string strTemp;	
	strSource = "ELSW";

	// oasis907 : 김상윤 [2011.1.10] // 서버 시간 가지고 오게 수정
	CTime ctCurrentTime( g_pData->GetServerCurrentTime() );

	ConvertWCHARToChar( strTemp, static_cast< const wchar_t * >( ctCurrentTime.Format( L"%Y%m%d%I" ) ) );
	strSource += strTemp;

	string strMD5 = KncUtil::GetMD5( strSource );


	strURL_ += strMD5;

	if ( NULL != g_pInstanceData )
	{
		strURL_ += "&token=";
		strURL_ += g_pInstanceData->GetNaverAccessToken();
	}
	////////////////////////////////

	rcWebPage_.left = 250;
	rcWebPage_.top	= 250;
	rcWebPage_.right	= 600;
	rcWebPage_.bottom	= 500;
}
#endif // SERV_NAVER_CHANNELING

#ifdef CASH_INVEN_PICKUP_ALL
bool CX2CashShop::PickUpAll()
{
	// 아이템을 하나 가져오면, 정렬을 하므로 항상 첫번째 아이템을 가져오도록 하자.
	CX2SlotItem* pItemSlot = NULL;
	pItemSlot = (CX2SlotItem*) GetSlot( CX2SlotManager::CX2Slot::ST_CASH_SHOP_DEPOSIT, 0 );
	if ( pItemSlot == NULL )
	{
		m_bPickUpAll = false;
		return false;
	}

	if ( pItemSlot->GetKBillOrderInfo().m_byteProductKind == 1 )
	{
		map< int, std::vector< int > > mapItemIDSocketOption;
		vector< KBillOrderInfo > vecKBillTOrderInfo;
		KBillPackageInfo kPackageInfo;
		vector< KBillPackageInfo > vecKBillPackageInfo;

		for ( int i = 0; i < (int)m_vecKBillPackageInfo.size(); i++ )
		{	
			KBillPackageInfo& kKBillPackageInfo = m_vecKBillPackageInfo[i];
			if ( /*kKBillPackageInfo.m_ulProductNo == pItemSlot->GetKBillOrderInfo().m_kBillProductInfo.m_iProductNo &&*/
				kKBillPackageInfo.m_iPackageNo == pItemSlot->GetKBillOrderInfo().m_iTransNo )
			{
				//int tempItemID = _wtoi( kKBillPackageInfo.m_wstrProductID.c_str() );
				//int tempItemID = kKBillPackageInfo.m_ulProductNo;
				int tempItemID = kKBillPackageInfo.m_iProductID;
				kPackageInfo = kKBillPackageInfo;
				vecKBillPackageInfo.push_back(kKBillPackageInfo);
				//if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( tempItemID ) == true )
				{
					if ( mapItemIDSocketOption.find(tempItemID) == mapItemIDSocketOption.end() )
						mapItemIDSocketOption.insert( std::pair<int, std::vector< int >>(tempItemID, kKBillPackageInfo.m_vecSocketOption));
				}
			}
		}
		vecKBillTOrderInfo.push_back( pItemSlot->GetKBillOrderInfo() );
		//m_pCashItemToInvenPopup->Reset( vecItemID, vecKBillTOrderInfo, m_vecKBillPackageInfo );
		//m_pCashItemToInvenPopup->SetOpen( true );

		if ( vecKBillTOrderInfo.empty() == false )
		{
			KBillOrderInfo& kKBillOrderInfo = vecKBillTOrderInfo[0];
			map< int, int > mapSelectedOptionList;
			map<int, int> mapOptionGroupIdList;		/// <SocketId, GroupdID>
			// 여기서 소켓 강제 셋팅 해주면 된다.
			
			for(map<int, std::vector< int >>::iterator mit = mapItemIDSocketOption.begin(); mit != mapItemIDSocketOption.end(); mit++)
			{
				if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( mit->first ) == true )
				{
					std::vector< int > vecSocket = mit->second;
					if( vecSocket[0] == 0 )
					{
						// 소켓 옵션그룹이 처음부터 0 이다?
						MessageBox(NULL, L"소켓 옵션 그룹 ID 가 0 입니다. 셋팅 체크 필요", L"Error", MB_OK);
					}
					else
					{
						const CX2Item::ItemTemplet* pItemTemplate = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
						// 첫번째 소켓 옵션을 강제로 셋팅을
						vector<int> vecSocketIdList;	/// iSocketGroupID 에 해당하는 SocketID 들을 얻어옴
						if ( 0 != g_pData->GetSocketItem()->GetSocketIdListForCashAvatar( vecSocket[0], vecSocketIdList, pItemTemplate->GetItemType() ) )
						{
							mapSelectedOptionList.insert( std::make_pair( pItemTemplate->GetItemID(), vecSocketIdList[0]));
							mapOptionGroupIdList.insert( std::make_pair( vecSocketIdList[0], m_TempCashItemToInven.m_vecSocketOption[0]));
						}
						else
						{
							// 그룹에 해당하는 소켓이 없다?
							MessageBox(NULL, L"소켓 옵션 그룹 에 해당하는 소켓 ID가 없습니다. 셋팅 체크 필요", L"Error", MB_OK);
						}
					}				
				}
			}			
			
			if ( vecKBillPackageInfo.empty() == false )
			{
				m_bPickUpAll = true;
				Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( kKBillOrderInfo, mapSelectedOptionList, mapOptionGroupIdList, vecKBillPackageInfo );
				return false;
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox(D3DXVECTOR2(250,300), GET_STRING( STR_ID_8 ), g_pMain->GetNowState() );
			}
		}
	}
	else
	{
		const CX2Item::ItemTemplet* pItemTemplate = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
		if ( pItemTemplate != NULL )
		{
	#ifdef SERV_PSHOP_AGENCY
			m_bUsePShopAgencyItem = false;
			if( pItemTemplate->GetItemID() == 233210 )
			{
				m_bUsePShopAgencyItem = true;
				//m_TempCashItemToInven = pItemSlot->GetKNXBTOrderInfo();
				m_TempCashItemToInven = pItemSlot->GetKBillOrderInfo();
				m_bPickUpAll = true;
				map< int, int > tempMap;
				Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( m_TempCashItemToInven, tempMap, tempMap);
				return false;
			}
	#endif SERV_PSHOP_AGENCY
			if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( pItemTemplate->GetItemID() ) == true )
			{
				m_TempCashItemToInven = pItemSlot->GetKBillOrderInfo();
				if(m_TempCashItemToInven.m_vecSocketOption[0] == 0 )
				{
					// 소켓 옵션그룹이 처음부터 0 이다?
					MessageBox(NULL, L"소켓 옵션 그룹 ID 가 0 입니다. 셋팅 체크 필요", L"Error", MB_OK);
				}
				else
				{
					// 첫번째 소켓 옵션을 강제로 셋팅을
					vector<int> vecSocketIdList;	/// iSocketGroupID 에 해당하는 SocketID 들을 얻어옴
					if ( 0 != g_pData->GetSocketItem()->GetSocketIdListForCashAvatar( m_TempCashItemToInven.m_vecSocketOption[0], vecSocketIdList, pItemTemplate->GetItemType() ) )
					{
						map< int, int > mapSelectedOptionList;
						map< int, int > mapSelectedOptionGroupList;
						mapSelectedOptionList.insert( std::make_pair( pItemTemplate->GetItemID(), vecSocketIdList[0]));
						mapSelectedOptionGroupList.insert( std::make_pair( vecSocketIdList[0], m_TempCashItemToInven.m_vecSocketOption[0]));
						m_bPickUpAll = true;
						Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( m_TempCashItemToInven, mapSelectedOptionList, mapSelectedOptionGroupList );
						return false;
					}
					else
					{
						// 그룹에 해당하는 소켓이 없다?
						MessageBox(NULL, L"소켓 옵션 그룹 에 해당하는 소켓 ID가 없습니다. 셋팅 체크 필요", L"Error", MB_OK);
					}
					return false;
				}				
			}
			else
			{
				m_TempCashItemToInven = pItemSlot->GetKBillOrderInfo();
				//KBillProductInfo* pKBillProductInfo = g_pData->GetItemManager()->GetCashItemProduct( m_TempCashItemToInven.m_iProductNo );
				//if( NULL != pKBillProductInfo )
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_TempCashItemToInven.m_kBillProductInfo.m_iProductID );
					if ( pItemTemplet != NULL )
					{
						m_bPickUpAll = true;
						map< int, int > tempMap;
						Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( m_TempCashItemToInven, tempMap, tempMap );
						return false;
					}
				}
			}
		}
	}

	m_bPickUpAll = false;

	return false;	
}
#endif CASH_INVEN_PICKUP_ALL


#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
void CX2CashShop::ChinaCash_Tap_Change_Direct_Charge(bool _bShow)
{
	bool _bshowTemp = !_bShow; 

	CKTDGUIStatic* pStaticChangeFont_Giantcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GIANTPOINT" );
	CKTDGUIStatic* pStaticChangeFont_Goldcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GOLDCOIN" );
	CKTDGUIStatic* pStaticChangeFONT_AFTER_GIANT = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_GIANT" );
	CKTDGUIStatic* pStaticChangeFONT_AFTER_ELS_CASH = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_ELS_CASH" );

	pStaticChangeFont_Giantcash->SetShowEnable(false,false);
	pStaticChangeFont_Goldcash->SetShowEnable(false,false);

	pStaticChangeFONT_AFTER_GIANT->SetShowEnable(false,false);
	pStaticChangeFONT_AFTER_ELS_CASH->SetShowEnable(false,false);

	CKTDGUIStatic* pStatic_Graphic = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Elsword_Tap" );
	pStatic_Graphic->SetShowEnable(_bshowTemp,_bshowTemp);
	CKTDGUIStatic* pStatic_Giant_Tap = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Giant_Tap" );
	pStatic_Giant_Tap->SetShowEnable(_bshowTemp,_bshowTemp);
	/*
	CKTDGUIComboBox* pCombo_cash= (CKTDGUIComboBox*) m_pDLGCashchange->GetControl( L"CashChangeList" );
	pCombo_cash->SetShowEnable(_bshowTemp, _bshowTemp);

	CKTDGUIButton* pCash_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_ok" );
	pCash_OK->SetShowEnable(_bshowTemp, _bshowTemp);

	CKTDGUIButton* pCash_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_cancel" );
	pCash_CANCEL->SetShowEnable(_bshowTemp, _bshowTemp);

	CKTDGUIButton* pCash_Giant_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_ok" );
	pCash_Giant_OK->SetShowEnable(_bshowTemp, _bshowTemp);

	CKTDGUIButton* pCash_Giant_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_cancel" );
	pCash_Giant_CANCEL->SetShowEnable(true, true);

	CKTDGUIStatic* pStaticChangeBingon = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"Exchange_cash_Bingon" );

	pStaticChangeBingon->SetShow(false);
	*/

	CKTDGUIStatic* pStatic_Direct_Charge_Tap = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Direct_Charge_Tap" );
	pStatic_Direct_Charge_Tap->SetShowEnable(_bShow,_bShow);

	CKTDGUIButton* pCash_Direct_Charge_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Direct_Charge_ok" );
	pCash_Direct_Charge_OK->SetShowEnable(_bshowTemp, _bshowTemp);
}


void CX2CashShop::SetShowChinaCash_Tap_Elsword(bool _bShow)
{
	bool _bshowTemp; 

	CKTDGUIStatic* pStaticChangeFont_Giantcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GIANTPOINT" );
	CKTDGUIStatic* pStaticChangeFont_Goldcash = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_GOLDCOIN" );
	CKTDGUIStatic* pStaticChangeFONT_AFTER_GIANT = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_GIANT" );
	CKTDGUIStatic* pStaticChangeFONT_AFTER_ELS_CASH = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"g_StateicFONT_AFTER_ELS_CASH" );

	pStaticChangeFont_Giantcash->SetShowEnable(false,false);
	pStaticChangeFont_Goldcash->SetShowEnable(false,false);

	if(m_bCashlistCheck == true)
	{
		pStaticChangeFONT_AFTER_GIANT->SetShowEnable(true,true);
		pStaticChangeFONT_AFTER_ELS_CASH->SetShowEnable(true,true);
	}
	else if(m_bCashlistCheck == false)
	{
		pStaticChangeFont_Giantcash->SetShowEnable(true,true);
		pStaticChangeFont_Goldcash->SetShowEnable(true,true);
		pStaticChangeFONT_AFTER_GIANT->SetShowEnable(false,false);
		pStaticChangeFONT_AFTER_ELS_CASH->SetShowEnable(false,false);
	}


	CKTDGUIStatic* pStatic_Graphic = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Elsword_Tap" );
	pStatic_Graphic->SetShowEnable(_bShow,_bShow);
	
	CKTDGUIComboBox* pCombo_cash= (CKTDGUIComboBox*) m_pDLGCashchange->GetControl( L"CashChangeList" );
	pCombo_cash->SetShowEnable(_bShow, _bShow);

	CKTDGUIButton* pCash_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_ok" );
	pCash_OK->SetShowEnable(_bShow, _bShow);

	CKTDGUIButton* pCash_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Exchange_cash_cancel" );
	pCash_CANCEL->SetShowEnable(_bShow, _bShow);
	
	CKTDGUIStatic* pStaticChangeBingon = (CKTDGUIStatic*)m_pDLGCashchange->GetControl( L"Exchange_cash_Bingon" );
	

	if(m_bLowCashCheck == true && _bShow == true)
	{
		pStaticChangeBingon->SetShow(true);
	}
	else if(m_bLowCashCheck == false && _bShow == true)
	{
		pStaticChangeBingon->SetShow(false);
	}
	else
	{
		pStaticChangeBingon->SetShow(false);
	}
}


void CX2CashShop::SetShowChinaCash_Tap_Giant(bool _bShow)
{
	CKTDGUIStatic* pStatic_Giant_Tap = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Giant_Tap" );
	pStatic_Giant_Tap->SetShowEnable(_bShow,_bShow);
	
	CKTDGUIButton* pCash_Giant_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_ok" );
	pCash_Giant_OK->SetShowEnable(_bShow, _bShow);

	CKTDGUIButton* pCash_Giant_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_cancel" );
	pCash_Giant_CANCEL->SetShowEnable(_bShow, _bShow);
}

void CX2CashShop::SetShowChinaCash_Tap_DirectCharge(bool _bShow)
{
	CKTDGUIStatic* pStatic_Direct_Charge_Tap = (CKTDGUIStatic*) m_pDLGCashchange->GetControl( L"BG_Cash_Direct_Charge_Tap" );
	pStatic_Direct_Charge_Tap->SetShowEnable(_bShow,_bShow);

	CKTDGUIButton* pCash_Direct_Charge_OK= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Direct_Charge_ok" );
	pCash_Direct_Charge_OK->SetShowEnable(_bShow, _bShow);

	// 취소 버튼은 기존 취소 버튼과 동일한 것을 사용합니다.
	CKTDGUIButton* pCash_Giant_CANCEL= (CKTDGUIButton*) m_pDLGCashchange->GetControl( L"Giant_cash_cancel" );
	pCash_Giant_CANCEL->SetShowEnable(_bShow, _bShow);
}

bool CX2CashShop::Handler_EGS_CASH_DIRECT_CHARGE_CN_REQ()
{
	KEGS_CASH_DIRECT_CHARGE_CN_REQ kPacket;

	kPacket.m_iUserUID = g_pData->GetMyUser()->GetUID();;

	g_pData->GetServerProtocol()->SendPacket( EGS_CASH_DIRECT_CHARGE_CN_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CASH_DIRECT_CHARGE_CN_ACK );

	return true;
}

bool CX2CashShop::Handler_EGS_CASH_DIRECT_CHARGE_CN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CASH_DIRECT_CHARGE_CN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_CASH_DIRECT_CHARGE_CN_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// 받은 정보로 인터넷 뱅킹 조수 연다
			// 테스트 기간동안은 운영자 이상만
			if(g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR)
			{
				ExecuteEpaySystem(kEvent.m_wstrToken);
			}
		}
	}

	return true;
}

void CX2CashShop::ExecuteEpaySystem( std::wstring wstrToken/* = L""*/ )
{
	STARTUPINFO stif;
	PROCESS_INFORMATION pi;
	memset(&stif,0,sizeof(STARTUPINFO));
	stif.cb=sizeof(STARTUPINFO);
	stif.dwFlags= STARTF_USESHOWWINDOW;
	stif.wShowWindow=SW_SHOW;

	//std::string command = localpath + patchername + " " + commandline;
	WCHAR	wszFullFilePath[MAX_PATH] = L"";
	WCHAR	wszFilePath[MAX_PATH] = L"";
	WCHAR	wszDriveName[10] = L"";
	WCHAR	wszMsg[MAX_PATH] = L"";
	WCHAR	wszExeFilePath[MAX_PATH] = L"";
	WCHAR	wszCommandLine[MAX_PATH] = L"";
	WCHAR	wszZone[MAX_PATH] = L"";
	WCHAR	wszUid[MAX_PATH] = L"";


	int iTempZone = (int)(g_pInstanceData->GetGiantGame() * 65536 +  g_pInstanceData->GetGiantZone());
	_itow(iTempZone,wszZone, 10 );
	//tempCommand =  (int)(g_pInstanceData->GetGiantZone() + g_pInstanceData->GetGiantGame());

	int iTempUid = (int)(g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
	_itow(iTempUid,wszUid, 10 );

	/*boost::str( boost::wformat( L"%d" ) % GetUID() );*/

	GetModuleFileNameW( NULL, wszFullFilePath, MAX_PATH );
	_wsplitpath(wszFullFilePath, wszDriveName, wszFilePath, NULL, NULL);
	// Data 폴더 패스 
	std::wstring wstrDataFolderFullPath(wszFullFilePath);

	wcscpy( wszFullFilePath, L"" );

	wcscat( wszFullFilePath, wszDriveName );
	wcscat( wszFullFilePath, wszFilePath );
	wcscat( wszFullFilePath, L"\\epay\\" );

	wcscat( wszExeFilePath, wszFullFilePath );
	wcscat( wszExeFilePath, L"\\epay.exe" );

	wcscat(wszCommandLine, L" -id ");
	wcscat(wszCommandLine, g_pInstanceData->GetUserID().c_str());
	wcscat(wszCommandLine, L" -zone ");
	//wcscat(wszCommandLine, L" ");

	wcscat(wszCommandLine, wszZone);
	wcscat(wszCommandLine, L" -cname ");
	//wcscat(wszCommandLine, L" ");
	wcscat(wszCommandLine, g_pData->GetMyUser()->GetSelectUnit()->GetNickName());
	wcscat(wszCommandLine, L" -cid ");
	//wcscat(wszCommandLine, L" ");
	wcscat(wszCommandLine, wszUid);

	if( L"" != wstrToken )
	{
		wcscat(wszCommandLine, L" -token ");
		wcscat(wszCommandLine, wstrToken.c_str());

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH_TEST_MODE
		if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
		{
			// token 값 확인을 위해서 메지시 박스 띄운다.
			MessageBoxW(NULL, wszCommandLine, L"Test", MB_OK);
		}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH_TEST_MODE
	}


	//wszCommandLine = boost::str(boost::wformat(L"-id %s, -zone %d, -cname %s, - cid %d") %L"a" %1 %L"f" %2 ).c_str();
	if(false == CreateProcessW(wszExeFilePath,wszCommandLine,NULL,NULL,0,0,NULL,wszFullFilePath,&stif,&pi))
	{
		ShellExecute(NULL, L"open", GET_STRING( STR_ID_14497 ), L"", L"", SW_SHOW ); 
		m_bRunEpay = true; //2011.12.21 epay 홈페이지가 떠도 충전 닫기 버튼을 누르면 갱신되게 수정
	}
	else
	{
		m_bRunEpay = true;
	}
}

#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH 

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
void CX2CashShop::ShowEventCategoryInCashShop()
{
	if ( m_pDLGFront->GetRadioButton_LUA( "tab_Event_2" ) != NULL )
		m_pDLGFront->GetRadioButton_LUA( "tab_Event_2" )->SetShowEnable( true, true );
}
void CX2CashShop::OffEventCategoryInCashShop()
{
	if ( m_pDLGFront->GetRadioButton_LUA( "tab_Event_2" ) != NULL )
		m_pDLGFront->GetRadioButton_LUA( "tab_Event_2" )->SetShowEnable( false, false );
}
#endif ADD_CASH_SHOP_CATEGORY_EVENT_2

//=======================================================================================//
//--------------------------------- ES_GLOBAL_CODE_AREA ---------------------------------//
//=======================================================================================//
