#include "RoomFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KRoomFSM::" L ## #id,
const wchar_t*  KRoomFSM::ms_szState[] = { 
#   include "RoomFSMState_def.h" 
};

const wchar_t*  KRoomFSM::ms_szInput[] = { 
#   include "RoomFSMInput_def.h" 
};

KRoomFSM::KRoomFSM() : FSMclass( S_INIT )
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
    pState = new FSMstate( S_WAIT, 5 );
    pState->AddTransition( I_TO_CLOSE,          S_CLOSE );
    pState->AddTransition( I_TO_LOAD,           S_LOAD );
	pState->AddTransition( I_TO_TIME_COUNT,		S_TIME_COUNT );
	pState->AddTransition( I_TO_RETURN_TO_FIELD,S_RETURN_TO_FIELD );
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
	//#ifdef SERV_NEW_DEFENCE_DUNGEON
	pState->AddTransition( I_TO_WAIT_FOR_DEFENCE, S_WAIT_FOR_DEFENCE );
	//#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
    AddState( pState );

	//{{ 2012. 05. 23	최육사	배틀필드 시스템
	//#ifdef SERV_BATTLE_FIELD_SYSTEM
	// S_TIME_COUNT
	pState = new FSMstate( S_TIME_COUNT, 2 );
	pState->AddTransition( I_TO_CLOSE,          S_CLOSE );
	pState->AddTransition( I_TO_LOAD,           S_LOAD );
	AddState( pState );
	//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

    // S_LOAD
    pState = new FSMstate( S_LOAD, 2 );
    pState->AddTransition( I_TO_CLOSE,          S_CLOSE );    
    pState->AddTransition( I_TO_PLAY,           S_PLAY );
    AddState( pState );

    // S_PLAY
    pState = new FSMstate( S_PLAY, 2 );
	pState->AddTransition( I_TO_CLOSE,			S_CLOSE );
	pState->AddTransition( I_TO_RESULT,         S_RESULT );
    AddState( pState );

	// S_RESULT
	pState = new FSMstate( S_RESULT, 3 );
	pState->AddTransition( I_TO_CLOSE,          S_CLOSE );
	pState->AddTransition( I_TO_WAIT,           S_WAIT );
	AddState( pState );

	//{{ 2012. 06. 07	최육사	배틀필드 시스템
	//#ifdef SERV_BATTLE_FIELD_SYSTEM
	// S_RETURN_TO_FIELD
	pState = new FSMstate( S_RETURN_TO_FIELD, 2 );
	pState->AddTransition( I_TO_CLOSE,          S_CLOSE );
	pState->AddTransition( I_TO_WAIT,           S_WAIT );
	AddState( pState );
	//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2013. 04. 15	최육사	어둠의 문 개편
	//#ifdef SERV_NEW_DEFENCE_DUNGEON
	pState = new FSMstate( S_WAIT_FOR_DEFENCE, 2 );
	pState->AddTransition( I_TO_CLOSE,          S_CLOSE );
	pState->AddTransition( I_TO_WAIT,           S_WAIT );
	AddState( pState );
	//#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
}

KRoomFSM::~KRoomFSM()
{
}


const wchar_t* KRoomFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KRoomFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}