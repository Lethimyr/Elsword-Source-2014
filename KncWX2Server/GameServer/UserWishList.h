#pragma once
#include <map>

// [참고] 장바구니에서 SlotData로 취급되는 ItemID들은 중복되지 않는다.

// 장바구니는 유저단위로 존재함. 따라서 캐릭터 선택시에 초기화 시키지 않는다.


class KUserWishList
{
private:
    enum WishListSlot
	{
		SLOT_MAX = 21,
	};

	class KWishSlotData
	{
	public:
		KWishSlotData() { m_iItemID = 0; m_iBeforeItemID = 0; }
		~KWishSlotData() {}

		int GetSlotData() const { return m_iItemID; }
		bool IsChanged() { return ( m_iItemID != m_iBeforeItemID ); }
		void SetValue( int iItemID ) { m_iItemID = iItemID; m_iBeforeItemID = iItemID; }

		void operator=( int iItemID )
		{
			m_iBeforeItemID = m_iItemID;
			m_iItemID = iItemID;
		}
		
	private:
		int m_iItemID;
		int m_iBeforeItemID;
	};

public:
	KUserWishList(void);
	~KUserWishList(void);

	void InitWishList( const std::map< int, int >& mapWishList );
	void GetWishList( std::map< int, int >& mapWishList );
	void FlushWishListChange( std::map< int, int >& mapWishList );

	bool AddWish( int iItemID );
	bool DelWish( int iItemID );

	bool GetBuyCashItemInWishList() { return m_bBuyCashItemInWishList; }
	void SetBuyCashItemInWishList( bool bValue ) { m_bBuyCashItemInWishList = bValue; }
	
private:
	KWishSlotData	m_arrWishList[SLOT_MAX];
	bool			m_bBuyCashItemInWishList;
};



