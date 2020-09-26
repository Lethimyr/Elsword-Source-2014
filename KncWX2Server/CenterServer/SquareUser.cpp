#include ".\squareuser.h"

#include "X2Data/XSLInventory.h"
#include "CnRoomManager.h"

KSquareUser::KSquareUser(void)
{
	m_kSquareUserInfo.m_cPersonalShopState	= CXSLSquareUnit::PSS_NONE;
	m_iPersonalShopUID	= 0;

	ZU_Refresh();
}

KSquareUser::~KSquareUser(void)
{
}

void KSquareUser::SetUserEquipItem( std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
    std::vector< KInventoryItemInfo >::iterator vit;
    for( vit = vecInventorySlotInfo.begin(); vit != vecInventorySlotInfo.end(); vit++ )
    {
        if( vit->m_cSlotCategory == CXSLInventory::ST_E_EQUIP )
        {
            // 아이템이 존재 하는 것만 insert 한다
			bool bIsInsert = false;
			std::vector< KInventoryItemSimpleInfo >::iterator vit2;
			for( vit2 = m_kSquareUserInfo.m_vecEquippedItem.begin(); vit2 != m_kSquareUserInfo.m_vecEquippedItem.end(); ++vit2 )
			{
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				if( vit->m_sSlotID == vit2->m_sSlotID )
#else
				if( vit->m_cSlotID == vit2->m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				{
					bIsInsert = true;

					{
						vit2->m_iItemUID		= vit->m_iItemUID;
						vit2->m_cSlotCategory	= vit->m_cSlotCategory;
						//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
						vit2->m_sSlotID			= vit->m_sSlotID;
#else
						vit2->m_cSlotID			= vit->m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
						//}}
						vit2->m_iItemID			= vit->m_kItemInfo.m_iItemID;
						vit2->m_EnchantLevel	= vit->m_kItemInfo.m_cEnchantLevel;
						//{{ 2008. 9. 26  최육사	속성강화
						vit2->m_kAttribEnchantInfo = vit->m_kItemInfo.m_kAttribEnchantInfo;
						//}}						
					}
					//*vit2 = *vit;

					break;
				}
			}

			if( bIsInsert == false )
			{
				KInventoryItemSimpleInfo kInfo;
				kInfo.m_iItemUID		= vit->m_iItemUID;
				kInfo.m_cSlotCategory	= vit->m_cSlotCategory;
				//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				kInfo.m_sSlotID			= vit->m_sSlotID;
#else
				kInfo.m_cSlotID			= vit->m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				kInfo.m_iItemID			= vit->m_kItemInfo.m_iItemID;
				kInfo.m_EnchantLevel	= vit->m_kItemInfo.m_cEnchantLevel;
				//{{ 2008. 9. 26  최육사	속성강화
				kInfo.m_kAttribEnchantInfo = vit->m_kItemInfo.m_kAttribEnchantInfo;
				//}}

				m_kSquareUserInfo.m_vecEquippedItem.push_back( kInfo );
			}
        }
    }
}

void KSquareUser::SetPersonalShopData( const KSquarePersonalShopInfo& kInfo, OUT KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT& kNot )
{
	switch( kInfo.m_cUpdateType )
	{
	case KSquarePersonalShopInfo::UT_ALL:
		{
			m_kSquareUserInfo.m_cPersonalShopState = kInfo.m_cPersonalShopState;
			m_kSquareUserInfo.m_cPersonalShopType = kInfo.m_cPersonalShopType;
			m_kSquareUserInfo.m_wstrPersonalShopName = kInfo.m_wstrPersonalShopName;
			m_iPersonalShopUID = kInfo.m_iPersonalShopUID;
		}
		break;

	case KSquarePersonalShopInfo::UT_DATA_CLEAR:
		{
			m_kSquareUserInfo.m_cPersonalShopState = CXSLSquareUnit::PSS_NONE;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
			m_kSquareUserInfo.m_cPersonalShopType = SEnum::AST_NONE;
#else //SERV_UPGRADE_TRADE_SYSTEM
			m_kSquareUserInfo.m_cPersonalShopType = CXSLSquareUnit::PST_NONE;
#endif //SERV_UPGRADE_TRADE_SYSTEM
			m_kSquareUserInfo.m_wstrPersonalShopName.clear();
			m_iPersonalShopUID = 0;
		}
		break;

	case KSquarePersonalShopInfo::UT_STATE_NAME:
		{
			m_kSquareUserInfo.m_cPersonalShopState = kInfo.m_cPersonalShopState;
			m_kSquareUserInfo.m_wstrPersonalShopName = kInfo.m_wstrPersonalShopName;
		}
		break;
	}

	kNot.m_cPersonalShopState	= m_kSquareUserInfo.m_cPersonalShopState;
	kNot.m_cPersonalShopType	= m_kSquareUserInfo.m_cPersonalShopType;
	kNot.m_iUnitUID				= GetCID();
	kNot.m_wstrPersonalShopName	= m_kSquareUserInfo.m_wstrPersonalShopName;

	START_LOG( clog, L"광장 개인상점 정보 셋팅.!" )
		<< BUILD_LOGc( kInfo.m_cPersonalShopState )
		<< BUILD_LOGc( kInfo.m_cPersonalShopType )
		<< BUILD_LOG( kInfo.m_iPersonalShopUID )
		<< BUILD_LOG( kInfo.m_wstrPersonalShopName );
}

bool KSquareUser::ZU_Tick()
{
	if( m_TimerZU.elapsed() >= GetKCnRoomManager()->GetZUTickTime() )
	{
		START_LOG( cout2, L" as a Zombie. delete unit.!![square]" )
			<< BUILD_LOG( m_kSquareUserInfo.m_iGSUID )
			<< BUILD_LOG( m_kSquareUserInfo.m_iUnitUID )
			<< BUILD_LOG( m_kSquareUserInfo.m_wstrNickName )
			//{{ 2009. 1. 28  최육사	개인상점을 열었을때 튕긴건지 보자
			<< BUILD_LOGc( m_kSquareUserInfo.m_cPersonalShopState )
			<< BUILD_LOGc( m_kSquareUserInfo.m_cPersonalShopType )
			//}}
			<< END_LOG;

		return true;
	}

	return false;
}

void KSquareUser::ZU_Refresh()
{
	m_TimerZU.restart();
}

//{{ 2010. 8. 2	최육사	광장 싱크 최적화
#ifdef SERV_SQUARE_SYNC

bool KSquareUser::CompareSquareSyncPacket( IN const KEGS_SQUARE_UNIT_SYNC_DATA_REQ& kInfo )
{
	if( m_kSquareSyncPacket.m_StateID != kInfo.m_StateID )
		goto end_proc;

	if( m_kSquareSyncPacket.m_StateChangeNum != kInfo.m_StateChangeNum )
		goto end_proc;

	if( m_kSquareSyncPacket.m_vPos != kInfo.m_vPos )
		goto end_proc;

	if( m_kSquareSyncPacket.m_LastTouchLineIndex != kInfo.m_LastTouchLineIndex )
		goto end_proc;

	if( m_kSquareSyncPacket.m_bIsRight != kInfo.m_bIsRight )
		goto end_proc;

	// 모두 동일한 내용이면 false
	return false;

end_proc:

	// 다른 내용의 패킷이면 저장하고 true
	m_kSquareSyncPacket = kInfo;
	return true;
}

#endif SERV_SQUARE_SYNC
//}}



