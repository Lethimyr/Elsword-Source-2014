#pragma once



#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

// thread safe 한 버전에서는 게임 내에 CKTDXLog 개체가 1개만 존재해야 한다.
#define KTDXLOG_MAX_THREAD_NUM 10

#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

class CKTDXLog
{
	public:
		CKTDXLog(void);
		~CKTDXLog(void);

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
        // 특정 thread 에서 로그를 남기려면 먼저 이 함수를 호출해야 한다, robobeg, 2014-01-14
        bool    RegisterCurrentThread();
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

        // 이 함수들은 thread safe 하게 구현되었다, robobeg, 2014-01-14
		void PushStateLog( const WCHAR* statLog );
		void PushErrorLog( int errNum, const WCHAR* errorLog );

		void PushStateLog( const char* statLog );
		void PushErrorLog( int errNum );
		void PushErrorLog( int errNum, const char* errorLog );

		void SetLastErrorLog( const char* statLog );
		void SetLastErrorLog( const WCHAR* statLog );
        const char *GetLastErrorLog();


        // 이 두 함수는 thread safe 하지 않다. 프로그램 종료시 모든 thread 가 작동을 멈추었다고 가정한다.
		void StateLogToFile( const char* fileName );
		void ErrorLogToFile( const char* fileName, const char* systemInfo = NULL );
        
        //int GetStateLogSize() { return m_StateLog.size(); }
        //int GetErrorLogSize() { return m_ErrorLog.size(); }
        //const char *GetStateLogStr(int inx) { return m_StateLog[inx].c_str(); }
        //const char *GetErrorLogStr(int inx) { return m_ErrorLog[inx].c_str(); }


#if 0
		// 사용하지 않으므로 제거한다.
		void EnQueLastErrorLog( const WCHAR* errorLog );
		const char *DeQueLastErrorLog();
#endif

	private:

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

        struct  KTLSData
        {
            volatile DWORD      m_dwThreadId;
		    std::deque<std::pair<DWORD,string> >	m_StateLog;
		    std::deque<std::pair<DWORD,string> >	m_ErrorLog;
		    string				m_LastErrorLog;

            KTLSData() : m_dwThreadId(0) {}
        };
   
        KTLSData            m_aTLSData[KTDXLOG_MAX_THREAD_NUM];
		__declspec(thread) static KTLSData*
                            ms_pTLSData;
        volatile DWORD      m_dwTimestamp;

#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

		vector<string>		m_StateLog;
		vector<string>		m_ErrorLog;
		string				m_LastErrorLog;

		//std::queue<wstring>	m_queLastErrorLog;

#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
};

extern CKTDXLog g_CKTDXLog;
