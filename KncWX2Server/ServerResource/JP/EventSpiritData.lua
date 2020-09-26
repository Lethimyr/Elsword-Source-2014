-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

-- iEventID , iReduceSpirit , iRewardSpirit , iRewardID

GameEvent:AddEventSpiritInfo
{
	iEventID = 1,
	iReduceSpirit = 80,			-- 감소량 %
	iRewardSpirit = 90,			-- 보상 받을 %
	iRewardID = 765,
	iUnitLevel = 15,			-- 유닛 레벨
	cUnitType = UNIT_TYPE["UT_LIRE"],		-- 레나
	cUnitClassType = UNIT_CLASS["UC_NONE"],
	APPLY_DUNGEON_TABLE = -- 감소 및 보상 적용 던전
	{
	},
	
	NO_APPLY_DUNGEON_TABLE =  -- 감소 및 보상 적용 안되는 던전
	{
		DUNGEON_ID["DI_RUBEN_EL_TREE_NORMAL"],
		DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_NORMAL"],
		DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_HARD"],
		DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_EXPERT"],
		DUNGEON_ID["DI_RUBEN_SWAMP_NORMAL"],
		DUNGEON_ID["DI_RUBEN_SWAMP_HARD"],
		DUNGEON_ID["DI_RUBEN_SWAMP_EXPERT"],
		
		DUNGEON_ID["DI_ELDER_HENIR_SPACE"],
		DUNGEON_ID["DI_BESMA_HENIR_SPACE"],
		DUNGEON_ID["DI_ALTERA_HENIR_SPACE"],
		DUNGEON_ID["DI_FEITA_HENIR_SPACE"],
		DUNGEON_ID["DI_VELDER_HENIR_SPACE"],
		DUNGEON_ID["DI_HAMEL_HENIR_SPACE"],
	},	
}

-- 2012.11.28 고속 레벨업 ( 불타는 열정 1개 )
GameEvent:AddEventSpiritInfo
{
	iEventID = 2,
	iReduceSpirit = 0,
	iRewardSpirit = 50,
	iRewardID = 772,
}

-- 2012.12.18 근성도 무제한
GameEvent:AddEventSpiritInfo
{
	iEventID = 3,
	iReduceSpirit = 100,
	iRewardSpirit = 0,
	iRewardID = 0,
}