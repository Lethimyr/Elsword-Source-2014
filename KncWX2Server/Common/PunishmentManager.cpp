#include "PunishmentManager.h"

#ifdef SERV_NEW_PUNISHMENT

KPunishmentManager::KPunishmentManager()
{
}

KPunishmentManager::~KPunishmentManager()
{
}

bool KPunishmentManager::IsAppliedPunishment(int iType)
{
	std::map<int,KPunishmentInfo>::iterator iter = m_mapPunishment.find(iType);
	if(iter != m_mapPunishment.end())
	{
		CTime tEndTime(iter->second.m_iStartTime + iter->second.m_iDuration);

		if(CTime::GetCurrentTime() < tEndTime)
		{
			START_LOG( clog, L"Ã³¹ú Àû¿ëµÊ" )
				<< BUILD_LOG( iter->second.m_iType )
				<< BUILD_LOG( tEndTime.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() )
				<< END_LOG;

			return true;
		}
	}

	return false;
}

void KPunishmentManager::SetPunishment(const KPunishmentInfo& info)
{
	m_mapPunishment[info.m_iType] = info;
}

#endif SERV_NEW_PUNISHMENT