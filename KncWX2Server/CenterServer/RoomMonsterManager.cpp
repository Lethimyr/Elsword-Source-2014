#include "RoomMonsterManager.h"
#include "X2Data/XSLUnitManager.h"


//{{ 2011. 01. 20	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW


KRoomMonsterManager::KRoomMonsterManager() : 
m_nNPCUID( 0 )
{
}

KRoomMonsterManager::~KRoomMonsterManager()
{
}

void KRoomMonsterManager::StartGame()
{
	m_nNPCUID = 1;
	m_mapAliveNpcList.clear();
	m_mapDieNpcList.clear();
}

void KRoomMonsterManager::EndGame()
{
    m_mapAliveNpcList.clear();
	m_mapDieNpcList.clear();
}

void KRoomMonsterManager::OnCloseRoom()
{
	m_nNPCUID = 1;
	m_mapAliveNpcList.clear();
	m_mapDieNpcList.clear();
}

bool KRoomMonsterManager::CreateMonster( IN const int iNpcUID, IN const KNPCUnitReq& kNpcInfo )
{
	int iDummy = 0;
    return CreateMonster( kNpcInfo, iDummy, iNpcUID );
}

bool KRoomMonsterManager::CreateMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID, IN const int iNpcUID /*= 0*/ )
{
	if( CXSLUnitManager::IsValidMonsterGrade( static_cast<CXSLUnitManager::MONSTER_GRADE>(kNpcInfo.m_cMonsterGrade) ) == false )
	{
        START_LOG( cerr, L"몬스터 등급 정보가 이상합니다!" )
			<< BUILD_LOG( kNpcInfo.m_NPCID )
			<< BUILD_LOGc( kNpcInfo.m_cMonsterGrade )
			<< END_LOG;
		//return false;
	}	

	// 1. NPC UID 발급
	if( iNpcUID != 0 )
	{
		iCreatedNpcUID = iNpcUID;
	}
	else
	{
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		iCreatedNpcUID = GetNewNpcUID();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
//		iCreatedNpcUID = m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
	}

	// 3. NPC(Monster) 관리를 위한 data 보관
	NPC_DATA npcData;
	npcData.m_cLevel	= kNpcInfo.m_Level;
	npcData.m_iNPCID	= kNpcInfo.m_NPCID;
	npcData.m_bActive	= kNpcInfo.m_bActive;
	npcData.m_bNoDrop	= kNpcInfo.m_bNoDrop;
	npcData.m_bIsBoss	= ( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC );	// 몬스터 등급으로 보스몬스터인지 체크.
	//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//보스몬스터인지 체크.
	//{{ 2009. 12. 18  최육사	던전경험치개편
	npcData.m_cMonsterGrade = kNpcInfo.m_cMonsterGrade; //일반몹인지 중간보스인지 보스인지 체크.
	//}}
	//{{ 2011. 11. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	npcData.m_iGroupID	= kNpcInfo.m_iGroupID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	m_mapAliveNpcList.insert( std::make_pair( iCreatedNpcUID, npcData ) );
	return true;
}

bool KRoomMonsterManager::GetNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const
{
	std::map< int, NPC_DATA >::const_iterator mit;
	mit = m_mapAliveNpcList.find( iNpcUID );
	if( mit == m_mapAliveNpcList.end() )
	{
		// 살아있는 몬스터중에 없으면 죽은 몬스터리스트에서 찾는다.
		mit = m_mapDieNpcList.find( iNpcUID );
		if( mit == m_mapDieNpcList.end() )
		{
			return false;
		}

		kNpcData = mit->second;
		return true;
	}

	kNpcData = mit->second;
	return true;
}

bool KRoomMonsterManager::IsExistMonster( IN const int iNpcUID ) const
{
	bool bIsDie = false;
    return IsExistMonster( iNpcUID, bIsDie );
}

bool KRoomMonsterManager::IsExistMonster( IN const int iNpcUID, OUT bool& bIsDie ) const
{
	std::map< int, NPC_DATA >::const_iterator mit;
	mit = m_mapAliveNpcList.find( iNpcUID );	
	if( mit == m_mapAliveNpcList.end() )
	{
		// 살아있는 몬스터중에 없으면 죽은 몬스터리스트에서 찾는다.
		mit = m_mapDieNpcList.find( iNpcUID );
		if( mit == m_mapDieNpcList.end() )
		{
			return false;
		}

		bIsDie = true;
		return true;
	}

	bIsDie = false;
	return true;
}

//{{ 2013. 04. 16	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
bool KRoomMonsterManager::IsAllBossMonstersDead() const
{
	std::map< int, NPC_DATA >::const_iterator mit;
	for( mit = m_mapAliveNpcList.begin(); mit != m_mapAliveNpcList.end(); ++mit )
	{
		if( mit->second.m_bIsBoss )
			return false;
	}

	return true;
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

bool KRoomMonsterManager::SetMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID, IN const bool bInsertDieList /*= true*/ )
{
	std::map< int, NPC_DATA >::iterator mit;
	mit = m_mapAliveNpcList.find( iNpcUID );
	if( mit == m_mapAliveNpcList.end() )
	{
		// 살아있는 몬스터 리스트에 없으면 실패!
		return false;
	}

	// 죽은 몬스터 리스트에 넣는다!
	if( bInsertDieList )
	{
		m_mapDieNpcList.insert( std::make_pair( iNpcUID, mit->second ) );
	}

	// 살아있는 몬스터 리스트에서는 뺀다!
	m_mapAliveNpcList.erase( mit );
	return true;
}

bool KRoomMonsterManager::SetMonsterRebirth( IN const int iNpcUID )
{
	std::map< int, NPC_DATA >::iterator mit;
	mit = m_mapDieNpcList.find( iNpcUID );
	if( mit == m_mapDieNpcList.end() )
	{
		// 살아있는 몬스터를 부활시킬수는 없습니다!
		return false;
	}

	// 죽은 몬스터 리스트에 넣는다!
	mit->second.m_uiAttUnit = 0;
	m_mapAliveNpcList.insert( std::make_pair( iNpcUID, mit->second ) );

	// 살아있는 몬스터 리스트에서는 뺀다!
	m_mapDieNpcList.erase( mit );
	return true;
}

void KRoomMonsterManager::PrintLuaLog_LUA( int iType, double fValue )
{
    START_LOG( cout, L"루아 로그 확인" )
		<< BUILD_LOG( iType )
		<< BUILD_LOG( fValue );
}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
int KRoomMonsterManager::GetNewNpcUID()
{
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	while( m_setReservedNpcUID.find( m_nNPCUID ) != m_setReservedNpcUID.end() )
		m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		

	return m_nNPCUID++;
}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#endif SERV_DUNGEON_RANK_NEW
//}}


//{{ 2011. 11. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM

void KRoomMonsterManager::GetAliveMonsterCountInfo( OUT CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterInfo ) const
{
	std::map< int, NPC_DATA >::const_iterator mit;
	for( mit = m_mapAliveNpcList.begin(); mit != m_mapAliveNpcList.end(); ++mit )
	{
		// 살아있는 몬스터 정보를 담는다.
		kAliveMonsterInfo.AddGroupMonsterCount( mit->second.m_iGroupID, mit->second.m_iNPCID );
	}
}

bool KRoomMonsterManager::GetMonsterGroupID( IN const int iNpcUID, OUT int& iGroupID ) const
{
	iGroupID = 0;

	std::map< int, NPC_DATA >::const_iterator mit;
	mit	= m_mapAliveNpcList.find( iNpcUID );
	if( mit	== m_mapAliveNpcList.end() )
	{
		START_LOG( cerr, L"존재하는 몬스터가 아닙니다!" )
			<< BUILD_LOG( iNpcUID )
			<< END_LOG;
		return false;
	}

	iGroupID = mit->second.m_iGroupID;
	return true;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

