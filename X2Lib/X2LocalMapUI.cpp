#include "StdAfx.h"
#include ".\x2localmapui.h"



// dmlee 2008.12.3 
// 로컬별로 다이얼로그를 따로 만들어서 로컬을 바꾸게 되면 해당 dialog가 enable, visible되는 것으로 처리함.
// 이유는 전체 지역의 로컬맵 UI를 한번에 다 로딩하는 것 보다는 필요한 것만 로딩하는 것이 좋을 것 같아서 
// 한번에 하나의 로컬맵만 visible







CX2LocalMapUI::CX2LocalMapUI(void) :
m_ePickedLocalMapID( CX2LocationManager::LMI_INVALID ),
m_bShowLocalMap(false),
m_pDlgHeroRecruit(NULL)
{
	SoundReady( L"NewStage_Open.ogg" );

	m_pStage = g_pMain->GetNowState();

#ifdef SERV_EPIC_QUEST
	m_pDLGQuestDesc = new CKTDGUIDialog( NULL, L"DLG_SlotManager.lua", 0.07f, XDL_POP_UP + 3 );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGQuestDesc );
	m_pDLGQuestDesc->SetShow( false );
	m_pDLGQuestDesc->SetFront( true );

	CKTDGUIStatic* pStatic = new CKTDGUIStatic();
	pStatic->SetName( L"ItemDesc" );
	m_pDLGQuestDesc->AddControl( pStatic );
	CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
	pStatic->AddPicture( pPicture );
	pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
	pPicture->SetTex( L"FadeInOut.dds" );

	CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
	pStatic->AddString( pString );
	pString->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;
	pString->color = D3DXCOLOR( 1,1,1,1 );
	pString->outlineColor = D3DXCOLOR( 0,0,0,1 );
	pString->msg = L"";
	pString->fontStyle = CKTDGFontManager::FS_SHELL;
	pString->sortFlag = DT_LEFT;
	pString->pos = D3DXVECTOR2( 20, 20 );
#endif SERV_EPIC_QUEST

	m_pDlgHeroRecruit = new CKTDGUIDialog(NULL, L"DLG_Map_Local_Hero_Recruit.lua", 0.07f, XDL_POP_UP ); 
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgHeroRecruit );
	m_pDlgHeroRecruit->SetShow( false );
}


/*virtual*/ CX2LocalMapUI::~CX2LocalMapUI(void)
{
	ClearLocalMapDLGs();
	SAFE_DELETE_DIALOG(m_pDlgHeroRecruit);
}


/*virtual*/ void CX2LocalMapUI::OnFrameMove( double fTime, float fElapsedTime )
{

}

/*virtual*/ bool CX2LocalMapUI::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
#ifdef SERV_EPIC_QUEST
	//{{ oasis907 : 김상윤 [2010.7.1] // 에픽 퀘스트
	case LMUCM_DUNGEON_MOUSE_OVER:
		{
			CKTDGUIButton* pButton_Dungeon = (CKTDGUIButton*) lParam;
			if( NULL == pButton_Dungeon )
				return true;
			int iDungeonID = pButton_Dungeon->GetDummyInt( 0 );
			
			D3DXVECTOR2 QuestDescPos = pButton_Dungeon->GetPos();
			QuestDescPos.x += 140.f;
			QuestDescPos.y += 80.f;
			
			DrawLocalMapQuestDesc(QuestDescPos, iDungeonID);

			//{{ kimhc // 2011-04-27 // 디펜스 던전 시간에 따라, 던전 아이콘 비/활성화 여부 결정
#ifdef	SERV_INSERT_GLOBAL_SERVER
			// 디펜스 던전 아이콘 중 비활성화 아이콘이면
			if ( true == g_pData->GetDungeonManager()->IsDefenceDungeon( iDungeonID ) &&
				 0 == _wcsicmp( pButton_Dungeon->GetName(), L"DefenceDungeonDisable" ) )
			{
				wstringstream wstrStreamDungeonIconDesc;
				g_pData->GetWorldMissionManager()->GetDefenceDungeonIcnoDesc( wstrStreamDungeonIconDesc );
				pButton_Dungeon->SetGuideDesc( wstrStreamDungeonIconDesc.str().c_str() );//g_pData->GetWorldMissionManager()->GetDefenceDungeonIconDesc().c_str() );
			}
#endif	SERV_INSERT_GLOBAL_SERVER
			//}} kimhc // 2011-04-27 // 디펜스 던전 시간에 따라, 던전 아이콘 비/활성화 여부 결정

#ifdef NEW_HENIR_TEST
			if ( CX2Dungeon::IsHenirDungeon( static_cast<const SEnum::DUNGEON_ID>( iDungeonID ) ) )
			{
				wstringstream wstrStreamDungeonIconDesc;
				GetHenirDungeonIcnoDesc( wstrStreamDungeonIconDesc );
				pButton_Dungeon->SetGuideDesc( wstrStreamDungeonIconDesc.str().c_str() );
			}
#endif NEW_HENIR_TEST

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
			if( CX2Dungeon::IsEventValentineDungeon( static_cast<const SEnum::DUNGEON_ID>( iDungeonID ) ) )
			{
				wstringstream wstrStreamDungeonIconDesc;
				GetEventValentineDungeonIconDesc( wstrStreamDungeonIconDesc );
				pButton_Dungeon->SetGuideDesc( wstrStreamDungeonIconDesc.str().c_str() );
			}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
		}
		break;
	case LMUCM_DUNGEON_MOUSE_OUT:
		{
			m_pDLGQuestDesc->SetShow(false);
		}
		break;
	//}}
#endif SERV_EPIC_QUEST
	case LMUCM_DUNGEON_CLICKED:
		{
			CKTDGUIButton* pButton_Dungeon = (CKTDGUIButton*) lParam;
			if( NULL == pButton_Dungeon )
				return true;

			if ( NULL != g_pData && NULL != g_pData->GetPartyManager() )
			{
				g_pData->GetPartyManager()->UpdateSelectedDungeon();
			}
			
			int iDungeonID = pButton_Dungeon->GetDummyInt( 0 );

			if( g_pData->GetPartyManager()->DoIHaveParty() )
			{
				if( iDungeonID != g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )
				{
#ifdef HENIR_TEST
					int iDungeonMode = CX2Dungeon::DM_HENIR_PRACTICE;	// 이전 코드가 기본으로 헤니르 일반으로 되어 있었음

				//{{ kimhc // 2010.3.19 //	비밀던전 개편 작업
		#ifdef	SERV_SECRET_HELL
					// 비밀던전인 경우에는 DM_SECRET_NORMAL 로
					CX2Dungeon::DUNGEON_TYPE eDungeonType = static_cast<CX2Dungeon::DUNGEON_TYPE>( g_pData->GetDungeonManager()->GetDungeonType( iDungeonID ) );
					
					switch ( eDungeonType )
					{
					default:
						break;

					case CX2Dungeon::DT_NORMAL:
						iDungeonMode = CX2Dungeon::DM_INVALID;
						break;

					case CX2Dungeon::DT_SECRET:
						iDungeonMode = CX2Dungeon::DM_SECRET_NORMAL;
						break;
					}
					
		#endif	SERV_SECRET_HELL
			//}} kimhc // 2010.3.19 //	비밀던전 개편 작업
					g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_DUNGEON_REQ( iDungeonID, (int) CX2Dungeon::DL_NORMAL, iDungeonMode );
#else HENIR_TEST
//{{AFX
					g_pData->GetPartyManager()->Handler_EGS_PARTY_CHANGE_DUNGEON_REQ( iDungeonID, (int) CX2Dungeon::DL_NORMAL, 1 );
//}}AFX
#endif HENIR_TEST				
				}
			}
			else
			{
				int iDungeonMode = CX2Dungeon::DM_HENIR_PRACTICE;
				// 아직 파티를 만들지 않았다면
				g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID			= ( iDungeonID > 0 ? iDungeonID : SEnum::DI_RUBEN_EL_TREE_NORMAL );
				g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty	= (int) CX2Dungeon::DL_NORMAL;
#ifdef HENIR_TEST
					//{{ kimhc // 2010.3.19 //	비밀던전 개편 작업
		#ifdef	SERV_SECRET_HELL
				// 비밀던전인 경우에는 DM_SECRET_NORMAL 로
				CX2Dungeon::DUNGEON_TYPE eDungeonType = static_cast<CX2Dungeon::DUNGEON_TYPE>( g_pData->GetDungeonManager()->GetDungeonType( iDungeonID ) );
				

				switch ( eDungeonType )
				{
				default:
				case CX2Dungeon::DT_NORMAL:
					iDungeonMode = CX2Dungeon::DM_INVALID;
					break;

				case CX2Dungeon::DT_HENIR:
					iDungeonMode = CX2Dungeon::DM_HENIR_PRACTICE;
					break;

				case CX2Dungeon::DT_SECRET:
					iDungeonMode = CX2Dungeon::DM_SECRET_NORMAL;
					break;
				}
		#endif	SERV_SECRET_HELL
					//}} kimhc // 2010.3.19 //	비밀던전 개편 작업

				g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode = iDungeonMode;
#endif HENIR_TEST

				if( NULL != g_pMain->GetPartyUI() )
				{
					g_pMain->GetPartyUI()->UpdatePartyDLG( true );
				}
			}

#ifdef REFORM_NOVICE_GUIDE
			if( NULL != g_pData && NULL != g_pData->GetPlayGuide()
				&& true == g_pData->GetPlayGuide()->GetActiveNoviceGuide())
			{
				switch( g_pData->GetPlayGuide()->GetGuideStep() )
				{
				case CX2PlayGuide::NGS_STEP_6:
				case CX2PlayGuide::NGS_STEP_8:
					{
						g_pData->GetPlayGuide()->SetMouseClickIndex( CX2PlayGuide::NGS_STEP_7 );
					} break;
				}
			}
#else
			// 초심자 가이드
			if( g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL )
			{
				if( g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true && g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP6_2 )
				{
					if( iDungeonID == SEnum::DI_RUBEN_EL_TREE_NORMAL )
					{
						g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_COMPLETE);
					}
					else
					{
						g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_ACCEPT);
					}
				}
			}
#endif //REFORM_NOVICE_GUIDE
			return true;
		} break;

	case LMUCM_DUNGEON_GATE_SELECTED:
		{
			CKTDGUIRadioButton* pRadio_LocalID = (CKTDGUIRadioButton*) lParam;
			int localMapID = pRadio_LocalID->GetDummyInt(0);

			OpenLocalMapDLG( true, (CX2LocationManager::LOCAL_MAP_ID) localMapID );
			UpdateLocalMapDLG();

			return true;
		} break;
	
	
	case LMUCM_CLOSE_WINDOW:
		{
			OpenLocalMapDLG( false );

#ifndef REFORM_NOVICE_GUIDE
			// 초심자 가이드
			if( g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL )
			{
				if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP6_2 )
				{
					g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_ACCEPT);
				}				
			}
#endif //REFORM_NOVICE_GUIDE
			return true;
		} break;

	}

	return false;
}


void CX2LocalMapUI::SetStage( CKTDXStage* pStage )
{
	m_pStage = pStage;

	map< CX2LocationManager::LOCAL_MAP_ID, CKTDGUIDialogType >::iterator it;
	for( it = m_mapLocalDLG.begin(); it != m_mapLocalDLG.end(); it++ )
	{
		CKTDGUIDialogType pDialog = it->second;
		pDialog->SetStage( pStage );
	}
}






void CX2LocalMapUI::OpenLocalMapDLG( bool bOpen, CX2LocationManager::LOCAL_MAP_ID eLocalMapID /*= CX2LocationManager::LMI_INVALID*/ )
{
	if( CX2LocationManager::LMI_INVALID == eLocalMapID )
	{
		eLocalMapID = m_ePickedLocalMapID;
	}

	if( CX2LocationManager::LMI_INVALID == eLocalMapID )
	{
		eLocalMapID = CX2LocationManager::LMI_RUBEN;
	}

	/************************************************************************/
	/**/
#ifdef DUNGEON_SELECT_ALL_CLOSE
	if( true == bOpen &&
		bOpen != m_bShowLocalMap )
	{
		CloseOtherUI();
	}
#endif //DUNGEON_SELECT_ALL_CLOSE

	m_bShowLocalMap = bOpen;
	/************************************************************************/

	if( false == bOpen )
	{
		CKTDGUIDialogType pDialog = GetLocalMapDLG( m_ePickedLocalMapID );
		if( NULL != pDialog )
		{
			pDialog->SetShowEnable( false, false );
		}
#ifdef SERV_EPIC_QUEST
		m_pDLGQuestDesc->SetShow(false);
#endif SERV_EPIC_QUEST
		m_pDlgHeroRecruit->SetShowEnable(false, false);
	}
	else
	{
		CKTDGUIDialogType pDialog = NULL;

		// 이전 로컵맵 dialog를 끄고
		if( m_ePickedLocalMapID != eLocalMapID )
		{
			CKTDGUIDialogType pDialog = GetLocalMapDLG( m_ePickedLocalMapID );
			if( NULL != pDialog )
			{
				pDialog->SetShowEnable( false, false );
			}
		}


		// 새 로컬맵 dialog를 연다
		m_ePickedLocalMapID = eLocalMapID;
		pDialog = GetLocalMapDLG( eLocalMapID );
		if( NULL == pDialog )
		{
			pDialog = CreateLocalMapDLG( eLocalMapID );
			ASSERT( NULL != pDialog );

		}

		pDialog->SetShowEnable( true, true );

		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgHeroRecruit, true );
		m_pDlgHeroRecruit->SetShowEnable(true, true);
	}
}





void CX2LocalMapUI::ClearLocalMapDLGs()
{
	map< CX2LocationManager::LOCAL_MAP_ID, CKTDGUIDialogType >::iterator it;
	for( it = m_mapLocalDLG.begin(); it != m_mapLocalDLG.end(); it++ )
	{
		SAFE_DELETE_DIALOG( it->second );
	}
	m_mapLocalDLG.clear();

	m_bShowLocalMap = false;
}

// 던전 클리어 정보 기타 등등을 설정
void CX2LocalMapUI::UpdateLocalMapDLG()
{
	CKTDGUIDialogType pDialog = GetLocalMapDLG( m_ePickedLocalMapID );
	if( NULL != pDialog )
	{
		for( int i=0; i<pDialog->GetControlSize(); i++ )
		{
			CKTDGUIControl* pControl = pDialog->GetControl( i );
			if( NULL == pControl )
				continue;

			if( CKTDGUIControl::UCT_RADIOBUTTON != pControl->GetType() )
				continue;

#ifdef QUEST_GUIDE
			bool bShowRadioButton = true;
#endif //QUEST_GUIDE
			if( pControl->GetDummyInt( 0 ) == (int) m_ePickedLocalMapID )
			{
				CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*>(pControl);
				pRadio->SetChecked( true );
#ifdef QUEST_GUIDE
				//해당 탭이 열려있으면 Q마크 보여주지 않기
				bShowRadioButton = false;
#endif //QUEST_GUIDE
				//break;
			}



			// 아직 갈수 없는 마을이면 안보이게
			if( -1 != pControl->GetDummyInt( 0 ) ) // 던전게이트 선택하는 라디오 버튼이 맞으면
			{
				pControl->SetShowEnable( true, true );

				CX2LocationManager::LocalMapTemplet* pLocalMapTemplet =	
					g_pData->GetLocationManager()->GetLocalMapTemplet( (CX2LocationManager::LOCAL_MAP_ID) pControl->GetDummyInt( 0 ) );

#ifdef SCRIPT_SELECT
				if(pLocalMapTemplet == NULL)
				{
					pControl->SetShowEnable(false, false);
					continue;
				}
#endif

#ifdef QUEST_GUIDE	
				CKTDGUIControl::CPictureData* pPicture = NULL;
				CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(pDialog->GetControl(L"QuestGuide"));
				if( NULL != pStatic && NULL != pStatic->GetPictureIndex( pControl->GetDummyInt( 0 ) ) )
				{
					pPicture = static_cast<CKTDGUIControl::CPictureData*>(pStatic->GetPictureIndex( pControl->GetDummyInt( 0 ) ));
					pPicture->SetShow(false);
				}
#endif //QUEST_GUIDE
				// 운영자 이상은 모든 마을 볼 수 있게
				if( NULL != g_pData->GetMyUser() &&
					g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
				{
#ifdef QUEST_GUIDE					
					//운영자 이상은 진입 가능 여부 상관 없이 퀘스트 가이드 체크
					if( true == bShowRadioButton )
					{
						CX2QuestManager* pQuestManager = g_pData->GetQuestManager();
						if( NULL != pQuestManager )
						{
							map<SEnum::DUNGEON_ID, bool> SetNeedToClearDungeonList;
							if( true == pQuestManager->GetNeedToClearDungeonList(SetNeedToClearDungeonList) ) //클리어가 필요한 dungeon 목록	
							{
								for( UINT i=0; i< pLocalMapTemplet->m_DungeonList.size(); ++i )
								{	
									map<SEnum::DUNGEON_ID, bool>::iterator mitDungeon = SetNeedToClearDungeonList.find(pLocalMapTemplet->m_DungeonList[i]);
									if( mitDungeon != SetNeedToClearDungeonList.end() && NULL != pPicture )
									{
										pPicture->SetFlicker( 0.5f, 1.0f, 0.1f );
										pPicture->SetShow(true);
										break; 
									}
								}
							}
						}
					}
#endif //QUEST_GUIDE
					continue;
				}

				if( NULL != pLocalMapTemplet )
				{
#ifdef FIX_LOCALMAP_OPEN_CONDITION
					if( g_pData->GetSelectUnitLevel() < pLocalMapTemplet->m_RequireUnitLevel )
					{
						pControl->SetShowEnable(false,false);
	#ifdef QUEST_GUIDE
						bShowRadioButton = false;
	#endif //QUEST_GUIDE
					}
#else //FIX_LOCALMAP_OPEN_CONDITION
					if( pLocalMapTemplet->m_vecRequireClearDungeonID.size() > 0  )
					{						
						if( false == g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( pLocalMapTemplet->m_vecRequireClearDungeonID ) ||
							g_pData->GetSelectUnitLevel() < pLocalMapTemplet->m_RequireUnitLevel )
						{
							pControl->SetShowEnable( false, false );
	#ifdef QUEST_GUIDE
							//진입 불가능한 탭은 Q마크 보여주지 않기
							bShowRadioButton = false;
	#endif //QUEST_GUIDE
						}												
					}
#endif //FIX_LOCALMAP_OPEN_CONDITION

#ifdef QUEST_GUIDE					
					//진입 가능한 던전게이트, 열려있지 않은 탭에 대해서만 퀘스트 가이드 체크
					if( true == bShowRadioButton )
					{					
						CX2QuestManager* pQuestManager = g_pData->GetQuestManager();
						if( NULL != pQuestManager )
						{
							map<SEnum::DUNGEON_ID, bool> SetNeedToClearDungeonList;
							if( true == pQuestManager->GetNeedToClearDungeonList(SetNeedToClearDungeonList) )//클리어가 필요한 dungeon 목록	
							{
								for( UINT i=0; i< pLocalMapTemplet->m_DungeonList.size(); ++i )
								{
									map<SEnum::DUNGEON_ID, bool>::iterator mitDungeon = SetNeedToClearDungeonList.find(pLocalMapTemplet->m_DungeonList[i]);
									if( mitDungeon != SetNeedToClearDungeonList.end() && NULL != pPicture )
									{
										pPicture->SetShow(true);
										break; 
									}
								}
							}
						}
					}
#endif //QUEST_GUIDE
				}
			}
		}
	}

	UpdateDungeonButtons( m_ePickedLocalMapID );
	if( NULL != g_pData && NULL != g_pData->GetPartyManager() )
		g_pData->GetPartyManager()->Send_EGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT();
	UpdateHeroRecruitButton();
}

CKTDGUIDialogType CX2LocalMapUI::CreateLocalMapDLG( CX2LocationManager::LOCAL_MAP_ID eLocalMapID )
{
	CX2LocationManager::LocalMapTemplet* pLocalMapTemplet =	g_pData->GetLocationManager()->GetLocalMapTemplet( eLocalMapID );
	if( NULL == pLocalMapTemplet )
		return NULL;

	CKTDGUIDialogType pDialog = new CKTDGUIDialog( m_pStage, L"DLG_UI_Local_Map_new.lua" );	
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialog );
	m_mapLocalDLG[ eLocalMapID ] = pDialog;

	KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

    if ( g_pKTDXApp->LoadAndDoMemory( &kLuamanager, pLocalMapTemplet->m_ScriptFileName.c_str() ) == false )
    {
		ASSERT( !"CreateLocalMapDLG() pLocalMapTemplet->m_ScriptFileName domemory" ); 
		return pDialog;
    }

	wstring wstrFileName = L"";
	//LUA_GET_VALUE( kLuamanager, "BaseWorldStateID", m_BaseWorldStateID, 0 );
	//LUA_GET_VALUE( kLuamanager, "DLGFront", dlgFrontFileName, "" );

	LUA_GET_VALUE( kLuamanager, "LOCAL_MAP_DLG", wstrFileName, L"" );
	ASSERT( false == wstrFileName.empty() );
	if( true == wstrFileName.empty() )
		return pDialog;

	pDialog->OpenScriptFile( wstrFileName.c_str() );

#ifdef SERV_NEW_EVENT_TYPES
	vector<CKTDGUIControl*>& vecControlList = pDialog->GetControlList();
	for( unsigned int uiControlIndex = 0; uiControlIndex < vecControlList.size(); ++uiControlIndex )
	{
		int iDungeonID = vecControlList[uiControlIndex]->GetDummyInt( 0 );
		if( iDungeonID < SEnum::DI_RUBEN_EL_TREE_NORMAL || iDungeonID > SEnum::DI_END )
			continue;

		if( g_pData->GetDungeonManager()->IsDungeonEnable( iDungeonID ) == false )
		{
			vecControlList[uiControlIndex]->SetShowEnable( false, false );
		}
	}
#endif SERV_NEW_EVENT_TYPES

	for( UINT i=0; i< pLocalMapTemplet->m_DungeonList.size(); i++ )
	{
		CreateDungeonClearStars( eLocalMapID, pLocalMapTemplet->m_DungeonList[i] ); 
	}

#ifdef SERV_EPIC_QUEST
	for( UINT i=0; i< pLocalMapTemplet->m_DungeonList.size(); i++ )
	{
		CreateLocalQuestPicture( eLocalMapID, pLocalMapTemplet->m_DungeonList[i] ); 
	}
#endif SERV_EPIC_QUEST

#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
	for( UINT i=0; i< pLocalMapTemplet->m_DungeonList.size(); i++ )
	{
		CreateLocalTearOfELWoman( eLocalMapID, pLocalMapTemplet->m_DungeonList[i] ); 
	}
#endif SERV_EVENT_TEAR_OF_ELWOMAN

	return pDialog;
}






CKTDGUIDialogType CX2LocalMapUI::GetLocalMapDLG( CX2LocationManager::LOCAL_MAP_ID eLocalMapID )
{
	map< CX2LocationManager::LOCAL_MAP_ID, CKTDGUIDialogType >::iterator it = 
		m_mapLocalDLG.find( eLocalMapID );

	if( it != m_mapLocalDLG.end() )
		return it->second;

	return NULL;
}


// 던전 버튼을 활성화, 비활성화, 새로 열린 던전 표시 등등
void CX2LocalMapUI::UpdateDungeonButtons( CX2LocationManager::LOCAL_MAP_ID eLocalMapID )
{
	CKTDGUIDialogType pDialog = GetLocalMapDLG( eLocalMapID );
	if( NULL == pDialog )
		return; 

#ifdef SERV_EPIC_QUEST
	m_mapLocalMapQuestDesc.clear();
	g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
#endif SERV_EPIC_QUEST

	for ( int i = 0; i < pDialog->GetControlSize(); ++i )
	{
		// 던전 아이콘 활성, 비활성화
		CKTDGUIControl* pControl = (CKTDGUIControl*) pDialog->GetControl( i );
		if ( pControl == NULL )
			continue;

		if( pControl->GetDummyInt(1) != 1 &&	//	  DLT_DUNGEON,
			pControl->GetDummyInt(1) != 2 )		//	  DLT_ARCADE,
			continue;

		SEnum::DUNGEON_ID eDungeonID = (SEnum::DUNGEON_ID) pControl->GetDummyInt( 0 );
		const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( eDungeonID );
		if( NULL == pDungeonData )
		{
#ifdef SCRIPT_SELECT
			pControl->SetShowEnable(false,false);
#endif
			continue;
		}
#ifdef SERV_NEW_EVENT_TYPES
		else if( g_pData->GetDungeonManager()->IsDungeonEnable( (int)eDungeonID ) == false )
		{
#ifdef SCRIPT_SELECT
			pControl->SetShowEnable(false,false);
#endif
			if( true == pDungeonData->m_bHellMode )
			{
				wstring controlName = pDungeonData->m_DungeonName;
				controlName += L"_Icon_Info";
				CKTDGUIStatic* pStatic_Icon_Info = (CKTDGUIStatic*) pDialog->GetControl( controlName.c_str() );
				if( NULL != pStatic_Icon_Info )
				{
					pStatic_Icon_Info->SetShowEnable( false, false );
				}
			}
			continue;
		}
#endif SERV_NEW_EVENT_TYPES

		//{{ kimhc // 2011-04-27 // 디펜스 던전 시간에 따라, 던전 아이콘 비/활성화 여부 결정
#ifdef SERV_INSERT_GLOBAL_SERVER
		// 디펜스 던전 인 경우
		if( NULL != g_pData->GetDungeonManager() && true == g_pData->GetDungeonManager()->IsDefenceDungeon( eDungeonID ) )
		{
			const bool bActiveDefenceDungeon = g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon();

			if ( true == UpdateDefenceDungeonIcon( pControl, bActiveDefenceDungeon ) )
				#ifdef SERV_EPIC_QUEST
					UdateLocalMapQuestDesc( eLocalMapID, eDungeonID, !(bActiveDefenceDungeon) );
				#endif SERV_EPIC_QUEST

			continue;
		}
#endif SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-27 // 디펜스 던전 시간에 따라, 던전 아이콘 비/활성화 여부 결정


		if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR ||
			true == g_pData->GetDungeonManager()->IsActiveDungeon( (int) eDungeonID, 0 ) )
		{
			pControl->SetShowEnable( true, true );
			UpdateDungeonClearStars( eLocalMapID, eDungeonID, true );
#ifdef SERV_EPIC_QUEST
			UdateLocalMapQuestDesc( eLocalMapID, eDungeonID, true );
#endif SERV_EPIC_QUEST
#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
			UpdateLocalTearOfELWoman( eLocalMapID, eDungeonID, true );
#endif SERV_EVENT_TEAR_OF_ELWOMAN
			if( true == pDungeonData->m_bHellMode )
			{
				wstring controlName = pDungeonData->m_DungeonName;
				controlName += L"_Icon_Info";
				CKTDGUIStatic* pStatic_Icon_Info = (CKTDGUIStatic*) pDialog->GetControl( controlName.c_str() );
				if( NULL != pStatic_Icon_Info )
				{
					pStatic_Icon_Info->SetShowEnable( true, true );
				}
			}
		}
		else
		{
			pControl->SetShowEnable( false, false );
			UpdateDungeonClearStars( eLocalMapID, eDungeonID, false );
#ifdef SERV_EPIC_QUEST
			UdateLocalMapQuestDesc( eLocalMapID, eDungeonID, false );
#endif SERV_EPIC_QUEST
#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
			UpdateLocalTearOfELWoman( eLocalMapID, eDungeonID, false );
#endif SERV_EVENT_TEAR_OF_ELWOMAN
			if( true == pDungeonData->m_bHellMode )
			{
				wstring controlName = pDungeonData->m_DungeonName;
				controlName += L"_Icon_Info";
				CKTDGUIStatic* pStatic_Icon_Info = (CKTDGUIStatic*) pDialog->GetControl( controlName.c_str() );
				if( NULL != pStatic_Icon_Info )
				{
					pStatic_Icon_Info->SetShowEnable( false, false );
				}
			}
		}


		// 새로 열린 던전에 대한 정보를 알려준다
		if ( pControl->GetShow() == true && pControl->GetEnable() == true )
		{
			if( true == g_pData->GetMyUser()->GetSelectUnit()->IsNewlyOpenedDungeon( (int) eDungeonID ) )
			{
				// hardcode!!
				if( eDungeonID == SEnum::DI_ELDER_HALLOWEEN_NORMAL )		// 할로윈 이벤트 던전이면
				{
					CreateNewDungeonParticle( pControl, true );					
				}
				else
				{
					CreateNewDungeonParticle( pControl );						
				}
				DropDownDungeonButton( pControl );	

				g_pKTDXApp->GetDeviceManager()->PlaySound( L"NewStage_Open.ogg", false, false );
			}
		}
	}

	g_pData->GetMyUser()->GetSelectUnit()->ClearNewlyOpenedDungeonAtLocalMap();

}



void CX2LocalMapUI::UpdateDungeonClearStars( CX2LocationManager::LOCAL_MAP_ID eLocalMapID, int dungeonID, bool bShow )
{
	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) dungeonID );
	if( NULL == pDungeonData )
		return;

#ifdef SERV_NEW_EVENT_TYPES
	if( g_pData->GetDungeonManager()->IsDungeonEnable( dungeonID ) == false )
		return;
#endif SERV_NEW_EVENT_TYPES
	CKTDGUIDialogType pDialog = GetLocalMapDLG( eLocalMapID );
	if( NULL == pDialog )
		return;

#ifdef NEW_HENIR_TEST
	if(pDungeonData->m_eDungeonType == CX2Dungeon::DT_HENIR )
	{
		KEGS_HENIR_REWARD_COUNT_NOT m_HenirRewardCountInfo = g_pData->GetMyUser()->GetSelectUnit()->GetHenirRewardCountInfo();

		wstring LimitInfoName = pDungeonData->m_DungeonName;
		LimitInfoName += L"_LimitInfo";
		CKTDGUIStatic* pStaticLimitInfo = (CKTDGUIStatic*) pDialog->GetControl( LimitInfoName.c_str() );
		
		wstring LimitInfoName_Unlimit = pDungeonData->m_DungeonName;
		LimitInfoName_Unlimit += L"_LimitInfo_Unlimit";
		CKTDGUIStatic* pStaticLimitInfo_Unlimit = (CKTDGUIStatic*) pDialog->GetControl( LimitInfoName_Unlimit.c_str() );

		if(m_HenirRewardCountInfo.m_bUnLimited == true)
		{
			pStaticLimitInfo->GetPicture(1)->SetShow( false );
			pStaticLimitInfo_Unlimit->SetShow( true );
		}
		else
		{
			int iHenirRewardCount = 0;

#ifdef NEW_HENIR_DUNGEON // 일반 / 도전에서의 보상 횟수 UI 구분해서 보여주기

			int iHenirChallengeRewareCount = 0;
			{
				iHenirChallengeRewareCount = m_HenirRewardCountInfo.m_iChallengeNormal;
				if(m_HenirRewardCountInfo.m_iPremiumMAX > 0)
				{
					iHenirChallengeRewareCount += m_HenirRewardCountInfo.m_iChallengePremium;
				}
				if(m_HenirRewardCountInfo.m_iEventMAX > 0)
				{
					iHenirChallengeRewareCount += m_HenirRewardCountInfo.m_iChallengeEvent;
				}
			}
#endif // NEW_HENIR_DUNGEON
			{
				iHenirRewardCount = m_HenirRewardCountInfo.m_iNormal;
				if(m_HenirRewardCountInfo.m_iPremiumMAX > 0)
				{
					iHenirRewardCount += m_HenirRewardCountInfo.m_iPremium;
				}
				if(m_HenirRewardCountInfo.m_iEventMAX > 0)
				{
					iHenirRewardCount += m_HenirRewardCountInfo.m_iEvent;
				}
			}

			if( NULL != pStaticLimitInfo && NULL != pStaticLimitInfo->GetPicture(1) )
			{
				if(iHenirRewardCount >= 0 && iHenirRewardCount < 10 )
				{
#ifdef NEW_HENIR_DUNGEON // 일반 / 도전에서의 보상 횟수 UI 구분해서 보여주기
					WCHAR RewardCountKeyName[100] = {0,};
					StringCchPrintf( RewardCountKeyName,	100, L"Bg_HenirNum_%d", iHenirRewardCount );
					pStaticLimitInfo->GetPicture(1)->SetTex( L"DLG_UI_Button21_NEW.tga", RewardCountKeyName );
#else
					WCHAR RewardCountTextureName[100] = {0,};
					StringCchPrintf( RewardCountTextureName,	100, L"PC_%d.dds", iHenirRewardCount );

					pStaticLimitInfo->GetPicture(1)->SetTex(RewardCountTextureName);
#endif // NEW_HENIR_DUNGEON
					pStaticLimitInfo->GetPicture(1)->SetShow( true );
				}
				else
				{
					pStaticLimitInfo->GetPicture(1)->SetShow( false );
				}
			}

#ifdef NEW_HENIR_DUNGEON // 일반 / 도전에서의 보상 횟수 UI 구분해서 보여주기
			if( NULL != pStaticLimitInfo && NULL != pStaticLimitInfo->GetPicture(3) )
			{
				if(iHenirChallengeRewareCount >= 0 && iHenirChallengeRewareCount < 10 )
				{
					WCHAR RewardCountKeyName[100] = {0,};
					StringCchPrintf( RewardCountKeyName,	100, L"Bg_HenirNum_%d_Challenge", iHenirChallengeRewareCount );
					pStaticLimitInfo->GetPicture(3)->SetTex( L"DLG_UI_Button21_NEW.tga", RewardCountKeyName );
					pStaticLimitInfo->GetPicture(3)->SetShow( true );
				}
				else
				{
					pStaticLimitInfo->GetPicture(3)->SetShow( false );
				}
			}
#endif // NEW_HENIR_DUNGEON

			pStaticLimitInfo_Unlimit->SetShow( false );
		}
	}
#endif NEW_HENIR_TEST

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	if(pDungeonData->m_DungeonID == SEnum::DI_EVENT_VALENTINE_DUNGEON_INT )
	{
		int iRewardCount = 3 - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().GetValentineItemCount();

		wstring LimitInfoName = pDungeonData->m_DungeonName;
		LimitInfoName += L"_LimitInfo";
		CKTDGUIStatic* pStaticLimitInfo = (CKTDGUIStatic*) pDialog->GetControl( LimitInfoName.c_str() );

		int iHenirRewardCount = 0;

		if( NULL != pStaticLimitInfo && NULL != pStaticLimitInfo->GetPicture(1) )
		{
			WCHAR RewardCountKeyName[100] = {0,};
			StringCchPrintf( RewardCountKeyName,	100, L"PC_%d.dds", iRewardCount );
			pStaticLimitInfo->GetPicture(1)->SetTex( RewardCountKeyName );
			pStaticLimitInfo->GetPicture(1)->SetShow( true );
		}
	}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

	//{{ kimhc // 2010-07-15 // 임시 코드 // 구현방법이 다시 정해지면 삭제해야함
	if ( pDungeonData->m_eDungeonType != CX2Dungeon::DT_NORMAL || 
		 pDungeonData->m_bNormalOnly == true )
		return;
	//}} kimhc // 2010-07-15 // 임시 코드 // 구현방법이 다시 정해지면 삭제해야함

	wstring controlName = pDungeonData->m_DungeonName;

	CKTDGUIStatic* pStaticStar = (CKTDGUIStatic*) pDialog->GetControl( controlName.c_str() );
	if( NULL == pStaticStar )
		return;

	pStaticStar->SetShowEnable( bShow, bShow );
	
	if( bShow == false )
		return; 
	
	for ( int i = 0; i < 3; i++ )
	{
		if(	NULL == pStaticStar->GetPicture( i + 1 ) )
			continue;

		if ( g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( dungeonID + i ) == true ) 
		{
			pStaticStar->GetPicture( i + 1 )->SetShow( true );
		}
		else
		{
			pStaticStar->GetPicture( i + 1 )->SetShow( false );
		}
	}
}




// 던전 버튼 하단에 보통, 어려움, 매우 어려움 클리어 정보 표시 UI 생성
void CX2LocalMapUI::CreateDungeonClearStars( CX2LocationManager::LOCAL_MAP_ID eLocalMapID, int dungeonID )
{
	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) dungeonID );
	if( NULL == pDungeonData )
		return;

#ifdef SERV_NEW_EVENT_TYPES
	if( g_pData->GetDungeonManager()->IsDungeonEnable( dungeonID ) == false )
		return;
#endif SERV_NEW_EVENT_TYPES
	CKTDGUIDialogType pDialog = GetLocalMapDLG( eLocalMapID );
	if( NULL == pDialog )
		return; 


	//SEnum::DUNGEON_ID dungeonIDToFind = (SEnum::DUNGEON_ID)dungeonID;

	//CX2LocationManager::LocalMapTemplet* pLocalMapTemplet = 
	//	g_pData->GetLocationManager()->GetLocalMapTemplet( (CX2LocationManager::LOCAL_MAP_ID)g_pMain->GetNowDetailStateID() );
	//if ( pLocalMapTemplet == NULL )
	//	return;

	//bool bCheckFind = false;
	//for ( int i = 0; i < (int)pLocalMapTemplet->m_DungeonList.size(); i++ )
	//{
	//	SEnum::DUNGEON_ID tempDungeonID = pLocalMapTemplet->m_DungeonList[i];
	//	if ( tempDungeonID == dungeonIDToFind )
	//	{
	//		bCheckFind = true;
	//	}
	//}

	//if ( bCheckFind == false )
	//	return;

	//const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( dungeonIDToFind );
	//if ( pDungeonData == NULL )
	//	return;
	//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON	// 페이타에서만 할로윈 던전 위치 수정에 따른 별 표시 위치 수정
	if( eLocalMapID == CX2LocationManager::LMI_PEITA )
	{
			if( (SEnum::DUNGEON_ID) dungeonID == SEnum::DI_ELDER_HALLOWEEN_NORMAL 
				|| (SEnum::DUNGEON_ID) dungeonID == SEnum::DI_ELDER_HALLOWEEN_HARD
				|| (SEnum::DUNGEON_ID) dungeonID == SEnum::DI_ELDER_HALLOWEEN_EXPERT )
			pStaticStar->MoveControl( -40.f, 0.f );
	}
#endif SERV_HALLOWEEN_DUNGEON
	//}}

	//for ( int i = 0; i < 3; i++ )
	//{
	//	if(	NULL == pStaticStar->GetPicture( i + 1 ) )
	//		continue;

	//	if ( g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( dungeonID + i ) == true ) 
	//	{
	//		pStaticStar->GetPicture( i + 1 )->SetShow( true );
	//	}
	//	else
	//	{
	//		pStaticStar->GetPicture( i + 1 )->SetShow( false );
	//	}
	//}
#ifdef NEW_HENIR_TEST
	if ( pDungeonData->m_eDungeonType == CX2Dungeon::DT_HENIR )
	{
		pDialog->OpenScriptFile( L"DLG_Map_Local_Henir_limit.lua" );
		CKTDGUIStatic* pStaticLimitInfo = (CKTDGUIStatic*) pDialog->GetControl( L"limit_info" );

		wstring LimitInfoName = pDungeonData->m_DungeonName;
		LimitInfoName += L"_LimitInfo";
		pStaticLimitInfo->SetName( LimitInfoName.c_str() );
		wstring controlName = pDungeonData->m_DungeonName;
		//별 위치 기준으로 헤니르 제한 횟수 위치 수정
		D3DXVECTOR2 LocalQuestPos;
		CKTDGUIStatic* pStaticStar = (CKTDGUIStatic*) pDialog->GetControl( controlName.c_str() );
		if( NULL != pStaticStar )
		{
			LocalQuestPos = pStaticStar->GetPicture(0)->GetPos();
		}
		LocalQuestPos.y -= 90.f;	
		LocalQuestPos.x -= 20.f;	
		pStaticLimitInfo->SetOffsetPos( LocalQuestPos );


		CKTDGUIStatic* pStaticLimitInfo_Unlimit = (CKTDGUIStatic*) pDialog->GetControl( L"g_pStatic_limit_info3" );

		wstring LimitInfoName_Unlimit = pDungeonData->m_DungeonName;
		LimitInfoName_Unlimit += L"_LimitInfo_Unlimit";
		pStaticLimitInfo_Unlimit->SetName( LimitInfoName_Unlimit.c_str() );
		pStaticLimitInfo_Unlimit->GetString(0)->msg = GET_STRING(STR_ID_13802);

		D3DXVECTOR2 LocalStartPos = LocalQuestPos ;
		LocalStartPos.x += (60.f);
		LocalStartPos.y += (15.f);
		pStaticLimitInfo_Unlimit->SetOffsetPos( LocalStartPos );

		pStaticLimitInfo_Unlimit->SetShow( false );
	}
#endif NEW_HENIR_TEST

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	if ( pDungeonData->m_DungeonID == SEnum::DI_EVENT_VALENTINE_DUNGEON_INT )
	{
		pDialog->OpenScriptFile( L"DLG_Map_Local_Valentine_limit.lua" );
		CKTDGUIStatic* pStaticLimitInfo = (CKTDGUIStatic*) pDialog->GetControl( L"limit_info" );

		wstring LimitInfoName = pDungeonData->m_DungeonName;
		LimitInfoName += L"_LimitInfo";
		pStaticLimitInfo->SetName( LimitInfoName.c_str() );
		wstring controlName = pDungeonData->m_DungeonName;
		//발렌타인 보상 제한 횟수
		D3DXVECTOR2 LocalQuestPos;
		CKTDGUIStatic* pStaticStar = (CKTDGUIStatic*) pDialog->GetControl( controlName.c_str() );
		if( NULL != pStaticStar )
		{
			LocalQuestPos = pStaticStar->GetPicture(0)->GetPos();
		}

		LocalQuestPos.y -= 30.f;		
		pStaticLimitInfo->SetOffsetPos( LocalQuestPos );
	}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
}



void CX2LocalMapUI::DropDownDungeonButton( CKTDGUIControl* pControl )
{
	if( NULL == pControl )
		return;


	// 던전버튼 떨어지기
	pControl->SetOffsetPos( D3DXVECTOR2( 0 , -200 ) );
	pControl->SetColor( D3DXCOLOR( 1,1,1,0 ) );
	pControl->Move( D3DXVECTOR2( 0, 0 ), D3DXCOLOR( 1,1,1,1 ), 0.4f, true );
	pControl->Move( D3DXVECTOR2( 0, -10 ), D3DXCOLOR( 1,1,1,1 ), 0.2f, false );
	pControl->Move( D3DXVECTOR2( 0, 0 ), D3DXCOLOR( 1,1,1,1 ), 0.2f, false );
	pControl->Move( D3DXVECTOR2( 0, -5 ), D3DXCOLOR( 1,1,1,1 ), 0.1f, false );
	pControl->Move( D3DXVECTOR2( 0, 0 ), D3DXCOLOR( 1,1,1,1 ), 0.05f, false );


	// 로컬맵 전체 흔들리기
	CKTDGUIDialogType pDialog = GetLocalMapDLG( m_ePickedLocalMapID );
	if( NULL == pDialog )
		return;

	pDialog->Move( D3DXVECTOR2( 0,0), D3DXCOLOR( 1,1,1,1), 0.4f, true );
	pDialog->Move( D3DXVECTOR2( 0,10), D3DXCOLOR( 1,1,1,1), 0.2f, false );
	pDialog->Move( D3DXVECTOR2( 0,0), D3DXCOLOR( 1,1,1,1), 0.1f, false );

}





// fix!! 일단은 한번 New생성되면 안 없어지는 것으로 
void CX2LocalMapUI::CreateNewDungeonParticle( CKTDGUIControl* pControl, bool bEventDungeon /*= false*/ )
{
	// fix!! not yet
	//
	//
	//

	//if ( m_pDLGBackStarBG != NULL && pControl != NULL )
	//{
	//	CKTDGUIStatic* pStatic = new CKTDGUIStatic();
	//	wstringstream controlName;
	//	controlName << pControl->GetName();
	//	controlName << m_pDLGBackStarBG->GetControlSize(); 
	//	pStatic->SetName( controlName.str().c_str() );
	//	m_pDLGBackStarBG->AddControl( pStatic );


	//	CKTDGUIControl::CPictureData* pCPictureData = new CKTDGUIControl::CPictureData();
	//	if( true == bEvent )
	//	{
	//		pCPictureData->SetTex( L"DLG_COMMON_NEW_TEXTURE52.TGA", L"EVENT" );
	//	}
	//	else
	//	{
	//		pCPictureData->SetTex( L"DLG_COMMON_TEXTURE22.TGA", L"New_Dungeon" );
	//	}
	//	
	//	D3DXVECTOR2 newTexSize = D3DXVECTOR2( 88, 66 );
	//	pCPictureData->SetSize( newTexSize );
	//	pStatic->AddPicture( pCPictureData );
	//	CKTDGUIButton* pButton = (CKTDGUIButton*)pControl;
	//	pStatic->SetOffsetPos( D3DXVECTOR2( pButton->GetPos().x + (pButton->GetWidth() / 2.0f) - (newTexSize.x / 2), pButton->GetPos().y - newTexSize.y - 7 ) );

	//	CKTDGParticleSystem::CParticleEventSequenceHandle hCParticleEventSequence = 
	//		g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Aurora3", pButton->GetPos().x + (pButton->GetWidth() / 2.0f), 
	//		pButton->GetPos().y + ( pButton->GetHeight() / 2.0f ), 0.0f,
	//		50, 50, 1, -1, 99999.0f );

	//	g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"DungeonDownSmoke", pButton->GetPos().x + (pButton->GetWidth() / 2.0f), 
	//		pButton->GetPos().y + ( pButton->GetHeight() * ( 2/3.0f ) ), 0.0f,
	//		100, 100, -1, 7, -1 );

	//	m_vecParticleNewDungeon.push_back( hCParticleEventSequence );
	//}

}

#ifdef SERV_EPIC_QUEST
void CX2LocalMapUI::DrawLocalMapQuestDesc(D3DXVECTOR2 DrawPos, int iDungeonID)
{
	wstring wstrItemDesc;
	g_pData->GetUIManager()->GetUIQuestNew()->GetLocalMapQuestDesc(iDungeonID, wstrItemDesc);
	
	if(wstrItemDesc == L"")
	{
		m_pDLGQuestDesc->SetShow( false );
		return;
	}
	int iMagicAddSizeX = 40;
	int iMagicAddSizeY = 40;

	CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX_SMALL );
	int itemDescWidth = (int)( pItemDescFont->GetWidth( wstrItemDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + iMagicAddSizeX;
	int itemDescHeight = (int)( pItemDescFont->GetHeight( wstrItemDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + iMagicAddSizeY;

	// 아이템 툴팁의 길이가 너무 길면 폰트 크기를 줄이자
	CKTDGUIStatic* pItemDesc = (CKTDGUIStatic*)m_pDLGQuestDesc->GetControl(L"ItemDesc");
	pItemDesc->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;	


	int offsetSize = 0;
	int lineNum = 0;
	offsetSize = wstrItemDesc.find( L"\n", offsetSize );
	while( offsetSize != -1 )
	{
		offsetSize++;
		lineNum++;
		offsetSize = wstrItemDesc.find( L"\n", offsetSize );
	}
	itemDescHeight += lineNum;

	int itemDescPosX = (int)DrawPos.x;
	int itemDescPosY = (int)DrawPos.y;



	// 아이템 설명 위부분이 화면 밖으로 나가지 않도록 제한
	if( itemDescPosY < 0 )
		itemDescPosY = 0;

	m_pDLGQuestDesc->SetPos( D3DXVECTOR2( (float)itemDescPosX + 140, (float)itemDescPosY - 70 ) );
	m_pDLGQuestDesc->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)itemDescHeight) );
	m_pDLGQuestDesc->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );



	m_pDLGQuestDesc->GetStatic_LUA( "ItemDesc" )->GetString(0)->msg = wstrItemDesc.c_str();

	m_pDLGQuestDesc->SetShow( true );


	return;
}

void CX2LocalMapUI::UdateLocalMapQuestDesc( CX2LocationManager::LOCAL_MAP_ID eLocalMapID, int dungeonID, bool bShow )
{
#ifdef SERV_NEW_DEFENCE_DUNGEON 
	if( NULL != g_pData->GetDungeonManager() &&
		true == g_pData->GetDungeonManager()->IsDefenceDungeon( dungeonID ) )
		return;
#endif //SERV_NEW_DEFENCE_DUNGEON

	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) dungeonID );
	if( NULL == pDungeonData )
		return;

#ifdef SERV_NEW_EVENT_TYPES
	if( g_pData->GetDungeonManager()->IsDungeonEnable( dungeonID ) == false )
		return;
#endif SERV_NEW_EVENT_TYPES
	CKTDGUIDialogType pDialog = GetLocalMapDLG( eLocalMapID );
	if( NULL == pDialog )
		return;

	wstring wstrItemDesc;
	bool bShowQuest = g_pData->GetUIManager()->GetUIQuestNew()->GetLocalMapQuestDesc(dungeonID, wstrItemDesc);
	m_mapLocalMapQuestDesc.insert( std::make_pair( dungeonID, wstrItemDesc ) );

	wstring LocalQuestName = pDungeonData->m_DungeonName;
	LocalQuestName += L"_LocalQuest";
	
	CKTDGUIStatic* pStaticLocalQuest = (CKTDGUIStatic*) pDialog->GetControl( LocalQuestName.c_str() );
	if ( NULL == pStaticLocalQuest )
	{
		ASSERT( !L"pStaticLocalQuest is NULL" );
		return;
	}

	if( true == bShowQuest )
	{
#ifdef HIDE_QUEST_MARK_INACTIVATION_SECRET_DUNGEON

		/// 활성화된 던전인 경우 와 개발자 계정인 경우에만 Q마크를 보여주기
		if ( g_pData->GetDungeonManager()->IsActiveDungeon( dungeonID, 0 )
			|| ( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR ) )
			pStaticLocalQuest->SetShow( true );
		else
			pStaticLocalQuest->SetShow(false);  
#else
		pStaticLocalQuest->SetShow(true);   
#endif HIDE_QUEST_MARK_INACTIVATION_SECRET_DUNGEON		
	}
	else
	{	
		pStaticLocalQuest->SetShow(false);
	}

	return;
}


void CX2LocalMapUI::CreateLocalQuestPicture( CX2LocationManager::LOCAL_MAP_ID eLocalMapID, int dungeonID )
{
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( NULL != g_pData->GetDungeonManager() &&
		true == g_pData->GetDungeonManager()->IsDefenceDungeon( dungeonID ) )
		return;
#endif // SERV_NEW_DEFENCE_DUNGEON


	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) dungeonID );
	if( NULL == pDungeonData )
		return;

#ifdef SERV_NEW_EVENT_TYPES
	if( g_pData->GetDungeonManager()->IsDungeonEnable( dungeonID ) == false )
		return;
#endif SERV_NEW_EVENT_TYPES

	CKTDGUIDialogType pDialog = GetLocalMapDLG( eLocalMapID );
	if( NULL == pDialog )
		return; 

	CKTDGUIStatic* pStatic = new CKTDGUIStatic();
	pDialog->AddControl( pStatic );
	CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
	pStatic->AddPicture( pPicture );
	pPicture->SetTex( L"DLG_UI_Common_Texture13.tga",  L"Quset_Notice_Big");
	pPicture->SetPoint();
	pPicture->SetFlicker( 0.5f, 1.0f, 0.1f );

	wstring LocalQuestName = pDungeonData->m_DungeonName;
	LocalQuestName += L"_LocalQuest";
	pStatic->SetName( LocalQuestName.c_str() );
	wstring controlName = pDungeonData->m_DungeonName;
//별 위치 기준으로 퀘스트 위치 지정
	CKTDGUIStatic* pStaticStar = (CKTDGUIStatic*) pDialog->GetControl( controlName.c_str() );
	if( NULL == pStaticStar )
		return;

	D3DXVECTOR2 LocalQuestPos = pStaticStar->GetPicture(0)->GetPos();
	LocalQuestPos.x += 3.f;	 
	LocalQuestPos.y -= 80.f;	 
	pStatic->SetOffsetPos( LocalQuestPos );
	pStatic->ScaleControl( 0.6f, 0.6f );
	pStatic->SetShow(false);

	//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON	// 페이타에서만 할로윈 던전 위치 수정에 따른 별 표시 위치 수정
	if( eLocalMapID == CX2LocationManager::LMI_PEITA )
	{
		if( (SEnum::DUNGEON_ID) dungeonID == SEnum::DI_ELDER_HALLOWEEN_NORMAL 
			|| (SEnum::DUNGEON_ID) dungeonID == SEnum::DI_ELDER_HALLOWEEN_HARD
			|| (SEnum::DUNGEON_ID) dungeonID == SEnum::DI_ELDER_HALLOWEEN_EXPERT )
			pStatic->MoveControl( -40.f, 0.f );
	}
#endif SERV_HALLOWEEN_DUNGEON
	//}}

	return;
}


#endif SERV_EPIC_QUEST

//{{ kimhc // 2011-04-27 // 디펜스 던전 시간에 따라, 던전 아이콘 비/활성화 여부 결정
#ifdef SERV_INSERT_GLOBAL_SERVER
/** @function : UpdateDefenceDungeonIcon
	@brief : 
	@param : CKTDGUIControl* pControlDefenceDungeon_(던전아이콘 버튼 컨트롤), const bool bActiveDefenceDungeon_(디펜스던전의 open여부)
	@return : bool (Disable아이콘에 변화가 있는 경우 false, 클릭되는 아이콘에 변화가 있는 경우 true)
*/
bool CX2LocalMapUI::UpdateDefenceDungeonIcon( CKTDGUIControl* pControlDefenceDungeon_, const bool bActiveDefenceDungeon_ )
{
	CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>( pControlDefenceDungeon_ );
	const WCHAR* pButtonName = pButton->GetName();

	// Disable 버전의 아이콘 이면
	if ( 0 == _wcsicmp( pButtonName, L"DefenceDungeonDisable" ) )
	{
		// 꺼져 있는 경우
		if ( bActiveDefenceDungeon_ == pButton->GetShow() )	// bActiveDefenceDungeon이 false 인 경우
			pButton->SetShow( !bActiveDefenceDungeon_ ); // true로 셋팅

		return false;
	}
	// Disalbe 버전의 아이콘이 아니면 (클릭되는 아이콘이면)
	else
	{
		// 켜져있는 경우
		if ( bActiveDefenceDungeon_ != pButton->GetShow() )	// bActiveDefenceDungeon이 true인 경우
			pButton->SetShow( bActiveDefenceDungeon_ ); // false로 셋팅

		return true;
	}

}
#endif SERV_INSERT_GLOBAL_SERVER
//}} kimhc // 2011-04-27 // 디펜스 던전 시간에 따라, 던전 아이콘 비/활성화 여부 결정

#ifdef NEW_HENIR_TEST
void CX2LocalMapUI::GetHenirDungeonIcnoDesc( OUT wstringstream& wstrStreamDungeonIconDesc_ )
{
	KEGS_HENIR_REWARD_COUNT_NOT m_HenirRewardCountInfo = g_pData->GetMyUser()->GetSelectUnit()->GetHenirRewardCountInfo();
	
	int iHenirRewardCount = m_HenirRewardCountInfo.m_iNormal;
	if(m_HenirRewardCountInfo.m_iPremiumMAX > 0)
	{
		iHenirRewardCount += m_HenirRewardCountInfo.m_iPremium;
	}
	if(m_HenirRewardCountInfo.m_iEventMAX > 0)
	{
		iHenirRewardCount += m_HenirRewardCountInfo.m_iEvent;
	}	

#ifdef NEW_HENIR_DUNGEON

	int iChallengeHenirRewardCount = m_HenirRewardCountInfo.m_iChallengeNormal;

	if(m_HenirRewardCountInfo.m_iPremiumMAX > 0)
	{
		iChallengeHenirRewardCount += m_HenirRewardCountInfo.m_iChallengePremium;
	}
	if(m_HenirRewardCountInfo.m_iEventMAX > 0)
	{
		iChallengeHenirRewardCount += m_HenirRewardCountInfo.m_iChallengeEvent;
	}

#endif // NEW_HENIR_DUNGEON

		
	wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_13759 ) << '\n' << '\n'; // // 헤니르의 시공에 입장합니다.

	if( m_HenirRewardCountInfo.m_bUnLimited == true )
	{
		wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_13766 ) << '\n'; // [이벤트] 진입 횟수에 상관없이 #CFF0000무제한#CX으로 \n보상을 받으실 수 있습니다.  
	}
	else if(iHenirRewardCount == 0 
#ifdef NEW_HENIR_DUNGEON
			&& iChallengeHenirRewardCount == 0 
#endif // NEW_HENIR_DUNGEON
		)
	{
		wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_13760 ) << '\n'; // 오늘은 헤니르의 시공에서 더 이상 \n보상을 획득할 수 없습니다.\n (던전 입장에는 제한이 없습니다.)
	}
	else
	{
#ifdef NEW_HENIR_DUNGEON // 보상 횟수 가이드 문구
		wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_13761 ) << '\n'; // 오늘 보상을 받을 수 있는 횟수는
		// 일반 보상 얻을 수 있는 횟수
		wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_27305, "i", iHenirRewardCount ) ); // #CFF0000@1#CX번 
		
		if(m_HenirRewardCountInfo.m_iPremiumMAX > 0)
			wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_13764, "i", m_HenirRewardCountInfo.m_iPremiumMAX ) ); // [PC방+@1]
		if(m_HenirRewardCountInfo.m_iEventMAX > 0)
			wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_13765, "i", m_HenirRewardCountInfo.m_iEventMAX ) ); // [이벤트+@1]


		// 도전 보상 얻을 수 있는 횟수
		wstrStreamDungeonIconDesc_  << '\n' << GET_REPLACED_STRING( ( STR_ID_27334, "i", iChallengeHenirRewardCount ) ); // #CFF0000@1#CX번 

		if(m_HenirRewardCountInfo.m_iPremiumMAX > 0)
			wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_13764, "i", m_HenirRewardCountInfo.m_iPremiumMAX ) ); // [PC방+@1]
		if(m_HenirRewardCountInfo.m_iEventMAX > 0)
			wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_13765, "i", m_HenirRewardCountInfo.m_iEventMAX ) ); // [이벤트+@1]
#else
		wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_13761 ) << '\n';
		wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_13763, "i", iHenirRewardCount ) ); // #CFF0000@1#CX번 

		if(m_HenirRewardCountInfo.m_iPremiumMAX > 0)
			wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_13764, "i", m_HenirRewardCountInfo.m_iPremiumMAX ) ); // [PC방+@1]
		if(m_HenirRewardCountInfo.m_iEventMAX > 0)
			wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_13765, "i", m_HenirRewardCountInfo.m_iEventMAX ) ); // [이벤트+@1]
#endif // NEW_HENIR_DUNGEON

		wstrStreamDungeonIconDesc_ << '\n' << GET_STRING( STR_ID_13762 ) << '\n';// 남았습니다.\n (던전 입장에는 제한이 없습니다.)
	}
}

#endif NEW_HENIR_TEST

//영웅모집공고 버튼 업데이트.
void CX2LocalMapUI::UpdateHeroRecruitButton()
{
	if( NULL ==	m_pDlgHeroRecruit )
		return;

	vector<CKTDGUIControl*>::iterator it = m_pDlgHeroRecruit->GetControlList().begin();
	for( ; it != m_pDlgHeroRecruit->GetControlList().end(); it++)
	{
		(*it)->SetShowEnable(false,false);
	}

	if( NULL != g_pData && NULL != g_pData->GetPartyManager() )
	{
		map<wstring, bool> mapHeroRecruitDungeonName = g_pData->GetPartyManager()->GetHeroRecruitDungeonList();
 		if( true == mapHeroRecruitDungeonName.empty() )
 			return;


		map<wstring, bool>::iterator it = mapHeroRecruitDungeonName.begin();
		for( ; it != mapHeroRecruitDungeonName.end(); it++)
		{			
			CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>(m_pDlgHeroRecruit->GetControl( it->first.c_str()));
			if( NULL != pButton )
			{
				//pButton의 정보가 현재 로컬맵에 보여지고 있는 던전이고, 영웅모집공고 활성화 상태라면
				if( m_ePickedLocalMapID == pButton->GetDummyInt(0) && true == it->second)
				{
					pButton->SetShowEnable(true, true);
				}
				else
				{
					pButton->SetShowEnable(false, false);
				}
			}
		}
	}
}



#ifdef DUNGEON_SELECT_ALL_CLOSE //던전선택화면이 활성화 되면 다른 UI들을 닫는다.
void CX2LocalMapUI::CloseOtherUI()
{		
	if( NULL == g_pData || NULL == g_pMain)
		return;

	CX2UIManager* pUIManager = g_pData->GetUIManager();

	if( NULL != pUIManager )
	{
		//인벤토리창이 열려있다면 닫는다
		if(true == pUIManager->GetShow(CX2UIManager::UI_MENU_INVEN))			
			pUIManager->GetUIInventory()->SetShow(false);
		//캐릭터상태창이 열려있다면 닫는다
		if(true == pUIManager->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			pUIManager->GetUICharInfo()->SetShow(false);
		//스킬트리창이 열려있다면 닫는다
		if(true == pUIManager->GetShow(CX2UIManager::UI_MENU_SKILL))
			pUIManager->GetUISkillTree()->SetShow(false);
		//커맨드스킬창이 열려있다면 닫는다
		if(true == pUIManager->GetShow(CX2UIManager::UI_MENU_COMBO_TREE))
			pUIManager->GetUIComboTree()->SetShow(false);
		//퀘스트창이 열려있다면 닫는다
		if(true == pUIManager->GetShow(CX2UIManager::UI_MENU_QUEST_NEW))
			pUIManager->GetUIQuestNew()->SetShow(false);
		//펫리스트창이 열려있다면 닫는다
		if(true == pUIManager->GetShow(CX2UIManager::UI_MENU_PET_LIST))
			pUIManager->GetUIPetList()->SetShow(false);
		//커뮤니티창이 열려있다면 닫는다, 길드창인 경우는 따로 조건을 주어야 함
		if(	true == pUIManager->GetShow(CX2UIManager::UI_MENU_COMMUNITY) 
			|| true == pUIManager->GetShow(CX2UIManager::UI_MENU_GUILD)
#ifdef	ADDED_RELATIONSHIP_SYSTEM
			|| true == pUIManager->GetShow( CX2UIManager::UI_MENU_RELATION )
#endif	// ADDED_RELATIONSHIP_SYSTEM
			)
			g_pData->GetMessenger()->SetOpen(false);

		//옵션창이 올려있다면 닫는다.
		switch( g_pMain->GetNowStateID())
		{
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_BATTLE_FIELD:
			{
				CX2StateOption* pStateOption = static_cast<CX2StateOption*>( g_pMain->GetNowState() );
				if( NULL != pStateOption )
				{
					if( true == pStateOption->GetOptionWindowOpen() )
					{
						pStateOption->CloseOptionWindow();
					}
				}
			}
			break;
		}
	}

}
#endif //DUNGEON_SELECT_ALL_CLOSE

#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
// 김석근 [12.08.27] 엘의 여인의 눈물
void CX2LocalMapUI::CreateLocalTearOfELWoman( CX2LocationManager::LOCAL_MAP_ID eLocalMapID, int dungeonID )
{

	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) dungeonID );
	if( NULL == pDungeonData )
		return;

	CKTDGUIDialogType pDialog = GetLocalMapDLG( eLocalMapID );
	if( NULL == pDialog )
		return; 

	CKTDGUIStatic* pStatic = new CKTDGUIStatic();
	pDialog->AddControl( pStatic );
	CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
	pStatic->AddPicture( pPicture );
	pPicture->SetTex( L"DLG_UI_Event_Tear_Of_ELWoman.tga",  L"DROP_B");
	pPicture->SetPoint();
	pPicture->SetFlicker( 1.0f, 1.0f, 0.1f );

	wstring LocalTearName = pDungeonData->m_DungeonName;
	LocalTearName += L"_TearOfELWoman";
	pStatic->SetName( LocalTearName.c_str() );
	wstring controlName = pDungeonData->m_DungeonName;
	
	CKTDGUIStatic* pStaticStar = (CKTDGUIStatic*) pDialog->GetControl( controlName.c_str() );
	if( NULL == pStaticStar )
		return;

	D3DXVECTOR2 LocalTearPos = pStaticStar->GetPicture(0)->GetPos();
	LocalTearPos.x += 10.f;	 
	LocalTearPos.y -= 50.f;	 
	pStatic->SetOffsetPos( LocalTearPos );
	pStatic->ScaleControl( 1.0f, 1.0f );
	pStatic->SetShow(false);
}

void CX2LocalMapUI::UpdateLocalTearOfELWoman( CX2LocationManager::LOCAL_MAP_ID eLocalMapID, int dungeonID, bool bShow )
{
#ifdef SERV_NEW_DEFENCE_DUNGEON 
	if( NULL != g_pData->GetDungeonManager() &&
		true == g_pData->GetDungeonManager()->IsDefenceDungeon( dungeonID ) )
		return;
#endif //SERV_NEW_DEFENCE_DUNGEON

	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) dungeonID );
	if( NULL == pDungeonData )
		return;

#ifdef SERV_NEW_EVENT_TYPES
	if( g_pData->GetDungeonManager()->IsDungeonEnable( dungeonID ) == false )
		return;
#endif SERV_NEW_EVENT_TYPES

	CKTDGUIDialogType pDialog = GetLocalMapDLG( eLocalMapID );
	if( NULL == pDialog )
		return;

	wstring tearOfWomanName = pDungeonData->m_DungeonName;
	tearOfWomanName += L"_TearOfELWoman";

	CKTDGUIStatic* pStaticTearOfELWoman = (CKTDGUIStatic*) pDialog->GetControl( tearOfWomanName.c_str() );
	if( NULL == pStaticTearOfELWoman )
	{
		ASSERT( !L"pStaticTearOfELWoman is NULL" );
		return;
	}

	pStaticTearOfELWoman->SetShow(false);

	KProtectedType<int> checkCurrLevel = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_Level;


	CX2Dungeon::DUNGEON_TYPE eDungeonType = static_cast<CX2Dungeon::DUNGEON_TYPE>( g_pData->GetDungeonManager()->GetDungeonType( pDungeonData->m_DungeonID ) );

	bool dungeonTearCheck = false;
	bool m_bCanHaveTear = true;

	switch ( eDungeonType )
	{
	case CX2Dungeon::DT_SECRET:
	case CX2Dungeon::DT_NORMAL: // 헤니르, 비던을 제외한 던전
		switch( pDungeonData->m_DungeonID )
		{
		case SEnum::DUNGEON_ID::DI_RUBEN_EL_TREE_NORMAL:
		case SEnum::DUNGEON_ID::DI_RUBEN_RUIN_OF_ELF_NORMAL:
		case SEnum::DUNGEON_ID::DI_RUBEN_RUIN_OF_ELF_HARD:
		case SEnum::DUNGEON_ID::DI_RUBEN_RUIN_OF_ELF_EXPERT:
		case SEnum::DUNGEON_ID::DI_RUBEN_SWAMP_NORMAL:
		case SEnum::DUNGEON_ID::DI_RUBEN_SWAMP_HARD:
		case SEnum::DUNGEON_ID::DI_RUBEN_SWAMP_EXPERT:
		case SEnum::DUNGEON_ID::DI_ELDER_HENIR_SPACE:
			m_bCanHaveTear = false;
			break;
		}

		if(m_bCanHaveTear)
		{
			for(int d = 0; d < 3; d++)
			{
				if(g_pData == NULL 
					|| g_pData->GetDungeonManager() == NULL
					|| g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)(dungeonID + d)) == NULL)
					return;

				int iDungeonMinLevel = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)(dungeonID + d))->m_MinLevel;
				int iDungeonMaxLevel = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)(dungeonID + d))->m_MaxLevel;

				if( (iDungeonMinLevel - 2) <= checkCurrLevel && checkCurrLevel <= (iDungeonMaxLevel + 2)) 
				{	
					IF_EVENT_ENABLED(CEI_TEAR_OF_ELWOMAN)
					{
						dungeonTearCheck = true;
					}
					ELSE
					{
						dungeonTearCheck = false;
					}	
					
				}

				if( eDungeonType == CX2Dungeon::DT_SECRET)
				{
					break;
				}
			}
			pStaticTearOfELWoman->SetShow(dungeonTearCheck);
			break;
		}
	default:
		break;
	}

}
#endif SERV_EVENT_TEAR_OF_ELWOMAN

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
void CX2LocalMapUI::GetEventValentineDungeonIconDesc( OUT wstringstream& wstrStreamDungeonIconDesc )
{
	int iHenirRewardCount = 3 - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().GetValentineItemCount();
		
	wstrStreamDungeonIconDesc << GET_STRING( STR_ID_30263 ) << '\n' << '\n'; // // 헤니르의 시공에 입장합니다.

	if(iHenirRewardCount == 0 )
	{
		wstrStreamDungeonIconDesc << GET_STRING( STR_ID_30264 ) << '\n'; // 오늘은 헤니르의 시공에서 더 이상 \n보상을 획득할 수 없습니다.\n (던전 입장에는 제한이 없습니다.)
	}
	else
	{
		wstrStreamDungeonIconDesc << GET_STRING( STR_ID_13761 ) << '\n'; // 오늘 보상을 받을 수 있는 횟수는
		wstrStreamDungeonIconDesc << GET_REPLACED_STRING( ( STR_ID_13763, "i", iHenirRewardCount ) ); // #CFF0000@1#CX번 
		wstrStreamDungeonIconDesc << '\n' << GET_STRING( STR_ID_13762 ) << '\n';// 남았습니다.\n (던전 입장에는 제한이 없습니다.)
	}
}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT