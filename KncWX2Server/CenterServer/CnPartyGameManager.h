#pragma once
#include "Event.h"
#include <boost/timer.hpp>
#include <KncContainer.h>


class KCnPartyGameManager
{
public:
	KCnPartyGameManager();
	~KCnPartyGameManager();

	void	Reset();
	void	SetGameStartInfo( IN const u_short usGameStartEventID, IN const short sGameStartWorldID );
	u_short GetGameStartEventID() const { return m_usGameStartEventID; }
	short	GetGameStartWorldID() const { return m_sGameStartWorldID; }

private:
	u_short			m_usGameStartEventID;
	short			m_sGameStartWorldID;
};



