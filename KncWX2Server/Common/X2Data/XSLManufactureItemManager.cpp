#include ".\xslmanufactureitemmanager.h"
#include "Enum\Enum.h"

ImplementRefreshSingleton( CXSLManufactureItemManager );

CXSLManufactureItemManager::CXSLManufactureItemManager(void)
{
}

CXSLManufactureItemManager::~CXSLManufactureItemManager(void)
{
}

ImplToStringW( CXSLManufactureItemManager )
{
	stm_	<< L"----------[ Manufacture Item Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapManufacture.size() )
			<< TOSTRINGW( m_mapResultGroup.size() )
			;

	return stm_;
}

ImplementLuaScriptParser( CXSLManufactureItemManager )
{
	lua_tinker::class_add<CXSLManufactureItemManager>( GetLuaState(), "CXSLManufactureItemManager" );
	lua_tinker::class_def<CXSLManufactureItemManager>( GetLuaState(), "AddManufactureTemplet",				&CXSLManufactureItemManager::AddManufactureTemplet_LUA );
	lua_tinker::class_def<CXSLManufactureItemManager>( GetLuaState(), "AddManufactureResultTemplet",		&CXSLManufactureItemManager::AddManufactureResultTemplet_LUA );
	lua_tinker::class_def<CXSLManufactureItemManager>( GetLuaState(), "AddManufactureVillageData",			&CXSLManufactureItemManager::AddManufactureVillageData_LUA );
	lua_tinker::class_def<CXSLManufactureItemManager>( GetLuaState(), "AddManufactureResultGroup",			&CXSLManufactureItemManager::AddManufactureResultGroup_LUA );
	lua_tinker::class_def<CXSLManufactureItemManager>( GetLuaState(), "AddManufactureResultGroupWithRate",	&CXSLManufactureItemManager::AddManufactureResultGroupWithRate_LUA );

	//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN
	lua_tinker::class_def<CXSLManufactureItemManager>( GetLuaState(), "CheckSealedState",	&CXSLManufactureItemManager::CheckSealedState_Lua );
#endif SERV_ADD_SEALED_ITEM_SIGN
	//}}


	lua_tinker::decl( GetLuaState(), "g_pManufactureItemManager", this );
}

bool CXSLManufactureItemManager::AddManufactureTemplet_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	TableBind( &luaManager );

	ManufactureData kManufactureData;
	int iManufactureID = 0;

	LUA_GET_VALUE_RETURN( luaManager, L"m_ManufactureID",	iManufactureID,						0,	return false; );
	LUA_GET_VALUE_RETURN( luaManager, L"m_ResultGroupID",	kManufactureData.m_ResultGroupID,	0,	return false; );
	LUA_GET_VALUE_RETURN( luaManager, L"m_Cost",			kManufactureData.m_Cost,			0,	return false; );

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 12. 18  최육사	서버군 확장
	int iServerGroupID = 0;
	LUA_GET_VALUE(					luaManager, L"m_iServerGroupID",	iServerGroupID,					-1 );
#ifdef EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID != -1 )
#else EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID != SEnum::SGI_INVALID )
#endif EXTEND_SERVER_GROUP_MASK
	{
		if( SEnum::IsValidServerGroupID( iServerGroupID ) == false )
		{
			START_LOG( cerr, L"정상적인 서버군ID값이 아닙니다." )
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( iManufactureID );
			return false;
		}

		if( KBaseServer::GetKObj()->GetServerGroupID() != iServerGroupID )
		{
			START_LOG( clog2, L"서버군이 다르므로 해당 제조는 파싱하지 않습니다." )
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( iManufactureID );

			//{{ 2011. 12. 14	최육사	서버 에러 로그 버그 수정
#ifdef SERV_SERVER_ERROR_LOG_BUG_FIX
			m_setDifServerGroupManufactureID.insert( iManufactureID );
#endif SERV_SERVER_ERROR_LOG_BUG_FIX
			//}}
			return true;
		}
	}
	//}}
	//////////////////////////////////////////////////////////////////////////

	if( luaManager.BeginTable( "m_MaterialTable" ) == S_OK )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == S_OK )
		{
			MaterialData materialData;
			LUA_GET_VALUE_RETURN( luaManager, L"m_MaterialID",		materialData.m_MaterialItemID,		0,	return false; );
			LUA_GET_VALUE_RETURN( luaManager, L"m_MaterialCount",	materialData.m_MaterialCount,		0,	return false; );

			if( materialData.m_MaterialItemID == 0 || materialData.m_MaterialCount == 0 )
			{
				START_LOG( cerr, L"material load error" )
					<< BUILD_LOG( iManufactureID )
					<< BUILD_LOG( materialData.m_MaterialItemID )
					<< BUILD_LOG( materialData.m_MaterialCount )
					<< END_LOG;
				return false;
			}

			kManufactureData.m_vecMaterials.push_back( materialData );

			luaManager.EndTable();
			index++;
		}
		luaManager.EndTable();
	}
	else
	{		
		START_LOG( cerr, L"ManufactureItemTable.lua(m_MaterialTable) 열기실패.!" )
			<< END_LOG;
		return false;
	}

	m_mapManufacture.insert( std::make_pair( iManufactureID, kManufactureData ) );
	return true;
}

bool CXSLManufactureItemManager::AddManufactureResultTemplet_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	TableBind( &luaManager );

	std::map< int, ManufactureData >::iterator mit;
	int iManufactureID;
	int iResultGroupID;
	float fRate;

	LUA_GET_VALUE_RETURN( luaManager, L"m_ManufactureID",		iManufactureID,				0,	return false; );
	LUA_GET_VALUE_RETURN( luaManager, L"m_ResultGroupID",		iResultGroupID,				0,	return false; );
	LUA_GET_VALUE_RETURN( luaManager, L"m_Rate",				fRate,					 0.0f,	return false; );

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 12. 18  최육사	서버군 확장
	int iServerGroupID = 0;
	LUA_GET_VALUE(					luaManager, L"m_iServerGroupID",	iServerGroupID,					-1 );
#ifdef EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID != -1 )
#else EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID != SEnum::SGI_INVALID )
#endif EXTEND_SERVER_GROUP_MASK
	{
		if( SEnum::IsValidServerGroupID( iServerGroupID ) == false )
		{
			START_LOG( cerr, L"정상적인 서버군ID값이 아닙니다." )
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( iManufactureID );
			return false;
		}

		if( KBaseServer::GetKObj()->GetServerGroupID() != iServerGroupID )
		{
			START_LOG( clog2, L"서버군이 다르므로 해당 제조결과는 파싱하지 않습니다." )
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( iManufactureID );			
			return true;
		}
	}
	//}}
	//////////////////////////////////////////////////////////////////////////
	
	// AddManufactureTemplet_LUA() 함수에서 ManufactureItem.lua를 이미 파싱했기 때문에 해당 제조ID는 이미 map에 있어야함!
	mit = m_mapManufacture.find( iManufactureID );
	if( mit == m_mapManufacture.end() )
	{
		//{{ 2011. 12. 14	최육사	서버 에러 로그 버그 수정
#ifdef SERV_SERVER_ERROR_LOG_BUG_FIX
		if( m_setDifServerGroupManufactureID.find( iManufactureID ) != m_setDifServerGroupManufactureID.end() )
		{
			START_LOG( cout, L"해당 서버군에서는 제조할수 없는 제조ID입니다." )
				<< BUILD_LOG( iManufactureID )
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"잘못된 제조ID입니다." )
				<< BUILD_LOG( iManufactureID )
				<< END_LOG;
		}
#else
		START_LOG( cerr, L"잘못된 제조ID입니다." )
			<< BUILD_LOG( iManufactureID )
			<< END_LOG;
#endif SERV_SERVER_ERROR_LOG_BUG_FIX
		//}}
		return false;
	}

	ManufactureData& kManufactureData = mit->second;

	if( kManufactureData.m_kLotteryResultItem.AddCase( iResultGroupID, fRate ) == false )
	{
		START_LOG( cerr, L"manufacture Lottery error" )
			<< BUILD_LOG( iManufactureID )
			<< BUILD_LOG( iResultGroupID )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}	

	return true;
}

bool CXSLManufactureItemManager::AddManufactureVillageData_LUA( int iManufactureID, int iVillageMapID )
{
#ifdef SERV_CHECK_POSSIBLE_MANUFACTURE_ID	// 적용날짜: 미정
	if( iManufactureID <= 0  ||  iVillageMapID <= 0 )
	{
		START_LOG( cerr, L"Manufacture Village Data 정보가 이상합니다!" )
			<< BUILD_LOG( iManufactureID )
			<< BUILD_LOG( iVillageMapID )
			<< END_LOG;
		return false;
	}

	std::set<int>::iterator sit = m_setPossibleManufactureID.find(iManufactureID);
	if( sit != m_setPossibleManufactureID.end() )
	{
		START_LOG( clog, L"Manufacture Village Data 정보가 중복입니다." )
			<< BUILD_LOG( iManufactureID )
			<< BUILD_LOG( iVillageMapID )
			<< END_LOG;
		return false;
	}

	m_setPossibleManufactureID.insert( iManufactureID );

	return true;
#else
	// 마을별로 구분되는 제조정보를 서버에서도 파싱할지 생각해보자..
	return true;
#endif	// SERV_CHECK_POSSIBLE_MANUFACTURE_ID
}

bool CXSLManufactureItemManager::AddManufactureResultGroup_LUA( int iGroupID, int iItemID )
{
	return true;
}

//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN

#ifdef SERV_MANUFACTURE_PERIOD
bool CXSLManufactureItemManager::AddManufactureResultGroupWithRate_LUA( int iGroupID, int iItemID, float fRate, bool bSealed /*= false */, int iPeriod /*= 0*/ )
#else //SERV_MANUFACTURE_PERIOD
bool CXSLManufactureItemManager::AddManufactureResultGroupWithRate_LUA( int iGroupID, int iItemID, float fRate, bool bSealed /*= false */)
#endif //SERV_MANUFACTURE_PERIOD

#else
bool CXSLManufactureItemManager::AddManufactureResultGroupWithRate_LUA( int iGroupID, int iItemID, float fRate )
#endif SERV_ADD_SEALED_ITEM_SIGN
//}}
{	
	if( iGroupID <= 0  ||  iItemID <= 0  ||  fRate <= 0.0f )
	{
		START_LOG( cerr, L"Manufacture Result Group 정보가 이상합니다!" )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	std::map< int, KLottery >::iterator mit;
	mit = m_mapResultGroup.find( iGroupID );
	if( mit == m_mapResultGroup.end() )
	{
		KLottery kLot;
#ifdef SERV_MANUFACTURE_PERIOD
		kLot.AddCaseIntegerCast( iItemID, fRate, iPeriod ); // 2012.08.30 lygan_조성욱 // 중국에 이벤트성으로 일부 제조에 소수점 확률일 존재 한다.
#else //SERV_MANUFACTURE_PERIOD
		kLot.AddCase( iItemID, fRate );
#endif //SERV_MANUFACTURE_PERIOD
		
		m_mapResultGroup.insert( std::make_pair( iGroupID, kLot ) );
		
		//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN
		if( bSealed == true )
		{
			m_mapIsSealed.insert( std::make_pair( iItemID, bSealed ) );
		}
#endif SERV_ADD_SEALED_ITEM_SIGN
		//}}

#ifdef SERV_MANUFACTURE_PERIOD_FIX
		if( iPeriod != 0 )
		{
			m_mapPeriodGroup.insert( std::make_pair( iGroupID, iPeriod ) );
			START_LOG( clog2, L"기간제 처음 읽었다." )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( iPeriod )
				<< END_LOG;
		}
#endif SERV_MANUFACTURE_PERIOD_FIX

	}
	else
	{
#ifdef SERV_MANUFACTURE_PERIOD
		mit->second.AddCaseIntegerCast( iItemID, fRate, iPeriod );
#else //SERV_MANUFACTURE_PERIOD
		mit->second.AddCase( iItemID, fRate );
#endif //SERV_MANUFACTURE_PERIOD

#ifdef SERV_MANUFACTURE_PERIOD_FIX
		if( iPeriod != 0 )
		{
			//그룹ID가 여러 결과 ItemID를 가지고 있고 그 ITEMID중 기간제가 있다면 기간을 -1 로 넘긴다. 
			//클라에서 기간이 -1이면 기간제 포함이라 뜬다.
			std::map< int, int >::iterator mit;
			mit = m_mapPeriodGroup.find( iGroupID );
			if( mit == m_mapPeriodGroup.end() )
			{
				m_mapPeriodGroup.insert( std::make_pair( iGroupID, -1 ) );
				START_LOG( clog2, L"기간제 그룹 ID 있는거 또 읽었는데 기간은 처음이네" )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
					<< END_LOG;
			}
			else
			{
				mit->second = -1;
				START_LOG( clog2, L"기간제 그룹 ID 있는거 또 읽었는데 기간도 있었네!" )
					<< BUILD_LOG( iGroupID )
					<< END_LOG;
			}
			
		}
#endif SERV_MANUFACTURE_PERIOD_FIX

	}

	return true;
}


const CXSLManufactureItemManager::ManufactureData* CXSLManufactureItemManager::GetManufactureData( int iManufactureID ) const
{
	std::map< int, ManufactureData >::const_iterator mit;
	mit = m_mapManufacture.find( iManufactureID );
	if( mit == m_mapManufacture.end() )
		return NULL;

	return &mit->second;
}

int CXSLManufactureItemManager::GetManufactureResultItemID( int iGroupID ) const
{
	std::map< int, KLottery >::const_iterator mit;
	mit = m_mapResultGroup.find( iGroupID );
	if( mit == m_mapResultGroup.end() )
	{
		START_LOG( cerr, L"없는 그룹ID가 제조 테이블에 등록되어있다?" )
			<< BUILD_LOG( iGroupID )
			<< END_LOG;

		// 일단 제조 실패 처리!
		return KLottery::CASE_BLANK;
	}

	return mit->second.Decision();
}

#ifdef SERV_MANUFACTURE_PERIOD
int CXSLManufactureItemManager::GetManufactureResultItemIDPeriod(int iGroupID, int iItemID ) const
{
	std::map< int, KLottery >::const_iterator mit;
	mit = m_mapResultGroup.find( iGroupID );
	if( mit == m_mapResultGroup.end() )
	{
		START_LOG( cerr, L"없는 그룹ID가 제조 테이블에 등록되어있다?" )
			<< BUILD_LOG( iGroupID )
			<< END_LOG;

		// 일단 제조 실패 처리!
		return KLottery::CASE_BLANK;
	}
	int iPeriod = 0;
	iPeriod = mit->second.GetParam1(iItemID);
	if(iPeriod == KLottery::CASE_BLANK)
	{
		iPeriod = 0;
	}

	return iPeriod;
}
#endif //SERV_MANUFACTURE_PERIOD

//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN
bool CXSLManufactureItemManager::IsItemSealed( int iItemID ) const
{
	std::map< int, bool >::const_iterator cit;
	cit = m_mapIsSealed.find( iItemID );
	if( cit == m_mapIsSealed.end() )
	{
		// default는 봉인되어 있지 않은 상태로 표시
		return false;
	}

	return cit->second;
}
void CXSLManufactureItemManager::CheckSealedState_Lua( int iItemID ) const
{
	START_LOG( cerr, L"발렌타인 이벤트 '애증의 초콜릿 봉인 여부 정보 확인'" )
		<< BUILD_LOG( m_mapIsSealed.size() )
		<< END_LOG;

	std::map< int, bool >::const_iterator cit;
	int	ItemId;
	bool State;

	if( 0 < iItemID )
	{
		cit = m_mapIsSealed.find( iItemID );
		if( cit == m_mapIsSealed.end() )
		{
			START_LOG( cerr, L"봉인된 상태가 아니거나 해당하는 아이템이 존재하지 않습니다." )
				<< END_LOG;
		}
		else
		{
			ItemId = cit->first;
			State = cit->second;
			START_LOG( cerr, ItemId )
				<< BUILD_LOG( State )
				<< END_LOG;
		}
	}
	else
	{
		for( cit = m_mapIsSealed.begin(); cit != m_mapIsSealed.end(); ++cit )
		{
			ItemId = cit->first;
			State = cit->second;
			START_LOG( cerr, ItemId )
				<< BUILD_LOG( State )
				<< END_LOG;
		}
	}
}
#endif SERV_ADD_SEALED_ITEM_SIGN
//}}

//{{ 2012. 08. 07	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
bool CXSLManufactureItemManager::IsRandomResult( const int iGroupID ) const
{
	std::map< int, KLottery >::const_iterator mit;
	mit = m_mapResultGroup.find( iGroupID );
	if( mit == m_mapResultGroup.end() )
	{
		return true;
	}

	for( std::map< int, KLottery::KCaseUnit >::const_iterator it = mit->second.m_mapCase.begin(); it != mit->second.m_mapCase.end(); ++it )
	{
		if( it->second.m_fProb < 100.0f )
			return true;
	}
	
	return false;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}