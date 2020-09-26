-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

-- true false값

TRUE  = 1
FALSE = 0


-- 16진수 변수

xff = 255
xcc = 204
x99 = 153
x66 = 102
x33 = 51
x00 = 0


--[[ Tool Option ]]

ConsoleImp:InitMornitoringToolOption
{
    m_iLoginAccountDsnID  = 1,

    m_iCCUGraphMaxValue	  = 10000,
    m_iGraphRefreshMinute = 1,
    
    m_iTodayLineColor	  = 3,
    m_iYesterdayLineColor = 5,
    m_iAverageLineColor   = 2,
    m_iGuestUserLineColor = 1,
}



--[[ File Dsn List ]]

ConsoleImp:AddFileDsn
{
	m_iFileDsnID     = 1,
	m_strDsnFileName = "tool_1.dat", -- AccountDB
	m_bIsLog		 = FALSE,
}

ConsoleImp:AddFileDsn
{
	m_iFileDsnID     = 2,
	m_strDsnFileName = "tool_2.dat", -- LogDB
	m_bIsLog		 = TRUE,
}



--[[ Line Color List ]]

ConsoleImp:AddLineColor
{
	m_iLineColorID = 0,
	
    m_iLineColorR = xff,
    m_iLineColorG = xff,
    m_iLineColorB = xff,
}
ConsoleImp:AddLineColor
{
	m_iLineColorID = 1,

    m_iLineColorR = xff,
    m_iLineColorG = x99,
    m_iLineColorB = xff,
}
ConsoleImp:AddLineColor
{
	m_iLineColorID = 2,

    m_iLineColorR = xff,
    m_iLineColorG = xff,
    m_iLineColorB = x00,
}
ConsoleImp:AddLineColor
{
	m_iLineColorID = 3,

    m_iLineColorR = x00,
    m_iLineColorG = xff,
    m_iLineColorB = xff,
}
ConsoleImp:AddLineColor
{
	m_iLineColorID = 4,
	
    m_iLineColorR = xff,
    m_iLineColorG = x00,
    m_iLineColorB = x00,
}
ConsoleImp:AddLineColor
{
	m_iLineColorID = 5,
	
    m_iLineColorR = xcc,
    m_iLineColorG = xff,
    m_iLineColorB = x00,
}
ConsoleImp:AddLineColor
{
	m_iLineColorID = 6,
	
    m_iLineColorR = x66,
    m_iLineColorG = xcc,
    m_iLineColorB = xff,
}
ConsoleImp:AddLineColor
{
	m_iLineColorID = 7,
	
    m_iLineColorR = xff,
    m_iLineColorG = x00,
    m_iLineColorB = xff,
}


--[[ Server Group List ]]
ConsoleImp:AddServerGroupInfo
{
	m_iGroupID     = 0,
	m_strGroupName = "Total",
}

ConsoleImp:AddServerGroupInfo
{
	m_iGroupID	   = 1,
	m_strGroupName = "Solres",
}

ConsoleImp:AddServerGroupInfo
{
	m_iGroupID	   = 2,
	m_strGroupName = "TestServer",
}


--[[ Server List ]]

--[[
ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 1,
	m_iYesterdayLineColorID = 2, 
	m_strServerName			= "기획자서버",
	m_strServerIPAddr		= "116.120.238.82",
	m_iServerGroupID		= 0,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "사내서버",
	m_strServerIPAddr		= "116.120.238.52",
	m_iServerGroupID		= 1,
}
--]]

--[[
ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "YukSa",
	m_strServerIPAddr		= "192.168.140.72",
	m_iServerGroupID		= 1,
}


ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 5,
	m_iYesterdayLineColorID = 6,
	m_strServerName			= "훈쓰",
	m_strServerIPAddr		= "116.120.238.69",
}
--]]

------------------------------------------------------------------ 솔레스

ConsoleImp:AddServerInfo
{
	m_bIsLoginServer		= TRUE,
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 0,
	m_iYesterdayLineColorID = 0,
	m_strServerName			= "Login & Channel",
	m_strServerIPAddr		= "211.39.155.94",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 01",
	m_strServerIPAddr		= "211.39.155.95",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 02",
	m_strServerIPAddr		= "211.39.155.96",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 03",
	m_strServerIPAddr		= "211.39.155.97",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 04",
	m_strServerIPAddr		= "211.39.155.98",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 05",
	m_strServerIPAddr		= "211.39.155.99",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 06",
	m_strServerIPAddr		= "211.39.155.100",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 07",
	m_strServerIPAddr		= "211.39.155.101",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 08",
	m_strServerIPAddr		= "211.39.155.102",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 09",
	m_strServerIPAddr		= "211.39.155.106",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 10",
	m_strServerIPAddr		= "211.39.155.107",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 11",
	m_strServerIPAddr		= "211.39.155.108",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 12",
	m_strServerIPAddr		= "211.39.155.109",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 13",
	m_strServerIPAddr		= "211.39.155.110",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 14",
	m_strServerIPAddr		= "211.39.155.90",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 15",
	m_strServerIPAddr		= "211.39.155.88",
	m_iServerGroupID		= 1,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Soles Game 16",
	m_strServerIPAddr		= "211.39.155.89",
	m_iServerGroupID		= 1,
}

------------------------------------------------------------------ 가이아

ConsoleImp:AddServerInfo
{
	m_bIsLoginServer		= TRUE,
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 0,
	m_iYesterdayLineColorID = 0,
	m_strServerName			= "Login & Channel",
	m_strServerIPAddr		= "211.39.155.76",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 01",
	m_strServerIPAddr		= "211.39.155.77",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 02",
	m_strServerIPAddr		= "211.39.155.78",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 03",
	m_strServerIPAddr		= "211.39.155.79",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 04",
	m_strServerIPAddr		= "211.39.155.80",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 05",
	m_strServerIPAddr		= "211.39.155.81",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 06",
	m_strServerIPAddr		= "211.39.155.82",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 07",
	m_strServerIPAddr		= "211.39.155.83",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 08",
	m_strServerIPAddr		= "211.39.155.84",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 09",
	m_strServerIPAddr		= "211.39.155.85",
	m_iServerGroupID		= 2,
}

ConsoleImp:AddServerInfo
{
	m_iLogDsnID				= 2,
	m_iTodayLineColorID		= 7,
	m_iYesterdayLineColorID = 4,
	m_strServerName			= "Gaia Game 10",
	m_strServerIPAddr		= "211.39.155.86",
	m_iServerGroupID		= 2,
}
