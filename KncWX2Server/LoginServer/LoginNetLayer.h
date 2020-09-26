#pragma once
#include "NetLayer.h"

class KLoginNetLayer : public KNetLayer
{
    DeclToStringW;
    DeclDump;
    NiDeclareRTTI;

public:
    KLoginNetLayer(void);
    virtual ~KLoginNetLayer(void);

    // derived from KNetLayer
    virtual void RegToLua();

protected:

public: // lua access
};