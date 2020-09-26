#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>
#include <boost/timer.hpp>


//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION


class KDDOSGuardSimulator
{
	DeclareSingleton( KDDOSGuardSimulator );
	DeclDump;
	DeclToStringW;

public:
	KDDOSGuardSimulator(void);
	~KDDOSGuardSimulator(void);

	void Tick();

	void RegUserSession( IN const std::string& strIP );
	bool IsRegUserSession( IN const std::string& strIP ) const;

private:
	std::map< std::string, boost::timer >		m_mapUserSession;
	boost::timer								m_tTimer;
};

DefSingletonInline( KDDOSGuardSimulator );


#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
//}}


