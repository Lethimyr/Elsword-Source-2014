#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI

#include ".\x2uiinventory.h"

CX2UIInventory::CX2UIInventory( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2UIDragable( pNowStage, NULL ),
//: CX2ItemSlotManager( pNowStage, pFileName )
m_NowInventorySortType(CX2Inventory::ST_EQUIP),
#ifndef	SERV_REFORM_INVENTORY_TEST
m_NowInvenSortTypePageNum(1),
m_NowInvenSortTypePageMaxNum(1),
#endif	SERV_REFORM_INVENTORY_TEST
m_MovedPosition(0,0),				// D3DXVECTOR2
m_pDLGUIInventory(NULL),
m_pDLGResolveItem(NULL),
m_pDLGResolveItemResult(NULL),
m_pDLGSocketItem(NULL),
m_pDLGSocketItemPushOk(NULL),
m_pDLGSocketItemCheat(NULL),
m_pDLGCubeCheck(NULL),
m_pDLGElChanger(NULL),
m_pDLGElChangerResult(NULL),
m_pDLGRandomItem(NULL),
m_eRandomItemEventType(RIOET_NONE),
m_iRandomItemUID(0),
m_RandomItemID(0),
m_bShow(false),
m_DLGPosition(0,0),					// D3DXVECTOR2
m_DLGSize(0,0),						// D3DXVECTOR2
m_ElChangerItemUID(0),
m_ElChangerItemCount(0),
m_DecompositionItemUID(0),
m_DecompositionWindowNowPageNum(0),
//m_mapResolveResultInsertedItem,
m_SocketItemUID(0),
m_SocketSlotIndex(-1),
m_bSocketRareMagicStone(false),
m_bSocketRemove(false)
#ifdef APRIL_5TH_TREE_DAY_EVENT			// 식목일 이벤트 관련 코드
,
m_pDLGTreeDay1(NULL),
m_pDLGTreeDay2(NULL),
m_bPlayingTreedayEvent(false),
m_bAprilSoundPlayed(true)
#endif
#ifdef ITEM_RECOVERY_TEST
,
m_RecoveryItemUid(0)
#endif
//{{100416 김정협 트레져 박스 추가
#ifdef 	SERV_TREASURE_BOX_ITEM
,
m_pDLGTreasureBox1(NULL),
m_pDLGTreasureBox2(NULL),
m_pDLGTreasureBox3(NULL),
m_pDLGTreasureBox4(NULL),
m_pDLGTreasureBox5(NULL),
m_pDLGTreasureBox6(NULL),
m_pDLGTreasureBox7(NULL),
m_pDLGTreasureBox8(NULL),
m_pDLGTreasureBox9(NULL),
m_pDLGTreasureBox_White(NULL),
m_bPlayingTreasureBox(false),
m_bTreasureBoxSoundPlayed(true)
#ifdef TREASURE_BOX_ITEM_THIN
,m_TreasureBoxSPItemID( 0 )
#endif TREASURE_BOX_ITEM_THIN
#endif 	SERV_TREASURE_BOX_ITEM
//}}100416 김정협 트레져 박스 추가
,m_WarpItem(0),
m_bIsNewItem(false),
m_UseItemTempUID( 0 )
#ifdef ATTRACTION_ITEM_TEST
,
//m_bOpenedAttractionItem(false),
m_bPlayAttractionItem(false),
m_pDLGOpenAttraction(NULL),
m_pDLGGetAttractionItemBG(NULL),
m_pDLGAttractionResult(NULL),
m_hMeshInstMeltMachine(INVALID_MESH_INSTANCE_HANDLE),
m_hMeshInstMeltMachineStart(INVALID_MESH_INSTANCE_HANDLE),
m_hSeqSpreadLight(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_hSeqEndLight(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_hSeqEndLight2(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_hSeqEndLight3(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_CouponTargetUid(0),
m_CouponUid(0),
m_fPlayTime(0)
#endif
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
,
m_bUsingRandomItemAutoOpen(false),
m_OpenedRandomItemCount(0),
m_pDLGAutoOpenMsgBox(NULL),
m_UsedItemUID(0),
m_TitleItemUID(0)
//m_MapAutoRandomItemGet.clear();
#endif
#ifdef RANDOM_CUBE_UI_NEW
, m_pDLGCubeOpenEffect(NULL)
, m_fCubeOpenTime(0)
, m_bCubeJustOpen(false)

#endif RANDOM_CUBE_UI_NEW
//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
,
m_pDLGItemEnroll( NULL ),
m_pDLGDeleteItemConfirm( NULL ),
m_iItemEnrollNum( 1 ),
m_EnrollItemUid( 0 )
#endif REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

//{{ kimhc // 2009-08-19 // 봉인의 인장 사용할 때 확인 창
#ifdef	SEAL_ITEM
, m_pDLGSealItemConfirm( NULL )
, m_pSlotItemToBeSealed( NULL )
, m_itemUIDForSeal( 0 )
, m_itemUIDToBeUnSealed( 0 )
//}} kimhc // 2009-08-19 // 봉인의 인장 사용할 때 확인 창
#endif	SEAL_ITEM
#ifdef FREE_WARP
, m_pDLGWarpDestination( NULL )
, m_iSelectedWarpDest(0)
, m_SumDelta(0)
#endif
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
, m_pDLGUSCJob( NULL )
, m_iSelectedUSCJob(0)
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
#ifdef SERV_SOCKET_NEW
// oasis907 : 김상윤 [2010.4.19] // 
, m_pUISocketItem(NULL)
#endif SERV_SOCKET_NEW
, m_pDLGIdentifyStone(NULL)
#ifdef SERV_PET_SYSTEM
, m_pPetFoodSlot( NULL )
, m_pUIPetInventory(NULL) // oasis907 : 김상윤 [2010.9.2] // 펫 인벤토리
, m_bAutoFeed(false)
#endif
#ifdef SERV_PSHOP_AGENCY
, m_pDLGCheckPShop( NULL )
#endif
#ifdef RANDOM_EXCHANGE_RESULT_VIEW
,m_eItemObtainResultType ( IORT_NONE )
#endif RANDOM_EXCHANGE_RESULT_VIEW
#ifdef NEW_ITEM_NOTICE
, m_pDLGItemHighLight(NULL)
#endif //NEW_ITEM_NOTICE
#ifdef FIX_RANDOM_CUBE_OPEN_RESULT_BUG
, m_bProcessCubeOpen(false)
#endif //FIX_RANDOM_CUBE_OPEN_RESULT_BUG
,m_iCurrentVillageWarpIndex(-1)
,m_iSelectedWarpDestIndex(-1)
,m_bIsEdConsumptionWarp(false)
#ifdef CUBE_OPEN_IMAGE_MANAGER
, m_bHideCubeOpneBar( false )
#endif // CUBE_OPEN_IMAGE_MANAGER
#ifdef SPT_NONE_ITEM_DELETE
,m_pSptNoneItemDelete(NULL)
,m_bIsSptNoneItem(false)
#endif //SPT_NONE_ITEM_DELETE
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
, m_iUseOpenRandomItemID(0)
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
#ifdef FIX_ICE_HEATER_EVENT
, m_pNowIceHeaterEventData( NULL )
, m_hUIEffectSetWait ( INVALID_EFFECTSET_HANDLE )
, m_hUIEffectSetPlay ( INVALID_EFFECTSET_HANDLE )
#endif FIX_ICE_HEATER_EVENT
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05//미라클큐브
, m_iConsumeItemUID ( 0 )
, m_iTargetItemUID ( 0 )
, m_iConsumeItemQuantity ( -1 )
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef RIDING_ITEM_POPUP_CHECK
, m_iRidingItemUid( 0 )
#endif //RIDING_ITEM_POPUP_CHECK
#ifdef FINALITY_SKILL_SYSTEM //JHKang
, m_pDLGElBreak( NULL )
, m_ElBreakItemCount( 0 )
#endif //FINALITY_SKILL_SYSTEM
#ifdef GOOD_ELSWORD //JHKang
, m_iConfirmED( 0 )
, m_pDLGBuyConfirm( NULL )
#endif //GOOD_ELSWORD
#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
, m_fLeftMouseWhellCoolTime (-1.f)
#endif // CAMERA_ZOOM_BY_MOUSE_WHEEL
#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
,m_UseTransformdItemUID( 0 )
,m_UseTransformItemPetUID( 0 )
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
#ifdef SERV_RECRUIT_EVENT_BASE
, m_pDLGRecommend( NULL )
#endif SERV_RECRUIT_EVENT_BASE
{
	// 초기화 ////////////////////////////////////////////////////////////////////////

	m_mapResolveResultInsertedItem.clear();

	//////////////////////////////////////////////////////////////////////////
	RegisterLuaBind();	

#ifdef SERV_PET_SYSTEM

	#ifdef	SERV_REFORM_INVENTORY_TEST
		m_pDLGUIInventory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Inventory_reform_Ver2.lua" );
	#else	SERV_REFORM_INVENTORY_TEST
		m_pDLGUIInventory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Inventory_Ver2.lua" );
	#endif SERV_REFORM_INVENTORY_TEST

#else
	m_pDLGUIInventory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_INVENTORY.lua" );
#endif
	m_pDLGUIInventory->SetShow(false);
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIInventory );
	m_pDLGUIInventory->SetDisableUnderWindow(true);

	CKTDGUIRadioButton* pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_Equip" );
	pEquipButton->SetChecked(true);

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		pItemSlot->LostItemUI();
		pItemSlot->SetShow( false );
		pItemSlot->SetEnable( false );
	}

	SetShow(false);

	m_pDLGUIInventory->SetShowEnable(false, false);

#ifdef NEW_ITEM_NOTICE

#ifdef SERV_REFORM_INVENTORY_TEST
	m_pDLGItemHighLight = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Inventory_HighLight_reform.lua" );
#else  SERV_REFORM_INVENTORY_TEST
	m_pDLGItemHighLight = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Inventory_HighLight.lua" );
#endif SERV_REFORM_INVENTORY_TEST
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemHighLight );
	m_pDLGItemHighLight->SetShow(false);
#endif //NEW_ITEM_NOTICE

#ifdef SERV_REFORM_INVENTORY_TEST
	for( int i = 0; i < MY_INVENTORY_MAX_SLOT_HEIGHT; ++i )
	{
		m_NowInventoryPageNum[i] = 1;
	}
#endif SERV_REFORM_INVENTORY_TEST

#ifdef FIX_ICE_HEATER_EVENT
	m_mapIceHeaterEventData.clear();

	/// 가열기  재생 정보 스크립트 파싱
	OpenScriptFile( L"IceHeaterEvent.lua" );
#endif // FIX_ICE_HEATER_EVENT
}


CX2UIInventory::~CX2UIInventory(void)
{
#ifdef SERV_RECRUIT_EVENT_BASE
	SAFE_DELETE_DIALOG(m_pDLGRecommend);
#endif SERV_RECRUIT_EVENT_BASE

	SAFE_DELETE_DIALOG( m_pDLGUIInventory );	

	SAFE_DELETE_DIALOG(m_pDLGRandomItem);
	SAFE_DELETE_DIALOG(m_pDLGCubeCheck);
	SAFE_DELETE_DIALOG(m_pDLGElChanger);

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	SAFE_DELETE_DIALOG( m_pDLGElBreak );
#endif //FINALITY_SKILL_SYSTEM
#ifdef GOOD_ELSWORD //JHKang
	SAFE_DELETE_DIALOG( m_pDLGBuyConfirm );
#endif //GOOD_ELSWORD

	SAFE_DELETE_DIALOG(m_pDLGResolveItem);
	SAFE_DELETE_DIALOG(m_pDLGResolveItemResult);

	SAFE_DELETE_DIALOG(m_pDLGSocketItem);
	SAFE_DELETE_DIALOG(m_pDLGSocketItemPushOk);
	SAFE_DELETE_DIALOG(m_pDLGSocketItemCheat);

	//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
#endif REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

#ifdef FREE_WARP
	SAFE_DELETE_DIALOG( m_pDLGWarpDestination );
#endif
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	SAFE_DELETE_DIALOG( m_pDLGUSCJob );
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
#ifdef ATTRACTION_ITEM_TEST
	SAFE_DELETE_DIALOG(m_pDLGOpenAttraction);
	SAFE_DELETE_DIALOG(m_pDLGGetAttractionItemBG);
	SAFE_DELETE_DIALOG(m_pDLGAttractionResult);

	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstMeltMachineStart )
	{
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
	}

	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstMeltMachine )
	{
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
	}

	if ( m_hSeqSpreadLight != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
	}

	if ( m_hSeqEndLight != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEndLight );
	}
#endif

	SAFE_DELETE_DIALOG(m_pDLGIdentifyStone);

#ifdef RANDOM_CUBE_UI_NEW
	SAFE_DELETE_DIALOG(m_pDLGCubeOpenEffect);
#endif RANDOM_CUBE_UI_NEW
#ifdef SERV_PSHOP_AGENCY
	SAFE_DELETE_DIALOG(m_pDLGCheckPShop);
#endif
#ifdef NEW_ITEM_NOTICE
	SAFE_DELETE_DIALOG(m_pDLGItemHighLight);
#endif //NEW_ITEM_NOTICE

#ifdef CUBE_OPEN_IMAGE_MANAGER
	SAFE_DELETE_DIALOG( m_pDLGEventCubeResultImage );
#endif // CUBE_OPEN_IMAGE_MANAGER

#ifdef SPT_NONE_ITEM_DELETE
	SAFE_DELETE_DIALOG( m_pSptNoneItemDelete );
#endif //SPT_NONE_ITEM_DELETE

#ifdef FIX_ICE_HEATER_EVENT
	
	/// 가열기 재생 정보값 모두 제거
	for ( map<RANDOM_ITEM_OPEN_EVENT_TYPE, IceHeaterEventData*>::iterator mIt = m_mapIceHeaterEventData.begin();
		  mIt != m_mapIceHeaterEventData.end(); )
	{
		IceHeaterEventData* pIceHeaterEventData = mIt->second;
		SAFE_DELETE( pIceHeaterEventData );
		mIt = m_mapIceHeaterEventData.erase( mIt );
	}

	if ( INVALID_EFFECTSET_HANDLE != m_hUIEffectSetWait )
		g_pData->GetUIEffectSet()->StopEffectSet( m_hUIEffectSetWait );

	if ( INVALID_EFFECTSET_HANDLE != m_hUIEffectSetPlay )
		g_pData->GetUIEffectSet()->StopEffectSet( m_hUIEffectSetPlay );

#endif // FIX_ICE_HEATER_EVENT
}



HRESULT CX2UIInventory::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
		
	POINT point;
	RECT rt;
		
	GetCursorPos( &point );	
	if( *m_pSlotBeforeDragging != NULL )
	{
 		if( g_pKTDXApp->GetHWND() != WindowFromPoint( point ) )
 		{
 			SetNowDragItemReturn();
 		}
 		else
		{
			ScreenToClient( g_pKTDXApp->GetHWND(), &point );
			GetClientRect( g_pKTDXApp->GetHWND(), &rt );
			if(	point.x <= rt.left || point.x >= rt.right || point.y <= rt.top || point.y >= rt.bottom )
			{
				SetNowDragItemReturn();
			}
		}
	}

#ifdef SERV_SOCKET_NEW
	// oasis907 : 김상윤 [2010.4.5] // 
	if ( m_pUISocketItem != NULL && m_pUISocketItem->GetShow() )
		m_pUISocketItem->OnFrameMove( fTime, fElapsedTime );
#endif SERV_SOCKET_NEW

#ifdef SERV_PET_SYSTEM
	// oasis907 : 김상윤 [2010.9.7] // 
	if ( m_pUIPetInventory != NULL && m_pUIPetInventory->GetShow() )
		m_pUIPetInventory->OnFrameMove( fTime, fElapsedTime );
#endif SERV_PET_SYSTEM


#ifdef RANDOM_CUBE_UI_NEW

	if(m_fCubeOpenTime > 0.f)
	{
#ifdef CUBE_OPEN_IMAGE_MANAGER
		ASSERT(g_pData->GetCubeOpenImageManager());
		if( g_pData->GetCubeOpenImageManager()->IsStart() == true )
			m_fCubeOpenTime -= (fElapsedTime);
		else
#endif // CUBE_OPEN_IMAGE_MANAGER
		m_fCubeOpenTime -= fElapsedTime;

		float fProgressOpen = 1.f;
		fProgressOpen -= (m_fCubeOpenTime / 3.f);

		if( m_pDLGCubeOpenEffect != NULL )
		{
			CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGCubeOpenEffect->GetControl(L"g_pStaticNote_Loading");
			CKTDGUIControl::CPictureData *pPicture = pStatic->GetPictureIndex(1);
#ifdef CUBE_OPEN_IMAGE_MANAGER
			pPicture->SetShow(!m_bHideCubeOpneBar);
#endif // CUBE_OPEN_IMAGE_MANAGER
			float fProgressRate = pPicture->GetOriginalSize().x * fProgressOpen;
			pPicture->SetSizeX( fProgressRate );
			
			//{{ oasis907 : 김상윤 [2011.3.29] 식목일 큐브 아이템
			if(m_pDLGCubeOpenEffect->GetDummyInt(0) == 1 && fProgressOpen > 0.5f)
			{	
				pStatic->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture51.tga", L"Note");
				m_pDLGCubeOpenEffect->ClearDummyInt();
			}
			//}}
#ifdef CUBE_OPEN_IMAGE_MANAGER
			ASSERT(g_pData->GetCubeOpenImageManager());
			if( g_pData->GetCubeOpenImageManager()->IsStart() == true)
			{
				if( g_pData->GetCubeOpenImageManager()->IsSoundPlaying() == true)
				{
					std::wstring wstrSoundName;
					if(g_pData->GetCubeOpenImageManager()->GetSoundName(m_RandomItemID, wstrSoundName) == true)
					{
						g_pKTDXApp->GetDeviceManager()->PlaySound( wstrSoundName.c_str(), false, false );
					}
					g_pData->GetCubeOpenImageManager()->SetSoundPlaying( false);
				}

				std::wstring wstrName;
				if(g_pData->GetCubeOpenImageManager()->GetCubeImageName(fProgressOpen, m_RandomItemID, wstrName) == true)
				{
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT 
					//sin 그래프 이용 
					if( g_pData->GetCubeOpenImageManager()->IsFadeInOutImage( m_RandomItemID ) == true )
					{
					double fAptime = 0.0;

					fAptime = sin(180 * 3.14 * (m_fCubeOpenTime*2/3 - 1 )  / 180 );
					if( m_fCubeOpenTime > 1.5 )
					{						
					}
					else
					{
						fAptime *= -1 ;
					}
					fAptime = min( fAptime * 2 , 1.0f );

					pStatic->GetPicture(0)->pPoint->color.a = (float)fAptime;
					}
#endif //RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
					pStatic->GetPicture(0)->SetTex( wstrName.c_str(), L"Note");
				}
			}
#endif // CUBE_OPEN_IMAGE_MANAGER
		}
	}
	else if(m_bCubeJustOpen)
	{
		SAFE_DELETE_DIALOG( m_pDLGCubeOpenEffect );
		m_pDLGCubeOpenEffect = NULL;
		m_bCubeJustOpen = false;
#ifdef EXCHANGE_OPEN_IMAGE
		if( m_eItemObtainResultType == IORT_RANDOM_EXCHANGE &&
			g_pData->GetUIManager()->GetUIItemExchangeShop() != NULL )
		{
			g_pData->GetUIManager()->GetUIItemExchangeShop()->Handler_EGS_ITEM_EXCHANGE_REQ();
		}
		else if( m_eItemObtainResultType == IORT_CUBE_OPEN )
#endif EXCHANGE_OPEN_IMAGE
		Handler_EGS_OPEN_RANDOM_ITEM_REQ();
	}
#endif RANDOM_CUBE_UI_NEW



#ifdef APRIL_5TH_TREE_DAY_EVENT
	if( m_bPlayingTreedayEvent )
	{
		m_fPlayTime += fElapsedTime;

		if(m_bAprilSoundPlayed == false && m_fPlayTime > 0.1f )
		{
			m_bAprilSoundPlayed = true;
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"TreeDay_Event.ogg", false, false );
		}

		if ( (m_pDLGTreeDay1 != NULL && m_pDLGTreeDay1->GetIsMoving() == false) 
			&&	( m_pDLGTreeDay2 != NULL && m_pDLGTreeDay2->GetIsMoving() == false) )
		{
			SAFE_DELETE_DIALOG(m_pDLGTreeDay1);
			SAFE_DELETE_DIALOG(m_pDLGTreeDay2);
			m_bPlayingTreedayEvent = false;

			m_fPlayTime = 0.0f;
			UpdateOpenTreeDayItemAck();
		}
	}
#endif

#ifdef CUBE_OPEN_IMAGE_MANAGER
	if( g_pData->GetCubeOpenImageManager()->IsPlaying() == true)
	{
		if( g_pData->GetCubeOpenImageManager()->IsNowResultEventCubePlaying() == true)
		{
			if(m_pDLGEventCubeResultImage != NULL && m_pDLGEventCubeResultImage->GetIsMoving() == false)
			{
				SAFE_DELETE_DIALOG(m_pDLGEventCubeResultImage);
				g_pData->GetCubeOpenImageManager()->SetPlaying( false);
				g_pData->GetCubeOpenImageManager()->SetStart( false);
				g_pData->GetCubeOpenImageManager()->SetNowResultEventCubePlaying(false);
				m_fPlayTime = 0.0f;
				UpdateOpenRandomItemAckNew();
			}
		}
		else
		{
			g_pData->GetCubeOpenImageManager()->SetPlaying( false);
			g_pData->GetCubeOpenImageManager()->SetStart( false);
			m_fPlayTime = 0.0f;
			UpdateOpenRandomItemAckNew();
		}
	}
#endif // CUBE_OPEN_IMAGE_MANAGER
#ifdef 	SERV_TREASURE_BOX_ITEM
	if( m_bPlayingTreasureBox )
	{
		m_fPlayTime += fElapsedTime;

		if(m_bTreasureBoxSoundPlayed == false && m_fPlayTime > 0.1f )
		{
			m_bTreasureBoxSoundPlayed = true;
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"TreasureBox.ogg", false, false );
		}

		if ( (m_pDLGTreasureBox1 != NULL && m_pDLGTreasureBox1->GetIsMoving() == false) 
			&&	( m_pDLGTreasureBox2 != NULL && m_pDLGTreasureBox2->GetIsMoving() == false) 
			&&	( m_pDLGTreasureBox3 != NULL && m_pDLGTreasureBox3->GetIsMoving() == false) 
			&&	( m_pDLGTreasureBox4 != NULL && m_pDLGTreasureBox4->GetIsMoving() == false) 
			&&	( m_pDLGTreasureBox5 != NULL && m_pDLGTreasureBox5->GetIsMoving() == false) 
			&&	( m_pDLGTreasureBox6 != NULL && m_pDLGTreasureBox6->GetIsMoving() == false) 
			&&	( m_pDLGTreasureBox7 != NULL && m_pDLGTreasureBox7->GetIsMoving() == false) 
			&&	( m_pDLGTreasureBox8 != NULL && m_pDLGTreasureBox8->GetIsMoving() == false) 
			&&	( m_pDLGTreasureBox9 != NULL && m_pDLGTreasureBox9->GetIsMoving() == false)
			&&	( m_pDLGTreasureBox_White != NULL && m_pDLGTreasureBox_White->GetIsMoving() == false))
		{
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox1);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox2);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox3);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox4);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox5);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox6);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox7);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox8);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox9);
			SAFE_DELETE_DIALOG(m_pDLGTreasureBox_White);
			SAFE_DELETE_DIALOG(m_pDLGGetAttractionItemBG);
			m_bPlayingTreasureBox = false;

			m_fPlayTime = 0.0f;
			UpdateOpenTreasureBoxItemAck();
		}
	}
#endif 	SERV_TREASURE_BOX_ITEM

#ifdef ATTRACTION_ITEM_TEST

#ifdef FIX_ICE_HEATER_EVENT		/// 기존의 가열기 재생 동작 간략화
	if ( NULL == g_pData ||
		 NULL == g_pData->GetUIEffectSet() ||
		 NULL == g_pData->GetUIMajorXMeshPlayer() )	/// 기본 널처리
		 return false;

	 /// 가열기 가동 전 동작 처리
	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstMeltMachine && 
		 true == g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachine ) )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMeltMachine );
		if(NULL != pMeshInst)
		{			
			if( pMeshInst->GetXSkinAnim() != NULL )
				pMeshInst->GetXSkinAnim()->GetRenderParam()->fOutLineWide = 1.2f;
			
			/// 해당 가열기의 가동 전 이펙트 설정
			if ( NULL == g_pData->GetUIEffectSet()->GetEffectSetInstance( m_hUIEffectSetWait ) )
			{
				const wstring wstrWaitUIEffectSetName = 
					NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrWaitUIEffectSetName : L"EffectSet_Ice_Heater_Wait";

				m_hUIEffectSetWait	= g_pData->GetUIEffectSet()->PlayEffectSetByMeshPlayer( wstrWaitUIEffectSetName.c_str(), pMeshInst->GetHandle(), CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR );
			}
		}
	}
	else
	{
		/// 해당 가열기의 가동 전 이펙트 해제
		if ( INVALID_EFFECTSET_HANDLE != m_hUIEffectSetWait )
			g_pData->GetUIEffectSet()->StopEffectSet( m_hUIEffectSetWait );
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_Start = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMeltMachineStart );
	if( NULL != pMeshInst_Start && pMeshInst_Start->GetXSkinAnim() != NULL )
	{
		pMeshInst_Start->GetXSkinAnim()->GetRenderParam()->fOutLineWide = 1.2f;
	}

	//////////////////////////////////////////////////////////////////////////
	
	/// 가열기 동작 처리
	if ( m_bPlayAttractionItem == true )
	{
		m_fPlayTime += fElapsedTime;

		/// 가열기 동작 전 매시, 이펙트가 남아있다면, 해제
		if( g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == false )
		{
			m_hMeshInstMeltMachineStart = INVALID_MESH_INSTANCE_HANDLE;
			g_pData->GetUIEffectSet()->StopEffectSet( m_hUIEffectSetWait );
		}
		else if( NULL == g_pData->GetUIEffectSet()->GetEffectSetInstance( m_hUIEffectSetWait ) )
		{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            m_hUIEffectSetWait = INVALID_EFFECTSET_HANDLE;
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
			m_hUIEffectSetWait = INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		}

		/// 가열기 재생 이펙트 설정
		if( NULL != pMeshInst_Start &&
			NULL != pMeshInst_Start->GetXSkinAnim() &&
			L"Start" == pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() && 
			true == pMeshInst_Start->EventTimerGlobal( 0.1f ) )
		{
			const wstring wstrPalyEffectSet = NULL != 
				m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrPlayUIEffectSetName : L"FireMachineStart";

			m_hUIEffectSetPlay = g_pData->GetUIEffectSet()->PlayEffectSetByMeshPlayer(  wstrPalyEffectSet.c_str(), pMeshInst_Start->GetHandle(), CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR );
		}

		/// 가동 시간 종료시 각종 객체 초기화 및 해제
		const float fEndPlayTime = NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_fPlayTime : 10.f;

		if( m_fPlayTime >= fEndPlayTime )
		{
			m_bPlayAttractionItem = false;
			m_fPlayTime = 0.0f;

			if ( m_pDLGGetAttractionItemBG != NULL )
			{
				m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2( 0,0 ), D3DXCOLOR( 0,0,0,0 ), 0.5f, true, true );
				m_pDLGGetAttractionItemBG = NULL;
			}

			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
			if( NULL != m_pDLGOpenAttraction )
			{
				m_pDLGOpenAttraction->SetHasUnit(NULL);
			}

			g_pData->GetUIEffectSet()->StopEffectSet( m_hUIEffectSetPlay );

			//{{ 2012. 10. 08  할로윈 가열기
#ifdef SERV_HALLOWEEN_ICE_HEATER
			if( m_eRandomItemEventType == RIOET_HALLOWEEN_ICE_HEATER )
			{
				UpdateOpenHalloWeenItemAck();
			}
			else
			{
				UpdateOpenAttractionItemAck();
			}
#else //SERV_HALLOWEEN_ICE_HEATER
			UpdateOpenAttractionItemAck();
#endif //SERV_HALLOWEEN_ICE_HEATER			
		}
		
	}

#else // FIX_ICE_HEATER_EVENT

	/// 가열기 가동 전 대기 상태 처리
	if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE && 
		g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachine ) == true )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMeltMachine );
		if(NULL != pMeshInst)
		{			
			if( pMeshInst->GetXSkinAnim() != NULL )
			{
				pMeshInst->GetXSkinAnim()->GetRenderParam()->fOutLineWide = 1.2f;
			}
	#ifdef RIDING_SYSTEM
			if( false == m_bIsRidingGacha )
	#endif //RIDING_SYSTEM
			if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqSpreadLight ) == false )
			{			
				D3DXVECTOR3 pos = pMeshInst->GetXSkinAnim()->GetCloneFramePosition( L"Object05" );
				// 좌표계 변환. 2D_PLAIN은 3D와 y축 방향이 다르고, Mesh의 좌표는 화면 중앙을 기준으로 맞추어져 있다.
				pos = D3DXVECTOR3(512, 384, 0) - pos;
				CKTDGParticleSystem::CParticleEventSequence* pTemp = NULL;

	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				if( RIOET_ALL_IN_ONE_ICE_HEATER == m_eRandomItemEventType )
					 pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"Light_Fire_machine01_DarkEL", pos );
				else
					pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"Light_Fire_machine01", pos );
	#else
				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"Light_Fire_machine01", pos );
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

				pTemp->SetOverUI(true);
				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
				m_hSeqSpreadLight = pTemp->GetHandle();
			}

		}

	}
	else
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
		m_hSeqSpreadLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}



	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_Start = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMeltMachineStart );
	if( NULL != pMeshInst_Start )
	{
		if( pMeshInst_Start->GetXSkinAnim() != NULL )
		{
			pMeshInst_Start->GetXSkinAnim()->GetRenderParam()->fOutLineWide = 1.2f;
		}
	}

	if( NULL != pMeshInst_Start &&
		pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"End" )
	{
		if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqEndLight ) == false )
		{
			D3DXVECTOR3 pos = pMeshInst_Start->GetXSkinAnim()->GetCloneFramePosition( L"Sphere01" );
			// 좌표계 변환. 2D_PLAIN은 3D와 y축 방향이 다르고, Mesh의 좌표는 화면 중앙을 기준으로 맞추어져 있다.
			pos = D3DXVECTOR3(512, 384, 0) - pos;
			pos.y += 22;

			CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"Light_Fire_machine_End02", pos );
			pTemp->SetOverUI(true);
			pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
			m_hSeqEndLight = pTemp->GetHandle();
		}
	}
	else
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEndLight );
		m_hSeqEndLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEndLight2 );
		m_hSeqEndLight2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEndLight3 );
		m_hSeqEndLight3 = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}



	if ( m_bPlayAttractionItem == true )
	{
		m_fPlayTime += fElapsedTime;

		if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE )
		{
			if( g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == false )
			{
				m_hMeshInstMeltMachineStart = INVALID_MESH_INSTANCE_HANDLE;
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
			}
			else if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqSpreadLight ) == false )
			{
				m_hSeqSpreadLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}


#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
		float fFireMachineParticleTime[AITI_END];

	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
		wstring fFireMachineParticleName[AIPNI_END];
	#endif // SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

		switch( m_eRandomItemEventType )
		{
		case RIOET_MINI_ICE_HEATER:		/// 미니 조각상 가열기는 훨씬 빨리 돈다.
			{
				fFireMachineParticleTime[AITI_START_PARTICLE_1]		= 0.7f;
				fFireMachineParticleTime[AITI_START_PARTICLE_2]		= 0.75f;
				fFireMachineParticleTime[AITI_START_PARTICLE_3]		= 2.7f;
				fFireMachineParticleTime[AITI_END_PARTICLE]			= 3.08f;
				fFireMachineParticleTime[AITI_END_TIME]				= 3.78f;

	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				fFireMachineParticleName[AIPNI_START_PARTICLE_1]	= L"LightParticle_Fire_Machine_Start05";
				fFireMachineParticleName[AIPNI_START_PARTICLE_2]	= L"LightParticle_Fire_Machine_Start04";
				fFireMachineParticleName[AIPNI_START_PARTICLE_3]	= L"LightParticle_Fire_Machine_Start03";
				fFireMachineParticleName[AIPNI_START_PARTICLE_4]	= L"Light_Fire_Machine_Start02";
				fFireMachineParticleName[AIPNI_START_PARTICLE_5]	= L"Light_Fire_Machine_Start01";
				fFireMachineParticleName[AIPNI_END_PARTICLE_1]		= L"LightParticle_Fire_Machine_End01";
				fFireMachineParticleName[AIPNI_END_PARTICLE_2]		= L"Light_Fire_machine_End02";
	#endif // SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

			} break;

#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
		case RIOET_ALL_IN_ONE_ICE_HEATER:	/// 올인원 가열기
			{
				fFireMachineParticleTime[AITI_START_PARTICLE_1]		= 3.f;
				fFireMachineParticleTime[AITI_START_PARTICLE_2]		= 3.33f;
				fFireMachineParticleTime[AITI_START_PARTICLE_3]		= 5.53f;
				fFireMachineParticleTime[AITI_END_PARTICLE]			= 5.79f + 0.12f;
				fFireMachineParticleTime[AITI_END_TIME]				= 8.5f + 1.5f;

				fFireMachineParticleName[AIPNI_START_PARTICLE_1]	= L"LightParticle_Fire_Machine_Start05";
				fFireMachineParticleName[AIPNI_START_PARTICLE_2]	= L"LightParticle_Fire_Machine_Start04";
				fFireMachineParticleName[AIPNI_START_PARTICLE_3]	= L"LightParticle_Fire_Machine_Start03";
				fFireMachineParticleName[AIPNI_START_PARTICLE_4]	= L"Light_Fire_Machine_Start02_DarkEL";
				fFireMachineParticleName[AIPNI_START_PARTICLE_5]	= L"Light_Fire_Machine_Start01_DarkEL";
				fFireMachineParticleName[AIPNI_END_PARTICLE_1]		= L"LightParticle_Fire_Machine_End01";
				fFireMachineParticleName[AIPNI_END_PARTICLE_2]		= L"Light_Fire_machine_End02_DarkEL";
			} break;
#endif // SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

#ifdef RIDING_SYSTEM
		case RIOET_RIDING_PET_GACHA:	/// 라이딩 펫 가챠 ( 고대 화석 판별기 )
			{
				fFireMachineParticleTime[AITI_START_PARTICLE_1]		= 1.87f;
				fFireMachineParticleTime[AITI_START_PARTICLE_2]		= 1.92f;
				fFireMachineParticleTime[AITI_START_PARTICLE_3]		= 3.87f;
				fFireMachineParticleTime[AITI_END_PARTICLE]			= 4.25f;
				fFireMachineParticleTime[AITI_END_TIME]				= 4.95f;

				fFireMachineParticleName[AIPNI_START_PARTICLE_1]	= L"LightParticle_Fire_Machine_Start05";
				fFireMachineParticleName[AIPNI_START_PARTICLE_2]	= L"LightParticle_Fire_Machine_Start04";
				fFireMachineParticleName[AIPNI_START_PARTICLE_3]	= L"LightParticle_Fire_Machine_Start03";
				fFireMachineParticleName[AIPNI_START_PARTICLE_4]	= L"Light_Fire_Machine_Start02_DarkEL";
				fFireMachineParticleName[AIPNI_START_PARTICLE_5]	= L"Light_Fire_Machine_Start01_DarkEL";
				fFireMachineParticleName[AIPNI_END_PARTICLE_1]		= L"LightParticle_Fire_Machine_End01";
				fFireMachineParticleName[AIPNI_END_PARTICLE_2]		= L"Light_Fire_machine_End02_DarkEL";
			} break;
#endif //RIDING_SYSTEM

		default:
			{
				fFireMachineParticleTime[AITI_START_PARTICLE_1]		= 3.f;
				fFireMachineParticleTime[AITI_START_PARTICLE_2]		= 3.33f;
				fFireMachineParticleTime[AITI_START_PARTICLE_3]		= 5.53f;
				fFireMachineParticleTime[AITI_END_PARTICLE]			= 5.79f + 0.12f;
				fFireMachineParticleTime[AITI_END_TIME]				= 8.5f + 1.5f;

	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				fFireMachineParticleName[AIPNI_START_PARTICLE_1]	= L"LightParticle_Fire_Machine_Start05";
				fFireMachineParticleName[AIPNI_START_PARTICLE_2]	= L"LightParticle_Fire_Machine_Start04";
				fFireMachineParticleName[AIPNI_START_PARTICLE_3]	= L"LightParticle_Fire_Machine_Start03";
				fFireMachineParticleName[AIPNI_START_PARTICLE_4]	= L"Light_Fire_Machine_Start02";
				fFireMachineParticleName[AIPNI_START_PARTICLE_5]	= L"Light_Fire_Machine_Start01";
				fFireMachineParticleName[AIPNI_END_PARTICLE_1]		= L"LightParticle_Fire_Machine_End01";
				fFireMachineParticleName[AIPNI_END_PARTICLE_2]		= L"Light_Fire_machine_End02";
	#endif // SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
			} break;
		}
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
		// 파티클 하드코딩.. ㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠ
		if( pMeshInst_Start != NULL )
		{
#ifdef RIDING_SYSTEM
			if( m_eRandomItemEventType == RIOET_RIDING_PET_GACHA)
			{
				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 0.6f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P02", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.9f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P02", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.76f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P01", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.9f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P03", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 1.33f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P04", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1.5, 1.5, 1.5) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 0.76f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P05", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.93f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P06", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.9f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P07", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 2.13f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P08", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.93f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P09", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 1.66f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P11", 512,400,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.93f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P12", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 0.6f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P07", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1, 1, 1) );
				}

				if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.33f ) == true )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Wind_Machine_FX_P14", 512,362,0.75 );
					pTemp->SetOverUI( true );
					pTemp->SetScaleFactor( D3DXVECTOR3(1.5, 1.5, 1.5) );
				}
			}
			else
			{
#endif //RIDING_SYSTEM

#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( fFireMachineParticleTime[AITI_START_PARTICLE_1] ) == true )
#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.0f ) == true )
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
			{
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, fFireMachineParticleName[AIPNI_START_PARTICLE_1].c_str(), 512,384+40,0.75 );
	#else  // SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LightParticle_Fire_Machine_Start05", 512,384+40,0.75 );
	#endif // SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp->SetOverUI( true );
				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
			}
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( fFireMachineParticleTime[AITI_START_PARTICLE_2] ) == true )
#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.33f ) == true )
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
			{
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  fFireMachineParticleName[AIPNI_START_PARTICLE_2].c_str(), 512,384-40,0.75 );
	#else  //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LightParticle_Fire_Machine_Start04", 512,384-40,0.75 );
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp->SetOverUI( true );
				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
			}
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( fFireMachineParticleTime[AITI_START_PARTICLE_3] ) == true )
#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 5.53f ) == true )
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
			{ 
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  fFireMachineParticleName[AIPNI_START_PARTICLE_3].c_str(), 512,304,0.75 );
	#else  //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LightParticle_Fire_Machine_Start03", 512,304,0.75 );
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp->SetOverUI( true );
				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, fFireMachineParticleName[AIPNI_START_PARTICLE_4].c_str(), 512,267,0.75);
	#else  //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"Light_Fire_Machine_Start02", 512,267,0.75);
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp->SetOverUI( true );
				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  fFireMachineParticleName[AIPNI_START_PARTICLE_5].c_str(), 512,267,0.75 );
	#else  //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"Light_Fire_Machine_Start01", 512,267,0.75 );
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp->SetOverUI( true );
				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
			}
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"End" && pMeshInst_Start->EventTimerGlobal( fFireMachineParticleTime[AITI_END_PARTICLE] ) == true )
#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"End" && pMeshInst_Start->EventTimerGlobal( 5.79f+0.12f ) == true )
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
			{
				D3DXVECTOR3 pos = pMeshInst_Start->GetXSkinAnim()->GetCloneFramePosition( L"Sphere01" );
				// 좌표계 변환. 2D_PLAIN은 3D와 y축 방향이 다르고, Mesh의 좌표는 화면 중앙을 기준으로 맞추어져 있다.
				pos = D3DXVECTOR3(512, 384, 0) - pos;
				pos.y += 22;
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  fFireMachineParticleName[AIPNI_END_PARTICLE_1].c_str(), 512,267,0.75 );
	#else  //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LightParticle_Fire_Machine_End01", 512,267,0.75 );
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp->SetOverUI( true );
				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
				m_hSeqEndLight2 = pTemp->GetHandle();
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, fFireMachineParticleName[AIPNI_END_PARTICLE_2].c_str(), 512,267,0.75 );
	#else  //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"Light_Fire_machine_End02", 512,267,0.75 );
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				pTemp->SetOverUI( true );
				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
				m_hSeqEndLight3 = pTemp->GetHandle();
			}
#ifdef RIDING_SYSTEM
			}
#endif //RIDING_SYSTEM
		}

#ifdef RIDING_SYSTEM
		float fEndTime;
		if( m_eRandomItemEventType == RIOET_RIDING_PET_GACHA)
		{
			fEndTime = 5.2f;
		}
		else
		{
			fEndTime = fFireMachineParticleTime[AITI_END_TIME];
		}

		if( m_fPlayTime >= fEndTime )
#else
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
		if ( m_fPlayTime >= fFireMachineParticleTime[AITI_END_TIME] )
#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
		const float fAttractionItemPlayTime = 8.5f;
		if ( m_fPlayTime >= fAttractionItemPlayTime + 1.5f)
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
#endif //RIDING_SYSTEM
		{
			m_bPlayAttractionItem = false;
			m_fPlayTime = 0.0f;

			if ( m_pDLGGetAttractionItemBG != NULL )
			{
				m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2( 0,0 ), D3DXCOLOR( 0,0,0,0 ), 0.5f, true, true );
				m_pDLGGetAttractionItemBG = NULL;
			}

			if ( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE )
			{
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
				if( NULL != m_pDLGOpenAttraction )
				{
					m_pDLGOpenAttraction->SetHasUnit(NULL);
				}
			}

#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
			if ( m_iUseOpenRandomItemID == ONE_THIRD_MINI_ATTRACTION_ITEM  )
			{
				UpdateOpenMiniRandomItemAckNew();
			}
			else
			{
				UpdateOpenAttractionItemAck();		
			}
#else //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
			UpdateOpenAttractionItemAck();
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

		}
	}
#endif // FIX_ICE_HEATER_EVENT

#endif ATTRACTION_ITEM_TEST

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	if( m_fLeftMouseWhellCoolTime > 0.f )
	{
		m_fLeftMouseWhellCoolTime -= fElapsedTime;
	}
#endif // CAMERA_ZOOM_BY_MOUSE_WHEEL

	// 현재 마우스의 위치가 자신의 UI에 있을경우에만 처리한다.
	// 가열기는 무조건 돌도록 가열기 코드 아래로
	if(m_pDLGUIInventory != NULL && m_pDLGUIInventory->GetIsMouseOver() == true)
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

bool CX2UIInventory::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGUIInventory ) )
	{
#ifdef FREE_WARP
		switch( uMsg )
		{
		case WM_MOUSEWHEEL:
			{ 
				if( m_pDLGWarpDestination != NULL && m_pDLGWarpDestination->GetIsMouseOver() ) 
				{
					CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGWarpDestination->GetControl( L"WarpDestControlList" );
					if( pControlList == NULL )
						return false;
					CKTDGUIScrollBar* pScroolBarY = pControlList->GetYScroolBar();
					if(pScroolBarY == NULL)
						return false;

					short zDelta = (short)HIWORD(wParam);
					m_SumDelta += zDelta;

					while (abs(m_SumDelta) >= WHEEL_DELTA)
					{
						if(m_SumDelta>0)
						{
							pScroolBarY->Scroll(-1);
							m_SumDelta -= WHEEL_DELTA;
						}
						else
						{	
							pScroolBarY->Scroll(1);
							m_SumDelta += WHEEL_DELTA;
						}
					}
					return true;
				}
			}break;
		}
#endif
		return false;
	}

#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	/// 인벤토리 확장 패치 이후 우편 보낼 때, 경고 안내창이 인벤토리를 가리거나, 인벤토리에 의해 가려지는 문제가 있어서
	/// 경고 안내창이 열려있을 때, 인벤토리 다이얼로그를 클릭하면 경고 안내창 사라질 수 있도록 추가
	switch( uMsg )
	{
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		{
			if( NULL != g_pData && 
				NULL != g_pData->GetUIManager() && 
				NULL != g_pData->GetUIManager()->GetUIPostBoxWarning() &&
				true == g_pData->GetUIManager()->GetUIPostBoxWarning()->GetShow() )
			{
				g_pData->GetUIManager()->GetUIPostBoxWarning()->SetShow(false);
			}
		}
		break;
	}
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST

#ifdef SERV_PET_SYSTEM
	// oasis907 : 김상윤 [2010.9.7] // 
	if(m_pUIPetInventory != NULL && m_pUIPetInventory->GetShow() == true)
	{
		if ( m_pUIPetInventory->MsgProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}
#endif SERV_PET_SYSTEM

#ifdef SERV_SOCKET_NEW 
	// oasis907 : 김상윤 [2010.4.5] // 
	if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true)
	{
		if ( m_pUISocketItem->MsgProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}
#endif SERV_SOCKET_NEW


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
			if ( false == m_pDLGUIInventory->GetIsMouseOver())
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
			if ( false == m_pDLGUIInventory->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_INVEN, true);
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

			MouseUp( mousePos );

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

	case WM_MOUSEMOVE:
		{

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
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
#ifdef ATTRACTION_ITEM_TEST
			m_CouponUid = 0;
			m_CouponTargetUid = 0;
#endif
			MouseRButtonUp( mousePos );

		}
		break;
#ifdef CHANGE_INVENTORY_TAB_BY_WHEEL
	case WM_MOUSEWHEEL:
		{ 
			return OnMouseWheel(hWnd, uMsg, wParam,lParam);
		} break;
#endif // CHANGE_INVENTORY_TAB_BY_WHEEL

	}

	return bFlag;
}


bool CX2UIInventory::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef SERV_SOCKET_NEW 
	// oasis907 : 김상윤 [2010.4.5] // 
	if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true)
	{
		if ( m_pUISocketItem->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}
#endif SERV_SOCKET_NEW

#ifdef SERV_PET_SYSTEM
	if(m_pUIPetInventory != NULL && m_pUIPetInventory->GetShow() == true)
	{
		if ( m_pUIPetInventory->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}
#endif SERV_PET_SYSTEM

	switch(wParam)
	{
#ifdef NEW_SKILL_TREE
	case UIM_SKILL_INIT_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			if( g_pData->GetSkillTree() != NULL )
			{
				g_pData->GetSkillTree()->Handler_EGS_INIT_SKILL_TREE_REQ(m_UsedItemUID);
			}
			return true;
		} break;
#endif NEW_SKILL_TREE
	case UIM_CHANGE_NICK_NAME_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_UseItemTempUID );
			m_UseItemTempUID = 0;

			// 닉네임 변경에 성공하면 EGS_DELETE_NICK_NAME_SUCCESS_NOT를 받고 캐릭터 선택창으로 이동한다
			return true;
		} break;
	case UIM_ITEM_USE_TITLE:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_TitleItemUID );
			m_TitleItemUID = 0;
		}
		break;

	case UIM_ITEM_USE_SKILL_UNSEAL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL && NULL != pControl->GetDialog() )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_UsedItemUID );
			m_UsedItemUID = 0;
		} break;


#ifdef AUTH_DELETE_ITEM
	case UIM_DELETE_ITEM:
		{
			//{{ kimhc // 실시간 엘소드 중 실시간 아이템 버리기
#ifdef	REAL_TIME_ELSWORD
			CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
			CX2Cursor*	pCursor = pState->GetCursor();
			pCursor->ChangeCursorState( CX2Cursor::XCS_DELETE_ITEM );

			return true;
#else	REAL_TIME_ELSWORD
			if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel() )
			{
				CX2State* pState = (CX2State*)g_pMain->GetNowState();
				CX2Cursor* pCursor = pState->GetCursor();
				pCursor->ChangeCursorState( CX2Cursor::XCS_DELETE_ITEM );
			}
#endif	REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 실시간 아이템 버리기
		} break;
#endif
#ifdef AUTH_AUTO_SELL_ITEM
	case UIM_SELL_CATEGORY_ALL:
		{
			if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel()
#ifdef CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
				|| CX2User::XUAL_OPERATOR == g_pData->GetMyUser()->GetAuthLevel()
#endif //CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
				)
			{
				if(g_pData->GetUIManager()->GetUIShop() == NULL)
				{
					g_pData->GetUIManager()->CreateUIShop();
				}
				g_pData->GetUIManager()->GetUIShop()->AuthAutoSellItem( m_NowInventorySortType );
			}
		} break;
#endif
#ifdef RANDOM_CUBE_UI_NEW
	case UIM_EXIT_RANDOM_ITEM_NEW:
		{
			SAFE_DELETE_DIALOG(m_pDLGCubeOpenEffect);
			m_fCubeOpenTime = 0.f;
			m_bCubeJustOpen = false;
#ifdef RANDOM_EXCHANGE_RESULT_VIEW
			m_eItemObtainResultType = IORT_NONE;
#else
			m_bCubeOpen = false;
#endif RANDOM_EXCHANGE_RESULT_VIEW
			return true;
		}
#endif RANDOM_CUBE_UI_NEW
	case UIM_EXIT:
		{
#ifdef CUBE_OPEN_IMAGE_MANAGER
			if( g_pData->GetCubeOpenImageManager() != NULL && g_pData->GetCubeOpenImageManager()->IsNowResultEventCubePlaying() == true)
				return true;
#endif // CUBE_OPEN_IMAGE_MANAGER

			SetShow(false);
			return true;
		}
	case UIM_EQUIP:
		{
#ifdef	SERV_REFORM_INVENTORY_TEST
			UpdatePageUI( GetNowInventoryPageNum( CX2Inventory::ST_EQUIP ) );
			return SetInventorySort(CX2Inventory::ST_EQUIP, GetNowInventoryPageNum( CX2Inventory::ST_EQUIP ) );
#else	SERV_REFORM_INVENTORY_TEST
			return SetInventorySort(CX2Inventory::ST_EQUIP);
#endif SERV_REFORM_INVENTORY_TEST
		}
	case UIM_ACCESSORY:
		{
#ifdef	SERV_REFORM_INVENTORY_TEST
			UpdatePageUI( GetNowInventoryPageNum( CX2Inventory::ST_ACCESSORY ) );
			return SetInventorySort(CX2Inventory::ST_ACCESSORY, GetNowInventoryPageNum( CX2Inventory::ST_ACCESSORY ) );
#else	SERV_REFORM_INVENTORY_TEST
			return SetInventorySort(CX2Inventory::ST_ACCESSORY);
#endif SERV_REFORM_INVENTORY_TEST
		}
	case UIM_QUICK_SLOT:
		{
#ifdef	SERV_REFORM_INVENTORY_TEST
			UpdatePageUI( GetNowInventoryPageNum( CX2Inventory::ST_QUICK_SLOT ) );
			return SetInventorySort(CX2Inventory::ST_QUICK_SLOT, GetNowInventoryPageNum( CX2Inventory::ST_QUICK_SLOT ) );
#else	SERV_REFORM_INVENTORY_TEST
			return SetInventorySort(CX2Inventory::ST_QUICK_SLOT);
#endif SERV_REFORM_INVENTORY_TEST
		}
	case UIM_MATERIAL:
		{
#ifdef	SERV_REFORM_INVENTORY_TEST
			UpdatePageUI( GetNowInventoryPageNum( CX2Inventory::ST_MATERIAL ) );
			return SetInventorySort(CX2Inventory::ST_MATERIAL, GetNowInventoryPageNum( CX2Inventory::ST_MATERIAL ) );
#else	SERV_REFORM_INVENTORY_TEST
			return SetInventorySort(CX2Inventory::ST_MATERIAL);
#endif SERV_REFORM_INVENTORY_TEST
		}
	case UIM_QUEST:
		{
#ifdef	SERV_REFORM_INVENTORY_TEST
			UpdatePageUI( GetNowInventoryPageNum( CX2Inventory::ST_QUEST ) );
			return SetInventorySort(CX2Inventory::ST_QUEST, GetNowInventoryPageNum( CX2Inventory::ST_QUEST ) );
#else	SERV_REFORM_INVENTORY_TEST
			return SetInventorySort(CX2Inventory::ST_QUEST);
#endif SERV_REFORM_INVENTORY_TEST
		}
	case UIM_SPECIAL:
		{
#ifdef	SERV_REFORM_INVENTORY_TEST
			UpdatePageUI( GetNowInventoryPageNum( CX2Inventory::ST_SPECIAL ) );
			return SetInventorySort(CX2Inventory::ST_SPECIAL, GetNowInventoryPageNum( CX2Inventory::ST_SPECIAL ) );
#else	SERV_REFORM_INVENTORY_TEST
			return SetInventorySort(CX2Inventory::ST_SPECIAL);
#endif SERV_REFORM_INVENTORY_TEST
		}
	case UIM_AVATAR:
		{
#ifdef	SERV_REFORM_INVENTORY_TEST
			UpdatePageUI( GetNowInventoryPageNum( CX2Inventory::ST_AVARTA ) );
			return SetInventorySort(CX2Inventory::ST_AVARTA, GetNowInventoryPageNum( CX2Inventory::ST_AVARTA ) );
#else	SERV_REFORM_INVENTORY_TEST
			return SetInventorySort(CX2Inventory::ST_AVARTA);
#endif	SERV_REFORM_INVENTORY_TEST
		}

		//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
	case UIM_PC_BANG:
		{
			if ( g_pData != NULL &&
				 g_pData->GetMyUser() != NULL &&
				 g_pData->GetMyUser()->GetIsPCRoom() == true
#ifdef SERV_PC_BANG_TYPE
				 && g_pData->GetPremiumBuffManager() != NULL
				 && g_pData->GetPremiumBuffManager()->IsPcBangAvatar() == true
#endif SERV_PC_BANG_TYPE
				 )
			{
#ifdef	SERV_REFORM_INVENTORY_TEST
				UpdatePageUI();
				return SetInventorySort(CX2Inventory::ST_PC_BANG );
#else	SERV_REFORM_INVENTORY_TEST
				return SetInventorySort( CX2Inventory::ST_PC_BANG );
#endif	SERV_REFORM_INVENTORY_TEST
			}
			else
			{
				CheckRadioButtonBySortType( m_NowInventorySortType );
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4838 ),g_pMain->GetNowState() );
				return true; // 메시지 박스 처리
			}
		}
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스


#ifdef SERV_REFORM_INVENTORY_TEST
	case UIM_PAGE_1:
	case UIM_PAGE_2:
	case UIM_PAGE_3:
	case UIM_PAGE_4:
		{
			/// 인벤토리 카테고리 범위가 아니면 패스
			if( CX2Inventory::ST_AVARTA < m_NowInventorySortType || CX2Inventory::ST_NONE >= m_NowInventorySortType )
				return false;

			/// 현재 지정된 인벤토리 페이지 수 저장
			const int iNowInvenPageNum = wParam - UIM_PAGE_1+1;
			SetNowInventoryPageNum( m_NowInventorySortType, iNowInvenPageNum );
			SetInventorySort( m_NowInventorySortType, iNowInvenPageNum );
			return true;
		}
	case UIM_OVER_PAGE_1:
	case UIM_OVER_PAGE_2:
	case UIM_OVER_PAGE_3:
	case UIM_OVER_PAGE_4:
		{
			/// 인벤토리 카테고리 범위가 아니면 패스
			if( CX2Inventory::ST_AVARTA < m_NowInventorySortType || CX2Inventory::ST_NONE >= m_NowInventorySortType )
				return false;

			/// 아이템을 드래그 중인 상태로 페이지 버튼을 오버 하면 인벤토리 갱신
			int iTargetPage = wParam - UIM_OVER_PAGE_1+1;
			if( *m_pSlotBeforeDragging != NULL && GetNowInventoryPageNum( m_NowInventorySortType ) != iTargetPage )
			{
				SetNowInventoryPageNum( m_NowInventorySortType, iTargetPage );
				SetInventorySort( m_NowInventorySortType, iTargetPage );
				UpdatePageUI( iTargetPage );
			}
			return true;
		}
#else  SERV_REFORM_INVENTORY_TEST
	case UIM_PREV_PAGE:
		{
			PrevInvenPage();
			return true;
		}
	case UIM_NEXT_PAGE:
		{
			NextInvenPage();
			return true;
		}
#endif SERV_REFORM_INVENTORY_TEST

	case UIM_OPEN_RANDOM_ITEM:
		{
#ifdef FIX_RANDOM_CUBE_OPEN_RESULT_BUG
			//큐브 오픈 동작중이라면 패킷 다시 보내지 않기
			if( true == m_bProcessCubeOpen )
				return true;
#endif //FIX_RANDOM_CUBE_OPEN_RESULT_BUG
			m_eItemObtainResultType = IORT_NONE;
			return Handler_EGS_OPEN_RANDOM_ITEM_REQ();
		}
		break;
#ifdef RANDOM_CUBE_UI_NEW
	case UIM_OPEN_RANDOM_ITEM_NEW:
		{
#ifdef EXCHANGE_OPEN_IMAGE
			OpenRandomItem( m_RandomItemID, IORT_CUBE_OPEN );
#else EXCHANGE_OPEN_IMAGE
			m_fCubeOpenTime = 3.f;

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
			m_eItemObtainResultType = IORT_CUBE_OPEN;
#else
			m_bCubeOpen = true;
#endif RANDOM_EXCHANGE_RESULT_VIEW
			m_bCubeJustOpen = true;
			SAFE_DELETE_DIALOG(m_pDLGCubeOpenEffect);
			m_pDLGCubeOpenEffect = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cube_Open.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCubeOpenEffect );
			m_pDLGCubeOpenEffect->SetFront(true);

#ifdef VARIOUS_RANDOM_CUBE_UI_NEW
			// oasis907 : 김상윤 [2010.9.29] // ITEM ID 별 예외 랜덤 아이템 큐브 열기 UI 
			CKTDGUIStatic* pStaticCubeOpenLoading = (CKTDGUIStatic*) m_pDLGCubeOpenEffect->GetControl( L"g_pStaticNote_Loading" );
			switch(m_RandomItemID)
			{
			case MEMO_RANDOM_CUBE_ITEM_ID:
			case CAPSULE_RANDOM_CUBE_ITEM_ID:
				pStaticCubeOpenLoading->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture43.tga", L"Note");
				break;
			//{{ oasis907 : 김상윤 [2011.3.29] // 식목일 큐브 아이템
			case TREE_DAY_CUBE_ITEM_ID:
				{
					pStaticCubeOpenLoading->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture50.tga", L"Note");
					m_pDLGCubeOpenEffect->AddDummyInt(1);
				}
				break;
			//}}
#ifdef NEW_HENIR_TEST
			case NEW_HENIR_CUBE_ITEM_ID:
				pStaticCubeOpenLoading->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture59.tga", L"Note");
				break;
#endif NEW_HENIR_TEST
			default:
				pStaticCubeOpenLoading->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture40.tga", L"Note");
				break;
			}
#endif VARIOUS_RANDOM_CUBE_UI_NEW

#ifdef CUBE_OPEN_IMAGE_MANAGER
			// 초기 이미지를 그려주는데, 프레임부브에서 바로 교체를 해주니, 여기서는 이미지 셋팅은 안해줘도 되지 않나?
			if( g_pData->GetCubeOpenImageManager() != NULL && g_pData->GetCubeOpenImageManager()->IsEventCube( m_RandomItemID ) == true)
			{
				g_pData->GetCubeOpenImageManager()->SetStart( true);
				g_pData->GetCubeOpenImageManager()->SetSoundPlaying( true );
			}
#endif // CUBE_OPEN_IMAGE_MANAGER
			if ( m_pDLGRandomItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRandomItem, NULL, false );
				m_pDLGRandomItem = NULL;
			}
			InvalidSlotDesc();
#endif EXCHANGE_OPEN_IMAGE
			return true;
		}
		break;
#endif RANDOM_CUBE_UI_NEW
	case UIM_CLOSE_RANDOM_ITEM_RESULT:
		{			
			if ( m_pDLGCubeCheck != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCubeCheck, NULL, false );

			m_pDLGCubeCheck = NULL;
			m_eRandomItemEventType = RIOET_NONE;
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
			m_iUseOpenRandomItemID = 0;
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
		}
		break;
	case UIM_CREATE_PERSONAL_SHOP:
		{
#ifdef SERV_PSHOP_AGENCY
			if( g_pInstanceData != NULL && g_pInstanceData->IsActiveAgencyShop() == true )
			{
				if( m_pDLGCheckPShop == NULL )
				{
					m_pDLGCheckPShop = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999, -999), GET_STRING( STR_ID_12236 ), UIM_CHECK_PSHOP_OK, g_pMain->GetNowState() );
				}
			}
			else
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, CX2UIPersonalShop::OPENFORSELL);
			}			
#else
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, CX2UIPersonalShop::OPENFORSELL);
#endif
		}
		break;
#ifdef SERV_MULTI_RESOLVE
	case UIM_RESOLVE_ITEM_OPEN:
		{
			if( g_pData->GetUIManager()->GetUIPersonalShop() !=NULL && g_pData->GetUIManager()->GetUIPersonalShop()->GetShow() == true )
			{
				return true;
			}

			if( g_pData->GetUIManager()->GetUIResolveItem() !=NULL )
			{
				if(g_pData->GetUIManager()->GetUIResolveItem()->GetShow() ==false)
				{
					OpenResolveWindow();
				}
			}
			else
			{
				OpenResolveWindow();
			}
		}
		break;
#endif SERV_MULTI_RESOLVE
	case UIM_RESOLVE_ITEM_OK:
		{
			if(g_pData->GetUIManager()->GetDLGFlag() == false)
			{
				g_pData->GetUIManager()->SetDLGFlag(true);
				CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
				pControl->RequestFocus();
				return true;
			}
			if ( m_pDLGResolveItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGResolveItem, NULL, false );
			}
			m_pDLGResolveItem = NULL;

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
			m_eItemObtainResultType = IORT_ITEM_RESOLVE;
#endif
			return Handler_EGS_RESOLVE_ITEM_REQ();
		}
		break;
	case UIM_RESOLVE_ITEM_CANCLE:
		{
			if ( m_pDLGResolveItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGResolveItem, NULL, false );
			}
			m_pDLGResolveItem = NULL;

			return true;
		}
		break;
	case UIM_RESOLVE_ITEM_RESULT_OK:
		{
			if ( m_pDLGResolveItemResult != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGResolveItemResult, NULL, false );
			}
			m_pDLGResolveItemResult = NULL;

//{{ 김상훈 : 2010.11.16
#ifdef RANDOM_CUBE_UI_NEW
			SAFE_DELETE_DIALOG(m_pDLGCubeOpenEffect);
			m_fCubeOpenTime = 0.f;
			m_bCubeJustOpen = false;
#ifdef RANDOM_EXCHANGE_RESULT_VIEW
			m_eItemObtainResultType = IORT_NONE;
#else
			m_bCubeOpen = false;
#endif RANDOM_EXCHANGE_RESULT_VIEW
#endif RANDOM_CUBE_UI_NEW
//}} 김상훈 : 2010.11.16

			return true;
		}
		break;
	case UIM_RESOLVE_ITEM_RESULT_PREV_PAGE:
		{
			ChangeResolveResultPage( false );
			return true;
		}
		break;
	case UIM_RESOLVE_ITEM_RESULT_NEXT_PAGE:
		{
			ChangeResolveResultPage( true );
			return true;
		}
		break;
#ifndef SERV_SOCKET_NEW
//{{ oasis907 : 김상윤 [2010.4.19] // 
	case UIM_SOCKET_DLG_CLOSE:
		{
			if( m_pDLGSocketItem != NULL )
			{	
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSocketItem, NULL, false );
			}
			m_pDLGSocketItem = NULL;
			return true;
		}
		break;
	case UIM_SOCKET_NORMAL_STONE:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			int socketIndex = pControl->GetDummyInt(0);
			socketIndex--;
			if ( socketIndex < 0 )
				socketIndex = 0;

			m_SocketSlotIndex = socketIndex;
			m_bSocketRareMagicStone = false;
			m_bSocketRemove = false;

			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
			if ( pCheckBox != NULL )
			{
				if ( pCheckBox->GetChecked() == true )
				{
					OpenSocketCheatWindow();
				}
				else
				{
					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
					if ( pItem != NULL )
					{
						wstringstream wstrstm;
						wstrstm << GET_REPLACED_STRING( ( STR_ID_791, "L", g_pMain->GetEDString( pItem->GetEDToSocketPush() ) ) );
						m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999, -999), wstrstm.str().c_str(), UIM_SOCKET_FEE_OK, g_pMain->GetNowState() );
					}
				}
			}
		}
		break;

	case UIM_SOCKET_RARE_STONE:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			int socketIndex = pControl->GetDummyInt(0);
			socketIndex--;
			if ( socketIndex < 0 )
				socketIndex = 0;

			m_SocketSlotIndex = socketIndex;
			m_bSocketRareMagicStone = true;
			m_bSocketRemove = false;

			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
			if ( pCheckBox->GetChecked() == true )
			{
				OpenSocketCheatWindow();
			}
			else
			{
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
				if ( pItem != NULL )
				{
					m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999, -999), GET_REPLACED_STRING( ( STR_ID_791, "L", g_pMain->GetEDString( pItem->GetEDToSocketPush() ) ) ), UIM_SOCKET_FEE_OK, g_pMain->GetNowState() );
				}
			}
		}
		break;

	case UIM_SOCKET_REMOVE:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			int socketIndex = pControl->GetDummyInt(0);
			socketIndex--;
			if ( socketIndex < 0 )
				socketIndex = 0;

			m_SocketSlotIndex = socketIndex;
			m_bSocketRareMagicStone = false;
			m_bSocketRemove = true;

			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
			if ( pItem != NULL )
			{
				wstringstream wstrstm;
				wstrstm << GET_REPLACED_STRING( ( STR_ID_792, "L", g_pMain->GetEDString( pItem->GetEDToSocketRemove() ) ) );
				m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), UIM_SOCKET_FEE_OK, g_pMain->GetNowState() );
			}
		}
		break;

	case UIM_SOCKET_FEE_OK:
		{
			if ( m_pDLGSocketItemPushOk != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSocketItemPushOk, NULL, false );

			m_pDLGSocketItemPushOk = NULL;
			return Handler_EGS_SOCKET_ITEM_REQ( m_SocketSlotIndex, m_bSocketRareMagicStone, m_bSocketRemove );
		}
		break;

		//////////////////////////////////////////////////////////////////////////
//}} oasis907 : 김상윤 [2010.4.19] // 
#endif SERV_SOCKET_NEW

	case UIM_EL_CHANGER_EXIT:
		{
			if( m_pDLGElChanger != NULL )
			{	
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGElChanger, NULL, false );
			}
			m_pDLGElChanger = NULL;
			return true;
		} break;
	case UIM_EL_CHANGER_NUM_PLUS:
		{
			m_ElChangerItemCount++;
			UpdateElChangerDLG(false);
			return true;
		} break;
	case UIM_EL_CHANGER_NUM_MINUS:
		{
			m_ElChangerItemCount--;
			UpdateElChangerDLG(false);
			return true;
		} break;
	case UIM_EL_CHANGER_NUM_UPDATE:
		{
			UpdateElChangerDLG(true);
			return true;
		} break;
	case UIM_EL_CHANGER_OK:
		{
			if( m_pDLGElChanger != NULL )
			{	
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGElChanger, NULL, false );
			}
			m_pDLGElChanger = NULL;

			// 엘조각과 특스탭 인벤 공간 검사
			int iMaxSlot = 0;
			int iUnUseSlot = 0;  

			if ( g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				iMaxSlot = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum( CX2Inventory::ST_SPECIAL );
				iUnUseSlot = iMaxSlot - g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetUsedSlotNum(CX2Inventory::ST_SPECIAL );
			}			

			if( m_ElChangerItemCount < 6 )
			{
				if( iUnUseSlot >= m_ElChangerItemCount )
				{
					return Handler_EGS_IDENTIFY_ITEM_REQ();
				}
				else
				{
					// 인벤토리 특수 탭의 빈 공간을 X 개 확보 해야 합니다.
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_5058, "i", m_ElChangerItemCount ) ), g_pMain->GetNowState() );
					return true;
				}
			}
			else
			{
				if( iUnUseSlot >= 6 )
				{
					return Handler_EGS_IDENTIFY_ITEM_REQ();
				}
				else
				{
					// 인벤토리 특수 탭의 빈 공간을 X 개 확보 해야 합니다.
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_5058, "i", 6 ) ), g_pMain->GetNowState() );            
					return true;
				}
			}

			return true;
		} break;
	case UIM_EL_CHANGER_RESULT_OK:
		{
			if( m_pDLGElChangerResult != NULL )
			{	
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGElChangerResult, NULL, false );
			}
			m_pDLGElChangerResult = NULL;
			return true;

		}
		break;
#ifdef ITEM_RECOVERY_TEST
	case UIM_ITEM_RECOVERY_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			Handler_EGS_RESTORE_ITEM_REQ( m_RecoveryItemUid );
			m_RecoveryItemUid = 0;

			return true;
		} break;
#endif
	case UIM_ITEM_USE_WARP:
		{
			// 마을별 귀환서
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )

				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_WarpItem );
			m_WarpItem = 0;
		}
		break;

#ifdef SKILL_PLUS_ITEM_USE_POPUP
	case UIM_SKILL_PLUS_ITEM_USE:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )

				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_UsedItemUID );
			m_UsedItemUID = 0;
		}
		break;
#endif SKILL_PLUS_ITEM_USE_POPUP

#ifdef SERV_TREASURE_BOX_ITEM
	case UIM_TREASURE_BOX_CANCLE : 
		{
			// 더미 : ESC를 흡수하기 위함. 아무것도 하지 않는다.
			return true;
		} break;
#endif // SERV_TREASURE_BOX_ITEM

#ifdef ATTRACTION_ITEM_TEST
	case UIM_ATTRACTION_ITEM_DUMMY:
		{
			// 더미 : ESC를 흡수하기 위함. 아무것도 하지 않는다.
			return true;
		} break;
	case UIM_ATTRACTION_ITEM_OK:
		{
#ifdef FIX_RANDOM_CUBE_OPEN_RESULT_BUG
			//큐브 오픈 동작중이라면 패킷 다시 보내지 않기
			if( true == m_bProcessCubeOpen )
				return true;
#endif //FIX_RANDOM_CUBE_OPEN_RESULT_BUG

			// 돌아가는 중에는 메시지를 받지 말자~
			if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE &&  
				g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == true )
				return true;
#ifdef FIX_ICE_HEATER_OPEN_BUG
			//EGS_OPEN_RANDOM_ITEM_ACK를 받기 전에 m_eRandomItemEventType 변수가 RIOET_NONE 되는 경우, 가열기의 오픈씬이 사라집니다.
			//OK와 CANCLE 둘다 타게 되면 m_eRandomItemEventType 변수가 RIOET_NONE 값을 가지게 되어 둘다 탈 수 없도록 수정했습니다.

			//CANCLE먼저 탈 경우에 대한 예외처리
			if( RIOET_NONE == m_eRandomItemEventType ) 
				return true;

			//OK 먼저 탈 경우에 대한 예외처리
			if( NULL != m_pDLGOpenAttraction )
				m_pDLGOpenAttraction->SetCloseCustomUIEventID(-1);
#endif //FIX_ICE_HEATER_OPEN_BUG

			return Handler_EGS_OPEN_RANDOM_ITEM_REQ();
		}
		break;
	case UIM_ATTRACTION_ITEM_CANCLE:
		{
			// 돌아가는 중에는 메시지를 받지 말자~
			if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE &&  
				g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == true )
				return true;

			if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
			{
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
				if( NULL != m_pDLGOpenAttraction )
				{
					m_pDLGOpenAttraction->SetHasUnit( NULL );
				}
			}

			if ( m_pDLGOpenAttraction != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOpenAttraction, NULL, false );
				m_pDLGOpenAttraction = NULL;
			}

			m_eRandomItemEventType = RIOET_NONE;

#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
			m_iUseOpenRandomItemID = 0;
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

			return true;
		}
		break;
	case UIM_ATTRACTION_ITEM_RESULT_OK:
		{
			if ( m_pDLGAttractionResult != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGAttractionResult, NULL, false );
			m_pDLGAttractionResult = NULL;

			m_eRandomItemEventType = RIOET_NONE;

#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
			m_iUseOpenRandomItemID = 0;
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

			return true;
		}
		break;
	case UIM_USE_COUPON_ITEM_OK:
		{
			Handler_EGS_ENCHANT_ATTACH_ITEM_REQ( m_CouponUid, m_CouponTargetUid );		

			m_CouponUid = 0;
			m_CouponTargetUid = 0;
			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			if(pState->GetCursor() != NULL)
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );

			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		}
		break;
#ifdef SERV_ATTRIBUTE_CHARM
	case UIM_USE_ATTRIBUTE_CHARM_COUPON_ITEM_OK:
		{
			Handler_EGS_ATTRIB_ATTACH_ITEM_REQ( m_CouponUid, m_CouponTargetUid );		

			m_CouponUid = 0;
			m_CouponTargetUid = 0;
			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			if(pState->GetCursor() != NULL)
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );

			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		}
		break;
#endif SERV_ATTRIBUTE_CHARM
		//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
	case UIM_SORT_CATEGORY_ITEM:
		{
			Handler_EGS_SORT_CATEGORY_ITEM_REQ(m_NowInventorySortType);

			return true;
		} break;
#endif SERV_SORT_CATEGORY_ITEM 
		//}}
	case UIM_USE_COUPON_ITEM_CANCLE:
		{
			// 			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			// 			if ( pState->GetCursor() != NULL )
			// 			{
			// 				m_CouponUid = 0;
			// 				m_CouponTargetUid = 0;
			// 				if(pState->GetCursor() != NULL)
			// 					pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
			// 			}

			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			return true;

		}
		break;
#endif
	case UIM_OPEN_RANDOM_ITEM_CANCLE:
		{
			m_eRandomItemEventType = RIOET_NONE;
			if ( m_pDLGRandomItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRandomItem, NULL, false );
				m_pDLGRandomItem = NULL;
			}

#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
			m_iUseOpenRandomItemID = 0;
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

		}
		break;

		//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
	case UIM_REGISTER_QUANTITY_CONFIRM:
		{
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );
			
			m_pDLGItemEnroll = NULL;
			
#ifdef SPT_NONE_ITEM_DELETE
			if( true == m_bIsSptNoneItem)
			{
				CX2Item* pItem		= NULL;
				pItem				= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnrollItemUid );

				if ( pItem == NULL || 
					pItem->GetItemTemplet() == NULL )
					return false;
#ifdef CLIENT_GLOBAL_LINEBREAK
				std::wstring wstrMsg = GET_REPLACED_STRING( (STR_ID_23495, "Li", pItem->GetFullName(), m_iItemEnrollNum ) );
				std::wstring tempName = CWordLineHandler::GetStrByLineBreakInX2Main( wstrMsg.c_str(), 257, 0 );

				m_pSptNoneItemDelete = 
					g_pMain->KTDGUIOkAndCancelEditBox2( D3DXVECTOR2( -999, -999 ), tempName.c_str(), 
					UIM_SPTNONE_ITEM_DELETE_OK, g_pMain->GetNowState(), 10, false, UIM_SPTNONE_ITEM_DELETE_CANCEL );
#else //CLIENT_GLOBAL_LINEBREAK
				m_pSptNoneItemDelete = 
					g_pMain->KTDGUIOkAndCancelEditBox2( D3DXVECTOR2( -999, -999 ), 
					GET_REPLACED_STRING( (STR_ID_23495, "Li", pItem->GetFullName(), m_iItemEnrollNum ) ), 
					UIM_SPTNONE_ITEM_DELETE_OK, g_pMain->GetNowState(), 10, false, UIM_SPTNONE_ITEM_DELETE_CANCEL );
#endif //CLIENT_GLOBAL_LINEBREAK
				m_bIsSptNoneItem = false;
				pItem = NULL;
			}
			else
#endif //SPT_NONE_ITEM_DELETE
			OpenDeleteItemConfirmDLG();
			return true;
		}
		break;

	case UIM_REGISTER_QUANTITY_CANCEL:
		{
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );

#ifdef SPT_NONE_ITEM_DELETE
			m_bIsSptNoneItem = false;
#endif //SPT_NONE_ITEM_DELETE

			m_pDLGItemEnroll = NULL;
			m_iItemEnrollNum = 1;		// 수량성 아이템을 버리는 Dialog Text 1로 고정
			return true;
		}
		break;

	case UIM_REGISTER_QUANTITY_PLUS:
		{
			m_iItemEnrollNum++;
			UpdateRegisterQuantityDLG(false);
			return true;
		}
		break;

	case UIM_REGISTER_QUANTITY_MINUS:
		{
			m_iItemEnrollNum--;
			UpdateRegisterQuantityDLG(false);
			return true;
		}
		break;

	case UIM_REGISTER_QUANTITY_UPDATE:
		{
			UpdateRegisterQuantityDLG(true);
			return true;
		}
		break;

	case UIM_DELETE_ITEM_CONFIRM_OK:
		{
			if ( m_pDLGDeleteItemConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGDeleteItemConfirm, NULL, false );

			Handler_EGS_DELETE_ITEM_REQ( m_EnrollItemUid, m_iItemEnrollNum );

			m_pDLGDeleteItemConfirm		= NULL;
			m_iItemEnrollNum			= 1;		// 수량성 아이템을 버리는 Dialog Text 1로 고정
			m_EnrollItemUid				= 0;
			return true;
		}
		break;

	case UIM_DELETE_ITEM_CONFIRM_CANCEL:
		{
			if ( m_pDLGDeleteItemConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGDeleteItemConfirm, NULL, false );

			m_pDLGDeleteItemConfirm		= NULL;
			m_iItemEnrollNum			= 1;		// 수량성 아이템을 버리는 Dialog Text 1로 고정
			m_EnrollItemUid				= 0;
			return true;
		}
		break;

#endif REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

#ifdef SPT_NONE_ITEM_DELETE
	case UIM_SPTNONE_ITEM_DELETE_OK:
		{
			if(m_pSptNoneItemDelete != NULL)
			{
				CKTDGUIIMEEditBox* pIMEEditBox = static_cast<CKTDGUIIMEEditBox*>(m_pSptNoneItemDelete->GetControl(L"IMEEditBoxMessage"));
				if( NULL != pIMEEditBox )
				{
					wstring wstrMessage = pIMEEditBox->GetText();
#ifdef ALWAYS_MISS_DEFINE_CHECK //태국어는 strcmp로 비교가 안됩니다.
					if(wstrMessage.compare( GET_STRING( STR_ID_23608 ) ) == 0)
#else //ALWAYS_MISS_DEFINE_CHECK
					if( 0 == StrCmp(wstrMessage.c_str(), GET_STRING(STR_ID_23608)) )
#endif ALWAYS_MISS_DEFINE_CHECK
					{
						
						//Handler_EGS_DELETE_ITEM_REQ( m_EnrollItemUid, m_iItemEnrollNum );
						g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pSptNoneItemDelete, NULL, false );
						m_pSptNoneItemDelete = NULL;
						m_bIsSptNoneItem = false;
						OpenDeleteItemConfirmDLG();
					}
					else
					{
						CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_23609 ), g_pMain->GetNowState() );
						if( NULL != pMsgBox )
							pMsgBox->SetEnableMoveByDrag_LUA(false);
					}
					
				}
			}
		}
		break;

	case UIM_SPTNONE_ITEM_DELETE_CANCEL:
		{
			if(m_pSptNoneItemDelete != NULL)
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pSptNoneItemDelete, NULL, false );
				m_pSptNoneItemDelete = NULL;
				m_bIsSptNoneItem = false;
			}
		}
		break;
#endif //SPT_NONE_ITEM_DELETE

		//{{ kimhc // 2009-08-19 // 밀봉 확인 이나 취소를 누른 경우
#ifdef	SEAL_ITEM
	case UIM_SEAL_ITEM_CONFIRM_OK:
		{
			if ( m_pDLGSealItemConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSealItemConfirm, NULL, false );

			if ( g_pData->GetMyUser() == NULL ||
				 g_pData->GetMyUser()->GetSelectUnit() == NULL )
			{
				ASSERT( !"GetMyuser or GetSelectUnit is NULL" );
				m_pSlotItemToBeSealed	= NULL;
				ResetItemIDForSealAndCursorState();
				return true;
			}

			if ( m_pSlotItemToBeSealed == NULL )
			{
				ASSERT( !"m_pSlotItemToBeSealed" );
				ResetItemIDForSealAndCursorState();
				return true;
			}

			Handler_EGS_SEAL_ITEM_REQ( m_pSlotItemToBeSealed->GetItemUID() );
			
			return true;
		}
		break;

	case UIM_SEAL_ITEM_CONFIRM_CANCEL:
		{
			if ( m_pDLGSealItemConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSealItemConfirm, NULL, false );

			m_pDLGSealItemConfirm		= NULL;
			m_pSlotItemToBeSealed		= NULL;

			return true;
		}
		break;
#endif	SEAL_ITEM
		//}} kimhc // 2009-08-19 // 밀봉 확인 이나 취소를 누른 경우

		//{{ kimhc // 2009-09-03 // 봉인된 아이템을 우클릭 했을때의 확인, 취소
#ifdef	SEAL_ITEM
	case UIM_UNSEAL_ITEM_CONFIRM_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
            
			Handler_EGS_UNSEAL_ITEM_REQ( m_itemUIDToBeUnSealed );

		}
		break;

	case UIM_UNSEAL_ITEM_CONFIRM_CANCEL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
		}
		break;
#endif	SEAL_ITEM
		//}} kimhc // 2009-09-03 // 봉인된 아이템을 우클릭 했을때의 확인, 취소

#ifdef FREE_WARP
	case UIM_WARP_DEST_CLOSE:
		{
			if( m_pDLGWarpDestination != NULL )
			{
				m_pDLGWarpDestination->SetShowEnable(false, false);
				m_iSelectedWarpDest = 0;
			}
		}
		break;
	case UIM_WARP_DEST_SELECT:
		{
			CKTDGUIButton *pButton = (CKTDGUIButton*)lParam;
			if( pButton == NULL )
				return true;

			m_iSelectedWarpDest = 0;
			m_iSelectedWarpDest = pButton->GetDummyInt(0);
			
			for(UINT i=0; i<m_vecWarpListButton.size(); ++i)
			{
				CKTDGUIButton *pDestButton = m_vecWarpListButton[i];
				if(pDestButton != NULL)
				{
					pDestButton->SetDownStateAtNormal(false);
					if( pDestButton->GetDummyInt(0) == m_iSelectedWarpDest )
					{
						m_iSelectedWarpDestIndex = i;
					}
				}
			}

			pButton->SetDownStateAtNormal(true);

		}			
		break;
	
	case UIM_WARP_DEST:
		{
			if( m_iCurrentVillageWarpIndex == -1 )
				return true;

			if( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() &&
				m_iSelectedWarpDest== g_pData->GetLocationManager()->GetCurrentVillageID() )
				return true;

			UseWarpPopup(m_bIsEdConsumptionWarp);
			return true;
		}
		break;
	case UIM_WARP_DEST_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			
			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_WarpItem, m_iSelectedWarpDest );
			m_WarpItem = 0;
			m_iSelectedWarpDest = 0;

			if( m_pDLGWarpDestination != NULL )
			{
				m_pDLGWarpDestination->SetShowEnable(false, false);
			}

			return true;
		}
		break;
#endif

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	case UIM_UNLIMITED_SECOND_CHANGE_JOB_CLOSE:
		{
			if( m_pDLGUSCJob != NULL )
			{
				m_pDLGUSCJob->SetShowEnable(false, false);
				m_iSelectedUSCJob = 0;
			}
		}
		break;
	case UIM_UNLIMITED_SECOND_CHANGE_JOB_SELECT:
		{
			CKTDGUIButton *pButton = (CKTDGUIButton*)lParam;
			if( pButton == NULL )
				return true;

			m_iSelectedUSCJob = 0;
			m_iSelectedUSCJob = pButton->GetDummyInt(0);

			for(UINT i=0; i<m_vecUSCJobButton.size(); ++i)
			{
				CKTDGUIButton *pUSCJobButton = m_vecUSCJobButton[i];
				if(pUSCJobButton != NULL)
				{
					pUSCJobButton->SetDownStateAtNormal(false);
				}
			}

			pButton->SetDownStateAtNormal(true);
		}			
		break;
	case UIM_UNLIMITED_SECOND_CHANGE_JOB_DEST:
		{
			if( m_iSelectedUSCJob >= static_cast<int>( CX2Unit::UC_ELSWORD_LORD_KNIGHT ) &&
				m_iSelectedUSCJob != static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() ) )
			{
				UseUSCJobPopup();
			}

			return true;
		}
		break;
	case UIM_UNLIMITED_SECOND_CHANGE_JOB_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			//전직 요청 한다 여러가지 체크를 해야한다. 아이템이 인벤에있는지 레벨 2차 전직인지,unit등을 체크하자
			
			if( 2 == g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() &&
				g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( UNLIMITED_SECOND_CHANGE_JOB_ITEM_ID ) != NULL )
			{
				KEGS_UNLIMITED_SECOND_CHANGE_JOB_NOT kPacket;
				kPacket.m_iUnlimitedSecondChangeJob = m_iSelectedUSCJob;

				g_pData->GetServerProtocol()->SendPacket( EGS_UNLIMITED_SECOND_CHANGE_JOB_NOT, kPacket );
			}
			
			m_iSelectedUSCJob = 0;

			if( m_pDLGUSCJob != NULL )
			{
				m_pDLGUSCJob->SetShowEnable(false, false);
			}

			return true;
		}
		break;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef GUILD_SKILL
		//{{ oasis907 : 김상윤 [2009.11.30] // 길드 스킬 초기화
	case UIM_GUILD_SKILL_INIT_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			if( g_pData->GetSkillTree() != NULL )
			{
				g_pData->GetSkillTree()->Handler_EGS_INIT_GUILD_SKILL_TREE_REQ(m_UsedItemUID);
			}
			return true;
		} break;
		//}} oasis907 : 김상윤 [2009.11.30] //
#endif GUILD_SKILL

#ifdef SERV_SKILL_NOTE
	case UIM_SKILL_NOTE:
		{
			if( g_pData != NULL && g_pData->GetUIManager() != NULL )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_NOTE, true);				
			}
			return true;
		}
		break;
#endif
	case UIM_UNKNOWN_TO_IDENTIFY:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGIdentifyStone, NULL, false );
			m_pDLGIdentifyStone = NULL;
			return Handler_EGS_IDENTIFY_ITEM_REQ();
		}		
		break;
#ifdef SERV_PET_SYSTEM
	case UIM_PET_FOOD:
		{
			FeedPet();			
		}
		break;
	case UIM_PET_FOOD_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			//if( g_pMain->GetInfoBox() != NULL )
			//	g_pMain->DeleteInfoBox();

			if( g_pData != NULL && g_pData->GetPetManager() != NULL && m_pPetFoodSlot != NULL )
			{
				g_pData->GetPetManager()->Handler_EGS_FEED_PETS_REQ( m_pPetFoodSlot->GetItemUID() );
				m_pPetFoodSlot = NULL;
			}
		}
		break;
#ifdef ADD_PET_INVENTORY_BUTTON
	case UIM_PET_INVENTORY_OPEN:
		{
			if(m_pUIPetInventory != NULL && m_pUIPetInventory->GetShow() == true)
			{
				m_pUIPetInventory->SetShow(false);
			}
			else
			{
				g_pData->GetUIManager()->PetInventoryForceCloseOtherDlg();
				OpenPetInventory();		
			}
		}
		break;

#endif ADD_PET_INVENTORY_BUTTON
#endif
#ifdef BUFF_TEMPLET_SYSTEM
	case UIM_ITEM_USE_DEFENCE_DUNGEON_POTION:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_DefencedungeonPotion, 0, m_DefencedungeonPotionID );
		}
		break;
#endif BUFF_TEMPLET_SYSTEM
#ifdef SERV_PSHOP_AGENCY
	case UIM_CHECK_PSHOP_OK:
		{
			if ( m_pDLGCheckPShop != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCheckPShop, NULL, false );
			m_pDLGCheckPShop = NULL;
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, CX2UIPersonalShop::OPENFORSELL);
		}
#endif //SERV_PSHOP_AGENCY
		//{{ 2011.10.24 이지헌 : 골드 티켓 작업 ( 사용하면 캐쉬 주는 아이템 )
#ifdef SERV_GOLD_TICKET
	case UIM_USE_GOLD_TICKET:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_UsedItemUID );
			return true;
		}
		break;
#endif //SERV_GOLD_TICKET
		//}}
#ifdef PET_DROP_ITEM_PICKUP
	case UIM_PET_DROP_ITEM_PICK_UP_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl == NULL )
				return true;

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_UsedItemUID, m_UseItemTempUID );					

			m_UsedItemUID = 0;
			m_UseItemTempUID = 0;
			return true;
		} break;
#endif //PET_DROP_ITEM_PICKUP

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	case UIM_PET_HALLOWEEN_TRANSFORM_POSION_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl == NULL )
				return true;

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			g_pData->GetPetManager()->Handler_EGS_SUMMON_PET_REQ( 0 ); // 펫 소환 해제 먼저!!!

			return true;
		} break;
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05//미라클큐브
	case UIM_USE_RESTORE_ITEM_EVALUATION_OK:
		{
			Handler_EGS_RESTORE_ITEM_EVALUATE_REQ();

			CX2State* pState = static_cast<CX2State*>(g_pMain->GetNowState());
			if( NULL != pState && NULL != pState->GetCursor() )
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );

			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		}
		break;
	case UIM_USE_RESTORE_ITEM_EVALUATION_CANCEL:
		{
			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		} break;
	case UIM_USE_EVALUATE_ITEM_OK:
		{
			Handler_EGS_ITEM_EVALUATE_REQ();

			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		}
		break;
	case UIM_USE_EVALUATE_ITEM_CANCEL:
		{
			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		} break;
	case UIM_EXCHANGE_NEW_ITEM_OK:
		{
			Handler_EGS_ITEM_CONVERT_REQ( m_iTargetItemUID, m_iConsumeItemQuantity );
			m_iTargetItemUID = -1;
			m_iConsumeItemQuantity = -1;

			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		}
		break;
	case UIM_EXCHANGE_NEW_ITEM_CANCEL:
		{
			m_iTargetItemUID = -1;
			m_iConsumeItemQuantity = -1;

			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		} break;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef RIDING_ITEM_POPUP_CHECK
	case UIM_RIDING_ITEM_USE_OK:
		{
			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			CX2RidingPetManager::GetInstance()->Handler_EGS_CREATE_RIDING_PET_REQ( m_iRidingItemUid );
			m_iRidingItemUid = 0;

			return true;
		} break;
#endif //RIDING_ITEM_POPUP_CHECK

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case UIM_EL_BREAK_EXIT:
		{
			if( m_pDLGElBreak != NULL )
			{	
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGElBreak, NULL, false );
			}
			m_pDLGElBreak = NULL;
			return true;
		} break;
	case UIM_EL_BREAK_NUM_PLUS:
		{
			++m_ElBreakItemCount;
			UpdateElBreakDLG( false );
			return true;
		} break;
	case UIM_EL_BREAK_NUM_MINUS:
		{
			--m_ElBreakItemCount;
			UpdateElBreakDLG( false );
			return true;
		} break;
	case UIM_EL_BREAK_NUM_UPDATE:
		{
			UpdateElBreakDLG( true );
			return true;
		} break;
	case UIM_EL_BREAK_OK:
		{
			if( m_pDLGElBreak != NULL )
			{	
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGElBreak, NULL, false );
			}
			m_pDLGElBreak = NULL;

			// 엘조각과 특스탭 인벤 공간 검사
			int iMaxSlot = 0;
			int iUnUseSlot = 0;  

			if ( g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				iMaxSlot = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum( CX2Inventory::ST_SPECIAL );
				iUnUseSlot = iMaxSlot - g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetUsedSlotNum(CX2Inventory::ST_SPECIAL );
			}			

			if( m_ElBreakItemCount < 300 )
			{
				if( iUnUseSlot >= 1 )
				{
					return Handler_EGS_ITEM_EXTRACT_REQ();
				}
				else
				{
					// 인벤토리 특수 탭의 빈 공간을 X 개 확보 해야 합니다.
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_5058, "i", 1 ) ), g_pMain->GetNowState() );
					return true;
				}
			}
			else
			{
				if( iUnUseSlot >= 1 )
				{
					return Handler_EGS_ITEM_EXTRACT_REQ();
				}
				else
				{
					// 인벤토리 특수 탭의 빈 공간을 X 개 확보 해야 합니다.
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_5058, "i", 6 ) ), g_pMain->GetNowState() );
					return true;
				}
			}

			return true;
		} break;
	case UIM_EL_BREAK_RESULT_OK:
		{
			return true;
		}
		break;
#endif //FINALITY_SKILL_SYSTEM

#ifdef GOOD_ELSWORD //JHKang
	case UIM_EXPAND_INVENTORY_CASH:
		{
			CX2StateField*	pStateField	=	NULL;
			pStateField		=  static_cast< CX2StateField* >( g_pMain->GetNowState() );

			if ( pStateField != NULL )
			{
				if ( g_pTFieldGame != NULL )
				{
					g_pTFieldGame->CloseFieldName( 0.0f );
				}

				g_pData->GetCashShop()->SetMenuTypeCallingCashShop( CX2UIManager::UI_MENU_PRIVATE_BANK );
				pStateField->ToggleCashShop();
			}

			return true;
		} break;
#ifndef NO_GOOD_ELSWORD_INT
	case UIM_EXPAND_INVENTORY_ED:
		{
			Handler_EGS_GET_NEXT_INVENTORY_ED_REQ();
			return true;
		} break;
	case UIM_EXPAND_INVENTORY_ED_BUY_CONFIRM:
		{
			if ( m_pDLGBuyConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyConfirm, NULL, false );

			m_pDLGBuyConfirm = NULL;

			m_bShow = false;
			SetShow( m_bShow );

			if ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED >= m_iConfirmED )
			{
				m_iConfirmED = 0;
				return Handler_EGS_EXPAND_INVENTORY_ED_REQ();
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28961 ), g_pMain->GetNowState() );
				m_iConfirmED = 0;
				m_bShow = true;
				SetShow( m_bShow );
				return false;
			}
		} break;

	case UIM_EXPAND_INVENTORY_ED_BUY_CANCEL:
		{
			if ( m_pDLGBuyConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyConfirm, NULL, false );

			m_pDLGBuyConfirm = NULL;

			return true;
		} break;
#endif NO_GOOD_ELSWORD_INT
#endif //GOOD_ELSWORD
#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장
	case UIM_USE_ADD_SOCKET_SLOT_OK:
		{
			Handler_EGS_SOCKET_EXPAND_ITEM_REQ();

			m_iTargetItemUID = -1;
			m_iConsumeItemQuantity = -1;

			CX2State* pState = static_cast<CX2State*>(g_pMain->GetNowState());
			if( NULL != pState && NULL != pState->GetCursor() )
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );

			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		} break;
	case UIM_USE_ADD_SOCKET_SLOT_CANCEL:
		{
			m_iTargetItemUID = -1;
			m_iConsumeItemQuantity = -1;

			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		} break;
#endif // ADD_SOCKET_SLOT

#ifdef REFORM_SKILL_NOTE_UI // 기술의 노트 버튼 제거하지 않고 비활성화로 변경
	case UIM_BUY_SKILL_NOTE_GUIDE_MOUSE_UP:
		{
			//던전/대전 게임중에는 캐쉬샵 열지 않기.
			if( g_pX2Game != NULL && g_pMain->GetNowStateID() != CX2Main::XS_BATTLE_FIELD )
			{
				g_pChatBox->AddChatLog(  GET_STRING( STR_ID_16478 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				return true;
			}
			if( NULL != g_pData->GetCashShop() )
			{
				g_pData->GetCashShop()->SetMenuTypeCallingCashShop( CX2UIManager::UI_SKILL_NOTE );
				g_pData->GetCashShop()->SetItemIDShowBuyUIAfterEnter(CASH_SKILL_NOTE_ITEM_ID);
			}
			if( g_pMain->IsInheritStateMenu() )
			{
				CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
				if( NULL != pStateMenu)
					pStateMenu->ToggleCashShop();
			}

			return true;
		}
#endif // REFORM_SKILL_NOTE_UI

#ifdef SERV_RELATIONSHIP_EVENT_INT
	case UIM_USE_PROPOSE_ITEM:
		{
			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			SAFE_DELETE_DIALOG(m_pDLGMarryEventNickName);
			m_pDLGMarryEventNickName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_MarryEvent_Nickname.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMarryEventNickName );			

			return true;
		} break;
	case UIM_USE_PROPOSE_ITEM_OK:
		{
			if( m_pDLGMarryEventNickName != NULL )
			{
				CKTDGUIIMEEditBox* pIMEEditID = (CKTDGUIIMEEditBox*)m_pDLGMarryEventNickName->GetControl( L"IMEEditBoxName" );

				if( pIMEEditID != NULL )
				{
					CX2State* pState = (CX2State*)(g_pMain->GetNowState());
					if( pState != NULL )
					{
						wstring wstrNickName = pIMEEditID->GetText();
						Handler_EGS_USE_PROPOSE_ITEM_REQ( m_UsedItemUID, wstrNickName );
					}
				}

				CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
				if ( pControl != NULL )
				{
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
				}
			}
			return true;
		} break;
	case UIM_USE_PROPOSE_ITEM_CANCLE:
		{
			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			return true;
		} break;
	case UIM_USE_DIVORCE_ITEM:
		{
			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			Handler_EGS_USE_DIVORCE_ITEM_REQ();
			return true;
		} break;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
	case UIM_RECOMMEND_OK:
		{
			//REQ날려보자
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGRecommend->GetControl( L"g_pIMEEditBoxName" );
			if ( pIMEEditBox != NULL && wcslen( pIMEEditBox->GetText() ) > 0 )
			{
				return Handler_EGS_REGISTER_RECRUITER_REQ( pIMEEditBox->GetText() );
			}
		}
		break;

	case UIM_RECOMMEND_CANCEL:
		{
			OpenRecommendMsgBox( false );
		}
		break;
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
	case UIM_USE_EXPAND_INVENTORY_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}

			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_UsedItemUID );
			return true;
		} break;
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
	default:
		break;
	}
	return false;
}

bool CX2UIInventory::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef SERV_SOCKET_NEW 
	// oasis907 : 김상윤 [2010.4.5] // 
	if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true)
	{
		if ( m_pUISocketItem->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}
#endif SERV_SOCKET_NEW

#ifdef SERV_PET_SYSTEM
	// oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리
	if(m_pUIPetInventory != NULL )
	{
		if ( m_pUIPetInventory->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}
#endif SERV_PET_SYSTEM

	switch ( wParam )
	{
	case EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK:
		{
			return Handler_EGS_CHANGE_INVENTORY_SLOT_ACK( hWnd, uMsg, wParam, lParam );
		}break;

		//{{ 2007. 10. 8  최육사  랜덤 아이템 ACK
	case EGS_OPEN_RANDOM_ITEM_ACK:
		{
			return Handler_EGS_OPEN_RANDOM_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
		//}}

	case EGS_RESOLVE_ITEM_ACK:
		{
			return Handler_EGS_RESOLVE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#ifndef SERV_SOCKET_NEW
//{{ oasis907 : 김상윤 [2010.4.19] // 
	case EGS_SOCKET_ITEM_ACK:
		{
			return Handler_EGS_SOCKET_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
//}}
#endif SERV_SOCKET_NEW
	case EGS_IDENTIFY_ITEM_ACK:
		{
			return Handler_EGS_IDENTIFY_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_USE_ITEM_IN_INVENTORY_ACK :
		{
			return Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_ITEM_EXPIRATION_NOT:
		{
			return Handler_EGS_ITEM_EXPIRATION_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
		
		//{{ kimhc // 2009-08-10 // 은행 등급을 올려주는 퀘스트 아이템을 사용 했을때 오는 패킷
#ifdef	PRIVATE_BANK

	case EGS_EXPAND_BANK_SLOT_NOT:	// 던전 or 필드에서 모두 사용이 가능 하도록???
		{
			return Handler_EGS_EXPAND_BANK_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-10 // 은행 등급을 올려주는 퀘스트 아이템을 사용 했을때 오는 패킷

		//{{ kimhc // 2009-08-27 // 아이템 봉인
#ifdef	SEAL_ITEM
	case EGS_SEAL_ITEM_ACK:
		{
			return Handler_EGS_SEAL_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_UNSEAL_ITEM_ACK:
		{
			return Handler_EGS_UNSEAL_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif	SEAL_ITEM
		//}} kimhc // 2009-08-27 // 아이템 봉인

#ifdef ITEM_RECOVERY_TEST
	case EGS_RESTORE_ITEM_ACK:
		{
			return Handler_EGS_RESTORE_ITEM_ACK( hWnd, uMsg, wParam, lParam );

		} break;
#endif
#ifdef ATTRACTION_ITEM_TEST
	case EGS_ENCHANT_ATTACH_ITEM_ACK:
		{
			return Handler_EGS_ENCHANT_ATTACH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#ifdef SERV_ATTRIBUTE_CHARM
	case EGS_ATTRIB_ATTACH_ITEM_ACK:
		{
			return Handler_EGS_ATTRIB_ATTACH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_ATTRIBUTE_CHARM
#endif
#ifdef AUTH_DELETE_ITEM
	case EGS_DELETE_ITEM_ACK:
		{
			return Handler_EGS_DELETE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} break;

#endif
		//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
	case EGS_SORT_CATEGORY_ITEM_ACK:
		{
			return Handler_EGS_SORT_CATEGORY_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_SORT_CATEGORY_ITEM 
		//}}

#ifdef	SERV_SHARING_BANK_TEST
	case EGS_UPDATE_ITEM_POSITION_NOT:
		{
			return Handler_EGS_UPDATE_ITEM_POSITION_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif	SERV_SHARING_BANK_TEST

#ifdef SERV_GOLD_TICKET
	case EGS_CHARGE_POINT_NOT:
		{
			return handler_EGS_CHARGE_POINT_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif //SERV_GOLD_TICKET

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	case EGS_ITEM_EVALUATE_ACK:
		{
			return Handler_EGS_ITEM_EVALUATE_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_RESTORE_ITEM_EVALUATE_ACK:
		{
			return Handler_EGS_RESTORE_ITEM_EVALUATE_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ITEM_CONVERT_ACK:
		{
			return Handler_EGS_ITEM_CONVERT_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case EGS_ITEM_EXTRACT_ACK:
		{
			return Handler_EGS_ITEM_EXTRACT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

#endif //FINALITY_SKILL_SYSTEM
#ifdef GOOD_ELSWORD //JHKang
#ifndef NO_GOOD_ELSWORD_INT
	case EGS_GET_NEXT_INVENTORY_ED_ACK:
		{
			return Handler_EGS_GET_NEXT_INVENTORY_ED_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_EXPAND_INVENTORY_ED_ACK:
		{
			return Handler_EGS_EXPAND_INVENTORY_ED_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif //GOOD_ELSWORD
#endif NO_GOOD_ELSWORD_INT
#ifdef ADD_SOCKET_SLOT
	case EGS_SOCKET_EXPAND_ITEM_ACK:
		{
			return Handler_EGS_SOCKET_EXPAND_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif // ADD_SOCKET_SLOT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	case EGS_USE_PROPOSE_ITEM_ACK:
		{
			return Handler_EGS_USE_PROPOSE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_USE_DIVORCE_ITEM_ACK:
		{
			return Handler_EGS_USE_DIVORCE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_CUBE_IN_ITEM_MAPPING
	case EGS_CUBE_IN_ITEM_MAPPING_NOT:
		{
			return Handler_EGS_CUBE_IN_ITEM_MAPPING_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_RECRUIT_EVENT_BASE
	case EGS_USE_RECRUIT_TICKET_ACK:
		{
			return Handler_EGS_USE_RECRUIT_TICKET_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_REGISTER_RECRUITER_ACK:
		{
			return Handler_EGS_REGISTER_RECRUITER_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_RECRUIT_EVENT_BASE
	}

	return false;
}


void CX2UIInventory::SetLayer(X2_DIALOG_LAYER layer)
{
#ifdef NEW_ITEM_NOTICE
	if( NULL != m_pDLGItemHighLight )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGItemHighLight, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGItemHighLight, false );
	}
#endif //NEW_ITEM_NOTICE
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);
	if(m_pDLGUIInventory != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIInventory, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIInventory, false );
	}
	//m_pDLGUIInventory->SetLayer(layer);

	//SetSlotManagerLayer(layer);


}

void CX2UIInventory::SetShow(bool val)
{
	m_bShow = val;
	//SetEnable(m_bShow);
	//SetShow(m_bShow);

	if(val)	// 켜질 때 처리해야 할 부분
	{
		D3DXVECTOR3 tmp;
		tmp = m_pDLGUIInventory->GetDummyPos(0);
		m_DLGPosition.x = tmp.x;
		m_DLGPosition.y = tmp.y;
		tmp = m_pDLGUIInventory->GetDummyPos(1);
		m_DLGSize.x = tmp.x;
		m_DLGSize.y = tmp.y;

		UpdateUIButton();

#ifdef SERV_REFORM_INVENTORY_TEST
		SetInventorySort(m_NowInventorySortType, GetNowInvenSortTypePageNum());
#else  SERV_REFORM_INVENTORY_TEST
		SetInventorySort(m_NowInventorySortType, m_NowInvenSortTypePageNum);
#endif SERV_REFORM_INVENTORY_TEST

		if(g_pData != NULL && g_pData->GetUIManager() != NULL)
		{
			g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_INVEN);
#ifdef GUIDE_QUICK_QUEST_COMPLETE
			g_pData->GetUIManager()->GetUIQuestNew()->ShowUIDesc(false);
#endif //GUIDE_QUICK_QUEST_COMPLETE
		}

	
		// 새로운 아이템이 존재할 경우 인벤이 열리면 새로운 아이템 존재여부 리셋
		m_bIsNewItem = false;

#ifdef NEW_ITEM_NOTICE
		if( NULL != m_pDLGItemHighLight )
			m_pDLGItemHighLight->SetShow(true);
#endif //NEW_ITEM_NOTICE

		//도움말 입력
		if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
		{	//인벤토리 열었을 때 장비 획득 가이드 끄기
			g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_OBTAIN_EQUIP_ITEM, false);
		}
#ifdef REFORM_QUEST
		//퀵퀘스트 완료 파티클이 UI를 가리면 제거하도록 수정
		if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
		{
			g_pData->GetUIManager()->GetUIQuestNew()->ClearQuestCompleteParticle();
		}
#endif //REFORM_QUEST
		SetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED );
		SetAPString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint );
	}
	else	// 꺼질 때 처리해야 할 부분
	{
#ifdef SPT_NONE_ITEM_DELETE
		if(m_pSptNoneItemDelete != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pSptNoneItemDelete, NULL, false );
			m_pSptNoneItemDelete = NULL;
			m_bIsSptNoneItem = false;	
		}
#endif //SPT_NONE_ITEM_DELETE

		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			pItemSlot->LostItemUI();
			pItemSlot->SetShow( false );
			pItemSlot->SetEnable( false );
		}

		// 아이템 마우스 오버 이미지를 지우자
		if(m_pDLGSelectedItem != NULL) m_pDLGSelectedItem->SetShow( false ); 
		InvalidSlotDesc();


		// 커서를 원래대로 바꿔주자
		CX2State* pState = (CX2State*)g_pMain->GetNowState();	// DownCast니까 안심
		if(pState != NULL)
		{
			CX2Cursor* pCursor = pState->GetCursor();
			if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
			{
				pCursor->ChangeCursorState(CX2Cursor::XCS_NORMAL);
			}
		}			

		//혹시 존재하고 있을 수 있는 서브 다이얼로그를 모두 지워주자
		// 주의 : 개인상점은 안 지운다
		SAFE_DELETE_DIALOG(m_pDLGResolveItem);
		SAFE_DELETE_DIALOG(m_pDLGResolveItemResult);
		SAFE_DELETE_DIALOG(m_pDLGSocketItem);
		SAFE_DELETE_DIALOG(m_pDLGSocketItemPushOk);
		SAFE_DELETE_DIALOG(m_pDLGSocketItemCheat);
		SAFE_DELETE_DIALOG(m_pDLGCubeCheck);
		SAFE_DELETE_DIALOG(m_pDLGElChanger );
		SAFE_DELETE_DIALOG(m_pDLGElChangerResult);
		SAFE_DELETE_DIALOG(m_pDLGRandomItem);

#ifdef ATTRACTION_ITEM_TEST
		SAFE_DELETE_DIALOG(m_pDLGOpenAttraction);
		SAFE_DELETE_DIALOG(m_pDLGAttractionResult);
		SAFE_DELETE_DIALOG(m_pDLGGetAttractionItemBG);
#endif
#ifdef CUBE_OPEN_IMAGE_MANAGER
		SAFE_DELETE_DIALOG(m_pDLGEventCubeResultImage);		
#endif // CUBE_OPEN_IMAGE_MANAGER

		//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
		SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
#endif REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

#ifdef SERV_PSHOP_AGENCY
		SAFE_DELETE_DIALOG( m_pDLGCheckPShop );
#endif

#ifdef FINALITY_SKILL_SYSTEM //JHKang
		SAFE_DELETE_DIALOG( m_pDLGElBreak );
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_SOCKET_NEW
		// oasis907 : 김상윤 [2010.4.5] // 
		SAFE_DELETE(m_pUISocketItem);
#endif SERV_SOCKET_NEW

#ifdef RANDOM_CUBE_UI_NEW
		SAFE_DELETE_DIALOG(m_pDLGCubeOpenEffect);
		m_fCubeOpenTime = 0.f;
		m_bCubeJustOpen = false;
#ifdef RANDOM_EXCHANGE_RESULT_VIEW
		m_eItemObtainResultType = IORT_NONE;
#else
		m_bCubeOpen = false;
#endif RANDOM_EXCHANGE_RESULT_VIEW
#endif RANDOM_CUBE_UI_NEW

#ifdef SERV_PET_SYSTEM
		// oasis907 : 김상윤 [2010.9.9] // 
		if(m_pUIPetInventory != NULL && m_pUIPetInventory->GetShow() == true)
		{
			m_pUIPetInventory->SetShow(false);
		}
#endif SERV_PET_SYSTEM

		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_INVEN);		

		if( m_pDLGWarpDestination != NULL )
			m_pDLGWarpDestination->SetShowEnable(m_bShow, m_bShow);

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		if( m_pDLGUSCJob != NULL )
			m_pDLGUSCJob->SetShowEnable(m_bShow, m_bShow);
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef NEW_ITEM_NOTICE
		if( NULL != m_pDLGItemHighLight )
			m_pDLGItemHighLight->SetShow(false);
#endif //NEW_ITEM_NOTICE

#ifdef GOOD_ELSWORD //JHKang
		SAFE_DELETE_DIALOG( m_pDLGBuyConfirm )
#endif //GOOD_ELSWORD
	}

	m_pDLGUIInventory->SetShowEnable(m_bShow, m_bShow);	
	//g_pKTDXApp->SkipFrame();
}

//{{ kimhc // 2009-08-10 // 은행 슬롯 확장
#ifdef	PRIVATE_BANK
bool CX2UIInventory::Handler_EGS_EXPAND_BANK_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )		// 은행 슬롯 확장
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXPAND_BANK_SLOT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2User*	  pUser		 = g_pData->GetMyUser();
	if( pUser == NULL )
	{
		ASSERT( !"User is NULL(X2User)" );
		return false;
	}

	CX2Unit*	  pUnit		 = pUser->GetSelectUnit();
	if( pUnit == NULL)
	{
		ASSERT( !"Unit is NULL(X2Unit)" );
		return false;
	}

	CX2Inventory& kInventory = pUnit->AccessInventory();

	map< int, int >::iterator mit;
	for ( mit = kEvent.m_mapExpandedCategorySlot.begin(); mit != kEvent.m_mapExpandedCategorySlot.end(); mit++ )
	{
		kInventory.SetItemMaxNum( (CX2Inventory::SORT_TYPE)mit->first, 
			kInventory.GetItemMaxNum((CX2Inventory::SORT_TYPE)mit->first) + mit->second );

		switch( static_cast< CX2Inventory::SORT_TYPE >( mit->first ) )
		{
#ifdef	REALTIME_EXPAND_QUICK_SLOT  
		case CX2Inventory::ST_E_QUICK_SLOT:
			if( NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
			{
				g_pData->GetUIManager()->GetUIQuickSlot()->SetExpandQuickSlot( kInventory.GetItemMaxNum( CX2Inventory::ST_E_QUICK_SLOT ) );	// 맥스 슬롯 올리고
			}  
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_16083 ),g_pMain->GetNowState() );
			break;
#endif  REALTIME_EXPAND_QUICK_SLOT
#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
		case CX2Inventory::ST_SPECIAL:
			ChangeInventoryTab( CX2Inventory::ST_SPECIAL );
			break;
#endif SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
		case CX2Inventory::ST_BANK:
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_3868 ),g_pMain->GetNowState() );
			break;
		}
	}

	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest(); 
	}
	return true;
}
#endif	PRIVATE_BANK	
//}} kimhc // 2009-08-10 // 은행 슬롯 확장

bool CX2UIInventory::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem )
{
	if ( pFromCX2SlotItem == NULL || pToCX2SlotItem == NULL )
		return false;

	if ( pFromCX2SlotItem->GetSortType() == pToCX2SlotItem->GetSortType() && pFromCX2SlotItem->GetSlotID() == pToCX2SlotItem->GetSlotID() )
		return false;

	//{{ 패션 악세사리 장착시 오류 해결을 위한 예외 처리 코드 : 장착중인 아이템을 우선 벗겨버린다

	if( ( pFromCX2SlotItem->GetSortType() == CX2Inventory::ST_E_EQUIP && pToCX2SlotItem->GetSortType() != CX2Inventory::ST_E_EQUIP ) || 
		( pFromCX2SlotItem->GetSortType() != CX2Inventory::ST_E_EQUIP && pToCX2SlotItem->GetSortType() == CX2Inventory::ST_E_EQUIP ) )
	{
		const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
		CX2Item* pFromItem = kInventory.GetItem( pFromCX2SlotItem->GetItemUID(), true );
		CX2Item* pToItem = kInventory.GetItem( pToCX2SlotItem->GetItemUID(), true );
		if(pFromItem != NULL && pToItem != NULL )
		{
			const CX2Item::ItemTemplet* pFromItemTemplet = pFromItem->GetItemTemplet();
			const CX2Item::ItemTemplet* pToItemTemplet = pToItem->GetItemTemplet();
			if(pFromItemTemplet != NULL && pToItemTemplet != NULL )
			{
				if( pFromItemTemplet->GetItemType() == CX2Item::IT_ACCESSORY &&
					pFromItemTemplet->GetEqipPosition() == pToItemTemplet->GetEqipPosition() &&
					pFromItemTemplet->GetFashion() != pToItemTemplet->GetFashion() )
				{
					// 장착중인 아이템을 우선 벗겨버린 뒤에 원래 하려던 교환을 계속 실행한다

					CX2SlotItem* pEquippedItemSlot = NULL;

					if(pFromCX2SlotItem->GetSortType() == CX2Inventory::ST_E_EQUIP )
					{
						pEquippedItemSlot = pFromCX2SlotItem;
					}
					else if( pToCX2SlotItem->GetSortType() == CX2Inventory::ST_E_EQUIP )
					{
						pEquippedItemSlot = pToCX2SlotItem;
					}


					// 장착된 아이템을 벗긴다
					{
						CX2Inventory::SORT_TYPE NowInventorySortType = kInventory.GetSortTypeByID( pEquippedItemSlot->GetItemTID() );
						for ( int i = 0; i < kInventory.GetItemMaxNum( NowInventorySortType ); i++ )
						{
							// 빈자리 찾아서
							CX2Item* pItem = kInventory.GetItem( NowInventorySortType, i );
							if ( pItem == NULL )
							{
								Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pEquippedItemSlot->GetSortType(), pEquippedItemSlot->GetSlotID(),
									NowInventorySortType, i );

								// 이전 슬롯은 지워주고
								pEquippedItemSlot->DestroyItemUI();
								break;
							}
						}
					}
				}
				// 장착된 아이템은 벗겼고
			}

		}

	}
	//}}

	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ;

	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType	= pFromCX2SlotItem->GetSortType();
	if ( pFromCX2SlotItem->GetSlotType() == CX2Slot::ST_INVENTORY )
	{

#ifdef SERV_REFORM_INVENTORY_TEST
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID();
#else SERV_REFORM_INVENTORY_TEST
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID() + (( m_NowInvenSortTypePageNum - 1 ) * MY_INVENTORY_SHOW_MAX_SLOT_NUM);
#endif SERV_REFORM_INVENTORY_TEST

		//{{ kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체
#ifdef REAL_TIME_ELSWORD
		if ( pFromCX2SlotItem->GetSortType() == CX2Inventory::ST_QUICK_SLOT && 
			pToCX2SlotItem->GetSortType() == CX2Inventory::ST_E_QUICK_SLOT )
		{
			g_pData->GetUIManager()->GetUIQuickSlot()->UpdateCoolTime( pFromCX2SlotItem->GetItemTID() );
		}		
#endif REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체
	}
	else
	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID();
	}


	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType	= pToCX2SlotItem->GetSortType();

#ifndef	SERV_REFORM_INVENTORY_TEST
	if ( pToCX2SlotItem->GetSlotType() == CX2Slot::ST_INVENTORY )
	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= pToCX2SlotItem->GetSlotID() + (( m_NowInvenSortTypePageNum - 1 ) * MY_INVENTORY_SHOW_MAX_SLOT_NUM);
	}
	else
#endif	SERV_REFORM_INVENTORY_TEST

	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= pToCX2SlotItem->GetSlotID();
	}
#ifdef SERV_SHARING_BANK_TEST
	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iShareUnitUID = -1;
	if( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIPrivateBank() != NULL )
	{
		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iShareUnitUID = g_pData->GetUIManager()->GetUIPrivateBank()->GetShareUnitUID();
	}
	if( (kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType == CX2Inventory::ST_SHARE_BANK && kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType != CX2Inventory::ST_SHARE_BANK) || 
		(kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType != CX2Inventory::ST_SHARE_BANK && kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType == CX2Inventory::ST_SHARE_BANK) )
	{
		CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
		CX2Item* pFromItem = kInventory.GetItem( pFromCX2SlotItem->GetItemUID(), true );
		CX2Item* pToItem = kInventory.GetItem( pToCX2SlotItem->GetItemUID(), true );

		// 거래 가능한지 체크 합시다.
		if(pFromItem != NULL || pToItem != NULL)
		{
			if(CheckCanMoveShareItem(pFromItem, pToItem, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ) == false)
			{
				return false;
			}
		}
	}
#endif SERV_SHARING_BANK_TEST
	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK );

	return true;
}


bool CX2UIInventory::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID )
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
	if( (kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType == CX2Inventory::ST_SHARE_BANK && kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType != CX2Inventory::ST_SHARE_BANK) || 
		(kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType != CX2Inventory::ST_SHARE_BANK && kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType == CX2Inventory::ST_SHARE_BANK) )
	{
		CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
		CX2Item* pFromItem = kInventory.GetItem( fromSortType, fromSlotID );
		CX2Item* pToItem = kInventory.GetItem( toSortType, toSlotID );
		
		// 거래 가능한지 체크 합시다.
		if(pFromItem != NULL || pToItem != NULL)
		{
			if(CheckCanMoveShareItem(pFromItem, pToItem, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ) == false)
			{
				return false;
			}
		}
	}
#endif SERV_SHARING_BANK_TEST

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_CHANGE_INVENTORY_SLOT_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK ) == true )
	{
		//{{ kimhc // 실시간 엘소드 중 실시간 장비 및 아바타 교체
#ifdef REAL_TIME_ELSWORD
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_BATTLE_FIELD:
		case CX2Main::XS_TRAINING_GAME:
			{
				if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
					return false;
				
				CX2GUUser* pMyUser = g_pX2Game->GetMyUnit();

				if ( NULL == pMyUser || 
					 NULL == pMyUser->GetUnit() )
					 return false;

				bool bWeaponChanged = false;
				BOOST_TEST_FOREACH( KInventoryItemInfo&, kInventoryItemInfo, kEvent.m_vecInventorySlotInfo )	// 변경된 장비들
				{
					if ( kInventoryItemInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
					{
						CX2GUUser*	pUser			= g_pX2Game->GetMyUnit();
						CX2Item*	pItemToAdd		= pUser->GetUnit()->GetInventory().GetItem( kInventoryItemInfo.m_iItemUID );	// 장착한 장비
						CX2Item*	pItemToRemove	= pUser->GetUnit()->GetInventory().GetItem( 
							static_cast< CX2Inventory::SORT_TYPE >( kInventoryItemInfo.m_cSlotCategory ), 
							kInventoryItemInfo.m_sSlotID );

						if ( pItemToRemove != NULL )
							g_pX2Game->RemoveSpecialAbilityInEquip( pItemToRemove->GetItemTemplet(), pUser );

						if ( pItemToAdd != NULL )
							g_pX2Game->SetSpecialAbilityInEquip( pItemToAdd->GetItemTemplet(), pUser );

						g_pX2Game->GetMyUnit()->UpdateEquipViewListInfo( pItemToAdd, pItemToRemove );

						if ( kInventoryItemInfo.m_sSlotID == _CONST_UIINVENTORY_::g_iEquippedNormalWeaponSlotID ||
							 kInventoryItemInfo.m_sSlotID == _CONST_UIINVENTORY_::g_iEquippedFashionWeaponslotID )
						{
							bWeaponChanged = true;
						} // if

						break;
					}
				}

				UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );

				//{{ kimhc // 2010.12.11 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
				pMyUser->SetShowViewList( true );
#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
				pMyUser->UpdateNotRenderingOtherEquipList();
#endif	NOT_RENDERING_OTHER_EQUIP_POS
				pMyUser->UpdateEquipCollisionData();

				pMyUser->SetEnableAttackBox( L"Rfoot", false );
				pMyUser->SetEnableAttackBox( L"Lfoot", false );
				pMyUser->SetEnableAttackBox( L"Lhand2", false );		
#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.12.11 //  2010-12-23 New Character CHUNG


				pMyUser->UpdateWeaponEnchantEffectAndExtraDamageType();
			}
			break;

		default:
			{
				UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );

#ifdef FIX_WRONG_EQUIP
				if ( g_pSquareGame != NULL )
				{
					CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
					if ( pSquareUnit != NULL )
					{
						pSquareUnit->GetUnitViewer()->UpdateEqip( true );
					}
				}
				else if( NULL != g_pTFieldGame )
				{
					CX2SquareUnit* pSquareUnit = g_pTFieldGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
					if ( pSquareUnit != NULL )
					{
						pSquareUnit->GetUnitViewer()->UpdateEqip( true );
					}
				}
				if ( g_pX2Room != NULL )
				{
					CX2Room::SlotData* pSlotData = g_pX2Room->GetMySlot();
					if ( pSlotData != NULL && pSlotData->m_pUnitViewer != NULL )
					{
						pSlotData->m_pUnitViewer->UpdateEqip( true );
					}
				}
#endif FIX_WRONG_EQUIP
			}
			break;
		}
#else	REAL_TIME_ELSWORD
		UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
#endif	REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 실시간 장비 및 아바타 교체

		if(g_pData->GetUIManager()->GetUIQuickSlot() != NULL)
			g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();

		// oasis907 : 김상윤 [2010.11.17] // 인벤토리(펫, 은행, 인벤) 간 퀘스트 관련 아이템 교환시 퀵 퀘스트 창 실시간 반영
		g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();

		return true;
	}

	return false;
}

//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
bool CX2UIInventory::Handler_EGS_SORT_CATEGORY_ITEM_REQ(CX2Inventory::SORT_TYPE NowInventorySortType)
{
	KEGS_SORT_CATEGORY_ITEM_REQ kEGS_SORT_CATEGORY_ITEM_REQ;
	kEGS_SORT_CATEGORY_ITEM_REQ.m_iCategoryType = NowInventorySortType;

	g_pData->GetServerProtocol()->SendPacket( EGS_SORT_CATEGORY_ITEM_REQ, kEGS_SORT_CATEGORY_ITEM_REQ);
	g_pMain->AddServerPacket( EGS_SORT_CATEGORY_ITEM_ACK );

	return true;
}
#endif SERV_SORT_CATEGORY_ITEM 
//}}

bool CX2UIInventory::Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ITEM_EXPIRATION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == kEvent.m_vecItemUID.empty() )
	{
		StateLog( L"ExpiredItemEmpty" );
		return true;
	}


	//{{ kimhc // 실시간 엘소드 중 실시간 장비 및 아바타 교체
#ifdef	REAL_TIME_ELSWORD

	if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
	{

		if ( g_pX2Game == NULL ||
			g_pX2Game->GetMyUnit() == NULL ||
			g_pX2Game->GetMyUnit()->GetUnit() == NULL )
			return false;

		CX2GUUser*					pMyUser = g_pX2Game->GetMyUnit();
		CX2Unit*					pUnit	= pMyUser->GetUnit();

		if ( kEvent.m_vecItemUID.empty() == false )
		{
			wstring tempString = L"";

			for ( int i = 0; i < (int)kEvent.m_vecItemUID.size(); i++ )
			{
				UidType		itemUID = kEvent.m_vecItemUID[i];
				CX2Item*	pItem	= pUnit->GetInventory().GetItem( itemUID );

				if ( pItem == NULL ||
					 pItem->GetItemTemplet() == NULL 
                     )
					continue;

				if ( pUnit->GetInventory().CheckEquippingItem( 
                    pItem->GetItemTemplet()->GetItemID()
#ifdef ITEM_EXPIRE_USING_ITEM_UID
					, pItem->GetUID()
#endif ITEM_EXPIRE_USING_ITEM_UID
                    ) == true )
				{
					g_pX2Game->RemoveSpecialAbilityInEquip( pItem->GetItemTemplet(), pMyUser );
					pMyUser->UpdateEquipViewListInfo( NULL, pItem );
				}
			}
		}

		//{{ kimhc // 2010.12.11 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	
		pMyUser->SetShowViewList( true );
	#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		pMyUser->UpdateNotRenderingOtherEquipList();
	#endif	NOT_RENDERING_OTHER_EQUIP_POS
		pMyUser->UpdateEquipCollisionData();

		pMyUser->SetEnableAttackBox( L"Rfoot", false );
		pMyUser->SetEnableAttackBox( L"Lfoot", false );
		pMyUser->SetEnableAttackBox( L"Lhand2", false );		
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.11 //  2010-12-23 New Character CHUNG
	}

#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 실시간 장비 및 아바타 교체

	wstring wstrExpirationMessage = L"";

	for ( std::vector<UidType>::iterator it = kEvent.m_vecItemUID.begin(); it < kEvent.m_vecItemUID.end(); it++ )
	{
		UidType itemUID = *it;



		// 1. 아이템을 지우기전에 어떤 아이템이 지워지는지 이름 먼저~
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID, true );
		if( NULL != pItem )
		{
			wstrExpirationMessage += L"[";
			wstrExpirationMessage += pItem->GetFullName();
			wstrExpirationMessage += L"] ";
		}




		// 2. unitviewer에서 장착된 Equip들을 제거하고, ( 거래광장, 마을, 대기방, 내캐릭정보창, 캐시샵, 서버선택창의 unitviewerui )
		if ( g_pSquareGame != NULL )
		{
			CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
			if ( pSquareUnit != NULL )
			{
				pSquareUnit->GetUnitViewer()->RemoveEqip( itemUID );
			}
		}
		else if( NULL != g_pTFieldGame )
		{
			CX2SquareUnit* pSquareUnit = g_pTFieldGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
			if ( pSquareUnit != NULL )
			{
				pSquareUnit->GetUnitViewer()->RemoveEqip( itemUID );
			}
		}

		if ( g_pX2Room != NULL )
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetMySlot();
			if ( pSlotData != NULL && pSlotData->m_pUnitViewer != NULL )
			{
				pSlotData->m_pUnitViewer->RemoveEqip( itemUID );
			}
		}

		if( NULL != g_pData->GetUIManager()->GetUICharInfo() &&
			NULL != g_pData->GetUIManager()->GetUICharInfo()->GetUnitViewerUI() )
		{
			g_pData->GetUIManager()->GetUICharInfo()->GetUnitViewerUI()->RemoveEqip( itemUID );
		}


		if( NULL != g_pData->GetCashShop() &&
			NULL != g_pData->GetCashShop()->GetUnitViewerUI() )
		{
			g_pData->GetCashShop()->GetUnitViewerUI()->RemoveEqip( itemUID );
		}

#ifdef ELSWORD_NEW_BEGINNING
		if( CX2Main::XS_BEGINNING == g_pMain->GetNowStateID() )
#else
		if( CX2Main::XS_SERVER_SELECT == g_pMain->GetNowStateID() )
#endif ELSWORD_NEW_BEGINNING
		{
#ifdef ELSWORD_NEW_BEGINNING
			CX2StateBeginning* pState = (CX2StateBeginning*) g_pMain->GetNowState();
#else
			CX2StateServerSelect* pState = (CX2StateServerSelect*) g_pMain->GetNowState();
#endif ELSWORD_NEW_BEGINNING
			if( NULL != pState )
			{
				if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					CX2UnitViewerUI* pUnitViewer = pState->GetUnitViewer( g_pData->GetMyUser()->GetSelectUnit() );
					if( NULL != pUnitViewer )
					{
						pUnitViewer->RemoveEqip( itemUID );
					}
				}
			}
		}


		// 3. 맨 마지막으로 유닛 정보와 인벤토리에서 아이템을 제거한다.
		g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( itemUID );
		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().RemoveItem( itemUID );



	} // for()



	if ( wstrExpirationMessage.empty() == false )
	{
		//{{ kimhc // 기간이 만료됬다는 알림을 채팅 창으로 변경
		if( g_pChatBox != NULL)
		{
			std::wstringstream strStream;
			strStream << GET_REPLACED_STRING( ( STR_ID_195, "L", wstrExpirationMessage ) );

			wstring wstrColor = L"#CECEC88";							// (236, 236, 136)
			D3DXCOLOR coTextColor(0.92549f, 0.92549f, 0.53333f, 1.f);	// (236, 236, 136)

			g_pChatBox->AddChatLog( strStream.str().c_str(), KEGS_CHAT_REQ::CPT_TOTAL, coTextColor, wstrColor );

		}

		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_PVP_RESULT:
		case CX2Main::XS_DUNGEON_RESULT:
		case CX2Main::XS_TRAINING_GAME:
		case CX2Main::XS_TRAINING_RESULT:
			break;

		default:
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_195, "L", wstrExpirationMessage ) ), g_pMain->GetNowState() );
			break;
		}
		//}} kimhc // 기간이 만료됬다는 알림을 채팅 창으로 변경
	}


	if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO) )
	{
		g_pData->GetUIManager()->GetUICharInfo()->ResetNowEquipUI();
		g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
	}

	if( NULL != g_pData->GetCashShop() )
	{
		g_pData->GetCashShop()->ResetUnitViewerEquip();
	}

	// 인벤토리 업데이트
	SetInventorySort( m_NowInventorySortType );

#ifdef SERV_PET_SYSTEM
	// oasis907 : 김상윤 [2010.9.16] // 펫 인벤토리에 있는 아이템 기간 만료시
	if(m_pUIPetInventory != NULL && m_pUIPetInventory->GetShow() == true)
	{
		m_pUIPetInventory->UpdateUI();
	}
#endif SERV_PET_SYSTEM

#ifdef QUEST_UPDATE_ITEM_EXPIRATION
	if( g_pData->GetUIManager() == NULL || g_pData->GetUIManager()->GetUIQuestNew() == NULL )
	{
		return true;
	}

	if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST_NEW ) == true )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->ResetQuestUI();
	}

	if( g_pData->GetUIManager()->GetUIQuestNew()->GetShowQuickQuestDLG() == true )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
	}
#endif // QUEST_UPDATE_ITEM_EXPIRATION

	return true;
}










bool CX2UIInventory::UpdateInventorySlot()
{

#ifdef SERV_REFORM_INVENTORY_TEST
	return SetInventorySort(m_NowInventorySortType, GetNowInvenSortTypePageNum());
#else  SERV_REFORM_INVENTORY_TEST
	return SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum );
#endif SERV_REFORM_INVENTORY_TEST

}


bool CX2UIInventory::UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo, bool bInventoryUIUpdate /*= true*/ )
{

	CX2Unit*		pMyUnit = g_pData->GetMyUser()->GetSelectUnit();

	ASSERT( pMyUnit != NULL );
	if( pMyUnit == NULL )
	{
		StateLog( L"updating inven failed - null unit" );		
		return false;
	}

	CX2Inventory&	kMyInventory = pMyUnit->AccessInventory();

	//{{ mauntain : 김태환 [2012.07.10] 장착중인 소비성 아이템 획득시 퀵슬롯 갱신
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	bool			bUpdateQuickSlot = false;		/// 퀵슬롯 갱신 여부
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}



	BOOST_TEST_FOREACH( const KInventoryItemInfo&, invenItemInfo, vecInventorySlotInfo )
	{
		if( invenItemInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
		{
			// 장착 장비 제거
			CX2Item* pItem = kMyInventory.GetItem( (CX2Inventory::SORT_TYPE)invenItemInfo.m_cSlotCategory, invenItemInfo.m_sSlotID );
			if ( pItem != NULL )
			{
				pMyUnit->RemoveEqip( pItem->GetUID() );
			}
		}

		// 인벤에서 아이템 제거
		kMyInventory.RemoveItem( (CX2Inventory::SORT_TYPE) invenItemInfo.m_cSlotCategory, invenItemInfo.m_sSlotID );
	}

#ifdef SERV_REFORM_INVENTORY_TEST
	bool bIsUpdate = false;
#endif SERV_REFORM_INVENTORY_TEST

	BOOST_TEST_FOREACH( const KInventoryItemInfo&, invenItemInfo, vecInventorySlotInfo )
	{
		if( invenItemInfo.m_iItemUID <= 0 )
			continue;

		// 인벤에 아이템 다시 추가
        {
		    CX2Item::ItemData kItemData( invenItemInfo );
		    kMyInventory.AddItem( (CX2Inventory::SORT_TYPE)invenItemInfo.m_cSlotCategory, invenItemInfo.m_sSlotID, kItemData );
        }

#ifdef SERV_REFORM_INVENTORY_TEST
		if( (CX2Inventory::SORT_TYPE)invenItemInfo.m_cSlotCategory == m_NowInventorySortType )
		{
			SetNowInventoryPageNum( m_NowInventorySortType, GetPageNumBySlotID(invenItemInfo.m_sSlotID) );
			bIsUpdate = true;
		}
#endif SERV_REFORM_INVENTORY_TEST

		// 장착 장비 다시 추가
		if ( invenItemInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
		{
			CX2Item* pItem = kMyInventory.GetItem( (CX2Inventory::SORT_TYPE)invenItemInfo.m_cSlotCategory, invenItemInfo.m_sSlotID );
			if ( pItem != NULL )
			{
				pMyUnit->AddEqip( pItem->GetUID() );
			}
		}

		//{{ mauntain : 김태환 [2012.07.10] 장착중인 소비성 아이템 획득시 퀵슬롯 갱신
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		else if( invenItemInfo.m_cSlotCategory == CX2Inventory::ST_E_QUICK_SLOT )
		{
			if( bUpdateQuickSlot == false )
				bUpdateQuickSlot = true;		/// 퀵슬롯 갱신 설정
		}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}
	}

#ifdef SERV_REFORM_INVENTORY_TEST
	if( bIsUpdate == true )
	{
		if( UpdatePageUI( GetNowInventoryPageNum( m_NowInventorySortType) ) == true )
		{
			SetInventorySort( m_NowInventorySortType, GetNowInventoryPageNum( m_NowInventorySortType ) );
		}
	}
#endif SERV_REFORM_INVENTORY_TEST


	// 내 정보창 갱신
	if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
	{
		g_pData->GetUIManager()->GetUICharInfo()->EquipChanged();
	}

	//{{ kimhc // 2009-08-07 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	if(g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_PRIVATE_BANK) == true )
	{
		g_pData->GetUIManager()->GetUIPrivateBank()->UpdateUI();
	}
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-07 // 캐릭터별 은행

#ifdef SERV_PET_SYSTEM
	//{{ oasis907 : 김상윤 [2010.9.6] // 
	if(m_pUIPetInventory != NULL && m_pUIPetInventory->GetShow() == true)
	{
		m_pUIPetInventory->UpdateUI();
	}
	//}}
#endif SERV_PET_SYSTEM

	//{{ mauntain : 김태환 [2012.07.10] 장착중인 소비성 아이템 획득시 퀵슬롯 갱신
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	if( true == bUpdateQuickSlot )		/// 퀵슬롯 갱신
	{
		if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
			g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();
	}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

	g_pMain->GetInformerManager()->GetInvenInformer()->SetNeedCheckInfo( true );

	// 인벤토리 업데이트
	if( true == bInventoryUIUpdate )
	{

#ifdef SERV_REFORM_INVENTORY_TEST
		if ( SetInventorySort( m_NowInventorySortType, GetNowInvenSortTypePageNum() ) == false )
#else  SERV_REFORM_INVENTORY_TEST
		if ( SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum ) == false )
#endif SERV_REFORM_INVENTORY_TEST
			return false;
	}


	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}

#ifdef SERV_NEW_YEAR_EVENT_2014
	g_pMain->GetMemoryHolder()->UpdateNewYear2014Event();
#endif SERV_NEW_YEAR_EVENT_2014

	return true;
}




bool CX2UIInventory::ChangeInventoryTab( CX2Inventory::SORT_TYPE sortType )
{
	return SetInventorySort( sortType );
}

bool CX2UIInventory::SetInventorySort( CX2Inventory::SORT_TYPE sortType, int pageNum )
{
	if ( m_bShow == false )
		return false;

	if ( CheckRadioButtonBySortType( sortType ) == false )
		return false;

	m_NowInventorySortType = sortType;	


	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

#ifndef	SERV_REFORM_INVENTORY_TEST
	int maxPageNum = kInventory.GetItemMaxNum( m_NowInventorySortType )/MY_INVENTORY_SHOW_MAX_SLOT_NUM;
	if ( kInventory.GetItemMaxNum( m_NowInventorySortType )%MY_INVENTORY_SHOW_MAX_SLOT_NUM > 0 )
		maxPageNum++;

	ResetInvenPage( pageNum, maxPageNum );
#endif	SERV_REFORM_INVENTORY_TEST

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
		{
// 			if(true == pItemSlot->IsCreatedItemUI())
// 			{
// 				pItemSlot->LostItemUI();
// 			}
			pItemSlot->SetShow( false );
			pItemSlot->SetEnable( false );
			pItemSlot->SetSortType( sortType );
		}

	}

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	if ( g_pData->GetUIManager() != NULL &&													
		g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true )
	{
		g_pData->GetUIManager()->GetUIResolveItem()->OffResolveCheckPicture();
	}
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	if ( g_pData->GetUIManager() != NULL &&													
		g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true )
	{
		g_pData->GetUIManager()->GetUISynthesisItem()->OffSynthesisCheckPicture();
	}
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef	SERV_REFORM_INVENTORY_TEST
	/// 해당 인벤토리 카테고리의 최대 페이지 수 연산
	int iNowPageSlotNum = GetMaxLineNumPerCategory( m_NowInventorySortType );
	iNowPageSlotNum -= ( ( pageNum - 1 ) * MY_INVENTORY_MAX_SLOT_HEIGHT );

	for( int i = 0; i < MY_INVENTORY_SHOW_SLOT_NUM_PER_PAGE / MY_INVENTORY_MAX_SLOT_WIDTH; i++ )
	{
		WCHAR buff[256] = {0,};

		StringCchPrintf( buff, 256, L"g_pStaticInvenSlot%d", i+1);
		CKTDGUIStatic* pSlotLine = (CKTDGUIStatic*) m_pDLGUIInventory->GetControl( buff );
		if(NULL != pSlotLine)
		{
			if( i < iNowPageSlotNum )
				pSlotLine->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
			else
				pSlotLine->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}
	}

#ifdef NEW_ITEM_NOTICE 
	CKTDGUIStatic* pStatic = NULL; 
	if( m_pDLGItemHighLight != NULL )
	{
		pStatic = (CKTDGUIStatic*)m_pDLGItemHighLight->GetControl(L"g_pStaticInvenHighLight");
	}
	if( NULL != pStatic )
	{
#ifdef SERV_REFORM_INVENTORY_TEST
		for ( int slotIndex = 0; slotIndex < MY_INVENTORY_SHOW_SLOT_NUM_PER_PAGE; ++slotIndex)
#else
		for ( int slotIndex = 0; slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM; ++slotIndex)
#endif //SERV_REFORM_INVENTORY_TEST
		{//New Item 하이라이트 모두 끄기
			if( NULL != pStatic->GetPicture(slotIndex) )
			{
				pStatic->GetPicture(slotIndex)->SetShow(false);
			}
		}
	}
#endif //NEW_ITEM_NOTICE 	

	int iLineIndex = 0;
	// slotID : Inventory 상의 실제 ID
	// slotIndex : UI상의 슬롯
	for ( int slotID = ((pageNum-1)*MY_INVENTORY_SHOW_SLOT_NUM_PER_PAGE), slotIndex = 0; 
		slotID < kInventory.GetItemMaxNum( m_NowInventorySortType ) && slotIndex < MY_INVENTORY_SHOW_SLOT_NUM_PER_PAGE;
		slotID++, slotIndex++ )
	{
		//CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(slotIndex);
		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(slotID);
		if ( pItemSlot == NULL )
			continue;		

		if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
		{
			pItemSlot->SetShow( true, false );
			pItemSlot->SetEnable( true, false );

			if(slotIndex == iLineIndex*MY_INVENTORY_MAX_SLOT_WIDTH )
			{
				WCHAR buff[256] = {0,};
				//wsprintf( buff, L"g_pStaticInvenSlot%d", iLineIndex+1);
				StringCchPrintf( buff, 256, L"g_pStaticInvenSlot%d", iLineIndex+1);
				CKTDGUIStatic* pSlotLine = (CKTDGUIStatic*) m_pDLGUIInventory->GetControl( buff );
				if(NULL != pSlotLine)
				{
					pSlotLine->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}				
				iLineIndex++;
			}
		}

#ifdef NEW_ITEM_NOTICE 
		CX2Item* pItem = kInventory.GetItem( m_NowInventorySortType, slotID );

		if( NULL != pItem )
		{
			UidType iItemUID = pItem->GetItemData().m_ItemUID;
			bool	bNewItem = kInventory.IsNewItem(m_NowInventorySortType, iItemUID );

			if( true == bNewItem && NULL != pStatic &&  NULL != pStatic->GetPicture(slotIndex) )
			{//위에서 모두 초기화 시켜 주기 때문에, true일때만 수정
				pStatic->GetPicture(slotIndex)->SetShow(bNewItem);
			}
		}
#endif //NEW_ITEM_NOTICE 	

	}

	for ( int slotIndex = 0; slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM; slotIndex++ )
	{
		CX2Item* pItem = kInventory.GetItem( m_NowInventorySortType, slotIndex );
		for ( int j = 0; j < (int)m_SlotList.size(); j++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
			if ( pItemSlot == NULL )
				continue;

			if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlot->GetSlotID() == slotIndex )
			{
				if(pItem != NULL)
				{
					if ( pItemSlot->IsCreatedItemUI() == true )
						pItemSlot->ResetItemUI( pItem );
					else
						pItemSlot->CreateItemUI( pItem );

					CKTDGUIDialogType pkDiag = pItemSlot->GetDialog();
					//	현재 페이지가 아니고 드래깅하고있는 아이템이 아니면 보이지 않게 처리한다.
					if( GetPageNumBySlotID( slotIndex ) != pageNum && *m_pSlotBeforeDragging != pItemSlot )
					{
						pkDiag->SetShow(false);
					}
				}
				else
				{
					pItemSlot->DestroyItemUI();
				}			

				break;
			}
		}
	}

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	if ( g_pData->GetUIManager() != NULL &&													
		g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true )
	{
		g_pData->GetUIManager()->GetUIResolveItem()->ShowResolveCheckPicture( m_NowInventorySortType );
	}
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	if ( g_pData->GetUIManager() != NULL &&													
		g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true )
	{
		g_pData->GetUIManager()->GetUISynthesisItem()->ShowSynthesisCheckPicture( m_NowInventorySortType );
	}
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef NEW_ITEM_NOTICE 
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGItemHighLight, true );	
#endif //NEW_ITEM_NOTICE 

	/// 인벤토리 최대 페이지 수 연산
	int iMaxPageNum = GetMaxLineNumPerCategory( m_NowInventorySortType ) / MY_INVENTORY_MAX_SLOT_HEIGHT;	/// 슬롯( 56개 )으로 꽉 찬 페이지 수 연산

	if( 0 < GetMaxLineNumPerCategory( m_NowInventorySortType ) % MY_INVENTORY_MAX_SLOT_HEIGHT )				/// 이외의 꽉 채우지 못한 페이지가 있는지 연산
		++iMaxPageNum;	/// 페이지 수 추가

	SetShowInventoryPageUI( iMaxPageNum );	/// 페이지 버튼 표시 처리
#else	SERV_REFORM_INVENTORY_TEST

	// 사용되지 않는 슬롯은 뒤의 그림도 나타나지 않게 한다.
	CKTDGUIStatic* pSlotLine1 = (CKTDGUIStatic*) m_pDLGUIInventory->GetControl( L"g_pStaticInvenSlot1" );
	pSlotLine1->SetShow(false);
	CKTDGUIStatic* pSlotLine2 = (CKTDGUIStatic*) m_pDLGUIInventory->GetControl( L"g_pStaticInvenSlot2" );
	pSlotLine2->SetShow(false);
	CKTDGUIStatic* pSlotLine3 = (CKTDGUIStatic*) m_pDLGUIInventory->GetControl( L"g_pStaticInvenSlot3" );
	pSlotLine3->SetShow(false);
	CKTDGUIStatic* pSlotLine4 = (CKTDGUIStatic*) m_pDLGUIInventory->GetControl( L"g_pStaticInvenSlot4" );
	pSlotLine4->SetShow(false);

	const int MAGIC_SLOT_PER_LINE = 8;	// UI상에서 한 줄에 나타나는 슬롯 수

	int iLineIndex = 0;
	// slotID : Inventory 상의 실제 ID
	// slotIndex : UI상의 슬롯
	for ( int slotID = ((pageNum-1)*MY_INVENTORY_SHOW_MAX_SLOT_NUM), slotIndex = 0; 
		slotID < kInventory.GetItemMaxNum( m_NowInventorySortType ) && slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM;
		slotID++, slotIndex++ )
	{

		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(slotIndex);
		if ( pItemSlot == NULL )
			continue;

		if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
		{
			pItemSlot->SetShow( true, false );
			pItemSlot->SetEnable( true, false );
			if(slotIndex == iLineIndex*MAGIC_SLOT_PER_LINE )
			{
				WCHAR buff[256] = {0,};
				//wsprintf( buff, L"g_pStaticInvenSlot%d", iLineIndex+1);
				StringCchPrintf( buff, 256, L"g_pStaticInvenSlot%d", iLineIndex+1);
				CKTDGUIStatic* pSlotLine = (CKTDGUIStatic*) m_pDLGUIInventory->GetControl( buff );
				if(NULL != pSlotLine)
				{
					pSlotLine->SetShow(true);
				}				
				iLineIndex++;
			}
		}

	}
#ifdef NEW_ITEM_NOTICE 
	CKTDGUIStatic* pStatic = NULL;
	if( m_pDLGItemHighLight != NULL )
	{
		pStatic = (CKTDGUIStatic*)m_pDLGItemHighLight->GetControl(L"g_pStaticInvenHighLight");
	}
	if( NULL != pStatic )
	{
		for ( int slotIndex = 0; slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM; ++slotIndex)
		{//New Item 하이라이트 모두 끄기
			if( NULL != pStatic->GetPicture(slotIndex) )
			{
				pStatic->GetPicture(slotIndex)->SetShow(false);
			}
		}
	}
#endif //NEW_ITEM_NOTICE 


	for ( int slotID = ((pageNum-1)*MY_INVENTORY_SHOW_MAX_SLOT_NUM), slotIndex = 0; 
		slotID < kInventory.GetItemMaxNum( m_NowInventorySortType ) && slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM; 
		slotID++, slotIndex++ )
	{
		CX2Item* pItem = kInventory.GetItem( m_NowInventorySortType, slotID );

#ifdef NEW_ITEM_NOTICE 
		if( NULL != pItem )
		{
			UidType iItemUID = pItem->GetItemData().m_ItemUID;
			bool	bNewItem = kInventory.IsNewItem(m_NowInventorySortType, iItemUID );

			if( true == bNewItem && NULL != pStatic &&  NULL != pStatic->GetPicture(slotIndex) )
			{//위에서 모두 초기화 시켜 주기 때문에, true일때만 수정
				pStatic->GetPicture(slotIndex)->SetShow(bNewItem);
			}
		}
#endif //NEW_ITEM_NOTICE 
		for ( int j = 0; j < (int)m_SlotList.size(); j++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
			if ( pItemSlot == NULL )
				continue;

			if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlot->GetSlotID() == slotIndex )
			{
				if(pItem != NULL)
				{
					if ( pItemSlot->IsCreatedItemUI() == true )
						pItemSlot->ResetItemUI( pItem );
					else
						pItemSlot->CreateItemUI( pItem );
				}
				else
				{
					pItemSlot->DestroyItemUI();
				}			

				break;
			}
		}
	}
	
#ifdef NEW_ITEM_NOTICE 
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGItemHighLight, true );	
#endif //NEW_ITEM_NOTICE 	

#endif	SERV_REFORM_INVENTORY_TEST

	if(g_pData != NULL && g_pData->GetUIManager() != NULL)
	{
		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_INVEN);
	}
	return true;
}

bool CX2UIInventory::ChangeInventoryTabByUid( UidType iItemUID )
{
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pItem = kInventory.GetItem( iItemUID, true );

	if( pItem == NULL)
		return false;

	if( kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) == m_NowInventorySortType &&
		*m_pSlotBeforeDragging != NULL &&
		CX2SlotItem::CX2Slot::ST_INVENTORY == ((*m_pSlotBeforeDragging)->GetSlotType()) )
	{
		return true;
	}

	int invenPage = 1;

	for ( int i = 0; i < kInventory.GetItemMaxNum( kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) ); i++ )
	{
		CX2Item* pItemTemp = kInventory.GetItem( kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ), i );
		if ( pItemTemp == NULL )
		{
			int showPage = i / MY_INVENTORY_SHOW_MAX_SLOT_NUM;
			showPage++;

			if ( showPage != 1 )
				invenPage = showPage;
			break;
		}
	}

#ifdef SERV_REFORM_INVENTORY_TEST
	CX2Inventory::SORT_TYPE eSortType = kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() );

	if( eSortType == GetSortType() )
	{
		return true;
	}
	else
	{
		UpdatePageUI( GetNowInventoryPageNum( eSortType ) );						// SortType이 ST_AVATAR로 넘어가기 직전이므로 SortType 체크하지 않도록 함.
		return SetInventorySort( eSortType, GetNowInventoryPageNum( eSortType ) );		
	}
#else SERV_REFORM_INVENTORY_TEST
	return SetInventorySort( kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ), invenPage );
#endif SERV_REFORM_INVENTORY_TEST

	//	if ( kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) != m_NowInventorySortType 
	//		|| invenPage != m_NowInvenSortTypePageNum )
	//	{
	//		if ( pItem != NULL && pItem->GetItemTemplet() != NULL )
	//		{
	//			wstring radioButtonName = GetRadioButtonNameByInvenSortType( kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) );
	//			CKTDGUIRadioButton* pRadioButtonInvenSortType = (CKTDGUIRadioButton*)m_pDLGMyInfoInventory->GetControl( radioButtonName.c_str() );
	//			if ( pRadioButtonInvenSortType != NULL )
	//			{
	//				pRadioButtonInvenSortType->GetCheckedControl()->SetChecked( false );
	//				pRadioButtonInvenSortType->SetChecked( true );
	//
	//
	//
	//				SetInventorySort( kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ), invenPage );
	//			}
	//		}
	//	}

}

#ifndef	SERV_REFORM_INVENTORY_TEST

void CX2UIInventory::ResetInvenPage( int nowPage, int maxPage )
{
	m_NowInvenSortTypePageNum = nowPage;
	m_NowInvenSortTypePageMaxNum = maxPage;

	ResetInvenPageUI();
}

void CX2UIInventory::ResetInvenPageUI()
{
	//m_NowInvenSortTypePageMaxNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetInvenSlot() m_NowInventorySortType
	//int invenMaxSize = GetInvenMaxSize( m_NowInventorySortType );

	if ( m_pDLGUIInventory != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGUIInventory->GetControl( L"Inventory_Page_Num" );
		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << m_NowInvenSortTypePageNum << L"/" << m_NowInvenSortTypePageMaxNum;
			pStatic->SetString( 0, wstrstm.str().c_str() );
		}
	}

}

void CX2UIInventory::NextInvenPage()
{
	if ( m_NowInvenSortTypePageNum >= m_NowInvenSortTypePageMaxNum )
		return;

	m_NowInvenSortTypePageNum++;

	SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum );
}

void CX2UIInventory::PrevInvenPage()
{
	if ( m_NowInvenSortTypePageNum <= 1 )
		return;

	m_NowInvenSortTypePageNum--;

	SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum );
}

#endif	SERV_REFORM_INVENTORY_TEST

int CX2UIInventory::GetNowInvenSortTypePageNum()
{
#ifdef SERV_REFORM_INVENTORY_TEST
	if( CX2Inventory::ST_PC_BANG == m_NowInventorySortType )	/// PC방만 예외 처리
		return 1;
	else
		return GetNowInventoryPageNum( m_NowInventorySortType );
#else  SERV_REFORM_INVENTORY_TEST
	return m_NowInvenSortTypePageNum;
#endif SERV_REFORM_INVENTORY_TEST
}

bool CX2UIInventory::MouseDown( D3DXVECTOR2 mousePos )
{

	CX2SlotItem* pSlot = (CX2SlotItem*)GetSlotInMousePos( mousePos );

	if ( pSlot == NULL )
		return false;

	if ( pSlot->IsResetItemUI() == false )
	{
		return false;
	}

	if ( pSlot->GetSlotType() != CX2SlotItem::ST_INVENTORY )
		return false;

	CX2State* pState = (CX2State*)g_pMain->GetNowState();
	if(pState != NULL)
	{
		CX2Cursor* pCursor = pState->GetCursor();
		if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
		{
			return true;
		}
	}

	*m_pSlotBeforeDragging = pSlot;
	//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( (*m_pSlotBeforeDragging)->GetDialog(), true );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), GetDraggingItemLayer() );
	(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );

	*m_DraggingItemUID = pSlot->GetItemUID();
	ChangeInventoryTabByUid(*m_DraggingItemUID);

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );


#ifdef SERV_SOCKET_NEW
	if (m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true)
	{
		if(m_pUISocketItem->IsStoneItem(*m_DraggingItemUID))
		{
			m_pUISocketItem->SocketEquipReadyEffectStart();
		}
	}
#endif SERV_SOCKET_NEW

	return true;
}


bool CX2UIInventory::MouseUp( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pSlot = (CX2SlotItem*)GetSlotInMousePos( mousePos );
	if(pSlot == NULL)
	{
#ifdef	SERV_REFORM_INVENTORY_TEST
		//	지금 페이지가 아닌 드래깅 아이템이면 지워주자(슬롯이 아닌곳에 드롭해서 아이템을 되돌릴때)
		if( *m_pSlotBeforeDragging != NULL &&
			m_NowInventoryPageNum[m_NowInventorySortType - 1] != GetPageNumBySlotID( (*m_pSlotBeforeDragging)->GetSlotID() ) )
		{
			CX2Slot::SLOT_TYPE eType = (*m_pSlotBeforeDragging)->GetSlotType(); 
			if( eType != CX2Slot::ST_BANK && 
				eType != CX2Slot::ST_EQUIPPED &&
				eType != CX2Slot::ST_PET )
			{
				(*m_pSlotBeforeDragging)->SetShow(false);
			}
		}
#endif	SERV_REFORM_INVENTORY_TEST

		return false;
	}

	// 마우스 클릭 시 행할 액션을 여기에 쓴다 

	CX2State* pState = (CX2State*)g_pMain->GetNowState();
	if(pState != NULL)
	{
		CX2Cursor* pCursor = pState->GetCursor();
		if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
		{
			UidType ItemUID = pSlot->GetItemUID();
			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ItemUID );

			if(pItem == NULL )
				return false;


			g_pData->GetUIManager()->SetDLGFlag(false);
			switch(pCursor->GetCurorState())
			{
			case CX2Cursor::XCS_SELL:
				{
					if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SHOP))
						g_pData->GetUIManager()->GetUIShop()->SellItem( pSlot->GetItemUID(), g_pKTDXApp->MouseConvertByResolution(mousePos) );
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_793 ), g_pMain->GetNowState() );

#ifdef SERV_SOCKET_NEW
					if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

						return true;
					}
#endif SERV_SOCKET_NEW

				}
				return true;
			case CX2Cursor::XCS_REPAIR:
				{
					if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SHOP))
						g_pData->GetUIManager()->GetUIShop()->RepairItem( pSlot->GetItemUID() );
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_794 ), g_pMain->GetNowState() );
#ifdef SERV_SOCKET_NEW
					if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

						return true;
					}
#endif SERV_SOCKET_NEW
				}
				return true;
			case CX2Cursor::XCS_DECOMPOSITION:
#ifdef SERV_SOCKET_NEW
				if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

					return true;
				}
#endif SERV_SOCKET_NEW
				if(m_pDLGResolveItem == NULL && m_pDLGResolveItemResult == NULL	)
				{
#ifndef SERV_MULTI_RESOLVE
					OpenResolveWindow( pSlot );
#endif SERV_MULTI_RESOLVE
				}
				return true;
			case CX2Cursor::XCS_ENCHANT:
				{
#ifdef SERV_SOCKET_NEW
					if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

						return true;
					}
#endif SERV_SOCKET_NEW
					if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SHOP))
						g_pData->GetUIManager()->GetUIShop()->EnchantItem( pSlot->GetItemUID(), g_pKTDXApp->MouseConvertByResolution(mousePos) );
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_795 ), g_pMain->GetNowState() );
				}
				return true;
			case CX2Cursor::XCS_SOCKET:
				if(m_pDLGSocketItem == NULL)
				{
#ifdef SERV_SOCKET_NEW
//{{ oasis907 : 김상윤 [2010.4.19] // 
					CX2State* pState = (CX2State*)g_pMain->GetNowState();
					if(pState != NULL)
					{
						CX2Cursor* pCursor = pState->GetCursor();
						if ( pCursor != NULL)
						{
							pCursor->ChangeCursorState(CX2Cursor::XCS_NORMAL);
						}
					}
					
					if ( true == OpenNewSocketWindow( pSlot ) )
						ChangeInventoryTab(CX2Inventory::ST_SPECIAL);

#else
					OpenSocketWindow( pSlot );
//}} oasis907 : 김상윤 [2010.4.19] // 
#endif SERV_SOCKET_NEW
				}
				return true;
			case CX2Cursor::XCS_ATTRIB_ENCHANT:
				{
#ifdef SERV_SOCKET_NEW
					if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

						return true;
					}
#endif SERV_SOCKET_NEW
					if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SHOP))
						g_pData->GetUIManager()->GetUIShop()->EnchantAttributeItem( pSlot->GetItemUID(), g_pKTDXApp->MouseConvertByResolution(mousePos) );
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_796 ), g_pMain->GetNowState() );
				}
				return true;
#ifdef SERV_ATTRIBUTE_CHARM
			case CX2Cursor::XCS_ATTACH_ATTRIB_ITEM:
#endif SERV_ATTRIBUTE_CHARM
#ifdef ATTRACTION_ITEM_TEST
			case CX2Cursor::XCS_ATTACH_ITEM:
				{
#ifdef SERV_SOCKET_NEW
					if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

						return true;
					}
#endif SERV_SOCKET_NEW
					// 처리하고
					if( pSlot->GetSortType() == CX2Inventory::ST_EQUIP )
					{
						m_CouponTargetUid = pSlot->GetItemUID();
						CX2Item* pCouponItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_CouponUid );
						CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_CouponTargetUid );

						//{{ kimhc // 2010-01-06 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
						if ( pTargetItem != NULL && 
                            pTargetItem->GetItemTemplet() != NULL && 
                            pTargetItem->GetItemTemplet()->GetIsPcBang() == true )
						{
							g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
							return true;
						}
#endif	PC_BANG_WORK
						//}} kimhc // 2010-01-06 // PC방 프리미엄 서비스

						if(pCouponItem != NULL || pTargetItem != NULL)
						{
#ifdef SERV_ATTRIBUTE_CHARM
							if( IsAttributeItem( pCouponItem->GetItemData().m_ItemID ) )
							{
								if(pTargetItem->GetItemTemplet()->GetItemType() != CX2Item::IT_WEAPON)
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_10324 ), g_pMain->GetNowState() );
								}
								else
								{
									wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_10283, "SSL", pTargetItem->GetItemTemplet()->GetFullName_(), pCouponItem->GetItemTemplet()->GetFullName_(), GetAttributeItemExtraDamageName(pCouponItem->GetItemData().m_ItemID) ) );
									g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), UIM_USE_ATTRIBUTE_CHARM_COUPON_ITEM_OK, g_pMain->GetNowState(), UIM_USE_COUPON_ITEM_CANCLE );
								}
							}
							else
							{
								wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_797, "iSS", pTargetItem->GetItemData().m_EnchantLevel, pTargetItem->GetItemTemplet()->GetFullName_(), pCouponItem->GetItemTemplet()->GetFullName_() ) );
								g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), UIM_USE_COUPON_ITEM_OK, g_pMain->GetNowState(), UIM_USE_COUPON_ITEM_CANCLE );
							}
#else
                            wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_797, "iSS", pTargetItem->GetItemData().m_EnchantLevel, pTargetItem->GetItemTemplet()->GetFullName_(), pCouponItem->GetItemTemplet()->GetFullName_() ) );

							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), UIM_USE_COUPON_ITEM_OK, g_pMain->GetNowState(), UIM_USE_COUPON_ITEM_CANCLE );
#endif SERV_ATTRIBUTE_CHARM
						}
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_798 ), g_pMain->GetNowState() );
					}
				} return true;
#endif ATTRACTION_ITEM_TEST
				//case CX2Cursor::XCS_ITEM_EXCHANGE:
#ifdef AUTH_DELETE_ITEM
			case CX2Cursor::XCS_DELETE_ITEM:
				{
#ifdef SERV_SOCKET_NEW
					if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

						return true;
					}
#endif SERV_SOCKET_NEW
					if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
						g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2650 ), g_pMain->GetNowState() );		
						return true;
					}

						//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
						CX2Item* pItem		= NULL;
						m_EnrollItemUid		= pSlot->GetItemUID();
						pItem				= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnrollItemUid );

						if ( pItem == NULL || 
							pItem->GetItemTemplet() == NULL )
							return false;
						
						if ( g_pData->GetMyUser()->GetAuthLevel() != CX2User::XUAL_DEV )
						{
							//{{ 2010. 11. 29	최육사	기간제템 삭제
#ifdef SERV_DELETE_PERIOD_ITEM
#else

#ifndef SPT_NONE_ITEM_DELETE
							if ( pItem->GetItemData().m_Period > 0 )
							{
								CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2629 ), g_pMain->GetNowState() );
								pMsgBox->SetEnableMoveByDrag_LUA(false);

								return true;
							}

							if ( pItem->GetItemTemplet()->GetShopPriceType() == CX2Item::SPT_NONE )
							{
								CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2630 ), g_pMain->GetNowState() );
								pMsgBox->SetEnableMoveByDrag_LUA(false);

								return true;
							}
#endif //SPT_NONE_ITEM_DELETE

#endif SERV_DELETE_PERIOD_ITEM
							//}}							

			//{{ kimhc // 2009-09-08 // 봉인된 아이템 버리기 불가
#ifdef	SEAL_ITEM
							if ( pItem->GetItemData().m_bIsSealed == true )
							{
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4477 ), g_pMain->GetNowState() );
								return true; 
							}
#endif	SEAL_ITEM
			//}} kimhc // 2009-09-08 // 봉인된 아이템 버리기 불가
						}
#ifdef SPT_NONE_ITEM_DELETE
						//pc방 아이템 예외처리
						if( true == pItem->GetItemTemplet()->GetIsPcBang() )
						{
							g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
							return true;
						}

						if ( pItem->GetItemTemplet()->GetShopPriceType() == CX2Item::SPT_NONE )
						{
							if( pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_QUEST )
							{
								CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_23610 ), g_pMain->GetNowState() );
								if( NULL != pMsgBox )
									pMsgBox->SetEnableMoveByDrag_LUA(false);

								return true;
							}
							m_bIsSptNoneItem = true;
						}
#endif //SPT_NONE_ITEM_DELETE

						// 수량성인지 아닌지 봐서 수량성이면
						if( CX2Item::PT_QUANTITY == pItem->GetItemTemplet()->GetPeriodType() )
						{
							D3DXVECTOR2 Pos;
							Pos.x = static_cast< float >( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos() );
							Pos.y = static_cast< float >( g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() );
							
							m_iItemEnrollNum	= pItem->GetItemData().m_Quantity;

							OpenRegisterQuantityDLG( Pos );
						}
						else		// 아니면
						{
							m_iItemEnrollNum	= 1;
#ifdef SPT_NONE_ITEM_DELETE
							if( true == m_bIsSptNoneItem)
							{
#ifdef CLIENT_GLOBAL_LINEBREAK
								std::wstring wstrMsg = GET_REPLACED_STRING( (STR_ID_23495, "Li", pItem->GetFullName(), pItem->GetItemData().m_Quantity) );
								std::wstring tempName = CWordLineHandler::GetStrByLineBreakInX2Main( wstrMsg.c_str(), 257, 0 );

								m_pSptNoneItemDelete = 
									g_pMain->KTDGUIOkAndCancelEditBox2( D3DXVECTOR2( -999, -999 ), tempName.c_str(), 
									UIM_SPTNONE_ITEM_DELETE_OK, g_pMain->GetNowState(), 10, false, UIM_SPTNONE_ITEM_DELETE_CANCEL );
#else //CLIENT_GLOBAL_LINEBREAK
								m_pSptNoneItemDelete = 
									g_pMain->KTDGUIOkAndCancelEditBox2( D3DXVECTOR2( -999, -999 ), 
									GET_REPLACED_STRING( (STR_ID_23495, "Li", pItem->GetFullName(), pItem->GetItemData().m_Quantity) ), 
									UIM_SPTNONE_ITEM_DELETE_OK, g_pMain->GetNowState(), 10, false, UIM_SPTNONE_ITEM_DELETE_CANCEL );
#endif //CLIENT_GLOBAL_LINEBREAK
							}
							else
#endif //SPT_NONE_ITEM_DELETE
							OpenDeleteItemConfirmDLG();
						}
						pItem = NULL;
#else REAL_TIME_ELSWORD
					if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel() )
					{
						Handler_EGS_DELETE_ITEM_REQ( pSlot->GetItemUID() );
					}
#endif REAL_TIME_ELSWORD
						//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기
					
					
				} return true;
#endif AUTH_DELETE_ITEM

				//{{ kimhc // 2009-08-19 // 봉인의 인장(가칭)을 우클릭 후 봉인 하고자 하는 아이템을 클릭한 경우
#ifdef	SEAL_ITEM
			case CX2Cursor::XCS_SEAL_ITEM:
				{
#ifdef SERV_SOCKET_NEW
					if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

						return true;
					}
#endif SERV_SOCKET_NEW
					if ( CanSealUpThisItem( pItem ) == true )
					{
						m_pSlotItemToBeSealed	= pSlot;

						// 봉인 확인 다이얼로그
						m_pDLGSealItemConfirm	= g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), 
							GET_REPLACED_STRING( ( STR_ID_4400, "L", pItem->GetFullName() ) ), 
							UIM_SEAL_ITEM_CONFIRM_OK , g_pMain->GetNowState(), UIM_SEAL_ITEM_CONFIRM_CANCEL );

						m_pDLGSealItemConfirm->SetEnableMoveByDrag_LUA(false);

						CKTDGUIButton* pButton	= static_cast< CKTDGUIButton* >( m_pDLGSealItemConfirm->GetControl( L"MsgBoxOkAndCancelOkButton" ) );

						if ( pButton != NULL )
							m_pDLGSealItemConfirm->RequestFocus( pButton );

					}
					else
					{
						ResetItemIDForSealAndCursorState();
					}

				} return true;
#endif	SEAL_ITEM
				//}} kimhc // 2009-08-19 // 봉인의 인장(가칭)을 우클릭 후 봉인 하고자 하는 아이템을 클릭한 경우
#ifdef SERV_PET_SYSTEM
			case CX2Cursor::XCS_PET_FOOD:
				{
					m_pPetFoodSlot = pSlot;
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5352 ), 
						UIM_PET_FOOD_OK, (CKTDXStage*) g_pMain->GetNowState() ); 
				}
				return true;					
#endif
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
			case CX2Cursor::XCS_ITEM_EXCHANGE:
				{
					if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
					{
						//g_pData->GetUIManager()->GetUIItemExchangeShop()->InvalidLastSrcItem();
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );

						return true;
					}
					if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_ITEM_EXCHANGE_SHOP))
					{
                        int iSelectedItemID = pItem->GetItemTemplet()->GetItemID();
						if( true == g_pData->GetUIManager()->GetUIItemExchangeShop()->CheckExchangeUseItem(iSelectedItemID) )
						{
							g_pData->GetUIManager()->GetUIItemExchangeShop()->InvalidLastSrcItem();
							if( true == g_pData->GetUIManager()->GetUIItemExchangeShop()->SelectExchangeSrcItem(ItemUID) )
								pSlot->SetShowExchangeSelectImage(true);

							CX2State* pState = (CX2State*)g_pMain->GetNowState();
							if(pState->GetCursor() != NULL)
								pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
						}
						else
						{
							//g_pData->GetUIManager()->GetUIItemExchangeShop()->InvalidLastSrcItem();
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_13860 ), g_pMain->GetNowState() );
						}
					}
					else
					{
						//g_pData->GetUIManager()->GetUIItemExchangeShop()->InvalidLastSrcItem();
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_796 ), g_pMain->GetNowState() );
					}
				}
#endif SERV_ITEM_EXCHANGE_NEW
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //미라클큐브 // 미감정 상태로 되돌리기
			case CX2Cursor::XCS_ITEM_EVALUTE:
				{
#ifdef SERV_SOCKET_NEW
					// 소켓 작업 중 처리 하지 않기
					if( NULL != GetUISocketItem() && true == GetUISocketItem()->GetShow() )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );
						return true;
					}
#endif // SERV_SOCKET_NEW

					if( NULL != g_pData->GetMyUser() &&
						NULL != g_pData->GetMyUser()->GetSelectUnit() )
					{
						m_iTargetItemUID = pSlot->GetItemUID();
						
						const CX2Item* pConsumeItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iConsumeItemUID );
						const CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iTargetItemUID );

						if( NULL != pConsumeItem && NULL != pConsumeItem->GetItemTemplet() &&
							NULL != pTargetItem && NULL != pTargetItem->GetItemTemplet() )
						{
#ifdef PC_BANG_WORK
							if( true == pTargetItem->GetItemTemplet()->GetIsPcBang() )
							{
								g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
								return true;
							}
#endif // PC_BANG_WORK

							wstring wstrMsg = 
								GET_REPLACED_STRING( ( STR_ID_24613, "SS", 
													pTargetItem->GetItemTemplet()->GetFullName_(), 
													pConsumeItem->GetItemTemplet()->GetFullName_() ) );

							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), 
								UIM_USE_RESTORE_ITEM_EVALUATION_OK, g_pMain->GetNowState(), UIM_USE_RESTORE_ITEM_EVALUATION_CANCEL );
						}
					}
				} return true;
			case CX2Cursor::XCS_EXCHANGE_NEW_ITEM:
				{
					if( NULL != pSlot &&
						NULL != g_pData && 
						NULL != g_pData->GetItemManager() )
					{		
						const CX2Item::ItemTemplet* pTargetItem = g_pData->GetItemManager()->GetItemTemplet( pSlot->GetItemTID() );
						if( NULL != pTargetItem )
						{
							//////////////////////
							const int iResultItemID = g_pData->GetItemManager()->GetConvertResultItemID( pTargetItem->GetItemID() );
							if( 0 == iResultItemID )
							{
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING(STR_ID_25008), g_pMain->GetNowState() );
							}
							else
							{
								const CX2Item::ItemTemplet* pResultItem = g_pData->GetItemManager()->GetItemTemplet( iResultItemID );
								if( NULL != pResultItem )
								{
									m_iTargetItemUID = pSlot->GetItemUID();
									m_iConsumeItemQuantity = 1;
									if( NULL != g_pData->GetMyUser() &&
										NULL != g_pData->GetMyUser()->GetSelectUnit() )
									{
										CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iTargetItemUID );
										if( NULL != pTargetItem )
										{
											m_iConsumeItemQuantity = pTargetItem->GetItemData().m_Quantity;
										}
									}

									wstring wstrMsg = L"";

									if( 1 < m_iConsumeItemQuantity || 
										CX2UnitManager::NUI_LUTO_VILLAGE == g_pTFieldGame->GetJoinNpcId() )
									{ // 수량성 아이템 문구
										wstrMsg = 
											GET_REPLACED_STRING( ( STR_ID_26940, "SiSi", pTargetItem->GetFullName_(), m_iConsumeItemQuantity,
											pResultItem->GetFullName_(), m_iConsumeItemQuantity ));
									}
									else
									{
										wstrMsg = 
											GET_REPLACED_STRING( ( STR_ID_25009, "SiSi", pTargetItem->GetFullName_(), pTargetItem->GetUseLevel(),
											pResultItem->GetFullName_(), pResultItem->GetUseLevel() ));
									}

									g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), 
										UIM_EXCHANGE_NEW_ITEM_OK, g_pMain->GetNowState(), UIM_EXCHANGE_NEW_ITEM_CANCEL );

								}
							}
						}
					}
					return true;
				} break;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장
			case CX2Cursor::XCS_ADD_SOCKET_SLOT:
				{
#ifdef SERV_SOCKET_NEW
					// 소켓 작업 중 처리 하지 않기
					if( NULL != GetUISocketItem() && true == GetUISocketItem()->GetShow() )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5111 ), g_pMain->GetNowState() );
						return true;
					}
#endif // SERV_SOCKET_NEW

					if( NULL != g_pData->GetMyUser() &&
						NULL != g_pData->GetMyUser()->GetSelectUnit() )
					{
						m_iTargetItemUID = pSlot->GetItemUID();

						CX2Item* pConsumeItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iConsumeItemUID );
						CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iTargetItemUID );

						if( NULL != pConsumeItem && NULL != pTargetItem )
						{
							if( NULL != pConsumeItem->GetItemTemplet() && NULL != pTargetItem->GetItemTemplet() )
							{
								// 방어구 / 무기 타입 검사
								if( ITEM_ID_ADD_WEAPON_SOCKET_SLOT == pConsumeItem->GetItemData().m_ItemID && 
									CX2Item::IT_WEAPON != pTargetItem->GetItemTemplet()->GetItemType() )
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28329 ), g_pMain->GetNowState() );
									return true;
								}
								else if( ITEM_ID_ADD_DEFENCE_SOCKET_SLOT == pConsumeItem->GetItemData().m_ItemID &&
									     CX2Item::IT_DEFENCE != pTargetItem->GetItemTemplet()->GetItemType() )
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28330 ), g_pMain->GetNowState() );
									return true;
								}

								// 사용에 문제 없으면, 실제 사용 여부 묻기
								wstring wstrMsg = 
									GET_REPLACED_STRING( ( STR_ID_24613, "SS", 
									pTargetItem->GetItemTemplet()->GetFullName_(), 
									pConsumeItem->GetItemTemplet()->GetFullName_() ) );

								g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), 
									UIM_USE_ADD_SOCKET_SLOT_OK, g_pMain->GetNowState(), UIM_USE_ADD_SOCKET_SLOT_CANCEL );
							}
						}
					}
				} break;				
#endif // ADD_SOCKET_SLOT
			default:
				break;
			}
			return true;
		}
	}

	if ( *m_pSlotBeforeDragging != NULL )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*) *m_pSlotBeforeDragging;
		bool bCheck = false;

		//{{ kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기
#ifdef	REAL_TIME_ELSWORD
		if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
		{
			if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
				return false;
			
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			if( g_pX2Game->GetMyUnit()->IsTransformed() )
				return false;
#endif

			if ( g_pX2Game->GetMyUnit()->GetNowHp() <= 0 )
			{
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );

				return false;
			}

			if ( g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetWaitStateID() &&
				g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetChargeMpStateID() 
#ifdef RIDING_SYSTEM
				&& g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetRidingWaitStateID() 
#endif // RIDING_SYSTEM

				)
				return false;
		}
#endif	REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기

		bCheck = OnDropAnyItem(mousePos);		// 인벤토리 슬롯에 떨어져쪄요

		if ( *m_pSlotBeforeDragging != NULL && (*m_pSlotBeforeDragging)->GetDialog() != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), (*m_pSlotBeforeDragging)->GetNormalLayer() );
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
		}

		if(bCheck)
		{
			*m_pSlotBeforeDragging = NULL;
			*m_DraggingItemUID = -1;
			return true;
		}
	}

	// 	if ( *m_pSlotBeforeDragging != NULL && (*m_pSlotBeforeDragging)->GetDialog() != NULL )
	// 	{
	// 		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), GetItemLayer() );
	// 		(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
	// 	}

	//*m_pSlotBeforeDragging = NULL;
	//*m_DraggingItemUID = -1;

	return false;
}

//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
void CX2UIInventory::OpenDeleteItemConfirmDLG()
{
	int popUpSizeX = 425;		// 개인 거래시에 뜨는 팝업의 위치를 가져다 씀
	int popUpSizeY = 147;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnrollItemUid );
	if ( pItem != NULL )
	{
		if ( pItem->GetItemTemplet() != NULL )
		{
			m_pDLGDeleteItemConfirm	= g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_REPLACED_STRING( ( STR_ID_2626, "Li", pItem->GetFullName(), m_iItemEnrollNum ) ), UIM_DELETE_ITEM_CONFIRM_OK , g_pMain->GetNowState(), UIM_DELETE_ITEM_CONFIRM_CANCEL );
			m_pDLGDeleteItemConfirm->SetEnableMoveByDrag_LUA(false);

			CKTDGUIButton* pButton	= static_cast< CKTDGUIButton* >( m_pDLGDeleteItemConfirm->GetControl( L"MsgBoxOkAndCancelOkButton" ) );
			
 			if ( pButton != NULL )
 			{
// 				pButton->ChangeMouseUpSound( L"Item_Sell.ogg" );
 				m_pDLGDeleteItemConfirm->RequestFocus( pButton );
 
 			}
		}
	}
}

bool CX2UIInventory::GetOpenDeleteItemConfirmDLG()
{
	if(m_pDLGDeleteItemConfirm != NULL && m_pDLGDeleteItemConfirm->GetShow())
		return true;

	return false;

}

bool CX2UIInventory::GetOpenRegisterQuantityDLG()
{
	if ( m_pDLGItemEnroll != NULL && m_pDLGItemEnroll->GetShow() )
	{
		return true;
	}

	return false;
}

void CX2UIInventory::OpenRegisterQuantityDLG( D3DXVECTOR2 pos )
{

	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);

	m_pDLGItemEnroll = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_An.lua" );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemEnroll );
	m_pDLGItemEnroll->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-865, 768-633), D3DXVECTOR2(75,20)) );

	CKTDGUIStatic* pStaticPrice			= static_cast < CKTDGUIStatic* >( m_pDLGItemEnroll->GetControl( L"Static_Price" ) );
	CKTDGUIStatic* pStaticEDMark		= static_cast < CKTDGUIStatic* >( m_pDLGItemEnroll->GetControl( L"Static_EDMark" ) );
	
	pStaticPrice->SetShow(false);
	pStaticEDMark->SetShow(false);

	CKTDGUIButton*		pButtonOK		= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_OK" ) );
	CKTDGUIButton*		pButtonCancle	= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Cancle" ) );
	CKTDGUIButton*		pButtonPlus		= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Plus" ) );
	CKTDGUIButton*		pButtonMinus	= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Minus" ) );
	CKTDGUIIMEEditBox*	pQuantity		= static_cast< CKTDGUIIMEEditBox* >( m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" ) );

	pQuantity->SetText( GET_REPLACED_STRING( ( STR_ID_2632, "i", m_iItemEnrollNum ) ), true);
	pButtonOK->SetCustomMsgMouseUp( UIM_REGISTER_QUANTITY_CONFIRM );
	pButtonCancle->SetCustomMsgMouseUp( UIM_REGISTER_QUANTITY_CANCEL );
	pButtonPlus->SetCustomMsgMouseUp( UIM_REGISTER_QUANTITY_PLUS );
	pButtonMinus->SetCustomMsgMouseUp( UIM_REGISTER_QUANTITY_MINUS );
	pQuantity->SetCustomMsgChange( UIM_REGISTER_QUANTITY_UPDATE );
	pQuantity->SetCustomMsgEnter( UIM_REGISTER_QUANTITY_CONFIRM );
	m_pDLGItemEnroll->SetCloseCustomUIEventID( UIM_REGISTER_QUANTITY_CANCEL );
	pQuantity->RequestFocus();

}

void CX2UIInventory::UpdateRegisterQuantityDLG( bool bReadIME )
{
	ASSERT( NULL != m_pDLGItemEnroll );
	if( NULL == m_pDLGItemEnroll )
		return; 

	CKTDGUIIMEEditBox* pQuantity = static_cast< CKTDGUIIMEEditBox* >( m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" ) );

	if(bReadIME)
	{
		m_iItemEnrollNum = g_pMain->GetEDFromString( pQuantity->GetText() );
	}	

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnrollItemUid );
	
	if ( m_iItemEnrollNum <= 0 )
	{
		if(bReadIME)				// 입력창으로 받아온 경우
			m_iItemEnrollNum = 0;
		else						// << 를 누른 경우 : 0 아래면 max로 순환시킨다.
			m_iItemEnrollNum = pItem->GetItemTemplet()->GetQuantity();	
	}

	if ( m_iItemEnrollNum >= pItem->GetItemData().m_Quantity )
		m_iItemEnrollNum = pItem->GetItemData().m_Quantity;

	WCHAR buff[256] = {0};
	_itow( m_iItemEnrollNum, buff, 10 );

	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str() );

}
#endif REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

bool CX2UIInventory::OnDropAnyItem( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
		return false;

	if ( pItemSlot == *m_pSlotBeforeDragging )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
		SetNowDragItemReturn();
#ifdef NEW_ITEM_NOTICE 
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGItemHighLight, true );	
#endif //NEW_ITEM_NOTICE 	
		return true;
	}

	//{{ 2008.11.13 김태완 : UI 예외처리
	if(!(*m_pSlotBeforeDragging)->IsItem()) return false;
	//*m_DraggingItemUID = ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetItemUID();
	//}}

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pItem = NULL;
	pItem = kInventory.GetItem( *m_DraggingItemUID, true );

	if ( pItem == NULL )
	{
		ASSERT( !"GetItem Is NULL" );
		return false;
	}
	
	// 같은 아이템 분류 타입인지 보고..
	if ( m_NowInventorySortType != kInventory.GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) ) return false;

	switch((*m_pSlotBeforeDragging)->GetSlotType())
	{
	case CX2Slot::ST_EQUIPPED:
		//장비창에서 넘어온 경우
		{
			switch ( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_DUNGEON_GAME:
			case CX2Main::XS_BATTLE_FIELD:
				{
					if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
						return false;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
					if( g_pX2Game->GetMyUnit()->IsTransformed() )
						return false;
#endif

					if ( g_pX2Game->GetMyUnit()->GetNowHp() <= 0 )
					{
						g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );
						return false;
					}

					// 게임내에서 바쁜 상태인 경우 장비교체 불가능
					if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
						return false;

				} break;

			default:
				break;
			}

			//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
		#ifdef	MODIFY_INFINITE_SKILL_BUG
			if ( g_pMain->GetNowState() != NULL)
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReadyByEquip();
					SetNowDragItemReturn();
					return true;
				}
			}

			if ( g_pInstanceData->GetSendGameStartReqPacket() == true )
			{
				SetNowDragItemReturn();
				return true;
			}
		#endif	MODIFY_INFINITE_SKILL_BUG
			//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정

			// 대상이 빈 슬롯일 경우
			if ( pItemSlot->IsResetItemUI() == false )
			{
				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)*m_pSlotBeforeDragging),
					pItemSlot );

				CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) *m_pSlotBeforeDragging;
				pkSlotBeforeDragging->DestroyItemUI();
				// 장비창의 장비를 리셋해 주자
				if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
				{
					g_pData->GetUIManager()->GetUICharInfo()->EquipChanged();
				}				
				return true;
			}
			// 대상이 이미 있는 경우, 비어잇는 슬롯을 검색
			else
			{
				for ( int j = 0; j < (int)m_SlotList.size(); j++ )
				{
					CX2SlotItem* pItemSlotToPush = (CX2SlotItem*)GetSlot(j);
					if ( pItemSlotToPush->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlotToPush->GetDialog() == NULL && pItemSlotToPush->GetEnable())
					{
						Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)*m_pSlotBeforeDragging), 
							pItemSlotToPush );

						CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) *m_pSlotBeforeDragging;
						pkSlotBeforeDragging->DestroyItemUI();
						// 장비창에 장비가 교환되었다는 메시지를 보내자
						if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
						{
							g_pData->GetUIManager()->GetUICharInfo()->EquipChanged();
						}
						return true;
					}
				}
			}
		}break;
		//{{ kimhc // 2009-08-08 // 은행에서 이동되었을 경우
#ifdef	PRIVATE_BANK
	case CX2Slot::ST_BANK:
#endif	PRIVATE_BANK
#ifdef SERV_PET_SYSTEM
	case CX2Slot::ST_PET:
#endif SERV_PET_SYSTEM
		//}} kimhc // 2009-08-08 // 은행에서 이동되었을 경우
	case CX2Slot::ST_INVENTORY:
		// 인벤토리간의 교환일 경우
		{
#ifdef SERV_PET_SYSTEM
			//{{kimch // 2010-12-18 // 던전 또는 대전의 레디 상태에서 펫 인벤 <-> 유저 인벤 안되도록 예외처리 추가
			if ( (*m_pSlotBeforeDragging)->GetSlotType() == CX2Slot::ST_PET )
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReady( STR_ID_9874 );
					SetNowDragItemReturn();
					return true;
				}
			} // if
			//}}kimch // 2010-12-18 // 던전 또는 대전의 레디 상태에서 펫 인벤 <-> 유저 인벤 안되도록 예외처리 추가
			
			// oasis907 : 김상윤 [2010.9.8] // 소켓 강화 사용 아이템 예외 처리, 펫 인벤에서 인벤으로 이동 못하게
			if( GetUISocketItem() != NULL &&
				GetUISocketItem()->GetShow() == true &&
				GetUISocketItem()->CheckSocketUseItem( static_cast< CX2SlotItem* >( (*m_pSlotBeforeDragging) )->GetItemUID() ) )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5112 ), g_pMain->GetNowState() );
				SetNowDragItemReturn();
				return true;
			}
#endif SERV_PET_SYSTEM

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	if ( g_pData->GetUIManager() != NULL &&													
		g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true &&
		g_pData->GetUIManager()->GetUIResolveItem()->CheckResolveUseItem( static_cast< CX2SlotItem* >( (*m_pSlotBeforeDragging) )->GetItemUID() ) )
	{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_17184 ), g_pMain->GetNowState() );
				SetNowDragItemReturn();
				return true;
	}
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
			if ( g_pData->GetUIManager() != NULL &&													
				g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true &&
				g_pData->GetUIManager()->GetUISynthesisItem()->CheckSynthesisUseItem( static_cast< CX2SlotItem* >( (*m_pSlotBeforeDragging) )->GetItemUID() ) )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_17184 ), g_pMain->GetNowState() );
				SetNowDragItemReturn();
				return true;
			}
#endif SERV_SYNTHESIS_AVATAR
	//}}
			if( Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)*m_pSlotBeforeDragging), pItemSlot ) == true )
			{
				CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) *m_pSlotBeforeDragging;
				pkSlotBeforeDragging->DestroyItemUI();

				return true;
			}
			else
			{
				SetNowDragItemReturn();
				return true;
			}
		}
	default:
		break;
	}

	// 처리가 안 된 경우 
	return false;

}



bool CX2UIInventory::OnRClickedItem( D3DXVECTOR2 mousePos )
{
	// 마우스업된 위치가 인벤 안일 경우
	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL )
		return false;

	if ( pItemSlot->GetSlotType() != CX2SlotManager::CX2Slot::ST_INVENTORY || pItemSlot->IsResetItemUI() == false )
		return false;

	const CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
	if ( pkItemTemplet == NULL )
		return false;

	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
	if(pUnit == NULL)
		return false;

	const CX2Unit::UnitData* pUnitData = &pUnit->GetUnitData();

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );
	if( NULL == pItem )
		return false;

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	if ( g_pData->GetUIManager() != NULL &&													
		g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true )
	{

		g_pData->GetUIManager()->GetUIResolveItem()->RegisterResolveItem(pItemSlot);
		return true;
	}
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	if ( g_pData->GetUIManager() != NULL &&													
		g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true )
	{

		g_pData->GetUIManager()->GetUISynthesisItem()->RegisterSynthesisItem(pItemSlot);
		return true;
	}
#endif SERV_SYNTHESIS_AVATAR
	//}}
	//{{ kimhc // 2009-08-05 // 은행
#ifdef	PRIVATE_BANK
	// 은행이 열려 있는 경우 아이템 사용 안되고 은행으로 이동
	if ( g_pData->GetUIManager() != NULL &&													
		g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_PRIVATE_BANK ) == true )	// 은행이 열려있는 경우
	{
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		// 2차전직 변경권 은행으로 이동 불가
		if ( pItem->GetItemTemplet()->GetItemID() == UNLIMITED_SECOND_CHANGE_JOB_ITEM_ID )
		{
			// 은행이로 이동 불가
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_29779 ), g_pMain->GetNowState() ); //18422
			return false;
		}	
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
#ifdef SERV_EVENT_TEAR_OF_ELWOMAN //pItemSlot->GetSlotType()
		// 펫인벤토리와 유저인벤토리, 은행과 유저 인벤토리 시, 엘의 여인의 눈물 이동 차단
		if( NULL == pkItemTemplet->GetItemID() )
			return false;

		if( pkItemTemplet->GetItemID() == TEAR_OF_ELWOMAN_ITEM_ID )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_18422 ), g_pMain->GetNowState() ); //18422
			SetNowDragItemReturn();
			return true;
		}	
#endif SERV_EVENT_TEAR_OF_ELWOMAN
		//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
		if ( pkItemTemplet->GetIsPcBang() == true )
		{
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
			return false;
		}
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스

		// 은행에 아이템을 집어 넣는 처리
		const CX2Inventory& kInventory	=	g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

		CX2Item*		pEmptySlot	=	NULL;
#ifdef SERV_SHARING_BANK_TEST_EME
		int				iItemMaxNum = kInventory.GetItemMaxNum( CX2Inventory::ST_BANK );
		CX2Inventory::SORT_TYPE	eType = CX2Inventory::ST_BANK;

#ifdef	SERV_SHARING_BANK_TEST
		if( g_pData->GetUIManager()->GetUIPrivateBank()->GetIsShareBank() == true )
		{
			iItemMaxNum = g_pData->GetUIManager()->GetUIPrivateBank()->GetShareBankSize();
			eType = CX2Inventory::ST_SHARE_BANK;
		}
#endif	SERV_SHARING_BANK_TEST

		// SERV_REFORM_INVENTORY_TEST
		for ( int i = 0; i < iItemMaxNum; i++ )
		{
			pEmptySlot		=	kInventory.GetItem( eType, i );
#else // SERV_SHARING_BANK_TEST_EME
		for ( int i = 0; i < kInventory.GetItemMaxNum( CX2Inventory::ST_BANK ); i++ )
		{
			pEmptySlot		=	kInventory.GetItem( CX2Inventory::ST_BANK, i );
#endif // SERV_SHARING_BANK_TEST_EME

			if ( pEmptySlot == NULL )
			{
				CX2SlotItem* pToSlotItem	= NULL;
				pToSlotItem					= static_cast< CX2SlotItem* >( g_pData->GetUIManager()->GetUIPrivateBank()->GetSlot( i ) );

				if ( pToSlotItem == NULL )
				{
					ASSERT( !"UIPrivateBank's Slot Is NULL" );
					return false;
				}				

#ifdef SERV_SHARING_BANK_TEST_EME
				if( Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pItemSlot, pToSlotItem ) == true )
				{
					pItemSlot->DestroyItemUI();
					InvalidSlotDesc();
				}
				else
				{
					SetNowDragItemReturn();
				}
#else // SERV_SHARING_BANK_TEST_EME
				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pItemSlot, pToSlotItem );
				pItemSlot->DestroyItemUI();
				InvalidSlotDesc();
#endif // SERV_SHARING_BANK_TEST_EME
				return true;
			}
			pEmptySlot	= NULL;				
		}

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3859 ), g_pMain->GetNowState() );
		return false;
	}

	// 은행을 확장 시켜주는 퀘스트 아이템 사용시
	if ( pItemSlot->GetItemTID() == _CONST_UI_PRIVATE_BANK_::g_iBankQuestItemID )	
	{
		const CX2Inventory& kInventory	=	g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
		
		int iNumOfBankSlot	= kInventory.GetItemMaxNum( CX2Inventory::ST_BANK );
		
		if ( iNumOfBankSlot	== CX2Inventory::MPB_PLATINUM )		// 이미 플래티넘 등급 이면
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_3871 ), g_pMain->GetNowState() );
		}
	}
	
	
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-05 // 은행

	
	//{{ kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기
#ifdef	REAL_TIME_ELSWORD
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
				return false;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			if( g_pX2Game->GetMyUnit()->IsTransformed() )
				return false;
#endif

			if ( g_pX2Game->GetMyUnit()->GetNowHp() <= 0 )
			{
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );

				return false;
			}

			// 게임내에서 바쁜 상태인 경우 장비교체 불가능
			if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
				return false;

		} break;

	default:
		break;
	}
#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 장비 교체 막기

	// 판매중의 개인상점이 열려있으면 막는다
	if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
		g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2650 ), g_pMain->GetNowState() );
		
		return true;
	}

#ifdef SERV_SOCKET_NEW		
	if( GetUISocketItem() != NULL && GetUISocketItem()->GetShow() == true)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_5112 ), g_pMain->GetNowState() );

		return true;
	}
#endif SERV_SOCKET_NEW


#ifdef ITEM_RECOVERY_TEST
	// 사용불능 아이템이라면 
	if( pItem != NULL && true == pItem->IsDisabled() )
	{
		// 1. 복원 아이템이 있는지 확인한다.

		vector<int> vecRecoveryItemIdList;
		// 1-1. 만일 방어구 아이템이라면 방어구 전용 복원서의 유무를 먼저 검사한다.
		if( 
            pItem->GetItemTemplet() != NULL && 
            pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_DEFENCE)
		{
			GetRequiredArmorOnlyRecoveryStoneID( pkItemTemplet->GetUseLevel(), vecRecoveryItemIdList );
			CheckRequiredRecoveryStoneInInventory( vecRecoveryItemIdList );
		}

		// 1-2. 방어구 전용 복원서가 없었으면 그냥 복원서의 유무를 검사.
		if ( vecRecoveryItemIdList.empty() )
		{
			GetRequiredRecoveryStoneID( pkItemTemplet->GetUseLevel(), vecRecoveryItemIdList );
			CheckRequiredRecoveryStoneInInventory( vecRecoveryItemIdList );
		}

		// 해당 복원서가 있었으면
		if( false == vecRecoveryItemIdList.empty() )
		{
            m_RecoveryItemUid = pItem->GetUID();
			const CX2Item::ItemTemplet* pkRecoverItemTemlpet = g_pData->GetItemManager()->GetItemTemplet( vecRecoveryItemIdList.back() );
			if(pkRecoverItemTemlpet != NULL)
			{
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_799, "S", pkRecoverItemTemlpet->GetName() ) ), UIM_ITEM_RECOVERY_OK, g_pMain->GetNowState() );
			}
		}
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_800 ), g_pMain->GetNowState() );
		}
		return true; 
	}
#endif

	// 우편전송창이 열려있으면 이 아이템을 우편전송창에 등록한다
	if(g_pMain != NULL && g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetOpendSendWindow() == true)
	{
		g_pMain->GetPostBox()->RegisterItem(pItemSlot);
		return true;
	}
	// 대기중의 개인상점이 열려있으면 이 아이템을 개인상점에 등록하는 창을 열도록 하자
	if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
#ifdef SERV_PSHOP_AGENCY
		( g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_WAIT ||
		g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_WAIT  ) )
#else
		g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_WAIT )
#endif
	{
		g_pData->GetUIManager()->GetUIPersonalShop()->EnrollItemByUid( pItemSlot->GetItemUID() );
		return true;
	}
	// 개인거래창이 열려있으면 (상동)
	if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE))
	{
		g_pData->GetUIManager()->GetUIPersonalTrade()->OnRegisterMyItem( pItemSlot->GetItemUID() );
		return true;
	}
	// 	// 상점이나 제조창이 열려있으면 아무것도 안한다
	// 	if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SHOP) || true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_MANUFACTURE) )
	// 	{
	// 		return true;
	// 	}


#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 감정 시 소모되는 ED 기획 결정 필요
	if( pItem != NULL && false == pItem->GetIsEvaluation() && 
		NULL != pItem->GetItemTemplet() )
	{
		int iConsumeED = static_cast<int>(pItem->GetItemTemplet()->GetPrice() * 0.05f);
		m_iTargetItemUID = pItem->GetUID();
		g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
			GET_REPLACED_STRING( ( STR_ID_24611, "i",  iConsumeED) ), 
			UIM_USE_EVALUATE_ITEM_OK, g_pMain->GetNowState(), UIM_USE_EVALUATE_ITEM_CANCEL );
		return true;
	}			
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

	//{{ kimhc // 2009-09-03 // 봉인된 아이템을 우클릭 했을 때 처리
#ifdef	SEAL_ITEM
	if ( pItem != NULL && pItem->GetItemData().m_bIsSealed == true )
	{
		g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
            GET_REPLACED_STRING( ( STR_ID_4472, "S", pkItemTemplet->GetFullName_() ) ), 
			UIM_UNSEAL_ITEM_CONFIRM_OK, g_pMain->GetNowState(), UIM_UNSEAL_ITEM_CONFIRM_CANCEL ); 

		m_itemUIDToBeUnSealed = pItem->GetUID();
		return true;
	}
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-03 // 봉인된 아이템을 우클릭 했을 때 처리

	// 인벤토리 내부 클릭 : 장비 교환
	// 해당하는 슬롯을 검색, 장비창으로 이동
	for ( unsigned int iS = 0; iS < m_SlotList.size(); ++iS )
	{
		if(NULL == g_pData->GetUIManager()->GetUICharInfo())
		{
			ASSERT( !"Charinfo NULL" );		// 항상 살아있어야 하는 class
			g_pData->GetUIManager()->CreateUICharInfo();            
		}

		CX2SlotItem* pItemSlotEquip = (CX2SlotItem*) g_pData->GetUIManager()->GetUICharInfo()->GetSlot(iS);
		if( NULL == pItemSlotEquip )
			continue;

		//		if ( pItemSlotEquip->GetSlotType() != CX2SlotManager::CX2Slot::ST_EQUIPPED )
		//			continue;

		switch( pkItemTemplet->GetItemType() )
		{
		case CX2Item::IT_WEAPON:
		case CX2Item::IT_DEFENCE:
		case CX2Item::IT_ACCESSORY:
			{		
				//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
		#ifdef	MODIFY_INFINITE_SKILL_BUG
				if ( g_pMain->GetNowState() != NULL)
				{
					CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
					if ( pX2State->CheckIsReady() == true )
					{
						pX2State->ShowOkAndCancelMsgForUnReadyByEquip();
						return true;
					}
				}

				if ( g_pInstanceData->GetSendGameStartReqPacket() == true )
					return true;
		#endif	MODIFY_INFINITE_SKILL_BUG
				//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정


				//{{ 허상형 : [2009/7/29] //	사용불가 장비 착용 불가능하게 코드 입력
				if(	pkItemTemplet->GetNoEquip() == true )
				{
					continue;
				}
				//}} 허상형 : [2009/7/29] //	사용불가 장비 착용 불가능하게 코드 입력

				if( false == CX2Unit::CanEquipAsParts( 
                    pkItemTemplet->GetItemID(), 
                    pUnit, pUnitData->m_Level ) ) 
					continue;

				//장비 아이템 내구도 다 닳았으면 거시기 해불자
				//드래그해서 거시기 하는부분도 해결해불자

				if ( pItem != NULL )
				{
					if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE )
					{
						if ( pItem->GetItemData().m_Endurance <= 0 )
						{
							continue;
						}
					}
				}

				//{{ 09.06.02 태완 : 악세사리 류의 경우에는 fashion 조건 체크를 하지 않도록 예외처리 :
				// 악세사리이면서 패션템인 아이템이 장착되지 않는 것 때문에
 				if ( pItemSlotEquip->GetEquipPos() == pkItemTemplet->GetEqipPosition() &&
 					(pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_ACCESSORY || pItemSlotEquip->GetFashion() == pkItemTemplet->GetFashion()) )
				{
					if(false == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
					{
						g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_CHARINFO, true);
					}

					Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pItemSlot, pItemSlotEquip );
					pItemSlot->DestroyItemUI();
					InvalidSlotDesc();
					return true;
				}
			} break;
		}
	}



	// 퀵슬롯 등록
	if( pItemSlot->GetSortType() == CX2Inventory::ST_QUICK_SLOT )	// 퀵슬롯에 장착가능한 아이템이면
	{
		if(g_pData->GetUIManager()->GetUIQuickSlot() == NULL)
		{
			ASSERT( !"Quickslot NULL" );	// 퀵슬롯은 항상 살아있어야 하니까..
			g_pData->GetUIManager()->CreateUIQuickSlot();
		}

		CX2SlotItem* pQuickSlot = g_pData->GetUIManager()->GetUIQuickSlot()->GetEmptyslot();
		if( pQuickSlot == NULL ) 
			return false;

		Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pItemSlot, pQuickSlot );
		return true;
	}


#ifdef SERV_PET_SYSTEM
	for(int i=0; i<ARRAY_SIZE(PET_ITEM_ID); ++i)
	{
		if( 
            pItem->GetItemTemplet()->GetItemID() == PET_ITEM_ID[i] 
        )
		{
			if( NULL != g_pMain && CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() && g_pX2Game != NULL )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_6550 ), g_pMain->GetNowState() );
			}
			else if( g_pSquareGame != NULL )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_6551 ), g_pMain->GetNowState() );
			}
			else if( g_pData != NULL && g_pData->GetPetManager() != NULL )
			{
				// 펫 이름짓기
				g_pData->GetPetManager()->ShowMakePetName(true, pItem->GetUID());				
				return true;
			}
		}
	}			
#endif

#ifdef RIDING_SYSTEM
	for( int i = 0; i < ARRAY_SIZE(RIDING_PET_ITEM_ID); ++i )
	{
		if( pItem->GetItemTemplet()->GetItemID() == RIDING_PET_ITEM_ID[i] )
		{
			if( NULL != g_pMain && CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() && g_pX2Game != NULL )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_24536 ), g_pMain->GetNowState() );
			}
			else if( g_pSquareGame != NULL )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_24537 ), g_pMain->GetNowState() );
			}
			else if( NULL != CX2RidingPetManager::GetInstance() )
			{
#ifdef RIDING_ITEM_POPUP_CHECK
				m_iRidingItemUid = pItem->GetUID();  
				wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_25115, "L", pItem->GetFullName()) );
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrMsg.c_str(), UIM_RIDING_ITEM_USE_OK, g_pMain->GetNowState() );
#else //RIDING_ITEM_POPUP_CHECK
				CX2RidingPetManager::GetInstance()->Handler_EGS_CREATE_RIDING_PET_REQ( pItem->GetUID() );
#endif //RIDING_ITEM_POPUP_CHECK
				return true;
			}
		}
	}
#endif //RIDING_SYSTEM

#ifdef SERV_EVENT_VC
	if( pkItemTemplet->GetItemID() == ITEM_ID_EVENT_VC_BATTLE_FOOD )
	{
		if( g_pData->GetPetManager() != NULL )
		{
			g_pData->GetPetManager()->UseIntimacyUpItem( pItem->GetUID() );
		}		
		return true;
	}
#endif //SERV_EVENT_VC

#ifdef RIDINGPET_STAMINA_ITEM
	if(pkItemTemplet->GetItemID() == RIDINGPET_STAMINA_30_PER_ITEM_ID)
	{
		int iPecValue = 0;
		switch(pkItemTemplet->GetItemID())
		{
		case RIDINGPET_STAMINA_30_PER_ITEM_ID:
			iPecValue = 30;
			break;
		}

		if(CX2RidingPetManager::GetInstance()->SetRidingPetStaminaPercentUP(iPecValue))
		{
			Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( pItemSlot->GetItemUID() );
			return true;
		}
		else
		{
			// 실패 메세지 처리 장소.
			return false;
		}
	}
#endif RIDINGPET_STAMINA_ITEM

	// ** 사용가능 아이템 및 랜덤템 우클릭 처리
	// 인벤내 사용가능 아이템에 대한 처리
	if( pItem->GetItemTemplet()->GetCanUseInventory() == true )
	{
		//m_iItemUID
		if( CheckUseItem(pItem) == true )
		{
			//{{ kimhc // 실시간 엘소드 중 던전내에서 아이템 사용시 막아야 할 것들
#ifdef SERV_SKILL_NOTE
			if( pItem != NULL && pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_SKILL_MEMO )
			{
#ifdef REFORM_SKILL_NOTE_UI
				RegisterMemo( pkItemTemplet, pItem->GetUID() );
#else
				if( g_pData == NULL || 
					g_pData->GetMyUser() == NULL || 
					g_pData->GetMyUser()->GetSelectUnit() == NULL || 
					g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet() == NULL ||
					pItem->GetItemTemplet() == NULL 
                    )
					return true;

				if( GetX2UnitClassCompatibility( g_pData->GetMyUser()->GetSelectUnit()->GetClass(), pItem->GetItemTemplet()->GetUnitClass() ) == false )
				{
					// 메모 사용 유닛이 맞지 않음
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5013), g_pMain->GetNowState() );
					return true;
				}
				if( pItem->GetItemTemplet()->GetUseLevel() > g_pData->GetSelectUnitLevel() )
				{
					// 메모 사용레벨이 맞지 않음
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5014), g_pMain->GetNowState() );
					return true;
				}

				if( g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot() <= 0)
				{
					// 기술의 노트 사용 불가상태
					g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4988 ),g_pMain->GetNowState() );
					return true;
				}

				if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME || g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
				{		
					// 게임중 메모사용 불가
					g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4989 ),g_pMain->GetNowState() );
					return true;
				}

				if( g_pData->GetUIManager() != NULL )
				{
					if( g_pData->GetUIManager()->GetUISkillNote() != NULL &&
						g_pData->GetUIManager()->GetUISkillNote()->GetUpdateMemo() == true )
					{						
						g_pData->GetUIManager()->GetUISkillNote()->UpdateMemoSlot( (int)g_pData->GetUIManager()->GetUISkillNote()->GetUpdatePage() );
						g_pData->GetUIManager()->GetUISkillNote()->ResetSkillNote();
						
						return true;
					}

					// 메모등록					
					g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_NOTE, true);
					if( g_pData->GetUIManager()->GetUISkillNote() != NULL )
					{
						CX2State* pState = (CX2State*)g_pMain->GetNowState();
						if(pState->GetCursor() != NULL)
							pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_REGISTER_MEMO );
						
						g_pData->GetUIManager()->GetUISkillNote()->RegisterMemo( pItem->GetUID() );
					}

					return true;					
				}			
#endif // REFORM_SKILL_NOTE_UI
	

				// 메모사용 불가
				return true;
			}			
#endif

#ifdef	REAL_TIME_ELSWORD
			if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
			{
                switch ( pItem->GetItemTemplet()->GetItemID() )
				{
				case WARP_ITEM_RUBEN_ITEM_ID:					// 루벤 마을 귀환서
				case WARP_ITEM_ELDER_ITEM_ID:					// 엘더 마을 귀환서
				case WARP_ITEM_BESMA_ITEM_ID:					// 베스마 마을 귀환서
				case WARP_ITEM_ALTERA_ITEM_ID:					// 알테라 마을 귀환서
				case WARP_ITEM_PEITA_DUNGEON_GATE_ITEM_ID:		// 페이타 던전 게이트 이동 티켓
				case WARP_ITEM_VELDER_ITEM_ID:					// 벨더 마을 귀환서
				case NICKNAME_CHANGE_CARD_ITEM_ID:				// 캐릭터 닉네임 변경 카드 ID
				case INIT_SKILL_TREE_ITEM:					// 스킬 초기화 메달
				case INIT_SKILL_TREE_EVENT_ITEM_ID:				// 망각의 드링크
				case CI_SKILL_INIT_EVENT_ITEM2:				// 망각의 드링크 1일권, 퀘스트 보상용
				case CI_SKILL_INIT_EVENT_ITEM3:				// 망각의 드링크 3일권, 퀘스트 보상용
				case CI_SKILL_INIT_EVENT_ITEM4:				// 망각의 드링크 밸런스 개편
#ifdef SERV_GLOBAL_DEFINE
				case INIT_SKILL_TREE_EVENT_ITEM_ID_JP:			// 망각의 드링크(국내)
#endif //SERV_GLOBAL_DEFINE
#ifdef SERV_EVENT_RURIEL_RESET_SKILL_ITEM
				case RURIEL_RESET_SKILL_ITEM:					// 루리엘의 스킬 초기화 메달
#endif SERV_EVENT_RURIEL_RESET_SKILL_ITEM
#ifdef SERV_VIP_SYSTEM
				case VIP_WARP_ITEM_FREE_ITEM_ID:				// VIP이동티켓 자유 이용권
#endif //SERV_VIP_SYSTEM
#ifdef FREE_WARP
				case WARP_ITEM_FREE_ITEM_ID:					// 이동티켓 자유 이용권
#endif

	//{{ kimhc // 2009-08-26 // 봉인의 인장 던전 내에서 사용 막기
#ifdef	SEAL_ITEM
				case ITEM_FOR_SEAL_NORMAL:								// 고양이 발 도장(하얀색)
				case ITEM_FOR_SEAL_RARE:								// 고양이 발 도장(노란색)
				case ITEM_FOR_SEAL_ELITE:								// 고양이 발 도장(보라색)
				case ITEM_FOR_SEAL_UNIQUE:								// 고양이 발 도장(황금색)
					
				case ITEM_FOR_SEAL_NORMAL_EVENT:						// 고양이 발 도장 이벤트(하얀색)
				case ITEM_FOR_SEAL_ELITE_EVENT:							// 고양이 발 도장 이벤트(보라색)
#ifdef CHILDRENS_DAY_EVENT_ITEM
				case ITEM_FOR_SEAL_UNIQUE_EVENT:						// 고양이 발 도장 이벤트(황금색)
#endif //CHILDRENS_DAY_EVENT_ITEM
#ifdef SERV_COUNTRY_JP
				case ITEM_FOR_SEAL_RARE_EVENT:							// 고양이 발 도장 이벤트(노란색)		
				case ITEM_FOR_SEAL_UNIQUE_EVENT:						// 고양이 발 도장 이벤트(황금색)
#endif //SERV_COUNTRY_JP
#ifdef SERV_RURIEL_EVENT_ITEM
				case ITEM_FOR_SEAL_NORMAL_EVENT_RURIEL:
				case ITEM_FOR_SEAL_RARE_EVENT_RURIEL:
				case ITEM_FOR_SEAL_ELITE_EVENT_RURIEL:
				case ITEM_FOR_SEAL_UNIQUE_EVENT_RURIEL:
#endif //SERV_RURIEL_EVENT_ITEM
#ifdef SERV_ARIEL_EVENT_ITEM
				case ITEM_FOR_SEAL_NORMAL_EVENT_ARIEL:
				case ITEM_FOR_SEAL_RARE_EVENT_ARIEL:
				case ITEM_FOR_SEAL_ELITE_EVENT_ARIEL:
				case ITEM_FOR_SEAL_UNIQUE_EVENT_ARIEL:
#endif //SERV_ARIEL_EVENT_ITEM
#endif	SEAL_ITEM
	//}} kimhc // 2009-08-26 // 고양이 발 도장 던전 내에서 사용 막기
#ifdef SKILL_PLUS_ITEM_USE_POPUP
				case SKILL_PLUS_ITEM_ID:			// oasis907 : 김상윤 [2010.12.9] // 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
				case SKILL_PLUS_ITEM_ID_JP:
				case SKILL_POINT_10_30DAY_USE_INVEN_JP:
				case SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
				case SKILL_POINT_30_7DAY_USE_INVEN:
				case SKILL_POINT_60_7DAY_USE_INVEN:
				case SKILL_POINT_60_15DAY_USE_INVEN:
				case SKILL_POINT_60_30DAY_USE_INVEN:
				case SKILL_POINT_60_7DAY_USE_INVEN_2:
				case SKILL_POINT_30_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
				case SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
				case SKILL_POINT_30_1DAY_USE_INVEN:
				case SKILL_POINT_60_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_SKILL_5_POINT_7_DAY_EU				
				case EVENT_SKILL_5_POINT_7_DAY:
#endif SERV_SKILL_5_POINT_7_DAY_EU
#endif SKILL_PLUS_ITEM_USE_POPUP
					//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
				case SKILL_POINT_5_USE_INVEN_ITEM_ID:
				case SKILL_POINT_10_USE_INVEN_ITEM_ID:

#endif SERV_ADD_PACKAGE_PRODUCT

#ifdef ADDED_RELATIONSHIP_SYSTEM
//				case WEDDING_RING_ITEM_ID : 후에 추가해줄 것!
//				case INVITE_WEDDING_LETTER_ITEM_ID :
#endif // ADDED_RELATIONSHIP_SYSTEM
					//}}
#ifdef SERV_GOLD_TICKET
				case GOLD_TICKET_10_ID:
				case GOLD_TICKET_50_ID:
				case GOLD_TICKET_100_ID:
				case GOLD_TICKET_500_ID:
				case GOLD_TICKET_1000_ID:
				case GOLD_TICKET_2000_ID:
#endif SERV_GOLD_TICKET
#ifdef SERV_EVENT_INIT_SKILL_TREE_ITEM
				case INIT_SKILL_TREE_ITEM_TW:
#endif SERV_EVENT_INIT_SKILL_TREE_ITEM
#ifdef SERV_CN_GNOSIS
				case EVENT_SKILL_POINT_10_USE_INVEN_7_DAY:
				case EVENT_SKILL_POINT_5_USE_INVEN_1_DAY:
				case EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
				case EVENT_SKILL_POINT_5_USE_INVEN_7_DAY_TYPE2:
				case EVENT_SKILL_POINT_10_USE_INVEN_15_DAY:
#endif SERV_CN_GNOSIS
#ifdef SERV_GNOSIS_BR
				case EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
				case EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_US_GNOSIS
				case EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_US_GNOSIS
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
				case SKILL_POINT_30_14DAY_USE_INVEN:	
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
				case SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
					{
						g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2646 ),g_pMain->GetNowState() );
						return true;
					}					
				default:
					break;
				}
			}
#endif	REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 던전내에서 아이템 사용시 막아야 할 것들

            switch ( pItem->GetItemTemplet()->GetItemID() )
			{
			case WARP_ITEM_RUBEN_ITEM_ID:					// 루벤 마을 귀환서
			case WARP_ITEM_ELDER_ITEM_ID:					// 엘더 마을 귀환서
			case WARP_ITEM_BESMA_ITEM_ID:					// 베스마 마을 귀환서
			case WARP_ITEM_ALTERA_ITEM_ID:					// 알테라 마을 귀환서
			case WARP_ITEM_PEITA_DUNGEON_GATE_ITEM_ID:		// 페이타 던전 게이트 이동 티켓
			case WARP_ITEM_VELDER_ITEM_ID:					// 벨더 마을 귀환서
				{
					switch ( g_pData->GetLocationManager()->GetCurrentVillageID() )
					{
					case SEnum::VMI_HAMEL:
					case SEnum::VMI_SANDER:
				
						{
							g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_11176 ),g_pMain->GetNowState() );
							return true;
						} break;

					default:
						break;
					} // if
				} break;

			default:
				break;
			}
						
			// 칭호사용 아이템
			for(int i=0; i<ARRAY_SIZE(TITLE_ITEM); ++i)
			{
                if( pItem->GetItemTemplet()->GetItemID() == TITLE_ITEM[i] )
				{
					wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_3712, "L", pItem->GetFullName()) );
					m_TitleItemUID = pItemSlot->GetItemUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), UIM_ITEM_USE_TITLE, g_pMain->GetNowState() );
					return true;
				}
			}


			if( true == g_pData->GetSkillTree()->IsUnsealSkillItemID( pItem->GetItemTemplet()->GetItemID() ) )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				const CX2SkillTree::SKILL_ID iSkillID = g_pData->GetSkillTree()->GetUnsealSkillItemInfo( pItem->GetItemTemplet()->GetItemID() );

				if ( CX2SkillTree::SI_NONE == iSkillID )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_3856 ), g_pMain->GetNowState() );
					return false;
				}

				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( iSkillID );
	#else // UPGRADE_SKILL_SYSTEM_2013
				int iSkillID = 0;
				char cUnitClass = 0;

				if( false == g_pData->GetSkillTree()->GetUnsealSkillItemInfo( 
					pItem->GetItemTemplet()->GetItemID(), 
					iSkillID, cUnitClass ) )
					return false;

				// 내 유닛이 사용할 수 있는 필살기 인지 확인
				if( false == GetX2UnitClassCompatibility( g_pData->GetMyUser()->GetSelectUnit()->GetClass(), (CX2Unit::UNIT_CLASS ) cUnitClass ) )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_3856 ), g_pMain->GetNowState() );
					return false;
				}

				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) iSkillID, 1 );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				ASSERT( NULL != pSkillTemplet );
				if( NULL == pSkillTemplet )
					return false;

				wstring wstrMsg = L"";
				if( true == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.IsSkillUnsealed( (CX2SkillTree::SKILL_ID) iSkillID ) )
				{
					wstrMsg = GET_REPLACED_STRING( (STR_ID_3855, "L", pSkillTemplet->m_wstrName) );
				}
				else
				{
					wstrMsg = GET_REPLACED_STRING( (STR_ID_3854, "L", pSkillTemplet->m_wstrName) );
				}
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), UIM_ITEM_USE_SKILL_UNSEAL, g_pMain->GetNowState() );		// 아이템을 사용하면 *** 스킬의 봉인해제 합니다 


				m_UsedItemUID = pItem->GetUID();
				return true;
			}


			// 마을추가시 마을귀환서 추가
            switch(pItem->GetItemTemplet()->GetItemID())
			{
			case WARP_ITEM_RUBEN_ITEM_ID: // 루벤마을 귀환서
				{
					m_WarpItem = pItemSlot->GetItemUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_801 ), UIM_ITEM_USE_WARP, g_pMain->GetNowState() );
					return true;
				} break;
			case WARP_ITEM_ELDER_ITEM_ID: // 엘더마을 귀환서
				{
					m_WarpItem = pItemSlot->GetItemUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_802 ), UIM_ITEM_USE_WARP, g_pMain->GetNowState() );
					return true;
				} break;	
			case WARP_ITEM_BESMA_ITEM_ID: // 베스마마을 귀환서
				{
					m_WarpItem = pItemSlot->GetItemUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_803 ), UIM_ITEM_USE_WARP, g_pMain->GetNowState() );
					return true;
				} break;
			case WARP_ITEM_ALTERA_ITEM_ID: // 알테라마을 귀환서
				{
					m_WarpItem = pItemSlot->GetItemUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_804 ), UIM_ITEM_USE_WARP, g_pMain->GetNowState() );
					return true;
				} break;
			case WARP_ITEM_PEITA_DUNGEON_GATE_ITEM_ID: // 페이타 던전게이트 이동티켓
				{
					m_WarpItem = pItemSlot->GetItemUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2628 ), UIM_ITEM_USE_WARP, g_pMain->GetNowState() );
					return true;
				}
				break;
			case WARP_ITEM_VELDER_ITEM_ID: // 벨더마을 귀환서
				{
					m_WarpItem = pItemSlot->GetItemUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4649 ), UIM_ITEM_USE_WARP, g_pMain->GetNowState() );
					return true;
				} break;
			case NICKNAME_CHANGE_CARD_ITEM_ID: // 캐릭터 닉네임 변경 카드 ID
				{
					m_UseItemTempUID = pItem->GetUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_805 ), UIM_CHANGE_NICK_NAME_OK, g_pMain->GetNowState() );
					return true;
				} break;

			case INIT_SKILL_TREE_ITEM:			// 스킬 초기화 메달
			case INIT_SKILL_TREE_EVENT_ITEM_ID:		// 망각의 드링크
			case CI_SKILL_INIT_EVENT_ITEM2: // 망각의 드링크 1일권, 퀘스트 보상용
			case CI_SKILL_INIT_EVENT_ITEM3: // 망각의 드링크 3일권, 퀘스트 보상용
			case CI_SKILL_INIT_EVENT_ITEM4: // 망각의 드링크 밸런스 개편
#ifdef SERV_GLOBAL_DEFINE
			case INIT_SKILL_TREE_EVENT_ITEM_ID_JP:			// 망각의 드링크(국내)
#endif //SERV_GLOBAL_DEFINE
#ifdef SERV_EVENT_INIT_SKILL_TREE_ITEM
			case INIT_SKILL_TREE_ITEM_TW:	// 망각의 드링크
#endif SERV_EVENT_INIT_SKILL_TREE_ITEM			
#ifdef SERV_EVENT_RURIEL_RESET_SKILL_ITEM
			case RURIEL_RESET_SKILL_ITEM:	// 루리엘의 스킬 초기화 메달
#endif SERV_EVENT_RURIEL_RESET_SKILL_ITEM
				{
					if( true == pUnitData->m_UserSkillTree.CanIInitSkillTree() )
					{
						m_UsedItemUID = pItem->GetUID();
					#ifdef SKILL_PAGE_SYSTEM //JHKang
						#ifdef NO_SKILL_PAGE_SYSTEM_INT
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_478 ), UIM_SKILL_INIT_OK, g_pMain->GetNowState() );
						#else //NO_SKILL_PAGE_SYSTEM_INT
						wstring wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_29930, "i", pUnitData->m_UserSkillTree.GetUsingPage() + 1 ) );
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrTextNotice.c_str(), UIM_SKILL_INIT_OK, g_pMain->GetNowState() );
						#endif //NO_SKILL_PAGE_SYSTEM_INT
					#else //SKILL_PAGE_SYSTEM
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_478 ), UIM_SKILL_INIT_OK, g_pMain->GetNowState() );
					#endif //SKILL_PAGE_SYSTEM
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2679 ), g_pMain->GetNowState() );
					}
					return true;
				} break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
			case COUPLE_NICKNAME_CHANGE_ITEM_ID :
			{
				if ( NULL != g_pData->GetRelationshipManager() &&
					NULL != g_pData->GetRelationshipManager()->GetUIRelationship() )
				{
					if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType 
						> SEnum::RT_COUPLE ) 
					{
						g_pData->GetRelationshipManager()->GetUIRelationship()->ShowChangeLoveMessageDlg( true, pItem->GetUID() );
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_24620 ), g_pMain->GetNowState() );
					}
				}
				return true;
			}
			break;

			case WEDDING_RING_ITEM_ID :
			{
				if ( NULL != g_pData->GetRelationshipManager() &&
					 NULL != g_pData->GetRelationshipManager()->GetUIRelationship() &&
					 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{					
					
					if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType ==
						SEnum::RT_COUPLE )
					{
						// 커플 경과 시간 체크
						if ( true == g_pData->GetRelationshipManager()->CheckWeddingQualification() )
						{
							// 결혼 반지 소지 여부 체크
							if ( NULL != g_pData->GetMyUser() &&
								NULL != g_pData->GetMyUser()->GetSelectUnit() &&
								NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID ( WEDDING_RING_ITEM_ID) )
							{	
								const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

								// 초대권 소지 개수 체크
								int iWeddingLetterMany = 0;
								CX2Item * pInviteLetterItem = kInventory.GetItemByTID ( INVITE_WEDDING_ITEM_ID);
								if( NULL != pInviteLetterItem )
								{
									iWeddingLetterMany = pInviteLetterItem->GetItemData().m_Quantity;
								}

								// 소지중엔 예약권으로 예식장 선택
								g_pData->GetRelationshipManager()->GetUIRelationship()->SetSelecteWeddingHallByPropose( iWeddingLetterMany );
							}
							else
							{// 결혼 반지 없을 때
								const CX2Item::ItemTemplet * pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( WEDDING_RING_ITEM_ID );
								if ( NULL != pItemTemplet )
								{			
									wstring wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_24451, "S", pItemTemplet->GetName() ) );													
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrTextNotice.c_str(), g_pMain->GetNowState() );
								}	
							}					
						}
					}
#ifdef SERV_RELATIONSHIP_SYSTEM_INT
					else
					{
						g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(250,300), GET_STRING( STR_ID_28122 ),g_pMain->GetNowState() );
					}
#else
					else
					{
						g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(250,300), GET_STRING( STR_ID_24476 ),g_pMain->GetNowState() );
					}
#endif SERV_RELATIONSHIP_SYSTEM_INT
					return true;
				}		
			}
			break;

			case COUPLE_RING_ITEM_ID:								
			{
				if ( NULL != g_pData->GetRelationshipManager() &&
					 NULL != g_pData->GetRelationshipManager()->GetUIRelationship() )
				{
					g_pData->GetRelationshipManager()->GetUIRelationship()->ShowInviteCoupleDlg( true );
				}
				return true;
			}
			break;

			case RESERVED_WEDDING_LETTER_ITEM_ID :
				{
					if (NULL != g_pData->GetRelationshipManager() &&
						NULL != g_pData->GetRelationshipManager()->GetUIRelationship() )
					{		
						g_pData->GetRelationshipManager()->GetUIRelationship()->ShowJoinWeddingDlg ( true, true, pItem->GetUID () );
					}
					return true;
				} break;
			case INVITE_WEDDING_LETTER_ITEM_ID :								
				{
					if (NULL != g_pData->GetRelationshipManager() &&
						NULL != g_pData->GetRelationshipManager()->GetUIRelationship() 
						)
					{		
						g_pData->GetRelationshipManager()->GetUIRelationship()->ShowJoinWeddingDlg ( true, false, pItem->GetUID () );
					}
					return true;
				}
				break;
#endif // ADDED_RELATIONSHIP_SYSTEM
				
				//{{ 허상형 : [2009/7/14] //	나소드 메가폰 관련
			case NASOD_SCOPE_ITEM_ID:
			case NASOD_SCOPE_HIGH_ITEM_ID:
#ifdef SERV_VIP_SYSTEM
			case EVENT_NASOD_SCOPE_ITEM_ID:
			case EVENT_NASOD_SCOPE_HIGH_ITEM_ID:
#endif //SERV_VIP_SYSTEM
				{
					g_pChatBox->SetItemSlot( pItemSlot );

					g_pChatBox->ShowNasodMessageDlg( true );
					return true;
				} break;
				//}} 허상형 : [2009/7/14] //	나소드 메가폰 관련
#ifdef CHANGE_PET_NAME 
			case CHANGE_PET_NAME_ITEM_ID:
				{
					if ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
					{
						if( g_pData != NULL && g_pData->GetPetManager() != NULL)
						{
							if(g_pData->GetPetManager()->GetMyPet() == NULL)
							{ 
								//소환된 펫이 없습니다.
#ifdef SERV_USE_GetErrStrF
								g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), NetError::GetErrStrF(NetError::ERR_PET_06) ,g_pMain->GetNowState() );  
#else SERV_USE_GetErrStrF
								g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), NetError::GetErrStr(NetError::ERR_PET_06) ,g_pMain->GetNowState() );  
#endif SERV_USE_GetErrStrF
							}
							else
							{
								// 펫 이름변경
								g_pData->GetPetManager()->ShowChangePetName(true, pItem->GetUID());  
							}
							return true;
						}
					} 
					else
					{
						g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4495 ),g_pMain->GetNowState() );
						return true;
					}  
				}
				break;		 
#endif CHANGE_PET_NAME

	#ifdef PET_DROP_ITEM_PICKUP 
			case ACTIVATION_DROP_ITEM_PICKUP_SKILL:
	#ifdef SERV_PET_AUTO_LOOTING_ITEM_CN
			case ACTIVATION_DROP_ITEM_PICKUP_SKILL_CN:
	#endif // SERV_PET_AUTO_LOOTING_ITEM_CN
				{
					if( NULL != g_pData && NULL != g_pData->GetPetManager() )						
					{
						//인벤토리에서 사용 할 때는 무조건 소환된 펫에게 적용 시키기 위해 getmypet이용
						CX2PET* pMyPet = g_pData->GetPetManager()->GetMyPet();
						if( NULL != pMyPet )
						{				
							CX2PET::PetInfo& pMyPetInfo = pMyPet->GetPetInfo();
#ifdef SERV_EVENT_VC
							// 용병 뽀루에게는 아이템 사용 안되도록
							if( pMyPetInfo.m_PetId == CX2PetManager::PUI_PET_MERCENARY_PPORU_EVENT_INT )
							{
								g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_813 ) ,g_pMain->GetNowState() );
								return true;
							}
#endif //SERV_EVENT_VC
							//아이템 사용 불가 예외처리
							//1. 펫의 진화단계가 유체 이상 일 때만 사용 가능
							//2. 아이템 줍기 스킬이 봉인되어 있는 상태 일 때만 사용 가능
							CX2PetManager::PetTemplet* pPetTemplet = 
								g_pData->GetPetManager()->GetPetTemplet( static_cast<CX2PetManager::PET_UNIT_ID>(pMyPetInfo.m_PetId) );
							UINT uiEvolutionStep = static_cast<UINT>(pMyPetInfo.m_Evolution_Step);//펫 진화 단계
							if( NULL != pPetTemplet && 
								uiEvolutionStep < pPetTemplet->m_vecPetStatus.size() ) //벡터의 크기가 인덱스보다 클 때만 접근
							{
								int iPetStatus = pPetTemplet->m_vecPetStatus[ uiEvolutionStep ];
								if( 0 == iPetStatus )  //알 단계 일 때
								{
									//아이템 줍기 스킬은 펫의 진화 단계가 유체 이상 일 때만 사용 가능 합니다.
									g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_21657 ) ,g_pMain->GetNowState() );
									return true;
								}
								else //유체 이상이라면 이미 봉인 해제 되어 있는지 확인
								{
									if( true == pMyPetInfo.m_bIsDropItemPickup )
									{
										//아이템 줍기 스킬이 이미 활성화 되어 있기 때문에 사용 할 수 없습니다.
										g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_21658 ) ,g_pMain->GetNowState() );
										return true;
									}
								}
							}
							//아이템 사용 가능한 상태라면
							m_UsedItemUID = pItem->GetUID();
							m_UseItemTempUID = pMyPetInfo.m_PetUid;
							//현재 소환된 '@1'펫의 아이템 줍기 스킬을 해제 하시겠습니까?
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
								GET_REPLACED_STRING( ( STR_ID_21659, "L", pMyPetInfo.m_wstrPetName ) ),
								UIM_PET_DROP_ITEM_PICK_UP_OK, g_pMain->GetNowState() );
							return true;
						}
						else
						{	//소환된 펫이 없습니다.
							if( NULL != g_pData->GetUIManager() && //펫 미보유
								NULL != g_pData->GetUIManager()->GetUIPetList() &&
								0 == g_pData->GetUIManager()->GetUIPetList()->GetPetListSize() )
							{
								g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_21662 ) ,g_pMain->GetNowState() );  
							}
							else
							{
								g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), 
									GET_REPLACED_STRING( ( STR_ID_21660, "L", g_pMain->GetKeyPad()->GetKeyString( GA_PET ) ) ),
									g_pMain->GetNowState() );  
							}
							return true;
						}
					}
					return true;
				}
				break;		 
	#endif //PET_DROP_ITEM_PICKUP

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
			case HALLOWEEN_TRANSFORM_POSION:
				{
					if( NULL != g_pData && NULL != g_pData->GetPetManager() )						
					{
						//인벤토리에서 사용 할 때는 무조건 소환된 펫에게 적용 시키기 위해 getmypet이용
						CX2PET* pMyPet = g_pData->GetPetManager()->GetMyPet();
						if( NULL != pMyPet )
						{	
							CX2PET::PetInfo& pMyPetInfo = pMyPet->GetPetInfo();
							//아이템 사용 불가 예외처리
							//1. 할로윈 펫의 진화단계가 완전체 일 때만 사용 가능
							//2. 친밀도 100%일때만 사용 가능

							if( pMyPetInfo.m_PetId == CX2PetManager::PUI_HALLOWEEN_PUMPKIN_TANYA || pMyPetInfo.m_PetId == CX2PetManager::PUI_HALLOWEEN_PUMPKIN_LAEL ||
								pMyPetInfo.m_PetId == CX2PetManager::PUI_HALLOWEEN_PUMPKIN_LOJETA || pMyPetInfo.m_PetId == CX2PetManager::PUI_HALLOWEEN_PUMPKIN_NARENEA ||
								pMyPetInfo.m_PetId == CX2PetManager::PUI_HALLOWEEN_PUMPKIN_TANYA_F || pMyPetInfo.m_PetId == CX2PetManager::PUI_HALLOWEEN_PUMPKIN_LAEL_F ||
								pMyPetInfo.m_PetId == CX2PetManager::PUI_HALLOWEEN_PUMPKIN_LOJETA_F || pMyPetInfo.m_PetId == CX2PetManager::PUI_HALLOWEEN_PUMPKIN_NARENEA_F )
							{
								CX2PetManager::PetTemplet* pPetTemplet = 
									g_pData->GetPetManager()->GetPetTemplet( static_cast<CX2PetManager::PET_UNIT_ID>(pMyPetInfo.m_PetId) );
								UINT uiEvolutionStep = static_cast<UINT>(pMyPetInfo.m_Evolution_Step);//펫 진화 단계

								if( NULL != pPetTemplet && 
									uiEvolutionStep < pPetTemplet->m_vecPetStatus.size() ) //벡터의 크기가 인덱스보다 클 때만 접근
								{
									int iPetStatus = pPetTemplet->m_vecPetStatus[ uiEvolutionStep ];
									if( 3 == iPetStatus )  //완전체일때
									{
										if( pMyPet->GetNowIntimacy() >= 1.0f )	// 친밀도가 100% 일때
										{
											m_UseTransformdItemUID = pItem->GetUID();
											m_UseTransformItemPetUID = pMyPetInfo.m_PetUid;
											g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_PET_HALLOWEEN_TRANSFORM_POSION_OK, g_pMain->GetNowState() );
										}
										else // 친밀도가 100% 아니면 안됨
										{
											g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_28199 ) ,g_pMain->GetNowState() );
										}
									}
									else // 완전체가 아니면 안됨~
									{
										g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_28197 ) ,g_pMain->GetNowState() );
									}
								}
							}
							else
							{
								g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_28197 ) ,g_pMain->GetNowState() );
							}
						}
						else
						{
							g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_28199 ), g_pMain->GetNowState() );
						}
					}
					return true;
				}
				break;	
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

				//{{ 허상형 : [2009/9/8] //		길드 관련 아이템 사용
#ifdef GUILD_MANAGEMENT
			case GUILD_CREATE_ITEM_ID:
				{
					if ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
					{
						if ( g_pData->GetGuildManager()->DidJoinGuild() == false )
							g_pData->GetGuildManager()->GetUIGuild()->SetShowCreateGuildDlg( true, pItemSlot->GetItemUID() );
						else
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4498 ), g_pMain->GetNowState() );
						}

						return true;
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4495 ),g_pMain->GetNowState() );
						return true;
					}

				} break;
#endif	//	GUILD_MANAGEMENT
				//}} 허상형 : [2009/9/8] //		길드 관련 아이템 사용
#ifdef FREE_WARP
			case WARP_ITEM_FREE_ITEM_ID: 
#ifdef SERV_VIP_SYSTEM
			case VIP_WARP_ITEM_FREE_ITEM_ID:
#endif //SERV_VIP_SYSTEM
				{
					UseWarpItem(pItemSlot, false); //ED소모없이 사용하는 자유이용권 
					return true;
				}
			case WARP_ITEM_ED_CONSUMPTION_ITEM_ID:
				{
					UseWarpItem(pItemSlot, true);  //ED소모하고 사용하는 자유이용권 
					return true;
				}	/// case
				break;
#endif

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
			case UNLIMITED_SECOND_CHANGE_JOB_ITEM_ID:
				{
					UseUSCJobItem(pItemSlot);  
					return true;
				}
				break;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef GUILD_SKILL
				//{{ oasis907 : 김상윤 [2009.11.30] // 길드 스킬 초기화
			case INIT_GUILD_SKILL_TREE_ITEM_ID:
				{
					if( true == pUnitData->m_UserSkillTree.CanIInitGuildSkillTree() )
					{
						m_UsedItemUID = pItem->GetUID();
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4802 ), UIM_GUILD_SKILL_INIT_OK, g_pMain->GetNowState() );
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2679 ), g_pMain->GetNowState() );
					}
					return true;
				} break;
				//}} oasis907 : 김상윤 [2009.11.30] //

			case GUILD_CASH_SKILL_ITEM_ID:
#ifdef EVENT_GUILD_ITEM
			case EVENT_GUILD_CASH_SKILL_ITEM_ID:
#endif //EVENT_GUILD_ITEM
#ifdef SERV_EVENT_GUILD_SKILL_GLOBAL
			case EVENT_GUILD_SKILL_5_POINT_15_DAY_JP:
#endif //SERV_EVENT_GUILD_SKILL_GLOBAL
				{
					//{{ oasis907 : 김상윤 [2009.11.19] // 길드마스터가 아닌 경우 막음
					if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade != CX2GuildManager::GUG_MASTER)
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_STRING( STR_ID_4801 ), g_pMain->GetNowState() );
						return true;
					}
					else
					{
						Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( pItemSlot->GetItemUID() );
						return true;
					}
				}			
#endif GUILD_SKILL

#ifdef SKILL_PLUS_ITEM_USE_POPUP
			case SKILL_PLUS_ITEM_ID: // oasis907 : 김상윤 [2010.12.9] // 그노시스 축복 사용
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
			case SKILL_PLUS_ITEM_ID_JP:
			case SKILL_POINT_10_30DAY_USE_INVEN_JP:
			case SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
			case SKILL_POINT_30_7DAY_USE_INVEN:
			case SKILL_POINT_60_7DAY_USE_INVEN:
			case SKILL_POINT_60_15DAY_USE_INVEN:
			case SKILL_POINT_60_30DAY_USE_INVEN:
			case SKILL_POINT_60_7DAY_USE_INVEN_2:
			case SKILL_POINT_30_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
			case SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
			case SKILL_POINT_30_1DAY_USE_INVEN:
			case SKILL_POINT_60_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
				//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
			case SKILL_POINT_5_USE_INVEN_ITEM_ID:
			case SKILL_POINT_10_USE_INVEN_ITEM_ID:
			case SKILL_POINT_5_USE_INVEN_ITEM_ID_7_DAY:
#endif SERV_ADD_PACKAGE_PRODUCT
#ifdef SERV_SKILL_5_POINT_7_DAY_EU		
			case EVENT_SKILL_5_POINT_7_DAY:	
#endif SERV_SKILL_5_POINT_7_DAY_EU
				//}}
#ifdef SERV_CN_GNOSIS
			case EVENT_SKILL_POINT_10_USE_INVEN_7_DAY:
			case EVENT_SKILL_POINT_5_USE_INVEN_1_DAY:
			case EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
			case EVENT_SKILL_POINT_5_USE_INVEN_7_DAY_TYPE2:
			case EVENT_SKILL_POINT_10_USE_INVEN_15_DAY:
#endif SERV_CN_GNOSIS
			case SKILL_POINT_5_USE_INVEN_ITEM_ID_15_DAY :
			case SKILL_POINT_5_USE_INVEN_ITEM_ID_30_DAY :
			case SKILL_POINT_5_USE_INVEN_ITEM_ID_60_DAY :
			case SKILL_POINT_10_USE_INVEN_ITEM_ID_30_DAY :
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			case SKILL_POINT_30_USE_INVEN_ITEM_ID_15_DAY :
			case SKILL_POINT_60_USE_INVEN_ITEM_ID_15_DAY :
			case SKILL_POINT_30_USE_INVEN_ITEM_ID_30_DAY :
			case SKILL_POINT_60_USE_INVEN_ITEM_ID_30_DAY :
#endif // UPGRADE_SKILL_SYSTEM_2013
				//}}
#ifdef SERV_US_GNOSIS
			case EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_US_GNOSIS
#ifdef SERV_GNOSIS_BR
			case EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
			case EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
			case SKILL_POINT_30_14DAY_USE_INVEN:	
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
			case LURIEL_GNOSIS_30_15DAY:
			case LURIEL_GNOSIS_30_30DAY:
			case LURIEL_GNOSIS_30_60DAY:
			case LURIEL_GNOSIS_60_15DAY:
			case LURIEL_GNOSIS_60_30DAY:
			case LURIEL_GNOSIS_60_60DAY:
			case LURIEL_GNOSIS_30_7DAY:
			case LURIEL_GNOSIS_60_7DAY:
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
			case SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
				{
					m_UsedItemUID = pItemSlot->GetItemUID();
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_9835 ), UIM_SKILL_PLUS_ITEM_USE, g_pMain->GetNowState() );
					return true;
				} break;
#endif SKILL_PLUS_ITEM_USE_POPUP				

#ifdef BUFF_TEMPLET_SYSTEM
				//비약 사용 시
			case PROTECTION_OF_NUT_ITEM_ID:
			case PROTECTION_OF_PUTA_ITEM_ID:
			case PROTECTION_OF_LAHELL_ITEM_ID: 
			case PROTECTION_OF_JAKIELL_ITEM_ID:
				//영약 사용 시
			case BELSSING_OF_SERAPHIM_ITEM_ID:
			case BELSSING_OF_GEV_ITEM_ID:
			case BELSSING_OF_AMON_ITEM_ID: 
			case BELSSING_OF_CRONOS_ITEM_ID:

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-11
			case ELIXIR_GIANT_POTION:
			case ELIXIR_BLAZING_BOMB:
			case ELIXIR_SPIRIT_OF_CHASER:
			case ELIXIR_CRADLE_OF_LITTLE_FAIRY:
			case ELIXIR_ICE_BALL_OF_DENIPH:
			case ELIXIR_FEATHER_OF_VENTUS:
			case ELIXIR_FLAME_RING_OF_ROSSO:
			case ELIXIR_BIG_HAND_POTION:
#endif // SERV_NEW_DEFENCE_DUNGEON
#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
			case THE_ESSENCE_OF_WEAK_HERETIC_POTION:
			case THE_ESSENCE_OF_HERETIC_POTION:
#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
			case BUFF_RURIEL_MANA_ENERGIZE_POTION:
#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
				{
					switch ( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_DUNGEON_GAME:
					case CX2Main::XS_BATTLE_FIELD:
						{
							m_DefencedungeonPotion	 = pItemSlot->GetItemUID(); 
                            m_DefencedungeonPotionID = pItem->GetItemTemplet()->GetItemID();

							//적용중인 영약 버프가 없다면, 사용 할 영약의 버프 적용
							Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_DefencedungeonPotion, 1, m_DefencedungeonPotionID );
						} break;

					default:
						{
							// 던전이 아니면 사용할 수 없다.
							g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_16529 ),g_pMain->GetNowState() );
							return true;
						} break;
					}					
				} break;
#endif BUFF_TEMPLET_SYSTEM
				//{{ 2011.10.24 이지헌 : 골드 티켓 작업 ( 사용하면 캐쉬 주는 아이템 )
#ifdef SERV_GOLD_TICKET
			case GOLD_TICKET_10_ID:
			case GOLD_TICKET_50_ID:
			case GOLD_TICKET_100_ID:
			case GOLD_TICKET_500_ID:
			case GOLD_TICKET_1000_ID:
			case GOLD_TICKET_2000_ID:
				{
					switch ( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_VILLAGE_MAP:
						{
							m_UsedItemUID = pItemSlot->GetItemUID();
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_3712, "L", pItem->GetFullName()) ), UIM_USE_GOLD_TICKET, g_pMain->GetNowState() );
						}
						break;
					default:
						{
							g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24660 ),g_pMain->GetNowState() );
							return true;
						}
						break;
					}
				}
				break;
#endif //SERV_GOLD_TICKET
				//}}
#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
			case INVENTORY_SLOT_ADD_ITEM_EQUIP_EVENT:
				{

					// 지헌 : 사용 전 인벤 확장이 가능 한가 테스트를 하자.
					if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum(CX2Inventory::ST_EQUIP) < INVENTORY_SLOT_MAX_NUM)
					{
						// 지헌 : 번역 - 스트링 넣어야 한다.
						m_UsedItemUID = pItemSlot->GetItemUID();
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_13554 ), UIM_USE_EXPAND_INVENTORY_OK, g_pMain->GetNowState() );
					}
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), NetError::GetErrStrF( NetError::ERR_BUY_CASH_ITEM_19 ), g_pMain->GetNowState() );
				} break;
			case INVENTORY_SLOT_ADD_ITEM_ACCESSORY_EVENT:
				{

					// 지헌 : 사용 전 인벤 확장이 가능 한가 테스트를 하자.
					if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum(CX2Inventory::ST_ACCESSORY) < INVENTORY_SLOT_MAX_NUM)
					{
						// 지헌 : 번역 - 스트링 넣어야 한다.
						m_UsedItemUID = pItemSlot->GetItemUID();
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_13554 ), UIM_USE_EXPAND_INVENTORY_OK, g_pMain->GetNowState() );
					}
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), NetError::GetErrStrF( NetError::ERR_BUY_CASH_ITEM_19 ), g_pMain->GetNowState() );
				} break;
			case INVENTORY_SLOT_ADD_ITEM_QUICK_SLOT_EVENT:
				{

					// 지헌 : 사용 전 인벤 확장이 가능 한가 테스트를 하자.
					if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum(CX2Inventory::ST_QUICK_SLOT) < INVENTORY_SLOT_MAX_NUM)
					{
						// 지헌 : 번역 - 스트링 넣어야 한다.
						m_UsedItemUID = pItemSlot->GetItemUID();
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_13554 ), UIM_USE_EXPAND_INVENTORY_OK, g_pMain->GetNowState() );
					}
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), NetError::GetErrStrF( NetError::ERR_BUY_CASH_ITEM_19 ), g_pMain->GetNowState() );
				} break;
			case INVENTORY_SLOT_ADD_ITEM_MATERIAL_EVENT:
				{

					// 지헌 : 사용 전 인벤 확장이 가능 한가 테스트를 하자.
					if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum(CX2Inventory::ST_MATERIAL) < INVENTORY_SLOT_MAX_NUM)
					{
						// 지헌 : 번역 - 스트링 넣어야 한다.
						m_UsedItemUID = pItemSlot->GetItemUID();
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_13554 ), UIM_USE_EXPAND_INVENTORY_OK, g_pMain->GetNowState() );
					}
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), NetError::GetErrStrF( NetError::ERR_BUY_CASH_ITEM_19 ), g_pMain->GetNowState() );
				} break;
			case INVENTORY_SLOT_ADD_ITEM_QUEST_EVENT:
				{

					// 지헌 : 사용 전 인벤 확장이 가능 한가 테스트를 하자.
					if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum(CX2Inventory::ST_QUEST) < INVENTORY_SLOT_MAX_NUM)
					{
						// 지헌 : 번역 - 스트링 넣어야 한다.
						m_UsedItemUID = pItemSlot->GetItemUID();
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_13554 ), UIM_USE_EXPAND_INVENTORY_OK, g_pMain->GetNowState() );
					}
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), NetError::GetErrStrF( NetError::ERR_BUY_CASH_ITEM_19 ), g_pMain->GetNowState() );								
				} break;
			case INVENTORY_SLOT_ADD_ITEM_SPECIAL_EVENT:
				{

					// 지헌 : 사용 전 인벤 확장이 가능 한가 테스트를 하자.
										if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum(CX2Inventory::ST_SPECIAL) < INVENTORY_SLOT_MAX_NUM)
					{
						// 지헌 : 번역 - 스트링 넣어야 한다.
						m_UsedItemUID = pItemSlot->GetItemUID();
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_13554 ), UIM_USE_EXPAND_INVENTORY_OK, g_pMain->GetNowState() );
					}
					else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), NetError::GetErrStrF( NetError::ERR_BUY_CASH_ITEM_19 ), g_pMain->GetNowState() );
				} break;
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM

#ifdef SERV_RELATIONSHIP_EVENT_INT
			case EVENT_PROPOSE_ITEM:
				{
					switch( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_VILLAGE_MAP:
						{
							m_UsedItemUID = pItemSlot->GetItemUID();
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_3712, "L", pItem->GetFullName()) ), UIM_USE_PROPOSE_ITEM, g_pMain->GetNowState() );
						}
						break;
					default:
						{
							g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24660 ),g_pMain->GetNowState() );
							return true;
						}
						break;
					}
				}
				break;
			case EVENT_DIVORCE_ITEM:
				{
					switch( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_VILLAGE_MAP:
						{
							m_UsedItemUID = pItemSlot->GetItemUID();
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_3712, "L", pItem->GetFullName()) ), UIM_USE_DIVORCE_ITEM, g_pMain->GetNowState() );
						}
						break;
					default:
						{
							g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24660 ),g_pMain->GetNowState() );
							return true;
						}
						break;
					}
				}
				break;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
			case RECOMMEND_TICKET_ID:
				{					
					Handler_EGS_USE_RECRUIT_TICKET_REQ( pItemSlot->GetItemUID() );
				} break;
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_SKILL_NOTE
			case SKILL_NOTE_ITEM_ID:
#endif
			default:
				{
					Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( pItemSlot->GetItemUID() );
				} break;
			}
		}
		return true;
	}





	//{{ 2007. 10. 8  최육사  인벤에서 랜덤템을 오른쪽 클릭했을 경우 [임시 테스트]
	const CX2Item::ItemData& kItemData = pItem->GetItemData();
	{
#ifdef SERV_OPEN_RANDOM_ITEM_LEVEL_CHECK 
        // 랜덤아이템의 사용레벨제한 보다 유저레벨이 낮다면.
		if (g_pData->GetSelectUnitLevel() < g_pData->GetItemManager()->GetItemTemplet( kItemData.m_ItemID )->GetUseLevel() && g_pData->GetItemManager()->GetItemTemplet( kItemData.m_ItemID )->GetItemType() == CX2Item::IT_SPECIAL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ),  GET_STRING(STR_ID_5014) , g_pMain->GetNowState() );
            return true;
		}
#endif SERV_OPEN_RANDOM_ITEM_LEVEL_CHECK 

#ifdef SERV_CUBE_OPEN_ED_CONDITION
		bool	bOpenRequiredED = false;
		int		iRequiredED = 0;
#endif //SERV_CUBE_OPEN_ED_CONDITION		
		
		//{{ kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가
#ifdef	MULTI_KEY_RANDOM_ITEM
		std::pair< CX2ItemManager::RandomItemItr, CX2ItemManager::RandomItemItr > pairRandomItem = g_pData->GetItemManager()->GetRandomItemData( kItemData.m_ItemID );

		if ( g_pData->GetItemManager()->IsRandomItem( pairRandomItem ) == true
#ifdef FIX_RANDOM_CUBE_OPEN_RESULT_BUG
			//큐브 오픈 동작중이라면 동작하지 않기
			&& false == m_bProcessCubeOpen
#endif //FIX_RANDOM_CUBE_OPEN_RESULT_BUG
			)
		{
			CX2ItemManager::RandomItemItr startItr	= pairRandomItem.first;
			CX2ItemManager::RandomItemItr endItr	= pairRandomItem.second;

			std::multimap< int, CX2ItemManager::RandomItemData* >::difference_type dfCount = 0;

			CX2ItemManager::RandomItemItr tempStartItr = startItr;

			while ( tempStartItr != endItr )
			{
				dfCount++;
				tempStartItr++;
			}

			if ( dfCount == 1 )		// 키가 한개면
			{
				//키 아이템, ED로 열 수 있도록 변경
#ifdef SERV_CUBE_OPEN_ED_CONDITION		
				CX2ItemManager::RandomItemData* pRandomItem = startItr->second;
				bool bEnoughRequiredItem = true;  // 개봉 요구 아이템이 충분한지 여부				

				wstring wstrMsg = L"";//키, 또는 ED가 충분하지 않을 때 출력하는 에러 팝업 메세지
				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kItemData.m_ItemID );
				if( NULL != pItemTemplet )	
					wstrMsg = GET_REPLACED_STRING( ( STR_ID_21083, "S", pItemTemplet->GetFullName_() )); 
								
				//개봉 조건 키 아이템 체크
				if( 0 < pRandomItem->m_RequiredKeyCount ) 
				{					
					int keyItemNum = pUnit->GetInventory().GetNumItemByTID( pRandomItem->m_KeyItemID );					
					if ( keyItemNum < pRandomItem->m_RequiredKeyCount )
					{
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
						if( ONE_THIRD_MINI_ATTRACTION_ITEM == kItemData.m_ItemID)
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24516 ), g_pMain->GetNowState() );
							return false;
						}
						else
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
						{
							const CX2Item::ItemTemplet* pItemTemplet =
								g_pData->GetItemManager()->GetItemTemplet( pRandomItem->m_KeyItemID );
							if( NULL != pItemTemplet )
							{
								const wchar_t* pwszitemName = pItemTemplet->GetFullName_();
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
								wstrMsg += GET_REPLACED_STRING( ( STR_ID_808, "SSi", pwszitemName, L"", pRandomItem->m_RequiredKeyCount ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
								wstrMsg += GET_REPLACED_STRING( ( STR_ID_808, "SIi", pwszitemName, KHanSungPostWordUnicodeWrapper( ( WCHAR* )pwszitemName, STR_ID_806, STR_ID_807 ), pRandomItem->m_RequiredKeyCount ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
								bEnoughRequiredItem = false;
							}
						}
					}
				}
				
				//개봉 조건 ED 체크
				iRequiredED = pRandomItem->m_iRequiredED;	//개봉에 필요한 ED
				int iMyED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED; //보유 ED
				if( 0 < iRequiredED )					
				{
					bOpenRequiredED = true;
					if( iMyED < iRequiredED )
					{
						wstrMsg += GET_REPLACED_STRING( ( STR_ID_21081, "L", g_pMain->GetEDString(iRequiredED) ));
						bEnoughRequiredItem = false;
					}
				}

				//키또는 ED가 충분하지 않다면 에러 팝업 출력하고 return
				if( false == bEnoughRequiredItem )
				{						
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), g_pMain->GetNowState() );
					return false;
				}
#else
				CX2ItemManager::RandomItemData* pRandomItem = startItr->second;
				int keyItemNum = pUnit->GetInventory().GetNumItemByTID( pRandomItem->m_KeyItemID );
				if ( keyItemNum < pRandomItem->m_RequiredKeyCount )
				{
					const CX2Item::ItemTemplet* pItemTemplet =
					    g_pData->GetItemManager()->GetItemTemplet( pRandomItem->m_KeyItemID );
					const wchar_t* pwszItemName = pItemTemplet->GetFullName_();
					wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_808, "SIi", pwszItemName, KHanSungPostWordUnicodeWrapper( ( WCHAR* )pwszItemName, STR_ID_806, STR_ID_807 ), pRandomItem->m_RequiredKeyCount ) );
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), g_pMain->GetNowState() );
					return false;
				}
#endif //SERV_CUBE_OPEN_ED_CONDITION
			}
			else if ( dfCount > 1 )							// 두개 이상이면
			{
				bool bHaveKey = false;
#ifdef MODIFY_SERV_CUBE_OPEN_ED_CONDITION
// 키 아이템이 2개 이상일 경우에 대해 부족한 아이템에 대한 안내문구 추가
				map<int, int> mapNotEnoughItemIDAndCount; // <int, int> : 아이템ID, 부족한 아이템 갯수.
#endif //MODIFY_SERV_CUBE_OPEN_ED_CONDITION
				while ( startItr != endItr )
				{
					CX2ItemManager::RandomItemData* pRandomItem = startItr->second;
					int keyItemNum = pUnit->GetInventory().GetNumItemByTID( pRandomItem->m_KeyItemID );
							
					if ( keyItemNum >= pRandomItem->m_RequiredKeyCount )
					{
						bHaveKey = true;
						break;
					}
#ifdef MODIFY_SERV_CUBE_OPEN_ED_CONDITION
					else
					{//큐브를 열기 위한 키 아이템이 부족 할 경우 체크						
					/// insert ( 아이템ID, 필요한 아이템 갯수 - 보유한 아이템 갯수 )
						mapNotEnoughItemIDAndCount.insert( std::make_pair(pRandomItem->m_KeyItemID, (pRandomItem->m_RequiredKeyCount - keyItemNum )) );
					}
#endif //MODIFY_SERV_CUBE_OPEN_ED_CONDITION
					++startItr;
				}

				if ( bHaveKey == false )
				{
#ifdef MODIFY_SERV_CUBE_OPEN_ED_CONDITION					
					if( mapNotEnoughItemIDAndCount.size() >= 1 )
					{
						wstring wstrMsg = L"";
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kItemData.m_ItemID );
						if( pItemTemplet != NULL ) // 개봉하려고 하는 큐브 아이템
						{
							wstrMsg += GET_REPLACED_STRING( ( STR_ID_21083, "S", pItemTemplet->GetFullName_() )); 
						}


						wstrMsg += GET_STRING(STR_ID_23473); //개봉하려면	
						bool bIsFirst = true;
						map<int, int>::iterator mit = mapNotEnoughItemIDAndCount.begin();
						for( ; mit != mapNotEnoughItemIDAndCount.end(); ++mit)
						{
							//개봉을 하기 위해 필요한 키 아이템
							pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( mit->first );
							if( NULL != pItemTemplet )
							{
								const wchar_t* itemName = pItemTemplet->GetFullName_();
								if( true == bIsFirst )
									wstrMsg += GET_REPLACED_STRING( ( STR_ID_23475, "SIi", itemName,		/// [@1]@2 @3개
																	KHanSungPostWordUnicodeWrapper( itemName, STR_ID_806, STR_ID_807 ), 
																	mit->second ) );
								else
									wstrMsg += GET_REPLACED_STRING( ( STR_ID_23476, "SIi", itemName,		/// 또는 [@1]@2 @3개
									KHanSungPostWordUnicodeWrapper( itemName, STR_ID_806, STR_ID_807 ), 
									mit->second ) );
							}
							bIsFirst = false;
						}
						wstrMsg += GET_STRING(STR_ID_23474); //위 아이템이 필요합니다.
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_OKMsgBoxPlus.lua" );
					}
#else
	#ifdef GUIDE_ATTRACTION_ITEM_USE_WITHOUT_ICE_HEATER
					if( 108900 == kItemData.m_ItemID )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_17700 ), g_pMain->GetNowState() );						
	
					}
					else
	#endif //GUIDE_ATTRACTION_ITEM_USE_WITHOUT_ICE_HEATER
					{
#ifdef CUBE_OPEN_IMAGE_MANAGER
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), g_pData->GetCubeOpenImageManager()->GetEventCubeNoKeyString( kItemData.m_ItemID ), g_pMain->GetNowState() );
#else
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4834 ), g_pMain->GetNowState() );
#endif CUBE_OPEN_IMAGE_MANAGER
					}
#endif //MODIFY_SERV_CUBE_OPEN_ED_CONDITION	
					return false;
				}

			}
			else				// 0 이면
				return false;
			
			// 랜덤 아이템이다~
			//{{ 2009.01.20 김태완
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
#else // AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			if( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
#endif // AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			{
				if ( m_pDLGUIInventory != NULL )
				{
					CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGUIInventory->GetControl( L"CheckBox_Auth" );
					if ( pCheckBox != NULL )
					{
						if ( pCheckBox->GetChecked() == true )
						{
							m_bUsingRandomItemAutoOpen = true;
						}
					}
				}
			}
#endif
			//}}

			m_RandomItemID = kItemData.m_ItemID;
			m_iRandomItemUID = pItemSlot->GetItemUID();

#ifdef CUBE_OPEN_IMAGE_MANAGER
			g_pData->GetCubeOpenImageManager()->SetPlaying( false );
			g_pData->GetCubeOpenImageManager()->SetStart( false );
			g_pData->GetCubeOpenImageManager()->SetSoundPlaying( false );
			m_bHideCubeOpneBar = false;
#endif // CUBE_OPEN_IMAGE_MANAGER
#ifdef ATTRACTION_ITEM_TEST

			bool bAttractionItemCheck = false;
			for(int i=0; i<ARRAY_SIZE(ATTRACTION_ITEM_ID); ++i)
			{
				if( kItemData.m_ItemID == ATTRACTION_ITEM_ID[i] )
				{
					bAttractionItemCheck = true;

#ifdef FIX_ICE_HEATER_EVENT			/// 가열기 종류에 따른 타입 설정
					m_eRandomItemEventType = RIOET_ICE_HEATER;
#endif // FIX_ICE_HEATER_EVENT

					break;
				}
			}

			bool bMiniAttractionItemCheck = false;		/// 미니 가열기 아이템인가?
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD		/// 미니 가열기 조각상인지 검사
			for(int i=0; i<ARRAY_SIZE(MINI_ATTRACTION_ITEM_ID); ++i)
			{
				if( kItemData.m_ItemID == MINI_ATTRACTION_ITEM_ID[i] )
				{
					bAttractionItemCheck = true;
					bMiniAttractionItemCheck = true;

#ifdef FIX_ICE_HEATER_EVENT			/// 가열기 타입에 따른 타입 설정
					m_eRandomItemEventType = RIOET_MINI_ICE_HEATER;
#endif // FIX_ICE_HEATER_EVENT

					break;
				}
			}
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD

#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD		//올인원 가열기 검사
			m_bIsAllInOne = false;
			if( kItemData.m_ItemID == ALL_IN_ONE_ATTRACTION_ITEM_ID )
			{
				bAttractionItemCheck = true;
				m_bIsAllInOne = true;

#ifdef FIX_ICE_HEATER_EVENT			/// 가열기 종류에 따른 타입 설정
				m_eRandomItemEventType = RIOET_ALL_IN_ONE_ICE_HEATER;
#endif // FIX_ICE_HEATER_EVENT

			}
#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

#ifdef RIDING_SYSTEM
			m_bIsRidingGacha = false;
			if( kItemData.m_ItemID == ANCIENT_FOSSIL_RIDING_PET )
			{
				bAttractionItemCheck = true;
				m_bIsRidingGacha = true;

#ifdef FIX_ICE_HEATER_EVENT			/// 가열기 종류에 따른 타입 설정
				m_eRandomItemEventType = RIOET_RIDING_PET_GACHA;
#endif // FIX_ICE_HEATER_EVENT

			}
#endif //RIDING_SYSTEM

#ifdef SERV_HALLOWEEN_ICE_HEATER
			m_bIsHalloweenIceHeater = false;
			for(int i=0; i<ARRAY_SIZE(HALLOWEEN_ATTRACTION_ITEM_ID); ++i)
			{
				if( kItemData.m_ItemID == HALLOWEEN_ATTRACTION_ITEM_ID[i] )
				{
					bAttractionItemCheck = true;
					m_bIsHalloweenIceHeater = true;

#ifdef FIX_ICE_HEATER_EVENT			/// 가열기 종류에 따른 타입 설정
					m_eRandomItemEventType = RIOET_HALLOWEEN_ICE_HEATER;
#endif // FIX_ICE_HEATER_EVENT

				}
			}
#endif //SERV_HALLOWEEN_ICE_HEATER

			bool bGoldAttractionItemCheck = false;
			for(int i=0; i<ARRAY_SIZE(GOLD_ATTRACTION_ITEM_ID); ++i)
			{
				if( kItemData.m_ItemID == GOLD_ATTRACTION_ITEM_ID[i] )
				{
					bGoldAttractionItemCheck = true;

#ifdef FIX_ICE_HEATER_EVENT			/// 가열기 종류에 따른 타입 설정
					m_eRandomItemEventType = RIOET_GOLD_ICE_HEATER;
#endif // FIX_ICE_HEATER_EVENT

					break;
				}
			}

			if( bAttractionItemCheck == true || bGoldAttractionItemCheck == true )
			{
				//{{ kimhc // 실시간 엘소드 중 던전내에서 아이템 사용시 막아야 할 것들
#ifdef	REAL_TIME_ELSWORD
				if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
				{
					g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2646 ),g_pMain->GetNowState() );

					return true;
				}
#endif	REAL_TIME_ELSWORD
				//}} kimhc // 실시간 엘소드 중 던전내에서 아이템 사용시 막아야 할 것들

#ifdef FIX_ICE_HEATER_EVENT

				/// 타입에 따른 가열기 재생 정보 설정
				m_pNowIceHeaterEventData = NULL;

				std::map< RANDOM_ITEM_OPEN_EVENT_TYPE, IceHeaterEventData* >::const_iterator mit;

				mit = m_mapIceHeaterEventData.find( m_eRandomItemEventType );

				if( mit != m_mapIceHeaterEventData.end() )
					m_pNowIceHeaterEventData = mit->second;		/// 현재 열려있는 가열기의 재생 정보 저장
#endif // FIX_ICE_HEATER_EVENT

				m_pDLGOpenAttraction = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Ice_Heater_Use.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOpenAttraction );

#ifdef FIX_ICE_HEATER_EVENT

				/// 가열기 오픈 다이얼로그 설정
				if( NULL != m_pDLGOpenAttraction )
				{
					CKTDGUIStatic* pStaticHeaterTitle		= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticHeater_Title" );
					CKTDGUIStatic* pStaticHelperString		= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"helper" );
					CKTDGUIStatic* pStaticFeel				= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticFeel" );

					if( NULL != pStaticHeaterTitle && NULL != pStaticHelperString && NULL != pStaticFeel && NULL != m_pNowIceHeaterEventData )
					{
						wstring wstrMainDialogTitleTexture		= m_pNowIceHeaterEventData->m_wstrMainDialogTitleTexture;
						wstring wstrMainDialogTitleTextureKey	= m_pNowIceHeaterEventData->m_wstrMainDialogTitleTextureKey;
						D3DXVECTOR2 vecIconOffSetPosition		( m_pNowIceHeaterEventData->m_fCautionIconOffSetPositionX, 
																  m_pNowIceHeaterEventData->m_fCautionIconOffSetPositionY );
						D3DXVECTOR2 vecStringOffSetPosition		( m_pNowIceHeaterEventData->m_fCautionStringOffSetPositionX, 
																  m_pNowIceHeaterEventData->m_fCautionStringOffSetPositionY );
						int iCautionStringID					= m_pNowIceHeaterEventData->m_iCautionStringNum;

						if( NULL != pStaticHeaterTitle->GetPicture(0) )							/// 타이틀명 텍스처 설정
						{
							pStaticHeaterTitle->GetPicture(0)->SetTex( wstrMainDialogTitleTexture.c_str(), wstrMainDialogTitleTextureKey.c_str() );
							pStaticHeaterTitle->GetPicture(0)->SetSizeAsTextureSize();			/// 텍스쳐 변경되면 변경된 텍스쳐 크기로 맞춤 해외팀 코드 추가
						}
						
						pStaticHeaterTitle->SetShow( true );		
						pStaticHelperString->SetString( 0, GET_STRING( iCautionStringID ) );	/// 안내 스트링 변경
						pStaticHelperString->SetOffsetPos(vecStringOffSetPosition );			/// 안내 스트링 위치 이동
						pStaticFeel->SetOffsetPos( vecIconOffSetPosition );						/// 스트링 앞 느낌표 아이콘 위치 이동
					}
				}

#else FIX_ICE_HEATER_EVENT

	#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
				if( true == bMiniAttractionItemCheck )		/// 미니 얼음 조각상 가열기용 다이얼로그 수정
				{
					if( NULL != m_pDLGOpenAttraction )
					{
						CKTDGUIStatic* pStaticMiniHeaterTitle	= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticMiniHeater_Title" );
						CKTDGUIStatic* pStaticHeaterTitle		= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticHeater_Title" );
						CKTDGUIStatic* pStaticHelperString		= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"helper" );
						CKTDGUIStatic* pStaticFeel				= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticFeel" );

						if( NULL != pStaticMiniHeaterTitle && NULL != pStaticHeaterTitle && NULL != pStaticHelperString && NULL != pStaticFeel )
						{
							pStaticMiniHeaterTitle->SetShow( true );	/// "미니 얼음 조각상 가열기 사용" 문구 표시
							pStaticHeaterTitle->SetShow( false );		/// "얼음 조각상 가열기 사용" 문구 숨김
							pStaticHelperString->SetString( 0, GET_STRING( STR_ID_22053 ) );	/// 안내 스트링 변경
							pStaticHelperString->SetOffsetPos( D3DXVECTOR2( -20.f, 0.f ) );		/// 안내 스트링 위치 이동
							pStaticFeel->SetOffsetPos( D3DXVECTOR2( -20.f, 0.f ) );				/// 스트링 앞 느낌표 아이콘 위치 이동
						}

					}
				}
	#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				else if( true == m_bIsAllInOne )
				{
					if( NULL != m_pDLGOpenAttraction )
					{
						CKTDGUIStatic* pStaticHeaterTitle		= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticHeater_Title" );
						CKTDGUIStatic* pStaticHelperString		= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"helper" );
						CKTDGUIStatic* pStaticFeel				= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticFeel" );

						if( NULL != pStaticHeaterTitle && NULL != pStaticHelperString && NULL != pStaticFeel )
						{
							if( NULL != pStaticHeaterTitle->GetPicture(0) )
								pStaticHeaterTitle->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_MarriageSystem_02.tga", L"Bg_Title_AllInOne" );
							pStaticHeaterTitle->SetShow( true );		
							pStaticHelperString->SetString( 0, GET_STRING( STR_ID_24383 ) );	/// 안내 스트링 변경
							pStaticHelperString->SetOffsetPos( D3DXVECTOR2( -23.f, 0.f ) );		/// 안내 스트링 위치 이동
							pStaticFeel->SetOffsetPos( D3DXVECTOR2( -23.f, 0.f ) );				/// 스트링 앞 느낌표 아이콘 위치 이동
						}
					}
				}
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	#ifdef RIDING_SYSTEM
				else if( true == m_bIsRidingGacha )
				{
					if( NULL != m_pDLGOpenAttraction )
					{
						CKTDGUIStatic* pStaticHeaterTitle		= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticHeater_Title" );
						CKTDGUIStatic* pStaticHelperString		= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"helper" );
						CKTDGUIStatic* pStaticFeel				= (CKTDGUIStatic*) m_pDLGOpenAttraction->GetControl( L"g_pStaticFeel" );

						if( NULL != pStaticHeaterTitle && NULL != pStaticHelperString && NULL != pStaticFeel )
						{
							if( NULL != pStaticHeaterTitle->GetPicture(0) )
								pStaticHeaterTitle->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_RidingPet_01.tga", L"riding_heater_title" );
							pStaticHeaterTitle->SetShow( true );		
							pStaticHelperString->SetString( 0, GET_STRING( STR_ID_24518 ) );	/// 안내 스트링 변경
							pStaticHelperString->SetOffsetPos( D3DXVECTOR2( -17.f, 0.f ) );		/// 안내 스트링 위치 이동
							pStaticFeel->SetOffsetPos( D3DXVECTOR2( -17.f, 0.f ) );				/// 스트링 앞 느낌표 아이콘 위치 이동
						}
					}
				}
	#endif //RIDING_SYSTEM

#endif // FIX_ICE_HEATER_EVENT

				// 중간에 모델 그려주자
				D3DXVECTOR3 boxPos = m_pDLGOpenAttraction->GetDummyPos( 0 );
				CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachine = NULL;

#ifdef FIX_ICE_HEATER_EVENT

				/// 가열기 오픈 전 대기 모션 설정
				const wstring wstrWaitMeshPlayerName = 
					NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrWaitMeshPlayerName : L"FireMachineWait" ;

				pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  wstrWaitMeshPlayerName.c_str(), boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );

#else // FIX_ICE_HEATER_EVENT

	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				if( m_bIsAllInOne == true )
					pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"AllInOneFireMachineWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
				else
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	#ifdef RIDING_SYSTEM
				if( m_bIsRidingGacha == true )
					pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"AncinetFossilRidingPetWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
				else
	#endif //RIDING_SYSTEM
				if( bGoldAttractionItemCheck == true )
					pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"GoldFireMachineWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
				else
							
					pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"FireMachineWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );

#endif // FIX_ICE_HEATER_EVENT
				
				g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst_MeltMachine);

				if( NULL != m_pDLGOpenAttraction )
				{
					m_pDLGOpenAttraction->SetHasUnit( pMeshInst_MeltMachine );
				}

				m_hMeshInstMeltMachine = pMeshInst_MeltMachine->GetHandle();

#ifndef FIX_ICE_HEATER_EVENT
				/// Enum값 정의 구문을 위로 이동

				if( bGoldAttractionItemCheck == true )
					m_eRandomItemEventType = RIOET_GOLD_ICE_HEATER;
		#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
				else if( bMiniAttractionItemCheck == true )
					m_eRandomItemEventType = RIOET_MINI_ICE_HEATER;		/// 미니 얼음 조각상 가열기
		#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
		#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				else if( m_bIsAllInOne == true )
					m_eRandomItemEventType = RIOET_ALL_IN_ONE_ICE_HEATER;
		#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
		#ifdef RIDING_SYSTEM
				else if( m_bIsRidingGacha == true )
					m_eRandomItemEventType = RIOET_RIDING_PET_GACHA;
		#endif //RIDING_SYSTEM
				else
					m_eRandomItemEventType = RIOET_ICE_HEATER;
#endif // FIX_ICE_HEATER_EVENT

				if(g_pChatBox != NULL)
					g_pChatBox->HideChatEditBox();

#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
					m_iUseOpenRandomItemID = kItemData.m_ItemID;
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
			}
			else
#endif ATTRACTION_ITEM_TEST
			{
#ifdef APRIL_5TH_TREE_DAY_EVENT
				if( IsAprilEventRandomItem(kItemData.m_ItemID) )
				{
					m_eRandomItemEventType = RIOET_APRIL_5TH_TREE_DAY_EVENT;
					m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
				} 
				else
#endif APRIL_5TH_TREE_DAY_EVENT
#ifdef 	SERV_TREASURE_BOX_ITEM
					if( IsTreasureBoxRandomItem(kItemData.m_ItemID) )
					{
						m_eRandomItemEventType = RIOET_TREASURE_BOX_ITEM;
#ifdef TREASURE_BOX_ITEM_THIN
						m_TreasureBoxSPItemID = kItemData.m_ItemID;
#endif TREASURE_BOX_ITEM_THIN
						m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
					} 
					else
#endif 	SERV_TREASURE_BOX_ITEM

#ifdef CUBE_OPEN_IMAGE_MANAGER
					if( g_pData->GetCubeOpenImageManager()->IsEventCube(kItemData.m_ItemID) == true)
					{
						m_eRandomItemEventType = RIOET_EVENT_CUBE;
						m_bHideCubeOpneBar = g_pData->GetCubeOpenImageManager()->IsHideBarCube(kItemData.m_ItemID);
						m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), g_pData->GetCubeOpenImageManager()->GetEventCubeOpenString(kItemData.m_ItemID), UIM_OPEN_RANDOM_ITEM_NEW, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
					}
					else
#endif CUBE_OPEN_IMAGE_MANAGER
					if ( IsMonsterCardSetItem( kItemData.m_ItemID ) == true )
					{
						m_eRandomItemEventType = RIOET_MONSTER_CARD_SET;
						m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
					}
					else					
					{
						m_eRandomItemEventType = RIOET_NONE;
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
						m_iUseOpenRandomItemID = kItemData.m_ItemID;
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

#ifdef SERV_CUBE_OPEN_ED_CONDITION
						bool bNoCubeOpenItemCheck = false;
						for(int i=0; i<ARRAY_SIZE(NO_CUBE_OPEN_ITEM_ID); ++i)
						{
							if( kItemData.m_ItemID == NO_CUBE_OPEN_ITEM_ID[i] )
							{
								bNoCubeOpenItemCheck = true;
								break;
							}
						}

						UI_INVENTORY_MSG eOpenMSG = UIM_OPEN_RANDOM_ITEM_NEW;
						if( true == bNoCubeOpenItemCheck)
							eOpenMSG = UIM_OPEN_RANDOM_ITEM;
						//ED오픈 아이템일 경우 사용 전 소모 ED알림 추가
						if( true == bOpenRequiredED )
						{
							const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kItemData.m_ItemID );
							if( NULL != pItemTemplet )			
							{
								m_pDLGRandomItem = 	g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
														GET_REPLACED_STRING( ( STR_ID_21082, "SL", pItemTemplet->GetFullName_(), g_pMain->GetEDString(iRequiredED) )) ,
														eOpenMSG, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
							}			
						}
						else
						{
							m_pDLGRandomItem = 	g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), eOpenMSG, 
																			g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
						}
					}
#else
#ifdef RANDOM_CUBE_UI_NEW

#ifdef SPECIAL_USE_ITEM
						bool bNoCubeOpenItemCheck = false;
						for(int i=0; i<ARRAY_SIZE(NO_CUBE_OPEN_ITEM_ID); ++i)
						{
							if( kItemData.m_ItemID == NO_CUBE_OPEN_ITEM_ID[i] )
							{
								bNoCubeOpenItemCheck = true;
								break;
							}
						}
						if( bNoCubeOpenItemCheck == true )
#else
						if(kItemData.m_ItemID == SPIRIT_POTION_ITEM_ID)
#endif SPECIAL_USE_ITEM
							m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
						else
							m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM_NEW, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
#else
						m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
#endif RANDOM_CUBE_UI_NEW
					}
#endif //SERV_CUBE_OPEN_ED_CONDITION	
			}
		}
#else	MULTI_KEY_RANDOM_ITEM
		CX2ItemManager::RandomItemData* pRandomItem = g_pData->GetItemManager()->GetRandomItemData( kItemData.m_ItemID );
		if( pRandomItem != NULL )
		{		
			// 랜덤 아이템이다~
			//{{ 2009.01.20 김태완
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
#else // AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			if( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
#endif // AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			{
				if ( m_pDLGUIInventory != NULL )
				{
					CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGUIInventory->GetControl( L"CheckBox_Auth" );
					if ( pCheckBox != NULL )
					{
						if ( pCheckBox->GetChecked() == true )
						{
							m_bUsingRandomItemAutoOpen = true;
						}
					}
				}
			}
#endif
			//}}


			m_RandomItemID = kItemData.m_ItemID;
			m_iRandomItemUID = pItemSlot->GetItemUID();

			int keyItemNum = pUnit->GetInventory().GetNumItemByTID( pRandomItem->m_KeyItemID );

			if ( keyItemNum < pRandomItem->m_RequiredKeyCount )
			{
				const CX2Item::ItemTemplet* pItemTemplet;
				    = g_pData->GetItemManager()->GetItemTemplet( pRandomItem->m_KeyItemID );
				const wchar_t* pwszitemName = pItemTemplet->GetFullName_();
				wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_808, "SIi", pwszitemName, KHanSungPostWordUnicodeWrapper( ( WCHAR* )pwszitemName, STR_ID_806, STR_ID_807 ), pRandomItem->m_RequiredKeyCount ) );
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrMsg.c_str(), g_pMain->GetNowState() );
				return false;
			}

#ifdef ATTRACTION_ITEM_TEST

			bool bAttractionItemCheck = false;
			for(int i=0; i<ARRAY_SIZE(ATTRACTION_ITEM_ID); ++i)
			{
				if( kItemData.m_ItemID == ATTRACTION_ITEM_ID[i] )
				{
					bAttractionItemCheck = true;
					break;
				}
			}

			bool bGoldAttractionItemCheck = false;
			for(int i=0; i<ARRAY_SIZE(GOLD_ATTRACTION_ITEM_ID); ++i)
			{
				if( kItemData.m_ItemID == GOLD_ATTRACTION_ITEM_ID[i] )
				{
					bGoldAttractionItemCheck = true;
					break;
				}
			}

			if( bAttractionItemCheck == true || bGoldAttractionItemCheck == true )
			{
				//{{ kimhc // 실시간 엘소드 중 던전내에서 아이템 사용시 막아야 할 것들
#ifdef	REAL_TIME_ELSWORD
				if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
				{
					g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2646 ),g_pMain->GetNowState() );

					return true;
				}
#endif	REAL_TIME_ELSWORD
				//}} kimhc // 실시간 엘소드 중 던전내에서 아이템 사용시 막아야 할 것들

				m_pDLGOpenAttraction = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Ice_Heater_Use.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOpenAttraction );

				// 중간에 모델 그려주자
				D3DXVECTOR3 boxPos = m_pDLGOpenAttraction->GetDummyPos( 0 );
				if( bGoldAttractionItemCheck == true )
					pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"GoldFireMachineWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
				else
					pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"FireMachineWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );

				g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst_MeltMachine);

				if( NULL != m_pDLGOpenAttraction )
				{
					m_pDLGOpenAttraction->SetHasUnit( pMeshInst_MeltMachine );
				}

				m_hMeshInstMeltMachine = pMeshInst_MeltMachine->GetHandle();

				if( bGoldAttractionItemCheck == true )
					m_eRandomItemEventType = RIOET_GOLD_ICE_HEATER;
				else
					m_eRandomItemEventType = RIOET_ICE_HEATER;

				if(g_pChatBox != NULL)
					g_pChatBox->HideChatEditBox();
			}
			else
#endif ATTRACTION_ITEM_TEST
			{
#ifdef APRIL_5TH_TREE_DAY_EVENT
				if( IsAprilEventRandomItem(kItemData.m_ItemID) )
				{
					m_eRandomItemEventType = RIOET_APRIL_5TH_TREE_DAY_EVENT;
					m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
				} 
				else
#endif APRIL_5TH_TREE_DAY_EVENT
					if ( IsMonsterCardSetItem( kItemData.m_ItemID ) == true )
					{
						m_eRandomItemEventType = RIOET_MONSTER_CARD_SET;
						m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
					}
					else					
					{
						m_eRandomItemEventType = RIOET_NONE;
						m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_809 ), UIM_OPEN_RANDOM_ITEM, g_pMain->GetNowState(), UIM_OPEN_RANDOM_ITEM_CANCLE );
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
						if ( pItemData != NULL )
						{
							m_iUseOpenRandomItemID = pItemData->m_ItemID;
						}
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
					}
			}
		}
#endif	MULTI_KEY_RANDOM_ITEM
		//}} kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가
		else // 랜덤 큐브가 아니면
		{
#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장
			if( true == IsAddSocketSlotItem( kItemData.m_ItemID ) )
			{
				UseAddSocketSlotItem( pItemSlot->GetItemUID() );
			}
#endif // ADD_SOCKET_SLOT
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 미라클 큐브( 미감정 상태로 되돌리는 아이템 )
			if( true == IsRestoreEvaluateItem( kItemData.m_ItemID ) )
			{
				UseRestoreEvaluteItem( pItemSlot->GetItemUID() );
			}
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef ATTRACTION_ITEM_TEST
			if( IsAttachItem( kItemData.m_ItemID ) )
			{
				// 강화권 쓰는 곳
				CX2State* pState = (CX2State*)g_pMain->GetNowState();
				if(pState->GetCursor() != NULL)
					pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_ATTACH_ITEM );
				m_CouponUid = pItemSlot->GetItemUID();

				// 탭 바꿔주자..
				ChangeInventoryTab( CX2Inventory::ST_EQUIP );
				return true;
			}
#endif ATTRACTION_ITEM_TEST

#ifdef SERV_ATTRIBUTE_CHARM
			if( IsAttributeItem( kItemData.m_ItemID ) )
			{
				CX2State* pState = (CX2State*)g_pMain->GetNowState();
				if(pState->GetCursor() != NULL)
					pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_ATTACH_ATTRIB_ITEM );
				m_CouponUid = pItemSlot->GetItemUID();

				ChangeInventoryTab( CX2Inventory::ST_EQUIP );
				return true;
			}
#endif SERV_ATTRIBUTE_CHARM


			switch( kItemData.m_ItemID )
			{
			case CX2EnchantItem::ATI_RED: 
			case CX2EnchantItem::ATI_BLUE: 
			case CX2EnchantItem::ATI_GREEN:
			case CX2EnchantItem::ATI_WIND: 
			case CX2EnchantItem::ATI_LIGHT:
			case CX2EnchantItem::ATI_DARK: 
				{
#ifdef FINALITY_SKILL_SYSTEM //JHKang
					OpenElBreakWindow( pItem );
#else //FINALITY_SKILL_SYSTEM
					//엘 속성 환원기가 있는지 확인
					if ( pUnit->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE ) > 0 )
					{
						OpenElChangerWindow( false, pItem );
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_810 ), g_pMain->GetNowState() );
					}
#endif //FINALITY_SKILL_SYSTEM

					return true;
				} break;

#ifdef TEMP_NO_EL_IDENTIFY
#else TEMP_NO_EL_IDENTIFY
			case CX2EnchantItem::ATI_UNKNOWN:
				{
					//엘 속성 판별기가 있는지 확인
					if ( pUnit->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE ) > 0 )
					{
						OpenElChangerWindow( true, pItem );
						
						//m_ElChangerItemUID	= pItem->GetItemData().m_ItemUID;
						//m_ElChangerItemCount = 1;
						//if( m_pDLGIdentifyStone == NULL )
						//{
						//	m_pDLGIdentifyStone = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5017), UIM_UNKNOWN_TO_IDENTIFY, g_pMain->GetNowState() );
						//}						
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_811 ), g_pMain->GetNowState() );
					}

					return true;
				} break;
#endif TEMP_NO_EL_IDENTIFY




				//case 203800: // 전체 스킬 초기화 아이템
				//	{
				//		if( NULL != m_pSkillTreeUI )
				//		{
				//			m_pSkillTreeUI->OpenResetAllSkillPopup( true );
				//		}
				//	} break;
			}
		}
	}
	//}}

	//{{ kimhc // 2009-08-19 // 봉인시키는 아이템을 오른쪽 클릭 시
#ifdef	SEAL_ITEM
    switch ( pItem->GetItemTemplet()->GetItemID() )
	{
	case ITEM_FOR_SEAL_NORMAL:								// 고양이 발 도장(하얀색)
	case ITEM_FOR_SEAL_RARE:								// 고양이 발 도장(노란색)
	case ITEM_FOR_SEAL_ELITE:								// 고양이 발 도장(보라색)
	case ITEM_FOR_SEAL_UNIQUE:								// 고양이 발 도장(황금색)
	case ITEM_FOR_SEAL_NORMAL_EVENT:						// 고양이 발 도장 이벤트(하얀색)
	case ITEM_FOR_SEAL_ELITE_EVENT:							// 고양이 발 도장 이벤트(보라색)
#ifdef SERV_COUNTRY_JP
	case ITEM_FOR_SEAL_RARE_EVENT:							// 고양이 발 도장 이벤트(노란색)		
	case ITEM_FOR_SEAL_UNIQUE_EVENT:						// 고양이 발 도장 이벤트(황금색)
#endif //SERV_COUNTRY_JP	
#ifdef CHILDRENS_DAY_EVENT_ITEM
	case ITEM_FOR_SEAL_UNIQUE_EVENT:						// 고양이 발 도장 이벤트(황금색)
#endif //CHILDRENS_DAY_EVENT_ITEM
#ifdef SERV_RURIEL_EVENT_ITEM
	case ITEM_FOR_SEAL_NORMAL_EVENT_RURIEL:
	case ITEM_FOR_SEAL_RARE_EVENT_RURIEL:
	case ITEM_FOR_SEAL_ELITE_EVENT_RURIEL:
	case ITEM_FOR_SEAL_UNIQUE_EVENT_RURIEL:
#endif //SERV_RURIEL_EVENT_ITEM
#ifdef SERV_ARIEL_EVENT_ITEM
	case ITEM_FOR_SEAL_NORMAL_EVENT_ARIEL:
	case ITEM_FOR_SEAL_RARE_EVENT_ARIEL:
	case ITEM_FOR_SEAL_ELITE_EVENT_ARIEL:
	case ITEM_FOR_SEAL_UNIQUE_EVENT_ARIEL:
#endif //SERV_ARIEL_EVENT_ITEM
		{
			RClickItemForSeal( pItem->GetUID() );
		}
		
		break;

	default:
		break;
	}
#endif	SEAL_ITEM
	//}} kimhc // 2009-08-19 // 봉인시키는 아이템을 오른쪽 클릭 시



	return false;
}

//{{ kimhc // 2009-08-19 // 아이템 봉인
#ifdef	SEAL_ITEM
void CX2UIInventory::RClickItemForSeal( UidType itemUID )
{
	CX2State*	pState	= NULL; 
	pState				= static_cast< CX2State* >( g_pMain->GetNowState() );
	if ( pState == NULL )
	{
		ASSERT( !"GetNowState() is NULL" );
		return;
	}

	CX2Cursor*	pCursor = NULL; 	
	pCursor				= pState->GetCursor();
	if ( pCursor == NULL )
	{
		ASSERT( !"GetCursor() is NULL" );
		return;
	}

	pCursor->ChangeCursorState( CX2Cursor::XCS_SEAL_ITEM );
	m_itemUIDForSeal	= itemUID;
}

bool CX2UIInventory::CanSealUpThisItem( CX2Item* pItem ) const
{
    if ( pItem == NULL )
    {
		ASSERT( !"pItem is NULL" );
		return false;
    }
 
    const CX2Item::ItemTemplet*		pItemTemplet	= pItem->GetItemTemplet();
	if ( pItemTemplet == NULL )
	{
		ASSERT( !"GetItemTemplet is NULL" );
		return false;
	}

	const CX2Item::ItemData&		kItemData		= pItem->GetItemData();

	//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
	if ( pItemTemplet->GetIsPcBang() == true )
	{
		g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
		return false;
	}
#endif	PC_BANG_WORK
	//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스

	// 기간제 아이템은 봉인할 수 없습니다.
	if ( kItemData.m_Period > 0 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4394 ), g_pMain->GetNowState() );		
		return false;
	}

	// 사용 불가 상태의 아이템은 봉인 할 수 없습니다.
	if ( pItem->IsDisabled() == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4395 ), g_pMain->GetNowState() );		
		return false;
	}

	// 장비(악세사리 포함), 아바타 아이템이 아니면 봉인 불가
	switch ( pItemTemplet->GetItemType() )
	{
	case CX2Item::IT_WEAPON:
	case CX2Item::IT_DEFENCE:
	case CX2Item::IT_ACCESSORY:
		// 봉인 가능
		break;

	default:
		{
#ifdef SERV_GOLD_TICKET_SEAL_COUNT
			if(IsGoldTicket(pItemTemplet->GetItemID()) == true)
			{
				break;
			}
#endif SERV_GOLD_TICKET_SEAL_COUNT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4396 ), g_pMain->GetNowState() );		
			return false;
		}
		break;
	}
	
	// 유저간의 거래가 이미 가능한(귀속되지 않은) 아이템은 아이템은 봉인 불가
	if ( pItemTemplet->GetVested() == false ||
		kItemData.m_bIsSealed == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4396 ), g_pMain->GetNowState() );		
		return false;
	}

	// 봉인 횟수
	if ( kItemData.m_ucTimesToBeSealed >= pItemTemplet->GetMaxSealCount() 
#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
		&& pItemTemplet->GetMaxSealCount() != CX2Inventory::SEAL_COUNT_MAX
#endif // SERV_REMOVE_SEAL_COUNT_DECREASE
		) // 지금까지 봉인된 횟수가 최대로 봉인 될수 있는 횟수 이상이면
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4398 ), g_pMain->GetNowState() );		
		return false;
	}

	// 무기나 방어구의 경우 내구도가 100% 미만인 경우 봉인 불가
	if ( pItemTemplet->GetItemType() == CX2Item::IT_WEAPON || pItemTemplet->GetItemType() == CX2Item::IT_DEFENCE )
	{
		if ( pItemTemplet->GetPeriodType() == CX2Item::PT_ENDURANCE && kItemData.m_Endurance != pItemTemplet->GetEndurance() )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4393 ), g_pMain->GetNowState() );		
			return false;
		}
	}

	// 삭제됨(유니크 등급 봉인 가능)
	//// 유니크 등급의 아이템은 봉인 불가
	//if ( pItemTemplet->GetItemGrade() == CX2Item::IG_UNIQUE )
	//{
	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4397 ), g_pMain->GetNowState() );		
	//	return false;
	//}

	return true;
}

#ifdef	SERV_SHARING_BANK_TEST
bool CX2UIInventory::IsSealItem( CX2Item* pItem, int &iCanSealCount )
{
	if( pItem == NULL )
	{
		return false;
	}

	const CX2Item::ItemData&		kItemData		=	pItem->GetItemData();
	const CX2Item::ItemTemplet*		pItemTemplet	=	NULL;
	iCanSealCount = 0;

	pItemTemplet			= pItem->GetItemTemplet();

	if ( pItemTemplet == NULL )
	{
		ASSERT( !"GetItemTemplet is NULL" );
		return false;
	}
	//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
	if ( pItemTemplet->GetIsPcBang() == true )
	{
		return false;
	}
#endif	PC_BANG_WORK
	//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스

	// 장비(악세사리 포함), 아바타 아이템이 아니면 봉인 불가
	switch ( pItemTemplet->GetItemType() )
	{
	case CX2Item::IT_WEAPON:
	case CX2Item::IT_DEFENCE:
	case CX2Item::IT_ACCESSORY:
		// 봉인 가능
		break;

	default:
		{
#ifdef SERV_GOLD_TICKET_SEAL_COUNT
			if(IsGoldTicket(pItemTemplet->GetItemID()) == true)
			{
				break;
			}
#endif SERV_GOLD_TICKET_SEAL_COUNT
			return false;
		}
		break;
	}

	// 유저간의 거래가 이미 가능한(귀속되지 않은) 아이템은 아이템은 봉인 불가
	if( pItemTemplet->GetVested() == false ||
		kItemData.m_bIsSealed == true )
	{
		return false;
	}

	iCanSealCount = pItemTemplet->GetMaxSealCount() - kItemData.m_ucTimesToBeSealed;
	
	return true;
}

bool CX2UIInventory::Handler_EGS_UPDATE_ITEM_POSITION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_ITEM_POSITION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
	
	std::vector< KTradeShareItemResult >::iterator vit;
	for(vit = kEvent.m_vecShareItemResult.begin(); vit != kEvent.m_vecShareItemResult.end(); vit++ )
	{
		kInventory.UpdateItemUID( vit->m_iBeforeItemUID, vit->m_iNewItemUID );
	}
	//{{ kimhc // 2009-08-07 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	if(g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_PRIVATE_BANK) == true )
	{
		g_pData->GetUIManager()->GetUIPrivateBank()->UpdateUI();
	}
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-07 // 캐릭터별 은행
	SetInventorySort( m_NowInventorySortType, GetNowInvenSortTypePageNum() );

	return true;
}

//{{ 지헌 : 은행간 교환 가능한 아이템인지 체크하는 함수
bool CX2UIInventory::CheckCanMoveShareItem( CX2Item* pFromItem, CX2Item* pToItem, KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ &kMoveInfo)
{
	// 둘다 NULL 이면 안되니 여기서도 체크 하자.
	if(pFromItem == NULL && pToItem == NULL)
		return false;

	// 봉인 가능 아이템이냐?
	bool bCanSeal = false;

	// FromItem  부터 체크
	if(pFromItem != NULL)
	{
		// 1. 기간제 아이템 거래 불가
		if( pFromItem->GetItemData().m_Period > 0 )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_14855 ), g_pMain->GetNowState() );
			return false;
		}

		// 2. 봉인아이템 체크
		int iCanSealCount = 0;
		if( IsSealItem( pFromItem, iCanSealCount ) == true )
		{
			if( iCanSealCount <= 0 )
			{
				// 봉인 횟수 0. 즉 거래 불가
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_14844 ), g_pMain->GetNowState() );
				return false;
			}
			else
			{
				bCanSeal = true;
			}
		}
		// 3. 거래 불가 아이템 체크
		else if( pFromItem->GetItemData().m_bIsSealed == false && pFromItem->GetItemTemplet()->GetVested() == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_14851 ), g_pMain->GetNowState() );
			return false;
		}
	}

	// ToItem 
	if(pToItem != NULL)
	{
		// 1. 기간제 아이템 거래 불가
		if( pToItem->GetItemData().m_Period > 0 )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_14855 ), g_pMain->GetNowState() );
			return false;
		}

		// 2. 봉인아이템 체크
		int iCanSealCount = 0;
		if( IsSealItem( pToItem, iCanSealCount ) == true )
		{
			if( iCanSealCount <= 0 )
			{
				// 봉인 횟수 0. 즉 거래 불가
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_14844 ), g_pMain->GetNowState() );
				return false;
			}
			else
			{
				bCanSeal = true;
			}
		}
		// 3. 거래 불가 아이템 체크
		else if( pToItem->GetItemData().m_bIsSealed == false && pToItem->GetItemTemplet()->GetVested() == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_14851 ), g_pMain->GetNowState() );
			return false;
		}
	}

	if(bCanSeal == true)
	{
		// 봉인 가능하니까 깍고 교환할건지 물어보자.
		m_kTempChangeSlotInfo = kMoveInfo;

#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
		const CX2Item::ItemTemplet* pItemTemplet = pFromItem->GetItemTemplet();
		if( pItemTemplet != NULL && pItemTemplet->GetMaxSealCount() == CX2Inventory::SEAL_COUNT_MAX )
		{
			g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ, g_pData->GetUIManager()->GetUIInventory()->m_kTempChangeSlotInfo );
			g_pMain->AddServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK );
		}	
		else
#endif // SERV_REMOVE_SEAL_COUNT_DECREASE
		g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_14843 ), CX2UIPrivateBank::UPBCM_CHANGE_INVENTORY_SLOT_ITEM_OK, g_pMain->GetNowState() );
		return false;
	}

	return true;
}
//}}
#endif	SERV_SHARING_BANK_TEST

void CX2UIInventory::ResetItemIDForSealAndCursorState()
{
	m_itemUIDForSeal	= 0;

	CX2State* pState	= NULL;
	pState				= static_cast< CX2State* >( g_pMain->GetNowState() );

	if( pState != NULL )
	{
		CX2Cursor* pCursor	= NULL;
		pCursor				= pState->GetCursor();

		if ( pCursor != NULL && pCursor->GetCurorState() == CX2Cursor::XCS_SEAL_ITEM )
		{
			pCursor->ChangeCursorState( CX2Cursor::XCS_NORMAL );
		}
	}	
}

bool CX2UIInventory::Handler_EGS_SEAL_ITEM_REQ( UidType itemUID )
{
    if ( g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
    {
		ASSERT( !"MyUser is NULL" );
		m_pSlotItemToBeSealed	= NULL;
		ResetItemIDForSealAndCursorState();
		return true;
    }

	CX2Item*				pItemToBeSealed		= NULL;
	const CX2Inventory&			kInventory	 		= g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	pItemToBeSealed					= kInventory.GetItem( itemUID );

	if ( pItemToBeSealed == NULL )
	{
		ASSERT( !"GetItem is NULL" );
		m_pSlotItemToBeSealed	= NULL;
		ResetItemIDForSealAndCursorState();
		return true;
	}

    const CX2Item::ItemTemplet*	pItemTemplet		= pItemToBeSealed->GetItemTemplet();

	if ( pItemTemplet == NULL )
	{
		ASSERT( !"GetItemTemplet is NULL" );
		m_pSlotItemToBeSealed	= NULL;
		ResetItemIDForSealAndCursorState();
		return true;
	}		

    const CX2Item::ItemData& kItemData = pItemToBeSealed->GetItemData();

	//if ( pItemData	== NULL )
	//{
	//	ASSERT( !"GetItemData is NULL" );
	//	m_pSlotItemToBeSealed	= NULL;
	//	ResetItemIDForSealAndCursorState();
	//	return true;
	//}

	if ( kInventory.GetItem( m_itemUIDForSeal ) == NULL ||
		 kInventory.GetItem( m_itemUIDForSeal )->GetItemTemplet() == NULL 
         )
	{
		ASSERT( !"m_itemUIDForSeal is wrong!" );
		m_pSlotItemToBeSealed	= NULL;
		ResetItemIDForSealAndCursorState();
		return true;
	}		

#ifdef SERV_SHARING_BANK_EVENT
	if( pItemTemplet->GetItemID() == _CONST_SHARING_BANK_EVENT_ITEM_::iKeyItem )
	{
		if ( g_pMain != NULL)
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4396 ), g_pMain->GetNowState() );
		}
		
		m_pSlotItemToBeSealed	= NULL;
		ResetItemIDForSealAndCursorState();
		return true;
	}
#endif SERV_SHARING_BANK_EVENT

	bool					bCanBeSealed			= true;
	int						iItemIDForSeal			= 0;
    const CX2Item::ItemTemplet*	pItemTempletForSeal = NULL;	// 어떤 고양이 발인지 알기위한 itemTemplet

    iItemIDForSeal			= kInventory.GetItem( m_itemUIDForSeal )->GetItemTemplet()->GetItemID();

	// 봉인할 아이템의 등급과 봉인에 쓰이는 아이템이 서로 등급이 일치하는지 검사
	switch ( pItemTemplet->GetItemGrade() )
	{

	case CX2Item::IG_NORMAL:								// 고양이 발 도장(하얀색)
		{
			switch ( iItemIDForSeal )
			{
			default:
				{
					bCanBeSealed		= false;	
					pItemTempletForSeal	= g_pData->GetItemManager()->GetItemTemplet( ITEM_FOR_SEAL_NORMAL );
				} break;

			case ITEM_FOR_SEAL_NORMAL:
			case ITEM_FOR_SEAL_NORMAL_EVENT:
#ifdef SERV_RURIEL_EVENT_ITEM
			case ITEM_FOR_SEAL_NORMAL_EVENT_RURIEL:
#endif //SERV_RURIEL_EVENT_ITEM
#ifdef SERV_ARIEL_EVENT_ITEM
			case ITEM_FOR_SEAL_NORMAL_EVENT_ARIEL:
#endif //SERV_ARIEL_EVENT_ITEM
				break;
			}
		}
		break;

	case CX2Item::IG_RARE:								// 고양이 발 도장(노란색)
		{
#if defined (SERV_COUNTRY_JP) || defined(SERV_RURIEL_EVENT_ITEM)
			switch ( iItemIDForSeal )
			{
			default:
				{
					bCanBeSealed		= false;	
					pItemTempletForSeal	= g_pData->GetItemManager()->GetItemTemplet( ITEM_FOR_SEAL_RARE );
				} break;

			case ITEM_FOR_SEAL_RARE:
#ifdef SERV_COUNTRY_JP
			case ITEM_FOR_SEAL_RARE_EVENT:		// SERV_SEAL_ITEM_EVENT
#endif //SERV_COUNTRY_JP
#ifdef SERV_RURIEL_EVENT_ITEM
			case ITEM_FOR_SEAL_RARE_EVENT_RURIEL:
#endif //SERV_RURIEL_EVENT_ITEM
#ifdef SERV_ARIEL_EVENT_ITEM
			case ITEM_FOR_SEAL_RARE_EVENT_ARIEL:
#endif SERV_ARIEL_EVENT_ITEM
				break;
			}
#else //SERV_COUNTRY_JP
			if ( iItemIDForSeal != ITEM_FOR_SEAL_RARE )
			{
				bCanBeSealed		= false;
				pItemTempletForSeal	= g_pData->GetItemManager()->GetItemTemplet( ITEM_FOR_SEAL_RARE );
			}
#endif //SERV_COUNTRY_JP
		}
		break;

	case CX2Item::IG_ELITE:								// 고양이 발 도장(보라색)
		{
			switch ( iItemIDForSeal )
			{
			default:
				{
					bCanBeSealed		= false;	
					pItemTempletForSeal	= g_pData->GetItemManager()->GetItemTemplet( ITEM_FOR_SEAL_ELITE );
				} break;

			case ITEM_FOR_SEAL_ELITE:
			case ITEM_FOR_SEAL_ELITE_EVENT:
#ifdef SERV_RURIEL_EVENT_ITEM
			case ITEM_FOR_SEAL_ELITE_EVENT_RURIEL:
#endif //SERV_RURIEL_EVENT_ITEM
#ifdef SERV_ARIEL_EVENT_ITEM
			case ITEM_FOR_SEAL_ELITE_EVENT_ARIEL:
#endif //SERV_ARIEL_EVENT_ITEM
				break;
			}
		}
		break;

	case CX2Item::IG_UNIQUE:								// 고양이 발 도장(황금색)
		{
#if defined(SERV_COUNTRY_JP) || defined(SERV_RURIEL_EVENT_ITEM)
			switch( iItemIDForSeal )
			{
			default:
				{
					bCanBeSealed		= false;
					pItemTempletForSeal	= g_pData->GetItemManager()->GetItemTemplet( ITEM_FOR_SEAL_UNIQUE );
				} break;

			case ITEM_FOR_SEAL_UNIQUE:
#ifdef SERV_COUNTRY_JP
			case ITEM_FOR_SEAL_UNIQUE_EVENT:	// SERV_SEAL_ITEM_EVENT
#endif //SERV_COUNTRY_JP
#ifdef SERV_RURIEL_EVENT_ITEM
			case ITEM_FOR_SEAL_UNIQUE_EVENT_RURIEL:			
#endif //SERV_RURIEL_EVENT_ITEM
#ifdef SERV_ARIEL_EVENT_ITEM
			case ITEM_FOR_SEAL_UNIQUE_EVENT_ARIEL:
#endif //SERV_ARIEL_EVENT_ITEM
				break;
			}
#else //SERV_COUNTRY_JP
			if ( iItemIDForSeal != ITEM_FOR_SEAL_UNIQUE )
			{
				bCanBeSealed		= false;
				pItemTempletForSeal	= g_pData->GetItemManager()->GetItemTemplet( ITEM_FOR_SEAL_UNIQUE );
			}
#endif //SERV_COUNTRY_JP
		}
		break;

	default:
		{
			ASSERT( !"This path is wrong!" );
			return true;
		}
		break;
	}

	// 일치하지 않으면
	if ( bCanBeSealed == false)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
            GET_REPLACED_STRING( ( STR_ID_4401, "SS", pItemTemplet->GetFullName_(), pItemTempletForSeal->GetFullName_() ) ), 
			g_pMain->GetNowState() );		

		m_pSlotItemToBeSealed	= NULL;
		ResetItemIDForSealAndCursorState();
		return true;
	}

	KEGS_SEAL_ITEM_REQ kEGS_SEAL_ITEM_REQ;

	kEGS_SEAL_ITEM_REQ.m_iDestItemUID	= itemUID;
	kEGS_SEAL_ITEM_REQ.m_iSealItemUID	= m_itemUIDForSeal;

	g_pData->GetServerProtocol()->SendPacket( EGS_SEAL_ITEM_REQ, kEGS_SEAL_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_SEAL_ITEM_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_SEAL_ITEM_ACK( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SEAL_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_pSlotItemToBeSealed = NULL;
	ResetItemIDForSealAndCursorState();

	g_pMain->DeleteServerPacket( EGS_SEAL_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			
			return true;
		}		
	}

	return false;
}

bool CX2UIInventory::Handler_EGS_UNSEAL_ITEM_REQ( UidType itemUID )
{
	KEGS_UNSEAL_ITEM_REQ kEGS_UNSEAL_ITEM_REQ;

	kEGS_UNSEAL_ITEM_REQ.m_iDestItemUID	= itemUID;
	m_itemUIDToBeUnSealed				= 0;

	g_pData->GetServerProtocol()->SendPacket( EGS_UNSEAL_ITEM_REQ, kEGS_UNSEAL_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_UNSEAL_ITEM_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_UNSEAL_ITEM_ACK( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UNSEAL_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_UNSEAL_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			return true;
		}		
	}

	return false;
}

#endif	SEAL_ITEM
//}} kimhc // 2009-08-19 // 봉인시키는 아이템을 오른쪽 클릭 시

//엘 감정, 되돌리기
// true면 판별 ( 불명 -> 색상 )
// false면 환원 ( 색상 -> 불명 )
void CX2UIInventory::OpenElChangerWindow( bool bIdentify, CX2Item* pItem )
{
	SAFE_DELETE_DIALOG( m_pDLGElChanger );

	if ( pItem == NULL || 
        pItem->GetItemTemplet() == NULL 
        )
		return;

	m_ElChangerItemUID		= pItem->GetItemData().m_ItemUID;
	m_ElChangerItemCount	= 1;

	m_pDLGElChanger = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_El_Changer_Window.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGElChanger );

	//아이템 이미지 셋팅해주고.
	CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"ItemImage" );
	if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
	{
		pStaticItemImage->GetPicture(0)->SetTex( 
            pItem->GetItemTemplet()->GetShopImage() 
            );
	}

	//아이템 이름 셋팅해주고.
	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Price" );
	if ( pStaticItemName != NULL )
	{
		pStaticItemName->SetString(0, 
            pItem->GetItemTemplet()->GetName()
            );
	}

	//감정하는건지 되돌리는건지 셋팅해주고.
	//감정하는거면 감정석.. 되돌리는거면 복구석 셋팅해주자 음허허허허허허허허허허허허허허허허...
	// true면 판별 ( 불명 -> 색상 )
	// false면 환원 ( 색상 -> 불명 )
	CKTDGUIStatic* pStaticItemNum2 = NULL;
	int itemNum = 0;
	if ( bIdentify == true )
	{
		CKTDGUIControl* pControl = m_pDLGElChanger->GetControl( L"Identify_Tool" );
		if ( pControl != NULL )
			pControl->SetShow( true );

		itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE );

		pStaticItemNum2 = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num2" );
	}
	else
	{
		CKTDGUIControl* pControl = m_pDLGElChanger->GetControl( L"Identify_Tool2" );
		if ( pControl != NULL )
			pControl->SetShow( true );

		itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE );

		pStaticItemNum2 = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num2" );

	}

	if ( pStaticItemNum2 != NULL )
	{
		wstringstream wstrstm;
		wstrstm << itemNum << GET_STRING( STR_ID_24 );
		pStaticItemNum2->SetString( 0, wstrstm.str().c_str() );
	}


	UpdateElChangerDLG(false);
}



void CX2UIInventory::UpdateElChangerDLG( bool bReadIME )
{
	//before, we must check the type if It is an Identifier or an returner.

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_ElChangerItemUID );
	if ( pItem == NULL || 
        pItem->GetItemTemplet() == NULL 
        )
		return;
	if ( m_pDLGElChanger == NULL )
		return;

	int needStoneQuantity = 0;
    if ( pItem->GetItemTemplet()->GetItemID() == CX2EnchantItem::ATI_UNKNOWN )
	{
		//identifier
		needStoneQuantity = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE );
	}
	else
	{
		//returner
		needStoneQuantity = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE );
	}
	const int MAGIC_MAX_EL_CHANGE_ONCE = 50;
	int nowQuantity;
	nowQuantity = min( needStoneQuantity, MAGIC_MAX_EL_CHANGE_ONCE );
	nowQuantity = min( pItem->GetItemData().m_Quantity, nowQuantity );


	// IME에서 값 받아오고
	CKTDGUIIMEEditBox* pQuantity = (CKTDGUIIMEEditBox*) m_pDLGElChanger->GetControl( L"IME_Identify_Window" );
	if(bReadIME)
	{
		m_ElChangerItemCount = g_pMain->GetEDFromString( pQuantity->GetText() );
	}	

	if ( m_ElChangerItemCount > nowQuantity )
		m_ElChangerItemCount = nowQuantity;

	// 확인해주고
	if ( m_ElChangerItemCount <= 0 )
	{
		if(bReadIME)				// 입력창으로 받아온 경우
			m_ElChangerItemCount = 0;
		else						// << 를 누른 경우 : 0 아래면 max로 순환시킨다.
			m_ElChangerItemCount = nowQuantity;	
	}

	WCHAR buff[256] = {0};
	_itow( m_ElChangerItemCount, buff, 10 );
	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str() );
}

void CX2UIInventory::OpenElChangerResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo )
{

	SAFE_DELETE_DIALOG( m_pDLGElChangerResult );

	m_pDLGElChangerResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_El_Changer_Result.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGElChangerResult );

	//	m_pDLGResolveItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 403, 350 ), D3DXVECTOR2( 270, 332 ) ) );

	// 	m_NowMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	// 	m_NowMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();	
	// 	D3DXVECTOR2 DLGPos = g_pKTDXApp->MouseConvertByResolution( m_NowMousePos ) - D3DXVECTOR2(222, 288);		// Left-top pos
	// 	if(DLGPos.x > 578)
	// 	{
	// 		DLGPos.x = 578;
	// 	}
	// 	if(DLGPos.y > 392)
	// 	{
	// 		DLGPos.y = 392;
	// 	}
	// 	m_pDLGElChangerResult->SetPos( DLGPos );
	m_pDLGElChangerResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 1024-578, 768-392 ), D3DXVECTOR2(222,288) ));

	CKTDGUIButton* pControl = (CKTDGUIButton*) m_pDLGElChangerResult->GetControl( L"Button_OK" );
	if ( pControl != NULL )
	{
		pControl->RequestFocus();
	}

	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGElChangerResult->GetControl( L"Static_Slot_Pic" );
	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGElChangerResult->GetControl( L"Static_SlotName" );
	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGElChangerResult->GetControl( L"Static_SlotNum" );


	//일단 초기화 부터 시켜줘야할꺼 같다 후덜덜
	const int MAGIC_EL_CHANGER_RESULT_MAX_SLOT_LIST_NUM = 6;
	for ( int slotNum = 0; slotNum < MAGIC_EL_CHANGER_RESULT_MAX_SLOT_LIST_NUM; slotNum++ )
	{
		if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
		{
			pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
		}

		if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
		{
			pStaticName->GetString(slotNum)->msg = L"";
		}

		if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
		{
			pStaticNum->GetString(slotNum)->msg = L"";
		}
	}

	std::map< int, int >::iterator i;
	int j = 0;
	for ( i = mapInsertedItem.begin(); i != mapInsertedItem.end(); i++, j++ )
	{
		int resultItemID = i->first;

		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
		if ( pItemTemplet != NULL )
		{
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
			{
                const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
				if ( pwszShopImage[0] != NULL )
					pStaticSlot->GetPicture(j)->SetTex( pwszShopImage );
				else
					pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

			if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
			{
                pStaticName->GetString(j)->msg = pItemTemplet->GetFullName_();
			}

			if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
			{
				wstringstream wstrstm;
				wstrstm  << i->second << GET_STRING( STR_ID_24 );
				pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
			}
		}
		// 아이템이 없는 경우
		else
		{
			pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			pStaticName->GetString(j)->msg = L"";
			pStaticNum->GetString(j)->msg = L"";
		}
	}

	UpdateInventorySlotList( vecKInventorySlotInfo );
}

bool CX2UIInventory::Handler_EGS_IDENTIFY_ITEM_REQ()
{
	KEGS_IDENTIFY_ITEM_REQ kPacket;

	kPacket.m_iItemUID = m_ElChangerItemUID;
	kPacket.m_iQuantity = m_ElChangerItemCount;

	g_pData->GetServerProtocol()->SendPacket( EGS_IDENTIFY_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_IDENTIFY_ITEM_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_IDENTIFY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_IDENTIFY_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_IDENTIFY_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			OpenElChangerResultWindow( kEvent.m_mapInsertedItem, kEvent.m_vecKInventorySlotInfo );
		}

		return true;
	}

	return false;

}

void CX2UIInventory::DrawSlotMouseOverImage()
{
	if ( GetCheckOperationCondition() == false )
	{
		m_pDLGSelectedItem->SetShow( false );
		return;
	}

	bool bCheck = false;

	if ( *m_pSlotBeforeDragging != NULL )
	{
		if ( IsInEquippedItemSlotReact( m_NowMousePos ) == true && (*m_pSlotBeforeDragging)->IsItem() )
		{

			//*m_DraggingItemUID = ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetItemUID();			

			const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

			// 예외처리 추가
			if( kInventory.GetItem( *m_DraggingItemUID, true ) == NULL || 
				kInventory.GetItem( *m_DraggingItemUID, true )->GetItemTemplet() == NULL 
                )
				return;

			CX2Item::ITEM_TYPE itemType = kInventory.GetItem( *m_DraggingItemUID, true )->GetItemTemplet()->GetItemType();
			if (  itemType == CX2Item::IT_WEAPON || itemType == CX2Item::IT_DEFENCE || itemType == CX2Item::IT_ACCESSORY )
			{
				CX2Unit::EQIP_POSITION equipPosition = kInventory.GetItem( *m_DraggingItemUID, true )->GetItemTemplet()->GetEqipPosition();

				for ( int i = 0; i < (int)m_SlotList.size(); i++ )
				{
					CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
					if ( pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED && pItemSlot->GetEquipPos() == equipPosition && 
						( itemType == CX2Item::IT_ACCESSORY || pItemSlot->GetFashion() == kInventory.GetItem( *m_DraggingItemUID )->GetItemTemplet()->GetFashion() ))
					{
						m_pDLGSelectedItem->SetPos( pItemSlot->GetPos() );
						m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( pItemSlot->GetSize() );

						bCheck = true;
					}
				}
			}
			else
			{
				if ( m_pNowOverItemSlot != NULL )
				{
					m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
					m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

					bCheck = true;
				}
			}
		}
		else
		{
			if ( m_pNowOverItemSlot != NULL )
			{
				m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
				m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

				bCheck = true;
			}
		}
	}
	else
	{
		if ( m_pNowOverItemSlot != NULL )
		{
			m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
			m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

			bCheck = true;
		}
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

wstring CX2UIInventory::GetSlotItemDesc()
{

	wstring itemDesc=  L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->IsResetItemUI() == false )
		{
			itemDesc = m_pNowOverItemSlot->GetSlotDesc();
		}
		else
		{	
			itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
#ifdef NEW_ITEM_NOTICE
			//마우스 오버 시 NewItem 하이라이트 꺼지도록 수정

			if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
				{
					if( true == kInventory.EraseNewItem(m_pNowOverItemSlot->GetItemUID()) )
					{
#ifdef SERV_REFORM_INVENTORY_TEST
						SetInventorySort(m_NowInventorySortType, GetNowInvenSortTypePageNum());
#else  SERV_REFORM_INVENTORY_TEST
						SetInventorySort(m_NowInventorySortType, m_NowInvenSortTypePageNum);
#endif SERV_REFORM_INVENTORY_TEST
					}
				}
			}				
#endif //NEW_ITEM_NOTICE
		}
	}

	return itemDesc;
}

#ifdef BUFF_TEMPLET_SYSTEM
bool CX2UIInventory::Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid, UidType iTempCode, int iItemID)
{
	KEGS_USE_ITEM_IN_INVENTORY_REQ kPacket;
	kPacket.m_iItemUID = iUid;

	switch( iItemID )
	{
	case PROTECTION_OF_NUT_ITEM_ID:
	case PROTECTION_OF_PUTA_ITEM_ID:
	case PROTECTION_OF_LAHELL_ITEM_ID: 
	case PROTECTION_OF_JAKIELL_ITEM_ID:
	case BELSSING_OF_SERAPHIM_ITEM_ID:
	case BELSSING_OF_GEV_ITEM_ID:
	case BELSSING_OF_AMON_ITEM_ID: 
	case BELSSING_OF_CRONOS_ITEM_ID:

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-11
	case ELIXIR_GIANT_POTION:
	case ELIXIR_BLAZING_BOMB:
	case ELIXIR_SPIRIT_OF_CHASER:
	case ELIXIR_CRADLE_OF_LITTLE_FAIRY:
	case ELIXIR_ICE_BALL_OF_DENIPH:
	case ELIXIR_FEATHER_OF_VENTUS:
	case ELIXIR_FLAME_RING_OF_ROSSO:
	case ELIXIR_BIG_HAND_POTION:
#endif // SERV_NEW_DEFENCE_DUNGEON
#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
	case THE_ESSENCE_OF_WEAK_HERETIC_POTION:
	case THE_ESSENCE_OF_HERETIC_POTION:
#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
	case BUFF_RURIEL_MANA_ENERGIZE_POTION:
#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
#ifdef RIDINGPET_STAMINA_ITEM
	case RIDINGPET_STAMINA_30_PER_ITEM_ID:
#endif RIDINGPET_STAMINA_ITEM
		{
			//비약, 영약 사용시 막 클릭할 때 여러개 사용되는 것 방지용
			kPacket.m_iTempCode = iTempCode;
		}break;
	default:
		{
			if( iTempCode > 0 )
				kPacket.m_iTempCode = iTempCode;
		}break;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_USE_ITEM_IN_INVENTORY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_USE_ITEM_IN_INVENTORY_ACK );

	return true;
}
#else  BUFF_TEMPLET_SYSTEM
bool CX2UIInventory::Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid, int iTempCode)
{
	KEGS_USE_ITEM_IN_INVENTORY_REQ kPacket;
	kPacket.m_iItemUID = iUid;
	if( iTempCode > 0 )
		kPacket.m_iTempCode = iTempCode;

	g_pData->GetServerProtocol()->SendPacket( EGS_USE_ITEM_IN_INVENTORY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_USE_ITEM_IN_INVENTORY_ACK );

	return true;
}
#endif BUFF_TEMPLET_SYSTEM

bool CX2UIInventory::Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 패킷 처리
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_ITEM_IN_INVENTORY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_USE_ITEM_IN_INVENTORY_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED		= kEvent.m_iED;
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}
#ifdef SERV_REFORM_INVENTORY_TEST
			SetInventorySort(m_NowInventorySortType, GetNowInvenSortTypePageNum());
#else  SERV_REFORM_INVENTORY_TEST
			SetInventorySort(m_NowInventorySortType, m_NowInvenSortTypePageNum);
#endif SERV_REFORM_INVENTORY_TEST

			// 마을간 워프 이동 검사
			if( kEvent.m_iWarpPointMapID > 0 && g_pData->GetLocationManager()->IsValidWarpField(kEvent.m_iWarpPointMapID) )
			{
				m_iCurrentVillageWarpIndex = -1;
#ifdef SERV_ADD_WARP_BUTTON
				CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
				if( NULL != pStateMenu )
				{
					pStateMenu->SetCurrentVillageWarpIndex( -1 );
				}
#endif // SERV_ADD_WARP_BUTTON

				if(g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME)
				{
					// 거래광장에서 워프					
					g_pData->SetStateArg(kEvent.m_iWarpPointMapID); 
					CX2StateSquareGame* pState = (CX2StateSquareGame*) g_pMain->GetNowState();
					pState->LeaveSquareGame();
				}
				else
				{
					// 이동티켓을 이용하여 마을 이동
					g_pData->SetStateArg(kEvent.m_iWarpPointMapID);
					CX2State* pState = (CX2State*) g_pMain->GetNowState();
					pState->Handler_EGS_STATE_CHANGE_FIELD_REQ(kEvent.m_iWarpPointMapID);
				}				
			}

#ifdef GUILD_MANAGEMENT
			if ( kEvent.m_iUsedItemID == GUILD_EXPANSION_ITEM_ID 
#ifdef EVENT_GUILD_ITEM
				|| kEvent.m_iUsedItemID == EVENT_GUILD_EXPANSION_ITEM_ID 
#endif //EVENT_GUILD_ITEM
				)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4593 ), g_pMain->GetNowState() );
			}
#endif GUILD_MANAGEMENT

#ifdef SERV_SKILL_NOTE
			if( g_pData != NULL && kEvent.m_iUsedItemID == SKILL_NOTE_ITEM_ID )
			{
				if( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL &&
					g_pData->GetUIManager()->GetUIInventory()->GetShow() == true )
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
				}

				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_5059 ), g_pMain->GetNowState() );				
			}
#endif

#ifdef PET_DROP_ITEM_PICKUP
			if( kEvent.m_iUsedItemID == ACTIVATION_DROP_ITEM_PICKUP_SKILL
#ifdef SERV_PET_AUTO_LOOTING_ITEM_CN
				|| kEvent.m_iUsedItemID == ACTIVATION_DROP_ITEM_PICKUP_SKILL_CN
#endif // SERV_PET_AUTO_LOOTING_ITEM_CN
				)
			{
				SetPetAutoLooting( kEvent.m_iTempCode );
			}
#endif //PET_DROP_ITEM_PICKUP

			return true;
		}
	}

	return false;
}

#ifdef SERV_GOLD_TICKET
bool CX2UIInventory::handler_EGS_CHARGE_POINT_NOT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHARGE_POINT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );		

	// 성공, 실패에 따라 메시지를 띄워주자
	if( g_pMain->IsValidPacket(kEvent.m_iOK) == true )
	{
		// 대만은, 필드에서 자기 캐쉬를 못보므로, 잔액을 갱신 해 줄 필요가 없다....가 아니구나.
		// 갱신을 해줘야 한다. 캐쉬샵 들어 갈 때 마다 갱신을 해 주는게 아니서
		g_pData->GetServerProtocol()->SendID( EGS_CHECK_BALANCE_REQ );
		g_pMain->AddServerPacket( EGS_CHECK_BALANCE_ACK );
		//{{ 2011.7.12 지헌 : Gash 포인트 통합
#ifdef SERV_BILLING_ADD_GASH_POINT
		g_pMain->AddServerPacket( EGS_CHECK_BALANCE_ACK ); // Gash 와 Elsword 따로 ACK옴
#endif // SERV_BILLING_ADD_GASH_POINT
		//}}

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( (STR_ID_14875, "i", kEvent.m_iChargedCash) ), g_pMain->GetNowState() );
		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_14875 ), g_pMain->GetNowState() );
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_14876 ), g_pMain->GetNowState() );
	}

	return false;
}
#endif //SERV_GOLD_TICKET

bool CX2UIInventory::CheckUseItem(CX2Item *pItem)
{
//#ifdef TITLE_SYSTEM
	for(int i=0; i<ARRAY_SIZE(TITLE_ITEM); ++i)
	{
        if( pItem->GetItemTemplet()->GetItemID() == TITLE_ITEM[i] )
		{
			if(g_pData != NULL && g_pData->GetTitleManager() != NULL && g_pData->GetTitleManager()->GetTakeTitle( TITLE_ID[i] ) == true)
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_813 ), g_pMain->GetNowState() );

				return false;
			}
		}
	}
//#endif

	return true;
}



//{{ 2007. 10. 8  최육사  
bool CX2UIInventory::Handler_EGS_OPEN_RANDOM_ITEM_REQ()
{
	if ( m_iRandomItemUID == 0 )
		return false;
	/*
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	for ( int i = CX2Inventory::ST_EQUIP; i <= CX2Inventory::ST_AVARTA; i++ )
	{
	if ( kInventory.GetUsedSlotNum( (CX2Inventory::SORT_TYPE)i ) >= kInventory.GetItemMaxNum( (CX2Inventory::SORT_TYPE)i ) )
	{
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_814 ), g_pMain->GetNowState() );
	return false;
	}
	}
	*/	

#ifdef FIX_RANDOM_CUBE_OPEN_RESULT_BUG
	//OPEN_RANDOM_ITEM_REQ를 이미 받은 경우 다시 처리하지 않도록 
	if( true == m_bProcessCubeOpen )
		return false;

	m_bProcessCubeOpen = true;
#endif //FIX_RANDOM_CUBE_OPEN_RESULT_BUG

	KEGS_OPEN_RANDOM_ITEM_REQ kEGS_OPEN_RANDOM_ITEM_REQ;

	kEGS_OPEN_RANDOM_ITEM_REQ.m_iItemUID  = m_iRandomItemUID;

#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
	if( true == m_bUsingRandomItemAutoOpen )
	{
		kEGS_OPEN_RANDOM_ITEM_REQ.m_bNotifyGetItem = false;
	}
	else
		kEGS_OPEN_RANDOM_ITEM_REQ.m_bNotifyGetItem = true;
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF

	g_pData->GetServerProtocol()->SendPacket( EGS_OPEN_RANDOM_ITEM_REQ, kEGS_OPEN_RANDOM_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_OPEN_RANDOM_ITEM_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_OPEN_RANDOM_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_OPEN_RANDOM_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( m_pDLGRandomItem != NULL )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRandomItem, NULL, false );
		m_pDLGRandomItem = NULL;
	}

#ifdef FIX_RANDOM_CUBE_OPEN_RESULT_BUG
	m_bProcessCubeOpen = false;
#endif //FIX_RANDOM_CUBE_OPEN_RESULT_BUG

	g_pMain->DeleteServerPacket( EGS_OPEN_RANDOM_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_CUBE_OPEN_ED_CONDITION
			if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
#endif //SERV_CUBE_OPEN_ED_CONDITION

			m_TempOpenRandomItemAck = kEvent;

			//{{ 2009.01.20 김태완
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
#else // AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			if( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
#endif // AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			{
				if(m_bUsingRandomItemAutoOpen)	// 자동 오픈을 사용중이면
				{
					if(m_pDLGAutoOpenMsgBox == NULL)
					{
						m_pDLGAutoOpenMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_815 ), m_pNowState );
					}
					// 받은 아이템을 저장하고 인벤토리 업뎃해준 후 바로 동일한 Uid에 대해서 Open을 요청
					UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );

					for(std::map<int, int>::iterator i = kEvent.m_mapInsertedItem.begin(); i != kEvent.m_mapInsertedItem.end(); i++)
					{
						std::map<int, int>::iterator target = m_MapAutoRandomItemGet.find(i->first);
						if(target != m_MapAutoRandomItemGet.end())
						{
							// 이미 얻었던 적이 있는 아이템이면 갯수를 더해주자
							target->second = target->second + i->second;
						}
						else
						{
							// 새로 나온 아이템이면 집어 넣자
							m_MapAutoRandomItemGet.insert( std::make_pair(i->first, i->second));
						}
					}

					m_OpenedRandomItemCount++;
					Handler_EGS_OPEN_RANDOM_ITEM_REQ();
					return true;
				}
			}
#endif

			switch(m_eRandomItemEventType)
			{
#ifdef ATTRACTION_ITEM_TEST
	#ifdef FIX_ICE_HEATER_EVENT
			case RIOET_ICE_HEATER:
			case RIOET_MINI_ICE_HEATER:
			case RIOET_ALL_IN_ONE_ICE_HEATER:
			case RIOET_RIDING_PET_GACHA:
			case RIOET_GOLD_ICE_HEATER:
#ifdef SERV_HALLOWEEN_ICE_HEATER
			case RIOET_HALLOWEEN_ICE_HEATER:
#endif //SERV_HALLOWEEN_ICE_HEATER
				{
					/// 모든 가열기를 한곳에서 처리
					PlayGetAttractionItem( m_eRandomItemEventType );
				} break;
	#else // FIX_ICE_HEATER_EVENT

			case RIOET_ICE_HEATER:
	#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD		/// 미니 얼음 조각상 가열기
			case RIOET_MINI_ICE_HEATER:
	#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
			case RIOET_ALL_IN_ONE_ICE_HEATER:
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	#ifdef RIDING_SYSTEM
			case RIOET_RIDING_PET_GACHA:
	#endif //RIDING_SYSTEM
				{
	#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
					PlayGetAttractionItem( m_eRandomItemEventType );	/// 아이템 이벤트 타입 인자를 추가
	#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
					PlayGetAttractionItem();
	#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
					//UpdateOpenAttractionItemAck();
				} break;
			case RIOET_GOLD_ICE_HEATER:
				{
					PlayGetAttractionItemGold();
				} break;
	#endif // FIX_ICE_HEATER_EVENT

#endif ATTRACTION_ITEM_TEST				 
#ifdef APRIL_5TH_TREE_DAY_EVENT
			case RIOET_APRIL_5TH_TREE_DAY_EVENT:
				{

					// 연출 : 다이얼로그 두개만 순서대로 띄우면 되니까 여기서 세팅해주자
					m_bPlayingTreedayEvent = true;
					SAFE_DELETE_DIALOG(m_pDLGTreeDay1);
					SAFE_DELETE_DIALOG(m_pDLGTreeDay2);

					m_pDLGTreeDay1 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Arbor_Day_Event01.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreeDay1 );
					m_pDLGTreeDay1->SetColor(D3DXCOLOR(1,1,1,0));

					m_pDLGTreeDay2 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Arbor_Day_Event02.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreeDay2 );
					m_pDLGTreeDay2->SetColor(D3DXCOLOR(1,1,1,0));

					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreeDay1, true );
					m_pDLGTreeDay1->Move( m_pDLGTreeDay1->GetPos(), D3DXCOLOR(1,1,1,1), 0.5f, true );	// 최초 0.5초간 밝아지고 (0~0.5)
					m_pDLGTreeDay1->Move( m_pDLGTreeDay1->GetPos(), D3DXCOLOR(1,1,1,1), 0.7f );			// 0.7초 유지 후 (0.5~1.2)
					m_pDLGTreeDay1->Move( m_pDLGTreeDay1->GetPos(), D3DXCOLOR(1,1,1,0), 0.5f );			// 0.5초간 사라진다 (1.2~1.5)

					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreeDay2, true );
					m_pDLGTreeDay2->Move( m_pDLGTreeDay2->GetPos(), D3DXCOLOR(1,1,1,0), 1.2f, true );	// 최초 1.2초간은 가만히 있고
					m_pDLGTreeDay2->Move( m_pDLGTreeDay2->GetPos(), D3DXCOLOR(1,1,1,1), 0.5f );			// 1번이 사라질 동안 나타나서 (1.2~1.7)
					m_pDLGTreeDay2->Move( m_pDLGTreeDay2->GetPos(), D3DXCOLOR(1,1,1,1), 1.0f );			// 1초 유지 후 끝 (1.7~2.7)

					// 시간에 맞춰서 사운드 켜주기 위함
					m_bAprilSoundPlayed = false;
					m_fPlayTime = 0.0f;
					//g_pKTDXApp->GetDeviceManager()->PlaySound( L"TreeDay_Event.ogg", false, false );

				} break;
#endif

#ifdef CUBE_OPEN_IMAGE_MANAGER
			case RIOET_EVENT_CUBE:
				{
					g_pData->GetCubeOpenImageManager()->SetPlaying( true );
					m_fPlayTime = 0.0f;
					SAFE_DELETE_DIALOG(m_pDLGEventCubeResultImage);

					if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
					{
						std::wstring wstrResultImageNamge;

						std::map<int, int>::iterator mit = m_TempOpenRandomItemAck.m_mapInsertedItem.begin();
						while(mit != m_TempOpenRandomItemAck.m_mapInsertedItem.end())
						{
							// 결과템 반짝이는데 사용하기 위해서 매니져에 결과템 ID를 담아두자
							// 결과템이 여러개면? 
							g_pData->GetCubeOpenImageManager()->AddResultItemID(mit->first);

							if( g_pData->GetCubeOpenImageManager()->GetResultImageName(m_RandomItemID, mit->first, wstrResultImageNamge) == true )
							{
								// 결과 큐브다!
								g_pData->GetCubeOpenImageManager()->SetNowResultEventCubePlaying( true );

								// 결과 큐브 재생 중에는 esc 안먹히게 해야 한다...
								m_pDLGEventCubeResultImage = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Event_Cube_Result.lua", 0.0f  );
								CKTDGUIStatic* pStaticCubeOpenLoading = (CKTDGUIStatic*) m_pDLGEventCubeResultImage->GetControl( L"Event_Cube_Result" );
								CKTDGUIControl::CPictureData *pPicture = pStaticCubeOpenLoading->GetPicture(0);
								pPicture->SetTex( wstrResultImageNamge.c_str() , L"Note");

								g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEventCubeResultImage );
								m_pDLGEventCubeResultImage->SetColor(D3DXCOLOR(1,1,1,0));

								g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGEventCubeResultImage, true );
								m_pDLGEventCubeResultImage->Move( m_pDLGEventCubeResultImage->GetPos(), D3DXCOLOR(1,1,1,1), 0.5f, true );	// 최초 0.5초간 밝아지고 (0~0.5)
								m_pDLGEventCubeResultImage->Move( m_pDLGEventCubeResultImage->GetPos(), D3DXCOLOR(1,1,1,1), 1.5f );			// 1.5초 유지 후 (0.5~1.2)
								m_pDLGEventCubeResultImage->Move( m_pDLGEventCubeResultImage->GetPos(), D3DXCOLOR(1,1,1,0), 0.5f );			// 0.5초간 사라진다 (1.2~1.5)
								break;
							}
							else
							{
								dbg::cerr << L"큐브 이미지, 결과 이미지 이름이 없다?? 결과 ID : " << mit->second << dbg::endl;
							}
							++mit;
						}		
					}					
				} break;
#endif // CUBE_OPEN_IMAGE_MANAGER

#ifdef SERV_TREASURE_BOX_ITEM
			case RIOET_TREASURE_BOX_ITEM:
				{					
					// 연출 : 다이얼로그 9개 띄우기
					m_bPlayingTreasureBox = true;
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox1);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox2);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox3);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox4);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox5);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox6);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox7);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox8);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox9);
					SAFE_DELETE_DIALOG(m_pDLGTreasureBox_White);
					SAFE_DELETE_DIALOG(m_pDLGGetAttractionItemBG);

					m_pDLGGetAttractionItemBG = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Unit_State_RandomItem_BG.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGetAttractionItemBG );
					m_pDLGGetAttractionItemBG->SetColor( D3DXCOLOR( 0,0,0,0) );
					m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 0,0,0,0.7f), 0.5f, true );


					m_pDLGTreasureBox1 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box1.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox1 );
					m_pDLGTreasureBox1->SetColor(D3DXCOLOR(1,1,1,0));

					m_pDLGTreasureBox2 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box2.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox2 );
					m_pDLGTreasureBox2->SetColor(D3DXCOLOR(1,1,1,0));

					m_pDLGTreasureBox3 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box3.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox3 );
					m_pDLGTreasureBox3->SetColor(D3DXCOLOR(1,1,1,0));

					m_pDLGTreasureBox4 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box4.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox4 );
					m_pDLGTreasureBox4->SetColor(D3DXCOLOR(1,1,1,0));

					m_pDLGTreasureBox5 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box5.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox5 );
					m_pDLGTreasureBox5->SetColor(D3DXCOLOR(0,0,0,0));

					m_pDLGTreasureBox6 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box6.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox6 );
					m_pDLGTreasureBox6->SetColor(D3DXCOLOR(0,0,0,0));

					m_pDLGTreasureBox7 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box7.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox7 );
					m_pDLGTreasureBox7->SetColor(D3DXCOLOR(0,0,0,0));

					m_pDLGTreasureBox8 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box8.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox8 );
					m_pDLGTreasureBox8->SetColor(D3DXCOLOR(0,0,0,0));

					m_pDLGTreasureBox9 = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box9.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox9 );
					m_pDLGTreasureBox9->SetColor(D3DXCOLOR(1,1,1,0));

					m_pDLGTreasureBox_White = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Treasure_Box_White.lua", 0.0f );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTreasureBox_White );
					m_pDLGTreasureBox_White->SetColor(D3DXCOLOR(1,1,1,0));

#ifdef TREASURE_BOX_ITEM_THIN
					if( m_TreasureBoxSPItemID == 67004400 )
					{
						CKTDGUIStatic *pStatic1 = (CKTDGUIStatic*)m_pDLGTreasureBox1->GetControl(L"Treasure_Box");
						pStatic1->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box1_1.tga", L"TREASURE_BOX1");
						CKTDGUIStatic *pStatic2 = (CKTDGUIStatic*)m_pDLGTreasureBox2->GetControl(L"Treasure_Box2");
						pStatic2->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box2_1.tga", L"TREASURE_BOX2");
						CKTDGUIStatic *pStatic3 = (CKTDGUIStatic*)m_pDLGTreasureBox3->GetControl(L"Treasure_Box3");
						pStatic3->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box3_1.tga", L"TREASURE_BOX3");
						CKTDGUIStatic *pStatic4 = (CKTDGUIStatic*)m_pDLGTreasureBox4->GetControl(L"Treasure_Box");
						pStatic4->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box4_1.tga", L"TREASURE_BOX4");
						CKTDGUIStatic *pStatic5 = (CKTDGUIStatic*)m_pDLGTreasureBox5->GetControl(L"Treasure_Box");
						pStatic5->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box5_1.tga", L"TREASURE_BOX5");
						CKTDGUIStatic *pStatic6 = (CKTDGUIStatic*)m_pDLGTreasureBox6->GetControl(L"Treasure_Box");
						pStatic6->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box6_1.tga", L"TREASURE_BOX6");
						CKTDGUIStatic *pStatic7 = (CKTDGUIStatic*)m_pDLGTreasureBox7->GetControl(L"Treasure_Box");
						pStatic7->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box7_1.tga", L"TREASURE_BOX7");
						CKTDGUIStatic *pStatic8 = (CKTDGUIStatic*)m_pDLGTreasureBox8->GetControl(L"Treasure_Box");
						pStatic8->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box8_1.tga", L"TREASURE_BOX8");
						CKTDGUIStatic *pStatic9 = (CKTDGUIStatic*)m_pDLGTreasureBox9->GetControl(L"Treasure_Box");
						pStatic9->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box9_1.tga", L"TREASURE_BOX9");
						m_TreasureBoxSPItemID = 0;
					}
					else if( m_TreasureBoxSPItemID == 67005029 || m_TreasureBoxSPItemID == 67005030 || m_TreasureBoxSPItemID == 67005031 ||
							m_TreasureBoxSPItemID == 67005032 || m_TreasureBoxSPItemID == 67005033 || m_TreasureBoxSPItemID == 67005034 ||
							m_TreasureBoxSPItemID == 67005035 || m_TreasureBoxSPItemID == 67005036 || m_TreasureBoxSPItemID == 67005037 ||
							m_TreasureBoxSPItemID == 67005038 || m_TreasureBoxSPItemID == 67005039 || m_TreasureBoxSPItemID == 67005040	)
					{
						CKTDGUIStatic *pStatic1 = (CKTDGUIStatic*)m_pDLGTreasureBox1->GetControl(L"Treasure_Box");
						pStatic1->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box1_2.tga", L"TREASURE_BOX1");
						CKTDGUIStatic *pStatic2 = (CKTDGUIStatic*)m_pDLGTreasureBox2->GetControl(L"Treasure_Box2");
						pStatic2->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box2_2.tga", L"TREASURE_BOX2");
						CKTDGUIStatic *pStatic3 = (CKTDGUIStatic*)m_pDLGTreasureBox3->GetControl(L"Treasure_Box3");
						pStatic3->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box3_2.tga", L"TREASURE_BOX3");
						CKTDGUIStatic *pStatic4 = (CKTDGUIStatic*)m_pDLGTreasureBox4->GetControl(L"Treasure_Box");
						pStatic4->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box4_2.tga", L"TREASURE_BOX4");
						CKTDGUIStatic *pStatic5 = (CKTDGUIStatic*)m_pDLGTreasureBox5->GetControl(L"Treasure_Box");
						pStatic5->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box5_2.tga", L"TREASURE_BOX5");
						CKTDGUIStatic *pStatic6 = (CKTDGUIStatic*)m_pDLGTreasureBox6->GetControl(L"Treasure_Box");
						pStatic6->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box6_2.tga", L"TREASURE_BOX6");
						CKTDGUIStatic *pStatic7 = (CKTDGUIStatic*)m_pDLGTreasureBox7->GetControl(L"Treasure_Box");
						pStatic7->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box7_2.tga", L"TREASURE_BOX7");
						CKTDGUIStatic *pStatic8 = (CKTDGUIStatic*)m_pDLGTreasureBox8->GetControl(L"Treasure_Box");
						pStatic8->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box8_2.tga", L"TREASURE_BOX8");
						CKTDGUIStatic *pStatic9 = (CKTDGUIStatic*)m_pDLGTreasureBox9->GetControl(L"Treasure_Box");
						pStatic9->GetPicture(0)->SetTex(L"DLG_UI_Treasure_Box9_2.tga", L"TREASURE_BOX9");
						m_TreasureBoxSPItemID = 0;
					}
#endif

					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox1, true );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox2, true );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox3, true );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox4, true );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox5, true );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox6, true );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox7, true );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox8, true );					

					float fDecreaseTime = 0.5f;
					float fDecreaseNum = 0.0f;

					m_pDLGTreasureBox1->Move( m_pDLGTreasureBox1->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);
					m_pDLGTreasureBox2->Move( m_pDLGTreasureBox2->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime, false, false, false, 1, false);
					m_pDLGTreasureBox3->Move( m_pDLGTreasureBox3->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime * 2, false, false, false, 1, false);
					m_pDLGTreasureBox4->Move( m_pDLGTreasureBox4->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime * 3, false, false, false, 1, false);

					m_pDLGTreasureBox1->Move( m_pDLGTreasureBox1->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime * 3, false, false, false, 1, false);
					m_pDLGTreasureBox2->Move( m_pDLGTreasureBox2->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox2->Move( m_pDLGTreasureBox2->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime * 3 - 0.2f, false, false, false, 1, false);
					m_pDLGTreasureBox3->Move( m_pDLGTreasureBox3->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox3->Move( m_pDLGTreasureBox3->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime * 3 - 0.4f, false, false, false, 1, false);
					m_pDLGTreasureBox4->Move( m_pDLGTreasureBox4->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);						

					m_pDLGTreasureBox4->Move( m_pDLGTreasureBox4->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime * 3 - 0.6f, false, false, false, 1, false);

					fDecreaseTime = 0.3f;

					m_pDLGTreasureBox1->Move( m_pDLGTreasureBox1->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox1->Move( m_pDLGTreasureBox1->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime, false, false, false, 1, false);
					m_pDLGTreasureBox2->Move( m_pDLGTreasureBox2->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox2->Move( m_pDLGTreasureBox2->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime, false, false, false, 1, false);
					m_pDLGTreasureBox3->Move( m_pDLGTreasureBox3->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox3->Move( m_pDLGTreasureBox3->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime, false, false, false, 1, false);
					m_pDLGTreasureBox4->Move( m_pDLGTreasureBox4->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox4->Move( m_pDLGTreasureBox4->GetPos(), D3DXCOLOR(1,1,1,0), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox5->Move( m_pDLGTreasureBox5->GetPos(), D3DXCOLOR(1,1,1,0), 3.3f, false, false, false, 1, false);	
					m_pDLGTreasureBox6->Move( m_pDLGTreasureBox6->GetPos(), D3DXCOLOR(1,1,1,0), 3.3f + fDecreaseTime, false, false, false, 1);
					m_pDLGTreasureBox7->Move( m_pDLGTreasureBox7->GetPos(), D3DXCOLOR(1,1,1,0), 3.3f + fDecreaseTime * 2, false, false, false, 1, false);
					m_pDLGTreasureBox8->Move( m_pDLGTreasureBox8->GetPos(), D3DXCOLOR(1,1,1,0), 3.3f + fDecreaseTime * 3, false, false, false, 1, false);


					m_pDLGTreasureBox5->Move( m_pDLGTreasureBox5->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox5->Move( m_pDLGTreasureBox5->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.1f, false, false, false, 1, false);
					m_pDLGTreasureBox6->Move( m_pDLGTreasureBox6->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox6->Move( m_pDLGTreasureBox6->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.2f, false, false, false, 1, false);
					m_pDLGTreasureBox7->Move( m_pDLGTreasureBox7->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox7->Move( m_pDLGTreasureBox7->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.3f, false, false, false, 1, false);
					m_pDLGTreasureBox8->Move( m_pDLGTreasureBox8->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox8->Move( m_pDLGTreasureBox8->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.4f, false, false, false, 1, false);

					fDecreaseTime = 0.2f;

					m_pDLGTreasureBox5->Move( m_pDLGTreasureBox5->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox5->Move( m_pDLGTreasureBox5->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.1f, false, false, false, 1, false);
					m_pDLGTreasureBox6->Move( m_pDLGTreasureBox6->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox6->Move( m_pDLGTreasureBox6->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.2f, false, false, false, 1, false);
					m_pDLGTreasureBox7->Move( m_pDLGTreasureBox7->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox7->Move( m_pDLGTreasureBox7->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.3f, false, false, false, 1, false);
					m_pDLGTreasureBox8->Move( m_pDLGTreasureBox8->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

					m_pDLGTreasureBox8->Move( m_pDLGTreasureBox8->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.4f, false, false, false, 1, false);

					fDecreaseTime = 0.1f;

					for(int i = 0;i < 6; i++)
					{	
						m_pDLGTreasureBox5->Move( m_pDLGTreasureBox5->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

						m_pDLGTreasureBox5->Move( m_pDLGTreasureBox5->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.1f , false, false, false, 1, false);
						m_pDLGTreasureBox6->Move( m_pDLGTreasureBox6->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

						m_pDLGTreasureBox6->Move( m_pDLGTreasureBox6->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.1f , false, false, false, 1, false);
						m_pDLGTreasureBox7->Move( m_pDLGTreasureBox7->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

						m_pDLGTreasureBox7->Move( m_pDLGTreasureBox7->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.1f , false, false, false, 1, false);
						m_pDLGTreasureBox8->Move( m_pDLGTreasureBox8->GetPos(), D3DXCOLOR(1,1,1,1), fDecreaseTime, false, false, false, 1, false);

						m_pDLGTreasureBox8->Move( m_pDLGTreasureBox8->GetPos(), D3DXCOLOR(0,0,0,0), fDecreaseTime * 3 - 0.1f, false, false, false, 1, false);
					}

					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox_White, true );
					m_pDLGTreasureBox_White->Move( m_pDLGTreasureBox_White->GetPos(), D3DXCOLOR(1,1,1,0), 7.0f, true );	// 최초 1.2초간은 가만히 있고
					m_pDLGTreasureBox_White->Move( m_pDLGTreasureBox_White->GetPos(), D3DXCOLOR(1,1,1,1), 0.3f );			// 1번이 사라질 동안 나타나서 (1.2~1.7)
					m_pDLGTreasureBox_White->Move( m_pDLGTreasureBox_White->GetPos(), D3DXCOLOR(1,1,1,1), 0.6f );			// 1초 유지 후 끝 (1.7~2.7)
					m_pDLGTreasureBox_White->Move( m_pDLGTreasureBox_White->GetPos(), D3DXCOLOR(1,1,1,0), 0.3f );			// 1초 유지 후 끝 (1.7~2.7)

					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGTreasureBox9, true );
					m_pDLGTreasureBox9->Move( m_pDLGTreasureBox9->GetPos(), D3DXCOLOR(1,1,1,0), 8.0f, true );	// 최초 1.2초간은 가만히 있고
					m_pDLGTreasureBox9->Move( m_pDLGTreasureBox9->GetPos(), D3DXCOLOR(1,1,1,1), 0.1f );			// 1번이 사라질 동안 나타나서 (1.2~1.7)
					m_pDLGTreasureBox9->Move( m_pDLGTreasureBox9->GetPos(), D3DXCOLOR(1,1,1,1), 1.5f );			// 1초 유지 후 끝 (1.7~2.7)

					// 시간에 맞춰서 사운드 켜주기 위함
					m_bTreasureBoxSoundPlayed = false;
					m_fPlayTime = 0.0f;
					//g_pKTDXApp->GetDeviceManager()->PlaySound( L"TreeDay_Event.ogg", false, false );
				} break;
#endif	SERV_TREASURE_BOX_ITEM

			case RIOET_MONSTER_CARD_SET:
				{
					UpdateOpenMonsterCardSet();
				} break;

			default:
			case RIOET_NONE:
				{
					//PlayGetRandomItem();
#ifdef RANDOM_CUBE_UI_NEW
					UpdateOpenRandomItemAckNew();
#else
					UpdateOpenRandomItemAck();
#endif RANDOM_CUBE_UI_NEW
				}break;
			}
			return true;
		}		
		else
		{
			//{{ 2009.01.20 김태완
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
#else // AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			if( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
#endif // AUTH_RANDOM_ITEM_AUTO_OPEN_OPERATOR
			{
				if(m_bUsingRandomItemAutoOpen)	// 자동 오픈을 사용중이면
				{
					// 자동 오픈이 끝났습니다~
					if(m_pDLGAutoOpenMsgBox != NULL)
					{
						SAFE_DELETE_DIALOG(m_pDLGAutoOpenMsgBox);
						m_pDLGAutoOpenMsgBox = NULL;
					}

					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_816, "i", m_OpenedRandomItemCount ) ), m_pNowState );

					UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
					// 더미 : OpenDecompositionResultWindow 안에서 인벤토리 정리를 안 하기 위한 빈 벡터 =3=
					std::vector<KInventoryItemInfo> vecDummyInfo;
					vecDummyInfo.clear();
					//OpenDecompositionResultWindow(m_MapAutoRandomItemGet, vecDummyInfo, false);
					OpenResolveResultWindow( m_MapAutoRandomItemGet, vecDummyInfo, false );
					// 다 쓴 물건은 치웁시다
					m_bUsingRandomItemAutoOpen = false;
					m_OpenedRandomItemCount = 0;
					m_MapAutoRandomItemGet.clear();
				}

			}
#endif

#ifdef ATTRACTION_ITEM_TEST
			// 실패시 창을 없애자~
			if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
			{
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );

				if( NULL != m_pDLGOpenAttraction )
				{
					m_pDLGOpenAttraction->SetHasUnit( NULL );
				}
			}

			if ( m_pDLGOpenAttraction != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOpenAttraction, NULL, false );
				m_pDLGOpenAttraction = NULL;
			}
#endif
			m_eRandomItemEventType = RIOET_NONE;
		}
	}

	return false;
}
//}}

#ifdef RANDOM_CUBE_UI_NEW
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
void CX2UIInventory::UpdateOpenMiniRandomItemAckNew()
{
	// 예전 창을 지우고..
	if ( m_pDLGOpenAttraction != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOpenAttraction, NULL, false );
	m_pDLGOpenAttraction = NULL;

	m_eRandomItemEventType = RIOET_NONE;
	m_eItemObtainResultType = IORT_CUBE_OPEN;

	wstring itemName = L"";
	wstring acquireComment;

	bool bCheck = false;

	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
	{
		for(std::map<int, int>::iterator i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++)
		{
			std::map<int, int>::iterator target = m_mapCubeItemGet.find(i->first);
			if(target != m_mapCubeItemGet.end())
			{
				// 이미 얻었던 적이 있는 아이템이면 갯수를 더해주자
				target->second = target->second + i->second;
			}
			else
			{
				// 새로 나온 아이템이면 집어 넣자
				m_mapCubeItemGet.insert( std::make_pair(i->first, i->second));
			}
		}

		bCheck = true;
	}

	if ( m_TempOpenRandomItemAck.m_iRessurectionCount != 0 )
	{
		wstringstream tempString;
		int getStoneNum = m_TempOpenRandomItemAck.m_iRessurectionCount - g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum(); 

		m_mapCubeItemGet.insert( std::make_pair(127030, getStoneNum));

		g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( m_TempOpenRandomItemAck.m_iRessurectionCount );

		//{{ kimhc // 2009-07-21 // 던전 내에서 부활석 큐브 사용시에 메인UI(왼쪽 상단)의 부활석 갯수가 변경 안되는 문제
#ifdef	REAL_TIME_ELSWORD
		if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
		{
			g_pX2Game->ResetReBirthStoneNumUI();
		}

#endif	REAL_TIME_ELSWORD
		//}} kimhc // 2009-07-21 // 던전 내에서 부활석 큐브 사용시에 메인UI(왼쪽 상단)의 부활석 갯수가 변경 안되는 문제

		if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
		{
			g_pData->GetUIManager()->GetUICharInfo()->ResetResurrectionStoneUI();
		}
		bCheck = true;
	}

	if ( bCheck == true )
	{
		// 더미 : OpenDecompositionResultWindow 안에서 인벤토리 정리를 안 하기 위한 빈 벡터 =3=
		std::vector<KInventoryItemInfo> vecDummyInfo;
		vecDummyInfo.clear();
		//OpenDecompositionResultWindow(m_MapAutoRandomItemGet, vecDummyInfo, false);

		OpenResolveResultWindow( m_mapCubeItemGet, vecDummyInfo, false );
		m_mapCubeItemGet.clear();
	}
	else if ( m_TempOpenRandomItemAck.m_iRestoreSpirit != 0 )
	{
		wstringstream tempString;
#ifdef RESTORE_SPIRIT_VALUE_BUG_FIX
		float fGetSpirit = static_cast<float>( m_TempOpenRandomItemAck.m_iRestoreSpirit - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit );
		fGetSpirit = fGetSpirit / static_cast<float>( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax ) * 100.0f;
		fGetSpirit = floor(fGetSpirit * 10.0f + 0.5f) / 10.0f;
		tempString << GET_REPLACED_STRING( ( STR_ID_819, "f", fGetSpirit ) );
#else
		int getSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit;
		getSpirit = (int)(getSpirit / (float)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax * 100.0f);
		tempString << GET_REPLACED_STRING( ( STR_ID_819, "i", getSpirit ) );
#endif RESTORE_SPIRIT_VALUE_BUG_FIX

		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_iSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit;
		// 근성도 변경시의 UI 업뎃은 StateMenu에서 처리된다

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), tempString.str().c_str(), g_pMain->GetNowState() );
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_820 ), g_pMain->GetNowState() );
	}

	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );

	if(m_pDLGCubeCheck != NULL)
	{
		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGCubeCheck->GetControl( L"ButtonCube_Window_Complete" );
		m_pDLGCubeCheck->RequestFocus(pButton);
	}

}
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

void CX2UIInventory::UpdateOpenRandomItemAckNew()
{



	m_eRandomItemEventType = RIOET_NONE;

	wstring itemName = L"";
	wstring acquireComment;

	bool bCheck = false;

	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
	{
		for(std::map<int, int>::iterator i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++)
		{
			std::map<int, int>::iterator target = m_mapCubeItemGet.find(i->first);
			if(target != m_mapCubeItemGet.end())
			{
				// 이미 얻었던 적이 있는 아이템이면 갯수를 더해주자
				target->second = target->second + i->second;
			}
			else
			{
				// 새로 나온 아이템이면 집어 넣자
				m_mapCubeItemGet.insert( std::make_pair(i->first, i->second));
			}
		}

		bCheck = true;
	}

	if ( m_TempOpenRandomItemAck.m_iRessurectionCount != 0 )
	{
		wstringstream tempString;
		int getStoneNum = m_TempOpenRandomItemAck.m_iRessurectionCount - g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum(); 

		m_mapCubeItemGet.insert( std::make_pair(127030, getStoneNum));


		g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( m_TempOpenRandomItemAck.m_iRessurectionCount );

		//{{ kimhc // 2009-07-21 // 던전 내에서 부활석 큐브 사용시에 메인UI(왼쪽 상단)의 부활석 갯수가 변경 안되는 문제
#ifdef	REAL_TIME_ELSWORD
		if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
		{
			g_pX2Game->ResetReBirthStoneNumUI();
		}

#endif	REAL_TIME_ELSWORD
		//}} kimhc // 2009-07-21 // 던전 내에서 부활석 큐브 사용시에 메인UI(왼쪽 상단)의 부활석 갯수가 변경 안되는 문제

		if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
		{
			g_pData->GetUIManager()->GetUICharInfo()->ResetResurrectionStoneUI();
		}
		bCheck = true;
	}




	if ( bCheck == true )
	{

		// 더미 : OpenDecompositionResultWindow 안에서 인벤토리 정리를 안 하기 위한 빈 벡터 =3=
		std::vector<KInventoryItemInfo> vecDummyInfo;
		vecDummyInfo.clear();
		//OpenDecompositionResultWindow(m_MapAutoRandomItemGet, vecDummyInfo, false);
		
		OpenResolveResultWindow( m_mapCubeItemGet, vecDummyInfo, false );
		m_mapCubeItemGet.clear();

	}
	else if ( m_TempOpenRandomItemAck.m_iRestoreSpirit != 0 )
	{
		wstringstream tempString;
#ifdef RESTORE_SPIRIT_VALUE_BUG_FIX
		float fGetSpirit = static_cast<float>( m_TempOpenRandomItemAck.m_iRestoreSpirit - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit );
		fGetSpirit = fGetSpirit / static_cast<float>( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax ) * 100.0f;
		fGetSpirit = floor(fGetSpirit * 10.0f + 0.5f) / 10.0f;
		tempString << GET_REPLACED_STRING( ( STR_ID_819, "f", fGetSpirit ) );
#else
		int getSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit;
		getSpirit = (int)(getSpirit / (float)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax * 100.0f);
		tempString << GET_REPLACED_STRING( ( STR_ID_819, "i", getSpirit ) );
#endif RESTORE_SPIRIT_VALUE_BUG_FIX

		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_iSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit;
		// 근성도 변경시의 UI 업뎃은 StateMenu에서 처리된다

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), tempString.str().c_str(), g_pMain->GetNowState() );
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_820 ), g_pMain->GetNowState() );
	}


	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );

	if(m_pDLGCubeCheck != NULL)
	{
		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGCubeCheck->GetControl( L"ButtonCube_Window_Complete" );
		m_pDLGCubeCheck->RequestFocus(pButton);
	}
}
#endif RANDOM_CUBE_UI_NEW

void CX2UIInventory::UpdateOpenRandomItemAck()
{
	m_eRandomItemEventType = RIOET_NONE;

	wstring itemName = L"";
	wstring acquireComment;

	bool bCheck = false;

	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
	{
		std::map< int, int >::iterator i;

		int nCount = 0;
		wstring getExplanation = L"";
		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
		{
			int resultItemID = i->first;
	
			//{{ oasis907 : 김상윤 [2010.4.27] // 큐브 아이템 결과창 수량 표기
			int resultNum = i->second;
			//}}

			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
			if ( pItemTemplet != NULL )
				itemName = pItemTemplet->GetFullName_();

			if ( nCount > 0 )
			{
				getExplanation += L", ";
			}

			getExplanation += itemName;
			
			//{{ oasis907 : 김상윤 [2010.4.27] // 큐브 아이템 결과창 수량 표기
			getExplanation += L" ";

			WCHAR buff[256] = {0};
			_itow( resultNum, buff, 10 );
			getExplanation += buff;

			getExplanation += GET_STRING( STR_ID_24 );
			//}}

			nCount++;
		}

		acquireComment = GET_REPLACED_STRING( ( STR_ID_817, "LI", getExplanation, KHanSungPostWordUnicodeWrapper( (WCHAR*)getExplanation.c_str(), STR_ID_198, STR_ID_199 ) ) );
		bCheck = true;
	}

	if ( m_TempOpenRandomItemAck.m_iRessurectionCount != 0 )
	{
		wstringstream tempString;
		int getStoneNum = m_TempOpenRandomItemAck.m_iRessurectionCount - g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum(); 
		tempString << GET_REPLACED_STRING( ( STR_ID_818, "i", getStoneNum ) );

		g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( m_TempOpenRandomItemAck.m_iRessurectionCount );

		//{{ kimhc // 2009-07-21 // 던전 내에서 부활석 큐브 사용시에 메인UI(왼쪽 상단)의 부활석 갯수가 변경 안되는 문제
#ifdef	REAL_TIME_ELSWORD
		if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
		{
			g_pX2Game->ResetReBirthStoneNumUI();
		}
		
#endif	REAL_TIME_ELSWORD
		//}} kimhc // 2009-07-21 // 던전 내에서 부활석 큐브 사용시에 메인UI(왼쪽 상단)의 부활석 갯수가 변경 안되는 문제

		if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
		{
			g_pData->GetUIManager()->GetUICharInfo()->ResetResurrectionStoneUI();
		}

		acquireComment = tempString.str().c_str();
		bCheck = true;
	}




	if ( bCheck == true )
	{
		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.str().c_str(), g_pMain->GetNowState() );
		SAFE_DELETE_DIALOG( m_pDLGCubeCheck );
		m_pDLGCubeCheck = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cube_Result_Window.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCubeCheck );
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCubeCheck->GetControl( L"StaticCube_Window_Text" );
		if ( pStatic != NULL && pStatic->GetString(0) != NULL )
		{
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring tpString = CWordLineHandler::GetStrByLineBreakInX2Main(acquireComment.c_str(), 366, XUF_DODUM_15_BOLD );
#else //CLIENT_GLOBAL_LINEBREAK
			wstring tpString = g_pMain->GetStrByLienBreak( acquireComment.c_str(), 366, XUF_DODUM_15_BOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
			pStatic->GetString(0)->msg = tpString;
		}

		if ( m_RandomItemID == 129755 )
		{
			CKTDGUIStatic* pStaticCube = (CKTDGUIStatic*)m_pDLGCubeCheck->GetControl( L"g_pStaticCube_Image" );
			if ( pStaticCube != NULL )
			{
				if ( pStaticCube->GetPicture(0) != NULL )
				{
					pStaticCube->GetPicture(0)->SetTex( L"DLG_Common_New_Texture62.tga", L"CUBE_EVENT_MILITARY" );
				}
			}
		}
	}
	else if ( m_TempOpenRandomItemAck.m_iRestoreSpirit != 0 )
	{
		wstringstream tempString;
#ifdef RESTORE_SPIRIT_VALUE_BUG_FIX
		float fGetSpirit = static_cast<float>( m_TempOpenRandomItemAck.m_iRestoreSpirit - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit );
		fGetSpirit = fGetSpirit / static_cast<float>( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax ) * 100.0f;
		fGetSpirit = floor(fGetSpirit * 10.0f + 0.5f) / 10.0f;
		tempString << GET_REPLACED_STRING( ( STR_ID_819, "f", fGetSpirit ) );
#else
		int getSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit;
		getSpirit = (int)(getSpirit / (float)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax * 100.0f);
		tempString << GET_REPLACED_STRING( ( STR_ID_819, "i", getSpirit ) );
#endif RESTORE_SPIRIT_VALUE_BUG_FIX

		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_iSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit;
		// 근성도 변경시의 UI 업뎃은 StateMenu에서 처리된다

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), tempString.str().c_str(), g_pMain->GetNowState() );
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_820 ), g_pMain->GetNowState() );
	}


	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );

	if(m_pDLGCubeCheck != NULL)
	{
		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGCubeCheck->GetControl( L"ButtonCube_Window_Complete" );
		m_pDLGCubeCheck->RequestFocus(pButton);
	}
}

void CX2UIInventory::UpdateOpenMonsterCardSet()
{
	m_eRandomItemEventType = RIOET_NONE;

	wstring itemName = L"";
	wstring acquireComment;

	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
	{
		std::map< int, int >::iterator i;

		int nCount = 0;
		wstring getExplanation = L"";
		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
		{
			int resultItemID = i->first;

			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
			if ( pItemTemplet != NULL )
				itemName = pItemTemplet->GetFullName_();

			if ( nCount > 0 )
			{
				getExplanation += L", ";
			}

			getExplanation += itemName;

			nCount++;
		}

		acquireComment = GET_REPLACED_STRING( ( STR_ID_817, "LI", getExplanation, KHanSungPostWordUnicodeWrapper( (WCHAR*)getExplanation.c_str(), STR_ID_198, STR_ID_199 ) ) );
		
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), acquireComment.c_str(), g_pMain->GetNowState() );		

		UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
	}
}

void CX2UIInventory::OpenResolveWindow( CX2SlotItem* pSlot )
{
	// 	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
	// 	{
	// 		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
	// 			L"장착중인 아이템은 분해할 수 없습니다.", g_pMain->GetNowState() );
	// 
	// 		return;
	// 	}

	if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
		g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2650 ), g_pMain->GetNowState() );		
		return;
	}

	if ( pSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
	{
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );

#ifdef ENABLE_RESOLVE_FASHION

		if ( pItem == NULL || 
            pItem->GetItemTemplet() == NULL 
            )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_STRING( STR_ID_821 ), g_pMain->GetNowState() );

			return;
		}

		//{{ kimhc // 2010-01-06 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
		if ( pItem->GetItemTemplet()->GetIsPcBang() == true )
		{
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
			return;
		}
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-06 // PC방 프리미엄 서비스

		//{{ kimhc // 2009-09-08 // 봉인된 아이템 분해 불가
#ifdef	SEAL_ITEM

		if ( pItem->GetItemData().m_bIsSealed == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4477 ), g_pMain->GetNowState() );
			return; 
		}
#endif	SEAL_ITEM
		//}} kimhc // 2009-09-08 // 봉인된 아이템 분해 불가

		switch(pItem->GetItemTemplet()->GetItemType())
		{
		case CX2Item::IT_WEAPON:
		case CX2Item::IT_DEFENCE:
			{
				if ( pItem->GetItemTemplet()->GetFashion() == true )
				{
					if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY &&
						pItem->GetItemData().m_Period > 0 )
					{
						//기간제로 판명.
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
							GET_STRING( STR_ID_822 ), g_pMain->GetNowState() );

						return;
					}
				}
			} break;
		case CX2Item::IT_ACCESSORY:
			{
				if ( pItem->GetItemTemplet()->GetFashion() != true )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
						GET_STRING( STR_ID_823 ), g_pMain->GetNowState() );

					return;
				}
				
				if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY &&
					pItem->GetItemData().m_Period > 0 )
				{
					//기간제로 판명.
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
						GET_STRING( STR_ID_822 ), g_pMain->GetNowState() );

					return;
				}
				
			} break;
		default:
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
					GET_STRING( STR_ID_823 ), g_pMain->GetNowState() );

				return;
			}
		}


#else //ENABLE_RESOLVE_FASHION

		if ( m_NowInventorySortType != CX2Inventory::ST_EQUIP )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_STRING( STR_ID_823 ), g_pMain->GetNowState() );

			return;
		}

#endif //ENABLE_RESOLVE_FASHION

		if ( pItem == NULL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_STRING( STR_ID_821 ), g_pMain->GetNowState() );

			return;
		}

#ifdef ITEM_RECOVERY_TEST
		if(true == pItem->IsDisabled())
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_800 ), g_pMain->GetNowState() );
			return; 
		}
#endif // ITEM_RECOVERY_TEST

		if ( pItem != NULL )
		{
			m_DecompositionItemUID = pItem->GetUID();
			if ( m_pDLGResolveItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGResolveItem, NULL, false );
			}

			m_pDLGResolveItem = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_Item_Apart.lua");
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItem );

			D3DXVECTOR2 DLGPos = g_pMain->GetWindowPos( D3DXVECTOR2(357,243), D3DXVECTOR2(184,160) );
			m_pDLGResolveItem->SetPos( DLGPos );

			CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItem->GetControl( L"Static_Broken_Item_Slot" );
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
			{
                const wchar_t* pwszShopImage = pItem->GetItemTemplet()->GetShopImage();
				if ( pwszShopImage[0] != NULL )
					pStaticSlot->GetPicture(0)->SetTex( pwszShopImage );
				else
					pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

			CKTDGUIControl* pControlButton = m_pDLGResolveItem->GetControl( L"Button_OK" );
			if ( pControlButton != NULL )
				pControlButton->RequestFocus();
		}
	}		

}

#ifdef SERV_MULTI_RESOLVE
void CX2UIInventory::OpenResolveWindow()
{
	if(g_pData->GetUIManager()->GetUIResolveItem() == NULL)
	{
		g_pData->GetUIManager()->CreateUIResolveItem();
	}

	g_pData->GetUIManager()->GetUIResolveItem()->SetShow( true );
}
#endif SERV_MULTI_RESOLVE

bool CX2UIInventory::Handler_EGS_RESOLVE_ITEM_REQ()
{
	KEGS_RESOLVE_ITEM_REQ kPacket;

	kPacket.m_iItemUID  = m_DecompositionItemUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_RESOLVE_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RESOLVE_ITEM_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_RESOLVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESOLVE_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_RESOLVE_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_MULTI_RESOLVE
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}
#endif SERV_MULTI_RESOLVE

			OpenResolveResultWindow( kEvent.m_mapInsertedItem, kEvent.m_vecKInventorySlotInfo, kEvent.m_bJackpot );

			return true;
		}		
	}

	return false;
}


void CX2UIInventory::OpenResolveResultWindow( std::map< int, int >& mapInsertedItem, 
											 std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
											 bool bJackpot )
{
	SAFE_DELETE_DIALOG( m_pDLGResolveItemResult );

	m_DecompositionWindowNowPageNum = 1;
	m_mapResolveResultInsertedItem = mapInsertedItem;

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	switch( m_eItemObtainResultType )
	{
	case IORT_CUBE_OPEN:
		{
#ifdef CUBE_OPEN_IMAGE_MANAGER
			if( g_pData->GetCubeOpenImageManager() != NULL && g_pData->GetCubeOpenImageManager()->IsShowResult(m_RandomItemID) == false)
			{
				UpdateInventorySlotList( vecKInventorySlotInfo );
				return;
			}
			if( g_pData->GetCubeOpenImageManager() != NULL && g_pData->GetCubeOpenImageManager()->IsShowResultCustom(m_RandomItemID) == true )
			{
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
				if( m_RandomItemID == 130825 )
				{
					InitItemObtainResult2( L"DLG_UI_Event_Cube_Result_Custom.lua", mapInsertedItem, 1 );
				}
				else
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
				InitItemObtainResult( L"DLG_UI_Event_Cube_Result_Custom.lua", mapInsertedItem);
			}
			else
#endif CUBE_OPEN_IMAGE_MANAGER
			InitItemObtainResult( L"DLG_UI_Cube_Result.lua", mapInsertedItem);
		}
		break;
	case IORT_RANDOM_EXCHANGE:
		{
			InitItemObtainResult( L"DLG_UI_Random_Exchange_Result.lua", mapInsertedItem);
		}
		break;
	case IORT_ITEM_RESOLVE:
		{ 
			InitItemResolveResult( L"DLG_UI_Apart_Result.lua", mapInsertedItem);
		}
		break;
	default:
		{
			//아이템을 획득한 타입이 지정되어 있지 않다면 결과창 출력하지않고 return;
			return;
		}
		break;
	}

	//페이지 출력
	int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
	if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
	{
		maxPage += 1;
	}

	wstring wstrPage = GET_REPLACED_STRING( (STR_ID_16214, "ii", m_DecompositionWindowNowPageNum, maxPage) );

	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"Static_Page_Number");
	if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
	{
		pStaticPageNum->GetString(0)->msg = wstrPage.c_str();
	}

	if( false == vecKInventorySlotInfo.empty() )
	{
		UpdateInventorySlotList( vecKInventorySlotInfo );
	}
#else

#ifdef CUBE_OPEN_IMAGE_MANAGER
	if( g_pData->GetCubeOpenImageManager() != NULL && g_pData->GetCubeOpenImageManager()->IsShowResult(m_RandomItemID) == false)
	{
		UpdateInventorySlotList( vecKInventorySlotInfo );
		// 결과템 반짝이게 해봅시다.

		return;
	}
#endif // CUBE_OPEN_IMAGE_MANAGER

#ifdef RANDOM_CUBE_UI_NEW
	if(m_bCubeOpen == true)
	{
		m_pDLGResolveItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Cube_Result.lua" );
	}
	else
	{
		m_pDLGResolveItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Apart_Result.lua" );
	}
#else
	m_pDLGResolveItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Apart_Result.lua" );
#endif RANDOM_CUBE_UI_NEW


	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItemResult );

	//	if ( bJackpot == true && m_pDLGResolveItemResult->GetControl( L"StaticBig_Item_Acquirement" ) != NULL )
	//		m_pDLGResolveItemResult->GetControl( L"StaticBig_Item_Acquirement" )->SetShow( true );

	//	m_pDLGResolveItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 403, 350 ), D3DXVECTOR2( 270, 332 ) ) );

	// 	m_NowMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	// 	m_NowMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();	
	// 	D3DXVECTOR2 DLGPos = g_pKTDXApp->MouseConvertByResolution( m_NowMousePos ) - D3DXVECTOR2(222, 288);		// Left-top pos
	// 	if(DLGPos.x > 578)
	// 		DLGPos.x = 578;
	// 	if(DLGPos.y > 392)
	// 		DLGPos.y = 392;
	// 	m_pDLGResolveItemResult->SetPos( DLGPos );


#ifdef RANDOM_CUBE_UI_NEW
	if(m_bCubeOpen == true)
	{
		m_pDLGResolveItemResult->SetPos( D3DXVECTOR2( 320, 230 ) );
	}
	else
	{
		m_pDLGResolveItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 446, 376 ), D3DXVECTOR2( 222, 288) ) );
	}
#else
	m_pDLGResolveItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 446, 376 ), D3DXVECTOR2( 222, 288) ) );

#endif RANDOM_CUBE_UI_NEW








	CKTDGUIButton* pControl = (CKTDGUIButton*) m_pDLGResolveItemResult->GetControl( L"Button_OK" );
	if ( pControl != NULL )
	{
		pControl->RequestFocus();
	}

	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_Slot_Pic" );
	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotName" );
	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotNum" );


	//일단 초기화 부터 시켜줘야할꺼 같다 후덜덜
	for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; slotNum++ )
	{
		if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
		{
			pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
		}
#ifdef RANDOM_CUBE_UI_NEW
		if(m_bCubeOpen)
		{
			pStaticName->GetString(slotNum*2)->msg = L"";
			pStaticName->GetString(slotNum*2 + 1)->msg = L"";
		}
		else
		{
			if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
			{
				pStaticName->GetString(slotNum)->msg = L"";
			}
		}
#else
		if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
		{
			pStaticName->GetString(slotNum)->msg = L"";
		}
#endif RANDOM_CUBE_UI_NEW
		if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
		{
			pStaticNum->GetString(slotNum)->msg = L"";
		}
	}

	std::map< int, int >::iterator i;
	int j = 0;
	for ( i = mapInsertedItem.begin(); i != mapInsertedItem.end(); i++, j++ )
	{
		int resultItemID = i->first;

		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
		if ( pItemTemplet != NULL )
		{
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
			{
                const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
				if ( pwszShopImage[0] != NULL )
					pStaticSlot->GetPicture(j)->SetTex( pwszShopImage );
				else
					pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

#ifdef RANDOM_CUBE_UI_NEW
			if(m_bCubeOpen == true)
			{
				bool bCheckTwoLine = false;
#ifdef CLIENT_GLOBAL_LINEBREAK
					wstring wstrFullName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetFullName().c_str(), 145, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
				wstring wstrFullName = g_pMain->GetStrByLienBreak( 
                    pItemTemplet->GetFullName_(),
                    145, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
				if ( wstrFullName.find( L"\n") != -1 )
					bCheckTwoLine = true;

				if(pStaticName->GetString(j*2) != NULL)
				{
					if ( bCheckTwoLine == false )
					{
						pStaticName->GetString(j*2)->msg = wstrFullName;
						pStaticName->GetString(j*2+1)->msg = L"";
					}
					else
					{
						pStaticName->GetString(j*2)->msg = L"";
						pStaticName->GetString(j*2+1)->msg = wstrFullName;
					}
				}
			}
			else
			{
				if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
				{
                    pStaticName->GetString(j)->msg = pItemTemplet->GetFullName_();
				}
			}
#else
			if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
			{
                pStaticName->GetString(j)->msg = pItemTemplet->GetFullName_();
			}
#endif RANDOM_CUBE_UI_NEW
			if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
			{
				wstringstream wstrstm;
				wstrstm  << i->second << GET_STRING( STR_ID_24 );
				pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
			}
		}
		// 아이템이 없는 경우
		else
		{
			pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			pStaticName->GetString(j)->msg = L"";
			pStaticNum->GetString(j)->msg = L"";
		}
	}

	int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
	if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
	{
		maxPage += 1;
	}

	wstringstream wstrstm;
	wstrstm << m_DecompositionWindowNowPageNum << L"/" << maxPage;
	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"Static_Page_Number");
	if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
	{
		pStaticPageNum->GetString(0)->msg = wstrstm.str().c_str();
	}


	UpdateInventorySlotList( vecKInventorySlotInfo );

#endif RANDOM_EXCHANGE_RESULT_VIEW

}


void CX2UIInventory::ChangeResolveResultPage( bool bNextPage )
{
	//거시기 창이 띄워져 있는 경우에만..
	if ( m_pDLGResolveItemResult != NULL )
	{
		if ( bNextPage == false )
		{
			if ( m_DecompositionWindowNowPageNum <= 1 )
				return;

			m_DecompositionWindowNowPageNum--;
		}

		int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
		if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
		{
			maxPage += 1;
		}

		if ( bNextPage == true )
		{
			if ( m_DecompositionWindowNowPageNum >= maxPage )
				return;

			m_DecompositionWindowNowPageNum++;
		}
#ifdef RANDOM_EXCHANGE_RESULT_VIEW

		wstring wstrPage = GET_REPLACED_STRING( (STR_ID_16214, "ii", m_DecompositionWindowNowPageNum, maxPage) );
		CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"Static_Page_Number");
		if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
		{
			pStaticPageNum->GetString(0)->msg = wstrPage.c_str();
		}

		//출력해야할 페이지 아이템의 iterator값 설정( 시작:it 끝 :maxiter )
		std::map< int, int >::iterator it = m_mapResolveResultInsertedItem.begin();
		for ( int nextNum = 0; nextNum < (m_DecompositionWindowNowPageNum - 1) * RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; ++nextNum )
		{
			it++;
		}

		std::map< int, int >::iterator maxiter = m_mapResolveResultInsertedItem.begin();
		for ( int nextNum = 0; nextNum < ( RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM*m_DecompositionWindowNowPageNum ); nextNum++ )
		{
			maxiter++;
		}

		CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_Slot_Pic" );
		CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotName" );
		CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotNum" );

		switch( m_eItemObtainResultType )
		{
		case IORT_CUBE_OPEN:
		case IORT_RANDOM_EXCHANGE:
			{
				//아이템 슬롯 초기화
				for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; ++slotNum )
				{
					if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
					{
						pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
					}

					pStaticName->GetString(slotNum*2)->msg = L"";
					pStaticName->GetString(slotNum*2 + 1)->msg = L"";

					if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
					{
						pStaticNum->GetString(slotNum)->msg = L"";
					}
				}
				//해당 아이템으로 슬롯 설정
				int index = 0;
				int resultItemID = -1;
				while( it != m_mapResolveResultInsertedItem.end() && it != maxiter )
				{
					resultItemID = it->first;

                    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
                    if ( pItemTemplet != NULL )
					{
						if ( pStaticSlot != NULL && pStaticSlot->GetPicture(index) != NULL )
						{
                            const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
							if ( pwszShopImage[0] != NULL )
								pStaticSlot->GetPicture(index)->SetTex( pwszShopImage );
							else
								pStaticSlot->GetPicture(index)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
						}

						bool bCheckTwoLine = false;
#ifdef CLIENT_GLOBAL_LINEBREAK
						wstring wstrFullName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetFullName_(), 145, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
						wstring wstrFullName = g_pMain->GetStrByLienBreak( 
                            pItemTemplet->GetFullName_(), 
                            145, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
						if ( wstrFullName.find( L"\n") != -1 )
							bCheckTwoLine = true;

						if(pStaticName->GetString(index*2) != NULL)
						{
							if ( bCheckTwoLine == false )
							{
								pStaticName->GetString(index*2)->msg = wstrFullName;
								pStaticName->GetString(index*2+1)->msg = L"";
							}
							else
							{
								pStaticName->GetString(index*2)->msg = L"";
								pStaticName->GetString(index*2+1)->msg = wstrFullName;
							}
						}

						if ( pStaticNum != NULL && pStaticNum->GetString(index) != NULL )
						{
							wstring wstrCount = GET_REPLACED_STRING( ( STR_ID_865, "i", it->second) );
							pStaticNum->GetString(index)->msg = wstrCount.c_str();
						}
					}
					++it;
					++index;
				}
			}
			break;
		case IORT_ITEM_RESOLVE:
			{
				// 아이템 슬롯 초기화
				for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; ++slotNum )
				{
					if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
					{
						pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
					}

					if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
					{
						pStaticName->GetString(slotNum)->msg = L"";
					}

					if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
					{
						pStaticNum->GetString(slotNum)->msg = L"";
					}
				}

				//해당 아이템으로 슬롯 설정
				int index = 0;
				int resultItemID = -1;
				while( it != m_mapResolveResultInsertedItem.end() && it != maxiter )
				{
					resultItemID = it->first;

					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
					if ( pItemTemplet != NULL )
					{
						if ( pStaticSlot != NULL && pStaticSlot->GetPicture(index) != NULL )
						{
                            const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
							if ( pwszShopImage[0] != NULL )
								pStaticSlot->GetPicture(index)->SetTex( pwszShopImage );
							else
								pStaticSlot->GetPicture(index)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
						}
						if ( pStaticName != NULL && pStaticName->GetString(index) != NULL )
						{
                            pStaticName->GetString(index)->msg = pItemTemplet->GetFullName_();
						}

						if ( pStaticNum != NULL && pStaticNum->GetString(index) != NULL )
						{
							wstring wstrCount = GET_REPLACED_STRING( ( STR_ID_865, "i", it->second) );
							pStaticNum->GetString(index)->msg = wstrCount.c_str();
						}
					}
					++it;
					++index;
				}
			}
			break;
		default:
			//해당 항목에 지정된 값이 아니라면, 결과창이 출력될 수 없었기 때문에 return;
			return;
			break;
		}


#else RANDOM_EXCHANGE_RESULT_VIEW
		wstringstream wstrstm;
		wstrstm << m_DecompositionWindowNowPageNum << L"/" << maxPage;
		CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"Static_Page_Number");
		if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
		{
			pStaticPageNum->GetString(0)->msg = wstrstm.str().c_str();
		}

		CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_Slot_Pic" );
		CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotName" );
		CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotNum" );

		std::map< int, int >::iterator i;
		int j = 0;
		i = m_mapResolveResultInsertedItem.begin();
		for ( int nextNum = 0; nextNum < (m_DecompositionWindowNowPageNum - 1) * RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; nextNum++ )
		{
			i++;
		}

		std::map< int, int >::iterator maxiter = m_mapResolveResultInsertedItem.begin();
		for ( int nextNum = 0; nextNum < ( RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM*m_DecompositionWindowNowPageNum ); nextNum++ )
		{
			maxiter++;
		}

		//일단 초기화 부터 시켜줘야할꺼 같다 후덜덜
		for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; slotNum++ )
		{
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
			{
				pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
			}

#ifdef RANDOM_CUBE_UI_NEW
			if(m_bCubeOpen)
			{
				pStaticName->GetString(slotNum*2)->msg = L"";
				pStaticName->GetString(slotNum*2 + 1)->msg = L"";
			}
			else
			{
				if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
				{
					pStaticName->GetString(slotNum)->msg = L"";
				}
			}
#else
			if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
			{
				pStaticName->GetString(slotNum)->msg = L"";
			}
#endif RANDOM_CUBE_UI_NEW

			if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
			{
				pStaticNum->GetString(slotNum)->msg = L"";
			}
		}

		for ( ; i != m_mapResolveResultInsertedItem.end() && i != maxiter ; i++, j++ )
		{
			int resultItemID = i->first;

			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
			if ( pItemTemplet != NULL )
			{
				if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
				{
                    const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
					if ( pwszShopImage[0] != NULL )
						pStaticSlot->GetPicture(j)->SetTex( pwszShopImage );
					else
						pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
				}

#ifdef RANDOM_CUBE_UI_NEW
				if(m_bCubeOpen == true)
				{
					bool bCheckTwoLine = false;
#ifdef CLIENT_GLOBAL_LINEBREAK
					wstring wstrFullName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetFullName().c_str(), 145, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
					wstring wstrFullName = g_pMain->GetStrByLienBreak( 
                        pItemTemplet->GetFullName_(),
                        145, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
					if ( wstrFullName.find( L"\n") != -1 )
						bCheckTwoLine = true;

					if(pStaticName->GetString(j*2) != NULL)
					{
						if ( bCheckTwoLine == false )
						{
							pStaticName->GetString(j*2)->msg = wstrFullName;
							pStaticName->GetString(j*2+1)->msg = L"";
						}
						else
						{
							pStaticName->GetString(j*2)->msg = L"";
							pStaticName->GetString(j*2+1)->msg = wstrFullName;
						}
					}
				}
				else
				{
					if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
					{
#ifdef CLIENT_GLOBAL_LINEBREAK
						wstring wstrFullName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetFullName().c_str(), 145, XUF_DODUM_13_SEMIBOLD );
						pStaticName->GetString(j)->msg = wstrFullName;
#else //CLIENT_GLOBAL_LINEBREAK
                        pStaticName->GetString(j)->msg = pItemTemplet->GetFullName_();
#endif //CLIENT_GLOBAL_LINEBREAK
					}
				}
#else
				if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
				{
                    pStaticName->GetString(j)->msg = pItemTemplet->GetFullName_();
				}
#endif RANDOM_CUBE_UI_NEW

				if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
				{
					wstringstream wstrstm;
					wstrstm  << i->second << GET_STRING( STR_ID_24 );
					pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
				}
			}
		}
#endif RANDOM_EXCHANGE_RESULT_VIEW
	}
}

#ifndef SERV_SOCKET_NEW
//{{ oasis907 : 김상윤 [2010.4.19] // 
void CX2UIInventory::OpenSocketWindow( CX2SlotItem* pSlot )
{
	// 	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
	// 	{
	// 		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
	// 			L"장착중인 아이템은 소켓 추가할 수 없습니다.", g_pMain->GetNowState(), -1, 3.0f );
	// 
	// 		return;
	// 	}

	if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( pSlot->GetItemTID() ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
			GET_STRING( STR_ID_824 ), g_pMain->GetNowState() , -1, 3.0f );

		return;
	}

	int normalStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_MAGIC_STONE_ITEM_ID );
	int specialStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( SPECIAL_MAGIC_STONE_ITEM_ID );

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );

	//{{ kimhc // 2010-01-06 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
	if ( pItem != NULL && 
        pItem->GetItemTemplet() != NULL && 
        pItem->GetItemTemplet()->GetIsPcBang() == true )
	{
		g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
		return;
	}
#endif	PC_BANG_WORK
	//}} kimhc // 2010-01-06 // PC방 프리미엄 서비스

#ifdef ITEM_RECOVERY_TEST
	// 강화 레벨 검사 (사용불능인가?)
	if(true == pItem->IsDisabled())
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_800 ), g_pMain->GetNowState() );
		return; 
	}
#endif //ITEM_RECOVERY_TEST
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	else if( false == pItem->GetIsEvaluation() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_24617 ), g_pMain->GetNowState() );
		return; 
	}			
#endif //SERV_NEW_ITEM_SYSTEM_2013_05


	bool bCheckHaveOption = false;
	if ( pItem != NULL )
	{
		for ( UINT i = 0; i < pItem->GetItemData().m_SocketOption.size(); i++ )
		{
			int socketOption = pItem->GetItemData().m_SocketOption[i];
			if ( socketOption != 0 )
				bCheckHaveOption = true;
		}
	}

	if ( normalStone <= 0 && specialStone <= 0 && bCheckHaveOption == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
			GET_STRING( STR_ID_825 ), g_pMain->GetNowState(),  -1, 3.0f );
		return;
	}

	if ( pItem != NULL && 
        pItem->GetItemTemplet() != NULL
        )
	{
		int slotNum = pItem->GetItemTemplet()->GetSocketSlotNum();
		if ( slotNum == 0 )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
				GET_STRING( STR_ID_826 ), g_pMain->GetNowState(), -1, 3.0f );

			return;
		}

		m_SocketItemUID = pItem->GetUID();

		m_pDLGSocketItem = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Soket_Item.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSocketItem );

		// 위치조절
		// 		m_NowMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
		// 		m_NowMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
		// 		D3DXVECTOR2 DLGPos = g_pKTDXApp->MouseConvertByResolution( m_NowMousePos ) - D3DXVECTOR2(178, 206);		// Left-top pos
		// 		if(DLGPos.x > 664)
		// 			DLGPos.x = 664;
		// 		if(DLGPos.y > 295)
		// 			DLGPos.y = 295;
		// 		m_pDLGSocketItem->SetPos( DLGPos );
		m_pDLGSocketItem->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-664, 768-295), D3DXVECTOR2(178,206)) );


		// 운영자 기능
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
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

		ResetSocketWindow();
	}
}
//}} oasis907 : 김상윤 [2010.4.19] // 
#endif SERV_SOCKET_NEW

#ifdef SERV_SOCKET_NEW
// oasis907 : 김상윤 [2010.4.14] // 
bool CX2UIInventory::OpenNewSocketWindow( CX2SlotItem* pSlot )
{
	//{{ oasis907 : 김상윤 [2010.4.5] //  예외 처리 
	if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( pSlot->GetItemTID() ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
			GET_STRING( STR_ID_824 ), g_pMain->GetNowState() , -1, 3.0f );

		return false;
	}

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );

	//{{ kimhc // 2010-01-06 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
	if ( pItem != NULL && 
        pItem->GetItemTemplet() != NULL && 
        pItem->GetItemTemplet()->GetIsPcBang() == true )
	{
		g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4839 ),g_pMain->GetNowState() );
		return false;
	}
#endif	PC_BANG_WORK
	//}} kimhc // 2010-01-06 // PC방 프리미엄 서비스

#ifdef ITEM_RECOVERY_TEST
	// 강화 레벨 검사 (사용불능인가?)
	if(true == pItem->IsDisabled())
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_800 ), g_pMain->GetNowState() );
		return false; 
	}
#endif //ITEM_RECOVERY_TEST
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 
	else if( false == pItem->GetIsEvaluation() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_24617 ), g_pMain->GetNowState() );
		return false; 
	}			
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

	// kimhc // 2010-10-19 // low 등급의 아이템 마법석 장착 제한
	if ( CX2Item::IG_LOW == pItem->GetItemTemplet()->GetItemGrade() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_9088 ), g_pMain->GetNowState() );
		return false; 
	}

	SAFE_DELETE( m_pUISocketItem );
	m_pUISocketItem = new CX2UISocketItem( g_pMain->GetNowState(), NULL );

	m_pUISocketItem->ResetSocketWindow(pItem->GetUID());

	return true;
}
// oasis907 : 김상윤 [2010.4.14] // 
#endif SERV_SOCKET_NEW

#ifdef SERV_PET_SYSTEM
//{{ oasis907 : 김상윤 [2010.9.2] // 펫 인벤토리
void CX2UIInventory::OpenPetInventory()
{

#ifdef ADD_PET_INVENTORY_BUTTON
	if(IsPetInventoryExist() == false)
	{
		return;
	}
#else
	// oasis907 : 김상윤 [2010.9.11] 펫 없는 경우 예외처리 여기서 한번에
	if(g_pData->GetPetManager()->GetMyPet() == NULL)
	{
		return;
	}

	// oasis907 : 김상윤 [2010.9.13] // 알 상태의 펫은 펫 인벤토리 안열리게
	int MyPetID = g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_PetId;
	int EvolutionStep = g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_Evolution_Step;

	int Invenslot_Size = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) MyPetID, EvolutionStep);
	
	if(Invenslot_Size == 0)
	{
		return;
	}
#endif ADD_PET_INVENTORY_BUTTON

	SAFE_DELETE( m_pUIPetInventory );
	m_pUIPetInventory = new CX2UIPetInventory( g_pMain->GetNowState(), m_pDLGUIInventory->GetLayer() );
	m_pUIPetInventory->SetShow(true);
	return;
}
//}}
#endif SERV_PET_SYSTEM


#ifndef SERV_SOCKET_NEW
void CX2UIInventory::ResetSocketWindow()
{
	if ( m_pDLGSocketItem == NULL )
		return;

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID ); 

	if ( pItem == NULL )
		return;

	//아이템 이름하고 아이템 슬롯 이미지 표시
	CKTDGUIStatic* pStaticSlotImage = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_item_Slot_Image" );
	if ( pStaticSlotImage != NULL && pStaticSlotImage->GetPicture(0) != NULL )
	{
        const wchar_t* pwszShopImage = pItem->GetItemTemplet()->GetShopImage();
		pStaticSlotImage->GetPicture(0)->SetTex( pwszShopImage );
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

	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"Static_comment4" );
	if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
	{
		pStaticItemName->GetString(0)->msg = pItem->GetFullName();
	}


	int slotNum = pItem->GetItemTemplet()->GetSocketSlotNum();

	int normalStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_MAGIC_STONE_ITEM_ID );
	int specialStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( SPECIAL_MAGIC_STONE_ITEM_ID );


	CKTDGUIStatic* pStaticMagicStoneNum = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_Normal_Magic_Stone_Num" );
	if ( pStaticMagicStoneNum != NULL && pStaticMagicStoneNum->GetString(0) != NULL )
	{
		wstringstream wstrstm;
		wstrstm << normalStone;
		pStaticMagicStoneNum->GetString(0)->msg = wstrstm.str().c_str();
	}

	CKTDGUIStatic* pStaticRareStoneNum = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_Rare_Magic_Stone_Num" );
	if ( pStaticRareStoneNum != NULL && pStaticRareStoneNum->GetString(0) != NULL )
	{
		wstringstream wstrstm;
		wstrstm << specialStone;
		pStaticRareStoneNum->GetString(0)->msg = wstrstm.str().c_str();
	}


	// 	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
	// 	{
	// 		wstringstream wstrstm;
	// 		wstrstm << L"g_pStatic_Lower_Back_Slot";
	// 		wstrstm << i + 1;
	// 		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
	// 		if ( pControl != NULL )
	// 		{
	// 			if ( slotNum == i + 1 )
	// 			{
	// 				pControl->SetShow( true );
	// 			}
	// 			else
	// 				pControl->SetShow( false );
	// 		}
	// 
	// 	}

	// 초기화
	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
	{
		wstringstream wstrstm;
		wstrstm << L"g_pStatic_Gray_Back_Slot";
		wstrstm << i + 1;
		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			if ( slotNum >= i + 1 )
			{
				pControl->SetShow( true );
			}
			else
				pControl->SetShow( false );
		}
		wstrstm.str(L"");

		wstrstm << L"Static_Empty";
		wstrstm << i + 1;
		pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShow( false );
		}
		wstrstm.str(L"");

		// 	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
		// 	{
		// 		wstringstream wstrstm;
		// 		wstrstm << L"g_pStatic_Jewely_Slot";
		// 		wstrstm << i + 1;
		// 		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		// 		if ( pControl != NULL )
		// 		{
		// 			pControl->SetShow( false );
		// 		}
		// 	}

		wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
		wstrstm << i + 1;
		pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShow( false );
		}
		wstrstm.str(L"");

		wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
		wstrstm << i + 1;
		pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShow( false );
		}
		wstrstm.str(L"");

		wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
		wstrstm << i + 1;
		pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );
		}
		wstrstm.str(L"");

		wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
		wstrstm << i + 1;
		pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );
		}
		wstrstm.str(L"");

		wstrstm << L"g_pButton_Remove_Stone_Slot";
		wstrstm << i + 1;
		pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );
		}
		wstrstm.str(L"");

		wstrstm << L"g_pStatic_Normal_Inactive";
		wstrstm << i + 1;
		pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );
		}
		wstrstm.str(L"");

		wstrstm << L"g_pStatic_Rare_Inactive";
		wstrstm << i + 1;
		pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );
		}
	}



	CKTDGUIStatic* pStatic_LastExtraDamage = NULL;



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

				// 				wstrstm << L"g_pStatic_Jewely_Slot";
				// 				wstrstm << i + 1;
				// 				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
				// 				if ( pControl != NULL )
				// 				{
				// 					pControl->SetShow( true );
				// 				}

				wstrstm.str( L"" );
				wstrstm << L"g_pButton_Remove_Stone_Slot";
				wstrstm << i + 1;
				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( true, true );
				}

				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
				if ( pSocketData != NULL )
				{
					//여기다가.. 소켓 옵션 설명 가져와서 한줄 짜리인지 두줄짜리인지 보고.. 넣자잉..

					bool bCheckTwoLine = false;

					wstring socketDesc = g_pMain->GetStrByLienBreak( pSocketData->GetSocketDesc().c_str(), 175, XUF_DODUM_13_SEMIBOLD );
					if ( socketDesc.find( L"\n") != -1 )
						bCheckTwoLine = true;

					if ( bCheckTwoLine == false )
					{
						wstringstream wstrstm;
						wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
						wstrstm << i + 1;
						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
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
						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
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


				wstringstream wstrstmRemove_Stone_Slot;
				wstrstmRemove_Stone_Slot << L"g_pButton_Remove_Stone_Slot";
				wstrstmRemove_Stone_Slot << i + 1;
				pControl = m_pDLGSocketItem->GetControl( wstrstmRemove_Stone_Slot.str().c_str() );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( true, true );
				}

				//보석 표시
				//설명 보고 원라인으로 할것인지 투라인으로 할것인지?
				//일반 마법석, 희귀 마법석 표시
			}
		}

		if ( bCheckEmpty == true )
		{
			//비어있는것들..
			wstringstream wstrstm;
			wstrstm << L"Static_Empty";
			wstrstm << i + 1;
			CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
			if ( pControl != NULL )
			{
				pControl->SetShow( true );
			}

			if ( normalStone > 0 )
			{
				wstringstream wstrstm;
				wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
				wstrstm << i + 1;
				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( true, true );
				}
			}
			else
			{
				wstringstream wstrstm;
				wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
				wstrstm << i + 1;
				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( false, false );
				}

				wstringstream wstrstmInactive;
				wstrstmInactive << L"g_pStatic_Normal_Inactive";
				wstrstmInactive << i + 1;
				CKTDGUIControl* pControlInactive = m_pDLGSocketItem->GetControl( wstrstmInactive.str().c_str() );
				if ( pControlInactive != NULL )
				{
					pControlInactive->SetShow( true );
				}

			}

			if ( specialStone > 0 )
			{
				wstringstream wstrstm;
				wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
				wstrstm << i + 1;
				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( true, true );
				}
			}	
			else
			{
				wstringstream wstrstm;
				wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
				wstrstm << i + 1;
				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( false, false );
				}

				wstringstream wstrstmInactive;
				wstrstmInactive << L"g_pStatic_Rare_Inactive";
				wstrstmInactive << i + 1;
				CKTDGUIControl* pControlInactive = m_pDLGSocketItem->GetControl( wstrstmInactive.str().c_str() );
				if ( pControlInactive != NULL )
				{
					pControlInactive->SetShow( true );
				}
			}

		}
	}
}


bool CX2UIInventory::Handler_EGS_SOCKET_ITEM_REQ( int socketIndex, bool bIsRareMagicStone, bool bIsRemoveSocketOption )
{
	KEGS_SOCKET_ITEM_REQ kPacket;

	kPacket.m_iItemUID = m_SocketItemUID;
	kPacket.m_iSocketIndex = socketIndex;
	kPacket.m_bIsRareMagicStone = bIsRareMagicStone;
	kPacket.m_bIsRemoveSocketOption = bIsRemoveSocketOption;
	kPacket.m_bDebug = false;
	kPacket.m_sDebugSocketOption = 0;

	if ( bIsRemoveSocketOption == false )
	{
		if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket");
			if ( pCheckBox != NULL )
			{
				if ( pCheckBox->GetChecked() == true )
				{
					kPacket.m_bDebug = true;

					//kPacket.m_sDebugSocketOption = (short)
					CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*)m_pDLGSocketItemCheat->GetControl(L"IME_EditJoinPassword");
					if ( pEditBox != NULL )
					{
						int socketOption = _wtoi( pEditBox->GetText() );
						kPacket.m_sDebugSocketOption = (short)socketOption;
					}

				}
			}
		}
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_SOCKET_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SOCKET_ITEM_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_SOCKET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SOCKET_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	CloseSocketCheatWindow();

	if ( g_pMain->DeleteServerPacket( EGS_SOCKET_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			vector< int > vecOrgSocketOption;
			bool bCheckNewElementOption = false;

			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEvent.m_iSocketItemUID );
			if ( pItem != NULL )
			{
				vecOrgSocketOption = pItem->GetItemData().m_SocketOption;
			}

			//특수 처리 고고싱
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEvent.m_iSocketItemUID );
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

			//ResetCharRoomEDnCashUInVP();
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}

			ResetSocketWindow();

			return true;
		}		
	}

	return false;
}
#endif SERV_SOCKET_NEW // ifndef SERV_SOCKET_NEW

//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
bool CX2UIInventory::Handler_EGS_SORT_CATEGORY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SORT_CATEGORY_ITEM_ACK kEGS_SORT_CATEGORY_ITEM_ACK;
	DeSerialize( pBuff, &kEGS_SORT_CATEGORY_ITEM_ACK );

	if( g_pMain->DeleteServerPacket( EGS_SORT_CATEGORY_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEGS_SORT_CATEGORY_ITEM_ACK.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateCategorySlotList((CX2Inventory::SORT_TYPE)kEGS_SORT_CATEGORY_ITEM_ACK.m_iCategoryType, kEGS_SORT_CATEGORY_ITEM_ACK.m_vecUpdatedInventorySlot);
			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
			return true;
		}
	}

	return false;
}
#endif SERV_SORT_CATEGORY_ITEM 
//}}

#ifdef ITEM_RECOVERY_TEST
void CX2UIInventory::GetRequiredArmorOnlyRecoveryStoneID( IN const int iItemLevel_, OUT vector<int>& vecRecoveryItemIdList_ )
{
	if( iItemLevel_ <= 20 )
	{
		vecRecoveryItemIdList_.push_back( 130545 );	// 복원석 lv1
	}
	else if( iItemLevel_ <= 30 )
	{
		vecRecoveryItemIdList_.push_back( 130546 );	// 복원석 lv2
	}
	else if( iItemLevel_ <= 40 )
	{
		vecRecoveryItemIdList_.push_back( 130547 );	// 복원석 lv3
	}
	else if( iItemLevel_ <= 50 )
	{
		vecRecoveryItemIdList_.push_back( 130548 );	// 복원석 lv4
	}
	else if ( iItemLevel_ <= 60 )
	{
		vecRecoveryItemIdList_.push_back( 160027 ); // 복원석 lv5
	}
	else if ( iItemLevel_ <= 70 )
	{
		vecRecoveryItemIdList_.push_back( 160028 ); // 복원석 lv6
	}
	else if ( iItemLevel_ <= 80 )
	{
		vecRecoveryItemIdList_.push_back( 160029 ); // 복원석 lv7
	}
	else if ( iItemLevel_ <= 90 )
	{
		vecRecoveryItemIdList_.push_back( 160030 ); // 복원석 lv8
	}
}

// 아이템 레벨에 따라 필요한 아이템 복원석의 ID
void CX2UIInventory::GetRequiredRecoveryStoneID( IN const int iItemLevel_, OUT vector<int>& vecRecoveryItemIdList_ )
{
	if( iItemLevel_ <= 20 )
	{
		vecRecoveryItemIdList_.push_back( 206880 );	// 복원석 lv1
#ifdef CHILDRENS_DAY_EVENT_ITEM
		vecRecoveryItemIdList_.push_back( 160790 ); // 이벤트 : [코보]축복받은 복원의 주문서 Lv.1
#endif //CHILDRENS_DAY_EVENT_ITEM
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 85002096 );	// 복원석 lv1
		vecRecoveryItemIdList_.push_back( 70001910 );	// 복원석(이벤트) lv1 JP
		vecRecoveryItemIdList_.push_back( 83000150 );	// 2012.07.24 lygan_조성욱 // 중국 전용 아리엘의 축복받은 복원석 lv1
		vecRecoveryItemIdList_.push_back( 83000300 );	// 루리엘의 복줌 lv1
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else if( iItemLevel_ <= 30 )
	{
		vecRecoveryItemIdList_.push_back( 206890 );	// 복원석 lv2
#ifdef CHILDRENS_DAY_EVENT_ITEM
		vecRecoveryItemIdList_.push_back( 160791 ); // 이벤트 : [코보]축복받은 복원의 주문서 Lv.2
#endif //CHILDRENS_DAY_EVENT_ITEM
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 85002097 );	// 복원석 lv1
		vecRecoveryItemIdList_.push_back( 70002801 );	// 복원석(이벤트) lv2 JP
		vecRecoveryItemIdList_.push_back( 83000301 );	// 루리엘의 복줌 lv2
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else if( iItemLevel_ <= 40 )
	{
		vecRecoveryItemIdList_.push_back( 206900 );	// 복원석 lv3
#ifdef CHILDRENS_DAY_EVENT_ITEM
		vecRecoveryItemIdList_.push_back( 160792 ); // 이벤트 : [코보]축복받은 복원의 주문서 Lv.3
#endif //CHILDRENS_DAY_EVENT_ITEM
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 85002098 );	// 복원석 lv1
		vecRecoveryItemIdList_.push_back( 70002802 );	// 복원석(이벤트) lv3 JP
		vecRecoveryItemIdList_.push_back( 83000152 );	// 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 축복받은 복원석 lv3
		vecRecoveryItemIdList_.push_back( 83000302 );	// 루리엘의 복줌 lv3
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else if( iItemLevel_ <= 50 )
	{
		// 순서 중요합니다.
		vecRecoveryItemIdList_.push_back( 206910 );	// 복원석 lv4
		vecRecoveryItemIdList_.push_back( 160370 );	// 복원석(이벤트) lv4
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 85002099 );	// 복원석 lv1
		vecRecoveryItemIdList_.push_back( 70002803 );	// 복원석(이벤트) lv4 JP		
		vecRecoveryItemIdList_.push_back( 83000153 );	// 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 축복받은 복원석 lv4
		vecRecoveryItemIdList_.push_back( 83000303 );	// 루리엘의 복줌 lv4
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else if( iItemLevel_ <= 60 )
	{
		// 순서 중요합니다.
		vecRecoveryItemIdList_.push_back( 206920 );	// 복원석 lv5
		vecRecoveryItemIdList_.push_back( 160371 );	// 복원석(이벤트) lv5
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 85002100 );	// 복원석 lv1
		vecRecoveryItemIdList_.push_back( 70002804 );	// 복원석(이벤트) lv5 JP		
		vecRecoveryItemIdList_.push_back( 83000154 );	// 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 축복받은 복원석 lv5
		vecRecoveryItemIdList_.push_back( 83000304 );	// 루리엘의 복줌 lv5
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else if( iItemLevel_ <= 70 )
	{
		vecRecoveryItemIdList_.push_back( 206930 );	// 복원석 lv6
#ifdef CHILDRENS_DAY_EVENT_ITEM
		vecRecoveryItemIdList_.push_back( 160793 ); // 이벤트 : [코보]축복받은 복원의 주문서 Lv.6
#endif //CHILDRENS_DAY_EVENT_ITEM
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 67006183 );	// 아리엘의 축복받은 복원의 주문서 Lv.6
		vecRecoveryItemIdList_.push_back( 70002805 );	// 복원석(이벤트) lv6 JP
		vecRecoveryItemIdList_.push_back( 83000155 );	// 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 축복받은 복원석 lv6
		vecRecoveryItemIdList_.push_back( 83000305 );	// 루리엘의 복줌 lv6
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else if( iItemLevel_ <= 80 )
	{
		vecRecoveryItemIdList_.push_back( 206940 );	// 복원석 lv7
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 70002806 );	// 복원석(이벤트) lv7
		vecRecoveryItemIdList_.push_back( 83000156 );	// 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 축복받은 복원석 lv7
		vecRecoveryItemIdList_.push_back( 83000306 );	// 루리엘의 복줌 lv7
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else if( iItemLevel_ <= 90 )
	{
		vecRecoveryItemIdList_.push_back( 206950 );	// 복원석 lv8
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 70002807 );	// 복원석(이벤트) lv8
		vecRecoveryItemIdList_.push_back( 83000157 );	// 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 축복받은 복원석 lv8
		vecRecoveryItemIdList_.push_back( 83000307 );	// 루리엘의 복줌 lv8
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else if( iItemLevel_ <= 100 )
	{
		vecRecoveryItemIdList_.push_back( 206960 );	// 복원석 lv9
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		vecRecoveryItemIdList_.push_back( 70002808 );	// 복원석(이벤트) lv9
		vecRecoveryItemIdList_.push_back( 83000158 );	// 2012.10.17 lygan_조성욱 // 중국 전용 아리엘의 축복받은 복원석 lv9
		vecRecoveryItemIdList_.push_back( 83000308 );	// 루리엘의 복줌 lv9
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
	}
	else
	{
		ASSERT( !"invalid item level" );
	}

}

void CX2UIInventory::CheckRequiredRecoveryStoneInInventory( IN OUT vector<int>& vecRecoveryItemIdList_ )
{
	if ( false == vecRecoveryItemIdList_.empty() )
	{
		const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
		vector<int>::const_iterator vItr = vecRecoveryItemIdList_.begin();
		
		do 
		{
			if ( 0 == kInventory.GetNumItemByTID( *vItr ) )
				vItr = vecRecoveryItemIdList_.erase( vItr );
			else
				++vItr;

		} while ( vItr != vecRecoveryItemIdList_.end() );
	}	
}

bool CX2UIInventory::Handler_EGS_RESTORE_ITEM_REQ( UidType RecoveryItemUid )
{
	// 일단 아이템 있는지 다시 확인해주고
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( RecoveryItemUid );
	if(pItem == NULL) 
		return false;

	KEGS_RESTORE_ITEM_REQ kEGS_RESTORE_ITEM_REQ;

	kEGS_RESTORE_ITEM_REQ.m_iItemUID = RecoveryItemUid;

	g_pData->GetServerProtocol()->SendPacket( EGS_RESTORE_ITEM_REQ, kEGS_RESTORE_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_RESTORE_ITEM_ACK );

	return true;

}

bool CX2UIInventory::Handler_EGS_RESTORE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESTORE_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_RESTORE_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

#ifdef JUNK_AVATAR
			if( CX2User::XUAL_DEV != g_pData->GetMyUser()->GetAuthLevel() )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_828 ), g_pMain->GetNowState() );
			}
#else
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_828 ), g_pMain->GetNowState() );
#endif

			return true;
		}		
	}

	return false;
}

#endif ITEM_RECOVERY_TEST

#ifdef AUTH_DELETE_ITEM

//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
bool CX2UIInventory::Handler_EGS_DELETE_ITEM_REQ( UidType DeleteItemUid, int Quantity )
{
	// 일단 아이템 있는지 다시 확인해주고
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( DeleteItemUid );
	if(pItem == NULL) 
		return false;
	
	// 수량성인 경우 버리려는 갯수가 가지고 있는 것보다 많으면
	if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_QUANTITY &&
		pItem->GetItemData().m_Quantity < m_iItemEnrollNum ) 
	{
		return false;
	}

	KEGS_DELETE_ITEM_REQ kEGS_DELETE_ITEM_REQ;

	kEGS_DELETE_ITEM_REQ.m_iItemUID = DeleteItemUid;
	kEGS_DELETE_ITEM_REQ.m_iQuantity = Quantity;

	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_ITEM_REQ, kEGS_DELETE_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_DELETE_ITEM_ACK );

	return true;


}
#else REAL_TIME_ELSWORD
bool CX2UIInventory::Handler_EGS_DELETE_ITEM_REQ( UidType DeleteItemUid )
{
	if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel() )
	{
		// 일단 아이템 있는지 다시 확인해주고
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( DeleteItemUid );
		if(pItem == NULL) 
			return false;

		KEGS_DELETE_ITEM_REQ kEGS_DELETE_ITEM_REQ;

		kEGS_DELETE_ITEM_REQ.m_iItemUID = DeleteItemUid;

		g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_ITEM_REQ, kEGS_DELETE_ITEM_REQ );
		g_pMain->AddServerPacket( EGS_DELETE_ITEM_ACK );

		return true;
	}

	return false;

}
#endif REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

bool CX2UIInventory::Handler_EGS_DELETE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_DELETE_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			std::vector< KInventoryItemInfo > veckInventoryItemInfo;
			veckInventoryItemInfo.push_back(kEvent.m_kInventoryItemInfo);

			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( veckInventoryItemInfo );

			UpdateInventorySlotList( veckInventoryItemInfo );

#ifdef SERV_EPIC_QUEST
			if ( g_pData->GetUIManager() == NULL ||
				g_pData->GetUIManager()->GetUIQuestNew() == NULL )
				return true;

			if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST_NEW ) == true )
			{
				g_pData->GetUIManager()->GetUIQuestNew()->ResetQuestUI();
			}

			g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
#else
			if ( g_pData->GetUIManager() == NULL ||
				g_pData->GetUIManager()->GetUIQuest() == NULL )
				return true;

			if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST ) == true )
			{
				g_pData->GetUIManager()->GetUIQuest()->UpdateQuestUI();
			}

			if ( g_pData->GetUIManager()->GetUIQuest()->GetShowQuickQuestDLG() == true )
			{
				g_pData->GetUIManager()->GetUIQuest()->UpdateQuickQuestDLG();
			}
#endif SERV_EPIC_QUEST
			return true;
		}		
	}

	return false;

}
#endif AUTH_DELETE_ITEM


#ifndef SERV_SOCKET_NEW
//{{ oasis907 : 김상윤 [2010.4.19] // 
void CX2UIInventory::OpenSocketCheatWindow()
{
	CloseSocketCheatWindow();
	m_pDLGSocketItemCheat = new CKTDGUIDialog( g_pMain->GetNowState() , L"DLG_UI_Num_Input.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSocketItemCheat );
	CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGSocketItemCheat->GetControl( L"check" );

	if ( pButton != NULL )
	{
		pButton->SetCustomMsgMouseUp( (int)CX2UIInventory::UIM_SOCKET_FEE_OK );
	}

	pButton = (CKTDGUIButton*)m_pDLGSocketItemCheat->GetControl( L"cancle" );
	if ( pButton != NULL )
	{
		pButton->SetShow( false );
		pButton->SetEnable( false );
	}

	CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*)m_pDLGSocketItemCheat->GetControl( L"IME_EditJoinPassword" );
	if ( pEditBox != NULL )
	{
		pEditBox->SetCustomMsgEnter( (int)CX2UIInventory::UIM_SOCKET_FEE_OK );
	}
}

void CX2UIInventory::CloseSocketCheatWindow()
{
	if ( m_pDLGSocketItemCheat != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSocketItemCheat, NULL, false );

	m_pDLGSocketItemCheat = NULL;
}
//}}
#endif SERV_SOCKET_NEW




std::wstring CX2UIInventory::GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem )
{
	std::wstring slotItemDesc = L"";

	if ( m_bShowRBDownMessage == true )
	{
		// 개인상점이 열려있으면 이 아이템을 개인상점에 등록할 수 있다고 하자
		if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
			g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_WAIT )
		{
			slotItemDesc += L"\n";
            slotItemDesc += GET_STRING( STR_ID_830 );
            return slotItemDesc;
        }
#ifdef SERV_PSHOP_AGENCY
		// 대리상점이 열려있으면 이 아이템을 개인상점에 등록할 수 있다고 하자
		if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
			g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_WAIT )
		{
			slotItemDesc += L"\n";
			slotItemDesc += GET_STRING( STR_ID_830 );
			return slotItemDesc;
		}
#endif
		// 개인거래창이 열려있으면 (상동)
		else if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE))
		{
			slotItemDesc += L"\n";
            slotItemDesc += GET_STRING( STR_ID_831 );
            return slotItemDesc;
        }

		// 상점이나 제조창이 열려있으면 아무것도 안한다
		// 		else if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SHOP) || true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_MANUFACTURE) )
		// 		{
		//             return slotItemDesc;
		// 		}

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

				    //{{ kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
    #ifdef	SEAL_ITEM
				    if ( pItem->GetItemData().m_bIsSealed == true )
				    {
					    slotItemDesc += GET_STRING( STR_ID_4422 );
				    }
				    else
    #endif	SEAL_ITEM
				    //}} kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
                    slotItemDesc += GET_STRING( STR_ID_19 );
                }
                break;
            }

#ifdef SERV_GOLD_TICKET_SEAL_COUNT
			if(IsGoldTicket(pkItemTemplet->GetItemID()) == true)
			{
				slotItemDesc += L"\n";

				//{{ kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
#ifdef	SEAL_ITEM
				if ( pItem->GetItemData().m_bIsSealed == true )
				{
					slotItemDesc += GET_STRING( STR_ID_4422 );
				}
				else
#endif	SEAL_ITEM
					//}} kimhc // 2009-08-26 // 아이템 봉인 시 디스크립션 변경
					slotItemDesc += GET_STRING( STR_ID_19 );
			}
#endif SERV_GOLD_TICKET_SEAL_COUNT
		}
	}

	return slotItemDesc;
}

// bAbsolute가 true면 화면상 절대좌표. false면 원래 인벤토리 위치를 기준으로 한 상대 좌표.
void CX2UIInventory::SetPosition(D3DXVECTOR2 vec, bool bAbsolute )
{
	// 일단 원위치로 복귀시킨 후 이동시키는 수법을 쓰기로 하자

	// 원위치 복귀
	if(m_pDLGUIInventory != NULL) m_pDLGUIInventory->SetPos( m_DLGPosition );

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		D3DXVECTOR2 pos;
		pos = pItemSlot->GetPos();
		pos -= m_MovedPosition;
		pItemSlot->SetPos(pos);
	}

	m_MovedPosition = D3DXVECTOR2(0, 0);

	// 이제 목표한 지점으로 이동시킨다

	m_MovedPosition = vec;
	if( bAbsolute ) m_MovedPosition = m_MovedPosition - m_DLGPosition;

	if(m_pDLGUIInventory != NULL) m_pDLGUIInventory->SetPos( m_MovedPosition );
	// 살짝 최적화 : 0,0이면 입 닦자 -_-
	if( m_MovedPosition != D3DXVECTOR2(0, 0) )
	{
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			D3DXVECTOR2 pos;
			pos = pItemSlot->GetPos();
			pos += m_MovedPosition;
			pItemSlot->SetPos(pos);
		}
	}
}

bool CX2UIInventory::GetOverMouseInven()
{
	if( GetShow() == true && m_pDLGUIInventory != NULL && m_pDLGUIInventory->GetIsMouseOver() == true )
		return true;
	return false;		
}

#ifdef ATTRACTION_ITEM_TEST

void CX2UIInventory::UpdateOpenAttractionItemAck()
{
	m_eRandomItemEventType = RIOET_NONE;

	// 예전 창을 지우고..
	if ( m_pDLGOpenAttraction != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOpenAttraction, NULL, false );
	m_pDLGOpenAttraction = NULL;

	// 새로 열려라
	m_pDLGAttractionResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Charm_Item_Result.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttractionResult );

#ifdef FIX_ICE_HEATER_EVENT		/// 현재 가열기에 맞는 결과 타이틀 설정

#ifdef UNIQUENESS_EU_ONLY
	const wstring wstrResultDialogTitle		= 
		NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrResultDialogTitleTexture : L"DLG_UI_Title02_A.tga";
#else //UNIQUENESS_EU_ONLY
	const wstring wstrResultDialogTitle		= 
	#ifdef CLIENT_COUNTRY_US
		NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrResultDialogTitleTexture : L"DLG_UI_Title02_B.tga";
	#else //CLIENT_COUNTRY_US
		NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrResultDialogTitleTexture : L"DLG_UI_Title02.tga";
	#endif //CLIENT_COUNTRY_US
#endif 	//UNIQUENESS_EU_ONLY

	const wstring wstrResultDialogTitleKey	= 
		NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrResultDialogTitleTextureKey : L"heater_title";


	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGAttractionResult->GetControl( L"Static_Title" ) );
	if( NULL != pStatic )
	{
		if( NULL != pStatic->GetPicture(0) )
		{
			pStatic->GetPicture(0)->SetTex( wstrResultDialogTitle.c_str(), wstrResultDialogTitleKey.c_str() );
			pStatic->GetPicture(0)->SetSizeAsTextureSize();		// 해외팀 코드 추가
		}
		if( NULL != pStatic->GetPicture(1) )
			pStatic->GetPicture(1)->SetShow(false);
	}

	/// 다 사용했으니, 정보 객체 해제
	m_pNowIceHeaterEventData = NULL;

#else FIX_ICE_HEATER_EVENT

#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	if( true == m_bIsAllInOne )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGAttractionResult->GetControl( L"Static_Title" ) );
		if( NULL != pStatic )
		{
			if( NULL != pStatic->GetPicture(0) )
				pStatic->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_MarriageSystem_02.tga", L"Bg_Title_AllInOne_2" );
			if( NULL != pStatic->GetPicture(1) )
				pStatic->GetPicture(1)->SetShow(false);
		}
		m_bIsAllInOne = false;
	}
#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
#ifdef RIDING_SYSTEM
	if( true == m_bIsRidingGacha )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGAttractionResult->GetControl( L"Static_Title" ) );
		if( NULL != pStatic )
		{
			if( NULL != pStatic->GetPicture(0) )
				pStatic->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_RidingPet_01.tga", L"riding_heater_title_result" );
			if( NULL != pStatic->GetPicture(1) )
				pStatic->GetPicture(1)->SetShow(false);
		}
		m_bIsRidingGacha = false;
	}
#endif //RIDING_SYSTEM

#endif FIX_ICE_HEATER_EVENT

	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
	{
		// 그냥 아이템 + 보너스 아이템 두개만 나온다고 가정하고..
		std::map< int, int >::iterator i;

		int nCount = 0;
		wstring getExplanation = L"";
		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
		{
			int resultItemID = i->first;
			int resultNum	= i->second;
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
			if ( pItemTemplet == NULL ) continue;
			
			CKTDGUIStatic* pStatic = NULL;
			if(resultItemID == ATTRACTION_BONUS_ITEM_ID
#ifdef SERV_BATTLEFIELD_COOKIE_PIECE
				|| resultItemID == RID_BATTLEFIELD_COOKIE
				|| resultItemID == RID_BATTLEFIELD_GOLD_COOKIE
#endif SERV_BATTLEFIELD_COOKIE_PIECE			
#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
				|| resultItemID == ALL_IN_ONE_ATTRACTION_BONUS_ITEM_ID
#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
#ifdef RIDING_SYSTEM
				|| resultItemID == ANCIENT_FOSSIL_PIECE
#endif //RIDING_SYSTEM
				)
			{

				// 보너스 아이템
				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Bonus" );
				pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_832, "Si", pItemTemplet->GetFullName_(), resultNum ) );
			}
			else
			{
				// 그냥 아이템
				WCHAR buf[256] = {0};
				wstring itemName = L"";
				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Item" );
                itemName = pItemTemplet->GetFullName_();

				if( resultNum == 1)
				{
                    pStatic->GetString(0)->msg = itemName;
				}
				else
				{
                    pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_832, "Li", itemName, resultNum ) );
				}
			}

			if( NULL != pStatic && 
				false == pStatic->GetPicture(0)->SetTex( 
                pItemTemplet->GetShopImage()
                ) )
			{
				pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

		}
	}

	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
}

#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD		/// 미니 얼음 조각상 가열기 처리를 위해, 오픈 이벤트 타입 추가
void CX2UIInventory::PlayGetAttractionItem( RANDOM_ITEM_OPEN_EVENT_TYPE eRandomItemEventType /*= RIOET_ICE_HEATER*/ )
#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
void CX2UIInventory::PlayGetAttractionItem() 
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
{
	if ( m_bPlayAttractionItem == false )
	{
		m_bPlayAttractionItem = true;

		m_pDLGGetAttractionItemBG = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Unit_State_RandomItem_BG.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGetAttractionItemBG );
		m_pDLGGetAttractionItemBG->SetColor( D3DXCOLOR( 0,0,0,0) );
		m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 0,0,0,0.7f), 0.5f, true );

		D3DXVECTOR3 boxPos = m_pDLGOpenAttraction->GetDummyPos( 0 );
		if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
		{
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
			if( NULL != m_pDLGOpenAttraction )
			{
				m_pDLGOpenAttraction->SetHasUnit( NULL );
			}
		}

#ifdef FIX_ICE_HEATER_EVENT 

		const wstring wstrPlayMeshPlayerName = 
			NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrPlayMeshPlayerName : L"FireMachineStart";

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachineStart = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL, wstrPlayMeshPlayerName.c_str(), 
			boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );

#else // FIX_ICE_HEATER_EVENT

	#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
		wstring strMeltMachineName = L"FireMachineStart";	/// 디폴트 리소스는 보통 얼음 조각상 가열기

		switch( eRandomItemEventType )
		{
			case RIOET_MINI_ICE_HEATER:		/// 미니 얼음 조각상 가열기는 다른 리소스를 사용
			{
				strMeltMachineName = L"MiniFireMachineStart";
			} break;
	#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
			case RIOET_ALL_IN_ONE_ICE_HEATER:
			{
				strMeltMachineName = L"AllInOneFireMachineStart";
			}break;
	#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	#ifdef RIDING_SYSTEM
			case RIOET_RIDING_PET_GACHA:
			{
				strMeltMachineName = L"AncinetFossilRidingPetStart";
			}break;
	#endif //RIDING_SYSTEM
		}


		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachineStart = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL, strMeltMachineName.c_str(), boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
	#else SERV_SERV_MINI_RANDOM_CUBE_REWARD
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachineStart = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"FireMachineStart", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
	#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD

#endif // FIX_ICE_HEATER_EVENT

        if ( pMeshInst_MeltMachineStart != NULL )
        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            pMeshInst_MeltMachineStart->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		    g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst_MeltMachineStart);
		    if( NULL != m_pDLGOpenAttraction )
		    {
			    m_pDLGOpenAttraction->SetHasUnit(pMeshInst_MeltMachineStart);
			
		    }
		    m_hMeshInstMeltMachineStart = pMeshInst_MeltMachineStart->GetHandle();
        }

#ifdef FIX_ICE_HEATER_EVENT
		/// 가열기 재생 사운드 설정
		const wstring pSoundName = 
			NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrIceHeaterPlaySound : L"Fire_Machine_Start.ogg";

			g_pKTDXApp->GetDeviceManager()->PlaySound( pSoundName.c_str(), false, false );

#else // FIX_ICE_HEATER_EVENT

	#ifdef RIDING_SYSTEM
		if( RIOET_RIDING_PET_GACHA == eRandomItemEventType )
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"AncientFossilMachine_Start.ogg", false, false );
		else
	#endif //RIDING_SYSTEM
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Fire_Machine_Start.ogg", false, false );

#endif // FIX_ICE_HEATER_EVENT


		// 		m_hSeqKeyTrace = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"KeyTrace", m_pMeshInstRandomBoxKey->GetPos() );
		// 		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqKeyTrace );
		// 		if( NULL != pSeq )
		// 		{
		// 			pSeq->SetOverUI( true );
		// 		}
		// 		else
		// 		{
		// 			m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
		// 		}

	}
}

void CX2UIInventory::PlayGetAttractionItemGold() 
{
	if ( m_bPlayAttractionItem == false )
	{
		m_bPlayAttractionItem = true;

		m_pDLGGetAttractionItemBG = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Unit_State_RandomItem_BG.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGetAttractionItemBG );
		m_pDLGGetAttractionItemBG->SetColor( D3DXCOLOR( 0,0,0,0) );
		m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 0,0,0,0.7f), 0.5f, true );

		D3DXVECTOR3 boxPos = m_pDLGOpenAttraction->GetDummyPos( 0 );
		if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
		{
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
			if( NULL != m_pDLGOpenAttraction )
			{
				m_pDLGOpenAttraction->SetHasUnit( NULL );
			}
		}

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachineStart = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"GoldFireMachineStart", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
        if ( pMeshInst_MeltMachineStart != NULL )
        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            pMeshInst_MeltMachineStart->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		    g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst_MeltMachineStart);
		    if( NULL != m_pDLGOpenAttraction )
		    {
			    m_pDLGOpenAttraction->SetHasUnit(pMeshInst_MeltMachineStart);

		    }
		    m_hMeshInstMeltMachineStart = pMeshInst_MeltMachineStart->GetHandle();
        }

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Fire_Machine_Start.ogg", false, false );


		// 		m_hSeqKeyTrace = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"KeyTrace", m_pMeshInstRandomBoxKey->GetPos() );
		// 		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqKeyTrace );
		// 		if( NULL != pSeq )
		// 		{
		// 			pSeq->SetOverUI( true );
		// 		}
		// 		else
		// 		{
		// 			m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
		// 		}

	}
}


bool CX2UIInventory::Handler_EGS_ENCHANT_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID )
{
	// 일단 아이템 있는지 다시 확인해주고
	CX2Item* pCouponItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( CouponItemUID );
	CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( TargetItemUID );

	// 사용가능한지도 확인한다
	if ( pCouponItem == NULL || pTargetItem == NULL ||
		pCouponItem->GetItemTemplet() == NULL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_833 ), g_pMain->GetNowState() );
		return false;
	}
	// 혹시 방어구 아닌 곳에 방어구 전용이 발리지 않나?
	if( pTargetItem->GetItemTemplet()->GetItemType() != CX2Item::IT_DEFENCE &&
		true == IsDefenceOnlyCoupon( 
        pCouponItem->GetItemTemplet()->GetItemID()
        ) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2564 ), g_pMain->GetNowState() );
		return false;
	}
#ifdef ADDED_ENCHANT_COUPON_HAVE_LEVEL_LIMIT    // 김종훈 / 13-07-03 / 레벨 제한을 가진 강화의 부적
	if ( true == CheckLevelIfLevelLimitEnchantCoupon 
		( pCouponItem->GetItemTemplet()->GetItemID(), pTargetItem->GetItemTemplet()->GetUseLevel() ) ) 
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_25399 ), g_pMain->GetNowState() );
		return false;
	}
#endif // ADDED_ENCHANT_ITEM_HAVE_LEVEL_LIMIT // 김종훈 / 13-07-03 / 레벨 제한을 가진 강화의 부적	

	if( IsEnchantCouponUseable( 	
		pCouponItem->GetItemTemplet()->GetItemID(), 
		pTargetItem->GetItemData().m_EnchantLevel ) )
	
	{
		KEGS_ENCHANT_ATTACH_ITEM_REQ kEGS_ATTACH_ITEM_REQ;

		kEGS_ATTACH_ITEM_REQ.m_iAttachItemUID = CouponItemUID;
		kEGS_ATTACH_ITEM_REQ.m_iDestItemUID = TargetItemUID;

		g_pData->GetServerProtocol()->SendPacket( EGS_ENCHANT_ATTACH_ITEM_REQ, kEGS_ATTACH_ITEM_REQ );
		g_pMain->AddServerPacket( EGS_ENCHANT_ATTACH_ITEM_ACK );

		return true;
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_834 ), g_pMain->GetNowState() );
	}

	return false;
}
bool CX2UIInventory::Handler_EGS_ENCHANT_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ENCHANT_ATTACH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_ENCHANT_ATTACH_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_835 ), g_pMain->GetNowState() );

			return true;
		}		
	}

	return false;

}

#ifdef SERV_ATTRIBUTE_CHARM

bool CX2UIInventory::Handler_EGS_ATTRIB_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID )
{
	// 일단 아이템 있는지 다시 확인해주고
	CX2Item* pCouponItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( CouponItemUID );
	CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( TargetItemUID );

	// 사용가능한지도 확인한다
	if ( pCouponItem == NULL || pTargetItem == NULL ||
		pCouponItem->GetItemTemplet() == NULL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_833 ), g_pMain->GetNowState() );
		return false;
	}

	KEGS_ATTRIB_ATTACH_ITEM_REQ kEGS_ATTRIB_ATTACH_ITEM_REQ;

	kEGS_ATTRIB_ATTACH_ITEM_REQ.m_iAttachItemUID = CouponItemUID;
	kEGS_ATTRIB_ATTACH_ITEM_REQ.m_iDestItemUID = TargetItemUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_ATTRIB_ATTACH_ITEM_REQ, kEGS_ATTRIB_ATTACH_ITEM_REQ );
	g_pMain->AddServerPacket( EGS_ATTRIB_ATTACH_ITEM_ACK );

	return true;

}
bool CX2UIInventory::Handler_EGS_ATTRIB_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ATTRIB_ATTACH_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_ATTRIB_ATTACH_ITEM_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_835 ), g_pMain->GetNowState() );
			InvalidSlotDesc();
			return true;
		}		
	}

	return false;

}
#endif SERV_ATTRIBUTE_CHARM


bool CX2UIInventory::IsEnchantCouponUseable(int CouponItemID, int TargetEnchantLevel )
{
	int CouponLevel = 0;

#ifdef INT_ENCHANT_COUPON_ITEM
	for(int i=0; i<ARRAY_SIZE(INT_ENCHANT_COUPON_ITEM_ID); ++i)
	{
		if( CouponItemID == INT_ENCHANT_COUPON_ITEM_ID[i] )
		{
			return true; //해외팀 강화아이템이면 먼저 검사한다.
			break;
		}
	}
#endif //INT_ENCHANT_COUPON_ITEM

	for(int i=0; i<ARRAY_SIZE(ENCHANT_COUPON_ITEM_ID); ++i)
	{
		if( CouponItemID == ENCHANT_COUPON_ITEM_ID[i] )
		{
			CouponLevel = i;
			break;
		}
	}
	if( CouponLevel == 0 )
	{
		for(int i=0; i<ARRAY_SIZE(DEFENCE_ENCHANT_COUPON_ITEM_ID); ++i)
		{
			if( CouponItemID == DEFENCE_ENCHANT_COUPON_ITEM_ID[i] )
			{
				CouponLevel = i;
				break;
			}
		}
	}

	//{{ kimhc // 2010.6.7 // 거래가 불가능한 강화권 추가
	if ( CouponLevel == 0 )
	{
		for ( UINT i = 0; i < ENCHANT_COUPON_ITEM_TO_NOT_TRADE_ID.size(); i++ )
		{
			if ( CouponItemID == ENCHANT_COUPON_ITEM_TO_NOT_TRADE_ID[i] )
			{
				CouponLevel = i;
				break;
			}
		}
	}

#ifdef ADDED_ENCHANT_COUPON_HAVE_LEVEL_LIMIT						// 김종훈 / 13-07-03 / 레벨 제한을 가진 강화의 부적
	if ( CouponLevel == 0 )
	{
		for ( UINT i = 0; i < ARRAY_SIZE(HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_ID); i++ )
		{
			if ( CouponItemID == HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_ID[i] )
			{
				CouponLevel = HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_ENCHANT_VALUE[i];
				// 쿠폰 레벨을 배열에서 얻어옴
				break;
			}
		}
	}
#endif // 김종훈 / 13-07-03 / 레벨 제한을 가진 강화의 부적
	//}} kimhc // 2010.6.7 // 거래가 불가능한 강화권 추가

// 	switch(CouponItemID)
// 	{
// 	case 130147:	// 5
// 	case 130543:	// 5 방어구전용
// 		CouponLevel = 5;
// 		break;
// 	case 130148:	// 6
// 	case 130544:	// 6 방어구전용
// 		CouponLevel = 6;
// 		break;
// 	case 130149:	// 7
// 		CouponLevel = 7;
// 		break;
// 	case 130150:	// 8
// 		CouponLevel = 8;
// 		break;
// 	case 130151:	// 9
// 		CouponLevel = 9;
// 		break;
// 	case 130152:	// 10
// 		CouponLevel = 10;
// 		break;
// 	default:
// 		return false;
// 	}

	if(CouponLevel > TargetEnchantLevel) return true;
	return false;
}
#endif

#ifdef APRIL_5TH_TREE_DAY_EVENT			// 식목일 이벤트 관련 코드
void CX2UIInventory::UpdateOpenTreeDayItemAck()
{
	m_eRandomItemEventType = RIOET_NONE;

	// 이전에 만들어져있던 Attraction Item의 결과창을 재활용한다. 
	// 메시지 (완료시의 CustomMsg)등도 가열기 결과창의 그것을 따른다.
	// 제목이랑 보너스 아이템 스태틱만 안 보이게 지워주면 될 듯.


	// 예전 창을 지우고..
	if ( m_pDLGOpenAttraction != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOpenAttraction, NULL, false );
	m_pDLGOpenAttraction = NULL;

	// 새로 열려라
	m_pDLGAttractionResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Charm_Item_Result_1slot.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttractionResult );

	// "얼음 조각상 가열기 사용 결과" 스태틱 안 보이게 해주고
	CKTDGUIStatic* pStatic_Title = (CKTDGUIStatic*)m_pDLGAttractionResult->GetControl( L"Static_Title" );
	pStatic_Title->SetShow(false);

	// 보너스 아이템 스태틱 안 보이게 해주고
	CKTDGUIStatic* pStatic_BonusMark = (CKTDGUIStatic*)m_pDLGAttractionResult->GetControl( L"Static_Bonus_Item_Mark" );
	pStatic_BonusMark->SetShow(false);

	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
	{
		// 식목일 이벤트 아이템은 최대 2개 나온다 ( 기획팀에서 확인한 사항임 )
		std::map< int, int >::iterator i;

		bool bFirstItem = true;
		wstring getExplanation = L"";
		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
		{
			int resultItemID = i->first;
			int resultNum	= i->second;
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
			if ( pItemTemplet == NULL ) continue;

			CKTDGUIStatic* pStatic = NULL;
			if(bFirstItem)
			{
				// 첫 번째 아이템
				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Item" );
                const wchar_t* pwszitemName = pItemTemplet->GetFullName_();

				if( resultNum == 1)
				{
                    pStatic->GetString(0)->msg = pwszitemName;
                }
				else
				{
                    pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_832, "Si", pwszitemName, resultNum ) );
				}
				bFirstItem = false;
			}
			else
			{
				// 두번째
				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Bonus" );
                const wchar_t* pwszitemName = pItemTemplet->GetFullName_();
                pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_832, "Si", pwszitemName, resultNum ) );
			}

			if( NULL != pStatic && 
				NULL != pStatic->GetPicture(0) &&
				false == pStatic->GetPicture(0)->SetTex( 
                pItemTemplet->GetShopImage()
                ) )
			{
				pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

		}
	}

	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );

}

bool CX2UIInventory::IsAprilEventRandomItem( int ItemID )
{
#ifdef RANDOM_CUBE_UI_NEW
	//{{ oasis907 : 김상윤 [2011.3.29] 랜덤 아이템 큐브에서 식목일 아이템 처리
	return false;
	//}}
#else
	switch(ItemID)
	{
// 	case 130523:		// 경험의 묘목 ItemID
// 	case 130524:		// 용기의 묘목 ItemID
// 	case 130525:		// 마력의 묘목 ItemID
	case 131859:		// 2010년 식목일 묘목
		return true;
	default:
		break;
	}
	return false;
#endif RANDOM_CUBE_UI_NEW
}

#endif

#ifdef SERV_TREASURE_BOX_ITEM			// 트레져 박스 추가
void CX2UIInventory::UpdateOpenTreasureBoxItemAck()
{
	m_eRandomItemEventType = RIOET_NONE;

	// 이전에 만들어져있던 Attraction Item의 결과창을 재활용한다. 
	// 메시지 (완료시의 CustomMsg)등도 가열기 결과창의 그것을 따른다.
	// 제목이랑 보너스 아이템 스태틱만 안 보이게 지워주면 될 듯.


	// 예전 창을 지우고..
	if ( m_pDLGOpenAttraction != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOpenAttraction, NULL, false );
	m_pDLGOpenAttraction = NULL;

	// 새로 열려라
	m_pDLGAttractionResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Charm_Item_Result_1slot.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttractionResult );

	// "얼음 조각상 가열기 사용 결과" 스태틱 안 보이게 해주고
	CKTDGUIStatic* pStatic_Title = (CKTDGUIStatic*)m_pDLGAttractionResult->GetControl( L"Static_Title" );
	pStatic_Title->SetShow(false);

	// 보너스 아이템 스태틱 안 보이게 해주고
	CKTDGUIStatic* pStatic_BonusMark = (CKTDGUIStatic*)m_pDLGAttractionResult->GetControl( L"Static_Bonus_Item_Mark" );
	pStatic_BonusMark->SetShow(false);

	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
	{

		std::map< int, int >::iterator i;

		bool bFirstItem = true;
		wstring getExplanation = L"";
		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
		{
			int resultItemID = i->first;
			int resultNum	= i->second;
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
			if ( pItemTemplet == NULL ) continue;

			CKTDGUIStatic* pStatic = NULL;
			if(bFirstItem)
			{
				// 첫 번째 아이템
				wstring itemName = L"";
				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Item" );
				itemName = pItemTemplet->GetFullName_();

				if( resultNum == 1)
				{
					pStatic->GetString(0)->msg = itemName;
				}
				else
				{
					pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_832, "Li", itemName, resultNum ) );
				}
				bFirstItem = false;
			}
			else
			{
				wstring itemName = L"";
				// 두번째
				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Bonus" );

				itemName = pItemTemplet->GetFullName_();
				pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_832, "Li", itemName, resultNum ) );
			}

			if( NULL != pStatic && 
				NULL != pStatic->GetPicture(0) &&
				false == pStatic->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() ) )
			{
				pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}
		}
	}

	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );

}

bool CX2UIInventory::IsTreasureBoxRandomItem( int ItemID )
{
	switch(ItemID)
	{
	case 70000065:		// 트레저박스 (엘소드) ItemID
	case 70000066:		// 트레저박스 (아이샤) ItemID
	case 70000067:		// 트레저박스 (레나) ItemID
	case 70000068:		// 트레저박스 (레이븐) ItemID
	case 70000069:		// 트레저박스 (이브) ItemID
	case 70001800:		// 트레저박스 (라셰) ItemID
#ifdef SERV_TREASURE_BOX_EVENT
	case 67006709:		// 트레저박스 (엘리시스) ItemID
	case 67006710:		// 트레저박스 (엘리시스) ItemID
	case 70006020:		// 트레저박스 (아라) ItemID
	case 70005650:
	case 67004670:
	case 250000260:
	case 67005029:		// 골드 트래져 박스(엘소드):TYPE1
	case 67005030:		// 골드 트래져 박스(아이샤):TYPE1
	case 67005031:		// 골드 트래져 박스(레나):TYPE1
	case 67005032:		// 골드 트래져 박스(레이븐):TYPE1
	case 67005033:		// 골드 트래져 박스(이브):TYPE1
	case 67005034:		// 골드 트래져 박스(라셰):TYPE1
	case 67005035:		// 골드 트래져 박스(엘소드):TYPE2
	case 67005036:		// 골드 트래져 박스(아이샤):TYPE2
	case 67005037:		// 골드 트래져 박스(레나):TYPE2
	case 67005038:		// 골드 트래져 박스(레이븐):TYPE2
	case 67005039:		// 골드 트래져 박스(이브):TYPE2
	case 67005040:		// 골드 트래져 박스(라셰):TYPE2
	case 67005380:		// 유카타 트레져 박스(청)
	case 70007190:		// 아라 런칭기념 트레저 박스
	case 67006285:		// 달나라에서 가져온 트레져 박스
#endif SERV_TREASURE_BOX_EVENT
#ifdef TREASURE_BOX_ITEM_THIN
	case 67004400:
#endif TREASURE_BOX_ITEM_THIN
#ifdef TREASURE_BOX_ITEM_TW
	case 75000000:// 열혈큐브
	case 60001806:
#endif TREASURE_BOX_ITEM_TW
		return true;
	default:
		break;
	}
	return false;
}
#endif SERV_TREASURE_BOX_ITEM			// 트레져 박스 추가

// kimhc // 2009-08-12 // 몬스터 카드 셋트인지?
bool CX2UIInventory::IsMonsterCardSetItem( int iItemID )
{
	switch( iItemID )
	{
	case MONSTER_CARD_SET_RUBEN:
	case MONSTER_CARD_SET_ELDER:
	case MONSTER_CARD_SET_BESMA:
	case MONSTER_CARD_SET_PEITA:
	case MONSTER_CARD_SET_ALTERA:
	case MONSTER_CARD_SET_SECRET:
	case MONSTER_CARD_SET_RUBEN_GOLD_EDITION:
	case MONSTER_CARD_SET_ELDER_GOLD_EDITION:
	case MONSTER_CARD_SET_BESMA_GOLD_EDITION:
	case MONSTER_CARD_SET_PEITA_GOLD_EDITION:
	case MONSTER_CARD_SET_ALTERA_GOLD_EDITION:
	case MONSTER_CARD_SET_SECRET_GOLD_EDITION:
	case MONSTER_CARD_SET_ELIOS:
	case MONSTER_CARD_SET_ELIOS_GOLD_EDITION:
	case MONSTER_CARD_SET_ELIOS_ALL_EDITION:
		return true;
		break;

	default:
		return false;
		break;	
	}
	return false;
}

#ifdef ATTRACTION_ITEM_TEST
bool CX2UIInventory::IsAttachItem( int ItemID )
{
	for(int i=0; i<ARRAY_SIZE(ATTACH_ITEM_ITEM_ID); ++i)
	{
		if( ItemID == ATTACH_ITEM_ITEM_ID[i] )
			return true;
	}
// 	switch(ItemID)
// 	{
// 	case 130147:		// 강화권 +5
// 	case 130148:
// 	case 130149:
// 	case 130150:
// 	case 130151:
// 	case 130152:		// +10
// 
// 	case 130543: // 방어구 전용 강화의 부적 Lv.5
// 	case 130544: // 방어구 전용 강화의 부적 Lv.6
// 		{
// 			return true;
// 		} break;
// 	default:
// 		return false;
// 	} 
	return false;
}

bool CX2UIInventory::IsDefenceOnlyCoupon( int ItemID )
{
	for(int i=5; i<ARRAY_SIZE(DEFENCE_ENCHANT_COUPON_ITEM_ID); ++i)
	{
		if( ItemID == DEFENCE_ENCHANT_COUPON_ITEM_ID[i] )
			return true;
	}
// 	switch(ItemID)
// 	{
// 	case 130543: // 방어구 전용 강화의 부적 Lv.5
// 	case 130544: // 방어구 전용 강화의 부적 Lv.6
// 		return true;
// 	default:
// 		return false;
// 	}
	return false;
}

#ifdef SERV_ATTRIBUTE_CHARM
bool CX2UIInventory::IsAttributeItem( int ItemID )
{
	for(int i=0; i<ARRAY_SIZE(ATTRIBUTE_CHARM_COUPON_ITEM_ID); ++i)
	{
		if( ItemID == ATTRIBUTE_CHARM_COUPON_ITEM_ID[i] )
			return true;
	}
	return false;
}

wstring CX2UIInventory::GetAttributeItemExtraDamageName( int itemTID )
{
	wstring enchantDesc = L"";
	CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType = CX2DamageManager::EDT_NONE;
	switch(itemTID)
	{
	case 133365:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER;		break;
	case 133366:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER;		break;
	case 133367:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_POISON_MASTER;		break;
	case 133368:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_MASTER_PIERCING;	break;
	case 133369:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_MASTER_SHOCK;		break;
	case 133370:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_MASTER_SNATCH;		break;
	case 133371:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING;		break;
	case 133372:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK;		break;
	case 133373:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH;		break;
	case 133374:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_FROZEN_PIERCING;	break;
	case 133375:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_FROZEN_SHOCK;		break;
	case 133376:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_FROZEN_SNATCH;		break;
	case 133377:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_POISON_PIERCING;	break;
	case 133378:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_POISON_SHOCK;		break;
	case 133379:
		extraDamageType = CX2DamageManager::EDT_ENCHANT_POISON_SNATCH;		break;
	default:
	 	extraDamageType = CX2DamageManager::EDT_NONE;
	}
	enchantDesc = g_pData->GetDamageManager()->GetExtraDamageName( extraDamageType );
	return enchantDesc;
}

#endif SERV_ATTRIBUTE_CHARM

#endif ATTRACTION_ITEM_TEST

void CX2UIInventory::UpdateUIButton()
{
	if( m_bShow == false || m_pDLGUIInventory == NULL )
		return;

#ifndef SERV_UPGRADE_TRADE_SYSTEM // 김태환			/// 이제 개인 상점 버튼 열 일이 없다.
	//{{ kimhc // 실시간 엘소드 중 던전내에서 아이템 소켓, 상점 개설 버튼 비활성화
	// 상점을 못 여는 상황이면
	CKTDGUIButton* pShopButton		= static_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"shop_open" ) );

	if ( g_pMain->GetNowStateID() != CX2Main::XS_SQUARE_GAME || g_pSquareGame == NULL )
	{
		pShopButton->SetShowEnable(false, false);
	}
#ifdef SERV_SOCKET_NEW
	// oasis907 : 김상윤 [2010.4.6] // 
	else if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true)
	{
		pShopButton->SetShowEnable( false, false );
	}
#endif SERV_SOCKET_NEW
	else
	{
		pShopButton->SetShowEnable(true, true);
	}
#endif //SERV_UPGRADE_TRADE_SYSTEM
	

#ifdef SERV_SKILL_NOTE
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		if( g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot() > 0 )
		{
#ifdef SERV_SOCKET_NEW
			if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true)
			{
#ifdef UI_SOCKET_NEW_FIX01
				HideSkillNote(false);
				SetEnableSkillNote(false);
#else
				HideSkillNote(true));
#endif UI_SOCKET_NEW_FIX01
			}
			else
			{
#endif SERV_SOCKET_NEW
				// 기술의 노트 사용가능
				HideSkillNote(false);
				SetEnableSkillNote(true);

#ifdef SERV_SOCKET_NEW
			}
#endif SERV_SOCKET_NEW
#ifdef REFORM_SKILL_NOTE_UI // 기술의 노트 버튼 제거하지 않고 비활성화로 변경
			SetEnableBuySkillNote( false );
#endif // REFORM_SKILL_NOTE_UI
		}
		else
		{
#ifdef REFORM_SKILL_NOTE_UI // 기술의 노트 버튼 제거하지 않고 비활성화로 변경
			HideSkillNote(false);
			SetEnableSkillNote(false);
			SetEnableBuySkillNote( true );
#else
			// 기술의 노트 사용불가
			HideSkillNote(true);
#endif // REFORM_SKILL_NOTE_UI

		}
	}
#else
	CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGUIInventory->GetControl(L"Noactive");
	if( pStatic != NULL )
	{
		pStatic->GetPictureIndex(1)->SetShow(false);
	}

	CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGUIInventory->GetControl(L"_Note");
	if( pButton != NULL )
	{
		pButton->SetShowEnable(false, false);
	}
#endif

#ifdef REAL_TIME_ELSWORD
	CKTDGUIButton* pSocketButtton	= static_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"soket" ) );

	if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME || 
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME ||
		g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE) == true
#ifdef SERV_MULTI_RESOLVE
		|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true
#endif SERV_MULTI_RESOLVE
#ifdef SERV_SYNTHESIS_AVATAR
		|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true
#endif SERV_SYNTHESIS_AVATAR
		)
	{
		pSocketButtton->SetShowEnable( false, false );
	}
#ifdef SERV_SOCKET_NEW
	// oasis907 : 김상윤 [2010.4.6] // 
	else if((m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true) || g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) == true)
	{
		pSocketButtton->SetShowEnable( false, false );
	}
#endif SERV_SOCKET_NEW
	else
	{
		pSocketButtton->SetShowEnable( true, true );
	}
#endif REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 던전내에서 아이템 소켓, 상점 개설 버튼 비활성화

	// 개인상점 열려있으면 분해/버리기 막음
	CKTDGUIButton* pTrashButtton	= static_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"trash" ) );
	if(pTrashButtton != NULL)
	{
		if( ( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
			( g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL 
#ifdef SERV_PSHOP_AGENCY
			|| g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_WAIT 
#endif
			) ) ||
			( g_pMain != NULL && g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetOpendSendWindow() == true) ||
			( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE) == true ) )
		{
			pTrashButtton->SetShowEnable(true, false);

#ifdef SERV_SKILL_NOTE
			SetEnableSkillNote(false);
#endif
#ifdef SERV_PET_SYSTEM
			SetEnableFeedPet( false );
#endif
#ifdef REFORM_SKILL_NOTE_UI // 기술의 노트 버튼 제거하지 않고 비활성화로 변경
			SetEnableBuySkillNote( false );
#endif // REFORM_SKILL_NOTE_UI 
		}
#ifdef SERV_SOCKET_NEW
		// oasis907 : 김상윤 [2010.4.6] // 
		else if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true
#ifdef SERV_MULTI_RESOLVE
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true
#endif SERV_MULTI_RESOLVE		
#ifdef SERV_SYNTHESIS_AVATAR
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true
#endif SERV_SYNTHESIS_AVATAR			
			)
		{
			pTrashButtton->SetShowEnable( true, false );

#ifdef SERV_PET_SYSTEM
			SetEnableFeedPet( false );
#endif
		}
#endif SERV_SOCKET_NEW
		else
		{
			pTrashButtton->SetShowEnable(true, true);

#ifdef SERV_SKILL_NOTE
	#ifdef REFORM_SKILL_NOTE_UI // 기술의 노트 버튼 제거하지 않고 비활성화로 변경
			if( g_pData != NULL && 
				g_pData->GetMyUser() != NULL && 
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				if( g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot() > 0 )
				{
					SetEnableSkillNote(true);
					SetEnableBuySkillNote( false );
				}
			}
	#else
			SetEnableSkillNote(true);
	#endif // REFORM_SKILL_NOTE_UI
#endif
#ifdef SERV_PET_SYSTEM
			SetEnableFeedPet( true );
#endif
		}
	}

	CKTDGUIButton* pResolveButtton	= static_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"resolve" ) );	
	if(pResolveButtton != NULL)
	{
		if( (true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
			( g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL 
#ifdef SERV_PSHOP_AGENCY
			|| g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_WAIT 
#endif
			) ) ||
			( g_pMain != NULL && g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetOpendSendWindow() == true) ||
			( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE) == true) )
		{
			pResolveButtton->SetShowEnable(false, false);
		}
#ifdef SERV_SOCKET_NEW
		// oasis907 : 김상윤 [2010.4.6] // 
		else if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true
#ifdef SERV_MULTI_RESOLVE
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true
#endif SERV_MULTI_RESOLVE
#ifdef SERV_SYNTHESIS_AVATAR
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true
#endif SERV_SYNTHESIS_AVATAR
			)
		{
			pResolveButtton->SetShowEnable( false, false );
		}
#endif SERV_SOCKET_NEW
		else
		{
			pResolveButtton->SetShowEnable(true, true);
		}
	}



#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL && g_pData->GetPetManager()->GetMyPet() != NULL )
	{
		if( ( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) &&
			( g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL 
#ifdef SERV_PSHOP_AGENCY
			|| g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_WAIT 
#endif
			) ) ||
			( g_pMain != NULL && g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetOpendSendWindow() == true) ||
			( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE) == true ) ||
			( m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true ) 
#ifdef SERV_MULTI_RESOLVE
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true
#endif SERV_MULTI_RESOLVE
#ifdef SERV_SYNTHESIS_AVATAR
			|| ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true )
#endif SERV_SYNTHESIS_AVATAR			
			)
		{
			SetEnableFeedPet( false );
		}
		else
		{
			SetEnableFeedPet( true );
		}
		
	}
	else
	{
		SetEnableFeedPet( false );
	}

#ifdef SERV_SYNTHESIS_AVATAR // SERV_MULTI_RESOLVE 인벤 정렬 버튼
	CKTDGUIButton* pPCButtton	= static_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"Inventory_Menu_PC" ) );	
	CKTDGUIButton* pSortButtton	= static_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"Button_SortItem" ) );	
	if(pPCButtton != NULL)
	{
		if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true 
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true	
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true
			)
		{
			pPCButtton->SetShowEnable(true, false);
		}
		else
		{
			pPCButtton->SetShowEnable(true, true);
		}
	}

	if(pSortButtton != NULL)
	{
		if(m_pUISocketItem != NULL && m_pUISocketItem->GetShow() == true 
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_RESOLVE ) == true		
			|| g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SYNTHESIS ) == true
			)
		{
			pSortButtton->SetShowEnable(true, false);
		}
		else
		{
			pSortButtton->SetShowEnable(true, true);
		}
	}
#endif SERV_SYNTHESIS_AVATAR // SERV_MULTI_RESOLVE

#ifdef ADD_PET_INVENTORY_BUTTON
	UpdateButtonPetInventory();
#endif ADD_PET_INVENTORY_BUTTON

#endif

#ifdef GOOD_ELSWORD //JHKang
	CKTDGUIButton* pCashShopButton = NULL;
	CKTDGUIButton* pEDButton = NULL;

	pCashShopButton = reinterpret_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"g_pButton_Upgrade_Inven" ) );

	if ( pCashShopButton == NULL )
	{
		ASSERT( !"Cashshop Button error" );
		return;
	}
#ifndef NO_GOOD_ELSWORD_INT
	pEDButton = reinterpret_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"g_pButton_Upgrade_Inven_ED" ) );

	if ( pEDButton == NULL )
	{
		ASSERT( !"ED Button error" );
		return;
	}
#endif NO_GOOD_ELSWORD_INT
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();


	if ( kInventory.IsAbleToExpandSlot() == true && 
		 ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP || g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD ) )
	{
		pCashShopButton->SetShowEnable( true, true );
#ifndef NO_GOOD_ELSWORD_INT
		pEDButton->SetShowEnable( true, true );
#endif NO_GOOD_ELSWORD_INT
		pCashShopButton->SetGuideDesc( GET_STRING( STR_ID_28665 ) );
		pCashShopButton->SetGuideDescOffsetPos( D3DXVECTOR2( 100, 0 ) );
#ifndef NO_GOOD_ELSWORD_INT
		pEDButton->SetGuideDesc( GET_STRING( STR_ID_28666 ) );
		pEDButton->SetGuideDescOffsetPos( D3DXVECTOR2( 100, 0 ) );
#endif NO_GOOD_ELSWORD_INT
	}
	else
	{
		pCashShopButton->SetShowEnable( false, false );
#ifndef NO_GOOD_ELSWORD_INT
		pEDButton->SetShowEnable( false, false );
#endif NO_GOOD_ELSWORD_INT
	}

#endif //GOOD_ELSWORD


	// 운영자 기능들
	if( CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel() )
	{
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGUIInventory->GetControl( L"CheckBox_Auth" );
		pCheckBox->SetChecked(false);
		pCheckBox->SetShow(true);
		CKTDGUIButton* pButton_AllSell = (CKTDGUIButton*)m_pDLGUIInventory->GetControl( L"Admin_SellAll" );
		pButton_AllSell->SetShow(true);

#ifndef CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
		CKTDGUIButton* pButton_Delete = (CKTDGUIButton*)m_pDLGUIInventory->GetControl( L"Admin_Delete" );
		pButton_Delete->SetShow(true);
#endif //CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
	}
#ifdef CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
	else if( CX2User::XUAL_OPERATOR == g_pData->GetMyUser()->GetAuthLevel() )
	{
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGUIInventory->GetControl( L"CheckBox_Auth" );
		pCheckBox->SetChecked(false);
		pCheckBox->SetShow(true);

		CKTDGUIButton* pButton_AllSell = (CKTDGUIButton*)m_pDLGUIInventory->GetControl( L"Admin_SellAll" );
		pButton_AllSell->SetShow(true);
	}
#endif // CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
	else
	{
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGUIInventory->GetControl( L"CheckBox_Auth" );
		pCheckBox->SetChecked(false);
		pCheckBox->SetShow(false);	
		pCheckBox->SetEnable(false);
		CKTDGUIButton* pButton_AllSell = (CKTDGUIButton*)m_pDLGUIInventory->GetControl( L"Admin_SellAll" );
		pButton_AllSell->SetShowEnable(false, false);
#ifndef CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
		CKTDGUIButton* pButton_Delete = (CKTDGUIButton*)m_pDLGUIInventory->GetControl( L"Admin_Delete" );
		pButton_Delete->SetShowEnable(false, false);
#endif // CUBE_AUTO_OPEN_AND_ALL_ITEM_DELETE_UI_FOR_GM
	}


}

#endif


#ifdef FREE_WARP
void CX2UIInventory::AddWarpList(CKTDGUIContolList *pControlList, const int row, const int iVillageId)
{
	if( pControlList == NULL )
		return;

	int iCurrentVillageID = -1;
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:		
	case CX2Main::XS_SQUARE_GAME:
		{
			iCurrentVillageID = static_cast<int>(g_pData->GetLocationManager()->GetCurrentVillageID());
		} break;
	case CX2Main::XS_BATTLE_FIELD:
		{
			iCurrentVillageID = g_pData->GetBattleFieldManager().GetReturnVillageId();
		} break;
	default:
		break;
	}
	
	if( iVillageId == iCurrentVillageID )
	{
		m_iCurrentVillageWarpIndex = row;
	}


	WCHAR wVillageId[256] = {0,};
	//wsprintf(wVillageId, L"Village_%d", iVillageId);
	StringCchPrintf(wVillageId, 256, L"Village_%d", iVillageId);
	// 마을리스트 추가
	CKTDGUIStatic *pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(wVillageId);
	pControl->SetShow(true);
	pControlList->InsertItem( pControl, row, 0, false );

	if( m_pDLGWarpDestination != NULL )
	{
		m_pDLGWarpDestination->OpenScriptFile(L"DLG_Destination_Control.lua");

		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGWarpDestination->GetControl( L"ListControl_Button" );
		//wsprintf(wVillageId, L"Button_%d", iVillageId);
		StringCchPrintf(wVillageId, 256, L"Button_%d", iVillageId);
		pButton->SetName( wVillageId );
		pButton->AddDummyInt(iVillageId);
		pControlList->InsertItem( pButton, row, 1 );
		pButton->SetShowEnable(true, true);

		pButton->SetDownStateAtNormal(false);

		m_vecWarpListButton.push_back( pButton );
	}
}

void CX2UIInventory::InitWarpList()
{
	CKTDGUIStatic *pControl = NULL;
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20000");	//VMI_RUBEN
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20001");	//VMI_ELDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1000");	//VMI_BATTLE_FIELD_ELDER_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20002");	//VMI_BESMA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1001");	//VMI_BATTLE_FIELD_BESMA_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20003");	//VMI_ALTERA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1002");	//VMI_BATTLE_FIELD_ALTERA_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20005");	//VMI_PEITA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20004");	//VMI_VELDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1004");	//VMI_BATTLE_FIELD_VELDER_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20006");	//VMI_HAMEL
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1005");	//VMI_BATTLE_FIELD_HAMEL_REST_00
	pControl->SetShow(false);
#ifdef VILLAGE_SANDER	
#ifndef NO_SANDER_VILLIAGE
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20007");	//VMI_SANDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1006");	//VMI_BATTLE_FIELD_SANDER_REST_00
	pControl->SetShow(false);
#endif NO_SANDER_VILLIAGE
#endif //VILLAGE_SANDER
}

void CX2UIInventory::CreateWarpDest()
{	
	m_vecWarpListButton.clear();
	SAFE_DELETE_DIALOG( m_pDLGWarpDestination );

	if( m_pDLGWarpDestination == NULL )
	{
		m_pDLGWarpDestination = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Destination.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWarpDestination );

	}

	if( m_pDLGWarpDestination != NULL )
	{		
		m_pDLGWarpDestination->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(300, 400), D3DXVECTOR2(75,20)) );
		m_pDLGWarpDestination->SetShowEnable(true, true);

		CKTDGUIContolList *pControlList = (CKTDGUIContolList*)m_pDLGWarpDestination->GetControl(L"WarpDestControlList");
		if( pControlList != NULL )
		{				
			CKTDGUIStatic *pControl = NULL;						
			CX2LocationManager::LocalMapTemplet* pLocalMapTemplet = NULL;
			
			int row = 0;
			InitWarpList();
			// 루벤지역
			{
			// 루벤마을
				pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20000");	//VMI_RUBEN
				pControl->SetShow(false);
				AddWarpList( pControlList, row, SEnum::VMI_RUBEN );
				++row;
			}			

			// 엘더지역
			const int iMyUnitLevel = g_pData->GetSelectUnitLevel();
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_ELDER );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// 엘더마을
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20001");	//VMI_ELDER
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_ELDER );
					++row;

					// 엘더 휴식처(월리성 관문)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1000");	//VMI_BATTLE_FIELD_ELDER_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_ELDER_REST_00 );
					++row;
				}
			}

			// 베스마지역
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_BESMA );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// 베스마마을
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20002");	//VMI_BESMA
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BESMA );
					++row;

					// 베스마 휴식처(아슬아슬 하늘길)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1001");	//VMI_BATTLE_FIELD_BESMA_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_BESMA_REST_00 );
					++row;
				}
			}

			// 알테라지역
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_ALTERA_ISLAND );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// 알테라마을
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20003");	//VMI_ALTERA
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_ALTERA );
					++row;

					// 알테라 휴식처(풍고족 은신처)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1002");	//VMI_BATTLE_FIELD_ALTERA_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00 );
					++row;
				}
			}

			// 페이타지역
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_PEITA );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// 페이타(파견 텐트)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20005");	//VMI_PEITA
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_PEITA );
					++row;
				}
			}

			// 벨더지역
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_VELDER );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// 벨더마을
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20004");	//VMI_VELDER
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_VELDER );
					++row;

					// 벨더 휴식처 (시계탑 광장)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1004");	//VMI_BATTLE_FIELD_VELDER_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_VELDER_REST_00 );
					++row;
				}
			}

			//{{ JHKang / 강정훈 / 2011/01/11 / 하멜 추가
#ifdef ADD_HAMEL_VILLAGE
			// 하멜지역
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_HAMEL );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// 수도 하멜
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20006");	//VMI_HAMEL
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_HAMEL );
					++row;

					// 하멜 휴식처(치유의 샘터)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1005");	//VMI_BATTLE_FIELD_HAMEL_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00 );
					++row;
				}
			}
#endif ADD_HAMEL_VILLAGE
			//}} JHKang / 강정훈 / 2011/01/11 / 하멜 추가

#ifdef VILLAGE_SANDER 
#ifndef NO_SANDER_VILLIAGE
			// 샌더지역
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_SANDER );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// 샌더 마을
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20007");	//VMI_HAMEL
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_SANDER );
					++row;

					// 샌더 휴식처(윈드스톤 유적지)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1006");	//VMI_BATTLE_FIELD_HAMEL_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_SANDER_REST_00 );
					++row;
				}
			}
#endif NO_SANDER_VILLIAGE
#endif //VILLAGE_SANDER
			pControlList->SetIndex(0, 0);				
		}
	}	
}

void CX2UIInventory::SetShowWarpDest(bool val)
{
	if( m_pDLGWarpDestination != NULL )
		m_pDLGWarpDestination->SetShowEnable(val, val);
}

bool CX2UIInventory::GetShowWarpDest()
{
	if( m_pDLGWarpDestination != NULL )
		return m_pDLGWarpDestination->GetShow();

	return false;
}

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
bool CX2UIInventory::AddSecondJobList(CKTDGUIContolList *pControlList, const int row, const CX2Unit::UNIT_CLASS eSecondJobId)
{
	if( pControlList == NULL )
		return false;

	if ( eSecondJobId == g_pData->GetMyUser()->GetSelectUnit()->GetClass() ||
		2 != g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() )
		return false;

	WCHAR wszSecondJobId[256] = {0,};
	//wsprintf(wVillageId, L"Village_%d", iVillageId);
	StringCchPrintf( wszSecondJobId, 256, L"Class_%d", static_cast<int>(eSecondJobId) );
	
	if( m_pDLGUSCJob != NULL )
	{
		// 전직 리스트 추가
		CKTDGUIStatic *pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl( wszSecondJobId );

		if( pControl == NULL )
			return false;

		pControl->SetShow(true);
		pControlList->InsertItem( pControl, row, 0, false );
	
		m_pDLGUSCJob->OpenScriptFile(L"DLG_Change_Job_Control.lua");

		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGUSCJob->GetControl( L"ListControl_Button" );

		if( pButton == NULL )
			return false;

		//wsprintf(wVillageId, L"Button_%d", iVillageId);
		StringCchPrintf( wszSecondJobId, 256, L"Button_%d", static_cast<int>(eSecondJobId) );
		pButton->SetName( wszSecondJobId );
		pButton->AddDummyInt(static_cast<int>(eSecondJobId));
		pControlList->InsertItem( pButton, row, 1 );
		pButton->SetShowEnable(true, true);
		pButton->SetDownStateAtNormal(false);

		m_vecUSCJobButton.push_back( pButton );
	}

	return true;
}

void CX2UIInventory::InitSecondJobList()
{
	CKTDGUIStatic *pControl = NULL;

	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_100");	//VMI_RUBEN
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_101");	//VMI_ELDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_102");	//VMI_BATTLE_FIELD_ELDER_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_103");	//VMI_BESMA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_104");	//VMI_BATTLE_FIELD_BESMA_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_105");	//VMI_ALTERA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_106");	//VMI_BATTLE_FIELD_ALTERA_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_107");	//VMI_PEITA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_108");	//VMI_VELDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_109");	//VMI_BATTLE_FIELD_VELDER_REST_00
	pControl->SetShow(false);

	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_110");	//VMI_HAMEL
	pControl->SetShow(false);

	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_111");	//VMI_BATTLE_FIELD_HAMEL_REST_00
	pControl->SetShow(false);

	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_112");	//VMI_SANDER
	pControl->SetShow(false);

	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_113");	//VMI_BATTLE_FIELD_SANDER_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_114");	//VMI_RUBEN
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_115");	//VMI_ELDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_116");	//VMI_BATTLE_FIELD_ELDER_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_117");	//VMI_BESMA
	pControl->SetShow(false);

	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_118");	//VMI_BATTLE_FIELD_BESMA_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_119");	//VMI_BATTLE_FIELD_BESMA_REST_00
	pControl->SetShow(false);

	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_120");
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_121");
	pControl->SetShow(false);
}

void CX2UIInventory::CreateSecondJobDest()
{	
	m_vecWarpListButton.clear();
	SAFE_DELETE_DIALOG( m_pDLGUSCJob );

	if( m_pDLGUSCJob == NULL )
	{
		if( CX2Unit::UT_ARA == g_pData->GetMyUser()->GetSelectUnit()->GetType())
		{
			m_pDLGUSCJob = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Change_Job_Ara.lua" ); //아라일때는 전직 할 수 있는게 한칸이다.
		}
		else
		{
			m_pDLGUSCJob = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Change_Job.lua" );
		}
		
		if( g_pKTDXApp != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUSCJob );
		}
	}

	if( m_pDLGUSCJob != NULL )
	{		
		m_pDLGUSCJob->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(300, 400), D3DXVECTOR2(75,20)) );
		m_pDLGUSCJob->SetShowEnable(true, true);

		CKTDGUIContolList *pControlList = (CKTDGUIContolList*)m_pDLGUSCJob->GetControl(L"ChangeJobControlList");
		if( pControlList != NULL )
		{				
			CKTDGUIStatic *pControl = NULL;						

			int row = 0;
			InitSecondJobList();
		
			CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
			switch( eUnitType )
			{
			case CX2Unit::UT_ELSWORD:
				{
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_100");	//UC_ELSWORD_LORD_KNIGHT
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ELSWORD_LORD_KNIGHT ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_101");	//UC_ELSWORD_RUNE_SLAYER
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ELSWORD_RUNE_SLAYER ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_112");	//UC_ELSWORD_INFINITY_SWORD
					pControl->SetShow(false);
#ifdef SERV_ELSWORD_INFINITY_SWORD
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ELSWORD_INFINITY_SWORD ) )
						++row;
#endif SERV_ELSWORD_INFINITY_SWORD
				} break;
			case CX2Unit::UT_ARME:
				{
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_104");	//UC_ARME_ELEMENTAL_MASTER
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ARME_ELEMENTAL_MASTER ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_105");	//UC_ARME_VOID_PRINCESS
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ARME_VOID_PRINCESS ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_113");	//UC_ARME_DIMENSION_WITCH
					pControl->SetShow(false);
#ifdef SERV_ARME_DIMENSION_WITCH
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ARME_DIMENSION_WITCH ) )
						++row;
#endif SERV_ARME_DIMENSION_WITCH
				} break;
			case CX2Unit::UT_LIRE:
				{
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_102");	//UC_ARME_ELEMENTAL_MASTER
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_LIRE_WIND_SNEAKER ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_103");	//UC_ARME_VOID_PRINCESS
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_LIRE_GRAND_ARCHER ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_114");	//UC_ARME_DIMENSION_WITCH
					pControl->SetShow(false);
#ifdef SERV_RENA_NIGHT_WATCHER
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_LIRE_NIGHT_WATCHER ) )
						++row;
#endif SERV_RENA_NIGHT_WATCHER
				} break;
			case CX2Unit::UT_RAVEN:
				{
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_106");	//UC_ARME_ELEMENTAL_MASTER
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_RAVEN_BLADE_MASTER ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_107");	//UC_ARME_VOID_PRINCESS
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_RAVEN_RECKLESS_FIST ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_115");	//UC_ARME_DIMENSION_WITCH
					pControl->SetShow(false);
#ifdef SERV_RAVEN_VETERAN_COMMANDER
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_RAVEN_VETERAN_COMMANDER ) )
						++row;
#endif SERV_RAVEN_VETERAN_COMMANDER
				} break;
			case CX2Unit::UT_EVE:
				{
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_108");	//UC_ARME_ELEMENTAL_MASTER
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_EVE_CODE_NEMESIS ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_109");	//UC_ARME_VOID_PRINCESS
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_EVE_CODE_EMPRESS ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_116");	//UC_ARME_DIMENSION_WITCH
					pControl->SetShow(false);
#ifdef SERV_EVE_BATTLE_SERAPH
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_EVE_BATTLE_SERAPH ) )
						++row;
#endif //SERV_EVE_BATTLE_SERAPH
				} break;
			case CX2Unit::UT_CHUNG:
				{
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_110");	//UC_ARME_ELEMENTAL_MASTER
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_CHUNG_IRON_PALADIN ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_111");	//UC_ARME_VOID_PRINCESS
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_CHUNG_DEADLY_CHASER ) )
						++row;

					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_117");	//UC_ARME_DIMENSION_WITCH
					pControl->SetShow(false);
#ifdef SERV_CHUNG_TACTICAL_TROOPER
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_CHUNG_TACTICAL_TROOPER ) )
						++row;
#endif SERV_CHUNG_TACTICAL_TROOPER
				} break;

			case CX2Unit::UT_ARA:
				{
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_118");	//UC_ARME_ELEMENTAL_MASTER
					pControl->SetShow(false);
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ARA_SAKRA_DEVANAM ) )
						++row;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_119");	//UC_ARME_VOID_PRINCESS
					pControl->SetShow(false);
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ARA_YAMA_RAJA ) )
						++row;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
				} break;

			case CX2Unit::UT_ELESIS:
				{
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_120");
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ELESIS_GRAND_MASTER ) )
						++row;
					pControl = (CKTDGUIStatic*)m_pDLGUSCJob->GetControl(L"Class_121");
					pControl->SetShow(false);
					if( AddSecondJobList( pControlList, row, CX2Unit::UC_ELESIS_BLAZING_HEART ) )
						++row;
				} break;
			}
			
			pControlList->SetIndex(0, 0);				
		}
	}	
}

void CX2UIInventory::SetShowSecondJobDest(bool val)
{
	if( m_pDLGUSCJob != NULL )
	{
		m_pDLGUSCJob->SetShowEnable(val, val);
	}
}

bool CX2UIInventory::GetShowSecondJobDest()
{
	if( m_pDLGUSCJob != NULL )
	{
		return m_pDLGUSCJob->GetShow();
	}

	return false;
}
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

bool CX2UIInventory::CheckRadioButtonBySortType( CX2Inventory::SORT_TYPE eType )
{
	CKTDGUIRadioButton* pEquipButton = NULL;
	switch( eType )
	{
	case CX2Inventory::ST_EQUIP:
		{
			pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_Equip" );

		} break;
	case CX2Inventory::ST_ACCESSORY:
		{
			pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_Accessory" );

		} break;
	case CX2Inventory::ST_QUICK_SLOT:
		{
			pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_QuickSlot" );

		} break;
	case CX2Inventory::ST_MATERIAL:
		{
			pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_Material" );

		} break;
	case CX2Inventory::ST_QUEST:
		{
			pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_Quest" );

		} break;
	case CX2Inventory::ST_SPECIAL:
		{
			pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_Special" );
		} break;
	case CX2Inventory::ST_AVARTA:
		{
			pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_Avatar" );
		} break;

		//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
	case CX2Inventory::ST_PC_BANG:
		{
			pEquipButton = (CKTDGUIRadioButton*) m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_Menu_PC" );
		} break;
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스

	default:
		{
			return false;
		} break;
	}

	if ( pEquipButton == NULL )
		return false;

	pEquipButton->SetChecked(true);

	return true;
}

#ifdef SERV_PET_SYSTEM
void CX2UIInventory::FeedPet()
{
	// 내가 dungeon ready상태이면 소환/소환해제/진화 막는다.		
	//if( g_pData->GetPartyManager()->DoIHaveParty() == true )
	//{
	//	bool bMyReady = false;
	//	UidType iUnitUid = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	//	CX2PartyManager::PartyMemberData* pMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iUnitUid );
	//	if( NULL != pMemberData )
	//	{
	//		bMyReady = pMemberData->m_bGameReady;
	//	}
	//	if( bMyReady == true )
	//	{
	//		return;
	//	}
	//}		

	if( g_pData->GetPetManager() != NULL )
	{
#ifdef SERV_PET_SYSTEM_EX1
		CX2PET *pPet = g_pData->GetPetManager()->GetMyPet();
		if( NULL != pPet && true == pPet->GetPetInfo().m_bAlwaysMaxSatiety )
		{
			CX2PetManager::PetTemplet* pTemplet = g_pData->GetPetManager()->GetPetTemplet(CX2PetManager::PUI_PET_MERCENARY_PPORU_EVENT_INT);
			if( NULL != pTemplet )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ),  GET_STRING( STR_ID_26365 ), g_pMain->GetNowState() );			
			}
			return;
		}
#endif //SERV_PET_SYSTEM_EX1

		CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
		CX2Cursor*	pCursor = pState->GetCursor();

		if( pCursor != NULL && pCursor->GetCurorState() == CX2Cursor::XCS_PET_FOOD )
		{
			pCursor->ChangeCursorState(CX2Cursor::XCS_NORMAL);
			return;
		}

		if( pCursor != NULL )
			pCursor->ChangeCursorState( CX2Cursor::XCS_PET_FOOD );

		if( g_pMain->GetInfoBox() == NULL )
			g_pMain->KTDGUIInfoBox( D3DXVECTOR2( 250, 200 ), GET_STRING( STR_ID_5365 ), pState );

		m_pPetFoodSlot = NULL;
	}
}

void CX2UIInventory::SetEnableFeedPet(bool bVal)
{
	if( m_pDLGUIInventory == NULL )
		return;

	CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGUIInventory->GetControl(L"_Food");
	if( pButton != NULL )
	{
		pButton->SetEnable(bVal);
	}
}
#endif

#ifdef SERV_SKILL_NOTE
void CX2UIInventory::HideSkillNote(bool bVal)
{
	if( m_pDLGUIInventory == NULL )
		return;

	CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGUIInventory->GetControl(L"Noactive");
	if( pStatic != NULL )
	{
		pStatic->GetPictureIndex(1)->SetShow(!bVal);
	}

	CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGUIInventory->GetControl(L"_Note");
	if( pButton != NULL )
	{
		pButton->SetShowEnable(!bVal, !bVal);
	}

#ifdef SERV_PET_SYSTEM
	// 기술의노트 버튼이 숨겨졌을 경우 먹이주기 버튼을 이동시킨다.
	bool bMove = false;
	if( pStatic->GetPictureIndex(1)->GetShow() == false )
	{
		bMove = true;
	}

	if( pStatic != NULL )
	{
		D3DXVECTOR2 vPosNote = pStatic->GetPictureIndex(1)->GetOriginalPos();
		D3DXVECTOR2 vPosFood = pStatic->GetPictureIndex(2)->GetOriginalPos();		
		if( bMove == true )
		{
			vPosFood.x = vPosNote.x;
		}
		pStatic->GetPictureIndex(2)->SetPos( vPosFood );
	}

	CKTDGUIButton *pButtonFood = (CKTDGUIButton*)m_pDLGUIInventory->GetControl(L"_Food");
	if( pButtonFood != NULL )
	{
		if( bMove == true )
			pButtonFood->SetOffsetPos( D3DXVECTOR2(-81.f, 0.f) );
		else
			pButtonFood->SetOffsetPos( D3DXVECTOR2(0.f, 0.f) );
	}
		
#endif
}

void CX2UIInventory::SetEnableSkillNote(bool bVal)
{
	if( m_pDLGUIInventory == NULL )
		return;
	
	bool bShow = true;
	CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGUIInventory->GetControl(L"Noactive");
	if( pStatic != NULL )
	{
		bShow = pStatic->GetPictureIndex(1)->GetShow();
	}

	if( bShow == true )
	{
		bShow = bVal;
	}

	CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGUIInventory->GetControl(L"_Note");
	if( pButton != NULL )
	{
		pButton->SetEnable(bShow);
	}

#ifdef SERV_PET_SYSTEM
	if( pStatic->GetPictureIndex(1)->GetShow() == true || pButton->GetShow() == true )
	{
		CKTDGUIButton *pButtonFood = (CKTDGUIButton*)m_pDLGUIInventory->GetControl(L"_Food");
		if( pButtonFood != NULL )
		{
			pButtonFood->SetOffsetPos( D3DXVECTOR2(0.f, 0.f) );
		}		
	}	
#endif
}
#endif

#ifdef ADD_PET_INVENTORY_BUTTON
bool CX2UIInventory::IsPetInventoryExist()
{
	// oasis907 : 김상윤 [2010.9.11] 펫 없는 경우 예외처리 여기서 한번에
	if(g_pData->GetPetManager()->GetMyPet() == NULL)
	{
		return false;
	}

	// oasis907 : 김상윤 [2010.9.13] // 알 상태의 펫은 펫 인벤토리 안열리게
	int MyPetID = g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_PetId;
	int EvolutionStep = g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_Evolution_Step;

	int Invenslot_Size = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) MyPetID, EvolutionStep);

	if(Invenslot_Size == 0)
	{
		return false;
	}
#ifdef SERV_EVENT_PET_INVENTORY
///여기서 펫 ID를 확인 한다. 그래서 펫 ID가 이벤트 ID이고 이벤트 먹이를 먹지 않았다면 인벤토리를 활성화 시켜주지 않는다.
///이벤트 먹이를 먹이는 순간 부터 이 펫은 영구 펫이 되기 때문에 여기서 체크를 계속 해줘야 겠네.
	bool EventFoodPetID = g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_bIsEventPetID;
	if( EventFoodPetID )
	{
		bool EventFoodEat = g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_bEventFoodEat;
		if( EventFoodEat == false ) //이벤트 아이템을 먹지 않았다.
		{
			return false;
		}
	}
#endif SERV_EVENT_PET_INVENTORY
	return true;
}

void CX2UIInventory::UpdateButtonPetInventory()
{
	CKTDGUIButton* pPetInventoryButton = static_cast< CKTDGUIButton* >( m_pDLGUIInventory->GetControl( L"PetInventory" ) );
	if(IsPetInventoryExist() == false)
	{
		pPetInventoryButton->SetShowEnable( false, false );
	}
	else if(g_pData->GetUIManager()->PetInventoryForceOpenCondition() == true)
	{
		pPetInventoryButton->SetShowEnable( true, true );
	}
	else
	{
		pPetInventoryButton->SetShowEnable( true, false );
	}	
	return;
}
#endif ADD_PET_INVENTORY_BUTTON

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
void CX2UIInventory::InitItemObtainResult(const WCHAR* pFileName, std::map< int, int >& mapInsertedItem )
{ 	
	m_pDLGResolveItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), pFileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItemResult );

	//아이템 획득 결과창의 위치 설정
	m_pDLGResolveItemResult->SetPos( D3DXVECTOR2( 320, 230 ) );

	CKTDGUIButton* pControl = (CKTDGUIButton*) m_pDLGResolveItemResult->GetControl( L"Button_OK" );
	if ( pControl != NULL )
	{
		pControl->RequestFocus();
	}

	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_Slot_Pic" );
	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotName" );
	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotNum" );

	//아이템 획득 결과창의 아이템 이미지와 스트링 초기화
	for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; ++slotNum )
	{
		if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
		{
			pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
		}

		pStaticName->GetString(slotNum*2)->msg = L"";
		pStaticName->GetString(slotNum*2 + 1)->msg = L"";

		if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
		{
			pStaticNum->GetString(slotNum)->msg = L"";
		}
	}


	//획득한 아이템으로 아이템 슬롯 설정
	int index = 0;
	std::map< int, int >::iterator it = mapInsertedItem.begin(); 
	while( it != mapInsertedItem.end() )
	{
		int resultItemID = it->first;

		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
		if ( pItemTemplet != NULL )
		{
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(index) != NULL )
			{
                const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
				if ( pwszShopImage[0] != NULL )
					pStaticSlot->GetPicture(index)->SetTex( pwszShopImage );
				else
					pStaticSlot->GetPicture(index)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

			bool bCheckTwoLine = false;
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring wstrFullName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetFullName_(), 145, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
			wstring wstrFullName = g_pMain->GetStrByLienBreak( 
                pItemTemplet->GetFullName_(), 
                145, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
			if ( wstrFullName.find( L"\n") != -1 )
				bCheckTwoLine = true;

			if(pStaticName->GetString(index*2) != NULL)
			{
				if ( bCheckTwoLine == false )
				{
					pStaticName->GetString(index*2)->msg = wstrFullName;
					pStaticName->GetString(index*2+1)->msg = L"";
				}
				else
				{
					pStaticName->GetString(index*2)->msg = L"";
					pStaticName->GetString(index*2+1)->msg = wstrFullName;
				}
			}
			if ( pStaticNum != NULL && pStaticNum->GetString(index) != NULL )
			{
				wstring wstrCount = GET_REPLACED_STRING( ( STR_ID_865, "i", it->second) );
				pStaticNum->GetString(index)->msg = wstrCount.c_str();
			}
		}
		// 아이템이 없는 경우
		else
		{
			pStaticSlot->GetPicture(index)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			pStaticName->GetString(index)->msg = L"";
			pStaticNum->GetString(index)->msg = L"";
		}
		++it;
		++index;
	}
}
void CX2UIInventory::InitItemResolveResult(const WCHAR* pFileName, std::map< int, int >& mapInsertedItem )
{
	m_pDLGResolveItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), pFileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItemResult );
	//아이템 획득 결과창의 위치 설정
	m_pDLGResolveItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 446, 376 ), D3DXVECTOR2( 222, 288) ) );

	CKTDGUIButton* pControl = (CKTDGUIButton*) m_pDLGResolveItemResult->GetControl( L"Button_OK" );
	if ( pControl != NULL )
	{
		pControl->RequestFocus();
	}

	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_Slot_Pic" );
	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotName" );
	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotNum" );
#ifdef ELLIPSE_GLOBAL
	CKTDGUIButton* pStaticToolTip[RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM] = { 0, };
#endif //ELLIPSE_GLOBAL
	//아이템 획득 결과창의 아이템 이미지와 스트링 초기화
	for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; slotNum++ )
	{
		if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
		{
			pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
		}
		if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
		{
			pStaticName->GetString(slotNum)->msg = L"";
		}
		if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
		{
			pStaticNum->GetString(slotNum)->msg = L"";
		}
#ifdef INTEGRATE_TOOLTIP
		pStaticToolTip[slotNum] = 
			static_cast< CKTDGUIButton* >( m_pDLGResolveItemResult->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"DungeonDescriptionToolTip" ), slotNum + 1 ) ) ) );
			
		if( NULL != pStaticToolTip[slotNum] )
		{
			pStaticToolTip[slotNum]->SetShowEnable( false, false );
		}
#endif //INTEGRATE_TOOLTIP
	}

	//획득한 아이템으로 아이템 슬롯 설정
	int index = 0;
	std::map< int, int >::iterator it = mapInsertedItem.begin(); 
	while ( it != mapInsertedItem.end() )
	{
		int resultItemID = it->first;

		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
		if ( pItemTemplet != NULL )
		{
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(index) != NULL )
			{
                const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
				if ( pwszShopImage[0] != NULL )
					pStaticSlot->GetPicture(index)->SetTex( pwszShopImage );
				else
					pStaticSlot->GetPicture(index)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

			if ( pStaticName != NULL && pStaticName->GetString(index) != NULL )
			{
#ifdef INTEGRATE_TOOLTIP			
				bool bEllipse = false;
				wstring wstrName = CWordLineHandler::CutStringWithEllipse(pItemTemplet->GetFullName_(), 140, XUF_DODUM_13_SEMIBOLD, 1, bEllipse);
				pStaticName->GetString(index)->msg = wstrName;

				if(bEllipse == true)
				{
					wstring wstrFullName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetFullName_(), 145, XUF_DODUM_13_SEMIBOLD );
					pStaticToolTip[index]->SetGuideDesc(wstrFullName.c_str());
					pStaticToolTip[index]->SetShowEnable(true, true);
				}
				else
				{
					pStaticToolTip[index]->SetShowEnable(false, false);
				}
                pStaticName->GetString(index)->msg = wstrName;
#else //INTEGRATE_TOOLTIP
                pStaticName->GetString(index)->msg = pItemTemplet->GetFullName_();
#endif //INTEGRATE_TOOLTIP			
			}
			if ( pStaticNum != NULL && pStaticNum->GetString(index) != NULL )
			{
				wstring wstrCount = GET_REPLACED_STRING( ( STR_ID_865, "i", it->second) );
				pStaticNum->GetString(index)->msg = wstrCount.c_str();
			}
		}
		// 아이템이 없는 경우
		else
		{
			pStaticSlot->GetPicture(index)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			pStaticName->GetString(index)->msg = L"";
			pStaticNum->GetString(index)->msg = L"";
		}
		++it;
		++index;
	}
}
#endif RANDOM_EXCHANGE_RESULT_VIEW


void CX2UIInventory::UseDefenceDungeonPotion( const UidType uidItem_, const int iItemId_ )
{

}

void CX2UIInventory::SetEDString(int iED_)
{
	if( false == m_bShow )
		return;

	if( NULL != m_pDLGUIInventory )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGUIInventory->GetControl(L"Static_ED"));
		if( NULL != pStatic && NULL != pStatic->GetString(0))
		{
			pStatic->GetString(0)->msg = g_pMain->GetEDString(iED_);
		}
	}
}
void CX2UIInventory::SetAPString(int iAP_)
{
	if( false == m_bShow )
		return;

	if( NULL != m_pDLGUIInventory )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGUIInventory->GetControl(L"Static_AP"));
		if( NULL != pStatic && NULL != pStatic->GetString(0))
		{
			pStatic->GetString(0)->msg = g_pMain->GetEDString(iAP_);
		}
	}
}
#endif	/// NEW_VILLAGE_UI

#ifdef FREE_WARP
void CX2UIInventory::UseWarpItem( CX2SlotItem* pItemSlot_, bool bIsEdConsumption_ )
{	
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_BATTLE_FIELD:
		{
#ifdef FIELD_BOSS_RAID // 김태환
			/// 보스 레이드 필드 안에서는 사용할 수 없다.
			if ( NULL != g_pData && g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
			{
				m_WarpItem = 0;
				g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4495 ),g_pMain->GetNowState() );
				break;
			}
#endif // FIELD_BOSS_RAID

			m_WarpItem = pItemSlot_->GetItemUID();
			m_bIsEdConsumptionWarp = bIsEdConsumption_;
			CreateWarpDest();
		} break;

	default:
		{
			// 마을/거래광장/배틀필드 이외에서는 사용안됨...
			m_WarpItem = 0;
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4495 ),g_pMain->GetNowState() );
		} break;
	}	/// switch
}
void CX2UIInventory::UseWarpPopup(bool bIsEdConsumption_)
{
	if( NULL != g_pData && NULL != g_pData->GetLocationManager() &&
		NULL != g_pData->GetLocationManager()->GetVillageMapTemplet(static_cast<SEnum::VILLAGE_MAP_ID>(m_iSelectedWarpDest)) )
	{
		wstring strVillageName = g_pData->GetLocationManager()->GetVillageMapTemplet(static_cast<SEnum::VILLAGE_MAP_ID>(m_iSelectedWarpDest))->m_Name;

		if( true == strVillageName.empty() )
			return;

		if( true == bIsEdConsumption_ )
		{
			int iWarpDistance = abs(m_iSelectedWarpDestIndex - m_iCurrentVillageWarpIndex);
			int iBasePrice = 6000;
			float fDecreaseFactor = 0.6f;
			if( NULL !=  g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
			{				
				float fLevel = static_cast<float>(g_pData->GetSelectUnitLevel());
				if( fLevel > 0 )
				{
					int iWarpCommission = static_cast<int>((fLevel/60) * (iBasePrice + static_cast<int>(iBasePrice * fDecreaseFactor) * (iWarpDistance-1) )); //이동 수수료	

					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
						GET_REPLACED_STRING((STR_ID_20266, "iL", iWarpCommission, strVillageName)), 
						UIM_WARP_DEST_OK, g_pMain->GetNowState() );	
				}
			}
		}
		else
		{
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
				GET_REPLACED_STRING((STR_ID_20297, "L", strVillageName)), 
				UIM_WARP_DEST_OK, g_pMain->GetNowState() );	
		}
	}
}
#endif //FREE_WARP

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
void CX2UIInventory::UseUSCJobItem( CX2SlotItem* pItemSlot_ )
{	
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			CreateSecondJobDest();
		} break;
	default:
		{
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4495 ),g_pMain->GetNowState() );
		} break;
	}
}

void CX2UIInventory::UseUSCJobPopup()
{
	//전직 작업 한다. 이 클래스로 전직 하시겠습니까? 물어봅니다.
	if( NULL != g_pData && NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit()->GetClass() )
	{
		CX2Unit::UNIT_CLASS cUnitClass = static_cast<CX2Unit::UNIT_CLASS>(m_iSelectedUSCJob);
		wstring strClassName = g_pData->GetUnitManager()->GetUnitTemplet( cUnitClass )->m_Name;
		

		if( 0 == strClassName.length() )
			return;

			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
				GET_REPLACED_STRING((STR_ID_29778, "L", strClassName)), 
				UIM_UNLIMITED_SECOND_CHANGE_JOB_OK, g_pMain->GetNowState() );	
		
	}
}
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef	SERV_REFORM_INVENTORY_TEST
void CX2UIInventory::SetShowInventoryPageUI( int iMaxPageNum /*= 0*/ )
{
#ifdef SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
	const int iMaxCreatePageNum = 4;
#else // SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
	const int iMaxCreatePageNum = 3;
#endif // SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR

	if( iMaxCreatePageNum < iMaxPageNum )
		iMaxPageNum = iMaxCreatePageNum;

	CKTDGUIRadioButton* pRadio_Page1	= static_cast<CKTDGUIRadioButton*>( m_pDLGUIInventory->GetControl( L"Inventory_page1" ) );	/// 페이지 1
	CKTDGUIRadioButton* pRadio_Page2	= static_cast<CKTDGUIRadioButton*>( m_pDLGUIInventory->GetControl( L"Inventory_page2" ) );	/// 페이지 2
	CKTDGUIRadioButton* pRadio_Page3	= static_cast<CKTDGUIRadioButton*>( m_pDLGUIInventory->GetControl( L"Inventory_page3" ) );	/// 페이지 3
	CKTDGUIRadioButton* pRadio_Page4	= static_cast<CKTDGUIRadioButton*>( m_pDLGUIInventory->GetControl( L"Inventory_page4" ) );	/// 페이지 4

	if( NULL != pRadio_Page1 && 
		NULL != pRadio_Page2 && 
		NULL != pRadio_Page3 && 
		NULL != pRadio_Page4 )
	{

		pRadio_Page1->SetShow( true );
		pRadio_Page2->SetShow( true );
		pRadio_Page3->SetShow( true );
		pRadio_Page4->SetShow( true );

		switch( iMaxPageNum )
		{
		case 1:
			{
				pRadio_Page1->SetShow( false );
				pRadio_Page2->SetShow( false );
			} 
		case 2:
			{
				pRadio_Page3->SetShow( false );
			} 
		case 3: 
			{
				pRadio_Page4->SetShow( false );
			} break;
		default: break;
		}
	}
}

bool CX2UIInventory::UpdatePageUI( int iPage /*= 1*/ )
{
	CKTDGUIRadioButton* pButtonPage1 = static_cast<CKTDGUIRadioButton*>(m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_page1" ));
	CKTDGUIRadioButton* pButtonPage2 = static_cast<CKTDGUIRadioButton*>(m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_page2" ));
	CKTDGUIRadioButton* pButtonPage3 = static_cast<CKTDGUIRadioButton*>(m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_page3" ));
	CKTDGUIRadioButton* pButtonPage4 = static_cast<CKTDGUIRadioButton*>(m_pDLGUIInventory->GetRadioButton_LUA( "Inventory_page4" ));

	switch( iPage )
	{
	case 1:
		if( pButtonPage1 != NULL ) pButtonPage1->SetChecked( true );
		break;
	case 2:
		if( pButtonPage2 != NULL ) pButtonPage2->SetChecked( true );
		break;
	case 3:
		if( pButtonPage3 != NULL ) pButtonPage3->SetChecked( true );
		break;
	case 4:
		if( pButtonPage4 != NULL ) pButtonPage4->SetChecked( true );
		break;
	}

	return true;
}

int CX2UIInventory::GetPageNumBySlotID( int iSlotID )
{
	if( iSlotID < 0 || iSlotID >= MY_INVENTORY_SHOW_MAX_SLOT_NUM )
		return -1;

	return static_cast<int>(iSlotID / MY_INVENTORY_SHOW_SLOT_NUM_PER_PAGE) + 1;
}

int CX2UIInventory::GetMaxLineNumPerCategory( CX2Inventory::SORT_TYPE InventorySortType_ )		/// 현제 카테고리의 최대 슬롯 줄 수 반환
{
	switch( InventorySortType_ )
	{
	case CX2Inventory::ST_SPECIAL:
		{
			return MY_INVENTORY_SPECIAL_MAX_SLOT_WIDTH;		/// 8줄
		} break;
	case CX2Inventory::ST_AVARTA:
		{
			return MY_INVENTORY_AVARTA_MAX_SLOT_WIDTH;		/// 21줄
		} break;
	case CX2Inventory::ST_PC_BANG:
		{
			return MY_INVENTORY_PC_BANG_MAX_SLOT_WIDTH;		/// 3줄
		} break;
	default:
		{
			return MY_INVENTORY_MAX_SLOT_HEIGHT;			/// 7줄
		} break;
	}
}

#endif	SERV_REFORM_INVENTORY_TEST

#ifdef PET_DROP_ITEM_PICKUP
/** function : SetPetAutoLooting
	brief : 물건 줍기 오라 스킬 활성화 아이템 사용 성공 시, 
	          펫의 정보를 변경시키는 함수
	param : uiPetUID_ : 변경 시킬 펫의 UID
*/
void CX2UIInventory::SetPetAutoLooting( UidType uiPetUID_ )
{
	if( uiPetUID_ <= 0 )
		return;
	
	if( NULL == g_pData->GetPetManager() )
		return;

	CX2PET* pMyPet = g_pData->GetPetManager()->GetPetByUid( uiPetUID_ );
	if( NULL != pMyPet )
	{ //아이템 줍기 스킬 활성화						
		pMyPet->GetPetInfo().m_bIsDropItemPickup = true;
	}

	if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIPetList() )
	{
		CX2UIPetInfo* pUIPetInfo = g_pData->GetUIManager()->GetUIPetList();
		//UI에서 사용 하고 있는 펫 정보 변경
		pUIPetInfo->SetPetAutoLooting( uiPetUID_, true);

		//캐릭터가 가지고 있는 펫 정보 변경
		if( NULL != g_pData->GetMyUser() &&
			NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			g_pData->GetMyUser()->GetSelectUnit()->SetPetAutoLooting( uiPetUID_, true);
		}

		if( true == pUIPetInfo->GetShow() )
		{
			pUIPetInfo->SetShow( false );
		}
	}

	if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
	{						
		g_pX2Game->GetMyUnit()->UpdateSocketDataAndEnchantData();
		g_pX2Game->GetMyUnit()->UpdatePassiveAndActiveSkillState();
	}

	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_21670 ), g_pMain->GetNowState() );				
}
#endif //PET_DROP_ITEM_PICKUP

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
void CX2UIInventory::UseTransformPetItem()
{
	if( m_UseTransformdItemUID == 0 || m_UseTransformItemPetUID == 0 )
	{
		return;
	}

	Handler_EGS_USE_ITEM_IN_INVENTORY_REQ( m_UseTransformdItemUID, m_UseTransformItemPetUID );

	m_UseTransformdItemUID = 0;
	m_UseTransformItemPetUID = 0;
}
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef FIX_ICE_HEATER_EVENT

/** @function	: OpenScriptFile
	@brief		: 가열기 정보 스크립트 오픈 함수
	@param		: 스크립트 파일명 ( IceHeaterEvent.lua )
*/
void CX2UIInventory::OpenScriptFile( const WCHAR* szScriptFileName_ )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIInventory", this );

    if ( g_pKTDXApp->LoadLuaTinker( szScriptFileName_ ) == false )
    {
		ASSERT( !"DoMemory doesn't work!" );
		ErrorLogMsg( XEM_ERROR2, szScriptFileName_ );
    }
}

/** @function	: AddIceHeaterEventInfo_LUA
	@brief		: 가열기 재생 관련 정보 스크립트 파싱 함수
*/
void CX2UIInventory::AddIceHeaterEventInfo_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	IceHeaterEventData* pIceHeaterEventData = new IceHeaterEventData;

	/// 디폴트 인자는 기본 가열기 정보로 설정
	RANDOM_ITEM_OPEN_EVENT_TYPE eIceHeaterEnum = RIOET_NONE;			/// 가열기 아이디

	LUA_GET_VALUE_ENUM( luaManager, "ICE_HEATER_ID",					eIceHeaterEnum,	
						RANDOM_ITEM_OPEN_EVENT_TYPE,					RIOET_ICE_HEATER										);

	/// 가열기 동작 전 이펙트			( UIMajorParticle.txt )
	LUA_GET_VALUE(		luaManager, "WAIT_UI_EFFECT_SET_NAME",			pIceHeaterEventData->m_wstrWaitUIEffectSetName,			L"EffectSet_Ice_Heater_Wait"	);
	/// 가열기 동작 이펙트				( UIEffectSet.lua )
	LUA_GET_VALUE(		luaManager, "PLAY_UI_EFFECT_SET_NAME",			pIceHeaterEventData->m_wstrPlayUIEffectSetName,			L"EffectSet_Ice_Heater_Start"	);

	/// 사용할 대기용 가열기 매시 플레이어		( UIMajorXMeshPlayer.txt )
	LUA_GET_VALUE(		luaManager, "WAIT_MESH_PLAYER_NAME",			pIceHeaterEventData->m_wstrWaitMeshPlayerName,			L"FireMachineWait"				);
	/// 사용할 재생용 가열기 매시 플레이어		( UIMajorXMeshPlayer.txt )
	LUA_GET_VALUE(		luaManager, "PLAY_MESH_PLAYER_NAME",			pIceHeaterEventData->m_wstrPlayMeshPlayerName,			L"FireMachineStart"				);

	/// 가열기 다이얼로그에 표시될 가열기 이름 텍스처
#ifdef UNIQUENESS_EU_ONLY
	LUA_GET_VALUE(		luaManager, "MAIN_DIALOG_TITLE_TEXTURE",		pIceHeaterEventData->m_wstrMainDialogTitleTexture,		L"DLG_UI_Title02_A.tga"			);
#else //UNIQUENESS_EU_ONLY
	LUA_GET_VALUE(		luaManager, "MAIN_DIALOG_TITLE_TEXTURE",		pIceHeaterEventData->m_wstrMainDialogTitleTexture,		L"DLG_UI_Title02.tga"			);
#endif 	//UNIQUENESS_EU_ONLY
	/// 가열기 다이얼로그에 표시될 가열기 이름 텍스처 키
	LUA_GET_VALUE(		luaManager, "MAIN_DIALOG_TITLE_TEXTURE_KEY",	pIceHeaterEventData->m_wstrMainDialogTitleTextureKey,	L"heater_title"					);

	/// 가열기 결과창 다이얼로그에 표시될 가열기 이름 텍스처
#ifdef UNIQUENESS_EU_ONLY
	LUA_GET_VALUE(		luaManager, "RESULT_DIALOG_TITLE_TEXTURE",		pIceHeaterEventData->m_wstrResultDialogTitleTexture,	L"DLG_UI_Title02_A.tga"			);
#else //UNIQUENESS_EU_ONLY
	LUA_GET_VALUE(		luaManager, "RESULT_DIALOG_TITLE_TEXTURE",		pIceHeaterEventData->m_wstrResultDialogTitleTexture,	L"DLG_UI_Title02.tga"			);
#endif 	//UNIQUENESS_EU_ONLY
	/// 가열기 결과창 다이얼로그에 표시될 가열기 이름 텍스처 키
	LUA_GET_VALUE(		luaManager, "RESULT_DIALOG_TITLE_TEXTURE_KEY",	pIceHeaterEventData->m_wstrResultDialogTitleTextureKey,	L"heater_title"					);

	/// 가열기 다이얼로그에 표시될 느낌표 아이콘의 오프셋 X 위치
	LUA_GET_VALUE(		luaManager, "CAUTION_ICON_POSITION_X",			pIceHeaterEventData->m_fCautionIconOffSetPositionX,		-20.f							);
	/// 가열기 다이얼로그에 표시될 느낌표 아이콘의 오프셋 Y 위치
	LUA_GET_VALUE(		luaManager, "CAUTION_ICON_POSITION_Y",			pIceHeaterEventData->m_fCautionIconOffSetPositionY,		0.f								);

	/// 가열기 다이얼로그의 느낌표 아이콘 옆에 표시될 스트링의 오프셋 X 위치
	LUA_GET_VALUE(		luaManager, "CAUTION_STRING_POSITION_X",		pIceHeaterEventData->m_fCautionStringOffSetPositionX,	0.f								);
	/// 가열기 다이얼로그의 느낌표 아이콘 옆에 표시될 스트링의 오프셋 Y 위치
	LUA_GET_VALUE(		luaManager, "CAUTION_STRING_POSITION_Y",		pIceHeaterEventData->m_fCautionStringOffSetPositionY,	0.f								);

	// 가열기 다이얼로그의 느낌표 아이콘 옆에 표시될 스트링
	LUA_GET_VALUE(		luaManager, "CAUTION_STRING_NUM",				pIceHeaterEventData->m_iCautionStringNum,				STR_ID_EMPTY					);

	/// 가열기 가동 재생음
	LUA_GET_VALUE(		luaManager, "ICE_HEATER_PLAY_SOUND",			pIceHeaterEventData->m_wstrIceHeaterPlaySound,			L"Fire_Machine_Start.ogg"		);

	/// 재생 시간 ( 가열기 가동 ~ 종료까지 시간 )
	LUA_GET_VALUE(		luaManager, "PLAY_TIME",						pIceHeaterEventData->m_fPlayTime,						10.f							);

	/// 위의 내용들을 삽입
	m_mapIceHeaterEventData.insert( std::make_pair( eIceHeaterEnum, pIceHeaterEventData ) );
}

#endif // FIX_ICE_HEATER_EVENT

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //아이템 개편 관련 패킷 작업
/** @fucntion : IsRestoreItem
	@brief : "장비 아이템을 미감정상태로 되돌리기는 아이템" 인지 확인 하는 함수
*/
bool CX2UIInventory::IsRestoreEvaluateItem( const int ItemID_ ) const
{
	for(int i=0; i<ARRAY_SIZE(ITEM_ID_RESTORE_ITEM_EVALUATE); ++i)
	{
		if( ItemID_ == ITEM_ID_RESTORE_ITEM_EVALUATE[i] )
			return true;
	}
	return false;
}
/** @fucntion : UseRestoreEvaluteItem
	@brief : 미라클 큐브(미감정 상태로 되돌리기 아이템) 사용 동작 처리
*/
void CX2UIInventory::UseRestoreEvaluteItem( UidType iConsumeItemUID_ )
{
	if( iConsumeItemUID_ > 0 ) 
	{
		m_iConsumeItemUID = iConsumeItemUID_;

		// 마우스 커서 변경
		CX2State* pState = static_cast<CX2State*>(g_pMain->GetNowState());
		if( NULL != pState && NULL != pState->GetCursor() )
		{
			pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_ITEM_EVALUTE );
		}

		ChangeInventoryTab( CX2Inventory::ST_EQUIP );

		return;
	}
}
bool CX2UIInventory::Handler_EGS_RESTORE_ITEM_EVALUATE_REQ()
{
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		const CX2Item* pConsumeItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iConsumeItemUID );
		const CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iTargetItemUID );

		if ( pConsumeItem == NULL || pConsumeItem->GetItemTemplet() == NULL || 
			pTargetItem == NULL || pTargetItem->GetItemTemplet() == NULL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_833 ), g_pMain->GetNowState() );
			return false;
		}

		if( false == g_pData->GetItemStatCalculator().IsRandomSocketOptionItem( pTargetItem->GetItemTemplet() ) )
		{
			// 미감정 상태로 되돌리기가 불가능한 아이템에 대한 가이드 팝업
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24614 ), g_pMain->GetNowState() );
			return false;
		}

		if( false == pTargetItem->GetIsEvaluation() )
		{
			// 미감정 상태로 되돌리기가 불가능한 아이템에 대한 가이드 팝업
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24614 ), g_pMain->GetNowState() );
			return false;
		}
	}

	KEGS_RESTORE_ITEM_EVALUATE_REQ kPacket;
	kPacket.m_iSupportItemUID = m_iConsumeItemUID;
	kPacket.m_iTargetItemUID = m_iTargetItemUID;

	m_iConsumeItemUID = 0;
	m_iTargetItemUID = 0;

	g_pData->GetServerProtocol()->SendPacket( EGS_RESTORE_ITEM_EVALUATE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RESTORE_ITEM_EVALUATE_ACK );

	return true;
}
bool CX2UIInventory::Handler_EGS_RESTORE_ITEM_EVALUATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_RESTORE_ITEM_EVALUATE_ACK kPacket;
	DeSerialize( pBuff, &kPacket );
	
	if( true == g_pMain->DeleteServerPacket( EGS_RESTORE_ITEM_EVALUATE_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kPacket.m_iOK ) )
		{
			if( NULL != g_pData && 
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
 				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kPacket.m_vecInventorySlotInfo);
			}

			UpdateInventorySlotList( kPacket.m_vecInventorySlotInfo );
 			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_835 ), g_pMain->GetNowState() );

			return true;
		}		
	}
	return false;
}
bool CX2UIInventory::Handler_EGS_ITEM_EVALUATE_REQ()
{
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		const CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iTargetItemUID );

		if( pTargetItem == NULL || pTargetItem->GetItemTemplet() == NULL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_833 ), g_pMain->GetNowState() );
			return false;
		}

		if( false == g_pData->GetItemStatCalculator().IsRandomSocketOptionItem( pTargetItem->GetItemTemplet() ) )
		{
			// 감정이 불가능한 아이템에 대한 가이드 팝업
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_833 ), g_pMain->GetNowState() );
			return false;
		}
	}

	KEGS_ITEM_EVALUATE_REQ kPacket;
	kPacket.m_iItemUID = m_iTargetItemUID;
	m_iTargetItemUID = 0;

	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_EVALUATE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ITEM_EVALUATE_ACK );

	return true;
}
bool CX2UIInventory::Handler_EGS_ITEM_EVALUATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ITEM_EVALUATE_ACK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( true == g_pMain->DeleteServerPacket( EGS_ITEM_EVALUATE_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kPacket.m_iOK ) )
		{
			if( NULL != g_pData && 
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kPacket.m_vecInventorySlotInfo);
			}
			UpdateInventorySlotList( kPacket.m_vecInventorySlotInfo );
			g_pData->SetSelectUnitED( kPacket.m_iED );

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_25080 ), g_pMain->GetNowState() );
			return true;
		}		
	}
	return true;
}
void CX2UIInventory::Handler_EGS_ITEM_CONVERT_REQ( UidType iItemUID, int iQuantity  )
{
	if( iItemUID < 0 )
		return;

	KEGS_ITEM_CONVERT_REQ kPacket;
	kPacket.m_iItemUID = iItemUID;
	kPacket.m_iQuantity = iQuantity;

	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_CONVERT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ITEM_CONVERT_ACK );
}
bool CX2UIInventory::Handler_EGS_ITEM_CONVERT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ITEM_CONVERT_ACK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( true == g_pMain->DeleteServerPacket( EGS_ITEM_CONVERT_ACK ) )
	{		
		if( true == g_pMain->IsValidPacket( kPacket.m_iOK ) )
		{
			if( NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kPacket.m_vecInventorySlotInfo);
			}
			UpdateInventorySlotList( kPacket.m_vecInventorySlotInfo );
		}

		return true;
	}

	return true;
}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADDED_ENCHANT_COUPON_HAVE_LEVEL_LIMIT    // 김종훈 / 13-07-03 / 레벨 제한을 가진 강화의 부적
bool CX2UIInventory::CheckLevelIfLevelLimitEnchantCoupon ( const int iCouponItemID_ , const int iTargetItemLevel_ )
{
	for ( UINT i = 0; i < ARRAY_SIZE(HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_ID); i++ )
	{
		if ( iCouponItemID_ == HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_ID[i] )
		{
			if ( i * 2 >= ARRAY_SIZE (HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_LIMIT_LEVEL) )
			{
				ASSERT ( L"HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_ID Array Size Over !" );
				return true;
			}

			else if ( iTargetItemLevel_ > HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_LIMIT_LEVEL[2*i] &&
				 iTargetItemLevel_ <= HAVE_LIMIT_LEVEL_ENCHANT_COUPON_ITEM_LIMIT_LEVEL[2*i+1] )
			{
				return false;
			}

			else 
			{
				return true;
			}
		}
	}


	return false;
}
#endif // ADDED_ENCHANT_COUPON_HAVE_LEVEL_LIMIT // 김종훈 / 13-07-03 / 레벨 제한을 가진 강화의 부적

#ifdef FINALITY_SKILL_SYSTEM //JHKang
void CX2UIInventory::OpenElBreakWindow( IN CX2Item* pItem_ )
{
	SAFE_DELETE_DIALOG( m_pDLGElBreak );

	if ( pItem_ == NULL )
		return;

	m_ElBreakItemUID	= pItem_->GetItemData().m_ItemUID;
	m_ElBreakItemCount	= 1;

	m_pDLGElBreak = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_El_Break_Window.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGElBreak );

	//아이템 이미지 셋팅해주고.
	CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGElBreak->GetControl( L"ItemImage" );
	if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
	{
		pStaticItemImage->GetPicture(0)->SetTex( g_pData->GetItemManager()->GetItemTemplet( pItem_->GetItemData().m_ItemID )->GetShopImage() );
	}

	//아이템 이름 셋팅해주고.
	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGElBreak->GetControl( L"Identify_Window_Price" );
	if ( pStaticItemName != NULL )
	{
		pStaticItemName->SetString( 0, g_pData->GetItemManager()->GetItemTemplet( pItem_->GetItemData().m_ItemID )->GetName() );
	}

	CKTDGUIControl* pControl = m_pDLGElBreak->GetControl( L"Break_Tool" );
	if ( pControl != NULL )
		pControl->SetShow( true );
		
	UpdateElBreakDLG( false );
}

void CX2UIInventory::UpdateElBreakDLG( IN const bool bReadIME_ )
{
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_ElBreakItemUID );
	
	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
		return;

	if ( m_pDLGElBreak == NULL )
		return;

	const int MAGIC_MAX_EL_CHANGE_ONCE = 100;
	int nowQuantity = 0;
	nowQuantity = min( pItem->GetItemData().m_Quantity, MAGIC_MAX_EL_CHANGE_ONCE );
	
	// IME에서 값 받아오고
	CKTDGUIIMEEditBox* pQuantity = (CKTDGUIIMEEditBox*) m_pDLGElBreak->GetControl( L"IME_Identify_Window" );
	if( bReadIME_ )
	{
		m_ElBreakItemCount = g_pMain->GetEDFromString( pQuantity->GetText() );
	}	

	if ( m_ElBreakItemCount > nowQuantity )
		m_ElBreakItemCount = nowQuantity;

	// 확인해주고
	if ( m_ElBreakItemCount <= 0 )
	{
		if( bReadIME_ )				// 입력창으로 받아온 경우
			m_ElBreakItemCount = 0;
		else						// << 를 누른 경우 : 0 아래면 max로 순환시킨다.
			m_ElBreakItemCount = nowQuantity;	
	}

	WCHAR buff[256] = {0};
	_itow( m_ElBreakItemCount, buff, 10 );
	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str() );
}

bool CX2UIInventory::Handler_EGS_ITEM_EXTRACT_REQ()
{
	KEGS_ITEM_EXTRACT_REQ kPacket;

	kPacket.m_iItemUID = m_ElBreakItemUID;
	kPacket.m_iQuantity = m_ElBreakItemCount;

	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_EXTRACT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ITEM_EXTRACT_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_ITEM_EXTRACT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ITEM_EXTRACT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_ITEM_EXTRACT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			wstring itemName = g_pData->GetItemManager()->GetItemTemplet( CX2EnchantItem::ATI_HYPER_SKILL_STONE )->GetFullName_();
			itemName += L" ";
			itemName += g_pMain->GetEDString( m_ElBreakItemCount * 3 );
			itemName += GET_STRING( STR_ID_24 );
			itemName += GET_STRING( STR_ID_426 );
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), itemName.c_str(), g_pMain->GetNowState() );
			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
		}

		return true;
	}

	return false;
}
#endif //FINALITY_SKILL_SYSTEM

#pragma region Good Elsword
#ifdef GOOD_ELSWORD //JHKang
#ifndef NO_GOOD_ELSWORD_INT
bool CX2UIInventory::Handler_EGS_GET_NEXT_INVENTORY_ED_REQ()
{
	if( true == g_pMain->IsWaitingServerPacket( EGS_GET_NEXT_INVENTORY_ED_ACK ) )
		return false;

	g_pData->GetServerProtocol()->SendID( EGS_GET_NEXT_INVENTORY_ED_REQ );
	g_pMain->AddServerPacket( EGS_GET_NEXT_INVENTORY_ED_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_GET_NEXT_INVENTORY_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>( lParam );
	KEGS_GET_NEXT_INVENTORY_ED_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_GET_NEXT_INVENTORY_ED_ACK );
	{
		if (g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			wstringstream	wstrstm;

			m_iConfirmED = kEvent.m_iED;

			wstrstm << GET_REPLACED_STRING( ( STR_ID_28667, "LL", g_pMain->GetEDString( kEvent.m_iED ),
											g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );

			if ( m_pDLGUIInventory->GetShow() )
			{
				m_pDLGBuyConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrstm.str().c_str(), UIM_EXPAND_INVENTORY_ED_BUY_CONFIRM, 
					g_pMain->GetNowState(), UIM_EXPAND_INVENTORY_ED_BUY_CANCEL, L"", D3DXVECTOR2( 0, 20 ) );
			}

			return true;
		}
	}

	return false;
}

bool CX2UIInventory::Handler_EGS_EXPAND_INVENTORY_ED_REQ()
{
	if( true == g_pMain->IsWaitingServerPacket( EGS_EXPAND_INVENTORY_ED_ACK ) )
		return false;

	g_pData->GetServerProtocol()->SendID( EGS_EXPAND_INVENTORY_ED_REQ );
	g_pMain->AddServerPacket( EGS_EXPAND_INVENTORY_ED_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_EXPAND_INVENTORY_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>( lParam );
	KEGS_EXPAND_INVENTORY_ED_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2User*	  pUser		 = g_pData->GetMyUser();
	if( pUser == NULL )
	{
		ASSERT( !"User is NULL(X2User)" );
		return false;
	}

	CX2Unit*	  pUnit		 = pUser->GetSelectUnit();
	if( pUnit == NULL)
	{
		ASSERT( !"Unit is NULL(X2Unit)" );
		return false;
	}

    CX2Inventory& kInventory = pUnit->AccessInventory();

	g_pMain->DeleteServerPacket( EGS_EXPAND_INVENTORY_ED_ACK );
	{
		if (g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			map< int, int >::iterator mit;
			for ( mit = kEvent.m_mapExpandedCategorySlot.begin(); mit != kEvent.m_mapExpandedCategorySlot.end(); mit++ )
			{
				kInventory.SetItemMaxNum( (CX2Inventory::SORT_TYPE)mit->first, 
					kInventory.GetItemMaxNum((CX2Inventory::SORT_TYPE)mit->first) + mit->second );

				switch( static_cast< CX2Inventory::SORT_TYPE >( mit->first ) )
				{
				case CX2Inventory::ST_EQUIP:
					g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_28668 ),g_pMain->GetNowState() );
					break;
				}
			}

			if( NULL != g_pData->GetQuestManager() )
			{
				g_pData->GetQuestManager()->GiveUpForbiddenQuest(); 
			}

			m_bShow = true;
			SetShow( m_bShow );

			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

			return true;
		}
	}

	return false;
}
#endif NO_GOOD_ELSWORD_INT
#endif //GOOD_ELSWORD
#pragma endregion 확장 관련 처리 패킷

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
bool CX2UIInventory::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 마우스 커서가 인벤토리 범위 안에 있을 때만 처리하기 
	if( false == CX2UIInventory::GetIsMouseOver() )
		return true;

	if( NULL != g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFirstFrontModalDlg() )
		return true;

	if( NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillNote() &&
		true == g_pData->GetUIManager()->GetUISkillNote()->GetShow() )
		return true;

	// 불편함 느끼지 않는 선에서 쿨타임 추가
	if( m_fLeftMouseWhellCoolTime > 0.f )
		return true;

	short zDelta = static_cast<short>(HIWORD(wParam));
	m_SumDelta += zDelta;
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		m_fLeftMouseWhellCoolTime = 0.2f;
		const int iNowPageNum = GetNowInventoryPageNum( m_NowInventorySortType );

		int iChangePageNum = 1;
		// 휠 업
		if(m_SumDelta>0)
		{
			iChangePageNum = max( (iNowPageNum - 1), 1 );
			m_SumDelta -= WHEEL_DELTA;
		}
		// 휠 다운
		else
		{
			/// 인벤토리 최대 페이지 수 연산
			int iMaxPageNum = GetMaxLineNumPerCategory( m_NowInventorySortType ) / MY_INVENTORY_MAX_SLOT_HEIGHT;	/// 슬롯( 56개 )으로 꽉 찬 페이지 수 연산
			if( 0 < GetMaxLineNumPerCategory( m_NowInventorySortType ) % MY_INVENTORY_MAX_SLOT_HEIGHT )				/// 이외의 꽉 채우지 못한 페이지가 있는지 연산
				++iMaxPageNum;	/// 페이지 수 추가

			iChangePageNum = min( (iNowPageNum + 1) , iMaxPageNum );
			m_SumDelta += WHEEL_DELTA;
		}

		// 페이지 같다면 처리하지 않기
		if( iNowPageNum == iChangePageNum )
			return true;

		WCHAR wBuf[32];
		StringCchPrintfW( wBuf, ARRAY_SIZE(wBuf), L"Inventory_page%d", iChangePageNum );
		CKTDGUIRadioButton* pRadioButton = 
			static_cast<CKTDGUIRadioButton*>( m_pDLGUIInventory->GetControl( wBuf ) );	/// 페이지 1
		if( NULL != pRadioButton )
			pRadioButton->SetChecked(true);

		SetNowInventoryPageNum( m_NowInventorySortType, iChangePageNum );
		SetInventorySort( m_NowInventorySortType, iChangePageNum );

		break;
	}
	return true;
}
bool CX2UIInventory::GetIsMouseOver()
{
	if( false == GetShow() )
		return false;

	if( NULL != m_pDLGUIInventory ) 
		return m_pDLGUIInventory->GetIsMouseOver(); 

	return false; 
}
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장
/** @function : IsAddSocketSlotItem
	@brief : 소켓 슬롯 확장 아이템 여부 확인
*/
bool CX2UIInventory::IsAddSocketSlotItem( const int ItemID_ ) const
{
	switch( ItemID_ )
	{
	case ITEM_ID_ADD_WEAPON_SOCKET_SLOT:
	case ITEM_ID_ADD_DEFENCE_SOCKET_SLOT:
		{
			return true;
		} break;
	default:
		return false;
		break;
	}
	return false;
}
/** @fucntion : UseAddSocketSlotItem
	@brief : 소켓 슬롯 확장 아이템 사용
*/
void CX2UIInventory::UseAddSocketSlotItem( UidType iConsumeItemUID_ )
{
	if( iConsumeItemUID_ > 0 ) 
	{
		m_iConsumeItemUID = iConsumeItemUID_;

		// 마우스 커서 변경
		CX2State* pState = static_cast<CX2State*>(g_pMain->GetNowState());
		if( NULL != pState && NULL != pState->GetCursor() )
		{
			pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_ADD_SOCKET_SLOT );
		}

		ChangeInventoryTab( CX2Inventory::ST_EQUIP );

		return;
	}
}
void CX2UIInventory::Handler_EGS_SOCKET_EXPAND_ITEM_REQ()
{
	KEGS_SOCKET_EXPAND_ITEM_REQ kPacket;
	kPacket.m_iMaterialItemUID = m_iConsumeItemUID;
	kPacket.m_iTargetItemUID = m_iTargetItemUID;

	m_iConsumeItemUID = -1;
	m_iTargetItemUID = -1;
 	
	g_pData->GetServerProtocol()->SendPacket( EGS_SOCKET_EXPAND_ITEM_REQ, kPacket );
 	g_pMain->AddServerPacket( EGS_SOCKET_EXPAND_ITEM_ACK );
}
bool CX2UIInventory::Handler_EGS_SOCKET_EXPAND_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
	DeSerialize( pBuff, &kPacket );
	
	if( true == g_pMain->DeleteServerPacket( EGS_SOCKET_EXPAND_ITEM_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kPacket.m_iOK ) )
		{
			if( NULL != g_pData && 
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList(kPacket.m_vecUpdatedInventorySlot);
			}
			UpdateInventorySlotList( kPacket.m_vecUpdatedInventorySlot );

 			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_835 ), g_pMain->GetNowState() );
			return true;
		}		
	}
	return false;
}
#endif // ADD_SOCKET_SLOT

#ifdef REFORM_SKILL_NOTE_UI // 기술의 노트 버튼 제거하지 않고 비활성화로 변경
void CX2UIInventory::SetEnableBuySkillNote( bool bVal )
{
	if( m_pDLGUIInventory == NULL )
		return;

	CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGUIInventory->GetControl(L"Button_Buy_Skill_Note");
	if( pButton != NULL )
	{
		pButton->SetEnable(bVal);
	}
}
void CX2UIInventory::RegisterMemo( const CX2Item::ItemTemplet* pItemTemplet_, UidType uiMemoItemUID_ )
{
	if( NULL == pItemTemplet_ )
		return;;

	if( 0 >= uiMemoItemUID_ )
		return;

	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		const CX2Unit* pSelectUnit = g_pData->GetMyUser()->GetSelectUnit();

		if( GetX2UnitClassCompatibility( pSelectUnit->GetClass(), pItemTemplet_->GetUnitClass() ) == false )
		{
			// 메모 사용 유닛이 맞지 않음
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5013), g_pMain->GetNowState() );
			return;
		}

		if( pItemTemplet_->GetUseLevel() > g_pData->GetSelectUnitLevel() )
		{
			// 메모 사용레벨이 맞지 않음
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5014), g_pMain->GetNowState() );
			return;
		}

		if( pSelectUnit->GetMaxSkillNoteSlot() <= 0)
		{
			// 기술의 노트 사용 불가상태
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4988 ),g_pMain->GetNowState() );
			return;
		}

		if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME || 
			g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		{		
			// 게임중 메모사용 불가
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4989 ),g_pMain->GetNowState() );
			return;
		}
	}


	if( g_pData->GetUIManager() != NULL )
	{
#ifndef REFORM_SKILL_NOTE_UI// 메모 즉시 등록되도록 변경
		if( g_pData->GetUIManager()->GetUISkillNote() != NULL &&
			g_pData->GetUIManager()->GetUISkillNote()->GetUpdateMemo() == true )
		{						
			g_pData->GetUIManager()->GetUISkillNote()->UpdateMemoSlot( (int)g_pData->GetUIManager()->GetUISkillNote()->GetUpdatePage() );
			g_pData->GetUIManager()->GetUISkillNote()->ResetSkillNote();

			return;
		}
#endif // REFORM_SKILL_NOTE_UI

		// 메모등록					
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_NOTE, true);
		if( g_pData->GetUIManager()->GetUISkillNote() != NULL )
		{
			CX2State* pState = static_cast<CX2State*>(g_pMain->GetNowState());
			if( NULL != pState && NULL != pState->GetCursor())
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_REGISTER_MEMO );

			g_pData->GetUIManager()->GetUISkillNote()->RegisterMemo( uiMemoItemUID_ );
		}

		return;					
	}				

	// 메모사용 불가
}
#endif // REFORM_SKILL_NOTE_UI

#ifdef EXCHANGE_OPEN_IMAGE
void CX2UIInventory::OpenRandomItem( int iItemID, ITEM_OBTAIN_RESULT_TYPE eIORT )
{
	m_fCubeOpenTime = 3.f;
	m_RandomItemID = iItemID;
	m_bHideCubeOpneBar = g_pData->GetCubeOpenImageManager()->IsHideBarCube( iItemID );

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	m_eItemObtainResultType = eIORT;
#else
	m_bCubeOpen = true;
#endif RANDOM_EXCHANGE_RESULT_VIEW
	m_bCubeJustOpen = true;
	SAFE_DELETE_DIALOG(m_pDLGCubeOpenEffect);
	m_pDLGCubeOpenEffect = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Cube_Open.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCubeOpenEffect );
	m_pDLGCubeOpenEffect->SetFront(true);

#ifdef VARIOUS_RANDOM_CUBE_UI_NEW
	// oasis907 : 김상윤 [2010.9.29] // ITEM ID 별 예외 랜덤 아이템 큐브 열기 UI 
	CKTDGUIStatic* pStaticCubeOpenLoading = (CKTDGUIStatic*) m_pDLGCubeOpenEffect->GetControl( L"g_pStaticNote_Loading" );
	switch( iItemID )
	{
	case MEMO_RANDOM_CUBE_ITEM_ID:
	case CAPSULE_RANDOM_CUBE_ITEM_ID:
		pStaticCubeOpenLoading->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture43.tga", L"Note");
		break;
		//{{ oasis907 : 김상윤 [2011.3.29] // 식목일 큐브 아이템
	case TREE_DAY_CUBE_ITEM_ID:
		{
			pStaticCubeOpenLoading->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture50.tga", L"Note");
			m_pDLGCubeOpenEffect->AddDummyInt(1);
		}
		break;
		//}}
#ifdef NEW_HENIR_TEST
	case NEW_HENIR_CUBE_ITEM_ID:
		pStaticCubeOpenLoading->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture59.tga", L"Note");
		break;
#endif NEW_HENIR_TEST
	default:
		pStaticCubeOpenLoading->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture40.tga", L"Note");
		break;
	}
#endif VARIOUS_RANDOM_CUBE_UI_NEW

	// 초기 이미지를 그려주는데, 프레임부브에서 바로 교체를 해주니, 여기서는 이미지 셋팅은 안해줘도 되지 않나?
	if( g_pData->GetCubeOpenImageManager() != NULL && g_pData->GetCubeOpenImageManager()->IsEventCube( iItemID ) == true)
	{
		g_pData->GetCubeOpenImageManager()->SetStart( true);
		g_pData->GetCubeOpenImageManager()->SetSoundPlaying( true );
	}
	if ( m_pDLGRandomItem != NULL )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRandomItem, NULL, false );
		m_pDLGRandomItem = NULL;
	}
	InvalidSlotDesc();
}
#endif EXCHANGE_OPEN_IMAGE

//{{ 2012. 10. 08  할로윈 가열기
#ifdef SERV_HALLOWEEN_ICE_HEATER
void CX2UIInventory::UpdateOpenHalloWeenItemAck()
{
	m_eRandomItemEventType = RIOET_NONE;

	// 이전에 만들어져있던 Attraction Item의 결과창을 재활용한다. 
	// 메시지 (완료시의 CustomMsg)등도 가열기 결과창의 그것을 따른다.
	// 제목이랑 보너스 아이템 스태틱만 안 보이게 지워주면 될 듯.

	// 예전 창을 지우고..
	if ( m_pDLGOpenAttraction != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGOpenAttraction, NULL, false );
	m_pDLGOpenAttraction = NULL;

	// 새로 열려라
	m_pDLGAttractionResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Charm_Item_Result_1slot.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttractionResult );

#ifdef UNIQUENESS_EU_ONLY
	const wstring wstrResultDialogTitle		= 
		NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrResultDialogTitleTexture : L"DLG_UI_Title02_A.tga";
#else //UNIQUENESS_EU_ONLY
	const wstring wstrResultDialogTitle		= 
	#ifdef CLIENT_COUNTRY_US
		NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrResultDialogTitleTexture : L"DLG_UI_Title02_B.tga";	
	#else //CLIENT_COUNTRY_US
		NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrResultDialogTitleTexture : L"DLG_UI_Title02.tga";	
	#endif //CLIENT_COUNTRY_US
#endif 	//UNIQUENESS_EU_ONLY	

	const wstring wstrResultDialogTitleKey	= 
		NULL != m_pNowIceHeaterEventData ? m_pNowIceHeaterEventData->m_wstrResultDialogTitleTextureKey : L"heater_title";

	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGAttractionResult->GetControl( L"Static_Title" ) );
	if( NULL != pStatic )
	{
		if( NULL != pStatic->GetPicture(0) )
			pStatic->GetPicture(0)->SetTex( wstrResultDialogTitle.c_str(), wstrResultDialogTitleKey.c_str() );

		if( NULL != pStatic->GetPicture(1) )
			pStatic->GetPicture(1)->SetShow(false);
	}

	/// 다 사용했으니, 정보 객체 해제
	m_pNowIceHeaterEventData = NULL;

	// 보너스 아이템 스태틱 안 보이게 해주고
	CKTDGUIStatic* pStatic_BonusMark = (CKTDGUIStatic*)m_pDLGAttractionResult->GetControl( L"Static_Bonus_Item_Mark" );
	pStatic_BonusMark->SetShow(false);

	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
	{
		// 식목일 이벤트 아이템은 최대 2개 나온다 ( 기획팀에서 확인한 사항임 )
		std::map< int, int >::iterator i;

		bool bFirstItem = true;
		wstring getExplanation = L"";
		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
		{
			int resultItemID = i->first;
			int resultNum	= i->second;
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
			if ( pItemTemplet == NULL ) continue;

			CKTDGUIStatic* pStatic = NULL;
			if(bFirstItem)
			{
				// 첫 번째 아이템
				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Item" );
#ifdef ELLIPSE_GLOBAL
				// [NOTE] 결과창에 아이탬 이름이 다이얼로그를 넘어가는 경우가 있어서 말줄임(...) 적용
				bool bEllipse = false;
				wstring wstrItemName = CWordLineHandler::CutStringWithEllipse(pItemTemplet->GetFullName_(), 290, pStatic->GetString(0)->fontIndex, 1, bEllipse);
				const wchar_t* pwszitemName = wstrItemName.c_str();
#else //ELLIPSE_GLOBAL
				const wchar_t* pwszitemName = pItemTemplet->GetFullName_();
#endif //ELLIPSE_GLOBAL
				if( resultNum == 1)
				{
					pStatic->GetString(0)->msg = pwszitemName;
				}
				else
				{
					pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_832, "Si", pwszitemName, resultNum ) );
				}
				bFirstItem = false;
			}
			else
			{
				// 두번째
				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Bonus" );
				const wchar_t* pwszitemName = pItemTemplet->GetFullName_();
				pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_832, "Si", pwszitemName, resultNum ) );
			}

			if( NULL != pStatic && 
				NULL != pStatic->GetPicture(0) &&
				false == pStatic->GetPicture(0)->SetTex( 
				pItemTemplet->GetShopImage()
				) )
			{
				pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}
		}
	}

	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
}
#endif SERV_HALLOWEEN_ICE_HEATER

#ifdef SERV_RELATIONSHIP_EVENT_INT
void CX2UIInventory::Handler_EGS_USE_PROPOSE_ITEM_REQ( UidType iItemUID, wstring wstrNickName )
{
	KEGS_USE_PROPOSE_ITEM_REQ kPacket;
	kPacket.m_iUsedItemUID = iItemUID;
	kPacket.m_wstrNickName = wstrNickName;

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	userData.iOpponentUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	
	wstring wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_24459, "L", wstrNickName ) );
	
	IF_EVENT_ENABLED( CEI_RELATIONSHIP_CODE_EVENT )
	{
		g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE,
			CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 10.0f,
			wstrTextNotice.c_str(),
			(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_USE_PROPOSE_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_USE_PROPOSE_ITEM_ACK );
}

bool CX2UIInventory::Handler_EGS_USE_PROPOSE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_USE_PROPOSE_ITEM_ACK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( true == g_pMain->DeleteServerPacket( EGS_USE_PROPOSE_ITEM_ACK ) )
	{		
		if( true == g_pMain->IsValidPacket( kPacket.m_iOK ) )
		{
			if( NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kPacket.m_vecKInventorySlotInfo );
			}
			UpdateInventorySlotList( kPacket.m_vecKInventorySlotInfo );
		}

		return true;
	}

	return true;
}

void CX2UIInventory::Handler_EGS_USE_DIVORCE_ITEM_REQ()
{
	if( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().GetCouple() == true )
	{
		g_pData->GetServerProtocol()->SendID( EGS_USE_DIVORCE_ITEM_REQ );
		g_pMain->AddServerPacket( EGS_USE_DIVORCE_ITEM_ACK );
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_28094 ), g_pMain->GetNowState() );
	}
}

bool CX2UIInventory::Handler_EGS_USE_DIVORCE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_USE_DIVORCE_ITEM_ACK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( true == g_pMain->DeleteServerPacket( EGS_USE_DIVORCE_ITEM_ACK ) )
	{		
		if( true == g_pMain->IsValidPacket( kPacket.m_iOK ) )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetCouple( false );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( 0 );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( L"" );

			if( NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kPacket.m_vecUpdatedInventorySlot );
			}
			UpdateInventorySlotList( kPacket.m_vecUpdatedInventorySlot );

			OpenRandomItem( 60007792, IORT_CUBE_OPEN );

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_28093 ), g_pMain->GetNowState() );
			return true;
		}		
	}

	return true;
}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_CUBE_IN_ITEM_MAPPING
bool CX2UIInventory::Handler_EGS_CUBE_IN_ITEM_MAPPING_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CUBE_IN_ITEM_MAPPING_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	std::map< int,std::map<int, kRandomItemMappingToInfoClient> >		m_mapUseOpenRandomID;		// 마우스 오른 클릭으로 열었던 아이템 큐브 아이템과 그룹ID저장
	m_mapUseOpenRandomID.clear();
	m_mapUseOpenRandomID = kEvent.m_mapCubeInItemMapping;

	CX2ItemSlotManager::SetToolTipUseOpenRandomID(m_mapUseOpenRandomID);

	return true;
}
#endif //SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_RECRUIT_EVENT_BASE
bool CX2UIInventory::Handler_EGS_USE_RECRUIT_TICKET_REQ( UidType iUid )
{
	KEGS_USE_RECRUIT_TICKET_REQ kPacket;
	kPacket.m_iItemUID = iUid;

	g_pData->GetServerProtocol()->SendPacket( EGS_USE_RECRUIT_TICKET_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_USE_RECRUIT_TICKET_ACK );

	return true;
}
bool CX2UIInventory::Handler_EGS_USE_RECRUIT_TICKET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_RECRUIT_TICKET_ACK kEGS_USE_RECRUIT_TICKET_ACK;
	DeSerialize( pBuff, &kEGS_USE_RECRUIT_TICKET_ACK );

	g_pMain->DeleteServerPacket( EGS_USE_RECRUIT_TICKET_ACK );
	{
		if( g_pMain->IsValidPacket( kEGS_USE_RECRUIT_TICKET_ACK.m_iOK ) == true )
		{
			OpenRecommendMsgBox( true );
			return true;
		}
	}

	return false;
}

void CX2UIInventory::OpenRecommendMsgBox( bool bOpen )
{
	if ( bOpen == true )
	{
		if ( m_pDLGRecommend != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRecommend, NULL, false );

		m_pDLGRecommend = NULL;

		m_pDLGRecommend = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Recommend_Event.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRecommend );

		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGRecommend->GetControl( L"g_pIMEEditBoxName" );
		if ( pIMEEditBox != NULL )
		{
			pIMEEditBox->RequestFocus();
		}
	}
	else
	{
		if ( m_pDLGRecommend != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRecommend, NULL, false );

		m_pDLGRecommend = NULL;
	}
}

bool CX2UIInventory::Handler_EGS_REGISTER_RECRUITER_REQ( const WCHAR* wszNickname, bool bCancel )
{
	KEGS_REGISTER_RECRUITER_REQ kPacket;
	kPacket.m_wstrNickname = wszNickname;
	kPacket.m_wstrNickname = kPacket.m_wstrNickname.substr( 0, 20 );

	if ( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, kPacket.m_wstrNickname.c_str() ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_717 ), g_pMain->GetNowState() );
		return true;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_REGISTER_RECRUITER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_REGISTER_RECRUITER_ACK );

	return true;
}

bool CX2UIInventory::Handler_EGS_REGISTER_RECRUITER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_REGISTER_RECRUITER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_REGISTER_RECRUITER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// Handler_EGS_USE_ITEM_IN_INVENTORY_ACK
			g_pData->GetMyUser()->GetSelectUnit()->GetInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}
#ifdef SERV_REFORM_INVENTORY_TEST
			SetInventorySort(m_NowInventorySortType, GetNowInvenSortTypePageNum());
#else
			SetInventorySort(m_NowInventorySortType, m_NowInvenSortTypePageNum);
#endif

			OpenRecommendMsgBox( false );

			return true;
		}
	}

	return false;
}
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
void CX2UIInventory::InitItemObtainResult2( const WCHAR* pFileName, std::map< int, int >& mapInsertedItem, int iSlotNum /* = RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM */ )
{
	m_pDLGResolveItemResult = new CKTDGUIDialog( g_pMain->GetNowState(), pFileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItemResult );

	//아이템 획득 결과창의 위치 설정
	m_pDLGResolveItemResult->SetPos( D3DXVECTOR2( 320, 230 ) );

	CKTDGUIButton* pControl = (CKTDGUIButton*) m_pDLGResolveItemResult->GetControl( L"Button_OK" );
	if ( pControl != NULL )
	{
		pControl->RequestFocus();
	}

	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_Slot_Pic" );
	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotName" );
	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"Static_SlotNum" );

	//아이템 획득 결과창의 아이템 이미지와 스트링 초기화
	for ( int slotNum = 0; slotNum < iSlotNum; ++slotNum )
	{
		if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
		{
			pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
		}

		pStaticName->GetString(slotNum*2)->msg = L"";
		pStaticName->GetString(slotNum*2 + 1)->msg = L"";

		if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
		{
			pStaticNum->GetString(slotNum)->msg = L"";
		}
	}

	//획득한 아이템으로 아이템 슬롯 설정
	int index = 0;
	std::map< int, int >::iterator it = mapInsertedItem.begin(); 
	while( it != mapInsertedItem.end() )
	{
		int resultItemID = it->first;

		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
		if ( pItemTemplet != NULL )
		{
			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(index) != NULL )
			{
				const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
				if ( pwszShopImage[0] != NULL )
					pStaticSlot->GetPicture(index)->SetTex( pwszShopImage );
				else
					pStaticSlot->GetPicture(index)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			}

			bool bCheckTwoLine = false;
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring wstrFullName = CWordLineHandler::GetStrByLineBreakInX2Main( pItemTemplet->GetFullName_(), 145, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
			wstring wstrFullName = g_pMain->GetStrByLienBreak( 
				pItemTemplet->GetFullName_(), 
				145, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
			if ( wstrFullName.find( L"\n") != -1 )
				bCheckTwoLine = true;

			if(pStaticName->GetString(index*2) != NULL)
			{
				if ( bCheckTwoLine == false )
				{
					pStaticName->GetString(index*2)->msg = wstrFullName;
					pStaticName->GetString(index*2+1)->msg = L"";
				}
				else
				{
					pStaticName->GetString(index*2)->msg = L"";
					pStaticName->GetString(index*2+1)->msg = wstrFullName;
				}
			}
			if ( pStaticNum != NULL && pStaticNum->GetString(index) != NULL )
			{
				wstring wstrCount = GET_REPLACED_STRING( ( STR_ID_865, "i", it->second) );
				pStaticNum->GetString(index)->msg = wstrCount.c_str();
			}
		}
		// 아이템이 없는 경우
		else
		{
			pStaticSlot->GetPicture(index)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
			pStaticName->GetString(index)->msg = L"";
			pStaticNum->GetString(index)->msg = L"";
		}
		++it;
		++index;
	}
}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT