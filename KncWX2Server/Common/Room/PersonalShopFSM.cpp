#include ".\PersonalShopFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KPersonalShopFSM::" L ## #id,
const wchar_t*  KPersonalShopFSM::ms_szState[] = 
{ 
#   include "PersonalShopFSMState_def.h" 
};

const wchar_t*  KPersonalShopFSM::ms_szInput[] = 
{ 
#   include "PersonalShopFSMInput_def.h" 
};

KPersonalShopFSM::KPersonalShopFSM() : FSMclass( S_INIT )
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
	pState->AddTransition( I_TO_SHOP,			S_SHOP );
	AddState( pState );

	// S_SHOP
	pState = new FSMstate( S_SHOP, 2 );
	pState->AddTransition( I_TO_STOP_SALE,		S_STOP_SALE );
	pState->AddTransition( I_TO_CLOSE,			S_CLOSE );
	AddState( pState );

	//{{ 2011. 04. 12	최육사	대리 상인
//#ifdef SERV_PSHOP_AGENCY
	// S_STOP_SALE
	pState = new FSMstate( S_STOP_SALE, 1 );
	pState->AddTransition( I_TO_CLOSE,			S_CLOSE );
	AddState( pState );
//#endif SERV_PSHOP_AGENCY
	//}}
}

KPersonalShopFSM::~KPersonalShopFSM()
{
}


const wchar_t* KPersonalShopFSM::GetStateIDString(int iStateID_) const
{
	return ms_szState[iStateID_];
}


const wchar_t* KPersonalShopFSM::GetInputIDString(int iInputID_) const
{
	return ms_szInput[iInputID_];
}