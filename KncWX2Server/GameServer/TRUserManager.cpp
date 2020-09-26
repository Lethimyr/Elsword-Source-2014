#include "TRUserManager.h"
#include "TRUserFSM.h"
#include <boost/bind.hpp>
#include <dbg/dbg.hpp>
#include "Serializer/Serializer.h"

#include "TRServer.h"

#define CLASS_TYPE      KTRUserManager


KTRUserManager::KTRUserManager(void)
: 
m_bCreateSessionEnable(true),
m_spFSM( new KTRUserFSM )
{
}

KTRUserManager::~KTRUserManager(void)
{
}

KTRUserPtr KTRUserManager::CreateTRUser()
{
	if( !m_bCreateSessionEnable ) return KTRUserPtr();

	KTRUserPtr spTRUser(new KTRUser);
	spTRUser->Init();

	if( spTRUser->GetUID() != static_cast<UidType>(-1) )
	{
		START_LOG( cerr, L"Fail Add to m_listSessionTable, UID is Not Zero : " << spTRUser->GetUID() );
		return KTRUserPtr();
	}

	spTRUser->SetFSM( m_spFSM );
	spTRUser->SetSendQueueSize( 32 * 1024 );
	spTRUser->StateTransition( KTRUserFSM::INPUT_CONNECT );

	{
		KLocker lock( m_cslist );
		m_listSessionTable.push_back( spTRUser );
	}

	return spTRUser;
}

KTRUserPtr KTRUserManager::GetTRUser( const UidType& iUID_ )
{
	KLocker lock( m_csmap );

	std::map< UidType, KTRUserPtr >::iterator mit;
	mit = m_mapSessionTable.find( iUID_ );

	if( mit != m_mapSessionTable.end() )
	{
		return mit->second;
	}

	START_LOG( cwarn, L"Not Found TRUser : " << iUID_ );

	return KTRUserPtr();
}

void KTRUserManager::Tick()
{
	KLocker lock( m_cslist );

	std::list<KTRUserPtr>::iterator lit;
	lit = m_listSessionTable.begin();

	while(lit != m_listSessionTable.end())
	{
		(*lit)->Tick();

		if( (*lit)->GetStateID() == KTRUserFSM::STATE_EXIT )
		{
			if( (*lit)->GetUID() != -1 ) // 유효한 UID가 등록 된 경우는 map에서도 지운다.
			{
				KLocker lock(m_csmap);
				m_mapSessionTable.erase( (*lit)->GetUID() );
				START_LOG(clog, "Delete User : " << (*lit)->GetUID() );
			}
			lit = m_listSessionTable.erase( lit );
		}
		else
		{
			++lit;
		}
	}
}

bool KTRUserManager::Insert( KTRUserPtr pkTRUser_ )
{
    if( !pkTRUser_ )
    {
        START_LOG( cerr, L"TRUser 포인터 이상" )
            << END_LOG;

        return false;
    }

	if( pkTRUser_->GetUID() == -1 )
	{
		START_LOG( cerr, L"Can't Regist User UID : -1 ");
		return false;
	}

	KLocker lock( m_csmap );
	std::map< UidType, KTRUserPtr >::iterator mit = m_mapSessionTable.find( pkTRUser_->GetUID() );
	if( mit != m_mapSessionTable.end() ) // 이미 등록된 UID에 대한 처리.
	{
		START_LOG( cerr, L"Already Exist")
			<< BUILD_LOG( pkTRUser_->GetUID() );
		return false;
	}

	m_mapSessionTable.insert( std::make_pair( pkTRUser_->GetUID(), pkTRUser_ ) );

	return true;
}



