#pragma once
#include "Event.h"
#include "SimObject.h"
#include <queue>
#include <Serializer/SerBuffer.h>

#include <windows.h>    // CRITICAL_SECTION
#include "Thread/Locker.h"

enum ePerformerID;
class KPerformer : public KSimObject
{
    NiDeclareRTTI;
    DeclToStringW;

public:
	//{{ 2009. 9. 16  최육사	덤프	
	struct KMaxQueueSize
	{
		KMaxQueueSize()
		{
			Clear();
		}

		void Clear()
		{
			m_QueueSize = 0;
		}

		std::wstring GetMaxQueueSizeRegDate() const
		{
			return ( std::wstring )( m_tRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );
		}

		size_t		m_QueueSize;
		CTime		m_tRegTime;
	};
	//}}

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

    virtual int GetPfID() = 0;

	//{{ 2009. 9. 16  최육사	덤프
	KMaxQueueSize GetMaxQueueSize() const
	{
		KLocker lock( m_csEventQueue );

		return m_kMaxQueueSize;
	}
	//}}
	//{{ 2011. 03. 24	최육사	서버 상태 체크
	KMaxQueueSize GetMaxQueueSizeAndClear()
	{
		KLocker lock( m_csEventQueue );

		KMaxQueueSize kTemp = m_kMaxQueueSize;
		m_kMaxQueueSize.Clear();
		return kTemp;
	}
	//}}

protected:
    bool            GetKEvent( KEventPtr& spEvent );
    virtual void    ProcessEvent( const KEventPtr& spEvent ) = 0;
    virtual bool    RoutePacket( const KEvent* pkEvent )   { return false; }
	friend void KncSend( DWORD dwPIFrom, UidType nFrom, KEventPtr spEvent );

protected:
    mutable CRITICAL_SECTION                    m_csEventQueue;
    std::queue<KEventPtr>                       m_queEvent;
	
	//{{ 2009. 9. 16  최육사	덤프
	KMaxQueueSize								m_kMaxQueueSize;
	//}}
};

#define DeclPfID \
public: \
    const static ePerformerID ms_ePfID; \
    virtual int GetPfID() { return ms_ePfID; }

#define ImplPfID( classname, id ) \
    const ePerformerID classname::ms_ePfID( id )
