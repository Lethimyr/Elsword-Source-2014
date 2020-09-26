#include "DefaultFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KCenterFSM::" L ## #id,
const wchar_t*  KDefaultFSM::ms_szState[] = { 
#   include "DefaultFSMState_def.h" 
};

const wchar_t*  KDefaultFSM::ms_szInput[] = { 
#   include "DefaultFSMInput_def.h" 
};

KDefaultFSM::KDefaultFSM() : FSMclass( KDefaultFSM::S_INIT )
{
    // S_INIT
    FSMstate* pState = new FSMstate( S_INIT, 2 );
    pState->AddTransition( I_AUTHENTICATED,     S_LOGINED );
    pState->AddTransition( I_EXIT,              S_EXIT );
    AddState( pState );

    // S_LOGINED
    pState = new FSMstate( S_LOGINED, 1 );
    pState->AddTransition( I_EXIT,              S_EXIT );
    AddState( pState );

    // STATE_EXIT
    pState = new FSMstate( S_EXIT, 0 );
    AddState( pState );

}

KDefaultFSM::~KDefaultFSM()
{
}


const wchar_t* KDefaultFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KDefaultFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}