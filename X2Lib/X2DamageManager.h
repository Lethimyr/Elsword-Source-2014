#pragma once

class CX2GameUnit;
typedef KObserverPtr<CX2GameUnit>   CX2GameUnitoPtr;

class CX2GUUser;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

struct  CX2DamageEffect_CEffect_TAG {};
typedef KHandleType<int,CX2DamageEffect_CEffect_TAG>     CX2DamageEffect_CEffectHandle;
#define INVALID_DAMAGE_EFFECT_HANDLE    (CX2DamageEffect_CEffectHandle::invalid_handle())

#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
typedef int CX2DamageEffect_CEffectHandle;
static const CX2DamageEffect_CEffectHandle INVALID_DAMAGE_EFFECT_HANDLE = CX2DamageEffect_CEffectHandle(-1);

#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
class CX2DamageEffectBasic;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


class CX2DamageManager
{
	public:
#ifdef UPGRADE_RAVEN
		/// 타격시 트리거 ID
		enum DAMAGE_TRIGGER_TYPE
		{
			DTT_NONE	= 0,

			DTT_RAVEN_RAGE_ATTACK,
#ifdef EVE_ELECTRA
			DTT_EVE_THOUSAND_STAR,
#endif EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			DTT_MARK_OF_COMMANDER,
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 레나 컴뱃 레인저, 날카로운 발차기의 일부 액티브 스킬 MP 회수율 증가를 시켜주는 Trigger Type
			DTT_SHARP_KICK_RELATE_ACTIVE_SKILL_INCREASE_MP_RATE,					
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
			// 기동 코어 공격에 반응하지 않는 AT_SPECIAL 공격의 Trigger Type ( 이브만 해당 )
			DTT_EVE_MANEUVER_ATTACK,
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
			DTT_END,
		};
#endif

		/// DAMAGE 형태
		enum DAMAGE_TYPE
		{
			DT_PHYSIC = 0,			/// 물리 타격
			DT_MAGIC,				/// 마법 타격
			DT_MIX,					/// 물리,마법 공격

			DT_WEAPON_PHYSIC,		/// 무기의 물리공격력(스탯제외)
			DT_WEAPON_MAGIC,		/// 무기의 마법공격력(스탯제외)
			DT_WEAPON_MIX,			/// 무기의 물리, 마법 공격력(스탯제외)

			DT_BUFF,				/// 버프용. 이 공격타입은 피*타격시 마나 + 증가치, 피*타격시 각성 + 증가치, 길드 스킬 값에 영향을 주지 않는다
		};

		/// 공격 형태
		enum ATTACK_TYPE
		{
			AT_NORMAL = 0,
			AT_SPECIAL,
		};

		/// 타격 형식
		enum HIT_TYPE
		{
			HT_NO_SOUND = -1,	/// 없음
			HT_DEFAULT = 0,		/// 기본
			HT_SWORD_SLASH,		/// 칼 베기
			HT_SWORD_SLASH2,	/// 칼 베기2
			HT_SWORD_HIT,		/// 칼 타격
			HT_SWORD_CRASH,		/// 칼 충돌

			HT_ROD_SLASH,		/// 지팡이 베기
			HT_ROD_SLASH2,		/// 지팡이 베기2
			HT_ROD_HIT,			/// 지팡이 타격
			HT_ROD_CRASH,		/// 지팡이 충돌

			HT_PUNCH_HIT,		/// 주먹 치기
			HT_PUNCH_HIT2,		/// 주먹 치기2
			HT_PUNCH_CRASH,		/// 주먹 충돌

			HT_KICK_SLASH,		/// 발 차기
			HT_KICK_SLASH2,
			HT_KICK_HIT,
			HT_KICK_CRASH,
			
			HT_ARROW_HIT,
			HT_ARROW_HIT2,
			HT_ARROW_CRASH,
			
			HT_FIRE,
			HT_FIRE2,
			HT_BOMB,
			HT_WOOD,

			HT_LIGHTNING,

			HT_NASOD_KING,

			HT_METAL_PUNCH_HIT,
            
#ifdef GROUND_HIT
            HT_GROUND_HIT,
#endif
			HT_SLAP,				// 이브 뺨 때리기
			HT_EVE_SLASH1,
			HT_EVE_SLASH2,

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
//#ifdef	NEW_CHARACTER_CHUNG
			HT_CHUNG_SLASH1,
			HT_CHUNG_SLASH2,
			HT_CHUNG_SLASH3,
//#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

//{{ kimhc // 2011.1.21 // 청 1차 전직
//#ifdef	CHUNG_FIRST_CLASS_CHANGE
			HT_HANDGUN_HIT1,
			HT_HANDGUN_HIT2,
//#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // 청 1차 전직
			
#ifdef AREA_HIT
			HT_AREA_HIT,
#endif
			HT_SPEAR_HIT,


#pragma region 아라 창 무기용 사운드
#ifdef ARA_CHARACTER_BASE
			HT_POLE_BRANDISH_WEAK,		/// 창 약한 공격
			HT_POLE_BRANDISH_STRONG,	/// 창 강한 공격
			HT_SHORTSWORD_SLASH,		/// 단검 가르기
			HT_SHORTSWORD_PIERCE,		/// 단검 찌르기
#endif
#pragma endregion 

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			HT_ELECTRIC_1,				/// 전기히트 1
			HT_ELECTRIC_2,				/// 전기히트 2
			HT_PLASMA_1,				/// 플라즈마히트 1
			HT_PLASMA_2,				/// 플라즈마히트 2
			HT_PRESSURE_1,				/// 기합/공기압히트 1
			HT_PRESSURE_2,				/// 기합/공기압히트 2
			HT_WATER_1,					/// 액체히트 1
			HT_WATER_2,					/// 액체히트 2
			HT_WATER_3,					/// 액체히트 3
			HT_ICE,						/// 얼음히트
			HT_METAL,					/// 쇳덩이히트
			HT_WIND,					/// 바람히트
			HT_STONE,					/// 돌덩이 히트
#endif //SERV_9TH_NEW_CHARACTER

			HIT_TYPE_COUNT,
		};		
		
		//피격 종류 - 사운드 결정
		enum HITTED_TYPE
		{
			HTD_NO_SOUND = -1,
			HTD_DEFAULT	= 0,
			HTD_MEAT,
			HTD_WOOD,
			HTD_STONE,
			HTD_STONE2,
			HTD_METAL,
			HTD_METAL2,
			HTD_BEEHOUSE,
//{{ kimhc // 2011.1.21 // 청 1차 전직
//#ifdef	CHUNG_FIRST_CLASS_CHANGE
			HTD_GUARD_1,
//#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // 청 1차 전직
//#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			HTD_GUARD_2,
//#endif //UPGRADE_SKILL_SYSTEM_2013
			HITTED_TYPE_COUNT,
		};

		/// 명중 반응 종류
		enum REACT_TYPE
		{
			RT_NO_DAMAGE = 0,			/// 무적이거나 등등 아무런 리액션도 주지 못함			
			RT_SMALL_DAMAGE,			/// 작은 데미지
			RT_BIG_DAMAGE,				/// 큰 데미지
			RT_DOWN,					/// 쓰러지기
			RT_UP,						/// 위로 쳐올리기
			RT_FLY,						/// 날아가서 넘어지기
			RT_REVENGE,					/// 반격기 경직
			RT_DUMMY_DAMAGE,			/// 맞는 표시가 안나는 데미지
			RT_GROGGY,					/// 비틀거리는 상태
			RT_GUARD,					/// 방어
            RT_SENSOR,          		/// 센서타입 (NO 데미지)
			RT_CREATE_DAMAGE_EFFECT,	/// 맞은 위치에 새로운 데미지 이펙트를 생성한다
			RT_CREATE_EFFECT_SET,		/// 
			RT_GRAPPLE,					/// 잡기 공격
			RT_THROW,					/// 던지기
			RT_BOUNDING,				///
			RT_DRAG_UP,					/// kimhc // 2010.02.08 // 위치보정 없이 위로 계속 끌어 올리는 공격
//#ifdef NEW_SKILL_2010_11
			RT_REFLECT,					// oasis907 : 김상윤 [2010.11.1] // 룬 슬레이어 - 마법저항 수련
//#endif NEW_SKILL_2010_11

			//{{ kimhc // 2011.1.8 // 청 1차 전직
			RT_FLIP_SAME_DIR,			/// kimhc // 2011.01.08 // 유저와 같은 방향을 바라보도록 회전(이미 같은 방향을 보고 있는 경우에는 회전하지 않음, 그외에는 기본적으로 Small_Damage_Type과 같게 처리)
			//}} kimhc // 2011.1.8 // 청 1차 전직

// #ifdef NEW_GRAPPLE_ATTACK
// 			RT_NEW_GRAPPLE,				/// lucidash :  잡기공격. 
// #endif NEW_GRAPPLE_ATTACK
//#ifdef SEASON3_MONSTER_2011_7
			RT_ABSORB_MAGIC,
//#endif SEASON3_MONSTER_2011_7
//#ifdef ARA_CHANGE_CLASS_FIRST
			RT_AVOIDANCE,				/// 회피
//#endif //ARA_CHANGE_CLASS_FIRST
// #ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
			RT_FLIP_DIFF_DIR,					/// kimjh, 유저와 다른 방향을 바라보도록 ( 서로 마주보도록 ) 회전 // 도발에 사용
			RT_FLIP_DIFF_DIR_DUMMY_DAMAGE,		/// kimjh, 유저와 다른 방향을 바라보도록 ( 서로 마주보도록 ) 회전 // 도발에 사용
// #endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

		};

		enum REACTER_TYPE
		{
			AT_UNIT = 0,
			AT_EFFECT,
			AT_MESH,
			AT_MANUAL_COLLISION_DATA,
		};

		enum EXTRA_DAMAGE_TYPE
		{
			EDT_NONE = 0,
			EDT_POISON,
			EDT_SLOW,
			EDT_FIRE,
			EDT_CURSE,
			EDT_SILENCE,
			EDT_LIGHTNING,
			EDT_REVERSE_LEFT_RIGHT,
			EDT_COLD,
			EDT_Y_PRESSED,



			EDT_FIRE_SMASH,				/// 드래고닉 무기의 화염강타, 중복 가능한 extra_damage
			EDT_COLD_SMASH,				/// 냉기 강타
			EDT_EVENT_SMASH,			/// 이벤트용으로 사용하는 강타 계열 extra_damage
			EDT_LIGHTNING_SMASH,		/// kimhc // 2010-07-30 // 셋트효과용 번개 강타 추가

			EDT_POISON_CLOUD,			/// 독구름
			EDT_DOUBLE_ATTACK,			/// 두번 공격

			EDT_MANA_DAMAGE,			/// MANA DAMAGE를 주는 Extra Damage, 마나이터 관련 

			EDT_FROZEN,					/// 얼음되서 못움직이고 데미지 입는
			EDT_LEG_WOUND,				/// 하체 상처, 지속적인 데미지, 대시 불가능, SI_A_RST_CUT_TENDON 관련
			EDT_WEAPON_BREAK,			/// 무기 파괴 
			EDT_ARMOR_BREAK,			/// 방어구 파괴 

			EDT_ENTANGLE,				/// 움직임 속도 0, 제자리 걸음. 피격시 다운 모션. 공격, 필살기 못함
			EDT_DULL,					/// 둔환 (SLOW와는 다르게 속성저항 적용하지 않음 / 지속데미지 없음)
			EDT_TIME_BOMB,				/// 시한 폭탄
			EDT_STUN,					/// 
			EDT_AGING,					/// 노화 (공속,이속,점속,공격력 감소) 
			EDT_DAMAGE_RATE,			/// 피격자 현재 hp의 특정비율로 데미지를 입힌다.			

			EDT_MAGIC_DEFENCE_DEBUFF, 
			EDT_STIGMA_DEBUFF,			/// 피격자가 받는 크리티컬 데미지를 증가시킨다 (낙인)

			EDT_DRYAD,					/// 인탱글과 효과는 동일하나, 자연저항이 적용되며, 드라이어드 이펙트가 따로 적용됨			
//#ifdef EDT_EVENT_SMASH_SECOND
			EDT_EVENT_SMASH_2,
//#endif EDT_EVENT_SMASH_SECOND

//#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰
			EDT_BLIND,
//#endif NEW_SKILL_2010_11

			//{{ kimhc // 2010.12.29 // 하멜 마을 추가시 들어가는 ExtraDamage
			EDT_ICE_STING,
			EDT_FIRE_BALL,
			//}} kimhc // 2010.12.29 // 하멜 마을 추가시 들어가는 ExtraDamage

//#ifdef EDT_WATER_HOLD_TEST   // lucidash :: 김상훈 [2010.1.4] 
			EDT_WATER_HOLD,
//#endif EDT_WATER_HOLD_TEST

			EDT_BIGHEAD,				// 유저에게 사용 불가 (user reform scale과 충돌남)
			//{{ 2011-04 에 패치될 청 캐시 스킬
//#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
			EDT_PANIC,					/// 좌우키 반전이 몇초 간격으로 걸렸다 풀렸다를 반복 & 크리티컬에 당할 확률 증가
			EDT_PAIN,					/// 상대방에게 걸려있는 ExtraDamage에 따라 폭발하는 횟수가 달라지는 ExtraDamage
			EDT_HEAD_SHOT,				/// 슈팅 가디언, 헤드샷 메모 장착 시 발동
//#endif	CASH_SKILL_FOR_CHUNG_2011_04				
			//}} 2011-04 에 패치될 청 캐시 스킬
			
			//{{ JHKang / 강정훈 / 2011.6.19 / LEG_SHOT EDT
//#ifdef CHUNG_SECOND_CLASS_CHANGE
			EDT_LEG_SHOT,		/// 이동, 점프 속도 감소
//#endif
			//}}
//#ifdef CHUNG_SECOND_CLASS_CHANGE // oasis907 : 김상윤 [2011.6.16] 청 2차 전직 - 아이언 팔라딘
			EDT_TRANSIT_DEBUFF,
//#endif CHUNG_SECOND_CLASS_CHANGE			
//#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
			EDT_WIND_STORM,
			EDT_EARTH_QUAKE,
//#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

//#ifdef VELDER_SECRET_DUNGEON
			EDT_CURSE_CLOUD,

			EDT_SCALE_CRITICAL_BUFF,
			EDT_BERSERKER_BUFF,
			EDT_MANA_BUFF,
			EDT_ATTACK_ALL_TEAM,
//#endif
//#ifdef UPGRADE_RAVEN
			EDT_RUNJUMP_SPEED,
//#endif
//#ifdef RAVEN_WEAPON_TAKER
			EDT_SMOKE_BLIND,
//#endif RAVEN_WEAPON_TAKER
//#ifdef EVE_ELECTRA
			EDT_FLASH_BANG,
//#endif EVE_ELECTRA
//#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			EDT_MARK_OF_COMMANDER,
//#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

//#ifdef ADD_SOCKET_FOR_SECRET_SET
			EDT_FLAME_SMASH,		/// 어택박스를 가지는 화염 타격
			EDT_FIRE_WRAITH,		/// 페이타 세트용 레이스 발사
//#endif

//#ifdef SERV_RENA_NIGHT_WATCHER
			EDT_START_OF_DELAYED_FIRING,	///mauntain : 김태환 [2012.05.21] 레나 2차 전직 나이트 와쳐 - 폭발의 화살
//#endif SERV_RENA_NIGHT_WATCHER

//#ifdef SERV_RAVEN_VETERAN_COMMANDER
			EDT_BLEEDING,			///mauntain : 김태환 [2012.07.16] 레이븐 3차 전직 베테랑 커맨더 - 치명상
//#endif SERV_RAVEN_VETERAN_COMMANDER

//#ifdef STARLIGHT_SMASH
			EDT_STARLIGHT_SMASH,	/// 별빛 강타 소켓
//#endif STARLIGHT_SMASH

//#ifdef CAMILLA_SMASH
			EDT_CAMILLA_SMASH,		/// 해외팀 추가 소켓
//#endif
//#ifdef ADD_SOCKET_OPTION_SANDER_SET
			EDT_FALLING_ROCK,		/// 샌더 셋트 효과 - 바위 소환 
			EDT_SANDSTORM,			/// 샌더 셋트 효과 - 모래돌풍 소환

			EDT_WIND_VIBRATION,		/// 샌더 칭호 
			EDT_BREEZE_ACCEL,
			EDT_EVIL_SWORD,			
			EDT_SPIRIT_SWORD,		
			EDT_GHOST_SWORD,
//#endif //ADD_SOCKET_OPTION_SANDER_SET

//#ifdef HAMEL_SECRET_DUNGEON // 김태환
			EDT_ACTIVE_DEBUFF,		/// 타격시 특정 디버프를 적용
//#endif // HAMEL_SECRET_DUNGEON

//#ifdef EDT_EVENT_SMASH_THIRD
			EDT_EVENT_SMASH_3,
//#endif EDT_EVENT_SMASH_THIRD

			//////////////////////////////////////////////////////////////////////////
			// 1개 속성 인챈트
			EDT_ENCHANT_BLAZE			= 100,
			EDT_ENCHANT_FROZEN,
			EDT_ENCHANT_POISON,
			EDT_ENCHANT_PIERCING,
			EDT_ENCHANT_SHOCK,
			EDT_ENCHANT_SNATCH,


			
			// 2개 속성 인챈트
			EDT_ENCHANT_BLAZE_MASTER,
			EDT_ENCHANT_FROZEN_MASTER,
			EDT_ENCHANT_POISON_MASTER,
			EDT_ENCHANT_MASTER_PIERCING,
			EDT_ENCHANT_MASTER_SHOCK,
			EDT_ENCHANT_MASTER_SNATCH,

			EDT_ENCHANT_BLAZE_PIERCING,
			EDT_ENCHANT_BLAZE_SHOCK,
			EDT_ENCHANT_BLAZE_SNATCH,

			EDT_ENCHANT_FROZEN_PIERCING,
			EDT_ENCHANT_FROZEN_SHOCK,
			EDT_ENCHANT_FROZEN_SNATCH,

			EDT_ENCHANT_POISON_PIERCING,
			EDT_ENCHANT_POISON_SHOCK,
			EDT_ENCHANT_POISON_SNATCH,


			// 3개 속성 ENCHANT
			EDT_ENCHANT_BLAZE_DOUBLE_MASTER,
			EDT_ENCHANT_FROZEN_DOUBLE_MASTER,
			EDT_ENCHANT_POISON_DOUBLE_MASTER,
			EDT_ENCHANT_DOUBLE_MASTER_PIERCING,
			EDT_ENCHANT_DOUBLE_MASTER_SHOCK,
			EDT_ENCHANT_DOUBLE_MASTER_SNATCH,
			EDT_ENCHANT_DOUBLE_BLAZE_PIERCING,
			EDT_ENCHANT_DOUBLE_BLAZE_SHOCK,
			EDT_ENCHANT_DOUBLE_BLAZE_SNATCH,
			EDT_ENCHANT_DOUBLE_FROZEN_PIERCING,
			EDT_ENCHANT_DOUBLE_FROZEN_SHOCK,
			EDT_ENCHANT_DOUBLE_FROZEN_SNATCH,
			EDT_ENCHANT_DOUBLE_POISON_PIERCING,
			EDT_ENCHANT_DOUBLE_POISON_SHOCK,
			EDT_ENCHANT_DOUBLE_POISON_SNATCH,
			EDT_ENCHANT_BLAZE_DOUBLE_PIERCING,
			EDT_ENCHANT_BLAZE_DOUBLE_SHOCK,
			EDT_ENCHANT_BLAZE_DOUBLE_SNATCH,
			EDT_ENCHANT_FROZEN_DOUBLE_PIERCING,
			EDT_ENCHANT_FROZEN_DOUBLE_SHOCK,
			EDT_ENCHANT_FROZEN_DOUBLE_SNATCH,
			EDT_ENCHANT_POISON_DOUBLE_PIERCING,
			EDT_ENCHANT_POISON_DOUBLE_SHOCK,
			EDT_ENCHANT_POISON_DOUBLE_SNATCH,

		};

#ifdef ELSWORD_WAY_OF_SWORD
		enum WAY_OF_SWORD_TYPE
		{
			WST_NONE,
			WST_VIGOR_NORMAL,
			WST_VIGOR_ACTIVE,
			WST_VIGOR_SPECIAL,
			WST_DESTRUCTION_NORMAL,
			WST_DESTRUCTION_ACTIVE,
			WST_DESTRUCTION_SPECIAL,			
		};
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		/// 공격 타입에 따른 DP 배율 수치
		/// 해당 타입은 Enum.lua에 정의 되어 있고, Add_NasodRuler.lua에 값이 매칭 되어 있다.
		/// 코드는 CX2GUAdd::GetDPRateByType 함수 내에서 연결중이다.
		enum DYNAMO_PARTICLE_RATE_TYPE
		{
			DPRT_NONE				= -1,
			DPRT_SPECIAL_ACTIVE		= 0,	/// 스페셜 액티브
			DPRT_ACTIVE				= 1,	/// 액티브 공격
			DPRT_COMMAND			= 2,	/// 커맨드 공격
			DPRT_NASOD_ARMOR		= 3,	/// 나소드 아머 공격
		};
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_RENA_SYSTEM //김창한
		enum NATURAL_FORCE_TYPE
		{
			NFT_NONE			= -1,	
			NFT_COMBO			= 0,	// 평타 공격
			NFT_COMBO_BOMB		= 1,	// 평타 막타 폭발 공격
			NFT_ACTIVE			= 2,	// 액티브 공격
			NFT_SPECIAL_ACTIVE	= 3,	// 스폐셜 액티브 공격
			NFT_RESULT_BOMB		= 4,	// NF를 소모하여 발생한 폭발
		};

		enum FIRST_ATTACK_CHECK
		{
			FAC_NOT_CHECK		= -1,	// 첫번째 어택이라는 것을 체크 하지 않음
			FAC_NONE			= 0,	// 아직 첫번째 어택이 되지 않음.
			FAC_FIRST_ATTACK	= 1,	// 첫번째 어택 판정이 이루어 짐.
		};
#endif //ADD_RENA_SYSTEM

#ifdef AREA_HIT
		enum DAMAGE_AREA_TYPE
		{
			DAT_ALL_UNIT = 0,
			DAT_NEAR_UNIT,
			DAT_FAR_UNIT,
		};

		struct sAreaHitUnit
		{
			int unitIndex;
			float fDistance;
		};
#endif

#ifdef SERV_RENA_NIGHT_WATCHER

		struct StartOfDelayedFiringData
		{
			int			m_iType;				/// 화살 타입( 폭발의 화살 인지 지연의 신호탄 인지 )
			int			m_iAttackerIndex;		/// 해당 화살 발사자
			int			m_iArrowOrderNumber;	/// 화살 발사 순서
			int			m_iArrowIndex;			/// 땅에 박힌 화살의 인덱스
			float		m_fCoolTime;			/// EDT 유지 시간
			float		m_fIncreaseDamage;		/// 해당 객체의 추가 데미지
			float		m_fSkillPowerRate;
			D3DXVECTOR3 m_vExplosionOffSet;		/// 유닛에게 폭발 될 이펙트 위치

			StartOfDelayedFiringData( int iType, int iAttackerIndex, int iArrowOrderNumber, int iArrowIndex = -1, float fCoolTime = 10.f, float fIncreaseDamage = 1.f )
			{
				m_iType				= iType;
				m_iAttackerIndex	= iAttackerIndex;
				m_iArrowOrderNumber = iArrowOrderNumber;
				m_iArrowIndex		= iArrowIndex;
				m_fCoolTime			= fCoolTime;
				m_fIncreaseDamage	= fIncreaseDamage;
				m_fSkillPowerRate	= 1.f;
				m_vExplosionOffSet  = D3DXVECTOR3( 0.f, 0.f, 0.f );
			}
		};

		struct StartOfDelayedFiringDataSort

		{
			bool operator()( const CX2DamageManager::StartOfDelayedFiringData* _Left, const CX2DamageManager::StartOfDelayedFiringData* _Right ) const
			{
				if( _Left->m_iArrowOrderNumber < _Right->m_iArrowOrderNumber )
				{
					return true;
				}
				else
				{
					return false;
				}
			};
		};

#endif SERV_RENA_NIGHT_WATCHER

		struct ExtraDamageData
		{
			EXTRA_DAMAGE_TYPE		m_ExtraDamageType;
			KProtectedType<float>	m_fTime;
			KProtectedType<float>	m_DamagePerSec;
			KProtectedType<int>		m_Accumulation;
			KProtectedType<float>	m_fElapsedTimeToEffectDamage;
			KProtectedType<float>	m_fRate;			/// 이 extra damage가 작동할 확률
#ifdef EXTRADAMAGE_RATE_BUG_FIX			
			KProtectedType<float>	m_fPureRate;		/// Rate 보정 때문에 따로 보관될 원본 Rate
#endif EXTRADAMAGE_RATE_BUG_FIX
			KProtectedType<float>	m_Damage;			/// 데미지

#ifdef NEW_EXTRA_DAMAGE
			KProtectedType<int>		m_iLevel;			/// 레벨
			KProtectedType<int>		m_iAddLevel;		/// 추가 레벨
			KProtectedType<bool>	m_bIgnoreResist;	/// 저항
#endif
#ifdef ADDITIONAL_MEMO
			bool					m_bDefenderLevel;
#endif //def ADDITIONAL_MEMO

#ifdef SKILL_CASH_10_TEST
			KProtectedType<float>	m_fDamageRate;		/// 데미지
			KProtectedType<float>	m_fRunJumpRate;		/// 이동, 점프 속도
			KProtectedType<float>	m_fAnimSpeedRate;	/// 애니메이션 속도
#endif

			bool					m_bFixedDamage;

			//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
			/// ExtraDamage를 Map으로 바꾸게 되면 삭제
			bool					m_bCorrectDirection;		/// Panic에서 키보드 입력에 따라 정방향으로 움직이는지 여부를 저장하는 플래그
			KProtectedType<float>	m_fCriticalRateToDefender;	/// 피격자가 크리티컬에 당할 확률 증가
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
			//}} 2011-04 에 패치될 청 캐시 스킬

			//{{ JHKang / 강정훈 / 2011.6.20 / 레그 샵 관련 이동, 점프 속도 비율 구분을 위한 변수 추가
#ifdef CHUNG_SECOND_CLASS_CHANGE
			KProtectedType<float>	m_fJumpRate;		/// 점프 속도(LEG_SHOT에서 m_fRunJumpRate를 이동 속도만 사용)
#endif
			//}}

			//{{ kimhc // 2011-07-29 // 칼라볼그 페인 버그 수정
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
			// 현재는 EDT_PAIN에서 누구에 의한 EDT인지를 구분하기 위한 용도로 사용
			CX2GameUnitoPtr			m_optrGameUnit;
#endif	CASH_SKILL_FOR_CHUNG_2011_04
			//}} kimhc // 2011-07-29 // 칼라볼그 페인 버그 수정
#ifdef ELSWORD_WAY_OF_SWORD			// CureExtraDamage - 버프 해제, 디버프 전이, 속박 해제등 ExtraDamage 해제에 앞으로 쓰일 수 있음.
			bool					m_bCured;
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_RENA_NIGHT_WATCHER
			vector<CX2DamageManager::StartOfDelayedFiringData*> m_vecStartOfDelayedFiringData;		/// 지연의 신호탄 적용 정보
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
			vector<float>				m_vecOverlapDamaage;	/// 치명상 중첩 데미지 정보
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef HAMEL_SECRET_DUNGEON // 김태환
			CX2BuffFactorPtr			m_ptrBuffFactor;		/// 적용할 버프 정보
#endif // HAMEL_SECRET_DUNGEON


#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
            bool                    m_bAttackedByMyUnit;        /// 내 유닛에 의해 공격당했는지
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

			ExtraDamageData()
			{
				Init();
			}
			void Init()
			{
				m_fRate							= 0.f;
#ifdef EXTRADAMAGE_RATE_BUG_FIX					
				m_fPureRate						= 0.f;
#endif EXTRADAMAGE_RATE_BUG_FIX					
				m_Damage						= 0.0f;
				m_ExtraDamageType				= EDT_NONE;
				m_fTime							= 0.f;
				m_DamagePerSec					= 0.f;
				m_Accumulation					= 0;
				m_fElapsedTimeToEffectDamage	= 0.f;

#ifdef NEW_EXTRA_DAMAGE
				m_iLevel						= -1;
				m_iAddLevel						= 0;
				m_bIgnoreResist					= false;
#endif
#ifdef ADDITIONAL_MEMO
				m_bDefenderLevel				= false;
#endif

#ifdef SKILL_CASH_10_TEST
				m_fDamageRate					= 0.f;
				m_fRunJumpRate					= 0.f;
				m_fAnimSpeedRate				= 0.f;
#endif
				m_bFixedDamage					= false;
				//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
				/// ExtraDamage를 Map으로 바꾸게 되면 삭제
				m_fCriticalRateToDefender		= 0.0f;
				m_bCorrectDirection				= true;	/// Panic에서 키보드 입력에 따라 정방향으로 움직이는지 여부를 저장하는 플래그
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
				//}} 2011-04 에 패치될 청 캐시 스킬

				//{{ JHKang / 강정훈 / 2011.6.20 / 레그 샵 관련 이동, 점프 속도 비율 구분을 위한 변수 추가
#ifdef CHUNG_SECOND_CLASS_CHANGE
				m_fJumpRate						= 0.f;
#endif
				//}}

				//{{ kimhc // 2011-07-29 // 칼라볼그 페인 버그 수정
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
				m_optrGameUnit.Reset();
#endif	CASH_SKILL_FOR_CHUNG_2011_04
				//}} kimhc // 2011-07-29 // 칼라볼그 페인 버그 수정
#ifdef ELSWORD_WAY_OF_SWORD
				m_bCured						= false;
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_RENA_NIGHT_WATCHER
				for( UINT i = 0; i < m_vecStartOfDelayedFiringData.size(); ++i )		/// 지연의 신호탄 정보 삭제
				{
					CX2DamageManager::StartOfDelayedFiringData* pData = m_vecStartOfDelayedFiringData[i];
					SAFE_DELETE( pData );
				}
				m_vecStartOfDelayedFiringData.erase( m_vecStartOfDelayedFiringData.begin(), m_vecStartOfDelayedFiringData.end() );
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
				m_vecOverlapDamaage.erase( m_vecOverlapDamaage.begin(), m_vecOverlapDamaage.end() );	/// 치명상 중첩 데미지 정보 삭제
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
                m_bAttackedByMyUnit = false;
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

			}
			void Copy( const ExtraDamageData& data )
			{
				m_fRate							= data.m_fRate;
#ifdef EXTRADAMAGE_RATE_BUG_FIX					
				m_fPureRate						= data.m_fPureRate;
#endif EXTRADAMAGE_RATE_BUG_FIX					
				m_Damage						= data.m_Damage;
				m_ExtraDamageType				= data.m_ExtraDamageType;
				m_fTime							= data.m_fTime;
				m_DamagePerSec					= data.m_DamagePerSec;
				m_Accumulation					= data.m_Accumulation;
				m_fElapsedTimeToEffectDamage	= data.m_fElapsedTimeToEffectDamage;

#ifdef NEW_EXTRA_DAMAGE
				m_iLevel						= data.m_iLevel;
				m_iAddLevel						= data.m_iAddLevel;
				m_bIgnoreResist					= data.m_bIgnoreResist;
#endif
#ifdef ADDITIONAL_MEMO
				m_bDefenderLevel				= data.m_bDefenderLevel;
#endif
#ifdef SKILL_CASH_10_TEST
				m_fDamageRate					= data.m_fDamageRate;
				m_fRunJumpRate					= data.m_fRunJumpRate;
				m_fAnimSpeedRate				= data.m_fAnimSpeedRate;
#endif
				m_bFixedDamage					= data.m_bFixedDamage;
				//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
				/// ExtraDamage를 Map으로 바꾸게 되면 삭제
				m_fCriticalRateToDefender		= data.m_fCriticalRateToDefender;
				m_bCorrectDirection				= data.m_bCorrectDirection;	/// Panic에서 키보드 입력에 따라 정방향으로 움직이는지 여부를 저장하는 플래그
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
				//}} 2011-04 에 패치될 청 캐시 스킬

				//{{ JHKang / 강정훈 / 2011.6.20 / 레그 샵 관련 이동, 점프 속도 비율 구분을 위한 변수 추가
#ifdef CHUNG_SECOND_CLASS_CHANGE
				m_fJumpRate						= data.m_fJumpRate;
#endif
				//}}

				//{{ kimhc // 2011-07-29 // 칼라볼그 페인 버그 수정
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
				m_optrGameUnit					= data.m_optrGameUnit;		// swallow copy
#endif	CASH_SKILL_FOR_CHUNG_2011_04
				//}} kimhc // 2011-07-29 // 칼라볼그 페인 버그 수정
#ifdef ELSWORD_WAY_OF_SWORD
				m_bCured						= data.m_bCured;
#endif ELSWORD_WAY_OF_SWORD

#ifdef HAMEL_SECRET_DUNGEON // 김태환
				m_ptrBuffFactor					= data.m_ptrBuffFactor;
#endif // HAMEL_SECRET_DUNGEON

#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
                m_bAttackedByMyUnit = data.m_bAttackedByMyUnit;
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

			}
			bool Verify() const
			{
				if( m_fTime.Verify() == false
					|| m_DamagePerSec.Verify() == false 
					|| m_Accumulation.Verify() == false
					|| m_fElapsedTimeToEffectDamage.Verify() == false 
					|| m_fRate.Verify() == false 
#ifdef EXTRADAMAGE_RATE_BUG_FIX						
					|| m_fPureRate.Verify() == false
#endif EXTRADAMAGE_RATE_BUG_FIX		
					|| m_Damage.Verify() == false
#ifdef NEW_EXTRA_DAMAGE
					|| m_iLevel.Verify() == false
					|| m_iAddLevel.Verify() == false
					|| m_bIgnoreResist.Verify() == false
#endif
#ifdef SKILL_CASH_10_TEST
					|| m_fDamageRate.Verify() == false
					|| m_fRunJumpRate.Verify() == false
					|| m_fAnimSpeedRate.Verify() == false
#endif
					//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
					/// ExtraDamage를 Map으로 바꾸게 되면 삭제
					|| m_fCriticalRateToDefender.Verify() == false
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
				//}} 2011-04 에 패치될 청 캐시 스킬
#ifdef CHUNG_SECOND_CLASS_CHANGE
					|| m_fJumpRate.Verify() == false
#endif
					)
				{
					return false;
				}
				return true;
			}

#ifdef NEW_EXTRA_DAMAGE
			float GetCalcPerDamage(int iUnitLevel, float fResist) const;
#endif

		};

		struct HitUnit
		{
			float			fRemainGap; //0보다 작아지면 재차 공격가능
			CX2GameUnitoPtr	optrHitGameUnit;

            HitUnit()
                : fRemainGap( 0.f )
                , optrHitGameUnit()
            {
            }
		};
        typedef std::vector<HitUnit>    HitUnitVector;
#ifdef  X2OPTIMIZE_STL_CONTAINER_USAGE
        static inline void swap( HitUnit& lhs_, HitUnit& rhs_ )
        {
            std::swap( lhs_.fRemainGap, rhs_.fRemainGap );
            lhs_.optrHitGameUnit.Swap( rhs_.optrHitGameUnit );
        }
#endif  X2OPTIMIZE_STL_CONTAINER_USAGE

		struct Damage
		{
			float	fPhysic;
			float	fMagic;

			Damage()
			{
				Init();
			}
			void Init()
			{
				fPhysic		= 0.0f;
				fMagic		= 0.0f;
			}
			float GetAllDamage()
			{
				return fPhysic + fMagic;
			}
		};

		struct AttackerGet
		{
			float m_fHP;
			float m_fMP;

			AttackerGet()
			{
				Init();
			}

			void Init()
			{
				m_fHP = 0.f;
				m_fMP = 0.f;
			}
		};

#ifdef ADD_RENA_SYSTEM //김창한
		struct DamageRelateSkillData
		{
			Byte					m_byteSkillIndex;	// 스킬 구별을 위한 index
			Byte					m_byteRelateData;	//최대로 소모하는 NF게이지 값을 저장

			DamageRelateSkillData():m_byteSkillIndex(0), m_byteRelateData(0) {}

			void Init()
			{
				m_byteSkillIndex = 0;
				m_byteRelateData = 0;
			}
		};
#endif //ADD_RENA_SYSTEM

		struct DamageData
		{
			DAMAGE_TYPE						damageType;
			REACTER_TYPE					attackerType;
			ATTACK_TYPE						attackType;
			CX2GameUnitoPtr					optrAttackerGameUnit;	/// 이것도 이거지만.. UID가 필요할 지도..
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect_CEffectHandle   hAttackerEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CX2DamageEffectBasic*			pAttackerEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			//{{ seojt // 2009-1-12, 16:15
			CKTDGXSkinAnim*	                pAttackerAnim;			/// 고민좀...
			//CKTDGXSkinAnimPtr	            pAttackerAnim;
			//}} seojt // 2009-1-12, 16:15
			CKTDXCollision::CollisionData*	pCollisionData;
			

			HIT_TYPE				hitType;
			REACT_TYPE				reActType;
			REACT_TYPE				reActResult;            
			bool					m_bMeleeAttack;		/// 근접공격인가?(기본적으로 유닛의 타격박스는 true, 데미지이펙트는 false 이나, 스크립트의 DAMAGE_DATA에서 이펙트여도 근접으로 변경가능)
			bool					m_bDrain;
			bool					m_bDrainMana;
			bool					m_bAttackAllTeam;
			float					m_fDungeonRate;
						
			int						techPoint;
			Damage					damage;							/// 물공, 마공으로 바꾸는게...

			vector<CX2BuffFactorPtr>	m_vecBuffFactorPtr;			/// 상대방에게 버프/디버프

			ExtraDamageData			m_ExtraDamage;					/// 데미지 스크립트에 있는 extra_damage
			ExtraDamageData			m_BufExtraDamage;				/// 버프용 데미지 
			ExtraDamageData			m_SocketExtraDamage;			/// 아이템 소켓 옵션에 의한 extra_damage, 중복 안되는 것
			vector<ExtraDamageData>	m_MultipleExtraDamage;			/// 아이템 소켓 옵션에 의한 extra_damage, 중복 되는 것


			AttackerGet				attackerGet;
			D3DXVECTOR2				backSpeed;
			float					fForceDownValue;
			float					fGroggyTime;

			float					fHitAddMP;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
			float					fHitAddHPPer;					/// 공격 성공시 공격자에게 회복시킬 체력 배율
#endif // SERV_ARA_CHANGE_CLASS_SECOND

			float					fStopTimeAtt;
			float					fStopTimeDef;

			CKTDGCamera::SHAKE_TYPE	m_eCameraShakeType;
			float					fCameraCrashGap;
			float					fCameraCrashTime;
			int						clearScreen;
			D3DXCOLOR				clearScreenColor;
#ifdef ARRANGED_FLY_DAMAGE_TEST
			//{{ oasis907 : 김상윤 [2010.2.11] // 
			bool					bArrangedFly;
			//}}
#endif ARRANGED_FLY_DAMAGE_TEST
			bool					bForceFly;
			bool					bCanRevenge;
			bool					bCanReflexMagic;
			bool					bReAttack;
			float					fHitGap;
			vector<HitUnit>			hitUnitList;

			D3DXVECTOR3				impactPoint;

			REACTER_TYPE			defenderType;
			CX2GameUnitoPtr			optrDefenderGameUnit;
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CX2DamageEffectBasic*	pDefenderEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

            bool                    m_bGuard;
            bool                    m_bWBackSpeed;

			std::wstring			m_wstrCreateDamageEffectName;
			std::wstring			m_wstrCreateEffectSetName;
			bool					m_bCreateEffectSetTraceUnit;			// 이펙트 생성하는 damage effect 일 때 생성된 effect set이 피격당한 유닛을 trace 할지 말지
#ifdef TRACE_UNIT_DIE_DELETE_EFFECTSET
			bool					m_bTraceUnitDieDeleteEffectSet;			// trace 유닛이 죽으면 해당 이펙트 셋을 삭제할지, true면 삭제
#endif //TRACE_UNIT_DIE_DELETE_EFFECTSET
			bool					m_bPenetrate;
            
            bool                    DamageLineGroup;
			float					DamageDistance;

			bool					m_bFlipDirOnHit;		// 타격을 받은 대상의 방향을 반대로 돌려버림

#ifdef AREA_HIT
			DAMAGE_AREA_TYPE		m_eDamageAreaType;
			int						m_iDamageUnitNum;
#endif
#ifdef SKILL_30_TEST
			bool					m_bNoDamage;
#endif
			bool					m_bHitedManaDamage;

			float					m_fDrainHpRate;		// 타격시 데미지의 얼마를 hp로 환원

			std::vector<std::wstring>	m_vecHitParticleName;

#ifdef NO_DETONATION
			bool					m_bNoDetonation;	// 기폭하지 못하도록 한다.
			bool					m_bDrainHpParticle; // 현재는...블러디액셀용...HP를 뺏어가는 이펙트를 보일 것인지..
#endif
			bool					m_bApplyExtraDamage;

#ifdef DUNGEON_ITEM
			int						m_ResistType;
			bool					m_bAttackOurTeam;
			float					m_fHpPercentUp;
			float					m_fMpPercentUp;
#endif

#ifdef	APPLY_MOTION_OFFSET
			bool					m_bApplyMotionOffset;
#endif	APPLY_MOTION_OFFSET

#ifdef SERV_SKILL_NOTE
			float					m_fCriticalRate;
			float					m_fDecreaseForceDown;
#endif

			bool					m_bNoBuff;

#ifdef DAMAGEDATA_RATE_MODIFIER
			float					m_fRateModifier;
#endif DAMAGEDATA_RATE_MODIFIER

			// 본섭 DamageEffect.lua 롤백하지 않고 반영시키기 위해 주석 해제
			// oasis907 : 김상윤 [2010.11.4] // 그랜드 아처 - 진동 사격(패시브)
			float					m_fHitDamageEffectInvokeRate;
			float					m_fHitDamageEffectDamageRate;

			bool					m_bIgnoreDefence;

#ifdef WIDE_BUFF_ANI_SPEED_UP
			bool	m_bAnimationSpeedUp;
			float	m_fAnimationSpeedUpRate;
#endif
#ifdef SKILL_BALANCE_20110728
			float	m_fPvpRate;
#endif

#ifdef UPGRADE_RAVEN
			DAMAGE_TRIGGER_TYPE		m_eDamageTrigger;	//
			bool					m_bOnlyAttackBasic;
			bool					m_bIgnoreStateDefence;
			bool					m_bNoMiss;
#endif

#ifdef ELSWORD_WAY_OF_SWORD
			WAY_OF_SWORD_TYPE		m_WayofSwordType;
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			DYNAMO_PARTICLE_RATE_TYPE	m_DPRateType;		///	DP 변동 배율
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_RENA_SYSTEM //김창한
			NATURAL_FORCE_TYPE		m_NaturalForceType;
#endif //ADD_RENA_SYSTEM

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			float					m_fDrainMpByReact;
			float					m_fDrainMpRate;
			bool					m_bLvDamage;
#endif

#ifdef EVE_ELECTRA
			int						m_iLaserGroupID;
#endif EVE_ELECTRA

#ifdef ADD_BACKATTACK
			bool					m_bApplyBackAttack;
#endif

#ifdef SERV_ARME_DIMENSION_WITCH
			float					m_fCreateDamageEffectRate;
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER		/// 청 2차 전직 택티컬 트루퍼 - 택티컬 필드
			float					m_fIncreaseMP;				/// 증가 엠피량
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef PLAY_EMOTION_BY_DAMAGE_EFFECT
			bool					m_bIsLaughterDamage;
#endif // PLAY_EMOTION_BY_DAMAGE_EFFECT

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
			int							m_iHitAddForcePower;		/// 피격시 공격자의 기력 회복
			CX2SkillTree::SKILL_ID		m_eUsedSkillID;				/// 해당 데미지 데이터를 생성한 스킬 아이디 ( 직접 설정해 줘야 해요... )
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            bool                    m_bRelaxNPCReactionStateCheck;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef ADD_MEMO_1ST_CLASS //김창한
			float					m_fApplyCriticalDamage;			/// 크리티컬 데미지 배율을 증가시킨다. ex> 기존 크리티컬 배율인 1.5에 해당 값의 배율이 곱해지고 데미지와 계산된다.
			float					fHitAddHPbyAttackPower;			/// 공격 성공시 공격자에게 회복시킬 체력 배율 (공격자 물마공의 평균값에 비례하여 적용)
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_RENA_SYSTEM //김창한
			FIRST_ATTACK_CHECK		m_eFirstAttack;					/// 첫번째 공격을 성공했는가?
			DamageRelateSkillData	m_RelateSkillData;
#endif //ADD_RENA_SYSTEM

			void SimpleInit()
			{
				damageType			= DT_PHYSIC;
				attackerType		= AT_UNIT;
				attackType			= AT_NORMAL;
				optrAttackerGameUnit.Reset();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                hAttackerEffect     = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				//{{ seojt // 2009-1-12, 16:15
				pAttackerAnim		= NULL;
                //pAttackerAnim.reset();
				//}} seojt // 2009-1-12, 16:15
				pCollisionData		= NULL;
				m_bMeleeAttack		= false;
				m_bDrain			= false;
				m_bDrainMana		= false;
				m_bAttackAllTeam	= false;
				m_fDungeonRate		= 1.0f;

				techPoint			= 0;
				hitType				= HT_DEFAULT;
				reActType			= RT_NO_DAMAGE;
				reActResult			= RT_NO_DAMAGE;

				damage.Init();

				m_vecBuffFactorPtr.resize( 0 );
				m_ExtraDamage.Init();
				m_BufExtraDamage.Init();


				backSpeed			= D3DXVECTOR2( 0.0f, 0.0f );
				fForceDownValue		= 0.0f;
				fGroggyTime			= 0.0f;
				
				fHitAddMP			= 6.0f;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				fHitAddHPPer		= 0.0f;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

				fStopTimeAtt		= 0.0f;
				fStopTimeDef		= 0.0f;
				m_eCameraShakeType	= CKTDGCamera::DECT_UP_DOWN;
				fCameraCrashGap		= 0.0f;
				fCameraCrashTime	= 0.0f;
				clearScreen			= 0;
				clearScreenColor	= D3DXCOLOR(0x00ffffff);
#ifdef ARRANGED_FLY_DAMAGE_TEST
				//{{ oasis907 : 김상윤 [2010.2.11] // 
				bArrangedFly		= false;
				//}
#endif ARRANGED_FLY_DAMAGE_TEST
				bForceFly			= false;
				bCanRevenge			= true;
				bCanReflexMagic		= false;
				bReAttack			= false;
				fHitGap				= 0.0f;
				
				hitUnitList.resize(0);
			
                m_bGuard = false;
                m_bWBackSpeed = false;

				m_wstrCreateDamageEffectName = L"";
				m_wstrCreateEffectSetName = L"";
				m_bCreateEffectSetTraceUnit = false;
#ifdef TRACE_UNIT_DIE_DELETE_EFFECTSET
				m_bTraceUnitDieDeleteEffectSet = true;
#endif //TRACE_UNIT_DIE_DELETE_EFFECTSET
				m_bPenetrate		= false;

                DamageLineGroup     = false;
				DamageDistance		= 99999.f;

				m_bFlipDirOnHit		= false;

#ifdef AREA_HIT
				m_eDamageAreaType	= DAT_ALL_UNIT;
				m_iDamageUnitNum	= 0;				
#endif
#ifdef SKILL_30_TEST
				m_bNoDamage			= false;
#endif
				m_bHitedManaDamage	= false;
				m_fDrainHpRate		= 0.f;

				m_vecHitParticleName.clear();

#ifdef NO_DETONATION
				m_bNoDetonation		= false;
#endif
#ifdef SKILL_CASH_10_TEST
				m_bDrainHpParticle	= false;
#endif
				m_bApplyExtraDamage = true;
#ifdef DUNGEON_ITEM
				m_ResistType = 0;
				m_fHpPercentUp = 0.f;
				m_fMpPercentUp = 0.f;
				m_bAttackOurTeam = false;
#endif
#ifdef	APPLY_MOTION_OFFSET
				m_bApplyMotionOffset	= true;
#endif	APPLY_MOTION_OFFSET
				
#ifdef SERV_SKILL_NOTE
				m_fCriticalRate		= 0.f;
				m_fDecreaseForceDown = 0.f;
#endif

				m_bNoBuff			= false;

#ifdef DAMAGEDATA_RATE_MODIFIER
				m_fRateModifier		= 1.f;
#endif DAMAGEDATA_RATE_MODIFIER

				// 본섭 DamageEffect.lua 롤백하지 않고 반영시키기 위해 주석 해제
				// oasis907 : 김상윤 [2010.11.4] // 그랜드 아처 - 진동 사격(패시브)
				m_fHitDamageEffectInvokeRate = 1.f;
				m_fHitDamageEffectDamageRate = 1.f;

				m_bIgnoreDefence	= false;

#ifdef WIDE_BUFF_ANI_SPEED_UP
				m_bAnimationSpeedUp = false;
				m_fAnimationSpeedUpRate = 0.f;
#endif
#ifdef SKILL_BALANCE_20110728
				m_fPvpRate = 1.f;
#endif

#ifdef UPGRADE_RAVEN
				m_eDamageTrigger = DTT_NONE;
				m_bOnlyAttackBasic = false;
				m_bIgnoreStateDefence = false;
				m_bNoMiss = false;
#endif

#ifdef ELSWORD_WAY_OF_SWORD
				m_WayofSwordType	= WST_NONE;
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
				m_DPRateType		= DPRT_NONE;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
				m_fDrainMpByReact = 0.f;
				m_fDrainMpRate = 1.f;
				m_bLvDamage = false;
#endif

#ifdef EVE_ELECTRA
				m_iLaserGroupID			= 0;
#endif EVE_ELECTRA

#ifdef ADD_BACKATTACK
				m_bApplyBackAttack		= true;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
				m_fCreateDamageEffectRate	= 0.f;
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER		/// 청 2차 전직 택티컬 트루퍼 - 택티컬 필드
				m_fIncreaseMP				= 0.f;				/// 증가 엠피량
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef PLAY_EMOTION_BY_DAMAGE_EFFECT
				m_bIsLaughterDamage = false;
#endif // PLAY_EMOTION_BY_DAMAGE_EFFECT

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				m_iHitAddForcePower = 0;
				m_eUsedSkillID		= CX2SkillTree::SI_NONE;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                m_bRelaxNPCReactionStateCheck = false;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef ADD_MEMO_1ST_CLASS //김창한
				m_fApplyCriticalDamage = 0.f;
				fHitAddHPbyAttackPower = 0.f;
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_RENA_SYSTEM //김창한
				m_eFirstAttack = FAC_NONE;
				m_RelateSkillData.Init();
#endif //ADD_RENA_SYSTEM
			}

			void Init()
			{
				SimpleInit();

				impactPoint		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
				defenderType	= AT_UNIT;
				optrDefenderGameUnit.Reset();
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pDefenderEffect	= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				
				m_SocketExtraDamage.Init();
				m_MultipleExtraDamage.resize(0);
			}

			void PushBuffFactor( CX2BuffFactorPtr ptrBuffFactor_, const bool bClearExistedFactor_ = true )
			{
				if ( bClearExistedFactor_ )
				{
					m_vecBuffFactorPtr.clear();
					PushSocketAndTemporaryBuffFactor( this );	/// 소켓옵션, 버프의해 일시적으로 생긴 팩터등을 다시 적용
				}

				if ( NULL != ptrBuffFactor_ )
				{
					CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor_->GetClonePtr();
					if ( NULL != ptrBuffFactorClone )
					{
						ptrBuffFactorClone->SetGameUnitBuffFactor( optrAttackerGameUnit.GetObservable() );
						m_vecBuffFactorPtr.push_back( ptrBuffFactorClone );
					}
				}
			}

			~DamageData()
			{
				hitUnitList.clear();
			}

			DamageData()
			{
				Init();
			}

			DamageData( const DamageData& rhs )
			{					
				*this = rhs;
			}

//{{ robobeg : 2013-09-17
            bool    IsAttackedByMyUnit();
//}} robobeg : 2013-09-17

		};

#ifdef ELSWORD_WAY_OF_SWORD
		struct WayOfSwordConstTable
		{
			float m_fSkillTypeConst_VigorCommand;
			float m_fSkillTypeConst_VigorActive;
			float m_fSkillTypeConst_VigorSpecialActive;
			float m_fSkillTypeConst_DestCommand;
			float m_fSkillTypeConst_DestActive;
			float m_fSkillTypeConst_DestSpecialActive;
			float m_fSkillTypeConst_DamageReact;

			float m_fStateConst_DestSkill_TowardVigorState;
			float m_fStateConst_VigorSkill_TowardVigorState;
			float m_fStateConst_DestSkill_TowrdDestState;
			float m_fStateConst_VigorSkill_TowrdDestState;
			float m_fStateConst_DestSkill_InVigorState;
			float m_fStateConst_VigorSkill_InVigorState;
			float m_fStateConst_DestSkill_InDestState;
			float m_fStateConst_VigorSkill_InDestState;
			float m_fStateConst_DestSkill_CenterState;
			float m_fStateConst_VigorSkill_CenterState;
			
			float m_fStateConst_DamageReact_TowardVigorState;
			float m_fStateConst_DamageReact_TowrdDestState;
			float m_fStateConst_DamageReact_InVigorState;
			float m_fStateConst_DamageReact_InDestState;
			float m_fStateConst_DamageReact_CenterState;


			float m_fGameModeConst_NPC_Hit;
			float m_fGameModeConst_PVP_Hit;

			float m_fConst_VigorStateThreshold;
			float m_fConst_DestStateThreshold;
			float m_fConst_DestRageWSPGain;

			// 엘리 기폭은 던전/대전 구분 되지 않음
			float m_fConst_DestRageWSPGain_Elesis;

			WayOfSwordConstTable()
			{
				m_fSkillTypeConst_VigorCommand				= 0.f;
				m_fSkillTypeConst_VigorActive				= 0.f;
				m_fSkillTypeConst_VigorSpecialActive		= 0.f;
				m_fSkillTypeConst_DestCommand				= 0.f;
				m_fSkillTypeConst_DestActive				= 0.f;
				m_fSkillTypeConst_DestSpecialActive			= 0.f;
				m_fSkillTypeConst_DamageReact				= 0.f;

				m_fStateConst_DestSkill_TowardVigorState	= 0.f;
				m_fStateConst_VigorSkill_TowardVigorState	= 0.f;
				m_fStateConst_DestSkill_TowrdDestState		= 0.f;
				m_fStateConst_VigorSkill_TowrdDestState		= 0.f;
				m_fStateConst_DestSkill_InVigorState		= 0.f;
				m_fStateConst_VigorSkill_InVigorState		= 0.f;
				m_fStateConst_DestSkill_InDestState			= 0.f;
				m_fStateConst_VigorSkill_InDestState		= 0.f;
				m_fStateConst_DestSkill_CenterState			= 0.f;
				m_fStateConst_VigorSkill_CenterState		= 0.f;

				m_fStateConst_DamageReact_TowardVigorState	= 0.f;
				m_fStateConst_DamageReact_TowrdDestState	= 0.f;
				m_fStateConst_DamageReact_InVigorState		= 0.f;
				m_fStateConst_DamageReact_InDestState		= 0.f;
				m_fStateConst_DamageReact_CenterState		= 0.f;

				m_fGameModeConst_NPC_Hit					= 0.f;
				m_fGameModeConst_PVP_Hit					= 0.f;

				m_fConst_VigorStateThreshold				= 0.f;
				m_fConst_DestStateThreshold					= 0.f;
				m_fConst_DestRageWSPGain					= 0.f;

				// 엘리 기폭은 던전/대전 구분 되지 않음
				m_fConst_DestRageWSPGain_Elesis				= 0.f;
			}
		};

#endif ELSWORD_WAY_OF_SWORD


	public:
		CX2DamageManager(void);
		~CX2DamageManager(void);

		static void PushSocketAndTemporaryBuffFactor( DamageData* pDamageData_ );	/// 소켓 옵션, 일시적으로 걸린 버프에 의한 BuffFactor 적용
		bool OpenDamageSoundScript( const WCHAR* wszFileName );
		bool ParseDamageSoundBlock( KLuaManager& luaManager, HIT_TYPE eHitType, const char* pszTableNameUTF8 );
		const WCHAR* GetDamageSound( HIT_TYPE hitType, HITTED_TYPE hittedType );		

		void ParsingBuffFactor( KLuaManager& luaManager_, DamageData* pDamageData_ );

#ifdef ADD_MEMO_1ST_CLASS
		bool SetDamageDataFromLUA( DamageData* pDamageData, KLuaManager& luaManager, const char* pTableName = "DAMAGE_DATA", float fPowerRate = 1.f, const bool IsEqippedMemo_ = false );
#else //ADD_MEMO_1ST_CLASS
		bool SetDamageDataFromLUA( DamageData* pDamageData, KLuaManager& luaManager, const char* pTableName = "DAMAGE_DATA", float fPowerRate = 1.f );
#endif //ADD_MEMO_1ST_CLASS

		bool DamageCheck( DamageData* pDamageData, bool bAttackOnlyThisUnit = false, UidType attackUnitUID = -1 );

		wstring GetExtraDamageName( CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType, bool bTwoLine = false );

#ifdef SKILL_30_TEST
		void CX2DamageManager::SetExtraDamageData( DamageData* pDamageData, bool bApplyEffect,
			CX2DamageManager::EXTRA_DAMAGE_TYPE edtType, float fRate, float fTime, float fDamage, 
			float fExtraDamage = 0.f, int iLevel = -1, bool bIgnoreResist = false,
			float fDamageRate = 0.f, float fRunJumpRate = 0.f, float fAnimSpeedRate = 0.f );
#endif
		
#ifdef ELSWORD_WAY_OF_SWORD
		float	CalcWayOfSwordPoint( WAY_OF_SWORD_TYPE wayOfSwordType_, float fDamageRate_, int iWayOfSwordState );
		float	CalcWayOfSwordPointOnDamageReact( float fDamageRate_, int iWayOfSwordState );
		float	CalcVigorMpUpOnAttack( WAY_OF_SWORD_TYPE wayOfSwordType_, int iWayOfSwordState );
		float	CalcVigorMpUpOnDamageReact( int iWayOfSwordState );
		bool	OpenWayOfSwordScript( const WCHAR* wszFileName );
		float	GetVigorStateThreshold(){	return m_WayOfSwordConstTable.m_fConst_VigorStateThreshold; }
		float	GetDestStateThreshold(){	return m_WayOfSwordConstTable.m_fConst_DestStateThreshold;	}
		float	GetDestRageWSPGain();

		// 엘리시스 검의 길
		float	GetDestRageWSPGain_Elesis();
		float	CalcVigorMpUpOnAttack_Elesis( WAY_OF_SWORD_TYPE wayOfSwordType_, int iWayOfSwordState );
#endif ELSWORD_WAY_OF_SWORD

#ifdef EVE_ELECTRA
		void	AddHitUnit( DamageData& kDamageData, HitUnit _HitUnit ){ kDamageData.hitUnitList.push_back(_HitUnit); }
#endif EVE_ELECTRA
		

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
		bool	GetIsPossibleProvokeExtraDamage ( DamageData * pDamageData );
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	private:
		bool CollisionDataCheckFromUnitToUnit( CX2GameUnit* pAttackerGameUnit_, 
			const CKTDXCollision::CollisionDataListSet& setDefenderCollisionDataList_,
			CKTDXCollision::CollisionType& collisionType_, D3DXVECTOR3* pImpactPos_ );

		//bool CollisionDataCheckFromUnitToEffect( CX2GameUnit* pAttackerGameUnit_, 
		//	const CKTDXCollision::CollisionDataList& listDefenderCollisionData_,
		//	CKTDXCollision::CollisionType& collisionType_, D3DXVECTOR3* pImpactPos_ );

		bool UnitToUnit(DamageData* pDamageData, bool bAttackOnlyThisUnit = false, UidType attackUnitUID = -1 );	//유닛이 유닛 공격
		bool MeshToUnit(DamageData* pDamageData );  //메시로 유닛 공격
		bool CollisionDataToUnit(DamageData* pDamageData );
		bool UnitToEffect( DamageData* pDamageData );
		bool EffectToUnit( DamageData* pDamageData );
		bool EffectToEffect( DamageData* pDamageData );
		
		float CalcHitDodgePercent( CX2GameUnit* pAttacker, CX2GameUnit* pDefender );

#ifdef GROUND_HIT
        bool GroundHit( DamageData* pDamageData, bool bAttackOnlyThisUnit, UidType attackUnitUID );
#endif
#ifdef AREA_HIT
		bool AreaHit( DamageData* pDamageData, bool bAttackOnlyThisUnit, UidType attackUnitUID );
#endif

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE
		/// 그룹 데미지 활성중 검사
		bool CheckActiveGroupDamage( const wstring& wstrDamageEffectName_, CX2DamageManager::DamageData* pDamageData_ );
		/// 그룹 데미지 설정
		void SetGroupDamage( const wstring& wstrDamageEffectName_, CX2DamageManager::DamageData* pDamageData_ );
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
		bool GetPossibleAttackOurTeamID( CX2GameUnit* pNPCUnit_ );	/// 같은 팀을 때릴 수 있는 NPC인지 검사
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER


#ifdef DAMAGE_DATA_BUFF_FACTOR_RELATIVE_SKILL_LEVEL
		bool SetBuffFactorToDamageDataByBuffFactorID( OUT DamageData* pDamageData_, IN const UINT uiBuffFactorId_, IN const UINT uiLevel_ = 1 );

#ifdef ADD_MEMO_1ST_CLASS
		void ParsingBuffFactorID( KLuaManager& luaManager_, DamageData* pDamageData_, const bool IsEqippedMemo_ = false );
#else //ADD_MEMO_1ST_CLASS
		void ParsingBuffFactorID( KLuaManager& luaManager_, DamageData* pDamageData_ );
#endif //ADD_MEMO_1ST_CLASS

#endif // DAMAGE_DATA_BUFF_FACTOR_RELATIVE_SKILL_LEVEL

#ifdef ELSWORD_WAY_OF_SWORD
		WayOfSwordConstTable	m_WayOfSwordConstTable;
#endif ELSWORD_WAY_OF_SWORD

		CKTDXCollision*			m_pCollision;

		std::wstring			m_DamageSoundName[HIT_TYPE_COUNT][HITTED_TYPE_COUNT];
		vector<CKTDXDeviceSound*>	m_pvecReadySound;

};
