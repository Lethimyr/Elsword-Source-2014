#pragma once

#include "DBThreadManager.h"
#include "ToString.h"
#include <KNCSingleton.h>

class KDBLayer
{
    DeclToStringW;
    DeclDump;
    DeclareSingleton( KDBLayer );

protected:
    KDBLayer(void);

public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum
    {
#   include "DBConnectionInfo_def.h"
    };

    virtual ~KDBLayer(void) {}

    virtual void RegToLua();
    bool AddDBConnectionInfo_LUA( int iDBConnection, const char* szDSN, int nThread, bool bDBConnStr );
	//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY
	bool AddDBConnectionByString( IN const int iDBType, IN const std::wstring& wstrDBConnectionString, IN const int iThreadCount );
	bool SetManagerDBConnectionInfo_LUA( int iServerClass, const char* szDSN, bool bDBConnStr );
#endif SERV_DB_CONNECTION_SECURITY
	//}}
	void SetSMSEnable_LUA( bool bEnable );

    void Init();
    void ShutDown( DWORD dwWaitTime );   // 종료 대기시간을 밀리세컨드 받는다.
    int GetTotalEventNum();     // 모든 db thread mgr 들이 가진 event 수를 리턴 (종료처리시 쓰임.)

    void QueueingEvent( const KEventPtr& spEvent );

	//{{ 2009. 12. 15  최육사	서버관리
	virtual void DumpToLogFile();
	//}}

protected:
    inline int GetTotalThreadNum();    // 모든 db thread mgr 들이 가진 thread 수를 리턴 (종료처리시 쓰임.)

public:
    static const char*                      ms_szDBConnection[];

protected:
    std::map< int, KDBThreadManagerPtr >    m_mapDBAgent;
	bool									m_bSMSEnable;	//잘못된 사용으로 인한 문제발생시 사용될수 있어서..
	//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY
	boost::shared_ptr< void >				m_spEventAck;
#endif SERV_DB_CONNECTION_SECURITY
	//}}
};

DefSingletonInline( KDBLayer );
