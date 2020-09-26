#pragma once
#include "thread/Thread.h"
#include "KncException.h"
#include "KncSmartPtr.h"

#define KTRIOThreadPtr KTRIOThread*
#undef KTRIOThreadPtr
SmartPointer( KTRIOThread );

class KTRIOThread : public KThread
{
	_DeclareException;

public:
	KTRIOThread(void);
	virtual ~KTRIOThread(void);

protected:
	virtual void Run();
	void Loop();
};
