-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

-- 큐브 내용물 매핑
-- g_pRandomItemManager:AddRandomItemMappingData( int nIndex, int iCubeID, int iKeyItemID, int iBeforeGroupID, int iAfterGroupID)
-- g_pRandomItemManager:AddRandomItemMappingTime( int nIndex, const char* szStartDate, const char* szEndDate)
g_pRandomItemManager:AddRandomItemMappingData( 1, 108900, 207500, 502070, 502071)
g_pRandomItemManager:AddRandomItemMappingTime( 1, "2014-03-12 00:00:00", "2014-03-31 23:59:00" ) -- 본섭용

g_pRandomItemManager:AddRandomItemMappingData( 2, 108900, 207500, 502070, 502071)
g_pRandomItemManager:AddRandomItemMappingTime( 2, "2014-02-23 15:00:00", "2014-02-25 17:59:00" ) -- 테섭용
--[[
-- 판별기
g_pRandomItemManager:AddRandomItemMappingData( 11, 160773, 269731, 402290, 600030)
g_pRandomItemManager:AddRandomItemMappingTime( 11, "2014-03-12 00:00:00", "2014-03-31 23:59:00" ) -- 본섭용

g_pRandomItemManager:AddRandomItemMappingData( 12, 160773, 269731, 402290, 600030)
g_pRandomItemManager:AddRandomItemMappingTime( 12, "2014-02-23 15:00:00", "2014-02-25 17:59:00" ) -- 테섭용
--]]