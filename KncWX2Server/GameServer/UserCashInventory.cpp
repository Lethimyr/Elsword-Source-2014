#include ".\usercashinventory.h"

//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLItemManager.h"
#endif SERV_CASH_ITEM_SOCKET_OPTION
//}


KUserCashInventory::KUserCashInventory(void)
{
	Clear();
}

KUserCashInventory::~KUserCashInventory(void)
{
}

void KUserCashInventory::Clear()
{
	// 실시간 부활석
	m_bCashResurrectionStone = false;
	m_iCashRStoneUnitUID	 = 0;
	
	m_mapSocketForCash.clear();
	ClearPickUpPackageInfo();
}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KUserCashInventory::GetSocketOptionForCash( int iItemID, int& iSocketOption )
{
	iSocketOption = 0;

	std::map< int, int >::const_iterator mit;
	mit = m_mapSocketForCash.find( iItemID );
	if( mit == m_mapSocketForCash.end() )
		return false;

	iSocketOption = mit->second;
	return true;
}
#else
bool KUserCashInventory::GetSocketOptionForCash( int iItemID, short& sSocketOption )
{
	sSocketOption = 0;

	std::map< int, int >::const_iterator mit;
	mit = m_mapSocketForCash.find( iItemID );
	if( mit == m_mapSocketForCash.end() )
		return false;

	sSocketOption = static_cast<short>(mit->second);
	return true;
}
#endif SERV_ITEM_OPTION_DATA_SIZE
//}}

void KUserCashInventory::AddPickUpPackageInfo( unsigned long ulProductNo, const KNXBTPickUpPackageInfo& kPickUpPackageInfo )
{
	m_mapPickUpPackageInfo.insert( std::make_pair( ulProductNo, kPickUpPackageInfo ) );
}

void KUserCashInventory::DelPickUpPackageInfo( unsigned long ulProductNo )
{
	m_mapPickUpPackageInfo.erase( ulProductNo );
}

bool KUserCashInventory::GetPickUpPackageInfo( unsigned long ulProductNo, KNXBTPickUpPackageInfo& kPickUpPackageInfo )
{
	std::map< unsigned long, KNXBTPickUpPackageInfo >::const_iterator mit;
	mit = m_mapPickUpPackageInfo.find( ulProductNo );
	if( mit == m_mapPickUpPackageInfo.end() )
		return false;

	kPickUpPackageInfo = mit->second;
	return true;
}

void KUserCashInventory::AddUpdatedInventorySlot( const std::vector< KInventoryItemInfo >& vecUpdatedSlotInfo )
{
	if( vecUpdatedSlotInfo.empty() )
		return;

	m_vecUpdatedInventorySlot.insert( m_vecUpdatedInventorySlot.begin(), vecUpdatedSlotInfo.begin(), vecUpdatedSlotInfo.end() );
}

//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
bool KUserCashInventory::SetSocketForCash( const std::map< int, int >& mapSocketForCash, const std::map< int, int >& mapSocketGroupIDForCash )
{
	// 정상적인 소켓 아이디 인지 확인 하자
	std::map< int, int >::const_iterator mitSocket = mapSocketForCash.begin();
	for( ; mitSocket != mapSocketForCash.end() ; ++mitSocket )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mitSocket->first );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( mitSocket->first )
				<< END_LOG;

			return false;
		}

		int iItemType = static_cast<int>(pItemTemplet->m_ItemType);

		if( SiCXSLSocketItem()->CheckCashItemSocketID( iItemType, mitSocket->second ) == false )
		{
			// 존재하지 않는 소켓 정보 입니다.
			START_LOG( cerr, L"[테스트]존재하지 않는 소켓 정보로 아이템을 셋팅하려 합니다." )
				<< BUILD_LOG( mitSocket->first )
				<< BUILD_LOG( mitSocket->second )
				<< BUILD_LOG( iItemType )
				<< END_LOG;

			return false;
		}
	}

	// 정상적인 그룹 아이디 인지 확인 하자
	std::map< int, int >::const_iterator mitGroup = mapSocketGroupIDForCash.begin();
	for( ; mitGroup != mapSocketGroupIDForCash.end() ; ++mitGroup )
	{
		if( SiCXSLSocketItem()->CheckCashItemGroupID( mitGroup->first, mitGroup->second ) == false )
		{
			// 존재하지 않는 그룹 정보 입니다.
			START_LOG( cerr, L"[테스트]존재하지 않는 그룹 정보로 아이템을 셋팅하려 합니다." )
				<< BUILD_LOG( mitGroup->first )
				<< BUILD_LOG( mitGroup->second )
				<< END_LOG;

			return false;
		}
	}

	m_mapSocketForCash = mapSocketForCash;

	return true;
}
#endif SERV_CASH_ITEM_SOCKET_OPTION
//}


