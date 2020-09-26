#include "BaseServer.h"
#include "HackingProtectionManager.h"


//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST


ImplementSingleton( KHackingProtectionManager );

KHackingProtectionManager::KHackingProtectionManager() :
m_iReleaseTick( 0 )
{
}

KHackingProtectionManager::~KHackingProtectionManager()
{
}

ImplToStringW( KHackingProtectionManager )
{
	return stm_;
}

void KHackingProtectionManager::Tick()
{
	if( m_tRefreshTimer.elapsed() <= 60.0 )
		return;

	m_tRefreshTimer.restart();

	KDBE_CHECK_HACKING_TOOL_LIST_REQ kPacketToDB;
	kPacketToDB.m_iReleaseTick = m_iReleaseTick;
	SendToLogDB( DBE_CHECK_HACKING_TOOL_LIST_REQ, kPacketToDB );
}

void KHackingProtectionManager::Init()
{
	KDBE_CHECK_HACKING_TOOL_LIST_REQ kPacketToDB;
	kPacketToDB.m_iReleaseTick = m_iReleaseTick;
	SendToLogDB( DBE_CHECK_HACKING_TOOL_LIST_REQ, kPacketToDB );
}

void KHackingProtectionManager::UpdateHackingToolList( IN const KDBE_CHECK_HACKING_TOOL_LIST_ACK& kInfo )
{
	// ReleaseTick이 같다면 리스트가 변경되지 않은것이므로 아무것도 처리하지 않는다.
	if( m_iReleaseTick == kInfo.m_iReleaseTick )
	{
		return;
	}
	else if( m_iReleaseTick > kInfo.m_iReleaseTick )
	{
		START_LOG( cerr, L"ReleaseTick값이 더 작아졌다. 일어날수 없는 에러!" )
			<< BUILD_LOG( m_iReleaseTick )
			<< BUILD_LOG( kInfo.m_iReleaseTick )
			<< END_LOG;

		return;
	}

	// ReleaseTick을 업데이트한다!
	m_iReleaseTick = kInfo.m_iReleaseTick;

	// 해킹 툴 리스트를 업데이트 한다!
	m_vecHackingToolList.clear();
	m_vecHackingToolList = kInfo.m_vecHackingToolList;

	START_LOG( cout, L"해킹 툴 리스트가 업데이트 되었습니다!" )
		<< BUILD_LOG( m_vecHackingToolList.size() );
}

void KHackingProtectionManager::GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList )
{
	vecHackingToolList.clear();

	vecHackingToolList = m_vecHackingToolList;
	iRandomKey = rand(); // 랜덤한 키값 제공
}

void KHackingProtectionManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}


#endif SERV_HACKING_TOOL_LIST
//}}


