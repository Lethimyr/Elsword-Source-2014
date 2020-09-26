#include "ChannelUser.h"
#include "HackingProtectionManager.h"


//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST


KUserHackingManager::KUserHackingManager(void)
{
	// 접속 하자마자 보내도록 0값으로 초기화 한다.
	m_fHackingToolCheckTime = 0.0f;
}

KUserHackingManager::~KUserHackingManager(void)
{
}

void KUserHackingManager::SetRandomCheckTime()
{
	int iRandNum = rand() % static_cast<int>(HE_MAX_TIME - HE_MIN_TIME);
	iRandNum += static_cast<int>(HE_MIN_TIME);

	m_fHackingToolCheckTime = static_cast<double>(iRandNum);
}

void KUserHackingManager::GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList )
{
	// 해킹툴 리스트를 얻는다.
	SiKHackingProtectionManager()->GetHackingToolList( iRandomKey, vecHackingToolList );

	// 랜덤 체크 타임을 변경한다.
    SetRandomCheckTime();
}

#endif SERV_HACKING_TOOL_LIST
//}}