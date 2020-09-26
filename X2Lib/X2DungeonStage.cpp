#include "StdAfx.h"
#include ".\x2dungeonstage.h"
#include <set>


CX2DungeonStage::CX2DungeonStage( StageData* pStageData )
	: m_pStageData( pStageData )
	, m_pNowSubStage( NULL )
	, m_NowSubStageIndex( 0 )
	, m_pBeforeSubStage( NULL )
	, m_BeforeSubStageIndex( 0 )
	, m_fElapsedTime( 0.0f )
	, m_hParticleGo( INVALID_PARTICLE_SEQUENCE_HANDLE )
{
	LoadReadyNPCResources();
	//LoadReadyNPC();
}

CX2DungeonStage::~CX2DungeonStage(void)
{
	StopGoEffect();
	FlushNPCs();
	RemoveReadyNPCResources();

	SAFE_DELETE( m_pBeforeSubStage );
	SAFE_DELETE( m_pNowSubStage );
	//g_pX2Game->GetMajorParticle()->DestroyInstance( m_pParticleGo );

	CX2GageManager::GetInstance()->ClearBossGageUIList();

#ifdef ADD_GAME_STAGE_DELETE_DAMAGEEFFECT
	if( g_pX2Game != NULL )
	{
		g_pX2Game->RemoveDamageEffect();
	}
#endif
}

void CX2DungeonStage::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	m_fElapsedTime = fElapsedTime;

	if( m_pNowSubStage != NULL )
		m_pNowSubStage->OnFrameMove( fTime, fElapsedTime );


	// fix!!! 2009-05-27, dmlee, 기존코드 유지하면서 지하예배당 던전만 예외처리, 나중에 테스트 되면 beforestage->onframemove를 호출하도록 정리해야함
	CX2DungeonGame* pDungeonGame = NULL;
	if( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
		pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);

	if( NULL != pDungeonGame &&
		NULL != pDungeonGame->GetDungeon() &&
		NULL != pDungeonGame->GetDungeon()->GetDungeonData() )
	{
		switch( pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID )
		{
		case SEnum::DI_PEITA_CHAPEL_EXPERT:
		case SEnum::DI_BESMA_SECRET_COMMON:
		case SEnum::DI_BESMA_SECRET_HELL:

			{
				if( m_pBeforeSubStage != NULL )
					m_pBeforeSubStage->OnFrameMove( fTime, fElapsedTime );
			} break;

		default:
			{
				if( m_pBeforeSubStage != NULL )
					m_pBeforeSubStage->CurtainFrameMove( fTime, fElapsedTime );
			} break;
		}
	}
	else
	{
	if( m_pBeforeSubStage != NULL )
		m_pBeforeSubStage->CurtainFrameMove( fTime, fElapsedTime );
	}

}

// void CX2DungeonStage::LoadReadyNPC()
// {
// 	//Ready 리스트에 있는 것들을 로드하고
// 	for( int i = 0; i < (int)m_pStageData->m_ReadyNPCList.size(); ++i )
// 	{
// 		CX2UnitManager::NPC_UNIT_ID readyNPCID		= m_pStageData->m_ReadyNPCList[i];
// 		const CX2UnitManager::NPCUnitTemplet* pTemplet	= g_pData->GetUnitManager()->GetNPCUnitTemplet( readyNPCID );
// 		if( NULL != pTemplet )
// 		{
// 			CX2GUNPC* pCX2GUNPC = CX2GUNPC::CreateGUNPC( 0, CX2Room::TN_MONSTER, pTemplet->m_LuaFileName.c_str(), 0, pTemplet, (int) CX2NPCAI::NAT_NORMAL );
// 			pCX2GUNPC->Init();
// //{{ robobeg : 2008-10-21
// 			//pCX2GUNPC->SetShow( false );
//             pCX2GUNPC->SetShowObject( false );
// //}} robobeg : 2008-10-21
// 			m_ReadyNPCList.push_back( pCX2GUNPC );
// 		}
// 	}
// 
// 	//서브 스테이지를 직접 검색하여 종류가 다른 놈들을 로드한다
// 	for( int i = 0; i < (int)m_pStageData->m_SubStageDataList.size(); i++ )
// 	{
// 		CX2DungeonSubStage::SubStageData* pSubStageData	= m_pStageData->m_SubStageDataList[i];
// 		
// 		for( int j = 0; j < (int)pSubStageData->m_NPCDataList.size(); j++ )
// 		{
// 			CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
// 			
// 			bool bNeedLoad = true;
// 			for( int k = 0; k < (int)m_pStageData->m_ReadyNPCList.size(); k++ )
// 			{
// 				CX2UnitManager::NPC_UNIT_ID readyNPCID	= m_pStageData->m_ReadyNPCList[k];
// 				if( pNPCData->m_UnitID == readyNPCID )
// 				{
// 					bNeedLoad = false;
// 					break;
// 				}
// 			}
// 			if( bNeedLoad == true )
// 			{
// 				const CX2UnitManager::NPCUnitTemplet* pTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( pNPCData->m_UnitID );
// 				if( NULL != pTemplet )
// 				{
// 					CX2GUNPC* pCX2GUNPC = CX2GUNPC::CreateGUNPC( 0, 2, pTemplet->m_LuaFileName.c_str(), 0, pTemplet, (int) CX2NPCAI::NAT_NORMAL );
// 					pCX2GUNPC->Init();
// //{{ robobeg : 2008-10-21
// 					//pCX2GUNPC->SetShow( false );
//                     pCX2GUNPC->SetShowObject( false );
// //}} robobeg : 2008-10-21
// 					m_ReadyNPCList.push_back( pCX2GUNPC );
// 				}
// 			}
// 		}
// 	}
// 
// 	//
// 	for( int i = 0; i < (int)m_pStageData->m_SubStageDataList.size(); i++ )
// 	{
// 		CX2DungeonSubStage::SubStageData* pSubStageData	= m_pStageData->m_SubStageDataList[i];
// 		pSubStageData->CreateNPC();
// 	}
// }

void CX2DungeonStage::LoadReadyNPCResources()
{
	if ( m_pStageData == NULL )
		return;

	if ( g_pX2Game == NULL )
		return;

	std::set<CX2UnitManager::NPC_UNIT_ID>   setUsed;

	//서브 스테이지를 직접 검색하여 종류가 다른 놈들을 로드한다
	for( UINT i = 0; i < m_pStageData->m_SubStageDataList.size(); i++ )
	{
#ifdef X2TOOL
		CX2DungeonSubStage::SubStageData* pSubStageData	= m_pStageData->m_SubStageDataList[i];
		
 		pSubStageData->m_NPCDataList.clear();
		if( g_pInstanceData->m_bCanDungeonPlay == true )
		{
 			pSubStageData->m_NPCDataList = pSubStageData->SummonNpcData4Tool(); // pSubStageData->m_vecSubStageNpcData;
		}
#else
		const CX2DungeonSubStage::SubStageData* pSubStageData	= m_pStageData->m_SubStageDataList[i];
#endif

		BOOST_TEST_FOREACH( const CX2DungeonSubStage::NPCData*, pNpcData, pSubStageData->m_NPCDataList )
		{
			if ( setUsed.insert( pNpcData->m_UnitID ).second == true )
			{
				g_pX2Game->AddNPCResourcesToDeviceList( pNpcData->m_UnitID );
			}//if
		}
	}

	//Ready 리스트에 있는 것들을 로드하고
	BOOST_TEST_FOREACH( CX2UnitManager::NPC_UNIT_ID, readyNPCID, m_pStageData->m_ReadyNPCList )
	{
		if ( setUsed.insert( readyNPCID ).second == true )
		{
			g_pX2Game->AddNPCResourcesToDeviceList( readyNPCID );
		}//if
	}
}

void    CX2DungeonStage::RemoveReadyNPCResources()
{
	if ( g_pX2Game != NULL )
		g_pX2Game->RemoveAllNPCResourcesFromUnitLoader();
}//CX2DungeonStage::RemoveReadyNPCResources()

void    CX2DungeonStage::FlushNPCs()
{
	if ( m_pStageData == NULL )
		return;

	unsigned    uSize = m_vecSubStageGUNPC.size();


	for( unsigned u = 0; u != uSize; u++ )
	{
		CX2GUNPC* pCX2GUNPC = m_vecSubStageGUNPC[ u ];
		SAFE_DELETE_KTDGOBJECT( pCX2GUNPC );
		m_vecSubStageGUNPC[ u ] = NULL;
	}//for

	if ( m_pBeforeSubStage != NULL )
	{
		DeleteSubStageNPCUnit( m_BeforeSubStageIndex );
	}//if
	if ( m_pNowSubStage != NULL )
	{
		DeleteSubStageNPCUnit( m_NowSubStageIndex );
	}//if

	m_vecSubStageGUNPC.resize( 0 );

	if ( g_pX2Game != NULL )
	{
		g_pX2Game->RemoveAllNPCResourcesFromUnitLoader();
	}//if

}//CX2DungeonStage::FlushNPCs()

void    CX2DungeonStage::CreateStageNPCs()
{
	ASSERT( g_pX2Game != NULL && m_pStageData != NULL );
	if ( g_pX2Game == NULL || m_pStageData == NULL )
		return;

	ASSERT( m_vecSubStageGUNPC.empty() == true );
	m_vecSubStageGUNPC.resize( 0 );
	
	if ( false == m_pStageData->m_SubStageDataList.empty() )
	{
		const CX2DungeonSubStage::SubStageData* pSubStageData	= m_pStageData->m_SubStageDataList[0];
		
		CX2DungeonSubStage::CreateAndAppendGUNPC( m_vecSubStageGUNPC, pSubStageData->m_NPCDataList );
	}
}//

CX2DungeonSubStage* CX2DungeonStage::CreateSubStage( int subStageIndex )
{
	if( m_pStageData->m_SubStageDataList.size() <= 0 || subStageIndex >= (int)m_pStageData->m_SubStageDataList.size() )
		return NULL;

	CX2DungeonSubStage::SubStageData* pSubStageData = m_pStageData->m_SubStageDataList[subStageIndex];
	if( pSubStageData == NULL )
		return NULL;

	if( m_pNowSubStage != NULL )
	{
#ifdef X2TOOL
		StopGoEffect();
#endif
		StartGoEffect( m_pNowSubStage->GetSubStageData()->m_GoType );
	}

	if ( m_pBeforeSubStage != NULL )
	{
		DeleteSubStageNPCUnit( m_BeforeSubStageIndex );
		SAFE_DELETE( m_pBeforeSubStage );
	}//if

	m_pBeforeSubStage		= m_pNowSubStage;
	m_BeforeSubStageIndex	= m_NowSubStageIndex;

	m_NowSubStageIndex		= subStageIndex;
	m_pNowSubStage			= new CX2DungeonSubStage( subStageIndex, pSubStageData );

	return m_pNowSubStage;
}

void CX2DungeonStage::CloseBeforeSubStage()
{
	if( m_pBeforeSubStage != NULL )
	{
		m_pBeforeSubStage->ToggleMainLineMapSet( false );
		m_pBeforeSubStage->ToggleStartLineMapSet( false );
	}

	if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
	{	//스테이지 이동 시, 스테이지 이동 가이드 끄기		
		g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_GO_NEXT_STAGE, false);
	}
}

void CX2DungeonStage::DeleteBeforeSubStage()
{
	if( m_pBeforeSubStage != NULL )
	{
		m_pBeforeSubStage->ToggleMainLineMapSet( false );
		m_pBeforeSubStage->ToggleStartLineMapSet( false );
		m_pBeforeSubStage->ToggleEndLineMapSet( false );
		m_pBeforeSubStage->BreakCurtain();
	}

	DeleteSubStageNPCUnit( m_BeforeSubStageIndex );
	SAFE_DELETE( m_pBeforeSubStage );
}


void CX2DungeonStage::StartGoEffect( CX2DungeonSubStage::GO_TYPE goType )
{
	//if( NULL != g_pData->GetSlideShot() &&
	//	true == g_pData->GetSlideShot()->IsPresentNow() )
	//{
	//	return;
	//}

	if ( goType == CX2DungeonSubStage::GT_RIGHT )
	{
		m_hParticleGo		= g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"SubStageGoRight", 850.0f, 300.0f, 0.0f, 1, 1 );
	}
	else if ( goType == CX2DungeonSubStage::GT_LEFT )
	{
		m_hParticleGo		= g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"SubStageGoLeft", 180.0f, 300.0f, 0.0f, 1, 1 );
	}
	else if ( goType == CX2DungeonSubStage::GT_UP )
	{
		m_hParticleGo		= g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"SubStageGoUp", 500.0f, 200.0f, 0.0f, 1, 1 );
	}
	else if ( goType == CX2DungeonSubStage::GT_DOWN )
	{
		m_hParticleGo		= g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"SubStageGoDown", 500.0f, 600.0f, 0.0f, 1, 1 );
	}

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Go.ogg", true, false );
}

void CX2DungeonStage::StopGoEffect()
{
	g_pKTDXApp->GetDeviceManager()->StopSound( L"Go.ogg" );

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hParticleGo );
}

bool CX2DungeonStage::IsGoEffectOn()
{
	if( true == g_pX2Game->GetMajorParticle()->IsLiveInstanceHandle( m_hParticleGo ) )
		return true;
	else 
		return false;
}

// CX2World* CX2DungeonStage::FlushWorld()
// {
// //#ifdef DUNGEON_WORLD_THREAD_LOADING
// //	CX2World* pCX2World = /*m_pStageData->m_pWorld;*/m_pStageData->GetWorld();
// //	m_pStageData->SetWorld( NULL );
// ////	m_pStageData->m_pWorld = NULL;
// //#else
// 
// 	CX2World* pCX2World = m_pStageData->m_pWorld;
// 	m_pStageData->m_pWorld = NULL;
// 
// //#endif //DUNGEON_WORLD_THREAD_LOADING
// 
// 	if( pCX2World != NULL )
// 		pCX2World->SetShowObject( true );
// 	return pCX2World;
// }

void    CX2DungeonStage::DeleteSubStageNPCUnit( int iSubStageIndex )
{
	if ( iSubStageIndex < 0 || NULL == m_pStageData )
		return;

	if ( iSubStageIndex < static_cast<int>( m_pStageData->m_SubStageDataList.size() ) )
	{
		CX2DungeonSubStage::DeleteAllNPCUnit( m_pStageData->m_SubStageDataList[iSubStageIndex]->m_NPCDataList );
	}//if

}//CX2DungeonStage::DeleteSubStageNPCUnit()


//////////////////////////////////////////////////////////////////////////////////////////

bool CX2DungeonStage::StageData::LoadData( KLuaManager& luaManager, bool bIsNpcLoad )
{
	LUA_GET_VALUE( luaManager, "SECRET_STAGE", m_bSecretStage, false );
	
	LUA_GET_VALUE( luaManager, "START_MOTION", m_bStartMotion, false );
	LUA_GET_VALUE_ENUM( luaManager, "WORLD_ID", m_WorldID, CX2World::WORLD_ID, CX2World::WI_NONE );
#ifdef X2TOOL
	LUA_GET_VALUE( luaManager, "WORLD_DATA_NAME", m_wstrWorldDataName, L"" );
#endif
	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
	LUA_GET_VALUE_ENUM( luaManager, "STAGE_TYPE", m_eStageType, CX2DungeonStage::STAGE_TYPE, CX2DungeonStage::ST_NONE );
	//}}

	LUA_GET_VALUE( luaManager, "MISSION_TITLE", m_MissionTitle, L"" );

	m_UnitColor = D3DXCOLOR(-1, -1, -1, 1);
	m_WorldColor = D3DXCOLOR(-1, -1, -1, 1);
	if( luaManager.BeginTable( "WORLD_COLOR" ) == true )
	{
		float fValue = -1.f;
		int index = 1; 
		while( luaManager.GetValue( index, fValue ) == true )
		{
			switch( index )
			{
			case 1:
				m_WorldColor.r = fValue/255.f;
				break;
			case 2:
				m_WorldColor.g = fValue/255.f;
				break;
			case 3:
				m_WorldColor.b = fValue/255.f;
				break;
			default:
				break;
			}
			index++;
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "UNIT_COLOR" ) == true )
	{
		float fValue = -1.f;
		int index = 1; 
		while( luaManager.GetValue( index, fValue ) == true )
		{
			switch( index )
			{
			case 1:
				m_UnitColor.r = fValue/255.f;
				break;
			case 2:
				m_UnitColor.g = fValue/255.f;
				break;
			case 3:
				m_UnitColor.b = fValue/255.f;
				break;
			default:
				break;
			}
			index++;
		}
		luaManager.EndTable();
	}

	//m_pWorld = g_pData->GetWorldManager()->CreateWorld( m_WorldID );
	//m_pWorld->SetShowObject( false );
	
//#ifdef DUNGEON_WORLD_THREAD_LOADING
//	::EnterCriticalSection( &m_csStageData );
//	
//	__super::EndThread();
//
//	::LeaveCriticalSection( &m_csStageData );
//
//	__super::BeginThread( NULL, NULL );
//
//#endif //DUNGEON_WORLD_THREAD_LOADING

	if( luaManager.BeginTable( "READY_TEXTURE" ) == true )
	{
		wstring texName;
		int index = 1; 
		while( luaManager.GetValue( index, texName ) == true )
		{
			g_pX2Game->TextureReady( texName.c_str() );
			index++;
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "READY_XMESH" ) == true )
	{
		wstring xmeshName;
		int index = 1; 
		while( luaManager.GetValue( index, xmeshName ) == true )
		{
			g_pX2Game->XMeshReady( xmeshName.c_str() );
			index++;
		}
		luaManager.EndTable();
	}


	if( luaManager.BeginTable( "READY_XSKIN_MESH" ) == true )
	{
		wstring xskinmeshName;
		int index = 1; 
		while( luaManager.GetValue( index, xskinmeshName ) == true )
		{
			g_pX2Game->XSkinMeshReady( xskinmeshName.c_str() );
			index++;
		}
		luaManager.EndTable();
	}



	if( luaManager.BeginTable( "READY_SOUND" ) == true )
	{
		wstring soundName;
		int index = 1; 
		while( luaManager.GetValue( index, soundName ) == true )
		{
			g_pX2Game->SoundReady( soundName.c_str() );
			index++;
		}
		luaManager.EndTable();
	}

	//스테이지에서 예약할 NPC 정보를 읽어둔다
	LoadReadyNPC( luaManager );

	//서브 스테이지 정보를 읽어온다
	LoadSubStageData( luaManager, bIsNpcLoad );

	return true;
}




void CX2DungeonStage::StageData::LoadReadyNPC( KLuaManager& luaManager )
{
	if( luaManager.BeginTable( "READY_NPC" ) == true )
	{
		int index = 1; 
		int nNPCUnitID;
		while( luaManager.GetValue( index, nNPCUnitID ) == true )
		{
			m_ReadyNPCList.push_back( (CX2UnitManager::NPC_UNIT_ID)nNPCUnitID );
			index++;
		}
		luaManager.EndTable();
	}
}

void CX2DungeonStage::StageData::LoadSubStageData( KLuaManager& luaManager, bool bIsNpcLoad )
{
	int subStageIndex = 0;
	while( luaManager.BeginTable( "SUB_STAGE", subStageIndex ) == true )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = new CX2DungeonSubStage::SubStageData();
		pSubStageData->LoadData( luaManager, bIsNpcLoad );

		subStageIndex++;
		m_SubStageDataList.push_back( pSubStageData );
		luaManager.EndTable();
	}
}





