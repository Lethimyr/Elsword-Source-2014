#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI
//{{ oasis907 : 김상윤 [2010.4.5] // 소켓 강화 UI 개편
#ifdef SERV_SOCKET_NEW

#include ".\X2UISocketItem.h"

CX2UISocketItem::CX2UISocketItem( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2UIDragable( pNowStage, NULL ),
m_bShow(false),
m_bFashion(false),
m_iSocketCount(0),
m_iTotalCostED(0),
m_ItemPrice(0),
m_MovedPosition(0,0),				// D3DXVECTOR2
m_DLGPosition(0,0),					// D3DXVECTOR2
m_DLGSize(0,0),						// D3DXVECTOR2
m_pDlgSocketItem(NULL),
#ifdef UI_SOCKET_NEW_CHEAT
m_pDlgSocketItemCheat(NULL),
#endif UI_SOCKET_NEW_CHEAT
m_Layer(XDL_NORMAL_4)
{	



	m_bShow = true;

	for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
	{
		m_hSocketSeq1[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
#ifdef UI_SOCKET_NEW_CHEAT
		m_iSocketOptionCode[i] = 0;
		m_pIMEEditSocketOptionCode[i] = NULL;
#endif UI_SOCKET_NEW_CHEAT
	}

	m_bMouseOverSoundCondition_1 = false;
	m_bMouseOverSoundCondition_2 = false;

	m_PickedSocketIndex = -1;
	m_SocketItemUID = -1;
}


CX2UISocketItem::~CX2UISocketItem(void)
{

	for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSocketSeq1[i] );
	}

	SAFE_DELETE_DIALOG(m_pDlgSocketItem);

}

void CX2UISocketItem::SetShow(bool bShow)
{
	m_bShow = bShow;
	if(bShow)
	{
		;
	}
	else // if bShow == false
	{
		SocketEquipReadyEffectEnd();
		m_pDlgSocketItem->SetShowEnable(false, false);
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
		g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
#ifdef UI_SOCKET_NEW_CHEAT
		m_pDlgSocketItemCheat->SetShowEnable(false, false);
#endif UI_SOCKET_NEW_CHEAT

#ifdef SERV_PET_SYSTEM
		// oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리에서 소켓 마법석 사용 
		if(g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory() != NULL)
		{
			g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->UpdateUI();
		}
#endif SERV_PET_SYSTEM
	}
	return;
}


void CX2UISocketItem::ResetSocketWindow( UidType Uid )
{
	m_SocketItemUID = Uid;
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID ); 

	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
		return;

#ifdef ADD_SOCKET_SLOT
	int slotNum = GetSocketSlotNum( pItem );
#else
	int slotNum = pItem->GetItemTemplet()->GetSocketSlotNum();
#endif // ADD_SOCKET_SLOT

	// 주의 : Dialog를 생성 시점에 로드하지 않는다
	RegisterLuaBind();

	SAFE_DELETE_DIALOG(m_pDlgSocketItem);
#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장 // 관리를 위해 리소스 직접 변경하지 않고 _New 추가 함
	switch(slotNum)
	{
	case 1:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_1slot_NEW.lua" );
		break;
	case 2:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_2slot_NEW.lua" );
		break;
	case 3:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_3slot_NEW.lua" );
		break;
	case 4:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_4slot_NEW.lua" );
		break;
	case 5:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_5slot_NEW.lua" );
		break;
	default:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_5slot_NEW.lua" );
		break;
	}
#else
	switch(slotNum)
	{
	case 1:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_1slot.lua" );
		break;
	case 2:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_2slot.lua" );
		break;
	case 3:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_3slot.lua" );
		break;
	case 4:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_4slot.lua" );
		break;
	default:
		m_pDlgSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_4slot.lua" );
		break;
	}
#endif // ADD_SOCKET_SLOT

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgSocketItem );
	SetLayer(m_Layer);

	m_pDlgSocketItem->SetDisableUnderWindow( true );

#ifdef UI_SOCKET_NEW_CHEAT
	SAFE_DELETE_DIALOG(m_pDlgSocketItemCheat);

#ifdef ADD_SOCKET_SLOT
	m_pDlgSocketItemCheat = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_Cheat_NEW.lua" );
#else
	m_pDlgSocketItemCheat = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_NEW_Socket_Cheat.lua" );
#endif // ADD_SOCKET_SLOT

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgSocketItemCheat );
	m_pDlgSocketItemCheat->SetDisableUnderWindow( true );
	

	m_pIMEEditSocketOptionCode[0] =  static_cast< CKTDGUIIMEEditBox* >( m_pDlgSocketItemCheat->GetControl( L"IMEEditCode1" ) );
	m_pIMEEditSocketOptionCode[1] =  static_cast< CKTDGUIIMEEditBox* >( m_pDlgSocketItemCheat->GetControl( L"IMEEditCode2" ) );
	m_pIMEEditSocketOptionCode[2] =  static_cast< CKTDGUIIMEEditBox* >( m_pDlgSocketItemCheat->GetControl( L"IMEEditCode3" ) );
	m_pIMEEditSocketOptionCode[3] =  static_cast< CKTDGUIIMEEditBox* >( m_pDlgSocketItemCheat->GetControl( L"IMEEditCode4" ) );
#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장
	m_pIMEEditSocketOptionCode[4] =  static_cast< CKTDGUIIMEEditBox* >( m_pDlgSocketItemCheat->GetControl( L"IMEEditCode5" ) );
#endif // ADD_SOCKET_SLOT

	m_pIMEEditSocketOptionCode[0]->SetShowEnable(false, false);
	m_pIMEEditSocketOptionCode[1]->SetShowEnable(false, false);
	m_pIMEEditSocketOptionCode[2]->SetShowEnable(false, false);
	m_pIMEEditSocketOptionCode[3]->SetShowEnable(false, false);
#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장
	m_pIMEEditSocketOptionCode[4]->SetShowEnable(false, false);
#endif // ADD_SOCKET_SLOT

	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		switch(slotNum)
		{
#ifdef ADD_SOCKET_SLOT// 소켓 슬롯 확장
		case 5:
			m_pIMEEditSocketOptionCode[4]->SetShowEnable(true, true);
#endif // ADD_SOCKET_SLOT
		case 4:
			m_pIMEEditSocketOptionCode[3]->SetShowEnable(true, true);
		case 3:
			m_pIMEEditSocketOptionCode[2]->SetShowEnable(true, true);
		case 2:
			m_pIMEEditSocketOptionCode[1]->SetShowEnable(true, true);
		case 1:
			m_pIMEEditSocketOptionCode[0]->SetShowEnable(true, true);
			break;
		default:
			break;
		}
	}
	m_pDlgSocketItemCheat->SetShowEnable(false, false);

	// 운영자 기능
	CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDlgSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
	if ( pCheckBox != NULL )
	{
		if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
		{
			pCheckBox->SetShowEnable( true, true );
			pCheckBox->SetChecked( false );
		}
		else
		{
			pCheckBox->SetShowEnable( false, false );
		}
	}
	

#endif UI_SOCKET_NEW_CHEAT




	//아이템 이름하고 아이템 슬롯 이미지 표시
	CKTDGUIStatic* pStaticSlotImage = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( L"g_pStatic_item_Slot_Image" );
	if( NULL != pStaticSlotImage )
	{
		if ( pStaticSlotImage->GetPicture(0) != NULL )
		{
			pStaticSlotImage->GetPicture(0)->SetTex( 
	            pItem->GetItemTemplet()->GetShopImage()
	            );
			//pStaticSlotImage->GetPicture(0)->SetSize( D3DXVECTOR2 ( 44, 44 ) );
		}
	
		//{{ kimhc // 2009-09-08 // 봉인된 아이템 이미지 출력
	#ifdef	SEAL_ITEM
		if ( pStaticSlotImage->GetPicture( 1 ) != NULL )
		{
			if ( pItem->GetItemData().m_bIsSealed == true )
				pStaticSlotImage->GetPicture( 1 )->SetShow( true );
			else
				pStaticSlotImage->GetPicture( 1 )->SetShow( false );
		}		
#endif	SEAL_ITEM
		//}} kimhc // 2009-09-08 // 봉인된 아이템 이미지 출력
	}


	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( L"Static_Item_Name1" );
	if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
	{
		pStaticItemName->GetString(0)->msg = pItem->GetFullName();
	}

	// 아바타
	m_bFashion = pItem->GetItemTemplet()->GetFashion();
	// 아이템 가격
	m_ItemPrice = pItem->GetItemTemplet()->GetPrice();
	// 수수료 초기화
	m_iTotalCostED = 0;
	UpdateUICostED();
	// 소켓 카운트 초기화
	m_iSocketCount = 0;

	// 초기화
	for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
	{
		wstringstream wstrstm;

		wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
		wstrstm << i + 1;
		CKTDGUIControl* pControl = m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShow( false );
		}
		wstrstm.str(L"");

		wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
		wstrstm << i + 1;
		pControl = m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShow( false );
		}
		wstrstm.str(L"");

		CKTDGUIStatic* pStaticStoneSlotImage = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"StaticStoneSlotImage" ));
		if ( pStaticStoneSlotImage != NULL )
		{
			CKTDGUIControl::CPictureData* pPicture = pStaticStoneSlotImage->GetPicture(i);
			if( NULL != pPicture )
			{
				pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"slot" );
				pPicture->SetSize( D3DXVECTOR2 (35, 35) );
				pPicture->SetShow(false);
			}
			else
			{
				ASSERT(!"Slot Picture is NULL!");
			}
			
			//pPicture->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
		}

		// 소켓 슬롯 구조체 초기화
		m_vecSocketSlotInfo[i].m_StateSlotState = CX2UISocketItem::SSS_NOT_AVAILABLE;
		m_vecSocketSlotInfo[i].m_StoneUID = -1;
		m_vecSocketSlotInfo[i].m_iQuantityStoneRemain = -1;

#ifdef UI_SOCKET_NEW_CHEAT
		// 소켓 치트 초기화
		m_iSocketOptionCode[i] = 0;
		if( NULL != m_pIMEEditSocketOptionCode[i] )
			m_pIMEEditSocketOptionCode[i]->SetText(L"");
		else
		{
			ASSERT(!"Slot IME is NULL!");
		}
#endif UI_SOCKET_NEW_CHEAT


		int iLastSlot = 0;
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			pItemSlot->SetShow( false );
			pItemSlot->SetEnable( false );
			iLastSlot = i;
		}

		// 마지막 슬롯은 아이템 정보 창
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[iLastSlot];
		pItemSlot->SetShow( true );
		pItemSlot->SetEnable( true ); 

	}





	CKTDGUIStatic* pStaticInfo = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"g_pStatic_Memo1" ));
	CKTDGUIStatic* pStaticInfo2 = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"g_pStatic_Memo2" ));
#ifdef CLIENT_GLOBAL_LINEBREAK
#ifdef MULTILINE_STR_ID_5080
	pStaticInfo->SetString(0, L"");
	pStaticInfo2->SetString(0, GET_STRING(STR_ID_5080));
#else MULTILINE_STR_ID_5080
	wstring wstrMemo = CWordLineHandler::GetStrByLineBreakInX2Main(GET_STRING(STR_ID_5080), 190, XUF_DODUM_13_SEMIBOLD);
	pStaticInfo->SetString(0, wstrMemo.c_str());
	pStaticInfo2->SetString(0, L"");
#endif //MULTILINE_STR_ID_5080
#else //CLIENT_GLOBAL_LINEBREAK

#ifdef MULTILINE_STR_ID_5080
	pStaticInfo->SetString(0, L"");
	pStaticInfo2->SetString(0, GET_STRING(STR_ID_5080));
#else MULTILINE_STR_ID_5080
	pStaticInfo->SetString(0, GET_STRING(STR_ID_5080));
	pStaticInfo2->SetString(0, L"");
#endif MULTILINE_STR_ID_5080

#endif //CLIENT_GLOBAL_LINEBREAK

	CKTDGUIStatic* pStatic_LastExtraDamage = NULL;

	// 아이템으로부터 소켓 정보를 읽어옴
	for ( int i = 0; i < slotNum; i++ )
	{
		bool bCheckEmpty = true;

		if ( i < (int)pItem->GetItemData().m_SocketOption.size() )
		{
			//0으로 채워져 있으면 비어있는거고 다른값으로 채워져 있는거면 있는거다..
			int socketOption = pItem->GetItemData().m_SocketOption[i];
			if ( socketOption != 0 )
			{
				bCheckEmpty = false;
				wstringstream wstrstm;
				wstrstm.str( L"" );

				// 소켓 카운트 증가
				m_iSocketCount++;

				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
				if ( pSocketData != NULL )
				{
					//여기다가.. 소켓 옵션 설명 가져와서 한줄 짜리인지 두줄짜리인지 보고.. 넣자잉..

					bool bCheckTwoLine = false;

		//{{ kimhc // 2011-07-21 // 옵션 수치화
					int iSocketLevel = pItem->GetItemTemplet()->GetUseLevel();
					if ( 0 >= iSocketLevel )
						iSocketLevel = g_pData->GetSelectUnitLevel();

#ifdef CLIENT_GLOBAL_LINEBREAK
					wstring socketDesc = CWordLineHandler::GetStrByLineBreakInX2Main( pSocketData->GetSocketDesc( iSocketLevel ).c_str(), 190, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
					wstring socketDesc = g_pMain->GetStrByLienBreak( pSocketData->GetSocketDesc( iSocketLevel ).c_str(), 190, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
		//}} kimhc // 2011-07-21 // 옵션 수치화
					
					if ( socketDesc.find( L"\n") != -1 )
						bCheckTwoLine = true;

					if ( bCheckTwoLine == false )
					{
						wstringstream wstrstm;
						wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
						wstrstm << i + 1;
						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
						if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
						{
							pControlStatic->GetString(0)->msg = socketDesc;
							pControlStatic->SetShow( true );
							pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );

							if( pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
							{
								if( NULL != pStatic_LastExtraDamage )
								{
									pStatic_LastExtraDamage->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.f ) );
								}
								pStatic_LastExtraDamage = pControlStatic;
							}
						}
					}
					else
					{
						wstringstream wstrstm;
						wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
						wstrstm << i + 1;
						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
						if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
						{
							pControlStatic->GetString(0)->msg = socketDesc;
							pControlStatic->SetShow( true );
							pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );

							if( pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
							{
								if( NULL != pStatic_LastExtraDamage )
								{
									pStatic_LastExtraDamage->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.f ) );
								}
								pStatic_LastExtraDamage = pControlStatic;
							}
						}
					}			
				}

#ifdef FIX_SOCKET_ICON_TEXTURE
				// 추가된 마법석 아이콘의 텍스쳐 파일명 식별 하기 위해 수정
				SetSocketIconTextureBySocketOption(i, socketOption);
#else
				// 장착된 마법석 이미지 표시
				CKTDGUIStatic* pStaticStoneSlotImage = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"StaticStoneSlotImage" ));
				if ( pStaticStoneSlotImage != NULL )
				{
					CKTDGUIControl::CPictureData* pPicture = pStaticStoneSlotImage->GetPicture(i);
					
					// <<<<<< 소켓 아이디로 마법석 아이템 이미지 식별하여 가져와야함 
					// socketOption

					wstring socketIconTextureKey = GetSocketIconTextureKeyBySocketOption(socketOption);

					pPicture->SetTex( L"DLG_UI_Common_Texture13.TGA", socketIconTextureKey.c_str() ); 
					pPicture->SetShow(true);
					//pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"slot" );
				}
#endif //FIX_SOCKET_ICON_TEXTURE
/*
				// 장착되어 있음을 표시
				CKTDGUIStatic* pStaticStoneEquipImage = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"StaticStoneEquipImage" ));
				if ( pStaticStoneEquipImage != NULL )
				{
					CKTDGUIControl::CPictureData* pPicture = pStaticStoneEquipImage->GetPicture(i);
					pPicture->SetShow(true);
				}
*/
				m_vecSocketSlotInfo[i].m_StateSlotState = CX2UISocketItem::SSS_EQUIPPED;
				m_vecSocketSlotInfo[i].m_StoneUID = -1;
				m_vecSocketSlotInfo[i].m_iQuantityStoneRemain = -1;
				m_vecSocketSlotInfo[i].m_iEquippedSocketOption = socketOption;
			}
			

		}
		if ( bCheckEmpty == true )
		{
			// 비어있는 마법석 이미지 표시
			CKTDGUIStatic* pStaticStoneSlotImage = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"StaticStoneSlotImage" ));
			if ( pStaticStoneSlotImage != NULL )
			{
				CKTDGUIControl::CPictureData* pPicture = pStaticStoneSlotImage->GetPicture(i);

				pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"slot" );
				pPicture->SetShow(false);
			}

			m_vecSocketSlotInfo[i].m_StateSlotState = CX2UISocketItem::SSS_EMPTY;
			m_vecSocketSlotInfo[i].m_StoneUID = -1;
			m_vecSocketSlotInfo[i].m_iQuantityStoneRemain = -1;
			m_vecSocketSlotInfo[i].m_iEquippedSocketOption = 0;

		}
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		pItemSlot->SetShow( true );
		pItemSlot->SetEnable( true );

	}


	CKTDGUIButton* pButtonEquipStone = static_cast< CKTDGUIButton* >(m_pDlgSocketItem->GetControl( L"ButtonEquipStone" ));
	if ( pButtonEquipStone != NULL )
	{
		pButtonEquipStone->SetShowEnable( true, true );
	}

	
	UpdateButtonEquip();
	
	g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
	g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();

#ifdef SERV_PET_SYSTEM
	// oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리에서 소켓 마법석 사용 
	if(g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory() != NULL)
	{
		g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->UpdateUI();
	}
#endif SERV_PET_SYSTEM

	return;
}
#ifdef FIX_SOCKET_ICON_TEXTURE
void CX2UISocketItem::SetSocketIconTextureBySocketOption(int iSoketIndex, int iSocketOption)
{
	// 장착된 마법석 이미지 표시
	if( NULL == m_pDlgSocketItem ) 
		return;

	CKTDGUIStatic* pStaticStoneSlotImage = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"StaticStoneSlotImage" ));
	if ( NULL == pStaticStoneSlotImage )
		return;

	CKTDGUIControl::CPictureData* pPicture = pStaticStoneSlotImage->GetPicture(iSoketIndex);
	if( NULL == pPicture )
		return;


	// 일반 마법석의 기본 텍스쳐
	wstring socketIconTextureFile = L"DLG_UI_Common_Texture13.TGA";
	wstring socketIconTextureKey  = L"GEM00";

	// 소켓 아이디로 마법석 아이템 이미지 식별

	if(iSocketOption >= 14000 && iSocketOption < 15000)
	{
		socketIconTextureKey = L"GEM01";		
	}
	else if(iSocketOption >= 15000 && iSocketOption < 16000)
	{
		socketIconTextureKey = L"GEM02";		
	}
	else if(iSocketOption >= 16000 && iSocketOption < 17000)
	{
		socketIconTextureKey = L"GEM03";		
	}
	else if(iSocketOption >= 17000 && iSocketOption < 18000)
	{
		socketIconTextureKey = L"GEM04";		
	}
	else if(iSocketOption >= 18000 && iSocketOption < 19000)
	{
		socketIconTextureKey = L"GEM05";		
	}
	else if(iSocketOption >= 19000 && iSocketOption < 20000)
	{
		socketIconTextureKey = L"GEM06";		
	}
#ifdef PVP_SEASON2
	else if( ( iSocketOption >= 50001 && iSocketOption < 50201 ) || ( iSocketOption >= 55001 && iSocketOption < 55201 ) ) // 파괴의 마법석
	{
		socketIconTextureFile = L"DLG_UI_Common_Texture15.TGA";
		socketIconTextureKey = L"GRAY";
	}
	else if( ( iSocketOption >= 50201 && iSocketOption < 50401 ) || ( iSocketOption >= 55201 && iSocketOption < 55401 ) ) // 폭발의 마법석
	{
		socketIconTextureKey = L"GEM05";
	}
	else if( ( iSocketOption >= 50401 && iSocketOption < 50601 ) || ( iSocketOption >= 55401 && iSocketOption < 55601 ) ) // 연소의 마법석
	{
		socketIconTextureFile = L"DLG_UI_Common_Texture15.TGA";
		socketIconTextureKey = L"DEEP_BLUE";				
	}
	else if( ( iSocketOption >= 50601 && iSocketOption < 50801 ) || ( iSocketOption >= 55601 && iSocketOption < 55801 ) ) // 신속의 마법석
	{
		socketIconTextureFile = L"DLG_UI_Common_Texture15.TGA";
		socketIconTextureKey = L"DIA";				
	}
	else if( ( iSocketOption >= 50801 && iSocketOption < 51001 ) || ( iSocketOption >= 55801 && iSocketOption < 56001 ) ) // 신속의 마법석
	{
		socketIconTextureFile = L"DLG_UI_Common_Texture15.TGA";
		socketIconTextureKey = L"BLUE";				
	}
#else
	else if( iSocketOption >= 50001 && iSocketOption < 50201 ) // 파괴의 마법석
	{
		socketIconTextureFile = L"DLG_UI_Common_Texture15.TGA";
		socketIconTextureKey = L"GRAY";
	}
	else if( iSocketOption >= 50201 && iSocketOption < 50401 ) // 폭발의 마법석
	{
		socketIconTextureKey = L"GEM05";
	}
	else if( iSocketOption >= 50401 && iSocketOption < 50601 ) // 연소의 마법석
	{
		socketIconTextureFile = L"DLG_UI_Common_Texture15.TGA";
		socketIconTextureKey = L"DEEP_BLUE";				
	}
	else if( iSocketOption >= 50601 && iSocketOption < 50801 ) // 신속의 마법석
	{
		socketIconTextureFile = L"DLG_UI_Common_Texture15.TGA";
		socketIconTextureKey = L"DIA";				
	}
	else if( iSocketOption >= 50801 && iSocketOption < 51001 ) // 신속의 마법석
	{
		socketIconTextureFile = L"DLG_UI_Common_Texture15.TGA";
		socketIconTextureKey = L"BLUE";				
	}
#endif

	pPicture->SetTex( socketIconTextureFile.c_str(), socketIconTextureKey.c_str() ); 
	pPicture->SetShow(true);	
}
#else
wstring CX2UISocketItem::GetSocketIconTextureKeyBySocketOption(int iSocketOption)
{
	wstring socketIconTextureKey=  L"";
	

	if(iSocketOption >= 14000 && iSocketOption < 15000)
	{
		socketIconTextureKey = L"GEM01";		
	}
	else if(iSocketOption >= 15000 && iSocketOption < 16000)
	{
		socketIconTextureKey = L"GEM02";		
	}
	else if(iSocketOption >= 16000 && iSocketOption < 17000)
	{
		socketIconTextureKey = L"GEM03";		
	}
	else if(iSocketOption >= 17000 && iSocketOption < 18000)
	{
		socketIconTextureKey = L"GEM04";		
	}
	else if(iSocketOption >= 18000 && iSocketOption < 19000)
	{
		socketIconTextureKey = L"GEM05";		
	}
	else if(iSocketOption >= 19000 && iSocketOption < 20000)
	{
		socketIconTextureKey = L"GEM06";		
	}
	else
	{
		socketIconTextureKey = L"GEM00";
	}

	

	return socketIconTextureKey;
}
#endif //FIX_SOCKET_ICON_TEXTURE

wstring CX2UISocketItem::GetSocketIconTextureNameBySocketTID(int TID)
{
	wstring socketIconTexName=  L"";
	
    socketIconTexName = g_pData->GetItemManager()->GetItemTemplet(TID)->GetShopImage();

	return socketIconTexName;
}


void CX2UISocketItem::SetPosition(D3DXVECTOR2 vec)
{
	m_pDlgSocketItem->SetPos(vec);
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


void CX2UISocketItem::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);

	m_Layer = layer;
	if(m_pDlgSocketItem != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgSocketItem, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgSocketItem, false );
	}	
}

void CX2UISocketItem::DrawSlotMouseOverImage()
{

	//g_pKTDXApp->GetDeviceManager()->PlaySound( L"MagicStone_MouseOver.ogg", false, false );


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

bool CX2UISocketItem::IsStoneItem(UidType Uid)
{
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( Uid );

	const UINT iArraySize = ARRAY_SIZE( _CONST_UISOCKETITEM_INFO_::g_iArrayMagicStoneID );
	const int iItemID = pItem->GetItemData().m_ItemID;

	bool bFind = false;
	for ( UINT i = 0; i < iArraySize; i++ )
	{
		if ( iItemID == _CONST_UISOCKETITEM_INFO_::g_iArrayMagicStoneID[i] )
		{
			bFind = true;
			break;
		}
	}

	return bFind;
}

 

void CX2UISocketItem::ShowSocketDescExpected(int iSlotIndex)
{
	
	//{{ 소켓 옵션 Desc 초기화
	wstringstream wstrstm;
	wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
	wstrstm << iSlotIndex + 1;
	CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
	if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
	{
		pControlStatic->GetString(0)->msg = L"";
		pControlStatic->SetShow( false );
		pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );
	}
	wstrstm.str(L"");
	wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
	wstrstm << iSlotIndex + 1;
	pControlStatic = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
	if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
	{
		pControlStatic->GetString(0)->msg = L"";
		pControlStatic->SetShow( false );
		pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );
	}
	//}}


	UidType Uid = m_vecSocketSlotInfo[iSlotIndex].m_StoneUID;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( Uid );

	wstringstream wstrDesc;
	wstrDesc.str( L"" );
	
	const UINT iArraySize = ARRAY_SIZE( _CONST_UISOCKETITEM_INFO_::g_iArrayMagicStoneID );
	const int iItemID = pItem->GetItemData().m_ItemID;

	int iIndex = -1;
	for ( UINT i = 0; i < iArraySize; i++ )
	{
		if ( iItemID == _CONST_UISOCKETITEM_INFO_::g_iArrayMagicStoneID[i] )
		{
			iIndex = i;
			break;
		}
	}

	if ( -1 < iIndex && iArraySize > iIndex )
	{
		// 포인터의 위치 계산으로 인덱스를 구해준다.
#ifdef SERV_9TH_NEW_CHARACTER //JHKang
		if ( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			if ( g_pData->GetMyUser()->GetSelectUnit()->GetType() == CX2Unit::UT_ADD )
			{
				UINT uStrID = _CONST_UISOCKETITEM_INFO_::g_iArrayMagicStoneStringID[iIndex];

				switch( iIndex )
				{
				case 4:		// STR_5076
					uStrID = STR_ID_29867;
					break;

				case 6:		// STR_5078
					uStrID = STR_ID_29868;
					break;

				case 20:	// STR_5089
				case 34:	// STR_5089
					uStrID = STR_ID_29869;
					break;

				case 19:	// STR_5091
				case 36:	// STR_5091
					uStrID = STR_ID_29870;
					break;

				case 12:	// STR_13904
				case 29:	// STR_13904
					uStrID = STR_ID_29873;
					break;

				default:
					break;
				}
			}
			else
				wstrDesc << GET_STRING( _CONST_UISOCKETITEM_INFO_::g_iArrayMagicStoneStringID[iIndex] );

		}
#else //SERV_9TH_NEW_CHARACTER
		wstrDesc << GET_STRING( _CONST_UISOCKETITEM_INFO_::g_iArrayMagicStoneStringID[iIndex] );
#endif //SERV_9TH_NEW_CHARACTER
	}
	
	bool bCheckTwoLine = false;
#ifdef CLIENT_GLOBAL_LINEBREAK
	wstring socketDesc = CWordLineHandler::GetStrByLineBreakInX2Main( wstrDesc.str().c_str(), 190, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
	wstring socketDesc = g_pMain->GetStrByLienBreak( wstrDesc.str().c_str(), 190, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
	if ( socketDesc.find( L"\n") != -1 )
		bCheckTwoLine = true;

	if ( bCheckTwoLine == false )
	{
		wstrstm.str(L"");
		wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
		wstrstm << iSlotIndex + 1;
		CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
		{
			pControlStatic->GetString(0)->msg = socketDesc;
			pControlStatic->SetShow( true );
			pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );
		}
	}
	else
	{
		wstrstm.str(L"");
		wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
		wstrstm << iSlotIndex + 1;
		CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
		{
			pControlStatic->GetString(0)->msg = socketDesc;
			pControlStatic->SetShow( true );
			pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );


		}
	}

	return;
}

int CX2UISocketItem::CalculateNewSocketCost(UidType UID)
{
	int iCost = 0;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( UID );
    CX2Item::ITEM_GRADE StoneGrade = pItem->GetItemTemplet()->GetItemGrade();

	double dStoneCostFactor = 0.0;

	switch(StoneGrade)
	{
	case CX2Item::IG_NONE:
	case CX2Item::IG_LOW:
	case CX2Item::IG_NORMAL:
		dStoneCostFactor = _CONST_UISOCKETITEM_INFO_::g_dNormalStoneCostFactor;
		break;
	case CX2Item::IG_RARE:
		dStoneCostFactor = _CONST_UISOCKETITEM_INFO_::g_dRareStoneCostFactor;
		break;
	case CX2Item::IG_ELITE:
		dStoneCostFactor = _CONST_UISOCKETITEM_INFO_::g_dEliteStoneCostFactor;
		break;
	case CX2Item::IG_UNIQUE:
		dStoneCostFactor = _CONST_UISOCKETITEM_INFO_::g_dUniqueStoneCostFactor;
		break;
	default:
		break;
	}

	
	int iSocketCount = m_iSocketCount;

	if(iSocketCount <= 0)
	{
		iSocketCount = 1;
	}

	if(m_bFashion)
	{
		iCost = (int)((double)1000 * dStoneCostFactor);
	}
	else
	{
		iCost = (int)((double)m_ItemPrice * _CONST_UISOCKETITEM_INFO_::g_dSocketCostFactor * (double)iSocketCount * dStoneCostFactor);
	}
	return iCost;
}
int CX2UISocketItem::CalculateReplaceSocketCost(UidType UID)
{
	return CalculateNewSocketCost(UID) * 3;
}
void CX2UISocketItem::UpdateUICostED()
{
	CKTDGUIStatic* pStatic_ED = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"g_pStatic_ED" ));
	WCHAR buff[256] = {0};
	_itow( m_iTotalCostED, buff, 10 );
	pStatic_ED->GetString(0)->msg = buff;
	
	UpdateButtonEquip();

	return;
}

void CX2UISocketItem::UpdateButtonEquip()
{

	bool bReadyToEquip = false;
	for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
	{
		if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP)
		{
			bReadyToEquip = true;
		}
	}
	if(!bReadyToEquip)
	{
		SetEnableButtonEquip(false);		
	}
	else
	{
		CKTDGUIStatic* pStatic_ED = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"g_pStatic_ED" ));
		CKTDGUIStatic* pStaticInfo = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"g_pStatic_Memo1" ));
		CKTDGUIStatic* pStaticInfo2 = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"g_pStatic_Memo2" ));

		if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED < m_iTotalCostED)
		{
			pStatic_ED->GetString(0)->color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
#ifdef MULTILINE_STR_ID_5080
			pStaticInfo->SetString(0, L"");
			pStaticInfo2->SetString(0, GET_STRING(STR_ID_5072));
#else MULTILINE_STR_ID_5080
			pStaticInfo->SetString(0, GET_STRING(STR_ID_5072));
			pStaticInfo2->SetString(0, L"");
#endif MULTILINE_STR_ID_5080


			SetEnableButtonEquip(false);
		}
		else
		{
			pStatic_ED->GetString(0)->color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pStaticInfo->SetString(0, L"");
			pStaticInfo2->SetString(0, GET_STRING(STR_ID_5071));
			SetEnableButtonEquip(true);
		}
	}
	return;
}

void CX2UISocketItem::SetEnableButtonEquip(bool bEnable)
{
	CKTDGUIButton* pButtonEquip = static_cast< CKTDGUIButton* >(m_pDlgSocketItem->GetControl( L"Button__Socket_Begin" ));

	pButtonEquip->SetEnable(bEnable);


	CKTDGUIStatic* pStaticEquip = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"ButtonEquipStone" ));

	if(bEnable == false)
	{
		pStaticEquip->GetString(1)->color = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	}
	else
	{
		pStaticEquip->GetString(1)->color = D3DXCOLOR(0.0f, 0.9f, 1.0f, 1.0f);
	}
	return;
}


bool CX2UISocketItem::OnRClickedItem( D3DXVECTOR2 mousePos )
{
	// 마우스업된 위치가 인벤 안일 경우
	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL )
		return false;


	int iSlotIndex = pItemSlot->GetSlotID();

	switch(m_vecSocketSlotInfo[iSlotIndex].m_StateSlotState)
	{
	case SSS_NOT_AVAILABLE:
	case SSS_EMPTY:
	case SSS_EQUIPPED:
		// do nothing
		break;
	case SSS_READY_TO_EQUIP:
		{

			// 해당 소켓에 대기 중인 마법석을 인벤토리로 돌려놓으므로, 다른 소켓에서 사용되는 해당 마법석의 개수를 증가시킨다.
			for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
			{
				if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP)
				{	
					if( m_vecSocketSlotInfo[i].m_StoneUID == m_vecSocketSlotInfo[iSlotIndex].m_StoneUID )
					{
						m_vecSocketSlotInfo[i].m_iQuantityStoneRemain++;
					}
				}
			}
			// 소켓 옵션 Desc 초기화
			wstringstream wstrstm;

			wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
			wstrstm << iSlotIndex + 1;
			CKTDGUIControl* pControl = m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
			if ( pControl != NULL )
			{
				pControl->SetShow( false );
			}
			wstrstm.str(L"");

			wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
			wstrstm << iSlotIndex + 1;
			pControl = m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
			if ( pControl != NULL )
			{
				pControl->SetShow( false );
			}

			// 비어있던 소켓이었을 경우
			if(m_vecSocketSlotInfo[iSlotIndex].m_iEquippedSocketOption == 0)
			{

				CKTDGUIStatic* pStaticStoneSlotImage = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"StaticStoneSlotImage" ));
				if ( pStaticStoneSlotImage != NULL )
				{
					CKTDGUIControl::CPictureData* pPicture = pStaticStoneSlotImage->GetPicture(iSlotIndex);
					pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"slot" );
					pPicture->SetSize( D3DXVECTOR2 (35, 35) );
					pPicture->SetShow(false);

					//pPicture->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
				}
		
				// 소켓 가격 원상복귀
				int iCost_ED = CalculateNewSocketCost(m_vecSocketSlotInfo[iSlotIndex].m_StoneUID);
				m_iTotalCostED = m_iTotalCostED - iCost_ED;
				UpdateUICostED();

				m_vecSocketSlotInfo[iSlotIndex].m_StateSlotState = SSS_EMPTY;
				m_vecSocketSlotInfo[iSlotIndex].m_StoneUID = -1;
				m_vecSocketSlotInfo[iSlotIndex].m_iQuantityStoneRemain = -1;

			}
			else // 옵션을 가지고 있던 소켓이었을 경우
			{
				CKTDGUIStatic* pStatic_LastExtraDamage = NULL;

				wstrstm.str( L"" );

				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( m_vecSocketSlotInfo[iSlotIndex].m_iEquippedSocketOption );
				if ( pSocketData != NULL )
				{
					bool bCheckTwoLine = false;

					//{{ kimhc // 2011-07-21 // 옵션 수치화
					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID ); 

					int iSocketLevel = g_pData->GetSelectUnitLevel();
					if ( NULL != pItem && 0 < pItem->GetItemTemplet()->GetUseLevel() )
						iSocketLevel = pItem->GetItemTemplet()->GetUseLevel();
#ifdef CLIENT_GLOBAL_LINEBREAK
					wstring socketDesc = CWordLineHandler::GetStrByLineBreakInX2Main(pSocketData->GetSocketDesc( iSocketLevel ).c_str(), 190, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
					wstring socketDesc = g_pMain->GetStrByLienBreak( pSocketData->GetSocketDesc( iSocketLevel ).c_str(), 190, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
					//}} kimhc // 2011-07-21 // 옵션 수치화

					if ( socketDesc.find( L"\n") != -1 )
						bCheckTwoLine = true;

					if ( bCheckTwoLine == false )
					{
						wstringstream wstrstm;
						wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
						wstrstm << iSlotIndex + 1;
						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
						if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
						{
							pControlStatic->GetString(0)->msg = socketDesc;
							pControlStatic->SetShow( true );
							pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );

							if( pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
							{
								if( NULL != pStatic_LastExtraDamage )
								{
									pStatic_LastExtraDamage->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.f ) );
								}
								pStatic_LastExtraDamage = pControlStatic;
							}
						}
					}
					else
					{
						wstringstream wstrstm;
						wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
						wstrstm << iSlotIndex + 1;
						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDlgSocketItem->GetControl( wstrstm.str().c_str() );
						if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
						{
							pControlStatic->GetString(0)->msg = socketDesc;
							pControlStatic->SetShow( true );
							pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );

							if( pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
							{
								if( NULL != pStatic_LastExtraDamage )
								{
									pStatic_LastExtraDamage->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.f ) );
								}
								pStatic_LastExtraDamage = pControlStatic;
							}
						}
					}			
				}

#ifdef FIX_SOCKET_ICON_TEXTURE
				// 추가된 마법석 아이콘의 텍스쳐 파일명 식별 하기 위해 수정
				SetSocketIconTextureBySocketOption(iSlotIndex, m_vecSocketSlotInfo[iSlotIndex].m_iEquippedSocketOption);
#else
				// 장착된 마법석 이미지 표시
				CKTDGUIStatic* pStaticStoneSlotImage = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"StaticStoneSlotImage" ));
				if ( pStaticStoneSlotImage != NULL )
				{
					CKTDGUIControl::CPictureData* pPicture = pStaticStoneSlotImage->GetPicture(iSlotIndex);

					// <<<<<< 소켓 아이디로 마법석 아이템 이미지 식별하여 가져와야함 
					// socketOption

					wstring socketIconTextureKey = GetSocketIconTextureKeyBySocketOption(m_vecSocketSlotInfo[iSlotIndex].m_iEquippedSocketOption);

					pPicture->SetTex( L"DLG_UI_Common_Texture13.TGA", socketIconTextureKey.c_str() ); 
					pPicture->SetShow(true);
					//pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"slot" );
				}
#endif //FIX_SOCKET_ICON_TEXTURE

				// 소켓 가격 원상복귀
				int iCost_ED = CalculateReplaceSocketCost(m_vecSocketSlotInfo[iSlotIndex].m_StoneUID);
				m_iTotalCostED = m_iTotalCostED - iCost_ED;
				UpdateUICostED();

				m_vecSocketSlotInfo[iSlotIndex].m_StateSlotState = SSS_EQUIPPED;
				m_vecSocketSlotInfo[iSlotIndex].m_StoneUID = -1;
				m_vecSocketSlotInfo[iSlotIndex].m_iQuantityStoneRemain = -1;

			}

			UpdateButtonEquip();

			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();

#ifdef SERV_PET_SYSTEM
			// oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리에서 소켓 마법석 사용 
			if(g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->UpdateUI();
			}
#endif SERV_PET_SYSTEM



			g_pKTDXApp->GetDeviceManager()->PlaySound( L"MagicStone_MouseOver.ogg", false, false );


		}
		break;
	}

	return true;
}


bool CX2UISocketItem::OnDropAnyItem( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
		return false;
	if ( pItemSlot == *m_pSlotBeforeDragging )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
		SetNowDragItemReturn();
		return true;
	}
	//{{ 2008.11.13 김태완 : UI 예외처리
	if(!(*m_pSlotBeforeDragging)->IsItem()) return false;
	//*m_DraggingItemUID = ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetItemUID();
	//}}


	int iSlotIndex = pItemSlot->GetSlotID();

	switch((*m_pSlotBeforeDragging)->GetSlotType())
	{
	case CX2Slot::ST_INVENTORY:
#ifdef SERV_PET_SYSTEM
	case CX2Slot::ST_PET:
#endif SERV_PET_SYSTEM
		// 인벤토리에서 가져온 경우
		{
            switch( iSlotIndex )
			{
			case 0:
			case 1:
			case 2:
			case 3:
#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장 
			case 4:
#endif // ADD_SOCKET_SLOT
				{
					m_PickedSocketIndex = iSlotIndex;

					CX2SlotItem* pDraggedItem = (CX2SlotItem*)*m_pSlotBeforeDragging;
					UidType DraggedItemUID = pDraggedItem->GetItemUID();

					if(!IsStoneItem(DraggedItemUID))
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_5070), g_pMain->GetNowState() );
						return false;
					}

					SocketEquipReadyEffectEnd();

					g_pKTDXApp->GetDeviceManager()->PlaySound( L"MagicStone_PutDown.ogg", false, false );

					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( DraggedItemUID ); 
					int DraggedItemTID = pItem->GetItemData().m_ItemID;

					// 실제 남은 마법석 수를 찾아낸다.
					int iNumDraggedItemRemain = -1;
					for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
					{
						if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP)
						{	
							if( DraggedItemUID == m_vecSocketSlotInfo[i].m_StoneUID )
							{
								iNumDraggedItemRemain = m_vecSocketSlotInfo[i].m_iQuantityStoneRemain;
							}
						}
					}
					// 만약 현재 소켓들에 장착 대기 중인 동일한 UID의 마법석이 없다면 인벤토리의 Quantity 정보를 얻어온다.
					if( iNumDraggedItemRemain == -1)
					{
						iNumDraggedItemRemain = pItem->GetItemData().m_Quantity;
					}
					
					// 마법석의 개수가 부족
					if(iNumDraggedItemRemain == 0)
					{
						//오류메시지
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_5110 ), g_pMain->GetNowState() );
						return false;
					}
					
					// 마법석 개수 감소
					iNumDraggedItemRemain--;

					// 소켓 벡터 정보에 갱신된 마법석 개수를 적용한다.
					for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
					{
						if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP)
						{
							if( DraggedItemUID == m_vecSocketSlotInfo[i].m_StoneUID )
							{
								m_vecSocketSlotInfo[i].m_iQuantityStoneRemain = iNumDraggedItemRemain;
							}
						}
					}

					// 대상 소켓에 이미 장착 대기 중인 마법석이 있다면 다른 소켓 벡터 정보에서 
					// 그 마법석의 개수를 증가시킨다.
					if( m_vecSocketSlotInfo[iSlotIndex].m_StateSlotState == SSS_READY_TO_EQUIP )
					{
						m_vecSocketSlotInfo[iSlotIndex].m_iQuantityStoneRemain++;

						for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
						{
							if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP)
							{	
								if( m_vecSocketSlotInfo[i].m_StoneUID == m_vecSocketSlotInfo[iSlotIndex].m_StoneUID )
								{
									m_vecSocketSlotInfo[i].m_iQuantityStoneRemain = m_vecSocketSlotInfo[iSlotIndex].m_iQuantityStoneRemain;
									if( i == iSlotIndex && m_vecSocketSlotInfo[iSlotIndex].m_StoneUID == DraggedItemUID)
									{
										iNumDraggedItemRemain++;
									}
								}
							}
						}
					}

					int iCost_ED = 0;
					switch (m_vecSocketSlotInfo[iSlotIndex].m_StateSlotState)
					{
					case SSS_EMPTY:
						iCost_ED = CalculateNewSocketCost(DraggedItemUID);
						break;
					case SSS_EQUIPPED:
						iCost_ED = CalculateReplaceSocketCost(DraggedItemUID);
						break;
					case SSS_READY_TO_EQUIP:
						if(m_vecSocketSlotInfo[iSlotIndex].m_iEquippedSocketOption == 0)
						{
							iCost_ED = CalculateNewSocketCost(DraggedItemUID) 
								- CalculateNewSocketCost(m_vecSocketSlotInfo[iSlotIndex].m_StoneUID);
						}
						else
						{
							iCost_ED = CalculateReplaceSocketCost(DraggedItemUID) 
								- CalculateReplaceSocketCost(m_vecSocketSlotInfo[iSlotIndex].m_StoneUID);
						}
						break;
					default:
						break;
					}

					m_iTotalCostED = m_iTotalCostED + iCost_ED;
					UpdateUICostED();


					// 마지막으로 대상 소켓에 드래그한 소켓의 정보를 집어넣는다.
					m_vecSocketSlotInfo[iSlotIndex].m_StateSlotState = SSS_READY_TO_EQUIP;
					m_vecSocketSlotInfo[iSlotIndex].m_StoneUID = DraggedItemUID;
					m_vecSocketSlotInfo[iSlotIndex].m_iQuantityStoneRemain = iNumDraggedItemRemain;
					
					
					// 장착될 마법석 이미지 표시
					CKTDGUIStatic* pStaticStoneSlotImage = static_cast< CKTDGUIStatic* >(m_pDlgSocketItem->GetControl( L"StaticStoneSlotImage" ));
					if ( pStaticStoneSlotImage != NULL )
					{
						CKTDGUIControl::CPictureData* pPicture = pStaticStoneSlotImage->GetPicture(iSlotIndex);

						wstring socketIconTextureName = GetSocketIconTextureNameBySocketTID(DraggedItemTID);

						pPicture->SetTex( socketIconTextureName.c_str() ); 
						pPicture->SetShow(true);
						//pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"slot" );
					}

					ShowSocketDescExpected(iSlotIndex);
					UpdateButtonEquip();

					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();

#ifdef SERV_PET_SYSTEM
					// oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리에서 소켓 마법석 사용 
					if(g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory() != NULL)
					{
						g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->UpdateUI();
					}
#endif SERV_PET_SYSTEM

				}
				break;
			default:
				break;
			}

			SetNowDragItemReturn();
			return true;
		}
		break;
	default:
		break;
	}

	// 처리가 안 된 경우
	return false;
 }



HRESULT CX2UISocketItem::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( (m_pDlgSocketItem != NULL && m_pDlgSocketItem->GetIsMouseOver() == true) )
	{
		return CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
	}
	else
	{
		CX2ItemSlotManager::InvalidSelectedItem();
	}

	return S_OK;
}


bool CX2UISocketItem::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
		{
			if ( false == m_pDlgSocketItem->GetIsMouseOver())
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
			if ( false == m_pDlgSocketItem->GetIsMouseOver())
			{
				return false;
			}

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

			MouseUp( mousePos );

		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );


			bool bStoneOverSlot = false;
			for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
			{
				if( m_vecSocketSlotInfo[i].m_StateSlotState != SSS_NOT_AVAILABLE )
				{	
					if(m_SlotList[i]->IsInMousePoint(mousePos) == true)
					{
						bStoneOverSlot = true;
						if(m_bMouseOverSoundCondition_1 &&
							m_bMouseOverSoundCondition_2)
						{
							
							g_pKTDXApp->GetDeviceManager()->PlaySound( L"MagicStone_MouseOver.ogg", false, false );
							m_bMouseOverSoundCondition_2 = false;
						}				
					}
				}
			}
			if(bStoneOverSlot == false)
			{
				m_bMouseOverSoundCondition_2 = true;
			}

		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			if ( MouseDown( mousePos) == true )
				return true;

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



bool CX2UISocketItem::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_SOCKET_ITEM_ACK:
		{
			return Handler_EGS_SOCKET_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} 
		break;
	}
	return false;
}


bool CX2UISocketItem::Handler_EGS_SOCKET_ITEM_REQ( bool bDev )
{
	KEGS_SOCKET_ITEM_REQ kPacket;

	kPacket.m_iItemUID = m_SocketItemUID;
	kPacket.m_mapSocketInfo.clear();

#ifdef UI_SOCKET_NEW_CHEAT
	if(bDev)
	{
		kPacket.m_bCheat = true;
		
		for(int i=0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++)
		{
			if( m_iSocketOptionCode[i] != 0 )
			{
				kPacket.m_mapSocketInfo.insert( std::make_pair( i, m_iSocketOptionCode[i] ) );
			}
		}
	}
	else
	{
		kPacket.m_bCheat = false;
#endif UI_SOCKET_NEW_CHEAT
		for(int i=0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++)
		{
			if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP )
			{
				kPacket.m_mapSocketInfo.insert( std::make_pair( i, m_vecSocketSlotInfo[i].m_StoneUID ) );
			}
		}
#ifdef UI_SOCKET_NEW_CHEAT
	}
#endif UI_SOCKET_NEW_CHEAT
	g_pData->GetServerProtocol()->SendPacket( EGS_SOCKET_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SOCKET_ITEM_ACK );

	return true;
}


bool CX2UISocketItem::Handler_EGS_SOCKET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SOCKET_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_SOCKET_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			vector< int > vecOrgSocketOption;
			bool bCheckNewElementOption = false;

			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEvent.m_iItemUID );
			if ( pItem != NULL )
			{
				vecOrgSocketOption = pItem->GetItemData().m_SocketOption;
			}

			//특수 처리 고고싱
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

#ifdef SERV_PET_SYSTEM
			// oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리에서 소켓 마법석 사용 
			if(g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->UpdateUI();
			}
#endif SERV_PET_SYSTEM

			pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEvent.m_iItemUID );
			if ( pItem != NULL )
			{
				vector< int > vecNewSocketOption = pItem->GetItemData().m_SocketOption;
				for ( int i = 0; i < (int)vecNewSocketOption.size(); i++ )
				{
					int newSocketOption = vecNewSocketOption[i];
					bool bCheck = true;
					for ( int j = 0; j < (int)vecOrgSocketOption.size(); j++ )
					{
						int orgSocketOption = vecOrgSocketOption[j];
						if ( newSocketOption == orgSocketOption )
						{
							bCheck = false;
						}
					}
					if ( bCheck == true )
					{
						const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( newSocketOption );
						if ( pSocketData != NULL )
						{
							if ( pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
							{
								bCheckNewElementOption = true;
							}	
						}
					}
				}

				if ( bCheckNewElementOption == true )
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_827 ), g_pMain->GetNowState() );
			}

			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}
			SocketEquipEffect();

			ResetSocketWindow( kEvent.m_iItemUID );
			return true;
		}
		else
		{
			//  오류일 경우도 업데이트
			ResetSocketWindow( m_SocketItemUID );
		}

	}

	return false;
}


bool CX2UISocketItem::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case USICM_EXIT:
		{
			SetShow(false);
			return true;
		}
		break;
	case USICM_SOCKET_EQUIP:
		{
			bool bOverWrite = false;
			for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
			{
				if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP)
				{
					if(m_vecSocketSlotInfo[i].m_iEquippedSocketOption != 0)
					{
						bOverWrite = true;
					}
				}
			}
			if(bOverWrite)
			{
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_5069), USICM_VERIFY_EQUIP_OK, g_pMain->GetNowState());
			}
			else
			{
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_5068), USICM_VERIFY_EQUIP_OK, g_pMain->GetNowState());
			}
			return true;
		}
		break;
	case USICM_VERIFY_EQUIP_OK:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			Handler_EGS_SOCKET_ITEM_REQ();
		}
		break;
#ifdef UI_SOCKET_NEW_CHEAT
	case USICM_SOCKET_EQUIP_DEV:
		{
			if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				Handler_EGS_SOCKET_ITEM_REQ(true);
			}
		}
		break;
	case USICM_SOCKET_SECRET:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDlgSocketItem->GetControl(L"g_pCheckBoxForceSocket");
			if ( pCheckBox != NULL )
			{
				if(pCheckBox->GetChecked() == true)
				{
					m_pDlgSocketItemCheat->SetShowEnable(true, true);
				}
				else
				{
					m_pDlgSocketItemCheat->SetShowEnable(false, false);
				}
			}
		}
		break;
	case USICM_ENTER_CODE:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			int iSocketIdx = pButton->GetDummyInt(0);

			if( iSocketIdx >= 0 && iSocketIdx < MAX_SOCKET_SLOT_NUM )
			{
				if(m_pIMEEditSocketOptionCode[iSocketIdx]->GetTextLength() == 0)
				{
					m_iSocketOptionCode[iSocketIdx] = 0;
				}
				else
				{
					m_iSocketOptionCode[iSocketIdx] =  (int) wcstol( m_pIMEEditSocketOptionCode[iSocketIdx]->GetText(), NULL, 10 );

				}
			}			
		}
		break;
#endif UI_SOCKET_NEW_CHEAT
	default:
		break;
	}
	return false;
}

void CX2UISocketItem::SocketEquipEffect()
{
	D3DXVECTOR3 vecEffectPos[_CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem];

	CX2SlotItem* pItemSlot = NULL;


	for(int i=0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++)
	{
		pItemSlot = (CX2SlotItem*)m_SlotList[i];

		vecEffectPos[i].x = pItemSlot->GetPos().x +  pItemSlot->GetSize().x / 2.0f;
		vecEffectPos[i].y = pItemSlot->GetPos().y +  pItemSlot->GetSize().y / 2.0f;

	}
	
	
	float ftime_latency = 0.f;

	for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
	{
#ifdef UI_SOCKET_NEW_CHEAT
		if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP || m_iSocketOptionCode[i] != 0)
#else
		if( m_vecSocketSlotInfo[i].m_StateSlotState == SSS_READY_TO_EQUIP)
#endif UI_SOCKET_NEW_CHEAT
		{	
		
			CKTDGParticleSystem::CParticleEventSequence* pParticle1 = 
				g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"UI_Socket_StartEffect01", vecEffectPos[i].x, vecEffectPos[i].y, 0 );
			if( pParticle1 != NULL )
			{
				pParticle1->SetOverUI( true );
				pParticle1->SetResolutionConvert(true);
				pParticle1->SetLatency( ftime_latency );
			}

			CKTDGParticleSystem::CParticleEventSequence* pParticle2 =
				g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"UI_Socket_StartEffect02", vecEffectPos[i].x, vecEffectPos[i].y, 0 );
			if( pParticle2 != NULL )
			{
				pParticle2->SetOverUI( true );
				pParticle2->SetResolutionConvert(true);
				pParticle2->SetLatency( ftime_latency + 0.1f );
			}

			ftime_latency = ftime_latency + _CONST_UISOCKETITEM_INFO_::g_fSocketEquipEffectFlickDelay;
		}	
	}

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"MagicStone_EquipSuccess.ogg", false, false );
	return;
}



void CX2UISocketItem::SocketEquipReadyEffectStart()
{
	D3DXVECTOR3 vecEffectPos[_CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem];

	CX2SlotItem* pItemSlot = NULL;

	m_bMouseOverSoundCondition_1 = true;

	for(int i=0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++)
	{
		pItemSlot = (CX2SlotItem*)m_SlotList[i];

		vecEffectPos[i].x = pItemSlot->GetPos().x +  pItemSlot->GetSize().x / 2.0f;
		vecEffectPos[i].y = pItemSlot->GetPos().y +  pItemSlot->GetSize().y / 2.0f;

	}


	float ftime_latency = 0.f;

	for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
	{
		if( m_vecSocketSlotInfo[i].m_StateSlotState != SSS_NOT_AVAILABLE)
		{	

			if( m_hSocketSeq1[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{	
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSocketSeq1[i] );
			}

			m_hSocketSeq1[i] = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"UI_Socket_StartEffect03", vecEffectPos[i].x, vecEffectPos[i].y, 0 );
			CKTDGParticleSystem::CParticleEventSequence* pParticle1 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSocketSeq1[i] );
			if( pParticle1 != NULL )
			{
				pParticle1->SetOverUI( true );
				pParticle1->SetResolutionConvert(true);
			}

		}	
	}
	return;
}


void CX2UISocketItem::SocketEquipReadyEffectEnd()
{
	D3DXVECTOR3 vecEffectPos[_CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem];

	CX2SlotItem* pItemSlot = NULL;

	m_bMouseOverSoundCondition_1 = false;

	for(int i=0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++)
	{
		pItemSlot = (CX2SlotItem*)m_SlotList[i];

		vecEffectPos[i].x = pItemSlot->GetPos().x +  pItemSlot->GetSize().x / 2.0f;
		vecEffectPos[i].y = pItemSlot->GetPos().y +  pItemSlot->GetSize().y / 2.0f;

	}


	float ftime_latency = 0.f;

	for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
	{
		if( m_vecSocketSlotInfo[i].m_StateSlotState != SSS_NOT_AVAILABLE)
		{	
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSocketSeq1[i] );

		}
	}
	return;
}





wstring CX2UISocketItem::GetSlotItemDesc()
{
	//g_pKTDXApp->GetDeviceManager()->PlaySound( L"MagicStone_MouseOver.ogg", false, false );
	//m_bMouseOverSoundCondition_2 = true;
	wstring itemDesc=  L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if (m_pNowOverItemSlot->GetSlotID() == 4)
		{
			itemDesc = GetSlotItemDescByUID( m_SocketItemUID );
		}
	}

	return itemDesc;
}
// 소켓 강화에 연관되는 아이템인지 판별하는 함수
bool CX2UISocketItem::CheckSocketUseItem(UidType Uid)
{
	if( m_SocketItemUID == Uid )
	{
		return true;
	}
	
	for ( int i = 0; i < _CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem; i++ )
	{
		if( m_vecSocketSlotInfo[i].m_StoneUID == Uid )
		{	
			return true;
		}
	}
	return false;
}


#endif SERV_SOCKET_NEW
//}} oasis907 : 김상윤 [2010.4.5] //  소켓 강화 UI 개편
#endif

#ifdef ADD_SOCKET_SLOT
/** @function : GetSocketSlotNum
	@brief : 추가된 소켓 포함한 슬롯 수 얻기
*/
int CX2UISocketItem::GetSocketSlotNum( const CX2Item* pItem_ )
{
	int iSlotNum = -1;
	if( NULL != pItem_ )
	{
		iSlotNum = pItem_->GetItemTemplet()->GetSocketSlotNum();
		iSlotNum += pItem_->GetbyAddedSocketSlot();
	}
	else
	{
		iSlotNum = pItem_->GetItemTemplet()->GetSocketSlotNum();
	}

	return iSlotNum;
}
#endif // ADD_SOCKET_SLOT
