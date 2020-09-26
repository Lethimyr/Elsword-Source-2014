----------     Channel Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "kknd345@kog.co.kr, bowrain@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 스크립트 파싱 실패 리포트
Server:SetScriptParsingErrReport( true )
Server:AddScriptParsingErrReportMail( 'kknd345@kog.co.kr' )	-- 조효진
Server:AddScriptParsingErrReportMail( 'bowrain@kog.co.kr' ) -- 임규수

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
SimLayer:SetPublisherBillingConnectType (PBCT_UNUSUAL)
SimLayer:SetCheckCouponByPublisher ( false )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )

-- IP Check --
--SimLayer:SetCheckIPMode( CIM_SERVICE_MODE )		-- 서비스 모드
SimLayer:SetCheckIPMode( CIM_CHECK_MODE )		-- 점검모드

-- 점검 모드 IP 체크
-- ZTGAME
SimLayer:AddCheckModeIP( "180.168.126" )
SimLayer:AddCheckModeIP( "210.13.74" )
SimLayer:AddCheckModeIP( "218.80.198" )
SimLayer:AddCheckModeIP( "222.66.162" )
SimLayer:AddCheckModeIP( "222.73.33" )

-- KOG
SimLayer:AddCheckModeIP( "14.45.79" )
SimLayer:AddCheckModeIP( "211.234.69" )
SimLayer:AddCheckModeIP( "218.54.221" )
SimLayer:AddCheckModeIP( "121.181.220" )


-- INCA
SimLayer:AddCheckModeIP( "210.217.175.248" )
SimLayer:AddCheckModeIP( "61.96.192.27" )
SimLayer:SetCheckModeCheckIPType( CIT_CHECK_IP_ALLOW )

-- 서비스 모드 IP 체크
SimLayer:AddIP( "192.168.66" )
SimLayer:SetCheckIPType( CIT_ALL_IP_ALLOW )


-- Script File Path --
SimLayer:AddPath( "D:\\ELS_Server\\ServerResource" )					-- server script
SimLayer:AddPath( "D:\\ELS_Server\\Script" )				-- client script



----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_SHOP_GATEWAY : 넥슨 빌링 게이트웨이
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

if Server:GetServerGroupID() == 1 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_ZR01.dsn',			5, false )
	DBLayer:AddDB( DC_LOG, 			'Log_CN_ZR01.dsn', 				5, false )
elseif Server:GetServerGroupID() == 2 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_ZR02.dsn',			5, false )
	DBLayer:AddDB( DC_LOG, 			'Log_CN_ZR02.dsn', 				5, false )
elseif Server:GetServerGroupID() == 3 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_ZR03.dsn',			5, false )
	DBLayer:AddDB( DC_LOG, 			'Log_CN_ZR03.dsn', 				5, false )
elseif Server:GetServerGroupID() == 4 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_WH01.dsn',					5, false )
	DBLayer:AddDB( DC_LOG,			'Log_CN_WH01.dsn',							5, false )
elseif Server:GetServerGroupID() == 5 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_WH02.dsn',					5, false )
	DBLayer:AddDB( DC_LOG,			'Log_CN_WH02.dsn',							5, false )
elseif Server:GetServerGroupID() == 6 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_ZR01.dsn',			5, false )
	DBLayer:AddDB( DC_LOG, 			'Log_CN_ZR01.dsn', 				5, false )
elseif Server:GetServerGroupID() == 101 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_TJ01.dsn',			5, false )
	DBLayer:AddDB( DC_LOG, 			'Log_CN_TJ01.dsn', 				5, false )
elseif Server:GetServerGroupID() == 102 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_TJ02.dsn',			5, false )
	DBLayer:AddDB( DC_LOG, 			'Log_CN_TJ02.dsn', 				5, false )

end

----------     인증 설정     ----------



----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 0 )