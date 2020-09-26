#include "XSLSkillTree.h"

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
ImplementRefreshSingleton( CXSLSkillTree );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
ImplementSingleton( CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2



#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
CXSLSkillTree::CXSLSkillTree(void)
{
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	lua_tinker::class_add<CXSLSkillTree>( g_pLua, "CXSLSkillTree" );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddSkillTemplet_LUA",			&CXSLSkillTree::AddSkillTemplet_LUA );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddSkillTreeTemplet_LUA",		&CXSLSkillTree::AddSkillTreeTemplet_LUA );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddSealSkillInfo",				&CXSLSkillTree::AddSealSkillInfo_LUA );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddGuildSkillTreeTemplet_LUA",	&CXSLSkillTree::AddGuildSkillTreeTemplet_LUA );	
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddSkillPowerRate_LUA",			&CXSLSkillTree::AddSkillPowerRate_LUA );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddDefaultSkill_LUA",			&CXSLSkillTree::AddDefaultSkill_LUA );

	lua_tinker::decl( g_pLua, "g_pSkillTree", this );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
}

CXSLSkillTree::~CXSLSkillTree(void)
{
	m_mapSkillTemplet.clear();
	m_mapUnitSkillTreeTemplet.clear();

	m_mapDefaultSkill.clear();
	m_setDefaultSkill.clear();
}

ImplToStringW( CXSLSkillTree )
{
	return stm_;
}

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
ImplementLuaScriptParser( CXSLSkillTree )
{
	lua_tinker::class_add<CXSLSkillTree>( GetLuaState(), "CXSLSkillTree" );
	lua_tinker::class_def<CXSLSkillTree>( GetLuaState(), "AddSkillTemplet_LUA",			&CXSLSkillTree::AddSkillTemplet_LUA );
	lua_tinker::class_def<CXSLSkillTree>( GetLuaState(), "AddSkillTreeTemplet_LUA",		&CXSLSkillTree::AddSkillTreeTemplet_LUA );
	lua_tinker::class_def<CXSLSkillTree>( GetLuaState(), "AddSealSkillInfo",				&CXSLSkillTree::AddSealSkillInfo_LUA );
	lua_tinker::class_def<CXSLSkillTree>( GetLuaState(), "AddGuildSkillTreeTemplet_LUA",	&CXSLSkillTree::AddGuildSkillTreeTemplet_LUA );	
	lua_tinker::class_def<CXSLSkillTree>( GetLuaState(), "AddSkillPowerRate_LUA",			&CXSLSkillTree::AddSkillPowerRate_LUA );
	lua_tinker::class_def<CXSLSkillTree>( GetLuaState(), "AddDefaultSkill_LUA",			&CXSLSkillTree::AddDefaultSkill_LUA );

	lua_tinker::decl( GetLuaState(), "g_pSkillTree", this );
}
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
bool CXSLSkillTree::OpenScriptFile( const char* pFileName )
{	
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );
	
	OpenScriptFilePostProcess();

	return true;
}
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

void CXSLSkillTree::OpenScriptFilePostProcess()
{
	{
		////////////////////////////////////////////////////////////////////////// 캐릭터 스킬
		// 선행스킬 정렬
		UnitSkillTreeTempletMap::iterator mit;
		SkillTreeTempletMap::iterator mit2;

		for( mit = m_mapUnitSkillTreeTemplet.begin(); mit != m_mapUnitSkillTreeTemplet.end(); ++mit )
		{
			SkillTreeTempletMap& mapSkillTreeTemplet = mit->second;

			for( mit2 = mapSkillTreeTemplet.begin(); mit2 != mapSkillTreeTemplet.end(); ++mit2 )
			{
				SkillTreeTemplet& skillTreeTemplet = mit2->second;
				skillTreeTemplet.m_vecFollowingSkill.resize(0);
			}
		}

		for( mit = m_mapUnitSkillTreeTemplet.begin(); mit != m_mapUnitSkillTreeTemplet.end(); ++mit )
		{
			int iUnitClass = mit->first;
			SkillTreeTempletMap& mapSkillTreeTemplet = mit->second;

			for( mit2 = mapSkillTreeTemplet.begin(); mit2 != mapSkillTreeTemplet.end(); ++mit2 )
			{
				int iSkillID = mit2->first;
				SkillTreeTemplet& skillTreeTemplet = mit2->second;

				if( skillTreeTemplet.m_iPrecedingSkill > 0)
				{
					const SkillTreeTemplet* pPrecedingSkillTreeTemplet = GetSkillTreeTemplet( iUnitClass, skillTreeTemplet.m_iPrecedingSkill );
					if( NULL == pPrecedingSkillTreeTemplet )
						continue;

					SkillTreeTemplet* pPrecedingSkillTemplet_NotConst = const_cast<SkillTreeTemplet*>( pPrecedingSkillTreeTemplet );
					pPrecedingSkillTemplet_NotConst->m_vecFollowingSkill.push_back( iSkillID );
				}
			}
		}
	}

	{
		////////////////////////////////////////////////////////////////////////// 길드 스킬
		// 선행스킬 정렬
		GuildSkillTreeTempletMap::iterator mit;

		//{{ 2009. 11. 26  최육사	길드스킬
		for( mit = m_mapGuildSkillTreeTemplet.begin(); mit != m_mapGuildSkillTreeTemplet.end(); ++mit )
		{
			SkillTreeTemplet& skillTreeTemplet = mit->second;
			skillTreeTemplet.m_vecFollowingSkill.resize(0);
		}

		for( mit = m_mapGuildSkillTreeTemplet.begin(); mit != m_mapGuildSkillTreeTemplet.end(); ++mit )
		{
			int iSkillID = mit->first;
			SkillTreeTemplet& skillTreeTemplet = mit->second;

			if( skillTreeTemplet.m_iPrecedingSkill > 0)
			{
				const SkillTreeTemplet* pPrecedingSkillTreeTemplet = GetGuildSkillTreeTemplet( skillTreeTemplet.m_iPrecedingSkill );
				if( NULL == pPrecedingSkillTreeTemplet )
					continue;

				SkillTreeTemplet* pPrecedingSkillTemplet_NotConst = const_cast<SkillTreeTemplet*>( pPrecedingSkillTreeTemplet );
				pPrecedingSkillTemplet_NotConst->m_vecFollowingSkill.push_back( iSkillID );
			}
		}
		//}}
	}
}

bool CXSLSkillTree::AddSkillTreeTemplet_LUA()
{
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-14	// 박세훈
	KLuaManager luaManager( GetLuaState() );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	KLuaManager luaManager( g_pLua );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

	int iSkillID = 0;
	LUA_GET_VALUE( luaManager, 		L"m_iSkillID",				iSkillID,				0 );

	if( iSkillID <= 0 )
	{
		START_LOG( cerr, L"AddSkillTreeTemplet 파싱 실패!" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;
		return false;
	}

	SkillTreeTemplet skillTreeTemplet;
	LUA_GET_VALUE( luaManager,		L"m_eUnitClass",			skillTreeTemplet.m_iUnitClass,				CXSLUnit::UC_NONE		);
	LUA_GET_VALUE( luaManager,		L"m_iMasterLevel",			skillTreeTemplet.m_iMasterSkillLevel,		0						);
	LUA_GET_VALUE( luaManager,		L"m_iRowIndex",				skillTreeTemplet.m_iTier,					-1						);
	LUA_GET_VALUE( luaManager,		L"m_iColumnIndex",			skillTreeTemplet.m_iColumn,					-1						);
	LUA_GET_VALUE( luaManager,		L"m_iPrecedingSkill",		skillTreeTemplet.m_iPrecedingSkill, 		0						);

	if( skillTreeTemplet.m_iMasterSkillLevel <= 0 || skillTreeTemplet.m_iTier <= -1 || skillTreeTemplet.m_iColumn <= -1 
		|| skillTreeTemplet.m_iUnitClass <= CXSLUnit::UC_NONE || skillTreeTemplet.m_iUnitClass >= CXSLUnit::UC_MAX )
	{
		START_LOG( cerr, L"AddSkillTreeTemplet 파싱 실패!" )
			<< BUILD_LOG( iSkillID )
			<< BUILD_LOG( skillTreeTemplet.m_iUnitClass )
			<< BUILD_LOG( skillTreeTemplet.m_iMasterSkillLevel )
			<< BUILD_LOG( skillTreeTemplet.m_iTier )
			<< BUILD_LOG( skillTreeTemplet.m_iColumn )
			<< END_LOG;
		return false;
	}

	SkillTreeTempletMap& mapSkillTreeTemplet = m_mapUnitSkillTreeTemplet[ skillTreeTemplet.m_iUnitClass ];
	mapSkillTreeTemplet[ iSkillID ] = skillTreeTemplet;

	return true;
}

bool CXSLSkillTree::AddGuildSkillTreeTemplet_LUA()
{
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-14	// 박세훈
	KLuaManager luaManager( GetLuaState() );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	KLuaManager luaManager( g_pLua );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

	int iSkillID = 0;
	LUA_GET_VALUE( luaManager, 		L"m_iSkillID",				iSkillID,				0 );

	if( iSkillID <= 0 )
	{
		START_LOG( cerr, L"AddGuildSkillTreeTemplet 파싱 실패!" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;
		return false;
	}

	SkillTreeTemplet skillTreeTemplet;
	LUA_GET_VALUE( luaManager,		L"m_iMasterLevel",			skillTreeTemplet.m_iMasterSkillLevel,		0				);
	LUA_GET_VALUE( luaManager,		L"m_iRowIndex",				skillTreeTemplet.m_iTier,					0				);
	LUA_GET_VALUE( luaManager,		L"m_iColumnIndex",			skillTreeTemplet.m_iColumn,					0				);
	LUA_GET_VALUE( luaManager,		L"m_iPrecedingSkill",		skillTreeTemplet.m_iPrecedingSkill, 		0				);

	if( skillTreeTemplet.m_iMasterSkillLevel <= 0 || skillTreeTemplet.m_iTier <= -1 || skillTreeTemplet.m_iColumn <= -1 )
	{
		START_LOG( cerr, L"AddGuildSkillTreeTemplet 파싱 실패!" )
			<< BUILD_LOG( iSkillID )
			<< BUILD_LOG( skillTreeTemplet.m_iMasterSkillLevel )
			<< BUILD_LOG( skillTreeTemplet.m_iTier )
			<< BUILD_LOG( skillTreeTemplet.m_iColumn )
			<< END_LOG;
		return false;
	}

	m_mapGuildSkillTreeTemplet.insert( std::make_pair( iSkillID, skillTreeTemplet ) );
	return true;
}

bool CXSLSkillTree::AddSkillTemplet_LUA()
{
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-14	// 박세훈
	KLuaManager luaManager( GetLuaState() );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	KLuaManager luaManager( g_pLua );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

	SkillTemplet skillTemplet;
	
	LUA_GET_VALUE_ENUM( luaManager, L"m_eType",						skillTemplet.m_eType,						SKILL_TYPE,			ST_NONE		);
	LUA_GET_VALUE( luaManager,		L"m_eID",						skillTemplet.m_iSkillID,					0								);
	LUA_GET_VALUE( luaManager,		L"m_bGuildSkill",				skillTemplet.m_bGuildSkill,					false							);
	LUA_GET_VALUE( luaManager,		L"m_bBornSealed",				skillTemplet.m_bBornSealed,					false							);
	LUA_GET_VALUE( luaManager,		L"m_iRequireLearnSkillPoint",	skillTemplet.m_iRequireLearnSkillPoint,		0								);
	LUA_GET_VALUE( luaManager,		L"m_iRequireUpgradeSkillPoint",	skillTemplet.m_iRequireUpgradeSkillPoint,	0								);
			
	//m_vecRequireCharactorLevel
	if( LoadSkillTemplet_RequireCharactorLevel( luaManager, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 필요 레벨 제한 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;

		return false;
	}

	// stat
	if( LoadSkillTemplet_Stat( luaManager, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 stat 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;

		return false;
	}

	// SKILL ABILITY
	if( LoadSkillTemplet_SA( luaManager, SA_MAX_HP_REL, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_MAX_HP_REL 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}
	if( LoadSkillTemplet_SA( luaManager, SA_ATK_PHYSIC_TO_ATK_MAGIC, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_ATK_PHYSIC_TO_ATK_MAGIC 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}
	if( LoadSkillTemplet_SA( luaManager, SA_ATK_MAGIC_TO_ATK_PHYSIC, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_ATK_MAGIC_TO_ATK_PHYSIC 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}
	if( LoadSkillTemplet_SA( luaManager, SA_DEF_PHYSIC_REL, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_DEF_PHYSIC_REL 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}
	if( LoadSkillTemplet_SA( luaManager, SA_DEF_MAGIC_REL, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_DEF_MAGIC_REL 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}
	if( LoadSkillTemplet_SA( luaManager, SA_ATK_MAGIC_REL, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_ATK_MAGIC_REL 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}
	if( LoadSkillTemplet_SA( luaManager, SA_ATK_PHYSIC_REL, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_ATK_PHYSIC_REL 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}
	if( LoadSkillTemplet_SA( luaManager, SA_EXP_GAIN_REL, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_EXP_GAIN_REL 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}
	if( LoadSkillTemplet_SA( luaManager, SA_ED_GAIN_REL, skillTemplet ) == false )
	{
		START_LOG( cerr, L"스킬 템플릿 SA_ED_GAIN_REL 파싱 오류" )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;
		return false;
	}

	std::map< int, SkillTemplet >::iterator it = m_mapSkillTemplet.find( skillTemplet.m_iSkillID );

	if( it != m_mapSkillTemplet.end() )
	{
		START_LOG( cerr, L"중복된 스킬을 템플릿에 등록하려고 함." )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< END_LOG;

		return false;
	}

	m_mapSkillTemplet[ skillTemplet.m_iSkillID ] = skillTemplet;

	m_setSkillIDList.insert( skillTemplet.m_iSkillID );

	return true;

}

const CXSLSkillTree::SkillTemplet* CXSLSkillTree::GetSkillTemplet( const int iSkillID )
{
	std::map<int, SkillTemplet>::iterator mit = m_mapSkillTemplet.find( iSkillID );
	if( mit == m_mapSkillTemplet.end() )
		return NULL;

	return &mit->second;
}

bool CXSLSkillTree::LoadSkillTemplet_RequireCharactorLevel( KLuaManager& luaManager, SkillTemplet& skillTemplet )
{
	std::vector< int > vecRequireCharactorLevel;
	
	if( luaManager.BeginTable( L"m_RequireCharacterLevel" ) == S_OK )
	{
		int iLevel = 0;
		int index = 1;
		while( S_OK == luaManager.GetValue( index, iLevel ) )
		{
			vecRequireCharactorLevel.push_back( iLevel );
			index++;
		}
		luaManager.EndTable();

		// 데이터가 하나라도 있으면 SLV_MAX_LEVEL 만큼 있어야 한다.
		if( vecRequireCharactorLevel.empty() != false && vecRequireCharactorLevel.size() != SkillTemplet::SLV_MAX_LEVEL )
		{
			START_LOG( cerr, L"스킬 레벨 만큼의 데이터가 존재하지 않습니다." )
				<< BUILD_LOG( vecRequireCharactorLevel.size() )
				<< END_LOG;

			return false;
		}
	}

	if( vecRequireCharactorLevel.empty() == false )
	{
		skillTemplet.m_vecRequireCharactorLevel = vecRequireCharactorLevel;
		return true;
	}
	else if( skillTemplet.m_eType == ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE 
		|| skillTemplet.m_eType == ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE 
		|| skillTemplet.m_eType == ST_GUILD_PASSIVE
		|| skillTemplet.m_eType == ST_RELATIONSHIP_SKILL
		|| skillTemplet.m_eType == ST_NONE
		)
	{
		return true;
	}

	return false;
}

bool CXSLSkillTree::LoadSkillTemplet_Stat( KLuaManager& luaManager, SkillTemplet& skillTemplet )
{
	std::vector< float > vecBaseHP;
	std::vector< float > vecAtkPhysic;
	std::vector< float > vecAtkMagic;
	std::vector< float > vecDefPhysic;
	std::vector< float > vecDefMagic;

	if( luaManager.BeginTable( L"m_Stat" ) == S_OK )
	{
		if( luaManager.BeginTable( L"m_BaseHP" ) == S_OK )
		{
			float fBaseHP = 0.f;
			int index = 1;
			while( S_OK == luaManager.GetValue( index, fBaseHP ) )
			{
				vecBaseHP.push_back( fBaseHP );
				index++;
			}
			luaManager.EndTable();

			// 데이터가 하나라도 있으면 SLV_MAX_LEVEL 만큼 있어야 한다.
			if( vecBaseHP.empty() != false && vecBaseHP.size() != SkillTemplet::SLV_MAX_LEVEL )
			{
				START_LOG( cerr, L"스킬 레벨 만큼의 데이터가 존재하지 않습니다." )
					<< BUILD_LOG( vecBaseHP.size() )
					<< END_LOG;

				return false;
			}
		}

		if( luaManager.BeginTable( L"m_AtkPhysic" ) == S_OK )
		{
			float fAtkPhysic= 0.f;
			int index = 1;
			while( S_OK == luaManager.GetValue( index, fAtkPhysic ) )
			{
				vecAtkPhysic.push_back( fAtkPhysic );
				index++;
			}
			luaManager.EndTable();

			// 데이터가 하나라도 있으면 SLV_MAX_LEVEL 만큼 있어야 한다.
			if( vecAtkPhysic.empty() != false && vecAtkPhysic.size() != SkillTemplet::SLV_MAX_LEVEL )
			{
				START_LOG( cerr, L"스킬 레벨 만큼의 데이터가 존재하지 않습니다." )
					<< BUILD_LOG( vecAtkPhysic.size() )
					<< END_LOG;

				return false;
			}
		}

		if( luaManager.BeginTable( L"m_AtkMagic" ) == S_OK )
		{
			float fAtkMagic = 0.f;
			int index = 1;
			while( S_OK == luaManager.GetValue( index, fAtkMagic ) )
			{
				vecAtkMagic.push_back( fAtkMagic );
				index++;
			}
			luaManager.EndTable();

			// 데이터가 하나라도 있으면 SLV_MAX_LEVEL 만큼 있어야 한다.
			if( vecAtkMagic.empty() != false && vecAtkMagic.size() != SkillTemplet::SLV_MAX_LEVEL )
			{
				START_LOG( cerr, L"스킬 레벨 만큼의 데이터가 존재하지 않습니다." )
					<< BUILD_LOG( vecAtkMagic.size() )
					<< END_LOG;

				return false;
			}
		}

		if( luaManager.BeginTable( L"m_DefPhysic" ) == S_OK )
		{
			float fDefPhysic = 0.f;
			int index = 1;
			while( S_OK == luaManager.GetValue( index, fDefPhysic ) )
			{
				vecDefPhysic.push_back( fDefPhysic );
				index++;
			}
			luaManager.EndTable();

			// 데이터가 하나라도 있으면 SLV_MAX_LEVEL 만큼 있어야 한다.
			if( vecDefPhysic.empty() != false && vecDefPhysic.size() != SkillTemplet::SLV_MAX_LEVEL )
			{
				START_LOG( cerr, L"스킬 레벨 만큼의 데이터가 존재하지 않습니다." )
					<< BUILD_LOG( vecDefPhysic.size() )
					<< END_LOG;

				return false;
			}
		}

		if( luaManager.BeginTable( L"m_DefPhysic" ) == S_OK )
		{
			float fDefPhysic = 0.f;
			int index = 1;
			while( S_OK == luaManager.GetValue( index, fDefPhysic ) )
			{
				vecDefMagic.push_back( fDefPhysic );
				index++;
			}
			luaManager.EndTable();

			// 데이터가 하나라도 있으면 SLV_MAX_LEVEL 만큼 있어야 한다.
			if( vecDefMagic.empty() != false && vecDefMagic.size() != SkillTemplet::SLV_MAX_LEVEL )
			{
				START_LOG( cerr, L"스킬 레벨 만큼의 데이터가 존재하지 않습니다." )
					<< BUILD_LOG( vecDefMagic.size() )
					<< END_LOG;

				return false;
			}
		}

		luaManager.EndTable(); // m_Stat
	}

	// 일단 초기화
	skillTemplet.m_vecStat.clear();

	// 데이터가 없다면 공간을 차지 하지 않아도 된다.
	if( vecBaseHP.empty() == true &&
		vecAtkPhysic.empty() == true &&
		vecAtkMagic.empty() == true &&
		vecDefPhysic.empty() == true &&
		vecDefMagic.empty() == true )
	{
		return true;
	}

	// 한가지라도 데이터가 있는 것이라면 넣자
	for( int iIndex = 0 ; iIndex < SkillTemplet::SLV_MAX_LEVEL ; ++iIndex )
	{
		CXSLStat::Stat kInfo;
		if( vecBaseHP.empty() == false )
		{
			kInfo.m_fBaseHP = vecBaseHP[iIndex];
		}
		if( vecAtkPhysic.empty() == false )
		{
			kInfo.m_fAtkPhysic = vecAtkPhysic[iIndex];
		}
		if( vecAtkMagic.empty() == false )
		{
			kInfo.m_fAtkMagic = vecAtkMagic[iIndex];
		}
		if( vecDefPhysic.empty() == false )
		{
			kInfo.m_fDefPhysic = vecDefPhysic[iIndex];
		}
		if( vecDefMagic.empty() == false )
		{
			kInfo.m_fDefMagic = vecDefMagic[iIndex];
		}

		skillTemplet.m_vecStat.push_back( kInfo );
	}
	return true;
}

bool CXSLSkillTree::LoadSkillTemplet_SA( KLuaManager& luaManager, SKILL_ABILITY_TYPE eType, SkillTemplet& skillTemplet )
{
	std::map< SKILL_ABILITY_TYPE, std::vector< float > >::iterator mit = skillTemplet.m_mapSkillAbility.find( eType );
	if( mit != skillTemplet.m_mapSkillAbility.end() )
	{
		START_LOG( cerr, L"중복 처리!  LoadSkillTemplet_SA" )
			<< BUILD_LOG( static_cast<int>(eType) )
			<< END_LOG;
		return false;
	}

	std::wstring wstrSkillAbility;

	switch( eType )
	{
	case SA_MAX_HP_REL:
		wstrSkillAbility = L"SA_MAX_HP_REL";
		break;
	case SA_ATK_PHYSIC_TO_ATK_MAGIC:
		wstrSkillAbility = L"SA_ATK_PHYSIC_TO_ATK_MAGIC";
		break;
	case SA_ATK_MAGIC_TO_ATK_PHYSIC:
		wstrSkillAbility = L"SA_ATK_MAGIC_TO_ATK_PHYSIC";
		break;
	case SA_DEF_PHYSIC_REL:
		wstrSkillAbility = L"SA_DEF_PHYSIC_REL";
		break;
	case SA_DEF_MAGIC_REL:
		wstrSkillAbility = L"SA_DEF_MAGIC_REL";
		break;
	case SA_ATK_MAGIC_REL:
		wstrSkillAbility = L"SA_ATK_MAGIC_REL";
		break;
	case SA_ATK_PHYSIC_REL:
		wstrSkillAbility = L"SA_ATK_PHYSIC_REL";
		break;
	case SA_EXP_GAIN_REL:
		wstrSkillAbility = L"SA_EXP_GAIN_REL";
		break;
	case SA_ED_GAIN_REL:
		wstrSkillAbility = L"SA_ED_GAIN_REL";
		break;
	default:
		{
			START_LOG( cerr, L"처리하지 않는 type 에 대한 요청입니다. 다시 한번 확인 하세요.  LoadSkillTemplet_SA" )
				<< BUILD_LOG( static_cast<int>(eType) )
				<< END_LOG;
			return false;
		}
	}

	std::vector< float > vecSkillAbility;
	if( luaManager.BeginTable( wstrSkillAbility.c_str() ) == S_OK )
	{
		float fValue = 0.f;
		int index = 1;
		while( S_OK == luaManager.GetValue( index, fValue ) )
		{
			vecSkillAbility.push_back( fValue );
			index++;
		}
		luaManager.EndTable();

		// 데이터가 하나라도 있으면 SLV_MAX_LEVEL 만큼 있어야 한다.
		if( vecSkillAbility.empty() != false && vecSkillAbility.size() != SkillTemplet::SLV_MAX_LEVEL )
		{
			START_LOG( cerr, L"스킬 레벨 만큼의 데이터가 존재하지 않습니다." )
				<< BUILD_LOG( vecSkillAbility.size() )
				<< END_LOG;
			return false;
		}
	}

	skillTemplet.m_mapSkillAbility.insert( std::make_pair( eType, vecSkillAbility ) );
	return true;
}

int CXSLSkillTree::GetMasterSkillLevel( int iUnitClass, int iSkillID )
{
	const SkillTreeTemplet* pSkillTreeTemplet = GetSkillTreeTemplet( iUnitClass, iSkillID );
	if( NULL == pSkillTreeTemplet )
		return 0;

	return pSkillTreeTemplet->m_iMasterSkillLevel;
}

int CXSLSkillTree::GetMaxSkillLevel( int iUnitClass, int iSkillID )
{
	const SkillTreeTemplet* pSkillTreeTemplet = GetSkillTreeTemplet( iUnitClass, iSkillID );
	if( NULL == pSkillTreeTemplet )
		return 0;

	return SkillTemplet::SLV_MAX_LEVEL;
}

int CXSLSkillTree::GetMasterGuildSkillLevel( int iGuildSkillID )
{
	const SkillTreeTemplet* pSkillTreeTemplet = GetGuildSkillTreeTemplet( iGuildSkillID );
	if( NULL == pSkillTreeTemplet )
		return 0;

	return pSkillTreeTemplet->m_iMasterSkillLevel;
}

bool CXSLSkillTree::AddSealSkillInfo_LUA()
{
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-14	// 박세훈
	KLuaManager luaManager( GetLuaState() );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	KLuaManager luaManager( g_pLua );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	
	int iItemID = 0;
	int iUnitClassType = CXSLUnit::UCT_NONE;
	std::set< int > setSkillID;

	LUA_GET_VALUE_RETURN( luaManager,		L"m_iItemID",			iItemID,			0,								return false	);
	LUA_GET_VALUE_RETURN_ENUM( luaManager,	L"m_eUnitClassType",	iUnitClassType,		CXSLUnit::UNIT_CLASS_TYPE,		CXSLUnit::UCT_NONE,		return false	);
	
	if( iUnitClassType >= CXSLUnit::UCT_END )
	{
		START_LOG( cerr, L"존재하지 않는 Unit Class Type 입니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iUnitClassType )
			<< END_LOG;

		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 ItemID가 봉인스킬아이템으로 설정되었습니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	if( luaManager.BeginTable( L"m_SkillID" ) == S_OK )
	{
		int iSkillID = 0;
		int index = 1;
		while( S_OK == luaManager.GetValue( index, iSkillID ) )
		{
			setSkillID.insert( iSkillID );
			index++;

			if( iSkillID <= 0 )
			{
				luaManager.EndTable();

				START_LOG( cerr, L"유효하지 않은 SkillID입니다!" )
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( iSkillID )
					<< END_LOG;

				return false;
			}

			if( IsExsitSkillID( iSkillID ) == false )
			{
				START_LOG( cerr, L"유효하지 않은 SkillID입니다!" )
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( iSkillID )
					<< END_LOG;

				return false;
			}
		}
		luaManager.EndTable();
	}

	// 아이템 - 스킬
	if( m_mapUnsealSkillItem.find( iItemID ) != m_mapUnsealSkillItem.end() )
	{
		START_LOG( cerr, L"이미 등록된 Unseal ItemID입니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	m_mapUnsealSkillItem.insert( std::make_pair( iItemID, setSkillID ) );
	
	// 아이템 - 직종
	if( m_mapUnsealSkillItem_UnitClassType.find( iItemID ) != m_mapUnsealSkillItem_UnitClassType.end() )
	{
		START_LOG( cerr, L"이미 등록된 iUnitClassType 입니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iUnitClassType )
			<< END_LOG;

		return false;
	}

	m_mapUnsealSkillItem_UnitClassType.insert( std::make_pair( iItemID, iUnitClassType ) );

	m_setUnsealSkillItem.insert( iItemID );

	std::set<int>::iterator sit = setSkillID.begin();
	for(  ; sit != setSkillID.end() ; ++sit )
	{
		std::map< int, std::vector< int > >::iterator mit = m_mapUnsealSkill.find( *sit );
		if( mit == m_mapUnsealSkill.end() )
		{
			std::vector< int > vecUnsealItemList;
			vecUnsealItemList.push_back( iItemID );
			m_mapUnsealSkill.insert( std::make_pair( *sit, vecUnsealItemList ) );
		}
		else
		{
			mit->second.push_back( iItemID );
		}
	}
	
	return true;
}

const CXSLSkillTree::SkillTreeTemplet* CXSLSkillTree::GetSkillTreeTemplet( const int iUnitClass, const int iSkillID )
{
	UnitSkillTreeTempletMap::const_iterator mit = m_mapUnitSkillTreeTemplet.find( iUnitClass );
	if( mit == m_mapUnitSkillTreeTemplet.end() )
		return NULL;

	const SkillTreeTempletMap& mapSkillTreeTemplet = mit->second;

	SkillTreeTempletMap::const_iterator itSkillTreeTemplet = mapSkillTreeTemplet.find( iSkillID );
	if( itSkillTreeTemplet == mapSkillTreeTemplet.end() )
		return NULL;

	return &itSkillTreeTemplet->second;
}

//{{ 2009. 11. 26  최육사	길드스킬
const CXSLSkillTree::SkillTreeTemplet* CXSLSkillTree::GetGuildSkillTreeTemplet( const int iGuildSkillID )
{
	SkillTreeTempletMap::iterator mit = m_mapGuildSkillTreeTemplet.find( iGuildSkillID );
	if( mit == m_mapGuildSkillTreeTemplet.end() )
		return NULL;

	return &mit->second;
}
//}}

bool CXSLSkillTree::GetCalcLevelUpIncreaseSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint )
{
	iSkillPoint = lua_tinker::call<int>( g_pLua, "CalcLevelUpIncreaseSkillPoint", (int)ucLevel );

	if( iSkillPoint <= 0 )
	{
		return false;
	}

	return true;
}

bool CXSLSkillTree::GetCalcInitSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint ) const
{
	iSkillPoint = 0;

	for( int iLevel = 2 ; iLevel <= (int)ucLevel ; ++iLevel )
	{
		int iLevelSkillPoint = lua_tinker::call<int>( g_pLua, "CalcLevelUpIncreaseSkillPoint", iLevel );
		iSkillPoint += iLevelSkillPoint;
	}

	if( iSkillPoint <= 0 )
	{
		return false;
	}

	return true;
}

bool CXSLSkillTree::IsUnitTypeDefaultSkill( IN int iSkillID )
{
	std::set<int>::iterator sit = m_setDefaultSkill.find( iSkillID );
	if( sit != m_setDefaultSkill.end() )
	{
		return true;
	}

	return false;
}

// vector 로 사용하지 않는 이유는 DB sp 에 고정 파라미터로 입력하게 되는데 오류로 인한 누락의 경우 큰 문제가 될 수 있어서 고정 변수로 전달한다.
bool CXSLSkillTree::GetUnitClassDefaultSkill( IN char cUnitClass, OUT int& iSkillID1, OUT int& iSkillID2, OUT int& iSkillID3, OUT int& iSkillID4, OUT int& iSkillID5, OUT int& iSkillID6 )
{
	int iSkillArry[6] = {0,};

	std::map< int,std::vector<int> >::iterator mit = m_mapDefaultSkill.find( static_cast<int>(cUnitClass) );
	if( mit == m_mapDefaultSkill.end() )
	{
		START_LOG( cerr, L"존재하지 않는 유닛 클래스의 기본 스킬을 얻으려 합니다." )
			<< BUILD_LOGc( cUnitClass )
			<< END_LOG;

		return false;
	}

	// 노전직
	if( CXSLUnit::IsInitNormalJob( (CXSLUnit::UNIT_CLASS)cUnitClass ) == true )
	{
		if( mit->second.size() != 2 )
		{
			START_LOG( cerr, L"유닛 클래스의 기본 스킬 갯수가 이상합니다." )
				<< BUILD_LOGc( cUnitClass )
				<< BUILD_LOG( mit->second.size() )
				<< END_LOG;

			return false;
		}
	}
	// 1차전직
	else if( CXSLUnit::IsFirstChangeJob( (CXSLUnit::UNIT_CLASS)cUnitClass ) == true )
	{
		if( mit->second.size() != 4 )
		{
			START_LOG( cerr, L"유닛 클래스의 기본 스킬 갯수가 이상합니다." )
				<< BUILD_LOGc( cUnitClass )
				<< BUILD_LOG( mit->second.size() )
				<< END_LOG;

			return false;
		}
	}
	// 2차전직
	else if( CXSLUnit::IsSecondChangeJob( (CXSLUnit::UNIT_CLASS)cUnitClass ) == true )
	{
		if( mit->second.size() != 6 )
		{
			START_LOG( cerr, L"유닛 클래스의 기본 스킬 갯수가 이상합니다." )
				<< BUILD_LOGc( cUnitClass )
				<< BUILD_LOG( mit->second.size() )
				<< END_LOG;

			return false;
		}
	}

	for( int i = 0 ; i < (int)mit->second.size() ; ++i )
	{
		iSkillArry[i] = mit->second[i];
	}
	
	// 노전직 기본 스킬
	iSkillID1 = iSkillArry[0];
	iSkillID2 = iSkillArry[1];
	// 1차 기본 스킬
	iSkillID3 = iSkillArry[2];
	iSkillID4 = iSkillArry[3];
	// 2차 기본 스킬
	iSkillID5 = iSkillArry[4];
	iSkillID6 = iSkillArry[5];

	return true;
}




// 문자열을 입력으로 받아서 숫자를 tokenize해서 하나씩 리턴해준다, 포인터는 다음 token 위치로 옮겨준다
int CXSLSkillTree::TokenizeInt( WCHAR** ppStart, WCHAR* pEnd )
{
#define IN_INT_CHARSET( c ) ( (c) == L'-' || ( (c) >= L'0' && (c) <= L'9' ) )


	if( NULL == ppStart )
		return 0;

	if( NULL == *ppStart )
		return 0;

	if( *ppStart == pEnd )
		return 0;

	WCHAR* pStop = pEnd;
	int iNumber = (int) wcstol( *ppStart, &pStop, 10 );
	*ppStart = pStop;

	// skip leading non-number character 
	while( !IN_INT_CHARSET( **ppStart ) )
	{
		if( **ppStart == L'\0' || *ppStart == pEnd )
			break;

		++ppStart;
	}

	return iNumber;
}

bool CXSLSkillTree::GetUnsealItemInfo( IN int iItemID, IN int iUnitClass, OUT std::vector<int>& vecSkillID )
{
	vecSkillID.clear();

	std::map< int, std::set<int> >::const_iterator mit = m_mapUnsealSkillItem.find( iItemID );
	if( mit == m_mapUnsealSkillItem.end() )
		return false;

	std::set<int>::const_iterator sit = mit->second.begin();
	for( ; sit != mit->second.end() ; ++sit )
	{
		const CXSLSkillTree::SkillTreeTemplet* pDelSkillTreeTemplet = SiCXSLSkillTree()->GetSkillTreeTemplet( iUnitClass, *sit );
		if( pDelSkillTreeTemplet != NULL )
		{
			vecSkillID.push_back( *sit );
		}
	}

	if( vecSkillID.empty() == true )
	{
		return false;
	}

	return true;
}

bool CXSLSkillTree::GetUnsealSkillItemList( IN int iSkillID, OUT std::vector< int >& vecItemList )
{
	vecItemList.clear();

	std::map< int, std::vector< int > >::const_iterator mit = m_mapUnsealSkill.find( iSkillID );
	if( mit == m_mapUnsealSkill.end() )
	{
        return false;
	}

	vecItemList = mit->second;
	return true;
}

bool CXSLSkillTree::IsUnsealItemID( int iItemID )
{
	KLocker lock( m_csUnsealSkillItem );

    return ( m_setUnsealSkillItem.find( iItemID ) != m_setUnsealSkillItem.end() );
}

//{{ 2009. 11. 9  최육사	엘소드2차전직
bool CXSLSkillTree::CheckUnitClassForUnsealSkill( char cUnitClass, char cUnsealSkillUnitClass )
{
	// 현재 유닛클래스와 맞는지 검사
	if( cUnitClass == cUnsealSkillUnitClass )
		return true;

	// 봉인 스킬의 상위 유닛클래스를 얻는다.
	const char cUpGradeUnitClass = CXSLUnit::GetUnitClassUpGrade( static_cast<CXSLUnit::UNIT_CLASS>(cUnsealSkillUnitClass) );
	if( cUnitClass == cUpGradeUnitClass )
		return true;

	return false;
}
//}}

/** @function : CalulateIncreaseingRate
	@brief : 1.05 (5%), 0.95 (-5%) 등의 증가율을 0.05, -0.05 등으로 반환 해주는 함수
	@param : float fRate_
*/
float CXSLSkillTree::CalulateIncreaseingRate( float fRate_ )
{
	const float STANDARD_RATE = 1.0f;

	return fRate_ - STANDARD_RATE;
}


//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
const CXSLSkillTree::SkillTreeTempletMap* CXSLSkillTree::GetSkillTreeTemplet( const int iUnitClass )
{
	UnitSkillTreeTempletMap::const_iterator mit = m_mapUnitSkillTreeTemplet.find( iUnitClass );
	if( mit == m_mapUnitSkillTreeTemplet.end() )
		return NULL;

	return &mit->second;
}

bool CXSLSkillTree::IsSealSkill( const int iSkillID )
{
	return m_mapUnsealSkill.find( iSkillID ) != m_mapUnsealSkill.end();
}

void CXSLSkillTree::AddSkillPowerRate_LUA()
{
	// 서버에서는 사용하지 않음
}

bool CXSLSkillTree::AddDefaultSkill_LUA()
{
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-14	// 박세훈
	KLuaManager luaManager( GetLuaState() );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	KLuaManager luaManager( g_pLua );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

	int iTableIndex = 1;	/// 테이블 인덱스 ( Lua는 테이블 첫번째 인덱스가 1 )
	int iValue		= -1;	/// 전달값
	int iUnitClass	= -1;	/// 유닛 클래스
	std::vector<int> vecDefaultSkill;

	int index = 1; 
	while( luaManager.GetValue( iTableIndex, iValue ) == S_OK )
	{
		if ( 1 == iTableIndex )		/// 첫번째 인덱스는 클레스 값
			iUnitClass = iValue;
		else						/// 이후로는 스킬 아이디
		{
			vecDefaultSkill.push_back( iValue );				
		}

		++iTableIndex;
	}

	if( iUnitClass <= 0 )
	{
		START_LOG( cerr, L"유닛 클래스 정보가 이상합니다." )
			<< BUILD_LOG( iUnitClass )
			<< BUILD_LOG( vecDefaultSkill.size() )
			<< END_LOG;
		return false;
	}

	if( (int)vecDefaultSkill.size() != 6 )
	{
		START_LOG( cerr, L"전달 인자 값의 수가 맞지 않습니다." )
			<< BUILD_LOG( iUnitClass )
			<< BUILD_LOG( vecDefaultSkill.size() )
			<< END_LOG;
		return false;
	}

	std::map< int, std::vector< int > >::iterator mit = m_mapDefaultSkill.find( iUnitClass );
	if( mit != m_mapDefaultSkill.end() )
	{
		START_LOG( cerr, L"유닛 클래스 정보가 중복 입니다." )
			<< BUILD_LOG( iUnitClass )
			<< BUILD_LOG( vecDefaultSkill.size() )
			<< END_LOG;
		return false;
	}

	std::vector<int> vecSkill;
	for( int i = 0 ; i < 6 ; ++i )
	{
		if(vecDefaultSkill[i] > 0 )
		{
			vecSkill.push_back( vecDefaultSkill[i] );

			std::set< int >::iterator sit = m_setDefaultSkill.find( vecDefaultSkill[i] );
			if( sit == m_setDefaultSkill.end() )
			{
				m_setDefaultSkill.insert( vecDefaultSkill[i] );
			}
		}
	}
	
	m_mapDefaultSkill.insert( std::make_pair( iUnitClass, vecSkill ) );	

	return true;
}

void CXSLSkillTree::GetUnitClassSkillList( IN char cUnitClass, OUT std::map< int, SkillTreeTemplet >& mapSkillTreeList )
{
	UnitSkillTreeTempletMap::iterator mit = m_mapUnitSkillTreeTemplet.find( static_cast<int>(cUnitClass) );
	if( mit == m_mapUnitSkillTreeTemplet.end() )
	{
		return;
	}

	mapSkillTreeList = mit->second;
}

bool CXSLSkillTree::GetCashSkillItemUnitClassType( IN int iItemID, OUT int& iUnitClassType )
{
	iUnitClassType = CXSLUnit::UCT_NONE;

	std::map< int, int >::iterator mit = m_mapUnsealSkillItem_UnitClassType.find( iItemID );
	if( mit == m_mapUnsealSkillItem_UnitClassType.end() )
	{
		return false;
	}

	iUnitClassType = mit->second;

	return true;
}


#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
CXSLSkillTree::CXSLSkillTree(void)
{
	lua_tinker::class_add<CXSLSkillTree>( g_pLua, "CXSLSkillTree" );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddSkillTemplet_LUA",			&CXSLSkillTree::AddSkillTemplet_LUA );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddSkillTreeTemplet_LUA",		&CXSLSkillTree::AddSkillTreeTemplet_LUA );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddSealSkillInfo",				&CXSLSkillTree::AddSealSkillInfo_LUA );
	lua_tinker::class_def<CXSLSkillTree>( g_pLua, "AddGuildSkillTreeTemplet_LUA",	&CXSLSkillTree::AddGuildSkillTreeTemplet_LUA );	
	

	lua_tinker::decl( g_pLua, "g_pSkillTree", this );
}

CXSLSkillTree::~CXSLSkillTree(void)
{
	m_mapSkillTemplet.clear();
	m_mapUnitSkillTreeTemplet.clear();
}

ImplToStringW( CXSLSkillTree )
{
	return stm_;
}


bool CXSLSkillTree::OpenScriptFile( const char* pFileName )
{	
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	OpenScriptFilePostProcess();
	return true;
}

void CXSLSkillTree::OpenScriptFilePostProcess()
{
	{
		////////////////////////////////////////////////////////////////////////// 캐릭터 스킬
		// 선행스킬 정렬
		UnitSkillTreeTempletMap::iterator mit;
		SkillTreeTempletMap::iterator mit2;

		for( mit = m_mapUnitSkillTreeTemplet.begin(); mit != m_mapUnitSkillTreeTemplet.end(); ++mit )
		{
			SkillTreeTempletMap& mapSkillTreeTemplet = mit->second;

			for( mit2 = mapSkillTreeTemplet.begin(); mit2 != mapSkillTreeTemplet.end(); ++mit2 )
			{
				SkillTreeTemplet& skillTreeTemplet = mit2->second;
				skillTreeTemplet.m_vecFollowingSkill.resize(0);
			}
		}

		for( mit = m_mapUnitSkillTreeTemplet.begin(); mit != m_mapUnitSkillTreeTemplet.end(); ++mit )
		{
			int iUnitClass = mit->first;
			SkillTreeTempletMap& mapSkillTreeTemplet = mit->second;

			for( mit2 = mapSkillTreeTemplet.begin(); mit2 != mapSkillTreeTemplet.end(); ++mit2 )
			{
				int iSkillID = mit2->first;
				SkillTreeTemplet& skillTreeTemplet = mit2->second;

				for( u_int ui = 0; ui < skillTreeTemplet.m_vecPrecedingSkill.size(); ++ui )
				{
					int iPrecedingSkillID = skillTreeTemplet.m_vecPrecedingSkill[ui];

					const SkillTreeTemplet* pPrecedingSkillTreeTemplet = GetSkillTreeTemplet( iUnitClass, iPrecedingSkillID );
					if( NULL == pPrecedingSkillTreeTemplet )
						continue;

					SkillTreeTemplet* pPrecedingSkillTemplet_NotConst = const_cast<SkillTreeTemplet*>( pPrecedingSkillTreeTemplet );
					pPrecedingSkillTemplet_NotConst->m_vecFollowingSkill.push_back( iSkillID );
				}
			}
		}
	}

	{
		////////////////////////////////////////////////////////////////////////// 길드 스킬
		// 선행스킬 정렬
		GuildSkillTreeTempletMap::iterator mit;

		//{{ 2009. 11. 26  최육사	길드스킬
		for( mit = m_mapGuildSkillTreeTemplet.begin(); mit != m_mapGuildSkillTreeTemplet.end(); ++mit )
		{
			SkillTreeTemplet& skillTreeTemplet = mit->second;
			skillTreeTemplet.m_vecFollowingSkill.resize(0);
		}
		
		for( mit = m_mapGuildSkillTreeTemplet.begin(); mit != m_mapGuildSkillTreeTemplet.end(); ++mit )
		{
			int iSkillID = mit->first;
			SkillTreeTemplet& skillTreeTemplet = mit->second;

			for( u_int ui = 0; ui < skillTreeTemplet.m_vecPrecedingSkill.size(); ++ui )
			{
				int iPrecedingSkillID = skillTreeTemplet.m_vecPrecedingSkill[ui];

				const SkillTreeTemplet* pPrecedingSkillTreeTemplet = GetGuildSkillTreeTemplet( iPrecedingSkillID );
				if( NULL == pPrecedingSkillTreeTemplet )
					continue;

				SkillTreeTemplet* pPrecedingSkillTemplet_NotConst = const_cast<SkillTreeTemplet*>( pPrecedingSkillTreeTemplet );
				pPrecedingSkillTemplet_NotConst->m_vecFollowingSkill.push_back( iSkillID );
			}
		}
		//}}
	}
}

bool CXSLSkillTree::AddSkillTreeTemplet_LUA()
{
	KLuaManager luaManager( g_pLua );

	int iUnitClass = 0;
	int iSkillID = 0;
	LUA_GET_VALUE( luaManager, 		L"UNIT_CLASS",				iUnitClass,				0 );
	LUA_GET_VALUE( luaManager, 		L"SKILL_ID",				iSkillID,				0 );


	SkillTreeTemplet skillTreeTemplet;
	LUA_GET_VALUE( luaManager,		L"MAX_SKILL_LEVEL",			skillTreeTemplet.m_iMaxSkillLevel,		0				);
	LUA_GET_VALUE( luaManager,		L"TIER_INDEX",				skillTreeTemplet.m_iTier,				0				);
	LUA_GET_VALUE( luaManager,		L"INDEX_IN_TIER",			skillTreeTemplet.m_iIndexInTier,		0				);


	std::wstring wstrPrecedingSkill = L"";
	LUA_GET_VALUE( luaManager,		L"m_vecPrecedingSkill",		wstrPrecedingSkill, 		L""		);

	if( wstrPrecedingSkill.empty() == false )
	{
		WCHAR* pBegin = const_cast<WCHAR*>(wstrPrecedingSkill.c_str());
		WCHAR* pEnd = pBegin + wstrPrecedingSkill.length();

		while( true )
		{
			int iSkillID = TokenizeInt( &pBegin, pEnd );
			if( iSkillID > 0 )
				skillTreeTemplet.m_vecPrecedingSkill.push_back( iSkillID );
			else 
				break;
		}
	}

	SkillTreeTempletMap& mapSkillTreeTemplet = m_mapUnitSkillTreeTemplet[ iUnitClass ];
	mapSkillTreeTemplet[ iSkillID ] = skillTreeTemplet;

	return true;
}

bool CXSLSkillTree::AddGuildSkillTreeTemplet_LUA()
{
	KLuaManager luaManager( g_pLua );
	
	int iSkillID = 0;
	LUA_GET_VALUE( luaManager, 		L"SKILL_ID",				iSkillID,				0 );


	SkillTreeTemplet skillTreeTemplet;
	LUA_GET_VALUE( luaManager,		L"MAX_SKILL_LEVEL",			skillTreeTemplet.m_iMaxSkillLevel,		0				);
	LUA_GET_VALUE( luaManager,		L"TIER_INDEX",				skillTreeTemplet.m_iTier,				0				);
	LUA_GET_VALUE( luaManager,		L"INDEX_IN_TIER",			skillTreeTemplet.m_iIndexInTier,		0				);


	std::wstring wstrPrecedingSkill = L"";
	LUA_GET_VALUE( luaManager,		L"m_vecPrecedingSkill",		wstrPrecedingSkill, 		L""		);

	if( wstrPrecedingSkill.empty() == false )
	{
		WCHAR* pBegin = const_cast<WCHAR*>(wstrPrecedingSkill.c_str());
		WCHAR* pEnd = pBegin + wstrPrecedingSkill.length();

		while( true )
		{
			int iSkillID = TokenizeInt( &pBegin, pEnd );
			if( iSkillID > 0 )
				skillTreeTemplet.m_vecPrecedingSkill.push_back( iSkillID );
			else 
				break;
		}
	}

	m_mapGuildSkillTreeTemplet.insert( std::make_pair( iSkillID, skillTreeTemplet ) );
	return true;
}

bool CXSLSkillTree::AddSkillTemplet_LUA()
{
	KLuaManager luaManager( g_pLua );

	SkillTemplet skillTemplet;

	//{{ 2009. 11. 25  최육사	길드스킬
	LUA_GET_VALUE( luaManager,		L"m_bGuildSkill",			skillTemplet.m_bGuildSkill,					false							);
#ifdef GUILD_SKILL_TEST
#else
	if( skillTemplet.m_bGuildSkill )
		return true;
#endif GUILD_SKILL_TEST
	//}}

	LUA_GET_VALUE_ENUM( luaManager, L"m_eType",					skillTemplet.m_eType,						SKILL_TYPE,			ST_NONE		);
	LUA_GET_VALUE( luaManager,		L"m_eID",					skillTemplet.m_iSkillID,					0								);
	LUA_GET_VALUE( luaManager,		L"m_iSkillLevel",			skillTemplet.m_iSkillLevel,					1								);
	LUA_GET_VALUE( luaManager,		L"m_fSkillCoolTime",		skillTemplet.m_fSkillCoolTime,				0.f								);
	LUA_GET_VALUE( luaManager,		L"m_fMPConsumption",		skillTemplet.m_fMPConsumption,				0.f								);

	//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	LUA_GET_VALUE( luaManager,		"CB_CONSUMPTION",			skillTemplet.m_usCBConsumption,				0 );
#endif	NEW_CHARACTER_CHUNG
	//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

	LUA_GET_VALUE( luaManager,		L"m_Name",					skillTemplet.m_Name,						L""								);
	LUA_GET_VALUE( luaManager,		L"m_Desc",					skillTemplet.m_Desc,						L""								);

	LUA_GET_VALUE( luaManager,		L"m_bBornSealed",			skillTemplet.m_bBornSealed,					false							);


	if( luaManager.BeginTable( L"m_Stat" ) == S_OK )
	{
		LUA_GET_VALUE( luaManager,		L"m_fBaseHP",			skillTemplet.m_Stat.m_fBaseHP,				0.f		);
		LUA_GET_VALUE( luaManager,		L"m_fAtkPhysic",		skillTemplet.m_Stat.m_fAtkPhysic,			0.f		);
		LUA_GET_VALUE( luaManager,		L"m_fAtkMagic",			skillTemplet.m_Stat.m_fAtkMagic,			0.f		);
		LUA_GET_VALUE( luaManager,		L"m_fDefPhysic",		skillTemplet.m_Stat.m_fDefPhysic,			0.f		);
		LUA_GET_VALUE( luaManager,		L"m_fDefMagic",			skillTemplet.m_Stat.m_fDefMagic,			0.f		);

		luaManager.EndTable(); // m_Stat
	}



	float fValue = 0.f;

	if( S_OK == luaManager.GetValue( L"SA_EFFECTIVE_TIME", fValue ) )					skillTemplet.m_mapSkillAbility[ SA_EFFECTIVE_TIME ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_EFFECTIVE_COUNT", fValue ) )          		skillTemplet.m_mapSkillAbility[ SA_EFFECTIVE_COUNT ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_SKILL_ID", fValue ) )                 		skillTemplet.m_mapSkillAbility[ SA_SKILL_ID ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_COMBO_ID", fValue ) )                 		skillTemplet.m_mapSkillAbility[ SA_COMBO_ID ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_EFFECTIVE_RATE", fValue ) )           		skillTemplet.m_mapSkillAbility[ SA_EFFECTIVE_RATE ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_REL", fValue ) )                  		skillTemplet.m_mapSkillAbility[ SA_ATK_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_PHYSIC_REL", fValue ) )           		skillTemplet.m_mapSkillAbility[ SA_ATK_PHYSIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_MAGIC_REL", fValue ) )            		skillTemplet.m_mapSkillAbility[ SA_ATK_MAGIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_DEF_PHYSIC_REL", fValue ) )           		skillTemplet.m_mapSkillAbility[ SA_DEF_PHYSIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_DEF_MAGIC_REL", fValue ) )            		skillTemplet.m_mapSkillAbility[ SA_DEF_MAGIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TEAM_ATK_PHYSIC_REL", fValue ) )      		skillTemplet.m_mapSkillAbility[ SA_TEAM_ATK_PHYSIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TEAM_ATK_MAGIC_REL", fValue ) )       		skillTemplet.m_mapSkillAbility[ SA_TEAM_ATK_MAGIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TEAM_DEF_PHYSIC_REL", fValue ) )      		skillTemplet.m_mapSkillAbility[ SA_TEAM_DEF_PHYSIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TEAM_DEF_MAGIC_REL", fValue ) )       		skillTemplet.m_mapSkillAbility[ SA_TEAM_DEF_MAGIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_DAMAGE_REL", fValue ) )               		skillTemplet.m_mapSkillAbility[ SA_DAMAGE_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_DAMAGED_REL", fValue ) )              		skillTemplet.m_mapSkillAbility[ SA_DAMAGED_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_UNFIXED_DEFENSE_REL", fValue ) )      		skillTemplet.m_mapSkillAbility[ SA_UNFIXED_DEFENSE_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_SUMMON_ATK_REL", fValue ) )           		skillTemplet.m_mapSkillAbility[ SA_SUMMON_ATK_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MAX_HP_ABS", fValue ) )               		skillTemplet.m_mapSkillAbility[ SA_MAX_HP_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MAX_HP_REL", fValue ) )               		skillTemplet.m_mapSkillAbility[ SA_MAX_HP_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MAX_MP_ABS", fValue ) )               		skillTemplet.m_mapSkillAbility[ SA_MAX_MP_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MAX_MP_REL", fValue ) )               		skillTemplet.m_mapSkillAbility[ SA_MAX_MP_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TELEPORT_RANGE_ABS", fValue ) )       		skillTemplet.m_mapSkillAbility[ SA_TELEPORT_RANGE_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_CHARGING_TIME", fValue ) )            		skillTemplet.m_mapSkillAbility[ SA_CHARGING_TIME ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_HP_REL_TO_MP_ABS_1", fValue ) )       		skillTemplet.m_mapSkillAbility[ SA_HP_REL_TO_MP_ABS_1 ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_HP_REL_TO_MP_ABS_2", fValue ) )       		skillTemplet.m_mapSkillAbility[ SA_HP_REL_TO_MP_ABS_2 ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MP_CONSUME_REL", fValue ) )           		skillTemplet.m_mapSkillAbility[ SA_MP_CONSUME_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MP_GAIN_ON_HIT_REL", fValue ) )       		skillTemplet.m_mapSkillAbility[ SA_MP_GAIN_ON_HIT_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_SCOPE_ABS", fValue ) )                		skillTemplet.m_mapSkillAbility[ SA_SCOPE_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_PROJECTILE_FIRE_COUNT", fValue ) )    		skillTemplet.m_mapSkillAbility[ SA_PROJECTILE_FIRE_COUNT ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_PROJECTILE_RANGE_REL", fValue ) )     		skillTemplet.m_mapSkillAbility[ SA_PROJECTILE_RANGE_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_PROJECTILE_SIZE", fValue ) )          		skillTemplet.m_mapSkillAbility[ SA_PROJECTILE_SIZE ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_PROJECTILE_PIERCING", fValue ) )      		skillTemplet.m_mapSkillAbility[ SA_PROJECTILE_PIERCING ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MOVE_SPEED", fValue ) )               		skillTemplet.m_mapSkillAbility[ SA_MOVE_SPEED ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_JUMP_SPEED", fValue ) )                   	skillTemplet.m_mapSkillAbility[ SA_JUMP_SPEED ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_PHYSIC_TO_ATK_MAGIC", fValue ) )      	skillTemplet.m_mapSkillAbility[ SA_ATK_PHYSIC_TO_ATK_MAGIC ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_MAGIC_TO_ATK_PHYSIC", fValue ) )      	skillTemplet.m_mapSkillAbility[ SA_ATK_MAGIC_TO_ATK_PHYSIC ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_READY_TIME_REL", fValue ) )               	skillTemplet.m_mapSkillAbility[ SA_READY_TIME_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_FORCE_DOWN_REL", fValue ) )               	skillTemplet.m_mapSkillAbility[ SA_FORCE_DOWN_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_FORCE_DOWN_MELEE_REL", fValue ) )         	skillTemplet.m_mapSkillAbility[ SA_FORCE_DOWN_MELEE_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_FORCE_DOWN_RANGE_REL", fValue ) )         	skillTemplet.m_mapSkillAbility[ SA_FORCE_DOWN_RANGE_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_FORCE_DOWN_ABS", fValue ) )               	skillTemplet.m_mapSkillAbility[ SA_FORCE_DOWN_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MP_REGENERATION_ABS", fValue ) )          	skillTemplet.m_mapSkillAbility[ SA_MP_REGENERATION_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MP_INTAKE_ABS", fValue ) )                	skillTemplet.m_mapSkillAbility[ SA_MP_INTAKE_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TARGET_ANI_SPEED", fValue ) )             	skillTemplet.m_mapSkillAbility[ SA_TARGET_ANI_SPEED ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TARGET_MOVE_SPEED", fValue ) )            	skillTemplet.m_mapSkillAbility[ SA_TARGET_MOVE_SPEED ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TARGET_JUMP_SPEED", fValue ) )            	skillTemplet.m_mapSkillAbility[ SA_TARGET_JUMP_SPEED ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TARGET_ATK_REL", fValue ) )               	skillTemplet.m_mapSkillAbility[ SA_TARGET_ATK_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_HP_INTAKE_REL_DAMAGE", fValue ) )         	skillTemplet.m_mapSkillAbility[ SA_HP_INTAKE_REL_DAMAGE ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_CRITICAL_DAM_ABS", fValue ) )         	skillTemplet.m_mapSkillAbility[ SA_ATK_CRITICAL_DAM_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_CRITICAL_RATE_ABS", fValue ) )        	skillTemplet.m_mapSkillAbility[ SA_ATK_CRITICAL_RATE_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_DEF_CRITICAL_DAM_ABS", fValue ) )         	skillTemplet.m_mapSkillAbility[ SA_DEF_CRITICAL_DAM_ABS ] = fValue;
	//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	if( S_OK == luaManager.GetValue( L"SA_DEF_CRITICAL_RATE_ABS", fValue ) )         	skillTemplet.m_mapSkillAbility[ SA_DEF_CRITICAL_RATE_ABS ] = fValue;
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
	//}} 2011-04 에 패치될 청 캐시 스킬
	if( S_OK == luaManager.GetValue( L"SA_BACK_SPEED_X", fValue ) )                 	skillTemplet.m_mapSkillAbility[ SA_BACK_SPEED_X ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ITEM_SPECIAL_ABILITY_REL", fValue ) )     	skillTemplet.m_mapSkillAbility[ SA_ITEM_SPECIAL_ABILITY_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MP_GAIN_GET_HIT_REL", fValue ) )          	skillTemplet.m_mapSkillAbility[ SA_MP_GAIN_GET_HIT_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_SOUL_GAIN_ON_HIT_REL", fValue ) )         	skillTemplet.m_mapSkillAbility[ SA_SOUL_GAIN_ON_HIT_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_SOUL_GAIN_GET_HIT_REL", fValue ) )        	skillTemplet.m_mapSkillAbility[ SA_SOUL_GAIN_GET_HIT_REL ] = fValue;

	if( S_OK == luaManager.GetValue( L"SA_HP_GAIN_REL_MAX_HP", fValue ) )           	skillTemplet.m_mapSkillAbility[ SA_HP_GAIN_REL_MAX_HP ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ED_GAIN_REL", fValue ) )                  	skillTemplet.m_mapSkillAbility[ SA_ED_GAIN_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_PHYSIC_PLUS_ATK_MAGIC_REL", fValue ) )	skillTemplet.m_mapSkillAbility[ SA_ATK_PHYSIC_PLUS_ATK_MAGIC_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_EXP_GAIN_REL", fValue ) )                 	skillTemplet.m_mapSkillAbility[ SA_EXP_GAIN_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_DAMAGE_ABSORB_REL_MAX_HP", fValue ) )     	skillTemplet.m_mapSkillAbility[ SA_DAMAGE_ABSORB_REL_MAX_HP ] = fValue;


	if( S_OK == luaManager.GetValue( L"SA_STRONG_MIND_ACTIVE_REL", fValue ) )     		skillTemplet.m_mapSkillAbility[ SA_STRONG_MIND_ACTIVE_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_STRONG_MIND_SPECIAL_ACTIVE_REL", fValue ) )   skillTemplet.m_mapSkillAbility[ SA_STRONG_MIND_SPECIAL_ACTIVE_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_MP_RETAKE_ABS", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_MP_RETAKE_ABS ] = fValue;

	if( S_OK == luaManager.GetValue( L"SA_RESIST_FIRE_ABS", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_RESIST_FIRE_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_RESIST_WATER_ABS", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_RESIST_WATER_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_RESIST_GREEN_ABS", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_RESIST_GREEN_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_RESIST_WIND_ABS", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_RESIST_WIND_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_RESIST_LIGHT_ABS", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_RESIST_LIGHT_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_RESIST_DARK_ABS", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_RESIST_DARK_ABS ] = fValue;

	if( S_OK == luaManager.GetValue( L"SA_ATK_MAGIC_FIRST_REL", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_ATK_MAGIC_FIRST_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_MAGIC_SECOND_REL", fValue ) )     		skillTemplet.m_mapSkillAbility[ SA_ATK_MAGIC_SECOND_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_ATK_MAGIC_THIRD_REL", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_ATK_MAGIC_THIRD_REL ] = fValue;


	if( S_OK == luaManager.GetValue( L"SA_REMOTE_SPEED_ABS", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_REMOTE_SPEED_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_DAMAGE_TYPE_CHANGE", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_DAMAGE_TYPE_CHANGE ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_DAMAGE_TIME", fValue ) )     					skillTemplet.m_mapSkillAbility[ SA_DAMAGE_TIME ] = fValue;

	// kimhc // 2010-12-14 // 신스킬 누락된 것
	if( S_OK == luaManager.GetValue( L"SA_RECOVER_MP_ABS", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_RECOVER_MP_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TARGET_MANA", fValue ) )     					skillTemplet.m_mapSkillAbility[ SA_TARGET_MANA ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TARGET_MANA_REL", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_TARGET_MANA_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_SIZE_ABS", fValue ) )     					skillTemplet.m_mapSkillAbility[ SA_SIZE_ABS ] = fValue;

	//#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.24] 
	if( S_OK == luaManager.GetValue( L"SA_REFLECT_REL", fValue ) )     					skillTemplet.m_mapSkillAbility[ SA_REFLECT_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_EL_DEFENCE_ABS", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_EL_DEFENCE_ABS ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TARGET_HP_REL", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_TARGET_HP_REL ] = fValue;
	if( S_OK == luaManager.GetValue( L"SA_TARGET_HITRATE_REL", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_TARGET_HITRATE_REL ] = fValue;
	//#endif NEW_SKILL_2010_11

	//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
	if( S_OK == luaManager.GetValue( L"SA_CHARGE_CANNON_BALL", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_CHARGE_CANNON_BALL ] = fValue;
	//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

	//{{ kimhc // 2011.1.14 // 청 1차 전직
	if( S_OK == luaManager.GetValue( L"SA_FIRE_DEFENCE_ABS", fValue ) )					skillTemplet.m_mapSkillAbility[ SA_FIRE_DEFENCE_ABS ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_WATER_DEFENCE_ABS", fValue ) )				skillTemplet.m_mapSkillAbility[ SA_WATER_DEFENCE_ABS ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_NATURE_DEFENCE_ABS", fValue ) )				skillTemplet.m_mapSkillAbility[ SA_NATURE_DEFENCE_ABS ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_WIND_DEFENCE_ABS", fValue ) )					skillTemplet.m_mapSkillAbility[ SA_WIND_DEFENCE_ABS ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_LIGHT_DEFENCE_ABS", fValue ) )				skillTemplet.m_mapSkillAbility[ SA_LIGHT_DEFENCE_ABS ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_DARK_DEFENCE_ABS", fValue ) )					skillTemplet.m_mapSkillAbility[ SA_DARK_DEFENCE_ABS ]			= fValue;

	if( S_OK == luaManager.GetValue( L"SA_EDT_FIRE_RESIST_REL", fValue ) )				skillTemplet.m_mapSkillAbility[ SA_EDT_FIRE_RESIST_REL ]		= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EDT_FROZEN_RESIST_REL", fValue ) )			skillTemplet.m_mapSkillAbility[ SA_EDT_FROZEN_RESIST_REL ]		= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EDT_ICE_RESIST_REL", fValue ) )				skillTemplet.m_mapSkillAbility[ SA_EDT_ICE_RESIST_REL ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EDT_POISON_RESIST_REL", fValue ) )			skillTemplet.m_mapSkillAbility[ SA_EDT_POISON_RESIST_REL ]		= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EDT_Y_PRESSED_RESIST_REL", fValue ) )			skillTemplet.m_mapSkillAbility[ SA_EDT_Y_PRESSED_RESIST_REL ]	= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EDT_LEG_WOUND_RESIST_REL", fValue ) )			skillTemplet.m_mapSkillAbility[ SA_EDT_LEG_WOUND_RESIST_REL ]	= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EDT_STUN_RESIST_REL", fValue ) )				skillTemplet.m_mapSkillAbility[ SA_EDT_STUN_RESIST_REL ]		= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EDT_CURSE_RESIST_REL", fValue ) )				skillTemplet.m_mapSkillAbility[ SA_EDT_CURSE_RESIST_REL ]		= fValue;

	if( S_OK == luaManager.GetValue( L"SA_EXPAND_CB", fValue ) )						skillTemplet.m_mapSkillAbility[ SA_EXPAND_CB ]					= fValue;
	if( S_OK == luaManager.GetValue( L"SA_ANI_SPEED", fValue ) )						skillTemplet.m_mapSkillAbility[ SA_ANI_SPEED ]					= fValue;
	//}} kimhc // 2011.1.14 // 청 1차 전직

	//{{ kimhc // 2011-04-01 // 2011-04 에 패치될 청 캐시 스킬
	if ( S_OK == luaManager.GetValue( L"SA_ACCURACY_PERCENT_ABS", fValue ) )			skillTemplet.m_mapSkillAbility[ SA_ACCURACY_PERCENT_ABS ]		= fValue;
	//}} kimhc // 2011-04-01 // 2011-04 에 패치될 청 캐시 스킬
#ifdef SERV_CHUNG_SECOND_CLASS_CHANGE
	if ( S_OK == luaManager.GetValue( L"SA_CRITICAL_EVASION_REL", fValue ) )			skillTemplet.m_mapSkillAbility[ SA_CRITICAL_EVASION_REL ]		= fValue;
#endif SERV_CHUNG_SECOND_CLASS_CHANGE

#ifdef SERV_RAVEN_WEAPON_TAKER
	if( S_OK == luaManager.GetValue( L"SA_EVASION_REL", fValue ) )     					skillTemplet.m_mapSkillAbility[ SA_EVASION_REL ]				= fValue;
	if( S_OK == luaManager.GetValue( L"SA_REDUCE_EDT_TIME_REL", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_REDUCE_EDT_TIME_REL ]		= fValue;
	if( S_OK == luaManager.GetValue( L"SA_OVERHEAT_HP", fValue ) )     					skillTemplet.m_mapSkillAbility[ SA_OVERHEAT_HP ]				= fValue;
#endif SERV_RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
	if( S_OK == luaManager.GetValue( L"SA_SCOPE_ABS_HYPER", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_SCOPE_ABS_HYPER ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EFFECTIVE_TIME_HYPER", fValue ) )     		skillTemplet.m_mapSkillAbility[ SA_EFFECTIVE_TIME_HYPER ]		= fValue;
	if( S_OK == luaManager.GetValue( L"SA_AIR_MOVE_COUNT", fValue ) )     				skillTemplet.m_mapSkillAbility[ SA_AIR_MOVE_COUNT ]				= fValue;
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	if( S_OK == luaManager.GetValue( L"SA_NOT_CANNONBALL_REL", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_NOT_CANNONBALL_REL ]				= fValue;
	if( S_OK == luaManager.GetValue( L"SA_INSERT_CANNONBALL_REL", fValue ) )     		skillTemplet.m_mapSkillAbility[ SA_INSERT_CANNONBALL_REL ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_BE_SHOT_CANNONBALL_REL", fValue ) )     		skillTemplet.m_mapSkillAbility[ SA_BE_SHOT_CANNONBALL_REL ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_CANNONBALL_DAMAGE_UP_REL", fValue ) )     	skillTemplet.m_mapSkillAbility[ SA_CANNONBALL_DAMAGE_UP_REL ]			= fValue;
	if( S_OK == luaManager.GetValue( L"SA_FORCE_DOWN_REL_DAMAGE_RATE_BASE", fValue ) )  skillTemplet.m_mapSkillAbility[ SA_FORCE_DOWN_REL_DAMAGE_RATE_BASE ]	= fValue;
	if( S_OK == luaManager.GetValue( L"SA_SIEGE_SHELLING_REL", fValue ) )     			skillTemplet.m_mapSkillAbility[ SA_SIEGE_SHELLING_REL ]					= fValue;
	if( S_OK == luaManager.GetValue( L"SA_EFFECTIVE_DAMAGED_ATTACK_BASE", fValue ) )    skillTemplet.m_mapSkillAbility[ SA_EFFECTIVE_DAMAGED_ATTACK_BASE ]		= fValue;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	if( S_OK == luaManager.GetValue( L"SA_FIRST_HYPER_EFFECTIVE", fValue ) )			skillTemplet.m_mapSkillAbility[ SA_FIRST_HYPER_EFFECTIVE ]		= fValue;
	if( S_OK == luaManager.GetValue( L"SA_SECOND_HYPER_EFFECTIVE", fValue ) )     		skillTemplet.m_mapSkillAbility[ SA_SECOND_HYPER_EFFECTIVE ]		= fValue;
	if( S_OK == luaManager.GetValue( L"SA_THIRD_HYPER_EFFECTIVE", fValue ) )			skillTemplet.m_mapSkillAbility[ SA_THIRD_HYPER_EFFECTIVE ]		= fValue;
#endif SERV_CHUNG_TACTICAL_TROOPER

	SkillKey skillKey;
	skillKey.m_iSkillID		= skillTemplet.m_iSkillID;
	skillKey.m_iSkillLevel	= skillTemplet.m_iSkillLevel;
	SkillTempletMap::iterator it = m_mapSkillTemplet.find( skillKey );

	if( it != m_mapSkillTemplet.end() )
	{
		START_LOG( cerr, L"중복된 스킬을 템플릿에 등록하려고 함." )
			<< BUILD_LOG( skillTemplet.m_iSkillID )
			<< BUILD_LOG( skillTemplet.m_iSkillLevel )
			<< END_LOG;

		return false;
	}

	m_mapSkillTemplet[ skillKey ] = skillTemplet;

	//{{ 2009. 8. 4  최육사		skill id list
	m_setSkillIDList.insert( skillTemplet.m_iSkillID );
	//}}

	return true;

}

const CXSLSkillTree::SkillTemplet* CXSLSkillTree::GetSkillTemplet( const int iSkillID, const int iSkillLevel )
{
SkillTempletMap::iterator mit = m_mapSkillTemplet.find( SkillKey( iSkillID, iSkillLevel ) );
if( mit == m_mapSkillTemplet.end() )
return NULL;

return &mit->second;
}

int CXSLSkillTree::GetMaxSkillLevel( int iUnitClass, int iSkillID )
{
	const SkillTreeTemplet* pSkillTreeTemplet = GetSkillTreeTemplet( iUnitClass, iSkillID );
	if( NULL == pSkillTreeTemplet )
		return 0;

	return pSkillTreeTemplet->m_iMaxSkillLevel;
}

//{{ 2009. 11. 26  최육사	길드스킬
int CXSLSkillTree::GetMaxGuildSkillLevel( int iGuildSkillID )
{
	const SkillTreeTemplet* pSkillTreeTemplet = GetGuildSkillTreeTemplet( iGuildSkillID );
	if( NULL == pSkillTreeTemplet )
		return 0;

	return pSkillTreeTemplet->m_iMaxSkillLevel;
}

bool CXSLSkillTree::AddSealSkillInfo_LUA( int iUnsealItemID, int iSkillID, char cUnitClass )
{
	if( cUnitClass <= 0 )
	{
		START_LOG( cerr, L"유효하지 않은 unitclass입니다!" )
			<< BUILD_LOG( iSkillID )
			<< BUILD_LOGc( cUnitClass )
			<< END_LOG;

		return false;
	}

	if( IsExsitSkillID( iSkillID ) == false )
	{
		START_LOG( cerr, L"유효하지 않은 SkillID입니다!" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iUnsealItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 ItemID가 봉인스킬아이템으로 설정되었습니다." )
			<< BUILD_LOG( iSkillID )
			<< BUILD_LOG( iUnsealItemID )
			<< END_LOG;

		return false;
	}

	if( m_mapUnsealSkillItem.find( iUnsealItemID ) != m_mapUnsealSkillItem.end() )
	{
		START_LOG( cerr, L"이미 등록된 Unseal ItemID입니다." )
			<< BUILD_LOG( iSkillID )
			<< BUILD_LOG( iUnsealItemID )
			<< END_LOG;

		return false;
	}

	KUnsealItemInfo kInfo;
	kInfo.m_iSkillID = iSkillID;
	kInfo.m_cUnitClass = cUnitClass;
	m_mapUnsealSkillItem.insert( std::make_pair( iUnsealItemID, kInfo ) );

	m_setUnsealSkillItem.insert( iUnsealItemID );

	std::map< int, std::vector< int > >::iterator mit = m_mapUnsealSkill.find( iSkillID );
	if( mit == m_mapUnsealSkill.end() )
	{
		std::vector< int > vecUnsealItemList;
		vecUnsealItemList.push_back( iUnsealItemID );
		m_mapUnsealSkill.insert( std::make_pair( iSkillID, vecUnsealItemList ) );
	}
	else
	{
		mit->second.push_back( iUnsealItemID );
	}

	return true;
}

const CXSLSkillTree::SkillTreeTemplet* CXSLSkillTree::GetSkillTreeTemplet( const int iUnitClass, const int iSkillID )
{
	UnitSkillTreeTempletMap::const_iterator mit = m_mapUnitSkillTreeTemplet.find( iUnitClass );
	if( mit == m_mapUnitSkillTreeTemplet.end() )
		return NULL;

	const SkillTreeTempletMap& mapSkillTreeTemplet = mit->second;

	SkillTreeTempletMap::const_iterator itSkillTreeTemplet = mapSkillTreeTemplet.find( iSkillID );
	if( itSkillTreeTemplet == mapSkillTreeTemplet.end() )
		return NULL;

	return &itSkillTreeTemplet->second;
}

//{{ 2009. 11. 26  최육사	길드스킬
const CXSLSkillTree::SkillTreeTemplet* CXSLSkillTree::GetGuildSkillTreeTemplet( const int iGuildSkillID )
{
	SkillTreeTempletMap::iterator mit = m_mapGuildSkillTreeTemplet.find( iGuildSkillID );
	if( mit == m_mapGuildSkillTreeTemplet.end() )
		return NULL;

	return &mit->second;
}
//}}

bool CXSLSkillTree::GetIncSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint )
{
	iSkillPoint = 2;
	return true;
}

bool CXSLSkillTree::IsUnitTypeDefaultSkill( IN int iSkillID )
{
	switch( (SKILL_ID) iSkillID )
	{
	case SI_SA_ES_MEGASLASH:
	case SI_SA_AV_CHAIN_FIRE_BALL:
	case SI_SA_LE_RAIL_STINGER:
	case SI_SA_RF_MAXIMUM_CANNON:
	case SI_SA_EN_ILLUSION_STRIKE:
	case SI_SA_CHUNG_BRUTAL_SWING:	// kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
	
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case SI_A_ES_ROLLING:
#else
	case SI_A_ES_COUNTER:
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case SI_A_AV_FIRE_ROAD:
#else
	case SI_A_AV_TELEPORT:
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_TRAPPING_RANGER_TEST
	case SI_A_LE_RISING_FALCON:
#else
	case SI_A_LE_SIEGEMODE:
#endif SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RAVEN_WEAPON_TAKER
	case SI_A_RF_BREAKING_FIST:
#else
	case SI_A_RF_BURNING_RUSH:
#endif SERV_RAVEN_WEAPON_TAKER
#ifdef SERV_EVE_ELECTRA
	case SI_A_EVE_PHOTON_BLINK:
#else
	case SI_A_EN_CLOAKING:
#endif SERV_EVE_ELECTRA
	case SI_A_CHUNG_RELOAD_CANNON:	// kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
		{
			return true;
		} break;
	}

	return false;
}


bool CXSLSkillTree::GetUnitTypeDefaultSkill( IN char cUnitType, OUT int& iSkillID, OUT int& iSkillID2 )
{
	iSkillID = 0;
	switch( cUnitType )
	{
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CXSLUnit::UT_ELSWORD:	iSkillID = (int)SI_SA_ES_MEGASLASH;			iSkillID2 = (int)SI_A_ES_ROLLING;		break;
#else
	case CXSLUnit::UT_ELSWORD:	iSkillID = (int)SI_SA_ES_MEGASLASH;			iSkillID2 = (int)SI_A_ES_COUNTER;		break;
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CXSLUnit::UT_ARME:		iSkillID = (int)SI_SA_AV_CHAIN_FIRE_BALL;	iSkillID2 = (int)SI_A_AV_FIRE_ROAD;		break;
#else
	case CXSLUnit::UT_ARME:		iSkillID = (int)SI_SA_AV_CHAIN_FIRE_BALL;	iSkillID2 = (int)SI_A_AV_TELEPORT;		break;
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_TRAPPING_RANGER_TEST
	case CXSLUnit::UT_LIRE:		iSkillID = (int)SI_SA_LE_RAIL_STINGER;		iSkillID2 = (int)SI_A_LE_RISING_FALCON;	break;
#else
	case CXSLUnit::UT_LIRE:		iSkillID = (int)SI_SA_LE_RAIL_STINGER;		iSkillID2 = (int)SI_A_LE_SIEGEMODE;		break;
#endif SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CXSLUnit::UT_RAVEN:	iSkillID = (int)SI_SA_RF_MAXIMUM_CANNON;	iSkillID2 = (int)SI_A_RF_BREAKING_FIST;	break;
#else
	case CXSLUnit::UT_RAVEN:	iSkillID = (int)SI_SA_RF_MAXIMUM_CANNON;	iSkillID2 = (int)SI_A_RF_BURNING_RUSH;	break;
#endif SERV_RAVEN_WEAPON_TAKER

#ifdef SERV_EVE_ELECTRA
	case CXSLUnit::UT_EVE:		iSkillID = (int)SI_SA_EN_ILLUSION_STRIKE;	iSkillID2 = (int)SI_A_EVE_PHOTON_BLINK;		break;
#else
	case CXSLUnit::UT_EVE:		iSkillID = (int)SI_SA_EN_ILLUSION_STRIKE;	iSkillID2 = (int)SI_A_EN_CLOAKING;		break;
#endif SERV_EVE_ELECTRA		
	// kimhc // 2010-12-23 에 추가될 신캐릭터 청 (임시로 메가 슬래시 넣어놓음)
	case CXSLUnit::UT_CHUNG:	iSkillID = (int)SI_SA_CHUNG_BRUTAL_SWING;	iSkillID2 = (int)SI_A_CHUNG_RELOAD_CANNON; break;

#ifdef SERV_ARA_MARTIAL_ARTIST
	case CXSLUnit::UT_ARA:		iSkillID = (int)SI_SA_ARA_WIND_WEDGE;		iSkillID2 = (int)SI_A_ARA_GAIN_FORCE;	break;
#endif
#ifdef SERV_NEW_CHARACTER_EL // 스킬ID 발급되기 전 임시 아라 스킬 사용
	case CXSLUnit::UT_ELESIS:		iSkillID = (int)SI_SA_ARA_WIND_WEDGE;		iSkillID2 = (int)SI_A_ARA_GAIN_FORCE;	break;
#endif // SERV_NEW_CHARACTER_EL
	default:
		return false;
	}

	return true;
}




// 문자열을 입력으로 받아서 숫자를 tokenize해서 하나씩 리턴해준다, 포인터는 다음 token 위치로 옮겨준다
int CXSLSkillTree::TokenizeInt( WCHAR** ppStart, WCHAR* pEnd )
{
#define IN_INT_CHARSET( c ) ( (c) == L'-' || ( (c) >= L'0' && (c) <= L'9' ) )


	if( NULL == ppStart )
		return 0;

	if( NULL == *ppStart )
		return 0;

	if( *ppStart == pEnd )
		return 0;

	WCHAR* pStop = pEnd;
	int iNumber = (int) wcstol( *ppStart, &pStop, 10 );
	*ppStart = pStop;

	// skip leading non-number character 
	while( !IN_INT_CHARSET( **ppStart ) )
	{
		if( **ppStart == L'\0' || *ppStart == pEnd )
			break;

		++ppStart;
	}

	return iNumber;
}

bool CXSLSkillTree::GetUnsealItemInfo( int iItemID, int& iSkillID, char& cUnitClass )
{
	std::map< int, KUnsealItemInfo >::const_iterator mit = m_mapUnsealSkillItem.find( iItemID );
	if( mit == m_mapUnsealSkillItem.end() )
		return false;

	iSkillID = mit->second.m_iSkillID;
	cUnitClass = mit->second.m_cUnitClass;
	return true;
}

bool CXSLSkillTree::GetUnsealSkillItemList( IN int iSkillID, OUT std::vector< int >& vecItemList )
{
	vecItemList.clear();

	std::map< int, std::vector< int > >::const_iterator mit = m_mapUnsealSkill.find( iSkillID );
	if( mit == m_mapUnsealSkill.end() )
	{
        return false;
	}

	vecItemList = mit->second;
	return true;
}

bool CXSLSkillTree::IsUnsealItemID( int iItemID )
{
	KLocker lock( m_csUnsealSkillItem );

    return ( m_setUnsealSkillItem.find( iItemID ) != m_setUnsealSkillItem.end() );
}

//{{ 2009. 11. 9  최육사	엘소드2차전직
bool CXSLSkillTree::CheckUnitClassForUnsealSkill( char cUnitClass, char cUnsealSkillUnitClass )
{
	// 현재 유닛클래스와 맞는지 검사
	if( cUnitClass == cUnsealSkillUnitClass )
		return true;

	// 봉인 스킬의 상위 유닛클래스를 얻는다.
	const char cUpGradeUnitClass = CXSLUnit::GetUnitClassUpGrade( static_cast<CXSLUnit::UNIT_CLASS>(cUnsealSkillUnitClass) );
	if( cUnitClass == cUpGradeUnitClass )
		return true;

	return false;
}
//}}

// @function : CalulateIncreaseingRate
//	@brief : 1.05 (5%), 0.95 (-5%) 등의 증가율을 0.05, -0.05 등으로 반환 해주는 함수
//	@param : float fRate_

float CXSLSkillTree::CalulateIncreaseingRate( float fRate_ )
{
	const float STANDARD_RATE = 1.0f;

	return fRate_ - STANDARD_RATE;
}


//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
const CXSLSkillTree::SkillTreeTempletMap* CXSLSkillTree::GetSkillTreeTemplet( const int iUnitClass )
{
	UnitSkillTreeTempletMap::const_iterator mit = m_mapUnitSkillTreeTemplet.find( iUnitClass );
	if( mit == m_mapUnitSkillTreeTemplet.end() )
		return NULL;

	return &mit->second;
}

bool CXSLSkillTree::IsSealSkill( const int iSkillID )
{
	return m_mapUnsealSkill.find( iSkillID ) != m_mapUnsealSkill.end();
}
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
//}}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013




