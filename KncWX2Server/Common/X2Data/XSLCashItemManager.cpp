#include ".\xslcashitemmanager.h"
#include "BaseServer.h"
#include "../Common/KAutoPath.h"
#include "Enum/Enum.h"

ImplementSingleton( CXSLCashItemManager );


CXSLCashItemManager::CXSLCashItemManager(void) : 
m_bIsUpdate( false ),
m_ulProductNoResStone( 0 )
{
	//lua_tinker::class_<CXSLCashItemManager>("CXSLCashItemManager")
	//	.def("refresh",						CXSLCashItemManager::Refresh_LUA)
	//	.def("AddPackageItemData",			CXSLCashItemManager::AddPackageItemData_LUA)
	//	.def("dump",						CXSLCashItemManager::Dump)
	//	;

	//lua_tinker::decl( "g_pCashItemManager", this );
	lua_tinker::class_add<CXSLCashItemManager>( g_pLua, "CXSLCashItemManager" );
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddPackageItemData",			&CXSLCashItemManager::AddPackageItemData_LUA );
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemPrice",				&CXSLCashItemManager::AddCashItemPrice_LUA );

	//{{ 2011.2.27 지헌 : 캐쉬 아바타 분해시 추가 아이템 나오도록 하기
#ifdef SERV_CASHITEM_RESOLVE_ADD
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemGroup",		&CXSLCashItemManager::AddCashItemGroup_LUA );
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemResolveAdd",		&CXSLCashItemManager::AddCashItemResolveAdd_LUA );
#endif
	//}}

	//{{ 2009. 12. 19  최육사	서버군 확장
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemServerGroupData",	&CXSLCashItemManager::AddCashItemServerGroupData_LUA );	
	//}}

	//{{ 2011. 01. 04  김민성	product no 별 서버군 확장
#ifdef SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemProductNoData",		&CXSLCashItemManager::AddCashItemProductNoData_LUA );	
#endif SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	//}}

	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "dump",							&CXSLCashItemManager::Dump );

	lua_tinker::decl( g_pLua, "g_pCashItemManager", this );
}

CXSLCashItemManager::~CXSLCashItemManager(void)
{
	m_mapPackageData.clear();
}

ImplToStringW( CXSLCashItemManager )
{
	stm_	<< L"----------[ Cash Item Manager ]----------" << std::endl
			<< L"Package Data Num : "  << m_mapPackageData.size() << std::endl
		;

	return stm_;
}

bool CXSLCashItemManager::OpenScriptFile( const char* pFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );
	return true;
}

bool CXSLCashItemManager::AddPackageItemData_LUA( int iPackageItemID, int iItemID, unsigned short usProductPieces, bool bShowItem )
{
	if( iPackageItemID <= 0 || iItemID <= 0 )
	{
		START_LOG( cerr, L"캐쉬 패키지 아이템 정보 읽기 실패!" )
			<< BUILD_LOG( iPackageItemID )
			<< BUILD_LOG( usProductPieces )
			<< END_LOG;
		return false;
	}

	// 패키지 정보
	PackageItemData kPackageItemData;
	kPackageItemData.m_iItemID		   = iItemID;
	kPackageItemData.m_usProductPieces = usProductPieces;
	//kPackageItemData.m_bShowItem	   = bShowItem; 서버에서는 쓰이지 않는다.

	std::map< int, std::vector< PackageItemData > >::iterator mit;
	mit = m_mapPackageData.find( iPackageItemID );
	if( mit == m_mapPackageData.end() )
	{
		std::vector< PackageItemData > vecPackageData;		
		vecPackageData.push_back( kPackageItemData );
		m_mapPackageData.insert( std::make_pair( iPackageItemID, vecPackageData ) );
	}
	else
	{
		// 중복 ProductNo 검사
		std::vector< PackageItemData >::const_iterator vit;
		for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
		{
			if( vit->m_iItemID == iItemID )
			{
				START_LOG( cerr, L"하나의 패키지에 중복되는 구성상품 ItemID가 존재합니다." )
					<< BUILD_LOG( iPackageItemID )
					<< BUILD_LOG( iItemID )
					<< END_LOG;
				return false;
			}
		}

		mit->second.push_back( kPackageItemData );
	}

	return true;
}

bool CXSLCashItemManager::AddCashItemPrice_LUA( int iItemID, int iPrice )
{
	if( iItemID <= 0  ||  iPrice <= 0 )
	{
		START_LOG( cerr, L"넥슨 빌링 상품 가격 정보가 이상합니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iPrice )
			<< END_LOG;
		return false;
	}

	m_mapCashItemPriceFromScript.insert( std::make_pair( iItemID, iPrice ) );
	return true;
}


//{{ 2011.2.27 지헌 : 캐쉬 아바타 분해시 추가 아이템 나오도록 하기
#ifdef SERV_CASHITEM_RESOLVE_ADD
bool CXSLCashItemManager::AddCashItemGroup_LUA( int iGroupID, int iItemID)
{
	std::map<int, int>::iterator mit; 
	mit = m_mapCashItemGroup.find( iItemID );

	if( mit != m_mapCashItemGroup.end() )  // 기존에 등록된 동일 아이템이 있으면? 오류
	{
		START_LOG( cerr, L"이미 등록 된 아이템 입니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	m_mapCashItemGroup.insert( std::make_pair( iItemID, iGroupID ) );

	return true;
}

bool CXSLCashItemManager::AddCashItemResolveAdd_LUA( int iGroupID, int iItemID, int iQuantity, float fProbability )
{
	// 그룹 오류 체크 넣자
	// 그룹을 second로 넣었더니 다 돌리네.. 그냥 찾는 함수 없나?
	std::map<int, int>::iterator mit; 
	for(mit = m_mapCashItemGroup.begin(); mit != m_mapCashItemGroup.end(); mit++)
	{	
		if( mit == m_mapCashItemGroup.end() )
		{
			START_LOG( cerr, L"캐쉬 아이템 그룹 정보가 이상합니다." )
				<< BUILD_LOG( iGroupID )
				<< END_LOG;
			return false;
		}
	}

	if( iItemID <= 0 )
	{
		START_LOG( cerr, L"넥슨 빌링 상품 가격 정보가 이상합니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}


	std::map<int,VecResolveInfo>::iterator mit2;
	mit2 = m_mapCashItemResolveGroup.find( iGroupID );

	if( mit2 == m_mapCashItemResolveGroup.end() )  // 기존에 등록된 동일 그룹 데이터가 없음 - 새로 추가.
	{
		VecResolveInfo vecInfo;
		vecInfo.clear();

		m_mapCashItemResolveGroup.insert( std::make_pair( iGroupID, vecInfo ) );
		mit2 = m_mapCashItemResolveGroup.find( iGroupID );
	}

	ResolveAddInfo info;
	info.iItemID = iItemID;
	info.iQuantity = iQuantity;
	info.kLottery.AddCase(RESOLVE_SUCCESS, fProbability);

	mit2->second.push_back(info);

	return true;
}

bool CXSLCashItemManager::GetCashItemResolveAddFromScript( int iItemID, VecResolveInfo& vecItem )
{
	vecItem.clear();

	// 우선 그룹을 찾자
	std::map<int, int>::iterator mit;
	mit = m_mapCashItemGroup.find(iItemID);

	if(mit == m_mapCashItemGroup.end())
	{
		// 여기 등록 안되는 아이템들도 많으므로 에러 로그는 남기지 않는다.
		/*
		START_LOG( cerr, L"그룹 등록이 안된 아이템 입니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		*/
		return false;
	}

	int group = mit->second;

	std::map< int, VecResolveInfo >::const_iterator mit2;
	mit2 = m_mapCashItemResolveGroup.find( group );
	if( mit2 == m_mapCashItemResolveGroup.end() )
	{
		START_LOG( cerr, L"그룹에 등록된 결과 아이템이 없습니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	// 모든 오류를 통과했으면, 해당 그룹의 결과 아이템들에 대해 모두 확률계산을 해서
	// 참이 나오는 것들을 vecItem 에 담아서 넘겨주자.
	VecResolveInfo vecInfo = mit2->second;
	VecResolveInfo::iterator vit;
	for(vit = vecInfo.begin(); vit != vecInfo.end(); vit++)
	{
		ResolveAddInfo info = (*vit);
		if(info.kLottery.Decision() == RESOLVE_SUCCESS)
		{
			// 같은 종류가 있다면 갯수를 더해준다.... 그냥 하면 어떻게 되는지 일단 보자.
			VecResolveInfo::iterator vit2;
			for(vit2 = vecItem.begin(); vit2 != vecItem.end(); vit2++)
			{
				if( info.iItemID == vit2->iItemID )
				{
					vit2->iQuantity += info.iQuantity;
					break;
				}
			}
			if(vit2 == vecItem.end() )
				vecItem.push_back(info);
		}
	}

	return true;
}
#endif SERV_CASHITEM_RESOLVE_ADD
//}}


//{{ 2009. 12. 19  최육사	서버군 확장
bool CXSLCashItemManager::AddCashItemServerGroupData_LUA( int iServerGroupID, int iItemID )
{
	if( !SEnum::IsValidServerGroupID( iServerGroupID )  ||  iItemID <= 0 )
	{
		START_LOG( cerr, L"서버군별 캐쉬샵 안보이는 상품 정보값이 이상합니다." )
			<< BUILD_LOG( iServerGroupID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	if( KBaseServer::GetKObj()->GetServerGroupID() != iServerGroupID )
		return true;

	// 판매되어서는 안되는 아이템 리스트
	m_setNotSellCashItem.insert( iItemID );
	return true;
}
//}}

//{{ 2011. 01. 04  김민성	product no 별 서버군 확장
#ifdef SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
bool CXSLCashItemManager::AddCashItemProductNoData_LUA( int iServerGroupID, unsigned long ulProductNo )
{
	if( !SEnum::IsValidServerGroupID( iServerGroupID )  ||  ulProductNo <= 0 )
	{
		START_LOG( cerr, L"ProductNo별 캐쉬샵 안보이는 상품 정보값이 이상합니다." )
			<< BUILD_LOG( iServerGroupID )
			<< BUILD_LOG( ulProductNo )
			<< END_LOG;

		return false;
	}
	// CashItemData.lua 파일 참고 - 해당 서버에 캐쉬샵 표기되지 않도록
	// m_setCashItemProductNoData 에 있는 item은 캐쉬샵에 나오면 안된다.
	if( !KBaseServer::GetKObj()->GetServerGroupID() != iServerGroupID )
		return true;

	m_setNotSellCashItemProductNo.insert( ulProductNo );
	return true;
}
#endif SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
//}}

bool CXSLCashItemManager::GetPackageData( int iPackageItemID, std::vector< PackageItemData >& vecPackageItem ) const
{
	vecPackageItem.clear();

	std::map< int, std::vector< PackageItemData > >::const_iterator mit;
	mit = m_mapPackageData.find( iPackageItemID );
	if( mit == m_mapPackageData.end() )
		return false;

	vecPackageItem = mit->second;
	return true;
}

//{{ 2008. 5. 15  최육사  판매중인 상품인지 검사 ItemID
void CXSLCashItemManager::UpdateProductList( std::vector< int >& vecProductItemID, std::vector< KCashItemResolveInfo >& vecCashResolveInfo, unsigned long ulProductNoResStone )
{
	m_setProductItemID.clear();
	m_mapCashItemPrice.clear();

	std::vector< int >::const_iterator vit;
	for( vit = vecProductItemID.begin(); vit != vecProductItemID.end(); ++vit )
	{
		m_setProductItemID.insert( *vit );
	}

	if( !m_setProductItemID.empty() )
		m_bIsUpdate = true;

	//{{ 2008. 6. 10  최육사  실시간 부활석
	m_ulProductNoResStone = ulProductNoResStone;
	//}}

	//std::map< int, int >::const_iterator mit;
	//std::vector< KCashItemResolveInfo >::const_iterator vitCR;
	//for( vitCR = vecCashResolveInfo.begin(); vitCR != vecCashResolveInfo.end(); ++vitCR )
	//{
	//	mit = m_mapCashItemPrice.find( vitCR->m_iItemID );
	//	if( mit != m_mapCashItemPrice.end() )
	//	{
	//		START_LOG( cerr, L"어라? 캐쉬 분해 정보를 업데이트 하던중에 중복되는 ItemID가 있네?" )
	//			<< BUILD_LOG( vitCR->m_iItemID )
	//			<< BUILD_LOG( vitCR->m_iSalePrice )
	//			<< BUILD_LOG( mit->first )
	//			<< BUILD_LOG( mit->second )
	//			<< END_LOG;
	//	}

	//	m_mapCashItemPrice.insert( std::make_pair( vitCR->m_iItemID, vitCR->m_iSalePrice ) );
	//}
}

bool CXSLCashItemManager::IsBuyAvailable( int iItemID )
{
	if( !m_bIsUpdate )
		return true;

	std::set< int >::const_iterator sit;
	sit = m_setProductItemID.find( iItemID );
	if( sit == m_setProductItemID.end() )
		return false;

	return true;
}
//}}

//{{ 2008. 9. 23  최육사	캐쉬분해
//bool CXSLCashItemManager::GetCashItemPrice( int iItemID, int& iPrice )
//{
//	std::map< int, int >::const_iterator mit;
//	mit = m_mapCashItemPrice.find( iItemID );
//	if( mit == m_mapCashItemPrice.end() )
//		return false;
//
//	iPrice = mit->second;
//	return true;
//}

bool CXSLCashItemManager::GetCashItemPriceFromScript( int iItemID, int& iPrice )
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapCashItemPriceFromScript.find( iItemID );
	if( mit == m_mapCashItemPriceFromScript.end() )
		return false;

	iPrice = mit->second;
	return true;
}
//}}


