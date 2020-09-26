#include ".\xslroom.h"

CXSLRoom::CXSLRoom(void)
{
}

CXSLRoom::~CXSLRoom(void)
{
}

bool CXSLRoom::IsValidWinKillNum( char cPvpGameType, char cWinKillNum )
{
	switch( cPvpGameType )
	{
	case PGT_TEAM:
		{
			if( cWinKillNum != 0 )
			{
                return false;
			}
		}
		break;

	case PGT_TEAM_DEATH:
		{
			switch( cWinKillNum )
			{
			case 4:
			case 8:
			case 12:
			case 16:
			case 20:
			case 24:
			case 28:
				break;

			default:
				return false;
			}
		}
		break;

	case PGT_SURVIVAL:
		{
			switch( cWinKillNum )
			{
			case 3:
			case 6:
			case 9:
			case 12:
				break;

			default:
				return false;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"대전 타입이 이상합니다." )
				<< BUILD_LOGc( cPvpGameType )
				<< END_LOG;

			return false;
		}
	}

	return true;
}

bool CXSLRoom::IsValidPvpPlayTime( char cPvpGameType, int iPlayTime )
{
	switch( cPvpGameType )
	{
	case PGT_TEAM:
		{
			switch( iPlayTime )
			{
			case 50: // 운영자급
			case 200:
			case 300:
			case 400:
				break;

			default:
				return false;
			}
		}
		break;

	case PGT_TEAM_DEATH:
		{
			switch( iPlayTime )
			{
			case 50: // 운영자급
			case 200:
			case 500:
			case 800:
			case 1100:
				break;

			default:
				return false;
			}
		}
		break;

	case PGT_SURVIVAL:
		{
			switch( iPlayTime )
			{
			case 50:
			case 200:
			case 500:
			case 800:
			case 1100:
				break;

			default:
				return false;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"대전 타입이 이상합니다." )
				<< BUILD_LOGc( cPvpGameType )
				<< END_LOG;

			return false;
		}
	}

	return true;
}

//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
bool CXSLRoom::IsValidPvpGameType( const PVP_GAME_TYPE ePvpGameType )
{
	switch( ePvpGameType )
	{
	case PGT_TEAM:
	case PGT_TEAM_DEATH:
	case PGT_SURVIVAL:
		return true;
	}

	return false;
}
#endif SERV_PVP_NEW_SYSTEM
//}}

