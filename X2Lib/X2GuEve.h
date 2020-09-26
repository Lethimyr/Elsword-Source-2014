#pragma once

class CX2GUEve : public CX2GUUser
{
public:
	enum EVE_STATE_ID
	{
		ENSI_BASE						= USI_END + 1,
		
		ENSI_DASH_JUMP_POWER_LANDING,

		ENSI_JUMP_FLY_UP,
		ENSI_JUMP_FLY_DOWN,
		ENSI_JUMP_DASH,

		ENSI_STANDUP_ROLLING_FRONT_FRONT,
		ENSI_STANDUP_ROLLING_FRONT_BACK,
		ENSI_STANDUP_ROLLING_BACK_FRONT,
		ENSI_STANDUP_ROLLING_BACK_BACK,

		ENSI_STANDUP_ATTACK_FRONT,
		ENSI_STANDUP_ATTACK_BACK,

		ENSI_INVISIBILITY_ON,


		ENSI_COMBO_Z,
		ENSI_COMBO_ZZ,
		ENSI_COMBO_ZZZ,
		ENSI_COMBO_ZZZZ,

		ENSI_COMBO_ZZZX,
		ENSI_COMBO_ZZZdownZ,
		ENSI_COMBO_ZZZdownZZ,



		ENSI_COMBO_X,
		ENSI_COMBO_XX,
		ENSI_COMBO_XXX,

		ENSI_COMBO_XXfrontX,
		ENSI_COMBO_XXfrontXX,

		ENSI_COMBO_XXdownX,
		ENSI_COMBO_XXdownXX,




		ENSI_DASH_COMBO_Z,
#ifdef EVE_COMBO_TEST_TEST
		ENSI_DASH_COMBO_ZX,
#endif EVE_COMBO_TEST_TEST
		ENSI_DASH_COMBO_X,

		ENSI_JUMP_COMBO_Z,
		ENSI_JUMP_COMBO_X,
		ENSI_JUMP_COMBO_X_LANDING,

		ENSI_DASH_JUMP_COMBO_Z,
		ENSI_DASH_JUMP_COMBO_ZZ,
		ENSI_DASH_JUMP_COMBO_ZZZ,

		ENSI_DASH_JUMP_COMBO_X,


//#ifdef WALL_JUMP_TEST
		ENSI_WALL_LANDING,
//#endif WALL_JUMP_TEST

#ifdef EVE_FIRST_CHANGE_JOB			
		ENSI_COMBO_XXZ4,
		ENSI_DASH_COMBO_ZZ,
		ENSI_DASH_COMBO_ZZZ,
		ENSI_DASH_COMBO_XZ,
		ENSI_DASH_JUMP_COMBO_ZX,
		ENSI_DASH_JUMP_COMBO_XZ,
#endif EVE_FIRST_CHANGE_JOB


//{{ kimhc // 2010.1.25 // 이브 2차 전직 스테이트 ID
#ifdef	EVE_SECOND_CLASS_CHANGE 

		ENSI_NS_DASH_COMBO_ZX,
		ENSI_NS_DASH_COMBO_ZXX,

		ENSI_NS_COMBO_XXZ,
		ENSI_NS_COMBO_XXZZ8,		// 총 Z키 8번의 입력을 받는 스테이트
		ENSI_NS_COMBO_XXZZ8Z,	

		ENSI_EP_DASH_COMBO_ZZ,
		ENSI_EP_DASH_COMBO_ZZZ,
		ENSI_EP_JUMP_COMBO_ZX,
		ENSI_EP_COMBO_XXdownX,

		ENSI_A_ENS_BURST_SPEAR_Landing,
		ENSI_A_ENS_BURST_SPEAR_Landing_HYPER,

#endif	EVE_SECOND_CLASS_CHANGE
//}} kimhc // 2010.1.25 // 이브 2차 전직 스테이트 ID

#ifdef EVE_ELECTRA
		ENSI_EPR_COMBO_ZZZfrontZ,
		ENSI_EPR_COMBO_XXZ,
		ENSI_EPR_DASH_COMBO_ZZ,
		ENSI_EPR_DASH_COMBO_ZZZ,
		ENSI_SA_EPR_SWEEP_ROLLING_CHARGE,
		ENSI_SA_EPR_SWEEP_ROLLING_FIRE,
		ENSI_SA_EPR_SWEEP_ROLLING_END,
#endif EVE_ELECTRA

#ifdef SERV_EVE_BATTLE_SERAPH
		ENSI_EBS_BOOST_DASH_START,		/// 부스트 대쉬 시작
		ENSI_EBS_BOOST_DASH,			/// 부스트 대쉬

		ENSI_EBS_BOOST_DASH_AIR_START,	/// 공중 부스트 대쉬 시작
		ENSI_EBS_BOOST_DASH_AIR,		/// 공중 부스트 대쉬

		ENSI_EBS_DASH_JUMP_COMBO_XZ,	/// 대쉬 점프 콤보 X(Push)Z

		ENSI_EBS_DASH_JUMP_COMBO_ZFrontX,	/// 대쉬 점프 콤보 ZXXX, (공중) 부스트 대쉬 XXX Front
		ENSI_EBS_DASH_JUMP_COMBO_ZUpX,		/// 대쉬 점프 콤보 ZXXX, (공중) 부스트 대쉬 XXX Up
		ENSI_EBS_DASH_JUMP_COMBO_ZDownX,	/// 대쉬 점프 콤보 ZXXX, (공중) 부스트 대쉬 XXX Down
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ENSI_SA_EEN_SWEEP_ROLLING_TRI_VULCAN_FIRE,
		ENSI_SA_EEN_SWEEP_ROLLING_TRI_VULCAN_END,
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		ENSI_PARRYING_SMALL,
		ENSI_PARRYING,
		ENSI_PARRYING_BLINK,
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어

#ifdef FIX_ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템 수정 사항
		ENSI_BERSERK_MODE,
#endif // FIX_ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템 수정 사항




		//////////////////////////////////////////////////////////////////////////
		// 위쪽에 추가해주세요~ 이 아래는 테스트로 추가된 enum 입니다


		ENSI_TRANSFORMED,

//#ifdef PVP_BOSS_COMBAT_TEST
//		ENSI_FROZEN,
//#endif PVP_BOSS_COMBAT_TEST

	};

	enum MajorMeshInstanceID
	{
		MajorMID_DashComboZ,
		MajorMID_DashComboZZ,
		MajorMID_DashComboZZZ,
		MajorMID_ComboZZZdownZ,
		MajorMID_ComboZZZdownZ_EG,

		MajorMID_IllusionStinger01,
		MajorMID_IllusionStinger02,

		MajorMID_GenBlackHole01,
		MajorMID_GenBlackHole02,
		MajorMID_GenBlackHole03,
		MajorMID_DashJumpComboZX,
		MajorMID_DashJumpComboXZ,

		MajorMID_ExplosionImpact01,			

		MajorMID_AT_SP3,
		MajorMID_EG_SP3_Wing,

		MajorMID_EG_SP3_Combo_Z1,
		MajorMID_EG_SP3_Combo_Z2,
		MajorMID_EG_SP3_Combo_Z3,
		MajorMID_EG_SP3_Combo_Z4_01,
		MajorMID_EG_SP3_Combo_Z4_02,

		MajorMID_EG_SP3_Jump_Attack_Z_01,
		MajorMID_EG_SP3_Jump_Attack_Z_02,
		MajorMID_EG_SP3_Jump_Attack_X_01,
		MajorMID_EG_SP3_Jump_Attack_X_02,

		MajorMID_EG_SP3_Dash_Jump_Combo_Z_01,
		MajorMID_EG_SP3_Dash_Jump_Combo_Z_02,
		MajorMID_EG_SP3_Dash_Jump_Combo_ZZ_01,
		MajorMID_EG_SP3_Dash_Jump_Combo_ZZ_02,
		MajorMID_EG_SP3_Dash_Jump_Combo_ZZZ_01,
		MajorMID_EG_SP3_Dash_Jump_Combo_ZZZ_02,

		//{{ kimhc // 2010-03-09 // effect들을 effectset으로 바꿈
		// 		MajorMID_AT_Start,
		// 		MajorMID_AT_Win,
		// 		MajorMID_AT_Win1,
		// 		MajorMID_AT_Lose,
		// 		MajorMID_AT_Lose1,
		// 		MajorMID_EG_Win1,
		//}} kimhc // 2010-03-09 // effect들을 effectset으로 바꿈
#ifdef SKILL_30_TEST
		MajorMID_AT_Overon_Guard_01,
		MajorMID_AT_Overon_Guard_02,

		MajorMID_EG_Metal_Dust_Aura_01,
		MajorMID_EG_Metal_Dust_Aura_02,
#endif SKILL_30_TEST

		//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
		MajorPID_SPEAR_BURST01,			/// 스피어 버스트 01
		MajorPID_SPEAR_BURST02,			/// 스피어 버스트 02
		MajorPID_SPEAR_BURST03,			/// 스피어 버스트 03
		MajorPID_SPEAR_BURST_DUMMY,		/// 스피어 버스트 더미 어택박스

		MajorPID_ATOMIC_BLASTER_GUN,	/// 아토믹 블래스터 메인 총
		MajorPID_ATOMIC_BLASTER_DUMMY,	/// Attack Box
		MajorPID_ATOMIC_BLASTER_BULLET,	/// 총알 떨어지는 mesh
#endif	EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.2.3 //	이브 2차 전직

		MajorMID_END,
	};

	enum MinorMeshInstanceID
	{
		MinorMID_TEST1,

		MinorMID_END,
	};

	enum EVE_MAJOR_PARTICLE_INSTANCE_ID
	{
		EVE_MAJOR_PII_ILLUSION_STRIKE_01,
		EVE_MAJOR_PII_ILLUSION_STRIKE_02,

		EVE_MAJOR_PII_ILLUSION_STINGER_01,
        EVE_MAJOR_PII_ILLUSION_STINGER_02,
        
		EVE_MAJOR_PII_DASH_JUMP_COMBO_X_01,
		EVE_MAJOR_PII_DASH_JUMP_COMBO_X_02,
		
#ifdef SKILL_30_TEST
		EVE_MAJOR_PII_OVERON_GUARD_01,
		EVE_MAJOR_PII_OVERON_GUARD_02,
#endif SKILL_30_TEST

		EVE_MAJOR_PII_END,
	};

	enum EVE_MINOR_PARTICLE_INSTANCE_ID
	{
		EVE_MINOR_PII_BASE,
		EVE_MINOR_PII_END,
	};

	// 장착된 액티브 스킬중 자주 호출되는 체크데이터를 미리 담아두기 위해..
	enum EquippedActiveSkillValID
	{
		EASI_NONE = 0,
		//EASI_MEGA_EBALL_SLOT_ID,

		EASI_END,
	};


	


#ifdef OLD_HAND_SLASH_TRACE
//{{AFX
	enum SlashTraceID
	{
		SlashID_LHand,
		SlashID_RHand,

		SlashID_END,
	};


	// fix!! 나중에 cx2gameunit::weapon에 있는거랑 합쳐서 정리하고 gameunit으로 옮기자. 
	struct SlashTraceData
	{
		CKTDGSlashTrace::SLASH_TRACE_TYPE m_eType;

		CKTDGSlashTrace*		m_pSlashTrace;
		CKTDGSlashTrace*		m_pSlashTraceTip;

		//D3DXCOLOR				m_coSlashTrace;
		//D3DXCOLOR				m_coSlashTraceHyper;
		//D3DXCOLOR				m_coSlashTraceTip;
		//D3DXCOLOR				m_coSlashTraceTipHyper;

		CKTDXDeviceXSkinMesh::MultiAnimFrame*	m_pFrame_TRACE_START;		// slash trace가 붙는 frame
		CKTDXDeviceXSkinMesh::MultiAnimFrame*	m_pFrame_TRACE_END;

		SlashTraceData()
		{
			m_eType			= CKTDGSlashTrace::STT_DEFAULT;

			m_pSlashTrace		= NULL;
			m_pSlashTraceTip	= NULL;

			m_pFrame_TRACE_START = NULL;
			m_pFrame_TRACE_END	= NULL;

			//m_coSlashTrace				= 0xcc5555ff;
			//m_coSlashTraceHyper 		= 0xccff3333;
			//m_coSlashTraceTip			= 0xff5555ff;
			//m_coSlashTraceTipHyper		= 0xffff3333;
		}

		~SlashTraceData()
		{
			CKTDGSlashTrace::DeleteKTDGObject( m_pSlashTrace );
			CKTDGSlashTrace::DeleteKTDGObject( m_pSlashTraceTip );
			//SAFE_DELETE( m_pSlashTrace );
			//SAFE_DELETE( m_pSlashTraceTip );
		}


		void SetShowObject( bool bShow )
		{
			if( NULL != m_pSlashTrace )
				m_pSlashTrace->SetShowObject( bShow );

			if( NULL != m_pSlashTraceTip )
				m_pSlashTraceTip->SetShowObject( bShow );
		}

	};
//}}AFX
#endif OLD_HAND_SLASH_TRACE


#ifdef EVE_ELECTRA
	enum EveElectraEffectType
	{
		EEET_LASER,		/// 레이저
		EEET_EFFECT,	/// 이펙트
		EEET_LOCKON,	/// 조준
	};

	enum PlaneFormationType
	{
		PFT_AMPLIFIER,	/// 증폭
		PFT_SPECTRUM,	/// 분광
//#ifdef SERV_EVE_BATTLE_SERAPH
		PFT_INDUCTION,	/// 반응
		PFT_FUSION,		/// 융합
		PFT_NONE,		/// 없음
//#endif
	};

	struct EveElectraSystem;
	struct EveElectraLaserData
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        vector<CX2DamageEffect::CEffectHandle>	    m_vecDamageEffect;		/// 이펙트 포인터
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		vector<CX2DamageEffect::CEffect*>	m_vecDamageEffect;		/// 이펙트 포인터
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		D3DXVECTOR3					m_vLaserStartPos;				/// 레이저 시작 위치
		D3DXVECTOR3					m_vDirVector;					/// 레이저 방향 벡터
		D3DXVECTOR3					m_vInitRotateLocalDegree;		/// 회전각
		float						m_fVelocity;					/// 속도
		float						m_fLaserPropagateTime;			/// 전파 시간
		float						m_fElapsedTime;					/// 경과 시간
		float						m_fPenetrateTime;				/// 관통 시간
		float						m_fLaserWidthScale;				/// 레이저 굵기 비율
		bool						m_bPlaneFront;					/// 평면 앞 뒤 구별
		bool						m_bPenetrate;					/// 관통 여부
		bool						m_bPenetrateGiga[4];			/// 
		wstring						m_TraceBone;					/// 뼈대
		bool						m_bDie;							/// 리스트에서 laser의 제거 여부 판단
		bool						m_bApplyLaserRangePassive;		/// 거리 증가 패시브 적용
		wstring						m_AmplifyDamageEffectName;		/// 증폭장 데미지 이펙트 이름
		wstring						m_SpectrumDamageEffectName;		/// 분광장 데미지 이펙트 이름
#ifdef SERV_EVE_BATTLE_SERAPH
		wstring						m_InductionDamageEffectName;	/// 반응장 데미지 이펙트 이름
		int							m_iFireCount;					/// 레이저 유도탄 발사 수
#endif
		EveElectraSystem*			m_pEveElectraSystem;			/// 일렉트라 시스템 포인트
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hParticleLaserCross;			/// 레이저 충돌 위치 이펙트
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hParticleGigaLaserCross[4];	/// 기가 스트림일 경우

		EveElectraLaserData( EveElectraSystem* _pEveElectraSystem );
		void OnFrameMove( float fElapsedTime );
		void Delete();

	};

	struct EveElectraEffectData
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle       m_hDamageEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*	m_pDamageEffect;				/// 데미지 이펙트
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		D3DXVECTOR3					m_vPlaneIntersectPoint;			/// 교차점 위치
		bool						m_bPlaneFront;					/// 장막 앞, 뒤
		bool						m_bDie;							/// 소멸 확인
		float						m_fElapsedTime;					/// 경과 시간
		wstring						m_AmplifyDamageEffectName;		/// 증폭장 데미지 이펙트 이름
		wstring						m_SpectrumDamageEffectName;		/// 분광장 데미지 이펙트 이름
#ifdef SERV_EVE_BATTLE_SERAPH
		wstring						m_InductionDamageEffectName;	/// 반응장 데미지 이펙트 이름
		wstring						m_FusionDamageEffectName;		/// 융합장 데미지 이펙트 이름
#endif
		EveElectraSystem*			m_pEveElectraSystem;			/// 일렉트라 시스템 포인트

		EveElectraEffectData( EveElectraSystem* _pEveElectraSystem );
		void OnFrameMove( float fElapsedTime );
	};

	struct EveElectraLockonData
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle       m_hDamageEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*	m_pDamageEffect;				/// 데미지 이펙트
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		D3DXVECTOR3					m_vPlaneIntersectPoint;			/// 교차점 위치
		bool						m_bPlaneFront;					/// 장막 앞, 뒤
		bool						m_bDie;							/// 소멸 확인
		float						m_fElapsedTime;					///	경과 시간
		wstring						m_AmplifyDamageEffectName;		/// 증폭장 데미지 이펙트 이름
		wstring						m_SpectrumDamageEffectName;		/// 분광장 데미지 이펙트 이름
#ifdef SERV_EVE_BATTLE_SERAPH
		wstring						m_InductionDamageEffectName;	/// 반응장 데미지 이펙트 이름
#endif
		EveElectraSystem*			m_pEveElectraSystem;			/// 일렉트라 시스템 포인트

		EveElectraLockonData( EveElectraSystem* _pEveElectraSystem );
		void OnFrameMove( float fElapsedTime );
	};

	struct EveElectraSystem
	{
		bool						m_bSystemEnable;			/// 분광결정을 사용 중인지 여부
		float						m_fSystemOperationTime;		/// 분광결정 유지 시간
		float						m_fFormationSetElapsedTime;	/// 몸주위에 붙는 분광결정 이펙트를 차례로 생성해 주기 위해 사용하는 경과 시간
		PlaneFormationType			m_ePlaneFormationType;		/// 증폭장, 분광장 등 분광결정 타입
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hEffectPlane;		/// 메쉬 이펙트
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hEffectCrystal1;	/// 메쉬 이펙트
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hEffectCrystal2;	/// 메쉬 이펙트
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hEffectCrystal3;	/// 메쉬 이펙트

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hParticlePlane;		/// 평면 메쉬
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hParticleCrystal[3];	/// 크리스탈 메쉬

		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hParticleFormationMark[3];	/// 유형 표시 파티클
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hParticleFormationMsg[3];		/// 유형 메시지 파티클

		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hParticleThousandStarFormation;	/// 천별 유형별 파티클

		bool						m_bPlaneEnable;				/// 역장 활성화
		bool						m_bSpectrumDynamicsEnable;	/// 분광 역학 활성화
		float						m_fLaserRangeScale;			/// Laser 거리 배율
		float						m_fLaserEndOffsetX;			/// X 위치 끝점
		float						m_fLaserEndOffsetY;			/// Y 위치 끝점

		D3DXPLANE					m_PrimaryPlane;			/// 평면
		D3DXVECTOR3					m_vPlaneNormal;			/// 평면 법선
		D3DXVECTOR3					m_vPlanePoint;			/// 평면 점
		float						m_fPlaneZAngle;			/// 평면 Z축 각도
		float						m_fLastPlaneZAngle;		/// 평면 최근 Z축 각도

		D3DXPLANE					m_PlaneCenter;			/// 중앙 평면
		D3DXPLANE					m_PlaneUpper;			/// 윗 평면
		D3DXPLANE					m_PlaneLower;			/// 아래 평면
		D3DXVECTOR3					m_vPlaneNormalCenter;	/// 중앙 평면 법선
		D3DXVECTOR3					m_vPlaneNormalUpper;	/// 윗 평면 법선
		D3DXVECTOR3					m_vPlaneNormalLower;	/// 아래 평면 법선

		D3DXVECTOR3					m_vPlaneRotateDegreeCenter;		/// 중앙 평면 회전 각도
		D3DXVECTOR3					m_vPlaneRotateDegreeUpper;		/// 윗 평면 회전 각도 
		D3DXVECTOR3					m_vPlaneRotateDegreeLower;		/// 아래 평면 회전 각도

//////////// GIGA STREAM ////////////////////////////////////////////////////////{{

		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hEffectGigaPlane;			/// 기가 평면 메쉬
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hEffectGigaCrystal[5];	/// 기가 크리스탈 메쉬

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hParticleGigaPlane;		/// 기가 평면 파티클
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hParticleGigaCrystal[5];	/// 기가 크리스탈 파티클

		bool						m_bGigaPlaneEnable;		/// 기가 평면 활성화 여부
		D3DXPLANE					m_GigaStreamPlane;		///	기가 평면
		D3DXVECTOR3					m_vGigaPlaneNormal;		/// 기가 평면 법선
		D3DXVECTOR3					m_vGigaPlanePoint;		/// 기가 평면 점
		float						m_fGigaPlaneZAngle;		/// 기가 평면 Z축 각도

//////////// GIGA STREAM ////////////////////////////////////////////////////////}}

		int							m_iPriorEffectID;		/// 우선하는 이펙트 ID
		int							m_iPosteriorEffectID;	/// 뒷부분 이펙트 ID

		CX2GUEve*					m_pGUEve;				/// 이브 포인트
		//CX2DamageEffect::CEffect*	m_pEffectPlane;			/// 이펙트 평면

		std::list<EveElectraLaserData>		m_EveElectraLaserDataList;		/// 레이저 데이터 리스트
		std::list<EveElectraEffectData>		m_EveElectraEffectDataList;		/// 이펙트 데이터 리스트
		std::list<EveElectraLockonData>		m_EveElectraLockonDataList;		/// 조준 데이터 리스트

#ifdef SERV_EVE_BATTLE_SERAPH
		bool	m_bHyperOpticalResearch;		/// 초광학 연구
		float	m_fHyperOpticalResearchScale;	/// 초광학 연구 증폭장 추가 증가
		
		bool	m_bExceptionSystem;						/// 에너제틱 하트 발동
		float	m_fExceptionSystemOperationTime;		/// 에너제틱 하트 가동 시간
		float	m_fExceptionSystemRemainTime;			/// 에너제틱 하트 유지 시간
		float	m_fExceptionFormationSetElapsedTime;	/// 에너제틱 하트 유형 경과 시간 설정
		PlaneFormationType m_ePrevSpectro;				/// 융합장 전의 역장 값을 저장

		float	m_fEveZAngle;		/// Dash Jump X(Push)X 레이저를 위한 이브의 초기각 값
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		int			m_iLaserGroupNum;		/// 리니어 디바이더 레이저 그룹 구분자
#endif //UPGRADE_SKILL_SYSTEM_2013

		DelegateProcess				m_delegateChangePlaneFormation;		/// 일렉트라, 배틀 세라트의 ChangePlaneFormation 의 Delegate

		EveElectraSystem( CX2GUEve* _pGUEve );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void ProcessSystem( float fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void ProcessSystem();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void ProcessPlane( float _fElapsedTime );
		void ProcessGigaPlane( float _fElapsedTime );
		void ProcessFormation( float _fElapsedTime );

		void StartFormationEffect();

#ifdef SERV_EVE_BATTLE_SERAPH
		void AddLaserToSystem( CX2DamageEffect::CEffect* _pDamageEffect, wstring _TraceBone, D3DXVECTOR3& _vLaserStartPos, D3DXVECTOR3&_vDirVector,
							   float _fVelocity, float _fLaserPropagateTime = 0.3f, float _fLaserWidthScale = 1.f, 
							   wstring AmplifyDamageEffectName = L"", wstring SpectrumDamageEffectName = L"", wstring InductionDamageEffectName = L"",
							   bool _bApplyLaserRangePassive = true,  float _fRadiusScale = 1.f );

		void AddEffectToSystem( CX2DamageEffect::CEffect* _pDamageEffect, wstring AmplifyDamageEffectName = L"",
								wstring SpectrumDamageEffectName = L"", wstring InductionDamageEffectName = L"", wstring FusionDamageEffectName = L"" );

		void AddLockonToSystem( CX2DamageEffect::CEffect* _pDamageEffect, wstring AmplifyDamageEffectName = L"",
								wstring SpectrumDamageEffectName = L"", wstring InductionDamageEffectName = L"" );

#else
		void AddLaserToSystem( CX2DamageEffect::CEffect* _pDamageEffect, wstring _TraceBone, D3DXVECTOR3& _vLaserStartPos, D3DXVECTOR3&_vDirVector,
							   float _fVelocity, float _fLaserPropagateTime = 0.3f, float _fLaserWidthScale = 1.f, 
							   wstring AmplifyDamageEffectName = L"", wstring SpectrumDamageEffectName = L"", 
							   bool _bApplyLaserRangePassive = true,  float _fRadiusScale = 1.f );

		void AddEffectToSystem( CX2DamageEffect::CEffect* _pDamageEffect, wstring AmplifyDamageEffectName = L"", wstring SpectrumDamageEffectName = L""  );
		void AddLockonToSystem( CX2DamageEffect::CEffect* _pDamageEffect, wstring AmplifyDamageEffectName = L"", wstring SpectrumDamageEffectName = L""  );
#endif
		void DeleteLaserFromSystem();

		void LaserAttackLineSet( CX2DamageEffect::CEffect* _pDamageEffect, float _fRadiusScale = 1.0f );


		void CreatePlane( float _fZAngle = 0.f, float _LifeTime = 2.f, float _fDistance = 300.f, float _fHeight = 130.f, wstring PlaneMeshName = L"" );
		void CreateGigaPlane( float _fZAngle = 0.f, float _LifeTime = 5.f, float _fDistance = 500.f, float _fHeight = 130.f, float _fScale = 1.0f );

		void SetPlaneFormation( PlaneFormationType _ePlaneFormationType );
		void ChangePlaneFormation();

		PlaneFormationType GetPlaneFormation(){ return m_ePlaneFormationType; }
		BUFF_TEMPLET_ID GetBuffTempletIdByPlaneFormation( const PlaneFormationType ePlaneFormationType_ ) const;

		bool GetEnableSystem(){ return m_bSystemEnable; }
		void SetEnableSystem( bool _bVal ){ m_bSystemEnable = _bVal; }

		bool GetEnablePlane(){ return m_bPlaneEnable; }
		void SetEnablePlane(bool val) { m_bPlaneEnable = val; }

		bool GetEnableGigaPlane(){ return m_bGigaPlaneEnable; }
		void SetEnableGigaPlane(bool val) { m_bGigaPlaneEnable = val; }

		bool GetEnableSpectrumDynamics(){ return m_bSpectrumDynamicsEnable; }
		void SetEnableSpectrumDynamics( bool _bVal ){ m_bSpectrumDynamicsEnable = _bVal; }

		float GetSystemOperationTime() const { return m_fSystemOperationTime; }
		void SetSystemOperationTime(float val) { m_fSystemOperationTime = val; }

		void DestroyFormationMarkAndMsgParticle();
		void DestroyPlaneAndCrystalEffect();
		void DestroyGigaPlaneAndCrystalEffect();
		
#ifdef SERV_EVE_BATTLE_SERAPH		/// 초광학 연구
		bool GetEnableHyperOpticalResearch(){ return m_bHyperOpticalResearch; }
		void SetEnableHyperOpticalResearch( bool _bVal ){ m_bHyperOpticalResearch = _bVal; }

		void SetExceptionSystemTime( float _fVal );
		void SetEnableExceptionSystem( bool _bVal ){ m_bExceptionSystem = _bVal; }
		bool GetEnableExceptionSystem() { return m_bExceptionSystem; }
#endif
		
	};
	
	struct FlyingImpactData
	{
		D3DXVECTOR3					m_vOffsetPos;
		D3DXVECTOR3					m_vOffsetRotate;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle       m_hEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*	m_pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		FlyingImpactData();
		~FlyingImpactData();

//{{ robobeg : 2013-08-30
        CX2DamageEffect::CEffect*   GetLiveDamageEffect();
//}} robobeg : 2013-08-30
	};


#endif EVE_ELECTRA



#ifdef EVE_FIRST_CHANGE_JOB
	struct EG_QueensThroneData
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		double	m_dMaxLifeTime;		/// 최대 실행시간.
#else //ADD_MEMO_1ST_CLASS
		const double	m_dMaxLifeTime;		/// 최대 실행시간.
#endif //ADD_MEMO_1ST_CLASS
		

		CKTDXTimer		m_TimerElapsedTime;	/// 진행시간.
		bool			m_bEnable;			/// 사용여부.
		float			m_fPowerRate;		/// 증가 비율

		// 등에 붙어있는 메쉬
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_ahMajorMeshInstance[4];

		enum PEH_POS
		{
			PP_WING,
			PP_BODY,
			PP_END,
		};
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahMajorParticleInstance[PP_END];

		std::wstring	m_wstrBoneName[5];
		int				m_iSelectBoneNameID;

		EG_QueensThroneData()
		:
		m_dMaxLifeTime(30.0),
		m_bEnable(false),
		m_iSelectBoneNameID(0),
		m_fPowerRate(0.f)
		{
			for( int i = 0; i < ARRAY_SIZE(m_ahMajorMeshInstance); ++i )
			{
				m_ahMajorMeshInstance[i] = INVALID_MESH_INSTANCE_HANDLE;
			}
			for( int i = 0; i < ARRAY_SIZE(m_ahMajorParticleInstance); ++i )
			{
				m_ahMajorParticleInstance[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
			m_wstrBoneName[0] = L"Bip01";
			m_wstrBoneName[1] = L"Bip01_L_UpperArm";
			m_wstrBoneName[2] = L"Bip01_R_UpperArm";
			m_wstrBoneName[3] = L"Bip01_L_Foot";
			m_wstrBoneName[4] = L"Bip01_R_Foot";
		}

		void Reset();

		int	 GetBoneNameID()
		{
			if( m_iSelectBoneNameID > 4 || m_iSelectBoneNameID < 0 )
				m_iSelectBoneNameID = 0;

			return m_iSelectBoneNameID;
		}
	};
#endif EVE_FIRST_CHANGE_JOB

#ifdef SKILL_30_TEST
	struct ATOveronGuardData
	{
		CKTDXTimer		m_TimerElapsedTime;	/// 진행시간.
		bool			m_bEnable;			/// 사용여부.

		double			m_dEffectiveTime;	/// 유지시간.
		float			m_fProjectileSize;	/// 발사체 크기.

		ATOveronGuardData(){ Reset(); }
		void	Reset(){ m_bEnable = false; m_dEffectiveTime = 0.f; m_fProjectileSize = 1.f; }

		void	SetEnable( bool bEnable ){ m_bEnable = bEnable; }
		bool	GetEnable(){ return m_bEnable; }
		bool	IsLive(){ return ((GetEffectiveTime() - m_TimerElapsedTime.elapsed() > 0.0) ? true : false); }
		void	SetEffectiveTime( float fEffectiveTime ){ m_dEffectiveTime = static_cast<double>(fEffectiveTime); }
		double	GetEffectiveTime(){ return m_dEffectiveTime; }
		void	SetProjectileSize( float fProjectileSize ){ m_fProjectileSize = fProjectileSize; }
		float	GetProjectileSize(){ return m_fProjectileSize; }
	};
#endif SKILL_30_TEST


	// 메가일랙트론볼 정보
	//#1. InitEquippedActiveSkillState() 슬롯 정보 셋팅.
	//#2. CommonEventProcess() 입력체크.
	//#3. CommonFrameMove() 차지 이펙트 처리 및 [**] 슬롯이 바뀐경우 실패처리 [**].
	//#4. SpecialAttackEventProcess() 발사처리.
	enum CHARGE_RANK
	{
		CR_RANK_1 = 1,
		CR_RANK_2,
		CR_RANK_3,
	};
	struct MEGA_ELECTRONBALL_Data
	{
		double			m_dMaxTime;
		CKTDXTimer		m_timerElapsedTime;

		bool			m_bEnable;			/// 차지 중인지.
		bool			m_bIsSucCharge;		/// 차지가 완료되었는지.
		CHARGE_RANK		m_ChargeRank;		/// 마지막에 사용되었던 랭크를 그대로 기억한다.
		bool			m_bSkillSlotSwap;	/// 슬롯 체인지 여부 기록


		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahMajorParticleInstance[4];

		MEGA_ELECTRONBALL_Data()
		:
		m_dMaxTime(0.),
		m_bEnable(false),
		m_iSkillSlotID(-1),
		m_ChargeRank(CR_RANK_1),
		m_bIsSucCharge(false),
		m_bSkillSlotSwap( false )
		{
			for( int i = 0; i < ARRAY_SIZE(m_ahMajorParticleInstance); ++i )
			{
				m_ahMajorParticleInstance[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}

		void	Reset( bool bIsChargeTrue = true, FrameData* pFrameDataFuture = NULL ); // 리셋타이밍이 차지 실패일경우. pvp 싱크를 날리기위해.
		void	ResetParticle();
		void	SetSlotID( const int iSlotID )// 스킬슬롯 B일경우 4~7값을 가짐.
		{ 
			m_iSkillSlotID = iSlotID;
		}
		int		GetSkillSlotID()
		{
			return m_iSkillSlotID;
		}
		void	SetSkillSlotSwap( bool bSkillSlotSwap ){ m_bSkillSlotSwap = bSkillSlotSwap; }
		bool	GetSkillSlotSwap(){ return m_bSkillSlotSwap; }

	private:
		int				m_iSkillSlotID;		// -1 이 아니면 해당슬롯에 등록되어있다는 의미.
	};

#ifdef SERV_EVE_BATTLE_SERAPH
#ifdef FIX_EVE_TASER_PILUM
	// 테이저 필라의 데이터가 발사 도중 변경되기 때문에 발사 중 쓰이는 데이터 추가
	struct TASER_PILUM_TinyData
	{
		CKTDXTimer		m_timerElapsedTime;
		CHARGE_RANK		m_ChargeRank;		
		int				m_iAddDamageTime;

		TASER_PILUM_TinyData():
		m_ChargeRank(CR_RANK_1),
		m_iAddDamageTime(0)
		{}

		void reset()
		{
			m_ChargeRank = CR_RANK_1;
			m_iAddDamageTime = 0;
			m_timerElapsedTime.ResetStartTime();
		}
	};
#endif // FIX_EVE_TASER_PILUM


	struct TASER_PILUM_Data
	{
		enum SLOT_TYPE
		{
			ST_A = 0,
			ST_B,

			ST_END,
		};

		double			m_dMaxTime;
		CKTDXTimer		m_timerElapsedTime;

		bool			m_bEnable;			/// 차지 중인지.
		bool			m_bIsSucCharge;		/// 차지가 완료되었는지.
		CHARGE_RANK		m_ChargeRank;		/// 마지막에 사용되었던 랭크를 그대로 기억한다.
		bool			m_bSkillSlotSwap;	/// 슬롯 체인지 여부 기록

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahMajorParticleInstance[4];

		TASER_PILUM_Data()
			:
		m_dMaxTime(0.),
			m_bEnable(false),
			m_bSelectedSlot(true),
			m_ChargeRank(CR_RANK_1),
			m_bIsSucCharge(false),
			m_bSkillSlotSwap(false)
		{
			m_iSkillSlotID[ST_A] = -1;
			m_iSkillSlotID[ST_B] = -1;

			for( int i = 0; i < ARRAY_SIZE(m_ahMajorParticleInstance); ++i )
			{
				m_ahMajorParticleInstance[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}

		void	Reset( bool bIsChargeTrue = true, FrameData* pFrameDataFuture = NULL ); // 리셋타이밍이 차지 실패일경우. pvp 싱크를 날리기위해.
		void	ResetParticle();

		void	SetSlotID( SLOT_TYPE eSlotType, const int iSlotID )//const int iSlotID, const bool bSlotB )
		{
			m_iSkillSlotID[eSlotType] = iSlotID;
			//m_bSkillSlotB	= bSlotB;
		}
		int		GetSkillSlotID()
		{
			if( m_bSelectedSlot == true )
				return m_iSkillSlotID[ST_A];
			else
				return m_iSkillSlotID[ST_B];
		}
		//bool	GetSkillSlotB(){return m_bSkillSlotB;}
		void	SetSelectedSlot( bool bSelectedSlot ){ m_bSelectedSlot = bSelectedSlot; }
		bool	GetSelectedSlot(){ return m_bSelectedSlot; }
		void	SetSkillSlotSwap( bool bSkillSlotSwap ){ m_bSkillSlotSwap = bSkillSlotSwap; }
		bool	GetSkillSlotSwap(){ return m_bSkillSlotSwap; }

	private:
		int				m_iSkillSlotID[ST_END];	// -1 이 아니면 해당슬롯에 등록되어있다는 의미.
		bool			m_bSelectedSlot;	// A : true | B : false
	};

	enum BOOST_DASH_STATE
	{
		BDS_NONE	= 0,	/// 부스트 대쉬가 아님
		BDS_RIGHT	= 1,	/// 부스트 대쉬 오른쪽
		BDS_LEFT	= 2,	/// 부스트 대쉬 왼쪽
	};

	struct ENERGY_NEEDLES_Data
	{
		bool	m_bEnable;			/// 에너지 니들스 발사
		float	m_fMaxTime;			/// 에너지 니들스 발사 유지 시간, SA_EFFECTIVE_TIME 값을 저장
		float	m_fIntervalTime;	/// 에너지 니들스 발사 간격( 0.33마다 한 발 )
		float	m_fPowerRate;		/// 에너지 니들스 PowerRate 수치
		
		D3DXVECTOR3 m_vEffectPos[6];

		ENERGY_NEEDLES_Data( )
			: m_bEnable( false )
			, m_fMaxTime( 0.f )
			, m_fIntervalTime( 0.33f )
			, m_fPowerRate( 1.f )
		{
			m_vEffectPos[0] = D3DXVECTOR3( 120,	-40,  20 );
			m_vEffectPos[1] = D3DXVECTOR3( 135,	-20, -20 );
			m_vEffectPos[2] = D3DXVECTOR3( 105,	 60,   0 );
			m_vEffectPos[3] = D3DXVECTOR3( 140,  75,   0 );
			m_vEffectPos[4] = D3DXVECTOR3( 110,	-60,   0 );
			m_vEffectPos[5] = D3DXVECTOR3( 100,	-75,  40 );
		}

		void OnFrameMove( IN CX2GUEve* pGUEve, IN float fElapsedTime_ );
	};
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	struct SKugelBlitz_Data
	{
		enum SLOT_TYPE
		{
			ST_A = 0,
			ST_B,

			ST_END,
		};

		double			m_dMaxTime;
		CKTDXTimer		m_timerElapsedTime;

		bool			m_bEnable;			/// 차지 중인지.
		bool			m_bIsSucCharge;		/// 차지가 완료되었는지.
		CHARGE_RANK		m_ChargeRank;		/// 마지막에 사용되었던 랭크를 그대로 기억한다.
		bool			m_bSkillSlotSwap;	/// 슬롯 체인지 여부 기록

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahMajorParticleInstance[4];

		SKugelBlitz_Data()
			: m_dMaxTime(0.)
			, m_bEnable(false)
			, m_bSelectedSlot(true)
			, m_ChargeRank(CR_RANK_1)
			, m_bIsSucCharge(false)
			, m_bSkillSlotSwap(false)
		{
			m_iSkillSlotID[ST_A] = -1;
			m_iSkillSlotID[ST_B] = -1;

			for( int i = 0; i < ARRAY_SIZE(m_ahMajorParticleInstance); ++i )
			{
				m_ahMajorParticleInstance[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}

		void	Reset( bool bIsChargeTrue = true, FrameData* pFrameDataFuture = NULL ); // 리셋타이밍이 차지 실패일경우. pvp 싱크를 날리기위해.
		void	ResetParticle();

		void	SetSlotID( SLOT_TYPE eSlotType, const int iSlotID )//const int iSlotID, const bool bSlotB )
		{
			m_iSkillSlotID[eSlotType] = iSlotID;
			//m_bSkillSlotB	= bSlotB;
		}
		int		GetSkillSlotID()
		{
			if( m_bSelectedSlot == true )
				return m_iSkillSlotID[ST_A];
			else
				return m_iSkillSlotID[ST_B];
		}

		void	SetSelectedSlot( bool bSelectedSlot ){ m_bSelectedSlot = bSelectedSlot; }
		bool	GetSelectedSlot(){ return m_bSelectedSlot; }
		void	SetSkillSlotSwap( bool bSkillSlotSwap ){ m_bSkillSlotSwap = bSkillSlotSwap; }
		bool	GetSkillSlotSwap(){ return m_bSkillSlotSwap; }

	private:
		int				m_iSkillSlotID[ST_END];	// -1 이 아니면 해당슬롯에 등록되어있다는 의미.
		bool			m_bSelectedSlot;	// A : true | B : false
	};

	struct SKugelBlitz_TinyData
	{
		CKTDXTimer		m_timerElapsedTime;
		CHARGE_RANK		m_ChargeRank;		
		int				m_iAddDamageTime;

		SKugelBlitz_TinyData()
			: m_ChargeRank(CR_RANK_1), m_iAddDamageTime(0)
		{}

		void reset()
		{
			m_ChargeRank = CR_RANK_1;
			m_iAddDamageTime = 0;
			m_timerElapsedTime.ResetStartTime();
		}
	};
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
	enum MANEUVER_CORE_STATE
	{	// 기동 코어 상태
		MCS_NONE = 0,	
		MCS_WAIT,			// 대기		
		MCS_BEFORE_ATTACK,	// 공격 상태, 공격 전
		MCS_ATTACK,			// 공격 상태, 공격 중
		MCS_AFTER_ATTACK,	// 공격 상태, 공격 후
		MCS_PARRYING,		// 방어 상태
	};
	
	enum MANEUVER_CORE_ATTACK_TYPE
	{	// 기동 코어 공격 타입
		MCAT_NONE = 0,
		MCAT_USE_MANEUVER_GAGE,					// 기동 게이지를 소모하는 공격
	};
	
	enum USING_MANEUVER_GAGE_ATTACK_TYPE
	{
		// 기동 게이지를 소모하는 공격의 타입
		UMGAT_NORMAL = 0,
		UMGAT_HYPER = 1,
	};

	struct ManeuverCoreData	
	{
		D3DXVECTOR3						m_vOffsetPos;
		D3DXVECTOR3						m_vOffsetRotate;
		CX2DamageEffect::CEffectHandle  m_hEffect;
		MANEUVER_CORE_STATE		m_eManeuverCoreState;
		ManeuverCoreData();
		~ManeuverCoreData();
		void ChangeManeuverCoreState ( MANEUVER_CORE_STATE eManeuverCoreAttackState_ ) 
		{ 
			m_eManeuverCoreState = eManeuverCoreAttackState_;
		}

		D3DXVECTOR3 GetManeuverCorePosition ();
		D3DXVECTOR3 GetManeuverCoreRotateExceptOffset ();
	};
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어



	private:
		CX2GUEve( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit );
		virtual ~CX2GUEve(void);

		bool IsUpSkillSlotMegaElectronBall(); // 메가일랙트론볼 차지중인 키가 업된는지 체크..
		const CX2UserSkillTree::SkillSlotData* GetSkillSlotDataMegaElectronBall( IN const CX2UserSkillTree& cUserSkillTree_, OUT int& iSkillSlotIndexPressed_ );

#ifdef SERV_EVE_BATTLE_SERAPH
		bool IsUpSkillSlotTaserPilum();
		const CX2UserSkillTree::SkillSlotData* GetSkillSlotDataTaserPilum( IN const CX2UserSkillTree& cUserSkillTree_, OUT int& iSkillSlotIndexPressed_ );
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		bool IsUpSkillSlotKugelBlitz();
		const CX2UserSkillTree::SkillSlotData* GetSkillSlotDataKugelBlitz( IN const CX2UserSkillTree& cUserSkillTree_, OUT int& iSkillSlotIndexPressed_ );
		int	GetKugelBlitzAddDamageTime() const {return m_sKugelBlitz_TinyData.m_iAddDamageTime;}
		void ResetKugelBlitzTinyData() { m_sKugelBlitz_TinyData.reset(); }
#endif //UPGRADE_SKILL_SYSTEM_2013

//{{ kimhc // 2010.11.2 // 엠프레스 - 충전 추진기
#ifdef	NEW_SKILL_2010_11
		bool CanUseChargingBooster( CX2SkillTree::SKILL_TYPE eSkillType_, const IN CX2UserSkillTree& cUserSkillTree_ );
		void UseChargingBooster( CX2UserSkillTree& cUserSkillTree_ );
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.2 // 엠프레스 - 충전 추진기



	public:
		static CX2GUEve* CreateGUEve( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit )
		{
			return new CX2GUEve( unitIndex, teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				frameBufferNum, pUnit );
		};


#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	static void			AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


	virtual void		ReInit( bool bRandom = false, int startPosIndex = -1 );

	virtual void		DamageReact( CX2DamageManager::DamageData* pDamageData );
	virtual void		AttackResultPlayEffect();

	virtual void		Win();
	virtual void		Lose();
	virtual void		Draw();

	virtual void		InitEffect();
	virtual void		InitEnchantWeaponEffectUnitSpecific();


	void				InitializeEveMajorParticleArray();	// 메이저 파티클 배열을 초기화 // kimhc // 2010.11.5 
	virtual void		CreateMajorParticleArray();		// 메이저 파티클 배열을 생성 // kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	static void			AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CKTDGParticleSystem::CParticleEventSequence* SetEveMajorParticleByEnum( EVE_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
	
	ParticleEventSequenceHandle	GetHandleEveMajorParticleByEnum( EVE_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const // 캐릭터만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
	{
		ASSERT( EVE_MAJOR_PII_END > eVal_ && EVE_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		return m_ahEveMajorParticleInstance[eVal_];
	}

	ParticleEventSequenceHandle& GetHandleEveMajorParticleByEnum( EVE_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) // 캐릭터만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
	{
		ASSERT( EVE_MAJOR_PII_END > eVal_ && EVE_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		return m_ahEveMajorParticleInstance[eVal_];
	}

	void				SetHandleEveMajorParticleByEnum( EVE_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ ) // 캐릭터만 쓰는 메이저 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함 // kimhc // 2010.11.5 
	{
		ASSERT( EVE_MAJOR_PII_END > eVal_ && EVE_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		m_ahEveMajorParticleInstance[eVal_] = hHandle_;
	}
	void				DeleteEveMajorParticle();


	void				InitializeEveMinorParticleArray();	// 공통적으로 쓰는 마이너 파티클 배열을 초기화 // kimhc // 2010.11.5 
	virtual void		CreateMinorParticleArray();		// 각 캐릭터들만 쓰는 마이너 파티클 배열을 초기화 // kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	static void			AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CKTDGParticleSystem::CParticleEventSequence* SetEveMinorParticleByEnum( EVE_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
	
	ParticleEventSequenceHandle	GetHandleEveMinorParticleByEnum( EVE_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const	// 캐릭터만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴	// kimhc // 2010.11.5 
	{
		ASSERT( EVE_MINOR_PII_END > eVal_ && EVE_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		return m_ahEveMinorParticleInstance[eVal_];
	}

	ParticleEventSequenceHandle& GetHandleEveMinorParticleByEnum( EVE_MINOR_PARTICLE_INSTANCE_ID eVal_ ) // 캐릭터만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴	// kimhc // 2010.11.5 
	{
		ASSERT( EVE_MINOR_PII_END > eVal_ && EVE_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		return m_ahEveMinorParticleInstance[eVal_];
	}

	void				SetHandleEveMinorParticleByEnum( EVE_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )	// 캐릭터만 쓰는 마이너 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함	// kimhc // 2010.11.5 
	{
		ASSERT( EVE_MINOR_PII_END > eVal_ && EVE_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		m_ahEveMinorParticleInstance[eVal_] = hHandle_;
	}
	void				DeleteEveMinorParticle();
	virtual	void		DeleteMinorParticle();

	void 				InitMajorParticle();
	void 				InitMinorParticle();

	void				CallSummonedNPC( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
	void				ResetMegaEletronCharge();
	
	virtual bool		SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet );
	virtual float		GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;

	virtual	float		GetAdditionalAccelBuffTime( const CX2SkillTree::SKILL_ID eSkillID_ ) const;
	virtual void		ApplyAuraAcceleratorBuffToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const float fAddtionalAccelBuffTime_, const D3DXVECTOR3& vMyPos_ );
	CX2BuffFactorPtr	GetEnergyOfConcentrationBuffFactorClonePtr() const;

#ifdef FIX_SPECTRO_EL_CRYSTAL_ACTIVE_BUG	/// 분광결정이 스킬 사용을 통해 시작 되었는지 여부
	bool				GetStartSpectroElCrystal() { return m_bStartSpectroElCrystal; }
	void				SetStartSpectroElCrystal( bool bStartSpectroElCrystal_ ) { m_bStartSpectroElCrystal = bStartSpectroElCrystal_; }
#endif FIX_SPECTRO_EL_CRYSTAL_ACTIVE_BUG

#ifdef EVE_ELECTRA
	bool				GetEnableElectraSystem(){ return m_EveElectraSystem.GetEnableSystem(); }
	PlaneFormationType	GetElectraFormationType(){ return m_EveElectraSystem.GetPlaneFormation(); }
	void				ClearThousandStar();
	void				CreateThousandStar();
	void				DoFrameMoveThousandStar();
	void				EntireAttackThousandStar();
	void				AttackThousandStar();
	int					GetThousandStarCount(){ return m_iThousandStarCount; }
	bool				GetThousandStarEntireFired(){ return m_bEntireFireMode; }

	virtual void		AttackResultByType( CX2DamageManager::DamageData &pDamageData );
#endif EVE_ELECTRA

#ifdef SERV_EVE_BATTLE_SERAPH
	void				ResetTaserPilumCharge();
#endif
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	void				ResetKugelBlitzCharge();
#endif //UPGRADE_SKILL_SYSTEM_2013
#ifdef FINALITY_SKILL_SYSTEM //김창한
	//엠프레스 궁극기 - 링크 오버 차지 썬더 볼트가 동작했는지 체크
	const bool GetActiveLinkOverChargeIllusion() const { return m_bStartLinkOverChargeIllusion; }
	void SetActiveLinkOverChargeIllusion( const bool bActive ) { m_bStartLinkOverChargeIllusion = bActive; }

	void SaveFerdinandNPCUID( const int npcUid ) { m_iFerdinandNPCUID = npcUid; }
	void ResetLinkOverChargeIllusion();
#endif //FINALITY_SKILL_SYSTEM

	virtual void HyperModeBuffEffectStart();
	virtual void AddUnitSlashData( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ );
	
	/// 각 유닛마다 특정 버프가 실행 또는 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 등...)
	virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );

	virtual void ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ );
	virtual void ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ );

	void		ChangePlaneForamtionSystem( const PlaneFormationType ePlaneFormatinoSystem_ );

	virtual int GetComboZStateID() const { return ENSI_COMBO_Z; }
	virtual int GetComboXStateID() const { return ENSI_COMBO_X; }

#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
	void	OnFrameMove_ManeuverCore ();

	void	ClearManeuverCore ( bool bClearManeuverGauge = false );
	void	CreateManeuverCore ( bool bShowCreateEffect = true );
	void	RestoreManeuverCore ();
	void	AttackManeuverCore();
	void	ChangePowerRateManeuverCore( CX2DamageEffect::CEffect* pEffect );

	float			GetManeuverGauge() { return m_fManeuverGauge; }	
	virtual void	AddManeuverGauge(float fVal);

	bool	GetCanAttackManeuverCore ( CX2GameUnit * pTargetUnit ) 
	{ 
		return ( NULL != pTargetUnit && 0 < pTargetUnit->GetNowHp() && 0 < GetManeuverCoreLevelByGauge() && true == IsWaitManeuverCore()  );		
	}
	int		GetManeuverCoreLevel() { return m_iManeuverCoreLv; }
	int		GetManeuverCoreLevelByGauge() { return static_cast<int> (m_fManeuverGauge / 30.f); }

	void	MoveManeuverCoreByState ( MANEUVER_CORE_STATE eState );
	void	RotateManeuverCoreByState ( MANEUVER_CORE_STATE eState );
	void	FindAndSetManeuverCoreTargetGameUnit ( UidType iGameUnitUID_, bool bIsNPC_ );
	CX2GameUnitoPtr	GetManeuverCoreTargetGameUnit ();
	void			SetManeuverCoreTargetGameUnit ( CX2GameUnitoPtr pTargetUnit_ );
	void			SetManeuverCoreRotateToTargetPos ( D3DXVECTOR3 vTargetPos_, D3DXVECTOR3 vOffsetRot_ );

#ifdef FIX_ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템 수정 사항
	D3DXVECTOR3			GetManeuverCoreTargetPosUsingCollisionData ();
	wstring				GetBoneNameNearestImpactPoint ( const D3DXVECTOR3 & vImpactPoint );
	virtual	const int	GetHyperModeStateID ();
#endif // FIX_ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템 수정 사항
	


	bool	CanParrying() { return m_bCanParrying; }		
	bool	CanParryingState();
	void	DoParrying();
	
	virtual void	HyperModeBuffEffectEnd() 
	{
		CX2GUUser::HyperModeBuffEffectEnd(); 
		
		SetFullHyperMode( false ); 

		/// 남은 RageGage가 있으면 HP와 MP를 증가시킨다
		if( IsMyUnit() == true && GetNowHp() > 0.f && m_fManeuverGauge > 0.f )
		{			
			UpNowHp( GetMaxHp() * (m_fManeuverGauge * 0.0005f) );
			UpNowMp( m_fManeuverGauge );
		}
		ClearManeuverCore( true );
	}
	virtual bool	IsFullHyperMode() const { return m_bHyperState; }
	virtual void	SetFullHyperMode( const bool bFullHyperMode_ ) { m_bHyperState = bFullHyperMode_; }

	void	SetManeuverCoreEffectSetPosition ();
	void	ClearManeuverCoreEffect ();
	void	ChangeManeuverCoreState ( MANEUVER_CORE_STATE eState_ );

	virtual void	SetAttackManeuverCore ( CX2DamageManager::DamageData & pDamageData );

	virtual	bool	IsWaitManeuverCore ()
	{
		if ( NULL != m_pManeuverCore )
		{
			switch ( m_pManeuverCore->m_eManeuverCoreState )
			{
			case MCS_WAIT :
				return true;
			}
		}		
		return false;
	}

	virtual bool	IsAttackManeuverCore() 
	{
		if ( NULL != m_pManeuverCore )
		{
			switch ( m_pManeuverCore->m_eManeuverCoreState )
			{
				case MCS_BEFORE_ATTACK :
				case MCS_ATTACK :
				case MCS_AFTER_ATTACK :
					return true;
			}
		}		
		return false;
	}
	
	virtual bool	IsSkillSummonedMonster ( CX2UnitManager::NPC_UNIT_ID eUnitID ) 
	{ 
		switch ( eUnitID )
		{
			case CX2UnitManager::NUI_NASOD_WATCH_EVE :
			case CX2UnitManager::NUI_ATOMIC_SHIELD_EVE :
			case CX2UnitManager::NUI_SI_HA_FERDINAND :
			case CX2UnitManager::NUI_WALLY_9TH_EVE :
			case CX2UnitManager::NUI_NASOD_ELEMENT_FIRE_EVE :
			case CX2UnitManager::NUI_GUARDIAN_SP1_EVE :
			case CX2UnitManager::NUI_GUARDIAN_SP1_EVE_MEMO :
			case CX2UnitManager::NUI_GUARDIAN_SP2_EVE :
			case CX2UnitManager::NUI_GUARDIAN_SP2_EVE_MEMO :
				return true;

			default :
				return false;
		}
		return false; 
	}

	bool	GetParrying() 
	{
		return ( NULL != m_pManeuverCore && m_pManeuverCore->m_eManeuverCoreState == MCS_PARRYING );
	}

	void SetManeuverCoreTargetUnit ( CX2GameUnit::GAME_UNIT_TYPE eGameUnitType_, UidType uidType_, MANEUVER_CORE_ATTACK_TYPE eManeuverCoreAttackType_ );
	
	CKTDGXMeshPlayer::CXMeshInstance*	GetManeuverCoreMeshInstance ();
	CX2DamageEffect::CEffectHandle		GetManeuverCoreEffectHandle();
	CX2DamageEffect::CEffect *			GetManeuverCoreEffect();


	void ENSI_PARRYING_Start ();
	void ENSI_PARRYING_StartFuture ();
	void ENSI_PARRYING_EventProcess ();
	void ENSI_PARRYING_StateEnd ();

	void ENSI_PARRYING_BLINK_Init ();
	void ENSI_PARRYING_BLINK_Start ();
	void ENSI_PARRYING_BLINK_StartFuture ();
	void ENSI_PARRYING_BLINK_FrameMove ();
	void ENSI_PARRYING_BLINK_FrameMoveFuture ();
	void ENSI_PARRYING_BLINK_EventProcess ();
	void ENSI_PARRYING_BLINK_End ();
	void ENSI_PARRYING_BLINK_EndFuture ();

#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어


protected:
	virtual void ParseCommonRandomState();
	void				InitState();
	virtual void		InitComponent();	
	void	InitStateID();
	void	InitStateCommon();



	virtual void		InitEquippedActiveSkillState( bool bOnlySkillLevel = false );

	virtual void		InitPassiveSkillState();
	virtual void		SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ );
	virtual void		SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ );



	virtual void		CommonStateStartFuture();
	virtual void		CommonStateStart();
	virtual void		CommonFrameMoveFuture();
	virtual void		CommonFrameMove();
	virtual void		CommonEventProcess();
	virtual void		CommonStateEndFuture();
	virtual void		CommonStateEnd();
	//{{ robobeg : 2008-10-28
	//virtual void		CommonRender();
	virtual RENDER_HINT CommonRender_Prepare();
	//}} robobeg : 2008-10-28

	virtual bool		SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );
	virtual void		NoStateChangeActionFrameMove();
	
#ifndef SERV_9TH_NEW_CHARACTER // 김태환
	/// 다른 캐릭터들 전부 똑같은 함수를 쓰고 있으니, X2GUUser로 옮기자.
	virtual void		CreateNotEnoughMPEffect( D3DXVECTOR3 vPos, float fDegreeX, float fDegreeY, float fDegreeZ );
#endif // SERV_9TH_NEW_CHARACTER

	virtual	void		CreateStepDust();

	void				CancelComboWhenChangedDirection( int iFlagCount );
	float				CalcInvisibilityAlphaValue();
	void				SetSelfDestructSummonedNPC( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );

	//{{ kimhc // 2010.2.10 //	이브 스피어 버스트가 시전 됬는지 식별 값
#ifdef	EVE_SECOND_CLASS_CHANGE
	void				CreateSpearBurst();
#endif	EVE_SECOND_CLASS_CHANGE
	//}} kimhc // 2010.2.10 //	이브 스피어 버스트가 시전 됬는지 식별 값

	void StateChangeJumpDashIfPossible();

#ifdef BALANCE_CODE_NEMESIS_20121213
	virtual void ModifyFinalDamageByGameUnitType( OUT CX2DamageManager::DAMAGE_TYPE& damageType_, OUT float& fAttackerPhysicDamage_, OUT float& fAttackerMagicDamage_, OUT float& fForceDownValue_ );
#else
	virtual void ModifyFinalDamageByGameUnitType( OUT CX2DamageManager::DAMAGE_TYPE& damageType_, OUT float& fAttackerPhysicDamage_, OUT float& fAttackerMagicDamage_ );
#endif //BALANCE_CODE_NEMESIS_20121213

	void StartElCrystalSystem();
	void EndElCrystalSystem( bool bEnergeticHeartEnd_= false );

	void ChangeThousandsOfStarsFormation();
#ifdef FIX_EVE_TASER_PILUM
	int	GetTaserPilumAddDamageTime() const {return m_sTaserPilumTinyData.m_iAddDamageTime;}
	void ResetTaserPilumTinyData() { m_sTaserPilumTinyData.reset(); }
#endif // FIX_EVE_TASER_PILUM

#ifdef FIX_EVE_ELCRYSTAL_BUG
	void OnFrameMove_EndElCrystalSystem();
#endif // FIX_EVE_ELCRYSTAL_BUG

#ifdef FIX_SHOOTING_MEGABALL_AT_JUMP_LINE_MAP
	virtual void InitInpuDataProcess();
#endif // FIX_SHOOTING_MEGABALL_AT_JUMP_LINE_MAP
private:

	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahEveMajorParticleInstance[EVE_MAJOR_PII_END];
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahEveMinorParticleInstance[EVE_MINOR_PII_END];

protected:
	bool								m_bReAttackZ1Right;
	float								m_fReAttackZ1Time;
	float								m_fReDashJumpXTime;
	//bool								m_bZZZX;


	int			m_iJumpFlyCount;			
	int			m_iDashJumpCount;
#ifdef EVE_ELECTRA
	int			m_iDashJumpUpCount;
#endif EVE_ELECTRA
#ifdef SERV_EVE_BATTLE_SERAPH
	bool				m_bAfterJumpDash;		/// 평행 대쉬 이동일 경우
	bool				m_bEnableBoostDash;		/// 평행 대쉬 이동후 같은 방향으로 이동할 경우
	BOOST_DASH_STATE	m_eBoostDashState;		/// 부스트 대쉬 상태
#endif

	//int			m_iJumpDashCount;			
	float		m_fCurrDashJumpTime;		// dash_jump 혹은 jump_dash


	float		m_fInvisibilityTime;		// 투명화 유지 시간
	CKTDXTimer	m_timerInvisibility;
	float		m_fInvisibilityAlpha;		// 투명화 됐을 때 현재 알파값





	CKTDXTimer	m_timerSummoned;
	float		m_fSummonedLifeTime;		// 소환된 몬스터들 살아있는 시간

//{{ kimhc // 2010.11.2 // 엠프레스 - 충전 추진기
#ifdef	NEW_SKILL_2010_11
	// 이후에 map or manager클래스 등을 만들고, damageManager와 연계하는 방법을 생각해 봐야함
	CSkillDataBasePtr	m_ChargingBoosterSkillDataBasePtr;	/// 엠프: 충전추진기 스킬 사용 데이터
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.2 // 엠프레스 - 충전 추진기

#ifdef OLD_HAND_SLASH_TRACE
//{{AFX
	SlashTraceData			m_aSlashTraceData[ SlashID_END ];
//}}AFX
#endif OLD_HAND_SLASH_TRACE

	CKTDGXMeshPlayer::CXMeshInstanceHandle				m_ahMajorMeshInstance[MajorMID_END];
	CKTDGXMeshPlayer::CXMeshInstanceHandle				m_ahMinorMeshInstance[MinorMID_END];

	int			m_iSuccessiveHit_OneZ;
	D3DXVECTOR3 m_vSummonedUnitBonePos;
#ifdef EVE_FIRST_CHANGE_JOB
	EG_QueensThroneData	m_sEGQueensThrone;
	CKTDXTimer			m_TimerEGQTRefreshMsg;
#endif EVE_FIRST_CHANGE_JOB

#ifdef SKILL_BALANCE_PATCH
	CX2EffectSet::Handle	m_hOberonGuard;
#endif

	MEGA_ELECTRONBALL_Data	m_sMegaElectronBallData;
	float					m_fEquippedActiveSkill[EASI_END];	//장착된 액티브 스킬의 체크데이터 보관변수.
	float					m_fElectronBallRangeRate;			//일랙트론볼 연마를 배우면 거리조정을 위해.
	float					m_fMPConsumeRateElectronBall;		//일랙트론볼 사용시 마나소모량

//#ifdef EVE_SECOND_CLASS_CHANGE
	// oasis907 : 김상윤 [2010.1.29] // 엠프레스 패시브 스킬 - 일렉트론볼 마스터
	float					m_fAttackPowerRateElectronBall;		// 데미지 증가
	float					m_fForceDownValueRateElectronBall;  // 다운 수치 감소

//#endif EVE_SECOND_CLASS_CHANGE

#ifdef SKILL_30_TEST
	ATOveronGuardData		m_sATOveronGuardData;
#endif SKILL_30_TEST
	
	bool					m_bIsWinMotion2;
	bool					m_bIsLoseMotion2;

//{{ kimhc // 2010.2.10  // 이브 2차전직 
#ifdef	EVE_SECOND_CLASS_CHANGE
	bool					m_bDoneSpearBurst;	//	이브 스피어 버스트가 시전 됬는지 식별 값
#endif	EVE_SECOND_CLASS_CHANGE
//}} kimhc // 2010.2.10 //	이브 2차전직

	//{{ JHKang / 강정훈 / 2010/11/22 // 아토믹 쉴드 관련
#ifdef NEW_SKILL_2010_11
	CKTDXTimer					m_timerAtomicShieldSummoned;	/// 아토믹 쉴드 타이머
	float						m_fAtomicShieldLifeTime;		/// 아토믹 쉴드 생명 시간
#endif NEW_SKILL_2010_11
	//}} JHKang /  / 2010/11/22 // 아토믹 쉴드 관련

#ifdef EVE_ELECTRA
	EveElectraSystem			m_EveElectraSystem;
	float						m_fPlaneZAngle;

	bool						m_bBlinkInvisibility;
	float						m_fBlinkInvisibilityAlpha;

	float						m_iPhotonBoosterAdditionalDashJumpCount;
	float						m_fPhotonBlinkBackSpeedX;
	float						m_fPhotonBlinkDummyLifeTime;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffectHandle   	m_hEffectSweepParadeTriangle;
	CX2DamageEffect::CEffectHandle	    m_hEffectSweepParadeElectric;	
	CX2DamageEffect::CEffectHandle	    m_hEffectSweepParadeLight;
	CX2DamageEffect::CEffectHandle	    m_hEffectElectraDashComboZ;
	CX2DamageEffect::CEffectHandle	    m_hEffectElectraDashComboZZ;
	CX2DamageEffect::CEffectHandle	    m_hEffectElectraLaserZZZfrontZ;
	CX2DamageEffect::CEffectHandle	    m_hEffectElectraLaserXXZ;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect*	m_EffectSweepParadeTriangle;
	CX2DamageEffect::CEffect*	m_EffectSweepParadeElectric;	
	CX2DamageEffect::CEffect*	m_EffectSweepParadeLight;
	CX2DamageEffect::CEffect*	m_EffectElectraDashComboZ;
	CX2DamageEffect::CEffect*	m_EffectElectraDashComboZZ;
	CX2DamageEffect::CEffect*	m_EffectElectraLaserZZZfrontZ;
	CX2DamageEffect::CEffect*	m_EffectElectraLaserXXZ;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	D3DXVECTOR3					m_vInitScaleByUnitSweepParade;
	float						m_fInitDamageSweepParade;
	float						m_fSweepParadeTriangleLIfeTime;

	bool						m_bXXZLaserEnd;
	bool						m_bNotEnoughMPZZZfrontZLaser;
	bool						m_bNotEnoughMPXXZLaser;
	bool						m_bNotEnoughMPDashZZZ;


	CKTDGXMeshPlayer::CXMeshInstanceHandle  m_hElectraDashComboZMesh;
	//CKTDGXMeshPlayer::CXMeshInstanceHandle  m_hElectraDashComboZMeshZ;



#ifdef BALANCE_PATCH_20120329
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hCountDownThousandStar;
#endif BALANCE_PATCH_20120329

	float						m_fElapsedTimeThousandStartCreated;

	bool						m_bLockOnNpc;
	UidType						m_iLockOnUid;
	bool						m_bDelayAttackThousandStar;
	float						m_fDelayAttackTime;

	bool						m_bEntireFireMode;
	float						m_fDelayAttackTimeEntireFire;
	
	int							m_iThousandStarCount;

	float						m_fThousandStarPowerRate;


	float						m_fThousandStarAttackTime;
	FlyingImpactData*			m_pThousandStar;
#endif EVE_ELECTRA

#ifdef SERV_EVE_BATTLE_SERAPH
	bool						m_bDashJumpXXLaserEnd;				/// 레이저 종료
	bool						m_bNotEnoughMPDashJumpXZLaser;		/// MP 불충분
	bool						m_bExceptionPlane;					/// 예외적인 역장의 경우 확인
	float						m_fExceptioncPlaneZAngle;			/// 예외적인 역장의 각도값
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffectHandle	    m_hEffectElectraLaserDashJumpXZ;		/// 대쉬 점프 XX 레이저
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect*	m_EffectElectraLaserDashJumpXZ;		/// 대쉬 점프 XX 레이저
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hBoostDashParticle;		/// 부스트 대쉬 파티클 이펙트
	//CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hBoostDashMesh;			/// 부스트 대쉬 메쉬 이펙트
	
	TASER_PILUM_Data	m_sTaserPilumData;				/// 테이저 필라 데이터, 충전식 스킬( 메가 일렉트론볼 참고 )
	bool				m_bHighPoweredElectrics;		/// 고출력 전자회로 패시브 
	float				m_fHighPoweredElectricsSpeed;	/// 고출력 전자회로 패시브 스킬 부스트 대시 속도 증가 값
	int					m_iAccumulateAccel;				/// 부스트 대쉬 1초 속도 누적 Count( 1초 : 0.05%, 2초 : 0.1%, 3초 : 0.15% )

#ifdef FIX_EVE_TASER_PILUM
	TASER_PILUM_TinyData m_sTaserPilumTinyData;			/// 테이저 필라의 데이터가 발사 도중 변경되기 때문에 발사 중 쓰이는 데이터 추가
#endif // FIX_EVE_TASER_PILUM

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	SKugelBlitz_Data			m_sKugelBlitz_Data;
	SKugelBlitz_TinyData		m_sKugelBlitz_TinyData;
	
	float		m_fEffectStartTime;
	float		m_fAddSpectrumTime;
	float		m_fAddEnergeticHeartTime;
	D3DXVECTOR3	m_vSonicGunRotate;
#endif //UPGRADE_SKILL_SYSTEM_2013

	ENERGY_NEEDLES_Data	m_EnergyNiddleData;		/// 에너지 니들스 처리

	bool	m_bNotEnoughMPDashJumpZX;	/// 부스트 점프 공격 MP
	short	m_iBoostAttackCount;		/// 부스트 공격 누적 횟수
	bool	m_bBoostUpAttack;			/// false 일 때만 Up 공격이 가능함

	int		m_iAddBoostAttack;		/// 부스트 추가 공격 횟수
	bool	m_bNotEnoughMPDashZX;	/// 부스트 공격 마나ENSI_EBS_DASH_JUMP_COMBO_ZFrontX_End

	CX2EffectSet::Handle	m_hEffectEnergeticHeart;	/// 에너제틱 하트 이펙트
#endif

#ifdef FIX_SPECTRO_EL_CRYSTAL_ACTIVE_BUG
	bool	m_bStartSpectroElCrystal;	/// 분광결정이 스킬 사용을 통해 시작 되었는지 여부
#endif FIX_SPECTRO_EL_CRYSTAL_ACTIVE_BUG
	
#ifdef FINALITY_SKILL_SYSTEM //김창한
	bool		m_bStartLinkOverChargeIllusion; //링크 오버차지 일루젼 스킬이 동작하였는지 여부
	int			m_iFerdinandNPCUID;	//링크 오버차지 일루젼 npc 페르디난도
	CKTDXTimer	m_timerFerdinandSummoned;	/// 링크 오버차지 일루젼 npc 페르디난도 소환 타이며
	float		m_fFerdinandLifeTime;		/// 링크 오버차지 일루젼 npc 페르디난도 생명 시간
#endif //FINALITY_SKILL_SYSTEM
	
#ifdef FIX_EVE_ELCRYSTAL_BUG
	BUFF_TEMPLET_ID m_eReserveEndElCrystalBuffID;	// 역장 종료 예약한 버프 ID
	bool			m_bIsCheckEndElCrystal;			// 역장 종료 체크 여부
#endif // FIX_EVE_ELCRYSTAL_BUG

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	float										m_fDecraseInvisibilityMP;		// 클로킹 초당 마나 감소량, SA 에서는 마이너스 값으로 표기
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편


	D3DXVECTOR3 m_vecBoneILLUSIONSTINGER1;
	D3DXVECTOR3 m_vecBoneILLUSIONSTINGER2;


#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
	// 후에 CX2GameUnit * 모두 Observe Pointer 로 바꿔야함!!! , 
	ManeuverCoreData *		m_pManeuverCore;						// 각성 시 이브를 따라다니는 코어와 관련된 데이터
	int						m_iManeuverCoreLv;						// 기동 코어 레벨
	KProtectedType<float>	m_fManeuverGauge;						// 기동 게이지

	bool					m_bManeuverCoreAttackLockOnNpc;			// 기동 코어가 NPC 를 LockOn 한 것인가?
	UidType					m_iManeuverCoreAttackLockOnUid;			// 기동 코어가 Lock On 한 Unit 의 UID
	CX2GameUnitoPtr			m_pManeuverCoreTargetGameUnit;			// 기동 코어가 타켓한 유닛

	bool					m_bCanParrying;							// 방어형 코어를 전개 할 수 있는가?
	bool					m_bAttackManeuverCore;					// 공격형 코어를 전개 중인가?
	bool					m_bDelayAttackManeuverCore;				// 공격한 코어가 쿨타임 중인가?
	float					m_fManeuverCoreStateTime;				// 특정 경우에 필요한 코어의 State 별로 돌아가는 Time	

	bool					m_bHyperState;									// 3각성 상태인가?
	
#ifdef FIX_ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템 수정 사항
	// 이펙트 셋에서 데미지 이펙트로 변경함에 따라 수정
	CX2DamageEffect::CEffect*	m_pManeuverCoreParryingEffect;				// 방어형 코어 전개 시 이펙트 핸들
#else // FIX_ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템 수정 사항
	CX2EffectSet::Handle		m_hManeuverCoreParryingEffect;				// 방어형 코어 전개 시 이펙트 핸들
#endif // FIX_ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템 수정 사항
	
	
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqHyperBall;	// 3각성 시 구슬 이펙트
	CX2DamageEffect::CEffect*	m_pManeuverCoreAttackDamageEffect;			// 기동 코어 공격의 데미지 이펙트 핸들
	float						m_fManeuverCoreAttackParticleCreateCoolTime;	// 3각성 기동 게이지 사용 공격 시, 이펙트 뿌려주는 시간
#ifdef FIX_ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템 수정 사항
	wstring					m_wstrManeuverCoreTargetBoneName;				// 기동 코어가 타켓한 본 이름
#endif // FIX_ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템 수정 사항

#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어

protected:
	//ENSI_DIE_FRONT
	void ENSI_DIE_FRONT_StartFuture();
	void ENSI_DIE_FRONT_Start();
	void ENSI_DIE_FRONT_FrameMove();
	void ENSI_DIE_FRONT_EventProcess();

	//ENSI_DIE_BACK
	void ENSI_DIE_BACK_StartFuture();
	void ENSI_DIE_BACK_Start();
	void ENSI_DIE_BACK_FrameMove();
	void ENSI_DIE_BACK_EventProcess();
	
	//ENSI_WAIT
	void ENSI_WAIT_StartFuture();
	void ENSI_WAIT_Start();
	void ENSI_WAIT_EventProcess();

	//ENSI_WALK
	void ENSI_WALK_EventProcess();

	//ENSI_JUMP_READY
	void ENSI_JUMP_READY_EventProcess();

	//ENSI_JUMP_UP
	void ENSI_JUMP_UP_EventProcess();

	//ENSI_JUMP_DOWN
	void ENSI_JUMP_DOWN_EventProcess();

	//ENSI_JUMP_LANDING
	void ENSI_JUMP_LANDING_StartFuture();
	void ENSI_JUMP_LANDING_EventProcess();

	//ENSI_DASH
	virtual void DashStartFuture();
	void ENSI_DASH_Start();
	void ENSI_DASH_FrameMoveFuture();
	void ENSI_DASH_EventProcess();

	//ENSI_DASH_END
	void ENSI_DASH_END_StartFuture();
	void ENSI_DASH_END_Start();
	void ENSI_DASH_END_FrameMove();
	void ENSI_DASH_END_EventProcess();

	//ENSI_DASH_JUMP
	void ENSI_DASH_JUMP_StartFuture();
	void ENSI_DASH_JUMP_Start();
	void ENSI_DASH_JUMP_FrameMoveFuture();
	void ENSI_DASH_JUMP_EventProcess();
	void ENSI_DASH_JUMP_EndFuture();

	//ENSI_DASH_JUMP_LANDING
	void ENSI_DASH_JUMP_LANDING_StartFuture();
	void ENSI_DASH_JUMP_LANDING_Start();
	void ENSI_DASH_JUMP_LANDING_EventProcess();

	//ENSI_DASH_JUMP_POWER_LANDING
	void ENSI_DASH_JUMP_POWER_LANDING_StartFuture();
	void ENSI_DASH_JUMP_POWER_LANDING_Start();
	void ENSI_DASH_JUMP_POWER_LANDING_EventProcess();




	void ENSI_JUMP_FLY_UP_StartFuture();
	void ENSI_JUMP_FLY_UP_Start();
	void ENSI_JUMP_FLY_UP_FrameMoveFuture();
	void ENSI_JUMP_FLY_UP_EventProcess();
	void ENSI_JUMP_FLY_UP_EndFuture();



	void ENSI_JUMP_FLY_DOWN_StartFuture();
	void ENSI_JUMP_FLY_DOWN_FrameMoveFuture();
	void ENSI_JUMP_FLY_DOWN_EventProcess();
	void ENSI_JUMP_FLY_DOWN_EndFuture();



	void ENSI_JUMP_DASH_StartFuture();
	void ENSI_JUMP_DASH_Start();
	void ENSI_JUMP_DASH_FrameMoveFuture();
	void ENSI_JUMP_DASH_EventProcess();
	void ENSI_JUMP_DASH_EndFuture();





	//ENSI_HYPER_MODE
	void HyperModeFrameMove();
#ifdef MODIFY_RIDING_PET_AWAKE
	void RidingHyperModeFrameMove();
	void CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ = false );
#endif // MODIFY_RIDING_PET_AWAKE

	//ENSI_DAMAGE_GROGGY
	//void ENSI_DAMAGE_GROGGY_FrameMoveFuture();
	//void ENSI_DAMAGE_GROGGY_EventProcess();

	//ENSI_DAMAGE_SMALL_FRONT
	void ENSI_DAMAGE_SMALL_FRONT_EventProcess();

	//ENSI_DAMAGE_SMALL_BACK
	void ENSI_DAMAGE_SMALL_BACK_EventProcess();

	//ENSI_DAMAGE_BIG_FRONT
	void ENSI_DAMAGE_BIG_FRONT_Start();
	void ENSI_DAMAGE_BIG_FRONT_EventProcess();

	//ENSI_DAMAGE_BIG_BACK
	void ENSI_DAMAGE_BIG_BACK_Start();
	void ENSI_DAMAGE_BIG_BACK_EventProcess();

	//ENSI_DAMAGE_DOWN_FRONT
	void ENSI_DAMAGE_DOWN_FRONT_FrameMove();
	void ENSI_DAMAGE_DOWN_FRONT_EventProcess();

	//ENSI_DAMAGE_DOWN_BACK
	void ENSI_DAMAGE_DOWN_BACK_FrameMove();
	void ENSI_DAMAGE_DOWN_BACK_EventProcess();

	//ENSI_DAMAGE_AIR_DOWN_LANDING
	void ENSI_DAMAGE_AIR_DOWN_LANDING_EventProcess();

	//ENSI_STANDUP_ROLLING_FRONT_FRONT
	void ENSI_STANDUP_ROLLING_FRONT_FRONT_EventProcess();
	void ENSI_STANDUP_ROLLING_FRONT_FRONT_End();

	//ENSI_STANDUP_ROLLING_FRONT_BACK
	void ENSI_STANDUP_ROLLING_FRONT_BACK_EventProcess();
	void ENSI_STANDUP_ROLLING_FRONT_BACK_End();

	//ENSI_STANDUP_ROLLING_BACK_FRONT
	void ENSI_STANDUP_ROLLING_BACK_FRONT_EventProcess();
	void ENSI_STANDUP_ROLLING_BACK_FRONT_End();

	//ENSI_STANDUP_ROLLING_BACK_BACK
	void ENSI_STANDUP_ROLLING_BACK_BACK_EventProcess();
	void ENSI_STANDUP_ROLLING_BACK_BACK_End();

	//ENSI_STANDUP_ATTACK_FRONT
	void ENSI_STANDUP_ATTACK_FRONT_FrameMove();
	void ENSI_STANDUP_ATTACK_FRONT_EventProcess();
	void ENSI_STANDUP_ATTACK_FRONT_End();

	//ENSI_STANDUP_ATTACK_BACK
	void ENSI_STANDUP_ATTACK_BACK_FrameMove();
	void ENSI_STANDUP_ATTACK_BACK_EventProcess();
	void ENSI_STANDUP_ATTACK_BACK_End();




	void ENSI_COMBO_Z_Start();
	void ENSI_COMBO_Z_End();
	void ENSI_COMBO_Z_FrameMove();
	void ENSI_COMBO_Z_EventProcess();

	void ENSI_COMBO_ZZ_Start();
	void ENSI_COMBO_ZZ_End();
	void ENSI_COMBO_ZZ_FrameMove();
	void ENSI_COMBO_ZZ_EventProcess();

	void ENSI_COMBO_ZZZ_Start();
	void ENSI_COMBO_ZZZ_End();
	void ENSI_COMBO_ZZZ_FrameMove();
	void ENSI_COMBO_ZZZ_EventProcess();

	void ENSI_COMBO_ZZZZ_Start();
	void ENSI_COMBO_ZZZZ_End();
	void ENSI_COMBO_ZZZZ_FrameMove();
	void ENSI_COMBO_ZZZZ_EventProcess();



	void ENSI_COMBO_ZZZX_FrameMove();
	void ENSI_COMBO_ZZZX_EventProcess();



	void ENSI_COMBO_ZZZdownZ_StartFuture();
	void ENSI_COMBO_ZZZdownZ_Start();
	void ENSI_COMBO_ZZZdownZ_End();
	void ENSI_COMBO_ZZZdownZ_FrameMove();
	void ENSI_COMBO_ZZZdownZ_EventProcess();

	//{{ 09.01.21.hoons. 전직용 함수로 사용되어짐.
	// : 기존 이브 테스트 코드로 남아있어서 이름교체를 따로 하지않음.
	void ENSI_COMBO_ZZZdownZZ_Init();
	void ENSI_COMBO_ZZZdownZZ_StartFuture();
	void ENSI_COMBO_ZZZdownZZ_Start();
	void ENSI_COMBO_ZZZdownZZ_End();
	void ENSI_COMBO_ZZZdownZZ_FrameMove();
	void ENSI_COMBO_ZZZdownZZ_EventProcess();
	//}}




	void ENSI_COMBO_X_FrameMove();
	void ENSI_COMBO_X_EventProcess();
	void ENSI_COMBO_XX_FrameMove();
	void ENSI_COMBO_XX_EventProcess();
#ifdef EVE_FIRST_CHANGE_JOB
	void ENSI_COMBO_XXZ4_Init();
	void ENSI_COMBO_XXZ4_FrameMove();
	void ENSI_COMBO_XXZ4_EventProcess();
#endif EVE_FIRST_CHANGE_JOB
	void ENSI_COMBO_XXX_FrameMove();
	void ENSI_COMBO_XXX_EventProcess();
#ifdef EVE_ELECTRA
	//void ENSI_COMBO_XXfrontX_FrameMove();
	void ENSI_COMBO_XXfrontX_EventProcess();
	void ENSI_COMBO_XXfrontXX_FrameMove();
	void ENSI_COMBO_XXfrontXX_EventProcess();

	//void ENSI_COMBO_XXdownX_FrameMove();
	void ENSI_COMBO_XXdownX_EventProcess();
	void ENSI_COMBO_XXdownXX_FrameMove();
	void ENSI_COMBO_XXdownXX_EventProcess();
#endif EVE_ELECTRA

	void ENSI_DASH_COMBO_Z_Start();
	void ENSI_DASH_COMBO_Z_End();
	void ENSI_DASH_COMBO_Z_FrameMove();
	void ENSI_DASH_COMBO_Z_EventProcess();


#ifdef EVE_COMBO_TEST_TEST
	void ENSI_DASH_COMBO_ZX_FrameMove();
	void ENSI_DASH_COMBO_ZX_EventProcess();
#endif EVE_COMBO_TEST_TEST

#ifdef EVE_FIRST_CHANGE_JOB
	void ENSI_DASH_COMBO_ZZ_Init();
	void ENSI_DASH_COMBO_ZZ_StartFuture();
	void ENSI_DASH_COMBO_ZZ_Start();
	void ENSI_DASH_COMBO_ZZ_End();
	void ENSI_DASH_COMBO_ZZ_FrameMove();
	void ENSI_DASH_COMBO_ZZ_EventProcess();

	void ENSI_DASH_COMBO_ZZZ_StartFuture();
	void ENSI_DASH_COMBO_ZZZ_Start();
	void ENSI_DASH_COMBO_ZZZ_End();
	void ENSI_DASH_COMBO_ZZZ_FrameMove();
	void ENSI_DASH_COMBO_ZZZ_EventProcess();
#endif EVE_FIRST_CHANGE_JOB



	void ENSI_DASH_COMBO_X_FrameMove();
	void ENSI_DASH_COMBO_X_EventProcess();

#ifdef EVE_FIRST_CHANGE_JOB
	void ENSI_DASH_COMBO_XZ_FrameMove();
	void ENSI_DASH_COMBO_XZ_EventProcess();
#endif EVE_FIRST_CHANGE_JOB



	void ENSI_JUMP_COMBO_Z_Start();
	void ENSI_JUMP_COMBO_Z_End();
	void ENSI_JUMP_COMBO_Z_FrameMove();
	void ENSI_JUMP_COMBO_Z_EventProcess();

	void ENSI_JUMP_COMBO_X_Start();
	void ENSI_JUMP_COMBO_X_End();
	void ENSI_JUMP_COMBO_X_FrameMove();		
	void ENSI_JUMP_COMBO_X_EventProcess();



	void ENSI_JUMP_COMBO_X_LANDING_StartFuture();
	void ENSI_JUMP_COMBO_X_LANDING_Start();
	void ENSI_JUMP_COMBO_X_LANDING_EventProcess();

	void ENSI_DASH_JUMP_COMBO_Z_Start();
	void ENSI_DASH_JUMP_COMBO_Z_End();
	void ENSI_DASH_JUMP_COMBO_Z_FrameMove();
	void ENSI_DASH_JUMP_COMBO_Z_EventProcess();

	void ENSI_DASH_JUMP_COMBO_ZZ_Start();
	void ENSI_DASH_JUMP_COMBO_ZZ_End();
	void ENSI_DASH_JUMP_COMBO_ZZ_FrameMove();
	void ENSI_DASH_JUMP_COMBO_ZZ_EventProcess();

	void ENSI_DASH_JUMP_COMBO_ZZZ_Start();
	void ENSI_DASH_JUMP_COMBO_ZZZ_End();
	void ENSI_DASH_JUMP_COMBO_ZZZ_FrameMove();
	void ENSI_DASH_JUMP_COMBO_ZZZ_EventProcess();

#ifdef EVE_FIRST_CHANGE_JOB
	void ENSI_DASH_JUMP_COMBO_ZX_FrameMove();
	void ENSI_DASH_JUMP_COMBO_ZX_EventProcess();
	void ENSI_DASH_JUMP_COMBO_ZX_End();
#endif EVE_FIRST_CHANGE_JOB



	
	void ENSI_DASH_JUMP_COMBO_X_Start();
	void ENSI_DASH_JUMP_COMBO_X_End();
	void ENSI_DASH_JUMP_COMBO_X_FrameMove();
	void ENSI_DASH_JUMP_COMBO_X_EventProcess();

#ifdef EVE_FIRST_CHANGE_JOB
	void ENSI_DASH_JUMP_COMBO_XZ_Init();
	void ENSI_DASH_JUMP_COMBO_XZ_FrameMove();
	void ENSI_DASH_JUMP_COMBO_XZ_EventProcess();
	void ENSI_DASH_JUMP_COMBO_XZ_End();
#endif EVE_FIRST_CHANGE_JOB

//{{ kimhc // 2010.1.25 //	이브 2차 전직 스킬 및 콤보
#ifdef	EVE_SECOND_CLASS_CHANGE

	// 네메시스 콤보
	void ENSI_NS_DASH_COMBO_ZX_Init();
	void ENSI_NS_DASH_COMBO_ZX_Start();
	void ENSI_NS_DASH_COMBO_ZX_FrameMove();
	void ENSI_NS_DASH_COMBO_ZX_EventProcess();
	

	void ENSI_NS_DASH_COMBO_ZXX_Init();
	void ENSI_NS_DASH_COMBO_ZXX_FrameMove();
	void ENSI_NS_DASH_COMBO_ZXX_EventProcess();

	void ENSI_NS_COMBO_XXZ_FrameMove();
	void ENSI_NS_COMBO_XXZ_EventProcess();
	
	void ENSI_NS_COMBO_XXZZ8_FrameMove();
	void ENSI_NS_COMBO_XXZZ8_EventProcess();
	
	void ENSI_NS_COMBO_XXZZ8Z_FrameMove();
	void ENSI_NS_COMBO_XXZZ8Z_EventProcess();

	// 엠프레스 콤보
	void ENSI_EP_DASH_COMBO_ZZ_Init();
	void ENSI_EP_DASH_COMBO_ZZ_FrameMove();
	void ENSI_EP_DASH_COMBO_ZZ_EventProcess();

	void ENSI_EP_DASH_COMBO_ZZZ_Init();
	void ENSI_EP_DASH_COMBO_ZZZ_FrameMove();
	void ENSI_EP_DASH_COMBO_ZZZ_EventProcess();

	void ENSI_EP_JUMP_COMBO_ZX_Init();
	void ENSI_EP_JUMP_COMBO_ZX_FrameMove();
	void ENSI_EP_JUMP_COMBO_ZX_EventProcess();

	void ENSI_EP_COMBO_XXdownX_Init();
	void ENSI_EP_COMBO_XXdownX_FrameMove();
	void ENSI_EP_COMBO_XXdownX_EventProcess();

	// 네메시스 스킬
	void ENSI_SA_ENS_ATOMIC_BLASTER_Init();
	void ENSI_SA_ENS_ATOMIC_BLASTER_Start();
	void ENSI_SA_ENS_ATOMIC_BLASTER_FrameMove();
	void ENSI_SA_ENS_ATOMIC_BLASTER_EventProcess();
	void ENSI_SA_ENS_ATOMIC_BLASTER_End();


	void ENSI_SA_ENS_ATOMIC_BLASTER_Hyper_Init();
	void ENSI_SA_ENS_ATOMIC_BLASTER_Hyper_Start();
	void ENSI_SA_ENS_ATOMIC_BLASTER_Hyper_FrameMove();
	void ENSI_SA_ENS_ATOMIC_BLASTER_Hyper_EventProcess();
	void ENSI_SA_ENS_ATOMIC_BLASTER_Hyper_End();

	void ENSI_A_ENS_BURST_SPEAR_Init();
	void ENSI_A_ENS_BURST_SPEAR_Start();
	void ENSI_A_ENS_BURST_SPEAR_FrameMove();
	void ENSI_A_ENS_BURST_SPEAR_EventProcess();
	void ENSI_A_ENS_BURST_SPEAR_End();

	void ENSI_A_ENS_BURST_SPEAR_Hyper_Init();
	void ENSI_A_ENS_BURST_SPEAR_Hyper_Start();
	void ENSI_A_ENS_BURST_SPEAR_Hyper_FrameMove();
	void ENSI_A_ENS_BURST_SPEAR_Hyper_EventProcess();
	void ENSI_A_ENS_BURST_SPEAR_Hyper_End();

	void ENSI_A_ENS_BURST_SPEAR_Landing_FrameMove();
	void ENSI_A_ENS_BURST_SPEAR_Landing_EventProcess();

	void ENSI_A_ENS_BURST_SPEAR_Landing_Hyper_FrameMove();
	void ENSI_A_ENS_BURST_SPEAR_Landing_Hyper_EventProcess();



	// oasis907 : 김상윤 [2010.1.28] //  엠프레스 스킬 
	void ENSI_SA_EEP_ELECTRONIC_FIELD_Init();
	void ENSI_SA_EEP_ELECTRONIC_FIELD_FrameMove();
	void ENSI_SA_EEP_ELECTRONIC_FIELD_EventProcess();

	void ENSI_SA_EEP_ELECTRONIC_FIELD_Hyper_Init();
	void ENSI_SA_EEP_ELECTRONIC_FIELD_Hyper_FrameMove();
	void ENSI_SA_EEP_ELECTRONIC_FIELD_Hyper_EventProcess();

	void ENSI_A_EEP_SPIT_FIRE_Init();
	void ENSI_A_EEP_SPIT_FIRE_FrameMove();
	void ENSI_A_EEP_SPIT_FIRE_EventProcess();

	void ENSI_A_EEP_SPIT_FIRE_Hyper_Init();
	void ENSI_A_EEP_SPIT_FIRE_Hyper_FrameMove();
	void ENSI_A_EEP_SPIT_FIRE_Hyper_EventProcess();


#endif	EVE_SECOND_CLASS_CHANGE
//}} kimhc // 2010.1.25 //  이브 2차 전직 스킬 및 콤보
	void ENSI_INVISIBILITY_ON_Start();
	void ENSI_INVISIBILITY_ON_FrameMove();
	void ENSI_INVISIBILITY_ON_EventProcess();
	void ENSI_INVISIBILITY_ON_End();





	// 여기서 부터 필살기
	void ENSI_SI1_ILLUSION_STRIKE_Init();
	//void ENSI_SI1_ILLUSION_STRIKE_CameraMove();
	void ENSI_SI1_ILLUSION_STRIKE_FrameMove();
	void ENSI_SI1_ILLUSION_STRIKE_EventProcess();
	void ENSI_SI1_ILLUSION_STRIKE_HYPER_FrameMove();
	void ENSI_SI1_ILLUSION_STRIKE_HYPER_EventProcess();


	void ENSI_SI2_ILLUSION_STINGER_Init();
	void ENSI_SI2_ILLUSION_STINGER_Start();
	void ENSI_SI2_ILLUSION_STINGER_End();
	//void ENSI_SI2_ILLUSION_STINGER_CameraMove();
	void ENSI_SI2_ILLUSION_STINGER_FrameMove();
	void ENSI_SI2_ILLUSION_STINGER_EventProcess();

	void ENSI_SI2_ILLUSION_STINGER_HYPER_Start();
	void ENSI_SI2_ILLUSION_STINGER_HYPER_End();
	void ENSI_SI2_ILLUSION_STINGER_HYPER_FrameMove();
	void ENSI_SI2_ILLUSION_STINGER_HYPER_EventProcess();


	void ENSI_SI3_GENERATE_BLACKHOLE_Init();
	//void ENSI_SI3_GENERATE_BLACKHOLE_CameraMove();
	void ENSI_SI3_GENERATE_BLACKHOLE_FrameMove();
	void ENSI_SI3_GENERATE_BLACKHOLE_EventProcess();
	void ENSI_SI3_GENERATE_BLACKHOLE_HYPER_FrameMove();
	void ENSI_SI3_GENERATE_BLACKHOLE_HYPER_EventProcess();







	void ENSI_SI1_DIMENSION_LINK_Init();
	//void ENSI_SI1_DIMENSION_LINK_CameraMove();
	void ENSI_SI1_DIMENSION_LINK_FrameMove();
	void ENSI_SI1_DIMENSION_LINK_EventProcess();
	void ENSI_SI1_DIMENSION_LINK_HYPER_FrameMove();
	void ENSI_SI1_DIMENSION_LINK_HYPER_EventProcess();



	void ENSI_SI2_DIMENSION_LINK_BLADER_Init();
	//void ENSI_SI2_DIMENSION_LINK_BLADER_CameraMove();
	void ENSI_SI2_DIMENSION_LINK_BLADER_FrameMove();
	void ENSI_SI2_DIMENSION_LINK_BLADER_EventProcess();
	void ENSI_SI2_DIMENSION_LINK_BLADER_HYPER_FrameMove();
	void ENSI_SI2_DIMENSION_LINK_BLADER_HYPER_EventProcess();



	void ENSI_SI3_DIMENSION_LINK_GUARDIAN_Init();
	//void ENSI_SI3_DIMENSION_LINK_GUARDIAN_CameraMove();
	void ENSI_SI3_DIMENSION_LINK_GUARDIAN_FrameMove();
	void ENSI_SI3_DIMENSION_LINK_GUARDIAN_EventProcess();
	void ENSI_SI3_DIMENSION_LINK_GUARDIAN_HYPER_FrameMove();
	void ENSI_SI3_DIMENSION_LINK_GUARDIAN_HYPER_EventProcess();

	//COMMON_BUFF
	virtual void COMMON_BUFF_FrameMove();

#ifdef EVE_FIRST_CHANGE_JOB
	// ARCHITECTURE
	void ENSI_SI1_AT_SURFACE_CUTING_Init();
	void ENSI_SI1_AT_SURFACE_CUTING_FrameMove();
	void ENSI_SI1_AT_SURFACE_CUTING_EventProcess();
	void ENSI_SI1_AT_SURFACE_CUTING_HYPER_FrameMove();
	void ENSI_SI1_AT_SURFACE_CUTING_HYPER_EventProcess();

	void ENSI_SI2_AT_SONIC_WAVE_Init();
	void ENSI_SI2_AT_SONIC_WAVE_FrameMove();
	void ENSI_SI2_AT_SONIC_WAVE_EventProcess();
	void ENSI_SI2_AT_SONIC_WAVE_HYPER_FrameMove();
	void ENSI_SI2_AT_SONIC_WAVE_HYPER_EventProcess();

	void ENSI_SI3_AT_GENOCIDE_RIPPER_Init();
	void ENSI_SI3_AT_GENOCIDE_RIPPER_Start();
	void ENSI_SI3_AT_GENOCIDE_RIPPER_FrameMove();
	void ENSI_SI3_AT_GENOCIDE_RIPPER_EventProcess();
	void ENSI_SI3_AT_GENOCIDE_RIPPER_HYPER_FrameMove();
	void ENSI_SI3_AT_GENOCIDE_RIPPER_HYPER_EventProcess();

	// EXOTIC GEAR
	void ENSI_SI1_EG_HORNET_STING_Init();
	void ENSI_SI1_EG_HORNET_STING_FrameMove();
	void ENSI_SI1_EG_HORNET_STING_EventProcess();
	void ENSI_SI1_EG_HORNET_STING_HYPER_FrameMove();
	void ENSI_SI1_EG_HORNET_STING_HYPER_EventProcess();

	void ENSI_SI2_EG_EXPLOSION_IMPACT_Init();
	void ENSI_SI2_EG_EXPLOSION_IMPACT_Start();
	void ENSI_SI2_EG_EXPLOSION_IMPACT_End();
	void ENSI_SI2_EG_EXPLOSION_IMPACT_FrameMove();
	void ENSI_SI2_EG_EXPLOSION_IMPACT_EventProcess();
	void ENSI_SI2_EG_EXPLOSION_IMPACT_HYPER_FrameMove();
	void ENSI_SI2_EG_EXPLOSION_IMPACT_HYPER_EventProcess();

	void ENSI_SI3_EG_QUEENS_THRONE_Init();
	void ENSI_SI3_EG_QUEENS_THRONE_Start();
	void ENSI_SI3_EG_QUEENS_THRONE_End();
	void ENSI_SI3_EG_QUEENS_THRONE_FrameMove();
	void ENSI_SI3_EG_QUEENS_THRONE_EventProcess();
	void ENSI_SI3_EG_QUEENS_THRONE_HYPER_FrameMove();
	void ENSI_SI3_EG_QUEENS_THRONE_HYPER_EventProcess();

#endif EVE_FIRST_CHANGE_JOB

	// 마력전환
	void ENSI_A_EN_CONVERSION_Init();
	void ENSI_A_EN_CONVERSION_Start();
	void ENSI_A_EN_CONVERSION_EventProcess();

	// 메가일랙트론볼
	void ENSI_A_EN_MEGA_ELECTRONBALL_Init();
	void ENSI_A_EN_MEGA_ELECTRONBALL_StartFuture();
	void ENSI_A_EN_MEGA_ELECTRONBALL_Start();
	void ENSI_A_EN_MEGA_ELECTRONBALL_FrameMoveFuture();
	void ENSI_A_EN_MEGA_ELECTRONBALL_FrameMove();
	void ENSI_A_EN_MEGA_ELECTRONBALL_EventProcess();
	void ENSI_A_EN_MEGA_ELECTRONBALL_EndFuture();


#ifdef SKILL_30_TEST
	void ENSI_SA_EEG_IRONSCRAPS_Init();
	void ENSI_SA_EEG_IRONSCRAPS_StateStart();
	void ENSI_SA_EEG_IRONSCRAPS_FrameMove();				
	void ENSI_SA_EEG_IRONSCRAPS_EventProcess();				
	void ENSI_SA_EEG_IRONSCRAPS_StateEnd();

	void ENSI_SA_EEG_IRONSCRAPS_HYPER_Init();
	void ENSI_SA_EEG_IRONSCRAPS_HYPER_StateStart();
	void ENSI_SA_EEG_IRONSCRAPS_HYPER_FrameMove();		
	void ENSI_SA_EEG_IRONSCRAPS_HYPER_EventProcess();	
	void ENSI_SA_EEG_IRONSCRAPS_HYPER_StateEnd();


	void ENSI_SA_EAT_SPACE_WRENCH_StateStart();
	void ENSI_SA_EAT_SPACE_WRENCH_FrameMove();				
	void ENSI_SA_EAT_SPACE_WRENCH_EventProcess();			
	void ENSI_SA_EAT_SPACE_WRENCH_StateEnd();

	void ENSI_SA_EAT_SPACE_WRENCH_HYPER_StateStart();
	void ENSI_SA_EAT_SPACE_WRENCH_HYPER_FrameMove();		
	void ENSI_SA_EAT_SPACE_WRENCH_HYPER_EventProcess();	
	void ENSI_SA_EAT_SPACE_WRENCH_HYPER_StateEnd();

	//ENSI_A_EEG_METAL_DUST_AURA
	void ENSI_A_EEG_METAL_DUST_AURA_Init();
	void ENSI_A_EEG_METAL_DUST_AURA_StateStart();
	void ENSI_A_EEG_METAL_DUST_AURA_FrameMove();				
	void ENSI_A_EEG_METAL_DUST_AURA_EventProcess();			
	void ENSI_A_EEG_METAL_DUST_AURA_StateEnd();

	void ENSI_A_EEG_METAL_DUST_AURA_HYPER_Init();
	void ENSI_A_EEG_METAL_DUST_AURA_HYPER_StateStart();
	void ENSI_A_EEG_METAL_DUST_AURA_HYPER_FrameMove();		
	void ENSI_A_EEG_METAL_DUST_AURA_HYPER_EventProcess();	
	void ENSI_A_EEG_METAL_DUST_AURA_HYPER_StateEnd();

	//ENSI_A_EAT_OBERON_GUARD
	void ENSI_A_EAT_OBERON_GUARD_Init();
	void ENSI_A_EAT_OBERON_GUARD_StateStart();
	void ENSI_A_EAT_OBERON_GUARD_FrameMove();				
	void ENSI_A_EAT_OBERON_GUARD_EventProcess();			
	void ENSI_A_EAT_OBERON_GUARD_StateEnd();

	void ENSI_A_EAT_OBERON_GUARD_HYPER_Init();
	void ENSI_A_EAT_OBERON_GUARD_HYPER_StateStart();
	void ENSI_A_EAT_OBERON_GUARD_HYPER_FrameMove();		
	void ENSI_A_EAT_OBERON_GUARD_HYPER_EventProcess();	
	void ENSI_A_EAT_OBERON_GUARD_HYPER_StateEnd();

#endif SKILL_30_TEST

#ifdef SKILL_CASH_10_TEST
	void ENSI_A_EEG_JUNK_BREAK_Init();
	void ENSI_A_EEG_JUNK_BREAK_FrameMove();
	void ENSI_A_EEG_JUNK_BREAK_Hyper_FrameMove();
	void ENSI_A_EEG_JUNK_BREAK_EventProcess();
	void ENSI_A_EEG_JUNK_BREAK_Hyper_EventProcess();

	void ENSI_A_EAT_HEAVENS_FIST_Init();
	void ENSI_A_EAT_HEAVENS_FIST_FrameMove();
	void ENSI_A_EAT_HEAVENS_FIST_Hyper_FrameMove();
	void ENSI_A_EAT_HEAVENS_FIST_EventProcess();
	void ENSI_A_EAT_HEAVENS_FIST_Hyper_EventProcess();
#endif

#ifdef NEW_SKILL_2010_11
	//{{ JHKang / 강정훈 / 2010/11/08 / 어설트 스피어
	void ENSI_SA_EAT_ASSUALT_SPEAR_Init();
	void ENSI_SA_EAT_ASSUALT_SPEAR_FrameMove();
	void ENSI_SA_EAT_ASSUALT_SPEAR_EventProcess();
	void ENSI_SA_EAT_ASSUALT_SPEAR_HYPER_FrameMove();
	void ENSI_SA_EAT_ASSUALT_SPEAR_HYPER_EventProcess();
	//}} JHKang / 강정훈 / 2010/11/08 / 어설트 스피어

	//{{ JHKang / 강정훈 / 2010/11/11 / 아토믹 쉴드
	void ENSI_SA_EEG_ATOMIC_SHIELD_Init();
	void ENSI_SA_EEG_ATOMIC_SHIELD_StateStart();
	void ENSI_SA_EEG_ATOMIC_SHIELD_FrameMove();
	void ENSI_SA_EEG_ATOMIC_SHIELD_EventProcess();
	void ENSI_SA_EEG_ATOMIC_SHIELD_HYPER_StateStart();
	void ENSI_SA_EEG_ATOMIC_SHIELD_HYPER_FrameMove();
	void ENSI_SA_EEG_ATOMIC_SHIELD_HYPER_EventProcess();
	//}} JHKang / 강정훈 / 2010/11/11 / 아토믹 쉴드
#endif NEW_SKILL_2010_11

	void ShowActiveSkillCutInAndLight( const WCHAR* szBoneName_, const float fTimeToShow_, const UINT uiCutInIndex_, const bool bOnlyLight_ = false );
	virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

#ifdef EVE_ELECTRA
	void ENSI_COMBO_X_Start();
	void ENSI_COMBO_XX_Start();
	void ENSI_COMBO_XXX_Start();

	void ENSI_EPR_COMBO_ZZZfrontZ_Start();
	void ENSI_EPR_COMBO_ZZZfrontZ_End();
	void ENSI_EPR_COMBO_ZZZfrontZ_FrameMove();
	void ENSI_EPR_COMBO_ZZZfrontZ_EventProcess();


	void ENSI_EPR_COMBO_XXZ_Start();
	void ENSI_EPR_COMBO_XXZ_End();
	void ENSI_EPR_COMBO_XXZ_FrameMove();
	void ENSI_EPR_COMBO_XXZ_EventProcess();

	void ENSI_EPR_DASH_COMBO_ZZ_Init();
	void ENSI_EPR_DASH_COMBO_ZZ_StartFuture();
	void ENSI_EPR_DASH_COMBO_ZZ_Start();
	void ENSI_EPR_DASH_COMBO_ZZ_EventProcess();
	void ENSI_EPR_DASH_COMBO_ZZ_FrameMove();
	void ENSI_EPR_DASH_COMBO_ZZ_End();

	void ENSI_EPR_DASH_COMBO_ZZZ_Init();
	void ENSI_EPR_DASH_COMBO_ZZZ_StartFuture();
	void ENSI_EPR_DASH_COMBO_ZZZ_Start();
	void ENSI_EPR_DASH_COMBO_ZZZ_EventProcess();
	void ENSI_EPR_DASH_COMBO_ZZZ_FrameMove();
	void ENSI_EPR_DASH_COMBO_ZZZ_EndFuture();
	void ENSI_EPR_DASH_COMBO_ZZZ_End();

	void ENSI_A_EVE_PHOTON_BLINK_Init();
	void ENSI_A_EVE_PHOTON_BLINK_Start();
	void ENSI_A_EVE_PHOTON_BLINK_StartFuture();
	void ENSI_A_EVE_PHOTON_BLINK_FrameMoveFuture();
	void ENSI_A_EVE_PHOTON_BLINK_FrameMove();
	void ENSI_A_EVE_PHOTON_BLINK_EventProcess();
	void ENSI_A_EVE_PHOTON_BLINK_EndFuture();
	void ENSI_A_EVE_PHOTON_BLINK_End();


	void ENSI_A_EPR_SPECTRO_EL_CRYSTAL_Init();
	void ENSI_A_EPR_SPECTRO_EL_CRYSTAL_Start();
	void ENSI_A_EPR_SPECTRO_EL_CRYSTAL_FrameMove();
	void ENSI_A_EPR_SPECTRO_EL_CRYSTAL_EventProcess();

	void ENSI_SA_EPR_PARTICLE_RAY_Init();
	void ENSI_SA_EPR_PARTICLE_RAY_Start();
	void ENSI_SA_EPR_PARTICLE_RAY_StartFuture();
	void ENSI_SA_EPR_PARTICLE_RAY_FrameMoveFuture();
	void ENSI_SA_EPR_PARTICLE_RAY_FrameMove();
	void ENSI_SA_EPR_PARTICLE_RAY_EventProcess();
	void ENSI_SA_EPR_PARTICLE_RAY_End();

	void ENSI_SA_EPR_PHOTON_FLARE_Init();
	void ENSI_SA_EPR_PHOTON_FLARE_StateStart();
	void ENSI_SA_EPR_PHOTON_FLARE_FrameMove();				
	void ENSI_SA_EPR_PHOTON_FLARE_EventProcess();				
	void ENSI_SA_EPR_PHOTON_FLARE_StateEnd();
	
	void ENSI_SA_EPR_SWEEP_ROLLING_READY_Init();
	void ENSI_SA_EPR_SWEEP_ROLLING_READY_Start();
	void ENSI_SA_EPR_SWEEP_ROLLING_READY_FrameMove();
	void ENSI_SA_EPR_SWEEP_ROLLING_READY_EventProcess();
	void ENSI_SA_EPR_SWEEP_ROLLING_READY_End();

	void ENSI_SA_EPR_SWEEP_ROLLING_CHARGE_Start();
	void ENSI_SA_EPR_SWEEP_ROLLING_CHARGE_FrameMove();
	void ENSI_SA_EPR_SWEEP_ROLLING_CHARGE_EventProcess();
	void ENSI_SA_EPR_SWEEP_ROLLING_CHARGE_End();

	void ENSI_SA_EPR_SWEEP_ROLLING_FIRE_Start();
	void ENSI_SA_EPR_SWEEP_ROLLING_FIRE_FrameMove();
	void ENSI_SA_EPR_SWEEP_ROLLING_FIRE_EventProcess();
	void ENSI_SA_EPR_SWEEP_ROLLING_FIRE_End();

	void ENSI_SA_EPR_SWEEP_ROLLING_END_Start();
	void ENSI_SA_EPR_SWEEP_ROLLING_END_FrameMove();
	void ENSI_SA_EPR_SWEEP_ROLLING_END_EventProcess();
	void ENSI_SA_EPR_SWEEP_ROLLING_END_End();



	void ENSI_SA_EPR_THOUSANDS_OF_STARS_Init();
	void ENSI_SA_EPR_THOUSANDS_OF_STARS_FrameMove();
	void ENSI_SA_EPR_THOUSANDS_OF_STARS_EventProcess();


	void ENSI_SA_EPR_GIGA_STREAM_Init();
	void ENSI_SA_EPR_GIGA_STREAM_Start();
	void ENSI_SA_EPR_GIGA_STREAM_StartFuture();
	void ENSI_SA_EPR_GIGA_STREAM_FrameMoveFuture();
	void ENSI_SA_EPR_GIGA_STREAM_FrameMove();
	void ENSI_SA_EPR_GIGA_STREAM_EventProcess();
	void ENSI_SA_EPR_GIGA_STREAM_End();

#ifdef SERV_EVE_BATTLE_SERAPH
	#pragma region 테이저 필라
	void ENSI_A_EBS_TASER_PILUM_Init();
	void ENSI_A_EBS_TASER_PILUM_StartFuture();
	void ENSI_A_EBS_TASER_PILUM_Start();
	void ENSI_A_EBS_TASER_PILUM_FrameMoveFuture();
	void ENSI_A_EBS_TASER_PILUM_FrameMove();
	void ENSI_A_EBS_TASER_PILUM_EventProcess();
	void ENSI_A_EBS_TASER_PILUM_EndFuture();
	#pragma endregion JHKang / 강정훈 / 2012. 7. 10

	#pragma region 부스트 대쉬 스타트
	void ENSI_EBS_BOOST_DASH_START_StartFuture();
	void ENSI_EBS_BOOST_DASH_START_EventProcess();
	void ENSI_EBS_BOOST_DASH_START_EndFuture();
	#pragma endregion JHKang / 강정훈 / 2012. 7. 21

	#pragma region 부스트 대쉬
	void ENSI_EBS_BOOST_DASH_StartFuture();
	void ENSI_EBS_BOOST_DASH_Start();
	void ENSI_EBS_BOOST_DASH_FrameMove();
	void ENSI_EBS_BOOST_DASH_FrameMoveFuture();
	void ENSI_EBS_BOOST_DASH_EventProcess();
	void ENSI_EBS_BOOST_DASH_End();
	#pragma endregion JHKang / 강정훈 / 2012. 7. 21

	#pragma region 부스트 대쉬 에어 스타트(한 바퀴 빙글 후 대쉬로)
	void ENSI_EBS_BOOST_DASH_AIR_START_StartFuture();
	void ENSI_EBS_BOOST_DASH_AIR_START_Start();
	void ENSI_EBS_BOOST_DASH_AIR_START_FrameMoveFuture();
	void ENSI_EBS_BOOST_DASH_AIR_START_EventProcess();
	void ENSI_EBS_BOOST_DASH_AIR_START_EndFuture();
	#pragma endregion JHKang / 강정훈 / 2012. 7. 23

	#pragma region 부스트 대쉬 에어
	void ENSI_EBS_BOOST_DASH_AIR_StartFuture();
	void ENSI_EBS_BOOST_DASH_AIR_Start();
	void ENSI_EBS_BOOST_DASH_AIR_FrameMove();
	void ENSI_EBS_BOOST_DASH_AIR_FrameMoveFuture();
	void ENSI_EBS_BOOST_DASH_AIR_EventProcess();
	void ENSI_EBS_BOOST_DASH_AIR_EndFuture();
	void ENSI_EBS_BOOST_DASH_AIR_End();
	#pragma endregion JHKang / 강정훈 / 2012. 7. 23

	#pragma region 대쉬 점프 콤보 X(Push)Z
	void ENSI_EBS_DASH_JUMP_COMBO_XZ_Start();
	void ENSI_EBS_DASH_JUMP_COMBO_XZ_FrameMove();
	void ENSI_EBS_DASH_JUMP_COMBO_XZ_EventProcess();
	void ENSI_EBS_DASH_JUMP_COMBO_XZ_EndFuture();
	void ENSI_EBS_DASH_JUMP_COMBO_XZ_End();
	#pragma endregion JHKang / 강정훈 / 2012. 7. 27

	#pragma region 대쉬 점프 콤보 ZXXX, (공중) 부스트 대쉬 XXX Front, Up, Down
	void ENSI_EBS_DASH_JUMP_COMBO_ZFrontX_Start();
	void ENSI_EBS_DASH_JUMP_COMBO_ZFrontX_FrameMove();
	void ENSI_EBS_DASH_JUMP_COMBO_ZFrontX_EventProcess();
	void ENSI_EBS_DASH_JUMP_COMBO_ZFrontX_EndFuture();
	void ENSI_EBS_DASH_JUMP_COMBO_ZFrontX_End();

	void ENSI_EBS_DASH_JUMP_COMBO_ZUpX_Start();
	void ENSI_EBS_DASH_JUMP_COMBO_ZUpX_FrameMove();
	void ENSI_EBS_DASH_JUMP_COMBO_ZUpX_EventProcess();
	void ENSI_EBS_DASH_JUMP_COMBO_ZUpX_EndFuture();
	void ENSI_EBS_DASH_JUMP_COMBO_ZUpX_End();

	void ENSI_EBS_DASH_JUMP_COMBO_ZDownX_Start();
	void ENSI_EBS_DASH_JUMP_COMBO_ZDownX_FrameMove();
	void ENSI_EBS_DASH_JUMP_COMBO_ZDownX_EventProcess();
	void ENSI_EBS_DASH_JUMP_COMBO_ZDownX_EndFuture();
	void ENSI_EBS_DASH_JUMP_COMBO_ZDownX_End();

	void StateChangeBoostAttackIfPossible();
	#pragma endregion JHKang / 강정훈 / 2012. 7. 30

	#pragma region 에너지 니들스 1필
	void ENSI_EBS_ENERGY_NEEDLES_Init();
	void ENSI_EBS_ENERGY_NEEDLES_Start();
	void ENSI_EBS_ENERGY_NEEDLES_FrameMove();
	void ENSI_EBS_ENERGY_NEEDLES_EventProcess();
	void ENSI_EBS_ENERGY_NEEDLES_End();
	#pragma endregion JHKang / 강정훈 / 2012. 8. 1

	#pragma region 에너제틱 하트 2필
	void ENSI_EBS_ENERGETIC_HEART_Start();
	void ENSI_EBS_ENERGETIC_HEART_FrameMove();
	void ENSI_EBS_ENERGETIC_HEART_EventProcess();
	void ENSI_EBS_ENERGETIC_HEART_End();
	#pragma endregion JHKang / 강정훈 / 2012. 8

	#pragma region 역장 변경 Start 함수 분리
	void SPECTRO_EL_CRYSTAL_Start();
	#pragma endregion			
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	#pragma region SI_SA_EN_ILLUSION_RAY
	void ENSI_A_EN_Illusion_Ray_Init();
	void ENSI_A_EN_Illusion_Ray_Start();
	void ENSI_A_EN_Illusion_Ray_StartFuture();
	void ENSI_A_EN_Illusion_Ray_FrameMoveFuture();
	void ENSI_A_EN_Illusion_Ray_FrameMove();
	void ENSI_A_EN_Illusion_Ray_EventProcess();
	void ENSI_A_EN_Illusion_Ray_End();
	#pragma endregion 일루전 레이

	#pragma region SI_A_EN_TESLA_SHOCK
	void ENSI_A_EN_Tesla_Shock_Init();
	void ENSI_A_EN_Tesla_Shock_StateStart();
	void ENSI_A_EN_Tesla_Shock_EventProcess();			
	void ENSI_A_EN_Tesla_Shock_StateEnd();
	#pragma endregion 테슬라 쇼크

	#pragma region SI_SA_EEG_HORNET_STING_SHAPED_CHARGE
	void ENSI_SA_EEG_Hornet_Sting_Shaped_Charge_Init();
	void ENSI_SA_EEG_Hornet_Sting_Shaped_Charge_FrameMove();
	void ENSI_SA_EEG_Hornet_Sting_Shaped_Charge_EventProcess();
	#pragma endregion 호넷 스팅 - 쉐이프트 차지

	#pragma region SI_SA_ENS_ATOMIC_BLASTER_SONICGUN
	void ENSI_SA_ENS_Atomic_Blaster_SonicGun_Init();
	void ENSI_SA_ENS_Atomic_Blaster_SonicGun_Start();
	void ENSI_SA_ENS_Atomic_Blaster_SonicGun_FrameMove();
	void ENSI_SA_ENS_Atomic_Blaster_SonicGun_EventProcess();
	#pragma endregion 아토믹 블래스터 - 소닉건

	#pragma region SI_SA_SA_EEL_SWEEP_ROLLING_TRI_VULCAN
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_Ready_Init();
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_Ready_Start();
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_Ready_FrameMove();
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_Ready_EventProcess();

	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_Fire_Start();
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_Fire_FrameMove();
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_Fire_EventProcess();
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_Fire_End();

	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_End_Start();
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_End_FrameMove();
	void ENSI_SA_EEL_Sweep_Rolling_Tri_Vulcan_End_EventProcess();
	#pragma endregion 스위프 롤링 - 트라이 발칸

	#pragma region SI_SA_EAT_HEAVENS_FIST_SWEEPER
	void ENSI_SA_EAT_Heavens_Fist_Sweeper_Init();
	void ENSI_SA_EAT_Heavens_Fist_Sweeper_FrameMove();
	void ENSI_SA_EAT_Heavens_Fist_Sweeper_EventProcess();
	#pragma endregion 헤븐즈 피스트 - 스위퍼

	#pragma region SI_SA_ENS_ASSAULT_SPEAR
	void ENSI_SA_ENS_Assualt_Spear_Init();
	void ENSI_SA_ENS_Assualt_Spear_FrameMove();
	void ENSI_SA_ENS_Assualt_Spear_EventProcess();
	#pragma endregion 어설트 스피어 - 네메시스

	#pragma region SI_A_EEP_SPIT_FIRE_GRENADE
	void ENSI_A_EEP_Spit_Fire_Grenade_Init();
	void ENSI_A_EEP_Spit_Fire_Grenade_FrameMove();
	void ENSI_A_EEP_Spit_Fire_Grenade_EventProcess();
	#pragma endregion 스핏 파이어 - 그레네이드

	#pragma region SI_A_EBS_KUGELBLITZ
	void ENSI_A_EBS_Kugelblitz_StartFuture();
	void ENSI_A_EBS_Kugelblitz_Start();
	void ENSI_A_EBS_Kugelblitz_FrameMoveFuture();
	void ENSI_A_EBS_Kugelblitz_FrameMove();
	void ENSI_A_EBS_Kugelblitz_EventProcess();
	void ENSI_A_EBS_Kugelblitz_EndFuture();
	#pragma endregion 쿠클블릿츠

	#pragma region SI_SA_EEL_LINEAR_DIVIDER
	void ENSI_SA_EEL_LINEAR_DIVIDER_Init();
	void ENSI_SA_EEL_LINEAR_DIVIDER_Start();
	void ENSI_SA_EEL_LINEAR_DIVIDER_StartFuture();
	void ENSI_SA_EEL_LINEAR_DIVIDER_FrameMoveFuture();
	void ENSI_SA_EEL_LINEAR_DIVIDER_FrameMove();
	void ENSI_SA_EEL_LINEAR_DIVIDER_EventProcess();
	void ENSI_SA_EEL_LINEAR_DIVIDER_End();
	#pragma endregion 드레드 쇼크
			
#endif //UPGRADE_SKILL_SYSTEM_2013

	// ACTIVE
	// ENSI_SA_EPR_SWEEP_ROLLING - X
	// ENSI_SA_EPR_GIGA_STREAM

#endif EVE_ELECTRA

#ifdef FINALITY_SKILL_SYSTEM //김창한
	//엠프레스, 링크 오버차지 일루전
	void ENSI_HA_EEP_LINK_OVERCHARGE_ILLUSION_Init();
	void ENSI_HA_EEP_LINK_OVERCHARGE_ILLUSION_Start();
	void ENSI_HA_EEP_LINK_OVERCHARGE_ILLUSION_FrameMove();
	void ENSI_HA_EEP_LINK_OVERCHARGE_ILLUSION_EventProcess();
	void ENSI_HA_EEP_LINK_OVERCHARGE_ILLUSION_End();


	//네메시스, 루나틱 스커드
	void ENSI_HA_ENS_LUNATIC_SCUD_Init();
	void ENSI_HA_ENS_LUNATIC_SCUD_EventProcess();

#endif //FINALITY_SKILL_SYSTEM

#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
	// 배틀 세라프, 사이킥 아틸러리 
	void ENSI_HA_EBS_PSYCHIC_ARTILLERY_Init();
	void ENSI_HA_EBS_PSYCHIC_ARTILLERY_FrameMove();
	void ENSI_HA_EBS_PSYCHIC_ARTILLERY_EventProcess();
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템	

#ifdef ADD_FULL_CHARGING_VOICE

	#pragma region 공격 스테이트가 아닐 때 사운드 파일 출력하는 함수
		void PlaySoundAtNoAttackState( const wstring& wstrSoundFileName_ );
	#pragma endregion 

#endif ADD_FULL_CHARGING_VOICE
};
