-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Channel Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "honnak@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 스크립트 파싱 실패 리포트
--Server:SetScriptParsingErrReport( true )
--Server:AddScriptParsingErrReportMail( 'honnak@kog.co.kr' )	--

-- Add moritoring server's IP for checking port of elsword's server ( SERV_PERMIT_PORT_CHECK )
--Server:AddPortCheckMoritoringServer( "79.110.90.240" )

----------     Channel NetLayer 설정     ----------

NetLayer:SetCheckSequenceNum( true )
NetLayer:SetPacketAuthFailLimit( 10 )


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
SimLayer:SetPublisherBillingConnectType( PBCT_DB )
SimLayer:SetCheckCouponByPublisher( false )

-- Nation Flag --
--[[
NF_TW     : 대만
NF_HK 	  : 홍콩
--]]
SimLayer:SetNationFlag( NF_HK )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
AF_GLOBAL_SERVICE
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )

-- IP Check --
-- 설정되어 있지 않으면 모든 IP 접속 허용

-- HK
SimLayer:AddIP( "118.143.50.8" )
-- KOG
SimLayer:AddIP( "14.45.79" )
SimLayer:AddIP( "211.223.69" )
SimLayer:AddIP( "218.54.221" )
SimLayer:AddIP( "121.181.220" )

--IP체크 여부	
SimLayer:SetCheckIPType( CIT_CHECK_IP_ALLOW )
--SimLayer:SetCheckIPType( CIT_ALL_IP_ALLOW )

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
DBLayer:AddDB( DC_ACCOUNT,	'Account.dsn',		20, false )
DBLayer:AddDB( DC_LOG,		'Log_SOLES.dsn',	5,	false )

----------     인증 설정     ----------
if SimLayer:GetNationFlag() == NF_HK
then
	GASHAuthManager:InitGASHAuthValue( '300159', 'HJ' )
	GASHAuthManager:InitGASHAuthAddress( '172.21.112.7', 5411, 5412, 5413 )
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