#pragma once
#include "Event.h"
#include <boost/timer.hpp>
#include <KncContainer.h>


class KCnPartyFeverManager
{
public:
	enum FEVER
	{
		//FEVER_RANDOM_FACTOR		= 60,
		FEVER_DECREASE_POINT_BY_LEAVE_PARTY	= 800,
		FEVER_100_PERCENT_POINT				= 4000,
	};

	enum UPDATE_FEVER_POINT
	{
		UFP_NONE = 0,
		UFP_ACTIVE_FEVER,
		UFP_CHANGE_FEVER,
	};

public:
	KCnPartyFeverManager();
	~KCnPartyFeverManager();

	void	Reset();

	short	GetRealFeverPoint() const							{ return m_sFeverPoint; }
	void	SetRealFeverPoint( IN const short sFeverPoint )		{ m_sFeverPoint = sFeverPoint; }

	short	GetFeverPoint( IN const int iNumMember ) const;

	int	CalcIncreaseFeverPoint( IN const int iNumMember, IN const char cRoomType, IN const short sSubStageNpcCount, IN const bool bComeBackUserInParty );
	UPDATE_FEVER_POINT UpdateFeverPoint( IN const int iFeverPoint, IN const int iNumMember, OUT int& iResultFeverPoint );

private:
	short							m_sFeverPoint;
};



