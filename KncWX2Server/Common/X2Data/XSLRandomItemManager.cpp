#include ".\xslrandomitemmanager.h"
#include "NetError.h"
#include "XSLItemManager.h"


ImplementRefreshSingleton( CXSLRandomItemManager );

CXSLRandomItemManager::CXSLRandomItemManager(void) :
m_bIsNotifyMsg( false )
{
}

CXSLRandomItemManager::~CXSLRandomItemManager(void)
{
}

ImplToStringW( CXSLRandomItemManager )
{
	stm_	<< L"----------[ Random Item Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapRandomItemKeyTable.size() )
			<< TOSTRINGW( m_mapRandomItem.size() )
			<< TOSTRINGW( m_mapItemGroup.size() )
			<< TOSTRINGW( m_mapAttractionItem.size() )
			//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
			<< TOSTRINGW( m_setSealRandomItem.size() )
			<< TOSTRINGW( m_setNotifyResultItem.size() )
#endif SERV_SEALED_RANDOM_ITEM
			//}}
			<< TOSTRINGW( m_bIsNotifyMsg )
			;

	return stm_;
}

ImplementLuaScriptParser( CXSLRandomItemManager )
{
	lua_tinker::class_add<CXSLRandomItemManager>( GetLuaState(), "CXSLRandomItemManager" );
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "AddRandomItemTemplet",	&CXSLRandomItemManager::AddRandomItemTemplet_LUA );
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "AddRandomItemGroup",		&CXSLRandomItemManager::AddRandomItemGroup_LUA );

#ifdef SERV_CUBE_IN_ITEM_MAPPING
#ifndef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "AddRandomItemMappingData",&CXSLRandomItemManager::AddRandomItemMappingData_LUA );
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "AddRandomItemMappingTime",&CXSLRandomItemManager::AddRandomItemMappingTime_LUA );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
#endif SERV_CUBE_IN_ITEM_MAPPING

	//{{ 2009. 6. 22  최육사	매력아이템 리스트
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "SetNotifyMsg",			&CXSLRandomItemManager::SetNotifyMsg_LUA );
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "AddAttractionItemInfo",	&CXSLRandomItemManager::AddAttractionItemInfo_LUA );
	//}}
	//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "AddSealRandomItemInfo",	&CXSLRandomItemManager::AddSealRandomItemInfo_LUA );
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "AddNotifyResultItemInfo",	&CXSLRandomItemManager::AddNotifyResultItemInfo_LUA );
#endif SERV_SEALED_RANDOM_ITEM
	//}}
	//{{ 2013. 3. 16	박세훈	매력 아이템 스크립트 처리
#ifdef SERV_CHARM_ITEM_SCRIPT
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "AddCharmItem",			&CXSLRandomItemManager::AddCharmItem_LUA );
#endif SERV_CHARM_ITEM_SCRIPT
	//}}
	lua_tinker::class_def<CXSLRandomItemManager>( GetLuaState(), "dump",					&CXSLRandomItemManager::Dump );

	lua_tinker::decl( GetLuaState(), "g_pRandomItemManager", this );
}

bool CXSLRandomItemManager::AddRandomItemTemplet_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	//TableBind( &luaManager );

	RandomItemData randomItemData;

	int iTemp = 0;
	LUA_GET_VALUE_RETURN(	luaManager, L"m_ItemID",				randomItemData.m_ItemID,					0,	goto end_proc );
	//{{ 2011. 12. 14	최육사	서버 에러 로그 버그 수정
#ifdef SERV_SERVER_ERROR_LOG_BUG_FIX
	if( SiCXSLItemManager()->GetItemTemplet( randomItemData.m_ItemID ) == false )
	{
		START_LOG( cwarn, L"Item.lua에 등록되지 않은 아이템 입니다. 랜덤 큐브 테이블에 포함할수 없습니다!" )
			<< BUILD_LOG( randomItemData.m_ItemID );
		return false;
	}
#endif SERV_SERVER_ERROR_LOG_BUG_FIX
	//}}
	LUA_GET_VALUE(			luaManager, L"m_KeyItemID",				randomItemData.m_KeyItemID,					0 );
	LUA_GET_VALUE(			luaManager, L"m_RequiredKeyCount",		randomItemData.m_RequiredKeyCount,			0 );

	LUA_GET_VALUE(			luaManager, L"m_iRessurectionCount",	randomItemData.m_iRessurectionCount,		0 );
	LUA_GET_VALUE(			luaManager, L"m_iRestoreSpirit",		randomItemData.m_iRestoreSpirit,			0 );

	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	LUA_GET_VALUE(			luaManager, L"m_iRequiredED",			randomItemData.m_iRequiredED,			0 );	
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}

	LUA_GET_VALUE(			luaManager, L"m_bGiveAll",			iTemp,	0 );
	if( iTemp == 0 )
		randomItemData.m_bGiveAll = false;
	else
		randomItemData.m_bGiveAll = true;

	//{{ 2012. 10. 16	박세훈	RandomBoxTable 구조 변경
#ifdef SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
	LUA_GET_VALUE(			luaManager, L"m_UseCondition",		iTemp,	0 );
	randomItemData.m_byteUseCondition	= static_cast<byte>( iTemp );
	if( ( randomItemData.m_byteUseCondition <= CXSLItem::UC_NONE ) || ( CXSLItem::UC_ONE_CLASS < randomItemData.m_byteUseCondition ) )
	{
		START_LOG( cerr, L"유효한 USE_CONDITION 값이 아닙니다." )
			<< BUILD_LOG( randomItemData.m_byteUseCondition )
			<< END_LOG;
		goto end_proc;
	}
#else
	LUA_GET_VALUE(			luaManager, L"m_bUnitSelect",		iTemp,	0 );
	if( iTemp == 0 )
		randomItemData.m_bUnitSelect = false;
	else
		randomItemData.m_bUnitSelect = true;

	LUA_GET_VALUE(			luaManager, L"m_bGiveCharacter",	iTemp,	0 );
	if( iTemp == 0 )
		randomItemData.m_bGiveCharacter = false;
	else
		randomItemData.m_bGiveCharacter = true;	
#endif SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
	//}}

	//{{  2012.09.13 조효진	  신 기획 툴 RandomBox 출력 수정 변경에 따른 코드 수정
#ifdef SERV_MODIFY_RANDOMBOX_LOAD
	// 신툴 개편후 부활석 큐브와 근성 포션 큐브는 UnitClass, GroupId 정보 출력 안됨

	// 부활석 큐브 처리 
	if (randomItemData.m_iRessurectionCount > 0)
	{
		// Default 값 0,0 으로 세팅 
		RandomUnitData unitData;
		unitData.m_cUnitClass = 0;
		unitData.m_iItemGroupID = 0;

		// 부활석 큐브
		randomItemData.m_vecUnitData.push_back( unitData );
	}
	// 근성 포션
	else if (randomItemData.m_iRestoreSpirit > 0)
	{
		// Default 값 0,0 으로 세팅 
		RandomUnitData unitData;
		unitData.m_cUnitClass = 0;
		unitData.m_iItemGroupID = 0;

		// 근성도 큐브
		randomItemData.m_vecUnitData.push_back( unitData );
	}
	else
	{
		int index = 1; 
		while( luaManager.BeginTable( index ) == S_OK )
		{
			RandomUnitData unitData;
			LUA_GET_VALUE(	luaManager, L"m_cUnitClass",	iTemp,		0 );
			unitData.m_cUnitClass = static_cast<char>(iTemp);

			//{{ 2012. 10. 16	박세훈	RandomBoxTable 구조 변경
#ifdef SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
			LUA_GET_VALUE(	luaManager, L"m_iItemGroupID",	unitData.m_iItemGroupID,	0 );

			if( randomItemData.m_byteUseCondition == CXSLItem::UC_ANYONE )
			{
				if( unitData.m_cUnitClass != CXSLUnit::UC_NONE )
				{
					START_LOG( cerr, L"m_UseCondition, m_cUnitClass 에 설정된 값이 의미상 일치하지 않습니다.")
						<< BUILD_LOG( randomItemData.m_byteUseCondition )
						<< BUILD_LOG( unitData.m_cUnitClass )
						<< END_LOG;
					goto end_proc;
				}
			}
			else if( randomItemData.m_byteUseCondition == CXSLItem::UC_ONE_UNIT )
			{
				if( ( unitData.m_cUnitClass != CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>( unitData.m_cUnitClass ) ) ) ||
					( unitData.m_iItemGroupID == 0 ) )
				{
					START_LOG( cerr, L"정상적인 큐브가 아닙니다!")
						<< BUILD_LOG( randomItemData.m_byteUseCondition )
						<< BUILD_LOGc( unitData.m_cUnitClass )
						<< BUILD_LOG( unitData.m_iItemGroupID )
						<< END_LOG;
					goto end_proc;
				}
			}
			else if( ( CXSLUnit::IsValidUnitClass( static_cast<CXSLUnit::UNIT_CLASS>( unitData.m_cUnitClass ) ) != true ) ||
					 ( unitData.m_iItemGroupID == 0 ) )
			{
				START_LOG( cerr, L"정상적인 큐브가 아닙니다!")
					<< BUILD_LOG( randomItemData.m_byteUseCondition )
					<< BUILD_LOG( unitData.m_cUnitClass )
					<< BUILD_LOG( unitData.m_iItemGroupID )
					<< END_LOG;
				goto end_proc;
			}

			// 일반 큐브 
			randomItemData.m_vecUnitData.push_back( unitData );
#else
			LUA_GET_VALUE(	luaManager, L"m_iItemGroupID",	unitData.m_iItemGroupID,	0 );

			if( false == randomItemData.m_bUnitSelect && 0 != unitData.m_iItemGroupID )
			{
				// 일반 큐브 
				randomItemData.m_vecUnitData.push_back( unitData );
			}
			else if( randomItemData.m_bUnitSelect && CXSLUnit::UC_NONE != unitData.m_cUnitClass  &&  0 != unitData.m_iItemGroupID )
			{
				// 일반 큐브
				randomItemData.m_vecUnitData.push_back( unitData );
			}
			else
			{
				START_LOG( cerr, L"정상적인 큐브가 아닙니다!" )
					<< BUILD_LOG( randomItemData.m_ItemID )
					<< BUILD_LOGc( unitData.m_cUnitClass )
					<< BUILD_LOG( unitData.m_iItemGroupID )
					<< END_LOG;

				goto end_proc;
			}
#endif SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
			//}}

			index++;
			luaManager.EndTable();
		}
	}

	if (randomItemData.m_vecUnitData.size() <= 0)
	{
		START_LOG( cerr, L"정상적인 큐브가 아닙니다! m_vecUnitData Size 0임" )
			<< BUILD_LOG( randomItemData.m_ItemID )
			<< BUILD_LOG( randomItemData.m_KeyItemID )
			<< BUILD_LOG( randomItemData.m_RequiredKeyCount )
			<< BUILD_LOG( randomItemData.m_iRessurectionCount )
			<< BUILD_LOG( randomItemData.m_iRestoreSpirit )
			<< END_LOG;

		goto end_proc;
	}

#else //SERV_MODIFY_RANDOMBOX_LOAD
	int index = 1; 
	while( luaManager.BeginTable( index ) == S_OK )
	{
		RandomUnitData unitData;
		LUA_GET_VALUE(	luaManager, L"m_cUnitClass",	iTemp,		0 );
		unitData.m_cUnitClass = static_cast<char>(iTemp);
		LUA_GET_VALUE(	luaManager, L"m_iItemGroupID",	unitData.m_iItemGroupID,	0 );

		if( CXSLUnit::UC_NONE != unitData.m_cUnitClass  &&  0 != unitData.m_iItemGroupID )
		{
			// 일반 큐브
			randomItemData.m_vecUnitData.push_back( unitData );
		}
		else if( CXSLUnit::UC_NONE != unitData.m_cUnitClass  &&  0 < randomItemData.m_iRessurectionCount )
		{
			// 부활석 큐브
			randomItemData.m_vecUnitData.push_back( unitData );
		}
		else if( CXSLUnit::UC_NONE != unitData.m_cUnitClass  &&  0 < randomItemData.m_iRestoreSpirit )
		{
			// 근성도 큐브
			randomItemData.m_vecUnitData.push_back( unitData );
		}
		else
		{
			//{{ 2011. 12. 14	최육사	서버 에러 로그 버그 수정
#ifdef SERV_SERVER_ERROR_LOG_BUG_FIX
			START_LOG( cerr, L"정상적인 큐브가 아닙니다!" )
				<< BUILD_LOG( randomItemData.m_ItemID )
				<< BUILD_LOGc( unitData.m_cUnitClass )
				<< BUILD_LOG( unitData.m_iItemGroupID )
				<< END_LOG;
#endif SERV_SERVER_ERROR_LOG_BUG_FIX
			//}}
			goto end_proc;
		}
		index++;
		luaManager.EndTable();
	}

#endif SERV_MODIFY_RANDOMBOX_LOAD
	//}}

	if( randomItemData.m_iRessurectionCount < 0 )
		goto end_proc;

	// 이미 등록된 랜덤 테이블인지 검사한다!
	if( GetRandomItemData( randomItemData.m_ItemID, randomItemData.m_KeyItemID ) != NULL )
	{
		START_LOG( cerr, L"이미 등록된 랜덤 아이템 테이블입니다. 데이터가 중복되네요.." )
			<< BUILD_LOG( randomItemData.m_ItemID )
			<< BUILD_LOG( randomItemData.m_KeyItemID )
			<< END_LOG;

		goto end_proc;
	}

	//{{ 2009. 12. 5  최육사	랜덤큐브시스템개편
	{
		RandomItemKey kRandomItemKey( randomItemData.m_ItemID, randomItemData.m_KeyItemID );
		m_mapRandomItem.insert( std::make_pair( kRandomItemKey, randomItemData ) );
	}

	{
		std::map< int, std::map< int, int > >::iterator mitKey = m_mapRandomItemKeyTable.find( randomItemData.m_ItemID );
		if( mitKey == m_mapRandomItemKeyTable.end() )
		{
			std::map< int, int  > mapKeyInfo;
			mapKeyInfo.insert( std::make_pair( randomItemData.m_KeyItemID, randomItemData.m_RequiredKeyCount ) );
			m_mapRandomItemKeyTable.insert( std::make_pair( randomItemData.m_ItemID, mapKeyInfo ) );
		}
		else
		{
			mitKey->second.insert( std::make_pair( randomItemData.m_KeyItemID, randomItemData.m_RequiredKeyCount ) );
		}
	}
	//}}

	return true;

end_proc:
	START_LOG( cerr, L"랜덤 상자 데이터 파싱 실패.!" )
		<< BUILD_LOG( randomItemData.m_ItemID )
		<< END_LOG;

	return false;
}

bool CXSLRandomItemManager::AddRandomItemGroup_LUA( int iGroupID, int iItemID, float fRate, int iPeriod, int iQuantity )
{
	//if( fRate == 0.0f )
	//{
	//	START_LOG( clog, L"기획의도에 의해 0%로 설정된 데이터이므로 무시하고 넘어간다." )
	//		<< BUILD_LOG( iGroupID )
	//		<< BUILD_LOG( iItemID )
	//		<< BUILD_LOG( iPeriod )
	//		<< BUILD_LOG( iQuantity );

	//	return true;
	//}

	if( iGroupID <= 0 || iItemID <= 0 || fRate <= 0.0f || iPeriod < 0  || iQuantity < 0 )
	{
		START_LOG( cerr, L"랜덤 아이템 그룹데이터 파싱 실패.!" )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( fRate )
			<< BUILD_LOG( iPeriod )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;

		return false;
	}

	ItemGroupData itemData;
	itemData.m_iItemID		= iItemID;
	itemData.m_fRate		= fRate;
	itemData.m_iPeriod		= iPeriod;
	itemData.m_iQuantity	= iQuantity;

	std::map< int, ItemGroupDataList >::iterator mit;
	mit = m_mapItemGroup.find( iGroupID );

	if( mit != m_mapItemGroup.end() )
	{
		mit->second.m_vecItemData.push_back( itemData );
#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
		if( mit->second.m_kLottery.AddDuplicateCaseIntegerCast( iItemID, fRate, iPeriod, iQuantity ) == false )
#else
		if( mit->second.m_kLottery.AddCaseIntegerCast( iItemID, fRate, iPeriod, iQuantity ) == false )
#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP   
		{
			START_LOG( cerr, L"랜덤 아이템 확률 설정 실패!" )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( fRate )
				<< BUILD_LOG( iPeriod )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;
		}
	}
	else
	{
		ItemGroupDataList itemGDList;
		itemGDList.m_iGroupID	= iGroupID;
		itemGDList.m_vecItemData.push_back( itemData );
#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
		if( itemGDList.m_kLottery.AddDuplicateCaseIntegerCast( iItemID, fRate, iPeriod, iQuantity ) == false )
#else
		if( itemGDList.m_kLottery.AddCaseIntegerCast( iItemID, fRate, iPeriod, iQuantity ) == false )
#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
		{
			START_LOG( cerr, L"랜덤 아이템 확률 설정 실패!" )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( fRate )
				<< BUILD_LOG( iPeriod )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;
		}

		m_mapItemGroup.insert( std::make_pair( iGroupID, itemGDList ) );
	}

	return true;
}

#ifdef SERV_CUBE_IN_ITEM_MAPPING
bool CXSLRandomItemManager::ModifyMapItemGroup(RANDOMITEM_MAPPING_DATA randItemMappingData)
{
	RandomItemKey mitRandomItemKey( randItemMappingData.iCubeID, randItemMappingData.iKeyItemID );
	std::map< RandomItemKey, RandomItemData >::iterator mit = m_mapRandomItem.find(mitRandomItemKey);

	if( mit == m_mapRandomItem.end() )
		return false;

	std::vector< RandomUnitData >::iterator mitRUD = mit->second.m_vecUnitData.begin();
	for(; mitRUD != mit->second.m_vecUnitData.end(); mitRUD++)
	{
		if( mitRUD->m_iItemGroupID == randItemMappingData.iBeforeGroupID )
		{
			mitRUD->m_iItemGroupID = randItemMappingData.iAfterGroupID;
			randItemMappingData.bCheckMapping = true;
		}
	}
	return true;
}

bool CXSLRandomItemManager::RestoreMapItemGroup(RANDOMITEM_MAPPING_DATA randItemMappingData)
{
	RandomItemKey mitRandomItemKey( randItemMappingData.iCubeID, randItemMappingData.iKeyItemID );
	std::map< RandomItemKey, RandomItemData >::iterator mit = m_mapRandomItem.find(mitRandomItemKey);

	if( mit == m_mapRandomItem.end() )
		return false;

	std::vector< RandomUnitData >::iterator mitRUD = mit->second.m_vecUnitData.begin();
	for(; mitRUD != mit->second.m_vecUnitData.end(); mitRUD++)
	{
		if( mitRUD->m_iItemGroupID == randItemMappingData.iAfterGroupID )
		{
			mitRUD->m_iItemGroupID = randItemMappingData.iBeforeGroupID;
			randItemMappingData.bCheckMapping = false;
		}
	}
	return true;
}

bool CXSLRandomItemManager::AddRandomItemMappingData_LUA( int nIndex, int iCubeID, int iKeyItemID, int iBeforeGroupID, int iAfterGroupID )
{
	RANDOMITEM_MAPPING_DATA stMappingData;
	std::wstring wstrStartDate, wstrEndDate;

	if( iCubeID <= 0 || iBeforeGroupID <= 0 || iAfterGroupID <= 0 )
	{
		START_LOG( cerr, L"랜덤 아이템 매핑 정보 그룹데이터 파싱 실패.!" )
			<< BUILD_LOG( nIndex )
			<< BUILD_LOG( iCubeID )
			<< BUILD_LOG( iKeyItemID )
			<< BUILD_LOG( iBeforeGroupID )
			<< BUILD_LOG( iAfterGroupID )
			<< END_LOG;

		return false;
	}

	stMappingData.iCubeID = iCubeID;
	stMappingData.iKeyItemID = iKeyItemID;
	stMappingData.iBeforeGroupID = iBeforeGroupID;
	stMappingData.iAfterGroupID = iAfterGroupID;
	stMappingData.bCheckMapping = false;

	m_mapRandomItemMappingData.insert(std::make_pair(nIndex, stMappingData));

	return true;
}

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
bool CXSLRandomItemManager::AddRandomItemMappingTime_LUA( int nIndex,  std::wstring wstrScriptStartDate, std::wstring wstrScriptEndDate )
#else SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
bool CXSLRandomItemManager::AddRandomItemMappingTime_LUA( int nIndex,  const char* szStartDate, const char* szEndDate )
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
{
	RANDOMITEM_MAPPING_TIME stMappingTime;
	std::wstring wstrStartDate, wstrEndDate;

#ifndef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	std::wstring wstrScriptStartDate = KncUtil::toWideString(szStartDate);
	std::wstring wstrScriptEndDate = KncUtil::toWideString(szEndDate);
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	CTime tStartTime, tEndTime;

	if( KncUtil::ConvertStringToCTime( wstrScriptStartDate, tStartTime ) == false)
	{
		START_LOG( cerr, L"tStartTime CTime 형식으로 치환 실패" )
			<< BUILD_LOG( wstrScriptStartDate )
			<< END_LOG;
		return false;
	}

	if( KncUtil::ConvertStringToCTime( wstrScriptEndDate, tEndTime ) == false)
	{
		START_LOG( cerr, L"tEndTime CTime 형식으로 치환 실패" )
			<< BUILD_LOG( wstrScriptEndDate )
			<< END_LOG;
		return false;
	}

	// 시간예외 처리 (월)
	if( tStartTime.GetMonth() > 12 || tEndTime.GetMonth() > 12 )
	{
		START_LOG( cerr, L"1년은 12월까지 있습니다. RandomItemTable.lua 확인" )
			<< BUILD_LOG( nIndex )
			<< BUILD_LOG( wstrScriptStartDate )
			<< BUILD_LOG( wstrScriptEndDate )
			<< END_LOG;
		return false;
	}
	// 시간예외 처리 (일)
	if( tStartTime.GetDay() > 31 || tEndTime.GetDay() > 31 )
	{
		START_LOG( cerr, L"1달은 31일을 넘지 않습니다. RandomItemTable.lua 확인" )
			<< BUILD_LOG( nIndex )
			<< BUILD_LOG( wstrScriptStartDate )
			<< BUILD_LOG( wstrScriptEndDate )
			<< END_LOG;
		return false;
	}
	// 시간예외 처리 (시간)
	if( tStartTime.GetHour() > 23 || tEndTime.GetHour() > 23 )
	{
		START_LOG( cerr, L"1루는 24시간을 넘지 않습니다. RandomItemTable.lua 확인" )
			<< BUILD_LOG( nIndex )
			<< BUILD_LOG( wstrScriptStartDate )
			<< BUILD_LOG( wstrScriptEndDate )
			<< END_LOG;
		return false;
	}
	
	stMappingTime.tStartTime = tStartTime;
	stMappingTime.tEndTime = tEndTime;

	m_mapRandomItemMappingTime.insert(std::make_pair(nIndex, stMappingTime));

	return true;
}
#endif SERV_CUBE_IN_ITEM_MAPPING

//{{ 2009. 6. 22  최육사	매력아이템 리스트
#ifdef SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
bool CXSLRandomItemManager::AddAttractionItemInfo_LUA( int iItemID, bool bNotify, bool bSealed )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템을 매력아이템으로 등록하였음!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	KAttractionItemInfo kInfo;
	kInfo.m_bNotifyMsg = bNotify;
	kInfo.m_iMsgNum = 0;
	m_mapAttractionItem.insert( std::make_pair( iItemID, kInfo ) );
	if( bSealed )
	{
		m_setSealedItemList_Global.insert( iItemID );

		START_LOG( clog, L"봉인 대상 아이템 등록" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
	}

	return true;
}
#else //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
bool CXSLRandomItemManager::AddAttractionItemInfo_LUA( int iItemID, bool bNotify )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템을 매력아이템으로 등록하였음!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	KAttractionItemInfo kInfo;
	kInfo.m_bNotifyMsg = bNotify;
	kInfo.m_iMsgNum = 0;
	m_mapAttractionItem.insert( std::make_pair( iItemID, kInfo ) );
	return true;
}
#endif //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
//}}

//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM

bool CXSLRandomItemManager::AddSealRandomItemInfo_LUA( int iItemID )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템을 밀봉 랜덤 아이템으로 등록하였음!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}
	
	m_setSealRandomItem.insert( iItemID );
	return true;
}

bool CXSLRandomItemManager::AddNotifyResultItemInfo_LUA( int iItemID )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템을 전체 공지 아이템으로 등록하였음!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	m_setNotifyResultItem.insert( iItemID );
	return true;
}

#endif SERV_SEALED_RANDOM_ITEM
//}}

//{{ 2009. 12. 5  최육사	랜덤큐브시스템개편
const std::map< int, int >* CXSLRandomItemManager::GetRandomItemKeyList( IN int iRandomItemID ) const
{
	std::map< int, std::map< int, int > >::const_iterator mit = m_mapRandomItemKeyTable.find( iRandomItemID );
	if( mit == m_mapRandomItemKeyTable.end() )
		return NULL;

	return &mit->second;
}
//}}

const CXSLRandomItemManager::RandomItemData* CXSLRandomItemManager::GetRandomItemData( IN int iRandomItemID, IN int iKeyItemID ) const
{
	std::map< RandomItemKey, RandomItemData >::const_iterator mit;
	mit = m_mapRandomItem.find( RandomItemKey( iRandomItemID, iKeyItemID ) );
	if( mit == m_mapRandomItem.end() )
		return NULL;
	
	return &mit->second;
}

//{{ 2009. 6. 22  최육사	매력아이템 리스트
bool CXSLRandomItemManager::MakeNotifyMsgAttrationItem( IN const int iRewardItemID, IN const std::wstring& wstrNickName, OUT std::wstring& wstrNotifyMsg )
{
	wstrNotifyMsg.clear();

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iRewardItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템입니다!" )
			<< BUILD_LOG( iRewardItemID )
			<< END_LOG;

		return false;
	}

	std::map< int, KAttractionItemInfo >::const_iterator mit;
	mit = m_mapAttractionItem.find( iRewardItemID );
	if( mit == m_mapAttractionItem.end() )
	{
		START_LOG( cerr, L"매력아이템이 아닙니다! 전체공지를 날릴수 없음!" )
			<< BUILD_LOG( iRewardItemID )
			<< END_LOG;

		return false;
	}

	if( mit->second.m_bNotifyMsg == false  ||  m_bIsNotifyMsg == false )
	{
		START_LOG( clog, L"매력아이템 획득했으나 전체 공지 날리지 않음!" )
			<< BUILD_LOG( iRewardItemID )
			<< END_LOG;

		return false;
	}

	//{{ 2011.11.20 임홍락	서버 단일화 // SERV_USE_NATION_FLAG 필수 포함임
#ifdef SERV_UNITED_SERVER_EU
	wchar_t wszItemID[20];
	_itow(pItemTemplet->m_ItemID, wszItemID, 10);
	std::wstring wstrItemID = wszItemID;

	wstrNotifyMsg = wstrNickName;
	wstrNotifyMsg += L";";
	wstrNotifyMsg += wstrItemID;
#else SERV_UNITED_SERVER_EU
	wstrNotifyMsg = wstrNickName;
	wstrNotifyMsg += L";";
	wstrNotifyMsg += pItemTemplet->m_Name;
#endif SERV_UNITED_SERVER_EU
	//}}
  
	return true;
}
//}}

//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM

bool CXSLRandomItemManager::MakeNotifyMsgRandomResultItem( IN const int iRewardItemID, IN const std::wstring& wstrNickName, OUT std::wstring& wstrNotifyMsg )
{
	wstrNotifyMsg.clear();

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iRewardItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템입니다!" )
			<< BUILD_LOG( iRewardItemID )
			<< END_LOG;

		return false;
	}

	//{{ 2011.11.20 임홍락	서버 단일화 // SERV_USE_NATION_FLAG 필수 포함임
#ifdef SERV_UNITED_SERVER_EU
	wchar_t wszItemID[20];
	_itow(pItemTemplet->m_ItemID, wszItemID, 10);
	std::wstring wstrItemID = wszItemID;

	wstrNotifyMsg = wstrNickName;
	wstrNotifyMsg += L";";
	wstrNotifyMsg += wszItemID;
#else SERV_UNITED_SERVER_EU
	wstrNotifyMsg = wstrNickName;
	wstrNotifyMsg += L";";
	wstrNotifyMsg += pItemTemplet->m_Name;
#endif SERV_UNITED_SERVER_EU
	//}}
	
	return true;
}

#endif SERV_SEALED_RANDOM_ITEM
//}}

//{{ 2011. 05. 12  김민성	랜덤 큐브 오픈시 인벤토리 검사 수정
#ifdef SERV_RANDOM_ITEM_CHECK_INVEN
bool CXSLRandomItemManager::GetResultItem( IN char cUnitClass, IN const RandomItemData* pRandomItemData, OUT std::map< int, KItemInfo >& mapResultItem, OUT std::map< int, int >& mapCheckEmpty, OUT bool& bIsGiveAll ) const
#else
bool CXSLRandomItemManager::GetResultItem( IN char cUnitClass, IN const RandomItemData* pRandomItemData, OUT std::map< int, KItemInfo >& mapResultItem, OUT std::map< int, int >& mapCheckEmpty ) const
#endif SERV_RANDOM_ITEM_CHECK_INVEN
//}}
{
	SET_ERROR( NET_OK );

	mapResultItem.clear();
	mapCheckEmpty.clear();

	int iGroupID = 0;
	std::map< int, ItemGroupDataList >::const_iterator mit;

	if( pRandomItemData == NULL )
	{
		START_LOG( cerr, L"랜덤정보 포인터가 이상하다." )
			<< END_LOG;

		goto end_proc;
	}

	//{{ 2012. 10. 16	박세훈	RandomBoxTable 구조 변경
#ifdef SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
	if( pRandomItemData->m_byteUseCondition == CXSLItem::UC_ANYONE )
	{
		if( pRandomItemData->m_vecUnitData.size() > 0 )
		{
			iGroupID = pRandomItemData->m_vecUnitData[0].m_iItemGroupID;
		}
	}
	else
	{
		if( pRandomItemData->m_byteUseCondition == CXSLItem::UC_ONE_UNIT )
		{
			// 랜덤 큐브 캐릭터 UnitType별로 오픈 기능
			// 해당 UnitClass의 기본 UnitClass를 얻는다.
			cUnitClass = static_cast<char>( CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>( cUnitClass ) ) );
		}
		/*else if( pRandomItemData->m_byteUseCondition == CXSLItem::UC_ONE_CLASS )
		{
		}*/

		for( int i = 0; i < (int)pRandomItemData->m_vecUnitData.size(); ++i )
		{
			if( pRandomItemData->m_vecUnitData[i].m_cUnitClass == cUnitClass )
			{
				iGroupID = pRandomItemData->m_vecUnitData[i].m_iItemGroupID;
				break;
			}
		}
	}
#else
	if( pRandomItemData->m_bUnitSelect == true )
	{
		// 랜덤 큐브 캐릭터 UnitType별로 오픈 기능
		if( pRandomItemData->m_bGiveCharacter )
		{
			// 해당 UnitClass의 기본 UnitClass를 얻는다.
			cUnitClass = static_cast<char>( CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>(cUnitClass) ) );
		}

		for( int i = 0; i < (int)pRandomItemData->m_vecUnitData.size(); ++i )
		{
			if( pRandomItemData->m_vecUnitData[i].m_cUnitClass == cUnitClass )
			{
				iGroupID = pRandomItemData->m_vecUnitData[i].m_iItemGroupID;
				break;
			}
		}
	}
	else
	{
		if( pRandomItemData->m_vecUnitData.size() > 0 )
		{
			iGroupID = pRandomItemData->m_vecUnitData[0].m_iItemGroupID;
		}
	}
#endif SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
	//}}

	if( iGroupID == 0  &&  pRandomItemData->m_iRessurectionCount > 0 )
	{
		return true;
	}
	else if( iGroupID == 0  &&  pRandomItemData->m_iRestoreSpirit > 0 )
	{
		return true;
	}
	else if( iGroupID == 0 )
	{
		goto end_proc;
	}

	mit = m_mapItemGroup.find( iGroupID );

	if( mit == m_mapItemGroup.end() )
	{
		goto end_proc;
	}

	//{{ 2011. 05. 12  김민성	랜덤 큐브 오픈시 인벤토리 검사 수정
#ifdef SERV_RANDOM_ITEM_CHECK_INVEN
	bIsGiveAll = pRandomItemData->m_bGiveAll;
#endif SERV_RANDOM_ITEM_CHECK_INVEN
	//}}

	//{{ 2008. 5. 27  최육사  랜덤 결과 인벤 공간 체크
	{	
#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
		std::multimap< int, KLottery::KCaseUnit >::const_iterator mitCase;
		for( mitCase = mit->second.m_kLottery.m_multimapCase.begin(); mitCase != mit->second.m_kLottery.m_multimapCase.end(); ++mitCase )
#else //SERV_DUPLICATE_RANDOM_ITEM_GROUP
		std::map< int, KLottery::KCaseUnit >::const_iterator mitCase;
		for( mitCase = mit->second.m_kLottery.m_mapCase.begin(); mitCase != mit->second.m_kLottery.m_mapCase.end(); ++mitCase )
#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
		{
			mapCheckEmpty.insert( std::make_pair( mitCase->first, mitCase->second.m_nParam2 ) );
		}
	}
	//}}

	if( pRandomItemData->m_bGiveAll == false )
	{
		int iItemID = 0;

#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
		KLottery::KDuplicateCaseResult kDuplicateCaseResult = mit->second.m_kLottery.DuplicateDecision();
		iItemID = kDuplicateCaseResult.m_iItemID;
#else //SERV_DUPLICATE_RANDOM_ITEM_GROUP
		iItemID = mit->second.m_kLottery.Decision();
#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP

		if( iItemID == KLottery::CASE_BLANK )
		{
			goto end_proc;
		}
		
		// 기간제
#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
		int iPeriod = kDuplicateCaseResult.m_nParam1;
#else //SERV_DUPLICATE_RANDOM_ITEM_GROUP
		int iPeriod = mit->second.m_kLottery.GetParam1( iItemID );
#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
		if( iPeriod == KLottery::PARAM_BLANK )
		{
			START_LOG( cerr, L"해당 케이스의 파라미터가 존재하지 않음. 큐브랜덤을 돌렸는데 기간제 정보가 없네? 확률 100프로 확인해보자" )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iPeriod )
				<< END_LOG;

			goto end_proc;
		}

		// 수량
#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
		int iQuantity = kDuplicateCaseResult.m_nParam2;
#else //SERV_DUPLICATE_RANDOM_ITEM_GROUP
		int iQuantity = mit->second.m_kLottery.GetParam2( iItemID );
#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
		if( iQuantity == KLottery::PARAM_BLANK )
		{
			START_LOG( cerr, L"해당 케이스의 파라미터가 존재하지 않음. 큐브랜덤을 돌렸는데 수량 정보가 없네..?  확률 100프로 확인해보자" )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iPeriod )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;

			goto end_proc;
		}
		
		//{{ 2008. 3. 17  최육사  DeleteAndInsert()
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
		}
		else
		{
			KItemInfo kInsertItemInfo;
			kInsertItemInfo.m_iItemID	 = iItemID;
			kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
			kInsertItemInfo.m_iQuantity	 = iQuantity;
			kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;			
			kInsertItemInfo.m_sPeriod	 = static_cast<short>(iPeriod); // 기간제 설정
			//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
#else
			//{{ 2011. 06. 01	최육사	강화된 아이템 보상 큐브
#ifdef SERV_ENCHANTED_ITEM_CUBE_REWARD
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON  || pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
			{
				CXSLItem::UpdateEnchantedItemCubeReward( pRandomItemData->m_ItemID, kInsertItemInfo );
			}
#endif SERV_ENCHANTED_ITEM_CUBE_REWARD
			//}}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}

			mapResultItem.insert( std::make_pair( iItemID, kInsertItemInfo ) );
		}
		//}}

#ifdef	SERV_CHARM_ITEM_SCRIPT// 적용날짜: 2013-04-26
		const int iBonusItemID = SiCXSLRandomItemManager()->GetBonusCharmItem( pRandomItemData->m_ItemID );
		if( 0 < iBonusItemID )
		{
			const CXSLItem::ItemTemplet* pItemTempletBonus = SiCXSLItemManager()->GetItemTemplet( iBonusItemID );
			if( pItemTempletBonus == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
					<< BUILD_LOG( iBonusItemID )
					<< END_LOG;
			}
			else
			{
				KItemInfo kBonusItemInfo;
				kBonusItemInfo.m_iItemID	= iBonusItemID;
				kBonusItemInfo.m_cUsageType	= pItemTempletBonus->m_PeriodType;
				kBonusItemInfo.m_iQuantity	= 1;
				kBonusItemInfo.m_sEndurance	= pItemTempletBonus->m_Endurance;
				kBonusItemInfo.m_sPeriod	= 0;

				mapResultItem.insert( std::make_pair( iBonusItemID, kBonusItemInfo ) );
			}
		}
#endif	// SERV_CHARM_ITEM_SCRIPT
	}
	else
	{
		for( int i = 0; i < (int)mit->second.m_vecItemData.size(); ++i )
		{
			// 기간제
#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
			int iPeriod = mit->second.m_kLottery.GetDuplicateParam1( mit->second.m_vecItemData[i].m_iItemID, mit->second.m_vecItemData[i].m_iPeriod, mit->second.m_vecItemData[i].m_iQuantity );
#else //SERV_DUPLICATE_RANDOM_ITEM_GROUP
			int iPeriod = mit->second.m_kLottery.GetParam1( mit->second.m_vecItemData[i].m_iItemID );
#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
			if( iPeriod == KLottery::PARAM_BLANK )
			{
				START_LOG( cerr, L"해당 케이스의 파라미터가 존재하지 않음. 큐브랜덤을 돌렸는데 기간제 정보가 없네..?" )
					<< BUILD_LOG( iGroupID )
					<< BUILD_LOG( mit->second.m_vecItemData[i].m_iItemID )
					<< BUILD_LOG( iPeriod )
					<< END_LOG;

				goto end_proc;
			}
			
			// 수량
#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
			int iQuantity = mit->second.m_kLottery.GetDuplicateParam2( mit->second.m_vecItemData[i].m_iItemID, mit->second.m_vecItemData[i].m_iPeriod, mit->second.m_vecItemData[i].m_iQuantity );
#else //SERV_DUPLICATE_RANDOM_ITEM_GROUP
			int iQuantity = mit->second.m_kLottery.GetParam2( mit->second.m_vecItemData[i].m_iItemID );
#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
			if( iQuantity == KLottery::PARAM_BLANK )
			{
				START_LOG( cerr, L"해당 케이스의 파라미터가 존재하지 않음. 큐브랜덤을 돌렸는데 수량 정보가 없네..?" )
					<< BUILD_LOG( iGroupID )
					<< BUILD_LOG( mit->second.m_vecItemData[i].m_iItemID )
					<< BUILD_LOG( iPeriod )
					<< BUILD_LOG( iQuantity )
					<< END_LOG;

				goto end_proc;
			}
			
			//{{ 2008. 3. 17  최육사  DeleteAndInsert()
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->second.m_vecItemData[i].m_iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
					<< BUILD_LOG( mit->second.m_vecItemData[i].m_iItemID )
					<< END_LOG;
			}
			else
			{
				KItemInfo kInsertItemInfo;
				kInsertItemInfo.m_iItemID	 = mit->second.m_vecItemData[i].m_iItemID;
				kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
				kInsertItemInfo.m_iQuantity	 = iQuantity;
				kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;				
				kInsertItemInfo.m_sPeriod 	 = static_cast<short>(iPeriod); // 기간제 설정
				//{{ 2011. 06. 01	최육사	강화된 아이템 보상 큐브
#ifdef SERV_ENCHANTED_ITEM_CUBE_REWARD
				if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON  || pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
				{
					CXSLItem::UpdateEnchantedItemCubeReward( pRandomItemData->m_ItemID, kInsertItemInfo );
				}
#endif SERV_ENCHANTED_ITEM_CUBE_REWARD
				//}}

				mapResultItem.insert( std::make_pair( mit->second.m_vecItemData[i].m_iItemID, kInsertItemInfo ) );
			}
			//}}

#ifdef	SERV_CHARM_ITEM_SCRIPT// 적용날짜: 2013-04-26
			const int iBonusItemID = SiCXSLRandomItemManager()->GetBonusCharmItem( pRandomItemData->m_ItemID );
			if( 0 < iBonusItemID )
			{
				const CXSLItem::ItemTemplet* pItemTempletBonus = SiCXSLItemManager()->GetItemTemplet( iBonusItemID );
				if( pItemTempletBonus == NULL )
				{
					START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
						<< BUILD_LOG( iBonusItemID )
						<< END_LOG;
				}
				else
				{
					KItemInfo kBonusItemInfo;
					kBonusItemInfo.m_iItemID	= iBonusItemID;
					kBonusItemInfo.m_cUsageType	= pItemTempletBonus->m_PeriodType;
					kBonusItemInfo.m_iQuantity	= 1;
					kBonusItemInfo.m_sEndurance	= pItemTempletBonus->m_Endurance;
					kBonusItemInfo.m_sPeriod	= 0;

					mapResultItem.insert( std::make_pair( iBonusItemID, kBonusItemInfo ) );
				}
			}
#endif	// SERV_CHARM_ITEM_SCRIPT
		}
	}

	if( mapResultItem.size() <= 0 )
	{
		goto end_proc;
	}

	return true;

end_proc:
	SET_ERROR( ERR_RANDOM_ITEM_02 );

	START_LOG( cwarn, L"랜덤 아이템 구룹찾기 실패.!" )
		<< BUILD_LOG( pRandomItemData->m_ItemID )
		<< BUILD_LOGc( cUnitClass )
		<< END_LOG;

	return false;
}

//{{ 2013. 3. 16	박세훈	매력 아이템 스크립트 처리
#ifdef SERV_CHARM_ITEM_SCRIPT
void CXSLRandomItemManager::AddCharmItem_LUA( IN int iItemID, IN int iBonusItemID )
{
	CharmItemInfo kInfo;
	kInfo.m_iBonusItemID = iBonusItemID;
	std::pair< std::map<int, CharmItemInfo>::iterator, bool > pairResult = m_mapCharItemInfo.insert( std::map<int, CharmItemInfo>::value_type( iItemID, kInfo ) );

	if( pairResult.second == false )
	{
		START_LOG( cerr, L"동일한 ItemID의 매력 아이템이 이미 등록되어 있습니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
	}
}

bool CXSLRandomItemManager::IsCharmItem( IN const int iItemID ) const
{
	return m_mapCharItemInfo.find( iItemID ) != m_mapCharItemInfo.end();
}

int CXSLRandomItemManager::GetBonusCharmItem( IN const int iItemID ) const
{
	std::map<int, CharmItemInfo>::const_iterator it = m_mapCharItemInfo.find( iItemID );
	if( it == m_mapCharItemInfo.end() )
	{
		return 0;
	}

	return it->second.m_iBonusItemID;
}
#endif SERV_CHARM_ITEM_SCRIPT
//}}
