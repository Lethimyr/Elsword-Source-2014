#include "LoginSimLayer.h"
#include "BaseServer.h"

#include ".\accountblockmanager.h"



#ifdef SERV_ACCOUNT_BLOCK


ImplementSingleton( KAccountBlockManager );

KAccountBlockManager::KAccountBlockManager(void)
{
	for( int iBlockType = 0; iBlockType < BT_MAX; ++iBlockType )
	{
		m_iReleaseTick[iBlockType] = 0;
	}
}

KAccountBlockManager::~KAccountBlockManager(void)
{
}

ImplToStringW( KAccountBlockManager )
{
	return stm_;
}

void KAccountBlockManager::Tick()
{
	if( m_kTimer[TM_RELEASE_TICK].elapsed() <= 60.0 )
		return;

	m_kTimer[TM_RELEASE_TICK].restart();

	// 릴리즈틱 체크
	KDBE_CHECK_ACCOUNT_BLOCK_LIST_REQ kPacket;
	GetReleaseTick( kPacket.m_mapReleaseTick );
	SendToAccountDB( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ, kPacket );
}

void KAccountBlockManager::Init()
{
	START_LOG( cout, L"계정 블럭 리스트 요청!" );

	KDBE_CHECK_ACCOUNT_BLOCK_LIST_REQ kPacket;
	GetReleaseTick( kPacket.m_mapReleaseTick );
	SendToAccountDB( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ, kPacket );
}

//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
void KAccountBlockManager::UpdateAccountBlockList( IN const KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK& kPacket, 
												  OUT std::map< int, std::vector< UidType > >& mapBlockUserList,
												  OUT std::map< int, std::vector< UidType > >& mapUnblockUserList,
												  OUT std::vector< std::string >& vecBlockMachineIDList )
#else
void KAccountBlockManager::UpdateAccountBlockList( IN const KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK& kPacket, 
												  OUT std::map< int, std::vector< UidType > >& mapBlockUserList,
												  OUT std::map< int, std::vector< UidType > >& mapUnblockUserList )
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}
{
	mapBlockUserList.clear();
	mapUnblockUserList.clear();
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	vecBlockMachineIDList.clear();
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

	{ // ReleaseTick Update
		std::map< int, int >::const_iterator mit;
		for( mit = kPacket.m_mapReleaseTick.begin(); mit != kPacket.m_mapReleaseTick.end(); ++mit )
		{
			const int iBlockType = mit->first;
			const int iReleaseTick = mit->second;

			if( iBlockType < 0  ||  iBlockType >= BT_MAX )
				continue;

			m_iReleaseTick[iBlockType] = iReleaseTick;
		}
	}
	
	{
		std::map< int, std::set< UidType > >::const_iterator mit;
		for( mit = kPacket.m_mapAccountBlockList.begin(); mit != kPacket.m_mapAccountBlockList.end(); ++mit )
		{
			const BLOCK_TYPE eBlockType = static_cast<BLOCK_TYPE>(mit->first);
			const std::set< UidType >& setNewBlockList = mit->second;

			if( eBlockType < 0  ||  eBlockType >= BT_MAX )
				continue;

			{ // 기존 리스트에 없었던 유저들은 따로 골라낸다!
				std::vector< UidType > vecBlockUserList;

				BOOST_TEST_FOREACH( const UidType, iUserUID, setNewBlockList )
				{
					if( IsBlockedAccount( iUserUID, eBlockType ) == false )
					{
						vecBlockUserList.push_back( iUserUID );
					}
				}

				mapBlockUserList.insert( std::make_pair( eBlockType, vecBlockUserList ) );
			}
			
			{ // 기존 리스트에 있었는데 없어지는 유저들도 따로 골라낸다!
				std::vector< UidType > vecUnblockUserList;

				BOOST_TEST_FOREACH( const UidType, iUserUID, m_setAccountBlockList[eBlockType] )
				{
					if( setNewBlockList.find( iUserUID ) == setNewBlockList.end() )
					{
                        vecUnblockUserList.push_back( iUserUID );
					}
				}

				mapUnblockUserList.insert( std::make_pair( eBlockType, vecUnblockUserList ) );
			}
		}
	}

	{ // 블럭 리스트를 새로 갱신해준다!
		std::map< int, std::set< UidType > >::const_iterator mit;
		for( mit = kPacket.m_mapAccountBlockList.begin(); mit != kPacket.m_mapAccountBlockList.end(); ++mit )
		{
			const int iBlockType = mit->first;
			const std::set< UidType >& setNewBlockList = mit->second;

			if( iBlockType < 0  ||  iBlockType >= BT_MAX )
				continue;

			const u_int uiOldCount = m_setAccountBlockList[iBlockType].size();
			m_setAccountBlockList[iBlockType].clear();
			m_setAccountBlockList[iBlockType] = setNewBlockList;

			START_LOG( cout, L"계정 블럭 리스트 업데이트!" )
				<< BUILD_LOG( iBlockType )
				<< BUILD_LOG( uiOldCount )
				<< BUILD_LOG( m_setAccountBlockList[iBlockType].size() )
				<< BUILD_LOG( setNewBlockList.size() );
		}
	}

	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
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
#endif SERV_MACHINE_ID_BLOCK
	//}}
}

void KAccountBlockManager::GetReleaseTick( OUT std::map< int, int >& mapReleaseTick )
{
	mapReleaseTick.clear();

	for( int iBlockType = 0; iBlockType < BT_MAX; ++iBlockType )
	{
		mapReleaseTick.insert( std::make_pair( iBlockType, m_iReleaseTick[iBlockType] ) );
	}
}

void KAccountBlockManager::AddBlockedAccount( IN const UidType iUserUID, IN const BLOCK_TYPE eBlockType )
{
	if( eBlockType < 0  ||  eBlockType >= BT_MAX )
	{
		START_LOG( cerr, L"잘못된 블럭 타입 입니다!" )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( eBlockType )
			<< END_LOG;
		return;
	}

	m_setAccountBlockList[eBlockType].insert( iUserUID );
}

bool KAccountBlockManager::IsBlockedAccount( IN const UidType iUserUID, IN const BLOCK_TYPE eBlockType ) const
{
	if( eBlockType < 0  ||  eBlockType >= BT_MAX )
	{
		START_LOG( cerr, L"잘못된 블럭 타입 입니다!" )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( eBlockType )
			<< END_LOG;
		return false;
	}

	return ( m_setAccountBlockList[eBlockType].find( iUserUID ) != m_setAccountBlockList[eBlockType].end() );
}

void KAccountBlockManager::SendToAccountDB( unsigned short usEventID )
{
	SendToAccountDB( usEventID, char() );
}


#endif SERV_ACCOUNT_BLOCK

