#pragma once

#include "FSM/FSMClass.h"
#include <boost/utility.hpp>

SmartPointer( IActorFactory );

// 단순히 KActorFactory의 인터페이스 역할만을 한다. 동적생성을 위한 포인터를 선언할 때 쓰인다.
class KActor;
class IActorFactory : public boost::noncopyable
{
public:
    virtual boost::shared_ptr<KActor> CreateActor() = 0;
};

template < class ActorType, class FSMType, int nSendQueueSize = 0 >
class KActorFactory : public IActorFactory
{
public:
    KActorFactory() : m_spFSM( new FSMType ) {}
    virtual ~KActorFactory(void) {}

    virtual boost::shared_ptr<KActor> CreateActor()
    {
        boost::shared_ptr<KActor> spActor( new ActorType );
        if( spActor )
        {
            spActor->Init( true );
            spActor->SetFSM( m_spFSM );
            spActor->SetSendQueueSize( nSendQueueSize );
        }
        else
        {
            START_LOG( cout, L" new KActor Fail..!!!! ");
        }

        return spActor;
    }

protected:
    FSMclassPtr m_spFSM;
};