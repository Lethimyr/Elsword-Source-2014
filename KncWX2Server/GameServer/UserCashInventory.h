#pragma once
#include "Event.h"
#include "CommonPacket.h"

class KUserCashInventory
{
public:
	KUserCashInventory(void);
	~KUserCashInventory(void);

	void Clear();

	// 실시간 부활석
	bool IsCashResurrectionStone() { return m_bCashResurrectionStone; }
	void SetCashResurrectionStone( bool bValue ) { m_bCashResurrectionStone = bValue; }
	UidType GetCashRStoneUnitUID() { return m_iCashRStoneUnitUID; }
	void SetCashRStoneUnitUID( UidType iUnitUID ) { m_iCashRStoneUnitUID = iUnitUID; }

	// 패키지 픽업
	void ClearPickUpPackageInfo() { m_mapPickUpPackageInfo.clear(); m_vecUpdatedInventorySlot.clear(); }
	bool IsEmptyPickUpPackageInfo() { return m_mapPickUpPackageInfo.empty(); }
	void AddPickUpPackageInfo( unsigned long ulProductNo, const KNXBTPickUpPackageInfo& kPickUpPackageInfo );
	void DelPickUpPackageInfo( unsigned long ulProductNo );
	bool GetPickUpPackageInfo( unsigned long ulProductNo, KNXBTPickUpPackageInfo& kPickUpPackageInfo );

	void AddUpdatedInventorySlot( const std::vector< KInventoryItemInfo >& vecUpdatedSlotInfo );
	void GetUpdatedInventorySlot( std::vector< KInventoryItemInfo >& vecUpdatedSlotInfo ) { vecUpdatedSlotInfo = m_vecUpdatedInventorySlot; }

	// 선물하기
	const std::wstring& GetReceiverUserID() { return m_wstrReceiverUserID; }
	void SetReceiverUserID( const std::wstring& wstrUserID ) { m_wstrReceiverUserID = wstrUserID; }
	const std::wstring& GetMessage() { return m_wstrMessage; }
	void SetMessage( const std::wstring& wstrMessage ) { m_wstrMessage = wstrMessage; }

	// 캐쉬 아이템 소켓
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool GetSocketOptionForCash( int iItemID, int& iSocketOption );
#else
	bool GetSocketOptionForCash( int iItemID, short& sSocketOption );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	bool SetSocketForCash( const std::map< int, int >& mapSocketForCash, const std::map< int, int >& mapSocketGroupIDForCash );
#else
	void SetSocketForCash( const std::map< int, int >& mapSocketForCash ) { m_mapSocketForCash = mapSocketForCash; }
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}


private:
	// 실시간 부활석
	bool							m_bCashResurrectionStone;
	UidType							m_iCashRStoneUnitUID;

	// 패키지 픽업
	std::map< unsigned long, KNXBTPickUpPackageInfo >	m_mapPickUpPackageInfo;
	std::vector< KInventoryItemInfo >					m_vecUpdatedInventorySlot;

	// 선물하기
	std::wstring					m_wstrReceiverUserID;
	std::wstring					m_wstrMessage;

	// 캐쉬 아이템 소켓
	std::map< int, int >			m_mapSocketForCash;
};



