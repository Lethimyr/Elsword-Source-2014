#pragma once

#include "RefreshSingleton.h"
#include <KncSend.h>
#include <boost/timer.hpp>
#include "BaseServer.h"
#include <map>


//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM

class KWeddingHallManager
{
private:
	DeclareRefreshSingleton( KWeddingHallManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:
	KWeddingHallManager(void);
	~KWeddingHallManager(void);

	

	void Init();
	void ShutDown();
	void SetWeddingInfoDB( IN std::map< int, KWeddingHallInfo >& mapWeddingInfo );
	void UpdateWeddingInfo_DB();



	bool AddWeddingInfo( IN KWeddingHallInfo& kWeddingInfo );		// 프로포즈 완료
	bool UpdateWeddingInfo( IN KWeddingHallInfo& kWeddingInfo );	// 룸UID 정보 변경
	bool DeleteWeddingInfo( IN int iWeddingUID );					// 결혼식 완료

	bool GetWeddingInfo( IN int iWeddingUID, OUT KWeddingHallInfo& kWeddingInfo );
	bool GetWeddingItemInfo( IN OUT std::map< UidType, KWeddingItemInfo >& mapWeddingItemInfo );

	void GetAllWeddingInfo( OUT std::map< int, KWeddingHallInfo >& mapWeddingInfo ) { mapWeddingInfo = m_mapWeddingHallDBData; }
protected:
	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	void SendToGameDB( unsigned short usEventID );
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:

	std::map< int, KWeddingHallInfo >			m_mapWeddingHallDBData;  // [결혼식uid,결혼식정보] 예약된 결혼식 정보 - db 관리 겸용 데이타

};

DefRefreshSingletonInline( KWeddingHallManager );

template < class T >
void KWeddingHallManager::SendToGameDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, usEventID, data );
}

template < class T >
void KWeddingHallManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}

#endif SERV_RELATIONSHIP_SYSTEM
//}