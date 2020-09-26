#include ".\kattribnpctable.h"
#include "../Common/X2Data/XSLAttribEnchantItem.h"
#include "../Common/X2Data/XSLDungeon.h"
#include "../Common/X2Data/XSLDamageManager.h"
#include "KDropTable.h"



ImplementRefreshSingleton( KAttribNpcTable );

KAttribNpcTable::KAttribNpcTable(void)
{
}

KAttribNpcTable::~KAttribNpcTable(void)
{
}

ImplToStringW( KAttribNpcTable )
{
	stm_	<< L"----------[ Attrib Npc Table ]----------" << std::endl
		//{{ 2010. 04. 08  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
		<< TOSTRINGW( m_mapDropNpcCount.size() )
#else
		<< TOSTRINGW( m_mapDropNpcCount.size() )
		<< TOSTRINGW( m_mapAttribNpc.size() )
#endif SERV_SECRET_HELL
		//}}
		<< TOSTRINGW( m_mapAttribDropTable.size() )
		<< TOSTRINGW( m_mapAttribDropGroup.size() )
		<< TOSTRINGW( m_setNoDropNpc.size() )
		;

	for( int i = 0; i < AND_MAX; ++i )
		stm_ << TOSTRINGW( m_mapAttribNpc[i].size() );

	return stm_;
}

ImplementLuaScriptParser( KAttribNpcTable )
{
	lua_tinker::class_add<KAttribNpcTable>( GetLuaState(), "KAttribNpcTable" );
	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	lua_tinker::class_def<KAttribNpcTable>( GetLuaState(), "SetAttribSubStageCount",			&KAttribNpcTable::SetAttribSubStageCount_LUA );
#else		// 엑셀 파일도 변경 되었으므로 참고 하자.
	lua_tinker::class_def<KAttribNpcTable>( GetLuaState(), "SetNpcDropCount",			&KAttribNpcTable::SetNpcDropCount_LUA );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}
	
	lua_tinker::class_def<KAttribNpcTable>( GetLuaState(), "AddAttribNpcInfo_LUA",		&KAttribNpcTable::AddAttribNpcInfo_LUA );
	lua_tinker::class_def<KAttribNpcTable>( GetLuaState(), "AddAttribDropInfo",			&KAttribNpcTable::AddAttribDropInfo_LUA );
	lua_tinker::class_def<KAttribNpcTable>( GetLuaState(), "AddAttribDropGroupInfo",	&KAttribNpcTable::AddAttribDropGroupInfo_LUA );
	lua_tinker::class_def<KAttribNpcTable>( GetLuaState(), "AddNoDropNpc",				&KAttribNpcTable::AddNoDropNpc_LUA );
	lua_tinker::class_def<KAttribNpcTable>( GetLuaState(), "dump",						&KAttribNpcTable::Dump );

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	lua_tinker::class_def<KAttribNpcTable>( GetLuaState(), "SetNpcDropCount",				&KAttribNpcTable::SetNpcDropCount_LUA );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	lua_tinker::decl( GetLuaState(), "g_pAttribNpcManager", this );
}

//{{ 2010. 04. 08  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
bool KAttribNpcTable::SetAttribSubStageCount_LUA( char cDungeonType, char cMode, int iSubStageCount )
{
	if( cDungeonType < 0  ||  cMode < 0  ||  iSubStageCount < 0 )
	{
		START_LOG( cerr, L"속성 NPC 드롭 테이블 정보가 이상합니다." )
			<< BUILD_LOGc( cDungeonType )
			<< BUILD_LOGc( cMode )
			<< BUILD_LOG( iSubStageCount )
			<< END_LOG;
		return false;
	}

	KDropNpcCount kDropNpcCnt;
	kDropNpcCnt.m_iSubStageCount = iSubStageCount;
	kDropNpcCnt.m_iDropNpcCount = 0;


	m_mapDropNpcCount.insert( std::make_pair( KAttribNpcDropKey( cDungeonType, cMode ), kDropNpcCnt ) );
	return true;
}

bool KAttribNpcTable::SetNpcDropCount_LUA( char cDungeonType, int iPlayerCount, int iNpcDropCount )
{
	if( cDungeonType < 0  ||  iPlayerCount < 1  ||  iNpcDropCount < 0 )
	{
		START_LOG( cerr, L"속성 NPC 드롭 카운트 테이블 정보가 이상합니다." )
			<< BUILD_LOGc( cDungeonType )
			<< BUILD_LOGc( iPlayerCount )
			<< BUILD_LOG( iNpcDropCount )
			<< END_LOG;
		return false;
	}

	m_mapAttribNpcCount.insert( std::make_pair( KAttribNpcCountKey( cDungeonType, iPlayerCount ), iNpcDropCount ) );
	return true;
}

#else

bool KAttribNpcTable::SetNpcDropCount_LUA( char cDungeonType, char cMode, int iSubStageCount, int iDropNpcCount )
{
	if( cDungeonType < 0  ||  cMode < 0  ||  iSubStageCount < 0  ||  iDropNpcCount < 0 )
	{
		START_LOG( cerr, L"속성 NPC 드롭 테이블 정보가 이상합니다." )
			<< BUILD_LOGc( cDungeonType )
			<< BUILD_LOGc( cMode )
			<< BUILD_LOG( iSubStageCount )
			<< BUILD_LOG( iDropNpcCount )			
			<< END_LOG;
		return false;
	}

	KDropNpcCount kDropNpcCnt;
	kDropNpcCnt.m_iSubStageCount = iSubStageCount;
	kDropNpcCnt.m_iDropNpcCount  = iDropNpcCount;

	m_mapDropNpcCount.insert( std::make_pair( KAttribNpcDropKey( cDungeonType, cMode ), kDropNpcCnt ) );
	return true;
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

bool KAttribNpcTable::AddAttribNpcInfo_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	//TableBind( &luaManager );

	KAttribNpcInfo kAttribNpc;

	CXSLDungeon::DUNGEON_TYPE eDungeonType;
	CXSLDungeon::DUNGEON_MODE eDungeonMode;
	int iDifficuly = 0;
	CXSLUnitManager::NPC_ABILITY_ID eNpcType;
	int iNpcLvBegin = 0;
	int iNpcLvEnd = 0;
	//{{ 2010. 04. 07  최육사	비밀던전 헬모드
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonType",		eDungeonType,		CXSLDungeon::DUNGEON_TYPE,		 CXSLDungeon::DT_INVALID,	goto end_proc );
	LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		eDungeonMode,		CXSLDungeon::DUNGEON_MODE,		 CXSLDungeon::DM_INVALID );
	//}}
	LUA_GET_VALUE_ENUM(			luaManager, L"m_eDifficulty",		iDifficuly,			CXSLDungeon::DIFFICULTY_LEVEL,	 -1 );

	if( eDungeonMode == CXSLDungeon::DM_INVALID  &&  iDifficuly == -1 )
	{
		START_LOG( cerr, L"던전 난이도 정보가 이상합니다!" )
			<< BUILD_LOG( eDungeonType )
			<< BUILD_LOG( eDungeonMode )
			<< BUILD_LOG( iDifficuly )
			<< END_LOG;
		goto end_proc;
	}

	LUA_GET_VALUE_RETURN(		luaManager, L"m_iNpcLvBegin",		iNpcLvBegin,		0,		goto end_proc );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iNpcLvEnd",			iNpcLvEnd,			0,		goto end_proc );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eNpcType",			eNpcType,			CXSLUnitManager::NPC_ABILITY_ID, CXSLUnitManager::NAI_NONE, goto end_proc );

	LUA_GET_VALUE(				luaManager, L"m_bTotalRandom",			kAttribNpc.m_bTotalRandom,	false );

	float fRateValue;
	if( kAttribNpc.m_bTotalRandom )
	{
		//////////////////////////////////////////////////////////////////////////
		// 하급 NPC
		LUA_GET_VALUE(			luaManager, L"m_fBraveRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_BRAVE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fCleaverRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_CLEVER, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fCriticalRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_CRITICAL, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fQuickerRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_QUICKER, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fPhysicDefRate",		fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_PHYSIC_DEFENSE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fMagicDefRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_MAGIC_DEFENSE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fSuperArmorRate",		fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_SUPER_ARMOR, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fHeavyRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_HEAVY, fRateValue );
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 상급 NPC
		LUA_GET_VALUE(			luaManager, L"m_fBraveRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kAtkNpcLot.AddCase( CXSLUnitManager::NAI_BRAVE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fCleaverRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kAtkNpcLot.AddCase( CXSLUnitManager::NAI_CLEVER, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fCriticalRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kAtkNpcLot.AddCase( CXSLUnitManager::NAI_CRITICAL, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fQuickerRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kAtkNpcLot.AddCase( CXSLUnitManager::NAI_QUICKER, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fPhysicDefRate",		fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kDefNpcLot.AddCase( CXSLUnitManager::NAI_PHYSIC_DEFENSE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fMagicDefRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kDefNpcLot.AddCase( CXSLUnitManager::NAI_MAGIC_DEFENSE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fSuperArmorRate",		fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kDefNpcLot.AddCase( CXSLUnitManager::NAI_SUPER_ARMOR, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fHeavyRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kDefNpcLot.AddCase( CXSLUnitManager::NAI_HEAVY, fRateValue );
	}

	//////////////////////////////////////////////////////////////////////////
	// 싱글 속성
	LUA_GET_VALUE(			luaManager, L"m_fBlazeRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_BLAZE, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_FROZEN, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_POISON, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPiercingRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fShockRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fSnatchRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_SNATCH, fRateValue );

	//////////////////////////////////////////////////////////////////////////
	// 듀얼 속성
	LUA_GET_VALUE(			luaManager, L"m_fBlazeMasterRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_BLAZE_MASTER, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenMasterRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_FROZEN_MASTER, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonMasterRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_POISON_MASTER, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fMasterPiercingRate",	fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_MASTER_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fMasterShockRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_MASTER_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fMasterSnatchRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_MASTER_SNATCH, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fBlazePiercingRate",	fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_BLAZE_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fBlazeShockRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_BLAZE_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fBlazeSnatchRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_BLAZE_SNATCH, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenPiercingRate",	fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_FROZEN_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenShockRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_FROZEN_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenSnatchRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_FROZEN_SNATCH, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonPiercingRate",	fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_POISON_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonShockRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_POISON_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonSnatchRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( AT_POISON_SNATCH, fRateValue );

	//////////////////////////////////////////////////////////////////////////
	// Npc레벨별로 템플릿 데이터 만들기

	if( eDungeonType == CXSLDungeon::DT_NORMAL )
	{
		// 일반 던전
		for( int iCnt = iNpcLvBegin; iCnt <= iNpcLvEnd; ++iCnt )
		{
			KAttribNpcKey kKey;
			kKey.first  = iCnt;
			kKey.second = iDifficuly;

			std::map< KAttribNpcKey, KAttribNpcRandom >::iterator mit;
			mit = m_mapAttribNpc[AND_NORMAL].find( kKey );
			if( mit == m_mapAttribNpc[AND_NORMAL].end() )
			{
				KAttribNpcRandom kRandom;
				if( eNpcType == CXSLUnitManager::NAI_BRAVE )
				{
					kRandom.m_kBrave = kAttribNpc;
				}
				else
				{
					kRandom.m_kClever = kAttribNpc;
				}
				m_mapAttribNpc[AND_NORMAL].insert( std::make_pair( kKey, kRandom ) );
			}
			else
			{
				if( eNpcType == CXSLUnitManager::NAI_BRAVE )
				{
					mit->second.m_kBrave = kAttribNpc;
				}
				else
				{
					mit->second.m_kClever = kAttribNpc;
				}
			}
		}
	}
	else if( eDungeonType == CXSLDungeon::DT_SECRET )
	{
		// 일반 던전
		for( int iCnt = iNpcLvBegin; iCnt <= iNpcLvEnd; ++iCnt )
		{
			KAttribNpcKey kKey;
			kKey.first  = iCnt;
			kKey.second = static_cast<int>(eDungeonMode);

			std::map< KAttribNpcKey, KAttribNpcRandom >::iterator mit;
			mit = m_mapAttribNpc[AND_SECRET].find( kKey );
			if( mit == m_mapAttribNpc[AND_SECRET].end() )
			{
				KAttribNpcRandom kRandom;
				if( eNpcType == CXSLUnitManager::NAI_BRAVE )
				{
					kRandom.m_kBrave = kAttribNpc;
				}
				else
				{
					kRandom.m_kClever = kAttribNpc;
				}
				m_mapAttribNpc[AND_SECRET].insert( std::make_pair( kKey, kRandom ) );
			}
			else
			{
				if( eNpcType == CXSLUnitManager::NAI_BRAVE )
				{
					mit->second.m_kBrave = kAttribNpc;
				}
				else
				{
					mit->second.m_kClever = kAttribNpc;
				}
			}
		}
	}
	else
	{
		START_LOG( cerr, L"잘못된 던전 타입입니다!" )
			<< BUILD_LOG( eDungeonType )
			<< END_LOG;
		goto end_proc;
	}

	return true;

end_proc:
	START_LOG( cerr, L"속성 NPC 스크립트 파싱 실패.!" )		
		<< END_LOG;

	return false;
}

#else

bool KAttribNpcTable::SetNpcDropCount_LUA( int iDifficulty, int iSubStageCount, int iDropNpcCount )
{
	if( iDifficulty < 0  ||  iSubStageCount < 0  ||  iDropNpcCount < 0 )
	{
		START_LOG( cerr, L"속성 NPC 드롭 테이블 정보가 이상합니다." )
			<< BUILD_LOG( iDifficulty )
			<< BUILD_LOG( iSubStageCount )
			<< BUILD_LOG( iDropNpcCount )			
			<< END_LOG;
		return false;
	}

	KDropNpcCount kDropNpcCnt;
	kDropNpcCnt.m_iSubStageCount = iSubStageCount;
	kDropNpcCnt.m_iDropNpcCount  = iDropNpcCount;

	m_mapDropNpcCount.insert( std::make_pair( iDifficulty, kDropNpcCnt ) );
	return true;
}

bool KAttribNpcTable::AddAttribNpcInfo_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	//TableBind( &luaManager );

	KAttribNpcInfo kAttribNpc;

	int iDifficuly = 0;
	CXSLUnitManager::NPC_ABILITY_ID eNpcType;
	int iNpcLvBegin = 0;
	int iNpcLvEnd = 0;
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDifficulty",		iDifficuly,			CXSLDungeon::DIFFICULTY_LEVEL,	 CXSLDungeon::DL_NORMAL,	goto end_proc );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iNpcLvBegin",		iNpcLvBegin,		0,		goto end_proc );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iNpcLvEnd",			iNpcLvEnd,			0,		goto end_proc );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eNpcType",			eNpcType,			CXSLUnitManager::NPC_ABILITY_ID, CXSLUnitManager::NAI_NONE, goto end_proc );

	LUA_GET_VALUE(			luaManager, L"m_bTotalRandom",			kAttribNpc.m_bTotalRandom,	false );

	float fRateValue;
	if( kAttribNpc.m_bTotalRandom )
	{
		//////////////////////////////////////////////////////////////////////////
		// 하급 NPC
		LUA_GET_VALUE(			luaManager, L"m_fBraveRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_BRAVE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fCleaverRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_CLEVER, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fCriticalRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_CRITICAL, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fQuickerRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_QUICKER, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fPhysicDefRate",		fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_PHYSIC_DEFENSE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fMagicDefRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_MAGIC_DEFENSE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fSuperArmorRate",		fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_SUPER_ARMOR, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fHeavyRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kTotalLot.AddCase( CXSLUnitManager::NAI_HEAVY, fRateValue );
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 상급 NPC
		LUA_GET_VALUE(			luaManager, L"m_fBraveRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kAtkNpcLot.AddCase( CXSLUnitManager::NAI_BRAVE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fCleaverRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kAtkNpcLot.AddCase( CXSLUnitManager::NAI_CLEVER, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fCriticalRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kAtkNpcLot.AddCase( CXSLUnitManager::NAI_CRITICAL, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fQuickerRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kAtkNpcLot.AddCase( CXSLUnitManager::NAI_QUICKER, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fPhysicDefRate",		fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kDefNpcLot.AddCase( CXSLUnitManager::NAI_PHYSIC_DEFENSE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fMagicDefRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kDefNpcLot.AddCase( CXSLUnitManager::NAI_MAGIC_DEFENSE, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fSuperArmorRate",		fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kDefNpcLot.AddCase( CXSLUnitManager::NAI_SUPER_ARMOR, fRateValue );

		LUA_GET_VALUE(			luaManager, L"m_fHeavyRate",			fRateValue,			0.0f );
		if( fRateValue > 0.0f )
			kAttribNpc.m_kDefNpcLot.AddCase( CXSLUnitManager::NAI_HEAVY, fRateValue );
	}

	//////////////////////////////////////////////////////////////////////////
	// 싱글 속성
	LUA_GET_VALUE(			luaManager, L"m_fBlazeRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_BLAZE, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_FROZEN, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_POISON, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPiercingRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fShockRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fSnatchRate",			fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_SNATCH, fRateValue );

	//////////////////////////////////////////////////////////////////////////
	// 듀얼 속성
	LUA_GET_VALUE(			luaManager, L"m_fBlazeMasterRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_BLAZE_MASTER, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenMasterRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_FROZEN_MASTER, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonMasterRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_POISON_MASTER, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fMasterPiercingRate",	fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_MASTER_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fMasterShockRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_MASTER_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fMasterSnatchRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_MASTER_SNATCH, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fBlazePiercingRate",	fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_BLAZE_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fBlazeShockRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_BLAZE_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fBlazeSnatchRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_BLAZE_SNATCH, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenPiercingRate",	fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_FROZEN_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenShockRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_FROZEN_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fFrozenSnatchRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_FROZEN_SNATCH, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonPiercingRate",	fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_POISON_PIERCING, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonShockRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_POISON_SHOCK, fRateValue );

	LUA_GET_VALUE(			luaManager, L"m_fPoisonSnatchRate",		fRateValue,			0.0f );
	if( fRateValue > 0.0f )
		kAttribNpc.m_kAttribLot.AddCase( ATTRIB_TYPE::AT_POISON_SNATCH, fRateValue );

	for( int iCnt = iNpcLvBegin; iCnt <= iNpcLvEnd; ++iCnt )
	{
		KAttribNpcKey kKey;
		kKey.first  = iCnt;
		kKey.second = iDifficuly;

		std::map< KAttribNpcKey, KAttribNpcRandom >::iterator mit;
		mit = m_mapAttribNpc.find( kKey );
		if( mit == m_mapAttribNpc.end() )
		{
			KAttribNpcRandom kRandom;
			if( eNpcType == CXSLUnitManager::NAI_BRAVE )
			{
				kRandom.m_kBrave = kAttribNpc;
			}
			else
			{
				kRandom.m_kClever = kAttribNpc;
			}
			m_mapAttribNpc.insert( std::make_pair( kKey, kRandom ) );
		}
		else
		{
			if( eNpcType == CXSLUnitManager::NAI_BRAVE )
			{
				mit->second.m_kBrave = kAttribNpc;
			}
			else
			{
				mit->second.m_kClever = kAttribNpc;
			}
		}
	}


	return true;

end_proc:
	START_LOG( cerr, L"속성 NPC 스크립트 파싱 실패.!" )		
		<< END_LOG;

	return false;
}

#endif SERV_SECRET_HELL
//}}

bool KAttribNpcTable::AddAttribDropInfo_LUA( int iNpcLv, int iNormalDropGroupID, int iHardDropGroupID, int iExpertDropGroupID )
{
	if( iNpcLv <= 0  ||  iNormalDropGroupID <= 0  ||  iHardDropGroupID <= 0  ||  iExpertDropGroupID <= 0 )
	{
		START_LOG( cerr, L"속성 NPC 드롭 테이블 정보가 이상합니다." )
			<< BUILD_LOG( iNpcLv )
			<< BUILD_LOG( iNormalDropGroupID )
			<< BUILD_LOG( iHardDropGroupID )
			<< BUILD_LOG( iExpertDropGroupID )
			<< END_LOG;
		return false;
	}

	KAttribDropTable kDropTable;
	kDropTable.insert( std::make_pair( CXSLDungeon::DL_NORMAL, iNormalDropGroupID ) );
	kDropTable.insert( std::make_pair( CXSLDungeon::DL_HARD, iHardDropGroupID ) );
	kDropTable.insert( std::make_pair( CXSLDungeon::DL_EXPERT, iExpertDropGroupID ) );

	m_mapAttribDropTable.insert( std::make_pair( iNpcLv, kDropTable ) );
	return true;
}

bool KAttribNpcTable::AddAttribDropGroupInfo_LUA( int iDropGroupID, int iDropTableGroupID, float fRate )
{
	if( iDropGroupID <= 0  ||  iDropTableGroupID <= 0  ||  fRate <= 0.0f )
	{
		START_LOG( cerr, L"속성 NPC 드롭 그룹 정보가 이상합니다." )
			<< BUILD_LOG( iDropGroupID )
			<< BUILD_LOG( iDropTableGroupID )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	std::map< int, KLottery >::iterator mit;
	mit = m_mapAttribDropGroup.find( iDropGroupID );
	if( mit == m_mapAttribDropGroup.end() )
	{
		KLottery kLot;
		kLot.AddCase( iDropTableGroupID, fRate );
		m_mapAttribDropGroup.insert( std::make_pair( iDropGroupID, kLot ) );
	}
	else
	{
		mit->second.AddCase( iDropTableGroupID, fRate );
	}
	return true;
}

bool KAttribNpcTable::AddNoDropNpc_LUA( int iNpcID )
{
	if( iNpcID <= 0 )
	{
		START_LOG( cerr, L"NPC ID 정보가 이상합니다." )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOG( static_cast<int>(m_setNoDropNpc.size()) )
			<< END_LOG;
		return false;
	}

	m_setNoDropNpc.insert( iNpcID );
	return true;
}

void KAttribNpcTable::GetAttribEnchantValue( IN int iAttribType, OUT char& cAttribEnchant1, OUT char& cAttribEnchant2 )
{
	cAttribEnchant1 = 0;
	cAttribEnchant2 = 0;

	switch( iAttribType )
	{
	case AT_BLAZE:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_BLAZE;
		break;

	case AT_FROZEN:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_WATER;
		break;

	case AT_POISON:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_NATURE;
		break;

	case AT_PIERCING:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_WIND;
		break;

	case AT_SHOCK:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_LIGHT;
		break;

	case AT_SNATCH:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_DARK;
		break;

	case AT_BLAZE_MASTER:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_BLAZE;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_BLAZE;
		break;

	case AT_FROZEN_MASTER:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_WATER;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_WATER;
		break;

	case AT_POISON_MASTER:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_NATURE;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_NATURE;
		break;

	case AT_MASTER_PIERCING:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_WIND;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_WIND;
		break;

	case AT_MASTER_SHOCK:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_LIGHT;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_LIGHT;
		break;

	case AT_MASTER_SNATCH:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_DARK;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_DARK;
		break;

	case AT_BLAZE_PIERCING:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_BLAZE;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_WIND;
		break;

	case AT_BLAZE_SHOCK:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_BLAZE;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_LIGHT;
		break;

	case AT_BLAZE_SNATCH:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_BLAZE;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_DARK;
		break;

	case AT_FROZEN_PIERCING:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_WATER;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_WIND;
		break;

	case AT_FROZEN_SHOCK:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_WATER;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_LIGHT;
		break;

	case AT_FROZEN_SNATCH:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_WATER;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_DARK;
		break;

	case AT_POISON_PIERCING:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_NATURE;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_WIND;
		break;

	case AT_POISON_SHOCK:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_NATURE;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_LIGHT;
		break;

	case AT_POISON_SNATCH:
		cAttribEnchant1 = CXSLAttribEnchantItem::ET_NATURE;
		cAttribEnchant2 = CXSLAttribEnchantItem::ET_DARK;
		break;

	default:
		START_LOG( cerr, L"정의되지 않은 타입입니다." )
			<< BUILD_LOG( iAttribType )
			<< END_LOG;
		break;
	}
}

//{{ 2010. 04. 08  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL

bool KAttribNpcTable::GetAttribNpcDropCount( IN const char cDungeonType, IN const char cMode, OUT int& iSubStageCount, OUT int& iDropNpcCount )
{
	iSubStageCount = 0;
	iDropNpcCount = 0;

	std::map< KAttribNpcDropKey, KDropNpcCount >::const_iterator mit;
	mit = m_mapDropNpcCount.find( KAttribNpcDropKey( cDungeonType, cMode ) );
	if( mit == m_mapDropNpcCount.end() )
	{
		// 정보가 없는경우는 엘리트 몬스터가 등장하지 않습니다.
		return false;
	}

	iSubStageCount = mit->second.m_iSubStageCount;
	iDropNpcCount  = mit->second.m_iDropNpcCount;

	return true;
}

bool KAttribNpcTable::GetAttribNpcInfo( IN const CXSLDungeon::DUNGEON_TYPE eDungeonType, IN const char cMode, IN const char cNpcLv, IN const int iNpcID, OUT KAttribEnchantNpcInfo& kAttribNpcInfo )
{
	KAttribNpcKey kKey;
	kKey.first  = static_cast<int>(cNpcLv);
	kKey.second = static_cast<int>(cMode);

	std::map< KAttribNpcKey, KAttribNpcRandom >::const_iterator mit;

	// 해당 던전 타입에 맞는 속성NPC정보를 얻자!
	if( eDungeonType == CXSLDungeon::DT_NORMAL )
	{
		mit = m_mapAttribNpc[AND_NORMAL].find( kKey );
		if( mit == m_mapAttribNpc[AND_NORMAL].end() )
		{
			START_LOG( cerr, L"속성 몬스터 정보를 찾지 못했다!" )
				<< BUILD_LOGc( cNpcLv )
				<< BUILD_LOGc( cMode )
				<< BUILD_LOG( m_mapAttribNpc[AND_NORMAL].size() )
				<< END_LOG;
			return false;
		}
	}
	else if( eDungeonType == CXSLDungeon::DT_SECRET )
	{
		mit = m_mapAttribNpc[AND_SECRET].find( kKey );
		if( mit == m_mapAttribNpc[AND_SECRET].end() )
		{
			START_LOG( cerr, L"속성 몬스터 정보를 찾지 못했다!" )
				<< BUILD_LOGc( cNpcLv )
				<< BUILD_LOGc( cMode )
				<< BUILD_LOG( m_mapAttribNpc[AND_SECRET].size() )
				<< END_LOG;
			return false;
		}
	}
	else
	{
		START_LOG( cerr, L"잘못된 던전타입입니다." )
			<< BUILD_LOGc( eDungeonType )
			<< END_LOG;
		return false;
	}

	const CXSLUnitManager::NPCUnitStat* pNpcUnitStat = SiCXSLUnitManager()->GetNPCUnitStat( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNpcID) );
	if( pNpcUnitStat == NULL )
	{
		START_LOG( cerr, L"NPC Unit Stat이 존재하지 않는 NpcID입니다." )
			<< BUILD_LOGc( eDungeonType )
			<< BUILD_LOGc( cMode )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOGc( cNpcLv )
			<< END_LOG;
		return false;
	}

	switch( pNpcUnitStat->m_DamageType )
	{
	case CXSLDamageManager::DT_PHYSIC:
		{
			if( mit->second.m_kBrave.m_bTotalRandom )
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kBrave.m_kTotalLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}
			}
			else
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kBrave.m_kAtkNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}

				kAttribNpcInfo.m_cExtraSpecialAbility2 = mit->second.m_kBrave.m_kDefNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility2 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility2 = 0;
				}
			}

			int iAttribType = mit->second.m_kBrave.m_kAttribLot.Decision();
			if( iAttribType != KLottery::CASE_BLANK )
			{
				GetAttribEnchantValue( iAttribType, kAttribNpcInfo.m_cAttribEnchant1, kAttribNpcInfo.m_cAttribEnchant2 );
			}
		}
		break;

	case CXSLDamageManager::DT_MAGIC:
		{
			if( mit->second.m_kBrave.m_bTotalRandom )
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kClever.m_kTotalLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}
			}
			else
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kClever.m_kAtkNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}

				kAttribNpcInfo.m_cExtraSpecialAbility2 = mit->second.m_kClever.m_kDefNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility2 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility2 = 0;
				}
			}

			int iAttribType = mit->second.m_kClever.m_kAttribLot.Decision();
			if( iAttribType != KLottery::CASE_BLANK )
			{
				GetAttribEnchantValue( iAttribType, kAttribNpcInfo.m_cAttribEnchant1, kAttribNpcInfo.m_cAttribEnchant2 );
			}
		}
		break;

	default:
		START_LOG( cerr, L"이상한 NPC 타입이 들어왔다?" )
			<< BUILD_LOGc( eDungeonType )
			<< BUILD_LOGc( cMode )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOGc( cNpcLv )
			<< BUILD_LOG( pNpcUnitStat->m_DamageType )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KAttribNpcTable::GetAttribNpcInfoExtraStage( IN const int iNpcID, OUT KAttribEnchantNpcInfo& kAttribNpcInfo )
{
	if( m_mapAttribNpc[AND_NORMAL].size() == 0 )
		return false;

	const int iSelectedIndex = rand() % m_mapAttribNpc[AND_NORMAL].size();
	int iIndex = 0;
    
	std::map< KAttribNpcKey, KAttribNpcRandom >::const_iterator mit;
	for( mit = m_mapAttribNpc[AND_NORMAL].begin(); mit != m_mapAttribNpc[AND_NORMAL].end(); ++mit )
	{
		if( iIndex == iSelectedIndex )
			break;

		++iIndex;
	}

	if( mit == m_mapAttribNpc[AND_NORMAL].end() )
		return false;

	const CXSLUnitManager::NPCUnitStat* pNpcUnitStat = SiCXSLUnitManager()->GetNPCUnitStat( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNpcID) );
	if( pNpcUnitStat == NULL )
	{
		START_LOG( cerr, L"NPC Unit Stat이 존재하지 않는 NpcID입니다." )			
			<< BUILD_LOG( iNpcID )			
			<< END_LOG;
		return false;
	}

	switch( pNpcUnitStat->m_DamageType )
	{
	case CXSLDamageManager::DT_PHYSIC:
		{
			if( mit->second.m_kBrave.m_bTotalRandom )
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kBrave.m_kTotalLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}
			}
			else
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kBrave.m_kAtkNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}

				kAttribNpcInfo.m_cExtraSpecialAbility2 = mit->second.m_kBrave.m_kDefNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility2 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility2 = 0;
				}
			}

			int iAttribType = mit->second.m_kBrave.m_kAttribLot.Decision();
			if( iAttribType != KLottery::CASE_BLANK )
			{
				GetAttribEnchantValue( iAttribType, kAttribNpcInfo.m_cAttribEnchant1, kAttribNpcInfo.m_cAttribEnchant2 );
			}
		}
		break;

	case CXSLDamageManager::DT_MAGIC:
		{
			if( mit->second.m_kBrave.m_bTotalRandom )
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kClever.m_kTotalLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}
			}
			else
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kClever.m_kAtkNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}

				kAttribNpcInfo.m_cExtraSpecialAbility2 = mit->second.m_kClever.m_kDefNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility2 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility2 = 0;
				}
			}

			int iAttribType = mit->second.m_kClever.m_kAttribLot.Decision();
			if( iAttribType != KLottery::CASE_BLANK )
			{
				GetAttribEnchantValue( iAttribType, kAttribNpcInfo.m_cAttribEnchant1, kAttribNpcInfo.m_cAttribEnchant2 );
			}
		}
		break;

	default:
		START_LOG( cerr, L"이상한 NPC 타입이 들어왔다?" )			
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOG( pNpcUnitStat->m_DamageType )
			<< END_LOG;
		return false;
	}

	return true;
}

#else

bool KAttribNpcTable::GetAttribNpcDropCount( int iDifficulty, int& iSubStageCount, int& iDropNpcCount )
{
	iSubStageCount = 0;
	iDropNpcCount = 0;

	std::map< int, KDropNpcCount >::const_iterator mit;
	mit = m_mapDropNpcCount.find( iDifficulty );
	if( mit == m_mapDropNpcCount.end() )
		return false;

	iSubStageCount = mit->second.m_iSubStageCount;
	iDropNpcCount  = mit->second.m_iDropNpcCount;
	return true;
}

bool KAttribNpcTable::GetAttribNpcInfo( char cDifficulty, int iNpcID, char cNpcLv, KAttribEnchantNpcInfo& kAttribNpcInfo )
{
	KAttribNpcKey kKey;
	kKey.first  = static_cast<int>(cNpcLv);
	kKey.second = static_cast<int>(cDifficulty);

	std::map< KAttribNpcKey, KAttribNpcRandom >::const_iterator mit;
	mit = m_mapAttribNpc.find( kKey );
	if( mit == m_mapAttribNpc.end() )
		return false;

	const CXSLUnitManager::NPCUnitStat* pNpcUnitStat = SiCXSLUnitManager()->GetNPCUnitStat( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNpcID) );
	if( pNpcUnitStat == NULL )
	{
		START_LOG( cerr, L"NPC Unit Stat이 존재하지 않는 NpcID입니다." )
			<< BUILD_LOGc( cDifficulty )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOGc( cNpcLv )
			<< END_LOG;
		return false;
	}

	switch( pNpcUnitStat->m_DamageType )
	{
	case CXSLDamageManager::DT_PHYSIC:
		{
			if( mit->second.m_kBrave.m_bTotalRandom )
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kBrave.m_kTotalLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}
			}
			else
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kBrave.m_kAtkNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}

				kAttribNpcInfo.m_cExtraSpecialAbility2 = mit->second.m_kBrave.m_kDefNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility2 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility2 = 0;
				}
			}

			int iAttribType = mit->second.m_kBrave.m_kAttribLot.Decision();
			if( iAttribType != KLottery::CASE_BLANK )
			{
				GetAttribEnchantValue( iAttribType, kAttribNpcInfo.m_cAttribEnchant1, kAttribNpcInfo.m_cAttribEnchant2 );
			}
		}
		break;

	case CXSLDamageManager::DT_MAGIC:
		{
			if( mit->second.m_kBrave.m_bTotalRandom )
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kClever.m_kTotalLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}
			}
			else
			{
				kAttribNpcInfo.m_cExtraSpecialAbility1 = mit->second.m_kClever.m_kAtkNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility1 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility1 = 0;
				}

				kAttribNpcInfo.m_cExtraSpecialAbility2 = mit->second.m_kClever.m_kDefNpcLot.Decision();
				if( kAttribNpcInfo.m_cExtraSpecialAbility2 == KLottery::CASE_BLANK )
				{
					kAttribNpcInfo.m_cExtraSpecialAbility2 = 0;
				}
			}

			int iAttribType = mit->second.m_kClever.m_kAttribLot.Decision();
			if( iAttribType != KLottery::CASE_BLANK )
			{
				GetAttribEnchantValue( iAttribType, kAttribNpcInfo.m_cAttribEnchant1, kAttribNpcInfo.m_cAttribEnchant2 );
			}
		}
		break;

	default:
		START_LOG( cerr, L"이상한 NPC 타입이 들어왔다?" )
			<< BUILD_LOGc( cDifficulty )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOGc( cNpcLv )
			<< BUILD_LOG( pNpcUnitStat->m_DamageType )
			<< END_LOG;
		return false;
	}

	return true;
}

#endif SERV_SECRET_HELL
//}}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT

#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
bool KAttribNpcTable::AttribNpcDropItem( IN int iDifficulty, IN int iNpcLv, IN const float fUserContribution, IN float fDropRate, OUT std::vector< int >& vecDropItem )
#else // SERV_DROP_EVENT_RENEWAL
bool KAttribNpcTable::AttribNpcDropItem( IN int iDifficulty, IN int iNpcLv, IN const float fUserContribution, IN int iDropCount, OUT std::vector< int >& vecDropItem )
#endif // SERV_DROP_EVENT_RENEWAL

#else
bool KAttribNpcTable::AttribNpcDropItem( IN int iDifficulty, IN int iNpcLv, IN const float fUserContribution, OUT std::vector< int >& vecDropItem )
#endif SERV_ITEM_DROP_EVENT
//}}
{
	vecDropItem.clear();

	// 1. 해당 npc 레벨의 드롭 테이블을 찾아보자
	std::map< int, KAttribDropTable >::const_iterator mit;
	mit = m_mapAttribDropTable.find( iNpcLv );
	if( mit == m_mapAttribDropTable.end() )
	{
		START_LOG( cerr, L"드롭테이블에 등록되지 않은 NPC Lv정보." )		
			<< BUILD_LOG( iNpcLv )
			<< END_LOG;
		return false;
	}

	// 2. 해당 난이도의 드롭 테이블을 찾아보자
	std::map< int, int >::const_iterator mitDrop;
	mitDrop = mit->second.find( iDifficulty );
	if( mitDrop == mit->second.end() )
	{
		START_LOG( cerr, L"난이도 데이터가 이상하다?" )
			<< BUILD_LOG( iDifficulty )
			<< BUILD_LOG( iNpcLv )
			<< END_LOG;
		return false;
	}

	int iDropGroupID = mitDrop->second;

	// 3. 드롭테이블[던전]의 그룹ID를 얻자
	std::map< int, KLottery >::const_iterator mitRandom;
	mitRandom = m_mapAttribDropGroup.find( iDropGroupID );
	if( mitRandom == m_mapAttribDropGroup.end() )
	{
		START_LOG( cerr, L"존재하지 않는 드롭 테이블 그룹ID입니다." )
			<< BUILD_LOG( iDifficulty )
			<< BUILD_LOG( iNpcLv )
			<< BUILD_LOG( iDropGroupID )
			<< END_LOG;
		return false;
	}

	if( fUserContribution <= 0.0f )
		return true;

	KLottery kDropInfoCopy = mitRandom->second;
	kDropInfoCopy.AddMultiProbRate( fUserContribution );

	//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT

#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	while( 0.0f < fDropRate )
	{
		if( fDropRate < 100.0f )
		{
			kDropInfoCopy.AddMultiProbRate( fDropRate );
			fDropRate = 0.0f;
		}
		else
		{
			fDropRate -= 100.0f;
		}

		int iDropTableGroupID = kDropInfoCopy.Decision();
		if( iDropTableGroupID == KLottery::CASE_BLANK )   // 획득한 아이템 없음.
			continue;

		int iDropItemID = 0;

		// 4. 드롭테이블[던전]을 참조하여 드롭아이템을 결정하자!
		if( !SiKDropTable()->DropGroupItem( iDropTableGroupID, iDropItemID ) )
			continue;		// 없을 수도 있다

		// 아이템 드롭을 안 할 수도 있다. 에러가 아니다.
		if( iDropItemID <= 0 )
			continue;

		vecDropItem.push_back( iDropItemID );
	}
#else // SERV_DROP_EVENT_RENEWAL
	for( int iCount = 0 ; iCount < iDropCount ; ++iCount )
	{
		int iDropTableGroupID = kDropInfoCopy.Decision();
		int iDropItemID = 0;

		// 4. 드롭테이블[던전]을 참조하여 드롭아이템을 결정하자!
		if( !SiKDropTable()->DropGroupItem( iDropTableGroupID, iDropItemID ) )
			continue;		// 없을 수도 있다

		// 아이템 드롭을 안 할 수도 있다. 에러가 아니다.
		if( iDropItemID <= 0 )
			continue;

		vecDropItem.push_back( iDropItemID );
	}
#endif // SERV_DROP_EVENT_RENEWAL

#else
	int iDropTableGroupID = kDropInfoCopy.Decision();
	int iDropItemID = 0;

	// 4. 드롭테이블[던전]을 참조하여 드롭아이템을 결정하자!
	if( !SiKDropTable()->DropGroupItem( iDropTableGroupID, iDropItemID ) )
		return true;		// 없을 수도 있다

	// 아이템 드롭을 안 할 수도 있다. 에러가 아니다.
	if( iDropItemID <= 0 )
		return true;

	vecDropItem.push_back( iDropItemID );
#endif SERV_ITEM_DROP_EVENT
	//}}
	
	return true;
}
#else
bool KAttribNpcTable::AttribNpcDropItem( IN int iDifficulty, IN int iNpcLv, IN int iDropItemNum, OUT std::vector< int >& vecDropItem )
{
	vecDropItem.clear();

	// 1. 해당 npc 레벨의 드롭 테이블을 찾아보자
	std::map< int, KAttribDropTable >::const_iterator mit;
	mit = m_mapAttribDropTable.find( iNpcLv );
	if( mit == m_mapAttribDropTable.end() )
	{
		START_LOG( cerr, L"드롭테이블에 등록되지 않은 NPC Lv정보." )		
			<< BUILD_LOG( iNpcLv )
			<< END_LOG;
		return false;
	}

	// 2. 해당 난이도의 드롭 테이블을 찾아보자
	std::map< int, int >::const_iterator mitDrop;
	mitDrop = mit->second.find( iDifficulty );
	if( mitDrop == mit->second.end() )
	{
		START_LOG( cerr, L"난이도 데이터가 이상하다?" )
			<< BUILD_LOG( iDifficulty )
			<< BUILD_LOG( iNpcLv )
			<< END_LOG;
		return false;
	}

	int iDropGroupID = mitDrop->second;

	// 3. 드롭테이블[던전]의 그룹ID를 얻자
	std::map< int, KLottery >::const_iterator mitRandom;
	mitRandom = m_mapAttribDropGroup.find( iDropGroupID );
	if( mitRandom == m_mapAttribDropGroup.end() )
	{
		START_LOG( cerr, L"존재하지 않는 드롭 테이블 그룹ID입니다." )
			<< BUILD_LOG( iDifficulty )
			<< BUILD_LOG( iNpcLv )
			<< BUILD_LOG( iDropGroupID )
			<< END_LOG;
		return false;
	}

	for( int i = 0; i < iDropItemNum; ++i )
	{
		int iDropTableGroupID = mitRandom->second.Decision();
		int iDropItemID = 0;

		// 4. 드롭테이블[던전]을 참조하여 드롭아이템을 결정하자!
		if( !SiKDropTable()->DropGroupItem( iDropTableGroupID, iDropItemID ) )
			continue;

		if( iDropItemID <= 0 )
			continue;

		vecDropItem.push_back( iDropItemID );
	}

	return true;
}
#endif SERV_REFORM_ITEM_DROP
//}}

bool KAttribNpcTable::IsNoDropNpc( IN int iNpcID )
{
	std::set< int >::const_iterator sit;
	sit = m_setNoDropNpc.find( iNpcID );
	if( sit == m_setNoDropNpc.end() )
		return false;

	return true;
}

//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
bool KAttribNpcTable::GetAttribNpcDropCount( IN char cDungeonType, IN char cMode, IN int iPalyerCount, OUT int& iSubStageCount, OUT int& iDropNpcCount )
{
	iSubStageCount = 0;
	iDropNpcCount = 0;

	std::map< KAttribNpcDropKey, KDropNpcCount >::const_iterator mit_drop;
	mit_drop = m_mapDropNpcCount.find( KAttribNpcDropKey( cDungeonType, cMode ) );
	if( mit_drop == m_mapDropNpcCount.end() )
	{
		// 정보가 없는경우는 엘리트 몬스터가 등장하지 않습니다.
		return false;
	}

	std::map< KAttribNpcCountKey, int >::const_iterator mit_count;
	mit_count = m_mapAttribNpcCount.find( KAttribNpcCountKey( cDungeonType, iPalyerCount ) );
	if( mit_count == m_mapAttribNpcCount.end() )
	{
		// 정보가 없는경우는 엘리트 몬스터가 등장하지 않습니다.
		return false;
	}

	iSubStageCount = mit_drop->second.m_iSubStageCount;
	iDropNpcCount  = mit_count->second;

	return true;
}

#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}