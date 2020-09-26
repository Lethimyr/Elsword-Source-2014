-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.


-------------------------------------------------------------------- 게임진입

ActionDecider:AddAction( S_EXIT, A_CONNECT_CHANNEL_SERVER, 1.0 )

ActionDecider:AddAction( S_CHANNEL_SELECT, A_GET_CHANNEL_LIST, 0.0 )
ActionDecider:AddAction( S_CHANNEL_SELECT, A_CONNECT_GAME_SERVER, 1.0 )

ActionDecider:AddAction( S_INIT, A_MAKE_ACCOUNT, 0.0 )
ActionDecider:AddAction( S_INIT, A_VERIFY, 1.0 )

ActionDecider:AddAction( S_SERVER_SELECT, A_SELECT_SERVER, 0.9 )
ActionDecider:AddAction( S_SERVER_SELECT, A_EXIT, 0.0 )

ActionDecider:AddAction( S_UNIT_SELECT, A_SELECT_UNIT, 0.5 )
ActionDecider:AddAction( S_UNIT_SELECT, A_CREATE_UNIT, 0.0 )
ActionDecider:AddAction( S_UNIT_SELECT, A_DELETE_UNIT, 0.0 )
ActionDecider:AddAction( S_UNIT_SELECT, A_EXIT, 0.0 )

--------------------------------------------------------------------- 게임시작

ActionDecider:AddAction( S_FIELD_MAP, A_STATE_CHANGE_SERVER_SELECT, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_STATE_CHANGE_FIELD, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_FIELD_MOVE, 0.5 )
ActionDecider:AddAction( S_FIELD_MAP, A_FIELD_STAND, 0.4 )
ActionDecider:AddAction( S_FIELD_MAP, A_FIELD_CHAT, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_CREATE_PARTY, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_JOIN_PARTY, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_CLIENT_QUIT, 0.1 )

ActionDecider:AddAction( S_PARTY, A_LEAVE_PARTY, 0.0 )
ActionDecider:AddAction( S_PARTY, A_FIELD_MOVE, 0.0 )
ActionDecider:AddAction( S_PARTY, A_FIELD_STAND, 0.0 )
ActionDecider:AddAction( S_PARTY, A_PARTY_CHAT, 0.0 )

ActionDecider:AddAction( S_FIELD_MAP, A_ENTER_NPC_HOUSE, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_ENTER_EQUIP_SHOP, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_EXIT, 0.0 )

ActionDecider:AddAction( S_FIELD_MAP, A_OPEN_PVP_ROOM_LIST, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_ROOM_LIST, 0.0 )

ActionDecider:AddAction( S_FIELD_MAP, A_JOIN_SQUARE, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_CREATE_ROOM, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_CREATE_TC_ROOM, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_JOIN_ROOM, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_QUICK_JOIN_ROOM, 0.0 )
ActionDecider:AddAction( S_FIELD_MAP, A_SEARCH_UNIT, 0.0 )

ActionDecider:AddAction( S_ROOM, A_SQUARE_UNIT_SYNC, 0.5 )
ActionDecider:AddAction( S_ROOM, A_LEAVE_ROOM, 0.1 )
ActionDecider:AddAction( S_ROOM, A_CHAT, 0.2 )
ActionDecider:AddAction( S_ROOM, A_CHANGE_TEAM, 0.2 )
ActionDecider:AddAction( S_ROOM, A_CHANGE_DIFFICULTY, 0.0 )
ActionDecider:AddAction( S_ROOM, A_SEARCH_UNIT, 0.0 )

SetConnectCapacity( 0.08, 1 )
--SetConnectCapacity( 3, 1 )

-- 마을 이동 랜덤값

RobotManager:AddFieldRandomInfo( 20000, 30.0 ) -- 루벤
RobotManager:AddFieldRandomInfo( 10000, 37.0 ) -- 루벤 던전게이트
RobotManager:AddFieldRandomInfo( 20001,  8.0 ) -- 엘더
RobotManager:AddFieldRandomInfo( 10001, 11.0 ) -- 엘더 던전게이트
RobotManager:AddFieldRandomInfo( 20002,  4.0 ) -- 베스마
RobotManager:AddFieldRandomInfo( 10002,  5.0 ) -- 베스마 던전게이트
RobotManager:AddFieldRandomInfo( 10004,  2.0 ) -- 페이타 던전게이트
RobotManager:AddFieldRandomInfo( 20003,  1.0 ) -- 알테라
RobotManager:AddFieldRandomInfo( 10003,  2.0 ) -- 알테라 던전게이트
--[[
--RobotManager:AddFieldRandomInfo( 20000, 100.0 ) -- 루벤
--]]

-- 브로드캐스팅 인원제한 옵션
RobotManager:SetLimitOption( 50 )

-- 사내섭
--RobotManager:AddServer( GetLocalIP(), 9300 )
RobotManager:AddServer( '192.168.140.53', 9300 )
--RobotManager:AddServer( '116.120.238.189', 9400 )

-- 오픈테섭
--RobotManager:AddServer( '211.39.155.92', 9300 )

-- 신규본섭
--RobotManager:AddServer( '211.39.155.95', 9300 )
--RobotManager:AddServer( '211.39.155.96', 9300 )
--RobotManager:AddServer( '211.39.155.97', 9300 )
--RobotManager:AddServer( '211.39.155.98', 9300 )
--RobotManager:AddServer( '211.39.155.99', 9300 )
--RobotManager:AddServer( '211.39.155.100', 9300 )
--RobotManager:AddServer( '211.39.155.101', 9300 )
--RobotManager:AddServer( '211.39.155.102', 9300 )

RobotManager:SetVersion( "N_20100405_A" )
--RobotManager:SetVersion( "N_20090928_A_SB" )
RobotManager:set( 1, 1800 )
--RobotManager:set( 2001, 4000 )

RobotManager:SetEnumFilePath( 'C:\\ProjectX2_SVN\\Resource\\KR\\Trunk\\dat\\Script\\Major\\enum.lua' )
RobotManager:SetVillageMapFilePath( 'MapVillage.lua' )
RobotManager:SetRobotPatternFilePath( 'C:\\ProjectX2_SVN\\source\\KR\\Trunk\\KncWX2Server\\GameServer\\RobotPattern\\RobotActionScript_' )


--robot:SetIP('116.120.238.69')
--robot:SetIP('211.39.155.92')
--robot:UserAge( 1, 100 )
--robot:UserAge( 1,  50)	--한줄 추가시 마다 쓰레드 한개씩 추가.
--robot:UserAge(51, 100)

log(0)
