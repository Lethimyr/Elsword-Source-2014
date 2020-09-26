#pragma once
#include <windows.h>
#include <winsock2.h>
#include <map>
#include "KPacket.h"
#include "KSingleton.h"
#include "KSimpleThreadManager.h"

class KTCPSocket;
class KSimpleIOCP : public KSimpleThreadManager
{
	DeclareSingleton( KSimpleIOCP );

public:
	enum {
		SIGNAL_SHUTDOWN_SERVER = 0
	};

	KSimpleIOCP(void);
	~KSimpleIOCP(void);

	bool AssociateSocket( KTCPSocket* pkSockObj );
	bool AssociateSocket( SOCKET hSocket, ULONG_PTR CompKey );
	bool PostStatus( ULONG_PTR CompKey, 
		DWORD dwNumBytes    = 0, 
		OVERLAPPED* po      = NULL );

	bool GetStatus( OUT ULONG_PTR* pCompKey, 
		OUT PDWORD pdwNumBytes,
		OUT OVERLAPPED** ppo, 
		IN DWORD dwMilliseconds = INFINITE );

	void EndThread();
	void DeleteCompKey( KTCPSocket* pkSockObj );
	void OnIOCompleted( IN DWORD dwKey, IN DWORD dwBytesTransfer, IN OVERLAPPED* povl );
	void OnClosedByLocal( IN DWORD dwKey );

	void BroadCastPacket( KPacket& kPacket );

private:
	KTCPSocket* GetSockObject( IN DWORD dwKey );
	virtual KSimpleThread* CreateThread();  // derived from KSimpleThreadManager	
	inline DWORD GenNewCompKey();

private:
	HANDLE m_hIocp;

	CRITICAL_SECTION			 m_csSockObj;
	std::map<DWORD,KTCPSocket*>  m_mapSockObj;
};

DefSingletonInline( KSimpleIOCP );