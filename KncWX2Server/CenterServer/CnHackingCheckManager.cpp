#include ".\CnHackingCheckManager.h"
#include "X2Data\XSLDungeonManager.h"
#include "Enum\Enum.h"

//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK


ImplementRefreshSingleton( KCnHackingCheckManager );

KCnHackingCheckManager::KCnHackingCheckManager(void)
{
	m_bDungeonResultCheckEnable = false;
	//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	m_bDungeonClearTimeCheckEnable = false;
	m_iDungeonClearTimeCheckCount = 0;
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	//}}
#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
    m_bHenirClearTimeCheckEnable = false;
#endif SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
}

KCnHackingCheckManager::~KCnHackingCheckManager(void)
{
}

ImplToStringW( KCnHackingCheckManager )
{
	stm_	<< L"----------[ Cn Hacking Check Manager ]----------" << std::endl
			<< TOSTRINGW( m_bDungeonResultCheckEnable )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_bClear )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_iStartedMember )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_eComboRank )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_eTechnicalRank )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_eTimeRank )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_eDamagedRank )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_iTechnicalScore )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_iContinueCount )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_bHavePet )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_setCheckUnitClass.size() )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_setNoAdventDungeonID.size() )
			<< TOSTRINGW( m_kDungeonResultCheckData.m_iMinLevelCheck )
			//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
			<< TOSTRINGW( m_bDungeonClearTimeCheckEnable )
			<< TOSTRINGW( m_kDungeonClearTimeCheckData.m_bClear )
			<< TOSTRINGW( m_kDungeonClearTimeCheckData.m_iStartedMember )
			<< TOSTRINGW( m_kDungeonClearTimeCheckData.m_iCheckPlayTime )
			<< TOSTRINGW( m_kDungeonClearTimeCheckData.m_setCheckUnitClass.size() )
			<< TOSTRINGW( m_kDungeonClearTimeCheckData.m_setNoAdventDungeonID.size() )
			<< TOSTRINGW( m_setDungeonClearTimeCheckUserList.size() )
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
			//}}
#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
            << TOSTRINGW( m_bHenirClearTimeCheckEnable )
            //<< TOSTRINGW( m_kHenirClearTimeCheckData.m_iCheckPlayTime )
            << TOSTRINGW( m_kHenirClearTimeCheckData.m_mapStageClearTime.size() )
#endif SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
			;

	return stm_;
}

ImplementLuaScriptParser( KCnHackingCheckManager )
{
	lua_tinker::class_add<KCnHackingCheckManager>( GetLuaState(), "KCnHackingCheckManager" );
	lua_tinker::class_def<KCnHackingCheckManager>( GetLuaState(), "SetDungeonResultCheckEnable",			&KCnHackingCheckManager::SetDungeonResultCheckEnable_LUA );
	lua_tinker::class_def<KCnHackingCheckManager>( GetLuaState(), "SetDungeonResultHackUserCheckData",		&KCnHackingCheckManager::SetDungeonResultHackUserCheckData_LUA );
	//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	lua_tinker::class_def<KCnHackingCheckManager>( GetLuaState(), "SetDungeonClearTimeCheckEnable",			&KCnHackingCheckManager::SetDungeonClearTimeCheckEnable_LUA );
	lua_tinker::class_def<KCnHackingCheckManager>( GetLuaState(), "SetDungeonClearTimeHackUserCheckData",	&KCnHackingCheckManager::SetDungeonClearTimeHackUserCheckData_LUA );
	lua_tinker::class_def<KCnHackingCheckManager>( GetLuaState(), "AddPhoneNum",							&KCnHackingCheckManager::AddPhoneNum_LUA );
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	//}}

#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
    lua_tinker::class_def<KCnHackingCheckManager>( GetLuaState(), "SetHenirClearTimeCheckEnable",			&KCnHackingCheckManager::SetHenirClearTimeCheckEnable_LUA );
    lua_tinker::class_def<KCnHackingCheckManager>( GetLuaState(), "SetHenirClearTimeHackUserCheckData",	    &KCnHackingCheckManager::SetHenirClearTimeHackUserCheckData_LUA );
#endif SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK

	lua_tinker::decl( GetLuaState(), "g_pCnHackingCheckManager", this );
}

void KCnHackingCheckManager::SetDungeonResultCheckEnable_LUA( bool bVal )
{
	m_bDungeonResultCheckEnable = bVal;

	START_LOG( cout, L"던전 클리어 결과 핵 유저 체크 동작 여부 설정!" )
		<< BUILD_LOG( IsDungeonResultCheckEnable() );
}

bool KCnHackingCheckManager::SetDungeonResultHackUserCheckData_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	//TableBind( &luaManager );

	LUA_GET_VALUE_RETURN(		luaManager, L"m_bClear",				m_kDungeonResultCheckData.m_bClear,				true,			return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iStartedMember",		m_kDungeonResultCheckData.m_iStartedMember,		0,				return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eComboRank",			m_kDungeonResultCheckData.m_eComboRank,			CXSLDungeon::RANK_TYPE,	 CXSLDungeon::RT_NONE,		return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eTechnicalRank",		m_kDungeonResultCheckData.m_eTechnicalRank,		CXSLDungeon::RANK_TYPE,	 CXSLDungeon::RT_NONE,		return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eTimeRank",				m_kDungeonResultCheckData.m_eTimeRank,			CXSLDungeon::RANK_TYPE,	 CXSLDungeon::RT_NONE,		return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDamagedRank",			m_kDungeonResultCheckData.m_eDamagedRank,		CXSLDungeon::RANK_TYPE,	 CXSLDungeon::RT_NONE,		return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iTechnicalScore",		m_kDungeonResultCheckData.m_iTechnicalScore,	0,				return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iContinueCount",		m_kDungeonResultCheckData.m_iContinueCount,		0,				return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_bHavePet",				m_kDungeonResultCheckData.m_bHavePet,			true,			return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iMinLevelCheck",		m_kDungeonResultCheckData.m_iMinLevelCheck,		0,				return false; );

	if( luaManager.BeginTable( L"CHECK_UNIT_CLASS" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf >= 0 )
			{
				m_kDungeonResultCheckData.m_setCheckUnitClass.insert( buf );
			}
			index++;
		}

		luaManager.EndTable();
	}

	if( luaManager.BeginTable( L"NO_ADVENT_DUNGEON_TABLE" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf >= 0 )
			{
				m_kDungeonResultCheckData.m_setNoAdventDungeonID.insert( buf );
			}				
			index++;
		}

		luaManager.EndTable();
	}

	START_LOG( cout, L"[ 던전 결과 핵 유저 체크 설정 ]" )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_bClear )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_iStartedMember )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_eComboRank )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_eTechnicalRank )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_eTimeRank )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_eDamagedRank )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_iTechnicalScore )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_iContinueCount )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_bHavePet )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_setCheckUnitClass.size() )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_setNoAdventDungeonID.size() )
		<< BUILD_LOG( m_kDungeonResultCheckData.m_iMinLevelCheck );

	return true;
}

//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK

void KCnHackingCheckManager::SetDungeonClearTimeCheckEnable_LUA( bool bVal )
{
	m_bDungeonClearTimeCheckEnable = bVal;

	START_LOG( cout, L"던전 클리어 타임 핵 유저 체크 동작 여부 설정!" )
		<< BUILD_LOG( IsDungeonClearTimeCheckEnable() );
}

bool KCnHackingCheckManager::SetDungeonClearTimeHackUserCheckData_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	//TableBind( &luaManager );

	LUA_GET_VALUE_RETURN(		luaManager, L"m_bClear",				m_kDungeonClearTimeCheckData.m_bClear,			true,			return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iStartedMember",		m_kDungeonClearTimeCheckData.m_iStartedMember,	0,				return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iMinLevelCheck",		m_kDungeonClearTimeCheckData.m_iMinLevelCheck,	0,				return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_iCheckPlayTime",		m_kDungeonClearTimeCheckData.m_iCheckPlayTime,	0,				return false; );

	if( luaManager.BeginTable( L"CHECK_UNIT_CLASS" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf >= 0 )
			{
				m_kDungeonClearTimeCheckData.m_setCheckUnitClass.insert( buf );
			}
			index++;
		}

		luaManager.EndTable();
	}

	if( luaManager.BeginTable( L"NO_ADVENT_DUNGEON_TABLE" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf >= 0 )
			{
				m_kDungeonClearTimeCheckData.m_setNoAdventDungeonID.insert( buf );
			}				
			index++;
		}

		luaManager.EndTable();
	}

	START_LOG( cout, L"[ 던전 클리어 타임 핵 유저 체크 설정 ]" )
		<< BUILD_LOG( m_kDungeonClearTimeCheckData.m_bClear )
		<< BUILD_LOG( m_kDungeonClearTimeCheckData.m_iStartedMember )
		<< BUILD_LOG( m_kDungeonClearTimeCheckData.m_setCheckUnitClass.size() )
		<< BUILD_LOG( m_kDungeonClearTimeCheckData.m_setNoAdventDungeonID.size() )
		<< BUILD_LOG( m_kDungeonClearTimeCheckData.m_iCheckPlayTime );

	return true;
}

void KCnHackingCheckManager::AddPhoneNum_LUA( const char* pszPhoneNum )
{
	m_vecPhoneNumList.push_back( KncUtil::toWideString( pszPhoneNum ) );
}

#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
//}}

#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
void KCnHackingCheckManager::SetHenirClearTimeCheckEnable_LUA( bool bVal )
{
    m_bHenirClearTimeCheckEnable = bVal;

    START_LOG( cout, L"헤니르시공 클리어 타임 핵 유저 체크 동작 여부 설정!" )
        << BUILD_LOG( IsHenirClearTimeCheckEnable() );
}
bool KCnHackingCheckManager::SetHenirClearTimeHackUserCheckData_LUA()
{
    KLuaManager luaManager( GetLuaState() );

    //LUA_GET_VALUE_RETURN( luaManager, L"m_iCheckPlayTime", m_kHenirClearTimeCheckData.m_iCheckPlayTime, 0, return false; );

    int nTableIndex = 1;
    while( luaManager.BeginTable( nTableIndex ) == S_OK )
    {
        int iStageID = -1;
        int iClearTime = 0;
        LUA_GET_VALUE( luaManager, L"m_iStageID", iStageID, -1 );
        LUA_GET_VALUE( luaManager, L"m_iClearTime", iClearTime, 0 );

        m_kHenirClearTimeCheckData.m_mapStageClearTime.insert( std::make_pair( iStageID, iClearTime ) );

        luaManager.EndTable();
        ++nTableIndex;
    }

    return true;
}
#endif SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK

bool KCnHackingCheckManager::CheckDungeonResultAutoHackUser( IN const int iDungeonID, 
															IN const char cDifficultyLevel,
															IN const u_char ucUnitClass, 
															IN const int iUnitLevel,
															IN const bool bClear, 
															IN const int iStartedMember, 
															IN const CXSLDungeon::RANK_TYPE eComboRank,
															IN const CXSLDungeon::RANK_TYPE eTechnicalRank,
															IN const CXSLDungeon::RANK_TYPE eTimeRank,
															IN const CXSLDungeon::RANK_TYPE eDamagedRank,
															IN const int iTechnicalScore,
															IN const int iContinueCount,
															IN const bool bHavePet
															) const
{
	// 1. 해당 되지 않는 던전이면 패스!
	if( m_kDungeonResultCheckData.m_setNoAdventDungeonID.find( iDungeonID ) != m_kDungeonResultCheckData.m_setNoAdventDungeonID.end() )
		return false;

	// 2. 해당 되지 않는 UnitClass면 패스!
	if( m_kDungeonResultCheckData.m_setCheckUnitClass.find( ucUnitClass ) == m_kDungeonResultCheckData.m_setCheckUnitClass.end() )
		return false;

	// 3. 적정 레벨 던전인지 체크!
	const int iLevelDif =  abs( SiCXSLDungeonManager()->GetDungeonMinLevel( iDungeonID + static_cast<int>(cDifficultyLevel) ) - iUnitLevel );
	if( m_kDungeonResultCheckData.m_iMinLevelCheck <= iLevelDif )
		return false;

	// 3. 클리어 조건
	if( m_kDungeonResultCheckData.m_bClear != bClear )
		return false;

	// 4. 시작 멤버
	if( m_kDungeonResultCheckData.m_iStartedMember != iStartedMember )
		return false;

	// 5. 콤보 랭크
	//if( m_kDungeonCheck.m_eComboRank != eComboRank )
	//	return false;

	// 6. 테크리컬 랭크
	//if( m_kDungeonCheck.m_eTechnicalRank != eTechnicalRank )
	//	return false;

	// 7. 타임 랭크
	//if( m_kDungeonCheck.m_eTimeRank != eTimeRank )
	//	return false;

	// 8. 대미지 랭크
	//if( m_kDungeonCheck.m_eDamagedRank != eDamagedRank )
	//	return false;

	// 9. 테크니컬 스코어
	if( m_kDungeonResultCheckData.m_iTechnicalScore < iTechnicalScore )
		return false;

	// 10. 컨티뉴 횟수
	if( m_kDungeonResultCheckData.m_iContinueCount < iContinueCount )
		return false;

	// 11. 펫 보유 여부
	if( m_kDungeonResultCheckData.m_bHavePet != bHavePet )
		return false;

	// 해킹 유저로 판단!
    return true;
}


#endif SERV_CN_SERVER_HACKING_CHECK
//}}

//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK

bool KCnHackingCheckManager::CheckDungeonClearTimeAutoHackUser( IN const char cAuthLevel,
															    IN const UidType iUserUID,
															    IN const int iDungeonID, 
																IN const char cDifficultyLevel,
																IN const u_char ucUnitClass,
																IN const int iUnitLevel,
																IN const bool bClear,
																IN const int iStartedMember, 
																IN const int iPlayTime 
																)
{
	// 1. 해당 되지 않는 던전이면 패스!
	if( m_kDungeonClearTimeCheckData.m_setNoAdventDungeonID.find( iDungeonID ) != m_kDungeonClearTimeCheckData.m_setNoAdventDungeonID.end() )
		return false;

	// 2. 해당 되지 않는 UnitClass면 패스!
	if( m_kDungeonClearTimeCheckData.m_setCheckUnitClass.find( ucUnitClass ) == m_kDungeonClearTimeCheckData.m_setCheckUnitClass.end() )
		return false;

	// 3. 적정 레벨 던전인지 체크!
	const int iLevelDif =  abs( SiCXSLDungeonManager()->GetDungeonMinLevel( iDungeonID + static_cast<int>(cDifficultyLevel) ) - iUnitLevel );
	if( m_kDungeonClearTimeCheckData.m_iMinLevelCheck <= iLevelDif )
		return false;

	// 3. 클리어 조건
	if( m_kDungeonClearTimeCheckData.m_bClear != bClear )
		return false;

	// 4. 시작 멤버
	if( m_kDungeonClearTimeCheckData.m_iStartedMember != iStartedMember )
		return false;

	// 9. 테크니컬 스코어
	if( m_kDungeonClearTimeCheckData.m_iCheckPlayTime < iPlayTime )
		return false;

	// 10. 운영자라면 패스!
	if( cAuthLevel >= SEnum::UAL_GM )
	{
		START_LOG( cout, L"비정상적인 플레이를 했지만 운영자 등급이므로 핵유저로 처리하지 않습니다!" )
			<< BUILD_LOGc( cAuthLevel )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( iDungeonID )
			<< BUILD_LOGc( cDifficultyLevel )
			<< BUILD_LOGc( ucUnitClass )
			<< BUILD_LOG( iUnitLevel )
			<< BUILD_LOG( bClear )
			<< BUILD_LOG( iStartedMember )
			<< BUILD_LOG( iPlayTime );
		return false;
	}

	// 해킹 유저로 판단!
	m_setDungeonClearTimeCheckUserList.insert( iUserUID );
	++m_iDungeonClearTimeCheckCount;
	return true;
}

void KCnHackingCheckManager::Tick()
{
	if( m_tDungeonClearTimeCheckTimer.elapsed() < 1800.0 )
		return;

	m_tDungeonClearTimeCheckTimer.restart();
    
	if( m_setDungeonClearTimeCheckUserList.empty() == true )
		return;

	const int iDungeonClearTimeCheckCount = m_iDungeonClearTimeCheckCount;
	m_iDungeonClearTimeCheckCount = 0;
	const u_int uiCheckUserCount = m_setDungeonClearTimeCheckUserList.size();
	m_setDungeonClearTimeCheckUserList.clear();

	KDBE_SEND_PHONE_MSG_NOT kPacketNot;
	//{{ 2012. 10. 15	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_DUNGEON_CLEAR_TIME_HACK_USER_CHECK, kPacketNot.m_vecPhoneNum );
#else
	kPacketNot.m_vecPhoneNum = m_vecPhoneNumList;
#endif SERV_SMS_TOTAL_MANAGER
	//}}

	CTime tCurTime = CTime::GetCurrentTime();
	CTime t30MinutesBeforeTime = tCurTime - CTimeSpan( 0, 0, 30, 0 );

	kPacketNot.m_wstrSMSMessage += L"[던전10초만에깬 유저 출현!] ";
	kPacketNot.m_wstrSMSMessage += (const wchar_t*)t30MinutesBeforeTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	kPacketNot.m_wstrSMSMessage += L" ~ ";
	kPacketNot.m_wstrSMSMessage += (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	kPacketNot.m_wstrSMSMessage += L", [";
	kPacketNot.m_wstrSMSMessage += boost::str( boost::wformat( L"%d회" ) % iDungeonClearTimeCheckCount );
	kPacketNot.m_wstrSMSMessage += L"]";
	kPacketNot.m_wstrSMSMessage += L", [";
	kPacketNot.m_wstrSMSMessage += boost::str( boost::wformat( L"%d계정" ) % uiCheckUserCount );
	kPacketNot.m_wstrSMSMessage += L"]";

	SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );

	START_LOG( cout, L"오토핵 유저 출현! SMS 문자 전송!" )
		<< BUILD_LOG( kPacketNot.m_wstrSMSMessage )
		<< BUILD_LOG( m_vecPhoneNumList.size() );
}

#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
//}}


#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK

bool KCnHackingCheckManager::CheckHenirClearTimeHackUser( IN const char cAuthLevel_,
    IN const UidType iUserUID_,
    IN const int iDungeonID_, 
    IN const int iStageID_,
    //IN const char cDifficultyLevel,
    //IN const u_char ucUnitClass,
    //IN const int iUnitLevel,
    //IN const bool bClear,
    //IN const int iStartedMember, 
    IN const int iPlayTime_
    )
{
    // 헤니르 시공인지 확인
    if ( iDungeonID_ != SEnum::DI_ELDER_HENIR_SPACE ) 
    {
        return false;
    }

    // 스테이지별 클리어 타임 체크
    if ( CheckStageMinimumClearTime( iStageID_, iPlayTime_ ) == false )
    {
        return false;
    }
    // 1. 클리어 타임.
    //if( m_kHenirClearTimeCheckData.m_iCheckPlayTime < iPlayTime_ )
        //return false;

    // 2. 운영자라면 패스!
    //if( cAuthLevel_ >= SEnum::UAL_GM )
    //{
    //    START_LOG( cout, L"헤니르 시공에서 비정상적인 플레이를 했지만 운영자 등급이므로 핵유저로 처리하지 않습니다!" )
    //        << BUILD_LOGc( cAuthLevel_ )
    //        << BUILD_LOG( iUserUID_ )
    //        << BUILD_LOG( iDungeonID_ )
    //        //<< BUILD_LOGc( cDifficultyLevel )
    //        //<< BUILD_LOGc( ucUnitClass )
    //        //<< BUILD_LOG( iUnitLevel )
    //        //<< BUILD_LOG( bClear )
    //        //<< BUILD_LOG( iStartedMember )
    //        << BUILD_LOG( iPlayTime_ );
    //    return false;
    //}

    return true;
}

bool KCnHackingCheckManager::CheckStageMinimumClearTime( IN const int iStageID_, IN const int iPlayTime_ )
{
    //std::map< int, int >::const_iterator mit;

    //mit = m_kHenirClearTimeCheckData.m_mapStageClearTime.find( iStageID_ );

    //if ( mit == m_kHenirClearTimeCheckData.m_mapStageClearTime.end() ) 
    //{ // 스테이지 설정 안된 경우가 있으므로, 해킹 아닌 것으로 간주. (모든 스테이지를 스크립트에 등록하지는 않는다) 
    //    return false;
    //}

    //if ( mit->second > iPlayTime_ ) 
    //{
    //    return true;
    //}
    
    // 설계 의도 : 하멜이 스테이지ID 40 번, 샌더가 50번이라고 가정하면, 45번 스테이지에서 던전을 클리어한 경우에 40번 기준 시간보다 짧으면 해킹으로 탐지
    BOOST_TEST_FOREACH( const KMapStageClearTime::value_type& , data, m_kHenirClearTimeCheckData.m_mapStageClearTime )
    {
        if ( data.first > iStageID_ ) // 스테이지 ID > 클리어한 스테이지 ID = 해킹 기준 스테이지 범위가 아니다
        {
            continue;
        }

        if ( data.second > iPlayTime_ ) // 기준 스테이지 이상의 스테이지인데  기준 스테이지 클리어 시간보다 짧으면 해킹
        {
            return true;
        }
    }

    return false;
}
#endif SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
