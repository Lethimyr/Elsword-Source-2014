-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Game Server 설정     ----------

--  서비스 버전을 사용함
Server:SetUseVersion( VE_SERVICE )

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "honnak@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 스크립트 파싱 실패 리포트
--Server:SetScriptParsingErrReport( true )
--Server:AddScriptParsingErrReportMail( 'honnak@kog.co.kr' )	-- 임홍락

-- Add moritoring server's IP for checking port of elsword's server ( SERV_PERMIT_PORT_CHECK )
--Server:AddPortCheckMoritoringServer( "79.110.90.240" )

-- Hack Shield HSB File Path
Server:SetHackShieldHSBFilePath( "D:\\Elsword\\Server\\GameServer\\HackShield\\X2.hsb" )

-- Add moritoring server's IP for checking port of elsword's server
Server:AddPortCheckMoritoringServer( "79.110.90.240" )

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
SimLayer:SetPublisherBillingConnectType( PBCT_NORMAL )
SimLayer:SetCheckCouponByPublisher( false )

-- Virsion Flag --
--[[
VF_INTERNAL	 : 사내 테스트 버전
VF_OPEN_TEST : 오픈 테스트 버전
VF_SERVICE	 : 서비스 버전
--]]
SimLayer:SetVersionFlag( VF_SERVICE )

-- Nation Flag --
--[[
NF_DE : 독일 스트링 필터
NF_FR : 프랑스 스트링 필터
NF_IT : 이탈리아 스트링 필터
NF_PL : 폴란드 스트링 필터
NF_ES : 스페인 스트링 필터
NF_TR : 터키 스트링 필터
NF_UK : 영국 스트링 필터
--]]
SimLayer:SetNationFlag( NF_IT )

-- Common Flag --
--[[
CF_NPGG     : nProtect Game Guard Check
CF_CHECK_IP : 접속 IP Check
CF_HSHIELD	: HackShield
CF_NONE
--]]
SimLayer:AddCommonFlag( CF_HSHIELD )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )

-- Billing Flag --
--[[
BF_NEXON_KOREA_TEST : 넥슨 빌링 테섭
BF_NEXON_KOREA : 넥슨 빌링 본섭
--]]
--SimLayer:SetBillingFlag( BF_NEXON_KOREA_TEST )
SimLayer:SetBillingFlag( BF_GLOBAL_SERVICE )

-- IP Check --
-- 설정되어 있지 않으면 모든 IP 접속 허용
--SimLayer:AddIP( "116.120.238" )

-- Script File Path --
SimLayer:AddPath( "D:\\Elsword\\Server\\ServerResource" ) -- server ServerResource
SimLayer:AddPath( "D:\\Elsword\\Server\\ClientScript" ) -- client script

-- Server Group 수 설정 --
SimLayer:SetServerGroupNum( 1 ) -- 서버군의 수(진입구조 개편 관련하여 필요)

----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_WEB		   : 넥슨 WEB
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,	'Account_GAMEFORGE_IT_LIVE.dsn',				10, false )
DBLayer:AddDB( DC_GAME,		'Game01_GAMEFORGE_IT_LIVE.dsn',				15, false )
DBLayer:AddDB( DC_LOG,		'Log_GAMEFORGE_IT_LIVE.dsn',						10, false )	
DBLayer:AddDB( DC_KOG_BILLING,		'Billing_GAMEFORGE_IT_LIVE.dsn',		10, false )
DBLayer:AddDB( DC_CHAT_LOG,   'ChatLog_GAMEFORGE_IT_LIVE.dsn',   5, false )


----------     빌링 설정     ----------

if SimLayer:GetNationFlag() == NF_IT
then
  -- GFBillingManager:AddWebServiceAddress( 'http://soapinterface.elsword.de:9980/ElswordWS.svc' )
  -- GFBillingManager:AddWebMethod( GFBilling_GetCash, 'http://tempuri.org/IElswordWS/getCashFromWebDB' )
  -- GFBillingManager:AddWebMethod( GFBilling_ReduceCash, 'http://tempuri.org/IElswordWS/reduceCashOnWebDB' )
  GFBillingManager:AddWebServiceAddress( 'http://it.soapinterface.elsword.de/elsword' )
  GFBillingManager:AddWebMethod( GFBilling_GetCash, 'urn:ElswordAction' )
  GFBillingManager:AddWebMethod( GFBilling_ReduceCash, 'urn:ElswordAction' )  
  GFBillingManager:Init( 5 )
end


----------     통계 설정     ----------

StatisticsManager:SendToFTPStatistics( false )
StatisticsManager:SetUserLog( true )
StatisticsManager:SetBillingPacketLog( true )

StatisticsManager:AddStatistics( SI_DISCONNECT_USERFSM, 1800, true )
StatisticsManager:AddStatistics( SI_DISCONNECT_REASON, 300, true )
StatisticsManager:AddStatistics( SI_PVP_MAP, 3600, true )
StatisticsManager:AddStatistics( SI_FPS, 3600, true )
StatisticsManager:AddStatistics( SI_ED, 3600, true )
StatisticsManager:AddStatistics( SI_QUEST, 3600, true )
StatisticsManager:AddStatistics( SI_CHARACTER, 3600, true )
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
