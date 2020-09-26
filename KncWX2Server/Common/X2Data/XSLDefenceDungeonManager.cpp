#include ".\XSLDefenceDungeonManager.h"
#include "NetError.h"


//{{ 2013. 04. 12	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON


ImplementRefreshSingleton( CXSLDefenceDungeonManager );

CXSLDefenceDungeonManager::CXSLDefenceDungeonManager(void) :
m_iQuickSlotPotionItemID( 0 ),
m_fNotOpenGateMultiplyValue( 0.f )
{
	
}

CXSLDefenceDungeonManager::~CXSLDefenceDungeonManager(void)
{
}

ImplToStringW( CXSLDefenceDungeonManager )
{
	stm_	<< L"----------[ Defence Dungeon Manager ]----------" << std::endl
			<< TOSTRINGW( m_fNotOpenGateMultiplyValue )
			<< TOSTRINGW( m_iQuickSlotPotionItemID )
			<< TOSTRINGW( m_vecEnterLevelCondition.size() )
			<< TOSTRINGW( m_setNotBeginDefenceDungeonID.size() )
			<< TOSTRINGW( m_setDefenceGuardianMonster.size() )
			<< TOSTRINGW( m_setQuickSlotRandomGroupID.size() )
			<< TOSTRINGW( m_mapQuickSlotItemGroup.size() )
			<< TOSTRINGW( m_mapSocketOptRandomGroup.size() )
			<< TOSTRINGW( m_mapManufactureItem.size() )
			<< TOSTRINGW( m_mapSpecialCardGroup.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLDefenceDungeonManager )
{
	lua_tinker::class_add<CXSLDefenceDungeonManager>( GetLuaState(), "CXSLDefenceDungeonManager" );
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "SetNotOpenGateMultiply",				&CXSLDefenceDungeonManager::SetNotOpenGateMultiply_LUA );
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "AddEnterLevelCondition",				&CXSLDefenceDungeonManager::AddEnterLevelCondition_LUA );
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "SetDefenceDungeonQuickSlotPotion",	&CXSLDefenceDungeonManager::SetDefenceDungeonQuickSlotPotion_LUA );
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "AddNotBeginDefenceDungeonID",			&CXSLDefenceDungeonManager::AddNotBeginDefenceDungeonID_LUA );	
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "AddDefenceGuardianMonster",			&CXSLDefenceDungeonManager::AddDefenceGuardianMonster_LUA );
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "AddDefenceDungeonQuickSlotItem",		&CXSLDefenceDungeonManager::AddDefenceDungeonQuickSlotItem_LUA );
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "AddSocketOptionRandomGroup",			&CXSLDefenceDungeonManager::AddSocketOptionRandomGroup_LUA );
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "AddItemInfoForManufacture",			&CXSLDefenceDungeonManager::AddItemInfoForManufacture_LUA );
	//{{ 2013. 06. 05	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "AddOldDefenceItemID",					&CXSLDefenceDungeonManager::AddOldDefenceItemID_LUA );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	lua_tinker::class_def<CXSLDefenceDungeonManager>( GetLuaState(), "dump",								&CXSLDefenceDungeonManager::Dump );
	
	lua_tinker::decl( GetLuaState(), "DefenceDungeonManager", this );
}

void CXSLDefenceDungeonManager::SetNotOpenGateMultiply_LUA( float fMultiplyValue )
{
	m_fNotOpenGateMultiplyValue = fMultiplyValue;
}

void CXSLDefenceDungeonManager::AddEnterLevelCondition_LUA( int iLevel, float fRate )
{
	if( iLevel < 0  ||  fRate < 0.f )
	{
		START_LOG( cerr, L"잘못된 정보 입니다!" )
			<< BUILD_LOG( iLevel )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return;
	}

    SEnterLevelConditionInfo kInfo;
	kInfo.m_iLevel = iLevel;
	kInfo.m_fRate = fRate;
	m_vecEnterLevelCondition.push_back( kInfo );
}

void CXSLDefenceDungeonManager::SetDefenceDungeonQuickSlotPotion_LUA( int iItemID )
{
	m_iQuickSlotPotionItemID = iItemID;
}

void CXSLDefenceDungeonManager::AddNotBeginDefenceDungeonID_LUA( int iDungeonID )
{
	m_setNotBeginDefenceDungeonID.insert( iDungeonID );
}

void CXSLDefenceDungeonManager::AddDefenceGuardianMonster_LUA( int iNpcID )
{
	m_setDefenceGuardianMonster.insert( iNpcID );
}

void CXSLDefenceDungeonManager::AddDefenceDungeonQuickSlotItem_LUA( int iGroupID, int iItemIDPhase1, int iItemIDPhase2, int iItemIDPhase3 )
{
	// 그룹 리스트를 따로 만들어 놓자!
	m_setQuickSlotRandomGroupID.insert( iGroupID );

	// 요건 아이템 정보 포함해서
	std::map< int, SQuickSlotItemGroupInfo >::iterator mit;
	mit = m_mapQuickSlotItemGroup.find( iGroupID );
	if( mit == m_mapQuickSlotItemGroup.end() )
	{
		SQuickSlotItemGroupInfo kInfo;
		kInfo.m_kLotQuickSlotItemGroup[DDP_PHASE_1].AddCaseSameProb( iItemIDPhase1 );
		kInfo.m_kLotQuickSlotItemGroup[DDP_PHASE_2].AddCaseSameProb( iItemIDPhase2 );
		kInfo.m_kLotQuickSlotItemGroup[DDP_PHASE_3].AddCaseSameProb( iItemIDPhase3 );
		m_mapQuickSlotItemGroup.insert( std::make_pair( iGroupID, kInfo ) );
	}
	else
	{
        mit->second.m_kLotQuickSlotItemGroup[DDP_PHASE_1].AddCaseSameProb( iItemIDPhase1 );
		mit->second.m_kLotQuickSlotItemGroup[DDP_PHASE_2].AddCaseSameProb( iItemIDPhase2 );
		mit->second.m_kLotQuickSlotItemGroup[DDP_PHASE_3].AddCaseSameProb( iItemIDPhase3 );
	}
}

void CXSLDefenceDungeonManager::AddSocketOptionRandomGroup_LUA( int iGroupID, int iSocketOptionID, float fRate )
{
	std::map< int, KLottery >::iterator mit;
	mit = m_mapSocketOptRandomGroup.find( iGroupID );
	if( mit == m_mapSocketOptRandomGroup.end() )
	{
		KLottery kLot;
		kLot.AddCase( iSocketOptionID, fRate );
		m_mapSocketOptRandomGroup.insert( std::make_pair( iGroupID, kLot ) );
	}
	else
	{
		mit->second.AddCase( iSocketOptionID, fRate );
	}
}

bool CXSLDefenceDungeonManager::AddItemInfoForManufacture_LUA()
{
	KLuaManager luaManager( GetLuaState() );

	SManufactureItemInfo kSocketOptionGroup;
	int iItemID = 0;
    LUA_GET_VALUE_RETURN(			luaManager, L"m_ItemID",				iItemID,					0, return false );

	if( luaManager.BeginTable( L"OPTION_GROUP" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
			{
				kSocketOptionGroup.m_vecSocketOptionGroup.push_back( buf );
			}

			index++;
		}

		luaManager.EndTable();
	}

	if( luaManager.BeginTable( L"SPECIAL_CARD_GROUP" ) == S_OK )
	{
		int index = 1; 
		while( luaManager.BeginTable( index ) == S_OK )
		{
			int iSocketOptionID = 0;
			int iSpecialCardItemID1 = 0;
			int iSpecialCardItemID2 = 0;
			int iSpecialCardItemID3 = 0;
			LUA_GET_VALUE(	luaManager, L"m_SocketOptionID",			iSocketOptionID,		0 );
			LUA_GET_VALUE(	luaManager, L"m_SpecialCardItemID1",		iSpecialCardItemID1,	0 );
			LUA_GET_VALUE(	luaManager, L"m_SpecialCardItemID2",		iSpecialCardItemID2,	0 );
			LUA_GET_VALUE(	luaManager, L"m_SpecialCardItemID3",		iSpecialCardItemID3,	0 );

			std::map< int, SSpecialCardGroupInfo >::iterator mit;
			mit = m_mapSpecialCardGroup.find( iItemID );
			if( mit == m_mapSpecialCardGroup.end() )
			{
				SSpecialCardGroupInfo kGroupInfo;
				kGroupInfo.m_mapSpecialCard[DDP_PHASE_1].insert( std::make_pair( iSocketOptionID, iSpecialCardItemID1 ) );
				kGroupInfo.m_mapSpecialCard[DDP_PHASE_2].insert( std::make_pair( iSocketOptionID, iSpecialCardItemID2 ) );
				kGroupInfo.m_mapSpecialCard[DDP_PHASE_3].insert( std::make_pair( iSocketOptionID, iSpecialCardItemID3 ) );
				m_mapSpecialCardGroup.insert( std::make_pair( iItemID, kGroupInfo ) );
			}
			else
			{
				mit->second.m_mapSpecialCard[DDP_PHASE_1].insert( std::make_pair( iSocketOptionID, iSpecialCardItemID1 ) );
				mit->second.m_mapSpecialCard[DDP_PHASE_2].insert( std::make_pair( iSocketOptionID, iSpecialCardItemID2 ) );
				mit->second.m_mapSpecialCard[DDP_PHASE_3].insert( std::make_pair( iSocketOptionID, iSpecialCardItemID3 ) );
			}

			index++;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}

	m_mapManufactureItem.insert( std::make_pair( iItemID, kSocketOptionGroup ) );
	return true;
}

bool CXSLDefenceDungeonManager::InsertSocketOptionForDefenceDungeonItem( IN const int iItemID, OUT std::vector< int >& vecSocketOption )
{
	std::map< int, SManufactureItemInfo >::const_iterator mit;
	mit = m_mapManufactureItem.find( iItemID );
	if( mit == m_mapManufactureItem.end() )
	{
		START_LOG( cerr, L"먼저 검사 했을텐데? 등록되지 않은 아이템 입니다!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const int, iGroupID, mit->second.m_vecSocketOptionGroup )
	{
		int iSocketOptionID = 0;

		if( GetResultRandomSocketOption( iGroupID, vecSocketOption, iSocketOptionID ) == false )
		{
			START_LOG( cerr, L"소켓 정보 얻기 실패!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iGroupID )
				<< END_LOG;
			continue;
		}
    
		vecSocketOption.push_back( iSocketOptionID );
	}

	return true;
}

bool CXSLDefenceDungeonManager::GetResultRandomSocketOption( IN const int iGroupID, IN const std::vector< int >& vecNotIncludeOptionID, OUT int& iSocketOptionID ) const
{
	iSocketOptionID = 0;

	std::map< int, KLottery >::const_iterator mit;
	mit = m_mapSocketOptRandomGroup.find( iGroupID );
	if( mit == m_mapSocketOptRandomGroup.end() )
	{
		START_LOG( cerr, L"존재하지 않는 그룹ID입니다!" )
			<< BUILD_LOG( iGroupID )
			<< END_LOG;
		return false;
	}

	// 복사본 만들기
	KLottery kLotSocketOption = mit->second;

	// 이미 획득한 옵션은 제외하자!
	BOOST_TEST_FOREACH( const int, iNotIncludeOptionID, vecNotIncludeOptionID )
	{
        kLotSocketOption.DelProbability( iNotIncludeOptionID );
	}

	// 확률을 100%로 맞추자!
	kLotSocketOption.MakeHundredProbabillty();

	// 자 이제 소켓옵션을 뽑아보자!
	int iResult = kLotSocketOption.Decision();
	if( iResult == KLottery::CASE_BLANK )
	{
		// 확률이 100%가 아닌경우 젤 첨 케이스를 가져온다.
		iResult = kLotSocketOption.GetFirstCase();
		if( iResult == KLottery::CASE_BLANK )
		{
			START_LOG( cerr, L"비어있는 확률이 존재합니다!" )
				<< BUILD_LOG( iGroupID )
				<< END_LOG;
			return false;
		}
	}

	iSocketOptionID = iResult;
	return true;
}

bool CXSLDefenceDungeonManager::GetResultSpecialCardByDefenceDungeonItem( IN const int iDefenceDungeonPhase, IN const int iItemID, IN const std::vector< int >& vecSocketOption, OUT int& iSpecialCardItemID ) const
{
	if( iDefenceDungeonPhase < 0  ||  iDefenceDungeonPhase >= DDP_PHASE_END )
	{
		START_LOG( cerr, L"어둠의 문 페이즈 값이 이상합니다!" )
			<< BUILD_LOG( iDefenceDungeonPhase )
			<< END_LOG;
		return false;
	}

	std::map< int, SSpecialCardGroupInfo >::const_iterator mit;
	mit = m_mapSpecialCardGroup.find( iItemID );
	if( mit == m_mapSpecialCardGroup.end() )
	{
		return false;
	}

	// 스페셜카드를 얻을수 있는 소켓 옵션이 있는지 검사!
	BOOST_TEST_FOREACH( const int, iSocketOptionID, vecSocketOption )
	{
		std::map< int, int >::const_iterator mitSO;
		mitSO = mit->second.m_mapSpecialCard[iDefenceDungeonPhase].find( iSocketOptionID );
		if( mitSO == mit->second.m_mapSpecialCard[iDefenceDungeonPhase].end() )
			continue;

		iSpecialCardItemID = mitSO->second;
		return true;
	}	

	return false;
}

void CXSLDefenceDungeonManager::GetResultQuickSlotItem( IN const int iDefenceDungeonPhase, OUT std::vector< int >& vecQuickSlotItemID ) const
{
	vecQuickSlotItemID.clear();

	if( iDefenceDungeonPhase < 0  ||  iDefenceDungeonPhase >= DDP_PHASE_END )
	{
		START_LOG( cerr, L"어둠의 문 페이즈 값이 이상합니다!" )
			<< BUILD_LOG( iDefenceDungeonPhase )
			<< END_LOG;
		return;
	}

	// 임시로 리스트 따로 복사
	std::set< int > setTempGroupID = m_setQuickSlotRandomGroupID;

	// 퀵슬롯 3칸을 채워야 하므로 3번 돌자!
    for( int iCnt = 0; iCnt < 3; ++iCnt )
	{
        if( setTempGroupID.empty() )
			continue;

		const int iRandomIndex = rand() % setTempGroupID.size();
		
		int iIndex = 0;
		std::set< int >::iterator sit;
		for( sit = setTempGroupID.begin(); sit != setTempGroupID.end(); ++sit, ++iIndex )
		{
			if( iIndex != iRandomIndex )
				continue;

			// 그룹ID
			const int iGroupID = *sit;

			std::map< int, SQuickSlotItemGroupInfo >::const_iterator mit;
			mit = m_mapQuickSlotItemGroup.find( iGroupID );
			if( mit == m_mapQuickSlotItemGroup.end() )
			{
				START_LOG( cerr, L"존재 하지 않는 그룹ID" )
					<< BUILD_LOG( iGroupID )
					<< END_LOG;
				continue;
			}

			// 획득할 퀵슬롯 아이템ID 랜덤 글림!
			const int iItemID = mit->second.m_kLotQuickSlotItemGroup[iDefenceDungeonPhase].DecisionSameProb();

			// 저장하자!
			vecQuickSlotItemID.push_back( iItemID );

			// 다음번엔 안나오도록 리스트에서 지우자!
			setTempGroupID.erase( sit );
			break;
		}
	}
}

float CXSLDefenceDungeonManager::GetEnterDefenceDungeonRate( IN const int iDungeonNpcLevel, IN const std::map< UidType, int >& mapUnitLevelList )
{
	float fResultRate = 0.f;

	std::map< UidType, int >::const_iterator mit;
	for( mit = mapUnitLevelList.begin(); mit != mapUnitLevelList.end(); ++mit )
	{
		const int iUnitLevel = mit->second;
		
		// 조건을 돌면서 포함되는 레벨이 있다면 해당 확률을 합산한다!
		BOOST_TEST_FOREACH( const SEnterLevelConditionInfo&, kConditionInfo, m_vecEnterLevelCondition )
		{
			const int iLevelGap = ::abs( iDungeonNpcLevel - iUnitLevel );

            if( kConditionInfo.m_iLevel >= iLevelGap )
			{
                fResultRate += kConditionInfo.m_fRate;
				break;
			}
		}
	}

	return fResultRate;
}


//{{ 2013. 06. 05	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
void CXSLDefenceDungeonManager::AddOldDefenceItemID_LUA( int iItemID )
{
	if( iItemID <= 0 )
	{
		START_LOG( cerr, L"잘못된 정보 입니다!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return;
	}

	m_setOldDefenceItem.insert( iItemID );
}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}


#endif SERV_NEW_DEFENCE_DUNGEON // 해외팀 위치 변경
//}}