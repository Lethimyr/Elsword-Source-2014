#include "StoredProcedure.h"

#include "odbc/Odbc.h"
//#include "odbc/DBUtil.h"

#include "X2Data/XSLAttribEnchantItem.h"
#include "X2Data/XSLUnit.h"



KStoredProcedure::KStoredProcedure(void)
:m_kODBC( KODBC() )
{
}

KStoredProcedure::~KStoredProcedure(void)
{

}

void KStoredProcedure::GetUnitInfo(  IN const UidType& nUserUID_, OUT std::vector<KUnitInfo>& vecUnitInfo_ )
{
    DO_QUERY( L"exec dbo.P_GUnit_Userinfo_GET_20131212", L"%d", % nUserUID_ );

    while( m_kODBC.Fetch() )
    {
        KUnitInfo kUnitInfo;
        int iMapIDDummy = 0;
        std::wstring wstrLastDate(L"");
#ifdef SERV_UNIT_WAIT_DELETE
		std::wstring wstrDelAbleDate  = L"";
		std::wstring wstrRestoreAbleDate  = L"";
		CTime tDelAbleDateTime;
		CTime tRestoreAbleDateTime;

		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kUnitInfo.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kUnitInfo.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_iOfficialMatchCnt
			>> kUnitInfo.m_cRank
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kUnitInfo.m_wstrNickName
			>> iMapIDDummy
			>> kUnitInfo.m_wstrLastDate
			>> kUnitInfo.m_bDeleted
			>> wstrDelAbleDate
			>> wstrRestoreAbleDate
			);

		//{{ // 2012.03.06 lygan_조성욱 // 기존 문자열시간을 그대로 받는 부분을 서버에서 숫자로 변경해서 클라로 전달 구조로 변경
		if( KncUtil::ConvertStringToCTime( wstrDelAbleDate, tDelAbleDateTime ) == true )
			kUnitInfo.m_trDelAbleDate = tDelAbleDateTime.GetTime();
		else
			kUnitInfo.m_trDelAbleDate = 0LL;

		//{{ // 2012.03.06 lygan_조성욱 // 기존 문자열시간을 그대로 받는 부분을 서버에서 숫자로 변경해서 클라로 전달 구조로 변경
		if( KncUtil::ConvertStringToCTime( wstrRestoreAbleDate, tRestoreAbleDateTime ) == true )
			kUnitInfo.m_trRestoreAbleDate = tRestoreAbleDateTime.GetTime();
		else
			kUnitInfo.m_trRestoreAbleDate = 0LL;
		//}}
#else
        FETCH_DATA( kUnitInfo.m_nUnitUID
            >> kUnitInfo.m_cUnitClass
            >> kUnitInfo.m_iEXP
            >> kUnitInfo.m_ucLevel
            >> kUnitInfo.m_iED
            >> kUnitInfo.m_iOfficialMatchCnt
            >> kUnitInfo.m_cRank
            >> kUnitInfo.m_kStat.m_iBaseHP
            >> kUnitInfo.m_kStat.m_iAtkPhysic
            >> kUnitInfo.m_kStat.m_iAtkMagic
            >> kUnitInfo.m_kStat.m_iDefPhysic
            >> kUnitInfo.m_kStat.m_iDefMagic
            >> kUnitInfo.m_iSPoint
            >> kUnitInfo.m_wstrNickName
            >> iMapIDDummy
            >> kUnitInfo.m_wstrLastLoginTime
            );
#endif SERV_UNIT_WAIT_DELETE

        if( CXSLUnit::IsValidUnitClass( static_cast<CXSLUnit::UNIT_CLASS>(kUnitInfo.m_cUnitClass) ) == false )
        {
            START_LOG( cout, L"[알림] 등록되지 않은 유닛 클래스입니다!" )
                << BUILD_LOG( kUnitInfo.m_nUnitUID )
                << BUILD_LOG( kUnitInfo.m_wstrNickName )
                << BUILD_LOGc( kUnitInfo.m_cUnitClass );
            continue;
        }

        vecUnitInfo_.push_back( kUnitInfo );
    }

end_proc:
    ;
}

void KStoredProcedure::GetEquipItemList( IN std::vector<KUnitInfo>& vecUnitInfo_ )
{
    std::vector< KUnitInfo >::iterator vit;
    // 장착 아이템 정보
    for( vit = vecUnitInfo_.begin(); vit != vecUnitInfo_.end(); ++vit )
    {
        std::map< UidType, int > mapItemUIDSlot;
        mapItemUIDSlot.clear();

        // 보유 아이템을 얻는다.
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_SEL_EquippedItemList", L"%d", % vit->m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_Equipped_Item_SEL", L"%d", % vit->m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
        while( m_kODBC.Fetch() )
        {
            int iEnchantLevel = 0;
            // 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
            const byte byteArraySize = 5;

            int arrSocketOption[byteArraySize];
            memset( arrSocketOption, 0, sizeof(int) * byteArraySize );

            KInventoryItemInfo kInventoryItemInfo;
            // 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
            FETCH_DATA( kInventoryItemInfo.m_iItemUID
                >> kInventoryItemInfo.m_kItemInfo.m_iItemID
                >> kInventoryItemInfo.m_kItemInfo.m_cUsageType
                >> kInventoryItemInfo.m_kItemInfo.m_iQuantity
                >> kInventoryItemInfo.m_kItemInfo.m_sEndurance
                >> kInventoryItemInfo.m_kItemInfo.m_sPeriod
                >> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
                >> iEnchantLevel
                >> arrSocketOption[0]
                >> arrSocketOption[1]
                >> arrSocketOption[2]
                >> arrSocketOption[3]
                >> arrSocketOption[4]
                >> kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum
                >> kInventoryItemInfo.m_cSlotCategory
                >> kInventoryItemInfo.m_sSlotID );

            kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel = static_cast<char>(iEnchantLevel);

            int iCheckIdx = byteArraySize;
            while( 0 <= --iCheckIdx )
            {
                if( arrSocketOption[iCheckIdx] != 0 )
                    break;
            }

            for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
            {
                kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );				
            }

            //{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
            // 2-1. 랜덤 소켓 정보 업데이트
            //std::map< UidType, std::vector< int > >::const_iterator mitCRS;
            //mitCRS = mapRandomSocketList.find( kInventoryItemInfo.m_iItemUID );
            //if( mitCRS != mapRandomSocketList.end() )
            //{
            //kInventoryItemInfo.m_kItemInfo.m_vecRandomSocket = mitCRS->second;
            //}
#endif SERV_NEW_ITEM_SYSTEM_2013_05

            std::map< int, KInventoryItemInfo >::iterator mit;
            mit = vit->m_mapEquippedItem.find( kInventoryItemInfo.m_sSlotID );

            if( mit != vit->m_mapEquippedItem.end() )
            {
                START_LOG( cerr, L"장착 아이템 정보가 중복됨." )
                    << BUILD_LOG( kInventoryItemInfo.m_iItemUID )
                    << BUILD_LOG( kInventoryItemInfo.m_sSlotID )
                    << BUILD_LOG( kInventoryItemInfo.m_kItemInfo.m_iItemID )
                    << END_LOG;
            }
            vit->m_mapEquippedItem.insert( std::make_pair( kInventoryItemInfo.m_sSlotID, kInventoryItemInfo ) );

            mapItemUIDSlot.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo.m_sSlotID ) );
        }
        //////////////////////////////////////////////////////////////////////////
        // 속성 강화 얻기
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItemAttribute_SEL", L"%d", % vit->m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_attribute", L"%d", % vit->m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
        while( m_kODBC.Fetch() )
        {
            UidType iItemUID = 0;
            int iAttribEnchantSlotNo = 0;
            int iAttribEnchantID = 0;

            FETCH_DATA( iItemUID
                >> iAttribEnchantSlotNo
                >> iAttribEnchantID );

            std::map< UidType, int >::const_iterator mitSlot;
            mitSlot = mapItemUIDSlot.find( iItemUID );
            if( mitSlot == mapItemUIDSlot.end() )
                continue;            

            std::map< int, KInventoryItemInfo >::iterator mitAtt;
            mitAtt = vit->m_mapEquippedItem.find( mitSlot->second );
            if( mitAtt == vit->m_mapEquippedItem.end() )
                continue;

            // 속성 강화
            switch( iAttribEnchantSlotNo )
            {
            case CXSLAttribEnchantItem::ESI_SLOT_1:
                mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = static_cast<char>( iAttribEnchantID );
                break;

            case CXSLAttribEnchantItem::ESI_SLOT_2:
                mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 = static_cast<char>( iAttribEnchantID );
                break;

            case CXSLAttribEnchantItem::ESI_SLOT_3:
                mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 = static_cast<char>( iAttribEnchantID );
                break;

            default:
                START_LOG( cerr, L"속성 강화 슬롯 넘버가 이상합니다." )
                    << BUILD_LOG( iItemUID )
                    << BUILD_LOG( iAttribEnchantSlotNo )
                    << BUILD_LOG( iAttribEnchantID )
                    << END_LOG;
                break;
            }
        }
    }

end_proc:
    ;
}

int KStoredProcedure::GetUnitSlotNum( IN const UidType& nUserUID_, IN const std::wstring& strUserID_ )
{
#ifdef SERV_ALL_RENEWAL_SP
	int iDefaultUnitSlotCount = 10;	// 기본 국내
#if defined( SERV_COUNTRY_TWHK )
	iDefaultUnitSlotCount = 5;
#elif defined( SERV_COUNTRY_JP )
	iDefaultUnitSlotCount = 11;
#elif defined( SERV_COUNTRY_EU )
	iDefaultUnitSlotCount = 4;
#elif defined( SERV_COUNTRY_US )
	iDefaultUnitSlotCount = 4;
#elif defined( SERV_COUNTRY_CN )
	iDefaultUnitSlotCount = 4;
#elif defined( SERV_COUNTRY_TH )
	iDefaultUnitSlotCount = 4;
#elif defined( SERV_COUNTRY_ID )
	iDefaultUnitSlotCount = 4;	
#elif defined( SERV_COUNTRY_BR )
	iDefaultUnitSlotCount = 5;
#elif defined( SERV_COUNTRY_PH )
	iDefaultUnitSlotCount = 3;
#endif //SERV_COUNTRY_XX
#endif //SERV_ALL_RENEWAL_SP

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUser_MER", L"%d, N\'%s\', %d", % nUserUID_ % strUserID_.c_str() % iDefaultUnitSlotCount );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_get_user_info", L"%d, N\'%s\'", % nUserUID_  % strUserID_.c_str() );
#endif //SERV_ALL_RENEWAL_SP	

    int nUnitSlot = 0;
    if( m_kODBC.BeginFetch() )
    {        
        FETCH_DATA( nUnitSlot );

        m_kODBC.EndFetch();
    }

    START_LOG( clog, L"P_GUser_MER 실행 로그 " )
        << BUILD_LOG( nUnitSlot )
        << BUILD_LOG( nUserUID_ )
        << BUILD_LOG( strUserID_ )
        << BUILD_LOG( m_kODBC.GetLastQuery() )
        << END_LOG;

end_proc:
    return nUnitSlot;
}

UidType KStoredProcedure::GetUserUID( IN const std::wstring& strUserID_ )
{
    UidType nUserUID = 0;
    DO_QUERY( L"exec dbo.get_useruid_by_userid", L"N\'%s\'", % strUserID_.c_str() );

    //spODBC->Query()

    if( m_kODBC.BeginFetch() )
    { // 디비에 데이터 없는 경우에 -1 을 리턴받음. 2014-01-23 주석추가 , 우상혁     
        FETCH_DATA( nUserUID );
        
        m_kODBC.EndFetch();
    }
end_proc:

    START_LOG( clog, L"GetUserUID(get_useruid_by_userid) 실행 로그 " )
        << BUILD_LOG( nUserUID )
        << BUILD_LOG( strUserID_ )
        << BUILD_LOG( m_kODBC.GetLastQuery() )
        << END_LOG;

    return nUserUID;
}

bool KStoredProcedure::Query_GetGuildMemberGrade( IN UidType iUnitUID, OUT u_char& ucGuildMemberGrade )
{
    ucGuildMemberGrade = 0;
    int iHonorPointDummy = 0;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_SEL_Grade", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_member_grade", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( ucGuildMemberGrade
            >> iHonorPointDummy );

        m_kODBC.EndFetch();
    }

    if( ucGuildMemberGrade == -1 )
    {
        START_LOG( cwarn, L"길드원 등급 정보가 존재하지 않음." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        goto end_proc;
    }

    return true;

end_proc:
    return false;
}

void KStoredProcedure::GetLastPosition( IN std::vector<KUnitInfo>& vecUnitInfo_ )
{
    std::vector< KUnitInfo >::iterator vit;
    // 장착 아이템 정보
    for( vit = vecUnitInfo_.begin(); vit != vecUnitInfo_.end(); ++vit )
    {
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnitLastPosition_SEL", L"%d", % vit->m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnitLastPosition_GET", L"%d", % vit->m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP

        
        if( m_kODBC.BeginFetch() )
        {            
            FETCH_DATA( vit->m_kLastPos.m_iMapID
                >> vit->m_kLastPos.m_ucLastTouchLineIndex
                >> vit->m_kLastPos.m_usLastPosValue
                );

            m_kODBC.EndFetch();
        }
        
    }

    
end_proc:
    ;
}
