#include "StdAfx.h"
#include ".\ktdxlog.h"

CKTDXLog g_CKTDXLog;

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
__declspec(thread) /*static*/ 
CKTDXLog::KTLSData* CKTDXLog::ms_pTLSData = NULL;
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY


CKTDXLog::CKTDXLog(void)
{
#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
    m_dwTimestamp = 0;
    RegisterCurrentThread();
#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
	m_LastErrorLog = "";
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
}

CKTDXLog::~CKTDXLog(void)
{
#ifndef X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
	m_StateLog.clear();
	m_ErrorLog.clear();

	//for(UINT i=0; i<m_queLastErrorLog.size(); ++i)
	//	m_queLastErrorLog.pop();	
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
}

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
bool    CKTDXLog::RegisterCurrentThread()
{
    if ( ms_pTLSData != NULL )
        return true;

	DWORD	dwCurrentThreadId = ::GetCurrentThreadId();
	for( int i = 0; i < KTDXLOG_MAX_THREAD_NUM; i++ )
	{
		DWORD	dwOldValue = ::InterlockedCompareExchange( &m_aTLSData[i].m_dwThreadId, dwCurrentThreadId, 0 );
		if ( dwOldValue == 0 )
        {
            ms_pTLSData = &m_aTLSData[i];
			return true;
        }
	}
	return false;
}
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
const char* CKTDXLog::GetLastErrorLog() 
{ 
    if ( ms_pTLSData != NULL )
    {
        return ms_pTLSData->m_LastErrorLog.c_str();
    }
    return "";
}
#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
const char* CKTDXLog::GetLastErrorLog() 
{ 
    return m_LastErrorLog.c_str(); 
}
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY


void CKTDXLog::PushStateLog( const WCHAR* statLog )
{
	string logstr;
	ConvertWCHARToChar( logstr, statLog );
	PushStateLog( logstr.c_str() );
}

void CKTDXLog::PushErrorLog( int errNum, const WCHAR* errorLog )
{
	string logstr;
	ConvertWCHARToChar( logstr, errorLog );
	PushErrorLog( errNum, logstr.c_str() );
}

void CKTDXLog::PushStateLog( const char* statLog )
{
	time_t t0 = time(0);
	stringstream strstm;
#ifdef	CONVERSION_VS
	struct tm t;
	localtime_s( &t, &t0);
	strstm << 1900+t.tm_year << "/" << 1+t.tm_mon << "/" << t.tm_mday << ", " << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << " > " << statLog;
#else	CONVERSION_VS
	tm* t = localtime(&t0);
    if ( t != NULL )
    {
	    strstm << 1900+t->tm_year << "/" << 1+t->tm_mon << "/" << t->tm_mday << ", " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << " > " << statLog;
    }
#endif	CONVERSION_VS
	
#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
    if ( ms_pTLSData != NULL )
    {
        DWORD   dwTimestamp = ::InterlockedIncrement( &m_dwTimestamp );
        ms_pTLSData->m_StateLog.push_back( std::pair<DWORD,std::string>(dwTimestamp,strstm.str()) );
    }
#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
	m_StateLog.push_back( strstm.str().c_str() );
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
}

void CKTDXLog::PushErrorLog( int errNum )
{
	time_t t0 = time(0);
	stringstream strstm;
#ifdef	CONVERSION_VS
	struct tm t;
	localtime_s( &t, &t0);
	strstm << 1900+t.tm_year << "/" << 1+t.tm_mon << "/" << t.tm_mday << ", " << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << " > " << "ErrorEnum(" << errNum << ")";
#else	CONVERSION_VS
	tm* t = localtime(&t0);
    if ( t != NULL )
    {
	    strstm << 1900+t->tm_year << "/" << 1+t->tm_mon << "/" << t->tm_mday << ", " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << " > " << "ErrorEnum(" << errNum << ")";
    }
#endif	CONVERSION_VS

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
    if ( ms_pTLSData != NULL )
    {
        DWORD   dwTimestamp = ::InterlockedIncrement( &m_dwTimestamp );
        ms_pTLSData->m_ErrorLog.push_back( std::pair<DWORD,std::string>( dwTimestamp, strstm.str() ) );
    }
#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
	m_ErrorLog.push_back( strstm.str().c_str() );
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
}

void CKTDXLog::PushErrorLog( int errNum, const char* errorLog )
{
	time_t t0 = time(0);
	stringstream strstm;
#ifdef	CONVERSION_VS
	struct tm t;
	localtime_s( &t, &t0);
	strstm << 1900+t.tm_year << "/" << 1+t.tm_mon << "/" << t.tm_mday << ", " << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << " > " << "ErrorEnum(" << errNum << "),\t" << "ErrorMsg(\"" << errorLog << "\")";
#else	CONVERSION_VS
	tm* t = localtime(&t0);
    if ( t != NULL )
    {
	    strstm << 1900+t->tm_year << "/" << 1+t->tm_mon << "/" << t->tm_mday << ", " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << " > " << "ErrorEnum(" << errNum << "),\t" << "ErrorMsg(\"" << errorLog << "\")";
    }
#endif	CONVERSION_VS

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
    if ( ms_pTLSData != NULL )
    {
        DWORD   dwTimestamp = ::InterlockedIncrement( &m_dwTimestamp );
        ms_pTLSData->m_ErrorLog.push_back( std::pair<DWORD,std::string>( dwTimestamp, strstm.str() ) );
    }
#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
	m_ErrorLog.push_back( strstm.str().c_str() );
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
}

void CKTDXLog::SetLastErrorLog( const char* statLog )
{ 
#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
    if ( ms_pTLSData != NULL )
    {
        ms_pTLSData->m_LastErrorLog = ( statLog != NULL ) ? statLog : "";
    }
#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
    m_LastErrorLog = statLog; 
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
}


void CKTDXLog::SetLastErrorLog( const WCHAR* statLog )
{ 
#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
    std::string str;
    ConvertWCHARToChar( str, statLog ); 
    SetLastErrorLog( str.c_str() );
#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

	ConvertWCHARToChar( m_LastErrorLog, statLog ); 
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
}

void CKTDXLog::StateLogToFile( const char* fileName )
{
#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

    if ( fileName == NULL || fileName[0] == NULL )
        return;

    int aIndex[KTDXLOG_MAX_THREAD_NUM];
    memset( aIndex, 0, sizeof(aIndex) );
    while( 1 )
    {
        int     iSmallestThread = -1;
        DWORD   dwSmallestTimestamp = 0;
        for( int iThread = 0; iThread < KTDXLOG_MAX_THREAD_NUM; iThread++ )
        {
            KTLSData& kTLSData = m_aTLSData[ iThread ];
            if ( aIndex[ iThread ] < (int) kTLSData.m_StateLog.size() )
            {
                if ( iSmallestThread < 0 
                    || kTLSData.m_StateLog[ aIndex[ iThread ] ].first < dwSmallestTimestamp )
                {
                    iSmallestThread = iThread;
                    dwSmallestTimestamp = kTLSData.m_StateLog[ aIndex[ iThread ] ].first;
                }
            }
        }
        if ( iSmallestThread < 0 )
            break;
        KTLSData& kTLSData = m_aTLSData[ iSmallestThread ];
        if ( aIndex[ iSmallestThread ] < (int) kTLSData.m_StateLog.size() )
        {
            KLOG(fileName) << "Thread " << iSmallestThread << " : " << kTLSData.m_StateLog[aIndex[ iSmallestThread ]].second.c_str() << fileout;
            aIndex[ iSmallestThread ]++;
        }
    };

#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

	for( int i = 0; i < (int)m_StateLog.size(); i++ )
	{
		KLOG(fileName) << m_StateLog[i].c_str() << fileout;
	}
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
	KLOG(fileName) << "_____LOG_END____" << fileout;
	KLOG(fileName) << "\n\n\n\n\n\n\n" << fileout;


}

void CKTDXLog::ErrorLogToFile( const char* fileName, const char* systemInfo )
{
#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

    if ( fileName == NULL || fileName[0] == NULL )
        return;

    int aIndex[KTDXLOG_MAX_THREAD_NUM];
    memset( aIndex, 0, sizeof(aIndex) );
    while( 1 )
    {
        int     iSmallestThread = -1;
        DWORD   dwSmallestTimestamp = 0;
        for( int iThread = 0; iThread < KTDXLOG_MAX_THREAD_NUM; iThread++ )
        {
            KTLSData& kTLSData = m_aTLSData[ iThread ];
            if ( aIndex[ iThread ] < (int) kTLSData.m_ErrorLog.size() )
            {
                if ( iSmallestThread < 0 
                    || kTLSData.m_ErrorLog[ aIndex[ iThread ] ].first < dwSmallestTimestamp )
                {
                    iSmallestThread = iThread;
                    dwSmallestTimestamp = kTLSData.m_ErrorLog[ aIndex[ iThread ] ].first;
                }
            }
        }
        if ( iSmallestThread < 0 )
            break;
        KTLSData& kTLSData = m_aTLSData[ iSmallestThread ];
        if ( aIndex[ iSmallestThread ] < (int) kTLSData.m_ErrorLog.size() )
        {
            KLOG(fileName) << "Thread " << iSmallestThread << " : " << kTLSData.m_ErrorLog[aIndex[ iSmallestThread ]].second.c_str() << fileout;
            aIndex[ iSmallestThread ]++;
        }
    };

    for( int iThread = 0; iThread < KTDXLOG_MAX_THREAD_NUM; iThread++ )
    {
        KTLSData& kTLSData = m_aTLSData[ iThread ];
        if ( kTLSData.m_LastErrorLog.empty() == false )
        {
	        KLOG(fileName) << "Thread " << iThread << " LastErrorLog : " << kTLSData.m_LastErrorLog.c_str() << fileout;
        }
    }

#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

	for( int i = 0; i < (int)m_ErrorLog.size(); i++ )
	{
		KLOG(fileName) << m_ErrorLog[i].c_str() << fileout;
	}

	KLOG(fileName) << "LastErrorLog : " << m_LastErrorLog.c_str() << fileout;

#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

//#if 0 
//	KLOG(fileName) << "\n" << fileout;
//	for(UINT i=0; i<m_queLastErrorLog.size(); ++i)
//	{
//		KLOG(fileName) << "LastErrorLog(" << i << ") : " << DeQueLastErrorLog() << fileout;
//	}	
//#endif
	KLOG(fileName) << "_____LOG_END____" << fileout;

	if( systemInfo != NULL )
	{
		KLOG(fileName) << "------INFO------" << fileout;
		KLOG(fileName) << systemInfo << fileout;
	}
	
	
	KLOG(fileName) << "\n\n\n\n\n\n\n" << fileout;


}

//#if 0
//void CKTDXLog::EnQueLastErrorLog( const WCHAR* errorLog )
//{
//	if(m_queLastErrorLog.size() > 5)
//	{
//		m_queLastErrorLog.pop();		
//	}
//
//	time_t t0 = time(0);
//
//#ifdef	CONVERSION_VS
//	struct tm t;
//	localtime_s( &t, &t0);
//	wstringstream wstrstm;
//	wstrstm << 1900+t.tm_year << "/" << 1+t.tm_mon << "/" << t.tm_mday << ", " << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << " > " << "ErrorMsg(\"" << errorLog << "\")";
//#else	CONVERSION_VS
//	tm* t = localtime(&t0);
//	wstringstream wstrstm;
//	wstrstm << 1900+t->tm_year << "/" << 1+t->tm_mon << "/" << t->tm_mday << ", " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << " > " << "ErrorMsg(\"" << errorLog << "\")";
//#endif	CONVERSION_VS
//
//	m_queLastErrorLog.push( wstrstm.str().c_str() );
//}
//
//const char* CKTDXLog::DeQueLastErrorLog()
//{
//	if(m_queLastErrorLog.size() > 0)
//	{
//		string strErrorLog = "";
//		wstring wstrErrorLog = m_queLastErrorLog.back();
//        m_queLastErrorLog.pop();
//
//
//		ConvertWCHARToChar( strErrorLog, wstrErrorLog.c_str() ); 
//
//		return strErrorLog.c_str();
//	}
//
//	return NULL;
//}
//#endif


