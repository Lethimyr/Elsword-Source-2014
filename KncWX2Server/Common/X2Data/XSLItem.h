#pragma once

#include "XSLUnit.h"

#define MAX_SPECIAL_ABILITY_NUM		 3
#define MAX_ENCHANT_CARD_NUM		10

class CXSLItem
{
	//{{ dmlee 2008.11.29 
	public: 
		static const int		MAX_MODEL_COUNT_A_ITEM = 6;
	//}} dmlee 2008.11.29 

	public:
		enum ITEM_TYPE
		{
			IT_NONE = 0,		//에러
			IT_WEAPON,			//무기
			IT_DEFENCE,			//방어구
			IT_ACCESSORY,		//액세서리
			IT_SKILL,			//스킬
			IT_QICK_SLOT,		//퀵슬롯 아이템(소비성)
			IT_MATERIAL,		//재료
			IT_SPECIAL,			//특수			
			IT_QUEST,			//퀘스트
			IT_OUTLAY,			//순간소비성
			IT_ETC,				//기타
			//{{ 2010. 03. 23  최육사	기술의 노트
			IT_SKILL_MEMO,		//기술의노트
			//}}
			//#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			IT_PET,				//펫
			IT_RIDING,			//탈것
			//#endif //SERV_UPGRADE_TRADE_SYSTEM
		};
		
		enum ITEM_GRADE
		{
			IG_NONE = 0,
			IG_UNIQUE,
			IG_ELITE,
			IG_RARE,
			IG_NORMAL,
			IG_LOW,
		};

		enum PERIOD_TYPE
		{
			PT_INFINITY = 0,	//무제한
			PT_ENDURANCE,		//내구도
			PT_QUANTITY,		//수량성
		};

		//{{ 2009. 8. 26  최육사	밀봉 아이템
		enum ITEM_STATE
		{
			IS_NORMAL = 0,
			IS_SEALED,
		};
		//}}

		enum SHOP_PRICE_TYPE
		{
			SPT_NONE = 0,		//팔기 불능
			SPT_CASH,			//캐쉬
			SPT_GP,				//GP
		};

		enum USE_CONDITION
		{
			UC_NONE = 0,		// 아무도 사용할 수 없음
			UC_ANYONE,			// 아무나 사용할 수 있음
			UC_ONE_UNIT,		// 지정된 unit_type 만
			UC_ONE_CLASS,		// 지정된 unit_class 만
		};

		enum USE_TYPE
		{
			UT_NONE = 0,		// 장착하는 장비가 아님.
			UT_SKIN,			// 장착하는 장비, 스킨 애니메이션
			UT_ATTACH_ANIM,		// 장착하는 장비, 어태치 애니메이션
			UT_ATTACH_NORMAL,	// 장착하는 장비, 어태치 메시
		};

		enum SPECIAL_SKILL_TYPE
		{
			SST_THROW_WOODEN_PIECE = 1,
			SST_MAGIC_SPEAR,
			SST_ICE_CRYSTAL,
			SST_FIRE_CRYSTAL,
			SST_MAGIC_BULLET,
			SST_POISON_POCKET,
		};

		enum SPECIAL_SKILL_MOTION_TYPE
		{
			SSMT_THROW = 1,		//던지는 형식
			SSMT_INSTALL,		//설치하는 형식
		};

		enum SUPPLEMENT_ITEM_TYPE
		{
			SIT_PLUS_SUCCESS_RATE = 0,
			SIT_NOT_BROKEN,
		};

		//{{ 2010. 07. 01  최육사	내구도 개편
		enum ENDURANCE
		{
			EN_MAX_ENDURANCE = 10000,
		};
		//}}

        enum SPECIAL_ABILITY_TYPE
        {
            SAT_NONE = 0,                    /// 없음
            SAT_HP_UP,                        /// HP증가
            SAT_MP_UP,                        /// MP증가
            SAT_REMOVE_CURSE,                /// 저주제거
            SAT_REMOVE_SLOW,                /// 슬로우제거
            SAT_REMOVE_FIRE,                /// 화염제거
            SAT_REMOVE_POISON,                /// 독제거
            SAT_RECIPE,                        /// 레시피 아이템
            SAT_SUPPLEMENT_ITEM,            /// 보충제 아이템
            SAT_HYPER_MODE,                    /// 각성
            SAT_SOUL_GAGE_UP,                /// 각성게이지 상승
            SAT_SHOW_OPPONENT_MP,            /// 상대팀 MP 보기
            SAT_UP_MP_AT_ATTACK_OR_DAMAGE,    /// 때리거나 맞을때 MP 상승
            SAT_HP_PERCENT_UP,                /// HP 퍼센트 증가
            SAT_TEAM_HP_UP,                    /// 팀원들 전부 HP % 증가
            SAT_TEAM_MP_UP,                    /// 팀원들 전부 MP % 증가
            SAT_PEPPER_RUN,                    /// 고추먹고 맴맴
            SAT_TRANSFORM_UNIT_SCALE,        /// 유닛 스케일 조정
            SAT_SPECIAL_SKILL,                /// 소비성 아이템을 사용함에 의한 특수 기술
            SAT_MP_PERCENT_UP,                /// MP 퍼센트 증가

            //#ifdef DUNGEON_ITEM
            SAT_POWER_RATE_UP,                /// 공격력 증가 (%)
            SAT_MOVE_SPEED_UP,                /// 이동 속도 증가 (%)
            SAT_JUMP_SPEED_UP,                /// 점프 속도 증가 (%)
            SAT_MP_REGENERATION,            /// 초당 mp 회복
            SAT_USE_HYPER,                    /// 각성구슬 없이 강제 각성            
            SAT_PHYSIC_ATTACK_UP,            /// 물리 공격력 증가 (%)
            SAT_MAGIC_ATTACK_UP,            /// 마법 공격력 증가 (%)
            SAT_PHYSIC_DEFENCE_UP,            /// 물리 방어력 증가 (%)
            SAT_MAGIC_DEFENCE_UP,            /// 마법 방어력 증가 (%)
            SAT_SUPERARMOR,                    /// 슈퍼아머    
            SAT_SUMMON_SPIRIT,                /// 정령소환
            //#endif //DUNGEON_ITEM
            SAT_REMOVE_FROZEN,                // 냉기제거
            SAT_TRANSFORM_MONSTER,            // 몬스터 변신            
            SAT_WAY_OF_SWORD_GAUGE_UP,        // 검의길 게이지 증가                
            //#ifdef SUMMON_MONSTER_CARD_SYSTEM
            SAT_SUMMON_MONSTER,                // 몬스터 소환 기능
            //#endif //SUMMON_MONSTER_CARD_SYSTEM
            SAT_FORCE_CRITICAL_MAX,            // 다음 타격 크리 100%
            SAT_ENABLE_ATTACK_MONSTER,         // 해외 할로윈 이벤트
            //#ifdef ARA_FORCE_POWER_ITEM
            SAT_ARA_FORCE_POWER_PERCENT_UP,    // 해외 아라 기력 증가 (%)
            //#endif ARA_FORCE_POWER_ITEM
			//#ifdef RETURN_TO_BASE_AT_PVE
			SAT_RETURN_TO_BASE_AT_PVE,			// PVE 귀환석
			//#endif // RETURN_TO_BASE_AT_PVE
			//#ifdef RIDINGPET_STAMINA_ITEM
			SAT_RIDINGPET_STAMINA_PERCENT_UP,    // 해외 라이딩펫 스테미너 증가 (%)
			//#endif RIDINGPET_STAMINA_ITEM
            SAT_END,
        };

		//{{ 2008. 4. 22  최육사  캐쉬 아이템
		enum CASH_ITEM
		{
			CI_RESURRECTION_STONE 			= 127030, // 부활석
			CI_EXPAND_INVENTORY	  			= 200750, // 소지품 확장
			CI_EXPAND_SKILL_SLOT  			= 200880, // 스킬슬롯B확장 기간제
			CI_EXPAND_SKILL_SLOT_PERMANENT  = 200885, // 스킬슬롯B확장 영구아이템
			CI_EXPAND_SKILL_SLOT_ARA		= 261630, // 스킬슬롯B확장 아라 전용 판매 캐쉬템
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// 적용날짜: 2013-07-11
			CI_EXPAND_SKILL_SLOT_ELESIS		= 261661, // 스킬슬롯B확장 엘리시스 전용 판매 캐쉬템
#endif	// SERV_CASH_ITEM_FOR_ELESIS

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
#ifdef SERV_CASH_SKILL_POINT_TW
			CI_CASH_SKILL_POINT_30_7		= 60007810,	// 30SP 7일권
			CI_CASH_SKILL_POINT_60_7		= 60007811,	// 60SP 7일권
#endif SERV_CASH_SKILL_POINT_TW
			CI_CASH_SKILL_POINT_30_15		= 271076,	// 30SP 15일권
			CI_CASH_SKILL_POINT_60_15		= 271077,	// 60SP 15일권
			CI_CASH_SKILL_POINT_30_30		= 271078,	// 30SP 30일권
			CI_CASH_SKILL_POINT_60_30		= 271079,	// 60SP 30일권
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
			CI_CASH_SKILL_POINT_5			= 211340, // 캐시 스킬 포인트 5
			CI_CASH_SKILL_POINT_10			= 211330, // 캐시 스킬 포인트 10
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

			CI_SKILL_RESET_ITEM				= 200730, // 스킬 한개 초기화
			CI_SKILL_RESET_EVENT_ITEM		= 130808, // 스킬 한개 초기화
			CI_SKILL_RESET_EVENT_ITEM2		= 160947, // 스킬 한개 초기화
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
			EI_SKILL_RESET_EVENT_ITEM2		= 67003592, // 망각의 알약(스킬 한개 초기화)
			EI_SKILL_RESET_EVENT_ITEM3		= 67006745, // 망각의 알약(스킬 한개 초기화)
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP

			CI_SKILL_INIT_ITEM				= 203800, // 스킬 전체 초기화
			CI_SKILL_INIT_EVENT_ITEM		= 130633, // 스킬 전체 초기화 이벤트 아이템

#ifdef SERV_NEW_SKILL_INIT_EVENT_ITEM// 작업날짜: 2013-06-25	// 박세훈
			CI_SKILL_INIT_EVENT_ITEM2		= 135416, // 스킬 전체 초기화 이벤트 아이템2
			CI_SKILL_INIT_EVENT_ITEM3		= 135417, // 스킬 전체 초기화 이벤트 아이템3
			CI_SKILL_INIT_EVENT_ITEM4		= 160946, // 스킬 전체 초기화 이벤트 아이템4
#endif // SERV_NEW_SKILL_INIT_EVENT_ITEM

#ifdef SERV_GLOBAL_DEFINE
			CI_SKILL_INIT_EVENT_ITEM_JP		= 65000430, // 스킬 전체 초기화 이벤트 아이템 국내 130633
#endif //SERV_GLOBAL_DEFINE

#ifdef SERV_UNLIMITED_SKILL_RESET_ITEM
			CI_UNLIMITED_SKILL_RESET_ITEM	= 60008069, // 무제한 선택 스킬 초기화 메달
#endif //SERV_UNLIMITED_SKILL_RESET_ITEM

			CI_EXPAND_CHAR_SLOT_1 			= 206820, // 캐릭터 슬롯 확장 1
			CI_EXPAND_CHAR_SLOT_2 			= 206830, // 캐릭터 슬롯 확장 2
			CI_CHANGE_NICKNAME	  			= 209660, // 캐릭터 닉네임 변경 카드
			CI_PREMIUM_SHOP		  			= 200740, // 프리미엄 개인상점

			//{{ 2009. 11. 24  최육사	길드스킬
			CI_GUILD_SKILL_RESET_ITEM		= 110857, // 망각의 피리
			CI_GUILD_SKILL_INIT_ITEM		= 110858, // 망각의 뿔피리
			//}}

			//{{ 2009. 8. 7  최육사		은행
			CI_BANK_MEMBERSHIP_SILVER		= 213460, // 실버 회원권
			CI_BANK_MEMBERSHIP_GOLD			= 213470, // 골드 회원권
			CI_BANK_MEMBERSHIP_EMERALD		= 213480, // 에메랄드 회원권
			CI_BANK_MEMBERSHIP_DIAMOND		= 213490, // 다이아몬드 회원권
			CI_BANK_MEMBERSHIP_PLATINUM		= 213500, // 플래티넘 회원권
			CI_BANK_MEMBERSHIP_ONE_PACKAGE	= 213610, // 코보 신용 보증서
			//}}

			//{{ 2011. 12. 13  김민성	퀵 슬롯 확장 아이템
#ifdef SERV_EXPAND_QUICK_SLOT
#if defined(SERV_COUNTRY_TWHK) || defined(SERV_COUNTRY_JP)
			CI_EXPAND_QUICK_SLOT	        = 70002820, // 퀵슬롯 확장권(대만, 일본)
#else
			CI_EXPAND_QUICK_SLOT			= 244560, // 퀵슬롯 확장권
#endif SERV_COUNTRY_TWHK
			CI_EXPAND_QUICK_SLOT_ARA		= 261650,	// 퀵슬롯 확장권(아라 전용)
#endif SERV_EXPAND_QUICK_SLOT
			//}}
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// 적용날짜: 2013-07-11
			CI_EXPAND_QUICK_SLOT_ELESIS		= 261663,	// 퀵슬롯 확장권(엘리시스 전용)
#endif	// SERV_CASH_ITEM_FOR_ELESIS

			CI_NASOD_MEGAPHONE				= 212350, // 메가폰
			CI_HIGH_NASOD_MEGAPHONE			= 212360, // 고성능 메가폰

			//{{ 2009. 9. 1  최육사		밀봉 아이템
			CI_SEAL_ITEM_NORMAL				= 214370, // 뽀루의 발 도장(하얀색)
			CI_SEAL_ITEM_RARE				= 214380, // 뽀루의 발 도장(노란색)
			CI_SEAL_ITEM_ELITE				= 214390, // 뽀루의 발 도장(보라색)
			CI_SEAL_ITEM_UNIQUE				= 214400, // 뽀루의 발 도장(황금색)
			//}}

			//{{  2011.12.21     김민성    코보 발도장 이벤트
#ifdef SERV_COBO_SEAL_ITEM_EVENT
#ifdef SERV_COUNTRY_JP
			CI_EVENT_SEAL_ITEM_NORMAL			= 70001900, // 뽀루의 발 도장(하얀색) 국내 160372
			CI_EVENT_SEAL_ITEM_RARE				= 70001901, // 뽀루의 발 도장(노란색)			
			CI_EVENT_SEAL_ITEM_ELITE			= 70001902, // 뽀루의 발 도장(보라색) 국내 160373
			CI_EVENT_SEAL_ITEM_UNIQUE			= 70001903, // 뽀루의 발 도장(황금색)			
#else //SERV_COUNTRY_JP
			CI_EVENT_SEAL_ITEM_NORMAL				= 160372, // 코보 뽀루의 발 도장(하얀색)
			CI_EVENT_SEAL_ITEM_ELITE				= 160373, // 코보 뽀루의 발 도장(보라색)

#ifdef SERV_EVENT_SEAL_ITEM_UNIQUE// 작업날짜: 2013-05-08	// 박세훈
			CI_EVENT_SEAL_ITEM_UNIQUE		= 160794, // [코보]뽀루의 발 도장 (황금색)
#endif // SERV_EVENT_SEAL_ITEM_UNIQUE
#endif //SERV_COUNTRY_JP

#endif SERV_COBO_SEAL_ITEM_EVENT
			//}}

			//{{ 2009. 9. 22  최육사	전직캐쉬
			CI_CHANGE_JOB_ELSWORD_KNIGHT	= 214410, // 소드 나이트의 증명
			CI_CHANGE_JOB_MAGIC_KNIGHT		= 214420, // 매직 나이트의 증명
			CI_CHANGE_JOB_COMBAT_RANGER		= 214450, // 컴뱃 레인저의 영혼
			CI_CHANGE_JOB_SNIPING_RANGER	= 214460, // 스나이핑 레인저의 영혼
			CI_CHANGE_JOB_HIGH_MAGICIAN		= 214430, // 하이 매지션의 메모라이즈
			CI_CHANGE_JOB_DARK_MAGICIAN		= 214440, // 다크 매지션의 메모라이즈
			CI_CHANGE_JOB_SOUL_TAKER		= 214470, // 소드 테이커의 계시
			CI_CHANGE_JOB_OVER_TAKER		= 214480, // 오버 테이커의 계시
			CI_CHANGE_JOB_EXOTIC_GEAR		= 214490, // 코드 엑조틱의 메모리
			CI_CHANGE_JOB_ARCHITECTURE		= 214500, // 코드 아키텍처의 메모리
			//}}
			//{{ 2009. 10. 24  최육사	엘소드2차전직
			CI_CHANGE_JOB_LORD_KNIGHT		= 216420, // 로드 나이트 임명장
			CI_CHANGE_JOB_RUNE_SLAYER		= 216430, // 룬 슬레이어 징표
			//}}
			//{{ 2009. 11. 23  최육사	레나2차전직
			CI_CHANGE_JOB_WIND_SNEAKER		= 217850, // 윈드 스니커 전직 캐시 아이템
			CI_CHANGE_JOB_GRAND_ARCHER		= 217860, // 그랜드 아처 전직 캐시 아이템
			//}}
			//{{ 2009. 12. 11  최육사	아이샤 2차 전직
			CI_CHANGE_JOB_ELMENTAL_MASTER	= 217920, // 엘리멘탈 마스터 전직 아이템
			CI_CHANGE_JOB_VOID_PRINCESS		= 217930, // 보이드 프린세스 전직 아이템
			//}}
			//{{ 2010. 1. 20  손영준	레이븐/이브 2차 전직
			CI_CHANGE_JOB_BLADE_MASTER		= 217950, // 블레이드 마스터 전직 아이템
			CI_CHANGE_JOB_RECKLESS_FIST		= 217960, // 레크리스 피스트 전직 아이템
			CI_CHANGE_JOB_NEMESIS			= 217970, // 코드 네미시스 전직 아이템
			CI_CHANGE_JOB_EMPRESS			= 217980, // 코드 엠프레스 전직 아이템
			//}}
			//{{ 2010. 04. 26  최육사	기술의 노트			
			CI_CASH_SKILL_NOTE_ITEM			= 221600,  // 캐쉬 기술의 노트 아이템
			//}}

			//{{ kimhc // 2011.1.3 // 청 1차 전직
			CI_CHANGE_JOB_FURY_GUARDIAN		= 229460,	// 퓨리 가디언의 결의
			CI_CHANGE_JOB_SHOOTER_GUARDIAN	= 229470,	// 슈터 가디언의 결의
			//}} kimhc // 2011.1.3 // 청 1차 전직

			//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
			CI_CHANGE_JOB_IRON_PALADIN		= 235120,	/// 아이언 팔라딘의
			CI_CHANGE_JOB_DEADLY_CHASER		= 235130,	/// 데들리 체이서의
			//}}
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
			CI_UNLIMITED_CHANGE_SECOND_JOB			= 153000168,	//무제한 2차 전직 아이템
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

			//{{ 2011. 04. 14	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
			CI_PERSONAL_SHOP_AGENCY			= 233210,	// 대리 판매 보증서
#endif SERV_PSHOP_AGENCY
			//}}

#ifdef SERV_ELSWORD_SHEATH_KNIGHT
			CI_CHANGE_JOB_SHEATH_KNIGHT		= 242310,	// 시스 나이트 전직 캐시 아이템
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			CI_CHANGE_JOB_BATTLE_MAGICIAN	= 242320,	// 배틀 매지션 전직 캐시 아이템
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_TRAPPING_RANGER_TEST
			CI_CHANGE_JOB_TRAPPING_RANGER	= 242330,	// 트래핑 레인져 전직 캐시 아이템
#endif SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RAVEN_WEAPON_TAKER
			CI_CHANGE_JOB_WEAPON_TAKER		= 242340,	// 웨폰 테이커 전직 캐시 아이템
#endif SERV_RAVEN_WEAPON_TAKER
#ifdef SERV_EVE_ELECTRA
			CI_CHANGE_JOB_EVE_ELECTRA		= 242350,	// 일렉트라 전직 캐시 아이템
#endif SERV_EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			CI_CHANGE_JOB_SHELLING_GUARDIAN	= 242360,	// 쉘링 가디언
#endif

#ifdef SERV_ELSWORD_INFINITY_SWORD
			CI_CHANGE_JOB_INFINITY_SWORD	= 252640,
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
			CI_CHANGE_JOB_DIMENSION_WITCH	= 252650,
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
			CI_CHANGE_JOB_NIGHT_WATCHER		= 252660,
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
			CI_CHANGE_JOB_VETERAN_COMMANDER	= 252670,
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
			CI_CHANGE_JOB_BATTLE_SERAPH		= 252680,
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
			CI_CHANGE_JOB_TACTICAL_TROOPER	= 252690,
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
			CI_CHANGE_JOB_LITTLE_HSIEN		= 264380,			/// 아라 소선 전직 아이템
			CI_CHANGE_JOB_SAKRA_DEVANAM		= 264390,			/// 아라 제천 전직 아이템
#endif //SERV_ARA_CHANGE_CLASS_FIRST

#ifdef SERV_NEW_CHARACTER_EL
			CI_CHANGE_JOB_SABER_KNIGHT		= 264391,			/// 엘리시스 세이버나이트 전직 아이템
			CI_CHANGE_JOB_PYRO_KNIGHT		= 264392,			/// 엘리시스 파이로나이트 전직 아이템
#endif //SERV_NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
			CI_CHANGE_JOB_LITTLE_DEVIL		= 264393,			/// 아라 소마 전직 아이템
			CI_CHANGE_JOB_YAMA_RAJA			= 264394,			/// 아라 명왕 전직 아이템
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한 
			CI_CHANGE_JOB_GRAND_MASTER		= 272169,			/// 엘리시스 그랜드 마스터 전직 아이템
			CI_CHANGE_JOB_BLAZING_HEART		= 272170,			/// 엘리시스 블레이징 하트 전직 아이템
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			CI_CHANGE_JOB_PSYCHIC_TRACER	= 272810,			/// 애드 사이킥 트레이서 전직 아이템
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			CI_CHANGE_JOB_LUNATIC_PSYKER	= 273232,			/// 애드 루나틱 사이커 전직 아이템
#endif //SERV_ADD_LUNATIC_PSYKER


			//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
			CI_SKILL_POINT_30_USE_INVEN		= 241780,	// 인벤토리에서 클릭해서 사용하는 그노시스 축복
			CI_SKILL_POINT_60_USE_INVEN		= 241790,	// 인벤토리에서 클릭해서 사용하는 그노시스 축복
			CI_SKILL_POINT_60_USE_INVEN_ARA	= 261660,	// 그노시스 축복(아라전용)
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
			/*
			CI_SKILL_POINT_5_USE_INVEN		= 241780,	// 인벤토리에서 클릭해서 사용하는 그노시스 축복
			CI_SKILL_POINT_10_USE_INVEN		= 241790,	// 인벤토리에서 클릭해서 사용하는 그노시스 축복
			CI_SKILL_POINT_10_USE_INVEN_ARA	= 261660,	// 그노시스 축복(아라전용)
			*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013			
			CI_EXPAND_SKILL_SLOT_IN_PACKAGE	= 241800,	// 스킬 슬롯 체인지 메달
#endif SERV_ADD_PACKAGE_PRODUCT
			//}}

			//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
			CI_CLASS_CHANGE_ELSWORD_KNIGHT			= 241950,
			CI_CLASS_CHANGE_ELSWORD_MAGIC_KNIGHT	= 241960,
			CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT	= 241970,

			CI_CLASS_CHANGE_AISHA_HIGH_MAGICIAN		= 241980,
			CI_CLASS_CHANGE_AISHA_DARK_MAGICIAN		= 241990,
			CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN	= 242000,

			CI_CLASS_CHANGE_RENA_COMBAT_RANGER		= 242010,
			CI_CLASS_CHANGE_RENA_SNIPING_RANGER		= 242020,
			CI_CLASS_CHANGE_RENA_TRAPPING_RANGER	= 242030,

			CI_CLASS_CHANGE_REVEN_SOUL_TAKER		= 242040,
			CI_CLASS_CHANGE_REVEN_OVER_TAKER		= 242050,
			CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER		= 242060,

			CI_CLASS_CHANGE_EVE_EXOTIC_GEAR			= 242070,
			CI_CLASS_CHANGE_EVE_ARCHITECTURE		= 242080,
			CI_CLASS_CHANGE_EVE_ELECTRA				= 242090,

			CI_CLASS_CHANGE_CHUNG_FURY_GUARDIAN		= 242100,
			CI_CLASS_CHANGE_CHUNG_SHOOTER_GUARDIAN	= 242110,
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN	= 242120,
#endif

			CI_CLASS_CHANGE_ELSWORD_LORD_KNIGHT		= 242130,
			CI_CLASS_CHANGE_ELSWORD_RUNE_SLAYER		= 242140,
#ifdef SERV_ELSWORD_INFINITY_SWORD
			CI_CLASS_CHANGE_ELSWORD_INFINITY_SWORD	= 242150,
#else
			CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT_2	= 242150,
#endif //SERV_ELSWORD_INFINITY_SWORD
			CI_CLASS_CHANGE_AISHA_VOID_PRINCESS		= 242160,
			CI_CLASS_CHANGE_AISHA_ELMENTAL_MASTER	= 242170,
#ifdef SERV_ARME_DIMENSION_WITCH
			CI_CLASS_CHANGE_AISHA_DIMENSION_WITCH	= 242180,
#else
			CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN_2	= 242180,
#endif

			CI_CLASS_CHANGE_RENA_WIND_SNEAKER		= 242190,			
			CI_CLASS_CHANGE_RENA_GRAND_ARCHER		= 242200,			
#ifdef SERV_RENA_NIGHT_WATCHER
			CI_CLASS_CHANGE_RENA_NIGHT_WATCHER		= 242210,
#else
			CI_CLASS_CHANGE_RENA_TRAPPING_RANGER_2	= 242210,
#endif
			
			CI_CLASS_CHANGE_REVEN_BLADE_MASTER		= 242220,
			CI_CLASS_CHANGE_REVEN_RECKLESS_FIST		= 242230,
#ifdef SERV_RAVEN_VETERAN_COMMANDER
			CI_CLASS_CHANGE_RAVEN_VETERAN_COMMANDER	= 242240,
#else
			CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER_2	= 242240,
#endif
			
			CI_CLASS_CHANGE_EVE_NEMESIS				= 242250,			
			CI_CLASS_CHANGE_EVE_EMPRESS				= 242260,
#ifdef SERV_EVE_BATTLE_SERAPH
			CI_CLASS_CHANGE_EVE_BATTLE_SERAPH		= 242270,
#else
			CI_CLASS_CHANGE_EVE_ELECTRA_2			= 242270,
#endif
			
			CI_CLASS_CHANGE_CHUNG_IRON_PALADIN		= 242280,
			CI_CLASS_CHANGE_CHUNG_DEADLY_CHASER		= 242290,
#ifdef SERV_CHUNG_TACTICAL_TROOPER
			CI_CLASS_CHANGE_CHUNG_TACTICAL_TROOPER	= 242300,
#else
			CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN_2	= 242300,
#endif

#ifdef SERV_NEW_CHARACTER_EL	// 적용날짜: 2013-07-18
			CI_CLASS_CHANGE_ELESIS_SABER_KNIGHT		= 252754,
			CI_CLASS_CHANGE_ELESIS_PYRO_KNIGHT		= 252755,
#endif	// SERV_NEW_CHARACTER_EL

#endif SERV_UNIT_CLASS_CHANGE_ITEM

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
			CI_CLASS_CHANGE_UC_ARA_LITTLE_HSIEN		= 252757,	/// 아라 1차 전직 소선 전직 변경권
			CI_CLASS_CHANGE_ARA_LITTLE_DEVIL		= 252756,	/// 아라 1차 전직 소마 전직 변경권
			
			CI_CLASS_CHANGE_ARA_SAKRA_DEVANAM		= 252751,	/// 아라 2차 전직 제천 전직 변경권
			CI_CLASS_CHANGE_ARA_YAMA_RAJA			= 252752,	/// 아라 2차 전직 명왕 전직 변경권
#endif // SERV_ARA_CHANGE_CLASS_SECOND
			//}}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
			CI_CLASS_CHANGE_ELESIS_GRAND_MASTER		= 272171,	/// 엘리시스 2차 전직 그랜드 마스터 전직 변경권
			CI_CLASS_CHANGE_ELESIS_BLAZING_HEART	= 272172,	/// 엘리시스 2차 전직 블레이징 하트 전직 변경권
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			CI_CLASS_CHANGE_PSYCHIC_TRACER			= 999992,	/// 애드 1차 전직 사이킥 트레이서 전직 변경권
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			CI_CLASS_CHANGE_LUNATIC_PSYKER			= 999993,	/// 애드 2차 전직 루나틱 사이커 전직 변경권
#endif //SERV_ADD_LUNATIC_PSYKER

			//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
			CI_PET_NAME_CHANGE						= 248390,
#endif SERV_PET_CHANGE_NAME
			//}}

			//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
			CI_GUILD_NAME_CHANGE					= 248400,
#endif SERV_GUILD_CHANGE_NAME
			//}}

			//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
			CI_PET_TOY								= 500580,
#endif SERV_PET_TOY
			//}}
			//{{ 2013. 2. 28	박세훈	 빙고 이벤트
#ifdef SERV_EVENT_BINGO
			CI_BINGO_MIX_ITEM						= 257280,
			CI_BINGO_RESET_ITEM						= 257290,
#endif SERV_EVENT_BINGO
			//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// 작업날짜: 2013-05-12	// 박세훈
			CI_REQUEST_OF_LOVE						= 135404,	// 사랑의 리퀘스트
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_SKILL_PAGE_SYSTEM
			CI_EXPAND_SKILL_PAGE					= 127273,	// 스킬 페이지 확장권
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_CASH_ITEM_FOR_ADD
			CI_EXPAND_SKILL_SLOT_ADD				= 272738,	// 스킬 슬롯 B확장 엘리시스 전용 판매 캐시템
#endif // SERV_CASH_ITEM_FOR_ADD

//----------------------------------------------------------------------------------//
#ifdef EVENT_GUILD_ITEM
			CI_EVENT_GUILD_SKILL_RESET_ITEM			= 80001182,	//(중국) 이벤트용 아리엘의 망각의 피리 // 2012.06.05 lygan_조성욱
#endif //EVENT_GUILD_ITEM
#ifdef SERV_EVENT_SKILL_POINT
			CI_SKILL_5_POINT_30						= 60005091,	// 예제임 미션던전때 쓰던 아이템
			CI_SKILL_10_POINT_60					= 60005092, // 예제임 미션던전때 쓰던 아이템
#endif

#ifdef SERV_REFORM_INVENTORY_INT_CASH_ITEM
			// 인벤토리 카테고리별 확장
			CI_EXPAND_INVENTORY_EQUIP				= 200751, // 장비
			CI_EXPAND_INVENTORY_ACCESSORY			= 200752, // 악세사리
			CI_EXPAND_INVENTORY_QUICK_SLOT			= 200753, // 소비
			CI_EXPAND_INVENTORY_MATERIAL			= 200754, // 재료
			CI_EXPAND_INVENTORY_QUEST				= 200755, // 퀘스트
			CI_EXPAND_INVENTORY_SPECIAL				= 200756, // 특수
#endif  SERV_REFORM_INVENTORY_INT_CASH_ITEM

			//{{ 지헌 - 은행 개편 퀘스트, 캐쉬 작업 
#ifdef SERV_SHARING_BANK_QUEST_CASH
#ifdef SERV_COUNTRY_TWHK
			CI_CASH_SHARING_BACK_OPEN				= 60003460,	// 은행 공유 활성화 아이템(대만)
#else
			CI_CASH_SHARING_BACK_OPEN				= 65000701,
#endif SERV_COUNTRY_TWHK
#endif

#ifdef SERV_SYNTHESIS_AVATAR
			CI_SYNTHESIS_ITEM						= 65000550,
#endif SERV_SYNTHESIS_AVATAR

#ifdef SERV_VIP_SYSTEM
			CI_EVENT_NASOD_MEGAPHONE				= 80000119, //(중국) 이벤트 용 나소드 메가폰 //2011.09.17 lygan_조성욱
			CI_EVENT_HIGH_NASOD_MEGAPHONE			= 80000120, //(중국) 이벤트 용 고성능 나소드 메가폰 //2011.09.17 lygan_조성욱
#endif //SERV_VIP_SYSTEM
#ifdef SERV_CN_GNOSIS
			CI_EVENT_SKILL_POINT_10_USE_INVEN_7_DAY	= 80001150,	//(중국) 이벤트용 그노시스의 축복 ( 10 SP, 7일권) // 2012.05.24 lygan_조성욱
			CI_EVENT_SKILL_POINT_5_USE_INVEN_1_DAY	= 80001327, //(중국) 이벤트용 그노시스의 축복 ( 5 SP, 1일권) // 2012.06.25 lygan_조성욱

			CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY		 = 80001355, //(중국) 이벤트용 그노시스의 축복 ( 5 SP, 7일권) // 2012.07.02 lygan_조성욱
			CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY_TYPE2 = 80001372, //(중국) 이벤트용 그노시스의 축복 ( 5 SP, 7일권) // 2012.07.02 lygan_조성욱 // 이건 리얼 들어가면 절대 삭제 하지 않는 아이템
			CI_EVENT_SKILL_POINT_10_USE_INVEN_15_DAY	 = 80001367, //(중국) 이벤트용 그노시스의 축복 ( 10 SP, 15일권) // 2012.07.02 lygan_조성욱
#endif SERV_CN_GNOSIS
#ifdef SERV_GNOSIS_BR
			CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY = 90000690, // 그노시스의 축복 (5 SP, 7일권)
			CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY = 90000691, // 그노시스의 축복 (5 SP, 15일권)
#endif SERV_GNOSIS_BR

#ifdef SERV_EVENT_VIP_SYSTEM			
			CI_CASH_REWARD_EVENT_VIP_OPEN			= 83000170,	// (중국) VIP 인증아이템 //2012.11.06 lygan_조성욱 // 중국 전용 던전 보상창 2번 탭 오픈용 (이벤트)
#endif //SERV_EVENT_VIP_SYSTEM
#ifdef SERV_US_GNOSIS
			CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY	= 90000747, // 미국 이벤트 그노시스
#endif SERV_US_GNOSIS

#ifdef SERV_CASHITEM_PURCHASE_RESTRICTION
			CI_ICE_BURNER							= 207500,
			CI_OFFICER_EVE_ICE_BURNER				= 120003200,
			CI_OFFICER_CHUNG_ICE_BURNER				= 90001070,
			CI_MAGIC_NECKLACE						= 75000600,
			CI_ICE_BURNER_LURIAL					= 210000226,
#endif SERV_CASHITEM_PURCHASE_RESTRICTION

//----------------------------------------------------------------------------------//
// 해외팀 전용 이벤트 아이템
#ifdef SERV_GLOBAL_EVENT_ITEM

#ifdef SERV_EVENT_INIT_SKILL_TREE_ITEM
			CI_INIT_SKILL_TREE_ITEM					= 60001727,	// 망각의 드링크
#endif SERV_EVENT_INIT_SKILL_TREE_ITEM
#ifdef SERV_EVENT_RESET_A_SKILL_ITEM
			CI_RESET_A_SKILL_ITEM					= 60001726, // 망각의 알약
#endif SERV_EVENT_RESET_A_SKILL_ITEM
#ifdef SERV_ADD_WARP_BUTTON
			CI_WARP_VIP_ITEM						= 215660,	// 새로운 아이템으로 변경해야 할까?
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_QUEST_SKILL_RESET
			CI_SKILL_RESET_QUEST_ITEM				= 141000000, // 스킬 한개 초기화
#endif SERV_QUEST_SKILL_RESET
#ifdef SERV_2ND_CLASS_SKILL_RESET
			CI_SKILL_RESET_2ND_CLASS_EVENT_ITEM		= 141000036, // 스킬 한개 초기화
#endif SERV_2ND_CLASS_SKILL_RESET

#ifdef SERV_EVENT_BUY_FAKE_ITEM
			CI_BUY_FAKE_ITEM						= 67005900, //만우절 이벤트용 아크엔젤 패키지
#endif //SERV_EVENT_BUY_FAKE_ITEM

#ifdef SERV_EVENT_RURIEL_RESET_SKILL_ITEM
			EI_RURIEL_RESET_A_SKILL_ITEM			= 67006453, // 루리엘의 선택 스킬 초기화 메달
			EI_RURIEL_RESET_SKILL_ITEM				= 67006454, // 루리엘의 스킬 초기화 메달
			EI_RURIEL_RESET_A_SKILL_EVENT_ITEM		= 67006451, // 루리엘의 망각의 알약
#endif SERV_EVENT_RURIEL_RESET_SKILL_ITEM

#ifdef SERV_RURIEL_EVENT_ITEM
			CI_RURIEL_EVENT_SEAL_ITEM_NORMAL		= 83000290,	 //루리엘의 뽀루의 발 도장 (하얀색)
			CI_RURIEL_EVENT_SEAL_ITEM_RARE			= 83000291,	 //루리엘의 뽀루의 발 도장 (노란색)
			CI_RURIEL_EVENT_SEAL_ITEM_ELITE			= 83000292,	 //루리엘의 뽀루의 발 도장 (보라색)
			CI_RURIEL_EVENT_SEAL_ITEM_UNIQUE		= 83000293,	 //루리엘의 뽀루의 발 도장 (황금색)
#endif //SERV_RURIEL_EVENT_ITEM

#ifdef SERV_ARIEL_EVENT_ITEM
			CI_ARIEL_EVENT_SEAL_ITEM_NORMAL			= 82214370,	 //아리엘의 뽀루의 발 도장 (하얀색)
			CI_ARIEL_EVENT_SEAL_ITEM_RARE			= 82214380,	 //아리엘의 뽀루의 발 도장 (노란색)
			CI_ARIEL_EVENT_SEAL_ITEM_ELITE			= 82214390,	 //아리엘의 뽀루의 발 도장 (보라색)
			CI_ARIEL_EVENT_SEAL_ITEM_UNIQUE			= 82214400,	 //아리엘의 뽀루의 발 도장 (황금색)
#endif //SERV_ARIEL_EVENT_ITEM

#endif //SERV_GLOBAL_EVENT_ITEM
//----------------------------------------------------------------------------------//
		};
		//}}

		//{{ 2009. 9. 7  최육사		밀봉 카운트
		enum SEAL_ITEM_INFO
		{
            SEAL_ITEM_DECREASE_COUNT = 1,
		};
		//}}

		//{{ 2009. 2. 23  최육사	이벤트 아이템
		enum EVENT_ITEM
		{
			EI_GOLD_MEDAL				= 130002, // 대회채널용 황금메달
#ifdef SERV_COUNTRY_JP
			EI_XMAS_EVENT_ITEM			= 65000390, //산타아리엘 쿠폰 // 임규수 일본 변경 131354 크리스마스 트리 장식
#else //SERV_COUNTRY_JP
			EI_XMAS_EVENT_ITEM			= 131354, // 크리스마스 트리 장식
#endif //SERV_COUNTRY_JP
			EI_CAMILLAS_COIN			= 130715, // 카밀라의 코인
			EI_CAMILLAS_COIN_2010		= 132701, // 카밀라의 코인 ( 2010년 영웅대전 )
			EI_PRELIMINARY_TICKET		= 130843, // 예선 대회 참여권
			EI_OFFLINE_TICKET			= 135304, // 온라인 토너먼트 참가 신청권 130844, // 오프라인 대회 참여권
			EI_TREE_SUMMON_ITEM			= 131857, // 자연의 정수 ( 나무수호신 소환 )

			//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
			//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
			EI_SKILL_POINT_30_USE_INVEN	= 133131, // 이벤트용 그노시스 축복
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
			EI_SKILL_POINT_130_1DAY_USE_INVEN = 67006450,	// 이벤트용 그노시스 축복 
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
			EI_SKILL_POINT_30_1DAY_USE_INVEN		= 67006500, // 이벤트용 그노시스 축복 30sp 1일
			EI_SKILL_POINT_60_1DAY_USE_INVEN		= 67006501, // 이벤트용 그노시스 축복 60sp 1일
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
			EI_SKILL_POINT_30_USE_INVEN_JP			= 67000382,	// 이벤트용 그노시스 축복(2일)	133131
			EI_SKILL_POINT_10_30DAY_USE_INVEN_JP	= 67005710, // 이벤트용 그노시스 축복 10sp 30일
			EI_SKILL_POINT_10_15DAY_USE_INVEN_JP	= 70007020, // 이벤트용 그노시스 축복 10sp 15일
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
			EI_SKILL_POINT_30_7DAY_USE_INVEN	= 60003758, // 이벤트용 그노시스 축복 30SP 7일
			EI_SKILL_POINT_60_7DAY_USE_INVEN	= 60003759, // 이벤트용 그노시스 축복 60SP 7일
			EI_SKILL_POINT_60_15DAY_USE_INVEN	= 60004280, // 이벤트용 그노시스 축복 60SP 15일
			EI_SKILL_POINT_60_30DAY_USE_INVEN	= 60005011, // 이벤트용 그노시스 축복 60SP 30일
			EI_SKILL_POINT_60_7DAY_USE_INVEN_2	= 60008007, // 이벤트용 그노시스 축복 60SP 7일	
			EI_SKILL_POINT_30_7DAY_USE_INVEN_2	= 70007030, // 이벤트용 그노시스 축복 30SP 7일
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
			/*
			EI_SKILL_POINT_5_USE_INVEN	= 133131, // 이벤트용 그노시스 축복
			*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
			EI_SKILL_POINT_30_7DAY_USE_INVEN_INT	= 70007030, // 이벤트용 그노시스 축복 30SP 7일
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
			//}}

			//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
			EI_XMAS_RANDOM_CUBE			= 133223, // 크리스마스 이벤트 큐브 ( 산타 양말 )
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
			//}}

			//{{ 2011. 07. 04    김민성    네이버 프로모션
#ifdef SERV_NAVER_PROMOTIONS
			EI_SUMMER_RANDOM_CUBE		= 160227,	 // 한 여름 대박 큐브
			EI_WINTER_RANDOM_CUBE		= 160326,	 // 한 겨울 대박 큐브
			EI_WINTER_BIG_RANDOM_CUBE	= 160374,	 // 한 겨울 왕대박 큐브
			EI_SUMMER_TEN_ENCHANT_CHARM = 130152,	 // 10 강 부적
			EI_SUMMER_RANDOM_NDS		= 160230, // 닌텐도 지급 번호
			EI_SUMMER_NEXON_CASH		= 160229, // 넥슨 캐쉬 10000
			EI_SUMMER_NEXON_CASH_50000	= 135105, // 넥슨 캐쉬 50000
			//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
			EI_RANDOM_CUBE_GOLD			= 135115, // 봉인된 얼음 조각상(Gold)
#endif SERV_RANDOM_CUBE_GOLD_REWARD
			//}}
#endif SERV_NAVER_PROMOTIONS
			//}} 

			//{{ 2011. 01. 06  김민성  스킬슬롯체인지 체크(인벤토리-기간제) 기능 구현
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
			EI_EXPAND_SKILL_SLOT_EVENT	= 200880, // 이벤트 스킬 슬롯B (기간제)
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY
			//}}
			//{{ 2011. 08. 26	최육사	캐릭터 레벨업 아이템
#ifdef SERV_CHAR_LEVEL_UP_ITEM
			EI_CHAR_LEVEL_UP_ITEM		= 160267, // 현자의 주문서
#endif SERV_CHAR_LEVEL_UP_ITEM
			//}}
			//{{ 2011. 12. 05	최육사	축복의 메달 큐브 체크
#ifdef SERV_CHECK_BLESSING_MEDAL_CUBE
			EI_BLESSING_MEDAL_CUBE		= 132698, // 축복의 메달(1일권) 큐브
#endif SERV_CHECK_BLESSING_MEDAL_CUBE
			//}}
			//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
			EI_COBO_PC_BANG_COUPON		= 135093, // 코보 PC방 쿠폰
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
			//}}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
			EI_EVENT_MONEY_ITEM		= 135465, // 뮤의 출석도장
#endif // SERV_EVENT_MONEY

			//{{ 2012. 07. 09	김민성       현자의 마법석
//#ifdef SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
			EI_WISE_MAN_STONE_ITEM		= 135184, // 현자의 마법석
//#endif SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
			//}}
			//{{ 2012. 07. 09	김민성       이벤트 큐브 오픈 가능 시간
#ifdef SERV_EVENT_CUBE_OPEN_TIME
			EI_EVENT_LOTTO_CUBE				= 160533, // 대박 큐브
			EI_EVENT_GOLD_CUBE				= 160534, // 황금 큐브
			EI_EVENT_CASH_CUBE				= 160535, // 넥슨 캐시 큐브

			EI_NEXON_CASH_100				= 160536, // 넥슨 캐시 100
			EI_NEXON_CASH_1000				= 160537, // 넥슨 캐시 1000
			EI_NEXON_CASH_5000				= 160538, // 넥슨 캐시 5000
			EI_NEXON_CASH_10000				= 160539, // 넥슨 캐시 10000
#endif SERV_EVENT_CUBE_OPEN_TIME
			//}}
			//{{ 2012. 12. 19  교환(한정 수량) - 김민성
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
			EI_ARA_EVENT_NEXON_CASH_1000		= 135318, // 천년 여우 넥슨 캐시 1000원 지급 쿠폰
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
			//}}

			//{{ 2013. 03. 27	 그노시스의 축복 5point 7일 추가 - 김민성
#ifdef SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
			EI_SKILL_POINT_30_DAY_7_USE_INVEN		= 268562,	// 인벤토리에서 클릭해서 사용하는 그노시스 축복 30포인트 7일제
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
			/*
			EI_SKILL_POINT_5_DAY_7_USE_INVEN		= 268562,	// 인벤토리에서 클릭해서 사용하는 그노시스 축복 5포인트 7일제
			*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#endif SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
			//}

#ifdef SERV_NEW_ITEM_COBO_SKILL_RESET_ITEM// 작업날짜: 2013-07-02	// 박세훈
			CI_COBO_SKILL_RESET_ITEM		= 135433, // 스킬 한개 초기화
#endif // SERV_NEW_ITEM_COBO_SKILL_RESET_ITEM

//----------------------------------------------------------------------------------//
// 해외팀 전용 이벤트 아이템
#ifdef SERV_GLOBAL_EVENT_ITEM
		// 추후 추가되는 경우 이곳에 넣어주시기 바랍니다.

#ifdef SERV_CHAR_LEVEL_UP_ITEM_EVENT_RENA
			EL_CHAR_LEVEL_UP_ITEM2		= 60004276,	// 시공간의 주문서
#endif SERV_CHAR_LEVEL_UP_ITEM_EVENT_RENA
		
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		// 복귀 유저를 위한 특별 패키지 판매(7.4)
		EI_SERVICE_COUPON_TYPE_A				= 60007075,	// 맞춤 서비수 쿠폰 Type A
		EI_SERVICE_COUPON_TYPE_A_PACKAGE		= 60007078,	// 맞춤 서비수 쿠폰 Type A 패키지

		EI_SERVICE_COUPON_TYPE_B				= 60007076,	// 맞춤 서비수 쿠폰 Type B
		EI_SERVICE_COUPON_TYPE_B_PACKAGE		= 60007079,	// 맞춤 서비수 쿠폰 Type B 패키지

		EI_SERVICE_COUPON_TYPE_C				= 60007077,	// 맞춤 서비수 쿠폰 Type C
		EI_SERVICE_COUPON_TYPE_C_PACKAGE		= 60007080,	// 맞춤 서비수 쿠폰 Type C 패키지

		EI_ARA_SPECIAL_COUPON					= 60007091,	// 아라 스페셜 쿠폰
		EI_ARA_SPECIAL_ATTRACTION_PACKAGE		= 60007113,	// 아라 스페셜 가열기 패키지
		EI_ARA_SPECIAL_AVATAR1_PACKAGE			= 60007114,	// 아라 스페셜 아바타 패키지1
		EI_ARA_SPECIAL_AVATAR2_PACKAGE			= 60007115,	// 아라 스페셜 아바타 패키지2
		EI_ARA_SPECIAL_PET_PACKAGE				= 60007116,	// 아라 스페셜 펫 패키지

		EI_ARIEL_SPECIAL_PRESENT_CUBE_LV12		= 60007062, // 아리엘 특별 선물 큐브 Lv.12
		EI_ARIEL_SPECIAL_PRESENT_CUBE_LV18		= 60007064, // 아리엘 특별 선물 큐브 Lv.18
		EI_ARIEL_SPECIAL_PRESENT_CUBE_LV20		= 60007066, // 아리엘 특별 선물 큐브 Lv.20
		EI_SPECIAL_PRESENT_COPPER_KEY			= 60007069, // 스페셜 선물 열쇠(동)
		EI_SPECIAL_PRESENT_SILVER_KEY			= 60007070, // 스페셜 선물 열쇠(은)
		EI_SPECIAL_PRESENT_GOLD_KEY				= 60007071, // 스페셜 선물 열쇠(금)

		EI_SPECIAL_HEAVENLY_BOSS_RARE			= 70007203, // 천상의 지배자(레어)
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_READY_TO_SOSUN_EVENT
		EI_READY_TO_SOSUN_EVENT_ITEM			= 60007092,	// 아라 1차 전직 이름맞추기 실행권
		EI_READY_TO_SOSUN_EVENT_WIN_ITEM		= 60007112,	// 아라 1차 전직 이름맞추기 당첨 보상
#endif SERV_READY_TO_SOSUN_EVENT
#ifdef SERV_RELATIONSHIP_EVENT_INT
		EI_EVENT_PROPOSE_ITEM					= 60007791,	// 가상 결혼 신청서
		EI_EVENT_DIVORCE_ITEM					= 60007792, // 가상 결혼 계약서
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
		EI_RECRUIT_CUBE							= 90002350, // 신규유저 지원 큐브
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		EI_INFINITY_EL_ESSENCE_ITEM_EVENT		= 60008064,		// 엘의 정수 공급기
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
		EI_SKILL_POINT_30_14DAY_USE_INVEN	= 141000896, //춘절 그노시스 
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
		EI_LURIEL_GNOSIS_30_15DAY				= 210000270, // 루리엘의 그노시스의 축복 (30SP/15일)
		EI_LURIEL_GNOSIS_30_30DAY				= 210000271, // 루리엘의 그노시스의 축복 (30SP/30일)
		EI_LURIEL_GNOSIS_30_60DAY				= 210000272, // 루리엘의 그노시스의 축복 (30SP/60일)
		EI_LURIEL_GNOSIS_30_7DAY				= 210000273, // 루리엘의 그노시스의 축복 (30SP/7일)
		EI_LURIEL_GNOSIS_60_15DAY				= 210000280, // 루리엘의 그노시스의 축복 (60SP/15일)
		EI_LURIEL_GNOSIS_60_30DAY				= 210000281, // 루리엘의 그노시스의 축복 (60SP/30일)
		EI_LURIEL_GNOSIS_60_60DAY				= 210000282, // 루리엘의 그노시스의 축복 (60SP/60일)
		EI_LURIEL_GNOSIS_60_7DAY				= 210000283, // 루리엘의 그노시스의 축복 (60SP/7일)
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
		EI_TEAR_OF_ELWOMAN		= 85001550	, // 엘의 여인의 눈물
#endif SERV_EVENT_TEAR_OF_ELWOMAN

#endif //SERV_GLOBAL_EVENT_ITEM
//----------------------------------------------------------------------------------//
		};
		//}}

		//{{ 2012. 03. 15	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		enum REWARD_ITEM
		{
            RI_AUTO_PARTY_BONUS			= 135289, // 영웅의 주화
		};
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		enum DEFENCE_DUNGEON_REWARD
		{
			DDR_DRAGON_PIECE			= 180034, // 비룡의 비늘 조각
		};

		//{{ 2009. 12. 15  최육사	ED아이템
		enum ED_ITEM
		{
            EDI_BRONZE_ED				= 127000, // 브론즈ED
			EDI_SILVER_ED				= 127010, // 실버ED
			EDI_GOLD_ED					= 127020, // 골드ED
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-15	// 박세훈
			EDI_GOLD_BAR				= 127041, // 금괴
#endif // SERV_BATTLE_FIELD_BOSS
		};
		//}}

		//{{ 2009. 3. 9  최육사		마을이동아이템
		enum USE_IN_INVENTORY_ITEM
		{
			SI_WARP_RUBEN				= 109995, // 마을별 특급 이동 티켓 (루벤 마을)
			SI_WARP_ELDER				= 109996, // 마을별 특급 이동 티켓 (엘더 마을)
			SI_WARP_BESMA				= 109997, // 마을별 특급 이동 티켓 (베스마 마을)
			SI_WARP_ALTERA				= 109998, // 마을별 특급 이동 티켓 (알테라 마을)
			SI_WARP_PEITA				= 110832, // 페이타 특급 이동 티켓 (페이타 던전게이트)
			SI_WARP_VELDER				= 110847, // 벨더 근교 특급 이동 티켓 (벨더근교 마을)

			//{{ 2009. 8. 7  최육사		은행
			SI_BANK_MEMBERSHIP_UPGRADE	= 99380, // 루리엘의 신용 보증서
			//}}

			//{{ 2009. 9. 22  최육사	길드
			SI_GUILD_FOUNDATION			= 110845, // 길드 창단 허가서
			//}}

			//{{ 2009. 9. 23  최육사	1차전직캐쉬템
			SI_FIRST_CHANGEJOB_CUBE		= 109999, // 1차 전직 프로모션 큐브
			//}}
			//{{ 2009. 11. 10  최육사	2차전직
			SI_SECOND_CHANGE_JOB_CUBE	= 110850, // 2차 전직 프로모션 큐브
			//}}

			//{{ 2009. 10. 9  최육사	길드
			SI_GUILD_MAX_MEMBER_UPGRADE	= 110846, // 길드 인원 확장권
			//}}

			SI_RING_OF_PVP_REBIRTH		= 202410, // 리스폰 반지
			SI_BLESSING_MEDAL			= 200720, // 축복의 메달
			SI_EXP_15_PERCENT_MEDAL		= 129940, // 경험치 15% 메달
			SI_EXP_100_PERCENT_MEDAL	= 130529, // 경험치 100% 메달

			//{{ 2011. 10. 19    김민성    200% 경험치 추가 메달(이벤트 아이템)
#ifdef SERV_ADD_EXP_200_MEDAL
			SI_EXP_200_PERCENT_MEDAL	= 160299, // 성장의 메달  경험치 200% 메달
#endif SERV_ADD_EXP_200_MEDAL
			//}} 

			SI_USE_FREE_BY_FIELD		= 215660, // 자동결제 마을이동 아이템.

			//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
			SI_USE_COBO_EXPRESS_TICKET	= 112323, // ED 소모 마을이동 아이템.
#endif SERV_ADD_COBO_EXPRESS_TICKET
			//}}

			//{{ 2009. 11. 27  최육사	길드스킬
			SI_GUILD_CASH_SKILL_POINT	= 110859, // 그노시스의 대축복
			//}}
			//{{ 2010. 04. 09  최육사	기술의 노트
			SI_SKILL_NOTE_ITEM			= 99600,  // 기술의 노트 아이템
			//}}
			//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
			SI_PSHOP_AGENCY_1_DAY		= 160060, // 대리 판매 보증서(1일권)
			SI_PSHOP_AGENCY_2_DAYS		= 160061, // 대리 판매 보증서(2일권)
			SI_PSHOP_AGENCY_3_DAYS		= 160062, // 대리 판매 보증서(3일권)
			SI_PSHOP_AGENCY_5_DAYS		= 160063, // 대리 판매 보증서(5일권)
			SI_PSHOP_AGENCY_7_DAYS		= 160064, // 대리 판매 보증서(7일권)
			SI_PSHOP_AGENCY_10_DAYS		= 160065, // 대리 판매 보증서(10일권)
			SI_PSHOP_AGENCY_15_DAYS		= 160066, // 대리 판매 보증서(15일권)
			SI_PSHOP_AGENCY_30_DAYS		= 160067, // 대리 판매 보증서(30일권)

            SI_PSHOP_AGENCY_7_DAYS_ED   = 160904, // 대리 판매 보증서 (7일권, ED)
            SI_PSHOP_AGENCY_15_DAYS_ED  = 160905,//             대리 판매 보증서
            SI_PSHOP_AGENCY_30_DAYS_ED  = 160906,//             대리 판매 보증서
#endif SERV_PSHOP_AGENCY
			//}}
			
			//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
			SI_COME_BACK_REWARD_SKILL_30_POINT_15_DAY		= 127180, // 그노시스의 축복(15일권)
			SI_COME_BACK_REWARD_SKILL_30_POINT_30_DAY		= 127190, // 그노시스의 축복(30일권)
			SI_COME_BACK_REWARD_SKILL_30_POINT_60_DAY		= 127200, // 그노시스의 축복(60일권)
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
			SI_COME_BACK_REWARD_SKILL_5_POINT_15_DAY		= 127180, // 그노시스의 축복(15일권)
			SI_COME_BACK_REWARD_SKILL_5_POINT_30_DAY		= 127190, // 그노시스의 축복(30일권)
			SI_COME_BACK_REWARD_SKILL_5_POINT_60_DAY		= 127200, // 그노시스의 축복(60일권)
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#endif SERV_COME_BACK_USER_REWARD
//}} 

#ifdef SERV_GOLD_TICKET
			SI_GOLD_TICKET_10_ID			= 75000700,// Gold Ticket (10)
			SI_GOLD_TICKET_50_ID			= 75000701,// Gold Ticket (50)
			SI_GOLD_TICKET_100_ID			= 75000702,// Gold Ticket (100)
			SI_GOLD_TICKET_500_ID			= 75000703,// Gold Ticket (500)
			SI_GOLD_TICKET_1000_ID			= 75000704,// Gold Ticket (1000)
			SI_GOLD_TICKET_2000_ID			= 75000705,// Gold Ticket (2000)
#endif SERV_GOLD_TICKET

			//{{ 2012. 03. 25	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
			SI_THE_GATE_OF_DARKNESS_ARCANUM_NUT		= 78520,	// 누트의 가호
			SI_THE_GATE_OF_DARKNESS_ARCANUM_PTAH	= 78530,	// 프타의 가호
			SI_THE_GATE_OF_DARKNESS_ARCANUM_RACHEL	= 78540,	// 라헬의 가호
			SI_THE_GATE_OF_DARKNESS_ARCANUM_ZADKIEL	= 78550,	// 자키엘의 가호

			// 영약
			SI_THE_GATE_OF_DARKNESS_ELIXIR_SERAPHIM	= 78560,	// 세라핌의의 축복
			SI_THE_GATE_OF_DARKNESS_ELIXIR_GEB		= 78570,	// 게브의 축복
			SI_THE_GATE_OF_DARKNESS_ELIXIR_AMON		= 78580,	// 아몬의 축복
			SI_THE_GATE_OF_DARKNESS_ELIXIR_CRONOS	= 78590,	// 크로노스의 축복
			
			//{{ 2013. 04. 12	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
			// 영약
			SI_THE_GATE_OF_DARKNESS_ELIXIR_GIANT_POTION				= 78894,	// 어둠의 문 영약 : 거인화 물약
			SI_THE_GATE_OF_DARKNESS_ELIXIR_BLAZING_BOMB				= 78895,	// 어둠의 문 영약 : 이글이글 폭약
			SI_THE_GATE_OF_DARKNESS_ELIXIR_SPIRIT_OF_CHASER			= 78896,	// 어둠의 문 영약 : 추적자의 혼
			SI_THE_GATE_OF_DARKNESS_ELIXIR_CRADLE_OF_LITTLE_FAIRY	= 78897,	// 어둠의 문 영약 : 아기 요정 요람
			SI_THE_GATE_OF_DARKNESS_ELIXIR_ICE_BALL_OF_DENIPH		= 78898,	// 어둠의 문 영약 : 데니프의 얼음 구슬
			SI_THE_GATE_OF_DARKNESS_ELIXIR_FEATHER_OF_VENTUS		= 78899,	// 어둠의 문 영약 : 벤투스의 날개깃
			SI_THE_GATE_OF_DARKNESS_ELIXIR_FLAME_RING_OF_ROSSO		= 78900,	// 어둠의 문 영약 : 로쏘의 화염고리
			SI_THE_GATE_OF_DARKNESS_ELIXIR_BIG_HAND_POTION			= 78901,	// 어둠의 문 영약 : 거대손 물약
#endif SERV_NEW_DEFENCE_DUNGEON
			//}}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
			//}}

			//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
			SI_PET_AUTO_LOOTING_ITEM				= 500720,	// 펫 오토 루팅
#ifdef SERV_PET_AUTO_LOOTING_ITEM_CN
			SI_PET_AUTO_LOOTING_ITEM_CN				= 100000120,	// 펫 오토 루팅 중국용(기존 아이템도 사용합니다)
#endif // SERV_PET_AUTO_LOOTING_ITEM_CN
#endif SERV_PET_AUTO_LOOTING
			//}}

			//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
			SI_COUPLE_RING_ITEM						= 269710,	// 커플링
			SI_WEDDING_RING_ITEM					= 269711,	// 결혼반지
			SI_WEDDING_HALL_A_ITEM					= 269712,	// 결혼식장 캐시 - A type
			SI_WEDDING_HALL_B_ITEM					= 269713,	// 결혼식장 캐시 - B type
			SI_WEDDING_INVITE_TICKET_ITEM			= 269714,	// 결혼 초대권
			SI_WEDDING_RESERVATION_ITEM				= 269715,	// 결혼식 예약권
			SI_WEDDING_INVITATION_ITEM				= 269716,	// 청첩장
			SI_WEDDING_DRESS_ITEM					= 160765,	// 의상 큐브
			SI_LOVE_WORD_CHANGE_ITEM_ITEM			= 269718,	// 애칭 변경권
			SI_COUPLE_ANNIVERSARY_DAY_REWARD_ITEM	= 160762,	// 커플 기념일 큐브
			SI_WEDDING_ANNIVERSARY_DAY_REWARD_ITEM	= 160763,	// 결혼 기념일 큐브
#endif SERV_RELATIONSHIP_SYSTEM
			//}

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
			SI_FINALITY_SKILL_ITEM					= 271310,	// 궁극의 비전서
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
			SI_FINALITY_SKILL_BOOK					= 271311,	// 궁극의 비전서
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

//----------------------------------------------------------------------------------//
// 해외팀 전용 이벤트 아이템
#ifdef SERV_GLOBAL_EVENT_ITEM

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
			SI_HALLOWEEN_TRANSFORM_POSION_ITEM		= 90002300, // 펫 변신 물약
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_EVENT_GUILD_SKILL_GLOBAL
			SI_EVENT_GUILD_SKILL_5_POINT_15_DAY	= 67005930, // 일본 그노시스 15일,5SP, 거래불가
#endif //SERV_EVENT_GUILD_SKILL_GLOBAL

#ifdef NEW_YEAR_2013_EVENT
			SI_EXP_10_PERCENT_MEDAL					= 60005596,	// 아리엘의 경험치 10% 메달
#endif
#ifdef SERV_EXP_MEDAL_JP
			SI_EXP_30_PERCENT_MEDAL			= 65000720, // 경험치 30% 메달
			SI_EXP_10_PERCENT_MEDAL365		= 67004384, // 경험치 10% 메달 ( 365 일 )
			SI_EXP_10_PERCENT_MEDAL1		= 67004385, // 경험치 10% 메달 ( 1일 )
			SI_EXP_50_PERCENT_MEDAL_JP		= 60007081, // 경험치 50% 메달
			SI_EXP_30_PERCENT_MEDAL_JP		= 67006203, // 경험치 30% 메달(이벤트용)
#endif
#ifdef EVENT_GUILD_ITEM
			SI_EVENT_GUILD_MAX_MEMBER_UPGRADE		= 80001183, // 중국 전용 이벤트용 길드 인원 확장권 // 2012.06.04 lygan_조성욱
			SI_EVENT_GUILD_CASH_SKILL_POINT			= 80001181, // 중국 전용 이벤트용 아리엘의 그노시스 대축복 15일 5GSP // 2012.06.05 lygan_조성욱
#endif //EVENT_GUILD_ITEM
#ifdef EVENT_REBIRTH_RING_CN
			SI_RING_OF_PVP_REBIRTH_EVENT = 83000001, // 중국 전용 리스폰 반지
#endif //EVENT_REBIRTH_RING_CN

#ifdef SERV_VIP_SYSTEM
			SI_VIP_TICKET				= 80000115, //VIP 티켓을 가졌을때 5%의 추가 경험치
			SI_USE_FREE_BY_FIELD_VIP	= 80000117, //VIP 전용 마을이동 아이템
#endif //SERV_VIP_SYSTEM
#ifdef SERV_EVENT_VIP_SYSTEM
			SI_EVENT_VIP_TICKET				= 83000170, // 이벤트용 VIP 티켓 // 2012.11.06 lygan_조성욱
#endif //SERV_EVENT_VIP_SYSTEM
#ifdef SERV_EVENT_ITEM_US
			SI_STARTER_EXP_15_PERCENT_MEDAL	= 120000100,// 엘스타터 패키지 경험치 15% 메달
			SI_EXP_50_PERCENT_MEDAL			= 120002740,// 경험치 50% 메달
			SI_EXP_30_PERCENT_MEDAL_1DAY	= 85001942, // 경험치 30% 메달(1일권)
			SI_EXP_10_PERCENT_MEDAL_US		= 85001972, // 경험치 10% 메달
#endif SERV_EVENT_ITEM_US
#ifdef SERV_EVENT_CHARACTER_SLOT_EXPAND_ITEM
			SI_CHARACTER_SLOT_EXPAND_ITEM1	= 60000410,	// 인벤토리 사용 이벤트용 캐릭터 슬롯 확장권1
			SI_CHARACTER_SLOT_EXPAND_ITEM2	= 60000825,	// 인벤토리 사용 이벤트용 캐릭터 슬롯 확장권2
			SI_CHARACTER_SLOT_EXPAND_ITEM3	= 60001965,	// 인벤토리 사용 이벤트용 캐릭터 슬롯 확장권3
#endif SERV_EVENT_CHARACTER_SLOT_EXPAND_ITEM
#ifdef SERV_SKILL_5_POINT_7_DAY_EU
			SI_EVENT_SKILL_5_POINT_7_DAY = 90000690,
#endif SERV_SKILL_5_POINT_7_DAY_EU
#ifdef SERV_EXP_MEDAL_TW
			SI_EXP_200_PERCENT_MEDAL_TWHK	= 64000012, // 성장의 메달  경험치 200% 메달(대만)
			SI_EXP_50_PERCENT_MEDAL_TWHK	= 60007081,	// 경험치 50% 메달
#endif SERV_EXP_MEDAL_TW
#ifdef SERV_TOUR_TICKET_EVENT
			SI_TOUR_TICKET_ITEM				= 60007447,
#endif SERV_TOUR_TICKET_EVENT
#ifdef SERV_EVENT_VC
			SI_EVENT_VC_BATTLE_FOOD			= 85003041,	// 전투 식량. 사용하면 용병 뽀루 친밀도 상승
#endif //SERV_EVENT_VC
#ifdef SERV_RECRUIT_EVENT_BASE
			SI_RECRUIT_TICKET				= 90000600,
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
			SI_INVENTORY_SLOT_ADD_ITEM_EQUIP_EVENT			= 141000900,// 인벤 확장 이벤트용
			SI_INVENTORY_SLOT_ADD_ITEM_ACCESSORY_EVENT		= 141000901,
			SI_INVENTORY_SLOT_ADD_ITEM_QUICK_SLOT_EVENT		= 141000902,
			SI_INVENTORY_SLOT_ADD_ITEM_MATERIAL_EVENT		= 141000903,
			SI_INVENTORY_SLOT_ADD_ITEM_QUEST_EVENT			= 141000904,
			SI_INVENTORY_SLOT_ADD_ITEM_SPECIAL_EVENT		= 141000905,
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM

#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
			EI_THE_ESSENCE_OF_WEAK_HERETIC_POTION			= 85003830,	// 이벤트 영약 : 미약한 사파의 정수
			EI_THE_ESSENCE_OF_HERETIC_POTION				= 85003831,	// 이벤트 영약 : 사파의 정수
#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
			EI_BUFF_RURIEL_MANA_ENERGIZE_POTION				= 60008058,	// 이벤트 영약 : 루리엘의 마나 활성화 포션
#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
#endif //SERV_GLOBAL_EVENT_ITEM
//----------------------------------------------------------------------------------//
		};
		//}}

		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		enum HENIR_DUNGEON_REWARD
		{
			HDR_TIMESPACE_PIECE			= 61205, // 시공간의 조각 
		};
#endif SERV_NEW_HENIR_TEST
		//}}

		//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
#ifdef	SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP 	// 적용날짜: 2013-06-20
		enum COOLTIME_ITEM_GROUP
		{
			CIG_NONE				= 0,
			CIG_HP_POTION,
			CIG_FOOD,
			CIG_AP_POTION,
			CIG_MP_POTION,
			CIG_DRINK,
			CIG_THROW_ITEM,
			CIG_MONSTER_CARD,
			CIG_DEFENCE_MONSTER,
			CIG_DEFENCE_MONSTER_SPECIAL,

			CIG_EVENT_APPLE,
			CIG_EVENT_RECOVER_ITEM,
			CIG_SPECIA_FOOD,
			CIG_SPECIAL_RECOVER_ITEM,
			CIG_ANTI_PITION,

			CIG_FULL_RECOVER_ITEM,
			CIG_SHERBET_ITEM,
			CIG_COBO_FULL_RECOVER_ITEM,
			CIG_PILL_ITEM,
			CIG_MP_ELIXIR_ITEM,
			CIG_WEDDING_CAKE_ITEM,
			CIG_WEDDING_DRINK_ITEM,

			CIG_MAX,
		};
#else
		enum COOLTIME_ITEM_GROUP
		{
			CIG_NONE				= 0,
			CIG_HP_POTION			= 1,
			CIG_FOOD				= 2,
			CIG_AP_POTION			= 3,
			CIG_MP_POTION			= 21,
			CIG_DRINK				= 22,
			CIG_THROW_ITEM			= 41,
			CIG_MONSTER_CARD		= 101,
			CIG_DEFENCE_MONSTER		= 111,
			CIG_DEFENCE_MONSTER_SPECIAL		= 112,
		};
#endif	// SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
		enum RIDING_PET_CREATE_ITEM
		{
			RPCI_NONE	= 0,
			RPCI_1		= 1,
			RPCI_2		= 2,
			RPCI_3		= 3,
			RPCI_MAX,
		};
#endif	// SERV_RIDING_PET_SYSTM

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		enum CASH_SKILL_POINT
		{
			CSP_NONE = 0,
			CSP_30_POINT = 30,
			CSP_60_POINT = 60,
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
			CSP_130_POINT = 130,
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
		};
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

		struct SpecialAbility
		{
			DWORD		m_Type;
			int							m_CoolTime;
			int							m_Value1;
			int							m_Value2;
			int							m_Value3;

			SpecialAbility()
			{
				m_Type			= SAT_NONE;
				m_CoolTime		= 0;
				m_Value1		= 0;
				m_Value2		= 0;
				m_Value3		= 0;
			}
		};

		//{{ 2013. 05. 10	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		struct SStatRelationLevel
		{
			bool		m_bIsCommonItem;
			BYTE		m_byBaseHPRelLV;
			BYTE		m_byAtkPhysicRelLV;
			BYTE		m_byAtkMagicRelLV;
			BYTE		m_byDefPhysicRelLV;
			BYTE		m_byDefMagicRelLV;

			SStatRelationLevel()
			{
				m_bIsCommonItem		= false;
				m_byBaseHPRelLV		= 0;
				m_byAtkPhysicRelLV	= 0;
				m_byAtkMagicRelLV	= 0;
				m_byDefPhysicRelLV	= 0;
				m_byDefMagicRelLV	= 0;
			}
		};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		struct ItemTemplet
		{
			USE_TYPE					m_UseType;					// 사용 방식 장착장비인지 아닌지, 장착장비라면 어떤식으로 장착하는지
			USE_CONDITION				m_UseCondition;				// 사용 조건
			int							m_UseLevel;					// 사용 제한 레벨
			CXSLUnit::UNIT_TYPE			m_UnitType;					// 사용 unittype
			CXSLUnit::UNIT_CLASS		m_UnitClass;				// 사용 unit 클래스
			//{{ 2011. 07. 27	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
			CXSLUnit::PVP_RANK			m_BuyPvpRankCondition;		// 사용 랭크
#endif SERV_PVP_NEW_SYSTEM
			//}}

			int							m_ItemID;					//아이템 ID
			std::wstring				m_Name;						//아이템 이름
			std::wstring				m_Description;				//아이템 설명
			std::wstring				m_DescriptionInShop;		//아이템 구매전 상점에서만 보이는 설명


			//{{ dmlee 2008.11.29 
			//std::wstring				m_ModelName;				//하이퍼 모드는 _Hyper 를 붙인다
			std::wstring				m_ModelName[ MAX_MODEL_COUNT_A_ITEM ];	//하이퍼 모드는 _Hyper 를 붙인다
			//}} dmlee 2008.11.29 

			std::wstring				m_TextureChangeXETName;		//하이퍼 모드는 _Hyper 를 붙인다
			std::wstring				m_AniXETName;				//하이퍼 모드는 _Hyper 를 붙인다
			std::wstring				m_AniName;					//하이퍼 모드는 _Hyper 를 붙인다
			std::wstring				m_ShopImage;
			std::wstring				m_DropViewer;				//드롭시 사용할 파티클 시스템 이름

			ITEM_TYPE					m_ItemType;
			//{{ 2007. 10. 31  최육사  
			ITEM_GRADE					m_ItemGrade;				//아이템 등급
			//}}
			bool						m_bFashion;					//패션 아이템
			bool						m_bVested;					//귀속 아이템(양도불능/판매불능)			
			bool						m_bCanEnchant;				//강화가능 아이템인가
			bool						m_bCanUseInventory;			//인벤토리에서 사용가능한 아이템인가
			//{{ 2009. 7. 30  최육사	장착불가
			bool						m_bNoEquip;					//착용할수 없는 장비인가(교환용 장비)
			//}}
			//{{ 2010. 01. 05  최육사	PC방 프리미엄
			bool						m_bIsPcBang;				//PC방 전용 아이템
			//}}
			//{{ 2011. 10. 14	최육사	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
			int							m_iItemLevel;				// 아이템 레벨 정보
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
			//}}

			//{{ dmlee 2008.11.29 
			//std::wstring				m_AttachFrameName;			//어느 본에 붙는가
			std::wstring				m_AttachFrameName[ MAX_MODEL_COUNT_A_ITEM ];	// 어느 본에 붙는가
			//}} dmlee 2008.11.29 
			bool						m_bCanHyperMode;			//하이퍼 모드 가능 아이템인가

			PERIOD_TYPE					m_PeriodType;				//아이템 존재기간
			short						m_Endurance;				//초기내구도
			short						m_EnduranceDamageMin;		//1스테이지플레이당내구도 감소량 최소
			short						m_EnduranceDamageMax;		//1스테이지플레이당내구도 감소량 최대
			//{{ 2007. 11. 16  최육사  
			float						m_RepairED;					//내구도 1 수리당 ED
			int							m_RepairVP;					//내구도 1 수리당 VP
			//}}
			int							m_Quantity;					//1묶음수량
			
			SHOP_PRICE_TYPE				m_PriceType;				//가격타입
			int							m_Price;					//가격
			int							m_PricePvPPoint;			//구매대전포인트


			CXSLUnit::EQIP_POSITION		m_EqipPosition;				//장착 위치
			CXSLStat::Stat				m_Stat;						//아이템 스탯

            std::vector<SpecialAbility>    m_SpecialAbilityList;
			//{{ dmlee 2008.11.14 아이템 고유속성에 소켓옵션 추가
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			std::vector< int >				m_vecSocketOption;
#else
			std::vector< short >			m_vecSocketOption;
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			//}} dmlee 2008.11.14 아이템 고유속성에 소켓옵션 추가

			//{{ 2013. 05. 10	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			std::vector< int >			m_vecRandomSocketGroupID;
			SStatRelationLevel			m_kStatRelationLevel;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}

			int							m_CoolTime;
			int							m_SetID;

			//{{ 2009. 9. 2  최육사		봉인
			BYTE						m_ucMaxSealCount;			// 최대로 봉인할 수 있는 횟수
			//}}
			//{{ 2009. 11. 17  최육사	속성강화가능갯수
			int							m_iMaxAttribEnchantCount;
			//}}

			//{{ 2012. 06. 28	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
			short						m_iAttributeLevel;			// 요구 속성 레벨
#endif SERV_2012_PVP_SEASON2_1
			//}}

			//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
			int							m_iBuffFactorID;			// 버프 팩터 id
#endif SERV_SERVER_BUFF_SYSTEM
			//}

#ifdef SERV_RESTRICTED_TO_MOVE_TO_BANK
			bool						m_bInventoryOnly;			// 은행, 펫인벤 이동제한
#endif SERV_RESTRICTED_TO_MOVE_TO_BANK

			ItemTemplet()
			{
				m_UseType			= UT_NONE;		// 사용 방식 장착장비인지 아닌지, 장착장비라면 어떤식으로 장착하는지
				m_UseCondition		= UC_NONE;	// 사용 조건
				m_UseLevel			= 0;						// 사용 제한 레벨
				m_UnitType			= CXSLUnit::UT_NONE;		// 사용 unittype
				m_UnitClass			= CXSLUnit::UC_NONE;		// 사용 unit 클래스
				//{{ 2011. 07. 27	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
				m_BuyPvpRankCondition = CXSLUnit::PVPRANK_NONE; // 사용 랭크
#endif SERV_PVP_NEW_SYSTEM
				//}}
				
				m_ItemID			= -1;						//아이템 ID

				m_ItemType			= IT_NONE;
				//{{ 2007. 10. 31  최육사  
				m_ItemGrade			= IG_NONE;					//아이템 등급
				//}}
				m_bFashion			= false;					//패션 아이템
				m_bVested			= false;					//귀속 아이템(양도불능/판매불능)				
				m_bCanEnchant		= false;					//강화가능 아이템인가
				m_bCanUseInventory	= false;					//인벤토리에서 사용가능한 아이템인가
				m_bNoEquip			= false;					//장착불가
				//{{ 2010. 01. 05  최육사	PC방 프리미엄
				m_bIsPcBang			= false;					//PC방 전용 아이템
				//}}
				//{{ 2011. 10. 14	최육사	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
				m_iItemLevel		= 0;						// 아이템 레벨 정보
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
				//}}
				
				m_bCanHyperMode		= false;					//하이퍼 모드 가능 아이템인가

				m_PeriodType		= PT_INFINITY;				//아이템 존재기간
				m_Endurance			= -1;						//초기내구도
				m_EnduranceDamageMin	= 0;					//1스테이지플레이당내구도 감소량
				m_EnduranceDamageMax	= 0;					//1스테이지플레이당내구도 감소량
				//{{ 2007. 11. 16  최육사  
				m_RepairED			= 0.0f;						//내구도 수리 ED
				m_RepairVP			= 0;						//내구도 수리 VP
				//}}
				m_Quantity			= 0;						//1묶음수량
				
				m_PriceType			= SPT_NONE;					//가격타입
				m_Price				= 0;						//가격
				m_PricePvPPoint		= 0;						//구매대전포인트
				
				m_UnitType			= CXSLUnit::UT_NONE;		//장착 유닛
				m_UnitClass			= CXSLUnit::UC_NONE;		//장착 클래스

				m_EqipPosition		= CXSLUnit::EP_NONE;		//장착 위치

				m_CoolTime			= 0;
				m_SetID				= 0;

				//{{ 2009. 9. 2  최육사		봉인
				m_ucMaxSealCount	= 0;						// 최대로 봉인할 수 있는 횟수
				//}}
				//{{ 2009. 11. 17  최육사	속성강화가능갯수
				m_iMaxAttribEnchantCount = 0;
				//}}		

				//{{ 2012. 06. 28	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
				m_iAttributeLevel	= 0;						// 요구 속성 레벨
#endif SERV_2012_PVP_SEASON2_1
				//}}

				//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
				m_iBuffFactorID		= 0;
#endif SERV_SERVER_BUFF_SYSTEM
				//}
#ifdef SERV_RESTRICTED_TO_MOVE_TO_BANK
				m_bInventoryOnly	= false;
#endif SERV_RESTRICTED_TO_MOVE_TO_BANK
			}

			//{{ 2013. 05. 20	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			bool IsCommonItem() const		{ return m_kStatRelationLevel.m_bIsCommonItem; }
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
		};

		struct ItemData
		{
			UidType				m_ItemUID;				//UID
			int					m_ItemID;

			PERIOD_TYPE         m_PeriodType;
			short				m_Endurance;			//내구도
			int					m_Quantity;				//수량

			int					m_EnchantAttribute;
			int					m_EnchantLevel;

			ItemData()
			{
				m_ItemUID				= 0;				//UID
				m_ItemID				= 0;

				m_PeriodType            = PT_INFINITY;
				m_Endurance				= -1;				//내구도
				m_Quantity				= 0;				//수량

				m_EnchantAttribute		= 0;
				m_EnchantLevel			= 0;
			}
			ItemData& operator=( const KItemInfo& data )
			{
//				m_ItemUID				= data.m_iItemUID;					//UID
				m_ItemID				= data.m_iItemID;

				m_PeriodType            = (CXSLItem::PERIOD_TYPE)data.m_cUsageType;
				//m_Endurance				= data.m_iEndurance;				//내구도
				m_Quantity				= data.m_iQuantity;					//수량

				//m_EnchantAttribute		= data.m_iEType;
				//m_EnchantLevel			= data.m_iELevel;

				return *this;
			}

			void MakePacket( KItemInfo& itemInfo )
			{
//				itemInfo.m_iItemUID				= m_ItemUID;				//UID
				itemInfo.m_iItemID				= m_ItemID;

				itemInfo.m_cUsageType           = m_PeriodType;
				//itemInfo.m_iEndurance			= m_Endurance;			//내구도
				itemInfo.m_iQuantity			= m_Quantity;			//수량

				//itemInfo.m_iEType				= m_EnchantAttribute;
				//itemInfo.m_iELevel				= m_EnchantLevel;
			}
		};

	public:
		CXSLItem( ItemData* pItemData );
		~CXSLItem(void);

		ItemData*		GetItemData()			{ return m_pItemData; }
		ItemTemplet*	GetItemTemplet()		{ return m_pItemTemplet; }

		UidType			GetUID()				{ return m_pItemData->m_ItemUID; }
		void			SetEqip( bool bEqip )	{ m_bEqip = bEqip; }
		bool			GetEqip()				{ return m_bEqip; }

	public:
		//{{ 2009. 7. 28  최육사	GM무기
		static	bool	IsGMweapon( const int iItemID );
		//}}
		//{{ 2009. 9. 22  최육사	전직캐쉬
		static	char	GetCashItemChangeUnitClass( const int iItemID );
		static  int		GetCashItemByUnitClass( const char cUnitClass );
		//}}
		//{{ 2009. 12. 14  최육사	서버코드정리
		static	bool	IsJobChangeCashItem( const int iItemID ) { return ( GetCashItemChangeUnitClass( iItemID ) != CXSLUnit::UC_NONE ); }
		//}}
		//{{ 2010. 04. 26  최육사	PC방 전용 캐쉬템
		static	bool	IsPcBangOnlyCashItem( const int iItemID );
		//}}

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		static	bool	IsKeepItemShowItem( const int iItemID );
		static	bool	IsKeepItemShowItem2( const int iItemID );
		static	bool	IsKeepItemShowItem3( const int iItemID );
		static	bool	IsKeepItemShowItem4( const int iItemID );
		static	bool	IsKeepItemShowItem5( const int iItemID );
		static	bool	IsKeepItemShowItem6( const int iItemID );
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifndef DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM //캐시샾 찜하기에서 전직 아이템에 대해 예외처리 해제.
		//{{ 2010. 03. 30  김정협 전직아이템인지 확인하는 함수
		static	bool	IsChangeJobItem( const int iItemID );
#endif //DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM

		static	bool	IsBankExpandItem( const int iItemID );
		//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
		static	bool	IsItemToMakeSkillPageExpanded( const int iItemID_ );
#endif // SERV_SKILL_PAGE_SYSTEM


		//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		static	int		GetPShopAgencyDays( IN const int iItemID );
        static  int     GetPShopAgencyType( IN const int iItemID_ );

#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 06. 01	최육사	강화된 아이템 보상 큐브
#ifdef SERV_ENCHANTED_ITEM_CUBE_REWARD
		//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		static	bool	UpdateEnchantedItemCubeReward( IN const int iCubeItemID, IN OUT KItemInfo& kInsertItemInfo );
#else
		static	void	UpdateEnchantedItemCubeReward( IN const int iCubeItemID, IN OUT KItemInfo& kInsertItemInfo );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
#endif SERV_ENCHANTED_ITEM_CUBE_REWARD
		//}}

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		static	int		GetComeBackRewardSkillPointDays( IN const int iItemID );
#endif SERV_COME_BACK_USER_REWARD
		//}}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		
		//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
		static	int		GetClassChangeCashItem( const int iItemID );
		static	bool	IsClassChangeCashItem( const int iItemID ) { return ( GetClassChangeCashItem( iItemID ) != CXSLUnit::UC_NONE ); }
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
		static int		GetClassChangeCashItemIDByUnitClass( const int iUnitClass_ );
#endif SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
#endif SERV_UNIT_CLASS_CHANGE_ITEM
		//}}

		//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
		static	bool	IsPetNameChangeCashItem( const int iItemID ) { return iItemID == CXSLItem::CI_PET_NAME_CHANGE; }
#endif SERV_PET_CHANGE_NAME
		//}}
#ifdef SERV_WISH_ITEM_DISABLE
		static const bool	IsPossibleCart( const int iItemID_ );
#endif SERV_WISH_ITEM_DISABLE

#ifdef SERV_GOLD_TICKET_SEAL_COUNT
		static bool		IsGoldTicket( const int iItemID );
#endif SERV_GOLD_TICKET_SEAL_COUNT

	private:
		ItemData*		m_pItemData;
		ItemTemplet*	m_pItemTemplet;

		bool			m_bEqip;
};



