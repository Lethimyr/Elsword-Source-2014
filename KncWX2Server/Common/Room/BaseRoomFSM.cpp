#include ".\BaseRoomFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KBaseRoomFSM::" L ## #id,
const wchar_t*  KBaseRoomFSM::ms_szState[] = 
{ 
#   include "BaseRoomFSMState_def.h" 
};

const wchar_t*  KBaseRoomFSM::ms_szInput[] = 
{ 
#   include "BaseRoomFSMInput_def.h" 
};

KBaseRoomFSM::KBaseRoomFSM() : FSMclass( S_INIT )
{
	// S_INIT
	FSMstate* pState = new FSMstate( S_INIT, 1 );
	pState->AddTransition( I_TO_WAIT,           S_WAIT );
	AddState( pState );

	// S_CLOSE
	pState = new FSMstate( S_CLOSE, 1 );
	pState->AddTransition( I_TO_INIT,           S_INIT );
	AddState( pState );

	// S_WAIT
	pState = new FSMstate( S_WAIT, 1 );
	pState->AddTransition( I_TO_CLOSE,          S_CLOSE );
	AddState( pState );
}

KBaseRoomFSM::~KBaseRoomFSM()
{
}


const wchar_t* KBaseRoomFSM::GetStateIDString(int iStateID_) const
{
	return ms_szState[iStateID_];
}


const wchar_t* KBaseRoomFSM::GetInputIDString(int iInputID_) const
{
	return ms_szInput[iInputID_];
}