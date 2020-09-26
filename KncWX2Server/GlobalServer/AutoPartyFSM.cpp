#include "AutoPartyFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KAutoPartyFSM::" L ## #id,
const wchar_t*  KAutoPartyFSM::ms_szState[] = { 
#   include "AutoPartyFSMState_def.h" 
};

const wchar_t*  KAutoPartyFSM::ms_szInput[] = { 
#   include "AutoPartyFSMInput_def.h" 
};

KAutoPartyFSM::KAutoPartyFSM() : FSMclass( S_INIT )
{
    // S_INIT
    FSMstate* pState = new FSMstate( S_INIT, 1 );
    pState->AddTransition( I_TO_AUTO_PARTY_MAKING,	S_AUTO_PARTY_MAKING );
    AddState( pState );

    // S_CLOSE
    pState = new FSMstate( S_CLOSE, 1 );
    pState->AddTransition( I_TO_INIT,				S_INIT );
    AddState( pState );

    // S_AUTO_PARTY_MAKING
    pState = new FSMstate( S_AUTO_PARTY_MAKING, 2 );
    pState->AddTransition( I_TO_CLOSE,				S_CLOSE );
    pState->AddTransition( I_TO_CHECK_ACCEPT,		S_CHECK_ACCEPT );
    AddState( pState );

    // S_CHECK_ACCEPT
    pState = new FSMstate( S_CHECK_ACCEPT, 2 );
    pState->AddTransition( I_TO_CLOSE,				S_CLOSE );
    pState->AddTransition( I_TO_REGROUP_PARTY,		S_REGROUP_PARTY );
    AddState( pState );

    // S_REGROUP_PARTY
    pState = new FSMstate( S_REGROUP_PARTY, 2 );
	pState->AddTransition( I_TO_CLOSE,				S_CLOSE );
	pState->AddTransition( I_TO_PLAY,				S_PLAY );
    AddState( pState );

	// S_PLAY
	pState = new FSMstate( S_PLAY, 1 );
	pState->AddTransition( I_TO_CLOSE,				S_CLOSE );
	AddState( pState );
}

KAutoPartyFSM::~KAutoPartyFSM()
{
}


const wchar_t* KAutoPartyFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KAutoPartyFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}