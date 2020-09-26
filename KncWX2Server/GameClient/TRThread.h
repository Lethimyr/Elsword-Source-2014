#pragma once
#include "TRUser.h"
#include "thread/Thread.h"
#include "KncException.h"
#include "KncSmartPtr.h"

SmartPointer( KIOThread );

class KTRThread : public KThread
{
	_DeclareException;

public:
	KTRThread(int iMin, int iMax, const char* strIP);
	virtual ~KTRThread(void);

protected:
	void Init( const int iCreateMin, const int iCreateMax, const char* strIP );

	virtual void Run();
	void Loop();

protected:
	std::vector<KTRUserPtr>		m_vecRobot;
};