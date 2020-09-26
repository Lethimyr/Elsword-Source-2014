#include ".\rbfsm.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KRBFSM::" L ## #id,
const wchar_t*  KRBFSM::ms_szState[] = { 
#   include "RBFSMState_def.h" 
};

const wchar_t*  KRBFSM::ms_szInput[] = { 
#   include "RBFSMInput_def.h" 
};

KRBFSM::KRBFSM() : FSMclass( KRBFSM::ENUM_STATE::S_INIT )
{
	// S_CHANNEL_SELECT
	FSMstate* pState = new FSMstate( S_CHANNEL_SELECT, 2 );
	pState->AddTransition( I_TO_INIT,		    S_INIT );
	pState->AddTransition( I_TO_EXIT,           S_EXIT );
	AddState( pState );

	// S_INIT
	pState = new FSMstate( S_INIT, 2 );
	pState->AddTransition( I_TO_UID_UPDATED,    S_UID_UPDATED );
	pState->AddTransition( I_TO_EXIT,           S_EXIT );
	AddState( pState );

	// S_UID_UPDATED
	pState = new FSMstate( S_UID_UPDATED, 2 );
	pState->AddTransition( I_TO_LOGINED,        S_LOGINED );
	pState->AddTransition( I_TO_EXIT,           S_EXIT );
	AddState( pState );

	// S_LOGINED
	pState = new FSMstate( S_LOGINED, 2 );
	pState->AddTransition( I_TO_SERVER_SELECT,  S_SERVER_SELECT );
	pState->AddTransition( I_TO_EXIT,           S_EXIT );
	AddState( pState );

	// S_SERVER_SELECT
	pState = new FSMstate( S_SERVER_SELECT, 2 );
	pState->AddTransition( I_TO_UNIT_SELECT,	S_UNIT_SELECT );
	pState->AddTransition( I_TO_EXIT,           S_EXIT );
	AddState( pState );

	// S_UNIT_SELECT
	pState = new FSMstate( S_UNIT_SELECT, 2 );
	pState->AddTransition( I_TO_FIELD_MAP,		S_FIELD_MAP );
	pState->AddTransition( I_TO_EXIT,           S_EXIT );
	AddState( pState );

	// S_FIELD_MAP
	pState = new FSMstate( S_FIELD_MAP, 5 );
	pState->AddTransition( I_TO_UNIT_SELECT,	S_UNIT_SELECT );
	pState->AddTransition( I_TO_PVP_LOBBY,		S_PVP_LOBBY );
	pState->AddTransition( I_TO_PARTY,			S_PARTY );
	pState->AddTransition( I_TO_TRAINING_CENTER,S_TRAINING_CENTER );
	pState->AddTransition( I_TO_EXIT,			S_EXIT );
	AddState( pState );

	// S_PVP_LOBBY
	pState = new FSMstate( S_PVP_LOBBY, 2 );
	pState->AddTransition( I_TO_FIELD_MAP,		S_FIELD_MAP );
	pState->AddTransition( I_TO_ROOM,			S_ROOM );
	AddState( pState );

	// S_PARTY
	pState = new FSMstate( S_PARTY, 2 );
	pState->AddTransition( I_TO_FIELD_MAP,		S_FIELD_MAP );
	pState->AddTransition( I_TO_ROOM,			S_ROOM );
	AddState( pState );
	
	// S_TRAINING_CENTER
	pState = new FSMstate( S_TRAINING_CENTER, 2 );
	pState->AddTransition( I_TO_FIELD_MAP,		S_FIELD_MAP );
	pState->AddTransition( I_TO_ROOM,			S_ROOM );
	AddState( pState );

	// S_ROOM
	pState = new FSMstate( S_ROOM, 3 );
	pState->AddTransition( I_TO_PVP_LOBBY,		S_PVP_LOBBY );
	pState->AddTransition( I_TO_PARTY,			S_PARTY );
	pState->AddTransition( I_TO_TRAINING_CENTER,S_TRAINING_CENTER );	
	AddState( pState );

	// S_EXIT
	pState = new FSMstate( S_EXIT, 2 );
	pState->AddTransition( I_TO_CHANNEL_SELECT,	S_CHANNEL_SELECT );	// 로봇에서만 사용.
	pState->AddTransition( I_TO_INIT,			S_INIT );			// 로봇에서만 사용.
	AddState( pState );
}

KRBFSM::~KRBFSM(void)
{
}

const wchar_t* KRBFSM::GetStateIDString(int iStateID_) const
{
	return ms_szState[iStateID_];
}


const wchar_t* KRBFSM::GetInputIDString(int iInputID_) const
{
	return ms_szInput[iInputID_];
}


