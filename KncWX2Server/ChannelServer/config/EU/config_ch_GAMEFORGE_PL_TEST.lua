----------     Channel Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "honnak@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- Add moritoring server's IP for checking port of elsword's server
Server:AddPortCheckMoritoringServer( "79.110.90.240" )

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
SimLayer:SetPublisherBillingConnectType( PBCT_NORMAL )
SimLayer:SetCheckCouponByPublisher( false )

-- Nation Flag --
--[[
NF_TW     : 대만
NF_HK 	  : 홍콩
--]]
SimLayer:SetNationFlag( NF_PL )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_GAME_FORGE_EU : 게임 포지 인증
--]]
SimLayer:SetAuthFlag( AF_GLOBAL_SERVICE )

-- IP Check --
-- 설정되어 있지 않으면 모든 IP 접속 허용
--SimLayer:AddIP( "116.120.238" )
--SimLayer:AddIP( "211.218.236" )
--SimLayer:AddIP( "211.218.237" )
-- 시안제작 인력
--SimLayer:AddIP( "218.153.37.41" )
--SimLayer:AddIP( "218.153.37.237" )
--SimLayer:AddIP( "218.153.37.238" )
--SimLayer:AddIP( "218.153.37.239" )

-- IP체크 여부
SimLayer:SetCheckIPType( CIT_ALL_IP_ALLOW )

-- Script File Path --
--SimLayer:AddPath( "D:\\Elsword\\1_0_0_1\\ServerResource" ) -- server common
--SimLayer:AddPath( "D:\\Elsword\\1_0_0_1\\ClientScript" ) -- client script
SimLayer:AddPath( "D:\\Elsword\\Server\\ServerResource" ) -- server common
SimLayer:AddPath( "D:\\Elsword\\Server\\ClientScript" ) -- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_SHOP_GATEWAY : 넥슨 빌링 게이트웨이
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,	'Account_GAMEFORGE_PL_TEST.dsn',	3, false )
DBLayer:AddDB( DC_LOG, 'Log_GAMEFORGE_PL_TEST.dsn', 3, false )


----------     인증 설정     ----------

if SimLayer:GetNationFlag() == NF_PL
then
  --GFAuthManager:AddWebServiceAddress( 'http://soapinterface.dev.elsword.de/ElswordWS.svc' )
  --GFAuthManager:AddWebMethod( GFO_AUTH, 'http://tempuri.org/IElswordWS/confirmLoginOnWebDB' )
  GFAuthManager:AddWebServiceAddress( 'http://dev.pl.soapinterface.elsword.de:9980/elsword' )  
  GFAuthManager:AddWebMethod( GFO_AUTH, 'urn:ElswordAction' ) 
  GFAuthManager:Init( 3 )
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
