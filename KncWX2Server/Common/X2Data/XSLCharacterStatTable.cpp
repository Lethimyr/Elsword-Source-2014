#include ".\XSLCharacterStatTable.h"
#include "neterror.h"
#include "XSLItemManager.h"
#include <math.h>


//{{ 2013. 05. 20	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05


ImplementRefreshSingleton( CXSLCharacterStatTable );

CXSLCharacterStatTable::CXSLCharacterStatTable(void)
{
}

CXSLCharacterStatTable::~CXSLCharacterStatTable(void)
{	
}

ImplToStringW( CXSLCharacterStatTable )
{
	stm_	<< L"----------[ Character Stat Table ]----------" << std::endl
			<< TOSTRINGW( m_mapCharStatDataTable.size() )
			<< TOSTRINGW( m_mapClassStatDataTable.size() )
			<< TOSTRINGW( m_mapEqipStatDataTable.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLCharacterStatTable )
{
	lua_tinker::class_add<CXSLCharacterStatTable>( GetLuaState(), "CXSLCharacterStatTable" );
	//lua_tinker::class_def<CXSLCharacterStatTable>( GetLuaState(), "AddSocketData_LUA",		 &CXSLCharacterStatTable::AddSocketData_LUA );
	lua_tinker::class_def<CXSLCharacterStatTable>( GetLuaState(), "dump",					 &CXSLCharacterStatTable::Dump );

	lua_tinker::decl( GetLuaState(), "g_pCX2SocketItem", this );
}

bool CXSLCharacterStatTable::OpenScriptFile_CharacterStatTable()
{
	KLuaManager luaManager;

	KAutoPath kautoPaht;
	std::string strFile;

	strFile = "Enum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}

	strFile = "DungeonEnum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}

	strFile = "DLG_Map_Enum.lua";
	kautoPaht.GetPullPath( strFile );

	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}

	strFile = "CharacterStatTable.lua";
	kautoPaht.GetPullPath( strFile );

	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;

		return false;
	}

	// 장비 장착 부위 별 능력치
	if( luaManager.BeginTable( L"EQIP_POSITION_RELATIVE_STAT_TABLE" ) != E_FAIL )
	{
		int iTableIndex = 1;
		while( luaManager.BeginTable( iTableIndex  ) != E_FAIL )
		{
			CXSLUnit::EQIP_POSITION eEqipPosition = CXSLUnit::EP_NONE;

			LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eEqipPosition,		CXSLUnit::EQIP_POSITION,		CXSLUnit::EP_NONE );
			if( CXSLUnit::EP_NONE == eEqipPosition )
			{
				luaManager.EndTable();
				START_LOG( cerr, L"아이템 장착 위치 정보가 이상합니다." )
					<< END_LOG;
				break;
			}

			KCharStatData kStatData;
			LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		kStatData.m_fAtkPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		kStatData.m_fDefPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		kStatData.m_fAtkMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		kStatData.m_fDefMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_HP,				kStatData.m_fBaseHP,		0.f );

			m_mapEqipStatDataTable.insert( std::make_pair( eEqipPosition, kStatData ) );

			++iTableIndex;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}

	// 캐릭터별 능력치
	if( luaManager.BeginTable( L"CHARACTER_RELATIVE_STAT_TABLE" ) != E_FAIL )
	{
		int iTableIndex = 1;
		while( luaManager.BeginTable( iTableIndex  ) != E_FAIL )
		{
			CXSLUnit::UNIT_TYPE eUnitType = CXSLUnit::UT_NONE;

			LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eUnitType,		CXSLUnit::UNIT_TYPE,		CXSLUnit::UT_NONE );
			if( CXSLUnit::UC_NONE == eUnitType )
			{
				luaManager.EndTable();
				START_LOG( cerr, L"유닛 타입 정보가 이상합니다." )
					<< END_LOG;
				break;
			}

			KCharStatData kStatData;
			LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		kStatData.m_fAtkPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		kStatData.m_fDefPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		kStatData.m_fAtkMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		kStatData.m_fDefMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_HP,				kStatData.m_fBaseHP,		0.f );

			m_mapCharStatDataTable.insert( std::make_pair( eUnitType, kStatData ) );

			++iTableIndex;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}

	// 2차 전직별 능력치( 2차전직이 아니라면 캐릭터별 능력치 사용 )
	if( luaManager.BeginTable( L"SECOND_CLASS_RELATIVE_STAT_TABLE" ) != E_FAIL )
	{
		int iTableIndex = 1;
		while( luaManager.BeginTable( iTableIndex  ) != E_FAIL )
		{
			CXSLUnit::UNIT_CLASS eUnitClass = CXSLUnit::UC_NONE;

			LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eUnitClass,		CXSLUnit::UNIT_CLASS,		CXSLUnit::UC_NONE );
			if( CXSLUnit::UT_NONE == eUnitClass )
			{
				luaManager.EndTable();
				START_LOG( cerr, L"유닛 클래스 정보가 이상합니다." )
					<< END_LOG;
				break;
			}

			KCharStatData kStatData;
			LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		kStatData.m_fAtkPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		kStatData.m_fDefPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		kStatData.m_fAtkMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		kStatData.m_fDefMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_HP,				kStatData.m_fBaseHP,		0.f );

			m_mapClassStatDataTable.insert( std::make_pair( eUnitClass, kStatData ) );

			++iTableIndex;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}	

	return true;
}

const CXSLCharacterStatTable::KCharStatData* CXSLCharacterStatTable::GetUnitTypeStatData( IN const int iUnitType ) const
{
	std::map< int, KCharStatData >::const_iterator mit;
	mit = m_mapCharStatDataTable.find( iUnitType );
	if( mit == m_mapCharStatDataTable.end() )
	{
		START_LOG( cerr, L"유닛 타입 정보가 없습니다." )
			<< BUILD_LOG( iUnitType )
			<< END_LOG;
		return NULL;
	}
	
	return &mit->second;
}

const CXSLCharacterStatTable::KCharStatData* CXSLCharacterStatTable::GetUnitClassStatData( IN const int iUnitClass ) const
{
	std::map< int, KCharStatData >::const_iterator mit;
	mit = m_mapClassStatDataTable.find( iUnitClass );
	if( mit == m_mapClassStatDataTable.end() )
	{
		START_LOG( cerr, L"유닛 클래스 정보가 없습니다." )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return NULL;
	}

	return &mit->second;
}

const CXSLCharacterStatTable::KCharStatData* CXSLCharacterStatTable::GetEqipStatData( IN const int iEqipPosition ) const
{
	std::map< int, KCharStatData >::const_iterator mit;
	mit = m_mapEqipStatDataTable.find( iEqipPosition );
	if( mit == m_mapEqipStatDataTable.end() )
	{
		START_LOG( cerr, L"장착 위치 정보가 없습니다." )
			<< BUILD_LOG( iEqipPosition )
			<< END_LOG;
		return NULL;
	}

	return &mit->second;
}

const CXSLCharacterStatTable::KCharStatData* CXSLCharacterStatTable::GetAData( IN const int iUnitType, IN const int iUnitClass ) const
{
	if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(iUnitClass) ) == true )
	{
		return GetUnitClassStatData( iUnitClass );
	}
	else
	{
		return GetUnitTypeStatData( iUnitType );
	}
}

bool CXSLCharacterStatTable::CalcCommonItemStat( IN const CXSLItem::ItemTemplet* pItemTemplet, 
												 IN const char cUnitType, 
												 IN const char cUnitClass, 
												 IN const KStatRelationLevel& kAddStatRelLevel,
												 OUT KStat& kCommonItemStat ) const
{
	//A = 현재 캐릭터 또는 현재 2차 전직 캐릭터의 기본 능력치(테이블 존재) unitclass, unittype
	const KCharStatData* pAData = GetAData( cUnitType, cUnitClass );
	if( IS_NULL( pAData ) )
	{
		START_LOG( cerr, L"A 정보를 구하지 못했습니다." )
			<< BUILD_LOGc( cUnitType )
			<< BUILD_LOGc( cUnitClass )
			<< END_LOG;
		return false;
	}

	//B = 기본 캐릭터의 능력치(테이블 존재) unittype
	const KCharStatData* pBData = GetUnitTypeStatData( cUnitType );
	if( IS_NULL( pBData ) )
	{
		START_LOG( cerr, L"B 정보를 구하지 못했습니다." )
			<< BUILD_LOGc( cUnitType )
			<< END_LOG;
		return false;
	}

	//C = 부위별 능력치 연산 상수(테이블 존재) eqip_position
	const KCharStatData* pCData = GetEqipStatData( pItemTemplet->m_EqipPosition );
	if( IS_NULL( pBData ) )
	{
		START_LOG( cerr, L"C 정보를 구하지 못했습니다." )
			<< BUILD_LOGc( pItemTemplet->m_EqipPosition )
			<< END_LOG;
		return false;
	}

	KStatRelationLevel kStatRelLevel = kAddStatRelLevel;
	kStatRelLevel.m_fBaseHPRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byBaseHPRelLV);
	kStatRelLevel.m_fAtkPhysicRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byAtkPhysicRelLV);
	kStatRelLevel.m_fAtkMagicRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byAtkMagicRelLV);
	kStatRelLevel.m_fDefPhysicRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byDefPhysicRelLV);
	kStatRelLevel.m_fDefMagicRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byDefMagicRelLV);

	// stat계산
	kCommonItemStat.m_iBaseHP	 = CalcHPStat( pAData->m_fBaseHP, 
											   pBData->m_fBaseHP, 
											   pCData->m_fBaseHP, 
											   static_cast<float>(pItemTemplet->m_UseLevel), 
											   kStatRelLevel.m_fBaseHPRelLV
											   );

	kCommonItemStat.m_iAtkPhysic = CalcAtkDefStat( pAData->m_fAtkPhysic, 
													pBData->m_fAtkPhysic, 
													pCData->m_fAtkPhysic, 
													static_cast<float>(pItemTemplet->m_UseLevel), 
													kStatRelLevel.m_fAtkPhysicRelLV
													);

	kCommonItemStat.m_iAtkMagic = CalcAtkDefStat( pAData->m_fAtkMagic, 
													pBData->m_fAtkMagic, 
													pCData->m_fAtkMagic, 
													static_cast<float>(pItemTemplet->m_UseLevel), 
													kStatRelLevel.m_fAtkMagicRelLV
													);

	kCommonItemStat.m_iDefPhysic = CalcAtkDefStat( pAData->m_fDefPhysic, 
													pBData->m_fDefPhysic, 
													pCData->m_fDefPhysic, 
													static_cast<float>(pItemTemplet->m_UseLevel), 
													kStatRelLevel.m_fDefPhysicRelLV
													);

	kCommonItemStat.m_iDefMagic = CalcAtkDefStat( pAData->m_fDefMagic, 
													pBData->m_fDefMagic, 
													pCData->m_fDefMagic, 
													static_cast<float>(pItemTemplet->m_UseLevel), 
													kStatRelLevel.m_fDefMagicRelLV
													);

	return true;
}

int	CXSLCharacterStatTable::CalcAtkDefStat( IN const float fSecondJobStat, IN const float fUnitTypeStat, IN const float fEquipPosStat, IN const float fUseLevel, IN const float fItemAddLv ) const
{
	//①물리/마법 공격력 + 물리/마법 방어력 부위별 능력치 연산 수식
	//= ROUND((C*[{A+A*(D+E-1)*0.15}-[(B*0.8)+0.15*{B*(D+E-1)*0.15}]]),0)   --소수점 1자리 반올림 연산.	

	// 2차전직 능력치 계산
	float fCal1 = fSecondJobStat + fSecondJobStat * ( fUseLevel + fItemAddLv - 1.f ) * 0.15f;

	// 기본 캐릭터 능력치 계산
	float fCal2 = fUnitTypeStat * 0.8f;
	float fCal3 = fUnitTypeStat * ( fUseLevel + fItemAddLv - 1.f ) * 0.0225f;

	// 아이템 부위 능력치 계산
	float fCal4 = fEquipPosStat * ( fCal1 - fCal2 - fCal3 );

	// 반올림해서 반환
	return static_cast<int>( ::floor( fCal4 + 0.5f ) );
}

int	CXSLCharacterStatTable::CalcHPStat( IN const float fSecondJobStat, IN const float fUnitTypeStat, IN const float fEquipPosStat, IN const float fUseLevel, IN const float fItemAddLv ) const
{
	//②HP 부위별 능력치 연산 수식
	//= ROUND((C*(E*[{A+A*(D-1)*0.15}-[(B*0.8)+0.15*{B*(D-1)*0.15}]])),0)   --소수점 1자리 반올림 연산.

	// 2차전직 능력치 계산
	float fCal1 = fSecondJobStat + fSecondJobStat * ( fUseLevel - 1.f ) * 0.15f;

	// 기본 캐릭터 능력치 계산
	float fCal2 = fUnitTypeStat;
	float fCal3 = fUnitTypeStat * ( fUseLevel - 1.f ) * 0.15f;

	// 아이템 레벨 능력치 계산
	float fCal4 = ( fItemAddLv / 10 ) * ( fCal1 - fCal2 - fCal3 );

	// 아이템 부위 능력치 계산
	float fCal5 = fEquipPosStat * fCal4;

	// 반올림해서 반환gpd
	return static_cast<int>( ::floor( fCal5 + 0.5 ) );
}


#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}


