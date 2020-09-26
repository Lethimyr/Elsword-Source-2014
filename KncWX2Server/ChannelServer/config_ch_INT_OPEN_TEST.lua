-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Channel Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "demiahun@kogstudios.com, comicprince@kogstudios.com" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

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
--]]
SimLayer:SetAuthFlag( AF_NEXON_KOREA )

-- IP Check --
SimLayer:SetCheckIPMode( CIM_SERVICE_MODE )		-- 서비스 모드
--SimLayer:SetCheckIPMode( CIM_CHECK_MODE )		-- 점검모드

-- 설정되어 있지 않으면 모든 IP 접속 허용
--SimLayer:AddIP( "211.218.236" )
--SimLayer:AddIP( "211.218.237" )
--SimLayer:AddIP( "218.54.221" )

-- 일본 IP 차단
SimLayer:AddIP( "61.116.29.157" )
SimLayer:AddIP( "61.116.31.137" )
SimLayer:SetCheckIPType( CIT_CHECK_IP_BLOCK )

-- Script File Path --
SimLayer:AddPath( "C:\\ElswordPatch\\ServerPatch_KR_BATTLE_FIELD\\ServerResource" ) -- server common
SimLayer:AddPath( "C:\\ElswordPatch\\ServerPatch_KR_BATTLE_FIELD\\ClientScript" ) -- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_SHOP_GATEWAY : 넥슨 빌링 게이트웨이
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,	'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=Account;WSID=ES_CN_OPEN_TEST;SERVER=211.39.155.71,6424',	5, true )

if Server:GetServerGroupID() == 0 then
	DBLayer:AddDB( DC_LOG, 'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=Statistics;WSID=ES_CH_OPEN_TEST;SERVER=211.39.155.71,6424',	 5, true )
else
	DBLayer:AddDB( DC_LOG, 'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=Statistics02;WSID=ES_CH_OPEN_TEST;SERVER=211.39.155.71,6424', 5, true )
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