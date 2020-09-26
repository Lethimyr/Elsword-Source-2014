#include "StdAfx.h"
#include ".\x2statearcaderesult.h"

CX2StateArcadeResult::CX2StateArcadeResult(void)
{
	m_DungeonResultAniOrder = DRAO_NONE;
	m_ArcadeResultAniOrder = ARAO_RESULT_BG;
	
	if ( m_pDLGBack != NULL )
		m_pDLGBack->SetShowEnable( false, false );

	if ( m_pDLGFront != NULL )
		m_pDLGFront->SetShowEnable( false, false );

	m_pDLGArcadeBack = NULL;

	m_fTimeShow = 0.0f;
	m_hParticleAracdeRank = INVALID_PARTICLE_HANDLE;

	m_pPicCharTodayRank = NULL;
	m_pPicCharNowRank = NULL;
	m_pPicCharTodayRankTime = NULL;

	m_hParticleMyNewRecord = INVALID_PARTICLE_HANDLE;

	InitArcadeUI();
}

CX2StateArcadeResult::~CX2StateArcadeResult(void)
{
	SAFE_DELETE_DIALOG( m_pDLGArcadeBack );

	if ( m_pPicCharTodayRank != NULL )
	{
		m_pPicCharTodayRank->Clear();
		SAFE_DELETE( m_pPicCharTodayRank );
	}

	if ( m_pPicCharNowRank != NULL )
	{
		m_pPicCharNowRank->Clear();
		SAFE_DELETE( m_pPicCharNowRank );
	}

	if ( m_pPicCharTodayRankTime != NULL )
	{
		m_pPicCharTodayRankTime->Clear();
		SAFE_DELETE( m_pPicCharTodayRankTime );
	}
	

	if ( m_hParticleAracdeRank != INVALID_PARTICLE_HANDLE )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleAracdeRank );
	}
}

HRESULT CX2StateArcadeResult::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bInitUIFail == true )
		return S_OK;

	CX2StateCommonBG::OnFrameMove( fTime, fElapsedTime );
	g_pKTDXApp->GetDGManager()->GetCamera()->UpdateCamera( fElapsedTime );

	if ( m_bStartTimer == true )
		m_fRemainTime -= fElapsedTime;

	if ( m_fRemainTime <= 0.0f && m_bSendRefreshRoomReq == false )
	{
		m_bSendRefreshRoomReq = true;

		if ( g_pX2Room->GetBan() == true )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( 550 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_ARCADE_ROOM, NULL, false );	
		}
		else
		{
			//Handler_EGS_REFRESH_ROOM_REQ();
			Handler_EGS_RESULT_SUCCESS_REQ();
		}
	}

	PicCharFrameMove( fTime, fElapsedTime );

	CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetMySlot();
	if( NULL != pSlotData && pSlotData->m_pUnit != NULL && NULL != pSlotData->m_pUnitViewer )
	{
		UnitViewerProcess( pSlotData->m_pUnitViewer );
	}

	if ( m_bUIAnimStart == false )
	{
		m_fStartWaitRemainTime =- fElapsedTime;
	}

	if ( m_fStartWaitRemainTime <= 0.0f )
	{
		m_bUIAnimStart = true;
	}


	if ( m_bUIAnimStart == true )
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGArcadeBack ) == false )
		{
			return S_OK;
		}

		const int X2_MAGIC_MULTY_NUM = 3500;
		const int X2_MAGIC_MULTY_NUM2 = 35;
		const int X2_MAGIC_MULTY_NUM3 = 70000;
		const int X2_MAGIC_MULTY_NUM4 = 350;


		CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = NULL;
		for ( int i = 0; i < (int)g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList.size(); i++ )
		{
			pDungeonResultUnitInfo = g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList[i];
			if ( pDungeonResultUnitInfo == NULL )
				continue;

			if ( g_pData->GetDungeonRoom()->GetMySlot() != NULL && pDungeonResultUnitInfo->m_UnitUID == g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit->GetUID() )
			{
				break;
			}
		}

		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(m_ArcadeResultAniOrder)
		{
		case ARAO_RESULT_BG:
			{
				m_ArcadeResultAniOrder = ARAO_MY_RECORD;
				m_fTimeShow = 1.5f;

				if ( m_hParticleMyNewRecord == INVALID_PARTICLE_HANDLE )
				{

					if ( g_pData->GetDungeonResultInfo() != NULL &&
						g_pData->GetDungeonResultInfo()->m_kMyTopRankingInfo.m_iStageCount == g_pData->GetDungeonResultInfo()->m_kResultRankingInfo.m_iStageCount &&
						g_pData->GetDungeonResultInfo()->m_kMyTopRankingInfo.m_ulPlayTime == g_pData->GetDungeonResultInfo()->m_kResultRankingInfo.m_ulPlayTime )
					{
						m_hParticleMyNewRecord = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"NewMyTopArcadeRank",
							754,
							174,
							-1200,
							50, 50, 1, -1, 99999.0f );
					}
				}
			} break;
		case ARAO_MY_RECORD:
			{
				m_fTimeShow -= fElapsedTime;
				if ( m_fTimeShow <= 0 )
				{
					//원래는 2초인데
					//지금은 도장박는 부분이 없어서 당분간 4초로.
					m_fTimeShow = 2.0f;
					m_ArcadeResultAniOrder = ARAO_RANK;
				}
			} break;
		case ARAO_RANK:
			{
				//show ranker name at this time.
				if ( g_pData->GetDungeonResultInfo() != NULL )
				{
					CKTDGUIStatic* pStatic_Ranking_Server_1st = (CKTDGUIStatic*)m_pDLGArcadeBack->GetControl(L"Ranking_Server_1st");
					if ( pStatic_Ranking_Server_1st != NULL )
					{
						int topRankerNum = (int)g_pData->GetDungeonResultInfo()->m_kTopRankingInfo.m_vecRankerList.size();
						if ( topRankerNum > 0 )
						{
							wstring hostName = g_pData->GetDungeonResultInfo()->m_kTopRankingInfo.m_vecRankerList[0].m_wstrNickName;
							if ( topRankerNum > 1 )
							{
								wstringstream wstrstm;
								wstrstm << L" (+" << topRankerNum - 1 << L")";
								hostName += wstrstm.str().c_str();
							}
							pStatic_Ranking_Server_1st->SetString( 0, hostName.c_str() );
						}
					}

					CKTDGUIStatic* pStatic_Ranking_Today = (CKTDGUIStatic*)m_pDLGArcadeBack->GetControl( L"Ranking_Today" );
					if ( pStatic_Ranking_Today != NULL )
					{
						for ( int i = 0; i < (int)g_pData->GetDungeonResultInfo()->m_vecTodayRankingInfo.size(); i++ )
						{
							KArcadeRankingInfo& rankingInfo = g_pData->GetDungeonResultInfo()->m_vecTodayRankingInfo[i];

							int topRankerNum = (int)rankingInfo.m_vecRankerList.size();
							if ( topRankerNum > 0 )
							{
								wstring hostName = rankingInfo.m_vecRankerList[0].m_wstrNickName;
								if ( topRankerNum > 1 )
								{
									wstringstream wstrstm;
									wstrstm << L" (+" << topRankerNum - 1  << L")";
									hostName += wstrstm.str().c_str();
								}
								pStatic_Ranking_Today->SetString( i, hostName.c_str() );
							}
						}


					}


				}


				m_fTimeShow -= fElapsedTime;
				if ( m_fTimeShow <= 0 )
				{
					m_fTimeShow = 6.0;
					m_ArcadeResultAniOrder = ARAO_RANK_CHECK;

					if ( m_hParticleMyNewRecord != INVALID_PARTICLE_HANDLE )	
					{
						g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleMyNewRecord );
					}

				}
			} break;
		case ARAO_RANK_CHECK:
			{
				m_fTimeShow -= fElapsedTime;
				if ( m_fTimeShow <= 0 )
				{
					m_fTimeShow = 0;
					//we have no stamp then go to next page!
					//m_ArcadeResultAniOrder = ARAO_RANK_CHECK;
					m_ArcadeResultAniOrder = ARAO_END;
					m_pDLGArcadeBack->SetShowEnable( false, false );
					m_pDLGBack->SetShowEnable( true, true );
					m_pDLGFront->SetShowEnable( true, true );
					m_DungeonResultAniOrder = DRAO_ED_BG;
				}

				if ( m_hParticleAracdeRank == INVALID_PARTICLE_HANDLE )
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"ResultStamp.ogg", false, false );

					//{{ 2009.01.19 김태완 : 코드정리 ifelse -> switch
					switch(g_pData->GetDungeonResultInfo()->m_iRank)
					{
					case 0:
					case 1:
						{
							m_hParticleAracdeRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Arcade_Rank_1", 0,0,0, 1000, 1000, 1, 1 );
						} break;
					case 2:
						{
							m_hParticleAracdeRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Arcade_Rank_2", 0,0,0, 1000, 1000, 1, 1 );
						} break;
					case 3:
						{	
							m_hParticleAracdeRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Arcade_Rank_3", 0,0,0, 1000, 1000, 1, 1 );
						} break;
					case 4:
						{
							m_hParticleAracdeRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Arcade_Rank_4", 0,0,0, 1000, 1000, 1, 1 );
						} break;
					case 5:
						{
							m_hParticleAracdeRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Arcade_Rank_5", 0,0,0, 1000, 1000, 1, 1 );
						} break;
					default:
						{
							m_hParticleAracdeRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Arcade_Rank_No", 0,0,0, 1000, 1000, 1, 1 );
						} break;

					}//}}

				}

			} break;
		default:
			break;
		}//}}

		


		/*
		if ( m_ArcadeResultAniOrder >= ARAO_MY_RECORD && m_DungeonResultAniOrder < DRAO_ED_BG )
		{
			

		}
		*/

		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(m_DungeonResultAniOrder)
		{
		case DRAO_ED_BG:
			{
				if ( m_hParticleAracdeRank != INVALID_PARTICLE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleAracdeRank );
				}


				g_pKTDXApp->GetDGManager()->GetCamera()->Point( 0,0,-1300, 0,0,0 );
				g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
					g_pKTDXApp->GetDGManager()->GetFar(), false );

				CKTDGUIStatic* pStaticReward_Choice_Item_Black = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Reward_Choice_Item_Black" );
				if ( pStaticReward_Choice_Item_Black->GetIsMoving() == false )
				{
					if ( m_pDLGFront->GetControl( L"Result_Title" ) != NULL )
						m_pDLGFront->GetControl( L"Result_Title" )->SetShow( false );

					if ( m_pDLGFront->GetControl( L"Reward_Title" ) != NULL )
						m_pDLGFront->GetControl( L"Reward_Title" )->SetShow( true );



					pSlotData->m_pUnitViewer->SetShowObject( true );

					if ( g_pData->GetMyUser()->GetSelectUnit()->GetIsLevelUp() == true )
					{
						g_pData->GetMyUser()->GetSelectUnit()->SetIsLevelUp( false );
						pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_LEVEL_UP );

						CKTDGXMeshPlayer::CXMeshInstance* pInst = NULL;
						pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp01_1", 
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().x,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().y - 25.0f,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().z, 
							0,0,0, 0,0,0 );
						pInst->SetDelayTime( 0.3f );
						pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp01",
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().x,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().y - 25.0f,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().z, 
							0,0,0, 0,0,0 );
						pInst->SetDelayTime( 0.3f );
						pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp04",
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().x,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().z, 
							0,0,0, 0,0,0 );
						pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp02", 
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().x + 5.0f,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().z + 30.0f, 
							0,0,0, 0,0,0 );
						pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp02", 
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().x + 5.0f,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().z + 30.0f, 
							0,0,0, 0,0,0 );
						pInst->SetDelayTime( 0.1f );
						pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp02",
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().x + 5.0f,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().z + 30.0f, 
							0,0,0, 0,0,0 );
						pInst->SetDelayTime( 0.2f );
						pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp02",
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().x + 5.0f,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
							pSlotData->m_pUnitViewer->GetMatrix().GetPos().z + 30.0f, 
							0,0,0, 0,0,0 );
						pInst->SetDelayTime( 0.3f );

						g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUp.ogg", false, false );

						m_fPlayBGM = -1;

						//if( g_pKTDXApp->GetDSManager()->GetInit() == true && g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL )
						//	g_pKTDXApp->GetDSManager()->GetMP3Play()->Play( L"LevelUpBGM.ogg" );

						g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUpBGM.ogg", false, false );

						g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LevelUp", 233, 458, 0, 1000, 1000, 1, 1, 1 );
						CKTDGParticleSystem::CParticleEventSequence* pPartInst = NULL;
						pPartInst = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LobbyLevelUp01", pSlotData->m_pUnitViewer->GetMatrix().GetPos() );
						pPartInst->SetLatency( 2.266f );
					}
					else
					{
						pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
					}


					CKTDGUIStatic* pStaticEDBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"StaticReward_ED" );
					pStaticEDBG->SetShow( true );
					pStaticEDBG->SetColor( D3DXCOLOR(1,1,1,0) );
					pStaticEDBG->SetOffsetPos( D3DXVECTOR2( pStaticEDBG->GetOffsetPos().x - 70, pStaticEDBG->GetOffsetPos().y ) );
					pStaticEDBG->Move( D3DXVECTOR2( pStaticEDBG->GetOffsetPos().x + 70, pStaticEDBG->GetOffsetPos().y ), D3DXCOLOR(1,1,1,1), 0.2f, true );

					m_DungeonResultAniOrder = DRAO_ITEM_BG;
				}
			} break;
		case DRAO_ITEM_BG:
			{
				CKTDGUIStatic* pStaticReward_Choice_Item_Black = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"StaticReward_ED" );
				if ( pStaticReward_Choice_Item_Black->GetIsMoving() == false )
				{


					CKTDGUIStatic* pStaticItemBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Item_BG" );
					pStaticItemBG->SetShow( true );
					pStaticItemBG->SetColor( D3DXCOLOR(1,1,1,0) );
					pStaticItemBG->SetOffsetPos( D3DXVECTOR2( pStaticItemBG->GetOffsetPos().x - 70, pStaticItemBG->GetOffsetPos().y ) );
					pStaticItemBG->Move( D3DXVECTOR2( pStaticItemBG->GetOffsetPos().x + 70, pStaticItemBG->GetOffsetPos().y ), D3DXCOLOR(1,1,1,1), 0.2f, true );

					int iCount = 7;
					for( UINT i=0; i<g_pMain->GetDungeonRewardItem().size(); i++ )
					{
						if( iCount > (7 + 25 ) )
							break;

						const KItemInfo& itemInfo = g_pMain->GetDungeonRewardItem().at(i);

						wstring wTexName = g_pData->GetItemManager()->GetItemTemplet( itemInfo.m_iItemID )->m_ShopImage.c_str();
						if( g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->IsValidFile( wTexName.c_str() ) == false )
							wTexName = L"HQ_Shop_Ui_Noimage.dds";

						string texName;
						ConvertWCHARToChar( texName, wTexName.c_str() );


						pStaticItemBG->GetPicture(iCount)->SetTex_LUA2( texName.c_str() );

						if ( itemInfo.m_cUsageType == CX2Item::PT_QUANTITY )
						{
							CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
							pStaticItemBG->AddString( pString );
							pString->fontIndex = XUF_DODUM_15_BOLD;
							pString->color = D3DXCOLOR( 0,0,0,1 );
							pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
							WCHAR buff[256] = {0};
							StringCchPrintf( buff, 256, L"%d", itemInfo.m_iQuantity );
							//wsprintf( buff, L"%d", itemInfo.m_iQuantity );
							pString->msg = buff;
							pString->fontStyle = CKTDGFontManager::FS_NONE;
							pString->sortFlag = DT_RIGHT;
							pString->pos = D3DXVECTOR2( pStaticItemBG->GetPicture(iCount)->GetPos().x +  pStaticItemBG->GetPicture(iCount)->GetSize().x - 20,
								pStaticItemBG->GetPicture(iCount)->GetPos().y +  pStaticItemBG->GetPicture(iCount)->GetSize().y - 20  );
						}

						iCount++;
					}						

					m_DungeonResultAniOrder = DRAO_EXP_BG;
				}
			} break;
		case DRAO_EXP_BG:
			{
				if ( m_pDLGBack->GetControl( L"Dungeon_Result_Item_BG" )->GetIsMoving() == false )
				{
					CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
					if ( pStaticEXPBG != NULL )
					{
						pStaticEXPBG->SetShow( true );
						pStaticEXPBG->SetColor( D3DXCOLOR(1,1,1,0) );
						pStaticEXPBG->SetOffsetPos( D3DXVECTOR2( pStaticEXPBG->GetOffsetPos().x - 70, pStaticEXPBG->GetOffsetPos().y ) );
						pStaticEXPBG->Move( D3DXVECTOR2( pStaticEXPBG->GetOffsetPos().x + 70, pStaticEXPBG->GetOffsetPos().y ), D3DXCOLOR(1,1,1,1), 0.2f, true );

						//EXP setting

						D3DXVECTOR2 picOrgSize = pStaticEXPBG->GetPicture( (pStaticEXPBG->GetPictureNum() - 1) )->GetOriginalSize();
						CKTDGUIControl::CPictureData* pPictureEXP = pStaticEXPBG->GetPicture( (pStaticEXPBG->GetPictureNum() - 1) );

						int nowExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData()->m_EXP;
						int nowBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData()->m_NowBaseLevelEXP;
						int nextBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData()->m_NextBaseLevelEXP;

						pPictureEXP->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );

						if ( pStaticEXPBG->GetString(0) != NULL )
						{
							WCHAR buff[256] = {0};
							swprintf( buff, L"%d / %d (%.1f%%)", nowExp - nowBaseExp, nextBaseExp - nowBaseExp, (float)(nowExp - nowBaseExp)/(nextBaseExp - nowBaseExp)*100.0f );
							pStaticEXPBG->GetString(0)->msg = buff;
						}
						/*
						int nowExp = pSlotData->m_pUnit->GetPrevEXP();
						int nowBaseExp = pSlotData->m_pUnit->GetPrevNowBaseLevelEXP();
						int nextBaseExp = pSlotData->m_pUnit->GetPrevNextBaseLevelEXP();

						m_fEXPSize = (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x;

						pStaticEXPBG->GetPicture(27)->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );
						pStaticEXPBG->GetPicture(28)->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );

						pStaticEXPBG->GetPicture(27)->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );
						*/


						m_DungeonResultAniOrder = DRAO_ED;

						//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", true );
					}
				}
			} break;
		case DRAO_ED:
			{
				CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
				if ( pStaticEXPBG->GetIsMoving() == false )
				{
					m_fED += (fElapsedTime * X2_MAGIC_MULTY_NUM);

					//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg" );

					if ( m_fED >= (float)pDungeonResultUnitInfo->m_nED )
					{
						m_fED = (float)pDungeonResultUnitInfo->m_nED;

						//g_pKTDXApp->GetDeviceManager()->StopSound( L"Count.ogg" );

						m_DungeonResultAniOrder = DRAO_ITEM;

					}
					/*
					WCHAR edBuff[256] = {0};
					wsprintf( edBuff, L"%d", (int)m_fED );
					CKTDGUIStatic* pStaticED = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"StaticReward_ED" );
					pStaticED->GetString(0)->msg = edBuff;
					*/
				}
			} break;
		case DRAO_ITEM:
			{
				//////////////////////////////////////////////////////////////////////////
				// At Next EXP Page
				int playerNum = 0;
				for ( int i = 0; i < g_pX2Room->GetSlotNum(); i++ )
				{
					CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData(i);
					if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
					{
						playerNum++;
					}
				}

				//WCHAR partyPayoutBuff[256] = {0};

				//if ( playerNum == 2 )
				//{
				//wsprintf( partyPayoutBuff, L"파티 분배율 : 50%%" );
				//}
				//else if ( playerNum == 3 )
				//{
				//wsprintf( partyPayoutBuff, L"파티 분배율 : 33%%" );
				//}
				//else if ( playerNum == 4 )
				//{
				//wsprintf( partyPayoutBuff, L"파티 분배율 : 25%%" );
				//}

				//CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
				//pStaticEXPBG->GetString(0)->msg = partyPayoutBuff;

				m_DungeonResultAniOrder = DRAO_BONUS_EXP;

				//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", true );
				//////////////////////////////////////////////////////////////////////////

			} break;
		case DRAO_BONUS_EXP:
			{


				if ( m_bCheckBonusEXP == false )
				{
					m_bCheckBonusEXP = true;

					// here stamp effect
					CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
					if ( pStaticEXPBG != NULL )
					{
						CX2StateDungeonResult::BonusEXPData bonusExpData;
						if ( pDungeonResultUnitInfo->m_nClearBonusEXP > 0 )
						{
							bonusExpData.bonusExpType = CX2StateDungeonResult::BET_CLEAR;
							bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nClearBonusEXP;
							m_vecBonusExpType.push_back( bonusExpData );

						}
						if ( pDungeonResultUnitInfo->m_nRankBonusEXP > 0 )
						{
							bonusExpData.bonusExpType = CX2StateDungeonResult::BET_RANK;
							bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nRankBonusEXP;
							m_vecBonusExpType.push_back( bonusExpData );

						}
						if ( pDungeonResultUnitInfo->m_nPartyBonusEXP > 0 )
						{

							bonusExpData.bonusExpType = CX2StateDungeonResult::BET_PARTY;
							bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nPartyBonusEXP;
							m_vecBonusExpType.push_back( bonusExpData );


						}
						if ( pDungeonResultUnitInfo->m_nMvpBonusEXP > 0 )
						{

							bonusExpData.bonusExpType = CX2StateDungeonResult::BET_MVP;
							bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nMvpBonusEXP;
							m_vecBonusExpType.push_back( bonusExpData );

						}
						if ( pDungeonResultUnitInfo->m_nGameBangBonusEXP > 0 )
						{

							bonusExpData.bonusExpType = CX2StateDungeonResult::BET_GAMEBANG;
							bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nGameBangBonusEXP;
							m_vecBonusExpType.push_back( bonusExpData );

						}
						if ( pDungeonResultUnitInfo->m_nPremiumBonusEXP > 0 )
						{

							bonusExpData.bonusExpType = CX2StateDungeonResult::BET_PREMIUM;
							bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nPremiumBonusEXP;
							m_vecBonusExpType.push_back( bonusExpData );

						}
						if ( pDungeonResultUnitInfo->m_nEventBonusExp > 0 )
						{

							bonusExpData.bonusExpType = CX2StateDungeonResult::BET_EVENT;
							bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nEventBonusExp;
							m_vecBonusExpType.push_back( bonusExpData );

						}

						/*
						if ( g_pData->GetMyUser()->GetIsPCRoom() == true )
						{
						if ( pStaticEXPBG->GetPicture( 23) != NULL )
						pStaticEXPBG->GetPicture( 23 )->SetShow( true );
						}
						else
						{
						if ( pStaticEXPBG->GetPicture( 23) != NULL )
						pStaticEXPBG->GetPicture( 23 )->SetShow( false );
						}
						*/
					}
				}

				//m_DungeonResultAniOrder = DRAO_BONUS_EXP;

				m_fTimeBonusEXPShowEvent += fElapsedTime;

				if ( m_fTimeBonusEXPShowEvent > 0.4f )
				{
					m_fTimeBonusEXPShowEvent = 0.0f;

					if ( m_vecBonusExpType.empty() == true )
						m_DungeonResultAniOrder = DRAO_EXP;
					else
					{
						CX2StateDungeonResult::BonusEXPData bonuseEXPData = m_vecBonusExpType[0];
						ShowBonusEXPEvent( bonuseEXPData.bonusExpType, bonuseEXPData.bonusExp );
						m_vecBonusExpType.erase( m_vecBonusExpType.begin() );
					}
				}
			} break;
		case DRAO_EXP:
			{
				m_fEXP += (fElapsedTime * X2_MAGIC_MULTY_NUM);

				if ( m_fEXP < (float)( pDungeonResultUnitInfo->GetTotalExp() ) ) 
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", false, false );

				if ( m_fEXP >= (float)( pDungeonResultUnitInfo->GetTotalExp() ) )
				{
					m_fEXP = (float)( pDungeonResultUnitInfo->GetTotalExp() );

					m_DungeonResultAniOrder = DRAO_EXP_BAR;


				}	
			} break;
		case DRAO_EXP_BAR:
			{
				/*
				//EXP setting
				CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
				D3DXVECTOR2 picOrgSize = pStaticEXPBG->GetPicture(27)->GetOriginalSize();

				int nowExp = pSlotData->m_pUnit->GetUnitData()->m_EXP;
				int nowBaseExp = pSlotData->m_pUnit->GetUnitData()->m_NowBaseLevelEXP;
				int nextBaseExp = pSlotData->m_pUnit->GetUnitData()->m_NextBaseLevelEXP;



				m_fEXPSize += (fElapsedTime*X2_MAGIC_MULTY_NUM4);

				float maxSize = 0;
				if ( pSlotData->m_pUnit->GetPrevLevel() == pSlotData->m_pUnit->GetUnitData()->m_Level )
				{
				maxSize = (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x;
				}
				else
				{
				maxSize = picOrgSize.x;
				}



				if ( m_fEXPSize >= maxSize )
				{
				m_fEXPSize = maxSize;

				if ( pSlotData->m_pUnit->GetPrevLevel() < pSlotData->m_pUnit->GetUnitData()->m_Level )
				{
				m_fEXPSize = 0;
				//pStaticEXPBG->GetPicture(28)->SetSizeX( 0 );
				pSlotData->m_pUnit->SetPrevLevel( pSlotData->m_pUnit->GetPrevLevel() + 1 );
				}
				else
				{
				m_DungeonResultAniOrder = DRAO_END;
				g_pKTDXApp->GetDeviceManager()->StopSound( L"EXPUp.ogg" );
				}
				}

				pStaticEXPBG->GetPicture(27)->SetSizeX( m_fEXPSize );
				*/

				pSlotData->m_pUnit->SetPrevLevel( pSlotData->m_pUnit->GetUnitData()->m_Level);

				m_DungeonResultAniOrder = DRAO_END;
			} break;
		case DRAO_END:
			{
				if( NULL != pSlotData &&
					NULL != pSlotData->m_pUnitViewer && 
					NULL != pSlotData->m_pUnitViewer->GetXSkinAnim() )
				{
					if ( pSlotData->m_pUnitViewer->GetXSkinAnim()->GetPlayType() == CKTDGXSkinAnim::XAP_ONE_WAIT )
					{
						if ( pSlotData->m_pUnitViewer->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAS_WAIT )
						{
							m_bStartTimer = true;
						}
					}
					else
					{
						m_bStartTimer = true;
					}
				}
			} break;
		default:
			break;

		}//}}


		PicCharDrawText();	
		PicCharDrawTextForArcade();

	}


	return S_OK;
}


bool CX2StateArcadeResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DialogLog( "CX2StateArcadeResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT Start" );

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	if ( g_pMain->GetIsPlayingTutorial() == false )
	{
		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_550 ) );
	}

	g_pX2Room->Set_KRoomInfo( kEvent.m_RoomInfo );
	g_pX2Room->Set_KRoomSlotInfoList( kEvent.m_vecSlot, true );

	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		g_pMain->SetIsPlayingTutorial( false );		

		SAFE_DELETE_DIALOG( m_pDLGMsgBox );
		//m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), L"단계별 훈련으로 가시겠습니까?", 
		//					DRUCM_GO_TRAINING_SCHOOL_OK, this, DRUCM_GO_TRAINING_SCHOOL_CANCEL );

		//m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), L"초보던전 '초심자의 숲'으로 가시겠습니까?", 
		//					DRUCM_GO_TRAINING_SCHOOL_OK, this, DRUCM_GO_TRAINING_SCHOOL_CANCEL );


		//CX2StateAutoChanger::TARGET_DETAIL targetDetail;
		//g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_VILLAGE_MAP, targetDetail, -1 );

		g_pMain->SetIsExitingTutorial( true );
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_ARCADE_ROOM, NULL, false );
	}
	else
	{
		
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_ARCADE_ROOM, NULL, false );		

	}


	return true;
}

void CX2StateArcadeResult::PicCharFrameMove( double fTime, float fElapsedTime )
{
	CX2StateDungeonResult::PicCharFrameMove( fTime, fElapsedTime );

	if ( m_pPicCharTodayRank != NULL )
		m_pPicCharTodayRank->OnFrameMove( fElapsedTime );

	if ( m_pPicCharNowRank != NULL )
		m_pPicCharNowRank->OnFrameMove( fElapsedTime );

	if ( m_pPicCharTodayRankTime != NULL )
		m_pPicCharTodayRankTime->OnFrameMove( fElapsedTime );
	
}

HRESULT CX2StateArcadeResult::InitArcadeUI()
{
	m_pDLGArcadeBack = new CKTDGUIDialog( this, L"DLG_Dungeon_Arcade_Result_Rank.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGArcadeBack );

	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"TodayRankPickChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharTodayRank = new CKTDGPicChar( m_pPicCharScore, pSeq );
	m_pPicCharTodayRank->SetWidth( 15.0f );

	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"TodayRankPickCharTime", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharTodayRankTime = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
	m_pPicCharTodayRankTime->SetWidth( 15.0f );

	

	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"NowRankPickChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharNowRank = new CKTDGPicChar( m_pPicCharScore, pSeq );
	m_pPicCharNowRank->SetWidth( 18.0f + 10 );




	

	CKTDGUIStatic* pStaticBG = (CKTDGUIStatic*)m_pDLGArcadeBack->GetControl(L"Arcade_Ranking_Bg");
	if ( pStaticBG != NULL )
	{
		wstring textureName;
		wstring textureKeyName;
		wstring textureName2;
		wstring textureKeyName2;

		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType)
		{
		case CX2Unit::UT_ELSWORD:
			{
				textureName		= L"DLG_Dungeon_Arcade_Rank_Elsword01.TGA";
				textureKeyName	= L"Arcade_Ranking_Elsword_Bg01";
				textureName2	= L"DLG_Dungeon_Arcade_Rank_Elsword02.TGA";
				textureKeyName2	= L"Arcade_Ranking_Elsword_Bg02"; 
			} break;
		case CX2Unit::UT_ARME:
			{
				textureName		= L"DLG_Dungeon_Arcade_Rank_Aisha01.TGA";
				textureKeyName	= L"Arcade_Ranking_Aisha_Bg01";
				textureName2	= L"DLG_Dungeon_Arcade_Rank_Aisha02.TGA";
				textureKeyName2	= L"Arcade_Ranking_Aisha_Bg02"; 
			} break;
		case CX2Unit::UT_LIRE:
			{
				textureName		= L"DLG_Dungeon_Arcade_Rank_Rena01.TGA";
				textureKeyName	= L"Arcade_Ranking_Rena_Bg01";
				textureName2	= L"DLG_Dungeon_Arcade_Rank_Rena02.TGA";
				textureKeyName2	= L"Arcade_Ranking_Rena_Bg02"; 
			} break;
		case CX2Unit::UT_RAVEN:
			{
				textureName		= L"DLG_Dungeon_Arcade_Rank_Raven01.TGA";
				textureKeyName	= L"Arcade_Ranking_Raven_Bg01";
				textureName2	= L"DLG_Dungeon_Arcade_Rank_Raven02.TGA";
				textureKeyName2	= L"Arcade_Ranking_Raven_Bg02"; 
			} break;

		case CX2Unit::UT_EVE:
			{
				textureName		= L"DLG_Dungeon_Arcade_Rank_Eve01.tga";		
				textureKeyName	= L"Arcade_Ranking_Eve_Bg01";
				textureName2	= L"DLG_Dungeon_Arcade_Rank_Eve02.TGA";		
				textureKeyName2	= L"Arcade_Ranking_Eve_Bg02"; 
			} break;

		// kimhc // 2010-11-24 // 현재는 사용하지 않음으로 CHUNG 은 만들지 않음
		default:
			{
				ASSERT( !"Unexpected UnitType" );
			} break;

		}//}}
		
		if ( pStaticBG->GetPicture(0) != NULL )
		{
			pStaticBG->GetPicture(0)->SetTex( textureName.c_str(), textureKeyName.c_str() );
		}

		if ( pStaticBG->GetPicture(1) != NULL )
		{
			pStaticBG->GetPicture(1)->SetTex( textureName2.c_str(), textureKeyName2.c_str() );
		}
	}

	int otherUserNum = 0;

	if ( g_pData->GetDungeonResultInfo() != NULL )
	{
		for ( int i = 0; i < (int)g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList.size(); i++ )
		{
			CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList[i];
			if ( pDungeonResultUnitInfo == NULL )
				continue;

			if ( g_pData->GetDungeonRoom()->GetMySlot() != NULL && pDungeonResultUnitInfo->m_UnitUID != g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit->GetUID() )
			{
				CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( pDungeonResultUnitInfo->m_UnitUID );
				if ( pSlotData == NULL )
					continue;

				if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
					continue;

				CKTDGUIStatic* pStaticPartyRank = (CKTDGUIStatic*)m_pDLGArcadeBack->GetControl( L"Dungeon_Result_Party_Rank" );
				if ( pStaticPartyRank != NULL && pStaticPartyRank->GetPicture( otherUserNum ) != NULL )
				{
					if ( pSlotData->m_pUnit != NULL )
					{
						pStaticPartyRank->GetPicture( otherUserNum )->SetShow( true );



						wstring fileName;
						wstring pieceName;
						if( true == pDungeonResultUnitInfo->m_bIsDie )
						{
							if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pSlotData->m_pUnit->GetUnitTemplet()->m_UnitClass, CX2Data::CIT_160by160Gray ) )
							{
								pStaticPartyRank->GetPicture( otherUserNum )->SetTex( fileName.c_str(), pieceName.c_str() );
							}
							else
							{
								ASSERT( !"NO" );
							}
						}
						else
						{
							if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pSlotData->m_pUnit->GetUnitTemplet()->m_UnitClass, CX2Data::CIT_160by160 ) )
							{
								pStaticPartyRank->GetPicture( otherUserNum )->SetTex( fileName.c_str(), pieceName.c_str() );
							}
							else
							{
								ASSERT( !"NO" );
							}
						}





						//switch(pSlotData->m_pUnit->GetUnitTemplet()->m_UnitClass)
						//{
						//case CX2Unit::UC_ELSWORD_SWORDMAN:
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Elsword1" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Elsword2" );

						//	} break;
						//case CX2Unit::UC_ELSWORD_KNIGHT:
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Elsword_Sword" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Elsword_Sword_Gray" );
						//	} break;
						//case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:	// 11
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Elsword_Magic" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Elsword_Magic_Gray" );

						//	} break;

						//case CX2Unit::UC_ARME_VIOLET_MAGE:		// 2
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Aisha1" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Aisha2" );
						//	} break;
						//case CX2Unit::UC_ARME_HIGH_MAGICIAN:		// 14
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Aisha_High" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Aisha_High_Gray" );
						//	} break;
						//case CX2Unit::UC_ARME_DARK_MAGICIAN:		// 15
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Aisha_Dark" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Aisha_Dark_Gray" );
						//	} break;

						//case CX2Unit::UC_LIRE_ELVEN_RANGER:		// 3
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Rena1" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Rena2" );
						//	} break;
						//case CX2Unit::UC_LIRE_COMBAT_RANGER:		// 12
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Rena_Combat" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Rena_Combat_Gray" );
						//	} break;
						//case CX2Unit::UC_LIRE_SNIPING_RANGER:		// 13
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Rena_Sniper" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Rena_Sniper_Gray" );
						//	} break;

						//case CX2Unit::UC_RAVEN_FIGHTER:			// 4
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture57.tga", L"DUNGEON_RESULT_RAVEN1" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture57.tga", L"DUNGEON_RESULT_RAVEN2" );
						//	} break;
						//case CX2Unit::UC_RAVEN_SOUL_TAKER:		// 16
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture15.tga", L"Result_Raven_Sword" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture15.tga", L"Result_Raven_Sword_Gray" );
						//	} break;
						//case CX2Unit::UC_RAVEN_OVER_TAKER:		// 17
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture15.tga", L"Result_Raven_Over" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture15.tga", L"Result_Raven_Over_Gray" );
						//	} break;

						//case CX2Unit::UC_EVE_NASOD:				// 5
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Character01.tga", L"EVE_NORMAL" );	
						//		else													  
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Character01.tga", L"EVE_GRAY" );	
						//	} break;
						//case CX2Unit::UC_EVE_EXOTIC_GEAR:			// 18
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Eve_Exotic" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Eve_Exotic_Gray" );
						//	} break;

						//case CX2Unit::UC_EVE_ARCHITECTURE:		// 19
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Eve_Arch" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Eve_Arch_Gray" );
						//	} break;
						//default:
						//	{
						//		ASSERT( !"Unexpected UnitClass" );
						//	} break;

						//}//}}





						
					}

				}


				//				WCHAR staticResultUnit[256] = {0};

				WCHAR staticResultDesc[256] = {0};
				WCHAR staticResultID[256] = {0};
				WCHAR staticResultPCRoom[256] = {0};

				//				wsprintf( staticResultUnit, L"Dungeon_Result_Other%d_Unit", otherUserNum );
				StringCchPrintf( staticResultDesc, 256, L"Dungeon_Result_Other%d_Desc", otherUserNum );
				StringCchPrintf( staticResultID, 256, L"Dungeon_Result_Other%d_ID", otherUserNum );
				StringCchPrintf( staticResultPCRoom, 256, L"Dungeon_Result_Other%d_PCRoom", otherUserNum );
				/*wsprintf( staticResultDesc, L"Dungeon_Result_Other%d_Desc", otherUserNum );
				wsprintf( staticResultID, L"Dungeon_Result_Other%d_ID", otherUserNum );
				wsprintf( staticResultPCRoom, L"Dungeon_Result_Other%d_PCRoom", otherUserNum );*/



				if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
				{
					//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
					ErrorLogMsg( XEM_ERROR23, "m_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
					m_bStartTimer = true;
					return S_OK;
				}
				/*

				CKTDGUIStatic* pStaticOtherUnit = (CKTDGUIStatic*)m_pDLGArcadeBack->GetControl( staticResultUnit );

				pStaticOtherUnit->SetShow( true );

				for ( int i = 0; i < 3; i++ )
				{
				pStaticOtherUnit->GetPicture(i)->SetShow( false );
				}

				if ( pSlotData->m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_SWORDMAN )
				{
				pStaticOtherUnit->GetPicture(0)->SetShow( true );
				}
				else if ( pSlotData->m_pUnit->GetClass() == CX2Unit::UC_ARME_VIOLET_MAGE )
				{
				pStaticOtherUnit->GetPicture(1)->SetShow( true );
				}
				else if ( pSlotData->m_pUnit->GetClass() == CX2Unit::UC_LIRE_ELVEN_RANGER )
				{
				pStaticOtherUnit->GetPicture(2)->SetShow( true );
				}
				*/

				if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
				{
					//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
					ErrorLogMsg( XEM_ERROR24, "m_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
					m_bStartTimer = true;
					return S_OK;
				}

				CKTDGUIStatic* pStaticOtherDesc = (CKTDGUIStatic*)m_pDLGArcadeBack->GetControl( staticResultDesc );
				pStaticOtherDesc->SetShow( true );
				WCHAR unitDescBuff[1024] = {0};
				StringCchPrintf( unitDescBuff, 1024, L"LV.%d ", pSlotData->m_pUnit->GetUnitData()->m_Level );
				//wsprintf( unitDescBuff, L"LV.%d ", pSlotData->m_pUnit->GetUnitData()->m_Level );
				pStaticOtherDesc->GetString(0)->msg = unitDescBuff;

				CKTDGUIStatic* pStaticOtherID = (CKTDGUIStatic*)m_pDLGArcadeBack->GetControl( staticResultID );
				pStaticOtherID->SetShow( true );
				pStaticOtherID->GetString(0)->msg = pSlotData->m_pUnit->GetNickName();

				if ( pSlotData->m_pUnit->GetUnitData()->m_bIsGameBang == true )
				{
					CKTDGUIStatic* pStaticOtherPCRoom = (CKTDGUIStatic*)m_pDLGArcadeBack->GetControl( staticResultPCRoom );
					pStaticOtherPCRoom->SetShow( true );

				}


				otherUserNum++;
			}
		}
	}

	return S_OK;
}

void CX2StateArcadeResult::PicCharDrawTextForArcade()
{
	if ( m_pPicCharScore != NULL && m_pPicCharComboAndTechScore != NULL && m_pPicCharED != NULL && m_pPicCharEXP != NULL &&
		m_pPicCharTodayRank != NULL && m_pPicCharNowRank != NULL && m_pPicCharTodayRankTime != NULL )
	{
		m_pPicCharTodayRank->Clear();
		m_pPicCharNowRank->Clear();
		m_pPicCharTodayRankTime->Clear();

		if ( m_ArcadeResultAniOrder >= ARAO_MY_RECORD && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
		{
			if ( g_pData->GetDungeonResultInfo() != NULL )
			{
				m_pPicCharComboAndTechScore->DrawText( g_pData->GetDungeonResultInfo()->m_kMyTopRankingInfo.m_iStageCount, D3DXVECTOR3(594 + 17,127,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
				m_pPicCharComboAndTechScore->DrawText( GetRecordTime( g_pData->GetDungeonResultInfo()->m_kMyTopRankingInfo.m_ulPlayTime ).c_str(), 
					D3DXVECTOR3(846+ 17,127,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );

				m_pPicCharNowRank->DrawText( g_pData->GetDungeonResultInfo()->m_kResultRankingInfo.m_iStageCount, D3DXVECTOR3(594 + 17,238,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
				m_pPicCharNowRank->DrawText( GetRecordTime( g_pData->GetDungeonResultInfo()->m_kResultRankingInfo.m_ulPlayTime ).c_str(), 
					D3DXVECTOR3(846+ 17,238,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
			}			
		}

		if ( m_ArcadeResultAniOrder >= ARAO_RANK && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
		{
			

			m_pPicCharComboAndTechScore->DrawText( g_pData->GetDungeonResultInfo()->m_kTopRankingInfo.m_iStageCount, D3DXVECTOR3(768 + 17,382,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
			m_pPicCharComboAndTechScore->DrawText( GetRecordTime( g_pData->GetDungeonResultInfo()->m_kTopRankingInfo.m_ulPlayTime ).c_str(), 
				D3DXVECTOR3(902 + 17,382,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );

			

			for ( int i = 0; i < (int)g_pData->GetDungeonResultInfo()->m_vecTodayRankingInfo.size(); i++ )
			{
				KArcadeRankingInfo& rankingInfo = g_pData->GetDungeonResultInfo()->m_vecTodayRankingInfo[i];

				m_pPicCharTodayRank->DrawText( rankingInfo.m_iStageCount, D3DXVECTOR3(813 ,483.0f + (i * 43),0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
				m_pPicCharTodayRankTime->DrawText( GetRecordTimeEx( rankingInfo.m_ulPlayTime ).c_str(), 
					D3DXVECTOR3(920  ,483.0f + (i * 44),0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
			}	
		}
		/*
		if ( m_DungeonResultAniOrder >= DRAO_COMBO_SCORE && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
			m_pPicCharComboAndTechScore->DrawText( (int)m_fComboScore, D3DXVECTOR3(656+26,566,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );

		if ( m_DungeonResultAniOrder >= DRAO_TECHNICAL_SCORE && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
			m_pPicCharComboAndTechScore->DrawText( (int)m_fTechnicalScore, D3DXVECTOR3(877+30,566,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );

		if ( m_DungeonResultAniOrder >= DRAO_TIME && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
		{
			WCHAR buff[256] = {0};
			wsprintf( buff, L"%02d=%02d=%02d", (int)m_fHour, (int)m_fMinute, (int)m_fSec );
			m_pPicCharComboAndTechScore->DrawText( buff, D3DXVECTOR3(731+12,383,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );
		}

		if ( m_DungeonResultAniOrder >= DRAO_DAMAGED && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
		{
			WCHAR buff[256] = {0};
			if ( m_fDamaged == 0 )
			{
				wsprintf( buff, L"0" );
			}
			else
			{
				wsprintf( buff, L"-%d", (int)m_fDamaged );
			}
			m_pPicCharComboAndTechScore->DrawText( buff, D3DXVECTOR3(945,383,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );
		}

		if ( m_DungeonResultAniOrder >= DRAO_TOTAL_SCORE && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
			m_pPicCharScore->DrawText( (int)m_fTotalScore, D3DXVECTOR3(865+47,653,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );

		if ( m_DungeonResultAniOrder >= DRAO_EXP  )
			m_pPicCharEXP->DrawText( (int)m_fEXP, D3DXVECTOR3(930,649,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );

		if ( m_DungeonResultAniOrder >= DRAO_ED )
		{
			m_pPicCharED->DrawText( (int)m_fED, D3DXVECTOR3(640,130,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );
		}
		*/
	}
}

wstring CX2StateArcadeResult::GetRecordTime( int iPlayTime )
{
	float fTime = iPlayTime/100.0f;

	//int iHour	= (int)(fTime / 3600.f);
	int iMinute = (int)( fTime / 60.f  );
	int iSecond = (int)(fTime-(float)iMinute*60.f );
	float fMilli = fTime - (int)fTime;
	fMilli = fMilli * 100;

	wstring recordTime;
	wstringstream wstrstm;
	wstrstm << iMinute << L"=" << iSecond << L"=" << (int)fMilli;
	recordTime = wstrstm.str().c_str();
	return recordTime;
}

wstring CX2StateArcadeResult::GetRecordTimeEx( int iPlayTime )
{
	float fTime = iPlayTime/100.0f;

	//int iHour	= (int)(fTime / 3600.f);
	int iMinute = (int)( fTime / 60.f  );
	int iSecond = (int)(fTime-(float)iMinute*60.f );
	float fMilli = fTime - (int)fTime;
	fMilli = fMilli * 100;

	wstring recordTime;
	wstringstream wstrstm;
	wstrstm << iMinute << L";" << iSecond << L";" << (int)fMilli;
	recordTime = wstrstm.str().c_str();
	return recordTime;
}
