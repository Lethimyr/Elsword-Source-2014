#pragma once

#include "GameEvent.h"
#include "Lottery.h"

class KGameEventMonster : public KGameEvent
{
public:
	KGameEventMonster();
	virtual ~KGameEventMonster();

	void	SetEventID( int iEventID )				{ m_iEventID = iEventID; }
	int		GetEventID()							{ return m_iEventID; }

private:
	int		m_iEventID;
};