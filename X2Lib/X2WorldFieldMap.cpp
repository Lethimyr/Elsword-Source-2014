#include "StdAfx.h"
#include ".\x2worldfieldmap.h"


CX2WorldFieldMap::CX2WorldFieldMap(void) :
m_pCurrFieldData( NULL ),
m_hSeqNewQuestRing(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_pFont( NULL )
, m_iMapID( -1 )
{
	CKTDGObject::SetLayer( XL_MINIMAP ); 
	CKTDGObject::SetAlphaObject( true );

	SetOverUI( true );

	m_pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_11_NORMAL );
}

CX2WorldFieldMap::~CX2WorldFieldMap(void)
{
}

/*virtual*/ void CX2WorldFieldMap::NotifyShowObjectChanged()
{

}

/*virtual*/ void CX2WorldFieldMap::OnFrameRender_Draw()
{
	D3DXVECTOR2 vProjectedPos( 0, 0 );
	const int MAGIC_ICON_SIZE_X = 30;	
	const int MAGIC_ICON_SIZE_Y = 30;
		
	if( m_pMyChar != NULL)
		m_pMyChar->SetShow(false);
	if( m_pPartyMember[0] != NULL )
		m_pPartyMember[0]->SetShow(false);
	if( m_pPartyMember[1] != NULL)
		m_pPartyMember[1]->SetShow(false);
	if( m_pPartyMember[2] != NULL)
		m_pPartyMember[2]->SetShow(false);

	if ( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
	{
		if( NULL == g_pTFieldGame )
			return;

		if( NULL == m_pCurrFieldData )
			return;
		
		if( g_pTFieldGame != NULL && m_pCurrFieldData->m_iMapID == g_pData->GetLocationManager()->GetCurrentVillageID() )
		{
			for( int i = 0; i < g_pTFieldGame->GetFieldNPCCount(); ++i )
			{
				CX2TFieldNpc* pFieldNPC = g_pTFieldGame->GetFieldNPC( i );
				if( NULL == pFieldNPC )
					continue;

				vProjectedPos = ProjectToScreenShot( pFieldNPC->GetPos() );
				vProjectedPos = CalcPosInWindowTexture( vProjectedPos );

				if( true == IsInWindowTexture( vProjectedPos )  )
				{
					vProjectedPos = CalcPosOnScreen( vProjectedPos );

					vProjectedPos.x = vProjectedPos.x + m_pFieldDLG->GetPos().x;
					vProjectedPos.y = vProjectedPos.y + m_pFieldDLG->GetPos().y;

					/*switch( pFieldNPC->GetHouseType() )
					{
					case CX2TFieldNpc::NT_EQUIP:
						{
							if ( m_pTextureIcon[MI_NPC_WEAPON_SHOP] != NULL )
								m_pTextureIcon[MI_NPC_WEAPON_SHOP]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 4, 
									(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y * 2, MAGIC_ICON_SIZE_X * 2, MAGIC_ICON_SIZE_Y * 2 );	
						} break;
					case CX2TFieldNpc::NT_ACCESSORY:
						{
							if ( m_pTextureIcon[MI_NPC_ACCESSARY_SHOP] != NULL )
								m_pTextureIcon[MI_NPC_ACCESSARY_SHOP]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 4,
									(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y * 2, MAGIC_ICON_SIZE_X * 2, MAGIC_ICON_SIZE_Y * 2 );
						} break;
					case CX2TFieldNpc::NT_PVP:
						{
							if ( m_pTextureIcon[MI_PVP_ARENA] != NULL )
								m_pTextureIcon[MI_PVP_ARENA]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 4,
									(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y * 2, MAGIC_ICON_SIZE_X * 2, MAGIC_ICON_SIZE_Y * 2 );			
						} break;

					case CX2TFieldNpc::NT_ALCHEMIST:
						{
							if ( m_pTextureIcon[MI_NPC_ALCHEMIST] != NULL )
								m_pTextureIcon[MI_NPC_ALCHEMIST]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 4,
									(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y * 2, MAGIC_ICON_SIZE_X * 2, MAGIC_ICON_SIZE_Y * 2 );		
						} break;

					case CX2TFieldNpc::NT_POSTBOX:
						{
							if ( m_pTextureIcon[MI_POSTBOX] != NULL )
								m_pTextureIcon[MI_POSTBOX]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 4,
									(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y * 2, MAGIC_ICON_SIZE_X * 2, MAGIC_ICON_SIZE_Y * 2 );			
						} break;

					case CX2TFieldNpc::NT_RANKING:
						{	
							if ( m_pTextureIcon[MI_BOARD] != NULL )
								m_pTextureIcon[MI_BOARD]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 4,
									(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y * 2, MAGIC_ICON_SIZE_X * 2, MAGIC_ICON_SIZE_Y * 2 );			
						} break;

					case CX2TFieldNpc::NT_PRIVATE_BANK:
						{	
							if ( m_pTextureIcon[MI_BANK] != NULL )
								m_pTextureIcon[MI_BANK]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 4,
									(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y * 2, MAGIC_ICON_SIZE_X * 2, MAGIC_ICON_SIZE_Y * 2 );			
						} break;

					case CX2TFieldNpc::NT_NO_RENDER_EVENT:	
						break;

					case CX2TFieldNpc::NT_EVENT:
					case CX2TFieldNpc::NT_TRAINNING:
					case CX2TFieldNpc::NT_NORMAL:
					default:
						{
							if ( m_pTextureIcon[MI_NPC_COMMON] != NULL )
								m_pTextureIcon[MI_NPC_COMMON]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 4,
									(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y * 2, MAGIC_ICON_SIZE_X * 2, MAGIC_ICON_SIZE_Y * 2 );		
						} break;
					}
					*/
					const int MAGIC_QUEST_ICON_OFFSET_X = 1;
					const int MAGIC_QUEST_ICON_OFFSET_Y = MAGIC_ICON_SIZE_Y - 4;

					int iQuestIconPositionX = (int)vProjectedPos.x + MAGIC_QUEST_ICON_OFFSET_X;
					int iQuestIconPositionY = (int)vProjectedPos.y - MAGIC_QUEST_ICON_OFFSET_Y;
					int iEventQuestIconPositionX = (int)vProjectedPos.x - MAGIC_QUEST_ICON_OFFSET_X;
					int iEventQuestIconPositionY = (int)vProjectedPos.y - MAGIC_QUEST_ICON_OFFSET_Y;
				
					// 퀘스트 정보 출력
					if( pFieldNPC->GetCompleteQuestNormal() > 0 )
					{
						if ( m_pTextureIcon[MI_NPC_COMPLETE_QUEST] != NULL )
							m_pTextureIcon[MI_NPC_COMPLETE_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY,
																		 (int) (MAGIC_ICON_SIZE_X), (int) (MAGIC_ICON_SIZE_Y) );		
					}
					else if( pFieldNPC->GetCompleteQuestRepeat() > 0 )
					{
						if ( m_pTextureIcon[MI_NPC_COMPLETE_REPEAT_QUEST] != NULL )
							m_pTextureIcon[MI_NPC_COMPLETE_REPEAT_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY,
																				(int) (MAGIC_ICON_SIZE_X), (int) (MAGIC_ICON_SIZE_Y) );		
					}
					else if( pFieldNPC->GetFairLvQuest() > 0) // ( pFieldNPC->GetNewQuestNormal() > 0 )
					{
						if ( m_pTextureIcon[MI_NPC_AVAIL_QUEST] != NULL )
							m_pTextureIcon[MI_NPC_AVAIL_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY,
																	  (int) (MAGIC_ICON_SIZE_X), (int) (MAGIC_ICON_SIZE_Y) );		
					}
					else if( pFieldNPC->GetNewQuestRepeat() > 0 )
					{
						if ( m_pTextureIcon[MI_NPC_AVAIL_REPEAT_QUEST] != NULL )
							m_pTextureIcon[MI_NPC_AVAIL_REPEAT_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY,
																			 (int) (MAGIC_ICON_SIZE_X), (int) (MAGIC_ICON_SIZE_Y) );		
					}
					else if( pFieldNPC->GetDoQuest() > 0 )
					{
						if ( m_pTextureIcon[MI_NPC_INCOMPLETE_QUEST] != NULL )
							m_pTextureIcon[MI_NPC_INCOMPLETE_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY,
																		   (int) (MAGIC_ICON_SIZE_X), (int) (MAGIC_ICON_SIZE_Y) );		
					}


					if( pFieldNPC->GetEventQuest() > 0 )
					{
						if ( m_pTextureIcon[MI_NPC_AVAIL_EVENT_QUEST] != NULL )
							m_pTextureIcon[MI_NPC_AVAIL_EVENT_QUEST]->Draw( iEventQuestIconPositionX , iEventQuestIconPositionY,
																			(int) (MAGIC_ICON_SIZE_X), (int) (MAGIC_ICON_SIZE_Y) );
					}
				}
			}

			// 내 유닛 위치 표시
			CX2SquareUnit* pMySquareUnit = g_pTFieldGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
			if( NULL != pMySquareUnit )
			{
				vProjectedPos = ProjectToScreenShot( pMySquareUnit->GetPos() );
				vProjectedPos = CalcPosInWindowTexture( vProjectedPos );

				if( true == IsInWindowTexture( vProjectedPos ) )
				{
					vProjectedPos = CalcPosOnScreen( vProjectedPos );
					D3DXVECTOR2 vPos( vProjectedPos.x - MAGIC_ICON_SIZE_X / 2 , vProjectedPos.y - MAGIC_ICON_SIZE_Y  );
					if ( NULL != m_pMyChar )
					{
						m_pMyChar->SetOffsetPos( vPos );
						m_pMyChar->SetShowEnable( true, true );
					}

#ifdef FIX_FIELD_MAP_SCROLLING
					D3DXVECTOR2 vDiffrence = m_vProjectedPos - vProjectedPos;
					if( D3DXVec2Length( &vDiffrence ) > 0.1f )
						CalcPosForScroll( vProjectedPos );

					m_vProjectedPos = vProjectedPos;						
#else FIX_FIELD_MAP_SCROLLING
					vProjectedPos = CalcPosForScroll( vProjectedPos );
#endif FIX_FIELD_MAP_SCROLLING

					/*m_pTextureIcon[MI_MYSELF]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, 
						(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y , MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );*/
				}
			}

			// 파티원 위치 표시
			if( true == g_pData->GetPartyManager()->DoIHaveParty() )
			{
				for( int i=0; i < g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount(); ++i )
				{
					const CX2PartyManager::PartyMemberData* pPartyMemberData = 
						g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( i );
					if( pPartyMemberData->m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
						continue;

					if( CX2Unit::CUS_FIELD_MAP != pPartyMemberData->m_eState ||	pPartyMemberData->m_iStateCode != (int) m_pCurrFieldData->m_iMapID )
						continue;

					CX2SquareUnit* pPartySquareUnit = g_pTFieldGame->GetSquareUnitByUID( pPartyMemberData->m_iUnitUID );
					D3DXVECTOR3 vPos = pPartyMemberData->m_vPosition;

					if( NULL != pPartySquareUnit )
						vPos = pPartySquareUnit->GetPos();

					vProjectedPos = ProjectToScreenShot( vPos );
					vProjectedPos = CalcPosInWindowTexture( vProjectedPos );

					if( false == IsInWindowTexture( vProjectedPos ) )
						continue;

					vProjectedPos = CalcPosOnScreen( vProjectedPos );
					D3DXVECTOR2 vPos2( vProjectedPos.x - MAGIC_ICON_SIZE_X / 2 , vProjectedPos.y - MAGIC_ICON_SIZE_Y  );
					if ( NULL != m_pPartyMember[i] )
					{
						m_pPartyMember[i]->SetOffsetPos( vPos2 );
						m_pPartyMember[i]->SetShowEnable( true, true );
					}

					/*m_pTextureIcon[MI_PARTY]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, 
						(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y , MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );*/

					const int MAGIC_FONT_OFFSET = 2;

#ifdef FIX_PARTY_MEMBER_POSITION
					vProjectedPos.x += m_pFieldDLG->GetPos().x;
#endif FIX_PARTY_MEMBER_POSITION

					m_pFont->OutTextXY( (int)vProjectedPos.x, (int)vProjectedPos.y + MAGIC_FONT_OFFSET, 
						pPartyMemberData->m_wstrNickName.c_str(), D3DCOLOR_RGBA(255,255,255,255), 
						CKTDGFontManager::FS_SHELL, D3DCOLOR_RGBA(0,0,0,255), NULL, DT_CENTER );
				}
			}
		}
	}
	else if ( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		if( NULL != g_pX2Game && m_iMapID == static_cast<SEnum::VILLAGE_MAP_ID>( g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam() ) )
		{
			// 내 유닛 위치 표시
			CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();

			if( NULL != pMyUnit )
			{
				vProjectedPos = ProjectToScreenShot( pMyUnit->GetPos() );
				vProjectedPos = CalcPosInWindowTexture( vProjectedPos );

				if( true == IsInWindowTexture( vProjectedPos ) )
				{
					vProjectedPos = CalcPosOnScreen( vProjectedPos );
				
					D3DXVECTOR2 vPos( vProjectedPos.x - MAGIC_ICON_SIZE_X / 2 , vProjectedPos.y - MAGIC_ICON_SIZE_Y  );
					if ( NULL != m_pMyChar )
					{
						m_pMyChar->SetOffsetPos( vPos );
						m_pMyChar->SetShowEnable( true, true );
					}

#ifdef FIX_FIELD_MAP_SCROLLING
					D3DXVECTOR2 vDiffrence = m_vProjectedPos - vProjectedPos;
					if( D3DXVec2Length( &vDiffrence ) > 0.1f )
						CalcPosForScroll( vProjectedPos );

					m_vProjectedPos = vProjectedPos;						
#else FIX_FIELD_MAP_SCROLLING
					vProjectedPos = CalcPosForScroll( vProjectedPos );
#endif FIX_FIELD_MAP_SCROLLING
				
					/*m_pTextureIcon[MI_MYSELF]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y , MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );*/
				}
			}


			// 파티원 위치 표시
			if( true == g_pData->GetPartyManager()->DoIHaveParty() )
			{
				for( int i = 0; i < g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount(); ++i )
				{
					const CX2PartyManager::PartyMemberData* pPartyMemberData = 
						g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( i );

					if( NULL == pPartyMemberData )
						continue;

					if( pPartyMemberData->m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
						continue;

					if( CX2Unit::CUS_FIELD_MAP != pPartyMemberData->m_eState )
						continue;

					CX2GUUser* pPartyUser = g_pX2Game->GetUserUnitByUID( pPartyMemberData->m_iUnitUID );				

					// 오현빈 // 2013-08-22 // 같은 필드 내에 존재하지 않는 유저가 이상한 위치에 보이는 문제 수정
					if( NULL == pPartyUser )
						continue;

					D3DXVECTOR3 vPos = pPartyMemberData->m_vPosition;

					if( NULL != pPartyUser )
						vPos = pPartyUser->GetPos();

					vProjectedPos = ProjectToScreenShot( vPos );
					vProjectedPos = CalcPosInWindowTexture( vProjectedPos );
					vProjectedPos = CalcPosOnScreen( vProjectedPos );
					D3DXVECTOR2 vPos2( vProjectedPos.x - MAGIC_ICON_SIZE_X / 2 , vProjectedPos.y - MAGIC_ICON_SIZE_Y  );
					if ( NULL != m_pPartyMember[i] )
					{
						m_pPartyMember[i]->SetOffsetPos( vPos2 );
						m_pPartyMember[i]->SetShowEnable( true, true );
					}

					/*m_pTextureIcon[MI_PARTY]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, 
						(int) vProjectedPos.y - MAGIC_ICON_SIZE_Y , MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );*/

#ifdef FIX_PARTY_MEMBER_POSITION
					vProjectedPos.x += m_pFieldDLG->GetPos().x;
#endif FIX_PARTY_MEMBER_POSITION

					const int MAGIC_FONT_OFFSET = 2;
					m_pFont->OutTextXY( (int)vProjectedPos.x, (int)vProjectedPos.y + MAGIC_FONT_OFFSET, 
						pPartyMemberData->m_wstrNickName.c_str(), D3DCOLOR_RGBA(255,255,255,255), 
						CKTDGFontManager::FS_SHELL, D3DCOLOR_RGBA(0,0,0,255), NULL, DT_CENTER );
				}
			}
		}
	}
}



void CX2WorldFieldMap::SetVillageID( int iMapID )
{
	ResetCameraMatrix( iMapID );
	ResetMapPosAndSize( iMapID );


	m_pCurrFieldData = GetFieldInfo( iMapID );

	m_iMapID = iMapID;
}


// 해당 field에 있는 npc의 위치, 퀘스트 정보를 갱신해서 가져온다.
// @iMapID는 SEnum::VILLAGE_MAP_ID이다
CX2WorldFieldMap::FieldData* CX2WorldFieldMap::GetFieldInfo( int iMapID )
{
	std::map< int, FieldData >::iterator it = m_mapFieldData.find( iMapID );
	//{{ 09.03.11 김태완 : 이러면 빠른 건 좋은데 퀘스트 정보 업데이트가 안 됨
	// 그러니 퀘스트 정보만 업데이트해주자
	bool bFieldDataExist = false;
	if( it != m_mapFieldData.end() )
	{
		FieldData& refNewFieldData = it->second;
		
		for(std::map< CX2UnitManager::NPC_UNIT_ID, FieldNPCData >::iterator itmFieldNPCData = refNewFieldData.m_mapFieldNPCData.begin();
			itmFieldNPCData != refNewFieldData.m_mapFieldNPCData.end();
			advance(itmFieldNPCData, 1) )
		{
			FieldNPCData& refFieldNPCData = itmFieldNPCData->second;
			int myLevel = g_pData->GetSelectUnitLevel();

			refFieldNPCData.m_nFairLvQuest = g_pData->GetQuestManager()->GetNewQuest(refFieldNPCData.m_eNPCID, refFieldNPCData.m_nNewQuestRepeat, refFieldNPCData.m_nNewQuestNormal, refFieldNPCData.m_nEventQuest, myLevel);
			// oasis907 : 김상윤 [2010.8.3] // 수행중, 완료 가능 퀘스트 마크 없애기 (에픽퀘스트 개편으로 End NPC가 필요없어짐)
			//g_pData->GetQuestManager()->GetDoQuest(refFieldNPCData.m_eNPCID, refFieldNPCData.m_nDoQuest);
			//g_pData->GetQuestManager()->GetCompleteQuest(refFieldNPCData.m_eNPCID, refFieldNPCData.m_nCompleteQuestRepeat, refFieldNPCData.m_nCompleteQuestNormal);
#ifdef QUEST_GUIDE
			g_pData->GetQuestManager()->GetCompleteTalkQuest( refFieldNPCData.m_eNPCID, refFieldNPCData.m_nCompleteQuestNormal );
#endif //QUEST_GUIDE
		}

		return &refNewFieldData;
	}
	

	CX2LocationManager::VillageTemplet* pVillageTemplate = g_pData->GetLocationManager()->GetVillageMapTemplet( (SEnum::VILLAGE_MAP_ID) iMapID );
	if( NULL == pVillageTemplate )
		return NULL;


	FieldData newFieldData;
	newFieldData.m_iMapID = iMapID;
	for( UINT i=0; i<pVillageTemplate->m_HouseList.size(); ++i)
	{
		CX2LocationManager::HouseTemplet* pHouseTemplate = g_pData->GetLocationManager()->GetHouseTemplet(pVillageTemplate->m_HouseList[i]);
		if( NULL == pHouseTemplate )
		{
			ASSERT( !"invalid house id" );
			continue;
		}

		if( true == pHouseTemplate->m_NPCList.empty() )
		{
			continue;
		}


		FieldNPCData newFieldNPCData;
		newFieldNPCData.m_eNPCID	= pHouseTemplate->m_NPCList[0];
		
		if( true == pHouseTemplate->m_vecCommonPos.empty() )
		{
			newFieldNPCData.m_vPos = pHouseTemplate->m_NPCMeshPos;
		}
		else
		{
			CX2World::WORLD_ID eWorldID = (CX2World::WORLD_ID) g_pData->GetLocationManager()->GetWorldID(  (SEnum::VILLAGE_MAP_ID) iMapID );

			BOOST_TEST_FOREACH( CX2LocationManager::CommonNpcPos&, npcPos, pHouseTemplate->m_vecCommonPos )
			{
				if( eWorldID == npcPos.m_eWorldID )
				{
					newFieldNPCData.m_vPos = npcPos.m_vNpcPos;
					break;
				}
			}
		}
		int myLevel = g_pData->GetSelectUnitLevel();

		newFieldNPCData.m_nFairLvQuest = g_pData->GetQuestManager()->GetNewQuest((CX2UnitManager::NPC_UNIT_ID)pHouseTemplate->m_NPCList[0], newFieldNPCData.m_nNewQuestRepeat, newFieldNPCData.m_nNewQuestNormal, newFieldNPCData.m_nEventQuest, myLevel);
		// oasis907 : 김상윤 [2010.8.3] // 수행중, 완료 가능 퀘스트 마크 없애기 (에픽퀘스트 개편으로 End NPC가 필요없어짐)
		//g_pData->GetQuestManager()->GetDoQuest((CX2UnitManager::NPC_UNIT_ID)pHouseTemplate->m_NPCList[0], newFieldNPCData.m_nDoQuest);
		//g_pData->GetQuestManager()->GetCompleteQuest((CX2UnitManager::NPC_UNIT_ID)pHouseTemplate->m_NPCList[0], newFieldNPCData.m_nCompleteQuestRepeat, newFieldNPCData.m_nCompleteQuestNormal);

		newFieldData.m_mapFieldNPCData[ (CX2UnitManager::NPC_UNIT_ID)pHouseTemplate->m_NPCList[0] ] = newFieldNPCData;
	}

	m_mapFieldData[ iMapID ] = newFieldData;

	return &m_mapFieldData[ iMapID ];
}

void CX2WorldFieldMap::MakeQuestNoticeEffect( int NPCID )
{

	if( NULL != m_pCurrFieldData && m_pCurrFieldData->m_mapFieldNPCData.find((CX2UnitManager::NPC_UNIT_ID)NPCID) != m_pCurrFieldData->m_mapFieldNPCData.end() )
	{
		const FieldNPCData& NPCData = m_pCurrFieldData->m_mapFieldNPCData[(CX2UnitManager::NPC_UNIT_ID)NPCID];
		
		D3DXVECTOR2 vProjectedPos;
		vProjectedPos = ProjectToScreenShot( NPCData.m_vPos );
		vProjectedPos = CalcPosInWindowTexture( vProjectedPos );
		vProjectedPos = CalcPosOnScreen( vProjectedPos );

		const int MAGIC_EFFECT_OFFSET_X = 0;	
		const int MAGIC_EFFECT_OFFSET_Y = -15;

		vProjectedPos.x += MAGIC_EFFECT_OFFSET_X;
		vProjectedPos.y += MAGIC_EFFECT_OFFSET_Y;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"HyperBallTick", vProjectedPos.x, vProjectedPos.y, 0.0f, 1000,1000, -1, 10 );
		if( pSeq != NULL )
		{
			pSeq->SetOverUI( true );
			pSeq->SetBlackHolePosition( pSeq->GetPosition() );
			pSeq->UseLookPoint( true );
			pSeq->SetLookPoint( pSeq->GetPosition() );
			pSeq->SetLatency( 0.3f );
		}
		
		DestoryQuestNoticeEffect();
		pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"HyperBallRing", vProjectedPos.x, vProjectedPos.y , 0.0f, 1000,1000, 1, -1 );
		if( pSeq != NULL )
		{
			pSeq->SetOverUI( true );		
			pSeq->SetLatency( 0.3f );
			m_hSeqNewQuestRing = pSeq->GetHandle();
		}
	
	}

}

void CX2WorldFieldMap::DestoryQuestNoticeEffect()
{
	if(m_hSeqNewQuestRing != INVALID_PARTICLE_SEQUENCE_HANDLE)
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqNewQuestRing );
	m_hSeqNewQuestRing = INVALID_PARTICLE_SEQUENCE_HANDLE;

}

D3DXVECTOR2 CX2WorldFieldMap::CalcPosForScroll( const D3DXVECTOR2& vPos_ )
{
	D3DXVECTOR2 vScrollPos( 0, 0 );
	D3DXVECTOR2 vWindowPos( 0, 0 );
	D3DXVECTOR2 vRightBottom( 0, 0 );

	if ( NULL != m_pFieldDLG )
	{
		CX2WorldMapUI *pWorldMapUI = g_pInstanceData->GetMiniMapUI()->GetWorldMapUI();

		if ( false == m_pFieldDLG->GetMouseDownForDrag() && NULL != pWorldMapUI && false == pWorldMapUI->GetDragNaviMap() )
		{
			vRightBottom = m_pFieldDLG->GetDraggingRangeCenter() - m_pFieldDLG->GetDraggingRange() * 0.5f - m_pFieldDLG->GetSize() * 0.5f;
			vWindowPos.x = vRightBottom.x * vPos_.x / m_pFieldDLG->GetSize().x;
			vWindowPos.y = vRightBottom.y * vPos_.y / m_pFieldDLG->GetSize().y;
			m_pFieldDLG->SetPos( vWindowPos );

			pWorldMapUI->UpdateFieldMap();
		}

		vScrollPos.x = vPos_.x + m_pFieldDLG->GetPos().x;
		vScrollPos.y = vPos_.y + m_pFieldDLG->GetPos().y;
			
		return vScrollPos;
	}

	return vPos_;
}

void CX2WorldFieldMap::InitCharacterMaker()
{
	if ( NULL != m_pFieldDLG )
	{
		if ( false == m_pFieldDLG->CheckControl( L"Button_MyChar" ) )
		{
			m_pMyChar = m_pFieldDLG->CreateButton();
			m_pFieldDLG->AddControl( m_pMyChar );
			m_pMyChar->SetName( L"Button_MyChar" );
			m_pMyChar->SetNormalTex( L"DLG_UI_Common_Texture70_NEW.tga", L"MiniMap_Char_My" );
			m_pMyChar->SetOverTex( L"DLG_UI_Common_Texture70_NEW.tga", L"MiniMap_Char_My" );
			m_pMyChar->SetDownTex( L"DLG_UI_Common_Texture70_NEW.tga", L"MiniMap_Char_My" );

			CKTDGUIControl::UIPointData* pNormalPointData = new CKTDGUIControl::UIPointData();
			pNormalPointData->leftTopPoint = D3DXVECTOR2( 0.f, 0.f );
			pNormalPointData->addSize.x = -8;
			pNormalPointData->addSize.y = -18.f;
			pNormalPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			CKTDGUIControl::UIPointData* pOverPointData = new CKTDGUIControl::UIPointData();
			pOverPointData->leftTopPoint = D3DXVECTOR2( 0.f, 0.f );
			pOverPointData->addSize.x = -8;
			pOverPointData->addSize.y = -18.f;
			pOverPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			CKTDGUIControl::UIPointData* pDownPointData = new CKTDGUIControl::UIPointData();
			pDownPointData->leftTopPoint = D3DXVECTOR2( 0.f, 0.f );
			pDownPointData->addSize.x = -8;
			pDownPointData->addSize.y = -18.f;
			pDownPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			m_pMyChar->SetNormalPoint( pNormalPointData );
			m_pMyChar->SetOverPoint( pOverPointData );
			m_pMyChar->SetDownPoint( pDownPointData );
			m_pMyChar->SetCustomMsgMouseOver( CX2WorldMapUI::WMUCM_MYCHAR_MOUSE_OVER );
			m_pMyChar->SetCustomMsgMouseOut( CX2WorldMapUI::WMUCM_MYCHAR_MOUSE_OUT );
			m_pMyChar->SetShowEnable( false, false );
		}
		else
		{
			m_pMyChar = static_cast<CKTDGUIButton*>( m_pFieldDLG->GetControl( L"Button_MyChar" ) );
		}

		for ( int i = 0; i < 4; ++i )
		{
			if ( false == m_pFieldDLG->CheckControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Button_PartyMember" ), i ) ) ) )
			{
				m_pPartyMember[i] = m_pFieldDLG->CreateButton();
				m_pFieldDLG->AddControl( m_pPartyMember[i] );
				m_pPartyMember[i]->SetName( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Button_PartyMember" ), i ) ) );
				m_pPartyMember[i]->SetNormalTex( L"DLG_UI_Common_Texture70_NEW.tga", L"MiniMap_Char_Party" );
				m_pPartyMember[i]->SetOverTex( L"DLG_UI_Common_Texture70_NEW.tga", L"MiniMap_Char_Party" );
				m_pPartyMember[i]->SetDownTex( L"DLG_UI_Common_Texture70_NEW.tga", L"MiniMap_Char_Party" );

				CKTDGUIControl::UIPointData* pNormalPointData = new CKTDGUIControl::UIPointData();
				pNormalPointData->leftTopPoint = D3DXVECTOR2( 0.f, 0.f );
				pNormalPointData->addSize.x = -8;
				pNormalPointData->addSize.y = -18.f;
				pNormalPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				CKTDGUIControl::UIPointData* pOverPointData = new CKTDGUIControl::UIPointData();
				pOverPointData->leftTopPoint = D3DXVECTOR2( 0.f, 0.f );
				pOverPointData->addSize.x = -8;
				pOverPointData->addSize.y = -18.f;
				pOverPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				CKTDGUIControl::UIPointData* pDownPointData = new CKTDGUIControl::UIPointData();
				pDownPointData->leftTopPoint = D3DXVECTOR2( 0.f, 0.f );
				pDownPointData->addSize.x = -8;
				pDownPointData->addSize.y = -18.f;
				pDownPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				m_pPartyMember[i]->SetNormalPoint( pNormalPointData );
				m_pPartyMember[i]->SetOverPoint( pOverPointData );
				m_pPartyMember[i]->SetDownPoint( pDownPointData );
				m_pPartyMember[i]->SetCustomMsgMouseOver( CX2WorldMapUI::WMUCM_PARTY_MEMBER_MOUSE_OVER );
				m_pPartyMember[i]->SetCustomMsgMouseOut( CX2WorldMapUI::WMUCM_PARTY_MEMBER_MOUSE_OUT );
				m_pPartyMember[i]->SetShowEnable( false, false );
				m_pPartyMember[i]->AddDummyInt(i);
			}
			else
			{
				m_pPartyMember[i] = static_cast<CKTDGUIButton*>( m_pFieldDLG->GetControl( GET_REPLACED_STRING(
								( STR_ID_3738, "Li", std::wstring( L"Button_PartyMember" ), i ) ) ) );

				m_pPartyMember[i]->SetCustomMsgMouseOver( CX2WorldMapUI::WMUCM_PARTY_MEMBER_MOUSE_OVER );
				m_pPartyMember[i]->SetCustomMsgMouseOut( CX2WorldMapUI::WMUCM_PARTY_MEMBER_MOUSE_OUT );

				if( -1 == m_pPartyMember[i]->GetDummyInt(0) )
				{
					m_pPartyMember[i]->AddDummyInt(i);
				}
				m_pPartyMember[i]->SetDummyInt(0, i);
			}
		}
	}
}
