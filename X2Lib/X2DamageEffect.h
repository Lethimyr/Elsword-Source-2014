#pragma once
#include "./X2SoundCloseManager.h"

#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
class CX2DamageEffectBasic
{
	public:
		CX2DamageEffectBasic(){}
		virtual	~CX2DamageEffectBasic(){}
};
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

class CX2DamageEffect
{
	public:
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        typedef CX2DamageEffect_CEffectHandle   CEffectHandle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		enum EFFECT_TYPE
		{
			DEET_NONE = 0,
			DEET_BASIC,
		};

		enum POSITION_TYPE
		{
			DEPT_BONE = 0,
			DEPT_UNIT_BONE,
			DEPT_IMPACT_POINT,
			DEPT_POS,
//#ifdef RAVEN_WEAPON_TAKER
			DEPT_DIE_POS,
//#endif RAVEN_WEAPON_TAKER
		};

		enum UNIT_SELECT
		{
			DEUS_ALL = 0,
			DEUS_FOCUS,
			DEUS_MY,
		};


		enum LOCK_ON_TYPE
		{
			LOT_NONE = 0,
			LOT_POS_BLACK_HOLE,
			LOT_UID_BLACK_HOLE,
			LOT_POS_VECTOR,
			LOT_UID_VECTOR,
			//{{ kimhc // 2010-12-17 // 가장 가까운 유닛 선택
			LOT_NEARST_UID_VECTOR,
			//}} kimhc // 2010-12-17 // 가장 가까운 유닛 선택
		//#ifdef CHUNG_SECOND_CLASS_CHANGE
			LOT_NEARST_UID_VECTOR_IN_RANGE,
		//#endif
			LOT_UID_BONE_POS,		/// 록온된 유저의 특정 본을 록온
		};

#ifdef BALANCE_PATCH_20131107
		enum LOCK_ON_CHECK_SAVE_DIRSPEED
		{
			LOCSD_NONE = 0,				//NOT_FIND_TARGET_MOVE_STOP 옵션이 설정되지 않은 일반 LOCK_ON_DATA
			LOCSD_SAVE_DIR_SPEED,		// NOT_FIND_TARGET_MOVE_STOP 옵션이 설정됨. 현재 DIR_SPEED값을 저장하고 있음.
			LOCSD_NOT_SAVE_DIR_SPEED,	// NOT_FIND_TARGET_MOVE_STOP 옵션이 설정됨. 현재 DIR_SPEED값을 저장하고 있지 않음.
		};
#endif //BALANCE_PATCH_20131107

#ifdef SERV_RENA_NIGHT_WATCHER		// mauntain : 김태환 [2012.05.21] 레나 2차 전직 나이트 와쳐 - 공격용 DamageEffect Type 구분
		enum  DAMAGE_EFFECT_TYPE
		{
			DET_NONE = -1,								/// -1 - None
			DET_ARROW_OF_EXPLOSION,						///  0 - 폭발의 화살
			DET_ARROW_OF_EXPLOSION_DELAY,				///  1 - 폭발의 화살 ( 지연 폭파 모드 )
			DET_ARROW_OF_EXPLOSION_DELAY_GROUND,		///  2 - 땅에 적용된 폭발의 화살 ( 지연 폭파 모드 )
			DET_START_OF_DELAYED_FIRING,				///  3 - 지연의 신호탄
			DET_START_OF_DELAYED_FIRING_GROUND,			///  4 - 땅에 적용된 지연의 신호탄
			DET_EXPLOSION_TRAP,							///  5 - 폭발의 덫
			DET_EXPLOSION_TRAP_GROUND,					///  6 - 땅에 적용된 폭발의 덫
			DET_ARROW_OF_EXPLOSION_READY,				///  7 - 폭발 대기
			DET_START_OF_DELAYED_FIRING_READY,			///  8 - 폭발 대기
//#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			DET_TRAPPING_ARROW_BURSTR,					///  9 - 트래핑 애로우 버스터
			DET_TRAPPING_ARROW_BURSTR_GROUND,			/// 10 - 트래핑 애로우 버스터 땅에 적용됨
			DET_TRAPPING_ARROW_FUNGUS,					/// 11 - 트래핑 애로우 펑거스
			DET_TRAPPING_ARROW_FUNGUS_GROUND,			/// 12 - 트래핑 애로우 펑거스 땅에 적용됨
			DET_SEED_OF_RUIN,							/// 13 - 진노의 씨앗
			DET_SEED_OF_RUIN_GROUND,					//  14 - 진노의 씨앗 - 땅
//#endif //UPGRADE_SKILL_SYSTEM_2013
		};
#endif SERV_RENA_NIGHT_WATCHER

		/// 데미지 이펙터 내 파티클
		struct ParticleData
		{
			POSITION_TYPE	posType;			/// 위치 형태
			wstring			particleName;		/// 파티클 이름(wide-character)
			wstring			boneName;			/// 위치 형태가 뼈대일 경우, 뼈대 이름
			bool			bApplyRotate;		/// 대상의 회전 변환을 적용할 것인가
			bool			bForceDie;			/// 
			bool			bForceDieHit;		/// 
			int				triggerCount;		/// 
			float			triggerTime;		/// 
			float			fEventTime;			/// 발생 시간
            float           fOffsetX;			/// 오프셋 위치 X
            float           fOffsetY;			/// 오프셋 위치 Y
            float           fOffsetZ;			/// 오프셋 위치 Z
			D3DXVECTOR3		vScale;				/// 크기 비율
			bool			bTrace;				/// 따라 다닐 것인가
#ifdef EVE_ELECTRA
			bool			bTraceMore;
#endif EVE_ELECTRA
			bool            bApplyRotatedOffset; // added by lucidash
#ifdef DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME
			bool			bUseGlobalTime;
#endif DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME
			//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
			D3DXVECTOR3		vMoveAnglePerFrame;
			D3DXVECTOR3		vRotateAngleDegree;
#endif	EVE_SECOND_CLASS_CHANGE
			//}} kimhc // 2010.2.3 //	이브 2차 전직
#ifdef HIT_PARTICLE_TRACE_UNIT
			bool			bTraceDefenderUnit;
#endif HIT_PARTICLE_TRACE_UNIT
#ifdef EVE_ELECTRA
			bool			bScaleByMainEffectScaleY;
#endif EVE_ELECTRA
#ifdef BALANCE_PATCH_20120329
			float			fShowLandHeight;	// 라인맵과의 높이차가 fShowLandHeight 이하일 경우만 보이도록 한다. (DEPT_BONE에만 적용됨)
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
			bool			m_bReverseY;
#endif // ARA_CHANGE_CLASS_FIRST
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			int             m_iShowLevel;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeq;

			ParticleData()
			{
				posType			= DEPT_BONE;
				particleName	= L"";
				boneName		= L"";
				bApplyRotate	= true;
				bForceDie		= false;
				bForceDieHit	= false;
				triggerCount	= -1;
				triggerTime		= -1.0f;
				fEventTime		= -1.0f;

                fOffsetX        = 0.0f;
                fOffsetY        = 0.0f;
                fOffsetZ        = 0.0f;

				vScale			= D3DXVECTOR3( 0, 0, 0 );
				bTrace			= true;
#ifdef EVE_ELECTRA
				bTraceMore		= false;
#endif EVE_ELECTRA
				bApplyRotatedOffset	= false;
#ifdef DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME
				bUseGlobalTime	= false;
#endif DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME

				//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
				vMoveAnglePerFrame = D3DXVECTOR3( 0, 0, 0 );
				vRotateAngleDegree = D3DXVECTOR3( 0, 0, 0 );
#endif	EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.2.3 //	이브 2차 전직
#ifdef HIT_PARTICLE_TRACE_UNIT
				bTraceDefenderUnit = false;
#endif HIT_PARTICLE_TRACE_UNIT
#ifdef EVE_ELECTRA
				bScaleByMainEffectScaleY = false;
#endif EVE_ELECTRA
#ifdef BALANCE_PATCH_20120329
				fShowLandHeight = 99999.f;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
				m_bReverseY = false;
#endif // ARA_CHANGE_CLASS_FIRST
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
				m_iShowLevel = CX2GameOption::OL_LOW;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

				m_hSeq			= INVALID_PARTICLE_SEQUENCE_HANDLE;				
			}

			//{{ kimhc // 2009-10-20 // 독구름 작업 관련 추가
#ifdef	DAMAGE_EFFECT_WORK
			ParticleData( const ParticleData& t ) { *this = t; }    // copy constructor
			ParticleData& operator=( const ParticleData& t )        // assign operator
			{
				posType			= t.posType;
				particleName	= t.particleName;
				boneName		= t.boneName;
				bApplyRotate	= t.bApplyRotate;
				bForceDie		= t.bForceDie;
				bForceDieHit	= t.bForceDieHit;
				triggerCount	= t.triggerCount;
				triggerTime		= t.triggerTime;
				fEventTime		= t.fEventTime;

				fOffsetX        = t.fOffsetX;
				fOffsetY        = t.fOffsetY;
				fOffsetZ        = t.fOffsetZ;

				vScale			= t.vScale;
				bTrace			= t.bTrace;
#ifdef EVE_ELECTRA
				bTraceMore		= t.bTraceMore;
#endif EVE_ELECTRA
				
#ifdef DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME
				bUseGlobalTime	= t.bUseGlobalTime;
#endif DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME
				//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
				vMoveAnglePerFrame = t.vMoveAnglePerFrame;
				vRotateAngleDegree = t.vRotateAngleDegree;
#endif	EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.2.3 //	이브 2차 전직
#ifdef HIT_PARTICLE_TRACE_UNIT
				bTraceDefenderUnit = t.bTraceDefenderUnit;
#endif HIT_PARTICLE_TRACE_UNIT

				m_hSeq			= t.m_hSeq;

				bApplyRotatedOffset = t.bApplyRotatedOffset;
				bScaleByMainEffectScaleY = t.bScaleByMainEffectScaleY;
				fShowLandHeight	= t.fShowLandHeight;
				
#ifdef ARA_CHANGE_CLASS_FIRST
				m_bReverseY = t.m_bReverseY;
#endif // ARA_CHANGE_CLASS_FIRST				
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
				m_iShowLevel = t.m_iShowLevel;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

				return *this;
			}
#endif	DAMAGE_EFFECT_WORK
			//}} kimhc // 2009-10-20 // 독구름 작업 관련 추가

			void DestroyParticleSequence();

#ifdef BOOST_SINGLETON_POOL_TEST
			void* operator new(size_t s);
			void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST

		};


		struct DamageEffectData
		{
			POSITION_TYPE	posType;
			wstring			damageEffectName;
			wstring			boneName;
			bool			bApplyRotate;
			float			fEventTime;
			D3DXVECTOR3		vRotateAngleDegree;
            D3DXVECTOR3     vOffset;
			D3DXVECTOR3		vMainEffectScale;
			D3DXVECTOR3		vPassiveParticleScale;
			bool			bApplyRotateOrigin;
			//{{ oasis907 : 김상윤 [2010.2.10] //
			bool			bApplyRotatedOffset;
			//}}
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			bool			bApplyMatrix;
			bool			bAutoDie; // 스테이트 이동시 자동 삭제
#endif
#ifdef SERV_RENA_NIGHT_WATCHER			
			bool			bLandCrashOnly;
			bool			bLifeTimeOnly;
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_RAVEN_VETERAN_COMMANDER
			int				iGroupID;	/// 그룹 데미지 설정 추가
#endif SERV_RAVEN_VETERAN_COMMANDER
			bool			bInheritBuffFactor;		/// 버프팩터를 상속 받을 것인가?
			//bool			bForceDie;
			//bool			bForceDieHit;
			//int			triggerCount;
			//float			triggerTime;
			//CX2DamageEffect	pDamageEffect;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
			float			fLifeTime; //코드상에서 DieDamageEffect, HitDamageEffect의 lifetime을 변화시키기 위해 추가
			bool			bOnlyApplyRotateOffset; //vAxisAngleDegree 값은 Hit, Die DamageEffect를 생성하는 DamageEffect의 값을 따라가고
													// vRotateAngleDegree 값만 Hit, Die DamageEffect괄호 안에 정의된 ROTATE_ 값을 받아와서 쓰도록 만듬.
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef LOOP_CREATE_DAMAGE_EFFECT // 김태환
			float			fLoopInterval;		/// 반복 생성 간격
			UINT			uiLoopEndCount;		/// 반복 생성 종료 횟수
			float			fLoopEndTime;		/// 반복 생성 종료 시간

			UINT			uiCreateCount;		/// 해당 이펙트 생성 수
#endif //LOOP_CREATE_DAMAGE_EFFECT

			DamageEffectData()
			{
				posType					= DEPT_BONE;
				damageEffectName		= L"";
				boneName				= L"";
				bApplyRotate			= true;
				fEventTime				= -1.0f;
				vRotateAngleDegree		= D3DXVECTOR3( 0, 0, 0 );
                vOffset					= D3DXVECTOR3( 0, 0, 0 );
				vMainEffectScale		= D3DXVECTOR3( 0, 0, 0 );
				vPassiveParticleScale	= D3DXVECTOR3( 0, 0, 0 );
				bApplyRotateOrigin		= false;
				bApplyRotatedOffset		= false; // oasis907 : 김상윤 [2010.2.10] // 
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				bApplyMatrix			= false;
				bAutoDie				= false;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
				bLandCrashOnly			= false;
				bLifeTimeOnly			= false;
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_RAVEN_VETERAN_COMMANDER
				iGroupID				= -1;		/// 그룹 데미지 설정 추가
#endif SERV_RAVEN_VETERAN_COMMANDER
				bInheritBuffFactor		= false;
				//bForceDie		= false;
				//bForceDieHit	= false;
				//triggerCount	= -1;
				//triggerTime		= -1.0f;
				//pDamageEffect	= NULL;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
				fLifeTime				= -1.f;
				bOnlyApplyRotateOffset	= false;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef LOOP_CREATE_DAMAGE_EFFECT // 김태환
				fLoopInterval			= 0.f;
				uiLoopEndCount			= 1;
				fLoopEndTime			= 99999.f;

				uiCreateCount			= 0;
#endif //LOOP_CREATE_DAMAGE_EFFECT
			}

			//{{ kimhc // 2009-10-20 // 독구름 작업 관련 추가
#ifdef	DAMAGE_EFFECT_WORK
			DamageEffectData( const DamageEffectData& t ) { *this = t; }    // copy constructor
			DamageEffectData& operator=( const DamageEffectData& t )        // assign operator
			{

				posType					= t.posType;
				damageEffectName		= t.damageEffectName;
				boneName				= t.boneName;
				bApplyRotate			= t.bApplyRotate;
				fEventTime				= t.fEventTime;
				vRotateAngleDegree		= t.vRotateAngleDegree;
				vOffset					= t.vOffset;
				vMainEffectScale		= t.vMainEffectScale;
				vPassiveParticleScale	= t.vPassiveParticleScale;
				bApplyRotateOrigin		= t.bApplyRotateOrigin;
				bApplyRotatedOffset		= t.bApplyRotatedOffset; // oasis907 : 김상윤 [2010.2.10] // 
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				bApplyMatrix			= t.bApplyMatrix;
				bAutoDie				= t.bAutoDie;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
				bLandCrashOnly			= t.bLandCrashOnly;
				bLifeTimeOnly			= t.bLifeTimeOnly;
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_RAVEN_VETERAN_COMMANDER
				iGroupID				= t.iGroupID;			/// 그룹 데미지 설정 추가
#endif SERV_RAVEN_VETERAN_COMMANDER
				bInheritBuffFactor		= t.bInheritBuffFactor;

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
				fLifeTime				= t.fLifeTime;
				bOnlyApplyRotateOffset	= t.bOnlyApplyRotateOffset;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

				return *this;
			}
#endif	DAMAGE_EFFECT_WORK
			//}} kimhc // 2009-10-20 // 독구름 작업 관련 추가

#ifdef BOOST_SINGLETON_POOL_TEST
			void* operator new(size_t s);
			void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST

		};


		struct CameraData
		{
			UNIT_SELECT		unitSelect;
			CKTDGCamera::SHAKE_TYPE		crashType;
			float			fSpeed;
			float			fAccel;
			float			fGap;
			float			fTime;

			float			fEventTime;

		};

		struct ScreenData
		{
			UNIT_SELECT		unitSelect;
			float			fEventTime;
			int				clearCount;
			D3DXCOLOR		color;

			ScreenData()
			{
				unitSelect	= DEUS_ALL;
				fEventTime	= 0.0f;
				clearCount	= 1;
				color		= 0xffffffff;
			}
		};

		struct SoundData
		{
			float			fEventTime;
			wstring			fileName;
			int				iRate;
			bool			bUseGlobalTime;
			bool			bLoop;

			SoundData()
			{
				fEventTime	= 0.f;
				fileName	= L"";
				iRate		= 100;
				bUseGlobalTime = false;
				bLoop = false;
			}
			SoundData( const SoundData& t ) { *this = t; }    // copy constructor
			SoundData& operator=( const SoundData& t )        // assign operator
			{
				fEventTime = t.fEventTime;
				fileName = t.fileName;
				iRate = t.iRate;
				bUseGlobalTime = t.bUseGlobalTime;
				bLoop = t.bLoop;

				return *this;
			}
		};
 
		struct LockOnData
		{
			LOCK_ON_TYPE	m_LockOnType;
			D3DXVECTOR3		m_LockOnPos;
			bool			m_bLockOnUserUnit;
			UidType			m_LockOnUnitUID;
			int				m_LockOnNPCUID;
			float			m_fCurveSpeed;
			int				m_iRandomSeed;
//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			float			m_fInDistanceToDieEffect; /// 이거리 내에 들어오면 이펙트 삭제(Die)
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG

		//{{ kimhc // 2011.1.18 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
			float			m_fTimeUnlockFromStart;		/// 생성 후 몇초 이후부터 Lock을 수행하기 위해 사용
#endif	CHUNG_FIRST_CLASS_CHANGE
		//}} kimhc // 2011.1.18 // 청 1차 전직
#ifdef UPGRADE_RAVEN
			float			m_fUnLockTime;
#endif
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			float			m_fRange;
#endif
#ifdef BALANCE_DEADLY_CHASER_20130214
			bool			m_bIsNotTargetPreLockOnTarget; /// 이전 타겟팅 한 대상을 재 타겟 하지 않도록 하는 기능
#endif //BALANCE_DEADLY_CHASER_20130214
#ifdef FINALITY_SKILL_SYSTEM //김창한
			bool			m_bIsOnlyFront; /// 시전자 위치 기준으로 전방으로만 타겟팅 되도록 하는 기능
			bool			m_bSaveIsRight;	/// 시전한 타이밍에 시전자 방향 저장.
			int				m_iSaveLastTouchLineIndex; /// 시전한 타이밍에 시전자의 마지막 라인맵 인덱스 저장.
			D3DXVECTOR3		m_vecSaveOwnerPos; /// 시전한 타이밍에 시전자 위치 저장.
#endif //FINALITY_SKILL_SYSTEM
#ifdef LOCK_ON_USER_ONLY_ON
			bool			m_bIsOnlyTargetAttack; /// targetunit만 데미지를 입도록 하는 기능
#endif //LOCK_ON_USER_ONLY_ON
#ifdef BALANCE_PATCH_20131107
			LOCK_ON_CHECK_SAVE_DIRSPEED	m_eNotFindTargetMoveStop;	/// 타겟을 찾지 못했으면 제자리에 멈춰있도록 하는 기능.
			float			m_fSaveDirSpeed;						/// Dir Speed를 저장해둘 변수.
#endif //BALANCE_PATCH_20131107
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			wstring			m_wstrTargetBoneName;					/// 특정 본을 타겟으로 잡음
#endif //SERV_9TH_NEW_CHARACTER
			//}}
			LockOnData()
			{
				m_LockOnType		= LOT_NONE;
				m_LockOnPos			= D3DXVECTOR3(0,0,0);
				m_bLockOnUserUnit	= true;
				m_LockOnUnitUID		= -1;
				m_LockOnNPCUID		= -1;
				m_fCurveSpeed		= 0.0f;
				m_iRandomSeed		= -1;
				//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
				m_fInDistanceToDieEffect = -1.0f;	// 이거리 내에 들어오면 이펙트 삭제(Die)
#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG

				//{{ kimhc // 2011.1.18 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
				m_fTimeUnlockFromStart = -1.0f;		// 생성 후 몇초 이후부터 Lock을 수행하기 위해 사용
#endif	CHUNG_FIRST_CLASS_CHANGE
				//}} kimhc // 2011.1.18 // 청 1차 전직
#ifdef UPGRADE_RAVEN
				m_fUnLockTime = 9999.f;
#endif
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				m_fRange = -1.f;
#endif
#ifdef BALANCE_DEADLY_CHASER_20130214
				m_bIsNotTargetPreLockOnTarget = false; /// 이전 타겟팅 한 대상을 재 타겟 하지 않도록 하는 기능
#endif //BALANCE_DEADLY_CHASER_20130214
#ifdef FINALITY_SKILL_SYSTEM //김창한
				m_bIsOnlyFront = false;
				m_bSaveIsRight = false;
				m_iSaveLastTouchLineIndex = -1;
				m_vecSaveOwnerPos = D3DXVECTOR3(0,0,0);
#endif //FINALITY_SKILL_SYSTEM
#ifdef LOCK_ON_USER_ONLY_ON
				m_bIsOnlyTargetAttack = false;
#endif //LOCK_ON_USER_ONLY_ON
#ifdef BALANCE_PATCH_20131107
				m_eNotFindTargetMoveStop = LOCSD_NONE;
				m_fSaveDirSpeed = 0.f;
#endif //BALANCE_PATCH_20131107
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
				m_wstrTargetBoneName = L"";
#endif //SERV_9TH_NEW_CHARACTER
			};
		};

		struct BlackHoleData
		{
			bool	m_bEnabled;
			float	m_fDraggingSpeed;
			float	m_fDraggingRange;
			bool	m_bDragOnlyHittedUnit;	// 이 데미지 이펙트에 의해 타격을 입은 unit만 빨려들게 하기
#ifdef ARA_CHARACTER_BASE
			bool	m_bCanPassUnit;
#endif

			BlackHoleData()
			{
				m_bEnabled = false;
				m_fDraggingSpeed = 1.f;
				m_fDraggingRange = 1.f;
				m_bDragOnlyHittedUnit = false;
				m_bCanPassUnit = true;
			}
		};

// 데미지 이펙트가 사인 곡선 운동을 하게 함 : 글리터 강령술사가 사용.	
#ifdef _SIN_MOVE_TEST_
		struct SinMoveData
		{

			bool								m_bSinMove;
			float								m_fTimeMax;
			float								m_fTimeMin;
			float								m_fMoveMax;
			float								m_fMoveMin;
			float								m_fMove;
			float								m_fTime;
			float								m_fCurrentTime;

			SinMoveData()
			{				
				m_bSinMove							= false;
				m_fTimeMax					= 0.0f;
				m_fTimeMin					= 0.0f;
				m_fMoveMax					= 0.0f;
				m_fMoveMin					= 0.0f;
				m_fMove						= 0.0f;
				m_fTime						= 0.0f;
				m_fCurrentTime				= 0.0f;

			};
		};
#endif _SIN_MOVE_TEST_
		
		//{{ kimhc // 2009-10-20 // DamageEffect.lua에 있는 데이터를 저장할 수 있는 구조체
#ifdef	DAMAGE_EFFECT_WORK
		struct DamageEffectDataInLua
		{
			std::vector< DamageEffectData* >				m_vecDieDamageEffect;				// DIE_DAMAGE_EFFECT 테이블
			std::vector< CX2DamageEffect::ParticleData* >	m_vecStartParticle;
			std::vector< CX2DamageEffect::ParticleData* >	m_vecPassiveParticle;
			std::vector< CX2DamageEffect::ParticleData* >	m_vecTimeParticle;
			std::vector< CameraData >						m_vecCameraData;
			std::vector< ScreenData >						m_vecScreenData;
			std::vector< SoundData >						m_vecSoundData;
			std::vector< DamageEffectData* >				m_vecCreateDamageEffect;
			std::vector< CX2DamageEffect::ParticleData* >	m_vecHitParticle;
			std::vector< DamageEffectData* >				m_vecHitDamageEffect;
			CX2DamageManager::DamageData*					m_pDamageDataNext;

			DamageEffectDataInLua() : m_pDamageDataNext( NULL )
			{}
		};
#endif	DAMAGE_EFFECT_WORK
		//}} kimhc // 2009-10-20 // DamageEffect.lua에 있는 데이터를 저장할 수 있는 구조체

		class CEffect 
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            : public CX2DamageEffectBasic
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			public:
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				~CEffect();	
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				virtual    ~CEffect();	
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
               		
				CEffect( CX2DamageEffect* pManager, KLuaManager* pLuaManager, CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate,
							D3DXVECTOR3 pos, D3DXVECTOR3 angleDegree, D3DXVECTOR3 moveAxisDegree, float fLandPos = 0.f, float fCreationDelayTime = -1.f, 
							float fParabolicTimeToReachTarget = 0.f, float fParabolicTimeToLive = 0.f, const int iMeshPlayerIndex = 0, 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                            CEffectHandle hHandle = INVALID_DAMAGE_EFFECT_HANDLE,
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                            float fHitAddMp = -1.f
#ifdef PET_SKILL_EFFECT_LINE_MAP
							, bool bPetSkillEffect = false
#endif //PET_SKILL_EFFECT_LINE_MAP
						);

#ifdef BOOST_SINGLETON_POOL_TEST
				void* operator new(size_t s);
				void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST

				virtual void OnFrameMove( double fTime, float fElapsedTime );
				void OnFrameMove_SoundData();
				void OnFrameMove_BlackHoleData( float fElapsedTime );
				void OnFrameMove_LockOnData( float fElapsedTime_ );
				void OnFrameMove_SineMoveData( float fElapsedTime );
				void OnFrameMove_Trace();

#ifdef RAVEN_WEAPON_TAKER
				void SetLandPositionOnLineMap( const D3DXVECTOR3& vLandPositionOnLineMap ) { m_vLandPositionOnLineMap = vLandPositionOnLineMap; }
#endif RAVEN_WEAPON_TAKER

				//virtual void OnFrameRender();

				void DamageReact( CX2DamageManager::DamageData* pDamageData );
				void AttackResult();

				bool			GetLive(){ return m_bLive; }
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
                // CX2DamageEffect 내에서만 사용됨! 외부에서 사용하지 말 것
                void            _SetDead()  { m_bLive = false; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
				CX2GameUnit*	GetOwnerUnit(){ return ( null != m_optrOwnerUnit ? m_optrOwnerUnit.GetObservable() : NULL ); }
				const WCHAR*	GetName(){ return m_Name.c_str(); }
				bool			GetAttackedByUnit(){ return m_bAttackedByUnit; }


				CKTDGXMeshPlayer::CXMeshInstanceHandle GetMainEffectHandle() { return m_hMeshInstHandle; }
				CKTDGXMeshPlayer::CXMeshInstance* GetMainEffect();


				LockOnData&		GetLockOnData(){ return m_LockOnData; }
				void			SetLockOnUnitUID( UidType unitUID ){ m_LockOnData.m_LockOnUnitUID = unitUID; }
				void			SetLockOnNPCUID( int NPCUID ){ m_LockOnData.m_LockOnNPCUID = NPCUID; }
				void			SetLockOnPos( D3DXVECTOR3 vPos ) { m_LockOnData.m_LockOnPos = vPos; }
				void			SetLockOnRandomSeed( int iSeed ) { m_LockOnData.m_iRandomSeed = iSeed; }
	#ifdef BALANCE_GRAND_ARCHER_20121213
				void			SetLockOnType( LOCK_ON_TYPE eLockOnType ){ m_LockOnData.m_LockOnType = eLockOnType; }
	#endif //BALANCE_GRAND_ARCHER_20121213
				UidType			GetLockOnUnitUID () const { return m_LockOnData.m_LockOnUnitUID; }

#ifdef SERV_SKILL_NOTE
				BlackHoleData*  GetBlackHoleData() { return &m_BlackHoleData; }
#endif

				D3DXVECTOR3		GetPos() { return m_Pos; }
#ifdef EVE_ELECTRA
				D3DXVECTOR3		GetBeforePos() { return m_BeforePos; }
#endif EVE_ELECTRA

				void			SetPos( const D3DXVECTOR3& pos );
				void			SetAngleDegree( D3DXVECTOR3 angleDegree ){ m_AngleDegree = angleDegree; }
				D3DXVECTOR3		GetMoveAxisDegree() const { return m_MoveAxisDegree; }
				void			SetMoveAxisDegree( D3DXVECTOR3 moveAxisDegree ){ m_MoveAxisDegree = moveAxisDegree; }
				void			SetUseDXMatrix( D3DXMATRIX& dxMATRIX )
				{
					m_bUseDXMatrix	= true;
					m_DXMATRIX		= dxMATRIX;
				}


				float GetPowerRate() const { return m_fPowerRate; }
				void  SetPowerRate(float fPowerRate) { m_fPowerRate = fPowerRate; }

				void SetAttackTime(D3DXVECTOR2 val) { m_AttackTime = val; }
#ifdef SERV_EVE_BATTLE_SERAPH
				const D3DXVECTOR2 GetAttackTime() const { return m_AttackTime; }
#endif
				void SetLandPos(float val) { m_fLandPos = val; }
				
				bool IsCreationDelayElapsed()
				{
					if( m_timerCreationDelay.elapsed() > (float) m_fCreationDelayTime )
					{
						return true;
					}
					return false;
				}

#ifdef NEW_SKILL_TREE
				CX2DamageManager::DamageData& GetDamageData() { return m_DamageData; }
				CX2DamageManager::DamageData* GetDamageDataPtr() { return &m_DamageData; }
				void SetDamageTime( int iHitCount ) { m_DamageTime = iHitCount; }
				int GetDamageTime() { return m_DamageTime; }

#ifdef UPGRADE_RAVEN
				int GetDamageTimeNow() { return m_DamageTimeNow; }
#endif

				bool SetMainEffectScale( float fX, float fY, float fZ );
				bool SetMainEffectScale( D3DXVECTOR3 vScale );

				void SetPassiveParticleScale( float fX, float fY, float fZ );
				void SetPassiveParticleScale( D3DXVECTOR3 vScale );

				void SetHitDamageEffectScale( float fX, float fY, float fZ );
				void SetHitDamageEffectScale( D3DXVECTOR3 vScale );

				void SetDieDamageEffectScale( float fX, float fY, float fZ );
				void SetDieDamageEffectScale( D3DXVECTOR3 vScale );

				void SetScale( float fX, float fY, float fZ );
				void SetScale( D3DXVECTOR3 vScale );

				void SetPassiveparticleTriggerTimeRate( float fPassiveParticleTriggerTimeRate ){ m_fPassiveParticleTriggerTimeRate = fPassiveParticleTriggerTimeRate; }
				float GetPassiveparticleTriggerTimeRate(){ return m_fPassiveParticleTriggerTimeRate; }

				void SetReflected( bool bVal ){ m_bReflected = bVal; }
#endif NEW_SKILL_TREE




#ifdef GIANT_UNIT_GIANT_EFFECT_TEST

				void SetScaleByUnit( const D3DXVECTOR3& vScale );
				bool SetMainEffectScaleByUnit( const D3DXVECTOR3& vScale );
				//void SetPassiveParticleScaleByUnit( const D3DXVECTOR3& vScale ) { m_vPassiveParticleScaleByUnit = vScale; }
				void SetHitDamageEffectScaleByUnit( const D3DXVECTOR3& vScale ) { m_vHitDamageScaleByUnit = vScale; }
				void SetDieDamageEffectScaleByUnit( const D3DXVECTOR3& vScale ) { m_vDieDamageScaleByUnit = vScale; }

#endif GIANT_UNIT_GIANT_EFFECT_TEST
#ifdef EVE_ELECTRA
				D3DXVECTOR3 CX2DamageEffect::CEffect::GetMainEffectScaleByUnit();
#endif EVE_ELECTRA



#ifdef STOP_UNIT_STOP_EFFECT_TEST

				void SetStopTime( float fTime ) { m_StopTime.m_fTimeLeft = fTime; }
				float GetStopTime() { return m_StopTime.m_fTimeLeft; }
				
#endif STOP_UNIT_STOP_EFFECT_TEST
				
				void SetApplyScaleByUnit(bool bVal) { m_bApplyScaleByUnit = bVal; }
				bool GetApplyScaleByUnit() { return m_bApplyScaleByUnit; }

#ifdef ARRANGED_FLY_DAMAGE_TEST
				bool GetFollowLineRight() { return m_bFollowLineRight; }
#endif ARRANGED_FLY_DAMAGE_TEST
#ifdef ELSWORD_SHEATH_KNIGHT
				void SetFollowLineRight( bool bVal ){ m_bFollowLineRight = bVal; }
#endif ELSWORD_SHEATH_KNIGHT
#ifdef INHERIT_FORCEDOWNRATE
				void SetInheritForceDownRate( bool bVal ) { m_bInheritForceDownRate = bVal; }
				void SetForceDownRate( float fVal ){ m_fForceDownRate = fVal; }
#endif INHERIT_FORCEDOWNRATE

#ifdef EVE_ELECTRA
				int		GetLaserGroupID(){ return m_iLaserGroupID; }
				void	SetLaserGroupID( int _iLaserGroupID );
#endif EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				bool	GetUseCannonBall() { return m_bUseCannonBall; }
				DamageEffectDataInLua& GetDamageEffectDataInLua() { return m_DamageEffectDataInLua; }
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef FIX_EFFECT_REFLECT
				int		GetReflectCount() { return m_iReflectCount; }
				void	SetReflectCount( IN int iReflectCount_ ) { m_iReflectCount = iReflectCount_;  }
#endif

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                CEffectHandle   GetHandle() const { return m_hHandle; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef SERV_RENA_NIGHT_WATCHER
				/// 데미지 이펙트 인덱스 제어
				int		GetIndex() const { return m_iIndex; }
				void	SetIndex( int iIndex ) { m_iIndex = iIndex; }
				/// 데미지 이펙트 타입 제어
				int		GetType() const { return m_iType; }
				void	SetType( int iType ) { m_iType = iType; }
				/// 폭발의 화살 적용 순서 제어( 지연의 신호탄 )
				int		GetOrderNum() const { return m_iOrderNum; }
				void	SetOrderNum( int iOrderNum ) { m_iOrderNum = iOrderNum; }
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RENA_NIGHT_WATCHER
				/// 물리, 마법 추가 데미지 제어
				float	GetAddDieEffectDamage() const { return m_fAddDieEffectDamage; }
				void	SetAddDieEffectDamage( float fAddDieEffectDamage ) { m_fAddDieEffectDamage = fAddDieEffectDamage; }

				void	AddDieEffectDamage( CEffect* pCEffect );	/// 다이 데미지 이펙트 생성시 추가 데미지 설정 함수( 지연의 신호탄 )
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_EVE_BATTLE_SERAPH
				void		SetCheckSpectro( IN bool bValue_ ) { m_bCheckSpectro = bValue_; }
				const bool	GetCheckSpectro() const { return m_bCheckSpectro; }
#endif

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE
				bool GetIsGroupDamage() const { return m_bGroupDamage; }	/// 데미지 이펙트 그룹 데미지 설정 반환 함수
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef ADDED_RELATIONSHIP_SYSTEM
				bool GetOnlyRelationShip() const { return m_bOnlyRelationShip; }
#endif // ADDED_RELATIONSHIP_SYSTEM

				// 청 택트 궁극기 추가로 메인 이펙트의 회전값 받아오거나 설정하는 부분 추가, 김종훈
				D3DXVECTOR3 GetMainEffectAxisAngleDegree() const		  { return m_vMainEffectAxisAngleDegree; }
				void		SetMainEffectAxisAngleDegree(D3DXVECTOR3 val) { m_vMainEffectAxisAngleDegree = val; }

#ifdef FIELD_BOSS_RAID // 지정한 위치에서 가까운 적 유도 시키기
				void SetLockOnNearstTarget( IN const D3DXVECTOR3& vMyPos_, int iTeam_ , int randomOffset = 0 );
#endif // FIELD_BOSS_RAID

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				const bool SetLinkMainEffectByPos( IN const D3DXVECTOR3& vMyPos_, IN const D3DXVECTOR3& vTargetPos_, 
												   IN const float fDistance_ = 99999.f );

				/// SetLinkMainEffectByPos() 를 쓰기 위해 필요한 길이 배율 반환
				const float GetLinkEffectRate() { return m_fLinkEffectRate; }
#endif //SERV_ADD_LUNATIC_PSYKER

			protected:
				void SetStartParticle();
				void SetPassiveParticle();
				void SetTimeParticle();
				void SetSuckMagicParticle(WCHAR *particleName);
				void SetHitParticle();
				void SetHitDamageEffect();
				public:
				void SetDieDamageEffect();
				void DestroyPassiveParticle();
				protected:
				void CreateHitDamageEffect( vector<DamageEffectData*>& vecHitDamageEffect );
				void CreateDieDamageEffect( vector<DamageEffectData*>& vecDieDamageEffect );
				
				void SetCameraData();
				void SetScreenData();
				void SetSoundData();
				void SetDamageEffect();

				void FollowLine();
				void LoadLockOnData();
				void LoadBlackHoleData();

				//{{ kimhc // 2009-10-20 // DamageEffect.lua에 있는 테이블 로드(독구름)
#ifdef DAMAGE_EFFECT_WORK
				void LoadDieDamageEffect();
				void LoadStartParticle();
				void LoadPassiveParticle();
				void LoadTimeParticle();
				void LoadCameraData();
				void LoadScreenData();
				void LoadSoundData();
				void LoadDamageEffect();
				void LoadHitParticle();
				void LoadHitDamageEffect();
				
				void SetDamageDataNext( CX2DamageManager::DamageData* pDamageData );

				ParticleData*		LoadParticleTable();
				void SetParticleTable( ParticleData* pData, bool bCreate = true, float fTriggerTimeRate = -1.f );

				DamageEffectDataInLua m_DamageEffectDataInLua;	// 임시(위치 옮겨야함)
#endif DAMAGE_EFFECT_WORK
				//}} kimhc // 2009-10-20 // DamageEffect.lua에 있는 테이블 로드(독구름)

#ifdef _SIN_MOVE_TEST_
				void LoadSinMoveData();			// **
#endif _SIN_MOVE_TEST_

				void VectorLockOn( const D3DXVECTOR3& targetPos );

				
				ParticleData*		LoadParticleSuckMagic( WCHAR *particleName );

				//{{ kimhc // 2009-10-20 // 기존의 LoadParticleTable을 Load와 Set 으로 분류(일명 독구름 작업)
#ifndef DAMAGE_EFFECT_WORK
				ParticleData*		LoadParticleTable( bool bCreate = true, float fTriggerTimeRate = -1.f );
#endif DAMAGE_EFFECT_WORK
				//}} kimhc // 2009-10-20 // 기존의 LoadParticleTable을 Load와 Set 으로 분류(일명 독구름 작업)

				void SetParticleData( ParticleData* pData );

#ifdef FIX_AFTER_IMAGE
				void LoadAfterImage();
#endif

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                CEffectHandle                     m_hHandle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
								
				float								m_fElapsedTime;
				bool								m_bFirstFrame;
				bool								m_bLive;
				CX2DamageEffect*					m_pManager;
				KLuaManager*						m_pLuaManager;
				D3DXVECTOR3							m_Pos;
				D3DXVECTOR3							m_BeforePos;
				D3DXVECTOR3							m_AngleDegree;
				D3DXVECTOR3							m_MoveAxisDegree;
				float								m_fLandPos;
				bool								m_bFollowLine;
				bool								m_bFollowLineRight;
				bool								m_bFollowLineNew;		// 경사진 라인맵 따라갈 수 있게
#ifdef RAVEN_WEAPON_TAKER
				bool								m_bMustOnLine;
#endif RAVEN_WEAPON_TAKER

				//{{ kimhc // 2010.2.10 //	매 프레임 LandPosition 갱신 안되도록
#ifdef	EVE_SECOND_CLASS_CHANGE
				bool								m_bRefreshLandPos;
#endif	EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.2.10 //	매 프레임 LandPosition 갱신 안되도록

				bool								m_bUseDXMatrix;
				D3DXMATRIX							m_DXMATRIX;
				
				EFFECT_TYPE							m_EffectType;
				wstring								m_Name;
				float								m_fPowerRate;
				CX2GameUnitoPtr						m_optrOwnerUnit;



#ifdef STICKY_DAMAGE_EFFECT_TEST
				bool			m_bTraceAttackedUnit;	// 피격자에게 달라붙는다
				bool			m_bEnableAttack;		// 피격자에게 달라붙었을 때 더이상 공격을 못하게 한다
				float			m_fLifeTimeAfterAttack;	// 공격성공후에 main mesh player instance의 life time 을 조정
				CX2GameUnit*	m_pTraceUnit;			// 따라가는 Unit
				D3DXVECTOR3		m_vTracePosition;						
#endif STICKY_DAMAGE_EFFECT_TEST
				
				CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshInstHandle;

				D3DXVECTOR3							m_vMainEffectAxisAngleDegree;
				D3DXVECTOR3							m_vMainEffectPosition;

				CX2DamageManager::DamageData		m_DamageData;		
				
				D3DXVECTOR2							m_AttackTime;				
				bool								m_bGlobalTime;
				
				int 								m_DamageTime;
				int									m_DamageTimeNow;
#ifdef CHUNG_SECOND_CLASS_CHANGE
				int									m_PrevDamageTime;
#endif
				bool								m_bAttackedByUnit;
				bool								m_bAttackedByEffect;
				bool								m_bReflect;

				vector<ParticleData*>				m_StartParticleDataList;
				vector<ParticleData*>				m_PassiveParticleDataList;
				vector<ParticleData*>				m_TimeParticleDataList;

				vector<CameraData>					m_CameraDataList;
				vector<ScreenData>					m_ScreenDataList;
				vector<SoundData>					m_SoundDataList;

				//std::vector< CKTDXDeviceSound* >	m_vecPlayingLoopSound;			// play중인 loop sound를 이펙트가 사라질 때 꺼주기 위해서

				vector<DamageEffectData*>			m_DamageEffectDataList;



#ifdef CLOSE_SOUND_TEST
				CX2SoundCloseManager		m_SoundCloseManager;
#endif CLOSE_SOUND_TEST


				float								m_fDamageDataChangeTime;
				float								m_fDamageDataChangeTimeGlobalTime;
				bool								m_bClearHitUnitOnChangeDamageData;

				LockOnData							m_LockOnData;
				
#ifdef _SIN_MOVE_TEST_
				SinMoveData							m_SinMoveData;
#endif _SIN_MOVE_TEST_

				BlackHoleData						m_BlackHoleData;

				vector<DamageEffectData*>			m_vecHitDamageEffect;
				vector<DamageEffectData*>			m_vecDieDamageEffect;

				float								m_fCreationDelayTime;
				CKTDXTimer							m_timerCreationDelay;

#ifdef NEW_SKILL_TREE
				D3DXVECTOR3							m_vPassiveParticleScale;
				D3DXVECTOR3							m_vHitDamageScale;
				D3DXVECTOR3							m_vDieDamageScale;

				float								m_fPassiveParticleTriggerTimeRate;	// CreateInstance() 에서 인자로 받은 lifeTime 을 적용해주기 위해
				bool								m_bReflected;
#endif NEW_SKILL_TREE


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
				D3DXVECTOR3							m_vHitDamageScaleByUnit;
				D3DXVECTOR3							m_vDieDamageScaleByUnit;
#endif GIANT_UNIT_GIANT_EFFECT_TEST


#ifdef SKILL_CASH_10_TEST
				float								m_fReactAnimSpeed;
				bool								m_bFollowOnLine;
#endif


#ifdef STOP_UNIT_STOP_EFFECT_TEST
				TimeLimited m_StopTime;
#endif STOP_UNIT_STOP_EFFECT_TEST

				//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
				D3DXVECTOR3							m_vMoveAngle;	// 움직이는 각도 Rotate에 plus 함
#endif	EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.2.3 //	이브 2차 전직

				bool								m_bApplyScaleByUnit;
#ifdef INHERIT_FORCEDOWNRATE
				bool								m_bInheritForceDownRate;
				float								m_fForceDownRate;
#endif INHERIT_FORCEDOWNRATE

#ifdef RAVEN_WEAPON_TAKER
				D3DXVECTOR3							m_vLandPositionOnLineMap;
#endif RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
				int									m_iLaserGroupID;
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				bool								m_bUseCannonBall;
#endif

#ifdef FIX_EFFECT_REFLECT
				int		m_iReflectCount;
#endif

#ifdef FIX_AFTER_IMAGE
				CKTDGXSkinAfterImage*	m_pAfterImage;		/// 잔상
				D3DXVECTOR2				m_afterImageTime;	/// 잔상 유지 시간
				float					m_fDurationTime;	/// 잔상 유지 시간 확인 타이머

				//{{ mauntain : 김태환 [2012.05.21] 레나 2차 전직 나이트 와쳐
#ifdef SERV_RENA_NIGHT_WATCHER
				int						m_iIndex;			/// 해당 DamageEffect의 Index
				int						m_iType;			/// 해당 DamageEffect의 Type
				int						m_iOrderNum;		/// 폭발의 화살 발사 순서
				float					m_fAddDieEffectDamage;		/// 다이 데미지 이펙트 생성시 fPhysic, fMagic에 추가되는 수치
#endif SERV_RENA_NIGHT_WATCHER
				//}}

#ifdef SERV_EVE_BATTLE_SERAPH
				bool	m_bCheckSpectro;	/// 반응장에 생성된 것인지 확인
#endif

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE
				bool	m_bGroupDamage;		/// 데미지 이펙트 그룹 데미지 설정
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#endif

#ifdef ADDED_RELATIONSHIP_SYSTEM
				bool	m_bOnlyRelationShip;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
				bool	m_bShouldNotifyToOwnerUnitWhenDie;
#endif // NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				float	m_fLinkEffectRate;	/// SetLinkMainEffectByPos() 를 쓰기 위해 필요한 길이 배율
#endif //SERV_ADD_LUNATIC_PSYKER

		}; // CEffect


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

        enum    EListType
        {
            LIST_LIVE = 0,
            LIST_RESERVE = 1,
            LIST_FREE = 2,
            LIST_NUM
        };
        struct  KInstanceHandleInfo
        {
            CEffect*         m_pInstance;
            WORD             m_wStamp;
            EListType        m_eListType;
            KInstanceHandleInfo()
                : m_pInstance( NULL )
                , m_wStamp(0)
                , m_eListType(LIST_FREE)
            {
            }
        };
        typedef kog::indexed_list<KInstanceHandleInfo>          KInstanceHandleList;
        typedef KInstanceHandleList::iterator                   KInstanceHandleIterator;

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	public:
		CX2DamageEffect(void);
		~CX2DamageEffect(void);

		void OpenScriptFile( const WCHAR* pFilename );
		HRESULT OnFrameMove( double fTime, float fElapsedTime );

		CEffect* CreateInstance_LUA( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos );
		CEffect* CreateInstance_LUA2( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos, D3DXVECTOR3 vRot );
#ifdef CREATEINSTANCE_WITH_LIFETIME_IN_LUA
		CEffect* CreateInstanceWithLifeTime_LUA( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos, float fLifeTimeRate  );
#endif //#ifdef CREATEINSTANCE_WITH_LIFETIME_IN_LUA
#ifdef SERV_PET_SYSTEM
		CEffect*  CreateInstanceByPet_LUA( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos, D3DXVECTOR3 vRot );
#endif

		CEffect* CreateInstance_LUA3( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos, D3DXVECTOR3 vRot, float fReserveTime );

		CEffect* CreateInstanceParabolic_LUA( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 vPos, D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vAcceleration,
			float fTimeToReachTarget, float fTimeToLive );

		CEffect* CreateInstanceParabolic( CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate, 
			const D3DXVECTOR3& vPos, const D3DXVECTOR3& vTargetPos, const D3DXVECTOR3& vAcceleration, 
			const float fTimeToReachTarget, const float fTimeToLive, const int iMeshPlayerIndex = 0 );


		CEffect* CreateInstance( CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate,
									D3DXVECTOR3 pos, 
									D3DXVECTOR3 angleDegree,
									D3DXVECTOR3 moveAxisDegree,
									float fLandPos = 0, bool bReserve = false, 
                                    float fReserveTime = -1.f, float fForceDownValueRate = 1.f, float fLifeTimeRate = 1.f, 
									int iHitCount = -1, float fHitGap = -1.f, const int iMeshPlayerIndex = 0, float fHitAddMp = -1.f
#ifdef  EVE_ELECTRA
                                    , bool bInsertFront = false 
#ifdef  PET_SKILL_EFFECT_LINE_MAP
                                    , bool bPetSkillEffect = false
#endif  PET_SKILL_EFFECT_LINE_MAP
#endif  EVE_ELECTRA
                                    )
		{
			return CreateInstance( pGameUnit, pName, fPowerRate, pos.x, pos.y, pos.z, 
									angleDegree.x, angleDegree.y, angleDegree.z,
									moveAxisDegree.x, moveAxisDegree.y, moveAxisDegree.z, fLandPos, bReserve, fReserveTime, fForceDownValueRate, fLifeTimeRate, 
									iHitCount, fHitGap, iMeshPlayerIndex, fHitAddMp
#ifdef  EVE_ELECTRA
                                    , bInsertFront
#ifdef  PET_SKILL_EFFECT_LINE_MAP
                                    , bPetSkillEffect
#endif  PET_SKILL_EFFECT_LINE_MAP
#endif  EVE_ELECTRA
                                    );
		}

		CEffect* CreateInstance( CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate,
									float x, float y, float z, 
									float angleXDegree, float angleYDegree, float angleZDegree,
									float moveAxisXDegree, float moveAxisYDegree, float moveAxisZDegree,
									float fLandPos = 0, bool bReserve = false, 
                                    float fReserveTime = -1.f, float fForceDownValueRate = 1.f, float fLifeTimeRate = 1.f,
									int iHitCount = -1, float fHitGap = -1.f, const int iMeshPlayerIndex = 0, float fHitAddMp = -1.f
#ifdef  EVE_ELECTRA
                                    , bool bInsertFront = false 
#ifdef  PET_SKILL_EFFECT_LINE_MAP
                                    , bool bPetSkillEffect = false
#endif  PET_SKILL_EFFECT_LINE_MAP
#endif  EVE_ELECTRA
                                    );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		CEffectHandle   CreateInstanceHandle( CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate,
									D3DXVECTOR3 pos, 
									D3DXVECTOR3 angleDegree,
									D3DXVECTOR3 moveAxisDegree,
									float fLandPos = 0, bool bReserve = false, 
                                    float fReserveTime = -1.f, float fForceDownValueRate = 1.f, float fLifeTimeRate = 1.f, 
									int iHitCount = -1, float fHitGap = -1.f, const int iMeshPlayerIndex = 0, float fHitAddMp = -1.f
#ifdef  EVE_ELECTRA
                                    , bool bInsertFront = false 
#ifdef  PET_SKILL_EFFECT_LINE_MAP
                                    , bool bPetSkillEffect = false
#endif  PET_SKILL_EFFECT_LINE_MAP
#endif  EVE_ELECTRA
                                    )
		{
			CEffect* pEffect = CreateInstance( pGameUnit, pName, fPowerRate, pos, angleDegree, moveAxisDegree, 
									fLandPos, bReserve, fReserveTime, fForceDownValueRate, fLifeTimeRate, 
									iHitCount, fHitGap, iMeshPlayerIndex, fHitAddMp
#ifdef  EVE_ELECTRA
                                    , bInsertFront
#ifdef  PET_SKILL_EFFECT_LINE_MAP
                                    , bPetSkillEffect
#endif  PET_SKILL_EFFECT_LINE_MAP
#endif  EVE_ELECTRA
                                    );
            return  ( pEffect != NULL ) ? pEffect->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
		}

		CEffectHandle   CreateInstanceHandle( CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate,
									float x, float y, float z, 
									float angleXDegree, float angleYDegree, float angleZDegree,
									float moveAxisXDegree, float moveAxisYDegree, float moveAxisZDegree,
									float fLandPos = 0, bool bReserve = false, 
                                    float fReserveTime = -1.f, float fForceDownValueRate = 1.f, float fLifeTimeRate = 1.f,
									int iHitCount = -1, float fHitGap = -1.f, const int iMeshPlayerIndex = 0, float fHitAddMp = -1.f
#ifdef  EVE_ELECTRA
                                    , bool bInsertFront = false 
#ifdef  PET_SKILL_EFFECT_LINE_MAP
                                    , bool bPetSkillEffect = false
#endif  PET_SKILL_EFFECT_LINE_MAP
#endif  EVE_ELECTRA
                                    )
        {
			CEffect* pEffect = CreateInstance( pGameUnit, pName, fPowerRate, x, y, z,
									angleXDegree, angleYDegree, angleZDegree,
									moveAxisXDegree, moveAxisYDegree, moveAxisZDegree, fLandPos, bReserve, fReserveTime, fForceDownValueRate, fLifeTimeRate, 
									iHitCount, fHitGap, iMeshPlayerIndex, fHitAddMp
#ifdef  EVE_ELECTRA
                                    , bInsertFront
#ifdef  PET_SKILL_EFFECT_LINE_MAP
                                    , bPetSkillEffect
#endif  PET_SKILL_EFFECT_LINE_MAP
#endif  EVE_ELECTRA
                                    );
            return  ( pEffect != NULL ) ? pEffect->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
        }


#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


		void PostProcessCreateInstance( CEffect* pCEffect, float fForceDownValueRate, float fLifeTimeRate, int iAddHitCount, float fHitGap );
		void DestroyAllInstances();
//{{ robobeg : 2013-08-30
		//void DestroyInstance( CEffect* pInstance );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        void DestroyInstanceHandle( CEffectHandle& hInstance, bool bSilently = false );
        void DestroyInstance( CEffect*& pInstance, bool bSilently = false );
        void DestroyInstance_LUA( CEffect* pInstance );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        void DestroyInstance( CEffect* pInstance );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
//}} robobeg : 2013-08-30

#ifdef EVE_ELECTRA
		void DestroyInstanceSilently( CEffect*& pInstance );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        void DestroyInstanceHandleSilently( CEffectHandle& hInstance );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		void AddLaserGroupHitUnit( int _iLaserGroupID, CX2DamageManager::HitUnit _hitUnit );
#endif EVE_ELECTRA


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		bool IsLiveInstance( CEffect* pInstance );
        bool IsLiveInstanceHandle( CEffectHandle handle );
        CEffect*  GetInstance( CEffectHandle handle, bool bLiveOnly = true );
        CEffect*  ValidateInstanceHandle( CEffectHandle& handle, bool bLiveOnly = true )
        {
            CEffect* pEffect = GetInstance( handle, bLiveOnly );
            if ( pEffect == NULL )
                handle = INVALID_DAMAGE_EFFECT_HANDLE;
            return  pEffect;
        }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		bool IsLiveInstance( CEffect* pInstance )
		{
            if ( pInstance == NULL )
                return false;
			for( int i = 0; i < (int)m_InstanceList.size(); i++ )
			{
				CEffect* pInst = m_InstanceList[i];
				if( pInstance == pInst )
				{
					return true;
				}
			}
			return false;
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		void UnitDeleteProcess( CX2GameUnit* pGameUnit );

#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		CEffect* GetInstance( int index )
		{
			if( index < 0 || index >= (int)m_InstanceList.size() )
				return NULL;
			return m_InstanceList[index]; 
		}
		int GetInstanceNum(){ return m_InstanceList.size(); }

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


#ifdef SERV_RENA_NIGHT_WATCHER

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

        template< typename FUNCTION >
        void        ApplyFunctionToLiveInstances( FUNCTION fn );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		const vector<CEffect*>&         GetInstanceList() const { return m_InstanceList; }		/// 데미지 이펙트 벡터 반환 함수
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		CEffect*				        GetInstanceByIndex( int iIndex );				/// 해당 인덱스를 지닌 데미지 이펙트 반환 함수
#endif SERV_RENA_NIGHT_WATCHER
		
//#ifdef	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD
		//bool	AppendToDeviceList( CKTDXDeviceDataList& listInOut_, const WCHAR* pTempletName, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		//bool	AppendToDeviceList( CKTDXDeviceDataList& listInOut_, const std::wstring& wstrTempletName, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
//#endif	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD
#ifdef EFFECT_TOOL
		bool IsValidDamageEffectByName( const WCHAR* pName );
		wstring GetMainMeshNameByDamageEffectName( const WCHAR* pName );
		void GetParticleListByEffectName( IN const WCHAR* pName_, OUT vector<wstring>& vecParticleList_ );
		void GetParticleListByEffectName( IN const WCHAR* pName_, OUT vector<ParticleData*>& vecParticleList_ );
		void GetDamageEffectListByEffectName( IN const WCHAR* pName_,  OUT vector<wstring>& vecEffectList_ );
		ParticleData* LoadParticleTable();
#endif //EFFECT_TOOL


    DWORD   ComposeHandle( WORD wIndex, WORD wStamp )
    {
        return wIndex | ( wStamp << 16L );
    }
    void    DecomposeHandle( DWORD dwHandle, OUT WORD& wIndex, OUT WORD& wStamp )
    {
        wIndex = (WORD) dwHandle;
        wStamp = (WORD) ( ( ( dwHandle ) & 0xffff0000 ) >> 16L );
    }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    bool                    IsInCriticalLoop() const { return m_bInCriticalLoop; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

	private:
		float				m_fElapsedTime;
		KLuaManager			m_LuaManager;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

        KInstanceHandleList             m_coInstanceHandleList;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        bool                            m_bInCriticalLoop;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		vector<CEffect*>	m_InstanceList;
		vector<CEffect*>	m_ReserveInstanceList;

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		//int					m_iCurrUniqueIndex;
};



#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
template< typename FUNCTION >
void        CX2DamageEffect::ApplyFunctionToLiveInstances( FUNCTION fn )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    bool bOldInCriticalLoop = m_bInCriticalLoop;
    m_bInCriticalLoop = true;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

    KInstanceHandleIterator iterEnd = m_coInstanceHandleList.end( LIST_LIVE );
    for( KInstanceHandleIterator iter = m_coInstanceHandleList.begin( LIST_LIVE );
        iter != iterEnd;
        ++iter )
    {
        KInstanceHandleInfo& info = *iter;
        CEffect* pEffect = info.m_pInstance;
		if( NULL == pEffect
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            || pEffect->GetLive() == false
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            )
		   continue;
        fn( *pEffect );
    }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    m_bInCriticalLoop = bOldInCriticalLoop;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
