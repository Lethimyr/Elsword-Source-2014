#pragma once


#ifdef NEW_SKILL_TREE
/** @class	CX2UserSkillTree
	@brief	유저의 스킬 슬롯 관련, B 슬롯 상태 확인, 던전 플레이 시 스킬 레벨 증가 아이템 적용 등
	@date	2011/01/06(주석 추가)
*/
class CX2UserSkillTree
{
public:
	/// 스킬 슬롯 인덱스
	enum SKILL_SLOT_INDEX
	{
		SKILL_SLOT_A1 = 0,		/// A슬롯 1, Key = A
		SKILL_SLOT_A2,			/// A슬롯 2, Key = S
		SKILL_SLOT_A3,			/// A슬롯 3, Key = D
		SKILL_SLOT_A4,			/// A슬롯 4, Key = C

		SKILL_SLOT_B1,			/// B슬롯 1, Key = A
		SKILL_SLOT_B2,			/// B슬롯 2, Key = S
		SKILL_SLOT_B3,			/// B슬롯 3, Key = D
		SKILL_SLOT_B4,			/// B슬롯 4, Key = C

		MAX_SKILL_SLOT,			/// 최대 슬롯
	};

	/// 슬롯 B 만료 상태 확인
	enum SKILL_SLOT_B_EXPIRATION_STATE
	{
		SSBES_EXPIRED,		/// 슬롯 B 기능 만료
		SSBES_NOT_EXPIRED,	/// 슬롯 B 기능 사용 가능
		SSBES_PERMANENT,	/// 슬롯 B 영구 사용
	};

	/** @struct	UserSkillData
		@brief	유저 스킬 정보
	*/
	struct UserSkillData
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		KProtectedType<int> m_iSkillLevel;					/// 스킬 레벨
		KProtectedType<int> m_iSkillCSPoint;				/// 스킬 포인트
		KProtectedType<int> m_iIncreaseSkillLevelByBuff;	/// 버프를 통해 증가된 스킬 레벨
		KProtectedType<int> m_iIncreaseSkillLevelBySocket;	/// 소켓을 통해 증가된 스킬 레벨

		UserSkillData()				/// 생성자
			: m_iSkillLevel( 0 )
			, m_iSkillCSPoint( 0 )
			, m_iIncreaseSkillLevelByBuff(0 )
			, m_iIncreaseSkillLevelBySocket(0 )
		{
		}

		/// 생성자
		UserSkillData( int iLevel, int iSkillCSPoint )
		{
			m_iSkillLevel					= iLevel;
			m_iSkillCSPoint					= iSkillCSPoint;
			m_iIncreaseSkillLevelByBuff		= 0;
			m_iIncreaseSkillLevelBySocket	= 0;
		}

		/// 스킬 레벨 증가값 총합 반환 함수 ( 소켓 + 버프 )
		int  GetIncreaseSkillLevel() const { return m_iIncreaseSkillLevelBySocket + m_iIncreaseSkillLevelByBuff; }
		
		/// 스킬 레벨 총합 반환 함수 ( 스킬 포인트 + 소켓 + 버프 )
		int  GetTotalSkillLevel() const { return m_iSkillLevel + m_iIncreaseSkillLevelBySocket + m_iIncreaseSkillLevelByBuff; }

		void SetIncreaseSkillLevelByBuff( int val ) { m_iIncreaseSkillLevelByBuff = val; }
		void SetIncreaseSkillLevelBySocket( int val ) { m_iIncreaseSkillLevelBySocket = val; }
	#else // UPGRADE_SKILL_SYSTEM_2013
		int m_iSkillLevel;			/// 스킬 레벨
		int m_iSkillCSPoint;		/// 스킬 포인트

		UserSkillData()				/// 생성자
			: m_iSkillLevel( 0 )
			, m_iSkillCSPoint( 0 )
		{
		}

		/// 생성자
		UserSkillData( int iLevel, int iSkillCSPoint )
		{
			m_iSkillLevel = iLevel;
			m_iSkillCSPoint = iSkillCSPoint;
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	};

	/** @class	SkillSlotData
		@brief	스킬 슬롯 정보
	*/
	struct SkillSlotData
	{
#ifdef VERIFY_STAT_BY_BUFF
		KProtectedType<CX2SkillTree::SKILL_ID>	m_eID;				/// 스킬 아이디
		KProtectedType<int>		m_iSkillLevel;		/// 스킬 레벨
		KProtectedType<int>		m_iSkillLevelPlus;	/// 아이템등에 의한 스킬 레벨 증가 효과
		KProtectedType<float>	m_fCoolTimeLeft;	/// 쿨타임
		KProtectedType<float>	m_fMPConsumption;	/// 마나 소모량
//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		KProtectedType<USHORT>	m_usCBConsumption;	/// 캐논볼 사용량
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		KProtectedType<USHORT>	m_usFPConsumption;	/// 기력 사용량
		KProtectedType<USHORT>	m_usFPGainCount;	/// 기력 획득량
#endif

#else	// VERIFY_STAT_BY_BUFF

		CX2SkillTree::SKILL_ID	m_eID;				/// 스킬 아이디
		int						m_iSkillLevel;		/// 스킬 레벨
		int						m_iSkillLevelPlus;	/// 아이템등에 의한 스킬 레벨 증가 효과
		float					m_fCoolTimeLeft;	/// 쿨타임
		float					m_fMPConsumption;	/// 마나 소모량
		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		USHORT					m_usCBConsumption;	/// 캐논볼 사용량
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		USHORT					m_usFPConsumption;	/// 기력 사용량
		USHORT					m_usFPGainCount;	/// 기력 획득량
#endif

#endif // VERIFY_STAT_BY_BUFF


		CX2SkillTree::SKILL_SLOT_ENCHANT m_eSlotEnchant;	/// 속성 상태

		SkillSlotData() :				/// 생성자
		m_eID( CX2SkillTree::SI_NONE ),
		m_iSkillLevel( 0 ),
		m_iSkillLevelPlus( 0 ),
		m_fCoolTimeLeft( 0.f ),
		m_fMPConsumption(0.f),
		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		m_usCBConsumption(0),	/// 캐논볼 사용량
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
#ifdef ARA_CHARACTER_BASE
		m_usFPConsumption(0),	/// 기력 사용량
		m_usFPGainCount(0),		/// 기력 획득량
#endif

		m_eSlotEnchant( CX2SkillTree::SSE_NONE )
		{
		}

		void Init()				/// 초기화 함수
		{
			m_eID = CX2SkillTree::SI_NONE;
			m_fCoolTimeLeft = 0.f;
			m_iSkillLevel = 0;
			//m_iSkillLevelPlus = 0;	// pvpgame에서 EGS_PLAY_START_NOT를 받으면 unitdata를 덮어쓰면서 Init()을 호출하기 때문에 그전에 계산된 m_iSkillLevelPlus와 m_fMPConsumption가 날아가버리는 문제 때문에
			//m_fMPConsumption = 0.f;	// pvpgame에서 EGS_PLAY_START_NOT를 받으면 unitdata를 덮어쓰면서 Init()을 호출하기 때문에 그전에 계산된 m_iSkillLevelPlus와 m_fMPConsumption가 날아가버리는 문제 때문에
		}

		int GetActualSkillLevel()		/// 스킬 레벨 계산(아이템에 의한 레벨 증가 고려)
		{
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if( CX2SkillTree::SI_ETC_WS_COMMON_LOVE == m_eID )
				return m_iSkillLevel;
			else
#endif //ADDED_RELATIONSHIP_SYSTEM
				return m_iSkillLevel + m_iSkillLevelPlus;
		}

#ifdef VERIFY_STAT_BY_BUFF
		bool Verify() const
		{
			if ( m_eID.Verify() == false
				|| m_iSkillLevel.Verify() == false
				|| m_iSkillLevelPlus.Verify() == false
				|| m_fCoolTimeLeft.Verify() == false
				|| m_fMPConsumption.Verify() == false
				|| m_usCBConsumption.Verify() == false
				|| m_usFPConsumption.Verify() == false
				|| m_usFPGainCount.Verify() == false
				)
				return false;
			else
				return true;
		}
#endif // VERIFY_STAT_BY_BUFF

	};
	
public:		/// public function
	CX2UserSkillTree(void);
	virtual ~CX2UserSkillTree(void);
	
	void OnFrameMove( double fTime, float fElapsedTime );
	void Reset( bool bResetSkillTree, bool bResetEquippedSkill, bool bResetUnsealedSkill, bool bResetSkillSlotB );

	void SetEquippedSkill( const int iSlotIndex_, const bool bSlotB_, const KSkillData& equippedSkillData_ );
	void SetEquippedSkill( const KSkillData aEquippedSkill[], const KSkillData aEquippedSkillSlotB[] );
	void SetEquippedSkillLevelPlusBySlotIndex( const int iEquippedSkillLevelPlus_, const int iSlotIndex_, const bool bIsSlotB_ );
	void SetEquippedSkillLevelPlus( const int aEquippedSkillLevelPlus[], const int aEquippedSkillLevelPlusSlotB[] );

	void ConstructSkillTreeNotMyUnit( const std::vector<KSkillData>& vecSkillData, const KSkillData aEquippedSkill[], const KSkillData aEquippedSkillSlotB[] );

#ifdef SKILL_PAGE_SYSTEM //JHKang
	void AddSkillPage();
	void SetAcquiredSkill( const std::vector<KUserSkillPageData>& vecSkillList );
	int GetSkillLevel( CX2SkillTree::SKILL_ID eSkillID, bool bBaslcLevel = false, USHORT usSkillPage_ = 0 ) const;
	USHORT GetSkillPoint( USHORT usPageNumber_ ) const { return m_arrSkillPoint[usPageNumber_ - 1]; }
	USHORT GetCashSkillPoint( USHORT usPageNumber_ ) const { return m_arrCashSkillPoint[usPageNumber_ - 1]; }
	void SetSkillPoint( const USHORT usSkillPoint_, const USHORT usSkillPage_ = 0 )
	{
		USHORT usPage = m_usUsingPage;

		if ( usSkillPage_ > 0 )
			usPage = usSkillPage_;

		m_arrSkillPoint[usPage] = usSkillPoint_;
	}
	void SetCashSkillPoint( const USHORT usSkillPoint_, const USHORT usSkillPage_ = 0 )
	{
		USHORT usPage = m_usUsingPage;

		if ( usSkillPage_ > 0 )
			usPage = usSkillPage_;

		m_arrCashSkillPoint[usPage] = usSkillPoint_;
	}
	void AddSkillPoint( const USHORT usSkillPoint_, const USHORT usSkillPage_ = 0 )
	{
		USHORT usPage = m_usUsingPage;

		if ( usSkillPage_ > 0 )
			usPage = usSkillPage_;

		m_arrSkillPoint[usPage] += usSkillPoint_;
	}
#else //SKILL_PAGE_SYSTEM
	void SetAcquiredSkill( const std::vector<KUserSkillData>& vecSkillList );
	int GetSkillLevel( CX2SkillTree::SKILL_ID eSkillID, bool bBaslcLevel = false ) const;
#endif //SKILL_PAGE_SYSTEM
	void SetUnsealedSkill( const std::vector<short>& vecUnsealedSkillList );
	
	//{{ oasis907 : 김상윤 //// 2009-11-18 ////   userskilltree.GetSkillLevel + skilltree.GetSkillTemplet 통합 함수
	const CX2SkillTree::SkillTemplet* GetUserSkillTemplet( CX2SkillTree::SKILL_ID eSkillID) const; 

	bool GetSkillLevelAndCSP( IN CX2SkillTree::SKILL_ID eSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint ) const;	// 획득한 스킬이 존재하는지 체크, 있다면 스킬레벨을 return, 없으면 -1 return
#ifdef SKILL_PAGE_SYSTEM //JHKang
	bool SetSkillLevelAndCSP( CX2SkillTree::SKILL_ID eSkillID, int iSkillLevel, int iSkillCSPoint, USHORT usSkillPage_ = 0 );				// 스킬이 존재하면 스킬레벨을 지정된 레벨로, 없으면 추가하고 스킬레벨을 지정된 레벨로	
#else //SKILL_PAGE_SYSTEM
	bool SetSkillLevelAndCSP( CX2SkillTree::SKILL_ID eSkillID, int iSkillLevel, int iSkillCSPoint );				// 스킬이 존재하면 스킬레벨을 지정된 레벨로, 없으면 추가하고 스킬레벨을 지정된 레벨로	
#endif //SKILL_PAGE_SYSTEM
	bool IsMaxSkillLevel( CX2SkillTree::SKILL_ID eSkillID ) const;														// 스킬을 최고레벨까지 배웠는지

	void SetSkillSlotInfo( int iSkillSlotID, CX2SkillTree::SKILL_ID eSkillID );

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	USHORT GetCBConsumption( const int iSlotIndex_, const bool bSlotB_ ) const;
	void SetCBConsumption( int iSlotIndex_, bool bSlotB_, USHORT usCBConsumption_ );
#ifdef ARA_CHARACTER_BASE
	bool SetSkillSlot( int iSlotIndex_, CX2SkillTree::SKILL_ID eSkillID_, float fMPConsume_, USHORT usCBConsume_,
					   USHORT usFPConsume, bool bSlotB_ );
#else
	bool SetSkillSlot( int iSlotIndex_, CX2SkillTree::SKILL_ID eSkillID_, float fMPConsume_, USHORT usCBConsume_, bool bSlotB_ );
#endif //ARA_CHARACTER_BASE
#else	NEW_CHARACTER_CHUNG
	bool SetSkillSlot( int iSlotIndex, CX2SkillTree::SKILL_ID eSkillID, float MPConsume, bool bSlotB );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	USHORT	GetFPConsumption( const int iSlotIndex_, const bool bSlotB_ ) const;
	void	SetFPConsumption( int iSlotIndex_, bool bSlotB_, USHORT usFPConsumption_ );

	USHORT	GetFPGainCount( const int iSlotIndex_, const bool bSlotB_ ) const;
	void	SetFPGainCount( int iSlotIndex_, bool bSlotB_, USHORT usFPConsumption_ );
#endif
	const CX2UserSkillTree::SkillSlotData* GetSkillSlot( int iSlotIndex, bool bSlotB ) const;
	const CX2UserSkillTree::SkillSlotData* GetSkillSlot( const CX2SkillTree::SKILL_ID eSkillID_ ) const;

	void ChangeMpConsumptionPercent( const CX2SkillTree::SKILL_ID eSkillID_, const float fMultiplier_ );
	void SetMpConsumption( int iSlotIndex_, bool bSlotB_, float fMpConsumption_ );

	// 09.05.30 태완
	int GetSkillEquippedSlotIndex( CX2SkillTree::SKILL_ID eSkillID, bool bSlotB ) const;		// 없으면 -1. 빈칸을 찾고 싶으면 스킬ID를 SI_NONE로 입력하면 된다.

	void SetSkillCoolTimeLeft( CX2SkillTree::SKILL_ID eSkillID, float fCoolTimeLeft );

//{{ kimhc // 2010.11.2 // 엠프레스 - 충전 추진기
#ifdef	NEW_SKILL_2010_11
	void ResetLeftSkillCoolTimeAll();

#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
	void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType, const float fCoolTime = 0.f, bool bForceSet = true, BUFF_CHANGE_TYPE eChangeType = BCT_RELATION_VALUE, const float fCoolTimeRate = 1.0f, const bool bIsRelativeAllSkill = false );

#else // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
	void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType, const float fCoolTime = 0.f, bool bForceSet = true, BUFF_CHANGE_TYPE eChangeType = BCT_RELATION_VALUE );

#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가

#else // UPGRADE_SKILL_SYSTEM_2013
	void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType );
#endif // UPGRADE_SKILL_SYSTEM_2013

#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.2 // 엠프레스 - 충전 추진기

	void InitSkillCoolTimeFromGageManager( const vector<float>& vecSkillCoolTime_ );

	CX2Stat::Stat GetSkillStat() const;

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-16 //// 
	bool IsGuildSkill( CX2SkillTree::SKILL_ID eSkillID ) const;

	//{{ oasis907 : 김상윤 //// 2009-11-18 ////   userskilltree.GetSkillLevel + skilltree.GetSkillTemplet 통합 함수 (길드용)
	const CX2SkillTree::SkillTemplet* GetUserSkillTemplet( CX2SkillTree::SKILL_ID eSkillID, int iGuildMemberGrade) const; 

	//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
	void Reset( bool bResetSkillTree, bool bResetGuildSkillTree, bool bResetEquippedSkill, bool bResetUnsealedSkill, bool bResetSkillSlotB );

	void ConstructGuildSkillTreeNotMyUnit( const std::vector<KSkillData>& vecGuildSkillData);

	//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
	void SetAcquiredGuildSkill( const std::vector<KGuildSkillData>& vecSkillList );

	//{{ oasis907 : 김상윤 //// 2009-11-18 //// 
	int GetSkillLevel( CX2SkillTree::SKILL_ID eSkillID, int iGuildMemberGrade) const;

	//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
	CX2Stat::Stat GetGuildSkillStat(int iGuildMemberGrade) const;

#endif GUILD_SKILL		
	int UpdateEquippedSkillLevelFromAcqureidMap( const CX2SkillTree::SKILL_ID eSkillID, const int iSlotIndex_, bool bSlotB = false );

	static void GetSlotIndexAndSlotB( IN const int iSkillSlotId_, OUT int& iSlotIndex_, OUT bool& bSlotB_ );	// 슬롯 ID를 통해 SlotIndex와 SlotB 인지 여부를 알수있는 함수

	CX2UserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE GetSkillSlotBExpirationState() const
	{
		return m_eSkillSlotBExpirationState; 
	}
	void SetSkillSlotBExpirationState(CX2UserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE val)
	{
		m_eSkillSlotBExpirationState = val; 
	}
	bool GetEnabledSkillSlotB() const;
	const std::wstring& GetSkillSlotBEndDateString() const { return m_wstrSkillSlotBEndDate; }
	void SetSkillSlotBEndDateString(std::wstring val) { m_wstrSkillSlotBEndDate = val; }

	void CalcUsedSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint ) const;
	void CalcCumulativeUsedSPointOnEachTier( OUT std::vector<int>& vecTierSPoint ) const;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환
	void ExpireCashSkillPoint( OUT const std::map<int, int> mapHaveSKill_ );
#else // UPGRADE_SKILL_SYSTEM_2013
	void ExpireCashSkillPoint();
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
	void CalcUsedGuildSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint ) const;
	void CalcCumulativeUsedGuildSPointOnEachTier( OUT std::vector<int>& vecTierSPoint ) const;
	void ExpireGuildCashSkillPoint(); // oasis907 : 김상윤 [2009.11.27] // 길드 캐시 스킬 포인트 기간 경과
#endif GUILD_SKILL

	bool IsSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID ) const;
	void AddSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID );

	bool DoIHaveThisSkill( CX2SkillTree::SKILL_ID eSkillID ) const;
	bool CanIResetThisSkill( CX2SkillTree::SKILL_ID eSkillID ) const;
	bool CanILearnThisSkill( CX2SkillTree::SKILL_ID eSkillID, bool bCheckSP = true ) const;
	bool CanIInitSkillTree() const;

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
	bool CanIInitGuildSkillTree() const;
#endif GUILD_SKILL

	int GetUnitClass() const { return m_iUnitClass; }
	void SetUnitClass( int val ) { m_iUnitClass = val; }

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
	int GetGuildClass() const { return m_iGuildClass; }
	void SetGuildClass( int val ) { m_iGuildClass = val; }
#endif GUILD_SKILL

#ifdef SERV_SKILL_NOTE
	void SetEqipSkillMemo( const std::vector<int>& vecSkillMemo) { m_vecSkillNote = vecSkillMemo; }
	bool GetEqipSkillMemo(int iSkillMemo) const;
#endif

	bool IsChangedSkillSlot( const int iSlotIndex_, const bool bSlotB_, const KSkillData& kSkillData_ ) const
	{
		if ( bSlotB_ )
		{
			// 슬롯의 스킬레벨과 ID가 같다면 변한것이 아님
			if ( m_aEquippedSkillSlotB[iSlotIndex_].m_iSkillLevel == kSkillData_.m_cSkillLevel && 
				 static_cast<int>( m_aEquippedSkillSlotB[iSlotIndex_].m_eID ) == kSkillData_.m_iSkillID )
				return false;
			else
				return true;			
		}
		else
		{
			// 슬롯의 스킬레벨과 ID가 같다면 변한것이 아님
			if ( m_aEquippedSkill[iSlotIndex_].m_iSkillLevel == kSkillData_.m_cSkillLevel && 
					 static_cast<int>( m_aEquippedSkill[iSlotIndex_].m_eID ) == kSkillData_.m_iSkillID )
				return false;
			else
				return true;
		}		
	}
	void ToggleSkillSlotAB()
	{
		SkillSlotData EquipSkillTemp[4];

		memcpy( EquipSkillTemp, m_aEquippedSkill, sizeof(m_aEquippedSkill) );
		memcpy( m_aEquippedSkill, m_aEquippedSkillSlotB, sizeof(m_aEquippedSkillSlotB) );
		memcpy( m_aEquippedSkillSlotB, EquipSkillTemp, sizeof(EquipSkillTemp) );
	}

#ifdef VERIFY_STAT_BY_BUFF
	bool VerifyEquippedSkillslot() const
	{
		for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT; i++ )
		{
			if ( m_aEquippedSkill[i].Verify() == false ||
				m_aEquippedSkillSlotB[i].Verify() == false )
				return false;
		}

		return true;
	}
#endif // VERIFY_STAT_BY_BUFF

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
#ifdef SKILL_PAGE_SYSTEM //JHKang
	const std::map< CX2SkillTree::SKILL_ID, UserSkillData >& GetMapSkillAcquired( USHORT usPageNum_ ) const
	{
		return m_vecSkillDataMap[usPageNum_];
	}	// 배운 스킬 목록 반환

	std::map< CX2SkillTree::SKILL_ID, UserSkillData >& AccessMapSkillAcquired() { return m_vecSkillDataMap[m_usUsingPage]; }	// 배운 스킬 목록 반환

	const bool IsEnableSkillAcquiredPage() { return ( m_vecSkillDataMap.size() > m_usUsingPage ) ? true : false; }				// 해당 스킬 페이지가 있는지 검사

	void SetDefaultSkill( USHORT usPageIndex_, bool bCanPassIfChecked_ = false );												// 각 전직별 기본 스킬 설정

	void SetUsingPage( IN USHORT usValue_ ) { m_usUsingPage = usValue_ - 1; }
	void SetOpenedPage( IN USHORT usValue_ ) { m_usOpenedPage = usValue_; }
	void SetSelectPage( IN USHORT usValue_ ) { m_usSelectPage = usValue_; }
	
	const USHORT GetUsingPage() const { return m_usUsingPage; }
	const USHORT GetOpenedPage() const { return m_usOpenedPage; }
	const USHORT GetSelectPage() const { return m_usSelectPage; }

	bool IsUsingPage() const { return ( m_usUsingPage + 1 == m_usSelectPage ? true : false ); }
#else //SKILL_PAGE_SYSTEM
	const std::map< CX2SkillTree::SKILL_ID, UserSkillData >& GetMapSkillAcquired() const { return m_mapSkillAcquired; }	/// 배운 스킬 목록 반환
	std::map< CX2SkillTree::SKILL_ID, UserSkillData >& AccessMapSkillAcquired() { return m_mapSkillAcquired; }	/// 배운 스킬 목록 반환
	void SetDefaultSkill( bool bCanPassIfChecked_ = false );													/// 각 전직별 기본 스킬 설정
#endif //SKILL_PAGE_SYSTEM
	int  GetIncreaseSkillLevel( CX2SkillTree::SKILL_ID eSkillID ) const;	/// 레벨 증가치 값 반환
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	/// 스킬 슬롯 ID에 설정 되어있는 스킬 아이디 반환
	const CX2SkillTree::SKILL_ID GetSkillIDByEquipSlotID( IN const int iStateID_ ) const;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	const float GetIncreaseGainSoulRateByHit() const;
	const float GetIncreaseGainSoulRateByDamage() const;
#endif //SERV_9TH_NEW_CHARACTER

/// private variable Start ----------
private:
	typedef std::map< CX2SkillTree::SKILL_ID, UserSkillData > SkillDataMap;				// <skill id, UserSkillData>
#ifdef SKILL_PAGE_SYSTEM //JHKang
	std::vector<SkillDataMap>	m_vecSkillDataMap;		/// 스킬 데이터 벡터
	USHORT	m_arrSkillPoint[3];		/// 스킬 포인트 벡터
	USHORT	m_arrCashSkillPoint[3];	/// 캐시 스킬 포인트 벡터
	USHORT	m_usUsingPage;			/// 현재 활성화된 페이지 / 벡터 접근( 0, 1, 2 )
	USHORT	m_usOpenedPage;			/// 확장한(열린) 페이지 수( 최대 3 )
	USHORT	m_usSelectPage;			/// 현재 선택한 스킬 페이지 번호
#else //SKILL_PAGE_SYSTEM
	SkillDataMap						m_mapSkillAcquired;
#endif //SKILL_PAGE_SYSTEM

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
	SkillDataMap						m_mapGuildSkillAcquired;
#endif GUILD_SKILL
	std::set<CX2SkillTree::SKILL_ID>	m_setUnsealedSkillID;

	SkillSlotData						m_aEquippedSkill[ EQUIPPED_SKILL_SLOT_COUNT ];
	SkillSlotData						m_aEquippedSkillSlotB[ EQUIPPED_SKILL_SLOT_COUNT ];

	SKILL_SLOT_B_EXPIRATION_STATE		m_eSkillSlotBExpirationState;
	std::wstring						m_wstrSkillSlotBEndDate;

	int									m_iUnitClass;
#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
	int									m_iGuildClass;
#endif GUILD_SKILL

#ifdef SERV_SKILL_NOTE
	std::vector<int> m_vecSkillNote; // CX2SkillTree::SKILL_MEMO_ID
#endif

/// private variable End ----------

/// private function Start ----------
private:
#ifdef NEXON_QA_CHEAT_REQ
	void AllSkillNoCoolTime();
#endif //NEXON_QA_CHEAT_REQ
	/// private function End ----------
};



#endif NEW_SKILL_TREE