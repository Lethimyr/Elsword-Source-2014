#pragma once

#include "KSimpleThread.h"

#define CLOSE_SOCKET( socket ) \
	if( socket != INVALID_SOCKET ) \
	{ \
	::shutdown( socket, SD_BOTH ); \
	::closesocket( socket ); \
	socket = INVALID_SOCKET; \
	} 

class KSimpleAccepter : public KSimpleThread
{
	typedef void (*PFNOnAccept)( SOCKET, const SOCKADDR_IN& );

public:
	KSimpleAccepter(void);
	virtual ~KSimpleAccepter(void);

	void Init( u_short usPort, PFNOnAccept fnOnAccept, bool bNagleAlgOn = true );
	bool GetNagleAlgOn() const   { return m_bNagleAlgOn; }

	// derived from KThread
	virtual void End( DWORD dwTimeOut = 3000 );

protected:
	// derived from KThread
	virtual void Run();
	inline void Loop();

	SOCKET          m_sock;
	PFNOnAccept     m_fnOnAccept;
	bool            m_bNagleAlgOn;
};
