#include "SADatabase.h"
#include <cryptopp/osrng.h>
#include "KncSecurity.h"
#include "dbg/dbg.hpp"

#undef max
#include <boost/random.hpp>
#include <limits>

NAMESPACE_BEGIN( KncSecurity )

KSADatabase::KSADatabase()
{
    InitializeCriticalSection( &m_csSaList );

    KLocker lock( m_csSaList );

	// 2009-10-16 육사 comment
	// SADatabase의 생성자에서는 기본적으로 SPIndex값이 0인 default KSecurityAssociation을 만들어둔다.
	// TCP Connection을 맺고나서 클라이언트는 서버로부터 첫번째로 E_ACCEPT_CONNECTION_NOT 이벤트를 통해 SPIndex를 받아야하는데
	// 그 패킷이 Valid한 패킷인지 체크할때 SPIndex값이 0인 상태에서 체크하기 때문임.
	
    m_mapSaList.insert( std::pair<SpiType, KSecurityAssociation>( 0, KSecurityAssociation() ) );
}

KSADatabase::~KSADatabase()
{
    DeleteCriticalSection( &m_csSaList );
}

void KSADatabase::Insert( OUT SpiType& nSPIndex, IN const KSecurityAssociation& sa )
{
    static boost::mt19937 rng;
    static boost::uniform_int<DWORD> uint32( 1, std::numeric_limits<SpiType>::max() );   // 1 이상. SPI 0은 생성자에서 이미 설정했다.
    static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);

    KLocker lock( m_csSaList );

    for( ;; )
    {
        // Generate a random number
        nSPIndex = static_cast<SpiType>( die() );

        // Search for the number in the database. If it's not there, use it.
        if( !Find( nSPIndex ) )
            break;
    }

    m_mapSaList.insert( std::pair<SpiType, KSecurityAssociation>( nSPIndex, sa ) );
}

void KSADatabase::Delete( SpiType nSPIndex )
{
    KLocker lock( m_csSaList );

    m_mapSaList.erase( nSPIndex );
}

bool KSADatabase::Find( SpiType nSPIndex ) const
{
    KLocker lock( m_csSaList );

    return m_mapSaList.find( nSPIndex ) != m_mapSaList.end();
}

KSecurityAssociation& KSADatabase::GetSA( SpiType nSPIndex )
{
    KLocker lock( m_csSaList );

    std::map<SpiType, KSecurityAssociation>::iterator i;
    i = m_mapSaList.find( nSPIndex );
    
    LIF( i != m_mapSaList.end() );

    // 찾고자 하는 SA가 없는 경우 SPI 0 으로 설정된 상수키를 리턴한다.
    if( i == m_mapSaList.end() ) i = m_mapSaList.begin();

    return i->second;
}

const KSecurityAssociation& KSADatabase::GetSA( SpiType nSPIndex ) const
{
    KLocker lock( m_csSaList );

    std::map<SpiType, KSecurityAssociation>::const_iterator i;
    i = m_mapSaList.find( nSPIndex );

    LIF( i != m_mapSaList.end() );

    // 찾고자 하는 SA가 없는 경우 SPI 0 으로 설정된 상수키를 리턴한다.
    if( i == m_mapSaList.end() ) i = m_mapSaList.begin();

    return i->second;
}

const KSecurityAssociation* KSADatabase::CreateNewSA( OUT SpiType& nSPIndex )
{
    KSecurityAssociation sa;
    sa.ResetRandomizeKey();

    Insert( nSPIndex, sa );

    return &GetSA( nSPIndex );
}

NAMESPACE_END