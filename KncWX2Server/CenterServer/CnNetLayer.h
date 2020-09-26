#pragma once
#include "NetLayer.h"

class KCnNetLayer : public KNetLayer
{
    DeclToStringW;
    DeclDump;
    NiDeclareRTTI;

public:
    KCnNetLayer(void);
    virtual ~KCnNetLayer(void);

    // derived from KNetLayer
    //virtual bool Init();
    //virtual void ShutDown();
    virtual void RegToLua();

	//{{ 2009. 12. 15  최육사	서버관리
	virtual void DumpToLogFile();
	//}}

protected:

public: // lua access
};