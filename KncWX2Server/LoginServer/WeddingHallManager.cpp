#include "WeddingHallManager.h"
#include "NetError.h"
#include "kncutil.h"
#include "Enum/Enum.h"
#include "BaseServer.h"

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM

ImplementRefreshSingleton( KWeddingHallManager );

ImplToStringW( KWeddingHallManager )
{
	return stm_;
}

KWeddingHallManager::KWeddingHallManager()
{
}

KWeddingHallManager::~KWeddingHallManager()
{
}

ImplementLuaScriptParser( KWeddingHallManager )
{

}

void KWeddingHallManager::Init()
{
	// DB 에 있는 결혼식 정보를 요청한다.
	SendToGameDB( DBE_LOAD_WEDDING_HALL_INFO_REQ );
}

void KWeddingHallManager::ShutDown()
{
	// Manager 에 있는 결혼식 정보를 DB 에 업데이트 한다.
	UpdateWeddingInfo_DB();
}

void KWeddingHallManager::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}

void KWeddingHallManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

void KWeddingHallManager::SetWeddingInfoDB( IN std::map< int, KWeddingHallInfo >& mapWeddingInfo )
{
	// 최초 DB 정보를 바탕으로 셋팅하는 함수

	std::map< int, KWeddingHallInfo >::iterator mitNew = mapWeddingInfo.begin();
	for( ; mitNew != mapWeddingInfo.end() ; ++mitNew )
	{
		CTime tWeddingDate;
		KncUtil::ConvertStringToCTime( mitNew->second.m_wstrWeddingDate, tWeddingDate );
		mitNew->second.m_tWeddingDate = tWeddingDate.GetTime();

		std::map< int, KWeddingHallInfo >::iterator mitOld = m_mapWeddingHallDBData.find( mitNew->first );
		if( mitOld == m_mapWeddingHallDBData.end() )
		{
			m_mapWeddingHallDBData.insert( std::make_pair( mitNew->first, mitNew->second ));
		}
		else
		{
			// 존재한다면 어떻게 할 것인가? // DB 에서 온 데이터 이므로 덮어 쓰자
			mitOld->second = mitNew->second;
		}
	}
}

void KWeddingHallManager::UpdateWeddingInfo_DB()
{
	// 로그인 서버 종료 할 때 호출 됨

	// 결혼 시간이 넘긴 데이터
	// 결혼이 완료된 데이터
	// 방UID 0으로 초기화? 고민 좀 해보자

	CTime tCurr = CTime::GetCurrentTime();

	KDBE_WEDDING_HALL_INFO_UPDATE_NOT kPacketToDB;

	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.begin();
	for( ; mit != m_mapWeddingHallDBData.end() ; ++mit )
	{
		KWeddingHallInfo kInfo = mit->second;

		if( mit->second.m_bSuccess == true )
		{
			kInfo.m_bDelete = true;
		}

		if( mit->second.m_tWeddingDate < tCurr.GetTime() )
		{
			kInfo.m_bDelete = true;
		}
		
		kPacketToDB.m_mapWeddingInfo.insert( std::make_pair( kInfo.m_iWeddingUID, kInfo ) );

		if( kPacketToDB.m_mapWeddingInfo.size() >= 100 )
		{
			SendToGameDB( DBE_WEDDING_HALL_INFO_UPDATE_NOT, kPacketToDB );
			kPacketToDB.m_mapWeddingInfo.clear();
		}
	}

	SendToGameDB( DBE_WEDDING_HALL_INFO_UPDATE_NOT, kPacketToDB );
}

bool KWeddingHallManager::AddWeddingInfo( IN KWeddingHallInfo& kWeddingInfo )
{
	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.find( kWeddingInfo.m_iWeddingUID );
	if( mit != m_mapWeddingHallDBData.end() )
	{
		START_LOG( cerr, L"[테스트]존재하는 결혼식 정보 입니다. 중복 정보 입력 하려 합니다." )
			<< BUILD_LOG( kWeddingInfo.m_iWeddingUID )
			<< BUILD_LOG( kWeddingInfo.m_iGroom )
			<< BUILD_LOG( kWeddingInfo.m_iBride )
			<< BUILD_LOGc( kWeddingInfo.m_cWeddingHallType )
			<< BUILD_LOGc( kWeddingInfo.m_cOfficiantNPC )
			<< BUILD_LOG( kWeddingInfo.m_wstrWeddingDate )
			<< BUILD_LOG( kWeddingInfo.m_bSuccess )
			<< BUILD_LOG( kWeddingInfo.m_bDelete )
			<< END_LOG;

		return false;
	}

	m_mapWeddingHallDBData.insert( std::make_pair( kWeddingInfo.m_iWeddingUID, kWeddingInfo ));
	return true;
}

bool KWeddingHallManager::UpdateWeddingInfo( IN KWeddingHallInfo& kWeddingInfo )
{
	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.find( kWeddingInfo.m_iWeddingUID );
	if( mit == m_mapWeddingHallDBData.end() )
	{
		START_LOG( cerr, L"[테스트]존재하지 않는 결혼식 UID 가 아닙니다." )
			<< BUILD_LOG( kWeddingInfo.m_iWeddingUID )
			<< END_LOG;

		return false;
	}

	mit->second = kWeddingInfo;
	return true;
}

bool KWeddingHallManager::DeleteWeddingInfo( IN int iWeddingUID )
{
	if( iWeddingUID <= 0 )
	{
		START_LOG( cerr, L"정상적인 결혼식 UID 가 아닙니다." )
			<< BUILD_LOG( iWeddingUID )
			<< END_LOG;

		return false;
	}

	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.find( iWeddingUID );
	if( mit == m_mapWeddingHallDBData.end() )
	{
		START_LOG( cerr, L"[테스트]존재하지 않는 결혼식 UID 가 아닙니다." )
			<< BUILD_LOG( iWeddingUID )
			<< END_LOG;

		return false;
	}

	m_mapWeddingHallDBData.erase( mit );

	return true;
}

bool KWeddingHallManager::GetWeddingInfo( IN int iWeddingUID, OUT KWeddingHallInfo& kWeddingInfo )
{
	if( iWeddingUID <= 0 )
	{
		START_LOG( cerr, L"정상적인 결혼식 UID 가 아닙니다." )
			<< BUILD_LOG( iWeddingUID )
			<< END_LOG;

		return false;
	}

	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.find( iWeddingUID );
	if( mit == m_mapWeddingHallDBData.end() )
	{
		START_LOG( cerr, L"[테스트]존재하지 않는 결혼식 UID 가 아닙니다." )
			<< BUILD_LOG( iWeddingUID )
			<< END_LOG;

		return false;
	}

	if( mit->second.m_bSuccess == true )
	{
		return false;
	}

	kWeddingInfo = mit->second;

	return true;
}

bool KWeddingHallManager::GetWeddingItemInfo( IN OUT std::map< UidType, KWeddingItemInfo >& mapWeddingItemInfo )
{
	std::map< UidType, KWeddingItemInfo >::iterator mit = mapWeddingItemInfo.begin();
	for( ; mit != mapWeddingItemInfo.end() ; ++mit )
	{
		KWeddingHallInfo kWeddingInfo;
		if( GetWeddingInfo( mit->second.m_iWeddingUID, kWeddingInfo ) == false )
		{
			continue;
		}

		mit->second.m_cWeddingHallType = kWeddingInfo.m_cWeddingHallType;
		mit->second.m_cOfficiantNPC = kWeddingInfo.m_cOfficiantNPC;
		mit->second.m_iGroom = kWeddingInfo.m_iGroom;
		mit->second.m_iBride = kWeddingInfo.m_iBride;
		mit->second.m_wstrWeddingDate = kWeddingInfo.m_wstrWeddingDate;
		mit->second.m_wstrWeddingMsg = kWeddingInfo.m_wstrWeddingMsg;
	}
	
	return true;
}

#endif SERV_RELATIONSHIP_SYSTEM
//}


