#include ".\xslguildmanager.h"
#include <dbg/dbg.hpp>
#include "NetError.h"
#include "Enum/Enum.h"


ImplementRefreshSingleton( CXSLGuildManager );

CXSLGuildManager::CXSLGuildManager(void) :
m_fPcBangFactor( 0.0f )
{
}

CXSLGuildManager::~CXSLGuildManager(void)
{
}

ImplToStringW( CXSLGuildManager )
{
	stm_	<< L"----------[ Guild Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapGuildEXPTable.size() )
			<< TOSTRINGW( m_vecCharFactor.size() )
			<< TOSTRINGW( m_mapMemberGradeFactor.size() )
			<< TOSTRINGW( m_fPcBangFactor )
			<< TOSTRINGW( m_mapPvpMemberFactor.size() )
			<< TOSTRINGWc( m_ucLimitLevel )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLGuildManager )
{
	lua_tinker::class_add<CXSLGuildManager>( GetLuaState(), "CXSLGuildManager" );	
	lua_tinker::class_def<CXSLGuildManager>( GetLuaState(), "SetLimitGuildLevel",			&CXSLGuildManager::SetLimitGuildLevel_LUA );
	lua_tinker::class_def<CXSLGuildManager>( GetLuaState(), "AddGuildEXP",					&CXSLGuildManager::AddGuildEXP_LUA );
	lua_tinker::class_def<CXSLGuildManager>( GetLuaState(), "SetCharLevelFactor",			&CXSLGuildManager::SetCharLevelFactor_LUA );
	lua_tinker::class_def<CXSLGuildManager>( GetLuaState(), "SetGuildMemberGradeFactor",	&CXSLGuildManager::SetGuildMemberGradeFactor_LUA );
	lua_tinker::class_def<CXSLGuildManager>( GetLuaState(), "SetPcBangFactor",				&CXSLGuildManager::SetPcBangFactor_LUA );
	//{{ 2009. 11. 16  최육사	대전길드포인트
	lua_tinker::class_def<CXSLGuildManager>( GetLuaState(), "SetPvpMemberFactor",			&CXSLGuildManager::SetPvpMemberFactor_LUA );
	//}}

	lua_tinker::decl( GetLuaState(), "g_pGuildManager", this );
}

void CXSLGuildManager::SetLimitGuildLevel_LUA( u_char ucGuildLevel )
{
	START_LOG( cout, L"길드 레벨 제한 : " << static_cast<int>(ucGuildLevel) );

	m_ucLimitLevel = ucGuildLevel;
}

bool CXSLGuildManager::AddGuildEXP_LUA( u_char ucGuildLevel, int iNeedEXP, int iTotalEXP )
{
	if( ucGuildLevel < 0  ||  iNeedEXP < 0  ||  iTotalEXP < 0 )
	{
		START_LOG( cerr, L"길드 경험치 테이블 정보가 이상합니다." )
			<< BUILD_LOGc( ucGuildLevel )
			<< BUILD_LOG( iNeedEXP )
			<< BUILD_LOG( iTotalEXP )
			<< END_LOG;

		return false;
	}

    KGuildEXP kInfo;
	kInfo.m_iNeedEXP = iNeedEXP;
	kInfo.m_iTotalEXP = iTotalEXP;
	m_mapGuildEXPTable.insert( std::make_pair( ucGuildLevel, kInfo ) );
    return true;
}

bool CXSLGuildManager::SetCharLevelFactor_LUA( u_char ucBeginLv, u_char ucEndLv, double fFactor )
{
	if( ucBeginLv < 1  ||  ucEndLv < ucBeginLv  ||  fFactor <= 0.0f )
	{
        START_LOG( cerr, L"명예 포인트 캐릭터 상수값 세팅이 이상합니다." )
			<< BUILD_LOGc( ucBeginLv )
			<< BUILD_LOGc( ucEndLv )
			<< BUILD_LOG( fFactor )
			<< END_LOG;

		return false;
	}

	KCharactorFactor kInfo;
	kInfo.m_ucBeginLv = ucBeginLv;
	kInfo.m_ucEndLv	  = ucEndLv;
	kInfo.m_fFactor	  = fFactor;
    m_vecCharFactor.push_back( kInfo );
	return true;
}

bool CXSLGuildManager::SetGuildMemberGradeFactor_LUA( u_char ucGuildMemberGrade, double fFactor )
{
	if( SEnum::GUG_INVALID >= ucGuildMemberGrade  ||  SEnum::GUG_NEW_USER < ucGuildMemberGrade  ||  fFactor <= 0.0f )
	{
		START_LOG( cerr, L"명예 포인트 길드원 등급 상수값 세팅이 이상합니다." )
			<< BUILD_LOGc( ucGuildMemberGrade )
			<< BUILD_LOG( fFactor )
			<< END_LOG;

		return false;
	}

	m_mapMemberGradeFactor.insert( std::make_pair( ucGuildMemberGrade, fFactor ) );
	return true;   
}

bool CXSLGuildManager::SetPcBangFactor_LUA( double fFactor )
{
	if( fFactor < 1.f )
	{
		START_LOG( cerr, L"길드 pc방 상수값 오류! pc방 상수의 최소값은 1.f입니다" )
			<< BUILD_LOG( fFactor )
			<< END_LOG;

		return false;
	}

	m_fPcBangFactor = fFactor;
	return true;
}

//{{ 2009. 11. 16  최육사	대전길드포인트
bool CXSLGuildManager::SetPvpMemberFactor_LUA( int iPvpMember, double fFactor )
{
	if( iPvpMember <= 0  ||  fFactor <= 0.0f )
	{
		START_LOG( cerr, L"대전 인원 상수값이 이상합니다." )
			<< BUILD_LOG( iPvpMember )
			<< BUILD_LOG( fFactor )
			<< END_LOG;

		return false;
	}

	m_mapPvpMemberFactor.insert( std::make_pair( iPvpMember, fFactor ) );
	return true;
}
//}}

double CXSLGuildManager::GetCharacterLevelFactor( IN u_char ucLevel ) const
{
	std::vector< KCharactorFactor >::const_iterator vit;
	for( vit = m_vecCharFactor.begin(); vit != m_vecCharFactor.end(); ++vit )
	{
        if( vit->m_ucBeginLv <= ucLevel  &&  vit->m_ucEndLv >= ucLevel )
		{
			// factor값 찾았다!
			return vit->m_fFactor;
		}
	}

	START_LOG( cerr, L"factor값 못찾았음" )
		<< BUILD_LOGc( ucLevel )
		<< END_LOG;

	return 0.0f;
}

double CXSLGuildManager::GetMemberGradeFactor( IN u_char ucGuildMemberGrade ) const
{
	std::map< u_char, double >::const_iterator mit = m_mapMemberGradeFactor.find( ucGuildMemberGrade );
	if( mit == m_mapMemberGradeFactor.end() )
	{
		START_LOG( cerr, L"길드원 등급 상수값이 존재하지 않습니다." )
			<< BUILD_LOGc( ucGuildMemberGrade )
			<< END_LOG;

		return 0.0f;
	}

	return mit->second;
}

double CXSLGuildManager::GetPvpMemberFactor( IN int iPvpMemberCount ) const
{
	std::map< int, double >::const_iterator mit = m_mapPvpMemberFactor.find( iPvpMemberCount );
	if( mit == m_mapPvpMemberFactor.end() )
	{
		START_LOG( cerr, L"대전 인원수 상수값이 존재하지 않습니다." )
			<< BUILD_LOG( iPvpMemberCount )
			<< END_LOG;
		
		return 0.0f;
	}

	return mit->second;
}

int	CXSLGuildManager::GetRequireTotalExpbyGuildLevel( IN u_char ucGuildLevel ) const
{
	{ // scope
		std::map< u_char, KGuildEXP >::const_iterator mit;
		mit = m_mapGuildEXPTable.find( ucGuildLevel );

		if( mit != m_mapGuildEXPTable.end() )
		{
			return mit->second.m_iTotalEXP;
		}
	}
	
	{ // scope
		std::map< u_char, KGuildEXP >::const_reverse_iterator mrit;
		mrit = m_mapGuildEXPTable.rbegin();

		if( mrit->first < ucGuildLevel )
			return mrit->second.m_iTotalEXP;
	}

	return 0;
}

int CXSLGuildManager::GetGuildLimitEXP() const
{
    return GetRequireTotalExpbyGuildLevel( m_ucLimitLevel );
}

u_char CXSLGuildManager::CheckGuildLevelUp( IN u_char ucGuildLevel, IN int iGuildExp ) const
{
	// 길드 레벨 최소값은 0입니다.
	if( ucGuildLevel < 1 )
		ucGuildLevel = 1;

	// 길드 경험치 최소값은 0입니다.
	if( iGuildExp < 0 )
		iGuildExp = 0;

	std::map< u_char, KGuildEXP >::const_iterator mit;
	mit = m_mapGuildEXPTable.find( ucGuildLevel );

	for( ; mit != m_mapGuildEXPTable.end(); ++mit )
	{
		if( iGuildExp < mit->second.m_iTotalEXP )
		{
			if( mit->first == ucGuildLevel )
				return ucGuildLevel;
			else
				return mit->first - 1;
		}
	}

	std::map< u_char, KGuildEXP >::const_reverse_iterator rmit;
	rmit = m_mapGuildEXPTable.rbegin();

	if( rmit->second.m_iTotalEXP < iGuildExp )
		return rmit->first;

	return ucGuildLevel;
}

int CXSLGuildManager::GetHonorPoint_Dungeon( IN u_char ucLevel, IN u_char ucGuildMemberGrade, IN int iDecreaseSpirit, IN bool bIsPcBang )
{
	const double fLevelFactor = GetCharacterLevelFactor( ucLevel );
	const double fMemberGradeFactor = GetMemberGradeFactor( ucGuildMemberGrade );
	const double fPcBangFactor = ( bIsPcBang ? m_fPcBangFactor : 1.f );

	return static_cast<int>( static_cast<double>(iDecreaseSpirit) * fLevelFactor * fMemberGradeFactor * fPcBangFactor );
}

int CXSLGuildManager::GetHonorPoint_Pvp( IN u_char ucLevel, IN u_char ucGuildMemberGrade, IN int iPvpMemberCount, IN bool bIsPcBang )
{
    const double fLevelFactor = GetCharacterLevelFactor( ucLevel );
	const double fMemberGradeFactor = GetMemberGradeFactor( ucGuildMemberGrade );
	const double fPcBangFactor = ( bIsPcBang ? m_fPcBangFactor : 1.f );
	const double fPvpMemberFactor = GetPvpMemberFactor( iPvpMemberCount );

	return static_cast<int>( 6.f * fPvpMemberFactor * fLevelFactor * fMemberGradeFactor * fPcBangFactor );
}


