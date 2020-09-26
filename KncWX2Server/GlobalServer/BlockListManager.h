#pragma once

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <boost/timer.hpp>
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "BlockData.h"


//{{ 2013. 05. 14	최육사	아이템 개편
#ifdef SERV_BLOCK_LIST


class KBlockListManager
{
	DeclareRefreshSingleton( KBlockListManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	KBlockListManager(void);
	virtual ~KBlockListManager(void);

	void	Init();
	void	Tick();

	void	UpdateblockList( IN const int iReleaseTick, IN const std::map< int, std::vector< KBlockInfo > >& mapBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock );
	void	UpdateTradeBlock( IN const std::vector< KBlockInfo >& vecBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock );
	void	UpdateAccountBlock( IN const std::vector< KBlockInfo >& vecBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock );
	void	UpdateIpBlock( IN const std::vector< KBlockInfo >& vecBlockInfo );

	// for dll
	bool	AddModuleInfo( IN const KBlockInfo& kInfo );
	void	UpdateModuleInfo( IN const std::wstring& wstrModuleName );
	void	UpdateModuleList( IN const std::vector< KBlockInfo >& vecModuleList );
	void	UpdateModuleList( IN const std::vector< std::wstring >& vecModuleList );
	bool	CheckHackingModuleList( IN const std::vector< std::wstring >& vecModuleList, OUT std::map< std::wstring, KModuleInfo >& mapModuleName );

	bool	IsTradeBlockUser( IN const UidType iUserUID ) const;
	bool	IsAccountBlockUser( IN const UidType iUserUID ) const;
	bool	IsIpBlockUser( IN const std::wstring& wstrIP ) const;

private:
	template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );

	int		GetReleaseTick() const						{ return m_iReleaseTick; }
	void	SetReleaseTick( IN const int iReleaseTick )	{ m_iReleaseTick = iReleaseTick; }

private:
	boost::timer							m_tTimer;
	int										m_iReleaseTick;

	// 각 블럭타입에 맞게 별도의 리스트로 관리하자. ( 성능 )
	std::map< UidType, KBlockData >			m_mapTradeBlock;
	std::map< UidType, KBlockData >			m_mapAccountBlock;
	std::map< u_int, KBlockData >			m_mapIpBlock;
	std::map< std::wstring, KBlockData >	m_mapDllBlock;
};

DefRefreshSingletonInline( KBlockListManager );

template < class T >
void KBlockListManager::SendToAccountDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_ACCOUNT_DB, 0, NULL, usEventID, data );
}


#endif SERV_BLOCK_LIST
//}}