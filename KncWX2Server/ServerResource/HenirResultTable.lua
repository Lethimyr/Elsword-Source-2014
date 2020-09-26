-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요


-- 스테이지별 보상	//	2013 헤니르 개편 // 베스마/알테라 통합 및 샌더 지역 추가 반영 // 2013.09.15 // 공솔
--#ifdef SERV_HENIR_RENEWAL_2013

--HenirResultTable:AddHenirResultItemInfo( 9, DUNGEON_MODE["DM_HENIR_PRACTICE"], 1, 1 )
--HenirResultTable:AddHenirResultItemInfo( 27, DUNGEON_MODE["DM_HENIR_PRACTICE"], 2, 1 )
--HenirResultTable:AddHenirResultItemInfo( 36, DUNGEON_MODE["DM_HENIR_PRACTICE"], 3, 1 )
--HenirResultTable:AddHenirResultItemInfo( 44, DUNGEON_MODE["DM_HENIR_PRACTICE"], 4, 1 )
--HenirResultTable:AddHenirResultItemInfo( 54, DUNGEON_MODE["DM_HENIR_PRACTICE"], 5, 1 )
--HenirResultTable:AddHenirResultItemInfo( 63, DUNGEON_MODE["DM_HENIR_PRACTICE"], 6, 1 )

--HenirResultTable:AddHenirResultItemInfo( 9, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 1, 1 )
--HenirResultTable:AddHenirResultItemInfo( 27, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 2, 1 )
--HenirResultTable:AddHenirResultItemInfo( 36, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 3, 1 )
--HenirResultTable:AddHenirResultItemInfo( 44, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 4, 1 )
--HenirResultTable:AddHenirResultItemInfo( 54, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 5, 1 )
--HenirResultTable:AddHenirResultItemInfo( 63, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 6, 1 )

--#else //SERV_HENIR_RENEWAL_2013

HenirResultTable:AddHenirResultItemInfo( 9, DUNGEON_MODE["DM_HENIR_PRACTICE"], 1, 1 )
HenirResultTable:AddHenirResultItemInfo( 19, DUNGEON_MODE["DM_HENIR_PRACTICE"], 2, 1 )
HenirResultTable:AddHenirResultItemInfo( 29, DUNGEON_MODE["DM_HENIR_PRACTICE"], 3, 1 )
HenirResultTable:AddHenirResultItemInfo( 38, DUNGEON_MODE["DM_HENIR_PRACTICE"], 4, 1 )
HenirResultTable:AddHenirResultItemInfo( 47, DUNGEON_MODE["DM_HENIR_PRACTICE"], 5, 1 )
HenirResultTable:AddHenirResultItemInfo( 56, DUNGEON_MODE["DM_HENIR_PRACTICE"], 6, 1 )

HenirResultTable:AddHenirResultItemInfo( 9, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 1, 1 )
HenirResultTable:AddHenirResultItemInfo( 19, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 2, 1 )
HenirResultTable:AddHenirResultItemInfo( 29, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 3, 1 )
HenirResultTable:AddHenirResultItemInfo( 38, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 4, 1 )
HenirResultTable:AddHenirResultItemInfo( 47, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 5, 1 )
HenirResultTable:AddHenirResultItemInfo( 56, DUNGEON_MODE["DM_HENIR_CHALLENGE"], 6, 1 )

--#endif //SERV_HENIR_RENEWAL_2013


-- 보상 아이템 그룹
HenirResultTable:AddHenirResultItemGroup( 1, 61205, 4, 100 )
HenirResultTable:AddHenirResultItemGroup( 2, 61205, 7, 100 )
HenirResultTable:AddHenirResultItemGroup( 3, 61205, 13, 100 )
HenirResultTable:AddHenirResultItemGroup( 4, 61205, 18, 100 )
HenirResultTable:AddHenirResultItemGroup( 5, 61205, 23, 100 )
HenirResultTable:AddHenirResultItemGroup( 6, 61205, 35, 100 )
