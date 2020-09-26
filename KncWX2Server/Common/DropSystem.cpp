#include "DropSystem.h"
#include "GCEnum.h"

// KDropInfo

void KDropInfo::InitFromLuaTable( const luabind::object& table_ )
{
    Init();

    JIF( table_.type() == LUA_TTABLE );

    // default value. lua에서 설정 생략하면 다음 값으로 처리된다.
    m_iTestGroup                        = 0;   
    m_kDropItemDetail.m_iDropReceiver   = KDropItemDetail::DR_ALL;
    m_mapMonsterAffectedProb.clear();

    std::string strKey;
    luabind::object::iterator itOuter;
    luabind::object::iterator itInner;
    for( itOuter = table_.begin(); itOuter != table_.end(); ++itOuter )
    {
        strKey = luabind::object_cast<std::string>( itOuter.key() );
        if( strKey == "TestGroup" )
        {
            m_iTestGroup = luabind::object_cast<int>( *itOuter );
        }
        else if( strKey == "NumItem" )
        {
            m_kDropItemDetail.m_iNum = luabind::object_cast<int>( *itOuter );
        }
        else if( strKey == "Probability" )
        {
            m_fProb = luabind::object_cast<float>( *itOuter );
        }
        else if( strKey == "ConditionItem" )
        {
            m_kDropItemDetail.m_iConditionItem = luabind::object_cast<int>( *itOuter );
        }
        else if( strKey == "NumRoomMember" )
        {
            m_kDropItemDetail.m_iNumRoomMember = luabind::object_cast<int>( *itOuter );
        }
        else if( strKey == "Receiver" )
        {
            m_kDropItemDetail.m_iDropReceiver = luabind ::object_cast<int>( *itOuter );
        }
        else if( strKey == "DropType" )
        {
            m_kDropItemDetail.m_iDropType = luabind ::object_cast<int>( *itOuter );
        }
        else if( strKey == "ItemList" )
        {
            luabind::object ItemListTable = table_[ "ItemList" ];
            for( itInner = ItemListTable.begin(); itInner != ItemListTable.end(); ++itInner )
            {
                int index = luabind::object_cast<int>( itInner.key() ) - 1;     // lua : 1-base
                m_kDropItemDetail.m_mapItemList[index] = luabind::object_cast<int>( *itInner );
            }
        }
        else if( strKey == "MonsterAffectedProb" )
        {
            luabind::object MonsterAffectedProbTable = table_[ "MonsterAffectedProb" ];
            for( itInner = MonsterAffectedProbTable.begin(); itInner != MonsterAffectedProbTable.end(); ++itInner )
            {
                int index = luabind::object_cast<int>( itInner.key() ) - 1;     // lua : 1-base
                m_mapMonsterAffectedProb[index] = luabind::object_cast<float>( *itInner );
            }
        }
        else
        {
            START_LOG( cerr, L"invalid key value." ) << END_LOG;
        }
    }
}

// KDropMode

KDropMode::KDropMode( const KDropMode& t_ )
{
    *this = t_;
}

void KDropMode::AddDropInfo( const luabind::object& table_ )
{
    KDropInfo kDropInfo;

    kDropInfo.InitFromLuaTable( table_ );

    std::map<int, KDropInfo>::iterator itDropInfo;

    itDropInfo = m_mapDropInfoSet.find( m_iNextID );
    if( itDropInfo != m_mapDropInfoSet.end() )
    {
        START_LOG( cwarn, L"추가하려는 DropInfo의 번호가 이미 존재함." )
            << BUILD_LOG( m_iNextID )
            << END_LOG;
    }

    m_mapDropInfoSet[m_iNextID] = kDropInfo;
    m_kActionDecider.AddAction( kDropInfo.m_iTestGroup, m_iNextID, kDropInfo.m_fProb );

    m_iNextID++;
}

void KDropMode::ProcessDrop( IN std::vector<int>& vecMonSlot_, OUT std::vector<KDropItemDetail>& vecDropItemDetail_ )
{
    KActionDecider kActionDecider = m_kActionDecider;

    std::map<int, int>::const_iterator monsterInfoIter;
    std::map<int, KDropInfo>::iterator dropInfoIter;

    for( int i = 0; i < (int)vecMonSlot_.size(); i++ )
    {
        if( vecMonSlot_[i] == 0 ) continue;

        dropInfoIter = m_mapDropInfoSet.begin();
        while( dropInfoIter != m_mapDropInfoSet.end() )
        {
            std::map<int, float>::iterator monAffectedProbIter;

            if( !(dropInfoIter->second).m_mapMonsterAffectedProb.empty() )
            {
                monAffectedProbIter = ( dropInfoIter->second ).m_mapMonsterAffectedProb.find( i );
                if( monAffectedProbIter != ( dropInfoIter->second ).m_mapMonsterAffectedProb.end() )
                {
                    kActionDecider.AddAction( dropInfoIter->second.m_iTestGroup, dropInfoIter->first, monAffectedProbIter->second * ( float )vecMonSlot_[i] );
                }
            }

            dropInfoIter++;
        }
    }

    std::map<int, int> mapResult;
    std::map<int, int>::iterator resultIter;

    kActionDecider.MakeDecision( mapResult );

    vecDropItemDetail_.clear();

    resultIter = mapResult.begin();
    while( resultIter != mapResult.end() )
    {
        dropInfoIter = m_mapDropInfoSet.find( resultIter->second );
        if( dropInfoIter == m_mapDropInfoSet.end() )
        {
            START_LOG( cerr, L"Drop 처리의 KDropInfo 인덱스가 이상함." )
                << BUILD_LOG( resultIter->second )
                << END_LOG;

            vecDropItemDetail_.clear();
            return;
        }

        START_LOG( clog, L"드랍된 아이템의 DropInfo index : " << resultIter->second );

        vecDropItemDetail_.push_back( dropInfoIter->second.m_kDropItemDetail );
        resultIter++;
    }
}

KDropMode& KDropMode::operator=( const KDropMode& t )
{
    m_iDropMethod       = t.m_iDropMethod;
    m_iNextID           = t.m_iNextID;
    m_kActionDecider    = t.m_kActionDecider;
    m_mapDropInfoSet    = t.m_mapDropInfoSet;

    return *this;
}

ImplOstmOperatorW2A( KDropMode );
ImplOstmOperatorW( KDropMode );
ImplToStringW( KDropMode )
{
    return stm_ << L"KDropMode Dump." << std::endl
        << TOSTRINGW( m_iDropMethod )
        << TOSTRINGW( m_iNextID )
        << TOSTRINGW( m_mapDropInfoSet.size() )
        << m_kActionDecider << std::endl;
}

// KDropSystem

KDropSystem::KDropSystem()
{
    m_bEnableAdminEvent = false;
    m_pairAdminEventItem = std::make_pair( -1, 0 );
}

KDropSystem::KDropSystem( const KDropSystem& t )
{
    *this = t;
}

void KDropSystem::ProcessDrop( int iMainKey_, int iSubKey_, int iDifficult_, int ireserve_,
                              std::vector<int>& vecMonSlot_, OUT std::vector<KDropItemDetail>& vecDropItemDetail_ )
{
    DWORD dwKey = (iMainKey_ << 24) + ( iSubKey_ << 16) + ( iDifficult_ << 8 ) + ( 0x000000ff & ireserve_ ); // 생성된 조합키

    std::map<DWORD, KDropMode>::iterator mit;
    mit = m_mapDropModeList.find( dwKey );

    if( mit != m_mapDropModeList.end() )
    {
        mit->second.ProcessDrop( vecMonSlot_, vecDropItemDetail_ );
        return;
    }

    START_LOG( clog, L"드랍 시스템에 등록된 모드가 아님." )
        << BUILD_LOG( iMainKey_ )
        << BUILD_LOG( iSubKey_ )
        << BUILD_LOG( iDifficult_ )
        << BUILD_LOG( ireserve_ )
        << BUILD_LOG( dwKey );
}

void KDropSystem::ProcessAdminEvent( std::vector<KDropItemDetail>& vecDropItemDetail_ )
{
    if( m_bEnableAdminEvent )
    {
        KDropItemDetail kDropItemDetail;
        int i;

        // there is no explicit constant defined for the number of jobs.
        for( i = 0; i < GC_CHAR_NUM * 3; i++ )
        {
            kDropItemDetail.m_mapItemList[i] = m_pairAdminEventItem.first;
        }
        kDropItemDetail.m_iNum = m_pairAdminEventItem.second;
        kDropItemDetail.m_iDropType = KDropItemDetail::DROP_TYPE::DT_ADMIN_EVENT;

        vecDropItemDetail_.push_back( kDropItemDetail );
    }
}

KDropSystem& KDropSystem::operator=( const KDropSystem& right_ )
{
    m_mapDropModeList       = right_.m_mapDropModeList;
    m_bEnableAdminEvent     = right_.m_bEnableAdminEvent;
    m_pairAdminEventItem    = right_.m_pairAdminEventItem;

    return *this;
}

void KDropSystem::AddMode( int iMainKey_, int iSubKey_, int iDifficult_, int ireserve_,  KDropMode& kMode_, int iDropMethod_ )
{
    DWORD dwKey = (iMainKey_ << 24) + ( iSubKey_ << 16) + ( iDifficult_ << 8 ) + ( 0x000000ff & ireserve_ ); // 생성된 조합키

    std::map<DWORD, KDropMode>::iterator mit;
    mit = m_mapDropModeList.find( dwKey );
    if( mit != m_mapDropModeList.end() )
    {
        START_LOG( cwarn, L"드랍 시스템에 이미 등록된 모드를 등록하려고 함." )
            << BUILD_LOG( iMainKey_ )
            << BUILD_LOG( iSubKey_ )
            << BUILD_LOG( iDifficult_ )
            << BUILD_LOG( ireserve_ )
            << BUILD_LOG( dwKey );
    }
    
    kMode_.m_iDropMethod = iDropMethod_;
    m_mapDropModeList[dwKey] = kMode_;

}

bool KDropSystem::GetMode( IN const int iMainKey_, IN const int iSubKey_, IN const int iDifficult_, IN const int ireserve_, OUT KDropMode& kMode_ )
{
    DWORD dwKey = (iMainKey_ << 24) + ( iSubKey_ << 16) + ( iDifficult_ << 8 ) + ( 0x000000ff & ireserve_ );
    std::map<DWORD, KDropMode>::iterator mit = m_mapDropModeList.find( dwKey );
    if( mit != m_mapDropModeList.end() )
    {
        kMode_ = mit->second;
        return true;
    }

    START_LOG( clog, L"얻어내려는 드랍 모드가 없음" );
    return false;
}

void KDropSystem::EnableAdminEvent( bool bEnable_, int iItemID_, int iNumItem_ )
{
    // suppose that the id of every item is greater than or equal to 0.
    if( bEnable_ && iItemID_ >= 0 && iNumItem_ > 0 )
    {
        m_bEnableAdminEvent = true;
        m_pairAdminEventItem = std::make_pair( iItemID_, iNumItem_ );
        START_LOG( cout, L"운영자 기능이 활성화 되었습니다." )
            << BUILD_LOG( iItemID_ )
            << BUILD_LOG( iNumItem_ );
    }
    else
    {
        m_bEnableAdminEvent = false;
        m_pairAdminEventItem = std::make_pair( -1, 0 );
        START_LOG( cout, L"운영자 기능이 비활성화 되었습니다." );
    }
}

bool KDropSystem::GetAdminEventInfo( std::pair<int, int>& pairAdminEventItem_ )
{
    pairAdminEventItem_ = m_pairAdminEventItem;
    return m_bEnableAdminEvent;
}


ImplOstmOperatorW( KDropSystem );
ImplOstmOperatorW2A( KDropSystem );

ImplToStringW( KDropSystem )
{
    return stm_ << L"-> KDropSystem ( Admin Event Information )" << std::endl
        << L"    " << ( m_bEnableAdminEvent ? L"Admin Event Enabled." : L"Admin Event Disabled." ) << std::endl
        << L"    Event Item ID : " << m_pairAdminEventItem.first << L", " << L"Number of Items : " << m_pairAdminEventItem.second;
}