#include "XSLRidingPetManager.h"

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
ImplementRefreshSingleton( CXSLRidingPetManager );

CXSLRidingPetManager::CXSLRidingPetManager(void)
: m_fEnableStamina( 0.0f )
{
}

CXSLRidingPetManager::~CXSLRidingPetManager(void)
{
}

ImplToStringW( CXSLRidingPetManager )
{
	stm_	<< L"----------[ Riding Pet Manager ]----------" << std::endl
		<< TOSTRINGW( m_mapRidingPetTemplet.size() )
		<< TOSTRINGW( m_mapRidingPetCreateItem.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLRidingPetManager )
{
	lua_tinker::class_add<CXSLRidingPetManager>( GetLuaState(), "CXSLRidingPetManager" );
	lua_tinker::class_def<CXSLRidingPetManager>( GetLuaState(), "AddRidingPetTemplet",			&CXSLRidingPetManager::AddRidingPetTemplet_LUA );
	lua_tinker::class_def<CXSLRidingPetManager>( GetLuaState(), "AddRidingPetCreateItemInfo",	&CXSLRidingPetManager::AddRidingPetCreateItemInfo_LUA );
	lua_tinker::class_def<CXSLRidingPetManager>( GetLuaState(), "SetEnableStamina",				&CXSLRidingPetManager::SetEnableStamina_LUA );

	lua_tinker::decl( GetLuaState(), "g_pRidingPetManager", this );
}

bool CXSLRidingPetManager::AddRidingPetTemplet_LUA()
{
	KLuaManager luaManager( GetLuaState() );

	RidingPetTemplet kData;
	float fStaminaIncreaseRate;

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"RIDING_PET_ID",			kData.m_iRidingPetID,			RIDING_PET_UNIT_ID,	RPUI_NONE,	goto end_proc );
	LUA_GET_VALUE_RETURN(		luaManager, L"STAMINA_INCREASE_RATE",	fStaminaIncreaseRate,			0.0f,	goto end_proc );
	LUA_GET_VALUE_RETURN(		luaManager, L"STAMINA_RECOVERY_RATE",	kData.m_fStaminaRecoveryRate,	0.0f,	goto end_proc );

	if( m_mapRidingPetTemplet.find( kData.m_iRidingPetID ) != m_mapRidingPetTemplet.end() )
	{
		START_LOG( cerr ,L"이미 등록된 펫 정보입니다!" )
			<< BUILD_LOG( kData.m_iRidingPetID )
			<< END_LOG;
		goto end_proc;	
	}

	kData.m_usMaxStamina = lua_tinker::call<USHORT>( GetLuaState(), "COMPUTE_RIDING_PET_MAX_STAMINA", fStaminaIncreaseRate );

	m_mapRidingPetTemplet.insert( std::map<RIDING_PET_UNIT_ID, RidingPetTemplet>::value_type( kData.m_iRidingPetID, kData ) );
	return true;

end_proc:
	START_LOG( cerr, L"라이딩 펫 스크립트 파싱 실패.!" )
		<< BUILD_LOG( kData.m_iRidingPetID )
		<< END_LOG;
	return false;
}

bool CXSLRidingPetManager::AddRidingPetCreateItemInfo_LUA( int iItemID, USHORT usRidingPetID, short sPeriod )
{
	if( IsExistRidingPetID( usRidingPetID ) == false )
	{
		START_LOG( cerr, L"등록 되지 않은 RidingPetID입니다!" )
			<< BUILD_LOG( usRidingPetID )
			<< END_LOG;
		return false;
	}

	if( m_mapRidingPetCreateItem.find( iItemID ) != m_mapRidingPetCreateItem.end() )
	{
		START_LOG( cerr, L"이미 등록된 ItemID입니다!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	m_mapRidingPetCreateItem.insert( std::map< int, std::pair<RIDING_PET_UNIT_ID, short> >::value_type( iItemID, std::make_pair( static_cast<RIDING_PET_UNIT_ID>( usRidingPetID ), sPeriod ) ) );
	return true;
}

const CXSLRidingPetManager::RidingPetTemplet* CXSLRidingPetManager::GetRidingPetTemplet( IN const USHORT usRidingPetID ) const
{
	std::map<RIDING_PET_UNIT_ID, RidingPetTemplet>::const_iterator it = m_mapRidingPetTemplet.find( static_cast<RIDING_PET_UNIT_ID>( usRidingPetID ) );
	if( it == m_mapRidingPetTemplet.end() )
	{
		START_LOG( cerr, L"존재하지 않는 RidingPetID입니다!" )
			<< BUILD_LOG( usRidingPetID )
			<< END_LOG;
		return NULL;
	}

	return &it->second;
}

void CXSLRidingPetManager::GetRidingPetID( IN const int iItemID, OUT USHORT& usCreatePetID, OUT short& sPeriod ) const
{
	std::map< int, std::pair<RIDING_PET_UNIT_ID, short> >::const_iterator it = m_mapRidingPetCreateItem.find( iItemID );
	if( it == m_mapRidingPetCreateItem.end() )
	{
		usCreatePetID = RPUI_NONE;
		return;
	}

	usCreatePetID	= it->second.first;
	sPeriod			= it->second.second;
}

float CXSLRidingPetManager::GetRecoveryTimePerOne( void )
{
	return lua_tinker::call<float>( GetLuaState(), "GET_RECOVERY_TIME_PER_ONE" );
}
#endif	// SERV_RIDING_PET_SYSTM