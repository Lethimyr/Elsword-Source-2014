#pragma once

#include "FSM/FSMclass.h"

class KMatchUserFSM : public FSMclass
{
public:
#undef _ENUM
#define _ENUM( id ) id,
    enum ENUM_STATE
    {
#       include "MatchUserFSMState_def.h"
    };
    enum ENUM_INPUT
    {
#       include "MatchUserFSMInput_def.h"
    };

    KMatchUserFSM();
    virtual ~KMatchUserFSM();

    virtual const wchar_t* GetStateIDString(int iStateID) const;
    virtual const wchar_t* GetInputIDString(int iInputID) const;

protected:    
    static const wchar_t*  ms_szState[];
    static const wchar_t*  ms_szInput[];
};