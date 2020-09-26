#pragma once
#include "ServerDefine.h"
#include "KncUtil.h"

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KNCSingleton.h>
#include <ToString.h>
#include <boost/timer.hpp>
#include <map>

class KReleaseTickManager
{
	DeclareSingleton( KReleaseTickManager );
	DeclDump;
	DeclToStringW;

public:
	KReleaseTickManager();
	~KReleaseTickManager();

	// function
	void	Init( void );
	void	Tick( void );
	void	SendReleaseTickListReq( void );
	
	// DB에서 이벤트 패킷이 올경우..
	void	SetReleaseTickData( IN const std::map< int, int >& mapReleaseTick );

private:
	std::map< int, int >	m_mapReleaseTick;
	boost::timer			m_TimerRefresh;		// 일정 시간마다 갱신
};

DefSingletonInline( KReleaseTickManager );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
