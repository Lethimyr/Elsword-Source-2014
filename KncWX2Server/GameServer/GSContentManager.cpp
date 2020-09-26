#include ".\gscontentmanager.h"


//{{ 2011. 10. 12	ÃÖÀ°»ç	ÄÁÅÙÃ÷ °ü¸®ÀÚ
//#ifdef SERV_CONTENT_MANAGER


ImplementRefreshSingleton( KGSContentManager );

KGSContentManager::KGSContentManager(void)
{
#ifdef SERV_CONTENT_MANAGER_INT
	Init();
#endif SERV_CONTENT_MANAGER_INT
}

KGSContentManager::~KGSContentManager(void)
{
}

ImplToStringW( KGSContentManager )
{
	stm_	<< L"----------[ Game Server Content Manager ]----------" << std::endl
			<< TOSTRINGW( m_bEnableCashShop )
		;

	return stm_;
}

ImplementLuaScriptParser( KGSContentManager )
{
	lua_tinker::class_add<KGSContentManager>( GetLuaState(), "KGSContentManager" );
	lua_tinker::class_def<KGSContentManager>( GetLuaState(), "SetEnableCashShop",		&KGSContentManager::SetEnableCashShop_LUA );
	lua_tinker::class_def<KGSContentManager>( GetLuaState(), "dump",					&KGSContentManager::Dump );

	lua_tinker::decl( GetLuaState(), "GSContentManager", this );
}

void KGSContentManager::SetEnableCashShop_LUA( bool bVal )
{
	m_bEnableCashShop = bVal;

	START_LOG( cout, L"[ÄÁÅÙÃ÷ ¸Å´ÏÀú] Ä³½¬¼¥ ±â´É : " << m_bEnableCashShop );
}

#ifdef SERV_CONTENT_MANAGER_INT
void KGSContentManager::SetEnableCashShop( bool bVal )
{
	if( m_bEnableCashShop != bVal )
	{
		START_LOG( cout, L"[ÄÁÅÙÃ÷ ¸Å´ÏÀú] Ä³½¬¼¥ ±â´É : " << bVal );
	}

	m_bEnableCashShop = bVal;
}

void KGSContentManager::Init()
{
	m_iReleaseTick = -1;
	m_bEnableCashShop = true;
}
#endif SERV_CONTENT_MANAGER_INT

//#endif SERV_CONTENT_MANAGER
//}}


