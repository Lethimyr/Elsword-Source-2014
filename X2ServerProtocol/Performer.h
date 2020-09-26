#pragma once

class KPerformer : public KSimObject
{
	NiDeclareRTTI;
	DeclToStringW;

public:
	KPerformer(void);
	virtual ~KPerformer(void);

	// 이곳에 위치한 virtual은 모두 최초 정의. 상속 아님
	void    QueueingEvent( const KEventPtr& spEvent );
    //void    QueueingID( UidType nSenderUID, unsigned short usEventID );
	size_t  GetQueueSize()
	{
		KLocker lock( m_csEventQueue );
		return m_queEvent.size();
	}
	virtual void            Tick();     // queue consumming

protected:
	virtual void ProcessEvent( IN KEvent* )   = 0;
	friend void KncSend( DWORD dwPIFrom, UidType nFrom, KEventPtr spEvent );

protected:
	mutable CRITICAL_SECTION                    m_csEventQueue;
	std::queue<KEventPtr>                       m_queEvent;

};