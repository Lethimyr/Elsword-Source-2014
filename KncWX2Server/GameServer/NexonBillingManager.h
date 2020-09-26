#pragma once

#include "KncSingleton.h"
#include "ThreadManager.h"

class KNexonBillingManager : public KThreadManager
{
    DeclDump;
    DeclToStringW;
    DeclPfID;
    DeclareSingleton( KNexonBillingManager );

public:
    KNexonBillingManager();
    virtual ~KNexonBillingManager();

    void RegToLua();
    void AddWebServiceAddress( int iSoapAction, const char* szWSDL, const char* szMethod );

    // derived from KThreadManager
    virtual KThread* CreateThread();

protected:
};

DefSingletonInline( KNexonBillingManager );