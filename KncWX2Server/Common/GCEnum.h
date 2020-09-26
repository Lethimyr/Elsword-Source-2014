#ifndef _GCENUM_H_
#define _GCENUM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// @enum EGCGameMode 그랜드체이스 게임 모드
enum EGCGameMode // GC_GM_*
{
	GC_GM_INVALID				= -1,       ///< invalid game mode value

    // 대전
    GC_GM_TEAM_MODE				= 0,        ///< 팀 모드
    GC_GM_SURVIVAL_MODE			= 1,        ///< 서바이벌 모드

    // 길드전
    GC_GM_GUILD_BATTLE			= 2,		///< 길드전 모드

    // 전직 미션
    GC_GM_ANGEL_RESCUE			= 3,		///< 천사 구출 작전
    GC_GM_ARCHER_CONTEST		= 4,		///< 여왕배 궁수 대회
    GC_GM_GADOSEN_DEFEAT		= 5,		///< 가도센 퇴치 작전

    // 몬스터 원정대
    GC_GM_MONSTER_CRUSADERS		= 6,		///< 몬스터 원정대

    // 몬스터 사냥
    GC_GM_MONSTER_HUNT			= 7,        ///< 몬스터 사냥

	GC_GM_ARENA_TEAM			= 8,		///< 아레나배틀 팀모드
	GC_GM_ARENA_SURVIVAL		= 9,		///< 아레나배틀 서바이벌모드

    // 퀘스트
    GC_GM_QUEST1				= 10,       ///< 퀘스트1
    GC_GM_QUEST2				= 11,       ///< 퀘스트2	
    GC_GM_QUEST3				= 12,       ///< 퀘스트3
    GC_GM_QUEST4				= 13,       ///< 퀘스트4
    GC_GM_QUEST5				= 14,       ///< 퀘스트5
	GC_GM_QUEST6				= 15,       ///< 퀘스트FINAL

	GC_GM_QUEST1_1_VVVIC		= 16,		///< 퀘스트1_1 퀴니
	GC_GM_QUEST1_2_VVVIC		= 17,		///< 퀘스트1_2 퀴니
	GC_GM_QUEST2_1_VVVIC		= 18,		///< 퀘스트2_1 퀴니
	GC_GM_QUEST2_2_VVVIC		= 19,		///< 퀘스트2_2 퀴니
	GC_GM_QUEST3_1_VVVIC		= 20,
	GC_GM_QUEST3_2_VVVIC		= 21,
	GC_GM_QUEST4_1_VVVIC		= 22,
	GC_GM_QUEST4_2_VVVIC		= 23,
	GC_GM_QUEST6_1_VVVIC		= 24,
	GC_GM_QUEST6_2_VVVIC		= 25,

	GC_GM_QUEST_ZERO			= 26,		///< 퀘스트_ZERO
    GC_GM_QUEST_ZERO_2			= 27,		///< 퀘스트_ZERO_2

	GC_GM_TAG_TEAM				= 28,		///< 태그매치 팀모드
	GC_GM_TUTORIAL				= 29,		///< 튜토리얼

    NUM_GC_GAME_MODE,           ///< 게임 모드 총 개수
};

/// @enum EGCGameMode 그랜드체이스 게임 모드 카테고리
enum EGCGameModeCategory // GC_GMC_*
{
    GC_GMC_MATCH,             ///< 대전             0
    GC_GMC_GUILD_BATTLE,      ///< 길드전           1
    GC_GMC_PROMOTION_MISSION, ///< 전직 미션        2
    GC_GMC_MONSTER_CRUSADERS, ///< 몬스터 원정대    3
    GC_GMC_MONSTER_HUNT,      ///< 몬스터 사냥      4
    GC_GMC_QUEST,             ///< 퀘스트           5
	GC_GMC_ARENABATTLE,       ///< 아레나배틀       6
	GC_GMC_TAG_MATCH,         ///< 태그 매치		7
	GC_GMC_TUTORIAL,		  ///< 튜토리얼			8
	
	GC_GMC_COUNT,			  ///< 모드카테고리 개수
    GC_GMC_INVALID           = -1,                      ///< invalid game mode category value
};

/// @enum EGCGameModeType 그랜드체이스 게임 모드 타입
enum EGCGameModeType
{
    GC_GMT_ALONE_FIGHT,     ///< 각자 싸우는 타입
    GC_GMT_TEAM_FIGHT,      ///< 팀을 이루어 싸우는 타입
    GC_GMT_UNION_FIGHT,     ///< 연합하여 싸우는 타입

    NUM_GC_GAME_MODE_TYPE,  ///< 게임 모드 타입 총 개수

    GC_GMT_INVALID  = -1,   ///< invalid game mode type value
};

/// @enum EGCSubGameMode 그랜드체이스 서브 게임 모드
enum EGCSubGameMode // GC_SGM_*
{
    GC_SGM_NORMAL,          ///< 일반 게임 모드(아이템전)
    GC_SGM_NO_ITEM,         ///< 노아이템전 게임 모드

	GC_SGM_QUEST_LEVEL1,
	GC_SGM_QUEST_LEVEL2,
	GC_SGM_QUEST_LEVEL3,
	GC_SGM_QUEST_LEVEL4,
	GC_SGM_QUEST_LEVEL5,

    NUM_GC_SUB_GAME_MODE,   ///< 서브 게임 모드 총 개수

    GC_SGM_INVALID  = -1,   ///< invalid sub game mode value
};
enum EGCObjType
{
    GC_OBJ_DEFAULT      = 0,
    GC_OBJ_MON          = 1,
    GC_OBJ_TARGET       = 2,        //이 Type은 꼭 부셔야지 넘어갈 수 있다는 Type이다.
    GC_OBJ_END,
};
enum EGCRenderLayer
{
    GC_LAYER_SKY                    = 0,
    GC_LAYER_BETWEEN_SKY_BLOCK      = 1,
    GC_LAYER_BLOCK                  = 2,
    GC_LAYER_BETWEEN_BLOCK_CHAR     = 3,
    GC_LAYER_CHAR                   = 4,
    GC_LAYER_BETWEEN_CHAR_FRONT     = 5,
    GC_LAYER_FRONT_MAP              = 6,
    GC_LAYER_FACE                   = 7,    // 필살기 쓸때 나오는 얼굴.
    GC_LAYER_UI                     = 8,
    GC_LAYER_END,
};
/// @enum EGCMonster 그랜드체이스 몬스터
enum EGCMonster // MON_*
{
    MON_GORGOS          = 0,    ///< 고르고스
    MON_TROLL           = 1,    ///< 트롤
    MON_HARPY           = 2,    ///< 하피
    MON_MUSHROOM        = 3,    ///< 버섯몬(Normal)
    MON_MUSHROOM_S      = 4,    ///< 버섯몬(Small)
    MON_DRILLMON        = 5,    ///< 드릴몬
    MON_SCARECROW       = 6,    ///< 허수아비
    MON_PARADOM         = 7,    ///< 파라돔
    MON_ANGEL           = 8,    ///< 천사
    MON_TARGET          = 9,    ///< 화살과녁
    MON_GADOSEN         = 10,   ///< 가도센
    MON_DRAGON          = 11,   ///< 바실리스크
	MON_CANNON          = 12,   ///< 대포
    MON_WOOD_DOOR       = 13,   ///< 나무문
    MON_IRON_DOOR       = 14,   ///< 철문
    MON_TREASURE_BOX    = 15,   ///< 보물상자
    MON_SOLDIER1_A      = 16,   ///< 병사1-A
    MON_SOLDIER1_B      = 17,   ///< 병사1-B
    MON_SOLDIER2_A      = 18,   ///< 병사2-A
    MON_SOLDIER2_B      = 19,   ///< 병사2-B
    MON_NEW_GORGOS      = 20,   ///< 레드고르고스
    MON_SANTA           = 21,   ///< 산타몬
	MON_MONGBAN_A		= 22,	///< 퀘스트2 문지기A
	MON_KAIDO			= 23,	///< 퀘스트2 카이도(법사)
	MON_MONGBAN_B		= 24,	///< 퀘스트2 문지기B
	MON_GAIKOZ			= 25,	///< 퀘스트2 가이코즈
	//가이코즈보다 번호가 낮은 몬스터는 절대로 추가하지 말 것!!!
	//이후의 몬스터 추가는 무조건 이 뒷번호로... 파일명 꼬인다...
	MON_LAPSTER			= 26,	///< 퀘스트3 아모어랍스터
	MON_GHOST_FISH		= 27,	///< 퀘스트3 귀신아귀
	MON_GHOST_FISH_B	= 28,	///< 퀘스트3 귀신아귀B
	MON_PATUSEI_HEAD	= 29,	///< 파투세이 머리
	MON_PATUSEI_BODY	= 30,	///< 파투세이 몸
	MON_PATUSEI_LEG		= 31,	///< 파투세이 다리
	
	MON_ELLENA			= 32,	///< 엘레나
	MON_KAMIKI			= 33,	///< 카미키
	MON_SANGSUN			= 34,	///< 상선씨
	MON_KAZEAZE         = 35,   ///< 카제아제
	MON_KAKIRU_HEAD     = 36,	///< 카키이루 머리
	MON_KAKIRU_LEFT     = 37,	///< 카키이루 왼팔
	MON_KAKIRU_RIGHT    = 38,	///< 카키이루 오른팔
	MON_KAKIRU_DOOR     = 39,   ///< 카키이루 문
	MON_CASTLE_DOOR		= 40,	///< 퀘스트Final 성문
	MON_TRANS_KAZEAZE   = 41,   ///< 변신카제아제
	MON_LAS			    = 42,	///< 신캐릭터 : 도적(퀘스트_FINAL)에 처음으로 등장할때 사용함
	MON_NEW_SCARECROW   = 43,
	MON_RACOON			= 44,	///< 너구리
	MON_SLIM			= 45,	///< 슬라임
	MON_ENT				= 46,	///< 앤트
    MON_H_GOBLIN        = 47,   ///< 홉 고블린
    MON_WENDY           = 48,   ///< 웬디
    MON_ORC             = 49,   ///< 오크
    MON_GOBLIN          = 50,   ///< 고블린
	MON_MINI_INSECTIVOROUS	= 51, ///< 미니 식충 식물
	MON_INSECTIVOROUS_RIGHT	= 52, ///< 거대 식충 식물 ( 오른쪽 )
	MON_INSECTIVOROUS_LEFT	= 53, ///< 거대 식충 식물 ( 왼쪽 )
    MON_WOODPILLAR          = 54, ///< 나무기둥
    MON_CRYSTALGEM          = 55, ///< 수정구슬
	MON_SOCCERBALL_PARADOM  = 56, ///< 축구공 파라돔
	MON_NEW_CANNON			= 57, ///< 대포
	MONSTER_MODEL_NUM,          ///< 몬스터 모델 개수

    MON_INVALID         = -1,   ///< invalid monster value
};

/// @enum EGCBackgroundMusic 그랜드체이스 배경 음악
enum EGCBackgroundMusic
{
    GC_BGM_MAIN_THEME           = 0,    ///< Track0     "메인 테마"
    GC_BGM_NEW_WORLD            = 1,    ///< Track1     "신세계"
    GC_BGM_MONSTER_ATTACK       = 2,    ///< Track2     "몬스터 습격"
    GC_BGM_GAME_WIN             = 3,    ///< Track3     "게임 승리"
    GC_BGM_GAME_LOSE            = 4,    ///< Track4     "게임 패배"
    GC_BGM_LETS_GO              = 5,    ///< Track5     "Let's go!"
    GC_BGM_LEGEND               = 6,    ///< Track6     "전설"
    GC_BGM_CASTLE               = 7,    ///< Track7     "성"
    GC_BGM_X_MAS                = 8,    ///< Track8     "크리스마스"
    GC_BGM_MAIN_THEME_LYRICS    = 9,    ///< Track9     "메인 테마 (가사)"
    GC_BGM_ANGEL                = 10,   ///< Track10    "천사"
    GC_BGM_VILLAGE              = 11,   ///< Track11    "마을"
    GC_BGM_INTO_THE_DARKNESS    = 12,   ///< Track12    "어둠 속으로"
    GC_BGM_ALLIANCE             = 13,   ///< Track13    "동맹"
    GC_BGM_GAME_START           = 14,   ///< Track14    "게임 시작"
    GC_BGM_LETS_DO_IT           = 15,   ///< Track15    "Let's do it!"
    GC_BGM_GORGOSLAIR           = 16,   ///< Track16    "고르고스의 동굴"
    GC_BGM_XMAS_EVENT           = 17,   ///< Track17    "XMAS이벤트"
	GC_BGM_RUN                  = 18,   ///< Track18    "질주"
	GC_BGM_LOGO                 = 19,   ///< Track19    "로고"
	GC_BGM_QFINAL               = 20,   ///< Track20    "퀘스트파이널"
	GC_BGM_JAPAN_LOGO			= 21,	///< Track21	"일본로고"
	GC_BGM_FIRST_WAY			= 22,	///< Track22	"초보자의길"
	GC_BGM_FIGHTING				= 23,	///< Track23	"자!힘을내자"
};
enum EGCDamageTexture
{
	GC_TEX_START				= -1,
	GC_TEX_SWORD_LIGHT			= 0,	///칼의 광
	GC_TEX_SWORD_FIRE_READY		= 1,	///장풍전 기모으기
	GC_TEX_SWORD_FIRE			= 2,	///장풍
	GC_TEX_MEGA_SLASH			= 3,	///메가 슬래시
	GC_TEX_WAVE					= 4,	///파동 효과
	GC_TEX_ARROW				= 5,	///화살
	GC_TEX_ARROW_UP				= 6,	///위로 올라가는 화살
	GC_TEX_ARROWS				= 7,	///여러 개의 화살
	GC_TEX_ARROW_DEFENCE		= 8,	///화살 방어할 때 뜨는 defence글자
	GC_TEX_LIGHTNING_ARROW_SET	= 9,	///번개화살 세트
	GC_TEX_CRITAICAL_ATTACK		= 10,	///critical attack 때 뜨는 글자
	GC_TEX_COUNTER_ATTACK		= 11,	///counter  attack 때 뜨는 글자
	GC_TEX_DOUBLE_ATTACK		= 12,	///double   attack 때 뜨는 글자
	GC_TEX_GORGOS_FIRE			= 13,	///고르고스의 불
	GC_TEX_MAGIC_STAR_COMBO1	= 14,	///마법사 법진 별 그림1
	GC_TEX_MAGIC_STAR_COMBO2	= 15,	///마법사 법진 별 그림2
	GC_TEX_MAGIC_STAR_COMBO3	= 16,	///마법사 법진 별 그림3
	GC_TEX_WITCH_TELEPORT		= 17,	///마법사 텔레포트 도중의 잔상
	GC_TEX_WITCH_CRASH_GADOSEN  = 18,	///마법사 텔레포트 도중 가도센에 막혔을 때
	GC_TEX_WITCH_RECHARGE		= 19,	///마법사 MP충전 중
	GC_TEX_WITCH_JUMPATTACK		= 20,	///마법사 공중 공격
	GC_TEX_STONE_CURSE			= 21,	///스톤 커스 할 때 지팡이가 내뿜는 기운
	GC_TEX_STONE_BODY			= 22,   ///돌이 되는 몸
	GC_TEX_LEFT_LEVER			= 23,	///스톤 커스맞았을 때 뜨는 좌측 레버
	GC_TEX_RIGHT_LEVER			= 24,   ///스톤 커스맞았을 때 뜨는 우측 레버
	GC_TEX_LIGHTNING_BOLT		= 25,   ///라이트닝 볼트 
	GC_TEX_METEO				= 26,   ///메테오	(사용안함)
	GC_TEX_METEO_FIRE			= 27,	///메테오에 붙은 불	
	GC_TEX_METEO_CRASH_WAVE		= 28,	///메테오 땅에 충돌시 파동 효과
	GC_TEX_LIGHTNING_GENERATE	= 29,	///라이트닝 볼트에 감전되었을 때 generate효과
	GC_TEX_LIGHTNING_ARROW_UP	= 30,	///위로올라가는 번개화살
	GC_TEX_REVERSE_GRAVITY		= 31,	///리버스 그래비티
	GC_TEX_CURE_HEART			= 32,   ///큐어 하트
	GC_TEX_FIREBALL				= 33,   ///파이어볼
	GC_TEX_HARPY_FEATHER_ATTACK = 34,	///하피의 깃털 공격
	//GC_TEX_ARCHER_REVENGE		= 35,	///궁수 도망가면서 쏘는 화살
	GC_TEX_WITCH_GROUND			= 36,	///마법사 땅 찍기
	GC_TEX_SKILL_READY_CIRCLE	= 37,	///기술 나가기전 준비	
	GC_TEX_POTALBIM				= 38,   ///맹계 포탈시 기둥
	GC_TEX_POTAL_CIRCLE			= 39,	///맹계 포탈시 기둥 주위를 도는 파티클 효과
	GC_TEX_DRILLMON_ROCK		= 40,	///드릴몬이 떨어뜨리는 돌
	GC_TEX_WATER_POLE			= 41,	///잊혀진 도시에서 나오는 물기둥
	GC_TEX_PARADOM_MISSILE		= 42,	///파라돔이 쏘는 미사일
	GC_TEX_FORCE_STRIKE			= 43,   ///창병 1단계 기술 
	GC_TEX_FIRE_LANCE			= 44,   ///창병 2단계 기술
	GC_TEX_LIGHTNING_ARROW		= 45,	///번개화살
	GC_TEX_ICE_CURSE			= 46,	///아이스 스톤커스
	GC_TEX_KAZEAZE_FIRE			= 47,	///카제아제 필살기
	GC_TEX_SNIPER_BOLT			= 48,	///석궁 활 큰거
	GC_TEX_HARPY_DOWN			= 49,	///석궁 3단계서 하피 떨어지는 모습
	GC_TEX_LIGHTNING_CB_ARROW	= 50,	///석궁번개화살
	GC_TEX_BLUE_LIGHTNING		= 51,	///블루라이트닝
	GC_TEX_BLUE_LIGHTNING_GEN	= 52,	///블루라이트닝 generate
	GC_TEX_BOMB					= 53,	///연금 폭탄
	GC_TEX_ARCIMIST_GAS			= 54,	///연금 가스
	GC_TEX_FLASK				= 55,	///플라스크
	GC_TEX_BLOCK_BUSTER			= 56,	///연금의 블락버스터
	GC_TEX_MINE					= 57,	///연금의 지뢰
	GC_TEX_DARK_STONECURSE		= 58,	///암흑 스톤커스
	GC_TEX_DEAD_SOUL			= 59,	///가도센에 영혼 먹힐 때
	GC_TEX_ICE_METEO			= 60,	///아이스 메테오
	GC_TEX_ICE_WAVE				= 61,	///아이스 웨이브
	GC_TEX_GAIKOZSLASH			= 62,	///가이코즈 슬레쉬
	GC_TEX_WATER_CANNON			= 63,	///랍스타 워터캐논
	GC_TEX_POISON_GAS			= 64,	///독가스
	GC_TEX_DARK_FIREBALL		= 65,	///암흑파볼
	GC_TEX_FLYING_IMPACT		= 66,	///창병 신기술 1단계 임팩트 슬래쉬
	GC_TEX_NARANTA_UP			= 67,	///석궁 신기술 1단계 나란타 업
	GC_TEX_WIND_BLADE			= 68,	///라스 3단계에 쓰는 윈드 블레이트
	GC_TEX_LAS_JUMP_ATTACK	    = 69,	///라스 공중 공격 때 그리는 둥근 자취
	GC_TEX_LAS_SPEAR			= 70,	///라스 표창. 2단 점프 후 아래로 던질 때 날림
	GC_TEX_MOTAL_BLOW			= 71,	///모탈 블로우
	GC_TEX_LAS_TELEPORT			= 72,	///라스의 텔레포트
	GC_TEX_NARANTA_DUST			= 73,	///석궁 신기술 1단계 나란타 먼지
	GC_TEX_KAMIKI_DARK			= 74,	///카미키 암흑마법
	GC_TEX_INVISIBLE			= 75,	///투명 텍스쳐!! 유용하다. 맞는 범위가 모션으로는 처리가 안 될 때 쓴다.
	GC_TEX_BIG_HAND0			= 76,	///연금 신기술 1단계 빅핸드 1.
	GC_TEX_BIG_HAND1			= 77,	///연금 신기술 1단계 빅핸드 2.
	GC_TEX_BIG_HAND2			= 78,	///연금 신기술 1단계 빅핸드 3.
	GC_TEX_BIG_HAND3			= 79,	///연금 신기술 1단계 빅핸드 4.
	GC_TEX_EGG_HERE				= 80,	///알의 HERE
	GC_TEX_WITCH_SHIELD			= 81,	///법사 쉴드
    GC_TEX_DEADLY_SHARP         = 82,   ///라스전직 3단계 필살기.
    GC_TEX_DASH_EFF             = 83,   ///자객 대쉬 이펙트
	GC_TEX_FRUIT				= 84,	///엔트 과일
	GC_TEX_GIGA_SLASH_READY 	= 85,	///기가 슬래쉬 준비 이펙트
	GC_TEX_SPIN_SHOCK			= 86,	///자주색 회전 충격파
	GC_TEX_TORNADO_SPIN			= 87,	///토네이도 블레이드 도는 텍스쳐
	GC_TEX_TORNADO_IMPACT		= 88,	///토네이도 블레이드가 땅에서 일으키는 먼지 이펙트
	GC_TEX_STONE			 	= 89,	///토네이도 블레이드 위로 솟구치는 돌들
	GC_TEX_ENT_LAND				= 90,	///엔트 땅쑤시기 땅이미지
	GC_TEX_EAGLE_SITE			= 91,	///이글 사이트 (궁2)
	GC_TEX_CONFUSION			= 92,	///법사암흑2단계 좌우 키바꾸기 -- 사용안함
	GC_TEX_SHOCK				= 93,	///충격파
	GC_TEX_EXPLOSION_ARROWS     = 94,	///익스플로젼 애로우 준비(궁3)
	GC_TEX_EXPLOSION_ARROW		= 95,	///익스플로젼 애로우 
	GC_TEX_TORNADO_VOLCANIC2 	= 96,	///토네이도 블레이드 위로 솟구치는 붉은 기운2(기사3)
	GC_TEX_TORNADO_VOLCANIC3 	= 97,	///토네이도 블레이드 위로 솟구치는 붉은 기운3
	GC_TEX_CURED_BY_BOOST_HEAL	= 98,	///부스트힐로 치료받을 때의 효과
	GC_TEX_TORNADO_VOLCANIC1	= 99,	///토네이도 블레이드 위로 솟구치는 붉은 기운1
	GC_TEX_SHOW_ATHENS_SWORD	= 100,	///아테네 소워드 효과로 벌개지기 전 뜨는 효과
	GC_TEX_BLUE_ADOGEN			= 101,  ///파란색 장풍
	GC_TEX_SKULL_BOMB			= 102,  ///해골폭탄
	GC_TEX_DARK_BALL			= 103,	///라스3단 BALL
	GC_TEX_BLOOD_BASH			= 104,  ///블러드베쉬 
    GC_TEX_GOBLIN_STONE         = 105,  ///고블린 돌던지는거.
	GC_TEX_ELESIS3_DUMMY1		= 106,
	GC_TEX_ELESIS3_DUMMY2		= 107,
	GC_TEX_ELESIS3_DUMMY3		= 108,
	GC_TEX_LIRE3_ARROW			= 109,	///리르3 거대한 화살
	GC_TEX_PARADOM_SOCCERBALL	= 110,	///축구공 파라돔 미사일
	GC_TEX_PHOENIX_ARROW_FIRE	= 111,	///리르3 필살기3 떨어지는 화살에 붙는 이펙트
	GC_TEX_PHOENIX_CRUSH1		= 112,	///리르3 필살기3 충돌1
	GC_TEX_PHOENIX_CRUSH2		= 113,	///리르3 필살기3 충돌2
	GC_TEX_PHOENIX_CRUSH3		= 114,	///리르3 필살기3 충돌3
	//GC_TEX_PHOENIX_CRUSH4		= 115,	///리르3 필살기3 충돌4
	//GC_TEX_PHOENIX_CRUSH5		= 116,	///리르3 필살기3 충돌5
	GC_TEX_RED_CANNON			= 117,	///빨간 대포알
	//							  118
	//							  119
	//							  120
	GC_TEX_LAS3_TRACE			= 121,	///라스3 잔상

	GC_DAMAGE_TEX_NUM,
};

enum EGCGameStage
{
	GC_GS_FOREST_OF_ELF     = 0,    // 엘프의 숲
	GC_GS_SWAMP_OF_OBLIVION = 1,    // 망각의 늪
	GC_GS_FLYING_SHIP       = 2,    // 비공정
	GC_GS_VALLEY_OF_OATH    = 3,    // 맹세의 계곡
	GC_GS_FOGOTTEN_CITY     = 4,    // 잊혀진 도시
	GC_GS_BABEL_OF_X_MAS    = 5,    // X-마스의 바벨
	GC_GS_TEMPLE_OF_FIRE    = 6,    // 불의 사원
	GC_GS_SHOOTING_FIELD    = 7,    // 사격 대회장
	GC_GS_HELL_BRIDGE       = 8,    // 헬 브릿지
	GC_GS_TEMPLE_OF_ORC     = 9,    // 오크 사원
	GC_GS_GUILD				= 10,   // 길드맵(세르딘 성 외곽)
	GC_GS_CARRIBEACH		= 11,   // 캐리비치

	GC_GS_QUEST1_A          = 12,   // 퀘스트1 A맵
	GC_GS_QUEST1_B          = 13,   // 퀘스트1 B맵
	GC_GS_QUEST1_C          = 14,   // 퀘스트1 C맵
	GC_GS_QUEST1_D          = 15,   // 퀘스트1 D맵

	GC_GS_QUEST1_X_MAS      = 16,   // 퀘스트1 크리스마스 맵

	GC_GS_QUEST2_A          = 17,   // 퀘스트2 A맵
	GC_GS_QUEST2_B          = 18,   // 퀘스트2 B맵
	GC_GS_QUEST2_C          = 19,   // 퀘스트2 C맵
	GC_GS_QUEST2_D          = 20,   // 퀘스트2 D맵
	GC_GS_QUEST2_E          = 21,   // 퀘스트2 E맵

	GC_GS_QUEST3_A          = 22,   // 퀘스트3 A맵
	GC_GS_QUEST3_B          = 23,   // 퀘스트3 B맵
	GC_GS_QUEST3_C          = 24,   // 퀘스트3 C맵
	GC_GS_QUEST3_D          = 25,   // 퀘스트3 D맵
	GC_GS_QUEST3_E          = 26,   // 퀘스트3 E맵

	GC_GS_QUEST4_A          = 27,   // 퀘스트4 A맵
	GC_GS_QUEST4_B          = 28,   // 퀘스트4 B맵
	GC_GS_QUEST4_C          = 29,   // 퀘스트4 C맵
	GC_GS_QUEST4_D          = 30,   // 퀘스트4 D맵
	GC_GS_QUEST4_E          = 31,   // 퀘스트4 E맵
	GC_GS_QUEST4_F          = 32,   // 퀘스트4 E맵
	GC_GS_QUEST6_A          = 33,   // 퀘스트 FINAL A맵
	GC_GS_QUEST6_B          = 34,   // 퀘스트 FINAL B맵
	GC_GS_QUEST6_C          = 35,   // 퀘스트 FINAL C맵
	GC_GS_QUEST6_D          = 36,   // 퀘스트 FINAL D맵
	GC_GS_QUEST6_E          = 37,   // 퀘스트 FINAL E맵
	GC_GS_QUEST6_F          = 38,   // 퀘스트 FINAL F맵
	GC_GS_QUEST6_G          = 39,   // 퀘스트 FINAL G맵

	GC_GS_QUEST_ZERO_A		= 40,
	GC_GS_QUEST_ZERO_B		= 41,
	GC_GS_QUEST_ZERO_C		= 42,

	GC_GS_FOREST_OF_TRIAL   = 43,
    GC_GS_QUEST_ZERO_2_A    = 44,
    GC_GS_QUEST_ZERO_2_B    = 45,
    GC_GS_QUEST_ZERO_2_C    = 46,

	NUM_TOTAL_STAGE,                // 게임 스테이지 개수
};

enum EGCSOLDIER_MOTION
{
    EGCSM_WAIT              = 0,
    EGCSM_WALK              = 1,
    EGCSM_RUN               = 2,
    EGCSM_EVASION           = 3,
    EGCSM_JUMP              = 4,
    EGCSM_DAMAGE            = 5,
    EGCSM_STANDUP           = 6,
    EGCSM_DOWN              = 7,			//맞아서 넘어지거나 죽어서 넘어지는 모션...
    EGCSM_JUMPATTACK        = 8,
    EGCSM_ATTACK1           = 9,
    EGCSM_ATTACK2           = 10,
    EGCSM_ATTACK3           = 11,
    EGCSM_ATTACK4           = 12,
    EGCSM_ATTACK5           = 13,
    EGCSM_SAN_DIE           = 14,
	EGCSM_KAMIKI_RISE       = 15,
	EGCSM_CASTLEDOOR_OPEN   = 16,		//성문과 카키루문 둘다 16번 사용합니다.
	EGCSM_TRANS_KAZEAZE_DIE = 17,
	EGCSM_SLIM_DIE			= 18,

	MONSTER_MOTION_NUM      = 19,
};

enum PET_MOTION
{
    PM_FLY1          = 0,
	PM_FLY2,
	PM_FLY3,
	PM_FLY4,
	PM_MASTER_ATTACK,
	PM_MASTER_ATTACKED,
	PM_MAGIC1,
	PM_MAGIC2,
	PM_MAGIC3,
	PM_MAGIC4,
	PM_MAGIC5,
	PM_END,
};

enum EGCCharType
{
    GC_CHAR_ELESIS = 0,     ///< 엘리시스
    GC_CHAR_LIRE,			///< 리르
    GC_CHAR_ARME,			///< 아르메
	GC_CHAR_LAS,			///< 라스
	GC_CHAR_RYAN,			///< 라이언

    GC_CHAR_NUM,
};

enum EGCGrade
{
	GC_GRADE_VAGABOND = 0,
	GC_GRADE_MERCENARY,
	GC_GRADE_ROYAL,
	GC_GRADE_HOLY,
	
	GC_GRADE_NUM,
};

enum CHAR_IDENTY
{
	CID_COMMON  = -1,//이펙트용 공통
	CID_KNIGHT  = 0,//기사
	CID_ARCHER,     //궁수
	CID_WITCH,      //법사
	CID_THIEF,		//도적
	CID_DRUID,		//라이언-드루이드

	CID_LANCER,     //창병
	CID_SNIPER,     //석궁병
	CID_ARCHIMIST,  //연금술사
    CID_HASSASSIN,  //자객
	CID_DRUID_1ST,	//라이언 - 예약

	CID_SWORD_MASTER,	// 엘리시스 2차전직 소드마스터
	CID_LIRE3,			// 리르 2차전직
	CID_WARLOCK,		// 아르메 2차전직 워록
	CID_DASSASSIN,		// 라스 2차전직 다크어쌔신
	CID_DRUID_2ST,	//라이언 - 예약
};

enum EGCGameItemType
{
    GC_GAME_ITEM_HPUP       = 0,    ///< 체력보충
    GC_GAME_ITEM_SUPER      = 1,    ///< 무적
    GC_GAME_ITEM_WING       = 2,    ///< 천사의 날개
    GC_GAME_ITEM_HALLOW     = 3,    ///< 투명
    GC_GAME_ITEM_REFLECTION = 4,    ///< 반사
    GC_GAME_ITEM_MPUP       = 5,    ///< 기력보충
    GC_GAME_ITEM_GP         = 6,    ///< 게임머니	
	GC_GAME_ITEM_HASTE      = 7,    ///< 헤이스트
    GC_GAME_ITEM_BONUS      = 8,    ///< 보너스
	GC_GAME_ITEM_CHICKEN    = 9,    ///< 닭고기
    GC_GAME_ITEM_GRAPE      = 10,   ///< 포도
    GC_GAME_ITEM_WATERMELON = 11,   ///< 수박
    GC_GAME_ITEM_SANTA_GIFT = 12,   ///< 산타선물
    GC_GAME_ITEM_RICECAKE   = 13,   ///< 송편
	GC_GAME_ITEM_EGGCOOKIE	= 14,	///< 계란과자
	GC_GAME_ITEM_PIGHEAD	= 15,	///< 돼지머리
    GC_GAME_ITEM_NUM,
};

enum STATE_BLOCK_ID
{
	DISABLE_STATE_ID  = -1,
	SB_INIT_STATE = 0,
	PARTICLE_STATE_ID,
};

enum KT_TIMER_ID
{
    KTIMER_QUEST_STAGE_FIRE1 = 0,
    KTIMER_QUEST_STAGE_FIRE2,
    KTIMER_QUEST_STAGE_FIRE3,
	KTIMER_QUEST2_STAGE4_FIRE1,
	KTIMER_QUEST2_STAGE4_FIRE2,
	KTIMER_QUEST3_STAGE3_FIRE1,
	KTIMER_QUEST3_STAGE4_FIRE1,
	KTIMER_QUEST3_STAGE4_FIRE2,
	KTIMER_QUEST3_STAGE4_FIRE3,
	KTIMER_QUEST3_STAGE5_FIRE1,
	

    KTIMER_MONSTER_AWAKE,
    KTIMER_EARTHQUAKE_DELAY,
};


enum GC_MON_EXTRA_TEX
{
	GMET_NO_USE = -1,
	GMET_MONGBANA,
	GMET_MONGBANB,
	GMET_PATUSEI,
	
	GMET_MIDNIGHT_REDANT,
	GMET_MIDNIGHT_BLUEANT,
	GMET_MIDNIGHT_GORGOS,
	GMET_MIDNIGHT_TROLL,
	GMET_MIDNIGHT_REDGORGOS,
	GMET_MIDNIGHT_KAIDO,
	GMET_MIDNIGHT_MONGBAN,
	GMET_MIDNIGHT_GAIKOZ,
	GMET_MIDNIGHT_LAPSTER,
	GMET_MIDNIGHT_GHOSTFISH,

	GMET_MIDNIGHT_BASILISK,

	GMET_TOTAL_NUM,
};

enum MOTIONID
{
	MID_ELESIS_WAIT = 0,				//0//엘리시스//기본자세
	MID_ELESIS_WALK,					//1//엘리시스//걷기
	MID_ELESIS_JUMPREADY,				//2//엘리시스//점프준비
	MID_ELESIS_JUMP,					//3//엘리시스//점프
	MID_ELESIS_ATK1_1,					//4//엘리시스//기본공격1_1
	MID_ELESIS_ATK1_2,					//5//엘리시스//기본공격1_2
	MID_ELESIS_ATK1_3,					//6//엘리시스//기본공격1_3
	MID_ELESIS_ATK1_4,					//7//엘리시스//기본공격1_4
	MID_ELESIS_ATK1_5,					//8//엘리시스//기본공격1_5
	MID_COMMON_SMALLDAMAGE,				//9//공용//약하게 맞았을때
	MID_COMMON_BIGDAMAGE,				//10//공용//강하게 맞았을때
	MID_COMMON_DOWN_AND_STANDUP,		//11//공용//넘어졌다 일어서기
	MID_ELESIS_DASH,					//12//엘리시스//데쉬
	MID_ELESIS_DASHATK,					//13//엘리시스//데쉬어텍
	MID_ELESIS_JUMPATK,					//14//엘리시스//점프어텍
	MID_ELESIS_SPECIAL1,				//15//엘리시스//필살기1
	MID_ELESIS_SPECIAL2,				//16//엘리시스//필살기2
	MID_ELESIS_SPECIAL3,				//17//엘리시스//필살기3
	MID_COMMON_DOWN_IN_SKY,				//18//공용//공중에서 맞아서 쓰러지기.. 떨어지면 11번으로 변경됨
	MID_ELESIS_WINPOS,					//19//엘리시스//승리포즈
	MID_LIRE_LEGATK,					//20//리르//다리찍기
	MID_COMMON_ARENA,					//21//공용//아래나
	MID_COMMON_DROP,					//22//공용//착지
	MID_ELESIS_START,					//23//엘리시스//시작
	MID_LIRE_WAIT,						//24//리르//기본자세
	MID_LIRE_WALK,						//25//리르//걷기
	MID_LIRE_DASH,						//26//리르//데쉬
	MID_LIRE_JUMPREADY,					//27//리르//점프준비
	MID_LIRE_JUMP,						//28//리르//점프
	MID_LIRE_DOUBLEJUMP,				//29//리르//2단점프
	MID_LIRE_ATK1_1,					//30//리르//기본공격1_1
	MID_LIRE_ATK1_2,					//31//리르//기본공격1_2
	MID_LIRE_ATK1_3,					//32//리르//기본공격1_3
	MID_LIRE_ATK1_4,					//33//리르//기본공격1_4
	MID_LIRE_ATK1_5,					//34//리르//기본공격1_5
	MID_LIRE_SPECIAL2,					//35//리르//필살기2
	MID_LIRE_SPECIAL3,					//36//리르//필살기3
	MID_LIRE_SPECIAL1,					//37//리르//필살기1
	MID_LIRE_JUMPATK,					//38//리르//점프어텍
	MID_ELE_ELE2_ARROWDEF_ON_GROUND,	//39//엘리,엘리(2)//화살막기(지상1)
	MID_ELE_ELE2_ARROWDEF_IN_SKY,		//40//엘리,엘리(2)//화살막기(공중)
	MID_LIRE_DASHATK,					//41//리르//데쉬어텍
	MID_ELE_ELE2_ARROWDEF_ON_GROUND2,	//42//엘리,엘리(2)//화살막기(지상2)
	MID_COMMON_FRONTDOWN_AND_STANDUP,	//43//공용//뒤로날아가며 앞으로 엎어졌다 일어서기
	MID_LIRE_START,						//44//리르//시작
	MID_ARME_EARTHQUAKE,				//45//아르메//땅찍기 공격(아르메 콤보중 UpKey)
	MID_LIRE_WINPOS,					//46//리르//승리포즈
	MID_ARME_WAIT_AND_FATAL,			//47//아르메//기본자세,기본자세(Fatal)
	MID_ARME_START,						//48//아르메//시작
	MID_ARME_WALK,						//49//아르메//걷기
	MID_ARME_ATK1_1,					//50//아르메//기본공격1_1
	MID_ARME_ATK1_2,					//51//아르메//기본공격1_2
	MID_ARME_ATK1_3,					//52//아르메//기본공격1_3
	MID_ARME_ATK1_4,					//53//아르메//기본공격1_4			
	MID_ARME_ARME2_DASH,				//54//아르,아르(2)//데쉬(순간이동)
	MID_ARME_JUMPREADY,					//55//아르메//점프준비
	MID_ARME_JUMP,						//56//아르메//점프
	MID_ARME_JUMPATK,					//57//아르메//점프어텍
	MID_ARME_SPECIAL1_BLACK,			//58//아르메//흑마법1단계
	MID_COMMON_STONECURSE,				//59//공용//스톤커스
	MID_ARME_SPECIAL2_BLACK,			//60//아르메//흑마법2단계
	MID_ARME_SPECIAL3_BLACK,			//61//아르메//흑마법3단계
	MID_ARME_SPECIAL1,					//62//아르메//필살기1
	MID_ARME_SPECIAL2,					//63//아르메//필살기2
	MID_COMMON_DAMAGED_BY_FLYATK,		//64//공용//띄우기 공격을 맞았을때
	MID_ARME_SPECIAL3,					//65//아르메//필살기3
	MID_ARME_DASHATK,					//66//아르메//데쉬어텍
	MID_ARME_WINPOS,					//67//아르메//승리포즈
	MID_ELESIS_CATCH,					//68//엘리시스//잡기
	MID_COMMON_CATCH_BY_ELESIS,			//69//공용//잡혀주기
	MID_LIRE_CATCH,						//70//리르//잡기
	MID_COMMON_CATCH_BY_LIRE,			//71//공용//잡혀주기
	MID_ARME_CATCH,						//72//아르메//잡기
	MID_COMMON_CATCH_BY_ARME,			//73//공용//잡혀주기
	MID_ELESIS_STANDUP_AND_ATK,			//74//엘리시스//일어서면서 치기
	MID_LIRE_ARME_ARME2_STANDUP_AND_ATK,//75//리르,아르메,아르메(2)//일어서면서 치기
	MID_ELESIS_RUN,						//76//엘리시스 모션Option:134//달리기
	MID_ELESIS_SELECT,					//77//엘리//선택
	MID_LIRE_SELECT,					//78//리르//선택
	MID_ARME_SELECT,					//79//아르메//선택
	MID_ELESIS_FLYATK,					//80//엘리시스//콤보중 UpKey누르면…상대를 띄운다
	MID_NOUSE_81,						//81//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_NOUSE_82,						//82//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_NOUSE_83,						//83//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_ELE_ELE2_RESULT_WIN,			//84//엘리,엘리(2)//결과창 안에서 이겼을 때 그려지는 모션
	MID_LIRE_LIRE2_RESULT_WIN,			//85//리르,리르(2)//결과창 안에서 이겼을 때 그려지는 모션
	MID_ARME_ARME2_RESULT_WIN,			//86//아르,아르(2)//결과창 안에서 이겼을 때 그려지는 모션
	MID_ELE_ELE2_RESULT_LOSE,			//87//엘리,엘리(2)//결과창 안에서 졌을 때 그려지는 모션
	MID_LIRE_LIRE2_RESULT_LOSE,			//88//리르,리르(2)//결과창 안에서 졌을 때 그려지는 모션
	MID_ARME_ARME2_RESULT_LOSE,			//89//아르,아르(2)//결과창 안에서 졌을 때 그려지는 모션
	MID_COMMON_SHOPPOS,					//90//공용//샾에 나오는 캐릭터 장면
	MID_COMMON_INTO_THE_TELEPORT,		//91//공용//순간이동들어가기
	MID_ELESIS_ARROW_REFLECTION,		//92//엘리시스//화살막기(쳐내는 모션)
	MID_ELESIS2_SELECT,					//93//엘리(2)//선택
	MID_ELESIS2_WAIT,					//94//엘리(2)//기본자세
	MID_ELESIS2_WALK,					//95//엘리(2)//걷기
	MID_ELESIS2_DASH,					//96//엘리(2)//데쉬
	MID_ELESIS2_JUMP,					//97//엘리(2)//점프
	MID_ELESIS2_JUMPREADY,				//98//엘리(2)//점프준비
	MID_ELESIS2_JUMPATK,				//99//엘리(2)//점프어텍
	MID_ELESIS2_DASHATK,				//100//엘리(2)//데쉬어텍
	MID_ELESIS2_ATK1_1,					//101//엘리(2)//기본공격1_1
	MID_ELESIS2_ATK1_2,					//102//엘리(2)//기본공격1_2
	MID_ELESIS2_ATK1_3,					//103//엘리(2)//기본공격1_3
	MID_ELESIS2_ATK1_4,					//104//엘리(2)//기본공격1_4
	MID_ELESIS2_ATK1_5,					//105//엘리(2)//기본공격1_5
	MID_ELESIS2_FASTATK_ON_COMBO1,		//106//엘리(2)//Z연타시 매우때리기 공격
	MID_ELESIS2_START,					//107//엘리(2)//시작
	MID_NOUSE_108,						//108//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_ELESIS2_SPECIAL1,				//109//엘리(2)//필살기1
	MID_ELESIS2_SPECIAL2,				//110//엘리(2)//필살기2
	MID_ELESIS2_SPECIAL3,				//111//엘리(2)//필살기3
	MID_ELESIS2_CATCH,					//112//엘리(2)//잡기
	MID_COMMON_CATCH_BY_ELESIS2,		//113//공용//잡혀주기
	MID_ELESIS2_STANDUP_AND_ATK,		//114//엘리(2)//일어서면서 치기
	MID_ELESIS2_WINPOS,					//115//엘리(2)//승리포즈
	MID_ELE_ELE2_FATAL,					//116//엘리,엘리(2)//기본자세(Fatal)
	MID_LIRE_LIRE2_FATAL,				//117//리르,리르(2)//기본자세(Fatal)
	MID_ELE_ELE2_JOKE,					//118//엘리,엘리(2)//도발
	MID_LIRE_LIRE2_JOKE,				//119//리르,리르(2)//도발
	MID_ARME_ARME2_JOKE,				//120//아르,아르(2)//도발
	MID_LIRE2_WAIT,						//121//리르(2)//기본자세
	MID_LIRE2_WALK,						//122//리르(2)//걷기
	MID_LIRE2_DASH,						//123//리르(2)//데쉬
	MID_NOUSE_124,						//124//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_LIRE2_SELECT,					//125//리르(2)//선택
	MID_LIRE2_START,					//126//리르(2)//시작
	MID_LIRE2_DASHATK,					//127//리르(2)//데쉬어텍
	MID_LIRE2_JUMPREADY,				//128//리르(2)//점프준비
	MID_LIRE2_JUMP,						//129//리르(2)//점프
	MID_LIRE2_ATK1_1,					//130//리르(2)//기본공격1_1(전방 공격)
	MID_LIRE2_ATK1_2,					//131//리르(2)//기본공격1_2
	MID_LIRE2_ATK1_3,					//132//리르(2)//기본공격1_3
	MID_LIRE2_ATK1_4,					//133//리르(2)//기본공격1_4
	MID_LIRE2_ATK1_5,					//134//리르(2)//기본공격1_5
	MID_LIRE2_ATK2_1,					//135//리르(2)//기본공격2_1(상방 공격)
	MID_LIRE2_ATK2_2,					//136//리르(2)//기본공격2_2
	MID_LIRE2_ATK2_3,					//137//리르(2)//기본공격2_3
	MID_LIRE2_ATK2_4,					//138//리르(2)//기본공격2_4
	MID_LIRE2_ATK2_5,					//139//리르(2)//기본공격2_5
	MID_LIRE2_ATK3_1,					//140//리르(2)//기본공격3_1(하방 공격)
	MID_LIRE2_ATK3_2,					//141//리르(2)//기본공격3_2
	MID_LIRE2_ATK3_3,					//142//리르(2)//기본공격3_3
	MID_LIRE2_ATK3_4,					//143//리르(2)//기본공격3_4
	MID_LIRE2_ATK3_5,					//144//리르(2)//기본공격3_5
	MID_LIRE2_JUMPATK,					//145//리르(2)//점프어텍
	MID_LIRE2_ATK1_STOP,				//146//리르(2)//기본공격1_콤보 중단
	MID_LIRE2_ATK2_STOP,				//147//리르(2)//기본공격2_콤보 중단
	MID_LIRE2_ATK3_STOP,				//148//리르(2)//기본공격3_콤보 중단
	MID_LIRE2_CATCH,					//149//리르(2)//잡기
	MID_COMMON_CATCH_BY_LIRE2,			//150//공용//잡혀주기
	MID_LIRE2_STANDUP_AND_ATK,			//151//리르(2)//일어서면서 치기
	MID_LIRE2_SPECIAL1_DOWN,			//152//리르(2)//필살기1(하방)
	MID_LIRE2_SPECIAL1_UP,				//153//리르(2)//필살기1(상방)
	MID_LIRE2_SPECIAL3,					//154//리르(2)//필살기3
	MID_LIRE2_SPECIAL2,					//155//리르(2)//필살기2
	MID_LIRE2_JUMPPING_DOWN_ARROW_ATK,	//156//리르(2)//공중에서 땅보고 화살쏘기
	MID_LIRE2_LAND_IN_DOWN_ARROW_ATK,	//157//리르(2)//156번에서의 착지
	MID_LIRE2_WINPOS,					//158//리르(2)//승리포즈
	MID_ELESIS123_BERSERK,				//159//엘리,엘리(2)//버서크!!!
	MID_ARME2_WAIT_AND_FATAL,			//160//아르메(2)//기본자세,Fatal
	MID_ARME2_WALK,						//161//아르메(2)//걷기
	MID_ARME2_DASHATK,					//162//아르메(2)//데쉬어텍
	MID_ARME2_ATK1_1,					//163//아르메(2)//기본공격1(폭탄놓기)
	MID_ARME2_ATK1_2,					//164//아르메(2)//기본공격1(폭탄놓기)
	MID_ARME2_ATK1_3,					//165//아르메(2)//기본공격1(폭탄놓기)
	MID_ARME2_ATK1_4,					//166//아르메(2)//기본공격1(폭탄놓기)
	MID_ARME2_ATK1_5,					//167//아르메(2)//기본공격1(폭탄놓기)
	MID_ARME2_JUMPREADY,				//168//아르메(2)//점프준비
	MID_ARME2_JUMP,						//169//아르메(2)//점프
	MID_ARME2_JUMPATK,					//170//아르메(2)//점프어텍
	MID_ARME2_SPECIAL2_BLACK,			//171//아르메(2)//흑마법2단계
	MID_ARME2_SPECIAL3,					//172//아르메(2)//필살기3
	MID_ARME2_SPECIAL3_BLACK,			//173//아르메(2)//흑마법3단계
	MID_ARME2_SPECIAL1_BLACK,			//174//아르메(2)//흑마법1단계
	MID_ARME2_CATCH,					//175//아르메(2)//잡기
	MID_COMMON_CATCH_BY_ARME2,			//176//공용//잡혀주기
	MID_ARME2_SPECIAL1,					//177//아르메(2)//필살기1
	MID_ARME2_SPECIAL2,					//178//아르메(2)//필살기2
	MID_NOUSE_179,						//179//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_ARME2_SELECT,					//180//아르메(2)//선택
	MID_ARME2_START,					//181//아르메(2)//시작(항아리에서 나온다)
	MID_ARME2_WINPOS,					//182//아르메(2)//승리포즈
	MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER,//183//아르메,아르메(2)//DASH중에 보스에게 밀리는 모션..
	MID_COMMON_INTROPOS,				//184//공용//게임 스타시에 나오는 캐릭터 장면
	MID_COMMON_TIGER_ROLLING_ACTION,	//185//공용//타이거롤링액션
	MID_COMMON_INTO_BASILISK,			//186//공용//바실에게 먹히기
//=================================================================================================//
	MID_LAS_WAIT,						//187//라스//기본자세
	MID_LAS_FATAL,						//188//라스//기본자세(Fatal)
	MID_LAS_WALK,						//189//라스//걷기
	MID_LAS_DASH,						//190//라스//대쉬
	MID_LAS_RUN,						//191//라스//달리기
	MID_LAS_JUMPREADY,					//192//라스//점프준비
	MID_LAS_JUMP,						//193//라스//점프
	MID_LAS_JUMPATK,					//194//라스//점프공격
	MID_LAS_DOUBLEJUMP,					//195//라스//2단점프
	MID_LAS_DOUBLEJUMP_ATK,				//196//라스//2단점프공격
	MID_LAS_START,						//197//라스//시작
	MID_LAS_ATK1_1,						//198//라스//기본공격1_1
	MID_LAS_FASTATK_ON_COMBO1,			//199//라스//기본공격 빠르게 연타
	MID_LAS_DASHATK1,					//200//라스//대쉬콤포1_1
	MID_LAS_DASHATK2,					//201//라스//대쉬콤보1_2
	MID_LAS_DASHATK3,					//202//라스//대쉬콤보1_3
	MID_LAS_DASHATK4,					//203//라스//대쉬콤보1_4
	MID_LAS_WINPOS,						//204//라스//승리포즈
	MID_LAS_STANDUP_AND_ATK,			//205//라스//일어나면서 공격
	MID_LAS_ARROWDEF_ON_GROUND1,		//206//라스//화살막기1
	MID_LAS_ARROWDEF_ON_GROUND2,		//207//라스//화살막기2
	MID_LAS_ARROWDEF_IN_SKY,			//208//라스//공중화살막기
	MID_LAS_SPECIAL1,					//209//라스//필살기1
	MID_LAS_SPECIAL2,					//210//라스//필살기2
	MID_LAS_SPECIAL3,					//211//라스//필살기3
	MID_LAS_EVASION_ATK,				//212//라스//회피공격
	MID_LAS_JOKE,						//213//라스//도발
	MID_LAS_CATCH,						//214//라스//잡기
	MID_COMMON_CATCH_BY_LAS,			//215//공용//라스에게 잡히기
	MID_LAS_SELECT,						//216//라스//선택
	MID_LAS_RESULT_WIN,					//217//라스//결과창 승리포즈
	MID_LAS_RESULT_LOSE,				//218//라스//결과창 패배포즈
	MID_LAS_DOUBLEJUMP_LAND,			//219//라스//2단점프 착지
	MID_LAS_CRITICAL_ATK,				//220//라스//크리티컬 어텍
	MID_LAS_DOUBLE_ATK,					//221//라스//더블 어텍
	MID_LAS_SPECIAL2_FAIL,				//222//라스//모탈블로우 실패
	MID_LAS_EVASION,					//223//라스//회피공격
	MID_LAS_ATK1_2,						//224//라스//기본공격1_2
	MID_LAS_ATK1_3,						//225//라스//기본공격1_3
//========================================================================================
//                                      신    필
//========================================================================================
	MID_ELESIS_SPECIAL_MOONBLADE,		//226//엘리시스//필살기.. 문블레이드....
	MID_ARME_SPECIAL_SHINING_ARROW,		//227//아르메//필살기..샤이닝애로우...
	MID_ELESIS2_SPECIAL_FLYING_IMPACT,  //228//엘리시스2//필살기 플라잉 임펙트
	MID_ARME2_SPECIAL_BIG_HAND,			//229//아르메2//필살기 빅핸드
	MID_LIRE_WIND_WARD,					//230//리르//필살기 윈드워드
	MID_LIRE2_NARANTA,					//231//리르2//필살기 나란타
//========================================================================================
//                                      라스 전직
//========================================================================================
    MID_LAS2_WAIT,                      //232//라스2//기본자세
    MID_LAS2_FATAL,                     //233//라스2//기본자세(페이탈)
    MID_LAS2_WALK,                      //234//라스2//걷기
    MID_LAS2_DASH_1,                    //235//라스2//대쉬1
    MID_LAS2_DASH_2,                    //236//라스2//대쉬2
    MID_LAS2_JUMPREADY,                 //237//라스2//점프준비
    MID_LAS2_JUMP,                      //238//라스2//점프
    MID_LAS2_JUMPATK,                   //239//라스2//점프공격
    MID_LAS2_START,                     //240//라스2//시작
    MID_LAS2_ATK1_1,                    //241//라스2//기본콤보1
    MID_LAS2_ATK1_2,                    //242//라스2//기본콤보2
    MID_LAS2_ATK1_3,                    //243//라스2//기본콤보3
    MID_LAS2_ATK1_4,                    //244//라스2//기본콤보4
    MID_LAS2_RESULT_WIN,                //245//라스2//결과창 승리 포즈
    MID_LAS2_RESULT_LOSE,               //246//라스2//결과창 패배 포즈
    MID_LAS2_JUMPDASH,                  //247//라스2//점프대쉬
    MID_LAS2_JUMPDASH_ATK,              //248//라스2//점프대쉬 어택
    MID_LAS2_ARROW_EVASION_SKY,         //249//라스2//점프 화살 회피
    MID_LAS2_STANDUP_AND_ATK,           //250//라스2//기상 공격
    MID_LAS2_SELECT,                    //251//라스2//대기실
    MID_LAS2_DASH_ATK,                  //252//라스2//대쉬어택
    MID_LAS2_JOKE,                      //253//라스2//도발
    MID_LAS2_WINPOS,                    //254//라스2//승리포즈
    MID_LAS2_CATCH,                     //255//라스2//잡기
    MID_COMMON_CATCH_BY_LAS2,           //256//공용//라스2에 잡히기
    MID_LAS2_ARROWDEF_IN_SKY,           //257//라스2//공중 화살 막기
    MID_LAS2_ARROWDEF_ON_GROUND1,       //258//라스2//지상 화살 막기1
    MID_LAS2_ARROWDEF_ON_GROUND2,       //259//라스2//지상 화살 막기2
    MID_LAS2_ARROW_EVASION_GROUND,      //260//라스2//지상 화살 회피
    MID_LAS2_SPECIAL1,                  //261//라스2//필살기1
    MID_LAS2_SPECIAL2,                  //262//라스2//필살기2
    MID_LAS2_SPECIAL3,                  //263//라스2//필살기3
    MID_LAS2_CRITICAL_ATK,              //264//라스2//크리티컬 공격
    MID_LAS2_DOUBLE_ATK,                //265//라스2//더블공격
//====================================================================================//
	MID_ELESIS_GIGA_SLASH,				//266//엘리시스//필살기 기가 슬래쉬
	MID_ELESIS_TORNADO_BLADE,			//267//엘리시스//필살기 토네이도 블레이드
	MID_LIRE_EAGLE_SITE,				//268//리르//이글 사이트
	MID_LIRE_EXPLOSION_ARROW,			//269//리르//익스플로젼 애로우
	MID_ARME_SPECIAL_CONFUSION,			//270//아르메//컨퓨젼
	MID_ARME_LAVA_FLOW,					//271//아르메//라바플로우
	MID_ARME_BOOST_HEAL,				//272//아르메//부스트 힐
	MID_ARME_DARK_SWORM,				//273//아르메//다크 스웜
	MID_COMMON_ENT_TRUNK,				//274//공용//엔트줄기에 잡히기
	MID_ARME_ATHENS_SWORD,				//275//아르메//아테네 소워드
	MID_LAS_DOUBLE_SLASH,				//276//라스//더블슬래쉬
	MID_LAS_BLOOD_BASH,					//277//라스//블러드배쉬(피자동회복안됨)
	MID_LAS_GUARDIAN_BALL,				//278//라스//다크볼(시야좁히기)
//================================2차 전직 시작=======================================//
	MID_ELESIS3_WAIT,					//279//엘리(3)//기본자세
	MID_ELESIS3_FATAL,					//280//엘리(3)//기본자세(페이탈)
	MID_ELESIS3_WALK,					//281//엘리(3)//걷기
	MID_ELESIS3_JOKE,					//282//엘리(3)//도발
	MID_ELESIS3_DASH,					//283//엘리(3)//대쉬
	MID_ELESIS3_RUN,					//284//엘리(3)//달리기
	MID_ELESIS3_DASHATK,				//285//엘리(3)//대쉬 공격
	MID_ELESIS3_JUMPREADY,				//286//엘리(3)//점프준비
	MID_ELESIS3_JUMP,					//287//엘리(3)//점프
	MID_ELESIS3_JUMPATK,				//288//엘리(3)//점프공격_노말
	MID_ELESIS3_JUMPATK_UP,				//289//엘리(3)//점프공격_↑눌렀을 때…
	MID_ELESIS3_JUMPATK_DOWN,			//290//엘리(3)//점프공격_↓눌렀을 때…
	MID_ELESIS3_ATK1_1,					//291//엘리(3)//기본콤보1-1
	MID_ELESIS3_ATK1_2,					//292//엘리(3)//기본콤보1-2
	MID_ELESIS3_ATK1_3,					//293//엘리(3)//기본콤보1-3
	MID_ELESIS3_SHADOW_ATK,				//294//엘리(3)//그림자 공격
	MID_ELESIS3_CRITICAL_ATK,			//295//엘리(3)//크리티컬 공격
	MID_ELESIS3_DOUBLE_ATK,				//296//엘리(3)//더블 공격
	MID_ELESIS3_CATCH,					//297//엘리(3)//잡기
	MID_COMMON_CATCH_BY_ELE3,			//298//공용	//엘리(3)에게 잡히기
	MID_ELESIS3_STANDUP_AND_ATK,		//299//엘리(3)//기상 공격
	MID_ELESIS3_SPECIAL1,				//300//엘리(3)//필살기1
	MID_ELESIS3_SPECIAL2,				//301//엘리(3)//필살기2
	MID_ELESIS3_SPECIAL3,				//302//엘리(3)//필살기3
	MID_ELESIS3_START,					//303//엘리(3)//시작
	MID_ELESIS3_WINPOS,					//304//엘리(3)//승리포즈
	MID_ELESIS3_SELECT,					//305//엘리(3)//대기실
	MID_ELESIS3_RESULT_WIN,				//306//엘리(3)//결과창 승리 포즈
	MID_ELESIS3_RESULT_LOSE,			//307//엘리(3)//결과창 패배 포즈
	MID_ELESIS3_ARROWDEF_ON_GROUND,		//308//엘리(3)//화살 막기1
	MID_ELESIS3_ARROWDEF_ON_GROUND2,	//309//엘리(3)//화살 막기2
	MID_ELESIS3_ARROWDEF_IN_SKY,		//310//엘리(3)//공중에서 화살 막기
	MID_ELESIS3_ARROW_REFLECTION,		//311//엘리(3)//화살 쳐내기
	MID_ELESIS3_JUMPATK_DOWN_LAND,		//312//엘리(3)//내려오며 찍기 공격의 착지동작!
//================================리르 2차 전직=======================================//
	MID_LIRE3_WAIT,						//313//리르(3)//기본자세
	MID_LIRE3_WALK,						//314//리르(3)//걷기
	MID_LIRE3_DASH,						//315//리르(3)//대쉬
	MID_LIRE3_RUN,						//316//리르(3)//달리기
	MID_LIRE3_DASHATK,					//317//리르(3)//대쉬 공격
	MID_LIRE3_JUMPREADY,				//318//리르(3)//점프준비
	MID_LIRE3_JUMP,						//319//리르(3)//점프
	MID_LIRE3_ATK1_1,					//320//리르(3)//기본콤보1-1
	MID_LIRE3_ATK1_2,					//321//리르(3)//기본콤보1-2
	MID_LIRE3_ATK1_3,					//322//리르(3)//기본콤보1-3
	MID_LIRE3_MELEEATK,					//323//리르(3)//근접공격
	MID_LIRE3_JUMPATK1,					//324//리르(3)//점프공격01
	MID_LIRE3_JUMPATK2,					//325//리르(3)//점프공격02
	MID_LIRE3_LANDING,					//326//리르(3)//점프 후 착지
	MID_LIRE3_CATCH,					//327//리르(3)//잡기
	MID_COMMON_CATCH_BY_LIRE3,			//328//공용//리르(3)에게 잡히기
	MID_LIRE3_SPECIAL1,					//329//리르(3)//필살기1
	MID_LIRE3_SPECIAL2,					//330//리르(3)//필살기2
	MID_LIRE3_SPECIAL3,					//331//리르(3)//필살기3
	MID_LIRE3_JOKE,						//332//리르(3)//도발
	MID_LIRE3_SELECT,					//333//리르(3)//대기실
	MID_LIRE3_START,					//334//리르(3)//시작
	MID_LIRE3_WINPOS,					//335//리르(3)//종료
	MID_LIRE3_RESULT_LOSE,				//336//리르(3)//결과창패배
	MID_LIRE3_RESULT_WIN,				//337//리르(3)//결과창승리	
	MID_LIRE3_FATAL,					//338//리르(3)//기본자세(파이탈)
	MID_LIRE3_JUMPATK3,					//339//리르(3)//점프공격03
	MID_LIRE3_DASHATK2,					//340//리르(3)//대쉬공격02	
	MID_LIRE3_CRITICALATK,				//341//리르(3)//크리티컬ATK
	MID_LIRE3_DOUBLEATK,				//342//리르(3)//더블ATK	
	MID_LIRE3_CRITICAL_MELEEATK,		//343//리르(3)//크리티컬밀리공격
	MID_LIRE3_CRITICAL_DASHATK2,		//344//리르(3)//크리티컬대쉬공격02
	MID_LIRE3_JUMP_DASH,				//345//리르(3)//점프대쉬
//================================리르 2차 전직 끝=======================================//
//================================아르메 2차 전직=======================================//
	MID_ARME3_WAIT,						//346//아르메(3)//기본자세
	MID_ARME3_FATAL,					//347//아르메(3)//패이탈
	MID_ARME3_WALK,						//348//아르메(3)//걷기
	MID_NOUSE_349,						//349//사용안함(SLOT)--XXXXXXXXXXXXXXX
	MID_NOUSE_350,						//350//사용안함(SLOT)--XXXXXXXXXXXXXXX
	MID_ARME3_JUMPREADY,				//351//아르메(3)//점프준비
	MID_ARME3_JUMP,						//352//아르메(3)//점프
	MID_ARME3_SUMMON_MOVE,				//353//아르메(3)//정령 준비 중 이동
	MID_ARME3_SUMMON_WAIT,				//354//아르메(3)//정령 소환 이동 후 대기
	MID_ARME3_UNSUMMON,					//355//아르메(3)//소환 해제
	MID_ARME3_DASHATK1,					//356//아르메(3)//대쉬 공격 1
	MID_ARME3_DASHATK2,					//357//아르메(3)//대쉬 공격 2
	MID_NOUSE_358,						//358//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_ARME3_JUMPATK1,					//359//아르메(3)//점프 공격 1
	MID_ARME3_JUMPATK2,					//360//아르메(3)//점프 공격 2
	MID_ARME3_ATK1READY,				//361//아르메(3)//공격준비 1
	MID_ARME3_ATK2READY,				//362//아르메(3)//공격준비 2
	MID_ARME3_SUMMON_ENDINE,			//363//아르메(3)//운디네 소환
	MID_ARME3_DASHATK3,					//364//아르메(3)//샐러맨더 소환 -> 대쉬공격3
	MID_ARME3_SUMMON_SHYLPH,			//365//아르메(3)//실프 소환
	MID_ARME3_SUMMON_UNDINE,			//366//아르메(3)//운다인 소환
	MID_ARME3_SUMMON_SALIST,			//367//아르메(3)//샐리스트 소환 -> 철자 이상해도 항의 X
	MID_NOUSE_368,						//368//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_ARME3_SPECIAL1,					//369//아르메(3)//필살기 1
	MID_ARME3_SPECIAL2,					//370//아르메(3)//필살기 2
	MID_ARME3_SPECIAL3,					//371//아르메(3)//필살기 3
	MID_ARME3_JOKE,						//372//아르메(3)//도발
	MID_ARME3_SELECT,					//373//아르메(3)//대기실
	MID_ARME3_START,					//374//아르메(3)//시작
	MID_ARME3_WINPOS,					//375//아르메(3)//종료
	MID_NOUSE376,						//376//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_NOUSE377,						//377//사용안함(SLOT)//XXXXXXXXXXXXXXX
	MID_ARME3_SUMMON_JUMP,				//378//아르메(3)//소환준비 중 점프 준비
//================================아르메 2차 전직 끝=======================================//
//================================라스 2차 전직 시작=======================================//
	MID_LAS3_WAIT,						//379//라스(3)//기본자세
	MID_LAS3_FATAL,						//380//라스(3)//패이탈
	MID_LAS3_WALK,						//381//라스(3)//걷기
	MID_LAS3_DASH,						//382//라스(3)//대쉬
	MID_LAS3_DASH_ATK2,					//383//라스(3)//대쉬 공격2
	MID_LAS3_JUMPREADY,					//384//라스(3))//점프 준비
	MID_LAS3_JUMP,						//385//라스(3)//점프
	MID_LAS3_UP_JUMPDASH,				//386//라스(3)//점프 대쉬 위로
	MID_LAS3_FRONT_JUMPDASH,			//387//라스(3)//점프 대쉬 앞으로
	MID_LAS3_DASH_ATK3,					//388//라스(3)//대쉬 공격2
	MID_LAS3_DOWN_JUMPDASH,				//389//라스(3)//점프 대쉬 아래로
	MID_LAS3_LANDING,					//390//라스(3)//점프 대쉬 착지
	MID_LAS3_ATK1,						//391//라스(3)//콤보 공격 1
	MID_LAS3_ATK2,						//392//라스(3)//콤보 공격 2
	MID_LAS3_ATK3,						//393//라스(3)//콤보 공격 3
	MID_LAS3_ATK4,						//394//라스(3)//콤보 공격 4
	MID_LAS3_ATK3_2,					//395//라스(3)//콤보 공격 3-2
	MID_LAS3_DASH_ATK,					//396//라스(3)//대쉬 공격
	MID_LAS3_JUMP_ATK,					//397//라스(3)//점프 공격
	MID_LAS3_CATCH,						//398//라스(3)//잡기
	MID_COMMON_CATCH_BY_LAS3,			//399//라스(3)//잡히기
	MID_LAS3_SPECIAL1,					//400//라스(3)//필살기 1
	MID_LAS3_SPECIAL2,					//401//라스(3)//필살기 2
	MID_LAS3_SPECIAL3,					//402//라스(3)//필살기 3
	MID_LAS3_JOKE,						//403//라스(3)//도발
	MID_LAS3_SELECT,					//404//라스(3)//대기실
//================================라스 2차 전직 끝=========================================//
//==================================라이언 시작============================================//
	MID_RYAN_WAIT,						//405//라이언//기본자세
	MID_RYAN_FATAL,						//406//라이언//페이탈
	MID_RYAN_JOKE,						//407//라이언//도발
	MID_RYAN_WALK,						//408//라이언//걷기
	MID_RYAN_DASH,						//409//라이언//대쉬
	MID_RYAN_JUMP_READY,				//410//라이언//점프준비
	MID_RYAN_JUMP,						//411//라이언//점프
	MID_RYAN_CATCH,						//412//라이언//잡기
	MID_COMMON_CATCH_BY_RYAN,			//413//라이언//잡히기
	MID_RYAN_ATK1,						//414//라이언//공격1
	MID_RYAN_ATK2,						//415//라이언//공격2
	MID_RYAN_ATK3,						//416//라이언//공격3
	MID_RYAN_DASH_ATK1,					//417//라이언//대쉬공격1
	MID_RYAN_DASH_ATK2,					//418//라이언//대쉬공격2
	MID_RYAN_JUMP_ATK1,					//419//라이언//점프공격1
	MID_RYAN_JUMP_ATK2,					//420//라이언//점프공격2
	MID_RYAN_JUMP_ATK2_LAND,			//421//라이언//점프공격2 착지
	MID_RYAN_SPECIAL1,					//422//라이언//필살기1
	MID_RYAN_SPECIAL2,					//423//라이언//필살기2
	MID_RYAN_SPECIAL3,					//424//라이언//필살기3
	MID_RYAN_SELECT,					//425//라이언//대기실
	MID_RYAN_START,						//426//라이언//시작
	MID_RYAN_WINPOS,					//427//라이언//종료
	MID_RYAN_RESULT_WIN,				//428//라이언//결과창 승리
	MID_RYAN_RESULT_LOSE,				//429//라이언//결과창 패배
//===================================라이언 끝=============================================//

	MID_TOTAL,
};

// 플레이어가 걸려있는 마법상태
enum EGCPlayerMagicEffect
{
	EGC_EFFECT_DARKBALL		= 0,
	EGC_EFFECT_ENT_TRUNK,
	EGC_EFFECT_EAGLE_SITE,
	EGC_EFFECT_CONFUSION,
	EGC_EFFECT_BOOST_HEAL,
	EGC_EFFECT_ATHENS_SWORD,
	EGC_EFFECT_DARK_SWORM,
	EGC_EFFECT_BLOOD_BASH,
	EGC_EFFECT_FIRE_SHIELD,
	EGC_EFFECT_BACKSTEP,
	EGC_EFFECT_BACKSTEP_APPEAR,
	EGC_EFFECT_ANKH_OF_RESURRECTION,

	EGC_EFFECT_NUM,
};

enum EMIT_POSITION_TYPE
{
	EPT_BODY_CENTER,
	EPT_BODY_CENTER_MIRROR,
	EPT_SWORD_TIP,
	EPT_SWORD_BODY,
	EPT_SWORD_CENTER,
	EPT_BOW,
	EPT_CROSSBOW_ONE,
	EPT_CROSSBOW_TWO,
	EPT_LASKNIFE_ONE,
	EPT_LASKNIFE_TWO,
	EPT_RSHOULDER,
	EPT_LSHOULDER,
	EPT_RHAND,
	EPT_LHAND,
	EPT_RFOOT,
	EPT_LFOOT,
};

// FRAME정보에 3개의 값 밖에 넣을 수 없으므로.. 이런 식으로 처리합니다.
// PHYSICATKTYPE, PHYSICATKREACTION, POWER 를 넣어줌
enum ePhysicAtkType
{
	PAT_SWORD = 1,
	PAT_LEG_LEFT = 2,
	PAT_LEG_RIGHT = 3,
	PAT_SWORD_LEFT = 4,
};

//사이에 다른 값이 추가되면 모션이 모두 수정되어야 하므로...
//여긴 중간에 절때 끼워넣어서는 안됨
enum ePhysicAtkReaction
{
	PAR_DOWN			= 0,

	PAR_SMALL_DAMAGE	= 1,		//살짝 맞는 동작으로 전환
	PAR_BIG_DAMAGE		= 2,				//크게 맞는 동작으로 전환

	PAR_FRONTDOWN_0		= 3,				//엎어지기... 높이는 N * FACTOR 따라서 PAR_FRONTDOWN_4가 가장 높이 뜨게됨
	PAR_FRONTDOWN_1		= 4,				//떨어지면서 데미지 없음
	PAR_FRONTDOWN_2		= 5,
	PAR_FRONTDOWN_3		= 6,
	PAR_FRONTDOWN_LAST	= 7,

	PAR_FRONTDOWN_DROPDAMAGE_0		= 8,		//엎어지기... 높이는 N * FACTOR 따라서 PAR_FRONTDOWN_4가 가장 높이 뜨게됨
	PAR_FRONTDOWN_DROPDAMAGE_1		= 9,		//떨어지면서 데미지 있음.. 높이에 따라...
	PAR_FRONTDOWN_DROPDAMAGE_2		= 10,
	PAR_FRONTDOWN_DROPDAMAGE_3		= 11,
	PAR_FRONTDOWN_DROPDAMAGE_LAST	= 12,

	PAR_SMALL_DAMAGE_NODOWN			= 13,		//공중에서 맞아도 쓰러지지 않는 작은 데미지 동작
	PAR_BIG_DAMAGE_NODOWN			= 14,		//공중에서 맞아도 쓰러지지 않는 큰 데미지 동작
	PAR_REVERSE_DOWN				= 15,
};

// 유저레벨
enum EGCUserLevel
{
	USERLEVEL_BAD = -1,		// 불량유저
	USERLEVEL_NOMAL = 0,	// 일반유저
	USERLEVEL_HERO = 1,		// 용사의섬 우승자
	USERLEVEL_HIGH = 2,		// 초고수, 방송진행자
	USERLEVEL_ADMIN = 3,	// 운영자
};
// 퍼블리셔 타입
enum PublisherAuthType
{
	AT_NETMARBLE    = 0x00000001,   // 넷마블   - 한국
	AT_NEXON        = 0x00000002,   // 넥슨저팬 - 일본
	AT_GASH         = 0x00000004,   // 감마니아 - 대만, 홍콩
	AT_SSO          = 0x00000008,   // 세가     - 상해, 북경
	AT_LEVELUP      = 0x00000010,   // 레벨업   - 브라질
	AT_NMJ          = 0x00000020,   // 넷마블 재팬
};

enum EGCEventEnum
{
	EVENT_ID_VALENTINE      = 0,
	EVENT_ID_GP_EXP         = 1,
	EVENT_ID_CARNATION      = 2,
	EVENT_ID_RICECAKE       = 3,//송편 이벤트
	EVENT_ID_WORLDCUP		= 4,//월드컵 이벤트
	EVENT_ID_FRIENDSHIP		= 5,//일본 우정의반지 이벤트
	EVENT_ID_SUMMER			= 6,//니니안세트
	EVENT_ID_EGGCOOKIE		= 7,//대만 계란과자이벤트
	EVENT_ID_GP2_PvPMode	= 8,//일본 대전에서 GP2배
	EVENT_ID_PIGHEAD		= 9,//대만 게임내 체력회복아이템이 돼지머리로 나옴
	EVENT_ID_GHOSTMONTH		= 10,//대만 귀신의 달 이벤트
	EVENT_ID_END,
};

#ifdef _ENUM
#undef _ENUM
#define             _ENUM( ID, strError_ )                  ID,
#else
#define             _ENUM( ID, strError_ )                  ID,
#endif

enum GC_CONNECTION_ERROR
{
#include "FailRate_def.h"
};

#endif // _GCENUM_H_
