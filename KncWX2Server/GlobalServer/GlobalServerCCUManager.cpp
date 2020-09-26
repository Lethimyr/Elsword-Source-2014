#include "GlobalSimLayer.h"
#include "BaseServer.h"

#include "GlobalServerCCUManager.h"


ImplementSingleton( KGlobalServerCCUManager );

KGlobalServerCCUManager::KGlobalServerCCUManager(void)
{
	m_mapCCUInfo.clear();
}

KGlobalServerCCUManager::~KGlobalServerCCUManager(void)
{
	m_mapCCUInfo.clear();
}

ImplToStringW( KGlobalServerCCUManager )
{
	return stm_;
}

void KGlobalServerCCUManager::Tick()
{
}

void KGlobalServerCCUManager::UpdateCCUInfo( IN const KUpdateCCUInfo& kInfo )
{
	std::map< UidType, KUpdateCCUInfo >::iterator mit;
	mit = m_mapCCUInfo.find( kInfo.m_iServerUID );
	if( mit == m_mapCCUInfo.end() )
	{
		m_mapCCUInfo.insert( std::make_pair( kInfo.m_iServerUID, kInfo ) );
	}
	else
	{
        mit->second = kInfo;
	}
}

int	KGlobalServerCCUManager::GetNowTotalCCU()
{
	int iTotalCCU = 0;
	std::map< UidType, KUpdateCCUInfo >::iterator mit;
	mit = m_mapCCUInfo.begin();
	if( mit == m_mapCCUInfo.end() )
	{
		START_LOG( cerr, L"동접 데이터가 존재하지 않는다." );
		return 0;
	}
	for( ; mit != m_mapCCUInfo.end() ; ++mit )
	{
		iTotalCCU = iTotalCCU + mit->second.m_iConcurrentUser;
	}

	return iTotalCCU;
}