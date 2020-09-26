/** @file : X2BuffTemplet.h
    @breif : 버프의 행동과 종료등을 가지고 있는 클래스 정의 파일
*/

class CX2BuffTemplet;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BuffTemplet> CX2BuffTempletPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BuffTemplet> CX2BuffTempletPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#pragma once

/** @class : CX2BuffTemplet
	@brief : 버프의 행동과 종료등을 가지고 있는 클래스
	@date : 2012/7/17/
*/
class CX2BuffTemplet
{
public:
	static CX2BuffTempletPtr CreateBuffTempletPtr() { return CX2BuffTempletPtr( new CX2BuffTemplet ); }

public:
	enum UN_COMMON_MEMBER		/// 모든 버프에 공통으로 적용되는 변수가 아닌 것을 정의 하기 위한 enum
	{
		UCM_ACCUMULATION_COUNT_NOW = 0,			/// 현재 중첩 카운트 (중첩가능할 시에 현재 몇개만큼 중첩이 되었는가)
		UCM_ACCUMULATION_MULTIPLIER,		/// 중첩시 적용되는 배율
		UCM_REGIST_TYPE,					/// 저항에 영향을 받을 때 어떤 저항에 영향을 받는지 지정(CX2EnchantItem::ENCHANT_TYPE)
	};

	CX2BuffTemplet( const CX2BuffTemplet& rhs_ ) 
		: m_BuffIdentity( rhs_.m_BuffIdentity ), m_uiAccumulationLimit( rhs_.m_uiAccumulationLimit )
		, m_bDeBuff( rhs_.m_bDeBuff ), m_bDidFinish( rhs_.m_bDidFinish ), m_bWorldBuff( rhs_.m_bWorldBuff )
#ifdef BUFF_ICON_UI
		, m_wstIconFileName ( rhs_.m_wstIconFileName ), m_wstIconKeyName ( rhs_.m_wstIconKeyName )
		, m_iBuffName( rhs_.m_iBuffName ), m_iBuffDesc( rhs_.m_iBuffDesc ), m_bUseBuffIcon ( rhs_.m_bUseBuffIcon )
#endif //BUFF_ICON_UI
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{
		m_vecBehaviorPtr.clear();
		m_vecBehaviorPtr.reserve( rhs_.m_vecBehaviorPtr.size() );
		BOOST_FOREACH( CX2BuffBehaviorTempletPtr templet, rhs_.m_vecBehaviorPtr )
		{
			m_vecBehaviorPtr.push_back( templet->GetClonePtr() );
		}

		m_vecFinalizerPtr.clear();
		m_vecFinalizerPtr.reserve( rhs_.m_vecFinalizerPtr.size() );
		BOOST_FOREACH( CX2BuffFinalizerTempletPtr templet, rhs_.m_vecFinalizerPtr )
		{
			m_vecFinalizerPtr.push_back( templet->GetClonePtr() );
		}

		m_vecDisplayerPtr.clear();
		m_vecDisplayerPtr.reserve( rhs_.m_vecDisplayerPtr.size() );
		BOOST_FOREACH( CX2BuffDisplayerTempletPtr templet, rhs_.m_vecDisplayerPtr )
		{
			m_vecDisplayerPtr.push_back( templet->GetClonePtr() );
		}

		m_mapUnCommonMember.clear();
		m_mapUnCommonMember = rhs_.m_mapUnCommonMember;
	}

	CX2BuffTempletPtr GetClonePtr() const { return CX2BuffTempletPtr( new CX2BuffTemplet( *this ) ); }

	bool ParsingScript( KLuaManager& luaManager_ );
	bool SetFactorToOnlyFinalizer( const CX2BuffFactor& buffFactor_, CX2GameUnit* pGameUnit_ );
	bool SetFactor( const CX2BuffFactor& buffFactor_, CX2GameUnit* pGameUnit_ );
	bool SetFactorFromPacket( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime )
	{
		OnFrameMoveBehavior( pGameUnit_, fElapsedTime );
		OnFrameMoveFinalizerAndCheckFinish( pGameUnit_, fElapsedTime );
		OnFrameMoveDisplayer( pGameUnit_, fElapsedTime );
	}
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void OnFrameMove( CX2GameUnit* pGameUnit_ )
	{
		OnFrameMoveBehavior( pGameUnit_ );
		OnFrameMoveFinalizerAndCheckFinish( pGameUnit_ );
		OnFrameMoveDisplayer( pGameUnit_ );
	}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	
	void DoFinishBehavior( CX2GameUnit* pGameUnit_ );
	void DoFinishDisplayer( CX2GameUnit* pGameUnit_ );

	void ReserveToFinish();

	BUFF_TEMPLET_ID GetBuffTempletID() const { return static_cast<BUFF_TEMPLET_ID>( m_BuffIdentity.m_eBuffTempletID ); }
	//void SetBuffTempletID(BUFF_TEMPLET_ID val) { m_eBuffTempletID = val; }

	UINT GetAccumulationLimit() const { return m_uiAccumulationLimit; }
	//void SetAccumulationLimit(UINT val) { m_uiAccumulationLimit = val; }
	
	bool IsWorldBuff() const { return m_bWorldBuff; }
	bool IsDeBuff() const { return m_bDeBuff; }
	//void DeBuff(bool val) { m_bDeBuff = val; }
	
	UINT GetAccumulationCountNow() const;
	void IncreaseAccumulationCountNow();

	float GetAccumulationMultiflier() const;

	UINT GetRegistType() const;

	bool GetDidFinish() const { return m_bDidFinish; }
	void SetDidFinish(bool val) { m_bDidFinish = val; }

	/// 중복이 가능한 버프 인 경우에 구분할 수 있는 숫자(카운트)
	UINT GetUniqueNum() const { return m_BuffIdentity.m_uiUniqueNum; }
	void SetUniqueNum( const UINT uiUniqueNum_ ) { m_BuffIdentity.m_uiUniqueNum = uiUniqueNum_; }

	const KBuffIdentity&	GetBuffIdentity() const { return m_BuffIdentity; }

	void GetFactor( OUT KBuffFactor& kBuffFactor_, const CX2GameUnit* pGameUnit_ ) const;

#ifdef BUFF_ICON_UI
	const wstring& GetIconFileName() const { return m_wstIconFileName; }
	const wstring& GetIconKeyName() const { return m_wstIconKeyName; }
	const int GetBuffName() const { return m_iBuffName; }
	const int GetBuffDesc() const { return m_iBuffDesc; }
	const bool GetUseBuffIcon() const { return m_bUseBuffIcon; }
#endif //BUFF_ICON_UI

	void GetFinalizerTempletPtrList( OUT vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ );
	void ChangeFinalizerTempletPtrList( const vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ );

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 행동 불능 디버프 인가?, 특정 BBT 를 가지고 있으면 참을 반환
	bool	IsNonControlDebuff();
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

protected:
	CX2BuffTemplet() : m_BuffIdentity(), m_uiAccumulationLimit( 0 ), m_bDeBuff( false )
						,m_bDidFinish( false ),	m_bWorldBuff( false )
#ifdef BUFF_ICON_UI
						,m_iBuffName( -1 ), m_iBuffDesc( -1 ), m_bUseBuffIcon ( false )
#endif //BUFF_ICON_UI
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	{
		m_mapUnCommonMember.clear();
	}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    CX2BuffTemplet& operator = ( const CX2BuffTemplet& );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void OnFrameMoveBehavior( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
	void OnFrameMoveFinalizerAndCheckFinish( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
	void OnFrameMoveDisplayer( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void OnFrameMoveBehavior( CX2GameUnit* pGameUnit_ );
	void OnFrameMoveFinalizerAndCheckFinish( CX2GameUnit* pGameUnit_ );
	void OnFrameMoveDisplayer( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	vector<CX2BuffBehaviorTempletPtr>		m_vecBehaviorPtr;
	vector<CX2BuffFinalizerTempletPtr>		m_vecFinalizerPtr;
	vector<CX2BuffDisplayerTempletPtr>		m_vecDisplayerPtr;
	KBuffIdentity							m_BuffIdentity;
	UINT									m_uiAccumulationLimit;		/// 중첩 가능 횟수로 0은 중복(같은 버프가 걸릴수있음), 1은 교체(새로운 버프로 교체), 2 이상 부터는 해당 횟수 만큼 중첩 가능(중첩 배율에 따라 산정)

	typedef map<UN_COMMON_MEMBER, float>	MAP_UN_COMMON_MEMBER;
	MAP_UN_COMMON_MEMBER					m_mapUnCommonMember;		/// 모든 버프에 공통으로 적용되는 변수가 아닌 것을 정의 하기 위한 Map

	bool									m_bDeBuff;					/// 해로운 효과를 주는 디버프인 경우 true
	bool									m_bDidFinish;				/// 종료된 버프 인가?
	bool									m_bWorldBuff;				/// 월드 버프인 경우 true

#ifdef BUFF_ICON_UI
	wstring									m_wstIconFileName;
	wstring									m_wstIconKeyName;
	int										m_iBuffName;
	int										m_iBuffDesc;
	bool									m_bUseBuffIcon;				/// 버프 아이콘 사용 여부
#endif //BUFF_ICON_UI

	
};

IMPLEMENT_INTRUSIVE_PTR( CX2BuffTemplet );


////////////////////////////////////////////////////////////////////////////////////////////////

/** @class : CX2BuffIdentity
	@brief : KBuffIdentity 를 가지며 해당 정보가 같은지를 비교하는 클래스
	@date : 2012/7/22/
*/
class CX2BuffIdentity;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BuffIdentity> CX2BuffIdentityPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BuffIdentity> CX2BuffIdentityPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2BuffIdentity
{
public:
	CX2BuffIdentity( const KBuffIdentity& BuffIdentity_ )
		: m_DataBuffIdentity( BuffIdentity_ )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
			
	virtual ~CX2BuffIdentity() {}

	/// 비교하고자 하는 버프에 의해 발생된 효과인지 확인
	bool IsSame( const KBuffIdentity& BuffIdentity_ ) const
	{
		if ( m_DataBuffIdentity.m_eBuffTempletID == BuffIdentity_.m_eBuffTempletID 
			&& m_DataBuffIdentity.m_uiUniqueNum == BuffIdentity_.m_uiUniqueNum )
			return true;
		else
			return false;
	}

#ifdef VERIFY_STAT_BY_BUFF
	virtual bool Verify() { return true; }
#endif // VERIFY_STAT_BY_BUFF

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

protected:
	CX2BuffIdentity()
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    CX2BuffIdentity& operator = ( const CX2BuffIdentity& );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR



protected:
	KBuffIdentity		m_DataBuffIdentity;		/// 어떤 버프에 의한 효과 인지

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
private:
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

};

IMPLEMENT_INTRUSIVE_PTR( CX2BuffIdentity );


/** @function : FindSameBuffIdentity
	@brief : CX2BuffIdentity를 상속받는 클래스를 담는 스마트 포인터 벡터 중 버프정보가 동일한 요소를 찾는 함수
	@param : 스마트포인터벡터(vector_), 버프정보(BuffIdentity_)
	@return : 
*/
template<typename ITR, typename T>
ITR FindSameBuffIdentity( IN vector<T>& vector_, const KBuffIdentity& BuffIdentity_ )
{
	if ( !vector_.empty() )
	{
		auto FindFunc = [&BuffIdentity_]( T ptr_ ) {
			return ptr_->IsSame( BuffIdentity_ );
		};

		vector<T>::iterator vItr = std::find_if( vector_.begin(), vector_.end(), FindFunc );
		return vItr;
	}
	else
		return vector_.end();
}

/** @class : CX2ReverseLefRight
	@brief : 좌우반전 효과 클래스
	@date : 2012/7/22/
*/
class CX2ReverseLefRight;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2ReverseLefRight> CX2ReverseLefRightPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2ReverseLefRight> CX2ReverseLefRightPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2ReverseLefRight : public CX2BuffIdentity
{
public:
	CX2ReverseLefRight( const KBuffIdentity& BuffIdentity_ ) 
		: CX2BuffIdentity( BuffIdentity_ ),
		m_bReverse( true )	/// 시작하자 마자 좌우반전 효과를 주기 위해 true로 셋팅
	{}

	/** @function : GetReverse
		@brief : 반전이 적용 중인가?
		@return : 반전중이면 true
	*/
	bool GetReverse() const { return m_bReverse; }

	/// 좌우반전 토글
	void ToggleReverseLeftRightByBuff() { m_bReverse = !m_bReverse; }

protected:
	CX2ReverseLefRight() {}

private:
	bool				m_bReverse;				/// true면 현재 프레임에 좌우반전 중이어야 함
};

/** @class : CX2NeverMoveByBuff
	@brief : 현재 위치에 고정시키는 클래스
	@date : 2012/9/6/
*/
class CX2NeverMoveByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2NeverMoveByBuff> CX2NeverMoveByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2NeverMoveByBuff> CX2NeverMoveByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2NeverMoveByBuff : public CX2BuffIdentity
{
public:
#ifdef NEVER_MOVE_GRAVITY_APPLY
	CX2NeverMoveByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vPos_, const bool vGravity_ ) 
		: CX2BuffIdentity( BuffIdentity_ ), m_vPos( vPos_ ), m_bIsGravity( vGravity_ )
	{}
#else
	CX2NeverMoveByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vPos_ ) 
		: CX2BuffIdentity( BuffIdentity_ ), m_vPos( vPos_ )
	{}
#endif //NEVER_MOVE_GRAVITY_APPLY

	const D3DXVECTOR3& GetPos() const { return m_vPos; }
#ifdef NEVER_MOVE_GRAVITY_APPLY
	const bool		   GetGravitiApply() const { return m_bIsGravity; }
#endif //NEVER_MOVE_GRAVITY_APPLY

protected:
	CX2NeverMoveByBuff() 
#ifdef NEVER_MOVE_GRAVITY_APPLY
		: CX2BuffIdentity(), m_vPos( 0.0f, 0.0f, 0.0f )	, m_bIsGravity( false )
#else
		: CX2BuffIdentity(), m_vPos( 0.0f, 0.0f, 0.0f )
#endif //NEVER_MOVE_GRAVITY_APPLY
	{}

private:
	D3DXVECTOR3		m_vPos;
#ifdef NEVER_MOVE_GRAVITY_APPLY
	bool			m_bIsGravity;	//중력을 적용할것인가. true면 중력 적용, false면 위치 고정
#endif //NEVER_MOVE_GRAVITY_APPLY

};

/** @class : CX2RenderParamByBuff
	@brief : 버프에 의해서 변경될수 있는 RenderParam 값에 BuffIdentity를 결합하여 구분할 수 있게한 클래스
	@date : 2012/8/3/
*/

class CX2RenderParamByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2RenderParamByBuff> CX2RenderParamByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2RenderParamByBuff> CX2RenderParamByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2RenderParamByBuff : public CX2BuffIdentity
{
public:
	CX2RenderParamByBuff( const KBuffIdentity& BuffIdentity_, const StBuffRenderParam& stBuffRenderParam_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_stBuffRenderParam( stBuffRenderParam_ )
	{}

	const StBuffRenderParam& GetBuffRenderParam() const { return m_stBuffRenderParam; }

private:
	StBuffRenderParam	m_stBuffRenderParam;
};

/** @class : CX2EffectSetImpactPointByBuff
	@brief : 타격시 타격 포인트에 이팩트 셋을 생성하는 기능을 BuffIdentity와 결합하여 구분할 수 있게 한 클래스
	@date : 2012/8/20/
*/
class CX2EffectSetImpactPointByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2EffectSetImpactPointByBuff> CX2EffectSetImpactPointByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2EffectSetImpactPointByBuff> CX2EffectSetImpactPointByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2EffectSetImpactPointByBuff : public CX2BuffIdentity
{
public:
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09		생성 간격 추가
	CX2EffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCreateGap )
		: CX2BuffIdentity( BuffIdentity_ ), m_wstrEffectSetName( pwszEffectSetName_ ), m_wstrSoundName( pwszSoundName_ ), m_fCreateGap( fCreateGap ), m_fNowCoolTime( 0.f )
		{}
#else // SERV_NEW_DEFENCE_DUNGEON
	CX2EffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_wstrEffectSetName( pwszEffectSetName_ ), m_wstrSoundName( pwszSoundName_ )
		{}
#endif // SERV_NEW_DEFENCE_DUNGEON

	void PlayEffectSetImpactPointByBuff( CX2GameUnit* pGameUnit_, const D3DXVECTOR3& vImpactPos_ );
	void PlayEffectSetHittedByBuff( CX2GameUnit* pGameUnit_ );

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09	
	void PlayEffectSetNormalAttackByBuff( CX2GameUnit* pGameUnit_, const CX2DamageManager::ATTACK_TYPE eAttackType );	/// 일반 공격 타격시 이펙트 셋 생성 함수
	void ProcessCoolTime( const float fElapsedTime );		/// 생성 간격 연산 함수
#endif // SERV_NEW_DEFENCE_DUNGEON
	
protected:
	inline void PlaySound( CX2GameUnit* pGameUnit_ );

private:
	wstring	m_wstrEffectSetName;
	wstring m_wstrSoundName;
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09
	const float m_fCreateGap;		/// 생성 간격
	float m_fNowCoolTime;			/// 생성 간격 처리용 쿨타임
#endif // SERV_NEW_DEFENCE_DUNGEON
};

/** @class : CX2ChangeUnitScaleByBuff
	@brief : 유닛의 Scale을 변경하는 기능을 BuffIdentity와 결합하여 구분할 수 있게 한 클래스
	@date : 2012/8/20/
*/
class CX2ChangeUnitScaleByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2ChangeUnitScaleByBuff> CX2ChangeUnitScaleByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2ChangeUnitScaleByBuff> CX2ChangeUnitScaleByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2ChangeUnitScaleByBuff : public CX2BuffIdentity
{
public:
	CX2ChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vScale_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_vScale( vScale_ )
	{}

#ifdef VERIFY_STAT_BY_BUFF
	const PROTECT_VECTOR3& GetScale() const { return m_vScale; }
#else	// VERIFY_STAT_BY_BUFF
	const D3DXVECTOR3& GetScale() const { return m_vScale; }
#endif // VERIFY_STAT_BY_BUFF
#ifdef POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE
	void	SetScale( const D3DXVECTOR3& vScale_ ) { m_vScale = vScale_; }
	
#ifdef VERIFY_STAT_BY_BUFF
	void	SetScale( const PROTECT_VECTOR3& vScale_ ) { m_vScale = vScale_; }	
#endif // VERIFY_STAT_BY_BUFF

#endif POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE

#ifdef VERIFY_STAT_BY_BUFF
	virtual bool Verify() { return m_vScale.Verify(); }
#endif // VERIFY_STAT_BY_BUFF

private:
	
#ifdef VERIFY_STAT_BY_BUFF
	PROTECT_VECTOR3	m_vScale;
#else	// VERIFY_STAT_BY_BUFF
	D3DXVECTOR3 m_vScale;
#endif // VERIFY_STAT_BY_BUFF
};

/** @class : CX2AfterImageByBuff
	@brief : 유닛의 AfterImage를 On 하는 기능을 BuffIdentity와 결합한 클래스
	@date : 2012/8/28/
*/
class CX2AfterImageByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2AfterImageByBuff> CX2AfterImageByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2AfterImageByBuff> CX2AfterImageByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2AfterImageByBuff : public CX2BuffIdentity
{
public:
	CX2AfterImageByBuff( const KBuffIdentity& BuffIdentity_, const D3DXCOLOR& d3dxColor_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_d3dxColor( d3dxColor_ )
	{}

	const D3DXCOLOR& GetColor() const { return m_d3dxColor; }

private:
	D3DXCOLOR	m_d3dxColor;
};

#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
/** @class : CX2WeaponAfterImageByBuff
	@brief : 유닛의 무기 AfterImage를 On 하는 기능을 BuffIdentity와 결합한 클래스
	@date : 2012/8/28/
*/
class CX2WeaponAfterImageByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2WeaponAfterImageByBuff> CX2WeaponAfterImageByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2WeaponAfterImageByBuff> CX2WeaponAfterImageByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2WeaponAfterImageByBuff : public CX2BuffIdentity
{
public:
	CX2WeaponAfterImageByBuff( const KBuffIdentity& BuffIdentity_, const D3DXCOLOR& d3dxColor_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_d3dxColor( d3dxColor_ )
	{}

	const D3DXCOLOR& GetColor() const { return m_d3dxColor; }

private:
	D3DXCOLOR	m_d3dxColor;
};
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

/** @class : CX2CustomFunctionByBuff
	@brief : 버프에 의한 행동 중CommonStateStart, CommonFrameMove, CommonStateEnd 에서 처리되야 하는 하드코딩 함수를 수행하도록 처리 되도록 하는 클래스
	@date : 2012/8/21/
*/
class CX2CustomFunctionByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2CustomFunctionByBuff> CX2CustomFunctionByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2CustomFunctionByBuff> CX2CustomFunctionByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2CustomFunctionByBuff : public CX2BuffIdentity
{
public:
	CX2CustomFunctionByBuff( const KBuffIdentity& BuffIdentity_ )
		: CX2BuffIdentity( BuffIdentity_ )
	{}
	virtual ~CX2CustomFunctionByBuff() {}

	virtual void FunctionInCommonStateStart( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ ) {};
	virtual void FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ ) {};
	virtual void FunctionInCommonStateEnd( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ ) {};
	virtual void FunctionInDamageReact( CX2GameUnit* pGameUnit_, IN CX2DamageManager::DamageData& damageData_ ) {}
};

/** @class : CX2CustomFunctionByBuffPhantomSword
	@brief : 팬텀소드 버프 클래스
	@date : 2012/8/21/
	*/
class CX2CustomFunctionByBuffPhantomSword : public CX2CustomFunctionByBuff
{
public:
	CX2CustomFunctionByBuffPhantomSword( const KBuffIdentity& BuffIdentity_, CX2GameUnit* pGameUnit_ );
	virtual ~CX2CustomFunctionByBuffPhantomSword();

	virtual void FunctionInCommonStateStart( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ );
	virtual void FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ );
	virtual void FunctionInCommonStateEnd( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ );

private:
	void InitializeDamageEffectThridBlade( CX2GameUnit* pGameUnit_ );
	void InitializeMeshThirdBaldeCenter( CX2GameUnit* pGameUnit_ );
	void CreateAndInsertThirdBladeData( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ );
	void MoveDataDataToDamageEffect( CX2GameUnit* pGameUnit_
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            , CX2DamageEffect::CEffect*   m_pDamageEffectThirdBlade
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        );

private:
	struct ThirdBladeMatrix
	{
		D3DXMATRIX		PastCombineMatrix;
		float			fPastTime;
		ThirdBladeMatrix()
		{
			fPastTime	= 0.f;
		}
	};
	typedef boost::shared_ptr<ThirdBladeMatrix> ThirdBladeMatrixPtr;

	struct ThirdBladeDamageData
	{
		CX2DamageManager::DamageData	m_DamageData;
		D3DXVECTOR2						m_vAttackTime;	/// x는 Start, y는 End값
		
		ThirdBladeDamageData() : m_DamageData(), m_vAttackTime( 0.0f, 0.0f )
		{
		}
	};
	typedef boost::shared_ptr<ThirdBladeDamageData> ThirdBladeDamageDataPtr;

	const float MAGIC_PHANTOM_SWORD_DELAY;			/// 팬텀소드 딜레이
	const float MAGIC_PHANTOM_DAMAGE_MULTIPLIER;	/// 팬텀소드 1.2배 데미지
	const float MAGIC_PHANTOM_STOP_TIME_DEF;		/// 팬텀소드에 맞았을 때 스탑되는 시간
	const float MAGIC_MAX_COUNT_CATCH_UP;			/// 팬텀소드가 한타임 늦게 따라가도록 하려는 건..가?

	float											m_fThirdBladeTime;				/// 팬텀소드 현재까지 진행된 시간 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffectHandle                  m_hDamageEffectThirdBlade;		/// 팬텀소드 데미지이펙트
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect*						m_pDamageEffectThirdBlade;		/// 팬텀소드 데미지이펙트
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hThirdBladeCenterMesh;		/// 이 메시 이미지는...?
	//CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hThirdBladeSheathMesh;

 	std::queue<ThirdBladeMatrixPtr>					m_ThirdBladeMatrixQueue;		/// 팬텀소드 움직임 담당 큐
 	std::list<ThirdBladeDamageDataPtr>				m_ThirdBladeDamageDataList;		/// 팬텀소드 데미지이펙트에 적용될 데미지 list

	string											m_strPhantomBladeDamageDataNameUTF8;	/// 팬텀소드 데미지데이터가 따로 있는 경우, 적용할 데미지데이타 테이블 명
	int												m_iPhantomBladeAttackTimeIndex;		/// 팬텀소드가 적용될 공격 타입
	
	D3DXVECTOR2										m_vPhantomBladeBackSpeed;	/// 팬텀소드 백스피드
	CX2DamageManager::REACT_TYPE					m_PhantomBladeReActType;	/// 팬텀소드의 ReactType
};

/** @class : CX2CustomFunctionByBuffBigBangStream
	@brief : 빅뱅스트림 버프 클래스
	@date : 2012/8/30/
	*/
class CX2CustomFunctionByBuffBigBangStream : public CX2CustomFunctionByBuff
{
public:
	CX2CustomFunctionByBuffBigBangStream( const KBuffIdentity& BuffIdentity_, CX2GameUnit* pGameUnit_ )
		: CX2CustomFunctionByBuff( BuffIdentity_ ), m_vStateStartPos( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) )
	{ m_vecAnimationTime.clear(); }

	~CX2CustomFunctionByBuffBigBangStream()
	{
		m_vecAnimationTime.clear();
		m_vecAnimationTime.swap( vector<float>() );
	}
	virtual void FunctionInCommonStateStart( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ );
	virtual void FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ );
	
private:
	virtual void DoExtraAttack( CX2GameUnit* pGameUnit_ );

private:
	vector<float>	m_vecAnimationTime;
	D3DXVECTOR3		m_vStateStartPos;
};

/** @class : CX2CustomFunctionByBuffMarkOfCommand
	@brief : 지휘관의 표버프 클래스
	@date : 2012/8/30/
	*/
class CX2CustomFunctionByBuffMarkOfCommand : public CX2CustomFunctionByBuff
{
public:
	CX2CustomFunctionByBuffMarkOfCommand( const KBuffIdentity& BuffIdentity_, const KBuffBehaviorFactor kBuffBehaviorFactor_ )
		: CX2CustomFunctionByBuff( BuffIdentity_ ), m_uiDamageEffectCount( 0 ), m_checkElapsedTime( 1.5f )
	{
		m_fBasicPosX[0] = -1000.0f;
		m_fBasicPosX[1] = -900.0f;
		m_fBasicPosX[2] = -1100.0f;
		m_fBasicPosX[3] = -800.0f;
		m_fBasicPosX[4] = -1200.0f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
		/// 데미지 이펙트를 출력할 갯수를 얻어옴
		m_uiDamageEffectCount = 3;
		m_fAddPowerRate = static_cast<float>( kBuffBehaviorFactor_[1] );
		
		m_uiDamageEffectCount = min( 5, m_uiDamageEffectCount );
		m_fMemoRate = kBuffBehaviorFactor_[0];	/// 없으면 0.0이 대입될 것이다.
#else // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
		m_uiDamageEffectCount = static_cast<UINT>( kBuffBehaviorFactor_[0] );
		m_uiDamageEffectCount = min( 5, m_uiDamageEffectCount );
		m_fMemoRate = kBuffBehaviorFactor_[1];	/// 없으면 0.0이 대입될 것이다.
#endif // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
	}

	~CX2CustomFunctionByBuffMarkOfCommand()
	{}
	
	virtual void FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ );

	virtual void FunctionInDamageReact( CX2GameUnit* pGameUnit_, IN CX2DamageManager::DamageData& damageData_ );
	
private:
	void DoMarkOfCommand( CX2GameUnit* pGameUnit_, IN CX2DamageManager::DamageData& damageData_ );

private:
	UINT	m_uiDamageEffectCount;				/// 발사할 포탄 갯수
	CKTDXCheckElapsedTime	m_checkElapsedTime;	/// 아무리 빨라도 이간견 안으로는 다시 발사하지 않음	
	float m_fBasicPosX[5];
	float m_fMemoRate;			/// 지휘관의 표식 메모를 장착했을 때의 확률

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
	float m_fAddPowerRate;
#endif // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
};

// /** @class : CX2CustomFunctionByBuffMagicalMakeUp
// 	@brief : 매지컬메이크업 처리 클래스
// 	@date : 2012/8/23/
// */
// class CX2CustomFunctionByBuffMagicalMakeUp : public CX2CustomFunctionByBuff
// {
// public:
// 	CX2CustomFunctionByBuffMagicalMakeUp( const KBuffIdentity& BuffIdentity_, CX2GameUnit* pGameUnit_ );
// 	virtual ~CX2CustomFunctionByBuffMagicalMakeUp();
// 
// 	virtual void FunctionInCommonStateStart( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ ) {};
// 	virtual void FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ );
// 	virtual void FunctionInCommonStateEnd( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ ) {};
// 
// private:
// 	const int	MAGICAL_MAKE_UP_WEAPON_ID;
// 	CX2EqipPtr	m_ptrEquipTransform;
// 	bool		m_bNeedToUpdateWeaponEffect;
// 	bool		m_bNotifiedDGManagerChanged;
// };

/** @class : C2StatDataByBuff
	@brief : 버프에 의한 스탯 증가시 사용
	@date : 2012/7/23/
*/
class CX2StatFactorByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2StatFactorByBuff> CX2StatFactorByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2StatFactorByBuff> CX2StatFactorByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2StatFactorByBuff : public CX2BuffIdentity
{
public:
	CX2StatFactorByBuff( const KBuffIdentity& BuffIdentity_, const float fValue_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_fValue( fValue_ )
	{}

	float GetValue() const { return m_fValue; }
	void SetValue(float val) { m_fValue = val; }

#ifdef VERIFY_STAT_BY_BUFF
	virtual bool Verify() { return m_fValue.Verify(); }
#endif // VERIFY_STAT_BY_BUFF

private:
	KProtectedType<float>		m_fValue;				/// 스탯의 종류에 따라 어떤 값이든 될 수 있음
};

/** @class : CX2AdditionalStatByBuff
	@brief : 버프에 의해 추가되는 스탯정보를 담는 클래스
	@date : 2012/8/6/
*/
class CX2AdditionalStatByBuff
{
public:
	CX2AdditionalStatByBuff() : m_fBasicStatValue( 0.0f ),
		 /*합의항등원은 0 */m_fPassiveSum( 0.0f ), /*곱의항등원은 1 */m_fPassiveMultiple( 1.0f ),
		m_fOptionValue( 0.0f ), m_fResultValue( 0.0f ), m_bMustChangeResultValue( false )
	{
		for ( UINT i = 0; i < BCT_END; ++i )
		{
			m_vecStatFactorArray[i].clear();
			m_fArrayResultValueByChangeType[i] = 0.0f;
		}
		m_fArrayResultValueByChangeType[BCT_PERCENT] = 1.0f;
		
		m_bitsetMustChange.reset();		
	}

	~CX2AdditionalStatByBuff()
	{
		for ( UINT i = 0; i < BCT_END; ++i )
		{
			m_vecStatFactorArray[i].clear();
			m_vecStatFactorArray[i].swap( vector<CX2StatFactorByBuffPtr>() );
		}
	}

	float GetBasicStatValue() const { return m_fBasicStatValue; }
	void SetBasicStatValue( const float fValue_ );

	float GetPassiveSum() const { return m_fPassiveSum; }
	void SetPassiveSum( const float fValue_ );

	float GetPassiveMultiple() const { return m_fPassiveMultiple; }
	void SetPassiveMultiple(const float fValue_);

	float GetOptionValue() const { return m_fOptionValue; }
	void SetOptionValue( const float fValue_ );

	float GetResultValue() const { return m_fResultValue; }
	bool GetMustChangeResultValue() const { return m_bMustChangeResultValue; }
	
	void UpdateResultValueByChangeType();

	void UpdateResultValue();			/// 기본스탯 계산용
	void CheckAndUpdateResultValue();	/// 기본스탯 계산용

	template<typename Func>
	void UpdateResultValueWithOptionData( Func func_ptr, const int iUserLevel_, const float fModifier_ = 1.0f );	/// 확장스탯 계산용

	template<typename Func>
	void CheckAndUpdateResultValueWithOptionData( Func func_ptr, const int iUserLevel_, const float fModifier_ = 1.0f );	/// 확장스탯 계산용(옵션수치화 적용)

	void CreateAndInsertStatFactor( const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fStatValue_ );
	bool UpdateAddtionalStatFactor( const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fValue_ );
	void EraseStatFactor( const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_ );
	
#ifdef VERIFY_STAT_BY_BUFF
	bool Verify();
#endif // VERIFY_STAT_BY_BUFF

private:
	void SetResultValue(float val) { m_fResultValue = val; }
	void SetMustChangeResultValue(bool val) { m_bMustChangeResultValue = val; }

#ifdef VERIFY_STAT_BY_BUFF
	bool VerifyStatFactor();
	bool VerifyArrayResult();
#endif // VERIFY_STAT_BY_BUFF

private:
	KProtectedType<float>					m_fBasicStatValue;		/// 서버로 부터 전달받은 또는 스크립트로부터 읽은 스탯 기본값(기본스탯+장비+패시브스킬)
	KProtectedType<float>					m_fPassiveSum;			/// 패시브스킬중 스탯 합배율
	KProtectedType<float>					m_fPassiveMultiple;		/// 패시브스킬중 스탯 곱배율
	KProtectedType<float>					m_fOptionValue;			/// 소켓옵션에 의한 값(일단, 체력, 공격력, 방어력등 스탯에서는 사용하지 않음)
	vector<CX2StatFactorByBuffPtr>			m_vecStatFactorArray[BCT_END];	/// 0: 스탯의 치환, 1: 고정수치, 2: 연동수치, 3: 배율
	KProtectedType<float>					m_fArrayResultValueByChangeType[BCT_END];	/// 각 ChangeType 별 결과값
	
	KProtectedType<float>					m_fResultValue;				/// 모든 ChangeType을 고려한 결과값(위의 리스트들을 고려한)
	std::bitset<BCT_END>					m_bitsetMustChange;			/// 각 ChangeType의 결과값이 변경되야 하는지여부
	bool									m_bMustChangeResultValue;	/// m_fResultValue를 다시 구해야 하는 경우 true

};

/** @function : UpdateResultValueWithOptionData
	@brief : 옵션수치화와 관련된 스탯에 대한 변경 결과값을 갱신하는 함수
	@param : 옵션수치화함수포인터(func_ptr), 유닛레벨(iUserLevel_)
*/
template<typename Func>
void CX2AdditionalStatByBuff::UpdateResultValueWithOptionData( Func func_ptr, const int iUserLevel_, const float fModifier_ /*= 1.0f*/ )
{
	if ( !m_vecStatFactorArray[BCT_SWAP_VALUE].empty() )	/// 치환요소가 있다면
	{
		/// 무조건 치환요소중 제일 나중에 걸린것 적용
		SetResultValue( m_fArrayResultValueByChangeType[BCT_SWAP_VALUE] );
	}
	else	/// 치환요소가 없다면
	{
		float fResultValue = GetBasicStatValue();
		bool bShouldMinus = false;
		/// 기본값이 0.0인것은 일단 1.0을 더 해주고
		if( !( 0.0f < fResultValue ) )
		{
			fResultValue += 1.0f;
			bShouldMinus = true;
		}

		float fIntermediateValue = GetOptionValue();
		fIntermediateValue += m_fArrayResultValueByChangeType[BCT_FIX_VALUE];
		fIntermediateValue += m_fArrayResultValueByChangeType[BCT_RELATION_VALUE];

		float fMultiplier 
			= ( 1 + func_ptr( fIntermediateValue, iUserLevel_ ) ) * ( 1 + GetPassiveSum() ) 
			* GetPassiveMultiple() * m_fArrayResultValueByChangeType[BCT_PERCENT];

		if ( fMultiplier > 0.0f )
		{
#ifdef ADJUST_BUFF_CALCULATE
			//이동, 점프속도 등의 경우 0.5배를 해주는데
			//이 때문에 BattleFieldBuffFactor.lua에서 값을 작게 설정해줘도 최소 0.5이하로 내려가지 않아 디버프쪽의 영향이 미비함.
			//따라서 BattleFieldBuffFactor.lua 값이 1보다 클 경우(버프)에는 기존과 같이 0.5를 곱하고
			//1보다 작을 경우(디버프)일 경우에는 0.5를 곱하지 않음.
			if( m_fArrayResultValueByChangeType[BCT_PERCENT] >= 1.0f )
#endif //ADJUST_BUFF_CALCULATE
			{
				// kimhc // if 문으로 fModifier_ 값을 판단해서 아래 수식을 적용하기 보다는 항상 하는게 더 나을 것이라 판단함
				fMultiplier -= 1.0f;		/// 먼저 1.0을 빼줘서 0.xx 상태로 만든다. (증가량만 구한다)
				fMultiplier *= fModifier_;	/// 그값에 fModifier_ 값을 곱해주고 (이동, 점프속도 등의 경우 0.5배)
				fMultiplier += 1.0f;
			}

			fResultValue *= fMultiplier;
		}

		if ( bShouldMinus )
			fResultValue -= 1.0f;	/// 다시 1.0을 빼준다.

		fResultValue = max( fResultValue, 0.0f );	/// 혹시 마이너스인 경우에는 최소값을 0으로 설정
		
		SetResultValue( fResultValue );
	}
	SetMustChangeResultValue( false );
}

/** @function : CheckAndUpdateResultValueWithOptionData
	@brief : 옵션수치화와 관련한 스탯에 대하여 변경되었는지 여부를 판단하고 갱신을 수행하게 하는 함수
	@param : 옵션수치화함수포인터(func_ptr), 유닛레벨(iUserLevel_)
*/
template<typename Func>
void CX2AdditionalStatByBuff::CheckAndUpdateResultValueWithOptionData( Func func_ptr, const int iUserLevel_, const float fModifier_ /*= 1.0f*/ )
{
	if ( GetMustChangeResultValue() )
	{
		/// 각 타입별 리스트 갱신
		UpdateResultValueByChangeType();

		/// 전체값 갱신
		UpdateResultValueWithOptionData( func_ptr, iUserLevel_, fModifier_ );
	}
}

/** @class : CX2Stun
	@brief : 그로기 스테이트 고정 효과 클래스
	@date : 2012/7/22/
*/
class CX2Stun;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2Stun> CX2StunPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2Stun> CX2StunPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2Stun : public CX2BuffIdentity
{
public:
	CX2Stun( const KBuffIdentity& BuffIdentity_ ) 
		: CX2BuffIdentity( BuffIdentity_ )
	{}

protected:
	CX2Stun() {}

private:
};

/** @class : CX2Freeze
	@brief : 그로기 스테이트 고정 효과 클래스
	@date : 2012/7/22/
*/
class CX2Freeze;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2Freeze> CX2FreezePtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2Freeze> CX2FreezePtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2Freeze : public CX2BuffIdentity
{
public:
	CX2Freeze( const KBuffIdentity& BuffIdentity_ ) 
		: CX2BuffIdentity( BuffIdentity_ )
	{}

protected:
	CX2Freeze() {}

private:
};

/** @class : CX2ChangeAttackByType
	@brief : 공격 타입에 따른 데미지 클래스
	@date : 2012/11/03/
*/
class CX2ChangeAttackByType;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2ChangeAttackByType> CX2ChangeAttackByTypePtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2ChangeAttackByType> CX2ChangeAttackByTypePtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2ChangeAttackByType : public CX2BuffIdentity
{
public:
	CX2ChangeAttackByType( const KBuffIdentity& BuffIdentity_, CX2DamageManager::ATTACK_TYPE eAttackType_, float fValue_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_eAttackType( eAttackType_ ), m_fValue( fValue_ )
	{}

	CX2DamageManager::ATTACK_TYPE GetAttackType() const { return m_eAttackType; }
	float						  GetValue() const { return m_fValue; }

#ifdef VERIFY_STAT_BY_BUFF
	virtual bool	Verify() { return m_fValue.Verify(); }
#endif // VERIFY_STAT_BY_BUFF

private:
	CX2DamageManager::ATTACK_TYPE	m_eAttackType;			///  공격 타입

#ifdef VERIFY_STAT_BY_BUFF
	KProtectedType<float>			m_fValue;				///  데미지 증가치
#else
	float							m_fValue;				///  데미지 증가치
#endif // VERIFY_STAT_BY_BUFF
};

/** @class : CX2ChangeEnchantAttackRate
	@brief : 소켓 속성 공격 발동 확률 클래스
	@date : 2012/11/03/
*/
class CX2ChangeEnchantAttackRate;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2ChangeEnchantAttackRate> CX2ChangeEnchantAttackRatePtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2ChangeEnchantAttackRate> CX2ChangeEnchantAttackRatePtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2ChangeEnchantAttackRate : public CX2BuffIdentity
{
public:
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 인챈트 속성 타입과 변경 타입 추가
	CX2ChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_, float fRate_, BUFF_ENCHANT_ATTRIBUTE_TYPE eType_, BUFF_CHANGE_TYPE eChangeType_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_fRate( fRate_ ), m_eEnchantAttributeType ( eType_ ), m_eBuffChangeType ( eChangeType_)
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	CX2ChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_, float fRate_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_fRate( fRate_ )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	{}

	float			GetRate() const { return m_fRate; }
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	BUFF_ENCHANT_ATTRIBUTE_TYPE	GetBuffEnchantAttributeType () const { return m_eEnchantAttributeType; }
	BUFF_CHANGE_TYPE			GetBuffChangeType() const { return m_eBuffChangeType; }
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#ifdef VERIFY_STAT_BY_BUFF
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 인챈트 속성 타입과 변경 타입에 Verify 추가
	virtual bool	Verify() { return ( m_fRate.Verify() && m_eEnchantAttributeType.Verify() && m_eBuffChangeType.Verify() ); }
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	virtual bool	Verify() { return m_fRate.Verify(); }
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#endif // VERIFY_STAT_BY_BUFF



private:

#ifdef VERIFY_STAT_BY_BUFF
	KProtectedType<float> m_fRate;				///  확률
#else	// VERIFY_STAT_BY_BUFF
	float				 m_fRate;				///  확률
#endif // VERIFY_STAT_BY_BUFF

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	KProtectedType<BUFF_ENCHANT_ATTRIBUTE_TYPE>		m_eEnchantAttributeType;		///  확률
	KProtectedType<BUFF_CHANGE_TYPE>				m_eBuffChangeType;				///  버프 변경 타입 ( BCT_FIX_VALUE, BCT_PERCENT 등.. )
#endif // BALANCE_PATCH_20131107				// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
};

/** @class : CX2DoubleAttackPtr
	@brief : 더블 어택 클래스
	@date : 2012/11/03/
*/
class CX2DoubleAttack;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2DoubleAttack> CX2DoubleAttackPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2DoubleAttack> CX2DoubleAttackPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2DoubleAttack : public CX2BuffIdentity
{
public:
	CX2DoubleAttack( const KBuffIdentity& BuffIdentity_, float fRate_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_fRate( fRate_ )
	{}

	float			GetRate() const { return m_fRate; }

#ifdef VERIFY_STAT_BY_BUFF
	virtual bool	Verify() { return m_fRate.Verify(); }
#endif // VERIFY_STAT_BY_BUFF

private:
	
#ifdef VERIFY_STAT_BY_BUFF
	KProtectedType<float> m_fRate;				///  확률
#else	// VERIFY_STAT_BY_BUFF
	float				m_fRate;				///	확률
#endif // VERIFY_STAT_BY_BUFF
};

/** @class : CX2AddSkillLevelByBuffPtr
	@brief : 더블 어택 클래스
	@date : 2013/04/16/
*/
class CX2AddSkillLevelByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2AddSkillLevelByBuff> CX2AddSkillLevelByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2AddSkillLevelByBuff> CX2AddSkillLevelByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2AddSkillLevelByBuff : public CX2BuffIdentity
{
public:
	CX2AddSkillLevelByBuff( const KBuffIdentity& BuffIdentity_, const int iAddSkillLevel_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_iAddSkillLevel( iAddSkillLevel_ )
	{}

	int				GetAddSkillLevel() const { return m_iAddSkillLevel; }
	void			SetAddSkillLevel(int val) { m_iAddSkillLevel = val; }
	
#ifdef VERIFY_STAT_BY_BUFF
	virtual bool	Verify() { return m_iAddSkillLevel.Verify(); }
#endif // VERIFY_STAT_BY_BUFF

private:
#ifdef VERIFY_STAT_BY_BUFF
	KProtectedType<int> m_iAddSkillLevel;				///  증감할 스킬 레벨
#else	// VERIFY_STAT_BY_BUFF
	int					m_iAddSkillLevel;				///  증감할 스킬 레벨
#endif // VERIFY_STAT_BY_BUFF
};

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환

/** @class : CX2ReflectMagicByBuff
	@brief : 공격 타입에 따른 데미지 클래스
	@date : 2013/07/10/
*/
class CX2ReflectMagicByBuff;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2ReflectMagicByBuff> CX2ReflectMagicByBuffPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2ReflectMagicByBuff> CX2ReflectMagicByBuffPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
class CX2ReflectMagicByBuff : public CX2BuffIdentity
{
public:
	CX2ReflectMagicByBuff( const KBuffIdentity& BuffIdentity_, float fValue_ )
		: CX2BuffIdentity( BuffIdentity_ ), m_fValue( fValue_ )
	{}

	float			GetValue() const { return m_fValue; }
	virtual bool	Verify() { return m_fValue.Verify(); }

private:
	KProtectedType<float>			m_fValue;				/// 데미지 증가치
};

#endif // SERV_ARA_CHANGE_CLASS_SECOND