#include "GameEventNotifyMSG.h"

KGameEventNotifyMSG::KGameEventNotifyMSG()
:
m_bIsSend( false ),
//{{ 2010. 05. 12  최육사	대전 던전 서버군 통합
m_bIsAllServers( false )
//}}
{
}

KGameEventNotifyMSG::~KGameEventNotifyMSG()
{
}