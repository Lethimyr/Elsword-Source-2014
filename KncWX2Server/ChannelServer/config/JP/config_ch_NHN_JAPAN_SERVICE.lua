-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Channel Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "86red@kog.co.kr, bowrain@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 스크립트 파싱 실패 리포트
--Server:SetScriptParsingErrReport( true )
--Server:AddScriptParsingErrReportMail( '86red@kog.co.kr' )	-- 김석근
--Server:AddScriptParsingErrReportMail( 'bowrain@kog.co.kr' ) -- 임규수

-- Add moritoring server's IP for checking port of elsword's server ( SERV_PERMIT_PORT_CHECK )
--Server:AddPortCheckMoritoringServer( "79.110.90.240" )

----------     Channel NetLayer 설정     ----------

NetLayer:SetCheckSequenceNum( true )
NetLayer:SetPacketAuthFailLimit( 100 ) -- 유저가 100개이상 비정상 패킷을 보내면 해당 유저 접속해제


----------     Channel SimLayer 설정     ----------
SimLayer:SetUseKogOTP ( true )

-- PublisherBillingConnectType Flag --
--[[
		PBCT_NONE				= 0,	
		// kncSend 떄문에 DBLayer와 일반 Manager 는 구분되어야 함
		PBCT_NORMAL		= 1,	// 일반적인 Manager 통해 (보통은 TCP, SOAP 등)
		PBCT_DB					= 2,	// DB 통신 Manager를 통해
		PBCT_UNUSUAL		= 3,	// 섞어 쓰던가 특이 사항 있는 경우
--]]
SimLayer:SetPublisherBillingConnectType (PBCT_NORMAL)
SimLayer:SetCheckCouponByPublisher ( false )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
AF_GLOBAL_SERVICE : 퍼블 인증
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )

-- IP Check --
SimLayer:SetCheckIPMode( CIM_SERVICE_MODE )		-- 서비스 모드
--SimLayer:SetCheckIPMode( CIM_CHECK_MODE )		-- 점검모드

-- 점검 모드 IP 체크
SimLayer:AddCheckModeIP( "192.168.66" )
SimLayer:SetCheckModeCheckIPType( CIT_CHECK_IP_BLOCK )

-- 서비스 모드 IP 체크
SimLayer:AddIP( "192.168.66" )
SimLayer:SetCheckIPType( CIT_ALL_IP_ALLOW )


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

--DBLayer:AddDB( DC_ACCOUNT, 'Account_NHN_JAPAN_INTERNAL.dsn', 3, false )
--DBLayer:AddDB( DC_LOG, 'Log_NHN_JAPAN_INTERNAL.dsn', 3, false )
DBLayer:AddDB( DC_ACCOUNT,		'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=Account;WSID=PBSWDDM1007;SERVER=10.32.25.5',		3, true )
DBLayer:AddDB( DC_LOG,			'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=Statistics;WSID=PBSWDDM1006;SERVER=10.32.25.3',	3, true )


-- Script File Path --
SimLayer:AddPath( "D:\\Server\\bin\\ServerResource" ) -- server common
SimLayer:AddPath( "D:\\Server\\bin\\ClientScript" ) -- client script


----------     인증 설정     ----------

--if SimLayer:GetAuthFlag() == AF_GLOBAL_SERVICE
--then
	--GASHAuthManager:InitGASHAuthValue( '300148', 'AD' )
	--GASHAuthManager:InitGASHAuthAddress( '192.168.211.199', 5411, 5412, 5413 )
	PurpleAuthManager:SetNumThread( 10 )
--end
-- Han Auth/Billing Init Flag --
--[[
HIF_ALPHA		: ALPHA (新Test環境)
HIF_REAL		: REAL　(昔Test環境、Real環境)
--]]
SimLayer:SetHanInitFlag( HIF_REAL )

----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 0 )