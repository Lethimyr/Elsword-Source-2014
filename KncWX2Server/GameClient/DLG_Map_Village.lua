-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.



LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_RUBEN"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_RUBEN"],			-- 해당 마을을 품는 로컬 ID

	START_POS = 
	{
	    {
	        StartPosId  = 1,
	        StartPosX   = 684.54,
	        StartPosY   = 1445,
	        StartPosZ   = -2917.31,	     
	        IsSummon    = TRUE,           
	    },
	    {
	        StartPosId = 2,
	        StartPosX   = 17077.4,
	        StartPosY   = 1434.79,
	        StartPosZ   = -5433.81,	     
	        IsSummon    = TRUE,   	        
	    },
	    {
	        StartPosId = 3,
	        StartPosX   = 17531,
	        StartPosY   = 1458,
	        StartPosZ   = -5839,	 
	        IsNPC       = FALSE,
	        LinkPos   = { 10, },       
	    },
	    {
	        StartPosId = 4,
	        StartPosX   = 12436.23,
	        StartPosY   = 1196.61,
	        StartPosZ   = -3166.73,	      	 
	        IsMarket    = TRUE,   
	    },
	    {
	        StartPosId = 5,
	        StartPosX   = -170,
	        StartPosY   = 1445,
	        StartPosZ   = -2864,	      	 
	        LinkDungeon = DUNGEON_ID["DI_EL_FOREST_GATE_NORMAL"], 
	        LinkDungeonPos = 1,
	    },
	}
}

LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_DUNGEON_GATE_RUBEN"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_RUBEN"],			-- 해당 마을을 품는 로컬 ID
	
	START_POS = 
	{
	    {
	        StartPosId  = 10,
	        StartPosX   = 548.196,
	        StartPosY   = 0,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 11,
	        StartPosX   = 18553.61,
	        StartPosY   = 0,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId = 12,
	        StartPosX   = 168.33,
	        StartPosY   = 0,
	        StartPosZ   = 0,
	        LinkPos   = { 2, }, 	        
	    },
	    {
	        StartPosId = 13,
	        StartPosX   = 20183,
	        StartPosY   = 0,
	        StartPosZ   = 0,
	        LinkPos   = { 20, }, 	        
	    },
	}
}

LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_ELDER"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_ELDER"],			-- 해당 마을을 품는 로컬 ID
	
	START_POS = 
	{
	    {
	        StartPosId  = 20,
	        StartPosX   = -8041,
	        StartPosY   = 1400,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 21,
	        StartPosX   = 7870,
	        StartPosY   = 1400,
	        StartPosZ   = 0,	
	        IsSummon    = TRUE,      
	    },
	    {
	        StartPosId  = 22,
	        StartPosX   = -8499,
	        StartPosY   = 1400,
	        StartPosZ   = 0,
	        LinkPos   = { 11, }, 	        
	    },
	    {
	        StartPosId  = 23,
	        StartPosX   = 8521,
	        StartPosY   = 1400,
	        StartPosZ   = 0,
	        LinkPos   = { 30, }, 	        
	    },
	}
}

LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_DUNGEON_GATE_ELDER"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_ELDER"],			-- 해당 마을을 품는 로컬 ID
	
	START_POS = 
	{
	    {
	        StartPosId  = 30,
	        StartPosX   = 1287.78,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 31,
	        StartPosX   = 20691,
	        StartPosY   = 1000,
	        StartPosZ   = 0,	
	        IsSummon    = TRUE,      
	    },
	    {
	        StartPosId  = 32,
	        StartPosX   = 119.688,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        LinkPos   = { 21, }, 	        
	    },
	    {
	        StartPosId  = 33,
	        StartPosX   = 21942.9,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        LinkPos   = { 40, }, 	        
	    },
	}
}


LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_BESMA"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_BESMA"],			-- 해당 마을을 품는 로컬 ID
	
	START_POS = 
	{
	    {
	        StartPosId  = 40,
	        StartPosX   = -4902.71,
	        StartPosY   = 5021,
	        StartPosZ   = -3011.96,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 41,
	        StartPosX   = 9348.22,
	        StartPosY   = 5019,
	        StartPosZ   = -2704.63,	  
	        IsSummon    = TRUE,    
	    },
	    {
	        StartPosId  = 42,
	        StartPosX   = -6045.52,
	        StartPosY   = 5021,
	        StartPosZ   = -2970.47,
	        LinkPos   = { 31, }, 	        
	    },
	    {
	        StartPosId  = 43,
	        StartPosX   = 10613.9,
	        StartPosY   = 5019,
	        StartPosZ   = -2776.48,
	        LinkPos   = { 50, }, 	        
	    },
	}
}


LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_DUNGEON_GATE_BESMA"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_BESMA"],			-- 해당 마을을 품는 로컬 ID
	
	START_POS = 
	{
	    {
	        StartPosId  = 50,
	        StartPosX   = 853.747,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 51,
	        StartPosX   = 14555.4,
	        StartPosY   = 50,
	        StartPosZ   = -1100,	
	        IsSummon    = TRUE,      
	    },
	    {
	        StartPosId  = 52,
	        StartPosX   = 17676.8,
	        StartPosY   = 1496.91,
	        StartPosZ   = 0,	   
	        IsSummon    = TRUE,   
	    },
	    {
	        StartPosId  = 53,
	        StartPosX   = 28.2639,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        LinkPos   = { 41, }, 	        
	    },
	    {
	        StartPosId  = 54,
	        StartPosX   = 15257,
	        StartPosY   = 50,
	        StartPosZ   = -1100,
	        LinkPos   = { 1, }, 	  --70 페이타      
	    },
	    {
	        StartPosId  = 55,
	        StartPosX   = 18563,
	        StartPosY   = 1660,
	        StartPosZ   = 0,
	        LinkPos   = { 60, }, 	        
	    },
	}
}


LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_ALTERA"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_ALTERA_ISLAND"],			-- 해당 마을을 품는 로컬 ID
	
	START_POS = 
	{
	    {
	        StartPosId  = 60,
	        StartPosX   = -6166.62,
	        StartPosY   = 2855.64,
	        StartPosZ   = 667.051,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 61,
	        StartPosX   = 5893.49,
	        StartPosY   = 1460,
	        StartPosZ   = 82,	 
	        IsSummon    = TRUE,     
	    },
	    {
	        StartPosId  = 62,
	        StartPosX   = -6685,
	        StartPosY   = 2917,
	        StartPosZ   = 686,
	        LinkPos   = { 52, }, 	        
	    },
	    {
	        StartPosId  = 63,
	        StartPosX   = 6529,
	        StartPosY   = 1460,
	        StartPosZ   = 82,
	        LinkPos   = { 1, }, 	        
	    },
	}
}