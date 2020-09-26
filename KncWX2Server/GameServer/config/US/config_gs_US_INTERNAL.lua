-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Game Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "ninil@kog.co.kr, bowrain@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 스크립트 파싱 실패 리포트
Server:SetScriptParsingErrReport( true )
Server:AddScriptParsingErrReportMail( 'bowrain@kog.co.kr' )	-- 오현택
Server:AddScriptParsingErrReportMail( 'ninil@kog.co.kr' )	-- 임홍락

-- Add moritoring server's IP for checking port of elsword's server ( SERV_PERMIT_PORT_CHECK )
--Server:AddPortCheckMoritoringServer( "79.110.90.240" )

-- Hack Shield HSB File Path
--Server:SetHackShieldHSBFilePath( "E:\\projectx2_svn\\source\\KR\\Trunk\\KncWX2Server\\GameServer\\HackShield\\X2.hsb" )

-- TRWorkerThread 갯수
TRServer:TRWorkerThreadCount( 6 )


----------     GS NetLayer 설정     ----------

NetLayer:SetCheckSequenceNum( true )
NetLayer:SetPacketAuthFailLimit( 100 ) -- 유저가 100개이상 비정상 패킷을 보내면 해당 유저 접속해제


----------     GS SimLayer 설정     ----------
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

-- Virsion Flag --
--[[
VF_INTERNAL	 : 사내 테스트 버전
VF_OPEN_TEST : 오픈 테스트 버전
VF_SERVICE	 : 서비스 버전
--]]
SimLayer:SetVersionFlag( VF_INTERNAL )

-- Common Flag --
--[[
CF_NPGG     : nProtect Game Guard Check
CF_CHECK_IP : 접속 IP Check
CF_HSHIELD	: HackShield
CF_XTRAP	: X-Trap
--]]
--SimLayer:AddCommonFlag( CF_HSHIELD )
--SimLayer:AddCommonFlag( CF_XTRAP )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
AF_K3RCOMBO_US 
--]]
SimLayer:SetAuthFlag( AF_INTERNAL )

-- Billing Flag --
--[[
BF_INTERNAL : 사내 버전 빌링
BF_NEXON_KOREA_TEST : 넥슨 빌링 테섭
BF_NEXON_KOREA : 넥슨 빌링 본섭
BF_PAYLETTER_US : 미국 페이레터
BF_PAYLETTER_US_INTERNAL : 미국 인터널
--]]
SimLayer:SetBillingFlag( BF_INTERNAL )

-- IP Check --
-- 설정되어 있지 않으면 모든 IP 접속 허용
--SimLayer:AddIP( "116.120.238" )

-- Script File Path --
SimLayer:AddPath( "D:\\ProjectX2_SVN\\source\\JP_TW_EU_CN_US\\Trunk\\KncWX2Server\\ServerResource\\US" ) -- server common
SimLayer:AddPath( "D:\\ProjectX2_SVN\\resource\\US\\Trunk\\dat\\Script" ) -- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_WEB		   : 넥슨 WEB
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,	'Config\\US\\Account_US_INTERNAL.dsn',			1, false )

if Server:GetServerGroupID() == 0 then
	DBLayer:AddDB( DC_GAME,		'Config\\US\\game01_US_INTERNAL.dsn',		3, false )
	DBLayer:AddDB( DC_LOG,		'Config\\US\\log_US_INTERNAL.dsn',		3, false )
elseif Server:GetServerGroupID() == 1 then
	DBLayer:AddDB( DC_GAME,		'Config\\US\\game02_US_INTERNAL.dsn',		3, false )
	DBLayer:AddDB( DC_LOG,		'Config\\US\\log_US_INTERNAL.dsn',			3, false )
else
	DBLayer:AddDB( DC_GAME,		'Config\\US\\game01_US_INTERNAL.dsn',		3, false )
	DBLayer:AddDB( DC_LOG,		'Config\\US\\log_US_INTERNAL.dsn',		3, false )
end

DBLayer:AddDB( DC_KOG_BILLING,		'Config\\US\\Billing_US_INTERNAL.dsn',		3, false )

--DBLayer:AddDB( DC_SMS,		'SMS_TEST.dsn',				1, false )
--DBLayer:AddDB( DC_NX_WEB,	'NXWeb_TEST.dsn',			3, false )


----------     빌링 설정     ----------

-- InitNexonBillingTCPInfo( 서버 IP, 서버 Port, 서버 도메인 )
if SimLayer:GetBillingFlag() == BF_PAYLETTER_US
then
PayletterBillingManager:InitPayletterBillingInfo( '222.127.25.29', 30010, 99 )
PayletterBillingManager:SetNumThread( 10 )
end



----------     통계 설정     ----------

StatisticsManager:SetUserLog( true )
StatisticsManager:SetBillingPacketLog( true )

StatisticsManager:AddStatistics( SI_DISCONNECT_USERFSM, 1800, true )
StatisticsManager:AddStatistics( SI_DISCONNECT_REASON, 300, true )
StatisticsManager:AddStatistics( SI_PVP_MAP, 3600, true )
StatisticsManager:AddStatistics( SI_FPS, 3600, true )
StatisticsManager:AddStatistics( SI_ED, 3600, true )
StatisticsManager:AddStatistics( SI_QUEST, 3600, true )
StatisticsManager:AddStatistics( SI_CHARACTER, 10, true )
StatisticsManager:AddStatistics( SI_LOC_ITEM, 3600, false )
StatisticsManager:AddStatistics( SI_LOC_SPIRIT, 3600, false )
StatisticsManager:AddStatistics( SI_LOC_ENCHANT, 3600, false )
StatisticsManager:AddStatistics( SI_ITEM_DB, 3600, true )
StatisticsManager:SetStatisticsFlushTime( SI_ITEM_DB, 4 )




----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 0 )
