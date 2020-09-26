#pragma once

#include "FSM/FSMclass.h"

class KRBFSM : public FSMclass
{
public:
#undef _ENUM
#define _ENUM( id ) id,
	enum ENUM_STATE
	{
#       include "RBFSMState_def.h"
	};
	enum ENUM_INPUT
	{
#       include "RBFSMInput_def.h"
	};

	KRBFSM(void);
	virtual ~KRBFSM(void);

	virtual const wchar_t* GetStateIDString(int iStateID) const;
	virtual const wchar_t* GetInputIDString(int iInputID) const;

protected:    
	static const wchar_t*  ms_szState[];
	static const wchar_t*  ms_szInput[];
};
