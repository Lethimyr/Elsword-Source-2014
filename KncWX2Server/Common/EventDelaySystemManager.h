#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "KncSend.h"



class KEventDelaySystemManager
{
	DeclareSingleton( KEventDelaySystemManager );
	DeclDump;
	DeclToStringW;

private:
	struct KOpenRandomCubeEvent
	{
		UidType						m_iUnitUID;
		KEGS_OPEN_RANDOM_ITEM_REQ	m_kEvent;

		KOpenRandomCubeEvent( const UidType iUnitUID, const KEGS_OPEN_RANDOM_ITEM_REQ& kPacket )
		{
			m_iUnitUID = iUnitUID;
			m_kEvent = kPacket;
		}
	};

public:
	KEventDelaySystemManager();
	~KEventDelaySystemManager();

	void Tick();

	bool IsRegRandomCubeEvent( IN const UidType iUnitUID ) const		{ return ( m_setRegUnitUID.find( iUnitUID ) != m_setRegUnitUID.end() ); }
	void RegOpenRandomCubeEvent( IN const UidType iUnitUID, IN const KEGS_OPEN_RANDOM_ITEM_REQ& kEvent );
	void UnRegOpenRandomCubeEvent( IN const UidType iUnitUID );
	
	// send event
	template < class T > void SendToGSUser( const UidType iUnitUID, unsigned short usEventID, const T& data );

private:	
	std::deque< KOpenRandomCubeEvent >	m_deqOpenRandomCubeEvent;
	std::set< UidType >					m_setRegUnitUID;
	boost::timer						m_tTimer;
};

DefSingletonInline( KEventDelaySystemManager );

template < class T >
void KEventDelaySystemManager::SendToGSUser( const UidType iUnitUID, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, NULL, usEventID, data );
}

