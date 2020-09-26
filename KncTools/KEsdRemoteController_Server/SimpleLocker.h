#include <process.h>

class KSimpleLocker
{
public:
	KSimpleLocker( CRITICAL_SECTION* cs ) { m_pCS = cs; EnterCriticalSection( cs ); }
	~KSimpleLocker() { LeaveCriticalSection( m_pCS ); }

private:
	CRITICAL_SECTION* m_pCS;
};