-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Login Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "ninil@kog.co.kr, bowrain@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 넥슨 passport 인증 레벨
--Server:SetPassportLevel( 2 )

-- 스크립트 파싱 실패 리포트
Server:SetScriptParsingErrReport( true )
Server:AddScriptParsingErrReportMail( 'ninil@kog.co.kr' )	-- 조효진
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
SimLayer:AddPath( "D:\\ELS_Server\\CN_BETA\\ServerResource" ) -- server script
SimLayer:AddPath( "D:\\ELS_Server\\CN_BETA\\Script" )			-- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_WEB		   : 넥슨 WEB
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,	'Account_CN_TEST.dsn',		3, false )
DBLayer:AddDB( DC_GAME,		'Game01_CN_TEST.dsn',			3, false )
DBLayer:AddDB( DC_LOG, 		'Log_CN_TEST.dsn', 			3, false )
DBLayer:AddDB( DC_KOG_BILLING,	'Billing_CN_TEST.dsn',		3, false )

----------     룸 설정     ----------
-- 좀비유저 체크 시간 설정 --
RoomManager:SetZUTickTime( 60 )


----------     인증 설정     ----------

if SimLayer:GetAuthFlag() == AF_GLOBAL_SERVICE
then

if Server:GetServerGroupID() == 0 then
	GiantAuthManager:SetAddress( '222.73.243.200', 9802 )
	GiantAuthManager:SetNumThread( 1 )
end	
	GiantRoleRegManager:SetAddress( '222.73.243.200', 39901 )
	GiantRoleRegManager:SetNumThread( 1 )
	
	GiantInfoManager:SetAddress( '222.73.243.200', 9903 )
	GiantInfoManager:SetNumThread( 1 )
end

if SimLayer:GetBillingFlag() == BF_GLOBAL_SERVICE
then

if Server:GetServerGroupID() == 0 then
	GiantBillingManager:SetAddress( '222.73.243.200', 9967 )
	GiantBillingManager:SetNumThread( 1 )
	GiantCouponManager:SetAddress( '222.73.243.200', 8587 )
	GiantCouponManager:SetNumThread( 1 )
end

end



----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 2 )