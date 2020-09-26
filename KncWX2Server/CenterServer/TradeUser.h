#pragma once

#include "FSM/support_FSM.h"
#include <boost/timer.hpp>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#define KTradeUserPtr KTradeUser*
#undef KTradeUserPtr
SmartPointer( KTradeUser );

class KTradeUser
{
public:
	KTradeUser();
	virtual ~KTradeUser(void);

	void		GetTradeUserInfo( KTradeUserInfo& kInfo )			{ kInfo = m_kTradeUserInfo; }
	void		SetTradeUserInfo( const KTradeUserInfo& kInfo )		{ m_kTradeUserInfo = kInfo; }

	UidType		GetGSUID()											{ return m_kTradeUserInfo.m_iGSUID; }
	UidType		GetCID()											{ return m_kTradeUserInfo.m_iUnitUID; }
	//{{ 2009. 4. 28  최육사	최대ED제한
	int			GetCurrentED()										{ return m_kTradeUserInfo.m_iED; }
	//}}
	bool		GetTradeItemInfo( UidType iItemUID, KInventoryItemInfo& kInfo );
	void		GetTradeItemInfo( std::vector< KInventoryItemInfo >& vecTradeItemInfo ) { vecTradeItemInfo = m_vecTradeItemInfo; }	
	bool		InsertTradeItemInfo( const KInventoryItemInfo& kInfo );
	bool		RemoveTradeItemInfo( UidType iItemUID );
	int			GetTradeED()										{ return m_iTradeED; }
	bool		SetTradeED( int iTradeED );
	
	bool		IsTradeItemEmpty()									{ return m_vecTradeItemInfo.empty(); }
	bool		IsCanInsertTradeItem()								{ return ( m_vecTradeItemInfo.size() < 10 ); }

	bool		IsOffered()											{ return m_bOffer; }
	bool		IsApproved()										{ return m_bApproval; }
	bool		IsConfirmed()										{ return m_bConfirm; }
	void		Offer()												{ m_bOffer = true; }
    void        CancelOffer()                                       { m_bOffer = false; }
	void		Approve()											{ m_bApproval = true; }
	//{{ 2009. 2. 10  최육사	개인거래 버그 예방
	void		Confirm( const KERM_CONFIRM_TRADE_ACK& kInfo );
	void		GetUpdateTemp( KERM_CONFIRM_TRADE_NOT& kInfo );
	//}}
	//{{ 2009. 4. 28  최육사	최대ED제한
	bool		CheckMaxED( __int64 iAddED );
	//}}
	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	bool		IsDBUpdateReady() const								{ return !m_vecDBUpdateInfo.empty(); }
	void		SetDBUpdateInfo( IN const KERM_DB_UPDATE_TRADE_RESULT_NOT& kInfo );
	bool		GetDBUpdateInfo( IN OUT std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >& vecDBUpdateInfo ) const;
#endif SERV_TRADE_DDOS_DEFENCE
	//}}

protected:
	KTradeUserInfo				m_kTradeUserInfo;
	bool						m_bOffer;
	bool						m_bApproval;
	bool						m_bConfirm;

	//{{ 2009. 1. 7  최육사		거래템 10개 확장
	std::vector< KInventoryItemInfo > m_vecTradeItemInfo;
	//}}
	int							m_iTradeED;	

	//{{ 2009. 2. 10  최육사	개인거래 버그 예방
	KItemQuantityUpdate			m_kItemQuantityUpdateTemp;
	std::vector< KItemInfo >	m_vecItemInfoTemp;
	int							m_iEDTemp;
	//}}
	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT > m_vecDBUpdateInfo;
#endif SERV_TRADE_DDOS_DEFENCE
	//}}
};
