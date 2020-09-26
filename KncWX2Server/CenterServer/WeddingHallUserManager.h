#pragma once

#include "RoomUserManager.h"

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM

#define KWeddingHallUserManagerPtr KWeddingHallUserManager*
#undef KWeddingHallUserManagerPtr
SmartPointer( KWeddingHallUserManager );


class KWeddingHallUserManager : public KRoomUserManager
{
public:
    KWeddingHallUserManager();
    virtual ~KWeddingHallUserManager();

	//{{ 2013. 01. 07	최육사	방에서 필드로 돌아오지 못하는 버그 수정
	virtual void Reset( USERLIST_TYPE eType = UT_GAME );
	//}}

	// 정보 얻기
	void	SetMaxSlot( IN const int iMaxSlot ) { m_MaxSlot = iMaxSlot; }
	int		GetMaxSlot() const				{ return m_MaxSlot; }

	// 확인
	bool	IsExistEmptySlot()				{ return ( GetMaxSlot() > GetNumMember() ); }

	// 입장 퇴장 처리
	bool	EnterRoom( IN const KRoomUserInfo& kInfo, IN const bool bConsiderTeam );
	virtual bool LeaveRoom( IN const UidType iUnitUID );

	// 동기화 처리
	bool	RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList );
	bool	RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iRegUnitUID );
	bool	UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList );
	bool	UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iUnregUnitUID );
	void	CheckAndGetIntrudeUnitLoadCompleteUserList( OUT std::vector< UidType >& vecNpcLoadUserList );

	bool	GetRoomUserList( std::map< UidType, KRoomUserPtr >& mapRoomUserList, USERLIST_TYPE eType = UT_GAME );
	void	GetRoomUserList( OUT std::vector< UidType >& vecUidList, USERLIST_TYPE eType = UT_GAME );

	void	GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo, USERLIST_TYPE eType = UT_GAME );

	bool	IsExistGroomAndBride( USERLIST_TYPE eType = UT_GAME );

protected:
	int										m_MaxSlot;
};


#endif SERV_RELATIONSHIP_SYSTEM
//}

