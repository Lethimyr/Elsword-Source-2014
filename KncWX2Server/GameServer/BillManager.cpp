#include "ActorManager.h"
#include "BillManager.h"

#include "KncSend.h"

#ifdef SERV_EXCEPT_NO_SELL
#include "X2Data/XSLCashItemManager.h"
#endif //SERV_EXCEPT_NO_SELL

DWORD KBillManager::ms_dwProductCheckGap( 1 * 60 * 1000 );
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
DWORD KBillManager::ms_dwCashEventCheckGap( 1 * 10 * 1000 );
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

ImplPfID( KBillManager, PI_GS_KOG_BILLING_MANAGER );
ImplementSingleton( KBillManager );

#define CLASS_TYPE KBillManager

KBillManager::KBillManager()
{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	m_vecStartBonusTime.clear();
	m_vecEndBonusTime.clear();
	m_mapReleaseTick.clear();
	m_bCheckBonusTime = false;
#else
	m_iReleaseTick = 0;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	
	m_dwLastProductCheckTick = ::GetTickCount() - ms_dwProductCheckGap;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	m_dwLasCashEvnetCheckTick = ::GetTickCount() - ms_dwCashEventCheckGap;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	
	KCSLOCK_SET_VALUE( m_ulCurrentPurchaseNo, 0 );
}

KBillManager::~KBillManager()
{
}

void KBillManager::GetBillProductCategory( std::vector< KBillProductCategory >& vecBillProductCategory )
{
	vecBillProductCategory = m_vecBillProductCategory;

	return;
}

void KBillManager::SetBillProductCategory( const std::vector< KBillProductCategory >& vecBillProductCategory )
{
	m_vecBillProductCategory = vecBillProductCategory;

	return;
}

void KBillManager::GetBillProductInfo( std::map< int, KBillProductInfo >& mapBillProductInfo )
{
	mapBillProductInfo.clear();
	mapBillProductInfo = m_mapBillProductInfo;
}

void KBillManager::SetBillProductInfo( const std::map< int, KBillProductInfo >& mapBillProductInfo )
{
	m_mapBillProductInfo = mapBillProductInfo;

	START_LOG( clog2, L"num product" )
		<< BUILD_LOG( m_mapBillProductInfo.size() )
		<< END_LOG;
}

void KBillManager::GetBillProductAttribute( std::map< int, KBillProductAttribute >& mapBillProductAttribute )
{
	mapBillProductAttribute.clear();
	mapBillProductAttribute = m_mapBillProductAttribute;
}

void KBillManager::SetBillProductAttribute( const std::map< int, KBillProductAttribute >& mapBillProductAttribute )
{
	m_mapBillProductAttribute = mapBillProductAttribute;
}

void KBillManager::GetBillPackageInfo( std::map< int, KBillPackageInfo >& mapBillPackageInfo )
{
	mapBillPackageInfo.clear();
	mapBillPackageInfo = m_mapBillPackageInfo;
}

void KBillManager::SetBillPackageInfo( const std::map< int, KBillPackageInfo>& mapBillPackageInfo )
{
	m_mapBillPackageInfo = mapBillPackageInfo;
}

bool KBillManager::GetBillProductPage( int iPage, std::vector< KBillProductInfo >& vecBillProductInfo, bool& bIsFinal )
{
	vecBillProductInfo.clear();
	bIsFinal = true;

	if( iPage <= 0 || iPage > ( int )m_vecBillProductPage.size() )
	{
		return false;
	}

	if( iPage < ( int )m_vecBillProductPage.size() )
	{
		bIsFinal = false;
	}

	vecBillProductInfo = m_vecBillProductPage[iPage - 1];

	return true;
}

void KBillManager::SetBillProductPage()
{
	m_vecBillProductPage.clear();

	std::vector< KBillProductInfo > vecProductInfo;
	std::map< int, KBillProductInfo >::reverse_iterator mit;

	for( mit = m_mapBillProductInfo.rbegin(); mit != m_mapBillProductInfo.rend(); mit++ ) //2011.11.24 lygan_조성욱 캐쉬샵 리스트 역순으로 보이게 하기
	{
		if( !mit->second.m_bSale )
		{
			continue;
		}

		if( !mit->second.m_bShow )
		{
			continue;
		}
#ifdef SERV_EXCEPT_NO_SELL
		if( mit->second.m_cCategoryNo == CXSLCashItemManager::CSDC_COUPON 
			|| mit->second.m_cCategoryNo == CXSLCashItemManager::CSDC_NO_SELL_IN_PACKAGE
			)
		{
			continue;
		}
#endif //SERV_EXCEPT_NO_SELL
		vecProductInfo.push_back( mit->second );
		if( vecProductInfo.size() >= NUM_PRODUCT_PER_PAGE )
		{
			m_vecBillProductPage.push_back( vecProductInfo );
			vecProductInfo.clear();
		}
	}

	if( vecProductInfo.size() > 0 )
	{
		m_vecBillProductPage.push_back( vecProductInfo );
	}

	START_LOG( clog2, L"product page" )
		<< BUILD_LOG( m_vecBillProductPage.size() )
		<< END_LOG;
}

int KBillManager::GetItemID( int iProductNo )
{
	std::map< int, KBillProductInfo >::iterator mit;
	mit = m_mapBillProductInfo.find( iProductNo );

	if( mit == m_mapBillProductInfo.end() )
	{
		return 0;
	}

	return mit->second.m_iProductID;
}

bool KBillManager::GetProductInfo( int iProductNo, KBillProductInfo& kInfo )
{
	std::map< int, KBillProductInfo >::iterator mit;
	mit = m_mapBillProductInfo.find( iProductNo );

	if( mit == m_mapBillProductInfo.end() )
	{
		return false;
	}

	kInfo =  mit->second;
	return true;
}
#ifdef SERV_GLOBAL_CASH_PACKAGE
bool KBillManager::GetPackageInfo( int iProductNo, KBillPackageInfo& kInfo )
{
	std::map< int, KBillPackageInfo >::iterator mit;
	mit = m_mapBillPackageInfo.find( iProductNo );

	if( mit == m_mapBillPackageInfo.end() )
	{
		return false;
	}

	kInfo =  mit->second;
	return true;
}
#endif //SERV_GLOBAL_CASH_PACKAGE

bool KBillManager::GetProductAttribute( int iProductNo, KBillProductAttribute& kInfo )
{
	std::map< int, KBillProductAttribute >::iterator mit;
	mit = m_mapBillProductAttribute.find( iProductNo );

	if( mit == m_mapBillProductAttribute.end() )
	{
		return false;
	}

	kInfo = mit->second;
	return true;
}

bool KBillManager::IsPackage( int iProductNo )
{
	std::map< int, KBillPackageInfo >::iterator mit = m_mapBillPackageInfo.find( iProductNo );

	return ( mit != m_mapBillPackageInfo.end() );
}

bool KBillManager::GetPrice( int iProductNo, int& iPrice )
{
	iPrice = 0;
	std::map< int, KBillProductInfo >::iterator mit;
	mit = m_mapBillProductInfo.find( iProductNo );
	if( mit == m_mapBillProductInfo.end() )
	{
		return false;
	}

	iPrice = mit->second.m_iSalePrice;
	return true;
}

bool KBillManager::GetTotalPrice( const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo, int& iPrice )
{
	iPrice = 0;

	int iSum = 0;
	std::vector< KNXBTPurchaseReqInfo >::const_iterator vit;
	for( vit = vecPurchaseReqInfo.begin(); vit != vecPurchaseReqInfo.end(); vit++ )
	{
		int iCurrent;
		if( !GetPrice( vit->m_ulProductNo, iCurrent ) )
		{
			return false;
		}

		iSum += iCurrent;
	}

	iPrice = iSum;
	return true;
}

unsigned long KBillManager::GetNextPurchaseNo()
{
	unsigned long ulCurrentPurchaseNo;

	KCSLOCK_BEGIN( m_ulCurrentPurchaseNo )
		m_ulCurrentPurchaseNo++;
		if( m_ulCurrentPurchaseNo > 0xfffffff0 )
		{
			m_ulCurrentPurchaseNo = 1;
		}
		ulCurrentPurchaseNo = m_ulCurrentPurchaseNo;
	KCSLOCK_END()

	return ulCurrentPurchaseNo;
}

void KBillManager::Tick()
{
	KPerformer::Tick();

	CheckProductInfo();
}

void KBillManager::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
		CASE( EBILL_CHECK_PRODUCT_INFO_ACK );
	default:
		START_LOG( cerr, "핸들러가 지정되지 않은 이벤트." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"이벤트 처리 소요 시간." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}

void KBillManager::CheckProductInfo()
{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if( ::GetTickCount() - m_dwLasCashEvnetCheckTick > ms_dwCashEventCheckGap )
	{
		if(m_vecStartBonusTime.empty() == false && m_vecEndBonusTime.empty() == false)
		{
			if(CheckBonusPeriodTime(m_vecStartBonusTime, m_vecEndBonusTime) == true)
			{
				if(m_bCheckBonusTime == false)
				{
					KEGS_BONUS_CASH_EVENT_NOT kPacketNot;
					kPacketNot.m_bEventStart = true;

					KEvent kEvent2;
					UidType anTrace[2] = { -1, -1 };
					kEvent2.SetData( PI_GS_PROXY, NULL, EGS_BONUS_CASH_EVENT_NOT, kPacketNot );
					KActorManager::GetKObj()->SendToAll( kEvent2 );

					m_bCheckBonusTime = true;
				}
			}
			else
			{
				if(m_bCheckBonusTime == true)
				{
					KEGS_BONUS_CASH_EVENT_NOT kPacketNot;
					kPacketNot.m_bEventStart = false;

					KEvent kEvent2;
					UidType anTrace[2] = { -1, -1 };
					kEvent2.SetData( PI_GS_PROXY, NULL, EGS_BONUS_CASH_EVENT_NOT, kPacketNot );
					KActorManager::GetKObj()->SendToAll( kEvent2 );

					m_bCheckBonusTime = false;
				}
			}
		}
	
		m_dwLasCashEvnetCheckTick = ::GetTickCount();
	}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	if( ::GetTickCount() - m_dwLastProductCheckTick < ms_dwProductCheckGap )
	{
		return;
	}
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	START_LOG( clog2, L"check product" )
		<< BUILD_LOG( GetReleaseTick(CSRTT_NORMAL) )
		<< END_LOG;
#else
	START_LOG( clog2, L"check product" )
		<< BUILD_LOG( GetReleaseTick() )
		<< END_LOG;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	m_dwLastProductCheckTick = ::GetTickCount();

	KEBILL_CHECK_PRODUCT_INFO_REQ kPacket_;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket_.m_mapReleaseTick.clear();
	kPacket_.m_mapReleaseTick.insert(std::make_pair(CSRTT_NORMAL,GetReleaseTick(CSRTT_NORMAL)));
	kPacket_.m_mapReleaseTick.insert(std::make_pair(CSRTT_BONUS_EVENT,GetReleaseTick(CSRTT_BONUS_EVENT)));
#else
	kPacket_.m_iReleaseTick = GetReleaseTick();
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	KncSend( PI_GS_KOG_BILLING_MANAGER, 0, PI_GS_KOG_BILLING_DB, 0, NULL, EBILL_CHECK_PRODUCT_INFO_REQ, kPacket_ );
}

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
void KBillManager::SetReleaseTick(CASH_SHOP_RELEASE_TICK_TYPE _enum ,std::map<int, int> &mapReleaseTick )
{
	std::map<int, int>::iterator sitstr = mapReleaseTick.find(_enum);
	std::map<int, int>::iterator sitdst = m_mapReleaseTick.find(_enum);

	if(sitdst != m_mapReleaseTick.end())
	{
		m_mapReleaseTick.find(_enum)->second = sitstr->second;
	}
	else
	{
		m_mapReleaseTick.insert(std::make_pair(sitstr->first, sitstr->second));
	}
}

int KBillManager::GetReleaseTick(CASH_SHOP_RELEASE_TICK_TYPE _enum)
{
	std::map<int, int>::const_iterator sit = m_mapReleaseTick.find(_enum);
	
	if(sit != m_mapReleaseTick.end())
	{
		return sit->second;
	}
	else
	{
		return 0;
	}
}

bool KBillManager::CheckBonusPeriodTime(std::vector<std::wstring> &_vecStartTime, std::vector<std::wstring> &_vecEndTime )
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	for(int i = (int)_vecStartTime.size()-1 ; i >= 0 ; i-- )
	{
		CTime ctBegin;
		CTime ctEnd;

		if(KncUtil::ConvertStringToCTime(_vecStartTime[i],ctBegin) == false)
		{
			START_LOG( cerr, L"보너스 캐쉬 이벤트 시작 문자열 시간조립 실패.!" )
				<< BUILD_LOG(_vecStartTime[i] )
				<< END_LOG;

			return false;
		}

		if(KncUtil::ConvertStringToCTime(_vecEndTime[i],ctEnd) == false)
		{
			START_LOG( cerr, L"보너스 캐쉬 이벤트 종료 문자열 시간조립 실패.!" )
				<< BUILD_LOG(_vecEndTime[i] )
				<< END_LOG;

			return false;
		}
		if(ctEnd < tCurrentTime )
		{
			if(_vecStartTime.empty() == false)
				_vecStartTime.erase(_vecStartTime.begin()+i);
			if(_vecEndTime.empty() == false)
				_vecEndTime.erase(_vecEndTime.begin()+i);

			START_LOG( clog, L"해당 보너스 캐쉬 이벤트 종료.!" )
				<< BUILD_LOG( _vecStartTime[i] )
				<< BUILD_LOG( _vecEndTime[i] )
				;
		}
		else if(ctBegin <= tCurrentTime && ctEnd >= tCurrentTime )
		{
			START_LOG( clog, L"해당 보너스 캐쉬 이벤트 시작.!" )
				<< BUILD_LOG( _vecStartTime[i] )
				<< BUILD_LOG( _vecEndTime[i] )
				;
			return true;
		}
	}
	return false;
}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

IMPL_ON_FUNC( EBILL_CHECK_PRODUCT_INFO_ACK )
{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	std::map<int, int>::const_iterator sitNormalReq = kPacket_.m_mapReleaseTick.find(CSRTT_NORMAL);
	std::map<int, int>::const_iterator sitBonusReq = kPacket_.m_mapReleaseTick.find(CSRTT_BONUS_EVENT);
	int normalTick = 0;
	int bonusEventTick = 0;
	
	if(sitNormalReq != kPacket_.m_mapReleaseTick.end())
	{
		normalTick = sitNormalReq->second;
	}

	if(sitBonusReq != kPacket_.m_mapReleaseTick.end())
	{
		bonusEventTick = sitBonusReq->second;
	}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	START_LOG( clog2, L"product info" )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		<< BUILD_LOG( kPacket_.m_mapReleaseTick.size() )
		<< BUILD_LOG( GetReleaseTick(CSRTT_NORMAL) )
		<< BUILD_LOG( normalTick )
		<< BUILD_LOG( GetReleaseTick(CSRTT_BONUS_EVENT) )
		<< BUILD_LOG( bonusEventTick )
#else
		<< BUILD_LOG( GetReleaseTick() )
		<< BUILD_LOG( kPacket_.m_iReleaseTick )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		<< BUILD_LOG( kPacket_.m_mapBillProductInfo.size() )
		<< BUILD_LOG( kPacket_.m_mapBillProductAttribute.size() )
		<< BUILD_LOG( kPacket_.m_mapBillPackageInfo.size() )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		<< BUILD_LOG( kPacket_.m_vecStartEvent.size() )
		<< BUILD_LOG( kPacket_.m_vecEndEvent.size() )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		<< END_LOG;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if(GetReleaseTick(CSRTT_BONUS_EVENT) != bonusEventTick)
	{
		KEGS_BONUS_CASH_EVENT_NOT kPacketNot;
		kPacketNot.m_bEventStart = false;
		SetBonusCashEvent(false);

		if(kPacket_.m_vecStartEvent.empty() == false && kPacket_.m_vecEndEvent.empty() == false)
		{
			m_vecStartBonusTime.clear();
			m_vecEndBonusTime.clear();

			for(int i = 0; i< static_cast<signed>(kPacket_.m_vecStartEvent.size()); i++)
			{
				m_vecStartBonusTime.push_back(kPacket_.m_vecStartEvent[i]);
				m_vecEndBonusTime.push_back(kPacket_.m_vecEndEvent[i]);
			}

			kPacketNot.m_bEventStart = CheckBonusPeriodTime(kPacket_.m_vecStartEvent, kPacket_.m_vecEndEvent);

			if(kPacketNot.m_bEventStart == true)
				SetBonusCashEvent(true);
			
		}
		else
		{
		
			START_LOG( clog, L"보너스 릴리즈 Tick 은 추가 되었지만 시간 정보 값이 없다. 보너스 이벤트 진행할게 없음" )
				<< BUILD_LOG( GetReleaseTick(CSRTT_BONUS_EVENT) )
				<< BUILD_LOG( bonusEventTick )
				<< BUILD_LOG( kPacket_.m_vecStartEvent.size() )
				<< BUILD_LOG( kPacket_.m_vecEndEvent.size() )
				<< END_LOG;
		}
		SetReleaseTick(CSRTT_BONUS_EVENT, kPacket_.m_mapReleaseTick );
		KEvent kEvent2;
		UidType anTrace[2] = { -1, -1 };
		kEvent2.SetData( PI_GS_PROXY, NULL, EGS_BONUS_CASH_EVENT_NOT, kPacketNot );
		KActorManager::GetKObj()->SendToAll( kEvent2 );

	}

#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if( GetReleaseTick(CSRTT_NORMAL) == normalTick )
	{
		return;
	}

	SetReleaseTick(CSRTT_NORMAL, kPacket_.m_mapReleaseTick );
#else
	if( GetReleaseTick() == kPacket_.m_iReleaseTick )
	{
		return;
	}

	SetReleaseTick( kPacket_.m_iReleaseTick );
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	SetBillProductInfo( kPacket_.m_mapBillProductInfo );
	SetBillProductAttribute( kPacket_.m_mapBillProductAttribute );
	SetBillPackageInfo( kPacket_.m_mapBillPackageInfo );

	SetBillProductPage();

	// 클라이언트에 상품 정보를 다시 다 받으라고 알린다.
	KEvent kEvent;
	UidType anTrace[2] = { -1, -1 };
	kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_PRODUCT_INFO_CHANGED_NOT );
	KActorManager::GetKObj()->SendToAll( kEvent );
}

#ifdef SERV_WISH_LIST_BUG_FIX
std::set<int> KBillManager::GetSellCashItemList()
{
	std::map<int, KBillProductInfo>::iterator mit;
	std::set<int> setSellCashItemList;
	
	for(mit = m_mapBillProductInfo.begin(); mit != m_mapBillProductInfo.end(); ++mit)
	{
		if(false == mit->second.m_bShow)
		{
			continue;
		}

		if(false == mit->second.m_bSale)
		{
			continue;
		}

		if(setSellCashItemList.find(mit->second.m_iProductID) != setSellCashItemList.end())
		{
			continue;
		}

		setSellCashItemList.insert(mit->second.m_iProductID);
	}

	return setSellCashItemList;
}
#endif SERV_WISH_LIST_BUG_FIX
