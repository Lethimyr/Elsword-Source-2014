#pragma once

#include "RoomList.h"
#include <KNCSingleton.h>
#include <ToString.h>

class KRoomListManager
{
	DeclareSingleton( KRoomListManager );
	DeclDump;
	DeclToStringW;

	enum
	{
		PVP_PAGE_NUM	 = 12,
		SQUARE_PAGE_NUM	 = 6,
	};

public:
	KRoomListManager(void);
	~KRoomListManager(void);

	bool			OpenScriptFile( const char* pFileName );
	
	bool			OpenPVPRoomList( IN KGSUserPtr spUser, OUT KEGS_OPEN_PVP_ROOM_LIST_ACK& kInfo );
	bool			ChangePVPRoomList( IN KGSUserPtr spUser, IN UINT uiRoomListID );

	//gsuser에게 제공할 기능	
	KRoomList::ROOM_LIST_TYPE GetRoomListType( IN UINT uiRoomListID );

	int				GetPVPChannelClassByRoomListID( IN UINT uiRoomListID );
	int				GetPVPChannelClassByRoomUID( IN UidType iRoomUID );
	UidType			GetWaitingRoomUID( IN UINT uiRoomListID, IN KEGS_QUICK_JOIN_REQ& kReq );

	bool			GetRoomInfoPage( IN UINT uiRoomListID, IN KEGS_ROOM_LIST_REQ& kReq, OUT KEGS_ROOM_LIST_ACK& kAck );
	bool			GetSquareInfoPage( IN KEGS_SQUARE_LIST_REQ& kReq, OUT KEGS_SQUARE_LIST_ACK& kAck );	

	//////////////////////////////////////////////////////////////////////////
	// 방정보 관리
	bool			AddRoomInfo( const KRoomInfo& kRoomInfo );
	bool			ChangeRoomInfo( const KRoomInfo& kRoomInfo );
	KRoomInfoPtr	GetRoomInfo( const UidType nRoomUID );
	bool			DelRoomInfo( const KRoomInfo& kRoomInfo );
	int				DelRoomInfo( const UidType nCenterUID );	//센터서버가 죽은경우 해당센터에 만들어졌던 방을 삭제한다.

	//////////////////////////////////////////////////////////////////////////
	// 광장정보 관리
	bool			AddSquareInfo( const KSquareInfo& kSquareInfo );
	bool			ChangeSquareInfo( const KSquareInfo& kSquareInfo );
	KSquareInfoPtr	GetSquareInfo( const UidType iSquareUID );
	bool			DelSquareInfo( const KSquareInfo& kSquareInfo );
	int				DelSquareInfo( const UidType iCenterUID );

	//////////////////////////////////////////////////////////////////////////
	// 방관련 개발자에게 보낼정보
	void            GetNumPvPRoom( OUT std::map< int, int > &mapNumPvPRoom );
	void            ChangeNumPvPRoom( int iGameMode, int iChange );

	//{{ 2011. 07. 04	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	UINT			GetPvpOffcialRoomListID() const		{ return m_nPvpOffcialRoomListID; }
#endif SERV_PVP_NEW_SYSTEM
	//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	UINT			GetTournamentPVPID( void ) const{	return m_uiTournamentPVPID;	}
#endif	// SERV_HERO_PVP_MANAGE_LIST

private:
	KRoomListPtr	Get( UINT uiRoomListID );
	KRoomListPtr	GetPVPFreeChannel();

	bool			AddRoomListTemplet_LUA();
	void			SetStartPVPID_LUA( UINT nID )		{ m_nStartPVPID = nID; }

	//{{ 2011. 07. 04	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	void			SetPvpOffcialRoomListID_LUA( UINT nID );	
#endif SERV_PVP_NEW_SYSTEM
	//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	void			SetTournamentPVPID_LUA( UINT nID )		{ m_uiTournamentPVPID = nID; }
#endif	// SERV_HERO_PVP_MANAGE_LIST
	
	bool			InsertRoomInfo( IN UINT uiChannelID, IN KRoomInfoPtr spRoomInfo );
	bool			DeleteRoomInfo( IN UINT uiChannelID, IN KRoomInfoPtr spRoomInfo );

private:
	// 방 리스트를 관리할 컨테이너
	std::map< UINT, KRoomListPtr >		m_mapRoomList;  // [key : RoomListID]
	KSquareListPtr						m_spSquare;

	// 방 정보를 직접 참조할 컨테이너
	std::map< UidType, KRoomInfoPtr >	m_mapRoomInfoList;
	std::map< UidType, KSquareInfoPtr >	m_mapSquareInfoList;

	UINT								m_nStartPVPID;
#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	UINT								m_uiTournamentPVPID;
#endif	// SERV_HERO_PVP_MANAGE_LIST

	//{{ 2011. 07. 04	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	UINT								m_nPvpOffcialRoomListID;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	// 운영자에게 넘겨주는 서버 정보에 PvP Room 개수와 Dungeon Room 개수를 넘겨준다.
	std::map< int, int >                m_mapNumPvPRoom;
};

DefSingletonInline( KRoomListManager );



