#pragma once

#include "ServerDefine.h"
#include <vector>
#include <set>
#include "Event.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "CacheData.h"
#include "RefreshSingleton.h"

#define IN
#define OUT

//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
#include <KNCSingleton.h>
#include <boost/timer.hpp>

struct KJackPotCuponState
{
	std::wstring	m_wstrCuponNumber;
	UidType			m_iUserUID;
	UidType			m_iUnitUID;
	int				m_iState;

	KJackPotCuponState();

	KJackPotCuponState( IN const std::wstring& wstrCuponNumber, IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iState )
		: m_wstrCuponNumber( wstrCuponNumber )
		, m_iUserUID( iUserUID )
		, m_iUnitUID( iUnitUID )
		, m_iState( iState )
	{
	}
};

struct KJackPotEndProcessInfo
{
	std::wstring	m_wstrBeginDate;
	std::wstring	m_wstrEndDate;
	int				m_iValidCuponCount;

	KJackPotEndProcessInfo()
		: m_iValidCuponCount( 0 )
	{
	}

	KJackPotEndProcessInfo( IN const std::wstring& wstrBeginDate
						  , IN const std::wstring& wstrEndDate
						  , IN int iValidCuponCount
						  )
		: m_wstrBeginDate( wstrBeginDate )
		, m_wstrEndDate( wstrEndDate )
		, m_iValidCuponCount( iValidCuponCount )
	{
	}
};

class KJackPotEvent
{
public:
	enum CuponState
	{
		STATE0	= 0,
		STATE1	= 1,
	};

	KJackPotEvent( IN const std::wstring& wstrBeginDate, IN const std::wstring& wstrEndDate )
		: m_wstrBeginDate( wstrBeginDate )
		, m_wstrEndDate( wstrEndDate )
		, m_iValidCuponCount( 0 )
		, m_iBeginProcessGab( 0 )
		, m_bEventStart( false )
	{
	}
	~KJackPotEvent(){}

	bool	CheckTheBeginEndDate( IN const std::wstring& wstrBeginDate, IN const std::wstring& wstrEndDate ) const;
	void	EndProcess( OUT std::vector<KJackPotEndProcessInfo>& vecJackPotEndProcessInfo ) const;
	void	AddCupon( IN const std::wstring& wstrCuponNumber, IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iState );
	bool	IsValidEvent( void ) const;
	bool	ExtractCuponNumber( IN const UidType iUserUID, IN const UidType iUnitUID, OUT std::wstring& wstrCuponNumber, OUT int& iState );
	//		- 추출 시 Manager에 어디 이벤트인지 기록하자 ( 재삽입 시 사용 )
	bool	BeginProcess( void );
	void	ConfirmProcess( void );

private:
	std::vector<KJackPotCuponState>	m_vecCuponInfo;
	std::wstring					m_wstrBeginDate;
	std::wstring					m_wstrEndDate;
	int								m_iValidCuponCount;
	
	boost::timer					m_tTimer;
	__int64							m_iBeginProcessGab;
	bool							m_bEventStart;
};

class KJackPotEventManager
{
	DeclareSingleton( KJackPotEventManager );

protected:
	KJackPotEventManager(){}
	~KJackPotEventManager(){}

public:
	bool	Init( IN const std::vector<KJackPotEventInitInfo>& vecJackpotEventInitInfo );
	void	Tick( OUT std::vector<KJackPotEventWinInfo>& vecJackPotEventWinInfo, OUT std::vector<KJackPotEndProcessInfo>& vecJackPotEndProcessInfo );

	void	AddUser( IN const UidType iUserUID, IN const UidType iUnitUID );
	void	RemoveUser( IN const UidType iUserUID, IN const UidType iUnitUID );
	bool	LotteryUser( OUT UidType& iUserUID, OUT UidType& iUnitUID );
	void	ReInsertUser( IN const UidType iUserUID );

	void	ReInsert( IN const KJackPotEventWinInfo& kJackPotEventWinInfo );
	void	FaildProcess( IN const UidType iUserUID, IN const UidType iUnitUID, IN const std::wstring& wstrCuponNumber );

private:
	std::vector<KJackPotEvent>	m_vecJackPotEvent;					// 기간별 잭팟 이벤트 정보

	std::map<UidType, UidType>	m_mapConnectUserAndUnitUID;			// 현재 접속 중인 유저 정보
	std::set<UidType>			m_setWinnerUserUID;					// 당첨자 리스트
	std::map<UidType, UidType>	m_mapConnectUserInfoExceptWinner;	// 당첨자를 제외한 현재 접속 중인 유저 ( UserUID, UnitUID )

	boost::timer				m_tTimer;
};

DefSingletonInline( KJackPotEventManager );
#endif SERV_EVENT_JACKPOT
//}}

//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
class KGSBingoEventInfo
{
	DeclareRefreshSingleton( KGSBingoEventInfo );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

	KGSBingoEventInfo();
	~KGSBingoEventInfo();

public:
	void	AddPackageItemPriceForGift_LUA( IN unsigned long ulProductNo, IN unsigned long ulSalePrice );
	void	GetPackageItemPriceForGift( IN unsigned long ulProductNo, OUT unsigned long& ulSalePrice ) const;
	void	AddPackageItemPrice_LUA( IN unsigned long ulProductNo, IN unsigned long ulSalePrice );
	void	GetPackageItemPrice( IN unsigned long ulProductNo, OUT unsigned long& ulSalePrice ) const;
	void	AddPresentInfo_LUA( IN int iItemID, IN unsigned long ulProductNo, IN bool bIsBanPresent = false );
	bool	GetPresentProductNoFromItemID( IN const int iItemID, OUT unsigned long& ulProductNo ) const;
	int		GetBingoEventItemID( IN const unsigned long ulProductNo ) const;

	void	AddBanSpotInfo_LUA( IN byte bytePos );

	void	SetBillingIP( IN const std::string& strBillingIP ){	m_strBillingIP = strBillingIP;	}
	bool	CheckBillingIP_LUA( IN const char* szBillingIP ){	return m_strBillingIP == std::string( szBillingIP );	}

	void	GetPresentInfo( OUT std::vector<int>& vecPresent );
	void	GetBanPresentInfo( OUT std::vector<int>& vecBanPresent );
	bool	IsBanSpot( IN const byte byteSpotIndex );
	bool	IsBingoPresent( IN unsigned long ulProductNo );

private:
	std::map<unsigned long, unsigned long>	m_mapPackageItemPriceForGift;
	std::map<unsigned long, unsigned long>	m_mapPackageItemPrice;
	std::map<int, unsigned long>			m_mapAllPresentItemID;
	std::map<unsigned long, int>			m_mapAllPresentProductNo;
	std::set<unsigned long>					m_setPresent;
	std::set<unsigned long>					m_setBanPresent;
	std::set<byte>							m_setBanSpot;

	std::string								m_strBillingIP;
};
DefRefreshSingletonInline( KGSBingoEventInfo );

class KGSBingoEvent
{
	/*
	줄 수의 증감에 따른 변화가 제대로 적용되는지는 확인 해보아야 한다. 5x5 환경에서 동작하는 것만을 보장함.
	특정 동작에서 실패한다면, 현재 DB에 기록되어 있는 정보로 롤백하자...
	*/
public:
	enum BingoSetValue
	{
		BSV_BINGOBOARD_LINE_SIZE	= 5,
		BSV_BINGOBOARD_SLOT_NUM		= BSV_BINGOBOARD_LINE_SIZE * BSV_BINGOBOARD_LINE_SIZE,
		BSV_PRESENT_NUM				= 12,
		BSV_NUM_MIN					= 0,
		BSV_NUM_MAX					= 100,
		BSV_LOTTERY_BALANCE			= 900,
		BSV_MIX_RESTRICT_OPEN_COUNT	= 10,
		BSV_ACTION_LOG_LOTTERY		= 0,
		BSV_ACTION_LOG_MIX			= 1,
		BSV_ACTION_LOG_RESET		= 2,
		BSV_ACTION_LOG_INIT			= 3,
		BSV_ACTION_LOG_REWARD		= 4,
		BSV_MIX_ITEM_ID				= 257280,
		BSV_RESET_ITEM_ID			= 257290,
	};

	enum BingoState
	{
		BS_OK,
		BS_BUSY,
		BS_UNKNOWN,
		BS_NONEDATA,
		BS_NONE_CHANCE,
		BS_ERR_MIX_RESTRICT,
		BS_ERR_EXISTDATA,
		BS_ERR_MIX_THE_PRESENT,
	};

	static	bool	IsMixItem( IN const int iItemID ){	return iItemID == BSV_MIX_ITEM_ID;	}
	static	bool	IsResetItem( IN const int iItemID ){	return iItemID == BSV_RESET_ITEM_ID;	}

	KGSBingoEvent();

	bool		CheckBingoEventReward( IN const unsigned long& ulProductNo );
	bool		IsBingoComplete( void );

	// DB 데이터 읽고 쓰기
	BingoState	GetBingoEventInfoForDB( IN const unsigned short usHandlingEventID, OUT KDBE_BINGO_EVENT_INFO_WRITE_REQ& kPacket );
	bool		Complete( IN const KDBE_BINGO_EVENT_INFO_WRITE_ACK& kPacket_ );


	BingoState	GetBingoEventInfo( OUT KEGS_BINGO_EVENT_INFO_ACK& kPacket ) const;
	// Set 되기 전에 구입한 물품에 대한 DB 업데이트가 실패한다면, 무조건 적으로 덮어 씌우지 않는다.
	BingoState	SetBingoEventInfo( IN const UidType iUnitUID , IN const KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket_ );

	BingoState	GetBingoBoard( OUT std::vector<byte>& vecBingoBoard ) const;
	BingoState	GetPresentInfo( OUT std::vector<int>& vecPresentInfo ) const;
	
	BingoState	GetLotteryNum( IN const UidType iUnitUID );
	BingoState	MixTheBoard( IN const UidType iUnitUID );
	BingoState	ResetTheBoard( IN const UidType iUnitUID );
	BingoState	InitTheBoard( IN const UidType iUnitUID );

	void		AddBalance( IN const int iBalance );
	void		AddMixChance( IN const unsigned short iQuantity );
	void		AddResetChance( IN const unsigned short iQuantity );

	int			GetBalance( void ){	return m_iBalance;	}

	BingoState	GetProductNo( IN const byte iPos, OUT unsigned long& ulProductNo );
	int			GetProductItemID( IN const byte iPos );
	
	void		SetPresentInfoForBuy( IN const std::vector<byte>& vecAcquiredPresent );
	//void		GetPresentInfoForBuy( OUT std::vector<byte>& vecAcquiredPresent );

	KEGS_BINGO_EVENT_LOTTERY_ACK&	GetLotteryAck( void ){	return m_TempLotteryAck;	}
	KEGS_BINGO_EVENT_MIX_ACK&		GetMixAck( void ){		return m_TempMixAck;		}
	KEGS_BINGO_EVENT_RESET_ACK&		GetResetAck( void ){	return m_TempResetAck;		}
	KEGS_BINGO_EVENT_INIT_ACK&		GetInitAck( void ){		return m_TempInitAck;		}

	// 선물 받은 아이템의 예외 처리용
	void		AddOrderNoListForPresentedItem( IN const unsigned long ulOrderNo ){	m_setPresentOrderNoList.insert( ulOrderNo );	}
	void		ClearOrderNoListForPresentedItem( void ){	m_setPresentOrderNoList.clear();	}
	bool		IsPresentedItem( IN const unsigned long ulOrderNo ) const{	return m_setPresentOrderNoList.find( ulOrderNo ) != m_setPresentOrderNoList.end();	}

	void		AddOrderNoForPresentedItem( IN const unsigned long ulOrderNo ){	m_vecPresentOrderNo.push_back( ulOrderNo );	}
	bool		CheckAndRemoveIsPresentedItem( IN const unsigned long ulOrderNo );

	void		AddCurrentOrderNo( IN const unsigned long ulOrderNo ){	m_setCurrentOrderNoList.insert( ulOrderNo );	}
	void		ClearCurrentOrderNoList( void ){	m_setCurrentOrderNoList.clear();	}
	bool		IsCurrentOrderNoItem( IN const unsigned long ulOrderNo ) const{	return m_setCurrentOrderNoList.find( ulOrderNo ) != m_setCurrentOrderNoList.end();	}

	// 보상 아이템 구매 실패 시의 재지급 처리용
	bool		BingoEventRewardRepeater( IN const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo );
	bool		IsBingoEventRewardRepeater( IN const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo );

private:
	void		_MixTheBoard( IN const bool bOnlyMove );
	void		_MixTheBoard( IN const bool bOnlyMove, OUT std::vector<byte>& vecOpenNum );
	BingoState	_MixThePresent( void );
	int			_InsertLotteryNum( IN const byte byteNum, IN OUT std::vector<byte>& vecAcquiredPresent );
	
	void		SetTheBingoBoard( IN const std::vector<byte>& vecBingoBoard );
	void		SetThePresentInfo( IN const std::vector< std::pair<int, bool> >& vecPresentInfo );

	void		CopyTheBingoBoard( OUT std::vector<byte>& vecBingoBoard ) const;
	void		CopyThePresentInfo( OUT std::vector< std::pair<int, bool> >& vecPresentInfo ) const;

	void		WriteEventLog(	IN const UidType iUnitUID, 
								IN const byte iActionType, 
								IN const char iOpenNum,	
								IN const char iPos, 
								IN const int iItemID
								);
private:
	// 체크 정보
	bool															m_bBusy;
	std::set<int>													m_setGetThePresent;

	// 빙고 정보
	KCacheData<int>													m_iBalance;			// 잔액 정보
	KCacheData<int>													m_iLotteryChance;	// 뽑기 찬스 횟수
	KCacheData<int>													m_iLotteryRecord;	// 뽑은 횟수
	KCacheData<int>													m_iMixChance;		// 뒤섞기 찬스 횟수
	KCacheData<int>													m_iResetChance;		// 초기화 찬스 횟수
	std::vector< KCacheData<byte> >									m_vecBingoBoard;	// 현재 빙고 숫자판
	std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >	m_vecPresentInfo;	// 현재 선물 정보
	std::vector<KBingoEventLog>										m_vecLog;			// 로그 정보

	// 클라에게 던져줄 정보 임시 보관
	KEGS_BINGO_EVENT_LOTTERY_ACK									m_TempLotteryAck;
	KEGS_BINGO_EVENT_MIX_ACK										m_TempMixAck;
	KEGS_BINGO_EVENT_RESET_ACK										m_TempResetAck;
	KEGS_BINGO_EVENT_INIT_ACK										m_TempInitAck;

	// 검사에 필요한 임시 정보
	// 1. 캐쉬 인벤토리 리스트를 받아올 때마다 초기화 후 선물 받은 아이템 리스트 임시 저장
	// 2. 가져오기 시도할 때, 선물 받은 아이템이라면, 임시 저장
	// 3. 가져오기 완료 후, 시도 시 저장한 선물 받은 아이템이라면, 예외 처리
	std::set<unsigned long>											m_setCurrentOrderNoList;	// 현재 보고 있는 페이지의 캐쉬 아이템만 가져올 수 있다.
	std::set<unsigned long>											m_setPresentOrderNoList;	// 선물 받은 아이템의 예외 처리용 ( 선물 받은 아이템에는 빙고 기회가 충전되면 안된다. )
	std::vector<unsigned long>										m_vecPresentOrderNo;		// 선물 받은 아이템의 예외 처리용 ( 선물 받은 아이템에는 빙고 기회가 충전되면 안된다. )

	// 보상 아이템 구매 실패 시의 재지급 처리용
	std::vector< std::pair< std::vector< KNXBTPurchaseReqInfo >, byte > >	m_vecRewardRepeateInfo;
};
#endif SERV_EVENT_BINGO
//}}