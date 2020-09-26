-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

-- 큐브 내용물 매핑
-- g_pRandomItemManager:AddRandomItemMappingData( int nIndex, int iCubeID, int iKeyItemID, int iBeforeGroupID, int iAfterGroupID)
-- g_pRandomItemManager:AddRandomItemMappingTime( int nIndex, const char* szStartDate, const char* szEndDate)

g_pRandomItemManager:AddRandomItemMappingData( 1, 160773, 269731, 235100, 235101 )
g_pRandomItemManager:AddRandomItemMappingData( 2, 160773, 269731, 235100, 235101 ) -- 테스트용

g_pRandomItemManager:AddRandomItemMappingTime( 1, "2013-12-26 10:00:00", "2014-01-30 09:59:00" )
g_pRandomItemManager:AddRandomItemMappingTime( 2, "2013-12-18 20:00:00", "2013-12-19 08:30:00" ) -- 테스트용