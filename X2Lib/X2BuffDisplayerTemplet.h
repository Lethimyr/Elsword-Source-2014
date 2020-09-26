/**@file : X2BuffBehaviorTemplet.h
   @breif : 버프의 행동을 정의하는 Templet 파일
*/

#pragma once

class CX2BuffTemplet;
class   CX2BuffDisplayerTemplet;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BuffDisplayerTemplet> CX2BuffDisplayerTempletPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BuffDisplayerTemplet> CX2BuffDisplayerTempletPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

/** @class : CX2BuffDisplayerTemplet
	@brief : 버프의 표현을 정의 하는 클래스
	@date : 2012/7/16/
*/
class CX2BuffDisplayerTemplet
{
public:
	static bool ParsingCombinationDisplayer( KLuaManager& luaManager_, OUT vector<CX2BuffDisplayerTempletPtr>& vecBuffDisplayerTempletPtr_ );

	CX2BuffDisplayerTemplet( const CX2BuffDisplayerTemplet& rhs_ ) 
		: m_eType( rhs_.m_eType ), m_bStart( rhs_.m_bStart )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
	virtual ~CX2BuffDisplayerTemplet() {}

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ ) = NULL;
	virtual CX2BuffDisplayerTempletPtr GetClonePtr() const = NULL;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ ) {}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ ) {}
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ ) {}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	bool DidStart() const { return m_bStart; }
	void SetStart(bool val) { m_bStart = val; }

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

protected:
	CX2BuffDisplayerTemplet() : m_eType( BDT_NONE ), m_bStart( false )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {}
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    CX2BuffDisplayerTemplet& operator = ( const CX2BuffDisplayerTemplet& );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	bool		ParsingDisplayerTemplateMethod( KLuaManager& luaManager_, const char* pszTableNameUTF8_ );
	virtual bool ParsingDisplayer( KLuaManager& luaManager_ ) = NULL;
	
	BUFF_DISPLAYER_TYPE GetType() const { return m_eType; }
	void SetType(BUFF_DISPLAYER_TYPE val) { m_eType = val; }
	
private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	BUFF_DISPLAYER_TYPE		m_eType;
	bool					m_bStart;
};

IMPLEMENT_INTRUSIVE_PTR( CX2BuffDisplayerTemplet );


/** @class : CX2BuffRenderParamDisplayerTemplet
	@brief : 게임유닛의 RenderParam을 바꾸는 표현 클래스
	@date : 2012/8/6/
*/

class CX2BuffRenderParamDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:

	/** @struct : BuffRenderParam
		@brief : 버프에 의해서 변경될 수 있는 RenderParam내의 값을 담은 구조체
		@date : 2012/8/6/
	*/
	struct BuffRenderParam		/// 버프에 의해서 변경될 수 있는 RenderParam내의 값
	{
		CKTDGXRenderer::RENDER_TYPE			m_eRenderType;		/// RenderParam 중 renderType과 연관
		CKTDGXRenderer::CARTOON_TEX_TYPE	m_eCartoonTexType;	/// RenderParam 중 cartoonTexType과 연관
		float				m_fOutLineWide;						/// RenderParam 중 fOutLineWide와 연관
		D3DXCOLOR			m_d3dxColorOutLine;					/// RenderParam 중 outLineColor와 연관
		D3DXCOLOR			m_d3dxColor;						/// RenderParam 중 color와 연관
		bool				m_bAlphaBlend;						/// RenderParam 중 bAlphaBlend와 연관

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		float				m_fFlickerColorGap;					/// RenderParam 중 color 반짝이는 간격
#endif //SERV_ADD_LUNATIC_PSYKER

		BuffRenderParam() 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		{
			Initializer();
		}

		BuffRenderParam( const BuffRenderParam& rhs_ ) :
			m_eRenderType( rhs_.m_eRenderType ), 
			m_eCartoonTexType( rhs_.m_eCartoonTexType ), m_fOutLineWide( rhs_.m_fOutLineWide ),
			m_d3dxColorOutLine( rhs_.m_d3dxColorOutLine ), m_d3dxColor( rhs_.m_d3dxColor ),
			m_bAlphaBlend( rhs_.m_bAlphaBlend )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		, m_fFlickerColorGap( rhs_.m_fFlickerColorGap )
#endif //SERV_ADD_LUNATIC_PSYKER
		{}

		void Initializer()
		{
			m_eRenderType		= CKTDGXRenderer::RT_REAL_COLOR;
			m_eCartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
			m_fOutLineWide		= 0.0f;
			m_d3dxColorOutLine	= 0xffffffff;
			m_d3dxColor			= 0xffffffff;
			m_bAlphaBlend		= false;
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			m_fFlickerColorGap	= 0.f;
#endif //SERV_ADD_LUNATIC_PSYKER
		}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        void    AddRef()    {   ++m_uRefCount; }
        void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
    private:
        BuffRenderParam& operator = ( const BuffRenderParam& );
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	};

	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffRenderParamDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffRenderParamDisplayerTemplet() : CX2BuffDisplayerTemplet(), 
		m_stBuffRenderParam()
	{}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );

private:
	BuffRenderParam			m_stBuffRenderParam;
};

typedef	CX2BuffRenderParamDisplayerTemplet::BuffRenderParam StBuffRenderParam;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<StBuffRenderParam> StBuffRenderParamPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<StBuffRenderParam> StBuffRenderParamPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
IMPLEMENT_INTRUSIVE_PTR(StBuffRenderParam);

/** @class : CX2BuffRenderParamByUnitTypeDisplayerTemplet
	@brief : 게임유닛의 타입별로 RenderParam을 바꾸는 표현 클래스
	@date : 2012/8/6/
*/
class CX2BuffRenderParamByUnitTypeDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:	
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffRenderParamByUnitTypeDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffRenderParamByUnitTypeDisplayerTemplet() : CX2BuffDisplayerTemplet()
	{
		m_mapStBuffRenderParamPtr.clear();
	}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );

private:
	map<CX2Unit::UNIT_TYPE, StBuffRenderParamPtr>		m_mapStBuffRenderParamPtr;
};

/** @class : CX2BuffEffectSetDisplayerTemplet
	@brief : 버프에 의해서 발생하는 이펙트셋을 출력하는 클래스
	@date : 2012/8/6/
*/
class CX2BuffEffectSetDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffEffectSetDisplayerTemplet() : CX2BuffDisplayerTemplet() 
	{
		m_vecHandleEffectSet.clear();
		m_vecWstrEffectSetName.clear();
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 교체 되는 타입의 버프일 경우 해당 Flag 가 True 일 때, 이펙트 셋을 다시 뿌려준다.
		m_bIsReplayEffectSetAccumulationType1 = false;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편		
		
	}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );

private:
	vector<CX2EffectSet::Handle>		m_vecHandleEffectSet;
	vector<wstring>						m_vecWstrEffectSetName;		/// 중첩시 다른 이펙트셋을 보여주기위한 벡터 (여러개의 이펙트셋이 아님)
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 교체 되는 타입의 버프일 경우 해당 Flag 가 True 일 때, 이펙트 셋을 다시 뿌려준다.
	bool								m_bIsReplayEffectSetAccumulationType1;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편	
};

/** @class : CX2BuffUnitSlashTraceDisplayerTemplet
	@brief : 유닛에 붙는(무기가아닌) 트레이스를 출력하는 클래스
	@date : 2012/8/6/
*/
class CX2BuffUnitSlashTraceDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffUnitSlashTraceDisplayerTemplet); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffUnitSlashTraceDisplayerTemplet() : CX2BuffDisplayerTemplet(), m_eSlashTraceCondition( 0 )
	{}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );

private:
	UINT m_eSlashTraceCondition;
};

/** @class : CX2BuffEffectSetImpactPointDisplayerTemplet 
	@brief : 이펙트 셋을 특정 시점에 출력하는 클래스
	@date : 2012/8/6/
*/
class CX2BuffEffectSetImpactPointDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetImpactPointDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );

	void StartDisplayerByType( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	void DoFinishByType( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffEffectSetImpactPointDisplayerTemplet() : CX2BuffDisplayerTemplet()
		, m_wstrEffectSetName()
		, m_wstrSoundName()
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
		, m_fCreateGap( 0.f )
#endif // SERV_NEW_DEFENCE_DUNGEON
	{}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );

private:
	wstring		m_wstrEffectSetName;
	wstring		m_wstrSoundName;
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
	float		m_fCreateGap;	// 이펙트 생성 간격
#endif // SERV_NEW_DEFENCE_DUNGEON
};

/** @class : CX2BuffWeaponParticleDisplayerTemplet
	@brief : 무기에 파티클을 붙이는 템플릿 (메이저 파티클만 고려함)
	@date : 2012/8/28/
*/
class CX2BuffWeaponParticleDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffWeaponParticleDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	void StartDisplayerByBuffTempletID( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffWeaponParticleDisplayerTemplet() : CX2BuffDisplayerTemplet(),
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_eFrameMoveType( FMT_DEFAULT )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_delegateOnFrameMove( DelegateOnFrameMoveByType() )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_vecHandleParticle.clear();
		m_vecWstrParticleName.clear();
	}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );
	void OnFrameMoveSwordFire( CX2GameUnit* pGameUnit_ );
	void OnFrameMoveSwordEnchant( CX2GameUnit* pGameUnit_ );

private:
	vector<CKTDGParticleSystem::CParticleEventSequenceHandle>	m_vecHandleParticle;
	vector<wstring>	m_vecWstrParticleName;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    enum EFrameMoveType
    {
        FMT_DEFAULT = 0,
        FMT_SWORD_FIRE = 1,
        FMT_ENCHANT = 2,
    };
    EFrameMoveType  m_eFrameMoveType;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	typedef srutil::delegate1<void, CX2GameUnit*> DelegateOnFrameMoveByType;
	DelegateOnFrameMoveByType m_delegateOnFrameMove;	/// 버프템플릿아이디 별 OnFrameMove를 지정
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
};

/** @class : CX2BuffAfterImageDisplayerTemplet 
	@brief : 표현요소로 AfterImage를 실행 시키는 템플릿
	@date : 2012/8/28/
*/
class CX2BuffAfterImageDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffAfterImageDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	
protected:
	CX2BuffAfterImageDisplayerTemplet() 
		: CX2BuffDisplayerTemplet(), m_d3dxColor( 0x000000ff )
	{}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );
	
private:
	D3DXCOLOR	m_d3dxColor;
};

/** @class : CX2BuffSoundDisplayTemplet
	@brief : 표현요소로 사운드를 출력하는 템플릿
	@date : 2012/12/05/
*/
class CX2BuffSoundDisplayTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffSoundDisplayTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	
protected:
	CX2BuffSoundDisplayTemplet() 
		: CX2BuffDisplayerTemplet(), m_wstrSoundName()
	{}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );
	
private:
	wstring		m_wstrSoundName;
};

#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
/** @class : CX2BuffWeaponAfterImageDisplayerTemplet 
	@brief : 표현요소로 무기에 AfterImage를 실행 시키는 템플릿
	@date : 2012/8/28/
*/
class CX2BuffWeaponAfterImageDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffWeaponAfterImageDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	
protected:
	CX2BuffWeaponAfterImageDisplayerTemplet() 
		: CX2BuffDisplayerTemplet(), m_d3dxColor( 0x000000ff )
	{}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );
	
private:
	D3DXCOLOR	m_d3dxColor;
};
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-10
/** @class : CX2BuffEffectSetCreateGapDisplayerTemplet
	@brief : 버프에 의해서 일정 간격마다 발생하는 이펙트셋을 출력하는 클래스
	@date : 2013/4/10/
*/
class CX2BuffEffectSetCreateGapDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetCreateGapDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

protected:
	CX2BuffEffectSetCreateGapDisplayerTemplet() : CX2BuffDisplayerTemplet() 
	{
		m_vecHandleEffectSet.clear();
		m_vecPairEffectSetNameAndGap.clear();
		m_vecCheckElapsedTimeForEffectSet.clear();
	}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );

private:
	vector<CX2EffectSet::Handle>						m_vecHandleEffectSet;				/// 생성한 이펙트에 대한 핸들 저장 벡터 ( 생성 순서대로 들어갈 뿐, 설정한 이펙트 셋 순서는 무관 )
	vector<std::pair<wstring, float>>					m_vecPairEffectSetNameAndGap;		/// 설정한 이펙트셋의 이름 및 생성 간격 저장 벡터
	vector<std::pair<wstring, CKTDXCheckElapsedTime>>	m_vecCheckElapsedTimeForEffectSet;	/// 생성 간격 연산용 쿨타임 객체 저장 벡터
};
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef HAMEL_SECRET_DUNGEON // 김태환
/** @class : CX2BuffEffectSetOnlyMyClassDisplayerTemplet
	@brief : 버프에 의해서 발생하는 이펙트셋을 출력하는 클래스
	@date : 2012/8/6/
*/
class CX2BuffEffectSetOnlyMyClassDisplayerTemplet : public CX2BuffDisplayerTemplet
{
public:
	static CX2BuffDisplayerTempletPtr	CreateBuffDisplayerTempletPtr() { return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetOnlyMyClassDisplayerTemplet ); }

	virtual void StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffDisplayerTempletPtr	GetClonePtr() const;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffEffectSetOnlyMyClassDisplayerTemplet() : CX2BuffDisplayerTemplet()
		 , m_hEffectSetHandle( INVALID_EFFECTSET_HANDLE )
	{
		m_vecEffectSetName.clear();
	}

	virtual bool ParsingDisplayer( KLuaManager& luaManager_ );

private:
	vector<wstring>				m_vecEffectSetName;
	CX2EffectSet::Handle		m_hEffectSetHandle;
};
#endif // HAMEL_SECRET_DUNGEON