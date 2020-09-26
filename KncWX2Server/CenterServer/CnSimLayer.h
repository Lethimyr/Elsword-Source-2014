#pragma once
#include "SimLayer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class CXSLDungeonManager;

class KCnSimLayer : public KSimLayer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;

public:
    KCnSimLayer(void);
    virtual ~KCnSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua();

	//{{ 임홍락 [2012.05.16] 서버 단일화 // SERV_USE_NATION_FLAG 필수 포함임
#ifdef SERV_UNITED_SERVER_EU
	std::string GetStrItemLuaName();
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	std::string GetStrItemTransLuaName();
#endif // SERV_ITEM_LUA_TRANS_DEVIDE
	std::string GetStrPvpNpcDataLua();
#endif SERV_UNITED_SERVER_EU
	//}}
	//////////////////////////////////////////////////////////////////////////
	void DisconnectAllCnUser_LUA();
	//////////////////////////////////////////////////////////////////////////

protected:
	//{{ 2009. 12. 15  최육사	서버관리
	void DumpToLogFile();
	//}}
};

DefKObjectInline( KCnSimLayer, KSimLayer );