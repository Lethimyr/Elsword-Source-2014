----------     Center Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "demiahun@kogstudios.com, comicprince@kogstudios.com" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )  -- 종료시 db 처리 대기시간 : 1분.

-- 서버간 접속 모니터링 동작 여부
Server:SetConnectionMornitoringEnable( true )

-- Add moritoring server's IP for checking port of elsword's server ( SERV_PERMIT_PORT_CHECK )
--Server:AddPortCheckMoritoringServer( "79.110.90.240" )

----------     Center NetLayer 설정     ----------

NetLayer:SetCheckSequenceNum( false ) -- 내부통신은 SequenceNum 체크필요없음


----------     Center SimLayer 설정     ----------
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


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_SHOP_GATEWAY : 넥슨 빌링 게이트웨이
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:SetManagerDB( ESC_CENTER, 'DRIVER=SQL Server;Network=DBMSSOCN;DATABASE=ManagerDB;WSID=ES_GAME_SERVER;SERVER=211.39.155.72', true )
--DBLayer:AddDB( DC_SMS,		'SMS.dsn', 1, false )


----------     룸 설정     ----------

RoomManager:SetNumPvPRoom( 1500 )
RoomManager:SetNumDungeonRoom( 1500 )
RoomManager:SetNumSquare( 10 ) -- DB에 설정되어 있는 광장 개수 이상이어야 한다.
RoomManager:SetNumTrade( 300 )
RoomManager:SetNumTrainingCenterRoom( 1500 )
RoomManager:SetNumPersonalShop( 150 )
RoomManager:SetNumBattleField( 1800 )
--RoomManager:SetNumArcadeRoom( 1000 ) -- 사용 안하는 방

-- 좀비유저 체크 시간 설정 --
RoomManager:SetZUTickTime( 300 )
RoomManager:SetBattleFieldZUTickTime( 20 )		-- 배틀필드 좀비유저 체크 시간 설정

----------     통계 설정     ----------

StatisticsManager:SendToFTPStatistics( true )

StatisticsManager:AddStatistics( SI_RES_STONE, 3600, true )
StatisticsManager:AddStatistics( SI_LOC_PVP, 3600, false )


----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 0 )