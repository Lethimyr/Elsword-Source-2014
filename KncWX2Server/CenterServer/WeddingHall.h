#pragma once

#include "Room.h"
#include "TimerManager.h"

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "WeddingHallUserManager.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}



//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM


SmartPointer( KWeddingHall );

class KWeddingHall : public KRoom
{
    NiDeclareRTTI;
    DeclToStringW;

public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum StatDungeon
    {
#   include "StatDungeon_def.h"
    };

	enum TIMER_ENUM
	{
		TE_WAIT_TIMER			= 0,

		TE_MAX,
	};



public:
    KWeddingHall();
    virtual ~KWeddingHall();

    // override.
    virtual void Tick();

protected:

	template< class T > void BroadCast_RejectUser( u_short usEventID, const T& data, const UidType iRejectUserUID, int iOption = KRoomUserManager::UL_ALL );
	void _BroadCast_RejectUser( int iFlag_, const KEvent& kEvent_ , const UidType iRejectUserUID );
	
	// Get Set	
    virtual void SetRoomInfo( const KRoomInfo& kInfo );
	virtual void GetRoomInfo( KRoomInfo& kInfo );

    // override.
	virtual	void SendRoomListInfo( IN const int iCode );
    virtual void ProcessNativeEvent( const KEventPtr& spEvent );
    virtual void StartGame();
    virtual void StartPlay();
    virtual void EndPlay();
    virtual void EndGame();
    virtual int  DecideWinTeam();
    virtual int  GetPlayMode();
    virtual void HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS );
	virtual void CloseRoom();
	virtual float GetRemainingPlayTime(){ return m_fRemainingPlayTime; }
	virtual void DeleteZombie( std::vector<UidType>& vecZombieUnit );	//추후 pvp/dungeon 방별 개별처리가 필요할경우 재작성을 위해
	
	virtual void	TimeCountFinishProcess( IN const u_short usEventID ) { /*아무것도 안합니다.*/ }
	virtual	void	LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom ) { /*아무것도 안합니다.*/ }

	void	SetWeddingHallInfo( IN const KWeddingHallInfo& kInfo );
	void	GetWeddingHallInfo( OUT KWeddingHallInfo& kInfo );
	void	GetWeddingHallSlotInfo( std::vector< KRoomSlotInfo >& vecSlot, KRoomUserManager::USERLIST_TYPE eType = KRoomUserManager::UT_GAME );
	int		GetWeddingHallUID()			{ return m_kHallInfo.m_iWeddingUID; }

	void	GetUnitList( OUT std::vector< UidType >& vecUidList );
	void	CheckContinueTimer();

	void	CheckLoadingSuccess();
	void	OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck );

	bool	CheckWeddingHallInfo( KWeddingHallInfo& kWeddingInfo );
	bool	IsGroomOrBride( IN UidType iUnitUID );
	bool	IsGroom( IN UidType iUnitUID )	{ return ( m_kHallInfo.m_iGroom == iUnitUID ); }
	bool	IsBride( IN UidType iUnitUID )	{ return ( m_kHallInfo.m_iBride == iUnitUID ); }

	DECL_ON_FUNC( ERM_OPEN_WEDDING_HALL_REQ );
	DECL_ON_FUNC( ERM_JOIN_WEDDING_HALL_REQ );
	DECL_ON_FUNC( ERM_LEAVE_ROOM_REQ );
	_DECL_ON_FUNC( ERM_LEAVE_WEDDING_HALL_REQ, KEGS_LEAVE_WEDDING_HALL_REQ );
	DECL_ON_FUNC_NOPARAM( ERM_CHECK_START_WEDDING_REQ );
	_DECL_ON_FUNC( ERM_START_WEDDING_ASK_NOT, KEGS_START_WEDDING_ASK_ACK );
	DECL_ON_FUNC( DBE_WEDDING_COMPLETE_ACK );
	_DECL_ON_FUNC( ERM_SQUARE_UNIT_SYNC_DATA_REQ, KEGS_SQUARE_UNIT_SYNC_DATA_REQ );
	_DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );
	

protected:
	
	KWeddingHallUserManagerPtr			m_spUserManager;
	boost::timer						m_tTimer[TE_MAX];

	KWeddingHallInfo					m_kHallInfo;
};


template< class T >
void KWeddingHall::BroadCast_RejectUser( u_short usEventID, const T& data, const UidType iRejectUserUID, int iOption /*= BC_ALL */)
{
	KEvent kEvent;  // _BroadCast에서 clone할 것이므로 여기선 heap 아니어도 됨.
	kEvent.m_kDestPerformer.m_dwPerformerID = PI_GS_CHARACTER;
	kEvent.m_usEventID = usEventID;

	KSerializer ks;
	kEvent.m_kbuff.Clear();

	// serialize - only data
	ks.BeginWriting( &kEvent.m_kbuff );
	ks.Put( data );
	ks.EndWriting();

	_BroadCast_RejectUser( iOption, kEvent, iRejectUserUID );
}

#endif SERV_RELATIONSHIP_SYSTEM
//}


