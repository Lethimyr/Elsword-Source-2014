#pragma once

#include "FSM/FSMclass.h"

class KAutoPartyUserFSM : public FSMclass
{
public:
#undef _ENUM
#define _ENUM( id ) id,
    enum ENUM_STATE
    {
#       include "AutoPartyUserFSMState_def.h"
    };
    enum ENUM_INPUT
    {
#       include "AutoPartyUserFSMInput_def.h"
    };

    KAutoPartyUserFSM();
    virtual ~KAutoPartyUserFSM();

    virtual const wchar_t* GetStateIDString(int iStateID) const;
    virtual const wchar_t* GetInputIDString(int iInputID) const;

protected:    
    static const wchar_t*  ms_szState[];
    static const wchar_t*  ms_szInput[];
};