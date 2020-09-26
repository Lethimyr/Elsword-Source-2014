
#pragma once

class CNpcCreatedInfo;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CNpcCreatedInfo> CNpcCreatedInfoPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CNpcCreatedInfo> CNpcCreatedInfoPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CNpcCreatedInfo
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	float			m_fDistance3Sq;	// 유저와의 거리

#ifdef	BATTLE_FIELD_TEST
	UINT			m_uiGroupId;		// NPC의 Group ID
#endif	BATTLE_FIELD_TEST

#ifdef  SERV_BATTLEFIELD_MIDDLE_BOSS
	UINT			m_uiBossGroupId;	// NPC 의 Boss Group ID
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS


	UINT			m_uiNpcId;			// NPC의 ID
	UINT			m_uiNpcUid;			// NPC의 UID
	UidType			m_uidAllyUid;		// AllyNpc인 경우 그 Npc의 Master User UID;

	float			m_fUnitScale;
	UINT			m_uiStartPosNum;
	vector<int>		m_vecPetrolLineIndex;
	vector<int>		m_vecPlayLineIndex;
	UCHAR			m_ucTeamNum;		// 팀 구분
	bool			m_bAggressive;		// 선공 몬스터 인가?
	UCHAR			m_ucLevel;			// 레벨
	bool			m_bActive;	
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	D3DXVECTOR3		m_vPos;
	bool			m_bIsRight;
	float			m_fDelayTime;
	char			m_KeyCode;
	char			m_cAIType;
	bool			m_bFocusCamera;
	KAttribEnchantNpcInfo
					m_kAttribEnchantNpcInfo;
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

#ifdef FIX_FIELD_SUMMON_MONSTER
	char			m_cCreateMonsterType;
#endif // FIX_FIELD_SUMMON_MONSTER
#ifdef FIELD_BOSS_RAID // 중보 분류
	char			m_cMonsterGrade;
#endif // FIELD_BOSS_RAID


	CNpcCreatedInfo( const float fDistance3Sq_, const KNPCUnitReq& kNpcUnitReq_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		, const KAttribEnchantNpcInfo* pkAttribEnchantNpcInfo = NULL
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		) : m_fDistance3Sq( fDistance3Sq_ )
#ifdef	BATTLE_FIELD_TEST
		, m_uiGroupId( kNpcUnitReq_.m_iGroupID )
#endif	BATTLE_FIELD_TEST


// #ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
#ifdef  SERV_BATTLEFIELD_MIDDLE_BOSS
		, m_uiBossGroupId ( kNpcUnitReq_.m_iBossGroupID )
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS		
// #endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		, m_uiNpcId( kNpcUnitReq_.m_NPCID ), m_uiNpcUid( kNpcUnitReq_.m_UID ),
		m_uidAllyUid( kNpcUnitReq_.m_iAllyUID ), m_fUnitScale( -1.0f ), m_uiStartPosNum( kNpcUnitReq_.m_nStartPos),
		m_ucTeamNum( kNpcUnitReq_.m_cTeamNum ), m_bAggressive( kNpcUnitReq_.m_bAggressive ),
		m_ucLevel( kNpcUnitReq_.m_Level ), m_bActive( kNpcUnitReq_.m_bActive )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{
		m_vecPetrolLineIndex	= kNpcUnitReq_.m_vecPetrolLineIndex;
		m_vecPlayLineIndex		= kNpcUnitReq_.m_vecPlayLineIndex;
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		m_vPos.x = kNpcUnitReq_.m_vPos.x;
		m_vPos.y = kNpcUnitReq_.m_vPos.y;
		m_vPos.z = kNpcUnitReq_.m_vPos.z;
		m_bIsRight = kNpcUnitReq_.m_bIsRight;
		m_fDelayTime = kNpcUnitReq_.m_fDelayTime;
		m_KeyCode = kNpcUnitReq_.m_KeyCode;
		m_cAIType = kNpcUnitReq_.m_cAIType;
		m_bFocusCamera = kNpcUnitReq_.m_bFocusCamera;
		if ( pkAttribEnchantNpcInfo )
			m_kAttribEnchantNpcInfo = *pkAttribEnchantNpcInfo;
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

#ifdef FIX_FIELD_SUMMON_MONSTER		// 해외팀 추가
		m_cCreateMonsterType = kNpcUnitReq_.m_cCreateMonsterType;
#endif FIX_FIELD_SUMMON_MONSTER

#ifdef FIELD_BOSS_RAID // 중보 분류
		m_cMonsterGrade	= kNpcUnitReq_.m_cMonsterGrade;
#endif // FIELD_BOSS_RAID
	}

public:
	static CNpcCreatedInfoPtr CreateNpcCreatedInfoPtr( const float fDistance3Sq_, const KNPCUnitReq& kNpcUnitReq_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		, const KAttribEnchantNpcInfo* pkAttribEnchantNpcInfo = NULL
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		)

	{
		return CNpcCreatedInfoPtr( new CNpcCreatedInfo( fDistance3Sq_, kNpcUnitReq_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
			, pkAttribEnchantNpcInfo
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
			) );
	}

	bool operator > ( const CNpcCreatedInfo& rhs_ ) const {

		if ( m_fDistance3Sq > rhs_.m_fDistance3Sq )
			return true;
		else
			return false;
	}
	bool operator < ( const CNpcCreatedInfo& rhs_ ) const {

		if ( m_fDistance3Sq < rhs_.m_fDistance3Sq )
			return true;
		else
			return false;
	}

#ifdef	BATTLE_FIELD_TEST
	UINT GetGroupId() const { return m_uiGroupId; }
	void SetGroupId( const UINT val_ ) { m_uiGroupId = val_; }
#endif	BATTLE_FIELD_TEST

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	UINT GetBossGroupId() const { return m_uiBossGroupId; }	
	void SetBossGroupId(UINT val) { m_uiBossGroupId = val; }
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS


#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	UINT		GetNpcId() const { return m_uiNpcId; }
	UINT		GetNpcUid() const { return m_uiNpcUid; }
	UidType		GetAllyUid() const { return m_uidAllyUid; }
	float		GetUnitScale() const { return m_fUnitScale; }
	UINT		GetStartPosNum() const { return m_uiStartPosNum; }
	UCHAR		GetTeamNum() const { return m_ucTeamNum; }
	bool		GetAggressive() const { return m_bAggressive; }
	UCHAR		GetLevel() const { return m_ucLevel; }
	bool		GetActive() const { return m_bActive; }
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	const D3DXVECTOR3& GetPos() const { return m_vPos; }
	bool		GetIsRight() const { return m_bIsRight; }
	float		GetDelayTime() const { return m_fDelayTime; }
	char		GetKeyCode() const { return m_KeyCode; }
	char		GetAIType() const { return m_cAIType; }
	bool		GetFocusCamera() const { return m_bFocusCamera; }
	void		SetFocusCamera( bool bFocusCamera_ ) { m_bFocusCamera = bFocusCamera_; }
	void		SetAllyUid( UidType uidAllyUID_ )	{ m_uidAllyUid = uidAllyUID_; }
	const KAttribEnchantNpcInfo&	GetAttribEnchantNpcInfo() const { return m_kAttribEnchantNpcInfo; }
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	const vector<int>& GetPetrolLineIndexList() { return m_vecPetrolLineIndex; }
	const vector<int>& GetPlayLineIndexList() { return m_vecPlayLineIndex; }
#ifdef FIX_FIELD_SUMMON_MONSTER		// 해외팀 추가
	char		GetCreateMonsterType() const { return m_cCreateMonsterType; }
#endif FIX_FIELD_SUMMON_MONSTER

#ifdef FIELD_BOSS_RAID // 중보 분류
	char		GetMonsterGrade() const { return m_cMonsterGrade; }
#endif // FIELD_BOSS_RAID

};

IMPLEMENT_INTRUSIVE_PTR(CNpcCreatedInfo);


class CX2BattleFieldNpcAi : public CX2GUNPCAI 
{
public:
	CX2BattleFieldNpcAi( CX2GUNPC* pMasterNpc_, const CNpcCreatedInfoPtr NpcCreatedInfoPtr_ );
	virtual ~CX2BattleFieldNpcAi();

	bool IsAggressive() const { return m_bAggressive; }
	void SetAggressive( bool bAggressive_ ) { m_bAggressive = bAggressive_; }

	bool GetCanTargeting() const { return m_bCanTargeting; }
	void SetCanTargeting( const bool bCanTargeting_ ) { m_bCanTargeting = bCanTargeting_; }

	bool GetCanPatrol() const { return m_bCanPatrol; }
	void SetCanPatrol( bool bCanPatrol_ ) { m_bCanPatrol = bCanPatrol_; }

	virtual void OnFrameMove( double fTime, float fElapsedTime );
	virtual void Targeting( float fElapsedTime );
	virtual void Moving( float fElapsedTime );

	virtual void RenderStateParam( WCHAR** wDest_ );
	virtual void RenderPatrolLineIndex( WCHAR** wDest_ );
	virtual void RenderPlayLineIndex( WCHAR** wDest_ );

	bool GetBackState01() const { return m_bBackState01; }
	void SetBackState01( bool val ) { m_bBackState01 = val; }

	bool GetBackState02() const { return m_bBackState02; }
	void SetBackState02( bool val ) { m_bBackState02 = val; }

	void SetFinalDestinationPosAndLineMapIndex( const D3DXVECTOR3& vDestPos_, const int iDestLineMapIndex_ )
	{
		SetFinalDestPos( vDestPos_ );
		SetFinalDestLineIndex( iDestLineMapIndex_ );
	}

	virtual void LoadAIDataFromLUA( KLuaManager& luaManager );

protected:
	bool Patroling();
	bool CheckAndResetTargetUnitLost();
	bool TargetingGameUnitWhichAttackMe();
	bool CanPreserveLastTargetUnit();
	void GetTargetListInNearRangeOrRange();
	bool CheckAndResetTargetLost();
	void SearchingTargetUnit( IN vector<CX2GameUnit*>& vecTargetCandidateUnitList_ );
	bool IsHigherNewTargetGameUnitLevel( const CX2GameUnit* pOldTargetGameUnit_, const CX2GameUnit* pNewTargetGameUnit_ );
	bool IsHigherNewTargetGameUnitHp( const CX2GameUnit* pOldTargetGameUnit_, const CX2GameUnit* pNewTargetGameUnit_ );
	bool IsNearNewTargetGameUnit( IN const CX2GameUnit* pOldTargetGameUnit_, IN const CX2GameUnit* pNewGameUnit_, IN OUT float& fDistanceSqFromOldTargetUnit_ );
	bool IsFarNewTargetGameUnit( IN const CX2GameUnit* pOldTargetGameUnit_, IN const CX2GameUnit* pNewGameUnit_, IN OUT float& fDistanceSqFromOldTargetUnit_ );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    bool GetCanScanCloaking( const float fDistance3Sq_, bool bAccumulate ) const
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	bool GetCanScanCloaking( const float fDistance3Sq_ ) const
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( fDistance3Sq_ <= GetScanNearRange() * GetScanNearRange() && 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            ( bAccumulate == true && m_pMasterNPC->EstimateFrameAccumPercent( GetScanCloaking() ) == true
            || bAccumulate == false && CX2GUNPC::EstimateFrameOneshotPercent( GetScanCloaking() ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			 RandomInt() < GetScanCloaking()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
             )
			 return true;
		else
			return false;
	}

	bool CheckOnPatrolLineMap();
	bool CheckOnPlayLineMap();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    void GetAIMessageAndUnitDirection( float fElapsedTime_, OUT CX2GUNPC::STATE_CHANGE_TYPE& eSctMessage_, OUT bool& bAiIsRight_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void GetAIMessageAndUnitDirection( OUT CX2GUNPC::STATE_CHANGE_TYPE& eSctMessage_, OUT bool& bAiIsRight_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	void CheckArrivedDestination();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    void UpNowHpBySecondProcess( float fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void UpNowHpBySecondProcess();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	float GetIncreaseHpPercentBySecond() const { return m_fIncreaseHpPercentBySecond; }
	void SetIncreaseHpPercentBySecond( const float fIncreaseHpPercentBySecond_ ) { m_fIncreaseHpPercentBySecond = fIncreaseHpPercentBySecond_; }

private:

	bool m_bAggressive;		/// true면 선공 몬스터, false면 비선공 몬스터
	bool m_bCanTargeting;	/// 현재 타겟팅을 수행 할 수 있는지 여부

	bool m_bCanPatrol;		/// 패트롤을 할 수 있는 유닛

	bool m_bBackState01;	/// 백스테이트 01 실행 중 (테스트)
	bool m_bBackState02;	/// 백스테이트 02 실행 중 (테스트)
	bool m_bArriveAtDest;	/// 목적지에 도착 함
	
	std::vector<int>	m_vecPatrolLineMapIndex;
	std::vector<int>	m_vecPlayLineMapIndex;

	std::vector<CX2GameUnit*> m_vecTargetGameUnitListInRange;		/// 특정 거리내의 타겟팅 대상들을 리스트화 (사용 후 바로 clear)
	std::vector<CX2GameUnit*> m_vecTargetGameUnitListInNearRange;	/// 무조건 타겟팅 하는 거리 내의 대상들을 리스트화 ( 사용 후 바로 clear)

	CKTDXCheckElapsedTime		m_ElapsedTimeCheckOnLineMap;				/// 라인맵 체크 경과시간
	CKTDXCheckElapsedTime		m_ElapsedTimeAfterSuccessToPatrol;			/// 패트롤 완료 후 경과 시간 (다시 패트롤 하는데에 사용, 패트롤 쿨타임)
	//CKTDXCheckElapsedTime		m_ElapsedTimeUpNowHp;						/// HP가 충전되는 시간 간격
	CKTDXCheckElapsedTime		m_ElapsedTimeComeBackState;					/// 컴백스테이트가 발동된 이후 경과 시간
	float						m_fIncreaseHpPercentBySecond;				/// 초당 증가 하는 HP의 Percent (ex: 0.01f은 1%)
	
	int							m_iCheckLineMapAtFirst;

	static const float			BASIC_INCREASE_HP_PERCENT_BY_SECOND;//						= 0.02f;
	static const float			OUT_OF_PATROL_LINE_INCREASE_HP_PERCENT_BY_SECOND;//		= 0.05f;
	static const float			OUT_OF_PLAY_LINE_INCREASE_HP_PERCENT_BY_SECOND;//			= 0.2f;

#ifdef	BATTLE_FIELD_TEST
	UINT			m_uiGroupId;		// NPC의 Group ID
#endif	BATTLE_FIELD_TEST
};
