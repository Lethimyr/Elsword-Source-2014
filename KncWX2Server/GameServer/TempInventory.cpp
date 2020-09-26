#include "TempInventory.h"

#include "X2Data/XSLItemManager.h"
#include "NetError.h"

KTempInventory::KTempInventory() : 
m_iTempItemUID( 0 )
{
	Clear();
}

KTempInventory::~KTempInventory()
{
}

void KTempInventory::Clear()
{
	m_iTempItemUID = 0;
	m_mapTempInventory.clear();	
}

//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
bool KTempInventory::AddTempInventoryItem( IN const int iItemID
										 , IN const u_char ucSealData
										 //{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
										 , IN const KItemInfo& kItemInfo
#endif SERV_ENCHANT_ITEM_DROP_EVENT
										 //}}
										 , OUT KTempInventoryItemInfo& kTempItemInfo
										 )
#else
bool KTempInventory::AddTempInventoryItem( int iItemID, u_char ucSealData, int iDropItemUID, KTempInventoryItemInfo& kTempItemInfo )
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
	{
		std::map< int, KTempInventoryItemInfo >::iterator mit;
		for( mit = m_mapTempInventory.begin(); mit != m_mapTempInventory.end(); ++mit )
		{
			if( mit->second.m_iItemID != iItemID )
				continue;

			if( mit->second.m_iQuantity >= pItemTemplet->m_Quantity )
				continue;

			// 수량 증가
			++mit->second.m_iQuantity;

			// 임시 인벤 아이템 정보
			kTempItemInfo = mit->second;
			break;
		}

		if( mit == m_mapTempInventory.end() )
		{
			KTempInventoryItemInfo kInfo;
			//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			kInfo.m_iTempItemUID = GetNextTempItemUID();
#else
			kInfo.m_iTempItemUID = iDropItemUID;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
			//}}
			kInfo.m_iItemID = iItemID;
			kInfo.m_iQuantity = 1;
			//{{ 2009. 11. 18  최육사	특정시각드롭이벤트
			kInfo.m_ucSealData = 0; // 수량성은 밀봉될수없음
			//}}
			//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
			// 장비가 수량성은 아니지만... 일단은 작업 해두자
			kInfo.m_cEnchantLevel	= kItemInfo.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
			m_mapTempInventory.insert( std::make_pair( kInfo.m_iTempItemUID, kInfo ) );

			// 임시 인벤 아이템 정보
			kTempItemInfo = kInfo;
		}
	}
	else
	{
		KTempInventoryItemInfo kInfo;
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		kInfo.m_iTempItemUID = GetNextTempItemUID();
#else
		kInfo.m_iTempItemUID = iDropItemUID;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		kInfo.m_iItemID = iItemID;
		kInfo.m_iQuantity = 1;
		//{{ 2009. 11. 18  최육사	특정시각드롭이벤트
		kInfo.m_ucSealData = ucSealData;
		//}}
		//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		kInfo.m_cEnchantLevel	= kItemInfo.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
		m_mapTempInventory.insert( std::make_pair( kInfo.m_iTempItemUID, kInfo ) );

		// 임시 인벤 아이템 정보
		kTempItemInfo = kInfo;
	}

	return true;
}

bool KTempInventory::PrepareTempItemToInventory( IN int iTempItemUID, OUT int& iGetItemID, OUT std::map< int, KItemInfo >& mapGetItem )
{
	mapGetItem.clear();

	std::map< int, KTempInventoryItemInfo >::iterator mit;
	mit = m_mapTempInventory.find( iTempItemUID );
	if( mit == m_mapTempInventory.end() )
	{
		START_LOG( cerr, L"존재하지 않는 임시 저장 아이템을 가져오려 했습니다." )
			<< BUILD_LOG( iTempItemUID )
			<< BUILD_LOG( static_cast<int>( m_mapTempInventory.size() ) )
			<< END_LOG;

		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->second.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( mit->second.m_iItemID )
			<< END_LOG;

		return false;
	}

	// 획득하려는 아이템의 ItemID를 담아준다.
	iGetItemID = mit->second.m_iItemID;

	KItemInfo kItemInfo;
	kItemInfo.m_iItemID		  = mit->second.m_iItemID;
	kItemInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
	kItemInfo.m_iQuantity	  = mit->second.m_iQuantity;
	kItemInfo.m_sEndurance	  = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
	//{{ 2009. 11. 18  최육사	특정시각드롭이벤트
	kItemInfo.m_ucSealData	  = ( pItemTemplet->m_PeriodType != CXSLItem::PT_QUANTITY ) ? mit->second.m_ucSealData : 0;
	//}}
	//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	kItemInfo.m_cEnchantLevel = mit->second.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}
	mapGetItem.insert( std::make_pair( mit->second.m_iItemID, kItemInfo ) );

	return true;
}

bool KTempInventory::DeleteTempItem( IN int iTempItemUID )
{
	std::map< int, KTempInventoryItemInfo >::iterator mit;
	mit = m_mapTempInventory.find( iTempItemUID );
	if( mit == m_mapTempInventory.end() )
	{
		START_LOG( cerr, L"존재하지 않는 임시 저장 아이템을 삭제하려고 했습니다." )
			<< BUILD_LOG( iTempItemUID )
			<< END_LOG;

		return false;
	}

	m_mapTempInventory.erase( mit );
	return true;
}


			