#include "XSLEDInventoryExpand.h"

ImplementRefreshSingleton( CXSLEDInventoryExpand )

CXSLEDInventoryExpand::CXSLEDInventoryExpand(void)
{
    
}

CXSLEDInventoryExpand::~CXSLEDInventoryExpand(void)
{

}

ImplToStringW( CXSLEDInventoryExpand )
{
    stm_ << L"----------[ ED Inventory Expand ]-----------" << std::endl
        << TOSTRINGW( m_mapBankGradeED.size() )
        << TOSTRINGW( m_mapInventoryGradeED.size() )
        ;
    return stm_;
}

ImplementLuaScriptParser( CXSLEDInventoryExpand )
{
    lua_tinker::class_add<CXSLEDInventoryExpand>( GetLuaState(), "CXSLEDInventoryExpand" );
    lua_tinker::class_def<CXSLEDInventoryExpand>( GetLuaState(), "AddBankGradeED", &CXSLEDInventoryExpand::AddBankGradeED_LUA );
    lua_tinker::class_def<CXSLEDInventoryExpand>( GetLuaState(), "AddInventoryGradeED", &CXSLEDInventoryExpand::AddInventoryGradeED_LUA );

    lua_tinker::decl( GetLuaState(), "g_pEDInventoryExpand", this );
}

bool CXSLEDInventoryExpand::AddBankGradeED_LUA()
{
    std::map< int, int > mapBankGradeED;
    mapBankGradeED.clear();

    KLuaManager luaManager( GetLuaState() );

    if ( S_OK == luaManager.BeginTable( "GRADE_ED" ) ) 
    {
        int iIndex = 1;

        while ( S_OK == luaManager.BeginTable( iIndex ) )
        {
            int iGrade = 0;
            int iED = 0;

            LUA_GET_VALUE_RETURN( luaManager, L"GRADE", iGrade, -1, goto end_proc );
            LUA_GET_VALUE_RETURN( luaManager, L"ED", iED, -1, goto end_proc );

            mapBankGradeED.insert( std::map<int, int>::value_type( iGrade, iED ) );
            luaManager.EndTable();
            ++iIndex;
        }
        luaManager.EndTable();
    }
    // lock 
    m_mapBankGradeED.swap( mapBankGradeED );
    return true;

end_proc:
    START_LOG( cerr, L"ED은행확장 스크립트 파싱 실패!" )
        << BUILD_LOG( mapBankGradeED.size() )
        << END_LOG;
    return false;
}


bool CXSLEDInventoryExpand::AddInventoryGradeED_LUA()
{
    std::map< int, int > mapGradeED;
    mapGradeED.clear();

    KLuaManager luaManager( GetLuaState() );

    if ( S_OK == luaManager.BeginTable( "GRADE_ED" ) ) 
    {
        int iIndex = 1;

        while ( S_OK == luaManager.BeginTable( iIndex ) )
        {
            int iGrade = 0;
            int iED = 0;

            LUA_GET_VALUE_RETURN( luaManager, L"GRADE", iGrade, -1, goto end_proc );
            LUA_GET_VALUE_RETURN( luaManager, L"ED", iED, -1, goto end_proc );

            mapGradeED.insert( std::map<int, int>::value_type( iGrade, iED ) );
            luaManager.EndTable();
            ++iIndex;
        }
        luaManager.EndTable();
    }
    // lock 
    m_mapInventoryGradeED.swap( mapGradeED );
    return true;

end_proc:
    START_LOG( cerr, L"ED 인벤토리 확장 스크립트 파싱 실패!" )
        << BUILD_LOG( mapGradeED.size() )
        << END_LOG;
    return false;
}

int CXSLEDInventoryExpand::GetNextBankUpgradeED( IN const int& iGrade_ )
{
    int iRet = 0;
    // lock 
    std::map< int, int >::const_iterator mit;
    mit = m_mapBankGradeED.find( iGrade_ );

    if ( mit != m_mapBankGradeED.end() ) 
    {
        iRet = mit->second;
    }

    return iRet;
}

int CXSLEDInventoryExpand::GetNextInventoryUpgradeED( IN const int& iGrade_ )
{
    int iRet = 0;
    //lock
    std::map< int, int >::const_iterator mit;
    mit = m_mapInventoryGradeED.find( iGrade_ );
    
    if ( mit != m_mapInventoryGradeED.end() )
    {
        iRet = mit->second;
    }

    return iRet;
}