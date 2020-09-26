-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

--한국 넥슨 피씨방 보너스 수치.
--#1arg : EXP rate
--#2arg : ED rate
--#3arg : AP rate
--#4arg : Spirit rate

--#ifndef SERV_PC_BANG_TYPE
--GameSysVal:AddKNXGameBang( 0.2, 0.5, 0.2, 0.8 )
--GameSysVal:AddKNXGameBang( 0.3, 0.5, 0.2, 0.8 )			-- 엘소드 PC방 이벤트 (2012년 12월 20일 ~ 2013년 2월 28일 정기 점검 전까지)
--void KGameSysVal::AddKNXGameBang_LUA( float fEXPRate, float fEDRate, float fVSPointRate, float fSpiritRate ) 
--#endif SERV_PC_BANG_TYPE
--GameSysVal:AddKNXGameBangPetEXP( 0.3 )					-- 엘소드 PC방 이벤트 (7월 5일 ~ 8월 30일 정기 점검 전까지)
--[[ void KGameSysVal::AddPCBang_LUA( int iPcBangType, float fEXPRate, float fEDRate, float fVSPointRate, bool bSpiritFree, float fPetEXPRate, 
int iAdditionalHenirReward, bool bSecretDungeonFree, bool bAvatar, bool bTitle ) ]]--
GameSysVal:AddPCBang( 0, 0.05, 0.05, 0.05, False, 0.0, 0, False, True, True )
GameSysVal:AddPCBang( 1, 0.1, 0.1, 0.1, False, 0.0, 0, False ,True ,True )

--제한 레벨.
GameSysVal:AddLimitsMaxLevel( 70 )

--프리미업 캐쉬아이템 경험치 숫치 : 축복의 메달
GameSysVal:AddPremiumEXP( 0.3 )

-- #ifdef SERV_VIP_SYSTEM VIP 캐쉬 아이템 경험치 숫지 :VIP 티캣
GameSysVal:AddVIPEXP( 0.05 )

--체험ID 제한 레벨
GameSysVal:SetGuestUserLimitLevel( 19 )

-- 핵 유저 발견시 접속 차단 유저 등록 처리
GameSysVal:SetIsHackUserRegRejected( False )	-- SERV_GLOBAL_COMMON 해외팀은 제재 우선 막음

-- 핵 유저 발견시 감시 대상 유저 등록 처리
GameSysVal:SetIsHackUserRegMonitors( True )

-- 핵 유저 발견시 강제 접속 종료 처리
GameSysVal:SetIsHackUserKick( False )			-- SERV_GLOBAL_COMMON 해외팀은 제재 우선 막음

-- 핵 유저 발견시 거래 정지 처리
GameSysVal:SetIsHackUserBlockTrade( False )		-- SERV_GLOBAL_COMMON 해외팀은 제재 우선 막음

-- 핵 유저 패킷 모니터링
GameSysVal:SetHackUserPacketMornitor( True )

--#ifdef SERV_NEW_UNIT_TRADE_LIMIT
GameSysVal:SetIsNewUnitTradeBlock( True )		-- 신규 유닛 거래 제한 유무
GameSysVal:SetNewUnitTradeBlockDay( 3 )			-- 신규 유닛 거래 제한 날짜
GameSysVal:SetNewUnitTradeBlockUnitClass( 10 )	-- 신규 유닛 거래 제한 전직 ( 전직해야만 거래 가능하면 10 으로 변경 )
--#endif SERV_NEW_UNIT_TRADE_LIMIT

--#ifdef SERV_DESAGREE_HACK_USER_ON_OFF
GameSysVal:SetDisagreeHackUserFunc( False )	-- 동의 안한 핵 유저에 관한 처리 유무
--#endif SERV_DESAGREE_HACK_USER_ON_OFF

--#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
-- 랜선렉
GameSysVal:SetLanBugOutJustLog( True )	-- 추방은 하지 않고 로그만 남깁니다.

--GameSysVal:SetLanBugOutDisconnectCheckStartOnGameLoadingReq( False )		-- EGS_GAME_LOADING_REQ 받고 성공적으로 처리가 되면 검사 시작
--GameSysVal:SetLanBugOutDisconnectCheckTerm( 10.0 )		-- Out 패킷을 날리고 블럭 판단을 내릴 때까지의 시간 간격 ( 음수일 경우 사용하지 않음 )
--GameSysVal:SetLanBugOutDisconnectTerm( 10, 50 )	-- 블럭 판단 이후 강제로 접속을 끊어버리기 까지의 시간 간격 ( 최소 + 랜덤 텀 )

-- 랜선렉 판별 기준 ( 패킷 전송 텀은 소수점 아래 한 자리 까지만 표현 )
GameSysVal:SetLanBugOutCheckOption
{
	bLanBugOutCheck				= False,		-- 사용 여부
	byteLanBugOutCheckMaxCount	= 0,		-- 최대 체크 횟수 ( 0 이면 무제한 )
	fLanBugOutCheckTime			= 1.0,		-- 판정 시간
	byteLanBugOutCheckCount		= 10,		-- 판정 횟수
	fLanBugOutCheckTermMin		= 1.0,		-- 판정 패킷 최소 간격
	fLanBugOutCheckTerm			= 0.0,		-- 판정 패킷 최대 텀 ( 최소 간격 + @ ) ( 소수점 아래 한 자리 까지만 표현 )
	fLanBugOutCheckRepeatTerm	= 0.5,		-- 반복 전송 간격
}

-- 랜선렉 악용 유저 판별 기준 ( 패킷 전송 텀은 소수점 아래 한 자리 까지만 표현 )
GameSysVal:SetLanBugOutVerifyOption
{
	bLanBugOutVerify			= False,		-- 사용 여부
	byteLanBugOutVerifyMaxCount	= 10,		-- 최대 체크 횟수 ( 0 이면 무제한 )
	fLanBugOutVerifyTime		= 1.0,		-- 판정 시간
	byteLanBugOutVerifyCount	= 10,		-- 판정 횟수
	fLanBugOutVerifyTermMin		= 1.0,		-- 판정 패킷 최소 간격
	fLanBugOutVerifyTerm		= 0.0,		-- 판정 패킷 최대 텀 ( 최소 간격 + @ ) ( 소수점 아래 한 자리 까지만 표현 )
	fLanBugOutVerifyRepeatTerm	= 0.5,		-- 반복 전송 간격
}
--#endif SERV_FIX_SYNC_PACKET_USING_RELAY

-- 빙고 보상 아이템 지급 실패 시 재지급 시도 횟수
--GameSysVal:SetBingoEventPayoutRepeateUse( False )
--GameSysVal:SetBingoEventPayoutRepeateUse2( False )
--GameSysVal:SetBingoEventPayoutRepeate( 1 )

-- 호스트 변경 시간(핑 체크)
GameSysVal:SetCheckChangeHostTime( 180 ) -- 방장 변경 검사 시간(6분 마다 변경 조건 검사)

-- SMS 설정

-- 우편 및 거래 감시
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_LETTER_AND_TRADE"], "010-4118-3867" )	-- 김창호 팀장님
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_LETTER_AND_TRADE"], "010-8584-3123" )	-- 권오당 팀장님
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_LETTER_AND_TRADE"], "010-9317-0790" )	-- 황정희 님

--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_LETTER_AND_TRADE"], "010-4072-1031" )	-- 김결
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_LETTER_AND_TRADE"], "010-3898-7886" )	-- 김강민
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_LETTER_AND_TRADE"], "010-5671-0502" )	-- 이우진

-- 실시간 동접 하락 체크 sms
--#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-4118-3867" )			-- 김창호 팀장님
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-8584-3123" )			-- 권오당 팀장님
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-7466-7958" )			-- 우원식
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-8421-1075" )			-- 최육사
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-5598-4163" )			-- 김민성
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-9490-8761" )			-- 황원준
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-8982-3382" )			-- 박창용
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-6226-2188" )			-- 박성호

--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-4072-1031" )			-- 김결
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-3898-7886" )			-- 김강민 
--GameSysVal:AddSMSRecipient( SMS_TYPE["SMS_SURVEILLANCE_THE_CCU_DROP"], "010-5671-0502" )			-- 이우진
-- #endif SERV_RUNTIME_SCRIPT_FOR_SMS

--광고 URL
GameSysVal:SetAdvertisementURL( 'http://ad.nexon.com/NetInsight/html/nexon/elsword.nexon.com/ending@middle' )

-- 캐릭터 통계 남기는 시작 날짜 세팅
GameSysVal:SetCharStatisticsStartDate( '2011-01-20 08:30:00' )

-- 계정 통계 남기는 시작 날짜 세팅
GameSysVal:SetAccountStatisticsStartDate( '2011-02-24 08:00:00' )

-- 개인거래 차단
GameSysVal:SetBlockPersonalTrade( False )
GameSysVal:SetBlockPersonalShop( False )
GameSysVal:SetBlockSendLetter( False )

-- 머신ID 블럭 기능
GameSysVal:SetMachineIDBlock( True )

-- 머신ID 중복 접속 체크 기능
GameSysVal:SetMachineIDDuplicateCheck( False )

-- 휴면 복귀 유저 보상 기준( 단계, 몇일 )
GameSysVal:SetComeBackReward( 1, 30 )	-- 1단계  30일
GameSysVal:SetComeBackReward( 2, 60 )	-- 2단계  60일
GameSysVal:SetComeBackReward( 3, 180 )	-- 3단계 180일


-- 개인상점 거래 감시
--GameSysVal:InsertItemPrice( 91620, 200000 )			-- 마법의 얼음 조각
--GameSysVal:InsertItemPrice( 77010, 200000 )			-- 초급 마나포션

--/////////////////////////////////////////////////////////////////////////////////////////////////////////
-- 우편 전송 횟수 감시 기능-- ED 거래 횟수 감시 기능
GameSysVal:SetLetterAndTradeMonitoringIsOn(true)

GameSysVal:SetLimitsCountOfLetter( 10 )
GameSysVal:SetLetterMonitoringCheckTime( 20 ) -- 분

GameSysVal:SetLimitsCountOfTrade( 10 )
GameSysVal:SetTradeMonitoringCheckTime( 20 ) -- 분

GameSysVal:SetMonitoringMinED( 400000 )				-- 감시 최소 금액
GameSysVal:SetMonitoringMaxED( 7000000 )			-- 감시 최대 금액

GameSysVal:SetMonitoringLetterAndTradeBlockTime( 5, 0, 0, 10800 )			-- 자동 블럭 시간(시작 시간, , , 유지시간)
--//////////////////////////////////////////////////////////////////////////////////////////////////////


-- Deserialize Fail Check 해킹 체크
GameSysVal:SetDeserializeFailCheck( True )
GameSysVal:SetDeserializeFailCheckCount( 5 )

-- 서브 스테이지 DIE NPC 시간 체크 기능
GameSysVal:SetSubStageNpcDieCheck( False )

-- 던전 클리어 시 현자의 주문서/마법석 지급 이벤트(2012-07-14)
--GameSysVal:SetDungeonClearStartTime( 2012, 7, 14, 14, 0, 0 )
--GameSysVal:SetDungeonClearEndTime( 2012, 7, 14, 15, 0, 0 )

-- 특정 시간 이벤트 큐브 오픈가능 조건
--GameSysVal:SetEventCubeOpenStartTime(  2012, 7, 15, 12, 0, 0)
--GameSysVal:SetEventCubeOpenEndTime( 2012, 7, 15, 12, 10, 0 )

--특정시간 가열기 확률 변경
--GameSysVal:SetAttractionItemTimeEvent( 108900, 160578, { wstrBeginDate = '2013-05-18 00:00:00',	wstrEndDate = '2013-05-19 23:59:00' } )
--GameSysVal:SetAttractionItemTimeEvent( 108900, 160578, { wstrBeginDate = '2013-05-25 00:00:00',	wstrEndDate = '2013-05-26 23:59:00' } )
--GameSysVal:SetAttractionItemTimeEvent( 108900, 160578, { wstrBeginDate = '2013-06-01 00:00:00',	wstrEndDate = '2013-06-02 23:59:00' } )
--GameSysVal:SetAttractionItemTimeEvent( 108900, 160578, { wstrBeginDate = '2013-06-08 00:00:00',	wstrEndDate = '2013-06-09 23:59:00' } )

-- ServerSN Check --
-- #ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
--GameSysVal:SetCheckServerSN_IN_GS( True )
--GameSysVal:SetCheckServerSN_ERR_01_1_IN_GS( false )
--GameSysVal:SetCheckServerSN_ERR_01_2_IN_GS( false )
--GameSysVal:SetCheckServerSN_ERR_02_IN_GS( false )
-- #endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER

-- 플루오르 강화 이벤트(2013-02-16)
--GameSysVal:SetEnchantEventStartTime( 2013, 2, 16, 0, 0, 0 )
--GameSysVal:SetEnchantEventEndTime( 2013, 2, 16, 23, 59, 0 )


-- 남캐 버프 이벤트 - 발렌타인 데이 이벤트
-- GameSysVal:SetMenBuffEvent( true )

-- 실시간 동접 하락 체크 sms
-- #ifdef SERV_CHECK_DROP_CCU
--GameSysVal:SetCheckDropCCU( true )
--GameSysVal:SetDropCCUPercent( 50 ) -- 50% 이상 하락하였으면 sms 발송
-- #endif SERV_CHECK_DROP_CCU

-- 해킹 유저 응답 패킷 이상 시 킥 여부 // 2012년 6월 07일 적용 (문제 발생 시 false)
GameSysVal:SetHackingUserKick( true )

-- 랜덤 큐브 오픈 딜레이 시스템
GameSysVal:SetEnableOpenRandomCubeDelaySystem( false )
GameSysVal:SetOpenRandomCubeDelayTime( 3 )
GameSysVal:SetOpenRandomCubeDelayCount( 10 )

-- 2012 대전 시즌2 전야 런칭 이벤트 시간(매일)
-- 1:토요일/일요일, 2:월~금
-- ( 주말/평일, 순번, 시작 시, 시작 분, 끝 시, 끝 분 )
--GameSysVal:SetPvpEventTime( 1, 1, 12, 0, 13, 0 )
--GameSysVal:SetPvpEventTime( 1, 2, 21, 0, 22, 0 )
--GameSysVal:SetPvpEventTime( 2, 1, 18, 30, 19, 30 )
--GameSysVal:SetPvpEventTime( 2, 2, 21, 0, 22, 0 )

-- 로컬 랭킹 시스템 랭커 인원 수 설정
-- #ifdef SERV_LOCAL_RANKING_SYSTEM
--GameSysVal:SetLocalRankingNumOfRanker( 1000 )
--GameSysVal:SetLocalRankingElpUpdateGap( 0.2 )
-- #endif SERV_LOCAL_RANKING_SYSTEM

-- ERM_NPC_UNIT_DIE_REQ 패킷 호스트 체크 텀 ( 0 이하의 값을 기록하면 체크하지 않는다. )
GameSysVal:SetHostCheckTerm( 3.0 )

-- 같은 스테이지 로딩 시 남길 로그
--GameSysVal:SetDungeonStageLoadLog( True )

--#ifdef SERV_PROCESS_COMMUNICATION_KSMS
GameSysVal:SetProcessCommunication( TRUE )
--#endif //SERV_PROCESS_COMMUNICATION_KSMS

-----------------------------------------------------------------------------

--PC방 프리미엄 아이템
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ELSWORD"], 131254 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ELSWORD"], 131429 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ELSWORD"], 131430 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ELSWORD"], 131431 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ELSWORD"], 131432 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ELSWORD"], 131433 )

GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARME"], 131255 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARME"], 131434 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARME"], 131435 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARME"], 131436 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARME"], 131437 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARME"], 131438 )

GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_LIRE"], 131256 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_LIRE"], 131439 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_LIRE"], 131440 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_LIRE"], 131441 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_LIRE"], 131442 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_LIRE"], 131443 )

GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_RAVEN"], 131257 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_RAVEN"], 131444 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_RAVEN"], 131445 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_RAVEN"], 131446 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_RAVEN"], 131447 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_RAVEN"], 131448 )

GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_EVE"], 131258 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_EVE"], 131449 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_EVE"], 131450 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_EVE"], 131451 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_EVE"], 131452 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_EVE"], 131453 )

GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_CHUNG"], 86550 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_CHUNG"], 86551 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_CHUNG"], 86552 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_CHUNG"], 86553 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_CHUNG"], 86554 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_CHUNG"], 86555 )

GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARA"], 86560 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARA"], 86561 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARA"], 86562 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARA"], 86563 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARA"], 86564 )
GameSysVal:AddPcBangPremiumItem( UNIT_TYPE["UT_ARA"], 86565 )




--[[
-- pc방 이벤트 아바타 (2013-03-28 ~ 2013-04-18)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ELSWORD"],	86570	)  --	글레이셜 프린스 크리스탈 소드(엘소드)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ELSWORD"],	86571	)  --	글레이셜 프린스 상의(엘소드)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ELSWORD"],	86572	)  --	글레이셜 프린스 하의(엘소드)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ELSWORD"],	86573	)  --	글레이셜 프린스 장갑(엘소드)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ELSWORD"],	86574	)  --	글레이셜 프린스 신발(엘소드)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ELSWORD"],	86575	)  --	글레이셜 프린스 헤어(엘소드)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ELSWORD"],	86576	)  --	글레이셜 프린스 신발(엘소드)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ELSWORD"],	86577	)  --	글레이셜 프린스 헤어(엘소드)

GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARME"],		86578	)  --	글레이셜 프린세스 크리스탈 스태프(아이샤)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARME"],		86579	)  --	글레이셜 프린세스 상의(아이샤)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARME"],		86580	)  --	글레이셜 프린세스 하의(아이샤)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARME"],		86581	)  --	글레이셜 프린세스 장갑(아이샤)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARME"],		86582	)  --	글레이셜 프린세스 신발(아이샤)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARME"],		86583	)  --	글레이셜 프린세스 헤어(아이샤)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARME"],		86584	)  --	글레이셜 프린세스 신발(아이샤)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARME"],		86585	)  --	글레이셜 프린세스 헤어(아이샤)

GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_LIRE"],		86586	)  --	글레이셜 프린세스 크리스탈 보우(레나)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_LIRE"],		86587	)  --	글레이셜 프린세스 상의(레나)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_LIRE"],		86588	)  --	글레이셜 프린세스 하의(레나)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_LIRE"],		86589	)  --	글레이셜 프린세스 장갑(레나)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_LIRE"],		86590	)  --	글레이셜 프린세스 신발(레나)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_LIRE"],		86591	)  --	글레이셜 프린세스 헤어(레나)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_LIRE"],		86592	)  --	글레이셜 프린세스 신발(레나)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_LIRE"],		86593	)  --	글레이셜 프린세스 헤어(레나)

GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_RAVEN"],		86594	)  --	글레이셜 프린스 크리스탈 블레이드(레이븐)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_RAVEN"],		86595	)  --	글레이셜 프린스 상의(레이븐)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_RAVEN"],		86596	)  --	글레이셜 프린스 하의(레이븐)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_RAVEN"],		86597	)  --	글레이셜 프린스 장갑(레이븐)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_RAVEN"],		86598	)  --	글레이셜 프린스 신발(레이븐)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_RAVEN"],		86599	)  --	글레이셜 프린스 헤어(레이븐)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_RAVEN"],		86600	)  --	글레이셜 프린스 신발(레이븐)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_RAVEN"],		86601	)  --	글레이셜 프린스 헤어(레이븐)

GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_EVE"],		86602	)  --	글레이셜 프린세스 크리스탈 기어(이브)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_EVE"],		86603	)  --	글레이셜 프린세스 상의(이브)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_EVE"],		86604	)  --	글레이셜 프린세스 하의(이브)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_EVE"],		86605	)  --	글레이셜 프린세스 장갑(이브)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_EVE"],		86606	)  --	글레이셜 프린세스 신발(이브)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_EVE"],		86607	)  --	글레이셜 프린세스 헤어(이브)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_EVE"],		86608	)  --	글레이셜 프린세스 신발(이브)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_EVE"],		86609	)  --	글레이셜 프린세스 헤어(이브)

GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_CHUNG"],		86610	)  --	글레이셜 프린스 크리스탈 캐논(청)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_CHUNG"],		86611	)  --	글레이셜 프린스 상의(청)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_CHUNG"],		86612	)  --	글레이셜 프린스 하의(청)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_CHUNG"],		86613	)  --	글레이셜 프린스 장갑(청)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_CHUNG"],		86614	)  --	글레이셜 프린스 신발(청)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_CHUNG"],		86615	)  --	글레이셜 프린스 헤어(청)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_CHUNG"],		86616	)  --	글레이셜 프린스 신발(청)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_CHUNG"],		86617	)  --	글레이셜 프린스 헤어(청)

GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARA"],		86618	)  --	글레이셜 프린세스 크리스탈 스피어(아라)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARA"],		86619	)  --	글레이셜 프린세스 상의(아라)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARA"],		86620	)  --	글레이셜 프린세스 하의(아라)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARA"],		86621	)  --	글레이셜 프린세스 장갑(아라)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARA"],		86622	)  --	글레이셜 프린세스 신발(아라)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARA"],		86623	)  --	글레이셜 프린세스 헤어(아라)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARA"],		86624	)  --	글레이셜 프린세스 신발(아라)
GameSysVal:AddPcBangPremiumItem(	UNIT_TYPE["UT_ARA"],		86625	)  --	글레이셜 프린세스 헤어(아라)
--]]

-- #ifdef SERV_PVP_EQUIPMENT
--[[
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ELSWORD"],	140000010 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ELSWORD"],	140000011 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ELSWORD"],	140000012 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ELSWORD"],	140000013 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ELSWORD"],	140000014 )

GameSysVal:AddPvpItem( UNIT_TYPE["UT_LIRE"],	140000020 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_LIRE"],	140000021 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_LIRE"],	140000022 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_LIRE"],	140000023 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_LIRE"],	140000024 )

GameSysVal:AddPvpItem( UNIT_TYPE["UT_ARME"],	140000030 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ARME"],	140000031 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ARME"],	140000032 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ARME"],	140000033 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_ARME"],	140000034 )

GameSysVal:AddPvpItem( UNIT_TYPE["UT_RAVEN"],	141000143 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_RAVEN"],	141000144 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_RAVEN"],	141000145 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_RAVEN"],	141000146 )
GameSysVal:AddPvpItem( UNIT_TYPE["UT_RAVEN"],	141000147 )
--]]
-- #endif SERV_PVP_EQUIPMENT