#include "StdAfx.h"
#include ".\x2worldmapui.h"





CX2WorldMapUI::CX2WorldMapUI(void) :
m_pStage( NULL ),
m_pDLGWorldMap( NULL ),
m_pDLGWorldMapFront( NULL ),
m_pDLGFieldMap( NULL ),
m_pWorldFieldMap( NULL ),
m_pDLGNPCToolTip( NULL ),
m_pDLGPartyToolTip(NULL),
m_pDLGMiscToolTip( NULL ),
m_iCurrFieldMapID( 0 )
, m_bZoomed( false )
, m_bBeforeField( false )
{
}

CX2WorldMapUI::~CX2WorldMapUI(void)
{
	SAFE_DELETE_DIALOG( m_pDLGWorldMap );
	SAFE_DELETE_DIALOG( m_pDLGWorldMapFront );
	// SAFE_DELETE_DIALOG( m_pDLGFieldMap );	// m_mapDLGFieldMap에 SAFE_DELETE_DIALOG를 수행하기 때문에 필요 없음
	SAFE_DELETE_DIALOG( m_pDLGNPCToolTip );
	SAFE_DELETE_DIALOG( m_pDLGPartyToolTip );
	SAFE_DELETE_DIALOG( m_pDLGMiscToolTip );	

	CX2WorldFieldMap::DeleteKTDGObject( (CKTDGObject*) m_pWorldFieldMap );

	map< int, CKTDGUIDialogType >::iterator it;
	for( it = m_mapDLGFieldMap.begin(); it != m_mapDLGFieldMap.end(); it++ )
	{
		SAFE_DELETE_DIALOG( it->second );
	}
	m_mapDLGFieldMap.clear();
}

/*virtual*/ bool CX2WorldMapUI::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case WMUCM_VILLAGE_CLICKED:
	case WMUCM_DUNGEON_GATE_CLICKED:
	case WMUCM_PREV_FIELD_MAP:
	case WMUCM_NEXT_FIELD_MAP:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			int iFieldID = pButton->GetDummyInt(0);

			OpenFieldMap( true, iFieldID );
			m_bZoomed = true;
			UpdateFieldMap();		
			return true;
		} break;

	case WMUCM_CLOSE_WORLD_MAP:
		{
			OpenWorldMap( false );
			OpenFieldMap( false );
			CloseAllToolTip();

			m_bZoomed = true;
			return true;
		} break;
			
	case WMUCM_FIELD_RIGHT_CLICK:
		{
			int iFieldID = -1;
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_SQUARE_GAME:
				iFieldID = g_pData->GetLocationManager()->GetCurrentVillageID();
				break;
			case CX2Main::XS_BATTLE_FIELD:
				iFieldID = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();
				break;
			}				
			
			OpenFieldMap( false, iFieldID );
			CloseAllToolTip();
			m_bZoomed = false;
			UpdateFieldMap();

			CKTDGUIDialogType pDialog = (CKTDGUIDialog*) lParam;

			if ( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( pDialog ) )
				m_bBeforeField = true;
			else
				m_bBeforeField = false;

			return true;
		}
		break;

	case WMUCM_WORLD_RIGHT_CLICK:
		{
			if ( false == m_bBeforeField )
			{
				OpenWorldMap( false );
				OpenFieldMap( false );
				CloseAllToolTip();

				m_bBeforeField = true;
				m_bZoomed = true;
			}
			else
			{
				m_bBeforeField = false;
			}

			return true;
		} break;

	case WMUCM_CLOSE_FIELD_MAP:
		{
			int iFieldID = -1;
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_SQUARE_GAME:
				iFieldID = g_pData->GetLocationManager()->GetCurrentVillageID();
				break;
			case CX2Main::XS_BATTLE_FIELD:
				iFieldID = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();
				break;
			}
			
			OpenFieldMap( false, iFieldID );
			OpenWorldMap( false );
			CloseAllToolTip();
			m_bZoomed = false;
			UpdateFieldMap();
			return true;
		} break;

	case WMUCM_NPC_MOUSE_OVER:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			int iNPCID = pButton->GetDummyInt(0);

			OpenNPCToolTip( true, iNPCID, pButton->GetPos() + pButton->GetDialog()->GetPos() );
			return true;
		} break;

	case WMUCM_NPC_MOUSE_OUT:
		{
			OpenNPCToolTip( false );
			return true;
		} break;
		//{{ 2009.2.13 김태완 : 파티원 정보표시
	case WMUCM_MYCHAR_MOUSE_OVER:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;

			//			D3DXVECTOR2 vPos;
			// 			vPos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
			// 			vPos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
			// 			vPos = g_pKTDXApp->MouseConvertByResolution( vPos );

			OpenPartyMemberToolTip( true, -1, pButton->GetPos() + pButton->GetDialog()->GetPos() );			

		} break;
	case WMUCM_PARTY_MEMBER_MOUSE_OVER:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			int iPartyIndex = pButton->GetDummyInt(0);

			OpenPartyMemberToolTip( true, iPartyIndex, pButton->GetPos() + pButton->GetDialog()->GetPos());

		} break;
	case WMUCM_MYCHAR_MOUSE_OUT:
	case WMUCM_PARTY_MEMBER_MOUSE_OUT:
		{
			OpenPartyMemberToolTip(false);

		} break;
		//}}
		//{{ 2009.02.12 김태완
		// Navi 관련 코드
	case WMUCM_NAVI_LEFT_CLICKED:
		{
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			D3DXVECTOR2 vPos;

			if ( true == m_bZoomed )
			{
				map< int, CKTDGUIDialogType >::iterator it = m_mapDLGFieldMap.find( m_iCurrFieldMapID );
				if( it != m_mapDLGFieldMap.end() )
				{
					vPos = it->second->GetSize();

					vPos.x *= vRelativePos.x;
					vPos.y *= vRelativePos.y;

					vPos.x = 0.f - vPos.x;
					vPos.y = 0.f - vPos.y;

					it->second->SetPos( vPos );
				}	
			}
			else
			{
				vPos = m_pDLGWorldMap->GetSize();
				
				vPos.x *= vRelativePos.x;
				vPos.y *= vRelativePos.y;

				vPos.x = 0.f - vPos.x;
				vPos.y = 0.f - vPos.y;

				m_pDLGWorldMap->SetPos( vPos );
			}

			return true;

		} break;
	case WMUCM_NAVI_LEFT_DRAGGING:
		{
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			D3DXVECTOR2 vPos;

			if ( true == m_bZoomed )
			{
				map< int, CKTDGUIDialogType >::iterator it = m_mapDLGFieldMap.find( m_iCurrFieldMapID );
				if( it != m_mapDLGFieldMap.end() )
				{
					vPos = it->second->GetSize();

					vPos.x *= vRelativePos.x;
					vPos.y *= vRelativePos.y;

					vPos.x = 0.f - vPos.x;
					vPos.y = 0.f - vPos.y;

					it->second->SetPos( vPos );
				}	
			}
			else
			{
				vPos = m_pDLGWorldMap->GetSize();

				vPos.x *= vRelativePos.x;
				vPos.y *= vRelativePos.y;

				vPos.x = 0.f - vPos.x;
				vPos.y = 0.f - vPos.y;

				m_pDLGWorldMap->SetPos( vPos );
			}

			return true;

		} break;
	case WMUCM_NAVI_FIELD_MAP_STOP_DRAGGING:
		{
			if( m_pDLGWorldMapFront != NULL && m_pDLGWorldMap != NULL )
			{

				CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGWorldMapFront->GetControl( L"WorldMap_Navi" );
				if( NULL != pNavi )
				{
					D3DXVECTOR2 vPos;

					UpdateNaviMap( vPos, pNavi );

					pNavi->SetRelativeWindowPos( vPos );
				}
			}

			return true;

		} break;
	case WMUCM_NAVI_WORLD_MAP_STOP_DRAGGING:
		{
			if( m_pDLGWorldMapFront != NULL && m_pDLGWorldMap != NULL )
			{

				CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGWorldMapFront->GetControl( L"WorldMap_Navi" );
				if( NULL != pNavi )
				{
					D3DXVECTOR2 vPos( 0, 0 );
					UpdateNaviMap( vPos, pNavi );

					pNavi->SetRelativeWindowPos( vPos );
				}
			}

			return true;

		} break;
		//}}
	case WMUCM_POSTBOX_MOUSE_OVER:
	case WMUCM_MARKET_MOUSE_OVER:
		//{{ kimhc // 2010.5.4 // 비밀던전 작업(일일퀘스트)
#ifndef SERV_SECRET_HELL
	case WMUCM_BILLBOARD_MOUSE_OVER:
#endif SERV_SECRET_HELL
		//}} kimhc // 2010.5.4 // 비밀던전 작업(일일퀘스트)

		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			OpenMiscToolTip( true, pButton->GetPos() + pButton->GetDialog()->GetPos(), (WORLD_MAP_UI_CUSTOM_MSG)wParam );
			return true;
		} break;
	case WMUCM_OTHER_FIELD_MOUSE_OVER:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			int iFieldID = pButton->GetDummyInt(0);
			OpenMiscToolTip( true, pButton->GetPos() + pButton->GetDialog()->GetPos(), (WORLD_MAP_UI_CUSTOM_MSG)wParam, iFieldID );
			return true;
		} break;

	case WMUCM_VILLAGE_MOUSE_OVER:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			int iFieldID = pButton->GetDummyInt(0);
// 			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
// 												(float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() );
			OpenMiscToolTip( true, pButton->GetPos() + pButton->GetDialog()->GetPos(), (WORLD_MAP_UI_CUSTOM_MSG)wParam, iFieldID );

			return true;
		} break;
	case WMUCM_VILLAGE_MOUSE_OUT:
	case WMUCM_POSTBOX_MOUSE_OUT:
	case WMUCM_MARKET_MOUSE_OUT:
	case WMUCM_OTHER_FIELD_MOUSE_OUT:
		//{{ kimhc // 2010.5.4 // 비밀던전 작업(일일퀘스트)
#ifndef SERV_SECRET_HELL
	case WMUCM_BILLBOARD_MOUSE_OUT:
#endif SERV_SECRET_HELL
		//}} kimhc // 2010.5.4 // 비밀던전 작업(일일퀘스트)

		{
			OpenMiscToolTip(false);
			return true;
		} break;
	}


	return false;
}

void CX2WorldMapUI::OpenWorldMap( bool bOpen )
{
	if( false == bOpen )
	{
		if( m_pDLGWorldMap != NULL )
		{
			m_pDLGWorldMap->SetShowEnable( bOpen, bOpen );
		}

		if( m_pDLGWorldMapFront != NULL )
		{
			m_pDLGWorldMapFront->SetShowEnable( bOpen, bOpen );
		}
		CloseAllToolTip();

		// 채팅창 셋팅
		g_pChatBox->SetChatBoxLayerUp(false);
		g_pChatBox->OpenChatWindow();
		g_pChatBox->CloseChatWindowButton();;	
		g_pChatBox->HideChatEditBox();
	}
	else
	{
		if( NULL == m_pStage )
		{
			m_pStage = (CKTDXStage*) g_pMain->GetNowState();
		}


		if( m_pDLGWorldMap == NULL )
		{
			m_pDLGWorldMap = new CKTDGUIDialog( m_pStage, L"DLG_UI_World_Map_New.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWorldMap );
		}

		if( m_pDLGWorldMapFront == NULL )
		{
			m_pDLGWorldMapFront = new CKTDGUIDialog( m_pStage, L"DLG_UI_World_Map_Front_NEW.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWorldMapFront );
		}

		m_pDLGWorldMap->SetPos( D3DXVECTOR2( 0, 0 ) );
		m_pDLGWorldMap->SetShowEnable( bOpen, bOpen );
		m_pDLGWorldMapFront->SetShowEnable( bOpen, bOpen );

		// 채팅창 셋팅
		g_pChatBox->SetChatBoxLayerUp(true);
		g_pChatBox->HideChatWindowFrame();
		g_pChatBox->HideChatWindow();
		g_pChatBox->HideChatEditBox();			
		g_pChatBox->CloseChatWindow();
		g_pChatBox->OpenChatWindowButton();

		// 펫 팝업 제거
		if ( NULL != g_pData->GetPetManager() )
		{
			CX2PET *pPet = g_pData->GetPetManager()->GetMyPet();

			if ( NULL != pPet && true == pPet->GetOnPopup() )
			{
				g_pData->GetPetManager()->ClosePetPopupMenu();
				pPet->SetOnPopup( false );
			}
		}
	}
}

void CX2WorldMapUI::UpdateWorldMap()
{
	// 아직 갈 수 없는 마을은 클릭 안되게
	if( m_pDLGWorldMap != NULL )
	{
		//{{ 2009.02.12 김태완 : 파티원 / 내 위치 표시 관련
		CKTDGUIButton* pButton_MyChar = (CKTDGUIButton*) m_pDLGWorldMap->GetControl( L"Button_Mychar" );
		CKTDGUIButton* pButton_PartyMember0 = (CKTDGUIButton*) m_pDLGWorldMap->GetControl( L"Button_PartyMember0" );
		CKTDGUIButton* pButton_PartyMember1 = (CKTDGUIButton*) m_pDLGWorldMap->GetControl( L"Button_PartyMember1" );
		CKTDGUIButton* pButton_PartyMember2 = (CKTDGUIButton*) m_pDLGWorldMap->GetControl( L"Button_PartyMember2" );
		if(pButton_MyChar != NULL)
			pButton_MyChar->SetShow(false);
		if(pButton_PartyMember0 != NULL)
			pButton_PartyMember0->SetShow(false);
		if(pButton_PartyMember1 != NULL)
			pButton_PartyMember1->SetShow(false);
		if(pButton_PartyMember2 != NULL)
			pButton_PartyMember2->SetShow(false);
		//}}

		for( int i=0; i<m_pDLGWorldMap->GetControlSize(); i++ )
		{
			CKTDGUIControl* pControl = m_pDLGWorldMap->GetControl( i );
			if( NULL == pControl )
				continue;

			if( CKTDGUIControl::UCT_BUTTON != pControl->GetType() )
				continue;

			CKTDGUIButton* pButton = (CKTDGUIButton*) pControl;
			if( WMUCM_VILLAGE_CLICKED != pButton->GetCustomMsgMouseUp() &&
				WMUCM_DUNGEON_GATE_CLICKED != pButton->GetCustomMsgMouseUp() )
			{
				continue;
			}

			if( -1 == pControl->GetDummyInt( 0 ) ) 
				continue;


			CX2LocationManager::LOCAL_MAP_ID eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( (SEnum::VILLAGE_MAP_ID) pControl->GetDummyInt( 0 ) );
			if( CX2LocationManager::LMI_INVALID == eLocalMapID )
				continue;

			CX2LocationManager::LocalMapTemplet* pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( eLocalMapID );
			if( NULL == pLocalMapTemplet )
			{
#ifdef SCRIPT_SELECT
				// "없는" 지역이다. (스크립트에서 빠져 있음)
				pControl->SetShow(false);
				// 같은 ID를 가진 스태틱을 꺼 준다
				for( int i=0; i<m_pDLGWorldMap->GetControlSize(); i++ )
				{
					CKTDGUIControl* pStaticControl = m_pDLGWorldMap->GetControl( i );
					if( NULL == pStaticControl )
						continue;

					if( CKTDGUIControl::UCT_STATIC != pStaticControl->GetType() )
						continue;

					if( pStaticControl->GetDummyInt(0) != pControl->GetDummyInt(0) )
						continue;

					pStaticControl->SetShow(false);

				}
#endif
				continue;
			}


#ifdef NO_SANDER_VILLIAGE
			if( CX2LocationManager::LMI_SANDER == eLocalMapID 
#ifdef SERV_NO_VELDER_VILLIAGE
				|| CX2LocationManager::LMI_VELDER == eLocalMapID 
#endif //SERV_NO_VELDER_VILLIAGE
#ifdef SERV_NO_HAMEL_VILLIAGE
				|| CX2LocationManager::LMI_HAMEL == eLocalMapID 
#endif //SERV_NO_HAMEL_VILLIAGE
				)
			{
				pControl->SetShow(false);

				for( int i=0; i<m_pDLGWorldMap->GetControlSize(); i++ )
				{
					CKTDGUIControl* pStaticControl = m_pDLGWorldMap->GetControl( i );
					if( NULL == pStaticControl )
						continue;

					if( CKTDGUIControl::UCT_STATIC != pStaticControl->GetType() )
						continue;

					if( pStaticControl->GetDummyInt(0) != pControl->GetDummyInt(0) )
						continue;

					if ( pStaticControl->GetDummyInt(1) == 1 )
					{
						pStaticControl->SetShow( true );
						continue;
					}

					pStaticControl->SetShow(false);
				}
				continue;
			}
#endif // NO_SANDER_VILLIAGE

			float PosOffset = 0.0f;
			SEnum::VILLAGE_MAP_ID eCurrentMapID = SEnum::VMI_INVALID;

			if ( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
				eCurrentMapID = g_pData->GetLocationManager()->GetCurrentVillageID();
			else if ( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
				eCurrentMapID = static_cast<SEnum::VILLAGE_MAP_ID>( g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam() );
			
			if( pControl->GetDummyInt(0) == eCurrentMapID )
			{
				D3DXVECTOR3 DummyPos = pControl->GetDummyPos(0);
				D3DXVECTOR2 CharMarkPos;
				CharMarkPos.x = DummyPos.x;
				CharMarkPos.y = DummyPos.y;
				pButton_MyChar->SetOffsetPos( CharMarkPos );
				pButton_MyChar->SetShow(true);
				PosOffset += (pButton_MyChar->GetWidth() + 1.0f);
			}

			// 파티가 있으면
			if( g_pData->GetPartyManager()->DoIHaveParty() )
			{
				// 파티원들을 하나씩 찾아보자..
				for(int i=0; i<3; i++ )
				{
					CX2PartyManager::PartyMemberData* pMemberData =
						g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( i, g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
					if(pMemberData != NULL)
					{
						// 해당 맵ID랑 파티원의 현재 맵ID랑 비교해서
						SEnum::VILLAGE_MAP_ID eCurrentVillageID = (SEnum::VILLAGE_MAP_ID) pMemberData->m_iStateCode;
						// 던전라운지면 던전게이트로 살짝 바꿔준다
						if(g_pData->GetLocationManager()->IsDungeonLounge(eCurrentVillageID))
						{
							CX2LocationManager::LOCAL_MAP_ID eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( eCurrentVillageID );
							eCurrentVillageID = g_pData->GetLocationManager()->GetDungeonGateID( eLocalMapID );
						}

						if( pControl->GetDummyInt(0) == eCurrentVillageID )
						{
							CKTDGUIButton* pButton_PartyMember;
							switch(i)
							{
							case 0:
								{
									pButton_PartyMember = pButton_PartyMember0;
								} break;
							case 1:
								{
									pButton_PartyMember = pButton_PartyMember1;
								} break;
							case 2:
								{
									pButton_PartyMember = pButton_PartyMember2;
								} break;

							}
							// 버튼에 해당하는 마을에 파티원이 있으면
							// 해당 버튼에 붙어있는 DummyPos를 가져와서 거기에 캐릭터 마크를 그린다
							D3DXVECTOR3 DummyPos = pControl->GetDummyPos(0);
							D3DXVECTOR2 CharMarkPos;
							CharMarkPos.x = DummyPos.x;
							CharMarkPos.y = DummyPos.y;
							CharMarkPos.x += PosOffset;
							// 							D3DXVECTOR2 CharMarkPos = pControl->GetNowPoint().leftTopPoint;
							// 							CharMarkPos.y -= ( pButton_PartyMember->GetHeight() + 1.0f);
							// 							CharMarkPos.x += PosOffset;
							pButton_PartyMember->SetOffsetPos( CharMarkPos );
							pButton_PartyMember->SetShow(true);
							PosOffset += (pButton_PartyMember->GetWidth() + 1.0f);
						}
					}
				}
			}
			//}}


			//if( pLocalMapTemplet->m_vecRequireClearDungeonID.size() > 0 )
			{
				if( /*false == g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( pLocalMapTemplet->m_vecRequireClearDungeonID ) ||*/
					g_pData->GetSelectUnitLevel() < pLocalMapTemplet->m_RequireUnitLevel )
				{
					// 갈 수 없는 지역이다..
					pControl->SetShow( false );
					// 같은 ID를 가진 스태틱을 꺼 준다
					for( int i = 0; i < m_pDLGWorldMap->GetControlSize(); ++i )
					{
						CKTDGUIControl* pStaticControl = m_pDLGWorldMap->GetControl( i );
						if( NULL == pStaticControl )
							continue;

						if( CKTDGUIControl::UCT_STATIC != pStaticControl->GetType() )
							continue;
						
						if( pStaticControl->GetDummyInt(0) != pControl->GetDummyInt(0) )
							continue;

						if ( pStaticControl->GetDummyInt(1) == 1 )
						{
							pStaticControl->SetShow( true );
							continue;
						}
												
						pStaticControl->SetShow(false);

					}

				}
				else
				{
					// 갈 수 있는 지역이다
					pControl->SetShow( true );
					// 같은 ID를 가진 스태틱을 켜 준다
					for( int i=0; i<m_pDLGWorldMap->GetControlSize(); i++ )
					{
						CKTDGUIControl* pStaticControl = m_pDLGWorldMap->GetControl( i );
						if( NULL == pStaticControl )
							continue;

						if( CKTDGUIControl::UCT_STATIC != pStaticControl->GetType() )
							continue;

						if( pStaticControl->GetDummyInt(0) != pControl->GetDummyInt(0) )
							continue;

						if ( pStaticControl->GetDummyInt(1) == 1 )
						{
							pStaticControl->SetShow( false );
							continue;
						}

						pStaticControl->SetShow(true);

					}
				}
			}


		}
	}

	//{{ 2009.02.12 김태완
	// 네비게이터를 맵 열릴 때 해당위치에 맞게 업데이트
	if( m_pDLGWorldMapFront != NULL && m_pDLGWorldMap != NULL )
	{

		CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGWorldMapFront->GetControl( L"WorldMap_Navi" );
		if( NULL != pNavi )
		{
			D3DXVECTOR2 vPos( 0, 0 );
			UpdateNaviMap( vPos, pNavi );

			pNavi->SetRelativeWindowPos( vPos );
		}
	}
	//}}
}

CKTDGUIDialogType CX2WorldMapUI::GetFieldMapDlg( int iFieldID )
{
	if( iFieldID < 0 )
		return NULL;

	m_iCurrFieldMapID = iFieldID;
	m_pStage = g_pMain->GetNowState();

	map< int, CKTDGUIDialogType >::iterator it = m_mapDLGFieldMap.find( iFieldID );
	if( it != m_mapDLGFieldMap.end() )
	{
		return it->second;
	}	
	
	CKTDGUIDialogType pDialog = NULL;

	// note!! 일단은 하드코딩
	switch( iFieldID )
	{
#pragma region 루벤
	case SEnum::VMI_RUBEN:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Ruben_Village_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Ruben_01_NEW.lua" );
		break;
#pragma endregion 월드맵 Lua

#pragma region 엘더
	case SEnum::VMI_ELDER:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Elder_Village_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_ELDER_FIELD_01:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Elder_01_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_ELDER_REST_00:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Elder_Rest_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_ELDER_FIELD_02:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Elder_02_NEW.lua" );
		break;
#pragma endregion 월드맵 Lua

#pragma region 베스마
	case SEnum::VMI_BESMA:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Besma_Village_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_01:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Besma_01_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_REST_00:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Besma_Rest_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_02:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Besma_02_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_03:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Besma_03_NEW.lua" );
		break;
#pragma endregion 월드맵 Lua

#pragma region 알테라
	case SEnum::VMI_ALTERA:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Altera_Village_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_01:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Altera_01_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_02:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Altera_02_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Altera_Rest_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_03:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Altera_03_NEW.lua" );
		break;
#pragma endregion 월드맵 Lua

#pragma region 페이타
	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_01:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Peita_01_NEW.lua" );
		break;
	case SEnum::VMI_PEITA:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Peita_Village_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_02:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Peita_02_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_03:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Peita_03_NEW.lua" );
		break;
#pragma endregion 월드맵 Lua

#pragma region 벨더
	case SEnum::VMI_VELDER:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Velder_Village_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_01:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Velder_01_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_02:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Velder_02_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_REST_00:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Velder_Rest_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_03:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Velder_03_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_04:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Velder_04_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_SHIP_STAGE:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Velder_Ship_NEW.lua" );
		break;
#pragma endregion 월드맵 Lua

#pragma region 하멜
	case SEnum::VMI_BATTLE_FIELD_HAMEL_SHIP_STAGE:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Hamel_Ship_NEW.lua" );
		break;
	case SEnum::VMI_HAMEL:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Hamel_Village_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Hamel_01_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Hamel_02_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Hamel_Rest_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Hamel_03_NEW.lua" );
		break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04:
		pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Hamel_04_NEW.lua" );
		break;
#pragma endregion 월드맵 Lua

#ifdef VILLAGE_SANDER 
	case SEnum::VMI_SANDER:
		{
			pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Sander_Village_NEW.lua" );
		} break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01:
		{
			pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Sander_01_NEW.lua" );
		} break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02:
		{
			pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Sander_02_NEW.lua" );
		} break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_REST_00:
		{
			pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Sander_Rest_NEW.lua" );
		} break;
#endif //VILLAGE_SANDER

#ifdef SANDER_DUNGEON_5_6
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03:
		{
			pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Sander_03_NEW.lua" );
		} break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04:
		{
			pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_FIELD_MAP_Sander_04_NEW.lua" );
		} break;
#endif //SANDER_DUNGEON_5_6

	default:
		ASSERT( !L"iFieldID is Invalid" );
		pDialog = NULL;
		break;
	}

	if( pDialog != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialog );
		m_mapDLGFieldMap.insert( make_pair( iFieldID, pDialog ) );
	}

	ASSERT( pDialog != NULL );

	m_bZoomed = true;

	return pDialog;
}

void CX2WorldMapUI::OpenFieldMap( bool bOpen, int iFieldID /*= -1*/ )
{
	// 09.03.10 김태완 : 던전라운지인 경우 열지 말자
	if(true == g_pData->GetLocationManager()->IsDungeonLounge( (SEnum::VILLAGE_MAP_ID)iFieldID ) )
	{
		bOpen = false;
	}


	if( bOpen == true && NULL == m_pWorldFieldMap )
	{
		m_pWorldFieldMap = CX2WorldFieldMap::CreateWorldFieldMap();
		g_pKTDXApp->GetDGManager()->AddObjectChain( (CKTDGObject*) m_pWorldFieldMap );
	}


	if( NULL != m_pWorldFieldMap )
	{
		m_pWorldFieldMap->SetVillageID( iFieldID );
		m_pWorldFieldMap->SetShowObject( bOpen );
	}

	CloseAllToolTip();

	if( false == bOpen )
	{
		if( m_pDLGFieldMap != NULL )
		{
			m_pDLGFieldMap->SetShowEnable( bOpen, bOpen );
		}
	}
	else
	{
		if( iFieldID == m_iCurrFieldMapID &&
			m_pDLGFieldMap != NULL )
		{
			m_pDLGFieldMap->SetShowEnable( bOpen, bOpen );
		}
		else
		{
			if( m_pDLGFieldMap != NULL )
			{
				m_pDLGFieldMap->SetShowEnable( false, false );
				m_pDLGFieldMap = NULL;
			}

			m_pDLGFieldMap = GetFieldMapDlg( iFieldID );
			if( m_pDLGFieldMap != NULL )
			{
				m_pDLGFieldMap->SetShowEnable( bOpen, bOpen );
			}
		}
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGWorldMapFront, true );
	if(m_pWorldFieldMap != NULL)
	{
		m_pWorldFieldMap->DestoryQuestNoticeEffect();

		if( m_pDLGFieldMap != NULL )
		{
			m_pWorldFieldMap->SetFieldDLG( m_pDLGFieldMap );
			m_pWorldFieldMap->InitCharacterMaker();
		}
	}
}

void CX2WorldMapUI::UpdateFieldMap()
{
	// 아직 갈 수 없는 마을은 클릭 안되게
	if( m_pDLGFieldMap != NULL )
	{
		//{{ 2009.02.13 김태완 : 파티원 / 내 위치 표시 관련
		/*CKTDGUIButton* pButton_MyChar = (CKTDGUIButton*) m_pDLGFieldMap->GetControl( L"Button_Mychar" );
		CKTDGUIButton* pButton_PartyMember0 = (CKTDGUIButton*) m_pDLGFieldMap->GetControl( L"Button_PartyMember0" );
		CKTDGUIButton* pButton_PartyMember1 = (CKTDGUIButton*) m_pDLGFieldMap->GetControl( L"Button_PartyMember1" );
		CKTDGUIButton* pButton_PartyMember2 = (CKTDGUIButton*) m_pDLGFieldMap->GetControl( L"Button_PartyMember2" );
		if(pButton_MyChar != NULL)
		pButton_MyChar->SetShow(false);
		if(pButton_PartyMember0 != NULL)
		pButton_PartyMember0->SetShow(false);
		if(pButton_PartyMember1 != NULL)
		pButton_PartyMember1->SetShow(false);
		if(pButton_PartyMember2 != NULL)
		pButton_PartyMember2->SetShow(false);*/
		//}}

		for( int i=0; i<m_pDLGFieldMap->GetControlSize(); i++ )
		{
			CKTDGUIControl* pControl = m_pDLGFieldMap->GetControl( i );
			if( NULL == pControl )
				continue;

			if( CKTDGUIControl::UCT_BUTTON != pControl->GetType() )
				continue;
			
			CKTDGUIButton* pButton = (CKTDGUIButton*) pControl;
			if( WMUCM_PREV_FIELD_MAP != pButton->GetCustomMsgMouseUp() &&
				WMUCM_NEXT_FIELD_MAP != pButton->GetCustomMsgMouseUp() &&
				WMUCM_DUNGEON_GATE_CLICKED != pButton->GetCustomMsgMouseUp() &&
				WMUCM_VILLAGE_CLICKED != pButton->GetCustomMsgMouseUp() 
				)
			{
				continue;
			}
			
			if( -1 == pControl->GetDummyInt( 0 ) ) 
				continue;


			CX2LocationManager::LOCAL_MAP_ID eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( (SEnum::VILLAGE_MAP_ID) pControl->GetDummyInt( 0 ) );
			if( CX2LocationManager::LMI_INVALID == eLocalMapID )
			{
#ifdef SCRIPT_SELECT
				pControl->SetShow(false);
#endif
				continue;
			}

			CX2LocationManager::LocalMapTemplet* pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( eLocalMapID );
			if( NULL == pLocalMapTemplet )
			{
#ifdef SCRIPT_SELECT
				pControl->SetShow(false);
#endif
				continue;
			}


			//if( pLocalMapTemplet->m_vecRequireClearDungeonID.size() > 0 )
			{
				if( /*false == g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( pLocalMapTemplet->m_vecRequireClearDungeonID ) ||*/
					g_pData->GetSelectUnitLevel() < pLocalMapTemplet->m_RequireUnitLevel )
				{
					pControl->SetEnable( false );
				}
				else
				{
					pControl->SetEnable( true );
				}
			}

			//{{ 파티원표시
			/*if( WMUCM_DUNGEON_GATE_CLICKED == pButton->GetCustomMsgMouseUp() ||
				WMUCM_VILLAGE_CLICKED == pButton->GetCustomMsgMouseUp() )
			{
				float PosOffset = 0.0f;
				SEnum::VILLAGE_MAP_ID eCurrentVillageID = g_pData->GetLocationManager()->GetCurrentVillageID();
				// 던전라운지면 던전게이트로 살짝 바꿔준다
				{
					CX2LocationManager::LOCAL_MAP_ID eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( eCurrentVillageID );
					eCurrentVillageID = g_pData->GetLocationManager()->GetDungeonGateID( eLocalMapID );
				}

				if(pControl->GetDummyInt(0) == eCurrentVillageID)
				{
					// 버튼에 해당하는 마을맵에 내가 있으면
					D3DXVECTOR2 CharMarkPos = pControl->GetNowPoint().leftTopPoint;
					CharMarkPos.y -= ( pButton_MyChar->GetHeight() + 1.0f);
					pButton_MyChar->SetOffsetPos( CharMarkPos );
					pButton_MyChar->SetShow(true);
					PosOffset += (pButton_MyChar->GetWidth() + 1.0f);
				}

				// 파티가 있으면
				if( g_pData->GetPartyManager()->DoIHaveParty() )
				{
					// 파티원들을 하나씩 찾아보자..
					for(int i=0; i<3; i++ )
					{
						CX2PartyManager::PartyMemberData* pMemberData =
							g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( i, g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
						if(pMemberData != NULL)
						{
							// 해당 맵ID랑 파티원의 현재 맵ID랑 비교해서
							SEnum::VILLAGE_MAP_ID eCurrentVillageID = (SEnum::VILLAGE_MAP_ID) pMemberData->m_iStateCode;
							// 던전라운지면 던전게이트로 살짝 바꿔준다
							if(g_pData->GetLocationManager()->IsDungeonLounge(eCurrentVillageID))
							{
								CX2LocationManager::LOCAL_MAP_ID eLocalMapID = g_pData->GetLocationManager()->GetLocalMapID( eCurrentVillageID );
								eCurrentVillageID = g_pData->GetLocationManager()->GetDungeonGateID( eLocalMapID );
							}
							if( pControl->GetDummyInt(0) == eCurrentVillageID )
							{
								CKTDGUIButton* pButton_PartyMember;
								switch(i)
								{
								case 0:
									{
										pButton_PartyMember = pButton_PartyMember0;
									} break;
								case 1:
									{
										pButton_PartyMember = pButton_PartyMember1;
									} break;
								case 2:
									{
										pButton_PartyMember = pButton_PartyMember2;
									} break;

								}
								D3DXVECTOR2 CharMarkPos = pControl->GetNowPoint().leftTopPoint;
								CharMarkPos.y -= ( pButton_PartyMember->GetHeight() + 1.0f);
								CharMarkPos.x += PosOffset;
								pButton_PartyMember->SetOffsetPos( CharMarkPos );
								pButton_PartyMember->SetShow(true);
								PosOffset += (pButton_PartyMember->GetWidth() + 1.0f);
							}
						}
					}
				}

			}*/
			//}}
		}

		if( m_pDLGWorldMapFront != NULL && m_pDLGWorldMap != NULL )
		{

			CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGWorldMapFront->GetControl( L"WorldMap_Navi" );
			if( NULL != pNavi )
			{
				D3DXVECTOR2 vPos;

				UpdateNaviMap( vPos, pNavi );
				pNavi->SetRelativeWindowPos( vPos );
			}
		}
	}
}

void CX2WorldMapUI::SetNpcToolTipImage( int iNPCID_)
{
	if( NULL == m_pDLGNPCToolTip )
		return;
	
	CKTDGUIStatic* pStatic_Image = (CKTDGUIStatic*) m_pDLGNPCToolTip->GetControl( L"Npc_Image" );
	if( NULL == pStatic_Image )
		return;

	if( NULL == pStatic_Image->GetPicture(0))

	pStatic_Image->SetShow(true);
	switch( iNPCID_ )
	{
	case CX2UnitManager::NUI_ANNE:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"ANN" );
		break;
	case CX2UnitManager::NUI_HAGERS:		
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"HAGUS" );
		break;
	case CX2UnitManager::NUI_LOW:			
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"ROU" );
		break;
	case CX2UnitManager::NUI_ADAMS:			
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"ADAMS" ); 
		break;
	case CX2UnitManager::NUI_ECHO:			
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"ECO" );
		break;
	case CX2UnitManager::NUI_HOFMANN:		
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"HOFMAN" );
		break;
	case CX2UnitManager::NUI_LUICHEL:		
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"RUICHEL" );
		break;
	case CX2UnitManager::NUI_LENPAD:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"RENPAT" );
		break;
	case CX2UnitManager::NUI_STELLA:		
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"STELRA" );
		break;
	case CX2UnitManager::NUI_CHACHABUK:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"CHACHABUK" );
		break;
	case CX2UnitManager::NUI_RICHANG:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"RICHIANG" );
		break;
	case CX2UnitManager::NUI_TOMA:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"TOMA" );
		break;
	case CX2UnitManager::NUI_ADEL:			
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"ADEL" );
		break;
	case CX2UnitManager::NUI_AGATHA:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"AGATA" );
		break;
	case CX2UnitManager::NUI_AIDA:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"AIDA" );
		break;
	case CX2UnitManager::NUI_AMOS:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"AMOSE" );
		break;
	case CX2UnitManager::NUI_MOON:          
		//pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"ANN" );
		break;
	case CX2UnitManager::NUI_CAMILLA:		
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"KAMILRA" );
		break;
	case CX2UnitManager::NUI_ARIEL:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"ARIEL" );
		break;
	case CX2UnitManager::NUI_HELLEN:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"HELREN" );
		break;
	case CX2UnitManager::NUI_ALLEGRO:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"ALREGRO" );
		break;
	case CX2UnitManager::NUI_LENTO:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"RENTO" );
		break;
	case CX2UnitManager::NUI_GLAVE:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"Glave" );
		break;
	case CX2UnitManager::NUI_LURIEL:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"LURIEL" );
		break;
		//{{ 허상형 : [2009/10/27] //	NPC 얼굴 수정
	case CX2UnitManager::NUI_GRAIL:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"GRAIL" );
		break;
	case CX2UnitManager::NUI_NOEL:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"NOEL" );
		break;
	case CX2UnitManager::NUI_VANESSA:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"VANESSA" );
		break;
	case CX2UnitManager::NUI_PRAUS:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"PRAUS" );
		break;
	case CX2UnitManager::NUI_HANNA:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"HANNA" );
		break;
		// oasis907 : 김상윤 [2011.1.27] // 하멜
	case CX2UnitManager::NUI_HORATIO:	
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"HORATIO" );
		break;
	case CX2UnitManager::NUI_LUCY:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"LUCY" );
		break;
	case CX2UnitManager::NUI_DAISY:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"DAISY" );
		break;
	case CX2UnitManager::NUI_PENENSIO:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"PENENSIO" );
		break;		
	case CX2UnitManager::NUI_DENKA:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"DENKA" );
		break;
	case CX2UnitManager::NUI_LORDROS:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"LORDROS" );
		break;
	case CX2UnitManager::NUI_HELLPUTT:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"Default" );
		break;
#ifdef SERV_PSHOP_AGENCY
	case CX2UnitManager::NUI_MU:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"MU" );
		break;
#endif
#ifdef NEW_HENIR_TEST
	case CX2UnitManager::NUI_LUTO_VILLAGE:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"RUTO" );
		break;
#endif NEW_HENIR_TEST
#ifdef APINK_NPC
	case CX2UnitManager::NUI_EVENT_CRAYONPOP:
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"CRAYONPOP" );
		break;
#endif
#ifdef APINK_ARCHANGEL_NPC
	case CX2UnitManager::NUI_EVENT_APINK_ARCHANGEL: // 에이핑크 대천사 NPC
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"NAUN" );
		break;
#endif //APINK_ARCHANGEL_NPC
#ifdef DARKMOON_NPC
	case CX2UnitManager::NUI_EVENT_DARKMOON: // 다크 문 NPC
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"DARKMOON" );
		break;
#endif

#ifdef SERV_APRIL_FOOLS_DAY
	case CX2UnitManager::NUI_EVENT_BENDERS: //만우절 이벤트 앙드레 벤더스
		pStatic_Image->GetPicture( 0 )->SetTex( L"DLG_UI_Npc_Face01.tga", L"Andre_Benders" );
		break;
#endif //APRIL_FOOLS_DAY

#ifdef VILLAGE_SANDER
	case CX2UnitManager::NUI_ROSEANG: // 악세사리 로즈앙
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"ROSEANG" );
		break;

	case CX2UnitManager::NUI_EMIRATE: // 족장 에미리트
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"EMIRATE" );
		break;

	case CX2UnitManager::NUI_VAPOR: // 연금술사 바포르
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"VAPOR" );
		break;

	case CX2UnitManager::NUI_DAPPAR: // 무기상 다파르
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"DAPPAR" );
		break;

	case CX2UnitManager::NUI_EVENT_MOON_RABBIT: // 추석이벤트 달토끼
		pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.tga", L"MoonRabbit" );
		break;
#endif VILLAGE_SANDER
	default:
		{
			// NUI_PPORU_TOWN,		// 블랙데이 이벤트 NPC
			// NUI_BENDERS_TOWN,	// 블랙데이 이벤트 NPC

			//{{ kimhc // 2010.5.4 // 비밀던전 작업(퀘스트)
#ifdef SERV_SECRET_HELL
			pStatic_Image->GetPicture(0)->SetTex( L"DLG_UI_Npc_Face01.TGA", L"Default" );
#else  SERV_SECRET_HELL
			pStatic_ImageSetShow(false);
#endif SERV_SECRET_HELL
			//}} kimhc // 2010.5.4 // 비밀던전 작업(퀘스트)
		} break;

	}
}
void CX2WorldMapUI::OpenNPCToolTip( bool bOpen, int iNPCID /*= -1*/, D3DXVECTOR2 vPos /*= D3DXVECTOR2(0, 0) */ )
{
	if( -1 == iNPCID )
		bOpen = false;

	const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( (CX2UnitManager::NPC_UNIT_ID) iNPCID );

	if( true == bOpen )
	{
		if( NULL == m_pStage )
			m_pStage = (CKTDXStage*) g_pMain->GetNowState();

		if( m_pDLGNPCToolTip == NULL )
		{
			m_pDLGNPCToolTip = new CKTDGUIDialog( m_pStage, L"DLG_UI_Npc_Info.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGNPCToolTip );
		}

		// 정보 update
		SetNpcToolTipImage(iNPCID); //툴팁 NPC 초상화 설정


		CKTDGUIStatic* pStatic_Name 	= (CKTDGUIStatic*) m_pDLGNPCToolTip->GetControl( L"Npc_Name" );
		if( CX2UnitManager::NUI_NONE != iNPCID)
		{
			if( NULL != pNPCTemplet)
				pStatic_Name->GetString(0)->msg = pNPCTemplet->m_Name;
		}
		else//NPCID가 NONE일경우 마켓으로 판단	
		{
			pStatic_Name->GetString(0)->msg = GET_STRING(STR_ID_4521); 
		}


		CKTDGUIStatic* pStatic_Desc 	= (CKTDGUIStatic*) m_pDLGNPCToolTip->GetControl( L"Npc_Desc" );		

		// 퀘스트 정보 받기.
		wstring wstrDesc = L"";
		if( NULL != pNPCTemplet)
		{
			wstrDesc += pNPCTemplet->m_Description;
			wstrDesc += GetNPCQuestDesc( pNPCTemplet->m_nNPCUnitID );
		}
		else//NPCID가 NONE일경우 마켓으로 판단	
			wstrDesc += GET_STRING(STR_ID_4340); 

		pStatic_Desc->GetString(0)->msg = wstrDesc;
		//{{ 2009.02.11 김태완
		// 창 크기 조절로 처리하자...
		CKTDGUIControl::UIStringData* pString = NULL;
		int ukFontID = 0;

		if ( pStatic_Desc != NULL )
		{
			pString = pStatic_Desc->GetString(0);
			if ( pString != NULL )
			{
				ukFontID = pString->fontIndex;
			}
		}

		const int MAGIC_DESC_ADD_SIZE_X = 25;
		// Y값은 좀 더 늘어날 수도 있으니 ADD_SIZE_Y를 좀 넉넉하게 잡아 둔다 : 이름 등등이 늘어나는 경우는 고려하지 않았음
		const int MAGIC_DESC_ADD_SIZE_Y = 40;
		CKTDGFontManager::CUKFont* pDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( ukFontID );
		int DescWidth = (int)( pDescFont->GetWidth( wstrDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + MAGIC_DESC_ADD_SIZE_X;
		int DescHeight = (int)( pDescFont->GetHeight( wstrDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() )+ MAGIC_DESC_ADD_SIZE_Y;

		// 두 값의 최소값을 보장해 준다. MinSize는 원래의 Lua 파일에 있던 크기
		CKTDGUIStatic* pStatic_BG	= (CKTDGUIStatic*) m_pDLGNPCToolTip->GetControl( L"Npc_Info" );
		const int MinSizeX = 130;
		const int MinSizeY = 52;

		// 높이 보정
		int enterNum = 1;
		// 블록은 구분용
		{
			int stringIndex = 0;
			wstring stringData = wstrDesc;


			while( 1 )
			{
				stringIndex = stringData.find( L"\n", stringIndex );
				if ( stringIndex == -1 )
					break;

				stringIndex++;
				enterNum++;
			}
		}


		DescWidth = max(DescWidth, MinSizeX);
		DescHeight = max(DescHeight+enterNum*2, MinSizeY);

		pStatic_BG->GetPicture(0)->SetSize( D3DXVECTOR2( (float)DescWidth, (float)DescHeight ) );

		// 테두리 흰선 그리기~
		pStatic_BG->GetPicture(1)->pPoint->leftTopPoint = D3DXVECTOR2(0,0);
		pStatic_BG->GetPicture(1)->SetSize( D3DXVECTOR2( (float)DescWidth+1, 2.0f));

		pStatic_BG->GetPicture(2)->pPoint->leftTopPoint = D3DXVECTOR2(0, (float)DescHeight);
		pStatic_BG->GetPicture(2)->SetSize( D3DXVECTOR2( (float)DescWidth+1, 2.0f));

		pStatic_BG->GetPicture(3)->pPoint->leftTopPoint = D3DXVECTOR2(0, 2.0f);
		pStatic_BG->GetPicture(3)->SetSize( D3DXVECTOR2( 2.0f, (float)DescHeight - 2));

		pStatic_BG->GetPicture(4)->pPoint->leftTopPoint = D3DXVECTOR2((float)DescWidth-1, 2.0f );
		pStatic_BG->GetPicture(4)->SetSize( D3DXVECTOR2( 2.0f, (float)DescHeight - 2 ));
		//}}

		// 위치를 정해 주자
		m_pDLGNPCToolTip->SetShowEnable( bOpen, bOpen );

		vPos = vPos + D3DXVECTOR2( 25,25 );
		if(vPos.x + DescWidth > 1024 )
		{
			vPos.x = (float)(1024 - DescWidth);
		}
		if(vPos.y + DescHeight > 768 )
		{
			vPos.y = (float)(768 - DescHeight);
		}
		m_pDLGNPCToolTip->SetPos( vPos );

	}

	if( m_pDLGNPCToolTip != NULL )
	{
		m_pDLGNPCToolTip->SetShowEnable( bOpen, bOpen );
	}

}

void CX2WorldMapUI::SetStage( CKTDXStage* pStage )
{
	m_pStage = pStage;

	if( m_pDLGWorldMap != NULL )
	{
		m_pDLGWorldMap->SetStage( pStage );
	}

	if( m_pDLGWorldMapFront != NULL )
	{
		m_pDLGWorldMapFront->SetStage( pStage );
	}

	if( m_pDLGFieldMap != NULL )
	{
		m_pDLGFieldMap->SetStage( pStage );
	}
	if( m_pDLGNPCToolTip != NULL )
	{
		m_pDLGNPCToolTip->SetStage( pStage );
	}
}

//{{ 2009.2.13 김태완
void CX2WorldMapUI::OpenPartyMemberToolTip( bool bOpen, int PartyMemberIndex /* = -1*/, D3DXVECTOR2 vPos /*= D3DXVECTOR2(0,0)*/ )
{
	if( false == bOpen )
	{
		if( NULL != m_pDLGPartyToolTip )
		{
			m_pDLGPartyToolTip->SetShowEnable( bOpen, bOpen );
		}
	}
	else
	{

		int iLevel = 0;
		wstring NickName = L"";
		CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_ELSWORD_KNIGHT;
		bool bPartyLeader = false;

		if(PartyMemberIndex == -1)
		{
			// 내정보
			const CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

			iLevel = pUnitData->m_Level;
			NickName = pUnitData->m_NickName;
			eUnitClass = pUnitData->m_UnitClass;
			if(g_pData->GetPartyManager()->DoIHaveParty())
			{
				CX2PartyManager::PartyMemberData* pMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
				if( NULL != pMemberData )
				{
					bPartyLeader = pMemberData->m_bPartyLeader;
				}
			}
		}
		else
		{
			if(g_pData->GetPartyManager()->DoIHaveParty())
			{
				// 남의 정보
				CX2PartyManager::PartyMemberData* pMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( PartyMemberIndex );
				if(NULL != pMemberData)
				{
					iLevel = pMemberData->m_iUnitLevel;
					NickName = pMemberData->m_wstrNickName;
					eUnitClass = pMemberData->m_eUnitClass;
					bPartyLeader = pMemberData->m_bPartyLeader;
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}


		if( NULL == m_pStage )
		{
			m_pStage = (CKTDXStage*) g_pMain->GetNowState();
		}

		if( m_pDLGPartyToolTip == NULL )
		{
			m_pDLGPartyToolTip = new CKTDGUIDialog( m_pStage, L"DLG_UI_Party_Comrade_Tool_Tip.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPartyToolTip );
		}

		// 위치 정하고, 정보 update
		m_pDLGPartyToolTip->SetShowEnable( bOpen, bOpen );
		m_pDLGPartyToolTip->SetPos( vPos + D3DXVECTOR2( 25, 25 ) );


		CKTDGUIStatic* pStatic_Image	= (CKTDGUIStatic*) m_pDLGPartyToolTip->GetControl( L"Cha_Image" );
		CKTDGUIStatic* pStatic_Name 	= (CKTDGUIStatic*) m_pDLGPartyToolTip->GetControl( L"Cha_Name" );
		CKTDGUIStatic* pStatic_Level10 	= (CKTDGUIStatic*) m_pDLGPartyToolTip->GetControl( L"Cha_Level_10" );
		CKTDGUIStatic* pStatic_Level1 	= (CKTDGUIStatic*) m_pDLGPartyToolTip->GetControl( L"Cha_Level_1" );


		pStatic_Name->GetString(0)->msg = NickName.c_str();


		wstring fileName;
		wstring pieceName;
		if( true == CX2Data::GetCharacterImageName( fileName, pieceName, eUnitClass, CX2Data::CIT_20by20 ) )
		{
			pStatic_Image->GetPicture(0)->SetTex( fileName.c_str(), pieceName.c_str() );
		}
		else
		{
			ASSERT( !"NO" );
		}


		for(int i=0; i<10; i++)
		{
			if(iLevel/10 == i)
			{
				pStatic_Level10->GetPicture(i)->SetShow(true);
			}
			else
			{
				pStatic_Level10->GetPicture(i)->SetShow(false);
			}

			if(iLevel%10 == i)
			{
				pStatic_Level1->GetPicture(i)->SetShow(true);
			}
			else
			{
				pStatic_Level1->GetPicture(i)->SetShow(false);
			}
		}
	}

}
//}}

bool CX2WorldMapUI::GetShow()
{
	if(m_pDLGWorldMap != NULL)
	{
		return m_pDLGWorldMap->GetShow();
	}
	return false;
}

// 2009.03.11 김태완
// msg : 해당하는 MouseUp Msg를 이용해서 어떤 message를 넣어줘야 할 지 구분한다.
void CX2WorldMapUI::OpenMiscToolTip(bool bOpen, D3DXVECTOR2 vPos, WORLD_MAP_UI_CUSTOM_MSG msg, int iFieldID )
{

	wstring wstrName = L"";
	wstring wstrDesc = L"";

	switch(msg)
	{
	case WMUCM_POSTBOX_MOUSE_OVER:
		{
			wstrName += GET_STRING( STR_ID_754 );
			wstrDesc += GET_STRING( STR_ID_942 );
		} break;
	case WMUCM_MARKET_MOUSE_OVER:
		{
			wstrName += GET_STRING( STR_ID_943 );
			wstrDesc += GET_STRING( STR_ID_944 );
		} break;
	case WMUCM_OTHER_FIELD_MOUSE_OVER:
		{
			wstrName += GET_STRING( STR_ID_945 );

			CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( (SEnum::VILLAGE_MAP_ID) iFieldID );

			if ( NULL != pVillageTemplet )
			{
				wstrDesc += GET_REPLACED_STRING( ( STR_ID_948, "LI",
					pVillageTemplet->m_Name,
					KHanSungPostWordUnicodeWrapper( pVillageTemplet->m_Name.c_str(), STR_ID_946, STR_ID_947 ) ) );
			}
			else
			{
				CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();

				if ( NULL != battleFieldManager.GetBattleFieldNameByBattleFieldId( iFieldID ) )
				{
					wstring wstr = battleFieldManager.GetBattleFieldNameByBattleFieldId( iFieldID );

					wstrDesc += GET_REPLACED_STRING( ( STR_ID_948, "LI", wstr, KHanSungPostWordUnicodeWrapper( wstr.c_str(), STR_ID_946, STR_ID_947 ) ) );
				}
			}
			
		} break;
		//{{ kimhc // 2010.5.4 // 비밀던전 작업(일일퀘스트)
#ifdef SERV_SECRET_HELL
	case WMUCM_BILLBOARD_MOUSE_OVER:
		{
			wstrName += GET_STRING( STR_ID_3723 );
			wstrDesc += GET_STRING( STR_ID_3844 );
		} break;

#endif SERV_SECRET_HELL
		//}} kimhc // 2010.5.4 // 비밀던전 작업(일일퀘스트)

	case WMUCM_VILLAGE_MOUSE_OVER:
		{
			CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( (SEnum::VILLAGE_MAP_ID) iFieldID );

			if ( NULL != pVillageTemplet )
			{
				wstrName += pVillageTemplet->m_Name.c_str();
				wstrDesc += pVillageTemplet->m_Name.c_str();
			}

			CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
			const WCHAR* pName = battleFieldManager.GetBattleFieldNameByBattleFieldId( iFieldID );

			if ( NULL != pName )
			{
				wstrName += pName; 
				wstrDesc += pName;
			}

		} break;

	default:
		{
			bOpen = false;
		} break;
	}

	if( false == bOpen )
	{
		if( m_pDLGMiscToolTip != NULL )
		{
			m_pDLGMiscToolTip->SetShowEnable( bOpen, bOpen );
		}
	}
	else
	{
		if( NULL == m_pStage )
		{
			m_pStage = (CKTDXStage*) g_pMain->GetNowState();
		}

		if( m_pDLGMiscToolTip == NULL )
		{
			m_pDLGMiscToolTip = new CKTDGUIDialog( m_pStage, L"DLG_UI_Misc_Info.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMiscToolTip );
		}

		CKTDGUIStatic* pStatic_Name 	= (CKTDGUIStatic*) m_pDLGMiscToolTip->GetControl( L"Misc_Name" );
		CKTDGUIStatic* pStatic_Desc 	= (CKTDGUIStatic*) m_pDLGMiscToolTip->GetControl( L"Misc_Desc" );

		pStatic_Name->GetString(0)->msg = wstrName.c_str();
		pStatic_Desc->GetString(0)->msg = wstrDesc.c_str();
		//{{ 2009.02.11 김태완
		// 창 크기 조절로 처리하자...
		CKTDGUIControl::UIStringData* pString = NULL;
		int ukFontID = 0;

		if ( pStatic_Desc != NULL )
		{
			pString = pStatic_Desc->GetString(0);
			if ( pString != NULL )
			{
				ukFontID = pString->fontIndex;
			}
		}

#ifdef CLIENT_COUNTRY_EU
		const int MAGIC_DESC_ADD_SIZE_X = 65;
#else //CLIENT_COUNTRY_EU
		const int MAGIC_DESC_ADD_SIZE_X = 25;
#endif //CLIENT_COUNTRY_EU
		// Y값은 좀 더 늘어날 수도 있으니 ADD_SIZE_Y를 좀 넉넉하게 잡아 둔다 : 이름 등등이 늘어나는 경우는 고려하지 않았음
		const int MAGIC_DESC_ADD_SIZE_Y = 40;
		CKTDGFontManager::CUKFont* pDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( ukFontID );
		int DescWidth = (int)( pDescFont->GetWidth( wstrDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + MAGIC_DESC_ADD_SIZE_X;
		int DescHeight = (int)( pDescFont->GetHeight( wstrDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() )+ MAGIC_DESC_ADD_SIZE_Y;

		// 두 값의 최소값을 보장해 준다. MinSize는 원래의 Lua 파일에 있던 크기
		CKTDGUIStatic* pStatic_BG	= (CKTDGUIStatic*) m_pDLGMiscToolTip->GetControl( L"Misc_Info" );
		const int MinSizeX = 130;
		const int MinSizeY = 52;
		DescWidth = max(DescWidth, MinSizeX);
		DescHeight = max(DescHeight, MinSizeY);

		pStatic_BG->GetPicture(0)->SetSize( D3DXVECTOR2( (float)DescWidth, (float)DescHeight ) );

		// 테두리 흰선 그리기~
		pStatic_BG->GetPicture(1)->pPoint->leftTopPoint = D3DXVECTOR2(0,0);
		pStatic_BG->GetPicture(1)->SetSize( D3DXVECTOR2( (float)DescWidth+1, 2.0f));

		pStatic_BG->GetPicture(2)->pPoint->leftTopPoint = D3DXVECTOR2(0, (float)DescHeight);
		pStatic_BG->GetPicture(2)->SetSize( D3DXVECTOR2( (float)DescWidth+1, 2.0f));

		pStatic_BG->GetPicture(3)->pPoint->leftTopPoint = D3DXVECTOR2(0, 2.0f);
		pStatic_BG->GetPicture(3)->SetSize( D3DXVECTOR2( 2.0f, (float)DescHeight - 2));

		pStatic_BG->GetPicture(4)->pPoint->leftTopPoint = D3DXVECTOR2((float)DescWidth-1, 2.0f );
		pStatic_BG->GetPicture(4)->SetSize( D3DXVECTOR2( 2.0f, (float)DescHeight - 2 ));
		//}}

		// Position 위치 예외처리 해 준다.
		if(vPos.x + DescWidth > 1024.f)
		{
			vPos.x = 1024.f - DescWidth - 30.f;
		}
		if(vPos.y + DescHeight > 768.f)
		{
			vPos.y = 768.f - DescWidth - 30.f;
		}

		// 위치 정하고, 정보 update
		m_pDLGMiscToolTip->SetShowEnable( bOpen, bOpen );
		m_pDLGMiscToolTip->SetPos( vPos + D3DXVECTOR2( 25, 25 ) );
	}

}

void CX2WorldMapUI::CloseAllToolTip()
{
	OpenMiscToolTip(false);
	OpenNPCToolTip(false);
	OpenPartyMemberToolTip(false);
}

wstring CX2WorldMapUI::GetNPCQuestDesc( CX2UnitManager::NPC_UNIT_ID eNPCID )
{
	std::multimap<int, const CX2QuestManager::QuestTemplet*> mapQuest;

	vector<int> vecQuestID;
	g_pData->GetQuestManager()->GetAvailableQuest( eNPCID, vecQuestID );

	// 아직 받지 않는 퀘스트 목록을 정렬하자
	vector<int> vecChangeJobQuestID;
	vector<int> vecEventQuestID;
	vector<int> vecNotAssignedQuestID;

	for( UINT i=0; i<vecQuestID.size(); i++ )
	{
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( vecQuestID[i] );
		if( NULL == pQuestTemplet )
			continue;

		const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( vecQuestID[i] );

		//{{ kimhc // 2010.2.1 //	PC방이 아닌곳에서는 받지 않은 PC방퀘스트 목록에서 보이지 않도록
#ifdef	PC_BANG_QUEST
		if ( pQuestTemplet->m_bIsPcBang == true && g_pData->GetMyUser()->GetIsPCRoom() == false
			&& pQuestInst == NULL )
			continue;
#endif	PC_BANG_QUEST
		//}} kimhc // 2010.2.1 //	PC방이 아닌곳에서는 받지 않은 PC방퀘스트 목록에서 보이지 않도록

		//{{ kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
#ifdef	SERV_DAY_QUEST
		if ( pQuestTemplet->m_bIsTimeEvent == true )
			continue;
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
		
#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		if( CX2QuestManager::IsNewUserOnlyQuest( vecQuestID[i] ) == true &&
			g_pInstanceData->IsRecruit() == false )
			continue;
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

		if( CX2QuestManager::QT_CHANGE_JOB == pQuestTemplet->m_eQuestType && NULL == pQuestInst )
		{
			vecChangeJobQuestID.push_back( vecQuestID[i] );
		}
		else if( ( CX2QuestManager::QT_EVENT == pQuestTemplet->m_eQuestType 
			//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
			|| CX2QuestManager::QT_PCBANG == pQuestTemplet->m_eQuestType
#endif	PC_BANG_QUEST
			//}} kimhc // 2010.2.1 //	PC방 퀘스트
			)
			&& NULL == pQuestInst )
		{
			vecEventQuestID.push_back( vecQuestID[i] );
		}
		else
		{
			int iLevelDiff = g_pData->GetSelectUnitLevel() - pQuestTemplet->m_iFairLevel;
			if( iLevelDiff > 5 )
			{
				//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
				switch ( pQuestTemplet->m_eRepeatType )
				{
				case CX2QuestManager::QRT_REPEAT:
				case CX2QuestManager::QRT_DAY:
					vecNotAssignedQuestID.push_back( vecQuestID[i] );
					break;

				default:
					break;
				}
#else	SERV_DAILY_QUEST
				if(pQuestTemplet->m_bRepeat)
				{
					vecNotAssignedQuestID.push_back( vecQuestID[i] );
				}
				//vecNotAssignedQuestID2.push_back( vecQuestID[i] );
#endif	SERV_DAILY_QUEST
				//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
			}
			else
			{
				vecNotAssignedQuestID.push_back( vecQuestID[i] );
			}
		}
	}

	// 이제 표시해줄 퀘스트가 존재하는지 확인한다
	if( vecNotAssignedQuestID.empty() == true &&
		vecEventQuestID.empty() == true &&
		vecChangeJobQuestID.empty() == true )
	{
		return L"";
	}


	// NotAssignedQuestID를 다시 정렬해준다
	for(vector<int>::iterator it = vecNotAssignedQuestID.begin(); it < vecNotAssignedQuestID.end(); ++it)
	{
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( *it );
		if( pQuestTemplet != NULL )
		{
			int DungeonID;

			// 해당 던전 아이디를 구해오자 : 첫번째 서브퀘스트를 기준으로 함. 여러 개의 던전에서 수행해야 하는 경우는 고려되지 않음.
			// 전직퀘 or 이벤트퀘는 특수로 분류한다
			if( pQuestTemplet->m_vecSubQuest.size() > 0 && 
				( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_NORMAL || pQuestTemplet->m_eQuestType == CX2QuestManager::QT_SKILL ) )
			{
				int SubQuestID = pQuestTemplet->m_vecSubQuest[0];
				const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( SubQuestID );
				if(pSubQuestTemplet != NULL )
				{
					switch( pSubQuestTemplet->m_eClearType )
					{
					case CX2QuestManager::SQT_NPC_HUNT:
					case CX2QuestManager::SQT_QUEST_ITEM_COLLECTION:
					case CX2QuestManager::SQT_DUNGEON_TIME:
					case CX2QuestManager::SQT_DUNGEON_RANK:
					case CX2QuestManager::SQT_DUNGEON_DAMAGE:
					case CX2QuestManager::SQT_DUNGEON_CLEAR_COUNT:
#ifdef SERV_INTEGRATION
					case CX2QuestManager::SQT_WITH_DIF_SERV_USER:
#endif SERV_INTEGRATION
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
					case CX2QuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
#ifdef SERV_POINT_COUNT_SYSTEM
					case CX2QuestManager::SQT_POINT_COUNT:
#endif SERV_POINT_COUNT_SYSTEM
						{
							// DID는 normal-hard-expert가 붙어 있고, 30000부터 시작하고, 각 던전마다 10단위로 분류되어 있다.
							// 따라서 그냥 10자리를 자른 값을 넣어 준다.
							// 해당 던전이 존재하는 던전이면 던전 ID로 묶어주고
#ifdef REFORM_QUEST
							if( true == pSubQuestTemplet->m_ClearCondition.m_setDungeonID.empty() )
							{
								std::set<SEnum::DUNGEON_ID>::iterator it = pSubQuestTemplet->m_ClearCondition.m_setDungeonID.begin();
								if( NULL != g_pData->GetDungeonManager()->GetDungeonData( *it ) )
								{
									DungeonID = (int)(*it / 10) * 10;
								}	
								else
								{
									DungeonID = SEnum::DI_END;
								}
							}
#else
							if( NULL != g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)pSubQuestTemplet->m_ClearCondition.m_eDungeonID) )
							{
								DungeonID = (int)(pSubQuestTemplet->m_ClearCondition.m_eDungeonID / 10) * 10;
							}	
#endif //REFORM_QUEST		
							else
							{
								// 아니면 일반으로 묶는다
								DungeonID = SEnum::DI_END;
							}	
						}
						break;
					case CX2QuestManager::SQT_NPC_TALK:
					case CX2QuestManager::SQT_ITEM_COLLECTION:
					case CX2QuestManager::SQT_PVP_PLAY:
					case CX2QuestManager::SQT_PVP_WIN:
					case CX2QuestManager::SQT_PVP_KILL:
#ifdef SERV_NEW_PVP_QUEST
					case CX2QuestManager::SQT_PVP_NPC_HUNT:
#endif SERV_NEW_PVP_QUEST
#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
					case CX2QuestManager::SQT_PVP_HERO_NPC_KILL:
					case CX2QuestManager::SQT_PVP_HERO_NPC_PLAY:
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND

					default:
						{
							// 없다
							DungeonID = SEnum::DI_END;
						} break;
					}
				}
				else
				{
					// 아예 해당 서브퀘스트가 없다
					DungeonID = SEnum::DI_END;
				}			
			}
			else
			{
				DungeonID = SEnum::DI_NONE;
			}
			mapQuest.insert(std::make_pair(DungeonID, pQuestTemplet));
		}
	}

	// 여기까지 했으면 퀘스트가 종류별/던전별로 분류가 되었으니

	wstring returnStr = L"";
	returnStr += GET_STRING( STR_ID_949 );
	int CurrentDID = -1;
	for( std::multimap<int, const CX2QuestManager::QuestTemplet*>::iterator it = mapQuest.begin(); it != mapQuest.end(); ++it )
	{
		if(CurrentDID != it->first)
		{
			CurrentDID = it->first;

			if(CurrentDID == SEnum::DI_END)
			{
				returnStr += L"+ ";
				returnStr += GET_STRING( STR_ID_950 );
				returnStr += L"\n";
			}
			else if(CurrentDID ==SEnum::DI_NONE)
			{
				returnStr += L"+ ";
				returnStr += GET_STRING( STR_ID_890 );
				returnStr += L"\n";
			}
			else if(NULL != g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)CurrentDID) )
			{
				returnStr += L"+ ";
				returnStr += g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)CurrentDID)->m_DungeonName;				
				returnStr += L"\n";
			}
			else
			{
				ASSERT( !"던전 못 찾았음" );
			}

		}
		const CX2QuestManager::QuestTemplet* pQuestTemplet = it->second;
		returnStr += L" ";
		returnStr += g_pData->GetQuestManager()->GetQuestColorString( pQuestTemplet->m_iID, g_pData->GetSelectUnitLevel());
		returnStr += pQuestTemplet->m_wstrTitle;

		//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
		switch ( pQuestTemplet->m_eRepeatType )
		{
		case CX2QuestManager::QRT_REPEAT:
			{
				returnStr += L" (";
				returnStr += GET_STRING( STR_ID_951 );
				returnStr += L")";
			}
			break;

		case CX2QuestManager::QRT_DAY:
			{
				returnStr += L" (";
				returnStr += GET_STRING( STR_ID_5114 );
				returnStr += L")";
			}
			break;

		default:
			break;
		}

		returnStr += L"#CX\n";
#else	SERV_DAILY_QUEST
		if(pQuestTemplet->m_bRepeat)
		{
			returnStr += L" (";
			returnStr += GET_STRING( STR_ID_951 );
			returnStr += L")";
			returnStr += L"#CX\n";
		}
#endif	SERV_DAILY_QUEST
		//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
	}

	if(false == vecEventQuestID.empty())
	{
		returnStr += L"+ ";
		returnStr += GET_STRING( STR_ID_952 );
		returnStr += L"\n";
		for(vector<int>::iterator it = vecEventQuestID.begin(); it < vecEventQuestID.end(); ++it)
		{
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( *it );
			returnStr += L" ";
			//returnStr += g_pData->GetQuestManager()->GetQuestColorString( pQuestTemplet->m_iID, g_pData->GetSelectUnitLevel());
			returnStr += L"#CFF8000";
			returnStr += pQuestTemplet->m_wstrTitle;

			//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
			switch ( pQuestTemplet->m_eRepeatType )
			{
			case CX2QuestManager::QRT_REPEAT:
				{
					returnStr += L" (";
					returnStr += GET_STRING( STR_ID_951 );
					returnStr += L")";
				}
				break;

			case CX2QuestManager::QRT_DAY:
				{
					returnStr += L" (";
					returnStr += GET_STRING( STR_ID_5114 );
					returnStr += L")";
				}
				break;

			default:
				break;
			}

			returnStr += L"#CX\n";
#else	SERV_DAILY_QUEST
			if(pQuestTemplet->m_bRepeat)
			{
				returnStr += L" (";
				returnStr += GET_STRING( STR_ID_951 );
				returnStr += L")";
			}
			returnStr += L"#CX\n";		
#endif	SERV_DAILY_QUEST
			//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)	
		}
	}

	if(false == vecChangeJobQuestID.empty())
	{
		returnStr += L"+ ";
		returnStr += GET_STRING( STR_ID_953 );
		returnStr += L"\n";
		for(vector<int>::iterator it = vecChangeJobQuestID.begin(); it < vecChangeJobQuestID.end(); ++it)
		{
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( *it );
			returnStr += L" ";
			//returnStr += g_pData->GetQuestManager()->GetQuestColorString( pQuestTemplet->m_iID, g_pData->GetSelectUnitLevel());
			returnStr += L"#C6E6EFF";
			returnStr += pQuestTemplet->m_wstrTitle;

			//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
			switch ( pQuestTemplet->m_eRepeatType )
			{
			case CX2QuestManager::QRT_REPEAT:
				{
					returnStr += L" (";
					returnStr += GET_STRING( STR_ID_951 );
					returnStr += L")";
				}
				break;

			case CX2QuestManager::QRT_DAY:
				{
					returnStr += L" (";
					returnStr += GET_STRING( STR_ID_5114 );
					returnStr += L")";
				}
				break;

			default:
				break;
			}

			returnStr += L"#CX\n";
#else	SERV_DAILY_QUEST
			if(pQuestTemplet->m_bRepeat)
			{
				returnStr += L" (";
				returnStr += GET_STRING( STR_ID_951 );
				returnStr += L")";
			}
			returnStr += L"#CX\n";	
#endif	SERV_DAILY_QUEST
			//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)		

		}
	}

	return returnStr;

}

void CX2WorldMapUI::UpdateNaviMap( IN D3DXVECTOR2& vPos_, IN CKTDGUINavigation* pNavi_ )
{
	CKTDGUIStatic* pStaticWorld = NULL;
	CKTDGUIStatic* pStaticField = NULL;

	if ( NULL != m_pDLGWorldMapFront )
	{
		pStaticWorld = (CKTDGUIStatic*)m_pDLGWorldMapFront->GetControl( L"Static_World_Remarks" );
		pStaticField = (CKTDGUIStatic*)m_pDLGWorldMapFront->GetControl( L"Static_Field_Remarks" );
	}

	if ( true == m_bZoomed )
	{
		if ( NULL != pStaticWorld )
			pStaticWorld->SetShow( false );

		if ( NULL != pStaticField )
			pStaticField->SetShow( true );

		map< int, CKTDGUIDialogType >::iterator it = m_mapDLGFieldMap.find( m_iCurrFieldMapID );
		if( it != m_mapDLGFieldMap.end() )
		{
			vPos_ = it->second->GetPos();
			vPos_.x /= -it->second->GetSize().x;
			vPos_.y /= -it->second->GetSize().y;

			switch ( m_iCurrFieldMapID )
			{
#pragma region 루벤
			case SEnum::VMI_RUBEN:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Village.TGA", L"TownMap_Navigation_Ruben" );
				break;

			case SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_00.TGA", L"FieldMap_Navigation_Ruben_01" );
				break;
#pragma endregion 네비게이션

#pragma region 엘더
			case SEnum::VMI_ELDER:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Village.TGA", L"TownMap_Navigation_Elder" );
				break;
			case SEnum::VMI_BATTLE_FIELD_ELDER_FIELD_01:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_00.TGA", L"FieldMap_Navigation_Elder_01" );
				break;
			case SEnum::VMI_BATTLE_FIELD_ELDER_REST_00:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Rest.TGA", L"FieldMap_Navigation_Elder_R" );
				break;
			case SEnum::VMI_BATTLE_FIELD_ELDER_FIELD_02:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_00.TGA", L"FieldMap_Navigation_Elder_02" );
				break;
#pragma endregion 네비게이션

#pragma region 베스마
			case SEnum::VMI_BESMA:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Village.TGA", L"TownMap_Navigation_Besma" );
				break;
			case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_01:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_00.TGA", L"FieldMap_Navigation_Besma_01" );
				break;
			case SEnum::VMI_BATTLE_FIELD_BESMA_REST_00:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Rest.TGA", L"FieldMap_Navigation_Besma_R" );
				break;
			case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_02:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_00.TGA", L"FieldMap_Navigation_Besma_02" );
				break;
			case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_03:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_00.TGA", L"FieldMap_Navigation_Besma_03" );
				break;
#pragma endregion 네비게이션

#pragma region 알테라
			case SEnum::VMI_ALTERA:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Village.TGA", L"TownMap_Navigation_Altera" );
				break;
			case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_01:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_00.TGA", L"FieldMap_Navigation_Altera_01" );
				break;
			case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_02:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_00.TGA", L"FieldMap_Navigation_Altera_02" );
				break;
			case SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Rest.TGA", L"FieldMap_Navigation_Altera_R" );
				break;
			case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_03:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_01.TGA", L"FieldMap_Navigation_Altera_03" );
				break;
#pragma endregion 네비게이션

#pragma region 페이타
			case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_01:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_01.TGA", L"FieldMap_Navigation_Peita_01" );
				break;
			case SEnum::VMI_PEITA:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Village.TGA", L"TownMap_Navigation_Peita" );
				break;
			case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_02:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_01.TGA", L"FieldMap_Navigation_Peita_02" );
				break;
			case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_03:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_01.TGA", L"FieldMap_Navigation_Peita_03" );
				break;
#pragma endregion 네비게이션

#pragma region 벨더
			case SEnum::VMI_VELDER:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Village.TGA", L"TownMap_Navigation_Velder" );
				break;
			case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_01:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_01.TGA", L"FieldMap_Navigation_Velder_01" );
				break;
			case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_02:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_01.TGA", L"FieldMap_Navigation_Velder_02" );
				break;
			case SEnum::VMI_BATTLE_FIELD_VELDER_REST_00:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Rest.TGA", L"FieldMap_Navigation_Velder_R" );
				break;
			case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_03:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_01.TGA", L"FieldMap_Navigation_Velder_03" );
				break;
			case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_04:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_01.TGA", L"FieldMap_Navigation_Velder_04" );
				break;
			case SEnum::VMI_BATTLE_FIELD_VELDER_SHIP_STAGE:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_02.TGA", L"FieldMap_Navigation_Velder_Ship" );
				break;
#pragma endregion 네비게이션

#pragma region 하멜
			case SEnum::VMI_BATTLE_FIELD_HAMEL_SHIP_STAGE:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_02.TGA", L"FieldMap_Navigation_Hamel_Ship" );
				break;
			case SEnum::VMI_HAMEL:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Village.TGA", L"TownMap_Navigation_Hamel" );
				break;
			case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_02.TGA", L"FieldMap_Navigation_Hamel_01" );
				break;
			case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_02.TGA", L"FieldMap_Navigation_Hamel_02" );
				break;
			case SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Rest.TGA", L"FieldMap_Navigation_Hamel_R" );
				break;
			case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_02.TGA", L"FieldMap_Navigation_Hamel_03" );
				break;
			case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_02.TGA", L"FieldMap_Navigation_Hamel_04" );
				break;
#pragma endregion 네비게이션

#pragma region 샌더
			case SEnum::VMI_SANDER:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Village.TGA", L"TownMap_Navigation_Sander" );
				break;
			case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_02.TGA", L"FieldMap_Navigation_Sander_01" );
				break;
			case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_02.TGA", L"FieldMap_Navigation_Sander_02" );
				break;
			case SEnum::VMI_BATTLE_FIELD_SANDER_REST_00:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Rest.TGA", L"FieldMap_Navigation_Sander_R" );
				break;
#ifdef SANDER_DUNGEON_5_6
			case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_03.TGA", L"FieldMap_Navigation_Sander_03" );
				break;
			case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04:
				pNavi_->SetBGTex( L"DLG_UI_Common_Texture_Field_03.TGA", L"FieldMap_Navigation_Sander_04" );
				break;
#endif //SANDER_DUNGEON_5_6
#pragma endregion 네비게이션

			default:
				break;
			}

			CKTDGUIControl::UIPointData* pNewPoint_Navi = new CKTDGUIControl::UIPointData();
			pNewPoint_Navi->bUseTextureSize		= false;
			pNewPoint_Navi->fChangeTime			= 0.0f;
			if ( vPos_.x > 0.24 )
			{
				pNewPoint_Navi->leftTopPoint		= D3DXVECTOR2( 763 - 750, 606 );
				pNewPoint_Navi->leftBottomPoint		= D3DXVECTOR2( 763 - 750, 732 );
				pNewPoint_Navi->rightTopPoint		= D3DXVECTOR2( 1019 - 750, 606 );
				pNewPoint_Navi->rightBottomPoint	= D3DXVECTOR2( 1019 - 750, 732 );
			}
			else
			{
				pNewPoint_Navi->leftTopPoint		= D3DXVECTOR2( 763, 606 );
				pNewPoint_Navi->leftBottomPoint		= D3DXVECTOR2( 763, 732 );
				pNewPoint_Navi->rightTopPoint		= D3DXVECTOR2( 1019, 606 );
				pNewPoint_Navi->rightBottomPoint	= D3DXVECTOR2( 1019, 732 );
			}
			pNavi_->SetBGPoint( pNewPoint_Navi );

			CKTDGUIControl::UIPointData* pNewPoint_NaviWindow = new CKTDGUIControl::UIPointData();
			pNewPoint_NaviWindow->bUseTextureSize	= false;
			pNewPoint_NaviWindow->fChangeTime		= 0.0f;
			pNewPoint_NaviWindow->leftTopPoint		= D3DXVECTOR2( 763, 606 );
			pNewPoint_NaviWindow->leftBottomPoint	= D3DXVECTOR2( 763, 732 );
			pNewPoint_NaviWindow->rightTopPoint		= D3DXVECTOR2( 909, 606 );
			pNewPoint_NaviWindow->rightBottomPoint	= D3DXVECTOR2( 909, 732 );
			pNavi_->SetWindowPoint( pNewPoint_NaviWindow );
		}	
	}
	else
	{
		if ( NULL != pStaticWorld )
			pStaticWorld->SetShow( true );

		if ( NULL != pStaticField )
			pStaticField->SetShow( false );

		vPos_ = m_pDLGWorldMap->GetPos();
		vPos_.x /= -m_pDLGWorldMap->GetSize().x;
		vPos_.y /= -m_pDLGWorldMap->GetSize().y;

		pNavi_->SetBGTex( L"DLG_UI_Common_Texture70_NEW.TGA", L"WorldMap_Navigation" );

		CKTDGUIControl::UIPointData* pNewPoint_Navi = new CKTDGUIControl::UIPointData();
		pNewPoint_Navi->bUseTextureSize		= false;
		pNewPoint_Navi->fChangeTime			= 0.0f;
		pNewPoint_Navi->leftTopPoint		= D3DXVECTOR2( 753, 524 );
		pNewPoint_Navi->leftBottomPoint		= D3DXVECTOR2( 753, 732 );
		pNewPoint_Navi->rightTopPoint		= D3DXVECTOR2( 1019, 524 );
		pNewPoint_Navi->rightBottomPoint	= D3DXVECTOR2( 1019, 732 );
		pNavi_->SetBGPoint( pNewPoint_Navi );

		CKTDGUIControl::UIPointData* pNewPoint_NaviWindow = new CKTDGUIControl::UIPointData();
		pNewPoint_NaviWindow->bUseTextureSize	= false;
		pNewPoint_NaviWindow->fChangeTime		= 0.0f;
		pNewPoint_NaviWindow->leftTopPoint		= D3DXVECTOR2( 758, 529 );
		pNewPoint_NaviWindow->leftBottomPoint	= D3DXVECTOR2( 758, 644 );
		pNewPoint_NaviWindow->rightTopPoint		= D3DXVECTOR2( 911, 529 );
		pNewPoint_NaviWindow->rightBottomPoint	= D3DXVECTOR2( 911, 644 );
		pNavi_->SetWindowPoint( pNewPoint_NaviWindow );
	}
}

bool CX2WorldMapUI::GetDragNaviMap()
{
	if ( NULL != m_pDLGWorldMapFront )
	{
		CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGWorldMapFront->GetControl( L"WorldMap_Navi" );
		
		if( NULL != pNavi )
		{
			return pNavi->GetDraggingWindow();
		}
	}

	return false;
}


#ifdef VILLAGE_MAP_FAST_RELOADING
void CX2WorldMapUI::ReLoadingVillageMap()
{
	map< int, CKTDGUIDialogType >::iterator it;
	for( it = m_mapDLGFieldMap.begin(); it != m_mapDLGFieldMap.end(); it++ )
	{
		SAFE_DELETE_DIALOG( it->second );
	}
	m_mapDLGFieldMap.clear();

	CKTDGUIDialogType pDLGFieldMap = NULL;
	for( int iVillageID = SEnum::VMI_BATTLE_FIELD_ELDER_REST_00; iVillageID <= SEnum::VMI_BATTLE_FIELD_SANDER_REST_00; ++iVillageID )
	{
		if( iVillageID != SEnum::VMI_BATTLE_FIELD_PEITA_REST_00 )
		{
			pDLGFieldMap = GetFieldMapDlg( iVillageID );
			if( NULL != pDLGFieldMap )
				pDLGFieldMap->SetShowEnable(false,false);
		}
	}

	for( int iVillageID = SEnum::VMI_RUBEN; iVillageID <= SEnum::VMI_SANDER; ++iVillageID )
	{
		pDLGFieldMap = GetFieldMapDlg( iVillageID );
		if( NULL != pDLGFieldMap )
			pDLGFieldMap->SetShowEnable(false,false);
	}

	for( int iVillageID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01; iVillageID <= SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04; ++iVillageID )
	{
		pDLGFieldMap = GetFieldMapDlg( iVillageID );
		if( NULL != pDLGFieldMap )
			pDLGFieldMap->SetShowEnable(false,false);
	}

	m_bZoomed = false;
}
#endif //VILLAGE_MAP_FAST_RELOADING