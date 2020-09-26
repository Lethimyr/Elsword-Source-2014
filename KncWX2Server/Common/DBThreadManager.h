#pragma once

#include "ThreadManager.h"
//{{ 2009. 6. 15  √÷¿∞ªÁ	sec string
#include "SecurityString.h"
//}}

#define KDBThreadManagerPtr KDBThreadManager*
#undef KDBThreadManagerPtr
SmartPointer( KDBThreadManager );

class KDBThreadManager : public KThreadManager
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KDBThreadManager() { m_bDBConnStr = false; }
    virtual ~KDBThreadManager() {}

    int GetDBConnectionID() { return m_iDBConnectionID; }
    void SetDBConnectionID( int iDBConnectionID ) { m_iDBConnectionID = iDBConnectionID; }

	// FileDSN
    std::wstring GetFileDSN() { return m_wstrDSN; }
    //void SetFileDSN( const char* szDSN );
    void SetFileDSN( const std::string& strDSN_ );


	// DB Connection String
	KSecurityString< std::wstring > GetDBConnString() { return m_SecConnStr; }
	void SetDBConnString( const std::wstring& wstrDBConnStr, KClearString< std::wstring > wstrID, KClearString< std::wstring > wstrPW );

protected:
    // derived from KThreadManager;
    virtual KThread* CreateThread();

public:

protected:
    int								m_iDBConnectionID;
	bool							m_bDBConnStr;
    std::wstring					m_wstrDSN;
	//{{ 2009. 6. 15  √÷¿∞ªÁ	sec string
	KSecurityString< std::wstring > m_SecConnStr;
	//}}
};

DeclOstmOperatorW( KDBThreadManager );
