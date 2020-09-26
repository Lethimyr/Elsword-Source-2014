#pragma once
#include "NetLayer.h"

class KChannelNetLayer : public KNetLayer
{
    DeclToStringW;
    DeclDump;
    NiDeclareRTTI;

public:
    KChannelNetLayer(void);
    virtual ~KChannelNetLayer(void);

    // derived from KNetLayer
    virtual void RegToLua();

protected:

public: // lua access
};