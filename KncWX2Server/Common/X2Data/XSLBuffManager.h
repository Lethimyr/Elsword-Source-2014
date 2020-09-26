#pragma once

#include "RefreshSingleton.h"
#include "X2Data/XSLMain.h"
#include <map>
#include <vector>


class CXSLBuffManager
{
	DeclareRefreshSingleton( CXSLBuffManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	/*
	버프 아이디는 DB에 기록되는 정보이기 때문에, 변경할 때는 이 점을 유의하셔야 합니다.
	*/
	enum BUFF_TEMPLET_ID
	{
		BTI_NONE						= 0,
		
		// 비약
		BTI_BUFF_PROTECTION_OF_NUT 			= 109,	// 누트의 가호
		BTI_BUFF_PROTECTION_OF_PTAH 		= 110,	// 프타의 가호
		BTI_BUFF_PROTECTION_OF_LAHEL 		= 111,	// 라헬의 가호
		BTI_BUFF_PROTECTION_OF_ZACHIEL 		= 112,	// 자키엘의 가호
		
		// 영약
		BTI_BUFF_BELSSING_OF_SERAPHIM 		= 113,	// 세라핌의 축복
		BTI_BUFF_BELSSING_OF_GEB 			= 114,	// 게브의 축복
		BTI_BUFF_BELSSING_OF_AMON 			= 115,	// 아몬의 축복
		BTI_BUFF_BELSSING_OF_CRONOS 		= 116,	// 크로노스의 축복
		
		BTI_BUFF_STRENGTHENING_BODY			= 117,	// 강화된 신체 ( 비약 디버프 )
		
		BTI_BUFF_RALLY_OF_HERO_LEVEL_1		= 118,	// 영웅의 집결
		BTI_BUFF_RALLY_OF_HERO_LEVEL_2		= 119,	// 영웅의 집결
		BTI_BUFF_RALLY_OF_HERO_LEVEL_3		= 120,	// 영웅의 집결
		
		BTI_DEBUFF_REST_OF_RELLY			= 121,	// 재집결을 위한 휴식
		
		BTI_BUFF_WARM_ENERGE_OF_EL			= 122,	// 편안한 휴식( 마을 )
		
		BTI_BUFF_THANKS_OF_RESIDENTS		= 123,	// 주민의 감사
		BTI_BUFF_BREATH_OF_DRAGON			= 124,	// 드래곤의 숨결
		BTI_BUFF_BLESSING_OF_EL				= 125,	// 엘의 축복
		
		BTI_BUFF_HERO_OF_ELIOS_LEVEL_1		= 126,	// 엘리오스의 수호 영웅 레벨 1
		BTI_BUFF_HERO_OF_ELIOS_LEVEL_2		= 127,	// 엘리오스의 수호 영웅 레벨 2
		BTI_BUFF_HERO_OF_ELIOS_LEVEL_3		= 128,	// 엘리오스의 수호 영웅 레벨 3
		BTI_BUFF_HERO_OF_ELIOS_LEVEL_4		= 129,	// 엘리오스의 수호 영웅 레벨 4
		
		BTI_BUFF_PVP_REVENGE_MODE_LEVEL_1	= 130,	// 리벤지 모드 레벨 1
		BTI_BUFF_PVP_REVENGE_MODE_LEVEL_2	= 131,	// 리벤지 모드 레벨 2
		BTI_BUFF_PVP_REVENGE_MODE_LEVEL_3	= 132,	// 리벤지 모드 레벨 3
		
		BTI_BUFF_RETURN_OF_HERO				= 133,	// 귀환자 버프

		BTI_BUFF_PREMIUM_PC_ROOM			= 138,	// PC 방 버퍼

		BTI_BUFF_PET_AURA_SKILL 			= 139,	// 팻 격려 스킬
		BTI_BUFF_MECHANIZATION_SPEED_UP		= 140,	// 기계화 셋트 스피드 버프

		BTI_SI_SA_EEG_ATOMIC_SHIELD			= 141,  // 아토믹 쉴드 디버프

		BTI_DEBUFF_POWERFUL_BOWSTRING		= 142,  // 그랜드아쳐 강화된 보우 스트링의 피격자 디버프

		BTI_BUFF_SUPER_ARMOR				= 143, 	// 슈퍼 아머 버프

		BTI_BUFF_WALDO_TROCK				= 144, 	// 거대한 왈도가 사용하는 버프
		BTI_BUFF_BUGI_TROCK					= 145, 	// 버기 트락이 사용하는 버프
		BTI_DEBUFF_SPRIGGAN_ENTANGLE		= 146, 	// 묶임(인탱글) 스프리건이 사용하는 인탱글

		BTI_BUFF_HENIR_FIRE					= 147,	// 헤니르 버프 - 불의 샘
		BTI_BUFF_HENIR_WATER				= 148,	// 헤니르 버프 - 불의 샘
		BTI_BUFF_HENIR_NATURE				= 149,	// 헤니르 버프 - 땅의 샘
		BTI_BUFF_HENIR_WIND					= 150,	// 헤니르 버프 - 바람의 생
		BTI_BUFF_HENIR_LIGHT				= 151,	// 헤니르 버프 - 빛의 샘
		BTI_DEBUFF_SHADOW_JAIL				= 152,	// 그림자 감옥 디버프
		BTI_DEBUFF_SHADOW_KNOT				= 153,	// 그림자 매듭 디버프

		BTI_EMPTY_EXP_BUFF				    = 154,	// 아라와 함께 플레이하면 경험치 보너스

		BTI_BUFF_FIELD_DEFENCE				= 187,	// 배틀필드 이벤트 버프(경치 +30%, ED +30%)

		BTI_BUFF_APRIL_FOOLSDAY				= 188,	// 만우절 레인보우 버프

		BTI_BUFF_GATE_DEFENCE				= 189,	// 어둠의 문 벨런스 설정용 방어력 증가 버프
		BTI_BUFF_ENEMY_GATE_DEFENCE			= 190,	// 어둠의 문 벨런스 설정용 마족 소환문 방어력 증가 버프

		BTI_BUFF_ELIXIR_GIANT_POTION		= 191,	// 어둠의 문 영약 : 거인화 물약
		BTI_BUFF_ELIXIR_BLAZING_BOMB		= 192,	// 어둠의 문 영약 : 이글이글 폭약
		BTI_BUFF_ELIXIR_SPIRIT_OF_CHASER	= 193,	// 어둠의 문 영약 : 추적자의 혼
		BTI_BUFF_ELIXIR_CRADLE_OF_LITTLE_FAIRY	= 194,	// 어둠의 문 영약 : 아기 요정 요람
		BTI_BUFF_ELIXIR_ICE_BALL_OF_DENIPH	= 195,	// 어둠의 문 영약 : 데니프의 얼음 구슬
		BTI_BUFF_ELIXIR_FEATHER_OF_VENTUS	= 196,	// 어둠의 문 영약 : 벤투스의 날개깃
		BTI_BUFF_ELIXIR_FLAME_RING_OF_ROSSO	= 197,	// 어둠의 문 영약 : 로쏘의 화염고리
		BTI_BUFF_ELIXIR_BIG_HAND_POTION		= 198,	// 어둠의 문 영약 :거대손 물약

		BTI_BUFF_2013_DEFENSE_FAKE_BUFF		= 201,	// 2013 어둠의 문 참여자 가짜 버프
		BTI_BUFF_2013_DEFENSE_BUFF			= 202,	// 2013 어둠의 문 참여자 진짜 버프
		BTI_BUFF_2013_DEFENSE_DEBUFF		= 203,	// 2013 어둠의 문 월드 디버프
		BTI_BUFF_2013_DEFENSE_BUFF_EVENT	= 204,	// 이벤트용 2013 어둠의 문 참여자 버프

		BTI_FESTIVAL_EVENT_BUFF				= 207,	// 공존의 축제 이벤트 버프,  (물리,마법 공격력 10% 증가)

		BTI_BUFF_TRADE_BLOCK                = 213,   // 해외 전용 신규 캐릭터 거래 블럭 버프

#ifdef SERV_CRAYON_POP_EVENT_BUFF// 작업날짜: 2013-06-10	// 박세훈
		BTI_BUFF_ELRIN						= 215,	// 크래용팝 엘린
		BTI_BUFF_SOYUL						= 216,	// 크래용팝 소율 
		BTI_BUFF_KUMI						= 217,	// 크래용팝 금미
		BTI_BUFF_CHOA						= 218,	// 크래용팝 초아
		BTI_BUFF_WEY						= 219,	// 크래용팝 웨이
#endif // SERV_CRAYON_POP_EVENT_BUFF

//#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
		BTI_BUFF_2013_DEFENCE_ENTER_100_PERCENT_BUFF = 300,
//#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT

		BTI_DEBUFF_RAINCOAT_CAT_STEB1_ATTACK_A = 301,                
		BTI_DEBUFF_RAINCOAT_CAT_STEB2_ATTACK_A = 302,                 
		BTI_DEBUFF_RAINCOAT_CAT_STEB3_ATTACK_A = 303,                  

		BTI_DEBUFF_ANCIENT_FIRE							= 304,	

		BTI_BUFF_WHEN_RIDE_ON_PET_FOR_ONLY_MASTER_FIX		= 305,	

		BTI_BUFF_MADNESS_SOUP							= 306,  
		BTI_BUFF_BIG_HEAD_SOUP							= 307,
		BTI_BUFF_HP_UP_SOUP								= 308,
		BTI_BUFF_MP_UP_SOUP								= 309,
		BTI_BUFF_SPEED_UP_SOUP							= 310,
		BTI_BUFF_ATTACK_UP_SOUP							= 311,

//#ifdef SERV_BONUS_BUFF_SYSTEM
		BTI_BUFF_2YEARS_EVENT_CN_DAMAGE_UP			= 312,
		//BTI_BUFF_2YEARS_EVENT_CN_MOVE_JUMP_SPEED_UP = 313, // 추후 재 사용시 이넘 수정 필요 // 필드 보스 재입장 불가 디버프랑 겹침.
		BTI_BUFF_2YEARS_EVENT_CN_ACCURACY_AVOID_UP	= 314,
		BTI_BUFF_2YEARS_EVENT_CN_CRI_SPEED_UP		= 315,
		BTI_BUFF_2YEARS_EVENT_CN_DAMAGE_INC_DEC_UP	= 316,
		BTI_BUFF_2YEARS_EVENT_CN_ALL_IN_ONE			= 317,
//#endif //SERV_BONUS_BUFF_SYSTEM

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		BTI_DEBUFF_REST_OF_RECHALLENGE		= 313,	// 필드 보스 시스템 재입장 불가 디버프
#endif // SERV_BATTLE_FIELD_BOSS
		BTI_SECRET_OF_THOUSAND_YEARS_FOX				= 327,  //SERV_BUFF_BONUSRATE_HAMEL_EVENT 하멜 이벤트 버프 1.2배
		
		BTI_BUFF_2013_CHRISTMAS				= 330,	// itexpertkim // 김현철 // 2013-12-17
		BTI_BUFF_NAVER						= 339,	// 네이버채널링버프, 물리/마법공격력 3%, 물리/마법방어 3%, MP 최대치 10% 증가
//#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
		BTI_BUFF_THE_ESSENCE_OF_WEAK_HERETIC_POTION	= 1318,
		BTI_BUFF_THE_ESSENCE_OF_HERETIC_POTION		= 1319,
//#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC

//#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
		BTI_BUFF_RURIEL_MANA_ENERGIZE_POTION		= 1320,
//#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
	};

	enum UNIT_BUFF_KEEP_TIME
	{
		UBKT_UNIT_RESET_TIME_HOUR = 6,				// 6시간 마다 초기화 - 시간
		UBKT_UNIT_RESET_TIME_SECOND = 21600,		// 6시간 마다 초기화 - 초
	};

	enum BUFF_ATTRIBUTE
	{
		BA_MULTIPLY_ATTACK_STAT = 0,
		BA_MULTIPLY_DEFENCE_STAT,
		BA_MULTIPLY_HP,
	};

	struct BuffTemplet
	{
		BUFF_TEMPLET_ID			m_eBuffID;
		std::wstring			m_wstrBuffName;
		std::vector< int >		m_vecOption;
		double					m_fBuffPeriodSecond;

		//{{ 2012. 05. 6	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		bool					m_bEndBuffAtTheLeaveDungeonRoom;
		bool					m_bEndBuffAtTheEndDungeonGame;

		//{{ 2012. 07. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_2
		bool					m_bEndBuffAtTheLeavePvpRoom;
		bool					m_bEndBuffAtTheEndPvpGame;
#endif SERV_2012_PVP_SEASON2_2
		//}}

		bool					m_bEndBuffAtTheUserDied;
		bool					m_bKeepBuffAtTheUserLogOut;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}

#ifdef SERV_SERVER_BUFF_SYSTEM// 작업날짜: 2013-05-14	// 박세훈
		bool					m_bEndBuffAtTheLeaveBattleFieldRoom;
#endif // SERV_SERVER_BUFF_SYSTEM
		
		//{{ 2012. 04. 17	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		std::vector< int >		m_vecRoomType;
		bool CheckRoomType( IN const int& iCheckRoomType ) const;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}

		//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
		int						m_iFactorID;
#endif SERV_SERVER_BUFF_SYSTEM
		//}}
		
		BuffTemplet()
		{
			m_eBuffID = BTI_NONE;
			m_fBuffPeriodSecond = 0.0f;

			//{{ 2012. 05. 6	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
			m_bEndBuffAtTheLeaveDungeonRoom	= false;
			m_bEndBuffAtTheEndDungeonGame	= false;

			//{{ 2012. 07. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_2
			m_bEndBuffAtTheLeavePvpRoom		= false;
			m_bEndBuffAtTheEndPvpGame		= false;
#endif SERV_2012_PVP_SEASON2_2
			//}}

			m_bEndBuffAtTheUserDied			= false;
			m_bKeepBuffAtTheUserLogOut		= false;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
			//}}

#ifdef SERV_SERVER_BUFF_SYSTEM// 작업날짜: 2013-05-14	// 박세훈
			m_bEndBuffAtTheLeaveBattleFieldRoom	= false;
#endif // SERV_SERVER_BUFF_SYSTEM

			//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
			m_iFactorID						= 0;
#endif SERV_SERVER_BUFF_SYSTEM
			//}
		}
	};

public:
	CXSLBuffManager(void);
	~CXSLBuffManager(void);

	// for lua
	bool AddBuffInfo_LUA();

	const BuffTemplet* GetBuffTemplet( IN const int iBuffID ) const;
	bool IsExistTitleID( IN const int iBuffID ) const		{ return ( GetBuffTemplet( iBuffID ) != NULL ); }

private:
	std::map< BUFF_TEMPLET_ID, BuffTemplet >		m_mapBuffList;

};

DefRefreshSingletonInline( CXSLBuffManager );



