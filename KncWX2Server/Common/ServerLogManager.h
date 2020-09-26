#pragma once

#include <ToString.h>
#include <KNCSingleton.h>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <atltime.h>
#include <ObjectPool/BoostAllocator.h>
#include <Thread/Thread.h>
#include <dbg/dbg.hpp>

#include "KncSmartPtr.h"

//////////////////////////////////////////////////////////////////////////
// macro
#define START_LOG_END_T( type, data ) \
{ \
	KLogStream kLog; \
	kLog.StartLog( serverlog::type, data, __WFILE__, __WFUNCTION__ ); \
} \

#define START_LOG_T( type, data ) \
{ \
	KLogStream kLog; \
	kLog.StartLog( serverlog::type, data, __WFILE__, __WFUNCTION__ ) \

#define BUILD_LOG_T( data ) \
	L"    "; kLog.BuildLog( data, L#data ) \

#define BUILD_LOG_END_T( data ) \
	L"    "; kLog.BuildLog( data, L#data ); \
} \

#define END_LOG_T \
	L"    "; kLog.EndLog( __WFILE__, __LINE__ ); \
} \

#define BUILD_LOGc_T( data ) \
	L"    "; kLog.BuildLog( static_cast<int>(data), L#data ) \

#define BUILD_LOGc_END_T( data ) \
	L"    "; kLog.BuildLog( static_cast<int>(data), L#data ); \
} \

#define BUILD_LOGtm_T( data ) \
	L"    "; kLog.BuildLog( data.Format( KNC_TIME_FORMAT ), L#data ) \

#define BUILD_LOGtm_END_T( data ) \
	L"    "; kLog.BuildLog( data.Format( KNC_TIME_FORMAT ), L#data ); \
} \


//////////////////////////////////////////////////////////////////////////
// log level
namespace serverlog
{
	enum LOG_LEVEL
	{
		CLOG = 0,
		CLOG2,
		CWARN,
		CERR,
		COUT,
		COUT2,
	};

	const wchar_t* GetLogLevelStr( LOG_LEVEL eLogLevel );
}

//////////////////////////////////////////////////////////////////////////
// log data
#define KLogInfoPtr KLogInfo*
#undef KLogInfoPtr
SmartPointer( KLogInfo );

#pragma pack( push, 1 )

BOOST_ALOC_SYNC_CLASS( KLogInfo )
{
public:
	char                        m_cLogLevel;
	__int64						m_tRegDate;
	std::wstring                m_wstrFunctionName;
	std::wstring                m_wstrFileName;
	int                         m_iFileLine;
	std::vector< std::wstring > m_vecContent;

	KLogInfo()
	{
		m_cLogLevel = 0;
		m_tRegDate	= 0;
		m_iFileLine	= 0;
	}
};

#pragma pack( pop )


//////////////////////////////////////////////////////////////////////////
// log stream
class KLogStream
{
public:
	KLogStream();
	~KLogStream();

	template< typename T >
	KLogStream& StartLog( serverlog::LOG_LEVEL eLogLevel, T data, const wchar_t* pStrFileName, const wchar_t* pStrFunctionName );

	KLogStream& operator<< ( const wchar_t* pStrTab )
	{
		// 아무것도 안함.
		return *this;
	}

	template< typename T >
	KLogStream& BuildLog( T data, const wchar_t* pStrDataName );

	void EndLog( const wchar_t* pStrFileName, int iStrLine );

private:
	KLogInfo m_kLogInfo;
};

template< typename T >
KLogStream& KLogStream::StartLog( serverlog::LOG_LEVEL eLogLevel, T data, const wchar_t* pStrFileName, const wchar_t* pStrFunctionName )
{
	std::wstringstream wstrStream;
	wstrStream << serverlog::GetLogLevelStr( eLogLevel ) << L" : " << pStrFunctionName << L"(), " <<(LPCTSTR)CTime::GetCurrentTime().Format( KNC_TIME_FORMAT ) << L", " << data << std::endl;

	// 로그 기본 데이터를 저장하자!
	m_kLogInfo.m_cLogLevel = eLogLevel;
	m_kLogInfo.m_tRegDate = CTime::GetCurrentTime().GetTime();
	m_kLogInfo.m_wstrFileName = pStrFileName;
	m_kLogInfo.m_wstrFunctionName = pStrFunctionName;
	m_kLogInfo.m_wstrFunctionName += L"()";

	// 로그파일에 출력할 정보 저장
	m_kLogInfo.m_vecContent.push_back( wstrStream.str() );
	return *this;
}

template< typename T >
KLogStream& KLogStream::BuildLog( T data, const wchar_t* pStrDataName )
{
	std::wstringstream wstrStream;
	wstrStream << L"    " << pStrDataName << L" : " << data << std::endl;

	m_kLogInfo.m_vecContent.push_back( wstrStream.str() );
	return *this;
}


//////////////////////////////////////////////////////////////////////////
// server log manager
class KServerLogManager : public KThread
{
public:
	static KServerLogManager& GetInstance() { return ms_selfInstance; }
protected:
	static KServerLogManager ms_selfInstance; // 서버 프로세스가 죽을때까지 살아있어야 하므로 static으로 두었음.

public:
    KServerLogManager();
    virtual ~KServerLogManager();
    
	DeclToStringW;
	DeclDump;

	void QueueingToLog( const KLogInfoPtr& spLogInfo );
	bool GetLogInfo( KLogInfoPtr& spLogInfo );

	virtual void Run();
	void Loop();

	void Shutdown();

private:
	bool						m_bIsShutDown;
	std::queue< KLogInfoPtr >	m_LogDataQueue;
	mutable KncCriticalSection	m_csLogDataQueue;
};



