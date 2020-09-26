#pragma once

#include "RoomList.h"
#include "Enum/Enum.h"

class KPVPList : public KRoomList
{
	DeclToStringW;
	NiDeclareRTTI;

public:
	KPVPList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum );
	virtual ~KPVPList(void);

	virtual UidType		GetWaitingRoomUID( IN KEGS_QUICK_JOIN_REQ& kReq );

	virtual void		SetVSPoint( IN int iMin, IN int iMax )		{ m_iMinVSPoint = iMin; m_iMaxVSPoint = iMax; }
	virtual void		GetVSPoint( OUT int& iMin, OUT int& iMax )	{ iMin = m_iMinVSPoint; iMax = m_iMaxVSPoint; }

	virtual void		SetPVPChannelClass( IN int iPVPChannelClass )	{ m_ePVPChannelClass = (KPVPChannelInfo::PVP_CHANNEL_CLASS)iPVPChannelClass; }
	virtual int			GetPVPChannelClass()							{ return (int)m_ePVPChannelClass; }

	//방리스트 관리함수
	virtual bool		AddRoomInfo( KRoomInfoPtr spRoomInfo );
	virtual bool		DelRoomInfo( KRoomInfoPtr spRoomInfo );
	virtual bool		GetRoomInfoPage( IN KEGS_ROOM_LIST_REQ& kReq, OUT KEGS_ROOM_LIST_ACK& kAck );

protected:

	int					m_iMinVSPoint;
	int					m_iMaxVSPoint;

	KPVPChannelInfo::PVP_CHANNEL_CLASS	m_ePVPChannelClass;

	std::vector< KRoomInfoPtr >	m_vecRoomList;
	std::vector< KRoomInfoPtr >	m_vecTypeRoomList[CXSLRoom::PGT_END];	//game type 별 방리스트
};

DeclOstmOperatorW( KPVPList );



