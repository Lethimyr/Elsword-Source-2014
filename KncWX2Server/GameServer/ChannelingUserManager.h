#ifdef SERV_CHANNELING_USER_MANAGER

#pragma once

#include <KNCSingleton.h>

//{{ 2009. 6. 10  최육사	타이머 매니저
#include "TimerManager.h"
//}}

class KChannelingUserManager
{
	DeclareSingleton( KChannelingUserManager );

public:

	KChannelingUserManager(void);
	~KChannelingUserManager(void);

	void UpdateChannelingUserList( IN bool bLogIn , IN int iChannelingCode , IN bool bGameServerLogin );
	void GetChannelingUserList( OUT std::map<int,int> &mapChannelingUserList );
	
private:
	
	std::map<int,int> m_mapChannelingUserList;

};

DefSingletonInline( KChannelingUserManager );

#endif SERV_CHANNELING_USER_MANAGER