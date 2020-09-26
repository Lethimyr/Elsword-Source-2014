-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.



LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_RUBEN"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_RUBEN"],			-- 해당 마을을 품는 로컬 ID

	START_POS = 
	{
	    {
	        StartPosId  = 1,
	        StartPosX   = 915.809,
	        StartPosY   = 1347,
	        StartPosZ   = -2867.6,     
	        IsSummon    = TRUE,           
	    },
	    {
	        StartPosId = 2,
	        StartPosX   = 17194,
	        StartPosY   = 1460,
	        StartPosZ   = -5533,	     
	        IsSummon    = TRUE,   	        
	    },
	    {
	        StartPosId = 3,
	        StartPosX   = 17508,
	        StartPosY   = 1460,
	        StartPosZ   = -5828,	 
	        IsNPC       = FALSE,
	        LinkPos   = { 10, },       
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
	        --StartPosX   = 18553.61,
	        StartPosX   = 13367.1,
	        StartPosY   = 0,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId = 12,
	        StartPosX   = 50.0,
	        StartPosY   = 0,
	        StartPosZ   = 0,
	        LinkPos   = { 2, }, 	        
	    },
	    {
	        StartPosId = 13,
	        --StartPosX   = 20183,
	        StartPosX   = 13952,
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
	        StartPosX   = 339.972,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 31,
	        StartPosX   = 13554,
	        StartPosY   = 1000,
	        StartPosZ   = 0,	
	        IsSummon    = TRUE,      
	    },
	    {
	        StartPosId  = 32,
	        StartPosX   = 0,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        LinkPos   = { 21, }, 	        
	    },
	    {
	        StartPosId  = 33,
	        StartPosX   = 13900,
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
	        LinkPos   = { 70, }, 	  --70 페이타      
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
	        LinkPos   = { 80, }, 	        
	    },
	}
}


LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_PEITA"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_PEITA"],			-- 해당 마을을 품는 로컬 ID
	
	START_POS = 
	{
	    {
	        StartPosId  = 70,
	        StartPosX   = 4953.64,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 71,
	        StartPosX   = 20625.2,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,     
	    },
	    {
	        StartPosId  = 72,
	        StartPosX   = 4225.3,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        LinkPos   = { 51, }, 	        
	    },
	}
}



LocationManager:Village
{
	VillageMapID		= VILLAGE_MAP_ID["VMI_DUNGEON_GATE_ALTERA"],

	BaseLocalStateID	= LOCAL_MAP_ID["LMI_ALTERA_ISLAND"],			-- 해당 마을을 품는 로컬 ID
	
	START_POS = 
	{
	     {
	        StartPosId  = 80,
	        StartPosX   = 515.295,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        IsSummon    = TRUE,
	    },
	    {
	        StartPosId  = 81,
	        StartPosX   = 9402.67,
	        StartPosY   = 1000,
	        StartPosZ   = 0,	
	        IsSummon    = TRUE,      
	    },
	    {
	        StartPosId  = 82,
	        StartPosX   = 0.001,
	        StartPosY   = 1000,
	        StartPosZ   = 0,
	        LinkPos   = { 61, }, 	        
	    },	    
	}
}













