#include "SimLayer.h"
#include "GlobalSimLayer.h"
#include "ServerSNManager.h"
#include "Socket/NetCommon.h"
#include "Enum/Enum.h"
#include "NetError.h"


//////////////////////////////////////////////////////////////////////////
//{{ 2012. 03. 29	김민성		중복 sn 접속 감시
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//////////////////////////////////////////////////////////////////////////

ImplementRefreshSingleton( KServerSNManager );

KServerSNManager::KServerSNManager(void)
{
}

KServerSNManager::~KServerSNManager(void)
{
}

ImplToStringW( KServerSNManager )
{
	stm_	<< L"----------[ ServerSNManager ]----------" << std::endl
			<< m_mapServerSNManager.size()
			;

	return stm_;
}

void KServerSNManager::RegServerSN( IN __int64& iServerSN, IN UidType& iUserUID, IN UidType& iGSUID, OUT int& iResult, OUT std::map<UidType, UidType>& mapOverlapUser )
{
	// 현재 접속한 ServerSN 을 등록한다.
	iResult = NetError::NET_OK;

	// 중복 접속 중이면...
	std::map< __int64, std::map<UidType, UidType> >::iterator mit = m_mapServerSNManager.find( iServerSN );
	if( mit != m_mapServerSNManager.end() )
	{
		iResult = NetError::ERR_SN_OVERLAP_01;

		START_LOG( cerr, L"ServerSN이 중복 접속 중이다." )
			<< BUILD_LOG( iServerSN )
			<< END_LOG;

		mit->second.insert( std::make_pair(iUserUID, iGSUID) );
		mapOverlapUser = mit->second;
		return;
	}

	std::map<UidType, UidType>	mapTemp;
	mapTemp.insert( std::make_pair(iUserUID,iGSUID) );
	m_mapServerSNManager.insert( std::make_pair( iServerSN, mapTemp ) );
}

void KServerSNManager::UnRegServerSN( IN __int64& iServerSN, IN UidType& iUserUID, OUT int& iResult )
{
	// 현재 접속 종료한 ServerSN 을 삭제한다.
	iResult = NetError::NET_OK;

	// 삭제하려는데 없으면...
	std::map< __int64, std::map<UidType, UidType> >::iterator mit = m_mapServerSNManager.find( iServerSN );
	if( mit == m_mapServerSNManager.end() )
	{
		iResult = NetError::ERR_SN_OVERLAP_02;

		START_LOG( cerr, L"ServerSN 존재하지 않는데 삭제하려한다." )
			<< BUILD_LOG( iServerSN )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;

		return;
	}

	if( mit->second.size() > 1 )
	{
		std::map<UidType, UidType>::iterator sit = mit->second.find( iUserUID );
		if( sit == mit->second.end() )
		{
			iResult = NetError::ERR_SN_OVERLAP_02;

			START_LOG( cerr, L"iUserUID가 존재하지 않는다." )
				<< BUILD_LOG( iServerSN )
				<< BUILD_LOG( iUserUID )
				<< END_LOG;
			return;
		}

		mit->second.erase( sit );
	}
	else
	{
		m_mapServerSNManager.erase( mit );
	}
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}
//////////////////////////////////////////////////////////////////////////
