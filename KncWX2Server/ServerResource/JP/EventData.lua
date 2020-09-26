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


--기축년 이벤트
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
GameEvent:AddReward( 259, { fDropRate =  2, bWithPlayPcBang = True } ) -- PC방 유저와 함께 플레이시 드롭률 2배


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
GameEvent:AddReward( 267, { fDropRate =  2 } ) -- 드롭률 2배


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



---------------------------------------------------------------------------------------------------
--공지 이벤트 날릴 센터서버UID
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_SOLES"],  1 ) -- 솔레스
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_GAIA"], 1 )  -- 가이아


------------------------------------------------------------------------------------------------------------------------------------------------------
-- 일본 접속 이벤트
------------------------------------------------------------------------------------------------------------------------------------------------------
GameEvent:AddReward( 501, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 502, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 503, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 504, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 505, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 506, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 507, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 508, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 509, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 510, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 511, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 512, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 513, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 514, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 515, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 516, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 517, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 518, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 519, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 520, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 521, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 522, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브
GameEvent:AddReward( 523, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 501, bGiveOnce =  True } )  -- 초보자 이벤트 큐브



GameEvent:AddReward( 530, 0.5, 0.0, 0.0, { iUnitLevel = 15, cUnitType = UNIT_TYPE["UT_NONE"] } )  -- 전 캐릭터 15렙 이하 경험치 50%
GameEvent:AddReward( 531, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 531, bGiveOnce =  True } )  -- 만우절 아이템


GameEvent:AddReward( 532, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 532, bGiveOnce =  True } )  -- 잠겨진 트레져 박스 여는 열쇠 주는 이벤트


GameEvent:AddReward( 536, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 511, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 537, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 512, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 538, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 513, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 539, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 514, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 540, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 515, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 541, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 516, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 542, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 517, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 543, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 518, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 544, 0.0, 0.0, 0.0, { fEventTime =   2, iRewardID = 519, bGiveOnce =  True } )  -- 골든위크 접속이벤트
GameEvent:AddReward( 545, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 520, bGiveOnce =  True } )  -- 골든위크 접속이벤트


GameEvent:AddReward( 546, 0.2, 0.0, 0.0, { iUnitLevel = 30, cUnitType = UNIT_TYPE["UT_NONE"] } )  -- 전 캐릭터 30렙 이하 경험치 20%
GameEvent:AddReward( 547, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 534, bGiveOnce =  True } )  -- 특정시간 접속하면 사탕큐브 주기

GameEvent:AddReward( 548, 0.0, 0.0, 0.0, { fEventTime =   30, iRewardID = 535, bGiveOnce =  True } )  -- 30분 접속이벤트
GameEvent:AddReward( 549, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 536, bGiveOnce =  True } )  -- 크로우 이벤트1
GameEvent:AddReward( 550, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 536, bGiveOnce =  True } )  -- 크로우 이벤트2

GameEvent:AddReward( 551, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 538, bGiveOnce =  True } )  -- 땀에젖은손수건
GameEvent:AddReward( 552, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 539, bGiveOnce =  True } )  -- 아리엘의실체

GameEvent:AddReward( 553, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 540, bGiveOnce =  True } )  -- 축구 1승 기념 큐브

GameEvent:AddReward( 554, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 541, bGiveOnce =  True } )  -- 카야크의 흔적

GameEvent:AddReward( 556, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 543, bGiveOnce =  True } )  -- 아이아이카사
GameEvent:AddReward( 555, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 542, bGiveOnce =  True } )  -- 찢어진 도전장

--GameEvent:AddReward( 557, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 544, bGiveOnce =  True } )  -- 일장기큐브
--GameEvent:AddReward( 558, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 545, bGiveOnce =  True } )  -- 축구공큐브

GameEvent:AddReward( 559, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 546, bGiveOnce =  True } )  -- 비던출입증 큐브

GameEvent:AddReward( 560, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 547, bGiveOnce =  True } )  -- 여왕의 큐브

GameEvent:AddReward( 561, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 548, bGiveOnce =  True } )  -- 바다의날 기념큐브

GameEvent:AddReward( 562, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 549, bGiveOnce =  True, bPcBangEvent = True } )  -- PC방 특전 티켓

GameEvent:AddReward( 563, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 550, bGiveOnce =  True } )  -- 아리엘의 미소 쿠폰

GameEvent:AddReward( 564, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 551, bGiveOnce =  True } )  -- 불꺼진 성냥

GameEvent:AddReward( 565, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 552, bGiveOnce =  True } )  -- 윌리엄 시공 입장권

GameEvent:AddReward( 567, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 553, bGiveOnce =  True } )  -- 김이 모락 타코야키

GameEvent:AddReward( 568, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 554, bGiveOnce =  True } )  -- R지급

GameEvent:AddReward( 569, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 555, bGiveOnce =  True } )  -- 유카타 1일권 큐브

GameEvent:AddReward( 570, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 556, bGiveOnce =  True } )  -- 아리엘의 편지

GameEvent:AddReward( 571, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 557, bGiveOnce =  True } )  -- 학교가자 큐브

GameEvent:AddReward( 572, 0.0, 0.0, 0.0, { fEventTime =   00, iRewardID = 558, bGiveOnce =  True } )  -- 이브의 머리카락 2

GameEvent:AddReward( 573, 0.0, 0.0, 0.0, { fEventTime =   30, iRewardID = 559, bGiveOnce =  True } )  -- 망각의 알약

GameEvent:AddReward( 574, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 560, bGiveOnce =  True } )  -- 망각의 알약

GameEvent:AddReward( 575, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 561, bGiveOnce =  True } )  -- 마족의 할로윈 망토 7일권 큐브
GameEvent:AddReward( 576, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 562, bGiveOnce =  True } )  -- 단풍잎

GameEvent:AddReward( 577, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 563, bGiveOnce =  True } )  -- 알비노고스트
GameEvent:AddReward( 578, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 564, bGiveOnce =  True } )  -- 스트레인저
GameEvent:AddReward( 579, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 565, bGiveOnce =  True } )  -- 펌킨 킹

GameEvent:AddReward( 580, 0.0, 0.0, 0.0, { fEventTime =   150, iRewardID = 566, bGiveOnce =  True } )  -- 치토세아메(노랑)
GameEvent:AddReward( 581, 0.0, 0.0, 0.0, { fEventTime =   150, iRewardID = 567, bGiveOnce =  True } )  -- 치토세아메(하양)

GameEvent:AddReward( 582, 0.0, 0.0, 0.0, { fEventTime =   30, iRewardID = 566, bGiveOnce =  True } )  -- 치토세아메(노랑)

GameEvent:AddReward( 583, 0.0, 0.0, 0.0, { fEventTime =   30, iRewardID = 568, bGiveOnce =  True } )  -- ewe 기념큐브
GameEvent:AddReward( 584, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 569, bGiveOnce =  True } )  -- 1st EWE

GameEvent:AddReward( 585, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 570, bGiveOnce =  True } )  -- 비던 입장권 큐브
GameEvent:AddReward( 586, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 571, bGiveOnce =  True } )  -- 컵라면


-- 크리스마스 트리 이벤트 [누적시간이벤트]
GameEvent:AddReward( 587, { fEventTime =  200, iRewardID = 573, bDirectReward = True } )	-- 크리스마스 트리 이벤트

GameEvent:AddReward( 588, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 572, bGiveOnce =  True } )  -- 개봉용 가위

GameEvent:AddReward( 589, 0.0, 0.0, 0.0, { fEventTime =   60, iRewardID = 574, bGiveOnce =  True } )  -- 우사기에마

GameEvent:AddReward( 590, 0.0, 0.0, 0.0, { fEventTime =   30, iRewardID = 575, bGiveOnce =  True, bPcBangEvent = True } )  -- 1일권 15%경험치메달(PC방)

GameEvent:AddReward( 591, 0.0, 0.0, 0.0, { fEventTime =   120, iRewardID = 576, bGiveOnce =  True } )  -- 아리엘의타코

GameEvent:AddReward( 592, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 577, bGiveOnce =  True } )  -- 발렌타인 데이 큐브

GameEvent:AddReward( 593, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 578, bGiveOnce =  True } )  -- 헤니르의 시공 접속 이벤트 (열쇠)
GameEvent:AddReward( 594, 0.0, 0.0, 0.0, { fEventTime =   0, iRewardID = 579, bGiveOnce =  True } )  -- 헤니르의 시공 접속 이벤트 (비밀열쇠)

GameEvent:AddReward( 595, 0.0, 0.0, 0.0, { fEventTime =   10, iRewardID = 580, bGiveOnce =  True } )  -- 동접 목표 7000 달성시 보상 이벤트
GameEvent:AddReward( 596, 0.0, 0.0, 0.0, { fEventTime =   10, iRewardID = 581, bGiveOnce =  True } )  -- 동접 목표 8000 달성시 보상 이벤트
GameEvent:AddReward( 597, 0.0, 0.0, 0.0, { fEventTime =   10, iRewardID = 582, bGiveOnce =  True } )  -- 동접 목표 9000 달성시 보상 이벤트
GameEvent:AddReward( 598, 0.0, 0.0, 0.0, { fEventTime =   10, iRewardID = 583, bGiveOnce =  True } )  -- 동접 목표 10000 달성시 보상 이벤트

GameEvent:AddReward( 599, { fEventTime =   0, iRewardID = 584 } ) -- 엘소드 일본 1주년 기념 칭호 14일권
GameEvent:AddReward( 600, { fEventTime =   0, iRewardID = 585 } ) -- 엘소드 일본 1주년 기념 칭호 영구권

GameEvent:AddReward( 601, { fEventTime =   0, iRewardID = 586 } ) -- 엘소드 일본 만우절 NPC복장,투명무기

GameEvent:AddReward( 602, { fEventTime =   0, iRewardID = 587 } ) -- 녹색의 날 자연의 선물 이벤트
GameEvent:AddReward( 603, { fEventTime =   0, iRewardID = 588 } ) -- 파자마 이벤트

GameEvent:AddReward( 604, { fEventTime =   0, iRewardID = 589 } ) -- 액세서리X 지급
GameEvent:AddReward( 605, { fEventTime =  60, iRewardID = 590 } ) -- 엘수색대
GameEvent:AddReward( 606, { fEventTime =   0, iRewardID = 591 } ) -- 비룡의 큐브

GameEvent:AddReward( 607, { fEventTime =   0, iRewardID = 592 } ) -- 왼쪽 라셰 사진
GameEvent:AddReward( 608, { fEventTime =  10, iRewardID = 593, bPcBangEvent = True } )  -- PC방 프리미엄 티켓

GameEvent:AddReward( 609, 0.0, 0.0, 0.0, { fEventTime =   30, iRewardID = 598, bGiveOnce =  True } )  -- 망각의 알약 주말 5개

GameEvent:AddReward( 610, { fEventTime =   0, iRewardID = 600 } ) -- 플르오네 탐험가 칭호 15일권


GameEvent:AddReward( 611, { fEventTime =   0, iRewardID = 601 } ) -- 1차 지령서
GameEvent:AddReward( 612, { fEventTime =   0, iRewardID = 602 } ) -- 2차 지령서
GameEvent:AddReward( 613, { fEventTime =   0, iRewardID = 603 } ) -- 3차 지령서

GameEvent:AddReward( 614, { fEventTime =   30, iRewardID = 604 } ) -- 마나민트 츄잉껌(7일)
GameEvent:AddReward( 615, { fEventTime =   30, iRewardID = 605 } ) -- 뽀루의 발 도장(하얀색)
GameEvent:AddReward( 616, { fEventTime =   30, iRewardID = 606 } ) -- 플루오르 스톤 랜덤 큐브
GameEvent:AddReward( 617, { fEventTime =   30, iRewardID = 607 } ) -- 완전회복 포션 10개
GameEvent:AddReward( 618, { fEventTime =   30, iRewardID = 608 } ) -- 뽀루의 발 도장(노란색)
GameEvent:AddReward( 619, { fEventTime =   30, iRewardID = 609 } ) -- 경험치 메달 큐브(1일권)
GameEvent:AddReward( 620, { fEventTime =   30, iRewardID = 610 } ) -- 프로모션 아바타 큐브(7일권)
GameEvent:AddReward( 621, { fEventTime =   30, iRewardID = 611 } ) -- 뽀루의 발 도장(보라색)
GameEvent:AddReward( 622, { fEventTime =   30, iRewardID = 612 } ) -- 일급 마법석 랜덤 큐브

GameEvent:AddReward( 623, { fEventTime =   0, iRewardID = 614 } ) -- 퀴즈퀴즈 칭호

GameEvent:AddReward( 624, { fEventTime =   30, iRewardID = 615 } ) -- 공존의 축제 큐브
GameEvent:AddReward( 625, { fEventTime =   0, iRewardID = 616 } ) -- 공존의 축제 큐브

GameEvent:AddReward( 626, { fEventTime =   30, iRewardID = 617 } ) -- 할로윈 고스트 캔디 3개 지급
GameEvent:AddReward( 627, { fEventTime =   30, iRewardID = 618 } ) -- 할로윈 고스트 캔디 6개 지급

-- 할로윈 던전 무료입장
GameEvent:AddReward( 628, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_NORMAL"]} )
GameEvent:AddReward( 629, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_HARD"]	} )
GameEvent:AddReward( 630, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_EXPERT"]} )

GameEvent:AddReward( 631, { fEventTime =   60, iRewardID = 620 } ) -- 시원한 얼음물 5개
GameEvent:AddReward( 632, { fEventTime =   60, iRewardID = 621 } ) -- 체육대회 출석도장
GameEvent:AddReward( 633, { fEventTime =   100, iRewardID = 622 } ) -- 호박 캔디 1개
GameEvent:AddReward( 634, { fEventTime =   100, iRewardID = 623 } ) -- 코보 특제 몬스터 가면 큐브

GameEvent:AddReward( 635, { fEventTime =   0, iRewardID = 619 } ) -- 코보 익스프레스 VIP 티켓 1일권

GameEvent:AddReward( 636, { fEventTime =   5, iRewardID = 624 } ) -- 변신 물약 랜덤 큐브 5개

GameEvent:AddReward( 637, { fEventTime =   60, iRewardID = 625 } ) -- 호프만의 출입증서
GameEvent:AddReward( 638, { fEventTime =   60, iRewardID = 626 } ) -- 나락의 지도
GameEvent:AddReward( 639, { fEventTime =   60, iRewardID = 627 } ) -- 항 알테라시아 캡슐
GameEvent:AddReward( 640, { fEventTime =   120, iRewardID = 611 } ) -- 뽀루의 발 도장(보라색)
GameEvent:AddReward( 641, { fEventTime =   120, iRewardID = 187 } ) -- 얼음 조각상 가열기 (아리엘의 선물)
GameEvent:AddReward( 642, { fEventTime =   120, iRewardID = 628 } ) -- 망각의 드링크
GameEvent:AddReward( 643, { fEventTime =   60, iRewardID = 629 } ) -- 플루오르 스톤 Lv.5
GameEvent:AddReward( 644, { fEventTime =   0, iRewardID = 630 } ) -- 아리엘의 쿠폰 2장
GameEvent:AddReward( 645, { fEventTime =   0, iRewardID = 631 } ) -- 루리엘의 감사의 증표 2장

-- 대장장이의 전투 지원
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	632	, iMinLevel = 	1	,   iMaxLevel =	13	 } )	-- 숲의 수호자 무기 큐브
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	633	, iMinLevel = 	14	,   iMaxLevel =	19	 } )	-- 시민의 수호자 무기 큐브
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	634	, iMinLevel = 	20	,   iMaxLevel =	25	 } )	-- 파멸의 용병 무기 큐브
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	635	, iMinLevel = 	26	,   iMaxLevel =	29	 } )	-- 분노의 화신 무기 큐브
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	636	, iMinLevel = 	30	,   iMaxLevel =	33	 } )	-- 문명의 제압자 무기 큐브
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	637	, iMinLevel = 	34	,   iMaxLevel =	37	 } )	-- 마도대전 용사의 무기 큐브
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	638	, iMinLevel = 	38	,   iMaxLevel =	45	 } )	-- 블랙스미스 연합 무기 큐브
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	639	, iMinLevel = 	46	,   iMaxLevel =	49	 } )	-- 혈투의 노래 무기 큐브
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 	640	, iMinLevel = 	50	,   iMaxLevel =	56	 } )	-- 빛의 수호자 무기 큐브

GameEvent:AddReward( 647, { fEventTime =   0,  iRewardID = 	641 } )	-- 연금술사의 전투지원 랜덤큐브

GameEvent:AddReward( 648, { fEventTime =   60,  iRewardID = 	642 } )	-- 청 2차전직 전야 큐브

GameEvent:AddReward( 649, { fEventTime =   150,  iRewardID = 	643, bAccountEvent = True  } )	-- 크리스마스 선물
GameEvent:AddReward( 650, { fEventTime =   0,  iRewardID = 	644 } ) -- 루돌프 빨간코
GameEvent:AddReward( 651, { fEventTime =   0,  iRewardID = 	628 } )	-- 망각의 드링크

GameEvent:AddReward( 652, { fEventTime =   0,  iRewardID = 	648 } )	-- 샤샤의 SOS! 칭호 1일권

GameEvent:AddReward( 653, { fEventTime =   0,  iRewardID = 	649 } )	-- 환각의 벨더 큐브

GameEvent:AddReward( 654, { fEventTime =   100,  iRewardID = 	651 } )	-- 달콤한 초콜릿

GameEvent:AddReward( 655, { fEventTime =   100,  iRewardID = 	652 } )	-- 레인저의 호출
GameEvent:AddReward( 656, { fEventTime =   60,  iRewardID = 	653 } )	-- 머리 위 고양이 1일권
GameEvent:AddReward( 657, { fEventTime =   60,  iRewardID = 	654 } )	-- 새롭게된 던전 아이템 큐브

GameEvent:AddReward( 658, { fEventTime =   0,  iRewardID = 	655,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) -- 활력의 검사 포션 1개
GameEvent:AddReward( 659, { fEventTime =   0,  iRewardID = 	656,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) -- 파괴의 검사 포션 1개
GameEvent:AddReward( 660, { fEventTime =   120,  iRewardID = 	657 } )	-- 검의 기운 조각 1개

GameEvent:AddReward( 661, { fEventTime =   0,  iRewardID = 	658 } )	-- 파자마 1일
GameEvent:AddReward( 662, { fEventTime =   0,  iRewardID = 	659 } )	-- 신선조 1일
GameEvent:AddReward( 663, { fEventTime =   0,  iRewardID = 	660 } )	-- 서머 비치룩 1일
GameEvent:AddReward( 664, { fEventTime =   0,  iRewardID = 	661 } )	-- 프로모션 3차 전직 1일
GameEvent:AddReward( 665, { fEventTime =   0,  iRewardID = 	662 } )	-- 프로모션 2차 전직 1일
GameEvent:AddReward( 666, { fEventTime =   0,  iRewardID = 	663 } )	-- 마린룩 1일
GameEvent:AddReward( 667, { fEventTime =   0,  iRewardID = 	664 } )	-- 스타 아카데미 (하복) 1일
GameEvent:AddReward( 668, { fEventTime =   0,  iRewardID = 	665 } )	-- 의사,간호사 1일
GameEvent:AddReward( 669, { fEventTime =   0,  iRewardID = 	666 } )	-- 할로윈 1일
GameEvent:AddReward( 670, { fEventTime =   0,  iRewardID = 	667 } )	-- 배드 가이즈 1일
GameEvent:AddReward( 671, { fEventTime =   0,  iRewardID = 	668 } )	-- 프로모션 1차 전직 1일
GameEvent:AddReward( 672, { fEventTime =   0,  iRewardID = 	669 } )	-- 경찰 1일
GameEvent:AddReward( 673, { fEventTime =   0,  iRewardID = 	670 } )	-- 타탄 체크 1일
GameEvent:AddReward( 674, { fEventTime =   0,  iRewardID = 	671 } )	-- 서커스 1일

GameEvent:AddReward( 675, { fEventTime =   0,  iRewardID = 	672 } )	-- 2주년 파티 칭호 1일
GameEvent:AddReward( 676, { fEventTime =   0,  iRewardID = 	673 } )	-- 2주년 특별 제조권

GameEvent:AddReward( 677, { fEventTime =   60,  iRewardID = 	674,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) -- 2주년 기념 뽀루 모자

GameEvent:AddReward( 678, { fEventTime =   120,  iRewardID = 	675, bAccountEvent = True } )	-- 뽀루의 황금 코인
	
GameEvent:AddReward( 679, { fEventTime =   60,  iRewardID = 	677,	cUnitType = UNIT_TYPE["UT_ARME"] } ) -- 2주년 기념 뽀루 모자
GameEvent:AddReward( 680, { fEventTime =   60,  iRewardID = 	678,	cUnitType = UNIT_TYPE["UT_LIRE"] } ) -- 2주년 기념 뽀루 모자
GameEvent:AddReward( 681, { fEventTime =   60,  iRewardID = 	679,	cUnitType = UNIT_TYPE["UT_RAVEN"] } ) -- 2주년 기념 뽀루 모자
GameEvent:AddReward( 682, { fEventTime =   60,  iRewardID = 	680,	cUnitType = UNIT_TYPE["UT_EVE"] } ) -- 2주년 기념 뽀루 모자
GameEvent:AddReward( 683, { fEventTime =   60,  iRewardID = 	681,	cUnitType = UNIT_TYPE["UT_CHUNG"] } ) -- 2주년 기념 뽀루 모자

GameEvent:AddReward( 684, { fEventTime =   60,  iRewardID = 	682,	cUnitType = UNIT_TYPE["UT_RAVEN"] } ) -- 심기체의 패키지 큐브

GameEvent:AddReward( 685, { fEventTime =   0,  iRewardID = 	683 } ) -- 양치기의 큐브
GameEvent:AddReward( 686, { fEventTime =   120,  iRewardID = 	683 } ) -- 양치기의 큐브
GameEvent:AddReward( 687, { fEventTime =   0,  iRewardID = 	684 } ) -- 피노키오의 매력 1일

GameEvent:AddReward( 688, { fEventTime =   0,  iRewardID = 		685 ,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) -- 그림자 큐브
GameEvent:AddReward( 689, { fEventTime =   0,  iRewardID = 		686 } ) -- 콘웰
GameEvent:AddReward( 690, { fEventTime =   60,  iRewardID = 	687, bAccountEvent = True } ) -- 인어의 비늘로 만든 정

-- 2012.04.25
GameEvent:AddReward( 691, { fEventTime =   60,  iRewardID = 	688, bAccountEvent = True } ) -- 골든 위크 기념권 (평일) 60분
GameEvent:AddReward( 692, { fEventTime =   120,  iRewardID = 	689, bAccountEvent = True } ) -- 골든 위크 기념권 (평일) 120분

GameEvent:AddReward( 693, { fEventTime =   60,  iRewardID = 	689, bAccountEvent = True } ) -- 골든 위크 기념권 (어린이날) 60분
GameEvent:AddReward( 694, { fEventTime =   120,  iRewardID = 	691, bAccountEvent = True } ) -- 골든 위크 기념권 (어린이날) 120분

GameEvent:AddReward( 695, { fEventTime =   60,  iRewardID = 	689, bAccountEvent = True } ) -- 골든 위크 기념권 (막날) 60분
GameEvent:AddReward( 696, { fEventTime =   120,  iRewardID = 	690, bAccountEvent = True } ) -- 골든 위크 기념권 (막날) 120분

GameEvent:AddReward( 697, { fEventTime =   30,  iRewardID = 	692 } ) -- 자연의 선물 (평일)
GameEvent:AddReward( 698, { fEventTime =   30,  iRewardID = 	693 } ) -- 자연의 선물 (주말)

GameEvent:AddReward( 699, { fEventTime =   0,  iRewardID = 		694 ,	cUnitType = UNIT_TYPE["UT_ARME"] } ) -- 아이샤 그림자 큐브
GameEvent:AddReward( 700, { fEventTime =   0,  iRewardID =		695 } ) -- 마력의 펜던트(이벤트)
GameEvent:AddReward( 701, { fEventTime =   0,  iRewardID = 		696 ,	cUnitType = UNIT_TYPE["UT_EVE"] } ) -- 이브 왕관

-- 2012.05.16
GameEvent:AddReward( 702, { fEventTime =   60,  iRewardID =		697 } ) -- 아이샤의 보석

-- 2012.05.30 운동회
GameEvent:AddReward( 703, { fEventTime =   120,  iRewardID =		698 } ) -- 운동회 행운 추첨 큐브 1회차
GameEvent:AddReward( 704, { fEventTime =   0,  iRewardID = 		699 ,	cUnitType = UNIT_TYPE["UT_LIRE"] } ) -- 레나 그림자 큐브
GameEvent:AddReward( 705, { fEventTime =   0,  iRewardID =		700 } ) -- 에렌딜에 관한 연구 논문(이벤트)
GameEvent:AddReward( 706, { fEventTime =   120,  iRewardID =		701 } ) -- 운동회 행운 추첨 큐브 2회차
----

-- 2012.06.13 트랩핑 레인저
GameEvent:AddReward( 707, { fEventTime =   90,  iRewardID =		702 } ) -- 메모 랜덤 큐브

-- 2012.06.27 니코니코 채널링
GameEvent:AddReward( 708, { fEXPRate = 1.0, iUnitLevel = 35, iChannelCode = CHANNELING_ENUM["CE_NICO"] }  )	-- 니코니코 경험치 2배
GameEvent:AddReward( 709, { fEventTime =   60,  iRewardID =		704 } ) -- 선택의 메달
GameEvent:AddReward( 710, { fEventTime =   0,  iRewardID = 		705 ,	cUnitType = UNIT_TYPE["UT_RAVEN"] } ) -- 레이븐 그림자 큐브
GameEvent:AddReward( 711, { fEventTime =   0,  iRewardID =		706 } ) -- 나소드 핸드 오버히트 개조법
GameEvent:AddReward( 712, { fEventTime =   30,  iRewardID =		707, bAccountEvent = True, bComebackEvent = True }  )	-- 복귀 환영 큐브( 1주차 )
GameEvent:AddReward( 713, { fEventTime =   30,  iRewardID =		708, bAccountEvent = True, bComebackEvent = True }  )	-- 복귀 환영 큐브( 2주차 )
GameEvent:AddReward( 714, { fEventTime =   30,  iRewardID =		709, bAccountEvent = True, bComebackEvent = True }  )	-- 복귀 환영 편지( 1주차 )
GameEvent:AddReward( 715, { fEventTime =   30,  iRewardID =		710, bAccountEvent = True, bComebackEvent = True }  )	-- 복귀 환영 편지( 2주차 )

-- 2012.07.06 웨폰테이커
GameEvent:AddReward( 716, { fEventTime =   60,  iRewardID =		711 } ) -- 웨폰테이커의 쿠폰
GameEvent:AddReward( 717, { fEventTime =   0,  iRewardID = 		712 ,	cUnitType = UNIT_TYPE["UT_EVE"] } ) -- 이브 그림자 큐브
GameEvent:AddReward( 718, { fEventTime =   0,  iRewardID =		713 } ) -- 분광 프리즘

-- 2012.07.25 올림픽
GameEvent:AddReward( 719, { fEventTime =   1,  iRewardID =		714 } ) -- 올림픽 칭호
GameEvent:AddReward( 720, { fEventTime =   10,  iRewardID =		715, bAccountEvent = True } ) -- 플루오르 스톤 Lv1
GameEvent:AddReward( 721, { fEventTime =   10,  iRewardID =		716, bAccountEvent = True } ) -- 축복받은 복원의 주문서 Lv1
GameEvent:AddReward( 722, { fEventTime =   120,  iRewardID =	717, bAccountEvent = True } ) -- 올림픽 폐막식 도전자의 상징
GameEvent:AddReward( 723, { fEventTime =   120,  iRewardID =	718, bAccountEvent = True } ) -- 올림픽 폐막식 아리엘의 가열기

-- 2012.08.08 축제
GameEvent:AddReward( 724, { fEventTime =   30,  iRewardID =		724 } ) -- 마츠리다 왓쇼이 15일권
GameEvent:AddReward( 725, { fEventTime =   0,  iRewardID = 		725 ,	cUnitType = UNIT_TYPE["UT_CHUNG"] } ) -- 청 그림자 큐브
GameEvent:AddReward( 726, { fEventTime =   0,  iRewardID =		726 } ) -- 레이저 발사 장치

-- 2012.08.10 쉘링가디언
GameEvent:AddReward( 727, { fEventTime =   60,  iRewardID =		727 } ) -- 월리의 성 입장권
GameEvent:AddReward( 728, { fEventTime =   60,  iRewardID =		728, iUnitLevel = 30, bAccountEvent = True } ) -- 캐시 큐브

-- 2012.08.24 신학기
GameEvent:AddReward( 729, { fEventTime =   0,  iRewardID =		729 } ) -- 자유연구의 소재큐브
GameEvent:AddReward( 730, { fEventTime =   0,  iRewardID =		730 } ) -- 연막탄 큐브

-- 2012.09.12 레이징 및 넥슨
GameEvent:AddReward( 731, { fEventTime =   0,  iRewardID =		731, bAccountEvent = True, iChannelCode = CHANNELING_ENUM["CE_NEXON"] } ) -- 액세서리 랜덤 큐브
GameEvent:AddReward( 732, { fEventTime =   30,  iRewardID =		732 } ) -- 다시 떠오르는 영웅들
GameEvent:AddReward( 733, { fEventTime = 0, iRewardID = 733, bAccountEvent = True, bNewUserEvent = True } ) -- 신규 유저 환영 큐브
GameEvent:AddReward( 734, { fEventTime = 0, iRewardID = 734, bAccountEvent = True, bCurrentUserEvent = True } ) -- 좋은사람입니다

-- 2012.09.26 어둠의문
GameEvent:AddReward( 735, { fEventTime = 30, iRewardID = 735 } ) -- 마족의 흔적
-- 2012.09.26 구미
GameEvent:AddReward( 736, { fEventTime = 30, iRewardID = 736 } ) -- 메구포이드 아바타1P (1일권) 큐브
-- 2012.09.26 한게임 페스티벌
GameEvent:AddReward( 737, { fEventTime = 30, iRewardID = 737 } ) -- 칠흑의 날개(가칭, 1일권)
-- 2012.09.26 넷카페
GameEvent:AddReward( 738, { fEventTime =  120, iRewardID = 738, bPcBangEvent = True } )  -- 아리엘의 프리미엄 티켓
GameEvent:AddReward( 739, { fEventTime =  120, iRewardID = 739, bPcBangEvent = True } )  -- 아리엘의 넷까페 지원 티켓

-- 2012.10.10 펫 이벤트
GameEvent:AddReward( 740, { fEventTime =  150, iRewardID = 740, bAccountEvent = True } )  -- 덫에 걸린 해츨링(유체)
GameEvent:AddReward( 741, { fEventTime =  30, iRewardID = 741, bAccountEvent = True } )  -- 카밀라의 데일리 펫 훈련 매거진

-- 2012.10.10 몬스터 카드 이벤트
GameEvent:AddReward( 742, { fEventTime =  0, iRewardID = 742 } )  -- 의문의 몬스터 카드
GameEvent:AddReward( 743, { fEventTime =  0, iRewardID = 743 } )  -- 몬스터 카드 수집가의 모자 ( 30일권 )
GameEvent:AddReward( 744, { fEventTime =  0, iRewardID = 744 , iMinLevel = 	40	,   iMaxLevel =	60 } )  -- 비던 입장권
GameEvent:AddReward( 745, { fEventTime =  60, iRewardID = 745 } )  -- 카밀라의 펫 훈련장 초대권 3장
GameEvent:AddReward( 746, { fEventTime =  60, iRewardID = 746 } )  -- 카밀라의 펫 훈련장 초대권 5장

-- 2012.10.24 태그토너먼트 서포터즈
GameEvent:AddReward( 747, { fEventTime = 30,	iRewardID = 747, bAccountEvent = True } ) -- 태그토너먼트 서포터즈 큐브
GameEvent:AddReward( 748, { fEventTime =  10, iRewardID = 748 } )  -- 아리엘의 할로윈 사탕 1개
GameEvent:AddReward( 749, { fEventTime =  10, iRewardID = 749 } )  -- 아리엘의 할로윈 사탕 2개
GameEvent:AddReward( 750, { fEventTime =  10, iRewardID = 750 } )  -- 데스사이즈 – 조화(1일권)
GameEvent:AddReward( 751, { fEventTime =  10, iRewardID = 751 } )  -- 데스사이즈 – 신비(1일권)
GameEvent:AddReward( 752, { fEventTime =  10, iRewardID = 752 } )  -- 데스사이즈 – 광휘(1일권)
GameEvent:AddReward( 753, { fEventTime =  10, iRewardID = 753 } )  -- 데스사이즈(1일권) 6종 큐브
GameEvent:AddReward( 754, { fEventTime =  10, iRewardID = 754 } )  -- 데스사이즈 – 전투(1일권)
GameEvent:AddReward( 755, { fEventTime =  10, iRewardID = 755 } )  -- 데스사이즈 – 증강(1일권)
GameEvent:AddReward( 756, { fEventTime =  10, iRewardID = 756 } )  -- 데스사이즈 – 적중(1일권)

-- 2012.10.30 미션던전
GameEvent:AddReward( 757, { fEventTime = 30,	iRewardID = 757 } ) -- 2012-11-1 30분 접속 시 나의 미션 메모리 카드A
GameEvent:AddReward( 758, { fEventTime = 30,	iRewardID = 758 } ) -- 2012-11-1 30분 접속 시 나의 미션 메모리 카드B
GameEvent:AddReward( 759, { fEventTime = 30,	iRewardID = 759 } ) -- 2012-11-1 30분 접속 시 나의 미션 메모리 카드C
GameEvent:AddReward( 760, { fEventTime = 30,	iRewardID = 760 } ) -- 2012-11-1 30분 접속 시 나의 미션 메모리 카드D
GameEvent:AddReward( 761, { fEventTime = 30,	iRewardID = 761 } ) -- 2012-11-1 30분 접속 시 나의 미션 메모리 카드E
GameEvent:AddReward( 762, { fEventTime = 30,	iRewardID = 762 } ) -- 2012-11-1 30분 접속 시 나의 미션 메모리 카드F
GameEvent:AddReward( 763, { fEventTime = 40,	iRewardID = 763 } ) -- 2012-11-1 40분 접속 시 보호의 부적 1개
GameEvent:AddReward( 764, { fEventTime = 70,	iRewardID = 764 } ) -- 2012-11-1 주말 70분 접속 시 보호의 부적 2개

-- 2012.11.14 월동준비
GameEvent:AddReward( 766, { fEventTime = 0,		iRewardID = 766 } ) -- 리스겔드의 편지 1개  (20121114~20121128)
GameEvent:AddReward( 767, { fEventTime = 0,		iRewardID = 767 } ) -- 리스트완느의 편지 1개 (20121128~20121212)
GameEvent:AddReward( 768, { fEventTime = 30,	iRewardID = 768 } ) -- 마법의 비약 조각 (20121114~20121212)

-- 2012.11.28 고속 레벨업
GameEvent:AddReward( 769, { fEXPRate = 1.0, iStartLevel = 1,  iEndLevel = 20 } ) -- 1레벨부터 20레벨까지 경험치 2배
GameEvent:AddReward( 770, { fEXPRate = 2.0, iStartLevel = 21,  iEndLevel = 35 } ) -- 21레벨부터 35레벨까지 경험치 3배
-- 2012.11.28 미스 엘리오스
GameEvent:AddReward( 771, { fEventTime = 30,	iRewardID = 773 } ) -- 축복의 메달 30% 1일권
GameEvent:AddReward( 772, { fEventTime = 60,	iRewardID = 774 } ) -- 플루오르 스톤 Lv.3~5 랜덤 큐브 1개
GameEvent:AddReward( 773, { fEventTime = 60,	iRewardID = 775 } ) -- 듀얼 마법석 랜덤 큐브 1개
GameEvent:AddReward( 774, { fEventTime = 60,	iRewardID = 776 } ) -- 승리의 왕관 14일권 큐브

-- Comeback 이벤트(복귀 유저 전용) (2012-12-12 ~ 2013-01-09)
GameEvent:AddReward( 775, { fEventTime = 30,	iRewardID =	777, bAccountEvent = True, bComebackEvent	= True }  )	-- Comeback 모험가의 큐브
-- 복귀 유저를 도와주세요! (일반 유저 전용 퀘스트) (2012-12-12 ~ 2013-01-09)
GameEvent:AddReward( 776, { fEventTime = 30,	iRewardID = 778, bAccountEvent = True, bCurrentUserEvent = True } ) -- 샹냥한 모험가의 큐브
-- 스킬 개편 이벤트 (2012-12-12 ~ 2012-12-18)
GameEvent:AddReward( 777, { fEventTime = 30,	iRewardID = 779 } ) -- 망각의 알약

-- 2012.12.18 크리스마스 이벤트
GameEvent:AddReward( 778, { fEventTime = 50, iRewardID = 780, bDirectReward = True } ) 	-- 2012.12.18 매일매일 메리 크리스마스 이벤트 ( 크리스마스 선물 상자, 크리스마스 특별 선물 상자 차등 지급 )
GameEvent:AddReward( 779, { fEventTime = 120, iRewardID = 781, bAccountEvent = True } ) -- 아리엘의 가열기 1개
GameEvent:AddReward( 780, { fEventTime = 120, iRewardID = 782, bAccountEvent = True } ) -- 플루오르 스톤 교환권 1개
GameEvent:AddReward( 781, { fEventTime = 120, iRewardID = 783, bAccountEvent = True } ) -- 크리스마스 지팡이 사탕 장식 액세서리 (영구권)

-- 2012.12.26 신년 이벤트
GameEvent:AddReward( 782, { fEventTime = 100, iRewardID = 784 } ) 	-- 되 찾은 의지 1개

-- 2013.01.09 아이샤 전야 이벤트
GameEvent:AddReward( 783, { fEventTime =   30,  iRewardID = 	785,	cUnitType = UNIT_TYPE["UT_ARME"] } )	-- 불완전한 문스톤 1개
GameEvent:AddReward( 784, { fEventTime =   30,  iRewardID = 	786,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) -- 연마도구 1개
GameEvent:AddReward( 785, { fEventTime =   30,  iRewardID = 	786,	cUnitType = UNIT_TYPE["UT_LIRE"] } )	-- 연마도구 1개
GameEvent:AddReward( 786, { fEventTime =   30,  iRewardID = 	786,	cUnitType = UNIT_TYPE["UT_RAVEN"] } )	-- 연마도구 1개
GameEvent:AddReward( 787, { fEventTime =   30,  iRewardID = 	786,	cUnitType = UNIT_TYPE["UT_EVE"] } )		-- 연마도구 1개
GameEvent:AddReward( 788, { fEventTime =   30,  iRewardID = 	786,	cUnitType = UNIT_TYPE["UT_CHUNG"] } )	-- 연마도구 1개
GameEvent:AddReward( 789, { fEventTime =  0, iRewardID = 787 } ) -- 성인식 아바타 1P 풀세트 1일권 큐브 1개

-- 2013.01.16 아이샤 디멘션 위치 이벤트
GameEvent:AddReward( 790, { fEventTime =  30, iRewardID = 789 } ) -- 두근두근 마법 소녀 1개

-- 2013.01.30 ESJP_나이트 와쳐 전야 이벤트
GameEvent:AddReward( 791, { fEventTime =  1, iRewardID = 790 } ) -- 퐁고족의 서신 1개
GameEvent:AddReward( 792, { fEventTime =  10, iRewardID = 791 } ) -- 폭발의 화살 1개

-- 2013.02.06 ESJP_나이트 와쳐 이벤트
GameEvent:AddReward( 793, { fEventTime =  100, iRewardID = 792,	cUnitType = UNIT_TYPE["UT_LIRE"] } ) -- 의리 초코 1개
GameEvent:AddReward( 794, { fEventTime =  0, iRewardID = 793,	cUnitType = UNIT_TYPE["UT_LIRE"] } ) -- 카밀라의 호출 1개
GameEvent:AddReward( 795, { fEventTime =  0, iRewardID = 794 } ) -- 한복 1P컬러(1일권) 큐브 1개
GameEvent:AddReward( 796, { fEventTime =  0, iRewardID = 795 } ) -- 파티쉐 1P컬러(1일권) 큐브 1개

-- 2013.02.20 ESJP_베테랑 커멘더 전직 전야 이벤트
GameEvent:AddReward( 797, { fEventTime =  1, iRewardID = 796, bGiveOnce = True } ) -- 무의 크로우라이즈(캐릭터/한번/1분)

-- 2013.02.27 ESJP_베테랑 커멘더 전직 이벤트
GameEvent:AddReward( 798, { fEventTime =  1, iRewardID = 797 } ) -- 비밀 지령 큐브 1개(캐릭터/매일/1분)

-- 2013.03.06 ESJP_3주년 이벤트
-- 강화의 주간 (20130306~30130313)
GameEvent:AddReward( 799, { fEventTime =   60, iRewardID = 798 } ) -- 강화성공의 소망 1개(캐릭터/매일/60분)
GameEvent:AddReward( 800, { fEventTime =  120, iRewardID = 799,  bAccountEvent = True, bGiveOnce = True, iMinLevel = 20 } ) -- 플루오르 스톤 교환권 1개(계정/한번/120분/Lv20이상)
GameEvent:AddReward( 801, { fEventTime =  120, iRewardID = 800,  bAccountEvent = True, bGiveOnce = True, iMinLevel = 20 } ) -- 복원의 주문서 교환권 1개(계정/한번/120분/Lv20이상)
-- 협동의 주간 (20130313~30130320) -- 누적(802), 180분->18분
GameEvent:AddReward( 802, { fEventTime =  180, iRewardID = 801,  bAccountEvent = True, bGiveOnce = True } ) -- 아리엘의 3주년 감사 선물(계정/한번/180분/Lv20이상=>제외/누적)
-- 나눔의 주간 (20130320~30130327) -- 누적(803), 30분-> 3분
GameEvent:AddReward( 803, { fEventTime = 30, iRewardID = 802, bDirectReward = True } ) -- 3(30분/Lv20이상=>제외/누적)

-- 2013.03.13 ESJP_Comeback ELSWORD 30분->3분(804)
GameEvent:AddReward( 804, { fEventTime = 30, iRewardID = 803, iEventItemID = 67005073 } ) -- 귀환감사티켓 (30분/ 귀환용사에게 보내는 편지 아이템 소지시)
GameEvent:AddReward( 805, { fEventTime = 30, iRewardID = 804, bGiveOnce = True, bNewUnitEvent = True } ) -- 엘소드의추억 (2011년3월16일이전/2개/30분/한번)
GameEvent:AddReward( 806, { fEventTime = 30, iRewardID = 805, bGiveOnce = True, bNewUnitEvent2 = True } ) -- 엘소드의추억 (2012년3월16일이전/1개/30분/한번)

-- 2013.03.19 ESJP_춘분 
GameEvent:AddReward( 807, { fEventTime = 10, iRewardID = 808 } ) -- 봄의 왈츠 풀세트 큐브(1일권) (캐릭터 / 10분)

-- 2013.03.20 ESJP_배틀 세라프
GameEvent:AddReward( 808, { fEventTime = 30, iRewardID = 809 } ) -- 칭호 배틀 세라프(7일권) (캐릭터 / 30분)

--------------------------------------------------------------------------
-- 2013.04.17 ESJP_택틱컬 커맨더 전야 이벤트( 2013-04-10 ~ 2013-04-17 ) 60분-> 6분
GameEvent:AddReward( 809, { fEventTime = 60, iRewardID = 821, bGiveOnce = True } ) -- 60분 접속 시 디스프론티어의 목걸이 우편 지급

--------------------------------------------------------------------------
-- 2013.04.24 ESJP_윌리의 어린이날 던전 이벤트( 2013-04-24 ~ 2013-05-22 ) 즉시
GameEvent:AddReward( 810, { fEventTime = 0, iRewardID = 822 } ) 
-- 2013.04.24 ESJP_필드100%즐기기! 필수 가이드북 만들기 이벤트( 2013-04-24 ~ 2013-05-22 ) 10분, Lv10이상
GameEvent:AddReward( 811, { fEventTime = 10, iRewardID = 823, bGiveOnce = True, iMinLevel = 10, } ) 
-- 2013.04.24 ESJP_필드 모험 준비 패키지 (한번 / 계정 단위 / 30분접속 /10이상) // 30분->3분
GameEvent:AddReward( 812, { fEventTime = 30, iRewardID = 824, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10, } ) 
-- 2013.04.24 ESJP_택틱컬 트루퍼 이벤트( 2013-04-24 ~ 2013-05-08 ) // 60분->6분
GameEvent:AddReward( 813, { fEventTime = 60, iRewardID = 827, bGiveOnce = True } ) -- 60분 접속 시 실수를 만회할 수 있는 기회 우편 지급(주말)
GameEvent:AddReward( 814, { fEventTime = 60, iRewardID = 828, bGiveOnce = True } ) -- 60분 접속 시 택티컬 트루퍼 칭호 우편 지급
GameEvent:AddReward( 815, { fEventTime = 1, iRewardID = 829, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_CHUNG"] } ) -- 1분 접속 시 청캐릭터, 정의를 위한 신념 우편 지급


--------------------------------------------------------------------------
-- 2013.05.22 ESJP_필드 이벤트 85002462 황금 필드 쿠키
GameEvent:AddReward( 816, { fEventTime = 10, iRewardID = 830, bGiveOnce = True, iMinLevel = 10 } ) -- 10분 캐릭터 접속 시 평일 Lv10, 필드 쿠키
GameEvent:AddReward( 817, { fEventTime = 60, iRewardID = 831, bGiveOnce = True, iMinLevel = 10 } ) -- 60분 캐릭터 접속 시 주말 Lv10, 황금 필드 쿠키

--------------------------------------------------------------------------
-- 2013.06.04 ESJP_오늘도 엘소드 이벤트 50분->5분, (누적,캐릭터,50분,연속)
GameEvent:AddReward( 818, { fEventTime = 50, iRewardID = 833, bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 0 } ) 	-- 아리엘의 감사쿠폰
GameEvent:AddReward( 819, { fEventTime = 50, iRewardID = 833, bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 818 } ) -- 아리엘의 감사쿠폰


--------------------------------------------------------------------------
-- 2013.06.19 ESJP_몬스터 도감만들기 (즉시, 1회, 캐릭터단위) 
GameEvent:AddReward( 820, { fEventTime = 0, iRewardID = 835, bGiveOnce = True  } ) -- 카메라 ES-888(20일)큐브
-- 2013.06.19 ESJP_대박 필드 보스 큐브 (30분, 매일, 캐릭터) (30분->3분)
GameEvent:AddReward( 821, { fEventTime = 30, iRewardID = 836 } ) -- 대박 필드 보스 큐브
-- 2013.06.19 ESJP_냐루코 되기 // 10분, 캐릭터당 1회
GameEvent:AddReward( 822, { fEventTime = 10, iRewardID = 837, bGiveOnce = True } ) -- 냐루코 되기(오리지날) 1일권 큐브
GameEvent:AddReward( 823, { fEventTime = 10, iRewardID = 838, bGiveOnce = True } ) -- 냐루코 되기(엘소드 Ver.) 1일권 큐브


--------------------------------------------------------------------------
-- 2013.07.03 ESJP_잠든 캐릭터를 꺠워라. 60분->6분
GameEvent:AddReward( 824, { fEventTime = 0, iRewardID = 839, bGiveOnce = True } ) -- 용사여 깨어나세요 큐브 
GameEvent:AddReward( 825, { fEventTime = 60, iRewardID = 840, bGiveOnce = True } ) -- 망각의 드링크 7일권 큐브 


--------------------------------------------------------------------------
-- 2013.07.10 ESJP_강화이벤트 60분->6분, 120분->12분
GameEvent:AddReward( 826, { fEventTime = 60, iRewardID = 841, bAccountEvent = True, bGiveOnce = True } ) -- 아리엘의 플루오르스톤 교환권(계정/60분)
GameEvent:AddReward( 827, { fEventTime = 120, iRewardID = 842, bAccountEvent = True, bGiveOnce = True } ) -- 아리엘의 강화부적랜덤 큐브(계정/60분)

-- 2013.07.10 ESJP_신규복귀 유저 이벤트(복귀유저를 위한 코보사 특별 고객 만족 캠페인) 30분->3분
GameEvent:AddReward( 828, { fEventTime = 30, iRewardID = 849, bAccountEvent = True, bGiveOnce = True, bNewUnitEvent = True, iMinLevel = 9, iMaxLevel = 18 } ) -- 30분, 계정, 5월31일이후, Lv.9~18
GameEvent:AddReward( 828, { fEventTime = 30, iRewardID = 850, bAccountEvent = True, bGiveOnce = True, bNewUnitEvent = True, iMinLevel = 19, iMaxLevel = 30 } ) -- 30분, 계정, 5월31일이후, Lv.19~30
GameEvent:AddReward( 828, { fEventTime = 30, iRewardID = 851, bAccountEvent = True, bGiveOnce = True, bNewUnitEvent = True, iMinLevel = 31, iMaxLevel = 37 } ) -- 30분, 계정, 5월31일이후, Lv.31~37
GameEvent:AddReward( 828, { fEventTime = 30, iRewardID = 852, bAccountEvent = True, bGiveOnce = True, bNewUnitEvent = True, iMinLevel = 38, iMaxLevel = 44 } ) -- 30분, 계정, 5월31일이후, Lv.38~44
GameEvent:AddReward( 828, { fEventTime = 30, iRewardID = 853, bAccountEvent = True, bGiveOnce = True, bNewUnitEvent = True, iMinLevel = 45, iMaxLevel = 51 } ) -- 30분, 계정, 5월31일이후, Lv.45~51
GameEvent:AddReward( 828, { fEventTime = 30, iRewardID = 854, bAccountEvent = True, bGiveOnce = True, bNewUnitEvent = True, iMinLevel = 52, iMaxLevel = 60 } ) -- 30분, 계정, 5월31일이후, Lv.52~60

-- 2013.07.10 ESJP_아라전야이벤트 30분->3분, 60분->6분
GameEvent:AddReward( 829, { fEventTime = 30, iRewardID = 855, bGiveOnce = True, iMinLevel = 10 } ) -- Lv10 이상 30분 접속 시 아라 가문의 문서 보관함 우편 지급(캐릭터 단위)
GameEvent:AddReward( 830, { fEventTime = 60, iRewardID = 856, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10 } ) -- Lv10 이상 60분 접속 시 봉인된 구미호 은큐브 우편 지급(계정 단위)

-- 2013.07.10 ESJP_샌더이벤트 
GameEvent:AddReward( 831, { fEventTime = 0, iRewardID = 859, bGiveOnce = True, iMinLevel = 10 }) -- Lv10 이상 최초 접속 시, 샌더마을 관광 가이드북 1개 지급
GameEvent:AddReward( 832, { fEventTime = 0, iRewardID = 860, bGiveOnce = True, iMinLevel = 63 } ) -- Lv63 이상 달성 시, 샌더 마을 정복자 큐브 1개 지급


-- 2013.07.31 ESJP_여름방학 이벤트 30분->3분, 60분->6분
GameEvent:AddReward( 833, { fEventTime = 30, iRewardID = 868, bGiveOnce = True } )  -- 7월 31일 30분 접속 시, 해적 코스프레 아바타 풀세트 큐브(2일권)
GameEvent:AddReward( 834, { fEventTime = 30, iRewardID = 869, bGiveOnce = True } )  -- 8월 01일 30분 접속 시, 축복받은 방어구 강화석 20개
GameEvent:AddReward( 835, { fEventTime = 30, iRewardID = 870, bGiveOnce = True } )  -- 8월 02일 30분 접속 시, 축복받은 무기 강화석 10개
GameEvent:AddReward( 836, { fEventTime = 60, iRewardID = 871, bGiveOnce = True, bAccountEvent = True} )  -- 8월 03일 30분 접속 시, 트레져 박스 큐브 1개
GameEvent:AddReward( 837, { fEventTime = 60, iRewardID = 872, bGiveOnce = True, bAccountEvent = True} )  -- 8월 04일 30분 접속 시, 아리엘의 축복 받은 복원의 주문서 교환권
GameEvent:AddReward( 838, { fEventTime = 30, iRewardID = 873, bGiveOnce = True } )  -- 8월 05일 30분 접속 시, 마법소녀 6세트 2일권 큐브
GameEvent:AddReward( 839, { fEventTime = 30, iRewardID = 874, bGiveOnce = True } )  -- 8월 06일 30분 접속 시, 마법소녀 악세사리 2세트 2일권 큐브

-- 2013.08.07 ESJP_제천 이벤트(사용법 틀림, 오로지 레벨만 됨. 절대금지)
GameEvent:AddReward( 840, { fEventTime = 0, iRewardID = 875, bGiveOnce = True, bAccountEvent = True, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 접속시, 아라응원 선물 1개
GameEvent:AddReward( 841, { fEventTime = 0, iRewardID = 876, bGiveOnce = True, bAccountEvent = True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 841, { fEventTime = 0, iRewardID = 876, bGiveOnce = True, bAccountEvent = True, cUnitType = UNIT_TYPE["UT_LIRE"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 841, { fEventTime = 0, iRewardID = 876, bGiveOnce = True, bAccountEvent = True, cUnitType = UNIT_TYPE["UT_ARME"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 841, { fEventTime = 0, iRewardID = 876, bGiveOnce = True, bAccountEvent = True, cUnitType = UNIT_TYPE["UT_RAVEN"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 841, { fEventTime = 0, iRewardID = 876, bGiveOnce = True, bAccountEvent = True, cUnitType = UNIT_TYPE["UT_EVE"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 841, { fEventTime = 0, iRewardID = 876, bGiveOnce = True, bAccountEvent = True, cUnitType = UNIT_TYPE["UT_CHUNG"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개


-- 2013.08.14 ESJP_샌더 마을에서의 여정
GameEvent:AddReward( 842, { fEventTime = 0, iRewardID = 877, bGiveOnce = True, } )  -- 샌더이벤트 보상 큐브(0분/캐릭터/1회)

-- 2013.08.14 ESJP_넷카페이벤트 (100분->10분)
GameEvent:AddReward( 843, { fEventTime = 0, iRewardID = 878, bGiveOnce = True, bPcBangEvent = True  } )  -- 넷카페 유저 서포터1 ( PC방 / 즉시 / 캐릭터)
GameEvent:AddReward( 844, { fEventTime = 100, iRewardID = 879, bGiveOnce = True, bPcBangEvent = True } )  -- 넷카페 유저 서포터2 ( PC방 / 100분 / 캐릭터)

-- 2013.08.28 ESJP_아이템 개편 이벤트 (60분->6분)
GameEvent:AddReward( 845, { fEventTime = 60, iRewardID = 882, bGiveOnce = True } )  -- 아이템 감정 증명서(한번/캐릭터/60분)
GameEvent:AddReward( 846, { fEventTime = 60, iRewardID = 883, bGiveOnce = True } )  -- 신 아이템 체험 증명서(한번/캐릭터/60분)

-- 2013.08.28 ESJP_개학 이벤트 (60분->6분, 30분->3분, )
GameEvent:AddReward( 847, { fEventTime = 60, iRewardID = 884, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자(월~금) (한번/캐릭터/60분)
GameEvent:AddReward( 848, { fEventTime = 60, iRewardID = 885, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자(월~화) (한번/캐릭터/60분)
GameEvent:AddReward( 849, { fEventTime = 30, iRewardID = 886, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠 (월) (한번/캐릭터/30분)
GameEvent:AddReward( 850, { fEventTime = 30, iRewardID = 887, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠 (화) (한번/캐릭터/30분)
GameEvent:AddReward( 851, { fEventTime = 30, iRewardID = 888, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠 (수) (한번/캐릭터/30분)
GameEvent:AddReward( 852, { fEventTime = 30, iRewardID = 889, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠 (목) (한번/캐릭터/30분)
GameEvent:AddReward( 853, { fEventTime = 30, iRewardID = 890, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠 (금) (한번/캐릭터/30분)

GameEvent:AddReward( 854, { fEventTime = 1, iRewardID = 891, bGiveOnce = True, iMinLevel = 20, iMaxLevel = 31 } )  -- 20레벨 10강 무기 큐브 (한번/캐릭터/1분)
GameEvent:AddReward( 854, { fEventTime = 1, iRewardID = 892, bGiveOnce = True, iMinLevel = 32, iMaxLevel = 43 } )  -- 32레벨 10강 무기 큐브 (한번/캐릭터/1분)
GameEvent:AddReward( 854, { fEventTime = 1, iRewardID = 893, bGiveOnce = True, iMinLevel = 44, iMaxLevel = 51 } )  -- 44레벨 10강 무기 큐브 (한번/캐릭터/1분)
GameEvent:AddReward( 854, { fEventTime = 1, iRewardID = 894, bGiveOnce = True, iMinLevel = 52, iMaxLevel = 59 } )  -- 52레벨 10강 무기 큐브 (한번/캐릭터/1분)
GameEvent:AddReward( 854, { fEventTime = 1, iRewardID = 895, bGiveOnce = True, iMinLevel = 60, iMaxLevel = 65 } )  -- 60레벨 10강 무기 큐브 (한번/캐릭터/1분)

GameEvent:AddReward( 855, { fEventTime = 1, iRewardID = 896, bGiveOnce = True } )  -- 주말 칭호(3일권) (한번/캐릭터/1분)


-- 2013.09.11 ESJP_어둠의 문 개편 이벤트( 30분->3분, )
GameEvent:AddReward( 856, { fEventTime = 30, iRewardID = 898, bGiveOnce = True } )  -- 방새로운 어둠의 등장에 대한 지침서 (한번/캐릭터/30분)

-- 2013.09.11 ESJP_실버위크 이벤트 ( 30분->3분, 60분->6분, 90분->9분, 120분->12분)
GameEvent:AddReward( 857, { fEventTime = 30, iRewardID = 899, bGiveOnce = True } )  -- 금성행 우주선 탑승권 (한번/캐릭터/30분)
GameEvent:AddReward( 858, { fEventTime = 60, iRewardID = 900, bGiveOnce = True } )  -- 금성행 우주선 탑승권 (한번/캐릭터/60분)
GameEvent:AddReward( 859, { fEventTime = 90, iRewardID = 901, bGiveOnce = True } )  -- 금성행 우주선 탑승권 (한번/캐릭터/90분)
GameEvent:AddReward( 860, { fEventTime = 120, iRewardID = 902, bGiveOnce = True } )  -- 금성행 우주선 탑승권 (한번/캐릭터/120분)

-- 2013.09.11 ESJP_13일의 금요일 저주 이벤트
GameEvent:AddReward( 861, { fEventTime = 13, iRewardID = 903, bGiveOnce = True } )  -- 제이슨 마스크(3일권) (한번/캐릭터/13분)


-- 2013.09.25 ESJP_할로윈 1차이벤트 (30분->3분)
GameEvent:AddReward( 862, { fEventTime = 3, iRewardID = 904, bGiveOnce = True } )  -- 신인 할로윈 디자이너 칭호(15일권) (한번/캐릭터/30분)
GameEvent:AddReward( 863, { fEventTime = 3, iRewardID = 905, bGiveOnce = True } )  -- 전설의 할로윈 디자이너 칭호(15일권) (한번/캐릭터/30분)

-- 2013.09.25 ESJP_탈 것 이벤트
GameEvent:AddReward( 864, { fEventTime = 10, iRewardID = 906, bGiveOnce = True } )  -- 나소드 모비(1일권) (한번/캐릭터/10분)
GameEvent:AddReward( 865, { fEventTime = 10, iRewardID = 907, bGiveOnce = True } )  -- 바람의 코우시카(1일권) (한번/캐릭터/10분)
GameEvent:AddReward( 866, { fEventTime = 10, iRewardID = 908, bGiveOnce = True } )  -- 에이션트 뽀루(1일권) (한번/캐릭터/10분)
GameEvent:AddReward( 867, { fEventTime = 10, iRewardID = 909, bGiveOnce = True } )  -- 나소드 모비(1일권), 바람의 코우시카(1일권),에이션트 뽀루(1일권) (한번/캐릭터/10분)
GameEvent:AddReward( 868, { fEventTime = 1, iRewardID = 910, bAccountEvent = True, bGiveOnce = True  } )  -- 고대 화석 판별기 (한번/계정/즉시)

-- 2013.09.25 ESJP_결혼 이벤트
GameEvent:AddReward( 869, { fEventTime = 10, iRewardID = 912, bGiveOnce = True } )  -- 가상 결혼 신청서 (한번/캐릭터/10분)


-- 2013.10.16 ESJP_복귀 유저와 성실 유저를 위한 선물
GameEvent:AddReward( 870, { fEventTime = 10, iRewardID = 914, bGiveOnce = True, bAccountEvent = True, bNewUnitEvent = True } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/8월28일이후 접속기록 없는 유저)
GameEvent:AddReward( 871, { fEventTime = 10, iRewardID = 915, bGiveOnce = True, bAccountEvent = True, bNewUnitEvent2 = True } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/8월28일이후 접속기록 있는 유저)

-- 2013.10.16 ESJP_복귀 유저를 위한 선물
-- 10/17 강화를 하자!(1)
GameEvent:AddReward( 872, { fEventTime = 10, iRewardID = 916, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 873, { fEventTime = 10, iRewardID = 917, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 874, { fEventTime = 10, iRewardID = 918, bGiveOnce = True, iMinLevel = 61, iMaxLevel = 65, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--10/18 강화를 하자!(2)
GameEvent:AddReward( 875, { fEventTime = 10, iRewardID = 919, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 876, { fEventTime = 10, iRewardID = 920, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 877, { fEventTime = 10, iRewardID = 921, bGiveOnce = True, iMinLevel = 61, iMaxLevel = 65, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--10/19 강화를 하자!(3)
GameEvent:AddReward( 878, { fEventTime = 10, iRewardID = 922, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 879, { fEventTime = 10, iRewardID = 923, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 880, { fEventTime = 10, iRewardID = 924, bGiveOnce = True, iMinLevel = 61, iMaxLevel = 65, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--10/20 감정을 해 보자!(1)
GameEvent:AddReward( 881, { fEventTime = 10, iRewardID = 925, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 882, { fEventTime = 10, iRewardID = 926, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 883, { fEventTime = 10, iRewardID = 927, bGiveOnce = True, iMinLevel = 61, iMaxLevel = 65, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--10/21 감정을 해 보자!(2)
GameEvent:AddReward( 884, { fEventTime = 10, iRewardID = 928, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 885, { fEventTime = 10, iRewardID = 929, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 886, { fEventTime = 10, iRewardID = 930, bGiveOnce = True, iMinLevel = 61, iMaxLevel = 65, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--10/22 감정을 해 보자!(3)
GameEvent:AddReward( 887, { fEventTime = 10, iRewardID = 931, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 888, { fEventTime = 10, iRewardID = 932, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 889, { fEventTime = 10, iRewardID = 933, bGiveOnce = True, iMinLevel = 61, iMaxLevel = 65, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--10/23 감정을 해 보자!(4)
GameEvent:AddReward( 890, { fEventTime = 10, iRewardID = 934, bGiveOnce = True, iMinLevel = 10, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 891, { fEventTime = 10, iRewardID = 935, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 892, { fEventTime = 10, iRewardID = 936, bGiveOnce = True, iMinLevel = 61, iMaxLevel = 65, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)

-- 2013.10.16 ESJP_성실 유저를 위한 선물
GameEvent:AddReward( 893, { fEventTime = 10, iRewardID = 937, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 894, { fEventTime = 10, iRewardID = 938, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 895, { fEventTime = 10, iRewardID = 939, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 896, { fEventTime = 10, iRewardID = 940, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 897, { fEventTime = 10, iRewardID = 941, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 898, { fEventTime = 10, iRewardID = 942, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)
GameEvent:AddReward( 899, { fEventTime = 10, iRewardID = 943, bGiveOnce = True, iEventItemID = 67006457 } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/성실유저인증티켓소지)

-- 2013.10.16 ESJP_수련을 시작하자
GameEvent:AddReward( 900, { fEventTime = 30, iRewardID = 944, bGiveOnce = True } )  -- 수련용 반지 (한번/캐릭터/30분)

-- 2013.10.23 ESJP_할로윈 이벤트
GameEvent:AddReward( 901, { fEventTime = 10, iRewardID = 951, bGiveOnce = True } )  -- 할로위치(15일권) (한번/캐릭터/10분)
GameEvent:AddReward( 902, { fEventTime = 0, iRewardID = 952, bGiveOnce = True } )  -- 할로윈 몬스터 코스프레 교환권 (한번/캐릭터/0분)
GameEvent:AddReward( 903, { fEventTime = 120, iRewardID = 953, bGiveOnce = True, bAccountEvent = True } )  -- 빛나는 할로윈 몬스터의 영혼 (한번/계정/120분)

-- 2013.11.06 ESJP_캐릭터 버닝 이벤트(남자)
GameEvent:AddReward( 904, { fEventTime = 10, iRewardID = 955, bGiveOnce = True, iEventItemID = 60007854 } )  -- 나쁜 남자의 MUST HAVE 아이템 A (한번/캐릭터/10분/이기적인 마음 소지시)
GameEvent:AddReward( 905, { fEventTime = 10, iRewardID = 956, bGiveOnce = True, iEventItemID = 60007854 } )  -- 나쁜 남자의 MUST HAVE 아이템 B (한번/캐릭터/10분/이기적인 마음 소지시)

-- 2013.11.20 ESJP_캐릭터 버닝 이벤트(여자)
GameEvent:AddReward( 906, { fEventTime = 10, iRewardID = 958, bGiveOnce = True, iEventItemID = 60007939 } )  -- 나쁜 여자의 MUST HAVE 아이템 A (한번/캐릭터/10분/이기적인 마음 소지시)
GameEvent:AddReward( 907, { fEventTime = 10, iRewardID = 959, bGiveOnce = True, iEventItemID = 60007939 } )  -- 나쁜 여자의 MUST HAVE 아이템 B (한번/캐릭터/10분/이기적인 마음 소지시)

-- 2013.12.11 ESJP_엘리시스 업데이트 이벤트
GameEvent:AddReward( 908, { fEventTime = 10, iRewardID = 961, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )  -- 엘소드 캐릭터 10분 접속시 신형 복사기 1개(매일)
GameEvent:AddReward( 909, { fEventTime = 10, iRewardID = 961, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_LIRE"] } )  -- 레나 캐릭터 10분 접속시 신형 복사기 1개(매일)
GameEvent:AddReward( 910, { fEventTime = 10, iRewardID = 961, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ARME"] } )  -- 아이샤 캐릭터 10분 접속시 신형 복사기 1개(매일)
GameEvent:AddReward( 911, { fEventTime = 10, iRewardID = 961, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_RAVEN"] } )  -- 레이븐 캐릭터 10분 접속시 신형 복사기 1개(매일)
GameEvent:AddReward( 912, { fEventTime = 10, iRewardID = 961, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_EVE"] } )  -- 이브 캐릭터 10분 접속시 신형 복사기 1개(매일)
GameEvent:AddReward( 913, { fEventTime = 10, iRewardID = 961, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_CHUNG"] } )  -- 청 캐릭터 10분 접속시 신형 복사기 1개(매일)
GameEvent:AddReward( 914, { fEventTime = 10, iRewardID = 961, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 캐릭터 10분 접속시 신형 복사기 1개(매일)

-- 2013.12.18 ESJP_엘리시스 1차 전직 이벤트
GameEvent:AddReward( 915, { fEventTime = 10, iRewardID = 966, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELESIS"] } )  -- 엘리시스 캐릭터 기사도 포션 2종 및 각성 포션 우편 지급(매일/계정/10분)
GameEvent:AddReward( 916, { fEXPRate = 1.0, iUnitLevel = 15, cUnitClass = UNIT_CLASS["UC_ELESIS_KNIGHT"] } )  -- 레벨 15이하 노전직 엘리시스 경험치 2배

-- 2013 크리스마스
GameEvent:AddReward( 2000, { fEventTime = 25, iRewardID = 2000, bGiveOnce = True, } )  -- 폴라 베어 코스프레 세트(한벌)
-- 2013 연말
GameEvent:AddReward( 2001, { fEventTime = 30, iRewardID = 2001, bGiveOnce = True, bAccountEvent = True, } )  -- 2013년 마지막 접속 인증 티켓
-- 2014 새해
GameEvent:AddReward( 2014, { fEventTime = 30, iRewardID = 2015, bGiveOnce = True, bAccountEvent = True } )  -- 포니 헤드 액세서리(3일권)

--------------------------------------------------------------------------
--봉헌의신전 입구 : 경험치, ED 2배
GameEvent:AddReward( 10000, 1.0, 0.0, 1.0, { iDungeonID = 32600, iUnitLevel = 0, cUnitClass = 0, iPartyNum = 0 } )
--나선회랑 : 경험치, ED 2배
GameEvent:AddReward( 10001, 1.0, 0.0, 1.0, { iDungeonID = 32700, iUnitLevel = 0, cUnitClass = 0, iPartyNum = 0 } )

--지하예배당 : 경험치, ED 2배
GameEvent:AddReward( 10002, 1.0, 0.0, 1.0, { iDungeonID = 32950, iUnitLevel = 0, cUnitClass = 0, iPartyNum = 0 } )

--지하정원 : 경험치, ED 2배
GameEvent:AddReward( 10003, 1.0, 0.0, 0.0, { iDungeonID = 32960, iUnitLevel = 0, cUnitClass = 0, iPartyNum = 0 } )

--35레벨 이하 : 경험치 2배
GameEvent:AddReward( 10004, { fEXPRate = 0.5, iUnitLevel = 35 } )

-- 엘소드2차 전직 이벤트
GameEvent:AddReward( 10005, { fEXPRate = 1.0, iUnitLevel = 34, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )  -- 엘소드 캐릭터 35렙 미만 경험치 2배

-- 레나2차 전직 이벤트
GameEvent:AddReward( 10006, { fEXPRate = 1.0, iUnitLevel = 34, cUnitType = UNIT_TYPE["UT_LIRE"] } )  -- 레나 캐릭터 35렙 이하 경험치 2배

-- 아이샤2차 전직 이벤트
GameEvent:AddReward( 10007, { fEXPRate = 1.0, iUnitLevel = 34, cUnitType = UNIT_TYPE["UT_ARME"] } )  -- 아이샤 캐릭터 35렙 이하 경험치 2배

-- 레이븐2차 전직 이벤트
GameEvent:AddReward( 10008, { fEXPRate = 1.0, iUnitLevel = 34, cUnitType = UNIT_TYPE["UT_RAVEN"] } )  -- 레이븐 캐릭터 35렙 이하 경험치 2배

-- 이브2차 전직 이벤트
GameEvent:AddReward( 10009, { fEXPRate = 1.0, iUnitLevel = 34, cUnitType = UNIT_TYPE["UT_EVE"] } )  -- 이브 캐릭터 35렙 이하 경험치 2배

-- 청2차 전직 이벤트
GameEvent:AddReward( 10010, { fEXPRate = 1.0, iUnitLevel = 34, cUnitType = UNIT_TYPE["UT_CHUNG"] } )  -- 청 캐릭터 35렙 이하 경험치 2배

-- 아라 전직 이벤트
GameEvent:AddReward( 10011, { fEXPRate = 0.5, iUnitLevel = 63, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 캐릭터 63렙 이하 경험치 1.5배

-- 아라2차 전직 이벤트
GameEvent:AddReward( 10012, { fEXPRate = 1.0, iUnitLevel = 34, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 캐릭터 35렙 이하 경험치 2배





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
GameEvent:AddReward( 2044, { fEventTime = 10, iRewardID = 2031, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 캐릭터 '소마를 도우는 힘' 큐브 우편 지급(매일/캐릭터/10분)

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

GameEvent:AddReward( 2058, { fEventTime = 35, iRewardID = 2034, bGiveOnce = True, iUnitClassLevel = 1, iMinLevel = 35, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 1차 35렙 캐릭터 World War 칭호(8일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2059, { fEventTime = 35, iRewardID = 2035, bGiveOnce = True, iUnitClassLevel = 1, iMinLevel = 35, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 1차 35렙 캐릭터 안티 포이즌 반지(3일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2060, { fEventTime = 35, iRewardID = 2036, bGiveOnce = True, iUnitClassLevel = 1, iMinLevel = 35, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 1차 35렙 캐릭터 소환석 : 돌진 전차 모비RT (3일권) 1개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2061, { fEventTime = 35, iRewardID = 2037, bGiveOnce = True, iUnitClassLevel = 1, iMinLevel = 35, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 1차 35렙 캐릭터 루리엘의 플루오르스톤 교환권 1개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2062, { fEventTime = 35, iRewardID = 2038, bGiveOnce = True, iUnitClassLevel = 1, iMinLevel = 35, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 1차 35렙 캐릭터 2차 전직 프로모션 아바타 무기(포이즈닝)(3일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2063, { fEventTime = 35, iRewardID = 2039, bGiveOnce = True, iUnitClassLevel = 1, iMinLevel = 35, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 1차 35렙 캐릭터 선택 스킬 초기화 3개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2064, { fEventTime = 35, iRewardID = 2040, bGiveOnce = True, iUnitClassLevel = 1, iMinLevel = 35, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 1차 35렙 캐릭터 루리엘의 활력의 포션 10개, 루리엘의 마나 에릭실 10개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2065, { fEventTime = 35, iRewardID = 2041, bGiveOnce = True, iUnitClassLevel = 1, iMinLevel = 35, cUnitType = UNIT_TYPE["UT_ELESIS"] } ) -- 엘리시스 1차 35렙 캐릭터 부활석 3개 큐브 1개 우편 지급(매일/캐릭터/35분)


-- 2014 아라 신전직 전야 이벤트
GameEvent:AddReward( 2041, { fEventTime = 10, iRewardID = 2043, bGiveOnce = True} ) -- 미약한 사파의 정수 2개 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2042, { fEventTime = 10, iRewardID = 2044, bGiveOnce = True, iEventItemID = 85003833 } )	-- 정파 비전 큐브 열쇠 1개 우편 지급(한번/캐릭터/10분/정파비전 큐브 보유시)
GameEvent:AddReward( 2043, { fEventTime = 10, iRewardID = 2045, bGiveOnce = True, iEventItemID = 85003834 } )	-- 사파 비전 큐브 열쇠 1개 우편 지급(한번/캐릭터/10분/사파비전 큐브 보유시)

-- 궁극의 힘
GameEvent:AddReward( 2046, { fEventTime =   10,	iRewardID = 2046,	iMinLevel = 10, } )	-- 루리엘의 마나 활성화 포션 등
GameEvent:AddReward( 2047, { fEventTime =   20,	iRewardID = 2047,	iEventItemID = 60008063,  } )	-- 루리엘의 마나 활성화 포션 등

-- 20140205 명왕 전야
GameEvent:AddReward( 2049, { fEventTime =   10,	iRewardID = 2049,	cUnitClass = UNIT_CLASS["UC_ARA_LITTLE_DEVIL"], } )	-- 루리엘의 마나 활성화 포션 등
GameEvent:AddReward( 2050, { fEventTime =   35,	iRewardID = 2050,	iMinLevel = 10, } )	-- 사파의 정수 5개
GameEvent:AddReward( 2051, { fEventTime =   35,	iRewardID = 2051,	iMinLevel = 10, } )	-- 루리엘의 마나 에릭실 5개
GameEvent:AddReward( 2052, { fEventTime =   35,	iRewardID = 2052,	iMinLevel = 10, } )	-- 루리엘의 각성 포션 5개
GameEvent:AddReward( 2053, { fEventTime =   35,	iRewardID = 2053,	iMinLevel = 10, } )	-- 루리엘의 펫 시리얼 5개
GameEvent:AddReward( 2054, { fEventTime =   35,	iRewardID = 2054,	iMinLevel = 10, } )	-- 루리엘의 망각의 알약 1개
GameEvent:AddReward( 2055, { fEventTime =   35,	iRewardID = 2055,	iMinLevel = 10, } )	-- 루리엘의 활력의 포션 5개
GameEvent:AddReward( 2056, { fEventTime =   35,	iRewardID = 2056,	iMinLevel = 10, } )	-- 루리엘의 축복받은 시공간의 주문서 교환권 1개
GameEvent:AddReward( 2057, { fEventTime =   35,	iRewardID = 2057,	iMinLevel = 10, } )	-- 부활석 (5개)큐브

-- 2014년 발렌타인데이 이벤트
GameEvent:AddReward( 10328,	{ fEventTime =  14,	iRewardID = 1000325, bGiveOnce =  True, iMinLevel = 10 } ) -- 헬렌의 발렌타인데이 초콜릿 5개
GameEvent:AddReward( 10329,	{ fEventTime =  14,	iRewardID = 1000326, bGiveOnce =  True, iMinLevel = 10 } ) -- 기적의 발렌타인데이 반지(Lv.1)

-- 샌더 5,6던전 이벤트  샌더 마을의 서신
GameEvent:AddReward( 2066, { fEventTime = 60, iRewardID = 2066, bGiveOnce = True, iMinLevel = 10 } ) -- 풍화된 샌더 마을의 서신 조각 1개, 엘의 조각(윈드) 1개

-- 4주년 이벤트
GameEvent:AddReward( 2067, { fEventTime = 10, iRewardID = 2067, bGiveOnce = True, bNewUnitEvent = True } )	-- ComeBack 럭키백
GameEvent:AddReward( 2068, { fEventTime = 10, iRewardID = 2068, bGiveOnce = True, bNewUnitEvent2 = True } )	-- Special 럭키백

--진입구조 개편 ---------------------
GameEvent:AddReward( 10410, { fEventTime =  10,	iRewardID = 10410, bGiveOnce =  True, iMinLevel = 10 } ) -- 대리 판매 보증서(VIP, 3일권)


---------------------------------------------------------------------------------------------------
--공지 이벤트 날릴 센터서버UID
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_SOLES"],  1 ) -- 일본

--GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_SOLES"],  101 ) -- 솔레스
--GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_GAIA"], 301 )  -- 가이아
--GameEvent:AddConnectEventUserGroup(	0	,	10931	)	-- 테스트 용