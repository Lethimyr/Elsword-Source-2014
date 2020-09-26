#pragma once


class CX2GUNPCAI : public CX2NPCAI
{
public:
	struct TargetData
	{
		bool				targetNPC;			/// NPC를 타겟을로 잡음
		TARGET_PRIORITY		targetPriority;
		float 				targetInterval;
		int					targetHeightLimit;	/// (배틀필드에서만 사용 중)높이가 이 범위 내에 있어야 타겟 가능 (기본100)
		int					targetNearRange;	/// 이 범위 안에 들어오면 무조건 타겟팅 된다
		int					targetRange;		/// 이 범위 안에 들어오면 타겟팅 가능
		int					targetLostRange;	/// 이 범위 밖으로 벗어나면 현재 타겟을 잃게 된다.
		float				targetSuccessRate;	
		float				attackTargetRate;	/// 나를 공격한 유닛을 타겟팅할 확률
		float				preserveLastTargetRate;	/// 이전에 타겟한 대상을 유지할 확률 (다른 유저가 공격을 했거나, 도망가고있다거나, 가까운유닛을 찾은경우 제외)
		string				strLuaTargetingFunc;	/// Targeting 을 수행할 루아 함수를 지정할 수 있음 // X2GUNPCAI의 m_strLuaTargetingFunc에 셋팅됨

		TargetData() : targetNPC( false ), targetPriority( CX2NPCAI::TP_RANDOM ), 
			targetInterval( 0 ), targetHeightLimit( 1000 ), targetNearRange( 0 ), targetRange( 0 ),
			targetLostRange( 0 ), targetSuccessRate( 0.0f ), attackTargetRate( 0.0f ), 
			preserveLastTargetRate( 0.0f ), strLuaTargetingFunc()
		{
		}
	};

	struct ChaseMoveData
	{
		int			moveSplitRange;		/// 현재위치와 목적한 위치 까지의 거리가 이 기준에 따라 near 인지 far 인지가 결정됨
		int			destGap;			/// 목적지에서 이거리 안에 있으면 도착했다고 판단한다
		int			moveGap;

		float		dirChangeInterval;	/// CHASE_MOVE의 DIR_CHANGE_INTERVAL로 AI Message에 의해 방향을 바꾸는 시간 간격

		float		walkInterval;
		float		nearWalkRate;		/// 현재위치와 목적한 위치 까지의 거리가moveSplitRange 보다 작은 경우 WALK할 확률
		float		farWalkRate;		/// 현재위치와 목적한 위치 까지의 거리가moveSplitRange 보다 큰 경우 WALK할 확률

		float		jumpInterval;
		float		upJumpRate;
		float		upDownRate;
		float		downJumpRate;
		float		downDownRate;

		float		m_fLineEndDetectRange;	/// 라인 끝에 도착했는지 체크할 때 사용되는 범위
		bool		bOnlyThisLineGroup;
	};

	struct PatrolMoveData
	{
		float		patrolBeginRate;		/// 타겟을 잃었을 때 patrol을 할 확률
		int			patrolRange;
		float		patrolCoolingTime;
		bool		bOnlyThisLineGroup;		/// 지금의 라인그룹만 패트롤 하는지 여부
	};

	struct EscapeMoveData
	{
		int			moveSplitRange;		/// 사용안함
		int			escapeGap;			/// 이 거리 보다 멀면 도망 성공, 도망을 그만둔다

		float		walkInterval;		/// 사용안함
		float		nearWalkRate;		/// 사용안함
		float		farWalkRate;		/// 사용안함

		float		jumpInterval;
		float		upJumpRate;		/// 목적지가 위에 있는데 점프 할 확률
		float		upDownRate;		/// 목적지가 위에 있는데 아래 라인맵으로 갈 확률..
		float		downJumpRate;	/// 목적지가 아래에 있는데 점프 할 확률..
		float		downDownRate;	/// 목적지가 아래에 있는데 아래 라인맵으로 갈 확률..

		float		m_fLineEndDetectRange;	/// 사용안함
	};

	struct EscapeCondition
	{ 
		// escapeRate, myHP, escapeRange 중 한가지라도 설정되어 있으면 설정되지 않은것은 100, 10000 등으로 설정됨
		// LoadAIDataFromLUA() 참조
		float		escapeRate;		/// 해당 확률로 도망
		int			myHP;			/// Percentage로 NPC 자신의 HP가 이것 적으면 도망
		float		escapeRange;
		// 09.04.16 태완 : 도망 조건에 Flag Check 추가
		int			m_iFlagTrue;	/// 셋팅 되어 있지 않으면 도망, 셋팅되어 있으면 GUNPC의 Flag의 Index로 사용하여 Flag 값으로 판단
		
		EscapeCondition()
		{
			escapeRate		= 0;
			myHP			= 0;
			escapeRange 	= 0.f;			
			m_iFlagTrue		= -1;
		}
	};

#ifdef AI_FLY
	struct FlyMoveData
	{
		float	fDestGap;
		float	fDestHeight;	/// DEGT_HEIGHT_GAP 으로 타겟과 유지할 높이
		float	fDestArea;
		float	fFlyMoveInterval;	/// FLY_MOVE_INTERVAL 로 이동 AI의 변경 Inverval 타임 (interval에 만족하지 못하면 이전의 이동을 사용)
		bool	bUnderWaterMode;
	};
#endif

	struct AIData
	{
		TargetData		targetData;
		ChaseMoveData	chaseMoveData;
		PatrolMoveData	patrolMoveData;
		EscapeMoveData	escapeMoveData;
		EscapeCondition	escapeCondition;
#ifdef AI_FLY
		FlyMoveData		flymovedata;
#endif
	};


	
public:
	CX2GUNPCAI( CX2GUNPC* pMasterNPC );
	virtual ~CX2GUNPCAI(void);

	virtual void OnFrameMove( double fTime, float fElapsedTime );
	virtual void LoadAIDataFromLUA( KLuaManager& luaManager );

	void TargetingNPC( float fElapsedTime );
	virtual void Targeting( float fElapsedTime );
	virtual void Moving( float fElapsedTime );

	void SetRallyToUnit( CX2GameUnit* pRallyGameUnit_ ); 	

	const D3DXVECTOR3& GetFinalDest() const { return m_vFinalDest; }
	void SetFinalDestPos( const D3DXVECTOR3& vDestPos_ ) { m_vFinalDest = vDestPos_; }

	void SetAITargetNPC( bool bTargetNPC ) { m_pAIData->targetData.targetNPC = bTargetNPC; }
	bool GetEnableLuaTargetingFunc() const { return m_bEnableLuaTargetingFunc; }
	void SetEnableLuaTargetingFunc(bool val) { m_bEnableLuaTargetingFunc = val; }
	const string& GetLuaTargetingFunc() const { return m_strLuaTargetingFunc; }
	void SetLuaTargetingFunc( const std::string& val) { m_strLuaTargetingFunc = val; }
	
	bool EscapeFlagCheck( int iFlagIndex );		//09.04.16 태완

	//{{ megagame / 박교현 / 2010.04.30 / NPC 스탯 추가
	float GetScanCloaking() const { return m_fScanCloaking; }
	float GetScanNearRange() const { return m_fScanCloakingNearRange; }
	//}} megagame / 박교현 / 2010.04.30 / NPC 스탯 추가
	void SetScanCloaking(float val) { m_fScanCloaking = val; }
	void SetScanNearRange(float val) { m_fScanCloakingNearRange = val; }

#ifdef AI_FLY
	float GetFlyDestHieght() { return m_pAIData->flymovedata.fDestHeight; }
#endif


	//{{ oasis : 김상윤 // 2009-10-7 ////////////////
	// AI Data 바꾸는 함수
	void SetAItoMainAIData();
	void SetAItoSubAIData();
	//}} oasis : 김상윤 // 2009-10-7 ////////////////

#ifdef RIDING_MONSTER
	void SetRideToUnit( CX2GameUnit* pRallyGameUnit_ );
	void StopRideToUnit();
#endif RIDING_MONSTER

	int GetFinalDestLineIndex() const { return m_iFinalDestLineIndex; }
	AIData* GetAIData() const { return m_pAIData; }
	int GetLastTouchLineIndex() const { return m_iLastTouchLineIndex; }
	bool GetFootOnLine() const { return m_bFootOnLine; }
	const D3DXVECTOR3& GetNowPosition() const { return m_vPosition; }
	float GetMaxJumpUp() const { return m_fMaxJumpUp; }
	float GetMaxJumpRight() const { return m_fMaxJumpRight; }

protected:

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    void TargetUpdate( bool bAccumulate_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void SetMasterUnitData();

	void SetFinalDestLineIndex( const int iFinalDestLineIndex_ ) { m_iFinalDestLineIndex = iFinalDestLineIndex_; }

	bool GetNowPatroling() const { return m_bNowPatroling; }
	void SetNowPatroling( bool bNowPatroling_ ) { m_bNowPatroling = bNowPatroling_; }
	
	bool IsRight() const { return m_bIsRight; }	

	bool GetLostTarget() const { return m_bLostTarget; }
	void SetLostTarget( const bool bLostTarget_ ) { m_bLostTarget = bLostTarget_; }


private:
	//{{ oasis : 김상윤 // 2009-10-7 ////////////////
	// 원본: AIData    m_AIData;
	AIData*			m_pAIData;	
	
	AIData*			m_pMainAIData;
	AIData*			m_pSubAIData;
	//}} oasis : 김상윤 // 2009-10-7 ////////////////
	int				m_iFinalDestLineIndex;			/// (T,M) 최종 목적지의 linedata index
	D3DXVECTOR3		m_vFinalDest;					// (T,M) 최종 목적지


	bool			m_bLostTarget;	/// (T,M)
	D3DXVECTOR3		m_vPivotPoint;	/// (T,M)			// 타겟이 없는 경우에는, 이 점을 중심으로 patrol하거나 guard, wander (보통 이 점은 타겟이 없을 때의 현재 위치)
	int				m_iPivotPointLineIndex;	/// 사용안함




	// MasterNPC에 대한 정보
	D3DXVECTOR3		m_vPosition;	///(T,) AI를 소유한 NPC의 위치
	int				m_iLastTouchLineIndex;	///(M) AI를 소유한 NPC가 마지막까지 있었던 LineMapIndex
	bool			m_bIsRight;		///(M)
	bool			m_bFootOnLine;	///(M)
	float			m_fMaxJumpUp;	///(M)
	float			m_fMaxJumpRight;	///(M)
	float			m_fNowHP;	///(T)
	float			m_fMaxHP;	///(T)	

	bool			m_bNowPatroling;			// (T,M) 현재 patrol 중인지 나타내는 변수

	//bool			m_bRallyToUnit;			/// (T,M) 특정 유닛이 있는 곳 까지 가야하는지의 여부

	CX2GameUnitoPtr	m_optrRallyGameUnit;	/// 특정 유닛 ( 2011.10.27 현재는 Ride나 Call monster 등에서 사용됨)
	//UidType			m_RallyUnitUID;			/// (T,M) 특정 유닛의 UID ( 2011.10.27 현재는 Ride나 Call monster 등에서 사용됨)
	//UidType			m_RallyUnitType;		/// (T)특정 유닛의 타입

#ifdef RIDING_MONSTER
	bool			m_bRideToUnit;			/// (T,M)다른 유닛을 ride
#endif RIDING_MONSTER

	bool			m_bEnableLuaTargetingFunc;
	string			m_strLuaTargetingFunc;			/// 타켓팅 Function으로 지정해줄 루아 함수 명

	float			m_fScanCloakingNearRange;		/// (T) 투명유닛이 이 거리내에 있으면 탐지 가능
	float			m_fScanCloaking;				/// (T) 투명유닛을 탐지할 확률
	bool			m_bScanCloaking;				/// (T) 투명유닛을 탐지할 수 있는지 여부

#ifdef SEASON3_MONSTER_2010_12
	bool			m_bTargetedOnlyOne;		/// 필요없을듯...(T) 하나의 타겟이 이미 선택되었는지 확인
#endif

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    std::vector<CX2GameUnit*>   m_vecpTempGameUnit;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
};


