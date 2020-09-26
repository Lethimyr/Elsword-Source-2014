#pragma once

#include "TradeUser.h"

class KTradeUserManager
{
public:
    KTradeUserManager(void);
   ~KTradeUserManager(void);

    void Init( IN int nMaxUser );
    void Reset();
    KTradeUserPtr GetUser( IN UidType nCID );

    bool AddUser( IN KTradeUserPtr spRoomUser );
    bool DeleteUser( IN UidType nCID );
    int  DeleteUserByGsUID( IN UidType nGsUID, OUT std::vector< UidType >& vecDelUserUID );   // 해당 게임서버에 접속된 유저는 모두 지운다. (지운 유저수를 리턴)

    bool EnterTrade( IN const KTradeUserInfo& kInfo );
    bool LeaveTrade( IN UidType nCID );

    int  GetNumMember(){ return static_cast<int>( m_mapTradeUser.size() ); }
	int  GetMaxMember(){ return static_cast<int>( m_dwMaxUser ); }

	bool GetTradeUserGS( UidType nCID, UidType& nGSUID );
	void GetUserList( OUT std::map< UidType, std::set< UidType > >& mapUidList ); // Server UID 별로 모은 User UID.
	void GetUserInfoList( OUT std::vector< KTradeUserInfo >& vecTradeUserInfo );
	bool GetOtherCID( IN UidType nCID, OUT UidType& nOtherCID );
	bool GetOtherTradeUserInfo( IN UidType nCID, OUT KTradeUserInfo& kOther );
    KTradeUserPtr GetOtherUser( IN UidType nCID );

	bool GetTradeItem( IN UidType nCID, IN UidType iItemUID, OUT KInventoryItemInfo& kInfo );
	bool GetTradeItem( IN UidType nCID, OUT std::vector< KInventoryItemInfo >& vecTradeItemInfo );	
	bool GetTradeED( IN UidType nCID, OUT int& iED );
	bool UpdateTradeItem( IN UidType nCID, IN KInventoryItemInfo& kInfo );
	bool UpdateTradeED( IN UidType nCID, IN int iED );
	bool RemoveTradeItem( IN UidType nCID, IN UidType iItemUID );
	bool Offer( IN UidType nCID );
	bool Approve( IN UidType nCID );
	bool Confirm( IN UidType nCID, IN const KERM_CONFIRM_TRADE_ACK& kInfo );
	bool IsOffered();	
	bool IsApproved();	
	bool IsConfirmed();
    void GetOfferState( OUT std::map< UidType, bool >& mapOfferState );
    bool IsThereNothingToTrade();
	bool IsCanInsertTradeItem( IN UidType nCID );
	//{{ 2009. 4. 7  최육사		아이템업데이트 가능 상태인지 검사
	bool CheckStateUpdateTradeItem( IN UidType nCID );
	//}}

	//{{ 2010. 05. 22  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	bool IsAllTradeUsersSameServerGroup( IN UidType iUnitUID );
#endif SERV_INTEGRATION
	//}}

	//{{ 2009. 2. 10  최육사	개인거래 버그예방
	bool GetUpdateTemp( IN UidType nCID, OUT KERM_CONFIRM_TRADE_NOT& kInfo );
	//}}

	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	bool		IsDBUpdateReady() const;
	bool		SetDBUpdateInfo( IN const UidType iUnitUID, IN const KERM_DB_UPDATE_TRADE_RESULT_NOT& kInfo );
	bool		GetDBUpdateInfo( IN const UidType iUnitUID, IN OUT std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >& vecDBUpdateInfo );
#endif SERV_TRADE_DDOS_DEFENCE
	//}}

protected:
	KTradeUserPtr	MakeUser( IN const KTradeUserInfo& kInfo );

protected:
	std::map< UidType, KTradeUserPtr > m_mapTradeUser;

	DWORD	m_dwMaxUser;
};


