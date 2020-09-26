#pragma once

#include "GameEvent.h"

class KGameEventQuest : public KGameEvent
{
public:
	KGameEventQuest(void);
	virtual ~KGameEventQuest(void);

	void	SetQuestID( int iQuestID )			{ m_iQuestID = iQuestID; }
	int		GetQuestID()						{ return m_iQuestID; }

private:
	int		m_iQuestID;
};
