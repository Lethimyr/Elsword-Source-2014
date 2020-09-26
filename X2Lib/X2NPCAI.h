#pragma once


// 핵심목표는 추후에 기능 수정, 추가가 용이하게 flexible하고 stable한 AI 코드만들기

// 몬스터 속도 벡터를 따로 sync를 맞추자
// 몬스터의 감성!! 대사!! 움직임!! 코믹!! 
// 몬스터가 하는 행동, 공격, 방어, 도망, ???

// REINFORCEMENT RULE



#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#include "./X2GameUnit.h"
#include "./X2GUNPC.h"
#endif REDUCED_PRECOMPILED_HEADER_TEST



class CX2NPCAI
{
public:
	enum NPC_AI_TYPE
	{
		NAT_NORMAL,			// 
		NAT_ALLY,			// 소환된 친구 NPC
		NAT_NO_BRAIN,
		NAT_FIELD,			/// 필드 타입
	};

	// 새로운 타겟을 정할 때 TARGET_RANGE 내에 있는 유저 중에서 우선 순위
	enum TARGET_PRIORITY
	{
		TP_RANDOM,				/// 50% 확률로 아무나 처음 걸리는 유저
		TP_ONLY_ONE,			/// 처음 걸리는 유저 	
		TP_HIGH_LEVEL_FIRST,	/// 레벨이 가장 높은 유저
		TP_LOW_LEVEL_FIRST,		/// 레벨이 가장 낮은 유저
		TP_HIGH_HP_FIRST,		/// HP가 가장 높은 유저
		TP_LOW_HP_FIRST,		/// HP가 가장 낮은 유저
		TP_NEAR_FIRST,			/// 가장 가까운 유저
		TP_FAR_FIRST,			/// 가장 멀리 떨어져 
	};


private:
	CX2NPCAI() {};

public: 
	CX2NPCAI( CX2GUNPC* pOwnerNPC, NPC_AI_TYPE eAIType );	
	virtual ~CX2NPCAI();

	virtual void OnFrameMove( double fTime, float fElapsedTime ) = NULL;
	virtual void LoadAIDataFromLUA( KLuaManager& luaManager );


public:
	bool GetEnabled() const { return m_bEnabled; }
	void SetEnabled(bool val) 
	{ 
		m_bEnabled = val; 

		if ( false == m_bEnabled )
			ResetTarget();
	}

	void ResetTarget() { m_optrTargetGameUnit.Reset(); }
	CX2GameUnit* GetTargetGameUnit() { return ( null != m_optrTargetGameUnit ? m_optrTargetGameUnit.GetObservable() : NULL ); }
	void SetTargetGameUnit( CX2GameUnit* pTargetGameUnit_ ) 
	{ 
		if ( NULL != pTargetGameUnit_ )
			m_optrTargetGameUnit = pTargetGameUnit_; 
	}
	//UidType GetTargetUnitUID() const { return m_TargetUnitUID; }
	//void SetTargetUnitUID(UidType val) { m_TargetUnitUID = val; }

	//CX2GameUnit::GAME_UNIT_TYPE GetTargetUnitType() const { return m_TargetUnitType; }
	//void SetTargetUnitType(CX2GameUnit::GAME_UNIT_TYPE val) { m_TargetUnitType = val; }

	CX2NPCAI::NPC_AI_TYPE GetAIType() const { return m_eAIType; }
	void SetAIType(CX2NPCAI::NPC_AI_TYPE val) { m_eAIType = val; }

public:
	CX2GUNPC::STATE_CHANGE_TYPE GetPrevAIMessage() const { return m_PrevAIMessage; }
	void SetPrevAIMessage(CX2GUNPC::STATE_CHANGE_TYPE val) { m_PrevAIMessage = val; }

	bool GetFleeing() const { return m_bFleeing; }
	void SetFleeing(bool val) { m_bFleeing = val; }


	void ResetAttackerGameUnit() { m_optrAttackerGameUnit.Reset(); }
	CX2GameUnit* GetAttackerGameUnit() { return ( null != m_optrAttackerGameUnit ? m_optrAttackerGameUnit.GetObservable() : NULL ); }
	void SetAttackerGameUnit( CX2GameUnit* pAttackerGameUnit_ ) 
	{ 
		if ( NULL != pAttackerGameUnit_ )
			m_optrAttackerGameUnit = pAttackerGameUnit_; 
	}
	
	bool IsTargetFront() const { return m_bIsTargetFront; }
	bool IsTargetAccquired() 
	{ 
		if( null != m_optrTargetGameUnit ) 
			return true; 
		else 
			return false; 
	}

	virtual void RenderStateParam( WCHAR** wDest_ );
	virtual void RenderPatrolLineIndex( WCHAR** wDest_ );
	virtual void RenderPlayLineIndex( WCHAR** wDest_ );

#ifdef AI_FLY
	void SetAIFly(bool val) 
	{ 
		m_bFlyUnit = val;
		m_bNowStateFly = val;
	}
	bool GetAIFly() { return m_bFlyUnit; }
	void SetAiFlyNowState(bool val) { m_bNowStateFly = val; }
	bool GetAIFlyNowState() { return m_bNowStateFly; }	
#endif

	bool GetNoBrain() const { return m_bNoBrain; }
	void SetNoBrain( bool bNoBrain ) { m_bNoBrain = bNoBrain; }

	int GetMasterNpcUid() const { return m_pMasterNPC->GetUID(); }

// 	float GetIncreaseMoveSpeedPercentAtComeBackState() const { return m_fIncreaseMoveSpeedPercentAtComeBackState; }
// 	void SetIncreaseMoveSpeedPercentAtComeBackState( const float fVal_ ) { m_fIncreaseMoveSpeedPercentAtComeBackState = fVal_; }
// 
// 	float GetIncreaseAnimSpeedPercentAtComeBackState() const { return m_fIncreaseAnimSpeedPercentAtComeBackState; }
// 	void SetIncreaseAnimSpeedPercentAtComeBackState( const float fVal_ ) { m_fIncreaseAnimSpeedPercentAtComeBackState = fVal_; }

protected:
	CX2SimplePathFinder		m_PathFinder; // (M)

	bool					m_bNoBrain; /// (F) AI 가 작동하지 않도록 스크립트에서 설정한다 (헬드랍, 박쥐, 스톤월, 아틸러사용 중)
	bool					m_bEnabled;	/// (F) 스턴, 인탱글, 드라이어드, 라이드, callNearOneNpc 치트 등 NPC를 움직이지 못하게 할 때 사용
	CX2GUNPC*				m_pMasterNPC;	/// (T, M, F)
	

protected:
	NPC_AI_TYPE				m_eAIType;
		
	CX2GameUnitoPtr					m_optrTargetGameUnit;	/// 현재 타겟 중인 GameUnit;
	//UidType							m_TargetUnitUID;	/// (T,M)현재 타겟팅 할 유닛의 UID
	//CX2GameUnit::GAME_UNIT_TYPE 	m_TargetUnitType;	/// (T)현재 타겟팅 할 유닛의 타입

protected:

	CX2GameUnitoPtr					m_optrAttackerGameUnit;		/// 자신을 공격한 GameUnit;

	CX2GUNPC::STATE_CHANGE_TYPE		m_PrevAIMessage;	/// (M) 이전의 AI Message (SCT_AI_DASH 등)

	bool			m_bIsTargetFront;		/// (M)타겟을 바라보고 서 있는지
	bool			m_bFleeing;				/// (T,M)현재 도망 중인지 나타내는 변수 true면, 무조건 TargetUID가 있어야함, Target을 잃으면 m_bFleeing도 false 가 됨


	float 			m_fElapsedTimeAfterLastTargeting;		/// (T,F) targetInterval 시간을 지나간 이후의 경과시간

	float			m_fElapsedTimeAfterLastRandomJump;		/// (M) 랜덤하게 아래, 위로 점프하는 것을 컨트롤 하는 변수 (점프 뿐만 아니라 다운도..)
	float			m_fElapsedTimeAfterLastWalkRunUpdate;	/// (M) walk, dash 중에 walk, dash로의 상태변화는 CHASE_MOVE의 WALK_INTERVAL 간격으로 갱신
	float			m_fElapsedTimeAfterLastDirChange;	/// (M)
	float			m_fElapsedPatrolCoolingTime;	/// (M)패트롤 쿨타임이지만... 패트롤 거리를 벗어낫거나, 한 라인그룹에서만 패트롤해야하는 경우에 그룹의 가장자리에 있게될때만 영향 받음

// 	float			m_fIncreaseMoveSpeedPercentAtComeBackState;
// 	float			m_fIncreaseAnimSpeedPercentAtComeBackState;

#ifdef AI_FLY
	float			m_fElapsedTimeAfterLastFlyMoveUpdate;	/// (M)
	bool			m_bFlyUnit;
	bool			m_bNowStateFly;
#endif

};

