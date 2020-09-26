#include "RoomSlotFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KRoomSlotFSM::" L ## #id,
const wchar_t*  KRoomSlotFSM::ms_szState[] = { 
#   include "RoomSlotFSMState_def.h" 
};

const wchar_t*  KRoomSlotFSM::ms_szInput[] = { 
#   include "RoomSlotFSMInput_def.h" 
};

KRoomSlotFSM::KRoomSlotFSM() : FSMclass( S_INIT )
{
    // S_INIT
    FSMstate* pState = new FSMstate( S_INIT, 2 );
    pState->AddTransition( I_TO_CLOSED,         S_CLOSED );
    pState->AddTransition( I_TO_ASSIGNED,       S_ASSIGNED );
    AddState( pState );

    // S_CLOSED
    pState = new FSMstate( S_CLOSED, 1 );
    pState->AddTransition( I_TO_INIT,           S_INIT );
    AddState( pState );

    // S_ASSIGNED
    pState = new FSMstate( S_ASSIGNED, 1 );
    pState->AddTransition( I_TO_INIT,           S_INIT );
    AddState( pState );
}

KRoomSlotFSM::~KRoomSlotFSM()
{
}


const wchar_t* KRoomSlotFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KRoomSlotFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}