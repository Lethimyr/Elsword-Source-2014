#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "KncSend.h"


class KCCUToolManager
{
	DeclareSingleton( KCCUToolManager );
	DeclDump;
	DeclToStringW;
	
public:
	typedef __int64												CCUDate;
	typedef std::map< UidType, std::vector< KCCUInfo > >		CCUDateData;


	KCCUToolManager();
	~KCCUToolManager();

	void Tick();

	const CCUDateData* GetCCUData( IN CCUDate iCCUDate );
	const CCUDateData* GetCCUData( IN CTime tCCUDate );

	bool NewCCUData( IN UidType iServerID, IN CCUDate iCCUDate, IN std::vector< KCCUInfo > vecCCUInfo );
	bool SetCCUData( IN CCUDate iCCUDate, IN UidType iServerID, IN std::vector< KCCUInfo > vecCCUInfo, IN char cCCUType );

	// 동접 함수	
	bool GetExistCCU( IN const KE_TOOL_GET_CCU_INFO_REQ& kReq, OUT KE_TOOL_GET_CCU_INFO_ACK& kAck );

	// send event
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	std::map< CCUDate, CCUDateData > m_mapCCU;
};

DefSingletonInline( KCCUToolManager );

template < class T >
void KCCUToolManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_CHANNEL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}

