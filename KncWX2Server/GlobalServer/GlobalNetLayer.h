#pragma once
#include "NetLayer.h"

class KGlobalNetLayer : public KNetLayer
{
    DeclToStringW;
    DeclDump;
    NiDeclareRTTI;

public:
    KGlobalNetLayer(void);
    virtual ~KGlobalNetLayer(void);

    // derived from KNetLayer
    virtual void RegToLua();

protected:

public: // lua access
};