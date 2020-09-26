#include "GameServer.h"
#include ".\ChannelingUserManager.h"
#include "Enum/Enum.h"
#include "socket/NetCommon.h"

#ifdef SERV_CHANNELING_USER_MANAGER

ImplementSingleton( KChannelingUserManager );

KChannelingUserManager::KChannelingUserManager(void)
{	
	m_mapChannelingUserList.clear();

	for(int iChannelingCode = 0; iChannelingCode < KNexonAccountInfo::JCE_MAX; iChannelingCode++)
	{
		m_mapChannelingUserList.insert( std::make_pair( iChannelingCode, 0 ) );
	}
}

KChannelingUserManager::~KChannelingUserManager(void)
{
	m_mapChannelingUserList.clear();
}

void KChannelingUserManager::UpdateChannelingUserList( IN bool bLogIn, IN int iChannelingCode , IN bool bGameServerLogin = false )
{
	if( KNexonAccountInfo::JCE_NONE >= iChannelingCode || iChannelingCode >= KNexonAccountInfo::JCE_MAX )
	{
		return;
	}

	std::map<int,int>::iterator mit;

	mit = m_mapChannelingUserList.find( iChannelingCode );
	if( mit != m_mapChannelingUserList.end() )
	{
		if( bLogIn == true )
			mit->second += 1;
		else if( bGameServerLogin == true )
			mit->second -= 1;
	}
	else
	{
		m_mapChannelingUserList.insert( std::make_pair( iChannelingCode, 1 ) );
	}

}

void KChannelingUserManager::GetChannelingUserList( OUT std::map<int,int> &mapChannelingUserList )
{
	if(!m_mapChannelingUserList.empty())
		mapChannelingUserList = m_mapChannelingUserList;
}

#endif SERV_CHANNELING_USER_MANAGER