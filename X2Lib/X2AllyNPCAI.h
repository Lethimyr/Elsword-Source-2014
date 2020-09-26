#pragma once

class CX2AllyNPCAI : public CX2NPCAI
{
public: 
	struct AllyData
	{
		float	allyLostRange;			// 이 거리 밖으로 벗어나면 무조건 ally를 쫒아간다.
		float	allyFarLostRange;		// 이 거리 밖으로 벗어나면 무조건 ally 바로 옆으로 순간 이동한다.

		AllyData()
		{
			allyLostRange		= 1000.f;
			allyFarLostRange	= 2000.f;
		}
	};
	
	struct TargetData
	{
		TARGET_PRIORITY		targetPriority;
		float 				targetInterval;		// 초 단위 
		float				targetRange;		// 이 범위 안에 들어오면 타게팅 된다
		float				targetNearRange;	// 이 범위 안에 들어오면 무조건 타게팅 된다				즉시, 타겟으로
		float				targetLostRange;	// 이 범위 밖으로 벗어나면 현재 타겟을 잃게 된다.		즉시, 타겟을 잃는다.
		int					targetSuccessRate;	// [0,100]
		int					targetAttakerRate;	// [0,100] 나를 공격한 유닛을 타겟팅할 확률				맞는 즉시, 타겟으로
		int					targetPreserveRate; // [0,100] 현재 타겟을 유지할 확률, 현재 타겟이 있을 경우에만
#ifdef EVOKE_TARGETING_BUG_FIX
		std::string			strLuaTargetingFunc;	/// 메뉴얼 타겟팅 함수명
#endif EVOKE_TARGETING_BUG_FIX
#ifdef ADD_NPC_CONDITION_TABLE
		bool				bTargetOnlyOurTeam;
#endif // ADD_NPC_CONDITION_TABLE

		TargetData()
		{
			targetPriority		= CX2NPCAI::TP_RANDOM;
			targetInterval		= 0.f;
			targetRange			= 0.f;
			targetNearRange		= 0.f;
			targetLostRange		= 0.f;
			targetSuccessRate	= 0;
			targetAttakerRate	= 0;
			targetPreserveRate	= 0;
#ifdef EVOKE_TARGETING_BUG_FIX
			strLuaTargetingFunc = "";
#endif EVOKE_TARGETING_BUG_FIX
#ifdef ADD_NPC_CONDITION_TABLE
			bTargetOnlyOurTeam = false;
#endif // ADD_NPC_CONDITION_TABLE
		}
	};

	struct ChaseMoveData
	{
		float		destGap;				// 목적지에서 이거리 안에 있으면 도착했다고 판단한다
		float 		moveGap;				// 목적지로 부터 이 거리보다 멀리 떨어져 있으면 다시 쫒아가기 시작한다.

		float		dirChangeInterval;		// 방향을 바꿀 시간 간격

		float		dashRange;				// 이 범위 안에 있으면 뛰고, 밖에 있으면 걷는다.
		bool		bReverseWalkDash;		// true이면 가까이 있을 때 걷고, 멀리 있을 때 뛴다.
		float		walkDashInterval;		// walk, dash 상태를 바꿀 시간 간격

		bool		bStayOnCurrentLineGroup;

#ifdef ADD_NPC_CONDITION_TABLE
		bool		bIfCannotFindMoveStateDoWait; // 이동 스테이트 찾지 못하면 대기 하도록 하는 설정
#endif // ADD_NPC_CONDITION_TABLE

		ChaseMoveData()
		{
			destGap		= 0.f;
			moveGap		= 0.f;
			dashRange	= 0.f;
			
			bReverseWalkDash		= false;
			dirChangeInterval		= 0.f;
			walkDashInterval		= 0;
			bStayOnCurrentLineGroup	= false;
#ifdef ADD_NPC_CONDITION_TABLE
			bIfCannotFindMoveStateDoWait = false; // 이동 스테이트 찾지 못하면 대기 하도록 하는 설정
#endif // ADD_NPC_CONDITION_TABLE
		}
	};


	struct RandomJumpData
	{
		bool		bEnabled;
		float		jumpInterval;		// 초
		int			jumpUpRate;			// [0,100]
		int			jumpDownRate;		// [0,100]

		RandomJumpData()
		{
			bEnabled		= false;
			jumpInterval	= 0.f;
			jumpUpRate		= 0;
			jumpDownRate	= 0;
		}
	};



	//struct PatrolData
	//{
	//	bool	m_bEnabled;
	//	float	m_fPatrolRange;
	//	float	m_fPatrolTime;			// patrol을 지속하는 시간
	//	float	m_fPatrolCoolTime;		// patrol을 멈추고 쉬는 시간
	//	bool	m_bStayOnLineGroup;
	//};

public:
	CX2AllyNPCAI( CX2GUNPC* pMasterNPC );
	virtual ~CX2AllyNPCAI(void);

	virtual void OnFrameMove( double fTime, float fElapsedTime );
	virtual void LoadAIDataFromLUA( KLuaManager& luaManager );


public:
	bool RallyToAlly();
	void Targeting( float fElapsedTime );
	void Moving( float fElapsedTime );

	void TargetUpdate();
	void SetMasterUnitData();

#ifdef EVOKE_TARGETING_BUG_FIX			/// 메뉴얼 타겟팅 추가
	bool GetEnableLuaTargetingFunc() const { return m_bEnableLuaTargetingFunc; }
	void SetEnableLuaTargetingFunc(bool val) { m_bEnableLuaTargetingFunc = val; }
	const std::string& GetLuaTargetingFunc() const { return m_strLuaTargetingFunc; }
	void SetLuaTargetingFunc( const std::string& val) { m_strLuaTargetingFunc = val; }
#endif EVOKE_TARGETING_BUG_FIX

public:
	UidType GetAllyUnitUID() const { return m_AllyUnitUID; }
	void SetAllyUnitUID( const UidType uidAllyGameUnit_ ) { m_AllyUnitUID = uidAllyGameUnit_; }

	CX2GameUnit* GetAllyGameUnit() const { return ( null != m_optrAllyGameUnit ? m_optrAllyGameUnit.GetObservable() : NULL ); }
	void SetAllyGameUnit( CX2GameUnit* pAllyGameUnit_ )
	{ 
		if ( NULL != pAllyGameUnit_ )
		{
			m_optrAllyGameUnit = pAllyGameUnit_; 
			SetAllyUnitUID( m_optrAllyGameUnit->GetUnitUID() );
			SetAllyUnitType( m_optrAllyGameUnit->GetGameUnitType() );
		}
	}

	CX2GameUnit::GAME_UNIT_TYPE GetAllyUnitType() const { return m_eAllyUnitType; }
	void SetAllyUnitType(const CX2GameUnit::GAME_UNIT_TYPE val) { m_eAllyUnitType = val; }

private:
	CX2GameUnitoPtr				m_optrAllyGameUnit;
	UidType						m_AllyUnitUID;	
	CX2GameUnit::GAME_UNIT_TYPE	m_eAllyUnitType;


	AllyData		m_AllyData;
	TargetData		m_TargetData;
	ChaseMoveData	m_ChaseMoveData;
	RandomJumpData	m_RandomJumpData;
	//PatrolData	m_PatrolData;


private:
	int				m_iFinalDestLineIndex;				// targetupdate()에서 계산된 최종 목적지의 linedata index
	D3DXVECTOR3		m_vFinalDest;						// 최종 목적지


	float			m_fMaxJumpUp;
	float			m_fMaxJumpRight;	

	//bool			m_bIsTargetFront;

#ifdef EVOKE_TARGETING_BUG_FIX							/// 메뉴얼 타겟팅 추가
	bool			m_bEnableLuaTargetingFunc;
	string			m_strLuaTargetingFunc;
#endif EVOKE_TARGETING_BUG_FIX

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    std::vector<CX2GameUnit*>   m_vecpTempGameUnit;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
};
