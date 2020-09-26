#pragma once

#include <map>

#include "Event.h"
#include "ServerPacket.h"

#ifdef SERV_NEW_PUNISHMENT

class KPunishmentManager
{
public:
	KPunishmentManager();
	virtual ~KPunishmentManager();

	void	SetPunishment(const KPunishmentInfo& info);
	bool	IsAppliedPunishment(int iType);

protected:
	std::map<int,KPunishmentInfo>			m_mapPunishment;
};

#endif SERV_NEW_PUNISHMENT