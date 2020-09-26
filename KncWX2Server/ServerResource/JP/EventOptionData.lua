-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.
--[[
30071		다운 상태의 적 공격시 데미지 2.5배로 증가	
35059		물리 공격력 +10%
35060		마법 공격력 +10%
35061		물리 공격력 +20%
35062		마법 공격력 +20%
35063		물리 공격력 +30%
35064		마법 공격력 +30%
35065		이동 속도 +10%
35066		점프 속도 +10%
35067		이동 속도 +20%
35068		점프 속도 +20%
35069		이동 속도 +30%
35070		점프 속도 +30%	
40050		동작속도 25 %
40150		크리티컬 25 %
40250		추가데미지 25 %
40550		회피 25 %
40650		데미지 감소 25 %
60213		스폐셜 액티브 스킬 데미지 25% 증가
60313		물리 공격력 25% 증가
60613		마법 공격력 25% 증가
]]--
GameEvent:AddEventOptionInfo
{
	iEventID = 1,
	iUnitLevel = 15,
	bLevelParty = True,		-- True면 전원이 제한레벨 이상이어야 함  , False면 한명이라도 제한레벨 이상이면 됨
	bPartyUserCntOver = True,	-- True면 요구 파티 인원 수 이상 가능 , False 면 딱 요구 파티 인원 수만 가능
	iMinRequirePartyUserCnt = 1,
	cUnitType = UNIT_TYPE["UT_LIRE"],
	cUnitClassType = UNIT_CLASS["UC_NONE"],

	iRandomSocketNumber = 2,		-- 0 이면 랜던 소켓 발동 안함 , 0 이상일때 스테이지 마다 설정 소켓 에서 랜덤으로 개수 만큼 발동
	
    SOCKET_OPTION =
    {
		35059,
		35060,
		35065,
		35066,
    },

	APPLY_DUNGEON_TABLE = -- 버프 적용 던전
	{
	},
	
	NO_APPLY_DUNGEON_TABLE =  -- 버프 적용 안되는 던전
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

GameEvent:AddEventOptionInfo
{
	iEventID = 2,
	iMinRequirePartyUserCnt = 3,
	
    SOCKET_OPTION =
    {
		35061,
		35062,
		35067,
		35068,
    },
}

GameEvent:AddEventOptionInfo
{
	iEventID = 3,
	iMinRequirePartyUserCnt = 4,
	
    SOCKET_OPTION =
    {
		35063,
		35064,
		35069,
		35070,
    },
}

GameEvent:AddEventOptionInfo
{
	iEventID = 4,

    SOCKET_OPTION =
    {
		35090
    },
}

GameEvent:AddEventOptionInfo
{
	iEventID = 5,
	--iMinRequirePartyUserCnt = 3,
	iUnitLevel = 1,
	bLevelParty = False,
	bPartyUserCntOver = True,	-- True면 요구 파티 인원 수 이상 가능 , False 면 딱 요구 파티 인원 수만 가능
	iMinRequirePartyUserCnt = 1,	
	--cUnitType = UNIT_TYPE["UT_RAVEN"],
	--cUnitClassType = UNIT_CLASS["UC_NONE"],
	
	iRandomSocketNumber = 1,		-- 0 이면 랜던 소켓 발동 안함 , 0 이상일때 스테이지 마다 설정 소켓 에서 랜덤으로 개수 만큼 발동
	bStageChange = False,
	
    SOCKET_OPTION =
    {
		--60913,
		60213,
		30071,
		60313,
		60613,
		40050,
		40150,
		40250,
		40650,
		40550,
    },
	
	APPLY_DUNGEON_TABLE = -- 버프 적용 던전
	{
	},
	
	NO_APPLY_DUNGEON_TABLE =  -- 버프 적용 안되는 던전
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

-- 13.03.13 복귀칭호 이벤트
GameEvent:AddEventOptionInfo
{
	iEventID = 6,
	--iMinRequirePartyUserCnt = 3,
	iUnitLevel = 1,
	bLevelParty = False,
	bPartyUserCntOver = True,	-- True면 요구 파티 인원 수 이상 가능 , False 면 딱 요구 파티 인원 수만 가능
	iMinRequirePartyUserCnt = 1,	
	--cUnitType = UNIT_TYPE["UT_RAVEN"],
	--cUnitClassType = UNIT_CLASS["UC_NONE"],
	
	iRandomSocketNumber = 0,		-- 0 이면 랜던 소켓 발동 안함 , 0 이상일때 스테이지 마다 설정 소켓 에서 랜덤으로 개수 만큼 발동
	bStageChange = False,
	
	iRequireTitleID = 5610, -- #define SERV_OPTION_EVENT_SYSTEM_REQUIRE_TITLE
	
	
    SOCKET_OPTION =
    {
		40110, -- 크리5%
		35059, -- 물리 공격력 +10%
		35060, -- 마법 공격력 +10%
		30071, -- 쓰러지면 2.5배
    },
	
	APPLY_DUNGEON_TABLE = -- 버프 적용 던전
	{
	},
	
	NO_APPLY_DUNGEON_TABLE =  -- 버프 적용 안되는 던전
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

-- 13.03.20 춘분이벤트
GameEvent:AddEventOptionInfo
{
	iEventID = 7,
	--iMinRequirePartyUserCnt = 3,
	iUnitLevel = 1,
	bLevelParty = False,
	bPartyUserCntOver = True,	-- True면 요구 파티 인원 수 이상 가능 , False 면 딱 요구 파티 인원 수만 가능
	iMinRequirePartyUserCnt = 1,	
	--cUnitType = UNIT_TYPE["UT_RAVEN"],
	--cUnitClassType = UNIT_CLASS["UC_NONE"],
	
	iRandomSocketNumber = 0,		-- 0 이면 랜던 소켓 발동 안함 , 0 이상일때 스테이지 마다 설정 소켓 에서 랜덤으로 개수 만큼 발동
	bStageChange = False,
	
    SOCKET_OPTION =
    {
		35067,--이동 속도 +20%
		35068,--점프 속도 +20%
		40050,--동작속도 25 %
		60213,--스폐셜 액티브 스킬 데미지 25% 증가
    },
	
	APPLY_DUNGEON_TABLE = -- 버프 적용 던전
	{
	},
	
	NO_APPLY_DUNGEON_TABLE =  -- 버프 적용 안되는 던전
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
