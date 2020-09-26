/**@file : X2BuffBehaviorTemplet.h
   @breif : 버프의 행동을 정의하는 Templet 파일
*/

#pragma once

class CX2BuffTemplet;
class CX2BuffBehaviorTemplet;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BuffBehaviorTemplet> CX2BuffBehaviorTempletPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BuffBehaviorTemplet> CX2BuffBehaviorTempletPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

/** @class : CX2BuffBehaviorTemplet
	@brief : 버프의 행동 방식을 정의하는 클래스
	@date : 2012/7/16/
*/
class CX2BuffBehaviorTemplet
{
public:
	static bool ParsingCombinationBehavior( KLuaManager& luaManager_, OUT vector<CX2BuffBehaviorTempletPtr>& vecBuffBehaviorTempletPtr_ );
	
	CX2BuffBehaviorTemplet( const CX2BuffBehaviorTemplet& rhs_ ) : m_eType( rhs_.m_eType ), m_bStart( rhs_.m_bStart )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
	virtual ~CX2BuffBehaviorTemplet() {}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ ) {}
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ ) {}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ ) = NULL;
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const = NULL;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const = NULL;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ ) {}
	
	bool SetFactorFromPacketTemplateMothod( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ );
	bool DidStart() const { return m_bStart; }
	void SetStart(bool val) { m_bStart = val; }

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 해당 BBT 타입을 받아와야 할 경우가 있어서 추가, GetType 과 동일
	BUFF_BEHAVIOR_TYPE GetBuffBehaviorType() const { return m_eType; }
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR


protected:
	CX2BuffBehaviorTemplet() : m_eType( BBT_NONE ), m_bStart( false )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {}
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    CX2BuffBehaviorTemplet& operator = ( const CX2BuffBehaviorTemplet& );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	BUFF_BEHAVIOR_TYPE GetType() const { return m_eType; }
	void SetType(BUFF_BEHAVIOR_TYPE val) { m_eType = val; }

	bool		ParsingBehaviorTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ );
	virtual bool ParsingBehavior( KLuaManager& luaManager_ ) { return true; }
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ ) {};

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	BUFF_BEHAVIOR_TYPE		m_eType;
	bool					m_bStart;
};

IMPLEMENT_INTRUSIVE_PTR( CX2BuffBehaviorTemplet );


/** @class : CX2BuffChangeNowHpPerSecondBehaviorTemplet
	@brief : 초당 HP가 변경 되는 행동을 정의 하는 클래스
	@date : 2012/7/16/
*/
class CX2BuffChangeNowHpPerSecondBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,							/// 초당 HP가 변화되는 타입
		FO_MULTIPLIER_OR_VALUE,					/// 배율 혹은 수치
		FO_PERCENT = FO_MULTIPLIER_OR_VALUE,	/// 수치타입이 아닌 경우에는 MULTIPLIER자리가 %
		FO_RELATION_TYPE,						/// 연동타입
		FO_ATTACK_VALUE,						/// 공격력
		FO_END									/// enum 개수
	};

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	enum SKILL_LEVEL
	{
		SL_VALUE = 30,
	};
#endif //UPGRADE_SKILL_SYSTEM_2013

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeNowHpPerSecondBehaviorTemplet ); }


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffChangeNowHpPerSecondBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_fResultValue( 0.0f ),	m_delegateChangeHpByTypeFunc(), 
		m_delegateGetValueOrPercentPerSecondFunc(),
		m_eChangeType( BCT_SWAP_VALUE ), m_bDrawText( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	void SelectDelegate();
	void ChangeHpByValue( CX2GameUnit* pGameUnit_ );
	void ChangeHpByPercent( CX2GameUnit* pGameUnit_ );

	float GetFixValuePerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const;
	float GetValuePerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const;
	float GetPercentPerSecondByPercent( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const;
	void DrawText( CX2GameUnit* pGameUnit_, const float fDeltaValue_ );

private:
	typedef srutil::delegate1<void, CX2GameUnit*> DelegateChangeHpByType;
	typedef srutil::delegate2<float, const KBuffBehaviorFactor*, const float> DelegateGetValueOrPercentPerSecond;

	CKTDXCheckElapsedTime		m_CheckElapsedTimeForInterval;	/// 몇 초마다 HP를 변경해야하는지를 판단하기 위한 멤버
	float						m_fResultValue;					/// 초당 변경될 수치 또는 %(중첩등 모든 계산을 다 끝낸 결정값)
	DelegateChangeHpByType		m_delegateChangeHpByTypeFunc;	/// 초당 값에 의해 변경되는지 %에 의해 변경되는지에 따라 다른 멤버 함수 실행
	DelegateGetValueOrPercentPerSecond m_delegateGetValueOrPercentPerSecondFunc;	/// 초당 변경될 수치 또는 %를 얻는 함수 델리게이트
	BUFF_CHANGE_TYPE			m_eChangeType;				/// 초당 변화되는 것이 % 인지 값인지 구분
	bool						m_bDrawText;				/// 변화되는 양을 표시할 것인가
};

/** @class : CX2BuffChangeNowMpPerSecondBehaviorTemplet
	@brief : 초당 Mp가 변경 되는 행동을 정의 하는 클래스
	@date : 2012/7/16/
*/
class CX2BuffChangeNowMpPerSecondBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// 초당 Mp가 변화되는 타입
		FO_VALUE_OR_PERCENT,		/// 수치 or %
		FO_END						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeNowMpPerSecondBehaviorTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffChangeNowMpPerSecondBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_CheckElapsedTimeForInterval( 0.0f ),
		m_fResultValue( 0.0f ), m_eChangeType( BCT_SWAP_VALUE ), m_bDrawText( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	void SelectDelegate();
	void ChangeMpByValue( CX2GameUnit* pGameUnit_ );
	void ChangeMpByPercent( CX2GameUnit* pGameUnit_ );

	float GetValueOrPercentPerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const;
	void DrawText( CX2GameUnit* pGameUnit_, const float fDeltaValue_ );

private:
	typedef srutil::delegate1<void, CX2GameUnit*> DelegateChangeMpByType;
	typedef srutil::delegate1<float, const float> DelegateGetValueOrPercentPerSecond;

	CKTDXCheckElapsedTime		m_CheckElapsedTimeForInterval;	/// 몇 초마다 Mp를 변경해야하는지를 판단하기 위한 멤버
	float						m_fResultValue;					/// 초당 변경될 수치 또는 %(중첩등 모든 계산을 다 끝낸 결정값)
	DelegateChangeMpByType		m_delegateChangeMpByTypeFunc;	/// 초당 값에 의해 변경되는지 %에 의해 변경되는지에 따라 다른 멤버 함수 실행
	BUFF_CHANGE_TYPE			m_eChangeType;				/// 초당 변화되는 것이 % 인지 값인지 구분
	bool						m_bDrawText;				/// 변화되는 양을 표시할 것인가
};

/** @class : CX2BuffReverseLeftRightBehaviorTemplet
	@brief : 일정 시간마다 좌우를 반전 시키는 행동 클래스
	@date : 2012/7/21/
*/
class CX2BuffReverseLeftRightBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffReverseLeftRightBehaviorTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffReverseLeftRightBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_CheckElapsedTimeForInterval( 0.0f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

	/// 복사생성자는 선언하지 않음(현재는 KBuffIdentity를 깊은 복사 할 이유 없음)
private:
	CKTDXCheckElapsedTime		m_CheckElapsedTimeForInterval;	/// 몇 초마다 좌우를 반전 시켜야 하는지
};

/** @class : CX2BuffAttackImpossibleBehaviorTemplet
	@brief : 해당 버프가 걸려있는 동안에는 공격(일반+스킬) 불가 행동
	@date : 2012/7/22/
*/
class CX2BuffIdentityBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffIdentityBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffIdentityBehaviorTemplet() : CX2BuffBehaviorTemplet()
	{}
};

/** @class : CX2BuffChangeStatBehaviorTemplet
	@brief : 해당 버프가 걸려있는 동안에 스탯 변경
	@date : 2012/7/23/
*/
class CX2BuffChangeStatBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// 변화되는 타입
		FO_VALUE,					/// 값
		FO_RELATION_TYPE,			/// CHANGE_TYPE이 연동인 경우 무엇과 연동될것인지
		FO_MULTIPLIER,				/// 배율(CHANGE_TYPE이 REATION 일때 사용)
		FO_END						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeStatBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeStatBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_eRelationType( BRT_END ),
		m_fResultValue( 0.0f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	float GetValue( const KBuffBehaviorFactor* pBehaviorFactor_, CX2GameUnit* pGameUnit_, const float fRegist_ ) const;
	bool MustUseOptionData() const;

private:
	BUFF_CHANGE_TYPE			m_eChangeType;
	BUFF_RELATION_TYPE			m_eRelationType;
	float						m_fResultValue;
};

/** @class : CX2BuffCreateBuffFactortBehaviorTemplet
	@brief : 해당 버프가 걸려있는 동안에 BuffFactor 적용
	@date : 2012/7/23/
*/
class CX2BuffCreateBuffFactortBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_BUFF_FACTOR_ID,
		FO_SKILL_ID,
		FO_END						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffCreateBuffFactortBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffCreateBuffFactortBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_uiBuffFactorId( 0 )
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		, m_uiSkillId( 0 )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{}

private:
	UINT						m_uiBuffFactorId;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	UINT						m_uiSkillId;
#endif //UPGRADE_SKILL_SYSTEM_2013
};

/** @class : CX2BuffAbsorbEffectAttackBehaviorTemplet
	@brief : 이펙트 공격을 흠수하여 MP 회복
	@date : 2012/7/27/
*/
class CX2BuffAbsorbEffectAttackBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_ABSORB_VALUE,			/// 발사체 한개당 회복량
		FO_END						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffAbsorbEffectAttackBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffAbsorbEffectAttackBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_fAbsorbValue( 0.0f )
	{}

private:
	float						m_fAbsorbValue;
};

// /** @class : CX2BuffChangeMpHitBehaviorTemplet
// 	@brief : 타격시 MP 회복량
// 	@date : 2012/7/27/
// */
// class CX2BuffChangeMpHitBehaviorTemplet	: public CX2BuffBehaviorTemplet
// {
// public:
// 	enum FACTOR_ORDER
// 	{
// 		FO_MULTIPLIER,				/// 배율
// 		FO_END						/// enum 개수
// 	};
// 
// 	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeMpHitBehaviorTemplet ); }
// 
// 	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
// 	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
// 	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
// 	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
// 	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
// 
// protected:
// 	CX2BuffChangeMpHitBehaviorTemplet() : CX2BuffBehaviorTemplet(),
// 		m_fMultiplier( 1.0f )
// 	{}
// 
// private:
// 	float						m_fMultiplier;
// };
// 
// /** @class : CX2BuffChangeMpHittedBehaviorTemplet
// 	@brief : 타격시 MP 회복량
// 	@date : 2012/7/27/
// */
// class CX2BuffChangeMpHittedBehaviorTemplet	: public CX2BuffBehaviorTemplet
// {
// public:
// 	enum FACTOR_ORDER
// 	{
// 		FO_MULTIPLIER,				/// 배율
// 		FO_END						/// enum 개수
// 	};
// 
// 	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeMpHittedBehaviorTemplet ); }
// 
// 	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
// 	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
// 	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
// 	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
// 	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
// 
// protected:
// 	CX2BuffChangeMpHittedBehaviorTemplet() : CX2BuffBehaviorTemplet(),
// 		m_fMultiplier( 1.0f ), m_eChangeType;
// 	{}
// 
// private:
// 	float						m_fMultiplier;
// 	BUFF_CHANGE_TYPE			m_eChangeType;
// };
// 
// /** @class : CX2BuffChangeHyperModeBehaviorTemplet
// 	@brief : 각성모드로 변경시키는 버프행동 (각성구슬이 있다는 전제하에 수행)
// 	@date : 2012/8/7/
// */
class CX2BuffChangeHyperModeBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_HYPER_MODE_TIME,			/// 각성유지시간
		FO_HYPER_MODE_COUNT,		/// 몇각성인가(각성했을 때의 구슬 갯수)
		FO_END						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeHyperModeBehaviorTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeHyperModeBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_uiHyperModeCount( 0 ), m_fRemainTime( 0.0f )
	{}

private:
	UINT						m_uiHyperModeCount;	/// 몇각성인가
	float						m_fRemainTime;		/// 남은 각성 시간 보여주기용...
};

/** @class : CX2BuffResetSkillCoolTimeBehaviorTemplet
	@brief : 스킬의 쿨타임을 초기화 시켜주는 행동
	@date : 2012/8/7/
*/
class CX2BuffResetSkillCoolTimeBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// 변화되는 타입
		FO_VALUE,					/// 값
		FO_END,						/// enum 개수
	};
#endif // UPGRADE_SKILL_SYSTEM_2013

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffResetSkillCoolTimeBehaviorTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	
protected:
	CX2BuffResetSkillCoolTimeBehaviorTemplet() : CX2BuffBehaviorTemplet()
		, m_eSkillTypeToResetCoolTime( 0 )
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환
		, m_eChangeType( BCT_FIX_VALUE )
		, m_fCoolTime( 0.f )
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
		, m_fCoolTimeRate ( 1.f ) 
		, m_bIsRelativeAllSkill ( false)
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	UINT				m_eSkillTypeToResetCoolTime;	/// 쿨타임을 리셋할 스킬타입
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환
	BUFF_CHANGE_TYPE	m_eChangeType;					/// 변수 설정 타입
	float				m_fCoolTime;					/// 설정할 쿨타임
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
	float				m_fCoolTimeRate;				// 설정한 쿨타임 (배율)
	bool				m_bIsRelativeAllSkill;			// 스킬 타입과는 관계 없이 모든 장착 가능 스킬 타입에 대해 영향을 미칠 것인가?
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가

};

/** @class : CX2BuffChangeUnitScaleBehaviorTemplet
	@brief : 유닛의 크기를 변경하는 행동 템프릿
	@date : 2012/8/7/
*/
class CX2BuffChangeUnitScaleBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeUnitScaleBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeUnitScaleBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_vScale( 0.0f, 0.0f, 0.0f ), m_bUseTimeToReturn( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	D3DXVECTOR3			m_vScale;
	bool				m_bUseTimeToReturn;
};

/** @class : CX2BuffCustomFunctionBehaviorTemplet
	@brief : 하드코딩된 처리를 CommonStateStart, CommonStateFrameMove, CommonStateEnd에서 수행하도록 하는 행동 템플릿
	@date : 2012/8/7/
*/
class CX2BuffCustomFunctionBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffCustomFunctionBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffCustomFunctionBehaviorTemplet() : CX2BuffBehaviorTemplet(), m_kBuffBehaviorFactor()
	{}

private:
	KBuffBehaviorFactor		m_kBuffBehaviorFactor;
};

/** @class : CX2BuffEffectSetWithDamageBehaviorTemplet
	@brief : 데미지를 주는 이펙트셋 행동 템플릿(버프를 사용한 스테이트의PowerRate와 난입한 유저가 이펙트셋을 실행 시킬때 상태의 PowerRate가 다른 경우가 많으므로 데미지가 있는 EffectSet의 경우 PowerRate를 동기화하는 Behavior 사용)
	@date : 2012/8/27/
*/
class CX2BuffEffectSetWithDamageBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffEffectSetWithDamageBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffEffectSetWithDamageBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_fPowerRate( 0.0f ), m_bNotUse( false )
	{
		m_vecHandleEffectSet.clear();
		m_vecWstrEffectSetName.clear();
	}
	
	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	void PlayEffectSet( CX2GameUnit* pGameUnit_ );

private:
	vector<CX2EffectSet::Handle>		m_vecHandleEffectSet;
	vector<wstring>						m_vecWstrEffectSetName;	/// 여러 이펙트셋을 지정가능
	float								m_fPowerRate;
	bool								m_bNotUse;				/// 템플릿에 EffectSet이 지정되어 있어도, Factor에서 이펙트셋을 사용 여부를 어떻게 지정했는지를 저장하는 변수
};

/** @class : CX2BuffStunBehaviorTemplet
	@brief : 기절 스테이트로 고정시키는 템플릿
	@date : 2012/8/29/
*/
class CX2BuffStunBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffStunBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffStunBehaviorTemplet() : CX2BuffBehaviorTemplet()
	{}	
};

/** @class : CX2BuffFinishOtherBuffDebuffBehaviorTemplet
	@brief : 다른 버프, 디버프를 종료 시키는 행동
	@date : 2012/9/2/
*/
class CX2BuffFinishOtherBuffDebuffBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffFinishOtherBuffDebuffBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;

	/// 필요가 없을 것 같아서 삭제
	//virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	
protected:
	CX2BuffFinishOtherBuffDebuffBehaviorTemplet() : CX2BuffBehaviorTemplet()
	{
		m_vecBuffTempletIDToBeFinished.clear();
	}
	
	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	inline void EraseBuffTempletFromGameUnit( CX2GameUnit* pGameUnit_, const BUFF_TEMPLET_ID eExceptBuffTempletID_ );

private:
	vector<BUFF_TEMPLET_ID>		m_vecBuffTempletIDToBeFinished;
};

/** @class : CX2BuffFixStateBehaviorTemplet
	@brief : 현재 스테이트로 고정시키는 템플릿
	@date : 2012/9/4/
*/
class CX2BuffFreezeBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffFreezeBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

protected:
	CX2BuffFreezeBehaviorTemplet() : CX2BuffBehaviorTemplet()
		, m_fAnimCurrentTime( 0.0f ), m_bIsDown( false )
	{}	

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	float	m_fAnimCurrentTime;
	bool	m_bIsDown;				/// 버프 종료시 다운 스테이트로 넘어갈 것인지
};

/** @class : CX2BuffCanPassUnitBehaviorTemplet
	@brief : 버프가 진행되는 동안 유닛의 CanPassUnit을 설정 하는 행동 템플릿
	@date : 2012/9/5/
*/
class CX2BuffCanPassUnitBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffCanPassUnitBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

protected:
	CX2BuffCanPassUnitBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_bCanPassUnit( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	bool	m_bCanPassUnit;
};

/** @class : CX2BuffNeverMoveBehaviorTemplet
	@brief : 버프가 진행되는 동안 유닛을 제자리 고정 시키는 행동 템프릿
	@date : 2012/9/5/
*/
class CX2BuffNeverMoveBehaviorTemplet: public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffNeverMoveBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
#ifdef NEVER_MOVE_GRAVITY_APPLY
	CX2BuffNeverMoveBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_vPosition( 0.0f, 0.0f, 0.0f ), m_bIsGravity( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
#else
	CX2BuffNeverMoveBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_vPosition( 0.0f, 0.0f, 0.0f )
	{}
#endif //NEVER_MOVE_GRAVITY_APPLY



private:
	D3DXVECTOR3		m_vPosition;
#ifdef NEVER_MOVE_GRAVITY_APPLY
	bool			m_bIsGravity;	//중력을 적용할것인가. true면 중력 적용, false면 위치 고정
#endif //NEVER_MOVE_GRAVITY_APPLY
};

/** @class : CX2BuffChangeAttackByTypeBehaviorTemplet
	@brief : 공격 타입에 따른 데미지를 증가시키는 행동 템프릿
	@date : 2012/11/03/
*/
class CX2BuffChangeAttackByTypeBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// 변화되는 타입
		FO_VALUE,					/// 값
		FO_ATTACK_TYPE,				/// 공격 타입
		FO_END,						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeAttackByTypeBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeAttackByTypeBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_fResultValue( 0.f ), m_eAttackType( CX2DamageManager::AT_NORMAL )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE				m_eChangeType;
	float							m_fResultValue;
	CX2DamageManager::ATTACK_TYPE	m_eAttackType;
};

/** @class : CX2BuffChangeEncahntAttackRateBehaviorTemplet
@brief : 소켓 속성 공격 발동 확률을 증가시키는 행동 템프릿
@date : 2012/11/03/
*/
class CX2BuffChangeEncahntAttackRateBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// 변화되는 타입
		FO_VALUE,					/// 값
		FO_END,						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeEncahntAttackRateBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeEncahntAttackRateBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_fRate( 0.f )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 기본 값은 모든 속성 강화
		, m_eEnchantAttributeType ( BEAT_ALL )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE				m_eChangeType;
	float							m_fRate;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	BUFF_ENCHANT_ATTRIBUTE_TYPE		m_eEnchantAttributeType;	// 인챈트 속성 타입
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
};

/** @class : CX2BuffChangeEncahntAttackRateBehaviorTemplet
@brief : 소켓 속성 공격 발동 확률을 증가시키는 행동 템프릿
@date : 2012/11/03/
*/
class CX2BuffDoubleAttackBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// 변화되는 타입
		FO_VALUE,					/// 값
		FO_END,						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffDoubleAttackBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffDoubleAttackBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_fRate( 0.f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE				m_eChangeType;
	float							m_fRate;
};

/** @class : CX2BuffChangeAttackSphereScaleBehaviorTemplet
	@brief : 어택박스의 크기를 변경하는 행동 템플릿
	@date : 2013/03/13/
*/
class CX2BuffChangeAttackSphereScaleBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	struct AttackSphereNameAndScale
	{
		wstring		m_wstrAttackSphereName;
		float		m_fScale;

		AttackSphereNameAndScale()
			: m_fScale( 1.0f )
		{
			m_wstrAttackSphereName.clear();
		}
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeAttackSphereScaleBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeAttackSphereScaleBehaviorTemplet() : CX2BuffBehaviorTemplet()
	{
		m_vecAttackSphereNameAndScale.clear();
	}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	vector<AttackSphereNameAndScale>	m_vecAttackSphereNameAndScale;
};

/** @class : CX2BuffAddSkillLevelBehaviorTemplet
	@brief : 스킬레벨을 Up 또는 Down 시키는 행동 템플릿
	@date : 2013/04/16/
*/
class CX2BuffAddSkillLevelBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_SKILL_LEVEL = 0,				///	변화 될 스킬레벨 값
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffAddSkillLevelBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffAddSkillLevelBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_iAddSkillLevel( 0 )
	{}

private:
	int	m_iAddSkillLevel;		/// 상승 또는 감소 시킬 스킬 레벨
};

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환

/** @class : CX2BuffReflectMagicBehaviorTemplet
	@brief : 해당 버프가 걸려있는 동안에는 공격(일반+스킬) 불가 행동
	@date : 2013/7/10/
*/
class CX2BuffReflectMagicBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// 변화되는 타입
		FO_VALUE,					/// 값
		FO_END						/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffReflectMagicBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffReflectMagicBehaviorTemplet() : CX2BuffBehaviorTemplet()
		, m_eChangeType( BCT_SWAP_VALUE )
		, m_fResultValue( 0.0f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE			m_eChangeType;
	float						m_fResultValue;
};

#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
/** @class : CX2BuffChangeManaConsumeRate
	@brief : 해당 디버프가 걸려있는 스킬사용 시 마나 소비량 변화
	@date : 2013/09/09/
*/
class CX2BuffChangeConsumeMpRate : public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,		/// 변화되는 타입
		FO_SKILL_ID,		/// 스킬 아이디
		FO_VALUE,			/// 값
		FO_END				/// enum 개수
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeConsumeMpRate ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeConsumeMpRate() : CX2BuffBehaviorTemplet()
		, m_eChangeType( BCT_SWAP_VALUE ), m_fResultValue( 0.0f ), m_uiSkillId( 0 ), m_fInitMp( 0.f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE	m_eChangeType;		/// 변화되는 타입
	UINT				m_uiSkillId;		/// 적용될 스킬 아이디
	float				m_fResultValue;		/// 값
	float				m_fInitMp;			/// 초기 마나 사용량
};
#endif //FIX_SKILL_BALANCE_AISHA_LENA