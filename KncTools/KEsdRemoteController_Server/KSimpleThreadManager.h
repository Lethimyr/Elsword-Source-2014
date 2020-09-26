#pragma once

#include <vector>
#include <process.h>

class KSimpleThread;
class KSimpleThreadManager
{
public:
	KSimpleThreadManager(void);
	virtual ~KSimpleThreadManager(void);

	virtual void Init( int nThreadNum );
	virtual void BeginThread();
	virtual void EndThread();   // derive to Child...

protected:
	virtual KSimpleThread* CreateThread() = 0;

	bool                                m_bTerminateReserved;
	size_t                              m_nTerminateCount;  // 종료된 스레드 갯수를 센다.
	std::vector< KSimpleThread* >		m_vecThread;
	CRITICAL_SECTION					m_csVecThread;
};
