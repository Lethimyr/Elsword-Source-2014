#include "StdAfx.h"
#include ".\x2statetrainingresult.h"

CX2StateTrainingResult::CX2StateTrainingResult(void)
{
	m_DungeonResultAniOrder = DRAO_ED_BG;
}

CX2StateTrainingResult::~CX2StateTrainingResult(void)
{
#ifndef HEAP_BROKEN_BY_ROOM
	g_pData->DeleteDungeonRoom();
#endif // HEAP_BROKEN_BY_ROOM

}

HRESULT CX2StateTrainingResult::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bInitUIFail == true )
		return S_OK;

	CX2StateCommonBG::OnFrameMove( fTime, fElapsedTime );
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	if ( m_bStartTimer == true )
		m_fRemainTime -= fElapsedTime;

	if ( m_fRemainTime <= 0.0f && m_bSendRefreshRoomReq == false )
	{
		m_bSendRefreshRoomReq = true;

		//
		//CX2Unit::UNIT_CLASS eUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();
		//int iNextTrainingID = g_pData->GetTrainingCenterTable()->GetNextTrainingID( eUnitClass, g_pMain->GetNowTrainingID() );
		//if( iNextTrainingID != -1 &&
		//	(g_pMain->GetTrainingRewardEXP() > 0 || g_pMain->GetTrainingRewardED() > 0) ) // 훈련에 성공했고 다음 훈련이 남아 있을때
		//{
		//	m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
		//		L"다음 훈련을 진행하시겠습니까?", STSUCM_START_NEXT_TRAINING_OK, this, STSUCM_START_NEXT_TRAINING_CANCEL );
		//}
		//else
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_743 ) );
			g_pMain->SetAutoStartNextTraining( false );
			Handler_EGS_STATE_CHANGE_FIELD_REQ();
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
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGBack ) == false )
		{
			return S_OK;
		}

		const int X2_MAGIC_MULTY_NUM = 3500;
		const int X2_MAGIC_MULTY_NUM2 = 35;
		const int X2_MAGIC_MULTY_NUM3 = 70000;
		const int X2_MAGIC_MULTY_NUM4 = 350;


		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(m_DungeonResultAniOrder)
		{
		case DRAO_ED_BG:
			{
				g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
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
// 						pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp04",
// 							pSlotData->m_pUnitViewer->GetMatrix().GetPos().x,
// 							pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
// 							pSlotData->m_pUnitViewer->GetMatrix().GetPos().z, 
// 							0,0,0, 0,0,0 );
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
						//	g_pKTDXApp->GetDSManager()->GetMP3Play()->Play( L"LevelUpBGM.mp3" );

						g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUpBGM.ogg" );

						g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LevelUp", 233, 458, 0, 1000, 1000, 1, 1, 1 );
						CKTDGParticleSystem::CParticleEventSequence* pPartInst = NULL;
						pPartInst = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LobbyLevelUp01", pSlotData->m_pUnitViewer->GetMatrix().GetPos() );
						pPartInst->SetLatency( 2.266f );
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

					int iCount = 15;
					for( UINT i=0; i<g_pMain->GetDungeonRewardItem().size(); i++ )
					{
						if( iCount > (15 + 23 ) )
							break;

						const KItemInfo& itemInfo = g_pMain->GetDungeonRewardItem().at(i);

                        wstring wTexName = g_pData->GetItemManager()->GetItemTemplet( itemInfo.m_iItemID )->GetShopImage();
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
							WCHAR buff[256] = {0,};
							//wsprintf( buff, L"%d", itemInfo.m_iQuantity );
							StringCchPrintf( buff, 256, L"%d", itemInfo.m_iQuantity );
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

						int nowExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
						int nowBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NowBaseLevelEXP;
						int nextBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NextBaseLevelEXP;

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

					if ( m_fED >= (float)g_pMain->GetTrainingRewardED() )
					{
						m_fED = (float)g_pMain->GetTrainingRewardED();

						//g_pKTDXApp->GetDeviceManager()->StopSound( L"Count.ogg" );

						m_DungeonResultAniOrder = DRAO_ITEM;

					}
					/*
					WCHAR edBuff[256] = {0,};
					//wsprintf( edBuff, L"%d", (int)m_fED );
					StringCchPrintf( buff, 256, L"%d", (int)m_fED );
					CKTDGUIStatic* pStaticED = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"StaticReward_ED" );
					pStaticED->GetString(0)->msg = edBuff;
					*/
				}
			} break;
		case DRAO_ITEM:
			{
				m_DungeonResultAniOrder = DRAO_BONUS_EXP;
			} break;
		case DRAO_BONUS_EXP:
			{


				if ( m_bCheckBonusEXP == false )
				{
					m_bCheckBonusEXP = true;
					/*
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
					}
					*/
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

				//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg" );

				if ( m_fEXP >= (float)g_pMain->GetTrainingRewardEXP() )
				{
					m_fEXP = (float)g_pMain->GetTrainingRewardEXP();

					// here stamp effect
					CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
					if ( pStaticEXPBG != NULL )
					{

					}


					m_DungeonResultAniOrder = DRAO_EXP_BAR;
				}	
			} break;
		case DRAO_EXP_BAR:
			{
				/*
				//EXP setting
				CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
				D3DXVECTOR2 picOrgSize = pStaticEXPBG->GetPicture(27)->GetOriginalSize();

				int nowExp = pSlotData->m_pUnit->GetUnitData().m_EXP;
				int nowBaseExp = pSlotData->m_pUnit->GetUnitData().m_NowBaseLevelEXP;
				int nextBaseExp = pSlotData->m_pUnit->GetUnitData().m_NextBaseLevelEXP;



				m_fEXPSize += (fElapsedTime*X2_MAGIC_MULTY_NUM4);

				float maxSize = 0;
				if ( pSlotData->m_pUnit->GetPrevLevel() == pSlotData->m_pUnit->GetUnitData().m_Level )
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

				if ( pSlotData->m_pUnit->GetPrevLevel() < pSlotData->m_pUnit->GetUnitData().m_Level )
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

				g_pData->GetMyUser()->GetSelectUnit()->SetPrevLevel( g_pData->GetSelectUnitLevel());

				m_DungeonResultAniOrder = DRAO_END;
			} break;
		case DRAO_END:
			{
				if( pSlotData->m_pUnitViewer->GetXSkinAnim() )
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

	}


	return S_OK;
}



bool CX2StateTrainingResult::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;


	switch(wParam)
	{
	case STSUCM_START_NEXT_TRAINING_OK:
		{
			m_fRemainTime = -1.f;
			g_pMain->SetAutoStartNextTraining( true );
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_739 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_TRAINING_SCHOOL, 0, false );
			if( NULL != m_pDLGMsgBox )
			{
				m_pDLGMsgBox->SetShowEnable( false, false );
			}
		
			return true;
		} break;

	case STSUCM_START_NEXT_TRAINING_CANCEL:
		{
			m_fRemainTime = -1.f;
			g_pMain->SetAutoStartNextTraining( false );
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_739 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_TRAINING_SCHOOL, 0, false );
			if( NULL != m_pDLGMsgBox )
			{
				m_pDLGMsgBox->SetShowEnable( false, false );
			}
			return true;
		} break;
	}


	return false;
}



