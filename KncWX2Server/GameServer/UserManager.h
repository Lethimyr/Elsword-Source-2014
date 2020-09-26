#pragma once
#include "GSUser.h"
#include "ActorManager.h"

class KUserManager : public KActorManager
{
    DeclToStringW;

private:
    KUserManager(const KUserManager& right);              // do not use.
    KUserManager& operator=(const KUserManager& right);   // do not use.

public:
    KUserManager();
    virtual ~KUserManager();

    void Insert( KGSUserPtr spActor );
    bool DeleteByCID( const UidType nCID );

    bool FindCID( const UidType nCID );
    KGSUserPtr GetByCID( const UidType nCID );
    KGSUserPtr GetByCName( const std::wstring& strCharName );

    template< class T > void ForEachUser( T& functor );
    CRITICAL_SECTION& GetCS()           { return m_csAct; }
    KGSUserPtr operator[](int i) const  { return boost::static_pointer_cast<KGSUser>( m_vecAct[i] ); }    // Note : 내부에 lock 처리가 없다. 
    KGSUserPtr At( int i ) const        { return boost::static_pointer_cast<KGSUser>( m_vecAct[i] ); }

    /* Note : operator[]의 사용

    내부에 lock 처리가 없으므로, multi-thread 환경에선 GetCS()를 이용해
    외부에서 lock을 걸어주는 것이 좋다.

    KGSPagePtr spPage( new KGSPage );

    { // locking scope

        KLocker lock( spPage->GetCS() );

        for( int i = 0; i < (int)spPage->GetCount(); i++ )
        {
            std::wstring strName = (*spPage)[i]->GetName();
        }

    } // locking scope                                          */

    // actormanager만의 고유함수들. 활용시엔 사용하지 않는다.
    void UnregCharInfo( IN KGSUser& kUser );    // 캐릭터 선택시, 이전 캐릭터 등록사항을 삭제한다.
    void RegCharInfo( IN KGSUser& kUser );      // 캐릭터를 변경한 경우, 변경된 사항을 적용한다.
    virtual void QueueingByCID( const UidType nReceiverCID, const KEventPtr& spEvent );
    virtual int  MultiQueueingByCID( const KEventPtr& spEvent, std::vector<UidType>& vecUID = std::vector<UidType>() ); // function over-riding 회피. Event안에 있는 uid들에게 모두 살포.

protected:
    virtual bool Delete( const UidType nUID );  // derived from KActorManager ( util func. )

protected:
    std::map<UidType,KGSUserPtr>        m_mapCID;   // character uid.
    std::map<std::wstring,KGSUserPtr>   m_mapCName; // character name.
};

DefKObjectInline( KUserManager, KActorManager );

template<typename Func> 
void KUserManager::ForEachUser( Func& functor )
{
    KLocker lock( m_csAct );
    for( int i = 0; i < GetCount(); i++ )
    {
        functor( At(i) );
    }
}
