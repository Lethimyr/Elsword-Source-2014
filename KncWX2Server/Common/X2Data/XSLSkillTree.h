#pragma once

#include "X2Data/XSLMain.h"
#include <RTTI.h>

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
#include "RefreshSingleton.h"
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
#include <ToString.h>
#include <KNCSingleton.h>
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2



class CXSLSkillTree
{
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
	DeclareRefreshSingleton( CXSLSkillTree );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	DeclareSingleton( CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	DeclToStringW;
	DeclDump;

public:
	enum SKILL_TYPE
	{
		ST_NONE = 0,
		ST_PASSIVE_PHYSIC_ATTACK,
		ST_PASSIVE_MAGIC_ATTACK,
		ST_PASSIVE_MAGIC_DEFENCE,
		ST_PASSIVE_PHYSIC_DEFENCE,
		ST_BUFF,
		ST_ACTIVE,
		ST_SPECIAL_ACTIVE,
		ST_PASSIVE,

		//#ifdef GUILD_SKILL
		ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE, 
		ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE,
		ST_GUILD_PASSIVE,
		//#endif GUILD_SKILL

		//#ifdef ADDED_RELATIONSHIP_SYSTEM
		ST_RELATIONSHIP_SKILL,
		//#endif // ADDED_RELATIONSHIP_SYSTEM
	};

	enum SKILL_ID
	{
		SI_INVALID = -1,
		SI_NONE,
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013
		// 공통 패시브
		SI_P_COMMON_PHYSIC_ATTACK_BEGINNER = 100,		/// 기초 근력단련
		SI_P_COMMON_MAGIC_ATTACK_BEGINNER,				/// 기초 마법수련
		SI_P_COMMON_PHYSIC_ATTACK_INTERMEDIATE,			/// 중급 근력단련
		SI_P_COMMON_MAGIC_ATTACK_INTERMEDIATE,			/// 중급 마법수련
		SI_P_COMMON_GET_CHANCE,							/// 기회의 포착
		SI_P_COMMON_SUPPORT_AURA = 105,					/// 격려의 기운

		// 공통 버프
		SI_SA_COMMON_AURA_POWER_ACCEL = 200,			/// 힘의 문장
		SI_SA_COMMON_AURA_MAGIC_ACCEL,					/// 지혜의 문장
		SI_SA_COMMON_AURA_SHIELD_ACCEL,					/// 인내의 문장
		SI_SA_COMMON_AURA_SPEED_ACCEL,					/// 가속의 문장
		SI_SA_COMMON_AURA_EL_DEFENCE_ACCEL,				///	가호의 문장
		SI_SA_COMMON_AURA_CRITICAL_ACCEL = 205,			/// 승리의 문장
		SI_SA_COMMON_AURA_ADDATK_ACCEL,					/// 섬멸의 문장

		// 길드, 결혼
		SI_GP_COMMON_BALANCED_BODY					= 500,		/// 균형잡힌 육체
		SI_GP_COMMON_HARMONIC_SPIRIT				= 501,		/// 조화로운 영혼
		SI_GP_COMMON_CHEERING_OF_TEAM				= 502,		/// 동료의 응원
		SI_GP_COMMON_MASTER_OF_ALCHEMIST			= 503,		/// 연금술의 달인
		SI_GP_COMMON_CONCENTRATION_OF_SPIRIT		= 504,		/// 정신 집중
		SI_GP_COMMON_CONCENTRATION_MAGICAL_POWER	= 505,		/// 집중된 마력
		SI_GP_COMMON_VIOLENT						= 506,		/// 격노
		SI_GP_COMMON_LIBERATION_OF_ANGER			= 507,		/// 분노 해방
		// 여기까지 2트리
		SI_GP_COMMON_INFINITE_STRENGTH				= 508,		/// 끝없는 체력
		SI_GP_COMMON_CHANCE_TO_REVERSE				= 509,		/// 역전의 기회
		SI_GP_COMMON_SACRIFICE_OF_HERO				= 510,		/// 영웅의 희생
		SI_GP_COMMON_MIDAS							= 511,		/// 마이더스의 손

		SI_ETC_WS_COMMON_LOVE						= 900,		/// 커플 스킬 천상지애

		// Elsword
		SI_SA_ES_FATAL_FURY = 1000,						/// 페이탈 퓨리
		SI_SA_ES_FLAME_GEYSER,							/// 플레임 게이져
		SI_SA_ES_UNLIMITED_BLADE,						/// 언리미티드 블레이드
		SI_SA_ES_TRIPLE_GEYSER,							/// 트리플 게이져
		SI_SA_ESK_SPIRAL_BLAST,							/// 스파이럴 블래스트
		SI_SA_ESK_ARMAGEDON_BLADE = 1005,				/// 아마겟돈 블레이드
		SI_SA_ESK_DOUBLE_SLASH,							/// 더블 슬래시
		SI_SA_ELK_WINDMILL,								/// 윈드밀
		SI_SA_ELK_SAND_STORM,							/// 샌드 스톰
		SI_SA_EMK_RISING_SLASH,							/// 라이징 슬래시
		SI_SA_EMK_RISING_WAVE = 1010,					/// 라이징 웨이브
		SI_SA_EMK_SWORD_FIRE,							/// 소드 파이어
		SI_SA_EMK_PHOENIX_TALON,						/// 피닉스 탤런
		SI_SA_ERS_STORM_BLADE,							/// 스톰 블레이드
		SI_SA_ERS_LUNA_BLADE,							/// 루나 블레이드
		SI_SA_ETK_FINAL_STRIKE = 1015,					/// 파이널 스트라이크
		SI_SA_ETK_PHANTOM_SWORD,						/// 팬텀 소드
		SI_SA_EIS_MAELSTORM_RAGE,						/// 메일스톰 레이지
		SI_SA_EIS_SWORD_FALL,							/// 소드폴
		SI_SA_EIS_RAGE_CUTTER,							/// 레이지 커터

		SI_A_ES_MEGASLASH = 1200,						/// 메가 슬래시
		SI_A_ES_ASSAULT_SLASH,							/// 어설트 슬래시
		SI_A_ES_COUNTER,								/// 반격
		SI_A_ES_ENDURANCE,								/// 극기
		SI_A_ES_KICK,									/// 발차기
		SI_A_ES_ROLLING = 1205,							/// 구르기
		SI_A_ES_SWORD_WAVE,								/// 소드 웨이브
		SI_A_ES_DEFENCE,								/// 막기
		SI_A_ESK_AIR_SLASH,								/// 에어 슬래시
		SI_A_ESK_ARMOR_BREAK,							/// 아머 브레이크
		SI_A_ESK_WEAPON_BREAK = 1210,					/// 웨폰 브레이크
		SI_A_ELK_SONIC_BLADE,							/// 소닉 블레이드
		SI_A_ELK_ROLLING_SMASH,							/// 롤링 스매쉬
		SI_A_ELK_IMPACT_SMASH,							/// 임팩트 스매시
		SI_A_EMK_BIG_BURST,								/// 대폭살
		SI_A_EMK_WIND_BLADE = 1215,						/// 윈드 블레이드
		SI_A_ERS_SPLASH_EXPLOSION,						/// 스플래쉬 익스플로젼
		SI_A_ERS_SWORD_ENCHANT,							/// 소드 인챈트
		SI_A_ERS_CRITICAL_SWORD,						///	크리티컬 소드
		SI_A_ERS_RUNE_OF_FIRE,							/// 초열의 룬
		SI_A_ERS_RUNE_OF_ICE = 1220,					/// 한빙의 룬
		SI_A_ETK_CRESCENT_CUT,							/// 크레센트 컷
		SI_A_ETK_SWORD_SHIELD,							/// 소드 실드
		SI_A_ETK_HARSH_CHASER,							/// 하쉬 체이서
		SI_A_ETK_PIERCING_SWORD,						/// 피어싱 소드
		SI_A_EIS_SWORD_BLASTING = 1225,					/// 소드 블래스팅
		SI_A_EIS_MIRAGE_STING,							/// 미라지 스팅
		SI_A_EIS_FATAL_SLAP,							/// 페이탈 슬랩

		SI_P_ES_POWERFUL_VITAL = 1400,					/// 강인한 신체
		SI_P_ESK_GET_MANA_UP,							/// 활력
		SI_P_ESK_AUTO_GUARD,							/// 오토 가드
		SI_P_ELK_STRONG_BONE,							/// 근골 체질
		SI_P_ELK_REDUCE_PAIN,							/// 고통억제
		SI_P_ELK_INTENSIVE_ATTACK = 1405,				/// 강렬한 일격
		SI_P_ELK_INDURANCE_OF_REVENGE,					/// 역전의 인내
		SI_P_ES_POWERFUL_FIREBALL,						/// 파이어볼 강화
		SI_P_ERS_SPELL_CHAIN,							/// 마력의 사슬
		SI_P_ERS_MAGIC_RESISTANCE,						/// 마법저항 수련
		SI_P_ERS_RUNE_MASTERY = 1410,					/// 룬 마스터리
		SI_P_ETK_BRUTAL_SLAYER,							/// 잔혹한 학살자
		SI_P_ETK_HARSH_SLAYER,							/// 냉혹한 학살자
		SI_P_EIS_COMPACT_COUNTER,						/// 빈틈 없는 반격
		SI_P_EIS_MIND_OF_FIGHTER,						/// 승부사의 기질
		SI_P_EIS_LIGHTNING_STEP = 1415,					/// 전광 석화
		SI_P_EIS_CUTTING_SWORD,							/// 날카로운 검

		SI_HA_ELK_GIGANTIC_SLASH = 1600,				/// 기간틱 슬래시
		SI_HA_ERS_SHINING_RUNE_BUSTER,					/// 샤이닝 룬버스터
		SI_HA_EIS_BLADE_RAIN,							/// 블레이드 레인

		// Aisha
		SI_SA_AV_GUST_SCREW = 2000,						/// 거스트 스크류
		SI_SA_AV_LIGHTNING_BOLT,						/// 라이트닝 볼트
		SI_SA_AV_GUST_STORM,							/// 거스트 스톰
		SI_SA_AV_ICE_STORM,								/// 아이스 스톰
		SI_SA_AHM_MAGIC_MISSILE,						/// 매직 미사일
		SI_SA_AHM_BLIZZARDSHOWER = 2005,				/// 블리자드 샤워
		SI_SA_AHM_CHAIN_LIGHTNING,						/// 체인 라이트닝
		SI_SA_AHM_BLAZE_STEP,							/// 블레이즈 스탭
		SI_SA_AEM_METEOR_SHOWER,						/// 메테오 샤워
		SI_SA_AEM_LIGHTNING_SHOWER,						/// 라이트닝 샤워
		SI_SA_ADM_DARK_CLOUD = 2010,					/// 다크 클라우드
		SI_SA_ADM_PLASMA_CUTTER,						/// 플라즈마 커터
		SI_SA_ADM_DEATHFIELD,							/// 데스필드
		SI_SA_ADM_AGING,								/// 에이징
		SI_SA_AVP_PHANTOM_BREATHING_DARK_FALL,			/// 팬텀 브리딩 - 다크폴
		SI_SA_AVP_PHANTOM_BREATHING_DARK_HOLE = 2015,	/// 팬텀 브리딩 - 다크 홀
		SI_SA_ABM_ENERGY_SPURT,							/// 에너지 스퍼트
		SI_SA_ABM_SUPER_NOVA,							/// 슈퍼 노바
		SI_SA_ABM_GUILLOTINE_PRESS,						/// 길로틴 프레스
		SI_SA_ABM_MAGICAL_MAKEUP,						/// 메지컬 메이크업
		SI_SA_ADW_SCREWDRIVER_TORNADO = 2020,			/// 스크류 드라이버 - 토네이도
		SI_SA_ADW_SCREWDRIVER_DRILLER,					/// 스크류 드라이버 - 드릴러

		SI_A_AV_CHAIN_FIRE_BALL = 2200,					/// 체인 파이어볼
		SI_A_AV_BINDING_CIRCLE,							/// 바인딩 서클
		SI_A_AV_TELEPORT,								/// 텔레포트
		SI_A_AV_MANA_SHIELD,							/// 마나 실드
		SI_A_AV_CIRCLE_FLAME,							/// 서클 플래임
		SI_A_AV_FIRE_ROAD = 2205,						/// 파이어 로드
		SI_A_AV_ICICLE_WAVE,							/// 아이시클 웨이브
		SI_A_AHM_CYCLONE,								/// 싸이클론
		SI_A_AHM_MEDITATION,							/// 명상
		SI_A_AEM_CHAIN_BURST,							/// 체인 버스트
		SI_A_AEM_STONE_WALL = 2210,						/// 돌벽
		SI_A_AEM_AQUA_SHOWER,							/// 아쿠아 샤워
		SI_A_AEM_SHINING_BODY,							/// 샤이닝 바디
		SI_A_ADM_HELL_STONE,							/// 헬스톤
		SI_A_ADM_HELL_DROP,								/// 헬 드롭
		SI_A_ADM_MANA_INTAKE = 2215,					/// 마나 흡수
		SI_A_AVP_SUMMON_BAT_TIME_BOMB,					/// 박쥐 소환 - 타임 밤
		SI_A_AVP_SUMMON_BAT_HEAVY_DOLL,					/// 쁘띠 앙고르 - 헤비 돌
		SI_A_AVP_SHADOW_BODY,							/// 쉐도우 바디
		SI_A_ABM_VITAL_DRAIN,							/// 바이탈 드레인
		SI_A_ABM_ENERGY_DRAIN = 2220,					/// 에너지 드레인
		SI_A_ADW_DISTORTION,							/// 공간 왜곡
		SI_A_ADW_ENERGETIC_BODY,						/// 에너제틱 바디
		SI_A_ADW_HEAVY_PRESS,							/// 헤비 프레스
		SI_A_ADW_IMPACT_HAMMER,							/// 임팩트 해머
		SI_A_ADW_WORM_HOLE = 2225,						/// 웜홀

		SI_P_AV_FIREBALL_UP = 2400,						/// 파이어볼 연마
		SI_P_AHM_FREEZING_POINT_RESEARCH,				/// 빙점 연구
		SI_P_AHM_UNLIMIT_MANA,							/// 끝없는 마력
		SI_P_AEM_ELEMENTAL_FRIENDSHIP,					/// 원소와의 친화
		SI_P_AEM_ENERGY_OF_THE_PLENTY,					/// 약화의 기운
		SI_P_AEM_ELEMENTAL_RESEARCH = 2405,				/// 원소학 연구
		SI_P_ADM_SHADOW_DATH,							/// 죽음의 그림자
		SI_P_ADM_SHADOW_OF_DISASTER,					/// 재앙의 그림자
		SI_P_AVP_LIMITED_MANA_MANAGEMENT,				/// 극한의 마나 운용
		SI_P_AVP_STRONG_MIND,							/// 어둠의 계약
		SI_P_AVP_SPRINTER = 2410,						/// 스프린터
		SI_P_AVP_MAGIC_AMPLIFICATION,					/// 마력 증폭
		SI_P_AVP_GRAVITY_BALL_RESEARCH,					/// 그라비티 볼 연구
		SI_P_ABM_FITNESS,								/// 신체 단련
		SI_P_ABM_MANAFLOW_OVERDRIVE,					/// 기혈 폭주
		SI_P_ADW_SPIRIT_ACCELERATION  = 2415,			/// 기력 가속
		SI_P_ADW_ADVANCED_TELEPORTATION,				/// 공간 도약
		SI_P_ADW_MAGICAL_STAFF,							/// 마력의 지팡이

		SI_HA_AEM_ELEMENTAL_STORM = 2600,				/// 엘리멘탈 스톰
		SI_HA_AVP_ABYSS_ANGOR,							/// 어비스 앙고르
		SI_HA_ADW_FATE_SPACE,							/// 페이트 스페이스

		// Lena
		SI_SA_LE_PERFECT_STORM = 3000,					/// 퍼팩트 스톰
		SI_SA_LE_MULTIPLE_STINGER,						/// 멀티플 스팅거
		SI_SA_LE_AERO_TORNADO,							/// 에어로 토네이도
		SI_SA_LE_PHOENIX_STRIKE,						/// 피닉스 스트라이크
		SI_SA_LCR_CRESCENT_KICK,						/// 크레센트 킥
		SI_SA_LCR_DIVE_KICK_BOMBING = 3005,				/// 다이브 킥 봄잉
		SI_SA_LCR_VIOLENT_ATTACK,						/// 바이올런트 어택
		SI_SA_LWS_SHARPFALL,							/// 샤프 폴
		SI_SA_LWS_AIRELINNA_NYMPH,						/// 아이레린나- 님프
		SI_SA_LWS_AIRELINNA_SYLPH,						/// 아이레린나 - 실프
		SI_SA_LSR_GUIDED_ARROW = 3010,					/// 가이드 애로우
		SI_SA_LSR_CRAZY_SHOT,							/// 크레이지 샷
		SI_SA_LSR_WINDWARD,								/// 윈드와드
		SI_SA_LSR_GUNGNIR,								/// 궁그닐
		SI_SA_LGA_AERO_STRAFE,							/// 에어로 스트레이프
		SI_SA_LTR_FATALITY = 3015,						///	페이탈리티
		SI_SA_LTR_TRAPPING_ARROW_BURST,					///	트래핑 애로우 - 버스트
		SI_SA_LTR_TRAPPING_ARROW_FUNGUS,				/// 트래핑 애로우 - 펑거스
		SI_SA_LTR_CALL_OF_RUIN,							///	콜 오브 루인
		SI_SA_LTR_KARMA,								///	카르마
		SI_SA_LNW_GLIDING_STRIKE = 3020,				///	글라이딩 스트라이크

		SI_A_LE_RAIL_STINGER = 3200,					/// 레일 스팅거
		SI_A_LE_ASSAULT_KICK,							/// 어설트 킥
		SI_A_LE_BACKJUMP_SHOT,							/// 백덤블링 샷
		SI_A_LE_REFLEX_MAGIC,							/// 마법반사
		SI_A_LE_RISING_FALCON,							///	라이징 팔콘
		SI_A_LE_SIEGEMODE = 3205,						/// 시즈 모드
		SI_A_LCR_SPINNING_KICK,							/// 스피닝 킥
		SI_A_LCR_LOW_KICK,								/// 로우킥
		SI_A_LCR_MIDDLE_KICK,							/// 미들킥
		SI_A_LWS_NATURE_FORCE,							/// 자연의 힘
		SI_A_LWS_SLIDE_KICK = 3210,						/// 슬라이드 더블 킥
		SI_A_LWS_ASSAULT_IMPACT,						/// 어설트 임팩트
		SI_A_LWS_BACK_KICK,								/// 백킥
		SI_A_LWS_HIGH_KICK,								/// 하이킥
		SI_A_LSR_ENTANGLE,								/// 인탱글 - 스타이핑 레인저
		SI_A_LSR_SHOOTING_MAGNUM = 3215,				/// 슈팅 매그넘
		SI_A_LSR_HUMMING_WIND,							/// 허밍 윈드
		SI_A_LGA_FREEZING_ARROW,						/// 프리징 애로우
		SI_A_LGA_FREEZING_BOLT,							/// 프리징 볼트
		SI_A_LGA_ARC_SHOT,								/// 아크 샷
		SI_A_LGA_RAPID_SHOT = 3220,						/// 래피드 샷
		SI_A_LGA_WIND_BLAST,							/// 윈드 블래스트
		SI_A_LGA_STIGMA_ARROW,							/// 낙인의 사격
		SI_A_LTR_EVOKE,									///	이보크
		SI_A_LTR_ENTANGLE,								/// 인탱글 - 트래핑
		SI_A_LTR_EXPLOSION_TRAP = 3225,					///	폭발의 덫
		SI_A_LNW_FURIOUS_ENGAGE,						///	퓨리어스 인게이지
		SI_A_LNW_DELAYED_FIRING,						///	지연의 신호탄
		SI_A_RNW_THORNS_TRAP,							/// 가시덩굴 덫
		SI_A_RNW_ROSEBUSH_TRAP,							/// 찔레 덫
		SI_A_RNW_SEED_OF_RUIN = 3230,					/// 진노의 씨앗
		SI_A_RNW_ANGER_OF_ELF,							/// 엘드랏실의 분노

		SI_P_LCR_SHARP_KICK = 3400,						/// 날렵한 발차기
		SI_P_LCR_AGILE_MOVEMENT,						/// 재빠른 몸놀림
		SI_P_LCR_SOFTBODY,								/// 유연한몸놀림
		SI_P_LWS_POWERFUL_SHOT,							/// 힘 있는 사격
		SI_P_LWS_FRIENDSHIP_OF_NATURE,					/// 자연과의 친화
		SI_P_LSR_POWERFUL_BOWSTRING = 3405,				/// 강화된 보우스트링
		SI_P_LSR_HAWK_EYE,								/// 호크아이
		SI_P_LSR_SHARP_ARROW,							/// 다듬어진 화살
		SI_P_LGA_COMMUNE_OF_NATURE,						/// 자연의 감응
		SI_P_LGA_VIBRATION_SHOOTING,					/// 진동 사격
		SI_P_LTR_HUNTERS_ABILITY = 3410,				///	사냥꾼의 재능
		SI_P_LTR_SHARPEN_ARROW,							///	날카로운 화살
		SI_P_LNW_VITALPOINT_PIERCING,					///	급소 찌르기
		SI_P_LNW_PRIOR_PLANNED_BLOW,					///	계산된 일격

		SI_HA_LWS_SPIRAL_STRIKE = 3600,					/// 스파이럴 스트라이크
		SI_HA_LGA_CRYOTRON_BOLT,						/// 크리아오트론 볼트
		SI_HA_LNW_INNOCENT,								/// 이노센트

		// Raven
		SI_SA_RF_POWER_ASSAULT = 4000,					/// 파워 어설트
		SI_SA_RF_CANNON_BLADE,							/// 캐논 블레이드
		SI_SA_RF_SEVEN_BURST,							/// 세븐 버스트
		SI_SA_RST_HYPER_SONIC_STAB,						/// 하이퍼 소닉스탭
		SI_SA_RST_SHOCKWAVE,							/// 쇼크 웨이브 - 강
		SI_SA_RST_SHOCKWAVE_FRONT = 4005,				/// 쇼크 웨이브 - 속
		SI_SA_RST_BLOODY_ACCEL,							/// 블러디 액셀
		SI_SA_RBM_FLYING_IMPACT,						/// 플라잉 임팩트
		SI_SA_RBM_GIGA_DRIVE_LIMITER,					/// 기가 드라이브- 리미터
		SI_SA_RBM_GIGA_DRIVE_SEISMIC_TREMOR,			/// 기가 드라이브 - 사이즈믹 트레몰
		SI_SA_ROT_ARC_ENEMY = 4010,						/// 아크 애너미
		SI_SA_ROT_GUARDIAN_STRIKE,						/// 가디언 스트라이크
		SI_SA_ROT_VALKYRIESJAVELIN,						/// 발키리스 자벨린
		SI_SA_ROT_NUCLEAR,								/// 뉴클리어
		SI_SA_RRF_WILD_CHARGE,							/// 와일드 차지
		SI_SA_RWT_BURSTING_BLADE = 4015,				/// 버스팅 블레이드
		SI_SA_RWT_REVOLVER_CANNON_ORDNANCE_PENETRATOR,	/// 리볼버 캐논 - OP탄
		SI_SA_RWT_REVOLVER_CANNON_HIGH_EXPLOSIVE_SHELL,	/// 리볼버 캐논 - HE탄
		SI_SA_RWT_HARPOON_SPEAR,						/// 하푼 스피어
		SI_SA_RWT_HELLFIRE_GATLING,						/// 헬파이어 개틀링
		SI_SA_RWT_GIGA_PROMINENCE = 4020,				/// 기가 프로미넌스
		SI_SA_RVC_DEADLY_RAID,							/// 데들리 레이드
		SI_SA_RVC_IGNITION_CROW_NAPALM,					/// 이그니션 크로우 - 네이팜
		SI_SA_RVC_IGNITION_CROW_INCINERATION,			/// 이그니션 크로우 - 인시너레이션

		SI_A_RF_MAXIMUM_CANNON = 4200,					/// 맥시멈 캐논
		SI_A_RF_EARTH_BREAKER,							/// 어스 브레이크
		SI_A_RF_MEGADRILL_BREAK,						/// 메가 드릴 브레이크
		SI_A_RF_BURNING_RUSH,							/// 버닝러시
		SI_A_RF_GROUND_IMPACT,							/// 그라운드 임팩트
		SI_A_RF_SHADOW_STEP = 4205,						/// 섀도우스탭
		SI_A_RF_BREAKING_FIST,							/// 브레이킹 피스트
		SI_A_RST_BERSERKER_BLADE,						/// 버서커 블레이드
		SI_A_RST_CUT_TENDON,							/// 컷텐던
		SI_A_RBM_ONE_FLASH,								/// 일섬
		SI_A_RBM_SONIC_SLASH = 4210,					/// 소닉 슬래시
		SI_A_RBM_WOFL_FANG,								/// 울프팽
		SI_A_ROT_CHARGED_BOLT_HEDGHOG,					/// 차지드 볼트 - 헤지호그
		SI_A_ROT_CHARGED_BOLT_BLOOD,					/// 차지드 볼트 - 쏜즈 블러드
		SI_A_RRF_HELL_DIVE,								/// 헬 다이브
		SI_A_ROT_WEAPON_BREAK = 4215,					/// 무기 깨기
		SI_A_ROT_ARMOR_BREAK,							/// 방어구 깨기
		SI_A_RRF_X_CRASH,								/// 엑스 크래시
		SI_A_RRF_LIMIT_CRUSHER,							/// 리미트 크러셔
		SI_A_RWT_BARRAGE_ATTACK,						/// 연무 폭파
		SI_A_RWT_FLAME_SWORD = 4220,					/// 화염인
		SI_A_RVC_BLEEDING_SLICER,						/// 치명상
		SI_A_RVC_NAPALM_GRENADE,						/// 네이팜 그레네이드

		SI_P_RF_SHADOW_PIERCING = 4400,					/// 섀도우피어싱
		SI_P_RF_HARDEN_BODY,							/// 단련된신체
		SI_P_RF_INDUCE_DESIRE_WINNING,					/// 호승심유발
		SI_P_RST_BLADE_MASTERY,							/// 숙련된 검술
		SI_P_RST_EXQUISITE_SWORDMANSHIP,				/// 섬세한 검술
		SI_P_RBM_REVERSAL_SOLDIER = 4405,				/// 역전의용사
		SI_P_RBM_SHADOW_THRUST,							/// 쉐도우 쓰러스트
		SI_P_RBM_EMERGENCY_ESCAPE,						/// 긴급 탈출
		SI_P_RRF_BURNING_NASOD_HAND,					/// 타오르는 나소드 핸드
		SI_P_RRF_SPIRITUALIZED_FURY,					/// 승화된 분노
		SI_P_RRF_EXPLOSIVE_NASOD_HAND = 4410,			/// 폭발하는 나소드 핸드
		SI_P_RRF_SHADOW_PUNISHER,						/// 쉐도우 퍼니셔
		SI_P_RRF_CONFIDENCE_VICTORY,					/// 승리의자신감		
		SI_P_RWT_BEHAVIOR_OF_MERCENARY,					/// 용병의 처신술
		SI_P_RVC_SHADOW_BACK_SLIDE,						/// 섀도우 백 슬라이드
		SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY = 4415,/// 용병의 생존 전략
		SI_P_RVC_WINNABLE_TECHNIQUE_OF_MERCENARY,		/// 용병의 승리전략
		SI_P_RVC_AIR_SLICER,							/// 에어 슬라이서
		SI_P_RVC_OVERHEAT_MODE,							/// 과열모드 작동!

		SI_HA_RBM_EXTREM_BLADE = 4600,					/// 익스트림 블레이드
		SI_HA_RRF_INFERNAL_ARM,							/// 인페르날 암즈
		SI_HA_RVC_BURNING_BUSTER,						/// 버닝 버스터

		// Eve
		SI_SA_EN_ILLUSION_STINGER = 5000,				/// 일루전 스팅어
		SI_SA_EN_DIMENSION_LINK_BLADER,					/// 디멘션 링크 - 블레이더
		SI_SA_EN_GENERATE_BLACKHOLE,					/// 제네레이터 블랙홀
		SI_SA_EN_DIMENSION_LINK_GUARDIAN,				/// 디멘션 링크 - 가디언
		SI_SA_EEG_EXPLOSION_IMPACT,						/// 익스플로전 임팩트
		SI_SA_EEG_QUEENS_THRONE = 5005,					/// 퀸스 쓰론
		SI_SA_EEG_JUNK_BREAK,							/// 정크 브레이크
		SI_SA_ENS_IRONSCRAPS,							/// 아이언 스크랩스
		SI_SA_ENS_ATOMIC_SHIELD,						/// 아토믹 쉴드
		SI_SA_ENS_ASSUALT_SPEAR_ANNIHILATOR,			/// 어설트 스피어 - 어나이얼레이터
		SI_SA_EAT_SONIC_WAVE = 5010,					/// 소닉 웨이브
		SI_SA_EAT_GENOCIDE_RIPPER,						/// 제노사이드 리퍼
		SI_SA_EAT_HEAVENS_FIST_PRESSURE,				/// 해븐즈 피스트 - 프레셔
		SI_SA_EAT_HEAVENS_FIST_SWEEPER,					/// 헤븐즈 피스트 - 스위퍼
		SI_SA_EEP_SPACE_WRENCH,							/// 스페이 스랜처
		SI_SA_EEP_ASSUALT_SPEAR_BURST = 5015,			/// 어설트 스피어 - 버스터
		SI_SA_EEL_PARTICLE_RAY,							/// 파티클 레이
		SI_SA_EEL_SWEEP_ROLLING_OVER_CHARGE,			/// 스위프 롤링 - 오버차지
		SI_SA_EEL_SWEEP_ROLLING_TRI_VULCAN,				/// 스위프 롤링 - 트라이 발칸
		SI_SA_EEL_PHOTON_FLARE,							/// 포톤 플레어
		SI_SA_EEL_GIGA_STREAM = 5020,					/// 기가 스트림
		SI_SA_EEL_THOUSANDS_OF_STARS,					/// 사우전드 스타
		SI_SA_EBS_ENERGY_NEEDLES,						/// 에너지 니들스
		SI_SA_EBS_ENERGETIC_HEART,						/// 에너제틱 하트
		SI_SA_EEL_DREAD_SHOCK,							/// 드레드 쇼크

		SI_A_EN_ILLUSION_STRIKE = 5200,					/// 일루전 스트라이크
		SI_A_EN_DIMENSION_LINK,							/// 디멘션 링크 - 센트리
		SI_A_EN_CLOAKING,								/// 클로킹
		SI_A_EN_CONVERSION,								/// 마력 전환
		SI_A_EN_MEGA_ELECTRONBALL,						/// 메가 일렉트론볼
		SI_A_EN_ILLUSION_RAY = 5205,					/// 일루전 레이
		SI_A_EN_TESLA_SHOCK,							/// 테슬라 쇼크
		SI_A_EN_PHOTON_BLINK,							/// 광자 점멸
		SI_A_EEG_HORNET_STING_EXPLOSION,				/// 호넷 스팅 - 익스프로전
		SI_A_EEG_HORNET_STING_SHAPED_CHARGE,			/// 호넷 스팅 - 쉐이프트 차지
		SI_A_EEG_METAL_DUST_AURA = 5210,				/// 메탈 더스트 아우라
		SI_A_ENS_ATOMIC_BLASTER_PLASMAGUN,				/// 아토믹 블래스터 - 플라즈마 건
		SI_A_ENS_ATOMIC_BLASTER_SONICGUN,				/// 아토믹 블래스터 - 소닉 건
		SI_A_ENS_SPEAR_BURST,							/// 스피어 버스트
		SI_A_EAT_SURFACE_CUTING,						/// 서피스 커팅
		SI_A_EAT_OBERON_GUARD = 5215,					/// 오베론 가드
		SI_A_EEP_SPIT_FIRE_FLUSH,						/// 스핏 파이어 - 플러시
		SI_A_EEP_SPIT_FIRE_GRENADE,						/// 스핏 파이어 - 그리네이드
		SI_SA_EEP_ELECTRONIC_FIELD,						/// 일렉트로닉 필드
		SI_A_EEL_SPECTRO_EL_CRYSTAL,					/// 엘 분광 결정
		SI_A_EBS_TASER_PILUM = 5220,					/// 테이저 필라 (복수형 발음)
		SI_A_EBS_KUGELBLITZ,							/// 쿠글블릿츠

		SI_P_EN_ELECTRONBALL_UP = 5400,					/// 일렉트론볼 연마
		SI_P_EN_QUEENS_CONTROL,							/// 여왕의 지배력
		SI_P_EN_QUEENS_POTENTIAL,						/// 여왕의 잠재력
		SI_P_EN_REFINED_STEP,							/// 우아한발걸음
		SI_P_EN_PHOTON_BOOSTER,							/// 광자 추진기
		SI_P_ENS_NASOD_WEAPON = 5405,					/// 나소드 무기 강화
		SI_P_ENS_ENERGY_OF_CONCENTRATION,				/// 집중의 기운
		SI_P_EEP_ELECTRONBALL_MASTER,					/// 일렉트론 볼마스터
		SI_P_EEP_CHARGING_BOOSTER,						/// 충전추진기
		SI_P_EEL_SPECTRUM_DYNAMICS,						/// 분광 역학
		SI_P_EEL_ADVANCED_EL_ENERGY_REACTOR = 5410,		/// 강화 엘 에너지 반응로
		SI_P_EBS_HYPER_OPTICAL_RESEARCH,				/// 초광학 연구
		SI_P_EBS_HIGHPOWERED_ELECTRICS,					/// 고출력 전자회로
		SI_P_EBS_AMPLIFIED_EL_ENERGY_REACTOR,			/// 증폭 엘 에너지 반응로

		SI_HA_ENS_LUNATIC_SCUD = 5600,					/// 루나틱 스커드
		SI_HA_EEP_LINK_OVERCHARGE_ILLUSION,				/// 링크 오버차지 일루전
		SI_HA_EBS_PSYCHIC_ARTILLERRY,					/// 사이킥 아틸러리( 이레이저 캐논 )

		// Chung
		SI_SA_CC_LUNATIC_BLOW = 6000,					/// 루나틱 블로우
		SI_SA_CC_SCARE_CHASE,							/// 스케어 체이스
		SI_SA_CC_GIGANTIC_IMPACT,						/// 기간틱 임팩트
		SI_SA_CC_ACELDAMA,								/// 아겔다마
		SI_SA_CFG_LUNATIC_FURY,							/// 루나틱 퓨리
		SI_SA_CFG_PANDEMONIUM_CHAOS = 6005,				/// 판데모니움 - 카오스
		SI_SA_CFG_PANDEMONIUM_FEAR,						/// 판데모니움 - 피어
		SI_SA_CFG_PAIN_OF_CALADBOLG,					/// 칼라볼그 페인
		SI_SA_CIP_IRON_HOWLING,							/// 아이언 하울링
		SI_SA_CIP_LAND_DEMOLISHIER_HEAVY_ARMS,			/// 랜드 디몰리셔 - 헤비 암즈
		SI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE = 6010,	/// 랜드 디몰리셔 - 어스 퀘이크
		SI_SA_CSG_ARTILLERY_STRIKE_QUANTUM_BALLISTA,	/// 아틸러리 스트라이크 - 퀀텀 발리스타
		SI_SA_CSG_HEAVY_RAILGUN,						/// 헤비레일건
		SI_SA_CSG_SHOOTING_STAR,						/// 슈팅 스타
		SI_SA_CDC_COMET_CRASHER,						/// 카밋 크래셔
		SI_SA_CDC_SHARPSHOOTER_SYNDROME = 6015,			/// 샤프슈터 신트롬
		SI_SA_CHG_BIGBANG_STREAM,						/// 빅뱅 스트림
		SI_SA_CHG_CHAOS_CANNON,							/// 카오스 캐논
		SI_SA_CHG_CARPET_BOMBING,						/// 카펫 바밍
		SI_SA_CTT_ARTILLERY_STRIKE,						/// 아틸러리 스트라이크 - 미슬 샤워
		SI_SA_CTT_DREAD_CHASE = 6020,					/// 드레드 체이스
		SI_SA_CTT_TACTICAL_FIELD_INDURANCE,				/// 택티컬 필드 - 인듀런스
		SI_SA_CTT_TACTICAL_FIELD_RAID,					/// 택티컬 필드 - 레이드

		SI_A_CC_BRUTAL_SWING = 6200,					/// 브루탈 스윙
		SI_A_CC_DETONATION,								/// 디토네이션
		SI_A_CC_IMPACT_DETONATION,						/// 임팩트 디토네이션
		SI_A_CC_RELOAD_CANNON,							/// 탄전
		SI_A_CC_AIMMING_SHOT,							/// 에이밍 샷
		SI_A_CC_SIEGE_SHELLING = 6205,					/// 고정 포격
		SI_A_CFG_STEEL_EDGE,							/// 스틸 엣지
		SI_A_CFG_GUARD,									/// 가드
		SI_A_CFG_LEAP_ATTACK,							/// 리프어택
		SI_A_CIP_SUDDEN_BURSTER,						/// 서든 버스터
		SI_A_CIP_ARTILLERY_NOVA = 6210,					/// 아틸러리 노바
		SI_A_CIP_BURST_WOLF,							/// 버스트 울프
		SI_A_CSG_DOUBLE_FIRE,							/// 더블 파이어
		SI_A_CSG_DUAL_BUSTER,							/// 듀얼 버스터
		SI_A_CSG_HEAD_SHOT,								/// 헤드샷
		SI_A_CSG_MAGNUM_SHOT = 6215,					/// 매그넘 샷
		SI_A_CDC_RUMBLE_SHOT,							/// 럼블샷
		SI_A_CDC_BULLET_BLITZ,							/// 불렛 블릿츠
		SI_A_CDC_GATLING_SHOT,							/// 개틀링 샷
		SI_A_CDC_LEG_SHOT,								/// 레그샷
		SI_A_CHG_CANNON_STRIKE = 6220,					/// 캐논 스트라이크
		SI_A_CHG_ELASTIC_BOMB_GRENADE,					/// 일레스틱 밤
		SI_A_CHG_ELASTIC_BOMB_LAUNCHER,					/// 일레스틱 밤 - 그리네이드 런처
		SI_A_CHG_MARK_OF_COMMANDER,						/// 지휘관의 표식
		SI_A_CTT_WONDER_WALL,							/// 원더 월
		SI_A_CTT_AUTOMATIC_MORTAR = 6225,				/// 박격포
		SI_A_CTT_BOMBARD_SERVICE,						/// 지원 포격

		SI_P_CFG_METABOLISM_BOOST = 6400,				/// 신진대사 촉진
		SI_P_CFG_GUARD_MASTERY,							/// 방어숙련
		SI_P_CFG_ELEMENTAL_DEFENCE_A,					/// 불과 물과 자연
		SI_P_CFG_ELEMENTAL_DEFENCE_B,					/// 바람과 해와 달
		SI_P_CIP_ADVANCED_METABOLISM_BOOST,				/// 신진대사 촉진 강화
		SI_P_CIP_IRON_WILL = 6405,						/// 굳건한 의지
		SI_P_CSG_REMODEL_MAGAZINE,						/// 탄창/약실 개조
		SI_P_CSG_REMODEL_RIFLE,							/// 강선 개조
		SI_P_CDC_TUSSLE_TECHNIQUE,						/// 난전의 기술
		SI_P_CDC_ACCURATE_MARKMANSHIP,					/// 정밀한 사격술
		SI_P_CDC_INTENSE_SHOWTIME = 6410,				/// 강렬한 쇼타임
		SI_P_CDC_GAS_PISTON_SYSTEM,						/// 가스 피스톤 시스템
		SI_P_CHG_RELOAD_MASTERY,						/// 리로드 마스터리
		SI_P_CHG_CONVERTED_WARHEAD,						/// 탄두 개조
		SI_P_CHG_CANNONEER_WITHSTANDING,				/// 포병의 버티기
		SI_P_CTT_MOBILE_ARMORED_MODE = 6415,			/// 이동 포격
		SI_P_CTT_REACTIVE_ARMOR,						/// 반응 장갑

		SI_HA_CIP_DOOM_STRIKER = 6600,					/// 툼 스트라이크
		SI_HA_CDC_OUTRAGE_STRIKE,						/// 아웃레이지 스트라이크
		SI_HA_CTT_SATELITE_RAIN,						/// 세틀라이트 레인

		// Ara
		SI_SA_AM_PULLING_PIERCE = 7000,					/// 천의_빨래 널기
		SI_SA_AM_TEMPEST_DANCE,							/// 선풍_휘모리
		SI_SA_AM_FALLING_DRAGON,						/// 강룡추_구름 떨구기
		SI_SA_AM_SHADOW_KNOT,							/// 결영_그림자 매듭_귀살 3식
		SI_SA_ALH_MOONLIGHT_SLASH,						/// 용아 4식 - 달빛 베기
		SI_SA_ALH_PEERLESSNESS_SLASH = 7005,			/// 무쌍참
		SI_SA_ALH_WHITE_TIGER,							/// 맹호격
		SI_SA_ALH_SWALLOW_ASSAULT,						/// 비연
		SI_SA_ASD_SUPPRESSION,							/// 제압
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		SI_SA_ALD_FINGER_BULLET,						/// 나찰 1식 흡혼
		SI_SA_ALD_WOLF_SPEAR = 7010,					/// 낭아 4식 늑대 이빨
		SI_SA_ALD_ENERGY_WAVE,							/// 기공파
		SI_SA_AYR_SUPPRESSION_ENERGY,					/// 제압 : 기
		SI_SA_ALD_ENERGY_BEAD,							/// 나찰 2식 연환장
		SI_SA_ALD_HELL_OF_TORNADO,						/// 연환지옥
		SI_SA_ALD_PRISON_SPEAR = 7015,					/// 나찰 3식 창살감옥
		SI_SA_AYR_ENERGY_VOID,							/// 초진공장
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		SI_A_AM_WIND_WEDGE = 7200,						/// 연풍설_바람 쐐기_낭아 2식
		SI_A_AM_SHADOW_RISING,							/// 벽력뢰_천둥 꽃망울
		SI_A_AM_TIGER_CLAW,								/// 호랑이_발톱_맹호 2식
		SI_A_AM_FORCE_SPEAR,							/// 기공창
		SI_A_AM_GAIN_FORCE,								/// 흡공
		SI_A_AM_QUICK_PIERCING = 7205,					/// 신속
		SI_A_AM_SHOULDER_BLOW,							/// 철산고
		SI_A_AM_QUICK_STAB,								/// 천격
		SI_A_ALH_ROCK_CRASH,							/// 맹호 1식 - 바위깨기
		SI_A_ALH_BREAKING,								/// 이화접목
		SI_A_ALH_DOUBLE_COLLISION = 7210,				/// 용아 3식 - 팔괘장
		SI_A_ASD_DRAGON_FALLING,						/// 용추
		SI_A_ASD_TURBULENT_WAVE,						/// 맹호 4식 - 노도
		SI_A_ASD_LOW_BRANDISH,							/// 맹호 3식 - 지축
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		SI_A_ALD_WOLF_CLAW,								/// 낭아 3식 늑대 발톱
		SI_A_ALD_CONVERSION_HEALTH = 7215,				/// 이혈공
		SI_A_AYR_HIGH_SPEED,							/// 쾌속
		SI_A_ALD_CONVERSION_MANA,						/// 이기공
		SI_A_AYR_STEAL_SOUL,							/// 나찰 4식 탈혼
		SI_A_ALD_REFLECTION,							/// 반탄공
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		SI_P_ALH_QUICK_STAND = 7400,					/// 재빠른 기상
		SI_P_ALH_VIGOR_ENERGIZE,						/// 기력 활성화
		SI_P_ALH_FILLED_POWER,							/// 충만한 힘
		SI_P_ASD_QUICKEN_GUARD,							/// 번득이는 기질
		SI_P_ASD_BRUTAL_PIERCING,						/// 창술의 극의
		SI_P_ASD_FULLMOON_SLASH = 7405,					/// 만월 베기
		SI_P_ASD_POWER_OF_TIGER,						/// 맹호의 힘
		SI_P_ASD_SELF_PROTECTION_FORTITUDE,				/// 호신강기
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		SI_P_AYR_ENERGY_BULLET_REINFORCE,				/// 기탄 강화
		SI_P_AYR_HOWLING,								/// 늑대의 울부짖음
		SI_P_AYR_RESURRECTION = 7410,					/// 회광반조
		SI_P_ALD_ENERGY_REINFORCE,						/// 기력 강화
		SI_P_AYR_HELL_GATE_OPEN,						/// 지옥 초래
		
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		SI_HA_ASD_THOUSANDS_BLOSSOMS = 7600,			/// 분기등천 - 천수화
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		SI_SA_AYR_ENERGY_CANNON,						/// 대규환지옥
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#else //SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		// 패시브 필살기, 캐릭터 공통	1~299
		SI_P_COMMON_PHYSIC_ATTACK_BEGINNER = 1,			//기초근력단련
		SI_P_COMMON_MAGIC_ATTACK_BEGINNER,				//기초마법수련
		SI_P_COMMON_PHYSIC_DEFENCE_BEGINNER,			//기초물리방어술
		SI_P_COMMON_MAGIC_DEFENCE_BEGINNER,				//기초마법방어술
		SI_P_COMMON_PHYSIC_ATTACK_INTERMEDIATE,			//중급근력단련
		SI_P_COMMON_MAGIC_ATTACK_INTERMEDIATE,			//중급마법수련
		SI_P_COMMON_PHYSIC_DEFENCE_INTERMEDIATE,		//중급물리방어술
		SI_P_COMMON_MAGIC_DEFENCE_INTERMEDIATE,			//중급마법방어술

		SI_P_COMMON_GET_CHANCE,							//기회의포착				
		SI_P_COMMON_GET_MANA_UP,						//활력						
		SI_P_COMMON_POWERFUL_VITAL,						//강인한신체				
		SI_P_COMMON_POWERFUL_SOUL,						//강인한정신력				
		SI_P_COMMON_SUPPORT_AURA,						//격려의기운				
		SI_P_COMMON_UNLIMIT_MANA,						//끝없는마력				
		SI_P_COMMON_SHADOW_DATH,						//죽음의그림자				
		SI_P_COMMON_PHYSIC_HARMONY,						//육체와정신의조화			
		SI_P_COMMON_SPRINTER,							//스프린터					
		SI_P_COMMON_SOFTBODY,							//유연한몸놀림				
		SI_P_COMMON_FIGHTER_SOUL,						//격투가의혼				
		SI_P_COMMON_CONFIDENCE_VICTORY,					//승리의자신감
		SI_P_COMMON_HARDEN_BODY,						//단련된신체				
		SI_P_COMMON_REVERSAL_SOLDIER,					//역전의용사
		SI_P_COMMON_INDUCE_DESIRE_WINNING,				//호승심유발				
		SI_P_COMMON_REFINED_STEP,						//우아한발걸음				


		// 버프 필살기, 캐릭터 공통		300~599
		SI_SA_COMMON_POWER_ACCEL = 300,					//파워엑셀레이터
		SI_SA_COMMON_MAGIC_ACCEL,						//매직엑셀레이터
		SI_SA_COMMON_SHIELD_ACCEL,						//실드엑셀레이터
		SI_SA_COMMON_AURA_POWER_ACCEL,					//오러파워엑셀레이터
		SI_SA_COMMON_AURA_MAGIC_ACCEL,					//오러매직엑셀레이터
		SI_SA_COMMON_AURA_SHIELD_ACCEL,					//오러실드엑셀레이터
		SI_SA_COMMON_POWER_ADRENALIN,					//파워아드레날린
		SI_SA_COMMON_MAGIC_ADRENALIN,					//매직아드레날린
		SI_SA_COMMON_SHIELD_ADRENALIN,					//실드아드레날린
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		SI_SA_COMMON_AURA_SPEED_ACCEL			= 309,	// 오러 스피드 액셀러레이터
#endif






		// 액티브 필살기, 캐릭터 순서데로, 캐릭터 하나당 skill id의 범위는 500
		// elsword
		SI_SA_ES_MEGASLASH = 1000,						//메가슬래시
		SI_SA_ES_ASSAULT_SLASH,							//어설트슬래시
		SI_SA_ES_FATAL_FURY,							//페이탈퓨리
		SI_SA_ES_FLAME_GEYSER,							//프레임게이져
		SI_SA_ES_UNLIMITED_BLADE,						//언리미티드블레이드
		SI_SA_ES_TRIPLE_GEYSER,							//트리플게이져
		SI_SA_ESK_AIR_SLASH,							//에어슬래시
		SI_SA_ESK_SPIRAL_BLAST,							//스파이럴블래스트
		SI_SA_ESK_ARMAGEDON_BLADE,						//아마겟돈블레이드
		SI_SA_EMK_WIND_BLADE,							//윈드블레이드
		SI_SA_EMK_RISING_SLASH,							//라이징슬래시
		SI_SA_EMK_STORM_BLADE,							//스톰블레이드
		SI_SA_ESK_WINDMILL,								
		SI_SA_EMK_SWORD_FIRE,
		SI_SA_ESK_DOUBLE_SLASH,							//더블 슬래시
		SI_SA_EMK_PHOENIX_TALON,						//피닉스 탤런				
		SI_SA_ELK_SONIC_BLADE,	
		SI_SA_ERS_SPLASH_EXPLOSION,
		SI_SA_ELK_SAND_STORM,							/// 샌드 스톰				// oasis907
		SI_SA_ERS_LUNA_BLADE,							/// 루나 블레이드			// JHKang
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
		SI_SA_ETK_CRESCENT_CUT,							/// 크레센트 컷
		SI_SA_ETK_SWORD_FALL,							/// 소드폴	
		SI_SA_ETK_FINAL_STRIKE,							/// 파이널 스트라이크
		SI_SA_ETK_MAELSTORM_RAGE,						/// 메일스톰 레이지
		SI_SA_ETK_PHANTOM_SWORD,						/// 팬텀 소드
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ELSWORD_INFINITY_SWORD
		SI_SA_EIS_SWORD_BLASTING	= 1025,				//  소드 블래스팅
		SI_SA_EIS_RAGE_CUTTER,							//  레이지 커터
#endif //SERV_ELSWORD_INFINITY_SWORD
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		SI_SA_EMK_RISING_SLASH_WAVE	= 1026,					//  라이징 웨이브	
#endif //UPGRADE_SKILL_SYSTEM_2013

		SI_A_ES_COUNTER = 1200,							//반격						
		SI_A_ES_ENDURANCE,								//극기						
		SI_A_ES_KICK,									//발차기					
		SI_A_ESK_ARMOR_BREAK,														
		SI_A_EMK_BIG_BURST,															
		SI_A_ELK_ROLLING_SMASH,
		SI_A_ERS_SWORD_ENCHANT,
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
		SI_A_ES_ROLLING,								/// 구르기
		SI_A_ETK_HARSH_CHASER,							/// 하쉬 체이서
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ELSWORD_INFINITY_SWORD
		SI_A_EIS_MIRAGE_STING	= 1209,					// 미라지 스팅
#endif //SERV_ELSWORD_INFINITY_SWORD
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		SI_A_ELK_IMPACT_SMASH,							//  임팩트 스매시
		SI_A_ES_SWORD_WAVE,								//  소드 웨이브
		SI_A_ES_DEFENCE,								//  막기			
		SI_A_ERS_CRITICAL_SWORD,						//	크리티컬 소드
		SI_A_ERS_RUNE_OF_FIRE,							//  초열의 룬
		SI_A_ERS_RUNE_OF_ICE,							//  한빙의 룬
		SI_A_ETK_PIERCING_SWORD,						//  피어싱 소드
		SI_A_EIS_FATAL_SLAP,							//  페이탈 슬랩
		SI_A_ETK_SWORD_SHIELD,							//  소드 실드
#endif //UPGRADE_SKILL_SYSTEM_2013


		SI_P_ES_COUNTER_UP	= 1400,						//반격연마					
		SI_P_ES_POWERFUL_FIREBALL,						//강화된파이어볼			
		SI_P_ELK_STRONG_BONE,			
		SI_P_ERS_SPELL_CHAIN,
		SI_P_ELK_REDUCE_PAIN,							/// 고통억제				// kimhc // 2010-11-15
		SI_P_ERS_MAGIC_RESISTANCE,						/// 마법저항 수련			// oasis907
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
		SI_P_ETK_COMPACT_COUNTER,
		SI_P_ETK_BRUTAL_SLAYER,							/// 잔혹한 학살자								
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ELSWORD_INFINITY_SWORD
		SI_P_EIS_MIND_OF_FIGHTER	= 1408,				// 승부사의 기질
		SI_P_EIS_LIGHTNING_STEP,						// 전광 석화
#endif //SERV_ELSWORD_INFINITY_SWORD
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		SI_P_ESK_AUTO_GUARD,							//  오토 가드
		SI_P_ERS_RUNE_MASTERY,							//  룬 마스터리
		SI_P_ESK_INTENSIVE_ATTACK,						//  강렬한 일격
		SI_P_ELK_INDURANCE_OF_REVENGE,					//  역전의 인내
		SI_P_EIS_CUTTING_SWORD,							//  날카로운 검
#endif //UPGRADE_SKILL_SYSTEM_2013


		// aisha
		SI_SA_AV_CHAIN_FIRE_BALL = 1500,				//체인파이어볼
		SI_SA_AV_BINDING_CIRCLE,						//바인딩서클
		SI_SA_AV_GUST_SCREW,							//거스트스크류
		SI_SA_AV_LIGHTNING_BOLT,						//라이트닝볼트
		SI_SA_AV_GUST_STORM,							//거스트스톰
		SI_SA_AV_METEO_CALL,							//메테오콜
		SI_SA_AHM_CHAIN_BURST,							//체인버스트
		SI_SA_AHM_MAGIC_MISSILE,						//매직미사일
		SI_SA_AHM_INFERNAL_WAVE,						//인퍼널웨이브
		SI_SA_ADM_HELL_STONE,							//헬스톤
		SI_SA_ADM_DARK_CLOUD,							//다크클라우드
		SI_SA_ADM_PLASMA_CUTTER,						//플라즈마커터
		SI_SA_AHM_BLIZZARDSHOWER,
		SI_SA_ADM_DEATHFIELD,
		SI_SA_AHM_BLAZE_STEP,							//블레이즈스탭
		SI_SA_ADM_AGING,								//에이징
		SI_SA_AEM_CYCLONE,
		SI_SA_AVP_HELL_DROP,

		SI_SA_AEM_METEOR_SHOWER					= 1518,
		SI_SA_AVP_PHANTOM_BREATHING				= 1519,

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		SI_SA_ABM_HEAVY_PRESS					= 1520,	// 헤비 프레스
		SI_SA_ABM_ENERGY_SPURT,							// 에너지 스퍼트
		SI_SA_ABM_SUPER_NOVA,							// 슈퍼 노바
		SI_SA_ABM_GUILLOTINE_PRESS,						// 길로틴 프레스
		SI_SA_ABM_MAGICAL_MAKEUP,						// 메지컬 메이크업
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
		SI_SA_ADW_MORNING_STAR	= 1525,					// 모닝 스타
		SI_SA_ADW_SCREWDRIVER,							// 스크류 드라이버
#endif


		SI_A_AV_TELEPORT = 1700,						//텔레포트					
		SI_A_AV_MANA_SHIELD,							//마나실드					
		SI_A_AV_CIRCLE_FLAME,							//서클플래임				
		SI_A_AHM_MEDITATION,														
		SI_A_ADM_MANA_INTAKE,							//마나인테이크				
		SI_A_AEM_STONE_WALL,
		SI_A_AVP_SUMMON_BAT,
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		SI_A_AV_FIRE_ROAD,								// 파이어 로드
		SI_A_ABM_ENERGY_DRAIN,							// 에너지 드레인
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
		SI_A_ADW_DISTORTION	= 1709,						// 공간 왜곡
#endif


		SI_P_AV_TELEPORT_UP = 1900,						//텔레포트연마				
		SI_P_AV_FIREBALL_UP,							//파이어볼연마				
		SI_P_AEM_ELEMENTAL_FRIENDSHIP,
		SI_P_AVP_STRONG_MIND, 
		SI_P_AEM_ENERGY_OF_THE_PLENTY,					/// 풍요의 기운				// oasis907

		SI_P_AVP_LIMITED_MANA_MANAGEMENT		= 1905,
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		SI_P_ABM_FITNESS						= 1906,	// 신체 단련
		SI_P_ABM_MANAFLOW_OVERDRIVE,					// 기혈 폭주
		SI_P_ABM_SPIRIT_ACCELERATION,					// 기력 가속
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
		SI_P_ADW_ADVANCED_TELEPORTATION	= 1909,			// 공간 도약
		SI_P_ADW_MAGICAL_STAFF,							// 마력의 지팡이

#endif

		// rena
		SI_SA_LE_RAIL_STINGER = 2000,					//레일스팅거
		SI_SA_LE_ASSAULT_KICK,							//어설트킥
		SI_SA_LE_PERFECT_STORM,							//퍼팩트스톰
		SI_SA_LE_MULTIPLE_STINGER,						//멀티플스팅거
		SI_SA_LE_AERO_TORNADO,							//에어로토네이도
		SI_SA_LE_PHOENIX_STRIKE,						//피닉스스트라이크
		SI_SA_LCR_SPINNING_KICK,						//스피닝킥
		SI_SA_LCR_CRESCENT_KICK,						//크레센트킥
		SI_SA_LCR_DIVE_KICK_BOMBING,					//다이브킥봄잉
		SI_SA_LSR_HUMMING_WIND,							//허밍윈드
		SI_SA_LSR_GUIDED_ARROW,							//가이드애로우
		SI_SA_LSR_CRAZY_SHOT,							//크레이지샷
		SI_SA_LCR_SHARPFALL,
		SI_SA_LSR_WINDWARD,
		SI_SA_LCR_VIOLENT_ATTACK,						//바이올런트 어택
		SI_SA_LSR_GUNGNIR,								//궁그닐
		SI_SA_LWS_SLIDE_KICK, 
		SI_SA_LGA_FREEZING_ARROW, 
		SI_SA_LWS_AIRELINNA,							/// 아이레린나				// oasis907

		SI_SA_LGA_AERO_STRAFE					= 2019,

#ifdef	SERV_TRAPPING_RANGER_TEST
		SI_SA_RTR_EVOKE							= 2020,	///	이보크
		SI_SA_RTR_FATALITY						= 2021,	///	페이탈리티
		SI_SA_RTR_TRAPING_ARROW					= 2022,	///	트래핑 애로우
		SI_SA_RTR_CALL_OF_RUIN					= 2023,	///	콜 오브 루인
		SI_SA_RTR_KARMA							= 2024,	///	카르마
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		SI_SA_RNW_FURIOUS_ENGAGE				= 2025,	///	퓨리어스 인게이지
		SI_SA_RNW_GLIDING_STRIKE				= 2026,	///	글라이딩 스트라이크
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		SI_SA_RTR_TRAPPING_ARROW_FUNGUS,				/// 트래핑 애로우 - 펑거스
		SI_SA_LWS_AIRELINNA_SYLPH,						/// 아이레린나 - 실프
#endif //UPGRADE_SKILL_SYSTEM_2013


		SI_A_LE_SIEGEMODE = 2200,						//시즈모드					
		SI_A_LE_BACKJUMP_SHOT,							//백덤블링샷
		SI_A_LE_REFLEX_MAGIC,							//마법반사
		SI_A_LCR_LOW_KICK,								//로우킥					// wonpok. 작업완료
		SI_A_LSR_ENTANGLE,															// dmlee 작업중
		SI_A_LWS_NATURE_FORCE, 
		SI_A_LGA_STIGMA_ARROW, 

#ifdef	SERV_TRAPPING_RANGER_TEST
		SI_A_LE_RISING_FALCON					= 2207,	///	라이징 팔콘
		SI_A_RTR_EXPLOSION_TRAP					= 2208,	///	폭발의 덫
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		SI_A_RNW_START_OF_DELAYED_FIRING		= 2209,	///	지연의 신호탄
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		SI_A_LCR_MIDDLE_KICK,							/// 미들킥
		SI_A_LWS_BACK_KICK,								/// 백킥
		SI_A_LWS_HIGH_KICK,								/// 하이킥
		SI_A_LSR_SHOOTING_MAGNUM,						/// 슈팅 매그넘
		SI_A_LGA_ARC_SHOT,								/// 아크 샷
		SI_A_LGA_RAPID_SHOT,							/// 래피드 샷
		SI_A_LGA_FREEZING_BOLT,							/// 프리징 볼트
		SI_A_LGA_WIND_BLAST,							/// 윈드 블래스트
		SI_A_RNW_THORNS_TRAP,							/// 가시덩굴 덫
		SI_P_RNW_SEED_OF_RUIN,							/// 진노의 씨앗
		SI_A_RNW_ANGER_OF_ELF,							/// 엘드랏실의 분노
		SI_A_RNW_ROSEBUSH_TRAP,							/// 찔레 덫
		SI_A_LWS_ASSAULT_IMPACT,						/// 어설트 임팩트
#endif //UPGRADE_SKILL_SYSTEM_2013


		SI_P_LE_SIEGEMODE_UP = 2400,					//시즈모드연마				
		SI_P_LE_SHARP_KICK,								//날렵한발차기				
		SI_P_LE_POWERFUL_BOWSTRING,						//강화된보우스트링			
		SI_P_LE_HAWK_EYE,								//호크아이					
		SI_P_LE_SHARP_ARROW,							//다듬어진화살				
		SI_P_LWS_POWERFUL_SHOT, 
		SI_P_LGA_COMMUNE_OF_NATURE,
		SI_P_LWS_FRIENDSHIP_OF_NATURE,					/// 자연과의 친화			// oasis907
		SI_P_LGA_VIBRATION_SHOOTING,					/// 진동 사격				// oasis907

#ifdef	SERV_TRAPPING_RANGER_TEST
		SI_P_RTR_HUNTERS_ABILITY				= 2409,	///	사냥꾼의 재능(패시브)
		SI_P_RTR_SHARPEN_ARROW					= 2410,	///	날카로운 화살(패시브)
		SI_P_RTR_VITALPOINT_PIERCING			= 2411,	///	급소 찌르기(패시브)
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		SI_P_RNW_PRIOR_PLANNED_BLOW				= 2412,	///	계산된 일격
		SI_P_RNW_GUARDIAN_OF_ELF,						/// 엘프의 수호자
#endif


		// raven
		SI_SA_RF_MAXIMUM_CANNON = 2500,					//맥시멈캐논
		SI_SA_RF_EARTH_BREAKER,							//어스브레이크
		SI_SA_RF_POWER_ASSAULT,							//파워어설트
		SI_SA_RF_CANNON_BLADE,							//캐논블레이드
		SI_SA_RF_MAGNUM_BLASTER,						//매그넘블래스터
		SI_SA_RF_SEVEN_BURST,							//세븐버스트
		SI_SA_RST_BERSERKER_BLADE,						//버서커블레이드
		SI_SA_RST_FLYING_IMPACT,						//플라잉임팩트
		SI_SA_RST_HYPER_SONIC_STAB,						//하이퍼소닉스탭
		SI_SA_ROT_CHARGED_BOLT,							//차지드볼트
		SI_SA_ROT_ARC_ENEMY,							//아크애너미
		SI_SA_ROT_GUARDIAN_STRIKE,						//가디언스트라이크
		SI_SA_RST_SHOCKWAVE,
		SI_SA_ROT_VALKYRIESJAVELIN,
		SI_SA_RST_BLOODY_ACCEL,							//블러디엑셀
		SI_SA_ROT_NUCLEAR,								//뉴클리어	
		SI_SA_RBM_WOFL_FANG,							//울프팽
		SI_SA_RRF_X_CRASH,								//엑스 크래시

		SI_SA_RBM_GIGA_DRIVE,							/// 기가 드라이브			// kimhc // 2010-11-15
		SI_SA_RRF_WILD_CHARGE,							/// 와일드 차지				// kimhc // 2010-11-15

#ifdef SERV_RAVEN_WEAPON_TAKER
		SI_SA_RWT_BURSTING_BLADE,						// 버스팅 블레이드
		SI_SA_RWT_REVOLVER_CANNON,						// 리볼버 캐논
		SI_SA_RWT_HARPOON_SPEAR,						// 하푼 스피어
		SI_SA_RWT_HELLFIRE_GATLING,						// 헬파이어 개틀링
		SI_SA_RWT_GIGA_PROMINENCE,						// 기가 프로미넌스
#endif SERV_RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		SI_SA_RVC_DEADLY_RAID = 2525,					// 데들리 레이드
		SI_SA_RVC_IGNITION_CROW,						// 이그니션 크로우
#endif SERV_RAVEN_VETERAN_COMMANDER


		SI_A_RF_BURNING_RUSH = 2700,					//버닝러시					
		SI_A_RF_SHADOW_STEP,							//섀도우스탭
		SI_A_RST_CUT_TENDON,							//컷텐돈		//빠졌길래 추가. 2011/12/19 박진웅
		SI_A_ROT_WEAPON_BREAK,							//무기 깨기		//빠졌길래 추가. 2011/12/19 박진웅
		SI_A_RBM_ONE_FLASH,								//일섬
		SI_A_RRF_GROUND_IMPACT,							//그라운드 임팩트
#ifdef SERV_RAVEN_WEAPON_TAKER
		SI_A_RF_BREAKING_FIST,							// 브레이킹 피스트
		SI_A_RWT_BARRAGE_ATTACK,						// 연무 폭파
#endif SERV_RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		SI_A_RVC_BLEEDING_SLICER = 2708,				// 치명상
#endif SERV_RAVEN_VETERAN_COMMANDER


		SI_P_RF_THRUST = 2900,							//섀도우피어싱
		SI_P_RBM_BLADE_MASTERY,							//숙련된 검술
		SI_P_RRF_BURNING_NASOD_HAND,					//타오르는 나소드 핸드
		SI_P_RBM_EMERGENCY_ESCAPE,						/// 긴급 탈출
		SI_P_RRF_SPIRITUALIZED_FURY,					/// 승화된 분노
#ifdef SERV_RAVEN_WEAPON_TAKER
		SI_P_RWT_AIR_SLICER,							// 에어 슬라이서
		SI_P_RWT_BEHAVIOR_OF_MERCENARY,					// 용병의 처신술
#endif SERV_RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		SI_P_RVC_SHADOW_BACK_SLIDE = 2907,				// 섀도우 백 슬라이드
		SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY,		// 용병의 생존술
#endif SERV_RAVEN_VETERAN_COMMANDER


		// eve
		SI_SA_EN_ILLUSION_STRIKE = 3000,				//일루전스트라이크
		SI_SA_EN_DIMENSION_LINK,						//디멘션링크
		SI_SA_EN_ILLUSION_STINGER,						//일루전스팅어
		SI_SA_EN_DIMENSION_LINK_BLADER,					//디멘션링크블레이더
		SI_SA_EN_GENERATE_BLACKHOLE,					//제네레이터블랙홀
		SI_SA_EN_DIMENSION_LINK_GUARDIAN,				//디멘션링크가디언
		SI_SA_EEG_HORNET_STING,							//호넷스팅
		SI_SA_EEG_EXPLOSION_IMPACT,						//익스플로전임팩트
		SI_SA_EEG_QUEENS_THRONE,						//퀸스쓰론
		SI_SA_EAT_SURFACE_CUTING,						//서피스커팅
		SI_SA_EAT_SONIC_WAVE,							//소닉웨이브
		SI_SA_EAT_GENOCIDE_RIPPER,						//제노사이드리퍼
		SI_SA_EEG_IRONSCRAPS,
		SI_SA_EAT_SPACE_WRENCH,
		SI_SA_EEG_JUNK_BREAK,							//정크브레이크
		SI_SA_EAT_HEAVENS_FIST,							//해븐즈피스트		
		SI_SA_ENS_ATOMIC_BLASTER,						//아토믹블래스터
		SI_SA_EEP_ELECTRONIC_FIELD,						//일렉트로닉필드
		SI_SA_ENS_ATOMIC_SHIELD,						/// 아토믹 쉴드
		SI_SA_EPP_ASSUALT_SPEAR,						/// 어설트 스피어
#ifdef SERV_EVE_ELECTRA
		SI_SA_EEL_PARTICLE_RAY,							// 파티클 레이
		SI_SA_EEL_SWEEP_ROLLING,						// 스위프 롤링
		SI_SA_EEL_PHOTON_FLARE,							// 포톤 플레어
		SI_SA_EEL_GIGA_STREAM,							// 기가 스트림
		SI_SA_EEL_THOUSANDS_OF_STARS,					// 사우전드 스타
#endif SERV_EVE_ELECTRA
#ifdef SERV_EVE_BATTLE_SERAPH
		SI_SA_EBS_ENERGY_NEEDLES,						/// 에너지 니들스
		SI_SA_EBS_ENERGETIC_HEART,						/// 에너제틱 하트
#endif

		SI_A_EN_CLOAKING = 3200,						//클로킹					
		SI_A_EN_CONVERSION,								//마력전환					
		SI_A_EN_MEGA_ELECTRONBALL,						//메가일랙트론볼
		SI_A_EEG_METAL_DUST_AURA,						//메탈더스트아우라
		SI_A_EAT_OBERON_GUARD,							//오베론가디안
		SI_A_ENS_SPEAR_BURST,							//스피어버스트
		SI_A_EEP_SPIT_FIRE,								//스핏파이어
#ifdef SERV_EVE_ELECTRA
		SI_A_EVE_PHOTON_BLINK,							// 광자 점멸
		SI_A_EEL_SPECTRO_EL_CRYSTAL,					// 엘 분광 결정
#endif SERV_EVE_ELECTRA
#ifdef SERV_EVE_BATTLE_SERAPH
		SI_A_EBS_TASER_PILUM,							/// 테이저 필라 (복수형 발음)
#endif

		SI_P_EN_CLOAKING_UP = 3400,						//클로킹연마				
		SI_P_EN_ELECTRONBALL_UP,						//일렉트론볼연마
		SI_P_EN_QUEENS_CONTROL,							//여왕의지배력				
		SI_P_EN_QUEENS_POTENTIAL,						//여왕의잠재력
		SI_P_ENS_NASOD_WEAPON,							//나소드 무기 강화
		SI_P_EEP_ELECTRONBALL_MASTER,					//일렉트론볼마스터
		SI_P_ENS_ENERGY_OF_CONCENTRATION,				/// 집중의 기운			// oasis907
		SI_P_EEP_CHARGING_BOOSTER,						/// 충전추진기			// kimhc // 2010-11-
#ifdef SERV_EVE_ELECTRA
		SI_P_EEL_SPECTRUM_DYNAMICS,						// 분광 역학
		SI_P_EEL_PHOTON_BOOSTER,						// 광자 추진기
		SI_P_EEL_ADVANCED_EL_ENERGY_REACTOR,			// 강화 엘 에너지 반응로
#endif SERV_EVE_ELECTRA
#ifdef SERV_EVE_BATTLE_SERAPH
		SI_P_EBS_HYPER_OPTICAL_RESEARCH,				/// 초광학 연구
		SI_P_EBS_HIGHPOWERED_ELECTRICS,					/// 고출력 전자회로 (공돌이 스킬, 납땜기 들고 와라!)
#endif

		
		///////////////////////////// CHUNG //////////////////////////////////////////////////////////////
		//////////////////////////// 스페셜 액티브
		SI_SA_CHUNG_BRUTAL_SWING	= 4000,				/// 브루탈 스윙
		SI_SA_CHUNG_DETONATION,							/// 디토네이션
		SI_SA_CHUNG_LUNATIC_BLOW,						/// 루나틱 블로우
		SI_SA_CHUNG_SCARE_CHASE,						/// 스케어 체이스
		SI_SA_CHUNG_GIGANTIC_IMPACT,					/// 기간틱 임팩트
		SI_SA_CHUNG_ACELDAMA,							/// 아겔다마

		//{{ kimhc // 2011.1.8 // 청 1차 전직
		SI_SA_CFG_STEEL_EDGE,							/// 스틸엣지 (퓨리가디언)
		SI_SA_CFG_IRON_HOWLING,							/// 아이언 하울링 (퓨리가디언)
		SI_SA_CFG_LUNATIC_FURY,							/// 루나틱퓨리 (퓨리가디언)

		SI_SA_CSG_DUAL_BUSTER,							/// 듀얼버스터 (슈팅가디언)
		SI_SA_CSG_HEAVY_RAILGUN,						/// 헤비레일건 (슈팅가디언)
		SI_SA_CSG_SHOOTING_STAR,						/// 슈팅스타 (슈팅가디언)
		//}} kimhc // 2011.1.8 // 청 1차 전직

		//{{ kimhc // 2011-3-16 // 2011-04 에 패치될 청 캐시 스킬
		SI_SA_CFG_PANDEMONIUM,							/// 판데모니움 (퓨리가디언)
		SI_SA_CFG_PAIN_OF_CALADBOLG,						/// 칼라볼그페인 (퓨리가디언)

		SI_SA_CSG_COMET_CRASHER,						/// 카밋크래셔 (슈팅가디언)
		SI_SA_CSG_SHARPSHOOTER_SYNDROME,				/// 샤프슈터신트롬 (슈팅가디언)
		//}} kimhc // 2011-3-16 // 2011-04 에 패치될 청 캐시 스킬

		SI_SA_CIP_BURST_WOLF,							/// 버스트 울프 (아이언 팔라딘)
		SI_SA_CIP_LAND_DEMOLISHIER,						/// 랜드 디몰리셔 (아이언 팔라딘)
		SI_SA_CDC_BULLET_BLITZ,							/// 불렛 블릿츠 (데들리 체이서)
		SI_SA_CDC_ARTILLERY_STRIKE,						/// 아틸러리 스트라이크 (데들리 체이서)

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		SI_SA_CHG_WONDER_WALL			= 4020,			// 원더 월
		SI_SA_CHG_DREAD_CHASE,							// 드레드 체이스
		SI_SA_CHG_BIGBANG_STREAM,						// 빅뱅 스트림
		SI_SA_CHG_CHAOS_CANNON,							// 카오스 캐논
		SI_SA_CHG_CARPET_BOMBING		= 4024,			// 카펫 바밍
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		SI_SA_CTT_CROSS_FIRE			= 4025,			// 크로스 파이어
		SI_SA_CTT_TACTICAL_FIELD		= 4026,			// 택티컬 필드
#endif //SERV_CHUNG_TACTICAL_TROOPER

		/////////////////////////////// 액티브
		SI_A_CHUNG_RELOAD_CANNON	= 4200,				/// 탄전
		SI_A_CHUNG_AIMMING_SHOT,						/// 에이밍 샷
		//{{ kimhc // 2011.1.10 // 청 1차 전직
		SI_A_CFG_GUARD,									/// 가드 (퓨리가디언)
		SI_A_CFG_LEAP_ATTACK,							/// 리프어택 (퓨리가디언)

		SI_A_CSG_HEAD_SHOT,								/// 헤드샷 (슈팅가디언)
		SI_A_CSG_RUMBLE_SHOT,							/// 럼플샷 (슈팅가디언)
		//}} kimhc // 2011.1.10 // 청 1차 전직

		SI_A_CIP_SUDDEN_BURSTER,						/// 서든 버스터 (아이언 팔라딘)
		SI_A_CDC_LEG_SHOT,								/// 레그샷 (데들리 체이서)

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		SI_A_CHUNG_SIEGE_SHELLING	= 4208,				// 고정 포격
		SI_A_CHG_ELASTIC_BOMB,							// 일레스틱 밤
		SI_A_CHG_MARK_OF_COMMANDER	= 4210,				// 지휘관의 표식
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		SI_A_CTT_AUTOMATIC_MORTAR		= 4211,				// 박격포
#endif //SERV_CHUNG_TACTICAL_TROOPER

		//////////////////////////////// 패시브
		SI_P_CHUNG_RELOAD_CANNON	= 4400,				/// 숙련된 탄전
		//{{ kimhc // 2011.1.10 // 청 1차 전직
		SI_P_CFG_METABOLISM_BOOST,						/// 신진대사 촉진 (퓨리가디언)
		SI_P_CFG_GUARD_MASTERY,							/// 방어숙련 (퓨리가디언)
		SI_P_CFG_ELEMENTAL_DEFENCE_A,					/// 불과 물과 자연 (퓨리가디언)
		SI_P_CFG_ELEMENTAL_DEFENCE_B,					/// 바람과 해와 달 (퓨리가디언)

		SI_P_CSG_REMODEL_MAGAZINE,						/// 탄창/약실 개조 (슈팅가디언)
		SI_P_CSG_REMODEL_RIFLE,							/// 강선 개조 (슈팅가디언)
		SI_P_CSG_TUSSLE_TECHNIQUE,						/// 난전의 기술 (슈팅가디언)
		//}} kimhc // 2011.1.10 // 청 1차 전직

		SI_P_CIP_ADVANCED_METABOLISM_BOOST,				/// 신진대사 촉진 강화 (아이언 팔라딘)
		SI_P_CIP_IRON_WILL,								/// 굳건한 의지 (아이언 팔라딘)
		SI_P_CDC_ACCURATE_MARKMANSHIP,					/// 정밀한 사격술 (데들리 체이서)
		SI_P_CDC_INTENSE_SHOWTIME,						/// 강렬한 쇼타임 (데들리 체이서)

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		SI_P_CHUNG_ABILITY_CANNONEER	= 4412,			// 숙련된 포병의 능력
		SI_P_CHG_RELOAD_MASTERY,						// 리로드 마스터리
		SI_P_CHG_CONVERTED_WARHEAD,						// 탄두 개조
		SI_P_CHG_CANNONEER_WITHSTANDING	= 4415,			// 포병의 버티기
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		SI_P_CTT_MOBILE_ARMORED_MODE	= 4416,			// 이동 포격
		SI_P_CTT_REACTIVE_ARMOR			= 4417,			// 반응 장갑
#endif //SERV_CHUNG_TACTICAL_TROOPER

#ifdef SERV_ARA_MARTIAL_ARTIST
		SI_SA_ARA_WIND_WEDGE			= 4500,		/// 연풍설_바람 쐐기_낭아 2식
		SI_SA_ARA_SHADOW_RISING_JUMP,				/// 벽력뢰_천둥 꽃망울
		SI_SA_ARA_PULLING_PIERCE,					/// 천의_빨래 널기
		SI_SA_ARA_TEMPEST_DANCE,					/// 선풍_휘모리
		SI_SA_ARA_FALLING_DRAGON,					/// 강룡추_구름 떨구기
		SI_SA_ARA_SHADOW_KNOT,						/// 결영_그림자 매듭_귀살 3식
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
		SI_SA_ALH_DOUBLE_COLLISION,					/// 용아 3식 - 팔괘장
		SI_SA_ALH_MOONLIGHT_SLASH,					/// 용아 4식 - 달빛 베기
		SI_SA_ALH_PEERLESSNESS_SLASH,				/// 무쌍참
		SI_SA_ALH_WHITE,TIGER,						/// 백호의 춤
		SI_SA_ALH_SWALLOW_ASSAULT,					/// 비연
		SI_SA_ASD_TURBULENT_WAVE,					/// 맹호 4식 - 노도
		SI_SA_ASD_SUPPRESSION,						/// 제압
#endif
		
		SI_A_ARA_TIGER_CLAW				= 4700,		/// 호랑이_발톱_맹호 2식
		SI_A_ARA_FORCE_SPEAR,						/// 기공창
		SI_A_ARA_GAIN_FORCE,						/// 흡공
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
		SI_A_ALH_ROCK_CRASH,						/// 맹호 1식 - 바위깨기
		SI_A_ALH_BREAKING,							/// 이화접목
		SI_A_ASD_LOW_BRANDISH,						/// 맹호 3식 - 지축
#endif

		SI_P_ARA_FORCE_SPEAR_MASTERY	= 4900,		/// 기공창 숙련
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
		SI_P_ALH_QUICK_STAND,						/// 재빠른 기상
		SI_P_ALH_VIGOR_ENERGIZE,					/// 기력 활성화
		SI_P_ALH_FILLED_POWER,						/// 충만한 힘
		SI_P_ASD_QUICKEN_GUARD,						/// 번득이는 기지
		SI_P_ASD_BRUTAL_PIERCING,					/// 창술의 극의
#endif

#endif

//#ifdef GUILD_SKILL
		SI_GP_COMMON_BALANCED_BODY					= 5000,		//균형잡힌 육체
		SI_GP_COMMON_HARMONIC_SPIRIT				= 5010,		//조화로운 영혼
		SI_GP_COMMON_CHEERING_OF_TEAM				= 5020,		//동료의 응원
		SI_GP_COMMON_MASTER_OF_ALCHEMIST			= 5030,		//연금술의 달인
		SI_GP_COMMON_CONCENTRATION_OF_SPIRIT		= 5040,		//정신 집중
		SI_GP_COMMON_CONCENTRATION_MAGICAL_POWER	= 5050,		//집중된 마력
		SI_GP_COMMON_VIOLENT						= 5060,		//격노
		SI_GP_COMMON_LIBERATION_OF_ANGER			= 5070,		//분노 해방
		// 여기까지 2트리
		SI_GP_COMMON_INFINITE_STRENGTH				= 5080,		//끝없는 체력
		SI_GP_COMMON_CHANCE_TO_REVERSE				= 5090,		//역전의 기회
		SI_GP_COMMON_SACRIFICE_OF_HERO				= 5100,		//영웅의 희생
		SI_GP_COMMON_MIDAS							= 5110,		//마이더스의 손
		SI_GP_COMMON_SUPPORT_AURA					= 5120,		//기사 회생
		SI_GP_COMMON_IMPROVE_CHANCE					= 5130,		//기회 포착
		SI_GP_COMMON_ESCAPE_FROM_CRISIS				= 5140, 	//위기 탈출
		SI_GP_COMMON_INSTRUCTION_OF_WISEMAN			= 5150,		//현자의 가르침
//#endif GUILD_SKILL

//#ifdef ADDED_RELATIONSHIP_SYSTEM
		SI_ETC_WS_COMMON_LOVE						= 6001,		/// 커플 스킬 천상지애
//#endif // ADDED_RELATIONSHIP_SYSTEM
*/
#endif //SERV_UPGRADE_SKILL_SYSTEM_2013

		// Elesis
		SI_SA_EL_WILD_SHOCK							= 8000,		/// 와일드 쇼크
		SI_SA_EL_POWER_BURSTER						= 8001,		/// 파워 버스터
		SI_SA_EL_MEGA_BURSTER						= 8002,		/// 메가 버스터
		SI_SA_EL_UNLIMITED_BLADE					= 8003,		/// 언리미티드 블레이드

		SI_SA_SSK_SPIRAL_BLAST						= 8004,		/// 스파이럴 블래스트
		SI_SA_SSK_EXTINCTION						= 8005,		/// 세이버 - 익스팅션
		SI_SA_SSK_VICTORIOUS_SWORD					= 8006,		/// 승리의 검
		SI_SA_SSK_JUDGEMENT_DRIVE					= 8007,		/// 저지먼트 드라이브

		SI_SA_SPK_BURST_RISING						= 8008,		/// 버스트 라이징
		SI_SA_SPK_BURST_WAVE						= 8009,		/// 버스트 웨이브
		SI_SA_SPK_SWORDFIRE							= 8010,		/// 소드 파이어
		SI_SA_SPK_INFERNAL_BLADE					= 8011,		/// 인페르날 블레이드
		SI_SA_SPK_ETERNAL_FIRE						= 8012,		/// 이터널 파이어
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		SI_SA_SGM_SONIC_BLADE						= 8013,		/// 소닉 블레이드
		SI_SA_SGM_CRITICAL_DIVE						= 8014,		/// 크리티컬 다이브
		SI_SA_SBH_SCALDIC_SWORD						= 8015,		/// 스칼딕 소드
		SI_SA_SBH_WALL_OF_PROMINENCE				= 8016,		/// 파이어 월
		SI_SA_SGM_JUGGERNAUT_BUSTER					= 8017,		/// 저거넛 버스터
		SI_SA_SGM_WAR_PRELUDE						= 8018,		/// 워 프렐류드
		SI_SA_SBH_BLAZE_WING						= 8019,		/// 블레이즈 윙	
		SI_SA_SBH_BLAZING_DANCE						= 8020,		/// 블레이징 댄스
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

		SI_A_EL_DODGE_AND_SLASH						= 8200,		/// 닷지&슬래시
		SI_A_EL_LEAP_CRASH							= 8201,		/// 도약
		SI_A_EL_RUSHING_SWORD						= 8202,		/// 러싱 소드
		SI_A_EL_KICK								= 8203,		/// 발차기
		SI_A_EL_MEGA_SLASH							= 8204,		/// 메가 슬래시
		SI_A_EL_SONIC_ASSAULT_STAB					= 8205,		/// 소닉 어설트 - 스탭
		SI_A_EL_SONIC_ASSAULT_STING					= 8206,		/// 소닉 어설트 - 스팅

		SI_A_EL_ENDURANCE_POWER						= 8207,		/// 극기 - 강

		SI_A_SSK_CROSS_SLASH						= 8208,		/// 크로스 슬래시
		SI_A_SSK_POWER_BREAK						= 8209,		/// 파워 브레이크
		SI_A_SSK_HEAVY_STUNNER						= 8210,		/// 헤비 스터너

		SI_A_SPK_SWORD_ERUPTION						= 8211,		/// 소드 이럽션
		SI_A_SPK_SCALDIC_SWORD						= 8212,		/// 불꽃의 검
		SI_A_SPK_BIG_BURST							= 8213,		/// 대폭살
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		SI_A_SGM_PROVOKE							= 8214,		/// 도발
		SI_A_SGM_DRASTIC_CHARGE						= 8215,		/// 드래스틱 차지
		SI_A_SGM_SNAP_CRASHER						= 8216,		/// 스냅 크래셔
		SI_A_SBH_FLAME_SWORD						= 8217,		///	홍련의 검
		SI_A_SBH_FIREWORKS							= 8218,		/// 불놀이
		SI_A_SBH_SCALET_ROSE						= 8219,		/// 불꽃 장미
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

		SI_P_SSK_AGILITY_MASTERY					= 8400,		/// 순발력 향상
		SI_P_SSK_CHIVALRY							= 8401,		/// 기사의 숙명
		SI_P_SPK_CONCENTRATION_MASTERY				= 8402,		/// 집중력 향상
		SI_P_SPK_BODY_OF_FIRE						= 8403,		/// 불의 신체
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		SI_P_SGM_ADVANCED_KNIGHT_MASTERY			= 8404,		/// 상급 기사 숙련
		SI_P_SGM_WALTZ_OF_SWORD						= 8405,		/// 검의 왈츠
		SI_P_SGM_FIRST_STRIKE						= 8406,		/// 선수필승
		SI_P_SBH_FIRE_BLOSSOMS						= 8407,		/// 불꽃 개화
		SI_P_SBH_UNEXTINGUISHABLE_FIRE				= 8408,		/// 꺼지지 않는 불꽃
		SI_P_SBH_STIGMA_OF_FIRE						= 8409,		/// 불꽃의 낙인	

		SI_HA_SGM_SWORD_OF_RELICS					= 8600,		/// 소드 오브 렐릭스
		SI_HA_SBH_ANCIENT_FIRE						= 8601,		/// 에인션트 파이어
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE	


		// Add
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		SI_SA_AN_VOID_BREAKER						= 9000,		/// 보이드 브레이커
		SI_SA_AN_PARTICLE_PRISM						= 9001,		/// 파티클 프리즘
		SI_SA_AN_ENERGY_BOOM						= 9002,		/// 에너지 붐
		SI_SA_AN_PARTICLE_ACCELERATOR				= 9003,		/// 파티클 액셀러레이터
		SI_SA_AN_NEUTRON_BOMB						= 9004,		/// 뉴트란 밤

		SI_SA_APT_PULSE_CANNON						= 9005,		/// 펄스 캐논
		SI_SA_APT_EMP_SHOCK							= 9006,		/// EMP 쇼크
		SI_SA_APT_QUAKE_BUSTER						= 9007,		/// 퀘이크 버스터
		SI_SA_APT_STASIS_FIELD						= 9008,		/// 스테이시스 필드
		SI_SA_APT_CONQUEROR							= 9009,		/// 퀀커러

	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		SI_SA_ALP_REVERSE_REACTOR					= 9010,		/// 리버스 리액터
		SI_SA_ALP_QUICKSILVER_ACCEL					= 9011,		/// 퀵실버 액셀
		SI_SA_ALP_QUICKSILVER_FRENZY				= 9012,		/// 퀵실버 프렌지
		SI_SA_ALP_DUST_ANATOMY						= 9013,		/// 더스트 아나토미
		SI_SA_ALP_PSYCHIC_STORM						= 9014,		/// 사이킥 스톰
	#endif //SERV_ADD_LUNATIC_PSYKER


		SI_A_AN_PULSE_BULLET						= 9200,		/// 입자탄
		SI_A_AN_MIND_BREAK							= 9201,		/// 정신 붕괴
		SI_A_AN_PHASE_SHIFT							= 9202,		/// 위상 변화

		SI_A_APT_MAGNETRON_DYNAMO					= 9203,		/// 다이너모 구성 - 마그네트론
		SI_A_APT_DESOLVER_DYNAMO					= 9204,		/// 다이너모 구성 - 디졸버
		SI_A_APT_PYLON_DYNAMO						= 9205,		/// 다이너모 구성 - 파일런

	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		SI_A_ALP_PSIONIC_BEAT						= 9206,		/// 사이오닉 비트
		SI_A_ALP_TWIRL_RUSH							= 9207,		/// 트월 러시
	#endif //SERV_ADD_LUNATIC_PSYKER


		SI_P_AN_CHARGED_IMPULSAR					= 9400,		/// 다이너모 구성 - 과충전기
		SI_P_AN_RESEARCH_DYNAMO_DISSOLUTION			= 9401,		/// 다이너모 연구 - 해체
		SI_P_AN_RESEARCH_DYNAMO_COMPOSITION			= 9402,		/// 다이너모 연구 - 구성
		SI_P_APT_NASOD_ARMOR_MODE					= 9403,		/// 나소드 아머 모드

		SI_P_APT_LIBRARY_OF_LIMITLESS				= 9404,		/// 무한의 도서관
		SI_P_APT_DUST_STORM							= 9405,		/// 미립자 폭풍

	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		SI_P_ALP_BODY_OF_TRANSCENDENCE				= 9406,		/// 초월자의 신체
		SI_P_ALP_POWER_EXCHANGER					= 9407,		/// 출력 교환기
		SI_P_ALP_FANTASY_TRACER						= 9408,		/// 환상 추적자
	#endif //SERV_ADD_LUNATIC_PSYKER


	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		SI_HA_ALP_DOOMS_DAY							= 9600,		/// 둠스 데이
	#endif //SERV_ADD_LUNATIC_PSYKER
#endif //SERV_9TH_NEW_CHARACTER

	}; 

	enum SKILL_ABILITY_TYPE
	{
		SA_INVALID,															
		SA_EFFECTIVE_TIME,					/// 유지 시간												
		SA_EFFECTIVE_COUNT,					/// 유지 횟수
		SA_SKILL_ID,						/// 특정 스킬 지정
		SA_COMBO_ID,						/// 특정 콤보 지정
		SA_EFFECTIVE_RATE,					/// 발동 확률
		SA_ATK_REL,							/// 공격력(물리,마법) 스탯 증가
		SA_ATK_PHYSIC_REL,					/// 물리 공격력 스탯 증가
		SA_ATK_MAGIC_REL,					/// 마법 공격력 스탯 증가
		SA_DEF_PHYSIC_REL,					/// 물리 방어력 스탯 증가
		SA_DEF_MAGIC_REL,					/// 마법 방어력 스탯 증가
		SA_TEAM_ATK_PHYSIC_REL,				/// 물리 공격력 스탯 증가 (팀)
		SA_TEAM_ATK_MAGIC_REL,				/// 마법 공격력 스탯 증가 (팀)
		SA_TEAM_DEF_PHYSIC_REL,				/// 물리 방어력 스탯 증가 (팀)
		SA_TEAM_DEF_MAGIC_REL,				/// 마법 방어력 스탯 증가 (팀)
		SA_DAMAGE_REL,						/// 데미지 배율 변경(패시브 스킬에만 적용 경우)
		SA_DAMAGED_REL,						/// 받는 데미지 변경
		SA_UNFIXED_DEFENSE_REL,				/// 방어도 무시
		SA_SUMMON_ATK_REL,					/// 소환수 공격력 증가
		SA_MAX_HP_ABS,						/// 최대 HP 증가 (수치)
		SA_MAX_HP_REL,						/// 최대 HP 증가 (%)
		SA_MAX_MP_ABS,						/// 최대 MP 증가 (수치)
		SA_MAX_MP_REL,						/// 최대 MP 증가 (%)
		SA_TELEPORT_RANGE_ABS,				/// 캐릭터 이동스킬 거리 증가
		SA_CHARGING_TIME,					/// 모으는 시간
		SA_HP_REL_TO_MP_ABS_1,				/// 마력전환 첫번째 param (HP의 %)
		SA_HP_REL_TO_MP_ABS_2,				/// 마력 전환 두번째 param (MP의 수치)
		SA_MP_CONSUME_REL,					/// 마나 소모 조정
		SA_MP_GAIN_ON_HIT_REL,				/// (타격시)마나 획득량 조정
		SA_SCOPE_ABS,						/// 스킬범위 조정 (반경,수치)
		SA_PROJECTILE_FIRE_COUNT,			/// 발사체 개수 조정
		SA_PROJECTILE_RANGE_REL,			/// 발사체 거리 조정
		SA_PROJECTILE_SIZE,					/// 발사체 사이즈 조정
		SA_PROJECTILE_PIERCING,				/// 발사체 힛트 수 (관통력) 조정
		SA_MOVE_SPEED,						/// 이동 속도 조정
		SA_JUMP_SPEED,						/// 점프 속도 조정
		SA_ATK_PHYSIC_TO_ATK_MAGIC,			/// 물리 공격력의 일정량 마법공격력
		SA_ATK_MAGIC_TO_ATK_PHYSIC,			/// 마법 공격력의 일정량 물리공격력
		SA_READY_TIME_REL,					/// 시즈모드 준비 시간 조정
		SA_FORCE_DOWN_REL,					/// 강제다운 수치 조정 (%)
		SA_FORCE_DOWN_MELEE_REL,			/// 근접공격 강제다운 수치 조정 (%)
		SA_FORCE_DOWN_RANGE_REL,			/// 발사체공격 강제다운 수치 조정 (%)
		SA_FORCE_DOWN_ABS,					/// 스킬타격시 강제다운 수치 조정 (수치)
		SA_MP_REGENERATION_ABS,				/// MP 회복 증가량
		SA_MP_INTAKE_ABS,					/// MP 강탈량
		SA_TARGET_ANI_SPEED,				/// 대상 애니 속도 조정
		SA_TARGET_MOVE_SPEED,				/// 대상 이동 속도 조정
		SA_TARGET_JUMP_SPEED,				/// 대상 점프 속도 조정
		SA_TARGET_ATK_REL,					/// 대상 공격력 조정
		SA_HP_INTAKE_REL_DAMAGE,			/// 준 데미지 대비 HP 흡수량(%)
		SA_ATK_CRITICAL_DAM_ABS,			/// 공격자가 주는 크리티컬 데미지 증가
		SA_ATK_CRITICAL_RATE_ABS,			/// 공격자가 주는 크리티컬 비율
		SA_DEF_CRITICAL_DAM_ABS,			/// 피격자가 받는 크리티컬 데미지 증가
		SA_DEF_CRITICAL_RATE_ABS,			/// 피격자가 받는 크리티컬 비율		// kimhc // 2011-04 에 패치될 청 캐시 스킬
		SA_BACK_SPEED_X,					/// 공격에 의한 x방향으로 밀려나는 값
		SA_ITEM_SPECIAL_ABILITY_REL,		/// HP/MP 아이템 습득시 회복량 증가(%)
		SA_MP_GAIN_GET_HIT_REL,				/// (피격시)마나 획득량 조정 (%) 
		SA_SOUL_GAIN_ON_HIT_REL,			/// (타격시)소울 획득량 조정 (%) 
		SA_SOUL_GAIN_GET_HIT_REL,			/// (피격시)소울 획득량 조정 (%) 
		SA_HP_GAIN_REL_MAX_HP,				/// 캐릭터의 MAX HP의 %에 해당하는 HP 회복
		SA_ED_GAIN_REL,						/// ED 습득량 증가
		SA_ATK_PHYSIC_PLUS_ATK_MAGIC_REL,	/// (물리 공격력 + 마법 공격력)의 %데미지
		SA_EXP_GAIN_REL,					/// EXP 습득량 증가
		SA_DAMAGE_ABSORB_REL_MAX_HP,		/// 캐릭터의 MAX HP의 %에 해당하는 데미지 흡수


		SA_STRONG_MIND_ACTIVE_REL,			/// ??
		SA_STRONG_MIND_SPECIAL_ACTIVE_REL,	/// ??
		SA_MP_RETAKE_ABS,					/// 마나 회복

		SA_RESIST_FIRE_ABS,					/// 불 저항
		SA_RESIST_WATER_ABS,				/// 물 저항
		SA_RESIST_GREEN_ABS,				/// 자연 저항
		SA_RESIST_WIND_ABS,					/// 바람 저항
		SA_RESIST_LIGHT_ABS,				/// 빛 저항
		SA_RESIST_DARK_ABS,					/// 어둠 저항

		SA_ATK_MAGIC_FIRST_REL,				/// 첫번째 마법
		SA_ATK_MAGIC_SECOND_REL,			/// 두번째 마법
		SA_ATK_MAGIC_THIRD_REL,				/// 세번째 마법

		SA_REMOTE_SPEED_ABS,				/// 속도 조절

		SA_DAMAGE_TYPE_CHANGE,				/// 데미지 형태 변경
		SA_DAMAGE_TIME,						/// 데미지 시간

//{{ 김상훈 : 2010.11.12
//#ifdef NEW_SKILL_2010_11
		SA_RECOVER_MP_ABS, 
		SA_TARGET_MANA,
//#endif NEW_SKILL_2010_11
//}} 김상훈 : 2010.11.12		
		SA_TARGET_MANA_REL,					/// 타겟 마나 제거 비율, JHKang
		SA_SIZE_ABS,						/// 쉴드 크기 값, JHKang
		SA_REFLECT_REL,						/// 마법 반사 확률
		SA_EL_DEFENCE_ABS,					/// 모든 속성 저항
		SA_TARGET_HP_REL,					/// 대상 HP 퍼센트 회복
		SA_TARGET_HITRATE_REL,				/// EDT BLIND의 강도
		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
		SA_CHARGE_CANNON_BALL,				/// 캐논볼 차지량
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		
		//{{ kimhc // 2011.1.14 // 청 1차 전직
		SA_FIRE_DEFENCE_ABS,				/// 스탯 창의 불
		SA_WATER_DEFENCE_ABS,				/// 스탯 창의 물
		SA_NATURE_DEFENCE_ABS,				/// 스탯 창의 자연(그린)
		SA_WIND_DEFENCE_ABS,				/// 스탯 창의 바람
		SA_LIGHT_DEFENCE_ABS,				/// 스탯 창의 빛
		SA_DARK_DEFENCE_ABS,				/// 스탯 창의 어둠

		SA_EDT_FIRE_RESIST_REL,				/// 화상 저항 배율
		SA_EDT_FROZEN_RESIST_REL,			/// 빙결 저항 배율
		SA_EDT_ICE_RESIST_REL,				/// 동상 저항 배율
		SA_EDT_POISON_RESIST_REL,			/// 독 저항 배율
		SA_EDT_Y_PRESSED_RESIST_REL,		/// 프레스 저항 배율
		SA_EDT_LEG_WOUND_RESIST_REL,		/// 상처 저항 배율
		SA_EDT_STUN_RESIST_REL,				/// 스턴 저항 배율
		SA_EDT_CURSE_RESIST_REL,			/// 저주 저항 배율

		SA_EXPAND_CB,						/// 캐논볼 확장 수
		SA_ANI_SPEED,						/// 애니메이션 속도 증가
		//}} kimhc // 2011.1.14 // 청 1차 전직

		//{{ kimhc // 2011-04-04 // 2011-04 에 패치될 청 캐시 스킬
		SA_ACCURACY_PERCENT_ABS,			/// 명중률
		//}} kimhc // 2011-04-04 // 2011-04 에 패치될 청 캐시 스킬
		SA_CRITICAL_EVASION_REL,
//#ifdef CHUNG_SECOND_CLASS_CHANGE
		SA_DECREASE_REL,					/// 감소 비율
//#endif
//#ifdef ELSWORD_WAY_OF_SWORD
		SA_WAY_OF_SWORD_TYPE,				/// 엘소드 개편, 검사의 길		
//#endif ELSWORD_WAY_OF_SWORD
//#ifdef ELSWORD_SHEATH_KNIGHT
		SA_ATK_ADD_DAMAGE_FIRST_ABS,
		SA_ATK_ADD_DAMAGE_SECOND_ABS,
		SA_ATK_ADD_DAMAGE_THIRD_ABS,
		SA_ATK_ADD_DAMAGE_FOURTH_ABS,
		SA_ATK_ADD_DAMAGE_FIFTH_ABS,
		SA_TELEPORT_SPEED_REL,
		HP_CONSUME_REL,
		SA_MP_BURN_ABS,
//#endif ELSWORD_SHEATH_KNIGHT
//#ifdef	SERV_TRAPPING_RANGER_TEST	//	페이탈리티 데미지와 확률
		SA_SKILLDAMAGE_MULTIPLE_01,
		SA_SKILLDAMAGE_MULTIPLE_02,
		SA_SKILLDAMAGE_MULTIPLE_03,
		SA_SKILLDAMAGE_MULTIPLE_04,

		SA_SKILLDAMAGE_MULTIPLE_01_RATE,
		SA_SKILLDAMAGE_MULTIPLE_02_RATE,
		SA_SKILLDAMAGE_MULTIPLE_03_RATE,
		SA_SKILLDAMAGE_MULTIPLE_04_RATE,
//#endif	SERV_TRAPPING_RANGER_TEST
//#ifdef SERV_RAVEN_WEAPON_TAKER
		SA_EVASION_REL,
		SA_REDUCE_EDT_TIME_REL,
		SA_OVERHEAT_HP,
//#endif SERV_RAVEN_WEAPON_TAKER
//#ifdef EVE_ELECTRA
		SA_SCOPE_ABS_HYPER,
		SA_EFFECTIVE_TIME_HYPER,
		SA_AIR_MOVE_COUNT,
//#endif EVE_ELECTRA
//#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		SA_NOT_CANNONBALL_REL,
		SA_INSERT_CANNONBALL_REL,
		SA_BE_SHOT_CANNONBALL_REL,
		SA_CANNONBALL_DAMAGE_UP_REL,
		SA_FORCE_DOWN_REL_DAMAGE_RATE_BASE,
		SA_SIEGE_SHELLING_REL,
		SA_EFFECTIVE_DAMAGED_ATTACK_BASE,
//#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN
//#ifdef SERV_CHUNG_TACTICAL_TROOPER
		SA_FIRST_HYPER_EFFECTIVE,
		SA_SECOND_HYPER_EFFECTIVE,
		SA_THIRD_HYPER_EFFECTIVE,
//#endif SERV_CHUNG_TACTICAL_TROOPER
//#ifdef BALANCE_CODE_NEMESIS_20121213
		SA_ATK_CRITICAL_RATE_ATK_BASE,			/// 공격력의 % 만큼 추가되는 크리티컬 비율
//#endif //BALANCE_CODE_NEMESIS_20121213
		// X2SkillTree.h에도 추가해주세요~
	};			
	
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	struct SkillTemplet
	{
		enum SKILL_LEVEL_VALUE
		{
			SLV_MASTER_LEVEL	= 20,
			SLV_MAX_LEVEL		= 30,
		};

		SKILL_TYPE				m_eType;						// 스킬 타입
		int						m_iSkillID;						// 스킬 ID
		int						m_iRequireLearnSkillPoint;		// 스킬 최초습득 시 필요 sp
		int						m_iRequireUpgradeSkillPoint;	// 스킬 강화 시 필요 sp
		bool					m_bBornSealed;					// 봉인이 있는 스킬인지
		bool					m_bGuildSkill;
		
		// <스킬 레벨별 관리 하기 편하게 컨테이너로 관리함>		
		// ****컨테이너가 비어 있지 않다면 무조건 30개(Lv30)가 되어야 한다.***
		std::vector< int >										m_vecRequireCharactorLevel;		// 스킬 단계별 필요 캐릭터 레벨
		std::vector< CXSLStat::Stat >							m_vecStat;						// 스킬 단계별 stat 증가치
		
		std::map< SKILL_ABILITY_TYPE, std::vector< float > >	m_mapSkillAbility;				// 스킬 단계별 SA 효과

		const float GetSkillAbilityValue( IN CXSLSkillTree::SKILL_ABILITY_TYPE eType, IN int iSkillLevel ) const 
		{
			if( true == m_mapSkillAbility.empty() )
				return 0.f;

			std::map< SKILL_ABILITY_TYPE, std::vector< float > >::const_iterator it = m_mapSkillAbility.find( eType );
			if( m_mapSkillAbility.end() == it )
				return 0.f;

			if( it->second.size() != SLV_MAX_LEVEL )
				return 0.f;

			if( iSkillLevel > SLV_MAX_LEVEL )
				iSkillLevel = SLV_MAX_LEVEL;

			return (float)it->second[iSkillLevel-1];
		}

		SkillTemplet()
		{
			m_eType						= ST_NONE;
			m_iSkillID					= 0;
			m_iRequireLearnSkillPoint	= 0;
			m_iRequireUpgradeSkillPoint	= 0;
			m_bBornSealed				= false;
			m_bGuildSkill				= false;
			m_vecRequireCharactorLevel.clear();
			m_vecStat.clear();
			m_mapSkillAbility.clear();
		}
	};

	struct SkillTreeTemplet
	{
		int						m_iUnitClass;
		int						m_iMasterSkillLevel;		// 유저가 배울 수 있는 최고 레벨
		int						m_iTier;					// 스킬트리에서 아이콘 위치, 세로로 몇 번째 row인지 0부터 시작
		int						m_iColumn;						// 스킬트리에서 아이콘 위치, 가로로 몇 번째 column인지 0부터 시작
		int						m_iPrecedingSkill;			// 선행 스킬 ID

		std::vector<int>		m_vecFollowingSkill;		// 스킬 되돌리기 할 때 필요

		SkillTreeTemplet() :
		m_iUnitClass( CXSLUnit::UC_NONE ),
		m_iMasterSkillLevel( 0 ), 
		m_iTier( 0 ),
		m_iColumn( 0 ),
		m_iPrecedingSkill( 0 )
		{
		}
	};
	typedef std::map< int, SkillTreeTemplet >		SkillTreeTempletMap;			// < skill_id, SkillTreeTemplet >
	typedef std::map< int, SkillTreeTempletMap >	UnitSkillTreeTempletMap;		// < unit_class, SkillTreeTempletMap >
	//{{ 2009. 11. 26  최육사	길드스킬
	typedef SkillTreeTempletMap						GuildSkillTreeTempletMap;		// < skill_id, SkillTreeTemplet >
	//}}

	private:
		CXSLSkillTree();

public:
	~CXSLSkillTree();

	bool OpenScriptFile( const char* pFileName );
	void OpenScriptFilePostProcess();

	// 일반스킬
	bool AddSkillTemplet_LUA();
	bool AddSkillTreeTemplet_LUA();

	// 길드스킬
	bool AddGuildSkillTreeTemplet_LUA();

	// 봉인스킬
	bool AddSealSkillInfo_LUA();

	void AddSkillPowerRate_LUA();
	bool AddDefaultSkill_LUA();			/// 직업별 기본 스킬 테이블 파싱 함수

	bool IsExsitSkillID( int iSkillID ) { return ( m_setSkillIDList.find( iSkillID ) != m_setSkillIDList.end() ); }

	const SkillTreeTemplet* GetSkillTreeTemplet( const int iUnitClass, const int iSkillID );

	const SkillTemplet* GetSkillTemplet( const int iSkillID );
	int GetMasterSkillLevel( int iUnitClass, int iSkillID );
	int GetMaxSkillLevel( int iUnitClass, int iSkillID );
	int GetMasterGuildSkillLevel( int iGuildSkillID );
	bool LoadSkillTemplet_RequireCharactorLevel( KLuaManager& luaManager, SkillTemplet& skillTemplet );
	bool LoadSkillTemplet_Stat( KLuaManager& luaManager, SkillTemplet& skillTemplet );
	bool LoadSkillTemplet_SA( KLuaManager& luaManager, SKILL_ABILITY_TYPE eType, SkillTemplet& skillTemplet );


	// 길드스킬
	const SkillTreeTemplet* GetGuildSkillTreeTemplet( const int iGuildSkillID );

	//////////////////////////////////////////////////////////////////////////
	//BEGIN : SERVER 에서만 사용되는 함수(레벨업시 스킬포인트를 주기위한..)
	bool					GetCalcLevelUpIncreaseSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint );	
	bool					GetCalcInitSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint ) const;	
	bool					IsUnitTypeDefaultSkill( IN int iSkillID );
	bool					GetUnitClassDefaultSkill( IN char cUnitClass, OUT int& iSkillID1, OUT int& iSkillID2, OUT int& iSkillID3, OUT int& iSkillID4, OUT int& iSkillID5, OUT int& iSkillID6 );	
	//END
	//////////////////////////////////////////////////////////////////////////


	int TokenizeInt( WCHAR** ppStart, WCHAR* pEnd );

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 8. 4  최육사		봉인스킬
	bool GetUnsealItemInfo( IN int iItemID, IN int iUnitClass, OUT std::vector<int>& vecSkillID );
	bool GetUnsealSkillItemList( IN int iSkillID, OUT std::vector< int >& vecItemList );
	bool IsUnsealItemID( int iItemID );
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2009. 11. 9  최육사	트리플속성
	static bool CheckUnitClassForUnsealSkill( IN char cUnitClass, IN char cUnsealSkillUnitClass );	
	//}}

	static float CalulateIncreaseingRate( float fRate_ );	/// kimhc // 1.05 (5%), 0.95 (-5%) 등의 증가율을 0.05, -0.05 등으로 반환 해주는 함수

	//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
	const SkillTreeTempletMap* GetSkillTreeTemplet( const int iUnitClass );
	bool IsSealSkill( const int iSkillID );

	// 클래스별, 스킬 리스트
	void GetUnitClassSkillList( IN char cUnitClass, OUT std::map< int, SkillTreeTemplet >& mapSkillTreeList );
	bool GetCashSkillItemUnitClassType( IN int iItemID, OUT int& iUnitClassType );

private:

	std::set< int >						m_setSkillIDList;
	std::map<int, SkillTemplet>			m_mapSkillTemplet;

	UnitSkillTreeTempletMap				m_mapUnitSkillTreeTemplet;
	//{{ 2009. 11. 26  최육사	길드스킬
	GuildSkillTreeTempletMap			m_mapGuildSkillTreeTemplet;
	//}}

	//{{ 2009. 8. 4  최육사		봉인스킬정보
	mutable	KncCriticalSection			m_csUnsealSkillItem;
	std::set< int >						m_setUnsealSkillItem;

	std::map< int, int >				m_mapUnsealSkillItem_UnitClassType;		// key : ItemID, value : UnitClassType (1차전직이냐, 2차 전직이냐)

	std::map< int, std::set< int > >	m_mapUnsealSkillItem;	// key : ItemID, value : skillIDList
	std::map< int, std::vector< int > > m_mapUnsealSkill;		// key : SkillID, value : Unseal Item List
	//}}

	std::map< int, std::vector< int > > m_mapDefaultSkill;		// key : unitclass, value : skill list
	std::set< int >						m_setDefaultSkill;

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	struct SkillTemplet
	{
		//{{ 2009. 11. 25  최육사	길드스킬
		bool					m_bGuildSkill;
		//}}
		SKILL_TYPE				m_eType;
		int						m_iSkillID; 
		int						m_iSkillLevel;				// 이 스킬템플릿의 스킬 레벨
		float					m_fSkillCoolTime;
		float					m_fMPConsumption;			// MP 소모량

		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		USHORT					m_usCBConsumption;			/// 캐논볼 소모량
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

		std::wstring			m_Name;
		std::wstring			m_Desc;

		CXSLStat::Stat			m_Stat;						// 버프필살기의 경우 stat 증가치
		bool					m_bBornSealed;				// 봉인이 있는 스킬인지

		//std::wstring			m_wstrTextureName;			// 스킬 아이콘
		//std::wstring			m_wstrTexturePieceName;		// 스킬 아이콘

		std::map< SKILL_ABILITY_TYPE, float > m_mapSkillAbility;

		const float GetSkillAbilityValue( CXSLSkillTree::SKILL_ABILITY_TYPE eType ) const 
		{
			//ASSERT( false == m_mapSkillAbility.empty() );
			if( true == m_mapSkillAbility.empty() )
				return 0.f;

			std::map< SKILL_ABILITY_TYPE, float >::const_iterator it = m_mapSkillAbility.find( eType );
			//ASSERT( m_mapSkillAbility.end() != it );
			if( m_mapSkillAbility.end() == it )
				return 0.f;

			return (float)it->second;
		}



		SkillTemplet() :
		//{{ 2009. 11. 25  최육사	길드스킬
		m_bGuildSkill(false),
			//}}
			m_eType(ST_NONE ),
			m_iSkillID( 0 ),
			m_iSkillLevel( 0 ),
			m_fSkillCoolTime( 0.f ),
			m_fMPConsumption( 0.f ),
			//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			m_usCBConsumption(0),			/// 캐논볼 소모량
#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
			m_Name( L"" ),
			m_Desc( L"" ),
			m_bBornSealed( false )//,
			//m_wstrTextureName( L"" ),
			//m_wstrTexturePieceName( L"" )
		{
		}
	}; // struct SkillTemplet


	struct SkillKey
	{
		int 			m_iSkillID;			// skill id
		int				m_iSkillLevel;		// skill level

		SkillKey() :
		m_iSkillID( 0 ),
		m_iSkillLevel( 0 )
		{
		}

		SkillKey( int iSkillID, int iSkillLevel )
		{
			m_iSkillID = iSkillID;
			m_iSkillLevel = iSkillLevel;
		}

		bool operator < ( const SkillKey& rhs ) const 
		{
			if( (int)m_iSkillID < (int)rhs.m_iSkillID )
				return true;
			else if( (int)m_iSkillID > (int)rhs.m_iSkillID )
				return false;

			if( m_iSkillLevel < rhs.m_iSkillLevel )
				return true;

			return false;
		}
	};
	typedef std::map<SkillKey, SkillTemplet> SkillTempletMap;

	struct SkillTreeTemplet
	{
		int				m_iMaxSkillLevel;
		int				m_iTier;					// 스킬트리에서 아이콘 위치, 세로로 몇 번째 row인지 0부터 시작
		int				m_iIndexInTier;				// 스킬트리에서 아이콘 위치, 가로로 몇 번째 column인지 0부터 시작

		std::vector<int>		m_vecPrecedingSkill;		// 선행 스킬 ID
		std::vector<int>		m_vecFollowingSkill;		// 스킬 되돌리기 할 때 필요

		SkillTreeTemplet() :
		m_iMaxSkillLevel( 0 ), 
		m_iTier( 0 ),
		m_iIndexInTier( 0 )
		{
		}
	};
	typedef std::map< int, SkillTreeTemplet >		SkillTreeTempletMap;			// < skill_id, SkillTreeTemplet >
	typedef std::map< int, SkillTreeTempletMap >	UnitSkillTreeTempletMap;		// < unit_class, SkillTreeTempletMap >
	//{{ 2009. 11. 26  최육사	길드스킬
	typedef SkillTreeTempletMap						GuildSkillTreeTempletMap;		// < skill_id, SkillTreeTemplet >
	//}}

	//{{ 2009. 8. 4  최육사		봉인스킬
	struct KUnsealItemInfo
	{
        int		m_iSkillID;
		char	m_cUnitClass;

		KUnsealItemInfo()		
		{
			m_iSkillID	 = 0;
			m_cUnitClass = CXSLUnit::UC_NONE;
		}
	};

private:
	CXSLSkillTree();

public:
	~CXSLSkillTree();

	bool OpenScriptFile( const char* pFileName );
	void OpenScriptFilePostProcess();

	// 일반스킬
	bool AddSkillTemplet_LUA();
	bool AddSkillTreeTemplet_LUA();

	// 길드스킬
	bool AddGuildSkillTreeTemplet_LUA();

	// 봉인스킬
	bool AddSealSkillInfo_LUA( int iUnsealItemID, int iSkillID, char cUnitClass );

	bool IsExsitSkillID( int iSkillID ) { return ( m_setSkillIDList.find( iSkillID ) != m_setSkillIDList.end() ); }

	const SkillTreeTemplet* GetSkillTreeTemplet( const int iUnitClass, const int iSkillID );

	const SkillTempletMap& GetSkillTempletMap() const { return m_mapSkillTemplet; }
	const SkillTemplet* GetSkillTemplet( const int iSkillID, const int iSkillLevel );
	int GetMaxSkillLevel( int iUnitClass, int iSkillID );
	int GetMaxGuildSkillLevel( int iGuildSkillID );

	// 길드스킬
	const SkillTreeTemplet* GetGuildSkillTreeTemplet( const int iGuildSkillID );

	//////////////////////////////////////////////////////////////////////////
	//BEGIN : SERVER 에서만 사용되는 함수(레벨업시 스킬포인트를 주기위한..)
	bool					GetIncSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint );	
	bool					IsUnitTypeDefaultSkill( IN int iSkillID );
	bool					GetUnitTypeDefaultSkill( IN char cUnitType, OUT int& iSkillID, OUT int& iSkillID2 );	
	//END
	//////////////////////////////////////////////////////////////////////////


	int TokenizeInt( WCHAR** ppStart, WCHAR* pEnd );

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 8. 4  최육사		봉인스킬
	bool GetUnsealItemInfo( IN int iItemID, OUT int& iSkillID, OUT char& cUnitClass );
	bool GetUnsealSkillItemList( IN int iSkillID, OUT std::vector< int >& vecItemList );
	bool IsUnsealItemID( int iItemID );
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2009. 11. 9  최육사	트리플속성
	static bool CheckUnitClassForUnsealSkill( IN char cUnitClass, IN char cUnsealSkillUnitClass );	
	//}}

	static float CalulateIncreaseingRate( float fRate_ );	/// kimhc // 1.05 (5%), 0.95 (-5%) 등의 증가율을 0.05, -0.05 등으로 반환 해주는 함수

	//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
	const SkillTreeTempletMap* GetSkillTreeTemplet( const int iUnitClass );
	bool IsSealSkill( const int iSkillID );
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
	//}}

private:
	
	std::set< int >						m_setSkillIDList;
	SkillTempletMap						m_mapSkillTemplet;

	UnitSkillTreeTempletMap				m_mapUnitSkillTreeTemplet;
	//{{ 2009. 11. 26  최육사	길드스킬
	GuildSkillTreeTempletMap			m_mapGuildSkillTreeTemplet;
	//}}

	//{{ 2009. 8. 4  최육사		봉인스킬정보
	mutable	KncCriticalSection			m_csUnsealSkillItem;
	std::set< int >						m_setUnsealSkillItem;

	std::map< int, KUnsealItemInfo >	m_mapUnsealSkillItem;	// key : ItemID, value : UnsealItemInfo
	std::map< int, std::vector< int > > m_mapUnsealSkill;		// key : SkillID, value : Unseal Item List
	//}}

*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

};

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
DefRefreshSingletonInline( CXSLSkillTree );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
DefSingletonInline( CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2