#include "UserJumpingCharacterManager.h"

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-11	// 박세훈
#include "KncUtil.h"

using namespace JumpingCharacter;

KUserJumpingCharacterManager::KUserJumpingCharacterManager()
: m_bInit( false )
{
}

int KUserJumpingCharacterManager::Init( IN const std::map< UidType, std::wstring >& mapJumpingInfo )
{
	if( IsInit() == true )
	{
		return JCR_ALREADY_INIT;
	}

	m_mapInfo.clear();
	for( std::map< UidType, std::wstring >::const_iterator it = mapJumpingInfo.begin(); it != mapJumpingInfo.end(); ++it )
	{
		CTime tRegDate = 0;
		if( KncUtil::ConvertStringToCTime( it->second, tRegDate ) == false )
		{
			return JCR_FAILED;
		}

		m_mapInfo.insert( MAP_JIC::value_type( it->first, tRegDate ) );
	}

	m_bInit = true;

	return JCR_SUCCEED;
}

int KUserJumpingCharacterManager::Check( IN const UidType& iUnitUID, IN const CTime& tEventBeginDate, IN const CTime& tEventEndDate, IN const bool bAccountEvent ) const
{
	if( IsInit() == false )
	{
		return JCR_NOT_INIT;
	}

	for( MAP_JIC::const_iterator it = m_mapInfo.begin(); it != m_mapInfo.end(); ++it )
	{
		if( it->second < tEventBeginDate )
		{
			continue;
		}

		if( tEventEndDate < it->second )
		{
			continue;
		}

		if( ( bAccountEvent == false ) && ( it->first != iUnitUID ) )
		{
			continue;
		}

		return JCR_FAILED;
	}

	return JCR_SUCCEED;
}

int KUserJumpingCharacterManager::SetJumpingCharacter( IN const UidType iUnitUID, IN const CTime tRegDate )
{
	if( IsInit() == false )
	{
		return JCR_NOT_INIT;
	}

	if( m_mapInfo.insert( MAP_JIC::value_type( iUnitUID, tRegDate ) ).second == false )
	{
		return JCR_FAILED;
	}

	return JCR_SUCCEED;
}

int KUserJumpingCharacterManager::ResetJumpingCharacter( IN const UidType iUnitUID )
{
	if( IsInit() == false )
	{
		return JCR_NOT_INIT;
	}

	m_mapInfo.erase( iUnitUID );

	return JCR_SUCCEED;
}

int KUserJumpingCharacterManager::IsJumpingCharacter( IN const UidType iUnitUID ) const
{
	if( IsInit() == false )
	{
		return JCR_NOT_INIT;
	}

	if( m_mapInfo.find( iUnitUID ) == m_mapInfo.end() )
	{
		return JCR_FAILED;
	}

	return JCR_SUCCEED;
}
#endif // SERV_JUMPING_CHARACTER