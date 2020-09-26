#pragma once
#include "thread/Thread.h"
#include "KncException.h"
#include "KncSmartPtr.h"

#define KGarenaBillingIOThreadPtr KGarenaBillingIOThread*
#undef KGarenaBillingIOThreadPtr
SmartPointer( KGarenaBillingIOThread );

class KGarenaBillingIOThread : public KThread
{
	_DeclareException;

public:
	KGarenaBillingIOThread(void);
	virtual ~KGarenaBillingIOThread(void);

protected:
	virtual void Run();
	void Loop();
};
