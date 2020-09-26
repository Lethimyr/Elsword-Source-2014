--lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

--## 각 이벤트 마다 아이디가 겹치지 않아야 하지만 다른 이벤트 끼리는 상광없음. ##

--[[REWARD]]
--1 arg : reward id
--2 arg : EXE rate
--3 arg : VP rate
--4 arg : Ed rate

--5 arg : Dungeon ID(난이도는 식별하지 않는다.)
--6 arg : Unit Level
--7 arg : Unit Class
--8 arg : dungeon game party unit num
--# 5, 6, 7, 8 arg 는 사용하지 않는 항목은 0으로 셋팅해야함.
--# 5,6,7 항목과 8 항목은 전혀 별개로 사용해야함.

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

--경험치 1.5배
GameEvent:AddReward( 33, { fEXPRate = 0.5 } )
--vp 1.5배
GameEvent:AddReward( 34, { fVPRate = 0.5 } )
--ed 1.5배
GameEvent:AddReward( 35, { fEDRate = 0.5 } )


--올림픽 이벤트
GameEvent:AddReward( 36, { fEventTime =   0, iRewardID = 42, bGiveOnce = False, bDelEndEvent = True  } )	--금메달 큐브
GameEvent:AddReward( 37, { fEventTime = 120, iRewardID = 43, bGiveOnce = False } )							--대표팀 응원 아이템


--신학기 기념 이벤트
GameEvent:AddReward( 38, { fEventTime =   0, iRewardID = 44 } )  --신학기 기념 출입증 큐브


--경험치 3배
GameEvent:AddReward( 39, { fEXPRate = 2.0 } )
--vp 3배
GameEvent:AddReward( 40, { fVPRate = 2.0 } )
--ed 3배
GameEvent:AddReward( 41, { fEDRate = 2.0 } )

--드롭률 2배 이벤트
GameEvent:AddReward( 267, { iDropCount =  2 } ) --드롭률 2배

---------------------------------------------------------------------------------------------------
--1차 CBT / 2010.11.17 ~ 2010.11.30

--매일매일 접속 이벤트
--5분
GameEvent:AddReward( 1000, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/17] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1001, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/18] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1002, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/19] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1003, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/20] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1004, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/21] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1005, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/22] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1006, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/23] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1007, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/24] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1008, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/25] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1009, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/26] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1010, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/27] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1011, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/28] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1012, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/29] 매일매일 접속 큐브(5분) / 공용
GameEvent:AddReward( 1013, 0.0, 0.0, 0.0, { fEventTime =   5, iRewardID = 1000, bGiveOnce =  True } ) --[11/30] 매일매일 접속 큐브(5분) / 공용

--30분
GameEvent:AddReward( 1014, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/17] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1015, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/18] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1016, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/19] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1017, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1002, bGiveOnce =  True } ) --[11/20] 매일매일 접속 큐브(30분) / 주말 --
GameEvent:AddReward( 1018, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1002, bGiveOnce =  True } ) --[11/21] 매일매일 접속 큐브(30분) / 주말 --
GameEvent:AddReward( 1019, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/22] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1020, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/23] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1021, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/24] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1022, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/25] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1023, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/26] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1024, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1002, bGiveOnce =  True } ) --[11/27] 매일매일 접속 큐브(30분) / 주말 --
GameEvent:AddReward( 1025, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1002, bGiveOnce =  True } ) --[11/28] 매일매일 접속 큐브(30분) / 주말 --
GameEvent:AddReward( 1026, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/29] 매일매일 접속 큐브(30분) / 평일
GameEvent:AddReward( 1027, 0.0, 0.0, 0.0, { fEventTime =  30, iRewardID = 1001, bGiveOnce =  True } ) --[11/30] 매일매일 접속 큐브(30분) / 평일

--60분
GameEvent:AddReward( 1028, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/17] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1029, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/18] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1030, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/19] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1031, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1004, bGiveOnce =  True } ) --[11/20] 매일매일 접속 큐브(60분) / 주말 --
GameEvent:AddReward( 1032, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1004, bGiveOnce =  True } ) --[11/21] 매일매일 접속 큐브(60분) / 주말 --
GameEvent:AddReward( 1033, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/22] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1034, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/23] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1035, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/24] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1036, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/25] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1037, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/26] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1038, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1004, bGiveOnce =  True } ) --[11/27] 매일매일 접속 큐브(60분) / 주말 --
GameEvent:AddReward( 1039, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1004, bGiveOnce =  True } ) --[11/28] 매일매일 접속 큐브(60분) / 주말 --
GameEvent:AddReward( 1040, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/29] 매일매일 접속 큐브(60분) / 평일
GameEvent:AddReward( 1041, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1003, bGiveOnce =  True } ) --[11/30] 매일매일 접속 큐브(60분) / 평일

--복떨이 3탄
--1분
GameEvent:AddReward( 1042, 0.0, 0.0, 0.0, { fEventTime =  1, iRewardID = 1007, bGiveOnce =  True } ) --[11/29] 복떨이 3탄 큐브

---------------------------------------------------------------------------------------------------
--2차 CBT / 2011.05.11 ~ 2011.05.15

--두배 이벤트
GameEvent:AddReward( 1050, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) --[05/11] 엘소드 경험치 2배
GameEvent:AddReward( 1051, { cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) --[05/11] 엘소드 근성도 무제한
GameEvent:AddReward( 1052, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_ARME"] } ) --[05/11] 아이샤 경험치 2배
GameEvent:AddReward( 1053, { cUnitType = UNIT_TYPE["UT_ARME"] } ) --[05/11] 아이샤 근성도 무제한
GameEvent:AddReward( 1054, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_LIRE"] } ) --[05/11] 레나 경험치 2배
GameEvent:AddReward( 1055, { cUnitType = UNIT_TYPE["UT_LIRE"] } ) --[05/11] 레나 근성도 무제한
GameEvent:AddReward( 1056, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_RAVEN"] } ) --[05/12] 레이븐 경험치 2배
GameEvent:AddReward( 1057, { cUnitType = UNIT_TYPE["UT_RAVEN"] } ) --[05/12] 레이븐 근성도 무제한
GameEvent:AddReward( 1058, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_EVE"] } ) --[05/13] 이브 경험치 2배
GameEvent:AddReward( 1059, { cUnitType = UNIT_TYPE["UT_EVE"] } ) --[05/13] 이브 근성도 무제한

--레벨업 이벤트
GameEvent:AddReward( 1060, { fEventTime =  0, iRewardID = 1010, bGiveOnce =  True } ) --[05/11~05/15] 엘의 조각(불명) 50개

--"엘의 수정" 오픈 아이템 지급 이벤트
GameEvent:AddReward( 1061, { fEventTime =  0, iRewardID = 1016, bGiveOnce =  True } ) --[05/11~05/15] 엘의 수정 10개 큐브

--근성 포션 3개 지급 이벤트
GameEvent:AddReward( 1062, { fEventTime =  0, iRewardID = 1, bGiveOnce =  True } ) --[05/14~05/15] 근성 포션 3개

--CBT 참여 감사 이벤트
GameEvent:AddReward( 1063, { fEventTime =  100, iRewardID = 1017, bGiveOnce =  True, bAccountEvent = True } ) --[05/14~05/15] 근성 포션 3개


-------------------------------------------------------------------------------------------------------------------
--3차 CBT // 2011.07.26 ~ 2011.08.22 lygan_조성욱 // 이벤트 세팅

--출석 이벤트
GameEvent:AddReward( 1070, { fEventTime =  0, iRewardID = 1020,  bGiveOnce =  True } ) --출석권


--서바이벌 미션  이벤트
GameEvent:AddReward( 1071, { fEventTime =  0, iRewardID = 1021,  bGiveOnce =  True } ) --생존의 자격 아이템 


--길드 이벤트
GameEvent:AddReward( 1072, { fEventTime =  0, iRewardID = 1022,  bGiveOnce =  True } ) --길드 인원 확장권 아이템 
GameEvent:AddReward( 1073, { fEventTime =  0, iRewardID = 1023,  bGiveOnce =  True } ) --그노시스의 대 축복 아이템 

GameEvent:AddReward( 1074, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1024, bGiveOnce =  True } ) --매일매일 접속 큐브(60분)

GameEvent:AddReward( 1075, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1025, bGiveOnce =  True } ) --8월 6일 토요일 매일매일 접속 큐브(60분)
GameEvent:AddReward( 1076, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1026, bGiveOnce =  True } ) --8월 7일 일요일 매일매일 접속 큐브(60분)

GameEvent:AddReward( 1077, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1027, bGiveOnce =  True } ) --8월 16일 들어가는 급 추가된 이벤트 화,수,목 아바타 7일권
GameEvent:AddReward( 1078, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1028, bGiveOnce =  True } ) --8월 16일 들어가는 급 추가된 이벤트 금,토,일,월 악세 7일권

-------------------------------------------------------------------------------------------------------------------
--OBT //2011.11.11 ~ 2011.12.14 lygan_조성욱 // 이벤트 세팅
GameEvent:AddReward( 1080, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1030, bGiveOnce =  True } ) --매일 매일 접속시 출석권 지급 (60분) lygan_조성욱

GameEvent:AddReward( 1081, { fEventTime =  0, iRewardID = 1031,  bGiveOnce =  True } ) --이벤트 기간 처음 생성된 캐릭터에게 5Lv 신병 패키지 큐브 지급 lygan_조성욱

GameEvent:AddReward( 1082, { fEventTime =  0, iRewardID = 1032,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 10월 29일 lygan_조성욱
GameEvent:AddReward( 1083, { fEventTime =  0, iRewardID = 1033,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 10월 30일 lygan_조성욱
GameEvent:AddReward( 1084, { fEventTime =  0, iRewardID = 1034,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 11월 05일 lygan_조성욱
GameEvent:AddReward( 1085, { fEventTime =  0, iRewardID = 1035,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 11월 06일 lygan_조성욱
GameEvent:AddReward( 1086, { fEventTime =  0, iRewardID = 1036,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 11월 12일 lygan_조성욱
GameEvent:AddReward( 1087, { fEventTime =  0, iRewardID = 1037,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 11월 13일 lygan_조성욱
GameEvent:AddReward( 1088, { fEventTime =  0, iRewardID = 1038,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 11월 19일 lygan_조성욱
GameEvent:AddReward( 1089, { fEventTime =  0, iRewardID = 1039,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 11월 20일 lygan_조성욱
GameEvent:AddReward( 1090, { fEventTime =  0, iRewardID = 1040,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 11월 26일 lygan_조성욱
GameEvent:AddReward( 1091, { fEventTime =  0, iRewardID = 1041,  bGiveOnce =  True } ) --이벤트 기간 경험치 2배와 드랍 2배시에 지급되야할 아이템 큐브 11월 27일 lygan_조성욱

GameEvent:AddReward( 1092, { fEventTime = 30, iRewardID = 1, bGiveOnce =  True } ) --[11/11] 근성 포션 3개 (30분) lygan_조성욱
GameEvent:AddReward( 1093, { fEventTime = 60, iRewardID = 1, bGiveOnce =  True } ) --[11/11] 근성 포션 3개 (60분) lygan_조성욱

-------------------------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1094, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1042, bGiveOnce =  True } ) --2011.11.16일 긴급 이벤트 60분 접속 부활석 지급 lygan_조성욱

GameEvent:AddReward( 1095, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1043, bGiveOnce =  True } ) --2011.11.16일 긴급 이벤트 60분 접속 엘의 조각(불명) lygan_조성욱

GameEvent:AddReward( 1096, 0.0, 0.0, 0.0, { fEventTime =  60, iRewardID = 1044, bGiveOnce =  True } ) --2011.11.16일 긴급 이벤트 60분 접속 액세서리 랜덤 큐브 lygan_조성욱


-------------------------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1100, 0.0, 0.0, 0.0, { fEventTime =  1, iRewardID = 1050, bGiveOnce =  True } ) --2011.11.30일 접속 Coming Soon 레이븐 큐브
-------------------------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1105, { fEventTime =  0, iRewardID = 1060, bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_RAVEN"] } ) --2011.12.07일 레이븐 개방 이벤트 lygan_조성욱
GameEvent:AddReward( 1106, { fEventTime =  60, iRewardID = 1061, bGiveOnce =  True } ) --2011.12.07일 OBT 출석카드 이벤트 lygan_조성욱
-------------------------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1110, { fEventTime =  60, iRewardID = 2, bGiveOnce =  True } ) --2011.12.14일 부활석을 지원해 드립니다 이벤트 lygan_조성욱
-------------------------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1111, { fEventTime =  0, iRewardID = 1062,  bGiveOnce =  True } ) --2011.12.21일 크리스마스는 엘소드와 함께 이벤트 lygan_조성욱
GameEvent:AddReward( 1112, { fEventTime =  150, iRewardID = 1063,  bAccountEvent = True } ) --2011.12.21일 크리스마스 특별선물 이벤트 lygan_조성욱

GameEvent:AddReward( 1113, { fEventTime =  60, iRewardID = 1064,  bGiveOnce =  True } ) --2011.12.21일 아리엘의 선물 이벤트 lygan_조성욱

-------------------------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1120, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_ELSWORD"], iUnitLevel = 35 } ) --2011.12.28 오늘은 엘소드의 날 이벤트!    lygan_조성욱

GameEvent:AddReward( 1121, { fEventTime =  60, iRewardID = 1070,  bGiveOnce =  True } ) --2011.12.28 일 윌리엄의 시공 이벤트 (윌리엄 뽀루의 제안) lygan_조성욱

GameEvent:AddReward( 1122, { fEventTime =  30, iRewardID = 1071,  bGiveOnce =  True } ) --2011.12.28 일 윌리엄의 시공 이벤트 (윌리엄 뽀루의 주말 특별 제안)  lygan_조성욱

GameEvent:AddReward( 1123, { fEventTime =  0, iRewardID = 1072,  bGiveOnce =  True } ) --2011.12.28 일 새해 맞이 이벤트 lygan_조성욱

GameEvent:AddReward( 1124, { fEventTime =  60, iRewardID = 1073,  bGiveOnce =  True } ) --2011.12.28 일 새해 맞이 이벤트 (새해 선물 선물) lygan_조성욱

GameEvent:AddReward( 1125, { fEventTime =  30, iRewardID = 1074,  bGiveOnce =  True, bAccountEvent = True } ) --2011.12.28 일 새해 맞이 이벤트 (새해 맞이 복리 이벤트 ) lygan_조성욱

GameEvent:AddReward( 1126, { fEventTime =  0, iRewardID = 1075,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) --2011.12.28 일 오늘은 엘소드의 날 이벤트! (  (2차 전직 프로모션 아바타 대방출 )) lygan_조성욱 (로드 나이트 1일권 큐브)

GameEvent:AddReward( 1127, { fEventTime =  0, iRewardID = 1076,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) --2011.12.28 일 오늘은  엘소드의 날 이벤트! (  (2차 전직 프로모션 아바타 대방출 ) lygan_조성욱 (룬 슬레이어 1일권 큐브)

GameEvent:AddReward( 1128, { fEventTime =  0, iRewardID = 1077,  bGiveOnce =  True } ) --2011.12.28 일 긴급 이벤트 부활석 큐브 1개 짜리 3개
---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1130, { fEventTime =  0, iRewardID = 1080,  bGiveOnce =  True, bAccountEvent = True } ) --2012.01.02 일 아이샤, 레나 2차 전직 전야 이벤트 2차 전직 프로모션 아바타 대방출  lygan_조성욱 (여캐를 위한 큐브)
GameEvent:AddReward( 1131, { fEventTime =  0, iRewardID = 1081,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ARME"] } ) --2012.01.02 일 오늘은  아이샤, 레나 2차 전직 전야 이벤트! (  (2차 전직 프로모션 아바타 대방출 ) lygan_조성욱 (아이샤 1일권 큐브)
GameEvent:AddReward( 1132, { fEventTime =  0, iRewardID = 1082,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_LIRE"] } ) --2012.01.02 일 오늘은  아이샤, 레나 2차 전직 전야 이벤트! (  (2차 전직 프로모션 아바타 대방출 ) lygan_조성욱 (레나 1일권 큐브)

GameEvent:AddReward( 1133, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_ARME"], iUnitLevel = 35 } ) --2012.01.02 아이샤, 레나 2차 전직 준비 기간! (아이샤 경험치 2배)   lygan_조성욱
GameEvent:AddReward( 1134, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_LIRE"], iUnitLevel = 35 } ) --2012.01.02 아이샤, 레나 2차 전직 준비 기간! (레나 경험치 2배)   lygan_조성욱

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1140, { fEventTime =  60, iRewardID = 1090,  bGiveOnce =  True } ) --2012.01.11 일 춘절 기념 이벤트 ( 복이 온다는 반지 ) lygan_조성욱
GameEvent:AddReward( 1141, { fEventTime =  120, iRewardID = 1091,  bGiveOnce =  True, bAccountEvent = True } ) --2012.01.11 일 춘절 기념 이벤트 ( 복은 우편함에서 온다 ) lygan_조성욱
GameEvent:AddReward( 1142, { fEventTime =  0, iRewardID = 1092,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_RAVEN"] } ) --2012.01.11 일 레이븐 전야 이벤트 ( 2차 전직 프로모션 아바타 방출 ) lygan_조성욱
GameEvent:AddReward( 1143, { fEXPRate = 1.0, cUnitType = UNIT_TYPE["UT_RAVEN"], iUnitLevel = 35 } ) --2012.01.11 레이븐 2차 전직 준비 기간! (레이븐 경험치 2배)   lygan_조성욱
GameEvent:AddReward( 1144, { fEXPRate = 2.0, iDungeonID = 30000 } ) 								--2012.01.11 루벤지역 경험치 3배 이벤트 ( DI_RUBEN_EL_TREE_NORMAL )
GameEvent:AddReward( 1145, { fEXPRate = 2.0, iDungeonID = 30080 } ) 								--2012.01.11 루벤지역 경험치 3배 이벤트 ( DI_RUBEN_RUIN_OF_ELF_NORMAL )
GameEvent:AddReward( 1146, { fEXPRate = 2.0, iDungeonID = 30081 } ) 								--2012.01.11 루벤지역 경험치 3배 이벤트 ( DI_RUBEN_RUIN_OF_ELF_HARD )
GameEvent:AddReward( 1147, { fEXPRate = 2.0, iDungeonID = 30082 } ) 								--2012.01.11 루벤지역 경험치 3배 이벤트 ( DI_RUBEN_RUIN_OF_ELF_EXPERT )
GameEvent:AddReward( 1148, { fEXPRate = 2.0, iDungeonID = 30090 } ) 								--2012.01.11 루벤지역 경험치 3배 이벤트 ( DI_RUBEN_SWAMP_NORMAL )
GameEvent:AddReward( 1149, { fEXPRate = 2.0, iDungeonID = 30091 } ) 								--2012.01.11 루벤지역 경험치 3배 이벤트 ( DI_RUBEN_SWAMP_HARD )
GameEvent:AddReward( 1150, { fEXPRate = 2.0, iDungeonID = 30092 } ) 								--2012.01.11 루벤지역 경험치 3배 이벤트 ( DI_RUBEN_SWAMP_EXPERT )
GameEvent:AddReward( 1151, { fEventTime = 10, iRewardID = 1093,  bGiveOnce =  True } ) 				--2012.01.11 일 용쟁호투 타이틀( 30일 ) 지급하는 이벤트 ( 용쟁호투 지급 ) lygan_조성욱
---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1160, { fEventTime = 60, iRewardID = 1100,  bGiveOnce =  True } )						 --2012.02.01 원소절 이벤트 등불을 밝혀 주세요 ( 등불을 밝혀주세요 이벤트 ) lygan_조성욱
GameEvent:AddReward( 1161, { fEventTime = 60, iRewardID = 1101,  bGiveOnce =  True } )						 --2012.02.01 원소절 이벤트 등불을 밝혀 주세요 ( 등불을 밝혀주세요 이벤트 )lygan_조성욱
GameEvent:AddReward( 1162, { fEventTime = 60, iRewardID = 1102,  bGiveOnce =  True } ) 						 --2012.02.01 이브 오픈 전야 이벤트 ( 새롭게 시작하는 이브 ) lygan_조성욱
GameEvent:AddReward( 1163, { fEventTime = 0, iRewardID = 1103,  bGiveOnce =  True } )					     --2012.02.01 원소절 이벤트 등불을 밝혀 주세요 ( 등불을 밝혀주세요 이벤트 )lygan_조성욱
GameEvent:AddReward( 1164, { fEventTime = 60, iRewardID = 1104,  bGiveOnce =  True, bAccountEvent = True } ) --2012.02.01 아리엘 가열기 지급 이벤트( 퍼블리셔 제안 )lygan_조성욱

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1170, { fEventTime =  30, iRewardID = 1110,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_EVE"] } ) --2012.02.15 일 이브 오픈 기념 이벤트 ( 레비와 모비만 남은것인가요 이벤트 ) lygan_조성욱
GameEvent:AddReward( 1171, { fEventTime = 60, iRewardID = 1111,  bGiveOnce =  True, bAccountEvent = True } ) --2012.02.15 이브 오픈 기념 이벤트  ( 이브? 그거 먹는 거임? )lygan_조성욱

---------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1180, { fEventTime = 30, iRewardID = 1120,  bGiveOnce =  True } ) --2012.02.29 접속만 해도 새학기 필수품이 와르르 이벤트 ( 엘리오스 새학기 필수품 큐브 1개 ) lygan_조성욱
GameEvent:AddReward( 1181, { fEventTime = 60, iRewardID = 1121,  bGiveOnce =  True } ) --2012.02.29 접속만 해도 새학기 필수품이 와르르 이벤트 ( 엘리오스 새학기 큐브 열쇠 ) lygan_조성욱
---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1190, { fEventTime = 60, iRewardID = 1130,  bGiveOnce =  True } ) --2012.03.14일	길드 대전 - 홍보 이벤트 ( 길드 대전 응원 칭호 1일권 ) lygan_조성욱
GameEvent:AddReward( 1191, { fEventTime = 60, iRewardID = 1131,  bGiveOnce =  True } ) --2012.03.14일	길드 대전 - 홍보 이벤트 ( 마법의 목걸이(+30%) 1일권 ) lygan_조성욱

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1200, { fEventTime = 30, iRewardID = 1140,  bGiveOnce =  True } )  --2012.03.28일	루벤 숲의 눈물 ( 자연의 선물 평일 5개 ) lygan_조성욱
GameEvent:AddReward( 1201, { fEventTime = 30, iRewardID = 1141,  bGiveOnce =  True } )  --2012.03.28일	루벤 숲의 눈물 ( 자연의 선물 주말 10개 ) lygan_조성욱
GameEvent:AddReward( 1202, { iDropCount = 2, iDungeonID = 39500 } ) 					--2012.03.28일	루벤 숲의 눈물 던전 드랍율 2배 ( 엘더 ) lygan_조성욱
GameEvent:AddReward( 1203, { iDropCount = 2, iDungeonID = 39510 } ) 					--2012.03.28일	루벤 숲의 눈물 던전 드랍율 2배 ( 베스마 ) lygan_조성욱
GameEvent:AddReward( 1204, { iDropCount = 2, iDungeonID = 39520 } ) 					--2012.03.28일	루벤 숲의 눈물 던전 드랍율 2배 ( 알테라 ) lygan_조성욱
GameEvent:AddReward( 1205, { iDropCount = 2, iDungeonID = 39540 } ) 					--2012.03.28일	루벤 숲의 눈물 던전 드랍율 2배 ( 벨더 ) lygan_조성욱

GameEvent:AddReward( 1206, { fEventTime = 30, iRewardID = 1142,  bGiveOnce =  True } ) --2012.03.28일	청명절 이벤트 조상님의 기운으로 ( 버드나무 가지 1개 지급 ) lygan_조성욱
GameEvent:AddReward( 1207, { fEventTime = 60, iRewardID = 1143,  bGiveOnce =  True } ) --2012.03.28일	청명절 이벤트 연휴기간은 엘소드와 함께 ( 청명절의 가호 1일권 칭호 아이템 ) lygan_조성욱
GameEvent:AddReward( 1208, { fEventTime = 60, iRewardID = 1144,  bGiveOnce =  True } ) --2012.03.28일	청명절 이벤트 연휴기간은 엘소드와 함께 ( 세인트 미니미 1일권 지급 ) lygan_조성욱
GameEvent:AddReward( 1209, { fEventTime = 60, iRewardID = 1145,  bGiveOnce =  True } ) --2012.03.28일	청명절 이벤트 연휴기간은 엘소드와 함께 ( 버드나무 아바타 1일권 큐브 ) lygan_조성욱

GameEvent:AddReward( 1210, { fEventTime = 0, iRewardID = 1146,  bGiveOnce =  True } ) --2012.03.28일	만우절 이벤트 만우절 선물 첫 접속시 지급 ( 양치기의 큐브 ) lygan_조성욱
GameEvent:AddReward( 1211, { fEventTime = 120, iRewardID = 1146,  bGiveOnce =  True } ) --2012.03.28일	만우절 이벤트 만우절 선물 120분 접속시 지급 ( 양치기의 큐브 ) lygan_조성욱
GameEvent:AddReward( 1212, { fEventTime = 0, iRewardID = 1147,  bGiveOnce =  True } ) --2012.03.28일	만우절 이벤트 만우절 선물 첫 접속시 지급 ( 피노키오의 매력 1일 ) lygan_조성욱

---------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1220, { fEventTime = 60, iRewardID = 1150,  bGiveOnce =  True } ) --2012.04.10일	빛나는 은 큐브를 열어보자 ( 빛나는 은 큐브 ) lygan_조성욱
GameEvent:AddReward( 1221, { fEventTime = 60, iRewardID = 1151,  bGiveOnce =  True } ) --2012.04.10일	빛나는 은 큐브를 열어보자 ( 고대의 은 큐브 1개 ) lygan_조성욱
GameEvent:AddReward( 1222, { fEventTime = 1, iRewardID = 1152,  bGiveOnce =  True } ) --2012.04.10일	뽀루의 장난 편지 ( 보코사의 지원 큐브 ) lygan_조성욱

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1230, { fEventTime = 60, iRewardID = 1160,  bGiveOnce =  True, bAccountEvent = True } ) --2012.04.25일	노동절 연휴를 즐기자  ( 노동절 황금 큐브 1개 ) lygan_조성욱
GameEvent:AddReward( 1231, { fEventTime = 10, iRewardID = 1161,  bGiveOnce =  True } ) --2012.04.25일	비던 입장권 지원 ( 호프만의 출입증서 2개 ) lygan_조성욱
GameEvent:AddReward( 1232, { fEventTime = 10, iRewardID = 1162,  bGiveOnce =  True } ) --2012.04.25일	비던 입장권 지원 ( 월리의 출입증서 2개 ) lygan_조성욱
GameEvent:AddReward( 1233, { fEventTime = 10, iRewardID = 1163,  bGiveOnce =  True } ) --2012.04.25일	비던 입장권 지원 ( 나락의 숨겨진 지도 2개 ) lygan_조성욱
GameEvent:AddReward( 1234, { fEventTime = 10, iRewardID = 1164,  bGiveOnce =  True } ) --2012.04.25일	비던 입장권 지원 ( 나락의 지도 2개 ) lygan_조성욱
---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1240, { fEventTime = 30, iRewardID = 1170,  bGiveOnce =  True } ) --2012.05.02 P2P 사내 테스트용 접속 이벤트 ( 아리엘의 주문서 지급 )
GameEvent:AddReward( 1241, { fEventTime = 90, iRewardID = 1170,  bGiveOnce =  True } ) --2012.05.02 P2P 사내 테스트용 접속 이벤트 ( 아리엘의 주문서 지급 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1250, { fEventTime = 60, iRewardID = 1180,  bGiveOnce =  True,  bAccountEvent = True } ) --2012.05.30	시공간의 큐브 이벤트  			( 시공간의 큐브 60개 )
GameEvent:AddReward( 1251, { fEventTime = 30, iRewardID = 1181,  bGiveOnce =  True,  bAccountEvent = True } ) --2012.05.30	시공간의 열쇠얻는법  이벤트 	( 시공간의 열쇠 1개 )
GameEvent:AddReward( 1252, { fEventTime = 60, iRewardID = 1182,  bGiveOnce =  True,  bAccountEvent = True } ) --2012.05.30	어린이날에 선물을 드려요~!		( 스타 아카데미 하복 풀세트 큐브 (3일) )
GameEvent:AddReward( 1253, { fEventTime = 60, iRewardID = 1183,  bGiveOnce =  True,  bAccountEvent = True } ) --2012.05.30	어린이날에 선물을 드려요~!		( 동물 복장 아바타 풀세트 큐브(3일) )
GameEvent:AddReward( 1254, { fEventTime = 60, iRewardID = 1184,  bGiveOnce =  True,  bAccountEvent = True } ) --2012.05.30	어린이날에 선물을 드려요~!		( 특수 기술의 반지 4종 15일권 큐브(1개) )
GameEvent:AddReward( 1255, { fEventTime = 60, iRewardID = 1185,  bGiveOnce =  True,  bAccountEvent = True } ) --2012.05.30	어린이날에 선물을 드려요~!		( 그노시스의 축복( 10 SP, 7일권 ) )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1260, { fEventTime = 60, iRewardID = 1190,  bGiveOnce =  True, bAccountEvent = True,  bCheckGuildUnit = True } ) --2012.06.13 길드 접속 이벤트 ( 길드 상품권 1개 )
GameEvent:AddReward( 1261, { fEventTime = 60, iRewardID = 1261,	 bDirectReward  = True } ) 											  --2012.06.13 하멜의 쓰러진 기둥 이벤트 ( 하멜의 흔적 1개)
GameEvent:AddReward( 1262, { fEventTime = 60, iRewardID = 1191,  bGiveOnce =  True } ) 												  --2012.06.13 하멜의 얼음 조각 ( 하멜의 얼음 조각 1개 )
GameEvent:AddReward( 1263, { fEventTime = 30, iRewardID = 1192,  bGiveOnce =  True } ) 												  --2012.06.13 하멜 지원 접속 이벤트 			( 하멜 악세서리 지원 큐브 1일권 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1270, { fEventTime = 0, iRewardID = 1200,  bGiveOnce =  True } ) 												  --2012.06.20 하멜을 지원 하라 ( 하멜을 지원 하라 칭호 30일 )
GameEvent:AddReward( 1271, { fEventTime = 30, iRewardID = 1201,  bGiveOnce =  True } ) 												  --2012.06.20 드래곤 보트 제작 패키지 ( 대나무 잎 1개 )
GameEvent:AddReward( 1272, { fEventTime = 60, iRewardID = 1202,  bGiveOnce =  True, bAccountEvent = True } )						--2012.06.20 단오절 연휴 특별 이벤트 ( 드래곤 보트 1일 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1280, { fEventTime = 0, iRewardID = 1210,  bGiveOnce =  True } ) 												  --2012.06.27일	함께 해요~ 사랑해요~ 이벤트 	( '함께 해요' 칭호 랜덤 큐브(1일) )

---------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1290, { fEventTime = 0, iRewardID = 1220,  bGiveOnce =  True } ) 						 													--2012.07.04일	새로운 영웅의 그림자 이벤트 	( 새로운 영웅의 이야기 보따리 1개 )
GameEvent:AddReward( 1291, { fEventTime = 30, iRewardID = 1221,  bGiveOnce =  True, bAccountEvent = True } ) 													--2012.07.04일	China Joy 뽀루 					( China Joy 깃발 1일권 1개 지급 )
---------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1300, { fEventTime = 0, iRewardID = 1230,  bGiveOnce =  True } ) 						 						 							--2012.07.11일	하멜의 하얀 늑대 이벤트 		( 하멜의 하얀 늑대 칭호 1개 1일권 )
GameEvent:AddReward( 1301, { fEventTime = 100, iRewardID = 1231,  bGiveOnce =  True, bAccountEvent = True } ) 						 							--2012.07.11일 새로운 동료와 함께 이벤트 		( 청 캐릭터 카드 1개 )
GameEvent:AddReward( 1302, { fEventTime = 60, iRewardID = 1232,  bGiveOnce =  True } ) 						 						 							--2012.07.11일 가짜 동료 찾기 					( 청의 첫번째 선물 1개 )
GameEvent:AddReward( 1303, { fEventTime = 60, iRewardID = 1233,  bGiveOnce =  True, bAccountEvent = True, bComebackUserCheck = True } ) 						--2012.07.11일 하멜의 부름 이벤트 				( 돌아온 모험가를 위한 지원 큐브 1개 )
GameEvent:AddReward( 1304, { fEventTime = 0, iRewardID = 1234,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_CHUNG"] } ) 						 				--2012.07.11일 코보사의 초보 모험가 지원 		( 숲의 수호자 무기 큐브 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1310, { fEventTime = 1, iRewardID = 1246,  bGiveOnce =  True } ) 						 						 							--2012.07.25일 올림픽 개막식 이벤트 			( 엘리오스 게임 (15일) 지급 )
GameEvent:AddReward( 1311, { fEventTime = 10, iRewardID = 1247,  bGiveOnce =  True, bAccountEvent = True } ) 						 						 	--2012.07.25일 올림픽 개막식 이벤트 			( 이벤트용 플루오르 스톤 Lv1 1개 ) ( 이벤트용 축복받은 복원의 주문서 Lv1 1 개 )
GameEvent:AddReward( 1312, { fEventTime = 120, iRewardID = 1248,  bGiveOnce =  True, bAccountEvent = True } ) 						 						 	--2012.07.25일 올림픽 개막식 이벤트 			( 아리엘의 얼음 조각상 가열기  1개 ) ( 도전자의 상징 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1320, { fEventTime = 60, iRewardID = 1250,  bGiveOnce =  True } ) 						 						 							--2012.08.02일 황금 코인 수집 방법 			( 황금 결정 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1330, { fEventTime = 1, iRewardID = 1260,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) 						 					--2012.08.08일 고독 칠석 						( 고독칠석(남) 1일권 칭호 1개 )
GameEvent:AddReward( 1331, { fEventTime = 1, iRewardID = 1260,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_RAVEN"] } ) 						 					--2012.08.08일 고독 칠석 						( 고독칠석(남) 1일권 칭호 1개 )
GameEvent:AddReward( 1332, { fEventTime = 1, iRewardID = 1260,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_CHUNG"] } ) 						 					--2012.08.08일 고독 칠석 						( 고독칠석(남) 1일권 칭호 1개 )
GameEvent:AddReward( 1333, { fEventTime = 1, iRewardID = 1261,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ARME"] } ) 						 					--2012.08.08일 고독 칠석 						( 고독칠석(여) 1일권 칭호 1개 )
GameEvent:AddReward( 1334, { fEventTime = 1, iRewardID = 1261,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_LIRE"] } ) 						 					--2012.08.08일 고독 칠석 						( 고독칠석(여) 1일권 칭호 1개 )
GameEvent:AddReward( 1335, { fEventTime = 1, iRewardID = 1261,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_EVE"] } ) 						 						--2012.08.08일 고독 칠석 						( 고독칠석(여) 1일권 칭호 1개 )
GameEvent:AddReward( 1336, { fEventTime = 60, iRewardID = 1262,  bGiveOnce =  True } ) 														 						--2012.08.08일 고독 칠석 						( 낭만 칠석  7일권 칭호 1개 )

---------------------------------------------------------------------------------------------------
GameEvent:AddReward( 1340, { fEventTime = 60, iRewardID = 1270,  bGiveOnce =  True } ) 														 						--2012.08.29일 세나스 무공 훈장 				( 세나스 무공 훈장 큐브 1개 )

GameEvent:AddReward( 1341, { fEventTime = 30, iRewardID = 1271,  bGiveOnce =  True, bAccountEvent = True, bNewUserEvent = True } ) 									--2012.08.29일 새학기 준비 					( 새학기 준비물 큐브 1개 )
GameEvent:AddReward( 1342, { fEventTime = 30, iRewardID = 1272,  bGiveOnce =  True, bAccountEvent = True, bCurrentUserEvent = True } ) 								--2012.08.29일 새출발 준비 					( 새출발 준비물 큐브 1개 )
GameEvent:AddReward( 1343, { fEventTime = 60, iRewardID = 1273,  bGiveOnce =  True } ) 														 						--2012.08.29일 새학기 주말 특별 이벤트			( 새학기 연필 1개 )
---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1350, { fEventTime = 30, iRewardID = 1280,  bGiveOnce =  True } )							 						 						 	--2012.09.06일 매일 마족 침공 				( 마족의 흔적 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1360, { fEventTime = 1, iRewardID = 1290,  bGiveOnce =  True, bAccountEvent = True } )			 						--2012.09.26일 중추절  ( 이브 캐릭터 카드(이벤트) 1개 )
GameEvent:AddReward( 1361, { fEventTime = 1, iRewardID = 1291,  bGiveOnce =  True, bAccountEvent = True } )			 						--2012.09.26일 국경절  ( 청 캐릭터 카드(이벤트) 1개 )
GameEvent:AddReward( 1362, { fEventTime = 1, iRewardID = 1292,  bGiveOnce =  True } )							 						 	--2012.09.26일 국경절  ( 아크엔젤 풀세트 큐브 1개 )
GameEvent:AddReward( 1363, { fEventTime = 1, iRewardID = 1293,  bGiveOnce =  True } )							 						 	--2012.09.26일 공존의 축제 ( 공존의 축제 칭호 ( 자애로운 평화 전도사 ) 1개 )
GameEvent:AddReward( 1364, { fEventTime = 30, iRewardID = 1294,  bGiveOnce =  True } )							 						 	--2012.09.26일 공존의 축제 ( 공존의 축제 의상 큐브 1개 )

GameEvent:AddReward( 1365, { fEventTime = 0, iRewardID = 1295,  bGiveOnce =  True, bAccountEvent = True, bComebackUserCheck = True } )			 						--2012.09.26일 복귀탐색 대원 큐브	   ( 복귀탐색대원 큐브 1개 )
GameEvent:AddReward( 1366, { fEventTime = 0, iRewardID = 1296,  bGiveOnce =  True, bAccountEvent = True, bCurrentUserEvent = True } )			 						--2012.09.26일 충실한 팬들의 보물상자 ( 충실한 팬들의 보물상자 1개 )

---------------------------------------------------------------------------------------------------


GameEvent:AddReward( 1370, { fEventTime = 150, iRewardID = 1300,  bGiveOnce =  True, bAccountEvent = True } )			 				--2012.10.10일 펫 공통 이벤트  			( 덫에 걸린 해츨링 (유체) 1개 )
GameEvent:AddReward( 1371, { fEventTime = 30, iRewardID = 1301,  bGiveOnce =  True, bAccountEvent = True } )			 				--2012.10.10일 펫 공통 이벤트  			( 카밀라의 데일리 펫 훈련 매거진 1개 )

GameEvent:AddReward( 1372, { fEventTime = 0, iRewardID = 1302,  bGiveOnce =  True, bAccountEvent = True } )			 					--2012.10.10일 영웅이 되는 준비 이벤트 	( 조운의 무기 상자 1개 )
GameEvent:AddReward( 1373, { fEventTime = 0, iRewardID = 1303,  bGiveOnce =  True, bAccountEvent = True } )			 					--2012.10.10일 영웅이 되는 준비 이벤트 	( 소리비주의 무기상자 1개

GameEvent:AddReward( 1374, { fEventTime = 60, iRewardID = 1304,  bGiveOnce =  True} )			 					--2012.10.10일 펫 던전 입장권 	( 펫 던전 입장권 3개 )
GameEvent:AddReward( 1375, { fEventTime = 60, iRewardID = 1305,  bGiveOnce =  True } )			 					--2012.10.10일 펫 던전 입장권 	( 펫 던전 입장권 5개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1380, { fEventTime = 10, iRewardID = 1310,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 데스사이드 조화 1개 (1일) )
GameEvent:AddReward( 1381, { fEventTime = 10, iRewardID = 1311,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 데스사이드 신비 1개 (1일) )
GameEvent:AddReward( 1382, { fEventTime = 10, iRewardID = 1312,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 데스사이드 광휘 1개 (1일) )
GameEvent:AddReward( 1383, { fEventTime = 10, iRewardID = 1313,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 데스사이드 큐브 1개 (1일) )
GameEvent:AddReward( 1384, { fEventTime = 10, iRewardID = 1314,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 데스사이드 전투 1개 (1일) )
GameEvent:AddReward( 1385, { fEventTime = 10, iRewardID = 1315,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 데스사이드 증강 1개 (1일) )
GameEvent:AddReward( 1386, { fEventTime = 10, iRewardID = 1316,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 데스사이드 적중 1개 (1일) )

GameEvent:AddReward( 1387, { fEventTime = 10, iRewardID = 1317,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 아리엘의 할로윈 사탕 1개 )
GameEvent:AddReward( 1388, { fEventTime = 10, iRewardID = 1318,  bGiveOnce =  True} )			 					--2012.10.24일 할로윈 공통 이벤트 			( 아리엘의 할로윈 사탕 2개 )

GameEvent:AddReward( 1389, { fEventTime = 1, iRewardID = 1319,  bGiveOnce =  True} )			 					--2012.10.24일 헤니르 및 비던 이벤트 		( 비밀던전 입장권 세트 큐브	 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1390, { fEventTime = 10, iRewardID = 1320,  bGiveOnce =  True} )			 					--2012.11.07일 검의 길 활력 vs 파괴 이벤트 			( 활력의 검사 (15일) 칭호 1개 )
GameEvent:AddReward( 1391, { fEventTime = 10, iRewardID = 1321,  bGiveOnce =  True} )			 					--2012.11.07일 검의 길 활력 vs 파괴 이벤트 			( 파괴의 검사 (15일) 칭호 1개 )
GameEvent:AddReward( 1392, { fEventTime = 10, iRewardID = 1322,  bGiveOnce =  True} )			 					--2012.11.07일 검의 길 활력 vs 파괴 이벤트 			( 활력의 룬 (15일) 1개 )
GameEvent:AddReward( 1393, { fEventTime = 10, iRewardID = 1323,  bGiveOnce =  True} )			 					--2012.11.07일 검의 길 활력 vs 파괴 이벤트 			( 파괴의 룬 (15일) 1개 )

GameEvent:AddReward( 1394, { fEventTime = 120, iRewardID = 1324,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } ) --2012.11.07일 검의 길 검사의 깨달음  이벤트 			( 검사의 깨달음 1개 )

GameEvent:AddReward( 1395, { fEventTime = 1, iRewardID = 1325,  bGiveOnce =  True} )			 					--2012.11.07일 엘소드 첫번째 생일파티에 여러분을 초대합니다!  이벤트 			( 생일 축하 풍선 ( 1일 )1개 )
GameEvent:AddReward( 1396, { fEventTime = 1, iRewardID = 1326,  bGiveOnce =  True} )			 					--2012.11.07일 엘소드 첫번째 생일파티에 여러분을 초대합니다!  이벤트 			( 엄마팬더 가면 ( 7일 )1개 )
GameEvent:AddReward( 1397, { fEventTime = 1, iRewardID = 1327,  bGiveOnce =  True} )			 					--2012.11.07일 초대받지 못한 불청객의 습격! 생일 케이크를 지켜라  이벤트 			( 케이크 수호 던전 입장권  1개 )

GameEvent:AddReward( 1398, { fEventTime = 111, iRewardID = 1328, iMinLevel = 	30	,   iMaxLevel =	60,  bGiveOnce =  True, bAccountEvent = True } )			--2012.11.07일 11월 11일 13:00 보너스 캐시 50 특별 보상  이벤트 			( 캐쉬 교환 카드 1개 )
GameEvent:AddReward( 1399, { fEventTime = 11, iRewardID = 1329,  bGiveOnce =  True} )			 					--2012.11.07일 11월 11일 13:00 보너스 캐시 50 특별 보상  이벤트 			( 생일 축하 풍선(무옵 영구권) 1개 )


GameEvent:AddReward( 1400, { fEventTime = 60, iRewardID = 1330,  bGiveOnce =  True} )			 					--lygan_조성욱 //혹시나 하는 마음에 세팅

---------------------------------------------------------------------------------------------------
--2012.11.15 lygan_조성욱 규수형이 근성도 소모할때 아이템 지급해주는거 중국에 맞게 추가 작업 해주기 전엔 아래와 같이 사용해야 함 여기서 틀만 만들고 본체는 EventSpiritData.lua 에 있다
GameEvent:AddReward( 1410, {bGiveOnce =  True} )																							--2012.11.15 lygan_조성욱 --규수형 작업 머지하고 근성도 무제한 이벤트 할려면 이 아이디 사용해야 함

GameEvent:AddReward( 1411, {bGiveOnce =  True} )																							--2012.11.21일 2. 아담스와 함께 하는 낚시 준비물 챙기기 2 _ 미끼편  이벤트 			( 어초 10개 ) --근성도 80% 남았을때 보상
GameEvent:AddReward( 1412, {bGiveOnce =  True} )																							--2012.11.21일 2. 아담스와 함께 하는 낚시 준비물 챙기기 2 _ 미끼편  이벤트 			( 귀뚜라미 10개 ) --근성도 60% 남았을때 보상
GameEvent:AddReward( 1413, {bGiveOnce =  True} )																							--2012.11.21일 2. 아담스와 함께 하는 낚시 준비물 챙기기 2 _ 미끼편  이벤트 			( 지렁이 10개 ) --근성도 40% 남았을때 보상
GameEvent:AddReward( 1414, {bGiveOnce =  True} )																							--2012.11.21일 2. 아담스와 함께 하는 낚시 준비물 챙기기 2 _ 미끼편  이벤트 			( 새우 10개 ) --근성도 20% 남았을때 보상
GameEvent:AddReward( 1415, {bGiveOnce =  True} )																							--2012.11.21일 2. 아담스와 함께 하는 낚시 준비물 챙기기 2 _ 미끼편  이벤트 			( 붕어 10개 ) --근성도 0% 남았을때 보상

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1420, { fEventTime = 10, iRewardID = 1350,  bGiveOnce =  True} )			 					--2012.12.5일 1. The Rage!  이벤트  The Rage(15일권) ( 칭호 1개 와 분노의 반지(15일) 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1430, { fEventTime = 50, iRewardID = 1430,	 bDirectReward  = True } ) 							--2012.12.18 매일매일 메리 크리스마스 이벤트 ( 크리스마스 선물 상자, 크리스마스 특별 선물 상자 차등 지급 )

GameEvent:AddReward( 1431, { fEventTime = 10, iRewardID = 1360,  bGiveOnce =  True} )								--2012.12.18일 새로운 검사의 상징 이벤트 ( 시스나이트 전야 )			( 콘웰의 헌신 ( 7일 ) 1개 )
GameEvent:AddReward( 1432, { fEventTime = 100, iRewardID = 1361,  bGiveOnce =  True, bAccountEvent = True } )		--2012.12.18일 크리스마스 특별 선물 받으면 기쁜 돌! 이벤트 ( 크리스마스 이벤트 )		( 플루오스 스톤 교환권 1개 )

GameEvent:AddReward( 1433, { fEventTime = 100, iRewardID = 1362,  bGiveOnce =  True, bAccountEvent = True } )		--2012.12.18일 크리스마스 특별 선물 이벤트 메리 크리스마스!( 크리스마스 이벤트 )		( 크리스마스 지팡이 사탕 장식 액세서리 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1440, { fEventTime = 1, iRewardID = 1370,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ELSWORD"], iMinLevel = 	21	,   iMaxLevel =	30, bCurrentUnitEvent = True } )								--2012.12.20일 코보사의 정규 모험가 재교육 프로그램  ( 시스나이트 이벤트 )		( 모험가 기초 교육 수강권 1개 )
GameEvent:AddReward( 1441, { fEventTime = 1, iRewardID = 1371,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ELSWORD"], iMinLevel = 	31	,   iMaxLevel =	40, bCurrentUnitEvent = True } )								--2012.12.20일 코보사의 정규 모험가 재교육 프로그램  ( 시스나이트 이벤트 )		( 모험가 실무 교육 수강권 1개 )
GameEvent:AddReward( 1442, { fEventTime = 1, iRewardID = 1372,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ELSWORD"], iMinLevel = 	41	,   iMaxLevel =	50, bCurrentUnitEvent = True } )								--2012.12.20일 코보사의 정규 모험가 재교육 프로그램  ( 시스나이트 이벤트 )		( 모험가 2차 직무 교육 수강권 1개 )
GameEvent:AddReward( 1443, { fEventTime = 1, iRewardID = 1373,  bGiveOnce =  True, cUnitType = UNIT_TYPE["UT_ELSWORD"], iMinLevel = 	51	,   iMaxLevel =	60, bCurrentUnitEvent = True } )								--2012.12.20일 코보사의 정규 모험가 재교육 프로그램  ( 시스나이트 이벤트 )		( 모험가 승급 교육 수강권 1개 )

GameEvent:AddReward( 1444, { fEventTime = 100, iRewardID = 1374,  bGiveOnce =  True} )								--2012.12.21일 2012년 못 다 이룬 연초 결심(12월 26일 ~ 12월 31일)  ( 신년 이벤트 )		( 되찾은 의지 1개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1450, { fEventTime = 10, iRewardID = 1380,  bGiveOnce =  True} )								--2012.01.09일 마법소녀의 날개 이벤트		( 마법소녀의 소망 (7일) 1 개 )

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1451, { fEventTime = 30, iRewardID = 1381,  bGiveOnce =  True} )								--마법소녀의 희망(15일권) 큐브

GameEvent:AddReward( 1452, { fEventTime = 10, iRewardID = 1382,  bGiveOnce =  True} )								--마법소녀의 수련장 입장권 3장
GameEvent:AddReward( 1453, { fEventTime = 10, iRewardID = 1383,  bGiveOnce =  True} )								--마법소녀의 수련장 입장권 5장

---------------------------------------------------------------------------------------------------

GameEvent:AddReward( 1454, { fEventTime = 1, iRewardID = 1385,  bGiveOnce =  True} )								--나소드 분광석
GameEvent:AddReward( 1455, { fEventTime = 30, iRewardID = 1386,  bGiveOnce =  True} )								--코드 분석 회로


--코드일렉트라, 춘절, 발렌타인
GameEvent:AddReward( 1456, { fEventTime = 120, iRewardID = 1387,  bGiveOnce =  True} )							--코보익스프레스티켓
GameEvent:AddReward( 1457, { fEventTime = 30, iRewardID = 1388,  bGiveOnce =  True, bAccountEvent = True } )	--복주머니
GameEvent:AddReward( 1458, { fEventTime = 88, iRewardID = 1389,  bGiveOnce =  True, bAccountEvent = True } )	--행운의 비약
GameEvent:AddReward( 1459, { fEXPRate = 1.0, cUnitClass = UNIT_TYPE["UT_EVE"], iUnitLevel = 20 } )					--코드 일렉트라


--태그모드
GameEvent:AddReward( 1460, { fEventTime = 10, iRewardID = 1390,  bGiveOnce =  True} )							--마법의 목걸이
GameEvent:AddReward( 1461, { fEventTime = 10, iRewardID = 1391,  bGiveOnce =  True} )							--기반(1일권) 큐브
GameEvent:AddReward( 1462, { fEventTime = 30, iRewardID = 1392,  bGiveOnce =  True} )							--태그모드 무기아바타

--웨낌전야
GameEvent:AddReward( 1463, { fEventTime = 10, iRewardID = 1393,  bGiveOnce =  True} )							--건블레이드 (영구)
GameEvent:AddReward( 1464, { fEventTime = 10, iRewardID = 1394,  bGiveOnce =  True} )							--코보 용병단 스타트업 큐브 열쇠

--웨폰테이커
GameEvent:AddReward( 1465, { fEXPRate = 1.0, cUnitClass = UNIT_TYPE["UT_RAVEN"], iUnitLevel = 15 } )											--웨폰테이커를 위한 레이븐 경험치 보너스
GameEvent:AddReward( 1466, { fEventTime = 30, iRewardID = 1396,  bGiveOnce =  True, cUnitClass = UNIT_CLASS["UC_RAVEN_FIGHTER"]} )		--돌아온 지휘관의 큐브 1
--등급 맞춤형 트레이닝!
GameEvent:AddReward( 1467, { fEventTime = 30, iRewardID = 1404,  bGiveOnce =  True, iMinLevel = 20 } )			--입문 등급 뱃지

--미션 임파서블
GameEvent:AddReward( 1468, { fEventTime = 30, iRewardID = 1405,  bGiveOnce =  True } )			--2013.03.27 메모리카드A
GameEvent:AddReward( 1469, { fEventTime = 30, iRewardID = 1406,  bGiveOnce =  True } )			--2013.03.27 메모리카드B
GameEvent:AddReward( 1470, { fEventTime = 30, iRewardID = 1407,  bGiveOnce =  True } )			--2013.03.27 메모리카드C
GameEvent:AddReward( 1471, { fEventTime = 30, iRewardID = 1408,  bGiveOnce =  True } )			--2013.03.27 메모리카드D
GameEvent:AddReward( 1472, { fEventTime = 30, iRewardID = 1409,  bGiveOnce =  True } )			--2013.03.27 메모리카드E
GameEvent:AddReward( 1473, { fEventTime = 60, iRewardID = 1410,  bGiveOnce =  True } )			--2013.03.27 메모리카드F

GameEvent:AddReward( 1474, { fEventTime = 40, iRewardID = 1411,  bGiveOnce =  True } )			--2013.03.27 보호의 부적 1개
GameEvent:AddReward( 1475, { fEventTime = 70, iRewardID = 1412,  bGiveOnce =  True } )			--2013.03.27 보호의 부적 2개

--나이트와쳐 전야
GameEvent:AddReward( 1476, { fEventTime = 30, iRewardID = 1413,  bGiveOnce =  True } )			--2013.04.03 영웅에 대한 소식지
GameEvent:AddReward( 1477, { fEventTime = 60, iRewardID = 1414,  bGiveOnce =  True } )			--2013.04.03 엘드랏실의 축복

--청명절
GameEvent:AddReward( 1478, { fEventTime = 20, iRewardID = 1415,  bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 0 } )			--2013.04.03 아직 시들지 않은
GameEvent:AddReward( 1479, { fEventTime = 20, iRewardID = 1416,  bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 1478 } )		--2013.04.03 아직 시들지 않은
GameEvent:AddReward( 1480, { fEventTime = 20, iRewardID = 1417,  bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 1479 } )		--2013.04.03 아직 시들지 않은
GameEvent:AddReward( 1481, { fEventTime = 20, iRewardID = 1418,  bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 1480 } )		--2013.04.03 아직 시들지 않은
GameEvent:AddReward( 1482, { fEventTime = 20, iRewardID = 1415,  bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 1481 } )		--2013.04.03 아직 시들지 않은
GameEvent:AddReward( 1483, { fEventTime = 20, iRewardID = 1416,  bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 1482 } )		--2013.04.03 아직 시들지 않은
GameEvent:AddReward( 1484, { fEventTime = 20, iRewardID = 1417,  bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 1483 } )		--2013.04.03 아직 시들지 않은
GameEvent:AddReward( 1485, { fEventTime = 20, iRewardID = 1418,  bGiveOnce =  True, bRepeatRewardEvent = True, iBeforeEventID = 1484 } )		--2013.04.03 아직 시들지 않은

--트래핑레인저
GameEvent:AddReward( 1486, { fEventTime = 10, iRewardID = 1419,  bGiveOnce =  True } )		--2013.04.10 엘드라실의 신성한 큐브

--몬스터카드
GameEvent:AddReward( 1487, { fEventTime = 1, iRewardID = 1420,  bGiveOnce =  True } )		--2013.04.24 의문의 몬스터 카드
GameEvent:AddReward( 1488, { fEventTime = 1, iRewardID = 1421,  bGiveOnce =  True } )		--2013.04.24 몬스터 카드 수집가의 모자

--결혼식이벤트(은경)
GameEvent:AddReward( 1489, { fEventTime = 10, iRewardID = 1427,  bGiveOnce =  True } )		--2013.04.24 실반지

--월리의 선물 공장
GameEvent:AddReward( 1490, { fEventTime = 10, iRewardID = 1428, bGiveOnce = True, } )       --2013.05.08 윌리의 선물공장 출입증

--필드 접속 이벤트 
--필드 100% 즐기기 필수 가이드북 (한번 / 캐릭터 단위 / 10분접속 /10이상)
GameEvent:AddReward( 1491, { fEventTime = 10, iRewardID = 1429, bGiveOnce = True, iMinLevel = 10, } )       --2013.05.08
--필드 모험 준비 패키지 (한번 / 계정 단위 / 30분접속 /10이상)
GameEvent:AddReward( 1492, { fEventTime = 30, iRewardID = 1430, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10, } )      --2013.05.08

--월리의 선물 공장 (평일용)
GameEvent:AddReward( 1493, { fEventTime = 10, iRewardID = 1433, bGiveOnce = True, } )       --2013.05.08 윌리의 선물공장 출입증

--필드 이벤트
GameEvent:AddReward( 1495, { fEventTime = 10, iRewardID = 1435, bGiveOnce = True, iMinLevel = 10 } )	--필드 쿠키
GameEvent:AddReward( 1496, { fEventTime = 60, iRewardID = 1436, bGiveOnce = True, iMinLevel = 10 } )	--황금 필드 쿠키

--필드 보스 몬스터들의 사진을 촬영하자
GameEvent:AddReward( 1497, { fEventTime = 10, iRewardID = 1437, bGiveOnce = True } )	--1분 접속 시 카메라 ES-888(악세서리) 우편 지급(캐릭터 단위)
GameEvent:AddReward( 1498, { fEventTime = 30, iRewardID = 1438, bGiveOnce = True } )	--30분 접속 시 대박 필드 보스 큐브 우편 지급(캐릭터 단위)

--나이트와쳐 전야 이벤트
GameEvent:AddReward( 1499, { fEventTime = 30, iRewardID = 1439, bGiveOnce = True } )	--퐁고족의 서신

--나이트와쳐 이벤트
GameEvent:AddReward( 1500, { fEventTime = 1, iRewardID = 1440, bGiveOnce = True, iEventItemID = 85002576 } )	--강렬한 에렌딜의 영혼 큐브 열쇠
GameEvent:AddReward( 1501, { fEventTime = 1, iRewardID = 1441, bGiveOnce = True, iEventItemID = 85002577 } )	--온유한 에렌딜의 영혼 큐브 열쇠
GameEvent:AddReward( 1502, { fEventTime = 1, iRewardID = 1442, bGiveOnce = True, iEventItemID = 85002578 } )	--내유외강 에렌딜의 영혼 큐브 열쇠

GameEvent:AddReward( 1503, { fEventTime = 1, iRewardID = 1443, bGiveOnce = True } )		--킹 나소드 소굴 입장 티켓
GameEvent:AddReward( 1504, { fEventTime = 20, iRewardID = 1444, bGiveOnce = True } )	--아기 퐁고의 메아리
GameEvent:AddReward( 1505, { fEventTime = 40, iRewardID = 1444, bGiveOnce = True } )	--아기 퐁고의 메아리2
GameEvent:AddReward( 1506, { fEventTime = 60, iRewardID = 1444, bGiveOnce = True } )	--아기 퐁고의 메아리3
GameEvent:AddReward( 1507, { fEventTime = 10, iRewardID = 1445, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_LIRE_NIGHT_WATCHER"] } )	--나소드의 비밀 열쇠
GameEvent:AddReward( 1508, { fEventTime = 30, iRewardID = 1446, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_LIRE_NIGHT_WATCHER"] } )	--킹 나소드의 비밀 열쇠

GameEvent:AddReward( 1509, { fEventTime = 10, iRewardID = 1447, bAccountEvent = True, bGiveOnce = True } )		--화동 2군 오픈 기념 큐브
GameEvent:AddReward( 1510, { fEventTime = 10, iRewardID = 1448, bAccountEvent = True, bGiveOnce = True } )		--빨강 리본 커팅 가위
GameEvent:AddReward( 1511, { fEventTime = 10, iRewardID = 1449, bAccountEvent = True, bGiveOnce = True } )		--파랑 리본 커팅 가위
GameEvent:AddReward( 1512, { fEventTime = 10, iRewardID = 1450, bAccountEvent = True, bGiveOnce = True } )		--초록 리본 커팅 가위
GameEvent:AddReward( 1513, { fEventTime = 10, iRewardID = 1451, bAccountEvent = True, bGiveOnce = True } )		--노랑 리본 커팅 가위

--제목 : 인피니티 소드의 등장
--이벤트 기간 : 2013년 7월 17일 ~ 2013년 7월 23일 (1주일)
GameEvent:AddReward( 1514, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ELSWORD_SHEATH_KNIGHT"], } ) --35레벨 이하 시스나이트는 경험치 2배 획득
GameEvent:AddReward( 1515, { fEventTime =  30, iRewardID = 1452, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_ELSWORD_SHEATH_KNIGHT"], } ) --인피니티 소드 준비 큐브
GameEvent:AddReward( 1516, { fEventTime =  60, iRewardID = 1453, bGiveOnce = True, iMinLevel = 10, } ) --미제련된 무한의 콘웰

--컴백 이벤트&인피니티소드 이벤트 (2013-07-24 ~ 2013-07-31)
--복귀유저
GameEvent:AddReward( 1517, { fEventTime =  30, iRewardID = 1454, bGiveOnce = True, bAccountEvent = True, iMinLevel = 10, iCustomEventID = 1} ) --기본 아바타 무기(영구) 랜덤 큐브, 돌아온 영웅의 칭호 (15일권)
GameEvent:AddReward( 1518, { fEventTime =  10, iRewardID = 1455, bGiveOnce = True, bAccountEvent = True, iMinLevel = 10, iCustomEventID = 1} ) --돌아온 영웅의 증표 1개
--기존, 신규유저
GameEvent:AddReward( 1519, { fEventTime =  10, iRewardID = 1456, bGiveOnce = True, bAccountEvent = True, iMinLevel = 10, iCustomEventID = 2} ) --성실한 모험가의 증표 1개 성실한 모험가의 칭호 (1일권)
--주말 동안 매일 모든 유저 60분 접속 시 돌아온 영웅의 코인 우편 보상 획득
GameEvent:AddReward( 1520, { fEventTime =  60, iRewardID = 1457, bGiveOnce = True, bAccountEvent = True, iMinLevel = 10, } ) --영웅의 코인 1개
--무한의 큐브 (2013-06-26 ~ 2013-07-09)
GameEvent:AddReward( 1521, { fEventTime =  1, iRewardID = 1458, bGiveOnce = True, iMinLevel = 10, } ) --미약한 무한의 큐브 1개
GameEvent:AddReward( 1522, { fEventTime =  1, iRewardID = 1459, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_ELSWORD_SHEATH_KNIGHT"], iMinLevel = 10, } ) --무한의 큐브 1개 다크엘의 결정 1개
GameEvent:AddReward( 1523, { fEventTime =  1, iRewardID = 1460, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_ELSWORD_INFINITY_SWORD"], iMinLevel = 10, } ) --진화된 무한의 큐브 1개
GameEvent:AddReward( 1524, { fEventTime =  10, iRewardID = 1461, bGiveOnce = True, } ) --인피니티 소드 준비 큐브 오픈 열쇠

-- 디멘션 위치 전직을 준비하자.  2013년 07월 31일 ~ 2013년 08월 06일 
GameEvent:AddReward( 1525, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ARME_BATTLE_MAGICIAN"], } ) -- 35레벨 이하 배틀매지션는 경험치 2배 획득
GameEvent:AddReward( 1526, { fEventTime =  30, iRewardID = 1462, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_ARME_BATTLE_MAGICIAN"], } ) -- 디멘션 위치 준비 큐브

-- 대전 이벤트
GameEvent:AddReward( 1527, { fEventTime =  30, iRewardID = 1463, bGiveOnce = True, } ) --영웅의 영광

-- 베커 및 전야 이벤트
GameEvent:AddReward( 1528, { fEventTime =  30, iRewardID = 1464, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_RAVEN_WEAPON_TAKER"]} ) -- 베테랑 커맨더 준비 큐브
GameEvent:AddReward( 1529, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_RAVEN_WEAPON_TAKER"], } ) -- 35레벨 이하 웨폰 테이커 는 경험치 2배 획득

-- 코드 배틀세라프 전야
GameEvent:AddReward( 1530, { fEventTime =  30, iRewardID = 1465, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_EVE_ELECTRA"]} ) -- 코드 배틀세라프 준비 큐브
GameEvent:AddReward( 1531, { fEventTime =  30, iRewardID = 1466, bGiveOnce = True, cUnitClass = UNIT_CLASS["UC_EVE_ELECTRA"]} ) -- 세라프 코드 랜덤 큐브
GameEvent:AddReward( 1532, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_EVE_ELECTRA"], } ) -- 35레벨 이하 코드일렉트라 는 경험치 2배 획득
GameEvent:AddReward( 1533, { fEventTime =  20, iRewardID = 1467, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  A
GameEvent:AddReward( 1534, { fEventTime =  40, iRewardID = 1468, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  B
GameEvent:AddReward( 1535, { fEventTime =  60, iRewardID = 1469, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  C
GameEvent:AddReward( 1536, { fEventTime =  80, iRewardID = 1470, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  D
GameEvent:AddReward( 1537, { fEventTime =  100, iRewardID = 1467, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  A
GameEvent:AddReward( 1538, { fEventTime =  120, iRewardID = 1468, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  B
GameEvent:AddReward( 1539, { fEventTime =  140, iRewardID = 1469, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  C
GameEvent:AddReward( 1540, { fEventTime =  160, iRewardID = 1470, bGiveOnce = True, iMinLevel = 10, } ) -- 세라프 코드  D

-- 택틱컬 트루퍼 전야
GameEvent:AddReward( 1541, { fEventTime =  30, iRewardID = 1471, bGiveOnce = True, iMinLevel = 35, cUnitClass = UNIT_CLASS["UC_CHUNG_SHELLING_GUARDIAN"]} ) -- 택티컬 트루퍼 준비 큐브
GameEvent:AddReward( 1542, { fEventTime =  30, iRewardID = 1472, bGiveOnce = True, iMinLevel = 10, } ) -- 디스프로즌 에너지
GameEvent:AddReward( 1543, { fEXPRate = 1.0, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_CHUNG_SHELLING_GUARDIAN"], } ) -- 35레벨 이하 쉘링가디언은 경험치 2배 획득

-- 개학 이벤트
GameEvent:AddReward( 1544, { fEventTime = 60, iRewardID = 1474, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자(월,화,수,목,금)
GameEvent:AddReward( 1545, { fEventTime = 60, iRewardID = 1475, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자(월,화)
GameEvent:AddReward( 1546, { fEventTime = 30, iRewardID = 1476, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠(월)
GameEvent:AddReward( 1547, { fEventTime = 30, iRewardID = 1477, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠(화)
GameEvent:AddReward( 1548, { fEventTime = 30, iRewardID = 1478, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠(수)
GameEvent:AddReward( 1549, { fEventTime = 30, iRewardID = 1479, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠(목)
GameEvent:AddReward( 1550, { fEventTime = 30, iRewardID = 1480, bGiveOnce = True } )  -- 방과 후 엘소드 선물상자 열쇠(금)
GameEvent:AddReward( 1551, { fEventTime = 1, iRewardID = 1481, bGiveOnce = True, iMinLevel = 20, iMaxLevel = 31 } )  -- 20레벨 10강 무기 큐브
GameEvent:AddReward( 1551, { fEventTime = 1, iRewardID = 1482, bGiveOnce = True, iMinLevel = 32, iMaxLevel = 43 } )  -- 32레벨 10강 무기 큐브
GameEvent:AddReward( 1551, { fEventTime = 1, iRewardID = 1483, bGiveOnce = True, iMinLevel = 44, iMaxLevel = 51 } )  -- 44레벨 10강 무기 큐브
GameEvent:AddReward( 1551, { fEventTime = 1, iRewardID = 1484, bGiveOnce = True, iMinLevel = 52, iMaxLevel = 59 } )  -- 52레벨 10강 무기 큐브
GameEvent:AddReward( 1551, { fEventTime = 1, iRewardID = 1485, bGiveOnce = True, iMinLevel = 60, iMaxLevel = 65 } )  -- 60레벨 10강 무기 큐브
GameEvent:AddReward( 1552, { fEventTime = 1, iRewardID = 1486, bGiveOnce = True, } )  -- 주말 칭호(3일권)

--중추절 이벤트
GameEvent:AddReward( 1553, { fEventTime = 30, iRewardID = 1487, bGiveOnce = True, } )  --달나라 우주선 연료
GameEvent:AddReward( 1554, { fEventTime = 60, iRewardID = 1487, bGiveOnce = True, } )  --달나라 우주선 연료
GameEvent:AddReward( 1555, { fEventTime = 90, iRewardID = 1487, bGiveOnce = True, } )  --달나라 우주선 연료
GameEvent:AddReward( 1556, { fEventTime = 120, iRewardID = 1487, bGiveOnce = True, } )  --달나라 우주선 연료

-- 택틱컬 트루퍼
GameEvent:AddReward( 1557, { fEventTime =  60, iRewardID = 1489, bGiveOnce = True, } ) -- 신규 2차전직 캐릭터 카드 랜덤 큐브

--국경절 이벤트
GameEvent:AddReward( 1558, { fEventTime =  10, iRewardID = 1490, bGiveOnce = True, } ) --명승고적 칭호 큐브
GameEvent:AddReward( 1559, { fEventTime =  10, iRewardID = 1491, bGiveOnce = True, } ) --달토끼(로켓)

-- 아이템 개편 이벤트
GameEvent:AddReward( 1560, { fEventTime = 60, iRewardID = 1500, bGiveOnce = True } )  -- 루리엘의 축복받은 시공간의 주문서 교환권
GameEvent:AddReward( 1561, { fEventTime = 60, iRewardID = 1501, bGiveOnce = True } )  -- 루리엘의 축복받은 시공간의 주문서 연동레벨

--{{ 할로윈 이벤트 2013년 10월 23일 ~ 2013년 11월 5일
--할로윈 연회 참석 준비
GameEvent:AddReward( 1562, { fEventTime =  10, iRewardID = 1502, bGiveOnce = True, } ) -- 10분 동안 접속 시 할로위치(15일권) 지급
GameEvent:AddReward( 1563, { fEventTime =  1, iRewardID = 1503, bGiveOnce = True, } ) -- 0분 동안 접속 시 매일 할로윈 몬스터 코스프레 교환권 1개 지급
GameEvent:AddReward( 1564, { fEventTime =  120, iRewardID = 1504, bGiveOnce = True, bAccountEvent = True, } ) -- 주말 및 할로윈 당일(10/26 ~ 10/27, 10/31, 11/2~11/3)에 120분 동안 연속으로 접속 시 빛나는 할로윈 몬스터의 영혼 1개 지급
--}}

-- 스킬트리 개편 이벤트 2013-11-06 정기 점검 후 ~ 2013-11-20 정기 점검 전
-- 복귀 유저와 성실 유저를 위한 선물
--GameEvent:AddReward( 1565, { fEventTime = 10, iRewardID = 1505, bGiveOnce = True, bAccountEvent = True, bNewUnitEvent = True } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/10월16일이후 접속기록 없는 유저)
--GameEvent:AddReward( 1566, { fEventTime = 10, iRewardID = 1506, bGiveOnce = True, bAccountEvent = True, bNewUnitEvent2 = True } )  -- 성실 유저인증 티켓 (한번/캐릭터/10분/10월16일이후 접속기록 있는 유저)
-- _복귀 유저를 위한 선물
-- 11/6, 11/13 강화를 하자!(1)
GameEvent:AddReward( 1567, { fEventTime = 10, iRewardID = 1507, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1568, { fEventTime = 10, iRewardID = 1508, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
-- 11/7, 11/14 강화를 하자!(2)
GameEvent:AddReward( 1569, { fEventTime = 10, iRewardID = 1509, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1570, { fEventTime = 10, iRewardID = 1510, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
-- 11/8, 11/15 강화를 하자!(3)
GameEvent:AddReward( 1571, { fEventTime = 10, iRewardID = 1511, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1572, { fEventTime = 10, iRewardID = 1512, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
-- 11/9, 11/16 감정을 해 보자!(1)
GameEvent:AddReward( 1573, { fEventTime = 10, iRewardID = 1513, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1574, { fEventTime = 10, iRewardID = 1514, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--11/10, 11/17 감정을 해 보자!(2)
GameEvent:AddReward( 1575, { fEventTime = 10, iRewardID = 1515, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1576, { fEventTime = 10, iRewardID = 1516, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--11/11, 11/18 감정을 해 보자!(3)
GameEvent:AddReward( 1577, { fEventTime = 10, iRewardID = 1517, bGiveOnce = True, iMinLevel = 10, iMaxLevel = 50, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
GameEvent:AddReward( 1578, { fEventTime = 10, iRewardID = 1518, bGiveOnce = True, iMinLevel = 51, iMaxLevel = 60, iEventItemID = 67006456 } )  -- 복귀 유저인증 티켓 (한번/캐릭터/10분/복귀유저인증티켓소지)
--11/12, 11/19 감정을 해 보자!(4)
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

-- 2주년 이벤트 2013-11-06 업데이트 후~ 2013-11-12 
-- 이벤트 기간동안 1분 접속 시 “생일 축하 모자 큐브” 1개 지급
GameEvent:AddReward( 1588, { fEventTime = 1, iRewardID = 1531, bGiveOnce = True } ) -- 생일 축하 모자 큐브
-- 이벤트 기간동안 30분 접속 시 매일 “2주년 축하 선물” 1개 지급
GameEvent:AddReward( 1589, { fEventTime = 30, iRewardID = 1532, bGiveOnce = True, bAccountEvent = True, } ) -- 2주년 축하 선물상자 (빨강) 1개
GameEvent:AddReward( 1590, { fEventTime = 30, iRewardID = 1533, bGiveOnce = True, bAccountEvent = True, } ) -- 2주년 축하 선물상자 (하양) 1개
GameEvent:AddReward( 1591, { fEventTime = 30, iRewardID = 1534, bGiveOnce = True, bAccountEvent = True, } ) -- 2주년 축하 선물상자 (노랑) 1개
GameEvent:AddReward( 1592, { fEventTime = 30, iRewardID = 1535, bGiveOnce = True, bAccountEvent = True, } ) -- 2주년 축하 선물상자 (파랑) 1개
GameEvent:AddReward( 1593, { fEventTime = 30, iRewardID = 1536, bGiveOnce = True, bAccountEvent = True, } ) -- 2주년 축하 선물상자 (초록) 1개
GameEvent:AddReward( 1594, { fEventTime = 30, iRewardID = 1537, bGiveOnce = True, bAccountEvent = True, } ) -- 2주년 축하 선물상자 (까망) 1개
GameEvent:AddReward( 1595, { fEventTime = 30, iRewardID = 1538, bGiveOnce = True, bAccountEvent = True, } ) -- 2주년 축하 선물상자 (분홍) 1개
-- 2주년 이벤트 끝

-- 2013.11.20 샌더 이벤트
GameEvent:AddReward( 1596, { fEventTime = 1, iRewardID = 1539, bGiveOnce = True, iMinLevel = 10 }) -- Lv10 이상 최초 접속 시, 샌더마을 관광 가이드북 1개 지급
-- 2013.12.11 아라전야이벤트 30분, 60분
GameEvent:AddReward( 1597, { fEventTime = 30, iRewardID = 1541, bGiveOnce = True, iMinLevel = 10 } ) -- Lv10 이상 30분 접속 시 아라 가문의 문서 보관함 우편 지급(캐릭터 단위)
GameEvent:AddReward( 1598, { fEventTime = 60, iRewardID = 1542, bAccountEvent = True, bGiveOnce = True, iMinLevel = 10 } ) -- Lv10 이상 60분 접속 시 봉인된 구미호 은큐브 우편 지급(계정 단위)

-- 2013.12.04 어둠의 문 개편 이벤트
GameEvent:AddReward( 1602, { fEventTime = 30, iRewardID = 1552, bGiveOnce = True, } ) -- 새로운 어둠의 등장에 대한 지침서

-- 2013-12-24  제천 이벤트
GameEvent:AddReward( 1603, { fEventTime = 1, iRewardID = 1555, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 접속시, 아라응원 선물 1개
GameEvent:AddReward( 1604, { fEventTime = 1, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ELSWORD"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1605, { fEventTime = 1, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_LIRE"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1606, { fEventTime = 1, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_ARME"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1607, { fEventTime = 1, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_RAVEN"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1608, { fEventTime = 1, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_EVE"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개
GameEvent:AddReward( 1609, { fEventTime = 1, iRewardID = 1556, bGiveOnce = True, cUnitType = UNIT_TYPE["UT_CHUNG"] } )  -- 아라 이외 접속시, 아라 서포터 큐브  1개

-- 2013-12-18  아라 전직 이벤트
GameEvent:AddReward( 1610, { fEXPRate = 0.5, iUnitLevel = 64, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 캐릭터 63렙 이하 경험치 1.5배

-- 2013-12-24  아라 제천 이벤트
GameEvent:AddReward( 1611, { fEXPRate = 1.0, iUnitLevel = 35, cUnitType = UNIT_TYPE["UT_ARA"] } )  -- 아라 캐릭터 35렙 이하 경험치 2배

-- 2013-12-18  2. 아라 업데이트 기념 매일 매일 선물 이벤트
GameEvent:AddReward( 1612, { fEventTime = 30, iRewardID = 1557 } )  -- 빛 나는 푸른 구미호 꼬리 액세서리 2일권 큐브
GameEvent:AddReward( 1613, { fEventTime = 30, iRewardID = 1558 } )  -- 빛 나는 붉은 구미호 꼬리 액세서리 2일권 큐브
GameEvent:AddReward( 1614, { fEventTime = 30, iRewardID = 1559 } )  -- 루리엘의 플루오르 스톤 교환권 1개
GameEvent:AddReward( 1615, { fEventTime = 30, iRewardID = 1560 } )  -- 루리엘의 축복 받은 복원의 주문서 1개
GameEvent:AddReward( 1616, { fEventTime = 30, iRewardID = 1561 } )  -- 루리엘의 시공간의 주문서 교환권 1개
GameEvent:AddReward( 1617, { fEventTime = 30, iRewardID = 1562 } )  -- 축복 받은 무기 강화석 10개  축복 받은 방어구 강화석 20개


-- 2013.12.04 은행 공유 이벤트
GameEvent:AddReward( 1619, { fEventTime = 10, iRewardID = 1564, bGiveOnce = True, iMinLevel = 10 } )-- 휴대용 강철금고 (15일권)

-- : 2013-12-18  크리스마스
GameEvent:AddReward( 2000, { fEventTime = 25, iRewardID = 2000, bGiveOnce = True, } )  -- 폴라 베어 코스프레 세트(한벌)

-- 2013-12-27 2013년 마지막 접속 이벤트
GameEvent:AddReward( 2001, { fEventTime = 30, iRewardID = 2001, bGiveOnce = True, bAccountEvent = True, } )  -- 2013년 마지막 접속 인증 티켓

-- 2014-01-01 2014년 첫 접속 인증
GameEvent:AddReward( 2014, { fEventTime = 30, iRewardID = 2015, bGiveOnce = True, bAccountEvent = True } )  -- 포니 헤드 액세서리(3일권)


-- 2014 탈 것 이벤트 2014-01-27 10:00 ~ 2014-02-05 09:59
--1. 프로 라이더의 꿈 (2014-01-27 10:00 ~ 2014-02-05 09:59)
GameEvent:AddReward( 2015, { fEventTime = 10, iRewardID = 2016, bGiveOnce = True, } )  -- 라이딩 체험 ( 돌진 전차 모비 RT (1일권) )
GameEvent:AddReward( 2016, { fEventTime = 10, iRewardID = 2017, bGiveOnce = True, } )  -- 라이딩 체험 ( 바람의 코우시카 (1일권) )
GameEvent:AddReward( 2017, { fEventTime = 10, iRewardID = 2018, bGiveOnce = True, } )  -- 라이딩 체험 ( 에이션트 라이딩 뽀루 (1일권) )
GameEvent:AddReward( 2018, { fEventTime = 10, iRewardID = 2019, bGiveOnce = True, } )  -- 라이딩 체험 ( 3종류 (1일권) )
-- 2. 프로 라이더를 위한 특별 선물 (2014-02-01 00:00 ~ 2014-02-02 23:59)
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

-- 2014 아라2차전직(명왕) 이벤트 2014-01-22 10:00 ~ 2014-01-29 09:59
--1. 아라 신규 2차 전직 오픈 기념 (2014-01-22 10:00 ~ 2014-01-29 09:59)
GameEvent:AddReward( 2032, { fEXPRate = 0.5, iUnitLevel = 35, cUnitClass = UNIT_CLASS["UC_ARA_LITTLE_DEVIL"] } ) -- 35레벨 이하 소마 캐릭터 경험치 1.5배
GameEvent:AddReward( 2033, { fEventTime = 35, iRewardID = 2034, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 1/22 2차 전직 캐릭터 World War 칭호(8일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2034, { fEventTime = 35, iRewardID = 2035, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 1/23 2차 전직 캐릭터 안티 포이즌 반지(3일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2035, { fEventTime = 35, iRewardID = 2036, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 1/24 2차 전직 캐릭터 소환석 : 돌진 전차 모비RT (3일권) 1개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2036, { fEventTime = 35, iRewardID = 2037, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 1/25 2차 전직 캐릭터 루리엘의 플루오르스톤 교환권 1개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2037, { fEventTime = 35, iRewardID = 2038, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 1/26 2차 전직 캐릭터 2차 전직 프로모션 아바타 무기(포이즈닝)(3일권) 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2038, { fEventTime = 35, iRewardID = 2039, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 1/27 2차 전직 캐릭터 선택 스킬 초기화 3개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2039, { fEventTime = 35, iRewardID = 2040, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 1/28 2차 전직 캐릭터 루리엘의 활력의 포션 10개, 루리엘의 마나 에릭실 10개 우편 지급(매일/캐릭터/35분)
GameEvent:AddReward( 2040, { fEventTime = 35, iRewardID = 2041, bGiveOnce = True, iUnitClassLevel = 2 } ) -- 2차 전직 캐릭터 부활석 3개 큐브 1개 우편 지급(매일/캐릭터/35분) ( 중국은 안줌 )

-- 2014 아라 신전직 전야 이벤트
GameEvent:AddReward( 2041, { fEventTime = 10, iRewardID = 2043, bGiveOnce = True} ) -- 미약한 사파의 정수 2개 우편 지급(매일/캐릭터/10분)
GameEvent:AddReward( 2042, { fEventTime = 10, iRewardID = 2044, bGiveOnce = True, iEventItemID = 85003833 } )	-- 정파 비전 큐브 열쇠 1개 우편 지급(한번/캐릭터/10분/정파비전 큐브 보유시)
GameEvent:AddReward( 2043, { fEventTime = 10, iRewardID = 2045, bGiveOnce = True, iEventItemID = 85003834 } )	-- 사파 비전 큐브 열쇠 1개 우편 지급(한번/캐릭터/10분/사파비전 큐브 보유시)

-- ES_CN 겨울방학, 엘소드와 함께! (2014-01-08 정기점검 이후 ~ 2014-01-22 정기점검 이전)
--GameEvent:AddReward( 2044, { fEventTime = 10, iRewardID = 2046, bGiveOnce = True, iMinLevel = 10,	byteCriterionUserType = CRITERION_USER_TYPE["CUT_NEW"],	byteCriterionPresentUserType = CRITERION_USER_TYPE["CUT_NEW"],	wstrCriterionDateBegin = '2014-01-08 06:00:00',	wstrCriterionDateEnd = '2014-01-22 06:00:00'		 } ) --겨울방학 신규 유저 지원 큐브 1개
GameEvent:AddReward( 2044, { fEventTime = 10, iRewardID = 2046, bGiveOnce = True,} ) --겨울방학 신규 유저 지원 큐브 1개
GameEvent:AddReward( 2045, { fEventTime = 10, iRewardID = 2047, bGiveOnce = True, iMinLevel = 10, } ) --겨울방학 버닝 유저 지원 큐브 1개, 겨울방학 버닝 드링크 1개

GameEvent:AddReward( 2046, { fEventTime = 1, iRewardID = 2048, bGiveOnce = True,} ) --금강야차 아바타(1일) 5세트 큐브

-- ES_CN 춘절 이벤트 2014-01-27 10:00 ~ 2014-02-12 정기점검 이전
--1. 2014년 새해 맞이 특별 접속 이벤트! (2014-01-27 10:00 ~ 2014-02-12 정기점검 이전)
GameEvent:AddReward( 10311,	{ fEventTime =  30,	iRewardID = 1000308, bGiveOnce =  True, iMinLevel = 10 } ) ----01/31 ~ 02/02 30분 접속 폴라 베어 코스프레 큐브 1개
GameEvent:AddReward( 10312,	{ fEventTime =  20,	iRewardID = 1000309, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) ----1/27 축복받은 방어구 강화석 14개
GameEvent:AddReward( 10313,	{ fEventTime =  20,	iRewardID = 1000310, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) ---- 1/28 축복받은 무기 강화석 14개
GameEvent:AddReward( 10314,	{ fEventTime =  20,	iRewardID = 1000311, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --- 1/29루리엘의 마나 에릭실 14개
GameEvent:AddReward( 10315,	{ fEventTime =  20,	iRewardID = 1000312, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) ---1/30루리엘의 활력의 포션 14개
GameEvent:AddReward( 10316,	{ fEventTime =  20,	iRewardID = 1000313, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) ---1/31루리엘의 얼음 조각상 가열기 1개
GameEvent:AddReward( 10317,	{ fEventTime =  20,	iRewardID = 1000314, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --2/1루리엘의 축복받은 시공간의 주문서 교환권 1개
GameEvent:AddReward( 10318,	{ fEventTime =  20,	iRewardID = 1000315, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --2/2루리엘의 플루오르 스톤 교환권 1개
GameEvent:AddReward( 10319,	{ fEventTime =  20,	iRewardID = 1000316, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) ---2/3루리엘의 완적 회복 포션 14개
GameEvent:AddReward( 10320,	{ fEventTime =  20,	iRewardID = 1000317, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) ---2/4루리엘의 각성 포션 14개
GameEvent:AddReward( 10321,	{ fEventTime =  20,	iRewardID = 1000318, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) ---- 2/5루리엘의 펫 시리얼 14개
GameEvent:AddReward( 10322,	{ fEventTime =  20,	iRewardID = 1000319, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --2/6고급 마법석 14개
GameEvent:AddReward( 10323,	{ fEventTime =  20,	iRewardID = 1000320, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --2/7엘의조각(불명) 14개
GameEvent:AddReward( 10324,	{ fEventTime =  20,	iRewardID = 1000321, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --- 2/8레어 아바타 풀셋 1일권 큐브
GameEvent:AddReward( 10325,	{ fEventTime =  20,	iRewardID = 1000322, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --- 2/9루리엘의 그노시스의 축복(30sp,14일)
GameEvent:AddReward( 10326,	{ fEventTime =  20,	iRewardID = 1000323, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --2/10마법석 14개
GameEvent:AddReward( 10327,	{ fEventTime =  20,	iRewardID = 1000324, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) --- 2/11부활석 14개 큐브
GameEvent:AddReward( 10328,	{ fEventTime =  20,	iRewardID = 1000325, bGiveOnce =  True, bAccountEvent = True, iMinLevel = 10 } ) -- 2/12루리엘의 망각의 알약 1개

-- 2014-02-12 정기점검 이후 ~ 2014-02-26 정기점검 결혼 이벤트
-- 1. 나의 반쪽을 찾아서 (2014-02-12 정기점검 이후 ~ 2014-02-26 정기점검 이전)
GameEvent:AddReward( 2051, { fEventTime = 10, iRewardID = 2051, bGiveOnce = True, iMinLevel = 10 } )  -- 가상 결혼 신청서 (한번/캐릭터/10분)

-- 스마트 밸런스! 2014-02-12 정기점검 이후 ~ 2014-02-18 23:59
GameEvent:AddReward( 2052, { fEventTime = 10, iRewardID = 2053, bGiveOnce = True, iMinLevel = 10 } ) -- 스마트 밸런스 쿨타임 가속기 (1일), 루리엘의 망각의 알약 (1일권) 1개

--2014원소절
GameEvent:AddReward( 2066, { fEventTime = 15, iRewardID = 2066,  bGiveOnce =  True, bRepeatRewardEvent = True, iMinLevel = 10, iBeforeEventID = 0 } )			--소환석 : 돌진 전차 모비RT (3일권)
GameEvent:AddReward( 2067, { fEventTime = 15, iRewardID = 2067,  bGiveOnce =  True, bRepeatRewardEvent = True, iMinLevel = 10, iBeforeEventID = 2066 } )		--어깨 망아지 인형(파랑) (3일권)
GameEvent:AddReward( 2068, { fEventTime = 15, iRewardID = 2068,  bGiveOnce =  True, bRepeatRewardEvent = True, iMinLevel = 10, iBeforeEventID = 2067 } )		--어깨 망아지 인형(분홍) (3일권)
GameEvent:AddReward( 2069, { fEventTime = 15, iRewardID = 2069,  bGiveOnce =  True, bRepeatRewardEvent = True, iMinLevel = 10, iBeforeEventID = 2068 } )		--레어 아바타 풀세트 (3일권) 큐브

--*이벤트 제목 : 발렌타인 데이 초콜릿 운반 작전
--*이벤트 기간 : 2014-02-12 업데이트 후 ~ 2014-02-26 업데이트 전
--1) 헬렌의 발렌타인 데이 초콜릿
GameEvent:AddReward( 2070,	{ fEventTime =  14,	iRewardID = 2070, bGiveOnce =  True, iMinLevel = 10 } ) -- 헬렌의 발렌타인데이 초콜릿 3개
-- 1) 기적의 발렌타인 데이
GameEvent:AddReward( 2071,	{ fEventTime =  14,	iRewardID = 2071, bGiveOnce =  True, iMinLevel = 10 } ) -- 기적의 발렌타인데이 반지(Lv.1)

--*이벤트 제목 : 불길한 모래 폭풍
--*이벤트 기간 : 2014-02-26 09:00 ~ 2014-03-12 08:59
--1) 샌더 마을의 서신
GameEvent:AddReward( 2053, { fEventTime = 60, iRewardID = 2054, bGiveOnce = True, iMinLevel = 10 } ) -- 풍화된 샌더 마을의 서신 조각 1개, 엘의 조각(윈드) 1개
                                                                                                                                           
-------------------------------------------------------------------------------------------------------------------------------------------
--영구 VIP 티켓
GameEvent:AddReward( 1494, { fEventTime = 0, iRewardID = 1434, bGiveOnce = False, iEventItemID = 84001750 } )	--VIP가열기

--광고판 등록
GameEvent:AddAdvertisement( 1, { URL = "DLG_Advertisement.tga" } )

--공지 이벤트 날릴 센터서버UID
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_TEST"], 0 ) --테스트
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_ZR01"], 30 ) --남방 1군
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_ZR02"], 30 )  --남방 2군
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_TJ01"], 30 )  --북방 1군
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_ZR03"], 30 )  --남방 3군
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_WH01"], 30 )  --남방 4군(우한)
GameEvent:AddNotifyMSGServerInfo( SERVER_GROUP_ID["SGI_WH02"], 30 )  --남방 5군(우한2)