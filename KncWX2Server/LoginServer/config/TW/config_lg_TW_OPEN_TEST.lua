-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Login Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "honnak@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 스크립트 파싱 실패 리포트
--Server:SetScriptParsingErrReport( true )
--Server:AddScriptParsingErrReportMail( 'honnak@kog.co.kr' )	--

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
SimLayer:SetPublisherBillingConnectType( PBCT_UNUSUAL )
SimLayer:SetCheckCouponByPublisher ( false )

SimLayer:SetPublisherCheckGameServerLogin ( false ) -- Only Login Server

-- Nation Flag --
--[[
NF_TW     : 대만
NF_HK 	  : 홍콩
--]]
SimLayer:SetNationFlag( NF_TW )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
AF_GLOBAL_SERVICE
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )

-- Script File Path --
SimLayer:AddPath( "C:\\ELS_Server\\ServerResource" ) -- server script
SimLayer:AddPath( "C:\\ELS_Server\\ClientScript" ) -- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_WEB		   : 넥슨 WEB
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,	'Account_TEST.dsn',		3, false )

if Server:GetServerGroupID() == 0 then
	DBLayer:AddDB( DC_GAME,		'Game01_SOLES_TEST.dsn',		3, false )
	DBLayer:AddDB( DC_LOG,		'Log_SOLES_TEST.dsn',		3, false )
elseif Server:GetServerGroupID() == 1 then
	DBLayer:AddDB( DC_GAME,		'Game02_GAIA_TEST.dsn',		3, false )
	DBLayer:AddDB( DC_LOG,		'Log_GAIA_TEST.dsn',			3, false )
else
	DBLayer:AddDB( DC_GAME,		'Game01_SOLES_TEST.dsn',		3, false )
	DBLayer:AddDB( DC_LOG,		'Log_SOLES_TEST.dsn',			3, false )
end	


----------     인증 설정     ----------

if SimLayer:GetNationFlag() == NF_TW
then
	GASHAuthManager:InitGASHAuthValue( '300148', 'AF' )
	GASHAuthManager:InitGASHAuthAddress( '192.168.211.199', 5411, 5412, 5413 )
	GASHAuthManager:SetNumThread( 1 )
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