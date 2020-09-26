#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include <boost/timer.hpp>


//{{ ¹öÇÁ °ü¸® ½Ã½ºÅÛ - ±è¹Î¼º
#ifdef SERV_SERVER_BUFF_SYSTEM


#define KUserServerBuffPtr KUserServerBuff*
#undef KUserServerBuffPtr
SmartPointer( KUserServerBuff );


class KUserServerBuff
{
public:
	KUserServerBuff( IN const KBuffInfo& kBuffInfo );
	~KUserServerBuff(void);

	void	InitPeriod( IN const CTime& tStartTime );
	int		GetBuffID() const						{ return m_iBuffID; }
	const CTime& GetStartTime() const				{ return m_tStartTime; }
	const std::wstring& GetStartTimeStr() const		{ return m_wstrStartTime; }
	const CTime& GetEndTime() const					{ return m_tEndTime; }
	const std::wstring& GetEndTimeStr() const		{ return m_wstrEndTime; }
	void	GetBuffInfo( OUT KBuffInfo& kBuffInfo ) const;
	bool	IsPeriodBuff() const					{ return ( m_fBuffPeriodTime != 0.0 ); }
	bool	IsExpired() const;
	
private:
	int							m_iBuffID;
	CTime						m_tStartTime;
	std::wstring				m_wstrStartTime;
	CTime						m_tEndTime;
	std::wstring				m_wstrEndTime;
	double						m_fBuffPeriodTime;
	int							m_iFactorID;
};


#endif SERV_SERVER_BUFF_SYSTEM
//}}