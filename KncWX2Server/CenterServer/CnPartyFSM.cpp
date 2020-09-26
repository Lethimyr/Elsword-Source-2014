#include "CnPartyFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KCnPartyFSM::" L ## #id,
const wchar_t*  KCnPartyFSM::ms_szState[] = { 
#   include "CnPartyFSMState_def.h" 
};

const wchar_t*  KCnPartyFSM::ms_szInput[] = { 
#   include "CnPartyFSMInput_def.h" 
};

KCnPartyFSM::KCnPartyFSM() : FSMclass( S_INIT )
{
    // S_INIT
    FSMstate* pState = new FSMstate( S_INIT, 1 );
    pState->AddTransition( I_TO_MAKING_PARTY,			S_MAKING_PARTY );
    AddState( pState );

    // S_CLOSE
    pState = new FSMstate( S_CLOSE, 1 );
    pState->AddTransition( I_TO_INIT,					S_INIT );
    AddState( pState );

	// S_MAKING_PARTY
	pState = new FSMstate( S_MAKING_PARTY, 2 );
	pState->AddTransition( I_TO_CLOSE,					S_CLOSE );
	pState->AddTransition( I_TO_WAIT,					S_WAIT );
	AddState( pState );

    // S_WAIT
    pState = new FSMstate( S_WAIT, 3 );
    pState->AddTransition( I_TO_CLOSE,					S_CLOSE );
	pState->AddTransition( I_TO_CHECK_PLAY_CONDITION,	S_CHECK_PLAY_CONDITION );
	pState->AddTransition( I_TO_CHECK_AUTO_PARTY,		S_CHECK_AUTO_PARTY );
    AddState( pState );

	// S_CHECK_PLAY_CONDITION
	pState = new FSMstate( S_CHECK_PLAY_CONDITION, 3 );
	pState->AddTransition( I_TO_CLOSE,					S_CLOSE );
	pState->AddTransition( I_TO_WAIT,					S_WAIT );
	pState->AddTransition( I_TO_ACCEPT_FOR_PLAY,		S_ACCEPT_FOR_PLAY );
	AddState( pState );

	// S_CHECK_AUTO_PARTY
	pState = new FSMstate( S_CHECK_AUTO_PARTY, 2 );
	pState->AddTransition( I_TO_CLOSE,					S_CLOSE );
	pState->AddTransition( I_TO_WAIT,					S_WAIT );
	AddState( pState );

    // S_ACCEPT_FOR_PLAY
    pState = new FSMstate( S_ACCEPT_FOR_PLAY, 3 );
    pState->AddTransition( I_TO_CLOSE,					S_CLOSE );
    pState->AddTransition( I_TO_PREPARE_FOR_PLAY,		S_PREPARE_FOR_PLAY );
	pState->AddTransition( I_TO_WAIT,					S_WAIT );
    AddState( pState );

    // S_PREPARE_FOR_PLAY
    pState = new FSMstate( S_PREPARE_FOR_PLAY, 2 );
	pState->AddTransition( I_TO_CLOSE,					S_CLOSE );
	pState->AddTransition( I_TO_WAIT,					S_WAIT );
    AddState( pState );
}

KCnPartyFSM::~KCnPartyFSM()
{
}


const wchar_t* KCnPartyFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KCnPartyFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}