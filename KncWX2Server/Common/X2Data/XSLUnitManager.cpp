#include ".\xslunitmanager.h"
#include ".\XSLDamageManager.h"
#include ".\XSLDungeon.h"

ImplementSingleton( CXSLUnitManager );

CXSLUnitManager::CXSLUnitManager(void)
{
	//lua_tinker::def( "IncludeLua", IncludeLua_ );

	//lua_tinker::class_<CXSLUnitManager>("CXSLUnitManager")
	//	.def("AddUnitTemplet",			CXSLUnitManager::AddUnitTemplet_LUA)
	//	.def("AddNPCTemplet",			CXSLUnitManager::AddNPCTemplet_LUA)
	//	.def("AddNPCStat",				CXSLUnitManager::AddNPCStat_LUA)
	//	;

	//lua_tinker::decl( "g_pUnitManager", this );

	lua_tinker::class_add<CXSLUnitManager>( g_pLua, "CXSLUnitManager" );

	lua_tinker::class_def<CXSLUnitManager>( g_pLua, "AddUnitTemplet",	&CXSLUnitManager::AddUnitTemplet_LUA );
	lua_tinker::class_def<CXSLUnitManager>( g_pLua, "AddNPCTemplet",	&CXSLUnitManager::AddNPCTemplet_LUA );
	lua_tinker::class_def<CXSLUnitManager>( g_pLua, "AddNPCStat",		&CXSLUnitManager::AddNPCStat_LUA );

	lua_tinker::decl( g_pLua, "g_pUnitManager", this );
}

CXSLUnitManager::~CXSLUnitManager(void)
{
	std::map<CXSLUnit::UNIT_CLASS,CXSLUnit::UnitTemplet*>::iterator mit;

	CXSLUnit::UnitTemplet* pUnitTemplet;
	for( mit = m_UnitTempletMap.begin(); mit != m_UnitTempletMap.end(); mit++ )
	{
		pUnitTemplet = mit->second;
		SAFE_DELETE( pUnitTemplet );
	}
	m_UnitTempletMap.clear();

	std::map<NPC_UNIT_ID,NPCUnitTemplet*>::iterator mit2;
	NPCUnitTemplet* pNPCUnitTemplet;
	for( mit2 = m_NPCUnitTemplet.begin(); mit2 != m_NPCUnitTemplet.end(); mit2++ )
	{
		pNPCUnitTemplet = mit2->second;
		SAFE_DELETE( pNPCUnitTemplet );
	}
	m_NPCUnitTemplet.clear();

	std::map<NPC_UNIT_ID, NPCUnitStat*>::iterator mit3;
	NPCUnitStat* pStat;
	for( mit3 = m_NPCUnitStat.begin(); mit3 != m_NPCUnitStat.end(); mit3++ )
	{
		pStat = mit3->second;
		SAFE_DELETE( pStat );
	}
	m_NPCUnitStat.clear();
}

ImplToStringW( CXSLUnitManager )
{
	return stm_;
}

bool CXSLUnitManager::OpenScriptFile( const char* pFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	return true;
}

bool CXSLUnitManager::OpenNPCScriptFile( const char* pFileName )
{
	return OpenScriptFile( pFileName );
}

bool CXSLUnitManager::OpenNPCStatScriptFile( const char* pFileName )
{
	return OpenScriptFile( pFileName );
}

const CXSLUnit::UnitTemplet* CXSLUnitManager::GetUnitTemplet( CXSLUnit::UNIT_CLASS unitClass )
{
	std::map<CXSLUnit::UNIT_CLASS,CXSLUnit::UnitTemplet*>::const_iterator iter;
	iter = m_UnitTempletMap.find( unitClass );
	if( iter == m_UnitTempletMap.end() )
		return NULL;

	return iter->second;
}

const CXSLUnitManager::NPCUnitTemplet* CXSLUnitManager::GetNPCUnitTemplet( NPC_UNIT_ID nNPCUnitID )
{
	std::map<NPC_UNIT_ID, NPCUnitTemplet*>::const_iterator mit;
	mit = m_NPCUnitTemplet.find( nNPCUnitID );
	if( mit == m_NPCUnitTemplet.end() )
		return NULL;

	return mit->second;
}

const CXSLUnitManager::NPCUnitStat* CXSLUnitManager::GetNPCUnitStat( NPC_UNIT_ID nNPCUnitID )
{
	std::map<NPC_UNIT_ID, NPCUnitStat*>::const_iterator mit;
	mit = m_NPCUnitStat.find( nNPCUnitID );
	if( mit == m_NPCUnitStat.end() )
		return NULL;

	return mit->second;
}

bool CXSLUnitManager::AddUnitTemplet_LUA()
{
	CXSLUnit::UnitTemplet* pUnitTemplet = new CXSLUnit::UnitTemplet();
	KLuaManager luaManager( g_pLua );
	TableBind( &luaManager );

	LUA_GET_VALUE_RETURN_ENUM( luaManager, "unitType",		pUnitTemplet->m_UnitType,	CXSLUnit::UNIT_TYPE,	CXSLUnit::UT_NONE,	SAFE_DELETE(pUnitTemplet); return false; );
	LUA_GET_VALUE_RETURN_ENUM( luaManager, "unitClass",		pUnitTemplet->m_UnitClass,	CXSLUnit::UNIT_CLASS,	CXSLUnit::UC_NONE,	SAFE_DELETE(pUnitTemplet); return false; );

	//LUA_GET_VALUE_RETURN(	luaManager, L"name",			pUnitTemplet->m_Name,			L"", SAFE_DELETE(pUnitTemplet); return false; );
	//LUA_GET_VALUE_RETURN(	luaManager, L"description",		pUnitTemplet->m_Description,	L"", SAFE_DELETE(pUnitTemplet); return false; );

	// removed by wonpok, 20100525.
	// 모션분리 작업으로 더이상 사용하지 않는 항목이다. 
	//LUA_GET_VALUE_RETURN(	luaManager, L"m_MotionFile",	pUnitTemplet->m_MotionFile,			L"", SAFE_DELETE(pUnitTemplet); return false; );
	
	LUA_GET_VALUE_RETURN( luaManager, "basicWeaponItemID",	pUnitTemplet->m_BasicWeaponItemID,		0, SAFE_DELETE(pUnitTemplet); return false; );
	LUA_GET_VALUE_RETURN( luaManager, "basicHairItemID",	pUnitTemplet->m_BasicHairItemID,		0, SAFE_DELETE(pUnitTemplet); return false; );
	LUA_GET_VALUE_RETURN( luaManager, "basicFaceItemID",	pUnitTemplet->m_BasicFaceItemID,		0, SAFE_DELETE(pUnitTemplet); return false; );
	LUA_GET_VALUE_RETURN( luaManager, "basicBodyItemID",	pUnitTemplet->m_BasicBodyItemID,		0, SAFE_DELETE(pUnitTemplet); return false; );
	LUA_GET_VALUE_RETURN( luaManager, "basicLegItemID",		pUnitTemplet->m_BasicLegItemID,			0, SAFE_DELETE(pUnitTemplet); return false; );
	LUA_GET_VALUE_RETURN( luaManager, "basicHandItemID",	pUnitTemplet->m_BasicHandItemID,		0, SAFE_DELETE(pUnitTemplet); return false; );
	LUA_GET_VALUE_RETURN( luaManager, "basicFootItemID",	pUnitTemplet->m_BasicFootItemID,		0, SAFE_DELETE(pUnitTemplet); return false; );

	LUA_GET_VALUE( luaManager, "m_RavenLeftArmItemID",	pUnitTemplet->m_RavenLeftArmItemID,		0 );
	LUA_GET_VALUE( luaManager, "m_SecondWeaponItemID",	pUnitTemplet->m_SecondWeaponItemID,		0 );


	if( m_UnitTempletMap.find(pUnitTemplet->m_UnitClass) != m_UnitTempletMap.end() )
	{
		SAFE_DELETE(pUnitTemplet); 
		return false;
	}
	else
	{
		m_UnitTempletMap.insert( std::make_pair(pUnitTemplet->m_UnitClass, pUnitTemplet) );
	}

	return true;
}

bool CXSLUnitManager::AddNPCTemplet_LUA()
{
	NPCUnitTemplet* pNPCUnitTemplet = new NPCUnitTemplet();
	KLuaManager luaManager( g_pLua );
	TableBind( &luaManager );

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"NPC_ID",			pNPCUnitTemplet->m_nNPCUnitID,		NPC_UNIT_ID,	NUI_NONE,	SAFE_DELETE(pNPCUnitTemplet); return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"CLASS_TYPE",		pNPCUnitTemplet->m_ClassType,			NPC_CLASS_TYPE,	NCT_BASIC,	SAFE_DELETE(pNPCUnitTemplet); return false; );

	//LUA_GET_VALUE_RETURN(	luaManager, L"NAME",				pNPCUnitTemplet->m_Name,				L"", SAFE_DELETE(pNPCUnitTemplet); return false; );
	//LUA_GET_VALUE_RETURN(	luaManager, L"DESC",				pNPCUnitTemplet->m_Description,		L"", SAFE_DELETE(pNPCUnitTemplet); return false; );
	LUA_GET_VALUE(			luaManager, L"LUA_FILE_NAME",		pNPCUnitTemplet->m_LuaFileName,		L"" );
	LUA_GET_VALUE(			luaManager, L"LUA_FILE_NAME_UI",	pNPCUnitTemplet->m_LuaFileNameUI ,	L"" );
	LUA_GET_VALUE_RETURN(	luaManager, L"IS_NPC",				pNPCUnitTemplet->m_bIsNPC,			false, SAFE_DELETE(pNPCUnitTemplet); return false; );

	LUA_GET_VALUE(	luaManager, L"NPC_FACE_TEXTURE",	pNPCUnitTemplet->m_NPCFaceTexture,			L"" );
	LUA_GET_VALUE(	luaManager, L"NPC_FACE_TEXTURE_KEY", pNPCUnitTemplet->m_NPCFaceTextureKey,		L"" );


	if( m_NPCUnitTemplet.find(pNPCUnitTemplet->m_nNPCUnitID) != m_NPCUnitTemplet.end() )
	{
		SAFE_DELETE(pNPCUnitTemplet); 
		return false;
	}
	else
	{
		m_NPCUnitTemplet.insert( std::make_pair(pNPCUnitTemplet->m_nNPCUnitID, pNPCUnitTemplet) );
	}

	return true;
}

bool CXSLUnitManager::AddNPCStat_LUA()
{
	KLuaManager luaManager( g_pLua );
	TableBind( &luaManager );

	NPCUnitStat* pStat = new NPCUnitStat;

	LUA_GET_VALUE_ENUM(	luaManager, L"NPC_ID",			pStat->m_nNPCUnitID,	CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE );
	LUA_GET_VALUE(		luaManager, L"MAX_HP",			pStat->m_fMaxHP,					0.0f );
	LUA_GET_VALUE(		luaManager, L"ATK_PHYSIC",		pStat->m_fAtkPhysic,				0.0f );
	LUA_GET_VALUE(		luaManager, L"ATK_MAGIC",		pStat->m_fAtkMagic,					0.0f );
	LUA_GET_VALUE(		luaManager, L"DEF_PHYSIC",		pStat->m_fDefPhysic,				0.0f );
	LUA_GET_VALUE(		luaManager, L"DEF_MAGIC",		pStat->m_fDefMagic,					0.0f );


	if( S_OK == luaManager.BeginTable( L"HARD_LEVEL" ) )
	{
		LUA_GET_VALUE(		luaManager, L"HP",				pStat->m_HardLevel.m_fHP,			0.0f );
		LUA_GET_VALUE(		luaManager, L"ATK_PHYSIC",		pStat->m_HardLevel.m_fAtkPhysic,	0.0f );
		LUA_GET_VALUE(		luaManager, L"ATK_MAGIC",		pStat->m_HardLevel.m_fAtkMagic,	0.0f );
		LUA_GET_VALUE(		luaManager, L"DEF_PHYSIC",		pStat->m_HardLevel.m_fDefPhysic,	0.0f );
		LUA_GET_VALUE(		luaManager, L"DEF_MAGIC",		pStat->m_HardLevel.m_fDefMagic,	0.0f );
		LUA_GET_VALUE(		luaManager, L"EXP",				pStat->m_HardLevel.m_fExp,		0.0f );

		luaManager.EndTable();
	}

	LUA_GET_VALUE_ENUM(	luaManager, L"DAMAGE_TYPE",			pStat->m_DamageType,	CXSLDamageManager::DAMAGE_TYPE,	CXSLDamageManager::DT_PHYSIC );

	if( m_NPCUnitStat.find( pStat->m_nNPCUnitID ) != m_NPCUnitStat.end() )
	{
		SAFE_DELETE( pStat );
		return false;
	}
	else
	{
		m_NPCUnitStat[ pStat->m_nNPCUnitID ] = pStat;
	}

	return true;
}

//void CXSLUnitManager::GetNPCUnitList( std::map< CXSLUnitManager::NPC_UNIT_ID, std::wstring >& mapNpcList )
//{
//	mapNpcList.clear();
//	std::map< NPC_UNIT_ID, NPCUnitTemplet* >::iterator mit = m_NPCUnitTemplet.begin();
//	for( ; mit != m_NPCUnitTemplet.end(); mit++ )
//	{
//		NPCUnitTemplet* npcTemp = mit->second;
//		mapNpcList.insert( std::make_pair( npcTemp->m_nNPCUnitID, npcTemp->m_Description ) );
//	}
//}

//{{ 2010. 07. 12  최육사	경험치 공식 변경
CXSLUnitManager::MONSTER_TYPE_FACTOR CXSLUnitManager::GetMonsterTypeFactor( IN bool bIsAttribNpc, IN char cDifficultyLevel, IN char cMonsterGrade )
{
	if( bIsAttribNpc )
	{
		switch( cDifficultyLevel )
		{
		case CXSLDungeon::DL_NORMAL:
		case CXSLDungeon::DL_HARD:
			return MTF_LOW_ELITE_NPC;

		case CXSLDungeon::DL_EXPERT:
			return MTF_HIGH_ELITE_NPC;

		default:
			START_LOG( cerr, L"던전 난이도가 이상합니다." )
				<< BUILD_LOGc( cDifficultyLevel )
				<< END_LOG;

			return MTF_NORMAL_NPC;
		}
	}
	else if( cMonsterGrade == MG_MIDDLE_BOSS_NPC )
	{
		return MTF_MIDDLE_BOSS_NPC;
	}
	else if( cMonsterGrade == MG_BOSS_NPC )
	{
		return MTF_BOSS_NPC;
	}

	return MTF_NORMAL_NPC;
}
//}}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
CXSLUnitManager::MONSTER_TYPE_FACTOR CXSLUnitManager::GetBattleFieldMonsterTypeFactor( IN int iAttribCount, IN char cMonsterGrade )
{
	if( iAttribCount > 0 )
	{
		if( iAttribCount < 3 )
			return MTF_LOW_ELITE_NPC;
		else
			return MTF_HIGH_ELITE_NPC;
	}
	else if( cMonsterGrade == MG_MIDDLE_BOSS_NPC )
	{
		return MTF_MIDDLE_BOSS_NPC;
	}
	else if( cMonsterGrade == MG_BOSS_NPC )
	{
		return MTF_BOSS_NPC;
	}

	return MTF_NORMAL_NPC;
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 2011. 10. 24	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
bool CXSLUnitManager::IsValidMonsterGrade( IN const MONSTER_GRADE eMonsterGrade )
{
	switch( eMonsterGrade )
	{
	case MG_NORMAL_NPC:
	case MG_BOSS_NPC:
	case MG_MIDDLE_BOSS_NPC:
#ifdef SERV_ADD_MONSTER_GRADE_FOR_DEFINITELY_DROP_HP_BALL_ITEM// 작업날짜: 2013-08-28	// 박세훈
	case MG_DEFINITELY_DROP_HP_BALL:
#endif // SERV_ADD_MONSTER_GRADE_FOR_DEFINITELY_DROP_HP_BALL_ITEM
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	case MONSTER_GRADE::MG_EVENT_BOSS_NPC:
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
		return true;
	}

	return false;
}
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}


#ifdef NOT_CREATE_NPC_EXCEPT_FOR_SPECIFIC_NPC
bool CXSLUnitManager::CanCreateThisNpc( IN const int iNpcID_ )
{
	switch ( iNpcID_ )
	{
	case NUI_GOD_DEFENDER_BIG_CRAB_A:
	case NUI_GOD_DEFENDER_BIG_CRAB_B:    
	case NUI_GOD_DEFENDER_BIG_CRAB_C:
	case NUI_GOD_DEFENDER_FOREST_GUARDIAN_A:
	case NUI_GOD_DEFENDER_FOREST_GUARDIAN_B:
	case NUI_GOD_DEFENDER_FOREST_GUARDIAN_C:
	case NUI_GOD_SWORDMAN_MONKEY_WILD_A:
	case NUI_GOD_SWORDMAN_MONKEY_WILD_B:
	case NUI_GOD_SWORDMAN_MONKEY_WILD_C:
	case NUI_GOD_SWORDMAN_TREE_KNIGHT_A:
	case NUI_GOD_SWORDMAN_TREE_KNIGHT_B:
	case NUI_GOD_SWORDMAN_TREE_KNIGHT_C:
	case NUI_GOD_ARCHER_SOLDIER_CROSSBOW_A:
	case NUI_GOD_ARCHER_SOLDIER_CROSSBOW_B:
	case NUI_GOD_ARCHER_SOLDIER_CROSSBOW_C:
	case NUI_GOD_ARCHER_CACTUS_A:
	case NUI_GOD_ARCHER_CACTUS_B:
	case NUI_GOD_ARCHER_CACTUS_C:
	case NUI_GOD_MAGICIAN_MONKEY_BOMB_A:
	case NUI_GOD_MAGICIAN_MONKEY_BOMB_B:
	case NUI_GOD_MAGICIAN_MONKEY_BOMB_C:
	case NUI_GOD_MAGICIAN_WHITEMAGICIAN_A:
	case NUI_GOD_MAGICIAN_WHITEMAGICIAN_B:
	case NUI_GOD_MAGICIAN_WHITEMAGICIAN_C:
	case NUI_GOD_SPECIAL_MUSHROOM_BOMB_A: 
	case NUI_GOD_SPECIAL_MUSHROOM_BOMB_B: 
	case NUI_GOD_SPECIAL_MUSHROOM_BOMB_C: 
	case NUI_GOD_SPECIAL_ELF_HEALER_A: 
	case NUI_GOD_SPECIAL_ELF_HEALER_B: 
	case NUI_GOD_SPECIAL_ELF_HEALER_C: 
	case NUI_GOD_SPECIAL_HARPY_A: 
	case NUI_GOD_SPECIAL_HARPY_B: 
	case NUI_GOD_SPECIAL_HARPY_C: 
	case NUI_GOD_SPECIAL_RED_KNIGHT_A: 
	case NUI_GOD_SPECIAL_RED_KNIGHT_B: 
	case NUI_GOD_SPECIAL_RED_KNIGHT_C: 
	case NUI_GOD_SPECIAL_LAGUZ_A: 
	case NUI_GOD_SPECIAL_LAGUZ_B: 
	case NUI_GOD_SPECIAL_LAGUZ_C: 
	case NUI_GOD_SPECIAL_MUSHROOM_BOMB_A_HIDDEN:  
	case NUI_GOD_SPECIAL_MUSHROOM_BOMB_B_HIDDEN: 
	case NUI_GOD_SPECIAL_MUSHROOM_BOMB_C_HIDDEN: 
	case NUI_GOD_SPECIAL_ELF_HEALER_A_HIDDEN:
	case NUI_GOD_SPECIAL_ELF_HEALER_B_HIDDEN:
	case NUI_GOD_SPECIAL_ELF_HEALER_C_HIDDEN:
	case NUI_GOD_SPECIAL_HARPY_A_HIDDEN:
	case NUI_GOD_SPECIAL_HARPY_B_HIDDEN: 
	case NUI_GOD_SPECIAL_HARPY_C_HIDDEN: 
	case NUI_GOD_SPECIAL_RED_KNIGHT_A_HIDDEN:
	case NUI_GOD_SPECIAL_RED_KNIGHT_B_HIDDEN:
	case NUI_GOD_SPECIAL_RED_KNIGHT_C_HIDDEN:
	case NUI_GOD_SPECIAL_LAGUZ_A_HIDDEN:
	case NUI_GOD_SPECIAL_LAGUZ_B_HIDDEN:
	case NUI_GOD_SPECIAL_LAGUZ_C_HIDDEN:
	case NUI_GOD_GUARDIAN_GHOST_CLOWN_A:
	case NUI_GOD_GUARDIAN_GHOST_CLOWN_B: 
	case NUI_GOD_GUARDIAN_GHOST_CLOWN_C: 
	case NUI_GOD_GUARDIAN_ANCIENT_PPORU_A:
	case NUI_GOD_GUARDIAN_ANCIENT_PPORU_B:
	case NUI_GOD_GUARDIAN_ANCIENT_PPORU_C:
	case NUI_GOD_GUARDIAN_TRANVASH_A:
	case NUI_GOD_GUARDIAN_TRANVASH_B:
	case NUI_GOD_GUARDIAN_TRANVASH_C:
	case NUI_GOD_AI_NPC_LOW:
	case NUI_GOD_AI_NPC_LIME:
	case NUI_GOD_AI_NPC_PENENSIO:

	/// kimhc // 김현철 // 아래 Enum 값이 아닌 부분은
	/// 무엇이 문제인지 잘 모르겠어서 일단 이렇게 처리함
	case 1472: // NUI_DARK_VEGA_TROCK_CARD:
	case 1473: // NUI_DARK_STONE_AXE_TROCK_CARD:
	case 1474: // NUI_DARK_STEEL_TROCK_CARD:
	case 1475: // NUI_ICOOS_CARD:
	case 1476: // NUI_ANU_CARD:
	case 1477: // NUI_ERIS_CARD:
	case 1478: // NUI_ICOOS_BOSS_CARD:
	case 1479: // NUI_ANU_BOSS_CARD:
	case 1480: // NUI_ERIS_BOSS_CARD:
	case 1481: //NUI_WALDOMENATION_CARD:

	case NUI_MARS_RUBEN_CARD:
	case NUI_SOLDIER_BENDERS_ELDER_CARD:
	case NUI_SAURUS_BESMA_CARD:
	case NUI_AIRSHIP_SOLDIER_ALTERA_CARD:
	case NUI_GLITER_LANCE_PEITA_CARD:
	case NUI_GLITER_HAMMER_VELDER_CARD:
	case NUI_SHADOW_WALKER_HAMEL_CARD:
	case NUI_CHICKEN_RUBEN_CARD:
	case NUI_TREE_KNIGHT_RUBEN_CARD:
	case NUI_FAIRY_GUARDIAN_RUBEN_CARD:
	case NUI_MUSHROOM_POISON_ELDER_CARD:
	case NUI_GHOST_MAGICIAN_RAGGED_ELDER_CARD:
	case NUI_PPORU_WILLIAM_THIEF_ELDER_CARD:
	case NUI_MONKEY_APPLE_ELDER_CARD:
	case NUI_MONKEY_BOMB_ELDER_CARD:
	case NUI_CACTUS_BESMA_CARD:
	case NUI_FIRE_KOMODO_BESMA_CARD:
	case NUI_BEE_NASOD_BESMA_CARD:
	case NUI_NASOD_MINER_THIN_BESMA_CARD:
	case NUI_ARMADILLO_BESMA_CARD:
	case NUI_SCORPION_KING_BESMA_CARD:
	case NUI_BAT_SONIC_ALTERA_CARD:
	case NUI_NASOD_GUARDIAN_SMALL_ALTERA_CARD:
	case NUI_NASOD_GUARD_ALTERA_CARD:
	case NUI_NASOD_SPRAY_ICE_ALTERA_CARD:
	case NUI_NASOD_GUARDIAN_ALTERA_CARD:
	case NUI_GOLEM_STONE_RAGE_PEITA_CARD:
	case NUI_GLITER_NECROMANCER_PEITA_CARD:
	case NUI_JUBIGEE_PEITA_CARD:
	case NUI_GARGOYLE_STONE_PEITA_CARD:
	case NUI_FLY_LANTERN_PEITA_CARD:
	case NUI_GLITER_ARCHER_BOSS_PEITA_CARD:
	case NUI_COACKATRIGLE_VELDER_CARD:
	case NUI_GLITER_ARCHER_GREAT_VELDER_CARD:
	case NUI_GLITER_THIEF_GREAT_VELDER_CARD:
	case NUI_GLITER_COMMANDER_VELDER_CARD:
	case NUI_ELEMENTAL_KENAZ_VELDER_CARD:
	case NUI_DARKELF_SENTINEL_VELDER_CARD:
	case NUI_GLITER_ALCHEMYST_GREAT_VELDER_CARD:
	case NUI_GLITER_THIEF_GREAT_BOSS_VELDER_CARD:
	case NUI_GLITER_LANCE_GREAT_BOSS_VELDER_CARD:
	case NUI_IS_HAMEL_CARD:
	case NUI_HAGALAZ_HAMEL_CARD:
	case NUI_ANCIENT_ICE_CRAB_HAMEL_CARD:
	case NUI_MERMAN_HAMEL_CARD:
	case NUI_CHILLING_HEDGEHOG_HAMEL_CARD:
	case NUI_SHADOW_LINKER_HAMEL_CARD:
	case NUI_SHADOW_FORT_TRICKSTER_HAMEL_CARD:
	case NUI_SHADOW_VOIDER_HAMEL_CARD:
	case NUI_CLOE_THE_CORRUPTION_HAMEL_CARD:
	case NUI_BOBOSSE_THE_REDHAMMER_HAMEL_CARD:
	case NUI_GLITER_FRAIL_GREAT_VELDER_CARD:
	case NUI_SHADOW_DEFENDER_HAMEL_CARD:
	case NUI_SHADOW_FORT_SNIPER_HAMEL_CARD:
	case NUI_SHADOW_TRICKSTER_HAMEL_CARD_TOTEM_B:
	case NUI_AIRSHIP_SOLDIER_SWORDMAN_ALTERA_CARD:
	case NUI_SHADOW_SNIPER_HAMEL_CARD:
	case NUI_ANCIENTNASOD_GUARD_HAMEL_CARD:
	case NUI_MANTA_RES_HAMEL_CARD:
	case NUI_KNIGHT_RUNE_SLAYER_HAMEL_CARD:
	case NUI_GLITER_SHIELD_ELITE_PEITA_CARD:
	case NUI_CSM_PVP_HERO_LOW:
	case NUI_CSM_PVP_HERO_PENENSIO:
	case NUI_CSM_PVP_HERO_NOA:
	case NUI_CSM_PVP_HERO_SPIKA:
	case NUI_CSM_PVP_HERO_LIME:
	case NUI_CSM_PVP_HERO_AMELIA:
	case NUI_CSM_PVP_HERO_EDAN:
	case NUI_CSM_PVP_HERO_BALAK:
	case NUI_CSM_PVP_HERO_CODE_Q_PROTO_00:
	case NUI_CSM_PVP_HERO_APPLE:
	case NUI_CSM_KIM_SWORD:
	case NUI_VEGA_TROCK_CARD:
	case NUI_STEEL_TROCK_CARD1:
	case NUI_STONE_AXE_TROCK_CARD:
	case NUI_WING_HARPY_CARD:
	case NUI_GIANT_HARPY_CARD:
	case NUI_SHAMAN_HARPY_CARD:
	case NUI_STRONG_STEEL_TROCK_CARD:
	case NUI_STONE_AXE_TROCK_BUTRUE_CARD:
	case NUI_GOLDWING_ALELO_CARD:
	case NUI_DARKWING_OQPETE_CARD:
	case NUI_RASH_VEGA_TROCK_CARD:
	case NUI_RASH_STONE_AXE_TROCK_CARD:
	case NUI_RASH_STEEL_TROCK_CARD:
	case NUI_KARUSO_SUPPRESSION_CARD:
	case NUI_KARUSO_ASSASSIN_CARD:
	case NUI_KARUSO_FIGHTER_CARD:
	case NUI_KARUSO_WIND_MAGICIAN_CARD:
	case NUI_ANGER_SIRAPE_CARD:
	case NUI_TROCK_DOG_BOSS_CARD:
	case NUI_KARUSO_SUPPRESSION_BOSS_CARD:
	case NUI_KARUSO_ASSASSIN_BOSS_CARD:
	case NUI_KARUSO_FIGHTER_BOSS_CARD:

	case NUI_EVOKE_TRAP_LIRE:
	case NUI_EVOKE_TRAP_LIRE_MEMO:
	case NUI_RENA_ANGER_OF_ELF:
	case NUI_RENA_ANGER_OF_ELF_DELAY:
	case NUI_ADW_DISTORTION:
	case NUI_SI_HA_FERDINAND:
	case NUI_ELESIS_FIRE_BLOSSOM:
	case NUI_UFO_AISHA_DROPPER:
	case NUI_UFO_AISHA_DROPPER_MEMO:
	case NUI_REMOTE_MORTAR:
	case NUI_NASOD_WATCH_EVE:
	case NUI_NASOD_WATCH_EVE_MEMO:
	case NUI_WALLY_9TH_EVE:       
	case NUI_NASOD_ELEMENT_FIRE_EVE:
	case NUI_GUARDIAN_SP1_EVE:
	case NUI_GUARDIAN_SP1_EVE_MEMO:
	case NUI_GUARDIAN_SP2_EVE: 
	case NUI_GUARDIAN_SP2_EVE_MEMO:
	case NUI_ATOMIC_SHIELD_EVE: 
	case NUI_ENTANGLE_TRAP_RENA:     
	case NUI_UFO_AISHA:    
	case NUI_STONE_WALL_AISHA:
	case NUI_TARGETING_CHUNG_NOT_SHOW_OTHER_PLAYER:
	case NUI_TARGETING_CHUNG:
	case NUI_SUMMON_SHADOW_BLAST:
	case NUI_SSM_HERO_BALAK:
	case NUI_BOMBER_SOCKET:
	case NUI_RASH_BOMB_TROCK_SUMMON:
	case NUI_EVENT_EL:
	case NUI_PARTICLE_PRISM_START:
	case NUI_PARTICLE_PRISM_END:
	case NUI_SONOKONG_STEB_1:
	case NUI_SONOKONG_STEB_2:
		return true;

	default:
		return false;
	}	
}
#endif // NOT_CREATE_NPC_EXCEPT_FOR_SPECIFIC_NPC


