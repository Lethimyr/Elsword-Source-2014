#include "DungeonRoom.h"
#include "DefenceDungeonManager.h"
#include "X2Data/XSLDefenceDungeonManager.h"


//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON


KDefenceDungeonManager::KDefenceDungeonManager() :
m_bEnterDefenceDungeon( false ),
m_bOpenDefenceDungeon( false ),
m_iDefenceDungeonPhase( CXSLDefenceDungeonManager::DDP_INIT )
{
}

KDefenceDungeonManager::~KDefenceDungeonManager()
{
}

void KDefenceDungeonManager::Clear()
{
	// 정보 초기화
	m_bOpenDefenceDungeon = false;
	m_bEnterDefenceDungeon = false;	
	m_iDefenceDungeonPhase = CXSLDefenceDungeonManager::DDP_INIT;
}

void KDefenceDungeonManager::StartGame()
{
	m_iDefenceDungeonPhase = CXSLDefenceDungeonManager::DDP_INIT;
}

void KDefenceDungeonManager::EndGame()
{
	m_iDefenceDungeonPhase = CXSLDefenceDungeonManager::DDP_INIT;
}

void KDefenceDungeonManager::OnCloseRoom()
{
	Clear();
}

//{{ 2013. 04. 29	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
bool KDefenceDungeonManager::DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const int iNpcLevel, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate, IN bool b100PerEventOn/*=false*/ )
#else //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
bool KDefenceDungeonManager::DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const int iNpcLevel, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate )
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
#else
bool KDefenceDungeonManager::DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate )
#endif SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
//}}
{
	static int ENTER_DEFENCE = 1;

	// 어둠의 문에서는 다시 체크되지 않는다.
	if( CXSLDungeon::IsDefenceDungeon( iDungeonID ) == true )
		return false;

	// 어둠의 문이 발생하지 않는 던전인지 체크
	if( SiCXSLDefenceDungeonManager()->IsNotBeginDefenceDungeonID( iDungeonID ) == true )
		return false;

	//{{ 2013. 04. 29	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
	int iDungeonNpcLevel = 0;

	// 던전 정보 얻기
	if( CXSLDungeon::IsSecretDungeon( iDungeonID ) )
	{
		iDungeonNpcLevel = iNpcLevel;
	}
	else
	{
		iDungeonNpcLevel = SiCXSLDungeonManager()->GetDungeonNpcLevel( iDungeonID );
		if( iDungeonNpcLevel == 0 )
		{
			START_LOG( cerr, L"던전 npc 레벨 정보가 이상합니다!" )
				<< BUILD_LOG( iDungeonID )
				<< END_LOG;
			return false;
		}
	}
#else
	const int iDungeonNpcLevel = SiCXSLDungeonManager()->GetDungeonNpcLevel( iDungeonID );
	if( iDungeonNpcLevel == 0 )
	{
		START_LOG( cerr, L"던전 npc 레벨 정보가 이상합니다!" )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
		return false;
	}
#endif SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
	//}}	

	// 어둠의 문 입장 확률 얻기
	float fEnterDefenceRate = SiCXSLDefenceDungeonManager()->GetEnterDefenceDungeonRate( iDungeonNpcLevel, mapUnitLevelList );
	
	// 어둠의 문이 열리지 않았다면 확률을 낮추자!
	if( m_bOpenDefenceDungeon == false )
	{
		fEnterDefenceRate *= SiCXSLDefenceDungeonManager()->GetNotOpenGateMultiplyValue();
	}
	
	if( fMultipleRate > 1.f )
	{
		fEnterDefenceRate *= fMultipleRate;
	}

	if( fEnterDefenceRate > 100.f )
		fEnterDefenceRate = 100.f;


#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	if(b100PerEventOn)
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_0_PERCENT_EVENT
		fEnterDefenceRate = 0.f;
#else //SERV_NEW_DEFENCE_DUNGEON_ENTER_0_PERCENT_EVENT
		fEnterDefenceRate = 100.f;
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_0_PERCENT_EVENT
		START_LOG( clog, L"김석근_어둠의 문 100% 이벤트 돌입!" )
			<< BUILD_LOG( fEnterDefenceRate )
			<< END_LOG;
	}
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT

	// 확률 입력
	KLottery kLotEnterDefence;
	kLotEnterDefence.AddCase( ENTER_DEFENCE, fEnterDefenceRate );

	// 결정하기!
	if( kLotEnterDefence.Decision() == ENTER_DEFENCE ) // 테스트로 무조건 입장하도록 처리
	{
		m_bEnterDefenceDungeon = true;
	}
	else
	{
		m_bEnterDefenceDungeon = false;
	}

	return IsEnterDefenceDungeon();
}

bool KDefenceDungeonManager::IncreaseDefenceDungeonPhase( IN const int iNpcID )
{
	if( SiCXSLDefenceDungeonManager()->IsDefenceGuardianMonster( iNpcID ) == false )
		return false;

	// 페이즈 증가!
	++m_iDefenceDungeonPhase;

	// 3이상은 없다
	if( m_iDefenceDungeonPhase > CXSLDefenceDungeonManager::DDP_PHASE_3 )
	{
		m_iDefenceDungeonPhase = CXSLDefenceDungeonManager::DDP_PHASE_3;
		return false;
	}

	return true;
}


#endif SERV_NEW_DEFENCE_DUNGEON
//}}


