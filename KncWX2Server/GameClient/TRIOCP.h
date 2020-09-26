#pragma once 

#include <windows.h>    // HANDLE, ULONG_PTR
#include <KNCSingleton.h>
#include "ThreadManager.h"

class KTRIocp : public KThreadManager
{
	DeclareSingleton( KTRIocp );
	DeclToStringW;
	NiDeclareRTTI;
	DeclPfID;

public:
	enum 
	{
		SIGNAL_SHUTDOWN_SERVER = 0
	};

protected:
	KTRIocp();
public:
	virtual ~KTRIocp(); 

	virtual void EndThread();  // derived from KThreadManager

    bool AssociateSocket( KSocketObjectPtr spSockObj );
	bool AssociateSocket( SOCKET hSocket, ULONG_PTR CompKey );
	bool PostStatus(ULONG_PTR CompKey, 
		DWORD dwNumBytes    = 0, 
		OVERLAPPED* po      = NULL );

	bool GetStatus(	OUT ULONG_PTR* pCompKey, 
		OUT PDWORD pdwNumBytes,
		OUT OVERLAPPED** ppo, 
		IN DWORD dwMilliseconds = INFINITE );

    bool DeleteCompKey( DWORD dwKey );
	void OnIOCompleted( IN DWORD dwKey, IN DWORD dwBytesTransfer, IN OVERLAPPED* povl );
	void OnClosedByLocal( IN DWORD dwKey );
    size_t GetSockObjNum() const;

private:
    KSocketObjectPtr GetSockObj( IN DWORD dwKey );
	virtual KThread* CreateThread();  // derived from KThreadManager
	inline DWORD GenNewCompKey() const;

	HANDLE                          m_hIOCP;
    mutable KncCriticalSection m_csSockObj;
    std::map< DWORD, KSocketObjectPtr > m_mapSockObj;

	mutable KncCriticalSection  m_cs;               // Critical Section
	mutable DWORD               m_dwTerm;           // 키 생성하는 가장 긴 시간
	mutable CTime               m_cTime;            // 가장 긴 키생성시간이 발생한 시간
    mutable DWORD               m_dwSockObjNum;

	friend class KTRIOThread;
};

DefSingletonInline( KTRIocp );