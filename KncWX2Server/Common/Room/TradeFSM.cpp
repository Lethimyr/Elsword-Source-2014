#include ".\TradeFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KTradeFSM::" L ## #id,
const wchar_t*  KTradeFSM::ms_szState[] = 
{ 
#   include "TradeFSMState_def.h" 
};

const wchar_t*  KTradeFSM::ms_szInput[] = 
{ 
#   include "TradeFSMInput_def.h" 
};

KTradeFSM::KTradeFSM() : FSMclass( S_INIT )
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
	pState = new FSMstate( S_WAIT, 2 );
	pState->AddTransition( I_TO_CLOSE,          S_CLOSE );
	pState->AddTransition( I_TO_CONFIRM,		S_CONFIRM );
	AddState( pState );

	// S_CONFIRM
	pState = new FSMstate( S_CONFIRM, 2 );
	pState->AddTransition( I_TO_CLOSE,			S_CLOSE );
	pState->AddTransition( I_TO_DB_UPDATE,		S_DB_UPDATE );
	AddState( pState );

	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
//#ifdef SERV_TRADE_DDOS_DEFENCE
	// S_DB_UPDATE
	pState = new FSMstate( S_DB_UPDATE, 1 );
	pState->AddTransition( I_TO_CLOSE,			S_CLOSE );
	AddState( pState );
//#endif SERV_TRADE_DDOS_DEFENCE
	//}}
}

KTradeFSM::~KTradeFSM()
{
}


const wchar_t* KTradeFSM::GetStateIDString(int iStateID_) const
{
	return ms_szState[iStateID_];
}


const wchar_t* KTradeFSM::GetInputIDString(int iInputID_) const
{
	return ms_szInput[iInputID_];
}