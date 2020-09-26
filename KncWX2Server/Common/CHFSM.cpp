#include "CHFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KCHFSM::" L ## #id,
const wchar_t*  KCHFSM::ms_szState[] = { 
#   include "CHFSMState_def.h" 
};

const wchar_t*  KCHFSM::ms_szInput[] = { 
#   include "CHFSMInput_def.h" 
};

KCHFSM::KCHFSM() : FSMclass( KCHFSM::S_INIT )
{
	// S_INIT
	FSMstate* pState = new FSMstate( S_INIT, 2 );
	pState->AddTransition( I_TO_AUTHENTICATED,		S_LOGINED );
	pState->AddTransition( I_TO_EXIT,				S_EXIT );
	AddState( pState );

	// S_LOGINED
	pState = new FSMstate( S_LOGINED, 2 );
	//{{ 2012. 07. 02	최육사		대기열 시스템
//#ifdef SERV_WAIT_QUEUE_SYSTEM
	pState->AddTransition( I_TO_WAIT_QUEUE,			S_WAIT_QUEUE );
//#endif SERV_WAIT_QUEUE_SYSTEM
	//}}
	pState->AddTransition( I_TO_EXIT,				S_EXIT );
	AddState( pState );

	//{{ 2012. 07. 02	최육사		대기열 시스템
//#ifdef SERV_WAIT_QUEUE_SYSTEM
	// S_WAIT_QUEUE
	pState = new FSMstate( S_WAIT_QUEUE, 2 );
	pState->AddTransition( I_TO_AUTHENTICATED,		S_LOGINED );
	pState->AddTransition( I_TO_EXIT,				S_EXIT );
	AddState( pState );
//#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

	// STATE_EXIT
	pState = new FSMstate( S_EXIT, 0 );
	AddState( pState );
}

KCHFSM::~KCHFSM()
{
}


const wchar_t* KCHFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KCHFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}
