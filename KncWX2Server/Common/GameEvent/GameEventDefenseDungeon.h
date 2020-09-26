#pragma once

#include "GameEvent.h"

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
class KGameEventDefenseDungeon : public KGameEvent
{
public:
	KGameEventDefenseDungeon();
	virtual ~KGameEventDefenseDungeon();

	void	SetDefenseDungeonOpenRate( const float fDefenseDungeonOpenRate )	{ m_fDefenseDungeonOpenRate = fDefenseDungeonOpenRate; }
	float	GetDefenseDungeonOpenRate( void ) const{	return m_fDefenseDungeonOpenRate;	}

private:
	float	m_fDefenseDungeonOpenRate;
};
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT