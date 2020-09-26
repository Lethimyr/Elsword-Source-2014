#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
#include "odbc/DBUtil.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY	// 빌드 오류로 해외팀 추가

class KDBConnectionManager
{
public:
    KDBConnectionManager();
    virtual ~KDBConnectionManager(void);

	bool GetDBConnectionInfoFromManagerDB( IN const int iServerType, IN const wchar_t* szConnStr, IN const bool bDBConnStr, OUT std::vector< KDBConnectionInfo >& vecDBConnectionInfo );
	bool DecryptDBString( IN const std::wstring& wstrDBString, OUT std::wstring& wstrDecryptString );

protected:
	KODBC			m_kODBC;
};

#endif SERV_DB_CONNECTION_SECURITY	// 빌드 오류로 해외팀 추가
//}}