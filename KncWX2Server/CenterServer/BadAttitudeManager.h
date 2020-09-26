#pragma once


#include "Event.h"
#include "CommonPacket.h"
#include "RoomUserManager.h"
#include "X2Data/XSLDungeon.h"

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

class KBadAttitudeManager
{

public:

	struct KSubStageRankInfo 
	{
		int					m_iStage;
		int					m_iSubStage;
		char				m_cRank;

		KSubStageRankInfo()
		{
			m_iStage		= -1;
			m_iSubStage		= -1;
			m_cRank			= CXSLDungeon::RT_NONE;
		}
	};

	struct KBadAttitudeInfo 
	{
		UidType				m_iUnitUID;				// 대상 유저
		KSubStageRankInfo	m_kRankInfo;			// 서브 스테이지 정보
		int					m_iBadAttitudePoint;	// 불량점수
		int					m_iVotePoint;			// 득표수
		std::set<UidType>	m_setVotedUnitUID;		// 나를 투표한 유저목록
		bool				m_bIsBadAttitudeUnit;	// 불량 유저 인가?
		bool				m_bForceExit;			// 강퇴 되어야 하는가?

		KBadAttitudeInfo( UidType iUnitUID )
		{
			m_iUnitUID					= iUnitUID;
			m_iBadAttitudePoint			= 0;
			m_iVotePoint				= 0;
			m_setVotedUnitUID.clear();
			m_bIsBadAttitudeUnit		= false;
			m_bForceExit				= false;
		}
	};
	
	

	KBadAttitudeManager(void);
	~KBadAttitudeManager(void);

	// 초기화
	void init( IN KRoomUserManagerPtr m_spRoomUserManager, IN int iDungeonIDAndDiff );
	// 던전 플레이 도중 나간 유저 삭제
	bool RemoveUnit( IN UidType iUnitUID, OUT KBadAttitudeInfo& kUnitData );
	// 불량 점수 1점 추가
	void IncreaseBadAttitudeOnePoint( IN UidType iUnitUID );
	// 강퇴 투표하기
	void IncreaseVoteOnePoint( IN UidType iBadAttitudeUnitUID, IN UidType iVoteUnitUID );
	// 불량 점수 와 전체 유저 수, 득표수를 비교하여 
	void CheckBadAttitudeUnit( OUT std::vector<UidType>& vecNewBadAttitudeUnit, OUT std::vector<UidType>& vecNewForceExitUnit );
	// 불량 유저가 되었다면 다른 불량 유저에게 강제로 투표하게함
	void BadAttitudeForceVote( IN UidType iBadAttitudeUnitUID );
	// 서브 스테이지의 정보를 다 받았는가?
	bool IsAllUnitGetScore();
	// 최신 서버 스테이지의 정보를 셋팅하자
	void SetUnitSubStageInfo( IN UidType iUnitUID, IN int iStage, IN int iSubStage, IN int iDungeonIDDiff, IN int iEndNumMember, IN int iCombo, IN int iTech );
	// 몬스터 정보
	void IncreaseSubStageMonsterDieCount( IN int iStageID, IN int iSubStageID, IN const char cMonsterTypeFactor, IN bool bIsDefenceDungeon = false );
	// 디펜스
	void SetDefenceDungeonWaveID( IN int iWaveID );
	// 정보 얻기
	bool GetUnitData( IN UidType iUnitUID, OUT KBadAttitudeInfo& kUnitData );

private:
	std::map<UidType, KBadAttitudeInfo>		m_mapUnitInfoList;
	CXSLDungeon::DUNGEON_TYPE				m_eDungeonType;
	std::map< std::pair< int, int >, int >	m_mapSubStageMonsterScore;   //Key(1:stage,2:substage)
	int										m_iWaveID;
};

#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}