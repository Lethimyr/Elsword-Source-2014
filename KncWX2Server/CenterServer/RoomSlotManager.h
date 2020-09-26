#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "RoomSlotFSM.h"
#include "RoomSlot.h"

#define MAX_ROOM_MEMBER 8       // 나중에 적당한 위치로 옮기자.

SmartPointer( KRoomSlotFSM );

class KRoomSlotManager
{
public:
    KRoomSlotManager();
    ~KRoomSlotManager();

	KRoomSlotPtr Get( int iSlotID ) const;
    KRoomSlotPtr Get( int iSlotID );
    KRoomSlotPtr GetByCID( UidType nCID );

    void ResetSlot();
    void ResetSlotEvent();

    bool EnterSlot( char cSlotEvent, const KRoomUserInfo& kInfo, bool bConsiderTeam );
	
    bool LeaveSlot( char cSlotEvent, UidType nCID );
    bool LeaveSlot( char cSlotEvent, int iSlotID );
    bool LeaveSlot( char cSlotEvent, KRoomSlotPtr spRoomSlot );

    bool ChangeTeam( UidType nCID );
    bool ChangeTeam( int iSlotID );
    bool ChangeTeam( KRoomSlotPtr spRoomSlot );

	bool ToggleReady( UidType nCID );
	bool ToggleReady( int iSlotID );
    bool ToggleReady( KRoomSlotPtr spRoomSlot );
    void ForceReady( char cSlotEvent );
    void ForceNotReady( char cSlotEvent );

    bool ChangeMotion( char cSlotEvent, UidType nCID, int iMotion );
    bool ChangeMotion( char cSlotEvent, int iSlotID, int iMotion );
    bool ChangeMotion( char cSlotEvent, KRoomSlotPtr spRoomSlot, int iMotion );

    bool OpenSlot( int iSlotID );
    bool OpenSlot( KRoomSlotPtr spRoomSlot );

    bool CloseSlot( int iSlotID );
    bool CloseSlot( KRoomSlotPtr spRoomSlot );

    bool ToggleOpenClose( int iSlotID );
    bool ToggleOpenClose( KRoomSlotPtr spRoomSlot );

    bool SetLoadingProgress( UidType nCID, int iLoadingProgress );
    void ResetLoadingProgress();

    bool SetP2PConnected( UidType nCID, bool bP2PConnected );
    void ResetP2PConnected();

	bool SetLoading( UidType nCID, bool bLoading );
	void ResetLoading();

	bool SetDieUnit( UidType nCID, UidType& uiMurdererUnit );
	bool GetDieUnit( UidType nCID, KEGS_USER_UNIT_DIE_NOT& kInfo );
	void ResetDieUnit();

    bool GetRoomUserInfo( UidType nCID, KRoomUserInfo& kInfo );
    void GetRoomUserInfo( std::map< UidType, KRoomUserInfo >& mapRoomUserInfo );

    bool GetRoomSlotInfo( int iSlotID, KRoomSlotInfo& kInfo );
    bool GetRoomSlotInfo( KRoomSlotPtr spRoomSlot, KRoomSlotInfo& kInfo );
    void GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo, bool bResetSlotEvent );

    int  GetNumTotalSlot();
    int  GetNumOpenedSlot();
    int  GetNumOccupiedSlot();

    bool GetSlotID( UidType nCID, int& iSlotID );
    bool GetCID( int iSlotID, UidType& nCID );

    bool IsHost( UidType nCID );
    bool IsHost( int iSlotID );
    bool IsHost( KRoomSlotPtr spRoomSlot );

    bool IsAllPlayerReady();
    bool IsAllPlayerFinishLoading();
    bool IsAllPlayerP2PConnected();
	bool IsAllPlayerLoading();

protected:
    void MakeSlot( int i );
    void HandleInvalidPointer( int i );
    
    KRoomSlotPtr FindEmptySlot( bool bConsiderTeam );
    UidType AppointNewHost();

public:

protected:
    int                                 m_nRoomSlot;
    std::vector< KRoomSlotPtr >         m_vecRoomSlot;
    std::map< UidType, KRoomSlotPtr >   m_mapRoomSlotByCID;

    static FSMclassPtr                  ms_spFSM;
};
