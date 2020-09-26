#pragma once

// 박교현 : [2010/05/12]		 //	시스템 정보
#ifdef SYS_INFO

#include <Winnls.h>
#include <Iphlpapi.h>

class CX2SystemInfo
{
public:

	struct LanguageInfo
	{
		LANGID							LangID;
		string							Name;
	};

	struct LocaleInfo
	{
		LanguageInfo					PrimaryLanguage;
		map<LANGID, LanguageInfo>		mapSublanguage;
	};

	CX2SystemInfo();
	virtual ~CX2SystemInfo();

	// OS
	const string&				GetOSVersion() const					{ return m_strOSVer; }
	const string&				GetOSName() const						{ return m_strOSName; }
	UINT						GetServicePack() const					{ return m_ServicePack; }

	// Locale
	string						GetLocaleName(DWORD id) const;
	const string&				GetSystemLocale() const					{ return m_strSystemLocale; }
	LANGID						GetSystemPrimaryLocaleID() const		{ return m_SystemPrimaryLocaleID; }
	LANGID						GetSystemSubLocaleID() const			{ return m_SystemSubLocaleID; }
#ifdef ADD_INFO_INPUT_LOCALE
	const vector<string>&		GetInputLocale() const					{ return m_vecInputLocale; }
	const vector<LANGID>&		GetInputLocaleID() const				{ return m_vecInputLocaleID; }
#endif // ADD_INFO_INPUT_LOCALE
#ifdef ADD_INFO_INSTALL_LOCALE
	const string&				GetInstallLocale() const				{ return m_strInstallLocale; }
	LANGID						GetInstallLocaleID() const				{ return m_InstallLocaleID; }
#endif // ADD_INFO_INSTALL_LOCALE
		
public:
#ifdef ADD_INFO_INSTALL_LOCALE
	static LANGID				QueryInstallLocaleID()
	{
		// http://msdn.microsoft.com/en-us/library/dd374098(v=VS.85).aspx
		// HKEY_LOCAL_MACHINE\Nls\Language 에 저장된 레지스트리를 가져옴
		// return GetSystemDefaultUILanguage();			// Win2000 이상

		typedef LANGID (WINAPI * PFNGETSYSTEMDEFAULTUILANGUAGE) (void);
		PFNGETSYSTEMDEFAULTUILANGUAGE pfnGetSystemDefaultUILanguage;
		HINSTANCE hKernel32;
		hKernel32 = GetModuleHandle(_T("kernel32.dll"));
		pfnGetSystemDefaultUILanguage = (PFNGETSYSTEMDEFAULTUILANGUAGE)GetProcAddress(hKernel32,"GetSystemDefaultUILanguage");
		if(pfnGetSystemDefaultUILanguage != NULL)
		{
			return pfnGetSystemDefaultUILanguage();
		}
		else
		{
			// 2000 미만 OS 처리 필요 없음
			/*
			if (GetVersion()&0x80000000)
			{
				// Win98은 레지스트리에서 읽어옴
				LANGID langid;
				HKEY hKey = NULL;
				LONG nResult = RegOpenKeyEx(HKEY_CURRENT_USER,
											_T( "Control Panel\\Desktop\\ResourceLocale" ),
											0, KEY_READ, &hKey);
				if (nResult == ERROR_SUCCESS)
				{
					DWORD dwType;
					TCHAR szValue[16];
					ULONG nBytes = sizeof( szValue );
					nResult = RegQueryValueEx(hKey, NULL, NULL, &dwType, (LPBYTE) szValue, &nBytes );
					if ((nResult == ERROR_SUCCESS) && (dwType == REG_SZ))
					{
						DWORD dwLangID;
						int nFields = _stscanf( szValue, _T( "%x" ), &dwLangID );
						if( nFields == 1 )
						{
							langid = (LANGID) dwLangID;
						}
					}
					RegCloseKey(hKey);
					return langid;
				}
			}
			else
			{
				// NT 4는 ntdll.dll의 버전 리소스 언어로 얻어옴
				// HMODULE hNTDLL = GetModuleHandle( _T( "ntdll.dll" ) );
				// if (hNTDLL != NULL)
				// { 
				//	langid = 0;
				//	EnumResourceLanguages( hNTDLL, RT_VERSION, MAKEINTRESOURCE( 1 ), 
				//							_AfxEnumResLangProc, 
				//							reinterpret_cast< LONG_PTR >( &langid ) ); 
				//	if (langid != 0) 
				//	{ 
				//		TRACE(_T("CMultiLanguage::DetectUILanguage() NT1st/2nd = %04X\n"), langid ); 
				//		return langid;
				//	} 
				// } 
			}*/
		}

		return 0;
	}
#endif // ADD_INFO_INSTALL_LOCALE

#ifdef ADD_INFO_INPUT_LOCALE
	static void				QueryInputLocale(vector<LANGID>& vecInputLocales)
	{
		HKL hkl[256];
		int locale_size = GetKeyboardLayoutList(sizeof(hkl)/sizeof(HKL), hkl);
		for(int i=0; i<locale_size; ++i)
		{
			LANGID lid = (LANGID)((DWORD)hkl[i] & NLS_VALID_LOCALE_MASK);
			vecInputLocales.push_back(lid);
		}
	}
#endif // ADD_INFO_INPUT_LOCALE


private:
	void InitLocaleMap();
	void LoadLanguageInfo(KLuaManager& luaManager, LanguageInfo& lang_info);

	bool InitOSInfo();
	bool InitOSLocale();
#ifdef ADD_INFO_INPUT_LOCALE
	bool InitInputLocale();
#endif // ADD_INFO_INPUT_LOCALE

private:
	// OS
	string								m_strOSVer;
	string								m_strOSName;
	UINT								m_ServicePack;

	// Locale
	LANGID								m_SystemPrimaryLocaleID;
	LANGID								m_SystemSubLocaleID;
	string								m_strSystemLocale;
#ifdef ADD_INFO_INPUT_LOCALE
	vector<string>						m_vecInputLocale;
	vector<LANGID>						m_vecInputLocaleID;
#endif // ADD_INFO_INPUT_LOCALE
#ifdef ADD_INFO_INSTALL_LOCALE
	LANGID								m_InstallLocaleID;
	string								m_strInstallLocale;
#endif // ADD_INFO_INSTALL_LOCALE

	std::multimap<LANGID, LocaleInfo>	m_mapLocale;

	// Network
	string								m_strInternalIP;
	string								m_strExternalIP;

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
public :
	INT64 GetMacAddress()
	{
		UINT uiMacAddrLength = 0;
		UCHAR acBuffer[6];

		if( !_GetMacAddress(uiMacAddrLength, acBuffer, 6) )
			return 0;

		INT64 i64MacAddr = 0;
		for (UINT uiIdx = 0; uiIdx < 6; ++uiIdx)
		{
			i64MacAddr = (i64MacAddr << 8) + acBuffer[uiIdx];
		}

		return i64MacAddr;
	}

	TCHAR* GetCpuString() 
	{
		CCpusInfo cpus;
		_GetCpusInfo( cpus );

		CCpuInfo cpu;
		_GetCpuInfo( 0, cpu );

		_stprintf(m_strCPU, _T("[Core]%d %s (%sMHz)"), cpus.m_dwNumCpus, cpu.m_szCpuDescription, cpu.m_szCpuSpeed);

		return m_strCPU;
	}

	TCHAR* GetMemString() 
	{
		CMemInfo mem;
		_GetMemInfo( mem );

		_stprintf(m_strMemory, _T("%sMB"), mem.m_szTotalPhys);

		return m_strMemory;
	}

	TCHAR* GetOsString() 
	{
		COsInfo os;
		_GetOsInfo( os );

		_stprintf(m_strCPU, _T("%s %s %s"), os.m_pszWinVersion, os.m_szSPVersion, os.m_szBuildNumber );

		return m_strCPU;
	}

	TCHAR* GetDisplayString(LPDIRECT3D9 lpD3D = NULL)
	{
		CDisplayInfo rVideoInfo;
		if( _GetDisplayInfo( lpD3D, rVideoInfo ) )
		{
			_stprintf(m_strGraphic, _T("%s %s"), rVideoInfo.m_szGraphicCardName, rVideoInfo.m_szVersion);
		}
		else _stprintf(m_strGraphic, _T("Unknown") );

		return m_strGraphic;
	}

private:
	/// CPUs information.
	struct CCpusInfo
	{
		/// Initialize the object.
		CCpusInfo(void) : m_pszCpuArch(NULL), m_dwNumCpus(0)
		{

		}

		/// Processors architecture.
		PCTSTR m_pszCpuArch;
		/// Number of processors.
		DWORD m_dwNumCpus;
	};

	/// CPU information.
	struct CCpuInfo
	{
		/// Initialize the object.
		CCpuInfo(void)
		{
			*m_szCpuId = _T('\0');
			*m_szCpuSpeed = _T('\0');
			*m_szCpuDescription = _T('\0');
		}

		/// Processor identifier.
		TCHAR m_szCpuId[128];
		/// Processor speed.
		TCHAR m_szCpuSpeed[16];
		/// Processor description.
		TCHAR m_szCpuDescription[128];
	};

	/// Memory information.
	struct CMemInfo
	{
		/// Initialize the object.
		CMemInfo(void)
		{
			*m_szMemoryLoad = _T('\0');
			*m_szTotalPhys = _T('\0');
			*m_szAvailPhys = _T('\0');
			*m_szTotalPageFile = _T('\0');
			*m_szAvailPageFile = _T('\0');
		}

		/// Current memory load
		TCHAR m_szMemoryLoad[8];
		/// Total physical memory
		TCHAR m_szTotalPhys[16];
		/// Available physical memory
		TCHAR m_szAvailPhys[16];
		/// Total page file memory
		TCHAR m_szTotalPageFile[16];
		/// Available page file memory
		TCHAR m_szAvailPageFile[16];
	};

	/// Operating system information.
	struct COsInfo
	{
		/// Initialize the object.
		COsInfo(void) : m_pszWinVersion(NULL)
		{
			*m_szSPVersion = _T('\0');
			*m_szBuildNumber = _T('\0');
		}

		/// Windows version.
		PCTSTR m_pszWinVersion;
		/// Service pack version.
		TCHAR m_szSPVersion[128];
		/// Build number.
		TCHAR m_szBuildNumber[16];
	};

	struct CDisplayInfo
	{
		CDisplayInfo(void)
		{
			*m_szGraphicCardName = _T('\0');
			*m_szVersion = _T('\0');
		}

		TCHAR m_szGraphicCardName[128];
		TCHAR m_szVersion[16];
	};

	void _GetCpusInfo(CCpusInfo& rCpusInfo);
	bool _GetCpuInfo(DWORD dwCpuNum, CCpuInfo& rCpuInfo);
	void _GetMemInfo(CMemInfo& rMemInfo);
	void _GetOsInfo(COsInfo& rOsInfo);
	bool _GetDisplayInfo(LPDIRECT3D9 lpD3D, CDisplayInfo& rVideoInfo);
	bool _GetMacAddress(UINT& uiMacAddressLength, UCHAR* pucBuffer, UINT uiBufferSize);

	TCHAR m_strCPU[256];
	TCHAR m_strMemory[256];

	TCHAR m_strOS[256];
	TCHAR m_strGraphic[256];
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
};

#endif // SYS_INFO
