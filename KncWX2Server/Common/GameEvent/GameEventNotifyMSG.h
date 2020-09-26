#pragma once

#include "GameEvent.h"

class KGameEventNotifyMSG : public KGameEvent
{
public:
	KGameEventNotifyMSG();
	virtual ~KGameEventNotifyMSG();

	const std::wstring& GetNotifyMSG() const { return GetEventName(); }

	void	SetIsSend( bool bIsSend ){ m_bIsSend = bIsSend; }
	bool	IsSend() const			 { return m_bIsSend; }

	//{{ 2010. 05. 12  최육사	대전 던전 서버군 통합
	void	SetIsAllServers( bool bVal ) { m_bIsAllServers = bVal; }
	bool	IsAllServers() const		 { return m_bIsAllServers; }
	//}}

protected:
	bool	m_bIsSend;
	//{{ 2010. 05. 12  최육사	대전 던전 서버군 통합
	bool	m_bIsAllServers;
	//}}
};