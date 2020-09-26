-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

-- 큐브 내용물 매핑
-- g_pRandomItemManager:AddRandomItemMappingData( int nIndex, int iCubeID, int iKeyItemID, int iBeforeGroupID, int iAfterGroupID)
-- g_pRandomItemManager:AddRandomItemMappingTime( int nIndex, const char* szStartDate, const char* szEndDate)
--g_pRandomItemManager:AddRandomItemMappingData( 1, 108900, 207500, 401530, 401632)
--g_pRandomItemManager:AddRandomItemMappingData( 2, 61206, 0, 14834, 1000050)

--g_pRandomItemManager:AddRandomItemMappingTime( 1, "2013-09-18 09:00:00", "2013-12-16 08:59:00" )  --홀리6셋으로 나오게 한다
--g_pRandomItemManager:AddRandomItemMappingTime( 1, "2013-08-21 09:00:00", "2013-09-20 14:30:00" ) -- 사내

-- 드래곤 나이트 7셋 + 리폼 7셋
--[[
-- 사내
g_pRandomItemManager:AddRandomItemMappingData( 1, 108900, 207500, 401530, 401632)
g_pRandomItemManager:AddRandomItemMappingTime( 1, "2013-11-14 09:00:00", "2013-11-14 23:59:59" )
g_pRandomItemManager:AddRandomItemMappingData( 2, 108900, 210000226, 401530, 401632)			 -- 루리엘 가열기
g_pRandomItemManager:AddRandomItemMappingTime( 2, "2013-11-14 09:00:00", "2013-11-14 23:59:59" )

 -- 본섭
g_pRandomItemManager:AddRandomItemMappingData( 3, 108900, 207500, 401530, 401632)
g_pRandomItemManager:AddRandomItemMappingTime( 3, "2014-01-08 09:00:00", "2014-01-31 00:00:00" )
g_pRandomItemManager:AddRandomItemMappingData( 4, 108900, 210000226, 401530, 401632)			 -- 루리엘 가열기
g_pRandomItemManager:AddRandomItemMappingTime( 4, "2014-01-08 09:00:00", "2014-01-31 00:00:00" )
]]--