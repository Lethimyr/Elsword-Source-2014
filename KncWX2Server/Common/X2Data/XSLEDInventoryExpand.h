#pragma once
#include "ServerDefine.h"

#include "RefreshSingleton.h"

class CXSLEDInventoryExpand
{
    DeclareRefreshSingleton( CXSLEDInventoryExpand );
    DeclareLuaScriptParser;
    DeclDumpToLogFile;
    DeclDump;
    DeclToStringW;

public:
    CXSLEDInventoryExpand(void);
    ~CXSLEDInventoryExpand(void);

    bool AddBankGradeED_LUA();
    bool AddInventoryGradeED_LUA();
    int GetNextBankUpgradeED( IN const int& iGrade_ );
    int GetNextInventoryUpgradeED( IN const int& iGrade_ );

private:
    std::map< int, int > m_mapBankGradeED; // first : 은행 등급 ,  second : 소모되는 ED
    std::map< int, int > m_mapInventoryGradeED; // first : 인벤토리 등급, second : 소모되는 ED
};

DefRefreshSingletonInline( CXSLEDInventoryExpand );