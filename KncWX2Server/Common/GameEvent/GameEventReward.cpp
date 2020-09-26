#include "GameEventReward.h"

KGameEventReward::KGameEventReward()
{
	m_sData.m_fEXPRate = 0.0f;
	m_sData.m_fVPRate = 0.0f;
	m_sData.m_fEDRate = 0.0f;
	m_sData.m_iDungeonID = 0;
	m_sData.m_iUnitLevel = 0;
	m_sData.m_cUnitType = CXSLUnit::UT_NONE;
	m_sData.m_cUnitClass = CXSLUnit::UC_NONE;
}

KGameEventReward::~KGameEventReward()
{
}

//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
void KGameEventReward::SetEXPRate( IN const float fEXPRate )
{
	if( fEXPRate < 0.0f )
		m_sData.m_fEXPRate = 0.0f;
	else
		m_sData.m_fEXPRate = fEXPRate;
}

void KGameEventReward::SetVPRate( IN const float fVPRate )
{
	if( fVPRate < 0.0f )
		m_sData.m_fVPRate = 0.0f;
	else
		m_sData.m_fVPRate = fVPRate;
}

void KGameEventReward::SetEDRate( IN const float fEDRate )
{
	if( fEDRate < 0.0f )
		m_sData.m_fEDRate = 0.0f;
	else
		m_sData.m_fEDRate = fEDRate;
}

#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
void KGameEventReward::SetRankingPointRate( IN const float fRankingPointRate )
{
	if( fRankingPointRate < 0.0f )
		m_sData.m_fRankingPointRate = 0.0f;
	else
		m_sData.m_fRankingPointRate = fRankingPointRate;
}
#endif	// SERV_RANKING_POINT_EVENT

void KGameEventReward::SetDungeonID( IN const int iDungeonID )
{
	if( iDungeonID < 0 )
		m_sData.m_iDungeonID = 0;
	else
		m_sData.m_iDungeonID = iDungeonID;
}

void KGameEventReward::SetUnitLevel( IN const int iUnitLevel )
{
	if( iUnitLevel < 0 )
		m_sData.m_iUnitLevel = 0;
	else
		m_sData.m_iUnitLevel = iUnitLevel;
}

void KGameEventReward::SetUnitType( IN const char cUnitType )
{
	if( cUnitType < 0 )
		m_sData.m_cUnitType = 0;
	else
		m_sData.m_cUnitType = cUnitType;
}

void KGameEventReward::SetUnitClass( IN const char cUnitClass )
{
	if( cUnitClass < 0)
		m_sData.m_cUnitClass = 0;
	else
		m_sData.m_cUnitClass = cUnitClass;
}

void KGameEventReward::SetPartyNum( IN const int iPartyNum )
{
	if( iPartyNum < 0 )
		m_sData.m_iPartyNum = 0;
	else
		m_sData.m_iPartyNum = iPartyNum;
}

void KGameEventReward::SetServerGroupID( IN const int iServerGroupID )
{
	if( iServerGroupID < -1 )
		m_sData.m_iServerGroupID = -1;
	else
		m_sData.m_iServerGroupID = iServerGroupID;
}

void KGameEventReward::Dump()
{	
	START_LOG( cout2, L"보상 이벤트 확률셋팅.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetEventName() )
		<< BUILD_LOG( m_sData.m_fEXPRate )
		<< BUILD_LOG( m_sData.m_fVPRate )
		<< BUILD_LOG( m_sData.m_fEDRate )
		<< BUILD_LOG( m_sData.m_iDungeonID )
		<< BUILD_LOG( m_sData.m_iUnitLevel )
		<< BUILD_LOGc( m_sData.m_cUnitType )
		<< BUILD_LOGc( m_sData.m_cUnitClass )
		<< BUILD_LOGc( m_sData.m_iPartyNum )
		<< BUILD_LOG( m_sData.m_iServerGroupID )
		//{{ 2012. 06. 20	김민성       이벤트 경험치 중첩 안됨 조건 추가
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
		<< BUILD_LOG( m_sData.m_bAccumulateEXP )
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
		//}}
		;
}
#else
void KGameEventReward::SetData( float fEXPRate, float fVPRate, float fEDRate, int iDungeonID, int iUnitLevel, char cUnitType, char cUnitClass, int iPartyNum )
{
	if( fEXPRate < 0.0f )
		fEXPRate = 0.0f;

	if( fVPRate < 0.0f )
		fVPRate = 0.0f;

	if( fEDRate < 0.0f )
		fEDRate = 0.0f;

	if( iDungeonID < 0 )
		iDungeonID = 0;

	if( iUnitLevel < 0 )
		iUnitLevel = 0;

	if( cUnitType < 0 )
		cUnitType = 0;

	if( cUnitClass < 0)
		cUnitClass = 0;

	if( iPartyNum < 0 )
		iPartyNum = 0;

	m_sData.m_fEXPRate = fEXPRate;
	m_sData.m_fVPRate = fVPRate;
	m_sData.m_fEDRate = fEDRate;
	m_sData.m_iDungeonID = iDungeonID;
	m_sData.m_iUnitLevel = iUnitLevel;
	m_sData.m_cUnitType = cUnitType;
	m_sData.m_cUnitClass = cUnitClass;
	m_sData.m_iPartyNum = iPartyNum;

	START_LOG( cout2, L"보상 이벤트 확률셋팅.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetEventName() )
		<< BUILD_LOG( m_sData.m_fEXPRate )
		<< BUILD_LOG( m_sData.m_fVPRate )
		<< BUILD_LOG( m_sData.m_fEDRate )
		<< BUILD_LOG( m_sData.m_iDungeonID )
		<< BUILD_LOG( m_sData.m_iUnitLevel )
		<< BUILD_LOGc( m_sData.m_cUnitType )
		<< BUILD_LOGc( m_sData.m_cUnitClass )
		<< BUILD_LOGc( m_sData.m_iPartyNum )
		;
}
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
//}}

float KGameEventReward::GetEXPRate() const
{
	VARIFY_ENABLE( return 0.0f; );

	return m_sData.m_fEXPRate;
}

float KGameEventReward::GetVPRate() const
{
	VARIFY_ENABLE( return 0.0f; );

	return m_sData.m_fVPRate;
}

float KGameEventReward::GetEDRate() const
{
	VARIFY_ENABLE( return 0.0f; );

	return m_sData.m_fEDRate;
}

#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
float KGameEventReward::GetRankingPointRate( void ) const
{
	VARIFY_ENABLE( return 0.0f; );

	return m_sData.m_fRankingPointRate;
}
#endif	// SERV_RANKING_POINT_EVENT
