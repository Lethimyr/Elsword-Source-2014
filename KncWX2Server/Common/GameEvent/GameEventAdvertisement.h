#pragma once

#include "GameEvent.h"
#include "ServerDefine.h"

#ifdef SERV_ADVERTISEMENT_EVENT
class KGameEventAdvertisement : public KGameEvent
{
public:
	KGameEventAdvertisement(void);
	virtual ~KGameEventAdvertisement(void);
	
	void	SetEventID( int iEventID )				{ m_iEventID = iEventID; }
	int		GetEventID()							{ return m_iEventID; }
	
	void	SetRewardID( int iRewardID )			{ m_iRewardID = iRewardID; }
	int		GetRewardID()							{ return m_iRewardID; }

	void	SetAdvertisementURL( std::wstring wstrAdvertisementURL ) { m_wstrAdvertisementURL = wstrAdvertisementURL; }
	const std::wstring& GetAdvertisementURL() const { return m_wstrAdvertisementURL; }

private:
	int				m_iEventID;
	int				m_iRewardID;
	std::wstring	m_wstrAdvertisementURL;
};
#endif SERV_ADVERTISEMENT_EVENT