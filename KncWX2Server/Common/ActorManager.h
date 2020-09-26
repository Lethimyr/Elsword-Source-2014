#pragma once
#include "Actor.h"

#include <map>
#include <RTTI.h>
#include <dbg/dbg.hpp>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	#include "TickDumper.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}

class KSocketObject;
class KActorManager
{
    NiDeclareRootRTTI(KActorManager);
    DeclToStringW;
    DeclInstanceKeeping( KActorManager );
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	DeclareTickDumper;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

    KActorManager(const KActorManager& right);              // do not use.
    KActorManager& operator=(const KActorManager& right);   // do not use.

protected:
    KActorManager();
public:
    virtual ~KActorManager();

    KActorPtr Get( const UidType nUID );
    int GetCount() const                 { KLocker lock( m_csAct ); return (int)m_mapUID.size(); }
    int GetCountNoLock() const           { return ( int )m_mapUID.size(); }

    template< class T > bool Get( IN const UidType nUID, IN OUT boost::shared_ptr<T>& spOut );
    template< class T > void ForEach( T& functor );
    //template< class T > void SendToAll( const unsigned short usEventID, const T& data );
    //template< class T > void QueueingToAll( const unsigned short usEventID, const T& data );
    //void SendIDToAll( const u_short usEventID );
    bool SendTo( const UidType nReceiverUID, IN const KEvent& kEvent );
    void QueueingTo( const UidType nReceiverUID, const KEventPtr& spEvent );
	int  MultiQueueing( const KEventPtr& spEvent, std::vector<UidType>& vecUID = std::vector<UidType>() ); // function over-riding 회피. Event안에 있는 uid들에게 모두 살포.
    void SendToAll( const KEvent& kEvent );
    void QueueingToAll( const KEventPtr& spEvent );

    virtual bool FindCID( const UidType nCID ) { return 0; }
    virtual void QueueingByCID( const UidType nReceiverCID, const KEventPtr& spEvent ) {}
    virtual int  MultiQueueingByCID( const KEventPtr& spEvent, std::vector<UidType>& vecUID = std::vector<UidType>() ) { return 0; } // function over-riding 회피. Event안에 있는 uid들에게 모두 살포.

    // actormanager만의 고유함수들. UserManager 활용시엔 사용하지 않는다.
    void ReserveAdd( KActorPtr spActor ); // Accepter thread에서 접근.
    void ReserveDelete( const UidType nUID );
    void Tick();
    bool UpdateUID( const UidType nUID, KActor& kActor );

	//{{ 2008. 10. 10  최육사	첫번째 UID얻기
	UidType GetFirstActorKey();
	//}}

protected:
    virtual bool Delete( const UidType nUID );
    void AddActor( KActorPtr spActor );

    mutable KncCriticalSection          m_csAct;    // m_vecAct, m_mapUID, m_mapCUID 을 동시에 보호한다.
    std::vector<KActorPtr>              m_vecAct;
    std::map<UidType,KActorPtr>         m_mapUID;  // user uid.

    mutable KncCriticalSection          m_csDel;
    std::vector<UidType>                m_vecDelReserved;

    mutable KncCriticalSection          m_csAdd;
    std::vector<KActorPtr>              m_vecAddAct;
};

template< typename ActorType >
bool KActorManager::Get( IN const UidType nUID_, IN OUT boost::shared_ptr<ActorType>& spOut_ )
{
    KActorPtr spActor( Get( nUID_ ) );

    if( spActor == NULL ) return false;

    if( ! NiIsExactKindOf( ActorType, spActor.get() ) )
        return false;

    spOut_ = boost::static_pointer_cast<ActorType>( spActor );
    return true;
}

template<typename Func> 
void KActorManager::ForEach( Func& functor )
{
    KLocker lock( m_csAct );
    std::for_each( m_vecAct.begin(), m_vecAct.end(), functor );
}

//template< class T >
//void KActorManager::QueueingToAll( const unsigned short usEventID_, const T& data_ )
//{
//    KEventPtr spEvent( new KEvent );
//    spEvent->SetData( 0, usEventID_, data_ );
//
//    KLocker lock( m_csAct );
//
//    std::vector<KActorPtr>::iterator vit;
//    for( vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++ )
//    {
//        (*vit)->QueueingEvent( spEvent );
//    }
//}
//
//template< class T >
//void KActorManager::SendToAll( const unsigned short usEventID_, const T& data_ )
//{
//    KLocker lock( m_csAct );
//
//    std::vector<KActorPtr>::iterator vit;
//    for( vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++ )
//    {
//        (*vit)->SendPacket( usEventID_, data_ );
//    }
//}
