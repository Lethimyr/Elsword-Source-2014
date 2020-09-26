-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

-- 큐브 내용물 매핑
-- g_pRandomItemManager:AddRandomItemMappingData( int nIndex, int iCubeID, int iKeyItemID, int iBeforeGroupID, int iAfterGroupID)
-- g_pRandomItemManager:AddRandomItemMappingTime( int nIndex, const char* szStartDate, const char* szEndDate)
--[[
g_pRandomItemManager:AddRandomItemMappingData( 1, 65000031, 131377, 161003, 161001)
g_pRandomItemManager:AddRandomItemMappingData( 2, 65000031, 207500, 161002, 161000)
g_pRandomItemManager:AddRandomItemMappingTime( 1, "2013-07-31 10:00:00", "2013-08-31 23:00:00" )
g_pRandomItemManager:AddRandomItemMappingTime( 2, "2013-07-31 10:00:00", "2013-08-31 23:00:00" )
--]]

g_pRandomItemManager:AddRandomItemMappingData( 1, 70007190, 0, 161060, 161140)
g_pRandomItemManager:AddRandomItemMappingData( 2, 160773, 269731, 161280, 161281)
-- 전 캐릭터 TB갱신
g_pRandomItemManager:AddRandomItemMappingData( 3, 70000065, 0, 161172, 102631)
g_pRandomItemManager:AddRandomItemMappingData( 4, 70000066, 0, 161173, 102632)
g_pRandomItemManager:AddRandomItemMappingData( 5, 70000067, 0, 161174, 102633)
g_pRandomItemManager:AddRandomItemMappingData( 6, 70000068, 0, 161175, 102634)
g_pRandomItemManager:AddRandomItemMappingData( 7, 70000069, 0, 161176, 102635)
g_pRandomItemManager:AddRandomItemMappingData( 8, 70001800, 0, 161177, 102636)
g_pRandomItemManager:AddRandomItemMappingData( 9, 70006020, 0, 161178, 102637)

g_pRandomItemManager:AddRandomItemMappingTime( 1, "2013-11-27 10:00:00", "2014-01-08 09:59:00" )	-- 아라 런칭 기념 트레져 박스( 2014년 1월 8일 패치때에는 자동 갱신 기능 끄고 들어가야함 )
g_pRandomItemManager:AddRandomItemMappingTime( 2, "2013-12-31 10:00:00", "2014-01-29 09:59:00" )	-- 고대 화석 판별기( 넉넉하게 기간 한달 잡아놓긴 했는데 구성품 바뀌는 시기 파악해야함 )

g_pRandomItemManager:AddRandomItemMappingTime( 3, "2013-12-04 10:00:00", "2014-01-08 09:59:00" )	
g_pRandomItemManager:AddRandomItemMappingTime( 4, "2013-12-04 10:00:00", "2014-01-08 09:59:00" )
g_pRandomItemManager:AddRandomItemMappingTime( 5, "2013-12-04 10:00:00", "2014-01-08 09:59:00" )
g_pRandomItemManager:AddRandomItemMappingTime( 6, "2013-12-04 10:00:00", "2014-01-08 09:59:00" )
g_pRandomItemManager:AddRandomItemMappingTime( 7, "2013-12-04 10:00:00", "2014-01-08 09:59:00" )
g_pRandomItemManager:AddRandomItemMappingTime( 8, "2013-12-04 10:00:00", "2014-01-08 09:59:00" )
g_pRandomItemManager:AddRandomItemMappingTime( 9, "2013-12-04 10:00:00", "2014-01-08 09:59:00" )
