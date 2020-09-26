#pragma once

SmartPointer( KActorProxy );

struct KServerInfo;
class KActorProxy : public KSession
{
	NiDeclareRTTI;
	DeclToStringW;

protected:
	KActorProxy();

public:
	virtual ~KActorProxy(void) {}

	void	SetHwnd( HWND hWnd );
	void	SetSendMsgCallBack( SEND_MSG_FUNC pSendGameMessage ){ m_pSendGameMessage = pSendGameMessage; }
	void	SetThrowWinMsg( bool bThrowWinMsg );
	bool	GetThrowWinMsg() { return m_bThrowWinMsg; }

	// 060720. 인증 처리까지 기다린다. KUserProxy, KGSProxy가 이를 사용한다.
	int		ConnectNAuth( IN const WCHAR* pIP, IN const int port, IN const KEvent& kEvent, IN DWORD dwTimeOut );

	void	SetClassID( int iClassID )	{ m_iClassID = iClassID; }  // 윈도우 메세지를 구성할때 사용할 id 할당.
	int		GetClassID()				{ return m_iClassID; }
	void	SetServerMsgID( UINT serverMsgID ){ m_ServerMsgID = serverMsgID; }
    void SetLoad( int iLoad )               { m_iLoad = iLoad; }
    int GetLoad()                           { return m_iLoad; }

    void SetServerUID( UidType iServerUID ) { m_iServerUID = iServerUID; }
    UidType GetServerUID()                  { return m_iServerUID; }



protected:
	void PostEvent( KEvent* pkEvent );    // 이벤트를 윈도우 메시지로 하거나 내부 큐에 큐잉한다.

	HWND                        m_hWnd;
	SEND_MSG_FUNC				m_pSendGameMessage;
	bool                        m_bThrowWinMsg;     ///< false이면 받은 메시지를 큐에 넣어두기만 한다.
	UINT						m_ServerMsgID;

	boost::shared_ptr<void>     m_spEventAck;
	int                         m_nAckOK;
	int                         m_iClassID;         // 이벤트를 윈도우 메세지로 던질 때, 구분자 역할을 한다.

    int                         m_iLoad;
    UidType                     m_iServerUID;
public:

	KncCriticalSection          m_csRecvMsg;
	std::queue<KEventPtr>       m_queRecvMsg;       ///< m_bThrowWinmsg = false일때 사용.

};