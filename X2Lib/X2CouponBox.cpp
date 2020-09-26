#include "StdAfx.h"
#include ".\x2couponbox.h"


#ifndef COUPON_SYSTEM // 이전 UI 제거
CX2CouponBox::CX2CouponBox( CKTDXStage* pNowState ) :
CX2ItemSlotManager( pNowState, NULL ),
m_bOpen( false ),
m_pDLGBack( NULL ), 
//m_vecpDLGSlot;
//m_vecItemList;
m_pDLGCouponNumberInput( NULL ),
m_iReqPageIndex(1),
m_iNowPageIndex(1),	// page 번호는 1부터 시작
m_iMaxPageIndex(1)
{
	// 변수 초기화

	// 변수 값 할당

	RegisterLuaBind();

}

CX2CouponBox::~CX2CouponBox()
{
	SAFE_DELETE_DIALOG( m_pDLGBack );
	SAFE_DELETE_DIALOG( m_pDLGCouponNumberInput );
}


HRESULT CX2CouponBox::OnFrameMove(double fTime, float fElapsedTime)
{
	CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}

void CX2CouponBox::RegisterLuaBind()
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCouponBox", this );
}


bool CX2CouponBox::MouseUp( D3DXVECTOR2 mousePos )
{
	return false;
}

bool CX2CouponBox::MouseDown( D3DXVECTOR2 mousePos )
{
	return false;
}

void CX2CouponBox::SetNowState( CX2State* pState )
{
	m_pNowState = pState;

	if( NULL != m_pDLGBack )
	{
		m_pDLGBack->SetStage( (CKTDXStage*) pState );
	}

	if( NULL != m_pDLGCouponNumberInput )
	{
		m_pDLGCouponNumberInput->SetStage( (CKTDXStage*) pState );
	}
}


bool CX2CouponBox::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case CBCUM_ACCEPT0:
		{
			KNXBTOrderInfo& order = m_vecItemList[0];
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ( order.m_ulOrderNo, order.m_ulProductNo, (int)order.m_usRemainOrderQuantity );
			return true;
		} break;

	case CBCUM_ACCEPT1:
		{
			KNXBTOrderInfo& order = m_vecItemList[1];
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ( order.m_ulOrderNo, order.m_ulProductNo, (int)order.m_usRemainOrderQuantity );
			return true;
		} break;

	case CBCUM_ACCEPT2:
		{
			KNXBTOrderInfo& order = m_vecItemList[2];
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ( order.m_ulOrderNo, order.m_ulProductNo, (int)order.m_usRemainOrderQuantity );
			return true;
		} break;

	case CBCUM_ACCEPT3:
		{
			KNXBTOrderInfo& order = m_vecItemList[3];
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ( order.m_ulOrderNo, order.m_ulProductNo, (int)order.m_usRemainOrderQuantity );
			return true;
		} break;

	case CBCUM_ACCEPT4:
		{
			KNXBTOrderInfo& order = m_vecItemList[4];
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ( order.m_ulOrderNo, order.m_ulProductNo, (int)order.m_usRemainOrderQuantity );
			return true;
		} break;

	case CBCUM_ACCEPT5:
		{
			KNXBTOrderInfo& order = m_vecItemList[5];
			Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ( order.m_ulOrderNo, order.m_ulProductNo, (int)order.m_usRemainOrderQuantity );
			return true;
		} break;


	case CBCUM_CLOSE_COUPON_BOX:
		{
			OpenCouponBoxDLG( false );
			return true;

		} break;

	case CBCUM_PREV_PAGE:
		{
			if( m_iNowPageIndex > 1 )
			{
				Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( m_iNowPageIndex-1, m_iMaxPageIndex );
			}
			return true;

		} break;

	case CBCUM_NEXT_PAGE:
		{
			if( m_iNowPageIndex < m_iMaxPageIndex )
			{
				Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( m_iNowPageIndex+1, m_iMaxPageIndex );
			}
			return true;

		} break;



	case CBCUM_USE_COUPON:
		{
			if( NULL != m_pDLGCouponNumberInput )
			{
				wstring wstrCouponNumber = L"";
				WCHAR wszControlName[64] = L"";
				bool bOK = false;

#if defined (SERV_COUNTRY_TWHK) || defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_ID) || defined(SERV_COUNTRY_TH) || defined (SERV_COUNTRY_IN)
				int iMaxCouponSize = 1;	
#elif defined (SERV_COUNTRY_US)
				int iMaxCouponSize = 4;	
#elif defined (SERV_COUNTRY_JP)
				int iMaxCouponSize = 6;	// Default 6
#elif defined (SERV_COUNTRY_EU)
				int iMaxCouponSize = 0;	// 쿠폰 사용안함
#elif defined (SERV_COUNTRY_BR) || defined (SERV_COUNTRY_PH)
				int iMaxCouponSize = 0;	// 쿠폰 사용안함
#else
				Not Configure Coupon	// 없는 국가는 넣으세요. 아래 코드 문제 없는지 검증 하시고요
#endif //SERV_COUNTRY_XX

#if defined (SERV_GLOBAL_BILLING)
				for( int i=0; i<iMaxCouponSize; i++ )
				{
					StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"EditBox%d", i );
					CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) m_pDLGCouponNumberInput->GetControl( wszControlName );

					wstrCouponNumber += pEditBox->GetText();
				
				}
#else
				Not Configure Coupon
#endif //defined (SERV_GLOBAL_BILLING)

				if ( wstrCouponNumber.size() > 0 )
					Handler_EGS_APPLY_COUPON_REQ( wstrCouponNumber.c_str() );
			}

			OpenCouponNumberInputDLG( false );
			return true;
		} break;

	case CBCUM_CLOSE_COUPON_INPUT_BOX:
		{
			OpenCouponNumberInputDLG( false );
			return true;
		} break;

	}
	
	return false;
}

bool CX2CouponBox::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{


		case EGS_PURCHASED_CASH_ITEM_LIST_ACK:
			{
				if ( m_bOpen == true )
				{
					Handler_EGS_PURCHASED_CASH_ITEM_LIST_ACK( hWnd, uMsg, wParam, lParam );
					return true;
				}
			} break;

		case EGS_GET_PURCHASED_CASH_ITEM_ACK:
			{
				if ( m_bOpen == true )
				{
					Handler_EGS_GET_PURCHASED_CASH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
					return true;
				}
			} break;
	
	case EGS_APPLY_COUPON_ACK:
		{
			Handler_EGS_APPLY_COUPON_ACK( hWnd, uMsg, wParam, lParam );
			return true;

		} break;
	}

	return false;
}





bool CX2CouponBox::Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( int iPageIndex, int nItemPerPage )
{
	m_iReqPageIndex = iPageIndex;

	KEGS_PURCHASED_CASH_ITEM_LIST_REQ kPacket;
	kPacket.m_iCurrentPage = iPageIndex;
	kPacket.m_nItemPerPage = nItemPerPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_PURCHASED_CASH_ITEM_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PURCHASED_CASH_ITEM_LIST_ACK );

	return true;
}

bool CX2CouponBox::Handler_EGS_PURCHASED_CASH_ITEM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PURCHASED_CASH_ITEM_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_PURCHASED_CASH_ITEM_LIST_ACK );
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_vecItemList = kEvent.m_vecOrderInfo;
			m_iNowPageIndex = m_iReqPageIndex;
			UpdateCouponBoxDLG( m_iNowPageIndex );
			return true;
		}
	}

	return false;
}

bool CX2CouponBox::Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ( uLong ulOrderNo, uLong ulProductNo, int iQuantity )
{
	KEGS_GET_PURCHASED_CASH_ITEM_REQ kPacket;
	kPacket.m_ulOrderNo = ulOrderNo;
	kPacket.m_ulProductNo = ulProductNo;
	kPacket.m_iQuantity = iQuantity;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_PURCHASED_CASH_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK );

	return true;
}

bool CX2CouponBox::Handler_EGS_GET_PURCHASED_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_PURCHASED_CASH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( m_iNowPageIndex, s_nItemPerPage );
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
		
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_109 ), (CKTDXStage*) m_pNowState );

			return true;
		}
	}

	return false;
}


bool CX2CouponBox::Handler_EGS_APPLY_COUPON_REQ( const WCHAR* wszCouponNumber )
{
	KEGS_APPLY_COUPON_REQ kPacket;
	kPacket.m_wstrCouponSerial = wszCouponNumber;

	g_pData->GetServerProtocol()->SendPacket( EGS_APPLY_COUPON_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_APPLY_COUPON_ACK );

	return true;
}

bool CX2CouponBox::Handler_EGS_APPLY_COUPON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_APPLY_COUPON_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_APPLY_COUPON_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_GLOBAL_BILLING
			g_pData->GetCashShop()->Handler_EGS_BILL_INVENTORY_INQUIRY_REQ(1, CASH_SHOP_DEPOSIT_SLOT_NUM);

			// 캐시 갱신
			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->Handler_EGS_CHECK_BALANCE_REQ();	
#else // SERV_GLOBAL_BILLING
			Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( m_iNowPageIndex, s_nItemPerPage );
#endif // SERV_GLOBAL_BILLING
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_110 ), (CKTDXStage*) m_pNowState );

			return true;
		}
	}

	return false;
}


void CX2CouponBox::OpenCouponBoxDLG( bool bOpen /*= true*/ )
{
	m_bOpen = bOpen;

	// note!! CX2CouponBox room은 cx2statemenu를 상속받은 클래스에서만 사용된다고 가정

#ifdef CHECK_INHERIT_STATE_MENU
	if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
	{
		CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
		if( NULL != pStateMenu )
		{
			if( true == bOpen )
			{
				pStateMenu->SetShowMouseClickUI( false, CX2State::MouseClickUI::HR_COUPON_BOX );
			}
			else
			{
				pStateMenu->SetShowMouseClickUI( true, CX2State::MouseClickUI::HR_COUPON_BOX );
			}
		}
	}



	if( true == bOpen )
	{
		SetEnable( true );
		SetShow( true );

		if( NULL == m_pDLGBack )
		{
			m_pDLGBack = new CKTDGUIDialog( (CKTDXStage*)m_pNowState, L"DLG_Gift_Box.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBack );
		}

		m_pDLGBack->SetShowEnable( true, true );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGBack, true );

		UpdateCouponBoxDLG( 1, true );
		
		// fix!!! m_imaxpageindex 를 지정해줘야~~
		Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, s_nItemPerPage ); 
	}
	else
	{
		SetEnable( false );
		SetShow( false );

		if( NULL != m_pDLGBack )
		{
			m_pDLGBack->SetShowEnable( false, false );
		}
	}
}



void CX2CouponBox::OpenCouponNumberInputDLG( bool bOpen /*= true*/ )
{
	if( true == bOpen )
	{
		if( NULL == m_pDLGCouponNumberInput )
		{
#ifdef NEW_VILLAGE_UI
			m_pDLGCouponNumberInput = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Coupon_Input_Window2.lua" );
#else
			m_pDLGCouponNumberInput = new CKTDGUIDialog( (CKTDXStage*)m_pNowState, L"DLG_Coupon_Input_Window2.lua" );
#endif
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCouponNumberInput );
		}

		if ( bOpen == true )
		{
			g_pMain->ChangeKoreanAndEnglishKey( false );
		}

		m_pDLGCouponNumberInput->SetShowEnable( true, true );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGCouponNumberInput, true );

		WCHAR wszControlName[64] = L"";
		for( int i=0; i<6; i++ )
		{
			StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"EditBox%d", i );
			CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) m_pDLGCouponNumberInput->GetControl( wszControlName );

			if( NULL != pEditBox )
			{
				pEditBox->ClearText();
			}
		}
	}
	else
	{
		if( NULL != m_pDLGCouponNumberInput )
		{
			m_pDLGCouponNumberInput->SetShowEnable( false, false );
		}
	}
}



void CX2CouponBox::UpdateCouponBoxDLG( int iPageIndex, bool bClearOnly /*= false*/ )
{
	if( NULL == m_pDLGBack )
		return;

	m_iNowPageIndex = iPageIndex;

	const int MAGIC_ITEM_PER_PAGE = 6; 

	CKTDGUIButton* pButton_Accept[MAGIC_ITEM_PER_PAGE] = { 0, };
	CKTDGUIButton* pButton_Discard[MAGIC_ITEM_PER_PAGE] = { 0, };
	CKTDGUIStatic* pStatic_Desc[MAGIC_ITEM_PER_PAGE] = { 0, };
	CKTDGUIStatic* pStatic_SendID[MAGIC_ITEM_PER_PAGE] = { 0, };


	WCHAR wszControlName[128] = L"";
	for( int i=0; i<s_nItemPerPage; i++ )
	{
		StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"%dRed1", i+1 );
		pButton_Discard[i] = (CKTDGUIButton*) m_pDLGBack->GetControl( wszControlName );

		StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"%dRed2", i+1 );
		pButton_Accept[i] = (CKTDGUIButton*) m_pDLGBack->GetControl( wszControlName );

		StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"%dGift_Name", i+1 );
		pStatic_Desc[i] = (CKTDGUIStatic*) m_pDLGBack->GetControl( wszControlName );

		StringCchPrintfW( wszControlName, ARRAY_SIZE(wszControlName), L"%dId", i+1 );
		pStatic_SendID[i] = (CKTDGUIStatic*) m_pDLGBack->GetControl( wszControlName );

		pButton_Accept[i]->SetShowEnable( false, false );
		pButton_Discard[i]->SetShowEnable( false, false );
		pStatic_Desc[i]->SetShowEnable( false, false );
		pStatic_SendID[i]->SetShowEnable( false, false );
	}




	if( true == bClearOnly )
		return;


	// todo!! page 번호 UI에 갱신

	
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		if( NULL != pItemSlot )
		{
			pItemSlot->DestroyItemUI();
		}
	}

	for( UINT i=0; i<m_vecItemList.size(); i++ )
	{
		KNXBTOrderInfo& order = m_vecItemList[i];

		pButton_Accept[i]->SetShowEnable( true, true );

		pStatic_Desc[i]->GetString(0)->msg = order.m_wstrProductName;
		pStatic_SendID[i]->GetString(0)->msg = order.m_wstrSenderGameID;

		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		if( NULL != pItemSlot )
		{
			int itemTID = wcstol( order.m_wstrProductID.c_str(), NULL, 10 );

            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID ); 
			//if( NULL == pItemTemplet )
			//{
			//	WCHAR wszText[256] = L"";
			//	wsprintf( wszText, L"잘못된 Item ID (%d) 입니다.", itemTID );
			//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), wszText, (CKTDXStage*) m_pNowState );
			//}
			pItemSlot->CreateItemUI( pItemTemplet, (int) order.m_usRemainOrderQuantity );
		}
	}

}





bool CX2CouponBox::GetCheckOperationCondition()
{
	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGBack ) == true )
		return false;

	return true;
}
#endif // COUPON_SYSTEM


