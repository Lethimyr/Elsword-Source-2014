#include "StdAfx.h"
#include ".\x2uiQuickSlot.h"

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26

namespace _CONST_QUICK_SLOT_
{
	const int MAX_QUICK_SLOT_NUM	= 6;	/// 최대 퀵슬롯 개수
	const int MAX_CARD_SLOT_NUM		= 4;	/// 어둠의 문 최대 카드 슬롯 개수
}

#endif // SERV_NEW_DEFENCE_DUNGEON

CX2UIQuickSlot::CX2UIQuickSlot( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2UIDragable( pNowStage, NULL ),
m_bShow(false),
m_bEnable(true),
m_pDLGQuickSlot(NULL),
m_bWaiting_EGS_USE_QUICK_SLOT_ACK( false )
#ifdef NEW_ITEM_NOTICE
, m_pDLGItemHighLight(NULL)
#endif //NEW_ITEM_NOTICE
#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-25
, m_bIsSummonCardSlot( false )	/// 어둠의 문 몬스터 카드용 퀵슬롯 적용 여부
#endif //SERV_NEW_DEFENCE_DUNGEON
//m_mapQuickSlotItemCoolTime;
{

	RegisterLuaBind();
	
	m_pDLGQuickSlot = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Quick_Slot.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGQuickSlot );
	//{{ 오현빈 // 2012-04-12 // 퀵슬롯 쿨타임 숫자 표기 추가
#ifdef SHOW_QUICKSLOT_COOLTIME
	CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"QuickSlotCoolTime" );		
	if( NULL != pStaticCoolTime )
	{
		for(int i=0; i< 5; i++)
		{
			if( NULL != pStaticCoolTime->GetString(i) )
			{
				pStaticCoolTime->GetString(i)->msg = L"";
			}
		}
	}
#endif //SHOW_QUICKSLOT_COOLTIME
	//}} 오현빈 // 2012-04-12 // 퀵슬롯 쿨타임 숫자 표기 추가

	CKTDGUIStatic* pStatic_Black = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Slot_Black" );
#ifdef	SERV_EXPAND_QUICK_SLOT
	if(NULL != pStatic_Black)
	{
		pStatic_Black->SetShow( true );
	}
#else
	if(NULL != pStatic_Black)
	{
		pStatic_Black->SetShow( false );
	}
#endif  SERV_EXPAND_QUICK_SLOT

#ifdef NEW_ITEM_NOTICE
	m_pDLGItemHighLight = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_QuickSlot_HighLight.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemHighLight );
	m_pDLGItemHighLight->SetShow(false);
#endif //NEW_ITEM_NOTICE

	//{{ JHKang / 강정훈 / QuickSlot의 Layer 수준이 계속 증가하는 것 방지
#ifdef FIX_TOOLTIP
	SetLayer( XDL_NORMAL_3 );
#else	
	SetLayer((X2_DIALOG_LAYER)m_pDLGQuickSlot->GetLayer());
#endif FIX_TOOLTIP
	//}} JHKang / 강정훈 / QuickSlot의 Layer 수준이 계속 증가하는 것 방지

	UpdateSlotKey();
}

CX2UIQuickSlot::~CX2UIQuickSlot(void)
{
	SAFE_DELETE_DIALOG(m_pDLGQuickSlot);
#ifdef NEW_ITEM_NOTICE
	SAFE_DELETE_DIALOG(m_pDLGItemHighLight);
#endif //NEW_ITEM_NOTICE
}

// bool CX2UIQuickSlot::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 
// 	switch( uMsg )
// 	{
// 	case WM_LBUTTONUP:
// 		{
// 			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
// 			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
// 
// 			MouseUp( mousePos );
// 		}
// 		break;
// 
// 	case WM_LBUTTONDOWN:
// 		{
// 			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
// 			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
// 
// 			if ( MouseDown( mousePos) == true )
// 				return true;
// 		}
// 		break;
// 
// 	case WM_MOUSEMOVE:
// 		{
// 
// 			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
// 			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
// 			m_NowMousePos = mousePos;
// 
// 			MouseMove( mousePos );
// 		}
// 		break;
// 
// 	}
// 
// }





//bool CX2UIQuickSlot::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	switch(wParam)
//	{
//	}
//	return false;
//}




/*virtual*/ HRESULT CX2UIQuickSlot::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();


	CX2ItemSlotManager::OnFrameMove(fTime, fElapsedTime);

	if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
	{
		if( g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetDialogShow() )
		{
			SetEnable(false);
		}
		else
		{
			SetEnable(true);
		}
	}

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
	case CX2Main::XS_VILLAGE_MAP:
		break;

	default:
		return S_OK;
	}
	
	if( NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return S_OK;
	}


	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-25
	int iSlotMaxNum = GetSlotMaxNum();

	if ( true == GetIsSummonCardSlot() )	/// 어둠의 문이면, 소환 카드 슬롯 출력을 위해 4개만 연산
		iSlotMaxNum = _CONST_QUICK_SLOT_::MAX_CARD_SLOT_NUM;

	for ( int i = 0; i < iSlotMaxNum; i++ )
#else // SERV_NEW_DEFENCE_DUNGEON
	for ( int i = 0; i < GetSlotMaxNum(); i++ )
#endif // SERV_NEW_DEFENCE_DUNGEON
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*) GetSlot( i );
		if ( pSlotItem == NULL )
			continue;
//퀵슬롯에 아이템 있을때 알파값 1.0으로 수정

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-25
		CKTDGUIStatic* pStaticSlot = NULL;

		if ( true == GetIsSummonCardSlot() )	/// 어둠의 문 이라면, 카드 슬롯을 가져온다.
			pStaticSlot = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"CardSlot" );
		else									/// 그 외엔 일반 아이템 슬롯을 가져온다.
			pStaticSlot = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Slot" );
#else // SERV_NEW_DEFENCE_DUNGEON
			CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Slot" );
#endif // SERV_NEW_DEFENCE_DUNGEON

		if ( NULL != pStaticSlot )
			pStaticSlot->GetPicture(i)->SetColor(D3DXCOLOR(1.f,1.f,1.f,1.f));

		if( pSlotItem->GetDialog() == NULL )
		{
//{{ 오현빈 // 2012-04-12 // 퀵슬롯 쿨타임 숫자 표기 추가
#ifdef SHOW_QUICKSLOT_COOLTIME
			//퀵슬롯에 아이템 없을 때 쿨타임 숫자 제거
			if( NULL != m_pDLGQuickSlot )
			{
				CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"QuickSlotCoolTime" );						
				if( NULL != pStaticCoolTime && NULL != pStaticCoolTime->GetString(i) )
				{
					pStaticCoolTime->GetString(i)->msg=L"";
				}
				//퀵슬롯에 아이템 없을 때 알파값 0.5로 수정
				if ( NULL != pStaticSlot )
					pStaticSlot->GetPicture(i)->SetColor(D3DXCOLOR(1.f,1.f,1.f,0.5f));
			}
#endif //SHOW_QUICKSLOT_COOLTIME
	//}} 오현빈 // 2012-04-12 // 퀵슬롯 쿨타임 숫자 표기 추가
			continue;
		}

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-11
		CX2Item* pItem = NULL;

		if ( true == GetIsSummonCardSlot() )	/// 어둠의 문 일 때, 어둠의 문 카드 슬롯 아이템 객체를 받아온다.
		{
			pItem = kInventory.GetItem( CX2Inventory::ST_E_DEFENCE_QUICK_SLOT, i );
		}
		else
		{
			pItem = kInventory.GetItem( pSlotItem->GetItemUID() );
		}
#else // SERV_NEW_DEFENCE_DUNGEON
		CX2Item* pItem = kInventory.GetItem( pSlotItem->GetItemUID() );
#endif // SERV_NEW_DEFENCE_DUNGEON
		if( NULL == pItem ||
			NULL == pItem->GetItemTemplet() 
            )
		{
			continue;
		}
#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
		OnFrameMove_CoolTime( pItem, pSlotItem );
	}
#else
		int itemID = pItem->GetItemData().m_ItemID;
		bool bCheck = false;

		map < int, CKTDXTimer >::iterator mit;
		for ( mit = m_mapQuickSlotItemCoolTime.begin(); mit != m_mapQuickSlotItemCoolTime.end(); mit++ )
		{
			if ( mit->first == itemID )
			{

				CKTDGUIStatic* pStaticItem = (CKTDGUIStatic*)pSlotItem->GetDialog()->GetControl( L"StaticRoot_ItemSlot" );
				if ( pStaticItem != NULL && pStaticItem->GetPicture(1) != NULL )
				{
					CKTDGUIControl::CPictureData* pPicture = pStaticItem->GetPicture(1);

					float fItemCoolTimeElapsedTime = (float)mit->second.elapsed();
					if ( fItemCoolTimeElapsedTime >= (float)pItem->GetItemTemplet()->GetCoolTime() )
					{
						bCheck = false;
						fItemCoolTimeElapsedTime = 0.0f;
					}
					else
					{
						bCheck = true;
						fItemCoolTimeElapsedTime = (float)pItem->GetItemTemplet()->GetCoolTime() - fItemCoolTimeElapsedTime;
						pPicture->SetShow( true );
					}

					pPicture->SetPos( D3DXVECTOR2( pPicture->GetOriginalPos().x, pPicture->GetOriginalPos().y + pPicture->GetOriginalSize().y - ( pPicture->GetOriginalSize().y * (  fItemCoolTimeElapsedTime / pItem->GetItemTemplet()->GetCoolTime()  ) ) ) );
					pPicture->SetSize( D3DXVECTOR2( pPicture->GetOriginalSize().x, pPicture->GetOriginalSize().y * (  fItemCoolTimeElapsedTime / pItem->GetItemTemplet()->GetCoolTime()  )  )  );

					//재사용 대기시간 숫자 설정
					CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*) pSlotItem->GetDialog()->GetControl( L"CoolTime" );					
					if( NULL != pStaticCoolTime )
					{
						WCHAR wBuf1[10];
						StringCchPrintfW( wBuf1, 10, L"%d", static_cast<int>(fItemCoolTimeElapsedTime)  );
						pStaticCoolTime->GetString(0)->msg = wBuf1;
					}
					break;
				}
			}
		}

		if ( bCheck == false )
		{
			if ( m_mapQuickSlotItemCoolTime.end() != mit )
				m_mapQuickSlotItemCoolTime.erase( mit );

			CKTDGUIStatic* pStaticItem = (CKTDGUIStatic*)pSlotItem->GetDialog()->GetControl( L"StaticRoot_ItemSlot" );
			if ( pStaticItem != NULL && pStaticItem->GetPicture(1) != NULL )
			{
				CKTDGUIControl::CPictureData* pPicture = pStaticItem->GetPicture(1);

				if ( pPicture->GetShow() == true )
				{
					//퀵슬롯을 다시 쓸 수 있다는 표시
					float fEffectPosX = (pSlotItem->GetDialog()->GetPos().x + ( pPicture->GetOriginalSize().x/2 )) * g_pKTDXApp->GetResolutionScaleX() ;
					float fEffectPosY = (pSlotItem->GetDialog()->GetPos().y + ( pPicture->GetOriginalSize().y/2 )) * g_pKTDXApp->GetResolutionScaleY() ;
					CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"UI_ItemBox_enable_P01",
						fEffectPosX, fEffectPosY, 0, 100, 100, true, 5, 10 );

					if ( NULL != pParticle )
						pParticle->SetOverUI( true );

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-25
					if( true == m_bIsSummonCardSlot )
					{
#ifdef CHECK_INHERIT_STATE_MENU
						if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
						{
							CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
							if( pStateMenu != NULL )
							{
								CX2State::MouseClickUI::MOUSE_CLICK_STYLE arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP_NORMAL;
								D3DXVECTOR2 vPos = D3DXVECTOR2(107, 763);
								pStateMenu->CreateMouseClickUI( (CKTDXStage*) pStateMenu, vPos, 0.5f, 1000, arrowStyle );
							}
						}
					}
#endif //SERV_NEW_DEFENCE_DUNGEON
				}
				pPicture->SetShow( false );
				//재사용 대기시간 숫자 설정
				CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*) pSlotItem->GetDialog()->GetControl( L"CoolTime" );			
				if( NULL != pStaticCoolTime )
				{
					pStaticCoolTime->GetString(0)->msg=L"";
				}				
			}
		}
	}
	CX2GageManager::GetInstance()->SetMyQuickSlotCoolTimeList( m_mapQuickSlotItemCoolTime );
#endif // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
	KeyEventProcess();
	return S_OK;
}




bool CX2UIQuickSlot::KeyEventProcess()
{
	KTDXPROFILE();

	if( false == m_bEnable )
		return false;

	if( true == m_bWaiting_EGS_USE_QUICK_SLOT_ACK )
		return false;

	CX2GUUser* pMyGUUser = NULL;

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if ( NULL == g_pX2Game )
				return false;

			if( CX2Game::GS_PLAY != g_pX2Game->GetGameState() )
				return false;


			pMyGUUser = g_pX2Game->GetMyUnit();

			if ( NULL == pMyGUUser )
				return false;

			// 죽은 상태거나 Hp가 0보다 크지 않으면
			if ( CX2GameUnit::GUSI_DIE == pMyGUUser->GetGameUnitState() || !( pMyGUUser->GetNowHp() > 0 ) )
			{
				//g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );
				return false;
			}

#ifdef KEYFRAME_CAMERA
			// oasis907 : 김상윤 [2010.12.28] // 자유시점 카메라에서 퀵슬롯 사용 불가하게 - 카메라 연출 위해
			if ( g_pX2Game->GetFreeCamera() == true)
				return false;
			
			if ( g_pX2Game->GetX2Camera() != NULL && g_pX2Game->GetX2Camera()->GetEnableWorldCamera() == true )
				return false;
#endif KEYFRAME_CAMERA

			if( false == g_pX2Game->CheckEnableKeyProcess() )
				return false;

		} break;

// 	case CX2Main::XS_VILLAGE_MAP:
// 		break;

	default:
		return false;
	}

	// 아이템 버리기의 수량성 입력 창이 오픈되있는 상태면 퀵슬롯 사용 금지
	if ( g_pData->GetUIManager()->GetUIInventory()->GetOpenRegisterQuantityDLG() == true )
		return false;
	
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	CX2Item* pQuickItem = NULL;
	int slotID = -1;

	#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-07

		/// 어둠의 문 이라면, 몬스터 소환 카드 퀵슬롯을 사용
		if ( true == GetIsSummonCardSlot() )
		{
			InputSummonCardSlotKey(  kInventory, &pQuickItem, slotID );	/// 몬스터 소환 카드 슬롯 키보드 입력 함수
		}
		/// 일반 퀵슬롯 입력
		else
		{
			InputQuickSlotKey( kInventory, &pQuickItem, slotID );		/// 퀵슬롯 키보드 입력 함수
		}

	#else // SERV_NEW_DEFENCE_DUNGEON

		if( GET_KEY_STATE( GA_QUICKSLOT1 ) == TRUE )
		{
			pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 0 );
			slotID = 0;
		}
		else if( GET_KEY_STATE( GA_QUICKSLOT2 ) == TRUE )
		{
			pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 1 );
			slotID = 1;
		}
		else if( GET_KEY_STATE( GA_QUICKSLOT3 ) == TRUE )
		{
			pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 2 );
			slotID = 2;
		}
		else if( GET_KEY_STATE( GA_QUICKSLOT4 ) == TRUE )
		{
			pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 3 );
			slotID = 3;
		}
		else if( GET_KEY_STATE( GA_QUICKSLOT5 ) == TRUE )
		{
			pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 4 );
			slotID = 4;
		}
		else if( GET_KEY_STATE( GA_QUICKSLOT6 ) == TRUE )
		{
			pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 5 );
			slotID = 5;
		}

	#endif // SERV_NEW_DEFENCE_DUNGEON


#ifdef PET_ITEM_GM
	if ( pQuickItem != NULL && pQuickItem->GetItemTemplet() != NULL )
	{	
		if( pQuickItem->GetItemTemplet()->GetItemID() == PET_ATTACK_SKILL_GM_ITEM_ID || pQuickItem->GetItemTemplet()->GetItemID() == PET_CHEER_SKILL_GM_ITEM_ID )
		{
			if( g_pData != NULL && g_pData->GetPetManager() != NULL )
			{
				CX2PET *pPet = g_pData->GetPetManager()->GetMyPet();
				if( pPet != NULL && pPet->GetXSkinAnim() != NULL )
				{
//#ifdef PET_ITEM_GM
//					if ( pQuickItem->GetItemTemplet()->m_ItemID == PET_ATTACK_SKILL_ITEM_ID || pQuickItem->GetItemTemplet()->m_ItemID == PET_CHEER_SKILL_ITEM_ID  )
//#endif //PET_ITEM_GM
//					{
//						if( pPet->GetNowMP() < 150.0f )
//						{
//							wstring wstrMsg = GET_STRING( STR_ID_19150 );
//							g_pChatBox->AddChatLog( wstrMsg.c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR( 1,1,0,1 ), L"#CFFFF00" );
//							return false;
//						}
//					}
				}
				else
				{
					g_pChatBox->AddChatLog( NetError::GetErrStr(NetError::ERR_PET_06), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR( 1,1,0,1 ), L"#CFFFF00" );
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
#endif //PET_ITEM_GM

	if ( pQuickItem != NULL && 
        pQuickItem->GetItemTemplet() != NULL 
        )
	{
#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
		const int iGroupID = g_pData->GetItemManager()->GetCoolTimeGroupID( pQuickItem->GetItemData().m_ItemID );
		std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( iGroupID );
		if( mit != m_mapQuickSlotItemCoolTime.end() )
		{
			if( mit->second.elapsed() < pQuickItem->GetItemTemplet()->GetCoolTime() )   // TODO : 쿨타임 정책 정해지면 아이템 템플릿에서 쿨타임을 추출하자.
			{
				if ( slotID != -1 )
					return true;
				else
					return false;
			}
		}
#else
		std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( pQuickItem->GetItemData().m_ItemID );
		if( mit != m_mapQuickSlotItemCoolTime.end() )
		{
			if( mit->second.elapsed() < pQuickItem->GetItemTemplet()->GetCoolTime() )   // TODO : 쿨타임 정책 정해지면 아이템 템플릿에서 쿨타임을 추출하자.
			{
				if ( slotID != -1 )
					return true;
				else
					return false;
			}
		}
#endif // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
		bool bAllowUseQuickSlotItem = true;
		if ( pQuickItem->GetItemTemplet() != NULL )
		{
            unsigned uNumSA = pQuickItem->GetItemTemplet()->GetNumSpecialAbility();
			for( int i = 0; i < (int)uNumSA; i++ )
			{
				const CX2Item::SpecialAbility* pSa = &pQuickItem->GetItemTemplet()->GetSpecialAbility(i);
				switch( pSa->GetType() )
				{
				case CX2Item::SAT_SPECIAL_SKILL:
					{
						if ( NULL != pMyGUUser )
						{
#ifdef RIDING_SYSTEM		/// 탈것 탑승시에는 투척 아이템 사용할 수 없도록 설정
#ifndef FIX_RIDINGPET_ITEM_SLOT
							if( NULL == g_pX2Game  )
								false;

							if( NULL != CX2RidingPetManager::GetInstance() && true == CX2RidingPetManager::GetInstance()->GetRidingOn() )
							{
								g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_24528 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
								bAllowUseQuickSlotItem = false;
							}
#endif //FIX_RIDINGPET_ITEM_SLOT
#endif // RIDING_SYSTEM

#ifndef SPECIAL_USE_ITEM
							if ( pMyGUUser->GetUnit()->GetUnitTemplet()->m_UnitType == CX2Unit::UT_ELSWORD ) // question?? 왜 엘소드만?
#endif SPECIAL_USE_ITEM
							{
#ifdef SPECIAL_USE_ITEM
								if ( pMyGUUser->GetNowFrameData().stateParam.bLandConnect == true && g_pX2Game->GetEnableAllKeyProcess() == true)
#else
								if ( pMyGUUser->GetNowFrameData().stateParam.bLandConnect == true )
#endif SPECIAL_USE_ITEM
								{
#ifdef FIX_QUICK_SLOT_USE_DUNGEON_PLAY
									if( NULL != g_pX2Game && 
										CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() && // 던전 게임 중
										NULL != g_pX2Game->GetMyUnit() &&
										false == g_pX2Game->GetMyUnit()->GetStarted() ) // 게임 시작 상태가 아니면 퀵슬롯 사용 막기
 									{
 										bAllowUseQuickSlotItem = false;
 									}
#ifdef RIDING_SYSTEM
#ifdef FIX_RIDINGPET_ITEM_SLOT
									else if( NULL != CX2RidingPetManager::GetInstance() && true == CX2RidingPetManager::GetInstance()->GetRidingOn() )
									{
										if(  NULL != g_pX2Game )
											g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_24528 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
										bAllowUseQuickSlotItem = false;
									}
#endif //FIX_RIDINGPET_ITEM_SLOT
#endif //RIDING_SYSTEM
 									else
#endif FIX_QUICK_SLOT_USE_DUNGEON_PLAY
										if ( pMyGUUser->GetNowFrameData().syncData.nowState == pMyGUUser->GetWaitStateID() ||
											pMyGUUser->GetNowFrameData().syncData.nowState == pMyGUUser->GetWalkStateID() ||
											pMyGUUser->GetNowFrameData().syncData.nowState == pMyGUUser->GetDashStateID() ||
#ifdef SPECIAL_USE_ITEM
											pMyGUUser->GetNowFrameData().syncData.nowState == pMyGUUser->GetChargeMpStateID() ||
#endif SPECIAL_USE_ITEM
											pMyGUUser->GetNowFrameData().syncData.nowState == pMyGUUser->GetDashEndStateID() )
										{
											m_bWaiting_EGS_USE_QUICK_SLOT_ACK = true;
											g_pX2Game->SetEnableAllKeyProcess( false );
										}
										else
										{
											bAllowUseQuickSlotItem = false;
										}
								}
							}
						}
					}
					break;
			//{{ mauntain : 김태환 [2012.06.19] 몬스터 카드 소환 기능 - User가 시작되지 않았을 때 소환 막기
#ifdef SUMMON_MONSTER_CARD_SYSTEM
				case CX2Item::SAT_SUMMON_MONSTER:
					{
						switch( g_pX2Game->GetGameType() )
						{
						case CX2Game::GT_DUNGEON:
							{
								if( NULL != g_pX2Game->GetMyUnit() &&
									false == g_pX2Game->GetMyUnit()->GetStarted() )
								{
									bAllowUseQuickSlotItem = false;
								}
								else
								{
									m_bWaiting_EGS_USE_QUICK_SLOT_ACK = true;
								}
							}
							break;
						default:
							{
								g_pChatBox->AddChatLog( GET_STRING(STR_ID_20120),
									KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
								bAllowUseQuickSlotItem = false;
							}
							break;
						}
					}
					break;
#endif SUMMON_MONSTER_CARD_SYSTEM
			//}}
#ifdef RIDING_SYSTEM	
				case CX2Item::SAT_PEPPER_RUN:
					{
						/// 탈것 탑승시에는 청양 고추 사용할 수 없도록 설정
						if( NULL != g_pX2Game && NULL != CX2RidingPetManager::GetInstance() && 
							true == CX2RidingPetManager::GetInstance()->GetRidingOn() )
						{
							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_813 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
							bAllowUseQuickSlotItem = false;
						}
					} 
					break;
#endif // RIDING_SYSTEM
#ifdef COOLTIME_SHARE_GROUP_ITEM
				default:
					{
						m_bWaiting_EGS_USE_QUICK_SLOT_ACK = true;
					}
					break;
#endif COOLTIME_SHARE_GROUP_ITEM
				}
			}
		}

		if ( bAllowUseQuickSlotItem == true )
		{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-11
			if ( true == GetIsSummonCardSlot() )	/// 어둠의 문 이라면, 어둠의 문 카드 사용 요청을 한다.
				Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ( slotID );
			else
#endif // SERV_NEW_DEFENCE_DUNGEON
				Handler_EGS_USE_QUICK_SLOT_REQ( slotID );
		}
	}

	if ( slotID != -1 )
		return true;


	return false;
}








bool CX2UIQuickSlot::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case EGS_USE_QUICK_SLOT_ACK:
		return Handler_EGS_USE_QUICK_SLOT_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_USE_QUICK_SLOT_NOT:
		return Handler_EGS_USE_QUICK_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		break;

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-11
	case EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK:
		return Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK( hWnd, uMsg, wParam, lParam );
		break;
#endif // SERV_NEW_DEFENCE_DUNGEON
	}
	return false;
}






//////////////////////////////////////////////////////////////////////////
bool CX2UIQuickSlot::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem )
{
	if ( pFromCX2SlotItem == NULL || pToCX2SlotItem == NULL )
		return false;

	if ( pFromCX2SlotItem->GetSortType() == pToCX2SlotItem->GetSortType() && pFromCX2SlotItem->GetSlotID() == pToCX2SlotItem->GetSlotID() )
		return false;

	if(g_pData->GetUIManager()->GetUIInventory() == NULL)
	{
		ASSERT( !"UIInventory NULL" );	// 최초 생성 후 안 사라져야 한다..
		g_pData->GetUIManager()->CreateUIInventory();
	}
	int NowInvenSortTypePageNum = g_pData->GetUIManager()->GetUIInventory()->GetNowInvenSortTypePageNum();

	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ;

	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType	= pFromCX2SlotItem->GetSortType();
	if ( pFromCX2SlotItem->GetSlotType() == CX2Slot::ST_INVENTORY )
	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID() + (( NowInvenSortTypePageNum - 1 ) * MY_INVENTORY_SHOW_MAX_SLOT_NUM);
		//{{ kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체
#ifdef REAL_TIME_ELSWORD
		UpdateCoolTime( pFromCX2SlotItem->GetItemTID() );
#endif REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체
	}
	else
	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID();
	}


	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType	= pToCX2SlotItem->GetSortType();

	if ( pToCX2SlotItem->GetSlotType() == CX2Slot::ST_INVENTORY )
	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= pToCX2SlotItem->GetSlotID() + (( NowInvenSortTypePageNum - 1 ) * MY_INVENTORY_SHOW_MAX_SLOT_NUM);
	}
	else
	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= pToCX2SlotItem->GetSlotID();
	}

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



bool CX2UIQuickSlot::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID )
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



bool CX2UIQuickSlot::Handler_EGS_USE_QUICK_SLOT_REQ( int slotID )
{

	if(m_bEnable)
	{
#ifdef CHRISTMAS_EVENT_2013
		CX2GUUser* pCX2GUUser = g_pX2Game->GetMyUnit();
		if( pCX2GUUser != NULL )
		{
			if( -1 != pCX2GUUser->AccessSummonMonsterCardData().GetSummonMonsterUID() )
			{
				CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
				CX2Item* pItem = pMyUnit->GetInventory().GetItem( CX2Inventory::ST_E_QUICK_SLOT, slotID );
				int iCoolTimeGroupID = g_pData->GetItemManager()->GetCoolTimeGroupID( pItem->GetItemTemplet()->GetItemID() );

				if( iCoolTimeGroupID == 7 )
				{
					m_bWaiting_EGS_USE_QUICK_SLOT_ACK = false;

					return true;
				}
			}
		}
#endif CHRISTMAS_EVENT_2013

		KEGS_USE_QUICK_SLOT_REQ kPacket;
		kPacket.m_sSlotID = slotID;

		g_pData->GetServerProtocol()->SendPacket( EGS_USE_QUICK_SLOT_REQ, kPacket ); 
		g_pMain->AddServerPacket( EGS_USE_QUICK_SLOT_ACK );

		return true;
	}	
	return false;
}





bool CX2UIQuickSlot::Handler_EGS_USE_QUICK_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_QUICK_SLOT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_USE_QUICK_SLOT_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( kEvent.m_ItemID == -1 )
			{
				return true;
			}

			g_pKTDXApp->GetDeviceManager()->PlaySound( L"UseItem.ogg", false, false );

			KInventoryItemInfo& kInventorySlotInfo = kEvent.m_InventorySlotInfo;

			CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
			pMyUnit->AccessInventory().RemoveItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );

			if ( kInventorySlotInfo.m_iItemUID > 0 )
			{
				CX2Item::ItemData kItemData( kInventorySlotInfo );
				pMyUnit->AccessInventory().AddItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID, kItemData );
			}

			ResetQuickSlotUI();
#ifdef COOLTIME_SHARE_GROUP_ITEM
			int iCoolTimeGroupID = g_pData->GetItemManager()->GetCoolTimeGroupID( kEvent.m_ItemID );
#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
			// 쿨타임을 ItemID에서 그룹ID기준으로 변경
			if( iCoolTimeGroupID > 0 )
			{
				std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( iCoolTimeGroupID );
				if( mit != m_mapQuickSlotItemCoolTime.end() )
				{
					mit->second.restart();
				}
				else
				{
					CKTDXTimer kTimer;
					m_mapQuickSlotItemCoolTime.insert( std::make_pair( iCoolTimeGroupID, kTimer ) );
				}
			}
#else
			if( iCoolTimeGroupID > 0 )
			{
				vector< int > vecCoolTimeGroupItemIDList;
				g_pData->GetItemManager()->GetCoolTimeGroupItemIDList( iCoolTimeGroupID, vecCoolTimeGroupItemIDList );
				for ( int i = 0; i < (int)vecCoolTimeGroupItemIDList.size(); i++ )
				{
					std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( vecCoolTimeGroupItemIDList[i] );
					if( mit != m_mapQuickSlotItemCoolTime.end() )
					{
						mit->second.restart();
					}
					else
					{
						CKTDXTimer kTimer;
						m_mapQuickSlotItemCoolTime.insert( std::make_pair( vecCoolTimeGroupItemIDList[i], kTimer ) );
					}
				}
			}
			else
			{
				std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( kEvent.m_ItemID );
				if( mit != m_mapQuickSlotItemCoolTime.end() )
				{
					mit->second.restart();
				}
				else
				{
					CKTDXTimer kTimer;
					m_mapQuickSlotItemCoolTime.insert( std::make_pair( kEvent.m_ItemID, kTimer ) );
				}
			}
#endif // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP

#else
			std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( kEvent.m_ItemID );
			if( mit != m_mapQuickSlotItemCoolTime.end() )
			{
				mit->second.restart();
			}
			else
			{
				CKTDXTimer kTimer;
				m_mapQuickSlotItemCoolTime.insert( std::make_pair( kEvent.m_ItemID, kTimer ) );
			}
#endif COOLTIME_SHARE_GROUP_ITEM

			switch ( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP:
				{
                    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEvent.m_ItemID );
					if ( NULL != pItemTemplet )
					{
						CX2GageData* pMyGageData = CX2GageManager::GetInstance()->GetMyGageData();

						if ( NULL != pMyGageData )
							pMyGageData->UseSpecialAbilityInVillage( pItemTemplet );

						if( NULL != g_pChatBox )
						{
							wstring wstrColor = L"#CECEC88";							// (236, 236, 136)
							D3DXCOLOR coTextColor(0.92549f, 0.92549f, 0.53333f, 1.f);	// (236, 236, 136)

							g_pChatBox->AddChatLog( L"사용", KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
						}
					}
				} break;

			default:
				break;
			}
			
		}
		
		m_bWaiting_EGS_USE_QUICK_SLOT_ACK = false;		

		if( NULL != g_pX2Game )
			g_pX2Game->SetEnableAllKeyProcess( true );

	}

	return true;
}


bool CX2UIQuickSlot::Handler_EGS_USE_QUICK_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_QUICK_SLOT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEvent.m_ItemID );
	if ( pItemTemplet == NULL )
		return true;

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if( NULL == g_pX2Game )
				return true;

			CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( kEvent.m_UnitUID );
			if ( pGUUser == NULL )
				return true;

#ifdef PET_ITEM_GM
			if( pItemTemplet->GetItemID() == PET_ATTACK_SKILL_GM_ITEM_ID || pItemTemplet->GetItemID() == PET_CHEER_SKILL_GM_ITEM_ID )
				g_pX2Game->UseItemPetAbility( pItemTemplet, pGUUser );
#endif //PET_ITEM_GM

			if( 0 < pItemTemplet->GetNumSpecialAbility() )
				g_pX2Game->UseItemSpecialAbility( pItemTemplet, pGUUser );

			if ( 0 < pItemTemplet->GetNumBuffFactorPtr() )
				g_pX2Game->ApplyBuffFactorToGUUser( pItemTemplet, pGUUser );

			wstring wstrBuff;
			if ( pGUUser->IsMyUnit() == false )
			{
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
				wstrBuff = GET_REPLACED_STRING( ( STR_ID_895, "SSS", pGUUser->GetUnit()->GetNickName(), pItemTemplet->GetFullName_(),
					L"" ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
				wstrBuff = GET_REPLACED_STRING( ( STR_ID_895, "SSI", pGUUser->GetUnit()->GetNickName(), pItemTemplet->GetFullName_(),
					KHanSungPostWordUnicodeWrapper( ( WCHAR* ) pItemTemplet->GetFullName_(), STR_ID_198, STR_ID_199 ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
			}
			else
			{ 
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
				wstrBuff = GET_REPLACED_STRING( ( STR_ID_896, "SS", pItemTemplet->GetFullName_(),
					L"" ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
				wstrBuff = GET_REPLACED_STRING( ( STR_ID_896, "SI", pItemTemplet->GetFullName_(),
					KHanSungPostWordUnicodeWrapper( ( WCHAR* ) pItemTemplet->GetFullName_(), STR_ID_198, STR_ID_199 ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
			}


			if( NULL != g_pChatBox )
			{
				wstring wstrColor = L"#CECEC88";							// (236, 236, 136)
				D3DXCOLOR coTextColor(0.92549f, 0.92549f, 0.53333f, 1.f);	// (236, 236, 136)

				g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
			}
		} break;

	default:
		{
			ASSERT( !L"Can not use this item in this State" );
			return true;
		} break;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////

//{{ kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체
#ifdef	REAL_TIME_ELSWORD
void CX2UIQuickSlot	::UpdateCoolTime( int itemID )
{
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		break;

	default:
		return;
		break;
	}
	
#ifdef COOLTIME_SHARE_GROUP_ITEM
	int iCoolTimeGroupID = g_pData->GetItemManager()->GetCoolTimeGroupID( itemID );
#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
	if ( iCoolTimeGroupID > 0 )
	{
		std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( iCoolTimeGroupID );
		if( mit != m_mapQuickSlotItemCoolTime.end() )
		{
			mit->second.restart();
		}
		else
		{
			CKTDXTimer kTimer;
			m_mapQuickSlotItemCoolTime.insert( std::make_pair( iCoolTimeGroupID, kTimer ) );
		}
	}
#else
	if ( iCoolTimeGroupID > 0 )
	{
		vector< int > vecCoolTimeGroupItemIDList;
		g_pData->GetItemManager()->GetCoolTimeGroupItemIDList( iCoolTimeGroupID, vecCoolTimeGroupItemIDList );
		for ( int i = 0; i < (int)vecCoolTimeGroupItemIDList.size(); i++ )
		{
			std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( vecCoolTimeGroupItemIDList[i] );
			if( mit != m_mapQuickSlotItemCoolTime.end() )
			{
				mit->second.restart();
			}
			else
			{
				CKTDXTimer kTimer;
				m_mapQuickSlotItemCoolTime.insert( std::make_pair( vecCoolTimeGroupItemIDList[i], kTimer ) );
			}
		}
	}
	else
	{
		std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( itemID );

		if( mit != m_mapQuickSlotItemCoolTime.end() )
		{
			mit->second.restart();
		}
		else
		{
			CKTDXTimer kTimer;
			m_mapQuickSlotItemCoolTime.insert( std::make_pair( itemID, kTimer ) );
		}
	}
#endif // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP

#else
	std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( itemID );

	if( mit != m_mapQuickSlotItemCoolTime.end() )
	{
		mit->second.restart();
	}
	else
	{
		CKTDXTimer kTimer;
		m_mapQuickSlotItemCoolTime.insert( std::make_pair( itemID, kTimer ) );
	}
#endif COOLTIME_SHARE_GROUP_ITEM
}

bool CX2UIQuickSlot::ResetCoolTime()
{
#ifdef FIX_COOLTIME_NOT_INIT_BUG
	m_mapQuickSlotItemCoolTime.clear();
#else
	
	if ( NULL == g_pX2Game ||
		 NULL == g_pX2Game->GetMyUnit() ||
		 NULL == g_pX2Game->GetMyUnit()->GetUnit() )
		 return false;
	const CX2Inventory& kInventory = &g_pX2Game->GetMyUnit()->GetUnit()->GetInventory();

	CX2Item* pQuickItem = NULL;
#ifdef SPECIAL_USE_ITEM
	for ( int i = 0; i < 6; i++ )
#else
	for ( int i = 0; i < 5; i++ )
#endif SPECIAL_USE_ITEM
	{
		pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, i );

		if ( pQuickItem != NULL && 
            pQuickItem->GetItemTemplet() != NULL 
            )
		{
#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
			const int iItemID = pItem_->GetItemData().m_ItemID;
			const int iGroupID = g_pData->GetItemManager()->GetCoolTimeGroupID( iItemID );
			std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( iGroupID );
#else
			std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( pQuickItem->GetItemData().m_ItemID );
#endif // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
			if( mit != m_mapQuickSlotItemCoolTime.end() )
			{
				mit->second.ResetStartTime();
			}
		}

	}
#endif //FIX_COOLTIME_NOT_INIT_BUG

#ifdef FIX_QUICK_SLOT_USE_DUNGEON_PLAY
	g_pData->GetUIManager()->GetUIQuickSlot()->SetWaiting_EGS_USE_QUICK_SLOT_ACK( false );
#endif FIX_QUICK_SLOT_USE_DUNGEON_PLAY
	return true;
}
#endif	REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체

void CX2UIQuickSlot::ResetQuickSlotUI()
{
	if( g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
        return;

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

#ifdef NEW_ITEM_NOTICE 
	CKTDGUIStatic* pStatic = NULL;
	if( m_pDLGItemHighLight != NULL )
	{
		pStatic = (CKTDGUIStatic*)m_pDLGItemHighLight->GetControl(L"g_pStaticInvenHighLight");
	}
	if( NULL != pStatic )
	{
		for ( int slotIndex = 0; slotIndex < static_cast<int>(m_SlotList.size()); ++slotIndex)
		{//New Item 하이라이트 모두 끄기
			if( NULL != pStatic->GetPicture(slotIndex) )
			{
				pStatic->GetPicture(slotIndex)->SetShow(false);
			}
		}
	}
	bool bIsNew = false;
#endif //NEW_ITEM_NOTICE 

#ifdef SERV_NEW_DEFENCE_DUNGEON		/// 일반 퀵슬롯과 몬스터 카드 퀵슬롯일 때의 아이콘 설정 구분

	if( true == GetIsSummonCardSlot() )
		SetSummonMonsterItemIcon( m_SlotList, kInventory, pStatic );	/// 몬스터 카드 퀵슬롯 아이콘 설정
	else
		SetSlotItemIcon( m_SlotList, kInventory, pStatic );				/// 퀵슬롯 아이콘 설정

#else SERV_NEW_DEFENCE_DUNGEON

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		if( NULL == pItemSlot )
			continue;

		pItemSlot->DestroyItemUI();

		CX2Item* pItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, i );
		if ( pItem == NULL )
			continue;
	
#ifdef NEW_ITEM_NOTICE 
		{
			UidType iItemUID = pItem->GetItemData().m_ItemUID;
			bool	bNewItem = kInventory.IsNewItem( CX2Inventory::ST_QUICK_SLOT, iItemUID );

			if( true == bNewItem && NULL != pStatic &&  NULL != pStatic->GetPicture(i) )
			{//위에서 모두 초기화 시켜 주기 때문에, true일때만 수정
				pStatic->GetPicture(i)->SetShow(bNewItem);
			}
		}
#endif //NEW_ITEM_NOTICE 

		pItemSlot->CreateItemUI( pItem );	
		//(레이어 문제 때문에 배경 픽쳐와 다르게 슬롯에 기록)
		//단축키 번호 스트링 
		CKTDGUIStatic* pStaticHotKey = pItemSlot->GetDialog()->CreateStatic();	
		if( NULL != pStaticHotKey )
		{
			pStaticHotKey->SetName(L"StaticHotKey");
			pItemSlot->GetDialog()->AddControl(pStaticHotKey);
			WCHAR buff[32] = {0};
			CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
			pStaticHotKey->AddString( pString );

			GetQuickSlotStringByIndex( i , pString->msg );
			pString->fontIndex = XUF_DODUM_11_NORMAL;
			pString->fontStyle = CKTDGFontManager::FS_SHELL;
			pString->sortFlag = DT_RIGHT;
			pString->pos = D3DXVECTOR2 ( 26, 0);
			pString->color = D3DXCOLOR( 1, 1, 0, 1 );
			pString->outlineColor = D3DXCOLOR( 0.3f,0.2f,0.2f,1 );
		}

		//재사용 대기 시간 스트링
		CKTDGUIStatic* pStaticCoolTime = pItemSlot->GetDialog()->CreateStatic();	
		if( NULL != pStaticCoolTime )
		{
			pStaticCoolTime->SetName(L"CoolTime");
			pItemSlot->GetDialog()->AddControl(pStaticCoolTime);
			WCHAR buff[32] = {0};
			CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
			pStaticCoolTime->AddString( pString );

			pString->fontIndex = XUF_DODUM_20_BOLD;
			pString->fontStyle = CKTDGFontManager::FS_SHELL;
			pString->sortFlag = DT_CENTER;
			pString->pos = D3DXVECTOR2 (20, 10);
			pString->color = D3DXCOLOR( 1, 1, 0, 1 );
			pString->outlineColor = D3DXCOLOR( 0.3f,0.2f,0.2f,1 );
		}		
	}

#endif //SERV_NEW_DEFENCE_DUNGEON

	UpdateSlotKey();
}




void CX2UIQuickSlot::SetLayer(X2_DIALOG_LAYER layer)
{
#ifdef FIX_TOOLTIP
	//아이템이미지가 배경이미지 뒤로 가는 오류 수정
	SetSlotManagerLayer( layer + 1 );
	SetSlotManagerChangeSequence(true);

	m_pDLGQuickSlot->SetLayer( layer );

#else
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(true);

	m_pDLGQuickSlot->SetLayer(layer+1);
#endif FIX_TOOLTIP	

	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGQuickSlot, true );	// 이건 앞으로 당기자 =3=

#ifdef NEW_ITEM_NOTICE
	if( NULL != m_pDLGItemHighLight )
	{
		m_pDLGItemHighLight->SetLayer( layer + 3 );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGItemHighLight, false );
	}
#endif //NEW_ITEM_NOTICE
}

bool CX2UIQuickSlot::OnDropAnyItem( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
		return false;

	//{{ kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기
#ifdef	REAL_TIME_ELSWORD
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
				return false;

			if ( g_pX2Game->GetMyUnit()->GetNowHp() <= 0 )
			{
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );

				return false;
			}

			if ( g_pX2Game->CheckAndWarningBusyStateNow() )
				return false;

		} break;

	default:
		break;
	}
#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기

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

	const CX2Inventory& kInventory	= g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pItem				= NULL;
	pItem						= kInventory.GetItem( *m_DraggingItemUID, true );

	if ( pItem == NULL )
	{
		SetNowDragItemReturn();
		return false;
	}
	
	// 퀵슬롯에 올릴 수 있는 아이템이 맞나 확인하고
	if ( pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_QICK_SLOT )
	{
		// 인벤토리 또는 퀵슬롯에서 넘어온 건지도 확인하고
		if ( (*m_pSlotBeforeDragging)->GetSlotType() == CX2Slot::ST_INVENTORY 
			|| ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetSortType() == CX2Inventory::ST_E_QUICK_SLOT )
		{

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26
			if ( true == GetIsSummonCardSlot() )	/// 소환 카드 슬롯은 바꿀 수 없다.
				return false;
#endif //SERV_NEW_DEFENCE_DUNGEON

			// 바뀌어라 얍!
			Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)*m_pSlotBeforeDragging), pItemSlot );

			CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) *m_pSlotBeforeDragging;
			pkSlotBeforeDragging->DestroyItemUI();
			return true;
			
		}

	}

	SetNowDragItemReturn();
	return false;
}

bool CX2UIQuickSlot::OnRClickedItem( D3DXVECTOR2 mousePos )
{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-03-26
	if ( true == GetIsSummonCardSlot() )		/// 소환 카드 슬롯은 바꿀 수 없다.
		return false;

#endif // SERV_NEW_DEFENCE_DUNGEON

	CX2SlotItem* pItemSlot = NULL;
	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL )
		return false;

	const CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
	if ( pkItemTemplet == NULL )
		return false;

	//{{ kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기
#ifdef	REAL_TIME_ELSWORD
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
				return false;

			if ( g_pX2Game->GetMyUnit()->GetNowHp() <= 0 )
			{
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );

				return false;
			}

			if ( g_pX2Game->CheckAndWarningBusyStateNow() )
				return false;

		} break;

	default:
		break;
	}
#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	{
		switch(g_pMain->GetNowStateID())
		{
		case CX2Main::XS_PVP_GAME:
			//{{ kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체(퀵슬롯 에서 인벤토리로 보낼때 마우스 오른쪽 클릭 사용 가능)
#ifndef REAL_TIME_ELSWORD	
		case CX2Main::XS_DUNGEON_GAME:
#endif REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체(퀵슬롯 에서 인벤토리로 보낼때 마우스 오른쪽 클릭 사용 가능)

		default:
			{
				for ( int i = 0; i < kInventory.GetItemMaxNum( CX2Inventory::ST_QUICK_SLOT ); i++ )
				{
					CX2Item* pItem = kInventory.GetItem( CX2Inventory::ST_QUICK_SLOT , i );
					if ( pItem == NULL )
					{
						g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);;
						g_pData->GetUIManager()->GetUIInventory()->SetInventorySort( CX2Inventory::ST_QUICK_SLOT );

						Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pItemSlot->GetSortType(), pItemSlot->GetSlotID(),
							CX2Inventory::ST_QUICK_SLOT , i );

						pItemSlot->DestroyItemUI();
						return true;
					}
				}

			} break;
		}
		
	}

	return false;
}

/* virtual */ bool CX2UIQuickSlot::MouseDown( D3DXVECTOR2 mousePos )
{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-03-26
	if ( true == GetIsSummonCardSlot() )		/// 소환 카드 슬롯은 변경할 수 없다.
		return false;
#endif // SERV_NEW_DEFENCE_DUNGEON

	CX2WorldMapUI *pWorldMapUI = g_pInstanceData->GetMiniMapUI()->GetWorldMapUI();

	if ( NULL != pWorldMapUI && true == pWorldMapUI->GetShow() )
		return false;

	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_PVP_GAME:
		break;

	default:
		{
			return CX2UIDragable::MouseDown( mousePos );
		}break;
	}
	return true;
}

void CX2UIQuickSlot::SetShow(bool val)
{
	if(val)
	{
		m_bShow = true;
		m_pDLGQuickSlot->SetShow(true);

#ifdef SERV_NEW_DEFENCE_DUNGEON		/// 어둠의 문 몬스터 카드용 퀵슬롯 표시 설정
		CKTDGUIStatic* pStaticCardSlot	= (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"CardSlot" );
		CKTDGUIStatic* pStaticSlot		= (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Slot" );

		if ( NULL != pStaticCardSlot && NULL != pStaticSlot )
		{
			if( true == GetIsSummonCardSlot() )		/// 카드용 퀵슬롯 표시
			{
				pStaticCardSlot->SetShow( true );
				pStaticSlot->SetShow( false );
			}
			else									/// 아이템용 퀵슬롯 표시
			{
				pStaticCardSlot->SetShow( false );
				pStaticSlot->SetShow( true );
			}
		}
#endif //SERV_NEW_DEFENCE_DUNGEON

		//{{ JHKang / 강정훈 / QuickSlot의 Layer 수준이 계속 증가하는 것 방지
#ifdef FIX_TOOLTIP
		SetLayer( XDL_NORMAL_3 );
#else	
		SetLayer((X2_DIALOG_LAYER)m_pDLGQuickSlot->GetLayer());
#endif FIX_TOOLTIP
		//}} JHKang / 강정훈 / QuickSlot의 Layer 수준이 계속 증가하는 것 방지
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			pItemSlot->SetShow(true);
			//pItemSlot->SetEnable(true);
		}
		ResetQuickSlotUI();

#ifdef NEW_ITEM_NOTICE

	#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-11
		if( NULL != m_pDLGItemHighLight )
		{
			/// 어둠의 문 에서는 하이라이트를 사용하지 않는다.
			if ( true == GetIsSummonCardSlot() )
				m_pDLGItemHighLight->SetShow( false );
			else
				m_pDLGItemHighLight->SetShow(true);
		}
	#else // SERV_NEW_DEFENCE_DUNGEON
		if( NULL != m_pDLGItemHighLight )
			m_pDLGItemHighLight->SetShow(true);
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif //NEW_ITEM_NOTICE
	}
	else
	{
		m_bShow = false;
		m_pDLGQuickSlot->SetShow(false);

		// 아이템 마우스 오버 이미지를 지우자
		m_pDLGSelectedItem->SetShow( false ); 
		InvalidSlotDesc();
		
		// 슬롯을 끄자
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			pItemSlot->LostItemUI();
			pItemSlot->SetShow( false );
			//pItemSlot->SetEnable( false );
		}
#ifdef NEW_ITEM_NOTICE
		if( NULL != m_pDLGItemHighLight )
			m_pDLGItemHighLight->SetShow(false);
#endif //NEW_ITEM_NOTICE
	}
}

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-25
void CX2UIQuickSlot::SetEnable( bool val, bool bDirectEnable/*= false*/ )
{
	if ( val == m_bEnable && false == bDirectEnable )	/// 소환 카드 슬롯 설정을 위해, 강제로 Enable 함수 동작할 수 있도록 추가
		return;
	else
		m_bEnable = val;
#else // SERV_NEW_DEFENCE_DUNGEON
void CX2UIQuickSlot::SetEnable( bool val )
{
	if(m_bEnable == val)
		return;

	m_bEnable = val;
#endif // SERV_NEW_DEFENCE_DUNGEON
	
	CKTDGUIStatic* pStatic_Black = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Slot_Black" );
#ifdef SERV_EXPAND_QUICK_SLOT

	bool bExpandQuicSlot = false;
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		bExpandQuicSlot = g_pData->GetMyUser()->GetSelectUnit()->IsExpandQuickSlot();
	}

	if(NULL != pStatic_Black)
	{
		for( int iSlotNum = 0; iSlotNum< (int)m_SlotList.size(); iSlotNum++ )
		{
			CKTDGUIControl::CPictureData* pPictureData = pStatic_Black->GetPicture( iSlotNum );

			if ( NULL == pPictureData )
			{
				ASSERT( ! L"PictureData Is NULL" );
				return;
			}

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-03-25
			if ( true == GetIsSummonCardSlot() )	/// 소환 카드 슬롯일 때, 검은 배경 다 숨긴다.
			{
					pPictureData->SetShow( false );
			}
			else									/// 그 외엔 비확장 슬롯만 검은 배경 설정한다.
			{
				if( bExpandQuicSlot == false && m_bEnable == true && iSlotNum >=3 )
					pPictureData->SetShow( m_bEnable );
				else
					pPictureData->SetShow( !m_bEnable );
			}
#else // SERV_NEW_DEFENCE_DUNGEON
			if( bExpandQuicSlot == false && m_bEnable == true && iSlotNum >=3 )
				pPictureData->SetShow( m_bEnable );
			else
				pPictureData->SetShow( !m_bEnable );
#endif // SERV_NEW_DEFENCE_DUNGEON
		}
	}
#else
	if(NULL != pStatic_Black)
	{
		pStatic_Black->SetShow( !m_bEnable );
	}
#endif SERV_EXPAND_QUICK_SLOT

	if(val)
	{
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
#ifdef SERV_EXPAND_QUICK_SLOT

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26
			if ( true == GetIsSummonCardSlot() )	/// 소환 카드 슬롯은 4개만 활성화 시킨다.
			{
				bool bShow = true;

				if ( i >= _CONST_QUICK_SLOT_::MAX_CARD_SLOT_NUM)
					bShow = false;

				pItemSlot->SetShow( bShow );
				pItemSlot->SetEnable( bShow );
			}
			else if ( bExpandQuicSlot == false && m_bEnable == true && i >=3 )
#else // SERV_NEW_DEFENCE_DUNGEON
			if( bExpandQuicSlot == false && m_bEnable == true && i >=3 )
#endif // SERV_NEW_DEFENCE_DUNGEON
			{
				pItemSlot->SetShow(false);
				pItemSlot->SetEnable(false);
			}
			else
			{
				pItemSlot->SetShow(true);
				pItemSlot->SetEnable(true);
			}
#else
			pItemSlot->SetEnable(true);
#endif SERV_EXPAND_QUICK_SLOT
		}
	}
	else
	{
		// 슬롯을 끄자
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
#ifdef SERV_EXPAND_QUICK_SLOT
			pItemSlot->SetShow(true);
#endif SERV_EXPAND_QUICK_SLOT
			pItemSlot->SetEnable( false );
		}

		// 아이템 마우스 오버 이미지를 지우자
		m_pDLGSelectedItem->SetShow( false ); 
		InvalidSlotDesc();
	}
}

CX2SlotItem* CX2UIQuickSlot::GetEmptyslot()
{
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2Item* pItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, i );
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];

		if ( pItem == NULL )
			return pItemSlot;
	}
	return NULL;

}

//void	CX2UIQuickSlot::DrawSlotMouseOverImage()
//{
//	if ( GetCheckOperationCondition() == false )
//	{
//		m_pDLGSelectedItem->SetShow( false );
//		return;
//	}
//
//	bool bCheck = false;
//
//	if ( m_pNowOverItemSlot != NULL )
//	{
//		m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
//		m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );
//
//		bCheck = true;
//	}
//
//	if ( bCheck == true )
//	{
//		m_pDLGSelectedItem->SetShow( true );
//	}
//	else
//	{
//		m_pDLGSelectedItem->SetShow( false );
//	}
//}


#ifdef	SERV_EXPAND_QUICK_SLOT
void	CX2UIQuickSlot::SetExpandQuickSlot( int iMaxNumOfSlot )
{
#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26
	if( true == GetIsSummonCardSlot() )		/// 소환 카드 슬롯은 4칸으로 설정핝다.
		iMaxNumOfSlot = _CONST_QUICK_SLOT_::MAX_CARD_SLOT_NUM;
	else if( iMaxNumOfSlot < 3 || ( iMaxNumOfSlot > 3 && iMaxNumOfSlot < 6) )
#else // SERV_NEW_DEFENCE_DUNGEON
	if( iMaxNumOfSlot < 3 || ( iMaxNumOfSlot > 3 && iMaxNumOfSlot < 6) )
#endif // SERV_NEW_DEFENCE_DUNGEON
	{
		iMaxNumOfSlot = 3;
	}
	else if( iMaxNumOfSlot > 6 )
	{
		iMaxNumOfSlot = 6;
	}

	CKTDGUIStatic* pStatic_Black = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Slot_Black" );

	if(NULL != pStatic_Black)
	{
		for( int iSlotNum = 0; iSlotNum< iMaxNumOfSlot; iSlotNum++ )
		{
			CKTDGUIControl::CPictureData* pPictureData = pStatic_Black->GetPicture( iSlotNum );

			if ( NULL == pPictureData )
			{
				ASSERT( ! L"PictureData Is NULL" );
				return;
			}
			pPictureData->SetShow( false );
		}
	}

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26
	if( true == GetIsSummonCardSlot() ) /// 소환 카드 슬롯은 4칸으로 확장 시킨다.
	{
		for ( int iSlotNum = 0; iSlotNum < _CONST_QUICK_SLOT_::MAX_CARD_SLOT_NUM ; ++iSlotNum )
		{
			bool bShow = true;
			if ( iSlotNum >= iMaxNumOfSlot )
				bShow = false;

			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[iSlotNum];
			pItemSlot->SetShow( bShow );
			pItemSlot->SetEnable( bShow );
		}

		if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			g_pData->GetMyUser()->GetSelectUnit()->SetExpandQuickSlot( true );
		CKTDGUIStatic* pStatcLock = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl(L"LOCK");
		pStatcLock->SetShow(false);

		CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGQuickSlot->GetControl(L"Button_Buy_Guide_Quick_Slot");
		if( NULL != pButton )
		{
			pButton->SetShowEnable(false,false);
		}	
	}
	else if(iMaxNumOfSlot == 3) //미확장 
#else // SERV_NEW_DEFENCE_DUNGEON
	if(iMaxNumOfSlot == 3) //미확장 
#endif // SERV_NEW_DEFENCE_DUNGEON
	{
		if(NULL != pStatic_Black)
		{
			for( int iSlotNum = 0; iSlotNum< iMaxNumOfSlot; iSlotNum++ )
			{
				CKTDGUIControl::CPictureData* pPictureData = pStatic_Black->GetPicture( iSlotNum + iMaxNumOfSlot );

				if ( NULL == pPictureData )
				{
					ASSERT( ! L"PictureData Is NULL" );
					return;
				}
				pPictureData->SetShow( true );
			}
		}

		for ( int iSlotNum = 0; iSlotNum < iMaxNumOfSlot ; iSlotNum++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[(iSlotNum+iMaxNumOfSlot)];
			pItemSlot->SetShow( false );
			pItemSlot->SetEnable( false );
		}

		if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			g_pData->GetMyUser()->GetSelectUnit()->SetExpandQuickSlot( false );
		CKTDGUIStatic* pStatcLock = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl(L"LOCK");
		pStatcLock->SetShow(true);

		CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGQuickSlot->GetControl(L"Button_Buy_Guide_Quick_Slot");
		if( NULL != pButton )
		{
			pButton->SetShowEnable(true, true);
		}
	}
	else if(iMaxNumOfSlot == 6)//확장
	{
		for ( int iSlotNum = 3; iSlotNum < iMaxNumOfSlot ; iSlotNum++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[iSlotNum];
			pItemSlot->SetShow( true );
			pItemSlot->SetEnable( true );
		}

		if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			g_pData->GetMyUser()->GetSelectUnit()->SetExpandQuickSlot( true );
		CKTDGUIStatic* pStatcLock = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl(L"LOCK");
		pStatcLock->SetShow(false);

		CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGQuickSlot->GetControl(L"Button_Buy_Guide_Quick_Slot");
		if( NULL != pButton )
		{
			pButton->SetShowEnable(false,false);
		}		
	}

}
#endif  SERV_EXPAND_QUICK_SLOT

#ifdef NEW_ITEM_NOTICE
wstring CX2UIQuickSlot::GetSlotItemDesc()
{
	//마우스 오버 시 NewItem 하이라이트 꺼지도록 수정

	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
		{
			if( true == kInventory.EraseNewItem(m_pNowOverItemSlot->GetItemUID()) )
			{
				ResetQuickSlotUI();
			}
		}
	}				
	return CX2UIDragable::GetSlotItemDesc();
}
#endif //NEW_ITEM_NOTICE

#ifdef CREATE_ALLY_NPC_BY_MONSTER
void CX2UIQuickSlot	::UpdateCoolTimeByGroupID( int iCoolTimeGroupID )
{
	if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
	{
		if ( iCoolTimeGroupID > 0 )
		{
#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
			std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( iCoolTimeGroupID );
			if( mit != m_mapQuickSlotItemCoolTime.end() )
				mit->second.restart();
			else
			{
				CKTDXTimer kTimer;
				m_mapQuickSlotItemCoolTime.insert( std::make_pair( iCoolTimeGroupID, kTimer ) );
			}
#else
			vector< int > vecCoolTimeGroupItemIDList;
			g_pData->GetItemManager()->GetCoolTimeGroupItemIDList( iCoolTimeGroupID, vecCoolTimeGroupItemIDList );
			for ( int i = 0; i < (int)vecCoolTimeGroupItemIDList.size(); i++ )
			{
				std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( vecCoolTimeGroupItemIDList[i] );
				if( mit != m_mapQuickSlotItemCoolTime.end() )
				{
					mit->second.restart();
				}
				else
				{
					CKTDXTimer kTimer;
					m_mapQuickSlotItemCoolTime.insert( std::make_pair( vecCoolTimeGroupItemIDList[i], kTimer ) );
				}
			}
#endif // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
		}
	}
}
#endif //CREATE_ALLY_NPC_BY_MONSTER

void CX2UIQuickSlot::InitMyQuickSlotCoolTimeFromGageManager( const map<int, int>& mapQuickSlotCoolTime_ )
{
	m_mapQuickSlotItemCoolTime.clear();

	map<int, int>::const_iterator cmItr = mapQuickSlotCoolTime_.begin();

	while ( mapQuickSlotCoolTime_.end() != cmItr )
	{
		CKTDXTimer timerQuickSlotCoolTime;
		timerQuickSlotCoolTime.SetTime( cmItr->second * CLOCKS_PER_SEC );
		m_mapQuickSlotItemCoolTime.insert( pair<int, CKTDXTimer>( cmItr->first, timerQuickSlotCoolTime ) );

		++cmItr;
	}	
}

void CX2UIQuickSlot::UpdateSlotKey()
{
	if ( NULL == m_pDLGQuickSlot )
		return;

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26

	CKTDGUIStatic* pStaticCardNum	= (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Static_QUICK_CARD_NUM" );	/// 소환 카드 슬롯
	CKTDGUIStatic* pStaticNum		= (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Static_QUICK_NUM" );		/// 아이템 퀵슬롯

	if ( NULL != pStaticCardNum && NULL != pStaticNum )
	{
		int iSlotNum = _CONST_QUICK_SLOT_::MAX_QUICK_SLOT_NUM;	/// 일반 퀵슬롯 최대 슬롯 수

		if ( true == GetIsSummonCardSlot() )	/// 소환 카드 슬롯이면, 소환 카드용 단축 번호 설정
		{
			iSlotNum = _CONST_QUICK_SLOT_::MAX_CARD_SLOT_NUM;

			pStaticCardNum->SetShow( true );	/// 소환 카드 슬롯 설정
			pStaticNum->SetShow( false );		/// 퀵슬롯 해제

			for( int i = 0 ; i < iSlotNum; ++i )
			{
				GetQuickSlotStringByIndex(i, pStaticCardNum->GetString(i)->msg );
			}
		}
		else									/// 퀵슬롯 단축 번호 설정
		{
			pStaticCardNum->SetShow( false );	/// 소환 카드 슬롯 해제
			pStaticNum->SetShow( true );		/// 퀵슬롯 설정

			for( int i = 0 ; i < iSlotNum; ++i )
			{
				GetQuickSlotStringByIndex(i, pStaticNum->GetString(i)->msg );
			}
		}
	}

#else // SERV_NEW_DEFENCE_DUNGEON

	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Static_QUICK_NUM" );

	if( NULL != pStaticNum )
	{
		for( int i = 0 ; i < 6; ++i )
		{
			GetQuickSlotStringByIndex(i, pStaticNum->GetString(i)->msg );
		}
	}

#endif // SERV_NEW_DEFENCE_DUNGEON
}

void CX2UIQuickSlot::GetQuickSlotStringByIndex( IN int iIndex_, OUT wstring& strQuickSlot_ ) const
{
	strQuickSlot_.clear();

	switch ( iIndex_ )
	{
	case 0:
		strQuickSlot_ = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_QUICKSLOT1 );
		break;
	case 1:
		strQuickSlot_ = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_QUICKSLOT2 );
		break;
	case 2:
		strQuickSlot_ = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_QUICKSLOT3 );
		break;
	case 3:
		strQuickSlot_ = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_QUICKSLOT4 );
		break;
	case 4:
		strQuickSlot_ = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_QUICKSLOT5 );
		break;
	case 5:
		strQuickSlot_ = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_QUICKSLOT6 );
		break;
	}		
}

/** @function	: SetSlotItemIcon
	@brief		: 퀵슬롯 아이콘 설정 함수 ( 기존의 코드를 함수로 분리 )
	@param		: 아이템 슬롯들이 저장되어 있는 컨테이너, 인벤토리 객체, 아이콘 컨트롤 객체
*/
#ifdef SERV_NEW_DEFENCE_DUNGEON
void CX2UIQuickSlot::SetSlotItemIcon( vector< CX2Slot* >& m_SlotList, const CX2Inventory& kInventory, CKTDGUIStatic* pStatic )
{
	if( NULL == pStatic )
		return;

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		if( NULL == pItemSlot )
			continue;

		pItemSlot->DestroyItemUI();

		/// 슬롯 아이템 아이콘 위치 및 크기 설정
		pItemSlot->SetSize( D3DXVECTOR2( 40.f, 40.f ) );

#ifdef INT_WIDE_BAR
		// DLG_UI_Quick_Slot.lua 에서 조절한 만큼 조절해준다. // 김석근, 86red, [2013.08.30]
		pItemSlot->SetPos( D3DXVECTOR2( 9.f + ( static_cast<float>( i ) * 46.f ), 711.f - 20.f) );
#else //INT_WIDE_BAR
		pItemSlot->SetPos( D3DXVECTOR2( 9.f + ( static_cast<float>( i ) * 46.f ), 711.f ) );
#endif //INT_WIDE_BAR

		CX2Item* pItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, i );
		if ( pItem == NULL )
			continue;

#ifdef NEW_ITEM_NOTICE 
		{
			UidType iItemUID = pItem->GetItemData().m_ItemUID;
			bool	bNewItem = kInventory.IsNewItem( CX2Inventory::ST_QUICK_SLOT, iItemUID );

			if( true == bNewItem && NULL != pStatic &&  NULL != pStatic->GetPicture(i) )
			{//위에서 모두 초기화 시켜 주기 때문에, true일때만 수정
				pStatic->GetPicture(i)->SetShow(bNewItem);
			}
		}
#endif //NEW_ITEM_NOTICE 

		pItemSlot->CreateItemUI( pItem );	
		//(레이어 문제 때문에 배경 픽쳐와 다르게 슬롯에 기록)
		//단축키 번호 스트링 
		CKTDGUIStatic* pStaticHotKey = pItemSlot->GetDialog()->CreateStatic();	
		if( NULL != pStaticHotKey )
		{
			pStaticHotKey->SetName(L"StaticHotKey");
			pItemSlot->GetDialog()->AddControl(pStaticHotKey);
			WCHAR buff[32] = {0};
			CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
			pStaticHotKey->AddString( pString );

			GetQuickSlotStringByIndex( i , pString->msg );
			pString->fontIndex = XUF_DODUM_11_NORMAL;
			pString->fontStyle = CKTDGFontManager::FS_SHELL;
			pString->sortFlag = DT_RIGHT;
			pString->pos = D3DXVECTOR2 ( 26.f, 0.f );
			pString->color = D3DXCOLOR( 1.f, 1.f, 0.f, 1.f );
			pString->outlineColor = D3DXCOLOR( 0.3f,0.2f,0.2f,1.f );
		}

		//재사용 대기 시간 스트링
		CKTDGUIStatic* pStaticCoolTime = pItemSlot->GetDialog()->CreateStatic();	
		if( NULL != pStaticCoolTime )
		{
			pStaticCoolTime->SetName(L"CoolTime");
			pItemSlot->GetDialog()->AddControl(pStaticCoolTime);
			WCHAR buff[32] = {0};
			CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
			pStaticCoolTime->AddString( pString );

			pString->fontIndex = XUF_DODUM_20_BOLD;
			pString->fontStyle = CKTDGFontManager::FS_SHELL;
			pString->sortFlag = DT_CENTER;
			pString->pos = D3DXVECTOR2 ( 20.f, 10.f );
			pString->color = D3DXCOLOR( 1.f, 1.f, 0.f, 1.f );
			pString->outlineColor = D3DXCOLOR( 0.3f,0.2f,0.2f,1.f );
		}	
	}
}

/** @function	: SetSummonMonsterItemIcon
	@brief		: 몬스터 소환 카드 퀵슬롯 아이콘 설정 함수
	@param		: 아이템 슬롯들이 저장되어 있는 컨테이너, 인벤토리 객체, 아이콘 컨트롤 객체
*/
void CX2UIQuickSlot::SetSummonMonsterItemIcon( vector< CX2Slot* >& m_SlotList, const CX2Inventory& kInventory, CKTDGUIStatic* pStatic )
{
	if( NULL == pStatic )
		return;

	CKTDGUIStatic* pStaticBuyGuideQuickSlot = (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"Button_Buy_Guide_Quick_Slot" );
	CKTDGUIStatic* pStaticLock				= (CKTDGUIStatic*) m_pDLGQuickSlot->GetControl( L"LOCK" );

	if ( pStaticBuyGuideQuickSlot || NULL != pStaticLock )
	{
		pStaticBuyGuideQuickSlot->SetShow( false );
		pStaticLock->SetShow( false );
	}

	SetExpandQuickSlot( _CONST_QUICK_SLOT_::MAX_CARD_SLOT_NUM );

	for ( int i = 0; i < _CONST_QUICK_SLOT_::MAX_CARD_SLOT_NUM; ++i )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		if( NULL == pItemSlot )
			continue;

		pItemSlot->DestroyItemUI();

		/// 슬롯 아이템 아이콘 위치 및 크기 설정
		pItemSlot->SetSize( D3DXVECTOR2( 64.f, 64.f ) );

#ifdef INT_WIDE_BAR
		// DLG_UI_Quick_Slot.lua 에서 조절한 만큼 조절해준다. // 김석근, 86red, [2013.08.30]
		pItemSlot->SetPos( D3DXVECTOR2( 11.f + ( static_cast<float>( i * 72 ) ), 686.f - 20.f ) );
#else //INT_WIDE_BAR
		pItemSlot->SetPos( D3DXVECTOR2( 11.f + ( static_cast<float>( i * 72 ) ), 686.f ) );
#endif //INT_WIDE_BAR
		

		CX2Item* pItem = kInventory.GetItem( CX2Inventory::ST_E_DEFENCE_QUICK_SLOT, i );
		if ( pItem == NULL )
			continue;

#ifdef NEW_ITEM_NOTICE 
		{
			UidType iItemUID = pItem->GetItemData().m_ItemUID;
			bool	bNewItem = kInventory.IsNewItem( CX2Inventory::ST_E_DEFENCE_QUICK_SLOT, iItemUID );

			if( true == bNewItem && NULL != pStatic &&  NULL != pStatic->GetPicture(i) )
			{//위에서 모두 초기화 시켜 주기 때문에, true일때만 수정
				pStatic->GetPicture(i)->SetShow(bNewItem);
			}
		}
#endif //NEW_ITEM_NOTICE 

		pItemSlot->CreateItemUI( pItem );	
		//(레이어 문제 때문에 배경 픽쳐와 다르게 슬롯에 기록)
		//단축키 번호 스트링 
		CKTDGUIStatic* pStaticHotKey = pItemSlot->GetDialog()->CreateStatic();	
		if( NULL != pStaticHotKey )
		{
			pStaticHotKey->SetName(L"StaticHotKey");
			pItemSlot->GetDialog()->AddControl(pStaticHotKey);
			WCHAR buff[32] = {0};
			CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
			pStaticHotKey->AddString( pString );

			GetQuickSlotStringByIndex( i , pString->msg );
			pString->fontIndex = XUF_DODUM_20_BOLD;
			pString->fontStyle = CKTDGFontManager::FS_SHELL;
			pString->sortFlag = DT_RIGHT;
			pString->pos = D3DXVECTOR2 ( 40.f, 2.f );
			pString->color = D3DXCOLOR( 1.f, 1.f, 0.f, 1.f );
			pString->outlineColor = D3DXCOLOR( 0.3f,0.2f,0.2f,1.f );
		}

		//재사용 대기 시간 스트링
		CKTDGUIStatic* pStaticCoolTime = pItemSlot->GetDialog()->CreateStatic();	
		if( NULL != pStaticCoolTime )
		{
			pStaticCoolTime->SetName(L"CoolTime");
			pItemSlot->GetDialog()->AddControl(pStaticCoolTime);
			WCHAR buff[32] = {0};
			CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
			pStaticCoolTime->AddString( pString );

			pString->fontIndex = XUF_HEADLINE_30_NORMAL;
			pString->fontStyle = CKTDGFontManager::FS_SHELL;
			pString->sortFlag = DT_CENTER;
			pString->pos = D3DXVECTOR2 ( 30.f, 17.f );
			pString->color = D3DXCOLOR( 1.f, 1.f, 0.f, 1.f );
			pString->outlineColor = D3DXCOLOR( 0.3f,0.2f,0.2f,1.f );
		}		
	}

	SetEnable( true, true );
}

/** @function	: InputSummonCardSlotKey
	@brief		: 몬스터 소환 카드 슬롯 키보드 입력 함수
	@param		: 인벤토리 정보, 설정할 퀵슬롯 아이템, 설정할 슬롯 인덱스
*/
void CX2UIQuickSlot::InputSummonCardSlotKey( const CX2Inventory& kInventory, CX2Item** pQuickItem, int& slotID )
{

	bool bUse = false;

	/// 어둠의 문 일땐, 몬스터 카드 슬롯의 아이템 정보를 가져온다.
	if( GET_KEY_STATE( GA_QUICKSLOT1 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_DEFENCE_QUICK_SLOT, 0 );
		slotID = 0;
		bUse = true;
	}
	else if( GET_KEY_STATE( GA_QUICKSLOT2 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_DEFENCE_QUICK_SLOT, 1 );
		slotID = 1;
		bUse = true;
	}
	else if( GET_KEY_STATE( GA_QUICKSLOT3 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_DEFENCE_QUICK_SLOT, 2 );
		slotID = 2;
		bUse = true;
	}
	else if( GET_KEY_STATE( GA_QUICKSLOT4 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_DEFENCE_QUICK_SLOT, 3 );
		slotID = 3;
	}

	if( true == bUse )
	{
#ifdef CHECK_INHERIT_STATE_MENU
		if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
		{
			CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
			if( pStateMenu != NULL )
				pStateMenu->DeleteMouseClickUI();
		}
	}
}

/** @function	: InputQuickSlotKey
	@brief		: 퀵슬롯 키보드 입력 함수
	@param		: 인벤토리 정보, 설정할 퀵슬롯 아이템, 설정할 슬롯 인덱스
*/
void CX2UIQuickSlot::InputQuickSlotKey( const CX2Inventory& kInventory, CX2Item** pQuickItem, int& slotID )
{
	if( GET_KEY_STATE( GA_QUICKSLOT1 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 0 );
		slotID = 0;
	}
	else if( GET_KEY_STATE( GA_QUICKSLOT2 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 1 );
		slotID = 1;
	}
	else if( GET_KEY_STATE( GA_QUICKSLOT3 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 2 );
		slotID = 2;
	}
	else if( GET_KEY_STATE( GA_QUICKSLOT4 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 3 );
		slotID = 3;
	}
	else if( GET_KEY_STATE( GA_QUICKSLOT5 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 4 );
		slotID = 4;
	}
	else if( GET_KEY_STATE( GA_QUICKSLOT6 ) == TRUE )
	{
		*pQuickItem = kInventory.GetItem( CX2Inventory::ST_E_QUICK_SLOT, 5 );
		slotID = 5;
	}
}

/** @function	: Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ
	@brief		: 퀵슬롯 사용 요청 함수
	@param		: 슬롯 인덱스
	@return		: 전송 여부
*/
bool CX2UIQuickSlot::Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ( int slotID )
{

	if(m_bEnable)
	{
		KEGS_USE_QUICK_SLOT_REQ kPacket;
		kPacket.m_sSlotID = slotID;

		g_pData->GetServerProtocol()->SendPacket( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ, kPacket ); 
		g_pMain->AddServerPacket( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK );

		return true;
	}	
	return false;
}

/** @function	: Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK
	@brief		: 퀵슬롯 사용 요청 함수
	@param		: 슬롯 인덱스
	@return		: 성공 여부
*/
bool CX2UIQuickSlot::Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( kEvent.m_ItemID == -1 )
			{
				return true;
			}

			g_pKTDXApp->GetDeviceManager()->PlaySound( L"UseItem.ogg", false, false );

			KInventoryItemInfo& kInventorySlotInfo = kEvent.m_InventorySlotInfo;

			CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
			pMyUnit->AccessInventory().RemoveItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );

			if ( kInventorySlotInfo.m_iItemUID > 0 )
			{
				CX2Item::ItemData kItemData( kInventorySlotInfo );
				pMyUnit->AccessInventory().AddItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID, kItemData );
			}

			ResetQuickSlotUI();
#ifdef COOLTIME_SHARE_GROUP_ITEM
			int iCoolTimeGroupID = g_pData->GetItemManager()->GetCoolTimeGroupID( kEvent.m_ItemID );
#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
			if( iCoolTimeGroupID > 0 )
			{
				std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( iCoolTimeGroupID );
				if( mit != m_mapQuickSlotItemCoolTime.end() )
				{
					mit->second.restart();
				}
				else
				{
					CKTDXTimer kTimer;
					m_mapQuickSlotItemCoolTime.insert( std::make_pair( iCoolTimeGroupID, kTimer ) );
				}
			}
#else
			if( iCoolTimeGroupID > 0 )
			{
				vector< int > vecCoolTimeGroupItemIDList;
				g_pData->GetItemManager()->GetCoolTimeGroupItemIDList( iCoolTimeGroupID, vecCoolTimeGroupItemIDList );
				for ( int i = 0; i < (int)vecCoolTimeGroupItemIDList.size(); i++ )
				{
					std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( vecCoolTimeGroupItemIDList[i] );
					if( mit != m_mapQuickSlotItemCoolTime.end() )
					{
						mit->second.restart();
					}
					else
					{
						CKTDXTimer kTimer;
						m_mapQuickSlotItemCoolTime.insert( std::make_pair( vecCoolTimeGroupItemIDList[i], kTimer ) );
					}
				}
			}
			else
			{
				std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( kEvent.m_ItemID );
				if( mit != m_mapQuickSlotItemCoolTime.end() )
				{
					mit->second.restart();
				}
				else
				{
					CKTDXTimer kTimer;
					m_mapQuickSlotItemCoolTime.insert( std::make_pair( kEvent.m_ItemID, kTimer ) );
				}
			}
#endif // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP



#else
			std::map< int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( kEvent.m_ItemID );
			if( mit != m_mapQuickSlotItemCoolTime.end() )
			{
				mit->second.restart();
			}
			else
			{
				CKTDXTimer kTimer;
				m_mapQuickSlotItemCoolTime.insert( std::make_pair( kEvent.m_ItemID, kTimer ) );
			}
#endif COOLTIME_SHARE_GROUP_ITEM

			switch ( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP:
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEvent.m_ItemID );
					if ( NULL != pItemTemplet )
					{
						CX2GageData* pMyGageData = CX2GageManager::GetInstance()->GetMyGageData();

						if ( NULL != pMyGageData )
							pMyGageData->UseSpecialAbilityInVillage( pItemTemplet );

						if( NULL != g_pChatBox )
						{
							wstring wstrColor = L"#CECEC88";							// (236, 236, 136)
							D3DXCOLOR coTextColor(0.92549f, 0.92549f, 0.53333f, 1.f);	// (236, 236, 136)

							g_pChatBox->AddChatLog( L"사용", KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
						}
					}
				} break;

			default:
				break;
			}

		}

		m_bWaiting_EGS_USE_QUICK_SLOT_ACK = false;		

		if( NULL != g_pX2Game )
			g_pX2Game->SetEnableAllKeyProcess( true );

	}

	return true;
}
#endif //SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
/** @function : OnFrameMove_CoolTime
	@brief : 퀵슬롯의 쿨타임 관련 프레임 무브
*/
void CX2UIQuickSlot::OnFrameMove_CoolTime( CX2Item* pItem_, CX2SlotItem* pSlotItem_ )
{
	if( NULL == pItem_ || NULL == pItem_->GetItemTemplet())
		return;

	if( NULL == g_pData || NULL == g_pData->GetItemManager() )
		return;
	
	const int iItemID = pItem_->GetItemData().m_ItemID;
	const int iGroupID = g_pData->GetItemManager()->GetCoolTimeGroupID( iItemID );

	bool bCheck = false;
	
	map < int, CKTDXTimer >::iterator mit = m_mapQuickSlotItemCoolTime.find( iGroupID );
	// 해당 그룹ID가 현재 쿨타임에 등록된 그룹ID라면
	if( mit != m_mapQuickSlotItemCoolTime.end() )
	{
		CKTDGUIStatic* pStaticItem = static_cast<CKTDGUIStatic*>(pSlotItem_->GetDialog()->GetControl( L"StaticRoot_ItemSlot"));
		if ( pStaticItem != NULL && pStaticItem->GetPicture(1) != NULL )
		{
			CKTDGUIControl::CPictureData* pPicture = pStaticItem->GetPicture(1);

			// 관리중인 쿨타임의 진행 시간 얻기.
			float fItemCoolTimeElapsedTime = static_cast<float>(mit->second.elapsed());
			float fItemCoolTimeLeftTime = 0.f;
			// 진행시간이 쿨타임보다 크면 남은 시간을 0으로 변경
			if ( fItemCoolTimeElapsedTime >= static_cast<float>(pItem_->GetItemTemplet()->GetCoolTime()) )
			{
				bCheck = false;
			}
			else
			{
				//아니라면 총 쿨타임에서 진행시간을 뺀 남은 쿨타임 얻기.
				bCheck = true;
				fItemCoolTimeLeftTime = (float)pItem_->GetItemTemplet()->GetCoolTime() - fItemCoolTimeElapsedTime;
				pPicture->SetShow( true );
			}

			pPicture->SetPos( D3DXVECTOR2( pPicture->GetOriginalPos().x, 
										   pPicture->GetOriginalPos().y + pPicture->GetOriginalSize().y - ( pPicture->GetOriginalSize().y * (  fItemCoolTimeLeftTime / pItem_->GetItemTemplet()->GetCoolTime()  ) ) ) );
			pPicture->SetSize( D3DXVECTOR2( pPicture->GetOriginalSize().x, 
											pPicture->GetOriginalSize().y * (fItemCoolTimeLeftTime / pItem_->GetItemTemplet()->GetCoolTime()  )  )  );

			//재사용 대기시간 숫자 설정
			CKTDGUIStatic* pStaticCoolTime = static_cast<CKTDGUIStatic*>(pSlotItem_->GetDialog()->GetControl( L"CoolTime" ));
			if( NULL != pStaticCoolTime && NULL != pStaticCoolTime->GetString(0))
			{
				WCHAR wBuf1[10];
				StringCchPrintfW( wBuf1, 10, L"%d", static_cast<int>(fItemCoolTimeLeftTime)  );
				pStaticCoolTime->GetString(0)->msg = wBuf1;
			}
		}
		else
		{
			ASSERT("QuickSlot Control Error");
		}
	}

	// false일 경우 퀵슬롯 활성화 
	if ( bCheck == false )
	{
		if ( m_mapQuickSlotItemCoolTime.end() != mit )
			m_mapQuickSlotItemCoolTime.erase( mit );

		CKTDGUIStatic* pStaticItem = static_cast<CKTDGUIStatic*>(pSlotItem_->GetDialog()->GetControl( L"StaticRoot_ItemSlot" ));
		if ( pStaticItem != NULL && pStaticItem->GetPicture(1) != NULL )
		{
			CKTDGUIControl::CPictureData* pPicture = pStaticItem->GetPicture(1);

			if ( pPicture->GetShow() == true )
			{
				//퀵슬롯을 다시 쓸 수 있다는 표시
				float fEffectPosX = (pSlotItem_->GetDialog()->GetPos().x + ( pPicture->GetOriginalSize().x/2 )) * g_pKTDXApp->GetResolutionScaleX() ;
				float fEffectPosY = (pSlotItem_->GetDialog()->GetPos().y + ( pPicture->GetOriginalSize().y/2 )) * g_pKTDXApp->GetResolutionScaleY() ;
				CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"UI_ItemBox_enable_P01",
					fEffectPosX, fEffectPosY, 0, 100, 100, true, 5, 10 );

				if ( NULL != pParticle )
					pParticle->SetOverUI( true );

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-25
				if( true == m_bIsSummonCardSlot )
				{
					if( g_pMain->IsInheritStateMenu() )
					{
						CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState());
						if( pStateMenu != NULL )
						{
							CX2State::MouseClickUI::MOUSE_CLICK_STYLE arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP_NORMAL;
							D3DXVECTOR2 vPos = D3DXVECTOR2(107, 763);
							pStateMenu->CreateMouseClickUI( g_pMain->GetNowState(), vPos, 0.5f, 1000, arrowStyle );
						}
					}
				}
#endif //SERV_NEW_DEFENCE_DUNGEON
			}
			pPicture->SetShow( false );
			//재사용 대기시간 숫자 설정
			CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*) pSlotItem_->GetDialog()->GetControl( L"CoolTime" );			
			if( NULL != pStaticCoolTime && NULL != pStaticCoolTime->GetString(0)  )
			{
				pStaticCoolTime->GetString(0)->msg=L"";
			}				
		}
	}
	CX2GageManager::GetInstance()->SetMyQuickSlotCoolTimeList( m_mapQuickSlotItemCoolTime );
}
#endif // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP