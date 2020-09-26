#pragma once

namespace _CONST_ELESIS_DETONATION_ 
{// 엘리시스 검의 길에 사용되는 상수
	const float MAX_CHARGE_DETONATION_VALUE = 2000.f; /// 최대 기폭 충전량
	const float	CHARGE_RATE_FOR_DETONATION = 3000.f;  /// 초당 기폭 충전량
}

class CX2GageData;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2GageData> CX2GageDataPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2GageData> CX2GageDataPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2ElswordGageData;
class CX2GageData {

public:

	struct GageFactor
	{
		float	fTime;
		float	fFactor;

		GageFactor()
		{
			fTime = 0.f;
			fFactor = 0.f;
		}
	};

	// TODO: Gage 구조체를 GageManager 클래스를 제외하고는 직접 참조할 수 없게 다 막자
	struct Gage
	{
		bool					bShow;

		KProtectedType<float>	fNow;
		KProtectedType<float>	fMax;
		KProtectedType<float>	fChangeRate;

		std::vector<GageFactor> vecFactor;

	public: 
		Gage()
		{
			Init();
		}
		
		Gage( const Gage *pGage)
		{
			if( NULL == pGage )
			{
				Init();
			}
			else
			{
				bShow = pGage->bShow;
				fNow = pGage->fNow;
				fMax = pGage->fMax;
				fChangeRate = pGage->fChangeRate;
				vecFactor.clear();
				vecFactor = pGage->vecFactor;
			}
		}

		Gage( const Gage& rhs_ )
		{
			bShow = rhs_.bShow;
			fNow = rhs_.fNow;
			fMax = rhs_.fMax;
			fChangeRate = rhs_.fChangeRate;
			vecFactor.clear();
			vecFactor = rhs_.vecFactor;
		}

		void operator=( const Gage& rhs_ )
		{
			bShow = rhs_.bShow;
			fNow = rhs_.fNow;
			fMax = rhs_.fMax;
			fChangeRate = rhs_.fChangeRate;
			vecFactor.clear();
			vecFactor = rhs_.vecFactor;
		}

		void Init()
		{
			bShow		= true;
			fNow		= 0.0f;
			fMax		= 1.0f;
			fChangeRate	= 1.0f;
			vecFactor.clear();
		}

		void AddFactor( const float fFactor_, const float fTime_ )
		{
			GageFactor gageFactor;
			gageFactor.fFactor = fFactor_;
			gageFactor.fTime = fTime_;

			vecFactor.push_back(gageFactor);
		}

		void ClearFactor()
		{
			vecFactor.clear();
		}
		void Increase( const float fIncrement, const float fMinimum = 0.f );

	};

	CX2GageData() : m_pHpGage( NULL ), m_pMpGage( NULL ), m_fSwapGageTime( 0.0f ), 
		m_fChangeRateByItem( 0.0f ), m_fChangeRateTimeByItem( 0.0f ), m_fHyperModeRemainTime( 0.0f ),
		m_byHyperModeCount( 0 ), m_bEnable( false ), m_bCanChargeMpForDetonation( false ),
		m_bHpRelativeChangeRate( false ), m_ChargeMpGageForDetonation(), m_ForceDownGage(), m_bUpdateMpPoint(true)
#ifdef FIX_LIMITED_MANA_MANAGEMENT
		,m_fAddMaxMP( 0.f )
#endif //FIX_LIMITED_MANA_MANAGEMENT
#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
		, m_bIsActivateForceDown(false)
		, m_bIsActivateDetonation(false)
		, m_fNowChargeMpForDetonation(0.f)
#endif //FIX_FORCE_DOWN_AND_DETONATION_BUG
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{
		Init();
		m_SoulGage.fMax = 100.0f;
	}

	CX2GageData( const CX2GageData& rhs_ ) : 
		m_pHpGage( new Gage( rhs_.m_pHpGage ) ), m_pMpGage( new Gage( rhs_.m_pMpGage ) ), m_SoulGage( rhs_.m_SoulGage ),
		m_fSwapGageTime( rhs_.m_fSwapGageTime ),
		m_fChangeRateByItem( rhs_.m_fChangeRateByItem ), m_fChangeRateTimeByItem( rhs_.m_fChangeRateTimeByItem ),
		m_fHyperModeRemainTime( rhs_.m_fHyperModeRemainTime ), m_byHyperModeCount( rhs_.m_byHyperModeCount ),
		m_bEnable( rhs_.m_bEnable ), m_bCanChargeMpForDetonation( rhs_.m_bCanChargeMpForDetonation ),
		m_bHpRelativeChangeRate( rhs_.m_bHpRelativeChangeRate ), m_ChargeMpGageForDetonation( rhs_.m_ChargeMpGageForDetonation ),
		m_ForceDownGage( rhs_.m_ForceDownGage ), m_bUpdateMpPoint( rhs_.m_bUpdateMpPoint )
#ifdef FIX_LIMITED_MANA_MANAGEMENT
		,m_fAddMaxMP( rhs_.m_fAddMaxMP )
#endif //FIX_LIMITED_MANA_MANAGEMENT
#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
		,m_bIsActivateForceDown( rhs_.m_bIsActivateForceDown )
		,m_bIsActivateDetonation( rhs_.m_bIsActivateDetonation )
		,m_fNowChargeMpForDetonation( rhs_.m_fNowChargeMpForDetonation )
#endif //FIX_FORCE_DOWN_AND_DETONATION_BUG
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{
	}

	virtual ~CX2GageData()
	{
		SAFE_DELETE( m_pHpGage );
		SAFE_DELETE( m_pMpGage );
	}

	void operator=( const CX2GageData& rhs_ )
	{
		m_bCanChargeMpForDetonation = rhs_.m_bCanChargeMpForDetonation;
		m_bEnable = rhs_.m_bEnable;
		m_bHpRelativeChangeRate = rhs_.m_bHpRelativeChangeRate;
		m_byHyperModeCount = rhs_.m_byHyperModeCount;
		m_ChargeMpGageForDetonation = rhs_.m_ChargeMpGageForDetonation;
		m_fChangeRateByItem = rhs_.m_fChangeRateByItem;
		m_fChangeRateTimeByItem = rhs_.m_fChangeRateTimeByItem;
		m_fHyperModeRemainTime = rhs_.m_fHyperModeRemainTime;
		m_ForceDownGage = rhs_.m_ForceDownGage;
		m_fSwapGageTime = rhs_.m_fSwapGageTime;
		*(m_pHpGage) = *(rhs_.m_pHpGage);
		*(m_pMpGage) = *(rhs_.m_pMpGage);		
		m_SoulGage = rhs_.m_SoulGage;	
		m_bUpdateMpPoint = rhs_.m_bUpdateMpPoint;
#ifdef FIX_LIMITED_MANA_MANAGEMENT
		m_fAddMaxMP = rhs_.m_fAddMaxMP;
#endif //FIX_LIMITED_MANA_MANAGEMENT
#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
		m_bIsActivateForceDown = rhs_.m_bIsActivateForceDown;
		m_bIsActivateDetonation = rhs_.m_bIsActivateDetonation;
		m_fNowChargeMpForDetonation = rhs_.m_fNowChargeMpForDetonation;
#endif //FIX_FORCE_DOWN_AND_DETONATION_BUG
	}

	void			Init()
	{
		SAFE_DELETE ( m_pHpGage );
		m_pHpGage = new Gage();

		SAFE_DELETE ( m_pMpGage );
		m_pMpGage = new Gage();
	}

	void SetShow( const bool bShow_ ) { ASSERT( !L"Gage Data SetShow" ); }
	bool GetShow() const { ASSERT( !L"Gage Data GetShow" ); }
	////////////////// Hp 관련 Get/Set ///////////////////////////////////////////
	/////// NowHp
	float			GetNowHp() const { return ( NULL != m_pHpGage ? m_pHpGage->fNow : 0.0f );	}
	void			SetNowHp( const float fNowHp_ )
	{
		if ( NULL != m_pHpGage )
		{
			if ( 0 > fNowHp_ )
				m_pHpGage->fNow = fNowHp_;
			else if ( fNowHp_ > GetMaxHp() )
				m_pHpGage->fNow = GetMaxHp();
			else
				m_pHpGage->fNow = fNowHp_;
		}
	}

	void			UpNowHp( const float fChangeOfNowHp_, const float fMinimum_ = 0.0f )
	{
		if ( NULL != m_pHpGage )
			m_pHpGage->Increase( fChangeOfNowHp_, fMinimum_ );
	}

	bool			VerifyNowHp() 
	{
		if ( NULL != m_pHpGage )
			return m_pHpGage->fNow.Verify();	
		else
			return false;
	}

	////// MaxHp
	float			GetMaxHp() const { return ( NULL != m_pHpGage ? m_pHpGage->fMax : 0.0f );	}
	void			SetMaxHp( const float fMaxHp_ )
	{
		if ( NULL != m_pHpGage )
			m_pHpGage->fMax = fMaxHp_;
	}
	void			UpMaxHp( const float fChangeofMaxHp_ )
	{
		if ( NULL != m_pHpGage )
			m_pHpGage->fMax += fChangeofMaxHp_;
	}

	/////// ChangeRateHp
	float			GetChangeRateHp() const { return ( NULL != m_pHpGage ? m_pHpGage->fChangeRate : 0.0f );	}
	void			SetChangeRateHp( const float fChangeRateHp_ )
	{
		if ( NULL != m_pHpGage )
			m_pHpGage->fChangeRate = fChangeRateHp_;
	}
	void			UpChangeRateHp( const float fChangeofChangeRateHp_ )
	{
		if ( NULL != m_pHpGage )
			m_pHpGage->fChangeRate += fChangeofChangeRateHp_;
	}

	////////////////// Mp 관련 Get/Set ///////////////////////////////////////////
	////// NowMp
	float			GetNowMp() const { return ( NULL != m_pMpGage ? m_pMpGage->fNow : 0.0f );	}
	void			SetNowMp( const float fNowMp_ )
	{
		if ( NULL != m_pMpGage )
		{
			if ( 0 > fNowMp_ )
				m_pMpGage->fNow = fNowMp_;
			else if ( fNowMp_ > GetMaxMp() )
				m_pMpGage->fNow = GetMaxMp();
			else
				m_pMpGage->fNow = fNowMp_;
		}
	}

	void			UpNowMp( const float fChangeOfNowMp_ )
	{
		if ( NULL != m_pMpGage )
			m_pMpGage->Increase( fChangeOfNowMp_ );
	}

	////// MaxMp
	float			GetMaxMp() const { return ( NULL != m_pMpGage ? m_pMpGage->fMax : 0.0f ); }
	void			SetMaxMp( const float fMaxMp_ )
	{
		if ( NULL != m_pMpGage )
		{
			if( m_pMpGage->fMax != fMaxMp_ )
			{
				m_pMpGage->fMax = fMaxMp_;
				m_bUpdateMpPoint = true;
			}
		}
	}
	void			UpMaxMp( const float fChangeOfMaxMp_ )
	{
		if ( NULL != m_pMpGage )
		{
			m_pMpGage->fMax += fChangeOfMaxMp_;
			m_bUpdateMpPoint = true;
		}
	}
#ifdef FIX_LIMITED_MANA_MANAGEMENT
	/** @function : SetAddMaxMP
		@brief : 최대 MP량을 추가 값
				 중첩 추가 되지 않고 1회만 추가 할 수 있도록 구현됨
		@param : fAddMaxMP_ : 추가할 MP량
	*/
	void SetAddMaxMP( const float fAddMaxMP_ )  
	{		
		if ( fAddMaxMP_ == m_fAddMaxMP )
			return;

		//이전에 추가했던 최대 MP량을 제거 후 새로운 추가 MP량으로 변경
		float fNowMaxMp = GetMaxMp() - m_fAddMaxMP + fAddMaxMP_;
		SetMaxMp( fNowMaxMp );

		//금회 MP 추가량을 기록
		m_fAddMaxMP = fAddMaxMP_;	
	}
#endif //FIX_LIMITED_MANA_MANAGEMENT

	/////// ChangeRateMp
	float			GetChangeRateMp() const { return ( NULL != m_pMpGage ? m_pMpGage->fChangeRate : 0.0f );	}
	void			SetChangeRateMp( const float fChangeRateMp_ )
	{
		if ( NULL != m_pMpGage )
			m_pMpGage->fChangeRate = fChangeRateMp_;
	}
	void			UpChangeRateMp( const float fChangeofChangeRateMp_ )
	{
		if ( NULL != m_pMpGage )
			m_pMpGage->fChangeRate += fChangeofChangeRateMp_;
	}

	////////////////// Soul(각성) 관련 Get/Set ///////////////////////////////////////////
	////// NowSoul
	float			GetNowSoul() const { return m_SoulGage.fNow; }
	void			SetNowSoul( const float fNowSoul_ ) { m_SoulGage.fNow = fNowSoul_; }

	void			UpNowSoul( const float fChangeOfNowSoul_ ) 
	{ 
		m_SoulGage.Increase( fChangeOfNowSoul_ );
	}

	////// MaxSoul
	float			GetMaxSoul() const { return m_SoulGage.fMax; }
	void			SetMaxSoul( const float fMaxSoul_ ) { m_SoulGage.fMax = fMaxSoul_; }
	void			UpMaxSoul( const float fChangeOfMaxSoul_ ) { m_SoulGage.fMax += fChangeOfMaxSoul_; }

	/////// ChangeRateSoul
	float			GetChangeRateSoul() const { return m_SoulGage.fChangeRate;	}
	void			SetChangeRateSoul( const float fChangeRateSoul_ ) { m_SoulGage.fChangeRate = fChangeRateSoul_; }
	void			UpChangeRateSoul( const float fChangeofChangeRateSoul_ ) { m_SoulGage.fChangeRate += fChangeofChangeRateSoul_; }

	////////////////// ForceDown 수치 관련 Get/Set ///////////////////////////////////////////
	////// NowForceDown
	float			GetNowForceDown() const { return m_ForceDownGage.fNow;	}
	void			SetNowForceDown( const float fNowForceDown_ ) { m_ForceDownGage.fNow = fNowForceDown_; }
	void			UpNowForceDown( const float fChangeOfNowForceDown_ ) 
	{ 
		m_ForceDownGage.Increase( fChangeOfNowForceDown_ );
	}

	////// MaxForceDown
	float			GetMaxForceDown() const { return m_ForceDownGage.fMax; }
	void			SetMaxForceDown( const float fMaxForceDown_ ) { m_ForceDownGage.fMax = fMaxForceDown_; }
	void			UpMaxForceDown( const float fChangeOfMaxForceDown_ ) { m_ForceDownGage.fMax += fChangeOfMaxForceDown_; }

	/////// ChangeRateForceDown
	float			GetChangeRateForceDown() const { return m_ForceDownGage.fChangeRate; }
	void			SetChangeRateForceDown( const float fChangeRateForceDown_ ) { m_ForceDownGage.fChangeRate = fChangeRateForceDown_; }
	void			UpChangeRateForceDown( const float fChangeofChangeRateForceDown_ ) { m_ForceDownGage.fChangeRate += fChangeofChangeRateForceDown_; }

	////////////////// ChargeMpGageForDetonation(기폭) 수치 관련 Get/Set ///////////////////////////////////////////
	////// ChargeMpForDetonation
	float			GetNowChargeMpForDetonation() const { return m_ChargeMpGageForDetonation.fNow; }
	void			SetNowChargeMpForDetonation( const float fChargeMpForDetonation_ ) {
		m_ChargeMpGageForDetonation.fNow = fChargeMpForDetonation_;
	}
	void			UpNowChargeMpForDetonation( const float fChangeOfChargeMpForDetonation_ ) {
		m_ChargeMpGageForDetonation.fNow += fChangeOfChargeMpForDetonation_;
	}

	////// MaxChargeMpforDetonation
	float			GetMaxChargeMpForDetonation() const { return m_ChargeMpGageForDetonation.fMax; }
	void			SetMaxChargeMpForDetonation( const float fMaxForceDown_ ) {
		m_ChargeMpGageForDetonation.fMax = fMaxForceDown_;
	}
	void			UpMaxChargeMpforDetonation( const float fChangeOfMaxForceDown_ ) {
		m_ChargeMpGageForDetonation.fMax += fChangeOfMaxForceDown_;
	}

	/////// ChangeRateChargeMpForDetonation
	float			GetChangeRateChargeMpForDetonation() const { return m_ChargeMpGageForDetonation.fChangeRate; }
	void			SetChangeRateChargeMpForDetonation( const float fChangeRateChargeMpForDetonation_ ) { 
		m_ChargeMpGageForDetonation.fChangeRate = fChangeRateChargeMpForDetonation_; 
	}
	void			UpChangeRateChargeMpForDetonation( const float fChangeofChangeRateChargeMpForDetonation_ ) { 
		m_ChargeMpGageForDetonation.fChangeRate += fChangeofChangeRateChargeMpForDetonation_; 
	}

	bool			GetEnable() const { return m_bEnable; }
	void			SetEnable( const bool bEnable_ ) { m_bEnable = bEnable_; }
	
	// 키 입력을 통한 기폭 충전 상태 여부
	bool			GetCanChargeMpForDetonation() const { return m_bCanChargeMpForDetonation; }
	void			SetCanChargeMpForDetonation( const bool bCanChargeMpForDetonation_ ) { m_bCanChargeMpForDetonation = bCanChargeMpForDetonation_; }

	float			GetChangeRateByItem() const { return m_fChangeRateByItem; }
	void			SetChangeRateByItem( const float fChangeRateByItem_ ) { m_fChangeRateByItem = fChangeRateByItem_; }

	float			GetChangeRateTimeByItem() const { return m_fChangeRateTimeByItem; }
	void			SetChangeRateTimeByItem( const float fChangeRateTimeByItem_ ) { m_fChangeRateTimeByItem = fChangeRateTimeByItem_; }

	bool			GetEnableHPRelativeChangeRate() const { return m_bHpRelativeChangeRate; }
	void			SetEnableHPRelativeChangeRate( const bool bHpRelativeChangeRate_ ) { m_bHpRelativeChangeRate = bHpRelativeChangeRate_; }

	void			AddMPFactor( const float fFactor_, const float fTime_ ) { m_pMpGage->AddFactor( fFactor_, fTime_ ); }
	void			ClearMPFactor() { m_pMpGage->ClearFactor(); }

	bool			FlushMp( const float fFlushMp_ );

	// 오현빈 // 엘리시스 섬멸 기폭 구현을 위해 가상함수로 변경
	virtual bool	DamageFlushMp();
	bool			FlushForceDown();
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
    bool            DamageFlushMp_CheckOnly();
	bool			FlushForceDown_CheckOnly();
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS

	bool			FlushSoul( const float fFlushSoul_ );

	void			SwapGage()
	{
		Gage *pGage = new Gage(m_pHpGage);
		SAFE_DELETE(m_pHpGage);
		m_pHpGage = pGage;

		pGage = new Gage(m_pMpGage);
		SAFE_DELETE(m_pMpGage);
		m_pMpGage = pGage;
		m_fSwapGageTime = 0.f;
	}

	float GetHyperModeRemainTime() const { return m_fHyperModeRemainTime; }
	void SetHyperModeRemainTime( const float fHyperModeRemainTime_ ) { m_fHyperModeRemainTime = fHyperModeRemainTime_; }
	bool VerifyHyperModeRemainTime() const { return m_fHyperModeRemainTime.Verify(); }

	BYTE GetHyperModeCount() const { return m_byHyperModeCount; }
	void SetHyperModeCount( BYTE byHyperModeCount_ ) { m_byHyperModeCount = byHyperModeCount_; }

	///////dummy

	void SetBossGaugeNumber( IN u_short usNumber_ ) { ASSERT( !L"Not Exist" ); }
	//////////////dummy
	virtual void	OnFrameMove( double fTime, float fElapsedTime );
	virtual void	DetonationFrameMove( double fTime, float fElapsedTime );

	virtual void	UpdateDataFromGameUnit( const CX2GageData* pGageData_ );
	virtual void	InitWhenGameIsOver();

	virtual bool GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus_ ) const;
	virtual void SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus_ );

	bool Veryfy() const;

	void UseSpecialAbilityInVillage( const CX2Item::ItemTemplet* pItemTemplet_ );

	/// 현재 GageData의 복사본을 생성 (파생 클래스 마다 서로의 복사본을 생성 하도록 하기 위해 가상으로 만듦)
	virtual CX2GageData* GetCloneGageData();
	virtual void CopyGageData( IN CX2GageData* pGageData_ );

	bool GetUpdateMpPoint()const{return m_bUpdateMpPoint;}
	void SetUpdateMpPoint(bool bVal){m_bUpdateMpPoint = bVal;}

#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
	// 다운수치
	bool GetActivateForceDown() const { return m_bIsActivateForceDown; }
	void SetActivateForceDown(bool val) { m_bIsActivateForceDown = val; }
	bool UpdateFlushForceDown();

	// 기폭
	bool GetActivateDetonation() const { return m_bIsActivateDetonation; }
	void SetActivateDetonation(bool val, float fNowCharge ) { m_bIsActivateDetonation = val; m_fNowChargeMpForDetonation = fNowCharge; }
	bool UpdateDamageFlushMP();
#endif // FIX_FORCE_DOWN_AND_DETONATION_BUG

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

protected:

	float GetSwapGageTime() const { return m_fSwapGageTime; }
	void SetSwapGageTime( const float fSwapGageTime_ ) { m_fSwapGageTime = fSwapGageTime_; }
	void UpSwapGageTime( const float fChangeOfSwapGageTime_ ) { m_fSwapGageTime += fChangeOfSwapGageTime_; }

private:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	Gage*					m_pHpGage;
	Gage*					m_pMpGage;
	Gage					m_SoulGage;
	Gage					m_ForceDownGage;
	Gage					m_ChargeMpGageForDetonation;

	float					m_fSwapGageTime;

#ifdef DUNGEON_ITEM
	KProtectedType<float>	m_fChangeRateByItem;		/// 소인의 비약 인듯.
	KProtectedType<float>	m_fChangeRateTimeByItem;	
#endif

	KProtectedType<float>	m_fHyperModeRemainTime;		/// 각성 남은 시간

	BYTE					m_byHyperModeCount;						// 각성 구슬 갯수
	bool					m_bEnable;
	bool					m_bCanChargeMpForDetonation;			// 기폭

#ifdef HP_RELATIVE_CHANGE_RATE
	bool					m_bHpRelativeChangeRate;	/// NPC 중에 ??
#endif HP_RELATIVE_CHANGE_RATE
	bool					m_bUpdateMpPoint;

#ifdef FIX_LIMITED_MANA_MANAGEMENT
	//오현빈//2013-01-23 //
	//보프 패시브 극한의 마나운용에서 사용하기 위해 GUUser객체에 있던 m_bLimitManaManagement 변수를
	//유저 객체 생성/소멸에 상관없이 각성시간동안 유지 시키기 위해 GageData클래스로 이동
	float					m_fAddMaxMP;				/// 최대 마나량 증가 값
#endif //FIX_LIMITED_MANA_MANAGEMENT

#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
	bool					m_bIsActivateForceDown;		// 다운수치 활성화 여부
	bool					m_bIsActivateDetonation;	// 기폭 활성화 여부
	float					m_fNowChargeMpForDetonation;// 기폭 활성화 되었을 때 충전 량
#endif //FIX_FORCE_DOWN_AND_DETONATION_BUG
};

IMPLEMENT_INTRUSIVE_PTR( CX2GageData );

class CX2ChungGageData : public CX2GageData
{
public:
	CX2ChungGageData() : CX2GageData(),
		m_iNowCannonBallCount( 0 ), m_iMaxCannonBallCount( 0 ),
		m_iNowCannonBallCountEx( 0 ), m_iMaxCannonBallCountEx( 0 ),
		m_bBerserkMode( false ), m_bBerserkModeChanged( false ), m_bCannonBallChanged( false )
	{}

	CX2ChungGageData( const CX2ChungGageData& rhs_ ) : CX2GageData( rhs_ ),
		m_iNowCannonBallCount( rhs_.m_iNowCannonBallCount ), m_iMaxCannonBallCount( rhs_.m_iMaxCannonBallCount ),
		m_iNowCannonBallCountEx( rhs_.m_iNowCannonBallCountEx ), m_iMaxCannonBallCountEx( rhs_.m_iMaxCannonBallCountEx ),
		m_bBerserkMode( rhs_.m_bBerserkMode ), m_bBerserkModeChanged( rhs_.m_bBerserkModeChanged ),
		m_bCannonBallChanged( rhs_.m_bCannonBallChanged )
	{}

	void operator=( const CX2ChungGageData& rhs_ )
	{
		CX2GageData::operator =( rhs_ );
		m_iNowCannonBallCount = rhs_.m_iNowCannonBallCount;
		m_iMaxCannonBallCount = rhs_.m_iMaxCannonBallCount;
		m_iNowCannonBallCountEx = rhs_.m_iNowCannonBallCountEx;
		m_iMaxCannonBallCountEx = rhs_.m_iMaxCannonBallCountEx;
		m_bBerserkMode = rhs_.m_bBerserkMode;
		m_bBerserkModeChanged = rhs_.m_bBerserkModeChanged;
		m_bCannonBallChanged = rhs_.m_bCannonBallChanged;
	}

	int GetNowCannonBallCount() const { return m_iNowCannonBallCount; }
	void SetNowCannonBallCount( const int iNowCannonBallCount_ ) 
	{ 
		if ( GetMaxCannonBallCount() < iNowCannonBallCount_ )
			m_iNowCannonBallCount = GetMaxCannonBallCount(); 
		else if ( 0 > iNowCannonBallCount_ )
			m_iNowCannonBallCount = 0;
		else
			m_iNowCannonBallCount = iNowCannonBallCount_;
	}

	int GetMaxCannonBallCount() const { return m_iMaxCannonBallCount; }
	void SetMaxCannonBallCount( const int iMaxCannonBallCount_ ) { m_iMaxCannonBallCount = iMaxCannonBallCount_; }

	bool GetBerserkMode() const { return m_bBerserkMode; }
	void SetBerserkMode( const bool bBerserkMode_ ) { m_bBerserkMode = bBerserkMode_; }

	bool GetBerserkModeChanged() const { return m_bBerserkModeChanged; }
	void SetBerserkModeChanged( bool bBerserkModeChanged_ ) { m_bBerserkModeChanged = bBerserkModeChanged_; }

	bool GetCannonBallChanged() const { return m_bCannonBallChanged; }
	void SetCannonBallChanged( const bool bCannonBallChanged_ ) { m_bCannonBallChanged = bCannonBallChanged_; }

	bool IsFullCannonBallCount() const { return GetNowCannonBallCount() >= GetMaxCannonBallCount(); }
	bool IsEmptyCannonBallCount() const { return GetNowCannonBallCount() <= 0; }

	/// 택틱컬 투르퍼
	int GetMaxCannonBallCountEx() const { return m_iMaxCannonBallCountEx; }
	void SetMaxCannonBallCountEx( const int iMaxCannonBallCountEx_ ) { m_iMaxCannonBallCountEx = iMaxCannonBallCountEx_; }

	int GetNowCannonBallCountEx() const { return m_iNowCannonBallCountEx; }
	void SetNowCannonBallCountEx( const int iNowCannonBallCountEx_ ) 
	{ 
		if ( GetMaxCannonBallCountEx() < iNowCannonBallCountEx_ )
			m_iNowCannonBallCountEx = GetMaxCannonBallCountEx(); 
		else if ( 0 > iNowCannonBallCountEx_ )
			m_iNowCannonBallCountEx = 0;
		else
			m_iNowCannonBallCountEx = iNowCannonBallCountEx_;
	}

	virtual void	UpdateDataFromGameUnit( const CX2GageData* pGageData_ );
	virtual void	InitWhenGameIsOver();
	
	virtual bool	GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const;
	virtual void	SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus );

	/// 현재 GageData의 복사본을 생성 (파생 클래스 마다 서로의 복사본을 생성 하도록 하기 위해 순수가상으로 만듦)
	virtual CX2GageData* GetCloneGageData();
	virtual void CopyGageData( IN CX2GageData* pGageData_ );

private:
	const static int		EXTRA_CANNON_BALL_COUNT_BITMASK_VALUE = 7;
	KProtectedType<int>		m_iNowCannonBallCount;
	KProtectedType<int>		m_iMaxCannonBallCount;
	
	/// 택틱컬투르퍼 전용 GageData 클래스를 만들고 싶었으나, CX2GUChung 자체가 클래스 하나로 되어 있기 때문에
	/// GageData를 직업별로 나누게 되면 사용에 어려움이 있어서 나누지 않았음
	KProtectedType<int>		m_iNowCannonBallCountEx;
	KProtectedType<int>		m_iMaxCannonBallCountEx;

	bool					m_bBerserkMode;
	bool					m_bBerserkModeChanged;
	bool					m_bCannonBallChanged;
	
};


class CX2ElswordGageData : public CX2GageData
{
public:
	enum WAY_OF_SWORD_STATE
	{
		WSS_DESTRUCTION				= -2,
		WSS_TOWARD_DESTRUCTION		= -1,
		WSS_CENTER					= 0,
		WSS_TOWARD_VIGOR			= 1,
		WSS_VIGOR					= 2,
	};

	CX2ElswordGageData() : CX2GageData(), m_bChangedWayOfSwordState( true ), 
		m_eWayOfSwordState( 0/*WSS_CENTER*/ ), m_fWayOfSwordPoint( 0.0f )
	{}
	
	CX2ElswordGageData( const CX2ElswordGageData& rhs_ ) : CX2GageData( rhs_ ),
		m_bChangedWayOfSwordState( rhs_.m_bChangedWayOfSwordState ),
		m_eWayOfSwordState( rhs_.m_eWayOfSwordState ), m_fWayOfSwordPoint( rhs_.m_fWayOfSwordPoint )
	{}

	void operator=( const CX2ElswordGageData& rhs_ )
	{
		CX2GageData::operator =( rhs_ );
		m_bChangedWayOfSwordState = rhs_.m_bChangedWayOfSwordState;
		SetWayOfSwordState( rhs_.m_eWayOfSwordState );
		m_fWayOfSwordPoint = rhs_.m_fWayOfSwordPoint;
	}

	bool GetChangedWayOfSwordState() const { return m_bChangedWayOfSwordState; }
	void SetChangedWayOfSwordState( const bool bChangedWayOfSwordState_ ) { m_bChangedWayOfSwordState = bChangedWayOfSwordState_; }

	int GetWayOfSwordState() const { return m_eWayOfSwordState; }
	void SetWayOfSwordState(/*CX2ElswordGageData::WAY_OF_SWORD_STATE*/ int val)
	{ 
		if ( GetWayOfSwordState() != val )
			SetChangedWayOfSwordState( true );

		m_eWayOfSwordState = val; 
	}

	float GetWayOfSwordPoint() const { return m_fWayOfSwordPoint; }
	void SetWayOfSwordPoint(float val) { m_fWayOfSwordPoint = val; }

	virtual void	UpdateDataFromGameUnit( const CX2GageData* pGageData_ );
	virtual void	InitWhenGameIsOver();
	virtual bool	GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const;
	virtual void	SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus );

	/// 현재 GageData의 복사본을 생성 (파생 클래스 마다 서로의 복사본을 생성 하도록 하기 위해 순수가상으로 만듦)
	virtual CX2GageData* GetCloneGageData();
	virtual void CopyGageData( IN CX2GageData* pGageData_ );

private:
	bool						m_bChangedWayOfSwordState;	// 검의 길 스테이트가 변경 되었는지 여부
	int/*WAY_OF_SWORD_STATE*/	m_eWayOfSwordState;	
	float						m_fWayOfSwordPoint;
	
};

#pragma region CX2AraGageData
/** @class : CX2AraGageData
	@brief : 아라 기력 관련 게이지
	@date  : 2012/11/26
*/
class CX2AraGageData : public CX2GageData
{
public:
	CX2AraGageData() : CX2GageData(), m_bBerserkMode( false ), m_bBerserkModeChanged( false ), m_bForcePowerChanged( false )
		, m_iNowForcePowerCount( 0 ), m_iMaxForcePowerCount( 0 )
	{}

	CX2AraGageData( const CX2AraGageData& rhs_ ) : CX2GageData( rhs_ ), m_bBerserkMode( rhs_.m_bBerserkMode )
		, m_bBerserkModeChanged( rhs_.m_bBerserkModeChanged ), m_bForcePowerChanged( rhs_.m_bForcePowerChanged )
		, m_iNowForcePowerCount( rhs_.m_iNowForcePowerCount), m_iMaxForcePowerCount( rhs_.m_iMaxForcePowerCount )
	{}

	void operator=( const CX2AraGageData& rhs_ )
	{
		CX2GageData::operator =( rhs_ );
		m_bBerserkMode = rhs_.m_bBerserkMode;
		m_bBerserkModeChanged = rhs_.m_bBerserkModeChanged;
		m_bForcePowerChanged = rhs_.m_bForcePowerChanged;
		m_iNowForcePowerCount = rhs_.m_iNowForcePowerCount;
		m_iMaxForcePowerCount = rhs_.m_iMaxForcePowerCount;
	}
		
	bool GetBerserkMode() const;
	void SetBerserkMode( const bool bBerserkMode_ );

	bool GetBerserkModeChanged() const;
	void SetBerserkModeChanged( bool bBerserkModeChanged_ );

	bool GetForcePowerChanged() const;
	void SetForcePowerChanged( const bool bForcePowerChanged_ );

	int GetNowForcePower() const;
	void SetNowForcePower( const int iNowForcePowerCount_ );

	int GetMaxForcePower() const;
	void SetMaxForcePower( const int iMaxForcePowerCount_ );

	bool IsFullForcePower() const;
	bool IsEmptyForcePower() const;
	
	virtual void	UpdateDataFromGameUnit( const CX2GageData* pGageData_ );
	virtual void	InitWhenGameIsOver();

	virtual bool	GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const;
	virtual void	SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus );

	/// 현재 GageData의 복사본을 생성 (파생 클래스 마다 서로의 복사본을 생성 하도록 하기 위해 순수가상으로 만듦)
	virtual CX2GageData* GetCloneGageData();
	virtual void CopyGageData( IN CX2GageData* pGageData_ );

private:
	bool	m_bBerserkMode;				/// 3각성 광폭화 상태인가?
	bool	m_bBerserkModeChanged;		/// 광폭화 상태로 전환되었는가?
	bool	m_bForcePowerChanged;		/// 기력이 변경되었는가?

	KProtectedType<int>	m_iNowForcePowerCount;	/// 현재 기력 수
	KProtectedType<int>	m_iMaxForcePowerCount;	/// 최대 기력 수
};
#pragma endregion 클래스

#ifdef NEW_CHARACTER_EL
class CX2ElesisGageData : public CX2GageData
{
public:
	enum WAY_OF_SWORD_STATE
	{
		WSS_DESTRUCTION				= -2,
		WSS_TOWARD_DESTRUCTION		= -1,
		WSS_CENTER					= 0,
		WSS_TOWARD_VIGOR			= 1,
		WSS_VIGOR					= 2,
	};

	CX2ElesisGageData() : CX2GageData(), m_bChangedWayOfSwordState( true ), 
		m_eWayOfSwordState( 0/*WSS_CENTER*/ ), m_fWayOfSwordPoint( 0.0f )
	{}

	CX2ElesisGageData( const CX2ElesisGageData& rhs_ ) : CX2GageData( rhs_ ),
		m_bChangedWayOfSwordState( rhs_.m_bChangedWayOfSwordState ),
		m_eWayOfSwordState( rhs_.m_eWayOfSwordState ), m_fWayOfSwordPoint( rhs_.m_fWayOfSwordPoint )
	{}

	void operator=( const CX2ElesisGageData& rhs_ )
	{
		CX2GageData::operator =( rhs_ );
		m_bChangedWayOfSwordState = rhs_.m_bChangedWayOfSwordState;
		SetWayOfSwordState( rhs_.m_eWayOfSwordState );
		m_fWayOfSwordPoint = rhs_.m_fWayOfSwordPoint;
	}

	bool GetChangedWayOfSwordState() const { return m_bChangedWayOfSwordState; }
	void SetChangedWayOfSwordState( const bool bChangedWayOfSwordState_ ) { m_bChangedWayOfSwordState = bChangedWayOfSwordState_; }

	int GetWayOfSwordState() const { return m_eWayOfSwordState; }
	void SetWayOfSwordState(/*CX2ElesisGageData::WAY_OF_SWORD_STATE*/ int val)
	{ 
		if ( GetWayOfSwordState() != val )
			SetChangedWayOfSwordState( true );

		m_eWayOfSwordState = val; 
	}

	float GetWayOfSwordPoint() const { return m_fWayOfSwordPoint; }
	void SetWayOfSwordPoint(float val) { m_fWayOfSwordPoint = val; }

	virtual void	DetonationFrameMove( double fTime, float fElapsedTime );
	virtual void	UpdateDataFromGameUnit( const CX2GageData* pGageData_ );
	virtual void	InitWhenGameIsOver();
	virtual bool	GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const;
	virtual void	SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus );

	/// 현재 GageData의 복사본을 생성 (파생 클래스 마다 서로의 복사본을 생성 하도록 하기 위해 순수가상으로 만듦)
	virtual CX2GageData* GetCloneGageData();
	virtual void CopyGageData( IN CX2GageData* pGageData_ );
	virtual bool DamageFlushMp();
private:
	bool						m_bChangedWayOfSwordState;	// 검의 길 스테이트가 변경 되었는지 여부
	int/*WAY_OF_SWORD_STATE*/	m_eWayOfSwordState;	
	float						m_fWayOfSwordPoint;

};
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
class CX2AddGageData : public CX2GageData
{
public:
	/// 기본 함수들
	CX2AddGageData() : CX2GageData(), 
		m_bIsFormationMode( false ), 
		m_bChangedMutationCount( false ),
		m_iMutationCount( 0 ),
		m_fDPValue( 0.f ),
		m_fMaxDPValue( MAX_DP_GAGE_VALUE ),
		m_fChangeFormationCoolTime( 0.f )
	{}

	CX2AddGageData( const CX2AddGageData& rhs_ ) : CX2GageData( rhs_ ),
		m_bIsFormationMode( rhs_.m_bIsFormationMode ),
		m_bChangedMutationCount( rhs_.m_bChangedMutationCount ),
		m_iMutationCount( rhs_.m_iMutationCount ),
		m_fDPValue( rhs_.m_fDPValue ),
		m_fMaxDPValue( rhs_.m_fMaxDPValue ),
		m_fChangeFormationCoolTime( rhs_.m_fChangeFormationCoolTime )
	{}

	void operator=( const CX2AddGageData& rhs_ )
	{
		CX2GageData::operator		= ( rhs_ );
		m_bIsFormationMode			= rhs_.m_bIsFormationMode;
		m_bChangedMutationCount		= rhs_.m_bChangedMutationCount;
		m_iMutationCount			= rhs_.m_iMutationCount;
		m_fDPValue					= rhs_.m_fDPValue;
		m_fMaxDPValue				= rhs_.m_fMaxDPValue;
		m_fChangeFormationCoolTime	= rhs_.m_fChangeFormationCoolTime;
	}

	virtual void UpdateDataFromGameUnit( const CX2GageData* pGageData_ );

	virtual bool GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const;
	virtual void SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus );

	/// 현재 GageData의 복사본을 생성 (파생 클래스 마다 서로의 복사본을 생성 하도록 하기 위해 순수가상으로 만듬)
	virtual CX2GageData* GetCloneGageData();
	virtual void CopyGageData( IN CX2GageData* pGageData_ );


	/// 추가 함수들
	bool GetIsFormationMode() const { return m_bIsFormationMode; }
	void SetIsFormationMode( bool val_ ) { m_bIsFormationMode = val_; }

	bool GetChangedMutationCount() const { return m_bChangedMutationCount; }
	void SetChangedMutationCount( bool val_ ) { m_bChangedMutationCount = val_; }

	int GetMutationCount() const { return min( max( m_iMutationCount, 0), MAX_MUTATION_COUNT_VALUE ); }
	void SetMutationCount( int val_ ) { m_iMutationCount = min( max( val_, 0), MAX_MUTATION_COUNT_VALUE ); }

	float GetDPValue() const { return min( max( m_fDPValue, 0.f ), MAX_DP_GAGE_VALUE ); }
	void SetDPValue( float val_ ) { m_fDPValue = min( max( val_, 0.f ), MAX_DP_GAGE_VALUE ); }

	float GetMaxDPValue() const { return m_fMaxDPValue; }
	void SetMaxDPValue( float val_ ) { m_fMaxDPValue = val_; }		/// 최대치는 X2Define.h 에 정의 되어 있어요!

	float GetChangeFormationCoolTime() const { return m_fChangeFormationCoolTime; }
	void SetChangeFormationCoolTime( float val_ ) { m_fChangeFormationCoolTime = val_; }

	virtual void OnFrameMove( double fTime, float fElapsedTime );
private:
	bool						m_bIsFormationMode;			/// 구성 모드 적용 여부
	bool						m_bChangedMutationCount;	/// 변이 수치 갱신 여부
	int							m_iMutationCount;			/// 변이 수치
	float						m_fDPValue;					/// DP 수치
	float						m_fMaxDPValue;				/// DP 최대 수치
	float						m_fChangeFormationCoolTime;	/// 구성 모드 전환 쿨타임
};
#endif //SERV_9TH_NEW_CHARACTER


#ifdef ADD_RENA_SYSTEM //김창한
/** @class : CX2RenaGageData
	@brief : 레나 자연의 기운 관련 게이지CX2RenaGageData
*/
class CX2RenaGageData : public CX2GageData
{
public:
	CX2RenaGageData() : CX2GageData(), m_bNFBuffMode( false ), m_bNFBuffModeChange(false ), m_bNaturalForceChanged( false )
		, m_iNowNaturalForceCount( 0 ), m_iMaxNaturalForceCount( MAX_NATURAL_FORCE_VALUE )
	{}

	CX2RenaGageData( const CX2RenaGageData& rhs_ ) : CX2GageData( rhs_ ), m_bNFBuffMode( rhs_.m_bNFBuffMode ), m_bNFBuffModeChange( rhs_.m_bNFBuffModeChange )
		, m_bNaturalForceChanged( rhs_.m_bNaturalForceChanged ), m_iNowNaturalForceCount( rhs_.m_iNowNaturalForceCount), 
		m_iMaxNaturalForceCount( rhs_.m_iMaxNaturalForceCount )
	{}

	void operator=( const CX2RenaGageData& rhs_ )
	{
		CX2GageData::operator =( rhs_ );
		m_bNFBuffMode = rhs_.m_bNFBuffMode;
		m_bNFBuffModeChange = rhs_.m_bNFBuffModeChange;
		m_bNaturalForceChanged = rhs_.m_bNaturalForceChanged;
		m_iNowNaturalForceCount = rhs_.m_iNowNaturalForceCount;
		m_iMaxNaturalForceCount = rhs_.m_iMaxNaturalForceCount;
	}
		
	bool GetNFBuffMode() const;
	void SetNFBuffMode( const bool bNFBuffMode_ );

	bool GetNaturalForceChanged() const;
	void SetNaturalForceChanged( const bool bNaturalForceChanged_ );

	int GetNowNaturalForce() const;
	void SetNowNaturalForce( const int iNowNaturalForceCount_ );

	int GetMaxNaturalForce() const;
	void SetMaxNaturalForce( const int iMaxNaturalForceCount_ );

	bool IsFullNaturalForce() const;
	bool IsEmptyNaturalForce() const;
	
	virtual void	UpdateDataFromGameUnit( const CX2GageData* pGageData_ );
	virtual void	InitWhenGameIsOver();

	virtual bool	GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const;
	virtual void	SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus );

	/// 현재 GageData의 복사본을 생성 (파생 클래스 마다 서로의 복사본을 생성 하도록 하기 위해 순수가상으로 만듦)
	virtual CX2GageData* GetCloneGageData();
	virtual void CopyGageData( IN CX2GageData* pGageData_ );

private:
	bool	m_bNFBuffMode;				/// 자연의 기운 버프.rhs_
	bool	m_bNFBuffModeChange;		/// 자연의 기운 버프가 적용되었는가?
	bool	m_bNaturalForceChanged;		/// 자연의 기운이 변경되었는가?

	KProtectedType<int>	m_iNowNaturalForceCount;	/// 현재 자연의 기운 수
	KProtectedType<int>	m_iMaxNaturalForceCount;	/// 최대 자연의 기운 수
};
#endif //ADD_RENA_SYSTEM