#include "DBThreadManager.h"
#include "KncSend.h" 

ImplPfID( KDBThreadManager, PI_NULL );    // 각각의 객체마다 다른 값을 가져야 한다. 그냥 사용 x.

NiImplementRTTI( KDBThreadManager, KThreadManager );
ImplToStringW( KDBThreadManager )
{
	stm_ << L"  Name : " << m_wstrDSN << ", Thread : " << GetThreadNum() << ", Event : " << GetQueueSize() << std::endl;
	//{{ 2009. 9. 16  최육사	덤프

	KMaxQueueSize kInfo = GetMaxQueueSize();
	std::wstring wstrRegDate = ( CStringW )( kInfo.m_tRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );
	stm_ << L"  MaxQueueSize : " << kInfo.m_QueueSize << ", RegDate : " << wstrRegDate << std::endl;
	//}}

    return stm_;
}

extern KThread* CreateDBThread( int iDBConnectionID, const wchar_t*, bool );

KThread* KDBThreadManager::CreateThread()
{
	if( m_bDBConnStr )
	{
		KClearString< std::wstring > kDBConnStr = m_SecConnStr.GetClearString();
		return CreateDBThread( m_iDBConnectionID, kDBConnStr.c_str(), m_bDBConnStr );
	}
	else
	{
		return CreateDBThread( m_iDBConnectionID, m_wstrDSN.c_str(), m_bDBConnStr );
	}
}

void KDBThreadManager::SetFileDSN( const std::string& strDSN_ )
{
	m_bDBConnStr = false;

	m_wstrDSN = KncUtil::toWideString( strDSN_ );
}

void KDBThreadManager::SetDBConnString( const std::wstring& wstrDBConnStr, KClearString< std::wstring > wstrID, KClearString< std::wstring > wstrPW )
{
	m_bDBConnStr = true;

	m_wstrDSN = wstrDBConnStr;

	// UID와 PWD를 조합한 DB Connection String을 만들어낸다! - 절대로 외부 노출되면 안됨!
	KClearString< std::wstring > kDBConnStrWithPW = wstrDBConnStr;
	//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
//#ifdef SERV_DB_CONNECTION_SECURITY
	if( wstrID.empty() == false )
	{
		kDBConnStrWithPW += L";UID=";
		kDBConnStrWithPW += wstrID;
	}
	if( wstrPW.empty() == false )
	{
		kDBConnStrWithPW += L";PWD=";
		kDBConnStrWithPW += wstrPW;
	}
//#else	
//	kDBConnStrWithPW += L";UID=";
//	kDBConnStrWithPW += wstrID;
//	kDBConnStrWithPW += L";PWD=";
//	kDBConnStrWithPW += wstrPW;
//#endif SERV_DB_CONNECTION_SECURITY
	//}}	

    m_SecConnStr.Init( kDBConnStrWithPW.GetString() ); // <--- 나중에 KClearString을 전달인자로 넣도록 바꾸자!
}

ImplOstmOperatorW( KDBThreadManager );


