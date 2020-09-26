#include "StdAfx.h"
#include ".\x2item.h"



CX2Item::CX2Item( int itemID )
{
//	m_pItemData		= NULL;
    m_kItemData.m_ItemID = itemID;
	m_pItemTemplet	= g_pData->GetItemManager()->GetItemTemplet( itemID );
	m_pOwnerUnit	= NULL;

	m_bEqip			= false;

}

CX2Item::CX2Item( const ItemData& kItemData, CX2Unit* pOwnerUnit )
    : m_kItemData( kItemData )
{
	//m_pItemData		= pItemData;
	m_pItemTemplet	= g_pData->GetItemManager()->GetItemTemplet( m_kItemData.m_ItemID );
	m_pOwnerUnit	= pOwnerUnit;

	m_bEqip			= false;
}

CX2Item::~CX2Item(void)
{
	//SAFE_DELETE( m_pItemData );
	m_pItemTemplet = NULL;
}


// 내구도 
//float CX2Item::GetRepairPriceDiscountRate()
//{
//	float fRepairePriceSale = 0.0f;
//
//	if( NULL != m_pItemTemplet )
//	{
//		for( UINT i=0; i<m_pItemTemplet->m_vecSocketOption.size(); i++ )
//		{
//			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( m_pItemTemplet->m_vecSocketOption[i] );
//			if( NULL == pSocketData )
//				continue;
//
//			fRepairePriceSale += pSocketData->m_fRepairPriceSale;
//		}
//	}
//	
//	{
//		for ( int i = 0; i < (int)m_kItemData.m_SocketOption.size(); i++ )
//		{
//			int socketOption = m_kItemData.m_SocketOption[i];
//			if ( socketOption != 0 )
//			{
//				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData(socketOption);
//				if ( pSocketData != NULL )
//				{
//					if ( pSocketData->m_fRepairPriceSale != 0.0f )
//					{
//						fRepairePriceSale += pSocketData->m_fRepairPriceSale;
//					}
//				}
//			}
//		}
//	}
//
//#ifdef TEMP_EVENT_REPAIR_COST_DISCOUNT
//	fRepairePriceSale += 0.5f;
//#endif TEMP_EVENT_REPAIR_COST_DISCOUNT
//
//	if( fRepairePriceSale >= CX2SocketItem::SocketData::MAX_REPAIR_PRICE_DISCOUNT )
//		fRepairePriceSale = CX2SocketItem::SocketData::MAX_REPAIR_PRICE_DISCOUNT;
//
//	return fRepairePriceSale;
//}
//
//

int CX2Item::GetEDToRepair()
{
	if ( 
        m_pItemTemplet == NULL 
        || m_pItemTemplet->GetPeriodType() != CX2Item::PT_ENDURANCE )
	{
		return 0;
	}

	float repairED = (float) ( m_pItemTemplet->GetEndurance() - GetItemData().m_Endurance ) * m_pItemTemplet->GetRepairED();
	//float fDiscountRate = GetRepairPriceDiscountRate();
	//repairED = repairED * ( 1.f - fDiscountRate );

	if( repairED < 0.f )
		repairED = 0.f;

	return (int) repairED;
}


int CX2Item::GetVPToRepair()
{
	return 0;
}

int CX2Item::GetEDToSell()
{
	if( NULL == m_pItemTemplet )
		return 0;

	float enduranceRate = 1.f;
	if( CX2Item::PT_ENDURANCE == m_pItemTemplet->GetPeriodType() )
	{
		enduranceRate = (float)GetItemData().m_Endurance / (float)m_pItemTemplet->GetEndurance();
	}

	float sellED = (float)m_pItemTemplet->GetPrice() * 0.2f * enduranceRate;

	if( sellED < 0.f )
		sellED = 0.f;

	return (int) sellED;

}

int CX2Item::GetEDToSocketPush()
{
	int edToPush = 0;
	if ( m_pItemTemplet == NULL )
		return edToPush;

	// 패션템일 경우 다른 수수료 공식 적용
	if( m_pItemTemplet->GetFashion() )
	{
		return 1000;
	}
	

	int iSocketCount = GetSocketOptionNum();
	if( iSocketCount < 1 )
		iSocketCount = 1;

	switch( m_pItemTemplet->GetItemType() )
	{
	case CX2Item::IT_WEAPON:
		{			
			edToPush = (int)( ( (float)m_pItemTemplet->GetPrice() * 0.1f ) * 0.1f ) * iSocketCount;
		}
		break;

	case CX2Item::IT_DEFENCE:
		{			
			edToPush = (int)( ( (float)m_pItemTemplet->GetPrice() * 0.1f ) * 0.05f ) * iSocketCount;
		}
		break;
	default:
		{
			// nothing
		} break;
	}
	return edToPush;

}

int CX2Item::GetEDToSocketRemove()
{
	int edToRemove = 0;
	if ( m_pItemTemplet == NULL )
		return edToRemove;


	// 패션템일 경우 다른 수수료 공식 적용
	if( m_pItemTemplet->GetFashion() )
	{
		return 2000;
	}	


	int iSocketCount = GetSocketOptionNum();
	if( iSocketCount < 1 )
		iSocketCount = 1;


	switch( m_pItemTemplet->GetItemType() )
	{
	case CX2Item::IT_WEAPON:
		{			
			edToRemove = (int)( ( (float)m_pItemTemplet->GetPrice() * 0.1f ) * 0.2f ) * iSocketCount;
		}
		break;

	case CX2Item::IT_DEFENCE:
		{			
			edToRemove = (int)( ( (float)m_pItemTemplet->GetPrice() * 0.1f ) * 0.1f ) * iSocketCount;

		}
		break;
	default:
		{
			// nothing
		} break;
	}



	return edToRemove;
}


int CX2Item::GetSocketOptionNum() const
{
	int optionNum = 0;

	{
		for ( UINT i = 0; i < m_kItemData.m_SocketOption.size(); i++ )
		{
			int socketOption = m_kItemData.m_SocketOption[i];
			if ( socketOption != 0 )
				optionNum++;
		}
	}

	return optionNum;
}

// UI에 정보 표시 목적으로만 사용해야함
CX2Stat::Stat CX2Item::GetStat( bool bIncludeSocketOption /*= false*/ )
{
	CX2Stat::Stat stat;

	ASSERT( m_pItemTemplet );
	if( NULL == m_pItemTemplet )
		return stat; // errror;

#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING	
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05

	// 스탯 추가 레벨 소켓 옵션 적용
	CX2SocketItem::KItemStatRelLVData kItemStatRelLVData;

	// 스탯테이블을 통해 스탯 계산
		CX2Item::KItemFormatStatData kCalculateStat;
		g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, m_pItemTemplet, kItemStatRelLVData);
		kCalculateStat.AddToStat( stat );
	#else
		m_pItemTemplet->GetStatData().AddToStat( stat );
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	stat.AddStat( m_pItemTemplet->GetStat() );
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

	// 아이템의 기본 stat
	{
		GetEnchantStat( stat );

		if( true == bIncludeSocketOption )
		{
			// 유저가 설치한 소켓에 의한 stat증가
			for( UINT i=0; i<m_kItemData.m_SocketOption.size(); i++ )
			{
				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( m_kItemData.m_SocketOption[i] );
				if( NULL == pSocketData )
					continue;

				if( pSocketData->m_bDungeonOnly == true && 
					( g_pX2Game == NULL || ( NULL != g_pX2Game && ( g_pX2Game->GetGameType() != CX2Game::GT_DUNGEON && g_pX2Game->GetGameType() != CX2Game::GT_BATTLE_FIELD ) ) ) )
					continue;

				if( pSocketData->m_bPVPOnly == true && 
					( g_pX2Game == NULL || ( NULL != g_pX2Game && g_pX2Game->GetGameType() != CX2Game::GT_PVP ) ) )
					continue;

				stat.AddStat( pSocketData->m_Stat, true );
			}
		}
	}


	if( true == bIncludeSocketOption )
	{
		// 아이템 고유로 있는 소켓옵션에 의한 stat증가
		if( NULL != m_pItemTemplet )
		{
            DWORD   dwNumSO = m_pItemTemplet->GetNumSocketOption();
			for( UINT i=0; i<dwNumSO; i++ )
			{
                const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( m_pItemTemplet->GetSocketOption(i) );
				if( NULL == pSocketData )
					continue;

				if( pSocketData->m_bDungeonOnly == true && 
					( g_pX2Game == NULL || ( NULL != g_pX2Game && ( g_pX2Game->GetGameType() != CX2Game::GT_DUNGEON && g_pX2Game->GetGameType() != CX2Game::GT_BATTLE_FIELD ) ) ) )
					continue;

				if( pSocketData->m_bPVPOnly == true && 
					( g_pX2Game == NULL || ( NULL != g_pX2Game && g_pX2Game->GetGameType() != CX2Game::GT_PVP ) ) )
					continue;

				stat.AddStat( pSocketData->m_Stat, true );
			}
		}

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		// 감정을 통해 얻는 랜덤 소켓 옵션
		BOOST_FOREACH( int iSocketID, m_kItemData.m_vecRandomSocket )
		{
			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( iSocketID );
			if( NULL == pSocketData )
				continue;

			if( pSocketData->m_bDungeonOnly == true && 
				( g_pX2Game == NULL || ( NULL != g_pX2Game && ( g_pX2Game->GetGameType() != CX2Game::GT_DUNGEON && g_pX2Game->GetGameType() != CX2Game::GT_BATTLE_FIELD ) ) ) )
				continue;

			if( pSocketData->m_bPVPOnly == true && 
				( g_pX2Game == NULL || ( NULL != g_pX2Game && g_pX2Game->GetGameType() != CX2Game::GT_PVP ) ) )
				continue;

			stat.AddStat( pSocketData->m_Stat, true );
		}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
	}

	return stat;
}

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
/// 접두어 출력 여부 추가
wstring CX2Item::GetFullName( IN const bool bAddRandomSocketName_ /*= false*/ )
#else // SERV_UPGRADE_TRADE_SYSTEM
wstring CX2Item::GetFullName()
#endif // SERV_UPGRADE_TRADE_SYSTEM
{
	wstring fullName = L"";
	if (
        NULL != m_pItemTemplet 
        && NULL != g_pData->GetItemManager() )
	{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
		/// 접두어 반환
		wstring wstrRandomSocketName = L"";

	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		if( true == bAddRandomSocketName_ &&
			true == GetItemData().m_bIsEvaluation &&
			false == GetItemData().m_vecRandomSocket.empty() &&
			NULL != g_pData->GetSocketItem() )
		{
			const int iSocketID = GetItemData().m_vecRandomSocket.front();
			g_pData->GetSocketItem()->GetSocketPrefix( iSocketID , wstrRandomSocketName );
		}
	#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#endif //SERV_UPGRADE_TRADE_SYSTEM


		if ( ( m_pItemTemplet->GetItemType() == CX2Item::IT_WEAPON || m_pItemTemplet->GetItemType() == CX2Item::IT_DEFENCE ) &&
			m_pItemTemplet->GetFashion() == false )
		{
			wstring slotItemDesc;

			/*
			if ( m_pItemTemplet->GetItemType() == CX2Item::IT_WEAPON )
			{
				vector< int > vecEnchantType;
				vecEnchantType.push_back( GetItemData().m_EnchantOption1 );
				vecEnchantType.push_back( GetItemData().m_EnchantOption2 );
				wstring tempDamageName = g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ) );
				if ( tempDamageName.empty() == false )
				{
					slotItemDesc += tempDamageName;
					slotItemDesc += L" ";
				}
			}
			else
			{
				wstring tempDamageName = g_pData->GetEnchantItem()->GetEnchantResistName( (CX2EnchantItem::ENCHANT_TYPE)GetItemData().m_EnchantOption1 );
				if ( tempDamageName.empty() == false )
				{
					slotItemDesc += tempDamageName;
					slotItemDesc += L" ";
				}

			}
			*/

			wstringstream wstrstm;

#ifdef ITEM_RECOVERY_TEST
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			/// 아이템 이름에 접두어 추가
			wstrstm << L"+" << abs(m_kItemData.m_EnchantLevel) << L" "  << wstrRandomSocketName << slotItemDesc << 
				m_pItemTemplet->GetFullName_();
	#else //SERV_UPGRADE_TRADE_SYSTEM
				wstrstm << L"+" << abs(m_kItemData.m_EnchantLevel) << L" " << slotItemDesc << 
				m_pItemTemplet->GetFullName_();
	#endif //SERV_UPGRADE_TRADE_SYSTEM
#else
			wstrstm << L"+" << m_kItemData.m_EnchantLevel << L" " << slotItemDesc << 
                m_pItemTemplet->GetFullName_();
#endif
			fullName = wstrstm.str().c_str();
		}
		else
		{
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			fullName = wstrRandomSocketName;			/// 접두어
			fullName += L"";
			fullName += m_pItemTemplet->GetFullName_();	/// 아이템 이름
	#else // SERV_UPGRADE_TRADE_SYSTEM
			fullName = m_pItemTemplet->GetFullName_();
	#endif // SERV_UPGRADE_TRADE_SYSTEM
		}
	}

	return fullName;
}

int CX2Item::ItemTemplet::GetSocketSlotNum() const
{
	int slotNum = 0;

	if ( GetItemType() == CX2Item::IT_WEAPON )
	{
		/*
		enum SOCKET_COUNT
		{
			SC_NONE           = -1,

			SC_WEAPON_LOW     = 0,
			SC_WEAPON_NORMAL  = 1,
			SC_WEAPON_RARE    = 2,
			SC_WEAPON_ELITE   = 3,
			SC_WEAPON_UNIQUE  = 4,

			SC_DEFENCE_LOW    = 0,
			SC_DEFENCE_NORMAL = 1,
			SC_DEFENCE_RARE   = 1,
			SC_DEFENCE_ELITE  = 2,
			SC_DEFENCE_UNIQUE = 2,
		};
		*/
		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(GetItemGrade())
		{
		case CX2Item::IG_LOW:
			{
				slotNum = CX2SocketItem::SC_WEAPON_LOW;
			} break;
		case CX2Item::IG_NORMAL:
			{
				slotNum = CX2SocketItem::SC_WEAPON_NORMAL;
			} break;
		case CX2Item::IG_RARE:
			{
				slotNum = CX2SocketItem::SC_WEAPON_RARE;
			} break;
		case CX2Item::IG_ELITE:
			{
				slotNum = CX2SocketItem::SC_WEAPON_ELITE;
			} break;
		case CX2Item::IG_UNIQUE:
			{
				slotNum = CX2SocketItem::SC_WEAPON_UNIQUE;
			} break;
		default:
			break;
		}//}} elseif -> switch
		
	}
	else if ( GetItemType() == CX2Item::IT_DEFENCE )
	{

		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(GetItemGrade())
		{
		case CX2Item::IG_LOW:
			{
				slotNum = CX2SocketItem::SC_DEFENCE_LOW;
			} break;
		case CX2Item::IG_NORMAL:
			{
				slotNum = CX2SocketItem::SC_DEFENCE_NORMAL;
			} break;
		case CX2Item::IG_RARE:
			{
				slotNum = CX2SocketItem::SC_DEFENCE_RARE;
			} break;
		case CX2Item::IG_ELITE:
			{
				slotNum = CX2SocketItem::SC_DEFENCE_ELITE;
			} break;
		case CX2Item::IG_UNIQUE:
			{
				slotNum = CX2SocketItem::SC_DEFENCE_UNIQUE;
			} break;
		default:
			break;
		}//}} elseif -> switch
		
	}
	
	return slotNum;
}


#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

const wchar_t*  CX2Item::ItemTemplet::GetNameColor_() const
{
	switch( m_ItemGrade )
	{
	case CX2Item::IG_UNIQUE:
		return  L"#CFFCC99";
	case CX2Item::IG_ELITE:
		return  L"#CFF00FF";
	case CX2Item::IG_RARE:
		return  L"#CFFFF00";
	case CX2Item::IG_NORMAL:
		return  L"#CFFFFFF";
	case CX2Item::IG_LOW:
		return  L"#C888888";
	}
	

	return L"#CFFFFFF";
}

const wchar_t* CX2Item::ItemTemplet::GetFullName_() const
{
	//wstring tempName = m_Name;
    return  m_Name.c_str();


	//if ( ( m_ItemType == CX2Item::IT_WEAPON ||
	//	m_ItemType == CX2Item::IT_DEFENCE )
	//	&& m_bFashion == false )
	//{
	//	if ( m_ItemGrade == CX2Item::IG_RARE )
	//	{
	//		tempName = L"고급 " + tempName;
	//	}
	//	else if ( m_ItemGrade == CX2Item::IG_ELITE )
	//	{
	//		tempName = L"엘리트 " + tempName;
	//	}
	//	else if ( m_ItemGrade == CX2Item::IG_UNIQUE )
	//	{
	//		tempName = L"유니크 " + tempName;
	//	}
	//	else if ( m_ItemGrade == CX2Item::IG_LOW )
	//	{
	//		tempName = L"낡은 " + tempName;
	//	}
	//}

	//return tempName;
}

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING



//CX2Item::ItemData* CX2Item::CreateItemData( const KPostItemInfo& kPostItemInfo )
//{
//    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kPostItemInfo.m_iScriptIndex );
//    if( pItemTemplet == NULL )
//        return NULL;
//    return new CX2Item::ItemData( kPostItemInfo, pItemTemplet->GetPeriodType(), pItemTemplet->GetEndurance() );
//}

//{{ robobeg : 2013-11-04
bool    CX2Item::ItemData::Initialize( const KPostItemInfo& data )
{
    const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( data.m_iScriptIndex );
    if( pItemTemplet == NULL )
    {
        Init();
        return false;
    }
    Init( data );
    m_PeriodType            = pItemTemplet->GetPeriodType();
    m_Endurance             = pItemTemplet->GetEndurance();
    return true;
}
//}} robobeg : 2013-11-04



#ifdef ITEM_RECOVERY_TEST
bool CX2Item::IsDisabled()
{
	// 장비아이템인데
	if ( ( m_pItemTemplet->GetItemType() == CX2Item::IT_WEAPON || m_pItemTemplet->GetItemType() == CX2Item::IT_DEFENCE ) &&
		m_pItemTemplet->GetFashion() == false )
	{
		if( m_kItemData.m_EnchantLevel < 0)
			return true;
	}

	return false;
}
#endif

/** @function : GetEnchantStat
	@brief : 강화가 적용된 스탯을 구함 (기본 아이템 스탯 포함)
	@param : 강화가 적용된 스탯을 저장할 레퍼런스
	@return : 강화가 적용될 수 없는 아이템은 false, 적용될 수 있는 아이템은 true
*/
bool CX2Item::GetEnchantStat( OUT CX2Stat::Stat& stat_ ) const
{
	// 강화에 의한 기본 stat의 증가
	if( false == m_pItemTemplet->GetFashion() )
	{
		switch( m_pItemTemplet->GetItemType() )
		{
		default:
			break;

		case CX2Item::IT_WEAPON:
		case CX2Item::IT_DEFENCE:
			{
				// 아이템의 기본 stat
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				// 스탯 추가 레벨 소켓 옵션 적용
				CX2SocketItem::KItemStatRelLVData kItemStatRelLVData;
				if( true == GetIsEvaluation() )
					g_pData->GetItemStatCalculator().GetSocketOptionStatRelLV(kItemStatRelLVData, m_kItemData, m_pItemTemplet);

				// 스탯테이블을 통해 스탯 계산
				CX2Item::KItemFormatStatData kCalculateStat;
				g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, m_pItemTemplet, kItemStatRelLVData);

				kCalculateStat.AssignToStat( stat_ );
	#else
				m_pItemTemplet->GetStatData().AssignToStat( stat_ );
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
				stat_ = m_pItemTemplet->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef ITEM_RECOVERY_TEST
				if( abs(m_kItemData.m_EnchantLevel) >= sizeof(ENCHANT_STAT_SCALE) )
				{
					ASSERT( !"invalid enchant level" );
				}
				else
				{
					stat_.MultiplyStat( ENCHANT_STAT_SCALE[ abs(m_kItemData.m_EnchantLevel) ] );
				}
#else
				if( m_kItemData.m_EnchantLevel < 0 || m_kItemData.m_EnchantLevel >= sizeof(ENCHANT_STAT_SCALE) )
				{
					ASSERT( !"invalid enchant level" );
				}
				else
				{
					stat_.MultiplyStat( ENCHANT_STAT_SCALE[ m_kItemData.m_EnchantLevel ] );
				}
#endif	// ITEM_RECOVERY_TEST

				return true;
			} break;
		}
	}

	return false;
}

/** @function : GetIEchantedItemLevel
	@brief : 강화에 의해서 증가된 아이템 레벨을 구함(기본 아이템 레벨은 포함 안됨)
	@return : 강화에 의해서 증가된 아이템 레벨
*/

int CX2Item::GetIEchantedItemLevel() const
{
	float fIncreasedItemLevelByEnchant = 0;	/// 인챈트에 의해 증가된 아이템 레벨
	if (
        NULL != m_pItemTemplet && 
        0 < m_kItemData.m_EnchantLevel )
	{
		CX2Stat::Stat stat;
		if ( GetEnchantStat( stat ) )
		{
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05

			// 스탯 추가 레벨 소켓 옵션 적용
			CX2SocketItem::KItemStatRelLVData kItemStatRelLVData;
			if( true == GetIsEvaluation() )
				g_pData->GetItemStatCalculator().GetSocketOptionStatRelLV(kItemStatRelLVData, m_kItemData, m_pItemTemplet);

			// 스탯테이블을 통해 스탯 계산
			CX2Item::KItemFormatStatData kCalculateStat;
			g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, m_pItemTemplet, kItemStatRelLVData);

			kCalculateStat.DecFromStat( stat );
	#else
			m_pItemTemplet->GetStatData().DecFromStat( stat );  // 기본 장비스탯을 빼서 증가된 것만 구함
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
			stat.DecStat( m_pItemTemplet->GetStat() );	// 기본 장비스탯을 빼서 증가된 것만 구함
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
			
			fIncreasedItemLevelByEnchant += ( stat.m_fAtkPhysic / STANDARD_VALUE_PHYSIC_ATTACK );
			fIncreasedItemLevelByEnchant += ( stat.m_fAtkMagic / STANDARD_VALUE_MAGIC_ATTACK );
			fIncreasedItemLevelByEnchant += ( stat.m_fDefPhysic / STANDARD_VALUE_PHYSIC_DEFENCE );
			fIncreasedItemLevelByEnchant += ( stat.m_fDefMagic / STANDARD_VALUE_MAGIC_DEFENCE );
			fIncreasedItemLevelByEnchant += ( stat.m_fBaseHP / STANDARD_VALUE_BASE_HP );			
		}	
	}
	fIncreasedItemLevelByEnchant += GetSocketOptionNum() * 2;

	return static_cast<int>(fIncreasedItemLevelByEnchant);	
}


#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
/** @function : GetStat
	@brief : GetStat 랩핑함수. 특정 클래스의 스탯을 확인 하기 위해 작성
*/
CX2Stat::Stat CX2Item::GetStat( IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_ )
{
	CX2Stat::Stat stat;

	ASSERT( m_pItemTemplet );
	if( NULL == m_pItemTemplet )
		return stat; // errror;

	// 스탯 추가 레벨 소켓 옵션 적용
	CX2SocketItem::KItemStatRelLVData kItemStatRelLVData;
	if( true == GetIsEvaluation() )
		g_pData->GetItemStatCalculator().GetSocketOptionStatRelLV(kItemStatRelLVData, m_kItemData, m_pItemTemplet);

	// 스탯테이블을 통해 스탯 계산
	CX2Item::KItemFormatStatData kCalculateStat;
	g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, m_pItemTemplet, kItemStatRelLVData, eUnitType_, eUnitClass_ );
	kCalculateStat.AddToStat( stat );

	// 아이템의 기본 stat
		GetEnchantStat( stat, eUnitType_, eUnitClass_ );
	return stat;
}
/** @function : GetEnchantStat
	@brief : GetEnchantStat 랩핑함수. 특정 클래스의 스탯을 확인 하기 위해 작성
*/
bool CX2Item::GetEnchantStat( OUT CX2Stat::Stat& stat_, IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_ ) const
{
	// 강화에 의한 기본 stat의 증가
	if( false == m_pItemTemplet->GetFashion() )
	{
		switch( m_pItemTemplet->GetItemType() )
		{
		default:
			break;

		case CX2Item::IT_WEAPON:
		case CX2Item::IT_DEFENCE:
			{
				// 스탯 추가 레벨 소켓 옵션 적용
				CX2SocketItem::KItemStatRelLVData kItemStatRelLVData;
				if( true == GetIsEvaluation() )
					g_pData->GetItemStatCalculator().GetSocketOptionStatRelLV(kItemStatRelLVData, m_kItemData, m_pItemTemplet);

				// 아이템의 기본 stat
				CX2Item::KItemFormatStatData kCalculateStat;
				g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, m_pItemTemplet, kItemStatRelLVData, eUnitType_, eUnitClass_ );

				kCalculateStat.AssignToStat( stat_ );

#ifdef ITEM_RECOVERY_TEST
				if( abs(m_kItemData.m_EnchantLevel) >= sizeof(ENCHANT_STAT_SCALE) )
				{
					ASSERT( !"invalid enchant level" );
				}
				else
				{
					stat_.MultiplyStat( ENCHANT_STAT_SCALE[ abs(m_kItemData.m_EnchantLevel) ] );
				}
#else
				if( m_kItemData.m_EnchantLevel < 0 || m_kItemData.m_EnchantLevel >= sizeof(ENCHANT_STAT_SCALE) )
				{
					ASSERT( !"invalid enchant level" );
				}
				else
				{
					stat_.MultiplyStat( ENCHANT_STAT_SCALE[ m_kItemData.m_EnchantLevel ] );
				}
#endif	// ITEM_RECOVERY_TEST

				return true;
			} break;
		}
	}

	return false;
}
/** @function : GetIEchantedItemLevel
	@brief : GetIEchantedItemLevel 랩핑함수. 특정 클래스의 스탯을 확인 하기 위해 작성
*/
int CX2Item::GetIEchantedItemLevel( IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_ ) const
{
	float fIncreasedItemLevelByEnchant = 0;	/// 인챈트에 의해 증가된 아이템 레벨
	if (
        NULL != m_pItemTemplet && 
        0 < m_kItemData.m_EnchantLevel )
	{
		CX2Stat::Stat stat;
		if ( GetEnchantStat( stat ) )
		{
			// 스탯 추가 레벨 소켓 옵션 적용
			CX2SocketItem::KItemStatRelLVData kItemStatRelLVData;
			if( true == GetIsEvaluation() )
				g_pData->GetItemStatCalculator().GetSocketOptionStatRelLV(kItemStatRelLVData, m_kItemData, m_pItemTemplet);

		// 스탯테이블을 통해 스탯 계산
			CX2Item::KItemFormatStatData kCalculateStat;
			g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, m_pItemTemplet, kItemStatRelLVData, eUnitType_, eUnitClass_ );

			kCalculateStat.DecFromStat( stat );
			
			fIncreasedItemLevelByEnchant += ( stat.m_fAtkPhysic / STANDARD_VALUE_PHYSIC_ATTACK );
			fIncreasedItemLevelByEnchant += ( stat.m_fAtkMagic / STANDARD_VALUE_MAGIC_ATTACK );
			fIncreasedItemLevelByEnchant += ( stat.m_fDefPhysic / STANDARD_VALUE_PHYSIC_DEFENCE );
			fIncreasedItemLevelByEnchant += ( stat.m_fDefMagic / STANDARD_VALUE_MAGIC_DEFENCE );
			fIncreasedItemLevelByEnchant += ( stat.m_fBaseHP / STANDARD_VALUE_BASE_HP );			
		}	
	}
	fIncreasedItemLevelByEnchant += GetSocketOptionNum() * 2;

	return static_cast<int>(fIncreasedItemLevelByEnchant);	
}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef ADD_SOCKET_SLOT
/** @function : GetbyAddedSocketSlot
	@brief : 추가 소켓 슬롯 수 반환.
			 현재는 2개 이상 될 수 없기 때문에 어썰트 처리 함.
*/
BYTE CX2Item::GetbyAddedSocketSlot() const 
{ 
#ifdef _IN_HOUSE_
	if( m_kItemData.m_byAddedSocketSlot > 1 )
	{
		ASSERT(!L"m_byAddedSocketSlot Error!");
	}
#endif // _IN_HOUSE_

	return m_kItemData.m_byAddedSocketSlot;
} 
#endif // ADD_SOCKET_SLOT 

