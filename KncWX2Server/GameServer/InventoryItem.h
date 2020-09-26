#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"

#include "CacheData.h"

#pragma pack( push, 1 )

class KInventoryItem
{
public:
    KInventoryItem();
    virtual ~KInventoryItem();

    void	Init( IN const KInventoryItemInfo& kInfo, IN const bool bNewItem );

	// 아이템 정보 얻기
	UidType GetItemUID() const						{ return m_iItemUID; }
	int		GetItemID()	const						{ return m_iItemID; }
	int		GetUsageType() const					{ return m_iUsageType; }
    int		GetQuantity() const;
	int		GetEndurance() const;
	int		GetEnchantLevel() const;
	//{{ 2009. 11. 9  최육사	트리플속성
	void	GetAttribEnchant( OUT KItemAttributeEnchantInfo& kInfo ) const;
	void	GetAttribEnchant( IN int iAttribEnchantSlotID, OUT int& iAttribEnchantID ) const;
	//}}	
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	const std::vector< int >& GetItemSocketList() const;
	bool	GetItemSocketOption( IN const int iSocketIndex, OUT int& iSocketOption );
#else
	const std::vector< short >& GetItemSocketList() const;
	bool	GetItemSocketOption( IN const int iSocketIndex, OUT short& sSocketOption );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	int		GetCountAssignedItemSocket() const;
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	const std::vector< int >& GetRandomSocketList() const;
	bool	GetRandomSocketOption( IN const int iSocketIndex, OUT int& iSocketOptionID );
	int		GetCountAssignedRandomSocket() const;
	void	SetRandomSocketOption( IN const std::vector< int > vecRandomSocket );
	char	GetItemState() const					{ return m_cItemState; }
	void	CheckAndUpdateItemState();
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	//{{ 2009. 8. 27  최육사	밀봉
	u_char	GetSealData() const						{ return m_ucSealData; }
	u_char	GetSealCount() const;
	//}}
    void	GetItemInfo( OUT KItemInfo& kInfo ) const;
	int		GetInventoryCategory() const			{ return m_iInventoryCategory; }
	int		GetSlotID() const						{ return m_iSlotID; }
    void	GetInventoryItemInfo( OUT KInventoryItemInfo& kInfo ) const;
	//{{ 2009. 10. 12  최육사	코드정리
	void	GetInventoryItemSimpleInfo( OUT KInventoryItemSimpleInfo& kInfo ) const;
	//}}

    bool	IsQuantityFull() const;
    bool	IsQuantityExhausted() const;
	bool	IsPeriodExpired() const;
	//{{ 2008. 12. 21  최육사	강화 깨짐
	bool	IsBroken() const;
	//}}
	//{{ 2009. 8. 27  최육사	밀봉
	bool	IsSealed() const;
	//}}
	//{{ 2010. 01. 04  최육사	PC방 프리미엄
	void	NotDBUpdateItem()						{ m_bIsDBUpdate = false; }
	bool	IsDBUpdate() const						{ return m_bIsDBUpdate; }
	//}}

    int		GetRemainedQuantityCapacity() const;
    int		GetEnduranceToRestore() const;

	void	IncreaseQuantity( IN int iQuantity, OUT int& iIncreased );
    void	DecreaseQuantity( IN int iQuantity, OUT int& iDecreased );
    void	IncreaseEndurance( IN int iEndurance, OUT int& iIncreased );
    void	DecreaseEndurance( IN int iEndurance, OUT int& iDecreased );
	void	IncreaseEnchantLevel( IN int iEnchantLevel, OUT int& iIncreased );
	void	DecreaseEnchantLevel( IN int iEnchantLevel, OUT int& iDecreased );
    void	SetPosition( IN int iCategory, IN int iSlotID );

	//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
	void	PrepareIncreaseQuantity( IN int iQuantity, OUT int& iIncreased );
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
	//}}

	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	void	SetPosition( IN int iCategory, IN int iSlotID, IN UidType iPetUID );
	void	UpdatePetUID( IN UidType iPetUID );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool	SetItemSocketOption( IN const int iSocketIndex, IN const int iSocketOption );
#else
	bool	SetItemSocketOption( IN const int iSocketIndex, IN const short sSocketOption );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	bool	SetAttribEnchant( IN char cAttribEnchantSlotID, IN char cAttribEnchantType );
	//{{ 2009. 8. 27  최육사	밀봉
	bool	Seal();
    bool	Unseal();
	//}}	

#ifdef	SERV_SHARING_BANK_TEST
	bool	IncreaseSealCount();
#endif	SERV_SHARING_BANK_TEST

	//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT	
	void	UpdatePeriod( IN short sPeriod, IN const std::wstring& wstrExpirationDate );
#endif SERV_RESET_PERIOD_EVENT
	//}}
	
    bool	FlushQuantityChange( OUT int& iChange );
    void	RollBackInitQuantity( IN int iChange );
	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	bool	FlushPositionChange( OUT int& iCategoryChange, OUT int& iSlotIDChange, OUT UidType& iPetUID );
	void	PositionUpdateComplete( IN int iCategoryChange, IN int iSlotIDChange, IN UidType iPetUID );
#else
	bool	FlushPositionChange( OUT int& iCategoryChange, OUT int& iSlotIDChange );
	void	PositionUpdateComplete( IN int iCategoryChange, IN int iSlotIDChange );
#endif SERV_PET_SYSTEM
	//}}	
	bool	FlushEnduranceChange( OUT int& iChange );
	void	RollBackInitEndurance( IN int iChange );

	//{{ 2011. 01. 06  김민성  스킬슬롯체인지 체크(인벤토리-기간제) 기능 구현
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
	void	GetExpirationDate( OUT std::wstring& wstrExpirationDate ) const { wstrExpirationDate = m_wstrExpirationDate; }
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY
	//}}

#ifdef	SERV_SHARING_BANK_TEST
	bool	GetIsChangedSlotID() { return m_iSlotID.IsChanged(); }

	KCacheData< int >	GetQuantityCache()		{ return m_iQuantity; }
	KCacheData< int >	GetEnduranceCache()		{ return m_iEndurance; }

	void	SetQuantityCache( KCacheData< int > iQuantity )		{ m_iQuantity = iQuantity; }
	void	SetEnduranceCache( KCacheData< int > iEndurance )	{ m_iEndurance = iEndurance; }

	UidType	GetPetUID() const					{ return m_iPetUID; }
#endif	SERV_SHARING_BANK_TEST

	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	void	SetItemID( IN int& iItemID )	{ m_iItemID = iItemID; }
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// 작업날짜: 2013-07-01	// 박세훈
	inline	bool	IsPeriod( void ) const{	return ( 0 < m_sPeriod );	}
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	void	UpdateExpandedSocketNum( IN const byte byteExpandedSocketNum ){	m_byteExpandedSocketNum = byteExpandedSocketNum;	}
	int		GetMaxSocketCount( void ) const;
#endif // SERV_BATTLE_FIELD_BOSS

protected:
    UidType						m_iItemUID;
    int							m_iItemID;
    int							m_iUsageType;
	int							m_iEnchantLevel;
	//{{ 2009. 11. 9  최육사	트리플속성
	KItemAttributeEnchantInfo	m_kAttribEnchantInfo;
	//}}
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >			m_vecItemSocket;
#else
	std::vector< short >		m_vecItemSocket;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::vector< int >			m_vecRandomSocket;
	char						m_cItemState;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	//{{ 2009. 8. 27  최육사	밀봉
	u_char						m_ucSealData;
	//}}
    short						m_sPeriod;
    std::wstring				m_wstrExpirationDate;
    KCacheData< int >			m_iQuantity;
	KCacheData< int >			m_iEndurance;
    KCacheData< int >			m_iInventoryCategory;
    KCacheData< int >			m_iSlotID;
	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	KCacheData< UidType	>		m_iPetUID;
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 01. 04  최육사	PC방 프리미엄
	bool						m_bIsDBUpdate;
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	byte						m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
};

#pragma pack( pop )


