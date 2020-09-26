-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

----------     Global Server 설정     ----------

-- 크래쉬 리포트 메일링 리스트 --
Server:SetMailingList( "honnak@kog.co.kr" )

-- 종료시 db 처리 대기시간 --
Server:SetShutDownWait( 60 * 1000 )

-- 스크립트 파싱 실패 리포트
Server:SetScriptParsingErrReport( true )
Server:AddScriptParsingErrReportMail( 'honnak@kog.co.kr' )	-- 임홍락
Server:AddScriptParsingErrReportMail( '86red@kog.co.kr' )	-- 김석근
Server:AddScriptParsingErrReportMail( 'aaron@kog.co.kr' )	-- 오세윤
Server:AddScriptParsingErrReportMail( 'wonjin@kog.co.kr' )	-- 김원진

----------     Global NetLayer 설정     ----------

NetLayer:SetCheckSequenceNum( false ) -- 내부통신은 SequenceNum 체크필요없음


----------     Global SimLayer 설정     ----------
SimLayer:SetUseKogOTP ( true )

-- Nation Flag --
--[[
NF_DE : 독일 스트링 필터
NF_FR : 프랑스 스트링 필터
NF_IT : 이탈리아 스트링 필터
NF_PL : 폴란드 스트링 필터
NF_ES : 스페인 스트링 필터
NF_TR : 터키 스트링 필터
NF_EN : 영국 스트링 필터
--]]
SimLayer:SetPublisherBillingConnectType (PBCT_NORMAL)
SimLayer:SetCheckCouponByPublisher ( false )
SimLayer:SetNationFlag( NF_DE )
-- Auth Flag --
--[[
AF_INTERNAL    : 사내 인증
AF_NEXON_KOREA : 넥슨 국내 인증
--]]
SimLayer:SetAuthFlag( AF_INTERNAL )

-- Script File Path --
SimLayer:AddPath( "D:\\ProjectX2_SVN\\source\\JP_TW_EU_CN_US\\Trunk\\KncWX2Server\\ServerResource\\EU" ) -- server ServerResource
SimLayer:AddPath( "D:\\ProjectX2_SVN\\resource\\EU\\DE\\Trunk\\dat\\Script" ) -- client script

--SimLayer:AddPath( "C:\\projectx2_svn\\source\\EU\\Trunk\\KncWX2Server\\Common" ) -- server common
--SimLayer:AddPath( "C:\\projectx2_svn\\resource\\EU\\Trunk\\dat\\Script" ) -- client script


----------     DB Layer 설정     ----------

-- 접속 DB 목록 --
--[[
DC_ACCOUNT         : 계정
DC_GAME            : 게임
DC_LOG             : 통계
DC_NX_WEB		   : 넥슨 WEB
--]]
-- AddDB( DB 종류, file dsn, thread 개수, DBConnectionString여부 )

DBLayer:AddDB( DC_ACCOUNT,		'Config\\EU\\Account_INTERNAL_DE.dsn',		1, false )
DBLayer:AddDB( DC_LOG,		'Config\\EU\\log_INTERNAL_DE.dsn',		3, false )
--DBLayer:AddDB( DC_SMS,		'SMS.dsn',				1, false )

----------     로그 레벨 설정     ----------

-- 로그 레벨 --
--[[
-1 : no logs
 0 : cerr, cout
 1 : cerr, cwarn, cout
 2 : cerr, cwarn, clog, cout
--]]
log( 2 )