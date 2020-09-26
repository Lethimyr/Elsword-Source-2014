#include ".\xsltitlemanager.h"
#include "NetError.h"


ImplementRefreshSingleton( CXSLTitleManager );

CXSLTitleManager::CXSLTitleManager(void)
{
	
}

CXSLTitleManager::~CXSLTitleManager(void)
{
}

ImplToStringW( CXSLTitleManager )
{
	stm_	<< L"----------[ Title Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapTitleInfo.size() )
			<< TOSTRINGW( m_mapMission.size() )
			<< TOSTRINGW( m_mapSubMission.size() )
			<< TOSTRINGW( m_vecAutoRegMission.size() )
			//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
			<< TOSTRINGW( m_mapTitleInfoByUseItemID.size() )
#endif SERV_TITLE_ITEM_NEW
			//}}
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLTitleManager )
{
	lua_tinker::class_add<CXSLTitleManager>( GetLuaState(), "CXSLTitleManager" );
	lua_tinker::class_def<CXSLTitleManager>( GetLuaState(), "AddTitleInfo_LUA",				&CXSLTitleManager::AddTitleInfo_LUA );
	lua_tinker::class_def<CXSLTitleManager>( GetLuaState(), "AddTitleMissionInfo_LUA",		&CXSLTitleManager::AddTitleMissionInfo_LUA );
	lua_tinker::class_def<CXSLTitleManager>( GetLuaState(), "AddSubTitleMissionInfo_LUA",	&CXSLTitleManager::AddSubTitleMissionInfo_LUA );
	lua_tinker::class_def<CXSLTitleManager>( GetLuaState(), "dump",							&CXSLTitleManager::Dump );

	lua_tinker::decl( GetLuaState(), "TitleManager", this );
}

bool CXSLTitleManager::AddTitleInfo_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	//TableBind( &luaManager );

	TitleTemplet kTitleInfo;
	
	LUA_GET_VALUE_RETURN(	luaManager, L"m_iTitleID",				kTitleInfo.m_iTitleID,					0,				goto end_proc );
	LUA_GET_VALUE(			luaManager, L"m_iSortNum",				kTitleInfo.m_iSortNum,					0 );

	LUA_GET_VALUE_ENUM(		luaManager, L"m_eTitleType",			kTitleInfo.m_eTitleType,				TITLE_TYPE,		TT_NONE );
	LUA_GET_VALUE(			luaManager, L"m_TitleName",				kTitleInfo.m_wstrTitleName,				L"" );

	LUA_GET_VALUE_ENUM(		luaManager, L"m_eUnitType",				kTitleInfo.m_eUnitType,					CXSLUnit::UNIT_TYPE,		CXSLUnit::UT_NONE );
	LUA_GET_VALUE_ENUM(		luaManager, L"m_eUnitClass",			kTitleInfo.m_eUnitClass,				CXSLUnit::UNIT_CLASS,		CXSLUnit::UC_NONE );
	LUA_GET_VALUE(			luaManager, L"m_iOpenLevel",			kTitleInfo.m_iOpenLevel,				0 );
	LUA_GET_VALUE(			luaManager, L"m_bIsSecretTitle",		kTitleInfo.m_bIsSecretTitle,			false );

	LUA_GET_VALUE(			luaManager, L"m_iBaseHP",				kTitleInfo.m_iBaseHP,					0 );
	LUA_GET_VALUE(			luaManager, L"m_iAtkPhysic",			kTitleInfo.m_iAtkPhysic,				0 );
	LUA_GET_VALUE(			luaManager, L"m_iAtkMagic",				kTitleInfo.m_iAtkMagic,					0 );
	LUA_GET_VALUE(			luaManager, L"m_iDefPhysic",			kTitleInfo.m_iDefPhysic,				0 );
	LUA_GET_VALUE(			luaManager, L"m_iDefMagic",				kTitleInfo.m_iDefMagic,					0 );

	if( luaManager.BeginTable( L"SOCKET_OPTION" ) == S_OK )
	{
		int index = 1;
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		int iSocketOption	= -1;
		while( luaManager.GetValue( index, iSocketOption ) == S_OK )
		{
			if( iSocketOption > 0 )
				kTitleInfo.m_vecSocketOption.push_back( iSocketOption );
			++index;
		}
#else
		short sSocketOption	= -1;
		while( luaManager.GetValue( index, sSocketOption ) == S_OK )
		{
			if( sSocketOption > 0 )
				kTitleInfo.m_vecSocketOption.push_back( sSocketOption );
			++index;
		}
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		luaManager.EndTable();
	}

#ifndef SERV_COUNTRY_TW
	if( kTitleInfo.m_iTitleID == 5160 )	//대만 전용 감마니아 칭호
	{
	
	}
	else
#endif SERV_COUNTRY_TW
	m_mapTitleInfo.insert( std::make_pair( kTitleInfo.m_iTitleID, kTitleInfo ) );
	return true;

end_proc:
	START_LOG( cerr, L"타이틀 스크립트 파싱 실패.!" )
		<< BUILD_LOG( kTitleInfo.m_iTitleID )
		<< END_LOG;

	return false;
}

const CXSLTitleManager::TitleTemplet* CXSLTitleManager::GetTitleTemplet( int iTitleID )
{
	std::map< int, TitleTemplet >::const_iterator mit;
	mit = m_mapTitleInfo.find( iTitleID );
	if( mit == m_mapTitleInfo.end() )
		return NULL;

	return &mit->second;
}

bool CXSLTitleManager::IsExistTitleID( int iTitleID )
{
	return ( GetTitleTemplet( iTitleID ) != NULL );
}

bool CXSLTitleManager::AddTitleMissionInfo_LUA()
{
	KLuaManager luaManager( GetLuaState() );

	MissionTemplet kMissionTemplet;

	LUA_GET_VALUE_RETURN(		luaManager, L"m_iMissionID",			kMissionTemplet.m_iMissionID,				0,			goto LoadFail; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eClearType",			kMissionTemplet.m_eClearType,				TITLE_MISSION_CLEAR_TYPE, TMCT_NONE, goto LoadFail; );	
	LUA_GET_VALUE_RETURN(		luaManager, L"m_MissionName",			kMissionTemplet.m_wstrMissionName,			L"",		goto LoadFail; );

	//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
	m_vecMissionIDByClearType[kMissionTemplet.m_eClearType].push_back( kMissionTemplet.m_iMissionID );
#endif SERV_TITLE_ITEM_NEW
	//}}

	// Load Mission Condition
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iConditionLv",			kMissionTemplet.m_kCondition.m_iLevel,		-1,			goto LoadFail; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eConditionUnitType",	kMissionTemplet.m_kCondition.m_eUnitType,	CXSLUnit::UNIT_TYPE,	CXSLUnit::UT_NONE,	goto LoadFail; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eConditionUnitClass",	kMissionTemplet.m_kCondition.m_eUnitClass,	CXSLUnit::UNIT_CLASS,	CXSLUnit::UC_NONE,	goto LoadFail; );	
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iConditionClearMissionID", kMissionTemplet.m_kCondition.m_iClearMissionID,	-1,			goto LoadFail; );

	// Load Sub Mission
	if( luaManager.BeginTable( L"m_SubMission" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
				kMissionTemplet.m_vecSubMission.push_back( buf );
			index++;
		}

		luaManager.EndTable();
	}

	// Load Reward
	LUA_GET_VALUE(				luaManager, L"m_sPeriod",				kMissionTemplet.m_sPeriod,		0 );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iTitleID",				kMissionTemplet.m_iTitleID,		0,		goto LoadFail; );

	// 비공개 타이틀인지 검사
	const TitleTemplet* pTitleTemplet = GetTitleTemplet( kMissionTemplet.m_iMissionID );
	if( pTitleTemplet == NULL )
	{
		START_LOG( cwarn, L"미션에 해당되는 칭호정보가 존재하지 않습니다." )
			<< BUILD_LOG( kMissionTemplet.m_iMissionID )
			<< END_LOG;

		return false;
	}

	kMissionTemplet.m_bIsSecretTitle = pTitleTemplet->m_bIsSecretTitle;
	
	m_mapMission.insert( std::make_pair( kMissionTemplet.m_iMissionID, kMissionTemplet ) );

	// 자동 미션 등록
	m_vecAutoRegMission.push_back( kMissionTemplet );

	return true;
LoadFail:

	START_LOG( cerr, L"TitleMission 스크립트 파싱 실패!" )
		<< BUILD_LOG( kMissionTemplet.m_iMissionID )
		<< END_LOG;
	return false;
}

const CXSLTitleManager::MissionTemplet* CXSLTitleManager::GetMissionInfo( int iMissionID )
{
	std::map< int, MissionTemplet >::const_iterator mit;
	mit = m_mapMission.find( iMissionID );
	if( mit == m_mapMission.end() )
		return NULL;

	return &mit->second;
}

bool CXSLTitleManager::AddSubTitleMissionInfo_LUA()
{
	KLuaManager luaManager( GetLuaState() );

	SubMissionTemplet kSubMissionTemplet;

	LUA_GET_VALUE_RETURN(		luaManager, L"m_iID",				kSubMissionTemplet.m_iSubMissionID,		0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_wstrDescription",	kSubMissionTemplet.m_wstrDescription,	L"",	goto LoadFail; );

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eClearType",		kSubMissionTemplet.m_eClearType,	TITLE_MISSION_CLEAR_TYPE,	TMCT_NONE,	goto LoadFail; );	

	LUA_GET_VALUE_RETURN(		luaManager, L"m_bAutomaticDescription",	kSubMissionTemplet.m_bAutomaticDescription,	true,		goto LoadFail; );

	if( LoadClearCondition( luaManager, kSubMissionTemplet ) == false )
		goto LoadFail;

	m_mapSubMission.insert( std::make_pair( kSubMissionTemplet.m_iSubMissionID, kSubMissionTemplet ) );

	return true;

LoadFail:
	START_LOG( cerr, L"::: Sub Mission Templet Load Fail :::" )
		<< BUILD_LOG( kSubMissionTemplet.m_iSubMissionID )
		<< BUILD_LOG( kSubMissionTemplet.m_wstrDescription )
		<< BUILD_LOG( kSubMissionTemplet.m_bAutomaticDescription )
		<< END_LOG;
	return false;
}

const CXSLTitleManager::SubMissionTemplet* CXSLTitleManager::GetSubMissionInfo( int iSubMissionID )
{
	std::map< int, SubMissionTemplet >::const_iterator mit;
	mit = m_mapSubMission.find( iSubMissionID );
	if( mit == m_mapSubMission.end() )
		return NULL;

	return &mit->second;
}

//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
bool CXSLTitleManager::GetUseItemTitleInfoBySubMissionID( IN const int iSubMissionID_, OUT TitleSimpleInfo& kInfo )
{
	bool bFindSuccess = false;

	BOOST_TEST_FOREACH( const int, iMissionID, m_vecMissionIDByClearType[TMCT_USE_ITEM] )
	{
		// 타이틀이 없다면 그냥 넘어가자!
		if( IsExistTitleID( iMissionID ) == false )
			continue;

		const MissionTemplet* pMissionTemplet = GetMissionInfo( iMissionID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 미션입니다!" )
				<< BUILD_LOG( iMissionID )
				<< END_LOG;
			continue;
		}

		BOOST_TEST_FOREACH( const int, iSubMissionID, pMissionTemplet->m_vecSubMission )
		{
			// 해당 서브 미션을 찾았다!
            if( iSubMissionID != iSubMissionID_ )
				continue;

			if( bFindSuccess )
			{
				START_LOG( cwarn, L"이미 찾았는데 또 나왔네? 이건 버그다!" )
					<< BUILD_LOG( iSubMissionID_ )
					<< BUILD_LOG( iMissionID )
					<< END_LOG;
			}
			
			// 찾았으면 정보를 넣자!
			kInfo.m_iTitleID = pMissionTemplet->m_iTitleID;
			kInfo.m_sPeriod = pMissionTemplet->m_sPeriod;
			bFindSuccess = true;
		}
	}

	return bFindSuccess;
}

bool CXSLTitleManager::GetUseItemTitleInfoByItemID( IN const int iItemID, OUT TitleSimpleInfo& kInfo )
{
	std::map< int, TitleSimpleInfo >::const_iterator mit;
	mit = m_mapTitleInfoByUseItemID.find( iItemID );
	if( mit == m_mapTitleInfoByUseItemID.end() )
	{
		return false;
	}

	kInfo = mit->second;
	return true;
}

#endif SERV_TITLE_ITEM_NEW
//}}

bool CXSLTitleManager::LoadClearCondition( KLuaManager& luaManager, SubMissionTemplet& kSubMissionTemplet )
{
	int iDifficulty;

	if( luaManager.BeginTable( L"m_ClearCondition" ) == S_OK )
	{
		switch( kSubMissionTemplet.m_eClearType )
		{
		case TMCT_NPC_TALK:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eTalkNPCID",		kSubMissionTemplet.m_ClearCondition.m_eTalkNPCID,		CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE, goto error_proc; );				
			}
			break;

		case TMCT_NPC_HUNT:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				LUA_GET_VALUE(				luaManager, L"m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );

				//{{ 2012. 1. 11	Merge 박세훈	2012.12.26 임규수 타이틀 미션 클리어 NPC 복수 적용
#ifdef SERV_SUB_TITLE_MISSION_PLURAL_NPC
				if( luaManager.BeginTable( L"m_eKillNPCID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubMissionTemplet.m_ClearCondition.m_setKillNPCID.insert( static_cast<CXSLUnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eKillNPCID",		kSubMissionTemplet.m_ClearCondition.m_eKillNPCID,		CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE, goto error_proc; );
#endif SERV_SUB_TITLE_MISSION_PLURAL_NPC
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iKillNum",			kSubMissionTemplet.m_ClearCondition.m_iKillNum,			0, goto error_proc; );
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubMissionTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,		CXSLDungeon::DM_INVALID );
			}
			break;

		case TMCT_ITEM_COLLECTION:
			{				
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemID",	kSubMissionTemplet.m_ClearCondition.m_iCollectionItemID,	0, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemNum",kSubMissionTemplet.m_ClearCondition.m_iCollectionItemNum,	0, goto error_proc; );
			}
			break;

		case TMCT_QUEST_ITEM_COLLECTION:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2012. 1. 11	Merge 박세훈	2012.12.26 임규수 타이틀 미션 클리어 NPC 복수 적용
#ifdef SERV_SUB_TITLE_MISSION_PLURAL_NPC
				if( luaManager.BeginTable( L"m_eKillNPCID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubMissionTemplet.m_ClearCondition.m_setKillNPCID.insert( static_cast<CXSLUnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eKillNPCID",		kSubMissionTemplet.m_ClearCondition.m_eKillNPCID,		CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE, goto error_proc; );
#endif SERV_SUB_TITLE_MISSION_PLURAL_NPC
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemID",	kSubMissionTemplet.m_ClearCondition.m_iCollectionItemID,	0,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemNum",kSubMissionTemplet.m_ClearCondition.m_iCollectionItemNum,	0,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_fQuestItemDropRate",kSubMissionTemplet.m_ClearCondition.m_fQuestItemDropRate,	0.0f,	goto error_proc; );
			}
			break;

		case TMCT_DUNGEON_TIME:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubMissionTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,		CXSLDungeon::DM_INVALID );
#endif SERV_HELL_MODE_TITLE
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE(				luaManager, L"m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
#endif SERV_HELL_MODE_TITLE
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearTime",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearTime,	0,		goto error_proc; );
				LUA_GET_VALUE(		luaManager, L"m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	1 );

			}
			break;

		case TMCT_DUNGEON_RANK:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubMissionTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,		CXSLDungeon::DM_INVALID );
#endif SERV_HELL_MODE_TITLE
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE(				luaManager, L"m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
#endif SERV_HELL_MODE_TITLE
				//}}
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonRank",		kSubMissionTemplet.m_ClearCondition.m_eDungeonRank,	CXSLDungeon::RANK_TYPE,		CXSLDungeon::RT_NONE, goto error_proc; );
				LUA_GET_VALUE(		luaManager, L"m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	 1 );
			}
			break;

		case TMCT_DUNGEON_DAMAGE:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubMissionTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,		CXSLDungeon::DM_INVALID );
#endif SERV_HELL_MODE_TITLE
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE(				luaManager, L"m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
#endif SERV_HELL_MODE_TITLE
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonDamage",	kSubMissionTemplet.m_ClearCondition.m_iDungeonDamage,	-1, goto error_proc; );
				LUA_GET_VALUE(		luaManager, L"m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	 1 );
			}
			break;

		case TMCT_DUNGEON_CLEAR_COUNT:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubMissionTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,		CXSLDungeon::DM_INVALID );
#endif SERV_HELL_MODE_TITLE
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,			goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE(				luaManager, L"m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
#endif SERV_HELL_MODE_TITLE
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );


			}
			break;

		case TMCT_PVP_PLAY:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",			kSubMissionTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPPlay",			kSubMissionTemplet.m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
			}
			break;

		case TMCT_PVP_WIN:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",			kSubMissionTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPWin",			kSubMissionTemplet.m_ClearCondition.m_iPVPWin,		-1, goto error_proc; );
			}
			break;

		case TMCT_PVP_KILL:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",			kSubMissionTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPKill",			kSubMissionTemplet.m_ClearCondition.m_iPVPKill,		-1, goto error_proc; );
			}
			break;

		case TMCT_QUEST:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iQuestID",			kSubMissionTemplet.m_ClearCondition.m_iQuestID,		-1, goto error_proc; );
			}
			break;

		case TMCT_COLLECT_TITLE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iTitleID",			kSubMissionTemplet.m_ClearCondition.m_iTitleID,		-1, goto error_proc; );
			}
			break;

		case TMCT_USE_ITEM:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemID",			kSubMissionTemplet.m_ClearCondition.m_iItemID,		-1, goto error_proc; );

				//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
				TitleSimpleInfo kInfo;
				if( GetUseItemTitleInfoBySubMissionID( kSubMissionTemplet.m_iSubMissionID, kInfo ) == false )
				{
					START_LOG( clog, L"칭호 획득 아이템 정보 얻기 실패!" )
						<< BUILD_LOG( kSubMissionTemplet.m_iSubMissionID )
						<< BUILD_LOG( kSubMissionTemplet.m_ClearCondition.m_iItemID )
						<< END_LOG;
					break;
				}

				if( GetUseItemTitleInfoByItemID( kSubMissionTemplet.m_ClearCondition.m_iItemID, kInfo ) == true )
				{
					START_LOG( cerr, L"이미 등록된 칭호 획득 아이템 정보 입니다!" )
						<< BUILD_LOG( kSubMissionTemplet.m_iSubMissionID )
						<< BUILD_LOG( kSubMissionTemplet.m_ClearCondition.m_iItemID )
						<< END_LOG;
					goto error_proc;
				}

				m_mapTitleInfoByUseItemID.insert( std::make_pair( kSubMissionTemplet.m_ClearCondition.m_iItemID, kInfo ) );
#endif SERV_TITLE_ITEM_NEW
				//}}
			}
			break;

		case TMCT_NONE:
			{
				// 아케이드 랭킹 보상임
			}
			break;

			//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		case TMCT_WITH_DIF_SERV_USER:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
			}
			break;
#endif SERV_INTEGRATION
			//}}

			//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		case TMCT_GIVE_PET_FEED:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eItemGrade",		kSubMissionTemplet.m_ClearCondition.m_eItemGrade,		CXSLItem::ITEM_GRADE,		CXSLItem::IG_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPetFeedCount",		kSubMissionTemplet.m_ClearCondition.m_iPetFeedCount,	-1, goto error_proc; );
			}
			break;
		case TMCT_PLAYER_WITH_DUNGEON_CLEAR:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",			kSubMissionTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",			iDifficulty,												-1,					goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				LUA_GET_VALUE(				luaManager, L"m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPlayerCount",			kSubMissionTemplet.m_ClearCondition.m_iPlayerCount,			-1,					goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearTime",		kSubMissionTemplet.m_ClearCondition.m_iDungeonClearTime,	-1,					goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
		case TMCT_RESURRECTION_STONE:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",			kSubMissionTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",			iDifficulty,												-1,					goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				LUA_GET_VALUE(				luaManager, L"m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_bCheckResurrectionStone",	kSubMissionTemplet.m_ClearCondition.m_bCheckResurrectionStone,	false );
				//{{ 2013. 02. 05  칭호 획득 조건 추가(부활석 사용 횟수, 샌더 마을) - 김민성
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
				LUA_GET_VALUE(				luaManager, L"m_iDieCount",	kSubMissionTemplet.m_ClearCondition.m_iUseResurrectionStoneCount,	0 );
#endif SERV_ADD_TITLE_CONDITION_SANDER
				//}
			}
			break;
		case TMCT_TOGETHER_MISSION_CLEAR:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
		case TMCT_USER_UNIT_DIE:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDieReason",			kSubMissionTemplet.m_ClearCondition.m_eDieReason,	KEGS_USER_UNIT_DIE_REQ::USER_UNIT_DIE_REASON,	KEGS_USER_UNIT_DIE_REQ::UUDR_UNKNOWN,	goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDieCount",			kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,			goto error_proc; );
			}
			break;
//}}
#endif SERV_ADD_TITLE_CONDITION
			//}}			

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
			// 편의상 m_iDungeonClearCount 로 통일 한다.
		case TMCT_ITEM_SOCKET:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemSocketCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemLevel",		kSubMissionTemplet.m_ClearCondition.m_iItemLevel,	-1,				goto error_proc; );

				if( kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount <= 0 || kSubMissionTemplet.m_ClearCondition.m_iItemLevel <= 0 )
					goto error_proc;
			}
			break;
		case TMCT_ITEM_ENCHANT_LEVEL:
			{
				kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount = 1; // 무조건 1회

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemEnchantLevel",	kSubMissionTemplet.m_ClearCondition.m_iEnchantLevel,	-1,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemLevel",		kSubMissionTemplet.m_ClearCondition.m_iItemLevel,	-1,				goto error_proc; );

				if( kSubMissionTemplet.m_ClearCondition.m_iEnchantLevel <= 0 || kSubMissionTemplet.m_ClearCondition.m_iItemLevel <= 0 )
					goto error_proc;
			}
			break;
		case TMCT_ITEM_ENCHANT_COUNT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemEnchantCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemLevel",		kSubMissionTemplet.m_ClearCondition.m_iItemLevel,	-1,				goto error_proc; );

				if( kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount <= 0 || kSubMissionTemplet.m_ClearCondition.m_iItemLevel <= 0 )
					goto error_proc;
			}
			break;
		case TMCT_ITEM_ATTRIB:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemAttribCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemLevel",		kSubMissionTemplet.m_ClearCondition.m_iItemLevel,	-1,				goto error_proc; );

				if( kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount <= 0 || kSubMissionTemplet.m_ClearCondition.m_iItemLevel <= 0 )
					goto error_proc;
			}
			break;
		case TMCT_ITEM_RESOLVE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iItemResolveCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,		goto error_proc; );

				if( kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount <= 0 )
					goto error_proc;
			}
			break;
#endif // SERV_ADD_TITLE_CONDITION_2013_08

		default:
			{
				START_LOG( cerr, L"Sub Mission ClearCondition Type 별 읽어 오기 실패.!" )
					<< BUILD_LOG( kSubMissionTemplet.m_iSubMissionID )
					<< BUILD_LOG( kSubMissionTemplet.m_wstrDescription )
					<< BUILD_LOG( kSubMissionTemplet.m_eClearType )
					<< END_LOG;

				luaManager.EndTable();
				return false;
			}
		}

		luaManager.EndTable();
		return true;
	}

error_proc:
	START_LOG( cerr, L"Sub Mission ClearCondition Type 별 읽어 오기 실패.!" )
		<< BUILD_LOG( kSubMissionTemplet.m_iSubMissionID )
		<< BUILD_LOG( kSubMissionTemplet.m_wstrDescription )
		<< BUILD_LOG( kSubMissionTemplet.m_eClearType )
		<< END_LOG;

	luaManager.EndTable();
	return false;
}


