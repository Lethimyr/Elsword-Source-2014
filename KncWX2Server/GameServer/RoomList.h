#pragma once

/*
※ room / square info를 포인터로 하는 이유는 매니져에서 방정보수정을 하고
각 채널에서 방리스트 관리를 하기 때문이며 매니져는 map을 이용한 관리때문에
잦은 정보 변경으로 사용하고 채널의 vector은 방리스트 구현에 편하기때문.

※ 매니져에서 부모포인터를 가지고 있기때문에 자식클래스에서 사용하게될 기능함수를
순수가상함수로 정의한다.
*/

#include <vector>
#include "UserManager.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLSquareGame.h"

// class
#define KRoomListPtr KRoomList*
#undef KRoomListPtr
SmartPointer( KRoomList );

#define KPVPListPtr KPVPList*
#undef KPVPListPtr
SmartPointer( KPVPList );

#define KSquareListPtr KSquareList*
#undef KSquareListPtr
SmartPointer( KSquareList );

// struct
#define KRoomInfoPtr KRoomInfo*
#undef KRoomInfoPtr
SmartPointers( KRoomInfo );

#define KSquareInfoPtr KSquareInfo*
#undef KSquareInfoPtr
SmartPointers( KSquareInfo );

class KRoomList
{
	DeclToStringW;
	NiDeclareRTTI;

public:
	enum ROOM_LIST_TYPE
	{
		RLT_NONE = 0,
		RLT_PVP,
		RLT_FREE_PVP,
		RLT_SQUARE,
		RLT_PARTY,

		RLT_END,
	};

public:
	KRoomList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum );
	virtual ~KRoomList(void);

	UINT			GetID()										{ return m_uiRoomListID; }

	void			SetType( ROOM_LIST_TYPE eRoomListType )		{ m_eRoomListType = eRoomListType; }
	ROOM_LIST_TYPE	GetType()									{ return m_eRoomListType; }

	UINT			GetPageNum()								{ return m_uiPageNum; }

public:
	//////////////////////////////////////////////////////////////////////////
	//하위클래스에서 사용하게될 함수목록.
	virtual UidType		GetWaitingRoomUID( IN KEGS_QUICK_JOIN_REQ& kReq )			{ return 0; }
	virtual void		SetVSPoint( IN int iMin, IN int iMax )		{}
	virtual void		GetVSPoint( OUT int& iMin, OUT int& iMax )	{}

	virtual bool		AddRoomInfo( KRoomInfoPtr spRoomInfo )		{ return false; }
	virtual bool		DelRoomInfo( KRoomInfoPtr spRoomInfo )		{ return false; }
	virtual bool		GetRoomInfoPage( IN KEGS_ROOM_LIST_REQ& kReq, OUT KEGS_ROOM_LIST_ACK& kAck ){ return false; }

	virtual bool		AddSquareInfo( KSquareInfoPtr spSquareInfo ){ return false; }
	virtual bool		DelSquareInfo( KSquareInfoPtr spSquareInfo ){ return false; }
	virtual bool		GetSquareInfoPage( IN KEGS_SQUARE_LIST_REQ& kReq, OUT KEGS_SQUARE_LIST_ACK& kAck ){ return false; }

	virtual void		SetPVPChannelClass( IN int iPVPChannelClass ){}
	virtual int			GetPVPChannelClass()						{ return 0; }
	//////////////////////////////////////////////////////////////////////////

protected:
	template< class T >	void PreInsertVector( std::vector<T>& vecList, T data );

protected:
	UINT			m_uiRoomListID;
	ROOM_LIST_TYPE	m_eRoomListType;
	UINT			m_uiPageNum;		// 한페이지에 몇개씩 줄것인지..!
};

template< class T >
void KRoomList::PreInsertVector( std::vector<T>& vecList, T data )
{
	if( vecList.empty() == true )
	{
		vecList.push_back( data );
	}
	else
	{
		vecList.insert( vecList.begin(), data );
	}
}

DeclOstmOperatorW( KRoomList );



