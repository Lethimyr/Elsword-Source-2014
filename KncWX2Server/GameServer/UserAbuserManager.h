#pragma once
#include "CommonPacket.h"


SmartPointer(KGSUser);


//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER


class KUserAbuserManager
{
public:
	enum ABUSER_ENUM
	{
		AE_ED_ABUSER_LIMIT = 1000000000, // 10억 이상 ED보유시 ED어뷰저로 판단!
		//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_LEVEL
		AE_AUTO_HACK_CHECK_LEVEL = 7,
#endif SERV_AUTO_HACK_CHECK_LEVEL
		//}}
	};

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-01	// 박세훈
	enum HackingUserCheckResult
	{
		HUCR_SUCCEED	= 0,
		HUCR_FAILED		= 1,
		HUCR_NOT_INITED	= 2,
	};

	struct KHackingUserCheckCount
	{
		bool			m_bChanged;
		byte			m_byteBlockCheckCount;
		std::wstring	m_wstrLastResetDate;

		KHackingUserCheckCount()
			: m_bChanged( false )
			, m_byteBlockCheckCount( 0 )
		{
		}
	};
	typedef	std::map<byte, KHackingUserCheckCount>	TYPE_HACKING_USER_CHECK_COUNT;
#endif // SERV_HACKING_USER_CHECK_COUNT

protected:
	enum ACCOUNT_ABUSER_TYPE
	{
		AT_MORNITORING_ABUSER = 0,

		AT_MAX,
	};

	enum CHAR_ABUSER_TYPE
	{
		CAT_ITEM_ABUSER = 0,
		CAT_ENCHANT_ABUSER,
		CAT_ED_ABUSER,
		//{{ 2010. 9. 8	최육사	네트워크 트래픽 유발 유저 모니터링
#ifdef SERV_TRAFFIC_USER_MONITORING
		CAT_TRAFFIC_ABUSER,
#endif SERV_TRAFFIC_USER_MONITORING
		//}}

		CAT_MAX,
	};	

public:
	KUserAbuserManager(void);
	~KUserAbuserManager(void);

	void Clear( bool bOnlyCharAbuser = false );
	void InitAccountAbuser( IN KGSUserPtr spUser );
	void InitCharAbuser( IN KGSUserPtr spUser );

	bool IsEnchantAbuser()			{ return m_bIsCharAbuser[CAT_ENCHANT_ABUSER]; }	
	//{{ 2010. 9. 8	최육사	네트워크 트래픽 유발 유저 모니터링
#ifdef SERV_TRAFFIC_USER_MONITORING
	bool IsTrafficAbuser()			{ return m_bIsCharAbuser[CAT_TRAFFIC_ABUSER]; }
#endif SERV_TRAFFIC_USER_MONITORING
	//}}

	// 어뷰저 등록 함수
	void RegEDAbuser( IN KGSUserPtr spUser );
	void RegItemAbuser( IN KGSUserPtr spUser );
	//{{ 2011. 09. 08	최육사	오토핵 패킷 모니터링
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
	void RegPacketMornitoring( IN KGSUserPtr spUser );
#endif SERV_AUTO_HACK_PACKET_MORNITOING
	//}}

	// 어뷰저 체크 함수
	//{{ 2013. 05. 29	최육사	패킷 모니터링 근성도 항목 추가
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
	void CheckMornitoringAbuser( IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iED, IN const int iSpirit, IN const unsigned short usEventID );
#else
	void CheckMornitoringAbuser( IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iED, IN const unsigned short usEventID );
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
	//}}	
	void CheckItemAbuser( IN KGSUserPtr spUser, IN const int iGetItemState, IN const int iItemID, IN const int iQuantity );
	void CheckItemAbuser( IN KGSUserPtr spUser, IN const int iGetItemState, IN const std::map< int, int >& mapGetItemInfo );
	void CheckEDAbuser( IN KGSUserPtr spUser, IN const int iGetEDState, IN const int iBeforeED );
	//{{ 2010. 9. 8	최육사	네트워크 트래픽 유발 유저 모니터링
#ifdef SERV_TRAFFIC_USER_MONITORING
	void CheckTrafficAbuser( IN unsigned short usEventID );
	void GetTrafficEventCount( OUT std::map< unsigned short, int >& m_mapTrafficEventCount ) const;
#endif SERV_TRAFFIC_USER_MONITORING
	//}}
	//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	void IncreaseGetItemCount( IN KGSUserPtr spUser, IN int iCount );
	int GetItemCount() const		{ return m_iGetItemCount; }
	int GetItemCountMin() const		{ return m_iGetItemCountMin; }
	int GetItemCountMax() const		{ return m_iGetItemCountMax; }
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
	//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_LEVEL
	bool IsAutoHackLevelUpUser( IN u_char ucCurrentCharLevel
#ifdef SERV_AUTO_HACK_CHECK_LEVEL_FIX
								, UidType iCurrentCharUID
#endif // SERV_AUTO_HACK_CHECK_LEVEL_FIX
		);
#endif SERV_AUTO_HACK_CHECK_LEVEL
	//}}
	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	void SetTradeBlockUser( IN const bool bTradeBlock ) { m_bIsTradeBlock = bTradeBlock; }
	bool IsTradeBlockUser()			{ return m_bIsTradeBlock; }
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	void SetTradeBlockUnit( IN const bool bIsTradeBlockUnit ) { m_bIsTradeBlockUnit = bIsTradeBlockUnit; }
	bool IsTradeBlockUnit()			{ return m_bIsTradeBlockUnit; }
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifdef SERV_MORNITORING_ABUSER_NOTIFY_SMS// 작업날짜: 2013-05-22	// 박세훈
	bool	IsMornitoringAbuser( void ){	return m_bIsAccountAbuser[AT_MORNITORING_ABUSER];	}
#endif // SERV_MORNITORING_ABUSER_NOTIFY_SMS

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-05-30	// 박세훈
	int		IncreaseCheckCount( IN const byte byteType, OUT byte& byteResult, OUT KAbuserLogManager::KHackingUserCheckInfo& kInfo );
	void	InitCheckCount( IN const byte byteType, IN const byte byteBlockCheckCount, IN const std::wstring& wstrLastResetDate );
	void	GetDBUpdateInfo( OUT std::map<byte, KHackingUserCheckCountDB>& mapHackingUserCheckCountDB ) const;
#endif // SERV_HACKING_USER_CHECK_COUNT

protected:
	bool CheckAbuserEvent( unsigned short usEventID );

protected:
	bool							m_bIsAccountAbuser[AT_MAX];
	bool							m_bIsCharAbuser[CAT_MAX];

	//{{ 2010. 9. 8	최육사	네트워크 트래픽 유발 유저 모니터링
#ifdef SERV_TRAFFIC_USER_MONITORING
	std::map< unsigned short, int >	m_mapTrafficEventCount;
#endif SERV_TRAFFIC_USER_MONITORING
	//}}

	//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	int								m_iGetItemCount;
	int								m_iGetItemCountMin;
	int								m_iGetItemCountMax;
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
	//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_LEVEL
	CTime							m_tLastLevelUpTime;
#ifdef SERV_AUTO_HACK_CHECK_LEVEL_FIX
	UidType							m_iLastLevelUpUID;
#endif // SERV_AUTO_HACK_CHECK_LEVEL_FIX
#endif SERV_AUTO_HACK_CHECK_LEVEL
	//}}
	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	bool							m_bIsTradeBlock;
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	bool							m_bIsTradeBlockUnit;
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-05-30	// 박세훈
	TYPE_HACKING_USER_CHECK_COUNT	m_mapHackingUserCheckCount;	// HackingType, Count
#endif // SERV_HACKING_USER_CHECK_COUNT
};


#endif SERV_USER_ABUSER_MANAGER
//}}
