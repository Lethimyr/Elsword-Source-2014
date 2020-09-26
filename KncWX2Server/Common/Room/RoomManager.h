#pragma once

#include "ToString.h"
#include "KNCSingleton.h"
#include "Thread/Thread.h"
#include "RTTI.h"
#include "Event.h"

class KRoomManager : public KThread
{
	DeclInstanceKeeping( KRoomManager );
	DeclToStringW;
	NiDeclareRootRTTI( KRoomManager );

protected:
	KRoomManager();

public:
	virtual ~KRoomManager();

	// derived from child class
	virtual void Run(){}
	virtual void ShutDown();

	virtual void QueueingEventToRoom( const KEventPtr& spEvent ){}
	virtual void QueueingToAll( int iRoomType, const KEventPtr& spEvent ){}
	virtual void QueueingToAll( const KEventPtr& spEvent ){}
};
