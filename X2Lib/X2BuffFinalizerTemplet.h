/** @file : X2BuffFinalizerTemplet.h
    @breif : 버프의 종료 부분을 정의하는 Templet 파일
*/

#pragma once

class CX2BuffFinalizerTemplet;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BuffFinalizerTemplet> CX2BuffFinalizerTempletPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BuffFinalizerTemplet> CX2BuffFinalizerTempletPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

/** @class : CX2BuffFinalizerTemplet
	@brief : 버프의 종료 부분을 정의하는 클래스
	@date : 2012/7/17/
*/
class CX2BuffFinalizerTemplet
{
public:
	static bool ParsingCombinationFinalizer( KLuaManager& luaManager_, OUT vector<CX2BuffFinalizerTempletPtr>& vecBuffFinalizerTempletPtr_ );
	
	CX2BuffFinalizerTemplet( const CX2BuffFinalizerTemplet& rhs_ ) : m_eType( rhs_.m_eType ), m_bReserveToFinish( false )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
	virtual ~CX2BuffFinalizerTemplet() {}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ ) {};
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ ) {};
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ ) = NULL;
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const = NULL;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const = NULL;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const = NULL;
	
	void ReserveToFinish() { m_bReserveToFinish = true; }
	bool DidReserveToFinish() const { return m_bReserveToFinish; }

	bool SetFactorFromPacketTemplateMothod( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ );
	BUFF_FINALIZER_TYPE GetType() const { return m_eType; }
#ifdef FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG
	virtual void SetBuffTempletID( BUFF_TEMPLET_ID eBuffTempletID_ ){}
#endif // FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR


protected:
	CX2BuffFinalizerTemplet() : m_eType( BFT_NONE )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    CX2BuffFinalizerTemplet& operator = ( const CX2BuffFinalizerTemplet& );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	void SetType(BUFF_FINALIZER_TYPE val) { m_eType = val; }

	bool		ParsingFinalizerTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ );
	virtual bool ParsingFinalizer( KLuaManager& luaManager_ ) { return true; }
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ ) {};
	
private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	BUFF_FINALIZER_TYPE		m_eType;
	bool					m_bReserveToFinish;	/// 종료를 예약 (이 값이 true 면 종료 체크시에 종료됨)
};

IMPLEMENT_INTRUSIVE_PTR( CX2BuffFinalizerTemplet );


/** @class : CX2BuffTimeFinalizerTemplet
	@brief : 시간에 의해 종료되는 조건을 정의하는 클래스
	@date : 2012/7/17/
*/
class CX2BuffTimeFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_DURATION_BUFF_TIME_TYPE,		/// 랜덤 시간인지, 고정 시간인지
		FO_NORMAL_TIME,					/// 정해진 시간
		FO_FORCE_TIME,					/// 강제각성 시간
		FO_MIN_TIME = FO_NORMAL_TIME,	/// 랜덤인 경우 최소 시간
		FO_MAX_TIME,					/// 랜덤인 경우 최대 시간
		
		FO_END,					/// enum 개수
	};

	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffTimeFinalizerTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
#ifdef FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG
	virtual void SetBuffTempletID( BUFF_TEMPLET_ID eBuffTempletID_ ){ m_eBuffTempleteID = eBuffTempletID_;}
#endif // FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG
protected:
	CX2BuffTimeFinalizerTemplet() : CX2BuffFinalizerTemplet(),
		m_fDurationTime( 0.0f ), m_eDurationTimeType( BDTT_NONE )
#ifdef BUFF_ICON_UI //버프 남은 지속 시간 얻기
		,m_eBuffTempleteID(BTI_NONE)
		,m_bIsAlreadyNotifyDurationTime(false)
#endif //BUFF_ICON_UI
#ifdef DISPLAY_BUFF_DURATION_TIME
		,m_iDurationSec(0)
#endif // DISPLAY_BUFF_DURATION_TIME
 {}

	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual bool ParsingFinalizer(  KLuaManager& luaManager_ );

private:
	float	m_fDurationTime;						/// 정해진 지속시간
	BUFF_DURATION_TIME_TYPE m_eDurationTimeType;	/// 랜덤시간인지, 고정시간인지
#ifdef BUFF_ICON_UI //버프 남은 지속 시간 얻기
	BUFF_TEMPLET_ID			m_eBuffTempleteID;
	bool					m_bIsAlreadyNotifyDurationTime;
#endif //BUFF_ICON_UI
#ifdef DISPLAY_BUFF_DURATION_TIME
	int		m_iDurationSec;
#endif // DISPLAY_BUFF_DURATION_TIME
	
};

/** @class : CX2BuffHitCountFinalizerTemplet
	@brief : 일정 타격 카운트가 되면 종료되는 조건을 정의하는 클래스
	@date : 2012/7/19/
*/
class CX2BuffHitCountFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	static const int	MAX_COUNT = 256;

	enum FACTOR_ORDER
	{
		FO_USE_BUFF_COUNT_TYPE,
		FO_COUNT,			/// FO_RESULT_COUNT 값이 없고, FO_USE_BUFF_COUNT_TYPE 이 상대방의 버프/디버프 개수를 참조하는 타입이면 FO_RESULT_COUNT를 계산 하기 위해 사용한다.
		FO_RESULT_COUNT,	/// 버프카운트까지 고려한 결과 count (버프카운트를 사용하지 않는 경우 FO_COUNT와 동일)
		FO_END,				/// enum 개수
	};

	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffHitCountFinalizerTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffHitCountFinalizerTemplet() : CX2BuffFinalizerTemplet(),
		m_eUseCountType( BUCT_NOT_USE ), m_iCountToBeFinished( 0 ), m_ucCountWhenStart( 0 ), m_bStart( false ) {}

	virtual bool ParsingFinalizer(  KLuaManager& luaManager_ );
	UINT		 GetNewCount( const KBuffFinalizerFactor* pFactor_, CX2GameUnit* pGameUnit_ ) const;


private:
	BUFF_USE_COUNT_TYPE		m_eUseCountType;
	int						m_iCountToBeFinished;		/// 종료되기 위해 도달해야하는 카운트(- 값도 될수 있음)
	UCHAR					m_ucCountWhenStart;			/// 버프 시작시 카운트

	/// 버프 시작 여부(SetFactor로 실행될때는 처음 실행할 때 true로 셋팅하고
	/// 서버로부터 패킷을 받아서 실행할 때는 P2P 패킷을 받은 이후에 true로 셋팅함
	bool					m_bStart;					
};

/** @class : CX2BuffHittedCountFinalizerTemplet
	@brief : 일정 피격 카운트가 되면 종료되는 조건을 정의하는 클래스
	@date : 2012/7/19/
*/
class CX2BuffHittedCountFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	static const int	MAX_COUNT = 256;

	enum FACTOR_ORDER
	{
		FO_USE_BUFF_COUNT_TYPE,
		FO_COUNT,			/// FO_RESULT_COUNT 값이 없고, FO_USE_BUFF_COUNT_TYPE 이 상대방의 버프/디버프 개수를 참조하는 타입이면 FO_RESULT_COUNT를 계산 하기 위해 사용한다.
		FO_RESULT_COUNT,	/// 버프카운트까지 고려한 결과 count (버프카운트를 사용하지 않는 경우 FO_COUNT와 동일)
		FO_END,				/// enum 개수
	};

	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffHittedCountFinalizerTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffHittedCountFinalizerTemplet() : CX2BuffFinalizerTemplet(),
		m_eUseCountType( BUCT_NOT_USE ), m_iCountToBeFinished( 0 ), m_ucCountWhenStart( 0 ), m_bStart( false ) {}

	virtual bool ParsingFinalizer(  KLuaManager& luaManager_ );
	UINT GetNewCount( const KBuffFinalizerFactor* pFactor_, CX2GameUnit* pGameUnit_ ) const;

private:
	BUFF_USE_COUNT_TYPE		m_eUseCountType;
	int						m_iCountToBeFinished;		/// 종료되기 위해 도달해야하는 카운트(- 값도 될수 있음)
	UCHAR					m_ucCountWhenStart;			/// 버프 시작시 카운트

	/// 버프 시작 여부(SetFactor로 실행될때는 처음 실행할 때 true로 셋팅하고
	/// 서버로부터 패킷을 받아서 실행할 때는 P2P 패킷을 받은 이후에 true로 셋팅함
	bool					m_bStart;				
};

/** @class : CX2BuffStateChangeFinalizerTemplet
	@brief : 현재 지정된 스테이트에서 변경되면 종료되는 조건을 정의하는 클래스
	@date : 2012/7/19/
*/
class CX2BuffStateChangeFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_STATE_ID,
		FO_END,				/// enum 개수
	};

	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffStateChangeFinalizerTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffStateChangeFinalizerTemplet() : CX2BuffFinalizerTemplet(),
		m_uiStateID( 0 ) {}

private:
	UINT	m_uiStateID;			/// 버프가 시작 됐을 때의 스테이트 ID
};

/** @class : CX2BuffDamageValueFinalizerTemplet
	@brief : 일정한 데미지를 받으면 종료되는 조건을 정의하는 클래스
	@date : 2012/7/20/
*/
class CX2BuffDamageValueFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// 변화되는 타입
		FO_VALUE,					/// 값
		FO_RELATION_TYPE,			/// CHANGE_TYPE이 연동인 경우 무엇과 연동될것인지
		FO_MULTIPLIER,				/// 배율(CHANGE_TYPE이 REATION 일때 사용)
		FO_END,						/// enum 개수
	};

	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffDamageValueFinalizerTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffDamageValueFinalizerTemplet() : CX2BuffFinalizerTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_eRelationType( BRT_END ),
		m_fNowHpOldFrame( 0.0f ), m_fDamageValue( 0.0f ), m_bStart( false )
	{}

	virtual bool ParsingFinalizer(  KLuaManager& luaManager_ );
	float GetValue( const KBuffFinalizerFactor* pFinalizerFactor_, CX2GameUnit* pGameUnit_ ) const;
	void ModifyDamageValue( const CX2GameUnit* pGameUnit_ );

private:
	BUFF_CHANGE_TYPE		m_eChangeType;			/// 변경타입
	BUFF_RELATION_TYPE		m_eRelationType;		/// 연동타입
	float					m_fNowHpOldFrame;		/// 이전 프레임의 Hp
	float					m_fDamageValue;			/// 데미지량
	/// 버프 시작 여부(SetFactor로 실행될때는 처음 실행할 때 true로 셋팅하고
	/// 서버로부터 패킷을 받아서 실행할 때는 P2P 패킷을 받은 이후에 true로 셋팅함
	bool					m_bStart;				
};

/** @class : CX2BuffPassHpFinalizerTemplet
	@brief : 기준 HP를 상향 또는 하향 돌파 하는 경우에 종료되는 요소 정의 클래스
	@date : 2012/7/20/
*/
class CX2BuffPassHpFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_PASS_UP,			/// 상향 돌파? 하향 돌파?
		FO_CHANGE_TYPE,		/// 타입(% 인지 수치 인지)
		FO_CRITERION,		/// 기준 수치
		FO_END,				/// enum 개수
	};

	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffPassHpFinalizerTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffPassHpFinalizerTemplet() : CX2BuffFinalizerTemplet(),
		m_eBuffChangeType( BCT_SWAP_VALUE ), m_fCriterion( 0.0f ), m_bPassUp( false ) {}

	virtual bool ParsingFinalizer(  KLuaManager& luaManager_ );
	void SelectDelegate();
	bool CheckHpByUp( CX2GameUnit* pGameUnit_ ) const;
	bool CheckHpByDown( CX2GameUnit* pGameUnit_ ) const;

	float GetCriterionByValue( CX2GameUnit* pGameUnit_ ) const;
	float GetCriterionByPercent( CX2GameUnit* pGameUnit_ ) const;

private:
	typedef srutil::delegate1<bool, CX2GameUnit*> DelegateCheckHpByPassType;
	typedef srutil::delegate1<float, CX2GameUnit*> DelegateGetCriterionByChangeType;

	DelegateCheckHpByPassType				m_delegateCheckHpByPassTypeFunc;			/// 타입에 의해서 다른 체크 함수 실행
	DelegateGetCriterionByChangeType		m_delegateGetCriterionByChangeTypeFunc;		/// 타입에 의해서 % 또는 값을 리턴
	BUFF_CHANGE_TYPE			m_eBuffChangeType;	/// 타입(% 인지 수치 인지)
	float						m_fCriterion;		/// 기준 수치
	bool						m_bPassUp;			/// 상향돌파 인가 하향돌파인가?
};

/** @class : CX2BuffPassMpFinalizerTemplet
	@brief : 기준 Mp를 상향 또는 하향 돌파 하는 경우에 종료되는 요소 정의 클래스
	@date : 2012/7/20/
*/
class CX2BuffPassMpFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_PASS_UP,
		FO_CHANGE_TYPE,		/// 타입(% 인지 수치 인지)
		FO_CRITERION,		/// 기준 수치
		FO_END,				/// enum 개수
	};

	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffPassMpFinalizerTemplet ); }
public:
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffPassMpFinalizerTemplet() : CX2BuffFinalizerTemplet(),
		m_eBuffChangeType( BCT_SWAP_VALUE ), m_fCriterion( 0.0f ), m_bPassUp( false ) {}

	virtual bool ParsingFinalizer(  KLuaManager& luaManager_ );
	void SelectDelegate();
	bool CheckMpByUp( CX2GameUnit* pGameUnit_ ) const;
	bool CheckMpByDown( CX2GameUnit* pGameUnit_ ) const;

	float GetCriterionByValue( CX2GameUnit* pGameUnit_ ) const;
	float GetCriterionByPercent( CX2GameUnit* pGameUnit_ ) const;

private:
	typedef srutil::delegate1<bool, CX2GameUnit*> DelegateCheckMpByPassType;
	typedef srutil::delegate1<float, CX2GameUnit*> DelegateGetCriterionByChangeType;

	DelegateCheckMpByPassType				m_delegateCheckMpByPassTypeFunc;			/// 타입에 의해서 다른 체크 함수 실행
	DelegateGetCriterionByChangeType		m_delegateGetCriterionByChangeTypeFunc;		/// 타입에 의해서 % 또는 값을 리턴
	BUFF_CHANGE_TYPE			m_eBuffChangeType;	/// 타입(% 인지 수치 인지)
	float						m_fCriterion;		/// 기준 수치
	bool						m_bPassUp;			/// 상향돌파 인가 하향돌파인가?
};

/** @class : CX2BuffImmediateFinalizerTemplet
	@brief : 적용 즉시 종료의 의미로 한번의 FrameMove 수행 후 종료되는 조건을 정의하는 클래스
	@date : 2012/7/19/
*/
class CX2BuffImmediateFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffImmediateFinalizerTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	
protected:
	CX2BuffImmediateFinalizerTemplet() : CX2BuffFinalizerTemplet()
	{}
};

/** @class : CX2BuffDungeonStageChangeFinalizerTemplet
	@brief : 던전의 스테이지 등이 변경 되면 종료되는 템플릿
	@date : 2012/9/1/
*/
class CX2BuffDungeonStageChangeFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffDungeonStageChangeFinalizerTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffDungeonStageChangeFinalizerTemplet() 
		: CX2BuffFinalizerTemplet(), m_uiIndex( 0 )
	{}

private:
	UINT	m_uiIndex;
};

#ifdef ADD_LIVE_CREATOR_FINALIZER
/** @class : CX2BuffLiveCreatorFinalizerTemplet
	@brief : 버프 시전자가 사망하거나 존재하지 않으면 종료되는 템플릿
	@date : 2012/11/15/
*/
class CX2BuffLiveCreatorFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	enum FACTOR_ORDER
	{
		/// int64를 16비트씩 float 4개에 쪼개서 넣음...
		/// ( FO_UNIT_UID_FRONT_UP   FO_UNIT_UID_FRONT_DOWN   FO_UNIT_UID_BACK_UP   FO_UNIT_UID_BACK_DOWN )
		FO_UNIT_UID_BACK_DOWN,		/// 유닛 UID 하위 아래 비트
		FO_UNIT_UID_BACK_UP,		/// 유닛 UID 하위 위   비트
		FO_UNIT_UID_FRONT_DOWN,		/// 유닛 UID 상위 아래 비트
		FO_UNIT_UID_FRONT_UP,		/// 유닛 UID 상위 위   비트
		FO_IS_USER_UNIT,			/// 유저인지 NPC인지 여부
	};

	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffLiveCreatorFinalizerTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffLiveCreatorFinalizerTemplet() 
		: CX2BuffFinalizerTemplet(), m_uidCreatorUID( -1 ), m_bIsLiveCreator( true )
	{}

private:
	//UidType m_iCreatorUID;
	UidType				m_uidCreatorUID;		/// 유닛 UID
	bool				m_bIsUserUnit;			/// 유저인지 NPC인지 여부
	bool				m_bIsLiveCreator;		/// 버프 시전자 생존 혹은 존재 여부
	CX2GameUnitoPtr		m_optrCreatorUnit;		/// 시전자 유닛 객체
};
#endif ADD_LIVE_CREATOR_FINALIZER

#ifdef ADD_BUFF_FINALIZER_FINISH_HYPER
/** @class : CX2BuffFinishHyperFinalizerTemplet
	@brief : 각성이 끝났을 경우 종료되는 템플릿
	@date  : 2013/02/22
*/
class CX2BuffFinishHyperFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffFinishHyperFinalizerTemplet ); }
	
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG
	virtual void SetBuffTempletID( BUFF_TEMPLET_ID eBuffTempletID_ ){ m_eBuffTempleteID = eBuffTempletID_;}
#endif // FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG

protected:
	CX2BuffFinishHyperFinalizerTemplet() 
		: CX2BuffFinalizerTemplet(), m_bHyper( false ), m_fDurationTime( 0.0f ), m_eBuffTempleteID(BTI_NONE)
		, m_bIsAlreadyNotifyDurationTime(false)
	{}

private:
	bool	m_bHyper;			/// 각성 상태 확인
	float	m_fDurationTime;	/// 정해진 지속시간

	BUFF_TEMPLET_ID		m_eBuffTempleteID;
	bool				m_bIsAlreadyNotifyDurationTime;
};
#endif //ADD_BUFF_FINALIZER_FINISH_HYPER

#ifdef	RIDING_SYSTEM
/** @class : CX2BuffFinishRidingPetOnOrNotFinalizerTemplet
	@brief : 라이딩 펫에 나거나 내리면 종료되는 템플릿
	@date  : 2013/04/22
*/
class CX2BuffFinishRidingPetOnOrNotFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffFinishRidingPetOnOrNotFinalizerTemplet ); }
	
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	
protected:
	CX2BuffFinishRidingPetOnOrNotFinalizerTemplet() 
		: CX2BuffFinalizerTemplet(), m_bFinishRidingOn( false )
	{}
	
	bool		ParsingFinalizerTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ );

private:

	/// 라이딩 펫에 탔을 때 버프가 종료되야 하면 true, 내렸을 때 종료 되어야 하면 false
	bool	m_bFinishRidingOn;
};
#endif	RIDING_SYSTEM

/** @class : CX2BuffDungeonStageChangeFinalizerTemplet
	@brief : X2 스테이트 변경 되면 종료 되는 템플릿
	@date : 2013/7/16/
*/
class CX2BuffX2StateChangeFinalizerTemplet : public CX2BuffFinalizerTemplet
{
public:
	static CX2BuffFinalizerTempletPtr CreateBuffFinalizerTempletPtr() { return CX2BuffFinalizerTempletPtr( new CX2BuffX2StateChangeFinalizerTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffFinalizerTempletPtr GetClonePtr() const;
	virtual bool DidFinish( CX2GameUnit* pGameUnit_ ) const;
	virtual void GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const;
	virtual void SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffX2StateChangeFinalizerTemplet() 
		: CX2BuffFinalizerTemplet(), m_uiIndex( 0 )
	{}

private:
	UINT	m_uiIndex;
};