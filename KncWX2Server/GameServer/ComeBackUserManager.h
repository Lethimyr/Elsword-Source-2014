#pragma once
#include "Event.h"
#include "CommonPacket.h"


//{{ 2011. 05. 27    ±è¹Î¼º    ÈÞ¸é º¹±Í À¯Àú º¸»ó
#ifdef SERV_COME_BACK_USER_REWARD

class KComeBackUserManager
{
public:

	KComeBackUserManager(void);
	~KComeBackUserManager(void);

	void Clear();
	void SetIsComeBackUser( IN bool bIsComeBack )										{ m_bIsComeBackUser = bIsComeBack; }
	bool GetIsComeBackUser() const														{ return m_bIsComeBackUser; }
	void SetIsComeBackFirst( IN bool bIsFirst )											{ m_bIsComeBackFirst = bIsFirst; }
	bool GetIsComeBackFirst()															{ return m_bIsComeBackFirst; }
	void SetComeBackRewardLevel( IN int iComeBackRewardLevel )							{ m_cComeBackeRewardLevel = static_cast<char>(iComeBackRewardLevel); }
	char GetComeBackRewardLevel()														{ return m_cComeBackeRewardLevel; }
	void SetComeBackBuffEndTime( IN std::wstring& wstrComeBackBuffEndDate );
	bool GiveComeBackUserRewardCube( OUT int& iRewardItem, OUT CTime& tBuffEnd, OUT int& iRewardPeriod );
	bool CheckComeBackBuffEnd();
	CTime& GetComeBackBuffEnd()															{ return m_tComeBackBuffEnd; }
	bool GetIsUnitSelect()																{ return m_bIsUnitSelect; }
	void SetIsUnitSelect( IN bool bIsSelect )											{ m_bIsUnitSelect = bIsSelect; }

	
private:

	bool							m_bIsComeBackUser;
	bool							m_bIsComeBackFirst;
	char							m_cComeBackeRewardLevel;
	CTime							m_tComeBackBuffEnd;
	bool							m_bIsUnitSelect;
};


#endif SERV_COME_BACK_USER_REWARD
//}} 