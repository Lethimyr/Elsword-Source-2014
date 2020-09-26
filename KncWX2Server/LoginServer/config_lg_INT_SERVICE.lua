-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Login Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "demiahun@kogstudios.com, comicprince@kogstudios.com" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 넥슨 passport 인증 레벨
Server:SetPassportLevel( 2 )

-- 서버간 접속 모니터링 동작 여부
Server:SetConnectionMornitoringEnable( true )

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
SimLayer:SetPublisherBillingConnectType (PBCT_NORMAL)
SimLayer:SetCheckCouponByPublisher ( false )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
--]]
SimLayer:SetAuthFlag( AF_NEXON_KOREA )

-- Script File Path --
SimLayer:AddPath( "C:\\ElswordPatch\\ServerPatch_KR\\ServerResource" ) -- server common
SimLayer:AddPath( "C:\\ElswordPatch\\ServerPatch_KR\\ClientScript" ) -- client script

-- 인증 대기 기능
SimLayer:SetEnableAuthWait( true )


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_WEB		   : 넥슨 WEB
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:SetManagerDB( ESC_LOGIN, 'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=ManagerDB;WSID=ES_GAME_SERVER;SERVER=211.39.155.72', true )
--DBLayer:AddDB( DC_SMS,		'SMS.dsn',				1, false )
--DBLayer:AddDB( DC_NX_WEB,	'NXWeb.dsn',			5, false )


----------     인증 설정     ----------

-- PC 방 인증 정보 --
-- InitNexonAuthInfo( 인증 서버 IP, 인증 서버 Port, 서버 도메인, 도메인네임, 세션체크타입, 세션체크카운트 )

if Server:GetServerGroupID() == 0 then
	NXAuthManager:InitNexonAuthInfo( '211.218.232.116', 10029, 0, '솔레스', 2, 25 )
else
	NXAuthManager:InitNexonAuthInfo( '211.218.232.116', 10029, 1, '가이아', 2, 25 )
end

-- 쓰레드 개수 --
NXAuthManager:SetNumThread( 2 )


----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 0 )