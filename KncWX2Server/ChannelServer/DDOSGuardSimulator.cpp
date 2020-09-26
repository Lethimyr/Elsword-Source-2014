#include "ChannelSimLayer.h"
#include "BaseServer.h"

#include "DDOSGuardSimulator.h"



//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION


ImplementSingleton( KDDOSGuardSimulator );

KDDOSGuardSimulator::KDDOSGuardSimulator(void)
{
}

KDDOSGuardSimulator::~KDDOSGuardSimulator(void)
{
}

ImplToStringW( KDDOSGuardSimulator )
{
	return stm_;
}

void KDDOSGuardSimulator::Tick()
{
    if( m_tTimer.elapsed() < 1.0 )
		return;

	m_tTimer.restart();

	std::vector< std::string > vecEraseList;

	std::map< std::string, boost::timer >::iterator mit;
	for( mit = m_mapUserSession.begin(); mit != m_mapUserSession.end(); ++mit )
	{
        if( mit->second.elapsed() > 5.0 )
		{
			vecEraseList.push_back( mit->first );
		}
	}

	std::vector< std::string >::const_iterator vit;
	for( vit = vecEraseList.begin(); vit != vecEraseList.end(); ++vit )
	{
        m_mapUserSession.erase( *vit );
	}
}

void KDDOSGuardSimulator::RegUserSession( IN const std::string& strIP )
{
	std::map< std::string, boost::timer >::iterator mit;
	mit = m_mapUserSession.find( strIP );
	if( mit == m_mapUserSession.end() )
	{
		m_mapUserSession.insert( std::make_pair( strIP, boost::timer() ) );
	}
	else
	{
        mit->second.restart();
	}
}

bool KDDOSGuardSimulator::IsRegUserSession( IN const std::string& strIP ) const
{
	std::map< std::string, boost::timer >::const_iterator mit;
	mit = m_mapUserSession.find( strIP );
	if( mit == m_mapUserSession.end() )
	{
		return false;
	}
	
	return true;
}

#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
//}}



