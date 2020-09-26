#include "StdAfx.h"
#include ".\x2characterroom.h"




//bool CX2CharacterRoom::m_sbCheckQuestionSellNum = true;
//
//
//
//CX2CharacterRoom::CX2CharacterRoom( CKTDXStage* pNowStage, const WCHAR* pFileName )
//: CX2ItemSlotManager( pNowStage, pFileName )
//{
//	RegisterLuaBind();
//	m_pDLGSkillSlot = NULL;
//
//	
//
//	m_pDLGMyInfoFront = new CKTDGUIDialog( pNowStage, L"DLG_Unit_State_Front.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyInfoFront );
//	
//	m_pDLGMyInfoInventory = new CKTDGUIDialog( pNowStage, L"DLG_Unit_State_Inventory.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyInfoInventory );
//
//
///*
//	CKTDGUIControl* pControlattribEnchant = m_pDLGMyInfoInventory->GetControl( L"attribEnchant" );
//	if ( pControlattribEnchant != NULL )
//	{
//		pControlattribEnchant->SetShowEnable( true, true );
//	}
//*/
//
//
//	CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGMyInfoInventory->GetControl(L"ResaleCheckBox");
//	if ( pCheckBox != NULL )
//		pCheckBox->SetChecked( !m_sbCheckQuestionSellNum );
//
//	if ( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV )
//	{
//		CKTDGUIControl* pControl = m_pDLGMyInfoInventory->GetControl( L"g_pCheckBoxForceEnchant" );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//			pControl->SetEnable( false );
//		}
//	}
//
//	m_pSkillTreeUI			= new CX2SkillTreeUI( pNowStage );
//
//	m_pDLGSkillSlot = new CKTDGUIDialog( pNowStage, L"DLG_Character_Room_Skill_Slot.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillSlot );
//
//
//	m_pNowState		= (CX2State*) pNowStage;
//	m_pStateMenu	= (CX2StateMenu*) pNowStage; // note!! statemenu를 상속받은 state에서만 character room이 쓰인다고 가정하자.
//
//	CX2UnitViewerUI::SetUnitClicked( false );
//	m_pUnitViewerUI = NULL;
//	m_fMouseSensitivity = 0.01f;
//	m_fRemainRotY = 0.0f;
//	m_bRotateReturn = true;
//
//	m_pUnitViewerUI = new CX2UnitViewerUI();
//	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
//	m_pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
//	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );
//
//
//	m_pUnitViewerUI->SetUnit( g_pData->GetMyUser()->GetSelectUnit() );
//	m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250, -60, 200 ) );
//	m_pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.5f, 1.5f, 1.5f ) );
//	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
//	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
//
//	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
//
//	m_UnitClickPos = D3DXVECTOR2( 145, 145 );
//	m_UnitClickSize = D3DXVECTOR2( 237, 297 );
//
//
//
//	m_NowCharacterRoomState = CRS_INVENTORY;
//
//
//	m_pDLGMyInfoQuickSlotNumber = new CKTDGUIDialog( pNowStage, L"DLG_Unit_State_QuickSlot_Number.lua", 0.07f, GetQuickSlotNumLayer() );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyInfoQuickSlotNumber );
//
//	m_MyInfoSlotItemReactLeftTop		= D3DXVECTOR2( m_pDLGMyInfoFront->GetDummyPos(2).x, m_pDLGMyInfoFront->GetDummyPos(2).y );
//	m_MyInfoSlotItemReactRightBottom	= D3DXVECTOR2( m_pDLGMyInfoFront->GetDummyPos(3).x, m_pDLGMyInfoFront->GetDummyPos(3).y );
//
//	m_NowInventorySortType = CX2Inventory::ST_EQUIP;
//
//	//인벤 탭에 맞게끔 페이지 설정해줘야함 지금은 그냥 대~충 때우는중
//	m_NowInvenSortTypePageNum		= 1;
//	m_NowInvenSortTypePageMaxNum	= 1;
//
////	m_pStat				= new CX2Stat();
////	m_pStatEqip			= new CX2Stat();
//
//	m_GarbageBoxPos		= D3DXVECTOR2( 527, 614 );
//	m_GarbageBoxSize	= D3DXVECTOR2( 90, 105 );
//
//	m_RepairBoxPos		= D3DXVECTOR2( 634, 629 );
//	m_RepairBoxSize		= D3DXVECTOR2( 95, 86 );
//
//	m_vQuickSlotWindowPos	= D3DXVECTOR2( m_pDLGMyInfoFront->GetDummyPos(0).x, m_pDLGMyInfoFront->GetDummyPos(0).y );
//	m_vQuickSlotWindowSize	= D3DXVECTOR2( m_pDLGMyInfoFront->GetDummyPos(1).x, m_pDLGMyInfoFront->GetDummyPos(1).y );
//
//	//{{ 2007. 10. 8  최육사  랜덤 아이템 초기화
//	m_pDLGRandomItem = NULL;
//	m_iRandomItemUID = 0;
//	m_RandomItemID = 0;
//	//}}
//
//	m_DecompositionItemUID = 0;
//	m_EnchantItemUID = 0;
//	m_bUseEnchantAdjuvant = false;
//
//	m_SocketItemUID = 0;
//	m_SocketSlotIndex = 0;
//	m_bSocketRareMagicStone = false;
//	m_bSocketRemove = false;
//
//	m_DecompositionWindowNowPageNum = 1;
//	m_mapResolveResultInsertedItem.clear();
//
//	m_TempSortType	= -1;
//	m_TempSlotID	= -1;
//	m_TempItemUID	= -1;
//	m_QuickSellItemUID		= 0;
//
//	m_pDialogToRemoveItem = NULL;
//	m_pDLGToRepair	= NULL;
//
//	m_bPlayGetRandomItem	= false;
//	m_fPlayTime				= 0.0f;
//	m_fPlayMaxTime			= 7.0f;
//
//	m_pDLGGetRandomItemBG	= NULL;
//	m_hMeshInstRandomBox	= INVALID_MESH_INSTANCE_HANDLE;
//	m_hMeshInstRandomBoxKey	= INVALID_MESH_INSTANCE_HANDLE;
//	m_hSeqKeyTrace			= INVALID_PARTICLE_SEQUENCE_HANDLE;
//
//	m_pDLGSelectSellNumWindowPopup	= NULL;
//	m_SellItemUID					= -1;
//	m_SellItemNum					= 1;
//
//	m_ElChangerItemUID				= 0;
//	m_ElChangerItemCount			= 0;
//
//	m_pDLGAllEquippingItemRepair	= NULL;
//
//	m_pDLGResolveItem		= NULL;
//	m_pDLGEnchantItem		= NULL;
//	m_pDLGResolveItemResult = NULL;
//	m_pDLGEnchantItemResult = NULL;
//
//	m_pDLGSocketItem = NULL;
//	m_pDLGSocketItemPushOk = NULL;
//	m_pDLGSocketItemCheat = NULL;
//	m_pDLGCubeCheck	= NULL;
//
//	m_pDLGAttribEnchantItem	= NULL;
//	m_pDLGElChanger			= NULL;
//
//	m_AttribEnchantSlotID	= -1;
//	m_AttribEnchantID		= -1;
//
//	m_pDLGSocketItemGuide = new CKTDGUIDialog( m_pNowState, L"DLG_Socket_Item_Guide.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSocketItemGuide );
//	m_pDLGSocketItemGuide->SetShowEnable( false, false );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGSocketItemGuide, false );
//
//	m_pDLGEnchantItemGuide = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Guide.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEnchantItemGuide );
//	m_pDLGEnchantItemGuide->SetShowEnable( false, false );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGEnchantItemGuide, false );
//
//	m_pDLGAttribItemGuide = new CKTDGUIDialog( m_pNowState, L"DLG_Attribute_item_guide.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttribItemGuide );
//	m_pDLGAttribItemGuide->SetShowEnable( false, false );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGAttribItemGuide, false );
//
//
//	m_pDLGWarningAllReady	= new CKTDGUIDialog( m_pNowState, L"DLG_Room_Warning_All_Ready.lua", 0.0f );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWarningAllReady );
//	m_pDLGWarningAllReady->SetColor( D3DXCOLOR(1,1,1,0) );
//
//	m_TotalExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
//	m_TotalED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
//	m_TotalVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
//	m_TotalLevel = g_pData->GetSelectUnitLevel();
//
//	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->CreateSequence( L"MenuInfoPicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
//	pSeq->SetOverUI( true );
//	m_pPicCharMenuPlusInfo = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
//	m_pPicCharMenuPlusInfo->SetWidth( 10 );
//
//	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( L"MenuInfoPicCharRed", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
//	pSeq->SetOverUI( true );
//	m_pPicCharMenuPlusInfoRed = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
//	m_pPicCharMenuPlusInfoRed->SetWidth( 10 );
//
//	ResourcePreLoad();
//
//#ifdef ITEM_RECOVERY_TEST
//	m_RecoveryItemUid = 0;
//#endif
//#ifdef ATTRACTION_ITEM_TEST
//	m_bOpenedAttractionItem	= false;
//	m_bPlayAttractionItem	= false;
//	m_pDLGOpenAttraction	= NULL;
//	m_pDLGGetAttractionItemBG = NULL;
//	m_pDLGAttractionResult	= NULL;
//	m_hMeshInstMeltMachine		= INVALID_MESH_INSTANCE_HANDLE;
//	m_hMeshInstMeltMachineStart = INVALID_MESH_INSTANCE_HANDLE;
//	m_hSeqSpreadLight			= INVALID_PARTICLE_SEQUENCE_HANDLE;
//	m_hSeqEndLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
//		
//	m_bUseCoupon	= false;
//	m_CouponTargetUid = 0;
//	m_CouponUid		= 0;
//#endif
//
//
////{{ 2009.01.20 김태완 : 운영자용 랜덤아이템 자동열기 기능
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//	m_bUsingRandomItemAutoOpen	= false;
//	m_OpenedRandomItemCount		= 0;
//	m_pDLGMsgBox				= NULL;
//	m_MapAutoRandomItemGet.clear();
//#endif
//
//	SetOpen(false);
//
//}
//
//CX2CharacterRoom::~CX2CharacterRoom(void)
//{
//	m_pNowState		= NULL;
//	m_pStateMenu	= NULL;
//	
//
//	SAFE_DELETE_DIALOG( m_pDLGMyInfoFront );
//
//	SAFE_DELETE_DIALOG( m_pDLGMyInfoInventory );
//
//	SAFE_DELETE_DIALOG( m_pDLGMyInfoQuickSlotNumber );
//
//	SAFE_DELETE_DIALOG( m_pDLGSkillSlot );
//	SAFE_DELETE_DIALOG( m_pDLGGetRandomItemBG );
//	SAFE_DELETE_DIALOG( m_pDLGResolveItemResult );
//	SAFE_DELETE_DIALOG( m_pDLGResolveItem );
//	SAFE_DELETE_DIALOG( m_pDLGEnchantItem );
//	SAFE_DELETE_DIALOG( m_pDLGEnchantItemResult );
//	SAFE_DELETE_DIALOG( m_pDLGSocketItem );
//	SAFE_DELETE_DIALOG( m_pDLGSocketItemCheat );
//
//	SAFE_DELETE_DIALOG( m_pDLGEnchantItemGuide );
//	SAFE_DELETE_DIALOG( m_pDLGSocketItemGuide );
//	SAFE_DELETE_DIALOG( m_pDLGAttribItemGuide );
//
//	SAFE_DELETE_DIALOG( m_pDLGWarningAllReady );
//
//	SAFE_DELETE_DIALOG( m_pDLGAttribEnchantItem );
//
//	SAFE_DELETE_DIALOG( m_pDLGElChanger );
//	
//	if ( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstRandomBox )
//	{
//		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstRandomBox );
//	}
//
//	if ( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstRandomBoxKey )
//	{
//		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstRandomBoxKey );
//	}
//
//	if ( m_hSeqKeyTrace != INVALID_PARTICLE_SEQUENCE_HANDLE )
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqKeyTrace );
//	}
//
//
//#ifdef ATTRACTION_ITEM_TEST
//	SAFE_DELETE_DIALOG(m_pDLGOpenAttraction);
//	SAFE_DELETE_DIALOG(m_pDLGGetAttractionItemBG);
//	SAFE_DELETE_DIALOG(m_pDLGAttractionResult);
//
//	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstMeltMachineStart )
//	{
//		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
//	}
//
//	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstMeltMachine )
//	{
//		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//	}
//
//	if ( m_hSeqSpreadLight != INVALID_PARTICLE_SEQUENCE_HANDLE )
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
//	}
//
//	if ( m_hSeqEndLight != INVALID_PARTICLE_SEQUENCE_HANDLE )
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEndLight );
//	}
//#endif
//	
//
//	SAFE_DELETE( m_pSkillTreeUI );
//
//	SAFE_DELETE( m_pUnitViewerUI );
//
//
////	SAFE_DELETE( m_pStat );
////	SAFE_DELETE( m_pStatEqip );
//
//	if ( m_pPicCharMenuPlusInfo != NULL )
//		m_pPicCharMenuPlusInfo->Clear();
//	SAFE_DELETE( m_pPicCharMenuPlusInfo );
//	
//	if ( m_pPicCharMenuPlusInfoRed != NULL )
//		m_pPicCharMenuPlusInfoRed->Clear();
//	SAFE_DELETE( m_pPicCharMenuPlusInfoRed );
//
//	ClearDeviceList();
//}
//
//
//
//void CX2CharacterRoom::SetNowStage( CKTDXStage* pNowStage )
//{
//	m_pNowState = (CX2State*)pNowStage;
//	m_pStateMenu = (CX2StateMenu*)pNowStage;
//
//	if ( m_pDLGMyInfoFront != NULL )
//		m_pDLGMyInfoFront->SetStage( m_pNowState );
//
//	if ( m_pDLGMyInfoInventory != NULL )
//		m_pDLGMyInfoInventory->SetStage( m_pNowState );
//
//	if ( m_pDLGItemDesc != NULL )
//		m_pDLGItemDesc->SetStage( m_pNowState );
//	if ( m_pDLGSelectedItem != NULL )
//		m_pDLGSelectedItem->SetStage( m_pNowState );
//
//	if ( m_pDLGMyInfoQuickSlotNumber != NULL )
//		m_pDLGMyInfoQuickSlotNumber->SetStage( m_pNowState );
//
//	if( m_pSkillTreeUI != NULL )
//		m_pSkillTreeUI->SetStage( m_pNowState );
//
//	if( m_pDLGSkillSlot != NULL )
//		m_pDLGSkillSlot->SetStage( m_pNowState );
//}
//
//
//HRESULT CX2CharacterRoom::OnFrameMove( double fTime, float fElapsedTime )
//{
//
//#ifdef TITLE_SYSTEM
//    if( g_pData->GetTitleManager() != NULL && g_pData->GetTitleManager()->GetShow() == true )
//    {
//        g_pData->GetTitleManager()->OnFrameMove( fTime, fElapsedTime );
//    }
//#endif
//
//	if ( m_pPicCharMenuPlusInfo != NULL )
//		m_pPicCharMenuPlusInfo->OnFrameMove( fElapsedTime );
//
//	if ( m_pPicCharMenuPlusInfoRed != NULL )
//		m_pPicCharMenuPlusInfoRed->OnFrameMove( fElapsedTime );
//
//	CheckMyInfoChange();
//
//	if ( m_bOpen == false )
//		return S_OK;
///*
//	//임시 코드 이거 수정해야 하는데 아직 답이 벗ㅇ다. ㅡ,ㅡㅋ
//	if ( m_pDLGMyInfoQuickSlotNumber != NULL )
//	{
//		if ( m_NowCharacterRoomState == CX2CharacterRoom::CRS_INVENTORY )
//		{
//			m_pDLGMyInfoQuickSlotNumber->SetEnable( !(g_pMain->GetNewQuestUI()->GetOpenQuestPopUpWindow()) );
//			m_pDLGMyInfoQuickSlotNumber->SetShow( !(g_pMain->GetNewQuestUI()->GetOpenQuestPopUpWindow()) );
//		}
//		else if ( m_NowCharacterRoomState == CX2CharacterRoom::CRS_SKILL_TREE )
//		{
//			m_pDLGMyInfoQuickSlotNumber->SetEnable( false );
//			m_pDLGMyInfoQuickSlotNumber->SetShow( false );
//		}
//	}
//*/
//
//	
//	m_pSkillTreeUI->OnFrameMove( fTime, fElapsedTime );
//
//	CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
//
//	UnitHandling( fTime, fElapsedTime );
//
//
//	if ( m_bPlayGetRandomItem == true )
//	{
//		m_fPlayTime += fElapsedTime;
//
//		if( m_hSeqKeyTrace != INVALID_PARTICLE_SEQUENCE_HANDLE && m_hMeshInstRandomBoxKey != NULL )
//		{
//			if( g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstRandomBoxKey ) == false )
//			{
//				m_hMeshInstRandomBoxKey = INVALID_MESH_INSTANCE_HANDLE;
//				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqKeyTrace );
//			}
//			else if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqKeyTrace ) == false )
//			{
//				m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
//			}
//			else
//			{
//				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqKeyTrace );
//				if( NULL != pSeq )
//				{
//					CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstRandomBoxKey );
//					if( NULL != pMeshInst )
//					{
//						pSeq->SetPosition( pMeshInst->GetPos() );
//					}
//				}
//				else
//				{
//					m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
//				}
//			}
//		}
//
//
//		if( m_hMeshInstRandomBox != NULL && g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstRandomBox ) == true )
//		{
//			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstRandomBox );
//
//			if( NULL != pMeshInst &&
//				pMeshInst->EventTimerGlobal( 3.3f ) == true )
//			{
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"BoxJewel", 0,0,-100 );
//				pTemp->SetOverUI( true );
//				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Lobby_Chest01", 0,0,-100 );
//				pTemp->SetOverUI( true );
//			}
//		}
//
//
//		if ( m_fPlayTime >= m_fPlayMaxTime )
//		{
//			m_bPlayGetRandomItem = false;
//			m_fPlayTime = 0.0f;
//
//			if ( m_pDLGGetRandomItemBG != NULL )
//			{
//				m_pDLGGetRandomItemBG->Move( D3DXVECTOR2( 0,0 ), D3DXCOLOR( 0,0,0,0 ), 0.5f, true, true );
//				m_pDLGGetRandomItemBG = NULL;
//			}
//
//			if ( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstRandomBox )
//			{
//				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstRandomBox );
//			}
//
//			UpdateOpenRandomItemAck();
//			
//		}
//	}
//
//#ifdef ATTRACTION_ITEM_TEST
//	if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE && 
//		g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachine ) == true )
//	{
//		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMeltMachine );
//		if( NULL != pMeshInst &&
//			g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqSpreadLight ) == false )
//		{
//			
//			D3DXVECTOR3 pos = pMeshInst->GetXSkinAnim()->GetCloneFramePosition( L"Object05" );
//			CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_machine01", pos );
//			pTemp->SetOverUI(true);
//			pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			m_hSeqSpreadLight = pTemp->GetHandle();
//		}
//	}
//	else
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
//		m_hSeqSpreadLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
//	}
//
//
//
//	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_Start = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMeltMachineStart );
//
//	if( NULL != pMeshInst_Start &&
//		pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"End" )
//	{
//		if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqEndLight ) == false )
//		{
//			D3DXVECTOR3 pos = pMeshInst_Start->GetXSkinAnim()->GetCloneFramePosition( L"Sphere01" );
//			pos.z = pos.z - 100;
//			pos.y = pos.y - 10;
//			CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_machine_End02", pos );
//			pTemp->SetOverUI(true);
//			pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			m_hSeqEndLight = pTemp->GetHandle();
//		}
//	}
//	else
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEndLight );
//		m_hSeqEndLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
//	}
//
//
//
//	if ( m_bPlayAttractionItem == true )
//	{
//		m_fPlayTime += fElapsedTime;
//
//		if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE )
//		{
//			if( g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == false )
//			{
//				m_hMeshInstMeltMachineStart = INVALID_MESH_INSTANCE_HANDLE;
//				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
//			}
//			else if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqSpreadLight ) == false )
//			{
//				m_hSeqSpreadLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
//			}
//		}
//
//
//		// 파티클 하드코딩.. ㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠ
//		if( pMeshInst_Start != NULL )
//		{
//			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.0f ) == true )
//			{
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"LightParticle_Fire_Machine_Start05", 0,-40,0 );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			}
//			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.33f ) == true )
//			{
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"LightParticle_Fire_Machine_Start04", 0,40,0 );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			}
//			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 5.53f ) == true )
//			{ 
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"LightParticle_Fire_Machine_Start03", 0,40,0 );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_Machine_Start02", 0,80,0);
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_Machine_Start01", 0,80,0 );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			}
//			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"End" && pMeshInst_Start->EventTimerGlobal( 0.12f ) == true )
//			{
//				D3DXVECTOR3 pos = pMeshInst_Start->GetXSkinAnim()->GetCloneFramePosition( L"Sphere01" );
//				pos.z = pos.z - 100;
//				pos.y = pos.y - 10;
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"LightParticle_Fire_Machine_End01", pos );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//				m_hSeqEndLight = pTemp->GetHandle();
//				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_machine_End02", pos );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			}
//			
//		}
//
//
//		
//		if ( m_fPlayTime >= m_fPlayMaxTime + 1.5f)
//		{
//			m_bPlayAttractionItem = false;
//			m_fPlayTime = 0.0f;
//
//			if ( m_pDLGGetAttractionItemBG != NULL )
//			{
//				m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2( 0,0 ), D3DXCOLOR( 0,0,0,0 ), 0.5f, true, true );
//				m_pDLGGetAttractionItemBG = NULL;
//			}
//
//			if ( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE )
//			{
//				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
//			}
//
//			UpdateOpenAttractionItemAck();
//
//		}
//	}
//#endif
//
//	//OnFrameMove_VP_EXP( fTime, fElapsedTime );
//
//	if ( m_pDLGEnchantItemGuide != NULL )
//	{
//		if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_ENCHANT )
//		{
//			m_pDLGEnchantItemGuide->SetShowEnable( true, true );
//		}
//		else
//		{
//			m_pDLGEnchantItemGuide->SetShowEnable( false, false );
//		}
//	}
//
//	if ( m_pDLGSocketItemGuide != NULL )
//	{
//		if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_SOCKET )
//		{
//			m_pDLGSocketItemGuide->SetShowEnable( true, true );
//		}
//		else
//		{
//			m_pDLGSocketItemGuide->SetShowEnable( false, false );
//		}
//	}
//
//	if ( m_pDLGAttribItemGuide != NULL )
//	{
//		if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_ATTRIB_ENCHANT )
//		{
//			m_pDLGAttribItemGuide->SetShowEnable( true, true );
//		}
//		else
//		{
//			m_pDLGAttribItemGuide->SetShowEnable( false, false );
//		}
//	}
//
//	return S_OK;
//}
//
//bool CX2CharacterRoom::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	if ( GetCheckOperationCondition() == false )
//		return false;
//
//	switch( uMsg )
//	{
//	case WM_LBUTTONUP:
//		{
//			if ( m_pUnitViewerUI != NULL && CX2UnitViewerUI::GetUnitClicked() == true )
//			{
//				D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
//				int degree = (int)D3DXToDegree( rot.y );
//				m_fRemainRotY = D3DXToRadian( degree % 360 );
//			}
//
//			CX2UnitViewerUI::SetUnitClicked( false );
//
//#ifdef ATTRACTION_ITEM_TEST
//			if ( m_pStateMenu->GetCursor() != NULL )
//			{
//				if ( m_pStateMenu->GetCursor()->GetCurorState() != CX2Cursor::XCS_ENCHANT )
//				{
//					m_bUseCoupon = false;
//					m_CouponUid = 0;
//					m_CouponTargetUid = 0;
//				}
//			}
//#endif
//			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
//			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
//			
//			MouseUp( mousePos );
//
//
//		}
//		break;
//
//	case WM_LBUTTONDOWN:
//		{
//			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
//			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
//
//
//			if ( MouseDown( mousePos) == true )
//				return true;
//
//			if ( m_bOpen == true && IsInUnitClickReact( mousePos ) == true )
//			{
//				CX2UnitViewerUI::SetUnitClicked( true );
//				return true;
//			}
//		}
//		break;
//
//	case WM_MOUSEMOVE:
//		{
//
//			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
//			
//			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
//			m_NowMousePos = mousePos;
//
//			MouseMove( mousePos );
//		}
//		break;
//
//	case WM_RBUTTONDOWN:
//		{
//			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
//			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
//
//			SetNowDragItemReturn();
//
//			if ( m_pStateMenu->GetCursor() != NULL )
//			{
//				if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_NORMAL )
//					MouseRButtonUp( mousePos );
//				else
//				{
//					if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_REMOVE_SKILL )
//					{
//						if( NULL != m_pSkillTreeUI )
//						{
//							m_pSkillTreeUI->UpdateRemoveSkillState( false );
//							if( true == m_pSkillTreeUI->GetShow() )
//							{
//								m_pSkillTreeUI->UpdateSkillTrees();
//							}
//						}
//					}
//#ifdef ATTRACTION_ITEM_TEST
//					m_bUseCoupon = false;
//					m_CouponUid = 0;
//					m_CouponTargetUid = 0;
//#endif
//					m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//
//					return true;
//				}
//			}
//		}
//		break;
//
//	case WM_RBUTTONUP:
//		{
//			SetNowDragItemReturn();
//		}
//		break;
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	if ( m_pSkillTreeUI != NULL && /*m_NowCharacterRoomState == CRS_SKILL_TREE &&*/
//		m_pSkillTreeUI->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
//		return true;   
//
//#ifdef TITLE_SYSTEM
//    if( g_pData->GetTitleManager() != NULL && g_pData->GetTitleManager()->GetShow() == true )
//    {
//        if( g_pData->GetTitleManager()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
//            return true;
//    }
//#endif
//
//	switch(wParam)
//	{
//	case CRM_QUICK_SELL_ITEM:
//		{
//			return Handler_EGS_SELL_ITEM_REQ();
//		}
//		break;
//
//	case CRM_QUICK_SELL_ITEM_CANCEL:
//		{
//			if ( m_pDialogToRemoveItem != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDialogToRemoveItem, NULL, false );
//
//			m_pDialogToRemoveItem = NULL;
//
//			return true;
//		}
//		break;
//
//	case CRM_REPAIR_ITEM:
//		{
//			return Handler_EGS_REPAIR_ITEM_REQ();
//		}
//		break;
//
//	case CRM_OPEN_RANDOM_ITEM:
//		{
//#ifdef ATTRACTION_ITEM_TEST
//			m_bOpenedAttractionItem = false;
//#endif
//			return Handler_EGS_OPEN_RANDOM_ITEM_REQ();
//		}
//		break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_1_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//			if( 0 != pUnitData->m_EqipSkill1 )
//			{
//				//CKTDGUIDialog* pDLGSkillSlot = GetDLGSkillSlot();
//				//CKTDGUIButton* pButton = (CKTDGUIButton*) pDLGSkillSlot->GetControl( L"Skill_Slot_1" );
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill1, vPos, true );
//			}
//
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_2_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//			if( 0 != pUnitData->m_EqipSkill2 )
//			{
//				//CKTDGUIDialog* pDLGSkillSlot = GetDLGSkillSlot();
//				//CKTDGUIButton* pButton = (CKTDGUIButton*) pDLGSkillSlot->GetControl( L"Skill_Slot_2" );
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill2, vPos, true );
//			}
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_3_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			if( 0 != pUnitData->m_EqipSkill3 )
//			{
//				//CKTDGUIDialog* pDLGSkillSlot = GetDLGSkillSlot();
//				//CKTDGUIButton* pButton = (CKTDGUIButton*) pDLGSkillSlot->GetControl( L"Skill_Slot_3" );
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill3, vPos, true );
//			}
//
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_4_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			if( 0 != pUnitData->m_Eqip2Skill1 )
//			{
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill1, vPos, true );
//			}
//
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_5_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			if( 0 != pUnitData->m_Eqip2Skill2 )
//			{
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill2, vPos, true );
//			}
//
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_6_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			if( 0 != pUnitData->m_Eqip2Skill3 )
//			{
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill3, vPos, true );
//			}
//
//		} break;
//
//
//
//	case CRM_SKILL_SLOT_UNEQUIP_OUT:
//		{
//			GetSkillTreeUI()->CloseSkillInfoPopup( true );
//			return true;
//		} break;
//
//	case CRM_ALL_EQUIPPING_ITEM_REPAIR:
//		{
//			OpenDLGAllEquippingItemRepair();
//			return true;
//		}
//		break;
//
//	case CRM_ALL_EQUIPPING_ITEM_REPAIR_OK:
//		{
//			AllEquippingItemRepairREQ();
//			return true;
//		}
//		break;
//
//	case CRM_ALL_EQUIPPING_ITEM_REPAIR_CANCEL:
//		{
//			if ( m_pDLGAllEquippingItemRepair != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGAllEquippingItemRepair, NULL, false );
//				m_pDLGAllEquippingItemRepair = NULL;
//			}
//			return true;
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_OK:
//		{
//			if ( m_pDLGResolveItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGResolveItem, NULL, false );
//			}
//			m_pDLGResolveItem = NULL;
//
//			return Handler_EGS_RESOLVE_ITEM_REQ();
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_CANCEL:
//		{
//			if ( m_pDLGResolveItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGResolveItem, NULL, false );
//			}
//			m_pDLGResolveItem = NULL;
//
//			return true;
//		}
//		break;
//
//	case CRM_SKILL_JOB_LEVEL_1:
//		{
//			if( NULL != m_pSkillTreeUI )
//			{
//				switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
//				{
//				case CX2Unit::UT_ELSWORD:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ELSWORD_SWORDMAN );
//					} break;
//
//				case CX2Unit::UT_ARME:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ARME_VIOLET_MAGE );
//					} break;
//
//				case CX2Unit::UT_LIRE:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_LIRE_ELVEN_RANGER );
//					} break;
//
//				case CX2Unit::UT_RAVEN:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_RAVEN_FIGHTER );
//					} break;
//				case CX2Unit::UT_EVE:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_EVE_NASOD );
//					} break;
//
//				}
//				m_pSkillTreeUI->OpenSkillTree( true, m_pSkillTreeUI->GetPickedUnitClass() );
//			}
//			return true;
//
//		} break;
//
//	case CRM_SKILL_JOB_LEVEL_2:
//		{
//			// note!! 전직 추가되면 수정
//			if( NULL != m_pSkillTreeUI )
//			{
//				switch( g_pData->GetMyUser()->GetSelectUnit()->GetClass() )
//				{
//				default:
//					break;
//
//				case CX2Unit::UC_ELSWORD_KNIGHT:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ELSWORD_KNIGHT );
//					} break;
//
//				case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ELSWORD_MAGIC_KNIGHT );
//					} break;
//
//				case CX2Unit::UC_LIRE_COMBAT_RANGER:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_LIRE_COMBAT_RANGER );
//					} break;
//
//				case CX2Unit::UC_LIRE_SNIPING_RANGER:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_LIRE_SNIPING_RANGER );
//					} break;
//
//				case CX2Unit::UC_ARME_HIGH_MAGICIAN:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ARME_HIGH_MAGICIAN );
//					} break;
//
//				case CX2Unit::UC_ARME_DARK_MAGICIAN:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ARME_DARK_MAGICIAN );
//					} break;
//
//				case CX2Unit::UC_RAVEN_SOUL_TAKER:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_RAVEN_SOUL_TAKER );
//					} break;
//				case CX2Unit::UC_RAVEN_OVER_TAKER:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_RAVEN_OVER_TAKER );
//					} break;
//#ifdef EVE_FIRST_CHANGE_JOB
//				case CX2Unit::UC_EVE_EXOTIC_GEAR:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_EVE_EXOTIC_GEAR );
//					} break;
//				case CX2Unit::UC_EVE_ARCHITECTURE:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_EVE_ARCHITECTURE );
//					} break;
//#endif
//				}	
//				m_pSkillTreeUI->OpenSkillTree( true, m_pSkillTreeUI->GetPickedUnitClass() );
//			}
//			return true;
//
//		} break;
//
//	case CRM_USE_ENCHANT_ADJUVANT:
//		{
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
//			m_bUseEnchantAdjuvant = pCheckBox->GetChecked();
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnchantItemUID );
//			ASSERT( NULL != pItem );
//			if( NULL == pItem )
//				return true;
//
//			if( true == m_bUseEnchantAdjuvant )
//			{
//				{
//					if( pItem->GetItemData().m_EnchantLevel >= 10 )
//					{
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//							L"강화 단계가 +10 이상인 아이템에는 플루오르 스톤을 사용할 수 없습니다.", m_pNowState );
//						m_bUseEnchantAdjuvant = false;
//						pCheckBox->SetChecked( false );
//					}
//				}
//			}
//
//			if( true == m_bUseEnchantAdjuvant )
//			{
//				ASSERT( NULL != pItem->GetItemTemplet() );
//				if( NULL != pItem->GetItemTemplet() )
//				{
//					int iAdjuvantItemID = GetRequiredEnchantAdjuvantItemID( pItem->GetItemTemplet()->m_UseLevel );
//					int iAdjuvantCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iAdjuvantItemID );
//					if( iAdjuvantCount <= 0 )
//					{
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"플루오르 스톤이 부족합니다.", m_pNowState );
//						m_bUseEnchantAdjuvant = false;
//						pCheckBox->SetChecked( false );
//					}
//				}
//			}
//			UpdateEnchantWindow();
//
//			return true;
//		} break;
//
//	case CRM_ENCHANT_ITEM_OK:
//		{
//			if ( m_pDLGEnchantItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItem, NULL, false );
//			}
//			m_pDLGEnchantItem = NULL;
//
//			return Handler_EGS_ENCHANT_ITEM_REQ( false );	 
//		}
//		break;
//
//	case CRM_SPECIAL_ENCHANT_ITEM_OK:
//		{
//			if ( m_pDLGEnchantItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItem, NULL, false );
//			}
//			m_pDLGEnchantItem = NULL;
//
//			return Handler_EGS_ENCHANT_ITEM_REQ( true );
//		}
//		break;
//
//	case CRM_ENCHANT_ITEM_CANCEL:
//		{
//			if ( m_pDLGEnchantItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItem, NULL, false );
//			}
//			m_pDLGEnchantItem = NULL;
//
//			return true;
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_RESULT_OK:
//		{
//			if ( m_pDLGResolveItemResult != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGResolveItemResult, NULL, false );
//			}
//			m_pDLGResolveItemResult = NULL;
//			
//			return true;
//		}
//		break;
//
//	case CRM_ENCHANT_ITEM_RESULT_OK:
//		{
//			if ( m_pDLGEnchantItemResult != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItemResult, NULL, false );
//			}
//			m_pDLGEnchantItemResult = NULL;
//
//			return true;
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_RESULT_PREV_PAGE:
//		{
//			ChangeDecompositionPage( false );
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_RESULT_NEXT_PAGE:
//		{
//			ChangeDecompositionPage( true );
//		}
//		break;
//
//	case CRM_RESALE_ITEM_NUM_CHECK:
//		{
//			m_sbCheckQuestionSellNum = false;
//		}
//		break;
//	
//	case CRM_RESALE_ITEM_NUM_UNCHECK:
//		{
//
//			m_sbCheckQuestionSellNum = true;
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_CLOSE:
//		{
//			if ( m_pDLGSocketItem != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGSocketItem, NULL, false );
//
//			m_pDLGSocketItem = NULL;
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_PUSH_NORMAL_MAGIC_STONE:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			wstring controlName = pControl->GetName();
//			WCHAR slotNum = controlName.at( controlName.size() - 1  );
//			controlName = slotNum;
//			int socketIndex = _wtoi( controlName.c_str() );
//			socketIndex--;
//			if ( socketIndex < 0 )
//				socketIndex = 0;
//
//			m_SocketSlotIndex = socketIndex;
//			m_bSocketRareMagicStone = false;
//			m_bSocketRemove = false;
//
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
//			if ( pCheckBox != NULL )
//			{
//				if ( pCheckBox->GetChecked() == true )
//				{
//					OpenSocketCheatWindow();
//				}
//				else
//				{
//					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
//					if ( pItem != NULL )
//					{
//						wstringstream wstrstm;
//						wstrstm << L"수수료는 [" << g_pMain->GetEDString( pItem->GetEDToSocketPush() ) << L"]ED 입니다.\n소켓으로 세공 하시겠습니까?";
//						m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), CRM_SOCKET_ITEM_FEE_CHECK_OK, m_pNowState );
//					}
//				}
//			}
//
//			
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_PUSH_RARE_MAGIC_STONE:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			wstring controlName = pControl->GetName();
//			WCHAR slotNum = controlName[ controlName.size() - 1 ];
//			controlName = slotNum;
//			int socketIndex = _wtoi( controlName.c_str() );
//			socketIndex--;
//			if ( socketIndex < 0 )
//				socketIndex = 0;
//
//			m_SocketSlotIndex = socketIndex;
//			m_bSocketRareMagicStone = true;
//			m_bSocketRemove = false;
//
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
//			if ( pCheckBox->GetChecked() == true )
//			{
//				OpenSocketCheatWindow();
//			}
//			else
//			{
//				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
//				if ( pItem != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"수수료는 [" << g_pMain->GetEDString( pItem->GetEDToSocketPush() ) << L"]ED 입니다.\n소켓으로 세공 하시겠습니까?";
//					m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), CRM_SOCKET_ITEM_FEE_CHECK_OK, m_pNowState );
//				}
//			}
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_REMOVE_MAGIC_STONE:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			wstring controlName = pControl->GetName();
//			WCHAR slotNum = controlName[ controlName.size() - 1 ];
//			controlName = slotNum;
//			int socketIndex = _wtoi( controlName.c_str() );
//			socketIndex--;
//			if ( socketIndex < 0 )
//				socketIndex = 0;
//			
//
//			m_SocketSlotIndex = socketIndex;
//			m_bSocketRareMagicStone = false;
//			m_bSocketRemove = true;
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
//			if ( pItem != NULL )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"수수료는 [" << g_pMain->GetEDString( pItem->GetEDToSocketRemove() ) << L"]ED 입니다.\n마법석을 제거하시겠습니까?\n#CFF0000주의! 제거된 마법석은 사라집니다.#CX";
//				m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), CRM_SOCKET_ITEM_FEE_CHECK_OK, m_pNowState );
//			}
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_FEE_CHECK_OK:
//		{
//			if ( m_pDLGSocketItemPushOk != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGSocketItemPushOk, NULL, false );
//
//			m_pDLGSocketItemPushOk = NULL;
//			return Handler_EGS_SOCKET_ITEM_REQ( m_SocketSlotIndex, m_bSocketRareMagicStone, m_bSocketRemove );
//		}
//		break;
//
//	case CRM_CLOSE_CUBE_RESULT:
//		{
//			if ( m_pDLGCubeCheck != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGCubeCheck, NULL, false );
//
//			m_pDLGCubeCheck = NULL;
//		}
//		break;
//
//	case CRM_INVEN_PAGE_PREV:
//		{
//			PrevInvenPage();
//		}
//		break;
//
//	case CRM_INVEN_PAGE_NEXT:
//		{
//			NextInvenPage();
//		}
//		break;
//
//	case CRM_ATTRIB_ENCHANT_ITEM_CANCEL:
//		{
//			if ( m_pDLGAttribEnchantItem != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGAttribEnchantItem, NULL, false );
//
//			m_pDLGAttribEnchantItem = NULL;
//		}
//		break;
//
//	case CRM_ATTRIB_ENCHANT_ITEM_OK:
//		{
//			//Send Packet gogo
//			CKTDGUIControl* pControlItem = (CKTDGUIControl*)lParam;
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID );
//			if ( pItem != NULL )
//			{
//				if ( pItem->GetItemData().m_EnchantOption1 == 0 )
//				{
//					m_AttribEnchantSlotID = CX2EnchantItem::ESI_SLOT_1;
//					m_AttribEnchantID = pControlItem->GetDummyInt(0);
//
//					
//					wstringstream wstrstm;
//					bool bDual = false;
//					bool bWeapon = false;
//					int eDForEnchant = 0;
//					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID );
//					if ( pItem != NULL )
//					{
//						if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//						{
//							bWeapon = true;
//						}
//						else
//						{
//							bWeapon = false;
//						}
//
//						if ( pItem->GetItemData().m_EnchantOption1 == 0 && pItem->GetItemData().m_EnchantOption2 == 0 )
//						{
//							bDual = false;
//						}
//						else
//						{
//							bDual = true;
//						}
//
//						g_pData->GetEnchantItem()->GetAttribEnchantRequireED( bDual, bWeapon, 
//							pItem->GetItemTemplet()->m_UseLevel, pItem->GetItemTemplet()->m_ItemGrade, eDForEnchant );
//						wstrstm << L"수수료는 [" << eDForEnchant << L"]ED 입니다.\n인챈트를 하시겠습니까?";
//						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), wstrstm.str().c_str(), CRM_ATTRIB_ENCHANT_ITEM_OK_REP, g_pMain->GetNowState() );
//					}
//					
//
//					return true;
//				}
//				else if ( pItem->GetItemData().m_EnchantOption2 == 0 )
//				{
//					m_AttribEnchantSlotID = CX2EnchantItem::ESI_SLOT_2;
//					m_AttribEnchantID = pControlItem->GetDummyInt(0);
//					
//
//					
//					wstringstream wstrstm;
//					bool bDual = false;
//					bool bWeapon = false;
//					int eDForEnchant = 0;
//					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID );
//					if ( pItem != NULL )
//					{
//						if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//						{
//							bWeapon = true;
//						}
//						else
//						{
//							bWeapon = false;
//						}
//
//						if ( pItem->GetItemData().m_EnchantOption1 == 0 && pItem->GetItemData().m_EnchantOption2 == 0 )
//						{
//							bDual = false;
//						}
//						else
//						{
//							bDual = true;
//
//						}
//
//						g_pData->GetEnchantItem()->GetAttribEnchantRequireED( bDual, bWeapon, 
//							pItem->GetItemTemplet()->m_UseLevel, pItem->GetItemTemplet()->m_ItemGrade, eDForEnchant );
//						wstrstm << L"수수료는 [" << eDForEnchant << L"]ED 입니다.\n인챈트를 하시겠습니까?";
//						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), wstrstm.str().c_str(), CRM_ATTRIB_ENCHANT_ITEM_OK_REP, g_pMain->GetNowState() );
//					}
//
//					return true;
//				}	
//			}
//		}
//		break;
//
//	case CRM_ATTRIB_ENCHANT_ITEM_REMOVE_SLOT:
//		{
//			//remove_slot
//			CKTDGUIControl* pControlItem = (CKTDGUIControl*)lParam;
//
//			m_AttribEnchantSlotID = pControlItem->GetDummyInt(0);
//			m_AttribEnchantID = CX2EnchantItem::ET_NONE;
//			//return Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( pControlItem->GetDummyInt(0), CX2EnchantItem::ET_NONE, false );
//
//			wstringstream wstrstm;
//			bool bDual = false;
//			bool bWeapon = false;
//			int eDForEnchant = 0;
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID );
//			if ( pItem != NULL )
//			{
//				if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//				{
//					bWeapon = true;
//				}
//				else
//				{
//					bWeapon = false;
//				}
//
//				if ( pItem->GetItemData().m_EnchantOption1 != 0 && pItem->GetItemData().m_EnchantOption2 != 0 )
//				{
//					bDual = true;
//				}
//				else
//				{
//					bDual = false;
//				}
//
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireED( bDual, bWeapon, 
//					pItem->GetItemTemplet()->m_UseLevel, pItem->GetItemTemplet()->m_ItemGrade, eDForEnchant );
//
//				wstrstm << L"수수료는 [" << eDForEnchant << L"]ED 입니다.\n속성을 제거 하시겠습니까?";
//
//				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), wstrstm.str().c_str(), CRM_ATTRIB_ENCHANT_ITEM_REMOVE_SLOT_REP, g_pMain->GetNowState() );
//			}
//
//
//			return true;
//		}
//		break;
//
//	case CRM_EL_CHANGER_OK:
//		{
//			Handler_EGS_IDENTIFY_ITEM_REQ();
//
//			//패킷을 보내볼까
//			if ( m_pDLGElChanger != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGElChanger, NULL, false );
//
//			m_pDLGElChanger = NULL;
//		}
//		break;
//
//	case CRM_EL_CHANGER_CANCEL:
//		{
//			//그냥 꺼부러
//			if ( m_pDLGElChanger != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGElChanger, NULL, false );
//
//			m_pDLGElChanger = NULL;
//		}
//		break;
//
//	case CRM_EL_CHANGER_CHANGE_ITEM_COUNT:
//		{
//			//여긴 갯수 제한 걸어줘야제?
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			AdjustElChangerItemCount( pControl->GetName() );
//
//		}
//		break;
//#ifdef TITLE_SYSTEM
//    case CRM_LIST_TITLE:
//        if(g_pData->GetTitleManager() != NULL)
//        {
//            g_pData->GetTitleManager()->OpenTitle();
//        }
//        break;
//    case CRM_CANCELLATION_TITLE:
//        if(g_pData->GetTitleManager() != NULL)
//        {
//            g_pData->GetTitleManager()->DetachTitle();
//        }
//        break;
//#endif
//
//	case CRM_ATTRIB_ENCHANT_ITEM_OK_REP:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//
//			return Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( m_AttribEnchantSlotID, m_AttribEnchantID, false );
//		}
//		break;
//			
//	case CRM_ATTRIB_ENCHANT_ITEM_REMOVE_SLOT_REP:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//
//			return Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( m_AttribEnchantSlotID, m_AttribEnchantID, false );
//		}
//		break;
//
//	case CRM_EXCHANGE_OLD_ITEM_OK:
//		{
//			//{{ dmlee 2008.12.2 구 아이템 교환 이벤트기간 종료로 기능 삭제
//			//CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			//if ( pControl != NULL )
//			//	g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//			//
//			//return Handler_EGS_ITEM_EXCHANGE_REQ();
//			return true;
//			//}} dmlee 2008.12.2 구 아이템 교환 이벤트기간 종료로 기능 삭제		
//		} break;
//#ifdef ITEM_RECOVERY_TEST
//	case CRM_ITEM_RECOVERY_OK:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//
//			Handler_EGS_RESTORE_ITEM_REQ( m_RecoveryItemUid );
//			m_RecoveryItemUid = 0;
//
//			return true;
//		} break;
//#endif
//#ifdef ATTRACTION_ITEM_TEST
//	case CRM_ATTRACTION_ITEM_DUMMY:
//		{
//			// 더미 : ESC를 흡수하기 위함. 아무것도 하지 않는다.
//			return true;
//		} break;
//	case CRM_ATTRACTION_ITEM_OK:
//		{
//			// 돌아가는 중에는 메시지를 받지 말자~
//			if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE &&  
//				g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == true )
//				return true;
//
//			m_bOpenedAttractionItem = true;
//
//			return Handler_EGS_OPEN_RANDOM_ITEM_REQ();
//		}
//		break;
//	case CRM_ATTRACTION_ITEM_CANCLE:
//		{
//			// 돌아가는 중에는 메시지를 받지 말자~
//			if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE &&  
//				g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == true )
//				return true;
//
//			if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
//			{
//				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//			}
//
//			if ( m_pDLGOpenAttraction != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGOpenAttraction, NULL, false );
//				m_pDLGOpenAttraction = NULL;
//			}
//			
//			m_bOpenedAttractionItem = false;
//
//			return true;
//		}
//		break;
//	case CRM_ATTRACTION_ITEM_RESULT_OK:
//		{
//			if ( m_pDLGAttractionResult != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGAttractionResult, NULL, false );
//			m_pDLGAttractionResult = NULL;
//
//			return true;
//		}
//		break;
//	case CRM_USE_COUPON_ITEM_OK:
//		{
//			Handler_EGS_ATTACH_ITEM_REQ( m_CouponUid, m_CouponTargetUid );		
//
//			m_bUseCoupon = false;
//			m_CouponUid = 0;
//			m_CouponTargetUid = 0;
//			if(m_pStateMenu->GetCursor() != NULL)
//				m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//
//			return true;
//		}
//		break;
//	case CRM_USE_COUPON_ITEM_CANCLE:
//		{
//			if ( m_pStateMenu->GetCursor() != NULL )
//			{
//				m_bUseCoupon = false;
//				m_CouponUid = 0;
//				m_CouponTargetUid = 0;
//				if(m_pStateMenu->GetCursor() != NULL)
//					m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//			}
//			
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//			return true;
//	
//		}
//		break;
//	case CRM_OPEN_RANDOM_ITEM_CANCLE:
//		{
//			// 물리치자 나쁜 놈
//			if ( m_pDLGRandomItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGRandomItem, NULL, false );
//				m_pDLGRandomItem = NULL;
//			}
//
//		}
//		break;
//#endif
//	}
//	return false;
//}
//
//
//
//
//bool CX2CharacterRoom::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	switch ( wParam )
//	{
//
//		case EGS_GET_SKILL_ACK:
//			{
//				return Handler_EGS_GET_SKILL_ACK( hWnd, uMsg, wParam, lParam );
//			} 
//			break;
//
//		case EGS_SELL_ED_ITEM_ACK:
//			{
//				return Handler_EGS_SELL_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_CHANGE_SKILL_SLOT_ACK:
//			{
//				return Handler_EGS_CHANGE_SKILL_SLOT_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_CHANGE_SKILL_SLOT_NOT:
//			{
//				return Handler_EGS_CHANGE_SKILL_SLOT_NOT( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_REPAIR_ITEM_ACK:
//			{
//				return Handler_EGS_REPAIR_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//			//{{ 2007. 10. 8  최육사  랜덤 아이템 ACK
//		case EGS_OPEN_RANDOM_ITEM_ACK:
//			{
//				return Handler_EGS_OPEN_RANDOM_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_RESOLVE_ITEM_ACK:
//			{
//				return Handler_EGS_RESOLVE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//			//}}
//
//		case EGS_ENCHANT_ITEM_ACK:
//			{
//				return Handler_EGS_ENCHANT_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_SOCKET_ITEM_ACK:
//			{
//				return Handler_EGS_SOCKET_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_ATTRIB_ENCHANT_ITEM_ACK:
//			{
//				return Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_IDENTIFY_ITEM_ACK:
//			{
//				return Handler_EGS_IDENTIFY_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//        case EGS_USE_ITEM_IN_INVENTORY_ACK:
//            {
//                return Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( hWnd, uMsg, wParam, lParam );
//            }
//            break;
//
//		case EGS_ITEM_EXCHANGE_ACK:
//			{
//				return Handler_EGS_ITEM_EXCHANGE_ACK( hWnd, uMsg, wParam, lParam );
//			} break;
//#ifdef ITEM_RECOVERY_TEST
//		case EGS_RESTORE_ITEM_ACK:
//			{
//				return Handler_EGS_RESTORE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//
//			} break;
//
//#endif
//#ifdef ATTRACTION_ITEM_TEST
//		case EGS_ATTACH_ITEM_ACK:
//			{
//				return Handler_EGS_ATTACH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			} break;
//#endif
//	}
//
//	if( NULL != m_pSkillTreeUI )
//	{
//		if( true == m_pSkillTreeUI->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
//		{
//			return true;
//		}
//	}
//
//
//	return false;
//}
//
//bool CX2CharacterRoom::GetCheckOperationCondition()
//{
//	if ( CX2ItemSlotManager::GetCheckOperationCondition() == false )
//		return false;
//
//	if ( m_bOpen == false )
//		return false;
//
//	return true;
//}
//
//void CX2CharacterRoom::SetOpen( bool bOpen ) 
//{ 
//	m_bOpen = bOpen;
//
//	SetShow( bOpen );
//	SetEnable( bOpen );
//
//#ifdef TITLE_SYSTEM
//    if(g_pData != NULL && g_pData->GetTitleManager() != NULL)
//    {
//        g_pData->GetTitleManager()->CloseTitle();
//    }
//#endif
//
//	// note!! character room은 cx2statemenu를 상속받은 클래스에서만 사용된다고 가정
//	CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
//	if( NULL != pStateMenu )
//	{
//		if( true == bOpen )
//		{
//			pStateMenu->SetShowMouseClickUI( false, CX2StateMenu::MouseClickUI::HR_CHARACTER_ROOM );
//		}
//		else
//		{
//			pStateMenu->SetShowMouseClickUI( true, CX2StateMenu::MouseClickUI::HR_CHARACTER_ROOM );
//		}
//	}
//	
//
//	m_pSkillTreeUI->SetShow( m_bOpen );
//	m_pDLGSkillSlot->SetShow( m_bOpen );
//	m_pDLGSkillSlot->SetEnable( m_bOpen );
//
//	if ( m_pStateMenu->GetCursor() != NULL )
//		m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//	
//	//일단 내정보창을 켜면 내 상태창에 대한 정보가 나온다고 가정한다.
//	if ( m_bOpen == true )
//	{
//		ResetStat();
//		ResetNowEquipUI();
//		ResetCharRoomEDnCashUInVP();
//		UpdateLevel();
//
//		CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticID" );
//		pStaticUserInfo->GetString(0)->msg = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
//
//		ResetResurrectionStoneUI();
//
//		
//
//		SetCharacterRoomState( m_NowCharacterRoomState );
//
//		//{{ 2007. 11. 13  최육사  안쓰는 통계정리
//		// 통계를 위한 서버 패킷
//		//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_MYINFO_MYINFO_NOT );
//		//}}
//
//		g_pKTDXApp->SkipFrame();
//	}
//	else
//	{
//		m_pUnitViewerUI->SetShowObject( m_bOpen );
//
//		m_pDLGMyInfoInventory->SetShow( m_bOpen );
//		m_pDLGMyInfoInventory->SetEnable( m_bOpen );
//
//		m_pDLGMyInfoFront->SetShow( m_bOpen );
//		m_pDLGMyInfoFront->SetEnable( m_bOpen );
//
//
//		m_pDLGMyInfoQuickSlotNumber->SetShow( m_bOpen );
//		m_pDLGMyInfoQuickSlotNumber->SetEnable( m_bOpen );
//
//
//
//		// 아이템 마우스 오버 이미지를 지우자
//		m_pDLGSelectedItem->SetShow( false ); 
//		InvalidSlotDesc();
//
//		// 스킬 설명 팝업 창을 닫자
//		if( NULL != m_pSkillTreeUI )
//		{
//			m_pSkillTreeUI->CloseSkillInfoPopup();			
//			m_pSkillTreeUI->CloseSkillInfoPopup( true );
//		}
//
//		SetNowDragItemReturn();
//
//		//{{ 2007. 11. 13  최육사  안쓰는 통계정리
//		// 통계를 위한 서버 패킷
//		//g_pData->GetServerProtocol()->SendID( EGS_CLOSE_WND_MYINFO_NOT );
//		//}}
//
//		SAFE_DELETE_DIALOG( m_pDLGSelectSellNumWindowPopup );
//		SAFE_DELETE_DIALOG( m_pDLGResolveItem );
//		SAFE_DELETE_DIALOG( m_pDLGEnchantItem );
//		SAFE_DELETE_DIALOG( m_pDLGResolveItemResult );
//		SAFE_DELETE_DIALOG( m_pDLGEnchantItemResult );
//		SAFE_DELETE_DIALOG( m_pDLGSocketItem );
//		SAFE_DELETE_DIALOG( m_pDLGSocketItemCheat );
//		SAFE_DELETE_DIALOG( m_pDLGCubeCheck );
//		SAFE_DELETE_DIALOG( m_pDLGAttribEnchantItem );
//		SAFE_DELETE_DIALOG( m_pDLGElChanger );
//
//		if ( m_pDLGEnchantItemGuide != NULL )
//			m_pDLGEnchantItemGuide->SetShowEnable( false, false );
//
//		if ( m_pDLGSocketItemGuide != NULL )
//			m_pDLGSocketItemGuide->SetShowEnable( false, false );
//
//		if ( m_pDLGAttribItemGuide != NULL )
//			m_pDLGAttribItemGuide->SetShowEnable( false, false );
//	}
//}
//
//
//bool CX2CharacterRoom::SetCharacterRoomState( CHARACTER_ROOM_STATE characterRoomState )
//{
//
//	m_pDLGMyInfoInventory->SetShow( false );
//	m_pUnitViewerUI->SetShowObject( false );
//
//	m_pDLGMyInfoInventory->SetEnable( false );
//
//	m_pDLGMyInfoQuickSlotNumber->SetEnable( false );
//	m_pDLGMyInfoQuickSlotNumber->SetShow( false );
//
//	m_pSkillTreeUI->SetShow( false );
//	m_pDLGSkillSlot->SetShowEnable( false, false );
//
//	m_pDLGMyInfoFront->SetShow( false );
//	m_pDLGMyInfoFront->SetEnable( false );
//
//
//	SetEnable( false );
//	SetShow( false );
//
//
//	if ( m_pStateMenu->GetCursor() != NULL )
//		m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//
//
//	switch ( characterRoomState	)
//	{
//	/*
//	case CRS_MY_STATE:
//		{
//			SetCharacterRoomMyState();
//		}
//		break;
//*/
//	case CRS_INVENTORY:
//		{
//			SetCharacterRoomInventoryState();
//
//#ifdef TITLE_SYSTEM
//            SetTitleName();
//            SetTitleNew(false);
//#endif
//			//{{ 2007. 11. 13  최육사  안쓰는 통계정리
//			// 통계를 위한 서버 패킷
//			//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_MYINFO_INVENTORY_NOT );
//			//}}
//		}
//		break;
//
//	case CRS_QUEST:
//		{
//			SetCharacterRoomQuestState();
//
//			//{{ 2007. 11. 13  최육사  안쓰는 통계정리
//			// 통계를 위한 서버 패킷
//			//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_MYINFO_QUEST_NOT );
//			//}}
//		}
//		break;
//		
//	case CRS_SKILL_TREE:
//		{
//			SetCharacterRoomSkillTreeState();
//			
//			//{{ 2007. 11. 13  최육사  안쓰는 통계정리
//			// 통계를 위한 서버 패킷
//			//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_MYINFO_SKILLTREE_NOT );
//			//}}
//		} 
//		break;
//
//	default:
//		{
//			return false;
//		}
//		break;
//	}
//
//	return true;
//}
//
//bool CX2CharacterRoom::ChangeInventoryTab( CX2Inventory::SORT_TYPE sortType )
//{
//	return SetInventorySort( sortType );
//}
//
//bool CX2CharacterRoom::SetInventorySort( CX2Inventory::SORT_TYPE sortType, int pageNum )
//{
//	if ( m_bOpen == false )
//		return false;
//
//	if ( sortType == CX2Inventory::ST_NONE )
//		return false;
//
//	m_NowInventorySortType = sortType;
//
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	if ( pInventory == NULL )
//		return false;
//
//	int maxPageNum = pInventory->GetItemMaxNum( m_NowInventorySortType )/MY_INVENTORY_SHOW_MAX_SLOT_NUM;
//	if ( pInventory->GetItemMaxNum( m_NowInventorySortType )%MY_INVENTORY_SHOW_MAX_SLOT_NUM > 0 )
//		maxPageNum++;
//
//	ResetInvenPage( pageNum, maxPageNum );
//
//	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//	{
//		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
//		if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
//		{
//			pItemSlot->LostItemUI();
//			pItemSlot->SetShow( false );
//			pItemSlot->SetEnable( false );
//			pItemSlot->SetSortType( sortType );
//		}
//
//	}
//
//	for ( int slotID = ((pageNum-1)*MY_INVENTORY_SHOW_MAX_SLOT_NUM), slotIndex = 0; slotID < pInventory->GetItemMaxNum( m_NowInventorySortType ) && slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM; slotID++, slotIndex++ )
//	{
//		for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//		{
//			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
//			if ( pItemSlot == NULL )
//				continue;
//
//			if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlot->GetSlotID() == slotIndex )
//			{
//				pItemSlot->SetShow( true, false );
//				pItemSlot->SetEnable( true, false );
//				break;
//			}
//		}
//
//
//
//	}
//
//	
//
//	for ( int slotID = ((pageNum-1)*MY_INVENTORY_SHOW_MAX_SLOT_NUM), slotIndex = 0; slotID < pInventory->GetItemMaxNum( m_NowInventorySortType ) && slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM; slotID++, slotIndex++ )
//	{
//		CX2Item* pItem = pInventory->GetItem( m_NowInventorySortType, slotID );
//		if ( pItem == NULL )
//			continue;
//
//
//		for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//		{
//			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
//			if ( pItemSlot == NULL )
//				continue;
//
//			if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlot->GetSlotID() == slotIndex )
//			{
//				if ( pItemSlot->IsCreatedItemUI() == true )
//					pItemSlot->ResetItemUI( pItem );
//				else
//					pItemSlot->CreateItemUI( pItem );
//
//				break;
//			}
//		}
//
//
//
//	}
//
//	return true;
//}
//
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_INVENTORY_SLOT_ACK( KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK& kEvent )
//{
//	UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
//
//	m_pStateMenu->OnResetStateAndEquip();
//
//
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ITEM_EXPIRATION_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( kEvent.m_vecItemUID.empty() == true )
//		StateLog( L"ExpiredItemEmpty" );
//
//	if ( kEvent.m_vecItemUID.empty() == false )
//	{
//		wstring tempString;
//
//		for ( int i = 0; i < (int)kEvent.m_vecItemUID.size(); i++ )
//		{
//			UidType itemUID = kEvent.m_vecItemUID[i];
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID );
//
//
//			// 스킬 슬롯 B 확장 아이템인 경우에 UI update
//			if( NULL != pItem && 
//				NULL != pItem->GetItemTemplet() )
//			{
//				const MAGIC_CHANGE_SKILL_SLOT_ITEM_ID = 200880;
//				if( MAGIC_CHANGE_SKILL_SLOT_ITEM_ID == pItem->GetItemTemplet()->m_ItemID )
//				{
//					if( NULL != m_pSkillTreeUI && true == m_pSkillTreeUI->GetShow() )
//					{
//						m_pSkillTreeUI->UpdateSkillSet( CX2SkillTreeUI::SKILL_SET_A );
//					}
//					if( true == m_bOpen )
//					{
//						ResetSkillSlotUI();
//					}
//				}
//			}
//
//
//			if ( pItem != NULL )
//			{
//				tempString += L"[";
//				tempString += pItem->GetFullName();
//				tempString += L"] ";
//			}
//
//			if ( m_pUnitViewerUI != NULL )
//				m_pUnitViewerUI->RemoveEqip( itemUID );
//
//			if ( g_pSquareGame != NULL )
//			{
//				CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
//				if ( pSquareUnit != NULL )
//				{
//					pSquareUnit->GetUnitViewer()->RemoveEqip( itemUID );
//				}
//			}
//
//			if ( g_pX2Room != NULL )
//			{
//				CX2Room::SlotData* pSlotData = g_pX2Room->GetMySlot();
//				if ( pSlotData != NULL && pSlotData->m_pUnitViewer != NULL )
//				{
//					pSlotData->m_pUnitViewer->RemoveEqip( itemUID );
//				}
//			}
//
//			g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( itemUID );
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().RemoveItem( itemUID );		
//		}
//
//		if ( tempString.empty() == false )
//		{
//			tempString += L"기간이 만료되어 사라졌습니다.";
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.c_str(), m_pNowState );
//		}
//
//
//		switch( g_pMain->GetNowStateID() )
//		{
//		case CX2Main::XS_PVP_RESULT:
//		case CX2Main::XS_DUNGEON_RESULT:
//			{
//				// do nothing
//			} break;
//
//		default:
//			{
//				m_pStateMenu->OnResetStateAndEquip(); // fix!! 이 함수 꼭 호출해야 하는지.
//			}
//			break;
//		}
//		
//
//		// 슬롯 업데이트
//		ResetNowEquipUI();
//
//		// 인벤토리 업데이트
//		SetInventorySort( m_NowInventorySortType );
//
//
//		// 스탯 업데이트
//		ResetStat();
//	}
//
//	return true;
//
//}
//
//bool CX2CharacterRoom::Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid)
//{
//    KEGS_USE_ITEM_IN_INVENTORY_REQ kPacket;
//    kPacket.m_iItemUID = iUid;
//
//    g_pData->GetServerProtocol()->SendPacket( EGS_USE_ITEM_IN_INVENTORY_REQ, kPacket );
//    g_pMain->AddServerPacket( EGS_USE_ITEM_IN_INVENTORY_ACK );
//
//    return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//    // 패킷 처리
//    KSerBuffer* pBuff = (KSerBuffer*)lParam;
//    KEGS_USE_ITEM_IN_INVENTORY_ACK kEvent;
//    DeSerialize( pBuff, &kEvent );	
//
//    g_pMain->DeleteServerPacket( EGS_USE_ITEM_IN_INVENTORY_ACK );
//    {
//        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//        {
//            
//            
//            g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//            UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//            ResetCharRoomEDnCashUInVP();
//
//            m_pStateMenu->OnChangedInventory();
//
//            // paul, Can you show me how to use the binding machine?
//            // sure, It's really easy, first you need to make sure the pages are collated and stacked properly like this.
//            // ok
//            // then, after punching the holes, insert the coil binder in to the holes like this.,
//
//            // what number do I press for room-service~?
//
//
//
//            return true;
//        }
//    }
//
//    return false;
//}
//
//void CX2CharacterRoom::SetUnitClicked( bool bClicked )
//{
//	if ( bClicked == false )
//	{
//		if ( CX2UnitViewerUI::GetUnitClicked() == true && m_pUnitViewerUI != NULL )
//		{
//			D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
//			int degree = (int)D3DXToDegree( rot.y );
//			m_fRemainRotY = D3DXToRadian( degree % 360 );
//		}
//	}
//
//	CX2UnitViewerUI::SetUnitClicked( bClicked );
//}
//
//void CX2CharacterRoom::SetNowDragItemReturn()
//{
//	if ( m_pSlotBeforeDragging != NULL )
//	{
//		if ( m_pSlotBeforeDragging->GetSlotType() == CX2Slot::ST_EQUIPPED )
//		{
//			if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( m_DraggingItemUID ) == true )
//				m_pUnitViewerUI->AddEqip( m_DraggingItemUID  );
//
//			ResetStat();
//		}
//
//		m_pSlotBeforeDragging->ResetPos();
//
//		if ( m_pSlotBeforeDragging->GetDialog() != NULL )
//			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
//	}
//	
//
//
//	m_pSlotBeforeDragging = NULL;
//	m_DraggingItemUID = -1;
//}
//
//void CX2CharacterRoom::ResourcePreLoad()
//{
//	/*
//	ResourcePreLoad( CX2Inventory::ST_EQUIP );
//	ResourcePreLoad( CX2Inventory::ST_AVARTA );
//	ResourcePreLoad( CX2Inventory::ST_E_EQUIP );
//	*/
//
//	
//	vector< CKTDXDevice* > vecReadyDevice;
//
//	for ( int i = 0; i < CX2Inventory::ST_END; i++ )
//	{
//		ResourcePreLoad( (CX2Inventory::SORT_TYPE)i, vecReadyDevice );
//	}
//
//	ClearDeviceList();
//
//	m_pReadyDeviceList = vecReadyDevice;
//
//	SoundReady( L"Item_Pick_And_Drop.ogg" );
//	SoundReady( L"Throw_Garbage.ogg" );
//	SoundReady( L"Skill_Getting.ogg" );
//	SoundReady( L"Skill_Setting2.ogg" );
//
//	SoundReady( L"LevelUp.ogg" );
//	SoundReady( L"LevelUpBGM.ogg" );
//
//	SoundReady( L"TreasureBox_Open.ogg" );
//
//	SoundReady( L"ItemUpgrade_Success.ogg" );
//	SoundReady( L"ItemUpgrade_Disappear.ogg" );
//	SoundReady( L"ItemUpgrade_DownGrade.ogg" );
//	SoundReady( L"ItemUpgrade_Initial.ogg" );
//	SoundReady( L"ItemUpgrade_NoChange.ogg" );
//
//	XMeshReady( L"Cubic_Key.Y" );
//	XSkinMeshReady( L"Motion_Cubic.x" );
//	TextureReady( L"Motion_Cubic2.tga" );
//	TextureReady( L"Motion_Cubic3.tga" );
//	TextureReady( L"Motion_Cubic4.tga" );
//	TextureReady( L"Motion_Cubic5.tga" );
//	TextureReady( L"Motion_Cubic6.tga" );
//	TextureReady( L"Motion_Cubic7.tga" );
//	TextureReady( L"Motion_Cubic8.tga" );
//	TextureReady( L"Motion_Cubic9.tga" );
//	TextureReady( L"Motion_Cubic10.tga" );
//	TextureReady( L"Motion_Cubic11.tga" );
//
//	TextureReady( L"Arme_Damage_Center.dds" );
//}
//
//
//void CX2CharacterRoom::ClearDeviceList()
//{
//	for( int i = 0; i < (int)m_pReadyDeviceList.size(); i++ )
//	{
//		CKTDXDevice* pDevice = m_pReadyDeviceList[i];
//		SAFE_CLOSE( pDevice );
//	}
//	m_pReadyDeviceList.clear();
//}
//
//
//bool CX2CharacterRoom::IsInEquippedItemSlotReact( D3DXVECTOR2 mousePos )
//{
//	if ( CX2ItemSlotManager::IsInEquippedItemSlotReact( mousePos ) == true )
//		return true;
//
//
//	D3DXVECTOR2 m_ConvertLeftTop		= g_pKTDXApp->ConvertByResolution( m_vQuickSlotWindowPos );
//	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( m_vQuickSlotWindowSize );
//
//	if ( mousePos.x >= m_ConvertLeftTop.x && mousePos.x <= m_ConvertLeftTop.x + m_ConvertSize.x &&
//		mousePos.y >= m_ConvertLeftTop.y && mousePos.y <= m_ConvertLeftTop.y + m_ConvertSize.y )
//	{
//		return true;
//	}
//
//	return false;
//}
//
//
//
//bool CX2CharacterRoom::Handler_EGS_DELETE_ITEM_REQ()
//{
//	//KEGS_DELETE_ITEM_REQ kPacket;
//	//kPacket.m_cSortType = m_TempSortTypeToRemove;
//	//kPacket.m_sSlotID = m_TempSlotIDToRemove;
//
//	//g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_ITEM_REQ, kPacket );
//	//g_pMain->AddServerPacket( EGS_DELETE_ITEM_ACK );
//
//	return true;
//}
//
//
//
//bool CX2CharacterRoom::Handler_EGS_DELETE_ITEM_ACK( KEGS_DELETE_ITEM_ACK& kEvent )
//{
//	//g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDialogToRemoveItem, NULL, false );
//	//m_pDialogToRemoveItem = NULL;
//
//	//UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//	//	
//	//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Throw_Garbage.ogg" );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_SELL_ITEM_REQ()
//{
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	if( NULL == pInventory )
//		return false;
//
//	CX2Item* pItem = pInventory->GetItem( m_TempItemUID );
//	if( NULL == pItem )
//		return false;
//
//	KEGS_SELL_ED_ITEM_REQ kPacket;
//	kPacket.m_iItemUID = pItem->GetItemData().m_ItemUID;
//	kPacket.m_iQuantity = m_SellItemNum;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_SELL_ED_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_SELL_ED_ITEM_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_SELL_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	// 다이얼로그 처리
//	if ( m_pDialogToRemoveItem != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDialogToRemoveItem, NULL, false );
//
//	m_pDialogToRemoveItem = NULL;
//
//	// 패킷 처리
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_SELL_ED_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	g_pMain->DeleteServerPacket( EGS_SELL_ED_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			std::vector< KInventoryItemInfo > vecInventoryItemInfo;
//			vecInventoryItemInfo.push_back( kEvent.m_kInventorySlotInfo );
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( vecInventoryItemInfo );
//			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED		= kEvent.m_iED;		
//
//			UpdateInventorySlotList( vecInventoryItemInfo );
//
//			ResetCharRoomEDnCashUInVP();
//			
//			m_pStateMenu->OnChangedInventory();
//
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_REPAIR_ITEM_REQ()
//{
//	if ( m_pDLGToRepair != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGToRepair, NULL, false );
//	
//	m_pDLGToRepair = NULL;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)m_TempSortType, m_TempSlotID );
//	if ( pItem != NULL )
//	{
//		int edToRepair = pItem->GetEDToRepair();
//		int vpToRepair = pItem->GetVPToRepair();
//
//
//		float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
//		if( fDiscountRate > 0.f )
//		{
//			edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
//		}
//
//
//
//		if ( edToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"ED가 부족하여 수리할 수 없습니다!", m_pNowState );
//			return true;
//		}
//		if ( vpToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"VP가 부족하여 수리할 수 없습니다!", m_pNowState );
//			return true;
//		}
//
//		UidType itemUID = pItem->GetUID();
//		KEGS_REPAIR_ITEM_REQ kPacket;
//		kPacket.m_vecItemUID.push_back( itemUID );
//
//		g_pData->GetServerProtocol()->SendPacket( EGS_REPAIR_ITEM_REQ, kPacket );
//		g_pMain->AddServerPacket( EGS_REPAIR_ITEM_ACK );
//	}
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_REPAIR_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_REPAIR_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	// 다이얼로그 처리
//	if ( m_pDLGToRepair != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGToRepair, NULL, false );
//	
//	m_pDLGToRepair = NULL;
//
//	g_pMain->DeleteServerPacket( EGS_REPAIR_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
//			if ( pUnit != NULL )
//			{
//				pUnit->AccessUnitData().m_ED = kEvent.m_iED;
//				pUnit->AccessUnitData().m_VSPoint = kEvent.m_iVP;
//
//				UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//				ResetCharRoomEDnCashUInVP();
//				ResetNowEquipUI();
//				m_pStateMenu->OnResetStateAndEquip();
//
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"수리 되었습니다.", m_pNowState );
//			}	
//		}
//	}
//
//	return true;
//}
//
//
//
//bool CX2CharacterRoom::Handler_EGS_GET_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	// 패킷 처리
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_GET_SKILL_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	if( NULL == m_pSkillTreeUI )
//		return false;
//
//	CX2SkillTree::SKILL_ID eSkillID = m_pSkillTreeUI->GetGetSkillID();
//
//
//
//	g_pMain->DeleteServerPacket( EGS_GET_SKILL_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			// 게임 룸 상태일 경우 처리
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			
//			pUnitData->m_GetSkillList.push_back( eSkillID );
//			pUnitData->m_iSPoint = kEvent.m_iSPoint;
//
//
//			// 내 케릭터일 경우, UI 업데이트
//			if( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData() == pUnitData )
//			{
//				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Getting.ogg", false, false );
//
//				m_pSkillTreeUI->UpdateInfo();
//				m_pSkillTreeUI->UpdateSkillTrees();
//
//				if( kEvent.m_vecUpdatedItem.size() > 0 )
//				{
//					UpdateInventorySlotList( kEvent.m_vecUpdatedItem, false );
//				}
//
//				CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
//				switch( pSkillTemplet->m_Type )
//				{
//				case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
//				case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
//				case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
//				case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
//					{
//						ResetStat();
//					} break;
//				}
//			}
//
//
//			// 스킬 슬롯이 비었거나 장착된 스킬보다 높은 레벨의 동일 스킬이면 바로 장착
//			if( NULL != m_pSkillTreeUI )
//			{
//				m_pSkillTreeUI->EquipSkillWhenGetSkill( eSkillID );
//			}
//			
//			return true;
//		}
//	}
//	
//	return false;
//}
////
////bool CX2CharacterRoom::Handler_EGS_GET_SKILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
////{
////	// 패킷 처리
////	KSerBuffer* pBuff = (KSerBuffer*)lParam;
////	KEGS_GET_SKILL_NOT kEvent;
////	DeSerialize( pBuff, &kEvent );	
////
////	// 게임 룸 상태일 경우 처리
////	CX2Unit::UnitData* pUnitData = NULL;
////	if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM )
////	{
////		CX2StatePVPRoom* pkStatePVPRoom = (CX2StatePVPRoom*) m_pStateMenu;
////		CX2PVPRoom* pkPVPRoom = pkStatePVPRoom->GetCX2PVPRoom();
////		CX2Room::SlotData* pkSlotData = pkPVPRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
////		if ( pkSlotData != NULL )
////			pUnitData = pkSlotData->m_pUnit->GetUnitData();
////	}
////	else if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_ROOM )
////	{
////		CX2StateDungeonRoom* pkStateDungeonRoom = (CX2StateDungeonRoom*) m_pStateMenu;
////		CX2DungeonRoom* pkDungeonRoom = pkStateDungeonRoom->GetCX2DungeonRoom();
////		CX2Room::SlotData* pkSlotData = pkDungeonRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
////		if ( pkSlotData != NULL )
////			pUnitData = pkSlotData->m_pUnit->GetUnitData();
////	}
////
////	if ( pUnitData == NULL )
////		return false;
////
////	if( NULL != m_pSkillTreeUI )
////	{
////		pUnitData->m_GetSkillList.push_back( kEvent.m_iSkillID );
////	}
////
////	return true;
////}
////
//
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_SKILL_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	// 패킷 처리
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_CHANGE_SKILL_SLOT_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	g_pMain->DeleteServerPacket( EGS_CHANGE_SKILL_SLOT_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			WCHAR wszText[256] = L"";
//
//
//			// 게임 룸 상태일 경우 처리
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			
//			switch( kEvent.m_iSlotID )
//			{
//			case 0:
//				{
//					pUnitData->m_EqipSkill1 = kEvent.m_iSkillID;
//				} break;
//			case 1:
//				{
//					pUnitData->m_EqipSkill2 = kEvent.m_iSkillID;
//				} break;
//			case 2:
//				{
//					pUnitData->m_EqipSkill3 = kEvent.m_iSkillID;
//				} break;
//
//			case 3:
//				{
//					pUnitData->m_Eqip2Skill1 = kEvent.m_iSkillID;
//				} break;
//			case 4:
//				{
//					pUnitData->m_Eqip2Skill2 = kEvent.m_iSkillID;
//				} break;
//			case 5:
//				{
//					pUnitData->m_Eqip2Skill3 = kEvent.m_iSkillID;
//				} break;
//			}
//
//
//			// 타 케릭터일 경우, UI 업데이트 스킵
//			if ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData() != pUnitData )
//				return true;
//
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Setting2.ogg", false, false );
//			ResetSkillSlotUI();
//			m_pSkillTreeUI->UpdateSkillTrees();
//
//			
//			if( 0 == kEvent.m_iSkillID )
//			{
//				if( true == m_pSkillTreeUI->GetRemoveSkillAfterUnEqip() )
//				{
//					m_pSkillTreeUI->SetRemoveSkillAfterUnEqip( false );
//					m_pSkillTreeUI->Handler_EGS_RESET_SKILL_REQ( m_pSkillTreeUI->GetRemoveSkillID() );
//				}
//			}
//					
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_SKILL_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	// 패킷 처리
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_CHANGE_SKILL_SLOT_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	WCHAR wszText[256] = L"";
//
//	//CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_Skill_Slot" );
//
//	// 게임 룸 상태일 경우 처리
//	CX2Unit::UnitData* pUnitData = NULL;
//
//	if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM )
//	{
//		CX2StatePVPRoom* pkStatePVPRoom = (CX2StatePVPRoom*) m_pStateMenu;
//		CX2PVPRoom* pkPVPRoom = pkStatePVPRoom->GetCX2PVPRoom();
//		CX2Room::SlotData* pkSlotData = pkPVPRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
//		if ( pkSlotData != NULL )
//		{
//			pUnitData = pkSlotData->m_pUnit->GetUnitData();
//		}
//	}
//	else if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_ROOM ||
//		g_pMain->GetNowStateID() == CX2Main::XS_ARCADE_ROOM )
//	{
//		CX2StateDungeonRoom* pkStateDungeonRoom = (CX2StateDungeonRoom*) m_pStateMenu;
//		CX2DungeonRoom* pkDungeonRoom = pkStateDungeonRoom->GetCX2DungeonRoom();
//		CX2Room::SlotData* pkSlotData = pkDungeonRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
//		if ( pkSlotData != NULL )
//		{
//			pUnitData = pkSlotData->m_pUnit->GetUnitData();
//		}
//	}
//
//	if ( pUnitData == NULL )
//		return false;
//
//	switch( kEvent.m_iSlotID )
//	{
//	case 0:
//		{
//			pUnitData->m_EqipSkill1 = kEvent.m_iSkillID;
//		} break;
//	case 1:
//		{
//			pUnitData->m_EqipSkill2 = kEvent.m_iSkillID;
//		} break;
//	case 2:
//		{
//			pUnitData->m_EqipSkill3 = kEvent.m_iSkillID;
//		} break;
//
//	case 3:
//		{
//			pUnitData->m_Eqip2Skill1 = kEvent.m_iSkillID;
//		} break;
//	case 4:
//		{
//			pUnitData->m_Eqip2Skill2 = kEvent.m_iSkillID;
//		} break;
//	case 5:
//		{
//			pUnitData->m_Eqip2Skill3 = kEvent.m_iSkillID;
//		} break;
//	}
//
//	return true;
//}
//
//void CX2CharacterRoom::PlayGetRandomItem() 
//{
//	if ( m_bPlayGetRandomItem == false )
//	{
//		g_pKTDXApp->GetDeviceManager()->PlaySound( L"TreasureBox_Open.ogg", false, false );
//
//		m_bPlayGetRandomItem = true;
//		m_pDLGGetRandomItemBG = new CKTDGUIDialog( m_pNowState, L"DLG_Unit_State_RandomItem_BG.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGetRandomItemBG );
//		m_pDLGGetRandomItemBG->SetColor( D3DXCOLOR( 0,0,0,0) );
//		m_pDLGGetRandomItemBG->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 0,0,0,0.7f), 0.5f, true );
//
//		D3DXVECTOR3 boxPos = m_pDLGGetRandomItemBG->GetDummyPos( 0 );
//
//		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_RandomBoxKey = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"RewardKey", boxPos.x, 300.0f, boxPos.z - 150.0f , 0,0,0, 0,0,0 );
//		pMeshInst_RandomBoxKey->SetOverUI( true );
//		m_hMeshInstRandomBoxKey = pMeshInst_RandomBoxKey->GetHandle();
//		
//		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_RandomBox = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"RewardItemBox", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
//		pMeshInst_RandomBox->SetOverUI( true );
//		m_hMeshInstRandomBox = pMeshInst_RandomBox->GetHandle();
//
//
//		//랜덤 상자에 따라 텍스쳐 바꿔주자
//		if( pMeshInst_RandomBox->GetXSkinMesh()->GetXET() != NULL )
//		{
//			switch( m_RandomItemID )
//			{
//				//전사
//				case 105000:
//				case 105010:
//				case 105020:
//				case 105030:
//				case 105040:
//				case 105050:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic.tga" );
//					break;
//
//				//궁수
//				case 105200:
//				case 105210:
//				case 105220:
//				case 105230:
//				case 105240:
//				case 105250:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic5.tga" );
//					break;
//
//				//법사
//				case 105400:
//				case 105410:
//				case 105420:
//				case 105430:
//				case 105440:
//				case 105450:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic2.tga" );
//					break;
//
//				//전사고급
//				case 105600:
//				case 105610:
//				case 105620:
//				case 105630:
//				case 105640:
//				case 105650:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic6.tga" );
//					break;
//
//				//궁수고급
//				case 105800:
//				case 105810:
//				case 105820:
//				case 105830:
//				case 105840:
//				case 105850:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic10.tga" );
//					break;
//
//				//법사고급
//				case 106000:
//				case 106010:
//				case 106020:
//				case 106030:
//				case 106040:
//				case 106050:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic7.tga" );
//					break;
//
//				//엘의상자
//				case 129020:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic4.tga" );
//					break;
//
//				//아리엘의 선물 - 분홍 상자
//				case 129030:
//				case 129040:
//				case 129050:
//				case 129220:
//				case 129260:
//				case 129270:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic3.tga" );
//					break;
//
//				//회색상자
//				case 108000:
//				case 108010:
//				case 108020:
//				case 108030:
//				case 108040:
//				case 108050:
//				case 108060:
//				case 108070:
//				case 108080:
//				case 108090:
//				case 108100:
//				case 108200:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic11.tga" );
//					break;
//	
//				//부활석 상자 - 흰색상자
//				case 129070:
//				case 129230:
//				case 129240:
//				case 129250:
//				case 129280:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic12.tga" );
//					break;
//
//			}
//		}
//		
//
//		m_hSeqKeyTrace = g_pData->GetUIMajorParticle()->CreateSequenceHandle( L"KeyTrace", pMeshInst_RandomBoxKey->GetPos() );
//		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqKeyTrace );
//		if( NULL != pSeq )
//		{
//			pSeq->SetOverUI( true );
//		}
//		else
//		{
//			m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
//		}
//
//		if ( m_pDLGRandomItem != NULL )
//		{
//			g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGRandomItem, NULL, false );
//			m_pDLGRandomItem = NULL;
//		}
//	}
//}
//
//
////{{ 2007. 10. 8  최육사  
//bool CX2CharacterRoom::Handler_EGS_OPEN_RANDOM_ITEM_REQ()
//{
//	if ( m_iRandomItemUID == 0 )
//		return false;
///*
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	for ( int i = CX2Inventory::ST_EQUIP; i <= CX2Inventory::ST_AVARTA; i++ )
//	{
//		if ( pInventory->GetUsedSlotNum( (CX2Inventory::SORT_TYPE)i ) >= pInventory->GetItemMaxNum( (CX2Inventory::SORT_TYPE)i ) )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"큐브를 사용하시려면 모든 소지품 카테고리에 각각 한칸 이상의 빈공간이 있어야 합니다.", g_pMain->GetNowState() );
//			return false;
//		}
//	}
//*/	
//
//	KEGS_OPEN_RANDOM_ITEM_REQ kEGS_OPEN_RANDOM_ITEM_REQ;
//
//	kEGS_OPEN_RANDOM_ITEM_REQ.m_iItemUID  = m_iRandomItemUID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_OPEN_RANDOM_ITEM_REQ, kEGS_OPEN_RANDOM_ITEM_REQ );
//	g_pMain->AddServerPacket( EGS_OPEN_RANDOM_ITEM_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_OPEN_RANDOM_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_OPEN_RANDOM_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( m_pDLGRandomItem != NULL )
//	{
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGRandomItem, NULL, false );
//		m_pDLGRandomItem = NULL;
//	}
//
//	g_pMain->DeleteServerPacket( EGS_OPEN_RANDOM_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			m_TempOpenRandomItemAck = kEvent;
////{{ 2009.01.20 김태완
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//			if(g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV)
//			{
//				if(m_bUsingRandomItemAutoOpen)	// 자동 오픈을 사용중이면
//				{
//					if(m_pDLGMsgBox == NULL)
//					{
//						m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"잠시 기다려 주세요", m_pNowState );
//					}
//					// 받은 아이템을 저장하고 인벤토리 업뎃해준 후 바로 동일한 Uid에 대해서 Open을 요청
//					UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
//					
//					for(std::map<int, int>::iterator i = kEvent.m_mapInsertedItem.begin(); i != kEvent.m_mapInsertedItem.end(); i++)
//					{
//						std::map<int, int>::iterator target = m_MapAutoRandomItemGet.find(i->first);
//						if(target != m_MapAutoRandomItemGet.end())
//						{
//							// 이미 얻었던 적이 있는 아이템이면 갯수를 더해주자
//							target->second = target->second + i->second;
//						}
//						else
//						{
//							// 새로 나온 아이템이면 집어 넣자
//							m_MapAutoRandomItemGet.insert( std::make_pair(i->first, i->second));
//						}
//					}
//
//					m_OpenedRandomItemCount++;
//					Handler_EGS_OPEN_RANDOM_ITEM_REQ();
//					return true;
//				}
//			}
//#endif
//
//#ifdef ATTRACTION_ITEM_TEST
//			if(m_bOpenedAttractionItem)
//			{
//				// 운영자용 스킵 코드
//				if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
//				{
//					if ( m_pDLGMyInfoInventory != NULL )
//					{
//						CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGMyInfoInventory->GetControl( L"g_pCheckBoxForceEnchant" );
//						if ( pCheckBox != NULL )
//						{
//							if ( pCheckBox->GetChecked() == true )
//							{
//								if ( m_pDLGGetAttractionItemBG != NULL )
//								{
//									m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2( 0,0 ), D3DXCOLOR( 0,0,0,0 ), 0.5f, true, true );
//									m_pDLGGetAttractionItemBG = NULL;
//								}
//
//								if ( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
//								{
//									g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//								}
//
//
//								if ( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE )
//								{
//									g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
//								}
//
//								UpdateOpenAttractionItemAck();
//								return true;								
//							}
//						}
//					}
//				}
//				
//				PlayGetAttractionItem();
//				//UpdateOpenAttractionItemAck();
//			}
//			else
//			{
//				//PlayGetRandomItem();
//				UpdateOpenRandomItemAck();	
//
//			}
//#else
//			//연출 없이 그냥 고고싱~
//
//			//PlayGetRandomItem();
//			UpdateOpenRandomItemAck();	
//#endif
//
//			return true;
//		}
//		else
//		{
////{{ 2009.01.20 김태완
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//			if(g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV)
//			{
//				if(m_bUsingRandomItemAutoOpen)	// 자동 오픈을 사용중이면
//				{
//					// 자동 오픈이 끝났습니다~
//					if(m_pDLGMsgBox != NULL)
//					{
//						SAFE_DELETE_DIALOG(m_pDLGMsgBox);
//						m_pDLGMsgBox = NULL;
//					}
//
//					WCHAR buf[256] = L"";
//					wsprintf( buf, L"랜덤아이템 %d개를 열었습니다.", m_OpenedRandomItemCount );
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), buf, m_pNowState );
//
//					UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
//					// 더미 : OpenDecompositionResultWindow 안에서 인벤토리 정리를 안 하기 위한 빈 벡터 =3=
//					std::vector<KInventoryItemInfo> vecDummyInfo;
//					vecDummyInfo.clear();
//					OpenDecompositionResultWindow(m_MapAutoRandomItemGet, vecDummyInfo, false);
//					// 다 쓴 물건은 치웁시다
//					m_bUsingRandomItemAutoOpen = false;
//					m_OpenedRandomItemCount = 0;
//					m_MapAutoRandomItemGet.clear();
//				}
//				
//			}
//#endif
//
//#ifdef ATTRACTION_ITEM_TEST
//			// 실패-_-시 창을 없애자~
//			if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
//			{
//				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//			}
//
//			if ( m_pDLGOpenAttraction != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGOpenAttraction, NULL, false );
//				m_pDLGOpenAttraction = NULL;
//			}
//			m_bOpenedAttractionItem = false;
//#endif
//		}
//
//	}
//
//	return false;
//}
////}}
//
//bool CX2CharacterRoom::Handler_EGS_RESOLVE_ITEM_REQ()
//{
//	KEGS_RESOLVE_ITEM_REQ kPacket;
//
//	kPacket.m_iItemUID  = m_DecompositionItemUID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_RESOLVE_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_RESOLVE_ITEM_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_RESOLVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_RESOLVE_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( g_pMain->DeleteServerPacket( EGS_RESOLVE_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			OpenDecompositionResultWindow( kEvent.m_mapInsertedItem, kEvent.m_vecKInventorySlotInfo, kEvent.m_bJackpot );
//
//			return true;
//		}		
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ENCHANT_ITEM_REQ( bool bSpecialEnchant )
//{
//	KEGS_ENCHANT_ITEM_REQ kPacket;
//
//	kPacket.m_iItemUID				= m_EnchantItemUID;
//	kPacket.m_bIsRareEnchantStone	= false;				// obsolete!! 사용안하는 변수
//	kPacket.m_bIsNewEnchant			= bSpecialEnchant;
//	kPacket.m_bDebug				= false;
//	kPacket.m_bIsSupportMaterial	= m_bUseEnchantAdjuvant;		// 강화 보조제
//
//
//	if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
//	{
//		if ( m_pDLGMyInfoInventory != NULL )
//		{
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGMyInfoInventory->GetControl( L"g_pCheckBoxForceEnchant" );
//			if ( pCheckBox != NULL )
//			{
//				if ( pCheckBox->GetChecked() == true )
//				{
//					kPacket.m_bDebug = true;
//				}
//			}
//		}
//	}
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_ENCHANT_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_ENCHANT_ITEM_ACK );
//	
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ENCHANT_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( g_pMain->DeleteServerPacket( EGS_ENCHANT_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			OpenEnchantResultWindow( kEvent );
//
//			return true;
//		}		
//	}
//	
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_SOCKET_ITEM_REQ( int socketIndex, bool bIsRareMagicStone, bool bIsRemoveSocketOption )
//{
//	KEGS_SOCKET_ITEM_REQ kPacket;
//
//
//	kPacket.m_iItemUID = m_SocketItemUID;
//	kPacket.m_iSocketIndex = socketIndex;
//	kPacket.m_bIsRareMagicStone = bIsRareMagicStone;
//	kPacket.m_bIsRemoveSocketOption = bIsRemoveSocketOption;
//	kPacket.m_bDebug = false;
//	kPacket.m_sDebugSocketOption = 0;
//	
//	if ( bIsRemoveSocketOption == false )
//	{
//		if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
//		{
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket");
//			if ( pCheckBox != NULL )
//			{
//				if ( pCheckBox->GetChecked() == true )
//				{
//					kPacket.m_bDebug = true;
//
//					//kPacket.m_sDebugSocketOption = (short)
//					CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*)m_pDLGSocketItemCheat->GetControl(L"EditBoxPassword");
//					if ( pEditBox != NULL )
//					{
//						int socketOption = _wtoi( pEditBox->GetText() );
//						kPacket.m_sDebugSocketOption = (short)socketOption;
//					}
//					
//				}
//			}
//		}
//	}
//
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_SOCKET_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_SOCKET_ITEM_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_SOCKET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_SOCKET_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	CloseSocketCheatWindow();
//
//	if ( g_pMain->DeleteServerPacket( EGS_SOCKET_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			vector< short > vecOrgSocketOption;
//			bool bCheckNewElementOption = false;
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEvent.m_iSocketItemUID );
//			if ( pItem != NULL )
//			{
//				vecOrgSocketOption = pItem->GetItemData().m_SocketOption;
//			}
//
//			//특수 처리 고고싱
//			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//
//			pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEvent.m_iSocketItemUID );
//			if ( pItem != NULL )
//			{
//				vector< short > vecNewSocketOption = pItem->GetItemData().m_SocketOption;
//				for ( int i = 0; i < (int)vecNewSocketOption.size(); i++ )
//				{
//					int newSocketOption = vecNewSocketOption[i];
//					bool bCheck = true;
//					for ( int j = 0; j < (int)vecOrgSocketOption.size(); j++ )
//					{
//						int orgSocketOption = vecOrgSocketOption[j];
//						if ( newSocketOption == orgSocketOption )
//						{
//							bCheck = false;
//						}
//					}
//					if ( bCheck == true )
//					{
//						const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( newSocketOption );
//						if ( pSocketData != NULL )
//						{
//							if ( CX2DamageManager::EDT_NONE != pSocketData->m_SocketExtraDamage.m_ExtraDamageType && 
//								pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
//							{
//								bCheckNewElementOption = true;
//							}	
//						}
//					}
//				}
//
//				if ( bCheckNewElementOption == true )
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"속성 공격은 중복되어 적용되지 않습니다.", m_pNowState );
//			}
//
//			ResetCharRoomEDnCashUInVP();
//
//			ResetSocketWindow();
//
//			return true;
//		}		
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( int slotNum, int enchantID, bool bDebug )
//{
//	KEGS_ATTRIB_ENCHANT_ITEM_REQ kPacket;
//	kPacket.m_iItemUID = m_AttribEnchantItemUID;
//	kPacket.m_cAttribEnchantSlotNo = slotNum;
//	kPacket.m_cAttribEnchantID = enchantID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_ATTRIB_ENCHANT_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK );
//	
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ATTRIB_ENCHANT_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//
//
//	if ( g_pMain->DeleteServerPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			//특수 처리 고고싱
//			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//
//			ResetCharRoomEDnCashUInVP();
//
//			ResetAtrribEnchantWindow();
//
//		}
//
//		return true;
//	}
//	
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_IDENTIFY_ITEM_REQ()
//{
//	KEGS_IDENTIFY_ITEM_REQ kPacket;
//
//
//	kPacket.m_iItemUID = m_ElChangerItemUID;
//	kPacket.m_iQuantity = m_ElChangerItemCount;
//	
//	g_pData->GetServerProtocol()->SendPacket( EGS_IDENTIFY_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_IDENTIFY_ITEM_ACK );
//	
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_IDENTIFY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_IDENTIFY_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( g_pMain->DeleteServerPacket( EGS_IDENTIFY_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			OpenDecompositionResultWindow( kEvent.m_mapInsertedItem, kEvent.m_vecKInventorySlotInfo, false, true );
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
//
//
//
//bool CX2CharacterRoom::Handler_EGS_ITEM_EXCHANGE_REQ()
//{
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	if( NULL == pInventory )
//		return false;
//
//	CX2Item* pItem = pInventory->GetItem( m_TempItemUID );
//	if( NULL == pItem )
//		return false;
//
//	KEGS_ITEM_EXCHANGE_REQ kPacket;
//	kPacket.m_iItemUID	= pItem->GetItemData().m_ItemUID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_EXCHANGE_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_ITEM_EXCHANGE_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ITEM_EXCHANGE_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	g_pMain->DeleteServerPacket( EGS_ITEM_EXCHANGE_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
//			if ( pUnit != NULL )
//			{
//				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//				UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//				m_pStateMenu->OnChangedInventory();
//
//				
//				CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEvent.m_kChangedItemInfo.m_iItemID );
//				if( NULL != pItemTemplet )
//				{
//					WCHAR wszMsg[512] = L"";
//					StringCchPrintfW( wszMsg, sizeof(wszMsg), L"+%d %s를 획득했습니다.", kEvent.m_kChangedItemInfo.m_cEnchantLevel, 
//						pItemTemplet->GetFullName().c_str() );
//
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wszMsg, m_pNowState );
//				}
//			}	
//		}
//	}
//
//	return true;
//}
//
//
//
//bool CX2CharacterRoom::MouseDown( D3DXVECTOR2 mousePos )
//{
//	CX2SlotItem* pSlot = (CX2SlotItem*)GetSlotInMousePos( mousePos );
//
//	if ( pSlot == NULL )
//		return false;
//
//	if ( pSlot->IsResetItemUI() == false )
//	{
//		return false;
//	}
//    
//	CX2Cursor* pCursor = m_pStateMenu->GetCursor();
//	if ( pCursor != NULL )
//	{
//		bool bCheck = false;
//
//		//{{ 2009.01.19 김태완 : 코드정리
//		switch(pCursor->GetCurorState())
//		{
//		case CX2Cursor::XCS_SELL:
//			{
//				bCheck = true;
//				OpenSellWindow( pSlot );
//			}break;
//		case CX2Cursor::XCS_REPAIR:
//			{
//				bCheck = true;
//				OpenRepairWindow( pSlot );
//			} break;
//		case CX2Cursor::XCS_DECOMPOSITION:
//			{
//				bCheck = true;
//				OpenDecompositionWindow( pSlot );
//			} break;
//		case CX2Cursor::XCS_ENCHANT:
//			{
//#ifdef ATTRACTION_ITEM_TEST
//				if(m_bUseCoupon)
//				{
//					// 처리하고
//
//					if( pSlot->GetSortType() == CX2Inventory::ST_EQUIP )
//					{
//						m_CouponTargetUid = pSlot->GetItemUID();
//						CX2Item* pCouponItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_CouponUid );
//						CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_CouponTargetUid );
//
//						if(pCouponItem != NULL || pTargetItem != NULL)
//						{
//
//							wstringstream wstrstm;
//							wstrstm << L"[+" << pTargetItem->GetItemData().m_EnchantLevel << L" " << pTargetItem->GetItemTemplet()->GetFullName().c_str() << L"]에\n";
//							wstrstm << L"[" << pCouponItem->GetItemTemplet()->GetFullName().c_str() << L"] 아이템을\n";
//							wstrstm << L"사용 하시겠습니까?";
//
//							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrstm.str().c_str(), CRM_USE_COUPON_ITEM_OK, m_pNowState, CRM_USE_COUPON_ITEM_CANCLE );
//
//							bCheck = true;
//						}
//					}
//					else
//					{
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"장비 아이템만 강화할 수 있습니다.", m_pNowState );
//						bCheck = true;
//					}
//				}
//				else
//				{
//					bCheck = true;
//					OpenEnchantWindow( pSlot );
//				}
//#else
//				bCheck = true;
//				OpenEnchantWindow( pSlot );
//#endif				
//			} break;
//		case CX2Cursor::XCS_SOCKET:
//			{
//				bCheck = true;
//				OpenSocketWindow( pSlot );
//			} break;
//		case CX2Cursor::XCS_ATTRIB_ENCHANT:
//			{
//				bCheck = true;
//				OpenAtrribEnchantWindow( pSlot );
//			} break;
//		case CX2Cursor::XCS_ITEM_EXCHANGE:
//			{
//				bCheck = true;
//				OpenExchangeOldItemWindow( pSlot );
//			} break;
//		default:
//			{
//
//			} break;
//		}
//		//}} elseif -> switch
//
//		if ( bCheck == true )
//			return true;
//	}
//
//	m_pSlotBeforeDragging = pSlot;
//	//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pSlotBeforeDragging->GetDialog(), true );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetDraggingItemLayer() );
//	m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );
//
//	m_DraggingItemUID = pSlot->GetItemUID();
//
//	if ( m_pSlotBeforeDragging->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{		
//
//		g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( m_DraggingItemUID );
//		m_pUnitViewerUI->RemoveEqip( m_DraggingItemUID );
//
//
//		ResetStat();
//		ResetRoomStateToInventory( m_DraggingItemUID );
//
//	}
//
//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
//
//	return true;
//}
//
//
//bool CX2CharacterRoom::MouseUp( D3DXVECTOR2 mousePos )
//{
//	if ( m_pSlotBeforeDragging != NULL )
//	{
//		// fix!! 스킬을 dragging 중이면 
//		CX2SlotItem* pSlotItem = (CX2SlotItem*) m_pSlotBeforeDragging;
//		if( CX2Inventory::ST_E_SKILL == pSlotItem->GetSortType() ) 
//		{
//			if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
//			{
//				g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetItemLayer() );
//				m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
//
//				m_pSlotBeforeDragging->ResetPos();
//			}
//
//			m_pSlotBeforeDragging = NULL;
//			m_DraggingItemUID = -1;
//
//			return true;
//		}
//
//		bool bCheck = false;
//
//		// 장비창 영역일 경우
//		if ( IsInEquippedItemSlotReact( mousePos ) == true  )
//		{
//			bCheck = OnDropItemAnyToEquip( mousePos );
//
//		}
//		// 인벤토리 영역일 경우
//		else
//		{
//			bCheck = OnDropItemAnyToInven( mousePos );
//
//		}
//
//		// 장비창과, 인벤토리 영역에서 처리되지 않았을 경우
//		if ( bCheck == false )
//		{
//			OnDropItemAnyToOther( mousePos );
//		}
//
//		if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
//		{
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetItemLayer() );
//			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
//		}
//
//		m_pSlotBeforeDragging = NULL;
//		m_DraggingItemUID = -1;
//
//		return true;
//	}
//
//
//	if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
//	{
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetItemLayer() );
//		m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
//	}
//
//	m_pSlotBeforeDragging = NULL;
//	m_DraggingItemUID = -1;
//
//	return false;
//}
//
//void CX2CharacterRoom::MouseMove( D3DXVECTOR2 mousePos )
//{
//	bool bCheck = false;
//	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//	{
//		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
//		if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
//			continue;
//
//		if ( pItemSlot->IsInMousePoint( mousePos ) == true )
//		{
//			m_pNowOverItemSlot = pItemSlot;
//			bCheck = true;
//		}
//	}
//
//	if ( bCheck == false )
//		m_pNowOverItemSlot = NULL;
//
//	CX2SlotManager::MouseMove( mousePos );
//}
//
//
//
//bool CX2CharacterRoom::MouseRButtonUp( D3DXVECTOR2 mousePos )
//{
//	m_pSlotBeforeDragging = NULL;
//	m_DraggingItemUID = -1;
//
//	if ( IsInEquippedItemSlotReact( mousePos ) == true )
//	{
//		return OnRClickedItemInEquip( mousePos );
//	}
//	else
//	{
//		return OnRClickedItemInInven( mousePos );
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::OnDropItemAnyToEquip( D3DXVECTOR2 mousePos )
//{
//	ResetRoomStateToInventory( m_DraggingItemUID );
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	CX2Item* pItem = pInventory->GetItem( m_DraggingItemUID );
//
//	CX2SlotItem* pItemSlot = NULL;
//	
//	if ( pInventory->GetItem( m_DraggingItemUID ) != NULL )
//	{
//		CX2Item::ITEM_TYPE itemType = pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_ItemType;
//		if (  itemType == CX2Item::IT_WEAPON || itemType == CX2Item::IT_DEFENCE || itemType == CX2Item::IT_ACCESSORY ||
//			itemType == CX2Item::IT_SKILL )
//		{
//			CX2Unit::EQIP_POSITION equipPosition = pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_EqipPosition;
//
//			for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//			{
//				CX2SlotItem* pTempItemSlot = (CX2SlotItem*)GetSlot(i);
//				if ( pTempItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED && pTempItemSlot->GetEquipPos() == equipPosition && 
//					pTempItemSlot->GetFashion() == pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_bFashion )
//				{
//					pItemSlot = pTempItemSlot;
//					break;
//				}
//			}
//		}
//	}
//	
//
//	if ( pItemSlot == NULL )
//		pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//
////	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
//		return false;
//
//	if ( pItemSlot->GetEquipPos() == pItem->GetItemTemplet()->m_EqipPosition &&
//		pItemSlot->GetFashion() == pItem->GetItemTemplet()->m_bFashion )
//	{
//		if ( pItemSlot == m_pSlotBeforeDragging )
//		{
//			if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( m_DraggingItemUID ) == true )
//				m_pUnitViewerUI->AddEqip( m_DraggingItemUID );
//
//			m_pSlotBeforeDragging->ResetPos();
//
//			ResetStat();
//
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
//		}	
//		else
//		{
//#ifdef ITEM_RECOVERY_TEST
//			if( true == pItem->IsDisabled() )
//			{
//				return false;
//			}
//#endif
//			if ( pItem->GetItemTemplet()->m_PeriodType == CX2Item::PT_ENDURANCE && pItem->GetItemData().m_Endurance <= 0 )
//			{
//				return false;
//			}
//			else
//			{
//				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ(
//					((CX2SlotItem*)m_pSlotBeforeDragging),
//					pItemSlot );
//
//				CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) m_pSlotBeforeDragging;
//				pkSlotBeforeDragging->DestroyItemUI();
//			}
//		}
//
//		return true;
//	}	
//	
//	return false;
//}
//
//bool CX2CharacterRoom::OnDropItemAnyToInven( D3DXVECTOR2 mousePos )
//{
//	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
//		return false;
//
//	if ( pItemSlot == m_pSlotBeforeDragging )
//	{
//		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
//		return false;
//	}
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	CX2Item* pItem = pInventory->GetItem( m_DraggingItemUID );
//
//	// 해당 타입의 카테고리인지 체크
//	if ( m_NowInventorySortType == pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) )
//	{
//		// 장비창에서 넘어온 경우
//		if ( m_pSlotBeforeDragging->GetSlotType() == CX2Slot::ST_EQUIPPED )
//		{
//			// 대상이 빈 슬롯일 경우
//			if ( pItemSlot->IsResetItemUI() == false )
//			{
//				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)m_pSlotBeforeDragging),
//					pItemSlot );
//
//				CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) m_pSlotBeforeDragging;
//				pkSlotBeforeDragging->DestroyItemUI();
//				return true;
//			}
//			// 대상이 이미 있는 경우, 비어잇는 슬롯을 검색
//			else
//			{
//				for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//				{
//					CX2SlotItem* pItemSlotToPush = (CX2SlotItem*)GetSlot(j);
//					if ( pItemSlotToPush->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlotToPush->GetDialog() == NULL )
//					{
//						Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)m_pSlotBeforeDragging), 
//							pItemSlotToPush );
//
//						CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) m_pSlotBeforeDragging;
//						pkSlotBeforeDragging->DestroyItemUI();
//						return true;
//					}
//				}
//			}
//		}
//		// 인벤토리간의 교환일 경우
//		else
//		{
//			Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)m_pSlotBeforeDragging), 
//				pItemSlot );
//
//			CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) m_pSlotBeforeDragging;
//			pkSlotBeforeDragging->DestroyItemUI();
//			return true;
//		}
//	}
//	else
//	{
//		//잘못된 타입의 인벤토리로 들어온경우
//		return false;
//	}
//	
//	return false;
//}
//
//void CX2CharacterRoom::OnDropItemAnyToOther( D3DXVECTOR2 mousePos )
//{
//	if ( m_pSlotBeforeDragging != NULL )
//	{
//
//
//		if ( m_pSlotBeforeDragging->GetSlotType() == CX2Slot::ST_EQUIPPED )
//		{
//			if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( m_DraggingItemUID ) == true )
//				m_pUnitViewerUI->AddEqip( m_DraggingItemUID  );
//
//			ResetStat();
//		}
//
//		m_pSlotBeforeDragging->ResetPos();
//	}
//}
//
//bool CX2CharacterRoom::OnRClickedItemInEquip( D3DXVECTOR2 mousePos )
//{
//	// 장비 아이템일 경우 처리 : 장착 해제
//	CX2SlotItem* pItemSlot = NULL;
//	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//	if ( pItemSlot == NULL )
//		return false;
//
//	if ( pItemSlot->GetSlotType() != CX2SlotManager::CX2Slot::ST_EQUIPPED )
//		return false;
//
//	CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
//	if ( pkItemTemplet == NULL )
//		return false;
//
//	ResetRoomStateToInventory( pItemSlot->GetItemUID() );
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	if ( pInventory != NULL )
//	{
//		for ( int i = 0; i < pInventory->GetItemMaxNum( m_NowInventorySortType ); i++ )
//		{
//			CX2Item* pItem = pInventory->GetItem( m_NowInventorySortType, i );
//			if ( pItem == NULL )
//			{
//				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pItemSlot->GetSortType(), pItemSlot->GetSlotID(),
//													m_NowInventorySortType, i );
//
//				pItemSlot->DestroyItemUI();
//				return true;
//			}
//		}
//	}
//	
//
//
//	return false;
//}
//
//bool CX2CharacterRoom::OnRClickedItemInInven( D3DXVECTOR2 mousePos )
//{
//	// 마우스업된 위치가 인벤 안일 경우
//	CX2SlotItem* pItemSlot = NULL;
//	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//	if ( pItemSlot == NULL )
//		return false;
//
//	if ( pItemSlot->GetSlotType() != CX2SlotManager::CX2Slot::ST_INVENTORY || pItemSlot->IsResetItemUI() == false )
//		return false;
//
//	CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
//	if ( pkItemTemplet == NULL )
//		return false;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );
//
//#ifdef ITEM_RECOVERY_TEST
//	// 사용불능인가여
//	if( pItem != NULL && true == pItem->IsDisabled() )
//	{
//		// 1. 복원 아이템이 있는지 확인한다.
//		int RecoveryItemID = GetRequiredRecoveryStoneID( pkItemTemplet->m_UseLevel );
//		int iRecoveryItemCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( RecoveryItemID );
//
//		if( iRecoveryItemCount > 0 )
//		{
//			m_RecoveryItemUid = pItem->GetUID();
//			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), L"[축복 받은 복원의 주문서] 아이템을 사용해서 \n아이템을 복원하시겠습니까?", CRM_ITEM_RECOVERY_OK, m_pNowState );
//		}
//		else
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"사용 불가 상태의 아이템입니다.\n[축복 받은 복원의 주문서] 아이템을 사용해서\n사용 불가 상태를 원상태로 복원할 수 있습니다.", m_pNowState );
//		}
//		return true; 
//	}
//#endif
//    
//	// 해당하는 슬롯을 검색, 장비창으로 이동
//	CX2SlotItem* pItemSlotEquip = NULL;
//	for ( unsigned int iS = 0; iS < m_SlotList.size(); ++iS )
//	{
//		CX2SlotItem* pItemSlotEquip = (CX2SlotItem*) m_SlotList[iS];
//		if ( pItemSlotEquip->GetSlotType() != CX2SlotManager::CX2Slot::ST_EQUIPPED )
//			continue;
//
//		
//		/*
//		else
//		{
//			CX2Item* pkEquipItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlotEquip->GetItemUID() );
//			CX2Item::ItemTemplet* pkEquipItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlotEquip->GetItemTID() );
//			if ( pkEquipItem != NULL && pkEquipItemTemplet != NULL )
//			{
//				if ( pkEquipItemTemplet->m_Quantity <= pkEquipItem->GetItemData().m_Quantity )
//					continue;
//			}
//		}
//		*/
//
//
//
//		// 장착장비류 이면
//		if ( pkItemTemplet->m_ItemType == CX2Item::IT_WEAPON || 
//			pkItemTemplet->m_ItemType == CX2Item::IT_DEFENCE || 
//			pkItemTemplet->m_ItemType == CX2Item::IT_ACCESSORY )
//		{
//			if( false == CX2Unit::CanEquipAsParts( pkItemTemplet->m_ItemID, g_pData->GetMyUser()->GetSelectUnit() ) ) // fix!! 레벨 체크는 왜 안하지?
//			{
//				continue;
//			}
//
//			//장비 아이템 내구도 다 닳았으면 거시기 해불자
//			//드래그해서 거시기 하는부분도 해결해불자
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );
//			if ( pItem != NULL )
//			{
//				if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE )
//				{
//					if ( pItem->GetItemData().m_Endurance <= 0 )
//					{
//						continue;
//					}
//				}
//			}
//			
//			if ( pItemSlotEquip->GetEquipPos() == pkItemTemplet->m_EqipPosition &&
//				pItemSlotEquip->GetFashion() == pkItemTemplet->m_bFashion )
//			{
//				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ(
//					pItemSlot, 
//					pItemSlotEquip );
//
//			pItemSlot->DestroyItemUI();
//				return true;
//			}
//		}
//		else if ( pkItemTemplet->m_ItemType == CX2Item::IT_QICK_SLOT )
//		{
//			if ( pItemSlotEquip->GetEquipPos() == CX2Unit::EP_QUICK_SLOT )
//			{
//				if ( pItemSlotEquip->IsResetItemUI() == false )
//				{
//					Handler_EGS_CHANGE_INVENTORY_SLOT_REQ(
//						pItemSlot, 
//						pItemSlotEquip );
//
//					pItemSlot->DestroyItemUI();
//					return true;
//				}
//			}
//		}
//
//		
//	}
//	
//    // 인벤내 사용가능 아이템에 대한 처리
//    bool bCanUse = false;
//    CX2Item *pUseItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );
//    if(pUseItem != NULL)
//    {
//        bCanUse = pUseItem->GetItemTemplet()->m_bCanUseInventory;
//        if(bCanUse == true)
//        {
//            //m_iItemUID
//            if(CheckUseItem(pUseItem) == true)
//            {
//                Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(pItemSlot->GetItemUID());
//                return true;
//            }
//        }
//    }
//
//	//{{ 2007. 10. 8  최육사  인벤에서 랜덤템을 오른쪽 클릭했을 경우 [임시 테스트]
//	if ( pItem != NULL )
//	{
//		const CX2Item::ItemData& kItemData = &pItem->GetItemData();
//		{
//			CX2ItemManager::RandomItemData* pRandomItem = g_pData->GetItemManager()->GetRandomItemData( kItemData.m_ItemID );
//			if( pRandomItem != NULL )
//			{		
//				// 랜덤 아이템인 경우~
////{{ 2009.01.20 김태완
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//				if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
//				{
//					if ( m_pDLGMyInfoInventory != NULL )
//					{
//						CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGMyInfoInventory->GetControl( L"g_pCheckBoxForceEnchant" );
//						if ( pCheckBox != NULL )
//						{
//							if ( pCheckBox->GetChecked() == true )
//							{
//								m_bUsingRandomItemAutoOpen = true;
//							}
//						}
//					}
//				}
//#endif
////}}
//
//				m_RandomItemID = kItemData.m_ItemID;
//				m_iRandomItemUID = pItemSlot->GetItemUID();
//
//				WCHAR buf[256] = L"";
//
//				int keyItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( pRandomItem->m_KeyItemID );
//
//				if ( keyItemNum < pRandomItem->m_RequiredKeyCount )
//				{
//					wstring itemName = L"";
//					CX2Item::ItemTemplet* pItemTemplet;
//					pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pRandomItem->m_KeyItemID );
//					itemName = pItemTemplet->GetFullName();
//					wsprintf( buf, L"[%s] %d개가 필요합니다.", itemName.c_str(), pRandomItem->m_RequiredKeyCount );
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), buf, m_pNowState );
//					return false;
//				}
//
//#ifdef ATTRACTION_ITEM_TEST
//
//				int MAGIC_ATTRACTION_ITEM_ID = 108900;
//				if( MAGIC_ATTRACTION_ITEM_ID == kItemData.m_ItemID )
//				{
//					// 여기 추가...
//					m_pDLGOpenAttraction = new CKTDGUIDialog( m_pNowState, L"DLG_Ice_Heater_Use.lua" );
//					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOpenAttraction );
//
//					// 중간에 모델 그려주자
//					D3DXVECTOR3 boxPos = m_pDLGOpenAttraction->GetDummyPos( 0 );
//					CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"FireMachineWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
//					pMeshInst_MeltMachine->SetOverUI(true);
//					m_hMeshInstMeltMachine = pMeshInst_MeltMachine->GetHandle();
//				}
//				else
//				{
//					m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), L"아이템을 사용하겠습니까?", CX2CharacterRoom::CRM_OPEN_RANDOM_ITEM, m_pNowState, CRM_OPEN_RANDOM_ITEM_CANCLE );
//				}
//#else
//				m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), L"아이템을 사용하겠습니까?", CX2CharacterRoom::CRM_OPEN_RANDOM_ITEM, m_pNowState );
//#endif
//			}
//			else // 랜덤 큐브가 아니면
//			{
//#ifdef ATTRACTION_ITEM_TEST
//				if( IsEnchantCoupon(kItemData.m_ItemID))
//				{
//					// 강화권 쓰는 곳
//					if(m_pStateMenu->GetCursor() != NULL)
//						m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_ENCHANT );
//					m_bUseCoupon = true;
//					m_CouponUid = pItemSlot->GetItemUID();
//                    
//
//					// 탭 바꿔주자..
//					ChangeInventoryTab( CX2Inventory::ST_EQUIP );
//
//					wstring radioButtonName = GetRadioButtonNameByInvenSortType( CX2Inventory::ST_EQUIP );
//					CKTDGUIRadioButton* pRadioButtonInvenSortType = (CKTDGUIRadioButton*)m_pDLGMyInfoInventory->GetControl( radioButtonName.c_str() );
//					if ( pRadioButtonInvenSortType != NULL )
//					{
//						pRadioButtonInvenSortType->GetCheckedControl()->SetChecked( false );
//						pRadioButtonInvenSortType->SetChecked( true );
//					}
//
//
//				}
//#endif
//				const int MAGIC_RESET_ALL_SKILL_ITEM_ID = 203800;	
//				//{{ 2009.01.19 김태완 : 코드정리
//				switch(kItemData.m_ItemID)
//				{
//				case MAGIC_RESET_ALL_SKILL_ITEM_ID:
//					{
//						if( NULL != m_pSkillTreeUI )
//						{
//							m_pSkillTreeUI->OpenResetAllSkillPopup( true );
//						}
//					} break;
//				case CX2EnchantItem::ATI_RED:
//				case CX2EnchantItem::ATI_BLUE:
//				case CX2EnchantItem::ATI_GREEN:
//				case CX2EnchantItem::ATI_WIND:
//				case CX2EnchantItem::ATI_LIGHT:
//				case CX2EnchantItem::ATI_DARK:
//					{
//						//거시기 있는지 확인해보고 ..환원기
//						if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE ) > 0 )
//						{
//							OpenElChangerWindow( false, pItem );
//						}
//						else
//						{
//							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"엘 속성 환원기가 없습니다!", g_pMain->GetNowState() );
//						}
//					} break;
//				case CX2EnchantItem::ATI_UNKNOWN:
//					{
//						//거시기 있는지 확인해보자. 감정기
//						if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE ) > 0 )
//						{
//							OpenElChangerWindow( true, pItem );
//						}
//						else
//						{
//							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"엘 속성 판별기가 없습니다!", g_pMain->GetNowState() );
//						}
//					} break;
//				default:
//					break;
//				}
//				//}} elseif -> switch
//
//			}
//		}
//	}
//	//}}
//
//	return false;
//}
//
//void CX2CharacterRoom::DrawSlotMouseOverImage()
//{
//	if ( GetCheckOperationCondition() == false )
//	{
//		m_pDLGSelectedItem->SetShow( false );
//		return;
//	}
//
//	bool bCheck = false;
//
//	if ( m_pSlotBeforeDragging != NULL )
//	{
//		if ( IsInEquippedItemSlotReact( m_NowMousePos ) == true )
//		{
//			CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//			CX2Item::ITEM_TYPE itemType = pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_ItemType;
//			if (  itemType == CX2Item::IT_WEAPON || itemType == CX2Item::IT_DEFENCE || itemType == CX2Item::IT_ACCESSORY ||
//				itemType == CX2Item::IT_SKILL )
//			{
//				CX2Unit::EQIP_POSITION equipPosition = pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_EqipPosition;
//
//				for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//				{
//					CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
//					if ( pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED && pItemSlot->GetEquipPos() == equipPosition && 
//						pItemSlot->GetFashion() == pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_bFashion )
//					{
//						m_pDLGSelectedItem->SetPos( pItemSlot->GetPos() );
//						m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( pItemSlot->GetSize() );
//
//						bCheck = true;
//					}
//				}
//			}
//			else
//			{
//				if ( m_pNowOverItemSlot != NULL )
//				{
//					m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
//					m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );
//
//					bCheck = true;
//				}
//			}
//		}
//		else
//		{
//			if ( m_pNowOverItemSlot != NULL )
//			{
//				m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
//				m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );
//
//				bCheck = true;
//			}
//		}
//	}
//	else
//	{
//		if ( m_pNowOverItemSlot != NULL )
//		{
//			m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
//			m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );
//
//			bCheck = true;
//		}
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
//
//}
//
//wstring CX2CharacterRoom::GetSlotItemDesc()
//{
//
//	wstring itemDesc=  L"";
//
//	if ( m_pNowOverItemSlot != NULL )
//	{
//		if ( m_pNowOverItemSlot->IsResetItemUI() == false )
//		{
//			itemDesc = m_pNowOverItemSlot->GetSlotDesc();
//		}
//		else
//		{	
//			itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
//		}
//	}
//
//	return itemDesc;
//
//
//	
//}
//
//
//void CX2CharacterRoom::UnitHandling( double fTime, float fElapsedTime )
//{
//	if( m_pUnitViewerUI != NULL )
//	{
//		float fRotSpeed = 13.0f;
//
//
//		if ( CX2UnitViewerUI::GetUnitClicked() == true )
//		{
//			D3DXVECTOR3 RotateY = D3DXVECTOR3(0, -((float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() * m_fMouseSensitivity), 0);
//			m_pUnitViewerUI->GetMatrix().RotateRel(RotateY);
//			m_bRotateReturn = false;
//
//			fRotSpeed = 13.0f;
//		}
//		else
//		{
//			D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
//
//
//
//			m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//			int addSpeed = 20;
//
//			if ( m_bRotateReturn == false )
//			{
//				fRotSpeed -= (fElapsedTime * addSpeed);
//
//				if ( fRotSpeed <= 0 )
//				{
//					fRotSpeed = fElapsedTime;
//				}
//
//				if (m_fRemainRotY >= 0.0f &&  m_fRemainRotY < 3.14f )
//				{
//
//					if ( m_fRemainRotY <= 0.0f )
//					{
//						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//						m_fRemainRotY = 0.0f;
//						m_bRotateReturn = true;
//					}
//					else
//					{
//						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
//						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));
//
//						if ( m_fRemainRotY <= 0.0f )
//						{
//							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//							m_fRemainRotY = 0.0f;
//							m_bRotateReturn = true;
//						}
//					}
//				}
//				else if ( m_fRemainRotY > 3.14f && m_fRemainRotY <= 6.18f  )
//				{
//					if ( m_fRemainRotY >= 6.18f )
//					{
//						m_bRotateReturn = true;
//						m_fRemainRotY = 0.0f;
//						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//					}
//					else
//					{
//						m_fRemainRotY += (fElapsedTime*fRotSpeed);
//						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));
//
//						if ( m_fRemainRotY >= 6.18f )
//						{
//							m_bRotateReturn = true;
//							m_fRemainRotY = 0.0f;
//							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//						}
//					}
//				}
//				else if ( m_fRemainRotY >= -3.14f && m_fRemainRotY <= 0.0f )
//				{
//					if ( m_fRemainRotY >= 0.0f )
//					{
//						m_bRotateReturn = true;
//						m_fRemainRotY = 0.0f;
//						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//					}
//					else
//					{
//						m_fRemainRotY += (fElapsedTime*fRotSpeed);
//						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));
//
//						if ( m_fRemainRotY >= 0.0f )
//						{
//							m_bRotateReturn = true;
//							m_fRemainRotY = 0.0f;
//							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//						}
//					}
//				}
//				else if (  m_fRemainRotY >= -6.28f && m_fRemainRotY <= -3.14f )
//				{
//					if ( m_fRemainRotY <= -6.28f )
//					{
//						m_bRotateReturn = true;
//						m_fRemainRotY = 0.0f;
//						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//					}
//					else
//					{
//						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
//						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));
//
//						if ( m_fRemainRotY <= -6.28f )
//						{
//							m_bRotateReturn = true;
//							m_fRemainRotY = 0.0f;
//							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//						}
//					}
//
//				}
//			}
//		}
//
//		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );
//
//		if ( m_pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
//		{
//			m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
//		}
//	}
//}
//
//void CX2CharacterRoom::ResetUnitViewerUI()
//{
//	SAFE_DELETE( m_pUnitViewerUI );
//
//	CX2UnitViewerUI::SetUnitClicked( false );
//	m_fMouseSensitivity = 0.01f;
//	m_fRemainRotY = 0.0f;
//	m_bRotateReturn = true;
//
//	m_pUnitViewerUI = new CX2UnitViewerUI();
//	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
//	m_pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
//	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );
//
//	m_pUnitViewerUI->SetUnit( g_pData->GetMyUser()->GetSelectUnit() );
//	m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250, -60, 200 ) );
//	m_pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.5f, 1.5f, 1.5f ) );
//	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
//	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
//
//	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
//
//}
//
//void CX2CharacterRoom::PlayWarningAllReady()
//{
//	if ( m_pDLGWarningAllReady != NULL )
//	{
//		if ( m_pDLGWarningAllReady->GetIsMoving() == false )
//		{
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGWarningAllReady, true );
//			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,1), 0.5f, true );
//			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,1), 2.0f );
//			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,0), 1.0f );
//			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,0), 4.0f );
//		}
//	}
//}
//
//void CX2CharacterRoom::StopWarningAllReady()
//{
//	if ( m_pDLGWarningAllReady != NULL )
//	{
//		m_pDLGWarningAllReady->MoveStop();
//		m_pDLGWarningAllReady->SetColor( D3DXCOLOR( 1,1,1,0 ) );
//	}
//}
//
//
//
//
//void CX2CharacterRoom::ResourcePreLoad( CX2Inventory::SORT_TYPE sortType, vector< CKTDXDevice* >& vecReadyDevice )
//{
//	return;
//
//	// obsolete!!
//
//	//if ( g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
//	//	return;
//
//	//CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//
//	//if ( pInventory == NULL )
//	//	return;
//
//	//for ( int slotID = 0; slotID < pInventory->GetItemMaxNum( sortType ); slotID++ )
//	//{
//	//	CX2Item* pItem = pInventory->GetItem( sortType, slotID );
//	//	if ( pItem == NULL )
//	//		continue;
//
//	//	wstring tempModelName = pItem->GetItemTemplet()->m_ModelName;
//	//	wstring tempTextureName = pItem->GetItemTemplet()->m_ShopImage;
//
//	//	if ( g_pData->GetMyUser()->GetSelectUnit()->EqipAbility( pItem ) == true )
//	//	{
//	//		if ( tempModelName.empty() == false )
//	//		{
//	//			//어태치 노멀이면 그냥 메쉬			
//	//			if ( pItem->GetItemTemplet() != NULL )
//	//			{
//	//				if ( pItem->GetItemTemplet()->m_UseType == CX2Item::UT_ATTACH_NORMAL )
//	//				{
//	//					/*
//	//					CKTDXDeviceXMesh* pModel = g_pKTDXApp->GetDeviceManager()->OpenXMesh( tempModelName );
//	//					if ( pModel != NULL )
//	//					{
//	//						vecReadyDevice.push_back( pModel );
//	//					}
//	//					*/
//	//				}
//	//				else
//	//				{
//	//					/*
//	//					CKTDXDeviceXSkinMesh* pModel = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( tempModelName );
//	//					if ( pModel != NULL )
//	//					{
//	//						vecReadyDevice.push_back( pModel );
//	//					}
//	//					*/
//	//				}
//	//			}
//	//		}
//	//	}
//
//	//	if ( tempTextureName.empty() == false )
//	//	{
//	//		/*
//	//		CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( tempTextureName );
//	//		if ( pTexture != NULL )
//	//		{
//	//			vecReadyDevice.push_back( pTexture );
//	//		}
//	//		*/
//	//	}
//	//}
//}
//
//bool CX2CharacterRoom::IsInUnitClickReact( D3DXVECTOR2 mousePos )
//{
//	D3DXVECTOR2 m_ConvertPos		= g_pKTDXApp->ConvertByResolution( m_UnitClickPos );
//	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( m_UnitClickSize );
//
//	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
//		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
//	{
//		return true;
//	}
//
//	return false;
//}
//
//
//void CX2CharacterRoom::ResetNowEquipUI()
//{
//	CX2Unit::UnitData* pMyUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//
//	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//	{
//		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
//		if ( pItemSlot != NULL && pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//		{
//			pItemSlot->DestroyItemUI();
//		}
//
//	}
//
//	if ( pMyUnitData != NULL && pInventory != NULL )
//	{
//		for ( int i = 0; i < (int)pMyUnitData->m_NowEqipItemUIDList.size(); i++ )
//		{
//			UidType uidType = pMyUnitData->m_NowEqipItemUIDList[i];
//			CX2Item* pItem = pInventory->GetItem( uidType );
//			if ( pItem == NULL )
//				continue;
//
//			for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//			{
//				CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
//				if ( pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED && pItemSlot->GetEquipPos() == pItem->GetItemTemplet()->m_EqipPosition &&
//					pItemSlot->GetFashion() == pItem->GetItemTemplet()->m_bFashion )
//				{
//					pItemSlot->CreateItemUI( pItem );
//
//					break;
//				}
//			}
//		}
//
//		for ( int i = 0; i < (int)pInventory->GetItemMaxNum( CX2Inventory::ST_E_QUICK_SLOT ); i++ )
//		{
//			CX2Item* pItem = pInventory->GetItem( CX2Inventory::ST_E_QUICK_SLOT, i );
//			if ( pItem == NULL )
//				continue;
//
//			for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//			{
//				CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
//				if ( pItemSlot->GetSortType() == CX2Inventory::ST_E_QUICK_SLOT && pItemSlot->GetSlotID() == i )
//				{
//					pItemSlot->CreateItemUI( pItem );
//					break;
//				}
//			}
//		}
//	}
//
//
//	ResetSkillSlotUI();
//}
//
//void CX2CharacterRoom::ResetStat()
//{
//	m_Stat.InitStat();
//	m_StatEqip.InitStat();
//
//	m_Stat.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetUnitStat() );
//	m_StatEqip.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetEqipStat(), true );
//	m_StatEqip.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetSkillStat(), true );
//	m_StatEqip.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetSetItemOptionStat(), true );
//	//m_StatEqip.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetEnchantDefence(), true );
//
//    
//#ifdef TITLE_SYSTEM
//    m_StatEqip.AddStat( g_pData->GetTitleManager()->GetSocketStat(), true );
//#endif
//	
//
//	// 소켓 옵션 스탯 더해주고
//
//
//	// 세트 아이템 옵션 스탯 더해주고.. 으흐 겁나게 많구나~~
//
//	WCHAR buff[256] = {0};
//
//	// 2008.10.27 인벤토리 스탯 보는 부분 수정, 예전 코드는 comment out - 김태완
//	// X2SkillTreeUI.cpp에서 긁었음
//	//{{
//
//	CKTDGUIStatic* pStaticUnitStatContent;
//	CKTDGUIStatic* pStaticUnitStatContent_add;
//	CKTDGUIStatic* pStaticUnitStatContent_NoEquip;
//
//	// hp
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"HP_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"HP_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"HP_Point_NoEquip" );
//
//	if( m_StatEqip.GetStat().m_fBaseHP == 0 &&
//		m_StatEqip.GetStat().m_ExtraStat.m_fIncreaseHPRate == 0.f )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fBaseHP );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//
//		pStaticUnitStatContent->SetShow(true);
//		pStaticUnitStatContent_add->SetShow(true);
//		pStaticUnitStatContent_NoEquip->SetShow(false);
//
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fBaseHP );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//
//
//		// 증가되는 HP 계산
//		if( m_StatEqip.GetStat().m_ExtraStat.m_fIncreaseHPRate > CX2SocketItem::SocketData::MAX_INCREASE_HP_RATE )
//			m_StatEqip.GetStat().m_ExtraStat.m_fIncreaseHPRate = CX2SocketItem::SocketData::MAX_INCREASE_HP_RATE;
//
//		float fAddHP = m_StatEqip.GetStat().m_fBaseHP;
//		fAddHP += m_Stat.GetStat().m_fBaseHP * m_StatEqip.GetStat().m_ExtraStat.m_fIncreaseHPRate;
//
//
//
//		if( fAddHP > 0.f )
//			wsprintf( buff, L"+%d", (int)fAddHP );
//		else
//			wsprintf( buff, L"-%d", (int)-fAddHP );
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//	}
//
//
//
//	// phy_attk
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Attack_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Attack_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Attack_Point_NoEquip" );
//	if( m_StatEqip.GetStat().m_fAtkPhysic == 0 )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkPhysic );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//		pStaticUnitStatContent -> SetShow(true);
//		pStaticUnitStatContent_add -> SetShow(true);
//		pStaticUnitStatContent_NoEquip -> SetShow(false);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkPhysic );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//		if( m_StatEqip.GetStat().m_fAtkPhysic > 0 )
//			wsprintf( buff, L"+%d", (int)m_StatEqip.GetStat().m_fAtkPhysic );
//		else
//			wsprintf( buff, L"-%d", abs((int)m_StatEqip.GetStat().m_fAtkPhysic ));
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//	}
//
//	// mag_attk
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Point_NoEquip" );
//	if( m_StatEqip.GetStat().m_fAtkMagic == 0 )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkMagic );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//		pStaticUnitStatContent -> SetShow(true);
//		pStaticUnitStatContent_add -> SetShow(true);
//		pStaticUnitStatContent_NoEquip -> SetShow(false);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkMagic );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//		if( m_StatEqip.GetStat().m_fAtkMagic > 0 )
//			wsprintf( buff, L"+%d", (int)m_StatEqip.GetStat().m_fAtkMagic );
//		else
//			wsprintf( buff, L"-%d", abs((int)m_StatEqip.GetStat().m_fAtkMagic) );
//
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//
//
//	}
//
//
//	// def
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Defense_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Defense_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Defense_Point_NoEquip" );
//	if( m_StatEqip.GetStat().m_fDefPhysic == 0 )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefPhysic );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//		pStaticUnitStatContent -> SetShow(true);
//		pStaticUnitStatContent_add -> SetShow(true);
//		pStaticUnitStatContent_NoEquip -> SetShow(false);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefPhysic );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//		if( m_StatEqip.GetStat().m_fDefPhysic > 0 )
//			wsprintf( buff, L"+%d", (int)m_StatEqip.GetStat().m_fDefPhysic );
//		else
//			wsprintf( buff, L"-%d", abs((int)m_StatEqip.GetStat().m_fDefPhysic) );
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//	}
//
//
//	// magicdef
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Defense_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Defense_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Defense_Point_NoEquip" );
//	if( m_StatEqip.GetStat().m_fDefMagic == 0 )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefMagic );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//		pStaticUnitStatContent -> SetShow(true);
//		pStaticUnitStatContent_add -> SetShow(true);
//		pStaticUnitStatContent_NoEquip -> SetShow(false);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefMagic );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//		if( m_StatEqip.GetStat().m_fDefMagic > 0 )
//			wsprintf( buff, L"+%d", (int)m_StatEqip.GetStat().m_fDefMagic );
//		else
//			wsprintf( buff, L"-%d", abs((int)m_StatEqip.GetStat().m_fDefMagic) );
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//
//	}
//
//
//	const CX2Stat::EnchantStat& enchantStat = g_pData->GetMyUser()->GetSelectUnit()->GetEnchantStat( true );
//
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Fire_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefBlaze, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Ice_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefWater, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Nature_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefNature, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Wind_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefWind, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Light_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefLight, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Dark_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefDark, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//
//	/*
//	CKTDGUIStatic* pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticHP_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fBaseHP == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fBaseHP );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fBaseHP > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fBaseHP, (int)m_StatEqip.GetStat().m_fBaseHP );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fBaseHP, abs( (int)m_StatEqip.GetStat().m_fBaseHP ) );
//		}
//
//		
//	}
//
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticAttack_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fAtkPhysic == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkPhysic );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkPhysic > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkPhysic, (int)m_StatEqip.GetStat().m_fAtkPhysic );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkPhysic, abs( (int)m_StatEqip.GetStat().m_fAtkPhysic ) );
//		}
//	}
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticMagic_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fAtkMagic == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkMagic );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkMagic > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkMagic, (int)m_StatEqip.GetStat().m_fAtkMagic );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkMagic, abs( (int)m_StatEqip.GetStat().m_fAtkMagic ) );
//		}
//		
//	}
//
//	
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticDefense_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fDefPhysic == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefPhysic );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefPhysic > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefPhysic, (int)m_StatEqip.GetStat().m_fDefPhysic );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefPhysic, abs( (int)m_StatEqip.GetStat().m_fDefPhysic ) );
//		}	
//
//		
//	}
//
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticMDefense_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fDefMagic == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefMagic );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefMagic > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefMagic, (int)m_StatEqip.GetStat().m_fDefMagic );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefMagic, abs( (int)m_StatEqip.GetStat().m_fDefMagic ) );
//		}
//	}
//
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	
//
//
//	CKTDGUIStatic* pStaticUnitDetailStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticPVP_Room_Unit_DetailStat_Content" );
//
//	if ( m_StatEqip.GetStat().m_fAtkFire == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkFire );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkFire > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkFire, (int)m_StatEqip.GetStat().m_fAtkFire );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkFire, abs( (int)m_StatEqip.GetStat().m_fAtkFire ) );
//		}
//
//		
//	}
//	
//	pStaticUnitDetailStatContent->GetString(0)->msg = buff;
//
//
//	if ( m_StatEqip.GetStat().m_fAtkIce == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkIce );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkIce > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkIce, (int)m_StatEqip.GetStat().m_fAtkIce );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkIce, abs( (int)m_StatEqip.GetStat().m_fAtkIce ) );
//		}
//	}
//
//	pStaticUnitDetailStatContent->GetString(1)->msg = buff;
//
//
//
//	if ( m_StatEqip.GetStat().m_fAtkLightning == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkLightning );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkLightning > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkLightning, (int)m_StatEqip.GetStat().m_fAtkLightning );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkLightning, abs( (int)m_StatEqip.GetStat().m_fAtkLightning ) );
//		}
//
//		
//	}
//
//	pStaticUnitDetailStatContent->GetString(2)->msg = buff;
//
//
//	if ( m_StatEqip.GetStat().m_fAtkEarth == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkEarth );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkEarth > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkEarth, (int)m_StatEqip.GetStat().m_fAtkEarth );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkEarth, abs( (int)m_StatEqip.GetStat().m_fAtkEarth ) );
//		}
//	}
//
//	pStaticUnitDetailStatContent->GetString(3)->msg = buff;
//
//
//	if ( m_StatEqip.GetStat().m_fDefFire == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefFire );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefFire > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefFire, (int)m_StatEqip.GetStat().m_fDefFire );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefFire, abs( (int)m_StatEqip.GetStat().m_fDefFire ) );
//		}
//
//
//	}
//
//	pStaticUnitDetailStatContent->GetString(4)->msg = buff;
//
//
//
//	if ( m_StatEqip.GetStat().m_fDefIce == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefIce );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefIce > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefIce, (int)m_StatEqip.GetStat().m_fDefIce );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefIce, abs( (int)m_StatEqip.GetStat().m_fDefIce ) );
//		}
//
//		
//	}
//
//	pStaticUnitDetailStatContent->GetString(5)->msg = buff;
//
//
//
//	if ( m_StatEqip.GetStat().m_fDefLightning == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefLightning );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefLightning > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefLightning, (int)m_StatEqip.GetStat().m_fDefLightning );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefLightning, abs( (int)m_StatEqip.GetStat().m_fDefLightning ) );
//		}
//
//		
//	}
//
//	pStaticUnitDetailStatContent->GetString(6)->msg = buff;
//
//
//
//	if ( m_StatEqip.GetStat().m_fDefEarth == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefEarth );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefEarth > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefEarth, (int)m_StatEqip.GetStat().m_fDefEarth );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefEarth, abs( (int)m_StatEqip.GetStat().m_fDefEarth ) );
//		}
//
//		
//	}
//
//	pStaticUnitDetailStatContent->GetString(7)->msg = buff;
//	*/
//
//
//}
//
//void CX2CharacterRoom::ResetCharRoomEDnCashUInVP()
//{
//
//	WCHAR buff[256] = {0};
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//	CKTDGUIStatic* pStaticPVP_Room_Unit_ED = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticED_Num" );
//	//wsprintf( buff, L"%d", (int)pUnitData->m_ED );
//	pStaticPVP_Room_Unit_ED->GetString(0)->msg = g_pMain->GetEDString( (int)pUnitData->m_ED );
//
//	pStaticPVP_Room_Unit_ED = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticVP_Num" );
//	wsprintf( buff, L"%d", (int)pUnitData->m_VSPoint  );
//	pStaticPVP_Room_Unit_ED->GetString(0)->msg = buff;
//
//	pStaticPVP_Room_Unit_ED = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticCash_Num" );
//	pStaticPVP_Room_Unit_ED->GetString(0)->msg = g_pMain->GetEDString( g_pData->GetMyUser()->GetCash() );
//}
//
//void CX2CharacterRoom::UpdateLevel()
//{
//
//	CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticLv_Num" );
//	if( NULL == pStaticUserInfo )
//		return;
//
//	if( NULL == g_pData->GetMyUser() ||
//		NULL == g_pData->GetMyUser()->GetSelectUnit() )
//		return;
//
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//	WCHAR wszText[32] = L"";
//	wsprintf( wszText, L"%d", (int)pUnitData->m_Level );
//	pStaticUserInfo->GetString(0)->msg = wszText;
//
//
//}
//
//
//void CX2CharacterRoom::RemoveNoEnduranceNowEquip()
//{
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//
//	if ( pInventory != NULL )
//	{
//		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//		{
//			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
//			if ( pItemSlot != NULL && pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//			{
//				if ( pItemSlot->GetSortType() == CX2Inventory::ST_E_EQUIP )
//				{
//					UidType itemUID = pItemSlot->GetItemUID();
//					CX2Item* pItem = pInventory->GetItem( itemUID );
//					if ( pItem != NULL )
//					{
//						const CX2Item::ItemData& kItemData = &pItem->GetItemData();
//						if ( kItemData.m_PeriodType == CX2Item::PT_ENDURANCE )
//						{
//							if ( kItemData.m_Endurance == 0 )
//							{
//								m_pUnitViewerUI->RemoveEqip( kItemData.m_ItemUID );
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//
//void CX2CharacterRoom::ResetResurrectionStoneUI()
//{
//	CKTDGUIStatic* pStaticResurrectionNum = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"resurrectionNum" );
//	if ( pStaticResurrectionNum != NULL && pStaticResurrectionNum->GetString(0) != NULL )
//		pStaticResurrectionNum->GetString(0)->msg = g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() );	
//}
//
//void CX2CharacterRoom::ResetSpirit()
//{
//	if ( m_pDLGMyInfoFront != NULL )
//	{
//		CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//		if ( pUnitData != NULL )
//		{
//			CKTDGUIStatic* pStaticSpiritGauge = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"SpiritGauge" );
//			if ( pStaticSpiritGauge != NULL && pStaticSpiritGauge->GetPicture(0) != NULL )
//			{
//				float _width = pStaticSpiritGauge->GetPicture(0)->GetOriginalSize().x * ( pUnitData->m_iSpirit / (float)pUnitData->m_iSpiritMax );
//
//				pStaticSpiritGauge->GetPicture(0)->SetSizeX( _width );
//			}
//
//			CKTDGUIButton* pButtonSpiritGauge = (CKTDGUIButton*)m_pDLGMyInfoFront->GetControl( L"SpiritButton" );
//			if ( pButtonSpiritGauge != NULL )
//			{
//				stringstream strstm;
//				strstm << "근성도 : " << (int)((pUnitData->m_iSpirit / (float)pUnitData->m_iSpiritMax) * 100 ) << "%";
//				pButtonSpiritGauge->SetGuideDesc_LUA( strstm.str().c_str() );
//			}
//		}
//	}
//
//}
//
//void CX2CharacterRoom::ResetRoomStateToInventory( UidType iItemUID )
//{
//	if ( GetCharRoomState() != CX2CharacterRoom::CRS_INVENTORY )
//		return;
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	CX2Item* pItem = pInventory->GetItem( iItemUID );
//
//	int invenPage = 1;
//	
//
//	for ( int i = 0; i < pInventory->GetItemMaxNum( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) ); i++ )
//	{
//		CX2Item* pItemTemp = pInventory->GetItem( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ), i );
//		if ( pItemTemp == NULL )
//		{
//			int showPage = i / MY_INVENTORY_SHOW_MAX_SLOT_NUM;
//			showPage++;
//
//			if ( showPage != 1 )
//				invenPage = showPage;
//
//
//			break;
//		}
//	}
//
//
//	if ( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) != m_NowInventorySortType 
//		|| invenPage != m_NowInvenSortTypePageNum )
//	{
//		if ( pItem != NULL && pItem->GetItemTemplet() != NULL )
//		{
//			wstring radioButtonName = GetRadioButtonNameByInvenSortType( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) );
//			CKTDGUIRadioButton* pRadioButtonInvenSortType = (CKTDGUIRadioButton*)m_pDLGMyInfoInventory->GetControl( radioButtonName.c_str() );
//			if ( pRadioButtonInvenSortType != NULL )
//			{
//				pRadioButtonInvenSortType->GetCheckedControl()->SetChecked( false );
//				pRadioButtonInvenSortType->SetChecked( true );
//
//				
//
//				SetInventorySort( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ), invenPage );
//			}
//		}
//	}
//}
//
//bool CX2CharacterRoom::UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo, bool bInventoryUIUpdate /*= true*/ )
//{
//	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
//	{
//		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
//		if ( kInventorySlotInfo.m_iSlotCategory == CX2Inventory::ST_E_EQUIP )
//		{
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem(
//				(CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_iSlotCategory, kInventorySlotInfo.m_iSlotID );
//			if ( pItem != NULL )
//			{
//				g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( pItem->GetUID() );
//				m_pUnitViewerUI->RemoveEqip( pItem->GetUID() );
//			}
//		}
//	}
//
//	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
//	{
//		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
//		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().RemoveItem( 
//			(CX2Inventory::SORT_TYPE) kInventorySlotInfo.m_iSlotCategory, kInventorySlotInfo.m_iSlotID );
//	}
//
//	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
//	{
//		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
//		if ( kInventorySlotInfo.m_iItemUID > 0 )
//		{
//          CX2Item::ItemData kItemData( kInventorySlotInfo );
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().AddItem( 
//				(CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_iSlotCategory, kInventorySlotInfo.m_iSlotID, kItemData );
//
//			if ( kInventorySlotInfo.m_iSlotCategory == CX2Inventory::ST_E_EQUIP )
//			{
//				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( 
//					(CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_iSlotCategory, kInventorySlotInfo.m_iSlotID );
//				if ( pItem != NULL )
//				{
//					if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( pItem->GetUID() ) == true )
//					{
//						m_pUnitViewerUI->AddEqip( pItem->GetUID() );
//
//					
//
//
//					}
//				}
//			}
//
//		}
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//
//	//	m_pUnitViewerUI->RemoveAllEquips();
//	//	m_pUnitViewerUI->UpdateEqip();
//
//	// 슬롯 업데이트
//	ResetNowEquipUI();
//
//	if( true == bInventoryUIUpdate )
//	{
//		// 인벤토리 업데이트
//		if ( SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum ) == false )
//			return false;
//	}
//
//	// 스탯 업데이트
//	ResetStat();
//
//
//	//// 스킬 장착 업데이트
//	//{
//	//	CX2SlotItem* pItemSlot = NULL;
//	//	
//	//	pItemSlot = (CX2SlotItem*)GetSlotByID( CX2Unit::NSSI_SKILL_1 );
//	//	pItemSlot->DestroyItemUI();
//	//	pItemSlot = (CX2SlotItem*)GetSlotByID( CX2Unit::NSSI_SKILL_2 );
//	//	pItemSlot->DestroyItemUI();
//	//	pItemSlot = (CX2SlotItem*)GetSlotByID( CX2Unit::NSSI_SKILL_3 );
//	//	pItemSlot->DestroyItemUI();
//	//}
//
//	return true;
//}
//
//void CX2CharacterRoom::UpdateSP()
//{
//
//	CKTDGUIStatic* pStatic_SP = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Info_SP" );
//	if( NULL == pStatic_SP )
//		return;
//
//	if( NULL == g_pData->GetMyUser() ||
//		NULL == g_pData->GetMyUser()->GetSelectUnit() )
//		return;
//
//
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//	WCHAR wszText[32] = L"";
//	wsprintf( wszText, L"%d", (int)pUnitData->m_iSPoint );
//	pStatic_SP->GetString(0)->msg = wszText;
//
//
//}
//
//void CX2CharacterRoom::UpdateExpGageBar()
//{
//	
//	CKTDGUIStatic* pStaticEXP = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticUser_Inventory_Info_EXP" );
//	if( NULL == pStaticEXP )
//		return; 
//
//	CKTDGUIControl::CPictureData* pPictureEXP = pStaticEXP->GetPicture( 0 );
//	D3DXVECTOR2 picOrgSize = pPictureEXP->GetOriginalSize();
//
//	int nowExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
//	int nowBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NowBaseLevelEXP;
//	int nextBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NextBaseLevelEXP;
//
//	pPictureEXP->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );
//
//	WCHAR buff[256] = {0};
//	if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_NORMAL_USER )
//	{
//		swprintf( buff, L"%d / %d (%.1f%%)", nowExp - nowBaseExp, nextBaseExp - nowBaseExp, (float)(nowExp - nowBaseExp)/(nextBaseExp - nowBaseExp)*100.0f );
//	}
//	else
//	{
//		swprintf( buff, L"%d/%d (%.1f%%) ( %d/%d )", nowExp - nowBaseExp, nextBaseExp - nowBaseExp, (float)(nowExp - nowBaseExp)/(nextBaseExp - nowBaseExp)*100.0f , nowExp, nextBaseExp );
//	}
//
//	pStaticEXP->GetString( 0 )->msg = buff;
//	
//}
//
//
//
//void CX2CharacterRoom::ResetSkillSlotUI()
//{
//	// 스킬 슬롯 update
//	WCHAR wszText[256] = L"";
//
//	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_Skill_Slot" );
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//	if( 0 == pUnitData->m_EqipSkill1 )
//	{
//		pStatic->GetPicture(0)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill1 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(0)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(0)->SetShow( true );
//			if( false == pStatic->GetPicture(0)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//	if( 0 == pUnitData->m_EqipSkill2 )
//	{
//		pStatic->GetPicture(1)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(  (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill2 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(1)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(1)->SetShow( true );
//			if( false == pStatic->GetPicture(1)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(1)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//
//	}
//
//
//	if( 0 == pUnitData->m_EqipSkill3 )
//	{
//		pStatic->GetPicture(2)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill3 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(2)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(2)->SetShow( true );
//			if( false == pStatic->GetPicture(2)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(2)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//
//
//
//
//
//	if( 0 == pUnitData->m_Eqip2Skill1 )
//	{
//		pStatic->GetPicture(3)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(  (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill1 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(3)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(3)->SetShow( true );
//			if( false == pStatic->GetPicture(3)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(3)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//
//
//
//
//
//	if( 0 == pUnitData->m_Eqip2Skill2 )
//	{
//		pStatic->GetPicture(4)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(  (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill2 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(4)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(4)->SetShow( true );
//			if( false == pStatic->GetPicture(4)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(4)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//
//
//
//	if( 0 == pUnitData->m_Eqip2Skill3 )
//	{
//		pStatic->GetPicture(5)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(  (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill3 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(5)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(5)->SetShow( true );
//			if( false == pStatic->GetPicture(5)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(5)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//
//	if( NULL != m_pSkillTreeUI )
//	{
//		m_pSkillTreeUI->ResetEquippedSkillSlotUI();
//	}
//}
//
//
//
//
//
//bool CX2CharacterRoom::XSkinMeshReady( const WCHAR* pFileName )
//{
//	CKTDXDeviceXSkinMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pFileName );
//	if( pDevice == NULL )
//		return false;
//
//	DeviceReady( pDevice );
//	return true;
//}
//
//bool CX2CharacterRoom::XMeshReady( const WCHAR* pFileName )
//{
//	CKTDXDeviceXMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXMesh( pFileName );
//	if( pDevice == NULL )
//		return false;
//
//	DeviceReady( pDevice );
//	return true;
//}
//
//bool CX2CharacterRoom::TextureReady( const WCHAR* pFileName )
//{
//	CKTDXDeviceTexture* pDevice = g_pKTDXApp->GetDeviceManager()->OpenTexture( pFileName );
//	if( pDevice == NULL )
//		return false;
//
//	DeviceReady( pDevice );
//	return true;
//}
//
//bool CX2CharacterRoom::SoundReady( const WCHAR* pFileName )
//{
//	CKTDXDeviceSound* pDevice = g_pKTDXApp->GetDeviceManager()->OpenSound( pFileName );
//	if( pDevice == NULL )
//		return false;
//
//	DeviceReady( pDevice );
//	return true;
//}
//
//
//
//wstring CX2CharacterRoom::GetRadioButtonNameByInvenSortType( CX2Inventory::SORT_TYPE sortType )
//{
//	wstring radioButtonName = L"";
//
//	//{{ 2009.01.19 김태완 : 코드정리
//	switch(sortType)
//	{
//	case CX2Inventory::ST_EQUIP:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Equip";
//		} break;
//	case CX2Inventory::ST_ACCESSORY:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Accessory";
//		} break;
//	case CX2Inventory::ST_QUICK_SLOT:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_UseItem";
//		} break;
//	case CX2Inventory::ST_MATERIAL:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Material";
//		} break;
//	case CX2Inventory::ST_QUEST:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Card";
//		} break;
//	case CX2Inventory::ST_SPECIAL:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Special";
//		} break;
//	case CX2Inventory::ST_AVARTA:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Avatar";
//		} break;
//	default:
//		break;
//	}
//	//}} elseif -> switch
//	
//	return radioButtonName;
//}
//
//void CX2CharacterRoom::UpdateOpenRandomItemAck()
//{
//	WCHAR buf[256] = {0};
//
//
//	wstring itemName = L"";
//	wstring acquireComment;
//
//	bool bCheck = false;
//
//	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
//	{
//		std::map< int, int >::iterator i;
//
//		int nCount = 0;
//		wstring getExplanation = L"";
//		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
//		{
//			int resultItemID = i->first;
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//			if ( pItemTemplet != NULL )
//				itemName = pItemTemplet->GetFullName();
//
//			if ( nCount > 0 )
//			{
//				getExplanation += L", ";
//			}
//
//			wsprintf( buf, L"%s", itemName.c_str() );
//
//			getExplanation += buf;
//
//			//{{ 2009.2.4 김태완 : 랜덤아이템 획득시 획득수 표시
//			int iNum = i->second;
//			if(iNum > 1)
//			{
//				wsprintf( buf, L" %d개", iNum );
//				getExplanation += buf;
//			}
//
//			//}}
//
//			nCount++;
//		}
//
//		wsprintf( buf, L"%s 얻었습니다!",
//			KHanSungPostWordUnicode( (WCHAR*)getExplanation.c_str() ) ? L"을" : L"를");
//
//		getExplanation += buf;
//
//		acquireComment = getExplanation;
//		bCheck = true;
//	}
//	
//	if ( m_TempOpenRandomItemAck.m_iRessurectionCount != 0 )
//	{
//		wstringstream tempString;
//		int getStoneNum = m_TempOpenRandomItemAck.m_iRessurectionCount - g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum(); 
//		tempString << L"부활석 " << getStoneNum << L"개를 얻었습니다!";
//
//		g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( m_TempOpenRandomItemAck.m_iRessurectionCount );
//		ResetResurrectionStoneUI();
//
//		acquireComment = tempString.str().c_str();
//		bCheck = true;
//	}
//
//	
//
//
//	if ( bCheck == true )
//	{
//		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.str().c_str(), m_pNowState );
//		SAFE_DELETE_DIALOG( m_pDLGCubeCheck );
//		m_pDLGCubeCheck = new CKTDGUIDialog( m_pNowState, L"DLG_Cube_Result_Window.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCubeCheck );
//		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCubeCheck->GetControl( L"StaticCube_Window_Text" );
//		if ( pStatic != NULL && pStatic->GetString(0) != NULL )
//		{
//			wstring tpString = g_pMain->GetStrByLienBreak( acquireComment.c_str(), 366, XUF_DODUM_15_BOLD );
//			pStatic->GetString(0)->msg = tpString;
//		}
//
//		if ( m_RandomItemID == 129755 )
//		{
//			CKTDGUIStatic* pStaticCube = (CKTDGUIStatic*)m_pDLGCubeCheck->GetControl( L"g_pStaticCube_Image" );
//			if ( pStaticCube != NULL )
//			{
//				if ( pStaticCube->GetPicture(0) != NULL )
//				{
//					pStaticCube->GetPicture(0)->SetTex( L"DLG_Common_New_Texture62.tga", L"CUBE_EVENT_MILITARY" );
//				}
//			}
//		}
//	}
//	else if ( m_TempOpenRandomItemAck.m_iRestoreSpirit != 0 )
//	{
//		wstringstream tempString;
//		int getSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit;
//		getSpirit = (int)(getSpirit / (float)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax * 100.0f);
//		tempString << L"근성도 " << getSpirit << L"%를 회복하였습니다.";
//
//		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_iSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit;
//		ResetSpirit();
//
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.str().c_str(), m_pNowState );
//	}
//	else
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"아무것도 얻지 못했습니다!!", m_pNowState );
//	}
//
//
//	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
//}
//
//void CX2CharacterRoom::OpenSellWindow( CX2SlotItem* pSlot )
//{
//	int popUpSizeX = 425;
//	int popUpSizeY = 147;
//
//	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"장착중인 아이템은 팔 수 없습니다.", m_pNowState );
//	}
//	else
//	{
//		m_TempItemUID = pSlot->GetItemUID();
//
//		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );
//		if ( pItem != NULL )
//		{
//			if ( pItem->GetItemTemplet() != NULL )
//			{
//				if ( pItem->GetItemTemplet()->m_PriceType == CX2Item::SPT_NONE )
//				{
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//						L"해당 아이템은 팔 수 없습니다!", m_pNowState );
//				}
//				else
//				{
//					if ( m_sbCheckQuestionSellNum == false )
//					{
//						if ( pItem->GetItemTemplet() != NULL && pItem->GetItemTemplet()->m_PeriodType == CX2Item::PT_QUANTITY )
//							m_SellItemNum = pItem->GetItemData().m_Quantity;
//						else
//							m_SellItemNum = 1;
//
//						WCHAR buff[256] = {0};
//						wsprintf( buff, L"판매가는 %d ED입니다.\n정말 파시겠습니까?", pItem->GetEDToSell() * m_SellItemNum );
//
//						m_pDialogToRemoveItem = g_pMain->KTDGUIOkAndCancelMsgBox( 
//							D3DXVECTOR2( -999, -999 ), 
//							buff, CRM_QUICK_SELL_ITEM , m_pNowState, CRM_QUICK_SELL_ITEM_CANCEL );
//
//						CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDialogToRemoveItem->GetControl( L"MsgBoxOkAndCancelOkButton" );
//						if ( pButton != NULL )
//						{
//							pButton->ChangeMouseUpSound( L"Item_Sell.ogg" );
//						}
//					}
//					else
//					{
//						if ( pItem->GetItemTemplet()->m_PeriodType == CX2Item::PT_QUANTITY )
//						{
//							OpenItemSellNumDLG( pItem->GetItemData().m_ItemUID );
//						}
//						else
//						{
//							WCHAR buff[256] = {0};
//							wsprintf( buff, L"판매가는 %d ED입니다.\n정말 파시겠습니까?", pItem->GetEDToSell() );
//							m_SellItemNum = 1;
//							m_pDialogToRemoveItem = g_pMain->KTDGUIOkAndCancelMsgBox( 
//								D3DXVECTOR2( -999, -999 ), 
//								buff, CRM_QUICK_SELL_ITEM , m_pNowState, CRM_QUICK_SELL_ITEM_CANCEL );
//
//							CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDialogToRemoveItem->GetControl( L"MsgBoxOkAndCancelOkButton" );
//							if ( pButton != NULL )
//							{
//								pButton->ChangeMouseUpSound( L"Item_Sell.ogg" );
//							}
//						}
//					}
//				}
//			}
//		}
//
//	}	
//}
//
//void CX2CharacterRoom::OpenItemSellNumDLG( UidType sellItemUID )
//{
//	SAFE_DELETE_DIALOG( m_pDLGSelectSellNumWindowPopup );
//
//	m_SellItemUID	= sellItemUID;
//	m_SellItemNum	= 1;
//
//	m_pDLGSelectSellNumWindowPopup = new CKTDGUIDialog( m_pNowState, L"DLG_Item_Sell_Num_Window.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSelectSellNumWindowPopup );
//	m_pDLGSelectSellNumWindowPopup->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 366, 212 ), D3DXVECTOR2( 211, 125 ) ) );
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SellItemUID );
//	if ( pItem != NULL )
//	{
//		WCHAR buff[256] = {0};
//		CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGSelectSellNumWindowPopup->GetControl( L"Item_Name" );
//		if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
//		{
//			pStaticItemName->GetString(0)->msg = pItem->GetItemTemplet()->GetFullName();
//		}
//
//		CKTDGUIStatic* pStaticItemSellED = (CKTDGUIStatic*)m_pDLGSelectSellNumWindowPopup->GetControl( L"StaticItem_Sell_ED" );
//		if ( pStaticItemSellED != NULL && pStaticItemSellED->GetString(0) != NULL )
//		{
//			wsprintf( buff, L"%d", pItem->GetEDToSell() );
//			pStaticItemSellED->GetString(0)->msg = buff;
//		}
//	}
//}
//
//void CX2CharacterRoom::OpenRepairWindow( CX2SlotItem* pSlot )
//{
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ((CX2SlotItem*)pSlot)->GetSortType(), 
//		((CX2SlotItem*)pSlot)->GetSlotID() );
//
//	if ( pItem != NULL )
//	{
//		if ( pItem->GetItemTemplet()->m_PeriodType == CX2Item::PT_ENDURANCE )
//		{
//			if ( pItem->GetItemData().m_Endurance >= pItem->GetItemTemplet()->m_Endurance )
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//					L"내구도가 이미 꽉 차있습니다!", m_pNowState );
//			}
//			else
//			{
//				m_TempSortType = ((CX2SlotItem*)pSlot)->GetSortType();
//				m_TempSlotID = ((CX2SlotItem*)pSlot)->GetSlotID();
//
//				WCHAR buff[256] = {0};
//				int repairEDCost = pItem->GetEDToRepair();
//				int repairVPCost = pItem->GetVPToRepair(); 
//
//
//				float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
//				if( fDiscountRate > 0.f )
//				{
//					repairEDCost = (int) ( repairEDCost * ( 1.f - fDiscountRate ) );
//				}
//
//
//#ifdef ITEM_RECOVERY_TEST
//				if ( repairVPCost == 0 )
//				{
//					if(true == pItem->IsDisabled())
//						wsprintf( buff, L"수리비용은 %d ED입니다. \n내구도는 회복되지만 사용 불가 상태는 회복되지 \n않습니다. 정말 수리하시겠습니까?", repairEDCost  );
//					else
//						wsprintf( buff, L"수리비용은 %d ED입니다. \n정말 수리하시겠습니까?", repairEDCost  );
//				}
//				else
//				{
//					if(true == pItem->IsDisabled())
//						wsprintf( buff, L"수리비용은 %d ED와 #CFF0000%d VP#CX입니다.\n내구도는 회복되지만 사용 불가 상태는 회복되지 \n않습니다. 정말 수리하시겠습니까?", repairEDCost, repairVPCost  );
//					else
//					wsprintf( buff, L"수리비용은 %d ED와 #CFF0000%d VP#CX입니다. \n정말 수리하시겠습니까?", repairEDCost, repairVPCost  );
//				}
//#else
//				if ( repairVPCost == 0 )
//					wsprintf( buff, L"수리비용은 %d ED입니다. \n정말 수리하시겠습니까?", repairEDCost  );
//				else
//					wsprintf( buff, L"수리비용은 %d ED와 #CFF0000%d VP#CX입니다. \n정말 수리하시겠습니까?", repairEDCost, repairVPCost  );
//#endif
//				m_pDLGToRepair = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
//					buff, CRM_REPAIR_ITEM , m_pNowState );
//			}
//		}
//		else
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"수리가능한 아이템이 아닙니다!", m_pNowState );
//		}
//	}
//}
//
//void CX2CharacterRoom::OpenDecompositionWindow( CX2SlotItem* pSlot )
//{
//	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"장착중인 아이템은 분해할 수 없습니다.", m_pNowState );
//
//		return;
//	}
//
//	if ( pSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
//	{
//		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );
//#ifdef ITEM_RECOVERY_TEST
//		if ( pItem == NULL )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"아이템을 찾을 수 없습니다!", m_pNowState );
//
//			return;
//		}
//		if(true == pItem->IsDisabled())
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"사용 불가 상태의 아이템입니다.\n[축복 받은 복원의 주문서] 아이템을 사용해서\n사용 불가 상태를 원상태로 복원할 수 있습니다.", m_pNowState );
//			return; 
//		}
//#endif // ITEM_RECOVERY_TEST
//
//#ifdef ENABLE_RESOLVE_FASHION
//
//		if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"아이템을 찾을 수 없습니다!", m_pNowState );
//
//			return;
//		}
//
//		if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON || 
//			pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_DEFENCE )
//		{
//			if ( pItem->GetItemTemplet()->m_bFashion == true )
//			{
//				if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY &&
//					pItem->GetItemData().m_Period > 0 )
//				{
//					//기간제로 판명.
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//						L"기간제 아이템은 분해할 수 없습니다!", m_pNowState );
//
//					return;
//				}
//			}
//		}
//		else
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"장비 아이템만 분해할 수 있습니다.", m_pNowState );
//
//			return;
//		}
//
//
//#else //ENABLE_RESOLVE_FASHION
//
//		if ( m_NowInventorySortType != CX2Inventory::ST_EQUIP )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"장비 아이템만 분해할 수 있습니다.", m_pNowState );
//
//			return;
//		}
//
//#endif //ENABLE_RESOLVE_FASHION
//
//		
//
//		
//
//		if ( pItem != NULL )
//		{
//			m_DecompositionItemUID = pItem->GetUID();
//			if ( m_pDLGResolveItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGResolveItem, NULL, false );
//			}
//			//m_pDLGResolveItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), L"정말 분해하시겠습니까?", CX2CharacterRoom::CRM_RESOLVE_ITEM, m_pNowState );
//			m_pDLGResolveItem = new CKTDGUIDialog( m_pNowState, L"DLG_Resolve_Item.lua" );
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItem );
//			m_pDLGResolveItem->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 400, 244 ), D3DXVECTOR2( 197, 157 ) ) );
//
//			CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItem->GetControl( L"Static_broken_item_Slot" );
//			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
//			{
//				if ( pItem->GetItemTemplet()->m_ShopImage.empty() == false )
//					pStaticSlot->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//				else
//					pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//
//			CKTDGUIControl* pControlButton = m_pDLGResolveItem->GetControl( L"ok_Button" );
//			if ( pControlButton != NULL )
//				pControlButton->RequestFocus();
//		}
//	}	
//}
//
//void CX2CharacterRoom::ChangeDecompositionPage( bool bNextPage )
//{
//	//거시기 창이 띄워져 있는 경우에만..
//	if ( m_pDLGResolveItemResult != NULL )
//	{
//		if ( bNextPage == false )
//		{
//			if ( m_DecompositionWindowNowPageNum <= 1 )
//				return;
//
//			m_DecompositionWindowNowPageNum--;
//		}
//
//		
//
//		int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
//		if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
//		{
//			maxPage += 1;
//		}
//
//		if ( bNextPage == true )
//		{
//			if ( m_DecompositionWindowNowPageNum >= maxPage )
//				return;
//
//			m_DecompositionWindowNowPageNum++;
//		}
//
//		wstringstream wstrstm;
//		wstrstm << m_DecompositionWindowNowPageNum << L"/" << maxPage;
//		CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"StaticResolve_List_Page_Num");
//		if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
//		{
//			pStaticPageNum->GetString(0)->msg = wstrstm.str().c_str();
//		}
//
//		CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_broken_result_Slot" );
//		CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_comment1" );
//		CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_num1" );
//
//		std::map< int, int >::iterator i;
//		int j = 0;
//		i = m_mapResolveResultInsertedItem.begin();
//		for ( int nextNum = 0; nextNum < (m_DecompositionWindowNowPageNum - 1) * RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; nextNum++ )
//		{
//			i++;
//		}
//
//		std::map< int, int >::iterator maxiter = m_mapResolveResultInsertedItem.begin();
//		for ( int nextNum = 0; nextNum < ( RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM*m_DecompositionWindowNowPageNum ); nextNum++ )
//		{
//			maxiter++;
//		}
//
//		//일단 초기화 부터 시켜줘야할꺼 같다 후덜덜
//		for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; slotNum++ )
//		{
//			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
//			{
//				pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
//			}
//
//			if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
//			{
//				pStaticName->GetString(slotNum)->msg = L"";
//			}
//
//			if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
//			{
//				pStaticNum->GetString(slotNum)->msg = L"";
//			}
//		}
//		
//		for ( ; i != m_mapResolveResultInsertedItem.end() && i != maxiter ; i++, j++ )
//		{
//			int resultItemID = i->first;
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//			if ( pItemTemplet != NULL )
//			{
//				if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
//				{
//					if ( pItemTemplet->m_ShopImage.empty() == false )
//						pStaticSlot->GetPicture(j)->SetTex( pItemTemplet->m_ShopImage.c_str() );
//					else
//						pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//				}
//
//				if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
//				{
//					pStaticName->GetString(j)->msg = pItemTemplet->GetFullName();
//				}
//
//				if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"x " << i->second;
//					pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
//				}
//			}
//		}
//	}
//}
//
//
//
//// @bIdentifyResult: 분해결과창이 아닌 속성석 감정 결과창인지, 다이얼로그를 공통으로 써서 일단은 요렇게 구분
//void CX2CharacterRoom::OpenDecompositionResultWindow( std::map< int, int >& mapInsertedItem, 
//													 std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
//													 bool bJackpot, bool bIdentifyResult /*= false*/ )
//{
//	SAFE_DELETE_DIALOG( m_pDLGResolveItemResult );
//
//	m_DecompositionWindowNowPageNum = 1;
//	m_mapResolveResultInsertedItem = mapInsertedItem;
//
//	m_pDLGResolveItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Resolve_Item_Result.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItemResult );
//
//
//	CKTDGUIStatic* pStatic_ResolveTitle		= (CKTDGUIStatic*) m_pDLGResolveItemResult->GetControl( L"g_pStatic_WindowTitle_Resolve99" );
//	CKTDGUIStatic* pStatic_IdentifyTitle	= (CKTDGUIStatic*) m_pDLGResolveItemResult->GetControl( L"g_pStatic_WindowTitle_Identify99" );
//	if( true == bIdentifyResult )		
//	{
//		if( NULL != pStatic_ResolveTitle )
//			pStatic_ResolveTitle->SetShowEnable( false, false );
//
//		if( NULL != pStatic_IdentifyTitle )
//			pStatic_IdentifyTitle->SetShowEnable( true, true );
//
//	}
//	else
//	{
//		if( NULL != pStatic_ResolveTitle )
//			pStatic_ResolveTitle->SetShowEnable( true, true );
//
//		if( NULL != pStatic_IdentifyTitle )
//			pStatic_IdentifyTitle->SetShowEnable( false, false );
//	}
//
//
//	if ( bJackpot == true && m_pDLGResolveItemResult->GetControl( L"StaticBig_Item_Acquirement" ) != NULL )
//		m_pDLGResolveItemResult->GetControl( L"StaticBig_Item_Acquirement" )->SetShow( true );
//
//	m_pDLGResolveItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 403, 350 ), D3DXVECTOR2( 270, 332 ) ) );
//
//	CKTDGUIControl* pControl = m_pDLGResolveItemResult->GetControl( L"ok_Button_Resolve_Item" );
//	if ( pControl != NULL )
//	{
//		pControl->RequestFocus();
//	}
//
//	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_broken_result_Slot" );
//	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_comment1" );
//	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_num1" );
//
//	std::map< int, int >::iterator i;
//	int j = 0;
//	for ( i = mapInsertedItem.begin(); i != mapInsertedItem.end(); i++, j++ )
//	{
//		int resultItemID = i->first;
//
//		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//		if ( pItemTemplet != NULL )
//		{
//			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
//			{
//				if ( pItemTemplet->m_ShopImage.empty() == false )
//					pStaticSlot->GetPicture(j)->SetTex( pItemTemplet->m_ShopImage.c_str() );
//				else
//					pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//
//			if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
//			{
//				pStaticName->GetString(j)->msg = pItemTemplet->GetFullName();
//			}
//
//			if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"x " << i->second;
//				pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
//			}
//		}
//	}
//
//	int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
//	if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
//	{
//		maxPage += 1;
//	}
//
//	wstringstream wstrstm;
//	wstrstm << m_DecompositionWindowNowPageNum << L"/" << maxPage;
//	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"StaticResolve_List_Page_Num");
//	if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
//	{
//		pStaticPageNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//
//	UpdateInventorySlotList( vecKInventorySlotInfo );
//}
//
//
//// 강화할 아이템 레벨에 따라 필요한 새로운 강화석의 레벨을 계산하는 함수
//int CX2CharacterRoom::GetEnchantStoneLevel( int iItemLevel )
//{
//
//	if( iItemLevel <= 20 )
//	{
//		return 0;
//	}
//	else if( iItemLevel <= 30 )
//	{
//		return 1;
//	}
//	else if( iItemLevel <= 40 )
//	{
//		return 2;
//	}
//	else if( iItemLevel <= 50 )
//	{
//		return 3;
//	}
//	else if( iItemLevel <= 60 )
//	{
//		return 4;
//	}
//	else if( iItemLevel <= 70 )
//	{
//		return 5;
//	}
//	else if( iItemLevel <= 80 )
//	{
//		return 6;
//	}
//	else if( iItemLevel <= 90 )
//	{
//		return 7;
//	}
//	else
//	{
//		return 8;
//	}
//}
//
//
//
//// 강화 UI 갱신, 강화보조제 사용여부 체크박스가 체크될 때 마다 호출된다.
//void CX2CharacterRoom::UpdateEnchantWindow()
//{
//	if( NULL == m_pDLGEnchantItem )
//		return;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnchantItemUID );
//	if( NULL == pItem )
//		return;
//
//
//	int normalStoneCount	= 0;
//	int newStoneCount		= 0;
//	int iNewStoneLevel		= GetEnchantStoneLevel( pItem->GetItemTemplet()->m_UseLevel );
//
//
//	int iAdjuvantItemID		= GetRequiredEnchantAdjuvantItemID( pItem->GetItemTemplet()->m_UseLevel );
//	int iAdjuvantCount		= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iAdjuvantItemID );
//
//	wstring leftButtonDesc	= L"";
//	wstring rightButtonDesc		= L"";
//	
//
//
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
//			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
//			if ( pItemTemplet != NULL )
//			{
//				leftButtonDesc = pItemTemplet->GetFullName();
//			}
//
//			pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//			if ( pItemTemplet != NULL )
//			{
//				rightButtonDesc = pItemTemplet->GetFullName();
//			}
//
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
//			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
//			if ( pItemTemplet != NULL )
//			{
//				leftButtonDesc = pItemTemplet->GetFullName();
//			}
//
//			pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//			if ( pItemTemplet != NULL )
//			{
//				rightButtonDesc = pItemTemplet->GetFullName();
//			}
//		} break;
//
//	default:
//		{
//			return;
//		} break;
//	}
//
//	leftButtonDesc += L" 1개";
//	rightButtonDesc += L" 1개";
//
//	if( true == m_bUseEnchantAdjuvant )
//	{
//		leftButtonDesc += L"와 플루오르 스톤 1개가 소모됩니다.";
//		rightButtonDesc += L"와 플루오르 스톤 1개가 소모됩니다.";
//	}
//	else
//	{
//		leftButtonDesc += L"가 소모됩니다.";
//		rightButtonDesc += L"가 소모됩니다.";
//	}
//
//
//
//	WCHAR leftButtonName[128] = L"";
//	WCHAR rightButtonName[128] = L"";
//	CKTDGUIControl* pButton = NULL;
//
//
//
//
//	// 레벨 없는 기존 강화석 버튼 (축복받은 강화석)
//	pButton = m_pDLGEnchantItem->GetControl( L"g_pButtonitem_normal_Button" );
//	if( NULL != pButton )
//	{
//		pButton->SetShowEnable( false, false );
//		pButton->SetGuideDesc( leftButtonDesc.c_str() );
//	}
//
//
//	// 플루오르 스톤 사용했을 때 축복받은 강화석 버튼
//	const int MAGIC_ENCHANT_BUTTON_COUNT = 4;
//	for( int i=0; i<MAGIC_ENCHANT_BUTTON_COUNT; i++ )
//	{
//		StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pitem_Bless_cash_upgrade_a%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( leftButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//			pButton->SetGuideDesc( leftButtonDesc.c_str() );
//
//		}
//
//		StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pitem_Bless_cash_upgrade_b%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( leftButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//			pButton->SetGuideDesc( leftButtonDesc.c_str() );
//		}
//	}
//	
//
//
//
//	CKTDGUIStatic* pStaticItemNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Item_Num1" );
//	if( pStaticItemNum != NULL && pStaticItemNum->GetString( 0 ) != NULL )
//	{
//		wstringstream wstrstm;
//		if( true == m_bUseEnchantAdjuvant ) 
//		{
//			wstrstm << L"가능 횟수:" << min( normalStoneCount, iAdjuvantCount );
//		}
//		else
//		{
//			wstrstm << L"가능 횟수:" << normalStoneCount;
//		}
//		pStaticItemNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//
//
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			if( true == m_bUseEnchantAdjuvant ) 
//			{
//				StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pitem_Bless_cash_upgrade_a%d", iNewStoneLevel+1 );
//			}
//			else
//			{
//				StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pButtonitem_normal_Button" );
//			}
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			if( true == m_bUseEnchantAdjuvant ) 
//			{
//				StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pitem_Bless_cash_upgrade_b%d", iNewStoneLevel+1 );
//			}
//			else
//			{
//				StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pButtonitem_normal_Button" );
//			}
//		} break;
//	}
//
//
//
//	pButton = m_pDLGEnchantItem->GetControl( leftButtonName );
//	if( NULL != pButton )
//	{
//		if( normalStoneCount >= 1 )
//		{
//			pButton->SetShowEnable( true, true );
//		}
//		else
//		{
//			pButton->SetShowEnable( true, false );
//		}
//		pButton->SetGuideDesc( leftButtonDesc.c_str() );
//	}
//
//
//
//
//
//
//
//
//
//
//
//	// 레벨붙은 새 강화석 버튼
//	const int MAGIC_NEW_ENCHANT_BUTTON_COUNT = 4;
//	for( int i=0; i<MAGIC_NEW_ENCHANT_BUTTON_COUNT; i++ )
//	{
//		StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_advanced%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//		}
//
//		StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_normal%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//		}
//
//
//		StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_def_advanced%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//		}
//
//		StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_def_normal%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//		}
//	}
//
//
//	pStaticItemNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Item_Num2" );
//	if ( pStaticItemNum != NULL && pStaticItemNum->GetString( 0 ) != NULL )
//	{
//		wstringstream wstrstm;
//		if( true == m_bUseEnchantAdjuvant ) 
//		{
//			wstrstm << L"가능 횟수:" << min( newStoneCount, iAdjuvantCount );
//		}
//		else
//		{
//			wstrstm << L"가능 횟수:" << newStoneCount;
//		}
//		pStaticItemNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			if( true == m_bUseEnchantAdjuvant ) 
//			{
//				StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_advanced%d", iNewStoneLevel+1 );
//			}
//			else
//			{
//				StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_normal%d", iNewStoneLevel+1 );
//			}
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			if( true == m_bUseEnchantAdjuvant ) 
//			{
//				StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_def_advanced%d", iNewStoneLevel+1 );
//			}
//			else
//			{
//				StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_def_normal%d", iNewStoneLevel+1 );
//			}
//		} break;
//	}
//
//
//
//	pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//	if( NULL != pButton )
//	{
//		if( newStoneCount >= 1 )
//		{
//			pButton->SetShowEnable( true, true );
//		}
//		else
//		{
//			pButton->SetShowEnable( true, false );
//		}
//		pButton->SetGuideDesc( rightButtonDesc.c_str() );
//	}
//
//
//}
//
//
//
//
//// 강화 UI 팝업
//void CX2CharacterRoom::OpenEnchantWindow( CX2SlotItem* pSlot )
//{
//	
//
//
//	if( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"장착중인 아이템은 강화할 수 없습니다.", m_pNowState );
//		return;
//	}
//
//	if( m_NowInventorySortType != CX2Inventory::ST_EQUIP )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"장비 아이템만 강화할 수 있습니다.", m_pNowState );
//		return;
//	}
//
//	if( pSlot->GetSlotType() != CX2Slot::ST_INVENTORY )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"인벤토리에 있는 아이템만 강화할 수 있습니다.", m_pNowState );
//		return;
//	}
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ((CX2SlotItem*)pSlot)->GetSortType(), 
//		((CX2SlotItem*)pSlot)->GetSlotID() );
//	if( NULL == pItem )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"아이템 정보가 올바르지 않습니다.", m_pNowState );
//		return; 
//	}
//
//
//#ifdef ITEM_RECOVERY_TEST
//	// 강화 레벨 검사 (사용불능인가?)
//	if(true == pItem->IsDisabled())
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"사용 불가 상태의 아이템입니다.\n[축복 받은 복원의 주문서] 아이템을 사용해서\n사용 불가 상태를 원상태로 복원할 수 있습니다.", m_pNowState );
//		return; 
//	}
//
//#endif
//
//
//
//
//	m_bUseEnchantAdjuvant = false;
//
//
//	int normalStoneCount	= 0;
//	int newStoneCount		= 0;
//	int iNewStoneLevel		= GetEnchantStoneLevel( pItem->GetItemTemplet()->m_UseLevel );
//
//
//	int iAdjuvantItemID		= GetRequiredEnchantAdjuvantItemID( pItem->GetItemTemplet()->m_UseLevel );
//	int iAdjuvantCount		= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iAdjuvantItemID );
//
//	wstring normalButtonDesc	= L"";
//	wstring newButtonDesc		= L"";
//	WCHAR wszEnchantED[256]		= L"";
//
//
//
//
//	// 비용 검사
//	int iCost = 0;
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			iCost = (int)( ( (float)pItem->GetItemTemplet()->m_Price * 0.1f ) * 0.5f );
//		}
//		break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			iCost = (int)( ( (float)pItem->GetItemTemplet()->m_Price * 0.1f ) * 0.1f );
//		}
//		break;
//	}
//
//	StringCchPrintfW( wszEnchantED, sizeof(wszEnchantED), L"%d", iCost );
//
//
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
//			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//
//			if ( normalStoneCount <= 0 && newStoneCount <= 0 )
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//					L"선택한 아이템을 강화하려면 무기 강화석이 필요합니다!", m_pNowState );
//
//				return;
//			}
//
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
//			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//
//			if ( normalStoneCount <= 0 && newStoneCount <= 0 )
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//					L"선택한 아이템을 강화하려면 방어구 강화석이 필요합니다!", m_pNowState );
//
//				return;
//			}
//
//
//		} break;
//
//	default:
//		{
//			return;
//		} break;
//	}
//	
//	m_EnchantItemUID = pItem->GetUID();
//	if ( m_pDLGEnchantItem != NULL )
//	{
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItem, NULL, false );
//	}
//
//	m_pDLGEnchantItem = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEnchantItem );
//	m_pDLGEnchantItem->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 402, 325 ), D3DXVECTOR2( 176, 244 ) ) );
//
//
//	// 강화 비용
//	CKTDGUIStatic* pStaticEnchantED = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_comment6" );
//	if ( pStaticEnchantED != NULL && pStaticEnchantED->GetString(0) != NULL )
//	{
//		pStaticEnchantED->GetString(0)->msg = g_pMain->GetEDString( wszEnchantED );  
//	}
//
//
//	// 강화 대상 아이템 이름
//	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_comment4" );
//	if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pItem->GetItemTemplet()->GetFullName().c_str();
//		pStaticItemName->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//	// 강화 대상 아이템 그림
//	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"g_pStatic_item_upgrade_Slot" );
//	if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
//	{
//		if ( pItem->GetItemTemplet()->m_ShopImage.empty() == false )
//			pStaticSlot->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//		else
//			pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//	}
//
//	// 강화 대상 아이템 강화 레벨 표시
//	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"g_pStatic_item_upgrade_Num" );
//	if ( pStaticNum != NULL )
//	{
//		for ( int i = 0; i <= MAX_ENCHANT_LEVEL; i++ )
//		{
//			if ( pStaticNum->GetPicture( i ) != NULL )
//			{
//				pStaticNum->GetPicture( i )->SetShow( false );
//			}
//		}
//
//		if ( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
//		{
//			pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );
//		}
//	}
//
//
//	// 해당하는 강화 버튼을 활성화 해준다
//	UpdateEnchantWindow();
//
//}
//
//
//void CX2CharacterRoom::OpenEnchantResultWindow( KEGS_ENCHANT_ITEM_ACK& kEGS_ENCHANT_ITEM_ACK )
//{
//	//업데이트 해주기전에..
//	//아이템 체크해서.. 업그레이드 됐는지, 초기화 됐는지, 변화없는지, 다운됐는지, 사라졌는지..
//
//	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEGS_ENCHANT_ITEM_ACK.m_iED;
//
//	ResetCharRoomEDnCashUInVP();
//
//	UpdateInventorySlotList( kEGS_ENCHANT_ITEM_ACK.m_vecInventorySlotInfo );
//
//	SAFE_DELETE_DIALOG( m_pDLGEnchantItemResult );
//
//	//{{ 2009.01.19 김태완 : 코드정리
//	switch( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult )
//	{
//	//강화 성공. 
//	case NetError::ERR_ENCHANT_RESULT_00:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Success.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Success.lua" );
//
//			//강화 성공한 경우 강화 단계 보여주고..
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
//			if ( pItem != NULL )
//			{
//				CKTDGUIStatic* pStaticComment = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_comment1" );
//				if ( pStaticComment != NULL && pStaticComment->GetString(0) != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"축하드립니다! 아이템 강화에 성공하여\n      강화레벨 " << pItem->GetItemData().m_EnchantLevel << L"이 되었습니다!";
//					pStaticComment->GetString(0)->msg = wstrstm.str().c_str();
//				}
//
//				CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"g_pStatic_item_upgrade_Num_Success" );
//				if ( pStaticNum != NULL )
//				{
//					for ( int i = 0; i < MAX_ENCHANT_LEVEL+1; i++ )
//					{
//						if ( pStaticNum->GetPicture( i ) != NULL )
//						{
//							pStaticNum->GetPicture( i )->SetShow( false );
//						}
//					}
//
//					if ( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
//					{
//						pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );
//					}
//				}
//			}
//
//
//		} break;
//	//변화 없음.
//	case NetError::ERR_ENCHANT_RESULT_01:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_NoChange.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Fail_NoChange.lua" );
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
//			if ( pItem != NULL )
//			{
//				CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"g_pStatic_item_upgrade_Num_NoChange" );
//				if ( pStaticNum != NULL )
//				{
//					for ( int i = 0; i < MAX_ENCHANT_LEVEL+1; i++ )
//					{
//						if ( pStaticNum->GetPicture( i ) != NULL )
//						{
//							pStaticNum->GetPicture( i )->SetShow( false );
//						}
//					}
//
//					if ( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
//					{
//						pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );
//					}
//				}
//			}
//			
//		} break;
//	//1단계 하락.
//	case NetError::ERR_ENCHANT_RESULT_02:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_DownGrade.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Fail_Down.lua" );
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
//			if ( pItem != NULL )
//			{
//				CKTDGUIStatic* pStaticComment = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_comment1" );
//				if ( pStaticComment != NULL && pStaticComment->GetString(0) != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"아깝네요! \n강화에 실패하여 강화 레벨이 ";
//					wstrstm << pItem->GetItemData().m_EnchantLevel << L"이 되었습니다\n   하지만 더 큰 피해가 없어서 다행이에요.";
//
//					pStaticComment->GetString(0)->msg = wstrstm.str().c_str();
//				}
//
//				CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"g_pStatic_item_upgrade_Num_Down" );
//				if ( pStaticNum != NULL )
//				{
//					for ( int i = 0; i < MAX_ENCHANT_LEVEL+1; i++ )
//					{
//						if ( pStaticNum->GetPicture( i ) != NULL )
//						{
//							pStaticNum->GetPicture( i )->SetShow( false );
//						}
//					}
//
//					if ( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
//					{
//						pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );
//					}
//				}
//			}
//		} break;
//	//강화 단계 초기화.
//	case NetError::ERR_ENCHANT_RESULT_03:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Initial.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Fail_Initial.lua" );
//		} break;
//	//아이템 분해.
//	case NetError::ERR_ENCHANT_RESULT_04:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Disappear.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Fail_Broken.lua" );
//		} break;
//	default:
//		{
//			// 뭔가 잘못됐다
//			ASSERT( !"Unexpected EnchantResult Neterror" );
//		} break;
//	}//}} elseif -> switch
//
//	if ( m_pDLGEnchantItemResult != NULL )
//	{
//		CKTDGUIControl* pControl = (CKTDGUIControl*)m_pDLGEnchantItemResult->GetControl( L"Enchant_OK_Button" );
//		if ( pControl != NULL )
//		{
//			pControl->RequestFocus();
//		}
//
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEnchantItemResult );
//
//		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemID );
//		if ( pItemTemplet != NULL )
//		{
//			//슬롯 이미지 표시 해주기.
//			CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"ItemSlot" );
//			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
//			{
//
//				if ( pItemTemplet->m_ShopImage.empty() == false )
//					pStaticSlot->GetPicture(0)->SetTex( pItemTemplet->m_ShopImage.c_str() );
//				else
//					pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//
//			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"ItemName" );
//			if ( pStaticName != NULL && pStaticName->GetString(0) != NULL )
//			{
//				if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_04 )
//				{
//					pStaticName->GetString(0)->msg = pItemTemplet->GetFullName();
//				}
//				else
//				{
//					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
//					if ( pItem != NULL )
//					{
//						wstringstream wstrstm;
//						wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pItemTemplet->GetFullName().c_str();
//						pStaticName->GetString(0)->msg = wstrstm.str().c_str();
//					}
//					else
//					{
//						pStaticName->GetString(0)->msg = pItemTemplet->GetFullName();
//					}	
//				}
//			}
//
//		}
//
//	}
//}
//
//void CX2CharacterRoom::OpenSocketWindow( CX2SlotItem* pSlot )
//{
//	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"장착중인 아이템은 소켓 추가할 수 없습니다.", m_pNowState );
//
//		return;
//	}
//
//
//	if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( pSlot->GetItemTID() ) == false )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"장비 아이템만 소켓 추가할 수 있습니다.", m_pNowState );
//
//		return;
//	}
//
//	int normalStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_MAGIC_STONE_ITEM_ID );
//	int specialStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( SPECIAL_MAGIC_STONE_ITEM_ID );
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );
//
//#ifdef ITEM_RECOVERY_TEST
//	// 강화 레벨 검사 (사용불능인가?)
//	if(true == pItem->IsDisabled())
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"사용 불가 상태의 아이템입니다.\n[축복 받은 복원의 주문서] 아이템을 사용해서\n사용 불가 상태를 원상태로 복원할 수 있습니다.", m_pNowState );
//		return; 
//	}
//
//#endif
//
//	bool bCheckHaveOption = false;
//	if ( pItem != NULL )
//	{
//		for ( int i = 0; i < (int)pItem->GetItemData().m_SocketOption.size(); i++ )
//		{
//			int socketOption = pItem->GetItemData().m_SocketOption[i];
//			if ( socketOption != 0 )
//				bCheckHaveOption = true;
//		}
//	}
//	
//	if ( normalStone <= 0 && specialStone <= 0 && bCheckHaveOption == false )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"해당 아이템을 세공하려면 마법석이 필요합니다!", m_pNowState );
//		return;
//	}
//
//	if ( pItem != NULL && pItem->GetItemTemplet() != NULL )
//	{
//		int slotNum = pItem->GetItemTemplet()->GetSocketSlotNum();
//		if ( slotNum == 0 )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"마법석을 장착할 소켓 공간이 없습니다!", m_pNowState );
//			
//			return;
//		}
//
//		m_SocketItemUID = pItem->GetUID();
//
//		m_pDLGSocketItem = new CKTDGUIDialog( m_pNowState, L"DLG_Socket_Item.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSocketItem );
//
//		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
//		if ( pCheckBox != NULL )
//		{
//			if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
//			{
//				pCheckBox->SetShow( true );
//				pCheckBox->SetEnable( true );
//			}
//			else
//			{
//				pCheckBox->SetShow( false );
//				pCheckBox->SetEnable( false );
//			}
//		}
//
//		
//
//		ResetSocketWindow();
//	}
//}
//
//void CX2CharacterRoom::ResetSocketWindow()
//{
//	if ( m_pDLGSocketItem == NULL )
//		return;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID ); 
//
//	if ( pItem == NULL )
//		return;
//
//	//아이템 이름하고 아이템 슬롯 이미지 표시
//	CKTDGUIStatic* pStaticSlotImage = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_item_Slot_Image" );
//	if ( pStaticSlotImage != NULL && pStaticSlotImage->GetPicture(0) != NULL )
//	{
//		pStaticSlotImage->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//	}
//
//	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"Static_comment4" );
//	if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
//	{
//		pStaticItemName->GetString(0)->msg = pItem->GetFullName();
//	}
//	
//
//	int slotNum = pItem->GetItemTemplet()->GetSocketSlotNum();
//
//	int normalStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_MAGIC_STONE_ITEM_ID );
//	int specialStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( SPECIAL_MAGIC_STONE_ITEM_ID );
//
//	
//	CKTDGUIStatic* pStaticMagicStoneNum = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_Normal_Magic_Stone_Num" );
//	if ( pStaticMagicStoneNum != NULL && pStaticMagicStoneNum->GetString(0) != NULL )
//	{
//		wstringstream wstrstm;
//		wstrstm << normalStone;
//		pStaticMagicStoneNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//	CKTDGUIStatic* pStaticRareStoneNum = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_Rare_Magic_Stone_Num" );
//	if ( pStaticRareStoneNum != NULL && pStaticRareStoneNum->GetString(0) != NULL )
//	{
//		wstringstream wstrstm;
//		wstrstm << specialStone;
//		pStaticRareStoneNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Lower_Back_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			if ( slotNum == i + 1 )
//			{
//				pControl->SetShow( true );
//			}
//			else
//				pControl->SetShow( false );
//		}
//			
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Gray_Back_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			if ( slotNum >= i + 1 )
//			{
//				pControl->SetShow( true );
//			}
//			else
//				pControl->SetShow( false );
//		}
//	}
//
//	//아이템 표시 해주기 전에 일단 전부다 초기화 해주고.. 밑에 필요한 것만 보여주자.
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Empty_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Jewely_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//		}
//	}
//
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShowEnable( false, false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShowEnable( false, false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pButton_Remove_Stone_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShowEnable( false, false );
//		}
//	}
//
//
//	CKTDGUIStatic* pStatic_LastExtraDamage = NULL;
//	
//
//
//	for ( int i = 0; i < slotNum; i++ )
//	{
//		bool bCheckEmpty = true;
//
//		if ( i < (int)pItem->GetItemData().m_SocketOption.size() )
//		{
//			//0으로 채워져 있으면 비어있는거고 다른값으로 채워져 있는거면 있는거다..
//			int socketOption = pItem->GetItemData().m_SocketOption[i];
//			if ( socketOption != 0 )
//			{
//				bCheckEmpty = false;
//
//				wstringstream wstrstm;
//				wstrstm << L"g_pStatic_Jewely_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShow( true );
//				}
//
//				wstrstm.str( L"" );
//				wstrstm << L"g_pButton_Remove_Stone_Slot";
//				wstrstm << i + 1;
//				pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, true );
//				}
//				
//				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
//				if ( pSocketData != NULL )
//				{
//					//여기다가.. 소켓 옵션 설명 가져와서 한줄 짜리인지 두줄짜리인지 보고.. 넣자잉..
//
//					bool bCheckTwoLine = false;
//
//					wstring socketDesc = g_pMain->GetStrByLienBreak( pSocketData->GetSocketDesc().c_str(), 175, XUF_DODUM_13_SEMIBOLD );
//					if ( socketDesc.find( L"\n") != -1 )
//						bCheckTwoLine = true;
//
//					if ( bCheckTwoLine == false )
//					{
//						wstringstream wstrstm;
//						wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
//						wstrstm << i + 1;
//						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//						if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
//						{
//							pControlStatic->GetString(0)->msg = socketDesc;
//							pControlStatic->SetShow( true );
//							pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );
//
//							if( CX2DamageManager::EDT_NONE != pSocketData->m_SocketExtraDamage.m_ExtraDamageType && 
//								pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
//							{
//								if( NULL != pStatic_LastExtraDamage )
//								{
//									pStatic_LastExtraDamage->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.f ) );
//								}
//								pStatic_LastExtraDamage = pControlStatic;
//							}
//						}
//					}
//					else
//					{
//						wstringstream wstrstm;
//						wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
//						wstrstm << i + 1;
//						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//						if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
//						{
//							pControlStatic->GetString(0)->msg = socketDesc;
//							pControlStatic->SetShow( true );
//							pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );
//
//							if( CX2DamageManager::EDT_NONE != pSocketData->m_SocketExtraDamage.m_ExtraDamageType && 
//								pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
//							{
//								if( NULL != pStatic_LastExtraDamage )
//								{
//									pStatic_LastExtraDamage->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.f ) );
//								}
//								pStatic_LastExtraDamage = pControlStatic;
//							}
//						}
//					}			
//				}
//
//
//				wstringstream wstrstmRemove_Stone_Slot;
//				wstrstmRemove_Stone_Slot << L"g_pButton_Remove_Stone_Slot";
//				wstrstmRemove_Stone_Slot << i + 1;
//				pControl = m_pDLGSocketItem->GetControl( wstrstmRemove_Stone_Slot.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, true );
//				}
//
//				//보석 표시
//				//설명 보고 원라인으로 할것인지 투라인으로 할것인지?
//				//일반 마법석, 희귀 마법석 표시
//			}
//			
//		}
//
//		if ( bCheckEmpty == true )
//		{
//			//비어있는것들..
//			wstringstream wstrstm;
//			wstrstm << L"g_pStatic_Empty_Slot";
//			wstrstm << i + 1;
//			CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//			if ( pControl != NULL )
//			{
//				pControl->SetShow( true );
//			}
//
//			if ( normalStone > 0 )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, true );
//				}
//			}
//			else
//			{
//				wstringstream wstrstm;
//				wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, false );
//				}
//			}
//
//			if ( specialStone > 0 )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, true );
//				}
//			}	
//			else
//			{
//				wstringstream wstrstm;
//				wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, false );
//				}
//			}
//		}
//	}
//}
//
////
////void CX2CharacterRoom::EmptySkillSlotUI()
////{
////	if( NULL == m_pDLGSkillSlot )
////		return;
////
////	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_Skill_Slot" );
////	if( NULL == pStatic )
////		return;
////
////	pStatic->GetPicture(0)->SetShow( false );
////	pStatic->GetPicture(1)->SetShow( false );
////	pStatic->GetPicture(2)->SetShow( false );
////
////}
//
//void CX2CharacterRoom::OpenSocketCheatWindow()
//{
//	CloseSocketCheatWindow();
//	m_pDLGSocketItemCheat = new CKTDGUIDialog( m_pNowState, L"DLG_PVP_Lobby_Password_Check.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSocketItemCheat );
//	CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGSocketItemCheat->GetControl( L"Ok" );
//	if ( pButton != NULL )
//	{
//		pButton->SetCustomMsgMouseUp( (int)CX2CharacterRoom::CRM_SOCKET_ITEM_FEE_CHECK_OK );
//	}
//
//	pButton = (CKTDGUIButton*)m_pDLGSocketItemCheat->GetControl( L"Cancel" );
//	if ( pButton != NULL )
//	{
//		pButton->SetShow( false );
//		pButton->SetEnable( false );
//	}
//
//	CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*)m_pDLGSocketItemCheat->GetControl( L"EditBoxPassword" );
//	if ( pEditBox != NULL )
//	{
//		pEditBox->SetCustomMsgEnter( CRM_SOCKET_ITEM_FEE_CHECK_OK );
//	}
//	
//}
//
//void CX2CharacterRoom::CloseSocketCheatWindow()
//{
//	if ( m_pDLGSocketItemCheat != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGSocketItemCheat, NULL, false );
//
//	m_pDLGSocketItemCheat = NULL;
//}
//
//
//
//void CX2CharacterRoom::OpenExchangeOldItemWindow( CX2SlotItem* pSlot )
//{
//	if( NULL == pSlot )
//		return; 
//
//
//	if( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"장착중인 아이템은 교환 할 수 없습니다.", m_pNowState );
//		return;
//	}
//
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ((CX2SlotItem*)pSlot)->GetSortType(), 
//		((CX2SlotItem*)pSlot)->GetSlotID() );
//
//	if( NULL == pItem ||
//		NULL == pItem->GetItemTemplet() )
//		return;
//
//
//
//
//	// 장착 장비 템만 교환대상이다
//	if( CX2Item::IT_WEAPON != pItem->GetItemTemplet()->m_ItemType &&
//		CX2Item::IT_DEFENCE != pItem->GetItemTemplet()->m_ItemType )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"이 아이템은 교환 대상이 아닙니다.", m_pNowState );
//		return;
//	}
//
//	if( false == g_pData->GetItemManager()->IsCanItemExchange( pItem->GetItemTemplet()->m_ItemID ) )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"이 아이템은 교환 대상이 아닙니다.", m_pNowState );
//		return;
//	}
//
//	m_TempItemUID = pSlot->GetItemUID();
//	//const WCHAR msg[] = L"기존의 아이템을 동일한 등급의 새로운 아이템으로 교체하고 있습니다.\n동일한 등급의 아이템 중 랜덤으로 한가지가 지급 됩니다.\n#CFF0000주의! 한번 교체한 아이템은 다시 되돌릴 수 없습니다.#CX";
//	const WCHAR msg[] = L"동일한 등급의 아이템 중 하나가 지급 됩니다.\n#CFF0000주의! 한번 교체한 아이템은#CX\n#CFF0000다시 되돌릴 수 없습니다.#CX";
//	g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), msg, CRM_EXCHANGE_OLD_ITEM_OK, m_pNowState );
//
//}
//
//
//void CX2CharacterRoom::OpenAtrribEnchantWindow( CX2SlotItem* pSlot )
//{
//	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"장착중인 아이템은 속성 강화할 수 없습니다.", m_pNowState );
//
//		return;
//	}
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ((CX2SlotItem*)pSlot)->GetSortType(), 
//		((CX2SlotItem*)pSlot)->GetSlotID() );
//
//	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"아이템을 찾을 수 없습니다!!", m_pNowState );
//
//		return;
//	}
//
//
//	CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
//	if ( pItemTemplet != NULL )
//	{
//		if ( ( pItemTemplet->m_ItemType == CX2Item::IT_WEAPON ||
//			pItemTemplet->m_ItemType == CX2Item::IT_DEFENCE ) && pItemTemplet->m_bFashion == false )
//		{
//
//		}
//		else
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"장비 아이템만 속성 강화할 수 있습니다.", m_pNowState );
//
//			return;
//		}
//	}
//
//
//#ifdef ITEM_RECOVERY_TEST
//	// (사용불능인가?)
//	if(true == pItem->IsDisabled())
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"사용 불가 상태의 아이템입니다.\n[축복 받은 복원의 주문서] 아이템을 사용해서\n사용 불가 상태를 원상태로 복원할 수 있습니다.", m_pNowState );
//		return; 
//	}
//#endif
//
//	if ( pItem != NULL && pItem->GetItemTemplet() != NULL )
//	{
//		m_AttribEnchantItemUID = pItem->GetUID();
//
//		SAFE_DELETE_DIALOG( m_pDLGAttribEnchantItem );
//		if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//		{
//			m_pDLGAttribEnchantItem = new CKTDGUIDialog( m_pNowState, L"DLG_Attribute_Item_Weapon.lua" );
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttribEnchantItem );
//		}
//		else if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_DEFENCE )
//		{
//			m_pDLGAttribEnchantItem = new CKTDGUIDialog( m_pNowState, L"DLG_Attribute_Item_Armor.lua" );
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttribEnchantItem );
//		}
//	
//		/*
//		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGAttribEnchantItem->GetControl(L"g_pCheckBoxForceSocket"); 
//		if ( pCheckBox != NULL )
//		{
//			if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
//			{
//				pCheckBox->SetShow( true );
//				pCheckBox->SetEnable( true );
//			}
//			else
//			{
//				pCheckBox->SetShow( false );
//				pCheckBox->SetEnable( false );
//			}
//		}
//		*/
//
//
//
//		ResetAtrribEnchantWindow();
//	}
//
//
//}
//
//void CX2CharacterRoom::ResetAtrribEnchantWindow()
//{
//	if ( m_pDLGAttribEnchantItem == NULL )
//		return;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID ); 
//
//	if ( pItem == NULL )
//		return;
//
//
//	if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//	{
//		//무기 이미지 셋팅해주고..
//		CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_item_Slot_Image" );
//		if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
//		{
//			pStaticItemImage->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//		}
//
//		//이름 셋팅해주고..
//		CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Name" );
//		if ( pStaticItemName != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pItem->GetItemTemplet()->GetFullName().c_str();
//
//			pStaticItemName->SetString( 0, wstrstm.str().c_str() );
//		}
//
//		//듀얼 속성인지.. 엘의 조각은 몇개 필요한지..
//		CKTDGUIStatic* pStaticUpgradeNotice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Upgrade_Notice" );
//		if ( pStaticUpgradeNotice != NULL )
//		{
//			pStaticUpgradeNotice->SetString(0, L"");
//			pStaticUpgradeNotice->SetString(1, L"");
//		}
//
//		//속성 이름 셋팅 해주고..
//		CKTDGUIStatic* pStaticAttribName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Attribute" );
//		if ( pStaticAttribName != NULL )
//		{
//			vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//			if( 0 != pItem->GetItemData().m_EnchantOption1 )
//				vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption1 );
//
//			if( 0 != pItem->GetItemData().m_EnchantOption2 )
//				vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption2 );
//			
//			pStaticAttribName->SetString( 0, g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ) ).c_str() );
//		}
//
//		//듀얼 속성인지.. 엘의 조각이 몇개 필요한지.. 아니면 더 이상 속성 인챈트를 할 수 없는지.. 등등..
//		CKTDGUIStatic* pStatic_First_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_First_Attribute_Notice" );
//		if ( pStatic_First_Attribute_Notice != NULL )
//			pStatic_First_Attribute_Notice->SetShow( false );
//
//		CKTDGUIStatic* pStatic_Random_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Random_Attribute_Notice" );
//		if ( pStatic_Random_Attribute_Notice != NULL )
//			pStatic_Random_Attribute_Notice->SetShow( false );
//
//		CKTDGUIStatic* pStatic_Enable_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Enable_Notice" );
//		if ( pStatic_Enable_Notice != NULL )
//		{
//			pStatic_Enable_Notice->SetShow( false );
//
//			for ( int i = 0; i <= 6; i++ )
//			{
//				pStatic_Enable_Notice->SetString( i, L"불가능" );
//			}
//		}
//
//		CKTDGUIStatic* pStatic_Two_Line_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Two_Line_Attribute_Notice" );
//		if ( pStatic_Two_Line_Attribute_Notice != NULL )
//			pStatic_Two_Line_Attribute_Notice->SetShow( false );
//
//
//		CKTDGUIButton* pButtonSlotBig = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_El_Slot_Big" );
//		if ( pButtonSlotBig != NULL )
//		{
//			pButtonSlotBig->SetShowEnable( false, false );
//		}
//
//		CKTDGUIStatic* pStaticElPieceEx = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );
//		if ( pStaticElPieceEx != NULL )
//			pStaticElPieceEx->SetShow( false );
//
//		wstringstream wstrstm2;
//		for ( int i = 1; i <= 6; i++ )
//		{
//			wstrstm2.str( L"" );
//			wstrstm2 << L"g_pButton_El_Slot" << i;
//
//			CKTDGUIButton* pButtonSlotElSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm2.str().c_str() );
//			if ( pButtonSlotElSlot != NULL )
//				pButtonSlotElSlot->SetShowEnable( false, false );
//
//		}
//
//		
//
//		//아무런 속성이 없는 경우
//		if ( pItem->GetItemData().m_EnchantOption1 == 0 && pItem->GetItemData().m_EnchantOption2 == 0 )
//		{
//			//버튼.. 셋팅~~!!
//			//엘의 조각이 20개가 필요!!
//
//			if ( pStatic_Random_Attribute_Notice != NULL )
//				pStatic_Random_Attribute_Notice->SetShow( true );
//
//			CKTDGUIButton* pButtonSlot = NULL;
//
//			//
//			pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_El_Slot_Big" );
//			if ( pButtonSlot != NULL )
//			{
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, true, pItem->GetItemTemplet()->m_UseLevel,
//																					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				if ( itemNum >= needItemNum )
//				{
//					pButtonSlot->SetShowEnable( true, true );
//				}
//			}
//
//
//			wstringstream wstrstm;
//			for ( int i = 1; i <= 6; i++ )
//			{
//				wstrstm.str( L"" );
//				wstrstm << L"g_pButton_El_Slot" << i;
//
//				pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm.str().c_str() );
//
//				int itemID = g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i );
//
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//				if ( itemNum >= needItemNum )
//				{
//					pButtonSlot->SetShowEnable( true, true );
//				}
//				else
//				{
//					pButtonSlot->SetShowEnable( false, false );
//				}
//
//
//				pButtonSlot->SetGuideDesc( g_pData->GetEnchantItem()->GetToolTip( (CX2DamageManager::EXTRA_DAMAGE_TYPE)( (int)CX2DamageManager::EDT_ENCHANT_BLAZE + i - 1 ), NULL ).c_str()  );
//			}
//
//			CKTDGUIStatic* pStaticElPieceEx = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );
//			if ( pStaticElPieceEx != NULL )
//			{
//				pStaticElPieceEx->SetShow( true );
//
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//
//				for ( int i = 0; i <= 6; i++ )
//				{
//					wstrstm.str( L"" );
//					int itemNum = 0;
//					if ( i == 0 )
//					{
//						itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//					}
//					else
//					{
//						itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i ) );
//					}
//
//					wstrstm << L"엘의 조각";
//
//					//{{ 2009.01.19 김태완 : 코드정리
//					switch(i)
//					{
//					case 0:
//						{
//							wstrstm << L"(불명)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 1:
//						{
//							wstrstm << L"(레드)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 2:
//						{
//							wstrstm << L"(블루)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 3:
//						{
//							wstrstm << L"(그린)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 4:
//						{
//							wstrstm << L"(윈드)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 5:
//						{
//							wstrstm << L"(라이트)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 6:
//						{
//							wstrstm << L"(다크)\nX " << itemNum << L" (" << needItemNum << L")";
//						}break;
//					default:
//						{
//							ASSERT(!"Invalid Attribute stone" );
//						}
//						break;
//					}//}} elseif -> switch
//					
//					pStaticElPieceEx->SetString( i, wstrstm.str().c_str() );
//				}
//			}
//
//			
//			if ( pStatic_First_Attribute_Notice != NULL )
//				pStatic_First_Attribute_Notice->SetShow( true );
//
//			if ( pStaticUpgradeNotice != NULL )
//			{
//				wstringstream wstrstm;
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				wstrstm << L"엘의 조각 " << needItemNum << L"개 필요!";
//				pStaticUpgradeNotice->SetString(1, wstrstm.str().c_str() );
//
//			}
//		}
//		//두개의 옵션이 같이 붙어있는 경우
//		else if ( pItem->GetItemData().m_EnchantOption1 != 0 && pItem->GetItemData().m_EnchantOption2 != 0 )
//		{
//			if ( pStatic_Enable_Notice != NULL )
//				pStatic_Enable_Notice->SetShow( true );
//
//			if ( pStaticUpgradeNotice != NULL )
//			{
//				pStaticUpgradeNotice->SetString(0, L"듀얼 속성");
//			}
//		}
//		//뭔진 모르지만 한개만 붙어있는 경우..
//		else
//		{
//			if ( pStatic_Random_Attribute_Notice != NULL )
//				pStatic_Random_Attribute_Notice->SetShow( true );
//
//			//엘의 조각이 60개가 필요!!
//			int existingOption = 0;
//			if ( pItem->GetItemData().m_EnchantOption1 != 0 )
//				existingOption = pItem->GetItemData().m_EnchantOption1;
//
//			if ( pItem->GetItemData().m_EnchantOption2 != 0 )
//				existingOption = pItem->GetItemData().m_EnchantOption2;
//
//			if ( pStaticUpgradeNotice != NULL )
//			{
//				wstringstream wstrstm;
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( true, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				wstrstm << L"엘의 조각 " << needItemNum << L"개 필요!";
//				pStaticUpgradeNotice->SetString(1, wstrstm.str().c_str() );
//
//			}
//
//			//이제 불가능 한거하고.. 이제 할 수 있는거하고 능동적이게 보이게끔 해줘야할텐데..
//			//어떻게 하면 좋을까 응응응응응?
//
//			
//
//			//랜덤 옵션 부분부터 처리하고..
//
//			CKTDGUIButton* pButtonSlot = NULL;
//
//			//
//			pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_El_Slot_Big" );
//			if ( pButtonSlot != NULL )
//			{
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( true, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				if ( itemNum >= needItemNum )
//				{
//					pButtonSlot->SetShowEnable( true, true );
//				}
//			}
//
//
//
//			//다음 순서대로.. 처리해보자..
//
//			if ( pStatic_Two_Line_Attribute_Notice != NULL )
//			{
//				pStatic_Two_Line_Attribute_Notice->SetShow( true );
//
//				//자자 이제 입력 셋팅을 해볼까.. 겁나게 복잡하네잉.. 토할꺼 같다
//
//				
//				if ( pStatic_Enable_Notice != NULL )
//				{
//					pStatic_Enable_Notice->SetShow( true );
//
//					if ( pStatic_Enable_Notice != NULL )
//					{
//						pStatic_Enable_Notice->SetString( 0, L"" );
//					}
//				}
//
//				CKTDGUIStatic* pStaticElPieceEx = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );
//				if ( pStaticElPieceEx != NULL )
//				{
//					pStaticElPieceEx->SetShow( true );
//					wstringstream wstrstm;
//
//					int needItemNum = 0;
//					g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( true, true, pItem->GetItemTemplet()->m_UseLevel,
//						pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//
//					for ( int i = 0; i <= 6; i++ )
//					{
//						wstrstm.str( L"" );
//
//						int itemNum = 0;
//						if ( i == 0 )
//						{
//							itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//						}
//						else
//						{
//							itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i ) );
//						}
//
//						wstrstm << L"엘의 조각";
//
//						//{{ 2009.01.19 김태완 : 코드정리
//						switch(i)
//						{
//						case 0:
//							{
//								wstrstm << L"(불명)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 1:
//							{
//								wstrstm << L"(레드)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 2:
//							{
//								wstrstm << L"(블루)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 3:
//							{
//								wstrstm << L"(그린)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 4:
//							{
//								wstrstm << L"(윈드)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 5:
//							{
//								wstrstm << L"(라이트)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 6:
//							{
//								wstrstm << L"(다크)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						default:
//							{
//                                ASSERT( !"Invalid Attribute stone" );
//							} break;
//						}//}} elseif -> switch
//
//						if ( g_pData->GetEnchantItem()->IsPossibleToPush( (CX2EnchantItem::ENCHANT_TYPE)existingOption, (CX2EnchantItem::ENCHANT_TYPE)i ) == true )
//						{
//							pStaticElPieceEx->SetString( i, wstrstm.str().c_str() );
//						}
//						else
//						{
//							pStaticElPieceEx->SetString( i, L"" );
//						}
//					}
//				}
//				
//			
//				wstringstream wstrstm;
//				for ( int i = 1; i <= 6; i++ )
//				{	
//					wstrstm.str( L"" );
//					wstrstm << L"g_pButton_El_Slot" << i;
//
//					pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm.str().c_str() );
//
//
//					if ( g_pData->GetEnchantItem()->IsPossibleToPush( (CX2EnchantItem::ENCHANT_TYPE)existingOption, (CX2EnchantItem::ENCHANT_TYPE)i ) == true )
//					{
//						int itemID = g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i );
//
//						int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID );
//						if ( pButtonSlot != NULL )
//						{
//							int needItemNum = 0;
//							g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( true, true, pItem->GetItemTemplet()->m_UseLevel,
//								pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//							if ( itemNum >= needItemNum )
//							{
//								pButtonSlot->SetShowEnable( true, true );
//
//								vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//								if( 0 != existingOption )
//									vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) existingOption );
//
//								if( 0 != i )
//									vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) i );
//
//								pButtonSlot->SetGuideDesc( g_pData->GetEnchantItem()->GetToolTip( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ), NULL ).c_str() );
//							}
//							else
//							{
//								pButtonSlot->SetShowEnable( false, false );
//							}
//						}
//						
//
//						vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//						if( 0 != existingOption )
//							vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) existingOption );
//
//						if( 0 != i )
//							vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) i );
//
//						
//						pStatic_Two_Line_Attribute_Notice->SetString( i - 1, g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ), true ).c_str() );
//
//						if ( pStatic_Enable_Notice != NULL )
//						{
//							pStatic_Enable_Notice->SetString( i, L"" );
//						}
//					}
//					else
//					{
//						if ( pButtonSlot != NULL )
//							pButtonSlot->SetShowEnable( false, false );
//
//						pStatic_Two_Line_Attribute_Notice->SetString( i - 1, L"" );
//
//						
//					}
//				}
//			
//			}
//			
//		}
//
//
//
//		//속성 제거하기 버튼하고 어떤 속성들이 박혀있는지 셋팅해줘야지?? 
//		CKTDGUIStatic* pStaticEmptySlot1 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Empty_Slot1" );
//		if ( pStaticEmptySlot1 != NULL )
//			pStaticEmptySlot1->SetShow( false );
//
//		CKTDGUIStatic* pStaticEmptySlot2 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Empty_Slot2" );
//		if ( pStaticEmptySlot2 != NULL )
//			pStaticEmptySlot2->SetShow( false );
//
//		CKTDGUIStatic* pStaticNoticeSlot = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Notice_Slot" );
//		if ( pStaticNoticeSlot != NULL )
//		{
//			pStaticNoticeSlot->SetString( 0, L"" );
//			pStaticNoticeSlot->SetString( 1, L"" );
//		}
//
//		CKTDGUIButton* pButtonRemoveSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_Remove_El_Slot1" );
//		if ( pButtonRemoveSlot != NULL )
//		{
//			pButtonRemoveSlot->SetShowEnable( false, false );
//		}
//
//		CKTDGUIButton* pButtonRemoveSlot2 = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_Remove_El_Slot2" );
//		if ( pButtonRemoveSlot2 != NULL )
//		{
//			pButtonRemoveSlot2->SetShowEnable( false, false );
//		}
//
//		if ( pItem->GetItemData().m_EnchantOption1 == 0 )
//		{
//			if ( pStaticEmptySlot1 != NULL )
//				pStaticEmptySlot1->SetShow( true );
//		}
//		else
//		{
//			if ( pStaticNoticeSlot != NULL )
//			{
//				vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//				if( 0 != pItem->GetItemData().m_EnchantOption1 )
//					vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) pItem->GetItemData().m_EnchantOption1 );
//
//				pStaticNoticeSlot->SetString( 0, g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ) ).c_str() );
//			}
//
//			if ( pButtonRemoveSlot != NULL )
//				pButtonRemoveSlot->SetShowEnable( true, true );
//		}
//
//		if ( pItem->GetItemData().m_EnchantOption2 == 0 )
//		{
//			if ( pStaticEmptySlot2 != NULL )
//				pStaticEmptySlot2->SetShow( true );
//		}
//		else
//		{
//			if ( pStaticNoticeSlot != NULL )
//			{
//				vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//				if( 0 != pItem->GetItemData().m_EnchantOption2 )
//					vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) pItem->GetItemData().m_EnchantOption2 );
//
//				pStaticNoticeSlot->SetString( 1, g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ) ).c_str() );
//			}
//
//			if ( pButtonRemoveSlot2 != NULL )	
//				pButtonRemoveSlot2->SetShowEnable( true, true );
//		}
//
//	}
//	else if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_DEFENCE )
//	{
//		
//
//		
//
//		//이미지 셋팅해주고..
//		CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_item_Slot_Image" );
//		if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
//		{
//			pStaticItemImage->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//		}
//
//		//이름 셋팅해주고..
//		CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Name" );
//		if ( pStaticItemName != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pItem->GetItemTemplet()->GetFullName().c_str();
//
//			pStaticItemName->SetString( 0, wstrstm.str().c_str() );
//		}
//
//		//엘의 조각은 몇개 필요한지..
//		CKTDGUIStatic* pStaticUpgradeNotice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Upgrade_Notice" );
//		if ( pStaticUpgradeNotice != NULL )
//		{
//			pStaticUpgradeNotice->SetString(0, L"");
//		}
//
//
//
//		//속성 이름 셋팅 해주고..
//		CKTDGUIStatic* pStaticAttribName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Attribute" );
//		if ( pStaticAttribName != NULL )
//		{
//			
//			pStaticAttribName->SetString( 0, g_pData->GetEnchantItem()->GetEnchantResistName( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption1 ).c_str() );
//		}
//
//		CKTDGUIStatic* pStaticElPieceEx = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );
//		if ( pStaticElPieceEx != NULL )
//			pStaticElPieceEx->SetShow( false );
//
//		CKTDGUIStatic* pStatic_Enable_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Enable_Notice" );
//		if ( pStatic_Enable_Notice != NULL )
//			pStatic_Enable_Notice->SetShow( false );
//
//		CKTDGUIStatic* pStatic_Random_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Random_Attribute_Notice" );
//		if ( pStatic_Random_Attribute_Notice != NULL )
//			pStatic_Random_Attribute_Notice->SetShow( false );
//
//		CKTDGUIButton* pButtonRandom = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_El_Slot_Big" );
//		if ( pButtonRandom != NULL )
//			pButtonRandom->SetShowEnable( false, false );
//
//		wstringstream wstrstm2;
//		for ( int i = 1; i <= 6; i++ )
//		{
//			wstrstm2.str( L"" );
//			wstrstm2 << L"g_pButton_El_Slot" << i;
//
//			CKTDGUIButton* pButtonSlotElSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm2.str().c_str() );
//			if ( pButtonSlotElSlot != NULL )
//				pButtonSlotElSlot->SetShowEnable( false, false );
//
//		}
//		
//
//		//아무런 속성이 없는 경우
//		if ( pItem->GetItemData().m_EnchantOption1 == 0 && pItem->GetItemData().m_EnchantOption2 == 0 )
//		{
//			if ( pStatic_Random_Attribute_Notice != NULL )
//				pStatic_Random_Attribute_Notice->SetShow( true );
//
//			wstringstream wstrstm;
//
//			if ( pStaticUpgradeNotice != NULL )
//			{
//				wstrstm.str( L"" );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, false, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				wstrstm << L"엘의 조각 " << needItemNum << L"개 필요";
//				pStaticUpgradeNotice->SetString(0, wstrstm.str().c_str() );
//			}
//
//
//			if ( pButtonRandom != NULL )
//			{
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, false, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				if ( itemNum >= needItemNum )
//				{
//					pButtonRandom->SetShowEnable( true, true );
//				}
//			}
//
//
//		
//			for ( int i = 1; i <= 6; i++ )
//			{
//				wstrstm.str( L"" );
//				wstrstm << L"g_pButton_El_Slot" << i;
//
//				CKTDGUIButton* pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm.str().c_str() );
//
//				int itemID = g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i );
//
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, false, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//				if ( itemNum >= needItemNum )
//				{
//					pButtonSlot->SetShowEnable( true, true );
//				}
//				else
//				{
//					pButtonSlot->SetShowEnable( false, false );
//				}
//			}
//
//
//			if ( pStaticElPieceEx != NULL )
//			{
//				pStaticElPieceEx->SetShow( true );
//
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, false, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//				for ( int i = 0; i <= 6; i++ )
//				{
//					wstrstm.str( L"" );
//					int itemNum = 0;
//					if ( i == 0 )
//					{
//						itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//					}
//					else
//					{
//						itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i ) );
//
//					}
//
//					wstrstm << L"엘의 조각";
//
//					//{{ 2009.01.19 김태완 : 코드정리
//					switch(i)
//					{
//					case 0:
//						{
//							wstrstm << L"(불명)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 1:
//						{
//							wstrstm << L"(레드)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 2:
//						{
//							wstrstm << L"(블루)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 3:
//						{
//							wstrstm << L"(그린)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 4:
//						{
//							wstrstm << L"(윈드)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 5:
//						{
//							wstrstm << L"(라이트)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 6:
//						{
//							wstrstm << L"(다크)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					default:
//						{
//							ASSERT( !"Invaild Attribute stone" );
//						}
//						break;
//					}//}} elseif -> switch
//					
//
//					pStaticElPieceEx->SetString( i, wstrstm.str().c_str() );
//				}
//			}
//		}
//		else
//		{
//			if ( pStatic_Enable_Notice != NULL )
//				pStatic_Enable_Notice->SetShow( true );	
//
//			
//		}
//
//
//
//		//밑에 속성 쪽
//
//		//속성 제거하기 버튼하고 어떤 속성들이 박혀있는지 셋팅해줘야지?? 
//		CKTDGUIStatic* pStaticEmptySlot1 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Empty_Slot1" );
//		if ( pStaticEmptySlot1 != NULL )
//			pStaticEmptySlot1->SetShow( false );
//
//		
//		CKTDGUIStatic* pStaticNoticeSlot = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Notice_Slot" );
//		if ( pStaticNoticeSlot != NULL )
//		{
//			pStaticNoticeSlot->SetString( 0, L"" );
//			
//		}
//
//		CKTDGUIButton* pButtonRemoveSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_Remove_El_Slot1" );
//		if ( pButtonRemoveSlot != NULL )
//		{
//			pButtonRemoveSlot->SetShowEnable( false, false );
//		}
//
//		
//
//		if ( pItem->GetItemData().m_EnchantOption1 == 0 )
//		{
//			if ( pStaticEmptySlot1 != NULL )
//				pStaticEmptySlot1->SetShow( true );
//		}
//		else
//		{
//			if ( pStaticNoticeSlot != NULL )
//			{
//				
//				pStaticNoticeSlot->SetString( 0, g_pData->GetEnchantItem()->GetEnchantResistName( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption1 ).c_str() );
//			}
//
//			if ( pButtonRemoveSlot != NULL )
//				pButtonRemoveSlot->SetShowEnable( true, true );
//		}
//
//	}
//}
//
////엘 감정, 되돌리기
//void CX2CharacterRoom::OpenElChangerWindow( bool bIdentify, CX2Item* pItem )
//{
//	SAFE_DELETE_DIALOG( m_pDLGElChanger );
//
//	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
//		return;
//
//	m_ElChangerItemUID		= pItem->GetItemData().m_ItemUID;
//	m_ElChangerItemCount	= 1;
//
//	m_pDLGElChanger = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_El_Changer_Window.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGElChanger );
//
//	//아이템 이미지 셋팅해주고.
//	CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"ItemImage" );
//	if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
//	{
//		pStaticItemImage->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//	}
//
//	//아이템 이름 셋팅해주고.
//	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Price" );
//	if ( pStaticItemName != NULL )
//	{
//		pStaticItemName->SetString(0, pItem->GetItemTemplet()->m_Name.c_str() );
//	}
//
//	//아이템의 최대 갯수 셋팅해줘야겠지.. 하나 만들자..
//	CKTDGUIStatic* pStaticItemNum3 = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num3" );
//	if ( pStaticItemNum3 != NULL )
//	{
//		wstringstream wstrstm3;
//		wstrstm3 << pItem->GetItemData().m_Quantity;
//		pStaticItemNum3->SetString(0, wstrstm3.str().c_str() );
//	}
//
//	//아이템 바꿀 갯수 셋팅해주고.
//	CKTDGUIStatic* pStaticItemNum = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num" );
//	if ( pStaticItemNum != NULL )
//	{
//		pStaticItemNum->SetString(0, L"1" );
//	}
//
//	//감정하는건지 되돌리는건지 셋팅해주고.
//	//감정하는거면 감정석.. 되돌리는거면 복구석 셋팅해주자 음허허허허허허허허허허허허허허허허...
//	if ( bIdentify == true )
//	{
//		CKTDGUIControl* pControl = m_pDLGElChanger->GetControl( L"Identify_Tool" );
//		if ( pControl != NULL )
//			pControl->SetShow( true );
//
//		int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE );
//
//		CKTDGUIStatic* pStaticItemNum2 = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num2" );
//	
//		if ( pStaticItemNum2 != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << itemNum << L"개";
//			pStaticItemNum2->SetString( 0, wstrstm.str().c_str() );
//		}
//		
//	}
//	else
//	{
//		CKTDGUIControl* pControl = m_pDLGElChanger->GetControl( L"Identify_Tool2" );
//		if ( pControl != NULL )
//			pControl->SetShow( true );
//
//		int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE );
//
//		CKTDGUIStatic* pStaticItemNum2 = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num2" );
//
//		if ( pStaticItemNum2 != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << itemNum << L"개";
//			pStaticItemNum2->SetString( 0, wstrstm.str().c_str() );
//		}
//	}
//}
//
//void CX2CharacterRoom::AdjustElChangerItemCount( const WCHAR* wszAdjustCommand )
//{
//	//before, we must check the type if It is an Identifier or an returner.
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_ElChangerItemUID );
//	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
//		return;
//
//	int nowQuantity = pItem->GetItemData().m_Quantity;
//
//	int needStoneQuantity = 0;
//	if ( pItem->GetItemTemplet()->m_ItemID == CX2EnchantItem::ATI_UNKNOWN )
//	{
//		//identifier
//		needStoneQuantity = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE );
//	}
//	else
//	{
//		//returner
//		needStoneQuantity = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE );
//	}
//
//	if ( wcscmp( wszAdjustCommand, L"Up10" ) == 0 )
//	{
//		m_ElChangerItemCount += 10;
//
//		if ( m_ElChangerItemCount >= nowQuantity )
//			m_ElChangerItemCount = nowQuantity;
//
//		if ( m_ElChangerItemCount >= needStoneQuantity )
//			m_ElChangerItemCount = needStoneQuantity;
//	}
//	else if ( wcscmp( wszAdjustCommand, L"Down10" ) == 0  )
//	{
//		m_ElChangerItemCount -= 10;
//		if ( m_ElChangerItemCount <= 1 )
//			m_ElChangerItemCount = 1;
//	}
//	else if ( wcscmp( wszAdjustCommand, L"Up1" ) == 0  )
//	{
//		m_ElChangerItemCount += 1;
//
//		if ( m_ElChangerItemCount >= nowQuantity )
//			m_ElChangerItemCount = nowQuantity;
//
//		if ( m_ElChangerItemCount >= needStoneQuantity )
//			m_ElChangerItemCount = needStoneQuantity;
//	}
//	else if ( wcscmp( wszAdjustCommand, L"Down1" ) == 0  )
//	{
//		m_ElChangerItemCount -= 1;
//		if ( m_ElChangerItemCount <= 1 )
//			m_ElChangerItemCount = 1;
//	}
//
//	if ( m_pDLGElChanger != NULL )
//	{
//		//아이템 바꿀 갯수 셋팅해주고.
//		CKTDGUIStatic* pStaticItemNum = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num" );
//		if ( pStaticItemNum != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << m_ElChangerItemCount;
//			pStaticItemNum->SetString(0, wstrstm.str().c_str() );
//		}
//	}
//
//	
//
//}
//
//void CX2CharacterRoom::CheckMyInfoChange()
//{
//	if ( m_TotalED < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
//	{
//		int changeED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED - m_TotalED;
//		wstringstream wstrstm;
//		wstrstm << L"ED +" << changeED;
//		m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 869,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//
//	if ( m_TotalED > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
//	{
//		int changeED = m_TotalED - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
//		wstringstream wstrstm;
//		wstrstm << L"ED -" << changeED;
//		m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 869,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//
//	m_TotalED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
//
//	if ( m_TotalExp < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP )
//	{
//		int changeEXP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP - m_TotalExp;
//		wstringstream wstrstm;
//		wstrstm << L"EXP +" << changeEXP;
//		m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 707,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//
//	if ( m_TotalExp > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP )
//	{
//		int changeEXP = m_TotalExp - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
//		wstringstream wstrstm;
//		wstrstm << L"EXP -" << changeEXP;
//		m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 707,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//	m_TotalExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
//
//	if ( m_TotalVP < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
//	{
//		int changeVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint - m_TotalVP;
//		wstringstream wstrstm;
//		wstrstm << L"VP +" << changeVP;
//		m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 969,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//
//	if ( m_TotalVP > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
//	{
//		int changeVP = m_TotalVP - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
//		wstringstream wstrstm;
//		wstrstm << L"VP -" << changeVP;
//		m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 969,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//	m_TotalVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
//
//	if ( m_TotalLevel < g_pData->GetSelectUnitLevel() )
//	{
//		for ( int i = 0; i < 3; i++ )
//		{
//			CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
//			pSeq = g_pData->GetUIMajorParticle()->CreateSequence( L"HyperBallTick", 
//				698, 730, 
//				0.0f, 1000,1000, -1, 10 );
//			if( pSeq != NULL )
//			{
//				pSeq->SetOverUI( true );
//				pSeq->SetBlackHolePosition( pSeq->GetPosition() );
//				pSeq->UseLookPoint( true );
//				pSeq->SetLookPoint( pSeq->GetPosition() );
//			}
//			pSeq = g_pData->GetUIMajorParticle()->CreateSequence( L"HyperBallRing", 
//				698, 730,
//				0.0f, 1000,1000, -1, 1 );
//			if( pSeq != NULL )
//				pSeq->SetOverUI( true );
//		}
//		
//		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // 체험 아이디 제한
//		{
//			CNMCOClientObject::GetInstance().ChangeMyLevel( ( (UINT32)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass << 24 ) | (UINT32)g_pData->GetSelectUnitLevel(), kUserFlag_GeneralLevelUp );
//		}
//	}
//
//	m_TotalLevel = g_pData->GetSelectUnitLevel();
//
//	
//}
//
//void CX2CharacterRoom::ResetInvenPage( int nowPage, int maxPage )
//{
//	m_NowInvenSortTypePageNum = nowPage;
//	m_NowInvenSortTypePageMaxNum = maxPage;
//
//	ResetInvenPageUI();
//}
//
//void CX2CharacterRoom::ResetInvenPageUI()
//{
//	//m_NowInvenSortTypePageMaxNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetInvenSlot() m_NowInventorySortType
//	//int invenMaxSize = GetInvenMaxSize( m_NowInventorySortType );
//
//	if ( m_pDLGMyInfoInventory != NULL )
//	{
//		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"Static_My_Inven_Page" );
//		if ( pStatic != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << m_NowInvenSortTypePageNum << L"/" << m_NowInvenSortTypePageMaxNum;
//			pStatic->SetString( 0, wstrstm.str().c_str() );
//		}
//	}
//	
//}
//
//void CX2CharacterRoom::NextInvenPage()
//{
//	if ( m_NowInvenSortTypePageNum >= m_NowInvenSortTypePageMaxNum )
//		return;
//
//	m_NowInvenSortTypePageNum++;
//
//	SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum );
//}
//
//void CX2CharacterRoom::PrevInvenPage()
//{
//	if ( m_NowInvenSortTypePageNum <= 1 )
//		return;
//
//	m_NowInvenSortTypePageNum--;
//
//	SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum );
//}
//
//
//void CX2CharacterRoom::CreatLevelupEffect()
//{
//	if( NULL == g_pData->GetUIMajorParticle() )
//		return;
//
//
//	CKTDGXMeshPlayer::CXMeshInstance* pInst = NULL;
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp01_1", 
//		m_pUnitViewerUI->GetMatrix().GetPos().x,
//		m_pUnitViewerUI->GetMatrix().GetPos().y - 25.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().z, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.3f );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp01",
//		m_pUnitViewerUI->GetMatrix().GetPos().x,
//		m_pUnitViewerUI->GetMatrix().GetPos().y - 25.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().z, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.3f );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp04",
//		m_pUnitViewerUI->GetMatrix().GetPos().x,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z, 
//		0,0,0, 0,0,0 );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp02", 
//		m_pUnitViewerUI->GetMatrix().GetPos().x + 5.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z + 30.0f, 
//		0,0,0, 0,0,0 );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp02", 
//		m_pUnitViewerUI->GetMatrix().GetPos().x + 5.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z + 30.0f, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.1f );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp02",
//		m_pUnitViewerUI->GetMatrix().GetPos().x + 5.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z + 30.0f, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.2f );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp02",
//		m_pUnitViewerUI->GetMatrix().GetPos().x + 5.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z + 30.0f, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.3f );
//
//
//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUp.ogg", false, false );
//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUpBGM.ogg", false, false );
//
//	//m_fPlayBGM = -1;
//
//	
//
//	g_pData->GetUIMajorParticle()->CreateSequence( L"LevelUp", 269, 361, 0, 1000, 1000, 1, 1, 1 );
//	CKTDGParticleSystem::CParticleEventSequence* pPartInst = NULL;
//	pPartInst = g_pData->GetUIMajorParticle()->CreateSequence( L"LobbyLevelUp01", 
//		m_pUnitViewerUI->GetMatrix().GetPos() );
//	pPartInst->SetLatency( 2.266f );
//}
//
//
//void CX2CharacterRoom::SellItemNumChange( int iNum )
//{
//	CX2Item* pSellItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SellItemUID );
//	if ( pSellItem != NULL )
//	{
//		int maxItemSellNum = pSellItem->GetItemData().m_Quantity;
//		m_SellItemNum += iNum;
//
//		if ( m_SellItemNum <= 1 )
//			m_SellItemNum = 1;
//	
//		if ( m_SellItemNum >= maxItemSellNum )
//			m_SellItemNum = maxItemSellNum;
//
//		WCHAR buff[256] = {0};
//		CX2Item* pItem = pSellItem;
//
//		CKTDGUIStatic* pStaticItemSellED = (CKTDGUIStatic*)m_pDLGSelectSellNumWindowPopup->GetControl( L"StaticItem_Sell_ED" );
//		if ( pStaticItemSellED != NULL && pStaticItemSellED->GetString(0) != NULL )
//		{
//			int itemSellED = pItem->GetEDToSell();
//			wsprintf( buff, L"%d", itemSellED * m_SellItemNum );
//			pStaticItemSellED->GetString(0)->msg = buff;			
//		}
//
//		CKTDGUIStatic* pStaticItemSellNum = (CKTDGUIStatic*)m_pDLGSelectSellNumWindowPopup->GetControl( L"Item_Num" );
//		if ( pStaticItemSellNum != NULL && pStaticItemSellED->GetString(0) != NULL )
//		{
//			wsprintf( buff, L"%d", m_SellItemNum );
//			pStaticItemSellNum->GetString(0)->msg = buff;
//		}
//	}
//}
//
//void CX2CharacterRoom::DecideSellItemNum( bool bCheck )
//{
//	if ( bCheck == true )	//결정한 경우
//	{
//		Handler_EGS_SELL_ITEM_REQ();
//	}
//
//	if ( m_pDLGSelectSellNumWindowPopup != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGSelectSellNumWindowPopup, NULL, false );
//
//	m_pDLGSelectSellNumWindowPopup = NULL;
//}
//
//void CX2CharacterRoom::OpenDLGAllEquippingItemRepair()
//{
//	int edToRepair = 0;
//	int vpToRepair = 0;
//	m_vecAllEquippingItem.clear();
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//
//
//	
//	
//
//	for ( int i = 0; i < pInventory->GetItemMaxNum( CX2Inventory::ST_E_EQUIP ); i++ )
//	{
//		CX2Item* pItem = pInventory->GetItem( CX2Inventory::ST_E_EQUIP, i );
//		if ( pItem == NULL || pItem->GetItemTemplet() == NULL || pItem->GetItemTemplet()->m_PeriodType != CX2Item::PT_ENDURANCE )
//			continue;
//
//		edToRepair += pItem->GetEDToRepair();
//		vpToRepair += pItem->GetVPToRepair();
//		m_vecAllEquippingItem.push_back( pItem->GetUID() );
//	}
//
//	float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
//	if( fDiscountRate > 0.f )
//	{
//		edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
//	}
//
//
//
//	WCHAR buff[256] = {0};
//	if ( vpToRepair == 0 )
//		wsprintf( buff, L"장착 중인 아이템을 모두 수리하는데\n%d ED가 소모됩니다.\n수리 하시겠습니까?", edToRepair );
//	else
//		wsprintf( buff, L"장착 중인 아이템을 모두 수리하는데\n%d ED와 #CFF0000%d VP#CX가 소모됩니다.\n수리 하시겠습니까?", edToRepair, vpToRepair );
//
//	m_pDLGAllEquippingItemRepair = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), buff, CRM_ALL_EQUIPPING_ITEM_REPAIR_OK, m_pNowState, CRM_ALL_EQUIPPING_ITEM_REPAIR_CANCEL );
//}
//
//void CX2CharacterRoom::AllEquippingItemRepairREQ()
//{
//	if ( m_pDLGAllEquippingItemRepair != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGAllEquippingItemRepair, NULL, false );
//
//	m_pDLGAllEquippingItemRepair = NULL;
//
//	KEGS_REPAIR_ITEM_REQ kPacket;
//	kPacket.m_vecItemUID = m_vecAllEquippingItem;
//
//	int edToRepair = 0;
//	int vpToRepair = 0;
//
//	for ( int i = 0; i < (int)m_vecAllEquippingItem.size(); i++ )
//	{
//		UidType itemUID = m_vecAllEquippingItem[i];
//		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID );
//		if ( pItem == NULL || pItem->GetItemTemplet() == NULL || pItem->GetItemTemplet()->m_PeriodType != CX2Item::PT_ENDURANCE )
//			continue;
//
//		edToRepair += pItem->GetEDToRepair();
//		vpToRepair += pItem->GetVPToRepair();
//	}
//
//
//	float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
//	if( fDiscountRate > 0.f )
//	{
//		edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
//	}
//
//
//
//
//	if ( edToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"ED가 부족하여 수리할 수 없습니다!", m_pNowState );
//		return;
//	}
//	if ( vpToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"VP가 부족하여 수리할 수 없습니다!", m_pNowState );
//		return;
//	}
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_REPAIR_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_REPAIR_ITEM_ACK );
//
//	
//
//	m_pDLGAllEquippingItemRepair = NULL;
//}
//
//
//
//void CX2CharacterRoom::SetCharacterRoomInventoryState()
//{
//	m_NowCharacterRoomState = CRS_INVENTORY;
//
//	//m_pUnitViewerUI->PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
//	m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING ); 
//	m_pUnitViewerUI->SetShowObject( true );
//
//	CX2UnitViewerUI::SetUnitClicked( false );
//	m_fMouseSensitivity = 0.01f;
//	m_fRemainRotY = 0.0f;
//	m_bRotateReturn = true;
//
//	m_pDLGMyInfoInventory->SetEnable( true );
//	m_pDLGMyInfoInventory->SetShow( true );
//	m_pUnitViewerUI->SetShowObject( true );
//
//	m_pDLGMyInfoQuickSlotNumber->SetEnable( true );
//	m_pDLGMyInfoQuickSlotNumber->SetShow( true );
//
//	m_pDLGMyInfoFront->SetShow( true );
//	m_pDLGMyInfoFront->SetEnable( true );
//
//	m_pDLGSkillSlot->SetShowEnable( true, true );
//
//	//m_pDLGSkillSlot->SetPos( D3DXVECTOR2(0,0 ) );
//
//	/*
//	CKTDGUIStatic* pkStaticEDnCash = (CKTDGUIStatic*) m_pDLGMyInfoFront->GetControl( L"StaticPVP_Room_Unit_ED" );
//	pkStaticEDnCash->SetShowEnable( true, true );
//	*/
//	SetEnable( CX2Slot::ST_EQUIPPED, true );
//	SetEnable( CX2Slot::ST_INVENTORY, true );
//	SetShow( CX2Slot::ST_EQUIPPED, true );
//	SetShow( CX2Slot::ST_INVENTORY, true );
//	/*
//	CKTDGUIRadioButton* pRadioButtonUnit_Inventory_Equip = (CKTDGUIRadioButton*)m_pDLGMyInfoInventory->GetControl( L"RadioButtonUnit_Inventory_Equip" );
//	if ( pRadioButtonUnit_Inventory_Equip->GetCheckedControl() != NULL )
//	{
//	pRadioButtonUnit_Inventory_Equip->GetCheckedControl()->SetChecked( false );
//	}
//	pRadioButtonUnit_Inventory_Equip->SetChecked( true );
//	*/
//
//	UpdateExpGageBar();
//	UpdateSP();
//
//	WCHAR buff[256] = {0};
//
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//	CKTDGUIStatic* pStaticPVP_Room_Unit_WinNum = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Info_PVP_Win" );
//	if( NULL != pStaticPVP_Room_Unit_WinNum &&
//		NULL != pStaticPVP_Room_Unit_WinNum->GetString(0) )
//	{	
//		wsprintf( buff, L"%d승 %d패", (int)pUnitData->m_Win, (int)pUnitData->m_Lose );
//		pStaticPVP_Room_Unit_WinNum->GetString(0)->msg = buff;
//	}
//
//	CKTDGUIStatic* pStaticPVP_Room_Unit_VP = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Info_VP1" );
//	wsprintf( buff, L"%d / %d", (int)pUnitData->m_VSPoint, (int)pUnitData->m_VSPointMax );
//	pStaticPVP_Room_Unit_VP->GetString(0)->msg = buff;
//
//	CKTDGUIStatic* pStaticPVP_Room_Unit_Title = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Info_PVP_Grade" );
//	if ( pStaticPVP_Room_Unit_Title != NULL && g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem() != NULL )
//	{
//		pStaticPVP_Room_Unit_Title->GetString(0)->msg = g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem()->m_EmblemName;
//		pStaticPVP_Room_Unit_Title->GetPicture(0)->SetTex( g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem()->m_TextureName.c_str(),
//			g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem()->m_TextureKey.c_str() );
//	}
//
//	ResetSpirit();
//
//	
//	RemoveNoEnduranceNowEquip();
//
//	SetInventorySort( m_NowInventorySortType );
//}
//
//
//
//void CX2CharacterRoom::SetCharacterRoomQuestState()
//{
//	m_NowCharacterRoomState = CRS_QUEST;
//
//	SetEnable( CX2Slot::ST_EQUIPPED, true );
//	SetShow( CX2Slot::ST_EQUIPPED, true );
//
//	m_pDLGMyInfoQuickSlotNumber->SetEnable( true );
//	m_pDLGMyInfoQuickSlotNumber->SetShow( true );
//	
//
//	m_pUnitViewerUI->SetShowObject( true );
//}
//
//
//void CX2CharacterRoom::SetCharacterRoomSkillTreeState()
//{
//	m_NowCharacterRoomState = CRS_SKILL_TREE;
//
//	//m_pDLGSkillSlot->SetPos( D3DXVECTOR2(0,5) );
//
//
//	if( NULL != m_pSkillTreeUI )
//	{
//		m_pSkillTreeUI->SetShow( true );
//	}
//
//	ResetSkillSlotUI();
//
//}
//
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem )
//{
//	if ( pFromCX2SlotItem == NULL || pToCX2SlotItem == NULL )
//		return false;
//
//	if ( pFromCX2SlotItem->GetSortType() == pToCX2SlotItem->GetSortType() && pFromCX2SlotItem->GetSlotID() == pToCX2SlotItem->GetSlotID() )
//		return false;
//
//	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ;
//
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType	= pFromCX2SlotItem->GetSortType();
//	if ( pFromCX2SlotItem->GetSlotType() == CX2Slot::ST_INVENTORY )
//	{
//		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID() + (( m_NowInvenSortTypePageNum - 1 ) * MY_INVENTORY_SHOW_MAX_SLOT_NUM);
//	}
//	else
//	{
//		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID();
//	}
//	
//
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType	= pToCX2SlotItem->GetSortType();
//
//	if ( pToCX2SlotItem->GetSlotType() == CX2Slot::ST_INVENTORY )
//	{
//		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= pToCX2SlotItem->GetSlotID() + (( m_NowInvenSortTypePageNum - 1 ) * MY_INVENTORY_SHOW_MAX_SLOT_NUM);
//	}
//	else
//	{
//		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= pToCX2SlotItem->GetSlotID();
//	}
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ );
//	g_pMain->AddServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK );
//
//	return true;
//}
//
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID )
//{
//	if ( fromSortType == toSortType && fromSlotID == toSlotID )
//		return false;
//
//	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ;
//
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType	= fromSortType;
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= fromSlotID;
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType	= toSortType;
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= toSlotID;
//
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ );
//	g_pMain->AddServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK );
//
//	return true;
//}
//
//
//
//
//// 내 인벤토리에 있는 강화 보조제 개수 
//int CX2CharacterRoom::GetRequiredEnchantAdjuvantItemID( int iItemLevel )
//{
//	if( iItemLevel <= 20 )
//	{
//		return 206730;
//	}
//	else if( iItemLevel <= 30 )
//	{
//		return 206740;
//	}
//	else if( iItemLevel <= 40 )
//	{
//		return 206750;
//	}
//	else if( iItemLevel <= 50 )
//	{
//		return 206760;
//	}
//	else if( iItemLevel <= 60 )
//	{
//		return 206770;
//	}
//	else if( iItemLevel <= 70 )
//	{
//		return 206780;
//	}
//	else if( iItemLevel <= 80 )
//	{
//		return 206790;
//	}
//	else if( iItemLevel <= 90 )
//	{
//		return 206800;
//	}
//	else if( iItemLevel <= 100 )
//	{
//		return 206810;
//	}
//	else
//	{
//		ASSERT( !"invalid item level" );
//		return -1;
//	}
//}
//
//#ifdef ITEM_RECOVERY_TEST
//// 아이템 레벨에 따라 필요한 아이템 복원석의 ID
//int CX2CharacterRoom::GetRequiredRecoveryStoneID( int iItemLevel )
//{
//	if( iItemLevel <= 20 )
//	{
//		return 206880;	// 복원석 lv1
//	}
//	else if( iItemLevel <= 30 )
//	{
//		return 206890;	// 복원석 lv2
//	}
//	else if( iItemLevel <= 40 )
//	{
//		return 206900;	// 복원석 lv3
//	}
//	else if( iItemLevel <= 50 )
//	{
//		return 206910;	// 복원석 lv4
//	}
//	else if( iItemLevel <= 60 )
//	{
//		return 206920;	// 복원석 lv5
//	}
//	else if( iItemLevel <= 70 )
//	{
//		return 206930;	// 복원석 lv6
//	}
//	else if( iItemLevel <= 80 )
//	{
//		return 206940;	// 복원석 lv7
//	}
//	else if( iItemLevel <= 90 )
//	{
//		return 206950;	// 복원석 lv8
//	}
//	else if( iItemLevel <= 100 )
//	{
//		return 206960;	// 복원석 lv9
//	}
//	else
//	{
//		ASSERT( !"invalid item level" );
//		return -1;
//	}
//
//}
//bool CX2CharacterRoom::Handler_EGS_RESTORE_ITEM_REQ( UidType RecoveryItemUid )
//{
//	// 일단 아이템 있는지 다시 확인해주고
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( RecoveryItemUid );
//	if(pItem == NULL) 
//		return false;
//	
//	KEGS_RESTORE_ITEM_REQ kEGS_RESTORE_ITEM_REQ;
//	
//	kEGS_RESTORE_ITEM_REQ.m_iItemUID = RecoveryItemUid;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_RESTORE_ITEM_REQ, kEGS_RESTORE_ITEM_REQ );
//	g_pMain->AddServerPacket( EGS_RESTORE_ITEM_ACK );
//
//	return true;
//
//}
//
//bool CX2CharacterRoom::Handler_EGS_RESTORE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_RESTORE_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	g_pMain->DeleteServerPacket( EGS_RESTORE_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"아이템이 복원되었습니다.", m_pNowState );
//
//			return true;
//		}		
//	}
//
//	return false;
//}
//
//#endif
//
//
//#ifdef TITLE_SYSTEM
//void CX2CharacterRoom::SetTitleName()
//{    
//    if(m_pDLGMyInfoInventory != NULL && m_pDLGMyInfoInventory->GetShow() == true)
//    {
//        CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Title" );
//        if(pStaticUserInfo != NULL)
//            pStaticUserInfo->GetString(0)->msg = g_pData->GetTitleManager()->GetTitleName();        
//    }    
//}
//
//void CX2CharacterRoom::SetTitleNew(bool val)
//{
//    if(m_pDLGMyInfoInventory != NULL && m_pDLGMyInfoInventory->GetShow() == true)
//    {
//        CKTDGUIStatic* pStaticNewTitle = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"g_pStatictitle" );
//        if(pStaticNewTitle != NULL)
//            pStaticNewTitle->GetPicture(0)->SetShow(val);        
//    }
//}
//#endif
//
//
//bool CX2CharacterRoom::CheckUseItem(CX2Item *pItem)
//{
//#ifdef TITLE_SYSTEM
//    if(pItem != NULL && pItem->GetItemTemplet()->m_ItemID == 130063) // 네잎클로버
//    {
//        if(g_pData != NULL && g_pData->GetTitleManager() != NULL && g_pData->GetTitleManager()->GetTakeTitle(80) == true)
//        {
//            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"아이템을 사용할 수 없습니다.", m_pNowState );
//
//            return false;
//        }
//    }
//#endif
//
//    return true;
//}
//
//#ifdef ATTRACTION_ITEM_TEST
//
//void CX2CharacterRoom::UpdateOpenAttractionItemAck()
//{
//	// 예전 창을 지우고..
//	if ( m_pDLGOpenAttraction != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGOpenAttraction, NULL, false );
//	m_pDLGOpenAttraction = NULL;
//	m_bOpenedAttractionItem = false;
//
//	// 새로 열려라
//	m_pDLGAttractionResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Charm_Item_Result.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttractionResult );
//
//	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
//	{
//		// 그냥 아이템 + 보너스 아이템 두개만 나온다고 가정하고..
//		std::map< int, int >::iterator i;
//
//		int nCount = 0;
//		wstring getExplanation = L"";
//		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
//		{
//			int resultItemID = i->first;
//			int resultNum	= i->second;
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//			if ( pItemTemplet == NULL ) continue;
//
//			int MAGIC_ATTRACTION_BONUS_ITEM_ID = 91610;
//			CKTDGUIStatic* pStatic;
//			if(resultItemID == MAGIC_ATTRACTION_BONUS_ITEM_ID)
//			{
//				WCHAR buf[256] = {0};
//				wstring itemName = L"";
//				// 보너스 아이템
//				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Bonus" );
//
//				itemName = pItemTemplet->GetFullName();
//
//				wsprintf( buf, L"%s %d개", itemName.c_str(), resultNum );
//				pStatic->GetString(0)->msg = buf;
//			}
//			else
//			{
//				// 그냥 아이템
//				WCHAR buf[256] = {0};
//				wstring itemName = L"";
//				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Item" );
//				itemName = pItemTemplet->GetFullName();
//				
//				if( resultNum == 1)
//				{
//					wsprintf( buf, L"%s", itemName.c_str() );					
//				}
//				else
//				{
//					wsprintf( buf, L"%s %d개", itemName.c_str(), resultNum );
//				}
//				pStatic->GetString(0)->msg = buf;
//
//			}
//
//			if( false == pStatic->GetPicture(0)->SetTex( pItemTemplet->m_ShopImage.c_str() ) )
//			{
//				pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//
//		}
//	}
//
//	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
//}
//
//void CX2CharacterRoom::PlayGetAttractionItem() 
//{
//	if ( m_bPlayAttractionItem == false )
//	{
//		m_bPlayAttractionItem = true;
//		
//		m_pDLGGetAttractionItemBG = new CKTDGUIDialog( m_pNowState, L"DLG_Unit_State_RandomItem_BG.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGetAttractionItemBG );
//		m_pDLGGetAttractionItemBG->SetColor( D3DXCOLOR( 0,0,0,0) );
//		m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 0,0,0,0.7f), 0.5f, true );
//
//		D3DXVECTOR3 boxPos = m_pDLGOpenAttraction->GetDummyPos( 0 );
//
//		if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
//		{
//			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//		}
//
//		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachineStart = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"FireMachineStart", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
//		pMeshInst_MeltMachineStart->SetOverUI(true);
//		m_hMeshInstMeltMachineStart = pMeshInst_MeltMachineStart->GetHandle();
//
//		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Fire_Machine_Start.ogg", false, false );
//				
//
//// 		m_hSeqKeyTrace = g_pData->GetUIMajorParticle()->CreateSequenceHandle( L"KeyTrace", m_pMeshInstRandomBoxKey->GetPos() );
//// 		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqKeyTrace );
//// 		if( NULL != pSeq )
//// 		{
//// 			pSeq->SetOverUI( true );
//// 		}
//// 		else
//// 		{
//// 			m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
//// 		}
//		
//	}
//}
//
//bool CX2CharacterRoom::IsEnchantCoupon( int ItemID )
//{
//	switch(ItemID)
//	{
//	case 130147:
//	case 130148:
//	case 130149:
//	case 130150:
//	case 130151:
//	case 130152:
//		return true;
//	default:
//		break;
//	}
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID )
//{
//	// 일단 아이템 있는지 다시 확인해주고
//	CX2Item* pCouponItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( CouponItemUID );
//	CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( TargetItemUID );
//
//	// 사용가능한지도 확인한다
//	if ( pCouponItem == NULL || pTargetItem == NULL ||
//		pCouponItem->GetItemTemplet() == NULL )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"아이템 정보를 찾을 수 없습니다.", m_pNowState );
//		return false;
//	}
//	if( IsEnchantCouponUseable( pCouponItem->GetItemTemplet()->m_ItemID, pTargetItem->GetItemData().m_EnchantLevel ) )
//	{
//		KEGS_ATTACH_ITEM_REQ kEGS_ATTACH_ITEM_REQ;
//
//		kEGS_ATTACH_ITEM_REQ.m_iAttachItemUID = CouponItemUID;
//		kEGS_ATTACH_ITEM_REQ.m_iDestItemUID = TargetItemUID;
//
//		g_pData->GetServerProtocol()->SendPacket( EGS_ATTACH_ITEM_REQ, kEGS_ATTACH_ITEM_REQ );
//		g_pMain->AddServerPacket( EGS_ATTACH_ITEM_ACK );
//
//		return true;
//	}
//	else
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"강화권보다 낮은 단계의 아이템을 선택해야 합니다.", m_pNowState );
//	}
//	return false;
//}
//bool CX2CharacterRoom::Handler_EGS_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ATTACH_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	g_pMain->DeleteServerPacket( EGS_ATTACH_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"아이템 사용에 성공했습니다!", m_pNowState );
//
//			return true;
//		}		
//	}
//
//	return false;
//
//}
//
//bool CX2CharacterRoom::IsEnchantCouponUseable(int CouponItemID, int TargetEnchantLevel )
//{
//	int CouponLevel = 0;
//	switch(CouponItemID)
//	{
//	case 130147:	// 5
//		CouponLevel = 5;
//		break;
//	case 130148:	// 6
//		CouponLevel = 6;
//		break;
//	case 130149:	// 7
//		CouponLevel = 7;
//		break;
//	case 130150:	// 8
//		CouponLevel = 8;
//		break;
//	case 130151:	// 9
//		CouponLevel = 9;
//		break;
//	case 130152:	// 10
//		CouponLevel = 10;
//		break;
//	default:
//		return false;
//	}
//	if(CouponLevel > TargetEnchantLevel) return true;
//	return false;
//}
//#endif
