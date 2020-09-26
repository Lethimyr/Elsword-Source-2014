#include "RoomUserFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KRoomUserFSM::" L ## #id,
const wchar_t*  KRoomUserFSM::ms_szState[] = { 
#   include "RoomUserFSMState_def.h" 
};

const wchar_t*  KRoomUserFSM::ms_szInput[] = { 
#   include "RoomUserFSMInput_def.h" 
};

KRoomUserFSM::KRoomUserFSM() : FSMclass( S_INIT )
{
    // S_INIT
    FSMstate* pState = new FSMstate( S_INIT, 1 );
	pState->AddTransition( I_TO_LOAD,           S_LOAD );	
    AddState( pState );

    // S_LOAD
    pState = new FSMstate( S_LOAD, 2 );
    pState->AddTransition( I_TO_INIT,           S_INIT );
    pState->AddTransition( I_TO_PLAY,           S_PLAY );
    AddState( pState );

    // S_PLAY
    pState = new FSMstate( S_PLAY, 2 );
	pState->AddTransition( I_TO_INIT,           S_INIT );
    pState->AddTransition( I_TO_RESULT,         S_RESULT );    
    AddState( pState );

	// S_RESULT
	pState = new FSMstate( S_RESULT, 1 );
	pState->AddTransition( I_TO_INIT,           S_INIT );
	AddState( pState );
}

KRoomUserFSM::~KRoomUserFSM()
{
}

const wchar_t* KRoomUserFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}

const wchar_t* KRoomUserFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}