#pragma once

#include <atltime.h>
#include <boost/timer.hpp>

#include "KncUidType.h"
#include "ServerDefine.h"

#ifdef SERV_ANTI_ADDICTION_SYSTEM

class AntiAddictionManager
{
public:
	enum AA_TIME_TYPE			{ AATT_PLAY_TIME, AATT_REST_TIME };
	enum AA_RESTRICTION_TYPE	{ AART_NONE, AART_HALF, AART_FULL };

	struct AntiAddictionInfo
	{
		CTime			m_tLastLogin;
		CTime			m_tLastLogout;
		int				m_iPlayTime;
		int				m_iRestTime;

		AntiAddictionInfo()
		: m_tLastLogin(2000, 1, 1, 0, 0, 0)
		, m_tLastLogout(2000, 1, 1, 0, 0, 0)
		, m_iPlayTime(0)
		, m_iRestTime(0)
		{
		}
	};

	AntiAddictionManager();
	virtual ~AntiAddictionManager();

	void	Init(UidType OwnerUID, const AntiAddictionInfo& info);
	void	Finalize();

	void	Tick();
	void	TickTypeCheck();
	void	TickNotifyCheck();

	float	GetRewardRatio() const;
	void	SetEnable(bool bEnable)								{ m_bEnable = bEnable; }
	bool	IsEnable() const;

private:
	void	UpdateTime(int iPlayTime, int iRestime);

	void	Notify();

private:
	UidType				m_OwnerUID;
	AntiAddictionInfo	m_Info;
	bool				m_bEnable;

	boost::timer		m_RefreshTimer;

	AA_RESTRICTION_TYPE	m_RestrictionType;
	int					m_iCurrentPlayTime;
	boost::timer		m_NotifyTimer;
};

#endif SERV_ANTI_ADDICTION_SYSTEM