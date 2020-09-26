#include ".\guildmanager.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"
#include "NetError.h"


#ifdef GUILD_TEST


ImplementSingleton( KGuildManager );

KGuildManager::KGuildManager(void)
{
	// lua
	lua_tinker::class_add<KGuildManager>( g_pLua, "KGuildManager" );
	lua_tinker::class_def<KGuildManager>( g_pLua, "dump",						&KGuildManager::Dump );

	lua_tinker::decl( g_pLua, "GuildManager", this );
}

KGuildManager::~KGuildManager(void)
{
}

ImplToStringW( KGuildManager )
{
	stm_ << L"------------------------------------------" << std::endl;

	stm_ << TOSTRINGW( m_vecGuild.size() );
	stm_ << TOSTRINGW( m_mapGuild.size() );
	stm_ << TOSTRINGW( m_vecDelReserved.size() );

	return stm_;
}

//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

void KGuildManager::Tick()
{
	std::vector< KGuildPtr >::iterator vit;
	for( vit = m_vecGuild.begin(); vit != m_vecGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		(*vit)->Tick();
	}

	//{{ 2009. 12. 2  최육사	길드구조개선
	if( !m_vecDelReserved.empty() )
	{
		std::vector< int >::const_iterator vitDR;
		for( vitDR = m_vecDelReserved.begin(); vitDR != m_vecDelReserved.end(); ++vitDR )
		{
			DeleteGuild( *vitDR );
		}

		m_vecDelReserved.clear();
	}
	//}}

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	if( 5.0 < m_kTimer.elapsed() )
	{
		m_kTimer.restart();

		CTime dTime = CTime::GetCurrentTime();
		
		for( std::list< ReservedGuildName >::iterator it = m_listReservedGuildName.begin(); it != m_listReservedGuildName.end(); )
		{
			if( 30 < ( dTime.GetTime() - it->m_dRecordTime.GetTime() ) )
			{
				m_listReservedGuildName.erase( it++ );
				continue;
			}
			++it;
		}
	}
#endif SERV_GUILD_CHANGE_NAME
	//}}
}

#endif GUILD_SKILL_TEST
//}}

KGuildPtr KGuildManager::GetGuild( IN int iGuildUID )
{
	std::map< int, KGuildPtr >::iterator mit = m_mapGuild.find( iGuildUID );
	if( mit == m_mapGuild.end() )
	{
		// 시간차로 발생할 수 있는 에러임.
		START_LOG( cwarn, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID );
		
		return KGuildPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"길드 포인터 이상.!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return KGuildPtr();
	}

	return mit->second;
}

void KGuildManager::InsertGuild( IN const KGuildInfo& kInfo )
{
	// 길드 객체 생성
	KGuildPtr spGuild( new KGuild( kInfo ) );

	// map컨테이너에 넣기
	m_mapGuild.insert( std::make_pair( kInfo.m_iGuildUID, spGuild ) );

	// vector컨테이너에 넣기
	std::vector< KGuildPtr >::iterator vit;
	for( vit = m_vecGuild.begin(); vit != m_vecGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->GetUID() == kInfo.m_iGuildUID )
		{
			START_LOG( cerr, L"guild vector에 중복된 guild가 있다!" )
				<< BUILD_LOG( kInfo.m_iGuildUID )
				<< END_LOG;

			// 스마트포인터를 덮어씌우자!
			(*vit) = spGuild;
			break;
		}
	}

	if( vit == m_vecGuild.end() )
	{
		m_vecGuild.push_back( spGuild );
	}
}

void KGuildManager::DeleteGuild( IN int iGuildUID )
{
	// map컨테이너에서 지우기
	m_mapGuild.erase( iGuildUID );

	// vector컨테이너에서 지우기
	std::vector< KGuildPtr >::iterator vit;
	for( vit = m_vecGuild.begin(); vit != m_vecGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->GetUID() == iGuildUID )
			break;
	}

	if( vit != m_vecGuild.end() )
	{
        m_vecGuild.erase( vit );
	}
}

void KGuildManager::DeleteReserve( IN int iGuildUID )
{
	_JIF( IsExistGuild( iGuildUID ), return );

    m_vecDelReserved.push_back( iGuildUID );
}

bool KGuildManager::GetGuildInfo( IN int iGuildUID, OUT KEGS_GUILD_INFO_NOT& kInfo )
{
	if( IsExistGuild( iGuildUID ) == false )
	{
		START_LOG( clog, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID );

		// 길드 정보를 못얻은것은 오류가 아니다.
		return false;
	}

	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"위에서 이미 검사 했는데? 일어날수 없는 에러!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	spGuild->GetGuildInfo( kInfo.m_kGuildInfo, kInfo.m_wstrGuildMasterName, kInfo.m_vecMemberUnitUID, kInfo.m_kGuildSkillInfo );
#else
	spGuild->GetGuildInfo( kInfo.m_kGuildInfo, kInfo.m_wstrGuildMasterName, kInfo.m_vecMemberUnitUID );
#endif GUILD_SKILL_TEST
	//}}
	return true;
}

bool KGuildManager::GetGuildSkillInfo( IN int iGuildUID, OUT KGuildSkillInfo& kInfo )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( clog, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID );
		
		return false;
	}

	spGuild->GetGuildSkillInfo( kInfo );
	return true;
}

bool KGuildManager::UpdateGuildInfo( IN const KGuildInfo& kGuildInfo )
{
	if( IsExistGuild( kGuildInfo.m_iGuildUID ) == true )
	{
		START_LOG( cerr, L"이미 길드정보가 생성되어 있습니다!" )
			<< BUILD_LOG( kGuildInfo.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	// 길드 정보 업데이트
	InsertGuild( kGuildInfo );

	return true;
}

bool KGuildManager::UpdateGuildMemberList( IN int iGuildUID, IN const std::vector< KGuildMemberInfo >& vecMemberInfo )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->UpdateGuildMemberList( vecMemberInfo );
	return true;
}

//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
bool KGuildManager::UpdateGuildSkill( IN int iGuildUID, IN const KGuildSkillInfo& kInfo )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->UpdateGuildSkill( kInfo );
	return true;
}
#endif GUILD_SKILL_TEST
//}}

bool KGuildManager::CheckJoinGuild( IN int iGuildUID, IN UidType iUnitUID, OUT UidType& iGuildMasterUnitUID )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}
	
	return spGuild->CheckJoinGuild( iUnitUID, iGuildMasterUnitUID );
}

bool KGuildManager::RemoveReserve( IN int iGuildUID, IN UidType iUnitUID )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}
	
	spGuild->RemoveReserve( iUnitUID );
	return true;
}

bool KGuildManager::JoinGuildMember( IN int iGuildUID, IN const KGuildMemberInfo& kMemberInfo, IN bool bIsLoginUser )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

    return spGuild->JoinGuildMember( kMemberInfo, bIsLoginUser );
}

bool KGuildManager::LoginGuildMember( IN int iGuildUID, IN const KGuildMemberInfo& kMemberInfo )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	return spGuild->LoginGuildMember( kMemberInfo );
}

bool KGuildManager::LogoutGuildMember( IN int iGuildUID, IN UidType iUnitUID )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	// 로그 아웃 처리
	const bool bRet = spGuild->LogoutGuildMember( iUnitUID );

	// 로그인된 유저가 한명도 없을경우 길드정보 삭제 예약을 한다.
	//{{ 2009. 12. 2  최육사	길드구조개선
#ifdef GUILD_SKILL_TEST
	if( spGuild->GetLoginedMemberCount() == 0 )
	{
        DeleteReserve( iGuildUID );
	}
#endif GUILD_SKILL_TEST
	//}}

	return bRet;
}

bool KGuildManager::CheckInviteCondition( IN int iGuildUID, IN UidType iReceiverUnitUID, IN UidType iSenderUnitUID, OUT KELG_INVITE_GUILD_FIND_USER_REQ& kPacket )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}
	
	return spGuild->CheckInviteCondition( iReceiverUnitUID, iSenderUnitUID, kPacket );
}

bool KGuildManager::GetGuildUserList( IN const KELG_GET_GUILD_USER_LIST_REQ& kReq, OUT KEGS_GET_GUILD_USER_LIST_ACK& kAck )
{
	KGuildPtr spGuild = GetGuild( kReq.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_GUILD_06;
		return false;
	}

	spGuild->GetGuildUserList( kReq, kAck );
    return true;    
}

bool KGuildManager::GuildChat( IN const KELG_CHAT_REQ& kInfo )
{
	KGuildPtr spGuild = GetGuild( kInfo.m_iChatData );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kInfo.m_iChatData )
			<< END_LOG;

		return false;
	}

	spGuild->GuildChat( kInfo );
	return true;
}

bool KGuildManager::CheckChangeGradeGuild( IN const KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ& kReq, OUT KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK& kAck )
{
	KGuildPtr spGuild = GetGuild( kReq.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}
	
	return spGuild->CheckChangeGradeGuild( kReq, kAck );
}

bool KGuildManager::UpdateChangeGuildMemberGrade( IN const KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK& kInfo )
{
	KGuildPtr spGuild = GetGuild( kInfo.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->UpdateChangeGuildMemberGrade( kInfo );
	return true;
}

bool KGuildManager::UpdateChangeGuildMessage( IN const KDBE_CHANGE_GUILD_MESSAGE_ACK& kInfo )
{
	KGuildPtr spGuild = GetGuild( kInfo.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->UpdateChangeGuildMessage( kInfo );
	return true;
}

bool KGuildManager::UpdateKickGuildMember( IN const KDBE_KICK_GUILD_MEMBER_ACK& kInfo, OUT std::wstring& wstrGuildName )
{
	KGuildPtr spGuild = GetGuild( kInfo.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		return false;
	}
	
	// 길드원 탈퇴 처리
	if( spGuild->KickGuildMemeber( kInfo.m_iUnitUID, kInfo.m_iTargetUnitUID ) == false )
		return false;

    wstrGuildName = spGuild->GetGuildName();
	return true;
}

bool KGuildManager::UpdateChangeGuildMemberMessage( IN const KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK& kInfo )
{
	KGuildPtr spGuild = GetGuild( kInfo.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	// 길드원 메시지 변경
	return spGuild->UpdateChangeGuildMemberMessage( kInfo );
}

bool KGuildManager::DisbandGuild( IN int iGuildUID )
{	
	if( IsExistGuild( iGuildUID ) == false )
	{
		START_LOG( cerr, L"길드를 해체하려고 하는데 길드 정보를 찾을수없다!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	// 길드 정보 삭제
	DeleteGuild( iGuildUID );
	return true;
}

bool KGuildManager::UpdateGuildMemberInfo( IN int iGuildUID, IN const KGuildMemberInfo& kInfo )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		// 시간차로 발생 가능하다.
		START_LOG( cwarn, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}
	
	return spGuild->UpdateGuildMemberInfo( kInfo );
}

bool KGuildManager::CheckGuildMaxMember( IN const KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ& kInfo )
{
	KGuildPtr spGuild = GetGuild( kInfo.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}
	
	return spGuild->CheckGuildMaxMember( kInfo );
}

bool KGuildManager::UpdateGuildMaxMember( IN int iGuildUID, IN u_short usMaxNumMember )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->UpdateGuildMaxMember( usMaxNumMember );
	return true;
}

//{{ 2009. 10. 27  최육사	길드레벨
bool KGuildManager::UpdateGuildEXP( IN int iGuildUID, IN const KDBE_UPDATE_GUILD_EXP_ACK& kInfo )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->UpdateGuildEXP( kInfo );
	return true;
}

bool KGuildManager::UpdateGuildLevelResult( IN int iGuildUID, IN const KDBE_GUILD_LEVEL_UP_ACK& kInfo )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->UpdateGuildLevelResult( kInfo );
	return true;
}
//}}

#endif GUILD_TEST

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

bool KGuildManager::CheckLearnGuildSkill( IN const KELG_GET_GUILD_SKILL_REQ& kReq, OUT KDBE_GET_GUILD_SKILL_REQ& kPacketToDB )
{
	KGuildPtr spGuild = GetGuild( kReq.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}

	return spGuild->CheckLearnGuildSkill( kReq, kPacketToDB );
}

bool KGuildManager::InsertGuildSkill( IN const KDBE_GET_GUILD_SKILL_ACK& kAck )
{
	KGuildPtr spGuild = GetGuild( kAck.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kAck.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}

	return spGuild->InsertGuildSkill( kAck );
}

bool KGuildManager::CheckResetGuildSkill( IN const KELG_RESET_GUILD_SKILL_REQ& kReq, OUT KDBE_RESET_GUILD_SKILL_REQ& kPacketToDB )
{
	KGuildPtr spGuild = GetGuild( kReq.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}

	return spGuild->CheckResetGuildSkill( kReq, kPacketToDB );
}

bool KGuildManager::ResetGuildSkill( IN const KDBE_RESET_GUILD_SKILL_ACK& kAck )
{
	KGuildPtr spGuild = GetGuild( kAck.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kAck.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}

	spGuild->ResetGuildSkill( kAck );
	return true;
}

bool KGuildManager::CheckInitGuildSkill( IN const KELG_INIT_GUILD_SKILL_REQ& kReq, OUT KDBE_INIT_GUILD_SKILL_REQ& kPacketToDB )
{
	KGuildPtr spGuild = GetGuild( kReq.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}

	return spGuild->CheckInitGuildSkill( kReq, kPacketToDB );    
}

bool KGuildManager::InitGuildSkill( IN const KDBE_INIT_GUILD_SKILL_ACK& kAck )
{
	KGuildPtr spGuild = GetGuild( kAck.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kAck.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}

	spGuild->InitGuildSkill( kAck );
	return true;
}

bool KGuildManager::InsertGuildCashSkillPoint( IN const KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT& kNot )
{
	KGuildPtr spGuild = GetGuild( kNot.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kNot.m_iGuildUID )
			<< END_LOG;

		return false;
	}
	
	return spGuild->InsertGuildCashSkillPoint( kNot );
}

bool KGuildManager::InsertGuildCashSkillPointResult( IN const KDBE_INSERT_GUILD_CASH_SKILL_POINT_ACK& kAck )
{
	KGuildPtr spGuild = GetGuild( kAck.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kAck.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->InsertGuildCashSkillPointResult( kAck );
	return true;
}

bool KGuildManager::ExpireGuildCashSkillPointResult( IN const KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK& kAck )
{
	KGuildPtr spGuild = GetGuild( kAck.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kAck.m_iGuildUID )
			<< END_LOG;
		
		return false;
	}

	spGuild->ExpireGuildCashSkillPointResult( kAck );
	return true;
}

bool KGuildManager::AdminInsertGuildSkillPoint( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ& kReq )
{
	KGuildPtr spGuild = GetGuild( kReq.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	return spGuild->AdminInsertGuildSkillPoint( kReq );
}

bool KGuildManager::AdminInsertGuildSkillPointResult( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK& kAck )
{
	KGuildPtr spGuild = GetGuild( kAck.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kAck.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->AdminInsertGuildSkillPointResult( kAck );
	return true;
}

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD

bool KGuildManager::CheckRegistrationGuildAd( IN int iGuildUID, IN UidType iUnitUID )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}

	return spGuild->CheckRegistrationGuildAd( iUnitUID );
}

bool KGuildManager::MakeGuildAdInfo( IN const KDBE_REGISTRATION_GUILD_AD_ACK& kAck, OUT KGuildAdInfo& kInfo )
{
	KGuildPtr spGuild = GetGuild( kAck.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kAck.m_iGuildUID )
			<< END_LOG;
		
		return false;
	}

	spGuild->MakeGuildAdInfo( kAck, kInfo );
	return true;
}

bool KGuildManager::ResultRegGuildAd( IN int iGuildUID, IN const std::wstring& wstrEndDate )
{
	KGuildPtr spGuild = GetGuild( iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuild->ResultRegGuildAd( wstrEndDate );
	return true;
}

bool KGuildManager::CheckGetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq )
{
	KGuildPtr spGuild = GetGuild( kReq.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}
    
	return spGuild->CheckGetApplyJoinGuildList( kReq );
}

bool KGuildManager::CheckDeleteApplyJoin( IN const KELG_DELETE_APPLY_JOIN_GUILD_REQ& kReq )
{
	KGuildPtr spGuild = GetGuild( kReq.m_iGuildUID );
	if( spGuild == NULL )
	{
		START_LOG( cerr, L"길드 정보 찾기 실패!" )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_06 );
		return false;
	}

	return spGuild->CheckDeleteApplyJoin( kReq );
}

#endif SERV_GUILD_AD
//}}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
bool KGuildManager::ReserveGuildName( IN const UidType& iUnitUID, IN const std::wstring& wstrGuildName )
{
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"UnitUID 값이 0 입니다.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( wstrGuildName )
			<< END_LOG;
		
		return false;
	}

	BOOST_TEST_FOREACH( ReservedGuildName, var, m_listReservedGuildName )
	{
		if( var.m_wstrGuildName == wstrGuildName )
		{
			return false;
		}
	}
	m_listReservedGuildName.push_back( ReservedGuildName( CTime::GetCurrentTime(), iUnitUID, wstrGuildName ) );

	return true;
}
bool KGuildManager::CancelGuildName( IN const UidType& iUnitUID )
{
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"UnitUID 값이 0 입니다.")
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	for( std::list< ReservedGuildName >::iterator it = m_listReservedGuildName.begin(); it != m_listReservedGuildName.end(); ++it )
	{
		if( it->m_iUnitUID == iUnitUID )
		{
			m_listReservedGuildName.erase( it );
			return true;
		}
	}

	return false;
}

bool KGuildManager::GetReservedGuildName( IN const UidType& iUnitUID, OUT std::wstring& wstrReservedGuildName )
{
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"UnitUID 값이 0 입니다.")
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	BOOST_TEST_FOREACH( ReservedGuildName, var, m_listReservedGuildName )
	{
		if( var.m_iUnitUID == iUnitUID )
		{
			wstrReservedGuildName = var.m_wstrGuildName;
			return true;
		}
	}
	return false;
}

bool KGuildManager::CheckReservedGuildName( IN const UidType& iUnitUID )
{
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"UnitUID 값이 0 입니다.")
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	BOOST_TEST_FOREACH( ReservedGuildName, var, m_listReservedGuildName )
	{
		if( var.m_iUnitUID == iUnitUID )
		{
			return true;
		}
	}
	return false;
}

void KGuildManager::TimeRefresh( IN const UidType& iUnitUID )
{
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"UnitUID 값이 0 입니다.")
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return;
	}

	BOOST_TEST_FOREACH( ReservedGuildName, var, m_listReservedGuildName )
	{
		if( var.m_iUnitUID == iUnitUID )
		{
			var.m_dRecordTime = CTime::GetCurrentTime();
			break;
		}
	}
}

#endif SERV_GUILD_CHANGE_NAME
//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
void KGuildManager::ResetLocalRankingInfo( void )
{
	BOOST_TEST_FOREACH( const KGuildPtr, spGuild, m_vecGuild )
	{
		spGuild->ResetLocalRankingInfo();
	}
}
#endif	// SERV_LOCAL_RANKING_SYSTEM