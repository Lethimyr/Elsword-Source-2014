#pragma once

#include <vector>
#include <map>
#include "ActionDecider.h"
#include <ToString.h>
#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/object.hpp"   // luabind::object
#include "UserPacket.h"         // MAX_NUM_MON_SLOT

struct KDropItemDetail
{
    enum DROP_RECEIVER
    {
        DR_ALL,
        DR_HOST,

        DR_NUM,
    };

    enum DROP_TYPE
    {
        DT_NORMAL,
        DT_ALLWAYS,     //항상 준다.
        DT_ADMIN_EVENT,

        DT_NUM,
    };

    std::map<int, int> m_mapItemList;       // (job id, item id) pairs
    
    int m_iNum;                             // the number of items to be dropped
    int m_iDropType;                        // type of drop event
    int m_iDropReceiver;                    // receiver filter
    int m_iConditionItem;                   // necessary item to receive reward when m_iDropReceiver == DR_ITEM_EXIST
    int m_iNumRoomMember;                   // number of room member filter

    KDropItemDetail() : m_iNum( 1 ), m_iDropType( DT_NORMAL ), m_iDropReceiver( DR_ALL ), m_iConditionItem( -1 ) { }

    KDropItemDetail( const KDropItemDetail& t ){ *this = t; }

    KDropItemDetail& operator=( const KDropItemDetail& t )
    {
        m_mapItemList       = t.m_mapItemList;
        m_iNum              = t.m_iNum;
        m_iDropType         = t.m_iDropType;
        m_iDropReceiver     = t.m_iDropReceiver;
        m_iConditionItem    = t.m_iConditionItem;
        m_iNumRoomMember    = t.m_iNumRoomMember;
        return *this;
    }

    void Init()
    {
        m_mapItemList.clear();
        m_iNum              = 1;
        m_iDropType         = DT_NORMAL;
        m_iDropReceiver     = DR_ALL;
        m_iConditionItem    = -1;
        m_iNumRoomMember    = 0;
    }
};

struct KDropInfo
{
    int m_iTestGroup;                                   // When the action decider roll the roulette at some state, 
                                                        // a KDropInfo would be chosen among the KDropInfo's
                                                        // those have same test group id with the state id.

    KDropItemDetail m_kDropItemDetail;                  // items to be dropped
    float m_fProb;                                      // drop probability
    std::map<int, float> m_mapMonsterAffectedProb;      // (monster id, probability to be added) pairs

    KDropInfo() : m_iTestGroup( -1 ), m_fProb( 0.0f ) {}

    KDropInfo( const KDropInfo& t )
    {
        *this = t;
    }

    KDropInfo& operator=( const KDropInfo& t )
    {
        m_iTestGroup = t.m_iTestGroup;
        m_kDropItemDetail = t.m_kDropItemDetail;
        m_fProb = t.m_fProb;
        m_mapMonsterAffectedProb = t.m_mapMonsterAffectedProb;

        return *this;
    }

    void Init()
    {
        m_iTestGroup = -1;
        m_kDropItemDetail.Init();
        m_fProb = 0.0f;
        m_mapMonsterAffectedProb.clear();
    }

    // for lua
    void InitFromLuaTable( const luabind::object& table );
};

class KDropMode
{
    DeclToStringW;
public:
    enum DROP_METHOD
    {
        DM_ALL,
        DM_PER_USER,

        DM_NUM,
    };

    KDropMode() : m_iNextID( 0 ),m_iDropMethod( 0 ) {}

    KDropMode( const KDropMode& t );

    KDropMode& operator=( const KDropMode& t );

    // for lua
    void AddDropInfo( const luabind::object& table );

    void ProcessDrop( std::vector<int>& vecMonSlot, OUT std::vector<KDropItemDetail>& vecDropItemDetail );

    int m_iDropMethod;
protected:
    int m_iNextID;                                  // to identify KDropInfo's
    KActionDecider m_kActionDecider;                // rolls a roulette and makes some decision
    std::map<int, KDropInfo> m_mapDropInfoSet;      // (id of KDropInfo, KDropInfo) pairs
};

DeclOstmOperatorW( KDropMode );



class KDropSystem
{
    DeclToStringW;
public:
    KDropSystem();
    KDropSystem( const KDropSystem& t );

    void ProcessDrop( int iMainKey, int iSubKey, int iDifficult, int ireserve, std::vector<int>& vecMonSlot, OUT std::vector<KDropItemDetail>& vecDropItemDetail );
    void ProcessAdminEvent( std::vector<KDropItemDetail>& vecDropItemDetail );

    KDropSystem& operator=( const KDropSystem& right );

    // for lua
    void AddMode( int iMainKey, int iSubKey, int iDifficult, int ireserve, KDropMode& kMode, int iDropMethod = KDropMode::DROP_METHOD::DM_ALL  );
    bool GetMode( IN const int iMainKey, IN const int iSubKey, IN const int iDifficult, IN const int ireserve, OUT KDropMode& kMode );
    void EnableAdminEvent( bool bEnable, int iItemID, int iNumItem );
    void PrintAdminEvent();
    bool GetAdminEventInfo( std::pair<int, int>& pairAdminEventItem );

    enum MazeStageID
    {
        MSI_MAZE1       = 6,
        MSI_MAZE2       = 14,
        MSI_MAZE3       = 9,
        MSI_MAZE4       = 15,
        MSI_MAZE5       = 12,
        MSI_MAZE6       = 24,
    };

protected:
    std::map<DWORD, KDropMode> m_mapDropModeList;     // (game mode id, KDropMode) pairs

    bool m_bEnableAdminEvent;                       // while this flag is set, users who have played the game with an administrator will be given an item.
    std::pair<int, int> m_pairAdminEventItem;       // (item id, number of items) pair which is used for the admin event
};

DeclOstmOperatorA( KDropSystem );
DeclOstmOperatorW( KDropSystem );
