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
GameEvent:AddReward( 259, { fDropRate =  2.0, bWithPlayPcBang = True } ) -- PC방 유저와 함께 플레이시 드롭률 2배


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
GameEvent:AddReward( 267, { fDropRate =  2.0 } ) -- 드롭률 2배


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


-- 엘소드 엑스포 이벤트
GameEvent:AddReward( 297, { fEventTime =  30, iRewardID = 10081 } ) -- EWE 기념 큐브 1개
GameEvent:AddReward( 298, { fEventTime =   0, iRewardID = 10082 } ) -- 1st EWE 1개


-- 다시시작하고싶습니다 이벤트
GameEvent:AddReward( 299, { fEventTime =  30, iRewardID = 10083 } ) -- 카밀라의 선물 1개
GameEvent:AddReward( 300, { fEventTime =  30, iRewardID = 10084 } ) -- 망각의 드링크(7일권) 큐브 1개


-- 3주년 이벤트
GameEvent:AddReward( 301, { fEventTime =  30, iRewardID = 10085 } ) -- 산타 양말 1개
GameEvent:AddReward( 302, { fEventTime =  60, iRewardID = 10086 } ) -- 악세사리 1개권 랜덤 큐브
GameEvent:AddReward( 303, { fEventTime =  60, iRewardID = 10087 } ) -- 아리엘의 얼음 조각상 가열기 1개
GameEvent:AddReward( 304, { fEventTime =  60, iRewardID = 10088 } ) -- 강화 부적 랜덤 큐브(lv5 ~ lv9) 1개
GameEvent:AddReward( 305, { fEventTime =   0, iRewardID = 10089 } ) -- 신묘한 행운을 불러오는 토끼


-- 추위를 녹이는 가이아의 열기 이벤트
GameEvent:AddReward( 306, { fEXPRate = 1.0, iUnitLevel = 34 } )  -- 35렙 미만 경험치 2배


-- 비밀던전 무료입장
GameEvent:AddReward( 307, { iDungeonID = DUNGEON_ID["DI_ELDER_SECRET_COMMON"],	iDungeonMode = DUNGEON_MODE["DM_SECRET_NORMAL"]		} )
GameEvent:AddReward( 308, { iDungeonID = DUNGEON_ID["DI_BESMA_SECRET_COMMON"],	iDungeonMode = DUNGEON_MODE["DM_SECRET_NORMAL"]		} )
GameEvent:AddReward( 309, { iDungeonID = DUNGEON_ID["DI_ALTERA_SECRET_COMMON"],	iDungeonMode = DUNGEON_MODE["DM_SECRET_NORMAL"]		} )

GameEvent:AddReward( 310, { iDungeonID = DUNGEON_ID["DI_ELDER_SECRET_HELL"],	iDungeonMode = DUNGEON_MODE["DM_SECRET_HELL"]		} )
GameEvent:AddReward( 311, { iDungeonID = DUNGEON_ID["DI_BESMA_SECRET_HELL"],	iDungeonMode = DUNGEON_MODE["DM_SECRET_HELL"]		} )
GameEvent:AddReward( 312, { iDungeonID = DUNGEON_ID["DI_ALTERA_SECRET_HELL"],	iDungeonMode = DUNGEON_MODE["DM_SECRET_HELL"]		} )

GameEvent:AddReward( 313, { iDungeonID = DUNGEON_ID["DI_ELDER_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_PRACTICE"]	} )
GameEvent:AddReward( 314, { iDungeonID = DUNGEON_ID["DI_BESMA_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_PRACTICE"]	} )
GameEvent:AddReward( 315, { iDungeonID = DUNGEON_ID["DI_ALTERA_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_PRACTICE"]	} )
GameEvent:AddReward( 316, { iDungeonID = DUNGEON_ID["DI_FEITA_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_PRACTICE"]	} )
GameEvent:AddReward( 317, { iDungeonID = DUNGEON_ID["DI_VELDER_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_PRACTICE"]	} )

GameEvent:AddReward( 318, { iDungeonID = DUNGEON_ID["DI_ELDER_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_CHALLENGE"]	} )
GameEvent:AddReward( 319, { iDungeonID = DUNGEON_ID["DI_BESMA_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_CHALLENGE"]	} )
GameEvent:AddReward( 320, { iDungeonID = DUNGEON_ID["DI_ALTERA_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_CHALLENGE"]	} )
GameEvent:AddReward( 321, { iDungeonID = DUNGEON_ID["DI_FEITA_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_CHALLENGE"]	} )
GameEvent:AddReward( 322, { iDungeonID = DUNGEON_ID["DI_VELDER_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_CHALLENGE"]	} )


-- 플루오네의 은총(2011/1/27~2011/2/9)
GameEvent:AddReward( 323, { fEventTime =  30, iRewardID = 10090, bAccountEvent = True } ) -- 1/27 마나민트 츄잉껌(7일) 1EA
GameEvent:AddReward( 324, { fEventTime =  30, iRewardID = 10091, bAccountEvent = True } ) -- 1/28 뽀루의 발 도장(하얀색) 1EA
GameEvent:AddReward( 325, { fEventTime =  30, iRewardID = 10092, bAccountEvent = True } ) -- 1/29 플루오르 스톤 교환권 1EA
GameEvent:AddReward( 326, { fEventTime =  30, iRewardID = 10093, bAccountEvent = True } ) -- 1/30 망각의 알약 큐브 1EA
GameEvent:AddReward( 327, { fEventTime =  30, iRewardID = 10094, bAccountEvent = True } ) -- 1/31 코보 익스프레스 VIP 티켓(7일)1EA
GameEvent:AddReward( 328, { fEventTime =  30, iRewardID = 10095, bAccountEvent = True } ) -- 2/1 액세서리 큐브(얼굴 3일) 1EA, 액세서리 큐브(상의 3일) 1EA
GameEvent:AddReward( 329, { fEventTime =  30, iRewardID = 10096, bAccountEvent = True } ) -- 2/2 2011 한복 아바타 큐브(1일) 1EA
GameEvent:AddReward( 330, { fEventTime =  30, iRewardID = 10097, bAccountEvent = True } ) -- 2/3 일급 마법석 랜덤 큐브 1EA
GameEvent:AddReward( 331, { fEventTime =  30, iRewardID = 10098, bAccountEvent = True } ) -- 2/4 특별 출입증 교환권 1장
GameEvent:AddReward( 332, { fEventTime =  30, iRewardID = 10099, bAccountEvent = True } ) -- 2/5 뽀루의 발 도장 (황금) 1EA
GameEvent:AddReward( 333, { fEventTime =  30, iRewardID = 10100, bAccountEvent = True } ) -- 2/6 축복받은 복원의 주문서 교환권 1EA
GameEvent:AddReward( 334, { fEventTime =  30, iRewardID = 10101, bAccountEvent = True } ) -- 2/7 프로모션 아바타 큐브(15일) 1EA
GameEvent:AddReward( 335, { fEventTime =  30, iRewardID = 10102, bAccountEvent = True } ) -- 2/8 경험치 메달 큐브(1일) 1EA
GameEvent:AddReward( 336, { fEventTime =  30, iRewardID = 10103, bAccountEvent = True } ) -- 2/9 뽀루의 발 도장(보라) 1EA

-- 아리엘의 설날 선물(2011/2/5 12:00~14:00)
GameEvent:AddReward( 337, { fEventTime =  60,  iRewardID = 10104, bAccountEvent = True } ) -- 2/5 얼음각상 가열기(아리엘의 선물) 1EA

-- 비밀던전 무료입장
GameEvent:AddReward( 338, { iDungeonID = DUNGEON_ID["DI_HAMEL_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_PRACTICE"]	} )
GameEvent:AddReward( 339, { iDungeonID = DUNGEON_ID["DI_HAMEL_HENIR_SPACE"],	iDungeonMode = DUNGEON_MODE["DM_HENIR_CHALLENGE"]	} )

-- 발렌타인 데이 준비물(2011/2/10 ~ 2011/2/16)
GameEvent:AddReward( 340, { fEventTime =  50,  iRewardID = 10105 } ) -- 2/10~2/16 아리엘의 발렌타인 선물권 1EA

-- 불편을 끼쳐드려 죄송합니다~OTL(2011/2/17 ~ 2011/2/23) 
-- 이벤트 타입 7번
GameEvent:AddReward( 341, { fEventTime =  0,  iRewardID = 10106, bAccountEvent = True } ) -- 2/17~ 2/23 얼음 조각상 가열기 (아리엘의 선물)1EA
GameEvent:AddReward( 342, { fEventTime =  0,  iRewardID = 10107, bAccountEvent = True } ) -- 2/19 기술의 반지(15일)1EA
GameEvent:AddReward( 343, { fEventTime =  0,  iRewardID = 10108, bAccountEvent = True } ) -- 2/20 마법의 목걸이(15일)1EA
GameEvent:AddReward( 344, { fEventTime =  0,  iRewardID = 10109 } )						  -- 2/17 경험치 메달 큐브(1일권)1EA
GameEvent:AddReward( 345, { fEventTime =  0,  iRewardID = 10109 } )						  -- 2/18 경험치 메달 큐브(1일권)1EA
GameEvent:AddReward( 346, { fEventTime =  0,  iRewardID = 10109 } )						  -- 2/19 경험치 메달 큐브(1일권)1EA
GameEvent:AddReward( 347, { fEventTime =  0,  iRewardID = 10109 } )						  -- 2/20 경험치 메달 큐브(1일권)1EA
GameEvent:AddReward( 348, { fEventTime =  0,  iRewardID = 10109 } )						  -- 2/21 경험치 메달 큐브(1일권)1EA
GameEvent:AddReward( 349, { fEventTime =  0,  iRewardID = 10109 } )						  -- 2/22 경험치 메달 큐브(1일권)1EA
GameEvent:AddReward( 350, { fEventTime =  0,  iRewardID = 10109 } )						  -- 2/23 경험치 메달 큐브(1일권)1EA

-- 공존의 축제 의상 큐브 지급(3/17~31)
GameEvent:AddReward( 351, { fEventTime =  0,  iRewardID = 10110 } )						  -- 3/17~31 공존의 축제 의상 큐브1EA

-- 공존의 축제 주말 하이라이트 이벤트 (3/19~20, 3/26~27)  12시 ~ 13시
GameEvent:AddReward( 352, { fEventTime =  0,  iRewardID = 10111, bAccountEvent = True  } )		-- 3/19,20,26,27 12~13시 공존의 축제 큐브 1EA

-- 식목일 이벤트 엘라보르 주중
GameEvent:AddReward( 353, { fEventTime =   0,  iRewardID = 10112 } )							-- 3/31,4/1,4/4~4/8,4/11~4/15,4/18~4/21(매일)   자연의 선물 3EA
-- 식목일 이벤트 엘라보르 주말
GameEvent:AddReward( 354, { fEventTime =   0,  iRewardID = 10113 } )							-- 4/2,3,9,10,16,17(매일)						자연의 선물 5EA
-- 식목일 이벤트 엘라보르 PC방 추가지급
GameEvent:AddReward( 355, { fEventTime =   0,  iRewardID = 10114, bPcBangEvent = True } )		-- 3/31~4/21(매일)								자연의 선물 2EA

-- 청 케릭터 플레이 추가 경험치
GameEvent:AddReward( 356, { fEXPRate = 0.2, cUnitType = UNIT_TYPE["UT_CHUNG"] } )				-- 4/21 ~ 4/28        청 캐릭터 경험치 20% 추가
-- 경보!마족의 엘리오스 침공
GameEvent:AddReward( 357, { fEventTime =   0,  iRewardID = 10115 } )							-- 4/21 ~ 4/28		  퓨어 베놈 7일 1EA
-- 벨더 아카데미 시험 주간
GameEvent:AddReward( 358, { fEventTime =   0,  iRewardID = 10116 } )							-- 4/21 ~ 5/12		  기묘한 노트 큐브 1EA
GameEvent:AddReward( 359, { fEventTime = 120,  iRewardID = 10117  } )							-- 4/21 ~ 5/12		  벨더 아카데미 시험지(100점)  1EA

-- 넌 누구니?
GameEvent:AddReward( 360, { fEventTime =  30,  iRewardID = 10118 } )							-- 4/21 ~ 4/28		  엘의 나무 씨앗 1EA

-- 어린이 날 선물
GameEvent:AddReward( 361, { fEventTime =   0,  iRewardID = 10119, bAccountEvent = True } )		-- 5/5   얼음 조각상 가열기 풀세트 큐브(1일권) 1EA
GameEvent:AddReward( 362, { fEventTime =   0,  iRewardID = 10120, bAccountEvent = True } )		-- 5/6   플루오르 스톤 교환권 1EA
GameEvent:AddReward( 363, { fEventTime =   0,  iRewardID = 10121, bAccountEvent = True } )		-- 5/7   액세서리 큐브 (팔 무기 반지 목걸이 : 1일권) 1EA
GameEvent:AddReward( 364, { fEventTime =   0,  iRewardID = 10122, bAccountEvent = True } )		-- 5/8   뽀루의 발 도장 (보라색) 1EA
GameEvent:AddReward( 365, { fEventTime =   0,  iRewardID = 10123, bAccountEvent = True } )		-- 5/9   특수 액세서리 4종 큐브(1일권) 1EA
GameEvent:AddReward( 366, { fEventTime =   0,  iRewardID = 10124, bAccountEvent = True } )		-- 5/10  한정 아바타 풀세트 큐브(1일권) 1EA
GameEvent:AddReward( 367, { fEventTime =   0,  iRewardID = 10125, bAccountEvent = True } )		-- 5/11  뽀루의 발 도장 (하얀색) 1EA


-- 용사의 성장 이벤트
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10126, iMinLevel = 1,	iMaxLevel = 13 } )		-- 숲의 수호자 무기 큐브 1개
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10127, iMinLevel = 14,	iMaxLevel = 19 } )		-- 시민의 수호자 무기 큐브 1개
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10128, iMinLevel = 20,	iMaxLevel = 25 } )		-- 파멸의 용병 무기 큐브 1개
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10129, iMinLevel = 26,	iMaxLevel = 29 } )		-- 분노의 화신 무기 큐브 1개
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10130, iMinLevel = 30,	iMaxLevel = 33 } )		-- 문명의 제압자 무기 큐브 1개
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10131, iMinLevel = 34,	iMaxLevel = 37 } )		-- 마도대전 용사의 무기 큐브 1개
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10132, iMinLevel = 38,	iMaxLevel = 45 } )		-- 블랙스미스 연합 무기 큐브 1개
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10133, iMinLevel = 46,	iMaxLevel = 49 } )		-- 혈투의 노래 무기 큐브 1개
GameEvent:AddReward( 368, { fEventTime =   0,  iRewardID = 10134, iMinLevel = 50,	iMaxLevel = 56 } )		-- 빛의 수호자 무기 큐브 1개

GameEvent:AddReward( 369, { fEXPRate = 0.3 } )													-- 6/2~6/30 평일(월~금)     경험치  30% 증가
GameEvent:AddReward( 370, { fEXPRate = 1.0 } )													-- 6/2~6/30 주말(토,일)     경험치 100% 증가

-- 엘리오스 데일리 파티
GameEvent:AddReward( 371, { fEventTime =   0,  iRewardID = 10136, bAccountEvent = True } )		-- 6/23, 6/30  액세서리 큐브 (팔 무기 반지 목걸이 : 1일권)
GameEvent:AddReward( 372, { fEventTime =   0,  iRewardID = 10137, bAccountEvent = True } )		-- 6/24, 7/1   부활석(3개) 큐브 1개
GameEvent:AddReward( 373, { fEventTime =   0,  iRewardID = 10138, bAccountEvent = True } )		-- 6/25, 7/2   행운의 엘의 조각 큐브 1개
GameEvent:AddReward( 374, { fEventTime =   0,  iRewardID = 10139, bAccountEvent = True } )		-- 6/26, 7/3   뽀루의 발 도장(보라색) 1개
GameEvent:AddReward( 375, { fEventTime =   0,  iRewardID = 10140, bAccountEvent = True } )		-- 6/27, 7/4   특수 액세서리 4종 큐브(1일권)
GameEvent:AddReward( 376, { fEventTime =   0,  iRewardID = 10141, bAccountEvent = True } )		-- 6/28, 7/5   드림 스텝(1일권) 랜덤 큐브
GameEvent:AddReward( 377, { fEventTime =   0,  iRewardID = 10142, bAccountEvent = True } )		-- 6/29, 7/6   뽀루의 발 도장(하얀색) 1개

-- 코보 주식회사의 모험가 캠페인
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10146	, iMinLevel = 	1	,   iMaxLevel =	1	 } )	-- [레벨 	1	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10147	, iMinLevel = 	2	,   iMaxLevel =	2	 } )	-- [레벨 	2	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10148	, iMinLevel = 	3	,   iMaxLevel =	3	 } )	-- [레벨 	3	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10149	, iMinLevel = 	4	,   iMaxLevel =	4	 } )	-- [레벨 	4	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10150	, iMinLevel = 	5	,   iMaxLevel =	5	 } )	-- [레벨 	5	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10151	, iMinLevel = 	6	,   iMaxLevel =	6	 } )	-- [레벨 	6	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10152	, iMinLevel = 	7	,   iMaxLevel =	7	 } )	-- [레벨 	7	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10153	, iMinLevel = 	8	,   iMaxLevel =	8	 } )	-- [레벨 	8	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10154	, iMinLevel = 	9	,   iMaxLevel =	9	 } )	-- [레벨 	9	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10155	, iMinLevel = 	10	,   iMaxLevel =	10	 } )	-- [레벨 	10	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10156	, iMinLevel = 	11	,   iMaxLevel =	11	 } )	-- [레벨 	11	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10157	, iMinLevel = 	12	,   iMaxLevel =	12	 } )	-- [레벨 	12	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10158	, iMinLevel = 	13	,   iMaxLevel =	13	 } )	-- [레벨 	13	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10159	, iMinLevel = 	14	,   iMaxLevel =	14	 } )	-- [레벨 	14	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10160	, iMinLevel = 	15	,   iMaxLevel =	15	 } )	-- [레벨 	15	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10161	, iMinLevel = 	16	,   iMaxLevel =	16	 } )	-- [레벨 	16	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10162	, iMinLevel = 	17	,   iMaxLevel =	17	 } )	-- [레벨 	17	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10163	, iMinLevel = 	18	,   iMaxLevel =	18	 } )	-- [레벨 	18	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10164	, iMinLevel = 	19	,   iMaxLevel =	19	 } )	-- [레벨 	19	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10165	, iMinLevel = 	20	,   iMaxLevel =	20	 } )	-- [레벨 	20	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10166	, iMinLevel = 	21	,   iMaxLevel =	21	 } )	-- [레벨 	21	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10167	, iMinLevel = 	22	,   iMaxLevel =	22	 } )	-- [레벨 	22	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10168	, iMinLevel = 	23	,   iMaxLevel =	23	 } )	-- [레벨 	23	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10169	, iMinLevel = 	24	,   iMaxLevel =	24	 } )	-- [레벨 	24	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10170	, iMinLevel = 	25	,   iMaxLevel =	25	 } )	-- [레벨 	25	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10171	, iMinLevel = 	26	,   iMaxLevel =	26	 } )	-- [레벨 	26	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10172	, iMinLevel = 	27	,   iMaxLevel =	27	 } )	-- [레벨 	27	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10173	, iMinLevel = 	28	,   iMaxLevel =	28	 } )	-- [레벨 	28	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10174	, iMinLevel = 	29	,   iMaxLevel =	29	 } )	-- [레벨 	29	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10175	, iMinLevel = 	30	,   iMaxLevel =	30	 } )	-- [레벨 	30	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10176	, iMinLevel = 	31	,   iMaxLevel =	31	 } )	-- [레벨 	31	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10177	, iMinLevel = 	32	,   iMaxLevel =	32	 } )	-- [레벨 	32	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10178	, iMinLevel = 	33	,   iMaxLevel =	33	 } )	-- [레벨 	33	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10179	, iMinLevel = 	34	,   iMaxLevel =	34	 } )	-- [레벨 	34	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10180	, iMinLevel = 	35	,   iMaxLevel =	35	 } )	-- [레벨 	35	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10181	, iMinLevel = 	36	,   iMaxLevel =	36	 } )	-- [레벨 	36	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10182	, iMinLevel = 	37	,   iMaxLevel =	37	 } )	-- [레벨 	37	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10183	, iMinLevel = 	38	,   iMaxLevel =	38	 } )	-- [레벨 	38	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10184	, iMinLevel = 	39	,   iMaxLevel =	39	 } )	-- [레벨 	39	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10185	, iMinLevel = 	40	,   iMaxLevel =	40	 } )	-- [레벨 	40	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10186	, iMinLevel = 	41	,   iMaxLevel =	41	 } )	-- [레벨 	41	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10187	, iMinLevel = 	42	,   iMaxLevel =	42	 } )	-- [레벨 	42	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10188	, iMinLevel = 	43	,   iMaxLevel =	43	 } )	-- [레벨 	43	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10189	, iMinLevel = 	44	,   iMaxLevel =	44	 } )	-- [레벨 	44	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10190	, iMinLevel = 	45	,   iMaxLevel =	45	 } )	-- [레벨 	45	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10191	, iMinLevel = 	46	,   iMaxLevel =	46	 } )	-- [레벨 	46	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10192	, iMinLevel = 	47	,   iMaxLevel =	47	 } )	-- [레벨 	47	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10193	, iMinLevel = 	48	,   iMaxLevel =	48	 } )	-- [레벨 	48	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10194	, iMinLevel = 	49	,   iMaxLevel =	49	 } )	-- [레벨 	49	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 378, { fEventTime =   0,  iRewardID = 	10195	, iMinLevel = 	50	,   iMaxLevel =	60	 } )	-- [레벨 	50	] 코보 모험가 서포트 큐브

-- 네이버 프로모션 이벤트
GameEvent:AddReward( 379, { fEventTime =  10,  iRewardID = 	10196	, 	bAccountEvent = True } )	-- 한여름 대박 큐브

-- 긴급 이벤트
GameEvent:AddReward( 380, { fEventTime =   0,  iRewardID = 	10197	, 	bAccountEvent = True } )	-- 얼음 조각상 가열기 1개

-- 네이버 프로모션 드롭 300%
GameEvent:AddReward( 381, { fDropRate =  3.0 } ) -- 드롭률 3배

-- 네이버 프로모션 얼음 조각상 가열기 풀 세트(15일권) 큐브
GameEvent:AddReward( 382, { fEventTime =  10,  iRewardID = 	10198	, 	bAccountEvent = True } ) -- 얼음 조각상 가열기 풀 세트(15일권) 큐브

-- 물의 신전
GameEvent:AddReward( 383, { fEventTime =   5,  iRewardID = 	10199    } ) -- 하멜의 마법석 큐브 

-- 엘리오스 데일리 파티 이벤트
GameEvent:AddReward( 384, { fEventTime =   5,  iRewardID = 	10200,	bAccountEvent = True } ) -- 목요일 (특전)여름 악세사리 큐브 1개
GameEvent:AddReward( 385, { fEventTime =   5,  iRewardID = 	10201,	bAccountEvent = True } ) -- 금요일 부활석(3개)큐브 1개
GameEvent:AddReward( 386, { fEventTime =   5,  iRewardID = 	10202,	bAccountEvent = True } ) -- 토요일 행운의 엘의 조각 큐브 1개
GameEvent:AddReward( 387, { fEventTime =   5,  iRewardID = 	10203,	bAccountEvent = True } ) -- 일요일 뽀루의 발 도장(보라색) 1개
GameEvent:AddReward( 388, { fEventTime =   5,  iRewardID = 	10204,	bAccountEvent = True } ) -- 월요일 특수 악세사리(5일권)큐브 1개
GameEvent:AddReward( 389, { fEventTime =   5,  iRewardID = 	10205,	bAccountEvent = True } ) -- 화요일 기술의 반지(5일권) 큐브 1개
GameEvent:AddReward( 390, { fEventTime =   5,  iRewardID = 	10206,	bAccountEvent = True } ) -- 수요일 뽀루의 발도장(하얀색) 1개

-- 운명의 큐브
GameEvent:AddReward( 391, { fEventTime =   0,  iRewardID = 	10207    } )					-- 운명의 큐브 

-- 헤니르의 비밀 열쇠
GameEvent:AddReward( 392, { fEventTime =   0,  iRewardID = 	10208    } )					-- 헤니르의 열쇠 
GameEvent:AddReward( 393, { fEventTime =  30,  iRewardID = 	10209    } )					-- 헤니르의 비밀 열쇠(누적)

-- 엘리오스 데일리 파티 이벤트
GameEvent:AddReward( 394, { fEventTime =   0,  iRewardID = 	10210,	bAccountEvent = True } ) -- 목요일 
GameEvent:AddReward( 395, { fEventTime =   0,  iRewardID = 	10211,	bAccountEvent = True } ) -- 금요일 
GameEvent:AddReward( 396, { fEventTime =   0,  iRewardID = 	10212,	bAccountEvent = True } ) -- 토요일 
GameEvent:AddReward( 397, { fEventTime =   0,  iRewardID = 	10213,	bAccountEvent = True } ) -- 일요일 
GameEvent:AddReward( 398, { fEventTime =   0,  iRewardID = 	10214,	bAccountEvent = True } ) -- 월요일 
GameEvent:AddReward( 399, { fEventTime =   0,  iRewardID = 	10215,	bAccountEvent = True } ) -- 화요일 
GameEvent:AddReward( 400, { fEventTime =   0,  iRewardID = 	10216,	bAccountEvent = True } ) -- 수요일 

GameEvent:AddReward( 401, { fEventTime =   0,  iRewardID = 	10210,	bAccountEvent = True } ) -- 목요일 
GameEvent:AddReward( 402, { fEventTime =   0,  iRewardID = 	10211,	bAccountEvent = True } ) -- 금요일 
GameEvent:AddReward( 403, { fEventTime =   0,  iRewardID = 	10212,	bAccountEvent = True } ) -- 토요일 
GameEvent:AddReward( 404, { fEventTime =   0,  iRewardID = 	10213,	bAccountEvent = True } ) -- 일요일 
GameEvent:AddReward( 405, { fEventTime =   0,  iRewardID = 	10214,	bAccountEvent = True } ) -- 월요일 
GameEvent:AddReward( 406, { fEventTime =   0,  iRewardID = 	10215,	bAccountEvent = True } ) -- 화요일 
GameEvent:AddReward( 407, { fEventTime =   0,  iRewardID = 	10216,	bAccountEvent = True } ) -- 수요일 

-- 케로로의 엘리오스 참전!
GameEvent:AddReward( 408, { fEventTime =   0,  iRewardID = 	10217,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"] } )		-- 케로로UFO 미니미 악세사리

-- 엘리오스 수호자들에게 경례!
GameEvent:AddReward( 409, { fEventTime =   0,  iRewardID = 	10218,	iChannelCode = CHANNELING_ENUM["CE_NEXON_ACCOUNT"] } )			-- 세나스 무공 훈장 악세사리

-- (헤니르 보상 획득 횟수)
GameEvent:AddReward( 410, { iHenirRewardEventCount = 3 } )			-- 헤니르 보상 획득 3회 추가
-- (헤니르 보상 획득 무제한)
GameEvent:AddReward( 411, { bHenirRewardUnLimited = True } )		-- 헤니르 보상 획득 무제한

-- 루토의 계약
GameEvent:AddReward( 412, { fEventTime =   0,  iRewardID = 	10225 } )			-- 비밀 던전 입장권 큐브 1개

-- 최후의 결전(넌 누구니?)
GameEvent:AddReward( 413, { fEventTime =   0,  iRewardID = 	10226 } )			-- 엘의 씨앗 1개

--경험치 1.2배
GameEvent:AddReward( 414, { fEXPRate = 1.2 } )

-- 즐거운 한가위 되세요
GameEvent:AddReward( 415, { fEventTime =   0,  iRewardID = 	10228 } )			-- 한가위 다과 선물 세트 큐브 1개

-- PC방을 찾아온 VIP 모험가
GameEvent:AddReward( 416, { fEventTime =   0,  iRewardID = 10229, bPcBangEvent = True, bAccountEvent = True } )		-- 부활석 2개 큐브 1개
GameEvent:AddReward( 417, { fEventTime =  10,  iRewardID = 10230, bPcBangEvent = True, bAccountEvent = True } )		-- PC방 VIP 큐브 1개

-- 다크 문 래빗의 전설
GameEvent:AddReward( 418, { fEventTime =  30,  iRewardID = 	10231 } )			-- 다크 문 래빗 인장 1개

-- 엘의 공명
GameEvent:AddReward( 419, { fEventTime =   0,  iRewardID = 	10232,	bAccountEvent = True } )	-- 망각의 알약 큐브  1개
GameEvent:AddReward( 420, { fEventTime =   0,  iRewardID = 	10233,	bAccountEvent = True } )	-- 강화의 부적 랜덤 큐브 1개

-- 대장장이의 전투 지원
GameEvent:AddReward( 421, { fEventTime =   0,  iRewardID = 	10234	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	1	,   iMaxLevel =	13	 } )	-- 용감한 숲의 수호자 무기 큐브(엘리트)
GameEvent:AddReward( 421, { fEventTime =   0,  iRewardID = 	10235	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	14	,   iMaxLevel =	19	 } )	-- 정의로운 시민의 수호자 무기 큐브(엘리트)
GameEvent:AddReward( 421, { fEventTime =   0,  iRewardID = 	10236	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	20	,   iMaxLevel =	25	 } )	-- 분노한 파멸의 용병 무기 큐브(엘리트)
GameEvent:AddReward( 421, { fEventTime =   0,  iRewardID = 	10237	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	26	,   iMaxLevel =	29	 } )	-- 매서운 분노의 화신 무기 큐브(엘리트)
GameEvent:AddReward( 421, { fEventTime =   0,  iRewardID = 	10238	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	30	,   iMaxLevel =	33	 } )	-- 찬란한 문명의 제압자 무기 큐브(엘리트)
GameEvent:AddReward( 421, { fEventTime =   0,  iRewardID = 	10239	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	34	,   iMaxLevel =	37	 } )	-- 용맹한 마도대전 용사의 무기 큐브(엘리트)
GameEvent:AddReward( 421, { fEventTime =   0,  iRewardID = 	10240	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	38	,   iMaxLevel =	45	 } )	-- 비밀스런 블랙스미스 연합 무기 큐브(엘리트)
GameEvent:AddReward( 421, { fEventTime =   0,  iRewardID = 	10241	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	46	,   iMaxLevel =	49	 } )	-- 피끓는 혈투의 노래 무기 큐브(엘리트)

-- 코보 주식회사의 모험가 캠페인
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10146	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	1	,   iMaxLevel =	1	 } )	-- [레벨 	1	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10147	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	2	,   iMaxLevel =	2	 } )	-- [레벨 	2	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10148	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	3	,   iMaxLevel =	3	 } )	-- [레벨 	3	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10149	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	4	,   iMaxLevel =	4	 } )	-- [레벨 	4	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10150	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	5	,   iMaxLevel =	5	 } )	-- [레벨 	5	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10151	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	6	,   iMaxLevel =	6	 } )	-- [레벨 	6	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10152	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	7	,   iMaxLevel =	7	 } )	-- [레벨 	7	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10153	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	8	,   iMaxLevel =	8	 } )	-- [레벨 	8	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10154	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	9	,   iMaxLevel =	9	 } )	-- [레벨 	9	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10155	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	10	,   iMaxLevel =	10	 } )	-- [레벨 	10	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10156	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	11	,   iMaxLevel =	11	 } )	-- [레벨 	11	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10157	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	12	,   iMaxLevel =	12	 } )	-- [레벨 	12	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10158	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	13	,   iMaxLevel =	13	 } )	-- [레벨 	13	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10159	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	14	,   iMaxLevel =	14	 } )	-- [레벨 	14	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10160	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	15	,   iMaxLevel =	15	 } )	-- [레벨 	15	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10161	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	16	,   iMaxLevel =	16	 } )	-- [레벨 	16	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10162	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	17	,   iMaxLevel =	17	 } )	-- [레벨 	17	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10163	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	18	,   iMaxLevel =	18	 } )	-- [레벨 	18	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10164	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	19	,   iMaxLevel =	19	 } )	-- [레벨 	19	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10165	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	20	,   iMaxLevel =	20	 } )	-- [레벨 	20	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10166	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	21	,   iMaxLevel =	21	 } )	-- [레벨 	21	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10167	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	22	,   iMaxLevel =	22	 } )	-- [레벨 	22	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10168	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	23	,   iMaxLevel =	23	 } )	-- [레벨 	23	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10169	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	24	,   iMaxLevel =	24	 } )	-- [레벨 	24	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10170	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	25	,   iMaxLevel =	25	 } )	-- [레벨 	25	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10171	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	26	,   iMaxLevel =	26	 } )	-- [레벨 	26	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10172	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	27	,   iMaxLevel =	27	 } )	-- [레벨 	27	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10173	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	28	,   iMaxLevel =	28	 } )	-- [레벨 	28	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10174	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	29	,   iMaxLevel =	29	 } )	-- [레벨 	29	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10175	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	30	,   iMaxLevel =	30	 } )	-- [레벨 	30	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10176	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	31	,   iMaxLevel =	31	 } )	-- [레벨 	31	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10177	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	32	,   iMaxLevel =	32	 } )	-- [레벨 	32	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10178	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	33	,   iMaxLevel =	33	 } )	-- [레벨 	33	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10179	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	34	,   iMaxLevel =	34	 } )	-- [레벨 	34	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10180	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	35	,   iMaxLevel =	35	 } )	-- [레벨 	35	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10181	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	36	,   iMaxLevel =	36	 } )	-- [레벨 	36	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10182	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	37	,   iMaxLevel =	37	 } )	-- [레벨 	37	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10183	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	38	,   iMaxLevel =	38	 } )	-- [레벨 	38	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10184	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	39	,   iMaxLevel =	39	 } )	-- [레벨 	39	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10185	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	40	,   iMaxLevel =	40	 } )	-- [레벨 	40	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10186	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	41	,   iMaxLevel =	41	 } )	-- [레벨 	41	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10187	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	42	,   iMaxLevel =	42	 } )	-- [레벨 	42	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10188	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	43	,   iMaxLevel =	43	 } )	-- [레벨 	43	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10189	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	44	,   iMaxLevel =	44	 } )	-- [레벨 	44	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10190	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	45	,   iMaxLevel =	45	 } )	-- [레벨 	45	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10191	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	46	,   iMaxLevel =	46	 } )	-- [레벨 	46	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10192	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	47	,   iMaxLevel =	47	 } )	-- [레벨 	47	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10193	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	48	,   iMaxLevel =	48	 } )	-- [레벨 	48	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10194	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	49	,   iMaxLevel =	49	 } )	-- [레벨 	49	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 422, { fEventTime =   0,  iRewardID = 	10195	,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"], iMinLevel = 	50	,   iMaxLevel =	60	 } )	-- [레벨 	50	] 코보 모험가 서포트 큐브

-- 엘리오스 데일리 파티 이벤트
GameEvent:AddReward( 423, { fEventTime =   0,  iRewardID = 	10244,	bAccountEvent = True } ) -- 목요일 
GameEvent:AddReward( 424, { fEventTime =   0,  iRewardID = 	10245,	bAccountEvent = True } ) -- 금요일 
GameEvent:AddReward( 425, { fEventTime =   0,  iRewardID = 	10246,	bAccountEvent = True } ) -- 토요일 
GameEvent:AddReward( 426, { fEventTime =   0,  iRewardID = 	10247,	bAccountEvent = True } ) -- 일요일 
GameEvent:AddReward( 427, { fEventTime =   0,  iRewardID = 	10248,	bAccountEvent = True } ) -- 월요일 
GameEvent:AddReward( 428, { fEventTime =   0,  iRewardID = 	10249,	bAccountEvent = True } ) -- 화요일	다크 프리스트
GameEvent:AddReward( 429, { fEventTime =   0,  iRewardID = 	10250,	bAccountEvent = True } ) -- 수요일 

-- '쁘띠뽀루' 분양
GameEvent:AddReward( 430, { fEventTime =   0,  iRewardID = 	10251,	iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"] } ) -- 투니랜드 유저에게만 쁘띠뽀루 1EA

GameEvent:AddReward( 431, { fEventTime =   0,  iRewardID = 	10253,	bAccountEvent = True } ) -- 화요일   홀리 프리스트

-- 미지의 검
GameEvent:AddReward( 432, { fEventTime =   0,  iRewardID = 	10254 } )             -- 미지의 검

--_할로윈 던전 무료입장
GameEvent:AddReward( 433, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_NORMAL"]} )
GameEvent:AddReward( 434, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_HARD"]} )
GameEvent:AddReward( 435, { iDungeonID = DUNGEON_ID["DI_ELDER_HALLOWEEN_EXPERT"]} )

-- 할로윈 카니발
GameEvent:AddReward( 436, { fEventTime =   30, iRewardID = 	10255 } )	-- 할로윈 고스트 캔디 3개
GameEvent:AddReward( 437, { fEventTime =   30, iRewardID = 	10256 } )	-- 할로윈 고스트 캔디 6개
GameEvent:AddReward( 438, { fEventTime =   0,  iRewardID = 	10257 } )   -- 펌킨 소울 큐브 1개

-- 분노의 검
GameEvent:AddReward( 439, { fEventTime =   0,  iRewardID = 	10259 } )            -- 분노의 검 1개

-- 버파 이벤트용
GameEvent:AddReward( 440, { fEventTime =   0,  iRewardID = 	10260, iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_ELSWORD"] } ) -- 엘소드 특공대 칭호

-- 수고하셨습니다!
GameEvent:AddReward( 441, { fEventTime =   0,  iRewardID = 	10261 } )            -- 빼빼로의 패기 1개

-- 대장장이의 전투 지원(버블파이터 특공대)
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10126	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	1	,   iMaxLevel =	13	 } )	-- 숲의 수호자 무기 큐브 1개
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10127	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	14	,   iMaxLevel =	19	 } )	-- 시민의 수호자 무기 큐브 1개
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10128	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	20	,   iMaxLevel =	25	 } )	-- 파멸의 용병 무기 큐브 1개
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10129	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	26	,   iMaxLevel =	29	 } )	-- 분노의 화신 무기 큐브 1개
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10130	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	30	,   iMaxLevel =	33	 } )	-- 문명의 제압자 무기 큐브 1개
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10131	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	34	,   iMaxLevel =	37	 } )	-- 마도대전 용사의 무기 큐브 1개
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10132	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	38	,   iMaxLevel =	45	 } )	-- 블랙스미스 연합 무기 큐브 1개
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10133	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	46	,   iMaxLevel =	49	 } )	-- 혈투의 노래 무기 큐브 1개
GameEvent:AddReward( 442, { fEventTime =   0,  iRewardID = 	10134	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	50	,   iMaxLevel =	56	 } )	-- 빛의 수호자 무기 큐브 1개

-- 코보 주식회사의 모험가 캠페인(버블파이터 특공대)
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10146	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	1	,   iMaxLevel =	1	 } )	-- [레벨 	1	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10147	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	2	,   iMaxLevel =	2	 } )	-- [레벨 	2	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10148	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	3	,   iMaxLevel =	3	 } )	-- [레벨 	3	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10149	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	4	,   iMaxLevel =	4	 } )	-- [레벨 	4	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10150	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	5	,   iMaxLevel =	5	 } )	-- [레벨 	5	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10151	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	6	,   iMaxLevel =	6	 } )	-- [레벨 	6	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10152	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	7	,   iMaxLevel =	7	 } )	-- [레벨 	7	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10153	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	8	,   iMaxLevel =	8	 } )	-- [레벨 	8	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10154	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	9	,   iMaxLevel =	9	 } )	-- [레벨 	9	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10155	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	10	,   iMaxLevel =	10	 } )	-- [레벨 	10	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10156	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	11	,   iMaxLevel =	11	 } )	-- [레벨 	11	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10157	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	12	,   iMaxLevel =	12	 } )	-- [레벨 	12	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10158	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	13	,   iMaxLevel =	13	 } )	-- [레벨 	13	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10159	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	14	,   iMaxLevel =	14	 } )	-- [레벨 	14	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10160	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	15	,   iMaxLevel =	15	 } )	-- [레벨 	15	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10161	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	16	,   iMaxLevel =	16	 } )	-- [레벨 	16	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10162	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	17	,   iMaxLevel =	17	 } )	-- [레벨 	17	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10163	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	18	,   iMaxLevel =	18	 } )	-- [레벨 	18	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10164	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	19	,   iMaxLevel =	19	 } )	-- [레벨 	19	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10165	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	20	,   iMaxLevel =	20	 } )	-- [레벨 	20	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10166	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	21	,   iMaxLevel =	21	 } )	-- [레벨 	21	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10167	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	22	,   iMaxLevel =	22	 } )	-- [레벨 	22	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10168	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	23	,   iMaxLevel =	23	 } )	-- [레벨 	23	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10169	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	24	,   iMaxLevel =	24	 } )	-- [레벨 	24	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10170	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	25	,   iMaxLevel =	25	 } )	-- [레벨 	25	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10171	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	26	,   iMaxLevel =	26	 } )	-- [레벨 	26	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10172	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	27	,   iMaxLevel =	27	 } )	-- [레벨 	27	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10173	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	28	,   iMaxLevel =	28	 } )	-- [레벨 	28	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10174	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	29	,   iMaxLevel =	29	 } )	-- [레벨 	29	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10175	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	30	,   iMaxLevel =	30	 } )	-- [레벨 	30	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10176	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	31	,   iMaxLevel =	31	 } )	-- [레벨 	31	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10177	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	32	,   iMaxLevel =	32	 } )	-- [레벨 	32	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10178	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	33	,   iMaxLevel =	33	 } )	-- [레벨 	33	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10179	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	34	,   iMaxLevel =	34	 } )	-- [레벨 	34	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10180	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	35	,   iMaxLevel =	35	 } )	-- [레벨 	35	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10181	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	36	,   iMaxLevel =	36	 } )	-- [레벨 	36	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10182	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	37	,   iMaxLevel =	37	 } )	-- [레벨 	37	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10183	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	38	,   iMaxLevel =	38	 } )	-- [레벨 	38	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10184	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	39	,   iMaxLevel =	39	 } )	-- [레벨 	39	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10185	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	40	,   iMaxLevel =	40	 } )	-- [레벨 	40	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10186	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	41	,   iMaxLevel =	41	 } )	-- [레벨 	41	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10187	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	42	,   iMaxLevel =	42	 } )	-- [레벨 	42	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10188	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	43	,   iMaxLevel =	43	 } )	-- [레벨 	43	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10189	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	44	,   iMaxLevel =	44	 } )	-- [레벨 	44	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10190	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	45	,   iMaxLevel =	45	 } )	-- [레벨 	45	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10191	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	46	,   iMaxLevel =	46	 } )	-- [레벨 	46	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10192	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	47	,   iMaxLevel =	47	 } )	-- [레벨 	47	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10193	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	48	,   iMaxLevel =	48	 } )	-- [레벨 	48	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10194	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	49	,   iMaxLevel =	49	 } )	-- [레벨 	49	] 코보 모험가 서포트 큐브
GameEvent:AddReward( 443, { fEventTime =   0,  iRewardID = 	10195	,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"], iMinLevel = 	50	,   iMaxLevel =	60	 } )	-- [레벨 	50	] 코보 모험가 서포트 큐브

-- 엘리오스 데일리 파티 이벤트
GameEvent:AddReward( 444, { fEventTime =   0,  iRewardID = 	10244,	bAccountEvent = True } ) -- 목요일 특수 기술의 반지(1일)큐브
GameEvent:AddReward( 445, { fEventTime =   0,  iRewardID = 	10263,	bAccountEvent = True } ) -- 금요일 로얄 블러드 풀 세트(1일)큐브
GameEvent:AddReward( 446, { fEventTime =   0,  iRewardID = 	10093,	bAccountEvent = True } ) -- 토요일 망각의 알약(3개)큐브
GameEvent:AddReward( 447, { fEventTime =   0,  iRewardID = 	10103,	bAccountEvent = True } ) -- 일요일 뽀루의 발 도장(보라)1개
GameEvent:AddReward( 448, { fEventTime =   0,  iRewardID = 	10140,	bAccountEvent = True } ) -- 월요일 특수 액세서리 4종(1일)큐브
GameEvent:AddReward( 449, { fEventTime =   0,  iRewardID = 	10264,	bAccountEvent = True } ) -- 화요일 한정 판매 아바타 풀세트(1일)큐브
GameEvent:AddReward( 450, { fEventTime =   0,  iRewardID = 	10250,	bAccountEvent = True } ) -- 수요일 뽀루의 발 도장(하얀)1개

GameEvent:AddReward( 451, { fEventTime =   0,  iRewardID = 	10265,	bAccountEvent = True } ) -- 금요일 살바토르 데니프 풀세트(1일)큐브
GameEvent:AddReward( 452, { fEventTime =   0,  iRewardID = 	10213,	bAccountEvent = True } ) -- 화요일 엘의 나무 씨앗(5개)큐브

-- 엘의 가호
GameEvent:AddReward( 453, { fEventTime =   0,  iRewardID = 	10262,	iBubbleFighterTeam = BUBBLE_FIGHTER_ENUM["BFE_BUBBLE_FIGHTER"] } ) -- 엘의 가호 칭호

-- 3번째 그림자
GameEvent:AddReward( 454, { fEventTime =   0,  iRewardID = 	10266,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) -- 엘소드 3번째 그림자
GameEvent:AddReward( 455, { fEventTime =   0,  iRewardID = 	10267 } )										-- 불명의 열쇠
GameEvent:AddReward( 456, { fEventTime =   0,  iRewardID = 	10268 } )										-- 콘웰의 자아
GameEvent:AddReward( 457, { fEventTime =   0,  iRewardID = 	10269,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) -- 엘소드 3번째 그림자 무기

-- 대장장이들의 전투 지원 이벤트
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10126,	iMinLevel = 1,   iMaxLevel = 13 } )	-- 숲의 수호자 무기 큐브 1개
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10127,	iMinLevel = 14,  iMaxLevel = 19 } )	-- 시민의 수호자 무기 큐브 1개
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10128,	iMinLevel = 20,  iMaxLevel = 25 } )	-- 파멸의 용병 무기 큐브 1개
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10129,	iMinLevel = 26,  iMaxLevel = 29 } )	-- 분노의 화신 무기 큐브 1개
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10130,	iMinLevel = 30,  iMaxLevel = 33 } )	-- 문명의 제압자 무기 큐브 1개
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10131,	iMinLevel = 34,  iMaxLevel = 37 } )	-- 마도대전 용사의 무기 큐브 1개
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10132,	iMinLevel = 38,  iMaxLevel = 45 } )	-- 블랙스미스 연합 무기 큐브 1개
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10133,	iMinLevel = 46,  iMaxLevel = 49 } )	-- 혈투의 노래 무기 큐브 1개
GameEvent:AddReward( 458, { fEventTime =   0,  iRewardID = 	10134,	iMinLevel = 50,  iMaxLevel = 56 } )	-- 빛의 수호자 무기 큐브 1개

-- 가이아의 열기 이벤트
GameEvent:AddReward( 459, { fEXPRate = 0.2,				SERVER_GROUP = SERVER_GROUP_ID["SGI_GAIA"] } )  -- 가이아 유저는 경험치 보너스 +120% 지급
GameEvent:AddReward( 460, { iHenirRewardEventCount = 2, SERVER_GROUP = SERVER_GROUP_ID["SGI_GAIA"] } )	-- 가이아 유저는 헤니르 보상 획득 5회 추가

-- 12월 PC방 이벤트
GameEvent:AddReward( 461, { fEventTime =  60,  iRewardID =  10270, bPcBangEvent = True } )	-- PC방 선물 큐브

-- 3번째 마법소녀 이벤트
GameEvent:AddReward( 462, { fEventTime =   0,  iRewardID =  10272, cUnitType = UNIT_TYPE["UT_ARME"] } )	-- 아이샤 3번째 그림자

-- 네이버 프로모션 이벤트(겨울용)
GameEvent:AddReward( 463, { fEventTime =  10,  iRewardID = 	10273	, 	bAccountEvent = True } )	-- 한겨울 대박 큐브

-- 마법 소녀의 재림
GameEvent:AddReward( 464, { fEventTime =  0,  iRewardID = 	10275	} )	-- 마법 소녀 변신 펜던트 1개
GameEvent:AddReward( 465, { fEventTime =  0,  iRewardID = 	10276	} )	-- 의문의 열쇠 1개 

-- 얼음 조각상 가열기(Gold)
GameEvent:AddReward( 466, { fEventTime = 30,  iRewardID = 	10277	} )	-- 얼음 조각상 가열기(Gold) 1개 ,봉인된 얼음 조각상(Gold) 1개
GameEvent:AddReward( 467, { fEventTime = 30,  iRewardID = 	10277	} )	-- 얼음 조각상 가열기(Gold) 1개 ,봉인된 얼음 조각상(Gold) 1개

-- 사랑에 감사 드립니다!
GameEvent:AddReward( 468, { fEventTime = 0,  iRewardID = 	10278	, 	bAccountEvent = True } )	-- Dream 교환권 1개

-- 3번째 엘프의 그림자
GameEvent:AddReward( 469, { fEventTime =   0,  iRewardID =  10279, cUnitType = UNIT_TYPE["UT_LIRE"] } )	-- 레나 3번째 그림자, 무기

-- 에렌딜의 증명 12/29~
GameEvent:AddReward( 470, { fEventTime =   0,  iRewardID =  10280 } )	-- 미지의 열쇠, 엘드랏실의 열매

-- 네이버 프로모션 이벤트(겨울용)
GameEvent:AddReward( 471, { fEventTime =  10,  iRewardID = 	10282	, 	bAccountEvent = True } )	-- 한겨울 왕대박 큐브

-- 세번째 레이븐의 그림자
GameEvent:AddReward( 472, { fEventTime =   0,  iRewardID =  10283, cUnitType = UNIT_TYPE["UT_RAVEN"] } )	-- 레이븐 3번째 그림자, 무기

-- 불굴의 열쇠
GameEvent:AddReward( 473, { fEventTime =   0,  iRewardID =  10285 } )	-- 웨폰 코어, 불굴의 열쇠

-- 3번째 이브의 그림자
GameEvent:AddReward( 474, { fEventTime =   0,  iRewardID =  10286, cUnitType = UNIT_TYPE["UT_EVE"] } )	-- 이브 3번째 그림자, 무기

-- 새해 복 많이 받으세요
GameEvent:AddReward( 475, { fEventTime =   10,  iRewardID =  187, bAccountEvent = True } )	-- 아리엘의 가열기
GameEvent:AddReward( 476, { fEXPRate = 1.8 } )

-- 흑룡의 새해! 나의 운세는?
GameEvent:AddReward( 477, { fEventTime =   0,  iRewardID =  10287 } )	-- 흑룡셋 + 신년 운세 캡슐

-- 이루어져라!
GameEvent:AddReward( 478, { fEventTime =   30,  iRewardID =  10288, bAccountEvent = True } )	-- 헤니르의 용사 큐브

-- 흑룡의 새해! 나의 운세는?
GameEvent:AddReward( 479, { fEventTime =   0,  iRewardID =  10291, iChannelCode = CHANNELING_ENUM["CE_TOONILAND_ACCOUNT"] } )	-- 투니랜드 4차 프로모션

-- 나소드 여왕의 재탄생
GameEvent:AddReward( 480, { fEventTime =   0,  iRewardID =  10292 } )	-- 코드 분석 회로, 고뇌의 열쇠

-- 3번째 청의 그림자
GameEvent:AddReward( 481, { fEventTime =   0,  iRewardID =  10293, cUnitType = UNIT_TYPE["UT_CHUNG"] } )	-- 청 3번째 그림자, 무기

-- 혼돈의 열쇠
GameEvent:AddReward( 482, { fEventTime =   0,  iRewardID =  10294 } )	-- 디스프로즌, 혼돈의 열쇠

-- 너! 파괴한다!
GameEvent:AddReward( 483, { fEventTime =   0,  iRewardID =  10296 } )	-- 폭발하는 울분의 수정구 x 2

-- 매일매일 벨더 아카데미 출석 선물
GameEvent:AddReward( 484, { fEventTime =   0,  iRewardID =  10297, bAccountEvent = True } )	-- ( 월 ) 표정 액세서리(1일권)
GameEvent:AddReward( 485, { fEventTime =   0,  iRewardID =  10298, bAccountEvent = True } )	-- ( 화 ) 별자리 액세서리(1일권)
GameEvent:AddReward( 486, { fEventTime =   0,  iRewardID =  10299, bAccountEvent = True } )	-- ( 수 ) 신학기 준비(1) 액세서리(1일권)
GameEvent:AddReward( 487, { fEventTime =   0,  iRewardID =  10300, bAccountEvent = True } )	-- ( 목 ) 축복의 메달(1일권)
GameEvent:AddReward( 488, { fEventTime =   0,  iRewardID =  10301, bAccountEvent = True } )	-- ( 금 ) 드래곤 나이트 다크사이드 풀 세트(1일권)
GameEvent:AddReward( 489, { fEventTime =   0,  iRewardID =  10302, bAccountEvent = True } )	-- ( 토 ) 공모전 아바타 풀 세트(1일권)
GameEvent:AddReward( 490, { fEventTime =   0,  iRewardID =  10303, bAccountEvent = True } )	-- ( 일 ) [코보]뽀루의 발 도장(하얀색)
GameEvent:AddReward( 491, { fEventTime =   0,  iRewardID =  10304, bAccountEvent = True } )	-- ( 수 ) 신학기 준비(2) 액세서리(1일권)

-- 공존의 축제에 오신 것을 환영합니다.
GameEvent:AddReward( 492, { fEventTime =   0,  iRewardID =  10305 } )	-- 공존의 축제 의상 큐브

-- 공존의 축제 주말 파티!
GameEvent:AddReward( 493, { fEventTime =   0,  iRewardID =  10306, bAccountEvent = True } )	-- 공존의 축제 큐브
GameEvent:AddReward( 494, { fDropRate =  2.0 } ) -- 드롭률 2배

-- 레이징 히어로 전야!
GameEvent:AddReward( 495, { fEventTime =   30,  iRewardID =  10307  } )	-- 레이징 히어로 큐브
GameEvent:AddReward( 496, { fEventTime =   0,  iRewardID =  10308  } )	-- 영웅의 반지(1일권)
GameEvent:AddReward( 497, { fEventTime =   0,  iRewardID =  10242  } )	-- 망각의 드링크

-- 영웅의 비상
GameEvent:AddReward( 498, { fEventTime =   0,  iRewardID = 	10309	} )	-- 영웅의 비상

-- 부활의 노래
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10310, iMinLevel = 	1	,   iMaxLevel =	13, bAccountEvent = True 	 } )	-- +6 용감한 숲의 수호자 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10311, iMinLevel = 	14	,   iMaxLevel =	19, bAccountEvent = True 	 } )	-- +6 정의로운 시민의 수호자 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10312, iMinLevel = 	20	,   iMaxLevel =	25, bAccountEvent = True 	 } )	-- +6 분노한 파멸의 용병 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10313, iMinLevel = 	26	,   iMaxLevel =	29, bAccountEvent = True 	 } )	-- +6 매서운 분노의 화신 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10314, iMinLevel = 	30	,   iMaxLevel =	33, bAccountEvent = True 	 } )	-- +6 찬란한 문명의 제압자 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10315, iMinLevel = 	34	,   iMaxLevel =	37, bAccountEvent = True 	 } )	-- +6 용맹한 마도대전 용사 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10316, iMinLevel = 	38	,   iMaxLevel =	45, bAccountEvent = True 	 } )	-- +6 블랙스미스 연합 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10317, iMinLevel = 	46	,   iMaxLevel =	49, bAccountEvent = True 	 } )	-- +6 피끓는 혈투의 노래 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10318, iMinLevel = 	50	,   iMaxLevel =	55, bAccountEvent = True 	 } )	-- +6 엘리오스의 영웅 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10319, iMinLevel = 	56	,   iMaxLevel =	59, bAccountEvent = True 	 } )	-- +6 광폭한 지배자 풀세트 큐브
GameEvent:AddReward( 499, { fEventTime =   0,  iRewardID = 	10320, iMinLevel = 	60	,   iMaxLevel =	60, bAccountEvent = True 	 } )	-- +6 군단 지휘관 풀세트 큐브

-- 아리엘의 복귀 용사님을 위한 선물!
--[[
ScriptID 500의 경우 '아리엘의 복귀 용사님을 위한 선물!' 이벤트에 하드 코딩되어 사용되어지는 부분이 있으니
이점 유의하시길 바랍니다.
--]]
GameEvent:AddReward( 500, { fEventTime =   0,  iRewardID = 	10321, iMinLevel = 	1	,   iMaxLevel =	19, bAccountEvent = True 	 } )	-- 드림 스텝 - 하트(1개권) 큐브
GameEvent:AddReward( 500, { fEventTime =   0,  iRewardID = 	10322, iMinLevel = 	20	,   iMaxLevel =	29, bAccountEvent = True 	 } )	-- 기술의 노트(1개권), 캐시 메모 큐브
GameEvent:AddReward( 500, { fEventTime =   0,  iRewardID = 	10323, iMinLevel = 	30	,   iMaxLevel =	39, bAccountEvent = True 	 } )	-- 기술의 반지 4종(1개권) 큐브
GameEvent:AddReward( 500, { fEventTime =   0,  iRewardID = 	10324, iMinLevel = 	40	,   iMaxLevel =	49, bAccountEvent = True 	 } )	-- 마법의 목걸이(II) (1개권)
GameEvent:AddReward( 500, { fEventTime =   0,  iRewardID = 	10325, iMinLevel = 	50	,   iMaxLevel =	54, bAccountEvent = True 	 } )	-- 스킬 초기화 메달(1개권), 한정 액세서리(1개권) 랜덤 큐브
GameEvent:AddReward( 500, { fEventTime =   0,  iRewardID = 	10326, iMinLevel = 	55	,   iMaxLevel =	59, bAccountEvent = True 	 } )	-- 한정판매 아바타 풀세트(1개권) 랜덤 큐브, 정제된 엘의 조각 마스터(1개권) 랜덤 큐브
GameEvent:AddReward( 500, { fEventTime =   0,  iRewardID = 	10327, iMinLevel = 	60	,   iMaxLevel =	60, bAccountEvent = True 	 } )	-- 아리엘 강화의 부적 Lv.8 (1개), BEST 인기 아바타 파츠(1개권) 큐브

-- Enjoy Riding~!
GameEvent:AddReward( 501, { fEventTime =   0,  iRewardID = 	10328	} )	-- ( 목 ) 기동전사 건달-덴드로차오름
GameEvent:AddReward( 502, { fEventTime =   0,  iRewardID = 	10329	} )	-- ( 금 ) 미흑인비행물체 콘라드 UFO
GameEvent:AddReward( 503, { fEventTime =   0,  iRewardID = 	10330	} )	-- ( 토 ) 애마전사
GameEvent:AddReward( 504, { fEventTime =   0,  iRewardID = 	10331	} )	-- ( 일 ) 3개 전부
GameEvent:AddReward( 505, { fEventTime =   0,  iRewardID = 	10331	} )	-- ( 월 ) 3개 전부
GameEvent:AddReward( 506, { fEventTime =   0,  iRewardID = 	10331	} )	-- ( 화 ) 3개 전부
GameEvent:AddReward( 507, { fEventTime =   0,  iRewardID = 	10331	} )	-- ( 수 ) 3개 전부

-- 이보시오 영자 양반! 내가 여자라니?!
GameEvent:AddReward( 508, { fEventTime =   0,  iRewardID = 	10332	} )	-- 망상의 큐브

-- 가이아 추가 경험치 80%
GameEvent:AddReward( 509, { fEXPRate = 0.8, SERVER_GROUP_ID["SGI_GAIA"] } )

-- 연금술 이벤트 보상
GameEvent:AddReward( 510, { fEventTime =   0,  iRewardID = 	10333, bAccountEvent = True	} )	-- 미니 딸기 타르트 30개

-- 엘소드 GM 리더를 뽑아주세요!
GameEvent:AddReward( 511, { fEventTime =   0,  iRewardID = 	10334, bAccountEvent = True	} )	-- 리엘   - 빛의 창 5개
GameEvent:AddReward( 512, { fEventTime =   0,  iRewardID = 	10335, bAccountEvent = True	} )	-- 아이린 - 완전회복포션 5개
GameEvent:AddReward( 513, { fEventTime =   0,  iRewardID = 	10336, bAccountEvent = True	} )	-- 고등어 - 엘의조각(불명) 5개
GameEvent:AddReward( 514, { fEventTime =   0,  iRewardID = 	10337, bAccountEvent = True	} )	-- 너구리 - 활력의 포션 5개
GameEvent:AddReward( 515, { fEventTime =   0,  iRewardID = 	10338, bAccountEvent = True	} )	-- 캅     - 초인의 사과 5개

-- 벨더 아카데미
GameEvent:AddReward( 516, { fEventTime =   0,  iRewardID = 	10339	} )	-- 최우수,우수,장려,노력상 큐브, 벨더아카데미시험지

-- 엘리오스 데일리 파티
GameEvent:AddReward( 517, { fEventTime =   0,  iRewardID =  10340, bAccountEvent = True } )	-- 금  금강야차 올세트(1일권) 큐브
GameEvent:AddReward( 518, { fEventTime =   0,  iRewardID =  10091, bAccountEvent = True } ) -- 수  뽀루의 발 도장(하얀색) 1EA

-- 예고!X2!
GameEvent:AddReward( 519, { fEXPRate = 1.0, fVPRate = 1.0, SERVER_GROUP_ID["SGI_SOLES"] } ) -- 솔레스 경험치, ap 200%
GameEvent:AddReward( 520, { fEXPRate = 0.8, fVPRate = 1.0, SERVER_GROUP_ID["SGI_GAIA"] } ) -- 가이아 경험치, ap 200%

-- 에이핑크가 응원합니다!
-- ScriptID 521, 522의 경우 '에이핑크가 응원합니다!' 이벤트에 하드 코딩되어 사용되어지는 부분이 있으니 이점 유의하시길 바랍니다.
-- ( 캐릭터 )
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10341,	iMinLevel = 	1	,   iMaxLevel =	13 	 } )	-- +6 에이핑크의 수호자 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10342,	iMinLevel = 	14	,   iMaxLevel =	19 	 } )	-- +6 정의로운 에이핑크 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10343,	iMinLevel = 	20	,   iMaxLevel =	25 	 } )	-- +6 분노한 에이핑크의 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10344,	iMinLevel = 	26	,   iMaxLevel =	29 	 } )	-- +6 사랑의 아이돌 에이핑크 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10345,	iMinLevel = 	30	,   iMaxLevel =	33 	 } )	-- +6 나를 제압한 에이핑크 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10346,	iMinLevel = 	34	,   iMaxLevel =	37 	 } )	-- +6 용감한 에이핑크 용사 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10347,	iMinLevel = 	38	,   iMaxLevel =	45 	 } )	-- +6 에이핑크 연합 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10348,	iMinLevel = 	46	,   iMaxLevel =	49 	 } )	-- +6 감성 아이돌 에이핑크 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10349,	iMinLevel = 	50	,   iMaxLevel =	55 	 } )	-- +6 에이핑크의 영웅 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10350,	iMinLevel = 	56	,   iMaxLevel =	59 	 } )	-- +6 에이핑크가 좋아한 풀세트 큐브
GameEvent:AddReward( 521, { fEventTime =   0,  iRewardID = 10351,	iMinLevel = 	60	,   iMaxLevel =	60 	 } )	-- +6 에이핑크를 사랑한 용사 풀세트 큐브

-- ( 계정 )
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10341,	iMinLevel = 	1	,   iMaxLevel =	13, bAccountEvent = True 	 } )	-- +6 에이핑크의 수호자 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10342,	iMinLevel = 	14	,   iMaxLevel =	19, bAccountEvent = True 	 } )	-- +6 정의로운 에이핑크 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10343,	iMinLevel = 	20	,   iMaxLevel =	25, bAccountEvent = True 	 } )	-- +6 분노한 에이핑크의 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10344,	iMinLevel = 	26	,   iMaxLevel =	29, bAccountEvent = True 	 } )	-- +6 사랑의 아이돌 에이핑크 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10345,	iMinLevel = 	30	,   iMaxLevel =	33, bAccountEvent = True 	 } )	-- +6 나를 제압한 에이핑크 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10346,	iMinLevel = 	34	,   iMaxLevel =	37, bAccountEvent = True 	 } )	-- +6 용감한 에이핑크 용사 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10347,	iMinLevel = 	38	,   iMaxLevel =	45, bAccountEvent = True 	 } )	-- +6 에이핑크 연합 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10348,	iMinLevel = 	46	,   iMaxLevel =	49, bAccountEvent = True 	 } )	-- +6 감성 아이돌 에이핑크 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10349,	iMinLevel = 	50	,   iMaxLevel =	55, bAccountEvent = True 	 } )	-- +6 에이핑크의 영웅 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10350,	iMinLevel = 	56	,   iMaxLevel =	59, bAccountEvent = True 	 } )	-- +6 에이핑크가 좋아한 풀세트 큐브
GameEvent:AddReward( 522, { fEventTime =   0,  iRewardID = 10351,	iMinLevel = 	60	,   iMaxLevel =	60, bAccountEvent = True 	 } )	-- +6 에이핑크를 사랑한 용사 풀세트 큐브

-- 완전 소중! 에이핑크 스탠딩 보드!						
GameEvent:AddReward( 523,	{ fEventTime =   0,	iRewardID = 10352,	bAccountEvent = True	} )	--	A Pink 박초롱 스탠딩 보드
GameEvent:AddReward( 524,	{ fEventTime =   0,	iRewardID = 10353,	bAccountEvent = True	} )	--	A Pink 오하영 스탠딩 보드
GameEvent:AddReward( 525,	{ fEventTime =   0,	iRewardID = 10354,	bAccountEvent = True	} )	--	A Pink 윤보미 스탠딩 보드
GameEvent:AddReward( 526,	{ fEventTime =   0,	iRewardID = 10355,	bAccountEvent = True	} )	--	A Pink 김남주 스탠딩 보드
GameEvent:AddReward( 527,	{ fEventTime =   0,	iRewardID = 10356,	bAccountEvent = True	} )	--	A Pink 정은지 스탠딩 보드
GameEvent:AddReward( 528,	{ fEventTime =   0,	iRewardID = 10357,	bAccountEvent = True	} )	--	A Pink 홍유경 스탠딩 보드
GameEvent:AddReward( 529,	{ fEventTime =   0,	iRewardID = 10358,	bAccountEvent = True	} )	--	A Pink 손나은 스탠딩 보드

-- 솔레스 추가 경험치 100%
GameEvent:AddReward( 530, { fEXPRate = 1.0, SERVER_GROUP_ID["SGI_SOLES"] } )

-- 2012-05-26 ~ 2012-05-28 토일월 주말 이벤트
GameEvent:AddReward( 531, { fEventTime =   10,  iRewardID =  10197, bAccountEvent = True } )	-- 얼음 조각상 가열기 1개

-- 매일매일 선물 상자
-- 루리엘 패키지
GameEvent:AddReward( 532,	{ fEventTime =   1,	iRewardID = 10359	} )	-- 루리엘 큐브 목요일
GameEvent:AddReward( 533,	{ fEventTime =   1,	iRewardID = 10360	} )	-- 루리엘 큐브 금요일
GameEvent:AddReward( 534,	{ fEventTime =   1,	iRewardID = 10361	} )	-- 루리엘 큐브 토요일
GameEvent:AddReward( 535,	{ fEventTime =   1,	iRewardID = 10362	} )	-- 루리엘 큐브 일요일
GameEvent:AddReward( 536,	{ fEventTime =   1,	iRewardID = 10363	} )	-- 루리엘 큐브 월요일
GameEvent:AddReward( 537,	{ fEventTime =   1,	iRewardID = 10364	} )	-- 루리엘 큐브 화요일
GameEvent:AddReward( 538,	{ fEventTime =   1,	iRewardID = 10365	} )	-- 루리엘 큐브 수요일

-- 아리엘 패키지
GameEvent:AddReward( 539,	{ fEventTime =   1,	iRewardID = 10366	} )	-- 아리엘 큐브 목요일
GameEvent:AddReward( 540,	{ fEventTime =   1,	iRewardID = 10367	} )	-- 아리엘 큐브 금요일
GameEvent:AddReward( 541,	{ fEventTime =   1,	iRewardID = 10368	} )	-- 아리엘 큐브 토요일
GameEvent:AddReward( 542,	{ fEventTime =   1,	iRewardID = 10369	} )	-- 아리엘 큐브 일요일
GameEvent:AddReward( 543,	{ fEventTime =   1,	iRewardID = 10370	} )	-- 아리엘 큐브 월요일
GameEvent:AddReward( 544,	{ fEventTime =   1,	iRewardID = 10371	} )	-- 아리엘 큐브 화요일
GameEvent:AddReward( 545,	{ fEventTime =   1,	iRewardID = 10372	} )	-- 아리엘 큐브 수요일

-- 이벤트 경험치 중복 조건추가    (예시)   GameEvent:AddReward( 546, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ELSWORD_SHEATH_KNIGHT"], bAccumulateEXP = True } ) 

-- 시스나이트 2차 전직 이벤트
GameEvent:AddReward( 546, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ELSWORD_SHEATH_KNIGHT"] } )  -- 중복 안됨 -- 시스나이트 캐릭터 35렙 이하 경험치 100% 

-- pc 방 유저의 경우 30분 마다 근성 포션 지급
GameEvent:AddReward( 547,	{ fEventTime =   30,	iRewardID = 10390, bRepeatEvent = True, bPcBangEvent = True	} )	-- 근성 포션

-- 수호천사 에이핑크 미니미를 만나요
GameEvent:AddReward( 548,	{ fEventTime =   0,	iRewardID = 10373, bAccountEvent = True	} )	-- 초롱 천사 미니미

-- 수호천사 에이핑크와 함께 전장을 누벼라!
GameEvent:AddReward( 549,	{ fEventTime =   0,	iRewardID = 10380	} )	-- 초롱 천사 소환 카드

-- 무한의 검사
GameEvent:AddReward( 550,	{ fEventTime =   0,	iRewardID = 10386	} )	-- 인피니티 소드의 열쇠

-- 배틀메이지 2차 전직 이벤트
GameEvent:AddReward( 551, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ARME_BATTLE_MAGICIAN"] } )  -- 중복 안됨 -- 시스나이트 캐릭터 35렙 이하 경험치 100% 

-- 여름방학 이벤트 큐브
GameEvent:AddReward( 552,	{ fEventTime =   10,	iRewardID = 10387, bAccountEvent = True	} )	-- 여름방학 대박 큐브
GameEvent:AddReward( 553,	{ fEventTime =   10,	iRewardID = 10388, bAccountEvent = True	} )	-- 여름방학 황금 큐브
GameEvent:AddReward( 554,	{ fEventTime =   10,	iRewardID = 10389, bAccountEvent = True	} )	-- 여름방학 넥슨 캐시 큐브

-- 수호천사 에이핑크 미니미를 만나요
GameEvent:AddReward( 555,	{ fEventTime =   0,	iRewardID = 10374, bAccountEvent = True	} )	-- 보미 천사 미니미 - 7/19~7/26
GameEvent:AddReward( 556,	{ fEventTime =   0,	iRewardID = 10375, bAccountEvent = True	} )	-- 하영 천사 미니미 - 7/26~8/2
GameEvent:AddReward( 557,	{ fEventTime =   0,	iRewardID = 10376, bAccountEvent = True	} )	-- 은지 천사 미니미 - 8/2~8/9
GameEvent:AddReward( 558,	{ fEventTime =   0,	iRewardID = 10377, bAccountEvent = True	} )	-- 유경 천사 미니미 - 8/9~8/16
GameEvent:AddReward( 559,	{ fEventTime =   0,	iRewardID = 10378, bAccountEvent = True	} )	-- 남주 천사 미니미 - 8/16~8/23
GameEvent:AddReward( 560,	{ fEventTime =   0,	iRewardID = 10379, bAccountEvent = True	} )	-- 대천사 나은 천사 미니미 - 8/23~8/30

-- 수호천사 에이핑크와 함께 전장을 누벼라!
GameEvent:AddReward( 561,	{ fEventTime =   0,	iRewardID = 10381	} )	-- 보미 천사 소환 카드 - 7/26~8/9
GameEvent:AddReward( 562,	{ fEventTime =   0,	iRewardID = 10382	} )	-- 하영 천사 소환 카드 - 8/9~8/23
GameEvent:AddReward( 563,	{ fEventTime =   0,	iRewardID = 10383	} )	-- 은지 천사 소환 카드 - 8/23~9/6
GameEvent:AddReward( 564,	{ fEventTime =   0,	iRewardID = 10384	} )	-- 유경 천사 소환 카드 - 9/6~9/20
GameEvent:AddReward( 565,	{ fEventTime =   0,	iRewardID = 10385	} )	-- 남주 천사 소환 카드 - 9/20~10/4

-- 공간을 다스리는 마녀
GameEvent:AddReward( 566,	{ fEventTime =   0,	iRewardID = 10391	} )	-- 디멘션 위치의 열쇠 - 7/26~8/2 

-- 사과의 큐브
GameEvent:AddReward( 567,	{ fEventTime =   0,	iRewardID = 10392, bAccountEvent = True	} )	-- 사과의 큐브 - 7/19~7/26

-- 트래핑 레인져 2차 전직 이벤트
GameEvent:AddReward( 568, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_LIRE_TRAPPING_RANGER"] } )  -- 중복 안됨 -- 트래핑 레인져 캐릭터 35렙 이하 경험치 100% 

-- 나라사랑 완장
GameEvent:AddReward( 569,	{ fEventTime =   0,	iRewardID = 10393	} )	-- 나라사랑 완장 7월 28일 11시~12시

-- 웨폰 테이커 2차 전직 이벤트
GameEvent:AddReward( 570, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_RAVEN_WEAPON_TAKER"] } )  -- 중복 안됨 -- 웨폰 테이커 캐릭터 35렙 이하 경험치 100% 

-- 엘드랏실의 수호자
GameEvent:AddReward( 571,	{ fEventTime =   0,	iRewardID = 10394	} )	-- 나이트 와처 열쇠

-- 코드 일렉트라 2차 전직 이벤트
GameEvent:AddReward( 572, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_EVE_ELECTRA"] } )  -- 중복 안됨 -- 코드 일렉트라 캐릭터 35렙 이하 경험치 100% 

-- 대천사의 주화
GameEvent:AddReward( 573,	{ fEventTime =   10,	iRewardID = 10395,	bAccountEvent = True	} )	-- 대천사의 주화

-- 다크엔젤 미니미
GameEvent:AddReward( 574,	{ fEventTime =   0,	iRewardID = 10396, bAccountEvent = True	} )	-- 대천사 나은 미니미(블랙)

-- 오버히트!폭발!불꽃의 지휘관!
GameEvent:AddReward( 575,	{ fEventTime =   0,	iRewardID = 10397	} )	-- 베테랑 커맨더 열쇠

-- 2012 여름 방학 주말 이벤트 근성 포션 지급
GameEvent:AddReward( 576,	{ fEventTime =   0,	iRewardID = 10390 } )	-- 근성 포션

-- 로열 서번트 메이드 올세트(1일권) 큐브
GameEvent:AddReward( 577,	{ fEventTime =   0,	iRewardID = 10398, bAccountEvent = True	} )	-- 로열 서번트 메이드 올세트(1일권) 큐브

-- 민중의 수호자 풀세트(1일권) 큐브
GameEvent:AddReward( 578,	{ fEventTime =   0,	iRewardID = 10399, bAccountEvent = True	} )	-- 민중의 수호자 풀세트(1일권) 큐브

-- 살바토르 솔레스 올세트(1일권) 큐브
GameEvent:AddReward( 579,	{ fEventTime =   0,	iRewardID = 10400, bAccountEvent = True	} )	-- 살바토르 솔레스 올세트(1일권) 큐브

-- 할로윈 밤의 마왕과 마녀 풀세트(1일권) 큐브
GameEvent:AddReward( 580,	{ fEventTime =   0,	iRewardID = 10401, bAccountEvent = True	} )	-- 할로윈 밤의 마왕과 마녀 풀세트(1일권) 큐브

-- 아크엔젤 올세트(1일권) 큐브
GameEvent:AddReward( 581,	{ fEventTime =   0,	iRewardID = 10402, bAccountEvent = True	} )	-- 아크엔젤 올세트(1일권) 큐브

-- 인궁무예랑 풀세트(1일권) 큐브
GameEvent:AddReward( 582,	{ fEventTime =   0,	iRewardID = 10403, bAccountEvent = True	} )	-- 인궁무예랑 풀세트(1일권) 큐브

-- 기동 천사
GameEvent:AddReward( 583,	{ fEventTime =   0,	iRewardID = 10404	} )	-- 배틀 세라프의 열쇠

-- 쉘링 가디언 2차 전직 이벤트
GameEvent:AddReward( 584, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_CHUNG_SHELLING_GUARDIAN"] } )  -- 중복 안됨 -- 쉘링 가디언 캐릭터 35렙 이하 경험치 100% 

-- 태풍 이슈 '기술의 반지(1일권)'
GameEvent:AddReward( 585, { fEventTime =  1,  iRewardID = 10405	} ) -- 기술의 반지(1일)1EA

-- 집중 포화!공포의 하얀 늑대
GameEvent:AddReward( 586,	{ fEventTime =   0,	iRewardID = 10406	} )	-- 택티컬 트루퍼의 열쇠

-- 백섭 보상 
GameEvent:AddReward( 587,	{ fEventTime =   0,	iRewardID = 10408, iMinLevel = 	1	,   iMaxLevel =	40, bAccountEvent = True	} )	-- 현자의 주문서 2개
GameEvent:AddReward( 588,	{ fEventTime =   0,	iRewardID = 10409, iMinLevel = 	41	,   iMaxLevel =	60, bAccountEvent = True	} )	-- 현자의 마법석 5개
GameEvent:AddReward( 589,	{ fEventTime =   0,	iRewardID = 10013, bAccountEvent = True	} )											-- 플루오르 스톤 교환권

-- 엘리오스의 부름( 필드 전야 이벤트 )
GameEvent:AddReward( 590,	{ fEventTime =   0,	iRewardID = 10410,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 코보 서포트 패키지, 레어 아바타 풀세트(30일권) 교환권

-- 필드 탐험! 황금의 상자를 찾아서!( 필드 런칭 이벤트 )
GameEvent:AddReward( 591,	{ fEventTime =   0,	iRewardID = 10412, bAccountEvent = True	} )	-- 필드 기념 주화 큐브

-- 필드 업데이트 기념! 핫타임
GameEvent:AddReward( 592,	{ fEventTime =   10,	iRewardID = 10413, bAccountEvent = True	} )	-- 얼음 조각상 가열기(Gold) 3개

GameEvent:AddReward( 593,	{ fEventTime =   10,	iRewardID = 10414, bAccountEvent = True	} )	-- [코보]뽀루의 발 도장 (하얀색) 1개, [코보]뽀루의 발 도장 (보라색) 1개

-- 엘리오스 조사단 1
GameEvent:AddReward( 594,	{ fEventTime =   0,		iRewardID = 10415	} )	-- 엘리오스 조사단 아바타 풀세트(1개권) 큐브

-- 매일 접속 이벤트
GameEvent:AddReward( 595,	{ fEventTime =   30,		iRewardID = 10418, bAccountEvent = True	} )	-- 장갑 아바타(1개권) 랜덤 큐브 (토)
GameEvent:AddReward( 596,	{ fEventTime =   60,		iRewardID = 10420, bAccountEvent = True	} )	-- 헤어 아바타(1개권) 랜덤 큐브 (토)
GameEvent:AddReward( 597,	{ fEventTime =   30,		iRewardID = 10419, bAccountEvent = True	} )	-- 신발 아바타(1개권) 랜덤 큐브 (일)
GameEvent:AddReward( 598,	{ fEventTime =   60,		iRewardID = 10421, bAccountEvent = True	} )	-- 무기 아바타(1개권) 랜덤 큐브 (일)
GameEvent:AddReward( 599,	{ fEventTime =   30,		iRewardID = 10416, bAccountEvent = True	} )	-- 상의 아바타(1개권) 랜덤 큐브 (월)
GameEvent:AddReward( 600,	{ fEventTime =   30,		iRewardID = 10417, bAccountEvent = True	} )	-- 하의 아바타(1개권) 랜덤 큐브 (화)
GameEvent:AddReward( 601,	{ fEventTime =   30,		iRewardID = 10422, bAccountEvent = True	} )	-- 한벌 아바타(1개권) 랜덤 큐브 (수)

-- 너구리가 자꾸 졸라서 하는 이벤트
GameEvent:AddReward( 602,	{ fEventTime =   30,		iRewardID = 10423, bAccountEvent = True	} )	-- 정제된 엘의 조각 랜덤 큐브 (1개)
GameEvent:AddReward( 603,	{ fEventTime =   30,		iRewardID = 10424, bAccountEvent = True	} )	-- 에픽 NPC 카드 랜덤 큐브 (2개)
GameEvent:AddReward( 604,	{ fEventTime =   30,		iRewardID = 10425, bAccountEvent = True	} )	-- 황금 얼음 조각상 가열기(5개) + 봉인된 황금 얼음 조각상(5개)
GameEvent:AddReward( 605,	{ fEventTime =   30,		iRewardID = 10426, bAccountEvent = True	} )	-- 아리엘의 강화 부적 랜덤 큐브 (1개)
GameEvent:AddReward( 606,	{ fEventTime =   30,		iRewardID = 10303, bAccountEvent = True	} )	-- [코보]뽀루의 발 도장(하얀색) (1개)
GameEvent:AddReward( 607,	{ fEventTime =   30,		iRewardID = 10427, bAccountEvent = True	} )	-- 대리 판매 보증서(7일권)
GameEvent:AddReward( 608,	{ fEventTime =   30,		iRewardID = 10428, bAccountEvent = True	} )	-- 얼음 조각상 가열기 (5개) + 봉인된 얼음 조각상 (5개)

-- 역습의 날개
GameEvent:AddReward( 609,	{ fEventTime =   0,			iRewardID = 10429	} )	-- 역습의 날개(기간제)
GameEvent:AddReward( 610,	{ fEventTime =   0,			iRewardID = 10430	} )	-- 각성한 역습의 날개 원소

-- (헤니르 보상 획득 횟수)
GameEvent:AddReward( 611,	{ iHenirRewardEventCount = 5, SERVER_GROUP = SERVER_GROUP_ID["SGI_GAIA"] } )	-- 가이아 유저는 헤니르 보상 획득 5회 추가

-- 시작된 역습1 (밸런스 개편 이벤트)
GameEvent:AddReward( 612,	{ fEventTime =	 0,			iRewardID = 10084,	cUnitType = UNIT_TYPE["UT_LIRE"] } )	-- 레나, 망각의 드링크(7일권) 큐브 1개
GameEvent:AddReward( 613,	{ fEventTime =	 0,			iRewardID = 10084,	cUnitType = UNIT_TYPE["UT_EVE"] } )	-- 이브, 망각의 드링크(7일권) 큐브 1개
GameEvent:AddReward( 614,	{ fEXPRate = 0.3,	cUnitType = UNIT_TYPE["UT_LIRE"] } )	-- 레나, 경험치 +30%
GameEvent:AddReward( 615,	{ fEXPRate = 0.3,	cUnitType = UNIT_TYPE["UT_EVE"] } )		-- 이브, 경험치 +30%

-- 천년 여우의 선물
GameEvent:AddReward( 616,	{ fEventTime =  0,			iRewardID = 10431, bAccountEvent = True	} )		-- 코보 서포트 패키지
GameEvent:AddReward( 617,	{ fEventTime =  30,			iRewardID = 10432, bAccountEvent = True	} )		-- 천년 여우의 선물 : 얼굴 액세서리(1개권) 랜덤 큐브
GameEvent:AddReward( 618,	{ fEventTime =  30,			iRewardID = 10433, bAccountEvent = True	} )		-- 천년 여우의 선물 : 상의 액세서리(1개권) 랜덤 큐브
GameEvent:AddReward( 619,	{ fEventTime =  30,			iRewardID = 10434, bAccountEvent = True	} )		-- 천년 여우의 선물 : 하의 액세서리(1개권) 랜덤 큐브
GameEvent:AddReward( 620,	{ fEventTime =  30,			iRewardID = 10435, bAccountEvent = True	} )		-- 천년 여우의 선물 : 팔 액세서리 (1개권) 랜덤 큐브
GameEvent:AddReward( 621,	{ fEventTime =  30,			iRewardID = 10436, bAccountEvent = True	} )		-- 천년 여우의 선물 : 반지 액세서리 (1개권) 랜덤 큐브

-- 천년 여우의 부름
GameEvent:AddReward( 622,	{ fEventTime =   0,			iRewardID = 10437	} )	-- 천년 여우의 부름

-- 천년 여우의 성장 지원
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10438,	iMinLevel = 	1	,   iMaxLevel =	13	} )	-- +6 천년 여우 수호자 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10439,	iMinLevel = 	14	,   iMaxLevel =	19	} )	-- +6 봉인된 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10440,	iMinLevel = 	20	,   iMaxLevel =	25	} )	-- +6 분노한 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10441,	iMinLevel = 	26	,   iMaxLevel =	29	} )	-- +6 불가사의한 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10442,	iMinLevel = 	30	,   iMaxLevel =	33	} )	-- +6 나를 제압한 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10443,	iMinLevel = 	34	,   iMaxLevel =	37	} )	-- +6 부활한 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10444,	iMinLevel = 	38	,   iMaxLevel =	45	} )	-- +6 각성한 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10445,	iMinLevel = 	46	,   iMaxLevel =	49	} )	-- +6 얄미운 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10446,	iMinLevel = 	50	,   iMaxLevel =	55	} )	-- +6 꼬리 치는 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10447,	iMinLevel = 	56	,   iMaxLevel =	59	} )	-- +6 질주하는 천년 여우 풀세트 큐브
GameEvent:AddReward( 623, { fEventTime =   0,  iRewardID = 10448,	iMinLevel = 	60	,   iMaxLevel =	65	} )	-- +6 신이 된 천년 여우 풀세트 큐브

-- 겨울 방학 대박 큐브
GameEvent:AddReward( 624, { fEventTime =   0,  iRewardID = 10449,	bAccountEvent = True	} )	-- 겨울 방학 대박 큐브

-- 겨울 방학 이벤트
GameEvent:AddReward( 625, { fEventTime =   0,  iRewardID = 10450	} )	-- 엣지 눈뽀루

-- 겨울 방학 대박 큐브
GameEvent:AddReward( 626, { fEventTime =   10,  iRewardID = 10449,	bAccountEvent = True	} )	-- 겨울 방학 대박 큐브

-- 겨울 방학 이벤트( 모래바람이 불어오는 곳 )
GameEvent:AddReward( 627, { fEventTime =   0,  iRewardID = 10454	} )	-- 낡은 모래바람 바람개비(14일권)

-- 엘소드 5주년 엘리오스 파티!
GameEvent:AddReward( 628, { fEventTime =   30,  iRewardID = 10455,	bAccountEvent = True	} )	-- 엘소드 5주념 기념 큐브
GameEvent:AddReward( 629, { fEventTime =   30,  iRewardID = 10456,	bAccountEvent = True	} )	-- 2013년 D-4 현물 경품 랜덤 큐브
GameEvent:AddReward( 630, { fEventTime =   30,  iRewardID = 10457,	bAccountEvent = True	} )	-- 2013년 D-3 프로모션 아바타 (15일권) 큐브
GameEvent:AddReward( 631, { fEventTime =   30,  iRewardID = 10458,	bAccountEvent = True	} )	-- 2013년 D-2 캐시 액세서리 랜덤 큐브
GameEvent:AddReward( 632, { fEventTime =   30,  iRewardID = 10459,	bAccountEvent = True	} )	-- 굿바이 2012년 큐브
GameEvent:AddReward( 633, { fEventTime =   30,  iRewardID = 10460,	bAccountEvent = True	} )	-- 웰컴 2013년 계사년 얼굴 액세서리 (1개권)
GameEvent:AddReward( 634, { fEventTime =   30,  iRewardID = 10461,	bAccountEvent = True	} )	-- 얼음 조각상 가열기 5개, 봉인된 얼음 조각상 5개

-- EMS 복귀 유저 프로모션 지원
--GameEvent:AddReward( 635, { fEventTime =   0,  iRewardID = 10462,	bAccountEvent = True, iEventGroupID = 0	} )	-- 천년 여우 복귀 환영 큐브

-- 모래바람의 소문
GameEvent:AddReward( 636, { fEventTime =   0,  iRewardID = 10463	} )	-- 모래바람의 축복

-- 역습의 헤드윙
GameEvent:AddReward( 637, { fEventTime =   0,  iRewardID = 10464	} )	-- 역습의 헤드윙(기간제)

-- 천년 여우 꼬리 잡기!
GameEvent:AddReward( 638, { fEventTime =   30,  iRewardID = 10465,	bAccountEvent = True	} )	-- 봉인된 천년 여우 꼬리

-- 시작된 역습2
GameEvent:AddReward( 639, { fEventTime =   0,  iRewardID = 10466	} )	-- 각성한 역습의 헤드윙 원소
GameEvent:AddReward( 640, { fEventTime =   0,  iRewardID = 10084,	cUnitType = UNIT_TYPE["UT_ARME"] } )	-- 망각의 드링크(7일권) 큐브 1개( 아이샤 )
GameEvent:AddReward( 641, { fEventTime =   0,  iRewardID = 10084,	cUnitType = UNIT_TYPE["UT_RAVEN"] } )	-- 망각의 드링크(7일권) 큐브 1개( 레이븐 )
GameEvent:AddReward( 642, { fEXPRate = 0.3,	cUnitType = UNIT_TYPE["UT_ARME"] } )	-- 아이샤, 경험치 +30%
GameEvent:AddReward( 643, { fEXPRate = 0.3,	cUnitType = UNIT_TYPE["UT_RAVEN"] } )	-- 레이븐, 경험치 +30%

-- 시즌3! 황금 주말 이벤트!
GameEvent:AddReward( 644, { fEventTime =   0,  iRewardID = 10465	} )	-- 봉인된 천년 여우 꼬리
GameEvent:AddReward( 645, { fEventTime =   10,  iRewardID = 10467,	bAccountEvent = True	} )	-- 황금 가열기 대박 큐브

-- 정파의 창
GameEvent:AddReward( 646, { fEventTime =   0,  iRewardID = 10468	} )	-- 정파의 기력

-- 천년여우의 전직 봉인 해제! (소선 & 제석천 런칭 이벤트 2)
GameEvent:AddReward( 647, { fEventTime =   0,  iRewardID = 10084,	cUnitType = UNIT_TYPE["UT_ARA"] } )	-- 망각의 드링크(7일권) 큐브 1개( 아라 )

-- 모래바람의 향기
GameEvent:AddReward( 648, { fEventTime =   0,  iRewardID = 10471 } )	-- 낡은 모래바람 윙츠 (14일권)

-- 역습의 나이프
GameEvent:AddReward( 649, { fEventTime =   0,  iRewardID = 10473	} )	-- 역습의 나이프(기간제)

-- 2013년 계사년 대박 큐브를 열어라!
GameEvent:AddReward( 650, { fEventTime =   0,  iRewardID = 10474	} )	-- 계사년 대박 열쇠, 계사년 초대박 큐브, 계사년 대박 큐브, 계사년 중박 큐브

-- 2013년 계사년 나의 운세는?
GameEvent:AddReward( 651, { fEventTime =   0,  iRewardID = 10475	} )	-- 신년 운세 캡슐

-- 2013년 계사년 대박 큐브를 열어라!
GameEvent:AddReward( 652, { fEventTime =   13,  iRewardID = 10476	} )	-- 계사년 대박 열쇠

-- 시작된 역습 3
GameEvent:AddReward( 653, { fEventTime =   0,  iRewardID = 10477	} )	-- 각성한 역습의 칼날 윙츠 원소
GameEvent:AddReward( 654, { fEventTime =   0,  iRewardID = 10084,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } )	-- 망각의 드링크(7일권) 큐브 1개( 엘소드 )
GameEvent:AddReward( 655, { fEventTime =   0,  iRewardID = 10084,	cUnitType = UNIT_TYPE["UT_CHUNG"] } )	-- 망각의 드링크(7일권) 큐브 1개( 청 )
GameEvent:AddReward( 656, { fEXPRate = 0.3,	cUnitType = UNIT_TYPE["UT_ELSWORD"] } )	-- 엘소드, 경험치 +30%
GameEvent:AddReward( 657, { fEXPRate = 0.3,	cUnitType = UNIT_TYPE["UT_CHUNG"] } )	-- 청, 경험치 +30%

-- 월리와 초콜릿 공장을 막아라!
GameEvent:AddReward( 658, { fEventTime =   0,  iRewardID = 10478,	bAccountEvent = True	} )	-- 월리의 초콜릿 공장 출입증 3개
GameEvent:AddReward( 659, { fEventTime =   0,  iRewardID = 10479,	bAccountEvent = True	} )	-- 월리의 초콜릿 공장 출입증 6개

-- 벨더 아카데미 학용품 고르기!
GameEvent:AddReward( 660, { fEventTime =   0,  iRewardID = 10480	} ) -- 다이어리 장바구니 (큐브) ,				벨더 아카데미 신학기 버닝 수정구 3개
GameEvent:AddReward( 661, { fEventTime =   0,  iRewardID = 10481	} ) -- 연필 장바구니 (큐브) ,					벨더 아카데미 신학기 버닝 수정구 3개
GameEvent:AddReward( 662, { fEventTime =   0,  iRewardID = 10482	} ) -- 2월 23일은 엘소드! (즉시 사용 큐브) ,	벨더 아카데미 신학기 버닝 수정구 5개
GameEvent:AddReward( 663, { fEventTime =   0,  iRewardID = 10483	} ) -- 2월 24일은 엘소드! (즉시 사용 큐브) ,	벨더 아카데미 신학기 버닝 수정구 5개
GameEvent:AddReward( 664, { fEventTime =   0,  iRewardID = 10484	} ) -- 필통 장바구니 (큐브) ,					벨더 아카데미 신학기 버닝 수정구 3개
GameEvent:AddReward( 665, { fEventTime =   0,  iRewardID = 10485	} ) -- 지우개 장바구니 (큐브) ,					벨더 아카데미 신학기 버닝 수정구 3개
GameEvent:AddReward( 666, { fEventTime =   0,  iRewardID = 10486	} ) -- 사인펜 장바구니 (큐브) ,					벨더 아카데미 신학기 버닝 수정구 3개

-- 벨더 아카데미 학용품 구매하기!
GameEvent:AddReward( 667, { fEventTime =   0,  iRewardID = 10487	} ) -- 다이어리 구매하기 ( 장바구니를 오픈하는 키 아이템 ), 벨더 아카데미 신학기 버닝 수정구 3개
GameEvent:AddReward( 668, { fEventTime =   0,  iRewardID = 10488	} ) -- 연필 구매하기 ( 장바구니를 오픈하는 키 아이템 ), 벨더 아카데미 신학기 버닝 수정구 3개
GameEvent:AddReward( 669, { fEventTime =   0,  iRewardID = 10489	} ) -- 3월 2일은 엘소드! 큐브 ( 즉시 사용 큐브 ), 벨더 아카데미 신학기 버닝 수정구 5개
GameEvent:AddReward( 670, { fEventTime =   0,  iRewardID = 10490	} ) -- 3월 3일은 엘소드! 큐브 ( 즉시 사용 큐브 ), 벨더 아카데미 신학기 버닝 수정구 5개
GameEvent:AddReward( 671, { fEventTime =   0,  iRewardID = 10491	} ) -- 필통 구매하기 ( 장바구니를 오픈하는 키 아이템 ), 벨더 아카데미 신학기 버닝 수정구 3개
GameEvent:AddReward( 672, { fEventTime =   0,  iRewardID = 10492	} ) -- 지우개 구매하기 ( 장바구니를 오픈하는 키 아이템 ), 벨더 아카데미 신학기 버닝 수정구 3개
GameEvent:AddReward( 673, { fEventTime =   0,  iRewardID = 10493	} ) -- 사인펜 구매하기 ( 장바구니를 오픈하는 키 아이템 ), 벨더 아카데미 신학기 버닝 수정구 3개

-- 벨더 아카데미 새학기 새만남!!
GameEvent:AddReward( 674, { fEventTime =   0,  iRewardID = 10494	} ) -- 불량배에게서 되찾은 코인

-- 필드를 지켜내라!
GameEvent:AddReward( 675, { fEventTime =   0,  iRewardID = 10495	} ) -- 필드 중간 보스 사냥의 증거 1개
GameEvent:AddReward( 676, { fEventTime =   0,  iRewardID = 10496	} ) -- 필드 중간 보스 사냥의 증거 3개

-- 화이트 데이 사탕 바구니!
GameEvent:AddReward( 677, { fEventTime =   0,  iRewardID = 10497,	bAccountEvent = True	} ) -- 화이트 데이 사탕 바구니

-- 친구 찾기
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10501,	iMinLevel = 	1	,   iMaxLevel =	13	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10502,	iMinLevel = 	14	,   iMaxLevel =	19	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10503,	iMinLevel = 	20	,   iMaxLevel =	25	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10504,	iMinLevel = 	26	,   iMaxLevel =	29	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10505,	iMinLevel = 	30	,   iMaxLevel =	33	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10506,	iMinLevel = 	34	,   iMaxLevel =	37	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10507,	iMinLevel = 	38	,   iMaxLevel =	45	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10508,	iMinLevel = 	46	,   iMaxLevel =	49	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10509,	iMinLevel = 	50	,   iMaxLevel =	55	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10511,	iMinLevel = 	56	,   iMaxLevel =	59	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward( 678, { fEventTime =   0,  iRewardID = 10512,	iMinLevel = 	60	,   iMaxLevel =	65	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

--퀘스트 아이템 드롭 50%
GameEvent:AddReward( 679, { m_fQuestItemDropRate = 0.5 } )

--퀘스트 아이템 드롭 100%
GameEvent:AddReward( 680, { m_fQuestItemDropRate = 1.0 } )

-- pc 방 선물 큐브
GameEvent:AddReward( 681, { fEventTime =   30,  iRewardID = 10513, bPcBangEvent = True } )

-- 앙벤 창립 기념 선물 상자
GameEvent:AddReward( 682, { fEventTime =   30,  iRewardID = 10514,	bAccountEvent = True } )

-- 벨더 비밀던전 무료 입장
GameEvent:AddReward( 683, { iDungeonID = DUNGEON_ID["DI_VELDER_SECRET_COMMON"],	iDungeonMode = DUNGEON_MODE["DM_SECRET_NORMAL"]		} )
GameEvent:AddReward( 684, { iDungeonID = DUNGEON_ID["DI_VELDER_SECRET_HELL"],	iDungeonMode = DUNGEON_MODE["DM_SECRET_HELL"]		} )

-- 엘레강스 레인보우 요일별 혜택: AP, RankingPoint 2배 이벤트
GameEvent:AddReward( 685, {	fVPRate = 1.0,	fRankingPointRate = 1.0	} )

-- 엘리오스를 푸르게 푸르게!
GameEvent:AddReward( 686, { fEventTime =   0,  iRewardID = 10519,	bAccountEvent = True	} ) -- 푸른 엘리오스 큐브

-- 만우절 복장 큐브
GameEvent:AddReward( 687, { fEventTime =   0,  iRewardID = 10520 } )	-- 만우절 복장 큐브

-- 벨더 아카데미 중간 시험 주간
GameEvent:AddReward( 688, { fEventTime =   0,  iRewardID = 10521 } )	-- 최우수상 큐브, 우수상 큐브, 장려상 큐브, 노력상 큐브, 벨더 아카데미 시험지

-- 벨더 아카데미 중간 시험 주간
GameEvent:AddReward( 689, { fEventTime =   0,  iRewardID = 10522 } )	-- 벨더 모범생의 열공 큐브

-- 돌아온 어둠!
GameEvent:AddReward( 690, { fEventTime =   0,  iRewardID = 10523,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_ALL"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"] + CRITERION_USER_TYPE["CUT_NEW"],	wstrCriterionDateBegin = '2013-04-18 07:00:00',	wstrCriterionDateEnd = '2013-05-02 07:00:00' } )	-- 강마룡 미니미
GameEvent:AddReward( 691, { fEventTime =   0,  iRewardID = 10524,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_ALL"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_EXIST"],										wstrCriterionDateBegin = '2013-04-18 07:00:00',	wstrCriterionDateEnd = '2013-05-02 07:00:00' } )	-- 강마룡 미니미의 결정

-- 벨더 아카데미 중간 시험 주간
GameEvent:AddReward( 692, { fEventTime =   30,  iRewardID = 10525,	bAccountEvent = True } )	-- 벨더 모범생의 재충전 큐브

-- 어둠의 문 오픈 확률 이벤트
GameEvent:AddReward( 693, {	fDefenseDungeonOpenRate = 2.0	} )	-- 2배

-- 어린이날 이벤트
GameEvent:AddReward( 694, { fEventTime =   0,  iRewardID = 10526 ,	bAccountEvent = True } )	-- 아리엘의 놀라운 엘리오스 1개
GameEvent:AddReward( 695, { fEventTime =   0,  iRewardID = 10527  } )	-- 카밀라의 강력한 엘리오스 칭호
GameEvent:AddReward( 696, { fEventTime =   0,  iRewardID = 10528,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"] } )	--

-- 라이딩 펫 이벤트
GameEvent:AddReward( 697, { fEventTime =   0,  iRewardID = 10529  } )	-- 라이딩 헬멧

-- 새콤달콤 뽀피트의 러브 레터
GameEvent:AddReward( 698, { fEventTime =   0,  iRewardID = 10533  } )	-- 사랑의 리퀘스트

-- 공존의 축제
GameEvent:AddReward( 699, { fEventTime =   30,  iRewardID = 10534, bPcBangEvent = True	} )	-- 공존의 축제 PC방 큐브

-- 아이템 개편 런칭 이벤트
GameEvent:AddReward( 700, { fEventTime =   0,  iRewardID = 10536	} )	-- [코보] 축복받은 시공간의 주문서 큐브, The 에볼루션 윙, [코보] 축복받은 시공간의 주문서 (연동레벨)

-- 아이템 개편 런칭 이벤트
GameEvent:AddReward( 701, { fEventTime =   0,  iRewardID = 10537,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- Back to the 에볼루션 큐브

-- 아이템 개편 런칭 이벤트
GameEvent:AddReward( 702, { fEventTime =   0,  iRewardID = 10538	} )	-- [코보] 축복받은 시공간의 주문서 큐브, [코보] 축복받은 시공간의 주문서 (연동레벨)

-- 최강의 전학생들, 크레용팝의 벨더 아카데미 등장!
GameEvent:AddReward( 703, { fEventTime =   0,  iRewardID = 10539,	bGiveOnce = False	} )	-- 크레용
GameEvent:AddReward( 704, { fEventTime =   0,  iRewardID = 10539	} )	-- 크레용

GameEvent:AddReward( 705, { fEventTime =   0,  iRewardID = 10540	} )	-- 크레용 PoP 인기 투표 용지

--스킬 트리 개편 전야 이벤트 : 준비의 시간
GameEvent:AddReward( 706, { fEventTime =   0,  iRewardID = 10541	} )	-- 준비의 시간

-- 스킬 트리 개편 런칭 이벤트 : 준비 완료, 질풍가도!
GameEvent:AddReward( 707, { fEventTime =   0,  iRewardID = 10547,	bGiveOnce = False	} )	-- 질풍가도

GameEvent:AddReward( 708, { fEventTime =   0,  iRewardID = 10548,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 아크엔젤 올세트(30일권) 큐브, 스킬 슬롯 체인지 메달(30일권) 큐브, 마법의 목걸이II(30일권) 큐브, 기술의 반지 4종(30일권) 큐브

GameEvent:AddReward(	709,	{ iRewardID = 10549,	iMinLevel =	2,	iMaxLevel =	2	} )	-- 근성 포션 20개
GameEvent:AddReward(	709,	{ iRewardID = 10530,	iMinLevel =	15,	iMaxLevel =	15	} )	-- 축복의 메달 1일권 큐브 1개
GameEvent:AddReward(	709,	{ iRewardID = 10532,	iMinLevel =	30,	iMaxLevel =	30	} )	-- 나소드 판넬 가드-Ver.K2(1개권) 큐브
GameEvent:AddReward(	709,	{ iRewardID = 10550,	iMinLevel =	45,	iMaxLevel =	45	} )	-- 올인원 얼음 조각상 가열기 5개, 봉인된 올인원 얼음 조각상 5개
GameEvent:AddReward(	709,	{ iRewardID = 10551,	iMinLevel =	55,	iMaxLevel =	55	} )	-- 보잉 선글라스 랜덤 큐브
GameEvent:AddReward(	709,	{ iRewardID = 10552,	iMinLevel =	65,	iMaxLevel =	65	} )	-- 올인원 얼음 조각상 가열기 7개, 봉인된 올인원 얼음 조각상 7개

GameEvent:AddReward(	710, { fEventTime =   0,  iRewardID = 10547	} )	-- 질풍가도

-- 집합하라 !붉은 기사단!
GameEvent:AddReward(	711,	{ iRewardID = 10556,	iMinLevel =	20,	iMaxLevel =	20	} )	-- 축복의 메달 1일권 큐브 2개
GameEvent:AddReward(	711,	{ iRewardID = 10557,	iMinLevel =	30,	iMaxLevel =	30	} )	-- [코보] 상급 붉은 기사단 전용 강화의 부적 lv.10
GameEvent:AddReward(	711,	{ iRewardID = 10558,	iMinLevel =	40,	iMaxLevel =	40	} )	-- 나소드 판넬 가드 ver.k2 무기 액세서리 큐브 1개
GameEvent:AddReward(	711,	{ iRewardID = 10559,	iMinLevel =	50,	iMaxLevel =	50	} )	-- 코보 익스프레스 VIP 티켓 (30일)
GameEvent:AddReward(	711,	{ iRewardID = 10560,	iMinLevel =	60,	iMaxLevel =	60	} )	-- 보잉 선크라스 1개권 랜덤 큐브 1개
GameEvent:AddReward(	711,	{ iRewardID = 10561,	iMinLevel =	65,	iMaxLevel =	65	} )	-- [코보]올인원 얼음 조각상 가열기 5개, 봉인된 올인원 얼음 조각상 5개

GameEvent:AddReward(	712,	{ fEventTime =   0,  iRewardID = 10562	} )			-- 집합하라 !붉은 기사단! -- [코보] 초급 붉은 기사단 전용 강화의 부적 lv.10, 근성 포션 20개 지급

-- 매일 꼬리를 무는 타임 이벤트
GameEvent:AddReward(	713,	{ fEventTime =   30,	 iRewardID = 10563	} ) -- [코보] 축복받은 시공간의 주문서 교환권
GameEvent:AddReward(	714,	{ fEventTime =   60,	 iRewardID = 10563	} ) -- [코보] 축복받은 시공간의 주문서 교환권
GameEvent:AddReward(	715,	{ fEventTime =   90,	 iRewardID = 10563	} ) -- [코보] 축복받은 시공간의 주문서 교환권
GameEvent:AddReward(	716,	{ fEventTime =   120,	 iRewardID = 10563	} ) -- [코보] 축복받은 시공간의 주문서 교환권

-- 붉은 기사단 쇼케이스!
GameEvent:AddReward(	717,	{ iRewardID = 10564,	iMinLevel =	30,	iMaxLevel =	30,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_ALL"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_NEW"],	wstrCriterionDateBegin = '2013-07-04 07:00:00',	wstrCriterionDateEnd = '2013-08-07 07:00:00' } )	-- 붉은 기사단 초대권

-- 새로운 붉은 기사단을 위하여!
GameEvent:AddReward(	718,	{ iRewardID = 10565,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NOT_JUMPING"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_NEW"],	wstrCriterionDateBegin = '2013-07-04 07:00:00',	wstrCriterionDateEnd = '2013-08-29 07:00:00' } ) -- 망각의 드링크(7일권) 10개 큐브

-- 돌아온 붉은 기사단을 위하여!
GameEvent:AddReward(	719,	{ iRewardID = 10566,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} ) -- 스킬 슬롯 체인지 메달 30일권, 마법의 목걸이(II) 30일권, 축복의 메달 1일권 큐브 3개, 정체불명의 고대 화석 랜덤 큐브 (7일권)
GameEvent:AddReward(	720,	{ iRewardID = 10570,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} ) -- 망각의 드링크(7일권) 10개 큐브
GameEvent:AddReward(	721,	{ iRewardID = 10567,	bAccountEvent = True,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} ) -- [코보]올인원 얼음 조각상 가열기 2개, 봉인된 올인원 얼음 조각상 2개

GameEvent:AddReward(	722,	{ iRewardID = 10567,	iMinLevel =	50,	iMaxLevel =	50,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- [코보]올인원 얼음 조각상 가열기 2개, 봉인된 올인원 얼음 조각상 2개
GameEvent:AddReward(	722,	{ iRewardID = 10568,	iMinLevel =	60,	iMaxLevel =	60,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- [코보]올인원 얼음 조각상 가열기 3개, 봉인된 올인원 얼음 조각상 3개
GameEvent:AddReward(	722,	{ iRewardID = 10561,	iMinLevel =	65,	iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- [코보]올인원 얼음 조각상 가열기 5개, 봉인된 올인원 얼음 조각상 5개

-- 돌아온 붉은 기사단을 위하여!
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10501,	iMinLevel = 	1	,   iMaxLevel =	13,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10502,	iMinLevel = 	14	,   iMaxLevel =	19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10503,	iMinLevel = 	20	,   iMaxLevel =	25,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10504,	iMinLevel = 	26	,   iMaxLevel =	29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10505,	iMinLevel = 	30	,   iMaxLevel =	33,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10506,	iMinLevel = 	34	,   iMaxLevel =	37,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10507,	iMinLevel = 	38	,   iMaxLevel =	45,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10508,	iMinLevel = 	46	,   iMaxLevel =	49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10509,	iMinLevel = 	50	,   iMaxLevel =	55,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10511,	iMinLevel = 	56	,   iMaxLevel =	59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward(	723, { fEventTime =   0,  iRewardID = 10512,	iMinLevel = 	60	,   iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

-- 집합하라 !붉은 기사단!
GameEvent:AddReward(	725, { fEventTime =   30,  iRewardID = 10569,	bAccountEvent = True	} )  -- 붉은 기사단 출석 인증

-- 점핑 캐릭터 이벤트
GameEvent:AddReward(	726, { iMinLevel =	1,	iMaxLevel =	19,	iUnitLevel = 40,	bAccountEvent = True,	cUnitType = UNIT_TYPE["UT_ELSWORD"]	} )  -- 엘소드 점핑!
GameEvent:AddReward(	727, { iMinLevel =	1,	iMaxLevel =	19,	iUnitLevel = 40,	bAccountEvent = True,	cUnitType = UNIT_TYPE["UT_ARME"]	} )  -- 아이샤 점핑!
GameEvent:AddReward(	728, { iMinLevel =	1,	iMaxLevel =	19,	iUnitLevel = 40,	bAccountEvent = True,	cUnitType = UNIT_TYPE["UT_LIRE"]	} )  -- 레나 점핑!
GameEvent:AddReward(	729, { iMinLevel =	1,	iMaxLevel =	19,	iUnitLevel = 40,	bAccountEvent = True,	cUnitType = UNIT_TYPE["UT_RAVEN"]	} )  -- 레이븐 점핑!
GameEvent:AddReward(	730, { iMinLevel =	1,	iMaxLevel =	19,	iUnitLevel = 40,	bAccountEvent = True,	cUnitType = UNIT_TYPE["UT_EVE"]		} )  -- 이브 점핑!
GameEvent:AddReward(	731, { iMinLevel =	1,	iMaxLevel =	19,	iUnitLevel = 40,	bAccountEvent = True,	cUnitType = UNIT_TYPE["UT_CHUNG"]	} )  -- 청 점핑!
GameEvent:AddReward(	732, { iMinLevel =	1,	iMaxLevel =	19,	iUnitLevel = 40,	bAccountEvent = True,	cUnitType = UNIT_TYPE["UT_ARA"]		} )  -- 아라 점핑!

-- 퀘스트 아이템 드롭 3배
GameEvent:AddReward(	733, { m_fQuestItemDropRate = 2.0 } )

-- 2013년 여름방학 이벤트
GameEvent:AddReward(	734, { fEventTime =   20,	iRewardID = 10568,	bAccountEvent = True	} )	-- [코보]올인원 얼음 조각상 가열기 3개, 봉인된 올인원 얼음 조각상 3개
GameEvent:AddReward(	735, { fEventTime =   20,	iRewardID = 10571,	bAccountEvent = True	} )	-- 정체불명의 고대 화석 3개, [코보] 고대 화석 판별기 3개
GameEvent:AddReward(	736, { fEventTime =   20,	iRewardID = 10572,	bAccountEvent = True	} )	-- 넥슨 캐시 큐브 1개

-- 햇츠온 1차 제휴 이벤트
GameEvent:AddReward(	737, { fEventTime =   0,	iRewardID = 10573  } )	-- 스팅코 가방 (1일권)

GameEvent:AddReward(	738, { fEventTime =   0,	iRewardID = 10574  } )	-- 스팅코 모자 (1일권)

GameEvent:AddReward(	739, { fEventTime =   0,	iRewardID = 10575  } )	-- 에보니 가방 (1일권)

GameEvent:AddReward(	740, { fEventTime =   0,	iRewardID = 10576  } )	-- 에보니 모자 (1일권)

GameEvent:AddReward(	741, { fEventTime =   0,	iRewardID = 10577  } )	-- 햇츠온 큐브

GameEvent:AddReward(	742, { fEventTime =   0,	iRewardID = 10578  } )	-- 스팅코 가방 (1일권), 스팅코 모자 (1일권), 햇츠온 큐브

GameEvent:AddReward(	743, { fEventTime =   0,	iRewardID = 10579  } )	-- 에보니 가방 (1일권), 에보니 모자 (1일권), 햇츠온 큐브

-- 엘리시스 런칭 이벤트
GameEvent:AddReward(	744, { fEventTime =   0,	iRewardID = 10580,	cUnitType = UNIT_TYPE["UT_ELESIS"],	bAccountEvent = True	} )	-- 모든 마법석 랜덤 큐브

GameEvent:AddReward(	745, { fEventTime =   0,	iRewardID = 10581,	cUnitType = UNIT_TYPE["UT_ELESIS"]	} )	-- 나소드 판넬 가드-Ver.K2 (엘리시스)

-- 돌아온 붉은 기사단을 위하여! ( 엘소드 )
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10501,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	1	,   iMaxLevel =	13,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10502,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	14	,   iMaxLevel =	19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10503,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	20	,   iMaxLevel =	25,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10504,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	26	,   iMaxLevel =	29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10505,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	30	,   iMaxLevel =	33,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10506,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	34	,   iMaxLevel =	37,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10507,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	38	,   iMaxLevel =	45,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10508,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	46	,   iMaxLevel =	49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10509,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	50	,   iMaxLevel =	55,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10511,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	56	,   iMaxLevel =	59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward(	746, { fEventTime =   0,  iRewardID = 10512,	cUnitType = UNIT_TYPE["UT_ELSWORD"],	iMinLevel = 	60	,   iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

-- 돌아온 붉은 기사단을 위하여! ( 아이샤 )
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10501,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	1	,   iMaxLevel =	13,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10502,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	14	,   iMaxLevel =	19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10503,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	20	,   iMaxLevel =	25,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10504,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	26	,   iMaxLevel =	29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10505,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	30	,   iMaxLevel =	33,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10506,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	34	,   iMaxLevel =	37,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10507,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	38	,   iMaxLevel =	45,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10508,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	46	,   iMaxLevel =	49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10509,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	50	,   iMaxLevel =	55,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10511,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	56	,   iMaxLevel =	59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward(	747, { fEventTime =   0,  iRewardID = 10512,	cUnitType = UNIT_TYPE["UT_ARME"],	iMinLevel = 	60	,   iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

-- 돌아온 붉은 기사단을 위하여! ( 레나 )
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10501,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	1	,   iMaxLevel =	13,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10502,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	14	,   iMaxLevel =	19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10503,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	20	,   iMaxLevel =	25,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10504,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	26	,   iMaxLevel =	29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10505,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	30	,   iMaxLevel =	33,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10506,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	34	,   iMaxLevel =	37,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10507,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	38	,   iMaxLevel =	45,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10508,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	46	,   iMaxLevel =	49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10509,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	50	,   iMaxLevel =	55,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10511,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	56	,   iMaxLevel =	59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward(	748, { fEventTime =   0,  iRewardID = 10512,	cUnitType = UNIT_TYPE["UT_LIRE"],	iMinLevel = 	60	,   iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

-- 돌아온 붉은 기사단을 위하여! ( 레이븐 )
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10501,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	1	,   iMaxLevel =	13,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10502,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	14	,   iMaxLevel =	19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10503,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	20	,   iMaxLevel =	25,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10504,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	26	,   iMaxLevel =	29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10505,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	30	,   iMaxLevel =	33,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10506,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	34	,   iMaxLevel =	37,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10507,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	38	,   iMaxLevel =	45,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10508,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	46	,   iMaxLevel =	49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10509,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	50	,   iMaxLevel =	55,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10511,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	56	,   iMaxLevel =	59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward(	749, { fEventTime =   0,  iRewardID = 10512,	cUnitType = UNIT_TYPE["UT_RAVEN"],	iMinLevel = 	60	,   iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

-- 돌아온 붉은 기사단을 위하여! ( 이브 )
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10501,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	1	,   iMaxLevel =	13,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10502,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	14	,   iMaxLevel =	19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10503,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	20	,   iMaxLevel =	25,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10504,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	26	,   iMaxLevel =	29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10505,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	30	,   iMaxLevel =	33,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10506,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	34	,   iMaxLevel =	37,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10507,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	38	,   iMaxLevel =	45,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10508,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	46	,   iMaxLevel =	49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10509,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	50	,   iMaxLevel =	55,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10511,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	56	,   iMaxLevel =	59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward(	750, { fEventTime =   0,  iRewardID = 10512,	cUnitType = UNIT_TYPE["UT_EVE"],	iMinLevel = 	60	,   iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

-- 돌아온 붉은 기사단을 위하여! ( 청 )
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10501,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	1	,   iMaxLevel =	13,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10502,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	14	,   iMaxLevel =	19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10503,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	20	,   iMaxLevel =	25,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10504,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	26	,   iMaxLevel =	29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10505,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	30	,   iMaxLevel =	33,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10506,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	34	,   iMaxLevel =	37,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10507,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	38	,   iMaxLevel =	45,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10508,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	46	,   iMaxLevel =	49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10509,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	50	,   iMaxLevel =	55,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10511,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	56	,   iMaxLevel =	59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward(	751, { fEventTime =   0,  iRewardID = 10512,	cUnitType = UNIT_TYPE["UT_CHUNG"],	iMinLevel = 	60	,   iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

-- 돌아온 붉은 기사단을 위하여! ( 아라 )
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10501,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	1	,   iMaxLevel =	13,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 견습 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10502,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	14	,   iMaxLevel =	19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초보 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10503,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	20	,   iMaxLevel =	25,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 초급 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10504,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	26	,   iMaxLevel =	29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 중급 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10505,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	30	,   iMaxLevel =	33,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 상급 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10506,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	34	,   iMaxLevel =	37,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 최상급 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10507,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	38	,   iMaxLevel =	45,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 노련한 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10508,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	46	,   iMaxLevel =	49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 숙련된 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10509,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	50	,   iMaxLevel =	55,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 세련된 모험가의 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10511,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	56	,   iMaxLevel =	59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 절정의 모험가 풀세트 큐브
GameEvent:AddReward(	752, { fEventTime =   0,  iRewardID = 10512,	cUnitType = UNIT_TYPE["UT_ARA"],	iMinLevel = 	60	,   iMaxLevel =	65,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 엘리오스 완벽한 모험가 풀세트 큐브

-- 노란 사과의 큐브
GameEvent:AddReward(	753, { fEventTime =   0,	iRewardID = 10582,	bAccountEvent = True	} )	-- 노란 사과의 큐브

-- 2013년 여름방학 타임 이벤트
GameEvent:AddReward(	754, { fEventTime =   0,	iRewardID = 10583,	bAccountEvent = True	} )	-- 붉은 기사단의 축복 큐브
GameEvent:AddReward(	755, { fEventTime =   0,	iRewardID = 10572,	bAccountEvent = True	} )	-- 넥슨 캐시 큐브 1개

-- 궁극의 힘!
GameEvent:AddReward(	756, { fEventTime =   0,	iRewardID = 10584,	iMinLevel =	60	} )	-- 궁극의 힘

-- 궁극의 힘을 향해!
GameEvent:AddReward(	757, { fEventTime =   0,	iRewardID = 10585,	iMaxLevel =	59	} )	-- 축복의 메달 1일권 큐브 3개

-- 하이퍼 액티브 스킬 추가로 인한 망각의 드링크 지급
GameEvent:AddReward(	758, { fEventTime =   0,	iRewardID = 10586	} )	-- 망각의 드링크(7일권) 큐브

-- 샌더, 카리스의 심장을 잡아라!
GameEvent:AddReward(	759, { fEventTime =   0,	iRewardID = 10587,	iMinLevel = 	60	,   byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"], byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 카리스의 심장 큐브

-- 기공 발사! 매일매일 기공을 받아라!
GameEvent:AddReward(	760, { fEventTime =   0,	iRewardID = 10359, bAccountEvent = True	} )	-- 축복의 메달 1일권 큐브
GameEvent:AddReward(	761, { fEventTime =   0,	iRewardID = 10590, bAccountEvent = True	} )	-- [코보] 축복받은 시공간의 주문서 큐브
GameEvent:AddReward(	762, { fEventTime =   0,	iRewardID = 10591, bAccountEvent = True	} )	-- 축복받은 시공간의 주문서 (연동레벨) 3개
GameEvent:AddReward(	763, { fEventTime =   0,	iRewardID = 10592, bAccountEvent = True	} )	-- 특별 출입증 교환권 5개
GameEvent:AddReward(	764, { fEventTime =   0,	iRewardID = 10427, bAccountEvent = True	} )	-- 대리 판매 보증서(7일권)
GameEvent:AddReward(	765, { fEventTime =   0,	iRewardID = 10220, bAccountEvent = True	} )	-- 고성능 나소드 메가폰 2개

-- 루토의 주사위를 흔들어라!
GameEvent:AddReward(	766, { fEventTime =   0,	iRewardID = 10593,	bGiveOnce = False	} )	-- 루토의 주사위 큐브, 루토의 출입증 3개

-- 루토의 주말 주사위 타임!
GameEvent:AddReward(	767, { fEventTime =   0,	iRewardID = 10594	} )	-- 루토의 주사위 큐브

-- 뜨거운 한가위!
GameEvent:AddReward(	768, {	fDropRate =  1.5	} ) -- 드롭률 50% 증가

-- PC방은 마음의 고향
GameEvent:AddReward(	769, {	bHenirRewardUnLimited = True,	bPcBangEvent = True	} )	-- PC방 유저 헤니르 보상 획득 무제한
GameEvent:AddReward(	770, {	fEventTime = 10,	iRewardID = 10230,	bPcBangEvent = True,	bAccountEvent = True	} ) -- PC방 VIP 큐브 1개

-- 소원을 말해봐
GameEvent:AddReward(	771, {	fEventTime = 10,	iRewardID = 10595,	bAccountEvent = True	} )	-- 한가위 송편 1개
GameEvent:AddReward(	772, {	fEventTime = 10,	iRewardID = 10596,	bAccountEvent = True	} )	-- 한가위 송편 2개

-- 반가운 모험가를 위한 추석선물
GameEvent:AddReward(	773, {	fEventTime = 0,	iRewardID = 10597,	bAccountEvent = True,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_ALL"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"] + CRITERION_USER_TYPE["CUT_NEW"],	wstrCriterionDateBegin = '2013-09-12 07:00:00',	wstrCriterionDateEnd = '2013-09-26 07:00:00' } ) -- 엘리오스 추석 선물세트 1개

-- 도전 출석왕
GameEvent:AddReward(	774, {	fEventTime = 30,	iRewardID = 10598,	bAccountEvent = True	} )	-- [코보] 올인원 얼음 조각상 가열기 1개 큐브

GameEvent:AddReward(	775, {	fEventTime = 30,	iRewardID = 10599,	bAccountEvent = True	} )	-- [코보] 올인원 얼음 조각상 가열기 2개 큐브

-- 헤니르의 시공 상향 보상
GameEvent:AddReward(	776, {	fEventTime = 0,	iRewardID = 10600,	bAccountEvent = True	} )	-- [코보] 축복받은 시공간의 주문서 큐브 3개

-- 초심자의 행운( 엘리시스 2차 전직 이벤트 )
GameEvent:AddReward(	777, {	fEventTime = 0,	iRewardID = 10601,	iMinLevel = 15,	bAccountEvent = True,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_NEW"],	wstrCriterionDateBegin = '2013-10-10 07:00:00',	wstrCriterionDateEnd = '2013-10-24 07:00:00'		} ) -- 넥슨캐시 10,000원 추첨권

-- 주고 또 주고( 엘리시스 2차 전직 이벤트 )
GameEvent:AddReward(	778, {	fEventTime = 0,	iRewardID = 10602,	bAccountEvent = True,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} ) -- [코보] 올인원 얼음 조각상 가열기 6개 큐브

-- 판타지 러너즈에서 엘소드가 달린다! (1)( 판타지 러너 제휴 이벤트 )
GameEvent:AddReward(	779, {	fEventTime = 0,	iRewardID = 0	} )

-- 나 엘소드로 돌아갈래~ ( 20131024_할로윈이벤트 )
GameEvent:AddReward(	780, {	fEventTime = 0,		iRewardID = 10604,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )

-- 트릭 or 트릿! ( 20131024_할로윈이벤트 )
GameEvent:AddReward(	781, {	fEventTime = 10,	iRewardID = 10605	} )	-- 할로윈 캔디 꾸러미, 몬스터 파티 큐브

GameEvent:AddReward(	782, {	fEventTime = 10,	iRewardID = 10606	} )	-- 할로윈 캔디 꾸러미 2개, 몬스터 파티 큐브

-- 마녀 뽀루의 초대장 ( 20131024_할로윈이벤트 )
GameEvent:AddReward(	783, {	fEventTime = 10,	iRewardID = 10607,	bAccountEvent = True	} )	-- 할로윈 마녀뽀루의 초대장 3개

GameEvent:AddReward(	784, {	fEventTime = 10,	iRewardID = 10608,	bAccountEvent = True	} )	-- 할로윈 마녀뽀루의 초대장 6개

-- 밸런스개편이벤트(내 머리 속의 지우개) (20131107_밸런스개편이벤트기획서)
-- 이벤트 기간 중 접속 시 접속한 캐릭터에 우편으로 [코보]망각의 드링크를 1개 지급한다(캐릭터 당 1회 한정)
GameEvent:AddReward(	785, { fEventTime =   0,	iRewardID = 10609  } )	-- [코보]망각의 드링크 (ItemID: 160946)

-- 수능이벤트 (복귀하라!1993) (20131107_밸런스개편이벤트기획서)
-- 일일접속 후 10분 경과 시 모든 유저에게 수능대박 소원상자를 일자별로 계정당 1회 지급한다.
GameEvent:AddReward(	786, {	fEventTime = 10,	iRewardID = 10610,	bAccountEvent = True	} )	-- 수능대박 소원상자 1개 (ItemID: 160945)
GameEvent:AddReward(	787, {	fEventTime = 10,	iRewardID = 10611,	bAccountEvent = True	} )	-- 수능대박 소원상자 2개 (ItemID: 160945)

-- 이벤트 기간동안 복귀한 유저가 접속 시 수능대박 소원상자 5개를 우편으로 1회 제공한다.
GameEvent:AddReward(	788, {	fEventTime = 0,	iRewardID = 10612,	bAccountEvent = True, byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )	-- 수능대박 소원상자 5개

GameEvent:AddReward(	789, { iHenirRewardEventCount = 1 } )			-- 헤니르 보상 획득 1회 추가

-- 따뜻한 겨울나기( 20131128_겨울방학전야이벤트 )
GameEvent:AddReward(	790, {	fEventTime = 10,	iRewardID = 10614	} )	-- 따뜻한 겨울 머플러 1개, 따뜻한 겨울 털장갑 1개

-- 성장하라, 70까지!( 20131205_1차 겨울방학이벤트 )
GameEvent:AddReward(	791, {	fEventTime = 0,	iRewardID = 10615,	iMinLevel = 1,	iMaxLevel = 9,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"] + CRITERION_USER_TYPE["CUT_EXIST"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_EXIST"],	wstrCriterionDateBegin = '2013-12-05 07:00:00',	wstrCriterionDateEnd = '2014-02-27 07:00:00'	} )	-- 엘더 마을 큐브
GameEvent:AddReward(	791, {	fEventTime = 0,	iRewardID = 10616,	iMinLevel = 10,	iMaxLevel = 19,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"] + CRITERION_USER_TYPE["CUT_EXIST"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_EXIST"],	wstrCriterionDateBegin = '2013-12-05 07:00:00',	wstrCriterionDateEnd = '2014-02-27 07:00:00'	} )	-- 베스마 마을 큐브
GameEvent:AddReward(	791, {	fEventTime = 0,	iRewardID = 10617,	iMinLevel = 20,	iMaxLevel = 29,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"] + CRITERION_USER_TYPE["CUT_EXIST"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_EXIST"],	wstrCriterionDateBegin = '2013-12-05 07:00:00',	wstrCriterionDateEnd = '2014-02-27 07:00:00'	} )	-- 알테라 마을 큐브
GameEvent:AddReward(	791, {	fEventTime = 0,	iRewardID = 10618,	iMinLevel = 30,	iMaxLevel = 39,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"] + CRITERION_USER_TYPE["CUT_EXIST"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_EXIST"],	wstrCriterionDateBegin = '2013-12-05 07:00:00',	wstrCriterionDateEnd = '2014-02-27 07:00:00'	} )	-- 페이타 마을 큐브
GameEvent:AddReward(	791, {	fEventTime = 0,	iRewardID = 10619,	iMinLevel = 40,	iMaxLevel = 49,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"] + CRITERION_USER_TYPE["CUT_EXIST"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_EXIST"],	wstrCriterionDateBegin = '2013-12-05 07:00:00',	wstrCriterionDateEnd = '2014-02-27 07:00:00'	} )	-- 벨더 마을 큐브
GameEvent:AddReward(	791, {	fEventTime = 0,	iRewardID = 10620,	iMinLevel = 50,	iMaxLevel = 59,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"] + CRITERION_USER_TYPE["CUT_EXIST"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_EXIST"],	wstrCriterionDateBegin = '2013-12-05 07:00:00',	wstrCriterionDateEnd = '2014-02-27 07:00:00'	} )	-- 하멜 마을 큐브
GameEvent:AddReward(	791, {	fEventTime = 0,	iRewardID = 10621,	iMinLevel = 60,	iMaxLevel = 69,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"] + CRITERION_USER_TYPE["CUT_EXIST"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_EXIST"],	wstrCriterionDateBegin = '2013-12-05 07:00:00',	wstrCriterionDateEnd = '2014-02-27 07:00:00'	} )	-- 샌더 마을 큐브

-- 뮤의 출석체크 이벤트( 20131205_1차 겨울방학이벤트 )
GameEvent:AddReward(	792, {	fEventTime = 10,	iRewardID = 10622,	bAccountEvent = True	} )	-- 뮤의 출석도장

-- 겨울 나그네 이벤트( 20131205_1차 겨울방학이벤트 )
GameEvent:AddReward(	793, {	fDropRate =  1.3	} ) -- 드롭률 30% 증가

-- 엘리오스 대륙에 어서 오세요!( 20131205_1차 겨울방학이벤트 )
GameEvent:AddReward(	794, {	fEventTime = 0,	iRewardID = 10623,	bAccountEvent = True,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_COMEBACK"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_COMEBACK"]	} )

-- 2014-01-14 ESTW_시험기간 이벤트
GameEvent:AddReward(	795, {	fEventTime = 10,	iRewardID = 795,	iMinLevel = 10,	 } )	-- 에너지 드링크, 시험 대박 기원 큐브

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


-- 궁극의 힘
GameEvent:AddReward(	2046, { fEventTime =   10,	iRewardID = 2046,	iMinLevel = 10, } )	-- 루리엘의 마나 활성화 포션 등
GameEvent:AddReward(	2047, { fEventTime =   20,	iRewardID = 2047,	iEventItemID = 60008063,  } )	-- 루리엘의 마나 활성화 포션 등

-- 2014년 발렌타인데이 이벤트
GameEvent:AddReward( 10308,	{ fEventTime =  14,	iRewardID = 1000307, bGiveOnce =  True, iMinLevel = 10 } ) -- 헬렌의 발렌타인데이 초콜릿 5개
GameEvent:AddReward( 10309,	{ fEventTime =  14,	iRewardID = 1000308, bGiveOnce =  True, iMinLevel = 10 } ) -- 기적의 발렌타인데이 반지(Lv.1)

--춘절 이벤트
GameEvent:AddReward( 10312,	{ fEventTime =  20,	iRewardID = 1000309, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 축복받은 무기 강화석
GameEvent:AddReward( 10313,	{ fEventTime =  20,	iRewardID = 1000310, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 마나 에릭실
GameEvent:AddReward( 10314,	{ fEventTime =  20,	iRewardID = 1000311, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 활력의 포션
GameEvent:AddReward( 10315,	{ fEventTime =  20,	iRewardID = 1000312, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 얼음 조각상 가열기
GameEvent:AddReward( 10316,	{ fEventTime =  20,	iRewardID = 1000313, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 축복받은 시공간의 주문서 교환권
GameEvent:AddReward( 10317,	{ fEventTime =  20,	iRewardID = 1000314, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 플루오르 스톤 교환권
GameEvent:AddReward( 10318,	{ fEventTime =  20,	iRewardID = 1000315, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 완전 회복 포션
GameEvent:AddReward( 10319,	{ fEventTime =  20,	iRewardID = 1000316, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 각성 포션
GameEvent:AddReward( 10320,	{ fEventTime =  20,	iRewardID = 1000317, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 펫 시리얼
GameEvent:AddReward( 10321,	{ fEventTime =  20,	iRewardID = 1000318, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --고급 마법석
GameEvent:AddReward( 10322,	{ fEventTime =  20,	iRewardID = 1000319, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --엘의 조각(불명)
GameEvent:AddReward( 10323,	{ fEventTime =  20,	iRewardID = 1000320, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --아크 엔젤 아바타 풀세트 큐브(1일)
GameEvent:AddReward( 10324,	{ fEventTime =  20,	iRewardID = 1000321, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 그노시스의 축복 (30SP, 14일)
GameEvent:AddReward( 10325,	{ fEventTime =  20,	iRewardID = 1000322, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --마법석
GameEvent:AddReward( 10326,	{ fEventTime =  20,	iRewardID = 1000323, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --부활석 14개 큐브
GameEvent:AddReward( 10327,	{ fEventTime =  20,	iRewardID = 1000324, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 망각의 알약
GameEvent:AddReward( 10328,	{ fEventTime =  20,	iRewardID = 1000325, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --루리엘의 축복받은 방어구 강화석

--2014원소절
GameEvent:AddReward( 2066, { fEventTime = 15, iRewardID = 2066,  bGiveOnce =  True, bRepeatRewardEvent = True, iMinLevel = 10, iBeforeEventID = 0 } )			--소환석 : 돌진 전차 모비RT (3일권)
GameEvent:AddReward( 2067, { fEventTime = 15, iRewardID = 2067,  bGiveOnce =  True, bRepeatRewardEvent = True, iMinLevel = 10, iBeforeEventID = 2066 } )		--어깨 망아지 인형(파랑) (3일권)
GameEvent:AddReward( 2068, { fEventTime = 15, iRewardID = 2068,  bGiveOnce =  True, bRepeatRewardEvent = True, iMinLevel = 10, iBeforeEventID = 2067 } )		--어깨 망아지 인형(분홍) (3일권)
GameEvent:AddReward( 2069, { fEventTime = 15, iRewardID = 2069,  bGiveOnce =  True, bRepeatRewardEvent = True, iMinLevel = 10, iBeforeEventID = 2068 } )		--레어 아바타 풀세트 (3일권) 큐브

-- 샌더 5,6던전 이벤트  샌더 마을의 서신
GameEvent:AddReward( 2053, { fEventTime = 60, iRewardID = 2054, bGiveOnce = True, iMinLevel = 10 } ) -- 풍화된 샌더 마을의 서신 조각 1개, 엘의 조각(윈드) 1개

--진입구조 개편 ---------------------
GameEvent:AddReward( 10410, { fEventTime =  10,	iRewardID = 10410, bGiveOnce =  True, iMinLevel = 10 } ) -- 대리 판매 보증서(VIP, 3일권)


---------------------------------------------------------------------------------------------------
--공지 이벤트 날릴 센터서버UID
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_SOLES"],  101 ) -- 솔레스
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_GAIA"], 301 )  -- 가이아


--GameEvent:AddConnectEventUserGroup(	0	,	10931	)	-- 테스트 용
