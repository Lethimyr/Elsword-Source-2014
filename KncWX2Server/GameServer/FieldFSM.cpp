#include ".\FieldFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KFieldFSM::" L ## #id,
const wchar_t*  KFieldFSM::ms_szState[] = 
{ 
#   include "FieldFSMState_def.h" 
};

const wchar_t*  KFieldFSM::ms_szInput[] = 
{ 
#   include "FieldFSMInput_def.h" 
};

KFieldFSM::KFieldFSM() : FSMclass( S_INIT )
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

KFieldFSM::~KFieldFSM()
{
}


const wchar_t* KFieldFSM::GetStateIDString(int iStateID_) const
{
	return ms_szState[iStateID_];
}


const wchar_t* KFieldFSM::GetInputIDString(int iInputID_) const
{
	return ms_szInput[iInputID_];
}