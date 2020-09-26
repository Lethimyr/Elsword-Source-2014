#include "GameEventDefenseDungeon.h"

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
KGameEventDefenseDungeon::KGameEventDefenseDungeon()
: m_fDefenseDungeonOpenRate( 1.0f )
{
}

KGameEventDefenseDungeon::~KGameEventDefenseDungeon()
{
}
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT