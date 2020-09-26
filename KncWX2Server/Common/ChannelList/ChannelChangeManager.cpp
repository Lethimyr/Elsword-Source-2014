#include ".\channelchangemanager.h"


ImplementSingleton( KChannelChangeManager );

KChannelChangeManager::KChannelChangeManager(void)
{
}

KChannelChangeManager::~KChannelChangeManager(void)
{	
}

ImplToStringW( KChannelChangeManager )
{
	return stm_;
}