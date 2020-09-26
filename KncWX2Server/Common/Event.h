#pragma once

#include <Serializer/SerBuffer.h>
#include "KncSmartPtr.h"
#include "KncUidType.h"
#include <dbg/dbg.hpp>
#include <set>
#include <ObjectPool/BlockAllocator.h>
//#include <boost/utility.hpp> // noncopyable
#include <ServerDefine.h>

#   undef  _ENUM
#   define _ENUM( id ) id,
enum ENUM_SYSTEM_EVENT_ID { 
#   include "EventID_System.h"
};
enum ENUM_CLIENT_EVENT_ID { 
	E_CLIENT_EVENT_ID_BEGIN = E_SYSTEM_EVENT_ID_END,
#   include "EventID_Client.h"
};
enum ENUM_SERVER_EVENT_ID { 
	E_SERVER_EVENT_ID_BEGIN = EGS_CLIENT_EVENT_ID_END,
#   include "EventID_Server.h"
};

#define KEventPtr KEvent*
#undef KEventPtr
SmartPointer( KEvent );

#pragma pack( push, 1 )

struct KPerformerInfo;
SERIALIZE_DEFINE_TAG( KPerformerInfo, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET( KPerformerInfo );
struct KPerformerInfo
{
    enum { MAX_UID_NUM = 2000 };
    KPerformerInfo( void );
    KPerformerInfo( const KPerformerInfo& right ) { *this = right; }
    KPerformerInfo& operator=( const KPerformerInfo& right );
    virtual ~KPerformerInfo( void );

    DWORD                   m_dwPerformerID;

    bool FindUID( UidType nUID );
    bool AddUID( UidType nUID );
    int GetUIDListSize() { return m_setUID.size(); }
    const std::set<UidType>& GetUidListReference() const { return m_setUID; }   // 외부에서 loop를 돌 필요가 있다.
    UidType GetFirstUID() const;

    SERIALIZE_DECLARE_FRIEND( KPerformerInfo );

protected:
    std::set<UidType>    m_setUID;   // 대상이 여럿일 수 있다.
};

class KEvent;
SERIALIZE_DEFINE_TAG(KEvent, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(KEvent);
class KEvent : /*public boost::noncopyable, */public BlockAllocator<KEvent, 2000>
{
public:
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	enum EVENT_FROM_TYPE
	{
		EFT_NONE = 0,
		EFT_SERVER,
		EFT_CLIENT,
	};
#endif SERV_KEVENT_FROM
	//}}

	KEvent( void );
    KEvent(const KEvent& right) { *this = right; }      // copy constructor
    KEvent& operator=(const KEvent& right);             // assign operator
    virtual ~KEvent(void);
    KEventPtr Clone() const;

    template <class T> void SetData( DWORD dwPerformerID, const UidType anTrace[], u_short usEventID, const T& data_ );
    void SetData( DWORD dwPerformerID, const UidType anTrace[], u_short usEventID );

    const wchar_t*          GetIDStr() const            { return GetIDStr( m_usEventID ); }
    static const wchar_t*   GetIDStr( const unsigned short usEventID );
    UidType     GetFirstSenderUID() const   { return m_anTrace[0]; }
    UidType     GetLastSenderUID() const    { return m_anTrace[1] == -1 ? m_anTrace[0] : m_anTrace[1]; }
    bool        IsEmptyTrace() const        { return m_anTrace[0] == -1; } // [0]이 무효하면 빈 경우다.
    void        PushTrace( UidType nUid );
    void        PopTrace();
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	unsigned short GetFromType() const		{ return m_usFromType; }
	void		SetFromType( const unsigned short usFromType ) { m_usFromType = usFromType; }
	bool		IsValidEventID() const;
#endif SERV_KEVENT_FROM
	//}}

    //SERIALIZE_DECLARE_FRIEND( KEvent );

//protected:
    KPerformerInfo      m_kDestPerformer;
    UidType             m_anTrace[2];
    unsigned short      m_usEventID;
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	unsigned short      m_usFromType;
#endif SERV_KEVENT_FROM
	//}}
    KSerBuffer          m_kbuff;

protected:
    static const wchar_t*   ms_szEventID[];
};

#pragma pack( pop )

template <class T>
void KEvent::SetData( DWORD dwPerformerID, const UidType anTrace[], u_short usEventID, const T& data_ )
{
    SetData( dwPerformerID, anTrace, usEventID );

    KSerializer ks;
    m_kbuff.Clear();

    // serialize - only data
    ks.BeginWriting( &m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();
}

// 자신만의 처리 스레드를 가지는 싱글톤 객체를 위한 CASE 매크로
// Event 객체를 해당 클래스에 Queueing하기만 한다.
#define CASE_FOR_REDIRECT( id, classpointer ) \
case id: \
{ \
	classpointer->QueueingEvent( spEvent_ ); \
} \
break

#define _CASE( id, packet ) \
case id: \
{ \
    KSerializer ks; \
    ks.BeginReading( &spEvent_->m_kbuff ); \
    packet kPacket; \
    if( !ks.Get(kPacket) ) \
    { \
        START_LOG( cerr, L"deserialze failed." L#id L" - " L#packet ); \
    } \
    else \
	{ \
		ON_##id( spEvent_->m_anTrace, kPacket); \
		spEvent_->m_kbuff.Reset(); \
	} \
} \
break

#define  CASE_NOPARAM(id)           case id: ON_##id( spEvent_->m_anTrace ); break 
#define  CASE(id)                  _CASE(id, K##id)

#define _IMPL_ON_FUNC(id, packet)   void CLASS_TYPE::ON_##id( UidType anTrace_[], packet& kPacket_ )
#define  IMPL_ON_FUNC_NOPARAM(id)   void CLASS_TYPE::ON_##id( UidType anTrace_[] )
#define  IMPL_ON_FUNC(id)           void CLASS_TYPE::ON_##id( UidType anTrace_[], K##id& kPacket_ )
// ^ _IMPL... 매크로를 사용해서 이중으로 정의하면 Visual Assist가 제대로 읽지 못한다.

#define _DECL_ON_FUNC(id, packet)   void ON_##id( UidType anTrace_[], packet& kPacket )
#define  DECL_ON_FUNC_NOPARAM(id)   void ON_##id( UidType anTrace_[] )
#define  DECL_ON_FUNC(id)          _DECL_ON_FUNC(id, K##id)

#define  LAST_SENDER_UID            ( anTrace_[1] == -1 ? anTrace_[0] : anTrace_[1] )
#define  FIRST_SENDER_UID           ( anTrace_[0] )


