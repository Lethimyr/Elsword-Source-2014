-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.


--#ifdef SERV_HENIR_RENEWAL_2013

-- 자동 부활 스테이지 : 비밀 스테이지를 제외한 진행한 스테이지 넘버		// 2013 헤니르 개편 // 샌더 지역 추가 및 휴식방으로 변경 // 2013.09.15 // 공솔
--HenirResultTable:AddResurrectionStage( 9 )		-- 루벤/엘더 휴식방
--HenirResultTable:AddResurrectionStage( 27 )		-- 베스마/알테라 휴식방
--HenirResultTable:AddResurrectionStage( 36 )		-- 페이타 휴식방
--HenirResultTable:AddResurrectionStage( 44 )		-- 벨더 휴식방
--HenirResultTable:AddResurrectionStage( 54 )		-- 하멜 휴식방
--HenirResultTable:AddResurrectionStage( 60 )		-- 최종 스테이지 (진 카리스)
--HenirResultTable:AddResurrectionStage( 63 )		-- 샌더 휴식방


-- 임시 인벤토리 비우는 스테이지	// 2013 헤니르 개편 // 샌더 지역 추가 및 휴식처 이후로 변경 // 2013.09.15 // 공솔
--HenirResultTable:AddClearTempInventoryStage( 10 )		-- 루벤/엘더 휴식방 이후 [베스마/알테라 1스테이지]
--HenirResultTable:AddClearTempInventoryStage( 11 )	
--HenirResultTable:AddClearTempInventoryStage( 12 )	
--HenirResultTable:AddClearTempInventoryStage( 13 )	
--HenirResultTable:AddClearTempInventoryStage( 14 )
--HenirResultTable:AddClearTempInventoryStage( 28 )		-- 베스마/알테라 휴식방 이후 [페이타 1스테이지]
--HenirResultTable:AddClearTempInventoryStage( 29 )
--HenirResultTable:AddClearTempInventoryStage( 37 )		-- 페이타 휴식방 이후 [벨더 1스테이지]
--HenirResultTable:AddClearTempInventoryStage( 38 )
--HenirResultTable:AddClearTempInventoryStage( 45 )	-- 벨더 휴식방 이후 [하멜 1스테이지]
--HenirResultTable:AddClearTempInventoryStage( 55 )		-- 하멜 휴식방 이후 [샌더 1스테이지]
--HenirResultTable:AddClearTempInventoryStage( 56 )

--#else //SERV_HENIR_RENEWAL_2013

-- 자동 부활 스테이지 : 비밀 스테이지를 제외한 진행한 스테이지 넘버
HenirResultTable:AddResurrectionStage( 9 )
HenirResultTable:AddResurrectionStage( 19 )
HenirResultTable:AddResurrectionStage( 29 )
HenirResultTable:AddResurrectionStage( 38 )
HenirResultTable:AddResurrectionStage( 47 )
HenirResultTable:AddResurrectionStage( 54 )
HenirResultTable:AddResurrectionStage( 56 )

-- 임시 인벤토리 비우는 스테이지
HenirResultTable:AddClearTempInventoryStage( 10 )
HenirResultTable:AddClearTempInventoryStage( 20 )
HenirResultTable:AddClearTempInventoryStage( 30 )
HenirResultTable:AddClearTempInventoryStage( 39 )
HenirResultTable:AddClearTempInventoryStage( 48 )
-- HenirResultTable:AddClearTempInventoryStage( 56 )

--#endif //SERV_HENIR_RENEWAL_2013


-- 헤니르 시공 던전 전체공지 띄우는 스테이지
HenirResultTable:AddClearNotifyStage( 31 )
HenirResultTable:AddClearNotifyStage( 32 )
HenirResultTable:AddClearNotifyStage( 33 )
HenirResultTable:AddClearNotifyStage( 34 )
HenirResultTable:AddClearNotifyStage( 35 )
HenirResultTable:AddClearNotifyStage( 36 )
HenirResultTable:AddClearNotifyStage( 37 )
HenirResultTable:AddClearNotifyStage( 38 )
HenirResultTable:AddClearNotifyStage( 39 )
HenirResultTable:AddClearNotifyStage( 40 )
HenirResultTable:AddClearNotifyStage( 41 )
HenirResultTable:AddClearNotifyStage( 42 )
HenirResultTable:AddClearNotifyStage( 43 )
HenirResultTable:AddClearNotifyStage( 44 )
HenirResultTable:AddClearNotifyStage( 45 )

-- #ifdef SERV_HENIR_NO_NOTIFY
HenirResultTable:SetEnbleNotify( False )