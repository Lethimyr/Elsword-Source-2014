#include "Temp.h"
#include <boost/test/utils/foreach.hpp>
#include <map>
#include "NetError.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
ImplementSingleton( KJackPotEventManager );

KJackPotCuponState::KJackPotCuponState()
: m_iUserUID( 0 )
, m_iUnitUID( 0 )
, m_iState( static_cast<int>( KJackPotEvent::STATE0 ) )
{
}
bool KJackPotEvent::CheckTheBeginEndDate( IN const std::wstring& wstrBeginDate, IN const std::wstring& wstrEndDate ) const
{
	if( ( m_wstrBeginDate == wstrBeginDate ) && ( m_wstrEndDate == wstrEndDate ) )
	{
		return true;
	}
	return false;
}

void KJackPotEvent::EndProcess( OUT std::vector<KJackPotEndProcessInfo>& vecJackPotEndProcessInfo ) const
{
	if( 0 < m_iValidCuponCount )
	{
		vecJackPotEndProcessInfo.push_back( KJackPotEndProcessInfo( m_wstrBeginDate, m_wstrEndDate, m_iValidCuponCount ) );
	}
}

void KJackPotEvent::AddCupon( IN const std::wstring& wstrCuponNumber, IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iState )
{
	m_vecCuponInfo.push_back( KJackPotCuponState( wstrCuponNumber, iUserUID, iUnitUID, iState ) );

	if( iState != STATE0 )
	{
		return;
	}
		
	++m_iValidCuponCount;

	CTime tBeginDate;
	if( KncUtil::ConvertStringToCTime( m_wstrBeginDate, tBeginDate ) == false )
	{
		START_LOG( cerr, L"시작 시간 변환 실패. 잭팟 이벤트 발동 텀을 수정할 수 없습니다.")
			<< BUILD_LOG( m_wstrBeginDate )
			<< BUILD_LOG( m_wstrEndDate )
			<< END_LOG;
		return;
	}

	CTime tEndDate;
	if( KncUtil::ConvertStringToCTime( m_wstrEndDate, tEndDate ) == false )
	{
		START_LOG( cerr, L"종료 시간 변환 실패. 잭팟 이벤트 발동 텀을 수정할 수 없습니다.")
			<< BUILD_LOG( m_wstrBeginDate )
			<< BUILD_LOG( m_wstrEndDate )
			<< END_LOG;
		return;
	}

	CTimeSpan	tGab = tEndDate - tBeginDate;
	m_iBeginProcessGab = tGab.GetTotalSeconds() / m_iValidCuponCount;
}

bool KJackPotEvent::IsValidEvent( void ) const
{
	if( m_iValidCuponCount < 1 )
	{
		return false;
	}

	CTime tEndDate;
	if( KncUtil::ConvertStringToCTime( m_wstrEndDate, tEndDate ) == false )
	{
		START_LOG( cerr, L"잭팟 이벤트 종료 시간 변환 실패( 초기화 부분에서 검사한 사항이기 때문에 발생해선 안된다! )" )
			<< BUILD_LOG( m_wstrBeginDate )
			<< BUILD_LOG( m_wstrEndDate )
			<< END_LOG;
	}
	
	if( tEndDate <= CTime::GetCurrentTime() )
	{
		int iNonPaymentCount = 0;
		BOOST_TEST_FOREACH( const KJackPotCuponState&, kJackPotCuponState, m_vecCuponInfo )
		{
			if( kJackPotCuponState.m_iState == 0 )
			{
				++iNonPaymentCount;
			}
		}
		
		START_LOG( cout, L"종료 처리될 잭팟 이벤트." )
			<< BUILD_LOG( m_wstrBeginDate )
			<< BUILD_LOG( m_wstrEndDate )
			<< BUILD_LOG( iNonPaymentCount )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KJackPotEvent::ExtractCuponNumber( IN const UidType iUserUID, IN const UidType iUnitUID, OUT std::wstring& wstrCuponNumber, OUT int& iState )
{
	BOOST_TEST_FOREACH( KJackPotCuponState&, kJackPotCuponState, m_vecCuponInfo )
	{
		if( kJackPotCuponState.m_iState != 0 )
		{
			continue;
		}

		kJackPotCuponState.m_iUserUID	= iUserUID;
		kJackPotCuponState.m_iUnitUID	= iUnitUID;
		kJackPotCuponState.m_iState		= STATE1;

		--m_iValidCuponCount;

		wstrCuponNumber					= kJackPotCuponState.m_wstrCuponNumber;
		iState							= kJackPotCuponState.m_iState;
		return true;
	}
	return false;
}

bool KJackPotEvent::BeginProcess( void )
{
	if( m_bEventStart == false )
	{
		CTime tBeginDate;
		if( KncUtil::ConvertStringToCTime( m_wstrBeginDate, tBeginDate ) == false )
		{
			START_LOG( cerr, L"잭팟 이벤트 시작 시간 변환 실패( 초기화 부분에서 검사한 사항이기 때문에 발생해선 안된다! )" )
				<< BUILD_LOG( m_wstrBeginDate )
				<< BUILD_LOG( m_wstrEndDate )
				<< END_LOG;
		}

		if( CTime::GetCurrentTime() < tBeginDate )
		{
			return false;
		}

		m_bEventStart = true;
	}

	if( m_tTimer.elapsed() < m_iBeginProcessGab )
	{
		return false;
	}
	return true;
}

void KJackPotEvent::ConfirmProcess( void )
{
	m_tTimer.restart();
}

bool KJackPotEventManager::Init( IN const std::vector<KJackPotEventInitInfo>& vecJackpotEventInitInfo )
{
	// 잭팟 이벤트가 끝나면 코드는 사용하지 않는다.
	// 즉, 잭팟 이벤트 기간 중에는 최소한 하나 이상의 잭팟 이벤트 정보를 읽어와야 한다.
	if( vecJackpotEventInitInfo.empty() == true )
	{
		START_LOG( cerr, L"잭팟 이벤트 정보가 하나도 없습니다." )
			<< END_LOG;
		return false;
	}

	std::set<std::wstring> setCuponNumber;
	BOOST_TEST_FOREACH( const KJackPotEventInitInfo&, kJackPotEventInitInfo, vecJackpotEventInitInfo )
	{
		CTime tDate;
		if( KncUtil::ConvertStringToCTime( kJackPotEventInitInfo.m_wstrBeginDate, tDate ) == false )
		{
			START_LOG( cerr, L"잭팟 이벤트 시작 시간이 이상합니다. 변환 실패!" )
				<< BUILD_LOG( kJackPotEventInitInfo.m_wstrBeginDate )
				<< END_LOG;
			return false;
		}

		if( KncUtil::ConvertStringToCTime( kJackPotEventInitInfo.m_wstrEndDate, tDate ) == false )
		{
			START_LOG( cerr, L"잭팟 이벤트 종료 시간이 이상합니다. 변환 실패!" )
				<< BUILD_LOG( kJackPotEventInitInfo.m_wstrEndDate )
				<< END_LOG;
			return false;
		}

		if( setCuponNumber.find( kJackPotEventInitInfo.m_wstrCuponNumber ) != setCuponNumber.end() )
		{
			START_LOG( cerr, L"잭팟 이벤트 쿠폰 정보가 중복되어 있습니다." )
				<< BUILD_LOG( kJackPotEventInitInfo.m_wstrCuponNumber )
				<< END_LOG;
			return false;
		}
		setCuponNumber.insert( kJackPotEventInitInfo.m_wstrCuponNumber );


		// 당첨자 정보 기록
		if( kJackPotEventInitInfo.m_iUserUID != 0 )
		{
			if( m_setWinnerUserUID.insert( kJackPotEventInitInfo.m_iUserUID ).second == false )
			{
				START_LOG( cerr, L"중복 당첨자가 존재한다? 있어선 안되는 에러!!" )
					<< BUILD_LOG( kJackPotEventInitInfo.m_iUserUID )
					<< END_LOG;
				// 일단은 통과하자
				// return false
			}
		}

		// 해당하는 기간의 이벤트를 찾아본다.
		std::vector<KJackPotEvent>::iterator it;
		for( it=m_vecJackPotEvent.begin(); it != m_vecJackPotEvent.end(); ++it )
		{
			if( it->CheckTheBeginEndDate( kJackPotEventInitInfo.m_wstrBeginDate, kJackPotEventInitInfo.m_wstrEndDate ) == true )
			{
				break;
			}
		}
		
		// 해당하는 기간의 이벤트가 존재한다면, 쿠폰 정보 추가
		if( it != m_vecJackPotEvent.end() )
		{
			it->AddCupon( kJackPotEventInitInfo.m_wstrCuponNumber, kJackPotEventInitInfo.m_iUserUID, kJackPotEventInitInfo.m_iUnitUID, kJackPotEventInitInfo.m_iState );
		}
		else	// 없다면 이벤트도 추가
		{
			KJackPotEvent kJackPotEvent( kJackPotEventInitInfo.m_wstrBeginDate, kJackPotEventInitInfo.m_wstrEndDate );
			kJackPotEvent.AddCupon( kJackPotEventInitInfo.m_wstrCuponNumber, kJackPotEventInitInfo.m_iUserUID, kJackPotEventInitInfo.m_iUnitUID, kJackPotEventInitInfo.m_iState );
			m_vecJackPotEvent.push_back( kJackPotEvent );
		}
	}

	return true;
}

void KJackPotEventManager::Tick( OUT std::vector<KJackPotEventWinInfo>& vecJackPotEventWinInfo, OUT std::vector<KJackPotEndProcessInfo>& vecJackPotEndProcessInfo )
{
	// 1초에 한번 수행한다.
	if( m_tTimer.elapsed() < 1.0 )
	{
		return;
	}
	m_tTimer.restart();

	// Tick을 돌면서 각 잭팟 이벤트에서 쿠폰 정보를 뽑는다.
	for( std::vector<KJackPotEvent>::iterator it=m_vecJackPotEvent.begin(); it != m_vecJackPotEvent.end(); )
	{
		// 종료된 이벤트라면 즉시 제거한다.
		if( it->IsValidEvent() == false )
		{
			// 문제가 있을 때 sms 알림을 위해 종료된 이벤트의 결과를 분석하자
			it->EndProcess( vecJackPotEndProcessInfo );
			it = m_vecJackPotEvent.erase( it );
			continue;
		}

		KJackPotEvent& kJackPotEvent = *it;
		++it;

		if( kJackPotEvent.BeginProcess() == false )
		{
			continue;
		}

		// 받을 유저를 추첨 해본다.
		KJackPotEventWinInfo kJackPotEventWinInfo;
		if( LotteryUser( kJackPotEventWinInfo.m_iUserUID, kJackPotEventWinInfo.m_iUnitUID ) == false )
		{
			continue;
		}

		// 쿠폰 정보를 추출 해본다.
		if( kJackPotEvent.ExtractCuponNumber( kJackPotEventWinInfo.m_iUserUID, kJackPotEventWinInfo.m_iUnitUID, kJackPotEventWinInfo.m_wstrCuponNumber, kJackPotEventWinInfo.m_iState ) == false )
		{
			ReInsertUser( kJackPotEventWinInfo.m_iUserUID );
			continue;
		}

		// 당첨 정보를 저장한다.
		vecJackPotEventWinInfo.push_back( kJackPotEventWinInfo );
		kJackPotEvent.ConfirmProcess();
	}
}

void KJackPotEventManager::AddUser( IN const UidType iUserUID, IN const UidType iUnitUID )
{
	std::pair<std::map<UidType, UidType>::iterator, bool> pairResult;
	
	pairResult = m_mapConnectUserAndUnitUID.insert( std::map<UidType, UidType>::value_type( iUserUID, iUnitUID ) );
	if( pairResult.second == false )
	{
		START_LOG( cerr, L"이미 등록된 유저가 또 등록될리가 없는데... 혹시 동일 계정 접속? 일단 덮어 씌우도록 하자!")
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( pairResult.first->second )
			<< END_LOG;
		pairResult.first->second = iUnitUID;
	}

	// 접속한 유저이면서 당첨 정보가 없다면 기록한다.
	if( m_setWinnerUserUID.find( iUserUID ) != m_setWinnerUserUID.end() )
	{
		return;
	}

	pairResult = m_mapConnectUserInfoExceptWinner.insert( std::map<UidType, UidType>::value_type( iUserUID, iUnitUID ) );
	if( pairResult.second == false )
	{
		START_LOG( cerr, L"이미 등록된 유저가 또 등록될리가 없는데... 혹시 동일 계정 접속? 일단 덮어 씌우도록 하자!(1)")
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( pairResult.first->second )
			<< END_LOG;
		pairResult.first->second = iUnitUID;
	}
}

void KJackPotEventManager::RemoveUser( IN const UidType iUserUID, IN const UidType iUnitUID )
{
	// 접속을 종료한 유저를 리스트에서 제외시킨다.
	m_mapConnectUserAndUnitUID.erase( iUserUID );
	m_mapConnectUserInfoExceptWinner.erase( iUserUID );
}

bool KJackPotEventManager::LotteryUser( OUT UidType& iUserUID, OUT UidType& iUnitUID )
{
	if( m_mapConnectUserInfoExceptWinner.empty() == true )
	{
		return false;
	}

	// 당첨될 유저를 추첨한다.
	int iCount = rand() % m_mapConnectUserInfoExceptWinner.size();
	std::map<UidType, UidType>::const_iterator it = m_mapConnectUserInfoExceptWinner.begin();
	for( int i=0; i < iCount; ++i )
	{
		++it;
	}

	iUserUID = it->first;
	iUnitUID = it->second;

	// 처리되는 동안 다시 뽑히지 않도록 하기 위해 당첨자 목록에 추가.
	m_setWinnerUserUID.insert( it->first );
	m_mapConnectUserInfoExceptWinner.erase( it->first );

	return true;
}

void KJackPotEventManager::ReInsertUser( IN const UidType iUserUID )
{
	// 현재 접속 중이 아니라면, 추가할 필요 없다.
	std::map<UidType, UidType>::iterator it = m_mapConnectUserAndUnitUID.find( iUserUID );
	if( it == m_mapConnectUserAndUnitUID.end() )
	{
		return;
	}

	// 혹시 모를 예외 상황 처리
	std::pair<std::map<UidType, UidType>::iterator, bool> pairResult = m_mapConnectUserInfoExceptWinner.insert( std::map<UidType, UidType>::value_type( it->first, it->second ) );
	if( pairResult.second == false )
	{
		START_LOG( cerr, L"이미 등록된 유저가 또 등록될리가 없는데... 일단 덮어 씌우도록 하자!")
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( it->second )
			<< BUILD_LOG( pairResult.first->second )
			<< END_LOG;
		pairResult.first->second = it->second;
	}
}
#endif SERV_EVENT_JACKPOT
//}}

//{{ 2012. 09. 25	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
ImplementRefreshSingleton( KGSBingoEventInfo );

KGSBingoEventInfo::KGSBingoEventInfo()
{
}

KGSBingoEventInfo::~KGSBingoEventInfo()
{
}

ImplToStringW( KGSBingoEventInfo )
{
	stm_	<< L"----------[ Bingo Event Info ]----------" << std::endl
		<< TOSTRINGW( m_mapPackageItemPriceForGift.size() )
		<< TOSTRINGW( m_mapPackageItemPrice.size() )
		<< TOSTRINGW( m_setPresent.size() )
		<< TOSTRINGW( m_setBanPresent.size() )
		<< TOSTRINGW( m_mapAllPresentItemID.size() )
		<< TOSTRINGW( m_mapAllPresentProductNo.size() )
		<< TOSTRINGW( m_setBanSpot.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KGSBingoEventInfo )
{
	lua_tinker::class_add<KGSBingoEventInfo>( GetLuaState(), "KGSBingoEventInfo" );

	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "AddPackageItemPriceForGift",	&KGSBingoEventInfo::AddPackageItemPriceForGift_LUA );
	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "AddPackageItemPrice",			&KGSBingoEventInfo::AddPackageItemPrice_LUA );
	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "AddPresentInfo",				&KGSBingoEventInfo::AddPresentInfo_LUA );
	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "AddBanSpotInfo",				&KGSBingoEventInfo::AddBanSpotInfo_LUA );
	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "CheckBillingIP",				&KGSBingoEventInfo::CheckBillingIP_LUA );

	lua_tinker::decl( GetLuaState(), "KGSBingoEventInfo", this );
}

void KGSBingoEventInfo::AddPackageItemPriceForGift_LUA( IN unsigned long ulProductNo, IN unsigned long ulSalePrice )
{
	if( m_mapPackageItemPriceForGift.insert( std::map<unsigned long, unsigned long>::value_type( ulProductNo, ulSalePrice ) ).second == false )
	{
		START_LOG( cerr, L"빙고 이벤트 - 동일한 ProductNo 정보가 이미 등록되어 있습니다." )
			<< BUILD_LOG( ulProductNo )
			<< BUILD_LOG( ulSalePrice )
			<< END_LOG;
	}
}

void KGSBingoEventInfo::GetPackageItemPriceForGift( IN unsigned long ulProductNo, OUT unsigned long& ulSalePrice ) const
{
	std::map<unsigned long, unsigned long>::const_iterator it = m_mapPackageItemPriceForGift.find( ulProductNo );
	if( it != m_mapPackageItemPriceForGift.end() )
	{
		ulSalePrice = it->second;
	}
}

void KGSBingoEventInfo::AddPackageItemPrice_LUA( IN unsigned long ulProductNo, IN unsigned long ulSalePrice )
{
	if( m_mapPackageItemPrice.insert( std::map<unsigned long, unsigned long>::value_type( ulProductNo, ulSalePrice ) ).second == false )
	{
		START_LOG( cerr, L"빙고 이벤트 - 동일한 ProductNo 정보가 이미 등록되어 있습니다." )
			<< BUILD_LOG( ulProductNo )
			<< BUILD_LOG( ulSalePrice )
			<< END_LOG;
	}
}

void KGSBingoEventInfo::GetPackageItemPrice( IN unsigned long ulProductNo, OUT unsigned long& ulSalePrice ) const
{
	std::map<unsigned long, unsigned long>::const_iterator it = m_mapPackageItemPrice.find( ulProductNo );
	if( it != m_mapPackageItemPrice.end() )
	{
		ulSalePrice = it->second;
	}
}

void KGSBingoEventInfo::AddPresentInfo_LUA( IN int iItemID, IN unsigned long ulProductNo, IN bool bIsBanPresent /*= false */)
{
	if( bIsBanPresent == true )
	{
		if( m_setPresent.find( ulProductNo ) != m_setPresent.end() )
		{
			START_LOG( cerr, L"빙고 이벤트 - 해당 ProductNo는 이미 일반 상품으로 등록되어 있습니다." )
				<< BUILD_LOG( ulProductNo )
				<< BUILD_LOG( bIsBanPresent )
				<< END_LOG;
			return;
		}
		m_setBanPresent.insert( ulProductNo );
	}
	else
	{
		if( m_setBanPresent.find( ulProductNo ) != m_setBanPresent.end() )
		{
			START_LOG( cerr, L"빙고 이벤트 - 해당 ProductNo는 이미 제한 상품으로 등록되어 있습니다." )
				<< BUILD_LOG( ulProductNo )
				<< BUILD_LOG( bIsBanPresent )
				<< END_LOG;
			return;
		}
		m_setPresent.insert( ulProductNo );
	}

	m_mapAllPresentItemID.insert( std::map<int, unsigned long>::value_type( iItemID, ulProductNo ) );
	m_mapAllPresentProductNo.insert( std::map<unsigned long, int>::value_type( ulProductNo, iItemID ) );
}

bool KGSBingoEventInfo::GetPresentProductNoFromItemID( IN const int iItemID, OUT unsigned long& ulProductNo ) const
{
	std::map<int, unsigned long>::const_iterator it = m_mapAllPresentItemID.find( iItemID );
	if( it == m_mapAllPresentItemID.end() )
	{
		return false;
	}
	
	ulProductNo = it->second;
	return true;
}

int KGSBingoEventInfo::GetBingoEventItemID( IN const unsigned long ulProductNo ) const
{
	std::map<unsigned long, int>::const_iterator it = m_mapAllPresentProductNo.find( ulProductNo );
	if( it == m_mapAllPresentProductNo.end() )
	{
		return 0;
	}

	return it->second;
}

void KGSBingoEventInfo::AddBanSpotInfo_LUA( IN byte bytePos )
{
	m_setBanSpot.insert( bytePos );
}

void KGSBingoEventInfo::GetPresentInfo( OUT std::vector<int>& vecPresent )
{
	vecPresent.clear();

	int iItemID;
	BOOST_TEST_FOREACH( const unsigned long, ulProductNo, m_setPresent )
	{
		iItemID = GetBingoEventItemID( ulProductNo );
		if( iItemID != 0 )
		{
			vecPresent.push_back( iItemID );
		}
	}
}

void KGSBingoEventInfo::GetBanPresentInfo( OUT std::vector<int>& vecBanPresent )
{
	vecBanPresent.clear();

	int iItemID;
	BOOST_TEST_FOREACH( const unsigned long, ulProductNo, m_setBanPresent )
	{
		iItemID = GetBingoEventItemID( ulProductNo );
		if( iItemID != 0 )
		{
			vecBanPresent.push_back( iItemID );
		}
	}
}

bool KGSBingoEventInfo::IsBanSpot( IN const byte byteSpotIndex )
{
	return m_setBanSpot.find( byteSpotIndex ) != m_setBanSpot.end();
}

bool KGSBingoEventInfo::IsBingoPresent( IN unsigned long ulProductNo )
{
	return m_mapAllPresentProductNo.find( ulProductNo ) != m_mapAllPresentProductNo.end();
}

bool KGSBingoEvent::CheckBingoEventReward( IN const unsigned long& ulProductNo )
{
	int iItemID = SiKGSBingoEventInfo()->GetBingoEventItemID( ulProductNo );

	if( iItemID == 0 )
	{
		return true;
	}
	else if( m_setGetThePresent.find( iItemID ) != m_setGetThePresent.end() )
	{
		m_setGetThePresent.erase( iItemID );
		return true;
	}
	return false;
}

KGSBingoEvent::KGSBingoEvent()
: m_bBusy( false )
{
	m_iLotteryChance.SetValue( 0 );
	m_iLotteryRecord.SetValue( 0 );
	m_iMixChance.SetValue( 0 );
	m_iResetChance.SetValue( 0 );
	m_iBalance.SetValue( 0 );

	m_vecBingoBoard.reserve( BSV_BINGOBOARD_SLOT_NUM );
	m_vecPresentInfo.reserve( BSV_PRESENT_NUM );
}

KGSBingoEvent::BingoState KGSBingoEvent::GetBingoEventInfoForDB( IN const unsigned short usHandlingEventID, OUT KDBE_BINGO_EVENT_INFO_WRITE_REQ& kPacket )
{
	BingoState bsResult = BS_NONE_CHANCE;

	// 돌아올 때 타고 올 패킷 종류 지정
	kPacket.m_usAckEventID = usHandlingEventID;

	// 1. 기회 정보 복사
	if( m_iBalance.IsChanged()			|| 
		m_iLotteryChance.IsChanged()	|| 
		m_iLotteryRecord.IsChanged()	|| 
		m_iMixChance.IsChanged()		|| 
		m_iResetChance.IsChanged()
		)
	{
		kPacket.m_iBalance			= m_iBalance.GetChangeValue();
		kPacket.m_iLotteryChance	= m_iLotteryChance.GetChangeValue();
		kPacket.m_iLotteryRecord	= m_iLotteryRecord.GetChangeValue();
		kPacket.m_iMixChance		= m_iMixChance.GetChangeValue();
		kPacket.m_iResetChance		= m_iResetChance.GetChangeValue();
		kPacket.m_bChanceChanged	= true;

		m_iBalance.SetValue( m_iBalance );				// 잔액 정보
		m_iLotteryChance.SetValue( m_iLotteryChance );	// 뽑기 찬스 횟수
		m_iLotteryRecord.SetValue( m_iLotteryRecord );	// 뽑은 횟수
		m_iMixChance.SetValue( m_iMixChance );			// 뒤섞기 찬스 횟수
		m_iResetChance.SetValue( m_iResetChance );		// 초기화 찬스 횟수

		bsResult = BS_OK;
	}

	byte i;

	// 2. 빙고판 정보 복사
	for( i=0; i < m_vecBingoBoard.size(); ++i )
	{
		if( m_vecBingoBoard[i].IsChanged() == true )
		{
			kPacket.m_mapBingoBoard.insert( std::map<byte, byte>::value_type( i, m_vecBingoBoard[i] ) );
			m_vecBingoBoard[i].SetValue( m_vecBingoBoard[i] );
			
			bsResult = BS_OK;
		}
	}

	// 3. 선물 정보 복사
	for( i=0; i < m_vecPresentInfo.size(); ++i )
	{
		if( ( m_vecPresentInfo[i].first.IsChanged() == true ) || ( m_vecPresentInfo[i].second.IsChanged() == true ) )
		{
			kPacket.m_mapPresentInfo.insert( std::map<byte, std::pair<int, bool> >::value_type( i, std::make_pair( m_vecPresentInfo[i].first, m_vecPresentInfo[i].second ) ) );
			m_vecPresentInfo[i].first.SetValue( m_vecPresentInfo[i].first );
			m_vecPresentInfo[i].second.SetValue( m_vecPresentInfo[i].second );

			bsResult = BS_OK;
		}
	}

	// 4. 로그 정보 복사
	if( m_vecLog.empty() == false )
	{
		kPacket.m_vecLog = m_vecLog;
		m_vecLog.clear();
		bsResult = BS_OK;
	}
	
	return bsResult;
}

bool KGSBingoEvent::Complete( IN const KDBE_BINGO_EVENT_INFO_WRITE_ACK& kPacket_ )
{
	bool iOK = true;
	
	m_iBalance.AdjustInitValue( m_iBalance.GetInitValue() - kPacket_.m_iBalance );
	m_iLotteryChance.AdjustInitValue( m_iLotteryChance.GetInitValue() - kPacket_.m_iLotteryChance );
	m_iLotteryRecord.AdjustInitValue( m_iLotteryRecord.GetInitValue() - kPacket_.m_iLotteryRecord );
	m_iMixChance.AdjustInitValue( m_iMixChance.GetInitValue() - kPacket_.m_iMixChance );
	m_iResetChance.AdjustInitValue( m_iResetChance.GetInitValue() - kPacket_.m_iResetChance );

	for( byte i=0; i < m_vecBingoBoard.size(); ++i )
	{
		std::map<byte, byte>::const_iterator it = kPacket_.m_mapBingoBoard.find( i );
		if( it == kPacket_.m_mapBingoBoard.end() )
		{
			continue;
		}

		// 다시 DB에 쓰기 위해 Init Value를 임의로 조작해서 증분을 만든다.
		m_vecBingoBoard[i].AdjustInitValue( !m_vecBingoBoard[i].GetInitValue() );
	}

	for( byte i=0; i < m_vecPresentInfo.size(); ++i )
	{
		std::map<byte, std::pair<int, bool> >::const_iterator it = kPacket_.m_mapPresentInfo.find( i );
		if( it == kPacket_.m_mapPresentInfo.end() )
		{
			continue;
		}

		// 다시 DB에 쓰기 위해 Init Value를 임의로 조작해서 증분을 만든다.
		m_vecPresentInfo[i].first.AdjustInitValue( !m_vecPresentInfo[i].first );
		m_vecPresentInfo[i].second.AdjustInitValue( !m_vecPresentInfo[i].second );
	}

	// 로그 정보 정리
	m_vecLog = kPacket_.m_vecLog;

	int iResult = ( iOK ) ? NetError::NET_OK : NetError::ERR_UNKNOWN;
	
	switch( kPacket_.m_usAckEventID )
	{
	case DBE_BINGO_EVENT_LOTTERY_ACK:
		m_TempLotteryAck.m_iOK	= iResult;
		m_bBusy					= false;
		break;

	case DBE_BINGO_EVENT_MIX_ACK:
		m_TempMixAck.m_iOK		= iResult;
		m_bBusy					= false;
		break;

	case DBE_BINGO_EVENT_RESET_ACK:
		m_TempResetAck.m_iOK	= iResult;
		m_bBusy					= false;
		break;

	case DBE_BINGO_EVENT_INIT_ACK:
		m_TempInitAck.m_iOK		= iResult;
		break;
	}

	return iOK;
}

KGSBingoEvent::BingoState KGSBingoEvent::GetBingoEventInfo( OUT KEGS_BINGO_EVENT_INFO_ACK& kPacket ) const
{
	if( ( m_vecBingoBoard.empty() == true ) && ( m_vecPresentInfo.empty() == true ) )
	{
		return BS_NONEDATA;
	}
	else if(( m_vecBingoBoard.size() == BSV_BINGOBOARD_SLOT_NUM	) &&
			( m_vecPresentInfo.size() == BSV_PRESENT_NUM		) )
	{
		kPacket.m_iLotteryChance	= m_iLotteryChance;
		kPacket.m_iLotteryRecord	= m_iLotteryRecord;
		kPacket.m_iMixChance		= m_iMixChance;
		kPacket.m_iResetChance		= m_iResetChance;
		CopyTheBingoBoard( kPacket.m_vecBingoBoard );
		CopyThePresentInfo( kPacket.m_vecPresentInfo );
		return BS_OK;
	}
	else
	{
		// 데이터 쓰기나 읽기에 실패한 비정상 상태이다.
		// 복구를 해주기 전까진 아무것도 할 수 없다!
		return BS_UNKNOWN;
	}
}

//{{ 2012. 09. 25	박세훈	빙고 이벤트 테스트
KGSBingoEvent::BingoState KGSBingoEvent::SetBingoEventInfo( IN const UidType iUnitUID , IN const KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket_ )
{
	m_vecBingoBoard.clear();
	m_vecPresentInfo.clear();

	// 기타 정보 세팅
	m_iBalance.SetValue( kPacket_.m_iBalance );
	m_iLotteryChance.SetValue( kPacket_.m_iLotteryChance );
	m_iLotteryRecord.SetValue( kPacket_.m_iLotteryRecord );
	m_iMixChance.SetValue( kPacket_.m_iMixChance );
	m_iResetChance.SetValue( kPacket_.m_iResetChance );

	// DB에 기존 데이터가 존재하지 않는 경우
	if( kPacket_.m_vecBingoBoard.empty() && kPacket_.m_vecPresentInfo.empty() )
	{
		InitTheBoard( iUnitUID );
	}
	else if(( kPacket_.m_vecBingoBoard.size() == BSV_BINGOBOARD_SLOT_NUM	) &&
			( kPacket_.m_vecPresentInfo.size() == BSV_PRESENT_NUM			) )
	{
		// 기존 데이터가 존재하는 경우
		
		// 빙고판 번호 세팅
		SetTheBingoBoard( kPacket_.m_vecBingoBoard );

		// 선물 정보 세팅
		SetThePresentInfo( kPacket_.m_vecPresentInfo );

		// 줄이 전부 완성되어 있다면, 초기화 한번 해주자!
		// check the complete line
		unsigned int iCompleteLineCount = 0;
		for( std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::iterator it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
		{
			if( it->second == true )
			{
				++iCompleteLineCount;
			}
		}

		if( BSV_PRESENT_NUM <= iCompleteLineCount )
		{
			InitTheBoard( iUnitUID );
		}
	}
	else
	{
		// 데이터 쓰기나 읽기에 실패한 비정상 상태이다.
		// 복구를 해주기 전까진 아무것도 할 수 없다!
		return BS_UNKNOWN;
	}
	return BS_OK;
}

KGSBingoEvent::BingoState KGSBingoEvent::GetBingoBoard( OUT std::vector<byte>& vecBingoBoard ) const
{
	vecBingoBoard.clear();
	for( std::vector< KCacheData<byte> >::const_iterator it=m_vecBingoBoard.begin(); it != m_vecBingoBoard.end(); ++it )
	{
		vecBingoBoard.push_back( *it );
	}
	return BS_OK;
}
KGSBingoEvent::BingoState KGSBingoEvent::GetPresentInfo( OUT std::vector<int>& vecPresentInfo ) const
{
	vecPresentInfo.clear();
	for( std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::const_iterator it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
	{
		vecPresentInfo.push_back( it->first );
	}
	return BS_OK;
}

KGSBingoEvent::BingoState KGSBingoEvent::GetLotteryNum( IN const UidType iUnitUID )
{
	if( m_bBusy == true )
	{
		return BS_BUSY;
	}
	m_bBusy = true;

	m_TempLotteryAck.clear();

	if( m_iLotteryChance < 1 )
	{
		return BS_NONE_CHANCE;
	}
	else if( m_vecBingoBoard.empty() || m_vecPresentInfo.empty() )
	{
		return BS_UNKNOWN;
	}

	// check the complete line
	unsigned int iCompleteLineCount = 0;
	for( std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::iterator it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
	{
		if( it->second == true )
		{
			++iCompleteLineCount;
		}
	}

	if( BSV_PRESENT_NUM <= iCompleteLineCount )
	{
		return BS_UNKNOWN;
	}

	m_iLotteryChance -= 1;
	m_iLotteryRecord += 1;

	//
	m_TempLotteryAck.m_iLotteryChance	= m_iLotteryChance;
	m_TempLotteryAck.m_iLotteryRecord	= m_iLotteryRecord;

	// 랜덤한 숫자 하나 뽑아서 해당 위치에 획득 표시를 한다.
	m_TempLotteryAck.m_byteNum = rand() % BSV_NUM_MAX;
	int iPos = _InsertLotteryNum( m_TempLotteryAck.m_byteNum, m_TempLotteryAck.m_vecAcquiredPresent );
	
	WriteEventLog( iUnitUID, BSV_ACTION_LOG_LOTTERY, m_TempLotteryAck.m_byteNum, iPos, 0 );
	BOOST_TEST_FOREACH( const byte, iPos, m_TempLotteryAck.m_vecAcquiredPresent )
	{
		WriteEventLog( iUnitUID, BSV_ACTION_LOG_REWARD, m_TempLotteryAck.m_byteNum, iPos, m_vecPresentInfo[iPos].first );
	}
	return BS_OK;
}

KGSBingoEvent::BingoState KGSBingoEvent::MixTheBoard( IN const UidType iUnitUID )
{
	if( m_bBusy == true )
	{
		return BS_BUSY;
	}
	m_bBusy = true;

	m_TempMixAck.clear();

	if( m_iMixChance < 1 )
	{
		return BS_NONE_CHANCE;
	}
	else if( m_vecBingoBoard.empty() || m_vecPresentInfo.empty() )
	{
		return BS_UNKNOWN;
	}

	// check the complete line
	for( std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::iterator it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
	{
		if( it->second == true )
		{
			return BS_ERR_MIX_RESTRICT;
		}
	}

	int iOpenNumCount = 0;
	for( std::vector< KCacheData<byte> >::iterator it=m_vecBingoBoard.begin(); it != m_vecBingoBoard.end(); ++it )
	{
		if( BSV_NUM_MAX <= *it )
		{
			++iOpenNumCount;
		}
	}
	if( BSV_MIX_RESTRICT_OPEN_COUNT <= iOpenNumCount )
	{
		return BS_ERR_MIX_RESTRICT;
	}

	m_iMixChance -= 1;

	//
	m_TempMixAck.m_iMixChance = m_iMixChance;

	std::vector<byte> vecOpenNum;
	_MixTheBoard( true, vecOpenNum );

	BOOST_TEST_FOREACH( byte, byteNum, vecOpenNum )
	{
		_InsertLotteryNum( byteNum, m_TempMixAck.m_vecAcquiredPresent );

		WriteEventLog( iUnitUID, BSV_ACTION_LOG_MIX, -1, -1, -1 );
		BOOST_TEST_FOREACH( const byte, iPos, m_TempMixAck.m_vecAcquiredPresent )
		{
			WriteEventLog( iUnitUID, BSV_ACTION_LOG_REWARD, -1, iPos, m_vecPresentInfo[iPos].first );
		}
	}

	GetBingoBoard( m_TempMixAck.m_vecBingoBoard );

	return BS_OK;
}

KGSBingoEvent::BingoState KGSBingoEvent::ResetTheBoard( IN const UidType iUnitUID )
{
	if( m_bBusy == true )
	{
		return BS_BUSY;
	}
	m_bBusy = true;

	m_TempResetAck.clear();

	if( m_iResetChance < 1 )
	{
		return BS_NONE_CHANCE;
	}
	else if( m_vecBingoBoard.empty() || m_vecPresentInfo.empty() )
	{
		return BS_UNKNOWN;
	}

	if( _MixThePresent() == BS_OK )
	{
		_MixTheBoard( false );
		// 귀퉁이 4군데는 기본적으로 열어준다.
		m_vecBingoBoard[0]													+= BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_LINE_SIZE - 1]						+= BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_SLOT_NUM - BSV_BINGOBOARD_LINE_SIZE] += BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_SLOT_NUM - 1]						+= BSV_NUM_MAX;
		
		m_iResetChance -= 1;

		//
		m_TempResetAck.m_iResetChance = m_iResetChance;

		GetBingoBoard( m_TempResetAck.m_vecBingoBoard );
		GetPresentInfo( m_TempResetAck.m_vecPresentInfo );
		WriteEventLog( iUnitUID, BSV_ACTION_LOG_RESET, -1, -1, -1 );

		return BS_OK;
	}
	else
	{
		return BS_ERR_MIX_THE_PRESENT;
	}
}

KGSBingoEvent::BingoState KGSBingoEvent::InitTheBoard( IN const UidType iUnitUID )
{
	m_TempInitAck.clear();

	if( _MixThePresent() == BS_OK )
	{
		_MixTheBoard( false );
		// 귀퉁이 4군데는 기본적으로 열어준다.
		m_vecBingoBoard[0]													+= BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_LINE_SIZE - 1]						+= BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_SLOT_NUM - BSV_BINGOBOARD_LINE_SIZE] += BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_SLOT_NUM - 1]						+= BSV_NUM_MAX;

		GetBingoBoard( m_TempInitAck.m_vecBingoBoard );
		GetPresentInfo( m_TempInitAck.m_vecPresentInfo );
		WriteEventLog( iUnitUID, BSV_ACTION_LOG_INIT, -1, -1, -1 );
		return BS_OK;
	}
	else
	{
		return BS_ERR_MIX_THE_PRESENT;
	}
}

void KGSBingoEvent::AddBalance( IN const int iBalance )
{
	m_iBalance			+= iBalance;
	m_iLotteryChance	+= ( m_iBalance / BSV_LOTTERY_BALANCE );
	m_iBalance.SetAliveValue( m_iBalance % BSV_LOTTERY_BALANCE );
}

void KGSBingoEvent::_MixTheBoard( IN const bool bOnlyMove )
{
	std::vector<byte> vecOpenNum;
	_MixTheBoard( bOnlyMove, vecOpenNum );
}

void KGSBingoEvent::_MixTheBoard( IN const bool bOnlyMove, OUT std::vector<byte>& vecOpenNum )
{
	std::list<byte>::iterator it;

	if( bOnlyMove == true )
	{
		std::list<byte> listCandidate;
		BOOST_TEST_FOREACH( byte, iBingoNum, m_vecBingoBoard )
		{
			if( BSV_NUM_MAX <= iBingoNum )
			{
				iBingoNum -= BSV_NUM_MAX;
				vecOpenNum.push_back( iBingoNum );
			}
			listCandidate.push_back( iBingoNum );
		}
		m_vecBingoBoard.clear();

		byte iNum = 0;
		while( listCandidate.empty() == false )
		{
			it = listCandidate.begin();
			iNum = rand() % listCandidate.size();
			while( 0 < iNum )
			{
				++it;
				--iNum;
			}
			KCacheData<byte> temp;
			temp.SetValue( -1 );
			temp.SetAliveValue( *it );
			m_vecBingoBoard.push_back( temp );
			listCandidate.erase( it );
		}
	}
	else
	{
		std::list<byte> listCandidate;
		for( byte i=0; i < BSV_NUM_MAX; ++i )
		{
			listCandidate.push_back( i );
		}
		m_vecBingoBoard.clear();

		byte iNum = 0;
		for( int i=0; i < BSV_BINGOBOARD_SLOT_NUM; ++i )
		{
			it = listCandidate.begin();
			iNum = rand() % listCandidate.size();
			while( 0 < iNum )
			{
				++it;
				--iNum;
			}
			KCacheData<byte> temp;
			temp.SetValue( -1 );
			temp.SetAliveValue( *it );
			m_vecBingoBoard.push_back( temp );
			listCandidate.erase( it );
		}
	}
}

KGSBingoEvent::BingoState KGSBingoEvent::_MixThePresent( void )
{
	std::vector<int>	vecPresent;
	std::vector<int>	vecPresentBan;
	std::list<byte>		listSpot;

	SiKGSBingoEventInfo()->GetPresentInfo( vecPresent );
	SiKGSBingoEventInfo()->GetBanPresentInfo( vecPresentBan );

	// 선물 정보를 비운다.
	std::vector< std::pair< KCacheData<int>, KCacheData<bool> > > vecPresentInfoBackUp;
	vecPresentInfoBackUp = m_vecPresentInfo;
	
	if( m_vecPresentInfo.size() < BSV_PRESENT_NUM )
	{
		m_vecPresentInfo.resize( BSV_PRESENT_NUM );
	}

	// 자리 인덱스를 수집한다.
	for( int i=0; i < BSV_PRESENT_NUM; ++i )
	{
		listSpot.push_back( i );
	}

	int iNum;
	std::list<byte>::iterator it;
	// 자리 인덱스가 남아 있다면 루프를 돈다.
	while( listSpot.empty() == false )
	{
		// 남은 자리 인덱스 중 임의의 하나를 꺼낸다.
		iNum = rand() % listSpot.size();
		it = listSpot.begin();
		while( 0 < iNum )
		{
			++it;
			--iNum;
		}

		// 자리 인덱스 감소
		iNum = *it;
		listSpot.erase( it );

		// BanSpot에는 일반 아이템만 넣을 수 있다.
		if( SiKGSBingoEventInfo()->IsBanSpot( iNum ) == true )
		{
			if( vecPresent.empty() == true )
			{
				// 일반 아이템이 더 이상 남아있지 않다. 정보 불일치
				m_vecPresentInfo = vecPresentInfoBackUp;
				return BS_ERR_MIX_THE_PRESENT;
			}
			else
			{
				// 일반 아이템 중 하나를 꺼내서 선물 정보에 저장한다.
				m_vecPresentInfo[iNum].first.SetAliveValue( vecPresent.back() );
				m_vecPresentInfo[iNum].second.SetAliveValue( false );
				vecPresent.pop_back();
			}
		}
		else
		{
			// BanSpot이 아니라면 BanPresent도 일반 Present도 넣을 수 있다.
			// 우선적으로 BanPresent를 넣어 주자.
			if( vecPresentBan.empty() == false )
			{
				// BanPresent 중 하나를 꺼내서 선물 정보에 저장한다.
				m_vecPresentInfo[iNum].first.SetAliveValue( vecPresentBan.back() );
				m_vecPresentInfo[iNum].second.SetAliveValue( false );
				vecPresentBan.pop_back();
			}
			else if( vecPresent.empty() == false )
			{
				// 일반 아이템 중 하나를 꺼내서 선물 정보에 저장한다.
				m_vecPresentInfo[iNum].first.SetAliveValue( vecPresent.back() );
				m_vecPresentInfo[iNum].second.SetAliveValue( false );
				vecPresent.pop_back();
			}
			else
			{
				// 공간이 남았는데 더이상 넣을 정보가 없다.
				m_vecPresentInfo = vecPresentInfoBackUp;
				return BS_ERR_MIX_THE_PRESENT;
			}
		}
	}

	return BS_OK;
}

int KGSBingoEvent::_InsertLotteryNum( IN const byte byteNum, IN OUT std::vector<byte>& vecAcquiredPresent )
{
	if( m_vecBingoBoard.empty() || m_vecPresentInfo.empty() )
	{
		return BS_UNKNOWN;
	}

	byte i;
	for( i=0; i < m_vecBingoBoard.size(); ++i )
	{
		if( m_vecBingoBoard[i] == byteNum )
		{
			break;
		}
	}

	// 빙고판 안에 있는 숫자인지, 이미 열려 있는지 검사한다.
	if( i == m_vecBingoBoard.size() )
	{
		// 빙고판에 없거나 이미 열린 숫자다.
		return -1;
	}

	byte iPos = i;
	m_vecBingoBoard[iPos] += BSV_NUM_MAX;

	// 완성한 줄 체크
	// 1. 행 검사
	byte iValue = ( iPos / BSV_BINGOBOARD_LINE_SIZE ) * BSV_BINGOBOARD_LINE_SIZE;
	for( i=0; i < BSV_BINGOBOARD_LINE_SIZE; ++i )
	{
		if( m_vecBingoBoard[iValue+i] < BSV_NUM_MAX )
		{
			break;
		}
	}

	// 행이 완성되었다.
	if( i == BSV_BINGOBOARD_LINE_SIZE )
	{
		iValue /= BSV_BINGOBOARD_LINE_SIZE;
		// 해당 줄의 선물에 성공 표시를 하고, 패킷에 선물 위치를 저장한다.
		m_vecPresentInfo[iValue].second.SetAliveValue( true );
		vecAcquiredPresent.push_back( iValue );
	}

	// 2. 열 검사
	iValue = iPos % BSV_BINGOBOARD_LINE_SIZE;
	for( i=0; i < BSV_BINGOBOARD_SLOT_NUM; i+=BSV_BINGOBOARD_LINE_SIZE )
	{
		if( m_vecBingoBoard[iValue+i] < BSV_NUM_MAX )
		{
			break;
		}
	}

	// 열이 완성되었다.
	if( i == BSV_BINGOBOARD_SLOT_NUM )
	{
		// 해당 줄의 선물에 성공 표시를 하고, 패킷에 선물 위치를 저장한다.
		iValue += BSV_BINGOBOARD_LINE_SIZE;
		m_vecPresentInfo[iValue].second.SetAliveValue( true );
		vecAcquiredPresent.push_back( iValue );
	}

	// 3. 대각선1 검사
	iValue = BSV_BINGOBOARD_LINE_SIZE + 1;

	// 대각1에 포함되는 숫자인가?
	if( ( ( iPos % iValue ) == 0 ) && ( ( iPos / iValue ) < BSV_BINGOBOARD_LINE_SIZE ) )
	{
		for( i=0; i < BSV_BINGOBOARD_LINE_SIZE; ++i )
		{
			if( m_vecBingoBoard[i*iValue] < BSV_NUM_MAX )
			{
				break;
			}
		}

		// 대각선이 완성되었다.
		if( i == BSV_BINGOBOARD_LINE_SIZE )
		{
			iValue = BSV_PRESENT_NUM-1;
			// 해당 줄의 선물에 성공 표시를 하고, 패킷에 선물 위치를 저장한다.
			m_vecPresentInfo[iValue].second.SetAliveValue( true );
			vecAcquiredPresent.push_back( iValue );
		}
	}

	// 3. 대각선2 검사
	iValue = BSV_BINGOBOARD_LINE_SIZE - 1;

	// 대각2에 포함되는 숫자인가?
	if( ( ( iPos % iValue ) == 0 ) && ( ( iPos / iValue ) <= BSV_BINGOBOARD_LINE_SIZE ) )
	{
		for( i=1; i <= BSV_BINGOBOARD_LINE_SIZE; ++i )
		{
			if( m_vecBingoBoard[i*iValue] < BSV_NUM_MAX )
			{
				break;
			}
		}

		// 대각선이 완성되었다.
		if( BSV_BINGOBOARD_LINE_SIZE < i )
		{
			iValue = BSV_PRESENT_NUM-2;
			// 해당 줄의 선물에 성공 표시를 하고, 패킷에 선물 위치를 저장한다.
			m_vecPresentInfo[iValue].second.SetAliveValue( true );
			vecAcquiredPresent.push_back( iValue );
		}
	}
	return iPos;
}

void KGSBingoEvent::SetTheBingoBoard( IN const std::vector<byte>& vecBingoBoard )
{
	m_vecBingoBoard.clear();
	KCacheData<byte> temp;

	BOOST_TEST_FOREACH( const byte, iValue, vecBingoBoard )
	{
		temp.SetValue( iValue );
		m_vecBingoBoard.push_back( temp );
	}
}

void KGSBingoEvent::SetThePresentInfo( IN const std::vector< std::pair<int, bool> >& vecPresentInfo )
{
	m_vecPresentInfo.clear();
	std::pair< KCacheData<int>, KCacheData<bool> > temp;
	
	for( size_t i=0; i < vecPresentInfo.size(); ++i )
	{
		temp.first.SetValue( vecPresentInfo[i].first );
		temp.second.SetValue( vecPresentInfo[i].second );
		m_vecPresentInfo.push_back( temp );
	}
}

void KGSBingoEvent::CopyTheBingoBoard( OUT std::vector<byte>& vecBingoBoard ) const
{
	vecBingoBoard.clear();
	BOOST_TEST_FOREACH( const KCacheData<byte>&, iValue, m_vecBingoBoard )
	{
		vecBingoBoard.push_back( iValue );
	}
}
void KGSBingoEvent::CopyThePresentInfo( OUT std::vector< std::pair<int, bool> >& vecPresentInfo ) const
{
	vecPresentInfo.clear();
	for( size_t i=0; i < m_vecPresentInfo.size(); ++i )
	{
		vecPresentInfo.push_back( m_vecPresentInfo[i] );
	}
}

void KGSBingoEvent::WriteEventLog(	IN const UidType iUnitUID, 
									IN const byte iActionType, 
									IN const char iOpenNum,	
									IN const char iPos, 
									IN const int iItemID
									)
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	KBingoEventLog kBingoEventLog;
	kBingoEventLog.iUnitUID		= iUnitUID;
	kBingoEventLog.iActionType	= iActionType;
	kBingoEventLog.iOpenNum		= iOpenNum;
	kBingoEventLog.iPos			= iPos;
	kBingoEventLog.iItemID		= iItemID;
	kBingoEventLog.wstrRegDate	= tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	m_vecLog.push_back( kBingoEventLog );
}

void KGSBingoEvent::AddMixChance( IN const unsigned short iQuantity )
{
	m_iMixChance += iQuantity;
}

void KGSBingoEvent::AddResetChance( IN const unsigned short iQuantity )
{
	m_iResetChance += iQuantity;
}

KGSBingoEvent::BingoState KGSBingoEvent::GetProductNo( IN const byte iPos, OUT unsigned long& ulProductNo )
{
	if( SiKGSBingoEventInfo()->GetPresentProductNoFromItemID( m_vecPresentInfo[iPos].first, ulProductNo ) == false )
	{
		return BS_UNKNOWN;
	}
	return BS_OK;
}

int KGSBingoEvent::GetProductItemID( IN const byte iPos )
{
	if( iPos < m_vecPresentInfo.size() )
	{
		return m_vecPresentInfo[iPos].first;
	}
	return 0;
}

void KGSBingoEvent::SetPresentInfoForBuy( IN const std::vector<byte>& vecAcquiredPresent )
{	
	BOOST_TEST_FOREACH( byte, iPos, vecAcquiredPresent )
	{
		m_setGetThePresent.insert( m_vecPresentInfo[iPos].first );
	}
}

//void KGSBingoEvent::GetPresentInfoForBuy( OUT std::vector<byte>& vecAcquiredPresent )
//{
//	int i=0;
//	std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::iterator it;
//	
//	for( it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
//	{
//		if( ( it->second.IsChanged() == true ) && ( it->second == true ) )
//		{
//			vecAcquiredPresent.push_back( i );
//			m_setGetThePresent.insert( m_vecPresentInfo[i].first );
//		}
//		++i;
//	}
//}

bool KGSBingoEvent::CheckAndRemoveIsPresentedItem( IN const unsigned long ulOrderNo )
{
	for( std::vector<unsigned long>::iterator it=m_vecPresentOrderNo.begin(); it != m_vecPresentOrderNo.end(); ++it )
	{
		if( *it == ulOrderNo )
		{
			m_vecPresentOrderNo.erase( it );
			return true;
		}
	}

	return false;
}

bool KGSBingoEvent::IsBingoComplete( void )
{
	BOOST_TEST_FOREACH( const KCacheData<byte>&, iValue, m_vecBingoBoard )
	{
		if( iValue < BSV_NUM_MAX )
		{
			return false;
		}
	}
	return true;
}

bool KGSBingoEvent::BingoEventRewardRepeater( IN const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo )
{
	std::vector< std::pair< std::vector< KNXBTPurchaseReqInfo >, byte > >::iterator it;
	for( it = m_vecRewardRepeateInfo.begin(); it != m_vecRewardRepeateInfo.end(); ++it )
	{
		bool bFind = true;

		// ACK의 물품 정보를 순회하면서,
		BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kInfo, vecPurchaseReqInfo )
		{
			bool bCheck = false;

			// 대조.
			BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kNXBTPurchaseReqInfo, it->first )
			{
				if( ( kInfo.m_ulProductNo == kNXBTPurchaseReqInfo.m_ulProductNo ) &&
					( kInfo.m_usOrderQuantity == kNXBTPurchaseReqInfo.m_usOrderQuantity )
					)
				{
					// 이건 존재한다. 다음 아이템을 검사하자
					bCheck = true;
					break;
				}
			}

			if( bCheck == false )
			{
				// 이 그룹엔 없다. 다음 그룹 검사하자!
				bFind = false;
				break;
			}
		}

		if( bFind == true )
		{
			// 일치하는 것을 찾았다.
			return true;
		}
	}
	
	return false;
}

bool KGSBingoEvent::IsBingoEventRewardRepeater( IN const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo )
{
	if( vecPurchaseReqInfo.empty() == true )
	{
		return false;
	}

	// 전부 빙고 이벤트 보상이 맞는가?
	BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kInfo, vecPurchaseReqInfo )
	{
		if( SiKGSBingoEventInfo()->IsBingoPresent( kInfo.m_ulProductNo ) == false )
		{
			return false;
		}
	}

	std::vector< std::pair< std::vector< KNXBTPurchaseReqInfo >, byte > >::iterator it;
	for( it = m_vecRewardRepeateInfo.begin(); it != m_vecRewardRepeateInfo.end(); ++it )
	{
		bool bFind = true;

		// ACK의 물품 정보를 순회하면서,
		BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kInfo, vecPurchaseReqInfo )
		{
			bool bCheck = false;

			// 대조.
			BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kNXBTPurchaseReqInfo, it->first )
			{
				if( ( kInfo.m_ulProductNo == kNXBTPurchaseReqInfo.m_ulProductNo ) &&
					( kInfo.m_usOrderQuantity == kNXBTPurchaseReqInfo.m_usOrderQuantity )
					)
				{
					// 이건 존재한다. 다음 아이템을 검사하자
					bCheck = true;
					break;
				}
			}

			if( bCheck == false )
			{
				// 이 그룹엔 없다. 다음 그룹 검사하자!
				bFind = false;
				break;
			}
		}

		if( bFind == true )
		{
			// 일치하는 것을 찾았다.
			if( it->second < SiKGameSysVal()->GetBingoEventPayoutRepeate() )
			{
				++it->second;
				START_LOG( cwarn, L"TESTLOG - 빙고 보상 지급 재시도 횟수" )
					<< BUILD_LOG( it->second )
					<< END_LOG;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	m_vecRewardRepeateInfo.push_back( std::make_pair( vecPurchaseReqInfo, 1 ) );
	return true;
}
#endif SERV_EVENT_BINGO
//}}