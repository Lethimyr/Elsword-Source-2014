#pragma once

#   undef  _ENUM
#   define _ENUM( id ) id,
enum ENUM_EVENT_ID { 
#   include "EventID_System.h"
//#   include "EventID_Client.h"
//#   include "EventID_Server.h"
};



SmartPointer(KEvent);

#pragma pack( push, 1 )

struct KPerformerInfo;
SERIALIZE_DEFINE_TAG( KPerformerInfo, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET( KPerformerInfo );
struct KPerformerInfo
{
    enum { MAX_UID_NUM = 100 };
    KPerformerInfo( void );
    KPerformerInfo( const KPerformerInfo& right ) { *this = right; }
    KPerformerInfo& operator=( const KPerformerInfo& right );
    virtual ~KPerformerInfo( void );

    DWORD           m_dwPerformerID;

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
	KEvent( void );
	KEvent(const KEvent& right) { *this = right; }      // copy constructor
	KEvent& operator=(const KEvent& right);             // assign operator
	virtual ~KEvent(void);

    template <class T> void SetData( DWORD dwPerformerID, const UidType anTrace[], u_short usEventID, const T& data_ );
    void SetData( DWORD dwPerformerID, const UidType anTrace[], u_short usEventID );

	const wchar_t*          GetIDStr() const { return GetIDStr( m_usEventID ); }
	static const wchar_t*   GetIDStr( const unsigned short usEventID );
	static void				SetEventID( const wchar_t**  szEventIDList, int eventIDEnd )
	{
		if( g_bIsSERVICE == false )
		{
			ms_szEventIDList	= szEventIDList;
			m_EventIDEnd		= eventIDEnd;
		}
	}

	//SERIALIZE_DECLARE_FRIEND( KEvent );

	//protected:
    KPerformerInfo          m_kDestPerformer;
    mutable UidType         m_anTrace[2];
	unsigned short          m_usEventID;
	KSerBuffer              m_kbuff;

protected:
	static const wchar_t*   ms_szEventID[];
	static const wchar_t**  ms_szEventIDList;
	static int				m_EventIDEnd;
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

#define _CASE( id, packet ) \
case id: \
{ \
    packet kPacket; \
    if( !ks.Get(kPacket) ) \
    { \
        START_LOG( cerr, L"deserialze failed." L#id L" - " L#packet ); \
    } \
    else ON_##id( pkEvent_->m_anTrace, kPacket); \
} \
break

#define  CASE_NOPARAM(id)           case id: ON_##id( spEvent_->m_anTrace ); break 
#define  CASE(id)                  _CASE(id, K##id)

#define _IMPL_ON_FUNC(id, packet)   void CLASS_TYPE::ON_##id( UidType anTrace_[], packet& kPacket_ )
#define  IMPL_ON_FUNC_NOPARAM(id)   void CLASS_TYPE::ON_##id( UidType anTrace_[] )
#define  IMPL_ON_FUNC(id)           void CLASS_TYPE::ON_##id( UidType anTrace_[], K##id& kPacket_ )
// ^ _IMPL... 매크로를 사용해서 이중으로 정의하면 Visual Assist가 제대로 읽지 못한다.

#define _DECL_ON_FUNC(id, packet)   void ON_##id( std::vector< UidType > vecTrace_, packet& kPacket )
#define  DECL_ON_FUNC_NOPARAM(id)   void ON_##id( std::vector< UidType > vecTrace_ )
#define  DECL_ON_FUNC(id)          _DECL_ON_FUNC(id, K##id)

#define  LAST_SENDER_UID            ( anTrace_[1] == -1 ? anTrace_[0] : anTrace_[1] )
#define  FIRST_SENDER_UID           ( anTrace_[0] )
