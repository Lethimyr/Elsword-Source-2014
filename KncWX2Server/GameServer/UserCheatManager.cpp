#include "GSUser.h"
#include ".\UserCheatManager.h"


//{{ 2012. 12. 5	최육사		옵저버 대전 난입 모드
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE


KUserCheatManager::KUserCheatManager(void) :
m_bObserverMode( false )
{
}

KUserCheatManager::~KUserCheatManager(void)
{
}


#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}


