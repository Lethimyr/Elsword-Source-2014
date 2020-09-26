#include ".\gameeventscriptmanager.h"
#include "BaseServer.h"
#include "X2Data/XSLDungeon.h"
//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	#include "Enum/Enum.h"
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
//}}



ImplementRefreshSingleton( KGameEventScriptManager );

KGameEventScriptManager::KGameEventScriptManager(void)
{
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	m_iNotifyCnServerUID = 0;
	m_bNotifyMSGEvent	 = false;
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	m_iTemp_1 = 0;
	m_iTemp_2 = 0;
	m_iTemp_3 = 0;
	m_cTimeNextDay = CTime::GetCurrentTime();
	m_iItemID = 0;
	m_iItemNum_1 = 0;
	m_iItemNum_2 = 0;
	m_iItemNum_3 = 0;
	m_iItemGetCount = 0;
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
}

KGameEventScriptManager::~KGameEventScriptManager(void)
{
}

ImplToStringW( KGameEventScriptManager )
{
	stm_	<< L"----------[ Game Event Script Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapEventMonster.size() )
			//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
			<< TOSTRINGW( m_mapEventData.size() )
			<< TOSTRINGW( m_iNotifyCnServerUID )
			<< TOSTRINGW( m_bNotifyMSGEvent )
#endif SERV_EVENT_SCRIPT_REFRESH
			//}}
#ifdef SERV_ADVERTISEMENT_EVENT
			<< TOSTRINGW( m_mapAdvertisementData.size() )
#endif SERV_ADVERTISEMENT_EVENT
			;

	return stm_;
}

ImplementLuaScriptParser( KGameEventScriptManager )
{
	lua_tinker::class_add<KGameEventScriptManager>( GetLuaState(), "KGameEventScriptManager" );	
	lua_tinker::class_def<KGameEventScriptManager>( GetLuaState(), "dump",					&KGameEventScriptManager::Dump );
	lua_tinker::class_def<KGameEventScriptManager>( GetLuaState(), "AddEventMonsterInfo",	&KGameEventScriptManager::AddEventMonsterInfo_LUA );
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	lua_tinker::class_def<KGameEventScriptManager>( GetLuaState(), "AddReward",				&KGameEventScriptManager::AddReward_LUA );
	//{{ 2008. 7. 11  최육사  이벤트 공지
	lua_tinker::class_def<KGameEventScriptManager>( GetLuaState(), "AddNotifyMSGServerInfo",&KGameEventScriptManager::AddNotifyMSGServerInfo_LUA );
	//}}
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
	lua_tinker::class_def<KGameEventScriptManager>( GetLuaState(), "AddConnectEventUserGroup",&KGameEventScriptManager::AddConnectEventUserGroup_LUA );
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
	//}}
#ifdef SERV_ADVERTISEMENT_EVENT
	lua_tinker::class_def<KGameEventScriptManager>( GetLuaState(), "AddAdvertisement",		&KGameEventScriptManager::AddAdvertisement_LUA );
#endif SERV_ADVERTISEMENT_EVENT
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	lua_tinker::class_def<KGameEventScriptManager>( GetLuaState(), "AddCoboEventData",		&KGameEventScriptManager::AddCoboEventData_LUA );
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	lua_tinker::class_def<KGameEventScriptManager>( GetLuaState(), "AddValenTineEventData",		&KGameEventScriptManager::AddValenTineEventData_LUA );
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

	lua_tinker::decl( GetLuaState(), "GameEvent", this );
}

bool KGameEventScriptManager::AddEventMonsterInfo_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	//TableBind( &luaManager );

	KAdventNpcEventInfo kInfo;

	LUA_GET_VALUE_RETURN(	luaManager, L"m_iEventID",				kInfo.m_iEventID,					0,			return false; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_iMonsterDropCount",		kInfo.m_iMonsterDropCount,			0,			return false; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_bIsAdventInBossStage",	kInfo.m_bIsAdventInBossStage,		false,		return false; );
	
	//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
	LUA_GET_VALUE( luaManager,		 L"m_bAllyNpc",			kInfo.m_bAllyNPC,		false );
#endif SERV_ALLY_EVENT_MONSTER
	//}}


	if( luaManager.BeginTable( "ADVENT_NPC_TABLE" ) == S_OK )
	{
		for( int i = 1; luaManager.BeginTable( i ) != E_FAIL; ++i )
		{
			int iNpcID = 0;
			float fRate = 0.f;
#ifdef SERV_ACTIVE_EVENT_MONSTER
			bool bActiveNpc = true;
#endif SERV_ACTIVE_EVENT_MONSTER

			LUA_GET_VALUE( luaManager, "NPC_ID",			iNpcID,		0 );		
			LUA_GET_VALUE( luaManager, "ADVENT_RATE",		fRate,		0.f );
#ifdef SERV_ACTIVE_EVENT_MONSTER
			LUA_GET_VALUE( luaManager, "IS_ACTIVE",			bActiveNpc,	true);
			kInfo.m_mapMonsterActive.insert(std::make_pair(iNpcID, bActiveNpc));
#endif SERV_ACTIVE_EVENT_MONSTER

			LIF( kInfo.m_kAdventNpcLot.AddCase( iNpcID, fRate ) );

			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( L"ADVENT_DUNGEON_TABLE" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			kInfo.m_setAdventDungeon.insert( buf );
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
			kInfo.m_setNoAdventDungeon.insert( buf );
			index++;
		}

		luaManager.EndTable();
	}
	
	if( luaManager.BeginTable( L"NO_ADVENT_STAGE_TABLE" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf >= 0 )
			{
				kInfo.m_setNoAdventStage.insert( buf );
			}				
			index++;
		}

		luaManager.EndTable();
	}

	//{{ 2012. 12. 31	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	LUA_GET_VALUE(	luaManager, L"m_iAddLevel",		kInfo.m_iAddLevel,		0		);
	LUA_GET_VALUE(	luaManager, L"m_bDrop",			kInfo.m_bDrop,			true	);
	LUA_GET_VALUE(	luaManager, L"m_bAggressive",	kInfo.m_bAggressive,	true	);
	LUA_GET_VALUE(	luaManager, L"m_fAddPosY",		kInfo.m_fAddPosY,		0.0f	);

	if( luaManager.BeginTable( L"ADVENT_FIELD_TABLE" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			kInfo.m_setAdventField.insert( buf );
			++index;
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( L"NO_ADVENT_FIELD_TABLE" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			kInfo.m_setNoAdventField.insert( buf );
			++index;
		}
		luaManager.EndTable();
	}
#endif SERV_FIELD_EVENT_MONSTER
	//}}

	m_mapEventMonster.insert( std::make_pair( kInfo.m_iEventID, kInfo ) );
	return true;
}

//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
void KGameEventScriptManager::AddReward_LUA( IN int iID )
{
	//{{ 2011. 05. 31	최육사	접속 이벤트 레벨 체크
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
#else
	if( m_mapEventData.find( iID ) != m_mapEventData.end() )
	{
		START_LOG( cerr, L"같은 아이디의 보상이벤트 스크립트.!" )
			<< BUILD_LOG( iID )
			<< END_LOG;
		return;
	}
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
	//}}	

	KLuaManager luaMgr( GetLuaState() );
	EVENT_DATA sData;

	// 경험치,VP,ED 보너스 이벤트
	LUA_GET_VALUE( luaMgr,		"fEXPRate",				sData.m_fEXPRate,			0.f );
	LUA_GET_VALUE( luaMgr,		"fVPRate",				sData.m_fVPRate,			0.f );
	LUA_GET_VALUE( luaMgr,		"fEDRate",				sData.m_fEDRate,			0.f );
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
	LUA_GET_VALUE( luaMgr,		"fRankingPointRate",	sData.m_fRankingPointRate,	0.f );
#endif	// SERV_RANKING_POINT_EVENT

	// 이벤트 적용 조건
	LUA_GET_VALUE( luaMgr,		"iDungeonID",		sData.m_iDungeonID,		0 );
	LUA_GET_VALUE_ENUM( luaMgr, "iDungeonMode",		sData.m_iDungeonMode,	CXSLDungeon::DUNGEON_MODE, CXSLDungeon::DM_INVALID );
	LUA_GET_VALUE( luaMgr,		"iUnitLevel",		sData.m_iUnitLevel,		0 );	

	int iUnitType = 0;
	LUA_GET_VALUE( luaMgr,		"cUnitType",		iUnitType,				0 );
	sData.m_cUnitType = static_cast<char>(iUnitType);

	int iUnitClass = 0;
	LUA_GET_VALUE( luaMgr,		"cUnitClass",		iUnitClass,				0 );
	sData.m_cUnitClass = static_cast<char>(iUnitClass);

	LUA_GET_VALUE( luaMgr,		"iPartyNum",		sData.m_iPartyNum,		0 );

	// 아이템 이벤트
	LUA_GET_VALUE( luaMgr,		"fEventTime",		sData.m_fEventTime,		0.f );
	LUA_GET_VALUE( luaMgr,		"iRewardID",		sData.m_iEventReward,	0 );
	LUA_GET_VALUE( luaMgr,		"bGiveOnce",		sData.m_bGiveOnce,		true );
	LUA_GET_VALUE( luaMgr,		"bDelEndEvent",		sData.m_bDelEndEvent,	false ); // 종료 이벤트 삭제
	LUA_GET_VALUE( luaMgr,		"bPcBangEvent",		sData.m_bPcBangEvent,	false ); // PC방이벤트
	LUA_GET_VALUE( luaMgr,		"bDirectReward",	sData.m_bDirectReward,	false ); // 접속시간이벤트
	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
	LUA_GET_VALUE( luaMgr,		"bAccountEvent",	sData.m_bAccountEvent,	false );
#endif SERV_ACC_TIME_EVENT
	//}}
	//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	LUA_GET_VALUE( luaMgr,		"iChannelCode",		sData.m_iChannelCode,	-1 );
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	//}}
	//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	LUA_GET_VALUE( luaMgr,		"iBubbleFighterTeam",		sData.m_iBF_Team,	0 );
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}
	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	LUA_GET_VALUE( luaMgr,		"iHenirRewardEventCount",		sData.m_iHenirRewardEventCount,	0 );
	LUA_GET_VALUE( luaMgr,		"bHenirRewardUnLimited",		sData.m_bHenirRewardUnLimited,	false );
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2010. 07. 09  최육사	드롭률 이벤트 확장
#ifdef SERV_PC_BANG_DROP_EVENT

#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	LUA_GET_VALUE( luaMgr,		"fDropRate",		sData.m_fDropRate,		1.0f );
#else // SERV_DROP_EVENT_RENEWAL
	LUA_GET_VALUE( luaMgr,		"iDropCount",		sData.m_iDropCount,		1 );
#endif // SERV_DROP_EVENT_RENEWAL

	LUA_GET_VALUE( luaMgr,		"bWithPlayPcBang",	sData.m_bWithPlayPcBang, false );
#endif SERV_PC_BANG_DROP_EVENT
	//}}	

	//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
#ifdef EXTEND_SERVER_GROUP_MASK
	LUA_GET_VALUE( luaMgr,	"SERVER_GROUP",		sData.m_iServerGroup,	 -1 );
#else EXTEND_SERVER_GROUP_MASK
	LUA_GET_VALUE_ENUM( luaMgr,	"SERVER_GROUP",		sData.m_iServerGroup,		SEnum::SERVER_GROUP_ID, SEnum::SGI_INVALID );
#endif EXTEND_SERVER_GROUP_MASK
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
	//}}

	//{{ 2012. 03. 28	박세훈	드롭률 설정 이벤트 개편 Merge 작업 ( 2012.03.20 lygan_조성욱 )
#ifdef DROPEVENT_RENEWAL
	LUA_GET_VALUE( luaMgr, "iStartLevel", sData.m_iStartLevel,	0 ); // 레벨 범위 제한 시작 레벨
	LUA_GET_VALUE( luaMgr, "iEndLevel", sData.m_iEndLevel,	0 ); // 레벨 범위 제한 끝 레벨
#endif DROPEVENT_RENEWAL
	//}}
	//{{ 2012. 06. 20	김민성       이벤트 경험치 중첩 안됨 조건 추가
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
	LUA_GET_VALUE( luaMgr,		"bAccumulateEXP",	sData.m_bAccumulateEXP, false );
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
	//}}
	//{{ 2012. 06. 29	김민성       접속 시간에 따라 아이템 반복 지급
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
	LUA_GET_VALUE( luaMgr,		"bRepeatEvent",	sData.m_bRepeatEvent, false );
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
	//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
	LUA_GET_VALUE( luaMgr,		"bRepeatRewardEvent",	sData.m_bRepeatRewardEvent, false );
	LUA_GET_VALUE( luaMgr,		"iBeforeEventID",		sData.m_iBeforeEventID, -1 );
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

	//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	LUA_GET_VALUE( luaMgr,		"bPlayWithCharEXP",	sData.m_bPlayWithCharEXP,	false );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}
	//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
	LUA_GET_VALUE( luaMgr,		"iEventGroupID",	sData.m_iEventGroupID,	-1 );
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
	//}}

	//{{ 2011. 05. 31	최육사	접속 이벤트 레벨 체크
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
	std::pair< int, int > pairMinMaxLevel;
	LUA_GET_VALUE( luaMgr,		"iMinLevel",		pairMinMaxLevel.first,		0 );
	LUA_GET_VALUE( luaMgr,		"iMaxLevel",		pairMinMaxLevel.second,		0 );

	// 최소 레벨, 최대 레벨 정보가 하나라도 있으면 저장!
	if( pairMinMaxLevel.first != 0  ||  pairMinMaxLevel.second != 0 )
	{
		std::map< int, EVENT_DATA >::iterator mit;
		mit = m_mapEventData.find( iID );
		if( mit != m_mapEventData.end() )
		{
			mit->second.m_mapMinMaxReward.insert( std::make_pair( pairMinMaxLevel, sData.m_iEventReward ) );

			START_LOG( clog, L"이벤트 보상 그룹 설정!" )
				<< BUILD_LOG( iID )
				<< BUILD_LOG( mit->second.m_mapMinMaxReward.size() );
			return;
		}
		else
		{
			sData.m_mapMinMaxReward.insert( std::make_pair( pairMinMaxLevel, sData.m_iEventReward ) );
		}
	}
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
	//}}

	//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	LUA_GET_VALUE( luaMgr,		"fQuestItemDropRate",	sData.m_fQuestItemDropRate,	1.f );
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT 
	LUA_GET_VALUE( luaMgr, "iEventItemID", sData.m_iEventItemID, 0 );
#endif 

#ifdef SERV_CUSTOM_CONNECT_EVENT
	LUA_GET_VALUE( luaMgr, "iCustomEventID", sData.m_iCustomEventID, 0 );
#endif SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
	LUA_GET_VALUE( luaMgr, "bOnlySteamUser", sData.m_bOnlySteamUser, false );
	LUA_GET_VALUE( luaMgr, "bOnlyNotSteamUser", sData.m_bOnlyNotSteamUser, false );
#endif SERV_STEAM_USER_CONNECT_EVENT

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
	LUA_GET_VALUE( luaMgr,		"fDefenseDungeonOpenRate",	sData.m_fDefenseDungeonOpenRate,	1.0f );
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
	char cCriterionTemp;
	LUA_GET_VALUE( luaMgr,		"byteCriterionUserType",		cCriterionTemp,	0 );	sData.m_byteCriterionUserType = static_cast< byte >( cCriterionTemp );
	LUA_GET_VALUE( luaMgr,		"byteCriterionPresentUserType",	cCriterionTemp,	0 );	sData.m_byteCriterionPresentUserType = static_cast< byte >( cCriterionTemp );

	if( sData.m_byteCriterionUserType & SEnum::CUT_NEW )
	{
		std::wstring wstrCriterionDateBegin;
		std::wstring wstrCriterionDateEnd;

		LUA_GET_VALUE( luaMgr,		L"wstrCriterionDateBegin",	wstrCriterionDateBegin, L"" );
		LUA_GET_VALUE( luaMgr,		L"wstrCriterionDateEnd",	wstrCriterionDateEnd, L"" );

		if( ( KncUtil::ConvertStringToCTime( wstrCriterionDateBegin, sData.m_tCriterionDateBegin ) == false ) ||
			( KncUtil::ConvertStringToCTime( wstrCriterionDateEnd, sData.m_tCriterionDateEnd ) == false )
			)
		{
			START_LOG( cerr, L"기준 일자 이벤트: 기준 일자가 잘못되었습니다.")
				<< BUILD_LOG( iID )
				<< BUILD_LOG( wstrCriterionDateBegin )
				<< BUILD_LOG( wstrCriterionDateEnd )
				<< END_LOG;
			return;
		}
	}
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	LUA_GET_VALUE( luaMgr, "bNewUnitEvent", sData.m_bNewUnitEvent, false );
	LUA_GET_VALUE( luaMgr, "bNewUnitEvent2", sData.m_bNewUnitEvent2, false );
	LUA_GET_VALUE( luaMgr, "bCurrentUnitEvent", sData.m_bCurrentUnitEvent, false );
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
	LUA_GET_VALUE( luaMgr, "iUnitClassLevel", sData.m_iUnitClassLevel, -1 );
#endif SERV_UNIT_CLASS_LEVEL_EVENT

#ifdef SERV_HENIR_REWARD_EVENT// 작업날짜: 2013-09-09	// 박세훈
	LUA_GET_VALUE( luaMgr,		"bAccumulate",		sData.m_bAccumulate,	false );
#endif // SERV_HENIR_REWARD_EVENT

	m_mapEventData.insert( std::make_pair( iID, sData ) );
}

//{{ 2008. 7. 11  최육사  이벤트 공지
void KGameEventScriptManager::AddNotifyMSGServerInfo_LUA( int iServerGroupID, int iServerUID )
{
	// 해당 서버군인지 체크
	if( KBaseServer::GetKObj()->GetServerGroupID() == iServerGroupID )
	{
		m_iNotifyCnServerUID = iServerUID;
	}
}
//}}

void KGameEventScriptManager::Init()
{
	// 공지날릴 서버 설정
	m_bNotifyMSGEvent = ( KBaseServer::GetKObj()->GetUID() == m_iNotifyCnServerUID );
	if( m_bNotifyMSGEvent )
	{
		START_LOG( cout, L"공지 이벤트를 날릴 센터 서버로 등록되었음!" );
	}
}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
const EVENT_DATA* KGameEventScriptManager::GetEventData( IN int iScriptID ) const
#else //SERV_EVENT_DB_CONTROL_SYSTEM
const KGameEventScriptManager::EVENT_DATA* KGameEventScriptManager::GetEventData( IN int iScriptID ) const
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
{
	std::map< int, EVENT_DATA >::const_iterator mit = m_mapEventData.find( iScriptID );
	if( mit == m_mapEventData.end() )
	{
		START_LOG( cerr, L"존재 하지 않는 이벤트입니다." )
			<< BUILD_LOG( iScriptID )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}

#endif SERV_EVENT_SCRIPT_REFRESH
//}}

void KGameEventScriptManager::GetAdventNpcEventInfo( IN const std::vector< int >& vecEventID, OUT std::vector< KAdventNpcEventInfo >& vecInfo )
{
	vecInfo.clear();

	BOOST_TEST_FOREACH( const int, iEventID, vecEventID )
	{
		std::map< int, KAdventNpcEventInfo >::const_iterator mit;
		mit = m_mapEventMonster.find( iEventID );
		if( mit == m_mapEventMonster.end() )
			continue;

		vecInfo.push_back( mit->second );
	}
}

//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
void KGameEventScriptManager::AddConnectEventUserGroup_LUA( int iEventGroupID, UidType iUserUID )
{
	std::map< int, std::set<UidType> >::iterator it = m_mapEventUserGroup.find( iEventGroupID );
	if( it == m_mapEventUserGroup.end() )
	{
		std::set<UidType> setUserUID;
		it = m_mapEventUserGroup.insert( std::map< int, std::set<UidType> >::value_type( iEventGroupID, setUserUID ) ).first;
	}
	
	if( it->second.insert( iUserUID ).second == false )
	{
		START_LOG( cerr, L"이벤트 유저 그룹에 이미 중복되는 값이 존재합니다." )
			<< BUILD_LOG( iEventGroupID )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;
	}
}

bool KGameEventScriptManager::CheckEventGroup( IN const int iEventGroupID, IN const UidType iUserUID ) const
{
	if( iEventGroupID < 0 )
		return true;

	std::map< int, std::set<UidType> >::const_iterator it = m_mapEventUserGroup.find( iEventGroupID );
	if( it == m_mapEventUserGroup.end() )
	{
		START_LOG( cerr, L"존재하지 않는 이벤트 그룹입니다." )
			<< BUILD_LOG( iEventGroupID )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;
		return false;
	}

	return it->second.find( iUserUID ) != it->second.end();
}
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
//}}

#ifdef SERV_ADVERTISEMENT_EVENT
void KGameEventScriptManager::AddAdvertisement_LUA( IN int iID )
{
	if( m_mapAdvertisementData.find( iID ) != m_mapAdvertisementData.end() )
	{
		START_LOG( cerr, L"같은 아이디 광고 스크립트.!" )
			<< BUILD_LOG( iID )
			<< END_LOG;
		return;
	}

	KLuaManager luaMgr( GetLuaState() );
	EVENT_DATA sData;

	// 경험치,VP,ED 보너스 이벤트
	LUA_GET_VALUE( luaMgr,		L"URL",			sData.m_wstrAdvertisementURL,		L"" );

	m_mapAdvertisementData.insert( std::make_pair( iID, sData ) );
}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
const EVENT_DATA* KGameEventScriptManager::GetAdvertisementData( IN int iScriptID ) const
#else //SERV_EVENT_DB_CONTROL_SYSTEM
const KGameEventScriptManager::EVENT_DATA* KGameEventScriptManager::GetAdvertisementData( IN int iScriptID ) const
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
{
	std::map< int, EVENT_DATA >::const_iterator mit = m_mapAdvertisementData.find( iScriptID );
	if( mit == m_mapEventData.end() )
	{
		START_LOG( cerr, L"존재 하지 않는 이벤트입니다." )
			<< BUILD_LOG( iScriptID )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
///스크립트에서 정보 받을 함수
void KGameEventScriptManager::AddCoboEventData_LUA(const char* szChangeEventDate, const char* szWeekEndStartDate_One, const char* szWeekEndEndDate_One,const char* szWeekEndStartDate_Two, const char* szWeekEndEndDate_Two, const char* szWeekNextDay,int remaind)
{
	std::wstring ChangeEventDate = KncUtil::toWideString(szChangeEventDate);
	std::wstring WeekEndStartDate_One = KncUtil::toWideString(szWeekEndStartDate_One);
	std::wstring WeekEndEndDate_One = KncUtil::toWideString(szWeekEndEndDate_One);
	std::wstring WeekEndStartDate_Two = KncUtil::toWideString(szWeekEndStartDate_Two);
	std::wstring WeekEndEndDate_Two = KncUtil::toWideString(szWeekEndEndDate_Two);
	std::wstring NextDay =  KncUtil::toWideString(szWeekNextDay);

	//시간으로 변환하자
	CTime EventTime;
	if( KncUtil::ConvertStringToCTime( ChangeEventDate, EventTime ) == false)
	{
		START_LOG( cerr, L"ChangeEventDate CTime 형식으로 치환 실패" )
			<< BUILD_LOG( ChangeEventDate )
			<< END_LOG;
		CTime TempTime(2013,12,11,0,0,0);
		m_vecCoboEventTimeData.push_back(TempTime);
	}
	m_vecCoboEventTimeData.push_back(EventTime);
	if( KncUtil::ConvertStringToCTime( WeekEndStartDate_One, EventTime ) == false)
	{
		START_LOG( cerr, L"WeekEndStartDate_One CTime 형식으로 치환 실패" )
			<< BUILD_LOG( WeekEndStartDate_One )
			<< END_LOG;
		CTime TempTime(2013,12,7,0,0,0);
		m_vecCoboEventTimeData.push_back(TempTime);
	}
	m_vecCoboEventTimeData.push_back(EventTime);
	if( KncUtil::ConvertStringToCTime( WeekEndEndDate_One, EventTime ) == false)
	{
		START_LOG( cerr, L"WeekEndEndDate_One CTime 형식으로 치환 실패" )
			<< BUILD_LOG( WeekEndEndDate_One )
			<< END_LOG;
		CTime TempTime(2013,12,8,23,59,59);
		m_vecCoboEventTimeData.push_back(TempTime);
	}
	m_vecCoboEventTimeData.push_back(EventTime);
	if( KncUtil::ConvertStringToCTime( WeekEndStartDate_Two, EventTime ) == false)
	{
		START_LOG( cerr, L"WeekEndStartDate_Two CTime 형식으로 치환 실패" )
			<< BUILD_LOG( WeekEndStartDate_Two )
			<< END_LOG;
		CTime TempTime(2013,12,14,0,0,0);
		m_vecCoboEventTimeData.push_back(TempTime);
	}
	m_vecCoboEventTimeData.push_back(EventTime);
	if( KncUtil::ConvertStringToCTime( WeekEndEndDate_Two, EventTime ) == false)
	{
		START_LOG( cerr, L"WeekEndEndDate_Two CTime 형식으로 치환 실패" )
			<< BUILD_LOG( WeekEndEndDate_Two )
			<< END_LOG;
		CTime TempTime(2013,12,15,23,59,59);
		m_vecCoboEventTimeData.push_back(TempTime);
	}
	m_vecCoboEventTimeData.push_back(EventTime);
	if( KncUtil::ConvertStringToCTime( NextDay, EventTime ) == false)
	{
		START_LOG( cerr, L"NextDay CTime 형식으로 치환 실패" )
			<< BUILD_LOG( NextDay )
			<< END_LOG;
		CTime TempTime(2013,12,15,0,0,1);
		m_vecCoboEventTimeData.push_back(TempTime);
	}
	m_vecCoboEventTimeData.push_back(EventTime);

	///남은 시간 체크
	m_iRemaindTime = remaind;
}
std::vector<CTime> KGameEventScriptManager::GetCoboEventData(void)
{
	return m_vecCoboEventTimeData;
}
int KGameEventScriptManager::GetRemainTime(void)
{
	return m_iRemaindTime;
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
void KGameEventScriptManager::AddValenTineEventData_LUA( int iTemp_1, int iTemp_2, int iTemp_3, const char* szChangeEventDate, int iItemID,int iItemNum_1,int iItemNum_2,int iItemNum_3, int iItemGetCount )
{
	m_iTemp_1 = iTemp_1; //초급자 시간
	m_iTemp_2 = iTemp_2; //중급자 시간
	m_iTemp_3 = iTemp_3; //상급자 시간
	std::wstring ChangeEventDate = KncUtil::toWideString(szChangeEventDate);
	if( KncUtil::ConvertStringToCTime( ChangeEventDate, m_cTimeNextDay ) == false)
	{
		START_LOG( cerr, L"ChangeEventDate CTime 형식으로 치환 실패" )
			<< BUILD_LOG( ChangeEventDate )
			<< END_LOG;
			CTime TempTime(2014,01,01,0,0,1);
			m_cTimeNextDay = TempTime;
	}
	m_iItemID = iItemID;
	m_iItemNum_1 = iItemNum_1;
	m_iItemNum_2 = iItemNum_2;
	m_iItemNum_3 = iItemNum_3;
	m_iItemGetCount = iItemGetCount;
}
int	KGameEventScriptManager::GetBeginnerTime(void)
{
	return m_iTemp_1;
}
int	KGameEventScriptManager::GetIntermediateTime(void)
{
	return m_iTemp_2;
}
int	KGameEventScriptManager::GetExpertTime(void)
{
	return m_iTemp_3;
}
CTime KGameEventScriptManager::GetNextDayTime(void)
{
	return m_cTimeNextDay;
}
int	KGameEventScriptManager::GetValenItemID(void)
{
	return m_iItemID;
}
int KGameEventScriptManager::GetValenItemNum_1(void)
{
	return m_iItemNum_1;
}
int KGameEventScriptManager::GetValenItemNum_2(void)
{
	return m_iItemNum_2;
}
int KGameEventScriptManager::GetValenItemNum_3(void)
{
	return m_iItemNum_3;
}
int KGameEventScriptManager::GetValenTinePlayCount(void)
{
	return m_iItemGetCount;
}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM