#include "BaseServer.h"
#include "EventDelaySystemManager.h"
#include "GameSysVal/GameSysVal.h"


ImplementSingleton( KEventDelaySystemManager );

KEventDelaySystemManager::KEventDelaySystemManager()
{
}

KEventDelaySystemManager::~KEventDelaySystemManager()
{
}

ImplToStringW( KEventDelaySystemManager )
{
	return stm_;
}

void KEventDelaySystemManager::Tick()
{
	if( SiKGameSysVal()->IsEnableOpenRandomCubeDelaySystem() == false )
		return;

	// 등록된 지연 시간만큼 큐브 오픈을 지연시킨다.
	if( m_tTimer.elapsed() < SiKGameSysVal()->GetOpenRandomCubeDelayTime() )
		return;

	m_tTimer.restart();

	// 등록된 카운트 만큼 랜덤 큐브 오픈을 시도한다!
	for( int iCnt = 0; iCnt < SiKGameSysVal()->GetOpenRandomCubeDelayCount(); ++iCnt )
	{
		if( m_deqOpenRandomCubeEvent.empty() )
			break;

		// 리스트에서 하나를 꺼낸다.
		KOpenRandomCubeEvent kEvent = m_deqOpenRandomCubeEvent.front();
		m_deqOpenRandomCubeEvent.pop_front();
		m_setRegUnitUID.erase( kEvent.m_iUnitUID );

		// 딜레이 패킷 OK처리
		kEvent.m_kEvent.m_bDelayedOK = true;

		// GSUser한테 다시 보내자!
		SendToGSUser( kEvent.m_iUnitUID, EGS_OPEN_RANDOM_ITEM_REQ, kEvent.m_kEvent );
	}
}

void KEventDelaySystemManager::RegOpenRandomCubeEvent( IN const UidType iUnitUID, IN const KEGS_OPEN_RANDOM_ITEM_REQ& kEvent )
{
	if( IsRegRandomCubeEvent( iUnitUID ) )
	{
		// 이미 등록되어있으면 원래 있던거 지우고 다시 등록하자!
		UnRegOpenRandomCubeEvent( iUnitUID );
	}

	m_setRegUnitUID.insert( iUnitUID );
	m_deqOpenRandomCubeEvent.push_back( KOpenRandomCubeEvent( iUnitUID, kEvent ) );
}

void KEventDelaySystemManager::UnRegOpenRandomCubeEvent( IN const UidType iUnitUID )
{
    if( IsRegRandomCubeEvent( iUnitUID ) == false )
		return;

	m_setRegUnitUID.erase( iUnitUID );

	std::deque< KOpenRandomCubeEvent >::iterator dit;
	for( dit = m_deqOpenRandomCubeEvent.begin(); dit != m_deqOpenRandomCubeEvent.end(); ++dit )
	{
		if( dit->m_iUnitUID == iUnitUID )
			break;
	}

	if( dit != m_deqOpenRandomCubeEvent.end() )
	{
		m_deqOpenRandomCubeEvent.erase( dit );
	}	
}



