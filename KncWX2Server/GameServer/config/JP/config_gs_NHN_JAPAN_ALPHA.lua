-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Game Server 설정     ----------

-- 사내 버전을 사용함
Server:SetUseVersion( VE_OPEN_TEST )

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

-- Hack Shield HSB File Path
--Server:SetHackShieldHSBFilePath( "D:\\projectx2\\source\\KR\\Trunk\\KncWX2Server\\GameServer\\HackShield\\X2.hsb" )

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
SimLayer:SetVersionFlag( VF_OPEN_TEST )

-- Common Flag --
--[[
CF_NPGG     : nProtect Game Guard Check
CF_CHECK_IP : 접속 IP Check
CF_HSHIELD	: HackShield
--]]
SimLayer:AddCommonFlag( CF_NPGG )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_GLOBAL_SERVICE : 퍼블 인증
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )



----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

--DBLayer:AddDB( DC_ACCOUNT,		'Account_NHN_JAPAN_INTERNAL.dsn',		1, false )
--DBLayer:AddDB( DC_GAME,			'Game01_NHN_JAPAN_INTERNAL.dsn',		3, false )
--DBLayer:AddDB( DC_LOG,			'Log_NHN_JAPAN_INTERNAL.dsn',			3, false )
--DBLayer:AddDB( DC_GASH_BILLING,	'GASHBilling_NHN_JAPAN_INTERNAL.dsn',	3, false )
--DBLayer:AddDB( DC_BILLING,		'Billing_NHN_JAPAN_INTERNAL.dsn',		3, false )
DBLayer:AddDB( DC_ACCOUNT,		'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=Account;WSID=PBSWDTD1002;SERVER=119.235.234.207',		1, true )
DBLayer:AddDB( DC_GAME,			'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=Game01;WSID=PBSWDTD1002;SERVER=119.235.234.207',		3, true )
DBLayer:AddDB( DC_LOG,			'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=Statistics;WSID=PBSWDTD1002;SERVER=119.235.234.207',	3, true )
DBLayer:AddDB( DC_KOG_BILLING,		'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=ES_Billing;WSID=PBSWDTD1002;SERVER=119.235.234.207',		3, true )
DBLayer:AddDB( DC_JP_RELAY_DB,		'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=ConnectDB;WSID=PBSWDTD1002;SERVER=119.235.234.207',		3, true )
DBLayer:AddDB( DC_SCRIPT,		'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=ScriptDB;WSID=PBSWDTD1002;SERVER=119.235.234.207',		1, true )
DBLayer:AddDB( DC_EVENT,		'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=GameEvent;WSID=PBSWDTD1002;SERVER=119.235.234.207',		3, true )


-- Script File Path --
SimLayer:AddPath( "D:\\Server\\bin\\ServerResource" ) -- server common
SimLayer:AddPath( "D:\\Server\\bin\\ClientScript" ) -- client script

-- Server Group 수 설정 --
SimLayer:SetServerGroupNum( 1 ) -- 서버군의 수(진입구조 개편 관련하여 필요)


----------     빌링 설정     ----------

-- Billing Flag --
--[[
BF_PURPLE		: PURPLE 빌링
BF_INTERNAL : 사내 버전 빌링
BF_NEXON_KOREA_TEST : 넥슨 빌링 테섭
BF_NEXON_KOREA : 넥슨 빌링 본섭
BF_GLOBAL_SERVICE : 퍼블 빌링
--]]
SimLayer:SetBillingFlag( BF_GLOBAL_SERVICE ) --BF_PURPLE

-- Han Auth/Billing Init Flag --
--[[
HIF_ALPHA		: ALPHA (新Test環境)
HIF_REAL		: REAL　(昔Test環境、Real環境)
--]]
SimLayer:SetHanInitFlag( HIF_REAL )



if SimLayer:GetBillingFlag() == BF_GLOBAL_SERVICE --BF_PURPLE
then

	PurpleBillingManager:SetNumThread( 10 )
end
-- 빌링 서버 정보 --



----------     통계 설정     ----------

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
log( 2 )
