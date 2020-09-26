#include "InventoryItem.h"

#include "X2Data/XSLItem.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLEnchantItemManager.h"
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLAttribEnchantItem.h"


KInventoryItem::KInventoryItem()
	: m_iItemUID( 0 )
	, m_iItemID( 0 )
	, m_iUsageType( 0 )
	, m_iEnchantLevel( 0 )
	, m_ucSealData( 0 )
	, m_sPeriod( 0 )
//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	, m_cItemState( 0 )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}
	, m_bIsDBUpdate( true )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	, m_byteExpandedSocketNum( 0 )
#endif // SERV_BATTLE_FIELD_BOSS
{
}

KInventoryItem::~KInventoryItem()
{
}

// 캐릭터 선택할 때 DB에서 받아온 보유 아이템은 bNewItem을 false로 한다.
// 구매나 드랍으로 아이템 획득시 DB에 추가된 아이템을 인벤토리에 추가할 때는 bNewItem을 true로 한다.
// 구매나 드랍으로 DB에 아이템이 추가될 때 인벤토리의 어느 슬롯에 들어갈 지는 정해지지 않은 시점이므로
// 슬롯 카테고리와 슬롯 ID는 모두 0으로 기록된다. 따라서 새 아이템이 추가될 때는 m_iInventoryCategory 와 m_iSlotID 의
// 초기값을 0으로 해주는 것이다.
void KInventoryItem::Init( IN const KInventoryItemInfo& kInfo, IN const bool bNewItem )
{
    m_iItemUID = kInfo.m_iItemUID;
    m_iItemID = kInfo.m_kItemInfo.m_iItemID;
    m_iUsageType = static_cast<int>(kInfo.m_kItemInfo.m_cUsageType);
    if( m_iUsageType == CXSLItem::PT_QUANTITY )
    {
        m_iQuantity.SetValue( kInfo.m_kItemInfo.m_iQuantity );
    }
    else
    {
        m_iQuantity.SetValue( 1 );
    }
    if( m_iUsageType == CXSLItem::PT_ENDURANCE )
    {
	    m_iEndurance.SetValue( kInfo.m_kItemInfo.m_sEndurance );
    }
    else
    {
        m_iEndurance.SetValue( 0 );
    }
	m_iEnchantLevel		 = static_cast<int>( kInfo.m_kItemInfo.m_cEnchantLevel );
	m_kAttribEnchantInfo = kInfo.m_kItemInfo.m_kAttribEnchantInfo;
	m_vecItemSocket		 = kInfo.m_kItemInfo.m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
	m_byteExpandedSocketNum	= kInfo.m_kItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	m_vecRandomSocket	 = kInfo.m_kItemInfo.m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	m_ucSealData		 = kInfo.m_kItemInfo.m_ucSealData;
    m_sPeriod			 = kInfo.m_kItemInfo.m_sPeriod;
    m_wstrExpirationDate = kInfo.m_kItemInfo.m_wstrExpirationDate;

    if( bNewItem )
    {
        m_iInventoryCategory.SetValue( 0 );
        m_iInventoryCategory += static_cast<int>(kInfo.m_cSlotCategory);
        m_iSlotID.SetValue( 0 );
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		m_iSlotID += static_cast<int>(kInfo.m_sSlotID);
#else
		m_iSlotID += static_cast<int>(kInfo.m_cSlotID);
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
    }
    else
    {
        m_iInventoryCategory.SetValue( kInfo.m_cSlotCategory );
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		m_iSlotID.SetValue( kInfo.m_sSlotID );
#else
		m_iSlotID.SetValue( kInfo.m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
    }
	//{{ 2010. 8. 4	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	m_iPetUID.SetValue( 0 );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	CheckAndUpdateItemState();
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	m_byteExpandedSocketNum = kInfo.m_kItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
}

int KInventoryItem::GetQuantity() const
{
    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        if( m_iQuantity != 1 )
        {
            START_LOG( cerr, L"수량 아이템이 아닌데 수량 값이 1이 아님." )
                << BUILD_LOG( m_iItemUID )
                << BUILD_LOG( m_iItemID )
                << BUILD_LOG( m_iUsageType )
                << BUILD_LOG( m_iQuantity )
                << END_LOG;

			// 아이템값을 보정해버리네.. 생각해보고 남겨둘지 결정하자.
            //m_iQuantity.SetValue( 1 );
        }
    }

    return m_iQuantity;
}

int KInventoryItem::GetEndurance() const
{
    if( m_iUsageType != CXSLItem::PT_ENDURANCE )
    {
        if( m_iEndurance != 0 )
        {
            START_LOG( cerr, L"내구도 아이템이 아닌데 내구도 값이 0이 아님." )
                << BUILD_LOG( m_iItemUID )
                << BUILD_LOG( m_iItemID )
                << BUILD_LOG( m_iUsageType )
                << BUILD_LOG( m_iEndurance )
                << END_LOG;

			// 아이템값을 보정해버리네.. 생각해보고 남겨둘지 결정하자.
            //m_iEndurance.SetValue( 0 );
        }
    }

    return m_iEndurance;
}

int KInventoryItem::GetEnchantLevel() const
{
    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return 0;
    }

    if( !pItemTemplet->m_bCanEnchant )
    {
        return 0;
    }

	return m_iEnchantLevel;
}

void KInventoryItem::GetAttribEnchant( OUT KItemAttributeEnchantInfo& kInfo ) const
{
	kInfo = m_kAttribEnchantInfo;
}

void KInventoryItem::GetAttribEnchant( IN int iAttribEnchantSlotID, OUT int& iAttribEnchantID ) const
{
	iAttribEnchantID = 0;

	switch( iAttribEnchantSlotID )
	{
	case CXSLAttribEnchantItem::ESI_SLOT_1: iAttribEnchantID = m_kAttribEnchantInfo.m_cAttribEnchant0; break;
	case CXSLAttribEnchantItem::ESI_SLOT_2: iAttribEnchantID = m_kAttribEnchantInfo.m_cAttribEnchant1; break;
	case CXSLAttribEnchantItem::ESI_SLOT_3: iAttribEnchantID = m_kAttribEnchantInfo.m_cAttribEnchant2; break;
	}
}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
const std::vector< int >& KInventoryItem::GetItemSocketList() const
#else
const std::vector< short >& KInventoryItem::GetItemSocketList() const
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;
	}

	return m_vecItemSocket;
}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventoryItem::GetItemSocketOption( IN const int iSocketIndex, OUT int& iSocketOption )
#else
bool KInventoryItem::GetItemSocketOption( IN const int iSocketIndex, OUT short& sSocketOption )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;
		return false;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	int iMaxSocketCount = GetMaxSocketCount();
#else // SERV_BATTLE_FIELD_BOSS
	int iMaxSocketCount = SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType );
#endif // SERV_BATTLE_FIELD_BOSS

	// 소켓 인덱스 검사
	if( iSocketIndex < 0 || iSocketIndex >= iMaxSocketCount )
	{
		START_LOG( cerr, L"아이템의 소켓 인덱스가 이상함." )
			<< BUILD_LOG( m_iItemUID )
			<< BUILD_LOG( m_iItemID )
			<< BUILD_LOG( pItemTemplet->m_ItemGrade )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< BUILD_LOG( iMaxSocketCount )
			<< BUILD_LOG( iSocketIndex )
			<< END_LOG;
		return false;
	}

	// 얻을 인덱스에 비해 소켓 슬롯수가 부족함!
	if( static_cast<int>(m_vecItemSocket.size()) <= iSocketIndex )
	{
		int iEmptyCount = iSocketIndex - m_vecItemSocket.size() + 1;
		if( iEmptyCount > 0 )
		{
			for( int idx=0; idx < iEmptyCount; idx++ )
			{
				m_vecItemSocket.push_back( 0 );
			}
		}
	}

	_JIF( 0 <= iSocketIndex && static_cast<int>(m_vecItemSocket.size()) > iSocketIndex, return false; );
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	iSocketOption = m_vecItemSocket[iSocketIndex];
#else
	sSocketOption = m_vecItemSocket[iSocketIndex];
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	return true;
}

int KInventoryItem::GetCountAssignedItemSocket() const
{
	int iSocketCount = 0;
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >::const_iterator vit;
#else
	std::vector< short >::const_iterator vit;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	for( vit = m_vecItemSocket.begin(); vit != m_vecItemSocket.end(); ++vit )
	{
		if( *vit > 0 )
			++iSocketCount;
	}
	
	return iSocketCount;
}

//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
const std::vector< int >& KInventoryItem::GetRandomSocketList() const
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( GetItemID() );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( GetItemID() )
			<< END_LOG;
	}

	return m_vecRandomSocket;
}

bool KInventoryItem::GetRandomSocketOption( IN const int iSocketIndex, OUT int& iSocketOptionID )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( GetItemID() );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( GetItemID() )
			<< END_LOG;
		return false;
	}

	// 소켓 인덱스 검사
	if( iSocketIndex < 0 || iSocketIndex >= CXSLSocketItem::RSC_MAX )
	{
		START_LOG( cerr, L"아이템의 소켓 인덱스가 이상함." )
			<< BUILD_LOG( GetItemUID() )
			<< BUILD_LOG( GetItemID() )
			<< BUILD_LOG( pItemTemplet->m_ItemGrade )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< BUILD_LOG( iSocketIndex )
			<< END_LOG;
		return false;
	}

	// 얻을 인덱스에 비해 소켓 슬롯수가 부족함!
	if( static_cast<int>(m_vecRandomSocket.size()) <= iSocketIndex )
	{
		int iEmptyCount = iSocketIndex - m_vecRandomSocket.size() + 1;
		if( iEmptyCount > 0 )
		{
			for( int idx=0; idx < iEmptyCount; ++idx )
			{
				m_vecRandomSocket.push_back( 0 );
			}
		}
	}

	_JIF( 0 <= iSocketIndex && static_cast<int>(m_vecRandomSocket.size()) > iSocketIndex, return false; );
	
	iSocketOptionID = m_vecRandomSocket[iSocketIndex];
	return true;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
int KInventoryItem::GetCountAssignedRandomSocket() const
{
	int iSocketCount = 0;

	BOOST_TEST_FOREACH( const int, iSocketOptionID, m_vecRandomSocket )
	{
		if( iSocketOptionID > 0 )
			++iSocketCount;
	}

	return iSocketCount;
}

void KInventoryItem::SetRandomSocketOption( IN const std::vector< int > vecRandomSocket )
{
    m_vecRandomSocket = vecRandomSocket;

	CheckAndUpdateItemState();
}

void KInventoryItem::CheckAndUpdateItemState()
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( GetItemID() );
	if( IS_NULL( pItemTemplet ) )
	{
		START_LOG( cerr, L"스크립트에 없는 itemid입니다." )
			<< BUILD_LOG( GetItemID() )
			<< END_LOG;

		m_cItemState = KItemInfo::IS_NORMAL;
		return;
	}

	// 랜덤 소켓 아이템이면서 현재 랜덤소켓이 하나도 안박혀있다면 아이템의 상태는 "미감정" 상태이다!
	if( pItemTemplet->m_vecRandomSocketGroupID.empty() == false  &&  m_vecRandomSocket.empty() == true )
	{
		m_cItemState = KItemInfo::IS_NOT_EVALUATED;
		return;
	}

	m_cItemState = KItemInfo::IS_NORMAL;
}

#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

//{{ 2009. 8. 27  최육사	밀봉
u_char KInventoryItem::GetSealCount() const
{
    if( m_ucSealData >= 100 )
	{
		return ( m_ucSealData - 100 );
	}
	else
	{
		return m_ucSealData;
	}
}
//}}

void KInventoryItem::GetItemInfo( KItemInfo& kInfo ) const
{
    kInfo.m_iItemID				= GetItemID();
    kInfo.m_cUsageType			= static_cast<char>( GetUsageType() );
    kInfo.m_iQuantity			= GetQuantity();
	kInfo.m_sEndurance			= static_cast<short>( GetEndurance() );
    kInfo.m_sPeriod				= m_sPeriod;
    kInfo.m_wstrExpirationDate	= m_wstrExpirationDate;
	kInfo.m_cEnchantLevel		= static_cast<char>( GetEnchantLevel() );
	kInfo.m_kAttribEnchantInfo	= m_kAttribEnchantInfo;
	kInfo.m_vecItemSocket		= GetItemSocketList();
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	kInfo.m_byteExpandedSocketNum	= m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	kInfo.m_vecRandomSocket		= GetRandomSocketList();
	kInfo.m_cItemState			= GetItemState();
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	kInfo.m_ucSealData			= GetSealData();

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	kInfo.m_byteExpandedSocketNum	= m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
}

void KInventoryItem::GetInventoryItemInfo( OUT KInventoryItemInfo& kInfo ) const
{
    kInfo.m_iItemUID	  = GetItemUID();
    kInfo.m_cSlotCategory = static_cast<char>( GetInventoryCategory() );
	//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	kInfo.m_sSlotID		  = static_cast<short>( GetSlotID() );
#else
	kInfo.m_cSlotID		  = static_cast<char>( GetSlotID() );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}
    GetItemInfo( kInfo.m_kItemInfo );
}

//{{ 2009. 10. 12  최육사	코드정리
void KInventoryItem::GetInventoryItemSimpleInfo( OUT KInventoryItemSimpleInfo& kInfo ) const
{
	kInfo.m_iItemUID		= GetItemUID();
	kInfo.m_iItemID			= GetItemID();
	kInfo.m_cSlotCategory	= static_cast<char>( GetInventoryCategory() );
	//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	kInfo.m_sSlotID			= static_cast<short>( GetSlotID() );
#else
	kInfo.m_cSlotID			= static_cast<char>( GetSlotID() );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}
	kInfo.m_EnchantLevel	= static_cast<char>( GetEnchantLevel() );
	kInfo.m_kAttribEnchantInfo = m_kAttribEnchantInfo;
}
//}}

bool KInventoryItem::IsQuantityFull() const
{
    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        START_LOG( cerr, L"수량아이템이 아닌데 가득 찼는지 확인함." )
            << BUILD_LOG( m_iItemUID )
            << BUILD_LOG( m_iItemID )
            << BUILD_LOG( m_iUsageType )
            << BUILD_LOG( m_iQuantity )
			<< BUILD_LOG( m_iEndurance )
            << BUILD_LOG( m_iInventoryCategory )
            << BUILD_LOG( m_iSlotID )
            << END_LOG;

        return true;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return m_iQuantity > 1;     // 어쩌라구.. 1보다 크면 full 인정.
    }

    return m_iQuantity >= pItemTemplet->m_Quantity;
}

//{{ 2008. 12. 21  최육사	강화 깨짐
bool KInventoryItem::IsBroken() const
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;

		// 깨진걸로 처리
		return true;
	}

	// 강화 아이템이 아니라면 무조건 안깨진것으로 처리
	if( !pItemTemplet->m_bCanEnchant )
		return false;

    return ( m_iEnchantLevel < 0 );
}
//}}

//{{ 2009. 8. 27  최육사	밀봉
bool KInventoryItem::IsSealed() const
{
	return ( m_ucSealData >= 100 );
}
//}}

bool KInventoryItem::IsQuantityExhausted() const
{
    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        START_LOG( cerr, L"수량아이템이 아닌데 다썼는지 확인함." )
            << BUILD_LOG( m_iItemUID )
            << BUILD_LOG( m_iItemID )
            << BUILD_LOG( m_iUsageType )
            << BUILD_LOG( m_iQuantity )
			<< BUILD_LOG( m_iEndurance )
            << BUILD_LOG( m_iInventoryCategory )
            << BUILD_LOG( m_iSlotID )
            << END_LOG;

        return false;
    }

    return m_iQuantity <= 0;
}

bool KInventoryItem::IsPeriodExpired() const
{
	if( m_iUsageType != CXSLItem::PT_INFINITY )
	{
		return false;
	}

	// 무제한 아이템인지 검사한다.
	if( m_sPeriod <= 0 )
	{
		return false;
	}

	if( m_wstrExpirationDate.empty() )
	{
		START_LOG( cerr, L"기간제 아이템인데 만료일이 없음." )
			<< BUILD_LOG( m_iItemUID )
			<< END_LOG;

		return false;
	}

	CTime tExpirationTime;
	if( !KncUtil::ConvertStringToCTime( m_wstrExpirationDate, tExpirationTime ) )
	{
		START_LOG( cerr, L"문자열 시간 변환 실패." )
			<< BUILD_LOG( m_wstrExpirationDate )
			<< END_LOG;

		return false;
	}

	CTime tCurrentTime = CTime::GetCurrentTime();
	if( tCurrentTime > tExpirationTime )
	{
		return true;
	}

	return false;
}

int KInventoryItem::GetRemainedQuantityCapacity() const
{
    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        return 0;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return 0;
    }

    _JIF( pItemTemplet->m_Quantity - m_iQuantity >= 0, return 0 );

    return pItemTemplet->m_Quantity - m_iQuantity;
}

int KInventoryItem::GetEnduranceToRestore() const
{
    if( m_iUsageType != CXSLItem::PT_ENDURANCE )
    {
        return 0;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return 0;
    }

    _JIF( pItemTemplet->m_Endurance - m_iEndurance >= 0, return 0 );

    return pItemTemplet->m_Endurance - m_iEndurance;
}

void KInventoryItem::IncreaseQuantity( int iQuantity, int& iIncreased )
{
    iIncreased = 0;

    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        START_LOG( clog, L"수량성 아이템이 아님." )
            << BUILD_LOG( m_iUsageType )
            << END_LOG;

        return;
    }

    if( iQuantity <=  0 )
    {
        START_LOG( cerr, L"증가시키려는 수량 값이 이상함." )
            << BUILD_LOG( iQuantity )
            << END_LOG;

        return;
    }

    if( m_iQuantity <= 0 )
    {
        START_LOG( cerr, L"수량 값이 이상함." )
            << BUILD_LOG( m_iQuantity )
            << END_LOG;

        return;
    }

    int iRemainedCapacity = GetRemainedQuantityCapacity();
    iIncreased = std::min< int >( iRemainedCapacity, iQuantity );

    m_iQuantity += iIncreased;

    return;
}

void KInventoryItem::DecreaseQuantity( int iQuantity, int& iDecreased )
{
    iDecreased = 0;

    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        START_LOG( clog, L"수량성 아이템이 아님." )
            << BUILD_LOG( m_iUsageType )
            << END_LOG;

        return;
    }

    if( iQuantity <=  0 )
    {
        START_LOG( cerr, L"감소시키려는 수량 값이 이상함." )
            << BUILD_LOG( iQuantity )
            << END_LOG;

        return;
    }

    if( m_iQuantity <= 0 )
    {
        START_LOG( cerr, L"수량 값이 이상함." )
            << BUILD_LOG( m_iQuantity )
            << END_LOG;

        return;
    }

    iDecreased = std::min< int >( m_iQuantity, iQuantity );

    m_iQuantity -= iDecreased;

    return;
}

void KInventoryItem::IncreaseEndurance( int iEndurance, int& iIncreased )
{
    iIncreased = 0;

    if( m_iUsageType != CXSLItem::PT_ENDURANCE )
    {
        START_LOG( clog, L"내구도 아이템이 아님." )
            << BUILD_LOG( m_iUsageType )
            << END_LOG;

        return;
    }

    if( iEndurance <=  0 )
    {
        START_LOG( cerr, L"증가시키려는 내구도 값이 이상함." )
            << BUILD_LOG( iEndurance )
            << END_LOG;

        return;
    }

    if( m_iEndurance < 0 )
    {
        START_LOG( cerr, L"내구도 값이 이상함." )
            << BUILD_LOG( m_iEndurance )
            << END_LOG;

        return;
    }

    int iEnduranceToRestore = GetEnduranceToRestore();
    iIncreased = std::min< int >( iEnduranceToRestore, iEndurance );

    m_iEndurance += iIncreased;

    return;
}

void KInventoryItem::DecreaseEndurance( int iEndurance, int& iDecreased )
{
    iDecreased = 0;

    if( m_iUsageType != CXSLItem::PT_ENDURANCE )
    {
        START_LOG( clog, L"내구도 아이템이 아님." )
			<< BUILD_LOG( m_iItemID )
            << BUILD_LOG( m_iUsageType )
            << END_LOG;

        return;
    }

    if( iEndurance <=  0 )
    {
        START_LOG( cerr, L"감소시키려는 내구도 값이 이상함." )
			<< BUILD_LOG( m_iItemID )
            << BUILD_LOG( iEndurance )
            << END_LOG;

        return;
    }

    if( m_iEndurance < 0 )
    {
        START_LOG( cerr, L"내구도 값이 이상함." )
            << BUILD_LOG( m_iEndurance )
            << END_LOG;

        return;
    }

    iDecreased = std::min< int >( m_iEndurance, iEndurance );

    m_iEndurance -= iDecreased;

    return;
}

void KInventoryItem::IncreaseEnchantLevel( int iEnchantLevel, int& iIncreased )
{
	iIncreased = 0;

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return;
    }

    if( !pItemTemplet->m_bCanEnchant )
    {
        START_LOG( clog, L"강화할 수 있는 아이템이 아님." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return;
    }
	
	// 무조건 양수만 가능!
	if( iEnchantLevel <=  0 )
	{
		START_LOG( cerr, L"증가시키려는 강화값이 이상함." )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		return;
	}

	if( m_iEnchantLevel < -(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL)  ||  m_iEnchantLevel > CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
	{
		START_LOG( cerr, L"강화 레벨값이 범위를 벗어났다." )
			<< BUILD_LOG( m_iItemID )
			<< BUILD_LOG( iEnchantLevel )
			<< BUILD_LOG( m_iEnchantLevel )
			<< BUILD_LOG( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
			<< END_LOG;

		return;
	}
	
	// 최대값 검사
	iIncreased = std::min< int >( ( int )( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL - m_iEnchantLevel ), iEnchantLevel );

	m_iEnchantLevel += iIncreased;
	return;
}

void KInventoryItem::DecreaseEnchantLevel( int iEnchantLevel, int& iDecreased )
{
	iDecreased = 0;

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return;
    }

    if( !pItemTemplet->m_bCanEnchant )
    {
        START_LOG( clog, L"강화할 수 있는 아이템이 아님." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return;
    }

	if( iEnchantLevel <=  0 )
	{
		START_LOG( cerr, L"감소시키려는 강화 레벨값이 이상함." )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		return;
	}

	if( m_iEnchantLevel < -( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )  ||  m_iEnchantLevel > CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
	{
		START_LOG( cerr, L"강화 레벨값이 범위를 벗어났다." )
			<< BUILD_LOG( m_iItemID )
			<< BUILD_LOG( iEnchantLevel )
			<< BUILD_LOG( m_iEnchantLevel )
			<< BUILD_LOG( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
			<< END_LOG;

		return;
	}

	iDecreased = std::min< int >( ( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL + m_iEnchantLevel ), iEnchantLevel );

	m_iEnchantLevel -= iDecreased;

	return;
}

void KInventoryItem::SetPosition( int iCategory, int iSlotID )
{
    if( iCategory <= CXSLInventory::ST_NONE || iCategory >= CXSLInventory::ST_END )
    {
        START_LOG( cerr, L"이상한 카테고리." )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlotID )
            << END_LOG;
    }

    if( iSlotID < 0 )
    {
        START_LOG( cerr, L"이상한 슬롯 ID." )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlotID )
            << END_LOG;
    }

    m_iInventoryCategory += ( iCategory - m_iInventoryCategory );
    m_iSlotID += ( iSlotID - m_iSlotID );
    return;
}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
void KInventoryItem::PrepareIncreaseQuantity( IN int iQuantity, OUT int& iIncreased )
{
	// 아이템 유입으로 기존에 있던 아이템 수량이 변동 될 때 체크 해야 할 사항 처리

	iIncreased = 0;

	if( m_iUsageType != CXSLItem::PT_QUANTITY )
	{
		START_LOG( clog, L"수량성 아이템이 아님." )
			<< BUILD_LOG( m_iUsageType )
			<< END_LOG;

		return;
	}

	if( iQuantity <=  0 )
	{
		START_LOG( cerr, L"증가시키려는 수량 값이 이상함." )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;

		return;
	}

	if( m_iQuantity <= 0 )
	{
		START_LOG( cerr, L"수량 값이 이상함." )
			<< BUILD_LOG( m_iQuantity )
			<< END_LOG;

		return;
	}

	// 맥스값 기준으로 더 넣을 수 있는 수량 얻기
	int iRemainedCapacity = GetRemainedQuantityCapacity();
	iIncreased = std::min< int >( iRemainedCapacity, iQuantity );

	return;
}
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

void KInventoryItem::SetPosition( IN int iCategory, IN int iSlotID, IN UidType iPetUID )
{
	if( iCategory <= CXSLInventory::ST_NONE || iCategory >= CXSLInventory::ST_END )
	{
		START_LOG( cerr, L"이상한 카테고리." )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;
	}

	if( iSlotID < 0 )
	{
		START_LOG( cerr, L"이상한 슬롯 ID." )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;
	}

	// 원래 있던 슬롯이 펫이라면?
	if( m_iInventoryCategory == CXSLInventory::ST_PET )
	{
		// 이동하려는 슬롯이 펫이 아니라면?
		if( iCategory != CXSLInventory::ST_PET )
		{
			m_iPetUID += ( 0 - m_iPetUID );
		}
	}
	// 원래 있던 슬롯이 펫이 아니라면?
	{
		// 이동하려는 슬롯이 펫이라면?
		if( iCategory == CXSLInventory::ST_PET )
		{
			m_iPetUID += ( iPetUID - m_iPetUID ); // 당연히 펫 정보를 업데이트 한다!
		}
	}

	m_iInventoryCategory += ( iCategory - m_iInventoryCategory );
	m_iSlotID += ( iSlotID - m_iSlotID );
}

void KInventoryItem::UpdatePetUID( IN UidType iPetUID )
{
    m_iPetUID.SetValue( iPetUID );
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventoryItem::SetItemSocketOption( IN const int iSocketIndex, IN const int iSocketOption )
#else
bool KInventoryItem::SetItemSocketOption( IN const int iSocketIndex, IN const short sSocketOption )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;

		return false;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	int iMaxSocketCount = GetMaxSocketCount();
#else // SERV_BATTLE_FIELD_BOSS
	int iMaxSocketCount = SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType );
#endif // SERV_BATTLE_FIELD_BOSS

	// 소켓 인덱스 검사
	if( iSocketIndex < 0 || iSocketIndex >= iMaxSocketCount )
	{
		START_LOG( cerr, L"아이템의 소켓 인덱스가 이상함." )
			<< BUILD_LOG( m_iItemUID )
			<< BUILD_LOG( m_iItemID )
			<< BUILD_LOG( pItemTemplet->m_ItemGrade )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< BUILD_LOG( iMaxSocketCount )
			<< BUILD_LOG( iSocketIndex )
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			<< BUILD_LOG( iSocketOption )
#else
			<< BUILD_LOG( sSocketOption )
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			<< END_LOG;
		return false;
	}

	// 넣을 인덱스에 비해 소켓 슬롯수가 부족함!
	if( static_cast<int>(m_vecItemSocket.size()) <= iSocketIndex )
	{
		int iEmptyCount = iSocketIndex - m_vecItemSocket.size() + 1;
		if( iEmptyCount > 0 )
		{
			for( int idx=0; idx < iEmptyCount; idx++ )
			{
				m_vecItemSocket.push_back( 0 );
			}
		}
	}

	// [참고] 소켓에서 옵션을 제거하는 기능을 위하여 소켓에 이미 다른옵션이 꽂혀있는지는 여기서 검사하지 않습니다.

	_JIF( 0 <= iSocketIndex && static_cast<int>(m_vecItemSocket.size()) > iSocketIndex, return false; );
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	m_vecItemSocket[iSocketIndex] = iSocketOption;
#else
	m_vecItemSocket[iSocketIndex] = sSocketOption;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	return true;
}

//{{ 2008. 9. 3  최육사		속성강화
bool KInventoryItem::SetAttribEnchant( char cAttribEnchantSlotID, char cAttribEnchantType )
{
	if( SiCXSLAttribEnchantItem()->VerifyAttribEnchantType( cAttribEnchantType ) )
	{
		switch( cAttribEnchantSlotID )
		{
		case CXSLAttribEnchantItem::ESI_SLOT_1:
			m_kAttribEnchantInfo.m_cAttribEnchant0 = cAttribEnchantType;
			break;

		case CXSLAttribEnchantItem::ESI_SLOT_2:
			m_kAttribEnchantInfo.m_cAttribEnchant1 = cAttribEnchantType;
			break;

			//{{ 2009. 11. 9  최육사	트리플속성
		case CXSLAttribEnchantItem::ESI_SLOT_3:
			m_kAttribEnchantInfo.m_cAttribEnchant2 = cAttribEnchantType;
			break;
			//}}

		default:
			START_LOG( cerr, L"속성강화 슬롯ID가 이상합니다." )
				<< BUILD_LOGc( cAttribEnchantSlotID )
				<< BUILD_LOGc( cAttribEnchantType )
				<< END_LOG;
			return false;
		}
	}
	else
	{
		START_LOG( cerr, L"정의된 속성강화값이 아닙니다." )
			<< BUILD_LOGc( cAttribEnchantSlotID )
			<< BUILD_LOGc( cAttribEnchantType )
			<< END_LOG;
		return false;
	}

	return true;
}
//}}

//{{ 2009. 8. 27  최육사	밀봉
bool KInventoryItem::Seal()
{
	if( IsSealed() )
	{
		START_LOG( cerr, L"이미 봉인되어 있습니다." )
			<< END_LOG;

		return false;
	}

#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;

		return 0;
	}

	if( pItemTemplet->m_ucMaxSealCount == CXSLInventory::SEAL_COUNT_MAX )
		m_ucSealData += 100; // 봉인처리만 함
	else
		m_ucSealData += 101; // 봉인처리 및 봉인횟수증가
#else // SERV_REMOVE_SEAL_COUNT_DECREASE
	m_ucSealData += 101; // 봉인처리 및 봉인횟수증가
#endif // SERV_REMOVE_SEAL_COUNT_DECREASE

	return true;
}

bool KInventoryItem::Unseal()
{
	if( !IsSealed() )
	{
		START_LOG( cerr, L"이미 봉인해제되어 있습니다." )
			<< END_LOG;

		return false;
	}

	m_ucSealData -= 100; // 봉인 해제
	return true;
}
//}}

#ifdef	SERV_SHARING_BANK_TEST
bool KInventoryItem::IncreaseSealCount()
{
	++m_ucSealData; // 봉인횟수 증가
	return true;
}
#endif	SERV_SHARING_BANK_TEST

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

void KInventoryItem::UpdatePeriod( IN short sPeriod, IN const std::wstring& wstrExpirationDate )
{
	if( m_sPeriod == 0  &&  sPeriod != 0 )
	{
		START_LOG( cerr, L"기간제가 아닌데 기간제로 바꿀려고 하네? 기획상 이런 기능은 없다!" )
			<< BUILD_LOG( GetItemUID() )
			<< BUILD_LOG( GetItemID() )
			<< BUILD_LOG( m_sPeriod )
			<< BUILD_LOG( sPeriod )
			<< END_LOG;
		return;
	}

	m_sPeriod = sPeriod;
	m_wstrExpirationDate = wstrExpirationDate;
}

#endif SERV_RESET_PERIOD_EVENT
//}}

bool KInventoryItem::FlushQuantityChange( int& iChange )
{
    iChange = 0;

    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        return false;
    }

    iChange = m_iQuantity.GetChangeValue();
    m_iQuantity.SetValue( m_iQuantity );

    return iChange != 0;
}

void KInventoryItem::RollBackInitQuantity( int iChange )
{
    m_iQuantity.AdjustInitValue( m_iQuantity.GetInitValue() - iChange );
}

//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

bool KInventoryItem::FlushPositionChange( OUT int& iCategoryChange, OUT int& iSlotIDChange, OUT UidType& iPetUID )
{
	iCategoryChange = 0;
	iSlotIDChange = 0;
	iPetUID = 0;

	bool bIsChanged = false;

	if( m_iInventoryCategory.IsChanged()  ||  m_iSlotID.IsChanged()  ||  m_iPetUID.IsChanged() )
	{
		iCategoryChange = GetInventoryCategory();
		iSlotIDChange   = GetSlotID();

		// PetUID값이 초기값과 달라졌다면?
		if( m_iPetUID.IsChanged() == true )
		{
			// PetUID를 DB에 업데이트하기 위해 저장!
			iPetUID	= m_iPetUID;
		}
		else
		{
			// PetUID와는 관련없으므로 INVALID값 저장!
			iPetUID = KItemPosition::IPE_INVALID_PET_UID;
		}

#ifdef SERV_SHARING_BANK_TEST
		//	은행공유 아이템이라면 타입을 변경해준다.
		if( iCategoryChange == CXSLInventory::ST_SHARE_BANK )
		{
			iCategoryChange = CXSLInventory::ST_BANK;
		}
#endif SERV_SHARING_BANK_TEST

		bIsChanged = true;
	}

	return bIsChanged;
}

//{{ 2008. 8. 22  최육사	아이템 위치 실값 업데이트
void KInventoryItem::PositionUpdateComplete( int iCategoryChange, int iSlotIDChange, UidType iPetUID )
{
	m_iInventoryCategory.AdjustInitValue( iCategoryChange );
	m_iSlotID.AdjustInitValue( iSlotIDChange );
	if( iPetUID != -1 )
	{
		m_iPetUID.AdjustInitValue( iPetUID );
	}
}
//}}

#else

bool KInventoryItem::FlushPositionChange( int& iCategoryChange, int& iSlotIDChange )
{
	iCategoryChange = 0;
	iSlotIDChange = 0;

	if( m_iInventoryCategory.IsChanged()  ||  m_iSlotID.IsChanged() )
	{
		iCategoryChange = GetInventoryCategory();
		iSlotIDChange   = GetSlotID();

#ifdef SERV_SHARING_BANK_TEST
		//	은행공유 아이템이라면 타입을 변경해준다.
		if( iCategoryChange == CXSLInventory::ST_SHARE_BANK )
		{
			iCategoryChange = CXSLInventory::ST_BANK;
		}
#endif SERV_SHARING_BANK_TEST
	}

	return ( iCategoryChange != 0 ) || ( iSlotIDChange != 0 );
}

//{{ 2008. 8. 22  최육사	아이템 위치 실값 업데이트
void KInventoryItem::PositionUpdateComplete( int iCategoryChange, int iSlotIDChange )
{
	m_iInventoryCategory.AdjustInitValue( iCategoryChange );
	m_iSlotID.AdjustInitValue( iSlotIDChange );
}
//}}

#endif SERV_PET_SYSTEM
//}}

bool KInventoryItem::FlushEnduranceChange( int& iChange )
{
	iChange = 0;

	if( m_iUsageType != CXSLItem::PT_ENDURANCE )
	{
		return false;
	}

	iChange = m_iEndurance.GetChangeValue();
	m_iEndurance.SetValue( m_iEndurance );

	return iChange != 0;
}

void KInventoryItem::RollBackInitEndurance( int iChange )
{
	m_iEndurance.AdjustInitValue( m_iEndurance.GetInitValue() - iChange );
}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
int KInventoryItem::GetMaxSocketCount( void ) const
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;
		return 0;
	}

	return SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType ) + m_byteExpandedSocketNum;
}
#endif // SERV_BATTLE_FIELD_BOSS