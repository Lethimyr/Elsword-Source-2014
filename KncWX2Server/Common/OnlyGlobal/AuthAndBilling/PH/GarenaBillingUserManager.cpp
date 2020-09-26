#include "GarenaBillingUserManager.h"
#include "GarenaBillingUserFSM.h"
#include <boost/bind.hpp>
#include <dbg/dbg.hpp>
#include "Serializer/Serializer.h"

#include "GarenaBillingServer.h"


//{{ 2013. 04. 08	최육사	넥슨 통합 운영툴 미들웨어
#ifdef SERV_COUNTRY_PH


#define CLASS_TYPE      KGarenaBillingUserManager


KGarenaBillingUserManager::KGarenaBillingUserManager(void)
: 
m_bCreateSessionEnable(true),
m_spFSM( new KGarenaBillingUserFSM )
{
}

KGarenaBillingUserManager::~KGarenaBillingUserManager(void)
{
}

KGarenaBillingUserPtr KGarenaBillingUserManager::CreateGarenaBillingUser()
{
	if( !m_bCreateSessionEnable ) return KGarenaBillingUserPtr();

	KGarenaBillingUserPtr spTRUser(new KGarenaBillingUser);
	spTRUser->Init();

	if( spTRUser->GetUID() != 0 )
	{
		START_LOG( cerr, L"Fail Add to m_listSessionTable, UID is Not Zero : " << spTRUser->GetUID() );
		return KGarenaBillingUserPtr();
	}

	spTRUser->SetFSM( m_spFSM );
	spTRUser->SetSendQueueSize( 32 * 1024 );
	spTRUser->StateTransition( KGarenaBillingUserFSM::INPUT_CONNECT );

	{
		KLocker lock( m_cslist );
		m_listSessionTable.push_back( spTRUser );
	}

	return spTRUser;
}

KGarenaBillingUserPtr KGarenaBillingUserManager::GetGarenaBillingUser( const UidType& iUID_ )
{
	KLocker lock( m_csmap );

	std::map< UidType, KGarenaBillingUserPtr >::iterator mit;
	mit = m_mapSessionTable.find( iUID_ );

	if( mit != m_mapSessionTable.end() )
	{
		return mit->second;
	}

	START_LOG( cwarn, L"Not Found TRUser : " << iUID_ );

	return KGarenaBillingUserPtr();
}

void KGarenaBillingUserManager::Tick()
{
	KLocker lock( m_cslist );

	std::list<KGarenaBillingUserPtr>::iterator lit;
	lit = m_listSessionTable.begin();

	while(lit != m_listSessionTable.end())
	{
		(*lit)->Tick();

		if( (*lit)->GetStateID() == KGarenaBillingUserFSM::STATE_EXIT )
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

bool KGarenaBillingUserManager::Insert( KGarenaBillingUserPtr pkTRUser_ )
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
	std::map< UidType, KGarenaBillingUserPtr >::iterator mit = m_mapSessionTable.find( pkTRUser_->GetUID() );
	if( mit != m_mapSessionTable.end() ) // 이미 등록된 UID에 대한 처리.
	{
		START_LOG( cerr, L"Already Exist")
			<< BUILD_LOG( pkTRUser_->GetUID() );
		return false;
	}

	m_mapSessionTable.insert( std::make_pair( pkTRUser_->GetUID(), pkTRUser_ ) );

	return true;
}


#endif SERV_COUNTRY_PH
//}}