#include "StdAfx.h"

#include "Event.h"

#undef _ENUM
#define _ENUM( id ) L#id,
const wchar_t*  KEvent::ms_szEventID[] = { 
#   include "EventID_System.h"
//#   include "EventID_Client.h"
//#   include "EventID_Server.h"
};

const wchar_t**		KEvent::ms_szEventIDList	= KEvent::ms_szEventID;
int					KEvent::m_EventIDEnd		= E_SYSTEM_EVENT_ID_END;

KPerformerInfo::KPerformerInfo( void ) : m_dwPerformerID( 0 )
{
}

KPerformerInfo::~KPerformerInfo( void )
{
}

KPerformerInfo& KPerformerInfo::operator=( const KPerformerInfo& right )
{
    m_dwPerformerID     = right.m_dwPerformerID;
    m_setUID            = right.m_setUID;
    return *this;
}

bool KPerformerInfo::AddUID( UidType nUID )
{
    if( m_setUID.size() >= MAX_UID_NUM )
        return false;

    m_setUID.insert( nUID );
    return true;
}

bool KPerformerInfo::FindUID( UidType nUID )
{
    return m_setUID.end() != m_setUID.find( nUID );
}

UidType KPerformerInfo::GetFirstUID() const
{
    if( m_setUID.empty() ) return -1;

    return *m_setUID.begin();
}

SERIALIZE_DEFINE_PUT( KPerformerInfo, obj, ks )
{
    return  ks.Put( obj.m_dwPerformerID ) &&
        ks.Put( obj.m_setUID );
}

SERIALIZE_DEFINE_GET( KPerformerInfo, obj, ks )
{
    return  ks.Get( obj.m_dwPerformerID ) &&
        ks.Get( obj.m_setUID );
}

KEvent::KEvent(void) : m_usEventID( 0 )
{
    m_anTrace[0] = m_anTrace[1] = -1;
}

KEvent::~KEvent(void)
{
}

KEvent& KEvent::operator=(const KEvent& right)
{
    m_kDestPerformer    = right.m_kDestPerformer;
    m_anTrace[0]        = right.m_anTrace[0];
    m_anTrace[1]        = right.m_anTrace[1];
	m_usEventID         = right.m_usEventID;
	m_kbuff             = right.m_kbuff;
	return *this;
}

void KEvent::SetData( DWORD dwPerformerID, const UidType anTrace[], u_short usEventID )
{
    m_kDestPerformer.m_dwPerformerID    = dwPerformerID;
    m_usEventID                         = usEventID;

    if( anTrace != NULL )
    {
        m_anTrace[0] = anTrace[0];
        m_anTrace[1] = anTrace[1];
    }
    else
        m_anTrace[0] = m_anTrace[1] = -1;
}

SERIALIZE_DEFINE_PUT( KEvent, obj, ks )
{
    return  ks.Put( obj.m_kDestPerformer ) &&
            ks.Put( obj.m_anTrace[0] ) &&
            ks.Put( obj.m_anTrace[1] ) &&
            ks.Put( obj.m_usEventID ) &&
            ks.Put( obj.m_kbuff );
}

SERIALIZE_DEFINE_GET( KEvent, obj, ks )
{
    return  ks.Get( obj.m_kDestPerformer ) &&
            ks.Get( obj.m_anTrace[0] ) &&
            ks.Get( obj.m_anTrace[1] ) &&
            ks.Get( obj.m_usEventID ) &&
            ks.Get( obj.m_kbuff );
}

const wchar_t* KEvent::GetIDStr( const unsigned short usEventID )
{
	if( usEventID >= m_EventIDEnd )
	{
		return ms_szEventIDList[ m_EventIDEnd ];
	}
	return ms_szEventIDList[ usEventID ];
} 
