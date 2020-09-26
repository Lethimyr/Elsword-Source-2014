#pragma once

//#include <ServerDefine.h>
#include "SubThread.h"
#include "Event.h"
#include "odbc/DBUtil.h"

#include "CommonPacket.h"
//#include "KncUidType.h"
class KODBC;

#define IN
#define OUT

struct KUnitInfo;

class KStoredProcedure
{
    DeclareSPProfiler;

private:
    KODBC& m_kODBC;
    KStoredProcedure(void);
public:
    
    KStoredProcedure( KODBC& kODBC ) : m_kODBC(kODBC) {};
    ~KStoredProcedure(void);

    void GetUnitInfo(  IN const UidType& nUserUID_, OUT std::vector<KUnitInfo>& vecUnitInfo_ );
    void GetEquipItemList( IN std::vector<KUnitInfo>& vecUnitInfo_ );
    int GetUnitSlotNum( IN const UidType& nUserUID_, IN const std::wstring& strUserID_ );
    UidType GetUserUID( IN const std::wstring& strUserID_ );
    bool Query_GetGuildMemberGrade( IN UidType iUnitUID, OUT u_char& ucGuildMemberGrade );
    void GetLastPosition( IN std::vector<KUnitInfo>& vecUnitInfo_ );
};