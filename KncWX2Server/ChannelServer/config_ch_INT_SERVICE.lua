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
SimLayer:SetCheckIPMode( CIM_SERVICE_MODE )		-- 서비스모드
--SimLayer:SetCheckIPMode( CIM_CHECK_MODE )		-- 점검모드


----------------------------------------------------------------
-- 점검 모드 IP 체크
SimLayer:AddCheckModeIP( "14.45.79" )
SimLayer:AddCheckModeIP( "211.234.69" )
SimLayer:AddCheckModeIP( "218.54.221" )
SimLayer:AddCheckModeIP( "211.218.237.129" )
SimLayer:SetCheckModeCheckIPType( CIT_CHECK_IP_ALLOW )



----------------------------------------------------------------
-- 서비스 모드 IP 체크
SimLayer:AddIP( "222.113.245.86" )
SimLayer:AddIP( "61.116.29.157" )
SimLayer:AddIP( "61.116.31.137" )
SimLayer:AddIP( "112.175.245.156" )	-- 2010년 12월 2일
SimLayer:AddIP( "211.126.45.146" )	-- 2011년 1월 4일
SimLayer:AddIP( "211.121.252.152" ) -- 2011년 1월 4일
SimLayer:AddIP( "210.254.60.39" )	-- 2011년 1월 4일
SimLayer:AddIP( "60.237.180.39" )	-- 2011년 1월 4일
SimLayer:AddIP( "143.90.180.138" )	-- 2011년 1월 4일
SimLayer:AddIP( "175.197.52.115" )	-- 2011년 7월 12일
SimLayer:AddIP( "61.252.165.7" )	-- 2011년 7월 12일

---------------------- 작업장 IP차단 리스트 ---------------------
SimLayer:AddIP( "175.125.125.2" )
SimLayer:AddIP( "175.125.125.92" )
SimLayer:AddIP( "175.214.8.7" )
SimLayer:AddIP( "180.131.58.111" )
SimLayer:AddIP( "180.131.58.121" )
SimLayer:AddIP( "180.131.58.131" )
SimLayer:AddIP( "180.131.58.251" )
SimLayer:AddIP( "203.255.146.158" )
SimLayer:AddIP( "203.255.146.178" )
SimLayer:AddIP( "203.255.146.179" )
SimLayer:AddIP( "210.172.220.112" )
SimLayer:AddIP( "210.172.220.113" )
SimLayer:AddIP( "210.172.220.114" )
SimLayer:AddIP( "210.172.220.115" )
SimLayer:AddIP( "210.172.220.116" )
SimLayer:AddIP( "210.172.220.117" )
SimLayer:AddIP( "210.172.220.118" )
SimLayer:AddIP( "210.172.220.119" )
SimLayer:AddIP( "210.172.220.125" )
SimLayer:AddIP( "210.172.220.126" )
SimLayer:AddIP( "210.172.220.127" )
SimLayer:AddIP( "210.172.220.128" )
SimLayer:AddIP( "210.172.220.129" )
SimLayer:AddIP( "210.172.220.140" )
SimLayer:AddIP( "210.172.220.142" )
SimLayer:AddIP( "210.172.220.145" )
SimLayer:AddIP( "210.172.220.147" )
SimLayer:AddIP( "210.172.220.148" )
SimLayer:AddIP( "210.172.220.149" )
SimLayer:AddIP( "210.172.220.150" )
SimLayer:AddIP( "210.172.220.151" )
SimLayer:AddIP( "210.172.220.22" )
SimLayer:AddIP( "210.172.220.3" )
SimLayer:AddIP( "210.172.220.30" )
SimLayer:AddIP( "210.172.220.31" )
SimLayer:AddIP( "210.172.220.32" )
SimLayer:AddIP( "210.172.220.33" )
SimLayer:AddIP( "210.172.220.36" )
SimLayer:AddIP( "210.172.220.39" )
SimLayer:AddIP( "210.172.220.4" )
SimLayer:AddIP( "210.172.220.41" )
SimLayer:AddIP( "210.172.220.54" )
SimLayer:AddIP( "210.172.220.55" )
SimLayer:AddIP( "210.172.220.57" )
SimLayer:AddIP( "210.172.220.58" )
SimLayer:AddIP( "210.172.220.59" )
SimLayer:AddIP( "210.172.220.60" )
SimLayer:AddIP( "210.172.220.64" )
SimLayer:AddIP( "210.172.220.65" )
SimLayer:AddIP( "210.172.220.66" )
SimLayer:AddIP( "210.172.220.67" )
SimLayer:AddIP( "210.172.220.68" )
SimLayer:AddIP( "210.172.220.69" )
SimLayer:AddIP( "210.172.220.7" )
SimLayer:AddIP( "210.172.220.70" )
SimLayer:AddIP( "210.172.220.8" )
SimLayer:AddIP( "210.172.220.87" )
SimLayer:AddIP( "210.172.220.88" )
SimLayer:AddIP( "210.172.220.89" )
SimLayer:AddIP( "210.172.220.90" )
SimLayer:AddIP( "210.172.220.91" )
SimLayer:AddIP( "210.172.220.92" )
SimLayer:AddIP( "210.172.220.93" )
SimLayer:AddIP( "210.172.220.94" )
SimLayer:AddIP( "210.172.221.116" )
SimLayer:AddIP( "210.172.221.117" )
SimLayer:AddIP( "210.172.221.118" )
SimLayer:AddIP( "210.172.221.119" )
SimLayer:AddIP( "210.172.221.120" )
SimLayer:AddIP( "210.172.221.121" )
SimLayer:AddIP( "210.172.221.122" )
SimLayer:AddIP( "210.172.221.133" )
SimLayer:AddIP( "210.172.221.134" )
SimLayer:AddIP( "210.172.221.135" )
SimLayer:AddIP( "210.172.221.136" )
SimLayer:AddIP( "210.172.221.137" )
SimLayer:AddIP( "210.172.221.138" )
SimLayer:AddIP( "210.172.221.14" )
SimLayer:AddIP( "210.172.221.147" )
SimLayer:AddIP( "210.172.221.148" )
SimLayer:AddIP( "210.172.221.15" )
SimLayer:AddIP( "210.172.221.150" )
SimLayer:AddIP( "210.172.221.151" )
SimLayer:AddIP( "210.172.221.16" )
SimLayer:AddIP( "210.172.221.17" )
SimLayer:AddIP( "210.172.221.175" )
SimLayer:AddIP( "210.172.221.176" )
SimLayer:AddIP( "210.172.221.177" )
SimLayer:AddIP( "210.172.221.178" )
SimLayer:AddIP( "210.172.221.179" )
SimLayer:AddIP( "210.172.221.18" )
SimLayer:AddIP( "210.172.221.180" )
SimLayer:AddIP( "210.172.221.181" )
SimLayer:AddIP( "210.172.221.186" )
SimLayer:AddIP( "210.172.221.187" )
SimLayer:AddIP( "210.172.221.188" )
SimLayer:AddIP( "210.172.221.191" )
SimLayer:AddIP( "210.172.221.192" )
SimLayer:AddIP( "210.172.221.193" )
SimLayer:AddIP( "210.172.221.194" )
SimLayer:AddIP( "210.172.221.20" )
SimLayer:AddIP( "210.172.221.206" )
SimLayer:AddIP( "210.172.221.207" )
SimLayer:AddIP( "210.172.221.208" )
SimLayer:AddIP( "210.172.221.209" )
SimLayer:AddIP( "210.172.221.210" )
SimLayer:AddIP( "210.172.221.234" )
SimLayer:AddIP( "210.172.221.235" )
SimLayer:AddIP( "210.172.221.236" )
SimLayer:AddIP( "210.172.221.237" )
SimLayer:AddIP( "210.172.221.238" )
SimLayer:AddIP( "210.172.221.239" )
SimLayer:AddIP( "210.172.221.240" )
SimLayer:AddIP( "210.172.221.32" )
SimLayer:AddIP( "210.172.221.33" )
SimLayer:AddIP( "210.172.221.34" )
SimLayer:AddIP( "210.172.221.36" )
SimLayer:AddIP( "210.172.221.37" )
SimLayer:AddIP( "210.172.221.38" )
SimLayer:AddIP( "210.172.221.52" )
SimLayer:AddIP( "210.172.221.53" )
SimLayer:AddIP( "210.172.221.54" )
SimLayer:AddIP( "210.172.221.55" )
SimLayer:AddIP( "210.172.221.56" )
SimLayer:AddIP( "210.172.221.57" )
SimLayer:AddIP( "210.172.221.58" )
SimLayer:AddIP( "210.172.221.63" )
SimLayer:AddIP( "210.172.221.64" )
SimLayer:AddIP( "210.172.221.79" )
SimLayer:AddIP( "210.172.221.80" )
SimLayer:AddIP( "210.172.221.81" )
SimLayer:AddIP( "210.172.221.82" )
SimLayer:AddIP( "210.172.221.90" )
SimLayer:AddIP( "210.172.221.91" )
SimLayer:AddIP( "210.172.221.92" )
SimLayer:AddIP( "210.172.221.93" )
SimLayer:AddIP( "210.172.221.94" )
SimLayer:AddIP( "210.172.221.96" )
SimLayer:AddIP( "210.172.221.97" )
SimLayer:AddIP( "49.143.216.226" )
SimLayer:AddIP( "49.143.216.246" )
SimLayer:AddIP( "211.188.86.165" )
------------------------------------ 2011년 8월 10일 추가
SimLayer:AddIP( "211.54.57.41" )
------------------------------------ 2011년 10월 5일 추가
SimLayer:AddIP( "211.238.11.4" )
SimLayer:AddIP( "1.237.19.163" )
------------------------------------ 2011년 10월 11일 추가
SimLayer:AddIP( "112.196.201.172" )
SimLayer:AddIP( "210.182.41.6" )
SimLayer:AddIP( "112.196.201.173" )
SimLayer:AddIP( "115.21.122.10" )
------------------------------------ 2011년 10월 12일 추가
SimLayer:AddIP( "114.108.177.89" )
SimLayer:AddIP( "211.232.66.222" )
SimLayer:AddIP( "58.229.121.15" )
SimLayer:AddIP( "61.96.204.11" )
SimLayer:AddIP( "218.234.23.60" )
SimLayer:AddIP( "175.126.145.10" )
SimLayer:AddIP( "211.34.105.31" )
------------------------------------ 2011년 10월 13일 추가
SimLayer:AddIP( "121.184.151.149" )
------------------------------------ 2011년 10월 17일 추가
SimLayer:AddIP( "74.86.74.96" )
SimLayer:AddIP( "174.139.17.98" )
------------------------------------ 2011년 10월 20일 추가
SimLayer:AddIP( "123.142.43.46" )
SimLayer:AddIP( "210.183.54.18" )
SimLayer:AddIP( "112.175.189.37" )
SimLayer:AddIP( "210.183.36.158" )
SimLayer:AddIP( "202.31.152.21" )
SimLayer:AddIP( "222.98.86.131" )
SimLayer:AddIP( "114.207.112.88" )
------------------------------------ 2011년 10월 27일 추가
SimLayer:AddIP( "211.196.186.231" )
------------------------------------ 2011년 10월 28일 추가
SimLayer:AddIP( "210.91.104.212" )
SimLayer:AddIP( "101.101.193.73" )
SimLayer:AddIP( "175.125.127.9" )
SimLayer:AddIP( "49.50.146.20" )
SimLayer:AddIP( "61.97.175.219" )
SimLayer:AddIP( "221.160.43.228" )
SimLayer:AddIP( "1.237.37.218" )
SimLayer:AddIP( "1.234.8.234" )
SimLayer:AddIP( "211.196.186.232" )
SimLayer:AddIP( "218.147.227.202" )
------------------------------------ 2011년 10월 31일 추가
SimLayer:AddIP( "112.196.201.200" )
SimLayer:AddIP( "115.178.34.126" )
SimLayer:AddIP( "112.175.189.26" )
SimLayer:AddIP( "1.234.8.231" )
------------------------------------ 2011년 11월 02일 추가
SimLayer:AddIP( "114.207.246.42" )
------------------------------------ 2011년 11월 08일 추가
SimLayer:AddIP( "211.233.5.54" )
SimLayer:AddIP( "202.30.224.4" )
------------------------------------ 2011년 11월 08일 추가
SimLayer:AddIP( "218.148.68.159" )
------------------------------------ 2011년 11월 08일 추가
SimLayer:AddIP( "211.189.145.94" )
SimLayer:AddIP( "211.47.156.14" )

-- 영구제재 --
------------------------------------ 2012년 08월 21일 추가
SimLayer:AddIP( "183.107.206.143" )
SimLayer:AddIP( "219.250.205.22" )
------------------------------------ 2012년 9월 4일 추가
SimLayer:AddIP( "219.250.204.40" )
SimLayer:AddIP( "219.250.204.38" )


SimLayer:SetCheckIPType( CIT_CHECK_IP_BLOCK )


-- Script File Path --
SimLayer:AddPath( "C:\\ElswordPatch\\ServerPatch_KR\\ServerResource" ) -- server common
SimLayer:AddPath( "C:\\ElswordPatch\\ServerPatch_KR\\ClientScript" ) -- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_SHOP_GATEWAY : 넥슨 빌링 게이트웨이
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:SetManagerDB( ESC_CHANNEL, 'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=ManagerDB;WSID=ES_GAME_SERVER;SERVER=211.39.155.72', true )
--DBLayer:AddDB( DC_SMS,		'SMS.dsn',		1, false )

----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 0 )