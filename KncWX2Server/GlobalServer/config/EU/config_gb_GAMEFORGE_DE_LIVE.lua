-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Global Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "honnak@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )


-- 스크립트 파싱 실패 리포트
Server:SetScriptParsingErrReport( true )
Server:AddScriptParsingErrReportMail( 'honnak@kog.co.kr' )	-- 임홍락
Server:AddScriptParsingErrReportMail( '86red@kog.co.kr' )	-- 김석근
Server:AddScriptParsingErrReportMail( 'aaron@kog.co.kr' )	-- 오세윤
Server:AddScriptParsingErrReportMail( 'wonjin@kog.co.kr' )	-- 김원진

----------     Global NetLayer 설정     ----------

NetLayer:SetCheckSequenceNum( false ) -- 내부통신은 SequenceNum 체크필요없음



----------     ShutDown 미리 알림 설정     ----------
--Server:AddMSGBeforShutDown( 23, 30, 0 )			-- 매일 23시 30분 00초
--Server:AddMSGBeforShutDown( 23, 50, 0 )			-- 매일 23시 50분 00초
--Server:AddMSGBeforShutDown( 23, 55, 0 )			-- 매일 23시 55분 00초



----------     Global SimLayer 설정     ----------
SimLayer:SetUseKogOTP ( true )

-- PublisherBillingConnectType Flag --
--[[
		PBCT_NONE				= 0,	
		// kncSend 떄문에 DBLayer와 일반 Manager 는 구분되어야 함
		PBCT_NORMAL		= 1,	// 일반적인 Manager 통해 (보통은 TCP, SOAP 등)
		PBCT_DB					= 2,	// DB 통신 Manager를 통해
		PBCT_UNUSUAL		= 3,	// 섞어 쓰던가 특이 사항 있는 경우
--]]
SimLayer:SetPublisherBillingConnectType( PBCT_NORMAL )
SimLayer:SetCheckCouponByPublisher( false )

-- Nation Flag --
--[[
NF_DE : 독일 스트링 필터
NF_FR : 프랑스 스트링 필터
NF_IT : 이탈리아 스트링 필터
NF_PL : 폴란드 스트링 필터
NF_ES : 스페인 스트링 필터
NF_TR : 터키 스트링 필터
NF_EN : 영국 스트링 필터
--]]
SimLayer:SetNationFlag( NF_DE )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )

-- Script File Path --
--SimLayer:AddPath( "D:\\Elsword\\1_0_0_1\\Common" ) -- server common
--SimLayer:AddPath( "D:\\Elsword\\1_0_0_1\\ClientScript" ) -- client script
SimLayer:AddPath( "D:\\Elsword\\Server\\ServerResource" ) -- server common
SimLayer:AddPath( "D:\\Elsword\\Server\\ClientScript" ) -- client script

----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_WEB		   : 넥슨 WEB
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,	'Account_GAMEFORGE_DE_LIVE.dsn',	1, false )	
DBLayer:AddDB( DC_LOG,	'Log_GAMEFORGE_DE_LIVE.dsn',	3, false )	

--DBLayer:AddDB( DC_SMS,		'SMS.dsn',				1, false )


----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 0 )