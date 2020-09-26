#pragma once

#include "FSM/support_FSM.h"
#include <boost/timer.hpp>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#define KPersonalShopUserPtr KPersonalShopUser*
#undef KPersonalShopUserPtr
SmartPointer( KPersonalShopUser );

class KPersonalShopUser
{
public:
	KPersonalShopUser();
	virtual ~KPersonalShopUser(void);

	void		GetPersonalShopUserInfo( KPersonalShopUserInfo& kInfo )			{ kInfo = m_kPersonalShopUserInfo; }
	void		SetPersonalShopUserInfo( const KPersonalShopUserInfo& kInfo )	{ m_kPersonalShopUserInfo = kInfo; }

	UidType		GetGSUID()	{ return m_kPersonalShopUserInfo.m_iGSUID; }
	UidType		GetCID()	{ return m_kPersonalShopUserInfo.m_iUnitUID; }
	std::wstring	GetNickName(){ return m_kPersonalShopUserInfo.m_wstrNickName; }

	void		SetHost( bool bIsHost ){ m_bIshost = bIsHost; }
	bool		IsHost(){ return m_bIshost; }

#ifdef DEF_TRADE_BOARD
	// 거래유저의 위치..!
	// true : 광장
	// false : 필드 거래게시판
	bool		IsSquare(){return m_kPersonalShopUserInfo.m_bIsSquare;}
#endif DEF_TRADE_BOARD

	//{{ 2009. 2. 10  최육사	개인거래 버그 예방
	void		SetUpdateTemp( const KItemQuantityUpdate& kUpdateTemp, const std::vector< KItemInfo >& vecItemInfoTemp, int iEDTemp );
	void		GetUpdateTemp( KItemQuantityUpdate& kUpdateTemp, std::vector< KItemInfo >& vecItemInfoTemp, int& iEDTemp );
	//}}

protected:
	KPersonalShopUserInfo		m_kPersonalShopUserInfo;

	bool						m_bIshost;

	//{{ 2009. 2. 10  최육사	개인거래 버그 예방
	KItemQuantityUpdate			m_kItemQuantityUpdateTemp;
	std::vector< KItemInfo >	m_vecItemInfoTemp;
	int							m_iEDTemp;
	//}}
};