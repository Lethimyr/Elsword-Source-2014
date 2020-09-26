#include ".\stattable.h"

#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"


ImplementSingleton( KStatTable );

KStatTable::KUnitStatInfo::KUnitStatInfo()
:
    m_usAtkPhysic( 0 ),
    m_usAtkMagic( 0 ),
    m_usDefPhysic( 0 ),
    m_usDefMagic( 0 ),
    m_uiHP( 0 )
{
}

KStatTable::KStatTable(void)
{
    RegisterLuaBind();
}

KStatTable::~KStatTable(void)
{
}

ImplToStringW( KStatTable )
{
    for( int i = 0; i < CXSLUnit::UT_END; ++i )
    {
        stm_ << L"Character Class : " << i << std::endl;

		std::map< int, std::vector< KUnitStatInfo > >::const_iterator mit;
        for( mit = m_mapStat.begin(); mit != m_mapStat.end(); ++mit )
        {
			std::vector< KUnitStatInfo >::const_iterator vit;			
			for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
			{
				stm_ << vit - mit->second.begin() << L" : "
					<< L"AtkPhysic = " << vit->m_usAtkPhysic
					<< L", AtkMagic = " << vit->m_usAtkMagic
					<< L", DefPhysic = " << vit->m_usDefPhysic
					<< L", DefMagic = " << vit->m_usDefMagic
					<< L", HP = " << vit->m_uiHP
					<< std::endl;
			}            
        }

        stm_ << std::endl;
    }

    return stm_;
}



bool KStatTable::OpenScriptFile( const char* pFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	return true;
}


bool KStatTable::GetUnitStat( IN char cUnitClass, IN u_char unitLevel, OUT KUnitStatInfo& sUnitStat )
{
    _JIF( 0 < cUnitClass && ( cUnitClass < CXSLUnit::UT_END || cUnitClass/100 < CXSLUnit::UT_END ), return false );

	std::map< int, std::vector< KUnitStatInfo > >::const_iterator mit;
	mit = m_mapStat.find( static_cast<int>( cUnitClass ) );
	_JIF( mit != m_mapStat.end(), return false; );
	_JIF( 0 < unitLevel && unitLevel < mit->second.size(), return false );

	sUnitStat = mit->second[unitLevel];

	return true;
}

void KStatTable::SetUnitStat( IN int nUnitClass, IN u_char unitLevel )
{
    JIF( 0 < nUnitClass && ( nUnitClass < CXSLUnit::UT_END || nUnitClass/100 < CXSLUnit::UT_END ) );

    KLuaManager luaMgr( g_pLua );

	std::map< int, std::vector< KUnitStatInfo > >::iterator mit;
	mit = m_mapStat.find( nUnitClass );
	JIF( mit != m_mapStat.end() );
	JIF( 0 < unitLevel && unitLevel < mit->second.size() );

    KUnitStatInfo& kInfo = mit->second[unitLevel];

    LUA_GET_VALUE( luaMgr, "AtkPhysic", kInfo.m_usAtkPhysic, 0 );
    LUA_GET_VALUE( luaMgr, "AtkMagic",  kInfo.m_usAtkMagic, 0 );
    LUA_GET_VALUE( luaMgr, "DefPhysic", kInfo.m_usDefPhysic, 0 );
    LUA_GET_VALUE( luaMgr, "DefMagic",  kInfo.m_usDefMagic, 0 );
    LUA_GET_VALUE( luaMgr, "HP",        kInfo.m_uiHP, 0 );

    // defence는 같은 수치를 사용한다.
    //kInfo.m_usDefMagic = kInfo.m_usDefPhysic;
}

void KStatTable::ReserveMemory( IN int nUnitClass, IN u_char ucMaxLevel )
{
    JIF( 0 < nUnitClass && ( nUnitClass < CXSLUnit::UT_END || nUnitClass/100 < CXSLUnit::UT_END ) );

	std::map< int, std::vector< KUnitStatInfo > >::iterator mit;
	mit = m_mapStat.find( nUnitClass );
	if( mit == m_mapStat.end() )
	{
		m_mapStat.insert( std::make_pair( nUnitClass, std::vector< KUnitStatInfo >() ) );
		mit = m_mapStat.find( nUnitClass );
		if( mit != m_mapStat.end() )
		{			
			mit->second.clear();
			mit->second.resize( ucMaxLevel + 1 );	// 0번 인덱스를 쓰지 않으므로, 한칸 더 많이 잡아줘야 한다.
		}
	}
}

void KStatTable::RegisterLuaBind()
{
 //   lua_tinker::class_<KStatTable>("KStatTable")
 //       .def("SetUnitStat",	KStatTable::SetUnitStat)
 //       .def("ReserveMemory", KStatTable::ReserveMemory)
 //       .def("dump",			KStatTable::Dump);

	//lua_tinker::decl( "StatTable", this );
	lua_tinker::class_add<KStatTable>( g_pLua, "KStatTable" );
	lua_tinker::class_def<KStatTable>( g_pLua, "SetUnitStat",	 &KStatTable::SetUnitStat );
	lua_tinker::class_def<KStatTable>( g_pLua, "ReserveMemory",  &KStatTable::ReserveMemory );
	lua_tinker::class_def<KStatTable>( g_pLua, "dump",			 &KStatTable::Dump );

	lua_tinker::decl( g_pLua, "StatTable", this );
}

