#include "GSUser.h"
#include "Inventory.h"

#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLRandomItemManager.h"
#include "X2Data/XSLManufactureItemManager.h"
#include "X2Data/XSLResolveItemManager.h"
#include "X2Data/XSLEnchantItemManager.h"
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLAttribEnchantItem.h"
#include "X2Data/XSLCashItemManager.h"
#include "GameSysVal/GameSysVal.h"
#include "NetError.h"

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 09. 26  임규수 칭호 수리비 감소 속성 적용
#ifdef SERV_TITLE_REPAIR_DISCOUNT
	#include "X2Data/XSLTitleManager.h"
#endif SERV_TITLE_REPAIR_DISCOUNT
//}}
//{{ 2012. 03. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}
//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
#include "X2Data/XSLSynthesisManager.h"
#endif SERV_SYNTHESIS_AVATAR
//}}
//{{ 2013. 04. 14	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON			
	#include "X2Data/XSLDefenceDungeonManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}
//{{ 2013. 05. 20	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	#include "X2Data/XSLCharacterStatTable.h"
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_GOOD_ELSWORD
    #include "X2Data/XSLEDInventoryExpand.h"
#endif //SERV_GOOD_ELSWORD

#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
#include <boost/foreach.hpp>
#endif SERV_MULTIPLE_BLESSED_ENCHANT_STONE

KInventory::KInventory()
{
    m_pUser = NULL;
#ifdef	SERV_SHARING_BANK_TEST
	m_iShareBankSize = 0;
	m_iShareUnitUID = 0;
#endif	SERV_SHARING_BANK_TEST

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	m_bUseItemNeedPayment = false;
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
}

KInventory::~KInventory()
{
}

void KInventory::Clear()
{
    m_mapItem.clear();
    m_vecDeletedItem.clear();
    for( int i = 0; i < CXSLInventory::ST_END; ++i )
    {
        m_vecInventorySlot[i].clear();
    }	
    m_mapItemCoolTime.clear();

	//{{ 2009. 8. 7  최육사		은행
	m_setGetMyInventory.clear();
	//}}
	//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
	m_kInventoryLocker.Clear();
#endif SERV_INVENTORY_LOCK
	//}}
}

void KInventory::SetOwner( KGSUser* pUser )
{
    m_pUser = pUser;
}

//{{ 2010. 8. 3	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
bool KInventory::Init( IN const std::map< int, int >& mapSlotSize, 
					   IN const std::map< UidType, KInventoryItemInfo >& mapInventoryItem, 
					   OUT std::map< UidType, KItemPosition >& mapWrongPosItem, 
					   IN const bool bIsChannelChange )
//#else
//bool KInventory::Init( IN const std::map< int, int >& mapSlotSize, 
//					   IN const std::map< UidType, KInventoryItemInfo >& mapInventoryItem, 
//					   OUT std::map< UidType, std::pair< int, int > >& mapWrongPosItem, 
//					   IN bool bIsChannelChange )
//#endif SERV_PET_SYSTEM
//}}
{
	Clear();
	mapWrongPosItem.clear();
        
    for( int iCategory = GetFirstCategory(); iCategory < GetSentinelCategory(); ++iCategory )
    {
        int iSlotSize = GetBaseSlotSize( iCategory );

		std::map< int, int >::const_iterator mit = mapSlotSize.find( iCategory );
        if( mit != mapSlotSize.end() )
        {
            LIF( mit->second > 0 );
            iSlotSize += mit->second;
        }

		//{{ 2009. 8. 7  최육사		은행
		if( iCategory == CXSLInventory::ST_BANK )
		{
			if( CXSLInventory::IsValidBankMemberShip( (CXSLInventory::MEMBERSHIP_PRIVATE_BANK)iSlotSize ) == false )
			{
				START_LOG( cerr, L"은행 멤버쉽 정보가 이상합니다. 절대 발생하면 안되는에러!" )
					<< BUILD_LOG( m_pUser->GetCharUID() )
					<< BUILD_LOG( iSlotSize )
					<< END_LOG;

				iSlotSize = GetBaseSlotSize( iCategory );
			}
		}		
		//}}
		//{{ 2010. 8. 2	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		else if( iCategory == CXSLInventory::ST_PET )
		{
			// 펫의 슬롯 크기는 여기서 초기화 하지 않습니다!
			continue;
		}
#endif SERV_PET_SYSTEM
		//}}

        if( iSlotSize > 0 )
        {
            m_vecInventorySlot[iCategory].insert( m_vecInventorySlot[iCategory].end(), iSlotSize, 0 );
        }
    }

	std::map< UidType, KItemInfo > mapNotInsertedItem;
    
	BOOST_MAP_CONST_FOREACH( UidType, KInventoryItemInfo, kDB, mapInventoryItem )
    {
        // mapInventoryItem 과 m_mapItem의 키가 같기 때문에 m_mapItem 에 현재 루프의
        // 아이템 UID가 이미 존재하는지 검사할 필요가 없다.

		//{{ 2010. 8. 2	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		if( kDB.second.m_cSlotCategory == CXSLInventory::ST_PET )
		{
			START_LOG( cerr, L"펫 아이템이 여기에 들어왔다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kDB.second.m_iItemUID )
				<< END_LOG;
			continue;
		}
#endif SERV_PET_SYSTEM
		//}}

        // 위치 업데이트가 안되었거나 운영툴로 추가한 아이템이면 카테고리와 슬롯 ID가 0 이다.
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		if( kDB.second.m_cSlotCategory == 0  &&  kDB.second.m_sSlotID == 0 )
//#else
//		if( kDB.second.m_cSlotCategory == 0  &&  kDB.second.m_cSlotID == 0 )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
        {
            mapNotInsertedItem.insert( std::make_pair( kDB.first, kDB.second.m_kItemInfo ) );
            continue;
        }

		//////////////////////////////////////////////////////////////////////////
		//{{ 2008. 5. 27  최육사  잘못된 카테고리에 대한 복구 처리		
		if( VerifyInventoryCategory( kDB.second.m_kItemInfo.m_iItemID, kDB.second.m_cSlotCategory ) == false )
		{
			// DB에 있는 정보 초기화 시키기
			//{{ 2010. 8. 3	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
			mapWrongPosItem.insert( std::make_pair( kDB.first, KItemPosition( 0, 0 ) ) );
//#else
//			mapWrongPosItem.insert( std::make_pair( kDB.first, std::make_pair( 0, 0 ) ) );
//#endif SERV_PET_SYSTEM
			//}}

			// 인벤 위치 새로 잡기
			mapNotInsertedItem.insert( std::make_pair( kDB.first, kDB.second.m_kItemInfo ) );

			START_LOG( cout, L"잘못된 카테고리에 들어간 아이템 위치 보정 조치" )
				<< BUILD_LOG( m_pUser->GetCharName() )
				<< BUILD_LOG( m_pUser->GetCharUID() )
				<< BUILD_LOG( kDB.first )
				<< BUILD_LOG( kDB.second.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kDB.second.m_cSlotCategory )
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				<< BUILD_LOG( kDB.second.m_sSlotID );
//#else
//				<< BUILD_LOGc( kDB.second.m_cSlotID );
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
			continue;
		}
		//}}
		//////////////////////////////////////////////////////////////////////////

		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		if( VerifyCategorySlotID( kDB.second.m_cSlotCategory, kDB.second.m_sSlotID ) )
//#else
//		if( VerifyCategorySlotID( kDB.second.m_cSlotCategory, kDB.second.m_cSlotID ) )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
        {
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			if( IsEmptySlot( kDB.second.m_cSlotCategory, kDB.second.m_sSlotID ) )
//#else
//			if( IsEmptySlot( kDB.second.m_cSlotCategory, kDB.second.m_cSlotID ) )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
            {
                KInventoryItem kItem;
                kItem.Init( kDB.second, false );
                m_mapItem.insert( std::make_pair( kDB.first, kItem ) );
                m_vecInventorySlot[kItem.GetInventoryCategory()][kItem.GetSlotID()] = kItem.GetItemUID();

                continue;
            }
#ifdef	SERV_WRONG_POSITION_ITEM_FIX
			else
			{
//#ifdef SERV_PET_SYSTEM
				mapWrongPosItem.insert( std::make_pair( kDB.first, KItemPosition( 0, 0 ) ) );
//#else
//				mapWrongPosItem.insert( std::make_pair( kDB.first, std::make_pair( 0, 0 ) ) );
//#endif SERV_PET_SYSTEM

				// 인벤 위치 새로 잡기
				mapNotInsertedItem.insert( std::make_pair( kDB.first, kDB.second.m_kItemInfo ) );

				START_LOG( cout, L"슬롯 중복 아이템 위치 보정" )
					<< BUILD_LOG( m_pUser->GetCharName() )
					<< BUILD_LOG( m_pUser->GetCharUID() )
					<< BUILD_LOG( kDB.first )
					<< BUILD_LOG( kDB.second.m_kItemInfo.m_iItemID )
					<< BUILD_LOGc( kDB.second.m_cSlotCategory )
					<< BUILD_LOG( kDB.second.m_sSlotID );
				continue;
			}
#endif	SERV_WRONG_POSITION_ITEM_FIX
        }
        else
        {
            START_LOG( cerr, L"인벤토리 슬롯 위치 이상." )
				<< BUILD_LOG( m_pUser->GetCharName() )
				<< BUILD_LOG( m_pUser->GetCharUID() )
				<< BUILD_LOG( kDB.second.m_kItemInfo.m_iItemID )
                << BUILD_LOGc( kDB.second.m_cSlotCategory )
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				<< BUILD_LOG( kDB.second.m_sSlotID )
//#else
//				<< BUILD_LOGc( kDB.second.m_cSlotID )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
                << END_LOG;
        }
    }

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	// PC방 유저라면 PC방 전용 아이템을 넣어주자!
	if( m_pUser->IsPcBang() == true  &&  bIsChannelChange == false )
	{
		std::vector< KInventoryItemInfo > vecPcBangPremiumItem;

		// PC방 전용 아이템 획득
#ifdef SERV_PC_BANG_TYPE
		SiKGameSysVal()->GetPcBangPremiumItem( m_pUser->GetUnitType(), vecPcBangPremiumItem, m_pUser->GetPcBangType() );
#else SERV_PC_BANG_TYPE
		SiKGameSysVal()->GetPcBangPremiumItem( m_pUser->GetUnitType(), vecPcBangPremiumItem );
#endif 

		BOOST_TEST_FOREACH( const KInventoryItemInfo&, kPcBangItem, vecPcBangPremiumItem )
		{
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			if( VerifyCategorySlotID( kPcBangItem.m_cSlotCategory, kPcBangItem.m_sSlotID ) == false )
//#else
//			if( VerifyCategorySlotID( kPcBangItem.m_cSlotCategory, kPcBangItem.m_cSlotID ) == false )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
			{
				START_LOG( cerr, L"PC방 아이템을 넣으려는데 슬롯 위치가 이상하다? 있을수없는에러!" )
					<< BUILD_LOG( m_pUser->GetCharName() )
					<< BUILD_LOG( m_pUser->GetCharUID() )
					<< BUILD_LOG( kPcBangItem.m_kItemInfo.m_iItemID )
					<< BUILD_LOGc( kPcBangItem.m_cSlotCategory )
					//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					<< BUILD_LOG( kPcBangItem.m_sSlotID )
//#else
//					<< BUILD_LOGc( kPcBangItem.m_cSlotID )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
					<< END_LOG;
				continue;
			}

			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			if( IsEmptySlot( kPcBangItem.m_cSlotCategory, kPcBangItem.m_sSlotID ) == false )
//#else
//			if( IsEmptySlot( kPcBangItem.m_cSlotCategory, kPcBangItem.m_cSlotID ) == false )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
			{
				KInventoryItemInfo kCheckInfo;
				LIF( GetInventoryItemInfo( kPcBangItem.m_cSlotCategory, kPcBangItem.m_sSlotID, kCheckInfo ) );

				START_LOG( cerr, L"PC방 아이템을 넣으려는데 슬롯이 비어있지 않다? 있을수 없는에러!" )
					<< BUILD_LOG( m_pUser->GetCharName() )
					<< BUILD_LOG( m_pUser->GetCharUID() )
					<< BUILD_LOG( kPcBangItem.m_kItemInfo.m_iItemID )
					<< BUILD_LOGc( kPcBangItem.m_cSlotCategory )
					//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					<< BUILD_LOG( kPcBangItem.m_sSlotID )
//#else
//					<< BUILD_LOGc( kPcBangItem.m_cSlotID )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
					<< BUILD_LOG( kCheckInfo.m_iItemUID )
					<< BUILD_LOG( kCheckInfo.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( kCheckInfo.m_kItemInfo.m_iQuantity )
					<< END_LOG;
				continue;
			}

			KInventoryItem kItem;
			kItem.Init( kPcBangItem, false );
			kItem.NotDBUpdateItem(); // DB에 업데이트 하지 않는 아이템으로 세팅
			m_mapItem.insert( std::make_pair( kPcBangItem.m_iItemUID, kItem ) );
			m_vecInventorySlot[kItem.GetInventoryCategory()][kItem.GetSlotID()] = kItem.GetItemUID();
		}

		START_LOG( clog2, L"PC방 전용 아이템 획득!" )
			<< BUILD_LOG( vecPcBangPremiumItem.size() );
	}

#endif SERV_PC_BANG_PRE
	//}}
	//////////////////////////////////////////////////////////////////////////

	std::vector< KInventoryItemInfo > vecInserted;
    return InsertItem( mapNotInsertedItem, vecInserted );
}

//{{ 2010. 01. 04  최육사	인벤토리 기능개선
void KInventory::GetSelectUnitInventoryInfo( OUT std::vector< KEGS_SELECT_UNIT_INVENTORY_INFO_NOT >& vecInventoryInfo )
{
	vecInventoryInfo.clear();

	for( int iCategory = GetFirstCategory(); iCategory < GetSentinelCategory(); ++iCategory )
	{
		// 은행과 장착 장비는 제외
		if( iCategory == CXSLInventory::ST_BANK  ||  iCategory == CXSLInventory::ST_E_EQUIP )
			continue;

		KEGS_SELECT_UNIT_INVENTORY_INFO_NOT kNot;
		kNot.m_iSlotCategory = iCategory;
		GetInventoryInfo( iCategory, kNot.m_mapItem );

		if( kNot.m_mapItem.empty() )
			continue;

        vecInventoryInfo.push_back( kNot );
	}
}

void KInventory::GetInventorySlotSize( OUT std::map< int, int >& mapInventorySlotSize )
{
	mapInventorySlotSize.clear();

	for( int i = GetFirstCategory(); i < GetSentinelCategory(); ++i )
	{
		mapInventorySlotSize.insert( std::make_pair( i, GetSlotSize( i ) ) );
	}
}
//}}

//{{ 2009. 8. 7  최육사		은행 정보 얻기
void KInventory::GetInventoryInfo( IN const int iCategory, OUT std::map< UidType, KInventoryItemInfo >& mapInventoryItem, IN const bool bForce /*= false*/ )
{
	if( iCategory < GetFirstCategory()  ||  iCategory >= GetSentinelCategory() )
	{
		START_LOG( cerr, L"카테고리 이상." )
			<< BUILD_LOG( iCategory )
			<< END_LOG;

		return;
	}

	// 강제로 얻는게 아니라면?
	if( bForce == false )
	{
		// 인벤정보는 한번만 준다!
		if( m_setGetMyInventory.find( iCategory ) != m_setGetMyInventory.end() )
			return;

		m_setGetMyInventory.insert( iCategory );
	}

	std::map< UidType, KInventoryItem >::iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		KInventoryItemInfo kInventoryItemInfo;
		mit->second.GetInventoryItemInfo( kInventoryItemInfo );

		if( kInventoryItemInfo.m_cSlotCategory != iCategory )
			continue;
		
		mapInventoryItem.insert( std::make_pair( mit->first, kInventoryItemInfo ) );
	}
}

void KInventory::GetInventoryInfo( IN const int iCategory, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo, IN const bool bForce /*= false*/ )
{
	if( iCategory < GetFirstCategory()  ||  iCategory >= GetSentinelCategory() )
	{
		START_LOG( cerr, L"카테고리 이상." )
			<< BUILD_LOG( iCategory )
			<< END_LOG;

		return;
	}

	// 강제로 얻는게 아니라면?
	if( bForce == false )
	{
		// 인벤정보는 한번만 준다!
		if( m_setGetMyInventory.find( iCategory ) != m_setGetMyInventory.end() )
			return;

		m_setGetMyInventory.insert( iCategory );
	}

	std::map< UidType, KInventoryItem >::iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		KInventoryItemInfo kInventoryItemInfo;
		mit->second.GetInventoryItemInfo( kInventoryItemInfo );

		if( kInventoryItemInfo.m_cSlotCategory != iCategory )
			continue;

		vecInventorySlotInfo.push_back( kInventoryItemInfo );
	}
}
//}}

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
void KInventory::GetItemIDSet( OUT std::set< int >& setItemID, bool bOnlyInventory /*= false*/ )
{
	std::map< UidType, KInventoryItem >::const_iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		if( bOnlyInventory == true && CXSLInventory::IsUserInventoryCategory( mit->second.GetInventoryCategory() ) == false )
			continue;

		setItemID.insert( mit->second.GetItemID() );
	}
}
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT

int KInventory::GetBaseSlotSize( IN const int iCategory ) const
{
    if( iCategory < GetFirstCategory() || iCategory >= GetSentinelCategory() )
    {
        START_LOG( cerr, L"잘못된 카테고리." )
            << BUILD_LOG( iCategory )
            << END_LOG;

        return 0;
    }

    switch( iCategory )
    {
    case CXSLInventory::ST_NONE:
        return 0;
		//{{ 2009. 8. 3  최육사		은행
	case CXSLInventory::ST_BANK:
		return 8;
		//}}
#ifdef	SERV_SHARING_BANK_TEST
	case CXSLInventory::ST_SHARE_BANK:
		return 48;
#endif	SERV_SHARING_BANK_TEST
    case CXSLInventory::ST_E_EQUIP:
        return CXSLUnit::NESI_END;
    case CXSLInventory::ST_E_SKILL:
        return CXSLUnit::NSSI_END;
    case CXSLInventory::ST_E_QUICK_SLOT:
		return 3;   
	case CXSLInventory::ST_AVARTA:
		//{{ 2012. 12. 14	허상형 ( Merged by 박세훈 )	인벤토리 개편 테스트
#ifdef SERV_REFORM_INVENTORY_TEST
#ifdef SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
		return 224;
#else SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
		return 168;
#endif SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
#else
		return 160;
#endif SERV_REFORM_INVENTORY_TEST
		//}}
	case CXSLInventory::ST_SPECIAL:
#ifdef SERV_REFORM_INVENTORY_INT
		// 해외팀 각국가 전용 디파인
#if defined( SERV_COUNTRY_TWHK ) || defined( SERV_COUNTRY_CN ) || defined( SERV_COUNTRY_TH ) || defined( SERV_COUNTRY_US ) || defined( SERV_COUNTRY_PH ) || defined( SERV_COUNTRY_IN )
		return 24;
#elif defined( SERV_COUNTRY_JP ) || defined( SERV_COUNTRY_EU ) || defined( SERV_COUNTRY_ID ) || defined( SERV_COUNTRY_BR )
		return 32;
#endif
#else // SERV_REFORM_INVENTORY_INT
		return 32;
#endif // SERV_REFORM_INVENTORY_INT
		//{{ 2010. 8. 6	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	case CXSLInventory::ST_PET:
		return 0; // 펫의 기본 슬롯 갯수는 0개임
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	case CXSLInventory::ST_E_DEFENCE_QUICK_SLOT:
		return 4;
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
    default:
#ifdef SERV_COUNTRY_TWHK
		return 16;
#else
        return 24;
#endif
    }
}

int KInventory::GetMaxSlotSize( IN const int iCategory ) const
{
    if( iCategory < GetFirstCategory() || iCategory >= GetSentinelCategory() )
    {
        START_LOG( cerr, L"잘못된 카테고리." )
            << BUILD_LOG( iCategory )
            << END_LOG;

        return 0;
    }

    switch( iCategory )
    {
    case CXSLInventory::ST_NONE:
        return 0;
		//{{ 2009. 8. 3  최육사		은행
	case CXSLInventory::ST_BANK:
#ifdef	SERV_SHARING_BANK_TEST
	case CXSLInventory::ST_SHARE_BANK:
#endif	SERV_SHARING_BANK_TEST
		return 48;
		//}}
    case CXSLInventory::ST_E_EQUIP:
        return CXSLUnit::NESI_END;
    case CXSLInventory::ST_E_SKILL:
        return CXSLUnit::NSSI_END;
    case CXSLInventory::ST_E_QUICK_SLOT:
		//{{ 2011. 12. 13  김민성	퀵 슬롯 확장 아이템
#ifdef SERV_EXPAND_QUICK_SLOT
		//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		return 10;
#else
		return 6;
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}		
#else
		return 3;
#endif SERV_EXPAND_QUICK_SLOT
		//}}
    case CXSLInventory::ST_AVARTA:
		//{{ 2012. 12. 14	허상형 ( Merged by 박세훈 )	인벤토리 개편 테스트
#ifdef SERV_REFORM_INVENTORY_TEST
#ifdef SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
		return 224;
#else SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
		return 168;
#endif SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
#else
        return 160;
#endif SERV_REFORM_INVENTORY_TEST
		//}}
	case CXSLInventory::ST_SPECIAL:
#ifdef	SERV_REFORM_INVENTORY_TEST
#ifdef SERV_REFORM_INVENTORY_INT
		return 56;
#else // SERV_REFORM_INVENTORY_INT
		return 64;
#endif // SERV_REFORM_INVENTORY_INT
#else
		return 40;
#endif
		//{{ 2010. 8. 6	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	case CXSLInventory::ST_PET:
		return 16; // 펫의 최대 슬롯 갯수는 현재 기획상 16개임
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	case CXSLInventory::ST_E_DEFENCE_QUICK_SLOT:
		return 4;
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
    default:
		//{{ 2012. 12. 14	허상형 ( Merged by 박세훈 )	인벤토리 개편 테스트
#ifdef SERV_REFORM_INVENTORY_TEST
#ifdef SERV_REFORM_INVENTORY_INT
		return 56;
#else // SERV_REFORM_INVENTORY_INT
		return 64;
#endif // SERV_REFORM_INVENTORY_INT
#else
        return 32;
#endif SERV_REFORM_INVENTORY_TEST
		//}}
    }
}

int KInventory::GetSlotSize( IN const int iCategory ) const
{
    if( iCategory < GetFirstCategory() || iCategory >= GetSentinelCategory() )
    {
        START_LOG( cerr, L"잘못된 카테고리." )
            << BUILD_LOG( iCategory )
            << END_LOG;

        return 0;
    }

    return ( int )m_vecInventorySlot[iCategory].size();
}

bool KInventory::IsExist( IN const int iItemID, IN const bool bOnlyInventory ) const
{
    std::map< UidType, KInventoryItem >::const_iterator mit;
    for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
    {
        if( mit->second.GetItemID() == iItemID )
        {
			//{{ 2009. 8. 7  최육사		은행
			if( bOnlyInventory )
			{
				if( CXSLInventory::IsUserInventoryCategory( mit->second.GetInventoryCategory() ) == false )
					continue;
			}
			//}}

            return true;
        }
    }

    return false;
}

bool KInventory::IsExistOnlyInventory( IN const int iItemID ) const
{
	return IsExist( iItemID, true );
}

bool KInventory::IsExistWithBank( IN const int iItemID ) const
{
    return IsExist( iItemID, false );
}

bool KInventory::IsExist( IN const UidType iItemUID ) const
{
    std::map< UidType, KInventoryItem >::const_iterator mit;
    mit = m_mapItem.find( iItemUID );
    return ( mit != m_mapItem.end() );
}

bool KInventory::IsExistInBank( IN const UidType iItemUID ) const
{
	int iCategory = 0;
	int iSlotID = 0;
    if( GetCategorySlot( iItemUID, iCategory, iSlotID ) == false )
	{
		START_LOG( cerr, L"아이템이 존재하지 않습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	return ( iCategory == CXSLInventory::ST_BANK );
}

bool KInventory::IsEquippedItem( IN const int iItemID ) const
{
	std::map< int, KInventoryItemInfo > mapEquippedItem;
	GetEquippedItem( mapEquippedItem );

	std::map< int, KInventoryItemInfo >::const_iterator mit;
	for( mit = mapEquippedItem.begin(); mit != mapEquippedItem.end(); ++mit )
	{
		if( mit->second.m_kItemInfo.m_iItemID == iItemID )
			return true;
	}

	return false;
}

bool KInventory::IsEmptySlot( IN const int iCategory, IN const int iSlotID ) const
{
    _JIF( VerifyCategorySlotID( iCategory, iSlotID ), return false );

    return m_vecInventorySlot[iCategory][iSlotID] <= 0;
}

bool KInventory::IsEnoughItemExist( IN const std::map< int, int >& mapRequiredItem, IN const bool bOnlyInventory ) const
{
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
	return IsEnoughItemExist( mapRequiredItem, bOnlyInventory, false );
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
    std::map< int, int >::const_iterator mit;
    for( mit = mapRequiredItem.begin(); mit != mapRequiredItem.end(); ++mit )
    {
        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
        if( pItemTemplet == NULL )
        {
            START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            return false;
        }

        int iUsageType = pItemTemplet->m_PeriodType;
        int iCount = 0;

        bool bFlag = false;
        int i, j;		
        for( i = GetFirstCategory(); i < GetSentinelCategory(); ++i )
        {
			//{{ 2009. 8. 7  최육사		은행
			if( bOnlyInventory )
			{
				if( CXSLInventory::IsUserInventoryCategory( i ) == false )
					continue;
			}
			//}}

            for( j = 0; j < GetSlotSize( i ); j++ )
            {
                if( GetItemID( i, j ) == mit->first )
                {
                    switch( iUsageType )
                    {
                    case CXSLItem::PT_INFINITY:
                    case CXSLItem::PT_ENDURANCE:
                        iCount += 1;
                        break;
                    case CXSLItem::PT_QUANTITY:
						int iQuantity;
						if( GetQuantity( i, j, iQuantity ) )
						{
							iCount += iQuantity;
						}
                        break;
                    default:
                        START_LOG( cerr, L"아이템 사용 타입이 이상함." )
                            << BUILD_LOG( mit->first )
                            << BUILD_LOG( iUsageType )
                            << END_LOG;
                        break;
                    }

                    if( iCount >= mit->second )
                    {
                        bFlag = true;
                    }
                }
            }
        }

        if( !bFlag )
        {
            return false;
        }
    }

    return true;
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
}

#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-06-25	// 박세훈
bool KInventory::IsEnoughItemExist( IN const std::map< int, int >& mapRequiredItem
								  , IN const bool bOnlyInventory
								  , IN const bool bOnlyInfinite	// 기한 제한이 없는 것
								  ) const
{
	std::map< int, int >::const_iterator it;
	for( it = mapRequiredItem.begin(); it != mapRequiredItem.end(); ++it )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( it->first );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( it->first )
				<< END_LOG;
			return false;
		}

		int iUsageType = pItemTemplet->m_PeriodType;
		int iCount = 0;

		bool bFlag = false;
		int i, j;		
		for( i = GetFirstCategory(); i < GetSentinelCategory(); ++i )
		{
			//{{ 2009. 8. 7  최육사		은행
			if( bOnlyInventory )
			{
				if( CXSLInventory::IsUserInventoryCategory( i ) == false )
					continue;
			}
			//}}

			for( j = 0; j < GetSlotSize( i ); j++ )
			{
				KItemInfo kItemInfo;
				if( GetItemInfo( i, j, kItemInfo ) == false )
				{
					continue;
				}

				if( kItemInfo.m_iItemID != it->first )
				{
					continue;
				}

				if( ( bOnlyInfinite == true ) && ( kItemInfo.m_sPeriod != 0 ) )
					continue;

				switch( iUsageType )
				{
				case CXSLItem::PT_INFINITY:
				case CXSLItem::PT_ENDURANCE:
					iCount += 1;
					break;
				case CXSLItem::PT_QUANTITY:
					int iQuantity;
					if( GetQuantity( i, j, iQuantity ) )
					{
						iCount += iQuantity;
					}
					break;
				default:
					START_LOG( cerr, L"아이템 사용 타입이 이상함." )
						<< BUILD_LOG( it->first )
						<< BUILD_LOG( iUsageType )
						<< END_LOG;
					break;
				}

				if( iCount >= it->second )
				{
					bFlag = true;
				}
			}
		}

		if( !bFlag )
		{
			return false;
		}
	}

	return true;
}
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX

bool KInventory::IsEnoughItemExistOnlyInventory( IN const std::map< int, int >& mapRequiredItem )
{
    return IsEnoughItemExist( mapRequiredItem, true );
}

bool KInventory::IsEnoughItemExist( IN const std::map< UidType, int >& mapRequiredItem_ )
{
    std::map< UidType, KInventoryItem >::iterator mit;
    std::map< UidType, int >::const_iterator mitRequired;

    for( mitRequired = mapRequiredItem_.begin(); mitRequired != mapRequiredItem_.end(); ++mitRequired )
    {
        int iQuantity;
        _JIF( GetQuantity( mitRequired->first, iQuantity ), return false );

        if( iQuantity < mitRequired->second )
        {
            return false;
        }
    }

    return true;
}

bool KInventory::IsSuitableSlot( int iItemID, int iCategory, int iSlotID )
{
    _JIF( VerifyCategorySlotID( iCategory, iSlotID ), return false );

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
    if( !pItemTemplet )
    {
        START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return false;
    }
#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
	// 카테코리가 펫, 은행, 은행 공유이고 ItemID  엘의 여인의 눈물
	if(pItemTemplet->m_ItemID == CXSLItem::EI_TEAR_OF_ELWOMAN &&
		( iCategory == CXSLInventory::ST_PET || iCategory == CXSLInventory::ST_BANK || iCategory == CXSLInventory::ST_SHARE_BANK) )
	{
		START_LOG( clog, L"엘의 여인의 눈물 아이템은 펫이나 은행, 은행공유 인벤토리로 넣을 수 없음." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iCategory )
			<< END_LOG;

		return false;
	}
#endif SERV_EVENT_TEAR_OF_ELWOMAN
	//{{ 2010. 01. 05  최육사	PC방 프리미엄
	// PC방이 인벤토리 슬롯 검사의 우선순위가 높다
#ifdef SERV_PC_BANG_PRE
	if( pItemTemplet->m_bIsPcBang == true )
	{
		if( iCategory != CXSLInventory::ST_E_EQUIP  &&
			iCategory != CXSLInventory::ST_PC_BANG )
		{
            return false;
		}
	}
#endif SERV_PC_BANG_PRE
	//}}

    switch( iCategory )
    {
    case CXSLInventory::ST_EQUIP:
        if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON  ||
            pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
        {
			//{{ 2010. 9. 16	최육사	아바타이면서 무기/방어구의 경우
			if( pItemTemplet->m_bFashion == true )
			{
				return false;
			}
			//}}

            return true;
        }
        else
        {
            return false;
        }

    case CXSLInventory::ST_ACCESSORY:
        if( pItemTemplet->m_ItemType == CXSLItem::IT_ACCESSORY )
        {
			//{{ 2010. 9. 16	최육사	아바타이면서 악세사리의 경우
			if( pItemTemplet->m_bFashion == true )
			{
				return false;
			}
			//}}

            return true;
        }
        else
        {
            return false;
        }

    case CXSLInventory::ST_MATERIAL:
        if( pItemTemplet->m_ItemType == CXSLItem::IT_MATERIAL )
        {
            return true;
        }
        else
        {
            return false;
        }

    case CXSLInventory::ST_SPECIAL:
#ifdef SERV_SKILL_NOTE
		if( pItemTemplet->m_ItemType == CXSLItem::IT_SPECIAL ||
			pItemTemplet->m_ItemType == CXSLItem::IT_SKILL_MEMO )
#else
#pragma ELSE_MESSAGE
        //if( pItemTemplet->m_ItemType == CXSLItem::IT_SPECIAL )
#endif
        {
            return true;
        }
        else
        {
            return false;
        }

    case CXSLInventory::ST_QUEST:
        if( pItemTemplet->m_ItemType == CXSLItem::IT_QUEST )
        {
            return true;
        }
        else
        {
            return false;
        }

    case CXSLInventory::ST_QUICK_SLOT:
        if( pItemTemplet->m_ItemType == CXSLItem::IT_QICK_SLOT )
        {
            return true;
        }
        else
        {
            return false;
        }

    case CXSLInventory::ST_AVARTA:
        return pItemTemplet->m_bFashion; // 패션아이템만 이동가능.

		//{{ 2009. 8. 3  최육사		은행
    case CXSLInventory::ST_BANK:
        return true; // 모든 종류의 아이템이 이동가능.
		//}}

#ifdef	SERV_SHARING_BANK_TEST
	case CXSLInventory::ST_SHARE_BANK:
		return true; // 모든 종류의 아이템이 이동가능.
#endif	SERV_SHARING_BANK_TEST

		//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	case CXSLInventory::ST_PET:
		return true; // 모든 종류의 아이템이 이동가능.
#endif SERV_PET_SYSTEM
		//}}

    case CXSLInventory::ST_E_EQUIP:
        switch( iSlotID )
        {
        case CXSLUnit::NESI_DEFENCE_HAIR_FASHION:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_HAIR && pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_DEFENCE_BODY:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_BODY && !pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_DEFENCE_BODY_FASHION:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_BODY && pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_DEFENCE_LEG:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_LEG && !pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_DEFENCE_LEG_FASHION:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_LEG && pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_DEFENCE_HAND:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_HAND && !pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_DEFENCE_HAND_FASHION:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_HAND && pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_DEFENCE_FOOT:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_FOOT && !pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_DEFENCE_FOOT_FASHION:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_DEFENCE_FOOT && pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_WEAPON_HAND:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_WEAPON_HAND && !pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
        case CXSLUnit::NESI_WEAPON_HAND_FASHION:
            if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_WEAPON_HAND && pItemTemplet->m_bFashion )
            {
                return true;
            }
            else
            {
                return false;
            }
		case CXSLUnit::NESI_AC_FACE1:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_FACE1 /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}
		case CXSLUnit::NESI_AC_FACE2:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_FACE2 /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}
		case CXSLUnit::NESI_AC_FACE3:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_FACE3 /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}
		case CXSLUnit::NESI_AC_BODY:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_BODY /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}
		case CXSLUnit::NESI_AC_ARM:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_ARM /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}
		case CXSLUnit::NESI_AC_LEG:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_LEG /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}
		case CXSLUnit::NESI_AC_RING:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_RING /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}
		case CXSLUnit::NESI_AC_NECKLESS:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_NECKLESS /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}
		case CXSLUnit::NESI_AC_WEAPON:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_AC_WEAPON /*&& !pItemTemplet->m_bFashion*/ )
			{
				return true;
			}
			else
			{
				return false;
			}

#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		case CXSLUnit::NESI_ONEPIECE_FASHION:
			if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_ONEPIECE_FASHION && pItemTemplet->m_bFashion )
			{
				return true;
			}
			else
			{
				return false;
			}
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
        default:
            START_LOG( cerr, L"이상한 슬롯." )
                << BUILD_LOG( iCategory )
                << BUILD_LOG( iSlotID )
                << END_LOG;
            return false;
        }

    case CXSLInventory::ST_E_SKILL: // 스킬슬롯은 따로 있음
        return false;

    case CXSLInventory::ST_E_QUICK_SLOT:
        if( pItemTemplet->m_EqipPosition == CXSLUnit::EP_QUICK_SLOT )
        {
            return true;
        }
        else
        {
            return false;
        }

		//{{ 2010. 01. 05  최육사	PC방 프리미엄
	case CXSLInventory::ST_PC_BANG:
		return pItemTemplet->m_bIsPcBang; // PC방 전용 아이템만 PC방 카테고리에 이동해야한다!
		//}}

    default:
        START_LOG( cerr, L"이상한 슬롯 카테고리." )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlotID )
            << END_LOG;
        return false;
    }
}

bool KInventory::IsAbleToEquip( IN int iItemID )
{
    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return false;
    }

    if( !CompareLevel( iItemID ) )
    {
        return false;
    }

    if( !CompareUnitClass( iItemID ) )
    {
        return false;
    }

	//{{ 2009. 7. 28  최육사	장착불가 장비아이템
	if( pItemTemplet->m_bNoEquip == true )
	{
		START_LOG( cerr, L"클라이언트에서 장착 못한다고 체크 했을텐데..?" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

        return false;
	}
	//}}

    // TODO : 클래스, 무기 타입에 관한 검사를 해야 한다.

    return true;
}

//////////////////////////////////////////////////////////////////////////

// 은행, 장착슬롯, 퀵슬롯은 제외한 빈공간 검사입니다.

//{{ 2012. 07. 4	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
bool KInventory::IsEnoughSpaceExist( IN const std::map< int, int >& mapItem )
{
	// 원본 아이템의 수량에 변화를 주지 않기 위해 복사본을 준비합니다.
	std::map< int, int > mapNewItem;
	std::copy( mapItem.begin(), mapItem.end(), std::inserter( mapNewItem, mapNewItem.begin() ) );

	// 임시로 채우기를 하기 위한 카테고리 슬롯 정보의 복사본입니다.
	std::map< UidType, KInventoryItem >	mapItemCopy;
	std::vector< UidType >				vecInventorySlotCopy[CXSLInventory::ST_END];

	for( int i=0; i < CXSLInventory::ST_END; ++i )
	{
		vecInventorySlotCopy[i].resize( m_vecInventorySlot[i].size() );
		std::copy( m_vecInventorySlot[i].begin(), m_vecInventorySlot[i].end(), vecInventorySlotCopy[i].begin() );
	}
	std::copy( m_mapItem.begin(), m_mapItem.end(), std::inserter( mapItemCopy, mapItemCopy.begin() ) );

	// 각 아이템에 대하여, 수용 가능성을 검사합니다.
	std::map< int, int >::iterator mit;
	for( mit = mapNewItem.begin(); mit != mapNewItem.end(); ++mit )
	{
		// 식별하기 쉬운 이름으로 변수를 준비합니다.
		const int& iItemID = mit->first;
		int& iQuantity = mit->second;

		const CXSLItem::ItemTemplet* pItemTemplate = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplate == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			return false;
		}

		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( iItemID, vecCategoryID );

		// 카테고리를 순회하면서 수량을 채워넣으며, 또한 빈 슬롯에 아이템을 채워봅니다.
		BOOST_TEST_FOREACH( const int&, iCategory, vecCategoryID )
		{
			// 유효한 카테고리입니까?
			if( IsValidCategory( iCategory ) == false )
			{
				START_LOG( cerr, L"카테고리 설정이 이상합니다." )
					<< BUILD_LOG( iCategory )
					<< BUILD_LOG( iItemID )
					<< END_LOG;
				return false;
			}

			if( pItemTemplate->m_PeriodType == CXSLItem::PT_QUANTITY )
			{
				int iNumIncreased = 0;

				// 복사본에 채워넣기
				if( FillTheUnFullSlot( iCategory, iItemID, iQuantity, vecInventorySlotCopy, mapItemCopy, NULL, NULL, NULL, iNumIncreased ) == false )
				{
					START_LOG( cerr, L"FillTheUnFullSlot 실패")
						<< BUILD_LOG( iItemID )
						<< BUILD_LOG( iQuantity )
						<< END_LOG;
					return false;
				}
				iQuantity -= iNumIncreased;

				// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
				if( iQuantity <= 0 )
				{
					break;
				}
			}

			// 남은 수량이 있다면, 현재 카테고리의 빈 슬롯에 먼저 채워넣기를 시도해본다. ( 복사본에 작업한다. )
			int iNumInserted = 0;
			if( InsertToEmptySlotForTest( iCategory, iItemID, iQuantity, NULL, NULL, NULL, vecInventorySlotCopy, mapItemCopy, NULL, NULL, iNumInserted ) == false )
			{
				START_LOG( cerr, L"InsertToEmptySlotForTest 실패")
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( iQuantity )
					<< END_LOG;
				return false;
			}
			iQuantity -= iNumInserted;

			// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
			if( iQuantity <= 0 )
			{
				break;
			}
		}

		// 해당 아이템의 수량을 관련된 모든 카테고리의 수용량으로 수용할 수 없다면, 실패 처리를 합니다.
		if( 0 < iQuantity )
		{
			return false;
		}
	}

	// 모든 아이템에 대하여 수용이 가능하다는 것을 알았습니다. 성공 처리를 합니다.
	return true;
}
#else
bool KInventory::IsEnoughSpaceExist( IN const std::map< int, int >& mapItem ) // itemID, quantity
{
    std::map< int, int >::iterator mit;
    std::map< int, int > mapNewItem = mapItem;

	// 1. 먼저 수량만으로 채울수 있는지 검사
    int iCategory, iSlotIndex;
    for( mit = mapNewItem.begin(); mit != mapNewItem.end(); ++mit )
    {
		// 빈공간을 검사할 아이템 카테고리가 정상값인지 검사하기
        iCategory = GetSuitableCategory( mit->first );
        if( iCategory < CXSLInventory::ST_EQUIP  ||  iCategory > CXSLInventory::ST_AVARTA )
        {
            START_LOG( cerr, L"카테고리 설정이 이상합니다." )
                << BUILD_LOG( iCategory )
                << BUILD_LOG( mit->first )
                << END_LOG;

            return false;
        }

        for( iSlotIndex = 0; iSlotIndex < GetSlotSize( iCategory ); ++iSlotIndex )
        {
			if( IsEmptySlot( iCategory, iSlotIndex ) )
			{
				continue;
			}

			if( GetItemID( iCategory, iSlotIndex ) == mit->first ) // 슬롯에 있는 아이템이 동일한가?
			{
			    mit->second -= std::min< int >( mit->second, GetRemainedQuantityCapacity( iCategory, iSlotIndex ) );
			}
        }
    }

    std::map< int, int >::iterator mit2;
    std::map< int, int > mapNumFreeSlot; // first int = category, second int = slot number

	// 2. 빈슬롯 얻기
	GetNumFreeSlot( mapNumFreeSlot );

	// 3. 빈슬롯에 다 채워지는지 검사
    for( mit = mapNewItem.begin(); mit != mapNewItem.end(); ++mit )
    {
        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
        if( !pItemTemplet )
        {
            START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            return false;
        }

        int iRequiredNumSlot = 0;
        if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
        {
            if( mit->second > 0 )
            {
                if( pItemTemplet->m_Quantity <= 0 )
                {
                    START_LOG( cerr, L"수량 제한 값이 이상함." )
                        << BUILD_LOG( pItemTemplet->m_Quantity )
                        << END_LOG;

                    return false;
                }
                iRequiredNumSlot = mit->second / pItemTemplet->m_Quantity; // item quantity /  quantity unit 1묶음 수량
                if( ( mit->second % pItemTemplet->m_Quantity ) > 0 ) // 나누어 떨어지는 경우와 아닌 경우의 필요 슬롯 수를 구하기 위한 공식으로 보인다
                {
                    iRequiredNumSlot++;
                }
            }
        }
        else // 수량 아이템이 아닌 경우 
        {
            _JIF( mit->second >= 0, return false );
            iRequiredNumSlot = mit->second;
        }

        int iSuitableCategory = GetSuitableCategory( mit->first );
        mit2 = mapNumFreeSlot.find( iSuitableCategory );
        if( mit2 == mapNumFreeSlot.end() )
        {
			std::pair< std::map< int,int >::iterator, bool > pr;
			pr = mapNumFreeSlot.insert( std::make_pair( iSuitableCategory, 0 ) );
			LIF( pr.second );
			mit2 = pr.first;
        }

        mit2->second -= iRequiredNumSlot;
        if( mit2->second < 0 )
        {
            return false;
        }
    }

    return true;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}

//{{ 2010. 05. 03  최육사	소켓 개편
#ifdef SERV_EMPTY_SLOT

bool KInventory::IsEnoughEmptySlot( IN int iCategory, IN int iEmptySlotNum )
{
	if( iCategory < GetFirstCategory()  ||  iCategory >= GetSentinelCategory() )
	{
		START_LOG( cerr, L"카테고리 이상." )
			<< BUILD_LOG( iCategory )
			<< END_LOG;

		return false;
	}

	// 빈슬롯 체크
	int iCurEmptySlotNum = 0;

	for( int iSlotID = 0; iSlotID < GetSlotSize( iCategory ); ++iSlotID )
	{
		if( IsEmptySlot( iCategory, iSlotID ) )
		{
			++iCurEmptySlotNum;
		}
	}

	// 필요한 빈슬롯 숫자 만큼 현재 비어있는가?
	if( iEmptySlotNum > iCurEmptySlotNum )
	{
		return false;
	}

	return true;
}

#endif SERV_EMPTY_SLOT
//}}

bool KInventory::IsPeriodExpired( IN UidType iItemUID )
{
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	return mit->second.IsPeriodExpired();
}

bool KInventory::IsPeriodExpired( IN int iCategory, IN int iSlotID )
{
	return IsPeriodExpired( GetItemUID( iCategory, iSlotID ) );
}

//{{ 2008. 12. 21  최육사	깨진 아이템
bool KInventory::IsBrokenItem( IN UidType iItemUID )
{
	std::map< UidType, KInventoryItem >::const_iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{		
		return false;
	}

	return mit->second.IsBroken();
}
//}}

//{{ 2009. 8. 7  최육사		은행
CXSLInventory::MEMBERSHIP_PRIVATE_BANK KInventory::GetBankMemberShip()
{
    return (CXSLInventory::MEMBERSHIP_PRIVATE_BANK)GetSlotSize( CXSLInventory::ST_BANK );
}

int KInventory::GetMyNextUpgradeBankItemID()
{
	switch( CXSLInventory::GetNextBankMemberShip( GetBankMemberShip() ) )
	{
	case CXSLInventory::MPB_SILVER:		return CXSLItem::CI_BANK_MEMBERSHIP_SILVER;
	case CXSLInventory::MPB_GOLD:		return CXSLItem::CI_BANK_MEMBERSHIP_GOLD;
	case CXSLInventory::MPB_EMERALD:	return CXSLItem::CI_BANK_MEMBERSHIP_EMERALD;
	case CXSLInventory::MPB_DIAMOND:	return CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND;
	case CXSLInventory::MPB_PLATINUM:	return CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM;
	}
	
	return 0;
}
//}}

//{{ 2009. 8. 27  최육사	봉인
bool KInventory::IsSealedItem( IN UidType iItemUID )
{
	std::map< UidType, KInventoryItem >::const_iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{		
		return false;
	}

	return mit->second.IsSealed();
}
//}}

//{{ 2010. 01. 04  최육사	PC방 프리미엄
bool KInventory::IsDBUpdateItem( IN UidType iItemUID ) const
{
	std::map< UidType, KInventoryItem >::const_iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"아이템이 존재하지 않습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	return mit->second.IsDBUpdate();
}
//}}

#ifdef	SERV_SHARING_BANK_TEST
bool KInventory::IsEmptyShareBankSlot()
{
	std::vector< UidType >::iterator vit;
	UidType iItemUID = 0;

	for( vit = m_vecInventorySlot[CXSLInventory::ST_SHARE_BANK].begin(); vit != m_vecInventorySlot[CXSLInventory::ST_SHARE_BANK].end(); vit++ )
	{
		iItemUID = *vit;
		if( iItemUID > 0 )	//	UID가 0 이상이면
		{
			return false;
		}
	}

	return true;
}

bool KInventory::IsEmptyShareItem()
{
	return m_setShareItem.empty();
}
#endif	SERV_SHARING_BANK_TEST

bool KInventory::GetCategorySlot( IN const UidType iItemUID, OUT int& iCategory, OUT int& iSlotID ) const
{
    std::map< UidType, KInventoryItem >::const_iterator mit;
    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        return false;
    }

    iCategory = mit->second.GetInventoryCategory();
    iSlotID = mit->second.GetSlotID();

    _JIF( GetItemUID( iCategory, iSlotID ) == iItemUID, return false );
    return true;
}

UidType KInventory::GetItemUID( IN const int iCategory, IN const int iSlotID ) const
{
    _JIF( VerifyCategorySlotID( iCategory, iSlotID ), return 0 );

    return m_vecInventorySlot[iCategory][iSlotID];
}

int KInventory::GetItemID( IN const UidType iItemUID ) const
{
    std::map< UidType, KInventoryItem >::const_iterator mit;

    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        return 0;
    }

    return mit->second.GetItemID();
}

int KInventory::GetItemID( IN const int iCategory, IN const int iSlotID ) const
{
    return GetItemID( GetItemUID( iCategory, iSlotID ) );
}

bool KInventory::GetUsageType( IN const UidType iItemUID, OUT int& iUsageType ) const
{
    std::map< UidType, KInventoryItem >::const_iterator mit;
    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        return false;
    }

    iUsageType = mit->second.GetUsageType();
    return true;
}

bool KInventory::GetUsageType( IN const int iCategory, IN const int iSlotID, OUT int& iUsageType ) const
{
    return GetUsageType( GetItemUID( iCategory, iSlotID ), iUsageType );
}

bool KInventory::GetQuantity( IN const int iItemID, OUT int& iQuantity ) const
{
	iQuantity = 0;

	std::map< UidType, KInventoryItem >::const_iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		if( mit->second.GetItemID() == iItemID )
		{
			iQuantity += mit->second.GetQuantity();
		}
	}

	if( iQuantity > 0 )
		return true;

	return false;
}

bool KInventory::GetQuantity( IN const UidType iItemUID, OUT int& iQuantity ) const
{
    std::map< UidType, KInventoryItem >::const_iterator mit;
    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        return false;
    }

    iQuantity = mit->second.GetQuantity();
    return true;
}

bool KInventory::GetQuantity( IN const int iCategory, IN const int iSlotID, OUT int& iQuantity ) const
{
    return GetQuantity( GetItemUID( iCategory, iSlotID ), iQuantity );
}

bool KInventory::GetEndurance( IN const UidType iItemUID, OUT int& iEndurance ) const
{
	std::map< UidType, KInventoryItem >::const_iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	iEndurance = mit->second.GetEndurance();
    return true;
}

bool KInventory::GetEndurance( IN const int iCategory, IN const int iSlotID, OUT int& iEndurance ) const
{
	return GetEndurance( GetItemUID( iCategory, iSlotID ), iEndurance );
}

bool KInventory::GetEnchantLevel( UidType iItemUID, int& iEnchantLevel )
{
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	iEnchantLevel = mit->second.GetEnchantLevel();
	return true;
}

bool KInventory::GetEnchantLevel( int iCategory, int iSlotID, int& iEnchantLevel )
{
	return GetEnchantLevel( GetItemUID( iCategory, iSlotID ), iEnchantLevel );
}

bool KInventory::GetAttribEnchant( IN UidType iItemUID, OUT KItemAttributeEnchantInfo& kInfo )
{
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	mit->second.GetAttribEnchant( kInfo );
	return true;
}

bool KInventory::GetAttribEnchant( IN int iCategory, IN int iSlotID, OUT KItemAttributeEnchantInfo& kInfo )
{
	return GetAttribEnchant( GetItemUID( iCategory, iSlotID ), kInfo );
}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventory::GetItemSocketList( UidType iItemUID, std::vector< int >& vecItemSocket )
#else
bool KInventory::GetItemSocketList( UidType iItemUID, std::vector< short >& vecItemSocket )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	vecItemSocket.clear();
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	vecItemSocket = mit->second.GetItemSocketList();
	return true;
}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventory::GetItemSocketList( int iCategory, int iSlotID, std::vector< int >& vecItemSocket )
#else
bool KInventory::GetItemSocketList( int iCategory, int iSlotID, std::vector< short >& vecItemSocket )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	return GetItemSocketList( GetItemUID( iCategory, iSlotID ), vecItemSocket );
}

//{{ 2010. 04. 15  최육사	소켓 개편
#ifdef SERV_SOCKET_NEW

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventory::GetItemSocketList( IN UidType iItemUID, OUT std::map< int, int >& mapItemSocket )
#else
bool KInventory::GetItemSocketList( IN UidType iItemUID, OUT std::map< int, short >& mapItemSocket )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	mapItemSocket.clear();
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	const std::vector< int >& vecItemSocket = mit->second.GetItemSocketList();
#else
	const std::vector< short >& vecItemSocket = mit->second.GetItemSocketList();
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	for( u_int ui = 0; ui < vecItemSocket.size(); ++ui )
	{
		if( vecItemSocket[ui] > 0 )
		{
			mapItemSocket.insert( std::make_pair( ui, vecItemSocket[ui] ) );
		}
	}
	
	return true;    
}

#endif SERV_SOCKET_NEW
//}}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventory::GetItemSocketOption( UidType iItemUID, const int iSocketIndex, int& iSocketOption )
{	
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	return mit->second.GetItemSocketOption( iSocketIndex, iSocketOption );
}
#else
bool KInventory::GetItemSocketOption( UidType iItemUID, const int iSocketIndex, short& sSocketOption )
{	
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	return mit->second.GetItemSocketOption( iSocketIndex, sSocketOption );
}
#endif SERV_ITEM_OPTION_DATA_SIZE
//}}

bool KInventory::GetCountAssignedItemSocket( UidType iItemUID, int& iAssignedSocketCount )
{	
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	iAssignedSocketCount = mit->second.GetCountAssignedItemSocket();
	return true;
}

bool KInventory::GetItemInfo( UidType iItemUID, KItemInfo& kInfo ) const
{
    std::map< UidType, KInventoryItem >::const_iterator mit;

    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        return false;
    }

    mit->second.GetItemInfo( kInfo );
    return true;
}

bool KInventory::GetItemInfo( int iCategory, int iSlotID, KItemInfo& kInfo ) const
{
    return GetItemInfo( GetItemUID( iCategory, iSlotID ), kInfo );
}

bool KInventory::GetInventoryItemInfo( IN const UidType iItemUID, OUT KInventoryItemInfo& kInventoryItemInfo ) const
{
	std::map< UidType, KInventoryItem >::const_iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

    mit->second.GetInventoryItemInfo( kInventoryItemInfo );
    return true;
}

bool KInventory::GetInventoryItemInfo( IN const int iCategory, IN const int iSlotID, OUT KInventoryItemInfo& kInventoryItemInfo ) const
{
    return GetInventoryItemInfo( GetItemUID( iCategory, iSlotID ), kInventoryItemInfo );
}

bool KInventory::GetInventorySlotInfo( IN const int iCategory, IN const int iSlotID, OUT KInventoryItemInfo& kInventoryItemInfo ) const
{
    if( !VerifyCategorySlotID( iCategory, iSlotID ) )
    {
        START_LOG( cerr, L"인벤토리 슬롯 위치 이상." )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        return false;
    }

    if( IsEmptySlot( iCategory, iSlotID ) )
    {
        kInventoryItemInfo.m_iItemUID = 0;
        kInventoryItemInfo.m_cSlotCategory = static_cast<char>(iCategory);
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		kInventoryItemInfo.m_sSlotID = static_cast<short>(iSlotID);
//#else
//		kInventoryItemInfo.m_cSlotID = static_cast<char>(iSlotID);
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}

        return true;
    }
    else
    {
        return GetInventoryItemInfo( iCategory, iSlotID, kInventoryItemInfo );
    }
}

bool KInventory::GetInventoryItemSimpleInfo( IN const UidType iItemUID, OUT KInventoryItemSimpleInfo& kInventoryItemInfo ) const
{
	std::map< UidType, KInventoryItem >::const_iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	mit->second.GetInventoryItemSimpleInfo( kInventoryItemInfo );
	return true;
}

bool KInventory::GetInventoryItemSimpleInfo( IN const int iCategory, IN const int iSlotID, OUT KInventoryItemSimpleInfo& kInventoryItemInfo ) const
{
	return GetInventoryItemSimpleInfo( GetItemUID( iCategory, iSlotID ), kInventoryItemInfo );
}

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
void KInventory::GetSpecialItem( OUT std::map< int, KInventoryItemInfo >& mapSpecialItem ) const
{
	mapSpecialItem.clear();

	for( int iSlotID = 0; iSlotID < GetSlotSize( CXSLInventory::ST_SPECIAL ); ++iSlotID )
	{
		if( !IsEmptySlot( CXSLInventory::ST_SPECIAL, iSlotID ) )
		{
			KInventoryItemInfo kInfo;
			if( !GetInventoryItemInfo( CXSLInventory::ST_SPECIAL, iSlotID, kInfo ) )
			{
				START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
					<< BUILD_LOG( iSlotID )
					<< END_LOG;

				continue;
			}

			// 키 값이 중복되지 않으므로 중복 체크를 하지 않고 insert 해도 된다.
			mapSpecialItem.insert( std::make_pair( iSlotID, kInfo ) );
		}
	}
}
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

void KInventory::GetEquippedItem( OUT std::map< int, KInventoryItemInfo >& mapEquippedItem ) const
{
    mapEquippedItem.clear();
    
    for( int iSlotID = 0; iSlotID < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++iSlotID )
    {
        if( !IsEmptySlot( CXSLInventory::ST_E_EQUIP, iSlotID ) )
        {
            KInventoryItemInfo kInfo;
            if( !GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, iSlotID, kInfo ) )
            {
                START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
                    << BUILD_LOG( iSlotID )
                    << END_LOG;

                continue;
            }

            // 키 값이 중복되지 않으므로 중복 체크를 하지 않고 insert 해도 된다.
            mapEquippedItem.insert( std::make_pair( iSlotID, kInfo ) );
        }
    }
}

void KInventory::GetEquippedItem( OUT std::vector< KInventoryItemSimpleInfo >& vecEquippedItem ) const
{
	vecEquippedItem.clear();
	
	for( int iSlotID = 0; iSlotID < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++iSlotID )
	{
		if( !IsEmptySlot( CXSLInventory::ST_E_EQUIP, iSlotID ) )
		{
			KInventoryItemSimpleInfo kInfo;
			if( !GetInventoryItemSimpleInfo( CXSLInventory::ST_E_EQUIP, iSlotID, kInfo ) )
			{
				START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
					<< BUILD_LOG( iSlotID )
					<< END_LOG;

				continue;
			}
			
			vecEquippedItem.push_back( kInfo );
		}
	}
}

//{{ 2012. 10. 09	김민성	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
void KInventory::GetEquippedItem( IN std::set< CXSLItem::ITEM_TYPE >& setItemType, IN bool bFashion, OUT std::map< int, KInventoryItemInfo >& mapEquippedItem ) const
{
	mapEquippedItem.clear();

	for( int iSlotID = 0; iSlotID < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++iSlotID )
	{
		if( !IsEmptySlot( CXSLInventory::ST_E_EQUIP, iSlotID ) )
		{
			KInventoryItemInfo kInfo;
			if( !GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, iSlotID, kInfo ) )
			{
				START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
					<< BUILD_LOG( iSlotID )
					<< END_LOG;

				continue;
			}
			
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿 포인트 이상.!" )
					<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
					<< END_LOG;

				continue;
			}

			// 아바타 인지 검사
			if( pItemTemplet->m_bFashion != bFashion )
			{
				continue;
			}

			// 특정 아이템 타입인 아이템만 거른다.
			std::set< CXSLItem::ITEM_TYPE >::iterator sit = setItemType.find( pItemTemplet->m_ItemType );
			if( sit != setItemType.end() )
			{
				// 키 값이 중복되지 않으므로 중복 체크를 하지 않고 insert 해도 된다.
				mapEquippedItem.insert( std::make_pair( iSlotID, kInfo ) );
			}
		}
	}
}
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
//}}

//{{ 2010. 10. 18	최육사	펫 오라 스킬
//////////////////////////////////////////////////////////////////////////
//#ifdef SERV_PET_AURA_SKILL
//////////////////////////////////////////////////////////////////////////

//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
#ifdef SERV_EVENT_VALENTINE_RING_IS_DUNGEON
void KInventory::GetEquippedStat( IN const bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel, IN int iTempDungeonID  )
#else 
void KInventory::GetEquippedStat( IN const bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel )
#endif SERV_EVENT_VALENTINE_RING_IS_DUNGEON
#else
void KInventory::GetEquippedStat( IN const bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat )
#endif SERV_USE_PERCENT_IN_OPTION_DATA
//}} 
{
	kAddStat.Init();

	if( m_pUser == NULL )
	{
		START_LOG( cerr, L"유저 포인터 이상." )
			<< END_LOG;
		return;
	}
	
	std::set< int > setEquippedItemSetID;
	KStat kItemBaseStat;

	for( int i = 0; i < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++i )
	{
		if( IsEmptySlot( CXSLInventory::ST_E_EQUIP, i ) == true )
			continue;

		KInventoryItemInfo kInfo;
		if( GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, i, kInfo ) == false )
		{
			START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
				<< BUILD_LOG( i )
				<< END_LOG;

			continue;
		}

		// 1. 내구도 아이템의 내구도가 0 이하이면 스탯에 추가되지 않는다.
		if( kInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_ENDURANCE )
		{
			if( kInfo.m_kItemInfo.m_sEndurance <= 0 )
			{
				continue;
			}
		}

		// 2. 사용불가 아이템이면 스탯에 추가되지 않는다.
		if( kInfo.m_kItemInfo.m_cEnchantLevel < 0 )
			continue;

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿 포인트 이상.!" )
				<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
				<< END_LOG;

			continue;
		}

		// 3. 유닛 레벨과 장착조건 레벨이 틀리면 장착되지 않는다. - 아이템의 레벨조정으로 인한 옵션기능.
		if( m_pUser->GetLevel() < pItemTemplet->m_UseLevel )
		{
			START_LOG( cwarn, L"아이템 장착조건이 되지 않는데 아이템을 착용하고 있음." )
				<< BUILD_LOG( pItemTemplet->m_ItemID )
				<< BUILD_LOG( pItemTemplet->m_Name )
				<< BUILD_LOG( m_pUser->GetLevel() )
				;
			continue;
		}
#ifdef SERV_EVENT_VALENTINE_RING_IS_DUNGEON
		if( pItemTemplet->m_ItemID == 141000980 ||
			pItemTemplet->m_ItemID == 141000981 ||
			pItemTemplet->m_ItemID == 141000982 ||
			pItemTemplet->m_ItemID == 141000983 ||
			pItemTemplet->m_ItemID == 141000984 )
		{
			if( iTempDungeonID != SEnum::DI_EVENT_VALENTINE_DUNGEON_INT )
			{
				START_LOG( clog, L"발렌타인 던전 아이템 다른던전에서 사용 안됨" )
					<< BUILD_LOG( pItemTemplet->m_ItemID )
					<< BUILD_LOG( iTempDungeonID )
					<< END_LOG;
					continue;
			}
			else
			{
				START_LOG( clog, L"발렌타인 던전 아이템 사용가능" )
				<< BUILD_LOG( pItemTemplet->m_ItemID )
				<< BUILD_LOG( iTempDungeonID )
				<< END_LOG;
			}
		}
#endif SERV_EVENT_VALENTINE_RING_IS_DUNGEON

		// 4. 세트 아이템인지 검사
		if( pItemTemplet->m_SetID > 0 )
		{
			setEquippedItemSetID.insert( pItemTemplet->m_SetID );
		}

		//{{ 2013. 05. 20	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		//////////////////////////////////////////////////////////////////////////
		// 5. 아이템 기본 스탯		
		if( pItemTemplet->IsCommonItem() == true )
		{
			// 각 소켓 정보들을 수집해서 
			KStatRelationLevel kAddStatRelLevel;
			SiCXSLSocketItem()->GetAndAddStatRelationLevel( kInfo.m_kItemInfo.m_vecItemSocket, kAddStatRelLevel );
			SiCXSLSocketItem()->GetAndAddStatRelationLevel( kInfo.m_kItemInfo.m_vecRandomSocket, kAddStatRelLevel );
			SiCXSLSocketItem()->GetAndAddStatRelationLevel( pItemTemplet->m_vecSocketOption, kAddStatRelLevel );

			// 공용 아이템의 기본 스탯
			KStat kCommonItemStat;
			LIF( SiCXSLCharacterStatTable()->CalcCommonItemStat( pItemTemplet, m_pUser->GetUnitType(), m_pUser->GetUnitClass(), kAddStatRelLevel, kCommonItemStat ) );

			kItemBaseStat.Init();
			kItemBaseStat.AddStat( kCommonItemStat );
		}
		else
		{
			// 일반 아이템 기본 스탯			
			kItemBaseStat.Init();
			kItemBaseStat.m_iBaseHP    += (int)( pItemTemplet->m_Stat.m_fBaseHP );
			kItemBaseStat.m_iAtkPhysic += (int)( pItemTemplet->m_Stat.m_fAtkPhysic );
			kItemBaseStat.m_iAtkMagic  += (int)( pItemTemplet->m_Stat.m_fAtkMagic );
			kItemBaseStat.m_iDefPhysic += (int)( pItemTemplet->m_Stat.m_fDefPhysic );
			kItemBaseStat.m_iDefMagic  += (int)( pItemTemplet->m_Stat.m_fDefMagic );
		}
#else
		//////////////////////////////////////////////////////////////////////////
		// 5. 아이템 기본 스탯
		kItemBaseStat.Init();
		kItemBaseStat.m_iBaseHP    += (int)( pItemTemplet->m_Stat.m_fBaseHP );
		kItemBaseStat.m_iAtkPhysic += (int)( pItemTemplet->m_Stat.m_fAtkPhysic );
		kItemBaseStat.m_iAtkMagic  += (int)( pItemTemplet->m_Stat.m_fAtkMagic );
		kItemBaseStat.m_iDefPhysic += (int)( pItemTemplet->m_Stat.m_fDefPhysic );
		kItemBaseStat.m_iDefMagic  += (int)( pItemTemplet->m_Stat.m_fDefMagic );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		//{{ 2012. 09. 25	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
		//////////////////////////////////////////////////////////////////////////		
		// 아이템 템플릿의 소켓옵션 적용 ( Level Linked Stat )
		{
			KStat kSocketOptionStat;
			if( pItemTemplet->m_UseLevel < 1 )  // 아이템 사용 제한 레벨이 없으면
			{
				SiCXSLSocketItem()->GetSocketOptionStat( pItemTemplet->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel, true );
			}
			else
			{
				unsigned int uiItemLevel = pItemTemplet->m_UseLevel;
				SiCXSLSocketItem()->GetSocketOptionStat( pItemTemplet->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiItemLevel, true );
			}

			// 스탯 더하기
			kItemBaseStat.AddStat( kSocketOptionStat );
		}
#endif SERV_2012_PVP_SEASON2_1
		//}}

		//////////////////////////////////////////////////////////////////////////
		// 6. 스탯 강화 적용
		float fEnchantRate;
		if( !SiCXSLEnchantItemManager()->GetEnchantRate( ( int )kInfo.m_kItemInfo.m_cEnchantLevel, fEnchantRate ) )
		{
			fEnchantRate = 1.f;
		}

		//kAddStat.m_iBaseHP			+= (int)( kItemBaseStat.m_iBaseHP ); // BaseHP 스탯은 강화적용 안시킴.
		// BaseHP 스텟도 강화 적용 되게 변경
		kAddStat.m_iBaseHP			+= (int)( kItemBaseStat.m_iBaseHP					* fEnchantRate );
		kAddStat.m_iAtkPhysic		+= (int)( kItemBaseStat.m_iAtkPhysic				* fEnchantRate );
		kAddStat.m_iAtkMagic		+= (int)( kItemBaseStat.m_iAtkMagic					* fEnchantRate );
		kAddStat.m_iDefPhysic		+= (int)( kItemBaseStat.m_iDefPhysic				* fEnchantRate );
		kAddStat.m_iDefMagic		+= (int)( kItemBaseStat.m_iDefMagic					* fEnchantRate );

		//////////////////////////////////////////////////////////////////////////
		// 7. 소켓옵션 스탯 적용
		{
			KStat kSocketOptionStat;
			//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
			if( pItemTemplet->m_UseLevel < 1 )  // 아이템 사용 제한 레벨이 없으면
			{
				SiCXSLSocketItem()->GetSocketOptionStat( kInfo.m_kItemInfo.m_vecItemSocket, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel );
			}
			else
			{
				unsigned int uiItemLevel = pItemTemplet->m_UseLevel;
				SiCXSLSocketItem()->GetSocketOptionStat( kInfo.m_kItemInfo.m_vecItemSocket, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiItemLevel );
			}
#else
			SiCXSLSocketItem()->GetSocketOptionStat( kInfo.m_kItemInfo.m_vecItemSocket, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
			//}} 
		
			// 스탯 더하기
			kAddStat.AddStat( kSocketOptionStat );
		}

		//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		//////////////////////////////////////////////////////////////////////////
		// 랜덤 소켓의 소켓옵션 적용
		{
			KStat kSocketOptionStat;
			if( pItemTemplet->m_UseLevel < 1 )  // 아이템 사용 제한 레벨이 없으면
			{
				SiCXSLSocketItem()->GetSocketOptionStat( kInfo.m_kItemInfo.m_vecRandomSocket, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel );
			}
			else
			{
				unsigned int uiItemLevel = pItemTemplet->m_UseLevel;
				SiCXSLSocketItem()->GetSocketOptionStat( kInfo.m_kItemInfo.m_vecRandomSocket, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiItemLevel );
			}

			// 스탯 더하기
			kAddStat.AddStat( kSocketOptionStat );
		}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		//////////////////////////////////////////////////////////////////////////
		// 8. 아이템 템플릿의 소켓옵션 적용
		{
			KStat kSocketOptionStat;
			//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
			if( pItemTemplet->m_UseLevel < 1 )  // 아이템 사용 제한 레벨이 없으면
			{
				SiCXSLSocketItem()->GetSocketOptionStat( pItemTemplet->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel );
			}
			else
			{
				unsigned int uiItemLevel = pItemTemplet->m_UseLevel;
				SiCXSLSocketItem()->GetSocketOptionStat( pItemTemplet->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiItemLevel );
			}
#else
			SiCXSLSocketItem()->GetSocketOptionStat( pItemTemplet->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
			//}} 

			// 스탯 더하기
			kAddStat.AddStat( kSocketOptionStat );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 9. 세트 아이템 스탯 적용
	BOOST_TEST_FOREACH( const int, iSetID, setEquippedItemSetID )
	{
		// 9-1. 세트 아이템 템플릿 얻기
		const CXSLSocketItem::SetItemData* pSetItemData = SiCXSLSocketItem()->GetSetItem( iSetID );
		if( pSetItemData == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 SetID를 가진 아이템을 장착중입니다." )
				<< BUILD_LOG( iSetID )
				<< END_LOG;
			continue;
		}

		// 9-2. 장착중인 아이템중 해당 setItem을 현재 몇개 장착중인지 얻기
		const int iSetItemCount = GetEquippedSetItemCount( iSetID );

		// 9-3. 장착된 수량만큼 적용될 소켓옵션 찾기
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::map< int, std::vector< int > >::const_iterator mit = pSetItemData->m_mapNeedPartsNumNOptions.end();
#else
		std::map< int, std::vector< short > >::const_iterator mit = pSetItemData->m_mapNeedPartsNumNOptions.end();
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		for( int iCnt = iSetItemCount; iCnt > 0; --iCnt )
		{
			mit = pSetItemData->m_mapNeedPartsNumNOptions.find( iCnt );
			if( mit == pSetItemData->m_mapNeedPartsNumNOptions.end() )
				continue;
			else
				break;
		}

		if( mit == pSetItemData->m_mapNeedPartsNumNOptions.end() )
			continue;

		// 9-4. 소켓 옵션 적용
		{
			KStat kSocketOptionStat;
			//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
			unsigned int uiSetMaxLevel = static_cast< unsigned int >(SiCXSLItemManager()->GetSetItemMaxLevel( iSetID ));
			if( uiSetMaxLevel < 1 )  // 아이템 사용 제한 레벨이 없으면
			{
				SiCXSLSocketItem()->GetSocketOptionStat( mit->second, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel );
			}
			else
			{
				SiCXSLSocketItem()->GetSocketOptionStat( mit->second, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiSetMaxLevel );
			}
#else
			SiCXSLSocketItem()->GetSocketOptionStat( mit->second, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
			//}} 

			// 스탯 더하기
			kAddStat.AddStat( kSocketOptionStat );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//#else
////////////////////////////////////////////////////////////////////////////
//
//void KInventory::GetEquippedStat( IN int iModifiedBaseHPBySkill, KStat& kStat, float& fTotalIncHPRate, bool bIsDungeonRoom )
//{
//	kStat.Init();
//
//	if( !m_pUser )
//	{
//		START_LOG( cerr, L"유저 포인터 이상." )
//			<< END_LOG;
//
//		return;
//	}
//
//	int i;
//	std::set< int > setEquippedItemSetID;
//	KStat kItemBaseStat;
//	for( i = 0; i < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++i )
//	{
//		if( !IsEmptySlot( CXSLInventory::ST_E_EQUIP, i ) )
//		{
//			KInventoryItemInfo kInfo;
//			if( !GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, i, kInfo ) )
//			{
//				START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
//					<< BUILD_LOG( i )
//					<< END_LOG;
//
//				continue;
//			}
//
//			// 1. 내구도 아이템의 내구도가 0 이하이면 스탯에 추가되지 않는다.
//			if( kInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_ENDURANCE )
//			{
//				if( kInfo.m_kItemInfo.m_sEndurance <= 0 )
//				{
//					continue;
//				}
//			}
//
//			// 2. 사용불가 아이템이면 스탯에 추가되지 않는다.
//			if( kInfo.m_kItemInfo.m_cEnchantLevel < 0 )
//				continue;
//
//			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
//			if( pItemTemplet == NULL )
//			{
//				START_LOG( cerr, L"아이템 템플릿 포인트 이상.!" )
//					<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
//					<< END_LOG;
//
//				continue;
//			}
//
//			// 3. 유닛 레벨과 장착조건 레벨이 틀리면 장착되지 않는다. - 아이템의 레벨조정으로 인한 옵션기능.
//			if( m_pUser->GetLevel() < pItemTemplet->m_UseLevel )
//			{
//				START_LOG( cwarn, L"아이템 장착조건이 되지 않는데 아이템을 착용하고 있음." )
//					<< BUILD_LOG( pItemTemplet->m_ItemID )
//					<< BUILD_LOG( pItemTemplet->m_Name )
//					<< BUILD_LOG( m_pUser->GetLevel() )
//					;
//				continue;
//			}
//
//			// 4. 세트 아이템인지 검사
//			if( pItemTemplet->m_SetID > 0 )
//			{
//				setEquippedItemSetID.insert( pItemTemplet->m_SetID );
//			}
//
//			// 5. 해당 아이템의 스탯
//			kItemBaseStat.Init();
//			kItemBaseStat.m_iBaseHP    += (int)( pItemTemplet->m_Stat.m_fBaseHP );
//			kItemBaseStat.m_iAtkPhysic += (int)( pItemTemplet->m_Stat.m_fAtkPhysic );
//			kItemBaseStat.m_iAtkMagic  += (int)( pItemTemplet->m_Stat.m_fAtkMagic );
//			kItemBaseStat.m_iDefPhysic += (int)( pItemTemplet->m_Stat.m_fDefPhysic );
//			kItemBaseStat.m_iDefMagic  += (int)( pItemTemplet->m_Stat.m_fDefMagic );
//
//			// 6. 스탯 강화 적용
//			float fEnchantRate;
//			if( !SiCXSLEnchantItemManager()->GetEnchantRate( ( int )kInfo.m_kItemInfo.m_cEnchantLevel, fEnchantRate ) )
//			{
//				fEnchantRate = 1.f;
//			}
//
//			//kStat.m_iBaseHP			+= (int)( kItemBaseStat.m_iBaseHP ); // BaseHP 스탯은 강화적용 안시킴.
//			// BaseHP 스텟도 강화 적용 되게 변경
//			kStat.m_iBaseHP			+= (int)( kItemBaseStat.m_iBaseHP					* fEnchantRate );
//			kStat.m_iAtkPhysic		+= (int)( kItemBaseStat.m_iAtkPhysic				* fEnchantRate );
//			kStat.m_iAtkMagic		+= (int)( kItemBaseStat.m_iAtkMagic					* fEnchantRate );
//			kStat.m_iDefPhysic		+= (int)( kItemBaseStat.m_iDefPhysic				* fEnchantRate );
//			kStat.m_iDefMagic		+= (int)( kItemBaseStat.m_iDefMagic					* fEnchantRate );
//
//			// 7. 소켓옵션 스탯 적용			
//			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
//			std::vector< int >::const_iterator vit = kInfo.m_kItemInfo.m_vecItemSocket.begin();
//#else
//			std::vector< short >::const_iterator vit = kInfo.m_kItemInfo.m_vecItemSocket.begin();
//#endif SERV_ITEM_OPTION_DATA_SIZE
//			//}} 
//			for( ; vit != kInfo.m_kItemInfo.m_vecItemSocket.end(); ++vit )
//			{
//				if( *vit <= 0 )
//					continue;
//
//				const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
//				if( pSocketData == NULL )
//				{
//					START_LOG( cerr, L"소켓 옵션 정보를 찾을수 없습니다." )
//						<< BUILD_LOG( *vit )
//						<< END_LOG;
//					continue;
//				}
//
//				//{{ 2009. 4. 1  최육사		던전온리
//				if( pSocketData->m_bDungeonOnly  &&  !bIsDungeonRoom )
//					continue;
//				//}}
//
//				if( true == pSocketData->m_bPVPOnly  &&  true == bIsDungeonRoom )
//					continue;
//
//				//{{ 2008. 11. 20  최육사	HP증가율 50% 제한
//				float fIncreaseHPRate = pSocketData->m_IncreaseHPRate;
//
//				if( ( fTotalIncHPRate + fIncreaseHPRate ) > 0.5f )
//				{
//					fIncreaseHPRate = 0.5f - fTotalIncHPRate;
//				}
//
//				fTotalIncHPRate += fIncreaseHPRate;				
//				//}}
//
//				kStat.m_iBaseHP		+= (int)( iModifiedBaseHPBySkill * fIncreaseHPRate );
//				kStat.m_iBaseHP		+= (int)( pSocketData->m_Stat.m_fBaseHP );
//				kStat.m_iAtkPhysic	+= (int)( pSocketData->m_Stat.m_fAtkPhysic );
//				kStat.m_iAtkMagic	+= (int)( pSocketData->m_Stat.m_fAtkMagic );
//				kStat.m_iDefPhysic	+= (int)( pSocketData->m_Stat.m_fDefPhysic );
//				kStat.m_iDefMagic	+= (int)( pSocketData->m_Stat.m_fDefMagic );
//			}
//
//			// 8. 아이템 템플릿의 소켓옵션 적용
//			vit = pItemTemplet->m_vecSocketOption.begin();
//			for( ; vit != pItemTemplet->m_vecSocketOption.end(); ++vit )
//			{
//				if( *vit <= 0 )
//					continue;
//
//				const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
//				if( pSocketData == NULL )
//				{
//					START_LOG( cerr, L"소켓 옵션 정보를 찾을수 없습니다." )
//						<< BUILD_LOG( *vit )
//						<< END_LOG;
//					continue;
//				}
//
//				//{{ 2009. 4. 1  최육사		던전온리
//				if( pSocketData->m_bDungeonOnly  &&  !bIsDungeonRoom )
//					continue;
//				//}}
//
//
//				if( true == pSocketData->m_bPVPOnly && true == bIsDungeonRoom )	
//					continue;
//
//				//{{ 2008. 11. 20  최육사	HP증가율 50% 제한
//				float fIncreaseHPRate = pSocketData->m_IncreaseHPRate;
//
//				if( ( fTotalIncHPRate + fIncreaseHPRate ) > 0.5f )
//				{
//					fIncreaseHPRate = 0.5f - fTotalIncHPRate;
//				}
//
//				fTotalIncHPRate += fIncreaseHPRate;
//				//}}
//
//				kStat.m_iBaseHP		+= (int)( iModifiedBaseHPBySkill * fIncreaseHPRate );
//				kStat.m_iBaseHP		+= (int)( pSocketData->m_Stat.m_fBaseHP );
//				kStat.m_iAtkPhysic	+= (int)( pSocketData->m_Stat.m_fAtkPhysic );
//				kStat.m_iAtkMagic	+= (int)( pSocketData->m_Stat.m_fAtkMagic );
//				kStat.m_iDefPhysic	+= (int)( pSocketData->m_Stat.m_fDefPhysic );
//				kStat.m_iDefMagic	+= (int)( pSocketData->m_Stat.m_fDefMagic );
//			}
//		}
//	}
//
//	// 9. 세트 아이템 스탯 적용
//	std::set< int >::const_iterator sit;
//	for( sit = setEquippedItemSetID.begin(); sit != setEquippedItemSetID.end(); ++sit )
//	{
//		// 9-1. 세트 아이템 템플릿 얻기
//		const CXSLSocketItem::SetItemData* pSetItemData = SiCXSLSocketItem()->GetSetItem( *sit );
//		if( pSetItemData == NULL )
//		{
//			START_LOG( cerr, L"존재하지 않는 SetID를 가진 아이템을 장착중입니다." )
//				<< BUILD_LOG( *sit )
//				<< END_LOG;
//			continue;
//		}
//
//		// 9-2. 장착중인 아이템중 해당 setItem을 현재 몇개 장착중인지 얻기
//		int iSetItemCount = GetEquippedSetItemCount( *sit );
//
//		// 9-3. 장착된 수량만큼 적용될 소켓옵션 찾기
//		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
//		std::map< int, std::vector< int > >::const_iterator mit = pSetItemData->m_mapNeedPartsNumNOptions.end();
//#else
//		std::map< int, std::vector< short > >::const_iterator mit = pSetItemData->m_mapNeedPartsNumNOptions.end();
//#endif SERV_ITEM_OPTION_DATA_SIZE
//		//}} 
//		for( int iCnt = iSetItemCount; iCnt > 0; --iCnt )
//		{
//			mit = pSetItemData->m_mapNeedPartsNumNOptions.find( iCnt );
//			if( mit == pSetItemData->m_mapNeedPartsNumNOptions.end() )
//				continue;
//			else
//				break;
//		}
//
//		if( mit == pSetItemData->m_mapNeedPartsNumNOptions.end() )
//			continue;
//
//		// 9-4. 소켓 옵션 적용
//		std::vector< short >::const_iterator vit;
//		for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
//		{
//			const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
//			if( pSocketData == NULL )
//			{
//				START_LOG( cerr, L"존재하지않는 소켓옵션이 세트아이템의 소켓옵션으로 세팅되어 있습니다." )
//					<< BUILD_LOG( pSetItemData->m_SetID )
//					<< BUILD_LOG( pSetItemData->m_SetName )
//					<< BUILD_LOG( *vit )
//					<< END_LOG;
//				continue;
//			}
//
//			//{{ 2009. 4. 1  최육사		던전온리
//			if( pSocketData->m_bDungeonOnly  &&  !bIsDungeonRoom )
//				continue;
//			//}}
//
//
//			if( true == pSocketData->m_bPVPOnly && true == bIsDungeonRoom )	
//				continue;
//
//			//{{ 2008. 11. 20  최육사	HP증가율 50% 제한
//			float fIncreaseHPRate = pSocketData->m_IncreaseHPRate;
//
//			if( ( fTotalIncHPRate + fIncreaseHPRate ) > 0.5f )
//			{
//				fIncreaseHPRate = 0.5f - fTotalIncHPRate;
//			}
//
//			fTotalIncHPRate += fIncreaseHPRate;
//			//}}
//
//			kStat.m_iBaseHP		+= (int)( iModifiedBaseHPBySkill * fIncreaseHPRate );
//			kStat.m_iBaseHP		+= (int)( pSocketData->m_Stat.m_fBaseHP );
//			kStat.m_iAtkPhysic	+= (int)( pSocketData->m_Stat.m_fAtkPhysic );
//			kStat.m_iAtkMagic	+= (int)( pSocketData->m_Stat.m_fAtkMagic );
//			kStat.m_iDefPhysic	+= (int)( pSocketData->m_Stat.m_fDefPhysic );
//			kStat.m_iDefMagic	+= (int)( pSocketData->m_Stat.m_fDefMagic );
//		}
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
//#endif SERV_PET_AURA_SKILL
////////////////////////////////////////////////////////////////////////////
//}}

//{{ 2009. 7. 28  최육사
void KInventory::GetEquippedStatOnlyGMWeapon( KStat& kStat )
{
	kStat.Init();

	if( !m_pUser )
	{
		START_LOG( cerr, L"유저 포인터 이상." )
			<< END_LOG;

		return;
	}

	if( m_pUser->GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"GM도 아닌데 이 함수 호출했네? 절대 발생해서는 안되는 에러!" )
			<< BUILD_LOGc( m_pUser->GetAuthLevel() )
			<< BUILD_LOG( m_pUser->GetCharUID() )
			<< BUILD_LOG( m_pUser->GetCharName() )
			<< END_LOG;

		return;
	}
	
	for( int i = 0; i < GetSlotSize( CXSLInventory::ST_E_EQUIP ); i++ )
	{
		if( !IsEmptySlot( CXSLInventory::ST_E_EQUIP, i ) )
		{
			KInventoryItemInfo kInfo;
			if( !GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, i, kInfo ) )
			{
				START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
					<< BUILD_LOG( i )
					<< END_LOG;

				continue;
			}

			// GM무기만 적용됨
			if( CXSLItem::IsGMweapon( kInfo.m_kItemInfo.m_iItemID ) == false )
				continue;

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿 포인트 이상.!" )
					<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
					<< END_LOG;

				continue;
			}

			// 해당 아이템의 스탯
			kStat.m_iBaseHP    += (int)( pItemTemplet->m_Stat.m_fBaseHP );
			kStat.m_iAtkPhysic += (int)( pItemTemplet->m_Stat.m_fAtkPhysic );
			kStat.m_iAtkMagic  += (int)( pItemTemplet->m_Stat.m_fAtkMagic );
			kStat.m_iDefPhysic += (int)( pItemTemplet->m_Stat.m_fDefPhysic );
			kStat.m_iDefMagic  += (int)( pItemTemplet->m_Stat.m_fDefMagic );
		}
	}
}
//}}

//{{ 2009. 2. 2  최육사		소켓옵션
void KInventory::GetEquippedItemBonusRate( IN const bool bIsDungeonRoom, OUT float& fAddTitleExpRate, OUT float& fAddTitleEDRate )
{
#ifdef SERV_SET_ITEM_OPTION_ADD
	// 4. 세트 아이템인지 검사
	std::set< int > setEquippedItemSetID;
#endif SERV_SET_ITEM_OPTION_ADD
	for( int i = 0; i < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++i )
	{
		if( !IsEmptySlot( CXSLInventory::ST_E_EQUIP, i ) )
		{
			KInventoryItemInfo kInfo;
			if( !GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, i, kInfo ) )
			{
				START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
					<< BUILD_LOG( i )
					<< END_LOG;

				continue;
			}

			// 1. 내구도 아이템의 내구도가 0 이하이면 스탯에 추가되지 않는다.
			if( kInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_ENDURANCE )
			{
				if( kInfo.m_kItemInfo.m_sEndurance <= 0 )
				{
					continue;
				}
			}

			// 2. 사용불가 아이템이면 소켓옵션이 적용되지 않는다.
			if( kInfo.m_kItemInfo.m_cEnchantLevel < 0 )
				continue;

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿 포인트 이상.!" )
					<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
					<< END_LOG;

				continue;
			}

			// 3. 유닛 레벨과 장착조건 레벨이 틀리면 소켓옵션이 적용되지 않는다. - 아이템의 레벨조정으로 인한 옵션기능.
			if( m_pUser->GetLevel() < pItemTemplet->m_UseLevel )
			{
				START_LOG( cwarn, L"아이템 장착조건이 되지 않는데 아이템을 착용하고 있음." )
					<< BUILD_LOG( pItemTemplet->m_ItemID )
					<< BUILD_LOG( pItemTemplet->m_Name )
					<< BUILD_LOG( m_pUser->GetLevel() )
					;
				continue;
			}

			// 4. 아이템 템플릿의 부가옵션 적용
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			std::vector< int >::const_iterator vit = pItemTemplet->m_vecSocketOption.begin();
#else
			std::vector< short >::const_iterator vit = pItemTemplet->m_vecSocketOption.begin();
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			for( ; vit != pItemTemplet->m_vecSocketOption.end(); ++vit )
			{
				if( *vit <= 0 )
					continue;

				const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
				if( pSocketData == NULL )
				{
					START_LOG( cerr, L"소켓 옵션 정보를 찾을수 없습니다." )
						<< BUILD_LOG( *vit )
						<< END_LOG;
					continue;
				}

				//{{ 2009. 4. 1  최육사		던전온리
				if( pSocketData->m_bDungeonOnly  &&  !bIsDungeonRoom )
					continue;
				//}}


				if( true == pSocketData->m_bPVPOnly && true == bIsDungeonRoom )	
					continue;


				if( pSocketData->m_fIncreaseExpPer > 0.0f )
					fAddTitleExpRate += pSocketData->m_fIncreaseExpPer;

				if( pSocketData->m_fIncreaseEDPer > 0.0f )
					fAddTitleEDRate += pSocketData->m_fIncreaseEDPer;
			}

			// 5. 소켓옵션 적용
			vit = kInfo.m_kItemInfo.m_vecItemSocket.begin();
			for( ; vit != kInfo.m_kItemInfo.m_vecItemSocket.end(); ++vit )
			{
				if( *vit <= 0 )
					continue;

				const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
				if( pSocketData == NULL )
				{
					START_LOG( cerr, L"소켓 옵션 정보를 찾을수 없습니다." )
						<< BUILD_LOG( *vit )
						<< END_LOG;
					continue;
				}

				//{{ 2009. 4. 1  최육사		던전온리
				if( pSocketData->m_bDungeonOnly  &&  !bIsDungeonRoom )
					continue;
				//}}


				if( true == pSocketData->m_bPVPOnly && true == bIsDungeonRoom )	
					continue;

				if( pSocketData->m_fIncreaseExpPer > 0.0f )
					fAddTitleExpRate += pSocketData->m_fIncreaseExpPer;

				if( pSocketData->m_fIncreaseEDPer > 0.0f )
					fAddTitleEDRate += pSocketData->m_fIncreaseEDPer;
			}
#ifdef SERV_SET_ITEM_OPTION_ADD
///셋트 아이템 옵션 적용
			if( pItemTemplet->m_SetID > 0 )
			{
				setEquippedItemSetID.insert( pItemTemplet->m_SetID );
			}
#endif SERV_SET_ITEM_OPTION_ADD
		}
	}
#ifdef SERV_SET_ITEM_OPTION_ADD
	//////////////////////////////////////////////////////////////////////////
	// 9. 세트 아이템 스탯 적용
	BOOST_TEST_FOREACH( const int, iSetID, setEquippedItemSetID )
	{
		// 9-1. 세트 아이템 템플릿 얻기
		const CXSLSocketItem::SetItemData* pSetItemData = SiCXSLSocketItem()->GetSetItem( iSetID );
		if( pSetItemData == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 SetID를 가진 아이템을 장착중입니다." )
				<< BUILD_LOG( iSetID )
				<< END_LOG;
			continue;
		}

		// 9-2. 장착중인 아이템중 해당 setItem을 현재 몇개 장착중인지 얻기
		const int iSetItemCount = GetEquippedSetItemCount( iSetID );

		// 9-3. 장착된 수량만큼 적용될 소켓옵션 찾기
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::map< int, std::vector< int > >::const_iterator mit = pSetItemData->m_mapNeedPartsNumNOptions.end();
#else
		std::map< int, std::vector< short > >::const_iterator mit = pSetItemData->m_mapNeedPartsNumNOptions.end();
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		for( int iCnt = iSetItemCount; iCnt > 0; --iCnt )
		{
			mit = pSetItemData->m_mapNeedPartsNumNOptions.find( iCnt );
			if( mit == pSetItemData->m_mapNeedPartsNumNOptions.end() )
				continue;
			else
				break;
		}
		if( mit == pSetItemData->m_mapNeedPartsNumNOptions.end() )
			continue;

		std::vector< int > vecTemp = mit->second;
		BOOST_TEST_FOREACH( const int, sSocketOptionID, vecTemp )
		{
			if( sSocketOptionID == 0 )
				continue;

			const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( sSocketOptionID );
			if( pSocketData == NULL )
			{
				START_LOG( cerr, L"존재하지않는 소켓옵션이 세팅되어 있습니다." )
					<< BUILD_LOG( sSocketOptionID )
					<< END_LOG;
				continue;
			}
			//{{ 2009. 4. 1  최육사		던전온리
			if( pSocketData->m_bDungeonOnly  &&  !bIsDungeonRoom )
				continue;
			//}}


			if( true == pSocketData->m_bPVPOnly && true == bIsDungeonRoom )	
				continue;

			if( pSocketData->m_fIncreaseExpPer > 0.0f )
				fAddTitleExpRate += pSocketData->m_fIncreaseExpPer;

			if( pSocketData->m_fIncreaseEDPer > 0.0f )
				fAddTitleEDRate += pSocketData->m_fIncreaseEDPer;
		}
	}
#endif SERV_SET_ITEM_OPTION_ADD
}
//}}

//{{ 2011. 10. 18	최육사	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
bool KInventory::GetItemLevel( IN const UidType iItemUID, OUT int& iItemLevel )
{
	iItemLevel = 0;

    KInventoryItemInfo kInfo;
	if( GetInventoryItemInfo( iItemUID, kInfo ) == false )
	{
		START_LOG( cerr, L"아이템 정보가 존재하지 않습니다!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;
		return false;
	}

	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	if( SiCXSLItemManager()->GetItemLevel( m_pUser->GetUnitType(), m_pUser->GetUnitClass(), kInfo.m_kItemInfo, iItemLevel ) == false )
#else
	if( SiCXSLItemManager()->GetItemLevel( kInfo.m_kItemInfo, iItemLevel ) == false )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}	
	{
		START_LOG( cerr, L"아이템 레벨 정보 얻기를 실패하였습니다!" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
			<< END_LOG;
		return false;
	}
	
	return true;
}
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
//}}

void KInventory::IncreaseQuantity( UidType iItemUID, int iQuantity, int& iIncreased )
{
    iIncreased = 0;
    std::map< UidType, KInventoryItem >::iterator mit;

    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        START_LOG( cerr, L"해당 아이템 UID가 없음." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iQuantity )
            << END_LOG;

        return;
    }

    mit->second.IncreaseQuantity( iQuantity, iIncreased );
	//{{ 2012. 10. 04	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	m_kItemQuantityVariation.IncreaseQuantity( mit->second.GetItemID(), iIncreased );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}
}

void KInventory::IncreaseQuantity( int iCategory, int iSlotID, int iQuantity, int& iIncreased )
{
    IncreaseQuantity( GetItemUID( iCategory, iSlotID ), iQuantity, iIncreased );
}

void KInventory::DecreaseQuantity( IN UidType iItemUID, IN int iQuantity, OUT int& iDecreased, IN KDeletedItemInfo::DELETE_REASON eReason )
{
    iDecreased = 0;
    std::map< UidType, KInventoryItem >::iterator mit;

    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        START_LOG( cerr, L"해당 아이템 UID가 없음." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iQuantity )
            << END_LOG;

        return;
    }

    mit->second.DecreaseQuantity( iQuantity, iDecreased );
	//{{ 2012. 10. 04	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	m_kItemQuantityVariation.DecreaseQuantity( mit->second.GetItemID(), iDecreased );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

    if( mit->second.IsQuantityExhausted() )
    {
        KInventoryItemInfo kInventorySlotInfo;
        DeleteItem( iItemUID, kInventorySlotInfo, eReason );
    }
}

void KInventory::DecreaseQuantity( IN int iCategory, IN int iSlotID, IN int iQuantity, OUT int& iDecreased, IN KDeletedItemInfo::DELETE_REASON eReason )
{
    DecreaseQuantity( GetItemUID( iCategory, iSlotID ), iQuantity, iDecreased, eReason );
}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
void KInventory::DecreaseQuantityOrDelete( IN UidType iItemUID, IN int iQuantity, OUT int& iDecreased, IN KDeletedItemInfo::DELETE_REASON eReason, OUT KInventoryItemInfo& kDeleteItemInfo )
{
	iDecreased = 0;
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;

		return;
	}

	mit->second.DecreaseQuantity( iQuantity, iDecreased );
	if( mit->second.IsQuantityExhausted() )
	{
		DeleteItem( iItemUID, kDeleteItemInfo, eReason );
	}
}

//{{ 2012. 07. 3	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
void KInventory::FixUpInventoryItem( IN OUT std::vector< KInventoryItemInfo >& vecInsertedItem, OUT KItemQuantityUpdate& kDBUpdate, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	kDBUpdate.m_mapQuantityChange.clear();		// 수량이 합쳐져서 증가/감소하는 것
	kDBUpdate.m_vecDeleted.clear();				// 수량이 합쳐져서 삭제되는 것

	// 거래시 자신에게서 나간 아이템정보가 있으므로 지우면 안된다.
	//vecInventorySlotInfo.clear();				

	std::vector< KInventoryItemInfo > vecInsertedItem_Copy = vecInsertedItem; // 실제 인설트 되어야 하는 아이템
	vecInsertedItem.clear();
	
	typedef std::map< int, GatheringClass >	TYPE_MAPQUANTITYITEM;
	TYPE_MAPQUANTITYITEM					mapQuantityItem;

	std::map< UidType, KInventoryItemInfo >	mapItem_Quantity;		// DB 에 기록되어 있는 아이템의 수량을 확인하기 위해서 임시 저장
	std::map< UidType, KInventoryItemInfo >	mapInventorySlotInfo;

	BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItem, vecInsertedItem_Copy )
	{
		const int& iItemID = kInsertItem.m_kItemInfo.m_iItemID;
		const CXSLItem::ItemTemplet* pItemTemplate = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplate == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( kInsertItem.m_iItemUID )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}

		if( pItemTemplate->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			TYPE_MAPQUANTITYITEM::iterator it = mapQuantityItem.find( iItemID );
			if( it == mapQuantityItem.end() )
			{
				GatheringClass temp;
				it = mapQuantityItem.insert( TYPE_MAPQUANTITYITEM::value_type( iItemID, temp ) ).first;
				if( it == mapQuantityItem.end() )
				{
					START_LOG( cerr, L"STL_MAP::Insert 실패!!")
						<< BUILD_LOG( iItemID )
						<< END_LOG;
					continue;
				}
			}
			it->second.push_back( kInsertItem );
			mapItem_Quantity.insert( std::make_pair( kInsertItem.m_iItemUID, kInsertItem ) );
		}
		else
		{
			// 한개 품목은 무조건 슬롯하나를 차지한다.
			vecInsertedItem.push_back( kInsertItem );
		}
	}

	// 인벤토리에 있는 수량성 아이템 중에 수량이 가득차지 않은 아이템을 찾아서 합치자

	// 임시로 채우기를 하기 위한 카테고리 슬롯 정보의 복사본입니다.
	std::map< UidType, KInventoryItem >	mapItemCopy;
	std::vector< UidType >				vecInventorySlotCopy[CXSLInventory::ST_END];

	for( int i=0; i < CXSLInventory::ST_END; ++i )
	{
		vecInventorySlotCopy[i].resize( m_vecInventorySlot[i].size() );
		std::copy( m_vecInventorySlot[i].begin(), m_vecInventorySlot[i].end(), vecInventorySlotCopy[i].begin() );
	}
	std::copy( m_mapItem.begin(), m_mapItem.end(), std::inserter( mapItemCopy, mapItemCopy.begin() ) );

	// 요점은 최대한 모아서 삽입은 최대한 한번에 몰아서 넣어야 한다는 것!
	// 나눠서 삽입하면 채울 때 문제가 생긴다.
	for( TYPE_MAPQUANTITYITEM::iterator it = mapQuantityItem.begin(); it != mapQuantityItem.end(); ++it )
	{
		const int&		iItemID			= it->first;
		GatheringClass& gatheringClass	= it->second;

		// 수량이 있는 것만
		if( gatheringClass.GetAmountOfQuantity() <= 0 )
		{
			START_LOG( clog, L"벌써 지웠는데 왜 또?" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( gatheringClass.GetAmountOfQuantity() )
				<< END_LOG;
			continue;
		}

		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( iItemID, vecCategoryID );

		// 카테고리를 순회하면서 수량을 채워넣으며, 또한 빈 슬롯에 아이템을 채워봅니다.
		BOOST_TEST_FOREACH( const int&, iCategory, vecCategoryID )
		{
			int iNumIncreased		= 0;

			// 원본에 채워넣기
			if( FillTheUnFullSlot( iCategory, iItemID, gatheringClass.GetAmountOfQuantity(), m_vecInventorySlot, m_mapItem, &kDBUpdate, &mapInventorySlotInfo, NULL, iNumIncreased ) == false )
			{
				START_LOG( cerr, L"채워넣기 실패! 이후의 결과를 예측할 수 없습니다.")
					<< END_LOG;
				continue;
			}
			gatheringClass.DecreaseTheAmountOfQuantity_Buffer( iNumIncreased );
			gatheringClass.FlushDecreaseQuantity();

			// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
			if( gatheringClass.GetAmountOfQuantity() <= 0 )
			{
				break;
			}

			// 남은 수량이 있다면, 현재 카테고리의 빈 슬롯에 먼저 채워넣기를 시도해본다. ( 복사본에 작업한다. )
			int iNumInserted = 0;
			if( InsertToEmptySlotForTest( iCategory, iItemID, gatheringClass.GetAmountOfQuantity(), NULL, NULL, NULL, vecInventorySlotCopy, mapItemCopy, NULL, NULL, iNumInserted ) == false )
			{
				START_LOG( cerr, L"복사본의 빈 슬롯에 집어넣기 실패! 이후의 결과를 예측할 수 없습니다.")
					<< END_LOG;
				break;
			}
			gatheringClass.DecreaseTheAmountOfQuantity( iNumInserted );

			// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
			if( gatheringClass.GetAmountOfQuantity() <= 0 )
			{
				break;
			}
		}
	}

	// 합치고 남은 들어오는 동일 아이템을 수량을 합친다.
	for( TYPE_MAPQUANTITYITEM::iterator it = mapQuantityItem.begin(); it != mapQuantityItem.end(); ++it )
	{
		BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItem, it->second.GetVecInventoryItemInfo() )
		{
			bool bReady = false;
			const CXSLItem::ItemTemplet* pItemTemplate = SiCXSLItemManager()->GetItemTemplet( kInsertItem.m_kItemInfo.m_iItemID );
			if( pItemTemplate == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
					<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iItemID )
					<< END_LOG;
				continue;
			}

			if( pItemTemplate->m_Quantity <= kInsertItem.m_kItemInfo.m_iQuantity )
			{
				continue;
			}

			BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItem_Copy, it->second.GetVecInventoryItemInfo() )
			{
				const CXSLItem::ItemTemplet* pItemTemplate_Copy = SiCXSLItemManager()->GetItemTemplet( kInsertItem_Copy.m_kItemInfo.m_iItemID );
				if( pItemTemplate_Copy == NULL )
				{
					START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
						<< BUILD_LOG( kInsertItem_Copy.m_kItemInfo.m_iItemID )
						<< END_LOG;
					continue;
				}

				if( kInsertItem.m_iItemUID == kInsertItem_Copy.m_iItemUID )
				{
					bReady = true;
					continue;
				}

				// UID 가 다르고 같은 종류의 아이템 일 경우
				if( bReady == false )
					continue;
					
				// 원본 아이템의 수량이 맥스 수량보다 작을때
				if( ( kInsertItem.m_kItemInfo.m_iQuantity < pItemTemplate->m_Quantity ) && ( 0 < kInsertItem_Copy.m_kItemInfo.m_iQuantity ) )
				{
					int iDecreased = std::min<int>( pItemTemplate->m_Quantity - kInsertItem.m_kItemInfo.m_iQuantity, kInsertItem_Copy.m_kItemInfo.m_iQuantity );
					kInsertItem.m_kItemInfo.m_iQuantity			+= iDecreased;
					kInsertItem_Copy.m_kItemInfo.m_iQuantity	-= iDecreased;
				}
			}
		}
	}

	// 수량이 0이 되는 것은 지우자
	for( TYPE_MAPQUANTITYITEM::iterator it = mapQuantityItem.begin(); it != mapQuantityItem.end(); ++it )
	{
		BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItem, it->second.GetVecInventoryItemInfo() )
		{
			if( 0 < kInsertItem.m_kItemInfo.m_iQuantity )
			{
				vecInsertedItem.push_back( kInsertItem );
			}
			else
			{
				kDBUpdate.m_vecDeleted.push_back( KDeletedItemInfo(kInsertItem.m_iItemUID, KDeletedItemInfo::DR_MOVE_SLOT)  );
			}

			std::map< UidType, KInventoryItemInfo >::iterator mitItem_Quantity = mapItem_Quantity.find( kInsertItem.m_iItemUID );
			if( mitItem_Quantity != mapItem_Quantity.end() )
			{
				// DB에 업데이트 하기위해 저장
				std::map< UidType, int >::iterator mitDBItem = kDBUpdate.m_mapQuantityChange.find( kInsertItem.m_iItemUID );
				if( mitDBItem != kDBUpdate.m_mapQuantityChange.end() )
				{
					// 				START_LOG( cerr, L"벌써 들어가 있으면 안되는 건데...." )
					// 					<< BUILD_LOG( kInsertItem.m_iItemUID )
					// 					<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iQuantity )
					// 					<< END_LOG;
				}
				else
				{
					kDBUpdate.m_mapQuantityChange.insert(std::make_pair( kInsertItem.m_iItemUID, kInsertItem.m_kItemInfo.m_iQuantity - mitItem_Quantity->second.m_kItemInfo.m_iQuantity ));
				}

				std::map< UidType, KInventoryItemInfo >::iterator mitInventorySlotInfo = mapInventorySlotInfo.find( kInsertItem.m_iItemUID );
				if( mitInventorySlotInfo == mapInventorySlotInfo.end() )
				{
					mapInventorySlotInfo.insert( std::make_pair( kInsertItem.m_iItemUID, kInsertItem ) );
				}
				else
				{
					START_LOG( cerr, L"클라 갱신 아이템 벌써 들어가 있으면 안되는 건데...." )
						<< BUILD_LOG( kInsertItem.m_iItemUID )
						<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iQuantity )
						<< END_LOG;
				}
			}
			else
			{
				START_LOG( cerr, L"들어오는 아이템에 없다는 건 말이 안된다." )
					<< BUILD_LOG( kInsertItem.m_iItemUID )
					<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iQuantity )
					<< END_LOG;
			}
		}
	}

	std::map< UidType, KInventoryItemInfo >::iterator mitInventorySlotInfo = mapInventorySlotInfo.begin();
	for( ; mitInventorySlotInfo != mapInventorySlotInfo.end() ; ++mitInventorySlotInfo )
	{
		if( mitInventorySlotInfo->second.m_sSlotID >= 0 )
		{
			bool bIsExist = false;
			// 무조건 추가 하면 안된다. 나간 아이템과 겹칠 경우 갱신 되지 않는다.
			BOOST_TEST_FOREACH( KInventoryItemInfo& , kInvenItem, vecInventorySlotInfo )
			{
				if( kInvenItem.m_iItemUID == mitInventorySlotInfo->second.m_iItemUID )
				{
					kInvenItem = mitInventorySlotInfo->second;
					bIsExist = true;
					break;
				}
			}

			if( bIsExist == false )
			{
				vecInventorySlotInfo.push_back( mitInventorySlotInfo->second );
			}
		}
	}
}
#else
void KInventory::FixUpInventoryItem( IN OUT std::vector< KInventoryItemInfo >& vecInsertedItem, OUT KItemQuantityUpdate& kDBUpdate, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	kDBUpdate.m_mapQuantityChange.clear();		// 수량이 합쳐져서 증가/감소하는 것
	kDBUpdate.m_vecDeleted.clear();				// 수량이 합쳐져서 삭제되는 것
	
	// 거래시 자신에게서 나간 아이템정보가 있으므로 지우면 안된다.
	//vecInventorySlotInfo.clear();				

	std::vector< KInventoryItemInfo > vecInsertedItem_Copy = vecInsertedItem; // 실제 인설트 되어야 하는 아이템
	vecInsertedItem.clear();

	std::vector< KInventoryItemInfo > vecItem_Quantity;
	std::map< UidType, KInventoryItemInfo > mapItem_Quantity;		// DB 에 기록되어 있는 아이템의 수량을 확인하기 위해서 임시 저장
	std::map< UidType, KInventoryItemInfo > mapInventorySlotInfo;
	std::map< UidType, KInventoryItemInfo >::iterator mitInventorySlotInfo;

	BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItem, vecInsertedItem_Copy )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInsertItem.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( kInsertItem.m_iItemUID )
				<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iItemID )
				<< END_LOG;

			continue;
		}

		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			vecItem_Quantity.push_back( kInsertItem );
			mapItem_Quantity.insert( std::make_pair( kInsertItem.m_iItemUID, kInsertItem ) );
		}
		else
		{
			// 한개 품목은 무조건 슬롯하나를 차지한다.
			vecInsertedItem.push_back( kInsertItem );
		}
	}

	// 인벤토리에 있는 수량성 아이템 중에 수량이 가득차지 않은 아이템을 찾아서 합치자
	BOOST_TEST_FOREACH( KInventoryItemInfo&, kItem, vecItem_Quantity )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kItem.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( kItem.m_kItemInfo.m_iItemID )
				<< END_LOG;

			continue;
		}

		// 수량이 있는 것만
		if( kItem.m_kItemInfo.m_iQuantity <= 0 )
		{
			START_LOG( clog, L"벌써 지웠는데 왜 또?" )
				<< BUILD_LOG( kItem.m_iItemUID );
			continue;
		}

		// 초기 수량값 보존
		int iQuantityTemp = kItem.m_kItemInfo.m_iQuantity;

		// 아이템의 카테고리 정보를 얻어오자
		int iCategory = GetSuitableCategory( kItem.m_kItemInfo.m_iItemID );
		int iSlot = 0;
		bool bIsDeleted = false;
		for( ; iSlot < GetSlotSize( iCategory ) ; ++iSlot )
		{
			if( kItem.m_kItemInfo.m_iQuantity <= 0 )
				continue;

			if( kItem.m_kItemInfo.m_iItemID != GetItemID( iCategory, iSlot ) )
				continue;

			int iRemainQuantity = 0;
			iRemainQuantity = GetRemainedQuantityCapacity( iCategory, iSlot );

			// 찾은 것이 있다.
			if( iRemainQuantity > 0 )
			{
				int iIncreased = 0;
				iRemainQuantity = std::min< int >( kItem.m_kItemInfo.m_iQuantity, iRemainQuantity );
				IncreaseQuantity( iCategory, iSlot, iRemainQuantity, iIncreased );
				kItem.m_kItemInfo.m_iQuantity -= iIncreased;

				std::map< UidType, int >::iterator mitInventory = kDBUpdate.m_mapQuantityChange.find( GetItemUID(iCategory, iSlot) );
				if( mitInventory != kDBUpdate.m_mapQuantityChange.end() )
				{
					mitInventory->second += iIncreased;

					KInventoryItemInfo kTemp;
					GetInventoryItemInfo( iCategory, iSlot, kTemp );
					mitInventorySlotInfo = mapInventorySlotInfo.find( kTemp.m_iItemUID );
					if( mitInventorySlotInfo == mapInventorySlotInfo.end() )
					{
						mapInventorySlotInfo.insert( std::make_pair( kTemp.m_iItemUID, kTemp ) );
					}
					else
					{
						mitInventorySlotInfo->second.m_kItemInfo.m_iQuantity = kTemp.m_kItemInfo.m_iQuantity;
					}
				}
				else
				{
					kDBUpdate.m_mapQuantityChange.insert( std::make_pair( GetItemUID(iCategory, iSlot), iIncreased ) );

					KInventoryItemInfo kTemp;
					GetInventoryItemInfo( iCategory, iSlot, kTemp );
					mitInventorySlotInfo = mapInventorySlotInfo.find( kTemp.m_iItemUID );
					if( mitInventorySlotInfo == mapInventorySlotInfo.end() )
					{
						mapInventorySlotInfo.insert( std::make_pair( kTemp.m_iItemUID, kTemp ) );
					}
					else
					{
						mitInventorySlotInfo->second.m_kItemInfo.m_iQuantity = kTemp.m_kItemInfo.m_iQuantity;
					}
				}

				// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
				if( kItem.m_kItemInfo.m_iQuantity <= 0 )
				{
					break;
				}
			}			
		}
	}

	// 합치고 남은 들어오는 동일 아이템을 수량을 합친다.
	BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItem, vecItem_Quantity )
	{
		bool bReady = false;
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInsertItem.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iItemID )
				<< END_LOG;

			continue;;
		}

		if( kInsertItem.m_kItemInfo.m_iQuantity >= pItemTemplet->m_Quantity )
		{
			continue;
		}

		BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItem_Copy, vecItem_Quantity )
		{			
			const CXSLItem::ItemTemplet* pItemTemplet_Copy = SiCXSLItemManager()->GetItemTemplet( kInsertItem_Copy.m_kItemInfo.m_iItemID );
			if( pItemTemplet_Copy == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
					<< BUILD_LOG( kInsertItem_Copy.m_kItemInfo.m_iItemID )
					<< END_LOG;

				continue;;
			}

			if( kInsertItem.m_iItemUID == kInsertItem_Copy.m_iItemUID )
			{
				bReady = true;
				continue;
			}

			// UID 가 다르고 같은 종류의 아이템 일 경우
			if( kInsertItem.m_iItemUID != kInsertItem_Copy.m_iItemUID
			 && kInsertItem.m_kItemInfo.m_iItemID == kInsertItem_Copy.m_kItemInfo.m_iItemID
			 && bReady == true	)
			{
				// 원본 아이템의 수량이 맥스 수량보다 작을때
				if( kInsertItem.m_kItemInfo.m_iQuantity < pItemTemplet->m_Quantity 
				 && kInsertItem_Copy.m_kItemInfo.m_iQuantity > 0 )
				{
					int iDecreased = std::min< int >( pItemTemplet->m_Quantity - kInsertItem.m_kItemInfo.m_iQuantity, kInsertItem_Copy.m_kItemInfo.m_iQuantity );
					kInsertItem.m_kItemInfo.m_iQuantity += iDecreased;
					kInsertItem_Copy.m_kItemInfo.m_iQuantity -= iDecreased;
				}
			}
		}
	}
	
	std::vector<KInventoryItemInfo> vecItem_QuantityTemp = vecItem_Quantity;
	vecItem_Quantity.clear();

	// 수량이 0이 되는 것은 지우자
	BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItem, vecItem_QuantityTemp )
	{
		if( kInsertItem.m_kItemInfo.m_iQuantity > 0 )
		{
			vecInsertedItem.push_back( kInsertItem );
		}
		else
		{
			kDBUpdate.m_vecDeleted.push_back( KDeletedItemInfo(kInsertItem.m_iItemUID, KDeletedItemInfo::DR_MOVE_SLOT) );
		}

		std::map< UidType, KInventoryItemInfo >::iterator mitItem_Quantity = mapItem_Quantity.find( kInsertItem.m_iItemUID );
		if( mitItem_Quantity != mapItem_Quantity.end() )
		{
			// DB에 업데이트 하기위해 저장
			std::map< UidType, int >::iterator mitDBItem = kDBUpdate.m_mapQuantityChange.find( kInsertItem.m_iItemUID );
			if( mitDBItem != kDBUpdate.m_mapQuantityChange.end() )
			{
// 				START_LOG( cerr, L"벌써 들어가 있으면 안되는 건데...." )
// 					<< BUILD_LOG( kInsertItem.m_iItemUID )
// 					<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iQuantity )
// 					<< END_LOG;
			}
			else
			{
				kDBUpdate.m_mapQuantityChange.insert(std::make_pair( kInsertItem.m_iItemUID, kInsertItem.m_kItemInfo.m_iQuantity - mitItem_Quantity->second.m_kItemInfo.m_iQuantity ));
			}


			std::map< UidType, KInventoryItemInfo >::iterator mitInventorySlotInfo = mapInventorySlotInfo.find( kInsertItem.m_iItemUID );
			if( mitInventorySlotInfo == mapInventorySlotInfo.end() )
			{
				mapInventorySlotInfo.insert( std::make_pair( kInsertItem.m_iItemUID, kInsertItem ) );
			}
			else
			{
				START_LOG( cerr, L"클라 갱신 아이템 벌써 들어가 있으면 안되는 건데...." )
					<< BUILD_LOG( kInsertItem.m_iItemUID )
					<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iQuantity )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"들어오는 아이템에 없다는 건 말이 안된다." )
				<< BUILD_LOG( kInsertItem.m_iItemUID )
				<< BUILD_LOG( kInsertItem.m_kItemInfo.m_iQuantity )
				<< END_LOG;
		}
	}

	mitInventorySlotInfo = mapInventorySlotInfo.begin();
	for( ; mitInventorySlotInfo != mapInventorySlotInfo.end() ; ++mitInventorySlotInfo )
	{
		if( mitInventorySlotInfo->second.m_sSlotID >= 0 )
		{
			bool bIsExist = false;
			// 무조건 추가 하면 안된다. 나간 아이템과 겹칠 경우 갱신 되지 않는다.
			BOOST_TEST_FOREACH( KInventoryItemInfo& , kInvenItem, vecInventorySlotInfo )
			{
				if( kInvenItem.m_iItemUID == mitInventorySlotInfo->second.m_iItemUID )
				{
					kInvenItem = mitInventorySlotInfo->second;
					bIsExist = true;
					break;
				}
			}

			if( bIsExist == false )
			{
				vecInventorySlotInfo.push_back( mitInventorySlotInfo->second );
			}
		}
	}
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

void KInventory::IncreaseEndurance( UidType iItemUID, int iEndurance, int& iIncreased )
{
    iIncreased = 0;
    std::map< UidType, KInventoryItem >::iterator mit;

    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        START_LOG( cerr, L"해당 아이템 UID가 없음." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iEndurance )
            << END_LOG;

        return;
    }

    mit->second.IncreaseEndurance( iEndurance, iIncreased );
}
void KInventory::IncreaseEndurance( int iCategory, int iSlotID, int iEndurance, int& iIncreased )
{
    IncreaseEndurance( GetItemUID( iCategory, iSlotID ), iEndurance, iIncreased );
}

void KInventory::DecreaseEndurance( UidType iItemUID, int iEndurance, int &iDecreased )
{
    iDecreased = 0;
    std::map< UidType, KInventoryItem >::iterator mit;

    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        START_LOG( cerr, L"해당 아이템 UID가 없음." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iEndurance )
            << END_LOG;

        return;
    }

    mit->second.DecreaseEndurance( iEndurance, iDecreased );
}

void KInventory::DecreaseEndurance( int iCategory, int iSlotID, int iEndurance, int& iDecreased )
{
    DecreaseEndurance( GetItemUID( iCategory, iSlotID ), iEndurance, iDecreased );
}

void KInventory::IncreaseEnchantLevel( UidType iItemUID, int iEnchantLevel, int& iIncreased )
{
	iIncreased = 0;
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		return;
	}

	mit->second.IncreaseEnchantLevel( iEnchantLevel, iIncreased );
}

void KInventory::IncreaseEnchantLevel( int iCategory, int iSlotID, int iEnchantLevel, int& iIncreased )
{
	IncreaseEnchantLevel( GetItemUID( iCategory, iSlotID ), iEnchantLevel, iIncreased );
}

void KInventory::DecreaseEnchantLevel( UidType iItemUID, int iEnchantLevel, int &iDecreased )
{
	iDecreased = 0;
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		return;
	}

	mit->second.DecreaseEnchantLevel( iEnchantLevel, iDecreased );
}

void KInventory::DecreaseEnchantLevel( int iCategory, int iSlotID, int iEnchantLevel, int& iDecreased )
{
	DecreaseEnchantLevel( GetItemUID( iCategory, iSlotID ), iEnchantLevel, iDecreased );
}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventory::SetItemSocketOption( UidType iItemUID, const int iSocketIndex, const int iSocketOption )
{	
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iSocketIndex )
			<< BUILD_LOG( iSocketOption )
			<< END_LOG;

		return false;
	}

	return mit->second.SetItemSocketOption( iSocketIndex, iSocketOption );
}

bool KInventory::SetItemSocketOption( int iCategory, int iSlotID, const int iSocketIndex, const int iSocketOption )
{
	return SetItemSocketOption( GetItemUID( iCategory, iSlotID ), iSocketIndex, iSocketOption );
}
#else
bool KInventory::SetItemSocketOption( UidType iItemUID, const int iSocketIndex, const short sSocketOption )
{	
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iSocketIndex )
			<< BUILD_LOG( sSocketOption )
			<< END_LOG;

		return false;
	}

	return mit->second.SetItemSocketOption( iSocketIndex, sSocketOption );
}

bool KInventory::SetItemSocketOption( int iCategory, int iSlotID, const int iSocketIndex, const short sSocketOption )
{
	return SetItemSocketOption( GetItemUID( iCategory, iSlotID ), iSocketIndex, sSocketOption );
}
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 

bool KInventory::SetAttribEnchant( IN UidType iItemUID, IN char cAttribEnchantSlotID, IN char cAttribEnchantType )
{
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOGc( cAttribEnchantSlotID )
			<< BUILD_LOGc( cAttribEnchantType )
			<< END_LOG;

		return false;
	}

	return mit->second.SetAttribEnchant( cAttribEnchantSlotID, cAttribEnchantType );
}

bool KInventory::SetAttribEnchant( IN int iCategory, IN int iSlotID, IN char cAttribEnchantSlotID, IN char cAttribEnchantType )
{
	return SetAttribEnchant( GetItemUID( iCategory, iSlotID ), cAttribEnchantSlotID, cAttribEnchantType );
}

//{{ 2009. 8. 27  최육사	봉인
bool KInventory::Seal( IN UidType iItemUID )
{
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	return mit->second.Seal();
}

bool KInventory::Unseal( IN UidType iItemUID )
{
	std::map< UidType, KInventoryItem >::iterator mit;

	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	return mit->second.Unseal();
}
//}}

bool KInventory::InsertItem( IN UidType iItemUID, IN const KItemInfo& kItemInfo, OUT KInventoryItemInfo& kInventorySlotInfo )
{
    _JIF( iItemUID > 0, return false );

    std::map< UidType, KInventoryItem >::iterator mit;
    mit = m_mapItem.find( iItemUID );
    if( mit != m_mapItem.end() )
    {
        START_LOG( cerr, L"이미 존재하는 아이템 UID" )
            << BUILD_LOG( iItemUID )
            << END_LOG;

        return false;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
            << BUILD_LOG( kItemInfo.m_iItemID )
            << END_LOG;

        return false;
    }

    if( kItemInfo.m_cUsageType != pItemTemplet->m_PeriodType )
	{
		START_LOG( cerr, L"kItemInfo.m_cUsageType != pItemTemplet->m_PeriodType [ItemID] : " << kItemInfo.m_iItemID );
		return false;
	}

    switch( kItemInfo.m_cUsageType )
    {
    case CXSLItem::PT_QUANTITY:
        _JIF( kItemInfo.m_iQuantity > 0 && kItemInfo.m_iQuantity <= pItemTemplet->m_Quantity, return false );
        break;
    case CXSLItem::PT_ENDURANCE:
        _JIF( kItemInfo.m_sEndurance >= 0 && kItemInfo.m_sEndurance <= pItemTemplet->m_Endurance, return false );
        break;
    }

	if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON || pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
	{
		// 강화 레벨 검사
		_JIF( kItemInfo.m_cEnchantLevel >= (-CXSLEnchantItemManager::MAX_ENCHANT_LEVEL) && kItemInfo.m_cEnchantLevel <= CXSLEnchantItemManager::MAX_ENCHANT_LEVEL, return false );
	}

    int iCategory, iSlotID;

	//{{ 2012. 07. 18	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	if( GetEmptyCategorySlotForItem( kItemInfo.m_iItemID, iCategory, iSlotID ) == false )
	{
		return false;
	}
#else
    _JIF( GetEmptyCategorySlotForItem( kItemInfo.m_iItemID, iCategory, iSlotID ), return false );
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

    KInventoryItemInfo kInfo;
    kInfo.m_iItemUID = iItemUID;
    kInfo.m_cSlotCategory = static_cast<char>(iCategory);
	//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	kInfo.m_sSlotID = static_cast<short>(iSlotID);
#else
	kInfo.m_cSlotID = static_cast<char>(iSlotID);
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}
    kInfo.m_kItemInfo = kItemInfo;
	
	// 실제로 가지고 있는 소켓정보와 최대 소켓 슬롯 수를 비교
	if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON || pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
	{
		// 소켓 정보 검사
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
		int iSocketCount = SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType ) + kItemInfo.m_byteExpandedSocketNum;
#else // SERV_BATTLE_FIELD_BOSS
		int iSocketCount = SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType );
#endif // SERV_BATTLE_FIELD_BOSS

		//1. 템프 카피
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::vector< int > vecItemSocketTemp = kInfo.m_kItemInfo.m_vecItemSocket;
#else
		std::vector< short > vecItemSocketTemp = kInfo.m_kItemInfo.m_vecItemSocket;
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		kInfo.m_kItemInfo.m_vecItemSocket.clear();
		for( int i = 0; i < (int)vecItemSocketTemp.size(); ++i )
		{
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			int socOptionID = vecItemSocketTemp[i];
#else
			short socOptionID = vecItemSocketTemp[i];
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			if( socOptionID < 0 )
				socOptionID = 0;

			kInfo.m_kItemInfo.m_vecItemSocket.push_back( socOptionID );

			if( i >= iSocketCount )
				break;
		}
	}

    KInventoryItem kInventoryItem;
    kInventoryItem.Init( kInfo, true );

    m_mapItem.insert( std::make_pair( iItemUID, kInventoryItem ) );
    m_vecInventorySlot[iCategory][iSlotID] = iItemUID;

    LIF( GetInventorySlotInfo( iCategory, iSlotID, kInventorySlotInfo ) );

	//{{ 2012. 10. 04	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	m_kItemQuantityVariation.IncreaseQuantity( kInventorySlotInfo.m_kItemInfo.m_iItemID, kInventorySlotInfo.m_kItemInfo.m_iQuantity );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

    return true;
}

bool KInventory::InsertItem( IN const std::map< UidType, KItemInfo >& mapItem, OUT std::vector< KInventoryItemInfo >& vecInserted )
{
    vecInserted.clear();

    bool bRet = true;
    std::map< UidType, KItemInfo>::const_iterator mit;
    for( mit = mapItem.begin(); mit != mapItem.end(); ++mit )
    {
        KInventoryItemInfo kInventoryItemInfo;
        if( InsertItem( mit->first, mit->second, kInventoryItemInfo ) )
        {
            vecInserted.push_back( kInventoryItemInfo );
        }
        else
        {
            bRet = false;
        }
    }

    return bRet;
}

#ifdef SERV_SHARING_BANK_TEST
void KInventory::GetShareItemMap( OUT std::map< UidType, KInventoryItemInfo > &mapItem )
{
	mapItem.clear();

	BOOST_TEST_FOREACH( const UidType, iItemUID, m_vecInventorySlot[CXSLInventory::ST_SHARE_BANK] )
	{
		if( iItemUID <= 0 )
			continue;

		KInventoryItemInfo kInfo;
		LIF( GetInventoryItemInfo( iItemUID, kInfo ) );
		mapItem.insert( std::make_pair( iItemUID, kInfo ) );
	}
}

bool KInventory::InsertShareItem( IN const UidType iItemUID, IN const KItemInfo& kItemInfo, IN const char cCategory, IN const short sSlotID )
{
	_JIF( iItemUID > 0, return false );

	if( cCategory != CXSLInventory::ST_SHARE_BANK )
	{
		START_LOG( cerr, L"은행 공유 : 올바른 카테고리가 아님" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOGc( cCategory )
			<< END_LOG;

		return false;
	}

	int iSlotSize = static_cast<int>(m_vecInventorySlot[cCategory].size());
	if( sSlotID >= iSlotSize || sSlotID < 0 )
	{
		START_LOG( cerr, L"은행 공유 : 슬롯 ID가 들어옴" )
			<< BUILD_LOGc( cCategory )
			<< BUILD_LOG( sSlotID )
			<< BUILD_LOG( iSlotSize )
			<< END_LOG;

		return false;
	}

	std::map< UidType, KInventoryItem >::iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit != m_mapItem.end() )
	{
		START_LOG( cerr, L"이미 존재하는 아이템 UID" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( kItemInfo.m_iItemID )
			<< END_LOG;

		return false;
	}

	if( kItemInfo.m_cUsageType != pItemTemplet->m_PeriodType )
	{
		START_LOG( cerr, L"kItemInfo.m_cUsageType != pItemTemplet->m_PeriodType [ItemID] : " << static_cast<int>(kItemInfo.m_cUsageType) << " " << pItemTemplet->m_PeriodType << " " << kItemInfo.m_iItemID );
		return false;
	}

	switch( kItemInfo.m_cUsageType )
	{
	case CXSLItem::PT_QUANTITY:
		_JIF( kItemInfo.m_iQuantity > 0 && kItemInfo.m_iQuantity <= pItemTemplet->m_Quantity, return false );
		break;
	case CXSLItem::PT_ENDURANCE:
		_JIF( kItemInfo.m_sEndurance >= 0 && kItemInfo.m_sEndurance <= pItemTemplet->m_Endurance, return false );
		break;
	}

	if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON || pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
	{
		// 강화 레벨 검사
		_JIF( kItemInfo.m_cEnchantLevel >= (-CXSLEnchantItemManager::MAX_ENCHANT_LEVEL) && kItemInfo.m_cEnchantLevel <= CXSLEnchantItemManager::MAX_ENCHANT_LEVEL, return false );
	}

	KInventoryItemInfo kInfo;
	kInfo.m_iItemUID = iItemUID;
	kInfo.m_cSlotCategory = static_cast<char>(cCategory);
	kInfo.m_sSlotID = static_cast<short>(sSlotID);
	kInfo.m_kItemInfo = kItemInfo;

	// 실제로 가지고 있는 소켓정보와 최대 소켓 슬롯 수를 비교
	if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON || pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
	{
		// 소켓 정보 검사
		int iSocketCount = SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType );

		//1. 템프 카피
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::vector< int > vecItemSocketTemp = kInfo.m_kItemInfo.m_vecItemSocket;
#else
		std::vector< short > vecItemSocketTemp = kInfo.m_kItemInfo.m_vecItemSocket;
#endif SERV_ITEM_OPTION_DATA_SIZE
		kInfo.m_kItemInfo.m_vecItemSocket.clear();
		for( int i = 0; i < (int)vecItemSocketTemp.size(); ++i )
		{
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			int socOptionID = vecItemSocketTemp[i];
#else
			short socOptionID = vecItemSocketTemp[i];
#endif SERV_ITEM_OPTION_DATA_SIZE
			if( socOptionID < 0 )
				socOptionID = 0;

			kInfo.m_kItemInfo.m_vecItemSocket.push_back( socOptionID );

			if( i >= iSocketCount )
				break;
		}
	}

	KInventoryItem kInventoryItem;
	kInventoryItem.Init( kInfo, false );

	m_mapItem.insert( std::make_pair( iItemUID, kInventoryItem ) );
	m_setShareItem.insert( iItemUID );
	m_vecInventorySlot[cCategory][sSlotID] = iItemUID;

	return true;
}

bool KInventory::UpdateShareItemPosition( IN const KTradeShareItemResult& kShareResult )
{
	//	1. 아이템 검색
	std::map< UidType, KInventoryItem >::iterator mit;
	mit = m_mapItem.find( kShareResult.m_iBeforeItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"은행 공유 : DB결과 업데이트 중 아이템 정보가 없음" )
			<< BUILD_LOG( kShareResult.m_iBeforeItemUID )
			<< BUILD_LOG( kShareResult.m_iNewItemUID )
			<< END_LOG;		
		return false;
	}

	//	2. 아이템 추가하기
	KInventoryItemInfo kInfo;
	kInfo.m_iItemUID = kShareResult.m_iNewItemUID;
	kInfo.m_cSlotCategory = mit->second.GetInventoryCategory();
	kInfo.m_sSlotID = mit->second.GetSlotID();
	mit->second.GetItemInfo( kInfo.m_kItemInfo );

	KInventoryItem kInventoryItem;
	kInventoryItem.Init( kInfo, false );

	//	3. 내구도 정보, 수량 정보를 복사한다
	kInventoryItem.SetQuantityCache( mit->second.GetQuantityCache() );
	kInventoryItem.SetEnduranceCache( mit->second.GetEnduranceCache() );

	//	4. 기존 아이템은 지워주고 새로운 아이템으로 채우기
	std::set< UidType >::iterator sit;
	sit = m_setShareItem.find( kShareResult.m_iBeforeItemUID );
	if( sit != m_setShareItem.end() )
	{
		m_setShareItem.erase( sit );
		m_setShareItem.insert( kShareResult.m_iNewItemUID );
	}
	
	m_mapItem.erase( kShareResult.m_iBeforeItemUID );
	m_mapItem.insert( std::make_pair( kShareResult.m_iNewItemUID, kInventoryItem ) );
	m_vecInventorySlot[kInfo.m_cSlotCategory][kInfo.m_sSlotID] = kInfo.m_iItemUID;

	START_LOG( clog, "은행 공유 : 공유은행에 아이템 추가완료" )
		<< BUILD_LOG( kShareResult.m_iBeforeItemUID )
		<< BUILD_LOG( kInfo.m_iItemUID )
		<< BUILD_LOGc( kInfo.m_cSlotCategory )
		<< BUILD_LOG( kInfo.m_sSlotID )
		<< END_LOG;

	return true;
}

//bool KInventory::GetShareItemNeedDBUpdate( OUT KDBE_UPDATE_SHARE_ITEM_REQ &kReq )
//{
//	if( GetShareUnitUID() <= 0 )	//	공유 상태가 아니라 업데이트 할게 없음
//		return false;
//
//	std::set< UidType>::iterator		sit;
//	std::map< UidType, KInventoryItem >::iterator mit;
//
//	bool bUpdate = false;
//
//	BOOST_TEST_FOREACH( const UidType, iItemUID, m_vecInventorySlot[CXSLInventory::ST_SHARE_BANK] )
//	{
//		if( iItemUID <= 0 )
//		{
//			continue;
//		}
//
//		//	위치검색
//		mit = m_mapItem.find( iItemUID );
//		if( mit == m_mapItem.end() )
//		{
//			START_LOG( cerr, L"은행 공유 : 아이템 리스트에 없는 아이템임" )
//				<< BUILD_LOG( iItemUID )
//				<< END_LOG;
//			return false;
//		}
//
//		sit = m_setShareItem.find( iItemUID );
//		if( sit != m_setShareItem.end() )	//	아이템이 있으면
//		{
//			//	1. 슬롯 위치 변경 체크
//			if( mit->second.GetInventoryCategory() != CXSLInventory::ST_SHARE_BANK )
//			{
//				START_LOG( cerr, L"은행 공유 : 카테고리가 잘못됨" )
//					<< BUILD_LOG( iItemUID )
//					<< BUILD_LOG( mit->second.GetInventoryCategory() )
//					<< END_LOG;
//
//				return false;
//			}
//
//			if( mit->second.GetIsChangedSlotID() == true )	//	위치가 바꼈으면
//			{
//				
//#ifdef SERV_PET_SYSTEM
//				KItemPosition kItemPos(CXSLInventory::ST_BANK, mit->second.GetSlotID());
//				kReq.m_mapUpdatePosition.insert( std::make_pair( iItemUID, kItemPos));
//#else
//				int iSlotCategory	= CXSLInventory::ST_BANK;	//	은행 type으로 변경
//				int iSlotID			= mit->second.GetSlotID();
//				kReq.m_mapUpdatePosition.insert( std::make_pair( iItemUID, std::make_pair( iSlotCategory, iSlotID ) ) );
//#endif SERV_PET_SYSTEM
//
//				START_LOG( clog2, L"테스트로그: 위치 바뀐녀석있음" )
//					<< BUILD_LOG( iItemUID )
//					<< END_LOG;
//
//				bUpdate = true;
//			}
//		}
//		else	//	아이템이 없으면
//		{
//			//	2. 인벤토리 -> 공유은행 체크
//			KTradeShareItemInfo kShareInfo;
//			KItemInfo kInfo;
//			mit->second.GetItemInfo( kInfo );
//
//			kShareInfo.m_iItemUID			= iItemUID;
//			kShareInfo.m_iFromUnitUID		= m_pUser->GetCharUID();
//			kShareInfo.m_iToUnitUID			= GetShareUnitUID();
//			kShareInfo.m_cInventoryCategory	= CXSLInventory::ST_BANK;
//			kShareInfo.m_sSlotID			= mit->second.GetSlotID();
//			kShareInfo.m_ucSealCnt			= kInfo.m_ucSealData;
//			kShareInfo.m_ucDeleteReason		= KDeletedItemInfo::DR_PUT_SHARE_BANK;
//
//			kShareInfo.m_iItemID			= mit->second.GetItemID();
//
//			kReq.m_mapInItem.insert( std::make_pair( iItemUID, kShareInfo ) );
//
//			START_LOG( clog2, L"테스트로그: 아이템 들어온 녀석 있음" )
//				<< BUILD_LOG( iItemUID )
//				<< END_LOG;
//
//			bUpdate = true;
//		}
//
//		//	3. 내구도 변경 체크
//		int iEnduranceChange;
//		if( mit->second.FlushEnduranceChange( iEnduranceChange ) )
//		{
//			KUpdateShareItemInfo kUpdateInfo;
//			kUpdateInfo.m_iItemUID = mit->first;
//			kUpdateInfo.m_iUsageType = CXSLItem::PT_ENDURANCE;
//			kUpdateInfo.m_iValue = iEnduranceChange;
//
//			sit = m_setShareItem.find( kUpdateInfo.m_iItemUID );
//			if( sit != m_setShareItem.end() )
//			{
//				kUpdateInfo.m_iUnitUID = GetShareUnitUID();
//			}
//			else
//			{
//				kUpdateInfo.m_iUnitUID = m_pUser->GetCharUID();
//			}
//
//			kReq.m_mapUpdateEndurance.insert( std::make_pair( mit->first, kUpdateInfo ) );
//			bUpdate = true;
//		}
//
//		//	4. 수량 변경 체크
//		int iQuantityChange;
//		if( mit->second.FlushQuantityChange( iQuantityChange ) )
//		{
//			KUpdateShareItemInfo kUpdateInfo;
//			kUpdateInfo.m_iItemUID = mit->first;
//			kUpdateInfo.m_iUsageType = CXSLItem::PT_QUANTITY;
//			kUpdateInfo.m_iValue = iQuantityChange;
//
//			sit = m_setShareItem.find( kUpdateInfo.m_iItemUID );
//			if( sit != m_setShareItem.end() )
//			{
//				kUpdateInfo.m_iUnitUID = GetShareUnitUID();
//			}
//			else
//			{
//				kUpdateInfo.m_iUnitUID = m_pUser->GetCharUID();
//			}
//
//			kReq.m_mapUpdateQuantity.insert( std::make_pair( mit->first, kUpdateInfo ) );
//			bUpdate = true;
//		}
//	}
//
//	//	5. 공유은행 -> 인벤토리
//	BOOST_TEST_FOREACH( const UidType, iItemUID, m_setShareItem )
//	{
//		if( iItemUID <= 0 )
//		{
//			continue;
//		}
//
//		//	위치검색
//		mit = m_mapItem.find( iItemUID );
//
//		if( mit == m_mapItem.end() )
//		{
//			START_LOG( cerr, L"은행 공유 : 아이템 리스트에 없는 아이템임" )
//				<< BUILD_LOG( iItemUID )
//				<< END_LOG;
//
//			return false;
//		}
//
//		if( mit->second.GetInventoryCategory() != CXSLInventory::ST_SHARE_BANK )
//		{
//			KTradeShareItemInfo kShareInfo;
//			KItemInfo kInfo;
//			mit->second.GetItemInfo( kInfo );
//
//			kShareInfo.m_iItemUID			= iItemUID;
//			kShareInfo.m_iFromUnitUID		= GetShareUnitUID();
//			kShareInfo.m_iToUnitUID			= m_pUser->GetCharUID();
//			kShareInfo.m_cInventoryCategory	= mit->second.GetInventoryCategory();
//			kShareInfo.m_sSlotID			= mit->second.GetSlotID();
//			kShareInfo.m_ucSealCnt			= kInfo.m_ucSealData;
//			kShareInfo.m_ucDeleteReason		= KDeletedItemInfo::DR_GET_SHARE_BANK;
//
//			kShareInfo.m_iItemID			= mit->second.GetItemID();
//
//			kReq.m_mapOutItem.insert( std::make_pair( iItemUID, kShareInfo ) );
//
//			bUpdate = true;
//		}
//	}
//
//	//	삭제 될 아이템 체크
//	if( m_vecDeletedShareItem.empty() == false )
//	{
//		kReq.m_vecDeletedItem = m_vecDeletedShareItem;
//		bUpdate = true;
//	}
//
//
//	return bUpdate;
//}

bool KInventory::GetShareItemNeedDBUpdate( IN const std::set< UidType >& setMovedItemUID, OUT KDBE_UPDATE_SHARE_ITEM_REQ &kReq )
{
	if( GetShareUnitUID() <= 0 )	//	공유 상태가 아니라 업데이트 할게 없음
		return false;
	
	std::set< UidType>::iterator		sit;
	std::map< UidType, KInventoryItem >::iterator mit;

	bool bUpdate = false;

	BOOST_TEST_FOREACH( const UidType, iItemUID, m_vecInventorySlot[CXSLInventory::ST_SHARE_BANK] )
	{
		if( iItemUID <= 0 )
			continue;

		// 이동한 아이템인지 아닌지 검사
		if( setMovedItemUID.find( iItemUID ) == setMovedItemUID.end() )
			continue;

		//	위치검색
		mit = m_mapItem.find( iItemUID );
		if( mit == m_mapItem.end() )
		{
			START_LOG( cerr, L"은행 공유 : 아이템 리스트에 없는 아이템임" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			return false;
		}

		// 공유은행 카테고리에 공유 아이템이 있으면
		sit = m_setShareItem.find( iItemUID );
		if( sit != m_setShareItem.end() )	
		{
			//////////////////////////////////////////////////////////////////////////
			//	1. 슬롯 위치 변경 체크
			if( mit->second.GetInventoryCategory() != CXSLInventory::ST_SHARE_BANK )
			{
				START_LOG( cerr, L"은행 공유 : 카테고리가 잘못됨" )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOG( mit->second.GetInventoryCategory() )
					<< END_LOG;
				return false;
			}

			// 슬롯ID 위치가 바꼈으면
			if( mit->second.GetIsChangedSlotID() == true )
			{
                // DB로 데이터를 가지고 올라가니 여기서는 반영된것으로 처리해야한다.
				mit->second.PositionUpdateComplete( CXSLInventory::ST_BANK, mit->second.GetSlotID(), mit->second.GetPetUID() );

#ifdef SERV_PET_SYSTEM
				KItemPosition kItemPos( CXSLInventory::ST_BANK, mit->second.GetSlotID() );
				kReq.m_mapUpdatePosition.insert( std::make_pair( iItemUID, kItemPos ) );
#else
				int iSlotCategory	= CXSLInventory::ST_BANK;	//	은행 type으로 변경
				int iSlotID			= mit->second.GetSlotID();
				kReq.m_mapUpdatePosition.insert( std::make_pair( iItemUID, std::make_pair( iSlotCategory, iSlotID ) ) );
#endif SERV_PET_SYSTEM

				START_LOG( clog2, L"테스트로그: 위치 바뀐녀석있음" )
					<< BUILD_LOG( iItemUID )
					<< END_LOG;

				bUpdate = true;
			}
		}
		else	//	아이템이 없으면
		{
			//////////////////////////////////////////////////////////////////////////
			//	2. 인벤토리 -> 공유은행 체크
			KTradeShareItemInfo kShareInfo;
			KItemInfo kInfo;
			mit->second.GetItemInfo( kInfo );

			kShareInfo.m_iItemUID			= iItemUID;
			kShareInfo.m_iFromUnitUID		= m_pUser->GetCharUID();
			kShareInfo.m_iToUnitUID			= GetShareUnitUID();
			kShareInfo.m_cInventoryCategory	= CXSLInventory::ST_BANK;
			kShareInfo.m_sSlotID			= mit->second.GetSlotID();
			kShareInfo.m_ucSealCnt			= kInfo.m_ucSealData;
			kShareInfo.m_ucDeleteReason		= KDeletedItemInfo::DR_PUT_SHARE_BANK;

			kShareInfo.m_iItemID			= mit->second.GetItemID();

			kReq.m_mapInItem.insert( std::make_pair( iItemUID, kShareInfo ) );

			START_LOG( clog2, L"테스트로그: 아이템 들어온 녀석 있음" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			bUpdate = true;
		}

		//////////////////////////////////////////////////////////////////////////		
		//	3. 내구도 변경 체크
		int iEnduranceChange = 0;
		if( mit->second.FlushEnduranceChange( iEnduranceChange ) )
		{
			KUpdateShareItemInfo kUpdateInfo;
			kUpdateInfo.m_iItemUID = mit->first;
			kUpdateInfo.m_iUsageType = CXSLItem::PT_ENDURANCE;
			kUpdateInfo.m_iValue = iEnduranceChange;

			sit = m_setShareItem.find( kUpdateInfo.m_iItemUID );
			if( sit != m_setShareItem.end() )
			{
				kUpdateInfo.m_iUnitUID = GetShareUnitUID();
			}
			else
			{
				kUpdateInfo.m_iUnitUID = m_pUser->GetCharUID();
			}

			// DB로 데이터를 가지고 올라가니 여기서는 반영된것으로 처리해야한다.
			kReq.m_mapUpdateEndurance.insert( std::make_pair( mit->first, kUpdateInfo ) );
			bUpdate = true;
		}

		//////////////////////////////////////////////////////////////////////////		
		//	4. 수량 변경 체크
		int iQuantityChange = 0;
		if( mit->second.FlushQuantityChange( iQuantityChange ) )
		{
			KUpdateShareItemInfo kUpdateInfo;
			kUpdateInfo.m_iItemUID = mit->first;
			kUpdateInfo.m_iUsageType = CXSLItem::PT_QUANTITY;
			kUpdateInfo.m_iValue = iQuantityChange;

			sit = m_setShareItem.find( kUpdateInfo.m_iItemUID );
			if( sit != m_setShareItem.end() )
			{
				kUpdateInfo.m_iUnitUID = GetShareUnitUID();
			}
			else
			{
				kUpdateInfo.m_iUnitUID = m_pUser->GetCharUID();
			}

			// DB로 데이터를 가지고 올라가니 여기서는 반영된것으로 처리해야한다.
			kReq.m_mapUpdateQuantity.insert( std::make_pair( mit->first, kUpdateInfo ) );
			bUpdate = true;
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	//	5. 공유은행 -> 인벤토리
	BOOST_TEST_FOREACH( const UidType, iItemUID, m_setShareItem )
	{
		if( iItemUID <= 0 )
			continue;

		// 이동한 아이템인지 아닌지 검사
		if( setMovedItemUID.find( iItemUID ) == setMovedItemUID.end() )
			continue;

		//	위치검색
		mit = m_mapItem.find( iItemUID );
		if( mit == m_mapItem.end() )
		{
			START_LOG( cerr, L"은행 공유 : 아이템 리스트에 없는 아이템임" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			return false;
		}

		// 현재 위치가 공유 인벤토리가 아니라는것은 이동했다는 의미
		if( mit->second.GetInventoryCategory() != CXSLInventory::ST_SHARE_BANK )
		{
			KTradeShareItemInfo kShareInfo;
			KItemInfo kInfo;
			mit->second.GetItemInfo( kInfo );

			kShareInfo.m_iItemUID			= iItemUID;
			kShareInfo.m_iFromUnitUID		= GetShareUnitUID();
			kShareInfo.m_iToUnitUID			= m_pUser->GetCharUID();
			kShareInfo.m_cInventoryCategory	= mit->second.GetInventoryCategory();
			kShareInfo.m_sSlotID			= mit->second.GetSlotID();
			kShareInfo.m_ucSealCnt			= kInfo.m_ucSealData;
			kShareInfo.m_ucDeleteReason		= KDeletedItemInfo::DR_GET_SHARE_BANK;

			kShareInfo.m_iItemID			= mit->second.GetItemID();

			// DB로 데이터를 가지고 올라가니 여기서는 반영된것으로 처리해야한다.
			kReq.m_mapOutItem.insert( std::make_pair( iItemUID, kShareInfo ) );

			bUpdate = true;
		}
	}

	{
		std::map< UidType, KTradeShareItemInfo >::const_iterator mitTSI;

		// DB로 데이터를 가지고 올라가니 여기서는 반영된것으로 처리해야한다.
		for( mitTSI = kReq.m_mapInItem.begin(); mitTSI != kReq.m_mapInItem.end(); ++mitTSI )
		{
			m_setShareItem.insert( mitTSI->first );
		}

		// DB로 업데이트가 완료되면 더이상 공유 아이템이 아니므로 공유아이템 리스트에서 삭제한다.		
		for( mitTSI = kReq.m_mapOutItem.begin(); mitTSI != kReq.m_mapOutItem.end(); ++mitTSI )
		{
            m_setShareItem.erase( mitTSI->first );
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// 6. 삭제 될 아이템 체크
	if( m_vecDeletedShareItem.empty() == false )
	{
		// 벡터에서 지워야 하지 않을까?
		BOOST_TEST_FOREACH( const UidType, iItemUID, m_vecDeletedShareItem )
		{
			// 이동한 아이템인지 아닌지 검사
			if( setMovedItemUID.find( iItemUID ) == setMovedItemUID.end() )
				continue;

			kReq.m_vecDeletedItem.push_back( iItemUID );
			bUpdate = true;
		}

		BOOST_TEST_FOREACH( const UidType, iDeletedShareItemUID, kReq.m_vecDeletedItem )
		{
			std::vector< UidType >::iterator vitDSI;
			for( vitDSI = m_vecDeletedShareItem.begin(); vitDSI != m_vecDeletedShareItem.end(); ++vitDSI )
			{
				if( iDeletedShareItemUID != *vitDSI )
					continue;

				m_vecDeletedShareItem.erase( vitDSI );
				break;
			}			
		}
	}


	return bUpdate;
}

void KInventory::ClearShareBank()
{
	std::map< UidType, KInventoryItem >::iterator mit;

	BOOST_TEST_FOREACH( UidType&, iUnitUID, m_vecInventorySlot[CXSLInventory::ST_SHARE_BANK] )
	{
		mit = m_mapItem.find( iUnitUID );
		if( mit != m_mapItem.end() )
		{
			m_mapItem.erase( mit );
			iUnitUID = 0;
		}
	}

	int iSize = static_cast<int>( m_vecInventorySlot[CXSLInventory::ST_SHARE_BANK].size() );

	for(int i = 0; i < iSize; ++i )
	{
		m_vecInventorySlot[CXSLInventory::ST_SHARE_BANK][i] = 0;
	}
	m_setShareItem.clear();
	m_vecDeletedShareItem.clear();
	SetShareBankSize( 0 );
	SetShareUnitUID( 0 );
	SetShareNickname( L"" );
}

bool KInventory::IsShareItem( UidType iItemUID )
{
	std::set< UidType >::const_iterator sit;
	sit = m_setShareItem.find( iItemUID );

	if( sit != m_setShareItem.end() )
	{
		return true;
	}

	return false;
}
#endif SERV_SHARING_BANK_TEST

bool KInventory::DeleteItem( IN UidType iItemUID, OUT KInventoryItemInfo& kInventorySlotInfo, IN KDeletedItemInfo::DELETE_REASON eReason )
{
    std::map< UidType, KInventoryItem >::iterator mit;
    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        START_LOG( cerr, L"아이템 검색 실패. 이미 지워졌을까?" )
            << BUILD_LOG( iItemUID )
            << END_LOG;

        return false;
    }

    int iCategory, iSlotID;
    if( !GetCategorySlot( iItemUID, iCategory, iSlotID ) )
    {
        START_LOG( cerr, L"카테고리, 슬롯 ID 얻기 실패." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        return false;
    }

#ifdef SERV_SHARING_BANK_TEST
	int iSlotSize = static_cast<int>(m_vecInventorySlot[iCategory].size());
	if( iSlotID >= iSlotSize || iSlotID < 0 )
	{
		START_LOG( cerr, L"은행 공유 : 슬롯 ID가 들어옴" )
			<< BUILD_LOGc( iCategory )
			<< BUILD_LOG( iSlotID )
			<< BUILD_LOG( iSlotSize )
			<< END_LOG;

		return false;
	}
#endif SERV_SHARING_BANK_TEST

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 01. 18  최육사	오류검사
//
//	if( KDeletedItemInfo::IsAbnormalReason( eReason ) == true )
//	{
//		KInventoryItemInfo kInfo;
//		GetInventoryItemInfo( iItemUID, kInfo );
//
//		START_LOG( cerr, L"절대 일어나서는 안되는 아이템 삭제 사유!" )
//			<< BUILD_LOG( iItemUID )
//			<< BUILD_LOG( eReason )
//			<< BUILD_LOGc( kInfo.m_cSlotCategory )
//			<< BUILD_LOGc( kInfo.m_cSlotID )
//			<< BUILD_LOGc( kInfo.m_kItemInfo.m_iItemID )
//			<< BUILD_LOGc( kInfo.m_kItemInfo.m_iQuantity )
//			<< END_LOG;
//	}
//
	//}}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//{{ 2010. 01. 04  최육사	PC방 프리미엄 아이템
#ifdef SERV_INVENTORY_NEW
	if( mit->second.IsDBUpdate() == false )
	{
		START_LOG( clog, L"DB에 업데이트하지 않는 아이템은 삭제 예약 리스트에 넣지 않는다." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( mit->second.GetItemID() )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;
	}
	else
#endif SERV_INVENTORY_NEW
	{
#ifdef	SERV_SHARING_BANK_TEST
		std::set< UidType >::const_iterator sit;
		sit = m_setShareItem.find( mit->first );

		if( sit != m_setShareItem.end() )
		{
			m_vecDeletedShareItem.push_back( mit->second.GetItemUID() );
		}
		else
		{
			//{{ 2009. 12. 15  최육사	아이템 삭제사유
			m_vecDeletedItem.push_back( KDeletedItemInfo( iItemUID, eReason ) );
			//}}
		}
#else	SERV_SHARING_BANK_TEST
		//{{ 2009. 12. 15  최육사	아이템 삭제사유
		m_vecDeletedItem.push_back( KDeletedItemInfo( iItemUID, eReason ) );
		//}}
#endif	SERV_SHARING_BANK_TEST
	}
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	m_mapDeletedItem.insert( std::make_pair( mit->first, mit->second ) );
#endif SERV_GUILD_CHANGE_NAME
	//}}
	//{{ 2012. 10. 04	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	m_kItemQuantityVariation.DecreaseQuantity( mit->second.GetItemID(), mit->second.GetQuantity() );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

    m_vecInventorySlot[iCategory][iSlotID] = 0;
#ifdef	SERV_SHARING_BANK_TEST
	m_setShareItem.erase( mit->first );
#endif	SERV_SHARING_BANK_TEST
    m_mapItem.erase( mit );

    LIF( GetInventorySlotInfo( iCategory, iSlotID, kInventorySlotInfo ) );
    return true;
}

bool KInventory::DeleteItem( IN int iCategory, IN int iSlotID, OUT KInventoryItemInfo& kInventorySlotInfo, IN KDeletedItemInfo::DELETE_REASON eReason )
{
    return DeleteItem( GetItemUID( iCategory, iSlotID ), kInventorySlotInfo, eReason );
}

bool KInventory::DeleteItemOnlyInventory( IN const std::map< int, int >& mapToDelete, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, IN KDeletedItemInfo::DELETE_REASON eReason )
{
    vecUpdatedItem.clear();

    if( !IsEnoughItemExistOnlyInventory( mapToDelete ) )
    {
        return false;
    }

    std::map< int, int >::const_iterator mit;
    for( mit = mapToDelete.begin(); mit != mapToDelete.end(); ++mit )
    {
        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
        if( pItemTemplet == NULL )
        {
            START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            return false;
        }

        std::vector< KInventoryItemInfo > vecInventoryItemInfo;
        GetInventoryItemInfoContainingThisItem( mit->first, vecInventoryItemInfo, true );

        int iQuantity = mit->second;
        std::vector< KInventoryItemInfo >::iterator vit;
        if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
        {
            for( vit = vecInventoryItemInfo.begin(); vit != vecInventoryItemInfo.end(); vit++ )
            {
                if( iQuantity <= 0 )
                {
                    break;
                }

                int iDecreased;
                DecreaseQuantity( vit->m_iItemUID, iQuantity, iDecreased, eReason );
                iQuantity -= iDecreased;

                if( iDecreased > 0 )
                {
                    KInventoryItemInfo kInfo;
					//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					GetInventorySlotInfo( vit->m_cSlotCategory, vit->m_sSlotID, kInfo );
#else
					GetInventorySlotInfo( vit->m_cSlotCategory, vit->m_cSlotID, kInfo );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
                    vecUpdatedItem.push_back( kInfo );
                }
            }
        }
        else
        {
            LIF( iQuantity >= ( int )vecInventoryItemInfo.size() );
            for( vit = vecInventoryItemInfo.begin(); vit != vecInventoryItemInfo.end(); ++vit )
            {
                if( iQuantity <= 0 )
                {
                    break;
                }

                KInventoryItemInfo kInfo;
                DeleteItem( vit->m_iItemUID, kInfo, eReason );
                vecUpdatedItem.push_back( kInfo );
                iQuantity --;
            }
        }
    }

    return true;
}

//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
void KInventory::DeleteItemAll( IN int iItemID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	vecInventorySlotInfo.clear();

	std::vector< UidType > vecDeleteItemUIDList;
	GetItemUIDListThisItem( iItemID, vecDeleteItemUIDList, false );

	BOOST_TEST_FOREACH( const UidType&, iDeleteItemUID, vecDeleteItemUIDList )
	{
		KItemInfo kDeleteItemInfo;
		LIF( GetItemInfo( iDeleteItemUID, kDeleteItemInfo ) );		

		KInventoryItemInfo kInventorySlotInfo;
		LIF( DeleteItem( iDeleteItemUID, kInventorySlotInfo, KDeletedItemInfo::DR_QUEST_GIVE_UP ) );
		vecInventorySlotInfo.push_back( kInventorySlotInfo );

		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( kDeleteItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_DISCARD, kDeleteItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_DISCARD, kDeleteItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
}
#endif SERV_QUEST_CLEAR_EXPAND
//}}

//{{ 2009. 5. 13  최육사	아이템버리기
bool KInventory::DiscardItem( IN UidType iItemUID, IN int iQuantity, OUT KInventoryItemInfo& kInventorySlotInfo, IN bool bDebug /*= false*/ )
{
	SET_ERROR( NET_OK );

	KInventoryItemInfo kInfo;
	if( GetInventoryItemInfo( iItemUID, kInfo ) == false )
	{
		START_LOG( cerr, L"아이템을 지울려고 하는데 아이템이 없다?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_04 );
		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_04 );
		return false;
	}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템을 버리려고 했습니다." )
			<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_04 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	//{{ 2009. 5. 28  최육사	삭제불가능 조건
	if( bDebug == false )
	{
		if( pItemTemplet->m_PriceType == CXSLItem::SPT_NONE )
		{
			//{{ 2010. 11. 29	최육사	기간제템 삭제
#ifdef SERV_DELETE_PERIOD_ITEM
			// 기간제 아이템이지만 버릴수 있는 아이템인지 체크하자!
			if( SiCXSLItemManager()->IsForceDeleteItem( kInfo.m_kItemInfo.m_iItemID ) == false )
			{
				START_LOG( clog, L"되팔기 불능 아이템은 버릴수 없다! 클라이언트에서 체크했을텐데.." )
					<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
					<< END_LOG;

				SET_ERROR( ERR_ITEM_17 );
				return false;
			}
#else
			if( pItemTemplet->m_ItemType == CXSLItem::IT_QUEST )
			{
				START_LOG( clog, L"되팔기 불능 아이템은 버릴수 없다! 클라이언트에서 체크했을텐데.." )
					<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
					<< END_LOG;

				SET_ERROR( ERR_ITEM_17 );
				return false;
			}
#endif SERV_DELETE_PERIOD_ITEM
			//}}			
		}

		if( kInfo.m_kItemInfo.m_sPeriod > 0 )
		{
			//{{ 2010. 11. 29	최육사	기간제템 삭제
#ifdef SERV_DELETE_PERIOD_ITEM
			// 기간제 아이템이지만 버릴수 있는 아이템인지 체크하자!
			if( SiCXSLItemManager()->IsForceDeleteItem( kInfo.m_kItemInfo.m_iItemID ) == false )
			{
				START_LOG( clog, L"기간제 아이템은 버릴수 없다! 클라이언트에서 체크했을텐데.." )
					<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( kInfo.m_kItemInfo.m_sPeriod )
					<< END_LOG;

				SET_ERROR( ERR_ITEM_16 );
				return false;
			}
#endif SERV_DELETE_PERIOD_ITEM
			//}}
		}
	}	
	//}}

	// 수량성이면 수량감소
	if( kInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
	{
		// 아이템 수량 체크
		if( iQuantity == 0 )
		{
			SET_ERROR( ERR_ITEM_15 );
			return false;
		}
		else if( iQuantity < 0 )
		{
			START_LOG( cerr, L"삭제 하려는 아이템 수량값이 이상합니다. 해킹인가..?" )				
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_15 );
			return false;
		}

		int iTempQuantity = 0;
		_JIF( GetQuantity( iItemUID, iTempQuantity ), return false );

		if( iQuantity > iTempQuantity )
		{
			START_LOG( cerr, L"감소시키려는 수량만큼 실제수량이 존재하지 않는다!" )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iQuantity )
				<< BUILD_LOG( iTempQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_04 );
			return false;
		}
		else if( iQuantity == iTempQuantity )
		{
			// 존재하는 수량만큼 삭제하는거라면 바로 삭제처리
			DeleteItem( iItemUID, kInventorySlotInfo, KDeletedItemInfo::DR_DISCARD );

			KStatisticsKey kKeyES;
			kKeyES.m_vecIntKey.push_back( kInfo.m_kItemInfo.m_iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_DISCARD, iQuantity );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_DISCARD, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
			return true;
		}
		
		int iDecreased = 0;
		DecreaseQuantity( iItemUID, iQuantity, iDecreased, KDeletedItemInfo::DR_DISCARD );

		LIF( iQuantity == iDecreased );

		GetInventoryItemInfo( iItemUID, kInventorySlotInfo );

		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( kInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_DISCARD, iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_DISCARD, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
	// 아니라면 바로 삭제
	else
	{
		if( iQuantity != 1 )
		{
			START_LOG( cerr, L"삭제하려는 수량이 맞지 않다!" )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_04 );
			return false;
		}

		DeleteItem( iItemUID, kInventorySlotInfo, KDeletedItemInfo::DR_DISCARD );

		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( kInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_DISCARD, iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_DISCARD, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
	
	return true;
}
//}}



bool KInventory::UseQuickSlotItem( IN int iSlotID, OUT int& iItemID, OUT KInventoryItemInfo& kInventorySlotInfo )
{
    _JIF( iSlotID >= 0 && iSlotID < GetSlotSize( CXSLInventory::ST_E_QUICK_SLOT ), return false );
    _JIF( !IsEmptySlot( CXSLInventory::ST_E_QUICK_SLOT, iSlotID ), return false );

    iItemID = GetItemID( CXSLInventory::ST_E_QUICK_SLOT, iSlotID );

    std::map< int, KTimerManager >::iterator mit = m_mapItemCoolTime.find( iItemID );
    if( mit != m_mapItemCoolTime.end() )
    {
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
            START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
                << BUILD_LOG( iItemID )
                << END_LOG;

			return false;
		}

        if( mit->second.elapsed() < ( double )pItemTemplet->m_CoolTime )
        {
            return false;
        }
    }

    int iDecreased;
	DecreaseQuantity( CXSLInventory::ST_E_QUICK_SLOT, iSlotID, 1, iDecreased, KDeletedItemInfo::DR_USE_QUICK_SLOT );
    if( iDecreased == 1 )
    {
        if( mit != m_mapItemCoolTime.end() )
        {
            mit->second.restart();
        }
        else
        {
            KTimerManager kTimer;
            m_mapItemCoolTime.insert( std::make_pair( iItemID, kTimer ) );
        }

        GetInventorySlotInfo( CXSLInventory::ST_E_QUICK_SLOT, iSlotID, kInventorySlotInfo );

        return true;
    }
    else
    {
        START_LOG( cerr, L"아이템이 사용되지 않았다." )
            << BUILD_LOG( iDecreased )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return false;
    }
}

//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
bool KInventory::UseDefenceDungeonQuickSlotItem( IN const int iSlotID, OUT int& iItemID, OUT KInventoryItemInfo& kInventorySlotInfo )
{
	_JIF( iSlotID >= 0 && iSlotID < GetSlotSize( CXSLInventory::ST_E_DEFENCE_QUICK_SLOT ), return false );
	_JIF( !IsEmptySlot( CXSLInventory::ST_E_DEFENCE_QUICK_SLOT, iSlotID ), return false );

	iItemID = GetItemID( CXSLInventory::ST_E_DEFENCE_QUICK_SLOT, iSlotID );

	std::map< int, KTimerManager >::iterator mit = m_mapItemCoolTime.find( iItemID );
	if( mit != m_mapItemCoolTime.end() )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			return false;
		}

		if( mit->second.elapsed() < ( double )pItemTemplet->m_CoolTime )
		{
			return false;
		}
	}

	int iDecreased;
	DecreaseQuantity( CXSLInventory::ST_E_DEFENCE_QUICK_SLOT, iSlotID, 1, iDecreased, KDeletedItemInfo::DR_USE_QUICK_SLOT );
	if( iDecreased == 1 )
	{
		if( mit != m_mapItemCoolTime.end() )
		{
			mit->second.restart();
		}
		else
		{
			KTimerManager kTimer;
			m_mapItemCoolTime.insert( std::make_pair( iItemID, kTimer ) );
		}

		GetInventorySlotInfo( CXSLInventory::ST_E_DEFENCE_QUICK_SLOT, iSlotID, kInventorySlotInfo );
		return true;
	}

	
	START_LOG( cerr, L"아이템이 사용되지 않았다." )
		<< BUILD_LOG( iDecreased )
		<< BUILD_LOG( iItemID )
		<< END_LOG;
	return false;
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

void KInventory::ResetQuickSlotCoolTime()
{
	std::map< int, KTimerManager >::iterator mit;
	for( mit = m_mapItemCoolTime.begin(); mit != m_mapItemCoolTime.end(); ++mit )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
		if( pItemTemplet == NULL )
		{
			START_LOG( cwarn, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;		
			continue;
		}

		// 각 아이템에 해당되는 cooltime을 더해준다. 퀵슬롯 아이템 사용 가능 상태로 초기화
		mit->second.SetAddTime( static_cast<double>(pItemTemplet->m_CoolTime) );
	}
}

#ifdef	SERV_SHARING_BANK_TEST
#ifdef SERV_PERSONAL_SHOP_NO_MOVE
bool KInventory::MoveItem( IN UidType iItemUID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged, OUT KDBE_SEAL_ITEM_REQ &kSealReq, IN std::vector< KSellPersonalShopItemInfo > vecPersonalShopItemInfo )
#else //SERV_PERSONAL_SHOP_NO_MOVE
bool KInventory::MoveItem( IN UidType iItemUID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged, OUT KDBE_SEAL_ITEM_REQ &kSealReq )
#endif //SERV_PERSONAL_SHOP_NO_MOVE
#else	SERV_SHARING_BANK_TEST
bool KInventory::MoveItem( IN UidType iItemUID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged )
#endif	SERV_SHARING_BANK_TEST
{
    vecChanged.clear();

	// 1. 이동하려는 슬롯이 유효한지 체크!
    _JIF( VerifyCategorySlotID( iDestCategory, iDestSlotID ), return false );

	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	// 2. 이동하려는 슬롯이 펫 인벤일 경우 현재 펫이 소환된 상태인지 체크!
	const UidType iSummonedPetUID = m_pUser->GetSummonedPetUID();
	if( iSummonedPetUID == 0  &&  iDestCategory == CXSLInventory::ST_PET )
	{
		START_LOG( cerr, L"소환된 펫도 없는데 펫 인벤토리로 아이템을 이동 시키려고 한다!" )
			<< BUILD_LOG( m_pUser->GetCharUID() )
			<< END_LOG;
		return false;
	}
#ifdef SERV_EVENT_PET_INVENTORY
	///여기서 예외 처리 한다.
	std::vector< KPetInfo > TempPetInfo;
	m_pUser->GetSummonePetInfo( TempPetInfo );
	if( TempPetInfo.size() > 0 )
	{
		if( TempPetInfo[0].m_bIsEventPetID ) //소환된 놈이 이벤트 펫이라면
		{
			if( TempPetInfo[0].m_bEventFoodEat == false && iDestCategory == CXSLInventory::ST_PET )
			{
				START_LOG( cerr, L"이벤트 펫 먹이를 먹이지 않았는데 인벤이 있네?? 해킹이다" )
					<< BUILD_LOG( m_pUser->GetCharUID() )
					<< END_LOG;
				return false;
			}
		}
	}
#endif SERV_EVENT_PET_INVENTORY
#endif SERV_PET_SYSTEM
	//}}

	// 3. 이동하려는 아이템 정보 얻기
    std::map< UidType, KInventoryItem >::iterator mit;
    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        START_LOG( cerr, L"해당 아이템 UID가 없음." )
            << BUILD_LOG( iItemUID )
			<< BUILD_LOG( iDestCategory )
			<< BUILD_LOG( iDestSlotID )
            << END_LOG;

        return false;
    }

	// 4. 슬롯 정보
    int iSrcCategory, iSrcSlotID;
    if( GetCategorySlot( iItemUID, iSrcCategory, iSrcSlotID ) == false )
    {
        START_LOG( cerr, L"카테고리, 슬롯 ID 얻기 실패." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iSrcCategory )
            << BUILD_LOG( iSrcSlotID )
            << END_LOG;

        return false;
    }

	// 5. 이동하려는 위치가 현재 위치랑 똑같은지 체크
    if( ( iSrcCategory == iDestCategory ) && ( iSrcSlotID == iDestSlotID ) )
    {
        return true;
    }

	// 6. 아이템ID 얻기
    const int iItemID = GetItemID( iItemUID );

	// 7. 이동하려는 아이템이 이동하려는 슬롯에 들어갈 수 있는지 체크!
    if( IsSuitableSlot( iItemID, iDestCategory, iDestSlotID ) == false )
    {
        return false;
    }

	// 8. 이동하려는 슬롯이 장착장비 카테고리라면?
    if( iDestCategory == CXSLInventory::ST_E_EQUIP )
    {
        if( IsAbleToEquip( iItemID ) == false )
        {
            return false;
        }

		//{{ 2008. 12. 21  최육사	사용불가 검사
		if( IsBrokenItem( iItemUID ) )
		{
			START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			return false;
		}
		//}}

		//{{ 2009. 9. 8  최육사		밀봉
		if( IsSealedItem( iItemUID ) )
		{
			START_LOG( cerr, L"밀봉된 아이템. 일어날수 없는에러" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			return false;
		}
		//}}
    }
	//{{ 2009. 5. 13  최육사	실시간 인벤
	else if( iSrcCategory != CXSLInventory::ST_E_QUICK_SLOT  &&
			 iDestCategory == CXSLInventory::ST_E_QUICK_SLOT  &&
			 bCoolTimeCheck == true )
	{
		std::map< int, KTimerManager >::iterator mit = m_mapItemCoolTime.find( iItemID );
		if( mit != m_mapItemCoolTime.end() )
		{
			mit->second.restart();
		}
		else
		{
			KTimerManager kTimer;
			m_mapItemCoolTime.insert( std::make_pair( iItemID, kTimer ) );
		}
	}
	//}}

	// 9. 이동하려는 슬롯이 비어있는지 체크!
    if( IsEmptySlot( iDestCategory, iDestSlotID ) )
    {
#ifdef SERV_PERSONAL_SHOP_NO_MOVE
		if( vecPersonalShopItemInfo.empty() == false )
		{
			std::vector< KSellPersonalShopItemInfo >::const_iterator vit;

			for( vit = vecPersonalShopItemInfo.begin(); vit != vecPersonalShopItemInfo.end(); vit++ )
			{
				const int iPersonalSellItemID = GetItemID( vit->m_kInventoryItemInfo.m_iItemUID );
				if ( iPersonalSellItemID <= 0 )
					return false;

				if (iPersonalSellItemID == iItemID) // 개인 상점에 등록된 아이템이면 이동이 안된다.
					return false;

			}
		}
#endif //SERV_PERSONAL_SHOP_NO_MOVE

#ifdef	SERV_SHARING_BANK_TEST
		if(  (iSrcCategory == CXSLInventory::ST_SHARE_BANK && iDestCategory != CXSLInventory::ST_SHARE_BANK) || 
			(iSrcCategory != CXSLInventory::ST_SHARE_BANK && iDestCategory == CXSLInventory::ST_SHARE_BANK) )
		{
			if( CheckCanMoveShareItem( iItemUID, kSealReq ) == false )
				return false;
		}
#endif	SERV_SHARING_BANK_TEST
		//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		mit->second.SetPosition( iDestCategory, iDestSlotID, iSummonedPetUID );
#else
		mit->second.SetPosition( iDestCategory, iDestSlotID );
#endif SERV_PET_SYSTEM
		//}}
        m_vecInventorySlot[iDestCategory][iDestSlotID] = iItemUID;
        m_vecInventorySlot[iSrcCategory][iSrcSlotID] = 0;

        KInventoryItemInfo kInventoryItemInfoSrc;
        KInventoryItemInfo kInventoryItemInfoDest;
        GetInventorySlotInfo( iSrcCategory, iSrcSlotID, kInventoryItemInfoSrc );
        GetInventorySlotInfo( iDestCategory, iDestSlotID, kInventoryItemInfoDest );
        vecChanged.push_back( kInventoryItemInfoSrc );
        vecChanged.push_back( kInventoryItemInfoDest );

        return true;
    }
    else
    {
        UidType iDestItemUID = GetItemUID( iDestCategory, iDestSlotID );

        std::map< UidType, KInventoryItem >::iterator mit2;
        mit2 = m_mapItem.find( iDestItemUID );
        if( mit2 == m_mapItem.end() )
        {
            START_LOG( cerr, L"해당 아이템 UID가 없음." )
                << BUILD_LOG( iDestCategory )
                << BUILD_LOG( iDestSlotID )
                << BUILD_LOG( iDestItemUID )
                << END_LOG;

            return false;
        }

#ifdef SERV_PERSONAL_SHOP_NO_MOVE
		if( vecPersonalShopItemInfo.empty() == false )
		{
			std::vector< KSellPersonalShopItemInfo >::const_iterator vit;

			for( vit = vecPersonalShopItemInfo.begin(); vit != vecPersonalShopItemInfo.end(); vit++ )
			{
				const int iPersonalSellItemID = GetItemID( vit->m_kInventoryItemInfo.m_iItemUID );
				if ( iPersonalSellItemID <= 0 )
					return false;

				if (iPersonalSellItemID == GetItemID(iDestItemUID) ) // 개인 상점에 등록된 아이템이면 이동이 안된다.
					return false;

				if ( iPersonalSellItemID == iItemID)
					return false;

			}
		}
#endif //SERV_PERSONAL_SHOP_NO_MOVE

        KItemInfo kItemInfoSrc, kItemInfoDest;
        _JIF( GetItemInfo( iItemUID, kItemInfoSrc ), return false );
        _JIF( GetItemInfo( iDestItemUID, kItemInfoDest ), return false );

        if( kItemInfoSrc.m_cUsageType == CXSLItem::PT_QUANTITY && kItemInfoDest.m_cUsageType == CXSLItem::PT_QUANTITY )
        {
            if( kItemInfoSrc.m_iItemID == kItemInfoDest.m_iItemID )
            {
                int iIncreased;
                IncreaseQuantity( iDestItemUID, kItemInfoSrc.m_iQuantity, iIncreased );
                if( iIncreased > 0 )
                {
                    int iDecreased;
					DecreaseQuantity( iItemUID, iIncreased, iDecreased, KDeletedItemInfo::DR_MOVE_SLOT );
                    if( iIncreased != iDecreased )
                    {
                        START_LOG( cerr, L"수량 변화 이상." )
                            << BUILD_LOG( iItemUID )
                            << BUILD_LOG( iDestItemUID )
                            << BUILD_LOG( kItemInfoSrc.m_iItemID )
                            << BUILD_LOG( kItemInfoSrc.m_iQuantity )
                            << BUILD_LOG( iIncreased )
                            << BUILD_LOG( iDecreased )
                            << END_LOG;
                    }

                    KInventoryItemInfo kInventoryItemInfoSrc;
                    KInventoryItemInfo kInventoryItemInfoDest;
                    GetInventorySlotInfo( iSrcCategory, iSrcSlotID, kInventoryItemInfoSrc );
                    GetInventorySlotInfo( iDestCategory, iDestSlotID, kInventoryItemInfoDest );
                    vecChanged.push_back( kInventoryItemInfoSrc );
                    vecChanged.push_back( kInventoryItemInfoDest );
                }

                return true;
            }
        }

#ifdef	SERV_SHARING_BANK_TEST
		if(  (iSrcCategory == CXSLInventory::ST_SHARE_BANK && iDestCategory != CXSLInventory::ST_SHARE_BANK) || 
			(iSrcCategory != CXSLInventory::ST_SHARE_BANK && iDestCategory == CXSLInventory::ST_SHARE_BANK) )
		{
			if( CheckCanMoveShareItem( iItemUID, kSealReq ) == false )
				return false;
			if( CheckCanMoveShareItem( iDestItemUID, kSealReq ) == false )
				return false;
		}
#endif	SERV_SHARING_BANK_TEST

		// 들어갈수 없는 카테고리이기 때문에 실패처리! --> 하지만 은행이 적용되면 무척 어색하다.. 어떻게 해야할까?
        if( IsSuitableSlot( kItemInfoDest.m_iItemID, iSrcCategory, iSrcSlotID ) == false )
        {
			//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM			
			if( iDestCategory == CXSLInventory::ST_BANK  ||
				iDestCategory == CXSLInventory::ST_PET  ||
				iDestCategory == CXSLInventory::ST_E_EQUIP 
#ifdef	SERV_SHARING_BANK_TEST
				||	iSrcCategory == CXSLInventory::ST_SHARE_BANK
				||	iDestCategory == CXSLInventory::ST_SHARE_BANK
#endif	SERV_SHARING_BANK_TEST
				)
#else			
			if( iDestCategory == CXSLInventory::ST_BANK  ||
				iDestCategory == CXSLInventory::ST_E_EQUIP
#ifdef	SERV_SHARING_BANK_TEST
				||	iSrcCategory == CXSLInventory::ST_SHARE_BANK
				||	iDestCategory == CXSLInventory::ST_SHARE_BANK
#endif	SERV_SHARING_BANK_TEST
				)
#endif SERV_PET_SYSTEM
			//}}
			{
				int iEmptyCategory, iEmptySlotID;
				if( GetEmptyCategorySlotForItem( kItemInfoDest.m_iItemID, iEmptyCategory, iEmptySlotID ) == false )
				{
					// 빈슬롯이 없군.. 이러면 이동 못하지~
					return false;
				}
				
				// 이동할 아이템이 있던 자리는 비우자!
				m_vecInventorySlot[iSrcCategory][iSrcSlotID] = 0;
				KInventoryItemInfo kInventoryItemInfoSrc;
				GetInventorySlotInfo( iSrcCategory, iSrcSlotID, kInventoryItemInfoSrc );
				vecChanged.push_back( kInventoryItemInfoSrc );

				// 이동할 아이템은 목적지에 이동시키자!
				//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
				mit->second.SetPosition( iDestCategory, iDestSlotID, iSummonedPetUID );
#else
				mit->second.SetPosition( iDestCategory, iDestSlotID );
#endif SERV_PET_SYSTEM
				//}}				
				m_vecInventorySlot[iDestCategory][iDestSlotID] = iItemUID;
				KInventoryItemInfo kInventoryItemInfoDest;
				GetInventorySlotInfo( iDestCategory, iDestSlotID, kInventoryItemInfoDest );
				vecChanged.push_back( kInventoryItemInfoDest );

				// 원래 있던 아이템은 해당 인벤으로 이동시키자!
				//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
				mit2->second.SetPosition( iEmptyCategory, iEmptySlotID, iSummonedPetUID );
#else
				mit2->second.SetPosition( iEmptyCategory, iEmptySlotID );
#endif SERV_PET_SYSTEM
				//}}				
				m_vecInventorySlot[iEmptyCategory][iEmptySlotID] = iDestItemUID;
				KInventoryItemInfo kInventoryItemInfoNewDest;
				GetInventorySlotInfo( iEmptyCategory, iEmptySlotID, kInventoryItemInfoNewDest );
				vecChanged.push_back( kInventoryItemInfoNewDest );

				return true;
			}
			//}}
			/////////////////////////////////////////////////////////////////////////			

            return false;
        }

        if( iSrcCategory == CXSLInventory::ST_E_EQUIP )
        {
            if( IsAbleToEquip( kItemInfoDest.m_iItemID ) == false )
            {
                return false;
            }

			//{{ 2008. 12. 21  최육사	사용불가 검사
			if( IsBrokenItem( iDestItemUID ) == true )
			{
				return false;
			}
			//}}

			//{{ 2009. 9. 8  최육사		밀봉
			if( IsSealedItem( iDestItemUID ) == true )
			{
				return false;
			}
			//}}
        }

		//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		mit->second.SetPosition( iDestCategory, iDestSlotID, iSummonedPetUID );
#else
		mit->second.SetPosition( iDestCategory, iDestSlotID );
#endif SERV_PET_SYSTEM
		//}}        
        m_vecInventorySlot[iDestCategory][iDestSlotID] = iItemUID;
		//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		mit2->second.SetPosition( iSrcCategory, iSrcSlotID, iSummonedPetUID );
#else
		mit2->second.SetPosition( iSrcCategory, iSrcSlotID );
#endif SERV_PET_SYSTEM
		//}}        
        m_vecInventorySlot[iSrcCategory][iSrcSlotID] = iDestItemUID;

        KInventoryItemInfo kInventoryItemInfoSrc;
        KInventoryItemInfo kInventoryItemInfoDest;
        GetInventorySlotInfo( iSrcCategory, iSrcSlotID, kInventoryItemInfoSrc );
        GetInventorySlotInfo( iDestCategory, iDestSlotID, kInventoryItemInfoDest );
        vecChanged.push_back( kInventoryItemInfoSrc );
        vecChanged.push_back( kInventoryItemInfoDest );

        return true;
    }
}

#ifdef	SERV_SHARING_BANK_TEST
#ifdef SERV_PERSONAL_SHOP_NO_MOVE
bool KInventory::MoveItem( IN int iSrcCategory, IN int iSrcSlotID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged, OUT KDBE_SEAL_ITEM_REQ &kSealReq, IN std::vector< KSellPersonalShopItemInfo > vecPersonalShopItemInfo )
#else
bool KInventory::MoveItem( IN int iSrcCategory, IN int iSrcSlotID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged, OUT KDBE_SEAL_ITEM_REQ &kSealReq )
#endif SERV_PERSONAL_SHOP_NO_MOVE
#else	SERV_SHARING_BANK_TEST
bool KInventory::MoveItem( IN int iSrcCategory, IN int iSrcSlotID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged )
#endif	SERV_SHARING_BANK_TEST
{
#ifdef	SERV_SHARING_BANK_TEST
#ifdef SERV_PERSONAL_SHOP_NO_MOVE
	return MoveItem( GetItemUID( iSrcCategory, iSrcSlotID ), iDestCategory, iDestSlotID, bCoolTimeCheck, vecChanged, kSealReq, vecPersonalShopItemInfo );
#else
	return MoveItem( GetItemUID( iSrcCategory, iSrcSlotID ), iDestCategory, iDestSlotID, bCoolTimeCheck, vecChanged, kSealReq );
#endif SERV_PERSONAL_SHOP_NO_MOVE
#else	SERV_SHARING_BANK_TEST
    return MoveItem( GetItemUID( iSrcCategory, iSrcSlotID ), iDestCategory, iDestSlotID, bCoolTimeCheck, vecChanged );
#endif	SERV_SHARING_BANK_TEST
}

//{{ 2009. 6. 1  최육사		인벤토리 정렬
bool KInventory::SortInventory( IN int iCategory, OUT std::vector< KInventoryItemInfo >& vecChanged )
{
	vecChanged.clear();

	if( GetFirstCategory() < iCategory  ||  GetSentinelCategory() >= iCategory )
	{
		START_LOG( cerr, L"이상한 카테고리입니다!" )
			<< BUILD_LOG( iCategory )
			<< END_LOG;

		return false;
	}

	std::vector< int > vecEmptySlotInfo;

	for( int i = 0; i < GetSlotSize( iCategory ); ++i )
	{
		// 1. 빈슬롯인지 검사
		if( IsEmptySlot( iCategory, i ) )
		{
			vecEmptySlotInfo.push_back( i );
			continue;
		}

		// 2. 빈슬롯 정보가 없으면 통과 ( 아이템이 붙어있는 상황임, 아이템 이동시킬 필요 없음 )
		if( vecEmptySlotInfo.empty() )
			continue;

		// 3. 빈슬롯 정보가 있다면 그 슬롯으로 이동시킨다
		int iDestSlotID = vecEmptySlotInfo.front();

		std::vector< KInventoryItemInfo > vecMoveItem;
		//if( MoveItem( iCategory, i, iCategory, iDestSlotID, vecMoveItem ) )
		//{
		//	START_LOG( cerr, L"인벤토리 정렬 실패!" )
		//		<< BUILD_LOG( iCategory )
		//		<< END_LOG;
		//	continue;
		//}

		// 5. 빈슬롯 정보 초기화
		vecEmptySlotInfo.erase( vecEmptySlotInfo.begin() );

		// 6. 원래 있던 슬롯은 빈슬롯이 되었으므로 빈슬롯 리스트에 추가
		vecEmptySlotInfo.push_back( i );
	}

	for( int i = 0; i < GetSlotSize( iCategory ); ++i )
	{
		KInventoryItemInfo kInfo;
		if( GetInventorySlotInfo( iCategory, i, kInfo ) == false )
		{
			START_LOG( cerr, L"인벤토리 정보 얻기 실패!" )
				<< BUILD_LOG( iCategory )
				<< BUILD_LOG( i )
				<< END_LOG;

			continue;
		}
        
		vecChanged.push_back( kInfo );
	}

	return true;
}
//}}

//{{ 2011. 06. 18	최육사	ED아이템 구매 체크
//#ifdef SERV_CHECK_BUY_ED_ITEM
bool KInventory::PrepareBuy( IN const std::map< int, int >& mapItem, IN const bool bDebug, IN const short sPeriod, OUT int& iTotalED, OUT int& iTotalPvPPoint, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, OUT std::vector< KBuyGPItemInfo >& vecNewItem )
{
	SET_ERROR( NET_OK );

    iTotalED = 0;
    iTotalPvPPoint = 0;
    vecUpdatedItem.clear();
    vecNewItem.clear();	
    
    std::map< int, int > mapItemQuantity = mapItem;

	// 구매에 필요한 ED와 VS Point를 충분히 가지고 있는지 확인한다.
	if( !m_pUser )
	{
		SET_ERROR( ERR_BUY_ED_ITEM_05 );
		return false;
	}

	{
		// 가격을 계산한다.
		std::map< int, int >::const_iterator mit;
		for( mit = mapItem.begin(); mit != mapItem.end(); ++mit )
		{
			const int iBuyItemID = mit->first;
			const int iBuyQuantity = mit->second;

			if( iBuyItemID <= 0 )
			{
				START_LOG( cerr, L"구매하려는 아이템ID가 이상함" )
					<< BUILD_LOG( iBuyItemID )
					<< BUILD_LOG( iBuyQuantity )
					<< END_LOG;

				SET_ERROR( ERR_BUY_ED_ITEM_05 );
				return false;
			}

			//{{ 2011. 09. 07	최육사	아이템 구매 예외 처리
#ifdef SERV_NPC_BUY_ITEM_BUG_FIX
			if( iBuyQuantity <= 0 )
			{
				START_LOG( cerr, L"사려는 수량이 이상함" )
					<< BUILD_LOG( iBuyItemID )
					<< BUILD_LOG( iBuyQuantity )
					<< END_LOG;

				SET_ERROR( ERR_BUY_ED_ITEM_05 );
				return false;
			}
#endif SERV_NPC_BUY_ITEM_BUG_FIX
			//}}

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iBuyItemID );
			if( pItemTemplet == NULL )
			{
				if( bDebug )
				{
					START_LOG( cout, L"Item ID가 아이템 템플릿에 존재하지 않음. 영자님 치트좀 제대로 쓰세염" )
						<< BUILD_LOG( iBuyItemID )
						<< BUILD_LOG( bDebug )
						<< END_LOG;
				}
				else
				{
					START_LOG( cerr, L"Item ID가 아이템 템플릿에 존재하지 않음." )
						<< BUILD_LOG( iBuyItemID )
						<< BUILD_LOG( bDebug )
						<< END_LOG;
				}

				SET_ERROR( ERR_BUY_ED_ITEM_05 );
				return false;
			}

			if( !bDebug )
			{
				// NPC상점에서 파는 아이템을 구매하려고 했는지 검사!
				if( SiCXSLItemManager()->IsNpcSellEDItem( iBuyItemID ) == false )
				{
                    START_LOG( cerr, L"NPC상점에서 팔지 않는 아이템을 구매하려 하였습니다! 해킹유저로 의심 됩니다!" )						
						<< BUILD_LOG( iBuyItemID )
						<< BUILD_LOG( iBuyQuantity )
						<< END_LOG;

					SET_ERROR( ERR_BUY_ED_ITEM_06 );
					return false;
				}

				//{{ 2011. 07. 27	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
				if( pItemTemplet->m_BuyPvpRankCondition != CXSLUnit::PVPRANK_NONE )
				{
					if( pItemTemplet->m_BuyPvpRankCondition > m_pUser->GetPvpRank() )
					{
						SET_ERROR( ERR_BUY_ED_ITEM_07 );
						return false;
					}
				}				
#endif SERV_PVP_NEW_SYSTEM
				//}}

				//{{ 2011. 09. 07	최육사	아이템 구매 예외 처리
//#ifdef SERV_NPC_BUY_ITEM_BUG_FIX
//				if( pItemTemplet->m_PeriodType == CXSLItem::PERIOD_TYPE::PT_QUANTITY )
//				{
//					if( pItemTemplet->m_Quantity < iBuyQuantity )
//					{
//						START_LOG( cerr, L"수량성 아이템인데 한번에 구매가능한 수량 이상을 사려고 한다! 해킹인가?" )
//							<< BUILD_LOG( iBuyItemID )
//							<< BUILD_LOG( pItemTemplet->m_Quantity )
//							<< BUILD_LOG( iBuyQuantity )
//							<< END_LOG;
//
//						SET_ERROR( ERR_BUY_ED_ITEM_05 );
//						return false;
//					}
//				}
//#endif SERV_NPC_BUY_ITEM_BUG_FIX
				//}}
			}

			if( pItemTemplet->m_Price < 0 )
			{
				SET_ERROR( ERR_BUY_ED_ITEM_05 );
				return false;
			}

			if( pItemTemplet->m_PricePvPPoint < 0 )
			{
				SET_ERROR( ERR_BUY_ED_ITEM_05 );
				return false;
			}

			iTotalED += bDebug ? 0 : pItemTemplet->m_Price * iBuyQuantity;
			iTotalPvPPoint += bDebug ? 0 : pItemTemplet->m_PricePvPPoint * iBuyQuantity;
		}
	}
		
	// 구매 ED 검사
	if( ( m_pUser->GetED() < iTotalED ) || ( iTotalED < 0 ) )
    {
		START_LOG( cerr, L"총 구매 금액이 마이너스다. 해킹 유저인가?" )
			<< BUILD_LOG( m_pUser->GetUID() )
			<< BUILD_LOG( m_pUser->GetCharUID() )
			<< BUILD_LOG( m_pUser->GetED() )
			<< BUILD_LOG( iTotalED )
			<< END_LOG;

		SET_ERROR( ERR_BUY_ED_ITEM_05 );
        return false;
    }

	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( ( m_pUser->GetAPoint() < iTotalPvPPoint ) || ( iTotalPvPPoint < 0 ) )
#else
	if( m_pUser->GetVSPoint() < iTotalPvPPoint )
#endif SERV_PVP_NEW_SYSTEM
	//}}    
    {
		START_LOG( cerr, L"총 구매 금액이 마이너스다. 해킹 유저인가?" )
			<< BUILD_LOG( m_pUser->GetUID() )
			<< BUILD_LOG( m_pUser->GetCharUID() )
			<< BUILD_LOG( m_pUser->GetAPoint() )
			<< BUILD_LOG( iTotalPvPPoint )
			<< END_LOG;

		SET_ERROR( ERR_BUY_ED_ITEM_05 );
        return false;
    }

    // 인벤토리에 충분한 공간이 있는지 확인한다.
    if( IsEnoughSpaceExist( mapItemQuantity ) == false )
    {
		SET_ERROR( ERR_BUY_ED_ITEM_05 );
        return false;
    }

	//{{ 2012. 07. 3	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	// 임시로 채우기를 하기 위한 카테고리 슬롯 정보의 복사본입니다.
	std::map< UidType, KInventoryItem >	mapItemCopy;
	std::vector< UidType >				vecInventorySlotCopy[CXSLInventory::ST_END];

	for( int i=0; i < CXSLInventory::ST_END; ++i )
	{
		vecInventorySlotCopy[i].resize( m_vecInventorySlot[i].size() );
		std::copy( m_vecInventorySlot[i].begin(), m_vecInventorySlot[i].end(), vecInventorySlotCopy[i].begin() );
	}
	std::copy( m_mapItem.begin(), m_mapItem.end(), std::inserter( mapItemCopy, mapItemCopy.begin() ) );

	std::map< int, int >::iterator mit;
	for( mit = mapItemQuantity.begin(); mit != mapItemQuantity.end(); ++mit )
	{
		const int& iItemID = mit->first;
		int& iQuantity	= mit->second;

		const CXSLItem::ItemTemplet* pItemTemplate = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplate == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}

		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( iItemID, vecCategoryID );

		// 카테고리를 순회하면서 수량을 채워넣습니다.
		BOOST_TEST_FOREACH( const int&, iCategory, vecCategoryID )
		{
			if( IsValidCategory( iCategory, iItemID ) == false )
			{
				continue;
			}

			if( pItemTemplate->m_PeriodType == CXSLItem::PT_QUANTITY )
			{
				int iNumIncreased = 0;

				// 원본에 채워넣기
				if( FillTheUnFullSlot( iCategory, iItemID, iQuantity, m_vecInventorySlot, m_mapItem, NULL, NULL, &vecUpdatedItem, iNumIncreased ) == false )
				{		
					START_LOG( cerr, L"채워넣기 실패! 이후의 결과를 예측할 수 없습니다.")
						<< END_LOG;
					continue;
				}
				iQuantity -= iNumIncreased;

				// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
				if( iQuantity <= 0 )
				{
					break;
				}
			}

			// 남은 수량이 있다면, 현재 카테고리의 빈 슬롯에 먼저 채워넣기를 시도해본다. ( 복사본에 작업한다. )
			int iNumInserted = 0;
			if( InsertToEmptySlotForTest( iCategory, iItemID, iQuantity, NULL, &bDebug, &sPeriod, vecInventorySlotCopy, mapItemCopy, NULL, &vecNewItem, iNumInserted ) == false )
			{
				START_LOG( cerr, L"복사본의 빈 슬롯에 집어넣기 실패! 이후의 결과를 예측할 수 없습니다.")
					<< END_LOG;
				break;
			}
			iQuantity -= iNumInserted;

			// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
			if( iQuantity <= 0 )
			{
				break;
			}
		}
	}
#else
	{
		// 새 UID를 만들지 않고 수량을 채울 수 있는 것이 있으면 채운다.
		std::map< int, int >::iterator mit;
		for( mit = mapItemQuantity.begin(); mit != mapItemQuantity.end(); ++mit )
		{
			const int iCategory = GetSuitableCategory( mit->first );
			if( iCategory < CXSLInventory::ST_EQUIP  ||  iCategory > CXSLInventory::ST_AVARTA )
			{
				continue;
			}

			for( int iSlotID = 0; iSlotID < GetSlotSize( iCategory ); ++iSlotID )
			{
				if( IsEmptySlot( iCategory, iSlotID ) )
				{
					continue;
				}

				if( GetItemID( iCategory, iSlotID ) != mit->first )
					continue;

				int iIncreased;
				IncreaseQuantity( iCategory, iSlotID, mit->second, iIncreased );
				mit->second -= iIncreased;

				if( iIncreased > 0 )
				{
					KInventoryItemInfo kInventoryItemInfo;
					LIF( GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo ) );
					vecUpdatedItem.push_back( kInventoryItemInfo );
				}

				if( mit->second <= 0 )
				{
					break;
				}				
			}
		}
	}    
    
	{
		// 남은 아이템을 DB에 신규 아이템으로 생성하기 위해 단위 수량씩 나누어 KBuyGPItemInfo에 담는다.
		std::map< int, int >::iterator mit;
		for( mit = mapItemQuantity.begin(); mit != mapItemQuantity.end(); ++mit )
		{
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;

				continue;
			}

			int iQuantity;
			int iMaxQuantity;
			if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
			{
				iMaxQuantity = pItemTemplet->m_Quantity;
			}
			else
			{
				iMaxQuantity = 1;
			}

			if( iMaxQuantity <= 0 )
			{
				START_LOG( cerr, L"수량 이상." )
					<< BUILD_LOG( iMaxQuantity )
					<< END_LOG;

				continue;
			}

			while( mit->second > 0 )
			{
				iQuantity = std::min< int >( mit->second, iMaxQuantity );

				// 구매한 아이템에 대한 초기값을 설정.
				KBuyGPItemInfo kInfo;
				kInfo.m_iItemID = mit->first;
				kInfo.m_iPrice = bDebug ? 0 : pItemTemplet->m_Price * iQuantity;
				kInfo.m_iPvPPoint = bDebug ? 0 : pItemTemplet->m_PricePvPPoint * iQuantity;
				kInfo.m_cPeriodType = pItemTemplet->m_PeriodType;
				kInfo.m_iQuantity = iQuantity;
				if( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE )
				{
					kInfo.m_sEndurance = pItemTemplet->m_Endurance;
				}
				else
				{
					kInfo.m_sEndurance = 0;
				}

				//{{ 2008. 4. 24  최육사   기간제 아이템의 경우 기간 설정
				if( kInfo.m_cPeriodType == CXSLItem::PT_INFINITY  &&  bDebug )
				{
					kInfo.m_sPeriod = sPeriod;
				}
				//}}

				mit->second -= iQuantity;
				vecNewItem.push_back( kInfo );
			}
		}
	}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

    return true;
}
//#endif SERV_CHECK_BUY_ED_ITEM
//}}

bool KInventory::ProcessSell( IN const UidType iItemUID, IN const int iQuantity, OUT int& iItemID, OUT int& iED, OUT KInventoryItemInfo& kInventoryItemInfo )
{
    iItemID = 0;
    iED = 0;

	if( !m_pUser )
	{
		START_LOG( cerr, L"유저 포인터 이상." )
			<< END_LOG;

		return false;
	}	

    // 아이템의 존재 여부 확인.
    if( IsExist( iItemUID ) == false )
    {
        START_LOG( cerr, L"존재하지 않는 아이템 팔기 시도." )
			<< BUILD_LOG( m_pUser->GetCharUID() )
            << BUILD_LOG( iItemUID )
            << END_LOG;

        return false;
    }

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템을 상점에 팔려고 했습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

    int iCategory, iSlotID;
    _JIF( GetCategorySlot( iItemUID, iCategory, iSlotID ), return false );
    iItemID = GetItemID( iItemUID );

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( m_pUser->GetCharUID() )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return false;
    }

    // 팔기 불능
    if( pItemTemplet->m_PriceType == CXSLItem::SPT_NONE )
    {
        START_LOG( cerr, L"팔기 불능 아이템." )
			<< BUILD_LOG( m_pUser->GetCharUID() )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return false;
    }

#ifdef SERV_IMPOSSIBLE_SELL_ITEM_ACCESSORY
	if( pItemTemplet->m_PriceType == CXSLItem::SPT_GP && pItemTemplet->m_ItemType == CXSLItem::IT_ACCESSORY )
	{
		KInventoryItemInfo kInfo;
		GetInventoryItemInfo(iItemUID, kInfo );

		if ( kInfo.m_kItemInfo.m_sPeriod != 0 )
		{
			START_LOG( cerr, L"팔기 불능 아이템." )
				<< BUILD_LOG( m_pUser->GetCharUID() )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			return false;
		}

	}
#endif // SERV_IMPOSSIBLE_SELL_ITEM_ACCESSORY

    // 충분한 수량이 있는지 확인.
    if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
    {
		//{{ 2010. 03. 18  최육사	클라이언트 데이터 검증
		// 정상적인 수량값인지 검증
		if( iQuantity <= 0  ||  iQuantity > pItemTemplet->m_Quantity )
		{
			START_LOG( cerr, L"비정상적인 수량값입니다. 해킹인가..?" )
				<< BUILD_LOG( m_pUser->GetUID() )
				<< BUILD_LOG( m_pUser->GetCharUID() )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iQuantity )
				<< BUILD_LOG( pItemTemplet->m_Quantity )
				<< END_LOG;

			return false;
		}
		//}}

        int iCurrentQuantity;
        _JIF( GetQuantity( iItemUID, iCurrentQuantity ), return false );
        if( iCurrentQuantity < iQuantity )
        {
            START_LOG( cerr, L"보유 수량보다 더 많은 수량 팔기 시도" )
                << BUILD_LOG( iItemUID )
                << BUILD_LOG( iItemID )
                << BUILD_LOG( iQuantity )
                << BUILD_LOG( iCurrentQuantity )
                << END_LOG;

            return false;
        }
    }
	
	//{{ 2011. 01. 20	최육사	아이템 분해 개편
#ifdef SERV_ITEM_RESOLVE_NEW
	if( GetSellPrice( iItemUID, iQuantity, iED ) == false )
	{
		START_LOG( cerr, L"해당 아이템의 되팔기 가격을 얻지 못했습니다! 되팔기 실패!" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;
		return false;
	}
#else
	//// 돌려받는 가격 계산.
	///************************************************************************/
	///* <<< 내구도 아이템 되팔기 계산 공식 >>>
	//// 수리비용 : [상점판매가격 * 0.2 * {(최대내구도 - 현재내구도) / (최대내구도 * 3)}]
	//// 돌려받는 가격 : [상점판매가격 * 0.2 - 수리비용]
	///************************************************************************/

 //   switch( pItemTemplet->m_PeriodType )
 //   {
 //   case CXSLItem::PT_INFINITY:
 //       iED = ( int )floorf( 0.2f * ( float )pItemTemplet->m_Price + 0.01f );
 //       break;
 //   case CXSLItem::PT_ENDURANCE:
 //       {
 //           float fOriginalPrice;
 //           float fRepairPrice;
 //           int iEndurance;
 //           _JIF( pItemTemplet->m_Endurance > 0, return false );
 //           _JIF( GetEndurance( iItemUID, iEndurance ), return false );
 //           _JIF( iEndurance >= 0, return false );
 //           _JIF( pItemTemplet->m_Endurance >= iEndurance, return false );
	//	    fOriginalPrice = ( float )pItemTemplet->m_Price; // 판매가
	//        fRepairPrice = ( float )pItemTemplet->m_RepairED * ( float )GetEnduranceToRestore( iItemUID );
	//	    iED = ( int )floorf( ( ( float )fOriginalPrice * 0.2f ) - fRepairPrice + 0.01f );
 //       }
 //       break;
 //   case CXSLItem::PT_QUANTITY:
	//	//////////////////////////////////////////////////////////////////////////
	//	// 2010-03-18  최육사
	//	// 아래 코드와 관련하여 홍콩에서 해킹으로 인한 ED버그 획득 이슈가 있었음.
	//	// 첫번째 문제는 클라이언트가 보내는 iQuantity값이 유효한 값인지 검사를 하지 않는 문제였고
	//	// 두번째 문제는 int타입을 float타입으로 타입캐스팅하면서 마이너스(-)부호가 소멸하여
	//	// 발생하는 문제였음.

 //       _JIF( pItemTemplet->m_Quantity > 0, return false );
 //       iED = ( int )floorf( 0.2f * ( float )pItemTemplet->m_Price * ( float )iQuantity + 0.01f );
 //       break;
 //   default:
 //       START_LOG( cerr, L"이상한 아이템 기간 타입." )
 //           << BUILD_LOG( pItemTemplet->m_PeriodType )
 //           << END_LOG;
 //       return false;
 //   }

	////{{ 2007. 11. 22  최육사  판매가 교정
	//if( iED < 0 )
	//{
	//	START_LOG( cwarn, L"장비템 수리 안하고 팔았나?" )
	//		<< BUILD_LOG( iItemID )
	//		<< BUILD_LOG( iQuantity )
	//		<< BUILD_LOG( pItemTemplet->m_PeriodType )
	//		<< BUILD_LOG( pItemTemplet->m_Endurance )
	//		<< BUILD_LOG( pItemTemplet->m_Price )
	//		<< BUILD_LOG( pItemTemplet->m_RepairED )
	//		<< BUILD_LOG( iED )
	//		<< END_LOG;

	//	// 0 ED로 판매가 수정
	//	iED = 0;
	//}
	//}}
#endif SERV_ITEM_RESOLVE_NEW
	//}}

	//{{ 2009. 4. 28  최육사	최대ED체크	
	if( m_pUser->CheckMaxED( iED ) == false )
	{
		START_LOG( cerr, L"아이템 판매시 최대ED를 초과함! 클라에서 체크했을텐데.." )
			<< BUILD_LOG( m_pUser->GetED() )
			<< BUILD_LOG( iED )
			<< END_LOG;

		return false;
	}
	//}}

    // 아이템 제거.
    if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
    {
        int iDecreased;
		DecreaseQuantity( iItemUID, iQuantity, iDecreased, KDeletedItemInfo::DR_SELL_TO_NPC );

        LIF( iDecreased == iQuantity );
    }
    else
    {
        DeleteItem( iItemUID, kInventoryItemInfo, KDeletedItemInfo::DR_SELL_TO_NPC );
    }

    LIF( GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo ) );

    return true;
}

//{{ 2012. 07. 8	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
bool KInventory::PrepareInsert( std::map< int, KItemInfo >& mapReward, std::map< int, int >& mapReceived, std::vector< KInventoryItemInfo >& vecUpdatedItem, std::vector< KItemInfo >& vecNewItem, bool bTransaction /*= false*/ )
{
	mapReceived.clear();
	vecUpdatedItem.clear();
	vecNewItem.clear();

	std::map< int, KItemInfo > mapRewardCopy = mapReward;
	
	// 임시로 채우기를 하기 위한 카테고리 슬롯 정보의 복사본입니다.
	std::map< UidType, KInventoryItem >	mapItemCopy;
	std::vector< UidType >				vecInventorySlotCopy[CXSLInventory::ST_END];

	for( int i=0; i < CXSLInventory::ST_END; ++i )
	{
		vecInventorySlotCopy[i].resize( m_vecInventorySlot[i].size() );
		std::copy( m_vecInventorySlot[i].begin(), m_vecInventorySlot[i].end(), vecInventorySlotCopy[i].begin() );
	}
	std::copy( m_mapItem.begin(), m_mapItem.end(), std::inserter( mapItemCopy, mapItemCopy.begin() ) );

	std::map< int, KItemInfo >::iterator mitReward;
	// 새 UID를 만들지 않고 수량을 채울 수 있는 것이 있으면 채운다.
	for( mitReward = mapRewardCopy.begin(); mitReward != mapRewardCopy.end(); ++mitReward )
	{
		_JIF( mitReward->second.m_iQuantity > 0, continue );

		const int iItemID	= mitReward->first;
		int& iQuantity		= mitReward->second.m_iQuantity;

		const CXSLItem::ItemTemplet* pItemTemplate = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplate == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			return false;
		}

		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( iItemID, vecCategoryID );

		// 카테고리를 순회하면서 수량을 채워넣습니다.
		BOOST_TEST_FOREACH( const int&, iCategory, vecCategoryID )
		{
			if( IsValidCategory( iCategory, iItemID ) == false )
			{
				continue;
			}

			if( pItemTemplate->m_PeriodType == CXSLItem::PT_QUANTITY )
			{
				int iNumIncreased = 0;

				// 원본에 채워넣기
				if( FillTheUnFullSlot( iCategory, iItemID, iQuantity, m_vecInventorySlot, m_mapItem, NULL, NULL, &vecUpdatedItem, iNumIncreased ) == false )
				{
					// 트랜잭션 동작 -> 롤백처리
					if( bTransaction == true )
					{
						goto err_proc;
					}
					else
					{
						START_LOG( cerr, L"채워넣기 실패! 이후의 결과를 예측할 수 없습니다.")
							<< END_LOG;
						continue;
					}
				}
				
				if( iNumIncreased != 0 )
				{
					iQuantity -= iNumIncreased;

					std::map< int, int >::iterator mitReceived = mapReceived.find( iItemID );
					if( mitReceived != mapReceived.end() )
					{
						mitReceived->second += iNumIncreased;
					}
					else
					{
						mapReceived.insert( std::make_pair( iItemID, iNumIncreased ) );
					}
				}

				// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
				if( iQuantity <= 0 )
				{
					break;
				}
			}

			// 남은 수량이 있다면, 현재 카테고리의 빈 슬롯에 먼저 채워넣기를 시도해본다. ( 복사본에 작업한다. )
			int iNumInserted = 0;
			if( InsertToEmptySlotForTest( iCategory, iItemID, iQuantity, &mitReward->second, NULL, NULL, vecInventorySlotCopy, mapItemCopy, &vecNewItem, NULL, iNumInserted ) == false )
			{
				// 트랜잭션 동작 -> 롤백처리
				if( bTransaction == true )
				{
					goto err_proc;
				}
				else
				{
					START_LOG( cerr, L"복사본의 빈 슬롯에 집어넣기 실패! 이후의 결과를 예측할 수 없습니다.")
						<< END_LOG;
					break;
				}
			}

			if( iNumInserted != 0 )
			{
				iQuantity -= iNumInserted;

				std::map< int, int >::iterator mitReceived = mapReceived.find( iItemID );
				if( mitReceived != mapReceived.end() )
				{
					mitReceived->second += iNumInserted;
				}
				else
				{
					mapReceived.insert( std::make_pair( iItemID, iNumInserted ) );
				}
			}

			// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
			if( iQuantity <= 0 )
			{
				break;
			}
		}

		// 아직도 남은 수량이 있다면, 뭔가 잘못된 것이다.
		if( 0 < iQuantity )
		{
			// 트랜잭션 동작 -> 롤백처리
			if( bTransaction == true )
			{
				goto err_proc;
			}
		}
	}

	return true;

err_proc:
	// 실제로 PrepareInsert가 실패하여 롤백처리를 하더라도, 이후 작업을 시도하기 위해 DB로 이동한다.
	// 비록 삽입처리에 대한 실제적인 삽입이 이루어지지 않은 채로, 삽입 시도한 만큼 롤백하지만,
	// 현재 루틴상에서 결과적으로는 문제될 것은 없다.

	// 트랜잭션 동작 -> 롤백처리
	if( bTransaction )
	{		
		vecUpdatedItem.clear(); // 업데이트될 인벤정보 초기화

		int iCategory, iSlotID;
		std::map< int, int >::iterator mitRB;
		for( mitRB = mapReceived.begin(); mitRB != mapReceived.end(); ++mitRB )
		{
			_JIF( mitRB->second > 0, continue );
			// 아이템의 카테고리 정보를 얻어오자

			const int iItemID = mitRB->first;
			std::vector<int> vecCategoryID;
			GetSuitableCategory( iItemID, vecCategoryID );

			for( size_t i=vecCategoryID.size(); 0 < i; --i )
			{
				iCategory = vecCategoryID[i-1];
				if( IsValidCategory( iCategory ) == false )
				{
					START_LOG( cerr, L"롤백시키기 위해 카테고리 검사하던중 이상한 카테고리 발견" )
						<< BUILD_LOG( mitRB->first )
						<< BUILD_LOG( mitRB->second )
						<< BUILD_LOG( iCategory )
						<< END_LOG;
					continue;
				}

				for( iSlotID = 0; iSlotID < GetSlotSize( iCategory ); ++iSlotID )
				{
					if( IsEmptySlot( iCategory, iSlotID ) )
					{
						continue;
					}

					if( GetItemID( iCategory, iSlotID ) == mitRB->first )
					{
						int iDecreased;
						DecreaseQuantity( iCategory, iSlotID, mitRB->second, iDecreased, KDeletedItemInfo::DR_PREPARE_INSERT );
						if( iDecreased <= 0 )
						{
							START_LOG( cerr, L"증가된 만큼 감소(롤백)시켰는데 뭔가 이상하다." )
								<< BUILD_LOG( mitRB->first )
								<< BUILD_LOG( mitRB->second )
								<< BUILD_LOG( iCategory )
								<< BUILD_LOG( iSlotID )
								<< END_LOG;
							continue;
						}

						// 감소시킨 만큼 뺀다.
						mitRB->second -= iDecreased;

						if( mitRB->second <= 0 )
						{
							break;
						}
					}
				}
			}
		}
	}
	return false;
}
#else
bool KInventory::PrepareInsert( std::map< int, KItemInfo >& mapReward, std::map< int, int >& mapReceived, std::vector< KInventoryItemInfo >& vecUpdatedItem, std::vector< KItemInfo >& vecNewItem, bool bTransaction /*= false*/ )
{
	mapReceived.clear();
	vecUpdatedItem.clear();
	vecNewItem.clear();

    int iCategory, iSlotID;
    std::map< int, KItemInfo > mapRewardCopy = mapReward;
    std::map< int, KItemInfo >::iterator mitReward;
    std::map< int, int >::iterator mitReceived;
    // 새 UID를 만들지 않고 수량을 채울 수 있는 것이 있으면 채운다.
    for( mitReward = mapRewardCopy.begin(); mitReward != mapRewardCopy.end(); ++mitReward )
    {
        _JIF( mitReward->second.m_iQuantity > 0, continue );
        iCategory = GetSuitableCategory( mitReward->first );
        if( iCategory < CXSLInventory::ST_EQUIP  ||  iCategory > CXSLInventory::ST_AVARTA )
        {
            continue;
        }

        for( iSlotID = 0; iSlotID < GetSlotSize( iCategory ); iSlotID++ )
        {
            if( IsEmptySlot( iCategory, iSlotID ) )
            {
                continue;
            }

            if( GetItemID( iCategory, iSlotID ) == mitReward->first )
            {
                int iIncreased;
                IncreaseQuantity( iCategory, iSlotID, mitReward->second.m_iQuantity, iIncreased );
                if( iIncreased <= 0 )
                {
                    continue;
                }

                mitReward->second.m_iQuantity -= iIncreased;
                mitReceived = mapReceived.find( mitReward->first );
                {
                    if( mitReceived != mapReceived.end() )
                    {
                        mitReceived->second += iIncreased;
                    }
                    else
                    {
                        mapReceived.insert( std::make_pair( mitReward->first, iIncreased ) );
                    }
                }

                if( iIncreased > 0 )
                {
                    KInventoryItemInfo kInventoryItemInfo;
                    GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
                    vecUpdatedItem.push_back( kInventoryItemInfo );
                }

                if( mitReward->second.m_iQuantity <= 0 )
                {
                    break;
                }
            }
        }
    }

    // 인벤토리의 빈 칸 개수를 센다.
    std::map< int, int > mapEmptySlot;
    GetNumFreeSlot( mapEmptySlot );

    // 남은 아이템을 DB에 신규 아이템으로 생성하기 위해 단위 수량씩 나누에 KItemInfo에 담는다.
    for( mitReward = mapRewardCopy.begin(); mitReward != mapRewardCopy.end(); mitReward++ )
    {
        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mitReward->first );
        if( pItemTemplet == NULL )
        {
            START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
                << BUILD_LOG( mitReward->first )
                << END_LOG;

            continue;
        }

		int iQuantity = ( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY ? pItemTemplet->m_Quantity : 1 );
        if( iQuantity <= 0 )
        {
            START_LOG( cerr, L"수량 이상." )
                << BUILD_LOG( mitReward->first )
                << BUILD_LOG( iQuantity )
                << END_LOG;

            continue;
        }

		short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
		if( sEndurance < 0 )
		{
			START_LOG( cerr, L"내구도 이상." )
                << BUILD_LOG( mitReward->first )
				<< BUILD_LOG( sEndurance )
				<< END_LOG;

			continue;
		}

        char cEnchantLevel = static_cast<char>(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL);

        while( mitReward->second.m_iQuantity > 0 )
        {
            std::map< int, int >::iterator mitEmptySlot;
            mitEmptySlot = mapEmptySlot.find( GetSuitableCategory( mitReward->first ) );
            if( mitEmptySlot == mapEmptySlot.end() )
            {
				// 트랜잭션 동작 -> 롤백처리
				if( bTransaction ) goto err_proc; else break;
            }

            if( mitEmptySlot->second <= 0 )
            {
				// 트랜잭션 동작 -> 롤백처리
				if( bTransaction ) goto err_proc; else break;
            }

            iQuantity = std::min< int >( mitReward->second.m_iQuantity, iQuantity );

            KItemInfo kInfo;
            kInfo.m_iItemID			= mitReward->first;
            kInfo.m_cUsageType		= pItemTemplet->m_PeriodType;
            kInfo.m_iQuantity		= iQuantity;
            kInfo.m_sEndurance		= std::min< short >( mitReward->second.m_sEndurance, sEndurance );
            kInfo.m_sPeriod			= mitReward->second.m_sPeriod;
            kInfo.m_cEnchantLevel	= std::min< char >( mitReward->second.m_cEnchantLevel, cEnchantLevel );
			kInfo.m_kAttribEnchantInfo = mitReward->second.m_kAttribEnchantInfo;
            kInfo.m_vecItemSocket	= mitReward->second.m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			kInfo.m_byteExpandedSocketNum	= mitReward->second.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
			//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			kInfo.m_vecRandomSocket	= mitReward->second.m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
			//{{ 2009. 9. 7  최육사		밀봉
			kInfo.m_ucSealData		= mitReward->second.m_ucSealData;
			//}}
            vecNewItem.push_back( kInfo );

            mitReward->second.m_iQuantity -= iQuantity;
            mitEmptySlot->second--;

            mitReceived = mapReceived.find( mitReward->first );
            if( mitReceived != mapReceived.end() )
            {
                mitReceived->second += iQuantity;
            }
            else
            {
                mapReceived.insert( std::make_pair( mitReward->first, iQuantity ) );
            }
        }
    }

    return true;
err_proc:

	// 트랜잭션 동작 -> 롤백처리
	if( bTransaction )
	{		
		vecUpdatedItem.clear(); // 업데이트될 인벤정보 초기화

		int iCategory, iSlotID;
		std::map< int, int >::iterator mitRB;
		for( mitRB = mapReceived.begin(); mitRB != mapReceived.end(); ++mitRB )
		{
			_JIF( mitRB->second > 0, continue );
			iCategory = GetSuitableCategory( mitRB->first );
			if( iCategory < CXSLInventory::ST_EQUIP  ||  iCategory > CXSLInventory::ST_AVARTA )
			{
				START_LOG( cerr, L"롤백시키기 위해 카테고리 검사하던중 이상한 카테고리 발견" )
					<< BUILD_LOG( mitRB->first )
					<< BUILD_LOG( mitRB->second )
					<< BUILD_LOG( iCategory )
					<< END_LOG;
				continue;
			}

			for( iSlotID = 0; iSlotID < GetSlotSize( iCategory ); ++iSlotID )
			{
				if( IsEmptySlot( iCategory, iSlotID ) )
				{
					continue;
				}

				if( GetItemID( iCategory, iSlotID ) == mitRB->first )
				{
					int iDecreased;
					DecreaseQuantity( iCategory, iSlotID, mitRB->second, iDecreased, KDeletedItemInfo::DR_PREPARE_INSERT );
					if( iDecreased <= 0 )
					{
						START_LOG( cerr, L"증가된 만큼 감소(롤백)시켰는데 뭔가 이상하다." )
							<< BUILD_LOG( mitRB->first )
							<< BUILD_LOG( mitRB->second )
							<< BUILD_LOG( iCategory )
							<< BUILD_LOG( iSlotID )
							<< END_LOG;
						continue;
					}

					// 감소시킨 만큼 뺀다.
					mitRB->second -= iDecreased;

					if( mitRB->second <= 0 )
					{
						break;
					}
				}
			}
		}
	}
	return false;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}

//{{ 2009. 3. 16  최육사	수량성이 아닌 아이템에 대한 prepareinsert
bool KInventory::PrepareInsertForTrade( IN const std::vector< KInventoryItemInfo >& vecReward, OUT std::map< int, int >& mapReceived, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, OUT std::vector< KItemInfo >& vecNewItem )
{
	mapReceived.clear();
	vecUpdatedItem.clear();
	vecNewItem.clear();

	std::vector< KInventoryItemInfo >::const_iterator vitR;

	//////////////////////////////////////////////////////////////////////////
	// 개인거래 아이템들	
	std::vector< KItemInfo > vecRewardOnlyOne;	

	// 수량성 템만 따로
	std::map< int, KItemInfo >::iterator mitRQ;
	std::map< int, KItemInfo > mapRewardQuantity;
	
	// 1. 수량성과 수량성 아닌것을 분리한다
	for( vitR = vecReward.begin(); vitR != vecReward.end(); ++vitR )
	{
		if( vitR->m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			mitRQ = mapRewardQuantity.find( vitR->m_kItemInfo.m_iItemID );
			if( mitRQ == mapRewardQuantity.end() )
			{
				mapRewardQuantity.insert( std::make_pair( vitR->m_kItemInfo.m_iItemID, vitR->m_kItemInfo ) );
			}
			else
			{
				mitRQ->second.m_iQuantity += vitR->m_kItemInfo.m_iQuantity;
			}
		}
		else
		{
            vecRewardOnlyOne.push_back( vitR->m_kItemInfo );
		}
	}

	// 2. 수량성 아이템을 먼저 처리함
	if( !mapRewardQuantity.empty() )
	{
        if( !PrepareInsert( mapRewardQuantity, mapReceived, vecUpdatedItem, vecNewItem, false ) )
		{
			START_LOG( cerr, L"개인거래중 prepareInsert 처리 실패! 일어날수 없는 에러!" )
				<< BUILD_LOG( mapRewardQuantity.size() )
				<< END_LOG;
		}
	}

	// 3. 인벤토리의 빈 칸 개수를 센다.
	std::map< int, int >::iterator mitReceived;
	std::map< int, int > mapEmptySlot;
	GetNumFreeSlot( mapEmptySlot );

	//{{ 2012. 07. 9	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	// 4. 남은 아이템을 DB에 신규 아이템으로 생성하기 위해 단위 수량씩 나누에 KItemInfo에 담는다.
	for( std::vector< KItemInfo >::iterator vitROO = vecRewardOnlyOne.begin(); vitROO != vecRewardOnlyOne.end(); ++vitROO )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( vitROO->m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< END_LOG;

			continue;
		}

		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			START_LOG( cerr, L"수량성은 여기 올수 없는데? 있을수 없는 에러!" )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< END_LOG;

			continue;
		}

		if( vitROO->m_iQuantity > 1 )
		{
			START_LOG( cerr, L"수량성이 아닌데 수량이 1개가 넘는다?" )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< BUILD_LOG( vitROO->m_iQuantity )
				<< END_LOG;

			vitROO->m_iQuantity = 1; // 수량을 1개로 수정
		}

		short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
		if( sEndurance < 0 )
		{
			START_LOG( cerr, L"내구도 이상." )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< BUILD_LOG( sEndurance )
				<< END_LOG;

			continue;
		}

		char cEnchantLevel = static_cast<char>(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL);

		while( vitROO->m_iQuantity > 0 )
		{
			// 아이템의 카테고리 정보를 얻어오자
			std::vector<int> vecCategoryID;
			GetSuitableCategory( vitROO->m_iItemID, vecCategoryID );

			BOOST_TEST_FOREACH( const int&, iCategory, vecCategoryID )
			{
				std::map< int, int >::iterator mitEmptySlot;
				mitEmptySlot = mapEmptySlot.find( iCategory );
				if( mitEmptySlot == mapEmptySlot.end() )
				{
					continue;
				}

				if( mitEmptySlot->second <= 0 )
				{
					continue;
				}

				KItemInfo kInfo;
				kInfo.m_iItemID			= vitROO->m_iItemID;
				kInfo.m_cUsageType		= pItemTemplet->m_PeriodType;
				kInfo.m_iQuantity		= 1; // 수량성이 아니므로 무조건 한개임
				kInfo.m_sEndurance		= std::min< int >( vitROO->m_sEndurance, sEndurance );
				kInfo.m_sPeriod			= vitROO->m_sPeriod;
				kInfo.m_cEnchantLevel	= std::min< char >( vitROO->m_cEnchantLevel, cEnchantLevel );
				kInfo.m_kAttribEnchantInfo = vitROO->m_kAttribEnchantInfo;
				kInfo.m_vecItemSocket	= vitROO->m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
				kInfo.m_byteExpandedSocketNum	= vitROO->m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
				//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				kInfo.m_vecRandomSocket	= vitROO->m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
				//}}
				//{{ 2009. 9. 7  최육사		밀봉
				kInfo.m_ucSealData		= vitROO->m_ucSealData;
				//}}
				vecNewItem.push_back( kInfo );

				// 생성할 아이템 수량 감소, 빈슬롯 감소
				--vitROO->m_iQuantity;
				--mitEmptySlot->second;

				mitReceived = mapReceived.find( vitROO->m_iItemID );
				if( mitReceived != mapReceived.end() )
				{
					++mitReceived->second;
				}
				else
				{
					mapReceived.insert( std::make_pair( vitROO->m_iItemID, 1 ) );
				}
			}

			if( 0 < vitROO->m_iQuantity )
			{
				START_LOG( cerr, L"어라? 빈공간을 미리 계산했을텐데.." )
					<< BUILD_LOG( vitROO->m_iItemID )
					<< BUILD_LOG( vitROO->m_iQuantity )
					<< END_LOG;
				break;
			}
		}
	}
#else
	// 4. 남은 아이템을 DB에 신규 아이템으로 생성하기 위해 단위 수량씩 나누에 KItemInfo에 담는다.
	for( std::vector< KItemInfo >::iterator vitROO = vecRewardOnlyOne.begin(); vitROO != vecRewardOnlyOne.end(); ++vitROO )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( vitROO->m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< END_LOG;

			continue;
		}

		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			START_LOG( cerr, L"수량성은 여기 올수 없는데? 있을수 없는 에러!" )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< END_LOG;

			continue;
		}

		if( vitROO->m_iQuantity > 1 )
		{
			START_LOG( cerr, L"수량성이 아닌데 수량이 1개가 넘는다?" )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< BUILD_LOG( vitROO->m_iQuantity )
				<< END_LOG;

			vitROO->m_iQuantity = 1; // 수량을 1개로 수정
		}

		short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
		if( sEndurance < 0 )
		{
			START_LOG( cerr, L"내구도 이상." )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< BUILD_LOG( sEndurance )
				<< END_LOG;

			continue;
		}

		char cEnchantLevel = static_cast<char>(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL);

		while( vitROO->m_iQuantity > 0 )
		{
			std::map< int, int >::iterator mitEmptySlot;
			mitEmptySlot = mapEmptySlot.find( GetSuitableCategory( vitROO->m_iItemID ) );
			if( mitEmptySlot == mapEmptySlot.end() )
			{
				START_LOG( cerr, L"어라? 빈공간을 미리 계산했을텐데.." )
					<< BUILD_LOG( vitROO->m_iItemID )
					<< BUILD_LOG( vitROO->m_iQuantity )
					<< END_LOG;
				break;
			}

			if( mitEmptySlot->second <= 0 )
			{
				START_LOG( cerr, L"어라? 빈공간을 미리 계산했을텐데.." )
					<< BUILD_LOG( vitROO->m_iItemID )
					<< BUILD_LOG( vitROO->m_iQuantity )
					<< END_LOG;
				break;
			}

			KItemInfo kInfo;
			kInfo.m_iItemID			= vitROO->m_iItemID;
			kInfo.m_cUsageType		= pItemTemplet->m_PeriodType;
			kInfo.m_iQuantity		= 1; // 수량성이 아니므로 무조건 한개임
			kInfo.m_sEndurance		= std::min< int >( vitROO->m_sEndurance, sEndurance );
			kInfo.m_sPeriod			= vitROO->m_sPeriod;
			kInfo.m_cEnchantLevel	= std::min< char >( vitROO->m_cEnchantLevel, cEnchantLevel );
			kInfo.m_kAttribEnchantInfo = vitROO->m_kAttribEnchantInfo;
			kInfo.m_vecItemSocket	= vitROO->m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			kInfo.m_byteExpandedSocketNum	= vitROO->m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
			//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			kInfo.m_vecRandomSocket	= vitROO->m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
			//{{ 2009. 9. 7  최육사		밀봉
			kInfo.m_ucSealData		= vitROO->m_ucSealData;
			//}}
			vecNewItem.push_back( kInfo );

			// 생성할 아이템 수량 감소, 빈슬롯 감소
			--vitROO->m_iQuantity;
			--mitEmptySlot->second;

			mitReceived = mapReceived.find( vitROO->m_iItemID );
			if( mitReceived != mapReceived.end() )
			{
				++mitReceived->second;
			}
			else
			{
				mapReceived.insert( std::make_pair( vitROO->m_iItemID, 1 ) );
			}
		}
	}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

	return true;
}
//}}

bool KInventory::DeleteAndInsert( IN const std::map< int, int >& mapToDelete
								, IN const std::map< int, KItemInfo >& mapToInsert
								, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem
								, OUT std::vector< KItemInfo >& vecNewItem
								, IN bool bDeleteOnlyInventory
								, IN KDeletedItemInfo::DELETE_REASON eReason
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
								, IN const bool bOnlyInfinite /*= false	*/	// 기한 제한이 없는 것
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
								)
{
    std::map< UidType, int > mapDummy;
    return DeleteAndInsert( mapToDelete
		, mapDummy
		, mapToInsert
		, vecUpdatedItem
		, vecNewItem
		, bDeleteOnlyInventory
		, eReason
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
		, bOnlyInfinite
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		);
}

//{{ 2012. 07. 9	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
bool KInventory::DeleteAndInsert( IN const std::map< UidType, int >& mapToDelete, IN const std::map< int, KItemInfo >& mapToInsert, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, OUT std::vector< KItemInfo >& vecNewItem, IN KDeletedItemInfo::DELETE_REASON eReason )
{
	std::map< UidType, int >::const_iterator mitToDelete;
	std::map< int, KItemInfo >::const_iterator mitToInsert;

	for( mitToDelete = mapToDelete.begin(); mitToDelete != mapToDelete.end(); ++mitToDelete )
	{
		_JIF( mitToDelete->first > 0, return false );
		_JIF( mitToDelete->second > 0, return false );
	}

	for( mitToInsert = mapToInsert.begin(); mitToInsert != mapToInsert.end(); ++mitToInsert )
	{
		_JIF( mitToInsert->first > 0, return false );
		_JIF( mitToInsert->second.m_iQuantity > 0, return false );
	}

	vecUpdatedItem.clear();
	vecNewItem.clear();

	// 지울 아이템을 가지고 있는지 조사한다.
	_JIF( IsEnoughItemExist( mapToDelete ), return false );

	// 인벤토리에 현재 비어 있는 슬롯들을 조사한다.
	std::map< int, int > mapNumEmptySlot;
	GetNumFreeSlot( mapNumEmptySlot );

	// 아이템을 제거할 경우 빈 공간이 더 생기는지 조사한다.
	for( mitToDelete = mapToDelete.begin(); mitToDelete != mapToDelete.end(); ++mitToDelete )
	{
		std::map< UidType, KInventoryItem >::iterator mit;
		mit = m_mapItem.find( mitToDelete->first );
		if( mit == m_mapItem.end() )
		{
			continue;
		}

		int iQuantity;
		_JIF( GetQuantity( mitToDelete->first, iQuantity ), return false );

		if( mitToDelete->second >= iQuantity )
		{
			int iCategory;
			int iSlotID;

			_JIF( GetCategorySlot( mitToDelete->first, iCategory, iSlotID ), return false );
			std::map< int, int >::iterator mitEmptySlot;
			mitEmptySlot = mapNumEmptySlot.find( iCategory );
			if( mitEmptySlot != mapNumEmptySlot.end() )
			{
				++mitEmptySlot->second;
			}
			else
			{
				mapNumEmptySlot.insert( std::make_pair( iCategory, 1 ) );
			}
		}
	}

	// 인벤토리에 공간이 충분한지 확인한다.
	std::map< int, KItemInfo > mapToInsertClone = mapToInsert;
	std::map< int, int > mapNumEmptySlotClone = mapNumEmptySlot;
	std::map< int, KItemInfo >::iterator mitToInsertClone;
	std::map< int, int >::iterator mitNumEmptySlotClone;

	for( mitToInsertClone = mapToInsertClone.begin(); mitToInsertClone != mapToInsertClone.end(); ++mitToInsertClone )
	{
		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( mitToInsertClone->first, vecCategoryID );

		BOOST_TEST_FOREACH( int, iCategory, vecCategoryID )
		{
			if( IsValidCategory( iCategory ) == false )
			{
				START_LOG( cerr, L"인벤토리 카테고리 이상." )
					<< BUILD_LOG( mitToInsertClone->first )
					<< BUILD_LOG( iCategory )
					<< END_LOG;
				return false;
			}

			for( int iSlot = 0; iSlot < GetSlotSize( iCategory ); ++iSlot )
			{
				if( IsEmptySlot( iCategory, iSlot ) )
				{
					continue;
				}

				if( GetItemID( iCategory, iSlot ) == mitToInsertClone->first )
				{
					mitToInsertClone->second.m_iQuantity -= min( mitToInsertClone->second.m_iQuantity, GetRemainedQuantityCapacity( iCategory, iSlot ) );
				}
			}
		}
	}

	for( mitToInsertClone = mapToInsertClone.begin(); mitToInsertClone != mapToInsertClone.end(); ++mitToInsertClone )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mitToInsertClone->first );
		if( !pItemTemplet )
		{
			START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
				<< BUILD_LOG( mitToInsertClone->first )
				<< END_LOG;

			return false;
		}

		int iRequiredNumSlot = 0;
		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			if( mitToInsertClone->second.m_iQuantity > 0 )
			{
				if( pItemTemplet->m_Quantity <= 0 )
				{
					START_LOG( cerr, L"수량 제한 값이 이상함." )
						<< BUILD_LOG( pItemTemplet->m_Quantity )
						<< END_LOG;

					return false;
				}
				iRequiredNumSlot = mitToInsertClone->second.m_iQuantity / pItemTemplet->m_Quantity;
				if( ( mitToInsertClone->second.m_iQuantity % pItemTemplet->m_Quantity ) > 0 )
				{
					++iRequiredNumSlot;
				}
			}
		}
		else
		{
			_JIF( mitToInsertClone->second.m_iQuantity >= 0, return false );
			iRequiredNumSlot = mitToInsertClone->second.m_iQuantity;
		}

		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( mitToInsertClone->first, vecCategoryID );

		BOOST_TEST_FOREACH( int, iCategory, vecCategoryID )
		{
			if( IsValidCategory( iCategory ) == false )
			{
				START_LOG( cerr, L"인벤토리 카테고리 이상." )
					<< BUILD_LOG( mitToInsertClone->first )
					<< BUILD_LOG( iCategory )
					<< END_LOG;
				return false;
			}

			mitNumEmptySlotClone = mapNumEmptySlotClone.find( iCategory );
			if( mitNumEmptySlotClone == mapNumEmptySlotClone.end() )
			{
				continue;
			}

			if( mitNumEmptySlotClone->second < iRequiredNumSlot )
			{
				iRequiredNumSlot -= mitNumEmptySlotClone->second;
				continue;
			}
			iRequiredNumSlot = 0;
			break;
		}

		if( 0 < iRequiredNumSlot )
			return false;
	}

	// 모든 검사가 끝났고 이제부터 인벤토리에 변화가 생기므로 이 이후부터는 함부로 false를 리턴하면 안된다.
	// Remove시킬 아이템을 깎는다.
	for( mitToDelete = mapToDelete.begin(); mitToDelete != mapToDelete.end(); ++mitToDelete )
	{
		int iItemID = GetItemID( mitToDelete->first );
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			continue;
		}

		int iCategory;
		int iSlotID;
		LIF( GetCategorySlot( mitToDelete->first, iCategory, iSlotID ) );
		KInventoryItemInfo kInventoryItemInfo;
		int iUsageType;
		_JIF( GetUsageType( mitToDelete->first, iUsageType ), continue );
		if( iUsageType == CXSLItem::PT_QUANTITY )
		{
			int iDecreased;
			DecreaseQuantity( mitToDelete->first, mitToDelete->second, iDecreased, eReason );
			LIF( mitToDelete->second == iDecreased );
			GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
		}
		else
		{
			LIF( mitToDelete->second == 1 );
			DeleteItem( mitToDelete->first, kInventoryItemInfo, eReason );
		}
		vecUpdatedItem.push_back( kInventoryItemInfo );
	}

	// 새 UID를 만들지 않고 수량을 채울 수 있는 것이 있으면 채운다.
	
	// 임시로 채우기를 하기 위한 카테고리 슬롯 정보의 복사본입니다.
	std::map< UidType, KInventoryItem >	mapItemCopy;
	std::vector< UidType >				vecInventorySlotCopy[CXSLInventory::ST_END];

	for( int i=0; i < CXSLInventory::ST_END; ++i )
	{
		vecInventorySlotCopy[i].resize( m_vecInventorySlot[i].size() );
		std::copy( m_vecInventorySlot[i].begin(), m_vecInventorySlot[i].end(), vecInventorySlotCopy[i].begin() );
	}
	std::copy( m_mapItem.begin(), m_mapItem.end(), std::inserter( mapItemCopy, mapItemCopy.begin() ) );

	mapToInsertClone = mapToInsert;
	for( mitToInsertClone = mapToInsertClone.begin(); mitToInsertClone != mapToInsertClone.end(); ++mitToInsertClone )
	{
		const int iItemID	= mitToInsertClone->first;
		int& iQuantity		= mitToInsertClone->second.m_iQuantity;

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}

		int iMaxQuantity = ( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY ? pItemTemplet->m_Quantity : 1 );
		if( iMaxQuantity <= 0 )
		{
			START_LOG( cerr, L"수량 이상." )
				<< BUILD_LOG( iMaxQuantity )
				<< END_LOG;
			continue;
		}

		short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
		if( sEndurance < 0 )
		{
			START_LOG( cerr, L"내구도 이상." )
				<< BUILD_LOG( sEndurance )
				<< END_LOG;
			continue;
		}

		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( iItemID, vecCategoryID );

		BOOST_TEST_FOREACH( int, iCategory, vecCategoryID )
		{
			if( IsValidCategory( iCategory ) == false )
			{
				START_LOG( cerr, L"인벤토리 카테고리 이상." )
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( iCategory )
					<< END_LOG;
				continue;
			}

			if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
			{
				// 원본에 채워넣기
				int iNumIncreased = 0;
				if( FillTheUnFullSlot( iCategory, iItemID, iQuantity, m_vecInventorySlot, m_mapItem, NULL, NULL, &vecUpdatedItem, iNumIncreased ) == false )
				{
					START_LOG( cerr, L"채워넣기 실패! 이후의 결과를 예측할 수 없습니다.")
						<< END_LOG;
					continue;
				}
				iQuantity -= iNumIncreased;

				if( iQuantity <= 0 )
				{
					break;
				}
			}

			// 남은 아이템을 DB에 신규 아이템으로 생성하기 위해 단위 수량씩 나누에 KItemInfo에 담는다.
			int iNumInserted = 0;
			if( InsertToEmptySlotForTest( iCategory, iItemID, iQuantity, &mitToInsertClone->second, NULL, NULL, vecInventorySlotCopy, mapItemCopy, &vecNewItem, NULL, iNumInserted ) == false )
			{
				START_LOG( cerr, L"복사본의 빈 슬롯에 집어넣기 실패! 이후의 결과를 예측할 수 없습니다.")
					<< END_LOG;
				break;
			}

			iQuantity -= iNumInserted;

			if( iQuantity <= 0 )
			{
				break;
			}
		}

		if( 0 < iQuantity )
		{
			START_LOG( cerr, L"[중요] 수량을 전부 채우지 못하였습니다. 이후의 결과를 예측할 수 없습니다.")
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;
		}
	}

	return true;
}
#else
bool KInventory::DeleteAndInsert( IN const std::map< UidType, int >& mapToDelete, IN const std::map< int, KItemInfo >& mapToInsert, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, OUT std::vector< KItemInfo >& vecNewItem, IN KDeletedItemInfo::DELETE_REASON eReason )
{
    std::map< UidType, int >::const_iterator mitToDelete;
    std::map< int, KItemInfo >::const_iterator mitToInsert;

    for( mitToDelete = mapToDelete.begin(); mitToDelete != mapToDelete.end(); ++mitToDelete )
    {
        _JIF( mitToDelete->first > 0, return false );
        _JIF( mitToDelete->second > 0, return false );
    }

    for( mitToInsert = mapToInsert.begin(); mitToInsert != mapToInsert.end(); ++mitToInsert )
    {
        _JIF( mitToInsert->first > 0, return false );
        _JIF( mitToInsert->second.m_iQuantity > 0, return false );
    }

    vecUpdatedItem.clear();
    vecNewItem.clear();

    // 지울 아이템을 가지고 있는지 조사한다.
    _JIF( IsEnoughItemExist( mapToDelete ), return false );

    // 인벤토리에 현재 비어 있는 슬롯들을 조사한다.
    std::map< int, int > mapNumEmptySlot; // category, number of slot
    GetNumFreeSlot( mapNumEmptySlot );

    // 아이템을 제거할 경우 빈 공간이 더 생기는지 조사한다.
    for( mitToDelete = mapToDelete.begin(); mitToDelete != mapToDelete.end(); ++mitToDelete )
    {
        std::map< UidType, KInventoryItem >::iterator mit;
        mit = m_mapItem.find( mitToDelete->first );
        if( mit == m_mapItem.end() )
        {
            continue;
        }

        int iQuantity;
        _JIF( GetQuantity( mitToDelete->first, iQuantity ), return false );

        if( mitToDelete->second >= iQuantity ) // ???
        {
            int iCategory;
            int iSlotID;

            _JIF( GetCategorySlot( mitToDelete->first, iCategory, iSlotID ), return false );
            std::map< int, int >::iterator mitEmptySlot;
            mitEmptySlot = mapNumEmptySlot.find( iCategory );
            if( mitEmptySlot != mapNumEmptySlot.end() )
            {
                mitEmptySlot->second++;
            }
            else
            {
                mapNumEmptySlot.insert( std::make_pair( iCategory, 1 ) );
            }
        }
    }

    // 인벤토리에 공간이 충분한지 확인한다.
    std::map< int, KItemInfo > mapToInsertClone = mapToInsert;
    std::map< int, int > mapNumEmptySlotClone = mapNumEmptySlot;
    std::map< int, KItemInfo >::iterator mitToInsertClone;
    std::map< int, int >::iterator mitNumEmptySlotClone;
    int i, j;
    for( mitToInsertClone = mapToInsertClone.begin(); mitToInsertClone != mapToInsertClone.end(); ++mitToInsertClone )
    {
        i = GetSuitableCategory( mitToInsertClone->first );
        if( i < CXSLInventory::ST_EQUIP || i > CXSLInventory::ST_AVARTA )
        {
            START_LOG( cerr, L"인벤토리 카테고리 이상." )
                << BUILD_LOG( mitToInsertClone->first )
                << BUILD_LOG( i )
                << END_LOG;

            return false;
        }

        for( j = 0; j < GetSlotSize( i ); j++ )
        {
            if( IsEmptySlot( i, j ) )
            {
                continue;
            }

            if( GetItemID( i, j ) == mitToInsertClone->first )
            {
                mitToInsertClone->second.m_iQuantity -= min( mitToInsertClone->second.m_iQuantity, GetRemainedQuantityCapacity( i, j ) );
            }
        }
    }

    for( mitToInsertClone = mapToInsertClone.begin(); mitToInsertClone != mapToInsertClone.end(); ++mitToInsertClone )
    {
        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mitToInsertClone->first );
        if( !pItemTemplet )
        {
            START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
                << BUILD_LOG( mitToInsertClone->first )
                << END_LOG;

            return false;
        }

        int iRequiredNumSlot = 0;
        if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
        {
            if( mitToInsertClone->second.m_iQuantity > 0 )
            {
                if( pItemTemplet->m_Quantity <= 0 )
                {
                    START_LOG( cerr, L"수량 제한 값이 이상함." )
                        << BUILD_LOG( pItemTemplet->m_Quantity )
                        << END_LOG;

                    return false;
                }
                iRequiredNumSlot = mitToInsertClone->second.m_iQuantity / pItemTemplet->m_Quantity;
                if( ( mitToInsertClone->second.m_iQuantity % pItemTemplet->m_Quantity ) > 0 )
                {
                    iRequiredNumSlot++;
                }
            }
        }
        else
        {
            _JIF( mitToInsertClone->second.m_iQuantity >= 0, return false );
            iRequiredNumSlot = mitToInsertClone->second.m_iQuantity;
        }

        int iSuitableCategory = GetSuitableCategory( mitToInsertClone->first );
        if( iSuitableCategory < CXSLInventory::ST_EQUIP || iSuitableCategory > CXSLInventory::ST_AVARTA )
        {
            START_LOG( cerr, L"인벤토리 카테고리 이상." )
                << BUILD_LOG( mitToInsertClone->first )
                << BUILD_LOG( iSuitableCategory )
                << END_LOG;

            return false;
        }

        mitNumEmptySlotClone = mapNumEmptySlotClone.find( iSuitableCategory );
        if( mitNumEmptySlotClone == mapNumEmptySlotClone.end() )
        {
            std::pair< std::map< int, int >::iterator, bool > pr;
            pr = mapNumEmptySlotClone.insert( std::make_pair( iSuitableCategory, 0 ) );
            LIF( pr.second );
            mitNumEmptySlotClone = pr.first;
        }

        // 빈슬롯수에 요구슬롯수를 빼주는 부분
        mitNumEmptySlotClone->second -= iRequiredNumSlot;
        if( mitNumEmptySlotClone->second < 0 )
        {
            return false;
        }
    }

    // 모든 검사가 끝났고 이제부터 인벤토리에 변화가 생기므로 이 이후부터는 함부로 false를 리턴하면 안된다.
    // Remove시킬 아이템을 깎는다.
    for( mitToDelete = mapToDelete.begin(); mitToDelete != mapToDelete.end(); ++mitToDelete )
    {
        int iItemID = GetItemID( mitToDelete->first );
        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
        if( pItemTemplet == NULL )
        {
            START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
                << BUILD_LOG( iItemID )
                << END_LOG;

            continue;
        }

        int iCategory;
        int iSlotID;
        LIF( GetCategorySlot( mitToDelete->first, iCategory, iSlotID ) );
        KInventoryItemInfo kInventoryItemInfo;
        int iUsageType;
        _JIF( GetUsageType( mitToDelete->first, iUsageType ), continue );
        if( iUsageType == CXSLItem::PT_QUANTITY )
        {
            int iDecreased;
            DecreaseQuantity( mitToDelete->first, mitToDelete->second, iDecreased, eReason );
            LIF( mitToDelete->second == iDecreased );
            GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
        }
        else
        {
            LIF( mitToDelete->second == 1 );
            DeleteItem( mitToDelete->first, kInventoryItemInfo, eReason );
        }
        vecUpdatedItem.push_back( kInventoryItemInfo );
    }

    // 새 UID를 만들지 않고 수량을 채울 수 있는 것이 있으면 채운다.
    mapToInsertClone = mapToInsert;
    for( mitToInsertClone = mapToInsertClone.begin(); mitToInsertClone != mapToInsertClone.end(); ++mitToInsertClone )
    {
        i = GetSuitableCategory( mitToInsertClone->first );
        if( i < CXSLInventory::ST_EQUIP || i > CXSLInventory::ST_AVARTA )
        {
            START_LOG( cerr, L"인벤토리 카테고리 이상." )
                << BUILD_LOG( mitToInsertClone->first )
                << BUILD_LOG( i )
                << END_LOG;

            continue;
        }

        for( j = 0; j < GetSlotSize( i ); ++j )
        {
            if( IsEmptySlot( i, j ) )
            {
                continue;
            }

            if( GetItemID( i, j ) == mitToInsertClone->first )
            {
                int iIncreased;
                IncreaseQuantity( i, j, mitToInsertClone->second.m_iQuantity, iIncreased );
                mitToInsertClone->second.m_iQuantity -= iIncreased;

                if( iIncreased > 0 )
                {
                    KInventoryItemInfo kInventoryItemInfo;
                    GetInventorySlotInfo( i, j, kInventoryItemInfo );
                    vecUpdatedItem.push_back( kInventoryItemInfo );
                }

                if( mitToInsertClone->second.m_iQuantity <= 0 )
                {
                    break;
                }
            }
        }
    }

    // 남은 아이템을 DB에 신규 아이템으로 생성하기 위해 단위 수량씩 나누에 KItemInfo에 담는다.
    mapNumEmptySlotClone = mapNumEmptySlot;
    for( mitToInsertClone = mapToInsertClone.begin(); mitToInsertClone != mapToInsertClone.end(); ++mitToInsertClone )
    {
        if( mitToInsertClone->second.m_iQuantity <= 0 )
        {
            continue;
        }

        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mitToInsertClone->first );
        if( pItemTemplet == NULL )
        {
            START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
                << BUILD_LOG( mitToInsertClone->first )
                << END_LOG;

            continue;
        }

		int iQuantity = ( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY ? pItemTemplet->m_Quantity : 1 );
        if( iQuantity <= 0 )
        {
            START_LOG( cerr, L"수량 이상." )
                << BUILD_LOG( iQuantity )
                << END_LOG;

            continue;
        }

		short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
        if( sEndurance < 0 )
        {
            START_LOG( cerr, L"내구도 이상." )
                << BUILD_LOG( sEndurance )
                << END_LOG;

            continue;
        }

        while( mitToInsertClone->second.m_iQuantity > 0 )
        {
            mitNumEmptySlotClone = mapNumEmptySlotClone.find( GetSuitableCategory( mitToInsertClone->first ) );
            if( mitNumEmptySlotClone == mapNumEmptySlotClone.end() )
            {
                START_LOG( cerr, L"빈 슬롯이 없음. 발생하면 안되는 상황." )
                    << BUILD_LOG( mitToInsertClone->first )
                    << BUILD_LOG( GetSuitableCategory( mitToInsertClone->first ) )
                    << END_LOG;

                break;
            }

            if( mitNumEmptySlotClone->second <= 0 )
            {
                START_LOG( cerr, L"빈 슬롯이 없음. 발생하면 안되는 상황." )
                    << BUILD_LOG( mitToInsertClone->first )
                    << BUILD_LOG( GetSuitableCategory( mitToInsertClone->first ) )
                    << BUILD_LOG( mitNumEmptySlotClone->first )
                    << BUILD_LOG( mitNumEmptySlotClone->second )
                    << END_LOG;

                break;
            }

            iQuantity = std::min< int >( mitToInsertClone->second.m_iQuantity, iQuantity );

            KItemInfo kInfo;
            kInfo.m_iItemID			= mitToInsertClone->first;
            kInfo.m_cUsageType		= pItemTemplet->m_PeriodType;
            kInfo.m_iQuantity		= iQuantity;
            kInfo.m_sEndurance		= sEndurance;
            kInfo.m_sPeriod			= mitToInsertClone->second.m_sPeriod;
			kInfo.m_vecItemSocket	= mitToInsertClone->second.m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			kInfo.m_byteExpandedSocketNum	= mitToInsertClone->second.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
			//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			kInfo.m_vecRandomSocket	= mitToInsertClone->second.m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
			kInfo.m_cEnchantLevel	= mitToInsertClone->second.m_cEnchantLevel;
			kInfo.m_kAttribEnchantInfo = mitToInsertClone->second.m_kAttribEnchantInfo;
			//{{ 2009. 9. 7  최육사		밀봉
			kInfo.m_ucSealData		= mitToInsertClone->second.m_ucSealData;
			//}}
            vecNewItem.push_back( kInfo );

            mitToInsertClone->second.m_iQuantity -= iQuantity;
            mitNumEmptySlotClone->second--;
        }
    }

    return true;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}

bool KInventory::DeleteAndInsert( IN const std::map< int, int >& mapToDeleteByItemID, 
								  IN OUT std::map< UidType, int >& mapToDeleteByItemUID, 
								  IN const std::map< int, KItemInfo >& mapToInsert, 
								  OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, 
								  OUT std::vector< KItemInfo >& vecNewItem, 
								  IN bool bDeleteOnlyInventory, 
								  IN KDeletedItemInfo::DELETE_REASON eReason
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
								  , IN const bool bOnlyInfinite /*= false	*/	// 기한 제한이 없는 것
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
								  )
{
    std::map< int, int >::const_iterator mitByItemID;
    std::map< UidType, int >::iterator mitByItemUID;

    for( mitByItemID = mapToDeleteByItemID.begin(); mitByItemID != mapToDeleteByItemID.end(); ++mitByItemID )
    {
        _JIF( mitByItemID->first > 0, return false );
        _JIF( mitByItemID->second > 0, return false );
    }

    for( mitByItemUID = mapToDeleteByItemUID.begin(); mitByItemUID != mapToDeleteByItemUID.end(); ++mitByItemUID )
    {
        _JIF( mitByItemUID->first > 0, return false );
        _JIF( mitByItemUID->second > 0, return false );
    }

    vecUpdatedItem.clear();
    vecNewItem.clear();

    std::map< UidType, int > mapToDeleteTotal = mapToDeleteByItemUID;

    for( mitByItemID = mapToDeleteByItemID.begin(); mitByItemID != mapToDeleteByItemID.end(); ++mitByItemID )
    {
        std::vector< KInventoryItemInfo > vecContain;
        GetInventoryItemInfoContainingThisItem( mitByItemID->first
			, vecContain
			, bDeleteOnlyInventory
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
			, bOnlyInfinite
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			);

        int i, j;
        for( i = 0; i < ( int )vecContain.size(); ++i )
        {
            mitByItemUID = mapToDeleteByItemUID.find( vecContain[i].m_iItemUID );
            if( mitByItemUID != mapToDeleteByItemUID.end() )
            {
                int iQuantity;
                _JIF( GetQuantity( mitByItemUID->first, iQuantity ), return false );

                for( j = i - 1; j >= 0; j-- )
                {
                    _JIF( vecContain[i].m_kItemInfo.m_iQuantity >= mitByItemUID->second, return false );
                    vecContain[i].m_kItemInfo.m_iQuantity -= mitByItemUID->second;
                    if( vecContain[i].m_kItemInfo.m_iQuantity < vecContain[j].m_kItemInfo.m_iQuantity )
                    {
                        KInventoryItemInfo kInfoDest, kInfoSrc;
                        kInfoDest = vecContain[j];
                        kInfoSrc = vecContain[i];
                        vecContain[j] = kInfoSrc;
                        vecContain[i] = kInfoDest;
                    }
                }
            }
        }

        std::vector< KInventoryItemInfo >::iterator vit;
        int iQuantityToDelete = mitByItemID->second;
        for( vit = vecContain.begin(); vit != vecContain.end(); ++vit )
        {
            if( iQuantityToDelete <= 0 )
            {
                break;
            }
            
            int iQuantityToDeleteInThisItem = std::min< int >( iQuantityToDelete, vit->m_kItemInfo.m_iQuantity );

            mitByItemUID = mapToDeleteTotal.find( vit->m_iItemUID );
            if( mitByItemUID != mapToDeleteTotal.end() )
            {
                mitByItemUID->second += iQuantityToDeleteInThisItem;
            }
            else
            {
                mapToDeleteTotal.insert( std::make_pair( vit->m_iItemUID, iQuantityToDeleteInThisItem ) );
            }

            iQuantityToDelete -= iQuantityToDeleteInThisItem;
        }

        _JIF( iQuantityToDelete == 0, return false );
    }

    return DeleteAndInsert( mapToDeleteTotal, mapToInsert, vecUpdatedItem, vecNewItem, eReason );
}

void KInventory::FlushQuantityChange( std::map< UidType, int >& mapChanged )
{
    mapChanged.clear();

    std::map< UidType, KInventoryItem >::iterator mit;
    for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
    {
		//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE
		if( mit->second.IsDBUpdate() == false )
			continue;
#endif SERV_PC_BANG_PRE
		//}}

        int iQuantityChange;
        if( mit->second.FlushQuantityChange( iQuantityChange ) )
        {
            mapChanged.insert( std::make_pair( mit->first, iQuantityChange ) );
        }
    }
}

void KInventory::RollBackInitQuantity( const std::map< UidType, int >& mapChanged )
{
    std::map< UidType, int >::const_iterator mit;
    for( mit = mapChanged.begin(); mit != mapChanged.end(); ++mit )
    {
        std::map< UidType, KInventoryItem >::iterator mitItem;
        mitItem = m_mapItem.find( mit->first );
        if( mitItem == m_mapItem.end() )
        {
            START_LOG( cerr, L"수량 롤백하려는 아이템이 없음." )
				<< BUILD_LOG( m_pUser->GetCharUID() )
				<< BUILD_LOG( m_pUser->GetCharName() )
                << BUILD_LOG( mit->first )
                << BUILD_LOG( mit->second )
                << END_LOG;

            continue;
        }

        mitItem->second.RollBackInitQuantity( mit->second );

		START_LOG( cerr, L"아이템 수량 DB업뎃실패! 롤백!" )
			<< BUILD_LOG( m_pUser->GetCharUID() )
			<< BUILD_LOG( m_pUser->GetCharName() )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second )
			<< END_LOG;
    }
}

void KInventory::FlushEnduranceChange( std::map< UidType, int >& mapChanged )
{
	mapChanged.clear();

	std::map< UidType, KInventoryItem >::iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE
		if( mit->second.IsDBUpdate() == false )
			continue;
#endif SERV_PC_BANG_PRE
		//}}

		int iEnduranceChange;
		if( mit->second.FlushEnduranceChange( iEnduranceChange ) )
		{
			mapChanged.insert( std::make_pair( mit->first, iEnduranceChange ) );
		}
	}
}

void KInventory::RollBackInitEndurance( const std::map< UidType, int >& mapChanged )
{
	std::map< UidType, int >::const_iterator mit;
	for( mit = mapChanged.begin(); mit != mapChanged.end(); ++mit )
	{
		std::map< UidType, KInventoryItem >::iterator mitItem;
		mitItem = m_mapItem.find( mit->first );
		if( mitItem == m_mapItem.end() )
		{
            START_LOG( cerr, L"내구도 롤백하려는 아이템이 없음." )
                << BUILD_LOG( mit->first )
                << BUILD_LOG( mit->second )
                << END_LOG;

			continue;
		}

		mitItem->second.RollBackInitEndurance( mit->second );

		START_LOG( cerr, L"아이템 내구도 DB업뎃실패! 롤백!" )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second )
			<< END_LOG;
	}
}

//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

void KInventory::FlushPositionChange( OUT std::map< UidType, KItemPosition >& mapChanged )
{
	mapChanged.clear();

	std::map< UidType, KInventoryItem >::iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE
		if( mit->second.IsDBUpdate() == false )
			continue;
#endif SERV_PC_BANG_PRE
		//}}

		KItemPosition kInfo;
		if( mit->second.FlushPositionChange( kInfo.m_iSlotCategory, kInfo.m_iSlotID, kInfo.m_iPetUID ) == true )
		{
			//{{ 2008. 6. 30  최육사  비정상적인 인벤 카테고리 검사
			if( !VerifyInventoryCategory( mit->second.GetItemID(), mit->second.GetInventoryCategory() ) )
			{
				START_LOG( cerr, L"비정상적인 인벤카테고리를 DB업데이트 하려함." )
					<< BUILD_LOG( mit->second.GetItemID() )
					<< BUILD_LOG( mit->second.GetInventoryCategory() )
					<< END_LOG;
			}
			//}}

			mapChanged.insert( std::make_pair( mit->first, kInfo ) );
		}
	}
}

void KInventory::PositionUpdateComplete( IN const std::map< UidType, KItemPosition >& mapChanged )
{
	std::map< UidType, KItemPosition >::const_iterator mit;
	for( mit = mapChanged.begin(); mit != mapChanged.end(); ++mit )
	{
		std::map< UidType, KInventoryItem >::iterator mitItem;
		mitItem = m_mapItem.find( mit->first );
		if( mitItem == m_mapItem.end() )
		{
			// 펫 아이템이면 아이템이 없는것은 정상이다.
			if( mit->second.m_iSlotCategory == CXSLInventory::ST_PET )
			{
				START_LOG( clog, L"펫 인벤토리의 슬롯 위치 업데이트 완료하려는 아이템이 현재는 없음." )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second.m_iSlotCategory )
					<< BUILD_LOG( mit->second.m_iSlotID )
					<< BUILD_LOG( mit->second.m_iPetUID )
					<< END_LOG;
			}
			else
			{
				START_LOG( cerr, L"슬롯 위치 업데이트 완료하려는 아이템이 없음." )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second.m_iSlotCategory )
					<< BUILD_LOG( mit->second.m_iSlotID )
					<< BUILD_LOG( mit->second.m_iPetUID )
					<< END_LOG;
			}
			
			continue;
		}

		mitItem->second.PositionUpdateComplete( mit->second.m_iSlotCategory, mit->second.m_iSlotID, mit->second.m_iPetUID );
	}
}

#else

void KInventory::FlushPositionChange( std::map< UidType, std::pair< int, int > >& mapChanged )
{
	mapChanged.clear();

	std::map< UidType, KInventoryItem >::iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE
		if( mit->second.IsDBUpdate() == false )
			continue;
#endif SERV_PC_BANG_PRE
		//}}

		int iCategoryChange, iSlotIDChange;
		if( mit->second.FlushPositionChange( iCategoryChange, iSlotIDChange ) == true )
		{
			//{{ 2008. 6. 30  최육사  비정상적인 인벤 카테고리 검사
			if( !VerifyInventoryCategory( mit->second.GetItemID(), mit->second.GetInventoryCategory() ) )
			{
				START_LOG( cerr, L"비정상적인 인벤카테고리를 DB업데이트 하려함." )
					<< BUILD_LOG( mit->second.GetItemID() )
					<< BUILD_LOG( mit->second.GetInventoryCategory() )
					<< END_LOG;			
			}
			//}}

			mapChanged.insert( std::make_pair( mit->first, std::make_pair( iCategoryChange, iSlotIDChange ) ) );
		}
	}
}

void KInventory::PositionUpdateComplete( IN const std::map< UidType, std::pair< int, int > >& mapChanged )
{
	std::map< UidType, std::pair< int, int > >::const_iterator mit;
	for( mit = mapChanged.begin(); mit != mapChanged.end(); mit++ )
	{
		std::map< UidType, KInventoryItem >::iterator mitItem;
		mitItem = m_mapItem.find( mit->first );
		if( mitItem == m_mapItem.end() )
		{
			START_LOG( cerr, L"슬롯 위치 업데이트 완료하려는 아이템이 없음." )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second.first )
				<< BUILD_LOG( mit->second.second )
				<< END_LOG;
			continue;
		}

		mitItem->second.PositionUpdateComplete( mit->second.first, mit->second.second );
	}
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2009. 12. 15  최육사	아이템 삭제사유
void KInventory::FlushDeletedItem( std::vector< KDeletedItemInfo >& vecDeleted )
{
    vecDeleted.clear();

    vecDeleted = m_vecDeletedItem;
    m_vecDeletedItem.clear();
}

void KInventory::RollBackDeletedItem( const std::vector< KDeletedItemInfo >& vecDeleted )
{
	if( vecDeleted.empty() )
	{
		//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
		m_mapDeletedItem.clear();
#endif SERV_GUILD_CHANGE_NAME
	//}}
		return;
	}

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	std::map< UidType, KInventoryItem > mapDeletedItem;
	std::map< UidType, KInventoryItem >::const_iterator cit;

	BOOST_TEST_FOREACH( KDeletedItemInfo, kDeletedItemInfo, vecDeleted )
	{
		cit = m_mapDeletedItem.find( kDeletedItemInfo.m_iItemUID );
		if( cit != m_mapDeletedItem.end() )
		{
			mapDeletedItem.insert( std::make_pair( cit->first, cit->second ) );
		}
	}

	m_mapDeletedItem = mapDeletedItem;
#endif SERV_GUILD_CHANGE_NAME
	//}}

    m_vecDeletedItem.insert( m_vecDeletedItem.end(), vecDeleted.begin(), vecDeleted.end() );

	START_LOG( cerr, L"삭제된 아이템 DB업뎃실패! 롤백!" )
		<< BUILD_LOG( m_pUser->GetCharUID() )
		<< BUILD_LOG( m_pUser->GetCharName() )
		<< END_LOG;
}
//}}

void KInventory::DecreaseEquippedItemEnduranceInPvp()
{
	std::vector< KDecreaseEnduranceInfo > vecDummy;
	std::vector< KInventoryItemInfo > vecDummy2;
	DecreaseEquippedItemEndurance( 0, 0, 0, vecDummy, vecDummy2 );
}

void KInventory::DecreaseEquippedItemEnduranceInDungeon( IN const int iExpDropNpcNum, IN const int iPartyNumMember, OUT std::vector< KDecreaseEnduranceInfo >& vecSlotInfo, OUT std::vector< KInventoryItemInfo >& vecZeroEndurance )
{
	LIF( iPartyNumMember > 0 );
	DecreaseEquippedItemEndurance( 0, iExpDropNpcNum, iPartyNumMember, vecSlotInfo, vecZeroEndurance );
}

void KInventory::DecreaseEquippedItemEndurance( IN const int iEnduranceDamage, 
											    IN const int iExpDropNpcNum, 
												IN const int iPartyNumMember, 
												OUT std::vector< KDecreaseEnduranceInfo >& vecSlotInfo, 
												OUT std::vector< KInventoryItemInfo >& vecZeroEndurance )
{
	vecSlotInfo.clear();
	vecZeroEndurance.clear();

    for( int i = 0; i < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++i )
    {
		// 1. 빈슬롯 확인
        if( IsEmptySlot( CXSLInventory::ST_E_EQUIP, i ) )
            continue;

		// 2. 인벤슬롯 정보 얻기
		KInventoryItemInfo kInfo;
		if( GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, i, kInfo ) == false )
		{
			START_LOG( cwarn, L"빈슬롯이 아닌지 확인했는데?" )
				<< BUILD_LOG( i )
				<< END_LOG;
			continue;
		}

		// 3. 이미 내구도가 0이라면 처리 안함
		if( kInfo.m_kItemInfo.m_sEndurance <= 0 )
			continue;
        
        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
        if( pItemTemplet == NULL )
        {
            START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
                << BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
                << END_LOG;

            continue;
        }

		// 4. 내구도 아이템인지 확인
		if( pItemTemplet->m_PeriodType != CXSLItem::PT_ENDURANCE )
			continue;

		int iDamage = 0;

		// 5. 깎을 내구도 구하기
		if( iEnduranceDamage > 0 )
		{
			iDamage = iEnduranceDamage;
		}
		else
		{
			short sEnduranceDamageMax = pItemTemplet->m_EnduranceDamageMax;
			short sEnduranceDamageMin = pItemTemplet->m_EnduranceDamageMin;
			_JIF( sEnduranceDamageMax >= 0, continue );
			_JIF( sEnduranceDamageMin >= 0, continue );
			_JIF( sEnduranceDamageMax >= sEnduranceDamageMin, continue );

			iDamage = rand() % ( sEnduranceDamageMax - sEnduranceDamageMin + 1 ) + sEnduranceDamageMin;
		}

		// 6. 한 서브 스테이지의 NPC수 만큼 나눈다!
		if( iExpDropNpcNum > 0 )
		{
            iDamage = iDamage / iExpDropNpcNum;
		}

		// 7. 파티원 수 만큼 나눈다!
		if( iPartyNumMember > 0 )
		{
			iDamage = iDamage / iPartyNumMember;
		}

		// 8. 내구도는 최소한 1은 소모되어야 한다!
		if( iDamage <= 0 )
			iDamage = 1;

		// 9. 내구도 감소
		int iDecreased = 0;
        DecreaseEndurance( kInfo.m_iItemUID, iDamage, iDecreased );

		// 10. 깎인 내구도 얻기
		int iCurEndurance = 0;
		LIF( GetEndurance( kInfo.m_iItemUID, iCurEndurance ) );

		// 11. 클라이언트에 업데이트할 데이터
		KDecreaseEnduranceInfo kDecEndurance;
		kDecEndurance.m_iItemUID = kInfo.m_iItemUID;
		kDecEndurance.m_iEndurance = iCurEndurance;
		vecSlotInfo.push_back( kDecEndurance );

		// 12. 내구도가 모두 소모되었다면..?
		if( iCurEndurance <= 0 )
		{
			KInventoryItemInfo kZeroEndurance;
			LIF( GetInventoryItemInfo( kInfo.m_iItemUID, kZeroEndurance ) );
			vecZeroEndurance.push_back( kZeroEndurance );
		}

        START_LOG( clog, L"내구도 깎임." )
            << BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )            
			<< BUILD_LOG( iEnduranceDamage )
            << BUILD_LOG( iDamage )
            << BUILD_LOG( iDecreased )
			<< BUILD_LOG( iCurEndurance );
    }
}

//{{ 2012. 03. 06	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KInventory::DecreaseEquippedItemEnduranceInBattleField( IN const int iExpDropNpcNum, 
															 IN const int iPartyNumMember, 
															 OUT std::vector< KDecreaseEnduranceInfo >& vecSlotInfo, 
															 OUT std::vector< KInventoryItemInfo >& vecZeroEndurance )
{
	vecSlotInfo.clear();
	vecZeroEndurance.clear();

	for( int i = 0; i < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++i )
	{
		// 1. 빈슬롯 확인
		if( IsEmptySlot( CXSLInventory::ST_E_EQUIP, i ) )
			continue;

		// 2. 인벤슬롯 정보 얻기
		KInventoryItemInfo kInfo;
		if( GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, i, kInfo ) == false )
		{
			START_LOG( cwarn, L"빈슬롯이 아닌지 확인했는데?" )
				<< BUILD_LOG( i )
				<< END_LOG;
			continue;
		}

		// 3. 이미 내구도가 0이라면 처리 안함
		if( kInfo.m_kItemInfo.m_sEndurance <= 0 )
			continue;

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
				<< END_LOG;

			continue;
		}

		// 4. 내구도 아이템인지 확인
		if( pItemTemplet->m_PeriodType != CXSLItem::PT_ENDURANCE )
			continue;

		int iDamage = 0;

		// 5. 깎을 내구도 구하기		
		const short sEnduranceDamageMax = pItemTemplet->m_EnduranceDamageMax;
		const short sEnduranceDamageMin = pItemTemplet->m_EnduranceDamageMin;
		_JIF( sEnduranceDamageMax >= 0, continue );
		_JIF( sEnduranceDamageMin >= 0, continue );
		_JIF( sEnduranceDamageMax >= sEnduranceDamageMin, continue );

		iDamage = rand() % ( sEnduranceDamageMax - sEnduranceDamageMin + 1 ) + sEnduranceDamageMin;		

		// 6. 한 서브 스테이지의 NPC수 만큼 나눈다!
		if( iExpDropNpcNum > 0 )
		{
			iDamage = iDamage / iExpDropNpcNum;
		}

		// 7. 파티원 수 만큼 곱한다!
		iDamage = iDamage * iPartyNumMember;

		// 8. 필드상수값을 곱한다.
		//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
		iDamage = static_cast<int>( static_cast<float>(iDamage) * SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() );
#else
		iDamage = static_cast<int>( static_cast<float>(iDamage) * SiCXSLBattleFieldManager()->GetBattleFieldFactor() );
#endif SERV_FIELD_ED_EXP_FACTOR
		//}

		// 8. 내구도는 최소한 1은 소모되어야 한다!
		if( iDamage <= 0 )
			iDamage = 1;

		// 9. 내구도 감소
		int iDecreased = 0;
		DecreaseEndurance( kInfo.m_iItemUID, iDamage, iDecreased );

		// 10. 깎인 내구도 얻기
		int iCurEndurance = 0;
		LIF( GetEndurance( kInfo.m_iItemUID, iCurEndurance ) );

		// 11. 클라이언트에 업데이트할 데이터
		KDecreaseEnduranceInfo kDecEndurance;
		kDecEndurance.m_iItemUID = kInfo.m_iItemUID;
		kDecEndurance.m_iEndurance = iCurEndurance;
		vecSlotInfo.push_back( kDecEndurance );

		// 12. 내구도가 모두 소모되었다면..?
		if( iCurEndurance <= 0 )
		{
			KInventoryItemInfo kZeroEndurance;
			LIF( GetInventoryItemInfo( kInfo.m_iItemUID, kZeroEndurance ) );
			vecZeroEndurance.push_back( kZeroEndurance );
		}

		START_LOG( clog, L"내구도 깎임." )
			<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( iDamage )
			<< BUILD_LOG( iDecreased )
			<< BUILD_LOG( iCurEndurance );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
bool KInventory::RepairItem( IN const std::vector< UidType > vecItemUID, OUT int& iED, OUT int &iVP, OUT std::vector< KInventoryItemInfo >& vecUpdated, IN KGSUserPtr pUser )
#endif SERV_SERVER_BUFF_SYSTEM
//}
{
	iED = 0;
    iVP = 0;
    vecUpdated.clear();

	//{{ 2009. 2. 3  최육사		수리할인을 해당 아이템만 할인에서 전체 할인으로 변경	
    int iTotalRequiredED = 0;
    int iRequiredVP = 0;

	BOOST_TEST_FOREACH( const UidType, iItemUID, vecItemUID )
    {
        const int iItemID = GetItemID( iItemUID );
        const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
        if( pItemTemplet == NULL )
        {
            START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
                << BUILD_LOG( iItemID )
                << END_LOG;

            return false;
        }

        _JIF( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE, return false );

		// 전체 수리비에 합산
        iTotalRequiredED += static_cast<int>( pItemTemplet->m_RepairED * static_cast<float>(GetEnduranceToRestore( iItemUID )) );

		//////////////////////////////////////////////////////////////////////////
		// [이벤트] 12월 3일까지 수리비 할인
		//iTotalRequiredED += iRequiredED - (int)( (float)iRequiredED * ( GetRepairItemDiscount( *vit ) + 0.5f ) );
		//////////////////////////////////////////////////////////////////////////
		
		// 20081118 - 최육사 : 내구도 소모시 VP는 복구비용이 들지 않음
        //iRequiredVP += pItemTemplet->m_RepairVP * iEndurance;
    }

	float fDiscount = GetRepairItemDiscount();
	
	//{{ 2011. 09. 26  임규수 칭호 수리비 감소 속성 적용
#ifdef SERV_TITLE_REPAIR_DISCOUNT
	fDiscount += GetRepairTitleDiscount( pUser->GetEquippedTitleID() );
#endif SERV_TITLE_REPAIR_DISCOUNT
	//}}

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	if( pUser->ExistActivateBuff( CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS ) == true )
		fDiscount =  fDiscount + 0.5f;
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
#ifdef SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#else //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	if( pUser->ExistActivateBuff( CXSLBuffManager::BTI_BUFF_2013_DEFENSE_BUFF ) == true )
	{
		fDiscount =  fDiscount + 0.5f;
	}

	if( pUser->ExistActivateBuff( CXSLBuffManager::BTI_BUFF_2013_DEFENSE_DEBUFF ) == true )
	{
		fDiscount =  fDiscount - 0.5f;
	}

	if( pUser->ExistActivateBuff( CXSLBuffManager::BTI_BUFF_2013_DEFENSE_BUFF_EVENT ) == true )
	{
		fDiscount =  fDiscount + 0.7f;
	}
#endif //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	if( fDiscount > 1.f )
		fDiscount = 1.f;

	//iTotalRequiredED = ( iTotalRequiredED - (int)( (float)iTotalRequiredED * fDiscount ) );


	// 클라이언트랑 수리비 계상 공식 동일하게 맞춤
	iTotalRequiredED = (int)( iTotalRequiredED * ( 1.f - fDiscount ) );

	if( iTotalRequiredED < 0 )
		iTotalRequiredED = 0;

	//}}

    _JIF( m_pUser, return false );
    _JIF( m_pUser->GetED() >= iTotalRequiredED, return false );
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	_JIF( m_pUser->GetAPoint() >= iRequiredVP, return false );
#else
	_JIF( m_pUser->GetVSPoint() >= iRequiredVP, return false );
#endif SERV_PVP_NEW_SYSTEM
	//}}    

    iED = iTotalRequiredED;
    iVP = iRequiredVP;

	BOOST_TEST_FOREACH( const UidType, iItemUID, vecItemUID )
    {
        int iCategory;
        int iSlotID;
        _JIF( GetCategorySlot( iItemUID, iCategory, iSlotID ), return false );

        int iIncreased;
        int iEndurance = GetEnduranceToRestore( iItemUID );
        if( iEndurance <= 0 )
        {
            continue;
        }
        IncreaseEndurance( iItemUID, iEndurance, iIncreased );
        LIF( iIncreased == iEndurance );
        KInventoryItemInfo kInventoryItemInfo;
        GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
        vecUpdated.push_back( kInventoryItemInfo );
    }

    return true;
}

float KInventory::GetRepairItemDiscount()
{
	float fTotalDiscount = 0.0f;

	for( int i = 0; i < GetSlotSize( CXSLInventory::ST_E_EQUIP ); ++i )
	{
		if( !IsEmptySlot( CXSLInventory::ST_E_EQUIP, i ) )
		{
			KInventoryItemInfo kInfo;
			if( !GetInventoryItemInfo( CXSLInventory::ST_E_EQUIP, i, kInfo ) )
			{
				START_LOG( cerr, L"인벤토리 아이템 정보 얻기 실패." )
					<< BUILD_LOG( i )
					<< END_LOG;

				continue;
			}

			// 1. 사용불가 아이템이면 소켓옵션이 적용되지 않는다.
			if( kInfo.m_kItemInfo.m_cEnchantLevel < 0 )
				continue;

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"아이템 템플릿 포인트 이상.!" )
					<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
					<< END_LOG;

				continue;
			}

			// 2. 유닛 레벨과 장착조건 레벨이 틀리면 소켓옵션이 적용되지 않는다. - 아이템의 레벨조정으로 인한 옵션기능.
			if( m_pUser->GetLevel() < pItemTemplet->m_UseLevel )
			{
				START_LOG( cwarn, L"아이템 장착조건이 되지 않는데 아이템을 착용하고 있음." )
					<< BUILD_LOG( pItemTemplet->m_ItemID )
					<< BUILD_LOG( pItemTemplet->m_Name )
					<< BUILD_LOG( m_pUser->GetLevel() )
					;
				continue;
			}

			// 3. 부가옵션 적용
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			std::vector< int >::const_iterator vit = pItemTemplet->m_vecSocketOption.begin();
#else
			std::vector< short >::const_iterator vit = pItemTemplet->m_vecSocketOption.begin();
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			for( ; vit != pItemTemplet->m_vecSocketOption.end(); ++vit )
			{
				if( *vit <= 0 )
					continue;

				const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
				if( pSocketData == NULL )
				{
					START_LOG( cerr, L"소켓 옵션 정보를 찾을수 없습니다." )
						<< BUILD_LOG( *vit )
						<< END_LOG;
					continue;
				}

				if( pSocketData->m_fRepairPriceSale > 0.0f )
					fTotalDiscount += pSocketData->m_fRepairPriceSale;
			}

			// 3. 소켓옵션 적용
			vit = kInfo.m_kItemInfo.m_vecItemSocket.begin();
			for( ; vit != kInfo.m_kItemInfo.m_vecItemSocket.end(); ++vit )
			{
				if( *vit <= 0 )
					continue;

				const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
				if( pSocketData == NULL )
				{
					START_LOG( cerr, L"소켓 옵션 정보를 찾을수 없습니다." )
						<< BUILD_LOG( *vit )
						<< END_LOG;
					continue;
				}

				if( pSocketData->m_fRepairPriceSale > 0.0f )
					fTotalDiscount += pSocketData->m_fRepairPriceSale;
			}
		}
	}

	return fTotalDiscount;
}

bool KInventory::GetZeroOfEndurancebyEquippedItem( IN std::vector<UidType>& vecItemUID, OUT std::vector<UidType>& vecZeroItemUID )
{
	vecZeroItemUID.clear();

	//수리될 아이템 항목을 검사시작한다.
	std::vector< UidType >::iterator vit;
	for( vit = vecItemUID.begin(); vit != vecItemUID.end(); vit++ )
	{
		int iCategory;
		int iSlotID;

		//1.위치정보를 찾아내고.
		_JIF( GetCategorySlot( *vit, iCategory, iSlotID ), return false );

		//2.해당위치에 있는 아이템정보를 구하여.
		KInventoryItemInfo kInventoryItemInfo;
		GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );

		//3.장착아이템인지.. 그리고 내구도가 0인지 체크.
		if( kInventoryItemInfo.m_cSlotCategory == CXSLInventory::ST_E_EQUIP )
		{
			if( kInventoryItemInfo.m_kItemInfo.m_sEndurance <= 0 )
			{
				vecZeroItemUID.push_back( kInventoryItemInfo.m_iItemUID );
			}
		}
	}

	return true;
}

//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
bool KInventory::OpenRandomItem( IN char cUnitClass, 
								IN UidType iItemUID, 
								IN const int iEventItemID,
								OUT std::map< int, int >& mapInsertedItem, 
								OUT std::vector< KInventoryItemInfo >& vecUpdated, 
								OUT std::vector< KItemInfo >& vecNewItem, 
								OUT int& iRessurectionCount,
								OUT int& iRestoreSpirit,
								//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
								OUT int& iED,
#endif SERV_CUBE_OPEN_ED_CONDITION
								//}
								OUT bool& bCharmItem )
#else
bool KInventory::OpenRandomItem( IN char cUnitClass, 
								IN UidType iItemUID, 
								OUT std::map< int, int >& mapInsertedItem, 
								OUT std::vector< KInventoryItemInfo >& vecUpdated, 
								OUT std::vector< KItemInfo >& vecNewItem, 
								OUT int& iRessurectionCount,
								OUT int& iRestoreSpirit,
								//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION //SERV_ALL_RENEWAL_SP
								OUT int& iED,
#endif SERV_CUBE_OPEN_ED_CONDITION //SERV_ALL_RENEWAL_SP
								//}
								OUT bool& bCharmItem )
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}
{
    mapInsertedItem.clear();
    vecUpdated.clear();
    vecNewItem.clear();

    std::map< UidType, int >   mapRandomBox;
    std::map< int, int >	   mapRequireKey;  // 열쇠템
    std::map< int, KItemInfo > mapResultItem;  // 결과템

    SET_ERROR( NET_OK );

    // 1. 큐브 아이템 존재 여부
    if( IsExist( iItemUID ) == false )
    {
        START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( m_pUser->GetName() )
			<< BUILD_LOG( m_pUser->GetCharName() )
            << BUILD_LOG( iItemUID )
            << END_LOG;

        SET_ERROR( ERR_RANDOM_ITEM_00 );
        return false;
    }
    mapRandomBox.insert( std::make_pair( iItemUID, 1 ) );

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는아이템은 오픈할수 없다" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

    const int iItemID = GetItemID( iItemUID );

	// 1-1. 매력아이템인지 검사
	//{{ 2013. 3. 16	박세훈	매력 아이템 스크립트 처리
#ifdef SERV_CHARM_ITEM_SCRIPT
	bCharmItem = SiCXSLRandomItemManager()->IsCharmItem( iItemID );
#endif SERV_CHARM_ITEM_SCRIPT
	//}}

	//{{ 2010.12.20 김민성 큐브 오픈 레벨 제한 확인
#ifdef SERV_OPEN_RANDOM_ITEM_LEVEL_CHECK
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿 정보 찾기 실패!" )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_04 );
			return false;
		}

		// 레벨 조건 검사
		if(pItemTemplet->m_UseLevel > m_pUser->GetLevel())
		{
			START_LOG( cerr, L"큐브보다 낮은 레벨의 유저가 오픈!" )			// cerr 인 이유: 클라에서 걸러진 내용인데 와서 이상함	
				<< BUILD_LOG( iItemID )
				<< END_LOG;
	
			SET_ERROR( ERR_RANDOM_ITEM_08 );
			return false;
		}
	}
#endif SERV_OPEN_RANDOM_ITEM_LEVEL_CHECK


	// 2. 랜덤 아이템 정보
	//{{ 2009. 12. 5  최육사	랜덤큐브시스템개편
	const std::map< int, int >* pRandomItemKeyList = SiCXSLRandomItemManager()->GetRandomItemKeyList( iItemID );
	if( pRandomItemKeyList == NULL )
	{
		START_LOG( cerr, L"해당 랜덤 아이템의 키 테이블 정보를 얻지 못함." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RANDOM_ITEM_01 );
		return false;
	}

	// 3. 키템의 수량이 충분한지 검사
	if( pRandomItemKeyList->empty() )
	{
		START_LOG( cerr, L"해당 랜덤 아이템에 대한 키템 정보가 없다. 있을수 없는에러!" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RANDOM_ITEM_01 );
		return false;
	}

	bool bEnoughKey = false;
	int iKeyItemID = 0;

	std::map< int, int >::const_iterator mitKey;
	for( mitKey = pRandomItemKeyList->begin(); mitKey != pRandomItemKeyList->end(); ++mitKey )
	{
		// 열쇠가 필요없는 랜덤 아이템인지 검사
        if( mitKey->first == 0 )
		{
			bEnoughKey = true;
			break;
		}

		if( mitKey->second <= 0 )
		{
			START_LOG( cerr, L"필요한 키 아이템 수량 이상." )
				<< BUILD_LOG( mitKey->first )
				<< BUILD_LOG( mitKey->second )
				<< END_LOG;

			SET_ERROR( ERR_RANDOM_ITEM_05 );
			return false;
		}

		std::map< int, int > mapCheckRequireKey;
		mapCheckRequireKey.insert( std::make_pair( mitKey->first, mitKey->second ) );

		if( IsEnoughItemExistOnlyInventory( mapCheckRequireKey ) == true )
		{
			iKeyItemID = mitKey->first;
            bEnoughKey = true;
#ifdef SERV_EVENT_RANDOM_ITEM_KEY_NO_DELETE
			bool bEventRandomItemKey = false;
			switch(GetItemID(iItemUID))
			{
			case 85003835: // 정파비전 큐브 열쇠
			case 85003836: // 사파비전 큐브 열쇠
				bEventRandomItemKey = true;
				break;
			default:
				break;
			}
			if(bEventRandomItemKey)
				break;
#endif SERV_EVENT_RANDOM_ITEM_KEY_NO_DELETE
#ifdef SERV_RANDOM_ITEM_KEY_NO_DELETE
			if(GetItemID(iItemUID) != 80000114) // 2011.09.07 lygan_조성욱 //VIP 큐브에서 특정 키가 소모 안되게 처리방식
#endif //SERV_RANDOM_ITEM_KEY_NO_DELETE
			mapRequireKey = mapCheckRequireKey;
			break;
		}
	}
	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	if( bEnoughKey == false && iKeyItemID == 0 )
	{
		SET_ERROR( ERR_RANDOM_ITEM_04 );
		return false;
	}
#else
	if( bEnoughKey == false )
	{
		SET_ERROR( ERR_RANDOM_ITEM_04 );
		return false;
	}
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}
	//}}

#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT_VER2// 작업날짜: 2013-05-14	// 박세훈
	int iItemIDForRandomItemData = iItemID;
	SiKGameSysVal()->CheckAttractionItemTimeEvent( iItemIDForRandomItemData );
	const CXSLRandomItemManager::RandomItemData* pRandomItemData = SiCXSLRandomItemManager()->GetRandomItemData( iItemIDForRandomItemData, iKeyItemID );
#else  // SERV_ATTRACTION_ITEM_TIME_EVENT_VER2
	const CXSLRandomItemManager::RandomItemData* pRandomItemData = SiCXSLRandomItemManager()->GetRandomItemData( iItemID, iKeyItemID );
#endif // SERV_ATTRACTION_ITEM_TIME_EVENT_VER2

	if( pRandomItemData == NULL )
	{
		START_LOG( cerr, L"랜덤 아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iKeyItemID )
			<< END_LOG;

		SET_ERROR( ERR_RANDOM_ITEM_01 );
		return false;
	}

	//{{ 2013. 02. 04  특정시간 가열기 확률 변경 - 김민성
#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT
	CTime tCurr = CTime::GetCurrentTime();
	if( SiKGameSysVal()->GetRandomCubeEventStartTime() <= tCurr && tCurr <= SiKGameSysVal()->GetRandomCubeEventEndTime() )
	{
		int iTimeEventItemID = SiKGameSysVal()->IsTimeEventItemID( iItemID );
		if( iTimeEventItemID > 0 )
		{
			pRandomItemData = SiCXSLRandomItemManager()->GetRandomItemData( iTimeEventItemID, iKeyItemID );
			if( pRandomItemData == NULL )
			{
				START_LOG( cerr, L"랜덤 아이템 템플릿을 얻지 못함" )
					<< BUILD_LOG( iTimeEventItemID )
					<< BUILD_LOG( iKeyItemID )
					<< END_LOG;

				SET_ERROR( ERR_RANDOM_ITEM_01 );
				return false;
			}
		}
	}
#endif SERV_ATTRACTION_ITEM_TIME_EVENT
	//}

	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	bool bEnoughED = false;

	// 4. ED 검사
	if( pRandomItemData->m_iRequiredED > 0 )
	{
		if( m_pUser->GetED() >= pRandomItemData->m_iRequiredED )
		{
			iED = pRandomItemData->m_iRequiredED;
			bEnoughED = true;
		}
	}
	else
	{
		iED = pRandomItemData->m_iRequiredED;
		bEnoughED = true;
	}

	// 5. Error 팝업 분류
	if( bEnoughKey == false && bEnoughED == false ) // 키 아이템, ed 부족
	{
		SET_ERROR( ERR_RANDOM_ITEM_12 );			// 'A' 큐브를 개봉 하기 위해서는 'B' 아이템과 'C' ed 가 필요합니다
		return false;
	}
	else if( bEnoughKey == false )	// 키 아이템 부족(ed는 있음)
	{
		SET_ERROR( ERR_RANDOM_ITEM_04 );			// 'A' 큐브를 개봉 하기 위해서는 'B' 아이템이 필요합니다
		return false;
	}
	else if( bEnoughED == false )	//  ed 부족(키 아이템은 있음)
	{
		SET_ERROR( ERR_RANDOM_ITEM_13 );			// ED가 부족해서 'A' 큐르블 개봉 할 수 없습니다.
		return false;
	}
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}
	
	// 3. 부활석 충전 정보 및 근성도 회복템 정보 얻기
	iRessurectionCount = pRandomItemData->m_iRessurectionCount;
	iRestoreSpirit	   = pRandomItemData->m_iRestoreSpirit;

	std::map< int, int > mapCheckEmpty; // 큐브의 모든 확률에 대한 빈공간 검사 [특정 확률 높이기 방지]
	    
    // 5. 랜덤 결과 얻기
	//{{ 2011. 05. 12  김민성	랜덤 큐브 오픈시 인벤토리 검사 수정
#ifdef SERV_RANDOM_ITEM_CHECK_INVEN
	bool				 bIsGiveAll = false;
	if( SiCXSLRandomItemManager()->GetResultItem( cUnitClass, pRandomItemData, mapResultItem, mapCheckEmpty, bIsGiveAll ) == false )
#else
	if( SiCXSLRandomItemManager()->GetResultItem( cUnitClass, pRandomItemData, mapResultItem, mapCheckEmpty ) == false )
#endif SERV_RANDOM_ITEM_CHECK_INVEN
	//}}
	{
		START_LOG( cwarn, L"랜덤아이템 받아오기 실패.!" )
			<< BUILD_LOGc( cUnitClass )
			<< BUILD_LOG( pRandomItemData->m_ItemID )
			<< BUILD_LOG( mapResultItem.size() )
			<< END_LOG;

		return false;
	}


	//{{ 2008. 5. 27  최육사  랜덤 결과 인벤 공간 검사	
	std::map< int, int > mapCheckResultItem;

	//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	if( iEventItemID != 0 )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iEventItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"존재 하지 않는 ItemID입니다!" )
				<< BUILD_LOG( m_pUser->GetCharUID() )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iEventItemID )
				<< END_LOG;

			SET_ERROR( ERR_RANDOM_ITEM_05 );
			return false;
		}

		KItemInfo kEventItemInfo;
		kEventItemInfo.m_iItemID	= iEventItemID;
		kEventItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
		kEventItemInfo.m_iQuantity	= 1;
		kEventItemInfo.m_sEndurance = pItemTemplet->m_Endurance;

		//{{ 2011. 07. 04    김민성    네이버 프로모션
#ifdef SERV_NAVER_PROMOTIONS
		if( iEventItemID == CXSLItem::EI_SUMMER_TEN_ENCHANT_CHARM )
		{
			mapResultItem.clear();
		}
#else
		mapResultItem.clear();
#endif SERV_NAVER_PROMOTIONS
		//}} 
		mapResultItem.insert( std::make_pair( iEventItemID, kEventItemInfo ) );	

		// 검사도 하자!
		mapCheckResultItem.insert( std::make_pair( iEventItemID, 1 ) );
	}
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	//}}	

	//{{ 2011. 05. 12  김민성	랜덤 큐브 오픈시 인벤토리 검사 수정
#ifdef SERV_RANDOM_ITEM_CHECK_INVEN
	if( bIsGiveAll == false )
	{
		BOOST_MAP_CONST_FOREACH( int, int, kResultItem, mapCheckEmpty )
		{
			mapCheckResultItem.clear();
			mapCheckResultItem.insert( std::make_pair( kResultItem.first, kResultItem.second ) );

			//{{ 2009. 12. 11  최육사 매력아이템오류수정
			if( bCharmItem )
			{
#ifdef	SERV_CHARM_ITEM_SCRIPT// 적용날짜: 2013-04-26
				const int iBonusItemID = SiCXSLRandomItemManager()->GetBonusCharmItem( iItemID );
				if( 0 < iBonusItemID )
				{
					const CXSLItem::ItemTemplet* pItemTempletBonus = SiCXSLItemManager()->GetItemTemplet( iBonusItemID );
					if( pItemTempletBonus == NULL )
					{
						START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
							<< BUILD_LOG( iBonusItemID )
							<< END_LOG;
					}
					else
					{
						mapCheckResultItem.insert( std::make_pair( iBonusItemID, 1 ) );
					}
				}
#else	// SERV_CHARM_ITEM_SCRIPT
				// 매력아이템의 결과템과 보너스템을 함께 빈공간검사를 해야한다.
				mapCheckResultItem.insert( std::make_pair( CXSLRandomItemManager::RID_ATTRANTION_BONUS, 1 ) );
#endif	// SERV_CHARM_ITEM_SCRIPT
			}

			if( IsEnoughSpaceExist( mapCheckResultItem ) == false )
			{
				SET_ERROR( ERR_RANDOM_ITEM_07 );
				return false;
			}
		}
	}
	else
	{
		if( IsEnoughSpaceExist( mapCheckEmpty ) == false )
		{
			SET_ERROR( ERR_RANDOM_ITEM_07 );
			return false;
		}
	} 
#else
	BOOST_MAP_CONST_FOREACH( int, int, kResultItem, mapCheckEmpty )
	{
		mapCheckResultItem.clear();
		mapCheckResultItem.insert( std::make_pair( kResultItem.first, kResultItem.second ) );

		//{{ 2009. 12. 11  최육사	매력아이템오류수정
		if( bCharmItem )
		{
			// 매력아이템의 결과템과 보너스템을 함께 빈공간검사를 해야한다.
			mapCheckResultItem.insert( std::make_pair( CXSLRandomItemManager::RID_ATTRANTION_BONUS, 1 ) );
		}
		//}}

		if( IsEnoughSpaceExist( mapCheckResultItem ) == false )
		{
			SET_ERROR( ERR_RANDOM_ITEM_07 );
			return false;
		}
	}
#endif SERV_RANDOM_ITEM_CHECK_INVEN
	//}}
		
	// 6. 습득 아이템 정보
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}

	// 7. 결과 처리
	if( DeleteAndInsert( mapRequireKey, mapRandomBox, mapResultItem, vecUpdated, vecNewItem, true, KDeletedItemInfo::DR_RANDOM_ITEM ) == false )
    {
        SET_ERROR( ERR_RANDOM_ITEM_05 );
        return false;
    }

	// 랜덤 큐브 사용 통계
	KStatisticsKey kKeyRandom;
	kKeyRandom.m_vecIntKey.push_back( iItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyRandom, KStatistics::SI_ITEM_USE, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyRandom, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	// 열쇠 사용 통계
	if( pRandomItemData->m_KeyItemID > 0 )
	{
		KStatisticsKey kKeyKey;
		kKeyKey.m_vecIntKey.push_back( pRandomItemData->m_KeyItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyKey, KStatistics::SI_ITEM_USE, pRandomItemData->m_RequiredKeyCount );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyKey, KStatistics::SI_ITEM_USE, pRandomItemData->m_RequiredKeyCount );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 랜덤 결과 아이템 통계
	std::map<int,int>::const_iterator mit;
	for( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_RANDOM, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_RANDOM, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

    return true;
}

#ifdef SERV_MANUFACTURE_FIX
bool KInventory::ItemManufacture( IN int iManufactureID,				
								 IN int iQuantity,													
								 OUT int& iED, 
								 OUT std::map< int, int >& mapInsertedItem, 
								 OUT std::vector< KInventoryItemInfo >& vecUpdated, 
								 OUT std::vector< KItemInfo >& vecNewItem )
{
	SET_ERROR( NET_OK );

	iED = 0;
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();

#ifdef SERV_CHECK_POSSIBLE_MANUFACTURE_ID	// 적용날짜: 미정
	if( SiCXSLManufactureItemManager()->IsPossibleManufactureID( iManufactureID ) == false )
	{
		START_LOG( cerr, L"제조 데이터를 얻지 못함" )
			<< BUILD_LOG( iManufactureID )
			<< END_LOG;

		SET_ERROR( ERR_MANUFACTURE_00 );
		return false;
	}
#endif	// SERV_CHECK_POSSIBLE_MANUFACTURE_ID

	// 1. 제조 데이터 검사
	const CXSLManufactureItemManager::ManufactureData* pManufactureData = SiCXSLManufactureItemManager()->GetManufactureData( iManufactureID );
	if( pManufactureData == NULL )
	{
		START_LOG( cerr, L"제조 데이터를 얻지 못함" )
			<< BUILD_LOG( iManufactureID )
			<< END_LOG;

		SET_ERROR( ERR_MANUFACTURE_02 );
		return false;
	}

	// 2. 재료 아이템 검사
	std::map< int, int > mapToDeleteByItemID;
	std::map< int, int >::iterator mitToDeleteByItemID;
	std::vector< CXSLManufactureItemManager::MaterialData >::const_iterator vitMaterial;
	for( vitMaterial = pManufactureData->m_vecMaterials.begin(); vitMaterial != pManufactureData->m_vecMaterials.end(); ++vitMaterial )
	{
		mitToDeleteByItemID = mapToDeleteByItemID.find( vitMaterial->m_MaterialItemID );
		if( mitToDeleteByItemID != mapToDeleteByItemID.end() )
		{
			START_LOG( cerr, L"이미 존재하는 재료 아이템 ID" )
				<< BUILD_LOG( iManufactureID )
				<< BUILD_LOG( vitMaterial->m_MaterialItemID )
				<< END_LOG;
		}
		mapToDeleteByItemID.insert( std::make_pair( vitMaterial->m_MaterialItemID, vitMaterial->m_MaterialCount * iQuantity ) );
	}

	// 3. 장착중인 아이템은 재료에서 제외
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-06-25	// 박세훈
	if( IsEnoughItemExist( mapToDeleteByItemID, true, true ) == false )
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
	if( !IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) )
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
	{
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-02	// 박세훈
		if( IsEnoughItemExist( mapToDeleteByItemID, true, false ) == true )
		{
			SET_ERROR( ERR_MANUFACTURE_12 );	// 기간제 아이템은 재료나 교환 아이템으로 사용할 수 없습니다.
		}
		else
		{
			SET_ERROR( ERR_MANUFACTURE_05 );
		}
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		SET_ERROR( ERR_MANUFACTURE_05 );
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		return false;
	}

	// 4. 제조 시작
	//{{ 2012. 08. 07	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	int iResultItemID = 0;
	int iResultItemGroupID = pManufactureData->m_kLotteryResultItem.Decision();
#ifdef SERV_MANUFACTURE_PERIOD
	int iResultGroupID = iResultItemID;
#endif //SERV_MANUFACTURE_PERIOD
	if( iResultItemGroupID != KLottery::CASE_BLANK )
	{
		iResultItemID = SiCXSLManufactureItemManager()->GetManufactureResultItemID( iResultItemGroupID );
	}
	else
	{
		START_LOG( clog, L"제조 실패!" )
			<< BUILD_LOG( iManufactureID )
			<< BUILD_LOG( iResultItemGroupID )
			<< END_LOG;
	}
#else
	int iResultItemID = pManufactureData->m_kLotteryResultItem.Decision();
#ifdef SERV_MANUFACTURE_PERIOD
	int iResultGroupID = iResultItemID;
#endif //SERV_MANUFACTURE_PERIOD
	if( iResultItemID != KLottery::CASE_BLANK )
	{
		iResultItemID = SiCXSLManufactureItemManager()->GetManufactureResultItemID( iResultItemID );
	}
	else
	{
		START_LOG( clog, L"제조 실패!" )
			<< BUILD_LOG( iManufactureID )
			<< BUILD_LOG( iResultItemID )
			<< END_LOG;
	}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}


	// 지헌 : 여기서, 만들어지는 총 개수에 따라 , 인벤 빈 자리가 있는지를 체크 하도록 하자.
	//		  관련 소스는, 상점 아이템 구매 부분을 보면 나올 듯.

	// 5. 생성된 아이템이 들어갈 자리가 있는지 조사	
	std::map< int, KItemInfo > mapToInsert;
	if( iResultItemID > 0 )
	{
		//////////////////////////////////////////////////////////////////////////
		//{{ 2008. 3. 17  최육사  DeleteAndInsert()
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iResultItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿 정보 찾기 실패!" )
				<< BUILD_LOG( iResultItemID )
				<< END_LOG;
		}
		else
		{
#ifdef SERV_MANUFACTURE_PERIOD
			int iPeriod = SiCXSLManufactureItemManager()->GetManufactureResultItemIDPeriod(iResultGroupID, iResultItemID );

			if (iPeriod == KLottery::PARAM_BLANK )
				iPeriod = 0;

			if (pItemTemplet->m_PeriodType != CXSLItem::PERIOD_TYPE::PT_INFINITY )
				iPeriod = 0;
#endif //SERV_MANUFACTURE_PERIOD

			KItemInfo kInsertItemInfo;
			kInsertItemInfo.m_iItemID	 = iResultItemID;
			kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
			kInsertItemInfo.m_iQuantity	 = iQuantity;
			kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
#ifdef SERV_MANUFACTURE_PERIOD
			kInsertItemInfo.m_sPeriod	 = static_cast<short>(iPeriod);
#endif //SERV_MANUFACTURE_PERIOD		

			//{{ 2013. 04. 14	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
			if( SiCXSLDefenceDungeonManager()->IsManufactureDefenceDungeonItem( iResultItemID ) == true )
			{
				//{{ 2013. 06. 03	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				if( SiCXSLDefenceDungeonManager()->InsertSocketOptionForDefenceDungeonItem( iResultItemID, kInsertItemInfo.m_vecRandomSocket ) == false )
#else
				if( SiCXSLDefenceDungeonManager()->InsertSocketOptionForDefenceDungeonItem( iResultItemID, kInsertItemInfo.m_vecItemSocket ) == false )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
				//}}				
				{
					START_LOG( cerr, L"어둠의 문 아이템에 소켓 옵션 박기가 실패하였습니다!" )
						<< BUILD_LOG( iResultItemID )
						<< END_LOG;
				}
			}
#endif SERV_NEW_DEFENCE_DUNGEON
			//}}

#ifdef SERV_ADD_SEALED_ITEM_SIGN
            switch( pItemTemplet->m_ItemType )
            {
            case CXSLItem::IT_WEAPON:
            case CXSLItem::IT_DEFENCE:
            case CXSLItem::IT_ACCESSORY:
                if( SiCXSLManufactureItemManager()->IsItemSealed( iResultItemID ) == true )
                {
                    kInsertItemInfo.m_ucSealData = 101;
                }
                break;
            }
#endif SERV_ADD_SEALED_ITEM_SIGN

			mapToInsert.insert( std::make_pair( iResultItemID, kInsertItemInfo ) );

			// [결과 아이템 ID, 결과 아이템 수량]
			mapInsertedItem.insert( std::make_pair( iResultItemID, iQuantity ) );
		}
		//}}

		//{{ 2010. 05. 03  최육사	소켓 개편
#ifdef SERV_SOCKET_NEW
		//{{ 2012. 07. 16	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM		
		// 인벤토리에 충분한 공간이 있는지 확인한다.
		if( SiCXSLManufactureItemManager()->IsRandomResult( iResultItemGroupID ) == true )
		{
			// Random 제조 결과물이라면,
			std::vector<int> vecCategoryID;
			GetSuitableCategory( iResultItemID, vecCategoryID );

			bool bCheck = false;
			BOOST_TEST_FOREACH( int, iCategoryCheck, vecCategoryID )
			{
				// 해당 결과물이 저장될 수 있는 모든 카테고리를 점검하여, 충분한 공간이 있는지 확인하고,
				// ( 수량성 제조 아이템의 랜덤 제조 시 어뷰징을 방지하기 위해 비어있는 슬롯을 체크한다!! )
				if( IsEnoughEmptySlot( iCategoryCheck, 1 ) == true )
				{
					break;
				}
			}

			// 공간이 없다면, 오류를 출력한다.
			if( bCheck == false )
			{
				START_LOG(clog2, L"슬롯 검사에서 에러!")
					<<BUILD_LOG( static_cast<int>( vecCategoryID.size() ) )
					<<BUILD_LOG( iQuantity )
					<<BUILD_LOG( iResultItemID )
					<<END_LOG;			

				SET_ERROR( ERR_MANUFACTURE_10 );
				return false;
			}
		}
		else
		{
			// 제조 결과물이 고정되어 있다면, 수량성의 경우 채울 공간이 있는지까지 포함하여 검사한다.
			// ( 빈 슬롯이 없어도 채울 수 있는 공간이 있다면 통과한다 )
			if( IsEnoughSpaceExist( mapInsertedItem ) == false )
			{
				SET_ERROR( ERR_MANUFACTURE_10 );
				return false;
			}
		}
#else
		const int iCheckEmptyCategory = GetSuitableCategory( iResultItemID );
		if( IsEnoughEmptySlot( iCheckEmptyCategory, 1 ) == false )
		{
			START_LOG(clog2, L"지헌로그:슬롯 검사에서 에러!")
				<<BUILD_LOG( iCheckEmptyCategory)
				<<BUILD_LOG( iQuantity)
				<<BUILD_LOG( iResultItemID )
				<<END_LOG;			

			SET_ERROR( ERR_MANUFACTURE_10 );
			return false;
		}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}
#else
#pragma ELSE_MESSAGE
		//if( !IsEnoughSpaceExist( mapInsertedItem ) )
		//{
		//	SET_ERROR( ERR_MANUFACTURE_10 );
		//	return false;
		//}
#endif SERV_SOCKET_NEW
		//}}

		// 지헌 : 여기서, 만들어지는 총 개수에 따라 , 인벤 빈 자리가 있는지를 체크 하도록 하자.
		//		  관련 소스는, 상점 아이템 구매 부분을 보면 나올 듯.

		// 인벤토리에 충분한 공간이 있는지 확인한다.
		if( IsEnoughSpaceExist( mapInsertedItem ) == false )
		{

			SET_ERROR( ERR_MANUFACTURE_10 );
			return false;
		}
	}

	// 6. 비용 검사
	iED = pManufactureData->m_Cost * iQuantity;
	if( iED < 0 )
	{
		START_LOG( cerr, L"제조 비용이 마이너스입니다. 즉시 확인이 필요합니다!!" )
			<< BUILD_LOG( iManufactureID )
			<< BUILD_LOG( pManufactureData->m_Cost )
			<< BUILD_LOG( iQuantity )
			<< BUILD_LOG( iED )
			<< END_LOG;

		SET_ERROR( ERR_MANUFACTURE_00 );
		return false;
	}

	if( m_pUser->GetED() < iED)
	{
		SET_ERROR( ERR_MANUFACTURE_03 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// [이벤트] ~ 12월 3일까지 제조 수수료 할인
	//if( m_pUser->GetED() < ( pManufactureData->m_Cost / 2 ) )
	//{
	//    SET_ERROR( ERR_MANUFACTURE_03 );
	//    return false;
	//}
	//iED = pManufactureData->m_Cost / 2;
	//////////////////////////////////////////////////////////////////////////

	// 7. 결과 처리
	if( !DeleteAndInsert( mapToDeleteByItemID
		, mapToInsert
		, vecUpdated
		, vecNewItem
		, true
		, KDeletedItemInfo::DR_MANUFACTURE
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
		, true
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		)
		)
	{
		SET_ERROR( ERR_MANUFACTURE_00 );
		return false;
	}

	//////////////////////////////////////////////////////////////
	// 제조 통계
	KStatisticsKey kKeyManufacture;
	kKeyManufacture.m_vecIntKey.push_back( iResultItemID );

	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyManufacture, KStatistics::SI_ITEM_MAKE, iQuantity );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyManufacture, KStatistics::SI_ITEM_MAKE, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	if( iResultItemID <= 0 )
	{
		KStatisticsKey kKeyManufactureFailed;
		kKeyManufactureFailed.m_vecIntKey.push_back( iResultItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyManufactureFailed, KStatistics::SI_ITEM_FAILED, iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyManufactureFailed, KStatistics::SI_ITEM_FAILED, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}	

	// 제조 재료 사용통계
	std::vector< CXSLManufactureItemManager::MaterialData >::const_iterator vit = pManufactureData->m_vecMaterials.begin();
	for( ; vit != pManufactureData->m_vecMaterials.end(); vit++ )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( vit->m_MaterialItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, vit->m_MaterialCount * iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, vit->m_MaterialCount * iQuantity);
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// ED 사용량 통계
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );	
	KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MManufacture, iED);

	return true;
}

#else SERV_MANUFACTURE_FIX
bool KInventory::ItemManufacture( IN int iManufactureID,								
								  OUT int& iED, 
								  OUT std::map< int, int >& mapInsertedItem, 
								  OUT std::vector< KInventoryItemInfo >& vecUpdated, 
								  OUT std::vector< KItemInfo >& vecNewItem )
{
    SET_ERROR( NET_OK );

    iED = 0;
    mapInsertedItem.clear();
    vecUpdated.clear();
    vecNewItem.clear();
    
    // 1. 제조 데이터 검사
    const CXSLManufactureItemManager::ManufactureData* pManufactureData = SiCXSLManufactureItemManager()->GetManufactureData( iManufactureID );
    if( pManufactureData == NULL )
    {
        START_LOG( cerr, L"제조 데이터를 얻지 못함" )
            << BUILD_LOG( iManufactureID )
            << END_LOG;

        SET_ERROR( ERR_MANUFACTURE_02 );
        return false;
    }
    
    // 2. 재료 아이템 검사
    std::map< int, int > mapToDeleteByItemID;
    std::map< int, int >::iterator mitToDeleteByItemID;
    std::vector< CXSLManufactureItemManager::MaterialData >::const_iterator vitMaterial;
    for( vitMaterial = pManufactureData->m_vecMaterials.begin(); vitMaterial != pManufactureData->m_vecMaterials.end(); ++vitMaterial )
    {
        mitToDeleteByItemID = mapToDeleteByItemID.find( vitMaterial->m_MaterialItemID );
        if( mitToDeleteByItemID != mapToDeleteByItemID.end() )
        {
            START_LOG( cerr, L"이미 존재하는 재료 아이템 ID" )
                << BUILD_LOG( iManufactureID )
                << BUILD_LOG( vitMaterial->m_MaterialItemID )
                << END_LOG;
        }
        mapToDeleteByItemID.insert( std::make_pair( vitMaterial->m_MaterialItemID, vitMaterial->m_MaterialCount ) );
    }

	// 3. 장착중인 아이템은 재료에서 제외
    if( !IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) )
    {
        SET_ERROR( ERR_MANUFACTURE_05 );
        return false;
    }
    
    // 4. 제조 시작
    int iResultItemID = pManufactureData->m_kLotteryResultItem.Decision();
    if( iResultItemID != KLottery::CASE_BLANK )
    {
        iResultItemID = SiCXSLManufactureItemManager()->GetManufactureResultItemID( iResultItemID );
    }
    else
    {
        START_LOG( clog, L"제조 실패!" )
            << BUILD_LOG( iResultItemID );
    }

    // 5. 생성된 아이템이 들어갈 자리가 있는지 조사	
	std::map< int, KItemInfo > mapToInsert;
	if( iResultItemID > 0 )
	{
		//////////////////////////////////////////////////////////////////////////
		//{{ 2008. 3. 17  최육사  DeleteAndInsert()
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iResultItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿 정보 찾기 실패!" )
				<< BUILD_LOG( iResultItemID )
				<< END_LOG;
		}
		else
		{
			KItemInfo kInsertItemInfo;
			kInsertItemInfo.m_iItemID	 = iResultItemID;
			kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
			kInsertItemInfo.m_iQuantity	 = 1;
			kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
			
			//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN
			switch( pItemTemplet->m_ItemType )
			{
			case CXSLItem::IT_WEAPON:
			case CXSLItem::IT_DEFENCE:
			case CXSLItem::IT_ACCESSORY:
				if( SiCXSLManufactureItemManager()->IsItemSealed( iResultItemID ) == true )
				{
					kInsertItemInfo.m_ucSealData = 101;
				}
				break;
			}
#endif SERV_ADD_SEALED_ITEM_SIGN
			//}}

			mapToInsert.insert( std::make_pair( iResultItemID, kInsertItemInfo ) );

			// [결과 아이템 ID, 결과 아이템 수량]
			mapInsertedItem.insert( std::make_pair( iResultItemID, 1 ) );
		}
		//}}

		//{{ 2010. 05. 03  최육사	소켓 개편
#ifdef SERV_SOCKET_NEW
		const int iCheckEmptyCategory = GetSuitableCategory( iResultItemID );
		if( IsEnoughEmptySlot( iCheckEmptyCategory, 1 ) == false )
		{
			SET_ERROR( ERR_MANUFACTURE_10 );
			return false;
		}
#else
#pragma ELSE_MESSAGE
		//if( !IsEnoughSpaceExist( mapInsertedItem ) )
		//{
		//	SET_ERROR( ERR_MANUFACTURE_10 );
		//	return false;
		//}
#endif SERV_SOCKET_NEW
		//}}
	}

    // 6. 비용 검사
    if( m_pUser->GetED() < pManufactureData->m_Cost )
    {
        SET_ERROR( ERR_MANUFACTURE_03 );
        return false;
    }
    iED = pManufactureData->m_Cost;

	//////////////////////////////////////////////////////////////////////////
	// [이벤트] ~ 12월 3일까지 제조 수수료 할인
	//if( m_pUser->GetED() < ( pManufactureData->m_Cost / 2 ) )
	//{
	//    SET_ERROR( ERR_MANUFACTURE_03 );
	//    return false;
	//}
	//iED = pManufactureData->m_Cost / 2;
	//////////////////////////////////////////////////////////////////////////

    // 7. 결과 처리
	if( !DeleteAndInsert( mapToDeleteByItemID, mapToInsert, vecUpdated, vecNewItem, true, KDeletedItemInfo::DR_MANUFACTURE ) )
    {
        SET_ERROR( ERR_MANUFACTURE_00 );
        return false;
    }

	//////////////////////////////////////////////////////////////
	// 제조 통계
	KStatisticsKey kKeyManufacture;
	kKeyManufacture.m_vecIntKey.push_back( iResultItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyManufacture, KStatistics::SI_ITEM_MAKE, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyManufacture, KStatistics::SI_ITEM_MAKE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	if( iResultItemID <= 0 )
	{
		KStatisticsKey kKeyManufactureFailed;
		kKeyManufactureFailed.m_vecIntKey.push_back( iResultItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyManufactureFailed, KStatistics::SI_ITEM_FAILED, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyManufactureFailed, KStatistics::SI_ITEM_FAILED, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}	

	// 제조 재료 사용통계
	std::vector< CXSLManufactureItemManager::MaterialData >::const_iterator vit = pManufactureData->m_vecMaterials.begin();
	for( ; vit != pManufactureData->m_vecMaterials.end(); vit++ )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( vit->m_MaterialItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, vit->m_MaterialCount );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, vit->m_MaterialCount );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// ED 사용량 통계
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );	
	KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MManufacture, pManufactureData->m_Cost );

    return true;
}
#endif SERV_MANUFACTURE_FIX
#ifdef SERV_DELETE_CUBE_GIVE_POST
void KInventory::ExpireItem( OUT std::vector< UidType >& vecItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo, OUT std::vector< KInventoryItemInfo >& vecDeleteItem)	
#else SERV_DELETE_CUBE_GIVE_POST
void KInventory::ExpireItem( OUT std::vector< UidType >& vecItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
#endif SERV_DELETE_CUBE_GIVE_POST
{
    std::map< UidType, KInventoryItem >::const_iterator mit;
    for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
    {
        if( mit->second.IsPeriodExpired() )
        {
            vecItemUID.push_back( mit->first );
        }
    }

    std::vector< UidType >::const_iterator vit;
    for( vit = vecItemUID.begin(); vit != vecItemUID.end(); ++vit )
    {
#ifdef SERV_DELETE_CUBE_GIVE_POST
		// 지워지기 전에 아이템 정보 미리얻자
		KInventoryItemInfo TempDeleteItem;
		GetInventoryItemInfo( *vit, TempDeleteItem );
		vecDeleteItem.push_back(TempDeleteItem);
#endif SERV_DELETE_CUBE_GIVE_POST

        KInventoryItemInfo kInfo;
		DeleteItem( *vit, kInfo, KDeletedItemInfo::DR_EXPIRED );
        vecInventorySlotInfo.push_back( kInfo );
    }
}

bool KInventory::VerifyCategorySlotID( IN const int iCategory, IN const int iSlotID ) const
{
    if( iCategory < GetFirstCategory()  ||  iCategory >= GetSentinelCategory() )
    {
        START_LOG( cerr, L"카테고리 이상." )
            << BUILD_LOG( iCategory )			
			<< BUILD_LOG( iSlotID )
            << END_LOG;

        return false;
    }

    if( iSlotID < 0  ||  iSlotID >= GetSlotSize( iCategory ) )
    {
        START_LOG( cerr, L"슬롯 ID 이상." )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( GetSlotSize( iCategory ) )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        return false;
    }

    return true;
}

//{{ 2008. 6. 30  최육사  카테고리 검사
//{{ 2012. 07. 9	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
bool KInventory::VerifyInventoryCategory( IN int iItemID, IN int iCategory )
{
	// 아이템의 카테고리 정보를 얻어오자
	std::vector<int> vecCategoryID;
	GetSuitableCategory( iItemID, vecCategoryID );

	BOOST_TEST_FOREACH( int, iCategoryCheck, vecCategoryID )
	{
		if( iCategory == iCategoryCheck )
			return true;

		switch( iCategory )
		{
		case CXSLInventory::ST_E_EQUIP:
			{
				switch( iCategoryCheck )
				{
				case CXSLInventory::ST_EQUIP:
				case CXSLInventory::ST_ACCESSORY:
				case CXSLInventory::ST_AVARTA:
					return true;

				default:
					break;
				}
			}
			break;

		case CXSLInventory::ST_E_QUICK_SLOT:
			{
				if( iCategoryCheck == CXSLInventory::ST_QUICK_SLOT )
					return true;
			}
			break;

			//{{ 2009. 8. 7  최육사		은행
		case CXSLInventory::ST_BANK:
			{
				return true;
			}
			break;
			//}}

			//{{ 2010. 9. 9	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		case CXSLInventory::ST_PET:
			{
				return true;
			}
			break;
#endif SERV_PET_SYSTEM
			//}}

		default:
			break;
		}
	}

	return false;
}
#else
bool KInventory::VerifyInventoryCategory( IN int iItemID, IN int iCategory )
{
	int iCategoryCheck = GetSuitableCategory( iItemID );

	if( iCategory == iCategoryCheck )
		return true;

	switch( iCategory )
	{
	case CXSLInventory::ST_E_EQUIP:
		{
			switch( iCategoryCheck )
			{
			case CXSLInventory::ST_EQUIP:
			case CXSLInventory::ST_ACCESSORY:
			case CXSLInventory::ST_AVARTA:
				return true;

			default:
				break;
			}
		}
		break;

	case CXSLInventory::ST_E_QUICK_SLOT:
		{
			if( iCategoryCheck == CXSLInventory::ST_QUICK_SLOT )
				return true;
		}
		break;

		//{{ 2009. 8. 7  최육사		은행
	case CXSLInventory::ST_BANK:
#ifdef	SERV_SHARING_BANK_TEST
	case CXSLInventory::ST_SHARE_BANK:
#endif	SERV_SHARING_BANK_TEST
		{
			return true;
		}
		break;
		//}}

		//{{ 2010. 9. 9	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	case CXSLInventory::ST_PET:
		{
            return true;
		}
		break;
#endif SERV_PET_SYSTEM
		//}}

	default:
		break;
	}

	return false;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}
//}}

int KInventory::GetRemainedQuantityCapacity( UidType iItemUID )
{
    std::map< UidType, KInventoryItem >::iterator mit;
    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        START_LOG( cerr, L"해당 아이템 UID가 없음." )
            << BUILD_LOG( iItemUID )
            << END_LOG;

        return 0;
    }

    return mit->second.GetRemainedQuantityCapacity();
}

int KInventory::GetRemainedQuantityCapacity( int iCategory, int iSlotID )
{
    return GetRemainedQuantityCapacity( GetItemUID( iCategory, iSlotID ) );
}

int KInventory::GetEnduranceToRestore( UidType iItemUID )
{
    std::map< UidType, KInventoryItem >::iterator mit;
    mit = m_mapItem.find( iItemUID );
    if( mit == m_mapItem.end() )
    {
        START_LOG( cerr, L"해당 아이템 UID가 없음." )
            << BUILD_LOG( iItemUID )
            << END_LOG;

        return 0;
    }

    return mit->second.GetEnduranceToRestore();
}

int KInventory::GetEnduranceToRestore( int iCategory, int iSlotID )
{
    return GetEnduranceToRestore( GetItemUID( iCategory, iSlotID ) );
}

bool KInventory::CompareLevel( int iItemID )
{
    if( !m_pUser )
    {
        return false;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
    if( !pItemTemplet )
    {
        START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return false;
    }

    return ( int )m_pUser->GetLevel() >= pItemTemplet->m_UseLevel;
}

bool KInventory::CompareUnitClass( int iItemID )
{
    if( !m_pUser )
    {
        return false;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
    if( !pItemTemplet )
    {
        START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return false;
    }

	

	switch( pItemTemplet->m_UseCondition )
	{
	case CXSLItem::UC_NONE:
	case CXSLItem::UC_ANYONE:
	case CXSLItem::UC_ONE_UNIT:
		{
			switch( pItemTemplet->m_UnitType )
			{
			case CXSLUnit::UT_NONE:
				{
					if( pItemTemplet->m_UnitClass == CXSLUnit::UC_NONE )
						return true;

					if( CXSLUnit::IsValidUnitClass( pItemTemplet->m_UnitClass ) == false )
					{
						START_LOG( cerr, L"이상한 유닛 클래스." )
							<< BUILD_LOG( pItemTemplet->m_UnitClass )
							<< END_LOG;

						return false;
					}

					// 동일한 UnitClass인지 검사
					return ( m_pUser->GetUnitClass() == pItemTemplet->m_UnitClass );
				}
				break;        

			case CXSLUnit::UT_ELSWORD:
			case CXSLUnit::UT_ARME:
			case CXSLUnit::UT_LIRE:
			case CXSLUnit::UT_RAVEN:
			case CXSLUnit::UT_EVE:
			// kimhc // 2010-12-23 에 추가될 신캐릭터 청
			case CXSLUnit::UT_CHUNG:
#ifdef SERV_ARA_MARTIAL_ARTIST
			case CXSLUnit::UT_ARA:
#endif
#ifdef SERV_NEW_CHARACTER_EL
			case CXSLUnit::UT_ELESIS:
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			case CXSLUnit::UT_ADD:
#endif //SERV_9TH_NEW_CHARACTER
				{
					return ( m_pUser->GetUnitType() == pItemTemplet->m_UnitType );
				}
				break;

			default:
				START_LOG( cerr, L"이상한 유닛 타입." )
					<< BUILD_LOG( pItemTemplet->m_UnitType )
					<< END_LOG;

				return false;
			}
		}
		break;

	case CXSLItem::UC_ONE_CLASS:
		{
			if( pItemTemplet->m_UnitClass == m_pUser->GetUnitClass() ||
				pItemTemplet->m_UnitClass == CXSLUnit::GetUnitClassDownGrade( (CXSLUnit::UNIT_CLASS)m_pUser->GetUnitClass() ) ||
				pItemTemplet->m_UnitClass == CXSLUnit::GetUnitClassBaseGrade( (CXSLUnit::UNIT_CLASS)m_pUser->GetUnitClass() )
				)
			{
				return true;
			}
		}
		break;
	default:
		break;
	}

	return false;
}

bool KInventory::IsAbleToExpandSlot( int nSlot )
{
    if( ( GetMaxSlotSize( CXSLInventory::ST_EQUIP ) - GetSlotSize( CXSLInventory::ST_EQUIP ) ) < nSlot )
    {
        return false;
    }

    if( ( GetMaxSlotSize( CXSLInventory::ST_ACCESSORY ) - GetSlotSize( CXSLInventory::ST_ACCESSORY ) ) < nSlot )
    {
        return false;
    }

    if( ( GetMaxSlotSize( CXSLInventory::ST_MATERIAL ) - GetSlotSize( CXSLInventory::ST_MATERIAL ) ) < nSlot )
    {
        return false;
    }

    if( ( GetMaxSlotSize( CXSLInventory::ST_SPECIAL ) - GetSlotSize( CXSLInventory::ST_SPECIAL ) ) < nSlot )
    {
        return false;
    }

    if( ( GetMaxSlotSize( CXSLInventory::ST_QUEST ) - GetSlotSize( CXSLInventory::ST_QUEST ) ) < nSlot )
    {
        return false;
    }

    if( ( GetMaxSlotSize( CXSLInventory::ST_QUICK_SLOT ) - GetSlotSize( CXSLInventory::ST_QUICK_SLOT ) ) < nSlot )
    {
        return false;
    }

    return true;
}

//{{ 2011. 12. 13  김민성	퀵 슬롯 확장 아이템
#ifdef SERV_EXPAND_QUICK_SLOT
bool KInventory::IsAbleToExpandSlot( IN int iCategory, IN int nSlot )
{
	if( iCategory < GetFirstCategory() || iCategory >= GetSentinelCategory() )
	{
		START_LOG( cerr, L"잘못된 카테고리." )
			<< BUILD_LOG( iCategory )
			<< END_LOG;

		return false;
	}

	return ( GetMaxSlotSize( iCategory ) - GetSlotSize( iCategory ) ) > nSlot;
}
#endif SERV_EXPAND_QUICK_SLOT
//}}

//{{ 2009. 8. 7  최육사		은행
bool KInventory::IsAbleToExpandBank( IN int nSlot )
{	
	if( ( GetMaxSlotSize( CXSLInventory::ST_BANK ) - GetSlotSize( CXSLInventory::ST_BANK ) ) < nSlot )
	{
		return false;
	}	

	return true;
}
//}}

void KInventory::ExpandSlot( int iCategory, int iSlot, int& iExpanded )
{
    iExpanded = 0;

    switch( iCategory )
    {
    case CXSLInventory::ST_EQUIP:
        break;
    case CXSLInventory::ST_ACCESSORY:
        break;
    case CXSLInventory::ST_MATERIAL:
        break;
    case CXSLInventory::ST_SPECIAL:
        break;
    case CXSLInventory::ST_QUEST:
        break;
    case CXSLInventory::ST_QUICK_SLOT:
        break;
		//{{ 2009. 8. 7  최육사		은행
	case CXSLInventory::ST_BANK:
		break;
		//}}
		//{{ 2011. 12. 13  김민성	퀵 슬롯 확장 아이템
#ifdef SERV_EXPAND_QUICK_SLOT
	case CXSLInventory::ST_E_QUICK_SLOT:
		break;
#endif SERV_EXPAND_QUICK_SLOT
		//}}
    default:
        START_LOG( cerr, L"확장할 수 있는 카테고리가 아님." )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlot )
            << END_LOG;
        return;
    }

    iExpanded = std::min< int >( GetMaxSlotSize( iCategory ) - GetSlotSize( iCategory ), iSlot );

	//{{ 2010. 07. 12  허상형	크래쉬 버그 수정
	if( iExpanded < 0 )
	{
		START_LOG( cerr, L"최대 인벤 개수보다 현재 슬롯 개수가 더 많음!! 있으면 안되는 오류" )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iSlot )
			<< END_LOG;

		iExpanded = 0;
	}
	//}}

    // 여기까지 왔다면 카테고리가 올바르다는 것이므로 인덱스 검사를 따로 하지 않는다.
    m_vecInventorySlot[iCategory].insert( m_vecInventorySlot[iCategory].end(), iExpanded, 0 );
}

int KInventory::CheckItemForTrade( UidType iItemUID, int iQuantity, KInventoryItemInfo& kInventoryItemInfo )
{
	_JIF( GetInventoryItemInfo( iItemUID, kInventoryItemInfo ), return NetError::ERR_TRADE_03 );
	_JIF( CXSLInventory::IsTradePossibleCategory( kInventoryItemInfo.m_cSlotCategory ), return NetError::ERR_TRADE_07 );
	//{{ 2008. 10. 16  최육사	마이너스 수량은 등록 못하도록
	_JIF( iQuantity > 0, return NetError::ERR_TRADE_03 );
	//}}

	//{{ 2008. 12. 21  최육사	사용불가 검사
	if( IsBrokenItem( iItemUID ) )
	{
		START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return NetError::ERR_RESTORE_ITEM_06;
	}	
	//}}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템을 거래하려고 했습니다." )
			<< BUILD_LOG( kInventoryItemInfo.m_kItemInfo.m_iItemID )
			<< END_LOG;

		return NetError::ERR_TRADE_03;
	}

#endif SERV_PC_BANG_PRE
	//}}

	if( kInventoryItemInfo.m_kItemInfo.m_iQuantity < iQuantity )
	{
		START_LOG( cerr, L"개인거래 및 개인상점에 등록하려는 상품수량이 이상합니다." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( kInventoryItemInfo.m_kItemInfo.m_iQuantity )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;

		return NetError::ERR_TRADE_03;
	}

	int iItemID = GetItemID( iItemUID );
	_JIF( iItemID > 0, return NetError::ERR_TRADE_03 );

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( !pItemTemplet )
	{
		START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return NetError::ERR_TRADE_03;
	}

	//{{ 2009. 8. 27  최육사	귀속인데 밀봉이면 체크를 하지 않는다
	if( pItemTemplet->m_bVested )
	{
		_JIF( kInventoryItemInfo.m_kItemInfo.IsSealedItem(), return NetError::ERR_TRADE_11 );
	}
	//}}

	if( kInventoryItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_ENDURANCE )
	{
		if( kInventoryItemInfo.m_kItemInfo.m_sEndurance < pItemTemplet->m_Endurance )
			return NetError::ERR_TRADE_14;
	}
	//{{ 2009. 1. 22  최육사	기간제 아이템의 경우 거래 불가능 하도록 예외처리
	else if( kInventoryItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_INFINITY )
	{
		if( kInventoryItemInfo.m_kItemInfo.m_sPeriod > 0 )
			return NetError::ERR_TRADE_11;
	}
	//}}

	return NetError::NET_OK;
}

//{{ 2009. 2. 10  최육사	개인상점 버그예방
bool KInventory::CheckTradeItemOut( IN const std::vector< KInventoryItemInfo >& vecItemOut, OUT KItemQuantityUpdate& kUpdateTemp, IN CXSLRoom::ROOM_TYPE eType )
{
	SET_ERROR( ERR_UNKNOWN );

	//kUpdateTemp.m_mapQuantityChange.clear();
	//kUpdateTemp.m_vecDeleted.clear();
	
	if( eType != CXSLRoom::RT_TRADE  &&  eType != CXSLRoom::RT_PERSONAL_SHOP )
	{
		START_LOG( cerr, L"잘못된 타입을 넣었음." )
			<< BUILD_LOG( static_cast<int>(eType) )
			<< END_LOG;

		SET_ERROR( ERR_UNKNOWN );
		return false;
	}

	bool bPersonalShop = ( eType != CXSLRoom::RT_TRADE );

	//////////////////////////////////////////////////////////////////////////
	std::vector< KInventoryItemInfo >::const_iterator vit;
	for( vit = vecItemOut.begin(); vit != vecItemOut.end(); ++vit )
	{
		if( vit->m_iItemUID <= 0 )
		{
			if( bPersonalShop )
			{
				START_LOG( cerr, L"개인상점의 경우 반드시 ItemUID가 정상값이어야 한다." )
					<< BUILD_LOG( vit->m_iItemUID )
					<< END_LOG;

				SET_ERROR( ERR_PERSONAL_SHOP_15 );
				return false;
			}
			else
			{
				continue;
			}
		}

#ifdef	SERV_SHARING_BANK_TEST
		if( IsShareItem(vit->m_iItemUID) == true )
		{
			START_LOG( cerr, L"은행 공유 : 은행 공유 아이템을 거래 하려 했음." )
				<< BUILD_LOG( vit->m_iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_SHARE_BANK_02 );
			return false;
		}
#endif	SERV_SHARING_BANK_TEST

		KInventoryItemInfo kInfo;
		if( !GetInventoryItemInfo( vit->m_iItemUID, kInfo ) )
		{
			START_LOG( cerr, L"이건.. 해킹인가? 아이템이 없다?" )
				<< BUILD_LOG( vit->m_iItemUID )
				<< END_LOG;

			if( bPersonalShop )
				SET_ERROR( ERR_PERSONAL_SHOP_15 );
			else
				SET_ERROR( ERR_TRADE_05 );

			return false;
		}

		if( kInfo.m_kItemInfo.m_iQuantity < vit->m_kItemInfo.m_iQuantity )
		{
			START_LOG( cerr, L"이건.. 해킹인가? 수량이 안맞다?" )
				<< BUILD_LOG( vit->m_kItemInfo.m_iQuantity )
				<< BUILD_LOG( kInfo.m_kItemInfo.m_iQuantity )
				<< END_LOG;

			if( bPersonalShop )
				SET_ERROR( ERR_PERSONAL_SHOP_15 );
			else
				SET_ERROR( ERR_TRADE_05 );

			return false;
		}
		else if( kInfo.m_kItemInfo.m_iQuantity == vit->m_kItemInfo.m_iQuantity )
		{
			kUpdateTemp.m_vecDeleted.push_back( vit->m_iItemUID );
		}
		else
		{
			if( kUpdateTemp.m_mapQuantityChange.find( vit->m_iItemUID ) != kUpdateTemp.m_mapQuantityChange.end() )
			{
				START_LOG( cerr, L"ItemUID가 중복되는것이 있다!" )
					<< BUILD_LOG( vit->m_iItemUID )
					<< END_LOG;

				if( bPersonalShop )
					SET_ERROR( ERR_PERSONAL_SHOP_15 );
				else
					SET_ERROR( ERR_TRADE_05 );

                return false;
			}

			kUpdateTemp.m_mapQuantityChange.insert( std::make_pair( vit->m_iItemUID, ( -vit->m_kItemInfo.m_iQuantity ) ) );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool KInventory::CheckTradeItemIn( IN const std::vector< KInventoryItemInfo >& vecItemIn, OUT KItemQuantityUpdate& kUpdateTemp, OUT std::vector< KItemInfo >& vecUpdateTemp, IN CXSLRoom::ROOM_TYPE eType )
{
	SET_ERROR( ERR_UNKNOWN );

	//kUpdateTemp.m_mapQuantityChange.clear();
	//kUpdateTemp.m_vecDeleted.clear();
	//vecUpdateTemp.clear();

	if( eType != CXSLRoom::RT_TRADE  &&  eType != CXSLRoom::RT_PERSONAL_SHOP )
	{
		START_LOG( cerr, L"잘못된 타입을 넣었음." )
			<< BUILD_LOG( static_cast<int>(eType) )
			<< END_LOG;

		SET_ERROR( ERR_UNKNOWN );
		return false;
	}

	bool bPersonalShop = ( eType != CXSLRoom::RT_TRADE );

	//////////////////////////////////////////////////////////////////////////	
	std::map< int, int > mapItem;
	std::map< int, int >::iterator mitFI;

	std::vector< KInventoryItemInfo >::const_iterator vit;
	for( vit = vecItemIn.begin(); vit != vecItemIn.end(); ++vit )
	{
		if( vit->m_iItemUID <= 0 )
		{
			if( bPersonalShop )
			{
				START_LOG( cerr, L"개인상점의 경우 반드시 ItemUID가 정상값이어야 한다." )
					<< BUILD_LOG( vit->m_iItemUID )
					<< END_LOG;

				SET_ERROR( ERR_PERSONAL_SHOP_17 );
				return false;
			}
			else
			{
				continue;
			}
		}

		//{{ 2009. 5. 6  최육사		아이템 소실 버그 수정
		mitFI = mapItem.find( vit->m_kItemInfo.m_iItemID );
		if( mitFI == mapItem.end() )
		{
			mapItem.insert( std::make_pair( vit->m_kItemInfo.m_iItemID, vit->m_kItemInfo.m_iQuantity ) );
		}
		else
		{
			mitFI->second += vit->m_kItemInfo.m_iQuantity;
		}
		//}}
	}
	
	if( !IsEnoughSpaceExist( mapItem ) )
	{
		//if( kPacket_.m_kItemOut.m_iSlotCategory == kPacket_.m_kItemIn.m_iSlotCategory )
		//{
		//	if( kInfo.m_kItemInfo.m_iQuantity == kPacket_.m_kItemOut.m_kItemInfo.m_iQuantity )
		//	{
		//		kPacketAck.m_iOK = NetError::NET_OK;
		//		_SendToCnRoom( m_iTradeUID, ERM_CONFIRM_TRADE_ACK, kPacketAck );
		//		return;
		//	}
		//}

		if( bPersonalShop )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_17 );
		}
		else
		{
            SET_ERROR( ERR_TRADE_09 );
		}

		return false;
	}

	if( !TrialPrepareInsert( vecItemIn, kUpdateTemp, vecUpdateTemp ) )
	{
		START_LOG( cerr, L"인벤토리에 공간도 없는데 개인거래를 하려고 하네?" )
			<< BUILD_LOG( vecItemIn.size() )
			<< END_LOG;

		if( bPersonalShop )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_17 );
		}
		else
		{
            SET_ERROR( ERR_TRADE_09 );
		}

		return false;
	}

    return true;
}

bool KInventory::TrialPrepareInsert( IN const std::vector< KInventoryItemInfo >& vecReward, OUT KItemQuantityUpdate& kUpdateTemp, OUT std::vector< KItemInfo >& vecUpdateTemp )
{
	//kUpdateTemp.m_mapQuantityChange.clear();
	//kUpdateTemp.m_vecDeleted.clear();
	//vecNewItem.clear();

	std::vector< KInventoryItemInfo >::const_iterator vitR;

	//////////////////////////////////////////////////////////////////////////
	// 개인거래 아이템들
	std::vector< KItemInfo > vecRewardOnlyOne;

	// 수량성 템만 따로
	std::map< int, KItemInfo >::iterator mitRQ;
	std::map< int, KItemInfo > mapRewardQuantity;

	// 1. 수량성과 수량성 아닌것을 분리한다
	for( vitR = vecReward.begin(); vitR != vecReward.end(); ++vitR )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( vitR->m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( vitR->m_kItemInfo.m_iItemID )
				<< END_LOG;
			continue;
		}

		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			mitRQ = mapRewardQuantity.find( vitR->m_kItemInfo.m_iItemID );
			if( mitRQ == mapRewardQuantity.end() )
			{
				mapRewardQuantity.insert( std::make_pair( vitR->m_kItemInfo.m_iItemID, vitR->m_kItemInfo ) );
			}
			else
			{
				mitRQ->second.m_iQuantity += vitR->m_kItemInfo.m_iQuantity;
			}
		}
		else
		{
			vecRewardOnlyOne.push_back( vitR->m_kItemInfo );
		}
	}

	// 2. 수량성 아이템을 먼저 처리함
	if( !mapRewardQuantity.empty() )
	{
		if( !TrialPrepareInsert( mapRewardQuantity, kUpdateTemp, vecUpdateTemp ) )
		{
			START_LOG( cerr, L"개인거래중 TrialPrepareInsert 처리 실패! 일어날수 없는 에러!" )
				<< BUILD_LOG( mapRewardQuantity.size() )
				<< END_LOG;			
		}
	}

	//{{ 2012. 07. 9	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	// 임시로 채우기를 하기 위한 카테고리 슬롯 정보의 복사본입니다.
	std::map< UidType, KInventoryItem >	mapItemCopy;
	std::vector< UidType >				vecInventorySlotCopy[CXSLInventory::ST_END];

	for( int i=0; i < CXSLInventory::ST_END; ++i )
	{
		vecInventorySlotCopy[i].resize( m_vecInventorySlot[i].size() );
		std::copy( m_vecInventorySlot[i].begin(), m_vecInventorySlot[i].end(), vecInventorySlotCopy[i].begin() );
	}
	std::copy( m_mapItem.begin(), m_mapItem.end(), std::inserter( mapItemCopy, mapItemCopy.begin() ) );
#else
	// 3. 인벤토리의 빈 칸 개수를 센다.
	std::map< int, int >::iterator mitReceived;
	std::map< int, int > mapEmptySlot;
	GetNumFreeSlot( mapEmptySlot );
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

	// 4. 남은 아이템을 DB에 신규 아이템으로 생성하기 위해 단위 수량씩 나누에 KItemInfo에 담는다.
	for( std::vector< KItemInfo >::iterator vitROO = vecRewardOnlyOne.begin(); vitROO != vecRewardOnlyOne.end(); ++vitROO )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( vitROO->m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< END_LOG;
			continue;
		}

		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			START_LOG( cerr, L"수량성은 여기 올수 없는데? 있을수 없는 에러!" )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< BUILD_LOGc( vitROO->m_cUsageType )
				<< END_LOG;

			continue;
		}

		if( vitROO->m_iQuantity > 1 )
		{
			START_LOG( cerr, L"수량성이 아닌데 수량이 1개가 넘는다?" )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< BUILD_LOG( vitROO->m_iQuantity )
				<< END_LOG;

			vitROO->m_iQuantity = 1; // 수량을 1개로 수정
		}

		short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
		if( sEndurance < 0 )
		{
			START_LOG( cerr, L"내구도 이상." )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< BUILD_LOG( sEndurance )
				<< END_LOG;
			continue;
		}

		//{{ 2012. 07. 9	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( vitROO->m_iItemID, vecCategoryID );

		BOOST_TEST_FOREACH( int, iCategory, vecCategoryID )
		{
			// 복사본에 삽입하기
			int iNumInserted = 0;
			if( InsertToEmptySlotForTest( iCategory, vitROO->m_iItemID, vitROO->m_iQuantity, &*vitROO, NULL, NULL, vecInventorySlotCopy, mapItemCopy, &vecUpdateTemp, NULL, iNumInserted ) == false )
			{
				START_LOG( cerr, L"복사본의 빈 슬롯에 집어넣기 실패! 이후의 결과를 예측할 수 없습니다.")
					<< END_LOG;
				break;
			}
			vitROO->m_iQuantity -= iNumInserted;

			if( vitROO->m_iQuantity <= 0 )
			{
				break;
			}
		}

		if( 0 < vitROO->m_iQuantity )
		{
			START_LOG( cerr, L"어라? 빈공간을 미리 계산했을텐데.." )
				<< BUILD_LOG( vitROO->m_iItemID )
				<< BUILD_LOG( vitROO->m_iQuantity )
				<< END_LOG;
			return false;
		}
#else
		while( vitROO->m_iQuantity > 0 )
		{
			std::map< int, int >::iterator mitEmptySlot;
			mitEmptySlot = mapEmptySlot.find( GetSuitableCategory( vitROO->m_iItemID ) );
			if( mitEmptySlot == mapEmptySlot.end() )
			{
				START_LOG( cerr, L"어라? 빈공간을 미리 계산했을텐데.." )
					<< BUILD_LOG( vitROO->m_iItemID )
					<< BUILD_LOG( vitROO->m_iQuantity )
					<< END_LOG;
				return false;
			}

			if( mitEmptySlot->second <= 0 )
			{
				START_LOG( cerr, L"어라? 빈공간을 미리 계산했을텐데.." )
					<< BUILD_LOG( vitROO->m_iItemID )
					<< BUILD_LOG( vitROO->m_iQuantity )
					<< END_LOG;
				return false;
			}

			KItemInfo kInfo;
			kInfo.m_iItemID			= vitROO->m_iItemID;
			kInfo.m_cUsageType		= pItemTemplet->m_PeriodType;
			kInfo.m_iQuantity		= 1; // 수량성이 아니므로 무조건 한개임
			kInfo.m_sEndurance		= std::min< int >( vitROO->m_sEndurance, sEndurance );
			kInfo.m_sPeriod			= vitROO->m_sPeriod;
			kInfo.m_cEnchantLevel	= std::min< char >( vitROO->m_cEnchantLevel, (char)(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL) );
			kInfo.m_kAttribEnchantInfo = vitROO->m_kAttribEnchantInfo;
			kInfo.m_vecItemSocket	= vitROO->m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			kInfo.m_byteExpandedSocketNum	= vitROO->m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
			//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			kInfo.m_vecRandomSocket	= vitROO->m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
			//{{ 2009. 9. 7  최육사		밀봉
			kInfo.m_ucSealData		= vitROO->m_ucSealData;
			//}}
			vecUpdateTemp.push_back( kInfo );

			--vitROO->m_iQuantity;
			--mitEmptySlot->second;
		}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}
	}	

	return true;
}

//{{ 2012. 07. 9	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
bool KInventory::TrialPrepareInsert( IN const std::map< int, KItemInfo >& mapReward, OUT KItemQuantityUpdate& kUpdateTemp, OUT std::vector< KItemInfo >& vecUpdateTemp )
{
	//kUpdateTemp.m_mapQuantityChange.clear();
	//kUpdateTemp.m_vecDeleted.clear();
	//vecNewItem.clear();

	std::map< int, KItemInfo > mapRewardCopy = mapReward;
	std::map< int, KItemInfo >::iterator mitReward;
	std::map< int, int >::iterator mitReceived;
	std::map< UidType, int >::iterator mitInc;

	// 1. 새 UID를 만들지 않고 수량을 채울 수 있는지 찾아본다.

	// 임시로 채우기를 하기 위한 카테고리 슬롯 정보의 복사본입니다.
	std::map< UidType, KInventoryItem >	mapItemCopy;
	std::vector< UidType >				vecInventorySlotCopy[CXSLInventory::ST_END];

	for( int i=0; i < CXSLInventory::ST_END; ++i )
	{
		vecInventorySlotCopy[i].resize( m_vecInventorySlot[i].size() );
		std::copy( m_vecInventorySlot[i].begin(), m_vecInventorySlot[i].end(), vecInventorySlotCopy[i].begin() );
	}
	std::copy( m_mapItem.begin(), m_mapItem.end(), std::inserter( mapItemCopy, mapItemCopy.begin() ) );

	for( mitReward = mapRewardCopy.begin(); mitReward != mapRewardCopy.end(); ++mitReward )
	{
		const int iItemID	= mitReward->first;
		int& iQuantity		= mitReward->second.m_iQuantity;

		_JIF( 0 < iQuantity, continue );

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}

		int iMaxQuantity = ( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY ? pItemTemplet->m_Quantity : 1 );
		if( iMaxQuantity <= 0 )
		{
			START_LOG( cerr, L"수량 이상." )
				<< BUILD_LOG( iMaxQuantity )
				<< END_LOG;
			continue;
		}

		short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
		if( sEndurance < 0 )
		{
			START_LOG( cerr, L"내구도 이상." )
				<< BUILD_LOG( sEndurance )
				<< END_LOG;
			continue;
		}

		// 아이템의 카테고리 정보를 얻어오자
		std::vector<int> vecCategoryID;
		GetSuitableCategory( iItemID, vecCategoryID );

		BOOST_TEST_FOREACH( int, iCategory, vecCategoryID )
		{
			if( IsValidCategory( iCategory ) == false )
				continue;

			if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
			{
				// 복사본에 채워넣기
				int iNumIncreasedCopy = 0;
				if( FillTheUnFullSlot( iCategory, iItemID, iQuantity, vecInventorySlotCopy, mapItemCopy, &kUpdateTemp, NULL, NULL, iNumIncreasedCopy ) == false )
				{
					START_LOG( cerr, L"채워넣기 실패! 이후의 결과를 예측할 수 없습니다.")
						<< END_LOG;
					continue;
				}
				iQuantity -= iNumIncreasedCopy;

				if( iQuantity <= 0 )
				{
					break;
				}
			}

			// 복사본에 삽입하기
			int iNumInserted = 0;
			if( InsertToEmptySlotForTest( iCategory, iItemID, iQuantity, &mitReward->second, NULL, NULL, vecInventorySlotCopy, mapItemCopy, &vecUpdateTemp, NULL, iNumInserted ) == false )
			{
				START_LOG( cerr, L"복사본의 빈 슬롯에 집어넣기 실패! 이후의 결과를 예측할 수 없습니다.")
					<< END_LOG;
				break;
			}
			iQuantity -= iNumInserted;

			if( iQuantity <= 0 )
			{
				break;
			}
		}
		
		if( 0 < iQuantity )
		{
			return false;
		}
	}

	return true;
}
#else
bool KInventory::TrialPrepareInsert( IN const std::map< int, KItemInfo >& mapReward, OUT KItemQuantityUpdate& kUpdateTemp, OUT std::vector< KItemInfo >& vecUpdateTemp )
{
	//kUpdateTemp.m_mapQuantityChange.clear();
	//kUpdateTemp.m_vecDeleted.clear();
	//vecNewItem.clear();

	int iCategory, iSlotID;
	std::map< int, KItemInfo > mapRewardCopy = mapReward;
	std::map< int, KItemInfo >::iterator mitReward;
	std::map< int, int >::iterator mitReceived;
	std::map< UidType, int >::iterator mitInc;

	// 1. 새 UID를 만들지 않고 수량을 채울 수 있는지 찾아본다.
	for( mitReward = mapRewardCopy.begin(); mitReward != mapRewardCopy.end(); ++mitReward )
	{
		_JIF( mitReward->second.m_iQuantity > 0, continue );
		iCategory = GetSuitableCategory( mitReward->first );
		if( iCategory < CXSLInventory::ST_EQUIP  ||  iCategory > CXSLInventory::ST_AVARTA )
			continue;

		// 모든 슬롯을 검사
		for( iSlotID = 0; iSlotID < GetSlotSize( iCategory ); ++iSlotID )
		{
			if( IsEmptySlot( iCategory, iSlotID ) )
				continue;

			KInventoryItemInfo kInventoryItemInfo;
			if( GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo ) )
			{
				if( kInventoryItemInfo.m_kItemInfo.m_iItemID == mitReward->first )
				{
					// 1-1. 수량성이 아니면 통과
					if( kInventoryItemInfo.m_kItemInfo.m_cUsageType != CXSLItem::PT_QUANTITY )
					{
						START_LOG( clog, L"수량성 아이템이 아님." )
							<< BUILD_LOGc( kInventoryItemInfo.m_kItemInfo.m_cUsageType )
							<< END_LOG;
						continue;
					}

					// 1-2. 수량값 검증
					if( kInventoryItemInfo.m_kItemInfo.m_iQuantity <= 0 )
					{
						START_LOG( cerr, L"수량 값이 이상함." )
							<< BUILD_LOG( kInventoryItemInfo.m_kItemInfo.m_iQuantity )
							<< END_LOG;
						continue;
					}

					// 1-3. 얼만큼 더 수량증가가 가능한지?
					int iRemainedCapacity = GetRemainedQuantityCapacity( kInventoryItemInfo.m_iItemUID );
					int iIncreased = std::min< int >( iRemainedCapacity, mitReward->second.m_iQuantity );
					if( iIncreased <= 0 )
						continue;

					// 1-4. Temp정보에 넣자!
					mitInc = kUpdateTemp.m_mapQuantityChange.find( kInventoryItemInfo.m_iItemUID );
					if( mitInc == kUpdateTemp.m_mapQuantityChange.end() )
					{
						kUpdateTemp.m_mapQuantityChange.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, iIncreased ) );
					}
					else
					{
						mitInc->second += iIncreased;
					}

					// 1-5. 증가시킨 만큼 감소
					mitReward->second.m_iQuantity -= iIncreased;

					// 1-6. 더이상 남은 수량 없으면 다음 아이템으로
					if( mitReward->second.m_iQuantity <= 0 )
						break;
				}
			}
		}
	}

	// 2. 인벤토리의 빈 칸 개수를 센다.
	std::map< int, int > mapEmptySlot;
	GetNumFreeSlot( mapEmptySlot );

	// 3. 남은 아이템을 DB에 신규 아이템으로 생성하기 위해 단위 수량씩 나누에 KItemInfo에 담는다.
	for( mitReward = mapRewardCopy.begin(); mitReward != mapRewardCopy.end(); ++mitReward )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mitReward->first );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( mitReward->first )
				<< END_LOG;
			continue;
		}

		int iQuantity = ( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY ? pItemTemplet->m_Quantity : 1 );
		if( iQuantity <= 0 )
		{
			START_LOG( cerr, L"수량 이상." )
				<< BUILD_LOG( mitReward->first )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;
			continue;
		}

		short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
		if( sEndurance < 0 )
		{
			START_LOG( cerr, L"내구도 이상." )
				<< BUILD_LOG( mitReward->first )
				<< BUILD_LOG( sEndurance )
				<< END_LOG;
			continue;
		}

		while( mitReward->second.m_iQuantity > 0 )
		{
			std::map< int, int >::iterator mitEmptySlot;
			mitEmptySlot = mapEmptySlot.find( GetSuitableCategory( mitReward->first ) );
			if( mitEmptySlot == mapEmptySlot.end() )
				return false;

			if( mitEmptySlot->second <= 0 )
				return false;

			iQuantity = std::min< int >( mitReward->second.m_iQuantity, iQuantity );

			KItemInfo kInfo;
			kInfo.m_iItemID			= mitReward->first;
			kInfo.m_cUsageType		= pItemTemplet->m_PeriodType;
			kInfo.m_iQuantity		= iQuantity;
			kInfo.m_sEndurance		= std::min< int >( mitReward->second.m_sEndurance, sEndurance );
			kInfo.m_sPeriod			= mitReward->second.m_sPeriod;
			kInfo.m_cEnchantLevel	= std::min< char >( mitReward->second.m_cEnchantLevel, (char)(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL) );
			kInfo.m_kAttribEnchantInfo = mitReward->second.m_kAttribEnchantInfo;
			kInfo.m_vecItemSocket	= mitReward->second.m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			kInfo.m_byteExpandedSocketNum	= mitReward->second.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
			//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			kInfo.m_vecRandomSocket	= mitReward->second.m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
			//{{ 2009. 9. 7  최육사		밀봉
			kInfo.m_ucSealData		= mitReward->second.m_ucSealData;
			//}}
			vecUpdateTemp.push_back( kInfo );

			mitReward->second.m_iQuantity -= iQuantity;
			mitEmptySlot->second--;
		}
	}

	return true;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}
//}}

//{{ 2010. 02. 08  최육사	PC방 채널 이동 오류 수정
void KInventory::GetChannelChangeInventoryInfo( OUT std::vector< KInventoryItemInfo >& vecPcBangItemInfo )
{
    vecPcBangItemInfo.clear();

	// PC방 전용 아이템의 정보를 담는다.

	std::map< UidType, KInventoryItem >::iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		if( mit->second.GetInventoryCategory() == CXSLInventory::ST_E_EQUIP  ||
			mit->second.GetInventoryCategory() == CXSLInventory::ST_PC_BANG )
		{
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->second.GetItemID() );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"아이템 정보가 이상합니다." )
					<< BUILD_LOG( mit->second.GetItemID() )
					<< END_LOG;

				continue;
			}

			// PC방 전용 아이템인지 검사
			if( pItemTemplet->m_bIsPcBang == false )
				continue;

			KInventoryItemInfo kInfo;
			mit->second.GetInventoryItemInfo( kInfo );

			// ItemID가 key값이다. PC방 전용 아이템은 중복되는 ItemID가 없다는 전제하에 채널이동 동작해야한다.
			vecPcBangItemInfo.push_back( kInfo );
		}
	}
}

void KInventory::SetChannelChangeInventoryInfo( IN const std::vector< KInventoryItemInfo >& vecPcBangItemInfo, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	vecInventorySlotInfo.clear();
	
	for( u_int ui = 0; ui < vecPcBangItemInfo.size(); ++ui )
	{
		const KInventoryItemInfo& kInfo = vecPcBangItemInfo[ui];

		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		if( VerifyCategorySlotID( kInfo.m_cSlotCategory, kInfo.m_sSlotID ) == false )
#else
		if( VerifyCategorySlotID( kInfo.m_cSlotCategory, kInfo.m_cSlotID ) == false )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		{
			START_LOG( cerr, L"PC방 아이템을 넣으려는데 슬롯 위치가 이상하다? 있을수없는에러!" )
				<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kInfo.m_cSlotCategory )
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				<< BUILD_LOGc( kInfo.m_sSlotID )
#else
				<< BUILD_LOGc( kInfo.m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				<< END_LOG;
			continue;
		}

		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		if( IsEmptySlot( kInfo.m_cSlotCategory, kInfo.m_sSlotID ) == false )
#else
		if( IsEmptySlot( kInfo.m_cSlotCategory, kInfo.m_cSlotID ) == false )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		{
			START_LOG( cerr, L"PC방 아이템을 넣으려는데 슬롯이 비어있지 않다? 있을수 없는에러!" )
				<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kInfo.m_cSlotCategory )
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				<< BUILD_LOGc( kInfo.m_sSlotID )
#else
				<< BUILD_LOGc( kInfo.m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				<< END_LOG;
			continue;
		}

		KInventoryItem kItem;
		kItem.Init( kInfo, false );
		kItem.NotDBUpdateItem(); // DB에 업데이트 하지 않는 아이템으로 세팅
		m_mapItem.insert( std::make_pair( kInfo.m_iItemUID, kItem ) );
		m_vecInventorySlot[kItem.GetInventoryCategory()][kItem.GetSlotID()] = kItem.GetItemUID();
	}
}
//}}

//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

void KInventory::InitPetInventory( IN UidType iSummonedPetUID, IN int iPetCategorySlotSize, IN const std::map< UidType, KInventoryItemInfo >& mapPetItem, OUT std::vector< KInventoryItemInfo >& vecPetInventorySlotInfo )
{
	vecPetInventorySlotInfo.clear();

	// 소환된 펫이 없다면 패스!
	if( iSummonedPetUID == 0 )
	{
		START_LOG( cerr, L"소환된 펫도 없는데 펫 인벤토리를 초기화 하려고 하네?" )
			<< BUILD_LOG( iSummonedPetUID )
			<< BUILD_LOG( iPetCategorySlotSize )
			<< BUILD_LOG( mapPetItem.size() )
			<< END_LOG;
		return;
	}
    
	// 펫 인벤토리 크기 정리
	m_vecInventorySlot[CXSLInventory::ST_PET].clear();
	m_vecInventorySlot[CXSLInventory::ST_PET].insert( m_vecInventorySlot[CXSLInventory::ST_PET].end(), iPetCategorySlotSize, 0 );

	// 카테고리 검사!
	BOOST_MAP_CONST_FOREACH( UidType, KInventoryItemInfo, kDB, mapPetItem )
	{
		// 펫 카테고리 아이템인지 검사
		if( kDB.second.m_cSlotCategory != CXSLInventory::ST_PET )
		{
			START_LOG( cerr, L"펫 아이템이 아닌데 여기에 들어왔다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kDB.second.m_iItemUID )
				<< END_LOG;
			continue;
		}

		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		if( VerifyCategorySlotID( kDB.second.m_cSlotCategory, kDB.second.m_sSlotID ) )
#else
		if( VerifyCategorySlotID( kDB.second.m_cSlotCategory, kDB.second.m_cSlotID ) )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		{
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			if( IsEmptySlot( kDB.second.m_cSlotCategory, kDB.second.m_sSlotID ) )
#else
			if( IsEmptySlot( kDB.second.m_cSlotCategory, kDB.second.m_cSlotID ) )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
			{
				KInventoryItem kItem;
				kItem.Init( kDB.second, false );
				kItem.UpdatePetUID( iSummonedPetUID ); // 소환한 펫의 UID값을 아이템 정보에 넣는다!
				m_mapItem.insert( std::make_pair( kDB.first, kItem ) );
				m_vecInventorySlot[kItem.GetInventoryCategory()][kItem.GetSlotID()] = kItem.GetItemUID();
				continue;
			}
		}
		else
		{
			START_LOG( cerr, L"인벤토리 슬롯 위치 이상." )
				<< BUILD_LOG( m_pUser->GetCharName() )
				<< BUILD_LOG( m_pUser->GetCharUID() )
				<< BUILD_LOG( kDB.second.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kDB.second.m_cSlotCategory )
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				<< BUILD_LOG( kDB.second.m_sSlotID )
#else
				<< BUILD_LOGc( kDB.second.m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				<< END_LOG;
		}
	}

	// 클라이언트에게 넘겨줄 펫 카테고리에 있는 아이템들 정보를 얻자!
	GetInventoryInfo( CXSLInventory::ST_PET, vecPetInventorySlotInfo, true );
}

void KInventory::ResetPetInventorySize( IN int iPetCategorySlotSize )
{
	const u_int uiPetCategorySlotSize = static_cast<u_int>(iPetCategorySlotSize);

	// 확장하려는 펫 인벤토리 크기 검증
	if( m_vecInventorySlot[CXSLInventory::ST_PET].size() > uiPetCategorySlotSize )
	{
		START_LOG( cerr, L"현재 인벤 크기보다 변경되는 인벤 크기가 더 커야하는데 아니네? 있을 수 없는 에러!" )
			<< BUILD_LOG( m_vecInventorySlot[CXSLInventory::ST_PET].size() )
			<< BUILD_LOG( uiPetCategorySlotSize )
			<< END_LOG;
		return;
	}
	
	// 펫 인벤토리 확장!
	const u_int uiAddPetCategorySlotSize = uiPetCategorySlotSize - m_vecInventorySlot[CXSLInventory::ST_PET].size();
	for( u_int ui = 0; ui < uiAddPetCategorySlotSize ; ++ui  )
	{
		m_vecInventorySlot[CXSLInventory::ST_PET].push_back( 0 );
	}

	// 결과 검사!
	if( uiPetCategorySlotSize != m_vecInventorySlot[CXSLInventory::ST_PET].size() )
	{
		START_LOG( cerr, L"현재 펫 인벤 크기와 실제 펫 이벤 슬롯 갯수가 맞지 않는다! 절재 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iPetCategorySlotSize )
			<< BUILD_LOG( uiPetCategorySlotSize )
			<< BUILD_LOG( uiAddPetCategorySlotSize )
			<< BUILD_LOG( m_vecInventorySlot[CXSLInventory::ST_PET].size() )
			<< END_LOG;
	}
}

void KInventory::RemovePetInventory( OUT std::map< UidType, KInventoryItemInfo >& mapRemovedPetItem )
{
	mapRemovedPetItem.clear();

	BOOST_TEST_FOREACH( UidType&, iItemUID, m_vecInventorySlot[CXSLInventory::ST_PET] )
	{
		if( iItemUID == 0 )
			continue;

		// 해당 아이템 정보 인벤에서 삭제
		KInventoryItemInfo kInfo;
		if( GetInventoryItemInfo( iItemUID, kInfo ) == false )
		{
			START_LOG( cerr, L"슬롯정보에는 있는데 실제 아이템 정보는 없네? 있을 수 없는에러!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// 아이템 정보를 담고
		mapRemovedPetItem.insert( std::make_pair( iItemUID, kInfo ) );

		// 해당 아이템을 지우자!
		m_mapItem.erase( iItemUID );

		// 인벤 슬롯에서도 삭제!
		iItemUID = 0;
	}
}

bool KInventory::FeedPetInInventory( IN UidType iItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	vecInventorySlotInfo.clear();

	// 아이템 검사
	KInventoryItemInfo kFeedItemInfo;
	if( GetInventoryItemInfo( iItemUID, kFeedItemInfo ) == false )
	{
		START_LOG( cerr, L"아이템이 있는지 검사! 미리 검사하고 들어왔을텐데 이상하군.." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;
		return false;
	}

	// 수량성인지 아닌지 체크
	if( kFeedItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
	{
		// 수량성 먹이는 한번에 1개만 먹자!
		int iDecreaseQuantity = 0;
		DecreaseQuantity( iItemUID, 1, iDecreaseQuantity, KDeletedItemInfo::DR_PET_FEED );

		KInventoryItemInfo kInfo;
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		LIF( GetInventorySlotInfo( kFeedItemInfo.m_cSlotCategory, kFeedItemInfo.m_sSlotID, kInfo ) );
#else
		LIF( GetInventorySlotInfo( kFeedItemInfo.m_cSlotCategory, kFeedItemInfo.m_cSlotID, kInfo ) );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		vecInventorySlotInfo.push_back( kInfo );
	}
	else
	{
		// 일반 먹이는 바로 먹자!
		KInventoryItemInfo kInfo;
		LIF( DeleteItem( iItemUID, kInfo, KDeletedItemInfo::DR_PET_FEED ) );
		vecInventorySlotInfo.push_back( kInfo );
	}

	// 아이템 통계
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kFeedItemInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_PET_FEED, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_PET_FEED, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
	return true;
}

bool KInventory::AutoFeedInPetInventory( IN u_char ucLevel, IN bool bOnlyEatSpecial, OUT int& iFeedItemID, OUT std::vector< KInventoryItemInfo >& vecPetInventorySlotInfo )
{
	iFeedItemID = 0;
	vecPetInventorySlotInfo.clear();
	
	UidType iEquipItemUID = 0;
	int iEquipItemPrice = INT_MAX;
	UidType iPetFeedItemUID = 0;
	CXSLItem::ITEM_GRADE ePetFeedItemGrade = CXSLItem::IG_NONE;

	BOOST_TEST_FOREACH( const UidType, iItemUID, m_vecInventorySlot[CXSLInventory::ST_PET] )
	{
		if( iItemUID == 0 )
			continue;

		// ItemID얻기
		const int iItemID = GetItemID( iItemUID );
		if( iItemID == 0 )
		{
			START_LOG( cerr, L"아이템 정보 찾기 실패!" )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}

		// 스페셜 먹이만 먹는지 체크
		if( bOnlyEatSpecial == true )
		{
			if( SiCXSLPetManager()->IsSpecialFeedItemID( iItemID ) == false )
			{
				// 스페셜 먹이가 아니라면 패스!
				continue;
			}
		}

		// 아이템 정보 얻기
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 정보가 없습니다." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}
		
		// 패션 아이템은 먹지 않는다!
		if( pItemTemplet->m_bFashion == true )
			continue;

		switch( pItemTemplet->m_ItemType )
		{
		case CXSLItem::IT_WEAPON:
		case CXSLItem::IT_DEFENCE:
			{
				// 아이템 착용 레벨이 캐릭터 레벨보다 11렙 낮으면 먹이로 줄수 없다.
				if( static_cast<int>(ucLevel) > pItemTemplet->m_UseLevel )
				{
					if( ( static_cast<int>(ucLevel) - pItemTemplet->m_UseLevel ) > CXSLPetManager::PE_PET_FEED_ITEM_LEVEL )
					{
						continue;
					}
				}

				// 가격이 가장 싼것을 고르자!
				if( iEquipItemPrice > pItemTemplet->m_Price )
				{
                    iEquipItemUID = iItemUID;
					iEquipItemPrice = pItemTemplet->m_Price;
				}
			}
			break;

		case CXSLItem::IT_SPECIAL:
			{
				// 이미 장비템이 선택 되었다면 패스!
				if( iEquipItemUID != 0 )
					break;

				// 스페셜 먹이가 아니라면 패스!
				if( SiCXSLPetManager()->IsSpecialFeedItemID( iItemID ) == false )
					break;

				// 등급이 가장 낮은것을 고르자!
				if( ePetFeedItemGrade < pItemTemplet->m_ItemGrade )
				{
                    iPetFeedItemUID = iItemUID;
					ePetFeedItemGrade = pItemTemplet->m_ItemGrade;
				}
			}
			break;
		}
	}

	// 먹이 아이템
	UidType iSelectedFeedItemUID = 0;

	// 가장 가격이 낮은 장비템 찾기!
	if( iEquipItemUID != 0 )
	{
		iSelectedFeedItemUID = iEquipItemUID;
	}
	else
	{
		// 가장 등급이 낮은 펫 먹이 찾기!
		if( iPetFeedItemUID != 0 )
		{
			iSelectedFeedItemUID = iPetFeedItemUID;
		}
	}

	// 자동으로 먹이를 먹자!
	if( iSelectedFeedItemUID > 0 )
	{
		// 아이템 검사
		KInventoryItemInfo kFeedItemInfo;
		if( GetInventoryItemInfo( iSelectedFeedItemUID, kFeedItemInfo ) == false )
		{
			START_LOG( cerr, L"아이템이 있는지 검사! 미리 검사하고 들어왔을텐데 이상하군.." )
				<< BUILD_LOG( iSelectedFeedItemUID )
				<< END_LOG;
			return false;
		}

		// 아이템ID 얻기!
		iFeedItemID = kFeedItemInfo.m_kItemInfo.m_iItemID;

		// 수량성인지 아닌지 체크
		if( kFeedItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			// 수량성 먹이는 한번에 1개만 먹자!
			int iDecreaseQuantity = 0;
			DecreaseQuantity( iSelectedFeedItemUID, 1, iDecreaseQuantity, KDeletedItemInfo::DR_PET_AUTO_FEED );

			KInventoryItemInfo kInfo;
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			LIF( GetInventorySlotInfo( kFeedItemInfo.m_cSlotCategory, kFeedItemInfo.m_sSlotID, kInfo ) );
#else
			LIF( GetInventorySlotInfo( kFeedItemInfo.m_cSlotCategory, kFeedItemInfo.m_cSlotID, kInfo ) );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
			vecPetInventorySlotInfo.push_back( kInfo );
		}
		else
		{
			// 일반 먹이는 바로 먹자!
			KInventoryItemInfo kInfo;
			LIF( DeleteItem( iSelectedFeedItemUID, kInfo, KDeletedItemInfo::DR_PET_AUTO_FEED ) );
			vecPetInventorySlotInfo.push_back( kInfo );
		}

		// 아이템 통계
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( kFeedItemInfo.m_kItemInfo.m_iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_PET_FEED, 1 );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_PET_FEED, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
		return true;
	}
	
	return false;
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

void KInventory::GetResetPeriodItemUIDList( OUT std::map< UidType, short >& mapExpandPeriodItemList ) const
{
	mapExpandPeriodItemList.clear();

	// 기간 연장 아이템 리스트가 비어있으면 바로 리턴
	if( SiCXSLItemManager()->IsEmptyExpandPeriodItemList() == true )
		return;
    
	std::map< UidType, KInventoryItem >::const_iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		short sExpandPeriod = 0;

		// 기간 초기화 아이템인지 확인해서 연장되는 기간 정보를 얻자!
		if( SiCXSLItemManager()->GetExpandPeriodByItemID( mit->second.GetItemID(), sExpandPeriod ) == false )
			continue;

		mapExpandPeriodItemList.insert( std::make_pair( mit->second.GetItemUID(), sExpandPeriod ) );
	}
}

bool KInventory::UpdatePeriod( IN UidType iItemUID, IN short sPeriod, IN const std::wstring& wstrExpirationDate, OUT KInventoryItemInfo& kInventorySlotInfo )
{
	std::map< UidType, KInventoryItem >::iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"아이템 정보를 찾을 수 없습니다!" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( sPeriod )
			<< BUILD_LOG( wstrExpirationDate )
			<< END_LOG;
		return false;
	}

	// 기간 정보 업데이트
	mit->second.UpdatePeriod( sPeriod, wstrExpirationDate );

	// 인벤토리 정보 얻기
	LIF( GetInventorySlotInfo( mit->second.GetInventoryCategory(), mit->second.GetSlotID(), kInventorySlotInfo ) );
	return true;
}

#endif SERV_RESET_PERIOD_EVENT
//}}

//{{ 2011. 01. 04  김민성	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM

bool KInventory::AttribAttachItem( IN UidType iDestItemUID, IN UidType iAttachItemUID, OUT KItemAttributeEnchantInfo& kAttribAttachResult, OUT std::vector< KInventoryItemInfo >& vecUpdated )
{
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	// 1. 속성강화할 아이템 검사
	if( IsExist( iDestItemUID ) == false  ||  IsExist( iAttachItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( iAttachItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_00 );
		return false;
	}

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iDestItemUID ) == true  ||  IsExistInBank( iAttachItemUID ) == true )
	{
		START_LOG( cerr, L"은행에서는 속성강화 못하는데?" )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( iAttachItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	//{{ 2008. 12. 21  최육사	사용불가 검사
	if( IsBrokenItem( iDestItemUID ) == true )
	{
		START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_06 );
		return false;
	}
	//}}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iDestItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템을 속성강화하려고 했습니다." )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	// 2. 속성 부여될 아이템이 무기 또는 방어구인지 검사
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( GetItemID( iDestItemUID ) );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿이 존재하지 않습니다" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_06 );
		return false;
	}

	if( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON  &&
		pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE )
	{
		START_LOG( cerr, L"무기와 방어구만 속성강화 가능한데.." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_07 );
		return false;
	}

	// 3. 해당 속성강화에 필요한 ItemID얻기
	const int iAttribAttachItemID = GetItemID( iAttachItemUID );

	KItemAttributeEnchantInfo kAttribAttachInfo;
	if( SiCXSLAttribEnchantItem()->GetAttribAttachInfo( iAttribAttachItemID, kAttribAttachInfo ) == false )
	{
		START_LOG( cerr, L"속성 부여 아이템이 아닙니다. 클라이언트에서 검사 했을텐데.." )
			<< BUILD_LOG( iAttribAttachItemID )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
		return false;
	}

	// 4. 속성 부여 가능한 아이템인지 체크!
	if( kAttribAttachInfo.GetAttribEnchantCount() > pItemTemplet->m_iMaxAttribEnchantCount )
	{
		SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
		return false;
	}

	std::map< int, int > mapRequireItem; // 인벤토리에서 소모시킬 속성 부여 아이템 수량 세팅
	mapRequireItem.insert( std::make_pair( iAttribAttachItemID, 1 ) );
	
	// 5. 속성 부여 작업
	for( int iSlotID = CXSLAttribEnchantItem::ESI_SLOT_1; iSlotID < CXSLAttribEnchantItem::ESI_SLOT_MAX; ++iSlotID )
	{
		char cAttribEnchantType = CXSLAttribEnchantItem::ET_NONE;
		switch( iSlotID )
		{
		case CXSLAttribEnchantItem::ESI_SLOT_1: cAttribEnchantType = kAttribAttachInfo.m_cAttribEnchant0; break;
		case CXSLAttribEnchantItem::ESI_SLOT_2: cAttribEnchantType = kAttribAttachInfo.m_cAttribEnchant1; break;
		case CXSLAttribEnchantItem::ESI_SLOT_3: cAttribEnchantType = kAttribAttachInfo.m_cAttribEnchant2; break;
		}
		
		if( SetAttribEnchant( iDestItemUID, iSlotID, cAttribEnchantType ) == false )
		{
			START_LOG( cerr, L"이미 검사했는데 에러다?" )
				<< BUILD_LOG( iDestItemUID )
				<< BUILD_LOG( iSlotID )
				<< BUILD_LOGc( cAttribEnchantType )
				<< END_LOG;

			SET_ERROR( ERR_ATTRIB_ENCHANT_00 );
			return false;
		}
	}

	// 6. 부여에 성공하면 아이템 삭제
	if( DeleteItemOnlyInventory( mapRequireItem, vecUpdated, KDeletedItemInfo::DR_ATTRIB_ATTACH ) == false )
	{
		START_LOG( cerr, L"수량 검사를 했는데 삭제 실패? 일어날 수 없는 에러.." )
			<< BUILD_LOG( mapRequireItem.size() )
			<< BUILD_LOG( vecUpdated.size() )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
		return false;
	}

	// 7. 부여된 아이템 인벤정보 얻기
	{
		KInventoryItemInfo kInventoryItemInfo;
		GetInventoryItemInfo( iDestItemUID, kInventoryItemInfo );
		vecUpdated.push_back( kInventoryItemInfo );

		// 속성 정보 얻기
		kAttribAttachResult = kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo;
	}

	//////////////////////////////////////////////////////////////////////////
	// 통계
	{
		KStatisticsKey kKeyMS;
		kKeyMS.m_vecIntKey.push_back( iAttribAttachItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyMS, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyMS, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	SET_ERROR( NET_OK );
	return true;
}

#endif SERV_ATTRIBUTE_CHARM
//}}

//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
bool KInventory::DeleteItemForPersonalShopAgency( IN const std::vector< KSellPersonalShopItemInfo >& vecSellItemInfo, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	vecInventorySlotInfo.clear();

	std::map< UidType, int > mapToDelete;
	std::map< int, KItemInfo > mapToInsertDummy;
	std::vector< KItemInfo > vecNewItem;

	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellItemInfo, vecSellItemInfo )
	{
		KInventoryItemInfo kInfo;
		if( GetInventoryItemInfo( kSellItemInfo.m_kInventoryItemInfo.m_iItemUID, kInfo ) == false )
		{
			START_LOG( cerr, L"존재 하지 않는 아이템을 대리상인에 등록하려고 했다!" )
				<< BUILD_LOG( m_pUser->GetName() )
				<< BUILD_LOG( m_pUser->GetCharName() )
				<< BUILD_LOG( kSellItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< END_LOG;
			return false;
		}

		if( kInfo.m_kItemInfo.m_iQuantity < kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
		{
			START_LOG( cerr, L"인벤토리에 존재 하는 수량보다 더 많은 수량을 대리상인에 등록하려고 했다!" )
				<< BUILD_LOG( m_pUser->GetName() )
				<< BUILD_LOG( m_pUser->GetCharName() )
				<< BUILD_LOG( kSellItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< END_LOG;
			return false;
		}

		mapToDelete.insert( std::make_pair( kSellItemInfo.m_kInventoryItemInfo.m_iItemUID, kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity ) );
	}

	// 아이템 삭제 처리
	if( DeleteAndInsert( mapToDelete, mapToInsertDummy, vecInventorySlotInfo, vecNewItem, KDeletedItemInfo::DR_PSHOP_AGENCY ) == false )
	{
		START_LOG( cerr, L"대리 상인에 등록할 아이템을 삭제 실패하였음." )
			<< BUILD_LOG( m_pUser->GetName() )
			<< BUILD_LOG( m_pUser->GetCharName() )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellItemInfo, vecSellItemInfo )
	{
		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_REG_PSHOP, kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_REG_PSHOP, kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}	

	return true;
}
#endif SERV_PSHOP_AGENCY
//}}

	//{{ 2012. 07. 2	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
void KInventory::GetSuitableCategory( IN int iItemID, OUT std::vector<int>& vecCategoryID )
{
	vecCategoryID.clear();

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return;
	}

	//{{ 2010. 01. 05  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE
	if( pItemTemplet->m_bIsPcBang == true )
	{
		vecCategoryID.push_back( CXSLInventory::ST_PC_BANG );
		return;
	}
#endif SERV_PC_BANG_PRE
	//}}

	if( pItemTemplet->m_bFashion )
	{
		vecCategoryID.push_back( CXSLInventory::ST_AVARTA );
		return;
	}

	switch( pItemTemplet->m_ItemType )
	{
	case CXSLItem::IT_NONE:
		vecCategoryID.push_back( CXSLInventory::ST_NONE );
		return;

	case CXSLItem::IT_WEAPON:
	case CXSLItem::IT_DEFENCE:
		vecCategoryID.push_back( CXSLInventory::ST_EQUIP );
		return;

	case CXSLItem::IT_ACCESSORY:
		vecCategoryID.push_back( CXSLInventory::ST_ACCESSORY );
		return;

	case CXSLItem::IT_MATERIAL:
		vecCategoryID.push_back( CXSLInventory::ST_MATERIAL );
		return;

	case CXSLItem::IT_SPECIAL:
		vecCategoryID.push_back( CXSLInventory::ST_SPECIAL );
		return;

	case CXSLItem::IT_QUEST:
		vecCategoryID.push_back( CXSLInventory::ST_QUEST );
		return;

	case CXSLItem::IT_QICK_SLOT:
		vecCategoryID.push_back( CXSLInventory::ST_E_QUICK_SLOT );
		vecCategoryID.push_back( CXSLInventory::ST_QUICK_SLOT );
		return;

#ifdef SERV_SKILL_NOTE
	case CXSLItem::IT_SKILL_MEMO:
		vecCategoryID.push_back( CXSLInventory::ST_SPECIAL );
		return;
#endif
	default:
		return;
	}
}
#else
int KInventory::GetSuitableCategory( int iItemID )
{
    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
    if( !pItemTemplet )
    {
        START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return CXSLInventory::ST_NONE;
    }

	//{{ 2010. 01. 05  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE
	if( pItemTemplet->m_bIsPcBang )
	{
		return CXSLInventory::ST_PC_BANG;
	}
#endif SERV_PC_BANG_PRE
	//}}

    if( pItemTemplet->m_bFashion )
    {
        return CXSLInventory::ST_AVARTA;
    }

    switch( pItemTemplet->m_ItemType )
    {
    case CXSLItem::IT_NONE:
        return CXSLInventory::ST_NONE;
    case CXSLItem::IT_WEAPON:
    case CXSLItem::IT_DEFENCE:
        return CXSLInventory::ST_EQUIP;
    case CXSLItem::IT_ACCESSORY:
        return CXSLInventory::ST_ACCESSORY;
    case CXSLItem::IT_MATERIAL:
        return CXSLInventory::ST_MATERIAL;
    case CXSLItem::IT_SPECIAL:
        return CXSLInventory::ST_SPECIAL;
	case CXSLItem::IT_QUEST:
		return CXSLInventory::ST_QUEST;
    case CXSLItem::IT_QICK_SLOT:
        return CXSLInventory::ST_QUICK_SLOT;
#ifdef SERV_SKILL_NOTE
	case CXSLItem::IT_SKILL_MEMO:
		return CXSLInventory::ST_SPECIAL;
#endif
    case CXSLItem::IT_PET:
    case CXSLItem::IT_RIDING:
        return CXSLInventory::ST_SPECIAL;

    default:
        return CXSLInventory::ST_NONE;
    }
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}

bool KInventory::GetEmptyCategorySlotForItem( int iItemID, int& iCategory, int& iSlotID )
{
	//{{ 2012. 07. 3	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	std::vector<int> vecCategoryID;
	GetSuitableCategory( iItemID, vecCategoryID );

	for( size_t i=0; i < vecCategoryID.size(); ++i )
	{
		iCategory = vecCategoryID[i];
		for( iSlotID=0; iSlotID < GetSlotSize( iCategory ); ++iSlotID )
		{
			if( IsEmptySlot( iCategory, iSlotID ) )
			{
				return true;
			}
		}
	}
#else
    iCategory = GetSuitableCategory( iItemID );

    int i;
    for( i = 0; i < GetSlotSize( iCategory ); i++ )
    {
        if( IsEmptySlot( iCategory, i ) )
        {
            iSlotID = i;
            return true;
        }
    }
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

    return false;
}

void KInventory::GetNumFreeSlot( std::map< int, int >& mapNumFreeSlot )
{
	mapNumFreeSlot.clear();

	std::map< int, int >::iterator mit;
	int iCategory, iSlotID;

	//{{ 2012. 07. 9	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	for( iCategory = CXSLInventory::ST_NONE; iCategory < CXSLInventory::ST_END; ++iCategory )
	{
		if( IsValidCategory( iCategory ) == false )
			continue;
#else
	for( iCategory = CXSLInventory::ST_EQUIP; iCategory <= CXSLInventory::ST_AVARTA; ++iCategory )
	{
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}
		for( iSlotID = 0; iSlotID < GetSlotSize( iCategory ); ++iSlotID )
		{
			if( IsEmptySlot( iCategory, iSlotID ) ) // 빈 슬롯이라면
			{
				mit = mapNumFreeSlot.find( iCategory );
				if( mit != mapNumFreeSlot.end() )
				{
					mit->second += 1;
				}
				else
				{
					mapNumFreeSlot.insert( std::make_pair( iCategory, 1 ) );
				}
			}
		}
	}
}

void KInventory::GetInventoryItemInfoContainingThisItem( int iItemID, std::vector< KInventoryItemInfo >& vecInventoryItemInfo, bool bOnlyInventory )
{
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
	GetInventoryItemInfoContainingThisItem( iItemID, vecInventoryItemInfo, bOnlyInventory, false );
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
    vecInventoryItemInfo.clear();

    std::map< UidType, KInventoryItem >::iterator mit;
    for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
    {
        if( mit->second.GetItemID() == iItemID )
        {
			//{{ 2009. 8. 7  최육사		인벤토리 아이템만 체크
			if( bOnlyInventory )
			{
				if( CXSLInventory::IsUserInventoryCategory( mit->second.GetInventoryCategory() ) == false )
					continue;
			}
			//}}

            if( !VerifyCategorySlotID( mit->second.GetInventoryCategory(), mit->second.GetSlotID() ) )
            {
                START_LOG( cerr, L"카테고리 또는 슬롯 ID 이상." )
                    << BUILD_LOG( mit->second.GetInventoryCategory() )
                    << BUILD_LOG( mit->second.GetSlotID() )
                    << END_LOG;

                continue;
            }

            KInventoryItemInfo kInventoryItemInfo;
            mit->second.GetInventoryItemInfo( kInventoryItemInfo );
            vecInventoryItemInfo.push_back( kInventoryItemInfo );
        }
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
    if( !pItemTemplet )
    {
        START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
            << BUILD_LOG( iItemID )
            << END_LOG;

        return;
    }

    if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
    {
        int i, j;
        for( i = 1; i < ( int )vecInventoryItemInfo.size(); ++i )
        {
            for( j = 0; j < i; ++j )
            {
                if( vecInventoryItemInfo[j].m_kItemInfo.m_iQuantity > vecInventoryItemInfo[j + 1].m_kItemInfo.m_iQuantity )
                {
                    KInventoryItemInfo kInfo;
                    kInfo = vecInventoryItemInfo[j];
                    vecInventoryItemInfo[j] = vecInventoryItemInfo[j + 1];
                    vecInventoryItemInfo[j + 1] = kInfo;
                }
            }
        }
    }
	#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
}

#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
void KInventory::GetInventoryItemInfoContainingThisItem( IN const int iItemID
													   , OUT std::vector< KInventoryItemInfo >& vecInventoryItemInfo
													   , IN const bool bOnlyInventory
													   , IN const bool bOnlyInfinite	// 기한 제한이 없는 것
													   )
{
	vecInventoryItemInfo.clear();

	std::map< UidType, KInventoryItem >::iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		if( mit->second.GetItemID() == iItemID )
		{
			//{{ 2009. 8. 7  최육사		인벤토리 아이템만 체크
			if( bOnlyInventory )
			{
				if( CXSLInventory::IsUserInventoryCategory( mit->second.GetInventoryCategory() ) == false )
					continue;
			}
			//}}

			if( ( bOnlyInfinite == true ) && ( mit->second.IsPeriod() == true ) )
			{
				continue;
			}

			if( !VerifyCategorySlotID( mit->second.GetInventoryCategory(), mit->second.GetSlotID() ) )
			{
				START_LOG( cerr, L"카테고리 또는 슬롯 ID 이상." )
					<< BUILD_LOG( mit->second.GetInventoryCategory() )
					<< BUILD_LOG( mit->second.GetSlotID() )
					<< END_LOG;

				continue;
			}

			KInventoryItemInfo kInventoryItemInfo;
			mit->second.GetInventoryItemInfo( kInventoryItemInfo );
			vecInventoryItemInfo.push_back( kInventoryItemInfo );
		}
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( !pItemTemplet )
	{
		START_LOG( cerr, L"아이템 템플릿을 찾을 수 없음." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return;
	}

	if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
	{
		int i, j;
		for( i = 1; i < ( int )vecInventoryItemInfo.size(); ++i )
		{
			for( j = 0; j < i; ++j )
			{
				if( vecInventoryItemInfo[j].m_kItemInfo.m_iQuantity > vecInventoryItemInfo[j + 1].m_kItemInfo.m_iQuantity )
				{
					KInventoryItemInfo kInfo;
					kInfo = vecInventoryItemInfo[j];
					vecInventoryItemInfo[j] = vecInventoryItemInfo[j + 1];
					vecInventoryItemInfo[j + 1] = kInfo;
				}
			}
		}
	}
}
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX

//{{ 2010. 10. 27	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
void KInventory::GetItemUIDListThisItem( IN int iItemID, OUT std::vector< UidType >& vecItemUIDList, IN bool bOnlyInventory )
{
	vecItemUIDList.clear();

	std::map< UidType, KInventoryItem >::iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		if( mit->second.GetItemID() == iItemID )
		{
			//{{ 2009. 8. 7  최육사		인벤토리 아이템만 체크
			if( bOnlyInventory )
			{
				if( CXSLInventory::IsUserInventoryCategory( mit->second.GetInventoryCategory() ) == false )
					continue;
			}
			//}}

			if( !VerifyCategorySlotID( mit->second.GetInventoryCategory(), mit->second.GetSlotID() ) )
			{
				START_LOG( cerr, L"카테고리 또는 슬롯 ID 이상." )
					<< BUILD_LOG( mit->second.GetInventoryCategory() )
					<< BUILD_LOG( mit->second.GetSlotID() )
					<< END_LOG;

				continue;
			}

			vecItemUIDList.push_back( mit->first );
		}
	}
}
#endif SERV_QUEST_CLEAR_EXPAND
//}}

//{{ 2008. 2. 19  최육사  아이템 분해
bool KInventory::ResolveItem( IN UidType iItemUID, 
							  IN OUT bool& bJackpot,
							  OUT bool& bFashion, 
							  OUT int& iResolveQuantity,
							  OUT std::map< int, int >& mapInsertedItem, 
							  OUT std::vector< KInventoryItemInfo >& vecUpdated, 
							  OUT std::vector< KItemInfo >& vecNewItem,
							  IN bool bForceInsertItem /*= false*/ )
{
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();

	std::map< UidType, int >   mapResolveItem; // 분해시킬 아이템
	std::map< int, KItemInfo > mapResultItem;  // 결과템

	SET_ERROR( NET_OK );
		
	if( IsExist( iItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( m_pUser->GetName() )
			<< BUILD_LOG( m_pUser->GetCharName() )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RESOLVE_ITEM_00 );
		return false;
	}
	mapResolveItem.insert( std::make_pair( iItemUID, 1 ) );

	//{{ 2009. 8. 7  최육사		은행 카테고리검사
	if( IsExistInBank( iItemUID ) == true )
	{
        START_LOG( cerr, L"은행에 있는 아이템은 분해못하는데? 클라에서 체크 안했나?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_00 );
		return false;
	}
	//}}

	//{{ 2008. 12. 21  최육사	사용불가 검사
	if( IsBrokenItem( iItemUID ) )
	{
		START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_06 );
		return false;
	}
	//}}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템을 분해하려고 했습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	const int iItemID = GetItemID( iItemUID );

	// 템플릿 정보 얻기
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RESOLVE_ITEM_01 );
		return false;
	}

	if( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON  &&
		pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE  &&
		pItemTemplet->m_ItemType != CXSLItem::IT_ACCESSORY )
	{
		START_LOG( cerr, L"무기나 방어구, 악세사리가 아님." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RESOLVE_ITEM_03 );
		return false;
	}

	// 캐쉬템중 기간무제한일 경우 분해 가능	
	if( pItemTemplet->m_bFashion == true )
	{
		KInventoryItemInfo kInvenItemInfo;
		GetInventoryItemInfo( iItemUID, kInvenItemInfo );

		if( kInvenItemInfo.m_kItemInfo.m_sPeriod > 0 )
		{
			SET_ERROR( ERR_RESOLVE_ITEM_08 );
			return false;
		}

		//{{ 2011.2.27 지헌 : 캐쉬 아바타 분해시 추가 아이템 나오도록 하기
#ifdef SERV_CASHITEM_RESOLVE_ADD
		std::vector<CXSLCashItemManager::ResolveAddInfo> vecItem;
		if( SiCXSLCashItemManager()->GetCashItemResolveAddFromScript( iItemID, vecItem ) )
		{
			// 넘어온 결과 아이템들을 다 넣어주자.
			std::vector<CXSLCashItemManager::ResolveAddInfo>::iterator vit;
			for(vit = vecItem.begin(); vit != vecItem.end(); vit++)
			{
				int iItemID = (*vit).iItemID;
				int iQuantity = (*vit).iQuantity;
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet(iItemID);
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
						<< BUILD_LOG( iItemID )
						<< END_LOG;
					return false;
				}
				KItemInfo kInsertItemInfo;
				kInsertItemInfo.m_iItemID	 = iItemID;
				kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
				kInsertItemInfo.m_iQuantity	 = iQuantity;
				kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
				mapResultItem.insert( std::make_pair( iItemID, kInsertItemInfo ) );
			}
		}
#endif

		// 무속성의 엘의 속성석을 캐쉬템 가격의 공식에 따라 수량을 계산하여 준다.
		int iPrice = 0;
		if( !SiCXSLCashItemManager()->GetCashItemPriceFromScript( iItemID, iPrice ) )
		{
			START_LOG( cwarn, L"캐쉬 가격을 얻을 수 없어서 분해 실패했음." )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			SET_ERROR( ERR_RESOLVE_ITEM_01 );
			return false;
		}

		if( iPrice <= 0 )
		{
			START_LOG( cerr, L"이게 왜 0원이 나오지?" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iPrice )
				<< END_LOG;

			SET_ERROR( ERR_RESOLVE_ITEM_01 );
			return false;
		}

		iResolveQuantity = iPrice / 200; // 0나누기 조심~

		KItemInfo kInsertItemInfo;
		kInsertItemInfo.m_iItemID	 = CXSLAttribEnchantItem::ATI_UNKNOWN;
		kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
		kInsertItemInfo.m_iQuantity	 = iResolveQuantity;
		kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;

		mapResultItem.insert( std::make_pair( CXSLAttribEnchantItem::ATI_UNKNOWN, kInsertItemInfo ) );		

		// 패션 아이템인지 체크
		bFashion = true;
	}
	else
	{
		int iSellPrice = 0;
		//{{ 2011. 01. 20	최육사	아이템 분해 개편
#ifdef SERV_ITEM_RESOLVE_NEW
		// 아이템 수량 얻기
        int iQuantity = 0;
		if( GetQuantity( iItemUID, iQuantity ) == false )
		{
			START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_RESOLVE_ITEM_00 );
			return false;
		}

		if( GetSellPrice( iItemUID, iQuantity, iSellPrice ) == false )
		{
			START_LOG( cerr, L"해당 아이템의 되팔기 가격을 얻지 못했습니다! 분해 실패!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_RESOLVE_ITEM_00 );
			return false;
		}

		// 분해 작업
		if( SiCXSLResolveItemManager()->GetResultItem( (int)pItemTemplet->m_ItemGrade, pItemTemplet->m_UseLevel, (int)pItemTemplet->m_ItemType, iSellPrice, bJackpot, mapResultItem ) == false )
		{
			START_LOG( cerr, L"분해 결과 아이템 받아오기 실패.!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( pItemTemplet->m_ItemGrade )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< BUILD_LOG( pItemTemplet->m_ItemType )
				<< BUILD_LOG( iSellPrice )
				<< BUILD_LOG( mapResultItem.size() )
				<< END_LOG;

			return false;
		}
#else
		// 분해 작업
		if( SiCXSLResolveItemManager()->GetResultItem( (int)pItemTemplet->m_ItemGrade, pItemTemplet->m_UseLevel, (int)pItemTemplet->m_EqipPosition, pItemTemplet->m_Price, bJackpot, mapResultItem ) == false )
		{
			START_LOG( cerr, L"분해 결과 아이템 받아오기 실패.!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( pItemTemplet->m_EqipPosition )
				<< BUILD_LOG( pItemTemplet->m_Price )
				<< BUILD_LOG( mapResultItem.size() )
				<< END_LOG;

			return false;
		}
#endif SERV_ITEM_RESOLVE_NEW
		//}}
	}	
	
	//////////////////////////////////////////////////////////////////////////
	//{{ 2008. 3. 17  최육사  DeleteAndInsert()
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}
	//}}
	//////////////////////////////////////////////////////////////////////////

	// 생성된 아이템이 들어갈 자리가 있는지 조사한다.
	if( !IsEnoughSpaceExist( mapInsertedItem ) )
	{
		SET_ERROR( ERR_RESOLVE_ITEM_06 );
		return false;
	}	

	// 분해 결과 처리
	if( !DeleteAndInsert( mapResolveItem, mapResultItem, vecUpdated, vecNewItem, KDeletedItemInfo::DR_RESOLVE ) )
	{
		SET_ERROR( ERR_RESOLVE_ITEM_04 );
		return false;
	}

	// 분해 통계	
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( iItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_RESOLVE, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_RESOLVE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	// 분해 결과 통계
	std::map< int, int >::const_iterator mitResult;
	for( mitResult = mapInsertedItem.begin(); mitResult != mapInsertedItem.end(); ++mitResult )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mitResult->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_R_RESULT, mitResult->second );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_R_RESULT, mitResult->second );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	return true;
}

//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
// 강화 파괴 방지랑 겹친다. 따로 쓸 경우 알아서 분리합시다.
#ifdef SERV_ENCHANT_PLUS_ITEM
bool KInventory::EnchantItem( IN UidType iItemUID, 
							  IN bool bIsRareEnchantStone, 
							  IN bool bIsNewEnchant,
							  IN bool bIsSupportMaterial,
							  IN bool bIsDestroyGuard,
							  IN bool bIsEnchantPlus,
							  IN bool bDebug, 
							  OUT int& iEnchantResult, 
							  OUT int& iLevelAfterEnchant, 
							  OUT int& iED, 
							  OUT std::vector< KInventoryItemInfo >& vecUpdated )
#else
bool KInventory::EnchantItem( IN UidType iItemUID, 
							  IN bool bIsRareEnchantStone, 
							  IN bool bIsNewEnchant,
							  IN bool bIsSupportMaterial,
							  IN bool bDebug, 
							  OUT int& iEnchantResult, 
							  OUT int& iLevelAfterEnchant, 
							  OUT int& iED, 
							  OUT std::vector< KInventoryItemInfo >& vecUpdated )
#endif//SERV_ENCHANT_PLUS_ITEM
							 //}}
{
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	// 강화될 아이템 검사
	if( IsExist( iItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ENCHANT_ITEM_00 );
		return false;
	}

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는 아이템은 강화 못하는데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	//{{ 2009. 9. 9  최육사		밀봉
	if( IsSealedItem( iItemUID ) == true )
	{
		START_LOG( cerr, L"밀봉된 아이템은 강화할 수 없다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ENCHANT_ITEM_14 );
		return false;
	}
	//}}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템을 강화하려고 했습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	int iItemID = GetItemID( iItemUID );

	// 템플릿 정보 얻기
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_ENCHANT_ITEM_01 );
		return false;
	}

	if( pItemTemplet->m_bCanEnchant == false )
	{
		START_LOG( cerr, L"강화할 수 있는 아이템이 아님." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_ENCHANT_ITEM_03 );
		return false;
	}

	//{{ 2008. 12. 21  최육사	깨진 아이템이면 강화 불가!
	if( IsBrokenItem( iItemUID ) )
	{
		START_LOG( cerr, L"클라이언트에서 깨진템인지 검사했을텐데..?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ENCHANT_ITEM_12 );
		return false;
	}
	//}}

    int iEnchantLevel;
    if( !GetEnchantLevel( iItemUID, iEnchantLevel ) )
    {
        START_LOG( cerr, L"강화 레벨 얻기 실패." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iItemID )
            << END_LOG;

        SET_ERROR( ERR_ENCHANT_ITEM_03 );
        return false;
    }

#ifdef SERV_ENCHANT_LIMIT
	if( iEnchantLevel >= SiCXSLEnchantItemManager()->GetSupportMaterialEnchantLimitLevel() )
	{
		START_LOG( clog, L"강화레벨 제한 걸림!" )
			<< BUILD_LOG( SiCXSLEnchantItemManager()->GetSupportMaterialEnchantLimitLevel() )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		SET_ERROR( ERR_ENCHANT_ITEM_04 );
		return false;
	}
#endif SERV_ENCHANT_LIMIT

	if( bIsSupportMaterial )
	{
		//{{ 2011. 01. 24	최육사	플루오르 강화 레벨 제한
#ifdef SERV_SUPPORT_MATERIAL_LIMIT
		//{{ 2012. 01. 19	김민성	플루오르 스톤 강화 이벤트 실시간 적용
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
		if( SiKGameSysVal()->IsEnchantEvent() == true
#ifdef SERV_ENCHANT_EVENT_USING_DB
			|| SiKGameEventManager()->IsEnableCode( CEI_11_ENCHANT_EVENT ) == true
#endif SERV_ENCHANT_EVENT_USING_DB
			)
		{
			if( iEnchantLevel >= SiCXSLEnchantItemManager()->GetEventSupportMaterialEnchantLimitLevel() )
			{
				switch( SiCXSLEnchantItemManager()->GetEventSupportMaterialEnchantLimitLevel() )
				{
				case 10: // 10강				
					SET_ERROR( ERR_ENCHANT_ITEM_11 );
					return false;

				case 11: // 11강
					SET_ERROR( ERR_ENCHANT_ITEM_15 );
					return false;

				default:
					{
						START_LOG( cerr, L"등록되지 않은 이벤트 플루오르스톤 강화 제한 레벨값입니다!" )
							<< BUILD_LOG( SiCXSLEnchantItemManager()->GetEventSupportMaterialEnchantLimitLevel() )
							<< END_LOG;

						SET_ERROR( ERR_ENCHANT_ITEM_11 );
						return false;
					}
					break;
				}
			}
		}
		else
		{
			if( iEnchantLevel >= SiCXSLEnchantItemManager()->GetSupportMaterialEnchantLimitLevel() )
			{
				switch( SiCXSLEnchantItemManager()->GetSupportMaterialEnchantLimitLevel() )
				{
				case 10: // 10강				
					SET_ERROR( ERR_ENCHANT_ITEM_11 );
					return false;

				case 11: // 11강
					SET_ERROR( ERR_ENCHANT_ITEM_15 );
					return false;

				default:
					{
						START_LOG( cerr, L"등록되지 않은 플루오르스톤 강화 제한 레벨값입니다!" )
							<< BUILD_LOG( SiCXSLEnchantItemManager()->GetSupportMaterialEnchantLimitLevel() )
							<< END_LOG;

						SET_ERROR( ERR_ENCHANT_ITEM_11 );
						return false;
					}
					break;
				}
			}
		}
#else
		if( iEnchantLevel >= SiCXSLEnchantItemManager()->GetSupportMaterialEnchantLimitLevel() )
		{
			switch( SiCXSLEnchantItemManager()->GetSupportMaterialEnchantLimitLevel() )
			{
			case 10: // 10강				
				SET_ERROR( ERR_ENCHANT_ITEM_11 );
				return false;

			case 11: // 11강
				SET_ERROR( ERR_ENCHANT_ITEM_15 );
				return false;

			default:
				{
					START_LOG( cerr, L"등록되지 않은 플루오르스톤 강화 제한 레벨값입니다!" )
						<< BUILD_LOG( SiCXSLEnchantItemManager()->GetSupportMaterialEnchantLimitLevel() )
						<< END_LOG;

					SET_ERROR( ERR_ENCHANT_ITEM_11 );
					return false;
				}
				break;
			}
		}
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
		//}}
#else
		if( iEnchantLevel >= CXSLEnchantItemManager::SUPPORT_MATERIAL_LIMIT )
		{
			// 보조제 사용 불가!
			SET_ERROR( ERR_ENCHANT_ITEM_11 );
			return false;
		}
#endif SERV_SUPPORT_MATERIAL_LIMIT
		//}}
	}

    if( iEnchantLevel < 0 || iEnchantLevel >= CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
    {
		if( iEnchantLevel != CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
		{
			START_LOG( cerr, L"강화 레벨 이상." )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;
		}

        SET_ERROR( ERR_ENCHANT_ITEM_04 );
        return false;
    }

	// 비용 검사
	switch( pItemTemplet->m_ItemType )
	{
	case CXSLItem::IT_WEAPON:
		{
			if( m_pUser->GetED() < (int)( ( (float)pItemTemplet->m_Price * 0.1f ) * 0.5f ) )
			{
				SET_ERROR( ERR_ENCHANT_ITEM_07 );
				return false;
			}
			iED = (int)( ( (float)pItemTemplet->m_Price * 0.1f ) * 0.5f );
		}
		break;

	case CXSLItem::IT_DEFENCE:
		{
			if( m_pUser->GetED() < (int)( ( (float)pItemTemplet->m_Price * 0.1f ) * 0.1f ) )
			{
				SET_ERROR( ERR_ENCHANT_ITEM_07 );
				return false;
			}
			iED = (int)( ( (float)pItemTemplet->m_Price * 0.1f ) * 0.1f );
		}
		break;

	default:		
		START_LOG( cerr, L"무기나 방어구가 아님." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< END_LOG;

		SET_ERROR( ERR_ENCHANT_ITEM_02 );
		return false;
	}	

    // 강화석 검사
	int iEnchantStoneID = 0;

	//{{ 2008. 11. 16  최육사	강화 개편
	if( bIsNewEnchant )
	{
#ifdef SERV_EVENT_ENCHANT_STONE_MULTI
		std::vector< int > vecEventEnchantStoneID;
		std::vector< int >::iterator vit;
		std::vector< KInventoryItemInfo > vecEnchantItemInfo;
		vecEnchantItemInfo.clear();

		SiCXSLEnchantItemManager()->GetEventEnchantStoneItemID( pItemTemplet->m_UseLevel, pItemTemplet->m_ItemType , vecEventEnchantStoneID );

		for( vit = vecEventEnchantStoneID.begin(); vit != vecEventEnchantStoneID.end(); ++vit )
		{
			if( (*vit) > 0 )
			{
				GetInventoryItemInfoContainingThisItem( (*vit), vecEnchantItemInfo, true );
			}

			if( vecEnchantItemInfo.empty() == false )
			{
				iEnchantStoneID = (*vit);
				break;
			}
		}

		if( iEnchantStoneID == 0 )
		{
			iEnchantStoneID = SiCXSLEnchantItemManager()->GetEnchantStoneItemID( pItemTemplet->m_UseLevel, pItemTemplet->m_ItemType );
			if( iEnchantStoneID <= 0 )
			{
				START_LOG( cerr, L"일어날수 없는 에러, 강화 정보 스크립트가 잘못되었나?" )
					<< BUILD_LOG( pItemTemplet->m_UseLevel )
					<< BUILD_LOG( pItemTemplet->m_ItemType )
					<< END_LOG;

				SET_ERROR( ERR_ENCHANT_ITEM_06 );
				return false;
			}
		}
#else
		iEnchantStoneID = SiCXSLEnchantItemManager()->GetEnchantStoneItemID( pItemTemplet->m_UseLevel, pItemTemplet->m_ItemType );
		if( iEnchantStoneID <= 0 )
		{
			START_LOG( cerr, L"일어날수 없는 에러, 강화 정보 스크립트가 잘못되었나?" )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< BUILD_LOG( pItemTemplet->m_ItemType )
				<< END_LOG;

			SET_ERROR( ERR_ENCHANT_ITEM_06 );
			return false;
		}
#endif SERV_EVENT_ENCHANT_STONE_MULTI
	}
	else
	{
		switch( pItemTemplet->m_ItemType )
		{
		case CXSLItem::IT_WEAPON:
			{
#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
				BOOST_FOREACH( const int iWeaponEnchantStoneID, SiCXSLEnchantItemManager()->GetRareWeaponEnchantStoneID() )
				{
					std::vector< KInventoryItemInfo > vecEnchantTempItemInfo;
					GetInventoryItemInfoContainingThisItem( iWeaponEnchantStoneID, vecEnchantTempItemInfo, true );
					if( vecEnchantTempItemInfo.empty() == false )
					{
						iEnchantStoneID = iWeaponEnchantStoneID;
						break;
					}
				}

				if( iEnchantStoneID == 0 )
				{
					BOOST_FOREACH( const int iWeaponEnchantStoneID, SiCXSLEnchantItemManager()->GetWeaponEnchantStoneID() )
					{
						std::vector< KInventoryItemInfo > vecEnchantTempItemInfo;
						GetInventoryItemInfoContainingThisItem( iWeaponEnchantStoneID, vecEnchantTempItemInfo, true );
						if( vecEnchantTempItemInfo.empty() == false )
						{
							iEnchantStoneID = iWeaponEnchantStoneID;
							break;
						}
					}
				}
#else //SERV_MULTIPLE_BLESSED_ENCHANT_STONE

#ifdef SERV_BLESSED_ARIEL_ENCHANT_STONE_EVENT
				iEnchantStoneID = SiCXSLEnchantItemManager()->GetRareWeaponEnchantStoneID();		

				// 인벤토리에서 강화석 찾기
				std::vector< KInventoryItemInfo > vecEnchantTempItemInfo;
				GetInventoryItemInfoContainingThisItem( iEnchantStoneID, vecEnchantTempItemInfo, true );

				// 레어인체트 아이템이 없다면
				if( vecEnchantTempItemInfo.empty() )
				{
					iEnchantStoneID = _CONST_BLESSED_RURIEL_ENCHANT_STONE_EVENT::iRuriel_Enchant_Stone_Weapon_Item;

					std::vector< KInventoryItemInfo > vecEnchantTempItemInfoEvent;
					GetInventoryItemInfoContainingThisItem( iEnchantStoneID, vecEnchantTempItemInfoEvent, true );

					if( vecEnchantTempItemInfoEvent.empty() )
					{
						iEnchantStoneID = SiCXSLEnchantItemManager()->GetWeaponEnchantStoneID();
					}
				}

#else // SERV_BLESSED_ARIEL_ENCHANT_STONE_EVENT
				if( bIsRareEnchantStone )
				{
					iEnchantStoneID = SiCXSLEnchantItemManager()->GetRareWeaponEnchantStoneID();				
				}
				else
				{
					iEnchantStoneID = SiCXSLEnchantItemManager()->GetWeaponEnchantStoneID();
				}
#endif // SERV_BLESSED_ARIEL_ENCHANT_STONE_EVENT

#endif //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
			}
			break;

		case CXSLItem::IT_DEFENCE:
			{
#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
				BOOST_FOREACH( const int& iArmorEnchantStoneID, SiCXSLEnchantItemManager()->GetRareArmorEnchantStoneID() )
				{
					std::vector< KInventoryItemInfo > vecEnchantTempItemInfo;
					GetInventoryItemInfoContainingThisItem( iArmorEnchantStoneID, vecEnchantTempItemInfo, true );
					if( vecEnchantTempItemInfo.empty() == false )
					{
						iEnchantStoneID = iArmorEnchantStoneID;
						break;
					}
				}

				if( iEnchantStoneID == 0 )
				{
					BOOST_FOREACH( const int& iArmorEnchantStoneID, SiCXSLEnchantItemManager()->GetArmorEnchantStoneID() )
					{
						std::vector< KInventoryItemInfo > vecEnchantTempItemInfo;
						GetInventoryItemInfoContainingThisItem( iArmorEnchantStoneID, vecEnchantTempItemInfo, true );
						if( vecEnchantTempItemInfo.empty() == false )
						{
							iEnchantStoneID = iArmorEnchantStoneID;
							break;
						}
					}
				}
#else //SERV_MULTIPLE_BLESSED_ENCHANT_STONE

#ifdef SERV_BLESSED_ARIEL_ENCHANT_STONE_EVENT
				iEnchantStoneID = SiCXSLEnchantItemManager()->GetRareArmorEnchantStoneID();		

				// 인벤토리에서 강화석 찾기
				std::vector< KInventoryItemInfo > vecEnchantTempItemInfo;
				GetInventoryItemInfoContainingThisItem( iEnchantStoneID, vecEnchantTempItemInfo, true );

				if( vecEnchantTempItemInfo.empty() )
				{
					iEnchantStoneID = _CONST_BLESSED_RURIEL_ENCHANT_STONE_EVENT::iRuriel_Enchant_Stone_Defence_Item;

					std::vector< KInventoryItemInfo > vecEnchantTempItemInfoEvent;
					GetInventoryItemInfoContainingThisItem( iEnchantStoneID, vecEnchantTempItemInfoEvent, true );

					if( vecEnchantTempItemInfoEvent.empty() )
					{
						iEnchantStoneID = SiCXSLEnchantItemManager()->GetArmorEnchantStoneID();
					}
				}

#else // SERV_BLESSED_ARIEL_ENCHANT_STONE_EVENT

				if( bIsRareEnchantStone )
				{
					iEnchantStoneID = SiCXSLEnchantItemManager()->GetRareArmorEnchantStoneID();
				}
				else
				{
					iEnchantStoneID = SiCXSLEnchantItemManager()->GetArmorEnchantStoneID();
				}
#endif // SERV_BLESSED_ARIEL_ENCHANT_STONE_EVENT

#endif //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
			}
			break;

		default:		
			START_LOG( cerr, L"일어날수 없는 에러, 위에서 이미 검사를 했음." )
				<< BUILD_LOG( pItemTemplet->m_ItemType )
				<< END_LOG;

			SET_ERROR( ERR_ENCHANT_ITEM_02 );
			return false;
		}
	}
	//}}	

	// 인벤토리에서 강화석 찾기
	std::vector< KInventoryItemInfo > vecEnchantItemInfo;
	GetInventoryItemInfoContainingThisItem( iEnchantStoneID, vecEnchantItemInfo, true );

	UidType iEnchantStoneUID = 0;
	std::vector< KInventoryItemInfo >::const_iterator vitEStone;
	for( vitEStone = vecEnchantItemInfo.begin(); vitEStone != vecEnchantItemInfo.end(); ++vitEStone )
	{
		// 강화석 UID 한개만 얻음.
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		iEnchantStoneUID = GetItemUID( vitEStone->m_cSlotCategory, vitEStone->m_sSlotID );
#else
		iEnchantStoneUID = GetItemUID( vitEStone->m_cSlotCategory, vitEStone->m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}

		if( iEnchantStoneUID <= 0 )
		{
			START_LOG( cerr, L"인벤에서 강화석을 찾는중 이상한 아이템UID 발견." )				
				<< BUILD_LOG( iEnchantStoneID )
				<< BUILD_LOGc( vitEStone->m_cSlotCategory )
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				<< BUILD_LOGc( vitEStone->m_sSlotID )
#else
				<< BUILD_LOGc( vitEStone->m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				<< BUILD_LOG( iEnchantStoneUID )
				<< END_LOG;
			continue;
		}

		break;
	}

	if( iEnchantStoneUID <= 0 )
	{
	    START_LOG( cerr, L"인벤토리에 해당 강화석이 없다!" )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< BUILD_LOG( bIsRareEnchantStone )
	        << BUILD_LOG( iEnchantStoneID )
			<< BUILD_LOG( iEnchantStoneUID )
	        << END_LOG;

	    SET_ERROR( ERR_ENCHANT_ITEM_05 );
	    return false;
	}

    // 강화시킬 아이템의 슬롯 정보 얻기
    int iCategory;
    int iSlotID;
    _JIF( GetCategorySlot( iItemUID, iCategory, iSlotID ), return false );

    if( iCategory == CXSLInventory::ST_E_EQUIP )
    {
        START_LOG( cerr, L"장착 중인 아이템 강화 시도." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        SET_ERROR( ERR_ENCHANT_ITEM_08 );
        return false;
    }

#ifdef SERV_ENCHANT_PLUS_ITEM
	int iEnchantPlusItemID = 0;
	std::vector< KInventoryItemInfo > vecEnchantPlusInfo;
#endif SERV_ENCHANT_PLUS_ITEM

	// 강화 작업
	//{{ 2012. 01. 19	김민성	플루오르 스톤 강화 이벤트 실시간 적용
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	if( SiKGameSysVal()->IsEnchantEvent() == true )
	{
		if( SiCXSLEnchantItemManager()->GetEventEnchantResult( bIsRareEnchantStone, iEnchantLevel + 1, iEnchantResult ) == false )
		{
			START_LOG( cerr, L"이벤트 강화 처리 오류!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iEnchantStoneID )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;

			SET_ERROR( ERR_ENCHANT_ITEM_06 );
			return false;
		}
	}
	else
	{
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
		if(bIsEnchantPlus)
		{
			// 지헌 : 셀레네 있나없나 체크 하고, 강화 결과 돌린다.
			iEnchantPlusItemID = SiCXSLEnchantItemManager()->GetEnchantPlusItemID( pItemTemplet->m_UseLevel );
			if( iEnchantPlusItemID <= 0 )
			{
				START_LOG( cerr, L"일어날수 없는 에러, 강화 확률 증가 정보 스크립트가 잘못되었나?" )
					<< BUILD_LOG( pItemTemplet->m_UseLevel )
					<< END_LOG;
	
				// 지헌 : 넷 에러 추가 1 - 강화 확률 증가 아이템 사용 불가
				SET_ERROR( ERR_ENCHANT_ITEM_10 );
				return false;
			}

			// 인벤토리에서 강화 확률 증가 아이템 찾기
			GetInventoryItemInfoContainingThisItem( iEnchantPlusItemID, vecEnchantPlusInfo, true );

			if( vecEnchantPlusInfo.empty() )
			{
				//START_LOG( cerr, L"인벤토리에 강화 확률 증가 아이템 없다." )
				//	<< BUILD_LOG( iSupportMaterialItemID )
				//	<< END_LOG;
	
				// 지헌 : 넷 에러 추가 2
				SET_ERROR( ERR_ENCHANT_ITEM_09 );
				return false;
			}
		}

		if( SiCXSLEnchantItemManager()->GetEnchantResult( bIsRareEnchantStone, bIsEnchantPlus, iEnchantLevel + 1, iEnchantResult ) == false )
#else	
		if( SiCXSLEnchantItemManager()->GetEnchantResult( bIsRareEnchantStone, iEnchantLevel + 1, iEnchantResult ) == false )
#endif//SERV_ENCHANT_PLUS_ITEM
		//}}	
		{
			START_LOG( cerr, L"강화 처리 오류!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iEnchantStoneID )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;

			SET_ERROR( ERR_ENCHANT_ITEM_06 );
			return false;
		}
	}
#else
	if( SiCXSLEnchantItemManager()->GetEnchantResult( bIsRareEnchantStone, iEnchantLevel + 1, iEnchantResult ) == false )
	{
		START_LOG( cerr, L"강화 처리 오류!" )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iEnchantStoneID )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		SET_ERROR( ERR_ENCHANT_ITEM_06 );
		return false;
	}
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	//}}
	
	// 강화 100% 성공 치트
	if( bDebug )
	{
		iEnchantResult = NetError::ERR_ENCHANT_RESULT_00;

		START_LOG( cout, L"운영자가 100% 강화성공 치트를 사용하였습니다." )
			<< BUILD_LOG( iEnchantLevel );
	}

	//{{ 2008. 12. 14  최육사	강화 보조제
	int iSupportMaterialItemID = 0;
	if( bIsSupportMaterial )
	{
		//{{  2011.12.21     김민성    코보 플루오르 스톤 이벤트
#ifdef SERV_COBO_SUPPORT_MATERIAL_EVENT
		int iEventSupportMaterialItemID = 0;
		std::vector< KInventoryItemInfo > vecEventSupportMaterialInfo;

		// 이벤트 보조제 아이템 찾기(정상적인 item 이라면 인벤토리에 존재 유무를 확인하자.
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		std::vector< int > vecEventSupportMaterialItemID;
		std::vector< int >::iterator vit;
		SiCXSLEnchantItemManager()->GetEventEnchantSupportMaterialItemID( pItemTemplet->m_UseLevel , vecEventSupportMaterialItemID );

		for( vit = vecEventSupportMaterialItemID.begin(); vit != vecEventSupportMaterialItemID.end(); ++vit )
		{
			if( (*vit) > 0 )
			{
				GetInventoryItemInfoContainingThisItem( (*vit), vecEventSupportMaterialInfo, true );
			}

			// 이벤트 보조제가 인벤토리에 존재한다.
			if( vecEventSupportMaterialInfo.empty() == false )
			{
				iSupportMaterialItemID = (*vit);
				break;
			}
		}

		if( iSupportMaterialItemID == 0 )
		{
			iSupportMaterialItemID = SiCXSLEnchantItemManager()->GetEnchantSupportMaterialItemID( pItemTemplet->m_UseLevel );
			if( iSupportMaterialItemID <= 0 )
			{
				START_LOG( cerr, L"일어날수 없는 에러, 보조제 정보 스크립트가 잘못되었나?" )
					<< BUILD_LOG( pItemTemplet->m_UseLevel )
					<< END_LOG;

				SET_ERROR( ERR_ENCHANT_ITEM_10 );
				return false;
			}
		}
#else SERV_EVENT_SUPPORT_MATERIAL_MULTI
		iEventSupportMaterialItemID = SiCXSLEnchantItemManager()->GetEventEnchantSupportMaterialItemID( pItemTemplet->m_UseLevel );
		if( iEventSupportMaterialItemID > 0 )
		{
			GetInventoryItemInfoContainingThisItem( iEventSupportMaterialItemID, vecEventSupportMaterialInfo, true );
		}
		
		// 이벤트 보조제가 인벤토리에 존재한다.
		if( vecEventSupportMaterialInfo.empty() == false )
		{
			iSupportMaterialItemID = iEventSupportMaterialItemID;
		}
		else
		{
			iSupportMaterialItemID = SiCXSLEnchantItemManager()->GetEnchantSupportMaterialItemID( pItemTemplet->m_UseLevel );
			if( iSupportMaterialItemID <= 0 )
			{
				START_LOG( cerr, L"일어날수 없는 에러, 보조제 정보 스크립트가 잘못되었나?" )
					<< BUILD_LOG( pItemTemplet->m_UseLevel )
					<< END_LOG;

				SET_ERROR( ERR_ENCHANT_ITEM_10 );
				return false;
			}
		}
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
#else SERV_COBO_SUPPORT_MATERIAL_EVENT
		iSupportMaterialItemID = SiCXSLEnchantItemManager()->GetEnchantSupportMaterialItemID( pItemTemplet->m_UseLevel );
		if( iSupportMaterialItemID <= 0 )
		{
			START_LOG( cerr, L"일어날수 없는 에러, 보조제 정보 스크립트가 잘못되었나?" )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< END_LOG;

			SET_ERROR( ERR_ENCHANT_ITEM_10 );
			return false;
		}
#endif SERV_COBO_SUPPORT_MATERIAL_EVENT
		//}}

		// 인벤토리에서 보조제 찾기
		std::vector< KInventoryItemInfo > vecSupportMaterialInfo;
		GetInventoryItemInfoContainingThisItem( iSupportMaterialItemID, vecSupportMaterialInfo, true );

		if( vecSupportMaterialInfo.empty() )
		{
			//START_LOG( cerr, L"인벤토리에 보조제가 없다." )
			//	<< BUILD_LOG( iSupportMaterialItemID )
			//	<< END_LOG;

			SET_ERROR( ERR_ENCHANT_ITEM_09 );
			return false;
		}

		std::vector< KInventoryItemInfo >::const_iterator vitSMaterial = vecSupportMaterialInfo.begin();
		for( ; vitSMaterial != vecSupportMaterialInfo.end(); ++vitSMaterial )
		{
			// 보조제 UID 한개만 얻음.
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			UidType iSupportMaterialUID = GetItemUID( vitSMaterial->m_cSlotCategory, vitSMaterial->m_sSlotID );
#else
			UidType iSupportMaterialUID = GetItemUID( vitSMaterial->m_cSlotCategory, vitSMaterial->m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}

			if( iSupportMaterialUID <= 0 )
			{
				START_LOG( cerr, L"인벤에서 보조제를 찾는중 이상한 아이템UID 발견." )
					<< BUILD_LOG( iSupportMaterialUID )
					<< BUILD_LOGc( vitSMaterial->m_cSlotCategory )
					//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					<< BUILD_LOGc( vitSMaterial->m_sSlotID )
#else
					<< BUILD_LOGc( vitSMaterial->m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
					<< BUILD_LOG( iSupportMaterialUID )
					<< END_LOG;
				continue;
			}

			// 보조제 삭제
			int iSMDecreased = 0;
			DecreaseQuantity( iSupportMaterialUID, 1, iSMDecreased, KDeletedItemInfo::DR_ENCHANT_PLUOR );
			LIF( iSMDecreased == 1 );

			KInventoryItemInfo kSupportMaterialInfo;
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			GetInventorySlotInfo( vitSMaterial->m_cSlotCategory, vitSMaterial->m_sSlotID, kSupportMaterialInfo );
#else
			GetInventorySlotInfo( vitSMaterial->m_cSlotCategory, vitSMaterial->m_cSlotID, kSupportMaterialInfo );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
			vecUpdated.push_back( kSupportMaterialInfo );
			break;
		}

		// 보조제 적용
		switch( iEnchantResult )
		{
		case NetError::ERR_ENCHANT_RESULT_02: // 1단계 하락
		case NetError::ERR_ENCHANT_RESULT_03: // 강화 단계 초기화
			{
				iEnchantResult = NetError::ERR_ENCHANT_RESULT_01; // 변화 없음
			}
			break;
		}
	}
	//}}

	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	// 강화 끝나고 나면 아이템 삭제
	if( bIsEnchantPlus )
	{
		std::vector< KInventoryItemInfo >::const_iterator vitEnchantPlus = vecEnchantPlusInfo.begin();
		for( ; vitEnchantPlus != vecEnchantPlusInfo.end(); ++vitEnchantPlus )
		{
			// 셀레네 UID 한개만 얻음.
			UidType iEnchantPlusUID = GetItemUID( vitEnchantPlus->m_cSlotCategory, vitEnchantPlus->m_sSlotID );

			if( iEnchantPlusUID <= 0 )
			{
				START_LOG( cerr, "인벤에서 보조제를 찾는중 이상한 아이템UID 발견." )
					<< BUILD_LOG( iEnchantPlusItemID )
					<< BUILD_LOGc( vitEnchantPlus->m_cSlotCategory )
					<< BUILD_LOG( vitEnchantPlus->m_sSlotID )
					<< BUILD_LOG( iEnchantPlusUID )
					<< END_LOG;
				continue;
			}

			// 셀레네 삭제
			int iSMDecreased = 0;
			DecreaseQuantity( iEnchantPlusUID, 1, iSMDecreased, KDeletedItemInfo::DR_ENCHANT_PLUOR );
			LIF( iSMDecreased == 1 );

			KInventoryItemInfo kDestroyGuardInfo;
			GetInventorySlotInfo( vitEnchantPlus->m_cSlotCategory, vitEnchantPlus->m_sSlotID, kDestroyGuardInfo );
			vecUpdated.push_back( kDestroyGuardInfo );
			break;
		}
	}
#endif SERV_ENCHANT_PLUS_ITEM
	//}}

	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	int iDestroyGuardItemID = 0;
	if( bIsDestroyGuard )
	{
		iDestroyGuardItemID = SiCXSLEnchantItemManager()->GetDestroyGuardItemID( pItemTemplet->m_UseLevel );
		if( iDestroyGuardItemID <= 0 )
		{
			START_LOG( cerr, L"일어날수 없는 에러, 파괴 방지 정보 스크립트가 잘못되었나?" )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< END_LOG;

			// 지헌 : 넷 에러 3
			SET_ERROR( ERR_ENCHANT_ITEM_10 );
			return false;
		}

		// 인벤토리에서 파괴 방지 아이템 찾기
		std::vector< KInventoryItemInfo > vecDestroyGuardInfo;
		GetInventoryItemInfoContainingThisItem( iDestroyGuardItemID, vecDestroyGuardInfo, true );

		if( vecDestroyGuardInfo.empty() )
		{
			//START_LOG( cerr, L"인벤토리에 파괴 방지 아이템 없다." )
			//	<< BUILD_LOG( iSupportMaterialItemID )
			//	<< END_LOG;

			// 지헌 : 넷 에러 4
			SET_ERROR( ERR_ENCHANT_ITEM_09 );
			return false;
		}

		std::vector< KInventoryItemInfo >::const_iterator vitDestroyGuard = vecDestroyGuardInfo.begin();
		for( ; vitDestroyGuard != vecDestroyGuardInfo.end(); ++vitDestroyGuard )
		{
			// 보조제 UID 한개만 얻음.
			UidType iDestroyGuardUID = GetItemUID( vitDestroyGuard->m_cSlotCategory, vitDestroyGuard->m_sSlotID );

			if( iDestroyGuardUID <= 0 )
			{
				START_LOG( cerr, "인벤에서 보조제를 찾는중 이상한 아이템UID 발견." )
					<< BUILD_LOG( iDestroyGuardItemID )
					<< BUILD_LOGc( vitDestroyGuard->m_cSlotCategory )
					<< BUILD_LOG( vitDestroyGuard->m_sSlotID )
					<< BUILD_LOG( iDestroyGuardUID )
					<< END_LOG;
				continue;
			}

			// 보조제 삭제
			int iSMDecreased = 0;
			DecreaseQuantity( iDestroyGuardUID, 1, iSMDecreased, KDeletedItemInfo::DR_ENCHANT_PLUOR );
			LIF( iSMDecreased == 1 );

			KInventoryItemInfo kDestroyGuardInfo;
			GetInventorySlotInfo( vitDestroyGuard->m_cSlotCategory, vitDestroyGuard->m_sSlotID, kDestroyGuardInfo );
			vecUpdated.push_back( kDestroyGuardInfo );
			break;
		}

		// 보조제 적용
		switch( iEnchantResult )
		{
		case NetError::ERR_ENCHANT_RESULT_04: // 파괴
			{
				START_LOG( clog, "지헌로그 : 파괴 방지 아이템으로 막았다!!" )
					<< BUILD_LOG( iDestroyGuardItemID )
					<< END_LOG;
				iEnchantResult = NetError::ERR_ENCHANT_RESULT_01; // 변화 없음
			}
			break;
		}
	}
	//}}
#endif SERV_DESTROY_GUARD_ITEM
	//}}

	// 강화 결과 처리
    int iIncreased;
    int iDecreased;
    int iLevelChange;
    KInventoryItemInfo kInventoryItemInfo;
	switch( iEnchantResult )
	{
    case NetError::ERR_ENCHANT_RESULT_00: // 강화 성공
        {
            iLevelChange = 1;
            IncreaseEnchantLevel( iItemUID, iLevelChange, iIncreased );
            LIF( iIncreased == iLevelChange );
            GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
            vecUpdated.push_back( kInventoryItemInfo );
        }
        break;
    case NetError::ERR_ENCHANT_RESULT_01: // 변화 없음
        break;
    case NetError::ERR_ENCHANT_RESULT_02: // 1단계 하락
        {
            iLevelChange = 1;
            DecreaseEnchantLevel( iItemUID, iLevelChange, iDecreased );
            LIF( iDecreased == iLevelChange );
            GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
            vecUpdated.push_back( kInventoryItemInfo );
        }
        break;
    case NetError::ERR_ENCHANT_RESULT_03: // 강화 단계 초기화
        {
            iLevelChange = iEnchantLevel;
            DecreaseEnchantLevel( iItemUID, iLevelChange, iDecreased );
            LIF( iDecreased == iLevelChange );
            GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
            vecUpdated.push_back( kInventoryItemInfo );
        }
        break;
    case NetError::ERR_ENCHANT_RESULT_04: // 아이템 사용불가
        {
			iLevelChange = abs(iEnchantLevel) * 2; // 마이너스 값
			DecreaseEnchantLevel( iItemUID, iLevelChange, iDecreased );
			LIF( iDecreased == iLevelChange );
			GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
			vecUpdated.push_back( kInventoryItemInfo );
        }
        break;

	default:
		START_LOG( cerr, L"정의되지 않은 강화 결과값입니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iEnchantLevel )
			<< BUILD_LOG( iEnchantResult )
            << BUILD_LOG( NetError::GetErrStr( iEnchantResult ) )
			<< END_LOG;

        SET_ERROR( ERR_ENCHANT_ITEM_06 );
		return false;
	}

    if( !GetEnchantLevel( iItemUID, iLevelAfterEnchant ) )
    {
        START_LOG( cerr, L"강화 레벨 얻기 실패." )
            << BUILD_LOG( iItemUID )
            << BUILD_LOG( iItemID )
            << END_LOG;

        SET_ERROR( ERR_ENCHANT_ITEM_03 );
        return false;
    }

	// 강화석 삭제
	GetCategorySlot( iEnchantStoneUID, iCategory, iSlotID );
	DecreaseQuantity( iEnchantStoneUID, 1, iDecreased, KDeletedItemInfo::DR_ENCHANT_STONE );
    LIF( iDecreased == 1 );
    GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
    vecUpdated.push_back( kInventoryItemInfo );

	if( !bDebug )
	{
		// 강화 통계
		int iColumn = iEnchantLevel * 2;
		if( iEnchantResult != NetError::ERR_ENCHANT_RESULT_00 )
		{
			// 강화 실패면 column값 증가
			++iColumn;
		}

		if( iColumn >= KStatistics::eSIColEnchant_Lv_1_Succ && iColumn < KStatistics::eSIColEnchant_Max )
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ENCHANT, kKey, iColumn, 1 );
		}

		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( iEnchantStoneID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}

		//{{ 2009. 1. 30  최육사	플루오르 스톤 사용통계
		if( bIsSupportMaterial )
		{
			KStatisticsKey kKeyMI;
			kKeyMI.m_vecIntKey.push_back( iSupportMaterialItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyMI, KStatistics::SI_ITEM_USE, 1 );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyMI, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
		//}}
		//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
		if( bIsEnchantPlus )	
		{
			// 사용 통계
			KStatisticsKey kKeyMI;
			kKeyMI.m_vecIntKey.push_back( iEnchantPlusItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyMI, KStatistics::SI_ITEM_USE, 1 );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyMI, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
#endif SERV_ENCHANT_PLUS_ITEM
		//}}
		//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
		if( bIsDestroyGuard )	
		{
			// 사용 통계
			KStatisticsKey kKeyMI;
			kKeyMI.m_vecIntKey.push_back( iDestroyGuardItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyMI, KStatistics::SI_ITEM_USE, 1 );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyMI, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
#endif SERV_DESTROY_GUARD_ITEM
		//}}

		// ED 사용량 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MEnchant, iED );
	}

    SET_ERROR( NET_OK );
	return true;
}
//}}

//{{ 2008. 12. 21  최육사	강화 복구
bool KInventory::RestoreItem( IN UidType iItemUID, OUT int& iLevelAfterEnchant, OUT std::vector< KInventoryItemInfo >& vecUpdated )
{
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	// 1. 복구될 아이템 검사
	if( IsExist( iItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_00 );
		return false;
	}

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iItemUID ) == true )
	{
		START_LOG( cerr, L"은행에서는 복구기능 사용 못하는데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	int iItemID = GetItemID( iItemUID );

	// 2. 템플릿 정보 얻기
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_01 );
		return false;
	}

	// 3. 강화가능한 템인지 검사
	if( !pItemTemplet->m_bCanEnchant )
	{
		START_LOG( cerr, L"강화할 수 있는 아이템이 아닌데 복구를 하려고 한다?" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_02 );
		return false;
	}

	if( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON  &&
		pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE )
	{
		START_LOG( cerr, L"강화할 수 있는 아이템이 아닌데 복구를 하려고 한다?" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_02 );
		return false;
	}
	
	// 4. 사용불가 아이템인지 검사
	if( !IsBrokenItem( iItemUID ) )
	{
		START_LOG( cerr, L"사용불가 아이템만 복구 가능함." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_03 );
		return false;
	}

	// 5. 강화 레벨 얻기
	int iEnchantLevel;
	if( !GetEnchantLevel( iItemUID, iEnchantLevel ) )
	{
		START_LOG( cerr, L"강화 레벨 얻기 실패." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_02 );
		return false;
	}

	// 6. 강화레벨 범위 검사
	if( iEnchantLevel >= 0  ||  iEnchantLevel < (-CXSLEnchantItemManager::MAX_ENCHANT_LEVEL) )
	{		
		START_LOG( cerr, L"강화 레벨 이상." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_02 );
		return false;
	}

	// 7. 복구석 itemid얻기
	int iRestoreItemID = 0;
	UidType iRestoreItemUID = 0;	

	// 방어구일 경우
	if( pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
	{
		iRestoreItemID = SiCXSLEnchantItemManager()->GetRestoreItemID( pItemTemplet->m_UseLevel, CXSLItem::IT_DEFENCE );
		if( iRestoreItemID <= 0 )
		{
			START_LOG( cerr, L"일어날수 없는 에러, 복구석 정보 스크립트가 잘못되었나?" )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< BUILD_LOG( pItemTemplet->m_ItemType )
				<< END_LOG;

			SET_ERROR( ERR_RESTORE_ITEM_01 );
			return false;
		}

		// 8. 인벤토리에서 복구석 찾기
		std::vector< KInventoryItemInfo > vecRestoreItemInfo;
		GetInventoryItemInfoContainingThisItem( iRestoreItemID, vecRestoreItemInfo, true );
		
		std::vector< KInventoryItemInfo >::const_iterator vitRI;
		for( vitRI = vecRestoreItemInfo.begin(); vitRI != vecRestoreItemInfo.end(); ++vitRI )
		{
			// 복구석 UID 한개만 얻음.
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			iRestoreItemUID = GetItemUID( vitRI->m_cSlotCategory, vitRI->m_sSlotID );
#else
			iRestoreItemUID = GetItemUID( vitRI->m_cSlotCategory, vitRI->m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}

			if( iRestoreItemUID <= 0 )
			{
				START_LOG( cerr, L"인벤에서 강화석을 찾는중 이상한 아이템UID 발견." )
					<< BUILD_LOG( iRestoreItemID )
					<< BUILD_LOGc( vitRI->m_cSlotCategory )
					//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					<< BUILD_LOG( vitRI->m_sSlotID )
#else
					<< BUILD_LOGc( vitRI->m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
					<< BUILD_LOG( iRestoreItemUID )
					<< END_LOG;
				continue;
			}

			break;
		}
	}

	if( iRestoreItemUID <= 0 )
	{
		//{{  2011.12.21     김민성    코보 축복받은 복원의 주문서 이벤트
#ifdef SERV_COBO_RESTORE_SCROLL_EVENT
		int iEventRestoreItemID = 0;
		std::vector< KInventoryItemInfo > vecEventRestoreItemInfo;

		// 이벤트 복구석 아이템 찾기(정상적인 item 이라면 인벤토리에 존재 유무를 확인하자.
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		iRestoreItemID = 0;

		std::vector< int > vecEventRestoreItemID;
		std::vector< int >::iterator vit;
		SiCXSLEnchantItemManager()->GetEventRestoreItemID( pItemTemplet->m_UseLevel, CXSLItem::IT_NONE, vecEventRestoreItemID );

		for( vit = vecEventRestoreItemID.begin(); vit != vecEventRestoreItemID.end(); ++vit )
		{
			if( (*vit) > 0 )
			{
				GetInventoryItemInfoContainingThisItem( (*vit), vecEventRestoreItemInfo, true );
			}

			// 이벤트 보조제가 인벤토리에 존재한다.
			if( vecEventRestoreItemInfo.empty() == false )
			{
				iRestoreItemID = (*vit);
				break;
			}
		}

		if( iRestoreItemID == 0 )
		{
			iRestoreItemID = SiCXSLEnchantItemManager()->GetRestoreItemID( pItemTemplet->m_UseLevel, CXSLItem::IT_NONE );
			if( iRestoreItemID <= 0 )
			{
				START_LOG( cerr, L"일어날수 없는 에러, 복구석 정보 스크립트가 잘못되었나?" )
					<< BUILD_LOG( pItemTemplet->m_UseLevel )
					<< BUILD_LOG( pItemTemplet->m_ItemType )
					<< END_LOG;

				SET_ERROR( ERR_RESTORE_ITEM_01 );
				return false;
			}
		}
#else
		iEventRestoreItemID = SiCXSLEnchantItemManager()->GetEventRestoreItemID( pItemTemplet->m_UseLevel, CXSLItem::IT_NONE );
		if( iEventRestoreItemID > 0 )
		{
			GetInventoryItemInfoContainingThisItem( iEventRestoreItemID, vecEventRestoreItemInfo, true );
		}

		// 이벤트 복구석이 인벤토리에 존재한다.
		if( vecEventRestoreItemInfo.empty() == false )
		{
			iRestoreItemID = iEventRestoreItemID;
		}
		else
		{
			iRestoreItemID = SiCXSLEnchantItemManager()->GetRestoreItemID( pItemTemplet->m_UseLevel, CXSLItem::IT_NONE );
			if( iRestoreItemID <= 0 )
			{
				START_LOG( cerr, L"일어날수 없는 에러, 복구석 정보 스크립트가 잘못되었나?" )
					<< BUILD_LOG( pItemTemplet->m_UseLevel )
					<< BUILD_LOG( pItemTemplet->m_ItemType )
					<< END_LOG;

				SET_ERROR( ERR_RESTORE_ITEM_01 );
				return false;
			}
		}
#endif SERV_EVENT_RESTORE_SCROLL_MULTI 
#else
		iRestoreItemID = SiCXSLEnchantItemManager()->GetRestoreItemID( pItemTemplet->m_UseLevel, CXSLItem::IT_NONE );
		if( iRestoreItemID <= 0 )
		{
			START_LOG( cerr, L"일어날수 없는 에러, 복구석 정보 스크립트가 잘못되었나?" )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< BUILD_LOG( pItemTemplet->m_ItemType )
				<< END_LOG;

			SET_ERROR( ERR_RESTORE_ITEM_01 );
			return false;
		}
#endif SERV_COBO_RESTORE_SCROLL_EVENT
		//}}

		// 8. 인벤토리에서 복구석 찾기
		std::vector< KInventoryItemInfo > vecRestoreItemInfo;
		GetInventoryItemInfoContainingThisItem( iRestoreItemID, vecRestoreItemInfo, true );
		
		std::vector< KInventoryItemInfo >::const_iterator vitRI;
		for( vitRI = vecRestoreItemInfo.begin(); vitRI != vecRestoreItemInfo.end(); ++vitRI )
		{
			// 복구석 UID 한개만 얻음.
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			iRestoreItemUID = GetItemUID( vitRI->m_cSlotCategory, vitRI->m_sSlotID );
#else
			iRestoreItemUID = GetItemUID( vitRI->m_cSlotCategory, vitRI->m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}

			if( iRestoreItemUID <= 0 )
			{
				START_LOG( cerr, L"인벤에서 강화석을 찾는중 이상한 아이템UID 발견." )
					<< BUILD_LOG( iRestoreItemID )
					<< BUILD_LOGc( vitRI->m_cSlotCategory )
					//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					<< BUILD_LOG( vitRI->m_sSlotID )
#else
					<< BUILD_LOGc( vitRI->m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
					<< BUILD_LOG( iRestoreItemUID )
					<< END_LOG;
				continue;
			}

			break;
		}		
	}

	if( iRestoreItemUID <= 0 )
	{
		START_LOG( cerr, L"인벤토리에 해당 복구석이 없다!" )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< BUILD_LOG( iRestoreItemID )
			<< BUILD_LOG( iRestoreItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_04 );
		return false;
	}

	// 9. 복구시킬 아이템의 슬롯 정보 얻기
	int iCategory;
	int iSlotID;
	_JIF( GetCategorySlot( iItemUID, iCategory, iSlotID ), return false );

	if( iCategory == CXSLInventory::ST_E_EQUIP )
	{
		START_LOG( cerr, L"장착 중인 아이템 복구 시도." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_05 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// 10. 복구 작업
	KInventoryItemInfo kInventoryItemInfo;
	int iIncreased = 0;
	int iLevelChange = abs(iEnchantLevel) * 2; // 양수 값
	IncreaseEnchantLevel( iItemUID, iLevelChange, iIncreased );
	LIF( iIncreased == iLevelChange );
	GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
	vecUpdated.push_back( kInventoryItemInfo );
	
	// 11. 복구석 삭제
	int iDecreased = 0;
	GetCategorySlot( iRestoreItemUID, iCategory, iSlotID );
	DecreaseQuantity( iRestoreItemUID, 1, iDecreased, KDeletedItemInfo::DR_RESTORE );
	LIF( iDecreased == 1 );
	GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
	vecUpdated.push_back( kInventoryItemInfo );
	//////////////////////////////////////////////////////////////////////////

	if( !GetEnchantLevel( iItemUID, iLevelAfterEnchant ) )
	{
		START_LOG( cerr, L"강화 레벨 얻기 실패. 일어날 수 없는 에러." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_01 );
		return false;
	}

	// 복구 통계
	{
		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( iRestoreItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	SET_ERROR( NET_OK );
	return true;
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 04. 06  최육사	마법석 개편
#ifdef SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////

//{{ 2008. 3. 5  최육사  소켓 아이템
//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventory::SocketItem( IN UidType iItemUID
						   , IN const std::map< int, UidType >& mapSocketInfo
						   , IN bool bCheat
						   , OUT int& iTargetItemID
						   , OUT int& iED
						   , OUT std::vector< int >& vecSocketResult
						   , OUT std::vector< KInventoryItemInfo >& vecUpdated
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
						   , OUT byte& byteExpandedSocketNum
#endif // SERV_BATTLE_FIELD_BOSS
						   )
#else
bool KInventory::SocketItem( IN UidType iItemUID, 
							IN const std::map< int, UidType >& mapSocketInfo,
							IN bool bCheat,
							OUT int& iTargetItemID, 
							OUT int& iED, 
							OUT std::vector< short >& vecSocketResult,
							OUT std::vector< KInventoryItemInfo >& vecUpdated )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	iED = 0;
	vecSocketResult.clear();
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	//////////////////////////////////////////////////////////////////////////
	// 소켓 설치할 아이템에 대한 검사
	
	// 1. 소켓 넣을 아이템 검사
	if( IsExist( iItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_00 );
		return false;
	}

	// 2. 은행에 있는 아이템에는 소켓 못넣음
	if( IsExistInBank( iItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는 아이템은 소켓 못넣는데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_01 );
		return false;
	}

	// 3. 사용불가 아이템인지 검사
	if( IsBrokenItem( iItemUID ) )
	{
		START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_06 );
		return false;
	}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	// 4. PC방 전용 아이템인지 검사
	if( IsDBUpdateItem( iItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템에 소켓을 박을려고 했습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	const int iItemID = GetItemID( iItemUID );
	iTargetItemID = iItemID;

	// 5. 템플릿 정보 얻기
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_01 );
		return false;
	}

	//{{ 2009. 8. 10  최육사	장착불가 아이템은 소켓설치 못함
	// 6. 장착 불가 아이템인지 검사
	if( pItemTemplet->m_bNoEquip )
	{
		START_LOG( clog, L"장착 불가 아이템은 소켓설치 못함." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_06 );
		return false;
	}
	//}}

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 06. 11  최육사	소켓 옵션 치트
	if( bCheat == true )
	{
		std::map< int, UidType >::const_iterator mitCheat;
		for( mitCheat = mapSocketInfo.begin(); mitCheat != mapSocketInfo.end(); ++mitCheat )
		{
			// 해당 소켓에 소켓옵션 넣기
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			if( SetItemSocketOption( iItemUID, mitCheat->first, static_cast< int >(mitCheat->second) ) == false )
#else
			if( SetItemSocketOption( iItemUID, mitCheat->first, static_cast< short >(mitCheat->second) ) == false )
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			{
				START_LOG( cerr, L"소켓 옵션 넣기 실패!" )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( mitCheat->first )
					<< BUILD_LOG( mitCheat->second )
					<< END_LOG;

				SET_ERROR( ERR_SOCKET_ITEM_06 );
				return false;
			}
		}

		// DB에 업데이트 하기 위한 소켓 옵션 리스트 얻기
		if( GetItemSocketList( iItemUID, vecSocketResult ) == false )
		{
			START_LOG( cerr, L"위에서 아이템의 존재 여부를 검사했다. 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_SOCKET_ITEM_06 );
			return false;
		}

		// 해당 아이템 인벤 정보 얻기
		KInventoryItemInfo kInventoryItemInfo;
		GetInventoryItemInfo( iItemUID, kInventoryItemInfo );
		vecUpdated.push_back( kInventoryItemInfo );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
		byteExpandedSocketNum = kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS

        return true;
	}
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 수수료 계산
	int iTotalCost = 0;

	// 7. 현재 박혀있는 소켓 갯수 얻기
	int iAssignedSocketCount = 0;
	if( GetCountAssignedItemSocket( iItemUID, iAssignedSocketCount ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데? 아이템 소켓 정보 얻기 실패!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_00 );
		return false;
	}	

	// 수량 확인할 컨테이너	
	std::map< UidType, int > mapRequireItemQuantity;

	// 8. 수수료 계산 한번 해봅시다!
	std::map< int, UidType >::const_iterator mitSI;
	for( mitSI = mapSocketInfo.begin(); mitSI != mapSocketInfo.end(); ++mitSI )
	{
		const int iSocketSlotID = mitSI->first;
		const UidType iMagicStoneUID = mitSI->second;

		// 8-1. 존재하는 아이템인지 확인
		std::map< UidType, int >::iterator mitCQ;
		mitCQ = mapRequireItemQuantity.find( iMagicStoneUID );
		if( mitCQ == mapRequireItemQuantity.end() )
		{
			mapRequireItemQuantity.insert( std::make_pair( iMagicStoneUID, 1 ) );
		}
		else
		{
            ++mitCQ->second;
		}

		// 8-2. 마법석이 맞는지 검사
		const int iMagicStoneItemID = GetItemID( iMagicStoneUID );
		if( SiCXSLSocketItem()->IsMagicStoneItemID( iMagicStoneItemID ) == false )
		{
			START_LOG( cerr, L"마법석이 아니다!." )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iMagicStoneUID )
				<< BUILD_LOG( iMagicStoneItemID )
				<< END_LOG;

			SET_ERROR( ERR_SOCKET_ITEM_03 );
			return false;
		}

		// 8-3. 해당 슬롯에 소켓이 박혀있는지 확인
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		int iSocketOptionID = 0;
		if( GetItemSocketOption( iItemUID, iSocketSlotID, iSocketOptionID ) == false )
#else
		short sSocketOptionID = 0;
		if( GetItemSocketOption( iItemUID, iSocketSlotID, sSocketOptionID ) == false )
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		{
			START_LOG( cerr, L"인벤토리에 없는 아이템UID인데? 아이템 소켓 정보 얻기 실패!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_SOCKET_ITEM_00 );
			return false;
		}

		// 8-4. 비어있는 소켓 슬롯인 경우
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		const bool bIsEmptySlot = ( iSocketOptionID == 0 );
#else
		const bool bIsEmptySlot = ( sSocketOptionID == 0 );
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 

		// 박는 비용
		int iCost = 0;
		if( SiCXSLSocketItem()->CalcInsertSocketCost( iItemID, iMagicStoneItemID, iAssignedSocketCount, bIsEmptySlot, iCost ) == false )
		{
			START_LOG( cerr, L"소켓 수수료 얻기 실패!" )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iMagicStoneItemID )
				<< BUILD_LOG( iAssignedSocketCount )
				<< BUILD_LOG( bIsEmptySlot )
				<< END_LOG;

			SET_ERROR( ERR_SOCKET_ITEM_03 );
			return false;
		}

		// 비용 합산!
		iTotalCost += iCost;
	}

	// 9. 비용 검사
	if( m_pUser->GetED() < iTotalCost )
	{
		SET_ERROR( ERR_SOCKET_ITEM_02 );
		return false;
	}
	else
	{
		iED = iTotalCost;
	}

	//////////////////////////////////////////////////////////////////////////
	// 마법석 검사

	// 10. 마법석 수량 검사
	if( IsEnoughItemExist( mapRequireItemQuantity ) == false )
	{
		START_LOG( cerr, L"마법석 수량이 부족합니다." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( mapRequireItemQuantity.size() )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_03 );
		return false;
	}	

	// 11. 마법석 설치!
	std::map< int, UidType >::const_iterator mitMC;
	for( mitMC = mapSocketInfo.begin(); mitMC != mapSocketInfo.end(); ++mitMC )
	{
        // 11-1. 마법석 ItemID얻기
		const int iSocketIndex = mitMC->first;
		const int iMagicStoneItemID = GetItemID( mitMC->second );

		// 11-2. 강화레벨 얻기
		int iEnchantLevel = 0;
		GetEnchantLevel( iItemUID, iEnchantLevel );

		// 11-3. 소켓 랜덤 결과
		//{{ 2012. 09. 26  김민성  대전 장비 소켓 고정 강화 레벨
#ifdef SERV_2012_PVP_SEASON2_1
		int iSocketResult = 0;
		if( SiCXSLSocketItem()->GetResultItem( iMagicStoneItemID, pItemTemplet->m_ItemType, pItemTemplet->m_PeriodType, pItemTemplet->m_UseLevel, pItemTemplet->m_bFashion, iEnchantLevel, iSocketResult ) == false )
#else
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
//		int iSocketResult = 0;
//		if( SiCXSLSocketItem()->GetResultItem( iMagicStoneItemID, pItemTemplet->m_ItemType, pItemTemplet->m_PeriodType, pItemTemplet->m_bFashion, iEnchantLevel, iSocketResult ) == false )
//#else
//		short sSocketResult = 0;
//		if( SiCXSLSocketItem()->GetResultItem( iMagicStoneItemID, pItemTemplet->m_ItemType, pItemTemplet->m_PeriodType, pItemTemplet->m_bFashion, iEnchantLevel, sSocketResult ) == false )
//#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
#endif SERV_2012_PVP_SEASON2_1
		//}}
		{
			START_LOG( cerr, L"소켓 랜덤 결과 얻기 실패." )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iMagicStoneItemID )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;

			return false;
		}

		// 11-4. 해당 소켓에 소켓옵션 넣기
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		if( SetItemSocketOption( iItemUID, iSocketIndex, iSocketResult ) == false )
		{
			START_LOG( cerr, L"소켓 옵션 넣기 실패!" )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iSocketIndex )
				<< BUILD_LOG( iSocketResult )
				<< END_LOG;

			SET_ERROR( ERR_SOCKET_ITEM_06 );
			return false;
		}
#else
		if( SetItemSocketOption( iItemUID, iSocketIndex, sSocketResult ) == false )
		{
			START_LOG( cerr, L"소켓 옵션 넣기 실패!" )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iSocketIndex )
				<< BUILD_LOG( sSocketResult )
				<< END_LOG;

			SET_ERROR( ERR_SOCKET_ITEM_06 );
			return false;
		}
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 

		// 11-6. 통계
		KStatisticsKey kKeyMS;
		kKeyMS.m_vecIntKey.push_back( iMagicStoneItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyMS, KStatistics::SI_ITEM_USE, 1 );
	}

	// 11. 해당 아이템 소켓 리스트 얻기
	if( GetItemSocketList( iItemUID, vecSocketResult ) == false )
	{
        START_LOG( cerr, L"위에서 아이템의 존재 여부를 검사했다. 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_06 );
		return false;
	}

	// 12. 해당 아이템 인벤 정보 얻기
	KInventoryItemInfo kInventoryItemInfo;
	GetInventoryItemInfo( iItemUID, kInventoryItemInfo );
	vecUpdated.push_back( kInventoryItemInfo );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	byteExpandedSocketNum = kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS

	//////////////////////////////////////////////////////////////////////////	
	// 13. 마법석 삭제
	std::map< UidType, int >::const_iterator mitMD;
	for( mitMD = mapRequireItemQuantity.begin(); mitMD != mapRequireItemQuantity.end(); ++mitMD )
	{
		// 해당 마법석 삭제
		int iCategory, iSlotID;
		int iDecreased;
		GetCategorySlot( mitMD->first, iCategory, iSlotID );
		DecreaseQuantity( mitMD->first, mitMD->second, iDecreased, KDeletedItemInfo::DR_SOCKET_MAGIC_STONE );
		LIF( iDecreased == mitMD->second );
		GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
		vecUpdated.push_back( kInventoryItemInfo );
	}
	//////////////////////////////////////////////////////////////////////////	

	// 14. ED 사용량 통계
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MSocket, iED );

	SET_ERROR( NET_OK );
	return true;
}
//}}

//////////////////////////////////////////////////////////////////////////
#else
#pragma ELSE_MESSAGE
//////////////////////////////////////////////////////////////////////////

//{{ 2008. 3. 5  최육사  소켓 아이템
//bool KInventory::SocketItem( IN UidType iItemUID, 
//							IN const int iSocketIndex, 
//							IN bool bIsRareMagicStone, 
//							IN bool bIsRemoveSocketOption, 
//							IN bool bDebug, 
//							IN short sCheatSocketOption, 
//							OUT int& iSocketResult, 
//							OUT int& iED, 
//							OUT std::vector< short >& vecSocketResult, 
//							OUT std::vector< KInventoryItemInfo >& vecUpdated )
//{
//	vecSocketResult.clear();
//	vecUpdated.clear();
//
//	SET_ERROR( ERR_UNKNOWN );
//
//	// 소켓 넣을 아이템 검사
//	if( IsExist( iItemUID ) == false )
//	{
//		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
//			<< BUILD_LOG( iItemUID )
//			<< END_LOG;
//
//		SET_ERROR( ERR_SOCKET_ITEM_00 );
//		return false;
//	}
//
//	//{{ 2009. 8. 7  최육사		은행
//	if( IsExistInBank( iItemUID ) == true )
//	{
//		START_LOG( cerr, L"은행에 있는 아이템은 소켓 못넣는데?" )
//			<< BUILD_LOG( iItemUID )
//			<< END_LOG;
//
//		SET_ERROR( ERR_BANK_01 );
//		return false;
//	}
//	//}}
//
//	//{{ 2008. 12. 21  최육사	사용불가 검사
//	if( IsBrokenItem( iItemUID ) )
//	{
//		START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
//			<< BUILD_LOG( iItemUID )
//			<< END_LOG;
//
//		SET_ERROR( ERR_RESTORE_ITEM_06 );
//		return false;
//	}
//	//}}
//
//	//{{ 2010. 01. 04  최육사	PC방 프리미엄
//#ifdef SERV_PC_BANG_PRE
//
//	if( IsDBUpdateItem( iItemUID ) == false )
//	{
//		START_LOG( cerr, L"PC방 프리미엄 아이템에 소켓을 박을려고 했습니다." )
//			<< BUILD_LOG( iItemUID )
//			<< END_LOG;
//
//		SET_ERROR( ERR_PC_BANG_00 );
//		return false;
//	}
//
//#endif SERV_PC_BANG_PRE
//	//}}
//
//	int iItemID = GetItemID( iItemUID );
//
//	// 템플릿 정보 얻기
//	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
//	if( pItemTemplet == NULL )
//	{
//		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
//			<< BUILD_LOG( iItemID )
//			<< END_LOG;
//
//		SET_ERROR( ERR_SOCKET_ITEM_01 );
//		return false;
//	}
//
//	//{{ 2009. 8. 10  최육사	장착불가 아이템은 소켓설치 못함
//	if( pItemTemplet->m_bNoEquip )
//	{
//		START_LOG( clog, L"장착 불가 아이템은 소켓설치 못함." )
//			<< BUILD_LOG( iItemID )
//			<< END_LOG;
//
//		SET_ERROR( ERR_SOCKET_ITEM_06 );
//		return false;
//	}
//	//}}
//
//	// 수수료 계산
//	int iAssignedSocketCount = 0;
//	if( !GetCountAssignedItemSocket( iItemUID, iAssignedSocketCount ) )
//	{
//		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데? 아이템 소켓 정보 얻기 실패!" )
//			<< BUILD_LOG( iItemUID )
//			<< END_LOG;
//
//		SET_ERROR( ERR_SOCKET_ITEM_00 );
//		return false;
//	}
//
//	// 소켓옵션이 하나도 없을경우
//	if( iAssignedSocketCount == 0 )
//		iAssignedSocketCount = 1;
//
//	int iCost = 0;
//	switch( pItemTemplet->m_ItemType )
//	{
//	case CXSLItem::IT_WEAPON:
//		{			
//			if( !bIsRemoveSocketOption )
//			{
//				// 박을때
//				iCost = (int)( ( (float)pItemTemplet->m_Price * 0.1f ) * 0.1f ) * iAssignedSocketCount;
//			}
//			else
//			{
//				// 제거할때
//				iCost = (int)( ( (float)pItemTemplet->m_Price * 0.1f ) * 0.2f ) * iAssignedSocketCount;
//			}			
//		}
//		break;
//
//	case CXSLItem::IT_DEFENCE:
//		{			
//			if( !bIsRemoveSocketOption )
//			{
//				// 박을때
//				iCost = (int)( ( (float)pItemTemplet->m_Price * 0.1f ) * 0.05f ) * iAssignedSocketCount;
//			}
//			else
//			{
//				// 제거할때
//				iCost = (int)( ( (float)pItemTemplet->m_Price * 0.1f ) * 0.1f ) * iAssignedSocketCount;
//			}
//		}
//		break;
//
//	default:		
//		START_LOG( cerr, L"무기나 방어구가 아님." )
//			<< BUILD_LOG( iItemUID )
//			<< BUILD_LOG( iItemID )
//			<< BUILD_LOG( pItemTemplet->m_ItemType )
//			<< END_LOG;
//
//		SET_ERROR( ERR_SOCKET_ITEM_03 );
//		return false;
//	}
//
//	// 패션템일 경우 다른 수수료 공식 적용
//	if( pItemTemplet->m_bFashion )
//	{
//		if( !bIsRemoveSocketOption )
//		{
//			// 박을때
//			iCost = 1000;
//		}
//		else
//		{
//			// 제거할때
//			iCost = 2000;
//		}		
//	}	
//
//	// 운영자 치트시 비용없음
//	if( bDebug )
//	{
//		iCost = 0;
//	}
//
//	// 비용 검사
//	if( m_pUser->GetED() < iCost )
//	{
//		SET_ERROR( ERR_SOCKET_ITEM_02 );
//		return false;
//	}
//	else
//	{
//		iED = iCost;
//	}
//
//	//////////////////////////////////////////////////////////////////////////	
//	// 마법석 검사
//	int iMagicStoneID = 0;
//	UidType iMagicStoneUID = 0;
//	if( !bIsRemoveSocketOption && !bDebug )
//	{
//		iMagicStoneID = SiCXSLSocketItem()->GetMagicStoneItemID( bIsRareMagicStone );
//		if( iMagicStoneID == 0 )
//		{
//			START_LOG( cerr, L"해당되는 마법석이 없습니다." )
//				<< BUILD_LOG( iItemUID )
//				<< BUILD_LOG( iItemID )
//				<< BUILD_LOG( pItemTemplet->m_ItemType )
//				<< BUILD_LOG( bIsRareMagicStone )
//				<< END_LOG;
//
//			SET_ERROR( ERR_SOCKET_ITEM_04 );
//			return false;
//		}
//
//		// 인벤토리에서 마법석 찾기
//		std::vector< KInventoryItemInfo > vecMagicStoneItemInfo;
//		GetInventoryItemInfoContainingThisItem( iMagicStoneID, vecMagicStoneItemInfo, true );
//
//		std::vector< KInventoryItemInfo >::const_iterator vitMStone;
//		for( vitMStone = vecMagicStoneItemInfo.begin(); vitMStone != vecMagicStoneItemInfo.end(); ++vitMStone )
//		{
//			// 마법석 UID 한개만 얻음.
//			iMagicStoneUID = GetItemUID( vitMStone->m_cSlotCategory, vitMStone->m_cSlotID );
//
//			if( iMagicStoneUID <= 0 )
//			{
//				START_LOG( cerr, L"인벤에서 마법석을 찾는중 이상한 아이템UID 발견." )				
//					<< BUILD_LOG( iMagicStoneID )
//					<< BUILD_LOGc( vitMStone->m_cSlotCategory )
//					<< BUILD_LOGc( vitMStone->m_cSlotID )
//					<< BUILD_LOG( iMagicStoneUID )
//					<< END_LOG;
//				continue;
//			}
//
//			break;
//		}
//
//		if( iMagicStoneUID <= 0 )
//		{
//			START_LOG( cerr, L"인벤토리에 해당 마법석이 없다!" )
//				<< BUILD_LOG( pItemTemplet->m_ItemType )
//				<< BUILD_LOG( bIsRareMagicStone )
//				<< BUILD_LOG( iMagicStoneID )
//				<< BUILD_LOG( iMagicStoneUID )
//				<< END_LOG;
//
//			SET_ERROR( ERR_SOCKET_ITEM_05 );
//			return false;
//		}
//	}
//	//////////////////////////////////////////////////////////////////////////	
//
//	// 해당 소켓 인덱스에 이미 소켓 옵션이 들어있는지 검사
//	short sSocketOption;
//	if( !GetItemSocketOption( iItemUID, iSocketIndex, sSocketOption ) )
//	{
//		START_LOG( cerr, L"해당 아이템의 소켓옵션정보를 찾지 못하였습니다." )
//			<< BUILD_LOG( iItemUID )
//			<< BUILD_LOG( iItemID )
//			<< BUILD_LOG( iSocketIndex )
//			<< BUILD_LOG( sSocketOption )
//			<< END_LOG;
//
//		SET_ERROR( ERR_SOCKET_ITEM_06 );
//		return false;
//	}
//
//	//////////////////////////////////////////////////////////////////////////	
//	short sSocketResult;
//	if( !bDebug )
//	{
//		if( !bIsRemoveSocketOption )
//		{
//			// 소켓에 마법석 꽂기
//			if( sSocketOption != 0 )
//			{
//				START_LOG( cerr, L"이미 소켓옵션이 설정된 소켓인덱스 입니다. 일어나서는 안되는 에러." )
//					<< BUILD_LOG( iItemUID )
//					<< BUILD_LOG( iItemID )
//					<< BUILD_LOG( iSocketIndex )
//					<< BUILD_LOG( sSocketOption )
//					<< END_LOG;
//
//				SET_ERROR( ERR_SOCKET_ITEM_07 );
//				return false;
//			}
//
//			// 강화레벨 얻기
//			int iEnchantLevel = 0;
//			GetEnchantLevel( iItemUID, iEnchantLevel );
//
//			// 소켓 랜덤 결과
//			if( !SiCXSLSocketItem()->GetResultItem( iMagicStoneID, pItemTemplet->m_ItemType, pItemTemplet->m_PeriodType, pItemTemplet->m_bFashion, iEnchantLevel, sSocketResult ) )
//			{
//				START_LOG( cerr, L"소켓 랜덤 결과 얻기 실패." )
//					<< BUILD_LOG( iItemUID )
//					<< END_LOG;
//				return false;
//			}
//
//			// 클라이언트에 결과를 알려주기위해..
//			iSocketResult = static_cast<int>(sSocketResult);
//		}
//		else
//		{
//			// 소켓에 마법석 제거하기
//			if( sSocketOption == 0 )
//			{
//				START_LOG( cerr, L"이미 소켓옵션이 제거된 소켓인덱스 입니다." )
//					<< BUILD_LOG( iItemUID )
//					<< BUILD_LOG( iItemID )
//					<< BUILD_LOG( iSocketIndex )
//					<< BUILD_LOG( sSocketOption )
//					<< END_LOG;
//
//				SET_ERROR( ERR_SOCKET_ITEM_08 );
//				return false;
//			}
//
//			// 마법석 제거를 위해 0값을 넣는다.
//			sSocketResult = 0;
//		}
//	}
//	else
//	{
//		// 운영자 치트시 설정된 소켓옵션을 강제로 넣기
//		const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( sCheatSocketOption );
//		if( pSocketData == NULL )
//		{
//			START_LOG( cwarn, L"운영자 치트로 강제로 소켓옵션 넣기를 실행하였으나 존재하지 않는 옵션을 넣으려고 하였음." )				
//				<< BUILD_LOG( iItemUID )
//				<< BUILD_LOG( iItemID )
//				<< BUILD_LOG( iSocketIndex )
//				<< BUILD_LOG( bDebug )
//				<< BUILD_LOG( sCheatSocketOption )
//				<< END_LOG;
//
//			SET_ERROR( ERR_SOCKET_ITEM_09 );
//			return false;
//		}
//
//		sSocketResult = sCheatSocketOption;
//	}
//	//////////////////////////////////////////////////////////////////////////	
//
//	// 해당 소켓에 소켓옵션 넣기
//	if( !SetItemSocketOption( iItemUID, iSocketIndex, sSocketResult ) )
//	{
//		START_LOG( cerr, L"소켓 옵션 넣기 실패!" )
//			<< BUILD_LOG( iItemUID )
//			<< BUILD_LOG( iItemID )
//			<< BUILD_LOG( iSocketIndex )
//			<< BUILD_LOG( sSocketResult )
//			<< END_LOG;
//
//		SET_ERROR( ERR_SOCKET_ITEM_06 );
//		return false;
//	}
//
//	// 해당 아이템의 소켓 정보 얻기
//	KInventoryItemInfo kInventoryItemInfo;
//	GetInventoryItemInfo( iItemUID, kInventoryItemInfo );
//	vecUpdated.push_back( kInventoryItemInfo );
//	vecSocketResult = kInventoryItemInfo.m_kItemInfo.m_vecItemSocket;
//
//	//////////////////////////////////////////////////////////////////////////	
//	// 마법석 삭제
//	if( !bIsRemoveSocketOption && !bDebug )
//	{
//		int iCategory, iSlotID;
//		int iDecreased;
//		GetCategorySlot( iMagicStoneUID, iCategory, iSlotID );
//		DecreaseQuantity( iMagicStoneUID, 1, iDecreased, KDeletedItemInfo::DR_SOCKET_MAGIC_STONE );
//		LIF( iDecreased == 1 );
//		GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
//		vecUpdated.push_back( kInventoryItemInfo );
//	}
//	//////////////////////////////////////////////////////////////////////////	
//
//	if( !bDebug )
//	{
//		//{{ 2008. 3. 10  최육사  소켓 통계	
//		// 통계
//		if( iMagicStoneID > 0 )
//		{
//			KStatisticsKey kKeyMS;
//			kKeyMS.m_vecIntKey.push_back( iMagicStoneID );
//			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyMS, KStatistics::SI_ITEM_USE, 1 );
//		}
//
//		// ED 사용량 통계
//		KStatisticsKey kKey;
//		kKey.m_vecIntKey.push_back( 0 );
//		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MSocket, iED );
//		//}}
//	}
//
//	SET_ERROR( NET_OK );
//	return true;
//}
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_SOCKET_NEW
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2008. 9. 3  최육사		속성강화
bool KInventory::AttribEnchantItem( IN UidType iItemUID,
								    IN char cAttribEnchantSlotNo,
									IN OUT char& cAttribEnchantType,									
									OUT std::vector< KInventoryItemInfo >& vecUpdated )
{
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	std::map< int, int >	   mapRequireItem; // 감정석 및 복구석

	// 1. 속성강화할 아이템 검사
	if( IsExist( iItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_00 );
		return false;
	}

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iItemUID ) == true )
	{
		START_LOG( cerr, L"은행에서는 속성강화 못하는데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	//{{ 2008. 12. 21  최육사	사용불가 검사
	if( IsBrokenItem( iItemUID ) == true )
	{
		START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RESTORE_ITEM_06 );
		return false;
	}
	//}}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템을 속성강화하려고 했습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	// 2. 속성강화값 검증
	if( SiCXSLAttribEnchantItem()->VerifyAttribEnchantType( cAttribEnchantType ) == false )
	{
		START_LOG( cerr, L"잘못된 속성강화값입니다." )
			<< BUILD_LOGc( cAttribEnchantType )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
		return false;
	}

	// 3. 해당 아이템의 현재 속성강화값 얻기
	KItemAttributeEnchantInfo kAttribEnchantInfo;

	if( GetAttribEnchant( iItemUID, kAttribEnchantInfo ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템인데 속성강화값을 얻을려고 했다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_00 );
		return false;
	}

	// 4. 해당 속성강화에 필요한 ItemID얻기	
	const int iRequireItemID = CXSLAttribEnchantItem::GetRequireItemID( cAttribEnchantType );
	if( iRequireItemID < 0 )
	{
		START_LOG( cerr, L"해당 속성강화에 필요한 아이템정보를 얻을수가 없습니다." )
			<< BUILD_LOGc( cAttribEnchantType )
			<< BUILD_LOG( iRequireItemID )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
		return false;
	}

	// 5. 속성강화 가능한지 체크
	if( SiCXSLAttribEnchantItem()->IsPossibleAttribEnchant( cAttribEnchantType, cAttribEnchantSlotNo, kAttribEnchantInfo ) == false )
		return false;

	// 6. 무기인지 방어구인지 검사
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( GetItemID( iItemUID ) );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿이 존재하지 않습니다" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_06 );
		return false;
	}

	bool bWeapon = false;
	switch( pItemTemplet->m_ItemType )
	{
	case CXSLItem::IT_WEAPON:
		bWeapon = true;
		break;

	case CXSLItem::IT_DEFENCE:
		bWeapon = false;
		break;

	default:
		START_LOG( cerr, L"무기와 방어구만 속성강화 가능한데.." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_07 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// 속성석을 박는것이라면..
	const CXSLAttribEnchantItem::ATTRIB_COUNT_TYPE eAttribCountType = CXSLAttribEnchantItem::GetAttribCountType( kAttribEnchantInfo );
	int iRequireQuantity = 0;
	int iCost = 0;

	if( cAttribEnchantType != CXSLAttribEnchantItem::ET_NONE )
	{
		// 7. 필요한 속성강화석 수량 얻고 검사하기
		
		//{{ 2012. 06. 28	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
		int iAttributeLevel;
		if( 0 < pItemTemplet->m_iAttributeLevel )
		{
			iAttributeLevel = pItemTemplet->m_iAttributeLevel;
		}
		else
		{
			iAttributeLevel = pItemTemplet->m_UseLevel;
		}
		if( SiCXSLAttribEnchantItem()->GetRequireItemCount_Insert( eAttribCountType, bWeapon, iAttributeLevel, pItemTemplet->m_ItemGrade, iRequireQuantity ) == false )
#else
		if( SiCXSLAttribEnchantItem()->GetRequireItemCount_Insert( eAttribCountType, bWeapon, pItemTemplet->m_UseLevel, pItemTemplet->m_ItemGrade, iRequireQuantity ) == false )
#endif SERV_2012_PVP_SEASON2_1
			//}}
		{
			START_LOG( cerr, L"속성강화에 필요한 강화석 수량값을 얻을 수가 없습니다." )
				<< BUILD_LOG( eAttribCountType )
				<< BUILD_LOG( bWeapon )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< BUILD_LOG( pItemTemplet->m_ItemGrade )
				<< END_LOG;

			SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
			return false;
		}

		if( iRequireItemID != CXSLAttribEnchantItem::ATI_NONE )
		{
			mapRequireItem.insert( std::make_pair( iRequireItemID, iRequireQuantity ) );

			if( IsEnoughItemExistOnlyInventory( mapRequireItem ) == false )
			{
				SET_ERROR( ERR_ATTRIB_ENCHANT_05 );
				return false;
			}
		}

		// 8. 수수료
		if( !SiCXSLAttribEnchantItem()->GetRequireED_Insert( eAttribCountType, bWeapon, pItemTemplet->m_UseLevel, pItemTemplet->m_ItemGrade, iCost ) )
		{
			START_LOG( cerr, L"속성강화에 필요한 ED를 얻을 수가 없습니다." )
				<< BUILD_LOG( eAttribCountType )
				<< BUILD_LOG( bWeapon )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< BUILD_LOG( pItemTemplet->m_ItemGrade )
				<< END_LOG;

			SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
			return false;
		}

		// 9. 수수료 검사 및 차감
		if( m_pUser->GetED() < iCost )
		{
			SET_ERROR( ERR_ATTRIB_ENCHANT_04 );
			return false;
		}
		else
		{
			// 수수료 차감
			//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			m_pUser->DecreaseED( iCost, KUserEDManager::ER_OUT_ED_ITEM_ATTRIBUTE );
#else
			m_pUser->DecreaseED( iCost );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 속성석을 빼는거라면..
	else
	{
		// 8. 수수료
		if( !SiCXSLAttribEnchantItem()->GetRequireED_Remove( eAttribCountType, bWeapon, pItemTemplet->m_UseLevel, pItemTemplet->m_ItemGrade, iCost ) )
		{
			START_LOG( cerr, L"속성석 제거에 필요한 ED를 얻을 수가 없습니다." )
				<< BUILD_LOG( eAttribCountType )
				<< BUILD_LOG( bWeapon )
				<< BUILD_LOG( pItemTemplet->m_UseLevel )
				<< BUILD_LOG( pItemTemplet->m_ItemGrade )
				<< END_LOG;

			SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
			return false;
		}

		// 9. 수수료 검사 및 차감
		if( m_pUser->GetED() < iCost )
		{
			SET_ERROR( ERR_ATTRIB_ENCHANT_08 );
			return false;
		}
		else
		{
			//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			m_pUser->DecreaseED( iCost, KUserEDManager::ER_OUT_ED_ITEM_ATTRIBUTE );	// 수수료 차감
#else
			m_pUser->DecreaseED( iCost );	// 수수료 차감
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}
		}
	}
	
	// 10. 속성 강화
	if( SetAttribEnchant( iItemUID, cAttribEnchantSlotNo, cAttribEnchantType ) == false )
	{
		START_LOG( cerr, L"이미 검사했는데 에러다?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_00 );
		return false;
	}

	// 11. 강화에 성공하면 엘의 조각 삭제
	if( DeleteItemOnlyInventory( mapRequireItem, vecUpdated, KDeletedItemInfo::DR_ATTRIB_ENCHANT ) == false )
	{
		START_LOG( cerr, L"수량 검사를 했는데 삭제 실패? 일어날 수 없는 에러.." )
			<< BUILD_LOG( mapRequireItem.size() )
			<< BUILD_LOG( vecUpdated.size() )
			<< END_LOG;

		SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
		return false;
	}

	// 12. 강화된 아이템 인벤정보 얻기
	KInventoryItemInfo kInventoryItemInfo;
	GetInventoryItemInfo( iItemUID, kInventoryItemInfo );
	vecUpdated.push_back( kInventoryItemInfo );

	//////////////////////////////////////////////////////////////////////////
	// 통계
	if( iRequireItemID > 0 )
	{
		KStatisticsKey kKeyMS;
		kKeyMS.m_vecIntKey.push_back( iRequireItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyMS, KStatistics::SI_ITEM_USE, iRequireQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyMS, KStatistics::SI_ITEM_USE, iRequireQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// ED 사용량 통계
	if( iCost > 0 )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MAttribEnchant, iCost );
	}
	//////////////////////////////////////////////////////////////////////////

	SET_ERROR( NET_OK );
	return true;
}

bool KInventory::IdentifyItem( IN UidType iItemUID, 
							   IN int iQuantity, 
							   OUT std::map< int, int >& mapInsertedItem, 
							   OUT std::vector< KInventoryItemInfo >& vecUpdated, 
							   OUT std::vector< KItemInfo >& vecNewItem )
{
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();
		
	std::map< UidType, int >   mapChangeItem;  // 무속성석
	std::map< int, int >	   mapRequireItem; // 감정석
	std::map< int, KItemInfo > mapResultItem;  // 결과템
	std::map< int, KItemInfo >::iterator mitRet;

	SET_ERROR( NET_OK );

	// 1. 감정될 아이템의 수량이 충분한지 검사
	mapChangeItem.insert( std::make_pair( iItemUID, iQuantity ) );

	if( !IsEnoughItemExist( mapChangeItem ) )
	{
		START_LOG( cerr, L"인벤토리에 해당 아이템들 수량만큼 없네?" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;

		SET_ERROR( ERR_IDENTIFY_01 );
		return false;
	}

	// 2. 감정 정보 얻기
	int iItemID = GetItemID( iItemUID );

	const CXSLAttribEnchantItem::KIdentifyInfo* pKIdentifyInfo = SiCXSLAttribEnchantItem()->GetIdentifyInfo( iItemID );
	if( pKIdentifyInfo == NULL )
	{
		START_LOG( cerr, L"감정 정보 템플릿을 얻지 못함" )			
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_IDENTIFY_00 );
		return false;
	}

	// 3. 감정석의 수량이 충분한지 검사	
	mapRequireItem.insert( std::make_pair( pKIdentifyInfo->m_iRequireItemID, iQuantity ) ); // 감정석

	if( !IsEnoughItemExistOnlyInventory( mapRequireItem ) )
	{
		START_LOG( cerr, L"인벤토리에 해당 아이템들 수량만큼 없네?" )
			<< BUILD_LOG( pKIdentifyInfo->m_iRequireItemID )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;

		SET_ERROR( ERR_IDENTIFY_01 );
		return false;
	}	

	//{{ 2008. 5. 27  최육사  랜덤 결과 인벤 공간 검사
	std::map< int, int > mapCheckCaseItem; // 속성석의 모든 확률에 대한 빈공간 검사 [특정 확률 높이기 방지]

	std::map< int, KLottery::KCaseUnit >::const_iterator mitLot = pKIdentifyInfo->m_kResult.m_mapCase.begin();
	for( ; mitLot != pKIdentifyInfo->m_kResult.m_mapCase.end(); ++mitLot )
	{
		mapCheckCaseItem.clear();
		mapCheckCaseItem.insert( std::make_pair( mitLot->first, iQuantity ) );

		if( !IsEnoughSpaceExist( mapCheckCaseItem ) )
		{
			SET_ERROR( ERR_IDENTIFY_02 );
			return false;
		}
	}
	//}}

	// 4. 랜덤 결과 얻기
	int iResultItemID = 0;
	for( int iCnt = 0; iCnt < iQuantity; ++iCnt )
	{
		iResultItemID = pKIdentifyInfo->m_kResult.Decision();
		if( iResultItemID == KLottery::CASE_BLANK )
		{
			START_LOG( cerr, L"확률이 100%가 아닌가? 있을수 없는 에러!" )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			// 실패 처리를 하자!
			SET_ERROR( ERR_IDENTIFY_03 );
			return false;
		}

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iResultItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iResultItemID )
				<< END_LOG;

			// 실패 처리를 하자!
			SET_ERROR( ERR_IDENTIFY_03 );
			return false;
		}

		mitRet = mapResultItem.find( iResultItemID );
		if( mitRet == mapResultItem.end() )
		{
			KItemInfo kInsertItemInfo;
			kInsertItemInfo.m_iItemID	 = iResultItemID;
			kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
			kInsertItemInfo.m_iQuantity	 = 1;
			kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;

			mapResultItem.insert( std::make_pair( iResultItemID, kInsertItemInfo ) );
		}
		else
		{
            mitRet->second.m_iQuantity += 1;
		}
	}	
	
	// 5. 랜덤 결과 인벤 공간 검사
	std::map< int, KItemInfo >::const_iterator mitCR;
	std::map< int, int > mapCheckResultItem;
	for( mitCR = mapResultItem.begin(); mitCR != mapResultItem.end(); ++mitCR )
	{
		mapCheckResultItem.insert( std::make_pair( mitCR->first, mitCR->second.m_iQuantity ) );
	}

	if( !IsEnoughSpaceExist( mapCheckResultItem ) )
	{
		SET_ERROR( ERR_IDENTIFY_02 );
		return false;
	}

	// 6. 습득 아이템 정보
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}

	// 7. 결과 처리
	if( !DeleteAndInsert( mapRequireItem, mapChangeItem, mapResultItem, vecUpdated, vecNewItem, true, KDeletedItemInfo::DR_IDENTIFY ) )
	{
		SET_ERROR( ERR_IDENTIFY_03 );
		return false;
	}

	// 감정석 사용 통계
	KStatisticsKey kIdentifyStone;
	kIdentifyStone.m_vecIntKey.push_back( pKIdentifyInfo->m_iRequireItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kIdentifyStone, KStatistics::SI_ITEM_USE, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kIdentifyStone, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	// 감정 결과 아이템 통계
	std::map< int, int >::const_iterator mit;
	for( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_IDENTIFY, mit->second );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_IDENTIFY, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	return true;
}
//}}

//{{ 2008. 4. 14  최육사  스킬 초기화
bool KInventory::ResetSkillItem( OUT KInventoryItemInfo& kInventoryItemInfo )
{
	// 스킬 초기화 아이템 검사
	std::vector< KInventoryItemInfo > vecResetSkillItem;

#ifdef SERV_UNLIMITED_SKILL_RESET_ITEM
	GetInventoryItemInfoContainingThisItem( CXSLItem::CI_UNLIMITED_SKILL_RESET_ITEM, vecResetSkillItem, true );
	if( false == vecResetSkillItem.empty() )
	{
		return true;
	}
#endif SERV_UNLIMITED_SKILL_RESET_ITEM

	GetInventoryItemInfoContainingThisItem( CXSLItem::CI_SKILL_RESET_EVENT_ITEM2, vecResetSkillItem, true );
	if( true == vecResetSkillItem.empty() )
	{
#ifdef SERV_NEW_ITEM_COBO_SKILL_RESET_ITEM// 작업날짜: 2013-07-02	// 박세훈
		GetInventoryItemInfoContainingThisItem( CXSLItem::CI_COBO_SKILL_RESET_ITEM, vecResetSkillItem, true );
		if( true == vecResetSkillItem.empty() )
		{
#endif // SERV_NEW_ITEM_COBO_SKILL_RESET_ITEM
			GetInventoryItemInfoContainingThisItem( CXSLItem::CI_SKILL_RESET_EVENT_ITEM, vecResetSkillItem, true );
			if( true == vecResetSkillItem.empty() )
			{
#ifdef SERV_EVENT_RESET_A_SKILL_ITEM
			GetInventoryItemInfoContainingThisItem( CXSLItem::CI_RESET_A_SKILL_ITEM, vecResetSkillItem, true );
			if( true == vecResetSkillItem.empty() )
			{
#endif SERV_EVENT_RESET_A_SKILL_ITEM
#ifdef SERV_2ND_CLASS_SKILL_RESET
				GetInventoryItemInfoContainingThisItem( CXSLItem::CI_SKILL_RESET_2ND_CLASS_EVENT_ITEM, vecResetSkillItem, true );
				if (true == vecResetSkillItem.empty() )
				{
#endif SERV_2ND_CLASS_SKILL_RESET
#ifdef SERV_QUEST_SKILL_RESET
					GetInventoryItemInfoContainingThisItem( CXSLItem::CI_SKILL_RESET_QUEST_ITEM, vecResetSkillItem, true );
					if( true == vecResetSkillItem.empty() )
					{
#endif SERV_QUEST_SKILL_RESET
#ifdef SERV_EVENT_RURIEL_RESET_SKILL_ITEM
						GetInventoryItemInfoContainingThisItem( CXSLItem::EI_RURIEL_RESET_A_SKILL_ITEM, vecResetSkillItem, true );
						if( true == vecResetSkillItem.empty() )
						{
							GetInventoryItemInfoContainingThisItem( CXSLItem::EI_RURIEL_RESET_A_SKILL_EVENT_ITEM, vecResetSkillItem, true );
							if( true == vecResetSkillItem.empty() )
							{
#endif SERV_EVENT_RURIEL_RESET_SKILL_ITEM

#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
								GetInventoryItemInfoContainingThisItem( CXSLItem::EI_SKILL_RESET_EVENT_ITEM2, vecResetSkillItem, true );
								if( true == vecResetSkillItem.empty() )
								{
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
									GetInventoryItemInfoContainingThisItem( CXSLItem::CI_SKILL_RESET_ITEM, vecResetSkillItem, true );
									if( true == vecResetSkillItem.empty() )
									{
										// 스킬 1개 초기화 아이템없이 스킬 되돌리기를 하려고 한 경우
										return false;
									}
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
								}
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_EVENT_RURIEL_RESET_SKILL_ITEM
							}
						}
#endif SERV_EVENT_RURIEL_RESET_SKILL_ITEM
#ifdef SERV_QUEST_SKILL_RESET
					}
#endif SERV_QUEST_SKILL_RESET
#ifdef SERV_2ND_CLASS_SKILL_RESET
				}
#endif SERV_2ND_CLASS_SKILL_RESET
#ifdef SERV_EVENT_RESET_A_SKILL_ITEM
			}
#endif SERV_EVENT_RESET_A_SKILL_ITEM
			}
#ifdef SERV_NEW_ITEM_COBO_SKILL_RESET_ITEM// 작업날짜: 2013-07-02	// 박세훈
		}
#endif // SERV_NEW_ITEM_COBO_SKILL_RESET_ITEM
	}

	UidType iItemUID = 0;
	int iSlotCategory = 0, iSlotID = 0;
	std::vector< KInventoryItemInfo >::const_iterator vitRSI;
	for( vitRSI = vecResetSkillItem.begin(); vitRSI != vecResetSkillItem.end(); ++vitRSI )
	{
		// 스킬 초기화 아이템 UID 한개만 얻음.
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		iItemUID = GetItemUID( vitRSI->m_cSlotCategory, vitRSI->m_sSlotID );
#else
		iItemUID = GetItemUID( vitRSI->m_cSlotCategory, vitRSI->m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}

		if( iItemUID <= 0 )
		{
			START_LOG( cerr, L"인벤에서 스킬 초기화 아이템을 찾는중 이상한 아이템UID 발견." )
				<< BUILD_LOGc( vitRSI->m_cSlotCategory )
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				<< BUILD_LOGc( vitRSI->m_sSlotID )
#else
				<< BUILD_LOGc( vitRSI->m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		iSlotCategory = vitRSI->m_cSlotCategory;
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		iSlotID		  = vitRSI->m_sSlotID;
#else
		iSlotID		  = vitRSI->m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		break;
	}


	if( iItemUID == 0 )
		return false;

	int iDecreased = 0;
	DecreaseQuantity( iItemUID, 1, iDecreased, KDeletedItemInfo::DR_RESET_SKILL );
	LIF( iDecreased == 1 );
	GetInventorySlotInfo( iSlotCategory, iSlotID, kInventoryItemInfo );
	return true;
}
//}}

//{{ 2008. 8. 18  최육사	세트 아이템
int KInventory::GetEquippedSetItemCount( IN int iSetID )
{
	int iSetItemCount = 0;
	std::map< int, KInventoryItemInfo > mapEquippedItem;
	GetEquippedItem( mapEquippedItem );

	std::map< int, KInventoryItemInfo >::const_iterator mit;
	for( mit = mapEquippedItem.begin(); mit != mapEquippedItem.end(); ++mit )
	{
		const KInventoryItemInfo& kInventoryItemInfo = mit->second;

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInventoryItemInfo.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
            START_LOG( cerr, L"아이템 템플릿 정보가 이상합니다." )
				<< BUILD_LOG( kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< END_LOG;
			continue;
		}

		if( pItemTemplet->m_SetID == iSetID )
			++iSetItemCount;
	}

	return iSetItemCount;
}
//}}

//{{ 2011. 01. 20	최육사	아이템 분해 개편
#ifdef SERV_ITEM_RESOLVE_NEW
bool KInventory::GetSellPrice( IN const UidType iItemUID, IN const int iQuantity, OUT int& iSellPrice )
{
	iSellPrice = 0;

	KInventoryItemInfo kItemInfo;
	if( GetInventoryItemInfo( iItemUID, kItemInfo ) == false )
	{
		START_LOG( cerr, L"존재 하지 않는 아이템 입니다!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;
		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kItemInfo.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 정보가 존재하지 않습니다!" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( kItemInfo.m_kItemInfo.m_iItemID )
			<< END_LOG;
		return false;
	}

	// 돌려받는 가격 계산.
	/************************************************************************/
	/* <<< 내구도 아이템 되팔기 계산 공식 >>>
	// 수리비용 : [상점판매가격 * 0.2 * {(최대내구도 - 현재내구도) / (최대내구도 * 3)}]
	// 돌려받는 가격 : [상점판매가격 * 0.2 - 수리비용]
	/************************************************************************/

    switch( pItemTemplet->m_PeriodType )
    {
    case CXSLItem::PT_INFINITY:
        iSellPrice = ( int )floorf( 0.2f * ( float )pItemTemplet->m_Price + 0.01f );
        break;

    case CXSLItem::PT_ENDURANCE:
        {
			//{{ 2011. 05. 01  김민성	내구도 장비 되팔기 금액 수정
#ifdef SERV_SELL_ED_ITEM_PRICE_FIX
			float fOriginalPrice;
			int iEndurance;
			float fEnduranceRate = 1.f;
			_JIF( pItemTemplet->m_Endurance > 0, return false );
			_JIF( GetEndurance( iItemUID, iEndurance ), return false );
			_JIF( iEndurance >= 0, return false );
			fEnduranceRate = (float)iEndurance / (float)pItemTemplet->m_Endurance;
			fOriginalPrice = ( float )pItemTemplet->m_Price; // 판매가
			iSellPrice = ( int )floorf( ( fOriginalPrice * 0.2f * fEnduranceRate ) + 0.01f );
#else
			float fOriginalPrice;
			float fRepairPrice;
			int iEndurance;
			_JIF( pItemTemplet->m_Endurance > 0, return false );
			_JIF( GetEndurance( iItemUID, iEndurance ), return false );
			_JIF( iEndurance >= 0, return false );
			_JIF( pItemTemplet->m_Endurance >= iEndurance, return false );
			fOriginalPrice = ( float )pItemTemplet->m_Price; // 판매가
			fRepairPrice = ( float )pItemTemplet->m_RepairED * ( float )GetEnduranceToRestore( iItemUID );
			iSellPrice = ( int )floorf( ( ( float )fOriginalPrice * 0.2f ) - fRepairPrice + 0.01f );
#endif SERV_SELL_ED_ITEM_PRICE_FIX
			//}}
        }
        break;

    case CXSLItem::PT_QUANTITY:
		//////////////////////////////////////////////////////////////////////////
		// 2010-03-18  최육사
		// 아래 코드와 관련하여 홍콩에서 해킹으로 인한 ED버그 획득 이슈가 있었음.
		// 첫번째 문제는 클라이언트가 보내는 iQuantity값이 유효한 값인지 검사를 하지 않는 문제였고
		// 두번째 문제는 int타입을 float타입으로 타입캐스팅하면서 마이너스(-)부호가 소멸하여
		// 발생하는 문제였음.

        _JIF( pItemTemplet->m_Quantity > 0, return false );
        iSellPrice = ( int )floorf( 0.2f * ( float )pItemTemplet->m_Price * ( float )iQuantity + 0.01f );
        break;

    default:
        START_LOG( cerr, L"이상한 아이템 기간 타입." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( kItemInfo.m_kItemInfo.m_iItemID )
            << BUILD_LOG( pItemTemplet->m_PeriodType )
            << END_LOG;
        return false;
    }

	//{{ 2007. 11. 22  최육사  판매가 교정
	if( iSellPrice < 0 )
	{
		START_LOG( cwarn, L"장비템 수리 안하고 팔았나?" )
			<< BUILD_LOG( kItemInfo.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( iQuantity )
			<< BUILD_LOG( pItemTemplet->m_PeriodType )
			<< BUILD_LOG( pItemTemplet->m_Endurance )
			<< BUILD_LOG( pItemTemplet->m_Price )
			<< BUILD_LOG( pItemTemplet->m_RepairED )
			<< BUILD_LOG( iSellPrice )
			<< END_LOG;

		// 0 ED로 판매가 수정
		iSellPrice = 0;
	}
	//}}
	return true;
}
#endif SERV_ITEM_RESOLVE_NEW
//}}

#ifdef	SERV_SHARING_BANK_TEST
bool KInventory::CheckCanMoveShareItem( IN UidType iItemUID, OUT KDBE_SEAL_ITEM_REQ &kSealReq )
{
	std::map< UidType, KInventoryItem >::iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"해당 아이템 UID가 없음." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	KInventoryItemInfo kItemInfo;
	// 봉인 또는 봉인해제될 아이템 검사
	if( GetInventoryItemInfo( iItemUID, kItemInfo ) == false )
	{
		START_LOG( cerr, L"은행 공유 : 인벤토리에 없는 아이템UID" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	int iItemID = kItemInfo.m_kItemInfo.m_iItemID;
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"은행 공유 : 아이템 템플릿 얻기 실패" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	if( kItemInfo.m_kItemInfo.m_sPeriod > 0 )
	{
		START_LOG( cerr, L"은행 공유 : 기간제 아이템은 이동 할 수 없음" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( kItemInfo.m_kItemInfo.m_sPeriod )
			<< END_LOG;

		return false;
	}

	if( pItemTemplet->m_bVested == true )
	{
		if( SiCXSLItemManager()->IsCanSealForCashItem( iItemID ) )
		{
			if( IsSealedItem( iItemUID ) == false &&
#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
				( pItemTemplet->m_ucMaxSealCount > mit->second.GetSealCount() ||
					pItemTemplet->m_ucMaxSealCount == CXSLInventory::SEAL_COUNT_MAX )
#else
				pItemTemplet->m_ucMaxSealCount > mit->second.GetSealCount()
#endif // SERV_REMOVE_SEAL_COUNT_DECREASE				
				)
			{
#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
				if( pItemTemplet->m_ucMaxSealCount != CXSLInventory::SEAL_COUNT_MAX )
#endif // SERV_REMOVE_SEAL_COUNT_DECREASE
					//	봉인 가능 횟수 차감
				mit->second.IncreaseSealCount();


				//	봉인 차감시에는 DB에 바로 업데이트해준다
				std::set< UidType >::iterator sit = m_setShareItem.find( iItemUID );

				if( sit == m_setShareItem.end() )
				{
					kSealReq.m_iUnitUID = m_pUser->GetCharUID();
				}
				else
				{
					kSealReq.m_iUnitUID = GetShareUnitUID();
				}

				kSealReq.m_iItemUID = iItemUID;
				kSealReq.m_ucSealResult = mit->second.GetSealData();
			}

			else if( IsSealedItem( iItemUID ) == false && 
#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
				( pItemTemplet->m_ucMaxSealCount <= mit->second.GetSealCount() &&
				pItemTemplet->m_ucMaxSealCount != CXSLInventory::SEAL_COUNT_MAX )
#else
				pItemTemplet->m_ucMaxSealCount <= mit->second.GetSealCount() 
#endif // SERV_REMOVE_SEAL_COUNT_DECREASE				
				)
			{
				START_LOG( cerr, L"은행 공유 : 더이상 봉인 가능 횟수를 차감 할 수 없음" )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOG( mit->second.GetSealCount() )
					<< END_LOG;

				return false;
			}
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 귀속 아이템은 이동할 수 없음" )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			return false;
		}
	}

	return true;
}
#endif	SERV_SHARING_BANK_TEST

//{{ 2008. 10. 9  최육사	아이템사용
bool KInventory::UseItemInInventory( IN UidType iItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo, IN bool bNotDeleteItem /*= false*/ )
{
	SET_ERROR( NET_OK );
	
	vecInventorySlotInfo.clear();

	if( m_pUser == NULL )
	{
		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_00 );
		return false;
	}

	// 1. 아이템 존재 여부
	const int iItemID = GetItemID( iItemUID );
	if( iItemID <= 0 )
	{
		START_LOG( cerr, L"존재하지 않는 아이템입니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_01 );
		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_01 );
		return false;
	}

	// 2. 인벤에서 사용가능한 아이템인지 검사
	if( !pItemTemplet->m_bCanUseInventory )
	{
		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_03 );
		return false;
	}

	// 3. 아이템 사용가능 레벨 검사
	if( pItemTemplet->m_UseLevel > m_pUser->GetLevel() )
	{
		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_02 );
		return false;
	}

	// 5. 해당 아이템의 인벤정보 얻기
	int iCategory = 0;
	int iSlotID = 0;
	if( !GetCategorySlot( iItemUID, iCategory, iSlotID ) )
	{
		START_LOG( cerr, L"카테고리 슬롯 정보 얻기 실패!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_01 );
		return false;
	}

	// 6. 해당 아이템의 수량 감소
	if( !bNotDeleteItem )
	{
		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			int iDecreased = 0;
			DecreaseQuantity( iItemUID, 1, iDecreased, KDeletedItemInfo::DR_USE_ITEM_IN_INVENTORY );
			if( 1 != iDecreased )
			{
				START_LOG( cerr, L"수량 변화 이상." )
					<< BUILD_LOG( iItemUID )				
					<< BUILD_LOG( iDecreased )
					<< END_LOG;

				SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_00 );
				return false;
			}
		}
		else
		{
			KInventoryItemInfo kDelInvenItem;
			if( !DeleteItem( iItemUID, kDelInvenItem, KDeletedItemInfo::DR_USE_ITEM_IN_INVENTORY ) )
			{
				START_LOG( cerr, L"아이템 삭제 실패." )
					<< BUILD_LOG( iItemUID )
					<< END_LOG;

				SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_00 );
				return false;
			}
		}

		// 아이템 사용 통계
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
	}

	// 7. 인벤정보
	KInventoryItemInfo kResultItemInfo;
	GetInventorySlotInfo( iCategory, iSlotID, kResultItemInfo );
	vecInventorySlotInfo.push_back( kResultItemInfo );
	
	return true;
}
//}}

//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-30	// 박세훈
bool KInventory::ItemExchangeCheck( IN const int iHouseID
					   , IN const UidType iSourceItemUID
					   , IN const int iSourceItemID
					   , IN const int iSourceQuantity
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
					   , IN int iEventMoney
#endif // SERV_EVENT_MONEY
					   , IN const int iDestItemID
					   )
{
	KInventoryItemInfo kSrcItemInfo;

	// 1. 특정 아이템을 체크해서 교환 요청한것인지 먼저 본다!
	if( iSourceItemUID != 0 )
	{
		// 실제로 해당 아이템이 존재하는지 체크!
		if( GetInventoryItemInfo( iSourceItemUID, kSrcItemInfo ) == false )
		{
			START_LOG( cerr, L"해당 아이템이 인벤토리에 없다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}

		// 존재 한다면 수량은 충분한지 체크!
		if( kSrcItemInfo.m_kItemInfo.m_iQuantity < iSourceQuantity )
		{
			START_LOG( cerr, L"해당 아이템의 수량이 충분하지 않다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< BUILD_LOG( iSourceQuantity )
				<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_iQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}

		// 기간제 아이템은 교환할 수 없다!
		if( kSrcItemInfo.m_kItemInfo.m_sPeriod != 0 )
		{
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-02	// 박세훈
			SET_ERROR( ERR_ITEM_EXCHANGE_06 );	// 기간제 아이템은 재료나 교환 아이템으로 사용할 수 없습니다.
#else
			START_LOG( cerr, L"기간제 아이템은 교환할 수 없다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< BUILD_LOG( iSourceQuantity )
				<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_sPeriod )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			return false;
		}

	}
	else
	{
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		if( iSourceItemID == CXSLItem::EI_EVENT_MONEY_ITEM )
		{
			if( iEventMoney < iSourceQuantity )
			{
				SET_ERROR( ERR_ITEM_EXCHANGE_00 );
				return false;
			}
		}
		else
		{
			// 교환 대상 아이템의 수량이 충분히 있는지 체크!
			std::map< int, int > mapToDeleteByItemID;
			mapToDeleteByItemID.insert( std::make_pair( iSourceItemID, iSourceQuantity ) );

#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-06-25	// 박세훈
			if( IsEnoughItemExist( mapToDeleteByItemID, true, true ) == false )
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			if( IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) == false )
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			{
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-02	// 박세훈
				if( IsEnoughItemExist( mapToDeleteByItemID, true, false ) == true )
				{
					SET_ERROR( ERR_ITEM_EXCHANGE_06 );	// 기간제 아이템은 재료나 교환 아이템으로 사용할 수 없습니다.
				}
				else
				{
					START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
						<< BUILD_LOG( iSourceItemID )
						<< BUILD_LOG( iSourceQuantity )
						<< END_LOG;

					SET_ERROR( ERR_ITEM_EXCHANGE_01 );
				}
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
				START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
					<< BUILD_LOG( iSourceItemID )
					<< BUILD_LOG( iSourceQuantity )
					<< END_LOG;

				SET_ERROR( ERR_ITEM_EXCHANGE_01 );
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
				return false;
			}
		}
#else // SERV_EVENT_MONEY
		// 교환 대상 아이템의 수량이 충분히 있는지 체크!
		std::map< int, int > mapToDeleteByItemID;
		mapToDeleteByItemID.insert( std::make_pair( iSourceItemID, iSourceQuantity ) );

#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-06-25	// 박세훈
		if( IsEnoughItemExist( mapToDeleteByItemID, true, true ) == false )
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		if( IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) == false )
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		{
			START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
				<< BUILD_LOG( iSourceItemID )
				<< BUILD_LOG( iSourceQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}
#endif // SERV_EVENT_MONEY
	}

	//{{ 2011. 12. 28	김민성        랜덤 교환 어뷰징 방지
#ifdef SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	bool bRandomExchange = false;
#endif SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	//}}

	char cExchangeType = 0;
	std::map< int, KItemInfo > mapResultItem;  // 결과템

	// 2. 교환 결과 얻기
	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	if( SiCXSLItemManager()->GetItemExchangeResult( iHouseID, iSourceItemID, iSourceQuantity, kSrcItemInfo.m_kItemInfo, iDestItemID, mapResultItem, bRandomExchange, cExchangeType ) == false )
#else
	if( SiCXSLItemManager()->GetItemExchangeResult( iHouseID, iSourceItemID, iSourceQuantity, kSrcItemInfo.m_kItemInfo, iDestItemID, mapResultItem, bRandomExchange ) == false )
#endif SERV_EXCHANGE_LOG
	//}
	{
		// 교환할 수 없는 아이템입니다.
		SET_ERROR( ERR_ITEM_EXCHANGE_02 );
		return false;
	}

	// 3. 습득 아이템 정보
	std::map< int, int > mapInsertedItem;
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}

	//{{ 2011. 12. 28	김민성        랜덤 교환 어뷰징 방지
#ifdef SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	if( bRandomExchange == true )
	{
		// 교환과 관련있는 인벤토리에 빈공간이 있어야 한다.
		for( int iCategory = GetFirstCategory() ; iCategory < GetSentinelCategory() ; ++iCategory )
		{
			// 교환과 관련 없는 슬롯은 검사하지 말자
			if( iCategory == CXSLInventory::ST_BANK 
				|| iCategory == CXSLInventory::ST_E_EQUIP 
				|| iCategory == CXSLInventory::ST_E_SKILL 
				|| iCategory == CXSLInventory::ST_E_QUICK_SLOT 
				|| iCategory == CXSLInventory::ST_PC_BANG 
				|| iCategory == CXSLInventory::ST_PET )
			{
				continue;
			}

			if( IsEnoughEmptySlot( iCategory, (int)mapInsertedItem.size() ) == false )
			{
				SET_ERROR( ERR_PERSONAL_SHOP_51 );
				return false;
			}
		}
	}
	else
	{
		// 결과물의 카테고리 별 필요한 빈슬롯 수를 구하자
		if( IsEnoughSpaceExist( mapInsertedItem ) == false )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_51 );
			return false;
		}
	}
#endif SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	//}}

	return true;
}
#endif // SERV_ITEM_EXCHANGE_LIMIT
bool KInventory::ItemExchange( IN const int iHouseID, 
							  IN const UidType iSourceItemUID, 
							  IN const int iSourceItemID, 
							  IN const int iSourceQuantity, 
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
							  IN OUT int& iEventMoney, 
#endif // SERV_EVENT_MONEY
							  IN const int iDestItemID, 
							  OUT std::map< int, int >& mapInsertedItem, 
							  OUT std::vector< KInventoryItemInfo >& vecUpdated, 
							  OUT std::vector< KItemInfo >& vecNewItem,
							  OUT char& cExchangeType )
{
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();

	std::map< int, int > mapToDeleteByItemID;
	std::map< UidType, int > mapToDeleteByItemUID;
	std::map< int, KItemInfo > mapResultItem;  // 결과템
	KInventoryItemInfo kSrcItemInfo;

	SET_ERROR( NET_OK );

	// 1. 특정 아이템을 체크해서 교환 요청한것인지 먼저 본다!
	if( iSourceItemUID != 0 )
	{
		// 실제로 해당 아이템이 존재하는지 체크!
		if( GetInventoryItemInfo( iSourceItemUID, kSrcItemInfo ) == false )
		{
			START_LOG( cerr, L"해당 아이템이 인벤토리에 없다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}

		// 존재 한다면 수량은 충분한지 체크!
		if( kSrcItemInfo.m_kItemInfo.m_iQuantity < iSourceQuantity )
		{
			START_LOG( cerr, L"해당 아이템의 수량이 충분하지 않다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< BUILD_LOG( iSourceQuantity )
				<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_iQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}

		// 기간제 아이템은 교환할 수 없다!
		if( kSrcItemInfo.m_kItemInfo.m_sPeriod != 0 )
		{
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-02	// 박세훈
			SET_ERROR( ERR_ITEM_EXCHANGE_06 );	// 기간제 아이템은 재료나 교환 아이템으로 사용할 수 없습니다.
#else
			START_LOG( cerr, L"기간제 아이템은 교환할 수 없다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< BUILD_LOG( iSourceQuantity )
				<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_sPeriod )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			return false;
		}

		mapToDeleteByItemUID.insert( std::make_pair( iSourceItemUID, iSourceQuantity ) );

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		iEventMoney = 0;
#endif // SERV_EVENT_MONEY
	}
	else
	{
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		if( iSourceItemID == CXSLItem::EI_EVENT_MONEY_ITEM )
		{
			if( iEventMoney < iSourceQuantity )
			{
				SET_ERROR( ERR_ITEM_EXCHANGE_00 );
				return false;
			}

			iEventMoney = iSourceQuantity;
		}
		else
		{
			iEventMoney = 0;
			// 교환 대상 아이템의 수량이 충분히 있는지 체크!
			mapToDeleteByItemID.insert( std::make_pair( iSourceItemID, iSourceQuantity ) );

#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-06-25	// 박세훈
			if( IsEnoughItemExist( mapToDeleteByItemID, true, true ) == false )
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			if( IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) == false )
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			{
				START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
					<< BUILD_LOG( iSourceItemID )
					<< BUILD_LOG( iSourceQuantity )
					<< END_LOG;

				SET_ERROR( ERR_ITEM_EXCHANGE_01 );
				return false;
			}
		}
#else // SERV_EVENT_MONEY
		// 교환 대상 아이템의 수량이 충분히 있는지 체크!
		mapToDeleteByItemID.insert( std::make_pair( iSourceItemID, iSourceQuantity ) );

#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-06-25	// 박세훈
		if( IsEnoughItemExist( mapToDeleteByItemID, true, true ) == false )
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		if( IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) == false )
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		{
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-02	// 박세훈
			if( IsEnoughItemExist( mapToDeleteByItemID, true, false ) == true )
			{
				SET_ERROR( ERR_ITEM_EXCHANGE_06 );	// 기간제 아이템은 재료나 교환 아이템으로 사용할 수 없습니다.
			}
			else
			{
				START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
					<< BUILD_LOG( iSourceItemID )
					<< BUILD_LOG( iSourceQuantity )
					<< END_LOG;

				SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			}
#else // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
				<< BUILD_LOG( iSourceItemID )
				<< BUILD_LOG( iSourceQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
			return false;
		}
#endif // SERV_EVENT_MONEY
	}

	//{{ 2011. 12. 28	김민성        랜덤 교환 어뷰징 방지
#ifdef SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	bool bRandomExchange = false;
#endif SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	//}}

	// 2. 교환 결과 얻기
	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	if( SiCXSLItemManager()->GetItemExchangeResult( iHouseID, iSourceItemID, iSourceQuantity, kSrcItemInfo.m_kItemInfo, iDestItemID, mapResultItem, bRandomExchange, cExchangeType ) == false )
#else
	if( SiCXSLItemManager()->GetItemExchangeResult( iHouseID, iSourceItemID, iSourceQuantity, kSrcItemInfo.m_kItemInfo, iDestItemID, mapResultItem, bRandomExchange ) == false )
#endif SERV_EXCHANGE_LOG
	//}
	{
		// 교환할 수 없는 아이템입니다.
		SET_ERROR( ERR_ITEM_EXCHANGE_02 );
		return false;
	}

	//{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
	if( ( GetItemID( iSourceItemUID ) == 135186 ) || ( iSourceItemID == 135186 ) )
	{
		std::map< int, KItemInfo >::iterator it;
		for( it=mapResultItem.begin(); it != mapResultItem.end(); ++it )
		{
			vecDestItem.push_back( it->first );
		}
	}
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
	//}}

	// 3. 습득 아이템 정보
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}

	//{{ 2011. 12. 28	김민성        랜덤 교환 어뷰징 방지
#ifdef SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	if( bRandomExchange == true )
	{
		// 교환과 관련있는 인벤토리에 빈공간이 있어야 한다.
		for( int iCategory = GetFirstCategory() ; iCategory < GetSentinelCategory() ; ++iCategory )
		{
			// 교환과 관련 없는 슬롯은 검사하지 말자
			if( iCategory == CXSLInventory::ST_BANK 
				|| iCategory == CXSLInventory::ST_E_EQUIP 
				|| iCategory == CXSLInventory::ST_E_SKILL 
				|| iCategory == CXSLInventory::ST_E_QUICK_SLOT 
				|| iCategory == CXSLInventory::ST_PC_BANG 
				|| iCategory == CXSLInventory::ST_PET )
			{
				continue;
			}

			if( IsEnoughEmptySlot( iCategory, (int)mapInsertedItem.size() ) == false )
			{
				SET_ERROR( ERR_PERSONAL_SHOP_51 );
				return false;
			}
		}
	}
	else
	{
		// 결과물의 카테고리 별 필요한 빈슬롯 수를 구하자
		if( IsEnoughSpaceExist( mapInsertedItem ) == false )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_51 );
			return false;
		}
	}
#endif SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	//}}

	// 4. 결과 처리
	if( DeleteAndInsert( mapToDeleteByItemID
		, mapToDeleteByItemUID
		, mapResultItem
		, vecUpdated
		, vecNewItem
		, true
		, KDeletedItemInfo::DR_EXCHANGE
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
		, true
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
		) == false
		)
	{
		SET_ERROR( ERR_ITEM_EXCHANGE_00 );
		return false;
	}

	// 아이템 교환 통계
	KStatisticsKey kKeyRandom;
	kKeyRandom.m_vecIntKey.push_back( iSourceItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyRandom, KStatistics::SI_ITEM_EXCHANGE_S, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyRandom, KStatistics::SI_ITEM_EXCHANGE_S, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	// 아이템 교환 통계
	std::map<int,int>::const_iterator mit;
	for( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_EXCHANGE_R, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_EXCHANGE_R, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	return true;
}
#else
/*
//{{ 2011. 08. 16	최육사	아이템 교환 개편
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_ITEM_EXCHANGE_NEW
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
bool KInventory::ItemExchange( IN const int iHouseID, 
							  IN const UidType iSourceItemUID, 
							  IN const int iSourceItemID, 
							  IN const int iSourceQuantity, 
							  IN const int iDestItemID, 
							  OUT std::map< int, int >& mapInsertedItem, 
							  OUT std::vector< KInventoryItemInfo >& vecUpdated, 
							  OUT std::vector< KItemInfo >& vecNewItem,
							  //{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
							  IN int&	iEventMoney,
							  OUT std::vector<int>& vecDestItem
#else
							  IN int&	iEventMoney
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
							  //}}
							  )
#else // SERV_EVENT_MONEY
bool KInventory::ItemExchange( IN const int iHouseID, 
							  IN const UidType iSourceItemUID, 
							  IN const int iSourceItemID, 
							  IN const int iSourceQuantity, 
							  IN const int iDestItemID, 
							  OUT std::map< int, int >& mapInsertedItem, 
							  OUT std::vector< KInventoryItemInfo >& vecUpdated, 
							  OUT std::vector< KItemInfo >& vecNewItem )
#endif // SERV_EVENT_MONEY
{
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();

	std::map< int, int > mapToDeleteByItemID;
	std::map< UidType, int > mapToDeleteByItemUID;
	std::map< int, KItemInfo > mapResultItem;  // 결과템
	KInventoryItemInfo kSrcItemInfo;

	SET_ERROR( NET_OK );

	// 1. 특정 아이템을 체크해서 교환 요청한것인지 먼저 본다!
	if( iSourceItemUID != 0 )
	{
		// 실제로 해당 아이템이 존재하는지 체크!
		if( GetInventoryItemInfo( iSourceItemUID, kSrcItemInfo ) == false )
		{
			START_LOG( cerr, L"해당 아이템이 인벤토리에 없다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}

		// 존재 한다면 수량은 충분한지 체크!
		if( kSrcItemInfo.m_kItemInfo.m_iQuantity < iSourceQuantity )
		{
			START_LOG( cerr, L"해당 아이템의 수량이 충분하지 않다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< BUILD_LOG( iSourceQuantity )
				<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_iQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}

		// 기간제 아이템은 교환할 수 없다!
		if( kSrcItemInfo.m_kItemInfo.m_sPeriod != 0 )
		{
			START_LOG( cerr, L"기간제 아이템은 교환할 수 없다!" )
				<< BUILD_LOG( iSourceItemUID )
				<< BUILD_LOG( iSourceQuantity )
				<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_sPeriod )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}

		mapToDeleteByItemUID.insert( std::make_pair( iSourceItemUID, iSourceQuantity ) );

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		EventMoney = 0;
#endif // SERV_EVENT_MONEY
	}
	else
	{
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		if( iSourceItemID == CXSLItem::EI_EVENT_MONEY_ITEM )
		{
			if( EventMoney < iSourceQuantity )
			{
				SET_ERROR( ERR_ITEM_EXCHANGE_00 );
				return false;
			}

			EventMoney = iSourceQuantity;
		}
		else
		{
			EventMoney = 0;
			// 교환 대상 아이템의 수량이 충분히 있는지 체크!
			mapToDeleteByItemID.insert( std::make_pair( iSourceItemID, iSourceQuantity ) );

			if( IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) == false )
			{
				START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
					<< BUILD_LOG( iSourceItemID )
					<< BUILD_LOG( iSourceQuantity )
					<< END_LOG;

				SET_ERROR( ERR_ITEM_EXCHANGE_01 );
				return false;
			}
		}
#else // SERV_EVENT_MONEY
		// 교환 대상 아이템의 수량이 충분히 있는지 체크!
		mapToDeleteByItemID.insert( std::make_pair( iSourceItemID, iSourceQuantity ) );

		if( IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) == false )
		{
			START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
				<< BUILD_LOG( iSourceItemID )
				<< BUILD_LOG( iSourceQuantity )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_EXCHANGE_01 );
			return false;
		}
#endif // SERV_EVENT_MONEY
	}

	//{{ 2011. 12. 28	김민성        랜덤 교환 어뷰징 방지
#ifdef SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	bool bRandomExchange = false;
#endif SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	//}}

	// 2. 교환 결과 얻기
	if( SiCXSLItemManager()->GetItemExchangeResult( iHouseID, iSourceItemID, iSourceQuantity, kSrcItemInfo.m_kItemInfo, iDestItemID, mapResultItem, bRandomExchange ) == false )
	{
		// 교환할 수 없는 아이템입니다.
		SET_ERROR( ERR_ITEM_EXCHANGE_02 );
		return false;
	}

	//{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
	if( ( GetItemID( iSourceItemUID ) == 135186 ) || ( iSourceItemID == 135186 ) )
	{
		std::map< int, KItemInfo >::iterator it;
		for( it=mapResultItem.begin(); it != mapResultItem.end(); ++it )
		{
			vecDestItem.push_back( it->first );
		}
	}
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
	//}}

	// 3. 습득 아이템 정보
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}

	//{{ 2011. 12. 28	김민성        랜덤 교환 어뷰징 방지
#ifdef SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	if( bRandomExchange == true )
	{
		// 교환과 관련있는 인벤토리에 빈공간이 있어야 한다.
		for( int iCategory = GetFirstCategory() ; iCategory < GetSentinelCategory() ; ++iCategory )
		{
			// 교환과 관련 없는 슬롯은 검사하지 말자
			if( iCategory == CXSLInventory::ST_BANK 
				|| iCategory == CXSLInventory::ST_E_EQUIP 
				|| iCategory == CXSLInventory::ST_E_SKILL 
				|| iCategory == CXSLInventory::ST_E_QUICK_SLOT 
				|| iCategory == CXSLInventory::ST_PC_BANG 
				|| iCategory == CXSLInventory::ST_PET )
			{
				continue;
			}

			if( IsEnoughEmptySlot( iCategory, (int)mapInsertedItem.size() ) == false )
			{
				SET_ERROR( ERR_PERSONAL_SHOP_51 );
				return false;
			}
		}
	}
	else
	{
		// 결과물의 카테고리 별 필요한 빈슬롯 수를 구하자
		if( IsEnoughSpaceExist( mapInsertedItem ) == false )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_51 );
			return false;
		}
	}
#endif SERV_ITEM_RANDOM_EXCHANGE_ABUSER
	//}}

	// 4. 결과 처리
	if( DeleteAndInsert( mapToDeleteByItemID, mapToDeleteByItemUID, mapResultItem, vecUpdated, vecNewItem, true, KDeletedItemInfo::DR_EXCHANGE ) == false )
	{
		SET_ERROR( ERR_ITEM_EXCHANGE_00 );
		return false;
	}

	// 아이템 교환 통계
	KStatisticsKey kKeyRandom;
	kKeyRandom.m_vecIntKey.push_back( iSourceItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyRandom, KStatistics::SI_ITEM_EXCHANGE_S, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyRandom, KStatistics::SI_ITEM_EXCHANGE_S, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	// 아이템 교환 통계
	std::map<int,int>::const_iterator mit;
	for( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_EXCHANGE_R, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_EXCHANGE_R, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
bool KInventory::ItemExchange( IN const int iItemID, IN const int iHouseID, IN const int iSelectedItemID, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem )
{
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();

	std::map< int, int > mapToDeleteByItemID;
	std::map< int, KItemInfo > mapResultItem;  // 결과템

	SET_ERROR( NET_OK );

	// 1. 교환 대상 아이템 존재 여부
	mapToDeleteByItemID.insert( std::make_pair( iItemID, 1 ) );

	if( IsEnoughItemExistOnlyInventory( mapToDeleteByItemID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_EXCHANGE_01 );
		return false;
	}

	// 2. 교환 결과 얻기
	if( SiCXSLItemManager()->GetItemExchangeResult( iHouseID, iItemID, iSelectedItemID, mapResultItem ) == false )
	{
		// 교환할 수 없는 아이템입니다.
		SET_ERROR( ERR_ITEM_EXCHANGE_02 );
		return false;
	}

	// 3. 습득 아이템 정보
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}

	// 4. 결과 처리
	if( DeleteAndInsert( mapToDeleteByItemID, mapResultItem, vecUpdated, vecNewItem, true, KDeletedItemInfo::DR_EXCHANGE ) == false )
	{
		SET_ERROR( ERR_ITEM_EXCHANGE_00 );
		return false;
	}

	// 아이템 교환 통계
	KStatisticsKey kKeyRandom;
	kKeyRandom.m_vecIntKey.push_back( iItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyRandom, KStatistics::SI_ITEM_EXCHANGE_S, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyRandom, KStatistics::SI_ITEM_EXCHANGE_S, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	// 아이템 교환 통계
	std::map<int,int>::const_iterator mit;
	for( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_EXCHANGE_R, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_EXCHANGE_R, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_ITEM_EXCHANGE_NEW
//////////////////////////////////////////////////////////////////////////
//}}
*/
#endif SERV_EXCHANGE_LOG
//}

//{{ 2008. 12. 25  최육사	부여
bool KInventory::EnchantAttachItem( IN UidType iDestItemUID, IN UidType iAttachItemUID, OUT int& iLevelAfterEnchant, OUT std::vector< KInventoryItemInfo >& vecUpdated )
{
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	// 1. 부여될 아이템 및 부여아이템 검사
	if( !IsExist( iDestItemUID )  &&  !IsExist( iAttachItemUID ) )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( iAttachItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_00 );
		return false;
	}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iDestItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템에 강화 부여를 하려고 했습니다." )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	int iDestItemID	  = GetItemID( iDestItemUID );
	int iAttachItemID = GetItemID( iAttachItemUID );

	// 2. 부여 아이템이라면 강화레벨 얻자!
	int iAttachEnchantLevel = 0;
	int iItemType = 0;
	if( !SiCXSLEnchantItemManager()->GetAttachItemEnchantLevel( iAttachItemID, iAttachEnchantLevel, iItemType ) )
	{
		// 부여 아이템이 아니네!

		SET_ERROR( ERR_ATTACH_ITEM_01 );
		return false;
	}

	// 3. 템플릿 정보 얻기
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iDestItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iDestItemID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_04 );
		return false;
	}

	//{{ 2011. 06. 28	최육사	강화 부적 기능 수정
#ifdef SERV_ENCHANT_ATTACH_MODIFY
	const CXSLItem::ItemTemplet *pAttachItemTemplet = SiCXSLItemManager()->GetItemTemplet( iAttachItemID );
	if( pAttachItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iAttachItemID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_04 );
		return false;
	}
#endif SERV_ENCHANT_ATTACH_MODIFY
	//}}

	// 3. 강화가능한 템인지 검사
	if( !pItemTemplet->m_bCanEnchant )
	{
		START_LOG( cerr, L"강화할 수 있는 아이템이 아닌데 강화부여를 하려고 한다?" )
			<< BUILD_LOG( iDestItemID )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_02 );
		return false;
	}

	//{{ 2009. 4. 20  최육사	강화 타입 구별
	if( iItemType == CXSLItem::IT_NONE )
	{
		if( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON  &&
			pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE )
		{
			START_LOG( cerr, L"강화할 수 있는 아이템이 아닌데 강화부여를 하려고 한다?" )
				<< BUILD_LOG( iDestItemID )
				<< END_LOG;

			SET_ERROR( ERR_ATTACH_ITEM_02 );
			return false;
		}
	}
	else if( iItemType == CXSLItem::IT_DEFENCE )
	{
		if( pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE )
		{
			START_LOG( cerr, L"강화할 수 있는 아이템이 아닌데 강화부여를 하려고 한다?" )
				<< BUILD_LOG( iDestItemID )
				<< END_LOG;

			SET_ERROR( ERR_ATTACH_ITEM_02 );
			return false;
		}
	}
	else
	{
		START_LOG( cerr, L"강화부여 테이블 정보가 이상하다!? 일어날 수 없는 에러!" )
			<< BUILD_LOG( iItemType )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_02 );
		return false;
	}
	//}}	

#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// 작업날짜: 2013-07-02	// 박세훈
	if( SiCXSLEnchantItemManager()->CheckAttachItemEnableLevel( iAttachItemID, pItemTemplet->m_UseLevel ) == false )
	{
		SET_ERROR( ERR_ATTACH_ITEM_04 );	// 해당 강화의 부적으로는 강화할 수 없는 사용 레벨의 장비입니다.
		return false;
	}
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM

	// 4. 사용불가 아이템인지 검사
	if( IsBrokenItem( iDestItemUID ) )
	{
		START_LOG( cerr, L"사용불가 아이템은 강화부여 불가능." )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_02 );
		return false;
	}

	// 5. 강화 레벨 얻기
	int iEnchantLevel;
	if( !GetEnchantLevel( iDestItemUID, iEnchantLevel ) )
	{
		START_LOG( cerr, L"강화 레벨 얻기 실패." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( iDestItemID )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_00 );
		return false;
	}

	// 6. 목표 아이템의 강화레벨이 부여 강화레벨과 같거나 크면 안된다
	if( iEnchantLevel >= iAttachEnchantLevel )
	{
		START_LOG( cerr, L"목표템 강화레벨이 부여강화레벨과 같거나 크다." )
			<< BUILD_LOG( iEnchantLevel )
			<< BUILD_LOG( iAttachEnchantLevel )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_02 );
		return false;
	}

	// 7. 강화레벨 범위 검사
	if( iEnchantLevel < 0  ||  iEnchantLevel > CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
	{
		START_LOG( cerr, L"강화 레벨 이상." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( iDestItemID )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_02 );
		return false;
	}

	// 8. 부여시킬 아이템의 슬롯 정보 얻기
	int iCategory;
	int iSlotID;
	_JIF( GetCategorySlot( iDestItemUID, iCategory, iSlotID ), return false );

	if( iCategory == CXSLInventory::ST_E_EQUIP )
	{
		START_LOG( cerr, L"장착 중인 아이템 강화 시도." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_02 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// 9. 강화 부여 작업
	KInventoryItemInfo kInventoryItemInfo;
	int iIncreased = 0;
	int iLevelChange = ( iAttachEnchantLevel - iEnchantLevel ); // 부여할 강화레벨
	IncreaseEnchantLevel( iDestItemUID, iLevelChange, iIncreased );
	LIF( iIncreased == iLevelChange );
	GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
	vecUpdated.push_back( kInventoryItemInfo );

	// 11. 부여 아이템 삭제
	//{{ 2011. 06. 28	최육사	강화 부적 기능 수정
#ifdef SERV_ENCHANT_ATTACH_MODIFY
	if( pAttachItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
	{
		int iDecreased = 0;
		GetCategorySlot( iAttachItemUID, iCategory, iSlotID );
		DecreaseQuantity( iAttachItemUID, 1, iDecreased, KDeletedItemInfo::DR_ATTACH );
		LIF( iDecreased == 1 );
		GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
		vecUpdated.push_back( kInventoryItemInfo );
	}
	else
	{
		DeleteItem( iAttachItemUID, kInventoryItemInfo, KDeletedItemInfo::DR_ATTACH );
		vecUpdated.push_back( kInventoryItemInfo );
	}
#else
	int iDecreased = 0;
	GetCategorySlot( iAttachItemUID, iCategory, iSlotID );
	DecreaseQuantity( iAttachItemUID, 1, iDecreased, KDeletedItemInfo::DR_ATTACH );
	LIF( iDecreased == 1 );
	GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo );
	vecUpdated.push_back( kInventoryItemInfo );
#endif SERV_ENCHANT_ATTACH_MODIFY
	//}}	
	//////////////////////////////////////////////////////////////////////////

	if( !GetEnchantLevel( iDestItemUID, iLevelAfterEnchant ) )
	{
		START_LOG( cerr, L"강화 레벨 얻기 실패. 일어날 수 없는 에러." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( iDestItemID )
			<< END_LOG;

		SET_ERROR( ERR_ATTACH_ITEM_00 );
		return false;
	}

	// 부여 아이템 사용 통계
	{
		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( iAttachItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	SET_ERROR( NET_OK );
	return true;
}
//}}

//{{ 2009. 8. 27  최육사	봉인
bool KInventory::SealItem( IN UidType iDestItemUID, IN UidType iSealItemUID, OUT u_char& ucBeforeSealCount, OUT u_char& ucSealResult, OUT std::vector< KInventoryItemInfo >& vecUpdated )
{
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	KInventoryItemInfo kDestItemInfo;

	// 봉인 또는 봉인해제될 아이템 검사
	if( GetInventoryItemInfo( iDestItemUID, kDestItemInfo ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_00 );
		return false;
	}

	// 봉인 이전 상태 저장
	ucBeforeSealCount = kDestItemInfo.m_kItemInfo.GetSealCount();

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iDestItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는 아이템은 봉인 또는 봉인해제 못하는데?" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	if( IsDBUpdateItem( iDestItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템을 봉인하려고 했습니다." )
			<< BUILD_LOG( kDestItemInfo.m_kItemInfo.m_iItemID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	// 템플릿 정보 얻기
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kDestItemInfo.m_kItemInfo.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( kDestItemInfo.m_kItemInfo.m_iItemID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_01 );
		return false;
	}

	// 장비 또는 아바타 아이템인지 확인
	if( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON  &&
		pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE  &&
		pItemTemplet->m_ItemType != CXSLItem::IT_ACCESSORY  &&
#ifdef SERV_GOLD_TICKET_SEAL_COUNT
		CXSLItem::IsGoldTicket(pItemTemplet->m_ItemID) != true &&
#endif SERV_GOLD_TICKET_SEAL_COUNT
		pItemTemplet->m_bFashion == false )
	{
		START_LOG( cerr, L"장비 또는 악세사리 및 아바타 아이템이 아닙니다" )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_02 );
		return false;
	}

	// 장착중인 아이템인가?
	if( kDestItemInfo.m_cSlotCategory == CXSLInventory::ST_E_EQUIP )
	{
		START_LOG( cerr, L"장착 중인 아이템 봉인 및 봉인해제 시도." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOGc( kDestItemInfo.m_cSlotCategory )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_03 );
		return false;
	}
	
	// 이미 봉인된 아이템인가?
	if( kDestItemInfo.m_kItemInfo.IsSealedItem() == true )
	{
		START_LOG( cerr, L"이미 봉인된 아이템을 봉인하려 합니다!" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_04 );
		return false;
	}
	
	// 내구도 아이템이라면 수리는 되었는가?
	if( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE )
	{
		if( kDestItemInfo.m_kItemInfo.m_sEndurance < CXSLItem::EN_MAX_ENDURANCE )
		{
			START_LOG( cerr, L"수리되지 않은 아이템은 봉인할 수 없다" )
				<< BUILD_LOG( iDestItemUID )
				<< BUILD_LOG( kDestItemInfo.m_kItemInfo.m_sEndurance )
				<< END_LOG;

			SET_ERROR( ERR_SEAL_ITEM_05 );
			return false;
		}
	}
	
	// 기간제 아이템인가?
	if( kDestItemInfo.m_kItemInfo.m_sPeriod > 0 )
	{
		START_LOG( cerr, L"기간제 아이템은 봉인할 수 없다!" )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( kDestItemInfo.m_kItemInfo.m_sPeriod )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_06 );
		return false;
	}

	// 파괴된 아이템인가?
	if( IsBrokenItem( iDestItemUID ) == true )
	{
		START_LOG( cerr, L"파괴된 아이템은 봉인할 수 없다!" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_07 );
		return false;
	}

	// 거래 가능한 아이템
	if( pItemTemplet->m_bVested == false )
	{
		START_LOG( cerr, L"거래 가능한 아이템은 봉인할 수 없다" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_08 );
		return false;
	}

	// 봉인 불가능으로 설정?

	// 봉인 회수 초과
	if( kDestItemInfo.m_kItemInfo.GetSealCount() >= pItemTemplet->m_ucMaxSealCount 
#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE		
		&& pItemTemplet->m_ucMaxSealCount != CXSLInventory::SEAL_COUNT_MAX
#endif // SERV_REMOVE_SEAL_COUNT_DECREASE
		)
	{
        START_LOG( cerr, L"봉인 횟수 제한입니다." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOGc( kDestItemInfo.m_kItemInfo.GetSealCount() )
			<< BUILD_LOGc( pItemTemplet->m_ucMaxSealCount )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_09 );
		return false;
	}

	////////////////////////////////////////////////////////////////////////// 봉인 주문서 검사
	int iSealItemID = 0;

	switch( pItemTemplet->m_ItemGrade )
	{
	case CXSLItem::IG_UNIQUE:	iSealItemID = CXSLItem::CI_SEAL_ITEM_UNIQUE;	break;
	case CXSLItem::IG_ELITE:	iSealItemID = CXSLItem::CI_SEAL_ITEM_ELITE;		break;
	case CXSLItem::IG_RARE:		iSealItemID = CXSLItem::CI_SEAL_ITEM_RARE;		break;
	case CXSLItem::IG_NORMAL:	iSealItemID = CXSLItem::CI_SEAL_ITEM_NORMAL;	break;
	default:
		{
			START_LOG( cerr, L"봉인할수 없는 아이템 등급입니다!" )
				<< BUILD_LOG( iDestItemUID )
				<< BUILD_LOG( pItemTemplet->m_ItemGrade )
				<< END_LOG;

			SET_ERROR( ERR_SEAL_ITEM_02 );
			return false;
		}
	}

	// 봉인 주문서 검사
	KInventoryItemInfo kSealItemInfo;
	if( GetInventoryItemInfo( iSealItemUID, kSealItemInfo ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iSealItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_00 );
		return false;
	}

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iSealItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는 아이템은 봉인 또는 봉인해제 못하는데?" )
			<< BUILD_LOG( iSealItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	// 봉인가능한 아이템이 맞는지 검사
	if( kSealItemInfo.m_kItemInfo.m_iItemID != iSealItemID )
	{
#ifdef SERV_RURIEL_EVENT_ITEM
		int iEventSealItemID1 = 0;
		int iEventSealItemID2 = 0;
#ifdef SERV_ARIEL_EVENT_ITEM
		int iEventSealItemID3 = 0;
#endif SERV_ARIEL_EVENT_ITEM

		switch( pItemTemplet->m_ItemGrade )
		{
		case CXSLItem::IG_NORMAL:
			{
				iEventSealItemID1 = CXSLItem::CI_EVENT_SEAL_ITEM_NORMAL;
				iEventSealItemID2 = CXSLItem::CI_RURIEL_EVENT_SEAL_ITEM_NORMAL;
#ifdef SERV_ARIEL_EVENT_ITEM
				iEventSealItemID3 = CXSLItem::CI_ARIEL_EVENT_SEAL_ITEM_NORMAL;
#endif SERV_ARIEL_EVENT_ITEM
			}
			break;
		case CXSLItem::IG_RARE:
			{
#ifdef SERV_COUNTRY_JP
				iEventSealItemID1 = CXSLItem::CI_EVENT_SEAL_ITEM_RARE;
#endif //SERV_COUNTRY_JP
				iEventSealItemID2 = CXSLItem::CI_RURIEL_EVENT_SEAL_ITEM_RARE;
#ifdef SERV_ARIEL_EVENT_ITEM
				iEventSealItemID3 = CXSLItem::CI_ARIEL_EVENT_SEAL_ITEM_RARE;
#endif SERV_ARIEL_EVENT_ITEM
			}
			break;
		case CXSLItem::IG_ELITE:
			{
				iEventSealItemID1 = CXSLItem::CI_EVENT_SEAL_ITEM_ELITE;
				iEventSealItemID2 = CXSLItem::CI_RURIEL_EVENT_SEAL_ITEM_ELITE;
#ifdef SERV_ARIEL_EVENT_ITEM
				iEventSealItemID3 = CXSLItem::CI_ARIEL_EVENT_SEAL_ITEM_ELITE;
#endif SERV_ARIEL_EVENT_ITEM
			}
			break;
		case CXSLItem::IG_UNIQUE:
			{
#if defined(SERV_COUNTRY_JP) || defined(SERV_EVENT_SEAL_ITEM_UNIQUE)
				iEventSealItemID1 = CXSLItem::CI_EVENT_SEAL_ITEM_UNIQUE;
#endif //SERV_COUNTRY_JP || SERV_EVENT_SEAL_ITEM_UNIQUE
				iEventSealItemID2 = CXSLItem::CI_RURIEL_EVENT_SEAL_ITEM_UNIQUE;
#ifdef SERV_ARIEL_EVENT_ITEM
				iEventSealItemID3 = CXSLItem::CI_ARIEL_EVENT_SEAL_ITEM_UNIQUE;
#endif SERV_ARIEL_EVENT_ITEM
			}
			break;
		default:
			{
				START_LOG( cerr, L"선택한 아이템을 봉인할수 있는 봉인주문서가 아닙니다." )
					<< BUILD_LOG( iDestItemUID )
					<< BUILD_LOG( iSealItemUID )
					<< BUILD_LOG( kSealItemInfo.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( iSealItemID )
					<< END_LOG;

				SET_ERROR( ERR_SEAL_ITEM_10 );
				return false;
			}
		}

		if( 0 != iEventSealItemID1 && iEventSealItemID1 == kSealItemInfo.m_kItemInfo.m_iItemID )
			iSealItemID = iEventSealItemID1;
		else if( 0 != iEventSealItemID2 && iEventSealItemID2 == kSealItemInfo.m_kItemInfo.m_iItemID )
			iSealItemID = iEventSealItemID2;
#ifdef SERV_ARIEL_EVENT_ITEM
		else if( 0 != iEventSealItemID3 && iEventSealItemID3 == kSealItemInfo.m_kItemInfo.m_iItemID )
			iSealItemID = iEventSealItemID3;
#endif SERV_ARIEL_EVENT_ITEM

		if( kSealItemInfo.m_kItemInfo.m_iItemID != iSealItemID )
		{
			START_LOG( cerr, L"선택한 아이템을 봉인할수 있는 봉인주문서가 아닙니다." )
				<< BUILD_LOG( iDestItemUID )
				<< BUILD_LOG( iSealItemUID )
				<< BUILD_LOG( kSealItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOG( iSealItemID )
				<< END_LOG;

			SET_ERROR( ERR_SEAL_ITEM_10 );
			return false;
		}
#else //SERV_RURIEL_EVENT_ITEM
		//{{  2011.12.21     김민성    코보 발도장 이벤트
#ifdef SERV_COBO_SEAL_ITEM_EVENT
		switch( pItemTemplet->m_ItemGrade )
		{
#ifdef SERV_COUNTRY_JP
		case CXSLItem::IG_RARE:		iSealItemID = CXSLItem::CI_EVENT_SEAL_ITEM_RARE;	break;
#endif //SERV_COUNTRY_JP
#ifdef SERV_EVENT_SEAL_ITEM_UNIQUE// 작업날짜: 2013-05-08	// 박세훈
		case CXSLItem::IG_UNIQUE:	iSealItemID = CXSLItem::CI_EVENT_SEAL_ITEM_UNIQUE;	break;
#endif // SERV_EVENT_SEAL_ITEM_UNIQUE
		case CXSLItem::IG_ELITE:	iSealItemID = CXSLItem::CI_EVENT_SEAL_ITEM_ELITE;	break;
		case CXSLItem::IG_NORMAL:	iSealItemID = CXSLItem::CI_EVENT_SEAL_ITEM_NORMAL;	break;
		default:
			{
				START_LOG( cerr, L"선택한 아이템을 봉인할수 있는 봉인주문서가 아닙니다." )
					<< BUILD_LOG( iDestItemUID )
					<< BUILD_LOG( iSealItemUID )
					<< BUILD_LOG( kSealItemInfo.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( iSealItemID )
					<< END_LOG;

				SET_ERROR( ERR_SEAL_ITEM_10 );
				return false;
			}
		}

		if( kSealItemInfo.m_kItemInfo.m_iItemID != iSealItemID )
		{
			START_LOG( cerr, L"선택한 아이템을 봉인할수 있는 봉인주문서가 아닙니다." )
				<< BUILD_LOG( iDestItemUID )
				<< BUILD_LOG( iSealItemUID )
				<< BUILD_LOG( kSealItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOG( iSealItemID )
				<< END_LOG;

			SET_ERROR( ERR_SEAL_ITEM_10 );
			return false;
		}
#else
		START_LOG( cerr, L"선택한 아이템을 봉인할수 있는 봉인주문서가 아닙니다." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( iSealItemUID )
			<< BUILD_LOG( kSealItemInfo.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( iSealItemID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_10 );
		return false;
#endif SERV_COBO_SEAL_ITEM_EVENT
		//}}
#endif //SERV_RURIEL_EVENT_ITEM
	}

	// 주문서 수량 검사		
	if( kSealItemInfo.m_kItemInfo.m_iQuantity < CXSLItem::SEAL_ITEM_DECREASE_COUNT )
	{
		START_LOG( cerr, L"봉인 주문서가 부족하여 봉인할수 없습니다." )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_12 );
		return false;
	}

	// 봉인하기
	if( Seal( iDestItemUID ) == false )
	{
		START_LOG( cerr, L"봉인 실패! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_11 );
		return false;
	}

	// 봉인주문서를 지우자!
	int iDecreased = 0;
	DecreaseQuantity( iSealItemUID, CXSLItem::SEAL_ITEM_DECREASE_COUNT, iDecreased, KDeletedItemInfo::DR_SEAL );
	LIF( CXSLItem::SEAL_ITEM_DECREASE_COUNT == iDecreased );

	// 봉인 주문서 정보 얻기
	{
		KInventoryItemInfo kInventoryItemInfo;
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		GetInventorySlotInfo( kSealItemInfo.m_cSlotCategory, kSealItemInfo.m_sSlotID, kInventoryItemInfo );
#else
		GetInventorySlotInfo( kSealItemInfo.m_cSlotCategory, kSealItemInfo.m_cSlotID, kInventoryItemInfo );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		vecUpdated.push_back( kInventoryItemInfo );
	}

	// 봉인된 아이템 정보 얻기
	{
		KInventoryItemInfo kInventoryItemInfo;
		if( GetInventoryItemInfo( iDestItemUID, kInventoryItemInfo ) )
		{
			vecUpdated.push_back( kInventoryItemInfo );

			// 봉인 결과 얻기
			ucSealResult = kInventoryItemInfo.m_kItemInfo.m_ucSealData;
		}
		else
		{
			START_LOG( cerr, L"봉인 결과 얻기 실패!" ) 
				<< BUILD_LOG( iDestItemUID )
				<< END_LOG;
            
			SET_ERROR( ERR_SEAL_ITEM_11 );
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// 아이템 통계
	KStatisticsKey kKeyES;
	kKeyES.m_vecIntKey.push_back( iSealItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_USE, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	SET_ERROR( NET_OK );
	return true;
}

bool KInventory::UnsealItem( IN UidType iDestItemUID, OUT u_char& ucSealResult, OUT std::vector< KInventoryItemInfo >& vecUpdated )
{
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	KInventoryItemInfo kDestItemInfo;

	// 봉인 또는 봉인해제될 아이템 검사
	if( GetInventoryItemInfo( iDestItemUID, kDestItemInfo ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_00 );
		return false;
	}

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iDestItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는 아이템은 봉인 또는 봉인해제 못하는데?" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	// 템플릿 정보 얻기
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kDestItemInfo.m_kItemInfo.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( kDestItemInfo.m_kItemInfo.m_iItemID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_01 );
		return false;
	}

	// 장비 또는 아바타 아이템인지 확인
	if( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON  &&
		pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE  &&
		pItemTemplet->m_ItemType != CXSLItem::IT_ACCESSORY  &&
		//{{ 2012. 03. 03	박세훈	캐시펫구입시 결정체 1회 거래가능하도록 봉인된 상태로 나옴
#ifdef SERV_SEALED_CASH_PET_ITEM
		pItemTemplet->m_ItemType != CXSLItem::IT_SPECIAL  &&
#endif SERV_SEALED_CASH_PET_ITEM
		//}}
		pItemTemplet->m_bFashion == false )
	{
		START_LOG( cerr, L"장비 또는 악세사리 및 아바타 아이템이 아닙니다" )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_02 );
		return false;
	}

	// 장착중인 아이템인가?
	if( kDestItemInfo.m_cSlotCategory == CXSLInventory::ST_E_EQUIP )
	{
		START_LOG( cerr, L"장착 중인 아이템 봉인 및 봉인해제 시도." )
			<< BUILD_LOG( iDestItemUID )
			<< BUILD_LOGc( kDestItemInfo.m_cSlotCategory )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_13 );
		return false;
	}
	
	if( kDestItemInfo.m_kItemInfo.IsSealedItem() == false )
	{
		START_LOG( cerr, L"이미 봉인해제된 아이템을 봉인해제 하려 합니다!" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_14 );
		return false;
	}

	// 봉인해제하기
	if( Unseal( iDestItemUID ) == false )
	{
		START_LOG( cerr, L"봉인 해제 실패! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iDestItemUID )
			<< END_LOG;

		SET_ERROR( ERR_SEAL_ITEM_15 );
		return false;
	}	

	// 봉인 및 봉인해제된 아이템 정보 얻기
	{
		KInventoryItemInfo kInventoryItemInfo;
		if( GetInventoryItemInfo( iDestItemUID, kInventoryItemInfo ) )
		{
			vecUpdated.push_back( kInventoryItemInfo );

			// 봉인 결과 얻기
			ucSealResult = kInventoryItemInfo.m_kItemInfo.m_ucSealData;
		}
		else
		{
			START_LOG( cerr, L"봉인 결과 얻기 실패!" ) 
				<< BUILD_LOG( iDestItemUID )
				<< END_LOG;

			SET_ERROR( ERR_SEAL_ITEM_15 );
			return false;
		}
	}

	SET_ERROR( NET_OK );
	return true;
}
//}}

//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST

bool KInventory::UseGuildFoundationItemCheckOnly( IN UidType iItemUID )
{
	std::vector< KInventoryItemInfo > vecDummy;

    return UseGuildFoundationItem( iItemUID, vecDummy, true );
}

bool KInventory::UseGuildFoundationItem( IN UidType iItemUID, OUT std::vector< KInventoryItemInfo >& vecUpdated, IN bool bCheckOnly )
{
	vecUpdated.clear();

	SET_ERROR( ERR_UNKNOWN );

	KInventoryItemInfo kItemInfo;

	// 봉인 또는 봉인해제될 아이템 검사
	if( GetInventoryItemInfo( iItemUID, kItemInfo ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_02 );
		return false;
	}

	// 길드 생성 아이템인가?
	if( kItemInfo.m_kItemInfo.m_iItemID != CXSLItem::SI_GUILD_FOUNDATION )
	{
		START_LOG( cerr, L"길드 생성 아이템이 아니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_03 );
		return false;
	}	

	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는 아이템은 사용할수없다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	// 템플릿 정보 얻기
	if( bCheckOnly == false )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
				<< BUILD_LOG( kItemInfo.m_kItemInfo.m_iItemID )
				<< END_LOG;

			SET_ERROR( ERR_GUILD_03 );
			return false;
		}

		// 길드창단허가서를 지우자!
		KInventoryItemInfo kInventorySlotInfo;
		DeleteItem( iItemUID, kInventorySlotInfo, KDeletedItemInfo::DR_CREATE_GUILD );
		vecUpdated.push_back( kInventorySlotInfo );

		//////////////////////////////////////////////////////////////////////////
		// 아이템 통계
		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( kItemInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	SET_ERROR( NET_OK );
	return true;
}
#endif GUILD_TEST
//}}

//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
bool KInventory::ResetGuildSkillItemCheckOnly()
{
	// 스킬 초기화 아이템 검사
	std::vector< KInventoryItemInfo > vecResetSkillItem;
	GetInventoryItemInfoContainingThisItem( CXSLItem::CI_GUILD_SKILL_RESET_ITEM, vecResetSkillItem, true );
	if( vecResetSkillItem.empty() )
	{
#ifdef EVENT_GUILD_ITEM
		GetInventoryItemInfoContainingThisItem( CXSLItem::CI_EVENT_GUILD_SKILL_RESET_ITEM, vecResetSkillItem, true );
		if(vecResetSkillItem.empty())
		{
			return false;
		}
		else
		{
			return true;
		}
#else //EVENT_GUILD_ITEM
		// 스킬 1개 초기화 아이템없이 스킬 되돌리기를 하려고 한 경우
		return false;
#endif //EVENT_GUILD_ITEM
	}

	return true;
}

bool KInventory::ResetGuildSkillItem( OUT std::vector< KInventoryItemInfo >& vecInventorySlotnfo )
{
	vecInventorySlotnfo.clear();

	// 스킬 초기화 아이템 검사
	std::vector< KInventoryItemInfo > vecResetSkillItem;
	GetInventoryItemInfoContainingThisItem( CXSLItem::CI_GUILD_SKILL_RESET_ITEM, vecResetSkillItem, true );
	if( vecResetSkillItem.empty() )
	{
#ifdef EVENT_GUILD_ITEM
		GetInventoryItemInfoContainingThisItem( CXSLItem::CI_EVENT_GUILD_SKILL_RESET_ITEM, vecResetSkillItem, true );
		if(vecResetSkillItem.empty())
		{
			return false;
		}
#else //EVENT_GUILD_ITEM
		// 스킬 1개 초기화 아이템없이 스킬 되돌리기를 하려고 한 경우
		return false;
#endif //EVENT_GUILD_ITEM
	}

	UidType iItemUID = 0;
	int iSlotCategory = 0, iSlotID = 0;
	std::vector< KInventoryItemInfo >::const_iterator vitRSI;
	for( vitRSI = vecResetSkillItem.begin(); vitRSI != vecResetSkillItem.end(); ++vitRSI )
	{
		// 스킬 초기화 아이템 UID 한개만 얻음.
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		iItemUID = GetItemUID( vitRSI->m_cSlotCategory, vitRSI->m_sSlotID );
#else
		iItemUID = GetItemUID( vitRSI->m_cSlotCategory, vitRSI->m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}

		if( iItemUID <= 0 )
		{
			START_LOG( cerr, L"인벤에서 길드 스킬 초기화 아이템을 찾는중 이상한 아이템UID 발견." )
				<< BUILD_LOGc( vitRSI->m_cSlotCategory )
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				<< BUILD_LOG( vitRSI->m_sSlotID )
#else
				<< BUILD_LOGc( vitRSI->m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		iSlotCategory = vitRSI->m_cSlotCategory;
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		iSlotID		  = vitRSI->m_sSlotID;
#else
		iSlotID		  = vitRSI->m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		break;
	}

	if( iItemUID == 0 )
		return false;

	KInventoryItemInfo kInventorySlotInfo;
	int iDecreased = 0;
	DecreaseQuantity( iItemUID, 1, iDecreased, KDeletedItemInfo::DR_RESET_GUILD_SKILL );
	LIF( iDecreased == 1 );
	GetInventorySlotInfo( iSlotCategory, iSlotID, kInventorySlotInfo );
	vecInventorySlotnfo.push_back( kInventorySlotInfo );
	return true;
}

bool KInventory::InitGuildSkillItemCheckOnly( IN UidType iItemUID )
{
	// 인벤토리에 길드 스킬 초기화 아이템이 있는지 검사
	if( IsExist( iItemUID ) == false )
	{
		START_LOG( cerr, L"길드스킬초기화 아이템이 없는데 패킷 날아옴.!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	const int iItemID = GetItemID( iItemUID );

	if( iItemID != CXSLItem::CI_GUILD_SKILL_INIT_ITEM )
	{
		START_LOG( cerr, L"스킬초기화 아이템ID가 이상함." )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		
		return false;
	}

	return true;
}

bool KInventory::InitGuildSkillItem( IN UidType iItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotnfo )
{
	vecInventorySlotnfo.clear();

	// 인벤토리에 길드 스킬 초기화 아이템이 있는지 검사
	if( IsExist( iItemUID ) == false )
	{
		START_LOG( cerr, L"길드스킬초기화 아이템이 없는데 패킷 날아옴.!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	// 길드 스킬 초기화 아이템을 삭제 처리!
	KInventoryItemInfo kInventorySlotInfo;
	int iSlotCategory = 0;
	int iSlotID = 0;
	int iDecreased = 0;
	if( GetCategorySlot( iItemUID, iSlotCategory, iSlotID ) == false )
	{
		START_LOG( cerr, L"길드스킬초기화 아이템이 없는데 패킷 날아옴.!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	DecreaseQuantity( iItemUID, 1, iDecreased, KDeletedItemInfo::DR_INIT_GUILD_SKILL );
	LIF( iDecreased == 1 );
	GetInventorySlotInfo( iSlotCategory, iSlotID, kInventorySlotInfo );
	vecInventorySlotnfo.push_back( kInventorySlotInfo );
    return true;
}

#endif GUILD_SKILL_TEST
//}}


//{{ 2011. 01. 06  김민성  스킬슬롯체인지 체크(인벤토리-기간제) 기능 구현
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY

bool KInventory::GetSlotChangeBEndDate( IN const int iItemID, IN OUT std::wstring& wstrExpirationDate )
{
	std::wstring wstrCahsItemDate;
	std::wstring wstrInventoryItemDate;

	CTime tCashItemTime;
	CTime tInventoryItemTime;
	CTime tNowTime = CTime::GetCurrentTime(); // 초기 값

	bool bIsExist = false;

	if( !KncUtil::ConvertStringToCTime( wstrExpirationDate, tCashItemTime ) )
	{
		if( !wstrExpirationDate.empty() )
		{
			START_LOG( cerr, L"문자열 시간 변환 실패." )
				<< BUILD_LOG( wstrExpirationDate )
				<< END_LOG;
			return false;
		}
	}

	if( tCashItemTime > tNowTime )	// 캐쉬 스킬슬롯이 존재
	{
		// 캐쉬 아이템 시간이 더 큰경우 현재 시간을 덮어씌운다.
		tNowTime = tCashItemTime;
	}

	std::map< UidType, KInventoryItem >::const_iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		if( mit->second.GetItemID() == iItemID )
		{
			mit->second.GetExpirationDate( wstrInventoryItemDate );

			if( !KncUtil::ConvertStringToCTime( wstrInventoryItemDate, tInventoryItemTime ) )
			{
				START_LOG( cerr, L"문자열 시간 변환 실패. 해당 아이템은 반드시 기간이 있어야한다. 아이템 정보가 잘못됐나?" )
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( wstrInventoryItemDate )
					<< END_LOG;
				return false;
			}

			if( tInventoryItemTime > tNowTime )				// 기간이 지금보다 더 남았으면 
			{
				wstrExpirationDate = wstrInventoryItemDate;
				tNowTime = tInventoryItemTime;

				bIsExist = true;
			}
		}
	}
	
	return  bIsExist ;
}

#endif SERV_SKILL_SLOT_CHANGE_INVENTORY

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
bool KInventory::GiveRetainingRewardItem( IN const KRewardItem& kItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem )
{
	if( kItem.m_iItemID <= 0 || kItem.m_iQuantity <= 0 )
	{
		START_LOG( cerr, L"이탈방지 보상 아이템 정보가 이상합니다." )
			<< BUILD_LOG( kItem.m_iItemID )
			<< BUILD_LOG( kItem.m_iQuantity )
			<< END_LOG;

		return false;
	}

	std::map< int, KItemInfo > mapResultItem;  // 결과템

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kItem.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
			<< BUILD_LOG( kItem.m_iItemID )
			<< END_LOG;

		return false;
	}
	else
	{
		KItemInfo kInsertItemInfo;
		kInsertItemInfo.m_iItemID	 = kItem.m_iItemID;
		kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
		kInsertItemInfo.m_iQuantity	 = kItem.m_iQuantity;
		kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
		mapResultItem.insert( std::make_pair( kItem.m_iItemID, kInsertItemInfo ) );
	}
	
	// 4. 결과 처리
	std::map< int, int > mapDummy;

	if( DeleteAndInsert( mapDummy, mapResultItem, vecUpdated, vecNewItem, true, KDeletedItemInfo::DR_NO_REASON ) == false )
	{
		return false;
	}

	// 아이템 통계
	KStatisticsKey kReward;
	kReward.m_vecIntKey.push_back( kItem.m_iItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kReward, KStatistics::SI_ITEM_RETRANINING_REWARD, 1 );
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kReward, KStatistics::SI_ITEM_RETRANINING_REWARD, 1 );

	return true;    
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}}

//{{ 2011. 09. 26  임규수 칭호 수리비 감소 속성 적용
#ifdef SERV_TITLE_REPAIR_DISCOUNT
float KInventory::GetRepairTitleDiscount( IN const int iTitleID )
{
	float fTotalDiscount = 0.0f;

	if( iTitleID == 0 )
	{
		START_LOG( clog, L"타이틀 미장착" )
			<< BUILD_LOG( iTitleID )
			<< END_LOG;
		return fTotalDiscount;
	}

	const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( iTitleID );
	if( pTitleInfo == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 타이틀을 장착중?" )
			<< BUILD_LOG( iTitleID )
			<< END_LOG;
		return fTotalDiscount;
	}

	BOOST_TEST_FOREACH( const int, iSocketOption, pTitleInfo->m_vecSocketOption )
	{
		const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( iSocketOption );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"존재하지않는 소켓옵션이 타이틀의 소켓옵션으로 세팅되어 있습니다." )
				<< BUILD_LOG( iTitleID )
				<< BUILD_LOG( iSocketOption )
				<< END_LOG;
			continue;
		}

		if( pSocketData->m_fRepairPriceSale > 0.0f )
			fTotalDiscount += pSocketData->m_fRepairPriceSale;
	}

	return fTotalDiscount;
}
#endif SERV_TITLE_REPAIR_DISCOUNT
//}} 

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
void KInventory::SetClassChangeItem( IN std::map< UidType, int >& mapItem, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	std::map< UidType, KInventoryItem >::iterator mymit;
	KInventoryItem kTempItem;

	std::map< UidType, int >::iterator mit = mapItem.begin();
	for(  ; mit != mapItem.end() ; ++mit )
	{
		mymit = m_mapItem.find( mit->first );
		if( mymit != m_mapItem.end() )
		{
			kTempItem = mymit->second;
			kTempItem.SetItemID( mit->second );
			mymit->second = kTempItem;

			KInventoryItemInfo kTempInventoryItemInfo;
			mymit->second.GetInventoryItemInfo( kTempInventoryItemInfo );
			vecInventorySlotInfo.push_back( kTempInventoryItemInfo );
		}
		else
		{
			START_LOG( cerr, L"이런 아이템을 가지고 있지 않다고?! 있다고 해서 기록한거잖아!!" )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second )
				<< END_LOG;
		}
	}
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
//{{ 2012. 02. 27	김민성	인벤토리 정렬	- 무거운 기능을 가볍게 변경
#ifdef SERV_SORT_CATEGORY_ITEM_FIX
#ifdef SERV_PERSONAL_SHOP_NO_MOVE
bool KInventory::SortCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot, OUT int& iNetError, IN const std::vector< KSellPersonalShopItemInfo > vecPersonalShopSell_ItemInfo )
#else 
bool KInventory::SortCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot, OUT int& iNetError )
#endif SERV_PERSONAL_SHOP_NO_MOVE
{
	iNetError = NetError::NET_OK;

	if(iCategory < CXSLInventory::ST_EQUIP || iCategory > CXSLInventory::ST_AVARTA)
	{
		if( iCategory != CXSLInventory::ST_PC_BANG )
		{
			START_LOG( cerr, L"이상한 카테고리입니다!" )
				<< BUILD_LOG( iCategory )
				<< END_LOG;

			iNetError = NetError::ERR_UNKNOWN;

			return false;
		}
	}

	switch(iCategory)
	{
	case CXSLInventory::ST_EQUIP:		// [1]  장비
	case CXSLInventory::ST_ACCESSORY:	// [2]  악세사리
	case CXSLInventory::ST_AVARTA:		// [7]  아바타
	case CXSLInventory::ST_PC_BANG:		// [12] PC방 전용 인벤
		{
			SortEquipCategory( iCategory, vecUpdatedInventorySlot );
		} break;
	case CXSLInventory::ST_MATERIAL:	// [3]  재료
	case CXSLInventory::ST_SPECIAL:		// [4]  특수
	case CXSLInventory::ST_QUEST:		// [5]  퀘스트
		{
			SortNormalCategory( iCategory, vecUpdatedInventorySlot );
		} break;
	case CXSLInventory::ST_QUICK_SLOT:	// [6]  소비
		{
			SortConsumptionCategory( iCategory, vecUpdatedInventorySlot );
		} break;
	}

	return true;
}

void KInventory::SortEquipCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot )
{
	// iCategory 에 맞는 아이템만 추출하기 위한 컨테이너
	std::multimap< int, KInventoryItemSortInfo, std::greater<int> > multimapInCategoryItem;
	std::multimap< int, KInventoryItemSortInfo, std::greater<int> >::iterator mmitInCategoryItem;

	// iCategory 에 맞는 아이템 중 만 장착 위치별 아이템을 추출하기 위한 컨테이너
	std::multimap< int, KInventoryItemSortInfo > multimapEquipTypeItem;
	std::multimap< int, KInventoryItemSortInfo >::iterator mmitEquipTypeItem;

	// iCategory 에 맞는 아이템 중 만 셋트 아이템을 추출하기 위한 컨테이너
	std::multimap< int, KInventoryItemSortInfo, std::greater<int> > multimapSetItem;
	std::multimap< int, KInventoryItemSortInfo, std::greater<int> >::iterator mmitSetItem;

	// iCategory 에 맞는 아이템 중 만 등급별 아이템을 추출하기 위한 컨테이너
	std::multimap< int, KInventoryItemSortInfo > multimapGradeItem;
	std::multimap< int, KInventoryItemSortInfo >::iterator mmitGradeItem;

	// iCategory 에 맞는 아이템 중 만 사용 레벨별 아이템을 추출하기 위한 컨테이너
	std::multimap< int, KInventoryItemSortInfo, std::greater<int> > multimapUseLevelItem;
	std::multimap< int, KInventoryItemSortInfo, std::greater<int> >::iterator mmitUseLevelItem;

	// iCategory 에 맞는 아이템 중 만 유닛 타입별 아이템을 추출하기 위한 컨테이너
	std::multimap< int, KInventoryItemSortInfo > multimapUnitTypeItem;
	std::multimap< int, KInventoryItemSortInfo >::iterator mmitUnitTypeItem;

	std::map< UidType, KInventoryItem >::iterator mit;

	// 해당 카테고리 아이템 추출겸 아이템 ID 역순 정렬
	for( mit = m_mapItem.begin() ; mit != m_mapItem.end() ; ++mit )
	{
		if( mit->second.GetInventoryCategory() == iCategory )
		{
			KInventoryItemSortInfo kInventoryItemSortInfo_;
			kInventoryItemSortInfo_ = std::make_pair( mit->second.GetItemID(), mit->second.GetItemUID() );
			multimapInCategoryItem.insert(std::make_pair( mit->second.GetItemID(), kInventoryItemSortInfo_ ));
		}
	}

	// 사용 레벨별 정렬
	for(mmitInCategoryItem = multimapInCategoryItem.begin(); mmitInCategoryItem != multimapInCategoryItem.end(); ++mmitInCategoryItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitInCategoryItem->second.first );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitInCategoryItem->second.first;

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		multimapUseLevelItem.insert(std::make_pair(pItemTemplet->m_UseLevel, mmitInCategoryItem->second));
	}

	// 장착 위치별 정렬
	for(mmitUseLevelItem = multimapUseLevelItem.begin(); mmitUseLevelItem != multimapUseLevelItem.end(); ++mmitUseLevelItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitUseLevelItem->second.first );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitUseLevelItem->second.first;

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		multimapEquipTypeItem.insert(std::make_pair(pItemTemplet->m_EqipPosition, mmitUseLevelItem->second));
	}

	// 세트 ID별 정렬
	for(mmitEquipTypeItem = multimapEquipTypeItem.begin(); mmitEquipTypeItem != multimapEquipTypeItem.end(); ++mmitEquipTypeItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitEquipTypeItem->second.first );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitEquipTypeItem->second.first;

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		multimapSetItem.insert(std::make_pair(pItemTemplet->m_SetID, mmitEquipTypeItem->second));
	}

	// 아이템 등급별 정렬
	for( mmitSetItem = multimapSetItem.begin() ; mmitSetItem != multimapSetItem.end() ; ++mmitSetItem )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitSetItem->second.first );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitSetItem->second.first;

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		if(pItemTemplet->m_ItemGrade == CXSLItem::IG_NONE)
		{
			multimapGradeItem.insert(std::make_pair(CXSLItem::IG_LOW + 1, mmitSetItem->second));
		}
		else
		{
			multimapGradeItem.insert(std::make_pair(pItemTemplet->m_ItemGrade, mmitSetItem->second));
		}
	}

	// 자기가 장착 가능한 장비 우선 나머지는 유닛 타입별 정렬
	for(mmitGradeItem = multimapGradeItem.begin(); mmitGradeItem != multimapGradeItem.end(); ++mmitGradeItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitGradeItem->second.first );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitGradeItem->second.first;

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}
	
		if(CompareUnitClass( pItemTemplet->m_ItemID ) == true ) // 자신의 장비이다
		{
			if( CompareLevel( pItemTemplet->m_ItemID ) == true ) // 장착 가능 레벨이다
			{
				if( pItemTemplet->m_UseCondition != CXSLItem::UC_ANYONE )
				{
					multimapUnitTypeItem.insert(std::make_pair(0, mmitGradeItem->second));
				}
				else
				{
					multimapUnitTypeItem.insert(std::make_pair(1, mmitGradeItem->second));
				}
			}
			else	// 자신의 장비이지만 장착 가능 레벨이 안된다
			{
				if( pItemTemplet->m_UseCondition != CXSLItem::UC_ANYONE )
				{
					multimapUnitTypeItem.insert(std::make_pair(3, mmitGradeItem->second));
				}
				else
				{
					multimapUnitTypeItem.insert(std::make_pair(4, mmitGradeItem->second));
				}
			}
		}
		else // 자신의 장비가 아니다
		{
			multimapUnitTypeItem.insert(std::make_pair(pItemTemplet->m_UnitType * 10, mmitGradeItem->second));
		}

// 원본 유지용
// 		if(CompareUnitClass( pItemTemplet->m_ItemID ) && CompareLevel( pItemTemplet->m_ItemID ))
// 		{
// 			if( pItemTemplet->m_UseCondition != CXSLItem::UC_ANYONE )
// 			{
// 				multimapUnitTypeItem.insert(std::make_pair(0, mmitGradeItem->second));
// 			}
// 			else
// 			{
// 				multimapUnitTypeItem.insert(std::make_pair(1, mmitGradeItem->second));
// 			}
// 		}
// 		else
// 		{
// 			multimapUnitTypeItem.insert(std::make_pair(pItemTemplet->m_UnitType * 10, mmitGradeItem->second));
// 		}
	}

	if(multimapInCategoryItem.size() != multimapUnitTypeItem.size())
	{
		return ;
	}

	// 실제 서버쪽 데이터 갱신하는 부분
	const int nInventorySlotSize = GetSlotSize( iCategory );
	int nSlotID = 0;

	m_vecInventorySlot[iCategory].clear();
	m_vecInventorySlot[iCategory].insert(m_vecInventorySlot[iCategory].end(), nInventorySlotSize, 0);

	vecUpdatedInventorySlot.clear();

	for(mmitUnitTypeItem = multimapUnitTypeItem.begin(); mmitUnitTypeItem != multimapUnitTypeItem.end(); ++mmitUnitTypeItem)
	{
		m_vecInventorySlot[iCategory].at(nSlotID) = mmitUnitTypeItem->second.second;
		mit = m_mapItem.find(mmitUnitTypeItem->second.second);

		if(mit != m_mapItem.end())
		{
			mit->second.SetPosition(iCategory, nSlotID);
		}

		nSlotID++;
	}

	// 클라이언트로 보낼 데이터
	vecUpdatedInventorySlot = m_vecInventorySlot[iCategory];
}

void KInventory::SortConsumptionCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot )
{
	std::multimap< int, KInventoryItemSortInfo > multimapSortItem;
	std::multimap< int, KInventoryItemSortInfo >::iterator mmitSortItem;

	std::map< UidType, KInventoryItem >::iterator mit;

	// ItemID로 우선 정렬
	for(mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit)
	{
		if(mit->second.GetInventoryCategory() == iCategory)
		{
			KInventoryItemSortInfo kInventoryItemSortInfo_;
			kInventoryItemSortInfo_ = std::make_pair( mit->second.GetItemID(), mit->second.GetItemUID() );
			multimapSortItem.insert( std::make_pair( mit->second.GetItemID(), kInventoryItemSortInfo_ ) );			
		}
	}

	// 실제 서버쪽 데이터 갱신하는 부분
	const int nInventorySlotSize = GetSlotSize( iCategory );
	int nSlotID = 0;

	m_vecInventorySlot[iCategory].clear();
	m_vecInventorySlot[iCategory].insert(m_vecInventorySlot[iCategory].end(), nInventorySlotSize, 0);
	vecUpdatedInventorySlot.clear();

	for(mmitSortItem = multimapSortItem.begin(); mmitSortItem != multimapSortItem.end(); ++mmitSortItem)
	{
		m_vecInventorySlot[iCategory].at(nSlotID) = mmitSortItem->second.second;
		mit = m_mapItem.find(mmitSortItem->second.second);

		if(mit != m_mapItem.end())
		{
			mit->second.SetPosition(iCategory, nSlotID);
		}
		nSlotID++;
	}

	// 클라이언트로 보낼 데이터
	vecUpdatedInventorySlot = m_vecInventorySlot[iCategory];
}

void KInventory::SortNormalCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot )
{
	std::multimap< int, KInventoryItemSortInfo, std::greater<int>  > multimapInCategoryItem;
	std::multimap< int, KInventoryItemSortInfo, std::greater<int>  >::iterator mmitInCategoryItem;

	std::multimap< int, KInventoryItemSortInfo, std::greater<int> > multimapUseLevelItem;
	std::multimap< int, KInventoryItemSortInfo, std::greater<int> >::iterator mmitUseLevelItem;

	std::multimap< int, KInventoryItemSortInfo > multimapGradeItem;
	std::multimap< int, KInventoryItemSortInfo >::iterator mmitGradeItem;

	std::multimap< int, KInventoryItemSortInfo, std::greater<int> > multimapItem;
	std::multimap< int, KInventoryItemSortInfo, std::greater<int> >::iterator mmitItem;

	std::map< UidType, KInventoryItem >::iterator mit;

	// 해당 카테고리 아이템 추출
	for(mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit)
	{
		if(mit->second.GetInventoryCategory() == iCategory)
		{
			KInventoryItemSortInfo kInventoryItemSortInfo_;
			kInventoryItemSortInfo_ = std::make_pair( mit->second.GetItemID(), mit->second.GetItemUID() );
			multimapInCategoryItem.insert(std::make_pair(mit->second.GetItemID(), kInventoryItemSortInfo_));
			
		}
	}

	// 사용 레벨로 정렬
	for(mmitInCategoryItem = multimapInCategoryItem.begin(); mmitInCategoryItem != multimapInCategoryItem.end(); ++mmitInCategoryItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitInCategoryItem->second.first );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitInCategoryItem->second.first;

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		multimapUseLevelItem.insert(std::make_pair(pItemTemplet->m_UseLevel, mmitInCategoryItem->second));
	}

	// 아이템 ID 역순 정렬
	for(mmitUseLevelItem = multimapUseLevelItem.begin(); mmitUseLevelItem != multimapUseLevelItem.end(); ++mmitUseLevelItem)
	{
		multimapItem.insert(std::make_pair(mmitUseLevelItem->second.first, mmitUseLevelItem->second));
	}

	// 아이템 등급별 정렬
	for(mmitItem = multimapItem.begin(); mmitItem != multimapItem.end(); ++mmitItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitItem->second.first );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitItem->second.first;

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		if(pItemTemplet->m_ItemGrade == CXSLItem::IG_NONE)
		{
			multimapGradeItem.insert(std::make_pair(CXSLItem::IG_LOW + 1, mmitItem->second));
		}
		else
		{
			multimapGradeItem.insert(std::make_pair(pItemTemplet->m_ItemGrade, mmitItem->second));
		}
	}

	if(multimapInCategoryItem.size() != multimapGradeItem.size())
	{
		return ;
	}

	// 실제 서버쪽 데이터 갱신하는 부분
	const int nInventorySlotSize = GetSlotSize( iCategory );
	int nSlotID = 0;

	m_vecInventorySlot[iCategory].clear();
	m_vecInventorySlot[iCategory].insert(m_vecInventorySlot[iCategory].end(), nInventorySlotSize, 0);
	vecUpdatedInventorySlot.clear();

	for(mmitGradeItem = multimapGradeItem.begin(); mmitGradeItem != multimapGradeItem.end(); ++mmitGradeItem)
	{
		m_vecInventorySlot[iCategory].at(nSlotID) = mmitGradeItem->second.second;
		mit = m_mapItem.find(mmitGradeItem->second.second);

		if(mit != m_mapItem.end())
		{
			mit->second.SetPosition(iCategory, nSlotID);
		}
		nSlotID++;
	}

	// 클라이언트로 보낼 데이터
	vecUpdatedInventorySlot = m_vecInventorySlot[iCategory];
}
#else
bool KInventory::SortCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot, OUT int& iNetError )
{
	iNetError = NetError::NET_OK;

	if(iCategory < CXSLInventory::ST_EQUIP || iCategory > CXSLInventory::ST_AVARTA)
	{
		if( iCategory != CXSLInventory::ST_PC_BANG )
		{
			START_LOG( cerr, L"이상한 카테고리입니다!" )
				<< BUILD_LOG( iCategory )
				<< END_LOG;

			iNetError = NetError::ERR_UNKNOWN;

			return false;
		}
	}

	switch( iCategory )
	{
	case CXSLInventory::ST_EQUIP:		// [1]  장비
	case CXSLInventory::ST_ACCESSORY:	// [2]  악세사리
	case CXSLInventory::ST_AVARTA:		// [7]  아바타
	case CXSLInventory::ST_PC_BANG:		// [12] PC방 전용 인벤
		{
			SortEquipCategory( iCategory, vecUpdatedInventorySlot );
		} break;
	case CXSLInventory::ST_MATERIAL:	// [3]  재료
	case CXSLInventory::ST_SPECIAL:		// [4]  특수
	case CXSLInventory::ST_QUEST:		// [5]  퀘스트
		{
			SortNormalCategory( iCategory, vecUpdatedInventorySlot );
		} break;
	case CXSLInventory::ST_QUICK_SLOT:	// [6]  소비
		{
			SortConsumptionCategory( iCategory, vecUpdatedInventorySlot );
		} break;
	}

	return true;
}

void KInventory::SortEquipCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot )
{
	std::multimap< int, KInventoryItem, std::greater<int> > multimapInCategoryItem;
	std::multimap< int, KInventoryItem, std::greater<int> >::iterator mmitInCategoryItem;

	std::multimap< int, KInventoryItem > multimapEquipTypeItem;
	std::multimap< int, KInventoryItem >::iterator mmitEquipTypeItem;

	std::multimap< int, KInventoryItem, std::greater<int> > multimapSetItem;
	std::multimap< int, KInventoryItem, std::greater<int> >::iterator mmitSetItem;

	std::multimap< int, KInventoryItem > multimapGradeItem;
	std::multimap< int, KInventoryItem >::iterator mmitGradeItem;

	std::multimap< int, KInventoryItem, std::greater<int> > multimapUseLevelItem;
	std::multimap< int, KInventoryItem, std::greater<int> >::iterator mmitUseLevelItem;

	std::multimap< int, KInventoryItem > multimapUnitTypeItem;
	std::multimap< int, KInventoryItem >::iterator mmitUnitTypeItem;

	std::map< UidType, KInventoryItem >::iterator mit;

	// 해당 카테고리 아이템 추출겸 아이템 ID 역순 정렬
	for(mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit)
	{
		if(mit->second.GetInventoryCategory() == iCategory)
		{
			multimapInCategoryItem.insert(std::make_pair(mit->second.GetItemID(), mit->second));
		}
	}

	// 아이템 등급별 정렬
	for(mmitInCategoryItem = multimapInCategoryItem.begin(); mmitInCategoryItem != multimapInCategoryItem.end(); ++mmitInCategoryItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitInCategoryItem->second.GetItemID() );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitInCategoryItem->second.GetItemID();

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		if(pItemTemplet->m_ItemGrade == CXSLItem::IG_NONE)
		{
			multimapGradeItem.insert(std::make_pair(CXSLItem::IG_LOW + 1, mmitInCategoryItem->second));
		}
		else
		{
			multimapGradeItem.insert(std::make_pair(pItemTemplet->m_ItemGrade, mmitInCategoryItem->second));
		}
	}

	// 사용 레벨별 정렬
	for(mmitGradeItem = multimapGradeItem.begin(); mmitGradeItem != multimapGradeItem.end(); ++mmitGradeItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitGradeItem->second.GetItemID() );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitGradeItem->second.GetItemID();

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		multimapUseLevelItem.insert(std::make_pair(pItemTemplet->m_UseLevel, mmitGradeItem->second));
	}

	// 장착 위치별 정렬
	for(mmitUseLevelItem = multimapUseLevelItem.begin(); mmitUseLevelItem != multimapUseLevelItem.end(); ++mmitUseLevelItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitUseLevelItem->second.GetItemID() );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitUseLevelItem->second.GetItemID();

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		multimapEquipTypeItem.insert(std::make_pair(pItemTemplet->m_EqipPosition, mmitUseLevelItem->second));
	}

	// 세트 ID별 정렬
	for(mmitEquipTypeItem = multimapEquipTypeItem.begin(); mmitEquipTypeItem != multimapEquipTypeItem.end(); ++mmitEquipTypeItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitEquipTypeItem->second.GetItemID() );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitEquipTypeItem->second.GetItemID();

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		multimapSetItem.insert(std::make_pair(pItemTemplet->m_SetID, mmitEquipTypeItem->second));
	}

	// 자기가 장착 가능한 장비 우선 나머지는 유닛 타입별 정렬
	for(mmitSetItem = multimapSetItem.begin(); mmitSetItem != multimapSetItem.end(); ++mmitSetItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitSetItem->second.GetItemID() );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitSetItem->second.GetItemID();

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		if(CompareUnitClass( pItemTemplet->m_ItemID ) && CompareLevel( pItemTemplet->m_ItemID ))
		{
			if( pItemTemplet->m_UseCondition != CXSLItem::UC_ANYONE )
			{
				multimapUnitTypeItem.insert(std::make_pair(0, mmitSetItem->second));
			}
			else
			{
				multimapUnitTypeItem.insert(std::make_pair(1, mmitSetItem->second));
			}
		}
		else
		{
			multimapUnitTypeItem.insert(std::make_pair(pItemTemplet->m_UnitType * 10, mmitSetItem->second));
		}
	}

	if(multimapInCategoryItem.size() != multimapUnitTypeItem.size())
	{
		return ;
	}

	// 실제 서버쪽 데이터 갱신하는 부분
	const int nInventorySlotSize = GetSlotSize( iCategory );
	int nSlotID = 0;

	m_vecInventorySlot[iCategory].clear();
	m_vecInventorySlot[iCategory].insert(m_vecInventorySlot[iCategory].end(), nInventorySlotSize, 0);

	vecUpdatedInventorySlot.clear();

	for(mmitUnitTypeItem = multimapUnitTypeItem.begin(); mmitUnitTypeItem != multimapUnitTypeItem.end(); ++mmitUnitTypeItem)
	{
		m_vecInventorySlot[iCategory].at(nSlotID) = mmitUnitTypeItem->second.GetItemUID();
		mit = m_mapItem.find(mmitUnitTypeItem->second.GetItemUID());

		if(mit != m_mapItem.end())
		{
			mit->second.SetPosition(iCategory, nSlotID);
		}

		nSlotID++;
	}

	// 클라이언트로 보낼 데이터
	vecUpdatedInventorySlot = m_vecInventorySlot[iCategory];
}

void KInventory::SortConsumptionCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot )
{
	std::multimap< int, KInventoryItem > multimapSortItem;
	std::multimap< int, KInventoryItem >::iterator mmitSortItem;

	std::map< UidType, KInventoryItem >::iterator mit;

	// ItemID로 우선 정렬
	for(mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit)
	{
		if(mit->second.GetInventoryCategory() == iCategory)
		{
			multimapSortItem.insert(std::make_pair(mit->second.GetItemID(), mit->second));
		}
	}

	// 실제 서버쪽 데이터 갱신하는 부분
	const int nInventorySlotSize = GetSlotSize( iCategory );
	int nSlotID = 0;

	m_vecInventorySlot[iCategory].clear();
	m_vecInventorySlot[iCategory].insert(m_vecInventorySlot[iCategory].end(), nInventorySlotSize, 0);
	vecUpdatedInventorySlot.clear();

	for(mmitSortItem = multimapSortItem.begin(); mmitSortItem != multimapSortItem.end(); ++mmitSortItem)
	{
		m_vecInventorySlot[iCategory].at(nSlotID) = mmitSortItem->second.GetItemUID();
		mit = m_mapItem.find(mmitSortItem->second.GetItemUID());

		if(mit != m_mapItem.end())
		{
			mit->second.SetPosition(iCategory, nSlotID);
		}
		nSlotID++;
	}

	// 클라이언트로 보낼 데이터
	vecUpdatedInventorySlot = m_vecInventorySlot[iCategory];
}

void KInventory::SortNormalCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot )
{
	std::multimap< int, KInventoryItem, std::greater<int>  > multimapInCategoryItem;
	std::multimap< int, KInventoryItem, std::greater<int>  >::iterator mmitInCategoryItem;

	std::multimap< int, KInventoryItem, std::greater<int> > multimapUseLevelItem;
	std::multimap< int, KInventoryItem, std::greater<int> >::iterator mmitUseLevelItem;

	std::multimap< int, KInventoryItem > multimapGradeItem;
	std::multimap< int, KInventoryItem >::iterator mmitGradeItem;

	std::multimap< int, KInventoryItem, std::greater<int> > multimapItem;
	std::multimap< int, KInventoryItem, std::greater<int> >::iterator mmitItem;

	std::map< UidType, KInventoryItem >::iterator mit;

	// 해당 카테고리 아이템 추출
	for(mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit)
	{
		if(mit->second.GetInventoryCategory() == iCategory)
		{
			multimapInCategoryItem.insert(std::make_pair(mit->second.GetItemID(), mit->second));
		}
	}

	// 사용 레벨로 정렬
	for(mmitInCategoryItem = multimapInCategoryItem.begin(); mmitInCategoryItem != multimapInCategoryItem.end(); ++mmitInCategoryItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitInCategoryItem->second.GetItemID() );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitInCategoryItem->second.GetItemID();

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		multimapUseLevelItem.insert(std::make_pair(pItemTemplet->m_UseLevel, mmitInCategoryItem->second));
	}

	// 아이템 등급별 정렬
	for(mmitUseLevelItem = multimapUseLevelItem.begin(); mmitUseLevelItem != multimapUseLevelItem.end(); ++mmitUseLevelItem)
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mmitUseLevelItem->second.GetItemID() );
		if( pItemTemplet == NULL )
		{
			UidType iUnitUID = m_pUser->GetCharUID();
			int nItemID = mmitUseLevelItem->second.GetItemID();

			START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( nItemID )
				<< END_LOG;

			continue;
		}

		if(pItemTemplet->m_ItemGrade == CXSLItem::IG_NONE)
		{
			multimapGradeItem.insert(std::make_pair(CXSLItem::IG_LOW + 1, mmitUseLevelItem->second));
		}
		else
		{
			multimapGradeItem.insert(std::make_pair(pItemTemplet->m_ItemGrade, mmitUseLevelItem->second));
		}
	}

	// 아이템 ID 역순 정렬
	for(mmitGradeItem = multimapGradeItem.begin(); mmitGradeItem != multimapGradeItem.end(); ++mmitGradeItem)
	{
		multimapItem.insert(std::make_pair(mmitGradeItem->second.GetItemID(), mmitGradeItem->second));
	}

	if(multimapInCategoryItem.size() != multimapItem.size())
	{
		return ;
	}

	// 실제 서버쪽 데이터 갱신하는 부분
	const int nInventorySlotSize = GetSlotSize( iCategory );
	int nSlotID = 0;

	m_vecInventorySlot[iCategory].clear();
	m_vecInventorySlot[iCategory].insert(m_vecInventorySlot[iCategory].end(), nInventorySlotSize, 0);
	vecUpdatedInventorySlot.clear();

	for(mmitItem = multimapItem.begin(); mmitItem != multimapItem.end(); ++mmitItem)
	{
		m_vecInventorySlot[iCategory].at(nSlotID) = mmitItem->second.GetItemUID();
		mit = m_mapItem.find(mmitItem->second.GetItemUID());

		if(mit != m_mapItem.end())
		{
			mit->second.SetPosition(iCategory, nSlotID);
		}
		nSlotID++;
	}

	// 클라이언트로 보낼 데이터
	vecUpdatedInventorySlot = m_vecInventorySlot[iCategory];
}
#endif SERV_SORT_CATEGORY_ITEM_FIX
//}}
#endif SERV_SORT_CATEGORY_ITEM 
//}}

//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
bool KInventory::CheckPetToyItem( void )
{
	BOOST_TEST_FOREACH( UidType, iItemUID, m_vecInventorySlot[CXSLInventory::ST_SPECIAL] )
	{
		const int iItemID = GetItemID( iItemUID );

		if( iItemID == CXSLItem::CI_PET_TOY )
		{
			return true;
		}
	}

	BOOST_TEST_FOREACH( UidType, iItemUID, m_vecInventorySlot[CXSLInventory::ST_PET] )
	{
		const int iItemID = GetItemID( iItemUID );

		if( iItemID == CXSLItem::CI_PET_TOY )
		{
			return true;
		}
	}

	return false;
}
#endif SERV_PET_TOY
//}}

//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
void KInventory::SetupDefenceDungeonQuickSlotItems( IN const int iDefenceDungeonPhase, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	vecInventorySlotInfo.clear();

	//////////////////////////////////////////////////////////////////////////
	// 어둠의 문 악세 장착한 상태인지 검사
	int iSpeicalCardItemID = 0;

	std::map< UidType, KInventoryItem >::const_iterator mit;
	for( mit = m_mapItem.begin(); mit != m_mapItem.end(); ++mit )
	{
		if( mit->second.GetInventoryCategory() != CXSLInventory::ST_E_EQUIP )
			continue;

		//{{ 2013. 06. 03	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		if( mit->second.GetRandomSocketList().empty() == false )
		{
			if( SiCXSLDefenceDungeonManager()->GetResultSpecialCardByDefenceDungeonItem( iDefenceDungeonPhase, mit->second.GetItemID(), mit->second.GetRandomSocketList(), iSpeicalCardItemID ) == true )
				break;
		}
		else
		{
			if( SiCXSLDefenceDungeonManager()->GetResultSpecialCardByDefenceDungeonItem( iDefenceDungeonPhase, mit->second.GetItemID(), mit->second.GetItemSocketList(), iSpeicalCardItemID ) == true )
				break;
		}
#else
		if( SiCXSLDefenceDungeonManager()->GetResultSpecialCardByDefenceDungeonItem( iDefenceDungeonPhase, mit->second.GetItemID(), mit->second.GetItemSocketList(), iSpeicalCardItemID ) == true )
			break;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}		
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 퀵슬롯 아이템 얻기
	std::vector< int > vecQuickSlotItemID;
    SiCXSLDefenceDungeonManager()->GetResultQuickSlotItem( iDefenceDungeonPhase, vecQuickSlotItemID );

	//////////////////////////////////////////////////////////////////////////	
	// 어둠의 문 전용 아이템 세팅 준비!
	std::vector< KInventoryItemInfo > vecDefenceDungeonItem;
	int iSlotID = 0;
	BOOST_TEST_FOREACH( const int, iQuickSlotItemID, vecQuickSlotItemID )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iQuickSlotItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 아이템을 PC방 프리미엄 아이템으로 세팅하였습니다!" )
				<< BUILD_LOG( iQuickSlotItemID )
				<< END_LOG;
			continue;
		}

		// 임시 ItemUID발급과 동시에 아이템 획득 처리
		KInventoryItemInfo kInventoryItemInfo;
		kInventoryItemInfo.m_iItemUID = SiKGameSysVal()->GetTempItemUID();
		kInventoryItemInfo.m_cSlotCategory = CXSLInventory::ST_E_DEFENCE_QUICK_SLOT;
		kInventoryItemInfo.m_sSlotID = iSlotID++;
		kInventoryItemInfo.m_kItemInfo.m_iItemID	= iQuickSlotItemID;
		kInventoryItemInfo.m_kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
		kInventoryItemInfo.m_kItemInfo.m_iQuantity	= pItemTemplet->m_Quantity;
		vecDefenceDungeonItem.push_back( kInventoryItemInfo );
	}

	//////////////////////////////////////////////////////////////////////////
	// 스페셜 카드값을 얻었다면 지급하자!
	if( iSpeicalCardItemID != 0 )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iSpeicalCardItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 아이템을 PC방 프리미엄 아이템으로 세팅하였습니다!" )
				<< BUILD_LOG( iSpeicalCardItemID )
				<< END_LOG;
		}
		else
		{
			// 임시 ItemUID발급과 동시에 아이템 획득 처리
			KInventoryItemInfo kInventoryItemInfo;
			kInventoryItemInfo.m_iItemUID = SiKGameSysVal()->GetTempItemUID();
			kInventoryItemInfo.m_cSlotCategory = CXSLInventory::ST_E_DEFENCE_QUICK_SLOT;
			kInventoryItemInfo.m_sSlotID = 3;
			kInventoryItemInfo.m_kItemInfo.m_iItemID	= iSpeicalCardItemID;
			kInventoryItemInfo.m_kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
			kInventoryItemInfo.m_kItemInfo.m_iQuantity	= pItemTemplet->m_Quantity;
			vecDefenceDungeonItem.push_back( kInventoryItemInfo );
		}
	}
	// 스페셜 카드를 못얻었다면 물약이라도 주자!
	else
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( SiCXSLDefenceDungeonManager()->GetQuickSlotPotionItemID() );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 아이템을 PC방 프리미엄 아이템으로 세팅하였습니다!" )
				<< BUILD_LOG( SiCXSLDefenceDungeonManager()->GetQuickSlotPotionItemID() )
				<< END_LOG;
		}
		else
		{
			// 임시 ItemUID발급과 동시에 아이템 획득 처리
			KInventoryItemInfo kInventoryItemInfo;
			kInventoryItemInfo.m_iItemUID = SiKGameSysVal()->GetTempItemUID();
			kInventoryItemInfo.m_cSlotCategory = CXSLInventory::ST_E_DEFENCE_QUICK_SLOT;
			kInventoryItemInfo.m_sSlotID = 3;
			kInventoryItemInfo.m_kItemInfo.m_iItemID	= SiCXSLDefenceDungeonManager()->GetQuickSlotPotionItemID();
			kInventoryItemInfo.m_kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
			kInventoryItemInfo.m_kItemInfo.m_iQuantity	= pItemTemplet->m_Quantity;
			vecDefenceDungeonItem.push_back( kInventoryItemInfo );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 인벤토리에 실제로 아이템을 넣자!
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kDefenceDungeonItem, vecDefenceDungeonItem )
	{
		if( VerifyCategorySlotID( kDefenceDungeonItem.m_cSlotCategory, kDefenceDungeonItem.m_sSlotID ) == false )
		{
			START_LOG( cerr, L"PC방 아이템을 넣으려는데 슬롯 위치가 이상하다? 있을수없는에러!" )
				<< BUILD_LOG( m_pUser->GetCharName() )
				<< BUILD_LOG( m_pUser->GetCharUID() )
				<< BUILD_LOG( kDefenceDungeonItem.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kDefenceDungeonItem.m_cSlotCategory )
				<< BUILD_LOG( kDefenceDungeonItem.m_sSlotID )
				<< END_LOG;
			continue;
		}

		// 슬롯이 비어있는지 체크!
		if( IsEmptySlot( kDefenceDungeonItem.m_cSlotCategory, kDefenceDungeonItem.m_sSlotID ) == false )
		{
			// 비어있지 않다면 지우자!
			KInventoryItemInfo kCheckInfo;
			if( DeleteItem( kDefenceDungeonItem.m_cSlotCategory, kDefenceDungeonItem.m_sSlotID, kCheckInfo, KDeletedItemInfo::DR_DEFENCE_DUNGEON_DUMMY_ITEM ) == false )
			{
				LIF( GetInventoryItemInfo( kDefenceDungeonItem.m_cSlotCategory, kDefenceDungeonItem.m_sSlotID, kCheckInfo ) );

				START_LOG( cerr, L"어둠의 문 전용 퀵슬롯 아이템을 넣으려는데 삭제에 실패했다! 있을수 없는에러!" )
					<< BUILD_LOG( m_pUser->GetCharName() )
					<< BUILD_LOG( m_pUser->GetCharUID() )
					<< BUILD_LOG( kDefenceDungeonItem.m_kItemInfo.m_iItemID )
					<< BUILD_LOGc( kDefenceDungeonItem.m_cSlotCategory )
					<< BUILD_LOG( kDefenceDungeonItem.m_sSlotID )
					<< BUILD_LOG( kDefenceDungeonItem.m_iItemUID )
					<< BUILD_LOG( kDefenceDungeonItem.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( kDefenceDungeonItem.m_kItemInfo.m_iQuantity )
					<< END_LOG;
			}
		}

		KInventoryItem kItem;
		kItem.Init( kDefenceDungeonItem, false );
		kItem.NotDBUpdateItem(); // DB에 업데이트 하지 않는 아이템으로 세팅
		m_mapItem.insert( std::make_pair( kDefenceDungeonItem.m_iItemUID, kItem ) );
		m_vecInventorySlot[kItem.GetInventoryCategory()][kItem.GetSlotID()] = kItem.GetItemUID();

		// 클라이언트에게 전해줄 정보도 넣자!
		vecInventorySlotInfo.push_back( kDefenceDungeonItem );

		// 각 아이템에 해당하는 쿨타임을 초기화 하자!
		m_mapItemCoolTime.erase( kDefenceDungeonItem.m_kItemInfo.m_iItemID );
	}
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2012. 07. 4	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
KInventory::GatheringClass::GatheringClass()
{
	m_iAmountOfQuantity	= 0;
	m_iDecreaseQuantity	= 0;
}

void KInventory::GatheringClass::push_back( IN const KInventoryItemInfo& kInventoryItemInfo )
{
	m_iAmountOfQuantity += kInventoryItemInfo.m_kItemInfo.m_iQuantity;
	m_vecInventoryItemInfo.push_back( kInventoryItemInfo );
}

int KInventory::GatheringClass::GetAmountOfQuantity( void )
{
	return m_iAmountOfQuantity;
}

void KInventory::GatheringClass::DecreaseTheAmountOfQuantity_Buffer( IN const int& iDecreaseQuantity )
{
	m_iDecreaseQuantity += iDecreaseQuantity;
}

void KInventory::GatheringClass::DecreaseTheAmountOfQuantity( IN const int& iDecreaseQuantity )
{
	m_iAmountOfQuantity -= iDecreaseQuantity;
}

void KInventory::GatheringClass::FlushDecreaseQuantity( void )
{
	BOOST_TEST_FOREACH( KInventoryItemInfo&, kInventoryItemInfo, m_vecInventoryItemInfo )
	{
		if( m_iDecreaseQuantity <= 0 )
			break;

		int iTryDecrease = std::min<int>( m_iDecreaseQuantity, kInventoryItemInfo.m_kItemInfo.m_iQuantity );
		kInventoryItemInfo.m_kItemInfo.m_iQuantity	-= iTryDecrease;
		m_iDecreaseQuantity							-= iTryDecrease;
		m_iAmountOfQuantity							-= iTryDecrease;
	}
}

std::vector< KInventoryItemInfo >& KInventory::GatheringClass::GetVecInventoryItemInfo( void )
{
	return m_vecInventoryItemInfo;
}

bool KInventory::IsValidCategory( IN const int& iCategoryID )
{
	switch( iCategoryID )
	{
	case CXSLInventory::ST_EQUIP:
	case CXSLInventory::ST_ACCESSORY:
	case CXSLInventory::ST_MATERIAL:
	case CXSLInventory::ST_SPECIAL:
	case CXSLInventory::ST_QUEST:
	case CXSLInventory::ST_QUICK_SLOT:
	case CXSLInventory::ST_AVARTA:
	case CXSLInventory::ST_E_QUICK_SLOT:
		return true;

	default:
		return false;
	}
}

int KInventory::GetEmptySlotNum( IN const int& iCategoryID )
{
	if( IsValidCategory( iCategoryID ) == false )
	{
		START_LOG( cerr, L"카테고리 이상." )
			<< BUILD_LOG( iCategoryID )
			<< END_LOG;
		return -1;
	}

	// 빈슬롯 체크
	int iNumEmptySlot = 0;

	for( int iSlotID=0; iSlotID < GetSlotSize( iCategoryID ); ++iSlotID )
	{
		if( IsEmptySlot( iCategoryID, iSlotID ) )
		{
			++iNumEmptySlot;
		}
	}
	
	return iNumEmptySlot;
}

bool KInventory::IsValidCategory( IN const int& iCategoryID, IN const int& iItemID )
{
	if( IsValidCategory( iCategoryID ) == false )
		return false;

	// 아이템의 카테고리 정보를 얻어옵니다.
	std::vector<int> vecCategoryID;
	GetSuitableCategory( iItemID, vecCategoryID );

	BOOST_TEST_FOREACH( const int&, iCheckCategoryID, vecCategoryID )
	{
		if( iCategoryID == iCheckCategoryID )
		{
			return true;
		}
	}
	return false;
}

bool KInventory::FillTheUnFullSlot( IN const int iCategory
								  , IN const int iItemID
								  , IN int iQuantity
								  , IN OUT std::vector< UidType >* const p_vecInventorySlot
								  , IN OUT std::map< UidType, KInventoryItem >& mapItem
								  , IN OUT KItemQuantityUpdate* const p_kDBUpdate
								  , IN OUT std::map< UidType, KInventoryItemInfo >* const p_mapSlotInfo
								  , IN OUT std::vector< KInventoryItemInfo >* const p_vecUpdatedItem
								  , OUT int& iFilledQuantity
								  )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	if( pItemTemplet->m_PeriodType != CXSLItem::PT_QUANTITY )
	{
		START_LOG( cerr, L"수량성 아이템이 아닌데 채워넣기를 시도하였습니다.." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	// 카테고리 정보를 확인합니다.
	if( IsValidCategory( iCategory, iItemID ) == false )
	{
		START_LOG( cerr, L"올바르지 않은 카테고리 정보입니다.")
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	iFilledQuantity = 0;

	// 카테고리의 슬롯들을 순회하면서 검사합니다.
	for( size_t iSlot=0; iSlot < p_vecInventorySlot[iCategory].size(); ++iSlot )
	{
		// 남은 수량이 없다면 멈춥니다.
		if( iQuantity <= 0 )
		{
			break;
		}

		// 가득 차지 않은 슬롯을 찾습니다.
		if( p_vecInventorySlot[iCategory][iSlot] <= 0 )
			continue;

		std::map< UidType, KInventoryItem >::iterator mit = mapItem.find( p_vecInventorySlot[iCategory][iSlot] );
		if( mit == mapItem.end() )
		{
			START_LOG( cerr, L"컨테이너에 존재하지 않는 아이템이라니 정보 상의 오류가 존재합니다.")
				<< BUILD_LOG( iCategory )
				<< BUILD_LOG( iSlot )
				<< BUILD_LOG( p_vecInventorySlot[iCategory][iSlot] )
				<< END_LOG;
			return false;
		}

		KInventoryItem& kInventoryItem = mit->second;

		// 아이템ID가 서로 다르다면, 채워넣을 수 없습니다.
		if( iItemID != kInventoryItem.GetItemID() )
			continue;

		// 해당 슬롯에 몇개를 더 채워넣을 수 있는지 확인합니다.
		int iRemainQuantity = pItemTemplet->m_Quantity - kInventoryItem.GetQuantity();

		if( 0 < iRemainQuantity )
		{
			// 채워 넣을 수 있는 양을 계산합니다.
			int iNumTryIncrease = std::min<int>( iQuantity, iRemainQuantity );
			int iNumIncreased = 0;

			kInventoryItem.IncreaseQuantity( iNumTryIncrease, iNumIncreased );

			// 수량 감소
			iQuantity		-= iNumIncreased;
			iFilledQuantity	+= iNumIncreased;

			if( p_kDBUpdate != NULL )
			{
				// 수량 변화가 있었으므로, 인벤토리 슬롯의 정보 갱신을 위해 해당 내역을 저장합니다.
				std::map< UidType, int >::iterator mitDBInfo = p_kDBUpdate->m_mapQuantityChange.find( kInventoryItem.GetItemUID() );
				if( mitDBInfo != p_kDBUpdate->m_mapQuantityChange.end() )
				{
					mitDBInfo->second += iNumIncreased;
				}
				else
				{
					p_kDBUpdate->m_mapQuantityChange.insert( std::make_pair( kInventoryItem.GetItemUID(), iNumIncreased ) );
				}
			}

			if( p_mapSlotInfo != NULL )
			{
				// p_vecInventorySlotInfo가 있을 경우의 준비 작업
				std::map< UidType, KInventoryItemInfo >::iterator mitSlotInfo = p_mapSlotInfo->find( kInventoryItem.GetItemUID() );
				if( mitSlotInfo == p_mapSlotInfo->end() )
				{
					KInventoryItemInfo kTemp;
					kInventoryItem.GetInventoryItemInfo( kTemp );
					p_mapSlotInfo->insert( std::make_pair( kTemp.m_iItemUID, kTemp ) );
				}
				else
				{
					mitSlotInfo->second.m_kItemInfo.m_iQuantity = kInventoryItem.GetQuantity();
				}
			}

			if( p_vecUpdatedItem != NULL )
			{
				KInventoryItemInfo kTemp;
				kInventoryItem.GetInventoryItemInfo( kTemp );
				p_vecUpdatedItem->push_back( kTemp );
			}
		}
	}
	return true;
}

bool KInventory::InsertToEmptySlotForTest( IN const int iCategory
										 , IN const int iItemID
										 , IN int iQuantity
										 , IN const KItemInfo* const p_kItemInfo
										 , IN const bool* const p_bDebug
										 , IN const short* const p_sPeriod
										 , IN OUT std::vector< UidType >* const p_vecInventorySlot
										 , IN OUT std::map< UidType, KInventoryItem >& mapItem
										 , IN OUT std::vector< KItemInfo >* const p_vecNewItem
										 , IN OUT std::vector< KBuyGPItemInfo >* const p_vecNewEDItem
										 , OUT int& iInsertedQuantity
										 )
{
	if( ( p_vecNewEDItem != NULL ) && ( ( p_bDebug == NULL ) || ( p_sPeriod == NULL ) ) )
	{
		START_LOG( cerr, L"ED 아이템 구매에 필요한 아이템 기간 정보를 얻지 못하였습니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	const int iMaxQuantity = ( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY ? pItemTemplet->m_Quantity : 1 );
	if( iMaxQuantity <= 0 )
	{
		START_LOG( cerr, L"수량 이상." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iMaxQuantity )
			<< END_LOG;
		return false;
	}

	short sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ? pItemTemplet->m_Endurance : 0 );
	if( sEndurance < 0 )
	{
		START_LOG( cerr, L"내구도 이상." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( sEndurance )
			<< END_LOG;
		return false;
	}

	char cEnchantLevel = static_cast<char>(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL);

	// 카테고리 정보를 확인합니다.
	if( IsValidCategory( iCategory, iItemID ) == false )
	{
		START_LOG( cerr, L"올바르지 않은 카테고리 정보입니다.")
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	iInsertedQuantity = 0;

	// 카테고리의 슬롯들을 순회하면서 검사합니다.
	for( size_t iSlot=0; iSlot < p_vecInventorySlot[iCategory].size(); ++iSlot )
	{
		// 남은 수량이 없다면 멈춥니다.
		if( iQuantity <= 0 )
		{
			break;
		}

		// 비어있는 슬롯을 찾습니다.
		if( 0 < p_vecInventorySlot[iCategory][iSlot] )
			continue;

		// 임의로 데이터를 생성해서 삽입합니다.

		// 중복되지 않는 임의의 ItemUID를 생성합니다.
		UidType iTempUID;
		do 
		{
			iTempUID = rand() << 32 | rand();
		}while( mapItem.find( iTempUID ) != mapItem.end() );

		KInventoryItemInfo kInventoryItemInfo;
		kInventoryItemInfo.m_iItemUID				= iTempUID;
		kInventoryItemInfo.m_cSlotCategory			= iCategory;
		kInventoryItemInfo.m_sSlotID				= iSlot;

		kInventoryItemInfo.m_kItemInfo.m_iItemID	= iItemID;
		kInventoryItemInfo.m_kItemInfo.m_iQuantity	= std::min<int>( iQuantity, iMaxQuantity );
		iQuantity									-= kInventoryItemInfo.m_kItemInfo.m_iQuantity;
		iInsertedQuantity							+= kInventoryItemInfo.m_kItemInfo.m_iQuantity;
		
		KInventoryItem kInventoryItem;
		kInventoryItem.Init( kInventoryItemInfo, true );

		if( p_vecNewItem != NULL )
		{
			KItemInfo kInfo;
			kInfo.m_iItemID				= iItemID;
			kInfo.m_cUsageType			= pItemTemplet->m_PeriodType;
			kInfo.m_iQuantity			= kInventoryItemInfo.m_kItemInfo.m_iQuantity;

			if( p_kItemInfo != NULL )
			{
				kInfo.m_sEndurance			= sEndurance;	//std::min< short >( p_kItemInfo->m_sEndurance, sEndurance );
				kInfo.m_sPeriod				= p_kItemInfo->m_sPeriod;
				kInfo.m_cEnchantLevel		= std::min< char >( p_kItemInfo->m_cEnchantLevel, cEnchantLevel );
				kInfo.m_kAttribEnchantInfo	= p_kItemInfo->m_kAttribEnchantInfo;
				kInfo.m_vecItemSocket		= p_kItemInfo->m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
				kInfo.m_byteExpandedSocketNum	= p_kItemInfo->m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
				//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				kInfo.m_vecRandomSocket		= p_kItemInfo->m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
				//}}
				//{{ 2009. 9. 7  최육사		밀봉
				kInfo.m_ucSealData			= p_kItemInfo->m_ucSealData;
			}

			p_vecNewItem->push_back( kInfo );
		}

		if( p_vecNewEDItem != NULL )
		{
			// 구매한 아이템에 대한 초기값을 설정.
			KBuyGPItemInfo kInfo;
			kInfo.m_iItemID		= iItemID;
			kInfo.m_iQuantity	= kInventoryItemInfo.m_kItemInfo.m_iQuantity;
			kInfo.m_iPrice		= *p_bDebug ? 0 : pItemTemplet->m_Price * kInfo.m_iQuantity;
			kInfo.m_iPvPPoint	= *p_bDebug ? 0 : pItemTemplet->m_PricePvPPoint * kInfo.m_iQuantity;
			kInfo.m_cPeriodType = pItemTemplet->m_PeriodType;

			if( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE )
			{
				kInfo.m_sEndurance = pItemTemplet->m_Endurance;
			}
			else
			{
				kInfo.m_sEndurance = 0;
			}

			//{{ 2008. 4. 24  최육사   기간제 아이템의 경우 기간 설정
			if( kInfo.m_cPeriodType == CXSLItem::PT_INFINITY  &&  *p_bDebug )
			{
				kInfo.m_sPeriod = *p_sPeriod;
			}
			//}}

			p_vecNewEDItem->push_back( kInfo );
		}

		p_vecInventorySlot[iCategory][iSlot] = iTempUID;
		mapItem.insert( std::map< UidType, KInventoryItem >::value_type( iTempUID, kInventoryItem ) );
	}
	return true;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}

//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
bool KInventory::FixUpInventoryItemForLetter( IN const std::vector< KInventoryItemInfo >& vecCreatedItem
								, OUT KItemQuantityUpdate& kDBUpdate
								, OUT std::vector< KInventoryItemInfo >& vecMoveItemSlotInfo
								, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo
								)
{
	kDBUpdate.m_mapQuantityChange.clear();		// 수량이 합쳐져서 증가/감소하는 것
	kDBUpdate.m_vecDeleted.clear();				// 수량이 합쳐져서 삭제되는 것
	vecInventorySlotInfo.clear();

	std::map<UidType, KInventoryItemInfo> mapInventorySlotInfo;
	std::vector< KInventoryItemInfo > vecCreatedItemTemp = vecCreatedItem;

	BOOST_TEST_FOREACH( KInventoryItemInfo&, kItem, vecCreatedItemTemp )
	{
		// 아이템의 카테고리 정보를 얻어오자
		int iCategory = GetSuitableCategory( kItem.m_kItemInfo.m_iItemID );
		bool bIsDeleted = false;
		for( int iSlot=0; iSlot < GetSlotSize( iCategory ); ++iSlot )
		{
			if( kItem.m_kItemInfo.m_iQuantity <= 0 )
				break;

			if( kItem.m_kItemInfo.m_iItemID != GetItemID( iCategory, iSlot ) )
				continue;

			int iRemainQuantity = 0;
			iRemainQuantity = GetRemainedQuantityCapacity( iCategory, iSlot );

			// 찾은 것이 있다.
			if( iRemainQuantity > 0 )
			{
				int iIncreased = 0;
				iRemainQuantity = std::min< int >( kItem.m_kItemInfo.m_iQuantity, iRemainQuantity );
				IncreaseQuantity( iCategory, iSlot, iRemainQuantity, iIncreased );
				kItem.m_kItemInfo.m_iQuantity -= iIncreased;

				std::map< UidType, int >::iterator mitInventory = kDBUpdate.m_mapQuantityChange.find( GetItemUID(iCategory, iSlot) );
				if( mitInventory != kDBUpdate.m_mapQuantityChange.end() )
				{
					mitInventory->second += iIncreased;

					KInventoryItemInfo kTemp;
					GetInventoryItemInfo( iCategory, iSlot, kTemp );
					std::map<UidType, KInventoryItemInfo>::iterator mitInventorySlotInfo = mapInventorySlotInfo.find( kTemp.m_iItemUID );
					if( mitInventorySlotInfo == mapInventorySlotInfo.end() )
					{
						mapInventorySlotInfo.insert( std::make_pair( kTemp.m_iItemUID, kTemp ) );
					}
					else
					{
						mitInventorySlotInfo->second.m_kItemInfo.m_iQuantity = kTemp.m_kItemInfo.m_iQuantity;
					}
				}
				else
				{
					kDBUpdate.m_mapQuantityChange.insert( std::make_pair( GetItemUID(iCategory, iSlot), iIncreased ) );

					KInventoryItemInfo kTemp;
					GetInventoryItemInfo( iCategory, iSlot, kTemp );
					std::map<UidType, KInventoryItemInfo>::iterator mitInventorySlotInfo = mapInventorySlotInfo.find( kTemp.m_iItemUID );
					if( mitInventorySlotInfo == mapInventorySlotInfo.end() )
					{
						mapInventorySlotInfo.insert( std::make_pair( kTemp.m_iItemUID, kTemp ) );
					}
					else
					{
						mitInventorySlotInfo->second.m_kItemInfo.m_iQuantity = kTemp.m_kItemInfo.m_iQuantity;
					}
				}

				// 들어온 아이템의 수량이 남아 있지 않으면 다음 아이템으로 넘어가자
				if( kItem.m_kItemInfo.m_iQuantity <= 0 )
				{
					kDBUpdate.m_vecDeleted.push_back( KDeletedItemInfo( kItem.m_iItemUID, KDeletedItemInfo::DR_MOVE_SLOT ) );
					break;
				}
			}			
		}

		if( 0 < kItem.m_kItemInfo.m_iQuantity )
		{
			KInventoryItemInfo kInventorySlotInfo;
			if( InsertItem( kItem.m_iItemUID, kItem.m_kItemInfo, kInventorySlotInfo ) == false )
			{
				START_LOG( cerr, L"절대로 있어선 안되고 있을 수도 없는 에러!! 이것을 사용하는 루틴의 시작 단계에서 검사해야 하는 것이다.")
					<< END_LOG;
				return false;
			}
			vecMoveItemSlotInfo.push_back( kInventorySlotInfo );
			vecInventorySlotInfo.push_back( kInventorySlotInfo );
		}
	}

	std::map<UidType, KInventoryItemInfo>::iterator it = mapInventorySlotInfo.begin();
	for( ; it != mapInventorySlotInfo.end(); ++it )
	{
		vecInventorySlotInfo.push_back( it->second );
	}

	return true;
}
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}

//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE

bool KInventory::ResolveAvatarItem( IN OUT bool& bJackpot, IN OUT std::map< int , UidType >& mapResolveItem ,
								   OUT int& iED,
								   OUT std::map< int, int >& mapInsertedItem, 
								   OUT std::vector< KInventoryItemInfo >& vecUpdated, 
								   OUT std::vector< KItemInfo >& vecNewItem )
{
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();
	KItemQuantityUpdate m_kItemQuantityUpdate;

	iED = 0;
	
	std::map< int, KItemInfo > mapTotalResultItem;  // 전체 결과템
	std::map< UidType, int >   mapDeleteResolveItem; // 분해시킬 아이템

	

	for(std::map<int,UidType>::iterator it = mapResolveItem.begin(); it !=mapResolveItem.end(); ++it)
	{
		std::map< int, KItemInfo > mapResultItem;  // 결과템
		UidType iResolveUID = it->second;
		int iResolveItemID = GetItemID( it->second );	

		int iResolveQuantity = 0;	// 아이템 분해
		int iAfterItem = 0;
		int iAfterItemQuantity = 0;
		bool bFashion = false;
		
		mapDeleteResolveItem.insert( std::make_pair( iResolveUID, 1 ) );

		SET_ERROR( NET_OK );

		if( IsExist( iResolveUID ) == false )
		{
			START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
				<< BUILD_LOG( iResolveUID )
				<< END_LOG;

			SET_ERROR( ERR_RESOLVE_ITEM_00 );
			return false;
		}
		//{{ 2009. 8. 7  최육사		은행 카테고리검사
		if( IsExistInBank( iResolveUID ) == true )
		{
			START_LOG( cerr, L"은행에 있는 아이템은 분해못하는데? 클라에서 체크 안했나?" )
				<< BUILD_LOG( iResolveUID )
				<< END_LOG;

			SET_ERROR( ERR_BANK_00 );
			return false;
		}
		//}}

		//{{ 2008. 12. 21  최육사	사용불가 검사
		if( IsBrokenItem( iResolveUID ) )
		{
			START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
				<< BUILD_LOG( iResolveUID )
				<< END_LOG;

			SET_ERROR( ERR_RESTORE_ITEM_06 );
			return false;
		}
		//}}

		//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

		if( IsDBUpdateItem( iResolveUID ) == false )
		{
			START_LOG( cerr, L"PC방 프리미엄 아이템을 분해하려고 했습니다." )
				<< BUILD_LOG( iResolveUID )
				<< END_LOG;

			SET_ERROR( ERR_PC_BANG_00 );
			return false;
		}

#endif SERV_PC_BANG_PRE
		//}}


		// 템플릿 정보 얻기
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iResolveItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
				<< BUILD_LOG( iResolveItemID )
				<< END_LOG;

			SET_ERROR( ERR_RESOLVE_ITEM_01 );
			return false;
		}

		if( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON  &&
			pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE  &&
			pItemTemplet->m_ItemType != CXSLItem::IT_ACCESSORY )
		{
			START_LOG( cerr, L"무기나 방어구, 악세사리가 아님." )
				<< BUILD_LOG( iResolveItemID )
				<< END_LOG;

			SET_ERROR( ERR_RESOLVE_ITEM_03 );
			return false;
		}


		// 캐쉬템중 기간무제한일 경우 분해 가능	
		if( pItemTemplet->m_bFashion == true )
		{
			KInventoryItemInfo kInvenItemInfo;
			GetInventoryItemInfo( iResolveUID, kInvenItemInfo );

			if( kInvenItemInfo.m_kItemInfo.m_sPeriod > 0 )
			{
				SET_ERROR( ERR_RESOLVE_ITEM_08 );
				return false;
			}



			//{{ 2011.2.27 지헌 : 캐쉬 아바타 분해시 추가 아이템 나오도록 하기
#ifdef SERV_CASHITEM_RESOLVE_ADD	// 임규수 일본 추가
			std::vector<CXSLCashItemManager::ResolveAddInfo> vecItem;
			if( SiCXSLCashItemManager()->GetCashItemResolveAddFromScript( iResolveItemID, vecItem ) )
			{
				// 넘어온 결과 아이템들을 다 넣어주자.
				std::vector<CXSLCashItemManager::ResolveAddInfo>::iterator vit;
				for(vit = vecItem.begin(); vit != vecItem.end(); vit++)
				{
					int iItemID = (*vit).iItemID;
					int iQuantity = (*vit).iQuantity;
					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet(iItemID);
					if( pItemTemplet == NULL )
					{
						START_LOG( cerr, L"아이템 템플릿 정보를 찾을 수 없습니다." )
							<< BUILD_LOG( iItemID )
							<< END_LOG;
						return false;
					}
					KItemInfo kInsertItemInfo;
					kInsertItemInfo.m_iItemID	 = iItemID;
					kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
					kInsertItemInfo.m_iQuantity	 = iQuantity;
					kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
					mapResultItem.insert( std::make_pair( iItemID, kInsertItemInfo ) );
				}
			}
#endif


			// 무속성의 엘의 속성석을 캐쉬템 가격의 공식에 따라 수량을 계산하여 준다.
			int iPrice = 0;
			if( !SiCXSLCashItemManager()->GetCashItemPriceFromScript( iResolveItemID, iPrice ) )
			{
				START_LOG( cwarn, L"캐쉬 가격을 얻을 수 없어서 분해 실패했음." )
					<< BUILD_LOG( iResolveUID )
					<< BUILD_LOG( iResolveItemID )
					<< END_LOG;

				SET_ERROR( ERR_RESOLVE_ITEM_01 );
				return false;
			}

			if( iPrice <= 0 )
			{
				START_LOG( cerr, L"이게 왜 0원이 나오지?" )
					<< BUILD_LOG( iResolveItemID )
					<< BUILD_LOG( iPrice )
					<< END_LOG;

				SET_ERROR( ERR_RESOLVE_ITEM_01 );
				return false;
			}

			//{{ 2011.03.16   임규수 아바타 분해 시스템
	#ifdef SERV_MULTI_RESOLVE
			const int RESOLVE_AVATAR = 1;
			KLottery kLotTemp;
			
			// 수수료 계산

			// 설정
			if(	pItemTemplet->m_bFashion == true )	
			{
				if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON  ||
					pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE  )
				{
					if(pItemTemplet->m_ItemGrade == CXSLItem::IG_RARE)
					{
						kLotTemp.AddCase( RESOLVE_AVATAR, 20.f );
						iED += 15000;
						// 레어 20% 15000ED
					}
					else if(pItemTemplet->m_ItemGrade == CXSLItem::IG_NORMAL)
					{
						if(pItemTemplet->m_SetID==0)
						{
							kLotTemp.AddCase( RESOLVE_AVATAR, 5.f );
							iED += 5000;
							// 일반	5% 5000ED
						}
						else
						{
							kLotTemp.AddCase( RESOLVE_AVATAR, 10.f );
							iED += 10000;
							// 한정	10 10000ED
						}
					}
				}
			}
			//

			// 9. 비용 검사
			if( m_pUser->GetED() < iED )
			{
				SET_ERROR( ERR_SOCKET_ITEM_02 );
				return false;
			}

			if( kLotTemp.Decision() == RESOLVE_AVATAR )
			{
				if( SiCXSLResolveItemManager()->GetResultItemAvatar( (int)pItemTemplet->m_ItemGrade, (int)pItemTemplet->m_ItemType, mapResultItem , iAfterItem, iAfterItemQuantity ) == false )
				{
					START_LOG( cerr, L"분해 결과 아이템 받아오기 실패.!" )
						<< BUILD_LOG( iResolveItemID )
						<< BUILD_LOG( pItemTemplet->m_ItemGrade )
						<< BUILD_LOG( pItemTemplet->m_ItemType )
						<< BUILD_LOG( mapResultItem.size() )
						<< END_LOG;

					return false;
				}
			}
	#endif SERV_MULTI_RESOLVE
			//}}

			//{{ 101020 김정협 일본 분해 개당 50원 설정
			//iResolveQuantity = iPrice / 50; // 0나누기 조심~ (일본)
			iResolveQuantity = iPrice / 200; // 0나누기 조심~ (국내와 통일작업) 김석근, 2013.05.24  수정.
			//}} 101020 김정협 일본 분해 개당 50원 설정

			KItemInfo kInsertItemInfo;
			kInsertItemInfo.m_iItemID	 = CXSLAttribEnchantItem::ATI_UNKNOWN;
			kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
			kInsertItemInfo.m_iQuantity	 = iResolveQuantity;
			kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;

			std::map< int, KItemInfo >::iterator it = mapResultItem.find(CXSLAttribEnchantItem::ATI_UNKNOWN); 
			if( it != mapResultItem.end())
			{
				it->second.m_iQuantity += iResolveQuantity;
			}
			else
			{
				mapResultItem.insert( std::make_pair( CXSLAttribEnchantItem::ATI_UNKNOWN, kInsertItemInfo ) );
			}
			// 패션 아이템인지 체크
			bFashion = true;


			// 캐쉬아이템을 분해 했다면 로그를 남긴다.
			if( bFashion )
			{
				KELOG_CASH_ITEM_RESOLVE_LOG_NOT kPacketLog;
				kPacketLog.m_iUnitUID = m_pUser->GetCharUID();
				kPacketLog.m_iItemUID = iResolveUID;
				kPacketLog.m_iAfterItemID = iAfterItem;
				kPacketLog.m_iAfterItemQuantity = iAfterItemQuantity;
				kPacketLog.m_iLevel	= m_pUser->GetLevel();
				kPacketLog.m_iResultQuantity = iResolveQuantity;

				m_pUser->SendToLogDB(ELOG_CASH_ITEM_RESOLVE_LOG_NOT, kPacketLog );
			}

			//////////////////////////////////////////////////////////////////////////
			// 분해 결과물 전체 합산!
			std::map< int, KItemInfo >::const_iterator mit;
			for( mit = mapResultItem.begin(); mit != mapResultItem.end(); ++mit )
			{
				std::map< int, KItemInfo >::iterator mitInsert;
				mitInsert = mapTotalResultItem.find( mit->first );
				if( mitInsert == mapTotalResultItem.end() )
				{
					mapTotalResultItem.insert( std::make_pair( mit->first, mit->second ) );
				}
				else
				{
					mitInsert->second.m_iQuantity += mit->second.m_iQuantity;
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			int iSellPrice = 0;
			
	#ifdef SERV_ITEM_RESOLVE_NEW		//{{ 2011. 01. 20	최육사	아이템 분해 개편
			
			int iQuantity = 0;		// 아이템 수량 얻기
			if( GetQuantity( iResolveUID, iQuantity ) == false )
			{
				START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
					<< BUILD_LOG( iResolveUID )
					<< END_LOG;

				SET_ERROR( ERR_RESOLVE_ITEM_00 );
				return false;
			}

			if( GetSellPrice( iResolveUID, iQuantity, iSellPrice ) == false )
			{
				START_LOG( cerr, L"해당 아이템의 되팔기 가격을 얻지 못했습니다! 분해 실패!" )
					<< BUILD_LOG( iResolveUID )
					<< END_LOG;

				SET_ERROR( ERR_RESOLVE_ITEM_00 );
				return false;
			}

			// 분해 작업
			if( SiCXSLResolveItemManager()->GetResultItem( (int)pItemTemplet->m_ItemGrade, pItemTemplet->m_UseLevel, (int)pItemTemplet->m_ItemType, iSellPrice, bJackpot, mapResultItem ) == false )
			{
				START_LOG( cerr, L"분해 결과 아이템 받아오기 실패.!" )
					<< BUILD_LOG( iResolveItemID )
					<< BUILD_LOG( pItemTemplet->m_ItemGrade )
					<< BUILD_LOG( pItemTemplet->m_UseLevel )
					<< BUILD_LOG( pItemTemplet->m_ItemType )
					<< BUILD_LOG( iSellPrice )
					<< BUILD_LOG( mapResultItem.size() )
					<< END_LOG;

				return false;
			}

			// 대박 후처리
			//if( bJackpot )
			//{
				// 대박 로그
			//	KELOG_RESOLVE_JACKPOT_LOG_NOT kPacketToLog;
			//	CTime kUsedTime = CTime::GetCurrentTime();
			//	kPacketToLog.m_UnitUID		  = m_pUser->GetCharUID();
			//	kPacketToLog.m_iResolveItemID = iResolveItemID;
			//	kPacketToLog.m_iElapsedTime	  = static_cast<int>( m_kTimer[TM_RESOLVE_JACKPOT].elapsed() );
			//	kPacketToLog.m_wstrRegDate	  = ( CStringW )( kUsedTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
			//	m_pUser->SendToLogDB( ELOG_RESOLVE_JACKPOT_LOG_NOT, kPacketToLog );
			//}


	#else
			// 분해 작업
			if( SiCXSLResolveItemManager()->GetResultItem( (int)pItemTemplet->m_ItemGrade, pItemTemplet->m_UseLevel, (int)pItemTemplet->m_EqipPosition, pItemTemplet->m_Price, bJackpot, mapResultItem ) == false )
			{
				START_LOG( cerr, L"분해 결과 아이템 받아오기 실패.!" )
					<< BUILD_LOG( iResolveItemID )
					<< BUILD_LOG( pItemTemplet->m_EqipPosition )
					<< BUILD_LOG( pItemTemplet->m_Price )
					<< BUILD_LOG( mapResultItem.size() )
					<< END_LOG;

				return false;
			}
	#endif SERV_ITEM_RESOLVE_NEW
			//}}

			//////////////////////////////////////////////////////////////////////////
			// 분해 결과물 전체 합산!
			std::map< int, KItemInfo >::const_iterator mit;
			for( mit = mapResultItem.begin(); mit != mapResultItem.end(); ++mit )
			{
				std::map< int, KItemInfo >::iterator mitInsert;
				mitInsert = mapTotalResultItem.find( mit->first );
				if( mitInsert == mapTotalResultItem.end() )
				{
					mapTotalResultItem.insert( std::make_pair( mit->first, mit->second ) );
				}
				else
				{
					mitInsert->second.m_iQuantity += mit->second.m_iQuantity;
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}	
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2008. 3. 17  최육사  DeleteAndInsert()
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapTotalResultItem.begin(); mitRI != mapTotalResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}
	//}}
	//////////////////////////////////////////////////////////////////////////

	// 생성된 아이템이 들어갈 자리가 있는지 조사한다.
	if( !IsEnoughSpaceExist( mapInsertedItem ) )
	{
		SET_ERROR( ERR_RESOLVE_ITEM_06 );
		return false;
	}	

 	// 분해 결과 처리
 	if( !DeleteAndInsert( mapDeleteResolveItem, mapTotalResultItem, vecUpdated, vecNewItem, KDeletedItemInfo::DR_RESOLVE ) )
 	{
 		SET_ERROR( ERR_RESOLVE_ITEM_04 );
 		return false;
 	}

/*

	// 분해 통계	
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( iItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_RESOLVE, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_RESOLVE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	// 분해 결과 통계
	std::map< int, int >::const_iterator mitResult;
	for( mitResult = mapInsertedItem.begin(); mitResult != mapInsertedItem.end(); ++mitResult )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mitResult->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_R_RESULT, mitResult->second );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_R_RESULT, mitResult->second );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
*/
	return true;
}

#endif SERV_MULTI_RESOLVE
//}}

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
bool KInventory::OpenSynthesisItem(
								IN UidType iKeyItemUID, 
								IN int iSocketNo ,
								IN std::map<int,UidType> mapSynthesisData,
								OUT std::map< int, int >& mapInsertedItem, 
								OUT std::vector< KInventoryItemInfo >& vecUpdated, 
								OUT std::vector< KItemInfo >& vecNewItem,
								OUT std::set< int > &setSealCashItem)
{
	SET_ERROR( NET_OK );

	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();

	
	std::map< UidType, int > mapToDeleteByItemID;	// 지울템
	std::map< int, KItemInfo > mapResultItem;	// 결과템


	// 1. 합성제 존재 여부
	if( IsExist( iKeyItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iKeyItemUID )
			<< END_LOG;

		SET_ERROR( ERR_RANDOM_ITEM_00 );
		return false;
	}
	//{{ 2009. 8. 7  최육사		은행
	if( IsExistInBank( iKeyItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는아이템은 오픈할수 없다" )
			<< BUILD_LOG( iKeyItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_02 );
		return false;
	}
	//}}

	int iKeyItemID = GetItemID( iKeyItemUID );
	
	if( CXSLItem::CI_SYNTHESIS_ITEM != iKeyItemID )
	{
		START_LOG( cerr, L"합성제가 없습니다." )
			<< BUILD_LOG( iKeyItemUID )
			<< BUILD_LOG( iKeyItemID )
			<< END_LOG;

		SET_ERROR( ERR_SYNTHESIS_01 );
		return false;
	}
	//}}


	// 합성 아바타 갯수는 꼭 4개여야 한다.
	
	if (mapSynthesisData.size() != 4)
	{
		START_LOG( cerr, L"합성용 아바타는 꼭 4개여야 합니다!!!!" )
			<< BUILD_LOG( mapSynthesisData.size() )
			<< END_LOG;

		SET_ERROR( ERR_SYNTHESIS_02 );
		return false;
	}

	int iUnitType = -1;
	int iEqipPosition = -1;
	
	// 합성제 삭제 등록
	mapToDeleteByItemID.insert( std::make_pair( iKeyItemUID, 1 ) );	

	// 일반 아바타 4개 데이터 검사 및 삭제 등록
	std::map<int,UidType>::iterator iter;
	for ( iter = mapSynthesisData.begin(); iter != mapSynthesisData.end(); iter++ )
	{
		int iTmpAvatarItemID = GetItemID( iter->second );
		const CXSLItem::ItemTemplet* pAvatarItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTmpAvatarItemID );

		if( iTmpAvatarItemID == 0 || pAvatarItemTemplet == NULL )
		{
			START_LOG( cerr, L"없는 아바타를 합성하려고 했습니다." )
				<< BUILD_LOG( iKeyItemUID )
				<< BUILD_LOG( iTmpAvatarItemID )
				<< END_LOG;

			SET_ERROR( ERR_SYNTHESIS_02 );
			return false;
		}

		if (iter == mapSynthesisData.begin())
		{
			iUnitType = pAvatarItemTemplet->m_UnitType;
			iEqipPosition = pAvatarItemTemplet->m_EqipPosition;
		}
		else if (iUnitType != pAvatarItemTemplet->m_UnitType || iEqipPosition != pAvatarItemTemplet->m_EqipPosition)
		{
			// 아바타의 UnitType이 다르거나 장비 포지션이 다른 경우 
			SET_ERROR( ERR_SYNTHESIS_04 );
			return false;
		}

		std::map<UidType,int>::iterator  mit = mapToDeleteByItemID.find(iter->second);
		if( mit == mapToDeleteByItemID.end() )
		{
			// 아바타 삭제 등록
			mapToDeleteByItemID.insert( std::make_pair( iter->second, 1 ) );
		}
		else
		{
			START_LOG( cerr, L"어떻게 같은 아바타 아이템 UID를 합성해달라고 요청하지???." )
				<< BUILD_LOG( iter->second )
				<< BUILD_LOG( iTmpAvatarItemID )
				<< END_LOG;

			SET_ERROR( ERR_SYNTHESIS_02 );
			return false;
		}
	}

	// 5. 가능한 소켓 옵션인지 체크
	int iTmpSocketGroupID = SiCXSLSynthesisManager()->GetAvatarSocketOptionGroupID(iEqipPosition);
	if (0 == iTmpSocketGroupID)
	{
		START_LOG( cerr, L"해당 파츠에 왜 소켓 그룹 ID가 없지???." )
			<< BUILD_LOG( iEqipPosition )
			<< BUILD_LOG( iTmpSocketGroupID )
			<< END_LOG;

		SET_ERROR( ERR_SYNTHESIS_03 );
		return false;
	}

	if ( false == SiCXSLSocketItem()->CheckCashItemGroupID( iSocketNo, iTmpSocketGroupID ) )
	{
		START_LOG( cerr, L"사용하지 못하는 Socket No인데??? GroupID 다름" )
			<< BUILD_LOG( iSocketNo )
			<< BUILD_LOG( iTmpSocketGroupID )
			<< END_LOG;

		SET_ERROR( ERR_SYNTHESIS_03 );
		return false;
	}

	// 5. 랜덤 결과 
	if( SiCXSLSynthesisManager()->GetSynthesisResultItem( iUnitType , iEqipPosition , iSocketNo , mapResultItem , setSealCashItem ) == false )
	{
		START_LOG( cwarn, L"랜덤아이템 받아오기 실패.!" )
			<< BUILD_LOGc( iUnitType )
			<< BUILD_LOGc( iEqipPosition )
			<< BUILD_LOG( mapResultItem.size() )
			<< END_LOG;

		return false;
	}


	// 6. 습득 아이템 정보
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}

	// 분해 결과 처리
	if( !DeleteAndInsert( mapToDeleteByItemID, mapResultItem, vecUpdated, vecNewItem, KDeletedItemInfo::DR_SYNTHESIS ) )
	{
		SET_ERROR( ERR_RESOLVE_ITEM_04 );
		return false;
	}	

	return true;
}
#endif SERV_SYNTHESIS_AVATAR
//}}
#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
bool KInventory::OpenRandomItem_DEV( IN char cUnitClass, 
									IN const int UsediItemID,
									IN const int iItemKeyID,
									OUT std::map< int, int >& mapInsertedItem, 
									OUT int& iRessurectionCount,
									OUT int& iRestoreSpirit,
									OUT bool& bCharmItem )
{

	mapInsertedItem.clear();


	std::map< int, int >	   mapRequireKey;  // 열쇠템
	std::map< int, KItemInfo > mapResultItem;  // 결과템


	const int iItemID = UsediItemID;

	// 1-1. 매력아이템인지 검사
	//{{ 2013. 3. 16	박세훈	매력 아이템 스크립트 처리
#ifdef SERV_CHARM_ITEM_SCRIPT
	bCharmItem = SiCXSLRandomItemManager()->IsCharmItem( iItemID );
#else  SERV_CHARM_ITEM_SCRIPT
	bCharmItem = ( iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM  
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_EVENT 
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ARCHANGEL_CHUNG 
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_SALVATOR_CHUNG
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_OFFICER_CHUNG
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_OFFICER
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_NASOD_BATTLE_CHUNG
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_NASOD_BATTLE
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ROYAL_SERVENT_CHUNG
		//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_GOLD
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}
		//{{ 2012. 09. 04	박세훈	봉인된 얼음 조각상(청 아크데빌)
#ifdef SERV_ATTRACTION_ITEM_ARCHDEVIL
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ARCHDEVIL_CHUNG
#endif SERV_ATTRACTION_ITEM_ARCHDEVIL
		//}}
		//{{ 2012. 11. 14	박세훈	봉인된 얼음 조각상(청 벨더 임페리얼 가드)
#ifdef SERV_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD
#endif SERV_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD
		//}}
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_9_10_ENCHANT_ITEM
		|| iItemID == CXSLRandomItemManager::RID_2012_WINTER_VACATION_LOTTO_CUBE
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ARCHANGEL_ARA
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ELSWORD_5YEAR_CUBE
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_2013_D4_CUBE
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_2013_D2_CUBE
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_2012_GOODBYE_CUBE
		//{{ 2012. 12. 28  미니 얼음 조각상 가열기 - 김민성
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
		|| iItemID == CXSLRandomItemManager::RID_MINI_ATTRACTION_ITEM
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
		//}}
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
		|| iItemID == CXSLRandomItemManager::RID_ONE_THIRD_MINI_ATTRACTION_ITEM 
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_EVELUN_ARA
#ifdef SERV_ATTRACTION_ITEM_ARC_DEVIL
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ARC_DEVIL
#endif
		//{{ 2013. 2. 21	박세훈	2013년 02월 21일 패치
#ifdef SERV_ATTRACTION_ITEM_GOLD_CONDOR
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_GOLD_CONDOR	// 봉인된 얼음 조각상( 황금 콘돌 아바타 )
#endif SERV_ATTRACTION_ITEM_GOLD_CONDOR
		//}}
		// 유럽 전용 가열기 추가
#ifdef SERV_ATTRACTION_ITEM_EU
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_OFFICER_EU
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_OFFICER_DE 
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_NASOD_EU
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_NASOD_DE 
#endif //SERV_ATTRACTION_ITEM_EU
#ifdef SERV_MINI_ATTRACTION_ITEM_TW
		|| iItemID == CXSLRandomItemManager::RID_MINI_ATTRACTION_ITEM_TW1
		|| iItemID == CXSLRandomItemManager::RID_MINI_ATTRACTION_ITEM_TW2
		|| iItemID == CXSLRandomItemManager::RID_MINI_ATTRACTION_ITEM_TW2
#endif SERV_MINI_ATTRACTION_ITEM_TW
#ifdef SERV_MINI_ATTRACTION_ITEM_JP
		|| iItemID == CXSLRandomItemManager::RID_MINI_ATTRACTION_ITEM_JP1
#endif SERV_MINI_ATTRACTION_ITEM_JP
#ifdef SERV_BATTLEFIELD_COOKIE_PIECE
		|| iItemID == CXSLRandomItemManager::RID_BATTLEFIELD_COOKIE
		|| iItemID == CXSLRandomItemManager::RID_BATTLEFIELD_GOLD_COOKIE
#endif SERV_BATTLEFIELD_COOKIE_PIECE
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
		|| iItemID == CXSLRandomItemManager::RID_ONE_THIRD_MINI_ATTRACTION_ITEM 
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
		//{{ 2012. 10. 08  할로윈 가열기
#ifdef SERV_HALLOWEEN_ICE_HEATER
		|| iItemID == CXSLRandomItemManager::RID_HALLOWEEN_ICE_HEATER_ITEM
		|| iItemID == CXSLRandomItemManager::RID_SHINE_HALLOWEEN_ICE_HEATER_ITEM
#endif SERV_HALLOWEEN_ICE_HEATER
		//}}
#ifdef SERV_COUNTRY_JP	
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ARA_OFFICER 
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_CHUNG_ANGEL 
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ARA_ANGEL 
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_FROM_MOON 
		|| iItemID == CXSLRandomItemManager::RID_ATTRACTION_ITEM_ARA_NBS 
#endif //SERV_COUNTRY_JP
		);
#endif SERV_CHARM_ITEM_SCRIPT
	//}}

	//{{ 2010.12.20 김민성 큐브 오픈 레벨 제한 확인
#ifdef SERV_OPEN_RANDOM_ITEM_LEVEL_CHECK
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );

		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿 정보 찾기 실패!" )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			SET_ERROR( ERR_ITEM_04 );
			return false;
		}

		// 레벨 조건 검사
		if(pItemTemplet->m_UseLevel > m_pUser->GetLevel())
		{
			START_LOG( cerr, L"큐브보다 낮은 레벨의 유저가 오픈!" )			// cerr 인 이유: 클라에서 걸러진 내용인데 와서 이상함	
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			SET_ERROR( ERR_RANDOM_ITEM_08 );
			return false;
		}
	}
#endif SERV_OPEN_RANDOM_ITEM_LEVEL_CHECK


	// 2. 랜덤 아이템 정보
	//{{ 2009. 12. 5  최육사	랜덤큐브시스템개편
	const std::map< int, int >* pRandomItemKeyList = SiCXSLRandomItemManager()->GetRandomItemKeyList( iItemID );
	if( pRandomItemKeyList == NULL )
	{
		START_LOG( cerr, L"해당 랜덤 아이템의 키 테이블 정보를 얻지 못함." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RANDOM_ITEM_01 );
		return false;
	}

	// 4. 키템의 수량이 충분한지 검사
	if( pRandomItemKeyList->empty() )
	{
		START_LOG( cerr, L"해당 랜덤 아이템에 대한 키템 정보가 없다. 있을수 없는에러!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RANDOM_ITEM_01 );
		return false;
	}

	//bool bEnoughKey = false;
	int iKeyItemID = iItemKeyID;

	//std::map< int, int >::const_iterator mitKey;
	//for( mitKey = pRandomItemKeyList->begin(); mitKey != pRandomItemKeyList->end(); ++mitKey )
	//{
	//	// 열쇠가 필요없는 랜덤 아이템인지 검사
	//	if( mitKey->first == 0 )
	//	{
	//		bEnoughKey = true;
	//		break;
	//	}

	//	if( mitKey->second <= 0 )
	//	{
	//		START_LOG( cerr, L"필요한 키 아이템 수량 이상." )
	//			<< BUILD_LOG( mitKey->first )
	//			<< BUILD_LOG( mitKey->second )
	//			<< END_LOG;

	//		SET_ERROR( ERR_RANDOM_ITEM_05 );
	//		return false;
	//	}

	//	std::map< int, int > mapCheckRequireKey;
	//	mapCheckRequireKey.insert( std::make_pair( mitKey->first, mitKey->second ) );

	//	if( IsEnoughItemExistOnlyInventory( mapCheckRequireKey ) == true )
	//	{
	//		iKeyItemID = mitKey->first;
	//		bEnoughKey = true;
	//		mapRequireKey = mapCheckRequireKey;
	//		break;
	//	}
	//}

	//if( bEnoughKey == false )
	//{
	//	SET_ERROR( ERR_RANDOM_ITEM_04 );
	//	return false;
	//}
	//}}


	const CXSLRandomItemManager::RandomItemData* pRandomItemData = SiCXSLRandomItemManager()->GetRandomItemData( iItemID, iKeyItemID );
	if( pRandomItemData == NULL )
	{
		START_LOG( cerr, L"랜덤 아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_RANDOM_ITEM_01 );
		return false;
	}

	// 3. 부활석 충전 정보 및 근성도 회복템 정보 얻기
	iRessurectionCount = pRandomItemData->m_iRessurectionCount;
	iRestoreSpirit	   = pRandomItemData->m_iRestoreSpirit;

	std::map< int, int > mapCheckEmpty; // 큐브의 모든 확률에 대한 빈공간 검사 [특정 확률 높이기 방지]

	// 5. 랜덤 결과 얻기
	//{{ 2011. 05. 12  김민성	랜덤 큐브 오픈시 인벤토리 검사 수정
#ifdef SERV_RANDOM_ITEM_CHECK_INVEN
	bool				 bIsGiveAll = false;
	if( SiCXSLRandomItemManager()->GetResultItem( cUnitClass, pRandomItemData, mapResultItem, mapCheckEmpty, bIsGiveAll ) == false )
#else
	if( SiCXSLRandomItemManager()->GetResultItem( cUnitClass, pRandomItemData, mapResultItem, mapCheckEmpty ) == false )
#endif SERV_RANDOM_ITEM_CHECK_INVEN
		//}}
	{
		START_LOG( cwarn, L"랜덤아이템 받아오기 실패.!" )
			<< BUILD_LOGc( cUnitClass )
			<< BUILD_LOG( pRandomItemData->m_ItemID )
			<< BUILD_LOG( mapResultItem.size() )
			<< END_LOG;

		return false;
	}


	//{{ 2008. 5. 27  최육사  랜덤 결과 인벤 공간 검사	
	std::map< int, int > mapCheckResultItem;



	// 6. 습득 아이템 정보
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}




	return true;

}
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

#ifdef SERV_READY_TO_SOSUN_EVENT
bool KInventory::ExchangeToEvent( IN int iDeleteItemID, IN int iDeleteItemCount, IN int iInsertItemID, IN int iInsertItemCount, OUT std::vector< KInventoryItemInfo >& vecUpdatedInventorySlot, OUT std::vector< KItemInfo >& vecNewItemInfo, IN int iFirstUnitClass  )
{
	SET_ERROR( NET_OK );

	std::vector< UidType > vecItemUIDList;
	GetItemUIDListThisItem( iDeleteItemID, vecItemUIDList, true );

	if(vecItemUIDList.size() == 0)
	{
		START_LOG(cerr, L"인벤토리에 이벤트 아이템이 없는데 보상을 받으려고 한다.")
			<< BUILD_LOG( iDeleteItemID )
			<< END_LOG;

		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_00 );
		return false;
	}

	UidType iDeleteItemUID = vecItemUIDList[0];		

	// 1. 삭제 아이템 정보
	std::map<UidType, int> mapDeleteItem;
	mapDeleteItem.insert( std::make_pair( iDeleteItemUID, iDeleteItemCount ));

	// 2. 지급 아이템 정보 
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iInsertItemID );
	if(pItemTemplet == NULL)
	{
		START_LOG( cerr, L"아이템 템플릿 포인터 이상.!" )
			<< BUILD_LOG( iInsertItemID )
			<< END_LOG;

		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_00 );
		return false;
	}

	KItemInfo kInsertItemInfo;
	kInsertItemInfo.m_iItemID	 = iInsertItemID;
	kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
	kInsertItemInfo.m_iQuantity	 = iInsertItemCount;
	kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;

	std::map< int, KItemInfo > mapInsertItem;
	mapInsertItem.clear();

	if( iFirstUnitClass == 28 )
	{
		mapInsertItem.insert( std::make_pair( iInsertItemID, kInsertItemInfo ) );
	}	

/*	타입 참고용
	bool DeleteAndInsert( IN const std::map< UidType, int >& mapToDelete, 
	IN const std::map< int, KItemInfo >& mapToInsert, 
	OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, 
	OUT std::vector< KItemInfo >& vecNewItem, 
	IN KDeletedItemInfo::DELETE_REASON eReason );
*/
	if( DeleteAndInsert(mapDeleteItem, mapInsertItem, vecUpdatedInventorySlot, vecNewItemInfo, KDeletedItemInfo::DR_EXTRACT_ENCHANT) == false )
	{
		START_LOG( cerr, L"강화 아이템 추출 기능에서 아이템 지급 및 삭제 부분에서 문제 발생!" )
			<< BUILD_LOG( iDeleteItemUID )
			<< BUILD_LOG( iInsertItemID )
			<< END_LOG;

		SET_ERROR( ERR_USE_ITEM_IN_INVENTORY_00 );
		return false;
	}

	return true;
}
#endif SERV_READY_TO_SOSUN_EVENT

//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
bool KInventory::ItemEvaluateCheck( IN const UidType iItemUID, OUT std::vector< int >& vecRandomSocket, OUT int& iCost )
{
	vecRandomSocket.clear();
	iCost = 0;

	SET_ERROR( ERR_UNKNOWN );

	//////////////////////////////////////////////////////////////////////////
	// 소켓 설치할 아이템에 대한 검사

	// 1. 소켓 넣을 아이템 검사
	if( IsExist( iItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_00 );
		return false;
	}

	// 2. 은행에 있는 아이템에는 소켓 못넣음
	if( IsExistInBank( iItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는 아이템은 소켓 못넣는데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_01 );
		return false;
	}

	// 3. 사용불가 아이템인지 검사
	if( IsBrokenItem( iItemUID ) )
	{
		START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_01 );
		return false;
	}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	// 4. PC방 전용 아이템인지 검사
	if( IsDBUpdateItem( iItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템에 소켓을 박을려고 했습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	const int iItemID = GetItemID( iItemUID );

	// 5. 템플릿 정보 얻기
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_00 );
		return false;
	}
	
	// 6. 장착 불가 아이템인지 검사
	if( pItemTemplet->m_bNoEquip )
	{
		START_LOG( clog, L"장착 불가 아이템은 소켓설치 못함." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_01 );
		return false;
	}

	// 7. 감정이 가능한 아이템 인지 검사
	switch( pItemTemplet->m_EqipPosition )
	{
	case CXSLUnit::EP_WEAPON_HAND:
	case CXSLUnit::EP_DEFENCE_BODY:
	case CXSLUnit::EP_DEFENCE_LEG:
	case CXSLUnit::EP_DEFENCE_HAND:
	case CXSLUnit::EP_DEFENCE_FOOT:
	case CXSLUnit::EP_AC_FACE1:
	case CXSLUnit::EP_AC_FACE2:
	case CXSLUnit::EP_AC_FACE3:
	case CXSLUnit::EP_AC_BODY:
	case CXSLUnit::EP_AC_LEG:
	case CXSLUnit::EP_AC_ARM:
	case CXSLUnit::EP_AC_RING:
	case CXSLUnit::EP_AC_NECKLESS:
	case CXSLUnit::EP_AC_WEAPON:
		break;

	default:
		{
			START_LOG( clog, L"특정 장착 위치의 아이템을 제외하고는 랜덤 소켓 불가능." )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( pItemTemplet->m_EqipPosition )
				<< END_LOG;

			SET_ERROR( ERR_EVALUATE_ITEM_01 );
			return false;
		}
		break;
	}
	
	// 8. 랜덤 소켓 아이템인지 확인
	if( pItemTemplet->m_vecRandomSocketGroupID.empty() == true )
	{
		START_LOG( clog, L"랜덤 소켓 아이템이 아닙니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( pItemTemplet->m_vecRandomSocketGroupID.size() )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_01 );
		return false;
	}

	// 9. 수수료 비용 검사
	const int iEvaluateCost = static_cast<int>( static_cast<float>(pItemTemplet->m_Price) * 0.05f );
	if( m_pUser->GetED() < iEvaluateCost )
	{
		SET_ERROR( ERR_EVALUATE_ITEM_02 );
		return false;
	}

	// 9. 랜덤 소켓 정보를 얻자!
	int iAssignedSocketCount = 0;
	if( GetCountAssignedRandomSocket( iItemUID, iAssignedSocketCount ) == false )
	{
		START_LOG( cerr, L"아이템이 존재하지 않습니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_00 );
		return false;
	}

	// 10. 이미 감정된 아이템인지 확인!
	if( iAssignedSocketCount != 0 )
	{
		START_LOG( cerr, L"이미 감정된 아이템입니다!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_03 );
		return false;
	}

	// 11. 아이템 감정!
	if( SiCXSLSocketItem()->EvaluateItemRandomSocket( iItemID, vecRandomSocket ) == false )
	{
		START_LOG( cerr, L"아이템 감정을 실패하였습니다.!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_04 );
		return false;
	}

	// 12. 아이템 감정 비용 삭감
	m_pUser->DecreaseED( iEvaluateCost );
	iCost = iEvaluateCost;

	SET_ERROR( NET_OK );
	return true;
}

bool KInventory::ItemEvaluateResult( IN const UidType iItemUID, IN const std::vector< int >& vecRandomSocket, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	vecInventorySlotInfo.clear();

	// 1. 랜덤 소켓 정보를 해당 아이템에 업데이트
	if( SetRandomSocketOption( iItemUID, vecRandomSocket ) == false )
	{
		START_LOG( cerr, L"아이템 감정을 실패하였습니다.!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_04 );
		return false;
	}

	// 2. 업데이트 되고 나서 해당 슬롯 정보 얻기
	KInventoryItemInfo kInfo;
	LIF( GetInventoryItemInfo( iItemUID, kInfo ) );
    vecInventorySlotInfo.push_back( kInfo );
	return true;
}

bool KInventory::GetRandomSocketList( IN const UidType iItemUID, OUT std::vector< int >& vecRandomSocket )
{
	vecRandomSocket.clear();

	std::map< UidType, KInventoryItem >::const_iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	vecRandomSocket = mit->second.GetRandomSocketList();
	return true;
}

bool KInventory::GetRandomSocketList( IN const int iCategory, IN const int iSlotID, OUT std::vector< int >& vecRandomSocket )
{
	return GetRandomSocketList( GetItemUID( iCategory, iSlotID ), vecRandomSocket );
}

bool KInventory::GetRandomSocketList( IN const UidType iItemUID, OUT std::map< int, int >& mapRandomSocket )
{
	mapRandomSocket.clear();

	std::map< UidType, KInventoryItem >::const_iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	const std::vector< int >& vecItemSocket = mit->second.GetItemSocketList();

	for( u_int ui = 0; ui < vecItemSocket.size(); ++ui )
	{
		if( vecItemSocket[ui] > 0 )
		{
			mapRandomSocket.insert( std::make_pair( ui, vecItemSocket[ui] ) );
		}
	}

	return true;    
}

bool KInventory::GetRandomSocketOption( IN const UidType iItemUID, IN const int iSocketIndex, OUT int& iSocketOptionID )
{
	std::map< UidType, KInventoryItem >::iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	return mit->second.GetRandomSocketOption( iSocketIndex, iSocketOptionID );
}

bool KInventory::GetCountAssignedRandomSocket( IN const UidType iItemUID, OUT int& iAssignedSocketCount )
{
	std::map< UidType, KInventoryItem >::const_iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		return false;
	}

	iAssignedSocketCount = mit->second.GetCountAssignedRandomSocket();
	return true;
}

bool KInventory::SetRandomSocketOption( IN const UidType iItemUID, IN const std::vector< int >& vecRandomSocket )
{
	std::map< UidType, KInventoryItem >::iterator mit;
	mit = m_mapItem.find( iItemUID );
	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"아이템이 존재하지 않습니다!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;
		return false;
	}
        
	mit->second.SetRandomSocketOption( vecRandomSocket );
	return true;
}

bool KInventory::RestoreItemEvaluateCheck( IN const UidType iSupportItemUID, IN const UidType iTargetItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	vecInventorySlotInfo.clear();

	SET_ERROR( ERR_UNKNOWN );

	// 1. 지정된 아이템들이 현재 인벤에 있는지 검사
	if( IsExist( iSupportItemUID ) == false  ||  IsExist( iTargetItemUID ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템UID인데?" )
			<< BUILD_LOG( iSupportItemUID )
			<< BUILD_LOG( iTargetItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_00 );
		return false;
	}

	// 2. 은행에 있는 아이템에는 사용을 못함
	if( IsExistInBank( iSupportItemUID ) == true  ||  IsExistInBank( iTargetItemUID ) == true )
	{
		START_LOG( cerr, L"은행에 있는 아이템은 소켓 못넣는데?" )
			<< BUILD_LOG( iSupportItemUID )
			<< BUILD_LOG( iTargetItemUID )
			<< END_LOG;

		SET_ERROR( ERR_BANK_01 );
		return false;
	}

	// 3. 사용불가 아이템인지 검사
	if( IsBrokenItem( iSupportItemUID ) == true  ||  IsBrokenItem( iTargetItemUID ) == true )
	{
		START_LOG( cerr, L"사용불가 아이템. 일어날수 없는에러" )
			<< BUILD_LOG( iSupportItemUID )
			<< BUILD_LOG( iTargetItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_01 );
		return false;
	}

	//{{ 2010. 01. 04  최육사	PC방 프리미엄
#ifdef SERV_PC_BANG_PRE

	// 4. PC방 전용 아이템인지 검사
	if( IsDBUpdateItem( iSupportItemUID ) == false  ||  IsDBUpdateItem( iTargetItemUID ) == false )
	{
		START_LOG( cerr, L"PC방 프리미엄 아이템에 소켓을 박을려고 했습니다." )
			<< BUILD_LOG( iSupportItemUID )
			<< BUILD_LOG( iTargetItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PC_BANG_00 );
		return false;
	}

#endif SERV_PC_BANG_PRE
	//}}

	// 5. 타겟 아이템에 현재 랜덤소켓이 박혀있는지 검사
	int iAssignedSocketCount = 0;
	if( GetCountAssignedRandomSocket( iTargetItemUID, iAssignedSocketCount ) == false )
	{
		START_LOG( cerr, L"해당 아이템에서 랜덤 소켓 정보를 얻지 못하였습니다." )
			<< BUILD_LOG( iSupportItemUID )
			<< BUILD_LOG( iTargetItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_00 );
		return false;
	}

	if( iAssignedSocketCount <= 0 )
	{
		SET_ERROR( ERR_EVALUATE_ITEM_01 );
		return false;
	}

	// 6. 서포트 아이템이 미라클 큐브가 맞는지 검사
	const int iMiracleCubeItemID = GetItemID( iSupportItemUID );
	const int iTargetItemID = GetItemID( iTargetItemUID );

	// 7. 아이템 정보 확인
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTargetItemID );
	if( IS_NULL( pItemTemplet ) )
	{
		START_LOG( cerr, L"아이템 정보가 존재 하지 않습니다." )
			<< BUILD_LOG( iTargetItemID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_00 );
		return false;
	}

	// 8. 사용 레벨 조건이 맞는지 검사
	if( SiCXSLSocketItem()->IsCanUseMiracleCube( iMiracleCubeItemID, pItemTemplet->m_UseLevel ) == false )
	{
		SET_ERROR( ERR_EVALUATE_ITEM_05 );
		return false;
	}

	// 수량 확인할 컨테이너
	std::map< UidType, int > mapRequireItemQuantity;
	mapRequireItemQuantity.insert( std::make_pair( iSupportItemUID, 1 ) );

	// 9. 미라클 큐브 수량 검사
	if( IsEnoughItemExist( mapRequireItemQuantity ) == false )
	{
		START_LOG( cerr, L"미라클 큐브 수량이 부족합니다." )
			<< BUILD_LOG( iSupportItemUID )
			<< BUILD_LOG( iMiracleCubeItemID )
			<< BUILD_LOG( mapRequireItemQuantity.size() )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_06 );
		return false;
	}

	// 10. 미라클 큐브 삭제
	std::map< UidType, int >::const_iterator mitMD;
	for( mitMD = mapRequireItemQuantity.begin(); mitMD != mapRequireItemQuantity.end(); ++mitMD )
	{
		// 해당 마법석 삭제
		int iCategory = 0;
		int iSlotID = 0;
		int iDecreased = 0;
		GetCategorySlot( mitMD->first, iCategory, iSlotID );
		DecreaseQuantity( mitMD->first, mitMD->second, iDecreased, KDeletedItemInfo::DR_SOCKET_MAGIC_STONE );
		LIF( iDecreased == mitMD->second );

		KInventoryItemInfo kInfo;
		GetInventorySlotInfo( iCategory, iSlotID, kInfo );
		vecInventorySlotInfo.push_back( kInfo );
	}

	SET_ERROR( NET_OK );
	return true;
}

bool KInventory::RestoreItemEvaluateResult( IN const UidType iTargetItemUID, IN OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	// 1. 타겟 아이템의 랜덤 소켓 정보를 초기화 시키자!
	std::vector< int > vecEmptyRandomSocket;
	if( SetRandomSocketOption( iTargetItemUID, vecEmptyRandomSocket ) == false )
	{
		START_LOG( cerr, L"아이템 미감정을 실패하였습니다.!" )
			<< BUILD_LOG( iTargetItemUID )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_04 );
		return false;
	}

	// 2. 업데이트 되고 나서 해당 슬롯 정보 얻기
	KInventoryItemInfo kInfo;
	LIF( GetInventoryItemInfo( iTargetItemUID, kInfo ) );
	vecInventorySlotInfo.push_back( kInfo );
	return true;
}

bool KInventory::ItemConvert( IN const UidType& iItemUID, 
                             IN const int& iItemQuantity_,
							 OUT std::map< int, int >& mapInsertedItem_, 
							 OUT std::vector< KItemInfo >& vecNewItem, 
							 OUT std::vector< KInventoryItemInfo >& vecUpdated, 
							 OUT int& iCommissionED )
{
	mapInsertedItem_.clear();
	vecUpdated.clear();
	vecNewItem.clear();
	iCommissionED = 0;

	std::map< UidType, int > mapToDeleteByItemUID;
	std::map< int, KItemInfo > mapResultItem;  // 결과템
	KInventoryItemInfo kSrcItemInfo;

	SET_ERROR( NET_OK );

	// 실제로 해당 아이템이 존재하는지 체크!
	if( GetInventoryItemInfo( iItemUID, kSrcItemInfo ) == false )
	{
		START_LOG( cerr, L"해당 아이템이 인벤토리에 없다!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_EXCHANGE_01 );
		return false;
	}

	// 기간제 아이템은 교환할 수 없다!
	if( kSrcItemInfo.m_kItemInfo.m_sPeriod != 0 )
	{
		START_LOG( cerr, L"기간제 아이템은 교환할 수 없다!" )
			<< BUILD_LOG( iItemUID )
			<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_sPeriod )
			<< END_LOG;

		SET_ERROR( ERR_EVALUATE_ITEM_07 );
		return false;
	}

	// 장비니깐 수량은 무조건 1개다 -> 2013-08-21  여러개 아이템 교환 기능으로 수정합니다. woosanghyuk.
	//mapToDeleteByItemUID.insert( std::make_pair( iItemUID, 1 ) );
    mapToDeleteByItemUID.insert( std::make_pair( iItemUID, iItemQuantity_ ) );

	if( IsEnoughItemExist( mapToDeleteByItemUID ) == false ) // map = ItemUID, quantity
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_EXCHANGE_01 );
		return false;
	}

	// 2. 교환 결과 얻기
	int iResultItemID = 0;
	if( SiCXSLItemManager()->GetResultItemConvert( kSrcItemInfo.m_kItemInfo.m_iItemID, iResultItemID, iCommissionED ) == false )
	{
		SET_ERROR( ERR_EVALUATE_ITEM_07 );
		return false;
	}

    if ( iItemQuantity_ > 1 ) // 아이템 수량이 여러개 일 때 
    {
        iCommissionED = iCommissionED * iItemQuantity_;
    }
    
	// 3. 수수료 검사
	if( m_pUser->GetED() < iCommissionED )
	{
		SET_ERROR( ERR_EVALUATE_ITEM_08 );
		return false;
	}

	// 3. 아이템 교환 정보 세팅
	{
		KItemInfo kNewItemInfo = kSrcItemInfo.m_kItemInfo;	// 기존의 아이템 정보를 카피하고

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 해외팀 주석 추가
		// 오래된 어둠의 문 아이템이라면 소켓 옵션 정보를 날려버리자
		if( SiCXSLDefenceDungeonManager()->IsOldDefenceItemID( kSrcItemInfo.m_kItemInfo.m_iItemID ) == true )
		{
            kNewItemInfo.m_vecItemSocket.clear();
		}
#endif // SERV_NEW_DEFENCE_DUNGEON

		kNewItemInfo.m_iItemID = iResultItemID;				// itemid값만 덮어쓰자
		mapResultItem.insert( std::make_pair( iResultItemID, kNewItemInfo ) );

		// 교환 대상 아이템이 들어갈 빈슬롯이 있는지 찾기위한 컨테이너
		mapInsertedItem_.insert( std::make_pair( iResultItemID, iItemQuantity_ ) );
	}

	// 결과물의 카테고리 별 필요한 빈슬롯 수를 구하자
	if( IsEnoughSpaceExist( mapInsertedItem_ ) == false )
	{
		SET_ERROR( ERR_PERSONAL_SHOP_51 );
		return false;
	}

	// 4. 결과 처리
	if( DeleteAndInsert( mapToDeleteByItemUID, mapResultItem, vecUpdated, vecNewItem, KDeletedItemInfo::DR_EXCHANGE ) == false )
	{
		SET_ERROR( ERR_ITEM_EXCHANGE_00 );
		return false;
	}

	// 5. 수수료 깎기
	m_pUser->DecreaseED( iCommissionED );

	// 아이템 교환 통계
	KStatisticsKey kKeyRandom;
	kKeyRandom.m_vecIntKey.push_back( kSrcItemInfo.m_kItemInfo.m_iItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyRandom, KStatistics::SI_ITEM_EXCHANGE_S, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyRandom, KStatistics::SI_ITEM_EXCHANGE_S, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	// 아이템 교환 통계
	std::map<int,int>::const_iterator mit;
	for( mit = mapInsertedItem_.begin(); mit != mapInsertedItem_.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_EXCHANGE_R, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_EXCHANGE_R, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	return true;
}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
bool KInventory::CheckKeepItem( const int nProductID )
{
	std::map< int, std::vector<int> >::iterator mit; //아이템 넘버
	mit = SiCXSLItemManager()->m_mapKeepShowItem.find(nProductID);
	if( mit != SiCXSLItemManager()->m_mapKeepShowItem.end() )
	{
		std::vector<int> TempVecter = mit->second;
			
		for( int i = 0; i < TempVecter.size(); ++i)
		{
			if( IsExistOnlyInventory(TempVecter[i]) == false )
			{
				return false;
			}
		}
		return true;		
	}		
	
	return false;
}
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP


#ifdef SERV_SHARE_BANK_ITEM_EVALUATE_FIX
char KInventory::GetItemState( IN const UidType iItemUID ) const
{
	std::map< UidType, KInventoryItem >::const_iterator mit;
	mit = m_mapItem.find( iItemUID );

	if( mit == m_mapItem.end() )
	{
		START_LOG( cerr, L"존재하지 않는 아이템입니다." )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;
		return KItemInfo::IS_INIT;
	}

	return mit->second.GetItemState();
}
#endif //SERV_SHARE_BANK_ITEM_EVALUATE_FIX

#ifdef SERV_RECRUIT_EVENT_BASE
void KInventory::DeleteItemAll( IN int iItemID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo, IN KDeletedItemInfo::DELETE_REASON eReason )
{
	vecInventorySlotInfo.clear();

	std::vector< UidType > vecDeleteItemUIDList;
	GetItemUIDListThisItem( iItemID, vecDeleteItemUIDList, false );

	BOOST_TEST_FOREACH( const UidType&, iDeleteItemUID, vecDeleteItemUIDList )
	{
		KItemInfo kDeleteItemInfo;
		LIF( GetItemInfo( iDeleteItemUID, kDeleteItemInfo ) );		

		KInventoryItemInfo kInventorySlotInfo;
		LIF( DeleteItem( iDeleteItemUID, kInventorySlotInfo, eReason ) );
		vecInventorySlotInfo.push_back( kInventorySlotInfo );

		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( kDeleteItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_DISCARD, kDeleteItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_DISCARD, kDeleteItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
}
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
bool KInventory::IsPossibleExtractItem( const int iItemID )
{
	return SiCXSLAttribEnchantItem()->IsExtractItem( iItemID );
}

bool KInventory::ItemExtract( IN const UidType iSourceItemUID, 
				 IN const int iSourceQuantity, 
				 OUT std::map< int, int >& mapInsertedItem, 
				 OUT std::vector< KInventoryItemInfo >& vecUpdated, 
				 OUT std::vector< KItemInfo >& vecNewItem	)
{
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();

	std::map< int, int > mapToDeleteByItemID;
	std::map< UidType, int > mapToDeleteByItemUID;
	std::map< int, KItemInfo > mapResultItem;  // 결과템
	KInventoryItemInfo kSrcItemInfo;

	SET_ERROR( NET_OK );

	// 실제로 해당 아이템이 존재하는지 체크!
	if( GetInventoryItemInfo( iSourceItemUID, kSrcItemInfo ) == false )
	{
		START_LOG( cerr, L"해당 아이템이 인벤토리에 없다!" )
			<< BUILD_LOG( iSourceItemUID )
			<< END_LOG;

		// 추출 아이템 정보가 이상합니다. 
		SET_ERROR( ERR_EXTRACT_01 );
		return false;
	}

	// 존재 한다면 수량은 충분한지 체크!
	if( kSrcItemInfo.m_kItemInfo.m_iQuantity < iSourceQuantity )
	{
		START_LOG( cerr, L"해당 아이템의 수량이 충분하지 않다!" )
			<< BUILD_LOG( iSourceItemUID )
			<< BUILD_LOG( iSourceQuantity )
			<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_iQuantity )
			<< END_LOG;

		// 아이템 수량이 이상합니다.
		SET_ERROR( ERR_EXTRACT_02 );
		return false;
	}

	// 기간제 아이템은 추출 할 수 없다!
	if( kSrcItemInfo.m_kItemInfo.m_sPeriod != 0 )
	{
		SET_ERROR( ERR_EXTRACT_03 );	// 기간제 아이템은 추출 할 수 없습니다.
		return false;
	}

	mapToDeleteByItemID.insert( std::make_pair( kSrcItemInfo.m_kItemInfo.m_iItemID, iSourceQuantity ) );

	if( IsEnoughItemExist( mapToDeleteByItemID, true, true ) == false )
	{
		if( IsEnoughItemExist( mapToDeleteByItemID, true, false ) == true )
		{
			SET_ERROR( ERR_EXTRACT_03 );	// 기간제 아이템은 추출 할 수 없습니다.
		}
		else
		{
			START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
				<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOG( iSourceQuantity )
				<< END_LOG;

			SET_ERROR( ERR_EXTRACT_01 );
		}

		return false;
	}

	// 추출 시 얻게 되는 아이템
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( CXSLAttribEnchantItem::ATI_ESSENCE );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( CXSLAttribEnchantItem::ATI_ESSENCE )
			<< END_LOG;

		SET_ERROR( ERR_EXTRACT_01 );

		return false;
	}

	KItemInfo kInsertItemInfo;
	kInsertItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
	kInsertItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
	kInsertItemInfo.m_iQuantity		= iSourceQuantity * 3;
	kInsertItemInfo.m_sEndurance	= pItemTemplet->m_Endurance;

	mapResultItem.insert( std::make_pair( kInsertItemInfo.m_iItemID, kInsertItemInfo ) );

	// 3. 습득 아이템 정보
	std::map< int, KItemInfo >::const_iterator mitRI;
	for( mitRI = mapResultItem.begin(); mitRI != mapResultItem.end(); ++mitRI )
	{
		mapInsertedItem.insert( std::make_pair( mitRI->first, mitRI->second.m_iQuantity ) );
	}

	// 공간 확인
	if( IsEnoughSpaceExist( mapInsertedItem ) == false )
	{
		SET_ERROR( ERR_EXTRACT_05 );
		return false;
	}

	// 4. 결과 처리
	if( DeleteAndInsert( mapToDeleteByItemID
		, mapToDeleteByItemUID
		, mapResultItem
		, vecUpdated
		, vecNewItem
		, true
		, KDeletedItemInfo::DR_ITEM_EXTRACT
		, true
		) == false 	)
	{
		SET_ERROR( ERR_EXTRACT_04 );
		return false;
	}

	// 아이템 추출 통계
	KStatisticsKey kKeyRandom;
	kKeyRandom.m_vecIntKey.push_back( kSrcItemInfo.m_kItemInfo.m_iItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyRandom, KStatistics::SI_ITEM_EXTRACT_S, iSourceQuantity );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyRandom, KStatistics::SI_ITEM_EXTRACT_S, iSourceQuantity );
	//}}

	// 아이템 추출 통계
	std::map<int,int>::const_iterator mit;
	for( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_EXTRACT_R, mit->second );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_EXCHANGE_R, mit->second );
		//}}
	}

	return true;
}

bool KInventory::UseFinalitySkill( IN const UidType iSourceItemUID, 
							 IN const int iSourceQuantity, 
							 OUT std::map< int, int >& mapInsertedItem, 
							 OUT std::vector< KInventoryItemInfo >& vecUpdated, 
							 OUT std::vector< KItemInfo >& vecNewItem	)
{
	mapInsertedItem.clear();
	vecUpdated.clear();
	vecNewItem.clear();

	std::map< int, int > mapToDeleteByItemID;
	std::map< UidType, int > mapToDeleteByItemUID;
	std::map< int, KItemInfo > mapResultItem;  // 결과템
	KInventoryItemInfo kSrcItemInfo;

	SET_ERROR( NET_OK );

	// 실제로 해당 아이템이 존재하는지 체크!
	if( GetInventoryItemInfo( iSourceItemUID, kSrcItemInfo ) == false )
	{
		START_LOG( cerr, L"해당 아이템이 인벤토리에 없다!" )
			<< BUILD_LOG( iSourceItemUID )
			<< END_LOG;

		// 추출 아이템 정보가 이상합니다. 
		SET_ERROR( ERR_FINALITY_SKILL_01 );
		return false;
	}

	// 존재 한다면 수량은 충분한지 체크!
	if( kSrcItemInfo.m_kItemInfo.m_iQuantity < iSourceQuantity )
	{
		START_LOG( cerr, L"해당 아이템의 수량이 충분하지 않다!" )
			<< BUILD_LOG( iSourceItemUID )
			<< BUILD_LOG( iSourceQuantity )
			<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_iQuantity )
			<< END_LOG;

		// 아이템 수량이 이상합니다.
		SET_ERROR( ERR_FINALITY_SKILL_02 );
		return false;
	}

	mapToDeleteByItemID.insert( std::make_pair( kSrcItemInfo.m_kItemInfo.m_iItemID, iSourceQuantity ) );

	if( IsEnoughItemExist( mapToDeleteByItemID, true, true ) == false )
	{
		START_LOG( cerr, L"인벤토리에 없는 아이템ID인데?" )
			<< BUILD_LOG( kSrcItemInfo.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( iSourceQuantity )
			<< END_LOG;

		SET_ERROR( ERR_FINALITY_SKILL_01 );
		return false;
	}

	// 결과 처리
	if( DeleteAndInsert( mapToDeleteByItemID
		, mapToDeleteByItemUID
		, mapResultItem
		, vecUpdated
		, vecNewItem
		, true
		, KDeletedItemInfo::DR_USE_FINALITY_SKILL
		, true
		) == false 	)
	{
		SET_ERROR( ERR_FINALITY_SKILL_03 );
		return false;
	}

	// 아이템 추출 통계
	KStatisticsKey kKeyRandom;
	kKeyRandom.m_vecIntKey.push_back( kSrcItemInfo.m_kItemInfo.m_iItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyRandom, KStatistics::SI_ITEM_USE_FINALITY_SKILL, iSourceQuantity );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyRandom, KStatistics::SI_ITEM_USE_FINALITY_SKILL, iSourceQuantity );
	//}}

	return true;
}
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_GOOD_ELSWORD
int KInventory::GetNextUpgradeBankED( IN const int& iNextGrade_ )
{
    return SiCXSLEDInventoryExpand()->GetNextBankUpgradeED( iNextGrade_ );
}

int KInventory::GetNextUpgradeInventoryED( IN const int& iNextGrade_ )
{
    return SiCXSLEDInventoryExpand()->GetNextInventoryUpgradeED( iNextGrade_ );
}

#endif //SERV_GOOD_ELSWORD

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
bool KInventory::UpdateExpandedSocketNum( IN UidType iItemUID, IN const byte byteExpandedSocketNum )
{
	std::map< UidType, KInventoryItem >::iterator it = m_mapItem.find( iItemUID );
	if( it == m_mapItem.end() )
	{
		return false;
	}

	it->second.UpdateExpandedSocketNum( byteExpandedSocketNum );

	return true;
}
#endif // SERV_BATTLE_FIELD_BOSS