#include "StdAfx.h"
#include "X2SystemInfo.h"

#ifdef SYS_INFO

#include <Winnls.h>

CX2SystemInfo::CX2SystemInfo()
{
	m_SystemPrimaryLocaleID = 0;
	m_SystemSubLocaleID = 0;
#ifdef ADD_INFO_INSTALL_LOCALE
	m_InstallLocaleID = 0;
#endif // ADD_INFO_INSTALL_LOCALE

	InitLocaleMap();

	InitOSInfo();
	InitOSLocale();
#ifdef ADD_INFO_INPUT_LOCALE
	InitInputLocale();
#endif //ADD_INFO_INPUT_LOCALE
}

CX2SystemInfo::~CX2SystemInfo()
{
}

void CX2SystemInfo::InitLocaleMap()
{
	// http://msdn.microsoft.com/en-us/library/dd318693(v=VS.85).aspx

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if(  g_pKTDXApp->LoadAndDoMemory( &luaManager, L"LocaleInfo.lua" ) == false )
	{
		return;
	}

	if( luaManager.BeginTable( "PRIMARY_LANGUAGE" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			LocaleInfo linfo;
			LoadLanguageInfo(luaManager, linfo.PrimaryLanguage);

			if( luaManager.BeginTable( "SUB_LANGUAGE" ) == true )
			{
				int sublang_index = 1;
				while( luaManager.BeginTable( sublang_index ) == true )
				{
					LanguageInfo sublang_info;
					LoadLanguageInfo(luaManager, sublang_info);
					linfo.mapSublanguage.insert( std::pair<LANGID, LanguageInfo>(sublang_info.LangID, sublang_info) );

					luaManager.EndTable();
					++sublang_index;
				}
				luaManager.EndTable();
			}

			m_mapLocale.insert( std::pair<LANGID, LocaleInfo>(linfo.PrimaryLanguage.LangID, linfo) );

			luaManager.EndTable();
			++index;
		}
		luaManager.EndTable();
	}
} 

void CX2SystemInfo::LoadLanguageInfo(KLuaManager& luaManager, LanguageInfo& lang_info)
{
	LUA_GET_VALUE( luaManager,		"LANGID",			lang_info.LangID,		0		);
	LUA_GET_VALUE_UTF8( luaManager,	"NAME",				lang_info.Name,	""		);
}

string CX2SystemInfo::GetLocaleName(DWORD id) const
{
	LANGID PrimaryLocaleID = PRIMARYLANGID(id);
	LANGID SystemSubLocaleID = SUBLANGID(id);

	string strPrimaryLang, strSubLang;

	typedef std::multimap<LANGID, LocaleInfo> LOCALEMAP;
	std::pair<LOCALEMAP::const_iterator, LOCALEMAP::const_iterator> iter_range;
	iter_range = m_mapLocale.equal_range(PrimaryLocaleID);
	for(LOCALEMAP::const_iterator iter = iter_range.first; iter != iter_range.second; ++iter)
	{
		map<LANGID, LanguageInfo>::const_iterator sublang_iter = iter->second.mapSublanguage.find(SystemSubLocaleID);
		if(sublang_iter != iter->second.mapSublanguage.end())
		{
			strPrimaryLang = iter->second.PrimaryLanguage.Name;
			strSubLang = sublang_iter->second.Name;
			break;
		}
	}

	string strInputLocale = strPrimaryLang;
	if(!strInputLocale.empty() && !strSubLang.empty())
		strInputLocale += " - ";
	if(!strSubLang.empty())
		strInputLocale += strSubLang;

	return strInputLocale;
}

bool CX2SystemInfo::InitOSInfo()
{
	// http://msdn.microsoft.com/en-us/library/ms724833

	OSVERSIONINFOEXW osinfo;
	ZeroMemory(&osinfo, sizeof(OSVERSIONINFOEXW));
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

	typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);  
	PGNSI pGNSI;  
	SYSTEM_INFO sysinfo;
	ZeroMemory(&sysinfo, sizeof(SYSTEM_INFO)); 
	pGNSI = (PGNSI) GetProcAddress(  
		GetModuleHandle(TEXT("kernel32.dll")),   
		"GetNativeSystemInfo");  
	if(NULL != pGNSI)  
		pGNSI(&sysinfo);  
	else 
		GetSystemInfo(&sysinfo);  

	if( !(GetVersionEx((OSVERSIONINFO *)&osinfo)) )  
		return false;

	// 버젼
	// Major.Minor.BuildNumber
	m_strOSVer = boost::str(boost::format("%1%.%2%.%3%") % osinfo.dwMajorVersion % osinfo.dwMinorVersion % osinfo.dwBuildNumber );

	// 이름
	if(osinfo.dwMajorVersion == 6)
	{
		if(osinfo.dwMinorVersion == 1)
		{
			if(osinfo.wProductType == VER_NT_WORKSTATION)
				m_strOSName = "Windows 7";
			else
				m_strOSName = "Windows Server 2008 R2";
		}
		else if(osinfo.dwMinorVersion == 0)
		{
			if(osinfo.wProductType == VER_NT_WORKSTATION)
				m_strOSName = "Windows Vista";
			else
				m_strOSName = "Windows Server 2008";
		}
	}
	else if(osinfo.dwMajorVersion == 5)
	{
		if(osinfo.dwMinorVersion == 2)
		{
			if(GetSystemMetrics(89/*SM_SERVERR2*/) != 0)
				m_strOSName = "Windows Server 2003 R2";
			else if(osinfo.wSuiteMask & 0x00008000/*VER_SUITE_WH_SERVER*/)
				m_strOSName = "Windows Home Server";
			else if(GetSystemMetrics(89/*SM_SERVERR2*/) == 0)
				m_strOSName = "Windows Server 2003";
			else if((osinfo.wProductType == VER_NT_WORKSTATION) 
				&& (sysinfo.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64))
				m_strOSName = "Windows XP Professional x64 Edition";
		}
		else if(osinfo.dwMinorVersion == 1)
		{
			m_strOSName = "Windows XP";
		}
		else if(osinfo.dwMinorVersion == 0)
		{
			m_strOSName = "Windows 2000";
		}
	}
	else
	{
		m_strOSName = "Unknown Version";
	}

	// 서비스팩
	m_ServicePack = osinfo.wServicePackMajor;

	return true;
}

bool CX2SystemInfo::InitOSLocale()
{
	LANGID lid = GetSystemDefaultLangID();
	m_SystemPrimaryLocaleID = PRIMARYLANGID(lid);
	m_SystemSubLocaleID = SUBLANGID(lid);
	m_strSystemLocale = GetLocaleName(lid);

#ifdef ADD_INFO_INSTALL_LOCALE
	m_InstallLocaleID = QueryInstallLocaleID();
	m_strInstallLocale = GetLocaleName(m_InstallLocaleID);
#endif // ADD_INFO_INSTALL_LOCALE

	return true;
}

#ifdef ADD_INFO_INPUT_LOCALE
bool CX2SystemInfo::InitInputLocale()
{
	QueryInputLocale(m_vecInputLocaleID);
	BOOST_TEST_FOREACH(LANGID, lid, m_vecInputLocaleID)
	{
		m_vecInputLocale.push_back( boost::str( boost::format("%s") % GetLocaleName(lid).c_str() ) );
		// m_vecInputLocale.push_back( boost::str( boost::format("%s [0x%X]") % GetLocaleName(lid).c_str() % hkl[i] ) );
	}

	return true;
}
#endif // ADD_INFO_INPUT_LOCALE

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
void CX2SystemInfo::_GetCpusInfo(CCpusInfo& rCpusInfo)
{
	static const TCHAR szIntel[] = _T("Intel-x86");
	static const TCHAR szIA64[] = _T("IA-64");
	static const TCHAR szAMD64[] = _T("AMD-x64");
	static const TCHAR szUnknown[] = _T("Unknown");

	rCpusInfo.m_pszCpuArch = szUnknown;
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	switch (si.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		rCpusInfo.m_pszCpuArch = szIntel;
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		rCpusInfo.m_pszCpuArch = szIA64;
		break;
	case PROCESSOR_ARCHITECTURE_AMD64:
		rCpusInfo.m_pszCpuArch = szAMD64;
		break;
	default: // PROCESSOR_ARCHITECTURE_UNKNOWN
		rCpusInfo.m_pszCpuArch = szUnknown;
		break;
	}

	rCpusInfo.m_dwNumCpus = si.dwNumberOfProcessors;
}

bool CX2SystemInfo::_GetCpuInfo(DWORD dwCpuNum, CCpuInfo& rCpuInfo)
{
	static const TCHAR szQuestion[] = _T("?");
	static const TCHAR szUnknown[] = _T("Unknown");

	_tcscpy(rCpuInfo.m_szCpuId, szUnknown);
	_tcscpy(rCpuInfo.m_szCpuSpeed, szQuestion);
	_tcscpy(rCpuInfo.m_szCpuDescription, szUnknown);

	bool bResult = true;

	TCHAR szCentralProcessorPath[MAX_PATH];
	_stprintf(szCentralProcessorPath, _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%lu"), dwCpuNum);
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szCentralProcessorPath, 0l, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwValue, dwValueType, dwValueSize;
		dwValueSize = sizeof(dwValue);
		if (RegQueryValueEx(hKey, _T("~MHz"), NULL, &dwValueType, (PBYTE)&dwValue, &dwValueSize) == ERROR_SUCCESS && dwValueType == REG_DWORD)
			_ultot_s(dwValue, rCpuInfo.m_szCpuSpeed, sizeof(rCpuInfo.m_szCpuSpeed) / sizeof(TCHAR), 10);
		else
			*rCpuInfo.m_szCpuSpeed = _T('\0');

		dwValueSize = sizeof(rCpuInfo.m_szCpuDescription);
		if (RegQueryValueEx(hKey, _T("ProcessorNameString"), NULL, &dwValueType, (PBYTE)rCpuInfo.m_szCpuDescription, &dwValueSize) == ERROR_SUCCESS && dwValueType == REG_SZ)
		{
			// TrimSpaces(rCpuInfo.m_szCpuDescription);
		}
		else
			*rCpuInfo.m_szCpuDescription = _T('\0');

		dwValueSize = sizeof(rCpuInfo.m_szCpuId);
		if (RegQueryValueEx(hKey, _T("Identifier"), NULL, &dwValueType, (PBYTE)rCpuInfo.m_szCpuId, &dwValueSize) == ERROR_SUCCESS && dwValueType == REG_SZ)
		{
			// TrimSpaces(rCpuInfo.m_szCpuId);
		}
		else
			*rCpuInfo.m_szCpuId = _T('\0');

		RegCloseKey(hKey);
	}
	else
		bResult = false;

	return bResult;
}

void CX2SystemInfo::_GetMemInfo(CMemInfo& rMemInfo)
{
	MEMORYSTATUSEX ms;
	ms.dwLength = sizeof (ms);
	GlobalMemoryStatusEx(&ms);
	_ultot(ms.dwMemoryLoad, rMemInfo.m_szMemoryLoad, 10);
	ms.ullTotalPhys = ms.ullTotalPhys / (1024 * 1024);
	_ultot((DWORD)ms.ullTotalPhys, rMemInfo.m_szTotalPhys, 10);
	_ultot((DWORD)ms.ullAvailPhys, rMemInfo.m_szAvailPhys, 10);
	_ultot((DWORD)ms.ullTotalPageFile, rMemInfo.m_szTotalPageFile, 10);
	_ultot((DWORD)ms.ullAvailPageFile, rMemInfo.m_szAvailPageFile, 10);
}

void CX2SystemInfo::_GetOsInfo(COsInfo& rOsInfo)
{
	static const TCHAR szUnknown[] = _T("Unknown");
	static const TCHAR szWindowsNT351[] = _T("Windows NT 3.51");
	static const TCHAR szWindowsNT40[] = _T("Windows NT 4.0");
	static const TCHAR szWindows95[] = _T("Windows 95");
	static const TCHAR szWindows98[] = _T("Windows 98");
	static const TCHAR szWindowsMe[] = _T("Windows Me");
	static const TCHAR szWindows2000[] = _T("Windows 2000");
	static const TCHAR szWindowsXP[] = _T("Windows XP");
	static const TCHAR szWindowsVista[] = _T("Windows Vista");
	static const TCHAR szWindowsServer2003[] = _T("Windows Server 2003");
	static const TCHAR szWindows7[] = _T("Windows 7");

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionEx(&osvi);

	rOsInfo.m_pszWinVersion = szUnknown;
	switch (osvi.dwMajorVersion)
	{
	case 3:
		if (osvi.dwMinorVersion == 51 && osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
			rOsInfo.m_pszWinVersion = szWindowsNT351;
		break;
	case 4:
		switch (osvi.dwMinorVersion)
		{
		case 0:
			if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
				rOsInfo.m_pszWinVersion = szWindowsNT40;
			else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				rOsInfo.m_pszWinVersion = szWindows95;
			break;
		case 10:
			if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				rOsInfo.m_pszWinVersion = szWindows98;
			break;
		case 90:
			if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				rOsInfo.m_pszWinVersion = szWindowsMe;
			break;
		}
		break;
	case 5:
		if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			switch (osvi.dwMinorVersion)
			{
			case 0:
				rOsInfo.m_pszWinVersion = szWindows2000;
				break;
			case 1:
				rOsInfo.m_pszWinVersion = szWindowsXP;
				break;
			case 2:
				rOsInfo.m_pszWinVersion = szWindowsServer2003;
				break;
			}
		}
		break;
	case 6:
		if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			switch (osvi.dwMinorVersion)
			{
			case 0:
				rOsInfo.m_pszWinVersion = szWindowsVista;
				break;
			case 1 :	// by 강민석
				rOsInfo.m_pszWinVersion = szWindows7;
				break;
			}
		}
		break;
	}

	_tcscpy( rOsInfo.m_szSPVersion, osvi.szCSDVersion );
	_ultot(osvi.dwBuildNumber, rOsInfo.m_szBuildNumber, 10);

}

bool CX2SystemInfo::_GetDisplayInfo(LPDIRECT3D9 lpD3D, CDisplayInfo& rVideoInfo)
{
	// 자동초기화 기능 : D3D DLL 초기화
	if( !lpD3D )
		lpD3D = Direct3DCreate9( D3D_SDK_VERSION );

	// D3D 없음
	if( !lpD3D )
		return false;

	D3DADAPTER_IDENTIFIER9 _Identifier; 
	lpD3D->GetAdapterIdentifier(0, 0, &_Identifier); 

#ifdef _UNICODE
	MultiByteToWideChar( CP_ACP , 0 , _Identifier.Description , -1, rVideoInfo.m_szGraphicCardName, 128 ) ; 
#else
	strcpy_s( rVideoInfo.m_szGraphicCardName, _Identifier.Description, 128);
#endif

	// 일단 무시
	_i64tot(_Identifier.DriverVersion.QuadPart, rVideoInfo.m_szVersion, 16);

	return true;
}

bool CX2SystemInfo::_GetMacAddress(UINT& uiMacAddressLength, UCHAR* pucBuffer, UINT uiBufferSize)
{
	UINT	uiPublicIPAddress = 0;
	UINT	uiPublicMacAddressLength = 0;
	UCHAR aucPublicMacAddress[MAX_ADAPTER_ADDRESS_LENGTH] = {0,};

	UINT uiPrivateIPAddress = 0;
	UINT uiPrivateMacAddressLength = 0;
	UCHAR aucPrivateMacAddress[MAX_ADAPTER_ADDRESS_LENGTH] = {0,};

	IP_ADAPTER_INFO* pkAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG	ulAdapterInfoSize = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo(pkAdapterInfo, &ulAdapterInfoSize) == ERROR_BUFFER_OVERFLOW)
	{
		free(pkAdapterInfo);
		pkAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulAdapterInfoSize);
	}

	if (GetAdaptersInfo(pkAdapterInfo, &ulAdapterInfoSize) == NO_ERROR)
	{
		IP_ADAPTER_INFO* pkCurrAdapterInfo = pkAdapterInfo;
		for (; pkCurrAdapterInfo != NULL; pkCurrAdapterInfo = pkCurrAdapterInfo->Next)
		{
			UINT uiIP = inet_addr(pkCurrAdapterInfo->IpAddressList.IpAddress.String);
			if (uiIP == 0 || uiIP == INT_MAX) continue;

			UCHAR ucIP0 = ((UCHAR*)&uiIP)[0];
			if (ucIP0 == 10 || ucIP0 == 192)
			{
				if (uiPrivateIPAddress == 0)
				{
					uiPrivateIPAddress = uiIP;
					uiPrivateMacAddressLength = pkCurrAdapterInfo->AddressLength;
					memcpy_s(aucPrivateMacAddress, MAX_ADAPTER_ADDRESS_LENGTH, pkCurrAdapterInfo->Address, uiPrivateMacAddressLength);
				}
			}
			else
			{
				if (uiPublicIPAddress == 0)
				{
					uiPublicIPAddress = uiIP;
					uiPublicMacAddressLength = pkCurrAdapterInfo->AddressLength;
					memcpy_s(aucPublicMacAddress, MAX_ADAPTER_ADDRESS_LENGTH, pkCurrAdapterInfo->Address, uiPublicMacAddressLength);
				}
			}
		}
	}

	free(pkAdapterInfo);

	//
	if (uiPublicMacAddressLength > 0)
	{
		memcpy_s(pucBuffer, uiBufferSize, aucPublicMacAddress, uiPublicMacAddressLength);
		return true;
	}
	else if (uiPrivateMacAddressLength > 0)
	{
		memcpy_s(pucBuffer, uiBufferSize, aucPrivateMacAddress, uiPrivateMacAddressLength);
		return true;
	}
	return false;
}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#endif SYS_INFO