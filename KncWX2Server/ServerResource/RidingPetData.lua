-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

-- 라이딩 펫 생성 아이템
--												ItemID				RidingPetID					  Period( 일 단위이며, 영구권은 -1 )
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550000, RIDING_PET_UNIT_ID["RPUI_NASOD_MOBI"],		-1	)	-- 소환석 : 나소드 모비
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550001, RIDING_PET_UNIT_ID["RPUI_NASOD_MOBI"],		1	)	-- 소환석 : 나소드 모비 (1일권)
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550003, RIDING_PET_UNIT_ID["RPUI_NASOD_MOBI"],		7	)	-- 소환석 : 나소드 모비 (7일권)

g_pRidingPetManager:AddRidingPetCreateItemInfo( 550010, RIDING_PET_UNIT_ID["RPUI_KOUCIKA"],			-1	)	-- 소환석 : 신속의 코카
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550011, RIDING_PET_UNIT_ID["RPUI_KOUCIKA"],			1	)	-- 소환석 : 신속의 코카 (1일권)
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550013, RIDING_PET_UNIT_ID["RPUI_KOUCIKA"],			7	)	-- 소환석 : 신속의 코카 (7일권)

g_pRidingPetManager:AddRidingPetCreateItemInfo( 550020, RIDING_PET_UNIT_ID["RPUI_ANCIENT_PPORU"],	-1	)	-- 소환석 : 에이션트 뽀루
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550021, RIDING_PET_UNIT_ID["RPUI_ANCIENT_PPORU"],	1	)	-- 소환석 : 에이션트 뽀루 (1일권)
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550023, RIDING_PET_UNIT_ID["RPUI_ANCIENT_PPORU"],	7	)	-- 소환석 : 에이션트 뽀루 (7일권)

g_pRidingPetManager:AddRidingPetCreateItemInfo( 550030, RIDING_PET_UNIT_ID["RPUI_SCORPION_TYPE_R"],	-1	)	-- 소환석 : 퐁고 스콜피온 TYPE-R

g_pRidingPetManager:AddRidingPetCreateItemInfo( 550040, RIDING_PET_UNIT_ID["RPUI_ICE_HAMELING"],	-1	)	-- 소환석 : 아이스 하멜링

g_pRidingPetManager:AddRidingPetCreateItemInfo( 550041, RIDING_PET_UNIT_ID["RPUI_DARK_HAMELING"],	-1	)	-- 소환석 : 다크 하멜링

g_pRidingPetManager:AddRidingPetCreateItemInfo( 550042, RIDING_PET_UNIT_ID["RPUI_RED_HAMELING"],	-1	)	-- 소환석 : 레드 하멜링


--겨울 방학 이벤트. 거래 불가용 소환석들
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550043, RIDING_PET_UNIT_ID["RPUI_NASOD_MOBI"],	-1	)		-- 소환석 : 나소드 모비
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550044, RIDING_PET_UNIT_ID["RPUI_KOUCIKA"],	-1	)			-- 소환석 : 신속의 코카
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550045, RIDING_PET_UNIT_ID["RPUI_ANCIENT_PPORU"],	-1	)	-- 소환석 : 에이션트 뽀루
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550046, RIDING_PET_UNIT_ID["RPUI_SCORPION_TYPE_R"],	-1	)	-- 소환석 : 퐁고 스콜피온 TYPE-R
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550047, RIDING_PET_UNIT_ID["RPUI_ICE_HAMELING"],	-1	)	-- 소환석 : 아이스 하멜링
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550048, RIDING_PET_UNIT_ID["RPUI_DARK_HAMELING"],	-1	)	-- 소환석 : 다크 하멜링
g_pRidingPetManager:AddRidingPetCreateItemInfo( 550049, RIDING_PET_UNIT_ID["RPUI_RED_HAMELING"],	-1	)	-- 소환석 : 레드 하멜링