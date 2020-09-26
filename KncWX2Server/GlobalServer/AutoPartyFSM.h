#pragma     once

#include "FSM/FSMclass.h"

class KAutoPartyFSM : public FSMclass
{
public:
    #undef _ENUM
    #define _ENUM( id ) id,
    enum ENUM_STATE
    {
#       include "AutoPartyFSMState_def.h"
    };
    enum ENUM_INPUT
    {
#       include "AutoPartyFSMInput_def.h"
    };

    KAutoPartyFSM();
    virtual ~KAutoPartyFSM();

    virtual const wchar_t* GetStateIDString(int iStateID) const;
    virtual const wchar_t* GetInputIDString(int iInputID) const;

protected:    
    static const wchar_t*  ms_szState[];
    static const wchar_t*  ms_szInput[];
};