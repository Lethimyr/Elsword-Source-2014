#include "PvpMonsterManager.h"
#include "X2Data/XSLUnitManager.h"


//{{ 2011. 10. 25	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KPvpMonsterManager::KPvpMonsterManager()
{
}

KPvpMonsterManager::~KPvpMonsterManager()
{
}

void KPvpMonsterManager::StartGame()
{
	KRoomMonsterManager::StartGame();
}

void KPvpMonsterManager::EndGame()
{
	KRoomMonsterManager::EndGame();

	//{{ 2011. 07. 19	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
	m_mapPvpNpcInfo.clear();
//#endif SERV_PVP_NEW_SYSTEM
	//}}
}

void KPvpMonsterManager::OnCloseRoom()
{
	KRoomMonsterManager::OnCloseRoom();

	//{{ 2011. 07. 19	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
	m_mapPvpNpcInfo.clear();
//#endif SERV_PVP_NEW_SYSTEM
	//}}
}

bool KPvpMonsterManager::CreatePvpMonster( IN const KNPCUnitReq& kCreateNpcInfo, OUT int& iNpcUID )
{
	iNpcUID = 0;

	// 1. 대전 전용 NPC를 찾는다!
	std::map< UidType, KPvpNpcInfo >::iterator mit;
	for( mit = m_mapPvpNpcInfo.begin(); mit != m_mapPvpNpcInfo.end(); ++mit )
	{
		if( mit->second.m_iNpcID == kCreateNpcInfo.m_NPCID )
			break;
	}

	if( mit == m_mapPvpNpcInfo.end() )
	{
		START_LOG( cerr, L"대전 NPC정보가 없는데 생성요청이 왔다!? 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kCreateNpcInfo.m_NPCID )
			<< END_LOG;
		return false;
	}
	

	// 2. 대전 전용 NPC의 UID값을 얻어내고 해당 NPC정보는 삭제한다!
	const int iPvpNpcUID = static_cast<int>(mit->first);
	m_mapPvpNpcInfo.erase( mit );


	// 3. 대전 전용 NPC생성!
	if( CreateMonster( iPvpNpcUID, kCreateNpcInfo ) == false )
	{
		START_LOG( cerr, L"대전 NPC 생성 실패!" )
			<< BUILD_LOG( kCreateNpcInfo.m_NPCID )
			<< END_LOG;
		return false;
	}

	// 클라이언트로 보낼 NPCUID값을 저장한다.
	iNpcUID = iPvpNpcUID;
	return true;
}

void KPvpMonsterManager::SetPvpNpcInfo( IN const std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo )
{
    if( m_mapPvpNpcInfo.empty() == false )
	{
		START_LOG( cerr, L"대전 NPC 정보가 남아있는데 업데이트 하려고 한다!" )
			<< BUILD_LOG( m_mapPvpNpcInfo.size() )
			<< BUILD_LOG( mapPvpNpcInfo.size() )
			<< END_LOG;
	}

	m_mapPvpNpcInfo = mapPvpNpcInfo;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}


