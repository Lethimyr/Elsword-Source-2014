#include "GlobalSimLayer.h"
#include "BaseServer.h"
#include "BlockListManager.h"


//{{ 2013. 05. 14	최육사	아이템 개편
#ifdef SERV_BLOCK_LIST


ImplementRefreshSingleton( KBlockListManager );

KBlockListManager::KBlockListManager(void) :
m_iReleaseTick( 0 )
{
}

KBlockListManager::~KBlockListManager(void)
{
}

ImplToStringW( KBlockListManager )
{
	stm_	<< L"----------[ KBlockListManager ]----------" << std::endl
			//<< TOSTRINGW( m_bActive )
			;

	return stm_;
}

ImplementLuaScriptParser( KBlockListManager )
{

	lua_tinker::class_add<KBlockListManager>( GetLuaState(), "KBlockListManager" );
	lua_tinker::class_def<KBlockListManager>( GetLuaState(), "dump",							&KBlockListManager::Dump );

	lua_tinker::decl( GetLuaState(), "BlockListManager", this );
}

void KBlockListManager::Init()
{
	// 서버 켜자마자 블럭 리스트 가져오기
	KDBE_CHECK_BLOCK_LIST_REQ kPacketToDB;
	kPacketToDB.m_iReleaseTick = GetReleaseTick();
	SendToAccountDB( DBE_CHECK_BLOCK_LIST_REQ, kPacketToDB );
}

void KBlockListManager::Tick()
{
	if( m_tTimer.elapsed() < 60.0 )
		return;

	m_tTimer.restart();

	// 1분에 한번씩 releasetick값을 체크하러 DB에 갑니다.
	KDBE_CHECK_BLOCK_LIST_REQ kPacketToDB;
	kPacketToDB.m_iReleaseTick = GetReleaseTick();
	SendToAccountDB( DBE_CHECK_BLOCK_LIST_REQ, kPacketToDB );
}

void KBlockListManager::UpdateblockList( IN const int iReleaseTick, IN const std::map< int, std::vector< KBlockInfo > >& mapBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock )
{
	vecNewBlock.clear();
	vecDelBlock.clear();

	if( GetReleaseTick() > iReleaseTick )
	{
		START_LOG( cerr, L"DB로부터 받아온 ReleaseTick값이 이상합니다." )
			<< BUILD_LOG( GetReleaseTick() )
			<< BUILD_LOG( iReleaseTick )
			<< BUILD_LOG( mapBlockInfo.size() )
			<< END_LOG;
		return;
	}

	// DB로부터 받은 Tick값과 현재 Tick값이 같으면 변화된 내용이 없다는 의미이다.
	if( GetReleaseTick() == iReleaseTick )
		return;

	// 릴리즈틱 업데이트
	SetReleaseTick( iReleaseTick );

	// 원래 있던 리스트와 비교한다.	
	for( int iType = 0; iType < KBlockInfo::BT_MAX; ++iType )
	{
		std::vector< KBlockInfo > vecBlockInfo;		

		std::map< int, std::vector< KBlockInfo > >::const_iterator mit;
		mit = mapBlockInfo.find( iType );
		if( mit != mapBlockInfo.end() )
		{
			vecBlockInfo = mit->second;
		}

		switch( iType )
		{
		case KBlockInfo::BT_TRADE_BLOCK:
			{
				UpdateTradeBlock( vecBlockInfo, vecNewBlock, vecDelBlock );
			}
			break;

		case KBlockInfo::BT_ACCOUNT_BLOCK:
			{
				UpdateAccountBlock( vecBlockInfo, vecNewBlock, vecDelBlock );
			}
			break;

		case KBlockInfo::BT_IP_BLOCK:
			{
				UpdateIpBlock( vecBlockInfo );
			}
			break;

		case KBlockInfo::BT_DLL_BLOCK:
			{
				UpdateModuleList( vecBlockInfo );
			}
			break;

		default:
			{
				START_LOG( cerr, L"등록되지 않은 블록타입입니다." )
					<< BUILD_LOGc( iType )
					<< END_LOG;
			}
			break;
		}
	}
}

void KBlockListManager::UpdateTradeBlock( IN const std::vector< KBlockInfo >& vecBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock )
{
	// 개인거래 블럭은 실시간으로 블럭을 풀어줘야하는 목적으로 수정된 항목에 대해서 리스트를 따로 유지하자.

	std::set< UidType > setUpdateBlock;	// DB로부터 받아온 블럭 대상 리스트
	std::set< UidType > setDelBlock;	// 제재 해제가 될 블럭 대상 리스트

	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecBlockInfo )
	{
		const UidType iTradeBlockUserUID = kInfo.m_iBlockTarget;

		// DB로 부터 받아온 데이터들을 따로 리스트로 담자
		setUpdateBlock.insert( iTradeBlockUserUID );

		std::map< UidType, KBlockData >::iterator mit;
		mit = m_mapTradeBlock.find( iTradeBlockUserUID );
		if( mit == m_mapTradeBlock.end() )
		{
			m_mapTradeBlock.insert( std::make_pair( iTradeBlockUserUID, KBlockData( kInfo ) ) );

			// 해당 블럭이 시작 되었는지 확인
			//if( kNewBlock.IsActiveBlock() == true )
			//{
				vecNewBlock.push_back( kInfo );
			//}
		}
		else
		{
			// 이전 상태를 저장
			//const bool bBeforeActive = mit->second.IsActiveBlock();

			// 새로운 내용으로 업데이트
			mit->second.SetBlockInfo( kInfo );

			// 새로 활성화 되는건가?
			//if( bBeforeActive == false  &&  mit->second.IsActiveBlock() == true )
			//{
			//	vecNewBlock.push_back( kInfo );
			//}
			//else if( bBeforeActive == true  &&  mit->second.IsActiveBlock() == false )
			//{
			//	vecDelBlock.push_back( kInfo );
			//}
		}
	}

	std::map< UidType, KBlockData >::const_iterator mit;
	for( mit = m_mapTradeBlock.begin(); mit != m_mapTradeBlock.end(); ++mit )
	{
		if( setUpdateBlock.find( mit->first ) != setUpdateBlock.end() )
			continue;

		setDelBlock.insert( mit->first );
	}

	BOOST_TEST_FOREACH( const UidType, iBlockTarget, setDelBlock )
	{
		std::map< UidType, KBlockData >::iterator mit;
		mit = m_mapTradeBlock.find( iBlockTarget );
		if( mit == m_mapTradeBlock.end() )
			continue;

		// 블럭 정보 얻고
		KBlockInfo kInfo;
		mit->second.GetBlockInfo( kInfo );
		vecDelBlock.push_back( kInfo );

		// 차단 리스트에서는 삭제
		m_mapTradeBlock.erase( iBlockTarget );
	}
}

void KBlockListManager::UpdateAccountBlock( IN const std::vector< KBlockInfo >& vecBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock )
{
	// 개인거래 블럭은 실시간으로 블럭을 풀어줘야하는 목적으로 수정된 항목에 대해서 리스트를 따로 유지하자.

	std::set< UidType > setUpdateBlock;	// DB로부터 받아온 블럭 대상 리스트
	std::set< UidType > setDelBlock;	// 제재 해제가 될 블럭 대상 리스트

	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecBlockInfo )
	{
		const UidType iBlockUserUID = kInfo.m_iBlockTarget;

		// DB로 부터 받아온 데이터들을 따로 리스트로 담자
		setUpdateBlock.insert( iBlockUserUID );

		std::map< UidType, KBlockData >::iterator mit;
		mit = m_mapAccountBlock.find( iBlockUserUID );
		if( mit == m_mapAccountBlock.end() )
		{
			m_mapAccountBlock.insert( std::make_pair( iBlockUserUID, KBlockData( kInfo ) ) );

			vecNewBlock.push_back( kInfo );
		}
		else
		{
			// 새로운 내용으로 업데이트
			mit->second.SetBlockInfo( kInfo );
		}
	}

	std::map< UidType, KBlockData >::const_iterator mit;
	for( mit = m_mapAccountBlock.begin(); mit != m_mapAccountBlock.end(); ++mit )
	{
		if( setUpdateBlock.find( mit->first ) != setUpdateBlock.end() )
			continue;

		setDelBlock.insert( mit->first );
	}

	BOOST_TEST_FOREACH( const UidType, iBlockTarget, setDelBlock )
	{
		std::map< UidType, KBlockData >::iterator mit;
		mit = m_mapAccountBlock.find( iBlockTarget );
		if( mit == m_mapAccountBlock.end() )
			continue;

		// 블럭 정보 얻고
		KBlockInfo kInfo;
		mit->second.GetBlockInfo( kInfo );
		vecDelBlock.push_back( kInfo );

		// 차단 리스트에서는 삭제
		m_mapAccountBlock.erase( iBlockTarget );
	}



	m_mapAccountBlock.clear();

	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecBlockInfo )
	{
		m_mapAccountBlock.insert( std::make_pair( kInfo.m_iBlockTarget, KBlockData( kInfo ) ) );
	}
}

void KBlockListManager::UpdateIpBlock( IN const std::vector< KBlockInfo >& vecBlockInfo )
{
	m_mapIpBlock.clear();

	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecBlockInfo )
	{
		const std::string strIP = KncUtil::toNarrowString( kInfo.m_wstrBlockTarget );
		const u_int uiIP = ::inet_addr( strIP.c_str() );

		m_mapIpBlock.insert( std::make_pair( uiIP, KBlockData( kInfo ) ) );
	}
}

bool KBlockListManager::IsTradeBlockUser( IN const UidType iUserUID ) const
{
	std::map< UidType, KBlockData >::const_iterator mit;
	mit = m_mapTradeBlock.find( iUserUID );
	if( mit == m_mapTradeBlock.end() )
	{
		return false;
	}

	return mit->second.IsActiveBlock();
}

bool KBlockListManager::IsAccountBlockUser( IN const UidType iUserUID ) const
{
	std::map< UidType, KBlockData >::const_iterator mit;
	mit = m_mapAccountBlock.find( iUserUID );
	if( mit == m_mapAccountBlock.end() )
	{
		return false;
	}

	return mit->second.IsActiveBlock();
}

bool KBlockListManager::IsIpBlockUser( IN const std::wstring& wstrIP ) const
{
	const std::string strIP = KncUtil::toNarrowString( wstrIP );
	const u_int uiIP = ::inet_addr( strIP.c_str() );

	std::map< u_int, KBlockData >::const_iterator mit;
	mit = m_mapIpBlock.find( uiIP );
	if( mit == m_mapIpBlock.end() )
	{
		return false;
	}

	return mit->second.IsActiveBlock();
}

bool KBlockListManager::AddModuleInfo( IN const KBlockInfo& kInfo )
{
	if( m_mapDllBlock.find( kInfo.m_wstrBlockTarget ) != m_mapDllBlock.end() )
	{
		START_LOG( cwarn, L"이미 등록되어있는 모듈UID입니다." )
			<< BUILD_LOG( kInfo.m_wstrBlockTarget )
			<< END_LOG;
		return false;
	}

	m_mapDllBlock.insert( std::make_pair( kInfo.m_wstrBlockTarget, KBlockData( kInfo ) ) );
	return true;
}

void KBlockListManager::UpdateModuleInfo( IN const std::wstring& wstrModuleName )
{
	std::map< std::wstring, KBlockData >::iterator mit;
	mit = m_mapDllBlock.find( wstrModuleName );
	if( mit == m_mapDllBlock.end() )
	{
		KBlockInfo kInfo;
		kInfo.m_cBlockType = KBlockInfo::BT_DLL_BLOCK;
		kInfo.m_wstrBlockTarget = wstrModuleName;
		kInfo.m_wstrStartDate = L"2000-01-01 00:00:00";
		kInfo.m_wstrEndDate = L"2050-01-01 00:00:00";
		AddModuleInfo( kInfo );
	}
	//else
	//{
	//	++mit->second.m_iCount;
	//	mit->second.m_bReserveDBUpdate = true;
	//}
}

void KBlockListManager::UpdateModuleList( IN const std::vector< KBlockInfo >& vecModuleList )
{
	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecModuleList )
	{
		UpdateModuleInfo( kInfo.m_wstrBlockTarget );
	}
}

void KBlockListManager::UpdateModuleList( IN const std::vector< std::wstring >& vecModuleList )
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrModuleName, vecModuleList )
	{
		UpdateModuleInfo( wstrModuleName );
	}
}

bool KBlockListManager::CheckHackingModuleList( IN const std::vector< std::wstring >& vecModuleList, OUT std::map< std::wstring, KModuleInfo >& mapModuleName )
{
	bool bCheckResult = true;

	std::map< std::wstring, KBlockData >::iterator mit;

	BOOST_TEST_FOREACH( const std::wstring&, wstrName, vecModuleList )
	{
		mit = m_mapDllBlock.find( wstrName );	
		if( mit != m_mapDllBlock.end() )
		{
			START_LOG( clog, L"Allow 값이 Black List 입니다." )
				<< BUILD_LOG( wstrName )
				<< END_LOG;

			bCheckResult = false;

			KModuleInfo kInfo;
			kInfo.m_iModuleUID = static_cast<int>(mit->second.GetBlockUID());
			kInfo.m_wstrModuleName = mit->second.GetBlockTargetString();
			kInfo.m_iCount;
			kInfo.m_iFlag;
			//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
			kInfo.m_iAllow;
#else
			kInfo.m_bAllow;
#endif SERV_HACKING_DLL_CHECK
			//}}
			kInfo.m_bReserveDBUpdate;
			mapModuleName.insert( std::make_pair( kInfo.m_wstrModuleName, kInfo ) );
		}
	}

	return bCheckResult;
}

#endif SERV_BLOCK_LIST
//}}


