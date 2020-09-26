#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI

#include ".\X2UIManufacture.h"

// item slot들을 구분하기 위해서 제조재료 slot은 CX2Slot::ST_MATERIAL로 되어있고, 제조결과 아이템 slot은 CX2Slot::ST_CASH_SHOP_MAIN_NEW로 되어있다.
// GetSlotperPage를 이용해서 슬롯 개수를 구하고 있다 : X2Define에 있는 define 문은 쓰지 않고 있음.

CX2UIManufacture::CX2UIManufacture( CKTDXStage* pNowStage, const WCHAR* pFileName )
// : CX2UIDragable( pNowStage, NULL )
: CX2ItemSlotManager( pNowStage, NULL ),
m_bShow(false),
m_MovedPosition(0,0),					// D3DXVECTOR2
m_DLGPosition(0,0),						// D3DXVECTOR2
m_DLGSize(0,0),							// D3DXVECTOR2
m_pDLGManufacture(NULL),
m_pDLGManufactureResult(NULL),
m_pDLGMakeQuery(NULL),
//m_vecRecipeSlot.clear();
m_nNowPage(1),
m_nMaxPage(1),
m_bShowAllCharItem(false),
m_SelectedRecipeID(0),
//m_vecManufactureIDList[MT_END]
m_NowItemCategory(MT_NONE)
//{{ kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
, m_iNowOpenHouseID(0)	// 현재 열린 제조 NPC의 HOUSE_ID
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
//}} kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
#ifdef SERV_MANUFACTURE_FIX
, m_pDLGItemEnroll(NULL)
, m_EnrollItemUid( 0 )
, m_nQuantity( 1 )
#endif
, m_bWaitForManufactureResult(false)
{

	m_vecRecipeSlot.clear();

	for(int i=0; i<MT_END; ++i)
	{
		m_vecManufactureIDList[i].clear();
	}
}


CX2UIManufacture::~CX2UIManufacture(void)
{
	SAFE_DELETE_DIALOG(m_pDLGManufacture);
	SAFE_DELETE_DIALOG(m_pDLGManufactureResult);
	SAFE_DELETE_DIALOG(m_pDLGMakeQuery);	

	for(UINT i=0; i<m_SlotList.size(); i++)
	{
		CX2Slot* pSlot = m_SlotList[i];
		SAFE_DELETE(pSlot);
	}
	m_SlotList.clear();
	for(UINT j=0; j<m_vecRecipeSlot.size(); j++)
	{
		ManufactureSlot* pMSlot = m_vecRecipeSlot[j];
		SAFE_DELETE(pMSlot);
	}
	m_vecRecipeSlot.clear();

#ifdef SERV_MANUFACTURE_FIX
	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
#endif
}



HRESULT CX2UIManufacture::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();

	if(m_pDLGManufacture != NULL && m_pDLGManufacture->GetIsMouseOver() == true)
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

bool CX2UIManufacture::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
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
			if ( false == m_pDLGManufacture->GetIsMouseOver())
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
			if ( false == m_pDLGManufacture->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_MANUFACTURE, true);
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
		break;
	case WM_MOUSEMOVE:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			MouseMove( mousePos );
			return true;
		}
		break;
	}

	return bFlag;
}
// 
// bool CX2UIManufacture::MouseRButtonUp( D3DXVECTOR2 mousePos )
// {
// 	CX2SlotItem* pSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
// 	if(pSlot == NULL) return false;
// 
// 	// 슬롯 마우스오버 이미지를 지워주고
// 	m_pDLGSelectedItem->SetShow( false );
// 	InvalidSlotDesc();
// 
// 	return true;
// 
// }

bool CX2UIManufacture::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case UMCM_EXIT:
		{
			if(m_bWaitForManufactureResult == true)
				return true;

			SetShow(false);

			if(g_pTFieldGame != NULL)
			{
				CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
				if(pJoinNpc != NULL)
				{
					pJoinNpc->GetNpcShop()->SetKeyEvent();
				}
			}
		}
		return true;
	case UMCM_CATEGORY_WEAPON:
		{
			m_NowItemCategory = MT_WEAPON;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_COAT:
		{
			m_NowItemCategory = MT_DEFENCE_BODY;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_PANTS:
		{
			m_NowItemCategory = MT_DEFENCE_LEG;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_GLOVE:
		{
			m_NowItemCategory = MT_DEFENCE_HAND;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_SHOES:
		{
			m_NowItemCategory = MT_DEFENCE_FOOT;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_QUICKSLOT:
		{
			m_NowItemCategory = MT_QUICK_SLOT;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_ETC:
		{
			m_NowItemCategory = MT_ETC;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_SHOW_OTHER_CHARACTER_EQUIP_ON:
		{
			if(m_bShowAllCharItem == false)
			{
				m_bShowAllCharItem = true;
				GetManufactureList();
				ChangeManufactureTab( 1 );
			}
		}
		return true;
	case UMCM_SHOW_OTHER_CHARACTER_EQUIP_OFF:
		{
			if(m_bShowAllCharItem == true)
			{
				m_bShowAllCharItem = false;
				GetManufactureList();
				ChangeManufactureTab( 1 );
			}
		}
		return true;
	case UMCM_PREV_PAGE:
		{
			if( m_nNowPage > 1 ) m_nNowPage--;
			ChangeManufactureTab( m_nNowPage );
		}
		return true;
	case UMCM_NEXT_PAGE:
		{
			if( m_nNowPage < m_nMaxPage ) m_nNowPage++;
			ChangeManufactureTab( m_nNowPage );
		}
		return true;
	case UMCM_SELECT_ITEM:
		{
			// 이런 거 안 씀-_-
			// 아이템 셀렉트는 현재 MsgProc : MouseUp에 있습니다.

			// 아 왜 기획 바뀌는데-_- 이거 분명히 내가 결정해달라고 했었잖아 ㅠ
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			if( pButton->GetDummyInt(0) >= 0 && pButton->GetDummyInt(0) < (int)m_vecRecipeSlot.size())
			{
				ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[ pButton->GetDummyInt(0) ];
				if ( pRecipeSlot != NULL )
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"X2_Button_Mouse_Up.ogg" );
					SelectRecipe( pButton->GetDummyInt(0) );
				}

			}
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

		}
		return true;
	case UMCM_MAKE:
		{
			if( CheckManufacture() == true )
			{
				if( m_pDLGMakeQuery == NULL )
				{
					m_pDLGMakeQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5018), UMCM_MAKE_OK, g_pMain->GetNowState() );
					if( m_pDLGMakeQuery != NULL )
					{
						CKTDGUIButton* pCancelButton = (CKTDGUIButton*)m_pDLGMakeQuery->GetControl( L"MsgBoxOkAndCancelCancelButton" );		
						pCancelButton->RequestFocus();
					}
				}	
			}
			else
			{
#ifdef SERV_MANUFACTURE_FIX
				if(CheckCanMake(m_SelectedRecipeID, m_nQuantity))
				{
					// 지헌 : 수량성이면 개수 정한 후에 제조 패킷을 날리도록 하자
					if(CheckIsQuantity(m_SelectedRecipeID))
					{
						OpenRegisterQuantityDLG();
					}
					else
						return Handler_EGS_ITEM_MANUFACTURE_REQ();
				}
				else
					return false;
#else
				return Handler_EGS_ITEM_MANUFACTURE_REQ();
#endif //SERV_MANUFACTURE_FIX
			}			
		}
		return true;
	case UMCM_RESULT_CLOSE:
		{
			if ( m_pDLGManufactureResult != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGManufactureResult, NULL, false );

			m_pDLGManufactureResult = NULL;            
		}
		return true;
	case UMCM_MAKE_OK:
		{
			if( m_pDLGMakeQuery != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMakeQuery, NULL, false );
				m_pDLGMakeQuery = NULL;
			}			
#ifdef SERV_MANUFACTURE_FIX
			if(CheckCanMake(m_SelectedRecipeID, m_nQuantity))
			{
				// 지헌 : 수량성이면 개수 정한 후에 제조 패킷을 날리도록 하자
				if(CheckIsQuantity(m_SelectedRecipeID))
				{
					OpenRegisterQuantityDLG();
				}
				else
					return Handler_EGS_ITEM_MANUFACTURE_REQ();
			}
			else
				return false;
#else
			return Handler_EGS_ITEM_MANUFACTURE_REQ();
#endif //SERV_MANUFACTURE_FIX
		}
		break;	
#ifdef SERV_MANUFACTURE_FIX
	case UMCM_REGISTER_QUANTITY_PLUS:
		{
			if(CheckCanMake(m_SelectedRecipeID, m_nQuantity + 1))
			{
				m_nQuantity++;
				UpdateRegisterQuantityDLG(false);
				ResetRecipeMaterial();
			}
			return true;
		}break;
	case UMCM_REGISTER_QUANTITY_MINUS:
		{
			if(m_nQuantity > 0)
			{
				if(CheckCanMake(m_SelectedRecipeID, m_nQuantity - 1))
				{
					m_nQuantity--;
					UpdateRegisterQuantityDLG(false);
					ResetRecipeMaterial();
				}
			}
			return true;
		}break;
	case UMCM_REGISTER_QUANTITY_UPDATE:
		{
			if(m_nQuantity > 0)
			{
				if(CheckCanMake(m_SelectedRecipeID, m_nQuantity))
				{
					UpdateRegisterQuantityDLG(true);
					ResetRecipeMaterial();
				}
			}
			return true;
		}break;
	case UMCM_REGISTER_QUANTITY_CONFIRM:
		{
			// 확인 하면 업데이트를 한번 더 해준다.

			UpdateRegisterQuantityDLG(true);
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );

			m_pDLGItemEnroll = NULL;
			m_EnrollItemUid = 0;

			Handler_EGS_ITEM_MANUFACTURE_REQ();

			return true;
		}break;
	case UMCM_REGISTER_QUANTITY_CANCEL:
		{
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );

			m_pDLGItemEnroll = NULL;
			m_nQuantity = 1;		// 수량성 아이템을 버리는 Dialog Text 1로 고정
			m_EnrollItemUid = 0;
			//m_bRegisteredTrade = false;

			ResetRecipeMaterial();

			//UnRegisterItem();

			return true;
		}break;
#endif //SERV_MANUFACTURE_FIX
	default:
		break;
	}

	return false;
}

bool CX2UIManufacture:: UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_ITEM_MANUFACTURE_ACK:
		{
			return Handler_EGS_ITEM_MANUFACTURE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	}
	return false;
}


void CX2UIManufacture::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);
	if(m_pDLGManufacture != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGManufacture, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGManufacture, false );
	}
}

void CX2UIManufacture::SetPosition(D3DXVECTOR2 vec)
{
	// 슬롯과 다이얼로그 위치를 조절해 준다
	
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		D3DXVECTOR2 pos;
		pos = pItemSlot->GetPos();
		
		pos -= m_MovedPosition;
		pos += vec;

		pItemSlot->SetPos(pos);
	}
	m_MovedPosition = vec;
}

void CX2UIManufacture::SetShow(bool val)
{
	SAFE_DELETE_DIALOG(m_pDLGManufactureResult);
	m_pDLGManufactureResult = NULL;
    
	if(val)	// 켜질 때 처리해야 할 부분
	{
		RegisterLuaBind();

		if(m_pDLGManufacture == NULL)
		{
			m_pDLGManufacture = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_Alchemy_List.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGManufacture );
		}
		m_pDLGManufacture->SetShowEnable(true, true);
        
		// 체크박스를 슥삭
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGManufacture->GetControl( L"CheckBox_ShowAll" );
		if ( pCheckBox != NULL )
			pCheckBox->SetCheckedPure( m_bShowAllCharItem );

		//{{ kimhc // 2009-12-10 // 라디오 버튼 모두 false로 초기화
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
		for ( int i = 0; i < static_cast<int>( MT_END ); i++ )
		{
			CKTDGUIRadioButton* pRadioButton = GetRadioButtonByType( MANUFACTURE_TYPE(i) );

			if ( pRadioButton != NULL )
				pRadioButton->SetShow( false );
		}
#else	ADD_HOUSE_ID_TO_MANUFACTURE
		// 라디오버튼 눌러놓고		
		m_NowItemCategory = MT_WEAPON;
		CKTDGUIRadioButton* pRButtionWeapon = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Weapon" );
		pRButtionWeapon->SetChecked(true);
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		// kimhc // 2009-12-10 // 라디오 버튼 모두 false로 초기화

		// 제조법 슬롯 설정 초기화
		const int SlotperPage = GetSlotperPage();
		for(UINT i = 0; i<m_vecRecipeSlot.size(); i++)
		{
			ManufactureSlot* pBuySlot = m_vecRecipeSlot[i];
			SAFE_DELETE(pBuySlot);
		}
		m_vecRecipeSlot.clear();

		// 새로 만들어 주자
		for(int i = 0; i<SlotperPage; i++)
		{
			WCHAR buff[256] = {0,};
			ManufactureSlot* pRecipeSlot = new ManufactureSlot();
			//wsprintf( buff, L"Static_Recipe%d", i);
			StringCchPrintf(buff, 256, L"Static_Recipe%d", i);
			pRecipeSlot->m_pStatic = (CKTDGUIStatic*) m_pDLGManufacture->GetControl( buff );
			//wsprintf( buff, L"Button_Recipe%d", i);
			StringCchPrintf(buff, 256, L"Button_Recipe%d", i);
			pRecipeSlot->m_pButton = (CKTDGUIButton*) m_pDLGManufacture->GetControl( buff );


			m_vecRecipeSlot.push_back(pRecipeSlot);
		}

		// 리스트 받아오고 세팅하고.. 등등을 처리하자
		GetManufactureList();

		//{{ kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
		// 켜자
		int MAGIC_CONTROL_WIDTH = 0;
		bool bFlag = false;
		for ( int i = 0; i < static_cast<int>( MT_END ); i++ )
		{
			if ( m_vecManufactureIDList[i].empty() == false )
			{
				CKTDGUIRadioButton* pRadioButton = GetRadioButtonByType( MANUFACTURE_TYPE(i) );

				if ( pRadioButton != NULL )
				{
					if ( false == bFlag )
					{
						m_NowItemCategory = MANUFACTURE_TYPE(i);						 
						pRadioButton->SetChecked( true );
						bFlag = true;
					}
					pRadioButton->SetOffsetPos( D3DXVECTOR2( (float)MAGIC_CONTROL_WIDTH, 0 ) );
					pRadioButton->SetShow( true );
					pRadioButton->SetEnable( true );
					MAGIC_CONTROL_WIDTH += pRadioButton->GetDummyInt(0);		// Width를 LUA에 AddDummyInt로 만들어 넣어 둘 것!
				}
			}
		}

		ChangeManufactureTab( 1 );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		//}} kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가

		m_bShow = true;
		D3DXVECTOR3 tmp;
		tmp = m_pDLGManufacture->GetDummyPos(0);
		m_DLGPosition.x = tmp.x;
		m_DLGPosition.y = tmp.y;
		tmp = m_pDLGManufacture->GetDummyPos(1);
		m_DLGSize.x = tmp.x;
		m_DLGSize.y = tmp.y;
		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_MANUFACTURE);

	}
	else	// 꺼질 때 처리해야 할 부분
	{

		for( UINT iS = 0; iS < m_SlotList.size(); iS++ )
		{
			CX2SlotItem* pkSlotItem = (CX2SlotItem*)m_SlotList[iS];
			if ( pkSlotItem != NULL )
			{
				pkSlotItem->DestroyItemUI();
				pkSlotItem->SetShow(false);
				pkSlotItem->SetEnable(false);
			}
		}

		// 슬롯 마우스오버 이미지를 지워주고
		m_pDLGSelectedItem->SetShow( false );
		InvalidSlotDesc();

		m_bShow = false;
		m_pDLGManufacture->SetShowEnable(false, false);
		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_MANUFACTURE);
	}

	//g_pKTDXApp->SkipFrame();
}

void CX2UIManufacture::GetManufactureList()
{
	m_SelectedRecipeID = -1;
	for ( int i = 0; i < MT_END; i++ )
	{
		m_vecManufactureIDList[i].resize(0);
	}

	// 리스트를 적절히 받고 정렬해 봅시다
	vector<int> vecItemList;
	//{{ kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
	g_pData->GetItemManager()->GetManufactureVillageData( m_iNowOpenHouseID, vecItemList );
#else	ADD_HOUSE_ID_TO_MANUFACTURE
	g_pData->GetItemManager()->GetManufactureVillageData( g_pData->GetLocationManager()->GetCurrentVillageID(), vecItemList );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
	//}} kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
	
	for( UINT i=0; i<vecItemList.size(); i++ )
	{
		int iManufactureID = vecItemList[i];
		CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( iManufactureID );
		
		if ( pManufactureData != NULL )
		{
			// 열심히 테스트했으니 아이템을 카데고리 잘 섞어서 집어넣어 줍시다
			// 일단 사용가능한 물건인지 확인
#ifdef MANUFACTURE_FROM_RESULTGROUP_ID
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pManufactureData->m_ResultGroupID );
#else
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iManufactureID );
#endif MANUFACTURE_FROM_RESULTGROUP_ID
			if ( pItemTemplet != NULL )
			{
				
				if ( m_bShowAllCharItem == false )
				{
					// 내가 쓸 수 있는 물건이 아니면 넘어가자
					if ( false == IsPossibleUsedByMyCharacter(pItemTemplet) )
					{
						//{{ 허상형 : [2009/7/29] //	착용불능 장비일 경우 화면에 표시해준다.(헤니르의 시공 관련 수정)
						if( true == pItemTemplet->GetNoEquip() != true )
						{
							continue;
						}						
						//}} 허상형 : [2009/7/29] //	착용불능 장비일 경우 화면에 표시해준다.(헤니르의 시공 관련 수정)
					}
				}
			}	// pItemTemplet != NULL

			// 카데고리에 따라서
#ifdef MANUFACTURE_FROM_RESULTGROUP_ID
			switch ( GetManufactureTypeByItemID( pManufactureData->m_ResultGroupID ) )
#else
			switch ( GetManufactureTypeByItemID( iManufactureID ) )
#endif MANUFACTURE_FROM_RESULTGROUP_ID
			{
			case MT_NONE:
				m_vecManufactureIDList[MT_NONE].push_back(iManufactureID);
				break;
			case MT_WEAPON:
				m_vecManufactureIDList[MT_WEAPON].push_back(iManufactureID);
				break;
			case MT_DEFENCE_BODY:
				m_vecManufactureIDList[MT_DEFENCE_BODY].push_back(iManufactureID);
				break;
			case MT_DEFENCE_LEG:
				m_vecManufactureIDList[MT_DEFENCE_LEG].push_back(iManufactureID);
				break;
			case MT_DEFENCE_HAND:
				m_vecManufactureIDList[MT_DEFENCE_HAND].push_back(iManufactureID);
				break;
			case MT_DEFENCE_FOOT:
				m_vecManufactureIDList[MT_DEFENCE_FOOT].push_back(iManufactureID);
				break;
			case MT_QUICK_SLOT:
				m_vecManufactureIDList[MT_QUICK_SLOT].push_back(iManufactureID);
				break;
			case MT_ETC:
				m_vecManufactureIDList[MT_ETC].push_back(iManufactureID);
				break;
			default:
				break;
			}
		}

	}// for

	//{{ kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가
#ifndef	ADD_HOUSE_ID_TO_MANUFACTURE
	ChangeManufactureTab( 1 );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
	//}} kimhc // 2009-12-08 // 판매 물품에 따라 탭 활성화하는 기능 추가
	
}

void CX2UIManufacture::ChangeManufactureTab( int nPage )
{
	// 슬롯 갯수에 따른 max page 설정
	const int MAGIC_MAX_NUM_ITEM_SLOT = GetSlotperPage();
	m_nNowPage = nPage;
	m_nMaxPage = (int)(m_vecManufactureIDList[m_NowItemCategory].size()+MAGIC_MAX_NUM_ITEM_SLOT-1)/MAGIC_MAX_NUM_ITEM_SLOT;
	m_nMaxPage = max(1, m_nMaxPage);
	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*) m_pDLGManufacture->GetControl( L"Static_PageNumber" );
	wstringstream wstrmPageNum;
	wstrmPageNum << m_nNowPage << L"/" << m_nMaxPage;
	pStaticPageNum->GetString(0)->msg = wstrmPageNum.str();

	// 전체 물건 인덱스
	int iStartIndex = (m_nNowPage-1) * MAGIC_MAX_NUM_ITEM_SLOT;
	int iEndIndex = min( m_nNowPage * MAGIC_MAX_NUM_ITEM_SLOT, (int)m_vecManufactureIDList[m_NowItemCategory].size() );

	// 슬롯 데이터 전부 초기화
	for( UINT iS = 0; iS < m_SlotList.size(); iS++ )
	{
		CX2SlotItem* pkSlotItem = (CX2SlotItem*)m_SlotList[iS];
		if ( pkSlotItem != NULL )
		{
			pkSlotItem->DestroyItemUI();
			pkSlotItem->SetShow(false);
			pkSlotItem->SetEnable(false);
		}
	}

	// 끄고
	for( unsigned int iS = 0; iS < m_vecRecipeSlot.size(); ++iS )
	{
		ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[iS];		
		if ( pRecipeSlot->m_pStatic != NULL )
		{
			pRecipeSlot->m_pStatic->SetShow(false);
			pRecipeSlot->m_pButton->SetShow(false);
		}
	}

	// Slot Index
	int iSlotIndex = 0;
	// 슬롯 데이터 셋팅
	for( int iS = iStartIndex; iS < iEndIndex; iS++ )
	{
		int iManufactureID = m_vecManufactureIDList[m_NowItemCategory][iS];
		
//		CX2SlotItem* pkSlotItem = (CX2SlotItem*) m_SlotList[iSlotIndex];
		ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[iSlotIndex];		
		CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( iManufactureID );

		//////////////////////////////////////////////////////////////////////////
		
		if ( pRecipeSlot != NULL && pManufactureData != NULL )
		{
			pRecipeSlot->m_iManufactureID = iManufactureID;
#ifdef MANUFACTURE_FROM_RESULTGROUP_ID
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pManufactureData->m_ResultGroupID );
#else
            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iManufactureID );
#endif MANUFACTURE_FROM_RESULTGROUP_ID
			if ( pItemTemplet != NULL )
			{
				wstring itemName = L"";

				bool bCheck = false;
				if ( CheckIsThereAllMaterial( iManufactureID ) == false )
					bCheck = true;

				if ( CheckIsEnoughED( iManufactureID ) == false )
					bCheck = true;

				if ( bCheck == true )
					itemName += L"#CFF0000";

                itemName += pItemTemplet->GetFullName_();

#ifdef CLIENT_GLOBAL_LINEBREAK //각 국가에 맞게 MAGIC_TEXT_WIDTH 수정 
#ifdef COUNTRY_WORDWRAP_WIDTH_ID 
				const int MAGIC_TEXT_WIDTH = 135;	
				bool bEllipse = false;
				itemName = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse( itemName.c_str(),(int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), pRecipeSlot->m_pStatic->GetString(0)->fontIndex, 1, bEllipse );
#else //COUNTRY_WORDWRAP_WIDTH_ID
#ifdef COUNTRY_WORDWRAP_WIDTH_US
				const int MAGIC_TEXT_WIDTH = 110;
#else //COUNTRY_WORDWRAP_WIDTH_US
				const int MAGIC_TEXT_WIDTH = 160;
#endif //COUNTRY_WORDWRAP_WIDTH_US
				const CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pRecipeSlot->m_pStatic->GetString(0)->fontIndex );	
				CWordLineHandler::LineBreakInX2Main( itemName, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true );
#endif //COUNTRY_WORDWRAP_WIDTH_ID
				
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
				const int MAGIC_TEXT_WIDTH = 110;
				const CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pRecipeSlot->m_pStatic->GetString(0)->fontIndex );
				CX2Main::LineBreak( itemName, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true );
#endif //CLIENT_GLOBAL_LINEBREAK

				pRecipeSlot->m_pStatic->GetString(0)->msg = itemName;
				pRecipeSlot->m_pStatic->SetShow(true);
				pRecipeSlot->m_pButton->SetShow(true);

#ifdef SERV_MANUFACTURE_PERIOD_FIX
				std::map< int, int >::iterator mit1;
				mit1 = m_mapPeriodGroup.find( iManufactureID );
				if( mit1 != m_mapPeriodGroup.end() )
				{
					wstringstream wstreamDesc;
					//제조결과템이 여러 종류일때 '기간제 포함' 이라고만 남긴다.
					if( mit1->second != -1)
					{
						wchar_t wszNumber[32];
						::_itow( mit1->second, wszNumber, 10 );
						wstreamDesc << " ( " << wszNumber << GET_STRING( STR_ID_14 ) << " ) ";
						if( pRecipeSlot->m_pStatic->GetString(1)  != NULL)
							pRecipeSlot->m_pStatic->GetString(1)->msg = wstreamDesc.str().c_str();
					}
					else
					{
						wstreamDesc << GET_STRING( STR_ID_24532 );
						if( pRecipeSlot->m_pStatic->GetString(1)  != NULL)
							pRecipeSlot->m_pStatic->GetString(1)->msg = wstreamDesc.str().c_str();
					}

				}
#endif SERV_MANUFACTURE_PERIOD_FIX

				for ( int slotListIter = 0; slotListIter < (int)m_SlotList.size(); slotListIter++ )
				{
					CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[slotListIter];
					if ( pSlotItem != NULL )
					{
						if ( pSlotItem->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW && pSlotItem->GetSlotID() == iSlotIndex )
						{
							pSlotItem->CreateItemUI( pItemTemplet );
							pSlotItem->SetShow(true);
							pSlotItem->SetEnable(true);
							AttachSelectedPicture(pSlotItem);
						}
					}
				}
			}
			else
			{
				pRecipeSlot->m_pStatic->GetString(0)->msg = L"";
			}
			
			// 슬롯 상태 노멀로 만들자 (선택을 취소)
			pRecipeSlot->m_pButton->SetDownStateAtNormal( false );

		}

		// 수수료 0으로 만들고
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGManufacture->GetControl( L"Static_Price" );
		if ( pStatic != NULL && pStatic->GetString(0) != NULL )
		{
			pStatic->GetString(0)->msg = L"0";
		}

		// 버튼을 꺼주고
		CKTDGUIControl* pControl = m_pDLGManufacture->GetControl( L"Button_Make" );
		if ( pControl != NULL )
			pControl->SetShowEnable( false, false );

		//////////////////////////////////////////////////////////////////////////

		iSlotIndex++;
	}
}



void CX2UIManufacture::SelectRecipe( int iSlotNum )
{

	// 슬롯 선택 방식을 안 쓰므로 여긴 빼고
// 	CX2UIManufacture::ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[iSlotNum];
// 	if ( pRecipeSlot != NULL )
// 	{
// 		if ( pRecipeSlot->m_iManufactureID != -1 )
// 			m_SelectedRecipeID = pRecipeSlot->m_iManufactureID;
// 	}
// 
// 	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
// 	{
// 		CX2SlotItem* pSlot = (CX2SlotItem*) m_SlotList[i];
// 		if ( pSlot != NULL && pSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW )
// 		{
// 			if ( pSlot->GetSlotID() == iSlotNum )
// 			{
// 				//** 슬롯 체크를 해주자
// 				SetShowSelectedPicture(pSlot, true);
// 				
// 			}
// 			else
// 			{
// 				//** 체크 표시를 지워주자
// 				SetShowSelectedPicture(pSlot, false);
// 				
// 			}
// 		}
// 	}
	// 버튼 선택 방식으로 변경
	for( int i = 0; i < (int)m_vecRecipeSlot.size(); i++)
	{
		CX2UIManufacture::ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[i];
		
		if( pRecipeSlot->m_pButton != NULL)
		{
			if( i == iSlotNum )
			{
				if ( pRecipeSlot->m_iManufactureID != -1 )
				{
					m_SelectedRecipeID = pRecipeSlot->m_iManufactureID;
					pRecipeSlot->m_pButton->SetDownStateAtNormal(true);
				}

			}
			else
			{
				pRecipeSlot->m_pButton->SetDownStateAtNormal(false);

			}
		}
		

	}

	if ( m_pDLGManufacture != NULL )
	{
		if ( CheckIsThereAllMaterial( m_SelectedRecipeID ) == false || 
			CheckIsEnoughED( m_SelectedRecipeID ) == false )
		{
			// 버튼을 꺼버리자
			CKTDGUIControl* pControl = m_pDLGManufacture->GetControl( L"Button_Make" );
			if ( pControl != NULL )
				pControl->SetShowEnable( false, false );
		}
		else
		{	
			// 버튼을 켜 주자
			CKTDGUIControl* pControl = m_pDLGManufacture->GetControl( L"Button_Make" );
			if ( pControl != NULL )
				pControl->SetShowEnable( true, true );
		}
	}

	ResetRecipeMaterial();
}

// 선택한 레시피의 제조 수수료, 제조재료...
void CX2UIManufacture::ResetRecipeMaterial()
{
	//선택된 레시피의 수수료, 이름, 설명에 대한 정보 UI 셋팅
	int recipeFee = 0;
	wstring recipeName = L"";
	wstring recipeDesc = L"";

	//실제 유효한 레시피가 선택된것인가?
	if ( m_SelectedRecipeID != -1 )
	{
		CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_SelectedRecipeID );
		if ( pManufactureData != NULL )
		{
			recipeFee = pManufactureData->m_Cost;
		}
	}

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGManufacture->GetControl( L"Static_Price" );
	if ( pStatic != NULL && pStatic->GetString(0) != NULL )
	{
		pStatic->GetString(0)->msg = g_pMain->GetEDString( recipeFee ).c_str();
	}

	// 재료를 세팅해 주자
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem != NULL )
		{
			if ( pSlotItem->GetSlotType() == CX2Slot::ST_MATERIAL )
			{
				pSlotItem->DestroyItemUI();
				pSlotItem->SetShow(false);
				pSlotItem->SetEnable(false);
			}
		}
	}

	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_SelectedRecipeID );
	if ( pManufactureData != NULL )
	{
		for ( int i = 0; i < (int)pManufactureData->m_vecMaterials.size(); i++ )
		{
			CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[i];
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( materialData.m_MaterialItemID );
			if ( pItemTemplet != NULL )
			{
				int itemQuantity = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );

				for ( int j = 0; j < (int)m_SlotList.size(); j++ )
				{
					CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[j];
					if ( pSlotItem != NULL && pSlotItem->GetSlotType() == CX2Slot::ST_MATERIAL &&
						pSlotItem->GetSlotID() == i )
					{
#ifdef SERV_MANUFACTURE_FIX
						pSlotItem->CreateMaterialItemUI( pItemTemplet, itemQuantity, materialData.m_MaterialCount * m_nQuantity);
#else
						pSlotItem->CreateMaterialItemUI( pItemTemplet, itemQuantity, materialData.m_MaterialCount);
#endif
						pSlotItem->SetShow(true);
						pSlotItem->SetEnable(true);
						break;
					}
				}
			}
		}
	}

}


bool CX2UIManufacture::Handler_EGS_ITEM_MANUFACTURE_REQ()
{
#ifdef SERV_MANUFACTURE_FIX
	if ( CheckIsThereAllMaterial( m_SelectedRecipeID, m_nQuantity ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_836 ), g_pMain->GetNowState() );
		return false;
	}

	// 순수하게 재료가 부족한(아예 없는) 경우는 위에서 미리 체크한다. 여기서는 장착중인지만 체크.
	// 오류 메시지를 다르게 하기 때문에 이렇게 처리.
	if ( CheckIsThereAllMaterialNoEquipped( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2565 ), g_pMain->GetNowState() );
		return false;
	}

	if ( CheckIsEnoughED( m_SelectedRecipeID, m_nQuantity ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_837 ), g_pMain->GetNowState() );
		return false;
	}

	KEGS_ITEM_MANUFACTURE_REQ kPacket;
	kPacket.m_iManufactureID = m_SelectedRecipeID;
	kPacket.m_iQuantity = m_nQuantity;
	m_nQuantity = 1;
#else
	if ( CheckIsThereAllMaterial( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_836 ), g_pMain->GetNowState() );
		return false;
	}

	// 순수하게 재료가 부족한(아예 없는) 경우는 위에서 미리 체크한다. 여기서는 장착중인지만 체크.
	// 오류 메시지를 다르게 하기 때문에 이렇게 처리.
	if ( CheckIsThereAllMaterialNoEquipped( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2565 ), g_pMain->GetNowState() );
		return false;
	}

	if ( CheckIsEnoughED( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_837 ), g_pMain->GetNowState() );
		return false;
	}

	KEGS_ITEM_MANUFACTURE_REQ kPacket;
	kPacket.m_iManufactureID = m_SelectedRecipeID;
#endif //SERV_MANUFACTURE_FIX

	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_MANUFACTURE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ITEM_MANUFACTURE_ACK );

	m_bWaitForManufactureResult = true;

	return true;
}


bool CX2UIManufacture::Handler_EGS_ITEM_MANUFACTURE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ITEM_MANUFACTURE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bWaitForManufactureResult = false;

	if( g_pMain->DeleteServerPacket( EGS_ITEM_MANUFACTURE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			wstring itemName = L"";

			if ( kEvent.m_mapInsertedItem.empty() == false )
			{

				std::map< int, int >::iterator i = kEvent.m_mapInsertedItem.begin();
				int resultItemID = i->first;

				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
				if ( pItemTemplet != NULL )
				{
                    itemName = pItemTemplet->GetFullName_();

					SAFE_DELETE_DIALOG( m_pDLGManufactureResult );
					m_pDLGManufactureResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_ManufactureUI_Result_Success.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGManufactureResult );

					CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGManufactureResult->GetControl( L"StaticAlchemy_Success_Window_Item_Slot" );
					if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
					{
                        const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
						if ( pwszShopImage[0] != NULL )
							pStatic->GetPicture(0)->SetTex( pwszShopImage );
						else
							pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
					}

#ifdef SERV_MANUFACTURE_FIX
					int iTextLenOffset = 0;// 수량 표시 공간
					CKTDGUIStatic* pStaticQuantity = (CKTDGUIStatic*)m_pDLGManufactureResult->GetControl( L"Success_Item_Quantity" );
					if( NULL != pStaticQuantity )
					{
						if( i->second >= 2 ) 
						{
							WCHAR wszQuantity[64];
							StringCchPrintfW( wszQuantity, ARRAY_SIZE(wszQuantity) , L"#CFF0000X%d", i->second );
							pStaticQuantity->GetString(0)->msg = wszQuantity;
							iTextLenOffset = -25; // 수량 표시 공간
						}
						else
						{
							pStaticQuantity->GetString(0)->msg = L"";
						}
					}
#endif //SERV_MANUFACTURE_FIX

					pStatic = (CKTDGUIStatic*)m_pDLGManufactureResult->GetControl( L"StaticAlchemy_Success_Item_Name" );

#ifdef SERV_MANUFACTURE_FIX
					const int constTextMaxLen = (int)(g_pKTDXApp->GetResolutionScaleX() * (250 + iTextLenOffset));
#else
					const int constTextMaxLen = (int)(g_pKTDXApp->GetResolutionScaleX() * 250);
#endif //SERV_MANUFACTURE_FIX					
					CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic->GetString(0)->fontIndex );
#ifdef CLIENT_GLOBAL_LINEBREAK
					int lineNum = CWordLineHandler::LineBreakInX2MainMsgBox( itemName, pFont, constTextMaxLen );
#else //CLIENT_GLOBAL_LINEBREAK
					int nowTextLen = 0;
					int lineNum = 1;

					for ( int i = 0; i < (int)itemName.size(); i++ )
					{
						WCHAR tempWchar = itemName[i];
						if ( nowTextLen < constTextMaxLen && tempWchar == '\n' )
						{
							if ( i != (int)itemName.size() - 1 )
							{
								lineNum++;
							}

							nowTextLen = 0;
							continue;
						}
						int iUniCharSize = 0;
						iUniCharSize = pFont->GetWidth( tempWchar );
						nowTextLen += iUniCharSize;

						if ( nowTextLen >= constTextMaxLen )
						{
							if ( i != (int)itemName.size() - 1 )
							{
								lineNum++;
							}

							nowTextLen = 0;
							wstring enterChar = L"\n";
							itemName.insert( itemName.begin() + i, enterChar.begin(), enterChar.end() );
							i++;
						}

						if ( i >= (int)itemName.size() )
						{
							break;
						}
					}
#endif //CLIENT_GLOBAL_LINEBREAK
					if ( lineNum == 2 )
					{
						pStatic->GetString(0)->pos.y -= 9.f;
					}
					
					if ( pStatic != NULL && pStatic->GetString(0) != NULL )
					{
						pStatic->GetString(0)->msg = itemName;
					}

// 					CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGManufactureResult->GetControl( L"ButtonCube_Window_Complete" );
// 					pButton->SetCustomMsgMouseUp(UMCM_RESULT_CLOSE);

				}
			}
			else
			{
				//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_838 ), m_p );

				SAFE_DELETE_DIALOG( m_pDLGManufactureResult );
				m_pDLGManufactureResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_ManufactureUI_Result_Fail.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGManufactureResult );
// 				CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGManufactureResult->GetControl( L"ButtonCube_Window_Complete" );
// 				pButton->SetCustomMsgMouseUp(UMCM_RESULT_CLOSE);

			}


			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
			
			if(g_pData->GetUIManager()->GetUIInventory() != NULL)
			{		
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
			}

			//			Reset();

			ResetRecipeMaterial();

			return true;
		}		
	}
	return false;
}



CX2UIManufacture::MANUFACTURE_TYPE CX2UIManufacture::GetManufactureTypeByItemID( int itemID )
{
	CX2UIManufacture::MANUFACTURE_TYPE manuType = CX2UIManufacture::MT_NONE;

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
	if ( pItemTemplet != NULL )
	{
		switch( pItemTemplet->GetItemType() ) 
		{
		case CX2Item::IT_WEAPON:
			{
				manuType = CX2UIManufacture::MT_WEAPON;
			}
			break;

		case CX2Item::IT_DEFENCE:
			{
				switch( pItemTemplet->GetEqipPosition() ) 
				{
				case CX2Unit::EP_DEFENCE_BODY:
					{
						manuType = CX2UIManufacture::MT_DEFENCE_BODY;
					}
					break;
				case CX2Unit::EP_DEFENCE_LEG:
					{
						manuType = CX2UIManufacture::MT_DEFENCE_LEG;
					}
					break;
				case CX2Unit::EP_DEFENCE_HAND:
					{
						manuType = CX2UIManufacture::MT_DEFENCE_HAND;
					}
					break;
				case CX2Unit::EP_DEFENCE_FOOT:
					{
						manuType = CX2UIManufacture::MT_DEFENCE_FOOT;
					}
					break;
				default:
					{
						manuType = CX2UIManufacture::MT_ETC;
					}
				}
			}
			break;

		case CX2Item::IT_QICK_SLOT:
			{
				manuType = CX2UIManufacture::MT_QUICK_SLOT;
			}
			break;

		default:
			{
				manuType = CX2UIManufacture::MT_ETC;
			}
		}
	}

	return manuType;
}


bool CX2UIManufacture::IsPossibleUsedByMyCharacter( 
    const CX2Item::ItemTemplet* pItemTemplet 
    )
{
	if ( pItemTemplet == NULL )
		return false;

	return CX2Unit::CanUse( 
        pItemTemplet->GetItemID(), 
        g_pData->GetMyUser()->GetSelectUnit() );
}

int CX2UIManufacture::GetSlotperPage()
{
	int num = 0;
	for(UINT i=0; i<m_SlotList.size(); i++)
	{
		CX2Slot* pSlot = m_SlotList[i];
		if(pSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW)
			num++;
	}

	return num;

}

#ifdef SERV_MANUFACTURE_FIX
bool CX2UIManufacture::CheckIsThereAllMaterial( int manufactureID, int nQuantity )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	for ( int j = 0; j < (int)pManufactureData->m_vecMaterials.size(); j++ )
	{
		CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[j];
		int nowCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );
		if ( nowCount < materialData.m_MaterialCount * nQuantity )
		{

			return false;
		}
	}

	return true;
}
#else
bool CX2UIManufacture::CheckIsThereAllMaterial( int manufactureID )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	for ( int j = 0; j < (int)pManufactureData->m_vecMaterials.size(); j++ )
	{
		CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[j];
		int nowCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );
		if ( nowCount < materialData.m_MaterialCount )
		{

			return false;
		}
	}

	return true;

}
#endif //SERV_MANUFACTURE_FIX

// 그냥 아이템 수가 부족한 경우는 CheckIsThereAllMaterial에서 체크.
// 여기서는 장비중인 아이템을 제외해서 수를 체크한다. CheckIsThereAllMaterial을 적용한 후 CheckMaterialIsEquipped을 사용함으로서
// 수가 모자란 경우를 검사한 후 장비중이라 안 되는 경우를 체크할 수 있다.
// 해당 아이템을 장비중이라도 같은 아이템이 인벤토리에 또 들어있는 경우에는 true를 리턴한다 : 인벤에 있는 아이템이 제조에 사용된다.
bool CX2UIManufacture::CheckIsThereAllMaterialNoEquipped( int manufactureID )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	for ( int j = 0; j < (int)pManufactureData->m_vecMaterials.size(); j++ )
	{
		CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[j];
//		int TotalCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );
		int NoEquipCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID, true );
		if ( NoEquipCount < materialData.m_MaterialCount )
		{
			return false;
		}
	}

	return true;


}

#ifdef SERV_MANUFACTURE_FIX
bool CX2UIManufacture::CheckIsEnoughED( int manufactureID, int nQuantity )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	if ( pManufactureData != NULL )
	{
		int recipeFee = pManufactureData->m_Cost * nQuantity;
		if ( recipeFee > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED)
		{
			return false;
		}
	}

	return true;

}
#else
bool CX2UIManufacture::CheckIsEnoughED( int manufactureID )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	if ( pManufactureData != NULL )
	{
		int recipeFee = pManufactureData->m_Cost;
		if ( recipeFee > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
		{
			return false;
		}
	}

	return true;

}
#endif //SERV_MANUFACTURE_FIX

wstring CX2UIManufacture::GetSlotItemDesc()
{
	if( NULL == m_pNowOverItemSlot )
		return L"";


	WCHAR buff[256] = L"";
	wstring desc = L"";

	if ( m_pNowOverItemSlot->IsResetItemUI() == false )
	{
		desc = m_pNowOverItemSlot->GetSlotDesc();
	}
	else
	{	
		switch( m_pNowOverItemSlot->GetSlotType() )
		{
		case CX2Slot::ST_MATERIAL:
			{
				if( m_pNowOverItemSlot->GetItemUID() != -1 )
				{
					desc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
				}
				else
				{
					desc = GetSlotItemDescByTID( NULL, m_pNowOverItemSlot->GetItemTID(), false );
				}
			} break;

		default:
		case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
			{
				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
				if( NULL != pItemTemplet )
				{
                    desc = pItemTemplet->GetFullName_();
					desc += L"\n\n";

					switch( pItemTemplet->GetUseCondition() )
					{
					default:
					case CX2Item::UC_ANYONE:
						{
							ZeroMemory( buff, sizeof(buff) );
						} break;

					case CX2Item::UC_ONE_UNIT:
						{
							switch( pItemTemplet->GetUnitType() )
							{
							case CX2Unit::UT_ELSWORD:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_614 ) ) );
								} break;

							case CX2Unit::UT_ARME:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_615 ) ) );
								} break;


							case CX2Unit::UT_LIRE:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_616 ) ) );
								} break;


							case CX2Unit::UT_RAVEN:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_617 ) ) );
								} break;


							case CX2Unit::UT_EVE:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_618 ) ) );
								} break;

						//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
					#ifdef	NEW_CHARACTER_CHUNG
							case CX2Unit::UT_CHUNG:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_9739 ) ) );
								} break;
					#endif	NEW_CHARACTER_CHUNG
						//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

					#ifdef ARA_CHARACTER_BASE
							case CX2Unit::UT_ARA:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_21181 ) ) );
								} break;
					#endif
					#ifdef NEW_CHARACTER_EL
							case CX2Unit::UT_ELESIS:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_25873 ) ) );
								} break;
					#endif // NEW_CHARACTER_EL
					#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
							case CX2Unit::UT_ADD:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_29422 ) ) );
								} break;
					#endif //SERV_9TH_NEW_CHARACTER

							}
						} break;

					case CX2Item::UC_ONE_CLASS:
						{
							const CX2Unit::UnitTemplet* pUnitTemplet = g_pData->GetUnitManager()->GetUnitTemplet( pItemTemplet->GetUnitClass() );
							if( NULL != pUnitTemplet )
							{
								StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "S", pUnitTemplet->m_Description.c_str() ) ) );
							}
						} break;
					}

					if( false == CX2Unit::CanUse( m_pNowOverItemSlot->GetItemTID(), g_pData->GetMyUser()->GetSelectUnit() ) )
					{
						desc += L"#CFF0000";
						desc += buff;
						desc += L"#CX";
					}
					else
					{
						desc += buff;
					}

					StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_840, "i", pItemTemplet->GetUseLevel() ) ) );
					desc += buff;


					desc += L"[";
                    desc += GET_STRING( STR_ID_841 );
                    desc += L"]\n\n";
				}

				int iSlotID = m_pNowOverItemSlot->GetSlotID() + GetSlotperPage() * (m_nNowPage-1);
				if( iSlotID >= 0 && iSlotID < (int)m_vecManufactureIDList[m_NowItemCategory].size() )
				{
					CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_vecManufactureIDList[m_NowItemCategory][iSlotID] );
					if( NULL != pManufactureData )
					{
						const CX2ItemManager::ManufactureResultGroupData* pResultGroupData = 
							g_pData->GetItemManager()->GetManufactureResultGroupData( pManufactureData->m_ResultGroupID );

						if ( NULL != pResultGroupData )
						{

							// 제조 결과 그룹에 아이템이 1개 밖에 없으면 아이템 설명을 
							if( pResultGroupData->m_vecResultGroupItemID.size() == 1 )
							{
								if( m_pNowOverItemSlot->GetItemUID() != -1 )
								{
									desc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
								}
								else
								{
									desc = GetSlotItemDescByTID( NULL, m_pNowOverItemSlot->GetItemTID(), false );
								}
							}
							else // 여러개 있다면 아이템 리스트를 보여준다
							{
								wstring wstrItems;
								for( UINT i=0; i<pResultGroupData->m_vecResultGroupItemID.size(); i++ )
								{
									const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pResultGroupData->m_vecResultGroupItemID[i] );
									if( NULL != pItemTemplet )
									{

										wstrItems += pItemTemplet->GetNameColor_();
										wstrItems += pItemTemplet->GetName();
										wstrItems += L"#CX\n";
									}
								}

								desc += GET_REPLACED_STRING( ( STR_ID_842, "L", wstrItems ) );
							}
						}
					} // if( NULL != pManufactureData )
				}

			} break;
		}
	}

	return desc;
}

void CX2UIManufacture::AttachSelectedPicture( CX2SlotItem* pItemSlot )
{
	CKTDGUIDialogType pkDialog = pItemSlot->GetDialog();
	if ( pkDialog == NULL )
		return;

	CKTDGUIStatic* pkStatic = pkDialog->GetStatic_LUA( "StaticRoot_ItemSlot" );
	if ( pkStatic == NULL )
		return;

	CKTDGUIControl::CPictureData* pPictureEquiped = new CKTDGUIControl::CPictureData();
	pPictureEquiped->SetTex( L"HQ_ShopEqipedBorder.tga" );
	pPictureEquiped->SetPoint( new CKTDGUIControl::UIPointData );
	pPictureEquiped->SetPos( D3DXVECTOR2( 0, 0 ) );
	pPictureEquiped->SetSize( pItemSlot->GetSize() );
	pPictureEquiped->SetShow( false );
	pPictureEquiped->SetColor( D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f ));

	pkStatic->AddPictureAt( 2, pPictureEquiped );
}

void CX2UIManufacture::SetShowSelectedPicture( CX2SlotItem* pItemSlot, bool bShow )
{
	CKTDGUIDialogType pkDialog = pItemSlot->GetDialog();
	if ( pkDialog == NULL )
		return;

	CKTDGUIStatic* pkStatic = pkDialog->GetStatic_LUA( "StaticRoot_ItemSlot" );
	if ( pkStatic == NULL )
		return;

	CKTDGUIControl::CPictureData* pPictureEquiped = pkStatic->GetPicture(2);
	if ( pPictureEquiped == NULL )
		return;

	CKTDGUIControl::CPictureData* pPictureImpossible = pkStatic->GetPicture(1);
	if ( pPictureImpossible == NULL )
		return;

	pPictureEquiped->SetShow( bShow );
	// 적절한 처리 : 사용불가능 아이템 마크를 처리하자..
	if(false == bShow)
	{
		if ( g_pData->GetMyUser()->GetSelectUnit()->EqipAbility( pItemSlot->GetItemTID() ) == true )
		{
			pPictureImpossible->SetShow( false );	
		}
		else
		{
			pPictureImpossible->SetShow( true );
		}
	}
	else
	{
		pPictureImpossible->SetShow( false );
	}
}

//{{ kimhc // 2009-11-23 // 제조 UI 변경
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
CKTDGUIRadioButton* CX2UIManufacture::GetRadioButtonByType( MANUFACTURE_TYPE eType ) const		// 제조 타입별 버튼 포인터 얻어오기
{
	if( m_pDLGManufacture == NULL )
		return NULL;

	CKTDGUIRadioButton* pRButton = NULL;
	
	switch( eType )
	{
	case MT_WEAPON:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Weapon" );
		break;
	case MT_DEFENCE_BODY:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Coat" );
		break;
	case MT_DEFENCE_LEG:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Pants" );
		break;
	case MT_DEFENCE_HAND:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Glove" );
		break;
	case MT_DEFENCE_FOOT:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Shoes" );
		break;
	case MT_QUICK_SLOT:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_QuickSlot" );
		break;
	case MT_ETC:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Etc" );
		break;
	default:
		break;
	}

	return pRButton;
}
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
//}} kimhc // 2009-11-23 // 제조 UI 변경

bool CX2UIManufacture::CheckManufacture()
{	 
	if( m_SelectedRecipeID == 132094 ||
		m_SelectedRecipeID == 132095 ||
		m_SelectedRecipeID == 132096 ||
		m_SelectedRecipeID == 132097 ||
		m_SelectedRecipeID == 132098 ||
		m_SelectedRecipeID == 132099 ||
		m_SelectedRecipeID == 132100 ||
		m_SelectedRecipeID == 132101 ||
		m_SelectedRecipeID == 132102 ||
		m_SelectedRecipeID == 132103 ||
		m_SelectedRecipeID == 132104 ||
		m_SelectedRecipeID == 132105 ||
		m_SelectedRecipeID == 132106 ||
		m_SelectedRecipeID == 132107 )
	{
		return true;
	}

	return false;
}

#ifdef SERV_MANUFACTURE_FIX

bool CX2UIManufacture::CheckIsQuantity( int manufactureID )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}
	const CX2ItemManager::ManufactureResultGroupData* pManuResult = g_pData->GetItemManager()->GetManufactureResultGroupData(pManufactureData->m_ResultGroupID);
	if ( pManuResult == NULL )
	{
		return false;
	}

	// 제조 결과가 여러개. 즉 랜덤 제조는 수량성 제조에서 제외 시키자.
	if(pManuResult->m_vecResultGroupItemID.size() > 1)
		return false;

	if(g_pData->GetItemManager()->GetItemTemplet(pManuResult->m_vecResultGroupItemID[0])->GetPeriodType() == CX2Item::PT_QUANTITY)
		return true;

	return false;
}

bool CX2UIManufacture::CheckCanMake( int manufactureID, int nQuantity)
{
	if ( CheckIsThereAllMaterial( m_SelectedRecipeID, nQuantity ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_836 ), g_pMain->GetNowState() );
		return false;
	}

	// 순수하게 재료가 부족한(아예 없는) 경우는 위에서 미리 체크한다. 여기서는 장착중인지만 체크.
	// 오류 메시지를 다르게 하기 때문에 이렇게 처리.
	if ( CheckIsThereAllMaterialNoEquipped( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2565 ), g_pMain->GetNowState() );
		return false;
	}

	if ( CheckIsEnoughED( m_SelectedRecipeID, nQuantity ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_837 ), g_pMain->GetNowState() );
		return false;
	}

	//}}

	// 인벤에 빈공간이 있는지 여기서도 체크 하자.
	// 어디든 인벤 빈공간이 꽉 찬 곳이 있으면 제조 불가로 하자.
	/*
	int iSortType = CX2Inventory::ST_EQUIP;
	while( iSortType != CX2Inventory::ST_END )
	{
		if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().IsPossibleAddItem((CX2Inventory::SORT_TYPE)iSortType) == false )
			return false;
		iSortType++;
	}
	*/

	return true;
}

void CX2UIManufacture::OpenRegisterQuantityDLG()
{

	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);

	m_pDLGItemEnroll = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_An.lua" );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemEnroll );
	m_pDLGItemEnroll->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-865, 768-633), D3DXVECTOR2(53,62)) );

	CKTDGUIStatic* pStaticPrice			= static_cast < CKTDGUIStatic* >( m_pDLGItemEnroll->GetControl( L"Static_Price" ) );
	CKTDGUIStatic* pStaticEDMark		= static_cast < CKTDGUIStatic* >( m_pDLGItemEnroll->GetControl( L"Static_EDMark" ) );

	// 제조 가격
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_SelectedRecipeID );
	pStaticPrice->GetString(0)->msg = g_pMain->GetEDString( m_nQuantity * pManufactureData->m_Cost );

	CKTDGUIButton*		pButtonOK		= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_OK" ) );
	CKTDGUIButton*		pButtonCancel	= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Cancle" ) );
	CKTDGUIButton*		pButtonPlus		= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Plus" ) );
	CKTDGUIButton*		pButtonMinus	= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Minus" ) );
	CKTDGUIIMEEditBox*	pQuantity		= static_cast< CKTDGUIIMEEditBox* >( m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" ) );

	pQuantity->SetText( GET_REPLACED_STRING( ( STR_ID_2632, "i", m_nQuantity ) ), true);
	pButtonOK->SetCustomMsgMouseUp( UMCM_REGISTER_QUANTITY_CONFIRM );
	pButtonCancel->SetCustomMsgMouseUp( UMCM_REGISTER_QUANTITY_CANCEL );
	pButtonPlus->SetCustomMsgMouseUp( UMCM_REGISTER_QUANTITY_PLUS );
	pButtonMinus->SetCustomMsgMouseUp( UMCM_REGISTER_QUANTITY_MINUS );
	pQuantity->SetCustomMsgChange( UMCM_REGISTER_QUANTITY_UPDATE );
	pQuantity->SetCustomMsgEnter( UMCM_REGISTER_QUANTITY_CONFIRM );
	m_pDLGItemEnroll->SetCloseCustomUIEventID( UMCM_REGISTER_QUANTITY_CANCEL );
	pQuantity->RequestFocus();

}

void CX2UIManufacture::UpdateRegisterQuantityDLG( bool bReadIME )
{
	ASSERT( NULL != m_pDLGItemEnroll );
	if( NULL == m_pDLGItemEnroll )
		return; 

	CKTDGUIIMEEditBox* pQuantity = static_cast< CKTDGUIIMEEditBox* >( m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" ) );
	CKTDGUIStatic* pStaticPrice = static_cast<CKTDGUIStatic*>(m_pDLGItemEnroll->GetControl( L"Static_Price" ));
	

	if(bReadIME)
	{
		m_nQuantity =(int) wcstol( pQuantity->GetText(), NULL, 10 );
	}	

	int nMaxNum = GetMaxMakeNum(m_SelectedRecipeID);

	if ( m_nQuantity <= 0 )
	{
		if(bReadIME)				// 입력창으로 받아온 경우
			m_nQuantity = 1;
		else						// << 를 누른 경우 : 1 아래면 max로 순환시킨다.
			m_nQuantity = nMaxNum;	
	}

	if ( m_nQuantity >= nMaxNum )
		m_nQuantity = nMaxNum;

	WCHAR buff[256] = {0};
	_itow( m_nQuantity, buff, 10 );

	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str() );

	// 제조 가격
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_SelectedRecipeID );
	pStaticPrice->GetString(0)->msg = g_pMain->GetEDString( m_nQuantity * pManufactureData->m_Cost );
}

// 지헌 : 최대 제조량 구하기
int CX2UIManufacture::GetMaxMakeNum( int manufactureID)
{
	int nMaxNum = 9999;

	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	for ( int j = 0; j < (int)pManufactureData->m_vecMaterials.size(); j++ )
	{
		CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[j];
		int nowCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );
		if( nMaxNum > (nowCount / materialData.m_MaterialCount) )
			nMaxNum = (nowCount / materialData.m_MaterialCount);
	}

#ifdef MANUFACTURE_MINUS_CHECK_ELD //nMaxNum(제조가능 최대갯수)가 소유한 ED를 넘지 않도록 한다.
	bool CheckingEld = false;
	CheckingEld = CheckIsEnoughED(manufactureID, nMaxNum); //nMaxNum만큼 ED가 충분한지 체크한다.
	while(!CheckingEld)									   //CheckingEld가 false라면(ED가 충분하지 않다면) while문을 돌며 nMaxNum을 1씩 줄인다.
	{
		nMaxNum--;
		CheckingEld = CheckIsEnoughED(manufactureID, nMaxNum);
	}
#endif MANUFACTURE_MINUS_CHECK_ELD

	return nMaxNum;
}

#endif //SERV_MANUFACTURE_FIX

#endif
