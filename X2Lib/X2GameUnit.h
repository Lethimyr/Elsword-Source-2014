#pragma once

#include "./X2SoundCloseManager.h"
#include "./X2RidingPet.h"

#define LINE_RADIUS 30.0f
#define EVENT_FLAG_LIST_NUM 10
#ifdef SERV_RAVEN_VETERAN_COMMANDER
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 35	/// 레이븐 2차 전직 베테랑 커맨더 - 치명상
#else
#ifdef SERV_RENA_NIGHT_WATCHER
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 34	/// 레나 2차 전직 나이트 와쳐 - 폭발의 화살
#else
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 33
#else
#ifdef EVE_ELECTRA
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 32
#else
#ifdef RAVEN_WEAPON_TAKER
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 31
#else RAVEN_WEAPON_TAKER
	//{{ kimhc // 2011-03-31 // 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	#ifdef CHUNG_SECOND_CLASS_CHANGE
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
		#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 30
#else
		#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 29
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE
	#else
		#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 28
	#endif
#else	CASH_SKILL_FOR_CHUNG_2011_04
	#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 26
#endif	CASH_SKILL_FOR_CHUNG_2011_04
	//}} kimhc // 2011-03-31 // 2011-04 에 패치될 청 캐시 스킬
#endif RAVEN_WEAPON_TAKER
#endif EVE_ELECTRA
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN
#endif SERV_RENA_NIGHT_WATCHER
#endif SERV_RAVEN_VETERAN_COMMANDER
#define SET_CB_FUNC(classname,funcname) GUStateCBFunc::from_method<classname, &classname::funcname>(this)

typedef	srutil::delegate0<void>	DelegateProcess;
#define SET_DELEGATE_PROCESS(classname,funcname) DelegateProcess::from_method<classname, &classname::funcname>(this)

typedef	srutil::delegate1<void, const CX2DamageManager::DamageData&>	DelegateProcessWithDamageData;
#define SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(classname,funcname) DelegateProcessWithDamageData::from_method<classname, &classname::funcname>(this)

#define ERASE_BUFF_IDENTITY( vecBuffIdentity_, BuffIdentity_ ) \
{ \
	vector<CX2BuffIdentityPtr>::iterator vItr \
		= FindSameBuffIdentity<vector<CX2BuffIdentityPtr>::iterator, CX2BuffIdentityPtr>( vecBuffIdentity_, BuffIdentity_ ); \
	if ( vecBuffIdentity_.end() != vItr ) \
		vecBuffIdentity_.erase( vItr ); \
} \

class CX2GameUnit;
typedef KObserverPtr<CX2GameUnit>   CX2GameUnitoPtr;

class CX2UnitSlashTraceManager;
class CX2Game;

// CX2GameUnit 의 첫번째 parent class 는 반드시 CKTDGObject 로 할 것!
// lua_tinker 의 오류로 다중 클래스 상속시 첫번째가 아닌 parent class 에 대한 상속을 시도할 경우 크래쉬 발생
class CX2GameUnit : public CKTDGObject, public CKTDXDeviceHolder, public KObservable<CX2GameUnit>
{
	friend CX2OldGageManager;
	friend CX2EnchantItem;

	// 공용 열거체, 구조체
	public:

		typedef CKTDGParticleSystem::CParticleEventSequenceHandle ParticleEventSequenceHandle;

		enum GAME_UNIT_TYPE
		{
			GUT_USER	= 0,
			GUT_NPC,			
			//GUT_TRANSFORMER, 
		};

		enum GAME_UNIT_STATE_ID
		{
			GUSI_NONE = 0,
			GUSI_LOADING,
			GUSI_READY,
			GUSI_PLAY,
			GUSI_DIE,
			GUSI_END,
		};


		enum ENABLE_DASH_STATE
		{
			EDS_LEG_WOUND,
			EDS_MEDITATION,
			EDS_END,
		};

#ifdef SERV_SKILL_NOTE
		enum SPEED_FACTOR_ID
		{
			SFI_NONE		= 0,
			SFI_SPEED_1,	/// 바인딩 써클 속도감소 디버프
			SFI_SPEED_2,	/// 엘더 무기 속도 증가 버프
			SFI_SPEED_3,	/// 펫 속도증가 버프
			SFI_SPEED_4,	/// 메티테이션 메모 이속 증가 버프
#ifdef NEW_SKILL_2010_11
			SFI_SPEED_5,	/// 윈드 스니커 - 아이레린나, 모든 속도 증가 지역 버프
#endif NEW_SKILL_2010_11
#ifdef WIDE_BUFF_ANI_SPEED_UP
			SFI_SPEED_6,	/// 몬스터 트릭스터 토템 애니메이션 속도 증가 버프
#endif
#ifdef NEW_HENIR_TEST
			SFI_SPEED_7,	/// 헤니르 바람의 샘 - 모든 속도 증가 버프 
#endif NEW_HENIR_TEST
#ifdef ELSWORD_WAY_OF_SWORD
			SFI_SPEED_8,	/// 엘소드 활력의 검사 - 공격 속도 증가 버프 
#endif ELSWORD_WAY_OF_SWORD
#ifdef UPGRADE_RAVEN
			SFI_SPEED_9,	// 레이븐 개편 투기
#endif
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			SFI_SPEED_10,	// 스피드 엑셀레이터
#endif
#ifdef SPECIAL_USE_ITEM
			SFI_SPEED_11,	// 던전 사용 아이템, 바람 구슬
#endif
#ifdef BALANCE_PATCH_20120329
			SFI_SPEED_13,	// 이브 클로킹 디버프
#endif
		};

		enum SPEED_FACTOR_SET_TYPE		// FACTOR요소를 Set 할 때 factor를 vector에 담는 경우
		{
			// 스피드 팩터 구조체에 가지고 있지 않고 Set 함수를 호출할 때만 전달인자로 넘겨주는 용도로 사용

			SFST_MULTY,					// vector내에 같은 아이디 요소가 여러개 있어도 되는 타입
			SFST_ONLY,					// vector내에 같은 아이디 요소가 있는 경우 새로 set 하는 factor를 무시하는 타입
			SFST_UPDATE,				// 기존에 vector 내에 있는 요소를 갱신 시켜주는 타입 (만일 이 타입으로 Set 실행 시에 vector내에 해당 요소 가 없으면 추가 함)
		};
#endif

#ifdef CHUNG_SECOND_CLASS_CHANGE
		enum PULSE_WEAPON_COLOR_ID
		{
			PWCI_NONE		= 0,
			PWCI_UNDEFINED,
			PWCI_CHUNG_LAND_DEMOLISHER,
		};
#endif CHUNG_SECOND_CLASS_CHANGE

//-------------------------------------------------------------------------------------------------------
		struct  InitDeviceData
		{
			std::vector<std::wstring>    m_vecstrReadyTexture;
			std::vector<std::wstring>    m_vecstrReadyXMesh;
			std::vector<std::wstring>    m_vecstrReadyXSkinMesh;
			std::vector<std::wstring>    m_vecstrReadySound;
			std::vector<std::wstring>	 m_vecstrReadyXET;
			std::vector<std::wstring>	 m_vecstrReadyGameMajorParticle;
			std::vector<std::wstring>	 m_vecstrReadyGameMinorParticle;

			void    AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;

		};//struct  InitDeviceData

		struct InputData
		{
			bool pureLeft;				/// 왼쪽 방향 화살표를 한번 계속 누르고 있는 상태
			bool oneLeft;				/// 왼쪽 방향 화살표를 한번 누르고 뗀 상태
			bool pureDoubleLeft;		/// 왼쪽 방향 화살표를 두번 누르고, 계속 누르고 있는 상태
			bool oneDoubleLeft;			/// 왼쪽 방향 화살표를 두번 누르고 뗀 상태

			bool pureRight;
			bool oneRight;
			bool pureDoubleRight;
			bool oneDoubleRight;

			bool pureUp;
			bool oneUp;
			bool pureDoubleUp;
			bool oneDoubleUp;

			bool pureDown;
			bool oneDown;
			bool pureDoubleDown;
			bool oneDoubleDown;

			bool pureZ;
			bool oneZ;
			bool pureDoubleZ;
			bool oneDoubleZ;

			bool pureX;
			bool oneX;
			bool pureDoubleX;
			bool oneDoubleX;

			bool pureC;
			bool oneC;
			bool pureDoubleC;
			bool oneDoubleC;

			bool pureA;
			bool oneA;
			bool pureDoubleA;
			bool oneDoubleA;

			bool pureS;
			bool oneS;
			bool pureDoubleS;
			bool oneDoubleS;

			bool pureD;
			bool oneD;
			bool pureDoubleD;
			bool oneDoubleD;

			bool pureF;
			bool oneF;
			bool pureDoubleF;
			bool oneDoubleF;

//{{오현빈 // 2012-04-30 // 스킬슬롯 체인지 없이 확장 스킬 사용 할 수 있도록 수정
			bool pureQ;
			bool oneQ;
			bool pureDoubleQ;
			bool oneDoubleQ;

			bool pureW;
			bool oneW;
			bool pureDoubleW;
			bool oneDoubleW;

			bool pureE;
			bool oneE;
			bool pureDoubleE;
			bool oneDoubleE;

			bool pureR;
			bool oneR;
			bool pureDoubleR;
			bool oneDoubleR;
//}}오현빈 // 2012-04-30 // 스킬슬롯 체인지 없이 확장 스킬 사용 할 수 있도록 수정

			bool pureLShift;
			bool oneLShift;
			bool pureDoubleLShift;
			bool oneDoubleLShift;

			bool pureLAlt;
			bool oneLAlt;
			bool pureDoubleLAlt;
			bool oneDoubleLAlt;

			bool pureSpace;
			bool oneSpace;
			bool pureDoubleSpace;
			bool oneDoubleSpace;

			bool pureLCtr;
			bool oneLCtr;
			bool pureDoubleLCtr;
			bool oneDoubleLCtr;

			InputData()
			{
				Init();
			}

			void Init()
			{
				pureLeft			= false;
				oneLeft				= false;
				pureDoubleLeft		= false;
				oneDoubleLeft		= false;

				pureRight			= false;
				oneRight			= false;
				pureDoubleRight		= false;
				oneDoubleRight		= false;

				pureUp				= false;
				oneUp				= false;
				pureDoubleUp		= false;
				oneDoubleUp			= false;

				pureDown			= false;
				oneDown				= false;
				pureDoubleDown		= false;
				oneDoubleDown		= false;

				pureZ				= false;
				oneZ				= false;
				pureDoubleZ			= false;
				oneDoubleZ			= false;

				pureX				= false;
				oneX				= false;
				pureDoubleX			= false;
				oneDoubleX			= false;

				pureC				= false;
				oneC				= false;
				pureDoubleC			= false;
				oneDoubleC			= false;

				pureA				= false;
				oneA				= false;
				pureDoubleA			= false;
				oneDoubleA			= false;

				pureS				= false;
				oneS				= false;
				pureDoubleS			= false;
				oneDoubleS			= false;

				pureD				= false;
				oneD				= false;
				pureDoubleD			= false;
				oneDoubleD			= false;


				pureF				= false;
				oneF				= false;
				pureDoubleF			= false;
				oneDoubleF			= false;

				//{{오현빈 // 2012-04-30 // 스킬슬롯 체인지 없이 확장 스킬 사용 할 수 있도록 수정
				pureQ				= false;
				oneQ				= false;
				pureDoubleQ			= false;
				oneDoubleQ			= false;

				pureW				= false;
				oneW				= false;
				pureDoubleW			= false;
				oneDoubleW			= false;

				pureE				= false;
				oneE				= false;
				pureDoubleE			= false;
				oneDoubleE			= false;

				pureR				= false;
				oneR				= false;
				pureDoubleR			= false;
				oneDoubleR			= false;
				//}}오현빈 // 2012-0= false;4-30 // 스킬슬롯 체인지 없이 확장 스킬 사용 할 수 있도록 수정


				pureLShift			= false;
				oneLShift			= false;
				pureDoubleLShift	= false;
				oneDoubleLShift		= false;

				pureLAlt			= false;
				oneLAlt				= false;
				pureDoubleLAlt		= false;
				oneDoubleLAlt		= false;

				pureSpace			= false;
				oneSpace			= false;
				pureDoubleSpace		= false;
				oneDoubleSpace		= false;

				pureLCtr			= false;
				oneLCtr				= false;
				pureDoubleLCtr		= false;
				oneDoubleLCtr		= false;
			}



			void Init_ZXArrowOnly()
			{
				pureLeft			= false;
				oneLeft				= false;
				pureDoubleLeft		= false;
				oneDoubleLeft		= false;

				pureRight			= false;
				oneRight			= false;
				pureDoubleRight		= false;
				oneDoubleRight		= false;

				pureUp				= false;
				oneUp				= false;
				pureDoubleUp		= false;
				oneDoubleUp			= false;

				pureDown			= false;
				oneDown				= false;
				pureDoubleDown		= false;
				oneDoubleDown		= false;

				pureZ				= false;
				oneZ				= false;
				pureDoubleZ			= false;
				oneDoubleZ			= false;

				pureX				= false;
				oneX				= false;
				pureDoubleX			= false;
				oneDoubleX			= false;


				//pureLShift			= false;
				//oneLShift			= false;
				//pureDoubleLShift	= false;
				//oneDoubleLShift		= false;

				//pureLAlt			= false;
				//oneLAlt				= false;
				//pureDoubleLAlt		= false;
				//oneDoubleLAlt		= false;

				//pureSpace			= false;
				//oneSpace			= false;
				//pureDoubleSpace		= false;
				//oneDoubleSpace		= false;

				//pureLCtr			= false;
				//oneLCtr				= false;
				//pureDoubleLCtr		= false;
				//oneDoubleLCtr		= false;

			}


			bool IsKeyPressed()
			{
				bool bPressed = false;

				UINT nSize = sizeof(InputData) / sizeof(bool);
				bool* pKey = (bool*)this;
				for( UINT i=0; i<nSize; i++ )
				{
					if( true == pKey[i] )
					{
						bPressed = true;
					}
				}
				return bPressed;
			}
		};

//-------------------------------------------------------------------------------------------------------
		
		/** @class : TIME_SPEED
			@brief : 유저의 이동속도, m_PhysicParam.nowSpeed에 일정 수치를 더하거나 대입하기 위한 구조체
			@date  : 2010/11/11
		*/
		struct TIME_SPEED
		{
			D3DXVECTOR3 vSpeed;		/// x는 X축, y는 Y축, z는 스피드가 적용될 애니메이션 타임 시점
			int iFlag;				/// 1이면 m_PhysicParam.nowSpeed 의 x, y에 += 연산을\n
									///	2이면 = 연산을 수행하게하는 플래그

			TIME_SPEED() :
			vSpeed( D3DXVECTOR3(0.f, 0.f, 0.f) ),
			iFlag( 0 )
			{
			}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            bool    operator < ( const TIME_SPEED& In_ ) const { return vSpeed.z < In_.vSpeed.z; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		};

//-------------------------------------------------------------------------------------------------------

#ifdef RAVEN_SECOND_CLASS_CHANGE
		struct TIME_PUSHPASS
		{
			float fTime;
			bool bCan;

			TIME_PUSHPASS() :
			fTime(0.f),
			bCan(false)
			{
			}
		};
#endif

//-------------------------------------------------------------------------------------------------------

		// m_fDuration 시간 동안 걷기, 달리기 속도에 m_fSpeedRate만큼 적용
		struct SPEED_FACTOR
		{
#ifdef SERV_SKILL_NOTE
			SPEED_FACTOR_ID	m_eId;
#endif
			float	m_fDuration;
			float	m_fSpeedRate;

//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
#ifdef SERV_SECRET_HELL
			CX2EffectSet::Handle	m_hEffectSet;
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)

			SPEED_FACTOR() :
#ifdef SERV_SKILL_NOTE
			m_eId( SFI_NONE ),
#endif
			m_fDuration( 0.f ),
			m_fSpeedRate( 0.f )

			//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
#ifdef SERV_SECRET_HELL
			, m_hEffectSet( INVALID_EFFECTSET_HANDLE )
#endif SERV_SECRET_HELL
			//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
			{
			}
		};

//-------------------------------------------------------------------------------------------------------

#ifdef DUNGEON_ITEM
		struct BUFFER_FACTOR
		{
			float m_fBufferRate;
			float m_fBufferTime;

			BUFFER_FACTOR() :
			m_fBufferRate( 1.f ),
			m_fBufferTime( 0.f )
			{
			}
		};
#endif

//-------------------------------------------------------------------------------------------------------

#ifdef GRAPPLING_TEST
		struct GrapplingState
		{
			GAME_UNIT_TYPE m_eUnitTypeGrapplingMe;	 
			UidType m_UnitUIDGrapplingMe;					// 나를 잡은 유닛
			GAME_UNIT_TYPE m_eUnitTypeGrappledByMe;	
			UidType m_UnitUIDGrappledByMe;					// 내가 잡은 유닛

			D3DXVECTOR3 m_vGrapplingPosition;				// 내가 잡았을때 잡은 나의 현재 손위치
			D3DXVECTOR3 m_vGrapplingRotation;
			CKTDXDeviceXSkinMesh::MultiAnimFrame* m_pFrameGrappling;

			D3DXVECTOR3 m_vGrappledPosition;				// 내가 잡혔을 떄 잡힌 나의 현재 멱살위치
			D3DXVECTOR3 m_vGrappledRotation;				
			CKTDXDeviceXSkinMesh::MultiAnimFrame* m_pFrameGrappled;


			GrapplingState()
			: m_eUnitTypeGrapplingMe( GUT_USER )
			, m_UnitUIDGrapplingMe( -1 )
			, m_eUnitTypeGrappledByMe( GUT_USER )
			, m_UnitUIDGrappledByMe( -1 )
			, m_vGrapplingPosition( 0, 0, 0 )
			, m_vGrapplingRotation( 0, 0, 0 )
			, m_pFrameGrappling( NULL )
			, m_vGrappledPosition( 0, 0, 0 )
			, m_vGrappledRotation( 0, 0, 0 ) 
			, m_pFrameGrappled( NULL )
			{
			}


			void ResetGrapplingState( bool OnlyMe = false );
			void SetUnitGrapplingMe( GAME_UNIT_TYPE eGameUnitType, UidType unitUID );
			CX2GameUnit* GetUnitGrapplingMe();
			void SetUnitGrappledByMe( GAME_UNIT_TYPE eGameUnitType, UidType unitUID );
			CX2GameUnit* GetUnitGrappledByMe();

			void CalcGrapplingPosition();
			const D3DXVECTOR3& GetGrapplingPosition() const { return m_vGrapplingPosition; }
			const D3DXVECTOR3& GetGrapplingRotation() const { return m_vGrapplingRotation; }

			void CalcGrappledPosition();
			const D3DXVECTOR3& GetGrappledPosition() const { return m_vGrappledPosition; }
			const D3DXVECTOR3& GetGrappledRotation() const { return m_vGrappledRotation; }

		};
#endif GRAPPLING_TEST

//-------------------------------------------------------------------------------------------------------

		// note!! 이 구조체 사이즈 커지면 필요한 것만 맵에 저장하는 형식으로 바꾸기
		struct ExtraDamagePack
		{
			CX2DamageManager::ExtraDamageData	m_Poison;
			CX2DamageManager::ExtraDamageData	m_Fire;
			CX2DamageManager::ExtraDamageData	m_Curse;
			CX2DamageManager::ExtraDamageData	m_Slow;
			CX2DamageManager::ExtraDamageData	m_Lightning;
			CX2DamageManager::ExtraDamageData	m_ReverseLeftRight;
			CX2DamageManager::ExtraDamageData	m_Cold;
			CX2DamageManager::ExtraDamageData	m_Y_Pressed;	// 삭제

#ifdef SKILL_30_TEST
			CX2DamageManager::ExtraDamageData	m_Frozen;				/// 얼음되서 멈추는 것
			CX2DamageManager::ExtraDamageData	m_LegWound;				/// 하체 상처
			CX2DamageManager::ExtraDamageData	m_WeaponBreak;			/// 무기 파괴
			CX2DamageManager::ExtraDamageData	m_ArmorBreak;			/// 방어구 파괴
			CX2DamageManager::ExtraDamageData	m_Entangled;			/// 레나 인탱글 필살기. 붙들고 못 움직이게
			CX2DamageManager::ExtraDamageData	m_Dull;					/// 레나 로우킥. 움직임 둔화
	
			CX2DamageManager::ExtraDamageData	m_TimeBomb;				/// 아이샤 데스필드 시한폭탄
			CX2DamageManager::ExtraDamageData	m_Stun;					/// 레나 샤프폴 stun
#endif SKILL_30_TEST

			CX2DamageManager::ExtraDamageData	m_Aging;				/// 노화


			CX2DamageManager::ExtraDamageData	m_EnchantBlaze;
			CX2DamageManager::ExtraDamageData	m_EnchantFrozen;
			CX2DamageManager::ExtraDamageData	m_EnchantPoison;
			CX2DamageManager::ExtraDamageData	m_EnchantShock;


			CX2DamageManager::ExtraDamageData	m_MagicDefenceDebuff;
			CX2DamageManager::ExtraDamageData	m_StigmaDebuff;
			
			CX2DamageManager::ExtraDamageData	m_Dryad;
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
			CX2DamageManager::ExtraDamageData	m_Blind;;
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
			CX2DamageManager::ExtraDamageData	m_WaterHold;
#endif EDT_WATER_HOLD_TEST

#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
			CX2DamageManager::ExtraDamageData	m_Panic;	/// 경과시간에 따라 키보드 컨트롤이 좌우 반전되는 익스트라 데미지
			CX2DamageManager::ExtraDamageData	m_Pain;		/// 상대방에게 걸려있는 ExtraDamage에 따라 폭발하는 횟수가 달라지는 ExtraDamage
#endif	CASH_SKILL_FOR_CHUNG_2011_04

#ifdef CHUNG_SECOND_CLASS_CHANGE
			CX2DamageManager::ExtraDamageData	m_LegShot;	/// 이동, 점프 속도를 감소시킨다.
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
			CX2DamageManager::ExtraDamageData	m_EarthQuake;/// 이동, 애니메이션 속도를 감소시킨다.
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef RAVEN_WEAPON_TAKER
			CX2DamageManager::ExtraDamageData	m_SmokeBlind;/// 회피감소
#endif RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
			CX2DamageManager::ExtraDamageData	m_FlashBang;
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			CX2DamageManager::ExtraDamageData	m_MarkOfCommander;
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

			// kimhc // 2010-04-05 // map으로 바꾸려 했으나 여러작업이 서로 겹쳐 있어 다음에 바꿔야 할듯
			// 현재 이 구조체 안의 ExtraDamageData는 30개로 이곳에 변수를 추가하면
			// #define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 의 수를 변경 시켜줘야함

#ifdef VELDER_SECRET_DUNGEON
			// 비던용으로 제작하므로 일단 NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 에 추가하지 않는다.
			// 범용적인 EDT로 추후 재 사용시 추가해야함
			CX2DamageManager::ExtraDamageData	m_ScaleCriticalBuff;
			CX2DamageManager::ExtraDamageData	m_BerserkerBuff;
			CX2DamageManager::ExtraDamageData	m_ManaBuff;
			CX2DamageManager::ExtraDamageData	m_AttackAllTeam;
#endif
#ifdef UPGRADE_RAVEN
			CX2DamageManager::ExtraDamageData	m_RunJumpSpeed;
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
			CX2DamageManager::ExtraDamageData	m_StartOfDelayedFiring;		/// 레나 2차 전직 나이트 와쳐 - 지연의 신호탄
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
			CX2DamageManager::ExtraDamageData	m_Bleeding;					/// 레이븐 2차 전직 베테랑 커맨더 - 치명상
#endif SERV_RAVEN_VETERAN_COMMANDER

			void Init();
#ifdef CHUNG_SECOND_CLASS_CHANGE
			void Init(CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageType);
#endif CHUNG_SECOND_CLASS_CHANGE
			bool Verify();
		};

//-------------------------------------------------------------------------------------------------------

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        struct  KAttackTimeProjSeqName
        {
            D3DXVECTOR2 m_v2AttackTime;
            wstring     m_wstrProjSeqName;

            KAttackTimeProjSeqName()
            : m_v2AttackTime( 0.f, 0.f ) {}
        };//struct  KAttackTimeProjSeqName

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		/// 유닛 상태 파라미터
		struct StateParam
		{
			bool					bLandConnect;	/// 땅에 닿아 있어야 하는 스테이트
			KProtectedType<bool>	bCanPushUnit;		
			KProtectedType<bool>	bCanPassUnit;		/// 다른 유닛들을 통과하거나, 다른 유닛들에 의해 통과 될 수 있음
			PROTECT_VECTOR2			invincibleTime;
			KProtectedType<bool>	bInvincible;
			KProtectedType<bool>	bSuperArmor;
			KProtectedType<float>	fRevengeStartTime;
			KProtectedType<float>	fRevengeEndTime;
			int						normalCamera;
			int						hitCamera;
			bool					bResetCamera;
			bool					bEventFlagList[EVENT_FLAG_LIST_NUM];
            bool                    bSuperArmorNotRed;		/// 슈퍼 아머일 때 붉은색을로 나타내지 않음
            int                     fGuardDefence;			/// 방어
			bool					bAttackState;			/// 지금 상태가 공격상태인지, m_AttackTimeList가 empty가 아니거나 이 변수가 설정되어 있다면 공격상태임
#ifdef SERV_EVE_BATTLE_SERAPH
			bool					bNoChangSpectro;		/// 스킬 중에서 역장이 변하지 않아야 되는 스킬 구분
#endif
			
			KProtectedType<float>	fReflexMagicStartTime;	/// 
			KProtectedType<float>	fReflexMagicEndTime;	/// 

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SUPER_ARMOR_TIME
			std::vector<D3DXVECTOR2> m_vecSuperArmorTime;
#endif

#ifdef ARA_CHARACTER_BASE
			USHORT					m_usSoundSet;		/// 사운드 Set 번호 선택
#endif

#ifdef ADD_CAN_PASS_ALLY_UNIT	//JHKang
			KProtectedType<bool>	bCanPassAllyUnit;	/// 다른 유닛들을 통과하거나, 다른 유닛들에 의해 통과 될 수 있음, 같은 편일 경우
#endif //ADD_CAN_PASS_ALLY_UNIT
			bool					bAllowDirChange;
			PROTECT_VECTOR3			m_DefenceTime;
			D3DXVECTOR2				afterImage;
			std::map< int, D3DXVECTOR3 > m_mapAddSlashTrace;		// weapon index, slashtrace_data
			float					fSlashTraceTipWide;
			bool					bApplyAnimMove;
			bool					bFallDown;			
            bool                    bGuardFront;			/// 앞 방어
			bool					bGuardBack;				/// 뒤 방어
            bool                    bDamageDown;			/// 각 스테이트상태별로 다운상태인지를 나타냄
			bool					bNotUseLandConnect;
#ifdef SEASON3_MONSTER_2010_12
			D3DXVECTOR3				m_vReflectMagic;	// x: 발동시작시간, y:유지시간, z:확율이나, 앞뒤판단등을 위한 extra value(현재사용안함)
#endif
#ifdef SECRET_VELDER
			D3DXVECTOR2				m_vInvisibleTime;	/// 특정 상태에서 일정 시간동안 투명화 구현(섀도우 스텝 구현 관련)
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


			StateParam()
			{
				Init();
			}
			bool Verify()
			{
				if( bCanPushUnit.Verify()			== false 
					|| bCanPassUnit.Verify()		== false 
					|| invincibleTime.Verify()		== false 
					|| bInvincible.Verify()			== false
					|| bSuperArmor.Verify()			== false
					|| fRevengeStartTime.Verify()	== false
					|| fRevengeEndTime.Verify()		== false 
					|| fReflexMagicStartTime.Verify() == false
					|| fReflexMagicEndTime.Verify() == false )
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef ADD_CAN_PASS_ALLY_UNIT	//JHKang
					|| bCanPassAllyUnit.Verify()	== false
#endif //ADD_CAN_PASS_ALLY_UNIT
					|| m_DefenceTime.Verify()		== false
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					return false;
				}
				return true;
			}
			StateParam& operator=( const StateParam& data )
			{
				bLandConnect			= data.bLandConnect;
				bCanPushUnit			= data.bCanPushUnit;
				bCanPassUnit			= data.bCanPassUnit;
				invincibleTime.m_fX		= data.invincibleTime.m_fX;
				invincibleTime.m_fY		= data.invincibleTime.m_fY;
				bInvincible				= data.bInvincible;
				bSuperArmor				= data.bSuperArmor;
				fRevengeStartTime		= data.fRevengeStartTime;
				fRevengeEndTime			= data.fRevengeEndTime;
				normalCamera			= data.normalCamera;
				hitCamera				= data.hitCamera;
				bResetCamera			= data.bResetCamera;
                bSuperArmorNotRed       = data.bSuperArmorNotRed;
                fGuardDefence           = data.fGuardDefence;
				bAttackState			= data.bAttackState;
#ifdef SERV_EVE_BATTLE_SERAPH
				bNoChangSpectro			= data.bNoChangSpectro;
#endif

				fReflexMagicStartTime	= data.fReflexMagicStartTime;
				fReflexMagicEndTime		= data.fReflexMagicEndTime;


				memcpy( bEventFlagList, data.bEventFlagList, sizeof(bool) * EVENT_FLAG_LIST_NUM );

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SUPER_ARMOR_TIME
				m_vecSuperArmorTime		= data.m_vecSuperArmorTime;
#endif

#ifdef ARA_CHARACTER_BASE
				m_usSoundSet			= data.m_usSoundSet;
#endif

#ifdef ADD_CAN_PASS_ALLY_UNIT	//JHKang
				bCanPassAllyUnit		= data.bCanPassAllyUnit;
#endif //ADD_CAN_PASS_ALLY_UNIT
				bAllowDirChange			= data.bAllowDirChange;
				m_DefenceTime.x			= data.m_DefenceTime.x;
				m_DefenceTime.y			= data.m_DefenceTime.y;
				m_DefenceTime.z			= data.m_DefenceTime.z;
				afterImage				= data.afterImage;
				m_mapAddSlashTrace		= data.m_mapAddSlashTrace;
				fSlashTraceTipWide		= data.fSlashTraceTipWide;
				bApplyAnimMove			= data.bApplyAnimMove;
				bFallDown				= data.bFallDown;
                bGuardFront             = data.bGuardFront;
				bGuardBack				= data.bGuardBack;
                bDamageDown             = data.bDamageDown;
				bNotUseLandConnect		= data.bNotUseLandConnect;
#ifdef SECRET_VELDER
				m_vInvisibleTime		= data.m_vInvisibleTime;
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


				return *this;
			}
			void Init()
			{
				bLandConnect			= true;
				bCanPushUnit			= true;
				bCanPassUnit			= false;
				bInvincible				= false;
				bSuperArmor				= false;
				fRevengeStartTime		= -1.0f;
				fRevengeEndTime			= -1.0f;
				normalCamera			= 0;
				hitCamera				= 0;
				bResetCamera			= true;
                bSuperArmorNotRed       = false;
                fGuardDefence           = 0;
				bAttackState			= false;
#ifdef SERV_EVE_BATTLE_SERAPH
				bNoChangSpectro			= false;
#endif

				fReflexMagicStartTime	= -1.f;
				fReflexMagicEndTime		= -1.f;

				ZeroMemory( bEventFlagList, sizeof(bool) * EVENT_FLAG_LIST_NUM );

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SUPER_ARMOR_TIME
				m_vecSuperArmorTime.clear();
#endif

#ifdef ARA_CHARACTER_BASE
				m_usSoundSet = 0;
#endif

#ifdef ADD_CAN_PASS_ALLY_UNIT	//JHKang
				bCanPassAllyUnit		= true;
#endif //ADD_CAN_PASS_ALLY_UNIT
				bAllowDirChange			= true;
				INIT_VECTOR2( afterImage, 0.0f, 0.0f );		
				m_mapAddSlashTrace.clear();
				fSlashTraceTipWide		= 4.0f;
				bApplyAnimMove			= true;
				bFallDown				= true;
                bGuardFront             = true;
				bGuardBack				= true;
                bDamageDown             = true;
				bNotUseLandConnect = false;
#ifdef SEASON3_MONSTER_2010_12
				m_vReflectMagic = D3DXVECTOR3(0.f, 0.f, 0.f);
#endif
#ifdef SECRET_VELDER
				INIT_VECTOR2( m_vInvisibleTime, 0.0f, 0.0f );
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


			}
		};

//-------------------------------------------------------------------------------------------------------

		struct UnitCondition	//상황을 나타내는 파라메터
		{
			char			nextState;
			char			nextSubState;
			float			fStateTime;			/// 스테이트 시작으로 부터 경과된 시간
			float			fStateTimeBack;		/// 스테이트 시작으로 부터 이전 프레임까지 경과된 시간
			bool			bTransitionChange;

			float			fUnitWidth;	
			float			fUnitHeight;	

			D3DXVECTOR3		dirDegree;
			D3DXVECTOR3		dirVector;

			float			fStopTime;
			float			fStop2Time;

			//bool			bFall;
			bool			bFootOnLine;

//{{ kimhc // 2010.7.7 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER
			bool			bTempLineMap;
#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.7 // 몬스터가 생성한 라인맵

//#ifdef STEP_ON_MONSTER_TEST
//			bool			bFootOnUnit;
//#endif STEP_ON_MONSTER_TEST
			D3DXVECTOR3		landPosition;

			bool			bHit;
			bool			bAttackerFront;

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			bool			bFrameBufferPass;	/// 현재 프레임이 m_ReceiveSyncDataList.size()가 0이면 true
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            union
            {
                KTWO_PARTS  stateChangeParts;
                char		stateChangeNum;
            };
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			char			stateChangeNum;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef WALL_JUMP_TEST
			bool			bFootOnWall;
#endif WALL_JUMP_TEST




#ifdef GRAPPLING_TEST
			GrapplingState m_GrapplingState;
#endif GRAPPLING_TEST



			UnitCondition()
			{
				Init();
			}
			void Init()
			{
				nextState					= -1;
				nextSubState				= -1;
				fStateTime					= 0.0f;
				fStateTimeBack				= 0.0f;
				bTransitionChange			= true;

				fUnitWidth					= 0.0f;
				fUnitHeight					= 0.0f;

				INIT_VECTOR3( dirDegree,	0,0,0 );
				INIT_VECTOR3( dirVector,	0,0,0 );

				fStopTime					= 0.0f;
				fStop2Time					= 0.0f;

				//bFall						= false;
				bFootOnLine					= true;

				//{{ kimhc // 2010.7.7 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER
				bTempLineMap				= false;
#endif	LINE_MAP_CREATED_BY_MONSTER
				//}} kimhc // 2010.7.7 // 몬스터가 생성한 라인맵

//#ifdef STEP_ON_MONSTER_TEST
//				bFootOnUnit					= false;
//#endif STEP_ON_MONSTER_TEST

				landPosition				= D3DXVECTOR3(0.0f,0.0f,0.0f);

				bHit						= false;
				bAttackerFront				= true;

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				bFrameBufferPass			= false;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				stateChangeNum				= 0;

#ifdef WALL_JUMP_TEST
				bFootOnWall					= false;
#endif WALL_JUMP_TEST

			}
		};

//-------------------------------------------------------------------------------------------------------

		struct PhysicParam
		{
			D3DXVECTOR2		nowSpeed;		/// 현재 스피드X,Y
			D3DXVECTOR2		passiveSpeed;	/// 이 스피드로 계속 보정해준다
			float			fDownAccel;		/// 하강 가속도

			float			fReloadAccel;	/// 전진 속도 감쇄율

			float			fGAccel;		/// 중력 가속도
			float			fMaxGSpeed;		/// 최대 하강 속도 Y

			float			fWalkSpeed;		/// 걷기 속도
			float			fRunSpeed;		/// 달리기 속도
			float			fJumpSpeed;		/// 일반 점프 상승속도
			float			fDashJumpSpeed;	/// 대시 점프 상승속도

//			float			fSpeedFactor;	/// 걷기, 달리기, 점프, 대시 점프 속도에 곱해진다. fix!! 근데 현재 구현은 이동속도에만 적용되도록 되어있다??

// 			SPEED_FACTOR 	m_MoveSpeedFactor;	/// 일정시간 동안 걷기, 뛰기 속도에 영향
// 			SPEED_FACTOR 	m_JumpSpeedFactor;	/// 일정시간 동안 점프, 대시 점프 속도에 영향
// 
// #ifdef DUNGEON_ITEM
// 			SPEED_FACTOR	m_MoveSpeedByItem;
// 			SPEED_FACTOR	m_JumpSpeedByItem;
// #endif

// #ifdef SERV_SKILL_NOTE
// 			std::vector<SPEED_FACTOR> m_vecAnimSpeedFactor;
// #endif
// 			std::vector<SPEED_FACTOR> m_vecMoveSpeedFactor;
// 			std::vector<SPEED_FACTOR> m_vecJumpSpeedFactor;
			
			PhysicParam()
			{
				INIT_VECTOR2( nowSpeed, 0.0f, 0.0f );
				INIT_VECTOR2( passiveSpeed, -1.0f, -1.0f );
				fDownAccel		= 0.0f;

				fReloadAccel	= 0.0f;

				fGAccel			= 0.0f;
				fMaxGSpeed		= 0.0f;

				fWalkSpeed		= 0.0f;
				fRunSpeed		= 0.0f;
				fJumpSpeed		= 0.0f;
				fDashJumpSpeed	= 0.0f;
				
//				fSpeedFactor	= 1.f;

// #ifdef SERV_SKILL_NOTE
// 				m_vecAnimSpeedFactor.clear();				
// #endif
// 				m_vecMoveSpeedFactor.clear();
// 				m_vecJumpSpeedFactor.clear();
			}

			void OnFrameMove( double fTime, float fElapsedTime )
			{
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					m_MoveSpeedFactor.m_fDuration -= fElapsedTime;
// 				}
// 
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					m_JumpSpeedFactor.m_fDuration -= fElapsedTime;
// 				}
// #ifdef DUNGEON_ITEM
// 				if( m_MoveSpeedByItem.m_fDuration > 0.f )
// 				{
// 					m_MoveSpeedByItem.m_fDuration -= fElapsedTime;
// 				}
// 				if( m_JumpSpeedByItem.m_fDuration > 0.f )
// 				{
// 					m_JumpSpeedByItem.m_fDuration -= fElapsedTime;
// 				}
// #endif
			}

			float GetWalkSpeed() const
			{
				return fWalkSpeed;
// 				float fMoveSpeed = 1.f;
// 				if( m_MoveSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fMoveSpeed = m_MoveSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fMoveSpeed *= GetVecMoveSpeed();
// 
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fWalkSpeed * fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate * fMoveSpeed;
// 				}
// 				else
// 				{
// 					return fWalkSpeed * fSpeedFactor * fMoveSpeed;
// 				}
			}

			float GetDashSpeed() const
			{
				return fRunSpeed;
// #ifdef DUNGEON_ITEM				
// 				float fMoveSpeed = 1.f;
// 				if( m_MoveSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fMoveSpeed = m_MoveSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fMoveSpeed *= GetVecMoveSpeed();
// 
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fRunSpeed * fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate * fMoveSpeed;
// 				}
// 				else
// 				{
// 					return fRunSpeed * fSpeedFactor * fMoveSpeed;
// 				}
// #else
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fRunSpeed * fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					return fRunSpeed * fSpeedFactor;
// 				}
// #endif				
			}

			float GetJumpSpeed() const
			{
				return fJumpSpeed;
// #ifdef DUNGEON_ITEM		
// 				float fJumpSpeedTemp = 1.f;
// 				if( m_JumpSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fJumpSpeedTemp *= m_JumpSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fJumpSpeedTemp *= GetVecJumpSpeed();
// 
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fJumpSpeed * m_JumpSpeedFactor.m_fSpeedRate * fJumpSpeedTemp;
// 				}
// 				else
// 				{
// 					return fJumpSpeed * fJumpSpeedTemp;
// 				}
// #else
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fJumpSpeed * m_JumpSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					return fJumpSpeed;
// 				}
// #endif
				
			};

			float GetDashJumpSpeed() const
			{
				return fDashJumpSpeed;
// #ifdef DUNGEON_ITEM				
// 				float fJumpSpeedTemp = 1.f;
// 				if( m_JumpSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fJumpSpeedTemp *= m_JumpSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fJumpSpeedTemp *= GetVecJumpSpeed();
// 
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fDashJumpSpeed * m_JumpSpeedFactor.m_fSpeedRate * fJumpSpeedTemp;
// 				}
// 				else
// 				{
// 					return fDashJumpSpeed * fJumpSpeedTemp;
// 				}
// #else
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fDashJumpSpeed * m_JumpSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					return fDashJumpSpeed;
// 				}
// #endif				
			}

#ifdef SERV_SKILL_NOTE
			float GetAnimSpeed()
			{
				return 1.0f;
// 				float fAnimSpeed = 1.f;
// 				for(UINT i=0; i<m_vecAnimSpeedFactor.size(); ++i)
// 				{
// 					if(m_vecAnimSpeedFactor[i].m_fDuration > 0.f)
// 					{
// 						fAnimSpeed *= m_vecAnimSpeedFactor[i].m_fSpeedRate;
// 					}
// 				}
// 
// 				return fAnimSpeed;
			}
#endif

			float GetVecMoveSpeed()
			{
				return 1.0f;
// 				float fMoveSpeed = 1.f;
// 				for(UINT i=0; i<m_vecMoveSpeedFactor.size(); ++i)
// 				{
// 					if(m_vecMoveSpeedFactor[i].m_fDuration > 0.f)
// 					{
// 						fMoveSpeed *= m_vecMoveSpeedFactor[i].m_fSpeedRate;
// 					}
// 				}
// 
// 				return fMoveSpeed;
			}

			float GetVecJumpSpeed()
			{
				return 1.0f;
// 				float fJumpSpeed = 1.f;
// 				for(UINT i=0; i<m_vecJumpSpeedFactor.size(); ++i)
// 				{
// 					if(m_vecJumpSpeedFactor[i].m_fDuration > 0.f)
// 					{
// 						fJumpSpeed *= m_vecJumpSpeedFactor[i].m_fSpeedRate;
// 					}
// 				}
// 
// 				return fJumpSpeed;
			}

			float GetMoveSpeedPower()
			{
				return 1.0f;
// #ifdef DUNGEON_ITEM			
// 				float fMoveSpeed = 1.f;
// 				if( m_MoveSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fMoveSpeed = m_MoveSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fMoveSpeed *= GetVecMoveSpeed();
// 
// 				float fRetSpeed = 1.f;
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					fRetSpeed = fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate * fMoveSpeed;
// 				}
// 				else
// 				{
// 					fRetSpeed = fSpeedFactor * fMoveSpeed;
// 				}
// #else
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					fRetSpeed = fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					fRetSpeed = fSpeedFactor;
// 				}
// #endif		
// 				if( fRetSpeed <= 0.f )
// 					fRetSpeed = 1.f;
// 
// 				return fRetSpeed;
			}

			float GetJumpSpeedPower()
			{
				return 1.0f;
// #ifdef DUNGEON_ITEM		
// 				float fJumpSpeedTemp = 1.f;
// 				if( m_JumpSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fJumpSpeedTemp *= m_JumpSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fJumpSpeedTemp *= GetVecJumpSpeed();
// 
// 				float fRetSpeed = 1.f;
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					fRetSpeed = m_JumpSpeedFactor.m_fSpeedRate * fJumpSpeedTemp;
// 				}
// 				else
// 				{
// 					fRetSpeed = fJumpSpeedTemp;
// 				}
// #else
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					fRetSpeed = m_JumpSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					return 1.f;
// 				}
// #endif
// 				if( fRetSpeed <= 0.f )
// 					fRetSpeed = 1.f;
// 
// 				return fRetSpeed;			
			}
		};

//-------------------------------------------------------------------------------------------------------
		struct SoundPlayData 
		{
			float			m_SoundPlayTime;		/// 시작 시간
			wstring			m_SoundPlayName;		/// 파일명
			int				m_SoundPlayRate;		/// 비율?
			bool			m_bOnlyIfMyUnit;		/// 
			float			m_fMaxSoundDistance;	/// 사운드 최대 거리
			bool			m_b3DSound;

            SoundPlayData()
              : m_SoundPlayTime( -1.f )
              , m_SoundPlayName()
              , m_SoundPlayRate( 100 )
              , m_bOnlyIfMyUnit( false )
              , m_fMaxSoundDistance( -1.f )
              , m_b3DSound( true ) 
            {
            }
		};

//-------------------------------------------------------------------------------------------------------
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		struct ParticleEffectData
		{
			bool		m_bUse;
			bool		m_bMajor;
			float		m_fTime;
			wstring		m_Name;
			bool		m_bWeapon;
			wstring		m_Pos;
			bool		m_StateEndDelete;
			bool		m_bTrace;
			D3DXVECTOR3 m_vOffset;
			bool		m_bApplyUnitRotation;
			D3DXVECTOR3	m_vRotation;
            D3DXVECTOR3 m_vScale;
			bool		m_bUnitScale;
			
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;

			ParticleEffectData()
			{
				m_bUse				= false;
				m_bMajor			= false;
				m_fTime				= 0.0f;
				m_bWeapon			= false;
				m_StateEndDelete	= false;
				m_hSeq				= INVALID_PARTICLE_SEQUENCE_HANDLE;
				m_bTrace			= true;
				m_vOffset			= D3DXVECTOR3(0, 0, 0);
				m_bApplyUnitRotation = false;
				m_vRotation			= D3DXVECTOR3(0, 0, 0);
                m_vScale            = D3DXVECTOR3(1, 1, 1);
				m_bUnitScale		= false;

			}
			~ParticleEffectData();

#ifdef BOOST_SINGLETON_POOL_TEST
			void* operator new(size_t s);
			void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST

		};

//-------------------------------------------------------------------------------------------------------
		struct MeshPlayerEffectData
		{
			bool		m_bUse;
			bool		m_bMajor;
			float		m_fTime;
			wstring		m_Name;
			bool		m_bWeapon;
			wstring		m_Pos;
			bool		m_StateEndDelete;
			bool		m_Trace;

			CKTDGXMeshPlayer::CXMeshInstanceHandle m_hMeshInst;

			MeshPlayerEffectData()
			{
				m_bUse				= false;
				m_bMajor			= false;
				m_fTime				= 0.0f;
				m_bWeapon			= false;
				m_StateEndDelete	= false;
				m_Trace				= false;
				m_hMeshInst			= INVALID_MESH_INSTANCE_HANDLE;
			}
			~MeshPlayerEffectData();

#ifdef BOOST_SINGLETON_POOL_TEST
			void* operator new(size_t s);
			void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST


		};

//-------------------------------------------------------------------------------------------------------
		struct CameraCrashData
		{
			float		m_fTime;
			float		m_fGap;
			float		m_CrashTime;
			bool		m_bOnlyFocus;

			CameraCrashData()
			{
				m_fTime			= 0.0f;
				m_fGap			= 0.0f;
				m_CrashTime		= 0.0f;
				m_bOnlyFocus	= false;
			}
		};
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//-------------------------------------------------------------------------------------------------------
		struct LeapData
		{
			D3DXVECTOR3										m_StartPosToLeap;
			D3DXVECTOR3										m_ArrivalPos;
			D3DXVECTOR3										m_DirVecToLeap;
			float											m_fVerticalSpeedToLeap;
			float											m_fParallelSpeedToLeap;
			float											m_fHeightToLeap;
			int												m_DegreeToLeap;
			float											m_fTimeToLeap;
			float											m_fElapsedTimeToLeap;

			LeapData()
			{
				m_StartPosToLeap				= D3DXVECTOR3(0,0,0);
				m_ArrivalPos					= D3DXVECTOR3(0,0,0);
				m_DirVecToLeap					= D3DXVECTOR3(0,0,0);
				m_fVerticalSpeedToLeap			= 0.0f;
				m_fParallelSpeedToLeap			= 0.0f;
				m_fHeightToLeap					= 0.0f;
				m_DegreeToLeap					= 0;
				m_fTimeToLeap					= 0.0f;
				m_fElapsedTimeToLeap			= 0.0f;
			}
		};

//-------------------------------------------------------------------------------------------------------

		struct WeaponParticle
		{
			wstring		m_wstrBoneName;
			bool		m_bIsMajorParticle;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;

			WeaponParticle();
			~WeaponParticle();
		};

//-------------------------------------------------------------------------------------------------------

//{{ robobeg : 2008-10-28
        struct Weapon : public CKTDGObject
//}} robobeg : 2008-10-28
		{
			//{{ kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07
		private:
			bool					m_bZEnable;		// NPC 스크립트 내에서 각 WEAPON 테이블의 Z_ENABLE 여부를 담는 변수
			//}} kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			// 청의 시스템 중 광폭화 모드에서 무기를 교환하는 기능과 관련하여 추가함
			// 기존에는 SlashTrace, EnchantData등의 정보를 m_puserWeaponEquip의 내부에서 가져왔으나
			// pItemTempletOrigin이 NULL 이 아닌 경우에는 이곳에서 가져오도록 하여, Item정보는 itemTemplet에서,
			// 메시 정보는 m_pUserWeaponEquip에서 따로 참조할 수도 있도록 함
			const CX2Item::ItemTemplet*  m_pItemTempletOrigin;	/// SlashTrace, EnchantData등에 대한 정보를 참조할 ItemTemplet
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		public:

//{{ robobeg : 2008-10-28
			//CX2GameUnit*			m_pOwnerUnit;
//}} robobeg : 2008-10-28

			int						m_iWeaponIndex;
            /// 내부적으로 사용하는 객체이므로 smart pointer를 사용하지 않는다.
            /// - jintaeks on 2009-01-12, 17:36
			CKTDGXSkinAfterImage*	m_pAfterImage[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

			CX2EqipPtr				m_pUserWeaponEquip;		// guuser는 이것을 사용하고

			//{{ kimhc // NPC만 사용함
            /// 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
            /// - jintaeks on 2009-01-12, 16:22
			CKTDGXSkinAnim*			m_pXSkinWeapon;			// gunpc는 이것을 사용한다
			D3DXMATRIX*				m_pMatrixWeapon;
			D3DXVECTOR3				m_WeaponRotation;
			D3DXVECTOR3				m_WeaponScale;
			//}} kimhc // NPC만 사용함

			
			// slash trace 관련 변수들
			bool					m_bUseSlashTrace;
			//wstring					m_SlashTraceTopBone;
			//wstring					m_SlashTraceBottomBone;
			CKTDGSlashTrace*		m_pSlashTrace[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
			CKTDGSlashTrace*		m_pSlashTraceTip[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
#ifdef ARA_CHARACTER_BASE
			CKTDGSlashTrace*		m_pSlashTraceTip2[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
			CKTDGSlashTrace*		m_pSlashTraceCustomTexture[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
#endif // CUSTOM_SLASH_TRACE_TEXTURE

			D3DXCOLOR				m_coSlashTrace;
			D3DXCOLOR				m_coSlashTraceHyper;
			D3DXCOLOR				m_coSlashTraceTip;
			D3DXCOLOR				m_coSlashTraceTipHyper;
			
			CKTDXDeviceXSkinMesh::MultiAnimFrame*	m_pFrame_TRACE_START[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];		// slash trace가 붙는 frame
			CKTDXDeviceXSkinMesh::MultiAnimFrame*	m_pFrame_TRACE_END[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

			vector<WeaponParticle*>					m_vecpWeaponParticle;

			CKTDGParticleSystem::CParticleEventSequenceHandle	m_ahEnchantWeapon[3];

//{{ robobeg : 2008-10-28
			//bool					m_bAlphaObject;
//}} robobeg : 2008-10-28
			bool                    m_bEnable;
			//vector<wstring>         m_vecAttackBoxName;

		private:
			//Weapon() { ASSERT( !"invalid" ); }
//{{ robobeg : 2008-10-28
			Weapon( CX2GameUnit& refInOwnerUnit_ );
//}} robobeg : 2008-10-28

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			Weapon( CX2GameUnit& refInOwnerUnit_, 
                const CX2Item::ItemTemplet* pItemTempletOrigin_ 
                );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
			~Weapon();

			bool					m_bTraceColor;
			D3DXCOLOR				m_vTraceColor;
			D3DXCOLOR				m_vTraceTipColor;

			//{{ JHKang / 2011.03.23 / Blend 값 지정
#ifdef ADD_WEAPON_BLEND_VALUE
			D3DBLEND	m_d3dSrcBlend;
			D3DBLEND	m_d3dDestBlend;
#endif

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 무기 강화 이펙트 객체
			CKTDXDeviceXSkinMesh::MultiAnimFrame*				m_pFrame_ATTACK_LINE_END0;
			CKTDXDeviceXSkinMesh::MultiAnimFrame*				m_pFrame_ATTACK_LINE_START0_FRONT;

			CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqUpgradeWeapon;
			CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqUpgradeWeapon2;
			CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqUpgradeWeapon3;
			CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqUpgradeWeapon4;
#endif ADD_UPGRADE_WEAPON_PARTICLE
			//}}

		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
		private:
	#ifdef ARA_CHARACTER_BASE
			void InitWeapon( CX2Unit::UNIT_TYPE eUnitType_ = CX2Unit::UT_NONE );
	#else 
			void InitWeapon();
	#endif //ARA_CHARACTER_BASE
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

		public:
			static Weapon* CreateWeapon( CX2GameUnit& refInOwnerUnit_ ) { return new Weapon( refInOwnerUnit_ ); }
//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			static Weapon* CreateWeapon( CX2GameUnit& refInOwnerUnit_, 
                const CX2Item::ItemTemplet* pItemTempletOrigin_ 
                )
			{
				return new Weapon( refInOwnerUnit_, pItemTempletOrigin_ );
			}
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		


			bool IsTraceColor() { return m_bTraceColor; }
			void SetTraceColor(bool val, D3DXCOLOR vColor, D3DXCOLOR vTipColor) 
			{ 
				m_bTraceColor = val; 
				m_vTraceColor = vColor; 
				m_vTraceTipColor = vTipColor; 
			}
			

//{{ robobeg : 2008-10-28
            CX2GameUnit& GetOwnerUnit() { return *static_cast<CX2GameUnit*>( GetManagerObject() ); }
//}} robobeg : 2008-10-28


			// @iModelIndex: 몇 번째 메시인지
			D3DXVECTOR3 GetBonePos( const WCHAR* pBoneName, int iModelIndex );

			//{{ kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)
			const D3DXMATRIX* GetCombineMatrix( const WCHAR* pBoneName, int iModelIndex ) const;
			//}} kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)

//{{ robobeg : 2008-10-28
			//void SetShow( bool bShow )
            virtual void NotifyShowObjectChanged();
//}} robobeg : 2008-10-28

			void SetEnable( bool bEnable );

			bool LoadDataForUser( KLuaManager& luaManager );
			bool LoadDataForNPC( KLuaManager& luaManager );

//{{ robobeg : 2008-10-28

			virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
            virtual RENDER_HINT     OnFrameRender_Prepare();
			//void OnFrameRenderForNPC();
            virtual void                OnFrameRender_Draw();
//}} robobeg : 2008-10-28

			
			void AddSlashTraceByType( int iType, D3DXVECTOR3 up, D3DXVECTOR3 down, D3DXVECTOR3 tipDown, int iModelIndex );

			void CommonStateEnd();
			void CommonFrameMoveForNPC( double fTime, float fElapsedTime );
			void CommonFrameMoveForUser( double fTime, float fElapsedTime );
		
			void CreateEnchantParticle( const WCHAR* pName1, const WCHAR* pName2, const WCHAR* pName3 );
			void InitEnchantParticleForSword();
			void InitEnchantParticleForBow();
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			static void AppendEnchantParticleForSwordToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
			static void AppendEnchantParticleForBowToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

			void EnchantParticleFrameMoveForSword();
			void EnchantParticleFrameMoveForBow();
			void EnchantParticleFrameMoveForEve();
#ifdef ARA_CHARACTER_BASE
			void EnchantParticleFrameMoveForAra();
#endif
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			void EnchantParticleFrameMoveForAdd();
#endif //SERV_9TH_NEW_CHARACTER


			void SetEnchantParticleShow(bool bShow);

			//{{ kimhc // 실시간 엘소드 중 실시간 장비 교체
#ifdef REAL_TIME_ELSWORD
			void DeleteEnchantParticle();
#endif REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 실시간 장비 교체

			//{{ kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07
			bool GetZEnable() const;
			void SetZEnable( bool bEnable );
			//}} kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07
			void UpdateSlashTraceTextureName();

#ifdef FIX_SLASH_TRACE
			void InitSlashTrace();
#endif

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 강화 무기에 대한 이펙트 출력 처리
			void InitUpgradeWeaponParticle();		/// 강화 무기 이펙트 초기화
			void CreateUpgradeWeaponParticle( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL );	/// 강화 무기 이펙트 생성
			void ProcessUpgradeWeaponParticle();	/// 강화 무기 이펙트 갱신
			void DeleteUpgradeWeaponParticle();		/// 강화 무기 이펙트 제거
#endif ADD_UPGRADE_WEAPON_PARTICLE
		};

//-------------------------------------------------------------------------------------------------------

		struct TimedCriticalBuff
		{
			float m_fTimeLeft;
			float m_fDamageRate;
			float m_fEffectiveRate; // 크리티컬 발동 확률 증가치

			TimedCriticalBuff()
			{
				Init();
			}

			void Init()
			{
				m_fTimeLeft = 0.f;
				m_fDamageRate = 0.f;
				m_fEffectiveRate = 0.f;
			}

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				if( m_fTimeLeft > 0.f )
				{
					m_fTimeLeft -= fElapsedTime;
					if( m_fTimeLeft <= 0.f )
					{
						m_fTimeLeft = 0.f;
						m_fDamageRate = 0.f;
						m_fEffectiveRate = 0.f;
					}
				}
			}
		};

//-------------------------------------------------------------------------------------------------------

		struct TimedMagicDefenceDebuff
		{
			float m_fEffectiveTime;
			float m_fMagicDefenceRate;
			float m_fEffectiveRate; // 크리티컬 버프가 발동할 확률

			TimedMagicDefenceDebuff()
			{
				Init();
			}

			void Init()
			{
				m_fEffectiveTime = 0.f;
				m_fMagicDefenceRate = 0.f;
				m_fEffectiveRate = 0.f;
			}

		};

//-------------------------------------------------------------------------------------------------------

		struct SkillRelatedData
		{
			float m_fForceDownValueRateOfRenaArrowAttack;		// 레나의 화살공격 강제다운 수치 비율
			float m_fForceDownValueRateOfRangeAttack;			// 발사체 공격의 강제다운 수치 비율
			float m_fForceDownValueRateOfMeleeAttack;			// melee 공격의 강제다운 수치 비율
			float m_fSkillCoolTimeLeft_SI_P_COMMON_SOFTBODY;
			int m_iNoDownDamageAtStandUpAttackPercent;
			float m_fStatAtkRateOfSummoned;						// 소환된 몬스터의 공격력 비율

		public: 
			SkillRelatedData()
			{
				Init();
			}

			void Init()
			{
				m_fForceDownValueRateOfRenaArrowAttack = 1.f;
				m_fForceDownValueRateOfRangeAttack = 1.f;
				m_fForceDownValueRateOfMeleeAttack = 1.f;
				m_fSkillCoolTimeLeft_SI_P_COMMON_SOFTBODY = 0.f;
				m_iNoDownDamageAtStandUpAttackPercent = 0;
				m_fStatAtkRateOfSummoned = 1.f;
			}

			float GetForceDownValueRateOfRenaArrowAttack() const { return m_fForceDownValueRateOfRenaArrowAttack; }
			void SetForceDownValueRateOfRenaArrowAttack( float fForceDownValueRateOfRenaArrowAttack_ ) 
			{ 
				m_fForceDownValueRateOfRenaArrowAttack = fForceDownValueRateOfRenaArrowAttack_; 

				ASSERT( 0.f < m_fForceDownValueRateOfRenaArrowAttack && 1.f >= m_fForceDownValueRateOfRenaArrowAttack );

				if ( 0.f >= m_fForceDownValueRateOfRenaArrowAttack || 1.f < m_fForceDownValueRateOfRenaArrowAttack )
					m_fForceDownValueRateOfRenaArrowAttack = 1.f;
			}

			// 오현빈 : TODO : 처리가 SetDamageDataFromLUA 외부에 있어서, 일부 처리되지 않는 곳이 있음.(DAMAGE_DATA_NEXT, 버프팩터 데미지 등..) 수정 필요
			float GetForceDownValueRateOfRangeAttack() const { return m_fForceDownValueRateOfRangeAttack; }
			void SetForceDownValueRateOfRangeAttack(float fForceDownValueRateOfRangeAttack_) 
			{ 
				m_fForceDownValueRateOfRangeAttack = fForceDownValueRateOfRangeAttack_; 

				ASSERT( 0.f < m_fForceDownValueRateOfRangeAttack && 1.f >= m_fForceDownValueRateOfRangeAttack );

				if ( 0.f >= m_fForceDownValueRateOfRangeAttack || 1.f < m_fForceDownValueRateOfRangeAttack )
					m_fForceDownValueRateOfRangeAttack = 1.f;
			}

			float GetForceDownValueRateOfMeleeAttack() const { return m_fForceDownValueRateOfMeleeAttack; }
			void SetForceDownValueRateOfMeleeAttack(float fForceDownValueRateOfMeleeAttack_) 
			{ 
				m_fForceDownValueRateOfMeleeAttack = fForceDownValueRateOfMeleeAttack_; 

				ASSERT( 0.f < m_fForceDownValueRateOfMeleeAttack && 1.f >= m_fForceDownValueRateOfMeleeAttack );

				if ( 0.f >= m_fForceDownValueRateOfMeleeAttack || 1.f < m_fForceDownValueRateOfMeleeAttack )
					m_fForceDownValueRateOfMeleeAttack = 1.f;
			}

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				KTDXPROFILE();
			}
		};

//-------------------------------------------------------------------------------------------------------

		struct CommonState
		{
			int m_Wait;
			//int m_Walk;
			//int m_Dash;

			//int m_GrabFront;			// 앞에 있는 것을 잡기
			//int m_PickUp;				// 땅에 있는 것을 줍기
#ifdef GRAPPLING_TEST
			int m_GrappledFront;
			int m_GrappledBack;
#endif // GRAPPLING_TEST
			int m_Thrown;
			//int m_ThrownForward;
			//int m_ThrownUpward;



#ifdef CLIFF_CLIMBING_TEST
			int m_ClimbWait;
			int m_ClimbFront;
			int m_ClimbUp;
			int m_ClimbUpFront;
			int m_ClimbDown;
			int m_ClimbDownFront;
#endif CLIFF_CLIMBING_TEST


			int m_DownDamageLandFront;	
			int m_DownDamageLandBack;	
			int m_DownDamageAir;
			int m_DamageAirDownLanding;


			int m_LinkJumpState;


//#ifdef TRANSFORMER_TEST
//			int m_Transformed; // 변신중인 상태
//#endif TRANSFORMER_TEST



			CommonState()
			: m_Wait( 0 )
			//, m_GrabFront( 0 )
			//, m_PickUp( 0 )
#ifdef GRAPPLING_TEST
			, m_GrappledFront( 0 )
			, m_GrappledBack( 0 )
#endif // GRAPPLING_TEST
			, m_Thrown( 0 )
			//, m_ThrownForward( 0 )
			//, m_ThrownUpward( 0 )
#ifdef CLIFF_CLIMBING_TEST
			, m_ClimbWait( 0 )
			, m_ClimbFront( 0 )
			, m_ClimbUp( 0 )
			, m_ClimbUpFront( 0 )
			, m_ClimbDown( 0 )
			, m_ClimbDownFront( 0 )
#endif CLIFF_CLIMBING_TEST
			, m_DownDamageLandFront( 0 )
			, m_DownDamageLandBack( 0 )
			, m_DownDamageAir( 0 )
			, m_DamageAirDownLanding( 0 )
			, m_LinkJumpState( 0 )
//#ifdef TRANSFORMER_TEST
//			, m_Transformed( 0 )
//#endif TRANSFORMER_TEST

			{
			}
		};

//-------------------------------------------------------------------------------------------------------

#ifdef MODIFY_EFFECT_SET_DELETE
		enum DELETE_CONDITION_EFFECT_SET
		{
			DCES_NONE = 0,
			DCES_DAMAGE_REACT,
			DCES_STATE_END,
			DCES_DIE,
			DCES_CUSTOM_STATE,
		};

		struct EffectSetToPlay
		{
			std::wstring m_wstrEffectSetName;
			float		m_fStartAnimationTime;

#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			vector<DELETE_CONDITION_EFFECT_SET> m_vecDeleteCondition;
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			DELETE_CONDITION_EFFECT_SET m_eDeleteCondition;
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장

#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			EffectSetToPlay()
				: m_wstrEffectSetName( L"" )
				, m_fStartAnimationTime( 0.f )
			{
				m_vecDeleteCondition.push_back( DCES_NONE );
			}

			EffectSetToPlay( const wstring& name, float fTime )
			{
				m_wstrEffectSetName = name;
				m_fStartAnimationTime = fTime;
				m_vecDeleteCondition.push_back( DCES_NONE );
			}
			
			EffectSetToPlay( const wstring& name, float fTime, vector<DELETE_CONDITION_EFFECT_SET> & vecDeleteCondition )
			{
				m_wstrEffectSetName = name;
				m_fStartAnimationTime = fTime;
				m_vecDeleteCondition.clear();
				m_vecDeleteCondition.resize( vecDeleteCondition.size() );
				std::copy ( vecDeleteCondition.begin(), vecDeleteCondition.end(), m_vecDeleteCondition.begin() );
			}

#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			EffectSetToPlay()
				: m_wstrEffectSetName( L"" )
				, m_fStartAnimationTime( 0.f )
				, m_eDeleteCondition( DCES_NONE )
			{}
			EffectSetToPlay( const wstring& name, float fTime, DELETE_CONDITION_EFFECT_SET eDeleteCondition = DCES_NONE )
			{
				m_wstrEffectSetName = name;
				m_fStartAnimationTime = fTime;
				m_eDeleteCondition = eDeleteCondition;
			}
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            bool    operator < ( const EffectSetToPlay& rhs_ ) const { return m_fStartAnimationTime < rhs_.m_fStartAnimationTime; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		};
#else // MODIFY_EFFECT_SET_DELETE
	// MODIFY_EFFECT_SET_DELETE 디파인 꺼졌을 때 사용~
	struct EffectSetToPlay
	{
		std::wstring m_wstrEffectSetName;
		float		m_fStartAnimationTime;

		EffectSetToPlay()
			: m_wstrEffectSetName( L"" )
			, m_fStartAnimationTime( 0.f )
		{
		}

		EffectSetToPlay( const wstring& name, float fTime )
		{
			m_wstrEffectSetName = name;
			m_fStartAnimationTime = fTime;
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        bool    operator < ( const EffectSetToPlay& rhs_ ) const { return m_fStartAnimationTime < rhs_.m_fStartAnimationTime; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	};
	// ~ MODIFY_EFFECT_SET_DELETE 디파인 꺼졌을 때 사용
#endif //MODIFY_EFFECT_SET_DELETE


//-------------------------------------------------------------------------------------------------------
#ifdef DAMAGE_EFFECT_GROUP_DAMAGE			/// 데미지 이펙트 그룹 데미지 정보 저장용 구조체

		struct DamageEffectGroupData;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        typedef boost::intrusive_ptr<DamageEffectGroupData> DamageEffectGroupDataPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		typedef boost::shared_ptr<DamageEffectGroupData> DamageEffectGroupDataPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		struct DamageEffectGroupData
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		{
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        private:
            unsigned                                        m_uRefCount;
        public:
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			std::wstring				m_wstrDamageEffectName;		/// 그룹 데미지 이름
			UidType						m_uidAttackerUID;			/// 공격자 유닛 아이디
			CX2GameUnit::GAME_UNIT_TYPE m_eAttackerGameUnitType;	/// 공격자 게임 유닛 타입
			float						m_fHitGFap;					/// 피격 간격 ( 피격 간격으로 데미지 판정 여부를 검사하므로, 피격 간격이 
																	///				없는 데미지 이펙트는 그룹 데미지가 동작되지 않는다. )

			DamageEffectGroupData()
				: m_wstrDamageEffectName( L"" )
				, m_uidAttackerUID( -1 )
				, m_fHitGFap( 0.f )
				, m_eAttackerGameUnitType( CX2GameUnit::GUT_USER )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			{
			}

			DamageEffectGroupData( const wstring& wstrDamageEffectName, UidType uidAttackerUID, 
								   CX2GameUnit::GAME_UNIT_TYPE eAttackerGameUnitType, float fHitGFap )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			{
				m_wstrDamageEffectName	= wstrDamageEffectName;
				m_uidAttackerUID		= uidAttackerUID;
				m_fHitGFap				= fHitGFap;
				m_eAttackerGameUnitType	= eAttackerGameUnitType;
			}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            void    AddRef()    {   ++m_uRefCount; }
            void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		};

#endif DAMAGE_EFFECT_GROUP_DAMAGE
//-------------------------------------------------------------------------------------------------------

	protected:
		CX2GameUnit( int unitIndex, int teamNum, const WCHAR* pScriptFileName );
		virtual ~CX2GameUnit(void);

#ifdef RIDING_SYSTEM
		bool CanNotShowEnchantedWeaponEffectToRiding();
#endif //RIDING_SYSTEM

//-------------------------------------------------------------------------------------------------------

		// Public Function Start
	public:
		static void					InitDevice( InitDeviceData& OutDevice_, KLuaManager& luaManager_ );
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void					InitComponent( InitDeviceData& OutDevice_, KLuaManager& luaManager_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void                 InitWeaponDataForNPC( InitDeviceData& OutDevice_, KLuaManager& luaManager_ );

		virtual void				Init( bool bUseTeamPos = true, int startPosIndex = -1 )		= NULL;
		virtual void				ReInit( bool bUseTeamPos = true, int startPosIndex = -1 );
		virtual void				InitEffect();
		virtual void				DeleteMinorParticle();
		void						DeleteGameUnitMajorParticle();
		void						DeleteGameUnitMinorParticle();


		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnCameraMove();

//{{ robobeg : 2008-10-28
		//virtual HRESULT				OnFrameRender()												= NULL;
        virtual void                OnFrameRender_Draw()                                        = NULL;
//}} robobeg : 2008-10-28

		virtual HRESULT				OnResetDevice()												= NULL;
		virtual HRESULT				OnLostDevice()												= NULL;

		virtual UidType				GetUnitUID() const { return m_UnitUID; }
		//virtual CX2Unit*			GetUnit() const { return NULL; }
		virtual CKTDGXSkinAnimPtr	GetSkinAnimPtr() const = NULL;
		//virtual void              StateChange( int state, bool bDirectChange = false )		= NULL;
        virtual bool                StateChange( int state, bool bDirectChange = false )		= NULL;
		virtual void				DamageReact( CX2DamageManager::DamageData* pDamageData )	= NULL;
		virtual void				AttackResult()												= NULL;
		
		//virtual CX2DamageManager::Damage SetAttackDamage( CX2DamageManager::DamageData* pDamageData ); // obsolete!!
		//virtual float				SetDefenceDamage( CX2DamageManager::DamageData* pDamageData, CX2DamageManager::Damage damage );		// 이함수 무슨 용도로 만들어 졌는지 모르겠음

		virtual float				GetItemAttackPower( CX2GameUnit* pAttacker, CX2Item* pItem = NULL, CX2DamageManager::DAMAGE_TYPE eDamageType = CX2DamageManager::DT_MIX );
		virtual float				GetFinalDamage( CX2DamageManager::DamageData* pAttDamageData );

#ifdef ELSWORD_WAY_OF_SWORD
		// GetFinalDamage 계산 할 때 검의길 타입에 따라 데미지 변경 시켜 주기 위한 함수
		virtual float GetFinalDamage_WayOfSword( CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_ ){ return 1.f; } // 기본 값 1
		virtual void SetCriticalPercent_WayOfSword( IN CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_,
													OUT float& fCriticalPercent_) {}
#endif //ELSWORD_WAY_OF_SWORD

		void InitEnableDash();
		virtual void SetEnableDash( bool bEnableDash,  ENABLE_DASH_STATE eState = EDS_END );
		bool GetEnableDash( ENABLE_DASH_STATE eState = EDS_END );

		bool GetEntangled() const { return m_bEntangled; }
		void SetEntangled( const bool val );

#ifdef DUNGEON_ITEM
		float GetTimeToReturnBasicScale() { return m_fTimeToReturnBasicScale; }
#endif

		float						CalcExtraAttackPower( CX2DamageManager::DamageData* pAttDamageData, CX2GameUnit* pAttacker, CX2Stat* pAttackerStat );
		float 						CalcLevelFactorForFinalDamage( CX2GameUnit* pAttacker );



		virtual D3DXVECTOR3			GetBonePos( const WCHAR* pBoneName ) const						= NULL;
		
#ifdef MODIFY_GET_BONE_POS
		// 오현빈 // 2012-12-17 // 본이 없는 경우에 대해 체크하기 위해 OUT 인자를 사용하는 함수 추가
		virtual bool				GetBonePos( const WCHAR* pBoneName, OUT D3DXVECTOR3& pos_ ) const = NULL;
#endif //MODIFY_GET_BONE_POS

//{{ kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)
		virtual const D3DXMATRIX*	GetCombineMatrixFromBoneName( const wstring& wstrBoneName )	const = NULL;
		const D3DXMATRIX* GetCombineMatrixFromWeaponBoneName( int iWeaponIndex, const wstring& wstrWeaponBoneName, int iModelIndex ) const;
//}} kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)

#ifdef RIDING_SYSTEM
		virtual const D3DXMATRIX*	GetCombineMatrixFromRidingPetBoneName( const wstring& wstrBoneName );
#endif //RIDING_SYSTEM

		//virtual D3DXVECTOR3			GetWeaponBonePos( const WCHAR* pBoneName );
		virtual void				Win()														= NULL;
		virtual void				Lose()														= NULL;
		virtual void				Draw()														= NULL;

//{{ robobeg : 2008-10-21
		//virtual void				SetShow( bool bShow );
		//bool						GetShow() { return m_bShow; }
        virtual void                NotifyShowObjectChanged();
//}} robobeg : 2008-10-21
		void						SetShadow( const WCHAR* pTexName, float fSize );

		virtual bool				IsFullHyperMode() const { return false; }		/// 3각성인가?
		virtual void				SetFullHyperMode( const bool bFullHyperMode_ ) {}

		virtual void				SetHyperModeCount( int i )	= NULL;
		virtual int					GetHyperModeCount() const	= NULL;
		virtual int					GetLastStateHyperModeCount() const = NULL;

		float						GetRemainHyperModeTime() const 
		{ 
			return ( NULL != GetGageData() ? GetGageData()->GetHyperModeRemainTime() : 0.0f );
		}

		void						SetRemainHyperModeTime( float fRemainHyperModeTime_ ) 
		{ 
			if ( NULL != GetGageData() )
				GetGageData()->SetHyperModeRemainTime( fRemainHyperModeTime_ ); 			
		}

		bool						VerifyRemainHyperModeTime() const 
		{ 
			return ( NULL != GetGageData() ? GetGageData()->VerifyHyperModeRemainTime() : true ); 
		} 

		float						GetMaxHyperModeTime() const { return m_fMaxHyperModeTime; }
		void						SetMaxHyperModeTime( float fMaxHyperModeTime_ ) { m_fMaxHyperModeTime = fMaxHyperModeTime_; };

		virtual		float			GetHyperModeTime() const { return GetMaxHyperModeTime(); }
		
		float GetForceHyperModeTime() const { return m_fForceHyperModeTime; }
		void SetForceHyperModeTime(float val) { m_fForceHyperModeTime = val; }
	

		bool						VerifyMaxHyperModeTime() const { return m_fMaxHyperModeTime.Verify(); }

		virtual float				GetFinalDamageToBeChangedBySkill( float fFinalDamage_ ) { return fFinalDamage_; }
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		virtual float				GetFinalDamageToBeChangedByActiveSkill( float fFinalDamage_ ) { return fFinalDamage_; }
#endif //UPGRADE_SKILL_SYSTEM_2013
		virtual void				ApplyHyperModeBuff();
		virtual void				HyperModeBuffEffectStart() = NULL;
		virtual void				HyperModeBuffEffectEnd();
		virtual void				HyperModeBuffEffectOnFrameMove();

		GAME_UNIT_TYPE				GetGameUnitType()	const			{ return m_GameUnitType; }
		void						SetUnitIndex( int unitIndex )		{ m_UnitIndex = unitIndex; }
		int							GetUnitIndex()						{ return m_UnitIndex; }
		int							GetTeam() const							{ return m_TeamNum; }
		void						SetTeamNum( int val )				{ m_TeamNum = val; } 
#ifdef	SERV_TRAPPING_RANGER_TEST
		void SetAllyTeam( CX2Room::TEAM_NUM eAllyTeam )					{ m_eAllyTeam = eAllyTeam; }
		CX2Room::TEAM_NUM GetAllyTeam()	const								{ return m_eAllyTeam; }
#endif	SERV_TRAPPING_RANGER_TEST

//#ifdef PVP_BOSS_COMBAT_TEST
//		int GetTeamNumOriginal() const { return m_TeamNumOriginal; }
//#endif PVP_BOSS_COMBAT_TEST

		virtual CX2GageData*	CreateGageData();
		virtual CX2GageUI*		CreateGageUI( const CX2GageData* pOwnerGageData_ );

		bool IsNullGageUI() const { return ( NULL == m_pGageUI ); }
		void SetShowGage( const bool bShow_ ) 
		{ 
			if ( !IsNullGageUI() )
				m_pGageUI->SetShow( bShow_ ); 
		}
		
		void SetHpTexture( const WCHAR* pwszSmallHpBar_ );
		void SetMpTexture( const WCHAR* pwszSmallMpBar_ );

		void RenderGageUI()
		{
			if ( !IsNullGageUI() )
				m_pGageUI->OnFrameRender();
		}

		D3DXVECTOR3					GetImpactPos()	const			{ return m_DamageData.impactPoint; }
        		
		const CX2GageData*			GetGageData() const				{ return m_pGageData; }
		CX2GageData*				GetGageData()					{ return m_pGageData; }
		
		float						GetNowHp()	const					{ return ( NULL != GetGageData() ? GetGageData()->GetNowHp() : 1 ); }
		float						GetMaxHp()	const					{ return GetGageData()->GetMaxHp(); }
		void						SetMaxHp( const float maxHP )		{ GetGageData()->SetMaxHp( maxHP ); }

		float						GetNowMp()	const					{ return GetGageData()->GetNowMp(); }
		float						GetMaxMp()	const					{ return GetGageData()->GetMaxMp(); }
		void						SetMaxMp( const float maxMP )		{ GetGageData()->SetMaxMp( maxMP ); }

		bool						VerifyNowHp() { return GetGageData()->VerifyNowHp(); }

		bool						GetEnableHPRelativeChangeRate() const { return GetGageData()->GetEnableHPRelativeChangeRate(); }
		void						SetEnableHPRelativeChangeRate( const bool bHpRelativeChangeRate_ ) { GetGageData()->SetEnableHPRelativeChangeRate( bHpRelativeChangeRate_ ); }

		bool						FlushMp( const float fFlushMp_ )	{ return GetGageData()->FlushMp( fFlushMp_ ); }
		bool						DamageFlushMp() { return GetGageData()->DamageFlushMp(); }
		bool						FlushForceDown() { return GetGageData()->FlushForceDown(); }
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
        bool						DamageFlushMp_CheckOnly() { return GetGageData()->DamageFlushMp_CheckOnly(); }
        bool						FlushForceDown_CheckOnly() { return GetGageData()->FlushForceDown_CheckOnly(); }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
		
		bool						FlushSoul( const float fFlushSoul_ ) { return GetGageData()->FlushSoul( fFlushSoul_ ); }

		void AddMPFactor( const float factor, const float time ) { GetGageData()->AddMPFactor( factor, time ); }
		void ClearMPFactor() { GetGageData()->ClearMPFactor(); }


		void SetChangeRateHp( const float changeRate ) { GetGageData()->SetChangeRateHp( changeRate ); }
		void SetChangeRateMp( const float changeRate ) { GetGageData()->SetChangeRateMp( changeRate ); }
		void SetChangeRateChargeMpForDetonation( const float changeRate ) { GetGageData()->SetChangeRateChargeMpForDetonation( changeRate ); }
		void SetNowChargeMpForDetonation( const float mpCharge ) { GetGageData()->SetNowChargeMpForDetonation( mpCharge ); }
		
		float GetChangeRateMp() { return GetGageData()->GetChangeRateMp(); }
				
		// TODO : UpHP() 함수와 합치자
		void UpHPIfAlive( const float hpIncrement, const float hpMinimum = 0.f ) 
		{
			if( GetNowHp() <= 0.f )
				return; 

			UpNowHp( hpIncrement, hpMinimum );
		}

		// TODO: rename UpHP() -> ChangeHPByDamage() or something 
		void UpNowHp( const float hpIncrement, const float hpMinimum = 0.f ) { GetGageData()->UpNowHp( hpIncrement, hpMinimum ); }
		void UpNowMp( const float mpIncrement ) { GetGageData()->UpNowMp( mpIncrement ); }
		void UpNowSoul( const float soulIncrement ) { GetGageData()->UpNowSoul( soulIncrement ); }

		void SetNowHp( float fHPNow );
		void SetNowMp( float fMPNow )			{ GetGageData()->SetNowMp( fMPNow ); }

		float GetMaxSoul() const { return GetGageData()->GetMaxSoul(); }
		void SetMaxSoul( const float maxSoul ) { GetGageData()->SetMaxSoul( maxSoul ); }

		float GetNowSoul() const { return GetGageData()->GetNowSoul(); }
		void SetNowSoul( const float soul ) { GetGageData()->SetNowSoul( soul ); }

		float GetNowDetonationGage() { return GetGageData()->GetNowChargeMpForDetonation(); }
		
		void SetNowForceDown( const float value ) { GetGageData()->SetNowForceDown( value ); }
		void SetMaxForceDown( const float value ) { GetGageData()->SetMaxForceDown(value); }
		
		void SetShowMPGage( bool bShow ) { ASSERT( !L"수정필요!" ); }//GetGageManager()->SetShowMPGage( bShow ); }
		bool GetShowMpGage() { ASSERT( !L"수정필요!" ); return true; } //{ return GetGageManager()->GetShowMPGage(); }

		void SetChangeRateByItem( const float fRate_, const float fTime_ )
		{ 
			GetGageData()->SetChangeRateByItem( fRate_ );
			GetGageData()->SetChangeRateTimeByItem( fTime_ );
		}

		void AddForceDownGage( const float fChangeOfNowForceDown_ ) { GetGageData()->UpNowForceDown( fChangeOfNowForceDown_ ); }

		const PhysicParam&			GetPhysicParam() const	{ return m_PhysicParam; }
		PhysicParam&				AccessPhysicParam()		{ return m_PhysicParam; }
		void						SetEnableAttackBox( const WCHAR* pName, bool bEnable );
		bool						SetEnableAttackBox( const CKTDXCollision::CollisionDataListSet& setCollisionDataList_, const WCHAR* pName_, const bool bEnable_ );

		void						SetEnableCollisionBox( const WCHAR* pName, bool bEnable );

		float GetSphereAttackBoxScale( const wstring& attackBoxName );
		void SetSphereAttackBoxScale( const wstring& attackBoxName, float fScale );


		const wstring&				GetHeadBoneName() const				{ return m_HeadBoneName; }
		virtual D3DXVECTOR3			GetHeadBonePos() const				{ return D3DXVECTOR3(0,0,0); }

		virtual void				SetStopTime( float fStopTime )		= NULL;
		virtual void				SetStop2Time( float fStopTime )		= NULL;

		virtual float				GetStopTime() = NULL;

#ifdef STOP_UNIT_STOP_EFFECT_TEST

		// @fRangeSq : 거리의 제곱입니다
		void SetEffectStopTime( float fTime, float fRangeSq = 1000.f );

#endif STOP_UNIT_STOP_EFFECT_TEST

		virtual int					GetNowStateID() const = NULL;
		float GetForceInvincible() { return m_fForceInvincibleTime; }
		float GetShowInvincible() { return m_fShowInvincibleTime; }
		virtual void				SetForceInvincible( float fTime ){ m_fForceInvincibleTime = fTime; }
		virtual void				SetShowInvincible( float fTime ){ m_fShowInvincibleTime = fTime; }

		virtual const UnitCondition& GetUnitCondition( bool bFuture = false ) const	= NULL;
		virtual const StateParam&	GetStateparam( bool bFuture = false ) const	= NULL;

		virtual int					GetLastTouchLineIndex( bool bFuture = false ) const = NULL;


		virtual void SetPositionOnLine( const D3DXVECTOR3& vPosition, const int iLineIndex ) = NULL;


		virtual const D3DXVECTOR3&	GetLandPos( bool bFuture = false ) const	= NULL;
		virtual const D3DXVECTOR3&	GetPos( bool bFuture = false ) const = NULL;
		virtual float				GetXPos( bool bFuture = false )	const	= NULL;
		virtual float				GetYPos( bool bFuture = false )	const	= NULL;
		virtual float				GetZPos( bool bFuture = false ) const		= NULL;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        virtual void BackupPosition() = 0;
        const D3DXVECTOR3&          GetPositionBackup() { if ( m_bPositionBackup == false ) BackupPosition(); return m_vPositionBackup; }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual void                OnFrameMove_PostProcess() = NULL;
        float                       GetElapsedTime() const              { return m_fElapsedTime; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		virtual const D3DXVECTOR3&	GetRotateDegree() const				= NULL;
		virtual float				GetXRotateDegree() const			= NULL;
		virtual float				GetYRotateDegree() const					= NULL;
		virtual float				GetZRotateDegree() const					= NULL;

		virtual float				GetPlaySpeed() const				= NULL;

		D3DXVECTOR3			        GetZVector() const
		{
			D3DXVECTOR3 vDirVector = GetDirVector();
			if( vDirVector.y != 0.f )
			{
				vDirVector.y = vDirVector.z;
				vDirVector.z = vDirVector.x;
				vDirVector.x = -vDirVector.y;
				vDirVector.y = 0.f;
				D3DXVec3Normalize( &vDirVector, &vDirVector );
			}
			else
			{
				vDirVector.y = vDirVector.z;
				vDirVector.z = vDirVector.x;
				vDirVector.x = -vDirVector.y;
				vDirVector.y = 0.f;
			}
			return vDirVector;
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual UINT    GetSizeOfAttackTimeList() const = NULL;
        virtual const D3DXVECTOR2* const GetAttackTimeByIndex( const UINT uiIndex_ ) const = NULL;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		UINT GetSizeOfAttackTimeList() const { return m_AttackTimeList.size(); }
		const D3DXVECTOR2* const GetAttackTimeByIndex( const UINT uiIndex_ )
		{
			if ( uiIndex_ < m_AttackTimeList.size() )
				return &m_AttackTimeList[uiIndex_];
			else
				return NULL;
		}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		virtual const D3DXVECTOR3&	GetDirVector() const						= NULL;
		virtual bool				GetIsRight() const						= NULL;
	
		virtual GAME_UNIT_STATE_ID	GetGameUnitState() const			= NULL;
		virtual bool				GetInvincible()	const				= NULL;
		virtual bool				GetIsFallDownState()				= NULL;

		virtual float				GetUnitWidth( bool bFuture ) const	= NULL;
		virtual float				GetUnitHeight( bool bFuture ) const	= NULL;

		bool						GetDyingStart() const { return m_bDyingStart; }

		virtual bool				IsMyUnit()	const		= NULL;
		virtual bool				IsLocalUnit() const		= NULL;
		virtual bool				IsFocusUnit() const		= NULL;
		virtual void				RenderName()		= NULL;

		void						DeleteComboManager() { SAFE_DELETE( m_pComboManager ); }
		CX2ComboManager*			GetComboManager() { return m_pComboManager; }
//{{ robobeg : 2013-11-04
		//CX2Stat*					GetStat() { return m_pStat; }
        const CX2Stat&				GetStat() const { return m_Stat; }
        CX2Stat&				    AccessStat() { return m_Stat; }
//}} robobeg : 2013-11-04
		void						MultiplyStat( float fFactor );

		bool						GetAttackedByUserUnit() { return m_bAttackedByUserUnit; }
		void						SetAttackedByUserUnit( bool bAttackedByUserUnit ) { m_bAttackedByUserUnit = bAttackedByUserUnit; }
		//UidType						GetAttackerUnitUID() { return m_AttackerUnitUID; }
		//void						SetAttackerUnitUID( UidType AttackerUnitUID ) { m_AttackerUnitUID = AttackerUnitUID; }

//#ifdef SERV_PET_SYSTEM
//		UidType						GetLastAttackUnitUID() { return m_LastAttackUnitUID; }
//		void						SetLastAttackUnitUID( UidType attackUnitUID ) { m_LastAttackUnitUID = attackUnitUID; }
//#endif
		CX2GameUnit*				GetGameUnitAttackedByMe() const { return ( null != m_optrGameUnitAttackedByMe ? m_optrGameUnitAttackedByMe.GetObservable() : NULL ); }
		void						SetGameUnitAttackedByMe( CX2GameUnit* pGameUnitAttackedByMe_ ) 
		{ 
			if ( NULL != pGameUnitAttackedByMe_ )
				m_optrGameUnitAttackedByMe = pGameUnitAttackedByMe_; 
		}
		void						ResetGameUnitAttackedByMe() { m_optrGameUnitAttackedByMe.Reset(); }

		CX2GameUnit*				GetGameUnitWhoAttackedMe() const { return ( null != m_optrGameUnitWhoAttackedMe ? m_optrGameUnitWhoAttackedMe.GetObservable() : NULL ); }
		void						SetGameUnitWhoAttackedMe( CX2GameUnit* pGameUnitWhoAttackedMe_ ) 
		{ 
			if ( NULL != pGameUnitWhoAttackedMe_ )
            {
#ifdef  X2OPTIMIZE_UNITTYPE_BUG_FIX
                m_bAttackedByUserUnit = pGameUnitWhoAttackedMe_->GetGameUnitType() == GUT_USER; 
#endif  X2OPTIMIZE_UNITTYPE_BUG_FIX
				m_optrGameUnitWhoAttackedMe = pGameUnitWhoAttackedMe_ ; 
            }//if
		}
		void						ResetGameUnitWhoAttackedMe() { m_optrGameUnitWhoAttackedMe.Reset(); }

		void						SetDamageData_LUA( const char* szTableNameUTF8 );

		CKTDXDeviceSound*			PlaySound( const WCHAR* pName, bool b3DSound = true, bool bLoop = false );
		void						PlaySound_LUA( const char* pName );
		void						StopSound_LUA( const char* pName );
		
		void						ClearHitUnitList_LUA()
		{ 
			m_DamageData.hitUnitList.resize(0); 
		}

		void						SetSpeedX( float fX ){ m_PhysicParam.nowSpeed.x = fX; }
		void						SetSpeedY( float fY ){ m_PhysicParam.nowSpeed.y = fY; }
		void						SetColor( D3DXCOLOR color ){ m_RenderParam.color = color; }
		D3DXCOLOR					GetColor() const { return m_RenderParam.color; }
		const CKTDGXRenderer::RenderParam& GetUnitRenderParam() const { return m_RenderParam; }

		D3DXVECTOR3					GetImpactPoint_LUA(){ return m_DamageData.impactPoint; }
		CX2GameUnit*				GetAttackerUnit_LUA(){ return ( null != m_DamageData.optrAttackerGameUnit ? m_DamageData.optrAttackerGameUnit.GetObservable() : NULL ); }

		virtual D3DXVECTOR3			GetCloneFramePosition( const WCHAR* boneName ) { return D3DXVECTOR3(0,0,0); }
		float						GetDieAfterTime(){ return m_fDieAfterTime; }
		
		virtual void				InitPosition( bool bUseTeamPos = true, int startPosIndex = -1 ) = NULL;
		bool						GetUpMPThisFrame(){ return m_bUpMPThisFrame; }
		void						SetUpMPThisFrame( bool bUpMPThisFrame ){ m_bUpMPThisFrame = bUpMPThisFrame; }

		float						GetChargeMpHitMeleeAttack() const { return m_AdditionalChargeMpHitMeleeAttack.GetResultValue(); }
		float						GetChargeMpHitRangeAttack() const { return m_AdditionalChargeMpHitRangeAttack.GetResultValue(); }

		float						GetChargeMpHitted() const { return m_AdditionalChargeMpHitted.GetResultValue(); }

		float						GetDistanceFrom( D3DXVECTOR3 vPos ) 
		{ 
			D3DXVECTOR3 vDist = vPos - GetPos();
			return D3DXVec3Length( &vDist ); 
		}
		float						GetXDistanceFrom( float fX ) { return fabs( fX - GetPos().x ); }
		void						SetUseWorldColor( bool bUseWorldColor ){ m_bUseWorldColor = bUseWorldColor; }


		bool						IsCanNotIntput() const;
		void						SetCanNotInputTime_LUA( float fCanNotInputTime ){ m_fCanNotInputTime = fCanNotInputTime; }
		void						SetCanNotInputTime_ZXArrow( float fCanNotInputTime ) { m_fCanNotInputTimeZXArrow = fCanNotInputTime; }
#ifdef MODIFY_DUNGEON_STAGING
		void						SetCanNotInput(bool bVal){m_bCanNotInput = bVal;}
#endif //MODIFY_DUNGEON_STAGING 



		void						SetDamageReducePercent( int damageReducePercent ) { m_DamageReducePercent = damageReducePercent; }
		int							GetDamageReducePercent() { return m_DamageReducePercent; }

        void                        SetDamageGaurdPercent( int damageGuardPercent ) { m_DamageGuardPercent = damageGuardPercent; }

#ifdef SERV_RENA_NIGHT_WATCHER
		void						SetExtraDamagePack( ExtraDamagePack *pExtraDamagePack );
#else  SERV_RENA_NIGHT_WATCHER
		void						SetExtraDamagePack( ExtraDamagePack *pExtraDamagePack ) { m_ExtraDamagePack = *pExtraDamagePack; }
#endif SERV_RENA_NIGHT_WATCHER

//{{ robobeg : 2013-11-04
		//ExtraDamagePack*			GetExtraDamagePack() { return &m_ExtraDamagePack; }
        const ExtraDamagePack&		GetExtraDamagePack() const { return m_ExtraDamagePack; }
        ExtraDamagePack&			AccessExtraDamagePack() { return m_ExtraDamagePack; }
//}} robobeg : 2013-11-04

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		//자신에게 걸린 무기 인챈트 타입을 반환
		const CX2DamageManager::EXTRA_DAMAGE_TYPE		GetApplyWeaponExtraDamageType();
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

		void						AddOnStatReq( KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT statType, float fStatVal, float fTime );
		void						SetAddOnStat( KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT statType, float fStatVal, float fTime );


		void									SetMajorMeshPlayer_LUA( int index, CKTDGXMeshPlayer::CXMeshInstance* pMeshPlayer );
		void									ClearMajorMeshPlayer_LUA( int index );
		CKTDGXMeshPlayer::CXMeshInstance*		GetMajorMeshPlayer_LUA( int index );
		CKTDGXMeshPlayer::CXMeshInstanceHandle	GetMajorMeshPlayerHandle( int index );

		void									SetMinorMeshPlayer_LUA( int index, CKTDGXMeshPlayer::CXMeshInstance* pMeshPlayer );
		void									ClearMinorMeshPlayer_LUA( int index );
		CKTDGXMeshPlayer::CXMeshInstance*		GetMinorMeshPlayer_LUA( int index );
		CKTDGXMeshPlayer::CXMeshInstanceHandle	GetMinorMeshPlayerHandle( int index );




		void								SetSubAttackList( CKTDGXSkinAnim* pXSkinAnim );
//{{ robobeg : 2013-11-04
		//CX2SocketItem::SocketData*		GetSocketData(){ return &m_SocketData; }
        const CX2SocketItem::SocketData&	GetSocketData() const { return m_SocketData; }
//}} robobeg : 2013-11-04
		CX2DamageManager::EXTRA_DAMAGE_TYPE GetEnchantExtraDamageType() { return m_eWeaponEnchantExtraDamageType; }		// fix!! rename GetWeaponEnchantExtraDamageType
		const CX2EnchantItem::EnchantData*	GetWeaponEnchantData( CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType ) const;
		
		
		virtual int							GetRandomInt( int iOffset = 0 ) = NULL;		// 0~99 사이의 값을 리턴
		virtual float						GetRandomFloat( int iOffset = 0 ) = NULL;	// 0~0.99999 사이의 값을 리턴

#ifdef NEW_SKILL_2010_11					// oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
		virtual float						GetAttackRateByAbnormalityBlind();
#endif NEW_SKILL_2010_11

		virtual float						GetCriticalPercent( BYTE byDamageType, float fExtraCritical = 0.f ) { return 0.0f; }
		virtual float						GetCriticalValue( float fDefenderCritical = 0.f ) { return GetCriticalAttack(); }

		//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
		/// 피격자가 크리티컬을 당할 기준 확률
		const float							GetCriticalValueFromDefender();
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
		//}} 2011-04 에 패치될 청 캐시 스킬

		virtual void						SetGameStat( const KStat& kGameStat_ );
		virtual void						SetGameStat( const CX2Stat::Stat& Stat_ );

		float								GetMaxHpValue() const { return m_AdditionalMaxHp.GetResultValue(); }
		float								GetPhysicAttackValue() const { return m_AdditionalPhysicAttack.GetResultValue(); }
		float								GetMagicAttackValue() const { return m_AdditionalMagicAttack.GetResultValue(); }
		float								GetPhysicDefenceValue() const { return m_AdditionalPhysicDefence.GetResultValue(); }
		float								GetMagicDefenceValue() const { return m_AdditionalMagicDefence.GetResultValue(); }
		float								GetAnimSpeedValue() const { return m_AdditionalAnimSpeed.GetResultValue(); }
		
		float								GetWalkSpeed() const { return m_AdditionalWalkSpeed.GetResultValue() * m_fRunJumpSpeedRate; }
		float								GetDashSpeed() const { return m_AdditionalDashSpeed.GetResultValue() * m_fRunJumpSpeedRate; }
		float								GetJumpSpeed() const { return m_AdditionalJumpPower.GetResultValue(); }
		float								GetDashJumpSpeed() const { return m_AdditionalDashJumpPower.GetResultValue(); }
		
		float								GetAccuracyPercent() const { return m_AdditionalAccuracy.GetResultValue(); }
		float								GetAvoidancePercent() const { return m_AdditionalAvoidance.GetResultValue(); }
		float								GetCriticalRate() const { return m_AdditionalCriticalRate.GetResultValue(); }
		float								GetCriticalAttack() const { return m_AdditionalCriticalAttack.GetResultValue(); }
		float								GetAntiCritialRate() const { return m_AdditionalAntiCriticalRate.GetResultValue(); }
		float								GetCriticalDefence() const { return m_AdditionalCriticalDefence.GetResultValue(); }
		float								GetAdditionalAttack() const { return m_AdditionalChangeAdditionalAttack.GetResultValue(); }
#ifdef FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER
		void								SetAdditionalAttack (const float AdditionalAttackDamage_ ) { m_AdditionalChangeAdditionalAttack.SetBasicStatValue(AdditionalAttackDamage_); }
#endif // FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		float								GetHyperChargeSpeed() const { return m_AdditionalHyperCharge.GetResultValue(); }
#endif //SERV_ADD_LUNATIC_PSYKER

		float								GetAdditionalDefence() const { return m_AdditionalChangeAdditionalDefence.GetResultValue(); }
		float								GetAdditionalFinalDamageMultiplierMeleeAttack() const { return m_AdditionalFinalDamageMultiplierMeleeAttack.GetResultValue(); }
		float								GetAdditionalFinalDamageMultiplierRangeAttack() const { return m_AdditionalFinalDamageMultiplierRangeAttack.GetResultValue(); }
		float								GetAdditionalFinalDefenceMultiplier() const { return m_AdditionalFinalDefenceMultiplier.GetResultValue(); }
		float								GetAdditionalForceDownMultiplierMeleeAttack() const { return m_AddtionalForceDownMultiplierMeleeAttack.GetResultValue(); }
		float								GetAdditionalForceDownMultiplierRangeAttack() const { return m_AddtionalForceDownMultiplierRangeAttack.GetResultValue(); }
		
		/// AttactResult에서 수행될 Delegate프로세스
		void								InsertDelegateProcessToAttackResult( const DelegateProcessWithDamageData& delegateProcess_ ) { m_vecDelegateProcessInAttackResult.push_back( delegateProcess_ ); }
		void								DoDelegateProcessInAttackResult( const CX2DamageManager::DamageData& damageData_ ) { DoDelegateProcessWithDamageData( m_vecDelegateProcessInAttackResult, damageData_ ); }

		/// DamageReact에서 수행될 Delegate프로세스
		void								InsertDelegateProcessToDamageReact( const DelegateProcess& delegateProcess_ ) { m_vecDelegateProcessInDamageReact.push_back( delegateProcess_ ); }
		void								DoDelegateProcessInDamageReact() { DoDelegateProcess( m_vecDelegateProcessInDamageReact ); }

		/// DamageReact 시에만 실행되는 델리게이트 모음(DamageData 인자 사용)
		void								InsertDelegateProcessWithDamageDataToDamageReact ( const DelegateProcessWithDamageData& delegateProcess_ ) { m_vecDelegateProcessWithDamageDataInDamageReact.push_back( delegateProcess_ ); }
		void								DoDelegateProcessWithDamageDataInDamageReact ( const CX2DamageManager::DamageData& damageData_ ) { DoDelegateProcessWithDamageData( m_vecDelegateProcessWithDamageDataInDamageReact, damageData_ ); }
		
		/// CommonFrameMove에서 수행될 Delegate프로세스
		void								InsertDelegateProcessToCommonFrameMove( const DelegateProcess& delegateProcess_ ) { m_vecDelegateProcessInCommonFrameMove.push_back( delegateProcess_ ); }
		void								DoDelegateProcessInCommonFrameMove() { DoDelegateProcess( m_vecDelegateProcessInCommonFrameMove ); }

		void								ClearAllDelegateProcess()
		{
			m_vecDelegateProcessInAttackResult.clear();
			m_vecDelegateProcessInDamageReact.clear();
			m_vecDelegateProcessInCommonFrameMove.clear();
			m_vecDelegateProcessWithDamageDataInDamageReact.clear();
		}

		virtual void						ChangeModelDetail( int detailPercent ) = 0;

		float GetScale() const { return m_fScale; }

		const D3DXVECTOR2& GetOrgUnitSize() { return m_vOrgUnitSize; }


		virtual void						ShowSnatchCureEffect() {}

		virtual float						GetDamageUpPercentBySMA( int npcID ) { return 0; }
		virtual float						GetEvadeUpPerBySMA( int npcID ) { return 0; }

		int							GetWeaponCount() { return (int) m_vecpWeapon.size(); }
		D3DXVECTOR3					GetWeaponBoneRot( int iWeaponIndex, int iModelIndex );
		D3DXVECTOR3					GetWeaponBonePos( int iWeaponIndex, const WCHAR* pBoneName, int iModelIndex ) const;
		D3DXVECTOR3 				GetWeaponBonePos_LUA( int iWeaponIndex, const char* pBoneName );
		void                        SetWeaponEnable(UINT index, bool bEnable)
		{
			if( index < 0 || index >= m_vecpWeapon.size() )
				return;

			m_vecpWeapon[index]->SetEnable(bEnable);
		}

#ifdef RIDING_SYSTEM
		CX2RidingPetPtr GetRidingPet()
		{
			return m_ptrRidingPet;
		}

		virtual bool GetRidingOn() const { return false; }
#endif //RIDING_SYSTEM

		void AddWeaponAttackCollisionBoxForNPC( Weapon* pWeapon );
		void RemoveAllWeaponAttackCollisionBoxForNPC();

		void AttachWeaponParticle( int iWeaponIndex, bool bIsMajorParticle, const wstring& wstrBoneName, 
			CKTDGParticleSystem::CParticleEventSequenceHandle hSeq );
		void AttachWeaponParticle_LUA( int iWeaponIndex, bool bIsMajorParticle, char* pBoneName, 
			CKTDGParticleSystem::CParticleEventSequenceHandle hSeq );

		bool GetInvisibility() const { return m_bInvisibility; }
		bool GetAbsoluteInvisibility() const { return m_bAbsoluteInvisibility; }




		// dmlee 2009.5.9 이브 블랙홀 필살기 관련 오류 수정 - 원래는 displace 수치를 누적하도록 했었는데, 시간멈춤이 있는 경우에 비정상적으로 수치가 누적되는 문제가 있어서 값을 assign 하는 것으로 수정했습니다.
		void SetDisplaceUnitPosition( const D3DXVECTOR3& vDisplace )
		{
			m_vDisplaceUnitPosition = vDisplace;
		}
		void ClearDisplaceUnitPosition()
		{
			m_vDisplaceUnitPosition = D3DXVECTOR3( 0, 0, 0 );
		}

		void SetFixedUnitPosition(const D3DXVECTOR3& val) { m_vFixedUnitPosition = val; }
		void ClearFixedUnitPosition() { m_vFixedUnitPosition = D3DXVECTOR3(0, 0, 0); }



		CX2DamageManager::DamageData& GetDamageData() { return m_DamageData; }
		void SetExtraDamageDataTime( float fTime ) // 레나 인탱글 트랩을 위한 예외처리 코드 
		{
			if( CX2DamageManager::EDT_ENTANGLE == m_DamageData.m_ExtraDamage.m_ExtraDamageType )
			{
				m_DamageData.m_ExtraDamage.m_fTime = fTime;
			}
		}

		
		
		// 더블어택
		void SetDoubleAttack(wstring wstrHitSound, float fDamage, D3DXVECTOR3 vimpactPoint);


		virtual float GetPowerRate() = NULL;



#ifdef GRAPPLING_TEST
		virtual void CalcGrapplingPosition( bool bFuture ) = NULL;
		virtual void ResetGrapplingState( bool bOnlyMe ) = NULL;

		virtual void SetUnitGrapplingMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID ) = NULL;
		virtual void SetUnitGrappledByMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID ) = NULL;

		virtual CX2GameUnit* GetUnitGrapplingMe( bool bFuture ) = NULL;
		virtual CX2GameUnit* GetUnitGrappledByMe( bool bFuture ) = NULL;


		virtual bool IsGrappled( bool bFuture ) = NULL;

		
#endif GRAPPLING_TEST



//#ifdef STEP_ON_MONSTER_TEST
//		bool CanBeSteppedOn() const { return m_bCanBeSteppedOn; }
//		void SetCanBeSteppedOn(const bool val) { m_bCanBeSteppedOn = val; }
//#endif STEP_ON_MONSTER_TEST


//#ifdef STEP_ON_MONSTER_COLLISION_BOX_TEST
//		bool CanBeSteppedOnCollsionBox() const { return m_bCanBeSteppedOnCollsionBox; }
//		void SetCanBeSteppedOnCollsionBox(const bool val) { m_bCanBeSteppedOnCollsionBox = val; }
//#endif STEP_ON_MONSTER_COLLISION_BOX_TEST

		//{{ kimhc // 실시간 엘소드 중 실시간 장비 교체
#ifdef REAL_TIME_ELSWORD
		void DeleteEnchantWeaponEffectUnitSpecific( int index );
#endif REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 실시간 장비 교체

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		virtual void LoadEffectSetFromScript( IN KLuaManager& luaManager );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		void MakeEffectSetBeDeletedWhenDie( const CX2EffectSet::Handle& hEffectSet_ );

		void DeleteEffectSetOnStateEnd();
		void DeleteEffectSetOnDamageReact();
		void DeleteEffectSetOnDie();
#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
		void DeleteEffectSetOnCustomState();
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE
		void PushEffectSetToDeleteOnDie( const CX2EffectSet::Handle& hEffectSet_ ) { m_vecEffectSetToDeleteOnDie.push_back(hEffectSet_); }

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void CommonFrameMove_EffectSet( float fNowAnimationTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		void AddEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect );
		void RemoveEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect );
        bool IsEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		void AddEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect );
		void RemoveEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect );
        bool IsEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		void ClearEffectiveBlackHoleDamageEffect();
		bool IsThereEffectiveBlackHoleDamageEffect();

		virtual int GetUnitLevel() const { return 0; }
		//virtual int GetUnitLevel() = 0;

#ifdef SKILL_30_TEST

		//const CX2GameUnit::SPEED_FACTOR& GetMoveSpeedFactor() const { return m_PhysicParam.m_MoveSpeedFactor; }
		void SetMoveSpeedFactor( float fSpeedRate, float fDuration )
		{ 
// 			m_PhysicParam.m_MoveSpeedFactor.m_fSpeedRate = fSpeedRate;
// 			m_PhysicParam.m_MoveSpeedFactor.m_fDuration = fDuration;
		};


		//const CX2GameUnit::SPEED_FACTOR& GetJumpSpeedFactor() const { return m_PhysicParam.m_JumpSpeedFactor; }
		void SetJumpSpeedFactor( float fSpeedRate, float fDuration )
		{ 
// 			m_PhysicParam.m_JumpSpeedFactor.m_fSpeedRate = fSpeedRate;
// 			m_PhysicParam.m_JumpSpeedFactor.m_fDuration = fDuration;
		};

#endif SKILL_30_TEST

#ifdef SERV_SKILL_NOTE
		void ClearAnimSpeedFactor();
		void SetAnimSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId = CX2GameUnit::SFI_NONE, SPEED_FACTOR_SET_TYPE eSetType = SFST_MULTY );
#endif

		void ClearMoveSpeedFactor();
		void ClearJumpSpeedFactor();
		void SetVecMoveSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId = CX2GameUnit::SFI_NONE, SPEED_FACTOR_SET_TYPE eSetType = SFST_MULTY );
		void SetVecJumpSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId = CX2GameUnit::SFI_NONE, SPEED_FACTOR_SET_TYPE eSetType = SFST_MULTY );
		void RemoveVecMoveSpeedFactor( SPEED_FACTOR_ID eID );

		virtual bool CanApplyBuffToGameUnit() const;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual const std::vector<BUFF_TEMPLET_ID>& GetVecImmunityAtThisState() const = NULL;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool ApplyBuffToGameUnitInDamageReact( const CX2DamageManager::DamageData* pDamageData_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		virtual bool SetBuffFactorToGameUnitByBuffFactorID( const UINT uiBuffFactorId_, IN const UINT uiLevel_ = 1 );
#else //UPGRADE_SKILL_SYSTEM_2013
		virtual bool SetBuffFactorToGameUnitByBuffFactorID( const UINT uiBuffFactorId_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef ADD_LIVE_CREATOR_FINALIZER
	#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		virtual bool SetBuffFactorToGameUnitByBuffFactorIDAndCreatorID( const UINT uiBuffFactorId_, const UidType iCreatorID_,
			const bool bIsUserID_ = true, IN const UINT uiLevel_ = 1  );
	#else //UPGRADE_SKILL_SYSTEM_2013
		virtual bool SetBuffFactorToGameUnitByBuffFactorIDAndCreatorID( const UINT uiBuffFactorId_, const UidType iCreatorID_, const bool bIsUserID_ = true );
	#endif //UPGRADE_SKILL_SYSTEM_2013
#endif ADD_LIVE_CREATOR_FINALIZER
		virtual bool SetBuffFactorToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const UINT uiIndex_ );
		virtual bool SetBuffFactorToGameUnit( const CX2BuffFactor& buffFactor_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		virtual bool SetBuffFactorToDamageData( const UINT uiBuffFactorId_, IN const UINT uiLevel_ = 1 );
#else //UPGRADE_SKILL_SYSTEM_2013
		virtual bool SetBuffFactorToDamageData( const UINT uiBuffFactorId_ );
#endif //UPGRADE_SKILL_SYSTEM_2013
		
		bool GetBuffFinalizerTempletPtrList( const BUFF_TEMPLET_ID eBuffTempletID_, OUT vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ );
		void ChangeBuffFinalizerTempletPtrList( const BUFF_TEMPLET_ID eBuffTempletID_, OUT vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ );

		void PushNewBuffTempletToGameUnit( IN CX2BuffTempletPtr ptrBuffTemplet_, const CX2BuffFactor& buffFactor_, const UINT uiUniqueNum_ );
		void EraseBuffTempletFromGameUnit( const BUFF_TEMPLET_ID eBuffTempletID_ );
		void EraseBuffTempletFromGameUnitByIterator( vector<CX2BuffTempletPtr>::iterator vItr_ );
		void ReserveToFinishBuffTempletFromGameUnit( const BUFF_TEMPLET_ID eBuffTempletID_ );

		bool HaveThisBuff( const BUFF_TEMPLET_ID eBuffTempletID_ );

		UINT GetAppliedBuffCount( const bool bIsDeBuff_ ) const;

		/// 각 유닛마다 특정 버프가 실행 또는 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 등...)
		virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ ) {}
		virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ ) {}

//{{ kimhc // 2010.4.2 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
		float	GetStatUpBufferBySocketItem() const;
		CX2SocketItem::STAT_UP_TYPE GetStatUpType() const { return m_StatUpType; }
		void	SetStatUpBufferBySocketItem( float fVal, float fTime )
		{
			m_StatUpBufferBySocketItem.m_fBufferRate = fVal;
			m_StatUpBufferBySocketItem.m_fBufferTime = fTime;
		}
		void	ProcessStatPercentUpBuffer( CX2GameUnit* pAttacker, OUT CX2Stat &AttackerStat, OUT CX2Stat &DefenderStat );
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.2 // 비밀던전 작업(셋트아이템효과)
		float GetGroggyTime() const { return m_fGroggyTime; }
		void SetGroggyTime(const float val) { m_fGroggyTime = val; }
		
		bool GetDrainMana() { return m_bHitedDrainMana; }



		float GetNoSkillTime() const { return m_fNoSkillTime; }
		void AddNoSkillTime(const float val) { m_fNoSkillTime += val; }


		const CX2GameUnit::SkillRelatedData& GetSkillRelatedData() const { return m_SkillRelatedData; }


#ifdef RESET_INVALID_UNIT_POSITION_TEST

		bool IsMisplaced() 
		{
			const float MAGIC_TIME = 3.f;
			if( true == m_bMisplaced && m_TimerMisplaced.elapsed() > MAGIC_TIME )
				return true;
			else
				return false;
		}

#endif RESET_INVALID_UNIT_POSITION_TEST


		bool IsImmuneToExtraDamage() const { return m_bImmuneToExtraDamage; }


		const CX2GameUnit::CommonState& GetCommonState() const { return m_CommonState; }



//#ifdef TRANSFORMER_TEST
//		bool GetTransformed() const { return m_bIsTransformed; }
//		void SetTransformed(const bool val) { m_bIsTransformed = val; }
//
//		UidType GetTransformerOwnerUID() const { return m_TransformerOwnerUID; }
//		void SetTransformerOwnerUID(const UidType val) { m_TransformerOwnerUID = val; }
//
//#endif TRANSFORMER_TEST

#ifdef RAVEN_SECOND_CLASS_CHANGE
		void ChangeDamageType(CX2DamageManager::DAMAGE_TYPE eDamageType, float fDamageRel)
		{
			m_bChangeDamageType = true;
			m_eDamageType = eDamageType;
			m_fDamageRel = fDamageRel;
		}
		void ResetDamageType()
		{
			m_bChangeDamageType = false;
			m_eDamageType = CX2DamageManager::DT_PHYSIC;
			m_fDamageRel = 1.f;
		}
		bool GetChangeDamageType(CX2DamageManager::DAMAGE_TYPE &eDamageType, float &fDamageRel)
		{
			eDamageType = m_eDamageType;
			fDamageRel = m_fDamageRel;
			return m_bChangeDamageType;
		}
		void ChangeDamageTimes(int iDamageTimes) { m_iDamageTimes = iDamageTimes; }
		void ResetDamageTimes() { m_iDamageTimes = -1; }
		int  GetDamageTimes() { return m_iDamageTimes; }
#endif
		
		/** @function SetShieldDamageReduce
			@brief 쉴드의 데미지 감소 능력
		*/
        //{{ JHKang / 강정훈 / 2010/12/07
#ifdef NEW_SKILL_2010_11
        void SetShieldDamageReduce( IN float fValue_ ) { m_fAtomicShieldDamageRate = fValue_; }
#endif NEW_SKILL_2010_11
        //}} JHKang / 강정훈 / 2010/12/07

		void SetLockOnNearTarget_LUA( CX2DamageEffect::CEffect* pCEffect, int randomOffset = 0 );

#ifdef CHUNG_SECOND_CLASS_CHANGE
		// oasis907 : 김상윤 [2011.6.16] 청 2차 전직 - 아이언 팔라딘
		void ProcessTransitDebuff( const CX2DamageManager::DamageData* pDamageData_, CX2GameUnit* pTargetUnit_ );
		bool GetEnableWeaponRenderEffect(){ return m_bEnableWeaponRenderEffect; }
		void SetEnableWeaponRenderEffect( bool _bVal );
		D3DXCOLOR GetWeaponRenderEffectColor(){ return m_WeaponRenderEffectColor; }
		void SetWeaponRenderEffectColor( const D3DXCOLOR& _Color){ m_WeaponRenderEffectColor = _Color; }

		void FadeInWeaponRenderEffect( const D3DXCOLOR& _StartColor, const D3DXCOLOR& _EndColor,  float _fChangeTime, float _fDuration);
		void PulseWeaponRenderEffect( const D3DXCOLOR& _StartColor, const D3DXCOLOR& _EndColor, float _fPeriod, float _fDuration, PULSE_WEAPON_COLOR_ID _ePulseWeaponColorID = PWCI_UNDEFINED);

		void FrameMoveWeaponRenderEffect();

#endif CHUNG_SECOND_CLASS_CHANGE

		//{{ kimhc // 2011-07-19 // 옵션데이타 수치화 작업
		// 옵션 수치화에 의한 추가 데미지
#ifdef ELSWORD_SHEATH_KNIGHT
		virtual float			GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData )
#else
		virtual float			GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData ) const
#endif ELSWORD_SHEATH_KNIGHT
		{ return 0.0f; }
		//}} kimhc // 2011-07-19 // 옵션데이타 수치화 작업


#ifdef SERV_PVP_NEW_SYSTEM
		bool IsPvpBot() const { return m_bPvpBot; }
		virtual wstring GetUnitName() { return L""; }
#endif

#ifdef VELDER_SECRET_DUNGEON
		void SetRestoreScale() { m_fTimeToReturnBasicScale = 0.f; }
#endif
#ifdef HIT_PARTICLE_TRACE_UNIT
		void AddTraceUnitHitParticle(CKTDGParticleSystem::CParticleEventSequenceHandle hParticle_){ m_listSeqHitParticleTraceUnit.push_back(hParticle_); }
#endif HIT_PARTICLE_TRACE_UNIT

#ifdef SERV_ARME_DIMENSION_WITCH
		void SetHitCreateDamageEffectRate( float fVal ) { m_fHitCreateDamageEffectRate = fVal; }
		float GetHitCreateDamageEffectRate() { return m_fHitCreateDamageEffectRate; }
		void SetHitCreateDamageEffectPowerRate( float fVal ) { m_fHitCreateDamageEffectPowerRate = fVal; }
		float GetHitCreateDamageEffectPowerRate() { return m_fHitCreateDamageEffectPowerRate; }
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		void		 SetStartOfDelayedFiringEmoticon();								/// 이모티콘 설정
		void		 CheckDamageEffectType( CX2DamageEffect::CEffect* pCEffect );	/// 데미지 이펙트 타입
		CX2GameUnit* GetGameUnitByUnitIndex( int iUnitIndex );						/// 인덱스에 해당하는 유닛 반환
		void		 ClearStartOfDelayedFiringData();								/// 지연의 신호탄 정보 삭제
		const vector<CX2DamageManager::StartOfDelayedFiringData*>& GetStartOfDelayedFiringData() const		/// 지연의 신호탄 저장 벡터 반환
		{ 
			return m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData; 
		}
		vector<CX2DamageManager::StartOfDelayedFiringData*>& AccessStartOfDelayedFiringData()		/// 지연의 신호탄 저장 벡터 반환
		{ 
			return m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData; 
		}
		void		 CommonFrameMoveStateAbnormalityStartOfDelayedFiring();			/// 지연의 신호탄 실시간 처리
		void		 SetOverlapEDTEmoticon( CKTDGParticleSystem::CParticleEventSequenceHandle& hEmoticon, int iActiveEDTNum );		/// 중첩된 EDT에 대한 중첩 횟수 이모티콘 설정
#endif SERV_RENA_NIGHT_WATCHER

		void		DownCrashCamera( const float fSpeed_ = -100.0f, const float fAccel_ = 1500.0f ) const;
		void		UpCrashCamera( const float fSpeed_ = 100.0f, const float fAccel_ = -1500.0f ) const;
		void		UpDownCrashCamera( const float fGap_ = 10.0f, const float fTime_ = 0.2f ) const;
		void		UpDownCrashCameraNoReset( const float fGap_ = 10.0f, const float fTime_ = 0.2f ) const;
		void		LeftRightCrashCameraNoReset( const float fGap_ = 10.0f, const float fTime_ = 0.2f ) const;
		void		ShakeLeftRight( const float fGap_ = 10.f, const float fTime_ = 0.2f, const float fTimeGap_ = 0.05f ) const;

		virtual void	UpNowMpPerHitOthers( const float fHitAddMp_, const int iAddValueByTeam_, const float fRel_ );	// 근접공격 성공시 엠회수 처리
		void			UpNowMpPerHitOthersWithDamageEffect( const float fHitAddMp_, const int iAddValueByTeam_, const float fRel_ );	// 이펙트 공격 성공시 엠회수 처리

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		void	UpNowHpPerHitOthers( IN const float fHitAddHpPer_ );	/// 공격 성공시, 공격자의 체력 회복 처리
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef ADD_MEMO_1ST_CLASS //김창한
		void	UpNowHpPerHitbyAttackPower( IN const float fHitAddHpPer_ );	/// 공격 성공시, 공격자의 체력 회복 처리( 공격자의 물마공 평균에 비례 )
#endif //ADD_MEMO_1ST_CLASS

 		/// 좌우반전
		void CreateAndInsertReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ );
		void ToggleReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ );
		void EraseReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ );

		/// 현재위치 고정
#ifdef NEVER_MOVE_GRAVITY_APPLY
		void CreateAndInsertNeverMoveByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vPos_, const bool vGravity );
#else
		void CreateAndInsertNeverMoveByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vPos_ );
#endif //NEVER_MOVE_GRAVITY_APPLY	
		void EraseNeverMoveByBuff( const KBuffIdentity& BuffIdentity_ );

		/// BuffIdentity에 의해 구분되는 효과중 멤버변수가 필요없는 것들(대시불가, 공격불가, 수퍼아머 등)
		void CreateAndInsertBuffIdentity( const BUFF_BEHAVIOR_TYPE eType_, const KBuffIdentity& BuffIdentity_ );
		void EraseBuffIdentity( const BUFF_BEHAVIOR_TYPE eType_, const KBuffIdentity& BuffIdentity_ );

		/// BuffIdentity에 의해 구분되는 효과 중 StateStart, CommonFrameMove, StageEnd에서 처리해야하는 하드코딩을 수행
		void CreateAndInsertCustomFunctionByBuff( const KBuffIdentity& BuffIdentity_, const KBuffBehaviorFactor& kBehaviorFactor_ );
		void EraseCustomFunctionByBuff( const KBuffIdentity& BuffIdentity_ );
		void DoCustomFunctionByBuffInCommonStateStart();
		void DoCustomFunctionByBuffInCommonFrameMove();
		void DoCustomFunctionByBuffInCommonStateEnd();
		void DoCustomFunctionByBuffInDamageReact( IN CX2DamageManager::DamageData& damageData_ );

		/// BuffIdentity에 의해 구분되는 효과 중 AfterImage 관련 함수
		void CreateAndInsertAfterImageByBuff( const KBuffIdentity& BuffIdentity_, const D3DXCOLOR& d3dxColor_ );
		void EraseAfterImageByBuff( const KBuffIdentity& BuffIdentity_ );
		void EnableAfterImage();
		void DisableAfterImage( const bool bForce_ = false );

		virtual bool IsSuperArmor() const = NULL;
		bool ShouldAttackAllTeam() const { return !m_vecAttackAllTeamPtr.empty(); }

		/// 버프에 의한 RenderParam 변경
		void CreateAndInsertRenderParamByBuff( const KBuffIdentity& BuffIdentity_, const StBuffRenderParam& stBuffRenderParam_ );
		virtual void CreateAndInsertRenderParamByBuffUnitType( const KBuffIdentity& BuffIdentity_, const map<CX2Unit::UNIT_TYPE, StBuffRenderParamPtr>& mapStBuffRenderParamPtr_ ) = NULL;
		void EraseRenderParamByBuff( const KBuffIdentity& BuffIdentity_ );

		/// 버프에 걸린 경우, 타격시 타격점에 출력될 이펙트셋(무조건 스크립트에서 자동종료시켜야함)
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
		void CreateAndInsertEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCreateGap );
#else // SERV_NEW_DEFENCE_DUNGEON
		void CreateAndInsertEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_ );
#endif // SERV_NEW_DEFENCE_DUNGEON
		void EraseEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_ );
		void PlayEffectSetImpactPointByBuff();

		/// 버프에 걸린 경우, 피격시 출력될 이펙트셋(무조건 스크립트에서 자동종료시켜야함)
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
		void CreateAndInsertEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCreateGap );
#else // SERV_NEW_DEFENCE_DUNGEON
		void CreateAndInsertEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_ );
#endif // SERV_NEW_DEFENCE_DUNGEON
		void EraseEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_ );
		void PlayEffectSetHittedByBuff();

		/// 버프에 의한 크기 변경
		void CreateAndInsertChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vScale_ );
		void EraseChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const bool bUseTimeToReturn_ );
		
#ifdef VERIFY_STAT_BY_BUFF
		virtual void TransformScale( const PROTECT_VECTOR3& vScale_ ) = NULL;
#else	// VERIFY_STAT_BY_BUFF
		virtual void TransformScale( const D3DXVECTOR3& vScale_ ) = NULL;
#endif // VERIFY_STAT_BY_BUFF
		
		void TransformScale_LUA( const float fScale_, const float fTime_ );
		void ProcessReturnFromScaleChange();
		bool IsChangedGameUnitScaleByBuff() const;	/// 변형되어 있는 상태인가?
#ifdef POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE
		void UpdateChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vScale_, const UINT uiAccumulationLimit_ );
		D3DXVECTOR3  GetChangeUnitScaleByBuff();
#endif POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE

		/// 기절
		void CreateAndInsertStunByBuff( const KBuffIdentity& BuffIdentity_ );
		void EraseStunByBuff( const KBuffIdentity& BuffIdentity_ );

		/// 동결
		void CreateAndInsertFreezeByBuff( const KBuffIdentity& BuffIdentity_ );
		void EraseFreezeByBuff( const KBuffIdentity& BuffIdentity_ );

		/// 스탯변경
		void InsertAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fValue_ );
		void UpdateAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fValue_ );
		void EraseAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_ );
		
		void CheckAndUpdateAdditionalStat();

		void InitAdditionalStatOptionValue();
		void InitAdditionalBasicStatValue();
		void InitAdditionalPassive();
		void InitAdditionalPassiveMultipleAndSum(CX2AdditionalStatByBuff& Additional_ );
		virtual void InitAddtionalDashAndJumpBasicStatValue();

		const vector<CX2BuffFactorPtr>&					GetBuffFactorList() const { return m_vecBuffFactorPtr; }
		void CreateAndInsertSkillAndSocketBuffFactorToList( CX2BuffFactorPtr ptrBuffFactor_ );

		/// 일시적인 BuffFactor
		const vector<CX2BuffFactorPtr>&					GetTemporaryBuffFactorList() const { return m_vecTemporaryBuffFactorPtr; }
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		void CreateAndInsertTemporaryBuffFactor( const CX2BuffFactor& BuffFactor_, const KBuffIdentity& BuffIdentity_, UINT uiLevel );
#else //UPGRADE_SKILL_SYSTEM_2013
		void CreateAndInsertTemporaryBuffFactor( const CX2BuffFactor& BuffFactor_, const KBuffIdentity& BuffIdentity_ );
#endif //UPGRADE_SKILL_SYSTEM_2013
		void EraseTemporaryBuffFactor( const KBuffIdentity& BuffIdentity_ );


		CKTDGXSkinAfterImage* GetAfterImage() const { return m_pAfterImage; }

		void SetBasicStatValueChargeMpHitMeleeAttack( const float fValue_ ) { m_AdditionalChargeMpHitMeleeAttack.SetBasicStatValue( fValue_ ); }
		void CheckAndUpdateAdditionalChargeMpHitMeleeAttack()		{ m_AdditionalChargeMpHitMeleeAttack.CheckAndUpdateResultValue(); }

		void SetBasicStatValueChargeMpHitRangeAttack( const float fValue_ ) { m_AdditionalChargeMpHitRangeAttack.SetBasicStatValue( fValue_ ); }
		void CheckAndUpdateAdditionalChargeMpHitRangeAttack()		{ m_AdditionalChargeMpHitRangeAttack.CheckAndUpdateResultValue(); }

		void SetBasicStatValueChargeMpHitted( const float fValue_ ) { m_AdditionalChargeMpHitted.SetBasicStatValue( fValue_ ); }
		void CheckAndUpdateAdditionalChargeMpHitted()	{ m_AdditionalChargeMpHitted.CheckAndUpdateResultValue(); }

		/// 이펙트공격흡수
		void CreateAndInsertAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_, const float fAbsorbValue_ );
		void UpdateAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_, const float fAbsrobValue_ );
		void EraseAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_ );

		bool EmptyAbsorbEffectAttackList() const { return m_vecAbsorbEffectAttackPtr.empty(); }
		bool AbsorbEffectAttackProcess( CX2DamageManager::DamageData* pDamageData_ );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		virtual bool DidReceiveFirstSyncPacket() { return true; };		/// 해당 게임유닛의 첫 싱크패킷을 받았는지 여부를 알아보는 함수
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual bool DidReceiveFirstSyncPacket() const = NULL;		/// 해당 게임유닛의 첫 싱크패킷을 받았는지 여부를 알아보는 함수
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		/// 공격 타입에 따른 데미지 변경
		void CreateAndInsertChangeAttackByType( const KBuffIdentity& BuffIdentity_, CX2DamageManager::ATTACK_TYPE eAttackType_, float fValue_ );
		void EraseChangeAttackByType( const KBuffIdentity& BuffIdentity_ );
		float GetAdditionalAttackByType( CX2DamageManager::ATTACK_TYPE eAttackType_ );

		/// 소켓 속성 공격 발동 확률 변경
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 인챈트 속성 타입과 변경 타입 추가
		void CreateAndInsertChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_, float fRate_, BUFF_ENCHANT_ATTRIBUTE_TYPE eEnchantAttributeType_ = BEAT_ALL, BUFF_CHANGE_TYPE eBuffChangeType_ = BCT_PERCENT );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		void CreateAndInsertChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_, float fRate_ );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		
		
		
		void EraseChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_ );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 인챈트 공격 확률 계산에 사용되는 구조체
		// 합 연산 값, 곱 연산 값, 연산 타입 ( 버프 체인지 타입 ) 에 대해 묶어둠 
		struct EnchantAttackRate
		{
			EnchantAttackRate () : fIncreaseAttackRate ( 0.f ), eBuffchangeType ( BCT_PERCENT ), fIncreaseAttackRateFixValue ( 0.f ) {}
			EnchantAttackRate ( float IncreaseAttackRate_ , float IncraseAttackRateFixValue_, BUFF_CHANGE_TYPE BuffchangeType_ ) : 
				fIncreaseAttackRate ( IncreaseAttackRate_ ),fIncreaseAttackRateFixValue ( IncraseAttackRateFixValue_ ), eBuffchangeType ( BuffchangeType_ ) {}

			float				fIncreaseAttackRate;				// 곱 연산 값
			float				fIncreaseAttackRateFixValue;		// 합 연산 값
			BUFF_CHANGE_TYPE	eBuffchangeType;					// 버프 변경 타입
		};
		EnchantAttackRate GetChangeEnchantAttackRate( CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageType );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		float GetChangeEnchantAttackRate();
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		/// 더블 어택
		void CreateAndInsertDoubleAttackByBuff( const KBuffIdentity& BuffIdentity_, float fRate_ );
		void EraseDoubleAttackByBuff( const KBuffIdentity& BuffIdentity_ );
		bool IsDoubleAttackByBuff();

		void SetKBuffFactorPacketToGameUnit( const vector<KBuffFactor>& vecBuffFactor_ );
		
		/// 필드<->던전 이동 시 자신에게 걸린 버프를 GageManager에 임시로 저장 했던 것을 다시 GameUnit에 적용하는 함수
		void SetKBuffFactorFromGageManagerToGameUnit();

		bool IsBuffTempletListEmpty() { return m_vecBuffTempletPtr.empty(); }	/// 버프 또는 디버프에 걸렸는지 여부를 알아보는 함수

		virtual void AddUnitSlashData( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ )
		{}

		/// 실행되어야 하는 UnitSlashTrace의 종류를 지정
		void SetUnitSlashTraceConditionToDo( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ ) { m_eUnitSlashTraceConditionToDo = eSlashTraceCondition_; }
		const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION GetUnitSlashTraceConditionToDo() const { return m_eUnitSlashTraceConditionToDo; }

#ifdef UPGRADE_SKILL_SYSTEM_2013
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_, const float fCoolTime = 0.f, bool bForceSet = true, BUFF_CHANGE_TYPE eChangeType = BCT_RELATION_VALUE, const float fCoolTimeRate = 1.f, const bool bIsRelativeAllSkill = false ) {}
#else // UPGRADE_SKILL_SYSTEM_2013
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_ ) {}
#endif // UPGRADE_SKILL_SYSTEM_2013

		void InsertPairSubAttackListSet( const CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshHandle_, const CKTDGXSkinAnim* pSkinAnim_ );
		void ErasePairSubAttackListSet( const CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshHandle_ );
		bool GetSubAttackListGetFromPair( const UINT uiIndex_, OUT const CKTDXCollision::CollisionDataListSet** ppCollisionDataListSet_ );
		UINT GetSizeOfPairSubAttackListSet() const { return m_vecPairSubAttackListSet.size(); }

		bool GetNeedToUpdateWeaponEffect() const { return m_bNeedToUpdateWeaponEffect; }
		void SetNeedToUpdateWeaponEffect( bool bNeedToUpdateWeaponEffect_ ) { m_bNeedToUpdateWeaponEffect = bNeedToUpdateWeaponEffect_; }

		virtual void UpdateWeaponEnchantEffectAndExtraDamageType() {}
		virtual void GuildSkillProcess( const CX2SkillTree::SKILL_ID eSkillID_ ) {}

		void GetBuffFactor( OUT vector<KBuffFactor>& vecBuffFactor_ ) const;
		void GetBuffFactorOnlyDebuff( OUT vector<KBuffFactor>& vecBuffFactor_ ) const;
		bool IsApplyBuffByBuffTempletID( BUFF_TEMPLET_ID eBuffTempletID_ ) const;

		virtual bool GetShowOnMiniMap() const;
		virtual bool IsGateNpc() const { return false; }
		
		//레이븐 용병의 처신술 패시브(SI_P_RWT_BEHAVIOR_OF_MERCENARY) 효과를 적용시키기 위해 추가한 함수
		virtual float GetReduceDebuffDurationRate(BUFF_TEMPLET_ID eBuffTempletID_){ return 1.f; }

#ifdef SERV_NEW_DEFENCE_DUNGEON  // 적용날짜: 2013-03-23
		/// 스크립트 내에서 트레이스 로그 출력을 해주는 함수
		void WriteTraceLog_LUA( const char* wstrLog_ ) 
		{ 
			wstring wstrStateName = L"";
			ConvertUtf8ToWCHAR( wstrStateName, wstrLog_ );

			wstrStateName += L"\n";

			DXUTTRACE( wstrStateName.c_str() ); 
		}

		/// HP 및 MP 변동치 출력 함수
		void ShowIncreaseValue( D3DXVECTOR3 vecPos, bool IsHP, float fValue );
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		const vector<CX2ReflectMagicByBuffPtr>& GetVecReflectMagicByBuffPtr() const { return m_vecReflectMagicByBuffPtr; }
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		virtual void		UpNowDPValueByHit( IN const CX2DamageManager::DamageData& DamageData_ ) {}	/// 애드일 때, 타격시 DP 변동 처리
		virtual	void		UpNowDPValue( IN const float fDPValue_, IN const float fChangeValueRate_ = 1.f ) {}	/// 애드일 때, DP 수치 연산 함수
		bool				IsRightTarget(D3DXVECTOR3 vPos);	/// 특정위치가 나의 오른쪽인지 왼쪽인지 판별한다. ( 유저도 쓰기 위해, CX2GameUnit으로 이동 )
#endif //SERV_9TH_NEW_CHARACTER

		// Public Function End

#ifdef MODIFY_SET_DAMAGE_DATA
		virtual	void SetDamageExceptionProcess( OUT CX2DamageManager::DamageData* pDamageData_ , OUT float& fPowerRate);
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
		/// BuffIdentity에 의해 구분되는 효과 중 무기의 AfterImage 관련 함수
		void CreateAndInsertWeaponAfterImageByBuff( const KBuffIdentity& BuffIdentity_, const D3DXCOLOR& d3dxColor_ );
		void EraseWeaponAfterImageByBuff( const KBuffIdentity& BuffIdentity_ );
		void EnableWeaponAfterImage();
		void DisableWeaponAfterImage( const bool bForce_ = false );
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

#ifdef FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER
		// GUNPC 와 GUUSER 의 GetAdditionalAttackDamage 함수가 비슷하게 사용됨에 따라
		// 계산하는 부분만 따로 묶어서 같이 사용하기 위해 작성
		float CalcAdditionalAttackDamage ( const CX2DamageManager::DamageData* pAttackDamageData_ ) const;
#endif // FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE		/// 그룹 데미지 정보 벡터 반환
		const vector<DamageEffectGroupDataPtr>& GetDamageEffectGroupData() const { return m_vecDamageEffectGroupDataPtr; }
        vector<DamageEffectGroupDataPtr>& AccessDamageEffectGroupData() { return m_vecDamageEffectGroupDataPtr; }

		void ProcessGroupDamageData( float fElapsedTime_ );	/// 그룹 데미지 정보 처리
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
		float GetAdditionalAttackPowerRateByType( CX2DamageManager::DamageData* pDamageData_ );		/// 특정 공격에 따른 데미지 배율 반환 함수 ( AT_SPECIAL, AT_NORMAL )
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-14
		/// 일반 공격 타격시 이펙트 셋 생성하는 디스플레이어 타입
		void CreateAndInsertEffectSetNormalAttackByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCreateGap );
		void EraseEffectSetNormalAttackByBuff( const KBuffIdentity& BuffIdentity_ );
		void PlayEffectSetNormalAttackByBuff( CX2DamageManager::ATTACK_TYPE eAttackType );
#endif // SERV_NEW_DEFENCE_DUNGEON

		/// 버프에 의해 스킬레벨이 증감하는 처리
		void CreateAndInsertAddSkillLevelByBuff( const KBuffIdentity& BuffIdentity_, const int iAddSkillLevel_ );
		void EraseAddSkillLevelByBuff( const KBuffIdentity& BuffIdentity_ );
		int GetAddSkillLevelByBuff() const;

		virtual void	UpdatePassiveAndActiveSkillState() {}

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		void CreateAndInsertReflectMagicByBuff( const KBuffIdentity& BuffIdentity_, float fValue_ );
		void EraseReflectMagicByBuff( const KBuffIdentity& BuffIdentity_ );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef WRITE_SCRIPT_LOG_AT_CONSOLE_WINDOW  // 김태환
		/// 스크립트에서 콘솔창에 로그를 찍을 수 있는 기능 추가
		void WriteLogByConsoleWindow_LUA( const char* wstrLog_ );
#endif // WRITE_SCRIPT_LOG_AT_CONSOLE_WINDOW

#ifdef NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
		// 데미지 이펙트로부터 불려지는 Callback 함수
		virtual void DieDamageEffectProc( const CX2DamageManager::DamageData& pDamageData_ ){}
#endif // NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE

//-------------------------------------------------------------------------------------------------------

	public:

//{{ robobeg : 2008-11-04

        const CKTDXCollision::CollisionDataListSet&           GetCollisionListSet() const { return m_CollisionListSet; }
        const CKTDXCollision::CollisionDataListSet&           GetAttackListSet() const { return m_AttackListSet; }
        const CKTDXCollision::CollisionDataListSet&           GetSubAttackListSet() const { return m_SubAttackListSet; }
		
				
		bool GetUnderWater() const { return m_bUnderWaterHead; }
		void ChangeUnderWater(bool bNewStatus)
				{
			if( m_bCheckChangeUnderWater == true )
			{
				if( m_bUnderWaterHead != bNewStatus )
					m_bChangeUnderWater = true;
				else
					m_bChangeUnderWater = false;
			}
			else
			{
				m_bCheckChangeUnderWater = true;
				m_bChangeUnderWater = false;
			}

			m_bUnderWaterHead = bNewStatus;
		}

		virtual float	GetEnchantResist( CX2EnchantItem::ENCHANT_TYPE enchantType );
		
		virtual void		ResetPhysicParam(bool bPreserveNowSpeed = false );
		virtual void		ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ );
		virtual void DoProcessWhenKillEnemy( const CX2GameUnit* pDefender_ ) {}
		virtual void		ApplyWorldLightColor( OUT CKTDGXRenderer::RenderParam* pRenderParam_ ){}

//-------------------------------------------------------------------------------------------------------
	protected:
		virtual void		Verify();
		
#ifdef VERIFY_STAT_BY_BUFF
		bool		VerifyStatByBuff();
		template<typename T>
		bool VerifyVector( IN vector<T>& vector_ )
		{
			BOOST_FOREACH( T val, vector_ )
			{
				if ( NULL != val && false == val->Verify() )
					return false;
			}

			return true;
		}
#endif // VERIFY_STAT_BY_BUFF

		virtual void		InitDevice();
		virtual void		InitSystem();
		virtual void		InitStat();
		virtual void		InitPhysic();
		virtual void		InitComponent();
		virtual	void		DyingStart();
		virtual void		InitEtc() {};

		virtual void		SetSmallGage();
		void PhysicProcessSyncLine( const CKTDGLineMap::LineData* pLineData, D3DXVECTOR3& position );
		void CalcLastPositionValueOnAnimatedLine( const CKTDGLineMap::LineData* pLineData, D3DXVECTOR3& position );


		__forceinline void PlaySequenceByTriggerCount( CKTDGParticleSystem::CParticleEventSequence*  pParticleEventSequence, const D3DXVECTOR3& pos, float minEmitRate, float maxEmitRate, int triggerCount )
		{
			if ( pParticleEventSequence == NULL )
				return;

			pParticleEventSequence->SetPosition( pos );
			pParticleEventSequence->SetLandPosition( pos.y );	
			pParticleEventSequence->SetEmitRate( minEmitRate, maxEmitRate );
			pParticleEventSequence->SetTriggerCount( triggerCount );
		}

		__forceinline void PlaySequenceByTriggerCount( CKTDGParticleSystem::CParticleEventSequence*  pParticleEventSequence, float fPosX, float fPosY, float fPosZ, float minEmitRate, float maxEmitRate, int triggerCount )
		{
			if ( pParticleEventSequence == NULL )
				return;

			D3DXVECTOR3 pos = D3DXVECTOR3( fPosX, fPosY, fPosZ );
			pParticleEventSequence->SetPosition( pos );
			pParticleEventSequence->SetLandPosition( pos.y );	
			pParticleEventSequence->SetEmitRate( minEmitRate, maxEmitRate );
			pParticleEventSequence->SetTriggerCount( triggerCount );
		}

		__forceinline void PlaySequenceByTriggerTime( CKTDGParticleSystem::CParticleEventSequence*  pParticleEventSequence, const D3DXVECTOR3& pos, float minEmitRate, float maxEmitRate, float fTriggerTime )
		{
			if ( pParticleEventSequence == NULL )
				return;

			pParticleEventSequence->SetPosition( pos );
			pParticleEventSequence->SetLandPosition( pos.y );	
			pParticleEventSequence->SetEmitRate( minEmitRate, maxEmitRate );
			pParticleEventSequence->SetTriggerTime( fTriggerTime );
		}

		__forceinline void PlaySequenceByTriggerTime( CKTDGParticleSystem::CParticleEventSequence*  pParticleEventSequence, float fPosX, float fPosY, float fPosZ, float minEmitRate, float maxEmitRate, float fTriggerTime )
		{
			if ( pParticleEventSequence == NULL )
				return;

			D3DXVECTOR3 pos = D3DXVECTOR3( fPosX, fPosY, fPosZ );
			pParticleEventSequence->SetPosition( pos );
			pParticleEventSequence->SetLandPosition( pos.y );	
			pParticleEventSequence->SetEmitRate( minEmitRate, maxEmitRate );
			pParticleEventSequence->SetTriggerTime( fTriggerTime );
		}

		
		virtual float	GetBasicReducedDamagePercent() { return 0.0f; }

		virtual float 	GetDamageUpByAMADS() { return 0.f; }
		
		virtual void	GetDamageReduceByResistExtraDamage( CX2DamageManager::EXTRA_DAMAGE_TYPE damageType, float& fFinalDamage ) {}
		
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE		
		//{{ kimhc // 2010.11.3 // 신스킬 작업
#ifdef	NEW_SKILL_2010_11
		// 각 스테이트에서 Immune되어야할 ExtraDamage를 스크립트에서 읽어옴
		void LoadImmunityAtThisStateFromScript( IN KLuaManager& luaManager_ );
#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.3 // 신스킬 작업
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		bool ProcessEnchantExtraDamage( CX2DamageManager::DamageData* pDamageData, 
			CX2DamageManager::EXTRA_DAMAGE_TYPE basicExtraDamageType, CX2DamageManager::EXTRA_DAMAGE_TYPE miscExtraDamageType, float fRandomValue );

		virtual bool	ProcessEnchantBlaze( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_BLAZE );
		virtual bool	ProcessEnchantFrozen( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_FROZEN );
		virtual bool	ProcessEnchantPoison( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_POISON );
		virtual bool	ProcessEnchantPiercing( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_PIERCING );
		virtual bool	ProcessEnchantShock( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_SHOCK );
		virtual bool	ProcessEnchantSnatch( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_SNATCH );

		virtual	void	PostProcessEnchantDamage( OUT CX2DamageManager::DamageData* pDamageData_ );

		virtual CX2EnchantItem::ENCHANT_TYPE CX2GameUnit::GetEnchantTypeFromExtraDamageType( CX2DamageManager::EXTRA_DAMAGE_TYPE edtType );
		virtual void	ApplyEnchantResist( CX2EnchantItem::ENCHANT_TYPE enchantType, float& fDamage );
		virtual bool	IsImmuneToEnchant( CX2EnchantItem::ENCHANT_TYPE enchantType );
#ifdef PET_AURA_SKILL
		virtual float	GetEnchantAttack( CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType );
#endif
#ifdef ENCHANT_ACCUMULATION_RATE_MODIFIER
		float EnchantAccumulationRateModifier( CX2DamageManager::EXTRA_DAMAGE_TYPE basicExtraDamageType, CX2DamageManager::EXTRA_DAMAGE_TYPE miscExtraDamageType, float fRate );
#endif ENCHANT_ACCUMULATION_RATE_MODIFIER
		

#ifdef SKILL_30_TEST
		void CommonFrameMoveStateAbnormality();
		virtual void CommonFrameMoveStateAbnormalityFrozen() = NULL;
		void CommonFrameMoveStateAbnormalityEntangled();
		void CommonFrameMoveStateAbnormalityTimeBomb();
#endif SKILL_30_TEST
		void CommonFrameMoveStateAbnormalityAging();
		void CommonFrameMoveStateAbnormalityStigmaDebuff();
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
		void CommonFrameMoveStateAbnormalityBlind();
#endif NEW_SKILL_2010_11

#ifdef EVE_ELECTRA
		virtual void CommonFrameMoveStateAbnormalityFlashBang() = NULL;
#endif EVE_ELECTRA

#ifdef EDT_WATER_HOLD_TEST
		virtual void CommonFrameMoveStateAbnormalityWaterHold() = NULL;
#endif EDT_WATER_HOLD_TEST

		

		//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
		void CommonFrameMoveStateAbnormalityPanic();
		void CommonFrameMoveStateAbnormalityPain();
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
		//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
		void CommonFrameMoveStateAbnormalityLegShot();
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
		void CommonFrameMoveStateAbnormalityEarthQuake();
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef SERV_SKILL_NOTE
		void CommonFrameMoveAnimSpeedFactor();
		void CommonFrameMoveAnimSpeedFactorEffect(SPEED_FACTOR speedFactor);
		void CommonFrameMovePoisonDeBuff();
		void CommonFrameMoveRevengeDefence();
#endif

		void CommonFrameMoveMoveSpeedFactor();
		void CommonFrameMoveJumpSpeedFactor();
		void CommonFrameMoveMoveSpeedFactorEffect(SPEED_FACTOR speedFactor);		
		void CommonFrameMoveJumpSpeedFactorEffect(SPEED_FACTOR speedFactor);

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void ParseAnimNameFromList( wstring &animName, int iRandRate );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool IsDamagedReaction( const CX2DamageManager::DamageData* pDamageData );


#ifdef SERV_SKILL_NOTE
		float GetDefenceMemoTime() { return m_fRevengeDefenceTime; }
		void  SetDefenceMemoTime(float fVal) { m_fRevengeDefenceTime = fVal; }
		float GetEnduranceAttackTime() { return m_fEnduranceAttackTime; }
		void  SetEnduranceAttackTime(float fVal) { m_fEnduranceAttackTime = fVal; }
		float GetPoisonDeBuff() { return m_fPoisonDeBuff; }
		void  SetPoisonDeBuff(float fVal) { m_fPoisonDeBuff = fVal; }
#endif



		virtual bool DoScaleHeadBone() { return true; }	/// FieldFix: OnFrameMove에서 프레임 꽤 차지함

//{{ kimhc // 2010.6.16 
// 1. GameUnit::Weapon::SetEnchantParticleShow() 과 GUUser::SetShowEnchantedWeaponEffectAtHand()을
// 같이 쓰는 경우가 많으므로 한데 묶음
// 2. NotifyShowObjectChanged()에서는 서로 따로 호출하도록 되어있는것을 한곳에서만 호출하도록 수정
// 3. virtual로 만들어서 GameUnit에서는 GameUnit::Weapon::SetEnchantParticleShow()를 호출하고
// GUUser에서는 SetShowEnchantedWeaponEffectAtHand()를 호출하도록 변경
#ifdef	ENCHANT_BUG_TEST
		virtual void SetShowEnchantWeaponParticle( bool bShow );
#endif	ENCHANT_BUG_TEST
//}} kimhc // 2010.6.16 

		//{{ kimhc // 2010.11.1 // 로드나이트 - 고통 억제
		
		static	bool GreaterExtraDamageTime( CX2DamageManager::ExtraDamageData* pFirst, CX2DamageManager::ExtraDamageData *pSecond );
		void CureExtraDamage( int iNumOfExtraDamageToBeCure_ = 1 );

		//{{ 2011-04 에 패치될 청 메모 헤드샷
#ifdef	CHUNG_MEMO_01				
		bool GetHittedExtraDamageHeadShot() const { return m_bHittedExtraDamageHeadShot; }
		void SetHittedExtraDamageHeadShot( bool bHittedExtraDamageHeadShot_ ) { m_bHittedExtraDamageHeadShot = bHittedExtraDamageHeadShot_; }
		void ApplyHeadShotDamage( float* pfFinalDamage_ );
#endif	CHUNG_MEMO_01				
		//}} 2011-04 에 패치될 청 메모 헤드샷

#ifdef NEW_HENIR_TEST

#ifndef ADD_HENIR_BUFF		/// 버프로 변경되면서 쓰이지 않게 된 구문
		float GetTimeHenirBuffLight(){ return m_fTimeHenirBuff_Light; }
#endif ADD_HENIR_BUFF

#endif NEW_HENIR_TEST
	
		void UpdateVisibilitySmallGageAndName();

		void DoDelegateProcess( const vector<DelegateProcess>& vecDelegateProcess_ );
		void DoDelegateProcessWithDamageData( const vector<DelegateProcessWithDamageData>& vecDelegateProcess_, const CX2DamageManager::DamageData& damageData_ );

		void AddToDamagedMap( const CX2GameUnit* pAttackerGameUnit_, const float fFinalDamage_ );
		UidType GetGameUnitUIDMaxDamage() const;

public:
		virtual void	AnimStop() = 0;
		virtual void	AnimPlay() = 0;
		virtual void SetCanPassUnit( const bool bFlag_ ) = NULL;
		bool GetShowSmallGageAndName() const { return m_bShowSmallGageAndName; }
		void SetShowSmallGageAndName( bool bShowSmallGageAndName_ ) { m_bShowSmallGageAndName = bShowSmallGageAndName_; }

#ifdef ELSWORD_WAY_OF_SWORD
		void CureExtraDamage( CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageTypeToCure_ );
#endif ELSWORD_WAY_OF_SWORD

		UCHAR GetHittedCount() const { return m_ucHittedCount; }
		void SetHittedCount(UCHAR val) { m_ucHittedCount = val; }

		UCHAR GetHitCount() const { return m_ucHitCount; }
		void SetHitCount(UCHAR val) { m_ucHitCount = val; }

		UCHAR GetNumOfDeBuff() const { return m_ucNumOfDeBuff; }
		void SetNumOfDeBuff(UCHAR val) { m_ucNumOfDeBuff = val; }

		UCHAR GetNumOfBuff() const { return m_ucNumOfBuff; }
		void SetNumOfBuff(UCHAR val) { m_ucNumOfBuff = val; }

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		DWORD	GetFrameMoveCountFuture() const { return m_dwFrameMoveCountFuture; }
//		void	SetFrameMoveCountFuture(DWORD val) { m_dwFrameMoveCountFuture = val; }
//
//		DWORD	GetFrameMoveCountNow() const { return m_dwFrameMoveCountNow; }
//		void	SetFrameMoveCountNow(DWORD val) { m_dwFrameMoveCountNow = val; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void	OnFrameMoveBuff( float fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void	OnFrameMoveBuff();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		virtual void	ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ );
		void	FinishAndClearAllBuff();

		UINT	GetUnitSlashTraceDataSize() const
		{
			if ( NULL != m_pUnitSlashTraceManager )
				m_pUnitSlashTraceManager->GetUnitSlashTraceDataSize();
			else
				return 0;
		}

		void	DoUnitSlashTraceInCommonStateStart();
		void	AddSlashTraceTiming();
#ifdef BALANCE_CODE_NEMESIS_20121213
		virtual void ModifyFinalDamageByGameUnitType( OUT CX2DamageManager::DAMAGE_TYPE& damageType_, OUT float& fAttackerPhysicDamage_, OUT float& fAttackerMagicDamage_, OUT float& fForceDownValue_ ) {}
#else
		virtual void ModifyFinalDamageByGameUnitType( OUT CX2DamageManager::DAMAGE_TYPE& damageType_, OUT float& fAttackerPhysicDamage_, OUT float& fAttackerMagicDamage_ ) {}
#endif //BALANCE_CODE_NEMESIS_20121213

		bool	ApplyDrainHpBuff()
		{
			const float fDrainHpRate = m_AdditionalDrainHpNormalAttack.GetResultValue();
			if( fDrainHpRate > 0.f && m_DamageData.attackType != CX2DamageManager::AT_SPECIAL )
			{
				m_DamageData.m_fDrainHpRate = fDrainHpRate;
				return true;
			}

			return false;
		}

#ifdef BALANCE_BLADE_MASTER_20130117		/// 체력 흡수치 반환 함수
		float	GetDrainHpBuffValue( CX2DamageManager::DamageData* pDamageData_ )
		{
			const float fDrainHpRate = m_AdditionalDrainHpNormalAttack.GetResultValue();

			if( NULL != pDamageData_ && fDrainHpRate > 0.f && pDamageData_->attackType != CX2DamageManager::AT_SPECIAL )
			{
				pDamageData_->m_fDrainHpRate = fDrainHpRate;
				return fDrainHpRate;
			}

			return 0.f;
		}
#endif BALANCE_BLADE_MASTER_20130117

		virtual void SetStateToNowAnimation( const CKTDGXSkinAnim::XSKIN_ANIM_STATE eAnimState_ ) = NULL;
		
		virtual float GetNowAnimationTime() const = NULL;
		virtual void SetNowAnimationTime( const float fAnimCurrentTime_ ) = NULL;

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		void SetToDamagedMap ( const UidType uidType_, const float Damage_ );		// 해당 유닛에 대미지 맵을 더함, Uid 로 검색
		void AddToDamagedMap ( const UidType uidType_, const float Damage_ );		// 해당 유닛에 대미지 맵을 더함, Uid 로 검색
		const map<UidType,float>& GetDamagedMap() const { return m_DamagedMap; }			// DamagedMap 을 반환
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef MODIFY_EFFECT_SET_DELETE
#ifdef  ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
		void AddEffectSetDeleteCondition( CX2EffectSet::Handle hEffectSetHandle_, const vector<DELETE_CONDITION_EFFECT_SET> & eDeleteCondition );
#else   ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
        void AddEffectSetDeleteCondition( CX2EffectSet::Handle hEffectSetHandle_, DELETE_CONDITION_EFFECT_SET eDeleteCondition );
#endif  ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
#endif //MODIFY_EFFECT_SET_DELETE 
		
		virtual void DamageDataChangeProcess(){}
//#ifdef INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
		//virtual void ParsingBuffFactorID( KLuaManager& luaManager_ ){}
		//virtual void CommonFrameMove_InsertBuffFactor(){}
//#endif // INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
#ifdef FIELD_BOSS_RAID
		virtual bool GetIsBosRaidNPC() const { return false; }
		virtual bool GetIsMiddleBosRaidNPC() const { return false; }
		virtual bool GetIsImmuneDebuff() const { return false; }
		virtual bool GetIsImmuneAllEDT() const { return false; }
#endif // FIELD_BOSS_RAID

#ifdef SERV_9TH_NEW_CHARACTER		
		virtual float	GetChangeDamageRateByAttackerSkill( const CX2DamageManager::DamageData* pDamageData_ ) const { return 1.f; }
		const D3DXCOLOR GetFlickerColorByBuff( IN const D3DXCOLOR& coRenderColor_, IN const float fFlickerGap_ );
#endif // SERV_9TH_NEW_CHARACTER

		// Public Member Start
	public:
		vector<UidType>							m_KillUserUnitList;		/// 킬한 유저수 인데.. PVP에서만 사용됨
		vector<int>								m_KillNPCList;			/// 사용 안함

		vector<UidType>							m_MaxDamageKillUserUnitList;	/// 의미는 죽은 유저 수 중에 자신이 최대 데미지를 준 수 같은데.. 잘못 쓰이고 있는 듯...
		vector<int>								m_MaxDamageKillNPCList;	/// 사용 안함
		
		vector<CX2GameUnit*>					m_HitUnitList;		/// kimhc // 필요 이유를 모르겠음
		int										m_iDieCount;		// 오현빈 //대전 플레이 중에 die 카운트를 확인하기 위해 추가
		// Public Member End

		//-------------------------------------------------------------------------------------------------------
	protected:

		CKTDXCollision::CollisionDataListSet            m_CollisionListSet;	/// 이건 피격박스 겠고..
		CKTDXCollision::CollisionDataListSet            m_AttackListSet;	/// 이건 타격 박스 겠고..
		CKTDXCollision::CollisionDataListSet            m_SubAttackListSet;	/// 이건..아마겟돈 블레이드 용...?

		typedef std::pair<CKTDGXMeshPlayer::CXMeshInstanceHandle, CKTDXCollision::CollisionDataListSet> PairSubAttackListSet;
		vector<PairSubAttackListSet>					m_vecPairSubAttackListSet;	/// first에는 Mesh의 핸들, second에는 CollisionDataListSet

#ifndef X2OPTIMIZE_NPC_NONHOST_SIMULATION
		float m_fCommonDamageChangeTime; // 스크립트의 DamageData 변경 시점 
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		//}} robobeg : 2008-11-04
	
//-------------------------------------------------------------------------------------------------------

private:
		KProtectedType<float>							m_fMaxHyperModeTime;	/// 최대 각성 시간
		//KProtectedType<float>							m_fRemainHyperModeTime;	/// 남아있는 각성 시간

//-------------------------------------------------------------------------------------------------------
	
protected:

		GAME_UNIT_TYPE									m_GameUnitType;		/// User인지 NPC인지를 구분 하는 타입
		int												m_UnitIndex;		/// NPC의 경우 m_NPCUnitList에서 unit이 차지하는 위치의 index(비어있는곳), User의 경우 slotIndex 이다.
		UidType											m_UnitUID;			/// Unique ID
		KProtectedType<int>								m_TeamNum;			/// Team 분류 넘버로, 던전에서는 보통 CX2Room::TN_RED 이다.
//#ifdef PVP_BOSS_COMBAT_TEST
//		int												m_TeamNumOriginal;		
//#endif PVP_BOSS_COMBAT_TEST
#ifdef	SERV_TRAPPING_RANGER_TEST
		CX2Room::TEAM_NUM								m_eAllyTeam;
#endif	SERV_TRAPPING_RANGER_TEST

		wstring											m_ScriptFileName;	/// 해당 Unit의 Script 파일명 char* 이어도 될 듯.. 
		
		float											m_fTime;			/// OnFrameMove를 통해 넘겨받은 fTime 값을 저장
		float											m_fElapsedTime;		/// 중국버전에서는 필요 없긴 했는데... Ktdxapp에 있는걸 그냥 사용하면 되니까...

		KLuaManager										m_LuaManager;		/// 루아매니저, 생성자리스트에서 초기화 시킨건 잘 한듯..

		//PhysicParam										m_ScriptedPhysicParam;		/// 스크립트에 지정된 원본 physic param
		PhysicParam										m_OrgPhysicParam;	/// Original이 변경될 것이라면 왜 Original 인가..?
		PhysicParam										m_PhysicParam;		/// 현재의 PhysicParam... 잘 정리해 보자.../

		CX2DamageManager::DamageData					m_DamageData;		/// User 또는 NPC 스크립트에 있는 DamageData
		
		float											m_fRevengeAddDamage;	/// 로반격에 필요한 상대방이 준 데미지 (DamageReact를 어떻게 구성하느냐에 따라서 엘소드에게만 있어도 될 듯
		int												m_DamageReducePercent;	/// 데미지를 감소시켜주는 Percent로, 현재는 청 가드, 레이븐 와일드차지, PVP 히어로 SPIKA 만 사용 하고 있음
        int                                             m_DamageGuardPercent;	/// 현재는 NPC에서만 사용하는 데미지 감소용




#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
		int			m_iLinkJumpTargetLineIndex;
		D3DXVECTOR3 m_vLinkJumpTarget;
		D3DXVECTOR3 m_vLinkJumpSpeed;
		D3DXVECTOR3 m_vLinkJumpAcceleration;	// 중력가속도를 포함한 가속도
		float		m_fLinkJumpTime;
		float		m_fElapsedTimeAfterLastLinkJump;
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST


		
		CX2OldGageManager								m_GageManager;
		CX2GageData*									m_pGageData;		/// HP, MP, 캐릭터 이미지 등이 나오는 게이지 뿐만이 아니라, 내 유닛이 아닌 경우에는 파티원 게이지, NPC등인 경우에는 NPC 게이지 등이 나오는 만능(?) 게이지....
		CX2GageUI*										m_pGageUI;

		CKTDGXRenderer::RenderParam						m_RenderParam;	/// 외곽선, alpha 등의 값을 지정하는 RenderParam
		//vector<CKTDGXRenderer::RenderParamPtr>			m_vecRenderParamPtr;

		CKTDGPlanShadow*								m_pUnitShadow;	/// 유닛의 그림자.. 생성여부는 스크트에서 지정 (대체로 shadow.dds)


		/// 각성 관련 이펙트 부분은 좀 생각해 봐야 하지 않을까..
		/// kimhc // 이 핸들들을 각성 버프안으로 옮겨볼까...
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHyperBoostRFoot;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHyperBoostLFoot;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHyperBoostRArm;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHyperBoostLArm;

		
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hQuestionMark;	/// NPC는 사용, 유저는 엘소드 외에는 사용하지 않는다.
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hExclamationMark;	/// NPC는 사용, 유저는 엘소드의 반격에 당했을 때 사용
		
		wstring												m_HeadBoneName;		/// 이건 NPC 한테만 있어도 될 것 같다..

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		vector<D3DXVECTOR2>								m_AttackTimeList;		/// 공격 타임, 데미지 체크를 수행하는 AnimationTime
		vector<wstring>									m_AttackProjSeqName;	/// 공격시 표현 되는 의성어 Particle을 이것으로 나타내고 있으나.. 방법이 좋지 않은듯... NPC것은 지우지도 않네..

		/// Stop 부분은 필드에서 어떻게 될지...
		vector<D3DXVECTOR3>								m_StopAllList;
		vector<D3DXVECTOR2>								m_StopOtherList;
		vector<D3DXVECTOR2>								m_StopMyList;
		vector<D3DXVECTOR3>								m_Stop2AllList;
		vector<D3DXVECTOR2>								m_Stop2OtherList;
		vector<D3DXVECTOR2>								m_Stop2MyList;

		vector<SoundPlayData>							m_vecSoundPlayData;		/// Sound 데이타
		vector<SoundPlayData>							m_vecSoundPlayLoop;		/// NPC 그중에서 벨더 비던의 글리터 등을 만드는 POT 에서만 현재 사용 중
		vector<CKTDXDeviceSound*>						m_vecLoopSound;		/// 각 스테이트에 있는 LOOP_SOUND를 NPC가 죽거나, STOP_LOOP_SOUND가 true 인 스테이트를 만났을 때 종료됨
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		CX2ComboManager*								m_pComboManager;	/// 자기 유저만 있으면 되는 것을 왜.. 다른 유저와 NPC 것 까지.. 생성하는가..
//{{ robobeg : 2013-11-04
		//CX2Stat*										m_pStat;			/// 서버가 전달해 준 정보로 셋팅 되는 줄 알았는데.. 아니였네.. g_pData->GetMyUser()->GetUnitByUID()가 m_pUnit인데 이것의 m_pUnit->GetUnitData()->m_GameStat 과 m_Stat을 사용함
        CX2Stat                                         m_Stat;
//}} robobeg : 2013-11-04

		float											m_DamageLightTime;	/// 타격을 하거나 피격을 당했을 경우 해당 시간 동안 World의 Light 위치를 변경 시킨다
		bool											m_bAttackedByUserUnit;	/// 자신을 타격한 유닛이 유저인지.. (죽었을 때의 죽인 주체가 누구인지 판단할 때 사용) 사용 안하는 방법은 없나..?
		//UidType											m_AttackerUnitUID;		/// 자신을 타격한 유닛의 UID

#ifdef SERV_PET_SYSTEM
		//UidType											m_LastAttackUnitUID;	/// 자신이 마지막으로 타격한 유닛
#endif

		CX2GameUnitoPtr									m_optrGameUnitAttackedByMe;		/// 자신에게 타격 당한 유닛
		CX2GameUnitoPtr									m_optrGameUnitWhoAttackedMe;	/// 자신을 타격 한 유닛

		wstring											m_ShadowFileName;		/// 그림자로 사용할 파일 명, 그러나 이것을 계속 가지고 있어야 할 이유는 없음
		float											m_fShadowSize;			/// 그림자의 사이즈 인듯 한데... 이것도 계속 가지고 있어야 할 이유는 없음
		float											m_fDieAfterTime;		/// 현재는 던전에서만 사용하는 값으로 바꾸어 보는 건??? HP < 0 이면 DungeonGame에 바로 등록...??
		bool											m_bDyingStart;			/// 죽는 스테이트를 실행 했는지.. 그러나 NPC한테는 필요할지도 모르나 User한테는...??


		// 기존에는 아래 무적 부분들이 (state invincle 아님) User에게만 사용되었는데 PVP_BOT이 들어가면서 변경되었다... 나누는게 좋지 않을까...?
		KProtectedType<float>							m_fForceInvincibleTime;	/// 무적 시간, 각 스테이트의 INVINCIBLE 키워드 보다 우선 하다

		KProtectedType<float>							m_fShowInvincibleTime;	/// 무적 시간동안 무적 상태임을 보여주는 시간

		float											m_fShowInvincibleSwapTime; /// 위의 무적 시간동안 흰색과 일반 색으로 깜박이는데 그 타이밍 (대략 0.05초)

		bool											m_bShowInvincibleWhite;	/// m_fShowInvincibleTime > 0 인 조건 하에 m_fShowInvincibleSwapTime이 <= 0 이 될 때 마다 토글 시켜준다.

		bool											m_bUpMPThisFrame;	/// 한 프레임 내에서 두번 MP가 UP되지 않도록 하려고 하는 것 같은데... 이게 필요 한가...? CommonFrameMove에 false로 셋팅해준다는건.... 한프레임에 두명이 맞은 경우에는 두번의 MP UP이 실행되는게 맞는것 같은데... 유저만 쓰는 것 같다. (DamageEffect에서는...?)
		float											m_fCanNotAttackTime;	/// 각 스테이트의 EventProcess에서 공격 관련된 버튼 입력을 받지 않는 구간을 만드는데 사용. 이건 정말.. 유저만 필요하다.
		float											m_fCanNotJumpTime;	/// Jump 관련 키입력을 받지 않는 시간. 이것도 유저만 필요하다.
		float											m_fCanNotInputTime;	/// 모든 키 입력을 받지 않는 시간. 이것도 유저만 사용 중
#ifdef MODIFY_DUNGEON_STAGING
		bool											m_bCanNotInput;
#endif //MODIFY_DUNGEON_STAGING
		float											m_fCanNotInputTimeZXArrow;	/// 유저만 사용 중인 것으로 이 값이 0 이상이면 ZXArrow 관련 한 키를 init 시킴 (Portal, Bungee 등에서 사용하는 듯)

		CX2DamageManager::HITTED_TYPE					m_HittedType;	/// 이 값에 따라 타격 당했을 때의 사운드가 달라진다
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//{{ kimhc // 2011.1.21 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		CX2DamageManager::HITTED_TYPE					m_eHittedTypeAtState;	// 스테이트 별 HittedType 지정, 이것도 일단은 청만 사용 하는 걸로 알고 있긴 한데...
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // 청 1차 전직
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		int												m_FlyCount;		/// 몇번이냐 공중으로 떳냐하는 회수로, 이 값이 클수록 뜨는 높이가 작아짐 가RT_UP, RT_FLY, RT_DRAG_UP 공격에 의해 위로 떳을 때 m_FlyCount가 증가)

		map<UidType,float>								m_DamagedMap;	/// 자신에게 데미지를 준 유닛의 UID와 데미지를 저장하고, 가장 많은 데미지를 준 유닛의 UID를 서버에 전송
		
		float											m_fScale;	/// 유닛의 스케일을 조정하기 위한 것인 것 같은데.. NPC만 사용 중...
		
		bool											m_bUseWorldColor;	/// 청과 몬스터 중 BEE_BOMB(?) 가 사용 중... 보통 true고 특별한 경우 RenderParam 값을 바꿀때 false로 사용

		vector<CX2BuffTempletPtr>						m_vecBuffTempletPtr;		/// 버프를 구분할 수 있는 키와 해당 유닛에게 걸린 버프(map으로 하지 않은 이유는 find 보다는 loop가 더 잦기 때문)
		
		/// 항상 남한테 거는 버프요소
		
		/// 타격시 남한테 거는 버프요소
		/// 피격시 남한테 거는 버프요소
		/// 회피시 남한테 거는 버프요소
		/// 디버프에 걸릴시 남한테 거는 버프요소
		
		/// 타격시 자신한테 거는 버프요소
		/// 피격시 자신한테 거는 버프요소
		/// 회피시 자신한테 거는 버프요소
		/// 디버프에 걸릴시 자신한테 거는 버프요소

		vector<CX2BuffFactorPtr>						m_vecBuffFactorPtr;				/// 남한테 주는거(장비, 펫, 칭호 등에 의해서 항상가지고 있는 것)
		vector<CX2BuffFactorPtr>						m_vecTemporaryBuffFactorPtr;	/// 남한테 주는거(버프등에 의해 일시적인 것)

		//vector<CX2BuffFactorPtr>		/// 남한테 주는건데 일시적인것
		//vector<CX2BuffFactorPtr>		/// 나한테 주는건데 일시적인것

// 		vector<CX2BuffFactorPtr>						m_vecEquipBuffFactorToOtherPtr;			/// 장비에 의해서 남한테 걸수있는 버프요소
// 		vector<CX2BuffFactorPtr>						m_vecTitleBuffFactorToOtherPtr;			/// 칭호에 의해서 남한테 걸수있는 버프요소
// 		vector<CX2BuffFactorPtr>						m_vecPetBuffFactorToOtherPtr;			/// 펫에 의해서 남한테 걸수있는 버프요소
// 		vector<CX2BuffFactorPtr>						m_vecBuffTempletBuffFactorToOtherPtr;	/// 자신한테 걸린 버프에 의해서 남한테 걸수있는 버프요소
// 
// 		vector<CX2BuffFactorPtr>						m_vecEquipBuffFactorToMePtr;			/// 장비에 의해서 자신한테 걸수있는 버프요소
// 		vector<CX2BuffFactorPtr>						m_vecTitleBuffFactorToMePtr;			/// 칭호에 의해서 자신한테 걸수있는 버프요소
// 		vector<CX2BuffFactorPtr>						m_vecPetBuffFactorToMePtr;				/// 펫에 의해서 자신한테 걸수있는 버프요소
		
		ExtraDamagePack									m_ExtraDamagePack;	/// ExtraDamage 구조체.. 감당이..
		CX2SocketItem::SocketData						m_SocketData;		/// 소켓 옵션, NPC는 이거 왜 가지고 있니..
		map< CX2EnchantItem::ENCHANT_TYPE, int >		m_mapDefenceEnchantData;	/// 방어관련 EnchantData
#ifdef PET_AURA_SKILL
		map< CX2EnchantItem::ENCHANT_TYPE, float >		m_mapAttackEnchantData;		/// 일단 NPC에게는 필요 없어 보이는데.. 휴..
#endif


		std::vector< Weapon* >							m_vecpWeapon;	/// 무기 객체 리스트


		//Minor
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPoisonBody;	/// m_ExtraDamagePack의 포이즌에 걸렸을 때 나오는 파티클 핸들... 포이즌에 걸리나 걸리지 않으나 가지고 있어야 하나..
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqFireBody1;	/// m_ExtraDamagePack에 Fire 또는 Blaze에 걸렸을 때 나오는 파티클 핸들 (Minor)
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqFireBody2;	/// m_ExtraDamagePack에 Fire 또는 Blaze에 걸렸을 때 나오는 파티클 핸들	(Minor)

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCurseBody;	/// 저주
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSlowBodyRight;	/// 슬로우 걸렸을 때 양쪽 다리에 붙는것...
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSlowBodyLeft;	/// 슬로우 걸렸을 때 양쪽 다리에 붙는것...
				
		// 이하는 clod나 EnchantFrozen에 걸렸을 때..
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold1;	
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold1_Right;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold2_Right;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold_Head;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPepperBody1;	/// 청양고추
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPepperBody2;	/// 청양고추

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqStun;		/// 스턴 (기절 정도인듯?)

		// 근데.. 스내치를 쓰는 NPC도 있나...?
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqHPUp;	/// 스내치
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqMPUp;	/// 스내치


		

		//////////////////////////////////////////////////////////////////////////////
		// 이펙트가 사용한 유저에게만 나와도 되는것을 구분하는게 어떨까?
		//////////////////////////////////////////////////////////////////////////////

		// Major
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Stun;	/// 인첸트스턴(석화??)
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Exclamation;	/// 인첸트 중첩 1개
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Exclamation2;	/// 인첸트 중첩 2개 이상
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Exclamation3;	/// 인첸트 중첩 3개 이상

		/// 구버전 블레이즈 ----------------------------------------------------------------
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Blaze_Second;
		/// -------------------------------------------------------------------------------
		
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Frozen_Second;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Poison_Second;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Stun_Second;
		
		// 상태이상 이모티콘
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqBleeding;	/// 이마에 피흘리는 얼굴
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSkeletonHead;		/// 시한폭탄 해골
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqStunAbnormality;	/// 스턴 걸렸을 때라는데... 파란색 똥글뱅이.. 페이타의 랜턴 한테 당했을 때 나오는 것
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqAging;			/// 노화된 얼굴 표시
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hStigma;	/// 과녁 마크
		CX2EffectSet::Handle								m_hEffSetDryad;	/// 땅에 유저를 붙잡는 나무줄기 이펙트
#ifdef SERV_SKILL_NOTE
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hRevengeDefence;	/// 엘소드 반격 메모를 위한 것 같은데...
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hPoisonDeBuff;	/// 얼굴 나오는 이미지에 독구름이 나오는 것 (m_ExtraDamagePack.m_Poison, 다른 포이즌하고 뭐가 다른거야?)
#endif
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqBlind;	/// 로나 샌드스톰에 의해 생기는 이미지...(육회안 같이 생긴게 부서진 이미지??)
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST// lucidash : 김상훈 [2010.1.4] 
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWaterHold1;	/// 라구즈 였나? 물방울 이펙트
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWaterHold2;	/// 역시 물방울 이펙트이지만 작은 것...
#endif EDT_WATER_HOLD_TEST

		//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqPanic;	/// 좌우가 계속 바귀는 혼란
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
		//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqLegShot;	/// 달팽이 표시..
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEarthQuakeRight;	/// 이동속도와 애니메이션을 둘다 느리게 하는 효과의 이펙트 핸들
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEarthQuakeLeft;	/// 이동속도와 애니메이션을 둘다 느리게 하는 효과의 이펙트 핸들
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

		// 상태이상 이펙트셋
		CX2EffectSet::Handle m_hEffSetStateAbnormalityFrozen;	/// m_ExtraDamagePack.m_Frozen 에 걸렸을 때.....

#ifdef RAVEN_WEAPON_TAKER
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSmokeBlind;
#endif RAVEN_WEAPON_TAKER

#ifdef GUIDE_QUEST_MONSTER
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqQuestMonster; // 퀘스트 몬스터 표시 이펙트
#endif //GUIDE_QUEST_MONSTER

		CX2DamageManager::EXTRA_DAMAGE_TYPE				m_eWeaponEnchantExtraDamageType;	/// 무기의 인첸트 속성
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMajorMeshPlayerList[10];	/// 이건 루아에서 특히 많이 사용 중인데.. 마음데로 없애기는 힘들고...
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMinorMeshPlayerList[10]; /// 이건 루아에서 특히 많이 사용 중인데.. 마음데로 없애기는 힘들고...


#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		vector<ParticleEffectData*>						m_ParticleEffData;	/// NPC만 사용 하는 듯
		vector<MeshPlayerEffectData*>					m_MeshPlayerEffData;	/// NPC만 사용하는 것처럼 보이나 실제 사용 하는 곳은 없는 듯
		vector<CameraCrashData*>						m_CameraCrashData;	/// NPC만 사용 하는 듯
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		bool											m_bIgnoreLineSpeed;			/// 라인에 지정된 스피드를 적용할지 말지 (기본 false, pos += dirvector * speed * elapsedTime)

#ifdef FOG_WORLD
        bool                                            m_bFog;		/// 옵션이 HIGH인 상태에서 true면 fog 사용, NPC만 사용 중
#endif
		
#ifdef VERIFY_STAT_BY_BUFF
		PROTECT_VECTOR3									m_vBasicScale;	/// 유닛의 기본 스케일을 나타내려는 것 같긴 한데...					
		PROTECT_VECTOR3									m_vTransScale;	/// 유닛의 변형된 크기
#else	// VERIFY_STAT_BY_BUFF
		D3DXVECTOR3										m_vBasicScale;	/// 유닛의 기본 스케일을 나타내려는 것 같긴 한데...					
		D3DXVECTOR3										m_vTransScale;	/// 유닛의 변형된 크기
#endif // VERIFY_STAT_BY_BUFF

		D3DXVECTOR2										m_vOrgUnitSize;

		float											m_fBasicBoneScale;

		float											m_fTimeDirectToReturnBasicScale;	/// 이 값에 따라서 m_vTransScale가 조금씩 커지다가 0이하가 되는 순간 원래 크기로 돌아옴
		float											m_fTimeToReturnBasicScale;			/// 원래 크기로 돌아갈 때 가지 걸리는 시간..(지속시간)

		bool											m_bCheckWidthEnlargeSize;		/// 이것도 스케일 조정과 관련된 변수
		bool											m_bCheckHeightEnlargeSize;		/// 이것도 스케일 조정과 관련된 변수 ... 근데 false는 둘다 시키는데.. (TransformScale에서) true는 if, else로 하나밖에 실행 안시키나...?
		
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool											m_bDisableGravity;				/// 도대체 이게 왜... 여기 있는지... User만 사용 함...코드내에서 중력 영향 무시하도록 할 때 사용합니다
		bool											m_bDisableGravityInScript;		/// 캐릭터, 몬스터 스크립트에서 중력 영향 무시하도록 할 때 사용합니다 라고 하지만 이것도 User만 사용 하는군...
		D3DXVECTOR2										m_vDisableGravityInScriptTime;	/// 중력 무시 시간 (X는 시작시간, Y는 종료시간)
		D3DXVECTOR2										m_vIgnoreLineTime;				/// 이 애니메이션 시간 동안은 떨어지는 중에도 라인 체크를 무시한다. 즉, 선위에 밟고 서지 않는다. (헬드롭하고, 아틸러리가 사용하는 듯) NPC만 사용 중이네..
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		

		bool											m_bOneTimeInit;	/// 사용안함

		bool											m_bWaitingForRebirthAfterBungee;	/// 번지라인맵으로 빠진 경우 이값을 true로 함, true가 되면 m_timerRebirthAfterBunge를 체크
		CKTDXTimer										m_timerRebirthAfterBungee;	/// 번지라인맵에서 다시 되돌아 올때 까지 걸리는 시간


		float											m_fGroggyTime;				/// 맞은 데미지에 그로기 시간이 있는 경우, 그로기에 걸리고 그로기 상태(피가 많이 소모되서 힘들어하는...)를 실행 함, 스턴 이펙트도 보여주네..?


		bool											m_bInvisibility;			/// 지금 안보이는 상태인지, 현재는 이브만 투명화 기술을 사용함. 이름, 팀표시 마크 렌더링도 빠져야 되기 때문에 변수선언을 여기에 함 (옮겨야 함..)
		bool											m_bAbsoluteInvisibility;	/// 위의 것 하고 뭔 차인가...m_bInvisibility == true인 상태에서 이름, 팀표시, after_image, slash_trace, outline, unit_shadow, 칭호 모두 렌더링 안되는 상태인지


		D3DXVECTOR3										m_vDisplaceUnitPosition;	/// 이브의 블랙홀 필살기와 같이 캐릭터 외부에서 캐릭터 위치를 옮겨줄 때 사용되는 변수, 이 값만큼 캐릭터 위치가 이동된다.
		D3DXVECTOR3										m_vFixedUnitPosition;		/// 사용안함(블랙홀 필살기 관련. 블랙홀에 일정거리이상 가까워지면 캐릭터 위치를 고정해버린다.)


		CKTDXTimer										m_TimerRandomSeedSync;		/// RandomSeed 값을 바꾸기 위한 타이머... 근데 GUUser에도 또 선언 되어 있네... 삭제해야함
		static const double								RANDOM_SEED_RESET_TIME;		/// RandomSeed 값을 바꾸는 주기


//#ifdef LINEMAP_FAST_WIND_TEST
//		bool											m_bEnableUpsideWind;	
//		bool											m_bWaitInTheAir;		/// 공중에서 일정시간 있으면 wait상태로 바뀐다
//		float											m_fTimeInTheAir;		/// 공중에서 이 시간만큼 있으면 wait 상태로 바뀐다
//#endif LINEMAP_FAST_WIND_TEST

#ifndef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		bool											m_bDoubleAttack;		/// 더블어택 성공여부
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		wstring											m_wstrHitSound;			/// 더블어택성공시 마지막 hit 사운드
		float											m_fFinalDamage;			/// 더블어택성공시 마지막 데미지
		D3DXVECTOR3										m_vImpactPoint;			/// 더블어택성공시 마지막 타격위치
		float											m_fDoubleAttackTime;	/// 더블어택 표시 시간 차

		//{{ 2011-04 에 패치될 청 메모 헤드샷
#ifdef	CHUNG_MEMO_01				
		bool											m_bHittedExtraDamageHeadShot;	/// 내가 만들었지만 참.. 문제다.. EDT_HEAD_SHOT에 맞았으면
#endif	CHUNG_MEMO_01				
		//}} 2011-04 에 패치될 청 메모 헤드샷

		std::vector< CX2EffectSet::Handle > m_vecEffectSetToDeleteOnDamageReact;	/// 데미지 리액트가 실행 되면 삭제 되야 하는 이펙트 셋의 핸들
		std::vector< CX2EffectSet::Handle > m_vecEffectSetToDeleteOnStateEnd;	/// 이펙트를 발동 했을 때의 상태가 끝나면 삭제 되야 하는 이펙트 셋의 핸들
		std::vector< CX2EffectSet::Handle > m_vecEffectSetToDeleteOnDie;	/// 죽으면 해제 되야 하는 이펙트 셋의 핸들
#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
		std::vector< CX2EffectSet::Handle > m_vecEffectSetToDeleteOnCustomState;	/// 특정 스테이트가 실행되면 삭제 되야 하는 스테이트의 핸들
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		std::vector< EffectSetToPlay > m_vecEffectSetToPlay;	/// 플레이 되야 하는 이펙트 셋

		bool m_bDeleteEffectSetOnStateEnd;	/// m_vecEffectSetToPlay에 있는 것 들을 m_vecEffectSetToDeleteOnStateEnd에 넣어야 하는지 여부
		bool m_bDeleteEffectSetOnDamageReact;	/// m_vecEffectSetToPlay에 있는 것 들을 m_vecEffectSetToDeleteOnDamageReact에 넣어야 하는지 여부
		bool m_bDeleteEffectSetOnDie;	/// m_vecEffectSetToPlay에 있는 것 들을 m_vecEffectSetToDeleteOnDie에 넣어야 하는지 여부
#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
		bool m_bDeleteEffectSetOnCustomState;	/// m_vecEffectSetToPlay에 있는 것 들을 m_vecEffectSetToPlay에 넣어야 하는지 여부
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE
#ifdef CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT
		bool m_bIsCustomStateDeleteEffectOnDamageReact;
#endif // CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT

		bool m_bHyperEffectSet;	/// 이펙트 셋에 HYPER_NAME을 지정하고, 이것을 설정 하면 HYPER_NAME으로 지정된 이펙트를 실행
		float m_fEffectSetLifeTime;	/// 이펙트 셋의 플레이 시간...
#ifdef ADDITIONAL_MEMO
		int	m_iMemoId;
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        std::set<CX2DamageEffect::CEffectHandle>		    m_setEffectiveBlackHoleDamageEffect;	/// 블랙홀Data를 쓰고 있는 이펙트에 끌려가거나 맞은 경우에 이곳에 데이터가 insert 된다.	
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		std::set<CX2DamageEffect::CEffect*>		m_setEffectiveBlackHoleDamageEffect;	/// 블랙홀Data를 쓰고 있는 이펙트에 끌려가거나 맞은 경우에 이곳에 데이터가 insert 된다.	
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


		bool m_abEnableDash[EDS_END];	/// dash가 가능한지 아닌지
		bool m_bEntangled;	/// 인탱글에 걸렸는지 아닌지?
		D3DXVECTOR3 m_vEntangledPosition;		/// 인탱글 위치...라... 싱크를 맞추기 위해 필요한 걸까..?
		bool m_bHitedDrainMana;		/// 마나를 뺐어가는 공격에 맞았는지...



#ifdef CLOSE_SOUND_TEST
		CX2SoundCloseManager m_SoundCloseManager;
#endif CLOSE_SOUND_TEST

#ifdef DUNGEON_ITEM
		BUFFER_FACTOR	m_AttackBuffer;		/// 공격력 배율
		BUFFER_FACTOR	m_DefenceBuffer;	/// 방어력 배율
		BUFFER_FACTOR	m_PhysicAttackBuffer;	/// 물리 공격력 배율
		BUFFER_FACTOR	m_MagicAttackBuffer;	/// 마법 공격력 배율
		BUFFER_FACTOR	m_PhysicDefenceBuffer;	/// 물리 방어력 배율
		BUFFER_FACTOR	m_MagicDefenceBuffer;	/// 마법 방어력 배율
#endif

		float			m_fNoSkillTime; /// 유저한테만 있어도 될듯... 스킬을 사용할 수 없는 시간

//{{ kimhc // 2010.4.2 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
		/// 이런걸 여기다 넣다니.. 나도 참..
		BUFFER_FACTOR					m_StatUpBufferBySocketItem;		/// 아이템의소켓옵션에 의한 버프효과
		CX2SocketItem::STAT_UP_TYPE		m_StatUpType;					/// 어떤 스탯이 증가 할 것인가?

		// 이것도.. 배열로 하면 문제 있음..
		// 상태이상 제거를 위해 사용할 현재 유닛의 ExtraDamageData의 주소를 담고 있는 변수
		// 이 배열을 남은 상태이상 시간순으로 정렬하여 상태이상을 제거함(time을 0으로 만듬)
		CX2DamageManager::ExtraDamageData*		m_pExtraDamageDataList[NUM_OF_EXTRA_DAMAGE_PACK_MEMBER];
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.2 // 비밀던전 작업(셋트아이템효과)


#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
		CX2UnitSlashTraceManager* m_pUnitSlashTraceManager;	/// 무기외의 몸에 나오는 TRACE
#endif UNIT_SLASH_TRACE_MANAGER_TEST





//#ifdef HEAD_INVERSE_KINEMATICS_TEST
//		bool m_bEnableHeadIK;
//		float m_fHeadIKAngleChangeRate;		// 회전각도가 점진적으로 적용된다. 
//		float m_fHeadIKAngleMax;
//		float m_fHeadIKAngle;
//		float m_fHeadIKRadius;
//#endif HEAD_INVERSE_KINEMATICS_TEST



//#ifdef STEP_ON_MONSTER_TEST
//		bool m_bCanBeSteppedOn; // 이 유닛위에 다른 유닛이 올라설 수 있다.
//#endif STEP_ON_MONSTER_TEST


//#ifdef STEP_ON_MONSTER_COLLISION_BOX_TEST
//		bool m_bCanBeSteppedOnCollsionBox; // 이 유닛의 충돌박스위에 다른 유닛이 올라설 수 있다.
//#endif STEP_ON_MONSTER_COLLISION_BOX_TEST





//#ifdef HEAD_ATTACHED_CAMERA_TEST
//		D3DXVECTOR3 m_vBoneAttachedCameraOffset;
//#endif HEAD_ATTACHED_CAMERA_TEST



		SkillRelatedData m_SkillRelatedData;	/// 몇몇 스킬 데이터 들인데... 유저만 사용


		float m_fLastPositionValueOnAnimatedLine;	/// 이전 frame에 움직이는 라인위에 있었을 경우에 위치값
		D3DXVECTOR3 m_vLastPositionOnAnimatedLine;	/// 이전 frame에 움직이는 라인위에 있었을 경우에 위치값

		

		CommonState m_CommonState;		/// 각 유닛들의 기본적인 상태ID 저장



#ifdef RESET_INVALID_UNIT_POSITION_TEST
		bool m_bMisplaced;
		CKTDXTimer m_TimerMisplaced;
#endif RESET_INVALID_UNIT_POSITION_TEST



#ifdef OVERKILL_TEST
		bool m_bOverKilled;
#endif OVERKILL_TEST



#ifdef REVERSE_GRAVITY_TEST
		float m_fReverseGravitySpeedDeltaY;		/// 리버스 그래비티 띄우는 증가량
		D3DXVECTOR3 m_vReverseGravityRotateSpeedDelta; /// 리버스 그래비티에 의해 애니메이션 로테이트 변경// oasis907 : 김상윤 [2011.6.29]
#endif REVERSE_GRAVITY_TEST



#ifdef BOUNDING_COMBO_TEST
		bool m_bDamageBoundingReserved;
		float m_fDamageBoundingSpeed;
#endif BOUNDING_COMBO_TEST

		bool m_bImmuneToExtraDamage;		/// 모든 extra damage 에 저항, 몬스터에만 구현되어있음



//#ifdef TRANSFORMER_TEST
//
//		bool m_bIsTransformed;
//		UidType m_TransformerOwnerUID;
//		
//#endif TRANSFORMER_TEST

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		std::vector<TIME_SPEED>         m_vecSpeedFactor;	/// 유저만 사용중, 스크립트 상에서 특정 시간대에 스피드를 줄때 사용
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef RAVEN_SECOND_CLASS_CHANGE
		// 레이븐만 사용.. 나소드 핸드
		bool							m_bChangeDamageType;	/// 데미지 타입이 변경 되었는지..
		CX2DamageManager::DAMAGE_TYPE	m_eDamageType;	/// 어떤 데미지 타입으로 변경 되었는지..
		float							m_fDamageRel;	/// 변경된 배율
		int								m_iDamageTimes;	/// 이건.. 일섬에서 사용하는 것 같은데..
#endif

#ifdef DAMAGE_HISTORY
		bool	m_bDamageHistory;	/// 핵방지용.. NPC만 사용
		float	*m_fDamageHistory;	/// 유저는 사용 안해되 될텐데.. NPC가 데미지를 입을때마다 여기에 더하고, 죽을 때 이 값이 <= 0 이면 해킹 유저로 판단
#endif

//{{ kimhc // 2010.2.17 //
#ifdef	APPLY_MOTION_OFFSET
		bool	m_bApplyMotionOffset;	/// 모션에 의해 이동되지 않도록 하는 플래그로... 유저만 사용
#endif	APPLY_MOTION_OFFSET
//}} kimhc // 2010.2.17 //

#ifndef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef SKILL_BALANCE_PATCH
		bool	m_bCanPassUnit;		// 다른 유닛이 패스 할 수 있느냐 인것 같은데.. 유저의 CommonStateStartFuture에 루아 파싱 부분을 넣으면 이 부분을 삭제해도 될 듯..
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SERV_SKILL_NOTE
		float m_fRevengeDefenceTime;	/// 엘소드 반격 인듯.. 엘소드만 사용
		float m_fEnduranceAttackTime;	/// 고통억제 인가?
		float m_fPoisonDeBuff;	/// 포이즌에 걸림
#endif
		//{{ JHKang / 강정훈 / 2010/11/26 / 아토믹 쉴드를 통한 방어력 증가 수치
#ifdef NEW_SKILL_2010_11
		float m_fAtomicShieldDamageRate;		/// 아토믹 쉴드 캐릭터 데미지 감소 비율

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		// kimhc // 2010-12-08	
		std::vector<BUFF_TEMPLET_ID>	m_vecImmunityAtThisState;	/// 각각의 스테이트에서만 Immune 되어야할 ExtraDamage 지정.. 근데 여기에 놓고선 유저한테만 적용 되도록 해놨군... 수정!!해야함
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#endif NEW_SKILL_2010_11
		//}} JHKang / 강정훈 / 2010/11/26 / 아토믹 쉴드를 통한 방어력 증가 수치

		bool	m_bForceChagneColor;	/// 유닛의 칼라를 변경 시켜줘야 하는가? (물 라인맵)
		bool	m_bCheckChangeUnderWater;	/// 물에 들어가고 나오고를 체크해야하는 상황인지..
		bool	m_bChangeUnderWater;	/// 이 값이 true면 풍덩 소리와 물방울이 튀는듯... 그리고는 다시 false로 바꿔줌.. (즉, 물에 들어가는 순간에만 true)
		bool	m_bUnderWaterHead;		/// 물 라인맵에 풍덩 했는지... 물에 들어갔으면 pRenderParam->color -= m_cLineUnitColor 값을 매 프레임 한다.
		D3DXCOLOR m_cLineUnitColor;		/// 라인맵에 의해서 변경 되야 하는 유닛 색
		//{{ JHKang / 강정훈 / 2011/01/13 / 전체 렌더 타입 변경 확인(유지할 렌더 타입이 지정)
#ifdef SEASON3_MONSTER_2010_12
		CKTDGXRenderer::RENDER_TYPE	m_BasicRenderType;	/// 몬스터의 스테이트 마다 RenderpParam 값 변경... 근데 그러고 나서는 어떻게 되돌아 오나?? NPC만 사용
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2011/01/13

		bool m_bScaleEffectSetByNpc;	/// NPC만 사용, 유닛의 Scale에 따라서 EffectSet의 Scale을 변경 할 것인지

#ifdef BALANCE_PATCH_20110303
		float	m_fMaxDownForce;	/// 한 프레임에서 얻은 최대 포스다운 수치.. 이것을 포스다운 게이지에 더해 줌
#endif

		float	m_fExtraBigHead;	/// BIG_HEAD 익스트라 데미지를 받았는지... 이거 관련한게 OnFrameMove에서 퍼포먼스를 많이 먹는군...

#ifdef CHUNG_SECOND_CLASS_CHANGE
		// oasis907 : 김상윤 [2011.6.20] 청 2차 전직 - 아이언 팔라딘
		float	m_fAntiCriticalRate;	/// 해당 값 만큼 타격 받을 때 크리티컬 확률을 감소시킴
		float	m_fDebuffTransitCompleteTimeLeft;	/// 자신의 ExtraDamage를 바로 없애지 않고, 광역데미지의 전파를 위해 (다른 몬스터도 ExtraDamage를 가져갈 기회를 준다) 없애기 전에 딜레이를 준다.
		vector<KBuffFactor>	m_vecKDebuffFactorListToTransit;		/// 전이할 디버프의 Factor 목록

		bool	m_bEnableWeaponRenderEffect;	/// 웨폰 렌더 이펙트를 사용하고 있는지..
		float	m_fFadeTimeLeft;				/// 얼마 동안 페이드가 적용 될 것인가?
		float	m_fFadeChangeTime;				/// 몇초 간격으로 바뀌는 지
		float	m_fFadeChangeTimeLeft;			/// 몇초 간격으로 바뀌는 지에 대하여 실제 변화되고 남은 시간 즉.. m_fFadeChangeTimeLeft = m_fFadeChangeTime - elpasedTime 인듯

		float	m_fPulseDuration;				/// 지속시간
		float	m_fPulsePeriod;					/// 얼마간격으로..
		float	m_fPulseTimeLeft;				/// 남은 시간인듯...
	
		PULSE_WEAPON_COLOR_ID m_eNowPulseWeaponColorID;		/// 펄스웨폰적용 아이디

		D3DXCOLOR	m_WeaponRenderEffectColorStart;		/// 변화 시작 시의 칼라
		D3DXCOLOR	m_WeaponRenderEffectColorEnd;		///	변화 끝나는 시점의 칼라
		D3DXCOLOR	m_WeaponRenderEffectColor;	/// 현재 칼라
#endif CHUNG_SECOND_CLASS_CHANGE


#ifdef SERV_PVP_NEW_SYSTEM
		bool		m_bPvpBot;		/// 대전 NPC 인지 아닌지... NPC로 빼자..
#endif

#ifdef NEW_HENIR_TEST

#ifndef ADD_HENIR_BUFF		/// 버프로 변경되면서 쓰이지 않게 된 구문
		float		m_fTimeHenirBuff_Fire;		/// 헤니르 시공 fire 버프 유지 시간
		float		m_fTimeHenirBuff_Water;		/// 헤니르 시공 water 버프 유지 시간
		float		m_fTimeHenirBuff_Nature;	/// 헤니르 시공 nature 버프 유지 시간
		float		m_fTimeHenirBuff_Wind;		/// 헤니르 시공 wind 버프 유지 시간
		float		m_fTimeHenirBuff_Light;		/// 헤니르 시공 light 버프 유지 시간
#endif ADD_HENIR_BUFF

#endif NEW_HENIR_TEST

//{{ robobeg : 2013-06-12
// CX2GUNPC 로 옮겨감
//#ifdef SECRET_VELDER
//		KProtectedType<bool>	m_bGlobalSuperArmor;		/// m_fGlobalTimeSuperArmor 시간동안 무조건 수퍼아머..
//		KProtectedType<float>	m_fGlobalTimeSuperArmor;	/// 글로벌 수퍼아머 시간
//#endif
//}} robobeg : 2013-06-12

#ifdef HIT_PARTICLE_TRACE_UNIT
#ifdef  X2OPTIMIZE_STL_CONTAINER_USAGE
        typedef std::vector<CKTDGParticleSystem::CParticleEventSequenceHandle>  SeqHitParticleTraceUnitContainer;
#else   X2OPTIMIZE_STL_CONTAINER_USAGE
        typedef std::list<CKTDGParticleSystem::CParticleEventSequenceHandle>    SeqHitParticleTraceUnitContainer;
#endif  X2OPTIMIZE_STL_CONTAINER_USAGE
		SeqHitParticleTraceUnitContainer    m_listSeqHitParticleTraceUnit;
#endif HIT_PARTICLE_TRACE_UNIT

#ifdef SPECIAL_USE_ITEM
		float		m_fSpecialItemBuff_Wind;
#endif


#ifdef SERV_ARME_DIMENSION_WITCH
		float		m_fHitCreateDamageEffectRate;
		float		m_fHitCreateDamageEffectPowerRate;
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_ArrowOfExplosion;		/// 폭발의 화살 이모티콘 핸들
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hStartOfDelayedFiringOverlap;		/// 지연의 신호탄 중첩수 출력 이모티콘
		CX2EffectSet::Handle								m_hActiveStartOfDelayedFiringEDT;	/// 지연의 신호탄 적용중 확인 이펙트
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		float		m_fTacticalFieldDecreaseDamageValue;		/// 택티컬 필드에 의해 감소되는 데미지 수치
#endif SERV_CHUNG_TACTICAL_TROOPER


		bool		m_bNeedToUpdateWeaponEffect;		/// 광폭화 모드, 매지컬 메이크업등에 의해 변했거나, 다시 일반모드로 변했을 때 무기에 붙은 이펙트를 한번씩만 갱신하기 위한 플래그
		bool		m_bShowSmallGageAndName;				/// SmallGage와Name이 보여야 하는지 여부(거리 및 카메라와 캐릭터의 외적 값에 의해서 판단)

		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_Head;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_R_Foot;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_L_Foot;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_R_UpperArm;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_L_UpperArm;

		float		m_fForceHyperModeTime;			/// 강제 각성모드 발동시에 참조할 각성시간
	
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		DWORD		m_dwFrameMoveCountNow;			/// CommonFrameMove 수행회수(X2GameUnit이 소멸되기 전까지 계속 증가)
//		DWORD		m_dwFrameMoveCountFuture;		/// CommonFrameMoveFuture 수행회수(X2GameUnit이 소멸되기 전까지 계속 증가)
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		UCHAR		m_ucHittedCount;				/// 피격 받은 횟수
		UCHAR		m_ucHitCount;					/// 타격 한 횟수

		UCHAR		m_ucNumOfDeBuff;				/// 디버프 개수
		UCHAR		m_ucNumOfBuff;					/// 버프 개수
		CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION		m_eUnitSlashTraceConditionToDo;		/// 실행되야 하는 UnitSlashTrace 종류

		float							m_fRunJumpSpeedRate;			/// 무기 속성 인챈트 프로즌과 관련한 임시 변수 (속성 인챈트를 버프로 변경 하면 없앨 예정)

		vector<CX2ReverseLefRightPtr>	m_vecReverseLeftRightPtr;		/// 좌우반전 효과
		vector<CX2BuffIdentityPtr>		m_vecDownImpossiblePtr;			/// 하단이동불가 효과
		vector<CX2BuffIdentityPtr>		m_vecDashImpossiblePtr;			/// 대시불가 효과
		vector<CX2BuffIdentityPtr>		m_vecAttackImpossiblePtr;		/// 공격불가 효과
		vector<CX2BuffIdentityPtr>		m_vecSkillImpossiblePtr;		/// 스킬공격불가 효과
		vector<CX2BuffIdentityPtr>		m_vecZXImpossiblePtr;			/// ZX공격불가 효과
		vector<CX2BuffIdentityPtr>		m_vecSuperArmorPtr;				/// 수퍼아머 효과
		vector<CX2NeverMoveByBuffPtr>	m_vecNeverMovePtr;				/// 현재위치에 고정 효과

		CX2AdditionalStatByBuff			m_AdditionalMaxHp;				/// 최대Hp변경효과
		CX2AdditionalStatByBuff			m_AdditionalPhysicAttack;		/// 물리공격력변경효과
		CX2AdditionalStatByBuff			m_AdditionalMagicAttack;		/// 마법공격력변경효과
		CX2AdditionalStatByBuff			m_AdditionalPhysicDefence;		/// 물리방어력변경효과
		CX2AdditionalStatByBuff			m_AdditionalMagicDefence;		/// 마법방어력변경효과
		CX2AdditionalStatByBuff			m_AdditionalAnimSpeed;			/// 동작속도변경효과
		
		CX2AdditionalStatByBuff			m_AdditionalWalkSpeed;			/// 걷기속도변경효과
		CX2AdditionalStatByBuff			m_AdditionalDashSpeed;			/// 대시속도변경효과

		CX2AdditionalStatByBuff			m_AdditionalJumpPower;			/// 점프력변경효과
		CX2AdditionalStatByBuff			m_AdditionalDashJumpPower;		/// 대시점프력변경효과
		
		CX2AdditionalStatByBuff			m_AdditionalAccuracy;			/// 명중변경효과
		CX2AdditionalStatByBuff			m_AdditionalAvoidance;				/// 회피변경효과
		CX2AdditionalStatByBuff			m_AdditionalCriticalRate;		/// 크리티컬성공확률변경효과
		CX2AdditionalStatByBuff			m_AdditionalCriticalAttack;		/// 크리티컬성공시공격력변경효과
		CX2AdditionalStatByBuff			m_AdditionalAntiCriticalRate;	/// 크리티컬방어확률변경효과
		CX2AdditionalStatByBuff			m_AdditionalCriticalDefence;	/// 크리티컬피격시방어력변경효과
		CX2AdditionalStatByBuff			m_AdditionalChangeAdditionalAttack;		/// 추가데미지변경효과
		CX2AdditionalStatByBuff			m_AdditionalChangeAdditionalDefence;	/// 데미지감소변경효과
		
		vector<CX2StatFactorByBuffPtr>	m_vecAbsorbEffectAttackPtr;	/// 이펙트공격흡수
		
		CX2AdditionalStatByBuff			m_AdditionalChargeMpHitMeleeAttack;		/// 근접 타격시 MP회복량 배율
		CX2AdditionalStatByBuff			m_AdditionalChargeMpHitRangeAttack;		/// 원거리 타격시 MP회복량 배율

		CX2AdditionalStatByBuff			m_AdditionalChargeMpHitted;		/// 피격시 MP회복량 배율

		CX2AdditionalStatByBuff			m_AdditionalFinalDamageMultiplierMeleeAttack;	/// 근접공격 주는 데미지 변경
		CX2AdditionalStatByBuff			m_AdditionalFinalDamageMultiplierRangeAttack;	/// 원거리(DAMAGE_EFFECT)공격 주는 데미지 변경
		CX2AdditionalStatByBuff			m_AdditionalFinalDefenceMultiplier;	/// 받는 데미지 변경

		CX2AdditionalStatByBuff			m_AddtionalForceDownMultiplierMeleeAttack;	/// 근접 공격에 대한 ForceDown 감소 배율
		CX2AdditionalStatByBuff			m_AddtionalForceDownMultiplierRangeAttack;	/// 원거리 공격에 대한 ForceDown 감소 배율
		
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeBlazeDefence;	/// 속성저항변경효과
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeWaterDefence;	/// 속성저항변경효과
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeNatureDefence;	/// 속성저항변경효과
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeWindDefence;		/// 속성저항변경효과
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeLightDefence;	/// 속성저항변경효과
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeDarkDefence;		/// 속성저항변경효과

		CX2AdditionalStatByBuff			m_AdditionalDrainHpNormalAttack;			/// 피흡

		CX2AdditionalStatByBuff			m_AdditionalMaxMp;							/// 최대Mp변경효과

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		CX2AdditionalStatByBuff			m_AdditionalHyperCharge;					/// 각성 충전 속도
#endif //SERV_ADD_LUNATIC_PSYKER

		vector<CX2StunPtr>				m_vecStunPtr;		/// 기절 효과
		vector<CX2FreezePtr>			m_vecFreezePtr;		/// 빙결 효과
		vector<CX2BuffIdentityPtr>		m_vecAttackAllTeamPtr;	/// 모든팀 공격 효과

		vector<CX2ChangeAttackByTypePtr>		m_AdditionalAttackByType;				/// 타입별공격력변경효과

		vector<CX2ChangeEnchantAttackRatePtr>	m_AdditionalChangeEnchantAttackRate;	/// 소켓속성공격발동확률효과

		vector<CX2DoubleAttackPtr>				m_vecDoubleAttack;						/// 더블 어택

		/// 버프에 의해 변경될 수 있는 RenderParam 값 중, 스크립트 또는 월드 등에 의해서 적용되는 기본 RenderParam값
		/// RenderParam을 변경시키는 버프 리스트가 empty 이면, 이 값으로 돌아온다
		StBuffRenderParam				m_stOriginBuffRenderParam;
		vector<CX2RenderParamByBuffPtr>	m_vecRenderParamByBuffPtr;	/// 버프에 의해 변경될 RenderParam 리스트들

		vector<DelegateProcessWithDamageData>			m_vecDelegateProcessInAttackResult;		/// AttackResult 공격 성공시에만 실행되는 델리게이트 모음
		vector<DelegateProcess>							m_vecDelegateProcessInDamageReact;		/// DamageReact 시에만 실행되는 델리게이트 모음
		vector<DelegateProcess>							m_vecDelegateProcessInCommonFrameMove;	/// CommonFrameMove 에서 실행되는 델리게이트 모음
		vector<DelegateProcessWithDamageData>			m_vecDelegateProcessWithDamageDataInDamageReact;		/// DamageReact 시에만 실행되는 델리게이트 모음(DamageData 인자 사용)

		vector<CX2EffectSetImpactPointByBuffPtr>	m_vecEffectSetImpactPointByBuffPtr;	/// 타격시 타격포인트에 출력되야하는 이펙트셋리스트
		vector<CX2EffectSetImpactPointByBuffPtr>	m_vecEffectSetHittedByBuffPtr;		/// 피격시 출력되야하는 이펙트셋리스트

		vector<CX2ChangeUnitScaleByBuffPtr>			m_vecChangeUnitScaleByBuffPtr;		/// 버프에 의한 유닛의 크기 변경
		DelegateProcess	m_delegateProcessReturnFromScaleChange;			/// Y_PRESS에서 울렁이는 효과를 연출하면서 원래 스케일로 돌아오는 처리

		vector<CX2CustomFunctionByBuffPtr>			m_vecCustomFunctionByBuffPtr;		/// 버프에 의한 효과 중 StateStart, CommonFrameMove, StageEnd에서 처리해야하는 하드코딩을 실행
		vector<CX2AfterImageByBuffPtr>				m_vecAfterImageByBuff;				/// Element는 AfterImage의 색깔로, Element가 있다면 AfterImage가 Enable 되야함
		CKTDGXSkinAfterImage*						m_pAfterImage;
#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
		vector<CX2WeaponAfterImageByBuffPtr>		m_vecWeaponAfterImageByBuff;		/// Element는 무기의 AfterImage의 색깔로, Element가 있다면 AfterImage가 Enable 되야함
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE
		vector<DamageEffectGroupDataPtr>			m_vecDamageEffectGroupDataPtr;		/// 그룹 데미지 정보 저장 벡터
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-14
		vector<CX2EffectSetImpactPointByBuffPtr>	m_vecEffectSetNormalAttackByBuffPtr;	/// 일반 공격 타격시 출력되야 하는 이펙트셋리스트
		vector<CX2AddSkillLevelByBuffPtr>			m_vecAddSkillLevelByBuffPtr;			/// 버프에 의해 증감될 스킬레벨 리스트로 이 리스트의 값을 SUM 해서 사용 해야 함
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		vector<CX2ReflectMagicByBuffPtr>			m_vecReflectMagicByBuffPtr;				/// 버프에 의한 마법 반사 효과
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// Z 공격 불가, X 공격 불가 버프 타입 추가
		vector<CX2BuffIdentityPtr>		m_vecZImpossiblePtr;			/// Z 공격불가 효과
		vector<CX2BuffIdentityPtr>		m_vecXImpossiblePtr;			/// X 공격불가 효과
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#ifdef RIDING_SYSTEM
		CX2RidingPetPtr		m_ptrRidingPet;		/// 탈 것 포인트
		bool				m_bPassDash;		/// 패스 대쉬인지 여부, 알파값 처리용
#endif //RIDING_SYSTEM

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef MODIFY_EFFECT_SET_DELETE
		map< UINT, float > m_mapStateInsertBuffFactor;	// 특정 스테이트의 지정 시간이 되면 자기 자신에게 거는 버프팩터 리스트
#endif // MODIFY_EFFECT_SET_DELETE
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        bool                                        m_bPositionBackup;
        D3DXVECTOR3                                 m_vPositionBackup;
        int                                         m_iPositionBackup_LineIndex;
        bool                                        m_bPositionBackup_FootOnLine;

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		float										m_fFlickerElapsedTime;	/// 버프 BDT_RENDER_PARAM 의 color를 반짝이는 설정에 사용하는 시간 값
#endif //SERV_ADD_LUNATIC_PSYKER

#ifdef SERV_DROP_FOR_FINISHER_ONLY
		UidType										m_iFinisherUID;
#endif SERV_DROP_FOR_FINISHER_ONLY
};

IMPLEMENT_INTRUSIVE_PTR( CX2GameUnit::DamageEffectGroupData );