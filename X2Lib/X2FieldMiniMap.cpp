#include "StdAfx.h"
#include "./X2FieldMiniMap.h"




CX2FieldMiniMap::CX2FieldMiniMap( CX2MiniMapUI* pMiniMapUI ) :
m_pStage(NULL)
{
	m_pMiniMapUI = pMiniMapUI;
	
	CKTDGObject::SetLayer( XL_MINIMAP ); 
	CKTDGObject::SetAlphaObject( true );

	// 	m_vMiniMapWindowPos		= D3DXVECTOR2( 744, 38 );
	// 	m_vMiniMapWindowSize	= D3DXVECTOR2( 242, 102 );				
	// 	m_vWindowTextureSize	= D3DXVECTOR2( 242, 102 );				

	m_vMiniMapWindowPos		= D3DXVECTOR2( 0, 0 );
	m_vMiniMapWindowSize	= D3DXVECTOR2( 248, 105 );				
	m_vWindowTextureSize	= D3DXVECTOR2( 248, 105 );				
				

}

/*virtual*/ CX2FieldMiniMap::~CX2FieldMiniMap()
{
    if ( g_pInstanceData->GetMiniMapUI() != NULL )
	    g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_FIELD, false );	
}


/*virtual*/ void CX2FieldMiniMap::NotifyShowObjectChanged()
{
	
}



/*virtual*/ void CX2FieldMiniMap::OnFrameRender_Draw()
{
	if( NULL == g_pTFieldGame )
		return; 

	if( true == g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) )
	{
		return;
	}

	D3DXVECTOR2 vProjectedPos = ProjectToScreenShot( g_pTFieldGame->GetMyUnit()->GetPos() );
	CalcWindowTexturePos( vProjectedPos );


	if( NULL != m_pTextureWholeMap )
	{
		m_pTextureWholeMap->Draw( m_vMiniMapWindowPos.x + 824, m_vMiniMapWindowPos.y + 558, m_vMiniMapWindowSize.x, m_vMiniMapWindowSize.y, 
			(int) (m_vWindowTexturePos.x - m_vPosInWholeTexture.x ), 
			(int) (m_vWindowTexturePos.y - m_vPosInWholeTexture.y ), 
			(int) m_vWindowTextureSize.x, 
			(int) m_vWindowTextureSize.y, D3DCOLOR_COLORVALUE( 1.f, 1.f, 1.f, 1.f ) );
	}

	const int MAGIC_SMALL_ICON_SIZE_X = 22;	
	const int MAGIC_SMALL_ICON_SIZE_Y = 22;
	const int MAGIC_ICON_SIZE_X = 36;	
	const int MAGIC_ICON_SIZE_Y = 36;
	const int MAGIC_BIG_ICON_OFFSET_Y = 8;

	for( int i=0; i<g_pTFieldGame->GetGatePortalCount(); i++ )
	{
		const D3DXVECTOR3 vPos = g_pTFieldGame->GetGatePortalPos(i);
		D3DXVECTOR2 vProjectedPos = ProjectToScreenShot( vPos );
		vProjectedPos = CalcPosInWindowTexture( vProjectedPos );

		if( false == IsInWindowTexture( vProjectedPos ) )
			continue;

		vProjectedPos = CalcPosOnScreen( vProjectedPos );

		if ( m_pTextureIcon[MI_GATE] != NULL )
			m_pTextureIcon[MI_GATE]->Draw( (int) vProjectedPos.x - MAGIC_SMALL_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_SMALL_ICON_SIZE_Y , MAGIC_SMALL_ICON_SIZE_X, MAGIC_SMALL_ICON_SIZE_Y  );			
	}

	for( int i=0; i<g_pTFieldGame->GetMarketEnteranceCount(); i++ )
	{
		const D3DXVECTOR3 vPos = g_pTFieldGame->GetMarketEnterancePos(i);
		D3DXVECTOR2 vProjectedPos = ProjectToScreenShot( vPos );
		vProjectedPos = CalcPosInWindowTexture( vProjectedPos );

		if( false == IsInWindowTexture( vProjectedPos ) )
			continue;

		vProjectedPos = CalcPosOnScreen( vProjectedPos );

        if ( m_pTextureIcon[MI_MARKET_PLACE] != NULL )
		    m_pTextureIcon[MI_MARKET_PLACE]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );			
	}



	for( int i=0; i<g_pTFieldGame->GetFieldNPCCount(); i++ )
	{
		CX2TFieldNpc* pFieldNPC = g_pTFieldGame->GetFieldNPC( i );
		if( NULL == pFieldNPC )
			continue;

		vProjectedPos = ProjectToScreenShot( pFieldNPC->GetPos() );
		vProjectedPos = CalcPosInWindowTexture( vProjectedPos );
		if( false == IsInWindowTexture( vProjectedPos ) )
			continue;
		vProjectedPos = CalcPosOnScreen( vProjectedPos );


		switch( pFieldNPC->GetHouseType() )
		{
		case CX2TFieldNpc::NT_EQUIP:
			{
                if ( m_pTextureIcon[MI_NPC_WEAPON_SHOP] != NULL )
				    m_pTextureIcon[MI_NPC_WEAPON_SHOP]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );	
			} break;
		case CX2TFieldNpc::NT_ACCESSORY:
			{
                if ( m_pTextureIcon[MI_NPC_ACCESSARY_SHOP] != NULL )
				    m_pTextureIcon[MI_NPC_ACCESSARY_SHOP]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );
			} break;
		case CX2TFieldNpc::NT_PVP:
			{
                if ( m_pTextureIcon[MI_PVP_ARENA] != NULL )
				    m_pTextureIcon[MI_PVP_ARENA]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );			
			} break;

		case CX2TFieldNpc::NT_ALCHEMIST:
			{
                if ( m_pTextureIcon[MI_NPC_ALCHEMIST] != NULL )
				    m_pTextureIcon[MI_NPC_ALCHEMIST]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );		
			} break;

		case CX2TFieldNpc::NT_POSTBOX:
			{
                if ( m_pTextureIcon[MI_POSTBOX] != NULL )
				    m_pTextureIcon[MI_POSTBOX]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );			
			} break;

		case CX2TFieldNpc::NT_RANKING:
			{	
                if ( m_pTextureIcon[MI_BOARD] != NULL )
				    m_pTextureIcon[MI_BOARD]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );			
			} break;

		case CX2TFieldNpc::NT_PRIVATE_BANK:
			{	
                if ( m_pTextureIcon[MI_BANK] != NULL )
				    m_pTextureIcon[MI_BANK]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );			
			} break;

			//{{ kimhc // 2009-12-08 // 미니맵 Render 안함
		case CX2TFieldNpc::NT_NO_RENDER_EVENT:	
			break;
			//}} kimhc // 2009-12-08 // 미니맵 Render 안함

		case CX2TFieldNpc::NT_EVENT:
		case CX2TFieldNpc::NT_TRAINNING:
		case CX2TFieldNpc::NT_NORMAL:
		default:
			{
                if ( m_pTextureIcon[MI_NPC_COMMON] != NULL )
				    m_pTextureIcon[MI_NPC_COMMON]->Draw( (int) vProjectedPos.x - MAGIC_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y, MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y  );		
			} break;
		}


		const int MAGIC_QUEST_ICON_OFFSET_X = 1;
		const int MAGIC_QUEST_ICON_OFFSET_Y = MAGIC_ICON_SIZE_Y+MAGIC_SMALL_ICON_SIZE_Y - 4;

		int iQuestIconPositionX = (int)vProjectedPos.x + MAGIC_QUEST_ICON_OFFSET_X;
		int iQuestIconPositionY = (int)vProjectedPos.y - MAGIC_QUEST_ICON_OFFSET_Y + MAGIC_BIG_ICON_OFFSET_Y;
		int iEventQuestIconPositionX = (int)vProjectedPos.x - MAGIC_QUEST_ICON_OFFSET_X - MAGIC_SMALL_ICON_SIZE_X;
		int iEventQuestIconPositionY = (int)vProjectedPos.y - MAGIC_QUEST_ICON_OFFSET_Y + MAGIC_BIG_ICON_OFFSET_Y;


		// 퀘스트 정보 출력
		if( pFieldNPC->GetCompleteQuestNormal() > 0 )
		{
            if ( m_pTextureIcon[MI_NPC_COMPLETE_QUEST] != NULL )
			    m_pTextureIcon[MI_NPC_COMPLETE_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY, (int) (MAGIC_SMALL_ICON_SIZE_X), (int) (MAGIC_SMALL_ICON_SIZE_Y) );		
		}
		else if( pFieldNPC->GetCompleteQuestRepeat() > 0 )
		{
            if ( m_pTextureIcon[MI_NPC_COMPLETE_REPEAT_QUEST] != NULL )
			    m_pTextureIcon[MI_NPC_COMPLETE_REPEAT_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY, (int) (MAGIC_SMALL_ICON_SIZE_X), (int) (MAGIC_SMALL_ICON_SIZE_Y) );		
		}
		else if( pFieldNPC->GetFairLvQuest() > 0) // ( pFieldNPC->GetNewQuestNormal() > 0 )
		{
            if ( m_pTextureIcon[MI_NPC_AVAIL_QUEST] != NULL )
			    m_pTextureIcon[MI_NPC_AVAIL_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY, (int) (MAGIC_SMALL_ICON_SIZE_X), (int) (MAGIC_SMALL_ICON_SIZE_Y) );		
		}
		else if( pFieldNPC->GetNewQuestRepeat() > 0 )
		{
            if ( m_pTextureIcon[MI_NPC_AVAIL_REPEAT_QUEST] != NULL )
			    m_pTextureIcon[MI_NPC_AVAIL_REPEAT_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY, (int) (MAGIC_SMALL_ICON_SIZE_X), (int) (MAGIC_SMALL_ICON_SIZE_Y) );		
		}
		else if( pFieldNPC->GetDoQuest() > 0 )
		{
            if ( m_pTextureIcon[MI_NPC_INCOMPLETE_QUEST] != NULL )
			    m_pTextureIcon[MI_NPC_INCOMPLETE_QUEST]->Draw( iQuestIconPositionX, iQuestIconPositionY, (int) (MAGIC_SMALL_ICON_SIZE_X), (int) (MAGIC_SMALL_ICON_SIZE_Y) );		
		}


		if( pFieldNPC->GetEventQuest() > 0 )
		{
			// 아이콘 튀어나가는 현상 때문에 이것만 위치 검사를 따로 해준다..
			// Y축은 검사하지 말자. x축만..
// 			D3DXVECTOR2 vPosEventQuestIcon = D3DXVECTOR2(vProjectedPos.x - (MAGIC_ICON_SIZE_X* 3/2), vProjectedPos.y - (MAGIC_ICON_SIZE_Y * 2));
// 			if(vPosEventQuestIcon.x > m_vMiniMapWindowPos.x && vPosEventQuestIcon.x < m_vMiniMapWindowPos.x+m_vMiniMapWindowSize.x )
// 			{
                if ( m_pTextureIcon[MI_NPC_AVAIL_EVENT_QUEST] != NULL )
				    m_pTextureIcon[MI_NPC_AVAIL_EVENT_QUEST]->Draw( iEventQuestIconPositionX , iEventQuestIconPositionY, (int) (MAGIC_SMALL_ICON_SIZE_X), (int) (MAGIC_SMALL_ICON_SIZE_Y) );
//			}
			
		}

	}





	// 내 유닛 위치 표시
	CX2SquareUnit* pMySquareUnit = g_pTFieldGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
	if( NULL != pMySquareUnit )
	{
		vProjectedPos = ProjectToScreenShot( pMySquareUnit->GetPos() );
		vProjectedPos = CalcPosInWindowTexture( vProjectedPos );

		//if( false == IsInWindowTexture( vProjectedPos ) )
		//	continue;

		vProjectedPos = CalcPosOnScreen( vProjectedPos );
        if ( m_pTextureIcon[MI_MYSELF] != NULL )
		    m_pTextureIcon[MI_MYSELF]->Draw( (int) vProjectedPos.x - MAGIC_SMALL_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_SMALL_ICON_SIZE_Y , MAGIC_SMALL_ICON_SIZE_X, MAGIC_SMALL_ICON_SIZE_Y  );			
	}


	// 파티원 위치 표시
	if( true == g_pData->GetPartyManager()->DoIHaveParty() )
	{
		for( int i=0; i<g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount(); i++ )
		{
			const CX2PartyManager::PartyMemberData* pPartyMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( i );
            ASSERT( pPartyMemberData != NULL );

			if(  g_pData->GetMyUser()->GetSelectUnit() != NULL
				&& pPartyMemberData->m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				continue;


			if( CX2Unit::CUS_FIELD_MAP != pPartyMemberData->m_eState ||
				pPartyMemberData->m_iStateCode != (int) g_pData->GetLocationManager()->GetCurrentVillageID() )
			{
				continue;
			}

			CX2SquareUnit* pPartySquareUnit = g_pTFieldGame->GetSquareUnitByUID( pPartyMemberData->m_iUnitUID );
			D3DXVECTOR3 vPos = pPartyMemberData->m_vPosition;
			if( NULL != pPartySquareUnit )
			{
				vPos = pPartySquareUnit->GetPos();
			}

			vProjectedPos = ProjectToScreenShot( vPos );
			vProjectedPos = CalcPosInWindowTexture( vProjectedPos );

			if( true == IsInWindowTexture( vProjectedPos ) )
			{
				vProjectedPos = CalcPosOnScreen( vProjectedPos );
                if ( m_pTextureIcon[MI_PARTY] != NULL )
				    m_pTextureIcon[MI_PARTY]->Draw( (int) vProjectedPos.x - MAGIC_SMALL_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_SMALL_ICON_SIZE_Y, MAGIC_SMALL_ICON_SIZE_X, MAGIC_SMALL_ICON_SIZE_Y );		
			}
			else
			{
				// 화살표로 파티원이 있는 방향을 표시해야 한다. 
				vProjectedPos = CalcPosOnScreen( vProjectedPos );

				D3DXVECTOR2 vMiniMapCenterPos = (m_vMiniMapWindowPos + D3DXVECTOR2( 824, 558 )) + m_vMiniMapWindowSize * 0.5f;
				D3DXVECTOR2 vDirToUnit = vProjectedPos - vMiniMapCenterPos;
				D3DXVec2Normalize( &vDirToUnit, &vDirToUnit );

				float fDistanceX = fabs( m_vMiniMapWindowSize.x * 0.5f / vDirToUnit.x );
				float fDistanceY = fabs( m_vMiniMapWindowSize.y * 0.5f / vDirToUnit.y );
				float fDistnaceMin = min( fDistanceX, fDistanceY );
				
				const float MAGIC_OFFSET = 10.f;
				vProjectedPos = vMiniMapCenterPos + vDirToUnit * (fDistnaceMin - MAGIC_OFFSET);

				float fRotateDegree = 0.f;
				if( vDirToUnit.x > 0.f  )
				{
					fRotateDegree = 180.f;
				}

                if ( m_pTextureIcon[MI_PARTY_ARROW] != NULL )
				    m_pTextureIcon[MI_PARTY_ARROW]->Draw( (int) vProjectedPos.x - MAGIC_SMALL_ICON_SIZE_X / 2, (int) vProjectedPos.y - MAGIC_SMALL_ICON_SIZE_Y, MAGIC_SMALL_ICON_SIZE_X, MAGIC_SMALL_ICON_SIZE_Y, 
					D3DCOLOR_RGBA(255, 255, 255, 255), fRotateDegree );		
			}
		}
	}
}


void CX2FieldMiniMap::SetVillageID( int iMapID )
{
	ResetCameraMatrix( iMapID );
	ResetWholeMapTexture( iMapID );
}



void CX2FieldMiniMap::ResetWholeMapTexture( int iMapID )
{
	SAFE_CLOSE( m_pTextureWholeMap );


	switch( iMapID )
	{
	case SEnum::VMI_RUBEN:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Ruben_Village.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0.f, 130.0f );			
		} break;

	case SEnum::VMI_ELDER:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Elder_Village.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0.f, 182.5f );
		} break;

	case SEnum::VMI_BESMA:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Besma_Village.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0.f, 182.5f );			

		} break;

	case SEnum::VMI_ALTERA:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Altera_Village.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 109 );			
		} break;
		
	case SEnum::VMI_VELDER:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Belder_Village.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0.f, 182.5f );		

			break;
		}
		//{{ JHKang / 강정훈 / 2011/01/04 / 하멜 마을 미니맵
#ifdef ADD_HAMEL_VILLAGE
	case SEnum::VMI_HAMEL:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Hamel_Village.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0.f, 146.f );		

			break;
		}
#endif ADD_HAMEL_VILLAGE
		//}} JHKang / 강정훈 / 2011/01/04 / 하멜 마을 미니맵

	case SEnum::VMI_DUNGEON_GATE_RUBEN:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Ruben_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 146 ); 
		} break;

	case SEnum::VMI_DUNGEON_GATE_ELDER:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Elder_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 146 );		
		} break;

	case SEnum::VMI_DUNGEON_GATE_BESMA:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Besma_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 146 );		
		} break;

	case SEnum::VMI_DUNGEON_GATE_ALTERA:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Altera_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 146 );			
		} break;

	case SEnum::VMI_PEITA:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Peita_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 146 );			
		} break;

	case SEnum::VMI_DUNGEON_GATE_VELDER:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Belder_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 182.5f );			
			break;
		}
		//{{ JHKang / 강정훈 / 2011/01/04 / 하멜 던전게이트 미니맵
#ifdef ADD_HAMEL_VILLAGE
	case SEnum::VMI_DUNGEON_GATE_HAMEL:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Hamel_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0.f, 182.5f );		

			break;
		}
#endif ADD_HAMEL_VILLAGE
		//}} JHKang / 강정훈 / 2011/01/04 / 하멜 던전게이트 미니맵

	case SEnum::VMI_BATTLE_FIELD_ELDER_REST_00:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Elder_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 146 );			
		} break;
		
	case SEnum::VMI_BATTLE_FIELD_BESMA_REST_00:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Peita_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 146 );			
		} break;

	case SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Elder_Gate.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0, 146 );			
		} break;
		
		
#ifdef VILLAGE_SANDER //임시
	case SEnum::VMI_SANDER:
		{
			m_pTextureWholeMap = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Minimap_Hamel_Village.tga" );
			m_vPosInWholeTexture = D3DXVECTOR2( 0.f, 146.f );		
		} break;
#endif //VILLAGE_SANDER
	}


	if( NULL != m_pTextureWholeMap )
	{
		m_vWholeTextureSize = D3DXVECTOR2( (float) m_pTextureWholeMap->GetWidth(), (float)m_pTextureWholeMap->GetHeight() );
	}
	else
	{
		m_vWholeTextureSize = D3DXVECTOR2( 512, 512 );
	}
}





void CX2FieldMiniMap::CalcWindowTexturePos( const D3DXVECTOR2& vProjectedMyPos )
{
	m_vWindowTexturePos = vProjectedMyPos - m_vWindowTextureSize * 0.5f;


	if( m_vWholeTextureSize.x <= m_vWindowTextureSize.x )
	{
		m_vWindowTexturePos.x = m_vPosInWholeTexture.x + ( m_vWholeTextureSize.x - m_vWindowTextureSize.x ) * 0.5f;
	}
	else
	{
		if( m_vWindowTexturePos.x < m_vPosInWholeTexture.x )
			m_vWindowTexturePos.x = m_vPosInWholeTexture.x;

		if( m_vWindowTexturePos.x > m_vPosInWholeTexture.x + m_vWholeTextureSize.x - m_vWindowTextureSize.x )
			m_vWindowTexturePos.x = m_vPosInWholeTexture.x + m_vWholeTextureSize.x - m_vWindowTextureSize.x;
	}


	if( m_vWholeTextureSize.y <= m_vWindowTextureSize.y )
	{
		m_vWindowTexturePos.y = m_vPosInWholeTexture.y + ( m_vWholeTextureSize.y - m_vWindowTextureSize.y ) * 0.5f;
	}
	else
	{
		if( m_vWindowTexturePos.y < m_vPosInWholeTexture.y )
			m_vWindowTexturePos.y = m_vPosInWholeTexture.y;

		if( m_vWindowTexturePos.y > m_vPosInWholeTexture.y + m_vWholeTextureSize.y - m_vWindowTextureSize.y )
			m_vWindowTexturePos.y = m_vPosInWholeTexture.y + m_vWholeTextureSize.y - m_vWindowTextureSize.y;
	}
}
