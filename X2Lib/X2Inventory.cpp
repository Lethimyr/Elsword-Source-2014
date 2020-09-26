#include "StdAfx.h"
#include ".\x2inventory.h"


CX2Inventory::InvenSlotData::~InvenSlotData()
{
    SAFE_DELETE( m_pItem );
}


CX2Inventory::CX2Inventory()
//{{ kimhc // 2009-08-07 // 캐릭터별 은행
: m_pOwnerUnit( NULL )
#ifdef	PRIVATE_BANK
, m_iCoutForGetMyBankInfoReq( 0 ) // 은행 정보를 받아온 횟수
, m_eBankMembershipGrade( CX2Inventory::MPB_NORMAL )
#endif	PRIVATE_BANK
//}} kimhc // 2009-08-07 // 캐릭터별 은행
{
	//m_pOwnerUnit = pOwnerUnit;

//Reset할 때 서버에서 날아오는 정보가 정확해지면 이 밑에 코드는 빠져야함.
	for( int i = ST_NONE; i < ST_END; i++ )
	{
		m_MaxSize[i] = MAX_INVENTORY_SIZE;
	}
/*
	m_MaxSize[ST_EQUIP]			= m_pOwnerUnit->GetUnitData().m_EquipInventorySize;
	m_MaxSize[ST_SKILL]			= m_pOwnerUnit->GetUnitData().m_SkillInventorySize;
	m_MaxSize[ST_MATERIAL]		= m_pOwnerUnit->GetUnitData().m_MaterialInventorySize;
	m_MaxSize[ST_SPECIAL]		= m_pOwnerUnit->GetUnitData().m_SpecialInventorySize;
	m_MaxSize[ST_CARD]			= m_pOwnerUnit->GetUnitData().m_CardInventorySize;
	m_MaxSize[ST_QUICK_SLOT]	= m_pOwnerUnit->GetUnitData().m_QuickSlotInventorySize;
	m_MaxSize[ST_AVARTA]		= m_pOwnerUnit->GetUnitData().m_AvartaInventorySize;
	*/
}

CX2Inventory::~CX2Inventory(void)
{
	Clear();
}

void CX2Inventory::Clear()
{
	for( int sortType = ST_NONE; sortType < ST_END; sortType++ )
	{
		if ( !m_ItemMap[sortType].empty() )
		{
			map<int,InvenSlotData*>::iterator iter;
			InvenSlotData* pInvenSlotData = NULL;
			for( iter = m_ItemMap[sortType].begin(); iter != m_ItemMap[sortType].end(); iter++ )
			{
				pInvenSlotData = iter->second;
				SAFE_DELETE( pInvenSlotData );
			}		
			m_ItemMap[sortType].clear();
		}
	}	
	
	if ( !m_ItemMapTotal.empty() )
	m_ItemMapTotal.clear();
#ifdef NEW_ITEM_NOTICE
	ResetNewItem();
#endif //NEW_ITEM_NOTICE
}

#ifdef SERV_SHARING_BANK_TEST
void CX2Inventory::ClearShareBank()
{
	map<int,InvenSlotData*>::iterator mit;
	map<UidType,InvenSlotData*>::iterator mit2;
	InvenSlotData* pInvenSlotData = NULL;

	for( mit = m_ItemMap[ST_SHARE_BANK].begin(); mit != m_ItemMap[ST_SHARE_BANK].end(); mit++ )
	{
		pInvenSlotData = mit->second;

		if( pInvenSlotData != NULL && pInvenSlotData->m_pItem != NULL )
		{
			mit2 = m_ItemMapTotal.find( pInvenSlotData->m_pItem->GetUID() );

			if( mit2 != m_ItemMapTotal.end() )
			{
				m_ItemMapTotal.erase( mit2 );
			}
		}

		SAFE_DELETE( pInvenSlotData );
	}		

	m_ItemMap[ST_SHARE_BANK].clear();
}

bool CX2Inventory::UpdateItemUID( UidType iBeforeUID, UidType iNewUID )
{
	map<UidType,InvenSlotData*>::iterator mit;
    mit = m_ItemMapTotal.find( iBeforeUID );

	if( mit == m_ItemMapTotal.end() )
	{
		return false;
	}

	InvenSlotData* pSlotData = mit->second;
	pSlotData->m_pItem->AccessItemData().m_ItemUID = iNewUID;

	m_ItemMapTotal.erase( mit );
	m_ItemMapTotal.insert( std::make_pair(iNewUID, pSlotData) );

	return true;
}
#endif SERV_SHARING_BANK_TEST

#ifdef SERV_SHARING_BANK_TEST
void CX2Inventory::SetShareBank( std::map< UidType, KInventoryItemInfo >& mapItem )
{
	ClearShareBank();

	std::map< UidType, KInventoryItemInfo >::iterator mItr;

	for ( mItr = mapItem.begin(); mItr != mapItem.end(); mItr++ )
	{
		UidType					iItemUID	= mItr->first;
		KInventoryItemInfo		kInfo		= mItr->second;

		//RemoveItem(	static_cast< CX2Inventory::SORT_TYPE >( kInfo.m_cSlotCategory ), kInfo.m_sSlotID );

		if ( iItemUID > 0 )
		{
			if ( iItemUID != kInfo.m_iItemUID )
			{
				wstringstream wstrstm;
				wstrstm << L"1 : " << iItemUID << L", 2 : " << kInfo.m_iItemUID;
				ErrorLogMsg( XEM_ERROR124, wstrstm.str().c_str() );
			}

			//CX2Item::ItemData* pItemData = new CX2Item::ItemData( kInfo );			
			AddItem( static_cast< CX2Inventory::SORT_TYPE >( kInfo.m_cSlotCategory ), kInfo.m_sSlotID, CX2Item::ItemData( kInfo ) );
		}		
	}
}
#endif SERV_SHARING_BANK_TEST

void CX2Inventory::Reset( std::map< int, int >& mapInventorySlotSize, 
						 std::map< UidType, KInventoryItemInfo >& mapItem )
{
	Clear();

	std::map< int, int >::iterator iS;
	for ( iS = mapInventorySlotSize.begin(); iS != mapInventorySlotSize.end(); ++iS )
	{
		int iCartegory = (*iS).first;
		int iSlotNum = (*iS).second;

		if ( iCartegory < ST_END )		 
		{
			SetItemMaxNum( (SORT_TYPE) iCartegory, iSlotNum );
			//m_MaxSize[iCartegory] = iSlotNum;
		}
	}

	//	은행에 있는 아이템 정보는 제외
	std::map< UidType, KInventoryItemInfo >::iterator iT;
	for ( iT = mapItem.begin(); iT != mapItem.end(); ++iT )
	{
		UidType iItemUID = (*iT).first;
		KInventoryItemInfo& kInfo = (*iT).second;

		RemoveItem( (CX2Inventory::SORT_TYPE) kInfo.m_cSlotCategory, kInfo.m_sSlotID );
	}

	for ( iT = mapItem.begin(); iT != mapItem.end(); ++iT )
	{
		UidType iItemUID = (*iT).first;
		KInventoryItemInfo& kInfo = (*iT).second;

		if ( iItemUID > 0 )
		{
			if ( iItemUID != kInfo.m_iItemUID )
			{
				wstringstream wstrstm;
				wstrstm << L"1 : " << iItemUID << L", 2 : " << kInfo.m_iItemUID;
				ErrorLogMsg( XEM_ERROR124, wstrstm.str().c_str() );
			}
	
			RemoveItem( (CX2Inventory::SORT_TYPE) kInfo.m_cSlotCategory, kInfo.m_sSlotID );
            {
			    CX2Item::ItemData kItemData( kInfo );	
			    AddItem( (CX2Inventory::SORT_TYPE) kInfo.m_cSlotCategory, kInfo.m_sSlotID, kItemData );
            }
		}
	}

	//{{ kimhc // 2009-08-07 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	m_iCoutForGetMyBankInfoReq =  0;		// 은행정보를 받아 왔는지에 대한 값
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-07 // 캐릭터별 은행


	//if( NULL != g_pData->GetQuestManager() )
	//{
	//	g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	//}
}

//{{ kimhc // 2009-08-07 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
void	CX2Inventory::SetBank( std::map< UidType, KInventoryItemInfo >& mapItem )
{
	m_ItemMap[ST_BANK].clear();

	std::map< UidType, KInventoryItemInfo >::iterator mItr;

	for ( mItr = mapItem.begin(); mItr != mapItem.end(); mItr++ )
	{
		UidType					iItemUID	= mItr->first;
		KInventoryItemInfo		kInfo		= mItr->second;

		RemoveItem(	static_cast< CX2Inventory::SORT_TYPE >( kInfo.m_cSlotCategory ), kInfo.m_sSlotID );

		if ( iItemUID > 0 )
		{
			if ( iItemUID != kInfo.m_iItemUID )
			{
				wstringstream wstrstm;
				wstrstm << L"1 : " << iItemUID << L", 2 : " << kInfo.m_iItemUID;
				ErrorLogMsg( XEM_ERROR124, wstrstm.str().c_str() );
			}

            {
			    CX2Item::ItemData kItemData( kInfo );			
			    AddItem( static_cast< CX2Inventory::SORT_TYPE >( kInfo.m_cSlotCategory ), kInfo.m_sSlotID, kItemData );
            }
		}		
	}

	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}
}
#endif	PRIVATE_BANK
//}} kimhc // 2009-08-07 // 캐릭터별 은행

void CX2Inventory::UpdateInventorySlotList( const std::vector<KInventoryItemInfo>& vecInventorySlotInfo )
{
	if( true == vecInventorySlotInfo.empty() )
		return;

	//{{ mauntain : 김태환 [2012.07.10] 장착중인 소비성 아이템 획득시 퀵슬롯 갱신
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	bool			bUpdateQuickSlot = false;		/// 퀵슬롯 갱신 여부
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

	BOOST_TEST_FOREACH( const KInventoryItemInfo&, itemInfo, vecInventorySlotInfo )
	{
		RemoveItem( (CX2Inventory::SORT_TYPE)itemInfo.m_cSlotCategory, itemInfo.m_sSlotID );

		if( itemInfo.m_iItemUID > 0 )
		{
			CX2Item::ItemData kItemData( itemInfo );
			AddItem( (CX2Inventory::SORT_TYPE)itemInfo.m_cSlotCategory, itemInfo.m_sSlotID, kItemData );
		}

		//{{ mauntain : 김태환 [2012.07.10] 장착중인 소비성 아이템 획득시 퀵슬롯 갱신
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		if( CX2Inventory::ST_E_QUICK_SLOT == itemInfo.m_cSlotCategory )
		{
			if( bUpdateQuickSlot == false )
				bUpdateQuickSlot = true;		/// 퀵슬롯 갱신 설정
		}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}
	}


	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}

	//{{ mauntain : 김태환 [2012.07.10] 장착중인 소비성 아이템 획득시 퀵슬롯 갱신
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	if( true == bUpdateQuickSlot )		/// 퀵슬롯 갱신
	{
		if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
			g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();
	}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

	if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIInventory() )
	{
		g_pData->GetUIManager()->GetUIInventory()->SetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED );
		g_pData->GetUIManager()->GetUIInventory()->SetAPString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint );

#ifdef REFORM_SKILL_NOTE_UI // 메모 소지량 갱신
		// 메모 열려 있다면 메모 UI도 갱신)
		if( NULL != g_pData->GetUIManager()->GetUISkillNote() )
			g_pData->GetUIManager()->GetUISkillNote()->UpdateMemoList_MemoItemNum();
#endif // REFORM_SKILL_NOTE_UI
	}

#ifdef SERV_NEW_YEAR_EVENT_2014
	g_pMain->GetMemoryHolder()->UpdateNewYear2014Event();
#endif SERV_NEW_YEAR_EVENT_2014
}

bool CX2Inventory::AddItem( CX2Inventory::SORT_TYPE sortType, int slotID, CX2Item* pItem )
{
	if ( sortType < CX2Inventory::ST_NONE || sortType >= CX2Inventory::ST_END )
		return false;

	if ( pItem == NULL )
		return false;

	if( GetItem( pItem->GetUID(), true ) != NULL )	//kimhc // 2008-08-08 // 은행탭 까지 포함하여 검사
	{
		//중복된 UID 아이템이 인벤토리에 있음
		wstringstream errorMsg;
		errorMsg << m_pOwnerUnit->GetNickName() << L", " << pItem->GetItemData().m_ItemID << ", " << pItem->GetUID();
		ErrorLogMsg(XEM_ERROR73, errorMsg.str().c_str() );

		return false;
	}

	if ( IsPossibleAddItem( sortType ) == false )
		return false;

	InvenSlotData* pInvenSlotData = new InvenSlotData();
	pInvenSlotData->m_SortType	= sortType;
	pInvenSlotData->m_SlotID	= slotID;
	pInvenSlotData->m_pItem		= pItem;

	m_ItemMap[sortType].insert( std::make_pair( slotID,pInvenSlotData ) );
	m_ItemMapTotal.insert( std::make_pair(pItem->GetUID(),pInvenSlotData) );
#ifdef NEW_ITEM_NOTICE
	if( true != m_SetRemoveItem.empty() )
	{
		set<UidType>::iterator it = m_SetRemoveItem.find(pItem->GetUID());
		if( it == m_SetRemoveItem.end())
		{	//지운적이 없는 아이템이면 새롭게 얻은 아이템으로 판단.
			m_SetNewItem.insert(pItem->GetUID());
		}
	}
	else
	{
		//지운적이 없는 아이템이면 새롭게 얻은 아이템으로 판단.
		m_SetNewItem.insert(pItem->GetUID());
	}

	if( ST_E_QUICK_SLOT == sortType &&
		NULL != g_pMain && NULL != g_pMain->GetInformerManager() && g_pMain->GetInformerManager()->GetInvenInformer() )
	{
		g_pMain->GetInformerManager()->GetInvenInformer()->SetNewQuickSlotItem(true, slotID);
	}
#endif //NEW_ITEM_NOTICE

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( pItem->GetItemData().m_ItemID == RESERVED_WEDDING_LETTER_ITEM_ID ||
		 pItem->GetItemData().m_ItemID == INVITE_WEDDING_LETTER_ITEM_ID )
		GetRelationItemInfoFromServer ();
#endif // ADDED_RELATIONSHIP_SYSTEM
	return true;
}

bool CX2Inventory::AddItem( CX2Inventory::SORT_TYPE sortType, int slotID, CX2ItemData_Base& kItemData_ )
{
    CX2Item::ItemData& kItemData = static_cast<CX2Item::ItemData&>( kItemData_ );

	if ( sortType < CX2Inventory::ST_NONE || sortType >= CX2Inventory::ST_END )
		return false;

	if( 
        g_pData->GetItemManager()->GetItemTemplet( kItemData.m_ItemID ) == NULL 
        )
	{
		//WCHAR buff[256] = {0};
		//wsprintf( buff, L"아이템 번호로 아이템 템플릿찾기 실패, 닉네임 : %s, 아이템 번호 : %d", m_pOwnerUnit->GetNickName(), kItemData.m_ItemID );
		//ErrorLogMsg(XEM_ERROR74, buff );

		//KEGS_DELETE_ITEM_REQ kPacket;
		//kPacket.m_cSortType = sortType;
		//kPacket.m_cSlotID = slotID;

		//g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_ITEM_REQ, kPacket );
		//g_pMain->AddServerPacket( EGS_DELETE_ITEM_ACK );

		return false;
	}

	CX2Item* pItem = new CX2Item( kItemData, m_pOwnerUnit );
	bool bRetVal = AddItem( sortType, slotID, pItem );
	if( bRetVal == false )
	{
		SAFE_DELETE( pItem );
	}

	return bRetVal;
}


// @bExcludeEquipped : 장착장비는 제외하고
int CX2Inventory::GetNumItemByTID( int itemTID, bool bExcludeEquipped /*= false*/, bool bIncludeBankType /* = false */) const
{
	int nCount = 0;
	map<UidType,InvenSlotData*>::const_iterator iter;
	for ( iter = m_ItemMapTotal.begin(); iter != m_ItemMapTotal.end(); ++iter )
	{
		const InvenSlotData* pInvenSlotData = iter->second;
		if ( pInvenSlotData->m_pItem != NULL &&
			pInvenSlotData->m_pItem->GetItemTemplet() &&
			pInvenSlotData->m_pItem->GetItemTemplet()->GetItemID() == itemTID 
            )
		{
			if( true == bExcludeEquipped &&
				CX2Inventory::ST_E_EQUIP == pInvenSlotData->m_SortType )
			{
				continue;
			}

			// bExcludeEquipped 는 퀵슬롯 장착도 포함한다.
			if( bExcludeEquipped == true &&
				CX2Inventory::ST_E_QUICK_SLOT == pInvenSlotData->m_SortType )
			{
				continue;
			}

			//{{ kimhc // 2009-08-08 // SortType이 ST_BANK인 것을 찾을 것인가?
#ifdef	PRIVATE_BANK
			if ( false	== bIncludeBankType && 
				ST_BANK	== pInvenSlotData->m_SortType )
			{
				continue;
			}
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-08 // SortType이 ST_BANK인 것을 찾을 것인가?

#ifdef	SERV_SHARING_BANK_TEST
			if ( ST_SHARE_BANK	== pInvenSlotData->m_SortType )	//	은행 공유된 아이템은 패스!
			{
				continue;
			}
#endif	SERV_SHARING_BANK_TEST

#ifdef SERV_PET_SYSTEM
			if ( false	== bIncludeBankType && 
				ST_PET == pInvenSlotData->m_SortType )
			{
				continue;
			}
#endif SERV_PET_SYSTEM

			nCount += pInvenSlotData->m_pItem->GetItemData().m_Quantity;
		}
	}

	return nCount;
}

//{{ mauntain : 김태환 [2012.07.16] 인벤토리 및 슬롯을 고려한 아이템 구입 갯수 연산
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
int CX2Inventory::GetNumSlotByTID( int itemTID ) const		/// 해당 아이템 아이디가 있는 Slot의 갯수를 반환
{
	int nCount = 0;
	map<UidType,InvenSlotData*>::const_iterator iter;
	for ( iter = m_ItemMapTotal.begin(); iter != m_ItemMapTotal.end(); ++iter )
	{
		const InvenSlotData* pInvenSlotData = iter->second;
		if ( pInvenSlotData->m_pItem != NULL &&
			 pInvenSlotData->m_pItem->GetItemTemplet() &&
			 pInvenSlotData->m_pItem->GetItemTemplet()->GetItemID() == itemTID 
             )
		{
			++nCount;
		}
	}

	return nCount;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}

CX2Item* CX2Inventory::GetItemByTID( int itemTID, bool bIncludeBankType /* = false */, bool bIncludeEEquipType /*= true */ ) const
{
	map<UidType,InvenSlotData*>::const_iterator iter;
	for ( iter = m_ItemMapTotal.begin(); iter != m_ItemMapTotal.end(); ++iter )
	{
		const InvenSlotData* pInvenSlotData = iter->second;
		if ( pInvenSlotData->m_pItem != NULL &&
			pInvenSlotData->m_pItem->GetItemTemplet() &&
			pInvenSlotData->m_pItem->GetItemTemplet()->GetItemID() == itemTID 
            )
		{
			//{{ kimhc // 2009-08-08 // SortType이 ST_BANK인 것을 찾을 것인가?
#ifdef	PRIVATE_BANK
			if ( bIncludeBankType == false && pInvenSlotData->m_SortType	== ST_BANK )
				continue;						
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-08 // SortType이 ST_BANK인 것을 찾을 것인가?
#ifdef	SERV_SHARING_BANK_TEST
			if ( pInvenSlotData->m_SortType	== ST_SHARE_BANK )
				continue;						
#endif	SERV_SHARING_BANK_TEST

#ifdef SERV_PET_SYSTEM
			if ( bIncludeBankType == false && pInvenSlotData->m_SortType == ST_PET )
			{
				continue;
			}
#endif SERV_PET_SYSTEM

			// 오현빈 // 2013-08-21 // 장착 중인 장비는 포함하지 않는 예외 추가
			if( false == bIncludeEEquipType && ST_E_EQUIP == pInvenSlotData->m_SortType )
			{
				continue;
			}

			return pInvenSlotData->m_pItem;
		}
	}

	return NULL;
}

#ifdef SERV_EVENT_TEAR_OF_ELWOMAN  
int CX2Inventory::GetItemByTIDCheckAll( int itemTID, bool bIncludeBankType /* = false */ )
{
	int ItemCountByitemTID = 0;

	map<UidType,InvenSlotData*>::iterator iter;
	for ( iter = m_ItemMapTotal.begin(); iter != m_ItemMapTotal.end(); ++iter )
	{
		InvenSlotData* pInvenSlotData = iter->second;
		if ( pInvenSlotData->m_pItem != NULL &&
			pInvenSlotData->m_pItem->GetItemTemplet() &&
			pInvenSlotData->m_pItem->GetItemTemplet()->GetItemID() == itemTID )
		{
			//{{ kimhc // 2009-08-08 // SortType이 ST_BANK인 것을 찾을 것인가?
			//}} kimhc // 2009-08-08 // SortType이 ST_BANK인 것을 찾을 것인가?
#ifdef	SERV_SHARING_BANK_TEST
			if ( pInvenSlotData->m_SortType	== ST_SHARE_BANK )
				continue;						
#endif	SERV_SHARING_BANK_TEST

			ItemCountByitemTID += pInvenSlotData->m_pItem->GetItemData().m_Quantity;
		}
	}

	return ItemCountByitemTID;
}
#endif SERV_EVENT_TEAR_OF_ELWOMAN 

CX2Item* CX2Inventory::GetItem( UidType itemUID, bool bIncludeBankType /* = false */ ) const
{
	CX2Item* pItem = NULL;
	map<UidType,InvenSlotData*>::const_iterator iter;
	iter = m_ItemMapTotal.find( itemUID );
	if( iter != m_ItemMapTotal.end() )
	{
		const InvenSlotData* pInvenSlotData = iter->second;
		if ( pInvenSlotData != NULL )
		{
			//{{ kimhc // 2009-08-08 // SortType이 ST_BANK인 것을 찾을 것인가?
#ifdef	PRIVATE_BANK
#ifdef	SERV_SHARING_BANK_TEST
			if ( bIncludeBankType == true || pInvenSlotData->m_SortType	!= ST_BANK && pInvenSlotData->m_SortType != ST_SHARE_BANK )
#else	SERV_SHARING_BANK_TEST
			if ( bIncludeBankType == true || pInvenSlotData->m_SortType	!= ST_BANK )
#endif	SERV_SHARING_BANK_TEST
				pItem	= pInvenSlotData->m_pItem;				
#else	PRIVATE_BANK
			pItem = pInvenSlotData->m_pItem;
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-08 // SortType이 ST_BANK인 것을 찾을 것인가?
			
		}
	}

	return pItem;
}

CX2Item* CX2Inventory::GetItem( SORT_TYPE sortType, int slotID ) const
{
	ASSERT( sortType > CX2Inventory::ST_NONE && sortType < CX2Inventory::ST_END );

	if ( sortType < CX2Inventory::ST_NONE || sortType >= CX2Inventory::ST_END )
		return NULL;

	CX2Item* pItem = NULL;
	map < int, InvenSlotData* >::const_iterator iter;
    iter = m_ItemMap[sortType].find( slotID );
	if ( iter != m_ItemMap[sortType].end() )
	{
		const InvenSlotData* pInvenSlotData = iter->second;

		if ( pInvenSlotData != NULL )
			pItem = pInvenSlotData->m_pItem;
	}

	return pItem;
}



CX2Inventory::SORT_TYPE CX2Inventory::GetItemSortType( UidType itemUID ) const
{
	CX2Inventory::SORT_TYPE sortType = CX2Inventory::ST_NONE;

	map<UidType,InvenSlotData*>::const_iterator iter;
	iter = m_ItemMapTotal.find( itemUID );
	if( iter != m_ItemMapTotal.end() )
	{
		const InvenSlotData* pInvenSlotData = iter->second;
		if ( pInvenSlotData != NULL )
			sortType = pInvenSlotData->m_SortType;
	}

	return sortType;
}

int CX2Inventory::GetUsedSlotNum( CX2Inventory::SORT_TYPE sortType ) const
{
    if ( false == ( sortType >= SORT_TYPE(0) && sortType < ST_END ) )
        return 0;

	int slotCount = 0;

	for ( int i = 0; i < GetItemMaxNum( sortType ); i++ )
	{	
		CX2Item* pItem = GetItem( sortType, i );
		if ( pItem != NULL )
			slotCount++;
	}

	return slotCount;
}

bool CX2Inventory::RemoveItem( UidType itemUID )
{
	// 은행은 사용 불가
	map<UidType,InvenSlotData*>::iterator iter;
	iter = m_ItemMapTotal.find( itemUID );
	if( iter == m_ItemMapTotal.end() )
	{
		return false;
	}
	
	InvenSlotData* pInvenSlotData = iter->second;

	m_ItemMapTotal.erase( iter );

	if ( pInvenSlotData == NULL )
		return false;

	map<int,InvenSlotData*>::iterator iter2;
	iter2 = m_ItemMap[pInvenSlotData->m_SortType].find( pInvenSlotData->m_SlotID );
	if( iter2 != m_ItemMap[pInvenSlotData->m_SortType].end() )
		m_ItemMap[pInvenSlotData->m_SortType].erase( iter2 );

	SAFE_DELETE( pInvenSlotData );
	
	return true;
}

bool CX2Inventory::RemoveItem( SORT_TYPE sortType, int slotID )
{
	ASSERT( sortType > CX2Inventory::ST_NONE && sortType < CX2Inventory::ST_END );
	if ( sortType < CX2Inventory::ST_NONE || sortType >= CX2Inventory::ST_END )
		return false;

	map < int, InvenSlotData* >::iterator iter;
	iter = m_ItemMap[sortType].find( slotID );
	if ( iter == m_ItemMap[sortType].end() )
		return false;

	InvenSlotData* pInvenSlotData = iter->second;
	m_ItemMap[sortType].erase( iter );

	
	UidType itemUID = pInvenSlotData->m_pItem->GetUID();

	map < UidType, InvenSlotData* >::iterator iter2;
	iter2 = m_ItemMapTotal.find( itemUID );
	if ( iter2 == m_ItemMapTotal.end() )
		return false;

	m_ItemMapTotal.erase( iter2 );
	SAFE_DELETE( pInvenSlotData );

#ifdef NEW_ITEM_NOTICE
	m_SetRemoveItem.insert(itemUID);
#endif //NEW_ITEM_NOTICE
	return true;
}



CX2Inventory::SORT_TYPE CX2Inventory::GetSortTypeByItemTemplet( const CX2ItemTemplet_Base* pItemTemplet_ ) const
{
	CX2Inventory::SORT_TYPE sortType = ST_NONE;

	if ( pItemTemplet_ == NULL )
		return sortType;

    const CX2Item::ItemTemplet* pItemTemplet = static_cast<const CX2Item::ItemTemplet*>( pItemTemplet_ );


	//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
	if ( pItemTemplet->GetIsPcBang() == true )
	{
		sortType = ST_PC_BANG;
	}
	else
#endif	PC_BANG_WORK
	//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스
	if ( pItemTemplet->GetFashion() == true )
	{
		sortType = ST_AVARTA;
	}
	else
	{
		switch ( pItemTemplet->GetItemType() )
		{
		case CX2Item::IT_WEAPON:
		case CX2Item::IT_DEFENCE:
			{
				sortType = ST_EQUIP;
			}
			break;

		case CX2Item::IT_ACCESSORY:
			{
				sortType = ST_ACCESSORY;
			}
			break;

		case CX2Item::IT_QUEST:
			{
				sortType = ST_QUEST;
			}
			break;

		case CX2Item::IT_MATERIAL:
			{
				sortType = ST_MATERIAL;
			}
			break;

		case CX2Item::IT_SPECIAL:
			{
				sortType = ST_SPECIAL;
			}
			break;

		case CX2Item::IT_QICK_SLOT:
			{
				sortType = ST_QUICK_SLOT;
			}
			break;
#ifdef SERV_SKILL_NOTE
		case CX2Item::IT_SKILL_MEMO:
			{
				sortType = ST_SPECIAL;
			}
			break;
#endif
		}
	}



	return sortType;
}


CX2Inventory::SORT_TYPE CX2Inventory::GetSortTypeByID( int itemID ) const
{
    return GetSortTypeByItemTemplet( g_pData->GetItemManager()->GetItemTemplet( itemID ) );
}




bool CX2Inventory::IsPossibleAddItem( SORT_TYPE sortType ) const
{
	if ( sortType < CX2Inventory::ST_NONE || sortType >= CX2Inventory::ST_END )
		return false;

	if ( m_MaxSize[sortType] <= (int)m_ItemMap[sortType].size() )
		return false;

	return true;
}

wstring	CX2Inventory::GetInvenSortTypeName( SORT_TYPE sortType ) const
{
	wstring invenSortTypeName = L"";

	switch( sortType )
	{
	case CX2Inventory::ST_EQUIP:
		{
			invenSortTypeName = GET_STRING( STR_ID_245 );
		}
		break;

	case CX2Inventory::ST_ACCESSORY:
		{
			invenSortTypeName = GET_STRING( STR_ID_246 );
		}
		break;

	case CX2Inventory::ST_MATERIAL:
		{
			invenSortTypeName = GET_STRING( STR_ID_247 );
		}
		break;

	case CX2Inventory::ST_SPECIAL:
		{
			invenSortTypeName = GET_STRING( STR_ID_248 );
		}
		break;

	case CX2Inventory::ST_QUEST:
		{
			invenSortTypeName = GET_STRING( STR_ID_249 );
		}
		break;

	case CX2Inventory::ST_QUICK_SLOT:
		{
			invenSortTypeName = GET_STRING( STR_ID_250 );
		}
		break;

	case CX2Inventory::ST_AVARTA:
		{
			invenSortTypeName = GET_STRING( STR_ID_251 );
		}
		break;

		//{{ 2009. 8. 3  최육사		은행
	case CX2Inventory::ST_BANK:
		//}}	
		{
			invenSortTypeName = GET_STRING( STR_ID_252 );
		}
		break;

	case CX2Inventory::ST_E_EQUIP:
		{
			invenSortTypeName = GET_STRING( STR_ID_253 );
		}	
		break;

	case CX2Inventory::ST_E_SKILL:
		{
			invenSortTypeName = GET_STRING( STR_ID_254 );
		}
		break;

	case CX2Inventory::ST_E_QUICK_SLOT:
		{
			invenSortTypeName = GET_STRING( STR_ID_255 );
		}
		break;
		
	};

	return invenSortTypeName;
}

CX2Item* CX2Inventory::GetEquippingItemByEquipPos( CX2Unit::EQIP_POSITION equipPos, bool bFashion ) const
{
	map< int, InvenSlotData* >::const_iterator it;

	for( it = m_ItemMap[ST_E_EQUIP].begin(); it != m_ItemMap[ST_E_EQUIP].end(); it++ )
	{
		const InvenSlotData* pInvenSlotData = it->second;
		if ( pInvenSlotData != NULL )
		{
			if ( pInvenSlotData->m_pItem != NULL 
                && pInvenSlotData->m_pItem->GetItemTemplet() != NULL 
                )
			{
				if ( pInvenSlotData->m_pItem->GetItemTemplet()->GetEqipPosition() == equipPos &&
					(pInvenSlotData->m_pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_ACCESSORY 
                     || pInvenSlotData->m_pItem->GetItemTemplet()->GetFashion() == bFashion) )	// 09.06.08 태완 : 악세사리 비교 예외처리
				{
					return pInvenSlotData->m_pItem;
				}
			}
		}
	}

	return NULL;
}
#ifdef MODIFY_INFORMER_INVEN
void CX2Inventory::GetItemIDAndNum( OUT set<int>& setItemPackage ) const
#else
void CX2Inventory::GetItemIDAndNum( map<int, int>& mapItemPackage ) const
#endif //MODIFY_INFORMER_INVEN
{
	int nCount = 0;
	map<UidType,InvenSlotData*>::const_iterator iter;
	for ( iter = m_ItemMapTotal.begin(); iter != m_ItemMapTotal.end(); ++iter )
	{
		const InvenSlotData* pInvenSlotData = iter->second;
		if ( pInvenSlotData->m_pItem != NULL &&
			pInvenSlotData->m_pItem->GetItemTemplet() != NULL )
		{
            int itemID = pInvenSlotData->m_pItem->GetItemTemplet()->GetItemID();

#ifdef MODIFY_INFORMER_INVEN
			//퀘스트아이템의 예외처리를 informer로 이동
			//if( CX2Item::IT_QUEST != pInvenSlotData->m_pItem->GetItemTemplet()->m_ItemType )
			{
				setItemPackage.insert( itemID );
			}
#else
			map<int,int>::iterator mit;
			mit = mapItemPackage.find( itemID );
			if ( mit != mapItemPackage.end() )
			{
				mit->second += pInvenSlotData->m_pItem->GetItemData().m_Quantity;
			}
			else
			{
				mapItemPackage.insert( std::make_pair( itemID, pInvenSlotData->m_pItem->GetItemData().m_Quantity ) );
			}
#endif //MODIFY_INFORMER_INVEN
		}
	}
}

int CX2Inventory::GetEqiuppingSetItemNum( int setID ) const
{
	int itemNum = 0;

	map< int, InvenSlotData* >::const_iterator it;

	for( it = m_ItemMap[ST_E_EQUIP].begin(); it != m_ItemMap[ST_E_EQUIP].end(); it++ )
	{
		const InvenSlotData* pInvenSlotData = it->second;
		if ( pInvenSlotData != NULL )
		{
			if ( pInvenSlotData->m_pItem != NULL && 
                pInvenSlotData->m_pItem->GetItemTemplet() != NULL 
                )
			{
				if ( pInvenSlotData->m_pItem->GetItemTemplet()->GetSetID() == setID )
				{
					itemNum++;
				}
			}
		}
	}

	return itemNum;
}

#ifdef ITEM_EXPIRE_USING_ITEM_UID
bool CX2Inventory::CheckEquippingItem( int itemID, UidType itemUID /*= -1*/ ) const
#else ITEM_EXPIRE_USING_ITEM_UID
bool CX2Inventory::CheckEquippingItem( int itemID ) const
#endif ITEM_EXPIRE_USING_ITEM_UID
{
	map< int, InvenSlotData* >::const_iterator it;

	for( it = m_ItemMap[ST_E_EQUIP].begin(); it != m_ItemMap[ST_E_EQUIP].end(); it++ )
	{
		const InvenSlotData* pInvenSlotData = it->second;
		if ( pInvenSlotData != NULL )
		{
			if ( pInvenSlotData->m_pItem != NULL 
                && pInvenSlotData->m_pItem->GetItemTemplet() != NULL 
                )
			{
                if ( pInvenSlotData->m_pItem->GetItemTemplet()->GetItemID() == itemID )
				{
#ifdef ITEM_EXPIRE_USING_ITEM_UID
					if( itemUID < 0 || pInvenSlotData->m_pItem->GetUID() == itemUID )
#endif ITEM_EXPIRE_USING_ITEM_UID
					return true;
				}
			}
		}
	}
	
	return false;
}


// note!! UI에 정보 update용으로만 사용해야 함
CX2Stat::Stat CX2Inventory::GetSetItemOptionStat() const
{
	CX2Stat::Stat tempStat;

	map<int,int> mapSetIDNPartsNum;

	map< int, InvenSlotData* >::const_iterator it;

	for( it = m_ItemMap[ST_E_EQUIP].begin(); it != m_ItemMap[ST_E_EQUIP].end(); it++ )
	{
		const InvenSlotData* pInvenSlotData = it->second;
		if ( pInvenSlotData != NULL )
		{
			if ( pInvenSlotData->m_pItem != NULL &&
                pInvenSlotData->m_pItem->GetItemTemplet() != NULL 
                )
			{
				//{{ kimhc // 실시간 엘소드 중 실시간 내구도 감소
#ifdef REAL_TIME_ELSWORD
				if ( pInvenSlotData->m_pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE && 
					pInvenSlotData->m_pItem->GetItemData().m_Endurance <= 0 )
					continue;
#endif REAL_TIME_ELSWORD
				//}} kimhc // 실시간 엘소드 중 실시간 내구도 감소
				int setID = pInvenSlotData->m_pItem->GetItemTemplet()->GetSetID();
				if ( setID != 0 )
				{
					map< int, int >::iterator mit = mapSetIDNPartsNum.find( setID );
					if ( mit != mapSetIDNPartsNum.end() )
					{
						mit->second += 1;
					}
					else
					{
						mapSetIDNPartsNum.insert( std::make_pair( setID, 1 ) );
					}
				}
			}
		}
	}

	vector< int > vecSetItemOptions;
	map< int, int >::iterator mit = mapSetIDNPartsNum.begin();
	for ( ; mit != mapSetIDNPartsNum.end(); mit++ )
	{
		g_pData->GetItemManager()->GetSetItemOptions( mit->first, mit->second, vecSetItemOptions );
	}


	for ( int i = 0; i < (int)vecSetItemOptions.size(); i++ )
	{
		int socketOptionID = vecSetItemOptions[i];
		const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOptionID );
		if( NULL == pSocketData )
			continue;

		if( pSocketData->m_bDungeonOnly == true && 
			( g_pX2Game == NULL || (NULL != g_pX2Game && ( g_pX2Game->GetGameType() != CX2Game::GT_DUNGEON && g_pX2Game->GetGameType() != CX2Game::GT_BATTLE_FIELD ) ) ) )
		{
			continue;
		}

		if( pSocketData->m_bPVPOnly == true && 
			( g_pX2Game == NULL || (NULL != g_pX2Game && g_pX2Game->GetGameType() != CX2Game::GT_PVP ) ) )
		{
			continue;
		}

		tempStat.AddStat( pSocketData->m_Stat, true ); 
	}

	return tempStat;
}

const CX2Inventory::InvenSlotData* CX2Inventory::GetInvenSlot( CX2Inventory::SORT_TYPE sortType, int slotID ) const
{
	const InvenSlotData* pInvenSlotData = NULL;

	if ( sortType < CX2Inventory::ST_NONE || sortType >= CX2Inventory::ST_END )
		return pInvenSlotData;


	map< int, InvenSlotData* >::const_iterator iter;
	iter = m_ItemMap[sortType].find( slotID );
	if ( iter != m_ItemMap[sortType].end() )
	{
		pInvenSlotData = iter->second;
	}

	return pInvenSlotData;
}

void CX2Inventory::SetItemMaxNum( SORT_TYPE sortType, int maxNum )
{
    if ( sortType >= SORT_TYPE(0) && sortType < ST_END )
    {
	    m_MaxSize[sortType] = maxNum;

	    if( ST_BANK == sortType )
	    {
		    m_eBankMembershipGrade	= static_cast< CX2Inventory::MEMBERSHIP_PRIVATE_BANK >( maxNum );
	    }
    }
}

//{{ kimhc // 2009-10-19 // 최대 MP 증가 값 얻어오기
float CX2Inventory::GetAddMaxMPValue() const
{
	map< int, InvenSlotData* >::const_iterator mItr;
	float	fAddMaxMPValue = 0.0f;

	for( mItr = m_ItemMap[ST_E_EQUIP].begin(); mItr != m_ItemMap[ST_E_EQUIP].end(); mItr++ )
	{
		InvenSlotData* pInvenSlotData = mItr->second;
		if ( pInvenSlotData != NULL )
		{
			if ( pInvenSlotData->m_pItem != NULL && 
                pInvenSlotData->m_pItem->GetItemTemplet() != NULL 
                )
			{
				fAddMaxMPValue += pInvenSlotData->m_pItem->GetItemTemplet()->GetAddMaxMP();
			}
		}
	}

	return fAddMaxMPValue;
}
//}} kimhc // 2009-10-19 // 최대 MP 증가 값 얻어오기

//{{ kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할
#ifdef	PC_BANG_WORK
void	CX2Inventory::ResetSize( const std::map<int, int>& mapInventorySize )
{
	std::map< int, int >::const_iterator iS;

	for ( iS = mapInventorySize.begin(); iS != mapInventorySize.end(); ++iS )
	{
		int iCartegory = (*iS).first;
		int iSlotNum = (*iS).second;

		if ( iCartegory < ST_END )		 
		{
			SetItemMaxNum( static_cast< SORT_TYPE >( iCartegory ), iSlotNum );
		}
	}

	//{{ kimhc // 2009-08-07 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	m_iCoutForGetMyBankInfoReq =  0;		// 은행정보를 받아 왔는지에 대한 값
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-07 // 캐릭터별 은행
#ifdef	SERV_EXPAND_QUICK_SLOT
	if( NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
	{
		g_pData->GetUIManager()->GetUIQuickSlot()->SetExpandQuickSlot( m_MaxSize[ST_E_QUICK_SLOT] );
	}
#endif  SERV_EXPAND_QUICK_SLOT
}

void	CX2Inventory::ResetItems( const std::map< UidType, KInventoryItemInfo >& mapItem )
{
	std::map< UidType, KInventoryItemInfo >::const_iterator iT;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	std::vector< UidType > m_vecRelationItemUID;
#endif // ADDED_RELATIONSHIP_SYSTEM

	for ( iT = mapItem.begin(); iT != mapItem.end(); ++iT )
	{
		UidType iItemUID = (*iT).first;
		const KInventoryItemInfo& kInfo = (*iT).second;

		if ( iItemUID > 0 )
		{
			if ( iItemUID != kInfo.m_iItemUID )
			{
				wstringstream wstrstm;
				wstrstm << L"1 : " << iItemUID << L", 2 : " << kInfo.m_iItemUID;
				ErrorLogMsg( XEM_ERROR124, wstrstm.str().c_str() );
			}
#ifdef ADDED_RELATIONSHIP_SYSTEM

// 청첩장이나 입장권이 있다면, 웨딩 정보를 얻어온다.			
		
			if ( kInfo.m_kItemInfo.m_iItemID == RESERVED_WEDDING_LETTER_ITEM_ID ||
				 kInfo.m_kItemInfo.m_iItemID == INVITE_WEDDING_LETTER_ITEM_ID )
			{
				m_vecRelationItemUID.push_back( kInfo.m_iItemUID );
//				g_pData->GetRelationshipManager()
			}
#endif // ADDED_RELATIONSHIP_SYSTEM
            {
			    CX2Item::ItemData kItemData( kInfo );	
			    //RemoveItem( static_cast< SORT_TYPE >( kInfo.m_cSlotCategory ), kInfo.m_sSlotID );
			    AddItem( static_cast< SORT_TYPE >( kInfo.m_cSlotCategory ), kInfo.m_sSlotID, kItemData );
            }
		}
	}

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipManager() )
	{
		if ( FALSE == m_vecRelationItemUID.empty() )
			g_pData->GetRelationshipManager()->Send_EGS_WEDDING_ITEM_INFO_REQ(m_vecRelationItemUID);
	}
#endif // ADDED_RELATIONSHIP_SYSTEM
	
#ifdef NEW_ITEM_NOTICE
	ResetNewItem();
#endif //NEW_ITEM_NOTICE
}

void	CX2Inventory::RemovePCBangEquips()		// 현재 장착중인 아이템들 중에 PC방 템 삭제!!
{
	vector< UidType > vecPCBangEquips;

	std::map< int, InvenSlotData* >::const_iterator mItr;

	for ( mItr = m_ItemMap[ST_E_EQUIP].begin(); mItr != m_ItemMap[ST_E_EQUIP].end(); ++mItr )
	{
		InvenSlotData* pInvenSlotData = mItr->second;

		if ( pInvenSlotData != NULL && pInvenSlotData->m_pItem != NULL && 
            pInvenSlotData->m_pItem->GetItemTemplet() != NULL 
            )
		{
			if ( pInvenSlotData->m_pItem->GetItemTemplet()->GetIsPcBang() == true )
				vecPCBangEquips.push_back( pInvenSlotData->m_pItem->GetUID() );
		}
	}

	BOOST_TEST_FOREACH ( UidType, value, vecPCBangEquips )
	{
		RemoveItem( value );
	}

}
#endif	PC_BANG_WORK
//}} kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할


#ifdef SERV_PET_SYSTEM
void	CX2Inventory::RemovePetInventory()
{
	vector< UidType > vecPetInventoryItems;

	std::map< int, InvenSlotData* >::const_iterator mItr;

	for ( mItr = m_ItemMap[ST_PET].begin(); mItr != m_ItemMap[ST_PET].end(); ++mItr )
	{
		InvenSlotData* pInvenSlotData = mItr->second;

		if ( pInvenSlotData != NULL && pInvenSlotData->m_pItem != NULL && 
            pInvenSlotData->m_pItem->GetItemTemplet() != NULL 
            )
		{
			vecPetInventoryItems.push_back( pInvenSlotData->m_pItem->GetUID() );
		}
	}
	
	BOOST_TEST_FOREACH ( UidType, value, vecPetInventoryItems )
	{
		RemoveItem( value );
	}
}

void	CX2Inventory::SetPetInventory( const std::vector<KInventoryItemInfo>& vecInventorySlotInfo, int iItemMaxNum )
{
	RemovePetInventory();

	SetItemMaxNum(ST_PET, iItemMaxNum);

	if( true == vecInventorySlotInfo.empty() )
		return;

	BOOST_TEST_FOREACH( const KInventoryItemInfo&, itemInfo, vecInventorySlotInfo )
	{
		if( itemInfo.m_iItemUID > 0 )
		{
			CX2Item::ItemData kItemDat( itemInfo );
			AddItem( (CX2Inventory::SORT_TYPE)itemInfo.m_cSlotCategory, itemInfo.m_sSlotID, kItemDat );
		}
	}

	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}
}
#endif SERV_PET_SYSTEM

//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
bool CX2Inventory::UpdateCategorySlotList( SORT_TYPE sortType, std::vector<UidType>& vecUpdatedInventorySlot )
{
	if ( sortType < CX2Inventory::ST_NONE || sortType >= CX2Inventory::ST_END )
	{
		return false;
	}

	if( true == vecUpdatedInventorySlot.empty() )
	{
		return true;
	}

	vector<UidType>::iterator vit;
	map<UidType,InvenSlotData*>::iterator mit;
	int nSlotID = 0;

	m_ItemMap[sortType].clear();

	for(vit = vecUpdatedInventorySlot.begin(); vit != vecUpdatedInventorySlot.end(); ++vit)
	{
		mit = m_ItemMapTotal.find( *vit );

		if(mit != m_ItemMapTotal.end())	// ItemUID로 해당 아이템을 찾았다.
		{
			mit->second->m_SlotID = nSlotID;
			m_ItemMap[sortType].insert( std::make_pair(nSlotID, mit->second) );
		}

		nSlotID++;
	}

	return true;
}
#endif SERV_SORT_CATEGORY_ITEM 
//}}

#ifdef NEW_ITEM_NOTICE
bool CX2Inventory::IsNewItem( SORT_TYPE sortType, UidType iItemUID) const
{
	if( true == m_SetNewItem.empty() )
		return false;

	set<UidType>::const_iterator it = m_SetNewItem.find(iItemUID);
	if( it != m_SetNewItem.end() )
	{
		return true;
	}
	return false;
}

void CX2Inventory::ResetNewItem()
{
	m_SetNewItem.clear();
	m_SetRemoveItem.clear();
#ifdef NEW_ITEM_NOTICE
	//퀵슬롯의 새로운 아이템 획득에 대한 정보 초기화
	if( NULL != g_pMain && NULL != g_pMain->GetInformerManager() && g_pMain->GetInformerManager()->GetInvenInformer() )
	{
		g_pMain->GetInformerManager()->GetInvenInformer()->SetNewQuickSlotItem(false);
	}
#endif //NEW_ITEM_NOTICE
}

bool CX2Inventory::EraseNewItem( UidType iItemUID )
{	
	set<UidType>::iterator it = m_SetNewItem.find(iItemUID);
	if( it != m_SetNewItem.end() )
	{
		m_SetNewItem.erase(it);
		return true;
	}

	return false;
}
#endif //NEW_ITEM_NOTICE



#ifdef PET_DROP_ITEM_PICKUP
/** function : GetItemUIDBySortTypeAndItemID
	brief	 : 정렬타입, 아이템ID를 이용하여 해당 아이템의 UID얻어오기
			   같은 종류의 아이템이라면 구분 없이 처음 검샏괴는 아이템의 UID를 반환
			   인벤토리 밖에서 특정 아이템의 UID를 얻기 위해 추가
    param	 : sortType : 정렬 타입 , ItemID : 아이템ID
	return   : 해당 아이템의 UID
*/
UidType CX2Inventory::GetItemUIDBySortTypeAndItemID( CX2Inventory::SORT_TYPE sortType_, int ItemID_ ) const
{
	if ( sortType_ < CX2Inventory::ST_NONE || sortType_ >= CX2Inventory::ST_END )
		return 0;

	map< int, InvenSlotData* >::const_iterator it;

	for( it = m_ItemMap[sortType_].begin(); it != m_ItemMap[sortType_].end(); it++ )
	{	
		const InvenSlotData* pSlotData = it->second;
		if( NULL != pSlotData && NULL != pSlotData->m_pItem )
		{
			if( ItemID_ == pSlotData->m_pItem->GetItemData().m_ItemID )
			{
				return pSlotData->m_pItem->GetUID();
			}
		}
	}
	return 0;
}
#endif //PET_DROP_ITEM_PICKUP



#ifdef ADDED_RELATIONSHIP_SYSTEM			
bool CX2Inventory::GetRelationItemInfoFromServer () const
{
	map<UidType,InvenSlotData*>::const_iterator iter;
	vector <UidType> vecItemUID;
	for ( iter = m_ItemMapTotal.begin(); iter != m_ItemMapTotal.end(); ++iter )
	{
		const InvenSlotData* pInvenSlotData = iter->second;
		if ( pInvenSlotData->m_pItem != NULL &&
			pInvenSlotData->m_pItem->GetItemTemplet() )
		{

			if ( pInvenSlotData->m_pItem->GetItemTemplet()->GetItemID() == RESERVED_WEDDING_LETTER_ITEM_ID  || 
				 pInvenSlotData->m_pItem->GetItemTemplet()->GetItemID() == INVITE_WEDDING_LETTER_ITEM_ID )
			{
				vecItemUID.push_back( pInvenSlotData->m_pItem->GetUID() );
			}
		}
	}

	if ( NULL != g_pData->GetRelationshipManager() )
	{
		if ( TRUE != vecItemUID.empty() )
		{
			g_pData->GetRelationshipManager()->Send_EGS_WEDDING_ITEM_INFO_REQ ( vecItemUID );
			return true;
		}
	}
	return false;
}
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef GOOD_ELSWORD //JHKang 
bool CX2Inventory::IsAbleToExpandSlot() const
{
	if ( GetItemMaxNum( ST_EQUIP ) == s_iMaxInventorySize )
		return false;

	if ( GetItemMaxNum( ST_ACCESSORY ) == s_iMaxInventorySize )
		return false;

	if ( GetItemMaxNum( ST_MATERIAL ) == s_iMaxInventorySize )
		return false;

	if ( GetItemMaxNum( ST_SPECIAL ) == s_iMaxInventorySize )
		return false;

	if( GetItemMaxNum( ST_QUEST ) == s_iMaxInventorySize )
		return false;
	
	return true;
}
#endif //GOOD_ELSWORD