#pragma once
#include "UserServerBuff.h"
#include "X2Data/XSLRoom.h"


//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM


class KUserServerBuffManager
{
public:
	KUserServerBuffManager(void);
	~KUserServerBuffManager(void);

	void Clear();
	void Init( IN const KUnitBuffInfo& kUnitBuffInfo );

	// 버프 활성화 & 비활성화
	bool ActivateBuff( IN KBuffInfo& kBuff, OUT std::vector< KBuffInfo >& vecActivatedBuff, IN const CTime tUseTime = 0, IN int iPeriod = 0 );
	bool DeactivateBuff( IN const int iBuffID, IN OUT std::vector< int >& vecDeleteBuff );	

	// 검사
	bool IsBuffActivated( IN const int iBuffID ) const	{ return ( m_mapBuff.find( iBuffID ) != m_mapBuff.end() ); }

	//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool GetActivateBuff( IN const int iBuffID, OUT KBuffInfo& kBuff ) const;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	
	// 버프 리스트 얻기
	void GetBuffList( OUT std::vector< KBuffInfo >& vecBuffInfo, IN const int iRoomType = CXSLRoom::RT_NONE ) const;
	void GetBuffIDList( OUT std::vector< int >& vecBuffID, IN const int iRoomType = CXSLRoom::RT_NONE ) const;
	void GetBuffFactorList( OUT std::vector< KBuffFactor >& vecBuffFactorID ) const;

	// 버프 효과 얻기
	void GetBuffBonusRate( IN OUT KRoomUserInfo& kRoomUserInfo, IN const int iRoomType = CXSLRoom::RT_NONE );
	
	// 조건 체크
	void CheckBuffPeriod( OUT std::vector< int >& vecDeactivatedBuff, IN const int& iRoomType = CXSLRoom::RT_NONE );
	void OnLeaveField( IN KGSUserPtr spUser, OUT std::vector< int >& vecDeactivatedBuff );
	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	void OnLeaveDungeonRoom( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivateBuffList, IN bool bBadAttitudeUser );
#else
	void OnLeaveDungeonRoom( IN KGSUserPtr spUser, std::vector< int >& vecDeactivateBuffList );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	void OnLeaveBattleField( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivatedBuff );

#ifdef SERV_FIX_REVENGE_BUFF// 작업날짜: 2013-08-09	// 박세훈
	void OnLeavePVPRoom( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivatedBuff );
#endif // SERV_FIX_REVENGE_BUFF

	void OnEndDungeonGame( IN KGSUserPtr spUser, IN const bool bDungeonClearSuccess, IN const bool bStartedByAutoParty, OUT std::vector< int >& vecDeactivatedBuff );
	void OnUserDied( IN KGSUserPtr spUser, std::vector< int >& vecDeactivatedBuff );

	// db update
	void GetDBUpdateInfo( OUT std::vector< KRecordBuffInfo >& vecRecordBuffInfo ) const;
	
private:
	std::map< int, KBuffInfo >			m_mapBuff;					// <버프 아이디, 버프정보>
	__int64								m_iInsertTime;				// 최초 입력된 버프 시간
};


#endif SERV_SERVER_BUFF_SYSTEM
//}}


