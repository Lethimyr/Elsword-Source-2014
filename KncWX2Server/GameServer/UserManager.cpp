#include "UserManager.h"
#include "NetError.h"

#include <boost/bind.hpp>

ImplToStringW( KUserManager )
{
    START_TOSTRING_PARENTW( KActorManager );

#if defined(DEBUG) || defined(_DEBUG)

    KLocker lock( m_csAct );
    std::vector<KActorPtr>::const_iterator vit;
    for( vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++ )
    {
        stm_ << vit - m_vecAct.begin() << L" : " << (*vit)->GetName() 
            << L"(UID:" << (*vit)->GetUID() << L") => "
            << static_cast< KGSUser* >((*vit).get())->GetUserName() << std::endl;
    }

#endif

    return stm_;

}

KUserManager::KUserManager()
{
}

KUserManager::~KUserManager()
{
}

void KUserManager::Insert( KGSUserPtr spUser_ )
{
    if( !spUser_ ) return;

    { // locking scope

        KLocker lock( m_csAct );

        m_vecAct.push_back( spUser_ );
        m_mapUID.insert( std::make_pair( spUser_->GetUID(), spUser_ ) );

    } // locking scope

    m_mapCID.insert( std::make_pair( spUser_->GetCharUID(), spUser_) );
}

bool KUserManager::Delete( const UidType nUID_ )
{
    {
        KGSUserPtr spUser;
        Get( nUID_, spUser );

        if( !spUser )
        {
            START_LOG( cerr, L"삭제하고자 하는 유저가 없음" )
                << BUILD_LOG( nUID_ )
                << BUILD_LOG( m_mapUID.size() )
                << BUILD_LOG( m_mapCID.size() )
                << END_LOG;
            return false;
        }

        m_mapCID.erase( spUser->GetCharUID() );
        m_mapCName.erase( spUser->GetCharName() );
    }

    return KActorManager::Delete( nUID_ );
}


bool KUserManager::DeleteByCID( const UidType nCID_ )
{
    UidType nUID = -1;
    {
        KGSUserPtr spUser( GetByCID( nCID_ ) );

        if( !spUser )
        {
            START_LOG( cerr, L"삭제하고자 하는 CID가 없음" )
                << BUILD_LOG( nCID_ )
                << BUILD_LOG( m_mapUID.size() )
                << BUILD_LOG( m_mapCID.size() )
                << END_LOG;
            return false;
        }
        else
            nUID = spUser->GetUID();
    }

    if( nUID == -1 )
        return false;

    return Delete( nUID );
}

bool KUserManager::FindCID( const UidType nCID_ )
{
    std::map<UidType,KGSUserPtr>::iterator mit;

    mit = m_mapCID.find( nCID_ );

    return !( mit == m_mapCID.end() );
}

KGSUserPtr KUserManager::GetByCID( const UidType nCID_ )
{
    std::map<UidType,KGSUserPtr>::iterator mit;

    mit = m_mapCID.find( nCID_ );

    if( mit == m_mapCID.end() ) return KGSUserPtr();

    return mit->second;
}

KGSUserPtr KUserManager::GetByCName( const std::wstring& strCharName_ )
{
    std::map<std::wstring,KGSUserPtr>::iterator mit;

    mit = m_mapCName.find( strCharName_ );

    if( mit == m_mapCName.end() ) return KGSUserPtr();

    return mit->second;
}

void KUserManager::RegCharInfo( IN KGSUser& kUser_ )
{
	std::pair< std::map< UidType, KGSUserPtr >::iterator, bool > pairCID;
	pairCID = m_mapCID.insert( std::make_pair( kUser_.GetCharUID(), kUser_.GetThisPtr<KGSUser>() ) );
	LIF( pairCID.second );

	std::pair< std::map< std::wstring, KGSUserPtr >::iterator, bool > pairNick;
    pairNick = m_mapCName.insert( std::make_pair( kUser_.GetCharName(), kUser_.GetThisPtr<KGSUser>() ) );
	LIF( pairNick.second );
}

void KUserManager::UnregCharInfo( IN KGSUser& kUser_ )
{
    m_mapCID.erase( kUser_.GetCharUID() );
    m_mapCName.erase( kUser_.GetCharName() );
}

void KUserManager::QueueingByCID( const UidType nReceiverCID_, const KEventPtr& spEvent_ )
{
    KGSUserPtr spUser( GetByCID( nReceiverCID_ ) );

    if( spUser )
        spUser->QueueingEvent( spEvent_ );
}

int KUserManager::MultiQueueingByCID( const KEventPtr& spEvent_, std::vector<UidType>& vecUID )
{
    const std::set<UidType>& setUID = spEvent_->m_kDestPerformer.GetUidListReference();

	int iNotExistUserCnt = 0;

    KActorPtr spActor;
    std::set<UidType>::const_iterator sit;
    for( sit = setUID.begin(); sit != setUID.end(); sit++ )
    {
        spActor = GetByCID( *sit );

        if( spActor )
		{
			spActor->QueueingEvent( spEvent_->Clone() );
		}
		else
		{
			vecUID.push_back( *sit );
			++iNotExistUserCnt;

			START_LOG( cwarn, L"m_mapCID에서 CID를 못찾음. GSUser에 Event Queueing실패!" )
				<< BUILD_LOG( spEvent_->GetFirstSenderUID() )
				<< BUILD_LOG( spEvent_->GetLastSenderUID() )
				<< BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
				<< BUILD_LOG( spEvent_->m_usEventID )
				<< BUILD_LOG( spEvent_->GetIDStr() )
				<< BUILD_LOG( *sit )
				<< END_LOG;
		}
    }

	if( setUID.empty() )
	{
		START_LOG( cwarn, L"DestPerformer에 UID가 하나도 들어있지 않음!" )
			<< BUILD_LOG( spEvent_->GetFirstSenderUID() )
			<< BUILD_LOG( spEvent_->GetLastSenderUID() )
			<< BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	return iNotExistUserCnt;
}