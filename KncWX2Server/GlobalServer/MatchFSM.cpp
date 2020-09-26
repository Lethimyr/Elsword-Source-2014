#include "MatchFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KMatchFSM::" L ## #id,
const wchar_t*  KMatchFSM::ms_szState[] = { 
#   include "MatchFSMState_def.h" 
};

const wchar_t*  KMatchFSM::ms_szInput[] = { 
#   include "MatchFSMInput_def.h" 
};

KMatchFSM::KMatchFSM() : FSMclass( KMatchFSM::S_INIT )
{
    // S_INIT
    FSMstate* pState = new FSMstate( S_INIT, 1 );
    pState->AddTransition( I_TO_MATCH_MAKING,		S_MATCH_MAKING );
    AddState( pState );

    // S_CLOSE
    pState = new FSMstate( S_CLOSE, 1 );
    pState->AddTransition( I_TO_INIT,				S_INIT );
    AddState( pState );

    // S_MATCH_MAKING
    pState = new FSMstate( S_MATCH_MAKING, 2 );
    pState->AddTransition( I_TO_CLOSE,				S_CLOSE );
    pState->AddTransition( I_TO_CHECK_ACCEPT,		S_CHECK_ACCEPT );
    AddState( pState );

    // S_CHECK_ACCEPT
    pState = new FSMstate( S_CHECK_ACCEPT, 3 );
    pState->AddTransition( I_TO_CLOSE,				S_CLOSE );
	pState->AddTransition( I_TO_MATCH_MAKING,		S_MATCH_MAKING );
    pState->AddTransition( I_TO_PREPARE_FOR_PLAY,	S_PREPARE_FOR_PLAY );
    AddState( pState );

    // S_PREPARE_FOR_PLAY
    pState = new FSMstate( S_PREPARE_FOR_PLAY, 2 );
	pState->AddTransition( I_TO_CLOSE,				S_CLOSE );
	pState->AddTransition( I_TO_PLAY,				S_PLAY );
    AddState( pState );

	// S_PLAY
	pState = new FSMstate( S_PLAY, 2 );
	pState->AddTransition( I_TO_CLOSE,				S_CLOSE );
	pState->AddTransition( I_TO_MATCH_MAKING,		S_MATCH_MAKING );
	AddState( pState );
}

KMatchFSM::~KMatchFSM()
{
}


const wchar_t* KMatchFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KMatchFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}