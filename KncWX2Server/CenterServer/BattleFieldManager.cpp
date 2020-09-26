#include ".\henirresulttable.h"
#include "BaseServer.h"
#include "X2Data/XSLItemManager.h"


ImplementRefreshSingleton( KHenirResultTable );

KHenirResultTable::KHenirResultTable(void)
{
}

KHenirResultTable::~KHenirResultTable(void)
{
}

ImplToStringW( KHenirResultTable )
{
	stm_	<< L"----------[ Henir Reward Table ]----------" << std::endl
			<< TOSTRINGW( m_mapHenirReward.size() )
			<< TOSTRINGW( m_mapRewardGroup.size() )			
			//{{ 2011. 08. 22	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
			<< TOSTRINGW( m_mapChallangeReward.size() )
#endif SERV_NEW_HENIR_TEST
			//}}
			<< TOSTRINGW( m_setResurrectionStage.size() )
            //{{ 2009. 12. 14  최육사	헤니르임시인벤
            << TOSTRINGW( m_setClearTempInventoryStage.size() )
            //}}
			<< TOSTRINGW( m_setClearNotifyStage.size() )
			;

	return stm_;
}

ImplementLuaScriptParser( KHenirResultTable )
{
	lua_tinker::class_add<KHenirResultTable>( GetLuaState(), "KHenirResultTable" );
	lua_tinker::class_def<KHenirResultTable>( GetLuaState(), "dump",						&KHenirResultTable::Dump );
	lua_tinker::class_def<KHenirResultTable>( GetLuaState(), "AddHenirResultItemInfo",		&KHenirResultTable::AddHenirResultItemInfo_LUA );
	lua_tinker::class_def<KHenirResultTable>( GetLuaState(), "AddHenirResultItemGroup",		&KHenirResultTable::AddHenirResultItemGroup_LUA );
	lua_tinker::class_def<KHenirResultTable>( GetLuaState(), "AddResurrectionStage",		&KHenirResultTable::AddResurrectionStage_LUA );
	//{{ 2009. 12. 14  최육사	헤니르임시인벤
	lua_tinker::class_def<KHenirResultTable>( GetLuaState(), "AddClearTempInventoryStage",	&KHenirResultTable::AddClearTempInventoryStage_LUA );
	//}}
	lua_tinker::class_def<KHenirResultTable>( GetLuaState(), "AddClearNotifyStage",			&KHenirResultTable::AddClearNotifyStage_LUA );
	//{{ 2011. 08. 22	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
	lua_tinker::class_def<KHenirResultTable>( GetLuaState(), "AddHenirChallangeRewardInfo",	&KHenirResultTable::AddHenirChallangeRewardInfo_LUA );
#endif SERV_NEW_HENIR_TEST
	//}}

	lua_tinker::decl( GetLuaState(), "HenirResultTable", this );
}

bool KHenirResultTable::AddHenirResultItemInfo_LUA( IN int iStageCount, IN char cHenirDungeonMode, IN int iItemGroupID, IN int iRandomCount )
{
	if( iStageCount <= 0  ||  cHenirDungeonMode < 0  ||  iItemGroupID <= 0  ||  iRandomCount <= 0 )
	{
		START_LOG( cerr, L"Henir 보상 스크립트 파싱 실패!" )
			<< BUILD_LOG( iStageCount )
			<< BUILD_LOGc( cHenirDungeonMode )
			<< BUILD_LOG( iItemGroupID )
			<< BUILD_LOG( iRandomCount )
			<< END_LOG;

		return false;
	}

	// key
	KHenirTableKey kKey( iStageCount, cHenirDungeonMode );

	// value
	KHenirReward kInfo;
	kInfo.m_iGroupID = iItemGroupID;
	kInfo.m_iRandomCount = iRandomCount;

	std::map< KHenirTableKey, std::vector< KHenirReward > >::iterator mit;
	mit = m_mapHenirReward.find( kKey );
	if( mit == m_mapHenirReward.end() )
	{
		std::vector< KHenirReward > vecRewardInfo;
		vecRewardInfo.push_back( kInfo );
		
		m_mapHenirReward.insert( std::make_pair( kKey, vecRewardInfo ) );
	}
	else
	{
		mit->second.push_back( kInfo );
	}

	return true;
}

bool KHenirResultTable::AddHenirResultItemGroup_LUA( IN int iItemGroupID, IN int iItemID, IN int iQuantity, IN float fRate )
{
	if( iItemGroupID <= 0  ||  iItemID < 0  ||  iQuantity <= 0  ||  fRate <= 0.0f )
	{
		START_LOG( cerr, L"Henir 보상 스크립트 파싱 실패!" )
			<< BUILD_LOG( iItemGroupID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iQuantity )
			<< BUILD_LOG( fRate )
			<< END_LOG;

		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 itemid가 보상으로 설정되었습니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	std::map< int, KLottery >::iterator mit = m_mapRewardGroup.find( iItemGroupID );
	if( mit == m_mapRewardGroup.end() )
	{
		KLottery kLot;
		kLot.AddCase( iItemID, fRate, iQuantity );
		m_mapRewardGroup.insert( std::make_pair( iItemGroupID, kLot ) );
	}
	else
	{
		if( !mit->second.AddCase( iItemID, fRate, iQuantity ) )
		{
			START_LOG( cerr, L"확률 설정이 이상합니다." )
				<< BUILD_LOG( iItemGroupID )
				<< END_LOG;

			return false;
		}
	}

	return true;
}

bool KHenirResultTable::AddResurrectionStage_LUA( IN int iStageCount )
{
    if( iStageCount < 0 )
	{
		START_LOG( cerr, L"스테이지 정보가 이상합니다" )
			<< BUILD_LOG( iStageCount )
			<< END_LOG;
		return false;
	}

	m_setResurrectionStage.insert( iStageCount );
	return true;
}

//{{ 2009. 12. 14  최육사	헤니르임시인벤
bool KHenirResultTable::AddClearTempInventoryStage_LUA( IN int iStageID )
{
	if( iStageID < 0 )
	{
		START_LOG( cerr, L"스테이지 정보가 이상합니다" )
			<< BUILD_LOG( iStageID )
			<< END_LOG;
		return false;
	}

	m_setClearTempInventoryStage.insert( iStageID );
	return true;
}
//}}

bool KHenirResultTable::AddClearNotifyStage_LUA( IN int iStageCount )
{
	if( iStageCount < 0 )
	{
		START_LOG( cerr, L"스테이지 정보가 이상합니다" )
			<< BUILD_LOG( iStageCount )
			<< END_LOG;
		return false;
	}

	m_setClearNotifyStage.insert( iStageCount );
	return true;
}

//{{ 2011. 08. 22	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
bool KHenirResultTable::AddHenirChallangeRewardInfo_LUA( IN int iStageID, IN int iItemID, IN int iQuantity )
{
	if( iStageID < 0  ||  iItemID < 0  ||  iQuantity <= 0 )
	{
		START_LOG( cerr, L"Henir 도전 보상 스크립트 파싱 실패!" )
			<< BUILD_LOG( iStageID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;
		return false;
	}

	std::map< int, SHenirChallangeReward >::iterator mit;
	mit = m_mapChallangeReward.find( iStageID );
	if( mit == m_mapChallangeReward.end() )
	{
		SHenirChallangeReward kInfo;
		kInfo.m_mapReward.insert( std::make_pair( iItemID, iQuantity ) );
		m_mapChallangeReward.insert( std::make_pair( iStageID, kInfo ) );
	}
	else
	{
		if( mit->second.m_mapReward.find( iItemID ) != mit->second.m_mapReward.end() )
		{
			START_LOG( cerr, L"이미 등록된 Henir 도전 보상 정보입니다!" )
				<< BUILD_LOG( iStageID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;
			return false;
		}

        mit->second.m_mapReward.insert( std::make_pair( iItemID, iQuantity ) );
	}

	return true;
}
#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2011. 08. 23	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
bool KHenirResultTable::GetHenirRewardItem( IN const int iStageID, IN const char cDungeonMode, IN OUT std::map< int, KItemInfo >& mapRewardItem )
{
	// key
	KHenirTableKey kKey( iStageID, cDungeonMode );

	// 해당 모드와 스테이지의 보상 정보 얻기
	std::map< KHenirTableKey, std::vector< KHenirReward > >::const_iterator mit;
	mit = m_mapHenirReward.find( kKey );
	if( mit == m_mapHenirReward.end() )
	{
		START_LOG( cerr, L"해당 key값의 보상정보가 없다? 세팅이 잘못 되었나?" )
			<< BUILD_LOG( iStageID )
			<< BUILD_LOGc( cDungeonMode )
			<< END_LOG;

		return false;
	}

	// 보상 얻기!
	std::vector< KHenirReward >::const_iterator vit;
	for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
	{
		// 보상 랜덤 그룹
		std::map< int, KLottery >::const_iterator mit = m_mapRewardGroup.find( vit->m_iGroupID );
		if( mit == m_mapRewardGroup.end() )
		{
			START_LOG( cerr, L"존재하지 않는 ItemGroupID가 보상으로 등록되어있음!" )
				<< BUILD_LOG( iStageID )
				<< BUILD_LOGc( cDungeonMode )
				<< BUILD_LOG( vit->m_iGroupID )
				<< END_LOG;

			continue;
		}

		// 보상 랜덤을 돌려보자!
		for( int i = 0; i < vit->m_iRandomCount; ++i )
		{
			int iRewardItemID = mit->second.Decision();
			if( iRewardItemID == KLottery::CASE_BLANK )
			{
				START_LOG( cerr, L"확률이 100%가 아니네?!" )
					<< BUILD_LOG( iStageID )
					<< BUILD_LOGc( cDungeonMode )
					<< BUILD_LOG( vit->m_iGroupID )
					<< END_LOG;

				continue;
			}

			int iQuantity = mit->second.GetParam1( iRewardItemID );
			if( iQuantity == KLottery::PARAM_BLANK )
			{
				START_LOG( cerr, L"수량값이 저장 안되어있네?" )
					<< BUILD_LOG( iStageID )
					<< BUILD_LOGc( cDungeonMode )
					<< BUILD_LOG( vit->m_iGroupID )
					<< BUILD_LOG( iRewardItemID )
					<< END_LOG;

				continue;
			}

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iRewardItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"존재하지 않는 itemid인데 헤니르 보상으로 등록되어있네?" )
					<< BUILD_LOG( iStageID )
					<< BUILD_LOGc( cDungeonMode )
					<< BUILD_LOG( vit->m_iGroupID )
					<< BUILD_LOG( iRewardItemID )
					<< END_LOG;

				continue;
			}

			// 보상 저장
			std::map< int, KItemInfo >::iterator mitRI = mapRewardItem.find( iRewardItemID );
			if( mitRI == mapRewardItem.end() )
			{
				KItemInfo kInfo;
				kInfo.m_iItemID		  = iRewardItemID;
				kInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
				kInfo.m_iQuantity	  = iQuantity;
				kInfo.m_sEndurance	  = pItemTemplet->m_Endurance;
				mapRewardItem.insert( std::make_pair( iRewardItemID, kInfo ) );
			}
			else
			{
				mitRI->second.m_iQuantity += iQuantity;
			}			
		}
	}

	return true;
}
#else
bool KHenirResultTable::GetHenirRewardItem( IN int iStageCount, IN char cDungeonMode, OUT std::map< int, KItemInfo >& mapRewardItem )
{
	mapRewardItem.clear();

	// key
	KHenirTableKey kKey( iStageCount, cDungeonMode );

	// 해당 모드와 스테이지의 보상 정보 얻기
	std::map< KHenirTableKey, std::vector< KHenirReward > >::const_iterator mit;
	mit = m_mapHenirReward.find( kKey );
	if( mit == m_mapHenirReward.end() )
	{
		START_LOG( cerr, L"해당 key값의 보상정보가 없다? 세팅이 잘못 되었나?" )
			<< BUILD_LOG( iStageCount )
			<< BUILD_LOGc( cDungeonMode )
			<< END_LOG;

		return false;
	}

	// 보상 얻기!
	std::vector< KHenirReward >::const_iterator vit;
	for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
	{
		// 보상 랜덤 그룹
		std::map< int, KLottery >::const_iterator mit = m_mapRewardGroup.find( vit->m_iGroupID );
		if( mit == m_mapRewardGroup.end() )
		{
			START_LOG( cerr, L"존재하지 않는 ItemGroupID가 보상으로 등록되어있음!" )
				<< BUILD_LOG( iStageCount )
				<< BUILD_LOGc( cDungeonMode )
				<< BUILD_LOG( vit->m_iGroupID )
				<< END_LOG;

			continue;
		}

		// 보상 랜덤을 돌려보자!
		for( int i = 0; i < vit->m_iRandomCount; ++i )
		{
			int iRewardItemID = mit->second.Decision();
			if( iRewardItemID == KLottery::CASE_BLANK )
			{
				START_LOG( cerr, L"확률이 100%가 아니네?!" )
					<< BUILD_LOG( iStageCount )
					<< BUILD_LOGc( cDungeonMode )
					<< BUILD_LOG( vit->m_iGroupID )
					<< END_LOG;

				continue;
			}

			int iQuantity = mit->second.GetParam1( iRewardItemID );
			if( iQuantity == KLottery::PARAM_BLANK )
			{
				START_LOG( cerr, L"수량값이 저장 안되어있네?" )
					<< BUILD_LOG( iStageCount )
					<< BUILD_LOGc( cDungeonMode )
					<< BUILD_LOG( vit->m_iGroupID )
					<< BUILD_LOG( iRewardItemID )
					<< END_LOG;

				continue;
			}

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iRewardItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"존재하지 않는 itemid인데 헤니르 보상으로 등록되어있네?" )
					<< BUILD_LOG( iStageCount )
					<< BUILD_LOGc( cDungeonMode )
					<< BUILD_LOG( vit->m_iGroupID )
					<< BUILD_LOG( iRewardItemID )
					<< END_LOG;

				continue;
			}

			// 보상 저장
			std::map< int, KItemInfo >::iterator mitRI = mapRewardItem.find( iRewardItemID );
			if( mitRI == mapRewardItem.end() )
			{
				KItemInfo kInfo;
				kInfo.m_iItemID		  = iRewardItemID;
				kInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
				kInfo.m_iQuantity	  = iQuantity;
				kInfo.m_sEndurance	  = pItemTemplet->m_Endurance;
				mapRewardItem.insert( std::make_pair( iRewardItemID, kInfo ) );
			}
			else
			{
				mitRI->second.m_iQuantity += iQuantity;
			}			
		}
	}

	return true;
}
#endif SERV_NEW_HENIR_TEST
//}}


//{{ 2011. 08. 22	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
bool KHenirResultTable::GetHenirChallangeRewardItem( IN const int iStageID, IN OUT std::map< int, KItemInfo >& mapRewardItem )
{
	std::map< int, SHenirChallangeReward >::const_iterator mit;
	mit = m_mapChallangeReward.find( iStageID );
	if( mit == m_mapChallangeReward.end() )
	{
		START_LOG( cerr, L"해당 스테이지에 대한 도전 보상 정보가 없다." )
			<< BUILD_LOG( iStageID )
			<< END_LOG;
		return false;
	}

	std::map< int, int >::const_iterator mitCR;
	for( mitCR = mit->second.m_mapReward.begin(); mitCR != mit->second.m_mapReward.end(); ++mitCR )
	{
		const int iRewardItemID = mitCR->first;
		const int iRewardQuantity = mitCR->second;
		
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iRewardItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 itemid인데 헤니르 보상으로 등록되어있네?" )
				<< BUILD_LOG( iStageID )
				<< BUILD_LOGc( iRewardItemID )
				<< BUILD_LOG( iRewardItemID )
				<< END_LOG;
			continue;
		}

		// 보상 저장
		std::map< int, KItemInfo >::iterator mitRI = mapRewardItem.find( iRewardItemID );
		if( mitRI == mapRewardItem.end() )
		{
			KItemInfo kInfo;
			kInfo.m_iItemID		  = iRewardItemID;
			kInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
			kInfo.m_iQuantity	  = iRewardQuantity;
			kInfo.m_sEndurance	  = pItemTemplet->m_Endurance;
			mapRewardItem.insert( std::make_pair( iRewardItemID, kInfo ) );
		}
		else
		{
			mitRI->second.m_iQuantity += iRewardQuantity;
		}
	}

	return true;    
}
#endif SERV_NEW_HENIR_TEST
//}}



