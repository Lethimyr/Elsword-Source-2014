#pragma once
//////////////////////////////////////////////////////////////////////////
// 2009.12 태완
// X2Viewer에서 파티클 렌더링을 위해 사용하는 class.
// 파티클 수정 기능 관련된 데이터도 일부 가지고 있다.
//
// 
class CX2ViewerParticle : public CX2ViewerObject
{
public:

	enum PARTICLEOBJECT_MODE
	{
		PM_NORMAL,
		PM_EDITOR,
	};

	// 파티클을 메쉬 / 스킨메쉬와 화면에 표시할 때 사용하는, 관련 정보를 담은 구조체.
	// NPC 스크립트의 PARTICLE_SEQ에 들어가는 data를 모두 포함한다. ( 스크립트로 뽑아낼 수 있도록 )
	struct ParticleEffectData
	{
		bool		m_bIsTemplet;
		bool		m_bUse;
		//bool		m_bMajor;		// Tool에선 안 쓴다.
		float		m_fTime;
		wstring		m_Name;
		wstring		m_SequenceName;
		bool		m_bWeapon;
		wstring		m_Pos;
		//bool		m_StateEndDelete;		// 모션이 시작할 때마다 리셋할 것.
		bool		m_bTrace;
		D3DXVECTOR3 m_vOffset;
		bool		m_bLandPosition;
		bool		m_bApplyUnitRotation;
		D3DXVECTOR3	m_vRotation;
		D3DXVECTOR3 m_vScale;
		bool		m_bUnitScale;


	#ifdef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE //JHKang
		CKTDGParticleSystem::CParticleHandle m_hSeq;
	#else //X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;
	#endif //X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		ParticleEffectData()
		{
			m_bIsTemplet		= true;
			m_bUse				= false;
			//m_bMajor			= false;
			m_fTime				= 0.0f;
			m_bWeapon			= false;
			//m_StateEndDelete	= false;
			m_hSeq				= INVALID_PARTICLE_HANDLE;
			m_bTrace			= true;
			m_vOffset			= D3DXVECTOR3(0, 0, 0);
			m_bLandPosition		= false;
			m_bApplyUnitRotation = false;
			m_vRotation			= D3DXVECTOR3(0, 0, 0);
			m_vScale            = D3DXVECTOR3(1, 1, 1);
			m_bUnitScale		= false;

		}
		~ParticleEffectData(){}

	};

	CX2ViewerParticle( CX2ViewerSkinMesh* pSkinMeshObj );
	virtual ~CX2ViewerParticle(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	CKTDGParticleSystem*		GetParticleSystem()			{ return m_pParticleSystem; }
	CKTDGParticleSystem*		GetCustomParticleSystem()			{ return m_pCustomParticleSystem; }

	int					GetParticleEffectNum(){ return m_ParticleEffData.size(); }
	vector<ParticleEffectData*>& GetParticleEffectData(){ return m_ParticleEffData; }
	ParticleEffectData* GetParticleEffectDataByName( const wstring name );
	bool				AddParticleEffectData( ParticleEffectData* pData );
	bool				DeleteParticleEffectData( const wstring name );
	void				SaveParticleEffectData( const WCHAR* pwszFileFullPath );
	
	void				ResetAllParticle();
	void				ReloadParticleFile();

	void				SetMode( PARTICLEOBJECT_MODE pm ){ m_mode = pm; }

	void				SetPreviewParticle( wstring name, bool bIsTemplet );
	bool				CopyTempletParticleToCustom( wstring name );
	
	CX2ViewerParticleEditor&	GetParticleEditor(){ return m_X2ParticleEditor; }
	void				ReplayPreviewParticle();
	void				SetPreviewPlayMode( bool bIsReplay ){ m_bPreviewRepeat = bIsReplay; }
	bool				GetPreviewPlayMode(){ return m_bPreviewRepeat; }
	void				SetPreviewPlaySpeed( float fPreviewPlaySpeed ){ m_fPreviewPlaySpeed = fPreviewPlaySpeed; }
	float				GetPreviewPlaySpeed(){ return m_fPreviewPlaySpeed; }
	void				SetPreviewPlayTime( float time ){ m_fElapsedPlayTime = time; }
	float				GetPreviewPlayTime(){ return m_fElapsedPlayTime; }


private:
	CKTDGParticleSystem*		m_pParticleSystem;				// 기존의 Particle은 여기서 관리된다.
	CKTDGParticleSystem*		m_pCustomParticleSystem;		// 유저가 수정한 Paticle Templet을 갖는다. 파티클 수정 기능으로 수정한
																// 파티클들은 이 아래에서 관리된다.

	vector<ParticleEffectData*>						m_ParticleEffData;		// Model Viwe 상태에서 표시할 파티클 리스트
	CX2ViewerSkinMesh*			m_pSkinMeshObj;								// X2Viewer에서 표시하고 있는 스킨메시
	int m_iLastPlayCount;
	wstring m_LastAnimName;
	
	PARTICLEOBJECT_MODE m_mode;									// Model view인지, 파티클 수정 상태인지

	ParticleEffectData			m_PreviewParticleEffData;		// Particle Edit모드에서 표시할 파티클
	bool						m_bPreviewRepeat;
	CX2ViewerParticleEditor		m_X2ParticleEditor;
	float						m_fPreviewPlaySpeed;
	float						m_fElapsedPlayTime;

};
