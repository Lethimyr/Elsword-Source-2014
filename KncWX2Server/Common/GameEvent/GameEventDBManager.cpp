#include ".\GameEventDBManager.h"
#include "BaseServer.h"
#include "X2Data/XSLDungeon.h"
//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	#include "Enum/Enum.h"
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
//}}


#ifdef SERV_EVENT_DB_CONTROL_SYSTEM

ImplementSingleton( KGameEventDBManager );

KGameEventDBManager::KGameEventDBManager(void)
{

}

KGameEventDBManager::~KGameEventDBManager(void)
{
}


const EVENT_DATA* KGameEventDBManager::GetEventDBData( IN int iScriptID ) const
{
	std::map< int, EVENT_DATA >::const_iterator mit = m_mapEventDBData.find( iScriptID );
	if( mit == m_mapEventDBData.end() )
	{
		START_LOG( cerr, L"존재 하지 않는 이벤트입니다." )
			<< BUILD_LOG( iScriptID )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}

bool KGameEventDBManager::SetEventDBData(std::map<int, std::string> mapEventDBData)
{

	std::map< int, std::string >::iterator mit;
	std::map<std::string, std::string> mapStringEvent;
	std::vector<std::string> vecStringkomma;
	
	vecStringkomma.clear();

	m_mapEventDBData.clear();


	for( mit = mapEventDBData.begin(); mit != mapEventDBData.end(); ++mit )
	{
		std::map<std::string, std::string> mapStringEvent;
		std::vector<std::string> vecStringkomma;

		mapStringEvent.clear();
		vecStringkomma.clear();

		vecStringkomma = split(mit->second, ",", false );

		for (int i = 0; i < vecStringkomma.size() ; ++i)
		{
			std::vector<std::string> vecStringData;

			vecStringData = split(vecStringkomma[i], "=", false);


			if (vecStringData[0].length() == 0)
				continue;

			std::map<std::string, std::string>::iterator mitStringData = mapStringEvent.find(vecStringData[0]);

			if ( mitStringData == mapStringEvent.end() )
			{
				mapStringEvent.insert(std::make_pair( vecStringData[0], vecStringData[1] ));
			}
			else
			{
				START_LOG( cerr, L"변수 이름이 중복으로 들어 왔습니다." )
					<< BUILD_LOG( vecStringData[0] )
					<< BUILD_LOG( vecStringData[1] )
					<< END_LOG;
			}
		}
		
		
		EVENT_DATA sData;
		std::map<std::string, std::string>::iterator mitStringDataTemp;

	
		mitStringDataTemp = mapStringEvent.find("fEXPRate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_fEXPRate = atof(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_fEXPRate = 0.f;
		}

		mitStringDataTemp = mapStringEvent.find("fVPRate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_fVPRate = atof(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_fVPRate = 0.f;
		}

		mitStringDataTemp = mapStringEvent.find("fEDRate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_fEDRate = atof(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_fEDRate = 0.f;
		}

		mitStringDataTemp = mapStringEvent.find("fRankingPointRate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_fRankingPointRate = atof(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_fRankingPointRate = 0.f;
		}

		mitStringDataTemp = mapStringEvent.find("iDungeonID");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iDungeonID = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iDungeonID = 0;
		}

		mitStringDataTemp = mapStringEvent.find("iDungeonMode");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iDungeonMode = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iDungeonMode = CXSLDungeon::DM_INVALID;
		}

		mitStringDataTemp = mapStringEvent.find("iUnitLevel");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iUnitLevel = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iUnitLevel = 0;
		}

		mitStringDataTemp = mapStringEvent.find("cUnitType");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			int iUnitType = atoi(mitStringDataTemp->second.c_str());
			sData.m_cUnitType = static_cast<char>(iUnitType);
		}
		else
		{
			sData.m_cUnitType = 0;
		}

		mitStringDataTemp = mapStringEvent.find("cUnitClass");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			int iUnitClass = atoi(mitStringDataTemp->second.c_str());
			sData.m_cUnitClass = static_cast<char>(iUnitClass);
		}
		else
		{
			sData.m_cUnitClass = 0;
		}

		mitStringDataTemp = mapStringEvent.find("iPartyNum");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iPartyNum = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iPartyNum = 0;
		}

		mitStringDataTemp = mapStringEvent.find("fEventTime");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_fEventTime = atof(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_fEventTime = 0.f;
		}

		mitStringDataTemp = mapStringEvent.find("iRewardID");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iEventReward = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iEventReward = 0;
		}

		mitStringDataTemp = mapStringEvent.find("bGiveOnce");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);

			sData.m_bGiveOnce = true;

			if (strBoolCheck.compare("FALSE") == 0)
			{
				sData.m_bGiveOnce = false;	
			}
			
			
		}
		else
		{
			sData.m_bGiveOnce = true;
		}

		mitStringDataTemp = mapStringEvent.find("bDelEndEvent");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			
			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bDelEndEvent = true;	
			}
			else
			{
				sData.m_bDelEndEvent = false;
			}

		}
		else
		{
			sData.m_bDelEndEvent = false;
		}

		mitStringDataTemp = mapStringEvent.find("bPcBangEvent");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bPcBangEvent = true;	
			}
			else
			{
				sData.m_bPcBangEvent = false;
			}

		}
		else
		{
			sData.m_bPcBangEvent = false;
		}

		mitStringDataTemp = mapStringEvent.find("bDirectReward");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bDirectReward = true;	
			}
			else
			{
				sData.m_bDirectReward = false;
			}

		}
		else
		{
			sData.m_bDirectReward = false;
		}

		mitStringDataTemp = mapStringEvent.find("bAccountEvent");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bAccountEvent = true;	
			}
			else
			{
				sData.m_bAccountEvent = false;
			}

		}
		else
		{
			sData.m_bAccountEvent = false;
		}


		mitStringDataTemp = mapStringEvent.find("iChannelCode");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iChannelCode = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iChannelCode = -1;
		}

		mitStringDataTemp = mapStringEvent.find("iBubbleFighterTeam");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iBF_Team = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iBF_Team = 0;
		}

		mitStringDataTemp = mapStringEvent.find("iHenirRewardEventCount");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iHenirRewardEventCount = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iHenirRewardEventCount = 0;
		}
		

		mitStringDataTemp = mapStringEvent.find("bHenirRewardUnLimited");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bHenirRewardUnLimited = true;	
			}
			else
			{
				sData.m_bHenirRewardUnLimited = false;
			}

		}
		else
		{
			sData.m_bHenirRewardUnLimited = false;
		}

#ifdef SERV_DROP_EVENT_RENEWAL
		mitStringDataTemp = mapStringEvent.find("fDropRate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_fDropRate = atof(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_fDropRate = 0.0f;
		}
#else //SERV_DROP_EVENT_RENEWAL
		mitStringDataTemp = mapStringEvent.find("iDropCount");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iDropCount = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iDropCount = 1;
		}
#endif //SERV_DROP_EVENT_RENEWAL
		

		mitStringDataTemp = mapStringEvent.find("bWithPlayPcBang");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bWithPlayPcBang = true;	
			}
			else
			{
				sData.m_bWithPlayPcBang = false;
			}

		}
		else
		{
			sData.m_bWithPlayPcBang = false;
		}


		mitStringDataTemp = mapStringEvent.find("SERVER_GROUP");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iServerGroup = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iServerGroup = SEnum::SGI_INVALID;
		}
		
#ifdef DROPEVENT_RENEWAL
		mitStringDataTemp = mapStringEvent.find("iStartLevel");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iStartLevel = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iStartLevel = 0;
		}

		mitStringDataTemp = mapStringEvent.find("iEndLevel");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iEndLevel = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iEndLevel = 0;
		}
#endif DROPEVENT_RENEWAL

		mitStringDataTemp = mapStringEvent.find("bAccumulateEXP");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bAccumulateEXP = true;	
			}
			else
			{
				sData.m_bAccumulateEXP = false;
			}

		}
		else
		{
			sData.m_bAccumulateEXP = false;
		}

		mitStringDataTemp = mapStringEvent.find("bRepeatEvent");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bRepeatEvent = true;	
			}
			else
			{
				sData.m_bRepeatEvent = false;
			}

		}
		else
		{
			sData.m_bRepeatEvent = false;
		}

		mitStringDataTemp = mapStringEvent.find("bRepeatRewardEvent");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bRepeatRewardEvent = true;	
			}
			else
			{
				sData.m_bRepeatRewardEvent = false;
			}

		}
		else
		{
			sData.m_bRepeatRewardEvent = false;
		}

		mitStringDataTemp = mapStringEvent.find("iBeforeEventID");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iBeforeEventID = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iBeforeEventID = -1;
		}
#ifdef SERV_THE_PREVIOUS_FIELD_EVENT
		mitStringDataTemp = mapStringEvent.find("bComeBackUserEvent");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bComeBackUserEvent = true;	
			}
			else
			{
				sData.m_bComeBackUserEvent = false;
			}

		}
		else
		{
			sData.m_bComeBackUserEvent = false;
		}

#endif SERV_THE_PREVIOUS_FIELD_EVENT

#ifdef SERV_FIXED_DATE_EVENT
		mitStringDataTemp = mapStringEvent.find("wstrFixedDate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::wstring wstrTempString = KncUtil::toWideString(mitStringDataTemp->second);

			if (wstrTempString.empty() == false)
			{
				CTime tFixedDate;
				sData.m_wstrFixedDate = wstrTempString;
				if( KncUtil::ConvertStringToCTime( sData.m_wstrFixedDate, tFixedDate ) == false )
				{
					START_LOG( cerr, L"이벤트 기준 일자 설정이 잘못되었습니다.")
						<< BUILD_LOG( mit->first )
						<< BUILD_LOG( sData.m_wstrFixedDate )
						<< END_LOG;
					
					sData.m_wstrFixedDate = L"";
				}
			}
			else
			{
				sData.m_wstrFixedDate = L"";
			}

		}
		else
		{
			sData.m_wstrFixedDate = L"";
		}

		mitStringDataTemp = mapStringEvent.find("iEventUserType");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iEventUserType = atoi(mitStringDataTemp->second.c_str());

			if (( sData.m_iEventUserType <= EUT_NONE ) || ( EUT_MAX <= sData.m_iEventUserType ) )
			{
				START_LOG( cerr, L"이벤트 유저 타입 설정이 잘못되었습니다.")
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( sData.m_iEventUserType )
					<< END_LOG;
				
				sData.m_iEventUserType = EUT_NONE;

			}
		}
		else
		{
			sData.m_iEventUserType = EUT_NONE;
		}
#endif SERV_FIXED_DATE_EVENT	

#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP

		mitStringDataTemp = mapStringEvent.find("bPlayWithCharEXP");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);

			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bPlayWithCharEXP = true;	
			}
			else
			{
				sData.m_bPlayWithCharEXP = false;
			}

		}
		else
		{
			sData.m_bPlayWithCharEXP = false;
		}

#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP

		mitStringDataTemp = mapStringEvent.find("iEventGroupID");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iEventGroupID = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iEventGroupID = -1;
		}


		std::pair< int, int > pairMinMaxLevel;

		mitStringDataTemp = mapStringEvent.find("iMinLevel");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			pairMinMaxLevel.first = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			pairMinMaxLevel.first = 0;
		}

		mitStringDataTemp = mapStringEvent.find("iMaxLevel");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			pairMinMaxLevel.second = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			pairMinMaxLevel.second = 0;
		}

		// 최소 레벨, 최대 레벨 정보가 하나라도 있으면 저장!
		if( pairMinMaxLevel.first != 0  ||  pairMinMaxLevel.second != 0 )
		{
			sData.m_mapMinMaxReward.insert(std::make_pair(pairMinMaxLevel, sData.m_iEventReward));
		}

		mitStringDataTemp = mapStringEvent.find("fQuestItemDropRate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_fQuestItemDropRate = atof(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_fQuestItemDropRate = 1.f;
		}

		mitStringDataTemp = mapStringEvent.find("iEventItemID");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iEventItemID = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iEventItemID = 0;
		}

		mitStringDataTemp = mapStringEvent.find("iCustomEventID");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iCustomEventID = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iCustomEventID = 0;
		}

#ifdef SERV_STEAM_USER_CONNECT_EVENT
		mitStringDataTemp = mapStringEvent.find("bOnlySteamUser");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bOnlySteamUser = true;	
			}
			else
			{
				sData.m_bOnlySteamUser = false;
			}

		}
		else
		{
			sData.m_bOnlySteamUser = false;
		}

		mitStringDataTemp = mapStringEvent.find("bOnlyNotSteamUser");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bOnlyNotSteamUser = true;	
			}
			else
			{
				sData.m_bOnlyNotSteamUser = false;
			}

		}
		else
		{
			sData.m_bOnlyNotSteamUser = false;
		}
#endif SERV_STEAM_USER_CONNECT_EVENT

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11

		mitStringDataTemp = mapStringEvent.find("fDefenseDungeonOpenRate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_fDefenseDungeonOpenRate = atof(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_fDefenseDungeonOpenRate = 1.0f;
		}
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
		//2013.11.13 darkstarbt_조성욱 // '2013-08-07 07:00:00' 와 같은 날짜 처리가 현재 되지 않기 때문에 이 이벤트의 경우 기존과 같이 EventData.lua 에서 사용해야 합니다.
		위 글을 읽으셨으면 SERV_CRITERION_DATE_EVENT 이 디파인 사용하는 국가는 이 부분 주석 처리 하시면 됩니다.
#endif	// SERV_CRITERION_DATE_EVENT


#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

		mitStringDataTemp = mapStringEvent.find("bNewUnitEvent");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bNewUnitEvent = true;	
			}
			else
			{
				sData.m_bNewUnitEvent = false;
			}

		}
		else
		{
			sData.m_bNewUnitEvent = false;
		}

		mitStringDataTemp = mapStringEvent.find("bNewUnitEvent2");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bNewUnitEvent2 = true;	
			}
			else
			{
				sData.m_bNewUnitEvent2 = false;
			}

		}
		else
		{
			sData.m_bNewUnitEvent2 = false;
		}

		mitStringDataTemp = mapStringEvent.find("bCurrentUnitEvent");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bCurrentUnitEvent = true;	
			}
			else
			{
				sData.m_bCurrentUnitEvent = false;
			}

		}
		else
		{
			sData.m_bCurrentUnitEvent = false;
		}
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR


#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		mitStringDataTemp = mapStringEvent.find("iUnitClassLevel");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{
			sData.m_iUnitClassLevel = atoi(mitStringDataTemp->second.c_str());
		}
		else
		{
			sData.m_iUnitClassLevel = 0;
		}
#endif //SERV_UNIT_CLASS_LEVEL_EVENT

#ifdef SERV_HENIR_REWARD_EVENT
		mitStringDataTemp = mapStringEvent.find("bAccumulate");
		if ( mitStringDataTemp != mapStringEvent.end() )
		{

			std::string strBoolCheck = MakeUpperCase(mitStringDataTemp->second);


			if (strBoolCheck.compare("TRUE") == 0)
			{
				sData.m_bAccumulate = true;	
			}
			else
			{
				sData.m_bAccumulate = false;
			}

		}
		else
		{
			sData.m_bAccumulate = false;
		}
#endif //SERV_HENIR_REWARD_EVENT


		std::map< int, EVENT_DATA >::iterator mitEventDBData = m_mapEventDBData.find(mit->first);

		if ( mitEventDBData == m_mapEventDBData.end() )
		{

			m_mapEventDBData.insert(std::make_pair(mit->first, sData));
		}
		else
		{
			if( pairMinMaxLevel.first != 0  ||  pairMinMaxLevel.second != 0 )
			{
				mitEventDBData->second.m_mapMinMaxReward.insert(std::make_pair(pairMinMaxLevel, sData.m_iEventReward));
				START_LOG( clog, L"이벤트 보상 그룹 설정!" )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mitEventDBData->second.m_mapMinMaxReward.size() );
			}

		}
		
	}

	return true;
}

// 2013.10.23 lygan_조성욱 // 구분인자 이용해서 문자열 끊는 함수 C#의 split 기능과 동일하게 제작
std::vector<std::string> KGameEventDBManager::split ( const std::string& s, const std::string& delim, const bool keep_empty /*= true*/)
{

	std::vector<std::string> result;
	if ( delim.empty() == true )
	{
		result.push_back(s);
		return result;
	}
	

	std::string stringTrim = s;
	stringTrim.erase(remove(stringTrim.begin(), stringTrim.end(), ' '), stringTrim.end());

	stringTrim.erase(remove(stringTrim.begin(), stringTrim.end(), '	'), stringTrim.end());

	const std::string cstringTrim = stringTrim;

	std::string::const_iterator substart = cstringTrim.begin(), subend;

	while (true)
	{
		subend = search(substart, cstringTrim.end(), delim.begin(), delim.end());

		std::string temp(substart, subend);

		if ( keep_empty || !temp.empty())
		{
			result.push_back(temp);
		}

		if ( subend == cstringTrim.end() )
		{
			break;
		}

		substart = subend + delim.size();

	}

	return result;
	


}

int KGameEventDBManager::str2AsciiInt(const std::string string2Ascii)
{
	int iAscii = 0;

	for(int k = 0; k < (int)string2Ascii.size(); k++ )
	{
		iAscii += string2Ascii[k];

	}

	return iAscii;

}
std::string KGameEventDBManager::MakeUpperCase(std::string str)
{
	for( std::string::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (char)toupper(*i);


	return str;
}

#endif //SERV_EVENT_DB_CONTROL_SYSTEM