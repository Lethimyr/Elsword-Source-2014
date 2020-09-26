/**@file : X2BuffFactor.h
   @breif : 버프를 발생 시키는 구성요소를 모아놓은 파일
*/

#pragma once

#define STATIC_CAST_FLOAT_TO_ENUM( type_, arg ) \
	static_cast<type_>( static_cast<int>( arg ) )

enum BUFF_BEHAVIOR_TYPE
{
	BBT_NONE = 0,
	BBT_CHANGE_NOW_HP_PER_SECOND,		/// 초당 HP 증감
   	BBT_CHANGE_NOW_MP_PER_SECOND,		/// 초당 MP 증감
	BBT_REVERSE_LEFT_RIGHT,				/// 특정 간격으로 좌우 반전
	BBT_DASH_IMPOSSIBLE,				/// 대시 불가
	BBT_ATTACK_IMPOSSIBLE,				/// 공격 불가
	BBT_CHANGE_MAX_HP,					/// 최대HP 변경
	BBT_CHANGE_PHYSIC_ATTACK,			/// 물리공격력
	BBT_CHANGE_MAGIC_ATTACK,			/// 마법공격력
	BBT_CHANGE_PHYSIC_DEFENCE,			/// 물리방어력
	BBT_CHANGE_MAGIC_DEFENCE,			/// 마법방어력
	BBT_CHANGE_ANIM_SPEED,				/// 동작속도
	BBT_CHANGE_WALK_RUN_SPEED,			/// 이동속도
	BBT_CHANGE_JUMP_POWER,				/// 점프력
	BBT_CHANGE_ACCURACY,				/// 명중
	BBT_CHANGE_AVOIDANCE,				///	회피
	BBT_CHANGE_CRITICAL_RATE,			/// 크리티컬공격확률
	BBT_CHANGE_CRITICAL_ATTACK,			/// 크리티컬공격력(크리티컬 시 데미지 관련)
	BBT_CHANGE_ANTI_CRITICAL_RATE,		/// 크리티컬방어확률
	BBT_CHANGE_CRITICAL_DEFENCE,		/// 크리티컬방어력(크리티컬 시 데미지 관련)
	BBT_CHANGE_ADDITIONAL_ATTACK,		/// 추가데미지
	BBT_CHANGE_ADDITIONAL_DEFENCE,		/// 데미지감소
   	BBT_CREATE_BUFF_FACTOR,				/// 버프가 끝날때 까지 버프 팩터 생성
	BBT_ABSORB_EFFECT_ATTACK,			/// 이펙트 공격을 흡수하여 MP 회복
   	
	BBT_CHARGE_MP_HIT_MELEE_ATTACK,		/// 근접 타격시 추가될 MP 회복량
	BBT_CHARGE_MP_HIT_RANGE_ATTACK,		/// 원거리 타격시 추가될 MP 회복량
	BBT_CHARGE_MP_HITTED,				/// 피격시 추가될 MP 회복량

	BBT_CHANGE_HYPER_MODE,				/// 각성
	BBT_SUPER_ARMOR,					/// 수퍼아머

	BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK,		/// 근접공격 주는 데미지 변경
	BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK,		/// 원거리(DAMAGE_EFFECT)공격 주는 데미지 변경

	BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER,					///	받는 데미지 변경

	BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK,		/// 근접공격 FORCE_DOWN 수치 변경
	BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK,		/// 원거리(DAMAGE_EFFECT)공격 FORCE_DOWN 수치 변경

	BBT_RESET_SKILL_COOL_TIME,							/// 스킬의 쿨타임을 초기화 시켜주는 행동
	BBT_CHANGE_UNIT_SCALE,								/// 유닛의 크기를 변경하는 행동

	BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE,					/// 속성 저항-불~어둠
	BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE,
	BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE,
	BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE,
	BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE,
	BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE,

	BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE,					/// 모든 속성 저항

	BBT_CUSTOM_FUNCTION,								/// CommonStateStart, CommonFrameMove, CommonStateEnd 에서 처리되야 하는 하드코딩 함수를 수행하게 하는 행동
	BBT_DRAIN_HP_NORMAL_ATTACK,							/// 일반 공격시 HP흡수(블러디 액셀 등에 이용)

	BBT_EFFECT_SET_WITH_DAMAGE,							/// 데미지를 주는 이펙트셋 (버프를 사용한 스테이트의PowerRate와 난입한 유저가 이펙트셋을 실행 시킬때 상태의 PowerRate가 다른 경우가 많으므로 데미지가 있는 EffectSet의 경우 PowerRate를 동기화하는 Behavior 사용)
	BBT_STUN,											/// 기절 스테이트로 고정시키는 템플릿
	BBT_FINISH_OTHER_BUFF_DEBUFF,						/// 다른 버프/디버프를 종료 시키는 행동
	BBT_FROZEN,										/// 현재 스테이트로 고정시키는 템플릿
	BBT_CAN_PASS_UNIT,								/// 버프가 적용되는 동안 유닛의 CanPassUnit 플래그를 true 또는 false 로 설정(true 면 다른 유닛이 나를 통과하게함)
	BBT_ATTACK_ALL_TEAM,							/// 모든 팀을 공격하는 행동
	BBT_NEVER_MOVE,									/// 움직일수없도록하는 행동(피격당하거나 하더라도..)
	BBT_CHANGE_MAX_MP,								/// 최대MP 변경
	BBT_CHANGE_ATTACK_BY_TYPE,						/// 공격 타입에 따른 공격력 증가(스페셜 액티브 스킬, 액티브 스킬, 일반 )
	BBT_CHANGE_ENCHANT_ATTACK_RATE,					/// 소켓 속성 공격 발동 확률 변경
	BBT_EMPTY,										/// 아무 행동도 하지 않음
	BBT_DOUBLE_ATTACK,								/// 더블 어택
	BBT_CHANGE_ATTACK_SPHERE_SCALE,					/// 캐릭터에게 붙은 어택박스의 크기 변경

	BBT_DOWN_IMPOSSIBLE,							/// 하단이동 불가
	BBT_ADD_SKILL_LEVEL,							/// 스킬레벨을 증감 시키는 행동

	BBT_SKILL_ATTACK_IMPOSSIBLE,					/// 스킬 공격 불가
	BBT_ZX_ATTACK_IMPOSSIBLE,						/// ZX 공격 불가

	BBT_REFLECT_MAGIC,								/// 마법 반사

	BBT_CHANGE_CONSUME_MP_RATE,						/// 마나 소비량 변경
// #ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
// Z 공격 불가, X 공격 불가 버프 타입 추가
	BBT_Z_ATTACK_IMPOSSIBLE,						///	Z 공격 불가
	BBT_X_ATTACK_IMPOSSIBLE,						/// x 공격 불가

// 각 속성 별 인챈트 확률 증가
	BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE,			/// 속성 공격 확률 증가 (불)
	BBT_CHANGE_ATTRIBUTE_WATER_ATTACK_RATE,			///  속성 공격 확률 증가 (물)
	BBT_CHANGE_ATTRIBUTE_NATURE_ATTACK_RATE,		/// 속성 공격 확률 증가 (자연)
	BBT_CHANGE_ATTRIBUTE_WIND_ATTACK_RATE,			/// 속성 공격 확률 증가 (바람)
	BBT_CHANGE_ATTRIBUTE_LIGHT_ATTACK_RATE,			/// 속성 공격 확률 증가 (빛)
	BBT_CHANGE_ATTRIBUTE_DARK_ATTACK_RATE,			/// 속성 공격 확률 증가 (어둠)
// #endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

//#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	BBT_CHANGE_HYPER_CHARGE_SPEED,						/// 각성 충전 속도 증가
//#endif //SERV_ADD_LUNATIC_PSYKER

};

enum BUFF_DURATION_TIME_TYPE
{
	BDTT_NONE = 0,
	BDTT_NORMAL_TIME,
	BDTT_RANDOM_TIME,
	BDTT_RELATION_HYPER_MODE_COUNT,			/// 각성구슬개수를 참조한 시간
};

enum BUFF_USE_COUNT_TYPE		/// 버프의 갯수를 배율로 사용하는 타입 정의
{
	BUCT_NOT_USE = 0,				/// 버프나 디버프 개수를 배율로 사용하지 않음
	BUCT_MY_BUFF_COUNT,				/// 자신에게 걸린 버프 개수를 배율로 사용
	BUCT_MY_DEBUFF_COUNT,			/// 자신에게 걸린 디버프 개수를 배율로 사용
	BUCT_ENEMY_BUFF_COUNT,			/// 적에게 걸린 버프 개수를 배율로 사용
	BUCT_ENEMY_DEBUFF_COUNT,		/// 적에게 걸린 디버프 개수를 배율로 사용
};

enum BUFF_FINALIZER_TYPE
{
	BFT_NONE = 0,
	BFT_TIME,			/// 시간에 의한 종료
	BFT_HIT_COUNT,		/// 일정 타격횟수가 되면 종료
	BFT_HITTED_COUNT,	/// 일정 피격횟수가 되면 종료
	BFT_STATE_CHANGE,	/// 스테이트가 변경되는 경우 종료
	BFT_DAMAGE_VALUE,	/// 입력된 수치의 데미지량을 넘어서면 종료
	BFT_PASS_HP,		/// HP가 기준 수치를 통과하면 종료
	BFT_PASS_MP,		/// MP가 기준 수치를 통과하면 종료
	BFT_IMMEDIATE,		/// 적용 즉시 종료(실제로는 1번의 FRAME_MOVE 수행 후 종료)
	BFT_DUNGEON_STAGE_CHANGE,	/// 던전스테이지가 변경되면 종료
	BFT_LIVE_CREATOR,	/// 버프 시전자가 사망하거나 존재하지 않으면 종료
	BFT_FINISH_HYPER,	/// 각성이 종료됐을 때
	BFT_RIDING_ON_OR_NOT,	/// 라이딩 펫을 타거나 내렸을 때
	BFT_X2STATE_CHANGE, /// 게임 스테이트 변경되는 경우 종료

	//BFT_BUFF,			/// 입력된 버프나 디버프가 걸리게 되면 종료
};

enum BUFF_CHANGE_TYPE
{
	BCT_SWAP_VALUE	= 0,	/// 특정 수치로 치환
	BCT_FIX_VALUE,			/// 고정 수치
	BCT_RELATION_VALUE,		/// 연동 수치
	BCT_PERCENT,			/// % 계산
	BCT_END,				/// enum의 끝, enum의 개수
};

enum BUFF_RELATION_TYPE		/// 연동 타입
{
	BRT_MAX_HP,					/// 최대HP 연동
	BRT_PHYSIC_ATTACK,			/// 물리공격력연동
	BRT_MAGIC_ATTACK,			/// 마법공격력연동
	BRT_MIX_ATTACK,				/// 평균공격력연동
	BRT_PHYSIC_DEFENCE,			/// 물리방어력연동
	BRT_MAGIC_DEFENCE,			/// 마법방어력연동
	BRT_MIX_DEFENCE,			/// 평균방어력연동
	BRT_END,					/// enum의 끝, enum의 개수
};

enum BUFF_DISPLAYER_TYPE
{
	BDT_NONE = 0,
	BDT_RENDER_PARAM,					/// CKTDGXRenderer::RenderParam 변경
	BDT_RENDER_PARAM_BY_UNIT_TYPE,		/// 유닛 타입별 CKTDGXRenderer::RenderParam 변경(공통은 UT_NONE 사용)
	BDT_EFFECT_SET,						/// 이펙트셋 사용
	BDT_UNIT_SLASH_TRACE,				/// UnitSlashTrace 사용
	BDT_EFFECT_SET_MELEE_IMPACT_POINT,	/// 근접공격의 타격점에 이펙트셋 사용	( 이펙트 셋 이름, 재생할 사운드, 생성 간격 )
	BDT_EFFECT_SET_HITTED,				/// 타격 당했을때 이펙트셋 보여주기		( 이펙트 셋 이름, 재생할 사운드, 생성 간격 )
	BDT_WEAPON_PARTICLE,				/// 무기에 붙이는 파티클
	BDT_AFTER_IMAGE,					/// AfterImage 실행
	BDT_SOUND,							/// 사운드 출력
//#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
	BDT_WEAPON_AFTER_IMAGE,				/// 무기 AfterImage 실행
//#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE
//#ifdef SERV_NEW_DEFENCE_DUNGEON
	BDT_EFFECT_SET_CREATE_GAP,			/// 일정 간격마다 이펙트셋 발생			( 이펙트 셋 이름, 생성 간격 )
	BDT_EFFECT_SET_NORMAL_ATTACK,		/// 일반 공격  타격시 이펙트셋 생성		( 이펙트 셋 이름, 재생할 사운드, 생성 간격 )
//#endif SERV_NEW_DEFENCE_DUNGEON
//#ifdef HAMEL_SECRET_DUNGEON // 김태환
	BDT_EFFECT_SET_ONLY_MY_CLASS		/// 내 유닛의 클래스에 따라 다른 이펙트셋을 생성
//#endif // HAMEL_SECRET_DUNGEON
};

// #ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
/// 무기 속성 공격 강화의 타입 추가
enum BUFF_ENCHANT_ATTRIBUTE_TYPE
{
	BEAT_ALL = 0,						/// 모든 속성 확률 강화
	BEAT_BLAZE,							/// 불 속성 확률 강화
	BEAT_FROZEN,						/// 불 속성 확률 강화
	BEAT_POSION,						/// 불 속성 확률 강화	
	BEAT_PIERCING,						/// 불 속성 확률 강화
	BEAT_SHOCK,							/// 불 속성 확률 강화
	BEAT_SNATCH,						/// 불 속성 확률 강화
};
// #endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
enum BUFF_TEMPLET_ID
{
	BTI_NONE									= 0,
	BTI_SI_SA_EMK_SWORD_FIRE					= 3,	// 소드 파이어
	BTI_SI_SA_EMK_PHOENIX_TALON					= 4,	// 피닉스 텔런
	BTI_HYPER_MODE								= 5,	// 각성
	BTI_SI_P_ETK_BRUTAL_SLAYER					= 19,	// 파괴의 검사
	BTI_DEBUFF_PRESS							= 25,	// 압착
	BTI_SI_SA_ETK_PHANTOM_SWORD					= 29,	// 팬텀 소드
	BTI_BLOODY_WEAPON							= 30,	// 블러디 액셀
	BTI_DEBUFF_WOUND							= 31,	// 상처(컷텐던)
	BTI_SI_SA_ABM_MAGICAL_MAKEUP				= 32,	// 매지컬 메이크 업
	BTI_BUFF_SI_A_AHM_MEDITATION				= 35,	// 매디테이션
	BTI_DEBUFF_FIRE								= 37,	// 화상
	BTI_DEBUFF_BLIND							= 38,	// 시야 방해(명중률 감소, 샌드 스톰 따위)
	BTI_DEBUFF_POISON							= 39,	// 중독
	BTI_DEBUFF_AGING							= 40,	// 노화
	BTI_DEBUFF_CURSE							= 41,	// 저주
	BTI_SI_SA_CSG_SHARPSHOOTER_SYNDROME			= 45,   // 샤프슈터 신드롬
	BTI_DEBUFF_STIGMA							= 46,	// 낙인의 사격
	BTI_DEBUFF_WEAPON_BREAK						= 48,	// 무기 파괴
	BTI_BUFF_AMPLIFICATION_PLACE				= 50,	// 증폭장
	BTI_BUFF_SPECTRUM_PLACE						= 51,	// 분광장
	BTI_DEBUFF_STUN								= 52,	// 스턴
	BTI_DEBUFF_PANIC							= 53,	// 공황 (판데모니움)
	BTI_BUFF_LAND_DIMOLISHER					= 54,	// 랜드 디몰리션
	BTI_BUFF_UNFIXED_CLIP						= 56,	// 고정 클립 해제
	BTI_DEBUFF_MOVE_JUMP_SLOWDOWN				= 58,	// 이동 둔화 (레그샷 따위)
	BTI_DEBUFF_LEG_SPASM						= 59,	// 다리 부상
	BTI_DEBUFF_COLD								= 60,	// 동상
	BTI_DEBUFF_CONFUSION						= 61,	// 혼란
	BTI_DEBUFF_MARK_OF_COMMANDER				= 62,	// 지휘관의 표식
	BTI_DEBUFF_SIDE_EFFECT_MEDICINE				= 75,	// 대두 ( 물약 부작용 )
	BTI_DEBUFF_SIDE_EFFECT_CREST				= 76,	// 쇠약의 문장 ( 문장 부작용 )
	BTI_DEBUFF_ENTANGLE							= 77,	// 인탱글
	BTI_DEBUFF_ARMOR_BREAK						= 78,	// 방어구 파괴
	BTI_DEBUFF_WATER_HOLD						= 79,	// 물의 결
	BTI_DEBUFF_FROZEN							= 80,	// 빙결
	BTI_DEBUFF_MAGIC_DEFFENCE_DROP				= 81,	// 마력의 사슬
	BTI_DEBUFF_BIND								= 82,	// 동작 정지
	BTI_DEBUFF_DEATH_SENTENCE					= 84,	// 죽음의 선고
	BTI_DEBUFF_MOTION_SLOWDOWN					= 85,	// 동작 둔화(로우킥 따위)
	BTI_DEBUFF_BLIND_SMOKE						= 87,	// 연무 (회피 감소)
	BTI_DEBUFF_ANGER_OF_DRYAD					= 88,	// 드라이어드의 분노
	BTI_CALM_REST								= 89,	// 편안한 휴식
	BTI_DEBUFF_DEADLY_POISON					= 96,	// 맹독 (일단은 다크 클라우드 메모에만 있음)
	BTI_DEBUFF_BLEEDING							= 99,	// 출혈
	BTI_DEBUFF_FLASH_BANG						= 100,	// 섬광탄 (일렉트라 포톤 플레어)
	BTI_BUFF_INDUCTION_PLACE					= 105,	// 반응장
	BTI_BUFF_SI_SA_EBS_ENERGETIC_HEART			= 106,	// 애너제틱 하트
	BTI_DEBUF_SI_A_AHM_MEDITATION				= 107,	// 매디테이션 디버프
	BTI_BUFF_SI_SA_CTT_TACTICAL_FIELD			= 108,	// 택티컬 필드
	BTI_BUFF_THANKS_OF_RESIDENTS				= 123,	// 주민의 감사
	BTI_SI_SA_EEG_ATOMIC_SHIELD					= 124,	// 아토믹 실드
//#ifdef BTI_BUFF_PREMIUM_PC_ROOM
	BTI_BUFF_PREMIUM_PC_ROOM					= 138,	//PC 방 버퍼
//#endif BTI_BUFF_PREMIUM_PC_ROOM
	BTI_BUFF_PET_AURA_SKILL						= 139,	// 팻 격려
//#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
	BTI_BUFF_HENIR_FIRE							= 147,	// 헤니르 버프 - 불의 샘
	BTI_BUFF_HENIR_WATER						= 148,	// 헤니르 버프 - 불의 샘
	BTI_BUFF_HENIR_NATURE						= 149,	// 헤니르 버프 - 땅의 샘
	BTI_BUFF_HENIR_WIND							= 150,	// 헤니르 버프 - 바람의 생
	BTI_BUFF_HENIR_LIGHT						= 151,	// 헤니르 버프 - 빛의 샘
//#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE
//#ifdef ARA_CHARACTER_BASE
	BTI_DEBUFF_SHADOW_JAIL						= 152,	// 그림자 감옥 디버프
	BTI_DEBUFF_SHADOW_KNOT						= 153,	// 그림자 매듭 디버프
	BTI_EMPTY_EXP_BUFF							= 154,  // 특정 캐릭터와 같이 파티하면 경험치 증가
//#endif
	BTI_DEBUFF_SHOCK							= 156,	// 디버프 쇼크
	BTI_ARA_FULL_HYPER_MODE						= 157,	// 아라 3각성
	BTI_DEBUFF_SUPPRESSION						= 161,	// 제압
	BTI_BUFF_SWORD_ENCHANT_ICE					= 163,	// 소드 인첸트 아이스
	BTI_BUFF_SWORD_ENCHANT_FIRE					= 164,	// 소드 인첸트 파이어
	BTI_BUFF_SWORD_ENCHANT_POISON				= 165,	// 소드 인첸트 포이즌
	BTI_DEBUFF_EVENT_BIG_HEAD					= 179,	// 벨더 아카데미 신학기 이벤트 - 몬스터 대두 버프
//#ifdef SERV_NEW_DEFENCE_DUNGEON
	BTI_BUFF_ELIXIR_BIG_HAND_POTION				= 198,	// 영약 : 거대손 물약
	BTI_BUFF_2013_DEFENSE_BUFF					= 202,	// 2013 어둠의 문 참여자 진짜 버프
	BTI_BUFF_2013_DEFENSE_DEBUFF				= 203,	// 2013 어둠의 문 월드 디버프
	BTI_BUFF_2013_DEFENSE_BUFF_EVENT			= 204,	// 이벤트용 2013 어둠의 문 참여자 버프
//#endif SERV_NEW_DEFENCE_DUNGEON

//#ifdef SERV_NEW_UNIT_TRADE_LIMIT	// 해외 전용! 국내에는 없음
	BTI_BUFF_TRADE_BLOCK						= 213,	// 해외 전용 신규 캐릭터 거래 블럭 버프
//#endif // SERV_NEW_UNIT_TRADE_LIMIT

#ifdef RIDING_SYSTEM
	BTI_RIDING_SCORPION_ENERGY_SWORD			= 214,	// 스콜피온 탈 것 스페셜 공격
#endif RIDING_SYSTEM

	BTI_DEBUFF_PANDEMONIUM_FEAR					= 223,	// 판데모니움 - 피어
	BTI_BUFF_SI_SA_CTT_TACTICAL_FIELD_RAID		= 224,  // 택티컬 필드

	BTI_BUFF_CHIVALRY_ATTACK					= 229,	// 엘리시스 패시브 스킬 기사의 숙명 - 물리, 마법 공격력
	BTI_BUFF_CHIVALRY_DEFENCE					= 230,	// 엘리시스 패시브 스킬 기사의 숙명 - 물리, 마법 방어력, 뎀감, 속성 저항

	BTI_BUFF_INDURANCE_POWER					= 238,	// 엘리시스 극기-강(强)
	BTI_BUFF_VICTORIOUS_SWORD					= 239,	// 엘리시스 승리의 검

	BTI_BUFF_GALE_WAY_OF_SWORD					= 240,	// 엘리시스 검의 길 질풍
	BTI_BUFF_ANNIHILATION_WAY_OF_SWORD			= 247,	// 엘리시스 검의 길 섬멸 ( 속성 공격 확률 증가 )

	BTI_DEBUFF_NEVER_MOVE						= 257,	// 아라 나찰 1식 - 흡혼
	BTI_DEBUFF_SELF_PROTECTION_FORTITUDE		= 258,	// 회광반조 사용 불가 안내 디버프
	BTI_BUFF_WATER_SHIELD						= 259,	// 하멜 비던 샤샤, 물의 보호막
	BTI_HEALTH_POINT_IMMEDIATELY_CHANGE_ONCE	= 260,	// HP 즉시 변동, 1번만

//#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	BTI_DEBUFF_STIGMA_OF_FIRE					= 290,	// 엘리시스(블레이징 하트) - 불꽃의 낙인 디버프
	BTI_CHANGE_PHYSIC_DEFENCE_AND_PHYSIC_DAMAGE = 292,	// 엘리시스(그랜드 마스터) - 도발 디버프, 물리 공격력 증가 / 물리 방어력 감소 
	BTI_DEBUFF_UNEXTINGUISHABLE_FIRE			= 293,	// 엘리시스(블레이징 하트) - 꺼지지 않는 불꽃 디버프
	BTI_BUFF_FIRE_BLOSSOMS						= 294,	// 엘리시스(블레이징 하트) - 불꽃 개화
	BTI_BUFF_FLAME_SWORD						= 298,
	BTI_DEBUFF_HIGH_FEVER						= 299,	// 불속성 저항을 감소시키는 디버프.
//#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

//#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	BTI_BUFF_2013_DEFENCE_ENTER_100_PERCENT_BUFF = 300,
//#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	BTI_DEBUFF_RAINCOAT_CAT_STEB1_ATTACK_A	= 301,		// 우비입은 고양이 공격
	BTI_DEBUFF_RAINCOAT_CAT_STEB2_ATTACK_A	= 302,		// 우비입은 고양이 공격
	BTI_DEBUFF_RAINCOAT_CAT_STEB3_ATTACK_A	= 303,		// 우비입은 고양이 공격
	// 할로윈 마녀의 마법 스프 버프
//#ifdef INT_HALLOWEEN_EVENT_BUF
	BTI_BUFF_BIG_HEAD_SOUP				= 307,
	BTI_BUFF_HP_UP_SOUP					= 308,
	BTI_BUFF_MP_UP_SOUP					= 309,
//#endif INT_HALLOWEEN_EVENT_BUF
//#ifdef BALANCE_PATCH_20131107							// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	BTI_BUFF_SA_ESK_WINDMILL					= 318,	// 엘소드(로드 나이트) - 2013년 하반기 밸런싱 작업, 최종 방어력 증가 버프 팩터 추가
//#endif // BALANCE_PATCH_20131107						// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	BTI_FORMATION_MODE							= 320,	// 애드 구성 모드
	BTI_BUFF_SGM_VICTORIOUS_SWORD				= 326,	// 엘리시스(그랜드마스터) - 승리의 검. 이펙트 바뀜.
	BTI_BUFF_LIBRARY_OF_LIMITLESS_MANA_RECOVERY = 327,	// 애드 - 무한의 도서관 '마나회복속도가 X초간 Y씩 증가' 하는 버프
	BTI_NASOD_ARMOR								= 331,	// 애드 - 나소드 아머

//#ifdef ADD_RENA_SYSTEM //김창한
	BTI_BUFF_NATURAL_FORCE						= 333,	// 레나 추가 시스템 - NF 버프
//#endif //ADD_RENA_SYSTEM

//#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	BTI_BUFF_POWER_EXCHANGER_HALF_UP			= 334,	// 애드 - 출력 교환기 마법 공격력, 크리티컬 상승 버프
	BTI_BUFF_POWER_EXCHANGER_HALF_DOWN			= 335,	// 애드 - 출력 교환기 각성 충전 속도 상승 버프
	BTI_DEBUFF_PLASMA_LINK						= 336,	// 애드 - 리버스 리액터 이동력, 속성 방어력 감소 디버프
	BTI_BUFF_QUICKSILVER_FRENZY					= 337,	// 애드 - 퀵실버 프렌지 동작속도, 이동력, 점프력 증가
//#endif //SERV_ADD_LUNATIC_PSYKER

	BTI_BUFF_NAVER								= 339,	// 네이버채널링버프, 물리/마법공격력 3%, 물리/마법방어 3%, MP 최대치 10% 증가
//#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
	BTI_BUFF_THE_ESSENCE_OF_WEAK_HERETIC_POTION	= 1318,
	BTI_BUFF_THE_ESSENCE_OF_HERETIC_POTION		= 1319,
//#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
//#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
	BTI_BUFF_RURIEL_MANA_ENERGIZE_POTION		= 1320,
//#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
};

enum BUFF_FACTOR_ID
{
	BFI_NONE									= 0,
 	BFI_HYPER_MODE								= 1,
	BFI_DEBUFF_SIDE_EFFECT_MEDICINE				= 67,
	BFI_DEBUFF_SIDE_EFFECT_CREST				= 68,
	BFI_DEBUFF_BIND								= 73,
	BFI_DEBUFF_BIND_WITH_MEMO					= 74,
	BFI_DEBUFF_DEATH_SENTENCE					= 75,
	BFI_CALM_REST								= 78,
//#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한	
	BFI_DEBUFF_SI_P_EIS_MIND_OF_FIGHTER			= 80,
//#endif //UPGRADE_SKILL_SYSTEM_2013
	BFI_BUFF_OVER_HEAT							= 84,

#ifdef FIX_REFORM_ENTRY_POINT_7TH						// 김종훈, 귀환자 엠블렘 출력 부분 Emblem Manager 로 변경
	BFI_BUFF_RETURN_OF_HERO						= 114,		// 귀환자 버프
#endif // FIX_REFORM_ENTRY_POINT_7TH					// 김종훈, 귀환자 엠블렘 출력 부분 Emblem Manager 로 변경

	BFI_BUFF_PET_AURA_SKILL_LEVEL_1_DEFAULT		= 125,
	BFI_BUFF_PET_AURA_SKILL_LEVEL_2_DEFAULT		= 130,
	BFI_BUFF_PET_AURA_SKILL_LEVEL_3_DEFAULT		= 135,	
	BFI_BUFF_MECHANIZATION_SPEED_UP				= 141,	// 기계화 셋트 스피드 버프
//#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
	BFI_BUFF_HENIR_FIRE							= 147,	// 헤니르 버프 - 불의 샘
	BFI_BUFF_HENIR_WATER						= 148,	// 헤니르 버프 - 불의 샘
	BFI_BUFF_HENIR_NATURE						= 149,	// 헤니르 버프 - 땅의 샘
	BFI_BUFF_HENIR_WIND							= 150,	// 헤니르 버프 - 바람의 생
	BFI_BUFF_HENIR_LIGHT						= 151,	// 헤니르 버프 - 빛의 샘
//#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

	BFI_EMPTY_EXP_BUFF							= 154,	// 경험치 버프 더미
	BFI_ARA_FULL_HYPER_MODE						= 156,	// 아라 3각성 버프 - 호선경
	BFI_BUFF_BREAKING_MEMO						= 181,	// 아라 메모 : 물 흐르듯이( 이화접목 ) - 회피율 증가

#ifdef RIDING_SYSTEM
	BFI_BUFF_SUPER_AMOR_FOR_RIDING				= 199,	// 탈 것을 타고 있을 시 특별한 상황(DOWN, UP, FLY 공격)을 받을 시 Super Armor 발동
#endif //RIDING_SYSTEM
	BFI_BUFF_TRADE_BLOCK						= 205,	// SERV_NEW_UNIT_TRADE_LIMIT 해외 전용 신규 캐릭터 거래 블록 버프
#ifdef RIDING_SYSTEM
	BFI_RIDING_SCORPION_ENERGY_SWORD			= 206,	// 스콜피온 탈 것 스페셜 공격
#endif //RIDING_SYSTEM
// #ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	BFI_BUFF_OVER_HEAT_ENHANCE					= 217,	// 과열모드 작동! 패시브 버프, 
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	BFI_BUFF_VICTORIOUS_SWORD					= 233,	// 엘리시스 승리의 검 세이버나이트 용.
	BFI_BUFF_GALE_WAY_OF_SWORD					= 237,	// 엘리시스 검의 길 질풍 ( 이속, 동속 증가 )
	BFI_BUFF_GALE_MP_GAIN_INCREASE				= 245,	// 엘리시스 검의 길 질풍 ( MP 회수율 증가 ) 
	BFI_BUFF_ANNIHILATION_WAY_OF_SWORD			= 246,	// 엘리시스 검의 길 섬멸 ( 속성 공격 확률 증가 )
	BFI_BUFF_ANNIHILATION_WAY_OF_SWORD_PVP		= 247,	// 엘리시스 검의 길 섬멸_대전( 속성 공격 확률 증가 )
	BFI_BUFF_ANNIHILATION_SUPER_ARMOR			= 248,	// 엘리시스 검의 길 섬멸 슈아팅 이후 슈퍼아머

	BFI_BUFF_SI_A_ALD_REFLECTION				= 259,	// 아라 반탄공
	BFI_DEBUFF_SELF_PROTECTION_FORTITUDE		= 264,	// 회광반조 사용 불가 안내 디버프
#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
	BFI_DEBUFF_TELEPORT_CONSUME_MP				= 290,	// 아이샤 텔레포트 디버프
#endif //FIX_SKILL_BALANCE_AISHA_LENA
//#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	BFI_DEBUFF_STIGMA_OF_FIRE					= 291,	// 엘리시스(블레이징 하트) - 불꽃의 낙인 디버프
	BFI_DEBUFF_JUDGEMENT_FIRE					= 292,	// 엘리시스(블레이징 하트) - 불꽃의 낙인 폭발 디버프
	BFI_BUFF_FIRE_BLOSSOMS						= 295,	// 엘리시스(블레이징 하트) - 불꽃 개화
	BFI_DEBUFF_HIGH_FEVER						= 300,	// 불속성 저항을 감소시키는 디버프.
	BFI_DEBUFF_FLAME_SWORD						= 301,	// 엘리시스(블레이징 하트) - 홍련의 검 디버프(화상효과)
// #ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	BFI_BUFF_2013_DEFENCE_ENTER_100_PERCENT_BUFF = 302,
// #endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	BFI_DEBUFF_JUDGEMENT_FIRE2					= 307,	// 엘리시스(블레이징 하트) - 불꽃의 낙인 폭발 디버프2
//#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	BFI_FORMATION_MODE							= 315,	// 애드 구성 모드
//#ifdef ADD_MEMO_1ST_CLASS //김창한
	BFI_BUFF_SI_A_ALD_REFLECTION_MEMO			= 334,	// 아라(소마) - 반탄공(메모)
	BFI_BUFF_SWORD_ENCHANT_CHARGE_MP			= 338,	// 엘소드(매직나이트) - 소드 인챈트 (메모)
//#endif //ADD_MEMO_1ST_CLASS
	BFI_BUFF_SGM_VICTORIOUS_SWORD				= 345,	// 엘리시스 승리의 검 그랜드 마스터 용.
	BFI_BUFF_LIBRARY_OF_LIMITLESS_MANA_RECOVERY	= 346,	// 애드 - 무한의 도서관
	BFI_DEBUFF_MIND_BREAK						= 347,	// 애드 - 정신 붕괴
	BFI_DEBUFF_EMP_SHOCK_NEAR					= 348,	// 애드 - EMP 쇼크 직접 피격
	BFI_DEBUFF_EMP_SHOCK_FAR					= 349,	// 애드 - EMP 쇼크 간접 피격
	BFI_NASOD_ARMOR 							= 353,	// 애드 - 나소드 아머

//#ifdef ADD_RENA_SYSTEM //김창한
	BFI_BUFF_NATURAL_FORCE						= 355,	// 레나 추가 시스템 - NF 버프
//#endif //ADD_RENA_SYSTEM

//#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	BFI_BUFF_POWER_EXCHANGER_HALF_UP			= 356,	// 애드 - 출력 교환기 마법 공격력, 크리티컬 상승 버프
	BFI_BUFF_POWER_EXCHANGER_HALF_DOWN			= 357,	// 애드 - 출력 교환기 각성 충전 속도 상승 버프
	BFI_DEBUFF_PLASMA_LINK						= 358,	// 애드 - 리버스 리액터 이동력, 속성 방어력 감소 디버프
	BFI_BUFF_QUICKSILVER_FRENZY					= 359,	// 애드 - 퀵실버 프렌지 동작속도, 이동력, 점프력 증가
//#endif //SERV_ADD_LUNATIC_PSYKER

	BFI_BUFF_NAVER								= 361,	// 네이버채널링버프, 물리/마법공격력 3%, 물리/마법방어 3%, MP 최대치 10% 증가
// #ifdef SERV_BONUS_BUFF_SYSTEM 이넘 겹친드아아아아
//	BFI_BUFF_2YEARS_EVENT_CN_DAMAGE_UP				= 307,	
//	BFI_BUFF_2YEARS_EVENT_CN_MOVE_JUMP_SPEED_UP 	= 308,
//	BFI_BUFF_2YEARS_EVENT_CN_ACCURACY_AVOID_UP		= 309,
//	BFI_BUFF_2YEARS_EVENT_CN_CRI_SPEED_UP			= 310,
//	BFI_BUFF_2YEARS_EVENT_CN_DAMAGE_INC_DEC_UP		= 311,
//	BFI_BUFF_2YEARS_EVENT_CN_ALL_IN_ONE				= 312,
// #endif //SERV_BONUS_BUFF_SYSTEM	
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BUFF_FACTOR
{
public:
	BUFF_FACTOR() : m_uidGameUnit( 0 ), m_bIgnoreRegistForRate( true )
#ifdef ADD_LIVE_CREATOR_FINALIZER
		, m_bIsUserUID( true )
#endif ADD_LIVE_CREATOR_FINALIZER
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		m_vecRate.clear();
#else //UPGRADE_SKILL_SYSTEM_2013
		m_fRate = 0.f;
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
		
	BUFF_FACTOR( const BUFF_FACTOR& rhs_ ) : m_BuffIdentity( rhs_.m_BuffIdentity ),
		m_uidGameUnit( rhs_.m_uidGameUnit ), m_bIgnoreRegistForRate( rhs_.m_bIgnoreRegistForRate )
#ifdef ADD_LIVE_CREATOR_FINALIZER
		, m_bIsUserUID( rhs_.m_bIsUserUID )
#endif ADD_LIVE_CREATOR_FINALIZER
	{
		m_vecBuffBehaviorFactors = rhs_.m_vecBuffBehaviorFactors;
		m_vecBuffFinalizerFactors = rhs_.m_vecBuffFinalizerFactors;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		m_vecRate = rhs_.m_vecRate;
#else //UPGRADE_SKILL_SYSTEM_2013
		 m_fRate = rhs_.m_fRate;
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	BUFF_FACTOR( const BUFF_FACTOR& rhs_, IN const UINT uiLevel_ ) : m_BuffIdentity( rhs_.m_BuffIdentity ),
		m_uidGameUnit( rhs_.m_uidGameUnit ), m_bIgnoreRegistForRate( rhs_.m_bIgnoreRegistForRate )
#ifdef ADD_LIVE_CREATOR_FINALIZER
		, m_bIsUserUID( rhs_.m_bIsUserUID )
#endif ADD_LIVE_CREATOR_FINALIZER
	{
		if ( uiLevel_ >= 1 )
		{
			m_vecRate.push_back( rhs_.m_vecRate[uiLevel_] );
			GetBehaviorFactorToLevel( rhs_.m_vecBuffBehaviorFactors, m_vecBuffBehaviorFactors, uiLevel_ );
			GetFinalizerFactorToLevel( rhs_.m_vecBuffFinalizerFactors, m_vecBuffFinalizerFactors, uiLevel_ );
		}
		else
		{
			m_vecRate.push_back( rhs_.m_vecRate[0] );
			GetBehaviorFactorToLevel( rhs_.m_vecBuffBehaviorFactors, m_vecBuffBehaviorFactors, 1 );
			GetFinalizerFactorToLevel( rhs_.m_vecBuffFinalizerFactors, m_vecBuffFinalizerFactors, 1 );
		}
	}

	void GetBehaviorFactorToLevel( IN const vector<KBuffBehaviorFactor>& vecInBehaviorFactorList ,
		OUT vector<KBuffBehaviorFactor>& vecOutBehaviorFactorList, IN const UINT uiLevel_ );

	void GetFinalizerFactorToLevel( IN const vector<KBuffFinalizerFactor>& vecInFinalizerFactorList ,
		OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const UINT uiLevel_ );

	void PushFinalizerTimeToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerHitToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerHittedToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerDamageValueToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerHpToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerMpToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

public:
	vector<KBuffBehaviorFactor>		m_vecBuffBehaviorFactors;
	vector<KBuffFinalizerFactor>	m_vecBuffFinalizerFactors;
	KBuffIdentity					m_BuffIdentity;				/// 이곳의 KBuffIdentity는 이 BUFF_FACTOR가 발생 시키는 버프가 무엇인지를 구분할 수 있게함(Templet으로부터 정보를 얻어와서 다른 유저들에게 알려줄때 사용)
	UidType							m_uidGameUnit;
#ifdef ADD_LIVE_CREATOR_FINALIZER
	bool							m_bIsUserUID;				/// 유저 유닛의 UID인지 여부( 시전사 사망시 소멸 시키는 파이널라이저를 위한 변수 )
#endif ADD_LIVE_CREATOR_FINALIZER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	vector<float>	m_vecRate;					/// 해당 요인에 의해 버프 또는 디버프가 걸릴 확률
#else //UPGRADE_SKILL_SYSTEM_2013
	float							m_fRate;					/// 해당 요인에 의해 버프 또는 디버프가 걸릴 확률
#endif //UPGRADE_SKILL_SYSTEM_2013
	bool							m_bIgnoreRegistForRate;		/// 위의 확률이 저항에 의해서 변경되는지 여부(true 인경우 저항이 있으면 걸릴 확률이 감소함)
	/// 중첩이 필요할듯...
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2GameUnit;
class CX2BuffFactor;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BuffFactor> CX2BuffFactorPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BuffFactor> CX2BuffFactorPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

/** @class : CX2BuffFactor
	@brief : 버프를 발생시키는 정보를 가지고 있는 클래스
	@date : 2012/7/16/
*/
class CX2BuffFactor
{
public:
	static CX2BuffFactorPtr CreateBuffFactorPtr() { return CX2BuffFactorPtr( new CX2BuffFactor ); }

	~CX2BuffFactor() { SAFE_DELETE( m_pDataBuffFactor ); }

	CX2BuffFactor( const CX2BuffFactor& rhs_ ) 
		: m_pDataBuffFactor( new BUFF_FACTOR( *(rhs_.m_pDataBuffFactor) ) ), m_DataBuffIdentity( rhs_.m_DataBuffIdentity )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	CX2BuffFactor( const CX2BuffFactor& rhs_, IN const UINT uiLevel_ ) 
		: m_pDataBuffFactor( new BUFF_FACTOR( *(rhs_.m_pDataBuffFactor), uiLevel_ ) ), m_DataBuffIdentity( rhs_.m_DataBuffIdentity )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
#endif //UPGRADE_SKILL_SYSTEM_2013

private:
	CX2BuffFactor() : m_pDataBuffFactor( new BUFF_FACTOR )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {}
	
public:
	CX2BuffFactorPtr	GetClonePtr() const { return CX2BuffFactorPtr( new CX2BuffFactor(*this) ); }
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	CX2BuffFactorPtr	GetClonePtr( IN const UINT uiLevel_ ) const { return CX2BuffFactorPtr( new CX2BuffFactor(*this, uiLevel_) ); }
#endif //UPGRADE_SKILL_SYSTEM_2013

	bool GetBehaviorFactor( const BUFF_BEHAVIOR_TYPE eType_, OUT const KBuffBehaviorFactor** ppFactor_ ) const;
	bool GetFinalizerFactor( const BUFF_FINALIZER_TYPE eType_, OUT const KBuffFinalizerFactor** ppFactor_ ) const;

	UidType GetUidGameUnit() const { return m_pDataBuffFactor->m_uidGameUnit; }
	void SetUidGameUnit(UidType val) { m_pDataBuffFactor->m_uidGameUnit = val; }

#ifdef ADD_LIVE_CREATOR_FINALIZER
	bool GetIsUserUID() const {  return m_pDataBuffFactor->m_bIsUserUID; }
	void SetIsUserUID( bool val ) { m_pDataBuffFactor->m_bIsUserUID = val; }
#endif ADD_LIVE_CREATOR_FINALIZER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	float GetRate( IN const UINT iLevel_ ) const
	{
		if ( true == m_pDataBuffFactor->m_vecRate.empty() )
			DISPLAY_ERROR( L"Rate is Empty!" );

		if ( m_pDataBuffFactor->m_vecRate.size() >= iLevel_ )
			return m_pDataBuffFactor->m_vecRate.at( iLevel_ - 1 );
		else
			return m_pDataBuffFactor->m_vecRate.at( 0 );
	}

	void SetRate( float val, IN const UINT iLevel_ )
	{
		if ( true == m_pDataBuffFactor->m_vecRate.empty() )
			DISPLAY_ERROR( L"Rate is Empty!" );

		if ( m_pDataBuffFactor->m_vecRate.size() >= iLevel_ )
			m_pDataBuffFactor->m_vecRate.at( iLevel_ - 1 ) = val;
		else
			m_pDataBuffFactor->m_vecRate.at( 0 ) = val;
	}

	void ParsingTableForWhile( IN KLuaManager& luaManager_, IN const char* pszTableName_, OUT vector<float>& vecValues_, IN bool bUpdateCount_ = true );
	void ParsingTableMinMaxForWhile( IN KLuaManager& luaManager_, IN const char* pszTableName_, OUT vector<float>& vecValues_, IN bool bUpdateCount_ = true );
#else //UPGRADE_SKILL_SYSTEM_2013
	float GetRate() const { return m_pDataBuffFactor->m_fRate; }
	void SetRate(float val) { m_pDataBuffFactor->m_fRate = val; }
#endif //UPGRADE_SKILL_SYSTEM_2013

	bool GetIgnoreRegistForRate() const { return m_pDataBuffFactor->m_bIgnoreRegistForRate; }
	void SetIgnoreRegistForRate(bool val) { m_pDataBuffFactor->m_bIgnoreRegistForRate = val; }

	BUFF_TEMPLET_ID GetBuffTempletID() const { return static_cast<BUFF_TEMPLET_ID>( m_pDataBuffFactor->m_BuffIdentity.m_eBuffTempletID ); }
	void SetBuffTempletID(BUFF_TEMPLET_ID val) { m_pDataBuffFactor->m_BuffIdentity.m_eBuffTempletID = val; }

	void SetBuffIdentity( const KBuffIdentity& BuffIdentity_ ) { m_DataBuffIdentity = BuffIdentity_; }
	const KBuffIdentity& GetBuffIdentity() const { return m_DataBuffIdentity; }

	/// 연동수치의 경우 이 함수를 이용하여 값을 셋팅함
	void UpdateValueByRelationType( const BUFF_RELATION_TYPE eRelationType_, OUT float& fValue_, CX2GameUnit* pGameUnit_ );

	////////////////////////////////////////////////////////////////////////////////////////////////
	// PushFactor
	////////////////////////////////////////////////////////////////////////////////////////////////
	void PushBehaviorFactor( const KBuffBehaviorFactor& factor_ ) { m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( factor_ ); }
	void PushFinalizerFactor( const KBuffFinalizerFactor& factor_ ) { m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( factor_ ); }

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Parsing
	////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	bool ParsingBuffFactor( KLuaManager& luaManager_, bool bIsSkill = true );
	bool ParsingBehavior( KLuaManager& luaManager_, bool bIsSkill = true );
	bool ParsingCombinationBehavior( KLuaManager& luaManager_, bool bIsSkill = true );
	bool ParsingFinalizer( KLuaManager& luaManager_, bool bIsSkill = true );
	bool ParsingCombinationFinalizer( KLuaManager& luaManager_, bool bIsSkill = true );
#else //UPGRADE_SKILL_SYSTEM_2013
	bool ParsingBuffFactor( KLuaManager& luaManager_ );
	bool ParsingBehavior( KLuaManager& luaManager_ );
	bool ParsingCombinationBehavior( KLuaManager& luaManager_ );
	bool ParsingFinalizer( KLuaManager& luaManager_ );
	bool ParsingCombinationFinalizer( KLuaManager& luaManager_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

	////////////////////////////////////////////////////////////////////////////////////////////////
	/// ApplyExternalFactor
	////////////////////////////////////////////////////////////////////////////////////////////////
	//void ApplyExternalFactorToBehavior( const BUFF_BEHAVIOR_TYPE eType_, const float fValue_ );
	void ApplyExternalFactorToFinalizer( const BUFF_FINALIZER_TYPE eType_, const float fValue_ );

	////////////////////////////////////////////////////////////////////////////////////////////////
	// SetGameUnit
	////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ADD_LIVE_CREATOR_FINALIZER
	void SetGameUnitBuffFactor( CX2GameUnit* pGameUnit_, UidType iCreatorUID = -1, bool bIsUserUID = true  );
#else  ADD_LIVE_CREATOR_FINALIZER
	void SetGameUnitBuffFactor( CX2GameUnit* pGameUnit_ );
#endif ADD_LIVE_CREATOR_FINALIZER
	void SetGameUnitBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );
	void SetGameUnitFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );
	

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Behavior
	////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	bool ParsingChangeNowHpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
	void SetGameUnitChangeNowHpPerSecondBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingChangeNowMpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingChangeStatBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
	void SetGameUnitChangeStatBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingCreateBuffFactorBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingAbsorbEffectAttackBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true  );

	bool ParsingChargeMpHitBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
	bool ParsingChargeMpHittedBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingGameUnitChangeHyperModeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitChangeHyperModeBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingEmptyBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingCustomBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingEffectSetWithDamageBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitEffectSetWithDamageBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingChangeAttackByTypeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingChangeEnchantAttackRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingDoubleAttackByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingAddSkillLevelByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingResetSkillCoolTimeByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	bool ParsingReflectMagicBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
	bool ParsingChangConsumeMpRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
#endif //FIX_SKILL_BALANCE_AISHA_LENA

#else //UPGRADE_SKILL_SYSTEM_2013
	bool ParsingChangeNowHpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitChangeNowHpPerSecondBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingChangeNowMpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingChangeStatBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitChangeStatBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingCreateBuffFactorBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingAbsorbEffectAttackBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingChargeMpHitBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingChargeMpHittedBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingGameUnitChangeHyperModeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitChangeHyperModeBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingEmptyBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingCustomBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingEffectSetWithDamageBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitEffectSetWithDamageBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingChangeAttackByTypeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingChangeEnchantAttackRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingDoubleAttackByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingAddSkillLevelByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	bool ParsingResetSkillCoolTimeByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
#endif // UPGRADE_SKILL_SYSTEM_2013
#endif //UPGRADE_SKILL_SYSTEM_2013

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Finalizer
	////////////////////////////////////////////////////////////////////////////////////////////////
	/// 시간에 의한 종료 (고정, 랜덤)
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	bool ParsingTimeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );
	void SetGameUnitTimeFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );
	void ApplyExternalFactorToTimeFinalizer( KBuffFinalizerFactor& factor_, const float fValue_ );

	/// 타격 카운트에 의한 종료(버프, 디버프 배율 사용 유무)
	bool ParsingHitCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eBehaviorType_, bool bIsSkill = true );
	void SetGameUnitHitCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// 피격 카운트에 의한 종료(버프, 디버프 배율 사용 유무)
	bool ParsingHittedCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );
	void SetGameUnitHittedCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );
	
	/// 일정 데미지를 받으면 종료
	bool ParsingDamageValueFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );
	void SetGameUnitDamageValueFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// 기준 HP를 상향 또는 하향 돌파 하면 종료
	bool ParsingPassHpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );

	/// 기준 MP를 상향 또는 하향 돌파 하면 종료 (HP와 종료 동작이 비슷하지만 따로 구현 한 이유는 두개를 같이 사용할 수도 있기 때문)
	bool ParsingPassMpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );

	bool ParsingEmptyFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
#else //UPGRADE_SKILL_SYSTEM_2013
	bool ParsingTimeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
	void SetGameUnitTimeFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );
	void ApplyExternalFactorToTimeFinalizer( KBuffFinalizerFactor& factor_, const float fValue_ );

	/// 타격 카운트에 의한 종료(버프, 디버프 배율 사용 유무)
	bool ParsingHitCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eBehaviorType_ );
	void SetGameUnitHitCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// 피격 카운트에 의한 종료(버프, 디버프 배율 사용 유무)
	bool ParsingHittedCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
	void SetGameUnitHittedCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// 스테이트가 변경되면 종료
	bool ParsingStateChangeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );

	/// 일정 데미지를 받으면 종료
	bool ParsingDamageValueFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
	void SetGameUnitDamageValueFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// 기준 HP를 상향 또는 하향 돌파 하면 종료
	bool ParsingPassHpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_);

	/// 기준 MP를 상향 또는 하향 돌파 하면 종료 (HP와 종료 동작이 비슷하지만 따로 구현 한 이유는 두개를 같이 사용할 수도 있기 때문)
	bool ParsingPassMpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );

	bool ParsingEmptyFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    CX2BuffFactor& operator = ( const CX2BuffFactor& );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned            m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	BUFF_FACTOR*		m_pDataBuffFactor;		/// 포인터로 할 것인가...?
	KBuffIdentity		m_DataBuffIdentity;		/// 어떤 버프로 부터 발생한 BuffFactor인지를 알 수 있게 함

};


IMPLEMENT_INTRUSIVE_PTR( CX2BuffFactor );