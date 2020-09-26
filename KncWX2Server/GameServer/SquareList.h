#pragma once

#include "RoomList.h"

class KSquareList : public KRoomList
{
	DeclToStringW;
	NiDeclareRTTI;

public:
	KSquareList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum );
	virtual ~KSquareList(void);

	//광장리스트 관리함수
	virtual bool		AddSquareInfo( KSquareInfoPtr spSquareInfo );
	virtual bool		DelSquareInfo( KSquareInfoPtr spSquareInfo );
	virtual bool		GetSquareInfoPage( IN KEGS_SQUARE_LIST_REQ& kReq, OUT KEGS_SQUARE_LIST_ACK& kAck );

protected:
	std::vector< KSquareInfoPtr >		m_vecSquareList;
};

DeclOstmOperatorW( KSquareList );