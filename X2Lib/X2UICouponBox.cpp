#include "StdAfx.h"
#include ".\X2UICouponBox.h"

#ifdef COUPON_SYSTEM

const BYTE CX2UICouponBox::byDoublePageDistance = 5;
const UINT CX2UICouponBox::uiCouponNameAbailableLimit = 18;
const UINT CX2UICouponBox::uiCouponNameHasBeenUsedLimit = 22;

CX2UICouponBox::CX2UICouponBox()
{
	Initialize();
}

CX2UICouponBox::~CX2UICouponBox()
{
	g_pMain->DeleteServerPacket( EGS_COUPON_LIST_ACK );
	g_pMain->DeleteServerPacket( EGS_REGIST_COUPON_ACK );
	g_pMain->DeleteServerPacket( EGS_USE_ITEM_COUPON_ACK );

	SAFE_DELETE_DIALOG( m_pDlgCouponBox );
}

void CX2UICouponBox::Initialize()
{
	m_pDlgCouponBox = NULL;
	SetCouponTabType( CTT_AVAILABLE );
	SetCouponBoxType( SEnum::NCBT_WHOLE );
	m_uiNowPageIndex = 0 ;
	m_uiMaxPage = 0;
}

void CX2UICouponBox::SetOpen( const bool bShow_ )
{
	if ( true == bShow_ )
	{
		if ( NULL == m_pDlgCouponBox )
		{
			Initialize();

			// 쿠폰 UI 생성
			{
				m_pDlgCouponBox 
					= new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cash_Shop_Popup_Coupon.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgCouponBox );
				m_pDlgCouponBox->SetShowEnable(false, false);
			}

			SetCouponTabType( CTT_AVAILABLE ); 
		}

		// 서버로 쿠폰데이터 요청
		if( CTT_REGISTER == GetCouponTabType() &&
			NULL != m_pDlgCouponBox )
		{
			m_pDlgCouponBox->SetShowEnable(true, true);
		}
		else
			Handler_COUPON_LIST_REQ();			
	}
	else
	{
		SAFE_DELETE_DIALOG( m_pDlgCouponBox );
		Initialize();
	}
}

void CX2UICouponBox::RegisterLuaBind()
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCouponBox", this );
}

bool CX2UICouponBox::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_COUPON_LIST_ACK:
		return Handler_COUPON_LIST_ACK( hWnd, uMsg, wParam, lParam );
		break;
	case EGS_REGIST_COUPON_ACK:
		return Handler_REGISTER_COUPON_ACK( hWnd, uMsg, wParam, lParam );
		break;
	case EGS_USE_ITEM_COUPON_ACK:
		return Handler_USE_COUPON_ACK( hWnd, uMsg, wParam, lParam );
		break;
	default:
		break;
	}

	return false;
}

bool CX2UICouponBox::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case CBCUM_COUPON_BOX_CLOSE:
		{
			m_pDlgCouponBox->SetShowEnable( false, false );
			return true;
		} break;

	case CBCUM_SELECT_COUPON_TAB_AVAILABLE: /// 탭 - 사용 가능한 쿠폰
		{
			if( true == SetCouponTabType( CTT_AVAILABLE ) )
			{
				m_uiNowPageIndex = 0 ;
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;

	case CBCUM_SELECT_COUPON_TAB_HAVE_BEEN_USED:	/// 탭 - 쿠폰 사용 내역
		{
			if( true == SetCouponTabType( CTT_HAVE_BEEN_USED ) )
			{
				m_uiNowPageIndex = 0 ;
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;

	case CBCUM_SELECT_COUPON_TAB_REGISTER:	/// 탭 - 쿠폰 등록
		{
			if( true == SetCouponTabType( CTT_REGISTER ) )
			{
				m_vecCouponInfoList.clear();
				UpdateUI();
			}
			return true;
		} break;

	case CBCUM_SELECT_COUPON_LIST_TAB_ALL:	/// 구분 / 전체
		{
			if( true == SetCouponBoxType( SEnum::NCBT_WHOLE ) )
			{
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;

	case CBCUM_SELECT_COUPON_LIST_TAB_FREE_CHARGE:/// 구분 / 무료
		{
			if( true == SetCouponBoxType( SEnum::NCBT_ITEM ) )
			{
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;

	case CBCUM_SELECT_COUPON_LIST_TAB_DISCOUNT:/// 구분 / 할인
		{
			if( true == SetCouponBoxType( SEnum::NCBT_DISCOUNT ) )
			{
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;

	case CBCUM_PUSH_DOUBLE_LEFT_BUTTON:
		{
			USHORT usPrePageIndex = m_uiNowPageIndex;
			SetNowPageIndex( max( 0, m_uiNowPageIndex - CX2UICouponBox::byDoublePageDistance ) );
			
			if( usPrePageIndex != m_uiNowPageIndex )
			{
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;

	case CBCUM_PUSH_SINGLE_LEFT_BUTTON:
		{
			USHORT usPrePageIndex = m_uiNowPageIndex;
			SetNowPageIndex(max( 0, m_uiNowPageIndex - 1 ));
			if( usPrePageIndex != m_uiNowPageIndex )
			{
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;

	case CBCUM_PUSH_DOUBLE_RIGHT_BUTTON:
		{
			USHORT usPrePageIndex = m_uiNowPageIndex;
			SetNowPageIndex(max( 0, m_uiNowPageIndex + CX2UICouponBox::byDoublePageDistance ));
			if( usPrePageIndex != m_uiNowPageIndex  && 
				m_uiNowPageIndex <= m_uiMaxPage ) 
			{
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;

	case CBCUM_PUSH_SINGLE_RIGHT_BUTTON:
		{
			USHORT usPrePageIndex = m_uiNowPageIndex;
			SetNowPageIndex(max( 0, m_uiNowPageIndex + 1 ));
			if( usPrePageIndex != m_uiNowPageIndex && 
				m_uiNowPageIndex <= m_uiMaxPage ) 
			{
				Handler_COUPON_LIST_REQ();
			}
			return true;
		} break;
	case CBCUM_BUTTON_REGISTER_COUPON: 	/// 쿠폰 번호 등록 버튼
		{
			RegisterCoupon();
		} break;
	case CBCUM_BUTTON_USE_COUPON:
		{
			CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(lParam);
			if( NULL != pButton)
			{
				int iColumn = pButton->GetDummyInt(0);
				if( -1 != iColumn )
				{
					Handler_USE_COUPON_REQ( static_cast<UINT>(iColumn));
				}
			}
		} break;
	}

	return false;
}

void CX2UICouponBox::UpdateUI()
{
	switch ( GetCouponTabType() )
	{
	case CTT_AVAILABLE:
		{
			SetShowStaticForm( true, false, false, true, false, true );
			SetShowRadioButtonForm( true );
			SetShowButtonForm( true, false, true, true );
			SetEnableImeEditBox( false );
			SetShowAndCouponNum( true );
		} break;

	case CTT_HAVE_BEEN_USED:
		{
			SetShowStaticForm( false, true, false, false, true, true );
			SetShowRadioButtonForm( true );
			SetShowButtonForm( true, false, true, true );
			SetEnableImeEditBox( false );
			SetShowAndCouponNum( true );
		} break;

	case CTT_REGISTER:
		{
			// 사용하기 버튼 초기화
			WCHAR wszUseButton[128] = { 0, };
			for ( int i = 1; i < 4; ++i )
			{
				StringCchPrintfW( wszUseButton, 30, L"Button_UseCoupon%d", i );
				CKTDGUIButton* pButton =
					static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( wszUseButton ) );
				if ( NULL != pButton ) pButton->SetShowEnable( false, false );
			}


			SetShowStaticForm( false, false, true, false, false, false );
			SetShowRadioButtonForm( false );
			SetShowButtonForm( false, true, false, false );
			SetEnableImeEditBox( true );
			SetShowAndCouponNum( false );
		} break;
	}
}

void CX2UICouponBox::SetShowStaticForm( const bool bShowAbleToUse_, const bool bShowHaveBeenUsed_, 
	const bool bShowRegister_, const bool bShowFrameAvalable_, const bool bShowFrame_HaveBeenUsed_, const bool bPageNum_)
{
	{
		CKTDGUIStatic* pStaticAbleToUse
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticAbleToUse" ) );

		if ( NULL != pStaticAbleToUse )
			pStaticAbleToUse->SetShow( bShowAbleToUse_ );
	}

	{
		CKTDGUIStatic* pStaticHaveBeenUsed
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticHaveBeenUsed" ) );

		if ( NULL != pStaticHaveBeenUsed )
			pStaticHaveBeenUsed->SetShow( bShowHaveBeenUsed_ );
	}

	{
		CKTDGUIStatic* pStaticRegister
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticRegister" ) );

		if ( NULL != pStaticRegister )
			pStaticRegister->SetShow( bShowRegister_ );
	}

	// 프레임, 쿠폰 목록 스트링 (사용 가능 탭)
	{
		CKTDGUIStatic* pStaticFrame
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticFrame_Available" ) );

		if ( NULL != pStaticFrame )
			pStaticFrame->SetShow( bShowFrameAvalable_ );

		CKTDGUIStatic* pStaticCouponList01
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticCouponList_01_Available" ) );

		if ( NULL != pStaticCouponList01 )
			pStaticCouponList01->SetShow( bShowFrameAvalable_ );

		CKTDGUIStatic* pStaticCouponList02
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticCouponList_02_Available" ) );

		if ( NULL != pStaticCouponList02 )
			pStaticCouponList02->SetShow( bShowFrameAvalable_ );

		CKTDGUIStatic* pStaticCouponList03
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticCouponList_03_Available" ) );

		if ( NULL != pStaticCouponList03 )
			pStaticCouponList03->SetShow( bShowFrameAvalable_ );
	}
	// 프레임, 쿠폰 목록 스트링 (사용 내역 탭)
	{
		CKTDGUIStatic* pStaticFrame
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticFrame_HaveBeenUsed" ) );

		if ( NULL != pStaticFrame )
			pStaticFrame->SetShow( bShowFrame_HaveBeenUsed_ );

		CKTDGUIStatic* pStaticCouponList01
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"Static_01_HaveBeenUsed" ) );

		if ( NULL != pStaticCouponList01 )
			pStaticCouponList01->SetShow( bShowFrame_HaveBeenUsed_ );

		CKTDGUIStatic* pStaticCouponList02
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"Static_02_HaveBeenUsed" ) );

		if ( NULL != pStaticCouponList02 )
			pStaticCouponList02->SetShow( bShowFrame_HaveBeenUsed_ );

		CKTDGUIStatic* pStaticCouponList03
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"Static_03_HaveBeenUsed" ) );

		if ( NULL != pStaticCouponList03 )
			pStaticCouponList03->SetShow( bShowFrame_HaveBeenUsed_ );
	}

	// 페이지 숫자
	{
		CKTDGUIStatic* pStaticPageNum
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticPageNum" ) );

		if ( NULL != pStaticPageNum )
			pStaticPageNum->SetShow( bPageNum_ );
	}
}

void CX2UICouponBox::SetShowButtonForm( const bool bShowArrow_, const bool bShowRegister_, const bool bPageNum_, const bool bDesc_ )
{
	// 페이지 화살표
	{
		CKTDGUIButton* pButtonArrowToFirst
			= static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( L"ButtonToFirst" ) );

		if ( NULL != pButtonArrowToFirst )
			pButtonArrowToFirst->SetShowEnable( bShowArrow_, bShowArrow_ );

		CKTDGUIButton* pButtonArrowToLeft
			= static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( L"ButtonToLeft" ) );

		if ( NULL != pButtonArrowToLeft )
			pButtonArrowToLeft->SetShowEnable( bShowArrow_, bShowArrow_ );

		CKTDGUIButton* pButtonArrowToRight
			= static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( L"ButtonToRight" ) );

		if ( NULL != pButtonArrowToRight )
			pButtonArrowToRight->SetShowEnable( bShowArrow_, bShowArrow_ );

		CKTDGUIButton* pButtonArrowToLast
			= static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( L"ButtonToLast" ) );

		if ( NULL != pButtonArrowToLast )
			pButtonArrowToLast->SetShowEnable( bShowArrow_, bShowArrow_ );
	}

	// 쿠폰 등록 버튼
	{
		CKTDGUIButton* pButtonRegister
			= static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( L"ButtonRegister" ) );

		if ( NULL != pButtonRegister )
			pButtonRegister->SetShowEnable( bShowRegister_, bShowRegister_ );
	}

	// 쿠폰 툴팁
	{
		CKTDGUIButton* pButtonDesc
			= static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( L"CouponDesc1" ) );
		if ( NULL != pButtonDesc )
			pButtonDesc->SetShowEnable( bDesc_, bDesc_ );

		pButtonDesc = static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( L"CouponDesc2" ) );
		if ( NULL != pButtonDesc )
			pButtonDesc->SetShowEnable( bDesc_, bDesc_ );

		pButtonDesc = static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( L"CouponDesc3" ) );
		if ( NULL != pButtonDesc )
			pButtonDesc->SetShowEnable( bDesc_, bDesc_ );
	}
}

void CX2UICouponBox::SetShowRadioButtonForm( const bool bShowListType_ )
{
	CKTDGUIRadioButton* pRadioButtonAll
		= static_cast<CKTDGUIRadioButton*>( m_pDlgCouponBox->GetControl( L"RadioButtonAll" ) );

	if ( NULL != pRadioButtonAll )
		pRadioButtonAll->SetShowEnable( bShowListType_, bShowListType_ );

	CKTDGUIRadioButton* pRadioButtonFreeCharge
		= static_cast<CKTDGUIRadioButton*>( m_pDlgCouponBox->GetControl( L"RadioButtonFreeCharge" ) );

	if ( NULL != pRadioButtonFreeCharge )
		pRadioButtonFreeCharge->SetShowEnable( bShowListType_, bShowListType_ );

	CKTDGUIRadioButton* pRadioButtonDiscount
		= static_cast<CKTDGUIRadioButton*>( m_pDlgCouponBox->GetControl( L"RadioButtonDiscount" ) );

	if ( NULL != pRadioButtonDiscount )
		pRadioButtonDiscount->SetShowEnable( bShowListType_, bShowListType_ );
}

void CX2UICouponBox::SetEnableImeEditBox( const bool bEnable_ )
{
	CKTDGUIIMEEditBox* pImeEditBox = NULL;
	WCHAR wszEditBoxName[30] = { 0, };

	for ( int i = 0; i < 6; i++ )
	{
		StringCchPrintfW( wszEditBoxName, 30, L"EditBox%d", i );

		pImeEditBox 
			= static_cast<CKTDGUIIMEEditBox*>( m_pDlgCouponBox->GetControl( wszEditBoxName ) );

		if ( NULL != pImeEditBox )
		{
			pImeEditBox->SetShowEnable( bEnable_, bEnable_ );
			
			if ( i == 0 && bEnable_ == true )
				pImeEditBox->RequestFocus();
		}		
	}
}

/** @function : Handler_COUPON_LIST_REQ
	@brief : 서버로 쿠폰 리스트 요청.
	@param : bUsed true : 쿠폰 사용 내역 / false : 사용 가능 쿠폰
*/
void CX2UICouponBox::Handler_COUPON_LIST_REQ()
{
	ASSERT( NULL != m_pDlgCouponBox );

	KEGS_COUPON_LIST_REQ kPacket;
	switch( m_eCouponTabType )
	{
		case CTT_AVAILABLE:  kPacket.m_bUsed = false; break; /// 탭 - 사용 가능한 쿠폰
		case CTT_HAVE_BEEN_USED: kPacket.m_bUsed = true; break; /// 탭 - 쿠폰 사용 내역
		default:
			{
				ASSERT(!"Coupon Tab Error");
				kPacket.m_bUsed = true;
			}
			break;
	}

	// 서버에서 관리하는 페이지는 0부터 시작
	kPacket.m_usPageNum = m_uiNowPageIndex ;
	kPacket.m_sCouponBoxType = m_eCouponBoxType ;

	g_pData->GetServerProtocol()->SendPacket( EGS_COUPON_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_COUPON_LIST_ACK );
}

/** @function : Handler_COUPON_LIST_ACK
	@brief : 
	1. 현재 사용 가능 쿠폰 갱신( m_iCouponCount )
	2. 쿠폰 목록 갱신 ( m_vecCouponList )
*/
bool CX2UICouponBox::Handler_COUPON_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_COUPON_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

 	if( g_pMain->DeleteServerPacket( EGS_COUPON_LIST_ACK ) == true )
 	{
 		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
 		{
			// 현재 페이지의 쿠폰 목록을 클라이언트에 맞게 변경
 			m_vecCouponInfoList.clear();
 			BOOST_FOREACH( const KClientCouponBox& sKClientCouponBox, kEvent.m_vecCouponList)
			{
				// 쿠폰함에서 즉시 사용 가능한 쿠폰 인지 검사
				int iCouponBoxType = _wtoi( sKClientCouponBox.m_wstrCouponBoxType.c_str() );
				bool bIsInstantlyUse = (iCouponBoxType == 1220) ? true : false;

				CouponInfo sCouponInfo(sKClientCouponBox, kEvent.m_bUsed, bIsInstantlyUse );


				// 대상 아이템 툴팁
				if( false == sKClientCouponBox.m_vecTargetItem.empty() )
				{
					sCouponInfo.m_CouponTargetItemDesc += GET_STRING(STR_ID_28433);
					BOOST_FOREACH( const KCouponBoxTargetItem& CouponTargetItem, sKClientCouponBox.m_vecTargetItem )
					{
						// 대상 아이템이 1개이고, 해당 아이템의 이름이 All Item 이면 전체 대상으로 툴팁 설정
						if( 1 == sKClientCouponBox.m_vecTargetItem.size() &&
							wstring::npos != CouponTargetItem.m_wstrItemName.find( L"All Item") )
						{
							sCouponInfo.m_CouponTargetItemDesc = GET_STRING(STR_ID_28434);// [전체 아이템에 적용 가능]
							break;
						}
						else
						{
							if( NULL != g_pData->GetItemManager() )
							{
								sCouponInfo.m_CouponTargetItemDesc += L"\n";

								sCouponInfo.m_CouponTargetItemDesc += CouponTargetItem.m_wstrItemName;
								// 널 문자 제거
								wstring::size_type NullCharacterIndex = sCouponInfo.m_CouponTargetItemDesc.find(L'\0');
								if( NullCharacterIndex != wstring::npos )
								{
									sCouponInfo.m_CouponTargetItemDesc.erase(NullCharacterIndex) ;
								}
								
// 								// 기간 표기
// 								if( 0 < CouponTargetItem.m_sItemUseDuration )
// 								{
// 									WCHAR buff[256] = {0};
// 
// 									StringCchPrintfW( buff, ARRAY_SIZE(buff), L" %d일", CouponTargetItem.m_sItemUseDuration );
// 									sCouponInfo.m_CouponTargetItemDesc += buff;									
// 								}
// 								// 수량 표기
// 								if( 1 < CouponTargetItem.m_iItemQuantity )
// 								{
// 									WCHAR buff[256] = {0};
// 
// 									StringCchPrintfW( buff, ARRAY_SIZE(buff), L" %d개", static_cast<int>(CouponTargetItem.m_iItemQuantity) );
// 									sCouponInfo.m_CouponTargetItemDesc += buff;
// 								}
								// 변경 전 기간/수량 표기 방법 코드 유지
 								unsigned long uiPorductNo = static_cast<unsigned long>(_wtoi(CouponTargetItem.m_wstrItemCode.c_str()));
 								KNXBTProductInfo* pProductInfo = g_pData->GetItemManager()->GetCashItemProduct( uiPorductNo );
 								if( NULL != pProductInfo )
 								{
 									if( pProductInfo->m_sProductExpire > 1 )
 									{										
 										sCouponInfo.m_CouponTargetItemDesc += L" ";
 
 										WCHAR wBuf1[32];
 										StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%d일", static_cast<int>(pProductInfo->m_sProductExpire) );
 										sCouponInfo.m_CouponTargetItemDesc += wBuf1;
 										//sCouponInfo.m_CouponTargetItemDesc += GET_REPLACED_STRING( ( STR_ID_22727, "i", static_cast<int>(pProductInfo->m_sProductExpire) ));
 									}
 									
 									if( pProductInfo->m_usProductPieces > 1 ) // 수량
 									{
 										sCouponInfo.m_CouponTargetItemDesc += L" ";
 
 										WCHAR wBuf1[32];
 										StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%d개", static_cast<int>(pProductInfo->m_usProductPieces) );
 										sCouponInfo.m_CouponTargetItemDesc += wBuf1;
 										//sCouponInfo.m_CouponTargetItemDesc += GET_REPLACED_STRING( ( STR_ID_865, "i", static_cast<int>(pProductInfo->m_usProductPieces) ));
 									}
 								}
							}
						}
					}
				}


 				m_vecCouponInfoList.push_back( sCouponInfo );		
 			}

			// 쿠폰 3개 이하라면, 초기화를 위해 빈 요소 추가
			if( 3 > m_vecCouponInfoList.size() )
			{
				for( unsigned int i = 0; i < (3 - m_vecCouponInfoList.size()); ++i )
				{
					m_vecCouponInfoList.push_back( CouponInfo());
				}
			}

			// 배경 UI 설정
 			UpdateUI();

			// 쿠폰 목록 UI 설정
 			UpdateCouponInfo();

			// 사용 가능 / 남은 쿠폰 수량 설정
			SetShowAndCouponNum(true, kEvent.m_iCouponCount);

			// 페이지 설정
			UINT uiMaxPageNum = ( 0 == kEvent.m_iCouponCount ) ? 0 : ( (kEvent.m_iCouponCount-1) / 3);
 			SelectPageButton(kEvent.m_usPageNum, uiMaxPageNum);

			if( NULL != m_pDlgCouponBox )
			{
				m_pDlgCouponBox->SetShowEnable(true, true);
			}
 		}
 	}

	return true;
}
/** @function : Handler_REGISTER_COUPON_REQ
	@brief : 쿠폰 등록
	@param : wszCouponNumber// 쿠폰 번호 외 '-'이 포함 된 문자열@
*/
void CX2UICouponBox::Handler_REGISTER_COUPON_REQ( const WCHAR* wszCouponNumber )
{
	KEGS_REGIST_COUPON_REQ kPacket;
	ConvertWCHARToChar( kPacket.m_strCouponPin, wszCouponNumber );
 
 	g_pData->GetServerProtocol()->SendPacket( EGS_REGIST_COUPON_REQ, kPacket );
 	g_pMain->AddServerPacket( EGS_REGIST_COUPON_ACK );
}
bool CX2UICouponBox::Handler_REGISTER_COUPON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_REGIST_COUPON_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if( g_pMain->DeleteServerPacket( EGS_REGIST_COUPON_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING(STR_ID_27289), g_pMain->GetNowState() );
		}
	}
	return true;
}

/** @function : Handler_USE_COUPON_REQ
	@brief : 쿠폰 사용
			uiColumn값은 1부터 시작.
*/
void CX2UICouponBox::Handler_USE_COUPON_REQ( const UINT uiColumn )
{
	if( true == m_vecCouponInfoList.empty() )
		return;

	if( uiColumn > m_vecCouponInfoList.size() )
		return;

	KEGS_USE_ITEM_COUPON_REQ kPacket;
	kPacket.m_iCouponCardNo = m_vecCouponInfoList[uiColumn-1].m_iCouponCardNo;

	if( -1 != kPacket.m_iCouponCardNo )
	{
		g_pData->GetServerProtocol()->SendPacket( EGS_USE_ITEM_COUPON_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_USE_ITEM_COUPON_ACK );
	}
}
bool CX2UICouponBox::Handler_USE_COUPON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_USE_ITEM_COUPON_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	
	
	if( g_pMain->DeleteServerPacket( EGS_USE_ITEM_COUPON_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_28353 ), g_pMain->GetNowState() );
			if( NULL != g_pData && g_pData->GetCashShop() )
				g_pData->GetCashShop()->Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
			return true;
		}
	}
	return false;
}

void CX2UICouponBox::RegisterCoupon()
{
	if( NULL != m_pDlgCouponBox )
	{
		wstring wstrCouponNumber = L"";

		CKTDGUIIMEEditBox* pImeEditBox = NULL;
		WCHAR wszEditBoxName[30] = { 0, };

		for ( int i = 0; i < 6; i++ )
		{
			StringCchPrintfW( wszEditBoxName, 30, L"EditBox%d", i );

			pImeEditBox 
				= static_cast<CKTDGUIIMEEditBox*>( m_pDlgCouponBox->GetControl( wszEditBoxName ) );

			if ( NULL != pImeEditBox )
			{
				wstrCouponNumber += pImeEditBox->GetText();
				if( 5 != i )
					wstrCouponNumber += L"-";

				pImeEditBox->SetText(L"");
			}		
		}

		Handler_REGISTER_COUPON_REQ( wstrCouponNumber.c_str() );
	}
}
/** @function : SelectPageButton
	@brief : 
*/
void CX2UICouponBox::SelectPageButton( UINT uiPageNum_, UINT uiMaxPageNum_ )
{
	if( NULL != m_pDlgCouponBox )
	{
		switch ( GetCouponTabType() )
		{
		case CTT_AVAILABLE:
		case CTT_HAVE_BEEN_USED:
			{
				// 페이지 숫자
				CKTDGUIStatic* pStaticPageNum
					= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticPageNum" ) );

				if ( NULL != pStaticPageNum )
				{
					CKTDGUIControl::UIStringData* pString = pStaticPageNum->GetString(0);

					// 서버는 0부터 시작, UI는 1부터 시작 하기 때문에 +1 계산 해줌 
					SetNowPageIndex(uiPageNum_);
					m_uiMaxPage = uiMaxPageNum_;

					WCHAR wBuf1[256];
					StringCchPrintfW( wBuf1, ARRAY_SIZE(wBuf1), L"%d / %d", m_uiNowPageIndex + 1, m_uiMaxPage + 1 );
					if( NULL != pString )
						pString->msg = wBuf1;

				}
			} break;
		default:
			return;
			break;
		}
	}

	return;
}

/** @function : SetShowAndStringCouponNum
	@brief : ( 남은/ 사용가능한 )쿠폰 수량 설정
*/
void CX2UICouponBox::SetShowAndCouponNum( bool bShow_,  const UINT uiCouponNum_ )// 쿠폰 수량
{
	if( NULL != m_pDlgCouponBox )
	{
		// 페이지 숫자
		CKTDGUIStatic* pStaticPageNum
			= static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( L"StaticCouponNum" ) );

		if ( NULL != pStaticPageNum )
		{
			pStaticPageNum->SetShow( bShow_ );
			if( true == bShow_ )
			{
				CKTDGUIControl::UIStringData* pString = pStaticPageNum->GetString(0);
				if( NULL != pString )
				{
					pString->msg = GET_REPLACED_STRING( ( STR_ID_26368, "i", uiCouponNum_ ) ) ; 
				}
			}
		}
	}
}
/** @function : UpdateCouponInfo
	@brief : 쿠폰 목록 갱신
*/
void CX2UICouponBox::UpdateCouponInfo()
{
	if( NULL == m_pDlgCouponBox )
		return;

	{	// 쿠폰 목록 스트링 초기화
		WCHAR wszCouponList[128] = { 0, };
		for ( int i = 1; i < 4; ++i )
		{
			StringCchPrintfW( wszCouponList, ARRAY_SIZE(wszCouponList), L"StaticCouponList_0%d_Available", i );
			CKTDGUIStatic* pStatic = 
				static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( wszCouponList ) );
			if ( NULL != pStatic ) pStatic->SetShow( false );
		}
		for ( int i = 1; i < 4; ++i )
		{
			StringCchPrintfW( wszCouponList, ARRAY_SIZE(wszCouponList) , L"Static_0%d_HaveBeenUsed", i );
			CKTDGUIStatic* pStatic = 
				static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( wszCouponList ) );
			if ( NULL != pStatic ) pStatic->SetShow( false );
		}
		// 사용하기 버튼 초기화
		WCHAR wszUseButton[128] = { 0, };
		for ( int i = 1; i < 4; ++i )
		{
			StringCchPrintfW( wszUseButton, 30, L"Button_UseCoupon%d", i );
			CKTDGUIButton* pButton =
				static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( wszUseButton ) );
			if ( NULL != pButton ) pButton->SetShowEnable( false, false );
		}
		// 쿠폰 툴팁 초기화
		for ( int i = 1; i < 4; ++i )
		{
			StringCchPrintfW( wszUseButton, ARRAY_SIZE(wszUseButton) , L"CouponDesc%d", i );
			CKTDGUIButton* pButton =
				static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( wszUseButton ) );
			if( NULL != pButton )
				pButton->SetGuideDesc(L"");
		}
	}
	
	//서버로부터 받은 값을 토대로 쿠폰 목록 갱신
	UINT uiColumn = 1;
	BOOST_FOREACH( const CouponInfo& sCouponInfo, m_vecCouponInfoList )
	{		
		if( 3 >= uiColumn )
		{
			SetCouponInfo( sCouponInfo, uiColumn ) ;
			++uiColumn;
		}
		else
		{
			ASSERT(!"CouponList Error");
			break;
		}
	}
}

/** @function : SetCouponInfo
	@brief : 쿠폰 목록 스트링 및 사용하기 버튼 갱신
	@param : uiColumn_ 1~3 
*/
void CX2UICouponBox::SetCouponInfo(const CouponInfo& sCouponInfo_, const UINT uiColumn_ )
{
	if ( 3 < uiColumn_ )
		return;

	WCHAR wszCouponList[128] = { 0, };
	switch( GetCouponTabType() )
	{
		case CTT_AVAILABLE:
			{
				StringCchPrintfW( wszCouponList, 128, L"StaticCouponList_0%d_Available", uiColumn_ );
				CKTDGUIStatic* pStatic = 
					static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( wszCouponList ) );
				if( NULL != pStatic )
				{
					pStatic->SetShow(true);
					CKTDGUIControl::UIStringData* pString =	pStatic->GetString(0); // 구분
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrCouponBoxType;
					
					pString = pStatic->GetString(1);// 쿠폰 명
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrCouponName;

					pString = pStatic->GetString(2);// 할인 혜택
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrDiscountValue;

					pString = pStatic->GetString(3);// 지급일
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrObtainDataTime;

					pString = pStatic->GetString(4);// 만료일
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrExpireDataTime;
				}

				// 쿠폰 툴팁 초기화
				{
					WCHAR wszUseButton[128] = { 0, };
					StringCchPrintfW( wszUseButton, ARRAY_SIZE(wszUseButton) , L"CouponDesc%d", uiColumn_ );
					CKTDGUIButton* pButton =
						static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( wszUseButton ) );
					if( NULL != pButton )
						pButton->SetGuideDesc(sCouponInfo_.m_CouponTargetItemDesc.c_str());
				}

				// #NOTE
				// 사용하기 버튼이 노출되어야 한다면.
				if( true == sCouponInfo_.m_bIsInstantlyUse )
				{
					StringCchPrintfW( wszCouponList, 128, L"Button_UseCoupon%d", uiColumn_ );
					CKTDGUIButton* pButton =
						static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( wszCouponList ) );
					if ( NULL != pButton ) pButton->SetShowEnable( true, true );
				}
			} break;
		case CTT_HAVE_BEEN_USED:
			{
				StringCchPrintfW( wszCouponList, 128, L"Static_0%d_HaveBeenUsed", uiColumn_ );
				CKTDGUIStatic* pStatic = 
					static_cast<CKTDGUIStatic*>( m_pDlgCouponBox->GetControl( wszCouponList ) );
				if( NULL != pStatic )
				{
					pStatic->SetShow(true);
					CKTDGUIControl::UIStringData* pString =	pStatic->GetString(0); // 구분
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrCouponBoxType;

					pString = pStatic->GetString(1);// 쿠폰 명
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrCouponName;

					pString = pStatic->GetString(2);// 지급일
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrObtainDataTime;

					pString = pStatic->GetString(3);// 사용일
					if( NULL != pString )
						pString->msg = sCouponInfo_.m_wstrUsedDateTime;
				}

				// 쿠폰 툴팁 초기화
				{
					WCHAR wszUseButton[128] = { 0, };
					StringCchPrintfW( wszUseButton, ARRAY_SIZE(wszUseButton) , L"CouponDesc%d", uiColumn_ );
					CKTDGUIButton* pButton =
						static_cast<CKTDGUIButton*>( m_pDlgCouponBox->GetControl( wszUseButton ) );
					if( NULL != pButton )
						pButton->SetGuideDesc(sCouponInfo_.m_CouponTargetItemDesc.c_str());
				}
			} break;
		default:
			{
				return;
			} break;
	}
	return;
}
void CX2UICouponBox::CouponInfo::LimitLengthCouponName( OUT wstring& wstrCouponName_, bool bUsed_ )
{
	UINT iLimitLength = ( true == bUsed_ ) ? uiCouponNameHasBeenUsedLimit: uiCouponNameAbailableLimit;


	if( wstrCouponName_.length() > iLimitLength )
	{
		wstrCouponName_ = wstrCouponName_.substr(0,iLimitLength);
		wstrCouponName_  += L"...";
	}

	return;
}

void CX2UICouponBox::SetNowPageIndex( USHORT uiPageIndex_ )
{
	//  현재 페이지가 최대 페이지를 넘을 수 없도록 제한
	m_uiNowPageIndex = ( m_uiMaxPage < uiPageIndex_) ? m_uiMaxPage : uiPageIndex_;
}

bool CX2UICouponBox::SetCouponTabType(CX2UICouponBox::COUPON_TAB_TYPE val) 
{
	if( true == g_pMain->IsWaitingServerPacket( EGS_COUPON_LIST_ACK ) )
	{
		g_pMain->DeleteServerPacket( EGS_COUPON_LIST_ACK );
	}

	m_eCouponTabType = val; 
	return true;
}
bool CX2UICouponBox::SetCouponBoxType(SEnum::NEXON_COUPON_BOX_TYPE val)
{ 
	if( true == g_pMain->IsWaitingServerPacket( EGS_COUPON_LIST_ACK ) )
	{
		g_pMain->DeleteServerPacket( EGS_COUPON_LIST_ACK );
	}

	m_eCouponBoxType = val; 
	return true;
}
#endif // COUPON_SYSTEM