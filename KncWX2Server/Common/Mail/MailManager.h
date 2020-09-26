#pragma once


//#include "ThreadManager.h"
#include <Windows.h>
#include "X2SMTPMail.h"
#include "Performer.h"
#include "ServerPacket.h"
#include "ThreadManager.h"


//#ifdef SERV_MAIL_SYSTEM


//////////////////////////////////////////////////////////////////////////
class KMailEventProcess : public KPerformer
{
	DeclPfID;

public:
	KMailEventProcess();
	virtual ~KMailEventProcess();

	// override.
	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer

	DECL_ON_FUNC( EMAIL_SEND_EMAIL_NOT );	

protected:
};



//////////////////////////////////////////////////////////////////////////
class KMailManager : public KThread
{
    DeclToStringW;
    DeclDump;

protected:
	virtual void Run(); // derived from KThread

public:
    KMailManager();
    virtual ~KMailManager();

	void Init();
    void Shutdown();

	template < class T >
	void QueueingEvent( unsigned short usEventID, const T& data );
	void QueueingEventID( unsigned short usEventID );

protected:
	void _QueueingEvent( const KEventPtr& spEvent );

	// function
	void Tick();

protected:
	KMailEventProcess				m_kMailEventProcess;

};

template < class T >
void KMailManager::QueueingEvent( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, usEventID, data );
	_QueueingEvent( spEvent );
}


//#endif SERV_MAIL_SYSTEM


