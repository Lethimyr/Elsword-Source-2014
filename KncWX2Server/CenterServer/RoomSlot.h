#pragma once

#include "FSM/support_FSM.h"
#include "RoomUser.h"

#define KRoomSlotPtr KRoomSlot*;
#undef KRoomSlotPtr
SmartPointer( KRoomSlot );

struct KRoomSlotInfo;
class KRoomSlot
{
    DeclareFSM_MT;

public:
    KRoomSlot();
    KRoomSlot( int iSlotID );
    ~KRoomSlot();

    int  GetSlotID()                                { return m_iSlotID; }
    int  GetTeam()                                  { return m_iTeam; }
    void AssignTeam( int iGameMode );
    UidType GetCID();
    void GetRoomSlotInfo( KRoomSlotInfo& kInfo );

    bool IsOpened();
    bool IsOccupied();

    bool Enter( KRoomUserPtr spRoomUser );
    bool Leave();
    bool Open();
    bool Close();
    bool ToggleOpenClose();

    void ResetSlot();

protected:
    void InitSlot( int iSlotID );
    void SetSlotID( int iSlotID )                   { m_iSlotID = iSlotID; }
    void SetTeam( int iTeam )                       { m_iTeam = iTeam; }

public:

protected:
    int                     m_iSlotID;
    int                     m_iTeam;
    KRoomUserPtr            m_spRoomUser;
};
