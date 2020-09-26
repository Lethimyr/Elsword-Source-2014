#pragma once
// 2013-02-20 // 오현빈
// GameMajorParticle / GameXMeshPlayer / DamageEffect / Effectset
// 상기 스크립트 파싱 및 생성을 위한 클래스.


class CX2EffectToolFileManager
{
public:
	CX2EffectToolFileManager(CX2EffectToolMain* pEffectTool_);
	~CX2EffectToolFileManager(void);

public:
	//public ~
	void SaveMajorParticle( const WCHAR* pwszFileFullPath_, const vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet_ );
	void SaveMajorMeshPlayer( const WCHAR* pwszFileFullPath_, CKTDGXMeshPlayer* pMeshTemplet_ );
	
	//현재 사용 하고 있는 단일 이펙트셋에 대한 저장 함수
	void SaveEffectSet( const WCHAR* pwszFileFullPath_, 
					vector<CX2EffectToolMain::TimedXMeshPlayer> vecMeshPlayerList_,
					vector<CX2EffectToolMain::TimedParticle> vecMajorParticleList_,
					vector<CX2EffectToolMain::CameraShake> vecCameraShakeList_);


	void SaveEffectSetWhole( const WCHAR* pwszFileFullPath_, const std::vector< CX2EffectSet::EffectSetData* >* vecEffectTemplet_ );
	void SaveEffectSet_Desc( const WCHAR* pwszFileFullPath_, const std::vector< CX2EffectSet::EffectSetData* >* vecEffectTemplet_ );

	void AddDamageEffect( const WCHAR* pwszFileFullPath_, const WCHAR* pwszDamageEffect_ );
	static std::string GetStringByEventData( CKTDGParticleSystem::CParticleEvent* pEventData_ );
	static std::string GetStringByEventData( CKTDGXMeshPlayer::CXMeshEvent* pEventData_ );

	//~ public
private:
	//private ~
	bool OpenFileByFieldPullPath( const WCHAR* pwszFileFullPath_, OUT std::ofstream& ofs_);
	bool ConverFilesAnsiToUTF8ByPullPath( const WCHAR* pwszFileFullPath_ );

//////////////////////////////////////////////////////////////////////////
#pragma region DamageEffect
	//DamageEffect

	//DamageEffect
#pragma endregion DamageEffect
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#pragma region EffectSet
//EffectSet
	void SaveEffectSet_MeshPlayer( std::ofstream& ofs_, vector<CX2EffectToolMain::TimedXMeshPlayer> vecMeshPlayerList_ );
	void SaveEffectSet_MajorParticle( std::ofstream& ofs_, vector<CX2EffectToolMain::TimedParticle> vecMajorParticleList_ );
	void SaveEffectSet_CameraCrash( std::ofstream& ofs_, vector<CX2EffectToolMain::CameraShake> vecCameraSahkeList_ );

//EffectSet
#pragma endregion EffectSet
//////////////////////////////////////////////////////////////////////////
	inline std::string GetStringEventTimerType( CX2EffectSet::EFFECT_SET_TIMER_TYPE eEffectTimerType_ );
	void SaveEffectSet_EffectData( std::ofstream& ofs_, std::vector<CX2EffectSet::EffectData*> pEffectData_ );
	void SaveEffectSet_CameraShake( std::ofstream& ofs_, std::vector<CX2EffectSet::CameraShakeData*> pCameraData_ );
	void SaveDamageEffect( const WCHAR* wcDamageEffectName_ );

	inline std::string GetStringShakeType( const char* pTemp_, CKTDGCamera::SHAKE_TYPE eEffectType_ );
	inline std::string GetStringEffectType( const char* pTemp_, CX2EffectSet::EFFECT_TYPE eEffectType_ );
//////////////////////////////////////////////////////////////////////////
#pragma region GameMajorXMeshPlayer
//GameMajorXMeshPlayer
	//GameMajorParitcle 생성
	void SaveMeshPlayer_BasicData( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );
	void SaveMeshPlayer_EventList( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );
		
	///enum
	void SaveMeshPlayer_EnumProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );	

	///CMinMax<float>
	void SaveMeshPlayer_NumericProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );

	///CMinMax<D3DXVECTOR3>
	void SaveMeshPlayer_VecProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );

	///bool
	void SaveMeshPlayer_BoolProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );

	///wstring
	void SaveMeshPlayer_StringProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );

	//CMinMax<float> 이벤트타입
	void SaveMeshPlayer_EventList_Common( std::ofstream& ofs_, const char* strEventType_, vector<CKTDGXMeshPlayer::CXMeshEvent*> vecEventList_);
//GameMajorXMeshPlayer
#pragma endregion GameMajorXMeshPlayer
//////////////////////////////////////////////////////////////////////////
	
//////////////////////////////////////////////////////////////////////////
#pragma region GameMajorParitcle
//GameMajorParticle.
	//GameMajorParitcle 생성
	void SaveParticle_BasicData( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ );
	void SaveParticle_EventList( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ );

	/// SeqNumericProp
	void SaveParticle_NumericProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ );
	inline std::string GetStringNumericProp( const char* pTemp_, int iValue_, int iEndTabCnt_ );
	inline std::string GetStringNumericProp( const char* pTemp_, float fValue_, int iEndTabCnt_ );
	inline std::string GetStringNumericProp( const char* pTemp_, const CMinMax<float>* cMinMaxFloat_, int iEndTabCnt_ );
	inline std::string GetStringNumericProp( const char* pTemp_, const CMinMax<int>* cMinMaxInt_, int iEndTabCnt_ );

	// 오현빈 // Set / Get에서의 수치가 다른 방식으로 처리 하기 때문에 ConvertRadianToDegreeByMinMax 함수로 예외처리
	inline const CMinMax<float> ConvertRadianToDegree( const CMinMax<float>* cMinMaxFloat_ ) const { return CMinMax<float>( cMinMaxFloat_->m_Min / D3DX_PI*180.f,  cMinMaxFloat_->m_Max / D3DX_PI*180.f );}
	inline const D3DXVECTOR3	ConvertRadianToDegree( const D3DXVECTOR3* vec_	) const { return ((*vec_) / D3DX_PI*180.f) ;}
	
	/// SeqVectorProp
	void SaveParticle_VectorProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ );
	inline std::string GetStringVectorProp( const char* pTemp_, const CMinMax<D3DXVECTOR3>* cMinMaxVec_, int iEndTabCnt_ );
	inline std::string GetStringVectorProp( const char* pTemp_, const D3DXVECTOR3* vecValue_, int iEndTabCnt_ );
	
	///enum
	void SaveParticle_EnumProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ );	

	/// SeqBoolProp	
	void SaveParticle_BoolProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ );	
	inline std::string GetStringBoolProp( const char* pTemp_, bool bBool_, int iEndTabCnt_ );

	/// SeqStringProp
	void SaveParticle_StringProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ );		
	inline std::string GetStringStringProp( const char* pStrType_, const WCHAR* pStrValue_, int iEndTabCnt_ ) const;

//GameMajorParticle.
#pragma endregion GameMajorParitcle
//////////////////////////////////////////////////////////////////////////

	static std::string GetStringBlend(  const char* pTemp_, int iBlendMode_ );
	static std::string GetStringParticleType( CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ );	
	static std::string GetStringLifeType(  const char* pTemp_, CKTDGXMeshPlayer::LIFE_TYPE eLifeType_ , int iEndTabCnt_ = 3);
	static std::string GetStringMeshType(  const char* pTemp_, CKTDGXMeshPlayer::MESH_TYPE eMeshType_, int iEndTabCnt_ = 3 );	
	static std::string GetStringRenderType(  const char* pTemp_, CKTDGXRenderer::RENDER_TYPE eRenderType_ );
	static std::string GetStringRenderType( CKTDGXRenderer::RENDER_TYPE eRenderType_ );
	static std::string GetStringCartoonTexType(  const char* pTemp_, CKTDGXRenderer::CARTOON_TEX_TYPE eCartoonType_ , int iEndTabCnt_ = 3 );
	static std::string GetStringCullMode(  const char* pTemp_, D3DCULL eCullMode_ );
	static std::string GetStringSlash_Trace_Curve_Type(  const char* pTemp_, CKTDGSlashTrace::CURVE_TYPE eSlashTraceCurveType_ );
	static std::string GetStringBillBoardType(  const char* pTemp_, CKTDGMatrix::BILLBOARD_TYPE eBillBoardType_ , int iEndTabCnt_ = 2 );

	static inline std::string GetStringTabAndKeword( const char* pTemp_, int iEndTabCnt_ );
	// random( x, x)로 표현해야 하는 범위형 값 인지에 대한 여부
	static inline bool	IsRangeValue( float fMinValue_, float fMaxValue_ ) 	{return (fMinValue_ != fMaxValue_ );}
	static inline bool	IsRangeValue( const CMinMax<float>* CMinMaxValue ) 	{return (CMinMaxValue->m_Min != CMinMaxValue->m_Max);}

	//ActualTime에 맞는 스트링 만들기. ( Fade so final / fade so from / Initial / at )
	static std::string GetStringByActualTime( bool bFade_, const CMinMax<float>* cMinMaxFloat_);
	static std::string	GetStringByCMinMax( const CMinMax<D3DXCOLOR>* cMinMaxColor_ ) ; // return RGBA(1,1,1,1)
	static std::string	GetStringByCMinMax( const CMinMax<D3DXVECTOR2>* cMinMaxVec_ ) ; // return XY(1,1) 
	static std::string	GetStringByCMinMax( const CMinMax<D3DXVECTOR3>* cMinMaxVec_ ) ; // return XYZ(1,1,1)
  	static std::string	GetStringByCMinMax( const CMinMax<float>* cMinMaxFloat_ ) ; // return 1
	static std::string	GetStringByCMinMax( float fMinValue_, float fMaxValue_ ) ;	// return 1
	static std::string	GetStringByVec( const D3DXVECTOR3* vecValue_ ) ;	// return xyz(1,1,1)
	static std::string	GetStringIntByFloat( float fFalue_) ;	// return xyz(1,1,1)
	//~ private

	//1000단위 기호 강제 표시되는 문제 때문에,
	//-1000 이하, 1000이상인지에 대한 체크 함수
	inline bool IsValidRange( const float fValue_ )
	{
		return (1000 > fValue_) && (-1000 < fValue_);
	}


private:

	int					m_iEffectSetCount;		// 이펙트셋 번호
	CX2EffectToolMain*	m_pEffectToolMain;

	std::set<wstring>	m_setAddDamageEffectList;
};

