-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

--## 각 이벤트 마다 아이디가 겹치지 않아야 하지만 다른 이벤트 끼리는 상광없음. ##

--[[REWARD]]
-- 1 arg : reward id
-- 2 arg : EXE rate
-- 3 arg : VP rate
-- 4 arg : Ed rate

-- 5 arg : Dungeon ID(난이도는 식별하지 않는다.)
-- 6 arg : Unit Level
-- 7 arg : Unit Class
-- 8 arg : dungeon game party unit num
-- # 5, 6, 7, 8 arg 는 사용하지 않는 항목은 0으로 셋팅해야함.
-- # 5,6,7 항목과 8 항목은 전혀 별개로 사용해야함.

CRITERION_USER_TYPE =
{
	CUT_NONE		= 0,
	CUT_EXIST		= 1,
	CUT_COMEBACK	= 2,
	CUT_NEW			= 4,
	CUT_JUMPING		= 8,
	CUT_NOT_JUMPING	= 7,
	CUT_ALL			= 15,
}

--경험치 2배
GameEvent:AddReward( 1, { fEXPRate = 1.0 } )
--vp 2배
GameEvent:AddReward( 2, { fVPRate = 1.0 } )
--ed 2배
GameEvent:AddReward( 3, { fEDRate = 1.0 } )

--초심자의 숲 : 경험치 2배
GameEvent:AddReward( 4, { fEXPRate = 1.0, iDungeonID = 30040 } )
--엘의 나무 서쪽 : 경험치 2배
GameEvent:AddReward( 5, { fEXPRate = 1.0, iDungeonID = 30010 } )
--엘의 나무 : 경험치 2배
GameEvent:AddReward( 6, { fEXPRate = 1.0, iDungeonID = 30000 } )
--엘의 나무 동쪽 : 경험치 2배
GameEvent:AddReward( 7, { fEXPRate = 1.0, iDungeonID = 30090 } )
--엘의 나무 북쪽 : 경험치 2배
GameEvent:AddReward( 8, { fEXPRate = 1.0, iDungeonID = 30020 } )
--원숭이 숲 : 경험치 2배
GameEvent:AddReward( 9, { fEXPRate = 1.0, iDungeonID = 30080 } )
--어둠의 숲 : 경험치 2배
GameEvent:AddReward( 10, { fEXPRate = 1.0, iDungeonID = 30070 } )

--파티경험치 이벤트
GameEvent:AddReward( 11, { fEXPRate = 1.0, iPartyNum = 2 } )

--경험치, vp, ed 150%
GameEvent:AddReward( 12, { fEXPRate = 0.5, fVPRate = 0.5, fEDRate = 0.5 } )

--경험치, vp, ed 200%
GameEvent:AddReward( 13, { fEXPRate = 1.0, fVPRate = 1.0, fEDRate = 1.0 } )


-- 매일매일 이벤트
GameEvent:AddReward( 14, { fEventTime = 120, iRewardID = 14 } ) -- 월요일
GameEvent:AddReward( 15, { fEventTime = 120, iRewardID = 15 } ) -- 화요일
GameEvent:AddReward( 16, { fEventTime = 120, iRewardID = 16 } ) -- 수요일
GameEvent:AddReward( 17, { fEventTime = 120, iRewardID = 17 } ) -- 목요일 6월 26일
GameEvent:AddReward( 18, { fEventTime = 120, iRewardID = 18 } ) -- 목요일 7월  3일
GameEvent:AddReward( 19, { fEventTime = 120, iRewardID = 19 } ) -- 목요일 7월 10일
GameEvent:AddReward( 20, { fEventTime = 120, iRewardID = 20 } ) -- 금요일
GameEvent:AddReward( 21, { fEventTime = 120, iRewardID = 21 } ) -- 토요일
GameEvent:AddReward( 22, { fEventTime = 120, iRewardID = 22 } ) -- 일요일

-- ACT3 이벤트
GameEvent:AddReward( 23, { fEventTime =   0, iRewardID = 30 } )  -- ACT3 이벤트	출입증서 랜덤 큐브
GameEvent:AddReward( 24, { fEventTime =   0, iRewardID = 31 } )  -- ACT3 이벤트	무기 랜덤 큐브
GameEvent:AddReward( 25, { fEventTime =   0, iRewardID = 32 } )  -- ACT3 이벤트	회복 랜덤 큐브
GameEvent:AddReward( 26, { fEventTime =   0, iRewardID = 33 } )  -- ACT3 이벤트	마법석 랜덤 큐브
GameEvent:AddReward( 27, { fEventTime =   0, iRewardID = 34 } )  -- ACT3 이벤트	강화석 랜덤 큐브
GameEvent:AddReward( 28, { fEventTime =   0, iRewardID = 35 } )  -- ACT3 이벤트	ACT3 글자 랜덤 큐브
GameEvent:AddReward( 29, { fEventTime =   0, iRewardID = 20 } )  -- ACT3 이벤트	ACT3 경험치 15% 메달
GameEvent:AddReward( 30, { fEventTime = 120, iRewardID = 36 } )  -- ACT3 이벤트	ACT3 글자 A
GameEvent:AddReward( 31, { fEventTime = 180, iRewardID = 37 } )  -- ACT3 이벤트	ACT3 글자 C
GameEvent:AddReward( 32, { fEventTime = 120, iRewardID = 38 } )  -- ACT3 이벤트	ACT3 글자 T


--경험치 1.5배
GameEvent:AddReward( 33, { fEXPRate = 0.5 } )
--vp 1.5배
GameEvent:AddReward( 34, { fVPRate = 0.5 } )
--ed 1.5배
GameEvent:AddReward( 35, { fEDRate = 0.5 } )


-- 올림픽 이벤트
GameEvent:AddReward( 36, { fEventTime =   0, iRewardID = 42, bGiveOnce = False, bDelEndEvent = True  } )	-- 금메달 큐브
GameEvent:AddReward( 37, { fEventTime = 120, iRewardID = 43, bGiveOnce = False } )							-- 대표팀 응원 아이템


-- 신학기 기념 이벤트
GameEvent:AddReward( 38, { fEventTime =   0, iRewardID = 44 } )  -- 신학기 기념 출입증 큐브


--경험치 3배
GameEvent:AddReward( 39, { fEXPRate = 2.0 } )
--vp 3배
GameEvent:AddReward( 40, { fVPRate = 2.0 } )
--ed 3배
GameEvent:AddReward( 41, { fEDRate = 2.0 } )


-- 추석 이벤트
GameEvent:AddReward( 42, { fEventTime =   0, iRewardID = 45 } )  -- 추석 이벤트	부활석 큐브
GameEvent:AddReward( 43, { fEventTime =   0, iRewardID = 46 } )  -- 추석 이벤트	출입 랜덤 큐브
GameEvent:AddReward( 44, { fEventTime =   0, iRewardID = 47 } )  -- 추석 이벤트	무기 강화석
GameEvent:AddReward( 45, { fEventTime =   0, iRewardID = 48 } )  -- 추석 이벤트	마법석
GameEvent:AddReward( 46, { fEventTime = 120, iRewardID = 49, bGiveOnce = False } )  -- 추석 이벤트	한가위 종합 선물 세트


-- ACT4 이벤트
GameEvent:AddReward( 47, { fEXPRate = 2.0, iPartyNum = 2 } )

GameEvent:AddReward( 48, { fEventTime =	 0, iRewardID = 50 } )  -- ACT4 이벤트 방어구 강화석
GameEvent:AddReward( 49, { fEventTime =	 0, iRewardID = 51 } )  -- ACT4 이벤트 요리재료세트 묶음
GameEvent:AddReward( 50, { fEventTime =	 0, iRewardID = 52 } )  -- ACT4 이벤트 무기 큐브
GameEvent:AddReward( 51, { fEventTime =	 0, iRewardID = 53 } )  -- ACT4 이벤트 장비제조재료세트 묶음
GameEvent:AddReward( 52, { fEventTime =	 0, iRewardID = 54 } )  -- ACT4 이벤트 무기 강화석
GameEvent:AddReward( 53, { fEventTime =	 0, iRewardID = 55 } )  -- ACT4 이벤트 고급 마법석
GameEvent:AddReward( 54, { fEventTime =	 0, iRewardID = 56 } )  -- ACT4 이벤트 출입 랜덤 큐브
GameEvent:AddReward( 55, { fEventTime =	 0, iRewardID = 57 } )  -- ACT4 이벤트 부활석 큐브
GameEvent:AddReward( 56, { fEventTime =	 0, iRewardID = 58 } )  -- ACT4 이벤트 마법석
GameEvent:AddReward( 57, { fEventTime =	 0, iRewardID = 59 } )  -- ACT4 이벤트 포션 세트


-- 300일 이벤트
GameEvent:AddReward( 58, { fEventTime =	 0, iRewardID = 60 } )  -- 300일 이벤트 종합 선물 세트


-- ACT4 이벤트
GameEvent:AddReward( 59, { fEventTime =	 0, iRewardID = 61 } )  -- ACT4 이벤트 엘의 조각 (불명)
GameEvent:AddReward( 60, { fEventTime =	 0, iRewardID = 62 } )  -- ACT4 이벤트 엘의 조각 (레드)
GameEvent:AddReward( 61, { fEventTime =	 0, iRewardID = 63 } )  -- ACT4 이벤트 엘의 조각 (블루)
GameEvent:AddReward( 62, { fEventTime =	 0, iRewardID = 64 } )  -- ACT4 이벤트 엘의 조각 (그린)
GameEvent:AddReward( 63, { fEventTime =	 0, iRewardID = 65 } )  -- ACT4 이벤트 엘의 조각 (윈드)
GameEvent:AddReward( 64, { fEventTime =	 0, iRewardID = 66 } )  -- ACT4 이벤트 엘의 조각 (라이트)
GameEvent:AddReward( 65, { fEventTime =	 0, iRewardID = 67 } )  -- ACT4 이벤트 엘의 조각 (다크)
GameEvent:AddReward( 66, { fEventTime =	 0, iRewardID = 68 } )  -- ACT4 이벤트 엘의 조각 (불명 1개)


-- 할로윈 이벤트
GameEvent:AddReward( 67, { fEventTime = 150, iRewardID = 69 } )  -- 할로윈 선물 세트
GameEvent:AddReward( 68, { fEventTime =	  0, iRewardID = 70, bGiveOnce = False } )  -- 할로윈 던전 입장권
	
	
-- 가을 이벤트
GameEvent:AddReward( 69, { fEventTime =   0, iRewardID = 71 } )  -- 가을 선물 세트


-- 개편 기념 이벤트
GameEvent:AddReward( 70, { fEventTime =   0, iRewardID = 72 } )  -- 개편 기념 선물 세트
GameEvent:AddReward( 71, { fEventTime = 150, iRewardID = 72 } )  -- 개편 기념 선물 세트


-- 20레벨 이하는 경험치 150% 보너스!
GameEvent:AddReward( 72, { fEXPRate = 0.5, iUnitLevel = 20 } )


-- 시즌2 전야 이벤트
GameEvent:AddReward( 73, { fEventTime = 150, iRewardID = 73 } )  -- 시즌2 전야 선물


-- 신캐릭터 이벤트
GameEvent:AddReward( 74, { fEventTime =   0, iRewardID = 75 } )  -- 신캐릭터 이벤트 : 경험치 15% 메달
GameEvent:AddReward( 75, { fEventTime =   0, iRewardID = 76 } )  -- 신캐릭터 이벤트 : 엘의 조각 (불명)
GameEvent:AddReward( 76, { fEventTime =   0, iRewardID = 77 } )  -- 신캐릭터 이벤트 : 축복 받은 무강 3개, 방강 3개


-- 크리스마스 이벤트
GameEvent:AddReward( 77, { fEventTime =   0, iRewardID = 78 } )  -- 포션세트
GameEvent:AddReward( 78, { fEventTime =   0, iRewardID = 79 } )  -- 출입 랜덤 큐브
GameEvent:AddReward( 79, { fEventTime =   0, iRewardID = 80 } )  -- 부활석 큐브
GameEvent:AddReward( 80, { fEventTime =   0, iRewardID = 81 } )  -- 고급 마법석 2개, 마법석 2개
GameEvent:AddReward( 81, { fEventTime =   0, iRewardID = 82 } )  -- 축복 받은 무강 2개, 축복 받은 방강 2개
GameEvent:AddReward( 82, { fEventTime =   0, iRewardID = 83 } )  -- 엘의 조각 (불명) 2개
GameEvent:AddReward( 83, { fEventTime =   0, iRewardID = 84 } )  -- 루돌프 사슴코 1개


-- 기축년 이벤트
GameEvent:AddReward( 84, { fEventTime =   0, iRewardID = 85 } )  -- 기축년 기념 칭호 1개


-- 겨울 캠프 이벤트
GameEvent:AddReward( 85,  { fEventTime = 100, iRewardID = 86  } )  -- '4주간의 겨울 캠프 1월8일자 응모권'
GameEvent:AddReward( 86,  { fEventTime = 100, iRewardID = 87  } )  -- '4주간의 겨울 캠프 1월9일자 응모권' 
GameEvent:AddReward( 87,  { fEventTime = 100, iRewardID = 88  } )  -- '4주간의 겨울 캠프 1월10일자 응모권'
GameEvent:AddReward( 88,  { fEventTime = 100, iRewardID = 89  } )  -- '4주간의 겨울 캠프 1월11일자 응모권'
GameEvent:AddReward( 89,  { fEventTime = 100, iRewardID = 90  } )  -- '4주간의 겨울 캠프 1월12일자 응모권'
GameEvent:AddReward( 90,  { fEventTime = 100, iRewardID = 91  } )  -- '4주간의 겨울 캠프 1월13일자 응모권'
GameEvent:AddReward( 91,  { fEventTime = 100, iRewardID = 92  } )  -- '4주간의 겨울 캠프 1월14일자 응모권'
GameEvent:AddReward( 92,  { fEventTime = 100, iRewardID = 93  } )  -- '4주간의 겨울 캠프 1월15일자 응모권'
GameEvent:AddReward( 93,  { fEventTime = 100, iRewardID = 94  } )  -- '4주간의 겨울 캠프 1월16일자 응모권'
GameEvent:AddReward( 94,  { fEventTime = 100, iRewardID = 95  } )  -- '4주간의 겨울 캠프 1월17일자 응모권'
GameEvent:AddReward( 95,  { fEventTime = 100, iRewardID = 96  } )  -- '4주간의 겨울 캠프 1월18일자 응모권'
GameEvent:AddReward( 96,  { fEventTime = 100, iRewardID = 97  } )  -- '4주간의 겨울 캠프 1월19일자 응모권'
GameEvent:AddReward( 97,  { fEventTime = 100, iRewardID = 98  } )  -- '4주간의 겨울 캠프 1월20일자 응모권'
GameEvent:AddReward( 98,  { fEventTime = 100, iRewardID = 99  } )  -- '4주간의 겨울 캠프 1월21일자 응모권'
GameEvent:AddReward( 99,  { fEventTime = 100, iRewardID = 100 } )  -- '4주간의 겨울 캠프 1월22일자 응모권'
GameEvent:AddReward( 100, { fEventTime = 100, iRewardID = 101 } )  -- '4주간의 겨울 캠프 1월23일자 응모권'
GameEvent:AddReward( 101, { fEventTime = 100, iRewardID = 102 } )  -- '4주간의 겨울 캠프 1월24일자 응모권'
GameEvent:AddReward( 102, { fEventTime = 100, iRewardID = 103 } )  -- '4주간의 겨울 캠프 1월25일자 응모권'
GameEvent:AddReward( 103, { fEventTime = 100, iRewardID = 104 } )  -- '4주간의 겨울 캠프 1월26일자 응모권'
GameEvent:AddReward( 104, { fEventTime = 100, iRewardID = 105 } )  -- '4주간의 겨울 캠프 1월27일자 응모권'
GameEvent:AddReward( 105, { fEventTime = 100, iRewardID = 106 } )  -- '4주간의 겨울 캠프 1월28일자 응모권'
GameEvent:AddReward( 106, { fEventTime = 100, iRewardID = 107 } )  -- '4주간의 겨울 캠프 1월29일자 응모권'
GameEvent:AddReward( 107, { fEventTime = 100, iRewardID = 108 } )  -- '4주간의 겨울 캠프 1월30일자 응모권'
GameEvent:AddReward( 108, { fEventTime = 100, iRewardID = 109 } )  -- '4주간의 겨울 캠프 1월31일자 응모권'
GameEvent:AddReward( 109, { fEventTime = 100, iRewardID = 110 } )  -- '4주간의 겨울 캠프 2월1일자 응모권' 
GameEvent:AddReward( 110, { fEventTime = 100, iRewardID = 111 } )  -- '4주간의 겨울 캠프 2월2일자 응모권' 
GameEvent:AddReward( 111, { fEventTime = 100, iRewardID = 112 } )  -- '4주간의 겨울 캠프 2월3일자 응모권' 
GameEvent:AddReward( 112, { fEventTime = 100, iRewardID = 113 } )  -- '4주간의 겨울 캠프 2월4일자 응모권' 


-- 설날 이벤트
GameEvent:AddReward( 113, { fEventTime =   0, iRewardID = 114 } )  -- 새해맞이 떡국 1개, 출입랜덤큐브 1개
GameEvent:AddReward( 114, { fEventTime =   0, iRewardID = 115 } )  -- 새해의 마음가짐 칭호 획득 아이템


-- 20레벨 이하는 경험치 200% 보너스!
GameEvent:AddReward( 115, { fEXPRate = 1.0, iUnitLevel = 20 } )


-- 화이트 데이 이벤트
GameEvent:AddReward( 116, { fEventTime =   0, iRewardID = 116 } )  -- 화이트 데이 선물 1개


-- 오픈테섭 오픈이벤트
GameEvent:AddReward( 117, { fEventTime =  30, iRewardID = 117 } )  -- 선구자를 위한 선물


-- 만우절 이벤트
GameEvent:AddReward( 118, { fEventTime =   0, iRewardID = 118 } )  -- 만우절 큐브


-- 식목일 이벤트
GameEvent:AddReward( 119, { fEventTime =  30, iRewardID = 119 } )  -- 아리엘의 꽃삽
GameEvent:AddReward( 120, { fEventTime =  30, iRewardID = 120 } )  -- 아리엘의 꽃삽 & 경험치 100% 메달 큐브


-- 블랙데이 이벤트
GameEvent:AddReward( 121, { fEventTime =  30, iRewardID = 121 } )  -- 블랙데이 선물 세트


-- 방어도 개편 이벤트
GameEvent:AddReward( 122, { fEventTime =  30, iRewardID = 122 } )  -- 축복받은 방어구 강화석
GameEvent:AddReward( 123, { fEventTime = 120, iRewardID = 123 } )  -- 방어구 전용 강화 부적


-- 어린이날 이벤트
GameEvent:AddReward( 124, { fEventTime =   0, iRewardID = 124 } )  -- 윌리엄의 초대장
GameEvent:AddReward( 125, { fEventTime =  90, iRewardID = 125 } )  -- 분노의 반지 큐브
GameEvent:AddReward( 126, { fEventTime =  90, iRewardID = 126 } )  -- 엘의 조각 5개
GameEvent:AddReward( 127, { fEventTime =  90, iRewardID = 127 } )  -- 식목일 무기큐브
GameEvent:AddReward( 128, { fEventTime =  90, iRewardID = 128 } )  -- 고급마법석 5개
GameEvent:AddReward( 129, { fEventTime =  90, iRewardID = 129 } )  -- 축무강 5개, 축방강 5개
GameEvent:AddReward( 130, { fEventTime =  90, iRewardID = 130 } )  -- 프로모션 무기큐브
GameEvent:AddReward( 131, { fEventTime =  90, iRewardID = 131 } )  -- 마법의 목걸이 큐브
GameEvent:AddReward( 132, { fEventTime =   0, iRewardID = 132 } )  -- 엘수색대 장교 제복 헤어 큐브 [5월5일 당일]
GameEvent:AddReward( 133, { fEventTime = 120, iRewardID = 133 } )  -- 넥슨캐시 1만원 응모권 [5월5일 당일]


-- 500일 이벤트
GameEvent:AddReward( 134, { fEventTime =   0, iRewardID = 134 } )  -- 아크엔젤 아바타 풀세트 큐브 [5월9일 당일]
GameEvent:AddReward( 135, { fEventTime = 120, iRewardID = 135 } )  -- 넥슨캐시 1만원 응모권 [5월9일 당일]


-- 15레벨 이하는 경험치 200% 보너스!
GameEvent:AddReward( 136, { fEXPRate = 1.0, iUnitLevel = 15 } )


-- 현충일 이벤트, Act2 페스티벌, 주말 접속 이벤트
GameEvent:AddReward( 137, { fEventTime =   0, iRewardID = 137 } )  -- 한민족의 기상 [현충일]
GameEvent:AddReward( 138, { fEventTime =  90, iRewardID = 138 } )  -- '마법의 목걸이' 큐브
GameEvent:AddReward( 139, { fEventTime =  90, iRewardID = 139 } )  -- '분노의 반지' 큐브
GameEvent:AddReward( 140, { fEventTime =   0, iRewardID = 140 } )  -- 출입 랜덤 큐브


-- 대전 활성화 이벤트
GameEvent:AddReward( 141, { fEventTime =   0, iRewardID = 141 } )  -- 카밀라의 열쇠 10개


-- ACT2외전 이벤트
GameEvent:AddReward( 142, { fEventTime =   0, iRewardID = 142 } )  -- 매우가벼운 프로모션 무기 큐브
GameEvent:AddReward( 143, { fEventTime =   0, iRewardID = 143 } )  -- 재빠른 프로모션 무기 큐브
GameEvent:AddReward( 144, { fEventTime =   0, iRewardID = 144 } )  -- 강력한 프로모션 무기 큐브


-- ACT2외전 이벤트
GameEvent:AddReward( 145, { fEventTime = 120, iRewardID = 145 } )  -- [월] 부활석 10개 큐브
GameEvent:AddReward( 146, { fEventTime = 120, iRewardID = 146 } )  -- [화] 마법석 랜덤 큐브
GameEvent:AddReward( 147, { fEventTime = 120, iRewardID = 147 } )  -- [수] 플루오르 스톤 큐브
GameEvent:AddReward( 148, { fEventTime = 120, iRewardID = 148 } )  -- [목] 강화 큐브
GameEvent:AddReward( 149, { fEventTime = 120, iRewardID = 149 } )  -- [금] 액세서리 큐브
GameEvent:AddReward( 150, { fEventTime = 120, iRewardID = 150 } )  -- [토] 경험치 보너스 메달 큐브
GameEvent:AddReward( 151, { fEventTime = 120, iRewardID = 151 } )  -- [일] 랜덤 칭호 아이템 큐브

GameEvent:AddReward( 152, { fEventTime =   0, iRewardID = 152 } )  -- 헤니르의 열쇠 3개
GameEvent:AddReward( 153, { fEventTime =   0, iRewardID = 153 } )  -- 헤니르의 비밀 열쇠 1개


-- ACT2 외전 마지막 선물 큐브
GameEvent:AddReward( 154, { fEventTime =   0, iRewardID = 154 } )  -- ACT2 외전 마지막 선물 큐브


-- 신학기 이벤트
GameEvent:AddReward( 155, { fEventTime =   0, iRewardID = 155 } )  -- 신학기 특급 선물 큐브 1개
GameEvent:AddReward( 156, { fEventTime =  60, iRewardID = 156 } )  -- 신학기 스페셜 선물 큐브 1개


-- 패션쇼 이벤트
GameEvent:AddReward( 157, { fEventTime =  60, iRewardID = 157 } )  -- 패션쇼 큐브 1개
GameEvent:AddReward( 158, { fEventTime =   0, iRewardID = 158 } )  -- 매우 가벼운 프로모션 무기 큐브 1개
GameEvent:AddReward( 159, { fEventTime =   0, iRewardID = 159 } )  -- 재빠른 프로모션 무기 큐브 1개
GameEvent:AddReward( 160, { fEventTime =   0, iRewardID = 160 } )  -- 강력한 프로모션 무기 큐브 1개


-- 한가위 이벤트
GameEvent:AddReward( 161, { fEventTime =  80, iRewardID = 161 } )  -- 한가위 종합 선물 세트 (월)
GameEvent:AddReward( 162, { fEventTime =  80, iRewardID = 162 } )  -- 한가위 종합 선물 세트 (화)
GameEvent:AddReward( 163, { fEventTime =  80, iRewardID = 163 } )  -- 한가위 종합 선물 세트 (수)
GameEvent:AddReward( 164, { fEventTime =  80, iRewardID = 164 } )  -- 한가위 종합 선물 세트 (목)
GameEvent:AddReward( 165, { fEventTime =  80, iRewardID = 165 } )  -- 한가위 종합 선물 세트 (금)
GameEvent:AddReward( 166, { fEventTime =  80, iRewardID = 166 } )  -- 한가위 종합 선물 세트 (토)
GameEvent:AddReward( 167, { fEventTime =  80, iRewardID = 167 } )  -- 한가위 종합 선물 세트 (일)


-- ACT3 이벤트
GameEvent:AddReward( 168, { fEventTime =  60, iRewardID = 168 } )  -- ACT3 기념 행복한 토요일 큐브 (10월 31일)
GameEvent:AddReward( 169, { fEventTime =  60, iRewardID = 169 } )  -- ACT3 기념 행복한 토요일 큐브 (11월 7일)
GameEvent:AddReward( 170, { fEventTime =  60, iRewardID = 170 } )  -- ACT3 기념 행복한 토요일 큐브 (11월 14일)
GameEvent:AddReward( 171, { fEventTime =  60, iRewardID = 171 } )  -- ACT3 기념 행복한 토요일 큐브 (11월 21일)
GameEvent:AddReward( 172, { fEventTime =  80, iRewardID = 172 } )  -- ACT3 출석권


-- PC방 이벤트
GameEvent:AddReward( 173, { fEventTime =  50, iRewardID = 173, bPcBangEvent = True } )  -- PC방 출석권
GameEvent:AddReward( 174, { fEventTime =   0, iRewardID = 174, bPcBangEvent = True } )  -- PC방 스페셜 카드
GameEvent:AddReward( 175, { fEventTime =  50, iRewardID = 175, bPcBangEvent = True } )  -- 그레이트 비틀 무기 큐브


-- ACT3 강화 이벤트
GameEvent:AddReward( 176, { fEventTime =   0, iRewardID = 176 } )  -- 강화 이벤트 주문서 A
GameEvent:AddReward( 177, { fEventTime =   0, iRewardID = 177 } )  -- 강화 이벤트 주문서 B


-- 엘소드2차 전직 이벤트
GameEvent:AddReward( 178, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )  -- 엘소드 캐릭터 35렙 이하 경험치 50%


-- 계정단위 접속시간 이벤트
GameEvent:AddReward( 179, { fEventTime =   1, iRewardID = 179 } )  -- 얼음 조각상 가열기


-- 겨울방학전야이벤트
GameEvent:AddReward( 180, { fEventTime =  70, iRewardID = 180 } )  -- 히어로 큐브 A
GameEvent:AddReward( 181, { fEventTime = 100, iRewardID = 181 } )  -- 히어로 큐브 B
GameEvent:AddReward( 182, { fEventTime =   0, iRewardID = 182 } )  -- 히어로 큐브 C


-- 크리스마스 트리 이벤트 [누적시간이벤트]
GameEvent:AddReward( 183, { fEventTime =  40, iRewardID = 183, bDirectReward = True } )	-- 크리스마스 트리 이벤트


-- 레나2차 전직 이벤트
GameEvent:AddReward( 184, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_LIRE"] } )  -- 레나 캐릭터 35렙 이하 경험치 50%


-- 강화 데이 이벤트
GameEvent:AddReward( 185, { fEventTime =   0, iRewardID = 184, iUnitLevel = 10 } ) -- 강화의 부적 랜덤 큐브 (Lv.5~Lv.8)


-- 크리스마스 이벤트
GameEvent:AddReward( 186, { fEventTime =   0, iRewardID = 185 } ) -- 크리스마스 특별 아바타 무기(7일권)큐브
GameEvent:AddReward( 187, { fEventTime =   0, iRewardID = 186 } ) -- 2008 크리스마스 아바타 풀 세트(1일권)큐브


-- 2주년 이벤트
GameEvent:AddReward( 188, { fEventTime =   0, iRewardID = 187, iUnitLevel = 10 } ) -- 얼음 조각상 가열기 (아리엘의 선물)
GameEvent:AddReward( 189, { fEventTime =   0, iRewardID = 188 } ) -- 엘소드 2주년 칭호 아이템


-- 신서버 이벤트
GameEvent:AddReward( 190, { fEventTime =   0, iRewardID = 194 } ) -- 헤니르의 열쇠 큐브


-- PC방 이벤트
GameEvent:AddReward( 191, { fEventTime =  70, iRewardID = 196, bPcBangEvent = True } ) -- 완전 회복 포션 10개


-- 새해 맞이 이벤트 [누적시간이벤트]
GameEvent:AddReward( 192, { fEventTime = 100, iRewardID = 197 } ) -- (월) 플루오르 스톤 교환권
GameEvent:AddReward( 193, { fEventTime = 100, iRewardID = 198 } ) -- (화) 반지 악세사리 1일권 큐브
GameEvent:AddReward( 194, { fEventTime = 100, iRewardID = 199 } ) -- (수) 프로모션 아바타 1일권 큐브
GameEvent:AddReward( 195, { fEventTime = 100, iRewardID = 200 } ) -- (목) 아이템 업그레이드 재료 교환권
GameEvent:AddReward( 196, { fEventTime = 100, iRewardID = 201 } ) -- (금) 새해의 마음가짐(5일권)
GameEvent:AddReward( 197, { fEventTime = 100, iRewardID = 202 } ) -- (토) 경험치 메달 1일권 큐브
GameEvent:AddReward( 198, { fEventTime = 100, iRewardID = 203 } ) -- (일) 악세사리 1일권 큐브


-- 새해 맞이 이벤트
GameEvent:AddReward( 199, { fEventTime =   0, iRewardID = 204 } ) -- 새해 맞이 이벤트 : 새해 맞이 한복 아바타 (1일권) 큐브


-- 아이샤2차 전직 이벤트
GameEvent:AddReward( 200, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_ARME"] } )  -- 아이샤 캐릭터 35렙 이하 경험치 50%


-- 겨울방학 이벤트 [누적시간이벤트]
GameEvent:AddReward( 201, { fEventTime =  80, iRewardID = 205 } ) -- 겨울방학 출석 스탬프 평일 5개
GameEvent:AddReward( 202, { fEventTime = 100, iRewardID = 206 } ) -- 겨울방학 출석 스탬프 주말 10개


-- PC방 이벤트
GameEvent:AddReward( 203, { fEventTime =  60, iRewardID = 207, bPcBangEvent = True } )  -- PC방 출석권
GameEvent:AddReward( 204, { fEventTime =   0, iRewardID = 208, bPcBangEvent = True } )  -- 헤니르 비밀열쇠 1개


-- PC방 이벤트
GameEvent:AddReward( 205, { fEventTime =  60, iRewardID = 209, bPcBangEvent = True } )  -- PC방 선물박스, 특별출석권
GameEvent:AddReward( 206, { fEventTime =   0, iRewardID = 210, bPcBangEvent = True } )  -- PC방 스페셜 큐브 1개


-- 레이븐2차 전직 이벤트
GameEvent:AddReward( 207, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_RAVEN"] } )  -- 레이븐 캐릭터 35렙 이하 경험치 50%


-- 이브2차 전직 이벤트
GameEvent:AddReward( 208, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_EVE"] } )  -- 이브 캐릭터 35렙 이하 경험치 50%


-- 가열기 이벤트
GameEvent:AddReward( 209, { fEventTime =   0, iRewardID = 187 } ) -- 얼음 조각상 가열기 (아리엘의 선물)


-- PC방 3월 이벤트
GameEvent:AddReward( 210, { fEventTime =  30, iRewardID = 211, bPcBangEvent = True } ) -- PC방 3월의 선물 큐브A 1개
GameEvent:AddReward( 211, { fEventTime =  30, iRewardID = 212, bPcBangEvent = True } ) -- PC방 3월의 선물 큐브B 1개


-- 화이트 데이 이벤트 [누적시간이벤트]
GameEvent:AddReward( 212, { fEventTime =  80, iRewardID = 10000 } ) -- 화이트 데이 기념 큐브A
GameEvent:AddReward( 213, { fEventTime = 100, iRewardID = 10001 } ) -- 화이트 데이 기념 큐브B

-- 화이트 데이 이벤트
GameEvent:AddReward( 214, { fEventTime =   0, iRewardID = 10002 } ) -- 화이트 데이 기념 큐브C


-- 던전 채널 통합 이벤트
GameEvent:AddReward( 215, { fEventTime =   0, iRewardID = 10003 } ) -- 던전 채널 통합 기념 큐브


-- 4월 PC방 이벤트
GameEvent:AddReward( 216, { fEventTime =  30, iRewardID = 10004, bPcBangEvent = True } ) -- PC방 4월의 선물 큐브A 1개
GameEvent:AddReward( 217, { fEventTime =  30, iRewardID = 10005, bPcBangEvent = True } ) -- PC방 4월의 선물 큐브B 1개


-- 만우절 깜짝 이벤트
GameEvent:AddReward( 218, { fEventTime =   0, iRewardID = 10006 } ) -- 만우절 특별 큐브


-- 식목일 이벤트
GameEvent:AddReward( 219, { fEventTime =   0, iRewardID = 10007 } ) -- 자연의 선물 3개
GameEvent:AddReward( 220, { fEventTime =   0, iRewardID = 10008 } ) -- 자연의 선물 5개
GameEvent:AddReward( 221, { fEventTime =   0, iRewardID = 10009, bPcBangEvent = True } ) -- 자연의 선물 2개


-- 열공모드 이벤트
GameEvent:AddReward( 222, { fEventTime =   0, iRewardID = 10010 } ) -- 고양이 가방 큐브(7일권) 1개


-- Special Week 이벤트
GameEvent:AddReward( 223, { fEventTime =  60, iRewardID = 10011 } ) -- 5/1 경험치 메달 1일권, 망각의 알약 3개
GameEvent:AddReward( 224, { fEventTime =  60, iRewardID = 10012 } ) -- 5/2 강화의 부적 랜덤 큐브, 망각의 알약 3개
GameEvent:AddReward( 225, { fEventTime =  60, iRewardID = 10013 } ) -- 5/3 플루오르 스톤 교환권
GameEvent:AddReward( 226, { fEventTime =  60, iRewardID = 10014 } ) -- 5/4 엘의 조각(불명) 5개
GameEvent:AddReward( 227, { fEventTime =  60, iRewardID = 10015 } ) -- 5/6 부활석 10개 큐브
GameEvent:AddReward( 228, { fEventTime =  60, iRewardID = 10016 } ) -- 5/7 프로모션 아바타 큐브(3일권)


-- 어린이날 이벤트
GameEvent:AddReward( 229, { fEventTime =   0, iRewardID = 10017 } ) -- 어린이날 선물 세트 큐브


-- 5월 PC방 이벤트
GameEvent:AddReward( 230, { fEventTime =   0, iRewardID = 10018, bPcBangEvent = True } ) -- PC방 특별 접속 이벤트


-- 천안함 추모 이벤트
GameEvent:AddReward( 231, { fEventTime =   0, iRewardID = 10019 } ) -- 흰국화 큐브


-- 헬모드 도전자 이벤트
GameEvent:AddReward( 232, { fEventTime =   0, iRewardID = 10020 } ) -- 헬던전 출입권 랜덤 큐브 1개
GameEvent:AddReward( 233, { fEventTime =   0, iRewardID = 10021 } ) -- 헬던전 출입권 큐브 1개


-- 스승의 날 이벤트
GameEvent:AddReward( 234, { fEventTime =  60, iRewardID = 10022 } ) -- 스승의 날 선물 큐브 1개


-- 서버군 통합 이벤트
GameEvent:AddReward( 235, { fEventTime =  90, iRewardID = 10023 } ) -- 특별 출입증 교환권 1개


-- 6월 PC방 이벤트
GameEvent:AddReward( 236, { fEventTime =  50, iRewardID = 10024, bPcBangEvent = True } ) -- 활력의 포션 5개
GameEvent:AddReward( 237, { fEventTime =  70, iRewardID = 10025, bPcBangEvent = True } ) -- 활력의 포션 10개


-- 900일 이벤트
GameEvent:AddReward( 238, { fEventTime =  90, iRewardID = 10026 } ) -- 몬스터 탈(7일권) 랜덤 큐브
GameEvent:AddReward( 239, { fEventTime =  90, iRewardID = 10027 } ) -- 골드 프로모션 무기 아바타(7일권) 큐브
GameEvent:AddReward( 240, { fEventTime =  90, iRewardID = 10028 } ) -- 무기 액세서리(15일권) 큐브
GameEvent:AddReward( 241, { fEventTime =  90, iRewardID = 10029 } ) -- 특수 액세서리(15일권) 큐브
GameEvent:AddReward( 242, { fEventTime =  90, iRewardID = 10030 } ) -- 기술의 반지(7일권) 큐브


-- 900일의 혜택 이벤트
GameEvent:AddReward( 243, { fEventTime =   0, iRewardID = 10031 } ) -- 특수 액세서리(3일권) 큐브
GameEvent:AddReward( 244, { fEventTime =   0, iRewardID = 10032 } ) -- 엘의 조각(불명) 3개
GameEvent:AddReward( 245, { fEventTime =   0, iRewardID = 10033 } ) -- 경험치 메달 1일권 큐브
GameEvent:AddReward( 246, { fEventTime =   0, iRewardID = 10034 } ) -- 기술의 반지(3일권) 큐브
GameEvent:AddReward( 247, { fEventTime =   0, iRewardID = 10035 } ) -- 플루오르 스톤 랜덤 큐브
GameEvent:AddReward( 248, { fEventTime =   0, iRewardID = 10036 } ) -- 마법석 랜덤 큐브
GameEvent:AddReward( 249, { fEventTime =   0, iRewardID = 10037 } ) -- 활력의 포션 3개
GameEvent:AddReward( 250, { fEventTime =   0, iRewardID = 10038 } ) -- 액세서리 큐브(얼굴(중), 3일권) 큐브
GameEvent:AddReward( 251, { fEventTime =   0, iRewardID = 10039 } ) -- 축복받은 복원의 주문서 랜덤 큐브
GameEvent:AddReward( 252, { fEventTime =   0, iRewardID = 10040 } ) -- 완전 회복 포션 3개
GameEvent:AddReward( 253, { fEventTime =   0, iRewardID = 10041 } ) -- 액세서리 큐브(팔, 3일권) 큐브


-- 월드컵 이벤트
GameEvent:AddReward( 254, { fEventTime =   0, iRewardID = 10042 } ) -- 1승 기념 특별 아이템 큐브
GameEvent:AddReward( 255, { fEventTime =   0, iRewardID = 10043 } ) -- 16강 기념 특별 아이템 큐브
GameEvent:AddReward( 256, { fEventTime =   0, iRewardID = 10044 } ) -- 8강 기념 특별 아이템 큐브


-- 7월 PC방 이벤트
GameEvent:AddReward( 257, { fEventTime =  50, iRewardID = 10045, bPcBangEvent = True } ) -- 응축된 마나 결정 1개
GameEvent:AddReward( 258, { fEventTime =  50, iRewardID = 10046, bPcBangEvent = True } ) -- 응축된 마나 결정 2개


-- 파티 플레이 드롭률 이벤트
GameEvent:AddReward( 259, { iDropCount =  2, bWithPlayPcBang = True } ) -- PC방 유저와 함께 플레이시 드롭률 2배


-- 여름방학 PC방 이벤트
GameEvent:AddReward( 260, { fEventTime =  30, iRewardID = 10047, bPcBangEvent = True } ) -- PC방 7월의 선물 큐브
GameEvent:AddReward( 261, { fEventTime =  30, iRewardID = 10048, bPcBangEvent = True } ) -- ACT4 기념 루벤 티켓, ACT4 기념 벨더 티켓


-- 가이아 서버 이벤트
GameEvent:AddReward( 262, { fEventTime =  60, iRewardID = 10049, bAccountEvent = True } ) -- 가이아의 축복 큐브


-- 가열기 이벤트
GameEvent:AddReward( 263, { fEventTime =   0, iRewardID = 10050, bAccountEvent = True } ) -- 얼음 조각상 가열기 (아리엘의 선물)
GameEvent:AddReward( 264, { fEventTime =  30, iRewardID = 10050, bAccountEvent = True } ) -- 얼음 조각상 가열기 (아리엘의 선물)
GameEvent:AddReward( 265, { fEventTime =  60, iRewardID = 10050, bAccountEvent = True } ) -- 얼음 조각상 가열기 (아리엘의 선물)


-- 주말 이벤트
GameEvent:AddReward( 266, { fEventTime =  80, iRewardID = 10051 } ) -- 특별 교환권


-- 드롭률 2배 이벤트
GameEvent:AddReward( 267, { iDropCount =  2 } ) -- 드롭률 2배


-- 8월 PC방 이벤트
GameEvent:AddReward( 268, { fEventTime =  50, iRewardID = 10052, bPcBangEvent = True } ) -- 망각의 알약 1개
GameEvent:AddReward( 269, { fEventTime =  50, iRewardID = 10053, bPcBangEvent = True } ) -- ACT4 기념 루벤 티켓 1개
GameEvent:AddReward( 270, { fEventTime =  50, iRewardID = 10054, bPcBangEvent = True } ) -- ACT4 기념 벨더 티켓 1개


-- 방학숙제 이벤트
GameEvent:AddReward( 271, { fEventTime =  60, iRewardID = 10055 } ) -- 방학 숙제 재료 1개


-- 1000일 이벤트
GameEvent:AddReward( 272, { fEventTime =  60, iRewardID = 10056 } ) -- 1000일을 위한 준비물 1개


-- 한가위 특별 선물 이벤트 & 1000일 기념 이벤트
GameEvent:AddReward( 273, { fEventTime =  30, iRewardID = 10057 } ) -- 아바타 세트(1일권) 큐브 - 동물 코스프레
GameEvent:AddReward( 274, { fEventTime =  30, iRewardID = 10058 } ) -- 아바타 세트(1일권) 큐브 - 스타 아카데미 하복
GameEvent:AddReward( 275, { fEventTime =  30, iRewardID = 10059 } ) -- 아바타 세트(1일권) 큐브 - 2009 썸머룩
GameEvent:AddReward( 276, { fEventTime =  30, iRewardID = 10060 } ) -- 아바타 세트(1일권) 큐브 - 판타지 아카데미
GameEvent:AddReward( 277, { fEventTime =  30, iRewardID = 10061 } ) -- 아바타 세트(1일권) 큐브 - 겨울 한복
GameEvent:AddReward( 278, { fEventTime =  30, iRewardID = 10062 } ) -- 1000일 기념 액세서리 세트(1일권) 큐브
GameEvent:AddReward( 279, { fEventTime =  30, iRewardID = 10063 } ) -- 아바타 세트(1일권) 큐브 - 2009 한가위
GameEvent:AddReward( 280, { fEventTime =  30, iRewardID = 10064 } ) -- 아바타 세트(1일권) 큐브 - 마린룩
GameEvent:AddReward( 281, { fEventTime =  30, iRewardID = 10065 } ) -- 아바타 세트(1일권) 큐브 - 밀리터리 룩
GameEvent:AddReward( 282, { fEventTime =  30, iRewardID = 10066 } ) -- 아바타 세트(1일권) 큐브 - 5월의 웨딩
GameEvent:AddReward( 283, { fEventTime =  30, iRewardID = 10067 } ) -- 아바타 세트(1일권) 큐브 - 스타 아카데미
GameEvent:AddReward( 284, { fEventTime =  30, iRewardID = 10068 } ) -- 아바타 세트(1일권) 큐브 - 2010 서머 비치룩
GameEvent:AddReward( 285, { fEventTime =  30, iRewardID = 10069 } ) -- 아바타 세트(1일권) 큐브 - 아이돌 밴드
GameEvent:AddReward( 286, { fEventTime =  30, iRewardID = 10070 } ) -- 아바타 세트(1일권) 큐브 - 대륙의 모험가


-- 시험대박 이벤트
GameEvent:AddReward( 287, { fEventTime =  30, iRewardID = 10071 } ) -- 시험 대박 7종 신기 큐브(1일차)
GameEvent:AddReward( 288, { fEventTime =  30, iRewardID = 10072 } ) -- 시험 대박 7종 신기 큐브(2일차)
GameEvent:AddReward( 289, { fEventTime =  30, iRewardID = 10073 } ) -- 시험 대박 7종 신기 큐브(3일차)
GameEvent:AddReward( 290, { fEventTime =  30, iRewardID = 10074 } ) -- 시험 대박 7종 신기 큐브(4일차)
GameEvent:AddReward( 291, { fEventTime =  30, iRewardID = 10075 } ) -- 시험 대박 7종 신기 큐브(5일차)
GameEvent:AddReward( 292, { fEventTime =  30, iRewardID = 10076 } ) -- 시험 대박 7종 신기 큐브(6일차)
GameEvent:AddReward( 293, { fEventTime =  30, iRewardID = 10077 } ) -- 시험 대박 7종 신기 큐브(7일차)
GameEvent:AddReward( 294, { fEventTime =   0, iRewardID = 10078 } ) -- 경험치 15% 메달(1일권)큐브


-- 할로윈 이벤트
GameEvent:AddReward( 295, { fEventTime =  30, iRewardID = 10079 } ) -- 할로윈 고스트 캔디 (평일 3개)
GameEvent:AddReward( 296, { fEventTime =  30, iRewardID = 10080 } ) -- 할로윈 고스트 캔디 (주말 6개)

-- 유럽 CBT 이벤트
GameEvent:AddReward( 500, { fEventTime =  30, iRewardID = 20000 } ) -- 기본 프로모션 아바타 7일권 큐브
GameEvent:AddReward( 501, { fEventTime =  30, iRewardID = 20001 } ) -- 축복받은 무기 강화석
GameEvent:AddReward( 502, { fEventTime =  30, iRewardID = 20002 } ) -- 축복받은 방어구 강화석
GameEvent:AddReward( 503, { fEventTime =  30, iRewardID = 20003 } ) -- 부활석(5개) 큐브
GameEvent:AddReward( 504, { fEventTime =  30, iRewardID = 20004 } ) -- 마법석 (5개)큐브
GameEvent:AddReward( 505, { fEventTime =  30, iRewardID = 20005 } ) -- 고급 마법석 5개 큐브
GameEvent:AddReward( 506, { fEventTime =  30, iRewardID = 20006 } ) -- 엘의 조각(불명)

-- 유럽 OBT 이벤트
GameEvent:AddReward( 507, { fEventTime =  60, iRewardID = 20007 } ) -- OBT기념큐브1
GameEvent:AddReward( 508, { fEventTime =  60, iRewardID = 20008 } ) -- OBT기념큐브2
GameEvent:AddReward( 509, { fEventTime =  60, iRewardID = 20009 } ) -- OBT기념큐브3
GameEvent:AddReward( 510, { fEventTime =  60, iRewardID = 20010 } ) -- OBT기념큐브4
GameEvent:AddReward( 511, { fEventTime =  60, iRewardID = 20011 } ) -- OBT기념큐브5
GameEvent:AddReward( 512, { fEventTime =  60, iRewardID = 20012 } ) -- OBT기념큐브6
GameEvent:AddReward( 513, { fEventTime =  60, iRewardID = 20013 } ) -- OBT기념큐브7

-- bConditionReward 코드가 임시 코드여서 코드 삭제로 해당 부분 주석 처리 
-- 유럽 크리스마스 , 새해 이벤트  ( 트리 이벤트 )
--GameEvent:AddReward( 514, { fEventTime = 100, iRewardID = 20016, bConditionReward = True } )	-- 크리스마스 양말
--GameEvent:AddReward( 515, { fEventTime = 100, iRewardID = 20017, bConditionReward = True } )	-- 축하케익박스

-- 윌리엄 시공 이벤트
GameEvent:AddReward( 516, { fEventTime =  0, iRewardID = 20022 } ) -- 평일 이벤트 (Weekday)
GameEvent:AddReward( 517, { fEventTime =  0, iRewardID = 20023 } ) -- 주말 이벤트 (Weekend)

-- 유럽 신 스킬 업데이트 접속 이벤트 (주말용)
GameEvent:AddReward( 518, { fEventTime =  120, iRewardID = 20024 } ) -- 기술의 반지 시리즈 큐브(1일) / 19일
GameEvent:AddReward( 519, { fEventTime =  120, iRewardID = 20025 } ) -- 기술의 반지 시리즈 큐브(1일) / 20일
GameEvent:AddReward( 520, { fEventTime =  120, iRewardID = 20026 } ) -- 기술의 반지 시리즈 큐브(1일) / 26일
GameEvent:AddReward( 521, { fEventTime =  120, iRewardID = 20027 } ) -- 기술의 반지 시리즈 큐브(1일) / 27일


-- 유럽 신 스킬 업데이트 접속 이벤트 (일일)
GameEvent:AddReward( 522, { fEventTime =  30, iRewardID = 20028 } ) -- 기술의 티켓 1장

-- 유럽 만우절 접속 이벤트
GameEvent:AddReward( 523, { fEventTime =  0, iRewardID = 20029 } ) -- 만우절 접속 이벤트

-- 윌리엄 시공 2 이벤트용 아이템 주기
GameEvent:AddReward( 524, { fEventTime =  0, iRewardID = 20030 } ) -- 오염된 행운의 편지


-- 부활절 이벤트
GameEvent:AddReward( 525, { fEventTime =  0, iRewardID = 20031 } ) -- 부활절 토끼 칭호
GameEvent:AddReward( 526, { fEventTime =  0, iRewardID = 20032 } ) -- 토끼귀 큐브 접속이벤트용 
GameEvent:AddReward( 527, { fEventTime =  0, iRewardID = 20033 } ) -- 부활절 토끼 무기 큐브


-- 엘소드 2차 전직 이벤트
GameEvent:AddReward( 528, { fEventTime = 100, iRewardID = 20035 } ) -- 전직의 기운 	-- 일단 5분 세팅 100분으로 수정해야 함

GameEvent:AddReward( 529, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )  -- 엘소드 캐릭터 35렙 이하 경험치 50%
GameEvent:AddReward( 530, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_ARME"] } )  -- 아이샤 캐릭터 35렙 이하 경험치 50%
GameEvent:AddReward( 531, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_LIRE"] } )  -- 레나 캐릭터 35렙 이하 경험치 50%

-- 사진 복원기 접속 이벤트
GameEvent:AddReward( 532, { fEventTime = 0, iRewardID = 20036, bGiveOnce = True } )

-- 35레벨 이하 레이븐 경험치 이벤트
GameEvent:AddReward( 533, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_RAVEN"] } )  -- 레이븐 캐릭터 35렙 이하 경험치 50%

-- 새로운 모험가 칭호 아이템 지급 이벤트
GameEvent:AddReward( 534, { fEventTime = 0, iRewardID = 20037, bGiveOnce = True } )

-- 기술의 노트 홍보 큐브 지급 이벤트
GameEvent:AddReward( 535, { fEventTime = 0, iRewardID = 20038, bGiveOnce = True } )

-- Bo-Co사의 지원 큐브 지급 이벤트
GameEvent:AddReward( 536, { fEventTime = 0, iRewardID = 20039, bGiveOnce = True } )

-- Lv1~Lv10 유저 접속시 아리엘의 초보 지원 큐브 지급 이벤트
GameEvent:AddReward( 537, { fEventTime = 0, iRewardID = 20040, bGiveOnce = True, iMinLevel = 1, iMaxLevel = 10 } ) -- 아리엘의 초보지원 큐브

-- 비밀던전 접속 이벤트(일일)
GameEvent:AddReward( 538, { fEventTime = 0, iRewardID = 20058 } ) -- 아리엘의 비밀던전 입장권

-- 프랑스 OBT 기념 접속 이벤트
GameEvent:AddReward( 539, { fEventTime = 60, iRewardID = 20059 } ) -- 프랑스 OBT 기념 큐브

-- 펫 전야 쁘띠뽀루의 목걸이 지급 이벤트
GameEvent:AddReward( 540, { fEventTime = 0, iRewardID = 20060, bGiveOnce = True } ) -- 쁘띠 뽀루의 목걸이(15일) 큐브

-- 할로윈 고스트 캔디 지급 이벤트
GameEvent:AddReward( 541, { fEventTime =   30, iRewardID = 20062 } ) -- 할로윈 고스트 캔디 5개 지급
GameEvent:AddReward( 542, { fEventTime =   30, iRewardID = 20063 } ) -- 할로윈 고스트 캔디 10개 지급

-- 할로윈 던전 무료입장
GameEvent:AddReward( 543, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_NORMAL"]} )
GameEvent:AddReward( 544, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_HARD"]	} )
GameEvent:AddReward( 545, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_EXPERT"]} )

GameEvent:AddReward( 546, { fEventTime =   0, iRewardID = 20064 } ) -- 코보 익스프레스 VIP 티켓 1일권

-- 35레벨 이하 이브 경험치 이벤트
GameEvent:AddReward( 547, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_EVE"] } )  -- 이브 캐릭터 35렙 이하 경험치 50%

-- 나소드의 여왕 칭호 아이템 우편 지급 이벤트
GameEvent:AddReward( 548, { fEventTime = 0, iRewardID = 20065, bGiveOnce = True } )

-- 크리스마스 루돌프 빨간코 15일권 큐브 우편 지급 이벤트
GameEvent:AddReward( 549, { fEventTime = 0, iRewardID = 20068, bGiveOnce = True } )

-- 크리스마스 특별 선물 우편 지급 이벤트
GameEvent:AddReward( 550, { fEventTime = 150, iRewardID = 20069, bAccountEvent = True } )

-- 헤니르 시공 입장권 우편 지급 이벤트(매일)
GameEvent:AddReward( 551, { fEventTime = 0, iRewardID = 20070 } ) -- 헤니르 시공 입장권 3매

-- 뽀루의 발도장(흰색) 1매 지급 이벤트(계정단위, 120분)
GameEvent:AddReward( 552, { fEventTime = 120, iRewardID = 20071, bAccountEvent = True } ) -- 뽀루의 발도장(흰색) 1매

-- 완전회복 포션 10개 지급 이벤트(계정단위, 120분)
GameEvent:AddReward( 553, { fEventTime = 120, iRewardID = 20072, bAccountEvent = True } ) -- 완전회복 포션 10개

-- 뽀루의 발도장(노란색) 1매 지급 이벤트(계정단위, 120분)
GameEvent:AddReward( 554, { fEventTime = 120, iRewardID = 20073, bAccountEvent = True } ) -- 뽀루의 발도장(노란색) 1매

-- 엘의조각(불명) 5개 지급 이벤트(계정단위, 120분)
GameEvent:AddReward( 555, { fEventTime = 120, iRewardID = 20074, bAccountEvent = True } ) -- 엘의조각(불명) 5개

-- 뽀루의 발도장(보라색) 1매 지급 이벤트(계정단위, 120분)
GameEvent:AddReward( 556, { fEventTime = 120, iRewardID = 20075, bAccountEvent = True } ) -- 뽀루의 발도장(보라색) 1매

-- 축복받은 무기 강화석 20개 지급 이벤트(계정단위, 120분)
GameEvent:AddReward( 557, { fEventTime = 120, iRewardID = 20076, bAccountEvent = True } ) -- 축복받은 무기 강화석 20개

-- 폴란드 OBT 기념 접속 이벤트
GameEvent:AddReward( 558, { fEventTime = 60, iRewardID = 20067 } ) -- 폴란드 OBT 기념 큐브

-- 디펜스 던전 접속 이벤트(캐릭터 단위, 접속 즉시 지급)
GameEvent:AddReward( 559, { fEventTime = 0, iRewardID = 20083, bGiveOnce = True } ) -- 비룡의 큐브

-- 던전 사용 아이템 접속 이벤트(계정 단위, 접속 즉시 지급)
GameEvent:AddReward( 560, { fEventTime = 0, iRewardID = 20084, bAccountEvent = True } ) -- 연금술사의 전투 지원 랜덤 큐브

-- 엘소드 데이 접속 이벤트(계정 단위, 100분)
GameEvent:AddReward( 561, { fEventTime = 100, iRewardID = 20085, bAccountEvent = True } ) -- 일급 마법석 랜덤 큐브

-- 엘소드 데이 접속 이벤트(계정 단위, 100분)
GameEvent:AddReward( 562, { fEventTime = 100, iRewardID = 20086, bAccountEvent = True } ) -- 플루오르 스톤 랜덤 큐브

-- 엘소드 데이 접속 이벤트(계정 단위, 100분)
GameEvent:AddReward( 563, { fEventTime = 100, iRewardID = 20087, bAccountEvent = True } ) -- 완전 회복 포션 10개

-- 엘소드 데이 접속 이벤트(계정 단위, 100분)
GameEvent:AddReward( 564, { fEventTime = 100, iRewardID = 20088, bAccountEvent = True } ) -- 부활석 5개 큐브

-- 공존의 축제 접속 이벤트(캐릭터 단위, 접속 즉시 지급, 기간내 한 번 지급)
GameEvent:AddReward( 565, { fEventTime = 0, iRewardID = 20089, bGiveOnce = True } ) -- 공존의 축제 의상 큐브

-- 만우절 접속 이벤트(캐릭터 단위, 접속 즉시 지급, 기간내 한 번 지급)
GameEvent:AddReward( 566, { fEventTime = 0, iRewardID = 20090, bGiveOnce = True } ) -- 피노키오의 매력(1일권)

-- 만우절 접속 이벤트(캐릭터 단위, 접속 즉시 지급, 기간내 한 번 지급)
GameEvent:AddReward( 567, { fEventTime = 0, iRewardID = 20091, bGiveOnce = True } ) -- 양치기의 무기 큐브 3개 큐브

-- 만우절 접속 이벤트(캐릭터 단위, 120분, 기간내 한 번 지급)
GameEvent:AddReward( 568, { fEventTime = 120, iRewardID = 20091, bGiveOnce = True } ) -- 양치기의 무기 큐브 3개 큐브

-- 식목일 이벤트 던전 아이템 드롭률 2배
GameEvent:AddReward( 569, { iDropCount = 2, iDungeonID = 39500 } ) -- 엘더 식목일 던전

-- 식목일 이벤트 던전 아이템 드롭률 2배
GameEvent:AddReward( 570, { iDropCount = 2, iDungeonID = 39510 } ) -- 베스마 식목일 던전

-- 식목일 이벤트 던전 아이템 드롭률 2배
GameEvent:AddReward( 571, { iDropCount = 2, iDungeonID = 39520 } ) -- 알테라 식목일 던전

-- 식목일 이벤트 던전 아이템 드롭률 2배
GameEvent:AddReward( 572, { iDropCount = 2, iDungeonID = 39540 } ) -- 벨더 식목일 던전

-- 스페인 OBT 기념 접속 이벤트(
GameEvent:AddReward( 573, { fEventTime = 60, iRewardID = 20067, bGiveOnce = True } ) -- 스페인 OBT 기념 큐브

-- 식목일 접속 이벤트(캐릭터 단위, 30분, 평일)
GameEvent:AddReward( 574, { fEventTime = 30, iRewardID = 20092, bGiveOnce = True } ) -- 자연의 선물 5개

-- 식목일 접속 이벤트(캐릭터 단위, 30분, 주말)
GameEvent:AddReward( 575, { fEventTime = 30, iRewardID = 20093, bGiveOnce = True } ) -- 자연의 선물 10개

-- 일일퀴즈 접속 이벤트(캐릭터 단위, 접속 즉시 지급)
GameEvent:AddReward( 576, { fEventTime = 0, iRewardID = 20094 } ) -- QuizQuiz 칭호(15일)

-- 하멜의 부서진 기둥 이벤트(캐릭터 단위, 60분, 누적 접속 시간 이벤트, 하루에 3번까지 보상 지급)
GameEvent:AddReward( 577, { fEventTime =  60, iRewardID = 577, bDirectReward = True } ) -- 직접 보상(하멜의 흔전 1개)

-- 하멜 전야 이벤트(캐릭터 단위, 접속 즉시 지급, 기간 한 번 지급)
GameEvent:AddReward( 578, { fEventTime = 0, iRewardID = 20096 } ) -- 하멜의 지원군 큐브

-- 하멜 전야 이벤트(캐릭터 단위, 접속 즉시 지급, 기간 한 번 지급)
GameEvent:AddReward( 579, { fEventTime = 0, iRewardID = 20097 } ) -- 하멜의 용사 큐브

-- 하멜 전야 이벤트(캐릭터 단위, 접속 즉시 지급, 기간 한 번 지급)
GameEvent:AddReward( 580, { fEventTime = 0, iRewardID = 20098 } ) -- 하멜의 수호자 큐브

-- 하멜 전야 이벤트(캐릭터 단위, 접속 즉시 지급, 기간 한 번 지급)
GameEvent:AddReward( 581, { fEventTime = 0, iRewardID = 20099 } ) -- 하멜의 절대자 큐브

-- 유로 2012 이벤트(캐릭터 단위, 접속 즉시 지급, 기간 내 한 번 지급)
GameEvent:AddReward( 582, { fEventTime = 0, iRewardID = 20100, bGiveOnce = True } ) -- 축구공 액세서리(1일)

-- 유로 2012 우승팀 맞추기 이벤트(계정 단위, 30분)
GameEvent:AddReward( 583, { fEventTime = 30, iRewardID = 20101, bAccountEvent = True } ) -- 우승팀 투표권(1일)

-- 하멜을 지원하라!. (칭호 보상) 칭호 우편 지급 (30일)
GameEvent:AddReward( 584, { fEventTime = 0, iRewardID = 20102 } ) -- 모든 캐릭터, 30일

-- 청 전야 이벤트 : 새로운 영웅의 상징!
GameEvent:AddReward( 585, { fEventTime = 30, iRewardID = 20103, bAccountEvent = True } ) -- 계정 단위 / 30분 접속시 / 아이템 우편 지급

-- 청 이벤트 : 새로운 영웅 등장!
GameEvent:AddReward( 586, { fEventTime = 10, iRewardID = 20104, bAccountEvent = True, bComeBackEvent = True } ) -- 돌아온 모험가의 큐브
GameEvent:AddReward( 587, { fEventTime = 10, iRewardID = 20105, bAccountEvent = True, bNotComeBackEvent = True } ) -- 코보 VIP 모험가 큐브
GameEvent:AddReward( 588, { fEventTime = 0, iRewardID = 20106, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )	-- 하멜의 하얀 늑대 칭호
GameEvent:AddReward( 589, { fEventTime = 0, iRewardID = 20106, cUnitType = UNIT_TYPE["UT_LIRE"] } )		-- 하멜의 하얀 늑대 칭호
GameEvent:AddReward( 590, { fEventTime = 0, iRewardID = 20106, cUnitType = UNIT_TYPE["UT_ARME"] } )		-- 하멜의 하얀 늑대 칭호
GameEvent:AddReward( 591, { fEventTime = 0, iRewardID = 20106, cUnitType = UNIT_TYPE["UT_RAVEN"] } )	-- 하멜의 하얀 늑대 칭호
GameEvent:AddReward( 592, { fEventTime = 0, iRewardID = 20106, cUnitType = UNIT_TYPE["UT_EVE"] } )		-- 하멜의 하얀 늑대 칭호

-- 2012.07.25 카밀라의 기술 향상 큐브 지급 이벤트(캐릭터 단위, 접속 즉시, 청 캐릭터 제한)
GameEvent:AddReward( 593, { fEventTime = 0, iRewardID = 20107, cUnitType = UNIT_TYPE["UT_CHUNG"], bGiveOnce = True } ) -- 카밀라의 기술 향상 큐브(기초)
GameEvent:AddReward( 594, { fEventTime = 0, iRewardID = 20108, cUnitType = UNIT_TYPE["UT_CHUNG"], bGiveOnce = True } ) -- 카밀라의 기술 향상 큐브(중급)
GameEvent:AddReward( 595, { fEventTime = 0, iRewardID = 20109, cUnitType = UNIT_TYPE["UT_CHUNG"], bGiveOnce = True } ) -- 카밀라의 기술 향상 큐브(상급)

-- 2012.07.25 카밀라의 수상한 부적 지급 이벤트(캐릭터 단위, 접속 즉시)
GameEvent:AddReward( 596, { fEventTime = 0, iRewardID = 20113 } ) -- 카밀라의 수상한 부적(1일권)

-- 2012.07.25 치명타의 발견 7개 지급 이벤트(캐릭터 단위, 접속 즉시)
GameEvent:AddReward( 597, { fEventTime = 0, iRewardID = 20114 } ) -- 치명타의 발견 7개

-- 2012.08.01 올림픽 개막식 기념 칭호 아이템 지급 이벤트(캐릭터 단위, 1분, 하루 지급)
GameEvent:AddReward( 598, { fEventTime = 1, iRewardID = 20120 } )

-- 2012.08.01 올림픽 개막식 기념 경험치 2배 이벤트
GameEvent:AddReward( 599, { fEXPRate = 1.0 } )

-- 2012.08.01 올림픽 개막식 기념 드롭률 2배 이벤트
GameEvent:AddReward( 600, { iDropCount =  2 } )

-- 2012.08.01 올림픽 기념 플루오르 스톤 LV.1 및 축복받은 복원의 주문서 LV.1 지급 이벤트(계정 단위, 10분, 매일 지급)
GameEvent:AddReward( 601, { fEventTime = 10, iRewardID = 20121, bAccountEvent = True } )

-- 2012.08.01 올림픽 폐막식 기념 도전자의 상징과 아리엘의 가열기 지급 이벤트(계정 단위, 120분, 하루 지급)
GameEvent:AddReward( 602, { fEventTime = 120, iRewardID = 20122, bAccountEvent = True } )


-- 2012.08.08 45레벨 이상 50레벨 이하 경험치 1.2배 이벤트
GameEvent:AddReward( 603, { fEXPRate = 0.2, iMinLevel = 45, iMaxLevel = 50 } )

-- 2012.08.08 평일 파수병의 열쇠 지급 이벤트(캐릭터 단위, 30분, 월,화,수,목,금요일 지급)
GameEvent:AddReward( 604, { fEventTime = 30, iRewardID = 20123 } )

-- 2012.08.08 주말 파수병의 열쇠 지급 이벤트(캐릭터 단위, 30분, 토,일요일 지급)
GameEvent:AddReward( 605, { fEventTime = 30, iRewardID = 20124 } )


-- 2012.08.22 35레벨 이하 청 캐릭터 경험치 50% 추가 획득
GameEvent:AddReward( 606, { fEXPRate = 0.5, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_CHUNG"] } )

-- 2012.08.22 청 2차 전직 기념 성장의 비결(인내의 상징) 1개 우편 지급 이벤트(캐릭터 단위, 60분, 매일 지급)
GameEvent:AddReward( 607, { fEventTime = 60, iRewardID = 20125, cUnitType = UNIT_TYPE["UT_CHUNG"] } )

-- 2012.08.29 엘의 여인의 눈물 5개 우편 지급 이벤트(한 번/계정 단위/30분)
GameEvent:AddReward( 608, { fEventTime = 30, iRewardID = 20127, bAccountEvent = True, bGiveOnce = True } )

-- 2012.08.29 세나스 공국의 분노 1개 우편 지급 이벤트(매일/계정 단위/30분)
GameEvent:AddReward( 609, { fEventTime = 30, iRewardID = 20128, bAccountEvent = True } )


-- 2012.09.12 다시 떠오르는 영웅들 이벤트(한 번/캐릭터 단위/10분)
GameEvent:AddReward( 610, { fEventTime = 10, iRewardID = 20129, bGiveOnce = True } )


-- 2012.09.26 평일 비밀의 정원 (2012-09-26 ~ 2012-10-09)이벤트(캐릭터 단위, 10레벨이상, 접속즉시, 월,화,수,목,금요일 3개 지급)
GameEvent:AddReward( 611, { fEventTime = 0, iMinLevel = 10, iRewardID = 20131 } )

-- 2012.09.26 주말 비밀의 정원 (2012-09-26 ~ 2012-10-09)이벤트(캐릭터 단위, 10레벨이상, 접속즉시 , 토,일요일 5개 지급)
GameEvent:AddReward( 612, { fEventTime = 0, iMinLevel = 10, iRewardID = 20132 } )

-- 2012.09.26 ESL 대전대회 기념 큐브를 드려요! (2012-10-01 ~ 2012-11-04) 이벤트(캐릭터 단위, 5분 , ESL 대전 아이템 큐브)
GameEvent:AddReward( 613, { fEventTime = 5, iRewardID = 20133 } )

-- 2012.09.26 ESL 대전대회 기념 큐브를 드려요! (2012-10-01 ~ 2012-11-04) 이벤트(캐릭터 단위, 5분 , ESL 대전 아이템 큐브 열쇠 )
GameEvent:AddReward( 614, { fEventTime = 5, iRewardID = 20134 } )


-- 2012.10.10 주말 헤니르 보상 횟수 +2 이벤트
GameEvent:AddReward( 615, { iHenirRewardEventCount = 2 } )

-- 2012.10.10 펫 이벤트 (계정 단위, 150분/30분)
GameEvent:AddReward( 616, { fEventTime =  150, iRewardID = 20135, bAccountEvent = True } )  -- 덫에 걸린 해츨링(유체)
GameEvent:AddReward( 617, { fEventTime =  30, iRewardID = 20136, bAccountEvent = True } )  -- 카밀라의 데일리 펫 훈련 매거진

-- 2012.10.10 펫 훈련장 입장권 우편 지급 이벤트(캐릭터 단위, 60분)
GameEvent:AddReward( 618, { fEventTime = 60, iRewardID = 20137 } )	-- 펫 훈련장 입장권 3개
GameEvent:AddReward( 619, { fEventTime = 60, iRewardID = 20138 } )	-- 펫 훈련장 입장권 5개


-- 2012.10.24 할로윈 이벤트
GameEvent:AddReward( 620, { fEventTime = 10, iRewardID = 20139 } )	-- 데스사이즈-조화(1일권)
GameEvent:AddReward( 621, { fEventTime = 10, iRewardID = 20140 } )	-- 데스사이즈-신비(1일권)
GameEvent:AddReward( 622, { fEventTime = 10, iRewardID = 20141 } )	-- 데스사이즈-광휘(1일권)
GameEvent:AddReward( 623, { fEventTime = 10, iRewardID = 20142 } )	-- 데스사이즈 1일권 6종 큐브
GameEvent:AddReward( 624, { fEventTime = 10, iRewardID = 20143 } )	-- 데스사이즈-전투(1일권)
GameEvent:AddReward( 625, { fEventTime = 10, iRewardID = 20144 } )	-- 데스사이즈-증강(1일권)
GameEvent:AddReward( 626, { fEventTime = 10, iRewardID = 20145 } )	-- 데스사이즈-적중(1일권)
GameEvent:AddReward( 627, { fEventTime = 10, iRewardID = 20146 } )	-- 아리엘의 할로윈 사탕 1개(평일)
GameEvent:AddReward( 628, { fEventTime = 10, iRewardID = 20147 } )	-- 아리엘의 할로윈 사탕 2개(주말)


-- 2012.11.07 검의 길 이벤트 1.활력vs파괴 1) 검사의 선택 (한 번, 캐릭터 단위, 10분 접속)
GameEvent:AddReward( 629, { fEventTime = 10, iRewardID = 20148, bGiveOnce = True } )	-- 활력의 검사 칭호 (15일) 
GameEvent:AddReward( 630, { fEventTime = 10, iRewardID = 20149, bGiveOnce = True } )	-- 파괴의 검사 칭호 (15일) 
GameEvent:AddReward( 631, { fEventTime = 10, iRewardID = 20150, bGiveOnce = True } )	-- 활력의 룬 (15일) 
GameEvent:AddReward( 632, { fEventTime = 10, iRewardID = 20151, bGiveOnce = True } )	-- 파괴의 른 (15일) 

-- 2012.11.07 검의 길 이벤트 1.활력vs파괴 3) 검사의 깨달음 (엘소드, 120분 접속)
GameEvent:AddReward( 633, { fEventTime = 120, iRewardID = 20152, cUnitType = UNIT_TYPE["UT_ELSWORD"]} )	-- 검사의 깨달음


-- 2012.11.21  마족의 흔적 (2012-11-21 ~ 2012-12-05) 30분 접속 시 ‘마족의 흔적 1개’ 지급 (캐릭터 마다)
GameEvent:AddReward( 634, { fEventTime = 30, iRewardID = 20153 } )	-- 마족의 흔적


-- 2012.12.05  The Rage! (2012-11-21 ~ 2012-12-05) 10분 접속 시 (캐릭터 마다)
GameEvent:AddReward( 635, { fEventTime = 10, iRewardID = 20154 } )	-- The Rage! (15일권), 아이템분노의 반지(15일)

-- 2012.12.05  새로운 검사의 상징 (2012-12-12 ~ 2012-12-18) (한 번/캐릭터 단위/10분)
GameEvent:AddReward( 636, { fEventTime = 10, iRewardID = 20155, bGiveOnce = True } )	-- 콘웰의 현신(7일권)


-- 2012.12.18 수습 모험가 “엘소드” (2012-12-18 ~ 2013-01-02) ( 한번 / 캐릭터단위 / 접속 즉시 / 엘소드 유저 / 신규유저)bNewUserEvent bNewUnitEvent
GameEvent:AddReward( 637, { fEventTime = 0, iRewardID = 20156, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"], bNewUnitEvent = True } )	-- 신규 엘소드 수습 모험가 기초 교육1 큐브
-- 2012.12.18 코보사의 정규 모험가 재교육 프로그램 (2012-12-18 ~ 2013-01-02) ( 한번 / 캐릭터 단위 / 접속 즉시 / 엘소드 유저 / 기존유저(신규,복귀 유저 제외)) bCurrentUserEvent bCurrentUnitEvent
GameEvent:AddReward( 638, { fEventTime = 0, iRewardID = 20157, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"], iMinLevel = 21, iMaxLevel = 30, bCurrentUnitEvent = True} )	-- 21~30레벨	모험가 기초 교육 수강권
GameEvent:AddReward( 638, { fEventTime = 0, iRewardID = 20158, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"], iMinLevel = 31, iMaxLevel = 40, bCurrentUnitEvent = True} )	-- 31~40레벨	모험가 실무 교육 수강권
GameEvent:AddReward( 638, { fEventTime = 0, iRewardID = 20159, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"], iMinLevel = 41, iMaxLevel = 50, bCurrentUnitEvent = True} )	-- 41~50레벨	모험가 2차 직무 교육 수강권
GameEvent:AddReward( 638, { fEventTime = 0, iRewardID = 20160, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"], iMinLevel = 51, iMaxLevel = 60, bCurrentUnitEvent = True} )	-- 51~60레벨	모험가 승급 교육 수강권

-- 2012.12.18 크리스마스 특별 선물 ( 근성도 무제한, 경험치 2배, 드롭율 2배 ) - 12/22
-- 2012.12.18 크리스마스 특별 선물 ( 한번 / 계정 단위 / 120분 / 최소레벨 10 )
GameEvent:AddReward( 639, { fEventTime = 120, iRewardID = 20161, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10 } )	-- 12/23 계정당 1회 120분 접속시 아리엘의 가열기 1개
GameEvent:AddReward( 640, { fEventTime = 120, iRewardID = 20162, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10 } )	-- 12/24 계정당 1회 120분 접속시 플루오스 스톤 교환권 지급
GameEvent:AddReward( 641, { fEventTime = 120, iRewardID = 20163, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10 } )	-- 12/25 계정당 1회 120분 접속시 크리스마스 지팡이 사탕 장식 액세서리 영구권 지급

-- 2012.12.18 모험가의 귀환 ( 한번 / 계정 단위 / 30분 / 복귀자)
GameEvent:AddReward( 642, { fEventTime = 30, iRewardID = 20164, bAccountEvent = True, bGiveOnce = True, bComeBackEvent = True } )	-- 돌아온 모험가의 큐브

-- 2012.12.18 2012년 못 다 이룬 연초 결심(12월 26일 ~ 12월 31일)  ( 한번 / 캐릭터 단위 / 100분 / 최소레벨 10)
GameEvent:AddReward( 643, { fEventTime = 100, iRewardID = 20165, bGiveOnce = True, iMinLevel = 10 } )	-- ‘되찾은 연초의 결심’
-- 2012.12.18 새해맞이 특별 이벤트 ( 12-31 ~ 1-1 ) 근성도 무제한, 경험치 2배, 드롭율 2배

-- 2012.12.18 마법소녀의 날개 (2013-01-09 ~ 2013-01-15)  ( 한번 / 캐릭터 단위 / 10분 / 최소레벨 10)
GameEvent:AddReward( 644, { fEventTime = 10, iRewardID = 20166, bGiveOnce = True, iMinLevel = 10 } )	-- ‘마법소녀의 소망(7일)’

-- 2012.12.18 매일 매리 크리스마스 캐릭터 단위 / 50분 / 최소레벨 10 / 직접)
GameEvent:AddReward( 645, { fEventTime =  50, iRewardID = 645, bDirectReward = True, iMinLevel = 10 } )	-- 크리스마스 트리 이벤트


-- 2013.01.16 배틀매지션 접속이벤트 (30분 접속/1회/캐릭터)
GameEvent:AddReward( 646, { fEventTime =  30, iRewardID = 20167, bGiveOnce = True } ) -- 마법소녀의 희망

-- 2013.01.17 배틀매지션 마법소녀의 수련장 입장권 (10분 접속/캐릭터)
GameEvent:AddReward( 647, { fEventTime =  10, iRewardID = 20168, iMinLevel = 10 } )	-- 마법소녀의 수련장 입장권 / 평일 3장

-- 2013.01.18 배틀매지션 마법소녀의 수련장 입장권 (10분 접속/캐릭터)
GameEvent:AddReward( 648, { fEventTime =  10, iRewardID = 20169, iMinLevel = 10 } )	-- 마법소녀의 수련장 입장권 / 주말 5장


-- 2013.01.18 태그 모드 이벤트 : 태그 모드 지원1 (10분 접속/캐릭터)
GameEvent:AddReward( 649, { fEventTime =  10, iRewardID = 20171 } )	-- 마법의 목걸이(1일권), 기술의 반지 4종(1일권) 큐브

-- 2013.01.18 태그 모드 이벤트 : 태그 모드 지원2 (30분 접속/캐릭터)
GameEvent:AddReward( 650, { fEventTime =  30, iRewardID = 20172 } )	-- 태그 모드 프로모션 무기 아바타 3종(1일권) 큐브

-- 2013.01.18 트래핑 레인저 전야 이벤트 : 접속 이벤트 (30분 접속/캐릭터)
GameEvent:AddReward( 651, { fEventTime =  30, iRewardID = 20173, iMinLevel = 10  } )	--  “영웅에 대한 소식지” 우편 지급 

-- 2013.01.18 트래핑 레인저 전야 이벤트 : 주말 접속 이벤트 (한번 / 캐릭터 단위 /주말 60분 접속/캐릭터)
GameEvent:AddReward( 652, { fEventTime =  60, iRewardID = 20174,bGiveOnce = True, iMinLevel = 10  } )	--  엘드랏실의 축복 우편 지급. 

-- 2013.02.13 비밀스런 바람의 소리_바람의 암살자 트레핑 레인저(한번 / 캐릭터 단위 / 10분접속 /캐릭터)
GameEvent:AddReward( 653, { fEventTime =  10, iRewardID = 20176, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_LIRE_TRAPPING_RANGER"] } )	--  엘드랏실의 신성한큐브 

-- 2013.02.27 미션 던전 이벤트 : 접속 이벤트 (한번 / 캐릭터 단위 / 30분접속 /캐릭터)
GameEvent:AddReward( 654, { fEventTime =  30, iRewardID = 20177, bGiveOnce = True, iMinLevel = 10  } )	--  메모리 카드A 지급
GameEvent:AddReward( 655, { fEventTime =  30, iRewardID = 20178, bGiveOnce = True, iMinLevel = 10  } )	--  메모리 카드B 지급
GameEvent:AddReward( 656, { fEventTime =  30, iRewardID = 20179, bGiveOnce = True, iMinLevel = 10  } )	--  메모리 카드C 지급
GameEvent:AddReward( 657, { fEventTime =  30, iRewardID = 20180, bGiveOnce = True, iMinLevel = 10  } )	--  메모리 카드D 지급
GameEvent:AddReward( 658, { fEventTime =  30, iRewardID = 20181, bGiveOnce = True, iMinLevel = 10  } )	--  메모리 카드E 지급
-- 2013.02.27 미션 던전 이벤트 : 접속 이벤트 (한번 / 캐릭터 단위 / 60분접속 /캐릭터)
GameEvent:AddReward( 659, { fEventTime =  60, iRewardID = 20182, bGiveOnce = True, iMinLevel = 10  } )	--  메모리 카드F 지급
-- 2013.02.27 미션 던전 이벤트 : 접속 이벤트 (한번 / 캐릭터 단위 / 평일40분접속 /캐릭터)
GameEvent:AddReward( 660, { fEventTime =  40, iRewardID = 20183, bGiveOnce = True, iMinLevel = 10  } )	--  보호 부적 평일 1장 40분
-- 2013.02.27 미션 던전 이벤트 : 접속 이벤트 (한번 / 캐릭터 단위 / 주말70분접속 /캐릭터)
GameEvent:AddReward( 661, { fEventTime =  70, iRewardID = 20184, bGiveOnce = True, iMinLevel = 10  } )	--  보호 부적 주말 2장 70분
-- 2013.03.06 웨폰테이커 전야 이벤트 : 접속 이벤트 (한번 / 캐릭터 단위 / 10분접속 /캐릭터/레벨제한 15)
GameEvent:AddReward( 662, { fEventTime =  10, iRewardID = 20185, bGiveOnce = True  } )	--  건블레이드
GameEvent:AddReward( 663, { fEventTime =  10, iRewardID = 20186, bGiveOnce = True, iMinLevel = 15   } )	--  코보 용병단 스타트업 큐브 열쇠

-- 길드원이 필요하다면? (2013-02-27 ~ 2013-03-13)
GameEvent:AddReward( 664, { fEventTime = 10, iRewardID = 20187, bCustomEvent = True, } ) -- 10분,2월 27일 이전 길드에 가입되어 있지 않던 캐릭터 길드 가입 프로모션 쿠폰
GameEvent:AddReward( 665, { fEventTime = 10, iRewardID = 20188, bNewUnitEvent = True, } ) -- 10분, 2월 28일 업데이트 이후 생성 캐릭터 길드 가입 프로모션 쿠폰

-- 미스 엘리오스 2013-02-27 ~ 2013-03-13
GameEvent:AddReward( 666, { fEventTime = 30, iRewardID = 20189 } ) -- 30분 접속 시 경험치 메달 30% 1일권(ItemID 200720) 지급(캐릭터)
GameEvent:AddReward( 667, { fEventTime = 60, iRewardID = 20190 } ) -- 60분 접속시  아리엘의 플루오르 스톤 Lv3 ~ 5(ItemID 67003746) 지급(캐릭터)
GameEvent:AddReward( 668, { fEventTime = 60, iRewardID = 20191 } ) -- 60분 접속시  듀얼 마법석 랜덤 큐브(ItemID 67003744 ) 지급(캐릭터) 
GameEvent:AddReward( 669, { fEventTime = 60, iRewardID = 20192 } ) -- 2013-03-12 60분   접속 시 승리의 왕관 14일권 큐브 우편 지급

-- 진정한 지휘관의 귀환 (2013-03-13 ~ 2013-03-27) 
-- 레이븐 레벨 15이하 유저는 경험치 2배를 획득할 수 있음
GameEvent:AddReward( 670, { fEXPRate = 1.0, iUnitLevel = 15, cUnitType = UNIT_TYPE["UT_RAVEN"], } )
-- 모든 노전직 레이븐 캐릭터로 접속 시 무기 큐브 증정
GameEvent:AddReward( 671, { fEventTime = 30, iRewardID = 20193, cUnitClass = UNIT_CLASS["UC_RAVEN_FIGHTER"], } ) -- 돌아온 지휘관의 큐브 1
--고대 나소드의 신성한 코드 획득(1분)
GameEvent:AddReward( 672, { fEventTime = 1, iRewardID = 20194, bGiveOnce = True, } ) --나소드 분광석(기간제)
--고대 나소드의 신성한 코드 해석(30분)
GameEvent:AddReward( 673, { fEventTime = 30, iRewardID = 20195, bGiveOnce = True, } ) --코드 분석 회로
--5회 : 다음날 60분 접속 시 정제된 엘의 조각(마스터) 랜덤 큐브 1개 우편 지급(캐릭단위)
GameEvent:AddReward( 674, { fEventTime = 60, iRewardID = 20196, bGiveOnce = True, } ) --정제된 엘의 조각 랜덤 큐브
--6회 : 다음날 60분 접속 시 뽀루의 발도장(하양,노랑,보라) 랜덤 큐브 1개 우편 지급(캐릭단위)
GameEvent:AddReward( 675, { fEventTime = 60, iRewardID = 20197, bGiveOnce = True, } ) --2013 미스 엘리오스 공약 뽀루의 발도장 랜덤 큐브

--1회 : 다음날 오후 7시 ~ 8시 까지 1시간동안 경험치 2배
GameEvent:AddReward( 676, { fEXPRate = 1.0 } )

--4회 : 다음날 오후6시~8시까지 2시간동안 아이템 드랍률 2배
GameEvent:AddReward( 677, { iDropCount =  2 } )

--모든 40레벨 이상 이브 캐릭터들에게 매일 30분 접속시 
GameEvent:AddReward( 678, { fEventTime = 30, iRewardID = 20207, bGiveOnce = True, iMinLevel = 40, cUnitType = UNIT_TYPE["UT_EVE"],  } ) -- 엘 에너지 캡슐

--(0424)몬스터 카드 접속 이벤트 
--몬스터카드 수집가의 모자 (한번 / 캐릭터 단위 / 10분접속 / 1이상)
GameEvent:AddReward( 679, { fEventTime = 10, iRewardID = 20211, bGiveOnce = True, } ) -- 몬스터 카드 수집가의 모자(30일) 큐브
--의문의 몬스터 카드 (매일 / 캐릭터 단위 / 10분접속 /캐릭터)
GameEvent:AddReward( 680, { fEventTime = 10, iRewardID = 20210, } ) -- 의문의 몬스터 카드 5개

--필드 접속 이벤트 
--필드 모험 준비 패키지 (한번 / 계정 단위 / 30분접속 /10이상)
GameEvent:AddReward( 681, { fEventTime = 30, iRewardID = 20217, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10, } ) 
--필드 100% 즐기기 필수 가이드북 (한번 / 캐릭터 단위 / 10분접속 /10이상)
GameEvent:AddReward( 682, { fEventTime = 10, iRewardID = 20218, bGiveOnce = True, iMinLevel = 10, } ) 

---------------------------------------------------------------------------------------------------
-- 2013.04.24 광고판등록
GameEvent:AddAdvertisement( 1, { URL = "http://de.elsword.org/popup" } )
GameEvent:AddAdvertisement( 2, { URL = "http://fr.elsword.org/popup" } )
GameEvent:AddAdvertisement( 3, { URL = "http://it.elsword.org/popup" } )
GameEvent:AddAdvertisement( 4, { URL = "http://pl.elsword.org/popup" } )
GameEvent:AddAdvertisement( 5, { URL = "http://es.elsword.org/popup" } )
GameEvent:AddAdvertisement( 6, { URL = "http://www.elswordonline.com/ingameinfo/" } )

--------------------------------------------------------------------------
-- 2013.06.04 ESeu_필드 이벤트 85002462 황금 필드 쿠키
GameEvent:AddReward( 683, { fEventTime = 10, iRewardID = 20220, bGiveOnce = True, iMinLevel = 10 } ) -- 10분 캐릭터 접속 시 평일 Lv10, 필드 쿠키
GameEvent:AddReward( 684, { fEventTime = 60, iRewardID = 20221, bGiveOnce = True, iMinLevel = 10 } ) -- 60분 캐릭터 접속 시 주말 Lv10, 황금 필드 쿠키

--------------------------------------------------------------------------
-- 2013.06.19 ESeu_나이트 와쳐 전야  85002560 풍고족의 서신
GameEvent:AddReward( 685, { fEventTime = 30, iRewardID = 20222, bGiveOnce = True, iMinLevel = 10 } ) -- 10분 캐릭터 접속 시 평일 Lv10, 풍고족의 서신

--* 제목 : 인피니티 소드의 등장
--* 이벤트 기간 : 2013년 6월 19일 ~ 2013년 6월 25일 (1주일)
GameEvent:AddReward( 686, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ELSWORD_SHEATH_KNIGHT"], } ) -- 35레벨 이하 시스나이트는 경험치 2배 획득
GameEvent:AddReward( 687, { fEventTime =  30, iRewardID = 20223, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_ELSWORD_SHEATH_KNIGHT"], } ) -- 인피니티 소드 준비 큐브
GameEvent:AddReward( 688, { fEventTime =  60, iRewardID = 20224, bGiveOnce = True, } ) -- 미제련된 무한의 콘웰

--ESEU_컴백 이벤트&인피니티소드 이벤트 (2013-06-26 ~ 2013-07-10)
-- 복귀유저
GameEvent:AddReward( 689, { fEventTime =  30, iRewardID = 20225, bGiveOnce = True, bAccountEvent = True, iMinLevel = 10, iCustomEventID = 1} ) -- 기본 아바타 무기(영구) 랜덤 큐브, 돌아온 영웅의 칭호 (15일권)
GameEvent:AddReward( 690, { fEventTime =  10, iRewardID = 20226, bGiveOnce = True, bAccountEvent = True, iMinLevel = 10, iCustomEventID = 1} ) -- 돌아온 영웅의 증표 1개
-- 기존, 신규유저
GameEvent:AddReward( 691, { fEventTime =  10, iRewardID = 20227, bGiveOnce = True, bAccountEvent = True, iMinLevel = 10, iCustomEventID = 2} ) -- 성실한 모험가의 증표 1개 성실한 모험가의 칭호 (1일권)
-- 주말 동안 매일 모든 유저 60분 접속 시 돌아온 영웅의 코인 우편 보상 획득
GameEvent:AddReward( 692, { fEventTime =  60, iRewardID = 20228, bGiveOnce = True, bAccountEvent = True, } ) -- 영웅의 코인 1개
-- 무한의 큐브 (2013-06-26 ~ 2013-07-09)
GameEvent:AddReward( 693, { fEventTime =  10, iRewardID = 20229, bGiveOnce = True, } ) -- 미약한 무한의 큐브 1개
GameEvent:AddReward( 694, { fEventTime =  10, iRewardID = 20230, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_ELSWORD_SHEATH_KNIGHT"],} ) -- 무한의 큐브 1개 다크엘의 결정 1개
GameEvent:AddReward( 695, { fEventTime =  10, iRewardID = 20231, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_ELSWORD_INFINITY_SWORD"],} ) -- 진화된 무한의 큐브 1개

--##############################################################################--
--ESEU_영국 obt 이벤트 85002891
--매일 60분 접속 시 우편으로 보상아이템 지급 – 중간접속 종료 시 다시 카운트 
GameEvent:AddReward( 696, { fEventTime =  60, iRewardID = 20232, bGiveOnce = True, bAccountEvent = True, } ) -- 엘소드 영국 OBT 기념 큐브 계정단위
GameEvent:AddReward( 697, { fEventTime =  10, iRewardID = 20233, bGiveOnce = True, } ) -- 엘리오스 조사단 아바타 풀세트 큐브(15일권)
--##############################################################################--
-- 아리엘의 감사 보상(2013-07-31 ~ 2013-08-07)(계정)
GameEvent:AddReward( 698, { fEventTime =  1, iRewardID = 20235, bGiveOnce = True, bAccountEvent = True,  cUnitClass = UNIT_CLASS["UC_ARME_DIMENSION_WITCH"], } ) -- 망각의 드링크(7일권) 큐브 1개

-- 디멘션 위치 전직을 준비하자.  2013년 07월 24일 ~ 2013년 07월 31일 
GameEvent:AddReward( 699, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ARME_BATTLE_MAGICIAN"], } ) -- 35레벨 이하 배틀매지션는 경험치 2배 획득
GameEvent:AddReward( 700, { fEventTime =  30, iRewardID = 20234, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_ARME_BATTLE_MAGICIAN"], } ) -- 디멘션 위치 준비 큐브

--나이트와쳐 이벤트
GameEvent:AddReward( 701, { fEventTime = 1, iRewardID = 20236, bGiveOnce = True, iEventItemID = 85002576 } )	--강렬한 에렌딜의 영혼 큐브 열쇠 
GameEvent:AddReward( 702, { fEventTime = 1, iRewardID = 20237, bGiveOnce = True, iEventItemID = 85002577 } )	--온유한 에렌딜의 영혼 큐브 열쇠
GameEvent:AddReward( 703, { fEventTime = 1, iRewardID = 20238, bGiveOnce = True, iEventItemID = 85002578 } )	--내유외강 에렌딜의 영혼 큐브 열쇠

GameEvent:AddReward( 704, { fEventTime = 1, iRewardID = 20239, bGiveOnce = True } )		--킹 나소드 소굴 입장 티켓
GameEvent:AddReward( 705, { fEventTime = 20, iRewardID = 20240, bGiveOnce = True } )	--아기 퐁고의 메아리
GameEvent:AddReward( 706, { fEventTime = 40, iRewardID = 20240, bGiveOnce = True } )	--아기 퐁고의 메아리2
GameEvent:AddReward( 707, { fEventTime = 60, iRewardID = 20240, bGiveOnce = True } )	--아기 퐁고의 메아리3
GameEvent:AddReward( 708, { fEventTime = 10, iRewardID = 20241, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_LIRE_NIGHT_WATCHER"] } )	--나소드의 비밀 열쇠
GameEvent:AddReward( 709, { fEventTime = 30, iRewardID = 20242, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_LIRE_NIGHT_WATCHER"] } )	--킹 나소드의 비밀 열쇠

--2013.08.07 EU 중보, 배커 전야 , 배커
GameEvent:AddReward( 710, { fEventTime =  1, iRewardID = 20243, bGiveOnce = True, } ) --카메라 ES-888(블랙)
GameEvent:AddReward( 711, { fEventTime =  30, iRewardID = 20244, bGiveOnce = True, } ) -- 대박 필드 보스 큐브
GameEvent:AddReward( 712, { fEventTime =  30, iRewardID = 20245, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_RAVEN_WEAPON_TAKER"]} ) -- 베테랑 커맨더 준비 큐브
GameEvent:AddReward( 713, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_RAVEN_WEAPON_TAKER"], } ) -- 35레벨 이하 웨폰 테이커 는 경험치 2배 획득
---------------------------------------------------------------------------------------------------
--2013.09.04 EU 배세, 택트
-- 코드 배틀세라프 전야
GameEvent:AddReward( 714, { fEventTime =  30, iRewardID = 20248, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_EVE_ELECTRA"]} ) -- 코드 배틀세라프 준비 큐브
GameEvent:AddReward( 715, { fEventTime =  30, iRewardID = 20249, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_EVE_ELECTRA"]} ) -- 세라프 코드 랜덤 큐브
GameEvent:AddReward( 728, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_EVE_ELECTRA"], } ) -- 35레벨 이하 코드일렉트라 는 경험치 2배 획득

GameEvent:AddReward( 716, { fEventTime =  20, iRewardID = 20250, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  A
GameEvent:AddReward( 717, { fEventTime =  40, iRewardID = 20251, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  B
GameEvent:AddReward( 718, { fEventTime =  60, iRewardID = 20252, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  C
GameEvent:AddReward( 719, { fEventTime =  80, iRewardID = 20253, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  D
GameEvent:AddReward( 720, { fEventTime =  100, iRewardID = 20250, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  A
GameEvent:AddReward( 721, { fEventTime =  120, iRewardID = 20251, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  B
GameEvent:AddReward( 722, { fEventTime =  140, iRewardID = 20252, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  C
GameEvent:AddReward( 723, { fEventTime =  160, iRewardID = 20253, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  D
-- 택틱컬 트루퍼 전야
GameEvent:AddReward( 724, { fEventTime =  30, iRewardID = 20254, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_CHUNG_SHELLING_GUARDIAN"]} ) -- 택티컬 트루퍼 준비 큐브
GameEvent:AddReward( 725, { fEventTime =  30, iRewardID = 20255, bGiveOnce = True, iMinLevel = 10, } ) -- 디스프로즌 에너지
GameEvent:AddReward( 727, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_CHUNG_SHELLING_GUARDIAN"], } ) -- 35레벨 이하 쉘링가디언은 경험치 2배 획득
-- 택틱컬 트루퍼
GameEvent:AddReward( 726, { fEventTime =  60, iRewardID = 20256, bGiveOnce = True, } ) -- 신규 2차전직 캐릭터 카드 랜덤 큐브
--할로윈 전야 이벤트 
GameEvent:AddReward( 729, { fEventTime =  30, iRewardID = 20257, bGiveOnce = True, } ) -- 신인 할로윈 디자어너 칭호(15일권)
GameEvent:AddReward( 730, { fEventTime =  30, iRewardID = 20258, bGiveOnce = True, } ) -- 전설의 할로윈 디자이너 칭호(15일권)
--13일의 금요일
GameEvent:AddReward( 731, { fEventTime =  1, iRewardID = 20259, bGiveOnce = True, } ) -- 13일의 금요일 제이슨 마스크(3일권)
---------------------------------------------------------------------------------------------------
--2013.10.16 EU 할로윈 , 아이템 개편
--할오윈 연회 참석 준비 
GameEvent:AddReward( 732, { fEventTime =  10, iRewardID = 20264, bGiveOnce = True, } ) -- 할로 위치펫(15일권)
GameEvent:AddReward( 733, { fEventTime =  1, iRewardID = 20265, bGiveOnce = True, } ) -- 할로윈 몬스터 코스프레 교환권 0분
GameEvent:AddReward( 734, { fEventTime =  120, iRewardID = 20266, bGiveOnce = True, bAccountEvent = True, } ) -- 빛나는 할로윈 몬스터의 영혼 120분
--아이템 개편
GameEvent:AddReward( 735, { fEventTime = 100, iRewardID = 20262, bGiveOnce = True, } )  -- 루리엘의 축복받은 시공간의 주문서 교환권
GameEvent:AddReward( 736, { fEventTime = 100, iRewardID = 20263, bGiveOnce = True, } )  -- 루리엘의 축복받은 시공간의 주문서 연동레벨
GameEvent:AddReward( 737, { fEventTime = 60, iRewardID = 20267, bGiveOnce = True, bAccountEvent = True, } )  -- 아리엘의 플로오르 스톤 교환권
--공지 이벤트 날릴 센터서버UID
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_SOLES"],  1 ) -- 솔레스
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_GAIA"], 1 )  -- 가이아
---------------------------------------------------------------------------------------------------
-- 스킬트리 개편 이벤트 2013-11-20 정기 점검 후 ~ 2013-12-04 정기 점검 전
-- 복귀 유저와 성실 유저를 위한 선물
GameEvent:AddReward( 1565, { fEventTime = 10, iRewardID = 1505, bGiveOnce = True, bAccountEvent = True, bNewUnitEvent = True } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/10월16일이후 접속기록 없는 유저)
GameEvent:AddReward( 1566, { fEventTime = 10, iRewardID = 1506, bGiveOnce = True, bAccountEvent = True, bNewUnitEvent2 = True } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/10월16일이후 접속기록 있는 유저)
-- _복귀 유저를 위한 선물
-- 11/20, 11/27 강화를 하자!(1)
GameEvent:AddReward( 1567, { fEventTime = 10, iRewardID = 1507, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1568, { fEventTime = 10, iRewardID = 1508, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
-- 11/21, 11/28 강화를 하자!(2)
GameEvent:AddReward( 1569, { fEventTime = 10, iRewardID = 1509, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1570, { fEventTime = 10, iRewardID = 1510, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
-- 11/22, 11/29 강화를 하자!(3)
GameEvent:AddReward( 1571, { fEventTime = 10, iRewardID = 1511, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1572, { fEventTime = 10, iRewardID = 1512, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
-- 11/23, 11/30 감정을 해 보자!(1)
GameEvent:AddReward( 1573, { fEventTime = 10, iRewardID = 1513, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1574, { fEventTime = 10, iRewardID = 1514, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--11/24, 12/01 감정을 해 보자!(2)
GameEvent:AddReward( 1575, { fEventTime = 10, iRewardID = 1515, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1576, { fEventTime = 10, iRewardID = 1516, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--11/25, 12/02 감정을 해 보자!(3)
GameEvent:AddReward( 1577, { fEventTime = 10, iRewardID = 1517, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1578, { fEventTime = 10, iRewardID = 1518, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--11/26, 12/03 감정을 해 보자!(4)
GameEvent:AddReward( 1579, { fEventTime = 10, iRewardID = 1519, bGiveOnce = True, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
-- 성실 유저를 위한 선물
GameEvent:AddReward( 1580, { fEventTime = 10, iRewardID = 1520, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 1581, { fEventTime = 10, iRewardID = 1521, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 1582, { fEventTime = 10, iRewardID = 1522, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 1583, { fEventTime = 10, iRewardID = 1523, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 1584, { fEventTime = 10, iRewardID = 1524, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 1585, { fEventTime = 10, iRewardID = 1525, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 1586, { fEventTime = 10, iRewardID = 1526, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
-- 수련을 시작하자
GameEvent:AddReward( 1587, { fEventTime = 1, iRewardID = 1527, bGiveOnce = True } )  -- 수련용 반지 (한번/캐릭터/30분)
---------------------------------------------------------------------------------------------------
-- 2013.11.20 샌더 이벤트
GameEvent:AddReward( 1596, { fEventTime = 1, iRewardID = 1539, bGiveOnce = True, iMinLevel = 10 }) -- Lv10 이상 최초 접속 시, 샌더마을 관광 가이드북 1개 지급

-- 2013.11.20 아라전야이벤트 30분->3분, 60분->6분
GameEvent:AddReward( 1597, { fEventTime = 30, iRewardID = 1541, bGiveOnce = True, iMinLevel = 10 } ) -- Lv10 이상 30분 접속 시 아라 가문의 문서 보관함 우편 지급(캐릭터 단위)
GameEvent:AddReward( 1598, { fEventTime = 60, iRewardID = 1542, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10 } ) -- Lv10 이상 60분 접속 시 봉인된 구미호 은큐브 우편 지급(계정 단위)

-- 2013.12.04 어둠의 문 개편 이벤트
GameEvent:AddReward( 1602, { fEventTime = 30, iRewardID = 1552, bGiveOnce = True, } ) -- 새로운 어둠의 등장에 대한 지침서

-- 2013.08.07 ESJP_제천 이벤트
GameEvent:AddReward( 1603, { fEventTime = 0, iRewardID = 1555, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 접속시, 아라응원 선물 1개
GameEvent:AddReward( 1604, { fEventTime = 0, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1605, { fEventTime = 0, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_LIRE"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1606, { fEventTime = 0, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ARME"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1607, { fEventTime = 0, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_RAVEN"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1608, { fEventTime = 0, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_EVE"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1609, { fEventTime = 0, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_CHUNG"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개

-- 아라 전직 이벤트
GameEvent:AddReward( 1610, { fEXPRate = 0.5, iUnitLevel = 64, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 캐릭터 63렙 이하 경험치 1.5배

-- 아라 제천 이벤트
GameEvent:AddReward( 1611, { fEXPRate = 1.0, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 캐릭터 35렙 이하 경험치 2배

-- 아라 업데이트 기념 이벤트
GameEvent:AddReward( 1612, { fEventTime = 30, iRewardID = 1557 } )  -- 12월 25일 30분 접속 시, 축복 받은 무기 강화석 10개
GameEvent:AddReward( 1613, { fEventTime = 30, iRewardID = 1558 } )  -- 12월 26일 30분 접속 시, 축복 받은 방어구 강화석 20개
GameEvent:AddReward( 1614, { fEventTime = 30, iRewardID = 1559 } )  -- 12월 27일 30분 접속 시, 루리엘의 시공간의 주문서 교환권 1개
GameEvent:AddReward( 1615, { fEventTime = 30, iRewardID = 1560 } )  -- 12월 28일 30분 접속 시, 루리엘의 플루오르 스톤 교환권 1개
GameEvent:AddReward( 1616, { fEventTime = 30, iRewardID = 1561 } )  -- 12월 29일 30분 접속 시, 루리엘의 축복 받은 복원의 주문서 1개
GameEvent:AddReward( 1617, { fEventTime = 30, iRewardID = 1562 } )  -- 12월 30일 30분 접속 시, 빛 나는 푸른 구미호 꼬리 액세서리 2일권 큐브
GameEvent:AddReward( 1618, { fEventTime = 30, iRewardID = 1563 } )  -- 12월 31일 30분 접속 시, 빛 나는 붉은 구미호 꼬리 액세서리 2일권 큐브

--[[
-- 2013.12.04 은행 공유 이벤트
GameEvent:AddReward( 1619, { fEventTime = 10, iRewardID = 1564, bGiveOnce = True, iMinLevel = 10 } )-- 휴대용 강철금고 (15일권)
--]]

-- 2013 크리스마스
GameEvent:AddReward( 2000, { fEventTime = 25, iRewardID = 2000, bGiveOnce = True, } )  -- 폴라 베어 코스프레 세트(한벌)

-- 2013 연말
GameEvent:AddReward( 2001, { fEventTime = 30, iRewardID = 2001, bGiveOnce = True, bAccountEvent = True, } )  -- 2013년 마지막 접속 인증 티켓

-- 2014 새해
GameEvent:AddReward( 2014, { fEventTime = 30, iRewardID = 2015, bGiveOnce = True, bAccountEvent = True } )  -- 포니 헤드 액세서리(3일권)

-- 2014 탈 것 이벤트
GameEvent:AddReward( 2015, { fEventTime = 10, iRewardID = 2016, bGiveOnce = True, } )  -- 라이딩 체험 ( 돌진 전차 모비 RT (1일권) )
GameEvent:AddReward( 2016, { fEventTime = 10, iRewardID = 2017, bGiveOnce = True, } )  -- 라이딩 체험 ( 바람의 코우시카 (1일권) )
GameEvent:AddReward( 2017, { fEventTime = 10, iRewardID = 2018, bGiveOnce = True, } )  -- 라이딩 체험 ( 에이션트 라이딩 뽀루 (1일권) )
GameEvent:AddReward( 2018, { fEventTime = 10, iRewardID = 2019, bGiveOnce = True, } )  -- 라이딩 체험 ( 3종류 (1일권) )
GameEvent:AddReward( 2019, { fEventTime = 30, iRewardID = 2021, bAccountEvent = True, bGiveOnce = True, } )  -- 프로 라이더를 위한 특별 선물 (루리엘의 고대 화석 판별기)

-- 2014 샌더던전 3,4 이벤트
GameEvent:AddReward( 2020, { fEventTime = 1, iRewardID = 2022, bGiveOnce = True } )  -- 샌더 마을에서의 여정(샌더 액세서리 큐브)

-- 2014 소마 이벤트
GameEvent:AddReward( 2021, { fEXPRate = 1.0, iUnitLevel = 15, cUnitClass = UNIT_CLASS["UC_ARA_MARTIAL_ARTIST"] } )	-- 15레벨 이하 아라 캐릭터 경험치 2배

GameEvent:AddReward( 2022, { fEventTime = 10, iRewardID = 2023, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_ARA_MARTIAL_ARTIST"] } )	-- 노전직 아라 캐릭터 '거침없는 그녀' 큐브 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2023, { fEventTime = 10, iRewardID = 2024, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_ARA_LITTLE_DEVIL"] } )	-- 소마 캐릭터 '거침없는 소마' 큐브 우편 지급(매일/캐릭터/10분)

GameEvent:AddReward( 2024, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )	-- 엘소드 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2025, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ARME"] } )	-- 아이샤 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2026, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_LIRE"] } )	-- 레나 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2027, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_RAVEN"] } )	-- 레이븐 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2028, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_EVE"] } )	-- 이브 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2029, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_CHUNG"] } )	-- 청 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2030, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_ARA_LITTLE_HSIEN"] } )	-- 아라 소선 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2031, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_ARA_SAKRA_DEVANAM"] } ) -- 아라 제천 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)

-- 2014 아라2차전직(명왕) 이벤트
GameEvent:AddReward( 2032, { fEXPRate = 0.5, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ARA_LITTLE_DEVIL"] } ) -- 35레벨 이하 소마 캐릭터 경험치 1.5배
GameEvent:AddReward( 2033, { fEventTime = 35, iRewardID = 2034, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 World War 칭호(8일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2034, { fEventTime = 35, iRewardID = 2035, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 안티 포이즌 반지(3일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2035, { fEventTime = 35, iRewardID = 2036, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 소환석 : 돌진 전차 모비RT (3일권) 1개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2036, { fEventTime = 35, iRewardID = 2037, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 루리엘의 플루오르스톤 교환권 1개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2037, { fEventTime = 35, iRewardID = 2038, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 2차 전직 프로모션 아바타 무기(포이즈닝)(3일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2038, { fEventTime = 35, iRewardID = 2039, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 선택 스킬 초기화 3개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2039, { fEventTime = 35, iRewardID = 2040, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 루리엘의 활력의 포션 10개, 루리엘의 마나 에릭실 10개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2040, { fEventTime = 35, iRewardID = 2041, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 부활석 3개 큐브 1개 우편 지급(매일/캐릭터/35분)

-- 2014 아라 신전직 전야 이벤트
GameEvent:AddReward( 2041, { fEventTime = 10, iRewardID = 2043, bGiveOnce = True} ) -- 미약한 사파의 정수 2개 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2042, { fEventTime = 10, iRewardID = 2044, bGiveOnce = True, iEventItemID = 85003833 } )	-- 정파 비전 큐브 열쇠 1개 우편 지급(한번/캐릭터/10분/정파비전 큐브 보유시)
GameEvent:AddReward( 2043, { fEventTime = 10, iRewardID = 2045, bGiveOnce = True, iEventItemID = 85003834 } )	-- 사파 비전 큐브 열쇠 1개 우편 지급(한번/캐릭터/10분/사파비전 큐브 보유시)

-- 2014-02-12 정기점검 이후 ~ 2014-02-26 정기점검 결혼 이벤트
-- 1. 나의 반쪽을 찾아서 (2014-02-12 정기점검 이후 ~ 2014-02-26 정기점검 이전)
GameEvent:AddReward( 2051, { fEventTime = 10, iRewardID = 2051, bGiveOnce = True } )  -- 가상 결혼 신청서 (한번/캐릭터/10분)

-- 스마트 밸런스! 2014-02-12 정기점검 이후 ~ 2014-02-18 23:59
GameEvent:AddReward( 2052, { fEventTime = 10, iRewardID = 2053, bGiveOnce = True, iMinLevel = 10 } ) -- 스마트 밸런스 쿨타임 가속기 (1일), 루리엘의 망각의 알약 (1일권) 1개

-- 샌더 5,6던전 이벤트  샌더 마을의 서신
GameEvent:AddReward( 2053, { fEventTime = 60, iRewardID = 2054, bGiveOnce = True, iMinLevel = 10 } ) -- 풍화된 샌더 마을의 서신 조각 1개, 엘의 조각(윈드) 1개

--진입구조 개편 ---------------------
GameEvent:AddReward( 10410, { fEventTime =  10,	iRewardID = 10410, bGiveOnce =  True, iMinLevel = 10 } ) -- 대리 판매 보증서(VIP, 3일권)

