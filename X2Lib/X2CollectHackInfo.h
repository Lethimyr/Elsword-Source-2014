#pragma once

struct COLLECT_SERVER_PACKET
{
	unsigned short	m_iEventID;
	unsigned short	m_usType;
	KSerBuffer*		m_pSerBuff;
	std::string		m_strHackInfo;
	bool			m_bSend;
	bool			m_bExit;
};

class CX2CollectHackInfo :
	public KJohnThread
{
public:
	CX2CollectHackInfo(void);
	virtual ~CX2CollectHackInfo(void);

	virtual bool BeginThread();
	virtual void EndThread(DWORD dwTimeout_ = INFINITE);
	virtual DWORD RunThread();

private:
	bool ConnectCollectServer_Thread();
	void SendCSImg_Thread( const std::string &strFileName, bool bExit );
	void SendCSEvent_Thread( unsigned short iEventId, unsigned short usType, const std::string &strHackInfo, bool bExit );
	void ReceiveCSEvent_Thread( COLLECT_SERVER_PACKET &spEvent );
	void DoProcessEvent_Thread();
	void LoadHackImg_Thread( const std::string &strFileName );

public:
	void ClearQueue();
	void EnQueue( unsigned short iEventID, KSerBuffer *pBuff, bool bSend, unsigned short usType, std::string strHackInfo, bool bExit = false );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	bool										m_bSendPacket;

private:
	CRITICAL_SECTION		                    m_csEventQueue;
	std::queue<COLLECT_SERVER_PACKET>           m_queEvent;
	CX2ServerProtocol*							m_pCollectServer;
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
    std::vector<char>                           m_vecHackImgBuf_Thread;
    int                                         m_iImageOffset_Thread;
    LONG volatile                               m_lCancelSendCSImg_Interlocked;
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	char*										m_pHackImgBuf;
	long										m_lTotalSize;
    int											m_iImageOffset;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	
	bool										m_bRunThread;
};

