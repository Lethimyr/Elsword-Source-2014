//////////////////////////////////////////////////////////////////////////
//	CJBilllingManager.h
//
//	CJ 빌링 관리 메니져
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <KNCSingleton.h>
#include <Thread/KCSLockType.h>
#include "ThreadManager.h"

#ifdef SERV_COUNTRY_ID

class KCJBillingManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KCJBillingManager );

public:
	KCJBillingManager();
	virtual ~KCJBillingManager();

	void RegToLua();

	void InitCJBillingInfo(
		const char*     szNexonAuthIP,
		unsigned short  usPort );

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();

	const std::string& GetURL() { return m_kCJBillingInfo.m_strIP; }
	unsigned short GetPort() { return m_kCJBillingInfo.m_usPort; }

protected:

	struct KCJBillingInfo
	{
		std::string         m_strIP;
		unsigned short      m_usPort;
	} m_kCJBillingInfo;
};

DefSingletonInline( KCJBillingManager );

#endif // SERV_COUNTRY_ID