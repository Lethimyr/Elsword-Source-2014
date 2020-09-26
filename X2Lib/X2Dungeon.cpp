#include "StdAfx.h"
#include ".\x2dungeon.h"

CX2Dungeon::CX2Dungeon( const DungeonData* pDungeonData )
{
	m_pDungeonData			= NULL;
	m_pNowStage				= NULL;

	m_pDungeonData			= pDungeonData;
	m_NowStageIndex			= -1;

	m_bIsNpcLoad			= false;

	m_fElapsedTime			= 0.f;
	
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	m_iBeforeSubStageIndexUsingPacket = 0;			// 패킷 전송에 사용하는 SubStage Index 를 저장하는 곳을 초기화 한다.
	m_iBeforeStageIndexUsingPacket = 0;				// 패킷 전송에 사용하는 Stage Index 를 저장하는 곳을 초기화 한다.
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

	if( NULL == pDungeonData )
		return;
	

	// 던전 로드 
	if ( OpenScriptFile( m_pDungeonData->m_DataFileName.c_str() ) == false )
	{
		MessageBox( g_pKTDXApp->GetHWND(), m_pDungeonData->m_DataFileName.c_str(), GET_STRING( STR_ID_133 ), MB_OK );
	}
}

//{{ 2007. 9. 14  최육사  NPC 스크립트 정보 받기 여부를 설정할 수 있는 생성자
CX2Dungeon::CX2Dungeon( const DungeonData* pDungeonData, bool bIsNpcLoad )
{
	m_pDungeonData			= NULL;
	m_pNowStage				= NULL;

	m_pDungeonData			= pDungeonData;
	m_NowStageIndex			= -1;

	m_bIsNpcLoad			= bIsNpcLoad;

	m_fElapsedTime			= 0.f;

	if( NULL == pDungeonData )
		return;

	// 던전 로드 
	OpenScriptFile( m_pDungeonData->m_DataFileName.c_str() );
}
//}}

CX2Dungeon::~CX2Dungeon(void)
{
	m_pDungeonData = NULL; // delete하면 안됨
	SAFE_DELETE( m_pNowStage );

	for( int i = 0; i < (int)m_StageDataList.size(); i++ )
	{
		CX2DungeonStage::StageData* pStageData = m_StageDataList[i];
		SAFE_DELETE( pStageData );
	}
	m_StageDataList.clear();
}

void CX2Dungeon::OnFrameMove( double fTime, float fElapsedTime )
{	
	KTDXPROFILE();

	m_fElapsedTime = fElapsedTime;

	if( m_pNowStage != NULL )
	{
		m_pNowStage->OnFrameMove( fTime, fElapsedTime );
	}
}

void    CX2Dungeon::FlushNPCs()
{
	if ( m_pNowStage == NULL )
		return;

	m_pNowStage->FlushNPCs();
}

void    CX2Dungeon::CreateStageNPCs()
{
	if ( m_pNowStage == NULL )
		return;

	m_pNowStage->CreateStageNPCs();
}//CX2Dungeon::CreateStageNPCs()

CX2DungeonStage* CX2Dungeon::CreateStage( int stageNum )
{
	if( m_StageDataList.size() <= 0 || stageNum >= (int)m_StageDataList.size() )
		return NULL;

	CX2DungeonStage::StageData* pStageData = m_StageDataList[stageNum];
	if( pStageData == NULL )
		return NULL;

	SAFE_DELETE( m_pNowStage );
	m_NowStageIndex		= stageNum;
	m_pNowStage			= new CX2DungeonStage( pStageData );

	return m_pNowStage;
}

// 스테이지 정보를 읽어와서 m_DungeonStageList에 채운다
bool CX2Dungeon::OpenScriptFile( const WCHAR* pFileName ) 
{
//{{ robobeg : 2008-10-28
	//KLuaManager luaManager;
    KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if( false == g_pKTDXApp->LoadLuaTinker( pFileName ) )
		return false;
	//if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, L"Enum.lua" ) )
		//return false;
	if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, pFileName ) )
		return false;
//}} robobeg : 2008-10-28

	if( luaManager.BeginTable( "STAGE_LIST" ) == true )
	{
		int stageIndex = 0;
		while( luaManager.BeginTable( "STAGE", stageIndex ) == true )
		{
			CX2DungeonStage::StageData* pStageData = new CX2DungeonStage::StageData();
//#ifdef DUNGEON_WORLD_THREAD_LOADING
//			pStageData->m_StageIndex = stageIndex;
//#endif
			pStageData->LoadData( luaManager, m_bIsNpcLoad );

			stageIndex++;
			m_StageDataList.push_back( pStageData );
			luaManager.EndTable(); // Stage(i)	
		}

		m_mapStageLine.clear();
		m_mapDungeonMapData.clear();
		if( luaManager.BeginTable( "DUNGEON_MAP" ) == true )
		{
			
			int iStageIndex = 0;
			while( luaManager.BeginTable( "STAGE", iStageIndex ) == true )
			{
				int iSubStageIndex = 0;
				while( luaManager.BeginTable( "SUB_STAGE", iSubStageIndex ) == true )
				{
					DungeonMapData dungeonMapData;
					LUA_GET_VALUE( luaManager, 1, dungeonMapData.m_vPos.x, 				0.f );
					LUA_GET_VALUE( luaManager, 2, dungeonMapData.m_vPos.y, 				0.f );
					LUA_GET_VALUE( luaManager, 3, dungeonMapData.m_bIsBossStage,		false );
					LUA_GET_VALUE( luaManager, 4, dungeonMapData.m_bIsNextStageRandom,	false );
#ifdef NEW_HENIR_TEST
					LUA_GET_VALUE( luaManager, 5, dungeonMapData.m_bRestRoomState,		false );
#endif NEW_HENIR_TEST

//{{ 오현빈 // 2012-07-19 // 모든 스크립트에서 X좌표를 수정해야 되기 때문에 파싱후에 증감연사 추가
					dungeonMapData.m_vPos.x -= 40.f;
//{{ 오현빈 // 2012-07-19 // 모든 스크립트에서 X좌표를 수정해야 되기 때문에 파싱후에 증감연사 추가
					m_mapDungeonMapData[ std::make_pair(iStageIndex, iSubStageIndex) ] = dungeonMapData;

					iSubStageIndex++;
					luaManager.EndTable(); // SUB_STAGE(i)	
				}

				iStageIndex++;
				luaManager.EndTable(); // STAGE(i)	
			}

			luaManager.EndTable(); // DUNGEON_MAP
		}



		m_UnitColor = D3DXCOLOR(-1, -1, -1, 1);
		m_WorldColor = D3DXCOLOR(-1, -1, -1, 1);
		if( luaManager.BeginTable( "DUNGEON_WORLD_COLOR" ) == true )
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

		if( luaManager.BeginTable( "DUNGEON_UNIT_COLOR" ) == true )
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

		luaManager.EndTable(); // StageList
	}






	// 스크립트에서 레벨 정보가 없는 npc들은 던전 최소레벨에 맞춘다
	if( NULL != m_pDungeonData )
	{
		for( UINT i=0; i<m_StageDataList.size(); i++ )
		{
			CX2DungeonStage::StageData* pStageData = m_StageDataList[i];
			for( UINT j=0; j<pStageData->m_SubStageDataList.size(); j++ )
			{
				CX2DungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[j];
#ifdef X2TOOL
				for( UINT k=0; k<pSubStageData->m_vecSubStageNpcData.size(); ++k )
				{
					CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_vecSubStageNpcData[k];
					if( pNPCData != NULL && pNPCData->m_iLevel4Tool <= 0 )
					{						
						if( GetDungeonData()->m_iNpcLevel > 0 )
							pNPCData->m_Level = GetDungeonData()->m_iNpcLevel;
						else
							pNPCData->m_Level = m_pDungeonData->m_MinLevel;
					}
					else
					{
						pNPCData->m_Level = pNPCData->m_iLevel4Tool;
					}
				}

#endif
				for( UINT k=0; k<pSubStageData->m_NPCDataList.size(); k++ )
				{
					CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[k];
					if( pNPCData->m_Level <= 0 )
					{
						pNPCData->m_Level = m_pDungeonData->m_MinLevel;
					}
				}
			}
		}
	}

	return true;
}

void CX2Dungeon::SetStageStaticNPC( int stageNum, std::map<int, KNPCList>& mapNPCData )
{
	if ( stageNum < 0 || stageNum >= (int)m_StageDataList.size() )
		return;

	CX2DungeonStage::StageData* pStageData = m_StageDataList[stageNum];

	int UIDIndex = 0;
	for( int i = 0; i < (int)pStageData->m_SubStageDataList.size(); i++ )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[i];

		std::map<int, KNPCList>::iterator iter = mapNPCData.find( i );
		if( iter != mapNPCData.end() )
		{
			KNPCList* pKNPCList = &iter->second;
			for( int j = 0; j < (int)pKNPCList->m_NPCList.size(); j++ )
			{
				KNPCUnitReq* pKNPCUnitReq = &pKNPCList->m_NPCList[j];
				pSubStageData->AddNPCData( *pKNPCUnitReq );
			}
		}		
	}
}

void CX2Dungeon::SetDungeonMapOpen( int iStageIndex, int iSubStageIndex, bool bOpen )
{
#ifdef NEW_HENIR_TEST
	D3DXVECTOR2 m_vOpenStagePos;
#endif NEW_HENIR_TEST

	map< std::pair<int,int>, DungeonMapData >::iterator it;
	it = m_mapDungeonMapData.find( std::make_pair(iStageIndex, iSubStageIndex) );
	if( it != m_mapDungeonMapData.end() )
	{
		DungeonMapData& dungeonMapData = it->second;
		dungeonMapData.m_bOpen = bOpen;
#ifdef NEW_HENIR_TEST
		m_vOpenStagePos = dungeonMapData.m_vPos;
#endif NEW_HENIR_TEST
	}

#ifdef NEW_HENIR_TEST
	for(it = m_mapDungeonMapData.begin(); it != m_mapDungeonMapData.end(); it++)
	{
		DungeonMapData& dungeonMapData = it->second;
		if(dungeonMapData.m_vPos == m_vOpenStagePos)
			dungeonMapData.m_bAutoOpen = true;
	}
#endif NEW_HENIR_TEST

	FullStageIndex nowStageIndex( iStageIndex, iSubStageIndex );
	m_mapStageLine[ std::make_pair(m_PrevStageIndex, nowStageIndex) ] = true;

	m_PrevStageIndex = nowStageIndex;
}

bool CX2Dungeon::GetDungeonMapOpen( int iStageIndex, int iSubStageIndex )
{
	map< std::pair<int,int>, DungeonMapData >::iterator it;
	it = m_mapDungeonMapData.find( std::make_pair(iStageIndex, iSubStageIndex) );
	if( it != m_mapDungeonMapData.end() )
	{
		DungeonMapData& dungeonMapData = it->second;
		return dungeonMapData.m_bOpen;
	}

	return false;
}

bool CX2Dungeon::IsStageExist( int iStageIndex, int iSubStageIndex )
{
	if( iStageIndex < 0 || iStageIndex > (int)m_StageDataList.size()-1 )
		return false;

	CX2DungeonStage::StageData* pStageData = m_StageDataList[iStageIndex];
	if( iSubStageIndex < 0 || iSubStageIndex > (int)pStageData->m_SubStageDataList.size()-1 )
		return false;

	return true;
}

bool CX2Dungeon::GetStageLineOpen( int iPrevStageIndex, int iPrevSubStageIndex, int iStageIndex, int iSubStageIndex )
{
	FullStageIndex prevStageIndex( iPrevStageIndex, iPrevSubStageIndex );
	FullStageIndex nowStageIndex( iStageIndex, iSubStageIndex );

	map< std::pair< FullStageIndex, FullStageIndex>, bool >::iterator it;
	it = m_mapStageLine.find( std::make_pair( prevStageIndex, nowStageIndex ) );
	if( it != m_mapStageLine.end() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
bool CX2Dungeon::IsFixedMembers( const SEnum::DUNGEON_ID eDungeonID_ )
{
	return ( g_pData->GetDungeonManager()->GetDungeonData( eDungeonID_ )->m_usFixedMembers > 0 );
}
short CX2Dungeon::GetFixedMembers( const SEnum::DUNGEON_ID eDungeonID_ )
{
	return g_pData->GetDungeonManager()->GetDungeonData( eDungeonID_ )->m_usFixedMembers;
}
#endif SERV_DUNGEON_OPTION_IN_LUA

bool CX2Dungeon::IsEventDungeon( const SEnum::DUNGEON_ID eDungeonId_ )
{
#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( g_pData->GetDungeonManager()->GetDungeonData( eDungeonId_ )->m_bEventDungeon == true )
		return true;
#endif SERV_DUNGEON_OPTION_IN_LUA

	switch( eDungeonId_ )
	{
	case SEnum::DI_EVENT_KIDDAY_RUBEN:
	case SEnum::DI_EVENT_KIDDAY_ELDER:
	case SEnum::DI_EVENT_KIDDAY_BESMA:
	case SEnum::DI_EVENT_KIDDAY_ALTERA:

	case SEnum::DI_EVENT_TREE_DAY_ELDER:		// 식목일 이벤트 던전
	case SEnum::DI_EVENT_TREE_DAY_BESMA:
	case SEnum::DI_EVENT_TREE_DAY_ALTERA:
	case SEnum::DI_EVENT_TREE_DAY_PEITA:
	case SEnum::DI_EVENT_TREE_DAY_VELDER:
	case SEnum::DI_EVENT_TREE_DAY_HAMEL:

		//{{ 2010. 10. 19	최육사	비밀던전 이벤트 업데이트
#ifdef SERV_SECRET_DUNGEON_EVENT
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
	case SEnum::DI_ELDER_HALLOWEEN_HARD:
	case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
#endif SERV_SECRET_DUNGEON_EVENT
		//}}
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		CASE_DEFENCE_DUNGEON
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 2011. 04. 13  김민성  글로벌 서버 추가
	case SEnum::DI_EVENT_VALENTINE_DAY:
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
	case SEnum::DI_EVENT_HALLOWEEN_DAY:
#endif //SERV_HALLOWEEN_EVENT_2013

		return true;
	}

	return false;
}

bool CX2Dungeon::IsHenirDungeon( const SEnum::DUNGEON_ID eDungeonId_ )
{
	switch( eDungeonId_ )
	{
	case SEnum::DI_ELDER_HENIR_SPACE:
		return true;
		break;
	}

	return false;
}

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
bool CX2Dungeon::IsEventValentineDungeon( const SEnum::DUNGEON_ID eDungeonId_ )
{
	switch( eDungeonId_ )
	{
	case SEnum::DI_EVENT_VALENTINE_DUNGEON_INT:
		return true;
		break;
	}

	return false;
}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
bool CX2Dungeon::IsSecretDungeon( const SEnum::DUNGEON_ID eDungeonId_ )
{
	switch( eDungeonId_ )
	{
	case SEnum::DI_EL_FOREST_HELL_NORMAL:
	case SEnum::DI_EL_FOREST_HELL_HARD:
	case SEnum::DI_EL_FOREST_HELL_EXPERT:
	case SEnum::DI_ELDER_WALLY_CASTLE_LAB_NORMAL:
	case SEnum::DI_ELDER_WALLY_CASTLE_LAB_HARD:
	case SEnum::DI_ELDER_WALLY_CASTLE_LAB_EXPERT:
	case SEnum::DI_BESMA_SECRET_NORMAL:
	case SEnum::DI_BESMA_SECRET_HARD:
	case SEnum::DI_BESMA_SECRET_EXPERT:
	case SEnum::DI_ALTERA_SECRET_NORMAL:
	case SEnum::DI_ALTERA_SECRET_HARD:
	case SEnum::DI_ALTERA_SECRET_EXPERT:
		//{{ 2010. 04. 05  최육사	비밀던전 헬모드
	case SEnum::DI_RUBEN_SECRET_COMMON:
	case SEnum::DI_RUBEN_SECRET_HELL:
	case SEnum::DI_ELDER_SECRET_COMMON:
	case SEnum::DI_ELDER_SECRET_HELL:
	case SEnum::DI_BESMA_SECRET_COMMON:
	case SEnum::DI_BESMA_SECRET_HELL:
	case SEnum::DI_ALTERA_SECRET_COMMON:
	case SEnum::DI_ALTERA_SECRET_HELL:
		//}}
		//{{ JHKang / 강정훈 / 2011.8.24
	case SEnum::DI_VELDER_SECRET_COMMON:
	case SEnum::DI_VELDER_SECRET_HELL:
		//}} 
	case SEnum::DI_HAMEL_SECRET_COMMON:
	case SEnum::DI_HAMEL_SECRET_HELL:
		return true;
	}

	return false;
}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

bool CX2Dungeon::IsDamageFreeGame()
{
	if ( NULL == g_pX2Game )
		return false;

	switch ( g_pX2Game->GetGameType() )
	{
	case CX2Game::GT_DUNGEON:
		{
			CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);

			if( NULL != pDungeonGame &&
				NULL != pDungeonGame->GetDungeon() &&
				NULL != pDungeonGame->GetDungeon()->GetDungeonData() )
			{
				const SEnum::DUNGEON_ID eDungeonId = pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID;

				if ( CX2Dungeon::IsEventDungeon( eDungeonId ) 
	#ifndef NEW_HENIR_DUNGEON // 1레벨 보정 -> 연동 레벨 던전으로 변경
					|| 	( CX2Dungeon::IsHenirDungeon( eDungeonId ) &&
						NULL != g_pData->GetPartyManager() && NULL != g_pData->GetPartyManager()->GetMyPartyData() &&
						  CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode ) 				  
	#endif // NEW_HENIR_DUNGEON
					)
					return true;
			}
		} break;
	case CX2Game::GT_PVP:
		{
			switch ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
			{
			case KPVPChannelInfo::PCC_TOURNAMENT:
			case KPVPChannelInfo::PCC_FREE:
				return true;
			}
		} break;

	default:
		break;
	}

	return false;
}

//#endif

const WCHAR* CX2Dungeon::GetDungeonDifficultyString( const DUNGEON_TYPE eDungeonType_, const char cDungeonMode_, const DIFFICULTY_LEVEL eDufficulty_ )
{
	switch( eDungeonType_ )
	{
	default:
	case CX2Dungeon::DT_NORMAL:
		{
			switch( eDufficulty_ )
			{
			default:
			case CX2Dungeon::DL_NORMAL:
				return GET_STRING( STR_ID_385 );
				break;
			case CX2Dungeon::DL_HARD:
				return GET_STRING( STR_ID_386 );
				break;
			case CX2Dungeon::DL_EXPERT:
				return GET_STRING( STR_ID_388 );
				break;
			}

		} break;

	case CX2Dungeon::DT_HENIR:
		{
			switch( cDungeonMode_ )
			{
			default:
			case CX2Dungeon::DM_HENIR_PRACTICE:
				return GET_STRING( STR_ID_938 );
				break;

			case CX2Dungeon::DM_HENIR_CHALLENGE:
				return GET_STRING( STR_ID_3747 );
				break;
			}

		} break;

		//{{ kimhc // 2010.3.22 //	비밀 던전 개편
#ifdef	SERV_SECRET_HELL
		// 파티 모집 시 파티 장의 머리위에 뜨는 푯말
	case CX2Dungeon::DT_SECRET:
		{
			switch ( cDungeonMode_ )
			{
			default:
			case CX2Dungeon::DM_SECRET_NORMAL:
				return GET_STRING( STR_ID_938 );
				break;

			case CX2Dungeon::DM_SECRET_HELL:
				return GET_STRING( STR_ID_4943 );
				break;

			} break;

		}
#endif	SERV_SECRET_HELL
		//}} kimhc // 2010.3.22 //	비밀 던전 개편
	}
		
}