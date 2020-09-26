#pragma once
#include "NetLayer.h"

class KGSNetLayer : public KNetLayer
{
    DeclToStringW;
    DeclDump;
    NiDeclareRTTI;

public:
    KGSNetLayer(void);
    virtual ~KGSNetLayer(void);

    // derived from KNetLayer
    //virtual bool Init();
    //virtual void ShutDown();
    virtual void RegToLua();

public:
};