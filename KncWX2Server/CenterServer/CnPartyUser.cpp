#include "CnPartyUserManager.h"



KCnPartyUser::KCnPartyUser( const KPartyUserInfo& kInfo ) :
//{{ 2012. 03. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
m_bIsOnGoToPartyButton( false ),
m_bCheckedForAutoParty( false ),
m_bCheckedPlayCondition( false ),
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
m_bReady( false ),
m_bHost( false )
{
	SetPartyUserInfo( kInfo );
}

KCnPartyUser::~KCnPartyUser()
{
}

void KCnPartyUser::SetPartyRoomUserInfo( IN const KPartyRoomUserInfo& kInfo )
{
    m_vecPartyRoomUserInfo.clear();
	m_vecPartyRoomUserInfo.push_back( kInfo );
}

bool KCnPartyUser::GetPartyRoomUserInfo( OUT KPartyRoomUserInfo& kInfo )
{
	for( u_int ui = 0; ui < m_vecPartyRoomUserInfo.size(); ++ui )
	{
		kInfo = m_vecPartyRoomUserInfo[ui];
		return true;
	}

	return false;
}

//{{ 2012. 03. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KCnPartyUser::GetOldPartyInfo( OUT KOldPartyInfo& kInfo ) const
{
	BOOST_TEST_FOREACH( const KOldPartyInfo&, kOldPartyInfo, m_vecOldPartyInfo )
	{
		kInfo = kOldPartyInfo;
		return true;
	}

	return false;
}

void KCnPartyUser::SetOldPartyInfo( IN const KOldPartyInfo& kInfo )
{
	m_vecOldPartyInfo.clear();
	m_vecOldPartyInfo.push_back( kInfo );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnPartyUser::SendToGSCharacter( unsigned short usEventID )
{
    SendToGSCharacter( usEventID, char() );
}

//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
void KCnPartyUser::GetMatchUserInfo( IN const UidType iPartyUID, OUT KMatchUserInfo& kInfo ) const
{
	kInfo.m_iGSUID			= GetGSUID();
	kInfo.m_iUnitUID		= GetCharUID();
	kInfo.m_iPartyUID		= iPartyUID;
	kInfo.m_wstrNickName	= GetCharName();
	kInfo.m_cUnitClass		= GetUnitClass();
	kInfo.m_ucLevel			= GetLevel();
	//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	kInfo.m_cCurrentRank		= m_kPartyUserInfo.m_cRank;
	kInfo.m_iOfficialMatchCnt	= m_kPartyUserInfo.m_iOfficialMatchCnt;
#endif SERV_2012_PVP_SEASON2
	//}}
	kInfo.m_iRating			= m_kPartyUserInfo.m_iRating;
	kInfo.m_iRPoint			= m_kPartyUserInfo.m_iRPoint;
}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnPartyUser::GetAutoPartyUserInfo( OUT KAutoPartyUserInfo& kInfo ) const
{
	kInfo.m_iGSUID			= GetGSUID();
	kInfo.m_iUnitUID		= GetCharUID();
	kInfo.m_wstrNickName	= GetCharName();
	kInfo.m_cUnitClass		= GetUnitClass();
	kInfo.m_ucLevel			= GetLevel();
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


