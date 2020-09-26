----------     Center Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "ninil@kog.co.kr, bowrain@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )  -- 종료시 db 처리 대기시간 : 1분.

-- 스크립트 파싱 실패 리포트
Server:SetScriptParsingErrReport( true )
Server:AddScriptParsingErrReportMail( 'ninil@kog.co.kr' )	-- 조효진
Server:AddScriptParsingErrReportMail( 'bowrain@kog.co.kr' ) -- 임규수

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
SimLayer:SetPublisherBillingConnectType (PBCT_UNUSUAL)
SimLayer:SetCheckCouponByPublisher ( false )

-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
--]]
SimLayer:SetAuthFlag( AF_INTERNAL )

-- Script File Path --
SimLayer:AddPath( "D:\\projectx2_svn\\source\\JP_TW_EU_CN_US\\Trunk\\KncWX2Server\\ServerResource\\CN" ) -- server script
SimLayer:AddPath( "D:\\projectx2_svn\\resource\\ES_INT\\Branches\\ES_CN\\dat\\Script" ) -- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_SHOP_GATEWAY : 넥슨 빌링 게이트웨이
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,	'Config\\CN\\Account_CN_INTERNAL.dsn',		3, false )
DBLayer:AddDB( DC_LOG,			'Config\\CN\\log_CN_INTERNAL.dsn',			3, false )

if Server:GetServerGroupID() == 0 then
	DBLayer:AddDB( DC_GAME,		'Config\\CN\\game01_CN_INTERNAL.dsn',		3, false )
elseif Server:GetServerGroupID() == 6 then
	DBLayer:AddDB( DC_GAME,		'Config\\CN\\game02_CN_INTERNAL.dsn',		3, false )
end

--DBLayer:AddDB( DC_SMS,		'SMS.dsn',			1, false )

----------     룸 설정     ----------

RoomManager:SetNumPvPRoom( 1500 )
RoomManager:SetNumDungeonRoom( 1500 )
RoomManager:SetNumSquare( 10 )					-- DB에 설정되어 있는 광장 개수 이상이어야 한다.
RoomManager:SetNumTrade( 300 )
RoomManager:SetNumTrainingCenterRoom( 1500 )
RoomManager:SetNumPersonalShop( 150 )
RoomManager:SetNumBattleField( 1500 )
RoomManager:SetNumWeddingHall( 300 )
RoomManager:SetZUTickTime( 60 )					-- 좀비유저 체크 시간 설정
RoomManager:SetBattleFieldZUTickTime( 20 )		-- 배틀필드 좀비유저 체크 시간 설정


----------     통계 설정     ----------

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
log( 2 )
