-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

--[[
-- 샘플 스크립트 작성
GameEvent:AddEventStageInfo
{
	iEventID = 1,
	eType = STAGE_EVENT_TYPE["SET_SUMMON_SPIRIT"],	-- 스테이지 이벤트 타입
	
	SUMMON_SPIRIT_TABLE = -- 소환 정령 세부 셋팅
	{
		{
			iSpiritID = 1;
			fRate = 100;
		},
	},
	iCount = 1,		-- 한 던전에서 최대 발생 할 수 있는 횟수
}
--]]

GameEvent:AddEventStageInfo
{
	iEventID = 1,
	eType = STAGE_EVENT_TYPE["SET_SUMMON_SPIRIT"],	-- 스테이지 이벤트 타입
	
	SUMMON_SPIRIT_TABLE = -- 소환 정령 세부 셋팅
	{
		{
			iSpiritID = 100;
			fRate = 5;
		},
	},
	iCount = 1,		-- 한 던전에서 최대 발생 할 수 있는 횟수
}