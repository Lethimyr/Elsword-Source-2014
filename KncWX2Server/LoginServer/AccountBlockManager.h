#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <set>
#include <vector>
#include <map>
#include <boost/timer.hpp>
#include "KncSend.h"


//////////////////////////////////////////////////////////////////////////
// Account Block Manager : 계정 블럭 관리
#ifdef SERV_ACCOUNT_BLOCK


class KAccountBlockManager
{
	DeclareSingleton( KAccountBlockManager );
	DeclDump;
	DeclToStringW;

	enum ENUM_TIMER
	{		
		TM_RELEASE_TICK = 0,

		TM_TOTAL_NUM,
	};

	enum BLOCK_TYPE
	{
		BT_CONNECT_BLOCK = 0,
		BT_TRADE_BLOCK,
		BT_IP_LOGIN_BLOCK,	// 해외팀에서 쓰고있음
		BT_IP_TRADE_BLOCK,	// 해외팀에서 쓰고있음
		BT_CHAT_BLOCK,		// 해외팀에서 쓰고있음
		//{{ 2011. 08. 09	최육사	머신ID 블럭기능
//#ifdef SERV_MACHINE_ID_BLOCK
		BT_MACHINE_ID_BLOCK,// 머신ID 블럭
//#endif SERV_MACHINE_ID_BLOCK
		//}}
		//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
//#ifdef SERV_HACKING_DLL_CHECK
		BT_MODULE_LIST,		// DLL 리스트 내용 변경
//#endif SERV_HACKING_DLL_CHECK
		//}}

		BT_MAX,
	};

public:
	KAccountBlockManager(void);
	~KAccountBlockManager(void);

	void Tick();

	void Init();
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	void UpdateAccountBlockList( IN const KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK& kPacket, 
								OUT std::map< int, std::vector< UidType > >& mapBlockUserList,
								OUT std::map< int, std::vector< UidType > >& mapUnblockUserList,
								OUT std::vector< std::string >& vecBlockMachineIDList );
#else
	void UpdateAccountBlockList( IN const KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK& kPacket, 
								OUT std::map< int, std::vector< UidType > >& mapBlockUserList,
								OUT std::map< int, std::vector< UidType > >& mapUnblockUserList );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}	
	
	void AddBlockedAccount( IN const UidType iUserUID, IN const BLOCK_TYPE eBlockType );
	void GetReleaseTick( OUT std::map< int, int >& mapReleaseTick );

	// Block Check Function
	bool IsBlockedAccount( IN const UidType iUserUID, IN const BLOCK_TYPE eBlockType ) const;
	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
	bool IsBlockedMachineID( IN const std::string& strMachineID ) const	{ return ( m_setMachineIDBlockList.find( strMachineID ) != m_setMachineIDBlockList.end() ); }
#endif SERV_MACHINE_ID_BLOCK
	//}}

protected:
	template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );
	void SendToAccountDB( unsigned short usEventID );

private:	
	std::set< UidType >			m_setAccountBlockList[BT_MAX];
	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
	std::set< std::string >		m_setMachineIDBlockList;
#endif SERV_MACHINE_ID_BLOCK
	//}}
	boost::timer				m_kTimer[TM_TOTAL_NUM];
	int							m_iReleaseTick[BT_MAX];
};

DefSingletonInline( KAccountBlockManager );

template < class T >
void KAccountBlockManager::SendToAccountDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_ACCOUNT_DB, 0, NULL, usEventID, data );
}


#endif SERV_ACCOUNT_BLOCK


