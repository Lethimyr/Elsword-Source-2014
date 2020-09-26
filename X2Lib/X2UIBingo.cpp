#include "StdAfx.h"

#ifdef SERV_EVENT_BINGO
#include "X2UIBingo.h"


CX2UIBingo::CX2UIBingo( CKTDXStage* pNowStage )
:m_iLotteryChange(0), 
m_iLotteryRecord(0),
m_iCheckedNumCnt(0), 
m_iCompleteLine(0), 
m_iUncompleteLine(0),
m_iResetChance(0),
m_iMixChance(0),
m_iPreAcuireNumber(-1),
m_bShowBingoUI(false),
m_bIsExpandButton(false),
m_pDLGBingo(NULL),
m_pDLGEventOpenButton(NULL),
m_pDLGRewardPopup(NULL),
m_hBingoOpenButton(INVALID_PARTICLE_SEQUENCE_HANDLE),
CX2ItemSlotManager( pNowStage, NULL)
{
	m_pDLGEventOpenButton = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Event_Bingo_Open_Button.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEventOpenButton );
	m_pDLGEventOpenButton->SetShowEnable( false, false );

	for( int i = 0; i < m_iMaxNumPresent ; ++i)
	{
		m_arrayPresentData[i].first = 257120;
		m_arrayPresentData[i].second = false;
	}

	for( int i = 0; i < m_iMaxNumBingoValue; ++i )
	{
		m_arrayBingoValue[i] = i;
	}
}


CX2UIBingo::~CX2UIBingo(void)
{
	SAFE_DELETE_DIALOG(m_pDLGEventOpenButton);
	SAFE_DELETE_DIALOG(m_pDLGBingo);
	SAFE_DELETE_DIALOG(m_pDLGRewardPopup);
}


HRESULT CX2UIBingo::OnFrameMove( double fTime, float fElapsedTime )
{
	if( false == m_bShowBingoUI )
		return S_OK;

	if( NULL != m_pDLGBingo && true == m_pDLGBingo->GetIsMouseOver() )
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

bool CX2UIBingo::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGBingo ) )
		return false;

	switch( uMsg )
	{
	case WM_MOUSEMOVE:
		{
			if( false == m_bShowBingoUI )
				return false;

			POINT mousePoint = { static_cast< short >( LOWORD( lParam ) ), static_cast< short >( HIWORD( lParam ) ) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( static_cast< float >( mousePoint.x ), 
												static_cast< float >( mousePoint.y ) );
			m_NowMousePos = mousePos;
			MouseMove( mousePos );
			return true;
		}
		break;
	}

	return false;
}

bool CX2UIBingo::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (wParam)
	{
	case UBCM_BUTTON_EXPAND: // 이벤트창 오픈 버튼 확대
		{
			SetExpandEventButton(true);
			return true;
		} break;
	case UBCM_BUTTON_MINIMIZE: // 이벤트창 오픈 버튼 축소
		{
			SetExpandEventButton(false);
			return true;
		} break;
	case UBCM_OPEN: // 이벤트창 열기
		{
			SetOpen(true);
			return true;
		} break;
	case UBCM_CLOSE: // 이벤트창 닫기
		{
			SetOpen(false);
			return true;
		} break;
	case UBCM_INITIALIZE: // 아이템 이용한 초기화 버튼
		{
			Handler_EGS_BINGO_EVENT_RESET_REQ();
			return true;
		} break;
	case UBCM_VALUE_MIX:  // 뒤섞기 버튼
		{
			Handler_EGS_BINGO_EVENT_MIX_REQ();
			return true;
		} break;
	case UBCM_LOTTERY:  // 빙고 뽑기
		{
			Handler_EGS_BINGO_EVENT_LOTTERY_REQ();
			return true;
		} break;
	case UBCM_REWARD_POPUP_OK: // 보상 알림 팝업 OK
		{
			if( NULL != m_pDLGBingo )
			{
				m_pDLGBingo->SetEnable(true);
			}

			if( NULL != m_pDLGRewardPopup )
			{
				m_pDLGRewardPopup->SetShowEnable(false, false);
			}

			if( m_iMaxNumPresent == m_iCompleteLine )
			{
				Handler_EGS_BINGO_EVENT_INIT_REQ();
			}
			return true;
		} break;
	}
	
	return false;
}


bool CX2UIBingo::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam)
	{
		case EGS_BINGO_EVENT_INFO_ACK:
			Handler_EGS_BINGO_EVENT_INFO_ACK(hWnd, uMsg, wParam, lParam);
			break;
		case EGS_BINGO_EVENT_LOTTERY_ACK:
			Handler_EGS_BINGO_EVENT_LOTTERY_ACK(hWnd, uMsg, wParam, lParam);
			break;
		case EGS_BINGO_EVENT_MIX_ACK:
			Handler_EGS_BINGO_EVENT_MIX_ACK(hWnd, uMsg, wParam, lParam);
			break;
		case EGS_BINGO_EVENT_RESET_ACK:
			Handler_EGS_BINGO_EVENT_RESET_ACK(hWnd, uMsg, wParam, lParam);
			break;
		case EGS_BINGO_EVENT_INIT_ACK:
			Handler_EGS_BINGO_EVENT_INIT_ACK(hWnd, uMsg, wParam, lParam);
			break;
	}
	return false;
}


/** @function : SetOpen
	@brief	: 빙고 메인 UI 오픈
	@param	: 오픈 여부 bVal
*/ 
void CX2UIBingo::SetOpen(bool bVal)
{
	m_bShowBingoUI = bVal;
	SetShowEventButton(!m_bShowBingoUI);
	CX2SlotManager::SetShow(m_bShowBingoUI);
	CX2ItemSlotManager::InvalidSelectedItem();
	CX2ItemSlotManager::InvalidSlotDesc();

	if( NULL != m_pDLGBingo )
	{	//show상태에 관계없이 false로 초기화.
		//event_info_ack를 정상적으로 받았을 경우는 true로 재설정 됩니다.
		m_pDLGBingo->SetShowEnable(false, false);
	}

	if( true == m_bShowBingoUI)
	{
		if( false == CreateBingoUI() )
			return;		

		Handler_EGS_BINGO_EVENT_INFO_REQ();
	}
}

/** @function : CreateBingoUI
	@brief	: 빙고 메인 UI DLG 생성
	@return	: 생성 성공 true, 실패 false
*/
bool CX2UIBingo::CreateBingoUI()
{
	if( NULL == m_pDLGBingo )
	{
		RegisterLuaBind();

		m_pDLGBingo = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_EVENT_BINGO_MAIN.lua" );
		if( NULL != m_pDLGBingo )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBingo );
			m_pDLGBingo->SetShowEnable( false, false );
		}
		else
		{
			return false;
		}		
	}

	SetSlotManagerLayer(XDL_POP_UP);
	return true;
}

/** @function : UpdateBingoNumber
	@brief	: 빙고 숫자 갱신
	@param	: 특정 인덱스만 업데이트 할 때 인덱스 값(iNumberIndex)
			  -1이라면 모든 값 갱신
*/
void CX2UIBingo::UpdateBingoNumber(int iNumberIndex/*= -1*/)
{
	//iNumberIndex 값이 범위를 벗어나면 return
	if( -1 != iNumberIndex &&
		(iNumberIndex < 0 || iNumberIndex >= m_iMaxNumBingoValue))
	{
		return;
	}

	if( -1 == iNumberIndex )
	{	//iNumberIndex값이 초기값이라면 모든 요소 갱신
		for( int i = 0; i < m_iMaxNumBingoValue; ++i )
		{
			SetNumberTex(i);
		}
	}
	else
	{ //iNumberIndex 값의 특정 요소 갱신
		SetNumberTex(iNumberIndex, true);
	}
}

/** @function : SetNumberTex
	@brief	: 빙고 숫자 텍스쳐 변경
			UpdateBingoNumber에서 호출되는 함수
	@param	: 텍스쳐 변경할 숫자의 인덱스값(iIndex), 이펙트 show 여부(bShowEffect)
*/
void CX2UIBingo::SetNumberTex(int iIndex, bool bShowEffect/* = false*/)
{
	if( NULL != m_pDLGBingo )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGBingo->GetControl(L"Bingo_Number"));
		if( NULL != pStatic && NULL != pStatic->GetPicture(iIndex) )
		{
			wstring NumberTextrueName = L"";
			wstring NumberKeyname = L"";
			GetNumberTextrueAndKeyName(NumberTextrueName, NumberKeyname, 
				GetNumExceptGainState(GetBingoNum(iIndex)), GetIsGainNum(iIndex) );

			if( true == NumberTextrueName.empty() || true == NumberKeyname.empty() )
			{
				return;
			}

			pStatic->GetPicture(iIndex)->SetTex(NumberTextrueName.c_str(), NumberKeyname.c_str());

			if( true == bShowEffect && true == GetIsGainNum(iIndex) )
			{
				if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
				{
					
					float fEffectPosX = (pStatic->GetPicture(iIndex)->GetPos().x + 75) ;
					float fEffectPosY = (pStatic->GetPicture(iIndex)->GetPos().y + 25) ;
					CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"UI_bingo_use_P01", fEffectPosX, fEffectPosY, 0, 1000, 1000, 1, 1, 1 );
					if( NULL != pParticle )
					{
						pParticle->SetOverUI(true);
					}
					pParticle = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"UI_bingo_use_P02", fEffectPosX, fEffectPosY, 0, 1000, 1000, 1, 1, 1 );
					if( NULL != pParticle )
					{
						pParticle->SetOverUI(true);
					}
				}
			}
		}
	}
}

/** @function : GetNumberKeyName
	@brief	: 빙고숫자, 획득여부에 따라 키값 얻어오기
	@param : OUT : 텍스쳐 파일 명 (NumberTextureName_), 텍스쳐 키값(NumberKeyName_),
			 IN : 빙고 숫자(iBingoValue_), 획득여부(bIsGain_)
*/
void CX2UIBingo::GetNumberTextrueAndKeyName ( OUT wstring& NumberTextureName_, OUT wstring& NumberKeyName_, IN int iBingoValue, IN bool bIsGain_) const
{
	if( iBingoValue > 99 || iBingoValue < 0)
		return;

	if( iBingoValue >= 81) //81이상은 텍스쳐 파일 명이 바뀜.
	{
		NumberTextureName_ = L"DLG_UI_Common_Texture_Event_06.tga";
	}
	else
	{
		if( true == bIsGain_ )
		{
			NumberTextureName_ = L"DLG_UI_Common_Texture_Event_05.tga";			
		}
		else
		{
			NumberTextureName_ = L"DLG_UI_Common_Texture_Event_04.tga";
		}
	}

	//키값
	WCHAR wBuf[50];
	if( true == bIsGain_ )
	{
		NumberKeyName_ = L"BG_BINGO_NUMBER_OVER_";
	}
	else
	{
		NumberKeyName_ = L"BG_BINGO_NUMBER_NORMAL_";
	}

	// 숫자가 한자리라며 앞에 0 붙여주기.
	if( iBingoValue >= 10 )
	{
		StringCchPrintfW( wBuf, sizeof(wBuf), L"%s%d", NumberKeyName_.c_str(), iBingoValue );
	}
	else
	{
		StringCchPrintfW( wBuf, sizeof(wBuf), L"%s0%d", NumberKeyName_.c_str(), iBingoValue );
	}

	NumberKeyName_ = wBuf;

}


/** @function : UpdatePresent
	@brief	: 빙고 상품 갱신
	@param	: 특정 인덱스만 업데이트 할 때 인덱스 값(iPresentIndex)
			  -1이라면 모든 값 갱신
*/
void CX2UIBingo::UpdatePresent( int iPresentIndex /*= -1*/ )
{
	//iNumberIndex 값이 범위를 벗어나면 return
	if( -1 != iPresentIndex &&
		(iPresentIndex < 0 || iPresentIndex >= m_iMaxNumPresent))
	{
		return;
	}

	if( -1 == iPresentIndex )
	{
		for( int i = 0 ; i < m_iMaxNumPresent; ++i)
		{
			SetPresentTex(i);
		}
	}
	else
	{
		SetPresentTex(iPresentIndex);
	}
}

/** @function : SetInfoData
	@brief	: 뽑기 기회, 빙고줄, 초기화, 뒤섞기 등 정보 갱신
	@param	: 초기화, 뒤섞기 버튼 갱신 여부 (bUpdateFunctionButton_)
*/
void CX2UIBingo::UpdateInfoData(bool bUpdateFunctionButton_/* = false*/)
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGBingo->GetControl(L"Bingo_Info"));
	if( NULL != pStatic )
	{
		if( NULL != pStatic->GetString(0) ) //빙고 기회 
		{
			pStatic->GetString(0)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", m_iLotteryChange));
		}

		if( NULL != pStatic->GetString(1) ) //사용한 빙고 기회
		{
			pStatic->GetString(1)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", m_iLotteryRecord));
		}

		if( NULL != pStatic->GetString(2) ) //완성 빙고 라인 수
		{
			pStatic->GetString(2)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", m_iCompleteLine));
		}

		if( NULL != pStatic->GetString(3) ) //남은 빙고 라인 수
		{
			pStatic->GetString(3)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", m_iUncompleteLine));
		}
	}

	if( true == bUpdateFunctionButton_ )
	{
		pStatic = static_cast<CKTDGUIStatic*>(m_pDLGBingo->GetControl(L"Bingo_Function"));
		if( NULL != pStatic )
		{
			if( NULL != pStatic->GetString(0) ) //초기화
			{
				pStatic->GetString(0)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", m_iResetChance));
			}

			if( NULL != pStatic->GetString(1) ) //뒤섞기
			{
				pStatic->GetString(1)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", m_iMixChance));
			}
		}
	}
}

/** @function : UpdateButtonUI
	@brief	: 기능 버튼 UI 갱신
	@param	: 초기화, 뒤섞기 버튼 갱신 여부 (bUpdateFunctionButton_)
*/
void CX2UIBingo::UpdateButtonUI(bool bUpdateFunctionButton_ /*= false*/)
{
	if( NULL == m_pDLGBingo )
		return;

	//빙고 코인이 있을 때만 빙고 버튼 활성화
	CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>(m_pDLGBingo->GetControl(L"Bingo_Lottery"));
	if( NULL != pButton )
	{
		if( m_iLotteryChange > 0 )
		{
			pButton->SetEnable(true);
		}
		else
		{
			pButton->SetEnable(false);
		}
	}

	//초기화, 뒤섞기 버튼 갱신
	if( true == bUpdateFunctionButton_ )
	{
		m_iCheckedNumCnt = GetCountOfCheckedNumber();
		//초기화
		pButton = static_cast<CKTDGUIButton*>(m_pDLGBingo->GetControl(L"Bingo_Initialize"));
		if( NULL != pButton )
		{
			//초기화 이용권 1개 이상
			if( m_iResetChance >= 1 )
			{
				pButton->SetEnable(true);
			}
			else
			{
				pButton->SetEnable(false);
			}
		}
		//뒤섞기
		pButton = static_cast<CKTDGUIButton*>(m_pDLGBingo->GetControl(L"Bingo_ValueMix"));
		if( NULL != pButton )
		{	
			//뒤섞기 이용권 1개 이상,  활성화된 숫자 10개 미만, //얻은 상품 1개 미만
			if( m_iMixChance >= 1 && m_iCheckedNumCnt < 10 && m_iCompleteLine < 1 )
			{
				pButton->SetEnable(true);
			}
			else
			{
				pButton->SetEnable(false);
			}
		}
	}
}
/** @function : NoticePresentGain
	@brief	: 상품 획득 알림 팝업
	@param	: 모든상품 획득 여부 (bIsAllComplete_)
*/
void CX2UIBingo::NoticePresentGain(bool bIsAllComplete_ /* = false */)
{
	if( NULL == m_pDLGRewardPopup )
	{
		m_pDLGRewardPopup = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_EVENT_BINGO_REWARD_POPUP.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRewardPopup );
		m_pDLGRewardPopup->SetShowEnable( false, false );
	}

	if( NULL != m_pDLGRewardPopup )
	{
		m_pDLGRewardPopup->SetShowEnable(true, true);

		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGRewardPopup->GetControl(L"Bingo_BackGround"));
		if( NULL != pStatic &&
			NULL != pStatic->GetPicture(0) &&
			NULL != pStatic->GetPicture(1) )
		{
			if( true == bIsAllComplete_ )
			{
				pStatic->GetPicture(0)->SetShow(false);
				pStatic->GetPicture(1)->SetShow(true);
			}
			else
			{
				pStatic->GetPicture(0)->SetShow(true);
				pStatic->GetPicture(1)->SetShow(false);
			}
		}
	}

	if( NULL != m_pDLGBingo )
	{
		m_pDLGBingo->SetEnable(false);
	}

	//캐쉬 인벤토리 갱신
	if( NULL != g_pData && NULL != g_pData->GetCashShop() )
	{
		g_pData->GetCashShop()->Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
	}
}

/** @function : NoticeBingoResult
	@brief	: 빙고 결과 알림
	@param	: 빙고 성공, 실패 여부(bIsAllComplete_)
*/
void CX2UIBingo::NoticeBingoResult(bool bShow_, bool bIsSucces_/* = false*/)
{
	if( NULL != m_pDLGBingo )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGBingo->GetControl(L"Bingo_Result"));
		if( NULL != pStatic && NULL != pStatic->GetPicture(0) )
		{
			if( true == bShow_ )
			{
				wstring strTextureKeyName = L"";
				int iResultRand = (rand()%3) ; //0~2 텍스쳐 중 선택
				if( true == bIsSucces_ )
				{
					iResultRand += 3; //성공했다면 +3
				}
				switch ( iResultRand )
				{
				case 0: strTextureKeyName = L"TEXT_BAD_1"; break;
				case 1:	strTextureKeyName = L"TEXT_BAD_2"; break;
				case 2: strTextureKeyName = L"TEXT_BAD_3"; break;
				case 3:	strTextureKeyName = L"TEXT_GOOD_1"; break;
				case 4:	strTextureKeyName = L"TEXT_GOOD_2"; break;
				case 5:	strTextureKeyName = L"TEXT_GOOD_3"; break;
				}

				if( true == bIsSucces_ )
				{
					pStatic->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_Event_02.tga", strTextureKeyName.c_str());
				}
				else
				{
					pStatic->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_Event_03.tga", strTextureKeyName.c_str());
				}
			}

			if( NULL != pStatic->GetString(0) )
			{
				if( m_iPreAcuireNumber >= 0 )
				{
					pStatic->GetString(0)->msg = GET_REPLACED_STRING((STR_ID_19111, "i", m_iPreAcuireNumber));
				}
				else
				{
					pStatic->GetString(0)->msg = L"";
				}
			}

			pStatic->SetShow(bShow_);
		}
	}
}
/** @function : SetPremiumTex
	@brief	: 빙고 상품 아이콘 UI 변경
			UpdatePremium에서 호출되는 함수
	@param : 아이콘 변경할 상품의 인덱스값(iIndex)
*/
void CX2UIBingo::SetPresentTex(int iIndex)
{
	if( iIndex >= m_iMaxNumPresent )
		return;

	if( 0 == m_arrayPresentData[iIndex].first )
	{
		return;
	}

	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlot( iIndex );
	if( NULL != pItemSlot )
	{
		if ( pItemSlot->IsCreatedItemUI() == true )
		{
			pItemSlot->ResetItemUI( m_arrayPresentData[iIndex].first);
		}
		else
		{
			pItemSlot->CreateItemUI( m_arrayPresentData[iIndex].first);
		}

		//획득한 아이템이라면 툴팁 보이지 않도록 설정
		pItemSlot->SetEnable(!m_arrayPresentData[iIndex].second);
		pItemSlot->SetShowImpossiblePicture(m_arrayPresentData[iIndex].second);
	}
}

/** @function : SetShowEventButton
	@brief	: 이벤트 버튼의 show 설정 // 캐쉬샵 show 상태와 일치
	@param : show 상태(val)
*/
void CX2UIBingo::SetShowEventButton(bool bVal, bool bShowEffect/* = false*/)
{
	if( NULL != m_pDLGEventOpenButton )
	{
		m_pDLGEventOpenButton->SetShowEnable(bVal,bVal);
		
		if( true == bVal )
		{
			SetExpandEventButton(false);

			if( true == bShowEffect && NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
			{
				CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>(m_pDLGEventOpenButton->GetControl(L"Bingo_Minimize"));
				if( NULL != pButton && INVALID_PARTICLE_SEQUENCE_HANDLE == m_hBingoOpenButton)
				{					
					float fEffectPosX = (pButton->GetPos().x + 43);
					float fEffectPosY = (pButton->GetPos().y + 41);
					CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"UI_bingoRing_active_P01", fEffectPosX, fEffectPosY, 0, 1000, 1000, 1, 99999, 99999 );
					if( NULL != pParticle )
					{
						m_hBingoOpenButton = pParticle->GetHandle();
						pParticle->SetOverUI(true);
					}
				}
			}
		}
		else
		{
			if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
			{
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hBingoOpenButton);
			}
		}
	}

}

/** @function : SetExpandEventButton
	@brief	: 이벤트 버튼의 확대 설정
	@param : 확대 여부 val(expand : true)
*/
void CX2UIBingo::SetExpandEventButton(bool bVal)
{
	if( NULL != m_pDLGEventOpenButton )
	{
		//축소 상태 버튼
		CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>(m_pDLGEventOpenButton->GetControl(L"Bingo_Minimize"));
		if( NULL != pButton )
		{
			pButton->SetShowEnable(!bVal, !bVal);
		}

		//확대 상태 버튼
		
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGEventOpenButton->GetControl(L"Bingo_Expand"));//배경
		if( NULL != pStatic )
		{
			pStatic->SetShowEnable(bVal, bVal);
		}
		
		pButton = static_cast<CKTDGUIButton*>(m_pDLGEventOpenButton->GetControl(L"Bingo_Expand_Exit"));//exit 버튼
		if( NULL != pButton )
		{
			pButton->SetShowEnable(bVal, bVal);
		}
		
		pButton = static_cast<CKTDGUIButton*>(m_pDLGEventOpenButton->GetControl(L"Bingo_Open"));//빙고판 오픈 버튼
		if( NULL != pButton )
		{
			pButton->SetShowEnable(bVal, bVal);
		}

		m_bIsExpandButton = bVal;

		if( true == m_bIsExpandButton )
		{
			if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
			{
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hBingoOpenButton);
			}
			m_pDLGEventOpenButton->SetCloseCustomUIEventID(UBCM_BUTTON_MINIMIZE);
		}
		else
		{
			m_pDLGEventOpenButton->SetCloseCustomUIEventID(-1);
		}
	}

}
/** @function : SetCompleteLine
	@brief	: 상품라인 획득 정보 설정
*/
void CX2UIBingo::UpdateCompleteLine()
{
	m_iCompleteLine = 0;
	m_iUncompleteLine = 0;

	for( int i = 0 ; i < m_iMaxNumPresent ; ++i )
	{
		if( true == m_arrayPresentData[i].second )
		{
			++m_iCompleteLine;
		}
		else
		{
			++m_iUncompleteLine;
		}
	}
}

/** @function : SetBingoNum
	@brief	: 빙고 숫자 설정
	@param : 갱신할 빙고 숫자 ( BingoNum_ )
	@return : 갱신 성공 true, 실패 false
*/
bool CX2UIBingo::SetBingoNum(  byte BingoNum_ )
{
	int iIndex = GetBingoIndexByNum(BingoNum_);
	if( -1 != iIndex )
	{
		if( m_arrayBingoValue[ iIndex ] != BingoNum_ )
		{
			m_arrayBingoValue[ iIndex ] = BingoNum_;
			return true;
		}
	}

	return false;
}

/** @function : SetBingoNum
	@brief	: 빙고판 설정
	@param : 갱신할 빙고목록 벡터(vecBingoNum_)
*/
void CX2UIBingo::SetBingoNumList(const vector<byte>& vecBingoNum_)
{
	if( m_iMaxNumBingoValue == vecBingoNum_.size() )
	{
		int iIndex = 0;
		for( vector<byte>::const_iterator it = vecBingoNum_.begin();
			it != vecBingoNum_.end(); ++it )
		{
			m_arrayBingoValue[iIndex++] = *it;
		}
	}
}

/** @function : SetPresentInfo
	@brief	: 갱신할 빙고목록 벡터<아이템ID, 획득여부>(vecPresentInfo_) 
*/
void CX2UIBingo::SetPresentList(const std::vector< std::pair<int, bool> >& vecPresentInfo_)
{
	if( m_iMaxNumPresent == vecPresentInfo_.size() )
	{
		int iIndex = 0;
		for( std::vector< std::pair<int, bool> >::const_iterator it = vecPresentInfo_.begin();
			it != vecPresentInfo_.end(); ++it )
		{
			m_arrayPresentData[iIndex].first = it->first;
			m_arrayPresentData[iIndex].second = it->second;
			++iIndex;
		}
	}
}

/** @function : SetPresentInfo
	@brief	:  갱신할 빙고목록 벡터(vecPresentInfo_) 
			   무조건 미획득 상태로 설정
*/
void CX2UIBingo::SetPresentList(const std::vector< int >& vecPresentInfo_)
{
	if( m_iMaxNumPresent == vecPresentInfo_.size() )
	{
		int iIndex = 0;
		for( std::vector< int >::const_iterator it = vecPresentInfo_.begin();
			it != vecPresentInfo_.end(); ++it )
		{
			m_arrayPresentData[iIndex].first = *it;
			m_arrayPresentData[iIndex].second = false;;
			++iIndex;
		}
	}
}

/** @function : SetAcquiredPresentList
	@brief	:  갱신할 상품목록 인덱스 벡터(vecAcuiredPresent_) 
*/
void CX2UIBingo::SetAcquiredPresentList(const vector<byte>& vecAcuiredPresentIndex_)
{
	for( vector<byte>::const_iterator it = vecAcuiredPresentIndex_.begin() ;
		it != vecAcuiredPresentIndex_.end(); ++it)
	{
		m_arrayPresentData[*it].second = true;
	}
}

/** @function : GetBingoNum
	@brief : 인덱스 이용해서 빙고값 얻기 
		     획득 여부를 표현하는 +100 값은 제외
	@param : 인덱스(iIndex)
	@return : 빙고 숫자 
*/
inline byte CX2UIBingo::GetBingoNum(int iIndex) const
{
	return ( m_arrayBingoValue[iIndex] >= 100 ) ? m_arrayBingoValue[iIndex] - 100 : m_arrayBingoValue[iIndex];
}

/** @function : GetIsGainNum
	@brief : 인덱스 이용해서 해당 인덱스의 빙고숫자의 획득 여부 얻기
	@param : 인덱스(iIndex)
	@return : 빙고 숫자 획득 true, 미획득 false;
*/
inline bool CX2UIBingo::GetIsGainNum(int iIndex) const
{
	return ( m_arrayBingoValue[iIndex] >= 100 ) ? true : false;
}

/** @function : GetCountOfCheckedNumber
	@brief : 획득한 숫자의 갯수를 얻어오는 함수
	@return : 획득한 숫자의 갯수
*/
inline int CX2UIBingo::GetCountOfCheckedNumber() const
{
	int iCountOfCheckedNumber = 0;	/// 획득한 숫자의 갯수
	/// 빙고판의 숫자 갯수만큼 loop 실행
	for ( int i = 0; i < m_iMaxNumBingoValue; ++i )
	{
		if ( GetIsGainNum( i ) )
			++iCountOfCheckedNumber;
	}

	return iCountOfCheckedNumber;
}


/** @function : GetBingoIndexByNum
	@brief : 유저 빙고판 내에 들어있는 숫자라면 해당 인덱스 얻기
	@param : 빙고숫자(byteNum)
	@return : 빙고숫자 내의 위치 인덱스, 없는숫자라면 -1 return
*/
const int CX2UIBingo::GetBingoIndexByNum(byte byteNum_) const
{
	byteNum_ = GetNumExceptGainState( byteNum_ );

	int iIndex = -1;
	for( int i = 0; i < m_iMaxNumBingoValue ; ++i )
	{
		if( byteNum_ == GetNumExceptGainState(m_arrayBingoValue[i]) )
		{
			iIndex = i;
			break;
		}
	}
	return iIndex;
}


/** @function : GetNumExceptGainState
	@brief : 획득 여부 제외한 실제 숫자 값 얻기 
	@param : 빙고숫자(byteNum)
	@return : 획득 여부 제외 숫자 값
*/
inline byte CX2UIBingo::GetNumExceptGainState(byte ByteNum_) const
{
	return ( ByteNum_ >= 100 ) ? ByteNum_ - 100 : ByteNum_ ;
}
/** @function : Handler_EGS_BINGO_EVENT_INFO_REQ
	@brief : 빙고 숫자, 상품 정보 등에 대한 자료 요청
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_INFO_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_BINGO_EVENT_INFO_REQ );
	g_pMain->AddServerPacket( EGS_BINGO_EVENT_INFO_ACK );

	return true;
}

/** @function : Handler_EGS_BINGO_EVENT_INFO_REQ
	@brief : 빙고 숫자, 상품 정보 등에 대한 자료 얻기
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BINGO_EVENT_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_BINGO_EVENT_INFO_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//자료 셋팅
			m_iLotteryChange	= kEvent.m_iLotteryChance;
			m_iLotteryRecord	= kEvent.m_iLotteryRecord;
			m_iResetChance		= kEvent.m_iResetChance;
			m_iMixChance		= kEvent.m_iMixChance;

			if( false == kEvent.m_vecBingoBoard.empty() )
			{
				SetBingoNumList(kEvent.m_vecBingoBoard);
			}
			
			if( false == kEvent.m_vecPresentInfo.empty() )
			{
				SetPresentList(kEvent.m_vecPresentInfo);
			}

			//정보 갱신
			UpdateBingoNumber();
			UpdatePresent();
			UpdateCompleteLine();
			UpdateInfoData(true);
			UpdateButtonUI(true);
			NoticeBingoResult(false);
				
			if( NULL != m_pDLGBingo )
			{
				m_pDLGBingo->SetShowEnable(m_bShowBingoUI, m_bShowBingoUI);
			}
			return true;
		}
		else
		{
			m_bShowBingoUI = false;
			SetShowEventButton(true);
			return true;
		}
	}

	return false;
}

/** @function : Handler_EGS_BINGO_EVENT_LOTTERY_REQ
	@brief : 빙고 숫자 뽑기
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_LOTTERY_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_BINGO_EVENT_LOTTERY_REQ );
	g_pMain->AddServerPacket( EGS_BINGO_EVENT_LOTTERY_ACK );
	return true;
}

/** @function : Handler_EGS_BINGO_EVENT_LOTTERY_ACK
	@brief : 빙고 숫자 뽑기, 이번 실행으로 획득한 선물 정보 얻기
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_LOTTERY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BINGO_EVENT_LOTTERY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_BINGO_EVENT_LOTTERY_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			m_iLotteryChange = kEvent.m_iLotteryChance;
			m_iLotteryRecord = kEvent.m_iLotteryRecord;

			m_iPreAcuireNumber = kEvent.m_byteNum;
			byte byteNumAndAcuireInfo = kEvent.m_byteNum + 100;//획득 정보를 포함시켜주기 위해 뽑힌 숫자 + 100
			bool bIsComplete = SetBingoNum( byteNumAndAcuireInfo );
			if( true == bIsComplete )
			{  //빙고판에 존재하는 숫자라면 업데이트 
				UpdateBingoNumber( GetBingoIndexByNum( byteNumAndAcuireInfo ) );
			}

			if( false == kEvent.m_vecAcquiredPresent.empty() )
			{ 
				SetAcquiredPresentList( kEvent.m_vecAcquiredPresent );
				UpdateCompleteLine(); //획득 라인 수 설정

				UpdatePresent();		
				NoticePresentGain( (m_iMaxNumPresent == m_iCompleteLine) ); //모든 빙고 획득 시 true
			}		
			UpdateInfoData();
			UpdateButtonUI(true);
			NoticeBingoResult(true, bIsComplete);
			return true;
		}
	}
	return false;
}

/** @function : Handler_EGS_BINGO_EVENT_MIX_REQ
	@brief : 빙고 뒤섞기 기능
			 빙고판의 숫자 정보 갱신
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_MIX_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_BINGO_EVENT_MIX_REQ );
	g_pMain->AddServerPacket( EGS_BINGO_EVENT_MIX_ACK );

	return true;
}

/** @function : Handler_EGS_BINGO_EVENT_MIX_ACK
	@brief : 빙고 뒤섞기 기능
			 빙고판의 숫자 정보 갱신
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_MIX_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BINGO_EVENT_MIX_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_BINGO_EVENT_MIX_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_iMixChance = kEvent.m_iMixChance;
			SetBingoNumList(kEvent.m_vecBingoBoard);
			UpdateBingoNumber();

			if( false == kEvent.m_vecAcquiredPresent.empty() )
			{ 
				SetAcquiredPresentList( kEvent.m_vecAcquiredPresent );
				UpdateCompleteLine(); //획득 라인 수 설정

				UpdatePresent();		
				NoticePresentGain( (m_iMaxNumPresent == m_iCompleteLine) ); //모든 빙고 획득 시 true
			}		
			UpdateInfoData(true);
			UpdateButtonUI(true);
			return true;
		}
	}

	return false;
}

/** @function : Handler_EGS_BINGO_EVENT_RESET_REQ
	@brief : 빙고 초기화 기능
			 빙고판의 숫자 정보, 상품 정보 갱신
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_RESET_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_BINGO_EVENT_RESET_REQ );
	g_pMain->AddServerPacket( EGS_BINGO_EVENT_RESET_ACK );

	return true;
}

/** @function : Handler_EGS_BINGO_EVENT_RESET_ACK
	@brief : 빙고 초기화 기능
			 빙고판의 숫자 정보, 상품 정보 갱신
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_RESET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BINGO_EVENT_RESET_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_BINGO_EVENT_RESET_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_iResetChance = kEvent.m_iResetChance;
			SetBingoNumList(kEvent.m_vecBingoBoard);
			SetPresentList(kEvent.m_vecPresentInfo);

			UpdateBingoNumber();
			UpdatePresent();

			UpdateCompleteLine();
			UpdateInfoData();
			UpdateInfoData(true);
			UpdateButtonUI(true);
			return true;
		}
	}

	return false;
}

/** @function : Handler_EGS_BINGO_EVENT_INIT_REQ
	@brief : 빙고 12줄 완성 후 초기화
			 빙고판의 숫자 정보, 상품 정보 갱신
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_INIT_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_BINGO_EVENT_INIT_REQ );
	g_pMain->AddServerPacket( EGS_BINGO_EVENT_INIT_ACK );

	return true;
}

/** @function : Handler_EGS_BINGO_EVENT_INIT_ACK
	@brief : 빙고 12줄 완성 후 초기화
			 빙고판의 숫자 정보, 상품 정보 갱신
*/
bool CX2UIBingo::Handler_EGS_BINGO_EVENT_INIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BINGO_EVENT_INIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_BINGO_EVENT_INIT_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			SetBingoNumList(kEvent.m_vecBingoBoard);
			SetPresentList(kEvent.m_vecPresentInfo);

			UpdateBingoNumber();
			UpdatePresent();

			UpdateCompleteLine();
			UpdateInfoData();
			UpdateButtonUI(true);
			return true;
		}
	}

	return false;
}
#endif //SERV_EVENT_BINGO

