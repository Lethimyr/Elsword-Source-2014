-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Login Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "kknd345@kog.co.kr, bowrain@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 넥슨 passport 인증 레벨
--Server:SetPassportLevel( 2 )

-- 스크립트 파싱 실패 리포트
Server:SetScriptParsingErrReport( true )
Server:AddScriptParsingErrReportMail( 'kknd345@kog.co.kr' )	-- 조효진
Server:AddScriptParsingErrReportMail( 'bowrain@kog.co.kr' ) -- 임규수

-- Add moritoring server's IP for checking port of elsword's server ( SERV_PERMIT_PORT_CHECK )
--Server:AddPortCheckMoritoringServer( "79.110.90.240" )

----------     Login NetLayer 설정     ----------

NetLayer:SetCheckSequenceNum( false ) -- 내부통신은 SequenceNum 체크필요없음


----------     Login SimLayer 설정     ----------
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
SimLayer:SetPublisherCheckGameServerLogin( false )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )

-- Billing Flag --
--[[
BF_INTERNAL : 사내 버전 빌링
BF_NEXON_KOREA_TEST : 넥슨 빌링 테섭
BF_NEXON_KOREA : 넥슨 빌링 본섭
--]]
SimLayer:SetBillingFlag( BF_GLOBAL_SERVICE )

-- Script File Path --
SimLayer:AddPath( "D:\\ELS_Server\\ServerResource" )					-- server script
SimLayer:AddPath( "D:\\ELS_Server\\Script" )				-- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_WEB		   : 넥슨 WEB
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

if Server:GetServerGroupID() == 1 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_ZR01.dsn',			20, false )
	DBLayer:AddDB( DC_GAME,		'Game_CN_ZR01.dsn',				15, false )
	DBLayer:AddDB( DC_LOG, 		'Log_CN_ZR01.dsn', 				5, false )
	DBLayer:AddDB( DC_KOG_BILLING,	'Billing_CN_ZR01.dsn',			5, false )
elseif Server:GetServerGroupID() == 2 then
DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_ZR02.dsn',			20, false )
	DBLayer:AddDB( DC_GAME,		'Game_CN_ZR02.dsn',				15, false )
	DBLayer:AddDB( DC_LOG, 		'Log_CN_ZR02.dsn', 				5, false )
	DBLayer:AddDB( DC_KOG_BILLING,	'Billing_CN_ZR02.dsn',			5, false )
elseif Server:GetServerGroupID() == 3 then
DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_ZR03.dsn',			20, false )
	DBLayer:AddDB( DC_GAME,		'Game_CN_ZR03.dsn',				15, false )
	DBLayer:AddDB( DC_LOG, 		'Log_CN_ZR03.dsn', 				5, false )
	DBLayer:AddDB( DC_KOG_BILLING,	'Billing_CN_ZR03.dsn',			5, false )
elseif Server:GetServerGroupID() == 4 then
	DBLayer:AddDB( DC_ACCOUNT,		'Account_CN_WH01.dsn',		20, false )
	DBLayer:AddDB( DC_GAME,			'Game_CN_WH01.dsn',			15, false )
	DBLayer:AddDB( DC_LOG, 			'Log_CN_WH01.dsn', 			5, false )
	DBLayer:AddDB( DC_KOG_BILLING,		'Billing_CN_WH01.dsn',		5, false )
elseif Server:GetServerGroupID() == 5 then
	DBLayer:AddDB( DC_ACCOUNT,		'Account_CN_WH02.dsn',		20, false )
	DBLayer:AddDB( DC_GAME,			'Game_CN_WH02.dsn',			15, false )
	DBLayer:AddDB( DC_LOG, 			'Log_CN_WH02.dsn', 			5, false )
	DBLayer:AddDB( DC_KOG_BILLING,		'Billing_CN_WH02.dsn',		5, false )
elseif Server:GetServerGroupID() == 6 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_ZR01.dsn',			20, false )
	DBLayer:AddDB( DC_GAME,		'Game_CN_ZR06.dsn',				15, false )
	DBLayer:AddDB( DC_LOG, 		'Log_CN_ZR01.dsn', 				5, false )
	DBLayer:AddDB( DC_KOG_BILLING,	'Billing_CN_ZR01.dsn',			5, false )
elseif Server:GetServerGroupID() == 101 then
DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_TJ01.dsn',			20, false )
	DBLayer:AddDB( DC_GAME,		'Game_CN_TJ01.dsn',				15, false )
	DBLayer:AddDB( DC_LOG, 		'Log_CN_TJ01.dsn', 				5, false )
	DBLayer:AddDB( DC_KOG_BILLING,	'Billing_CN_TJ01.dsn',			5, false )
elseif Server:GetServerGroupID() == 102 then
	DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_TJ02.dsn',			20, false )
	DBLayer:AddDB( DC_GAME,		'Game_CN_TJ02.dsn',				15, false )
	DBLayer:AddDB( DC_LOG, 		'Log_CN_TJ02.dsn', 				5, false )
	DBLayer:AddDB( DC_KOG_BILLING,	'Billing_CN_TJ02.dsn',			5, false )
end
----------     룸 설정     ----------
-- 좀비유저 체크 시간 설정 --
RoomManager:SetZUTickTime( 60 )


----------     인증 설정     ----------

if SimLayer:GetAuthFlag() == AF_GLOBAL_SERVICE
then

if Server:GetServerGroupID() == 1 then
	GiantAuthManager:SetAddress( '192.168.204.192', 9802 ) --남방 1
	GiantAuthManager:SetNumThread( 15 )
elseif Server:GetServerGroupID() == 4 then
	GiantAuthManager:SetAddress( '101.226.182.5', 9802 ) --남방 ( 우한1 군)
	GiantAuthManager:SetNumThread( 15 )
elseif Server:GetServerGroupID() == 6 then
	GiantAuthManager:SetAddress( '192.168.204.192', 9802 ) --남방 2
	GiantAuthManager:SetNumThread( 15 )
elseif Server:GetServerGroupID() == 101 then
	GiantAuthManager:SetAddress( '125.39.176.20', 9802 ) --북방
	GiantAuthManager:SetNumThread( 15 )
end

	GiantRoleRegManager:SetAddress( '222.73.243.246', 9902 )
	GiantRoleRegManager:SetNumThread( 15 )
	
	GiantInfoManager:SetAddress( '222.73.243.246', 29903 )
	GiantInfoManager:SetNumThread( 15 )
end

if SimLayer:GetBillingFlag() == BF_GLOBAL_SERVICE
then

if Server:GetServerGroupID() == 101 then
	GiantBillingManager:SetAddress( '112.65.197.30', 19968 )
else
	GiantBillingManager:SetAddress( '222.73.225.229', 19968 )
end
	
	
	
	GiantBillingManager:SetNumThread( 15 )
	--GiantBillingManager:SetHeartBitInterval( 30 * 1000 )
	--GiantBillingManager:SetConnectionCheckInterval( 1* 1000 )
end



----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 0 )