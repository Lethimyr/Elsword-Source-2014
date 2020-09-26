#include ".\xslSynthesisManager.h"
#include "NetError.h"
#include "XSLItemManager.h"
#include "XSLUnit.h"

//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
#include "X2Data/XSLSocketItem.h"
#endif SERV_CASH_ITEM_SOCKET_OPTION
//}


ImplementRefreshSingleton( CXSLSynthesisManager );

CXSLSynthesisManager::CXSLSynthesisManager(void)
{
}

CXSLSynthesisManager::~CXSLSynthesisManager(void)
{
}

ImplToStringW( CXSLSynthesisManager )
{
	stm_	<< L"----------[ Synthesis Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapRandomItem.size() )
			<< TOSTRINGW( m_mapItemGroup.size() )
			;

	return stm_;
}

ImplementLuaScriptParser( CXSLSynthesisManager )
{
	lua_tinker::class_add<CXSLSynthesisManager>( GetLuaState(), "CXSLSynthesisManager" );
	lua_tinker::class_def<CXSLSynthesisManager>( GetLuaState(), "AddAvatarSocketOptionForEquipPosition",		&CXSLSynthesisManager::AddAvatarSocketOptionForEquipPosition_LUA );
	lua_tinker::class_def<CXSLSynthesisManager>( GetLuaState(), "AddSynthesisGroup",		&CXSLSynthesisManager::AddSynthesisGroup_LUA );

	lua_tinker::decl( GetLuaState(), "g_pSynthesisManager", this );
}

bool CXSLSynthesisManager::AddAvatarSocketOptionForEquipPosition_LUA( int iEquipPosition, int iSocketGroupID )
{
	if (iEquipPosition == CXSLUnit::EQIP_POSITION::EP_WEAPON_HAND ||
		iEquipPosition == CXSLUnit::EQIP_POSITION::EP_DEFENCE_HAIR ||
		iEquipPosition == CXSLUnit::EQIP_POSITION::EP_DEFENCE_FACE ||
		iEquipPosition == CXSLUnit::EQIP_POSITION::EP_DEFENCE_BODY ||
		iEquipPosition == CXSLUnit::EQIP_POSITION::EP_DEFENCE_LEG ||
		iEquipPosition == CXSLUnit::EQIP_POSITION::EP_DEFENCE_HAND ||
		iEquipPosition == CXSLUnit::EQIP_POSITION::EP_DEFENCE_FOOT ||
		iEquipPosition == CXSLUnit::EQIP_POSITION::EP_WEAPON_HAND )
	{
		std::map< int, int >::iterator find_it = m_mapAvatarSocketOptionGroupID.find(iEquipPosition);
		if (find_it == m_mapAvatarSocketOptionGroupID.end())
		{
			m_mapAvatarSocketOptionGroupID.insert(std::make_pair(iEquipPosition, iSocketGroupID));
			return true;
		}
		else
		{
			START_LOG( cerr, L"엥? 왜 같은 부위 옵션을 두번 지정했지???" )
				<< BUILD_LOG( iEquipPosition )
				<< BUILD_LOG( iSocketGroupID )
				<< END_LOG;
			return false;
		}
	}
	else
	{
		START_LOG( cerr, L"지원하지 않는 EquipPosition입니다!" )
			<< BUILD_LOG( iEquipPosition )
			<< BUILD_LOG( iSocketGroupID )
			<< END_LOG;
	}

	return false;
}


bool CXSLSynthesisManager::AddSynthesisGroup_LUA( int iGroupID, int iItemID , float fRate)
{
	if( iGroupID <= 0 || iItemID <= 0  )
	{
		START_LOG( cerr, L"랜덤 아이템 그룹데이터 파싱 실패.!" )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iItemID )
//			<< BUILD_LOG( fRate )
			<< END_LOG;

		return false;
	}

	ItemGroupData itemData;
	itemData.m_iItemID		= iItemID;
	itemData.m_fRate		= fRate;

	std::map< int, ItemGroupDataList >::iterator mit;
	mit = m_mapItemGroup.find( iGroupID );

	if( mit != m_mapItemGroup.end() )
	{
		mit->second.m_vecItemData.push_back( itemData );
		if( mit->second.m_kLottery.AddCaseIntegerCast( iItemID, fRate ) == false )
		{
			START_LOG( cerr, L"랜덤 아이템 확률 설정 실패!" )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( fRate )
				<< END_LOG;
		}
	}
	else
	{
		ItemGroupDataList itemGDList;
		itemGDList.m_iGroupID	= iGroupID;
		itemGDList.m_vecItemData.push_back( itemData );
		if( itemGDList.m_kLottery.AddCaseIntegerCast( iItemID, fRate ) == false )
		{
			START_LOG( cerr, L"랜덤 아이템 확률 설정 실패!" )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( fRate )
				<< END_LOG;
		}

		m_mapItemGroup.insert( std::make_pair( iGroupID, itemGDList ) );
	}

	return true;
}

int CXSLSynthesisManager::GetAvatarSocketOptionGroupID(int iEquipPosition)
{
	int iTmpSocketGroupID = 0;
	std::map< int, int >::iterator find_it = m_mapAvatarSocketOptionGroupID.find(iEquipPosition);
	if (find_it != m_mapAvatarSocketOptionGroupID.end())
	{
		iTmpSocketGroupID = find_it->second;
	}

	return iTmpSocketGroupID;
}

const CXSLSynthesisManager::RandomItemData* CXSLSynthesisManager::GetSynthesisItemData( IN int iRandomItemID, IN int iKeyItemID ) const
{
	std::map< RandomItemKey, RandomItemData >::const_iterator mit;
	mit = m_mapRandomItem.find( RandomItemKey( iRandomItemID, iKeyItemID ) );
	if( mit == m_mapRandomItem.end() )
		return NULL;
	
	return &mit->second;
}

bool CXSLSynthesisManager::GetSynthesisResultItem( IN int iUnitType, IN int iEqipPosition , IN int iSocketNo , OUT std::map< int, KItemInfo >& mapResultItem , OUT std::set< int > &setSealCashItem ) const
{
	SET_ERROR( NET_OK );

	mapResultItem.clear();

	int iGroupID = 0;
	std::map< int, ItemGroupDataList >::const_iterator mit;

		switch(iEqipPosition)
		{
			case CXSLUnit::EP_WEAPON_HAND:
				iEqipPosition -= 2;
				break;
			case CXSLUnit::EP_DEFENCE_BODY:
				iEqipPosition -= 7;
				break;
			case CXSLUnit::EP_DEFENCE_LEG:
				iEqipPosition -= 7;
				break;
			case CXSLUnit::EP_DEFENCE_HAND:
				iEqipPosition -= 7;
				break;
			case CXSLUnit::EP_DEFENCE_FOOT:
				iEqipPosition -= 7;
				break;
			case CXSLUnit::EP_DEFENCE_HAIR:
				iEqipPosition -= 1;
				break;
		}			

	iGroupID = (iUnitType*10) + iEqipPosition;

	if( iGroupID == 0 )
	{
		goto end_proc;
	}

	mit = m_mapItemGroup.find( iGroupID );

	if( mit == m_mapItemGroup.end() )
	{
		goto end_proc;
	}

		int iItemID = 0;
		iItemID = mit->second.m_kLottery.Decision();

		if( iItemID == KLottery::CASE_BLANK )
		{
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
			kInsertItemInfo.m_iQuantity	 = 1;
			kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;			
//			kInsertItemInfo.m_sPeriod	 = static_cast<short>(iPeriod); // 기간제 설정

			//{{ 2009. 8. 27  최육사	밀봉
			if( SiCXSLItemManager()->IsCanSealForCashItem( iItemID ) == true )
			{
				setSealCashItem.insert( pItemTemplet->m_ItemID );
			}
			//}}

			kInsertItemInfo.m_vecItemSocket.push_back( iSocketNo );


			mapResultItem.insert( std::make_pair( iItemID, kInsertItemInfo ) );
		}
		//}}

		//{{ 2008. 5. 27  최육사  랜덤 결과 인벤 공간 체크
//		std::map< int, KLottery::KCaseUnit >::const_iterator mitCase;
//		for( mitCase = mit->second.m_kLottery.m_mapCase.begin(); mitCase != mit->second.m_kLottery.m_mapCase.end(); ++mitCase )
//		{
//			mapCheckEmpty.insert( std::make_pair( mitCase->first, mitCase->second.m_nParam2 ) );
//		}
//		//}}

	if( mapResultItem.size() <= 0 )
	{
		goto end_proc;
	}

	return true;

end_proc:
	SET_ERROR( ERR_RANDOM_ITEM_02 );

	START_LOG( cwarn, L"랜덤 아이템 구룹찾기 실패.!" )
		<< END_LOG;

	return false;
}