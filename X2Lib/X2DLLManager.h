#pragma once

//{{ 박교현 : [2010/05/05]		 //	안티핵 코드
#ifdef DLL_MANAGER

#include <vector>

class CX2DLLManager
{
	DeclareSingleton( CX2DLLManager );

public:
	class DLLEvent
	{
	public:
		enum DLL_EVENT_TYPE			{ DET_UNKNOWN, DET_DLL_LOAD, DET_DLL_INJECTION, };
		enum INJECTION_METHOD_TYPE	{	HMT_UNKNOWN, 
										HMT_CREATEREMOTETHREAD, 
										HMT_SETWINDOWHOOKEX, 
										HMT_SETTHREADCONTEXT, 
										HMT_NEWTHEREADDLL, 
										HMT_KERNELMODEMEMORY, 
										HMT_BADCODEPTR, 
										HMT_VERIFIEDDLL, 
									};

		DLLEvent(DLL_EVENT_TYPE EventType, INJECTION_METHOD_TYPE MethodType, const wstring& FileName, PVOID pCaller);

		DLL_EVENT_TYPE			GetEventType() const	{ return m_EventType; }
		INJECTION_METHOD_TYPE	GetMethodType() const	{ return m_MethodType; }
		const wstring&			GetDLLFileName() const	{ return m_DLLFileName; }
		PVOID					GetCallerAddr() const	{ return m_pCaller; }

		bool					operator==(const DLLEvent& event) const;

	private:
		DLL_EVENT_TYPE			m_EventType;
		INJECTION_METHOD_TYPE	m_MethodType;
		wstring					m_DLLFileName;
		PVOID					m_pCaller;
	};

	class DLLEventListener
	{
	public:
		virtual void OnLoadDLLEvent(const DLLEvent& event)			{}
		virtual void OnInverifiedDLLEvent(const DLLEvent& event)	{}
	};

	CX2DLLManager();
	virtual ~CX2DLLManager();

	bool Init();
	void Final();

#ifdef FORCE_FREE_MODULE
	void ForceFree();	
#endif
#ifdef FREE_MODULE
	void ForceFreeModule(wstring wstrFreeModuleName);
#endif

	
	void LoadVerifiedDLLLists();
	bool IsVerifiedDLL(const wstring& pFileName) const;

	void AddListener(DLLEventListener* pListener);
	void RemoveListener(DLLEventListener* pListener);
	void RemoveAllListener();

	void ProcessEvent(const DLLEvent& event);

	void ReportEvent();
	wstring GetReportEvent();
#ifdef CHECK_FROM_DLLMANAGER
	bool FindModule(const wstring &wstrModuleName);
	bool FindModuleIn(const wstring &wstrModuleName);
#endif

	const char* GetPreLoadedDllList() { return m_PreLoadedDllLIst.c_str(); }

private:
	bool CallerVerificationStart();
	void CallerVerificationStop();

	bool HookInjectionApi( LPBYTE TargetFunction, LPBYTE Trampoline, LPVOID DetouredFunction, DWORD MinimumSize );
	void UnHookInjectionApi( LPBYTE TargetFunction, LPBYTE Trampoline, DWORD MinimumSize );

#ifdef CHECK_FROM_DLLMANAGER
	void SetDllCount(int iVal) { m_nOldDLLEventCount = iVal; }
#endif

private:
	std::vector<DLLEvent>					m_vecDLLEvent;
	std::vector<DLLEventListener*>			m_vecDLLEventListeners;
	std::set<wstring>						m_vecExcludedDlls;

	bool									m_bHook;

#ifdef CHECK_FROM_DLLMANAGER
	int m_nOldDLLEventCount;
#endif

	string							m_PreLoadedDllLIst;
};

DefSingletonInline( CX2DLLManager );

#endif //DLL_MANAGER
//}} 박교현 : [2010/05/05]