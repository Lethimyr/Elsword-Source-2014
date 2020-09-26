#include "GSFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KGSFSM::" L ## #id,
const wchar_t*  KGSFSM::ms_szState[] = { 
#   include "GSFSMState_def.h" 
};

const wchar_t*  KGSFSM::ms_szInput[] = { 
#   include "GSFSMInput_def.h" 
};

KGSFSM::KGSFSM() : FSMclass( S_INIT )
{
    // S_INIT
    FSMstate* pState = new FSMstate( S_INIT, 2 );
    pState->AddTransition( I_TO_UID_UPDATED,		S_UID_UPDATED );
    pState->AddTransition( I_TO_EXIT,				S_EXIT );
    AddState( pState );

    // S_UID_UPDATED
    pState = new FSMstate( S_UID_UPDATED, 2 );
    pState->AddTransition( I_TO_LOGINED,			S_LOGINED );
    pState->AddTransition( I_TO_EXIT,				S_EXIT );
    AddState( pState );

    // S_LOGINED
    pState = new FSMstate( S_LOGINED, 2 );
    pState->AddTransition( I_TO_SERVER_SELECT,		S_SERVER_SELECT );
    pState->AddTransition( I_TO_EXIT,				S_EXIT );
    AddState( pState );

	//{{ 2011. 06. 23	최육사	2차 비번 예외처리
//#ifdef SERV_CHECK_SECOND_PW_STATE
    // S_SERVER_SELECT
    pState = new FSMstate( S_SERVER_SELECT, 3 );
	pState->AddTransition( I_TO_FIELD_MAP,			S_FIELD_MAP );
	pState->AddTransition( I_TO_CHECK_SECOND_PW,	S_CHECK_SECOND_PW );
    pState->AddTransition( I_TO_EXIT,				S_EXIT );
    AddState( pState );
//#endif SERV_CHECK_SECOND_PW_STATE
	//}}

	//{{ 2011. 06. 23	최육사	2차 비번 예외처리
//#ifdef SERV_CHECK_SECOND_PW_STATE
	// S_CHECK_SECOND_PW
	pState = new FSMstate( S_CHECK_SECOND_PW, 2 );
	pState->AddTransition( I_TO_SERVER_SELECT,		S_SERVER_SELECT );
	pState->AddTransition( I_TO_EXIT,				S_EXIT );
	AddState( pState );
//#endif SERV_CHECK_SECOND_PW_STATE
	//}}

	// S_FIELD_MAP
	pState = new FSMstate( S_FIELD_MAP, 3 );	
	pState->AddTransition( I_TO_SERVER_SELECT,		S_SERVER_SELECT );
	pState->AddTransition( I_TO_ROOM,				S_ROOM );
	pState->AddTransition( I_TO_EXIT,				S_EXIT );
	AddState( pState );

	// S_ROOM
	pState = new FSMstate( S_ROOM, 2 );
	pState->AddTransition( I_TO_FIELD_MAP,			S_FIELD_MAP );
	pState->AddTransition( I_TO_EXIT,				S_EXIT );
	AddState( pState );

    // S_EXIT
    pState = new FSMstate( S_EXIT, 1 );
    pState->AddTransition( I_TO_INIT,				S_INIT );   // 로봇에서만 사용.
    AddState( pState );

}

KGSFSM::~KGSFSM()
{
}


const wchar_t* KGSFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KGSFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}
