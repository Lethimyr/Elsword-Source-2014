#include "ChannelSimLayer.h"
#include "BaseServer.h"

#include "MachineBlockManager.h"



//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING


ImplementSingleton( KMachineBlockManager );

KMachineBlockManager::KMachineBlockManager(void)
{
	for( int iBlockType = 0; iBlockType < BT_MAX; ++iBlockType )
	{
		m_iReleaseTick[iBlockType] = 0;
	}
}

KMachineBlockManager::~KMachineBlockManager(void)
{
}

ImplToStringW( KMachineBlockManager )
{
	return stm_;
}

void KMachineBlockManager::Tick()
{
	if( m_kTimer[TM_RELEASE_TICK].elapsed() <= 60.0 )
		return;

	m_kTimer[TM_RELEASE_TICK].restart();

	// 릴리즈틱 체크
	KDBE_CHECK_ACCOUNT_BLOCK_LIST_REQ kPacket;
	GetReleaseTick( kPacket.m_mapReleaseTick );
	SendToAccountDB( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ, kPacket );
}

void KMachineBlockManager::Init()
{
	START_LOG( cout, L"계정 블럭 리스트 요청!" );

	KDBE_CHECK_ACCOUNT_BLOCK_LIST_REQ kPacket;
	GetReleaseTick( kPacket.m_mapReleaseTick );
	SendToAccountDB( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ, kPacket );
}

void KMachineBlockManager::UpdateMachineBlockList( IN const KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK& kPacket, OUT std::vector< std::string >& vecBlockMachineIDList )
{
	vecBlockMachineIDList.clear();


	{ // ReleaseTick Update
		std::map< int, int >::const_iterator mit;
		for( mit = kPacket.m_mapReleaseTick.begin(); mit != kPacket.m_mapReleaseTick.end(); ++mit )
		{
			const int iBlockType = mit->first;
			const int iReleaseTick = mit->second;

			if( iBlockType != BT_MACHINE_ID_BLOCK )
				continue;

			m_iReleaseTick[iBlockType] = iReleaseTick;
		}
	}
	{
		u_int uiOldCount = m_setMachineIDBlockList.size();

		BOOST_TEST_FOREACH( const std::string&, strNewMachineID, kPacket.m_vecMachineIDBlockList )
		{
			if( IsBlockedMachineID( strNewMachineID ) == false )
			{
				vecBlockMachineIDList.push_back( strNewMachineID );
			}
		}

		m_setMachineIDBlockList.clear();
		BOOST_TEST_FOREACH( const std::string&, strNewMachineID, kPacket.m_vecMachineIDBlockList )
		{
			m_setMachineIDBlockList.insert( strNewMachineID );
		}

		START_LOG( cout, L"머신 ID 블럭 리스트 업데이트!" )
			<< BUILD_LOG( uiOldCount )
			<< BUILD_LOG( m_setMachineIDBlockList.size() )
			<< BUILD_LOG( vecBlockMachineIDList.size() );
	}
}

void KMachineBlockManager::GetReleaseTick( OUT std::map< int, int >& mapReleaseTick )
{
	mapReleaseTick.clear();

	for( int iBlockType = 0; iBlockType < BT_MAX; ++iBlockType )
	{
		mapReleaseTick.insert( std::make_pair( iBlockType, m_iReleaseTick[iBlockType] ) );
	}
}

void KMachineBlockManager::SendToAccountDB( unsigned short usEventID )
{
	SendToAccountDB( usEventID, char() );
}

#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}
