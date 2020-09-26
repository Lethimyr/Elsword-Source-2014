#include ".\billingscriptmanager.h"
#include "KAutoPath.h"


//{{ 2011. 12. 07	최육사	빌링 스크립트 매니저
//#ifdef SERV_BILLING_SCRIPT_MANAGER


KBillingScriptManager::KBillingScriptManager(void)
{
}

KBillingScriptManager::~KBillingScriptManager(void)
{
}

ImplToStringW( KBillingScriptManager )
{
	stm_	<< L"----------[ Billing Script Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapProductCategoryModifyForItemID.size() )
		;

	return stm_;
}

bool KBillingScriptManager::OpenScriptFile( IN const char* pszFileName )
{
	KLuaManager luaManager;

	KAutoPath		kautoPaht;
	std::string		strFile;

	strFile = "Enum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< BUILD_LOG( pszFileName )
			<< END_LOG;
		return false;
	}

	strFile = "DungeonEnum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< BUILD_LOG( pszFileName )
			<< END_LOG;
		return false;
	}
	 
	strFile = "DLG_Map_Enum.lua";
	kautoPaht.GetPullPath( strFile );

	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< BUILD_LOG( pszFileName )
			<< END_LOG;
		return false;
	}

	lua_tinker::class_add<KBillingScriptManager>( luaManager.GetLuaState(), "KBillingScriptManager" );
	lua_tinker::class_def<KBillingScriptManager>( luaManager.GetLuaState(), "AddProductCategoryModifyForItemID",	&KBillingScriptManager::AddProductCategoryModifyForItemID_LUA );
	lua_tinker::class_def<KBillingScriptManager>( luaManager.GetLuaState(), "dump",									&KBillingScriptManager::Dump );
	lua_tinker::decl( luaManager.GetLuaState(), "g_pBillingManager", this );

	strFile = pszFileName;
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		std::wstring wstrErr = KncUtil::toWideString( lua_tostring( luaManager.GetLuaState(), -1 ) );
		START_LOG( cerr, pszFileName )
			<< BUILD_LOG( wstrErr )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KBillingScriptManager::AddProductCategoryModifyForItemID_LUA( int iItemID, int iServiceCategoryNo, int iTestServCategoryNo )
{
	if( iItemID <= 0  ||  iServiceCategoryNo <= 0  ||  iTestServCategoryNo <= 0 )
	{
		START_LOG( cerr, L"잘못된 상품 카테고리 정보 입니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iServiceCategoryNo )
			<< BUILD_LOG( iTestServCategoryNo )
			<< END_LOG;
		return false;
	}

	if( IsExistProductCategoryModifyInfoForItemID( iItemID ) == true )
	{
		START_LOG( cerr, L"이미 등록된 정보 입니다!" )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iServiceCategoryNo )
			<< BUILD_LOG( iTestServCategoryNo )
			<< END_LOG;
		return false;
	}

	KCategoryInfo kInfo;
	kInfo.m_iServiceCategory = iServiceCategoryNo;
	kInfo.m_iTestServCategory = iTestServCategoryNo;
	m_mapProductCategoryModifyForItemID.insert( std::make_pair( iItemID, kInfo ) );
	return true;;
}

bool KBillingScriptManager::GetModifiedProductCategoryForItemID( IN const int iItemID, IN const bool bService, OUT unsigned long& ulModifiedCategoryNo ) const
{
	std::map< int, KCategoryInfo >::const_iterator mit;
	mit = m_mapProductCategoryModifyForItemID.find( iItemID );
	if( mit == m_mapProductCategoryModifyForItemID.end() )
	{
		START_LOG( cerr, L"해당 상품에 대한 카테고리 수정 정보는 존재하지 않습니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	if( bService )
	{
		ulModifiedCategoryNo = static_cast<unsigned long>(mit->second.m_iServiceCategory);
	}
	else
	{
		ulModifiedCategoryNo = static_cast<unsigned long>(mit->second.m_iTestServCategory);
	}
	
	return true;
}

//#endif SERV_BILLING_SCRIPT_MANAGER
//}}


