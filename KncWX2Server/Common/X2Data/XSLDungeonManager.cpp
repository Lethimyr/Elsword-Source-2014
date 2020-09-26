#include "XSLDungeonManager.h"

ImplementRefreshSingleton( CXSLDungeonManager );

CXSLDungeonManager::CXSLDungeonManager()
{
}

CXSLDungeonManager::~CXSLDungeonManager()
{
}

ImplToStringW( CXSLDungeonManager )
{
	stm_	<< L"----------[ Dungeon Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapDungeon.size() )
			<< TOSTRINGW( m_mapDungeonData.size() )
			<< TOSTRINGW( m_iSecretDungeonExtraStageNpcLevel )
			<< TOSTRINGW( m_mapExtraStageConditionFactor.size() )
			;

	return stm_;
}

ImplementLuaScriptParser( CXSLDungeonManager )
{
	lua_tinker::class_add<CXSLDungeonManager>( GetLuaState(), "CXSLDungeonManager" );

	lua_tinker::class_def<CXSLDungeonManager>( GetLuaState(), "AddDungeonData",						&CXSLDungeonManager::AddDungeonData_LUA );
	lua_tinker::class_def<CXSLDungeonManager>( GetLuaState(), "AddDefaultRoomTitle_LUA",			&CXSLDungeonManager::AddDefaultRoomTitle_LUA );
	lua_tinker::class_def<CXSLDungeonManager>( GetLuaState(), "OpenScriptFile",						&CXSLDungeonManager::OpenScriptFile );
	lua_tinker::class_def<CXSLDungeonManager>( GetLuaState(), "SetSecretDungeonExtraStageNpcLevel",	&CXSLDungeonManager::SetSecretDungeonExtraStageNpcLevel_LUA );
	lua_tinker::class_def<CXSLDungeonManager>( GetLuaState(), "AddExtraStageCondition",				&CXSLDungeonManager::AddExtraStageCondition_LUA );
	//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_def<CXSLDungeonManager>( GetLuaState(), "SetExtraStageFactor",				&CXSLDungeonManager::SetExtraStageFactor_LUA );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	lua_tinker::decl( GetLuaState(), "g_pDungeonManager", this );
}

bool CXSLDungeonManager::OpenScriptFile_AllDungeonScriptLoad( IN lua_State* pLuaState )
{
	if( OpenScriptFile( pLuaState ) == false )
	{
		START_LOG( cerr, L"던전 스크립트 파싱 실패!" )
			<< END_LOG;
		return false;
	}

	if( m_mapDungeonData.empty() )
	{
		START_LOG( cerr, L"던전 스크립트 파싱 실패!" )
			<< BUILD_LOG( m_mapDungeonData.size() )
			<< END_LOG;
		return false;
	}

	const DWORD dwTime = ::GetTickCount();		

	std::pair< std::map< SEnum::DUNGEON_ID, CXSLDungeon >::iterator, bool > pairRet;

	std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >::iterator mit;
	for( mit = m_mapDungeonData.begin(); mit != m_mapDungeonData.end(); ++mit )
	{
		pairRet = m_mapDungeon.insert( std::make_pair( mit->second.m_DungeonID, CXSLDungeon( mit->second ) ) );
		if( pairRet.second )
		{
			// 던전 스크립트 파싱!
            if( pairRet.first->second.OpenScriptFile() == false )
			{
				START_LOG( cerr, L"던전 스크립트 파싱 실패!" )
					<< BUILD_LOG( mit->second.m_DungeonID )
					<< END_LOG;

                // 스크립트 파싱 실패 했으면 던전 정보를 삭제 하자!
				m_mapDungeon.erase( pairRet.first );
				continue;
			}
		}
	}

	START_LOG( cout2, L"던전 스크립트 로딩 시간 : " )
		<< BUILD_LOG( GetTickCount() - dwTime );
	return true;
}

bool CXSLDungeonManager::AddDungeonData_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	TableBind( &luaManager );

	CXSLDungeon::DungeonData dungeonData; //= new CXSLDungeon::DungeonData;

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"dungeonID",			dungeonData.m_DungeonID,	SEnum::DUNGEON_ID,	SEnum::DI_NONE,	return false; );
	//LUA_GET_VALUE_RETURN(		luaManager, L"dungeonName",			dungeonData.m_DungeonName,	L"",	return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"dataFileName",		dungeonData.m_DataFileName,	L"",	return false; );

	LUA_GET_VALUE(				luaManager, L"m_MinLevel",			dungeonData.m_MinLevel,			 0 );
	LUA_GET_VALUE(				luaManager, L"m_MaxLevel",			dungeonData.m_MaxLevel,			 0 );
	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	LUA_GET_VALUE(				luaManager, L"m_NPCLevel",			dungeonData.m_NpcLevel,			 0 );
#endif SERV_DUNGEON_NPC_LEVEL
	//}}
	//LUA_GET_VALUE(				luaManager, L"requireLevel",		dungeonData.m_RequireLevel,		 0 );
	LUA_GET_VALUE_ENUM(			luaManager, L"requireDungeonID",	dungeonData.m_RequireDungeonID, SEnum::DUNGEON_ID,	SEnum::DI_NONE );
	//{{ 2007. 8. 29  최육사  
	LUA_GET_VALUE(				luaManager, L"requireItemID",		dungeonData.m_RequireItemID,	 0 );
	LUA_GET_VALUE(				luaManager, L"requireItemCount",	dungeonData.m_RequireItemCount,	 0 );
	//}}
#ifdef SERV_ADDITIONAL_TICKET
	if( dungeonData.m_RequireItemID > 0 )
	{
		dungeonData.m_vecAdditionalTicket.push_back( std::make_pair( dungeonData.m_RequireItemID, dungeonData.m_RequireItemCount ) );

		START_LOG( cout, L"기본 티켓 추가 성공" )
			<< BUILD_LOG( dungeonData.m_RequireItemID )
			<< BUILD_LOG( dungeonData.m_RequireItemCount )
			<< END_LOG;
	}
	if( luaManager.BeginTable( "additionalTicket" ) == S_OK )
	{
		int iTicketIndex = 1;
		while( luaManager.BeginTable( iTicketIndex ) == S_OK )
		{
			int iRequireItemID;
			int iRequireItemCount;
			LUA_GET_VALUE(				luaManager, L"requireItemID",		iRequireItemID,	 0 );
			LUA_GET_VALUE(				luaManager, L"requireItemCount",	iRequireItemCount,	 0 );
			if( iRequireItemID > 0 )
			{
				dungeonData.m_vecAdditionalTicket.push_back( std::make_pair( iRequireItemID, iRequireItemCount ) );

				START_LOG( cout, L"추가 티켓 추가 성공" )
					<< BUILD_LOG( iRequireItemID )
					<< BUILD_LOG( iRequireItemCount )
					<< END_LOG;
			}

			luaManager.EndTable();
			iTicketIndex++;
		}
		luaManager.EndTable();
	}
#endif SERV_ADDITIONAL_TICKET

	LUA_GET_VALUE(				luaManager, L"requireItemID2",		dungeonData.m_RequireItemID2,		0 );
	LUA_GET_VALUE(				luaManager, L"requireItemCount2",	dungeonData.m_RequireItemCount2,	0 );
	

	//{{ 2007. 10. 4  최육사  근성도
	LUA_GET_VALUE(				luaManager, L"requireSpirit",		dungeonData.m_RequireSpirit,	 0 );
	//}}

	//{{ 2012. 05. 22	최육사	던전 입장 아이템 레벨
#ifdef SERV_DUNGEON_REQUIRE_ITEM_LEVEL
	LUA_GET_VALUE(				luaManager, L"m_RequireItemLevel",	dungeonData.m_RequireItemLevel,	 0 );
#endif SERV_DUNGEON_REQUIRE_ITEM_LEVEL
	//}}

	LUA_GET_VALUE_ENUM(			luaManager, L"difficulty",			dungeonData.m_eDifficulty,	CXSLDungeon::DIFFICULTY_LEVEL, CXSLDungeon::DL_NORMAL );

	LUA_GET_VALUE(				luaManager, L"normalOnly",			dungeonData.m_bNormalOnly,		false );

	//{{ dmlee 2009.2.25 던전 플레이 제한 시간
	LUA_GET_VALUE(				luaManager, L"m_fTimeLimit",		dungeonData.m_fTimeLimit,		-1.f );
	//}} dmlee 2009.2.25 던전 플레이 제한 시간

	//{{ 2010. 04. 26  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	LUA_GET_VALUE(				luaManager, L"m_fPoisonTimeLimit",			 dungeonData.m_fPoisonTimeLimit,			-1.f );
	LUA_GET_VALUE(				luaManager, L"m_fExtraStagePoisonTimeLimit", dungeonData.m_fExtraStagePoisonTimeLimit,	-1.f );
#endif SERV_SECRET_HELL
	//}}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	LUA_GET_VALUE(				luaManager, L"m_bLevelEqualized",	dungeonData.m_bLevelEqualized,		false );
	LUA_GET_VALUE(				luaManager, L"m_bDamageEqualized",	dungeonData.m_bDamageEqualized,		false );
	LUA_GET_VALUE(				luaManager, L"m_bEventDungeon",		dungeonData.m_bEventDungeon,		false );
	LUA_GET_VALUE(				luaManager, L"m_usFixedMembers",	dungeonData.m_usFixedMembers,		0 );
#endif SERV_DUNGEON_OPTION_IN_LUA

	LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonType",				dungeonData.m_eDungeonType,					CXSLDungeon::DUNGEON_TYPE,	CXSLDungeon::DT_NORMAL );
	LUA_GET_VALUE(				luaManager, L"m_bRelativeMonsterLevel",		dungeonData.m_bRelativeMonsterLevel,		false );

#ifdef SERV_NEW_EVENT_TYPES
	LUA_GET_VALUE(				luaManager, L"m_bSwitchingWithEventInfo",			dungeonData.m_bSwitchingWithEventInfo,		false );

	if( dungeonData.m_bSwitchingWithEventInfo == true &&
		m_mapEventDungeonEnable.find( dungeonData.m_DungeonID ) == m_mapEventDungeonEnable.end() )
	{
		m_mapEventDungeonEnable.insert( std::make_pair( dungeonData.m_DungeonID, false ) );	
	}
	else if( dungeonData.m_bSwitchingWithEventInfo == false &&
		m_mapEventDungeonEnable.find( dungeonData.m_DungeonID ) != m_mapEventDungeonEnable.end() )
	{
		m_mapEventDungeonEnable.erase( dungeonData.m_DungeonID );
	}
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	LUA_GET_VALUE(				luaManager, L"m_iLimitedPlayTimes",		dungeonData.m_iLimitedPlayTimes,		0 );
	LUA_GET_VALUE(				luaManager, L"m_iLimitedClearTimes",	dungeonData.m_iLimitedClearTimes,		0 );
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

	m_mapDungeonData.insert( std::make_pair( dungeonData.m_DungeonID, dungeonData ) );
	return true;
}

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
int CXSLDungeonManager::GetNPCDropTime( IN const int nDungeonID, IN const int iStageIndex, IN const int iSubStageIndex, IN const int iNPCID )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: 던전 스크립트가 정상인지 확인 바랍니다~!!!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;

		return 0;
	}

	return mit->second.GetNPCDropTime( iStageIndex, iSubStageIndex, iNPCID );
}
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// 적용날짜: 2013-08-13
float CXSLDungeonManager::GetNPCExpRate( IN const int nDungeonID, IN const int iStageIndex, IN const int iSubStageIndex, IN const int iNPCID )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: 던전 스크립트가 정상인지 확인 바랍니다~!!!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;

		return 1.f;
	}

	return mit->second.GetNPCExpRate( iStageIndex, iSubStageIndex, iNPCID );
}
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE

//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
bool CXSLDungeonManager::GetNPCData( IN const int nDungeonID, IN const int nStageNum, IN const int iRelativeMonsterLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage )
#else
bool CXSLDungeonManager::GetNPCData( IN const int nDungeonID, IN const int nStageNum, IN const int iHighestUserLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage )
#endif SERV_DUNGEON_NPC_LEVEL
//}}
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: 던전 스크립트가 정상인지 확인 바랍니다~!!!" )
			<< BUILD_LOG( nDungeonID )
			<< BUILD_LOG( nStageNum )
			<< END_LOG;

		return false;
	}

	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	if( mit->second.GetNPCData( nStageNum, iRelativeMonsterLevel, kNot, bIsBossStage ) == false )
#else
	if( mit->second.GetNPCData( nStageNum, iHighestUserLevel, kNot, bIsBossStage ) == false )
#endif SERV_DUNGEON_NPC_LEVEL
	//}}	
	{
		START_LOG( cerr, L" ::: STAGE DATA를 찾지 못했음 ::: " )
			<< BUILD_LOG( nDungeonID )
			<< BUILD_LOG( nStageNum )
			<< END_LOG;

		return false;
	}

	return true;
}

bool CXSLDungeonManager::GetDungeonName( IN int nDungeonID, OUT std::wstring& strDungeonName )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: " )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;

		return false;
	}

	strDungeonName = mit->second.GetDungeonData().m_DungeonName;
	return true;
}
int CXSLDungeonManager::GetDungeonMinLevel( IN int nDungeonID ) const
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::const_iterator mit;

	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );

	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_MinLevel;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(MaxLevel) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return 0;
}

int CXSLDungeonManager::GetDungeonMaxLevel( IN int nDungeonID ) const
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::const_iterator mit;

	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );

	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_MaxLevel;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(MaxLevel) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return 0;
}

//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_DUNGEON_NPC_LEVEL
int CXSLDungeonManager::GetDungeonNpcLevel( IN const int iDungeonID ) const
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::const_iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(iDungeonID) );
	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_NpcLevel;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(NpcLevel) 찾기 실패.!" )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
	}

	return 0;
}
#endif SERV_DUNGEON_NPC_LEVEL
//}}

int CXSLDungeonManager::GetRequireDungeonID( IN int nDungeonID )
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::iterator mit;

	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );

	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_RequireDungeonID;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(RequireDungeonID) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return 0;
}

//{{ 2012. 05. 22	최육사	던전 입장 아이템 레벨
#ifdef SERV_DUNGEON_REQUIRE_ITEM_LEVEL
int	CXSLDungeonManager::GetRequireItemLevel( IN const int iDungeonID ) const
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::const_iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(iDungeonID) );
	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_RequireItemLevel;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(m_RequireItemLevel) 찾기 실패.!" )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
	}

	return 0;
}
#endif SERV_DUNGEON_REQUIRE_ITEM_LEVEL
//}}

//{{ 2010. 03. 24  최육사	비밀던전 헬모드
CXSLDungeonStage::STAGE_TYPE CXSLDungeonManager::GetStageType( IN int iDungeonID, IN int iStageNum )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( static_cast<SEnum::DUNGEON_ID>(iDungeonID) );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L"던전 정보가 없습니다!" )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;

		return CXSLDungeonStage::ST_NONE;
	}

	return mit->second.GetStageType( iStageNum );
}
//}}

#ifdef SERV_ADDITIONAL_TICKET
bool CXSLDungeonManager::GetTicketInformation( IN int nDungeonID, OUT std::vector< std::pair< int, int > >& vecTicketInformation )
{
	vecTicketInformation.clear();

	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );

	if( mit != m_mapDungeonData.end() )
	{
		vecTicketInformation = mit->second.m_vecAdditionalTicket;

		return true;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(m_mapDungeonData) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return false;
}
#endif SERV_ADDITIONAL_TICKET

bool CXSLDungeonManager::GetRequireItemIDAndCount( IN int nDungeonID, IN CXSLDungeon::DUNGEON_MODE eDungeonMode, OUT int& iItemID, OUT int& iItemCount )
{
	iItemID = 0;
	iItemCount = 0;
	
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );

	if( mit != m_mapDungeonData.end() )
	{
		switch( mit->second.m_eDungeonType )
		{
		default:
			{
				START_LOG( cerr, L"던전데이터(RequireItemID) 오류!" )
					<< BUILD_LOG( nDungeonID )
					<< BUILD_LOG( (int)eDungeonMode )
					<< END_LOG;
				return false;
			} break;
			
		case CXSLDungeon::DT_NORMAL:
			{
				iItemID		= mit->second.m_RequireItemID;
				iItemCount	= mit->second.m_RequireItemCount;
				return true;
			} break;

		case CXSLDungeon::DT_HENIR:
			{
				switch( eDungeonMode )
				{
				default:
					{
						START_LOG( cerr, L"헤니르 던전 모드 오류!" )
							<< BUILD_LOG( nDungeonID )
							<< BUILD_LOG( (int)eDungeonMode )
							<< END_LOG;

						return false;
					} break;
				case CXSLDungeon::DM_HENIR_PRACTICE:
					{
						iItemID		= mit->second.m_RequireItemID;
						iItemCount	= mit->second.m_RequireItemCount;
						return true;

					} break;

				case CXSLDungeon::DM_HENIR_CHALLENGE:
					{
						iItemID		= mit->second.m_RequireItemID2;
						iItemCount	= mit->second.m_RequireItemCount2;
						return true;

					} break;
				}
			} break;

			//{{ 2010. 03. 18  최육사	비밀던전 헬모드
		case CXSLDungeon::DT_SECRET:
			{
				switch( eDungeonMode )
				{
				default:
					{
						START_LOG( cerr, L"비밀 던전 모드 오류!" )
							<< BUILD_LOG( nDungeonID )
							<< BUILD_LOG( (int)eDungeonMode )
							<< END_LOG;

						return false;
					} break;
				case CXSLDungeon::DM_SECRET_NORMAL:
					{

						iItemID		= mit->second.m_RequireItemID;
						iItemCount	= mit->second.m_RequireItemCount;
						return true;

					} break;
				case CXSLDungeon::DM_SECRET_HELL:
					{
						iItemID		= mit->second.m_RequireItemID;
						iItemCount	= mit->second.m_RequireItemCount;
						return true;

					} break;
				}
			} break;
			//}}
		}
	}
	else
	{
		START_LOG( cerr, L"던전데이터(RequireItemID) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< BUILD_LOG( (int)eDungeonMode )
			<< END_LOG;
	}

	return false;
}

//{{ 2007. 10. 4  최육사  근성도 얻기 함수
bool CXSLDungeonManager::GetRequireSpiritCount( IN int nDungeonID, OUT int& nSpiritByDungeon )
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::iterator mit;

	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );

	if( mit != m_mapDungeonData.end() )
	{
		nSpiritByDungeon = mit->second.m_RequireSpirit;
		return true;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(RequireSpirit) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return false;
}
//}}

int CXSLDungeonManager::GetStartPosByRandom( IN int nDungeonID, IN int iStageIndex, IN int iSubStageIndex )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: " )
			<< BUILD_LOG( nDungeonID )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( iSubStageIndex )
			<< END_LOG;

		return -1;
	}

	return mit->second.GetStartPosByRandom( iStageIndex, iSubStageIndex );
}

//{{ 2010. 04. 26  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
void CXSLDungeonManager::GetExtraNpcInfo( IN int nDungeonID, IN int iStageIndex, IN int iSubStageIndex, OUT int& iStartPos, OUT bool& bIsRight )
{
	iStartPos = -1;
	bIsRight = false;

	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: " )
			<< BUILD_LOG( nDungeonID )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( iSubStageIndex )
			<< END_LOG;

		return;
	}

	mit->second.GetExtraNpcInfo( iStageIndex, iSubStageIndex, iStartPos, bIsRight );
}

float CXSLDungeonManager::GetPoisonTimeLimit( IN int nDungeonID )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >::const_iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );
	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_fPoisonTimeLimit;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(PoisonTimeLimit) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return -1;
}

float CXSLDungeonManager::GetExtraStagePoisonTimeLimit( IN int nDungeonID )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >::const_iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );
	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_fExtraStagePoisonTimeLimit;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(ExtraStagePoisonTimeLimit) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return -1;
}

#endif SERV_SECRET_HELL
//}}


float CXSLDungeonManager::GetPlayTimeLimit( IN int nDungeonID )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >::const_iterator mit;

	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );

	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_fTimeLimit;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(PlayTimeLimit) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return -1;
}

//{{ 2009. 7. 3  최육사		헤니르던전
CXSLDungeon::DUNGEON_TYPE CXSLDungeonManager::GetDungeonType( IN int nDungeonID )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >::const_iterator mit;

	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) );

	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_eDungeonType;
	}
	else
	{
		START_LOG( cerr, L"던전타입(DungeonType) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return CXSLDungeon::DT_INVALID;
}
//}}

//{{ 2010. 03. 24  최육사	비밀던전 헬모드
void CXSLDungeonManager::SetSecretDungeonExtraStageNpcLevel_LUA( int iLevel )
{
	if( iLevel < 0 )
	{
		START_LOG( cerr, L"level값이 이상합니다." )
			<< BUILD_LOG( iLevel )
			<< END_LOG;
		return;
	}

	m_iSecretDungeonExtraStageNpcLevel = iLevel;
}

void CXSLDungeonManager::AddExtraStageCondition_LUA( int iBeginLevel, int iEndLevel, float fFactor )
{
	if( iBeginLevel < 0  ||  iEndLevel < 0  ||  fFactor < 0 )
	{
		START_LOG( cerr, L"스크립트 세팅값이 이상합니다." )
			<< BUILD_LOG( iBeginLevel )
			<< BUILD_LOG( iEndLevel )
			<< BUILD_LOG( fFactor )
			<< END_LOG;
		return;
	}

	for( int iLv = iBeginLevel; iLv <= iEndLevel; ++iLv )
	{
		m_mapExtraStageConditionFactor.insert( std::make_pair( iLv, fFactor ) );
	}
}

//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void CXSLDungeonManager::SetExtraStageFactor_LUA( float fFactor )
{
	START_LOG( cout, L"Extra Stage Factor : " << fFactor );

	m_fExtraStageFactor = fFactor;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

float CXSLDungeonManager::GetExtraStageConditionFactor( IN char cLevel )
{
	std::map< char, float >::const_iterator mit;
	mit = m_mapExtraStageConditionFactor.find( cLevel );
	if( mit == m_mapExtraStageConditionFactor.end() )
	{
		START_LOG( cerr, L"해당 레벨에 대한 데이터가 없습니다. 절대 일어날수 없는에러!" )
			<< BUILD_LOGc( cLevel )
			<< END_LOG;

		return 0.0f;
	}

	return mit->second;
}
//}}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
bool CXSLDungeonManager::IsFixedMembers( IN int nDungeonID )
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::const_iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>( nDungeonID ) );

	if( mit != m_mapDungeonData.end() )
	{
		return ( mit->second.m_usFixedMembers > 0 );
	}
	else
	{
		START_LOG( cerr, L"던전데이터(m_usFixedMembers) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return false;
}
short CXSLDungeonManager::GetFixedMembers( IN int nDungeonID )
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::const_iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>( nDungeonID ) );

	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_usFixedMembers;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(m_usFixedMembers) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return 0;
}
bool CXSLDungeonManager::IsEventDungeon( IN int nDungeonID )
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::const_iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>( nDungeonID ) );

	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_bEventDungeon;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(m_bEventDungeon) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return false;
}
#endif SERV_DUNGEON_OPTION_IN_LUA


#ifdef SERV_NEW_EVENT_TYPES
bool CXSLDungeonManager::IsSwitchingWithEventInfo( IN int nDungeonID )
{
	std::map<SEnum::DUNGEON_ID, CXSLDungeon::DungeonData>::const_iterator mit;
	mit = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>( nDungeonID ) );

	if( mit != m_mapDungeonData.end() )
	{
		return mit->second.m_bSwitchingWithEventInfo;
	}
	else
	{
		START_LOG( cerr, L"던전데이터(m_bSwitchingWithEventInfo) 찾기 실패.!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;
	}

	return false;
}
#endif SERV_NEW_EVENT_TYPES

//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool CXSLDungeonManager::GetCanBeEnteredDungeonID( IN const std::map< int, KDungeonClearInfo >& mapDungeonClear, OUT int& iDungeonID )
{
	iDungeonID = 0;

    if( mapDungeonClear.empty() )
	{
		iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;
		return true;
	}

	std::map< int, KDungeonClearInfo >::const_reverse_iterator mit;
	for( mit = mapDungeonClear.rbegin(); mit != mapDungeonClear.rend(); ++mit )
	{
		std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >::const_iterator mitDI;
		mitDI = m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(mit->first) );
		if( mitDI == m_mapDungeonData.end() )
		{
			START_LOG( cerr, L"클리어 했는데 던전데이터에는 없는 던전이네?" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mitDI->second.m_eDungeonType != CXSLDungeon::DT_NORMAL )
			continue;

        iDungeonID = mit->first;
		break;
	}

	return true;
}

void CXSLDungeonManager::GetNormalDungeonIDList( OUT std::vector< int >& vecNormalDungeonIDList )
{
	vecNormalDungeonIDList.clear();

	std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >::const_iterator mit;
	for( mit = m_mapDungeonData.begin(); mit != m_mapDungeonData.end(); ++mit )
	{
		if( mit->second.m_eDungeonType != CXSLDungeon::DT_NORMAL )
			continue;

		if( CXSLDungeon::IsDefenceDungeon( mit->first ) == true )
			continue;

		vecNormalDungeonIDList.push_back( static_cast<int>(mit->first) );
	}
}

void CXSLDungeonManager::GetDefenceDungeonIDList( OUT std::vector< int >& vecDefenceDungeonIDList )
{
	vecDefenceDungeonIDList.clear();

	std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >::const_iterator mit;
	for( mit = m_mapDungeonData.begin(); mit != m_mapDungeonData.end(); ++mit )
	{
		if( CXSLDungeon::IsDefenceDungeon( mit->first ) == false )
			continue;

		vecDefenceDungeonIDList.push_back( static_cast<int>(mit->first) );
	}
}

bool CXSLDungeonManager::CheckMinLevelForEnterDungeon( IN const int iDungeonID, IN const u_char ucLevel ) const
{
	const int iMinLevel = GetDungeonMinLevel( iDungeonID );

	return ( ( iMinLevel - 2 ) < static_cast<int>(ucLevel) );
}

bool CXSLDungeonManager::CheckLevelForDungeonAutoPartyBonus( IN const int iDungeonID, IN const u_char ucLevel ) const
{
	const int iNpcLevel = GetDungeonNpcLevel( iDungeonID );
	const float fDungeonBalRate = CXSLDungeon::CheckDungeonBalRateByNpcLv( ucLevel, iNpcLevel );
	return ( fDungeonBalRate > 0.f );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
int CXSLDungeonManager::GetSecretStageEnteringEvent( IN const int nDungeonID, IN const int iStageIndex, IN const int iSubStageIndex, IN int iClearConditionIndex )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: 던전 스크립트가 정상인지 확인 바랍니다~!!!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;

		return 0;
	}

	return mit->second.GetSecretStageEnteringEvent( iStageIndex, iSubStageIndex, iClearConditionIndex );
}

bool CXSLDungeonManager::GetNextStage( OUT CXSLDungeonSubStage::NextStageData& kNextStageData, IN const int nDungeonID, IN const int iStageIndex, IN const int iSubStageIndex, IN int iClearConditionIndex, IN int iSecretPadIndex/* = 0*/ )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: 던전 스크립트가 정상인지 확인 바랍니다~!!!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;

		return false;
	}

	return mit->second.GetNextStage( kNextStageData, iStageIndex, iSubStageIndex, iClearConditionIndex, iSecretPadIndex );
}
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
bool CXSLDungeonManager::GetLimitedPlayTimes( IN const int nDungeonID, OUT int& iPlayTimes )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: 던전 스크립트가 정상인지 확인 바랍니다~!!!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;

		return false;
	}

	// 플레이 횟수 제한 던전이 아닌 경우
	if( mit->second.GetDungeonData().m_iLimitedPlayTimes <= 0 )
		return false;

	// 플레이 횟수 제한 던전인 경우
	iPlayTimes = mit->second.GetDungeonData().m_iLimitedPlayTimes;
	return true;
}
bool CXSLDungeonManager::GetLimitedClearTimes( IN const int nDungeonID, OUT int& iClearTimes )
{
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >::const_iterator mit;
	mit = m_mapDungeon.find( (SEnum::DUNGEON_ID)nDungeonID );
	if( mit == m_mapDungeon.end() )
	{
		START_LOG( cerr, L" ::: DUNGEON DATA를 찾지 못했음 ::: 던전 스크립트가 정상인지 확인 바랍니다~!!!" )
			<< BUILD_LOG( nDungeonID )
			<< END_LOG;

		return false;
	}

	// 플레이 횟수 제한 던전이 아닌 경우
	if( mit->second.GetDungeonData().m_iLimitedClearTimes <= 0 )
		return false;

	// 플레이 횟수 제한 던전인 경우
	iClearTimes = mit->second.GetDungeonData().m_iLimitedClearTimes;
	return true;
}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES