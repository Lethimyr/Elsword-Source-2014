#pragma once

#ifndef EFFECT_TOOL

//#ifdef _IN_HOUSE_
//#define ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//#define ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
//#define ACTIVE_KOG_GAME_PERFORMANCE_TCPPINGTIME_CHECK//TCP Ping Time 관련 측정은 기본적으로 꺼두자.
//#endif // _IN_HOUSE_

#endif //EFFECT_TOOL

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#include <limits.h>

#include "../KOGGamePerformanceCheck/include/import_KOGGamePerformanceCheck.h"
#include "../KOGGamePerformanceCheck/include/KOGGamePerformanceCheck.h"
#include "../KOGGamePerformanceCheck/include/KOGGamePerformanceCheck_CallbackFuncTable.h"
#include "../KOGGamePerformanceCheck/include/KAvgFPSCheckManager.h"
#include "../KOGGamePerformanceCheck/include/KPingCheckManager.h"
#include "../KOGGamePerformanceCheck/include/KUDPTransCheckManager.h"
#include "../KOGGamePerformanceCheck/include/KNonHostNpcReactionCheckManager.h"
#include "../KOGGamePerformanceCheck/include/KMemUsageCheckManager.h"
#include "../KOGGamePerformanceCheck/include/KLagCheckManager.h"

#include "../X2Lib/KOGGamePerformanceCheckPort.h"

enum ELagCheckType
{
	eLagCheckType_No = 0,

	//알려지지 않은 랙
	eUnKnown_LagCheckType_OnIdle = 1,
	eUnKnown_LagCheckType_Simulation = 2,
	eUnKnown_LagCheckType_Rendering = 3,

	//알려진 랙
	eKnown_LagCheckType_PacketProcess = 4,
	eKnown_LagCheckType_LocalInputProcess = 5,
	eKnown_LagCheckType_GameUnit_FrameMove = 6,
	eKnown_LagCheckType_StateMenu_FrameMove = 7,
	eKnown_LagCheckType_KTDSManager_FrameMove = 8,
	eKnown_LagCheckType_KTDGFontManager_FrameMove = 9,
	eKnown_LagCheckType_UDPProcess_Host = 10,
	eKnown_LagCheckType_UDPProcess_Transformer = 11,
	
	//max
	eLagCheckType_Max,
};

#define KLagCheck( x ) KLagCheck kLagCheck( x )

#else//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#define KLagCheck( x )

#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

