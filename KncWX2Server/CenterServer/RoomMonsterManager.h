#pragma once
#include "Event.h"
#include "CommonPacket.h"
//{{ 2011. 11. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleField.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//{{ 2011. 01. 20	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW


class KRoomMonsterManager
{
public:

	//{{ dmlee 2008.11.13 PVP monster, 원래 dungeonroom.h에 있던 것 여기로 옮김
	//////////////////////////////////////////////////////////////////////////
	//생성된 npc data
	//
	//드롭조건::
	//1: 일반아이템 / exp :	m_bNoDrop == false
	//2: 이벤트아이템 :		m_bNoDrop == false && m_bActive == true
	struct NPC_DATA
	{
		UidType	m_uiAttUnit;	//공격한 user unit(죽을때 값을 받는다.)
		int		m_iNPCID;
		char	m_cLevel;
		bool	m_bNoDrop;
		bool	m_bActive;
		bool	m_bIsBoss;
		//{{ 2009. 12. 18  최육사	던전경험치개편
		char	m_cMonsterGrade;
		//}}
		//{{ 2011. 11. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		int		m_iGroupID;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		int		m_iBossGroupID;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}

		NPC_DATA()
		{
			m_uiAttUnit		= 0;
			m_iNPCID		= 0;
			m_cLevel		= 0;
			m_bNoDrop		= true;
			m_bActive		= false;
			m_bIsBoss		= false;
			//{{ 2009. 12. 18  최육사	던전경험치개편
			m_cMonsterGrade = 0;
			//}}
			//{{ 2011. 11. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
			m_iGroupID		= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			m_iBossGroupID  = 0;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
		}
	};
	//}} dmlee 2008.11.13 PVP monster, 원래 dungeonroom.h에 있던 것 여기로 옮김

public:
    KRoomMonsterManager();
    virtual ~KRoomMonsterManager();

	void	StartGame();
	void	EndGame();
	void	OnCloseRoom();

	// 체크 함수
	bool	GetNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const;
	bool	IsExistMonster( IN const int iNpcUID ) const;
	bool	IsExistMonster( IN const int iNpcUID, OUT bool& bIsDie ) const;
	bool	IsMonsterAlive( IN const int iNpcUID ) const	{ return ( m_mapAliveNpcList.find( iNpcUID ) != m_mapAliveNpcList.end() ); }
	//bool	IsMonsterDie( IN const int iNpcUID ) const		{ return ( m_mapDieNpcList.find( iNpcUID ) != m_mapDieNpcList.end() ); }
	//{{ 2013. 04. 16	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool	IsAllBossMonstersDead() const;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	int		GetAliveMonsterCount() const					{ return static_cast<int>(m_mapAliveNpcList.size()); }
	int		GetDieMonsterCount() const						{ return static_cast<int>(m_mapDieNpcList.size()); }
	void	GetAliveMonsterCountInfo( OUT CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterInfo ) const;
	bool	GetMonsterGroupID( IN const int iNpcUID, OUT int& iGroupID ) const;

	// 생성
	bool	CreateMonster( IN const int iNpcUID, IN const KNPCUnitReq& kNpcInfo );
	bool	CreateMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID, IN const int iNpcUID = 0 );

	// 생사 처리
	virtual bool SetMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID, IN const bool bInsertDieList = true );
	bool	SetMonsterRebirth( IN const int iNpcUID );
	
	//////////////////////////////////////////////////////////////////////////
	void PrintLuaLog_LUA( int iType, double fValue );	
	//////////////////////////////////////////////////////////////////////////
protected:
	int							m_nNPCUID;				// NPC Unit UID 발급
	std::map< int, NPC_DATA >	m_mapAliveNpcList;		// 현재 생성된 몬스터	[key:NpcUID,	 value:NPC_DATA]
	std::map< int, NPC_DATA >	m_mapDieNpcList;		// 죽은 몬스터 리스트	[key:NpcUID,	 value:NPC_DATA]

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
protected:
	void InsertReservedNpcUID( int iNpcUID ) { m_setReservedNpcUID.insert( iNpcUID ); }
	void DeleteReservedNpcUID( int iNpcUID ) { m_setReservedNpcUID.erase( iNpcUID ); }
	int GetNewNpcUID();

private:
	std::set< int > m_setReservedNpcUID;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
};


#endif SERV_DUNGEON_RANK_NEW
//}}


