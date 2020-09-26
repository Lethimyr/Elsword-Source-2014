/** @file   X2World.h
    @desc   interface of X2World
    @date   jintaeks on 2008-10-23, 14:19
            - add KHelpSystem
            - 객체의 이름으로 객체를 찾는 기능 추가
                -- 객체 이름은 메시 파일 이름과 같은 것을 사용
*/

#pragma once

struct WORLDMESH_SYNC
{
	float fTime;
	float fFutureTime;
};

class CX2World
{
#ifdef KHELP_SYSTEM_TEST // 2008-10-23

public:
    DECLARE_HELP_SYSTEM( CX2World );

#endif // KHELP_SYSTEM_TEST // 2008-10-23

	public:
		enum WORLD_ID
		{
			WI_NONE = 0,
			WI_FOREST_OF_EL,				// 폴더 ElTreeDown에 해당하는 것
			WI_TREE_OF_LIGHT_TOP,
			WI_EL_FOREST_MAIN_STAGE2,
			WI_EL_FOREST_MAIN_STAGE3,
			WI_EL_FOREST_MAIN_STAGE4,
			

			WI_TEMP0,						// note!! 테스트용 월드 아이디
			WI_TEMP1,
			WI_TEMP2,
			WI_TEMP3,
			WI_TEMP4,
			WI_TEMP5,

			WI_EL_FOREST_WEST_STAGE1,
			WI_EL_FOREST_WEST_STAGE2,
			WI_EL_FOREST_WEST_STAGE3,
			WI_EL_FOREST_WEST_STAGE1_1,
			WI_EL_FOREST_WEST_STAGE2_1,
			WI_EL_FOREST_MAIN_STAGE2_1,
			WI_EL_FOREST_MAIN_STAGE3_1,

			WI_EL_FOREST_NORTH_STAGE1,
			WI_EL_FOREST_NORTH_STAGE1_1,
			WI_EL_FOREST_NORTH_STAGE1_1B,
			WI_EL_FOREST_NORTH_STAGE2,		
			WI_EL_FOREST_NORTH_STAGE2B,
			WI_EL_FOREST_NORTH_STAGE2_1,
			WI_EL_FOREST_NORTH_STAGE2_1B,
			WI_EL_FOREST_NORTH_STAGE3,
			
			WI_TUTORIAL,						/// 튜토리얼

			WI_NOVICE_FOREST_STAGE1,			/// 초심자의 숲_1
			WI_NOVICE_FOREST_STAGE2,			/// 초심자의 숲_2

			WI_ELDER_BENDERS_CAVE_STAGE1,		/// 엘더_벤더스 동굴_1
			WI_ELDER_BENDERS_CAVE_STAGE1_1,		/// 엘더_벤더스 동굴_1_1
			WI_ELDER_BENDERS_CAVE_STAGE2,		/// 엘더_벤더스_동굴_2
			WI_ELDER_BENDERS_CAVE_STAGE2_1,		/// 엘더_벤더스_동굴_2_1
			WI_ELDER_BENDERS_CAVE_STAGE3,		/// 엘더_벤더스_동굴_3

			WI_ELDER_NATURE_CAVE_STAGE1,		/// 엘더_
			WI_ELDER_NATURE_CAVE_STAGE1_1,		/// 엘더_
			WI_ELDER_NATURE_CAVE_STAGE2,		/// 엘더_
			WI_ELDER_NATURE_CAVE_STAGE2_1,		/// 엘더_
			WI_ELDER_NATURE_CAVE_STAGE3,		/// 엘더_

			WI_EL_FOREST_EXTRA_STAGE1,			/// 엘의 숲_1
			WI_EL_FOREST_EXTRA_STAGE2,			/// 엘의_숲_2
			WI_EL_FOREST_EXTRA_STAGE3,			/// 엘의_숲_3
			WI_EL_FOREST_EXTRA_STAGE4,			/// 엘의_숲_4

			WI_EL_FOREST_EXTRA_CAVE_STAGE1,		/// 엘의_숲_동굴_1
			WI_EL_FOREST_EXTRA_CAVE_STAGE2,		/// 엘의_숲_동굴_2
			WI_EL_FOREST_EXTRA_CAVE_STAGE3,		/// 엘의_숲_동굴_3
			WI_EL_FOREST_EXTRA_CAVE_STAGE4,		/// 엘의_숲_동굴_4
			WI_EL_FOREST_EXTRA_CAVE_STAGE5,		/// 엘의_숲_동굴_5

			WI_SQUARE,							/// 광장

			WI_ELDER_DRUID_FOREST_STAGE1,		/// 엘더_드루이드_숲_1
			WI_ELDER_DRUID_FOREST_STAGE1_1A,	/// 엘더_드루이드_숲_1A
			WI_ELDER_DRUID_FOREST_STAGE1_1B,	/// 엘더_드루이드_숲_1B
			WI_ELDER_DRUID_FOREST_STAGE2A,		/// 엘더_드루이드_숲_2A
			WI_ELDER_DRUID_FOREST_STAGE2B,		/// 엘더_드루이드_숲_2B
			WI_ELDER_DRUID_FOREST_STAGE2_1A,	/// 엘더_드루이드_숲_2_1A
			WI_ELDER_DRUID_FOREST_STAGE2_1B,	/// 엘더_드루이드_숲_2_1B
			WI_ELDER_DRUID_FOREST_STAGE3,		/// 엘더_드루이드_숲_3

			WI_ELDER_EXTRA_CAVE_STAGE1,			/// 엘더_동굴_1
			WI_ELDER_EXTRA_CAVE_STAGE2,			/// 엘더_동굴_2

			WI_EL_FOREST_NORTH_STAGE1_1B_MIRROR,	/// 엘의_숲_북쪽_1_1_
			WI_EL_FOREST_NORTH_STAGE2_1_MIRROR,		/// 엘의_숲_북쪽_2_1_

			WI_ELDER_WALLY_CASTLE_ROOF_STAGE1,		/// 엘더_월리의_성_외곽_1
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE1_1,	/// 엘더_월리의_성_외곽_1_1
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE2,		/// 엘더_월리의_성_외곽_2
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE2_1,	/// 엘더_월리의_성_외곽_2_1
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE3,		/// 엘더_월리의_성_외곽_3
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE4,		/// 엘더_월리의_성_외곽_4


			WI_ELDER_WALLY_CASTLE_CENTER_STAGE1,	/// 엘더_윌리의_성_1
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE1_1,	/// 엘더_윌리의_성_1_1
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE2,	/// 엘더_윌리의_성_2
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE2_1,	/// 엘더_윌리의_성_2_1
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE3,	/// 엘더_윌리의_성_3
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE4,	/// 엘더_윌리의-성_4


			WI_ELDER_BELOW_PASSAGE_STAGE1,			/// 엘더_지하수로_1
			WI_ELDER_BELOW_PASSAGE_STAGE2A,			/// 엘더_지하수로_2A
			WI_ELDER_BELOW_PASSAGE_STAGE2B,			/// 엘더_지하수로_2B
			WI_ELDER_BELOW_PASSAGE_STAGE3A,			/// 엘더_지하수로_3A
			WI_ELDER_BELOW_PASSAGE_STAGE3B,			/// 엘더_지하수로_3B
			WI_ELDER_BELOW_PASSAGE_STAGE4,			/// 엘더_지하수로_4
			WI_ELDER_BELOW_PASSAGE_STAGE2_1A,		/// 엘더_지하수로_2_1A
			WI_ELDER_BELOW_PASSAGE_STAGE2_1B,		/// 엘더_지하수로_2_1B
			WI_ELDER_BELOW_PASSAGE_STAGE3_1A,		/// 엘더_지하수로_3_1A
			WI_ELDER_BELOW_PASSAGE_STAGE3_1B,		/// 엘더_지하수로_3_1B

			WI_MONSTER_TEST_WORLD,					/// 몬스터_테스트

			WI_BESMA_DRAGON_ROAD_STAGE1,			/// 베스마_용의_길_1
			WI_BESMA_DRAGON_ROAD_STAGE2A,			/// 베스마_용의_길_2A
			WI_BESMA_DRAGON_ROAD_STAGE2B,			/// 베스마_용의_길_2B
			WI_BESMA_DRAGON_ROAD_STAGE2_1A,			/// 베스마_용의_길_2_1A
			WI_BESMA_DRAGON_ROAD_STAGE2_1B,			/// 베스마_용의_길_2_1B
			WI_BESMA_DRAGON_ROAD_STAGE3,			/// 베스마_용의_길_3
			WI_BESMA_DRAGON_ROAD_STAGE4,			/// 베스마_용의_길_4

			WI_BESMA_MINE_STAGE1,					/// 베스마_리치광산_1
			WI_BESMA_MINE_STAGE2A,					/// 베스마_리치광산_2A
			WI_BESMA_MINE_STAGE2_1A,				/// 베스마_리치광산_2_1A
			WI_BESMA_MINE_STAGE2_2A,				/// 베스마_리치광산_2_2A
			WI_BESMA_MINE_STAGE3A,					/// 베스마_리치광산_3A
			WI_BESMA_MINE_STAGE4,					/// 베스마_리치광산_4

			WI_TRAINING_CENTER_STAGE1,				/// 훈련소_1
			WI_TRAINING_CENTER_STAGE2,				/// 훈련소_2
			WI_TRAINING_CENTER_STAGE3,				/// 훈련소_3
			WI_TRAINING_CENTER_STAGE4,				/// 훈련소_4

			WI_BESMA_LAKE_STAGE1,					/// 베스마_호수_1
			WI_BESMA_LAKE_STAGE2A,					/// 베스마_호수_2A
			WI_BESMA_LAKE_STAGE2_1A,				/// 베스마_호수_2_1A
			WI_BESMA_LAKE_STAGE3A,					/// 베스마_호수_3A
			WI_BESMA_LAKE_STAGE3_1A,				/// 베스마_호수_3_1A
			WI_BESMA_LAKE_STAGE4,					/// 베스마_호수_4


			WI_BESMA_LAKE_NIGHT_STAGE1,				/// 베스마_호수_밤_1
			WI_BESMA_LAKE_NIGHT_STAGE2A,			/// 베스마_호수_밤_2A
			WI_BESMA_LAKE_NIGHT_STAGE2_1A,			/// 베스마_호수_밤_2_1A
			WI_BESMA_LAKE_NIGHT_STAGE3A,			/// 베스마_호수_밤_3A
			WI_BESMA_LAKE_NIGHT_STAGE3_1A,			/// 베스마_호수_밤_3_1A
			WI_BESMA_LAKE_NIGHT_STAGE4,				/// 베스마_호수_밤_4


			WI_BESMA_DRAGON_NEST_STAGE1,			/// 베스마_용의_둥지_1
			WI_BESMA_DRAGON_NEST_STAGE2,			/// 베스마_용의_둥지_2
			WI_BESMA_DRAGON_NEST_STAGE2_1,			/// 베스마_용의_둥지_2_1
			WI_BESMA_DRAGON_NEST_STAGE2_1B,			/// 베스마_용의_둥지_2_1B
			WI_BESMA_DRAGON_NEST_STAGE3,			/// 베스마_용의_둥지_3
			WI_BESMA_DRAGON_NEST_STAGE4,			/// 베스마_용의_둥지_4

			WI_BESMA_AIRSHIP_STAGE1,				/// 베스마_수송비공정_1
			WI_BESMA_AIRSHIP_STAGE2A,				/// 베스마_수송비공정_2A
			WI_BESMA_AIRSHIP_STAGE2B,				/// 베스마_수송비공정_2B
			WI_BESMA_AIRSHIP_STAGE2_1A,				/// 베스마_수송비공정_2_1A
			WI_BESMA_AIRSHIP_STAGE2_1B,				/// 베스마_수송비공정_2_1B
			WI_BESMA_AIRSHIP_STAGE3A,				/// 베스마_수송비공정_3A
			WI_BESMA_AIRSHIP_STAGE4,				/// 베스마_수송비공정_4
			WI_BESMA_AIRSHIP_STAGE5,				/// 베스마_수송비공정_5

			WI_BESMA_MINE2_STAGE1,					/// 베스마_리치광산(수정)_1
			WI_BESMA_MINE2_STAGE2A,					/// 베스마_리치광산(수정)_2A
			WI_BESMA_MINE2_STAGE2_1A,				/// 베스마_리치광산(수정)_2_1A
			WI_BESMA_MINE2_STAGE2_2A,				/// 베스마_리치광산(수정)_2_2A
			WI_BESMA_MINE2_STAGE3A,					/// 베스마_리치광산(수정)_3A
			WI_BESMA_MINE2_STAGE4,					/// 베스마_리치광산(수정)_4

			WI_BESMA_LAKE_CANYON_STAGE3_1A,			/// 베스마_호수_협곡_3_1A
			WI_BESMA_LAKE_CANYON_STAGE3A,			/// 베스마_호수_협곡_3A
			WI_BESMA_LAKE_CANYON_STAGE4A,			/// 베스마_호수_협곡_4A
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE2_1A,	/// 베스마_용의_길_협곡_2_1A
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE2_1B,	/// 베스마_용의_길_협곡_2_1B
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE2B,	/// 베스마_용의_길_협곡_2B
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE3,		/// 베스마_용의_길_협곡_3
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE4,		/// 베스마_용의_길_협곡_4


			
			WI_ALTERA_PLAIN_STAGE1,					/// 알테라_평원_1
			WI_ALTERA_PLAIN_STAGE2,					/// 알테라_평원_2
			WI_ALTERA_PLAIN_STAGE2B,				/// 알테라_평원_2B
			WI_ALTERA_PLAIN_STAGE2_1A,				/// 알테라_평원_2_1A
			WI_ALTERA_PLAIN_STAGE2_1B,				/// 알테라_평원_2_1B
			WI_ALTERA_PLAIN_STAGE3,					/// 알테라_평원_3
			WI_ALTERA_PLAIN_STAGE4,					/// 알테라_평원_4


			WI_ALTERA_BATTLE_AIR_SHIP_STAGE1,		/// 알테라_블랙크로우호_1
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE2,		/// 알테라_블랙크로우호_2
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE3,		/// 알테라_블랙크로우호_3
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE4,		/// 알테라_블랙크로우호_4
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE5,		/// 알테라_블랙크로우호_5
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE6,		/// 알테라_블랙크로우호_6

			WI_WORLDTOOL_DEFAULT,						// worldtool에서 쓰는 default world


			WI_ALTERA_BELOW_TUNNEL_STAGE1,			/// 알테라_운송_터널_1
			WI_ALTERA_BELOW_TUNNEL_STAGE2A,			/// 알테라_운송_터널_2A
			WI_ALTERA_BELOW_TUNNEL_STAGE2_1A,		/// 알테라_운송_터널_2_1A
			WI_ALTERA_BELOW_TUNNEL_STAGE2_2A,		/// 알테라_운송_터널_2_2A
			WI_ALTERA_BELOW_TUNNEL_STAGE3A,			/// 알테라_운송_터널_3A
			WI_ALTERA_BELOW_TUNNEL_STAGE4,			/// 알테라_운송_터널_4

            WI_BESMA_BESMA_LAKE_PVPMAP,				/// 베스마_호수_대전

            WI_SQUARE_MARKET,						/// 광장_마켓

			WI_ELDER_WALLY_CASTLE_LAB_STAGE1,		/// 엘더_월리의_성_지하연구실_1
			WI_ELDER_WALLY_CASTLE_LAB_STAGE2,		/// 엘더_월리의_성_지하연구실_2
			WI_ELDER_WALLY_CASTLE_LAB_STAGE3,		/// 엘더_월리의_성_지하연구실_3
			WI_ELDER_WALLY_CASTLE_LAB_STAGE4,		/// 엘더_월리의_성_지하연구실_4
            WI_ELDER_WALLY_CASTLE_LAB_STAGE5,		/// 엘더_월리의_성_지하연구실_5

            WI_BESMA_AIR_SHIP_PVPMAP,				/// 베스마_수송비공정_대전

            WI_EL_TREE_MAIN_TEST_DUNGEON,			/// 엘의_나무_주_테스트_던전


			WI_ALTERA_PLAIN_RECYCLE_STAGE1,			/// 알테라_회귀의_평원_1
			WI_ALTERA_PLAIN_RECYCLE_STAGE2,			/// 알테라_회귀의_평원_2
			WI_ALTERA_PLAIN_RECYCLE_STAGE2B,		/// 알테라_회귀의_평원_2B
			WI_ALTERA_PLAIN_RECYCLE_STAGE2_1A,		/// 알테라_회귀의_평원_2_1A
			WI_ALTERA_PLAIN_RECYCLE_STAGE2_1B,		/// 알테라_회귀의_평원_2_1B
			WI_ALTERA_PLAIN_RECYCLE_STAGE3,			/// 알테라_회귀의_평원_3
			WI_ALTERA_PLAIN_RECYCLE_STAGE4,			/// 알테라_회귀의_평원_4

            WI_ALTERA_NASOD_FACTORY_STAGE1, 		/// 알테라_나소드_생산기지_1
            WI_ALTERA_NASOD_FACTORY_STAGE2, 		/// 알테라_나소드_생산기지_2
            WI_ALTERA_NASOD_FACTORY_STAGE3, 		/// 알테라_나소드_생산기지_3
            WI_ALTERA_NASOD_FACTORY_STAGE4, 		/// 알테라_나소드_생산기지_4
            WI_ALTERA_NASOD_FACTORY_STAGE5, 		/// 알테라_나소드_생산기지_5
            WI_ALTERA_NASOD_FACTORY_BOSS, 			/// 알테라_나소드_생산기지_보스
			
			WI_BESMA_SECRET_STAGE1,					/// 베스마_용의_둥지_나락_1
			WI_BESMA_SECRET_STAGE2,					/// 베스마_용의_둥지_나락_2
			WI_BESMA_SECRET_STAGE2_1,				/// 베스마_용의_둥지_나락_2_1
			WI_BESMA_SECRET_STAGE3,					/// 베스마_용의_둥지_나락_3
			WI_BESMA_SECRET_STAGE3_1,				/// 베스마_용의_둥지_나락_3_1
			WI_BESMA_SECRET_STAGE4,					/// 베스마_용의_둥지_나락_4
			WI_BESMA_SECRET_STAGE5,					/// 베스마_용의_둥지_나락_5
			WI_BESMA_SECRET_STAGE6,					/// 베스마_용의_둥지_나락_6
			WI_BESMA_SECRET_STAGE7,					/// 베스마_용의_둥지_나락_7
			WI_BESMA_SECRET_BOSS,					/// 베스마_용의_둥지_나락_보스


			WI_ARCADE_RUBEN_STAGE1,					/// 아케이드_루벤_1
			WI_ARCADE_RUBEN_STAGE1_1,				/// 아케이드_루벤_1_1
			WI_ARCADE_RUBEN_STAGE1_2,				/// 아케이드_루벤_1_2
			WI_ARCADE_RUBEN_STAGE2,					/// 아케이드_루벤_2
			WI_ARCADE_RUBEN_STAGE3,					/// 아케이드_루벤_3
			WI_ARCADE_RUBEN_STAGE4,					/// 아케이드_루벤_4
			WI_ARCADE_RUBEN_STAGE5,					/// 아케이드_루벤_5
			WI_ARCADE_RUBEN_STAGE6,					/// 아케이드_루벤_6
			WI_ARCADE_RUBEN_STAGE7,					/// 아케이드_루벤_7
			WI_ARCADE_RUBEN_STAGE8,					/// 아케이드_루벤_8
			WI_ARCADE_RUBEN_STAGE8_1,				/// 아케이드_루벤_8_1
			WI_ARCADE_RUBEN_STAGE9,					/// 아케이드_루벤_9
			WI_ARCADE_RUBEN_STAGE10,				/// 아케이드_루벤_10
			WI_ARCADE_RUBEN_STAGE11,				/// 아케이드_루벤_11
			WI_ARCADE_RUBEN_STAGE12,				/// 아케이드_루벤_12
			WI_ARCADE_RUBEN_STAGE13,				/// 아케이드_루벤_13
			WI_ARCADE_RUBEN_STAGE14,				/// 아케이드_루벤_14
			WI_ARCADE_RUBEN_STAGE15,				/// 아케이드_루벤_15
			WI_ARCADE_RUBEN_STAGE16,				/// 아케이드_루벤_16
			WI_ARCADE_RUBEN_STAGE17,				/// 아케이드_루벤_17
			WI_ARCADE_RUBEN_STAGE18,				/// 아케이드_루벤_18
			WI_ARCADE_RUBEN_STAGE19,				/// 아케이드_루벤_19
			WI_ARCADE_RUBEN_STAGE20,				/// 아케이드_루벤_20
			WI_ARCADE_RUBEN_STAGE21,				/// 아케이드_루벤_21
			WI_ARCADE_RUBEN_STAGE22,				/// 아케이드_루벤_22
			WI_ARCADE_RUBEN_STAGE23,				/// 아케이드_루벤_23


			WI_ARCADE_ELDER_STAGE1,					/// 아케이드_엘더_1
			WI_ARCADE_ELDER_STAGE2,					/// 아케이드_엘더_2
			WI_ARCADE_ELDER_STAGE3,					/// 아케이드_엘더_3
			WI_ARCADE_ELDER_STAGE3_1,				/// 아케이드_엘더_3_1
			WI_ARCADE_ELDER_STAGE4,					/// 아케이드_엘더_4
			WI_ARCADE_ELDER_STAGE5,					/// 아케이드_엘더_5
			WI_ARCADE_ELDER_STAGE5_1,				/// 아케이드_엘더_5_1
			WI_ARCADE_ELDER_STAGE6,					/// 아케이드_엘더_6
			WI_ARCADE_ELDER_STAGE7,					/// 아케이드_엘더_7
			WI_ARCADE_ELDER_STAGE8,					/// 아케이드_엘더_8
			WI_ARCADE_ELDER_STAGE9,					/// 아케이드_엘더_9
			WI_ARCADE_ELDER_STAGE10,				/// 아케이드_엘더_10
			WI_ARCADE_ELDER_STAGE11,				/// 아케이드_엘더_11
			WI_ARCADE_ELDER_STAGE12,				/// 아케이드_엘더_12
			WI_ARCADE_ELDER_STAGE13,				/// 아케이드_엘더_13
			WI_ARCADE_ELDER_STAGE14,				/// 아케이드_엘더_14
			WI_ARCADE_ELDER_STAGE15,				/// 아케이드_엘더_15

			WI_ARCADE_ELDER_STAGE30,				/// 아케이드_엘더_30
			WI_ARCADE_ELDER_STAGE31,				/// 아케이드_엘더_31
			WI_ARCADE_ELDER_STAGE32,				/// 아케이드_엘더_32
			WI_ARCADE_ELDER_STAGE33,				/// 아케이드_엘더_33
			WI_ARCADE_ELDER_STAGE34,				/// 아케이드_엘더_34
			WI_ARCADE_ELDER_STAGE35,				/// 아케이드_엘더_35


			WI_SUMMER_2008_PVP,						/// 여름_2008_대전


			WI_ALTERA_CORE_STAGE1,					/// 알테라_코어_1
			WI_ALTERA_CORE_STAGE1_MIDDLE,			/// 알테라_코어_1_이동
			WI_ALTERA_CORE_STAGE2,					/// 알테라_코어_2
			WI_ALTERA_CORE_STAGE2_MIDDLE,			/// 알테라_코어_2_이동
			WI_ALTERA_CORE_STAGE3,					/// 알테라_코어_3
			WI_ALTERA_CORE_STAGE3_MIDDLE,			/// 알테라_코어_3_이동
			WI_ALTERA_CORE_STAGE4,					/// 알테라_코어_4
			WI_ALTERA_CORE_STAGE4_MIDDLE,			/// 알테라_코어_4_이동
			WI_ALTERA_CORE_BOSS_STAGE,				/// 알테라_코어_보스

			WI_EL_FOREST_MAIN_STAGE1,				/// 엘의_나무_주_1

			WI_TRAINING_CENTER_FREESTAGE,			/// 훈련소_자유

			WI_EL_FOREST_MONKEY_STAGE1,				/// 엘의_나무_원숭이_1
			WI_EL_FOREST_MONKEY_STAGE2,				/// 엘의_나무_원숭이_2
			WI_EL_FOREST_MONKEY_STAGE3,				/// 엘의_나무_원숭이_3

	
			WI_VILLAGE_RUBEN,						/// 루벤_마을
			WI_DUNGEON_GATE_RUBEN,					/// 루벤_던전_게이트


			WI_ELDER_HALLOWEEN_CARNIVAL_STAGE1,		/// 엘더_할로윈_축제_1
			WI_ELDER_HALLOWEEN_CARNIVAL_STAGE2,		/// 엘더_할로윈_축제_2


			WI_VILLAGE_BESMA,						/// 베스마_마을
			WI_VILLAGE_ELDER,						/// 엘더_마을
			WI_DUNGEON_GATE_ELDER,					/// 엘더_던전_게이트

			WI_VILLAGE_ALTERA,						/// 알테라_마을
			WI_DUNGEON_GATE_BESMA,					/// 베스마_던전_게이트
			WI_DUNGEON_GATE_ALTERA,					/// 알테라_던전_게이트
			
			WI_VILLAGE_PEITA,						/// 페이타_마을

			WI_PEITA_OFFERINGS_STAGE1,				/// 페이타_봉헌의신전_1
			WI_PEITA_OFFERINGS_STAGE2,				/// 페이타_봉헌의신전_2
			WI_PEITA_OFFERINGS_STAGE3,				/// 페이타_봉헌의신전_3
			WI_PEITA_OFFERINGS_STAGE4,				/// 페이타_봉헌의신전_4
			WI_PEITA_OFFERINGS_STAGE_BOSS,			/// 페이타_봉헌의신전_보스

			WI_PEITA_SPIRAL_CORRIDOR_STAGE1,		/// 페이타_나선_회랑_1
			WI_PEITA_SPIRAL_CORRIDOR_STAGE2,		/// 페이타_나선_회랑_2
			WI_PEITA_SPIRAL_CORRIDOR_STAGE3,		/// 페이타_나선_회랑_3
			WI_PEITA_SPIRAL_CORRIDOR_STAGE4,		/// 페이타_나선_회랑_4
			WI_PEITA_SPIRAL_CORRIDOR_STAGE_BOSS,	/// 페이타_나선_회랑_보스


			WI_EVE_TUTORIAL,						/// 이브_튜토리얼


			WI_DUNGEON_LOUNGE_RUBEN,				/// 루벤_던전_휴게실
			WI_DUNGEON_LOUNGE_ELDER,				/// 엘더_던전-휴게실
			WI_DUNGEON_LOUNGE_BESMA,				/// 베스마_던전_휴게실
			WI_DUNGEON_LOUNGE_ALTERA,				/// 알테라_던전_휴게실
			WI_DUNGEON_LOUNGE_PEITA,				/// 페이타_던전_휴게실


			WI_ALTERA_SECRET_STAGE1,				/// 알테라_운송터널_오염구역_1
			WI_ALTERA_SECRET_STAGE2,				/// 알테라_운송터널_오염구역_2
			WI_ALTERA_SECRET_STAGE3,				/// 알테라_운송터널_오염구역_3
			WI_ALTERA_SECRET_STAGE4,				/// 알테라_운송터널_오염구역_4
			WI_ALTERA_SECRET_BOSS,					/// 알테라_운송터널_오염구역_보스


			WI_2009_NEW_YEAR_EVENT_STAGE1,			/// 2009_새해_이벤트_1
			WI_2009_NEW_YEAR_EVENT_BOSS,			/// 2009_새해_이벤트_보스


			WI_PEITA_OFFERING_PVP_STAGE,			/// 페이타_봉헌의신전_대전
			WI_PEITA_SPIRAL_CORRIDOR_PVP_STAGE,		/// 페이타_나선_회랑_대전

			WI_PEITA_CHAPEL_STAGE1,
			WI_PEITA_CHAPEL_STAGE2_1,
			WI_PEITA_CHAPEL_STAGE2_2,
			WI_PEITA_CHAPEL_STAGE2_3,
			WI_PEITA_CHAPEL_STAGE3,
			WI_PEITA_CHAPEL_STAGE4,
			WI_PEITA_CHAPEL_STAGE5,
			WI_PEITA_CHAPEL_STAGE_BOSS,

			WI_PEITA_UNDER_GARDEN_STAGE1,
			WI_PEITA_UNDER_GARDEN_STAGE2,
			WI_PEITA_UNDER_GARDEN_STAGE_BOSS,

			WI_PEITA_TOWER_HEART_STAGE1,
			WI_PEITA_TOWER_HEART_STAGE2,
			WI_PEITA_TOWER_HEART_STAGE3,
			WI_PEITA_TOWER_HEART_STAGE_BOSS,

			WI_PEITA_OFFERINGS_ALTER_STAGE1,
			WI_PEITA_OFFERINGS_ALTER_STAGE2,
			WI_PEITA_OFFERINGS_ALTER_STAGE3,
			WI_PEITA_OFFERINGS_ALTER_STAGE4,
			WI_PEITA_OFFERINGS_ALTER_STAGE_BOSS,

			WI_ARCADE_NEW_STAGE_REST,	

			WI_VELDER_THIRD_DWELLING_STAGE1,
			WI_VELDER_THIRD_DWELLING_STAGE2,
			WI_VELDER_THIRD_DWELLING_STAGE3,
			WI_VELDER_THIRD_DWELLING_STAGE_BOSS,

			WI_VELDER_BRIDGE_HOPE_STAGE1,	
			WI_VELDER_BRIDGE_HOPE_STAGE2,	
			WI_VELDER_BRIDGE_HOPE_STAGE2_1,	
			WI_VELDER_BRIDGE_HOPE_STAGE3,	
			WI_VELDER_BRIDGE_HOPE_STAGE3_1,	
			WI_VELDER_BRIDGE_HOPE_STAGE4,	
			WI_VELDER_BRIDGE_HOPE_STAGE_BOSS,	

			WI_VILLAGE_VELDER,
			WI_DUNGEON_GATE_VELDER,
			WI_DUNGEON_LOUNGE_VELDER,

			WI_NEW_ELDER_BELOW_PASSAGE_STAGE3_1B,
			WI_NEW_ELDER_WALLY_CASTLE_ROOF_STAGE1_1,
			WI_NEW_ELDER_WALLY_CASTLE_ROOF_STAGE2,
			WI_NEW_ELDER_WALLY_CASTLE_ROOF_STAGE2_1,			
			WI_NEW_ELDER_WALLY_CASTLE_ROOF_STAGE3,

			WI_NEW_ELDER_WALLY_CASTLE_CENTER_STAGE2_1,
			WI_NEW_ELDER_WALLY_CASTLE_CENTER_STAGE1_1, 

			WI_NEW_BESMA_DRAGON_ROAD_STAGE1,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE2A,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE2_1B,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE4,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE2B,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE2_1A,
			WI_NEW_BESMA_LAKE_STAGE1,
			WI_NEW_BESMA_LAKE_STAGE2A,

			WI_NEW_BESMA_DRAGON_NEST_STAGE1,
			WI_NEW_BESMA_DRAGON_NEST_STAGE2_1B,

			WI_ELDER_BRIDGE_HOPE_PVPMAP,
			WI_WINTER_PVPMAP,

			WI_NEW_BESMA_LAKE_NIGHT_STAGE1,
			WI_NEW_BESMA_LAKE_NIGHT_STAGE2A,

			WI_NEW_BESMA_AIRSHIP_STAGE2A,
			WI_NEW_BESMA_MINE_STAGE2_2A,
			WI_NEW_BESMA_MINE_STAGE2A,			

			WI_NEW_BESMA_AIRSHIP_STAGE1,

			WI_EVENT_TREE_DAY_STAGE1,
			WI_EVENT_TREE_DAY_STAGE2,
			WI_EVENT_TREE_DAY_STAGE3,
			WI_EVENT_TREE_DAY_STAGE_BOSS,

			// 비던 개편 추가 월드
			WI_BESMA_HELL_EXTRA_BOSS,
			WI_ELDER_SECRET_STAGE4,
			WI_ELDER_SECRET_BOSS,
			WI_BESMA_HELL_STAGE1,
			WI_BESMA_HELL_STAGE2,
			WI_BESMA_HELL_STAGE3_1,
			WI_BESMA_HELL_BOSS,

			// 비던 개편 엑스트라 모드 월드
			WI_BESMA_HELL_EXTRA_STAGE1,
			WI_BESMA_HELL_EXTRA_STAGE2,
			WI_BESMA_HELL_EXTRA_STAGE3,
			WI_BESMA_HELL_EXTRA_STAGE4,
			WI_BESMA_HELL_EXTRA_STAGE5,

			// 엘더 엑스트라 월드
			WI_ELDER_HELL_EXTRA_STAGE1,
			WI_ELDER_HELL_EXTRA_STAGE2,
			WI_ELDER_HELL_EXTRA_STAGE3,
			WI_ELDER_HELL_EXTRA_STAGE4,
			WI_ELDER_HELL_EXTRA_STAGE5, 

			WI_ALTERA_HELL_EXTRA_STAGE1,
			WI_ALTERA_HELL_EXTRA_STAGE2,
			WI_ALTERA_HELL_EXTRA_STAGE3,
			WI_ALTERA_HELL_EXTRA_STAGE4,
			WI_ALTERA_HELL_EXTRA_STAGE5, 

            // 루벤마을 엘의나무
			WI_RUBEN_EL_TREE_STAGE1,
			WI_RUBEN_EL_TREE_STAGE2,
			WI_RUBEN_EL_TREE_BOSS_STAGE,

			WI_RUBEN_RUIN_OF_ELF_STAGE1,
			WI_RUBEN_RUIN_OF_ELF_STAGE2,
			WI_RUBEN_RUIN_OF_ELF_BOSS_STAGE,


			WI_RUBEN_SWAMP_STAGE1,
			WI_RUBEN_SWAMP_STAGE2,
			WI_RUBEN_SWAMP_STAGE3,
			WI_RUBEN_SWAMP_STAGE3_1,
			WI_RUBEN_SWAMP_BOSS_STAGE,

			WI_VELDER_PALACE_ENTRANCE_STAGE1,
			WI_VELDER_PALACE_ENTRANCE_STAGE2,
			WI_VELDER_PALACE_ENTRANCE_STAGE3,
			WI_VELDER_PALACE_ENTRANCE_STAGE_BOSS,

			WI_VELDER_BRIDGE_BURNING_STAGE1,
			WI_VELDER_BRIDGE_BURNING_STAGE2,

			WI_VELDER_BRIDGE_BURNING_STAGE2_1,
			WI_VELDER_BRIDGE_BURNING_STAGE3,
			WI_VELDER_BRIDGE_BURNING_BOSS_STAGE, 

			WI_VELDER_MARKET_STAGE1,
			WI_VELDER_MARKET_STAGE2,
			WI_VELDER_MARKET_STAGE3,
			WI_VELDER_MARKET_BOSS_STAGE,
			WI_VELDER_MARKET_STAGE2_1,

			WI_VELDER_GATE_STAGE1,
			WI_VELDER_GATE_STAGE2,
			WI_VELDER_GATE_STAGE3,
			WI_VELDER_GATE_BOSS_STAGE,
			WI_VELDER_GATE_STAGE2_1,
			WI_VELDER_GATE_LOUNGE,

			//{{ JHKang / 강정훈 / 2010.10.19 / 할로윈 이벤트 관련 추가 Stage
			WI_EVENT_HALLOWEEN_STAGE1,
			WI_EVENT_HALLOWEEN_STAGE2,
			WI_EVENT_HALLOWEEN_STAGE3,
			WI_EVENT_HALLOWEEN_STAGE4,
			WI_EVENT_HALLOWEEN_STAGE5,
			WI_EVENT_HALLOWEEN_STAGE_BOSS,
			//}} JHKang / 강정훈 / 2010.10.19 / 할로윈 이벤트 관련 추가 Stage

			WI_VILLAGE_HAMEL,				/// 하멜 마을
			WI_DUNGEON_GATE_HAMEL,			/// 하멜 던전 게이트
			WI_DUNGEON_LOUNGE_HAMEL,		/// 하멜 던전 라운지

			WI_BATTLE_FIELD_VELDER_SHIP_STAGE,	/// 벨더 --> 하멜 전투함
			WI_BATTLE_FIELD_HAMEL_SHIP_STAGE,		/// 하멜 --> 벨더 전투함

			//{{ JHKang / 강정훈 / 2011.1.10 / 레시암 근교 던전
			WI_HAMEL_RESIAM_STAGE1,
			WI_HAMEL_RESIAM_STAGE2,
			WI_HAMEL_RESIAM_STAGE3,
			WI_HAMEL_RESIAM_BOSS_STAGE,
			//}} JHKang / 강정훈 / 2011.1.10 / 레시암 근교 던전

			//{{ JHKang / 강정훈 / 2011.1.12 / 고대 수로
			WI_HAMEL_WATERWAY_STAGE1,
			WI_HAMEL_WATERWAY_STAGE2,
			WI_HAMEL_WATERWAY_STAGE3,
			WI_HAMEL_WATERWAY_STAGE4,
			WI_HAMEL_WATERWAY_BOSS_STAGE,
			//{{ JHKang / 강정훈 / 2011.1.12 / 고대 수로

			//{{ JHKang / 강정훈 / 2011.1.14 / 가라앉은 레시암
			WI_HAMEL_RESIAM_UNDERWATER_STAGE1,
			WI_HAMEL_RESIAM_UNDERWATER_STAGE2,
			WI_HAMEL_RESIAM_UNDERWATER_STAGE3,
			WI_HAMEL_RESIAM_UNDERWATER_STAGE4,
			WI_HAMEL_RESIAM_UNDERWATER_BOSS_STAGE,
			//}} JHKang / 강정훈 / 2011.1.14 / 가라앉은 레시암

			//{{ JHKang / 강정훈 / 2011.1.20 / 고대 수로 중심부
			WI_HAMEL_WATERWAY_CORE_STAGE1,
			WI_HAMEL_WATERWAY_CORE_STAGE2,
			WI_HAMEL_WATERWAY_CORE_STAGE3,
			WI_HAMEL_WATERWAY_CORE_STAGE4,
			WI_HAMEL_WATERWAY_CORE_STAGE4_1,
			WI_HAMEL_WATERWAY_CORE_STAGE5,
			WI_HAMEL_WATERWAY_CORE_BOSS_STAGE,
			//}} JHKang / 강정훈 / 2011.1.20 / 고대 수로 중심부

			WI_HAMEL_RESIAM_STAGE3_1,		/// 레시암 스테이지 구분용 월드
			
			//{{ JHKang / 강정훈 / 2011.04.18 / 디펜스 던전 스테이지
			WI_DEFENCE_DUNGEON_NORMAL_STAGE,	/// 일반
			WI_DEFENCE_DUNGEON_HARD_STAGE,		/// 어려움
			WI_DEFENCE_DUNGEON_EXPERT_STAGE,	/// 매우 어려움
			//}}

			//{{ JHKang / 강정훈 / 2011.05.17 / 마그만타의 동굴(5번 던전)
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE1,
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE2,
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE3,
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE4,
			WI_HAMEL_CAVE_OF_MAGMANTA_BOSS_STAGE,
			//}}
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE3_1,

			//{{ JHKang / 강정훈 / 2011.6.14 / 얼어붙은 물의 사원
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE1,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE2,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE2_1,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE3,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE3_1,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE4,
			WI_HAMEL_FROZEN_WATER_TEMPLE_B0SS_STAGE,
			//}}

			//{{ JHKang / 강정훈 / 2011.6.29 / 얼어붙은 물의 전당
			WI_HAMEL_FROZEN_WATER_HALL_STAGE1,
			WI_HAMEL_FROZEN_WATER_HALL_STAGE2,
			WI_HAMEL_FROZEN_WATER_HALL_STAGE3,
			WI_HAMEL_FROZEN_WATER_HALL_STAGE4,
			WI_HAMEL_FROZEN_WATER_HALL_STAGE5,
			WI_HAMEL_FROZEN_WATER_HALL_BOSS_STAGE,
			//}}

			//{{ JHKang / 강정훈 / 2011.8.24 / 벨더 비밀 던전
			WI_VELDER_SECRET_STAGE1,
			WI_VELDER_SECRET_STAGE2,
			WI_VELDER_SECRET_STAGE3,
			WI_VELDER_SECRET_STAGE4, 
			WI_VELDER_SECRET_STAGE5,
			WI_VELDER_SECRET_BOSS_STAGE,

			WI_VELDER_SECRET_EXTRA_STAGE1,
			WI_VELDER_SECRET_EXTRA_STAGE2,
			WI_VELDER_SECRET_EXTRA_STAGE3, 
			WI_VELDER_SECRET_EXTRA_STAGE4,
			WI_VELDER_SECRET_EXTRA_STAGE5,
			WI_VELDER_SECRET_EXTRA_BOSS_STAGE,
			//}}

			WI_EVENT_HALLOWEEN_STAGE6,		/// 해외팀 할로윈 이벤트 스테이지 추가
			WI_FISHING_HOLE,				/// 해외팀 이벤트 던전(낚시터 추가) 2012.03.14 임성식
			WI_EVENT_DARKDOOR_ABYSS,		/// 해외팀 이벤트 던전 추가 2012.02.29 임성식
			
			WI_EVENT_TREE_DAY_NEW_STAGE2,
			WI_EVENT_TREE_DAY_NEW_STAGE3,

			WI_TAG_PVPMAP,

			/// 해외팀 미션 던전 스테이지 월드 추가
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE1,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE1B,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE2,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE3,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE4,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE5,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE6,

			WI_VILLAGE_SANDER,				// 샌더마을

			WI_BATTLE_FIELD_RUBEN_FIELD_00,		/// 배틀필드 루벤 00
  
			WI_BATTLE_FIELD_ELDER_FIELD_00,		/// 엘더 00
			WI_BATTLE_FIELD_ELDER_FIELD_REST_00, /// 엘더 휴식처
			WI_BATTLE_FIELD_ELDER_FIELD_01,		/// 엘더 01 
  
			WI_BATTLE_FIELD_BESMA_FIELD_00,		/// 배틀필드 베스마 00
			WI_BATTLE_FIELD_BESMA_FIELD_REST_00, /// 배스마 휴식쳐
			WI_BATTLE_FIELD_BESMA_FIELD_01,		/// 배틀필드 베스마 01
			WI_BATTLE_FIELD_BESMA_FIELD_02,		/// 배틀필드 베스마 02

			WI_BATTLE_FIELD_ALTERA_FIELD_00,			/// 배틀필드 알테라 00
			WI_BATTLE_FIELD_ALTERA_FIELD_01,			/// 배틀필드 알테라 01
			WI_BATTLE_FIELD_ALTERA_FIELD_REST_00,	/// 배틀필드 알테라 쉼터 00
			WI_BATTLE_FIELD_ALTERA_FIELD_02,			/// 배틀필드 알테라 02

			WI_BATTLE_FIELD_PEITA_FIELD_00,			/// 배틀필드 페이타 00 
			WI_BATTLE_FIELD_PEITA_FIELD_01,			/// 배틀필드 페이타 01 
			WI_BATTLE_FIELD_PEITA_FIELD_02,			/// 배틀필드 페이타 02 
			WI_BATTLE_FIELD_PEITA_FIELD_REST_00,	/// 배틀필드 페이타 쉼터 

			WI_BATTLE_FIELD_VELDER_FIELD_00, /// 배틀필드 벨더 00
			WI_BATTLE_FIELD_VELDER_FIELD_01, /// 배틀필드 벨더 01
			WI_BATTLE_FIELD_VELDER_FIELD_02, /// 배틀필드 벨더 02
			WI_BATTLE_FIELD_VELDER_FIELD_03, /// 배틀필드 벨더 03
			WI_BATTLE_FIELD_VELDER_FIELD_REST_00, /// 배틀필드 벨더 04

			WI_BATTLE_FIELD_HAMEL_FIELD_00, /// 배틀필드 벨더 00
			WI_BATTLE_FIELD_HAMEL_FIELD_01, /// 배틀필드 벨더 01
			WI_BATTLE_FIELD_HAMEL_FIELD_02, /// 배틀필드 벨더 02
			WI_BATTLE_FIELD_HAMEL_FIELD_03, /// 배틀필드 벨더 03
			WI_BATTLE_FIELD_HAMEL_FIELD_REST_00, /// 배틀필드 휴식처

			WI_BATTLE_FIELD_TUTORIAL,		/// 배틀필드 튜토리얼

			//메마른 샌더
			WI_SANDER_DRY_SANDER_STAGE0,		
			WI_SANDER_DRY_SANDER_STAGE1,		
			WI_SANDER_DRY_SANDER_STAGE1_1,	
			WI_SANDER_DRY_SANDER_STAGE2,	
			WI_SANDER_DRY_SANDER_B0SS_STAGE,	

			//가르파이 기암지대
			WI_SANDER_GARPAI_ROCK_STAGE0,	
			WI_SANDER_GARPAI_ROCK_STAGE1,	
			WI_SANDER_GARPAI_ROCK_STAGE1_1,	
			WI_SANDER_GARPAI_ROCK_STAGE2,	
			WI_SANDER_GARPAI_ROCK_STAGE3,	
			WI_SANDER_GARPAI_ROCK_BOSS_STAGE,

			WI_BATTLE_FIELD_SANDER_FIELD_00, //샌더 필드 1
			WI_BATTLE_FIELD_SANDER_FIELD_01, //샌더 필드 2		
			WI_BATTLE_FIELD_SANDER_FIELD_02, //샌더 필드 3
			WI_BATTLE_FIELD_SANDER_FIELD_03, //샌더 필드 4
			WI_BATTLE_FIELD_SANDER_FIELD_REST_00,//샌더 휴식처

			//트락 소굴
			WI_SANDER_DEN_OF_TROCK_STAGE0,
			WI_SANDER_DEN_OF_TROCK_STAGE1,
			WI_SANDER_DEN_OF_TROCK_STAGE2_1,	
			WI_SANDER_DEN_OF_TROCK_STAGE2,
			WI_SANDER_DEN_OF_TROCK_STAGE3,
			WI_SANDER_DEN_OF_TROCK_BOSS_STAGE,

			//칼루소 부족 마을
			WI_SANDER_KARUSO_VILLAGE_STAGE0,
			WI_SANDER_KARUSO_VILLAGE_STAGE1,
			WI_SANDER_KARUSO_VILLAGE_STAGE2,
			WI_SANDER_KARUSO_VILLAGE_BOSS_STAGE,

			WI_EVENT_VALENTINE_DAY_STAGE0			= 545,	// 발렌타인 이벤트 던전

			//샌더 PVP 맵
			WI_SANDER_GARPAI_ROCK_PVPMAP				= 546,	
			WI_SANDER_VILLIAGE_PVPMAP					= 547,							WI_RUBEN_WEDDING							= 548,			WI_SANDER_WEDDING							= 549,

			// 샌더 5번 던전, 샌틸러스 호
			WI_SANDER_SANTILUS_SHIP_STAGE0			 	= 550,
			WI_SANDER_SANTILUS_SHIP_STAGE1,
			WI_SANDER_SANTILUS_SHIP_STAGE2,
			WI_SANDER_SANTILUS_SHIP_STAGE3,
			WI_SANDER_SANTILUS_SHIP_BOSS_STAGE,

			// 샌더 6번 던전, 베히모스 심장부

			WI_SANDER_BEHIMOSS_HEART_STAGE0,
			WI_SANDER_BEHIMOSS_HEART_STAGE1,
			WI_SANDER_BEHIMOSS_HEART_STAGE2,
			WI_SANDER_BEHIMOSS_HEART_STAGE3,
			WI_SANDER_BEHIMOSS_HEART_BOSS_STAGE,

			//해외팀 나이트와쳐 이벤트 던젼 2013.07.03 김창한
			WI_EVENT_NIGHT_WATCHER_STAGE1 = 560,

			//하멜 비던
			WI_HAMEL_SECRET_STAGE0					 	= 561,			WI_HAMEL_SECRET_STAGE1					 	= 562,			WI_HAMEL_SECRET_STAGE2					 	= 563,			WI_HAMEL_SECRET_STAGE3					 	= 564,			WI_HAMEL_SECRET_BOSS_STAGE					= 565,			WI_HAMEL_SECRET_EXTRA_STAGE1				= 566,			WI_HAMEL_SECRET_EXTRA_STAGE2				= 567,			WI_HAMEL_SECRET_EXTRA_STAGE3				= 568,			WI_HAMEL_SECRET_EXTRA_BOSS_STAGE0			= 569,

			WI_FIELD_BOSSRAID_KINGDOM_OF_NASOD			= 570,	// 나소드 왕국
			WI_FIELD_BOSSRAID_VALLEY_OF_ABYSS			= 571,	// 심연의 계곡
			WI_END,
		};

		struct WorldData
		{
			WORLD_ID	worldID;
			wstring		worldName;
			wstring		dataFileName;
		};

		enum SQUARE_CAMERA_TYPE
		{
			SCT_DEFAULT,
			SCT_PRESET,			// 월드별로 개별 설정된 lookat, eye height등을 사용
			SCT_LINE_DEPENDENT,	// CKTDGLineMap::CameraData를 사용하는 
		};

		struct SquareCameraData
		{
			SQUARE_CAMERA_TYPE	m_eSquareCameraType;
			
			float 				m_fCameraDist;
			float 				m_fLookAtHeight;
			float 				m_fEyeHeight;

			SquareCameraData()
			{
				m_eSquareCameraType = SCT_DEFAULT;
				m_fCameraDist		= 0.f;
				m_fLookAtHeight		= 0.f;
				m_fEyeHeight		= 0.f;
			}
		};


#ifdef MARIO_LIKE_BLOCK_TEST

		struct WorldMonsterData
		{
			int m_iTeamNumber;
			int m_iNPCID;
			int m_iLevel;
			D3DXVECTOR3 m_vPosition;

			WorldMonsterData()
				: m_iTeamNumber( 0 )
				, m_iNPCID( 0 )
				, m_iLevel( 0 )
				, m_vPosition( 0, 0, 0 )
			{
			}
		};

#endif MARIO_LIKE_BLOCK_TEST

	public:
		CX2World( WorldData* pWorldData, const bool bBackgroundLoad_ = false );
		virtual ~CX2World(void);

		virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT		OnFrameRender();
		virtual HRESULT		OnResetDevice();
		virtual HRESULT		OnLostDevice();

		void				SetMapDetail( CX2GameOption::OptionLevel optionLevel );

		void				PlayBGM();
		void				StopBGM();

		bool GetRenderLineMap() const { return m_bRenderLineMap; }
		void SetRenderLineMap(bool val) { m_bRenderLineMap = val; }

		void				SetWorldColor( D3DXCOLOR color )
		{
			m_NowColor		= color;
			m_FadeColor		= color;
			m_fFadeTime		= 0.0f;
		}

		void				FadeOriginWorldColor( D3DXCOLOR color, float fTime )
		{
			m_FadeOriginColor = color;	
			m_fFadeOriginTime = fTime;	
		}

		void				FadeWorldColor( D3DXCOLOR color, float fTime )
		{
			m_FadeColor		= color;
			m_fFadeTime		= fTime;
		}

		void				FadeLightColor( D3DXCOLOR color, float fTime )
		{
			m_FadeLightColor	= color;
			m_fFadeLightTime	= fTime;
		}

		void				FadeWorldLayerColor( D3DXCOLOR color, float fTime )
		{
			for( UINT i=0; i<m_LayerList.size(); i++ )
			{
				CX2WorldLayer* pWorldLayer = m_LayerList[i];
				if( NULL == pWorldLayer )
					continue;

				pWorldLayer->FadeColor( color, fTime );
			}
		}


		



		bool				RayCheck( D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd );
		
		void				SetShowObject( bool bShow );
#ifdef X2TOOL
		void				SetShowObjectByTool( bool bShow );
#endif
		CKTDGLineMap*		GetLineMap(){ return m_pLineMap; }
		CKTDGLineMap*		GetLineMapFuture() { return m_pLineMapFuture; }

		D3DXVECTOR3			GetLightPos(){ return m_LightPos; }
		D3DXCOLOR			GetLightColor(){ return m_LightColor; }

		

        const WorldData* GetWorldData() const { return m_pWorldData; }

		//{{ seojt // 2008-10-24, 11:02
		//vector<CX2WorldObject*>& GetWorldObjectList() { return m_ObjectList; }
        // GetWorldObjectList()를 호출한 쪽에서 container를 변경하지 않는 다는 것을
        // 보장하기 위해, const&를 리턴하도록 수정 - jintaeks on 2008-10-24, 11:03
		const vector<CX2WorldObjectPtr>& GetWorldObjectList() { return m_ObjectList; }
		//}} seojt // 2008-10-24, 11:02

#if defined(WORLD_TOOL) || defined(X2TOOL)
		//{{ 2009.02.25 김태완 : GetWorldObjectList에서 받은 vector을 변경시킬 수 없는 관계로
		//World에서 특정 오브젝트를 삭제하는 기능 추가 : 월드툴 용
		void RemoveWorldObject(CX2WorldObject* pTargetWorldObject);
		//}}
		vector<CX2WorldObjectPtr> GetWorldObjectList4Tool() { return m_ObjectList; }
		void AddWorldObject4Undo(CX2WorldObjectPtr pTargetWorldObjectPtr);
		void DeleteWorldObject4Undo(CX2WorldObject* pTargetWorldObject);
#endif

//{{ seojt // 2008-10-24, 11:11
        /// 객체 이름으로 m_ObjectList[]를 정렬한다.
        void SortOnetime();

        /// 이름으로 CX2WorldObject*를 찾는다.
        /// @param  wstrName_ : CX2WorldObject:m_Name에 해당하는 이름을 명시한다.
        /// @return CX2WorldObject* : 이름에 해당하는 객체가 없는 경우, NULL을 리턴한다.
        /// @note   이 함수의 시간 복잡도는 O(n)이다. 조심스럽게 사용하자.
        /// @date   jintaeks on 2008-10-21, 17:26
        CX2WorldObject*     FindObjectByName( const std::wstring& wstrName_ );
//}} seojt // 2008-10-24, 11:11

		vector<CKTDGSkyDome*>& GetSkyDomeList() { return m_SkyDomeList; }
#ifdef WORLD_LAYER
        vector<CX2WorldLayer*>& GetWorldLayerList() { return m_LayerList; }
#ifdef X2TOOL
		void SwapWorldLayerId(int iOld, int iNew)
		{
			CX2WorldLayer *pTemp = m_LayerList[iOld];
			m_LayerList[iOld] = m_LayerList[iNew];
			m_LayerList[iNew] = pTemp;
		}
#endif
#endif
		bool				OpenScriptFile( const WCHAR* pFileName, KLuabinder* pKLuabinder = NULL );

		D3DXCOLOR			GetOriginColor(){ return m_OriginColor; }

		//LUA
		void				SetOriginColor_LUA( float fRed, float fGreen, float fBlue );	// 월드 전체 색상
		void				SetLightColor_LUA( float fRed, float fGreen, float fBlue );		// 캐릭터 색상
		void				SetLightPos( float x, float y, float z );
		bool				SetLineMap_LUA( const char* pFileName );
		
#ifdef X2TOOL
		bool				GetLineMapByTool( wstring wstrFileName );
#endif

		void				SetBGM_LUA( const char* pFileName );
		void				SetBGMName( const wstring& BGMName_){ m_BGMName = BGMName_; } 
		wstring				GetBGMName() const {return m_BGMName;}
		void				AddEffectBGM_LUA( const char* pFileName );
		void				Add3DEffectBGM_LUA( const char* pFileName, float x, float y, float z, float fMax3DDistance );
		void				StopAllEffectBGM();
		void				PlayAllEffectBGM();
		

		// oasis907 : 김상윤 [2010.12.13] // 본섭 파싱 위해 디파인 싸지 않음
		bool				SetWorldCamera_LUA( const char* pFileName );
#ifdef KEYFRAME_CAMERA
		void				UnLoadWorldCamera();
#endif KEYFRAME_CAMERA

		void				SetSquareCameraSetting( float fCameraDist, float fLookAtHeight, float fEyeHeight )
		{
			m_SquareCameraData.m_fCameraDist		= fCameraDist;
			m_SquareCameraData.m_fLookAtHeight		= fLookAtHeight;
			m_SquareCameraData.m_fEyeHeight			= fEyeHeight;
		}

		void				SetSquareCameraType( int iType )
		{
			m_SquareCameraData.m_eSquareCameraType = (SQUARE_CAMERA_TYPE) iType;
		}

		CX2WorldObjectParticle*		CreateObjectParticle( CKTDGParticleSystem* pParticleSystem, const char* pszSequenceName );
		CX2WorldObjectParticle*		CreateObjectParticleDelay( CKTDGParticleSystem* pParticleSystem, const char* pszSequenceName, float fDelay );
		
		CX2WorldObjectLightFlow*	CreateObjectLightFlow();
		CX2WorldObjectSkinMesh*		CreateObjectSkinMesh();
		CX2WorldObjectMesh*         CreateObjectMesh();
		CKTDGSkyDome*				CreateSkyDome();
		CKTDGLensFlare*				CreateLensFlare();
#ifdef WORLD_LAYER
        CX2WorldLayer*              CreateWorldLayer();
        void                        RemoveWordLayer(int index);
#endif
	
        void                        ClearObjectMesh();

		void						SetLOD( int lodPercent );
		int							GetLOD() { return m_LodPercent; }

#ifdef FOG_WORLD
        void                        SetFogWorld(float fNearX, float fFarX, float fNearY, float fFarY, float fDensity, D3DXCOLOR fogColor);
        void                        ToggleFog(bool flag = false);
                    
    public:
        bool                        m_bFog;    
        bool                        m_bFogShow;
        float                       m_fNearX;
        float                       m_fFarX;
        float                       m_fNearY;
        float                       m_fFarY;
        float                       m_fDensity;
        D3DXCOLOR                   m_FogColor;
#endif


#ifdef BUBBLE_BOBBLE_TEST
		D3DXVECTOR3 GetLookAt() const { return m_vLookAt; }
		void SetLookAt(D3DXVECTOR3 val) { m_vLookAt = val; }
		D3DXVECTOR3 GetEye() const { return m_vEye; }
		void SetEye(D3DXVECTOR3 val) { m_vEye = val; }
#endif BUBBLE_BOBBLE_TEST

        bool                        GetDistanceColor() { return m_bDistanceColor; }
        void                        SetDistanceColor(bool flag) { m_bDistanceColor = flag; }
        void                        AddSyncObject(CX2WorldObject *object) { m_vecSyncObject.push_back(object); }
        void                        FrameMoveSyncObject(float fx, float fy);
        CX2WorldObject*             GetSyncObject(int index);
		
		void PlayEffectBGM( bool bPlay );
		void Play3DEffectBGM( bool bPlay );
		void SetEffectBGMVolume( float fSoundVolume );


		const CX2World::SquareCameraData& GetSquareCameraData() const { return m_SquareCameraData; }




#ifdef DEEP_WATER_DUNGEON_TEST
		float GetGravityAndSpeedRate() const { return m_fGravityAndSpeedRate; }
		void SetGravityAndSpeedRate(float val) { m_fGravityAndSpeedRate = val; }

		float GetUnitAnimSpeedRate() const { return m_fUnitAnimSpeedRate; }
		void SetUnitAnimSpeedRate(float val) { m_fUnitAnimSpeedRate = val; }
#endif DEEP_WATER_DUNGEON_TEST




		void AddWorldMonster_LUA();
#ifdef MARIO_LIKE_BLOCK_TEST
		std::vector< WorldMonsterData >& GetVecWorldMonsterData() { return m_vecWorldMonsterData; }
#endif MARIO_LIKE_BLOCK_TEST



#ifdef WORLD_TRIGGER
		bool CanActiveTrigger(int iVal)
		{
			if( GetLineMap() == NULL || iVal < 0 )
				return false;

			if( iVal >= (int)GetLineMap()->GetLineMapTrigger()->m_vecTriggerSeg.size() )
				return false;

			CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = GetLineMap()->GetLineMapTrigger()->m_vecTriggerSeg[iVal];
			if( triggerSeg.m_iTriggerDataIndex < 0 )
				return false;

			for(UINT i=0; i<GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.size(); ++i)
			{
				int iNowIndex = GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger[i];
				if( iNowIndex == iVal )
					return false;		
			}

			return true;
		}
		void ActiveTrigger(int iVal)
		{
			if( CanActiveTrigger(iVal) == true )
				GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.push_back(iVal);
		}
		void InactiveTrigger(int iVal)
		{
			if( GetLineMap() == NULL )
				return;

			for(UINT i=0; i<GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.size(); ++i)
			{
				if( GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger[i] == iVal )
				{
					GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.erase(GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.begin()+i);
					return;
				}

			}
		}
		void InitTriggerDone(CKTDGLineMap::TRIGGER_DATA &triggerData, bool bDone = false);
		CX2WorldObjectSkinMesh *GetTriggerMesh(const wstring &wstrMeshName);

		void DoTriggerMesh(CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh);
		void DoTriggerLine(CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine);
		void DoTriggerEtc(CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc);
		void DoTriggerFrameMove( double fTime, float fElapsedTime );

		void SetTriggerCamera(int iVal, float fVal) { m_iTriggerCameraIndex = iVal; m_fDurationTriggerCamera = fVal; }
		int	GetTriggerCamera()
		{
			if( m_fDurationTriggerCamera > 0.f && m_iTriggerCameraIndex >= 0 )
			{
				return m_iTriggerCameraIndex;
			}

			return -1;
		}
#endif

#ifdef MODIFY_FRUSTUM
		void SetProjection( float fNear, float fFar, float fPerspective, float fBaseDistance, float fStepDist1, float fStepDist2, float fStepDist3 );
		float GetBaseCameraDistance() { return m_fBaseCameraDistance; }
		float GetCameraStepDistacne(int iStep) { return m_fCameraDistanceStep[iStep]; }
		void SetCameraDistance( float fBaseDist, float fStepDist1, float fStepDist2, float fStepDist3 )
		{
			m_fBaseCameraDistance = fBaseDist;
			m_fCameraDistanceStep[0] = fStepDist1;
			m_fCameraDistanceStep[1] = fStepDist2;
			m_fCameraDistanceStep[2] = fStepDist3;
		}
#endif

#if defined(WORLD_TOOL) || defined(X2TOOL)
		wstring GetBGMName() { return m_BGMName; }
		wstring GetWorldName() { return m_wstrWorldName; }
		void RemoveSkyDome(int index);
		wstring GetWorldCamera() { return m_wstrWorldCamera; }
#endif

    //{{ seojt // 2008-10-24, 14:06
    public:
        void Debug_RenderHelp( int ix_, int iy_ );
    //}} seojt // 2008-10-24, 14:06




	private:
		WorldData*					m_pWorldData; //기본 월드 데이타

		bool						m_bBackgroundLoad;
		CKTDGLineMap*				m_pLineMap;
		CKTDGLineMap*				m_pLineMapFuture;			
		bool						m_bFirstLineMapFrameMove;


		wstring						m_BGMName;

		D3DXVECTOR3					m_LightPos;
		D3DXCOLOR					m_LightColor;			// 캐릭터 색상에 적용되는 color
		D3DXCOLOR					m_FadeLightColor;		
		float						m_fFadeLightTime;


		D3DXCOLOR					m_OriginColor;			// 영구적인 배경 object color
		D3DXCOLOR					m_FadeOriginColor;		
		float						m_fFadeOriginTime;		


		D3DXCOLOR					m_NowColor;				// 배경 object에 적용되는 color, 캐릭터들이 필살기를 사용하거나 상태가 바뀔 때 매번 이 색상이 m_OriginColor로 리셋된다. 즉, instance color
		D3DXCOLOR					m_FadeColor;			
		float						m_fFadeTime;

		


		

		vector<CKTDGSkyDome*>		m_SkyDomeList;
		vector<CX2WorldObjectPtr>   m_ObjectList;
        bool                        m_bObjectListSorted;
		vector<CKTDGLensFlare*>		m_pLensFlareList;
#ifdef WORLD_LAYER
        vector<CX2WorldLayer*>      m_LayerList;
#endif
		vector<CKTDXDeviceSound*>	m_vecEffectBGM;
		vector<CKTDXDeviceSound*>	m_vec3DEffectBGM;


		bool						m_bRenderLineMap;

		int							m_LodPercent;


#ifdef BUBBLE_BOBBLE_TEST
		D3DXVECTOR3					m_vLookAt;	// when it's fixed camera mode
		D3DXVECTOR3					m_vEye;
#endif BUBBLE_BOBBLE_TEST

        bool                        m_bDistanceColor;
        vector<CX2WorldObject*>     m_vecSyncObject;

		KLuabinder*					m_pKLuabinder;



		SquareCameraData			m_SquareCameraData;


#ifdef DEEP_WATER_DUNGEON_TEST
		float						m_fGravityAndSpeedRate; // 중력 및 캐릭터 움직이는 속도에 곱해지는 수, 물속 던전 만들기 위해서 처음 추가
		float						m_fUnitAnimSpeedRate;		// 캐릭터 애니메이션 속도에 곱해지는 수
#endif DEEP_WATER_DUNGEON_TEST


#ifdef MARIO_LIKE_BLOCK_TEST

		std::vector< WorldMonsterData > m_vecWorldMonsterData;		

#endif MARIO_LIKE_BLOCK_TEST


#ifdef WORLD_TRIGGER
		int								m_iTriggerCameraIndex;
		float							m_fDurationTriggerCamera;
#endif

#ifdef MODIFY_FRUSTUM
		float m_fBaseCameraDistance;
		float m_fCameraDistanceStep[3];
#endif

#if defined(WORLD_TOOL) || defined(X2TOOL)
		wstring m_wstrWorldName;
		wstring m_wstrWorldCamera;
#endif
};//class CX2World

