#include "StdAfx.h"
#include ".\x2questmanager.h"

//{{ 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력
#ifdef PRINT_INGAMEINFO_TO_EXCEL
#include "BasicExcel.hpp"
using namespace YExcel;
#endif PRINT_INGAMEINFO_TO_EXCEL
//}} 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력

CX2QuestManager::CX2QuestManager(void) :
m_bUpdateQuest(false)
#ifdef QUEST_GUIDE
, m_bHasDungeonQuest(false)
#endif //QUEST_GUIDE
{
}

CX2QuestManager::~CX2QuestManager(void)
{
	QuestTemplet* pQuestTemplet = NULL;
	map<int,QuestTemplet*>::iterator iter;

	iter = m_mapQuestTemplet.begin();
	for( iter = m_mapQuestTemplet.begin(); iter != m_mapQuestTemplet.end(); iter++ )
	{
		pQuestTemplet = iter->second;
		SAFE_DELETE( pQuestTemplet );
	}
	m_mapQuestTemplet.clear();

	SubQuestTemplet* pSubQuestTemplet = NULL;
	map<int,SubQuestTemplet*>::iterator iter2;

	iter2 = m_mapSubQuestTemplet.begin();
	for( iter2 = m_mapSubQuestTemplet.begin(); iter2 != m_mapSubQuestTemplet.end(); iter2++ )
	{
		pSubQuestTemplet = iter2->second;
		SAFE_DELETE( pSubQuestTemplet );
	}
	m_mapSubQuestTemplet.clear();

	ClearUnitQuest();
}

bool CX2QuestManager::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pQuestManager", this );
	return g_pKTDXApp->LoadLuaTinker( pFileName );
}


bool CX2QuestManager::AddQuestTemplet_LUA()
{


	QuestTemplet* pQuestTemplet = new QuestTemplet();
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

#ifdef ADD_SERVER_GROUP
	LUA_GET_VALUE(	luaManager, "m_iServerGroupID",				pQuestTemplet->m_iServerGroupID,	    -1 );
#ifdef EXTEND_SERVER_GROUP_MASK
#else EXTEND_SERVER_GROUP_MASK
	if( pQuestTemplet->m_iServerGroupID >= SGI_END )
		goto LoadFail;
#endif EXTEND_SERVER_GROUP_MASK
#endif

	LUA_GET_VALUE_RETURN(	luaManager, "m_iID",				pQuestTemplet->m_iID,			0,			goto LoadFail; );



	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eQuestType",		pQuestTemplet->m_eQuestType,	QUEST_TYPE,	QT_NORMAL,	goto LoadFail; );

#ifdef EXCEPTION_EPIC_QUEST_TEMPLET
	if(pQuestTemplet->m_eQuestType >= 5)
	{
		return true;
	}
#endif EXCEPTION_EPIC_QUEST_TEMPLET

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eStartNPCID",	pQuestTemplet->m_eStartNPCID,	CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NUI_NONE,	goto LoadFail; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eEndNPCID",		pQuestTemplet->m_eEndNPCID,		CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NUI_NONE,	goto LoadFail; );

	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrTitle",			pQuestTemplet->m_wstrTitle,		L"",		goto LoadFail; );	
	//LUA_GET_VALUE_RETURN(	luaManager, "m_iTitle_Index",			m_nString_Index,		0,		goto LoadFail; );
	//pQuestTemplet->m_wstrTitle = GET_SCRIPT_STRING(m_nString_Index);
	
	LUA_GET_VALUE_RETURN(	luaManager, "m_iFairLevel",			pQuestTemplet->m_iFairLevel,	0,			goto LoadFail; );

//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eRepeatType",		pQuestTemplet->m_eRepeatType,	QUEST_REPEAT_TYPE,	QRT_NORMAL,	goto LoadFail; );
#else	SERV_DAILY_QUEST
	LUA_GET_VALUE_RETURN(	luaManager, "m_bRepeat",			pQuestTemplet->m_bRepeat,		false,		goto LoadFail; );
#endif	SERV_DAILY_QUEST
//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
	
	//{{ 2010. 01. 29  최육사	일일퀘스트
	LUA_GET_VALUE_RETURN(	luaManager, "m_bIsPcBang",			pQuestTemplet->m_bIsPcBang,		false,		goto LoadFail; );
	//}}
	//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
	LUA_GET_VALUE_RETURN(	luaManager, "m_bIsTimeEvent",		pQuestTemplet->m_bIsTimeEvent,	false,		goto LoadFail; );
	//}}

	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrMainText",			pQuestTemplet->m_wstrMainText,			L"",	goto LoadFail; );
	//LUA_GET_VALUE_RETURN(	luaManager, "m_iMainText_Index",			m_nString_Index,		0,		goto LoadFail; );
	//pQuestTemplet->m_wstrMainText = GET_SCRIPT_STRING(m_nString_Index);
	
	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrThanksText",			pQuestTemplet->m_wstrThanksText,		L"",	goto LoadFail; );
	//LUA_GET_VALUE_RETURN(	luaManager, "m_iThanksText_Index",			m_nString_Index,		0,		goto LoadFail; );
	//pQuestTemplet->m_wstrThanksText = GET_SCRIPT_STRING(m_nString_Index);
	
	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrDissClearText",		pQuestTemplet->m_wstrDissClearText,		L"",	goto LoadFail; );
	//LUA_GET_VALUE_RETURN(	luaManager, "m_iDissClearText_Index",			m_nString_Index,		0,		goto LoadFail; );
	//pQuestTemplet->m_wstrDissClearText = GET_SCRIPT_STRING(m_nString_Index);

#ifdef SERV_EPIC_QUEST

	LUA_GET_VALUE(		luaManager, "m_iEpisodeGroupID",	pQuestTemplet->m_iEpisodeGroupID,	0 );
	LUA_GET_VALUE(		luaManager, "m_iPlayLevel",		pQuestTemplet->m_iPlayLevel,		0 );
	LUA_GET_VALUE(		luaManager, "m_iFairLevel",		pQuestTemplet->m_iFairLevel,		0 );

	LUA_GET_VALUE(		luaManager, "m_wstrEpisodeGroupTitle",		pQuestTemplet->m_wstrEpisodeGroupTitle,	L"" );
	LUA_GET_VALUE(		luaManager, "m_wstrStartScene",			pQuestTemplet->m_wstrStartScene,		L"" );
	LUA_GET_VALUE(		luaManager, "m_wstrEndScene",				pQuestTemplet->m_wstrEndScene,			L"" );	
	LUA_GET_VALUE(		luaManager, "m_iNextVillageID",			pQuestTemplet->m_iNextVillageID,		0 );//특정 퀘스트 완료 시 다음 마을로 이동에 대한 가이드 추가
	// 등록
	if( 0 != pQuestTemplet->m_iEpisodeGroupID && false == pQuestTemplet->m_wstrEpisodeGroupTitle.empty() )
	{
		if(m_mapEpicGroupTitle.find(pQuestTemplet->m_iEpisodeGroupID) == m_mapEpicGroupTitle.end())
		{
			m_mapEpicGroupTitle.insert( std::make_pair( pQuestTemplet->m_iEpisodeGroupID, pQuestTemplet->m_wstrEpisodeGroupTitle ) );
		}
	}
	//}}
#endif SERV_EPIC_QUEST

#ifdef SERV_RANDOM_DAY_QUEST
	LUA_GET_VALUE(		luaManager, "m_iRandomGroupID",	pQuestTemplet->m_iRandomGroupID,		0 );
#endif SERV_RANDOM_DAY_QUEST

	if( LoadQuestCondition( luaManager, pQuestTemplet->m_Condition ) == false )
		goto LoadFail;



	ASSERT( pQuestTemplet->m_Condition.m_eUnitType == CX2Unit::UT_NONE || pQuestTemplet->m_Condition.m_setUnitClass.empty() == true );
	if( pQuestTemplet->m_Condition.m_eUnitType != CX2Unit::UT_NONE && pQuestTemplet->m_Condition.m_setUnitClass.empty() == false )
	{
		wstringstream wstrstm;
		wstrstm << L"QuestID: " << pQuestTemplet->m_iID;
		ErrorLogMsg( XEM_ERROR125, wstrstm.str().c_str() );
	}



	if( QT_CHANGE_JOB == pQuestTemplet->m_eQuestType )
	{
		CHANGE_JOB_TYPE eJobType;
		LUA_GET_VALUE_ENUM(	luaManager, "CHANGE_JOB_TYPE",		eJobType, CHANGE_JOB_TYPE, CJT_JOB_NONE );

		//ASSERT( eJobType > CJT_NONE && eJobType < CJT_JOB_END );
		if( eJobType >= CJT_JOB_NONE && eJobType < CJT_JOB_END )
		{
			ASSERT( 1 == pQuestTemplet->m_Condition.m_setUnitClass.size() );


			std::set<int>& setClassChangeQuestID = m_mapChangeJobQuest[ *(pQuestTemplet->m_Condition.m_setUnitClass.begin()) ].m_setQuestID[eJobType];
			setClassChangeQuestID.insert( pQuestTemplet->m_iID );
		}
	}

	if( luaManager.BeginTable( "m_vecSubQuest" ) == true )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == true )
		{
			if( buf > 0 )
				pQuestTemplet->m_vecSubQuest.push_back( buf );
			index++;
		}

		luaManager.EndTable();
	}

	if( true == pQuestTemplet->m_vecSubQuest.empty() )
	{
		ASSERT( !"there is no subquest" );
	}
#ifdef REFORM_QUEST
	if( luaManager.BeginTable( "m_vecSubQuest_Group" ) == true )
	{
		int index = 1; 
		while( luaManager.BeginTable( index ) == true )
		{
			int iSubIndex	= 1; 
			int iSubQuestID = -1;
			while( luaManager.GetValue( iSubIndex, iSubQuestID ) == true )
			{
				if( -1 != iSubQuestID )
				{
					std::map< int, std::vector<int> >::iterator mit = pQuestTemplet->m_mapSubQuestGroup.find( index );
					if( mit == pQuestTemplet->m_mapSubQuestGroup.end() ) // 기존 그룹이 없다면
					{
						std::vector<int> vecTemp;
						vecTemp.push_back( iSubQuestID );
						pQuestTemplet->m_mapSubQuestGroup.insert( std::make_pair( index, vecTemp ) );
					}
					else
					{
						mit->second.push_back( iSubQuestID );
					}
				}					
				iSubIndex++;
			}
			++index;
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}
#endif //REFORM_QUEST

	if( LoadReward( luaManager, "Reward", pQuestTemplet->m_Reward ) == false )
		goto LoadFail;

	if( luaManager.BeginTable( "SelectReward" ) == true )
	{
		LUA_GET_VALUE(	luaManager, "m_iSelectionCount",	pQuestTemplet->m_SelectReward.m_iSelectionCount,		1 );

		if( luaManager.BeginTable( "m_vecSelectItem" ) == true )
		{
			int index = 1; 
			while( luaManager.BeginTable( index ) == true )
			{
				ITEM_DATA itemData;
				LUA_GET_VALUE(	luaManager, "m_iItemID",		itemData.m_iItemID,		0 );
				LUA_GET_VALUE(	luaManager, "m_iQuantity",		itemData.m_iQuantity,	0 );
				LUA_GET_VALUE(	luaManager, "m_iPeriod",		itemData.m_iPeriod,		0 );
				LUA_GET_VALUE(  luaManager, "m_sSocketOption",	itemData.m_iSocketOption1, 0 );


				if( 0 != itemData.m_iItemID && 0 != itemData.m_iQuantity )
				{
					pQuestTemplet->m_SelectReward.m_vecSelectItem.push_back( itemData );
				}

				index++;
				luaManager.EndTable();
			}
			luaManager.EndTable(); // "m_vecSelectItem"
		}

		luaManager.EndTable(); // "SelectReward"
	}


	if( luaManager.BeginTable( "SHOW_QUEST_ITEM" ) == true )
	{
		int index	= 1; 
		int value	= -1;
		while( luaManager.GetValue( index, value ) == true )
		{
			if( value > 0 )
			{
				pQuestTemplet->m_vecShowItemID.push_back( value );
#ifndef INT_SHOW_QUEST_ITEM
				if( QT_EVENT == pQuestTemplet->m_eQuestType )
				{
					DISPLAY_ERROR(L"이벤트 퀘스트는 SHOW_QUEST_ITEM을 설정 할 수 없습니다.");
					pQuestTemplet->m_vecShowItemID.clear();
					break;
				}
#endif //INT_SHOW_QUEST_ITEM			
			}
			index++;
		}

		luaManager.EndTable(); // "SHOW_QUEST_ITEM"
	}



	if( luaManager.BeginTable( "HIDE_QUEST_ITEM" ) == true )
	{
		int index	= 1; 
		int value	= -1;
		while( luaManager.GetValue( index, value ) == true )
		{
			if( value > 0 )
				pQuestTemplet->m_vecHideItemID.push_back( value );
			index++;
		}

		luaManager.EndTable(); // "HIDE_QUEST_ITEM"
	}


	if( luaManager.BeginTable( "HIDE_QUEST_SKILL" ) == true )
	{
		int index	= 1; 
		int value	= -1;
		while( luaManager.GetValue( index, value ) == true )
		{
			if( value > 0 )
				pQuestTemplet->m_vecHideSkillID.push_back( value );
			index++;
		}

		luaManager.EndTable(); // "HIDE_QUEST_SKILL"
	}

	if( luaManager.BeginTable( "HIDE_QUEST_BANK_GRADE" ) == true )
	{
		int index	= 1; 
		int value	= -1;
		while( luaManager.GetValue( index, value ) == true )
		{
			if( value > 0 )
				pQuestTemplet->m_vecHideBankGrade.push_back( (CX2Inventory::MEMBERSHIP_PRIVATE_BANK) value );
			index++;
		}

		luaManager.EndTable(); // "HIDE_QUEST_BANK_GRADE"
	}

#ifdef SERV_RANDOM_DAY_QUEST
	if( pQuestTemplet->m_iRandomGroupID > 0 )
	{
		// 이미 Random GroupID 데이터가 있다. 거기에 추가하자
		std::map< int, std::vector< int > >::iterator mitRandom = m_mapRandomQuest.find( pQuestTemplet->m_iRandomGroupID );
		if( mitRandom != m_mapRandomQuest.end() )
		{
			// QuestID 중복 체크
			std::vector< int >::iterator vitRandom = mitRandom->second.begin();
			for( ; vitRandom != mitRandom->second.end() ; ++vitRandom )
			{
				if( (*vitRandom) == pQuestTemplet->m_iID )
				{
					//중복되는 랜덤 퀘스트가 있다
					break;
				}
			}
			// 중복 되는 것이 없으니 추가하자
			mitRandom->second.push_back( pQuestTemplet->m_iID );
		}
		// 새로운 Random GroupID 이다
		else
		{
			std::vector< int > vecRandomQuest;
			vecRandomQuest.push_back( pQuestTemplet->m_iID );
			m_mapRandomQuest.insert( std::make_pair( pQuestTemplet->m_iRandomGroupID, vecRandomQuest ) );
		}
	}
#endif SERV_RANDOM_DAY_QUEST

	if( m_mapQuestTemplet.find( pQuestTemplet->m_iID ) != m_mapQuestTemplet.end() )
		goto LoadFail;


	// 퀘스트 템플릿 로드 성공
	m_mapQuestTemplet.insert( std::make_pair( pQuestTemplet->m_iID, pQuestTemplet ) );
	return true;

LoadFail:
	wstringstream wstrStream;
	wstrStream << L"QuestTempletLoadFail_" << pQuestTemplet->m_iID << L"_" << pQuestTemplet->m_wstrTitle;

	ErrorLogMsg( XEM_ERROR79, wstrStream.str().c_str() );
	SAFE_DELETE( pQuestTemplet );
	return false;
}

bool CX2QuestManager::AddSubQuestTemplet_LUA()
{

	SubQuestTemplet* pSubQuestTemplet = new SubQuestTemplet();
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE_RETURN(		luaManager, "m_iID",				pSubQuestTemplet->m_iID,				0,		goto LoadFail; );

	LUA_GET_VALUE_RETURN(		luaManager, "m_wstrDescription",	pSubQuestTemplet->m_wstrDescription,	L"",	goto LoadFail; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eClearType",			pSubQuestTemplet->m_eClearType,	SUB_QUEST_TYPE,	SQT_NONE,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(		luaManager, "m_bAutomaticDescription",		pSubQuestTemplet->m_bAutomaticDescription,	true,	goto LoadFail; );


	if( LoadClearCondition( luaManager, pSubQuestTemplet ) == false )
		goto LoadFail;


	m_mapSubQuestTemplet.insert( std::make_pair( pSubQuestTemplet->m_iID, pSubQuestTemplet ) );

	return true;

LoadFail:
	wstringstream wstrStream;
	wstrStream << L"SubQuestTempletLoadFail_" << pSubQuestTemplet->m_iID << L"_" << pSubQuestTemplet->m_wstrDescription;

	ErrorLogMsg( XEM_ERROR80, wstrStream.str().c_str() );
	SAFE_DELETE( pSubQuestTemplet );
	return false;
}

#ifdef LUA_TRANS_DEVIDE
bool CX2QuestManager::AddQuestTempletTrans_LUA()
{
	QuestTemplet* pQuestTemplet = new QuestTemplet();
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	LUA_GET_VALUE_RETURN(	luaManager, "m_iID",					pQuestTemplet->m_iID,			0,			goto LoadFail; );
	LUA_GET_VALUE(			luaManager, "m_iEpisodeGroupID",		pQuestTemplet->m_iEpisodeGroupID,	0 );
	LUA_GET_VALUE(			luaManager, "m_wstrEpisodeGroupTitle",	pQuestTemplet->m_wstrEpisodeGroupTitle,	L"" );
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrTitle",				pQuestTemplet->m_wstrTitle,		L"",		goto LoadFail; );	
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrMainText",			pQuestTemplet->m_wstrMainText,			L"",	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrThanksText",			pQuestTemplet->m_wstrThanksText,		L"",	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrDissClearText",		pQuestTemplet->m_wstrDissClearText,		L"",	goto LoadFail; );

	// 여기서 치환 해주어야 함.
	map<int, QuestTemplet*>::iterator mit = m_mapQuestTemplet.find( pQuestTemplet->m_iID );
	if( mit == m_mapQuestTemplet.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"QuestTrans.Lua And Quest.lua Not Equal ItemID..." << (pQuestTemplet->m_iID) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// 에러로그 pQuestTemplet
		SAFE_DELETE(pQuestTemplet); 
		return false;
	}
	else
	{
		mit->second->m_wstrTitle = pQuestTemplet->m_wstrTitle;
		mit->second->m_wstrMainText = pQuestTemplet->m_wstrMainText;
		mit->second->m_wstrThanksText = pQuestTemplet->m_wstrThanksText;
		mit->second->m_wstrDissClearText = pQuestTemplet->m_wstrDissClearText;
	}

	// 에픽타이틀 치환
	map<int, wstring>::iterator mit2 = m_mapEpicGroupTitle.find( pQuestTemplet->m_iEpisodeGroupID );
	if( mit2 == m_mapEpicGroupTitle.end() )
	{
		if(pQuestTemplet->m_iEpisodeGroupID == 0)
		{
			// 0 인경우가 에픽퀘스트가 아닌 경우
		}
		else
		{
			m_mapEpicGroupTitle.insert( std::make_pair( pQuestTemplet->m_iEpisodeGroupID, pQuestTemplet->m_wstrEpisodeGroupTitle));
		}
	}
	else
	{
		mit2->second = pQuestTemplet->m_wstrEpisodeGroupTitle;
	}

	return true;

LoadFail:
	wstringstream wstrStream;
	wstrStream << L"Trans_QuestTempletLoadFail_" << pQuestTemplet->m_iID << L"_" << pQuestTemplet->m_wstrTitle;
	ErrorLogMsg( XEM_ERROR79, wstrStream.str().c_str() );
	SAFE_DELETE( pQuestTemplet );
	return false;
}


bool CX2QuestManager::AddSubQuestTempletTrans_LUA()
{
	SubQuestTemplet* pSubQuestTemplet = new SubQuestTemplet();
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	LUA_GET_VALUE_RETURN(	luaManager, "m_iID",				pSubQuestTemplet->m_iID,				0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, "m_wstrDescription",	pSubQuestTemplet->m_wstrDescription,	L"",	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, "m_bAutomaticDescription",		pSubQuestTemplet->m_bAutomaticDescription,	true,	goto LoadFail; );

	// 여기서 치환 해주어야 함.
	map<int, SubQuestTemplet*>::iterator mit;
	mit = m_mapSubQuestTemplet.find( pSubQuestTemplet->m_iID );
	if( mit == m_mapSubQuestTemplet.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"SubQuestTrans.Lua And SubQuest.lua Not Equal ItemID..." << (pSubQuestTemplet->m_iID) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// 에러로그 pQuestTemplet
		SAFE_DELETE(pSubQuestTemplet); 
		return false;
	}
	else
	{
		mit->second->m_wstrDescription = pSubQuestTemplet->m_wstrDescription;
		mit->second->m_bAutomaticDescription = pSubQuestTemplet->m_bAutomaticDescription;
	}

	return true;

LoadFail:
	wstringstream wstrStream;
	wstrStream << L"Trans_SubQuestTempletLoadFail_" << pSubQuestTemplet->m_iID << L"_" << pSubQuestTemplet->m_wstrDescription;

	ErrorLogMsg( XEM_ERROR80, wstrStream.str().c_str() );
	SAFE_DELETE( pSubQuestTemplet );
	return false;
}
#endif LUA_TRANS_DEVIDE

#ifdef DAY_OF_THE_WEEK_QUEST
bool CX2QuestManager::AddAccountQuestDate_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	int iAccountQuestID = - 1;
	UINT iDayOfWeek = -1;
	LUA_GET_VALUE(	luaManager, "AccountQuestID",	iAccountQuestID, -1 );
	LUA_GET_VALUE(	luaManager, "DayOfWeek",		iDayOfWeek, -1 );

	if( -1 == iAccountQuestID || -1 == iDayOfWeek )
		return false;

	m_mapDayOfTheWeekQuestList[iAccountQuestID] = iDayOfWeek; // 요일별 퀘스트 요일 정보
	return true;
}
UINT CX2QuestManager::GetQuestDayByQuestID( int iQuestID_ )
{
	// 1 = 일요일
	// 2 = 월요일
	// ....
	// 7 = 토요일
	map< int, UINT >::iterator it = m_mapDayOfTheWeekQuestList.find( iQuestID_ );
	if( it == m_mapDayOfTheWeekQuestList.end() )
		return 0;

	return it->second;
}
#endif //DAY_OF_THE_WEEK_QUEST
//{{ kimhc // 2010.4.12 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST

void CX2QuestManager::SetUnitQuest( const std::vector<KQuestInstance>& vecQuestInst, const std::vector<KCompleteQuestInfo>& vecCompleteQuest )
{
	ClearUnitQuest();

	// 캐릭터가 진행중인 퀘스트
	BOOST_TEST_FOREACH( const KQuestInstance&, val, vecQuestInst )
	{
		QuestInst* pQuestInst = new QuestInst();
		pQuestInst->SetKQuestInstance( &val );
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR			
		InsertCompletedVisitQuestID( pQuestInst );
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
		m_mapUnitQuest.insert( std::make_pair( pQuestInst->m_iID, pQuestInst ) );
	}

	__time64_t t64TimeToRefreshDailyQuest = GetTimeToRefreshQuest();

	// 캐릭터가 완료한 퀘스트
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, val, vecCompleteQuest )
	{
		// 퀘스트가 일일 퀘스트 이고
		const QuestTemplet* pQuestTemplet = GetQuestTemplet( val.m_iQuestID );

		if ( pQuestTemplet != NULL &&
			 pQuestTemplet->m_eRepeatType == QRT_DAY )
		{
			// 퀘스트 완료가 갱신 시간보다 전 이면 완료 목록에서 빼줌
#ifdef CORRECT_QUEST_CLEAR_TIME
			__int64 iCurrent = CTime::GetCurrentTime().GetTime();
			__int64 iServerCurrent = g_pData->GetServerCurrentTime();
			__int64 iServerCompleted = val.m_tCompleteDate - iCurrent + iServerCurrent;
			if ( iServerCompleted < t64TimeToRefreshDailyQuest )
#else CORRECT_QUEST_CLEAR_TIME
			if ( val.m_tCompleteDate < t64TimeToRefreshDailyQuest )
#endif CORRECT_QUEST_CLEAR_TIME
				continue;
		}

		map<int, int>::iterator mItr = m_mapCompleteQuest.find( val.m_iQuestID );

		if ( mItr == m_mapCompleteQuest.end() )
			m_mapCompleteQuest[val.m_iQuestID] = 1;
		else
			(mItr->second)++;
	}
#ifdef REFORM_NOVICE_GUIDE
	if( NULL != g_pData && NULL != g_pData->GetPlayGuide() &&
		NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
	{
		if( m_mapCompleteQuest.find( CX2PlayGuide::TQI_CHASE_THIEF ) != m_mapCompleteQuest.end() )
		{
			g_pData->GetPlayGuide()->SetShowDungeonMenu(true);
		}

		if( m_mapCompleteQuest.find( CX2PlayGuide::TQI_VISIT_RUIN_OF_ELF ) != m_mapCompleteQuest.end() ||
			10 <= g_pData->GetSelectUnitLevel() )
		{
			g_pData->GetPlayGuide()->SetCompleteTutorial(true);
		}
		else
		{
			g_pData->GetPlayGuide()->SetCompleteTutorial(false);
		}
	}
#endif //REFORM_NOVICE_GUIDE
#ifdef REFORM_QUEST
	SetHasDungeonQuest();
#endif //REFORM_QUEST
}

__int64 CX2QuestManager::GetTimeToRefreshQuest() const
{
	// 일일 퀘스트가 갱신 되야하는 시간을 구함 (매일 AM 6시에 갱신)
	// ex) 현재 시간이 04월 10일 AM 5 시 이면 퀘스트가 갱신 되었어야 하는 시간은
	// 04월 9일 AM 6시가 갱신되어야 할 시간이고 (이전날 AM 6시),
	// 현재 시간이 04월 10일 AM 8시 이면 04월 10일 AM 6시가 갱신되어야 할 시간임

	
	CTime cTime( g_pData->GetServerCurrentTime() );

	// 현재 시간이 갱신시간 보다 작으면( ex: 0시 ~ 5시 59분 이면 )
	if ( cTime.GetHour() < HOUR_TO_REFRESH_DAILY_QUEST )
	{
		// 어제 기준시간으로 맞춤( ex: 25일 5시면, 24일 6시로 맞춤 )
		cTime -= CTimeSpan( 1, 0, cTime.GetMinute(), cTime.GetSecond() );
		cTime += CTimeSpan( 0, HOUR_TO_REFRESH_DAILY_QUEST - cTime.GetHour(), 0, 0 );
	}
	else // 현재 시간이 갱신 시간 보다 크면 ( ex: 6시 ~ 23시 59분 이면 )
	{
		// 오늘 기준시간으로 맞춤 ( ex: 25일 8시면, 25일 6시로 맞춤 )
		cTime -= CTimeSpan( 0, cTime.GetHour() - HOUR_TO_REFRESH_DAILY_QUEST, cTime.GetMinute(), cTime.GetSecond() );
	}

	return cTime.GetTime();
}

bool CX2QuestManager::RefreshDailyQuest()
{
	if ( m_mapCompleteQuest.size() == 0 )
		return false;

	map<int, int>::iterator mItr = m_mapCompleteQuest.begin();
	vector<int> vecDailyQuest;

	while ( mItr != m_mapCompleteQuest.end() )
	{
		// 완료 퀘스트 목록 중 일일퀘스트가 있으면
		const CX2QuestManager::QuestTemplet* pQuestTemplet = GetQuestTemplet( mItr->first );
		if ( pQuestTemplet != NULL && pQuestTemplet->m_eRepeatType == QRT_DAY )
			vecDailyQuest.push_back( mItr->first );

		mItr++;
	}	

	BOOST_TEST_FOREACH( int, val, vecDailyQuest )
	{
		m_mapCompleteQuest.erase( val );
	}

	return true;
}
#else	SERV_DAILY_QUEST

void CX2QuestManager::SetUnitQuest( const std::vector<KQuestInstance>& vecQuestInst, const std::vector<int>& vecCompleteQuest )
{
	ClearUnitQuest();

	for( int  i = 0; i < (int)vecQuestInst.size(); i++ )
	{
		const KQuestInstance* pKQuestInstance = &vecQuestInst[i];
		QuestInst* pQuestInst = new QuestInst();
		pQuestInst->SetKQuestInstance( pKQuestInstance );

		m_mapUnitQuest.insert( std::make_pair(  pQuestInst->m_iID, pQuestInst ) );
	}

	for( i = 0; i < (int)vecCompleteQuest.size(); i++ )
	{
		int iSubQuestID = vecCompleteQuest[i];
		std::map<int,int>::iterator mit;
		mit = m_mapCompleteQuest.find( iSubQuestID );

		if( mit == m_mapCompleteQuest.end() )
		{
			m_mapCompleteQuest[iSubQuestID] = 1;
		}
		else
		{
			++(mit->second);
		}
	}
}

#endif	SERV_DAILY_QUEST
//}} kimhc // 2010.4.12 // 비밀던전 작업(일일퀘스트 시스템)


bool CX2QuestManager::CreateUnitQuest( int iQuestID, const KQuestInstance& questInst )
{
	map<int,QuestInst*>::iterator mit = m_mapUnitQuest.find( iQuestID );

	if( mit != m_mapUnitQuest.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"CreateQuest_AddQuest Fail" << L"_" << iQuestID;
		ErrorLogMsg( XEM_ERROR81, wstrStream.str().c_str() );

		return false;
	}
	QuestInst* pQuestInst = new QuestInst();
	pQuestInst->SetKQuestInstance( &questInst );

	m_mapUnitQuest.insert( std::make_pair( pQuestInst->m_iID, pQuestInst ) );

	return true;
}

#ifdef SERV_EPIC_QUEST
bool CX2QuestManager::CheckUpdateSubQuestInst( const int iQuestID_,  const KSubQuestInstance& NewSubQuestInst_ )
{
	map<int,QuestInst*>::iterator mit = m_mapUnitQuest.find( iQuestID_ );

	if( mit == m_mapUnitQuest.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"UpdateQuest_QuestUpdate Fail" << L"_" << iQuestID_;
		ErrorLogMsg( XEM_ERROR81, wstrStream.str().c_str() );

		return false;
	}

	const QuestInst* pQuestInst = mit->second;

	const vector<SubQuestInst>& vecSubQuestInst = pQuestInst->m_vecSubQuestInst;

	for( UINT i = 0; i < vecSubQuestInst.size(); i++ )
	{
		const SubQuestInst& OldSubQuestInst = vecSubQuestInst[i];

		if(OldSubQuestInst.m_iID == NewSubQuestInst_.m_iID)
		{
			if(OldSubQuestInst.m_ucClearData != NewSubQuestInst_.m_ucClearData ||
				OldSubQuestInst.m_bIsSuccess != NewSubQuestInst_.m_bIsSuccess)
				return true;
			else
				return false;
		}
	}

	return false;

}
bool CX2QuestManager::GetCompletableQuestEpic( std::vector<int>& vecQuestID )
{	
	vecQuestID.clear();

	map<int, QuestTemplet*>::const_iterator it;

	for( it = m_mapQuestTemplet.begin(); it != m_mapQuestTemplet.end(); it++ )
	{
		QuestTemplet* pTemplet = (QuestTemplet*) it->second;
		if( pTemplet == NULL )
			continue;
		
		if( true == IsForbiddenQuest( pTemplet->m_iID ) )
			continue;

		vecQuestID.push_back( (int)it->first );
	}

	// NPC가 완료해줄 수 있는 퀘스트중에서 현재 내가 완료 할 수 있는 퀘스트 만~~
	for( UINT i=0; i<vecQuestID.size(); i++ )
	{
		const CX2QuestManager::QuestInst* pQuestInst = GetUnitQuest( vecQuestID[i] );
		if( NULL != pQuestInst )
		{
			if( false == pQuestInst->IsComplete() )
			{
				vecQuestID.erase( vecQuestID.begin() + i );
				i--;
			}
		}
		else
		{
			vecQuestID.erase( vecQuestID.begin() + i );
			i--;
		}
	}

	return true;
}
#endif SERV_EPIC_QUEST


bool CX2QuestManager::UpdateUnitQuest( int iQuestID, const KQuestInstance& questInst )
{
	map<int,QuestInst*>::iterator mit = m_mapUnitQuest.find( iQuestID );

	if( mit == m_mapUnitQuest.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"UpdateQuest_QuestUpdate Fail" << L"_" << iQuestID;
		ErrorLogMsg( XEM_ERROR81, wstrStream.str().c_str() );

		return false;
	}

	mit->second->SetKQuestInstance( &questInst );

	return true;
}

const CX2QuestManager::QuestTemplet*	CX2QuestManager::GetQuestTemplet( int questID )
{
	map<int,QuestTemplet*>::iterator iter;
	iter = m_mapQuestTemplet.find( questID );

	if( iter == m_mapQuestTemplet.end() )	
		return NULL;
	
	
#ifdef ADD_SERVER_GROUP
	QuestTemplet *pQuest = (QuestTemplet*)iter->second;
	if( pQuest != NULL && (pQuest->m_iServerGroupID != -1 && pQuest->m_iServerGroupID != g_pInstanceData->GetServerGroupID() ) )
	{
		return NULL;
	}
	else
	{
		return pQuest;
	}
#else
	return (QuestTemplet*)iter->second;
#endif
}

const CX2QuestManager::SubQuestTemplet* CX2QuestManager::GetSubQuestTemplet( int subQuestID )
{
	map<int,SubQuestTemplet*>::iterator iter;
	iter = m_mapSubQuestTemplet.find( subQuestID );
	if( iter == m_mapSubQuestTemplet.end() )
		return NULL;
	else
		return (SubQuestTemplet*)iter->second;
}

CX2QuestManager::QuestInst* CX2QuestManager::GetUnitQuest( int iQuestID )
{
	map<int,QuestInst*>::iterator mit;
	mit = m_mapUnitQuest.find( iQuestID );

	if( mit == m_mapUnitQuest.end() )
		return NULL;

	return mit->second;
}

bool CX2QuestManager::GetUnitCompleteQuest( int iQuestID )
{
	map<int,int>::iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );

	if( mit == m_mapCompleteQuest.end() )
		return false;

	return true;
}

bool CX2QuestManager::GetStartQuestList( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID )
{
	map<int, QuestTemplet*>::const_iterator it;

	for( it = m_mapQuestTemplet.begin(); it != m_mapQuestTemplet.end(); it++ )
	{
		QuestTemplet* pTemplet = (QuestTemplet*) it->second;
	
		if( pTemplet == NULL )
			continue;

		if( pTemplet->m_eStartNPCID != eQuestNPCID )
			continue;		

		if( true == IsForbiddenQuest( pTemplet->m_iID ) )
			continue;

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 제거
		if( pTemplet->m_eQuestType == QT_EPIC )
			continue;	
#endif SERV_EPIC_QUEST


		vecQuestID.push_back( (int)it->first );
	}

	if( true == vecQuestID.empty() )
		return false;
	else
		return true;

}


bool CX2QuestManager::GetEndQuestList( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID )
{
	map<int, QuestTemplet*>::const_iterator it;

	for( it = m_mapQuestTemplet.begin(); it != m_mapQuestTemplet.end(); it++ )
	{
		QuestTemplet* pTemplet = (QuestTemplet*) it->second;
		if( pTemplet == NULL )
			continue;

		if( pTemplet->m_eEndNPCID != eQuestNPCID )
			continue;

		if( true == IsForbiddenQuest( pTemplet->m_iID ) )
			continue;

		vecQuestID.push_back( (int)it->first );
	}

	if( vecQuestID.size() > 0 )
		return true;
	else
		return false;
}

int CX2QuestManager::GetNewQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, int &nRepeat, int &nNormal, int &nEvent, int checkLevel )
{
	int nFairLvQuest = 0;
	nRepeat = nNormal = nEvent = 0;

	if( CX2UnitManager::NUI_NONE == eQuestNPCID )
		return nFairLvQuest;

	vector<int> vecQuest;
	GetAvailableQuest(eQuestNPCID, vecQuest);



	for(UINT i=0; i<vecQuest.size(); ++i)
	{
		const CX2QuestManager::QuestTemplet* pQuestTemplet = GetQuestTemplet( vecQuest[i] );

#ifdef ADD_SERVER_GROUP
		if( pQuestTemplet == NULL )
			continue;
#endif

		//{{ kimhc // 2010.2.1 //	PC방이 아닌곳에서는 받지 않은 PC방퀘스트 목록에서 보이지 않도록
#ifdef	PC_BANG_QUEST
		const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( vecQuest[i] );

		if ( pQuestTemplet->m_bIsPcBang == true && g_pData->GetMyUser()->GetIsPCRoom() == false
			&& pQuestInst == NULL )
			continue;
#endif	PC_BANG_QUEST
		//}} kimhc // 2010.2.1 //	PC방이 아닌곳에서는 받지 않은 PC방퀘스트 목록에서 보이지 않도록

		//{{ kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
#ifdef	SERV_DAY_QUEST
		if ( pQuestTemplet->m_bIsTimeEvent == true )
			continue;
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		if( IsNewUserOnlyQuest( vecQuest[i] ) == true &&
			g_pInstanceData->IsRecruit() == false )
			continue;
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
		switch ( pQuestTemplet->m_eQuestType )
		{
		default:
			{
				switch ( pQuestTemplet->m_eRepeatType )
				{
				default:
					{
						++nNormal;

						// 적정렙의 퀘스트인지 판단한다.
						int iLevelGap = checkLevel - pQuestTemplet->m_iFairLevel;
						if( iLevelGap <= 5 || pQuestTemplet->m_eQuestType == QT_CHANGE_JOB )
						{
							++nFairLvQuest;
						}
					}
					break;

				case QRT_REPEAT:
					++nRepeat;
					break;
				}
			}
			break;

		case QT_EVENT:
			//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
		case QT_PCBANG:
#endif	PC_BANG_QUEST
			//}} kimhc // 2010.2.1 //	PC방 퀘스트
			++nEvent;
			break;
		}
#else	SERV_DAILY_QUEST
		
		if(pQuestTemplet->m_bRepeat == true && pQuestTemplet->m_eQuestType != QT_EVENT 
			//{{ kimhc // 2010.2.1 //	PC방 퀘스트
	#ifdef	PC_BANG_QUEST
			&& pQuestTemplet->m_eQuestType != QT_PCBANG
	#endif	PC_BANG_QUEST
			//}} kimhc // 2010.2.1 //	PC방 퀘스트
			) 
		{
			++nRepeat;			
		}
		else if(pQuestTemplet->m_bRepeat == false && pQuestTemplet->m_eQuestType != QT_EVENT 
			//{{ kimhc // 2010.2.1 //	PC방 퀘스트
	#ifdef	PC_BANG_QUEST
			&& pQuestTemplet->m_eQuestType != QT_PCBANG
	#endif	PC_BANG_QUEST
			//}} kimhc // 2010.2.1 //	PC방 퀘스트 
			) 
		{
			++nNormal;

			// 적정렙의 퀘스트인지 판단한다.
			int iLevelGap = checkLevel - pQuestTemplet->m_iFairLevel;
			if( iLevelGap <= 5 || pQuestTemplet->m_eQuestType == CX2QuestManager::QT_CHANGE_JOB )
			{
				++nFairLvQuest;
			}
		}
		else if(pQuestTemplet->m_eQuestType == QT_EVENT //{{ kimhc // 2010.2.1 //	PC방 퀘스트
	#ifdef	PC_BANG_QUEST
			|| pQuestTemplet->m_eQuestType == QT_PCBANG
	#endif	PC_BANG_QUEST
			//}} kimhc // 2010.2.1 //	PC방 퀘스트
			)
		{
			++nEvent;
		}

#endif	SERV_DAILY_QUEST
//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
	}

	return nFairLvQuest;
}

void CX2QuestManager::GetDoQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, int &nQuest )
{
	nQuest = 0;

	if( CX2UnitManager::NUI_NONE == eQuestNPCID )
		return;

	vector<int> vecQuest;
	GetOnGoingQuest(eQuestNPCID, vecQuest);
	
	nQuest = vecQuest.size();
}

void CX2QuestManager::GetCompleteQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, int &nRepeat, int &nNormal )
{
	nRepeat = nNormal = 0;

	if( CX2UnitManager::NUI_NONE == eQuestNPCID )
		return;

	vector<int> vecQuest;
	GetCompletableQuest(eQuestNPCID, vecQuest);

	for(UINT i=0; i<vecQuest.size(); ++i)
	{
		const CX2QuestManager::QuestTemplet* pQuestTemplet = GetQuestTemplet( vecQuest[i] );

#ifdef ADD_SERVER_GROUP
		if( pQuestTemplet == NULL )
			continue;
#endif

			//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
		if( QRT_REPEAT == pQuestTemplet->m_eRepeatType )
#else	SERV_DAILY_QUEST
		if( true == pQuestTemplet->m_bRepeat )
#endif	SERV_DAILY_QUEST
			//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
		{
			++nRepeat;
		}
		else
		{
			++nNormal;
		}
	}
}

#ifdef SERV_EPIC_QUEST
bool CX2QuestManager::GetBoardStartQuestList(std::vector<int>& vecQuestID )
{
#ifdef MODIFY_ACCEPT_QUEST // 현재 마을 설정
	SEnum::VILLAGE_MAP_ID CurrentVillageID = SEnum::VMI_INVALID;
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			CurrentVillageID = g_pData->GetLocationManager()->GetCurrentVillageID();
		} break;
	case CX2Main::XS_BATTLE_FIELD:
		{
			CurrentVillageID = static_cast<SEnum::VILLAGE_MAP_ID>(g_pData->GetBattleFieldManager().GetReturnVillageId());
		} break;
	default:
		break;
	}
#else
	SEnum::VILLAGE_MAP_ID CurrentVillageID = g_pData->GetLocationManager()->GetCurrentVillageID();
#endif // MODIFY_ACCEPT_QUEST
	SEnum::VILLAGE_MAP_ID BoardVillageID = SEnum::VMI_RUBEN;

	switch(CurrentVillageID)
	{

	case SEnum::VMI_RUBEN:
	case SEnum::VMI_DUNGEON_GATE_RUBEN:
	case SEnum::VMI_DUNGEON_LOUNGE_RUBEN:
		{
			BoardVillageID = SEnum::VMI_RUBEN;
		}
		break;
	case SEnum::VMI_ELDER:
	case SEnum::VMI_DUNGEON_GATE_ELDER:
	case SEnum::VMI_DUNGEON_LOUNGE_ELDER:
	case SEnum::VMI_BATTLE_FIELD_ELDER_REST_00:
		{
			BoardVillageID = SEnum::VMI_ELDER;
		}
		break;
	case SEnum::VMI_BESMA:
	case SEnum::VMI_DUNGEON_GATE_BESMA:
	case SEnum::VMI_DUNGEON_LOUNGE_BESMA:
	case SEnum::VMI_BATTLE_FIELD_BESMA_REST_00:
		{
			BoardVillageID = SEnum::VMI_BESMA;
		}
		break;
	case SEnum::VMI_ALTERA:
	case SEnum::VMI_DUNGEON_GATE_ALTERA:
	case SEnum::VMI_DUNGEON_LOUNGE_ALTERA:
	case SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00:
		{
			BoardVillageID = SEnum::VMI_ALTERA;
		}
		break;
	case SEnum::VMI_VELDER:
	case SEnum::VMI_DUNGEON_GATE_VELDER:
	case SEnum::VMI_DUNGEON_LOUNGE_VELDER:
	case SEnum::VMI_BATTLE_FIELD_VELDER_REST_00:
		{
			BoardVillageID = SEnum::VMI_VELDER;
		}
		break;
	case SEnum::VMI_PEITA:
	case SEnum::VMI_DUNGEON_LOUNGE_PEITA:
	case SEnum::VMI_BATTLE_FIELD_PEITA_REST_00:
		{
			BoardVillageID = SEnum::VMI_PEITA;
		}
		break;
	case SEnum::VMI_HAMEL:
	case SEnum::VMI_DUNGEON_GATE_HAMEL:
	case SEnum::VMI_DUNGEON_LOUNGE_HAMEL:
	case SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00:
		{
			BoardVillageID = SEnum::VMI_HAMEL;
		} break;

	case SEnum::VMI_SANDER:
	case SEnum::VMI_BATTLE_FIELD_SANDER_REST_00:	/// 윈드스톤 유적지
		{
			BoardVillageID = SEnum::VMI_SANDER;
		}
		break;
	}


	// 해서 해당하는 NPC들의 정보를 저장하자.
	CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( BoardVillageID );

	for(std::vector<CX2LocationManager::HOUSE_ID>::iterator it = pVillageTemplet->m_HouseList.begin(); it < pVillageTemplet->m_HouseList.end(); it++)
	{
		CX2LocationManager::HouseTemplet* pHouseTemplet = g_pData->GetLocationManager()->GetHouseTemplet(*it);

		for(std::vector<CX2UnitManager::NPC_UNIT_ID>::iterator itNPC = pHouseTemplet->m_NPCList.begin(); itNPC < pHouseTemplet->m_NPCList.end(); itNPC++)
		{
			GetStartQuestList( *itNPC, vecQuestID );
		}
	}


	// VMI_PEITA의 경우 위에서 이미 추가됨
	if(CurrentVillageID != SEnum::VMI_PEITA)
	{
		GetStartQuestList(CX2UnitManager::NUI_GLAVE, vecQuestID);
		GetStartQuestList(CX2UnitManager::NUI_LUTO_VILLAGE, vecQuestID);
		GetStartQuestList(CX2UnitManager::NUI_EVENT_DAO, vecQuestID);
#ifndef EVENT_NPC_IN_VILLAGE
		GetStartQuestList(CX2UnitManager::NUI_EVENT_BENDERS, vecQuestID);
#endif //EVENT_NPC_IN_VILLAGE
	}

	return true;
}
#endif SERV_EPIC_QUEST




bool CX2QuestManager::GetAvailableQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID )
{
	if( CX2UnitManager::NUI_NONE == eQuestNPCID )
		return false;

	vecQuestID.clear();

#ifdef SERV_EPIC_QUEST
	if(eQuestNPCID == CX2UnitManager::NUI_BILLBOARD)
	{
		GetBoardStartQuestList(vecQuestID);
	}
	else
	{
		GetStartQuestList( eQuestNPCID, vecQuestID );
	}
#else
	GetStartQuestList( eQuestNPCID, vecQuestID );						// 받을 수 있는 퀘스트 
#endif SERV_EPIC_QUEST

	const map<int, int>& mapQuestClear = GetMapCompleteQuest();			// 이미 완료, 보상 받은 퀘스트
	map<int, int>::const_iterator it;


	//전직퀘스트 중에서 한쪽은 안보이게 하자
	const CX2QuestManager::ChangeJobQuest& changeJobQuest = 
		g_pData->GetQuestManager()->GetChangeJobQuest( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );


	// 가지고 있거나 완료한 전직 퀘스트중에서 해당하는 것을 찾는다.
	CHANGE_JOB_TYPE ePickedChangeJobType = CJT_JOB_NONE;
	for( UINT i = CJT_JOB_FIRST_OLD; i < CJT_JOB_END; ++i )
	{
		const std::set<int>& setClassChangeQuestID = changeJobQuest.m_setQuestID[ CHANGE_JOB_TYPE(i) ];
		BOOST_TEST_FOREACH( int, iClassChangeQuestID, setClassChangeQuestID )
		{
			if( NULL != g_pData->GetQuestManager()->GetUnitQuest( iClassChangeQuestID ) ||
				mapQuestClear.end() != mapQuestClear.find( iClassChangeQuestID ) )
			{
				ePickedChangeJobType = static_cast<CHANGE_JOB_TYPE>( i );
				break;
			}
		}
	}

	// 받을 수 있는 퀘스트 중에서 내가 가지고 있는 퀘스트는 빼자
	for( UINT i=0; i<vecQuestID.size(); i++ )
	{
		const CX2QuestManager::QuestInst* pQuestInst = GetUnitQuest( vecQuestID[i] );
		if( NULL != pQuestInst )
		{
			vecQuestID.erase( vecQuestID.begin() + i );
			i--;
		}
	}

	for( UINT i=0; i<vecQuestID.size(); i++ )
	{
		const CX2QuestManager::QuestTemplet* pQuestTemplet = GetQuestTemplet( vecQuestID[i] );
		if( NULL == pQuestTemplet )
		{
			vecQuestID.erase( vecQuestID.begin() + i );
			i--;
			continue;
		}

		// 레벨제한에 걸리는 애들은 빼자
#ifdef REFORM_QUEST
		const int iSelectUnitLevel = g_pData->GetSelectUnitLevel();
		if( iSelectUnitLevel < pQuestTemplet->m_Condition.m_iLevel ||
			(-1 != pQuestTemplet->m_Condition.m_iLimitLevel && iSelectUnitLevel > pQuestTemplet->m_Condition.m_iLimitLevel ) )
#else
		if( pQuestTemplet->m_Condition.m_iLevel > g_pData->GetSelectUnitLevel() )
#endif //REFORM_QUEST
		{
			vecQuestID.erase( vecQuestID.begin() + i );
			i--;
			continue;
		}

			//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
		if( QRT_REPEAT != pQuestTemplet->m_eRepeatType )
#else	SERV_DAILY_QUEST
		if( false == pQuestTemplet->m_bRepeat )
#endif	SERV_DAILY_QUEST
			//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
		{
			it = mapQuestClear.find( vecQuestID[i] );
			if( it != mapQuestClear.end() )
			{
				// 퀘스트 목록중에서 반복 퀘스트가 아닌 것중에 이미 완료,보상 받은 것들은 빼자
				vecQuestID.erase( vecQuestID.begin() + i );
				i--;
				continue;
			}
#ifdef SERV_RANDOM_DAY_QUEST
			if( true == CheckGiveUpRandomQuest( pQuestTemplet->m_iRandomGroupID, pQuestTemplet->m_iID ) )
			{
				vecQuestID.erase( vecQuestID.begin() + i );
				i--;
				continue;
			}
#endif SERV_RANDOM_DAY_QUEST
		}



		// **존재하는** 선행 퀘스트를 완료하지 못한 퀘스트 목록에서 제외
		bool bNotClearBeforeQuest = false;
		for( int iC = 0; iC < (int)pQuestTemplet->m_Condition.m_vecBeforeQuestID.size(); ++iC )
		{
			const CX2QuestManager::QuestTemplet* pBeforeQuestTemplet = GetQuestTemplet( pQuestTemplet->m_Condition.m_vecBeforeQuestID[iC] );
			if( NULL != pBeforeQuestTemplet )
			{
				it = mapQuestClear.find( pQuestTemplet->m_Condition.m_vecBeforeQuestID[iC] );
				if( it == mapQuestClear.end() )
				{
					vecQuestID.erase( vecQuestID.begin() + i );
					i--;
					bNotClearBeforeQuest = true;
					break;
				}
			}
		}
		if( true == bNotClearBeforeQuest )
		{
			continue;
		}


		// 피해야할 퀘스트를 완료한 퀘스트는 목록에서 제외
		if( -1 != pQuestTemplet->m_Condition.m_iNotBeforeQuestID )
		{
			if( mapQuestClear.end() != mapQuestClear.find( pQuestTemplet->m_Condition.m_iNotBeforeQuestID ) )
			{
				vecQuestID.erase( vecQuestID.begin() + i );
				i--;
				continue;
			}
		}

		if ( CJT_JOB_NONE != ePickedChangeJobType )
		{
			for ( UINT uiJobType = CJT_JOB_NONE; uiJobType < CJT_JOB_END; ++uiJobType )
			{
				// 완료 했거나 진행 중인 퀘스트 외의 전직 퀘스트는 제외
				if ( ePickedChangeJobType != uiJobType )
				{
					const std::set<int>& setClassChangeQuestID = changeJobQuest.m_setQuestID[ static_cast<CHANGE_JOB_TYPE>( uiJobType ) ];
					if( setClassChangeQuestID.find( vecQuestID[i] ) != setClassChangeQuestID.end() )
					{
						vecQuestID.erase( vecQuestID.begin() + i );
						i--;
						continue;
					}	// if
				}	// if
			}	// for
		}	// if
	}







	// temp!! 이벤트 기간 종료
	////{{ dmlee 2008.7.8 temp!! 이벤트 퀘스트 둘중에 하나만 받을 수 있다
	//{
	//	const int MAGIC_EVENT_QUEST_A = 2610;
	//	const int MAGIC_EVENT_QUEST_B = 2620;
	//	
	//	for ( int j = 0; j < (int)vecAccecptableQuest.size(); j++ )
	//	{
	//		int tempQuestID = vecAccecptableQuest[j];

	//		if( MAGIC_EVENT_QUEST_A == tempQuestID )
	//		{
	//			const CX2QuestManager::QuestInst* pTempQuestInst = g_pData->GetQuestManager()->GetUnitQuest( MAGIC_EVENT_QUEST_B );
	//			if( NULL != pTempQuestInst )
	//			{
	//				vecAccecptableQuest.erase( vecAccecptableQuest.begin() + j );
	//				j--;
	//			}
	//		}
	//		else if( MAGIC_EVENT_QUEST_B == tempQuestID )
	//		{
	//			const CX2QuestManager::QuestInst* pTempQuestInst = g_pData->GetQuestManager()->GetUnitQuest( MAGIC_EVENT_QUEST_A );
	//			if( NULL != pTempQuestInst )
	//			{
	//				vecAccecptableQuest.erase( vecAccecptableQuest.begin() + j );
	//				j--;
	//			}
	//		}
	//	}
	//}
	////}} dmlee 2008.7.8 temp!! 이벤트 퀘스트 둘중에 하나만 받을 수 있다



	// temp!! 이벤트 기간 종료
	//const int MAGIC_MARKETING_EVENT_QUEST_NECESSARY_ITEM_ID = 129765;
	//const int MAGIC_MARKETING_EVENT_QUEST_ID = 2500;
	////특정 아이템이 있으면 특정 이벤트가 안보이게끔 수정
	//for ( int j = 0; j < (int)vecAccecptableQuest.size(); j++ )
	//{
	//	int tempQuestID = vecAccecptableQuest[j];
	//	if ( tempQuestID == MAGIC_MARKETING_EVENT_QUEST_ID )
	//	{
	//		CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
	//		if( NULL != pMyUnit )
	//		{
	//			if( NULL != pMyUnit->GetInventory().GetItemByTID( MAGIC_MARKETING_EVENT_QUEST_NECESSARY_ITEM_ID ) )
	//			{
	//				vecAccecptableQuest.erase( vecAccecptableQuest.begin() + j );
	//				j--;
	//			}
	//		}
	//	}
	//}

#ifdef TEMP_MEDAL_EVENT_QUEST
	{
		const int MAGIC_EVENT_QUEST_A = 3050;	
		const int MAGIC_EVENT_QUEST_B = 3055;
		const int MAGIC_EVENT_QUEST_ITEM_ID_A = 129992;

		for ( int j = 0; j < (int)vecQuestID.size(); j++ )
		{
			int tempQuestID = vecQuestID[j];
			if( MAGIC_EVENT_QUEST_A == tempQuestID )
			{
				CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
				if( NULL != pMyUnit )
				{
					if( true == g_pData->GetQuestManager()->GetUnitCompleteQuest( MAGIC_EVENT_QUEST_B ) )
					{
						vecQuestID.erase( vecQuestID.begin() + j );
						j--;
					}

					if( pMyUnit->GetInventory().GetNumItemByTID( MAGIC_EVENT_QUEST_ITEM_ID_A ) >= 10 )
					{
						vecQuestID.erase( vecQuestID.begin() + j );
						j--;
					}
				}
			}
		}
	}
#endif TEMP_MEDAL_EVENT_QUEST







#ifdef TEMP_ITEM_DEPENDENT_REPEATITIVE_QUEST
	{
		const int MAGIC_EVENT_QUEST_ID			= 7070;
		const int MAGIC_EVENT_QUEST_ITEM_ID[]	= { 130965, 130966 };

		if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

			for( int j=0; j<(int)vecQuestID.size(); j++ )
			{
				int iQuestID = vecQuestID[j];

				if( iQuestID == MAGIC_EVENT_QUEST_ID )
				{
					if( NULL != kInventory.GetItemByTID( MAGIC_EVENT_QUEST_ITEM_ID[0] ) || 
						NULL != kInventory.GetItemByTID( MAGIC_EVENT_QUEST_ITEM_ID[1] ) )
					{
						vecQuestID.erase( vecQuestID.begin() + j );
						j--;
					}
					break;
				}
			}
		}
		//const int MAGIC_EVENT_QUEST_ID[]		= { 5860, 5870, 5880, 5890, 6120, };
		//const int MAGIC_EVENT_QUEST_ITEM_ID[]	= { 130545, 130546, 130547, 130548, 129940, };

		//if( NULL != g_pData->GetMyUser()->GetSelectUnit() &&
		//	NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory() )
		//{
		//	CX2Inventory* pInven = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

		//	for( int j=0; j<(int)vecQuestID.size(); j++ )
		//	{
		//		int iQuestID = vecQuestID[j];
		//		for( int i=0; i<ARRAY_SIZE(MAGIC_EVENT_QUEST_ID); i++ )
		//		{
		//			if( iQuestID == MAGIC_EVENT_QUEST_ID[i] )
		//			{
		//				if( NULL != kInventory.GetItemByTID( MAGIC_EVENT_QUEST_ITEM_ID[i] ) )
		//				{
		//					vecQuestID.erase( vecQuestID.begin() + j );
		//					j--;
		//				}
		//				break;
		//			}
		//		}
		//	}
		//}
	}
#endif TEMP_ITEM_DEPENDENT_REPEATITIVE_QUEST



	return true;
}
#ifdef SERV_EPIC_QUEST // oasis907 : 김상윤 [2010.7.14] // 캐릭터가 가지고 있는 퀘스트 중 완료할 수 있는 퀘스트가 하나 이상 있는지
bool CX2QuestManager::HasCompletableQuest()
{
	map<int,QuestInst*>::iterator mit;
	for( mit = m_mapUnitQuest.begin(); mit != m_mapUnitQuest.end(); mit++ )
	{
		int QuestID = mit->first;
		CX2QuestManager::QuestInst* pQuestInst = (CX2QuestManager::QuestInst*) mit->second;
		if(pQuestInst == NULL)
			continue;
		if(pQuestInst->IsComplete())
		{
			return true;
		}
	}
	return false;
}
#endif SERV_EPIC_QUEST

bool CX2QuestManager::GetCompletableQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID )
{
	if( CX2UnitManager::NUI_NONE == eQuestNPCID )
		return false;

	vecQuestID.clear();
	GetEndQuestList( eQuestNPCID, vecQuestID );		// 완료할 수 있는 퀘스트

	// NPC가 완료해줄 수 있는 퀘스트중에서 현재 내가 완료 할 수 있는 퀘스트 만~~
	for( UINT i=0; i<vecQuestID.size(); i++ )
	{
		const CX2QuestManager::QuestInst* pQuestInst = GetUnitQuest( vecQuestID[i] );
		if( NULL != pQuestInst )
		{
			if( false == pQuestInst->IsComplete() )
			{
				vecQuestID.erase( vecQuestID.begin() + i );
				i--;
			}
		}
		else
		{
			vecQuestID.erase( vecQuestID.begin() + i );
			i--;
		}
	}

	return true;
}

bool CX2QuestManager::GetOnGoingQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID )
{
	if( CX2UnitManager::NUI_NONE == eQuestNPCID )
		return false;

	vecQuestID.clear();
	GetStartQuestList( eQuestNPCID, vecQuestID );		// 완료할 수 없는 퀘스트

	// NPC가 완료해줄 수 있는 퀘스트중에서 현재 내가 완료 할 수 없는 퀘스트 만~~
	for( UINT i=0; i<vecQuestID.size(); i++ )
	{
		const CX2QuestManager::QuestInst* pQuestInst = GetUnitQuest( vecQuestID[i] );
		if( NULL != pQuestInst )
		{
			if( true == pQuestInst->IsComplete() )
			{
				vecQuestID.erase( vecQuestID.begin() + i );
				i--;
			}
		}
		else
		{
			vecQuestID.erase( vecQuestID.begin() + i );
			i--;
		}
	}

	return true;
}

D3DXCOLOR CX2QuestManager::GetQuestColor( int questID, int checkLevel )
{
	const CX2QuestManager::QuestTemplet* pQuestTemplet = GetQuestTemplet( questID );
	if( NULL == pQuestTemplet )
	{
		return D3DXCOLOR( 1,1,1,1 );
	}

	if( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_EVENT 
		//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
		|| pQuestTemplet->m_eQuestType == CX2QuestManager::QT_PCBANG
#endif	PC_BANG_QUEST
		//}} kimhc // 2010.2.1 //	PC방 퀘스트
		)
	{
		return D3DXCOLOR( 0, 1, 0, 1 );
	}

	if( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_CHANGE_JOB )
	{
		return D3DXCOLOR( 0, 1, 0, 1 );
	}

	int iLevelGap = checkLevel - pQuestTemplet->m_iFairLevel;
	if( iLevelGap > 5 )
	{
		return D3DXCOLOR( 0.7058f, 0.7058f, 0.7058f, 1 );
	}
	else if( iLevelGap >= 0 )
	{
		return D3DXCOLOR( 0, 1, 0, 1 );
	}
	else if( iLevelGap >= -2 )
	{
		return D3DXCOLOR( 0.8f, 0.8f, 0, 1 );
	}
	else
	{
		return D3DXCOLOR( 1, 0, 0, 1 );
	}
}


wstring CX2QuestManager::GetQuestColorString( int questID, int checkLevel )
{
	const CX2QuestManager::QuestTemplet* pQuestTemplet = GetQuestTemplet( questID );
	if( NULL == pQuestTemplet )
	{
		return L"#CFFFFFF";			// D3DXCOLOR( 1,1,1,1 );
	}

	if( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_EVENT
		//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
		|| pQuestTemplet->m_eQuestType == CX2QuestManager::QT_PCBANG
#endif	PC_BANG_QUEST
		//}} kimhc // 2010.2.1 //	PC방 퀘스트
		)
	{
		return L"#C00FF00";			// D3DXCOLOR( 0, 1, 0, 1 );
	}

	if( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_CHANGE_JOB )
	{
		return L"#C00FF00";			// D3DXCOLOR( 0, 1, 0, 1 );
	}

	int iLevelGap = checkLevel - pQuestTemplet->m_iFairLevel;
	if( iLevelGap > 5 )
	{
		return L"#CB3B3B3";		// D3DXCOLOR( 0.7058f, 0.7058f, 0.7058f, 1 );
	}
	else if( iLevelGap >= 0 )
	{
		return L"#C00FF00";		// D3DXCOLOR( 0, 1, 0, 1 );
	}
	else if( iLevelGap >= -2 )
	{
		return L"#CCCCC00";		// D3DXCOLOR( 0.8f, 0.8f, 0, 1 );
	}
	else
	{
		return L"#CFF0000";		// D3DXCOLOR( 1, 0, 0, 1 );
	}
}



bool CX2QuestManager::LoadQuestCondition( KLuaManager& luaManager, QuestCondition& questCondition )
{
	if( luaManager.BeginTable( "QuestCondition" ) == true )
	{
#ifdef SERV_EPIC_QUEST
	#ifdef SERV_REFORM_QUEST
		if( luaManager.BeginTable( "m_iEnableVillage" ) == true )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == true )
			{
				if( buf > 0 )
				{
					questCondition.m_setEnableVillage.insert( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}
	#else
		LUA_GET_VALUE( luaManager, "m_iEnableVillage",	questCondition.m_iEnableVillage, 0 );
	#endif //SERV_REFORM_QUEST
#endif SERV_EPIC_QUEST
		LUA_GET_VALUE_RETURN(		luaManager,	"m_iLevel",			questCondition.m_iLevel,			-1, return false; );
#ifdef REFORM_QUEST
		LUA_GET_VALUE(				luaManager,	"m_iLimitLevel",		questCondition.m_iLimitLevel,		-1 );

		if( luaManager.BeginTable( "m_iEnableDungeon" ) == true )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == true )
			{
				if( buf > 0 )
				{
					questCondition.m_setEnableDungeon.insert( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}
		if( luaManager.BeginTable( "m_iEnableBattleField" ) == true )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == true )
			{
				if( buf > 0 )
				{
					questCondition.m_setEnableBattleField.insert( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}
#endif //REFORM_QUEST
		LUA_GET_VALUE_ENUM(			luaManager, "m_eUnitType",			questCondition.m_eUnitType,			CX2Unit::UNIT_TYPE,		CX2Unit::UT_NONE );
		
		//LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eUnitClass",		questCondition.m_eUnitClass,		CX2Unit::UNIT_CLASS,	CX2Unit::UC_NONE, return false; );
		if( true == luaManager.BeginTable( "m_eUnitClass" ) )
		{
			int iUnitClass = 0;
			for( int iValueIndex = 1; iValueIndex<100; iValueIndex++ )
			{
				if( false == luaManager.GetValue( iValueIndex, iUnitClass ) )
					break;

				if( CX2Unit::UC_NONE == (CX2Unit::UNIT_CLASS) iUnitClass )
					continue;

				questCondition.m_setUnitClass.insert( (CX2Unit::UNIT_CLASS) iUnitClass );
			}
			luaManager.EndTable();	// m_eUnitClass
		}

		LUA_GET_VALUE_RETURN(		luaManager, "m_iConditionItemID",	questCondition.m_iConditionItemID,	-1, return false; );
		LUA_GET_VALUE_RETURN(		luaManager, "m_iConditionItemNum",	questCondition.m_iConditionItemNum,	-1, return false; );
		LUA_GET_VALUE(				luaManager, "m_iNotBeforeQuestID",	questCondition.m_iNotBeforeQuestID,	-1 );


		if( luaManager.BeginTable( "m_vecBeforeQuestID" ) == true )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == true )
			{
				if( buf >= 0 )
				{
					questCondition.m_vecBeforeQuestID.push_back( buf );
				}

				index++;
			}

			luaManager.EndTable();	// m_vecBeforeQuestID
		}

		luaManager.EndTable(); // QuestCondition
	}

	return true;
}



bool CX2QuestManager::LoadReward( KLuaManager& luaManager, const char* pTableNameUTF8, Reward& reward )
{
	if( luaManager.BeginTable( pTableNameUTF8 ) == true )
	{
		LUA_GET_VALUE_RETURN(	luaManager, "m_bEnable",			reward.m_bEnable,			true,	return false; );
		LUA_GET_VALUE(			luaManager, "m_iEXP",				reward.m_iEXP,				0 );
		LUA_GET_VALUE(			luaManager, "m_iED",				reward.m_iED,				0 );
		LUA_GET_VALUE(			luaManager, "m_wstrName",			reward.m_wstrName,			L"" );
		LUA_GET_VALUE(			luaManager, "m_iBuff",				reward.m_iBuff,				0 );
		LUA_GET_VALUE(			luaManager, "m_iSP",				reward.m_iSP,				0 );
		LUA_GET_VALUE_ENUM( 	luaManager, "m_eChangeUnitClass",	reward.m_eChangeUnitClass,	CX2Unit::UNIT_CLASS, CX2Unit::UC_NONE );

		if( luaManager.BeginTable( "m_vecItem" ) == true )
		{
			int index = 1; 
			while( luaManager.BeginTable( index ) == true )
			{
				ITEM_DATA itemData;
				LUA_GET_VALUE(	luaManager, "m_iItemID",			itemData.m_iItemID,				0 );
				LUA_GET_VALUE(	luaManager, "m_iQuantity",			itemData.m_iQuantity,			0 );
				LUA_GET_VALUE(	luaManager, "m_iPeriod",			itemData.m_iPeriod,				0 );
				LUA_GET_VALUE(	luaManager, "m_sSocketOption",		itemData.m_iSocketOption1,		0 );

				if( 0 != itemData.m_iItemID && 0 != itemData.m_iQuantity )
				{
					reward.m_vecItem.push_back( itemData );
				}

				index++;
				luaManager.EndTable();
			}
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}
	return true;
}



bool CX2QuestManager::LoadClearCondition( KLuaManager& luaManager, SubQuestTemplet* pSubQuestTemplet )
{
	if( luaManager.BeginTable( "m_ClearCondition" ) == true )
	{
		int iDifficulty = -1;

		switch( pSubQuestTemplet->m_eClearType )
		{
		case SQT_NPC_TALK:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eTalkNPCID",		pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID,		CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NUI_NONE, goto error_proc; );
				//LUA_GET_VALUE_RETURN(		luaManager, "m_bTalkNPC",			pSubQuestTemplet->m_ClearCondition.m_bTalkNPC,			false, goto error_proc; );

#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eVillageID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.insert( buf );
						index++;
					}
					luaManager.EndTable();
				}
#endif //REFORM_QUEST
			}
			break;

		case SQT_NPC_HUNT:
			{
#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,			SEnum::DI_NONE, goto error_proc; );
#endif REFORM_QUEST
				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		pSubQuestTemplet->m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,	CX2Dungeon::DM_INVALID );
#endif SERV_SECRET_HELL
				//}}
#ifndef REFORM_QUEST //m_cDifficulty 제거
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,			-1, goto error_proc; );
				pSubQuestTemplet->m_ClearCondition.m_cDifficulty = (char) iDifficulty;
#endif //REFORM_QUEST
				//{{ 2009. 3. 31  최육사	난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, "m_bUpperDifficulty",	pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty,	false, goto error_proc; );
				//}}
				//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eFieldID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}
					luaManager.EndTable();
				}
	#else
					LUA_GET_VALUE(				luaManager, "m_eFieldID",			pSubQuestTemplet->m_ClearCondition.m_iVillageMapID,			0 );
	#endif REFORM_QUEST
#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}

				if( luaManager.BeginTable( "m_eKillNPCID" ) == true )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.insert( static_cast<CX2UnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN(		luaManager, "m_iKillNum",			pSubQuestTemplet->m_ClearCondition.m_iKillNum,			0, goto error_proc; );

			}
			break;

		case SQT_ITEM_COLLECTION:
			{
				//LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,			SEnum::DI_NONE, goto error_proc; );
				//LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,			-1, goto error_proc; );
				//pSubQuestTemplet->m_ClearCondition.m_cDifficulty = (char) iDifficulty;
				//LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eKillNPCID",		pSubQuestTemplet->m_ClearCondition.m_eKillNPCID,		CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NUI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemID",	pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID,	0, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemNum",pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum,0, goto error_proc; );
			}
			break;

		case SQT_QUEST_ITEM_COLLECTION:
			{

#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}

				if( luaManager.BeginTable( "m_eFieldID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}
					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
	#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
				LUA_GET_VALUE(				luaManager, "m_eFieldID",			pSubQuestTemplet->m_ClearCondition.m_iVillageMapID,			0 );
	#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}
#endif //REFORM_QUEST
				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		pSubQuestTemplet->m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,	CX2Dungeon::DM_INVALID );
#endif SERV_SECRET_HELL
				//}}
#ifndef REFORM_QUEST //m_cDifficulty 제거
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				pSubQuestTemplet->m_ClearCondition.m_cDifficulty = (char) iDifficulty;
#endif //REFORM_QUEST

				//{{ 2009. 3. 31  최육사	난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, "m_bUpperDifficulty",	pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty,	false, goto error_proc; );
				//}}
				
				if( luaManager.BeginTable( "m_eKillNPCID" ) == true )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.insert( static_cast<CX2UnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemID",	pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID,	0, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemNum",pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum,0, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_fQuestItemDropRate",pSubQuestTemplet->m_ClearCondition.m_fQuestItemDropRate,0.0f, goto error_proc; );
			}
			break;

		case SQT_DUNGEON_TIME:
			{
#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
#endif //REFORM_QUEST
				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		pSubQuestTemplet->m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,	CX2Dungeon::DM_INVALID );
#endif SERV_SECRET_HELL
				//}}
#ifndef REFORM_QUEST //m_cDifficulty 제거
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,			-1, goto error_proc; );
				pSubQuestTemplet->m_ClearCondition.m_cDifficulty = (char) iDifficulty;
#endif //REFORM_QUEST			

				//{{ 2009. 3. 31  최육사	난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, "m_bUpperDifficulty",	pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty,	false, goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearTime",	pSubQuestTemplet->m_ClearCondition.m_iDungeonClearTime,	0, goto error_proc; );
			}
			break;

		case SQT_DUNGEON_RANK:
			{
#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
#endif //REFORM_QUEST
				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		pSubQuestTemplet->m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,	CX2Dungeon::DM_INVALID );
#endif SERV_SECRET_HELL
				//}}
#ifndef REFORM_QUEST //m_cDifficulty 제거
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,			-1, goto error_proc; );
				pSubQuestTemplet->m_ClearCondition.m_cDifficulty = (char) iDifficulty;
#endif //REFORM_QUEST	

				//{{ 2009. 3. 31  최육사	난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, "m_bUpperDifficulty",	pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty,	false, goto error_proc; );
				//}}
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(				luaManager, "m_iUseTitleID",		pSubQuestTemplet->m_ClearCondition.m_iUseTitleID,		-1);
#endif SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonRank",		pSubQuestTemplet->m_ClearCondition.m_eDungeonRank,	CX2DungeonRoom::RANK_TYPE,		CX2DungeonRoom::RT_NONE, goto error_proc; );

				LUA_GET_VALUE( luaManager, "m_iDungeonClearCount", pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount,		0 );
			}
			break;

		case SQT_DUNGEON_DAMAGE:
			{
#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
#endif //REFORM_QUEST
				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		pSubQuestTemplet->m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,	CX2Dungeon::DM_INVALID );
#endif SERV_SECRET_HELL
				//}}
#ifndef REFORM_QUEST //m_cDifficulty 제거
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				pSubQuestTemplet->m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
#endif //REFORM_QUEST		

				//{{ 2009. 3. 31  최육사	난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, "m_bUpperDifficulty",	pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty,	false, goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonDamage",	pSubQuestTemplet->m_ClearCondition.m_iDungeonDamage,	-1, goto error_proc; );
			}
			break;

		case SQT_DUNGEON_CLEAR_COUNT:
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
		case SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			{
#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
#endif //REFORM_QUEST

				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		pSubQuestTemplet->m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,	CX2Dungeon::DM_INVALID );
#endif SERV_SECRET_HELL
				//}}
#ifndef REFORM_QUEST //m_cDifficulty 제거
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				pSubQuestTemplet->m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
#endif //REFORM_QUEST			

				//{{ 2009. 3. 31  최육사	난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, "m_bUpperDifficulty",	pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty,	false, goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearCount",	pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(				luaManager, "m_iUseTitleID",		pSubQuestTemplet->m_ClearCondition.m_iUseTitleID,		-1);
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonRank",		pSubQuestTemplet->m_ClearCondition.m_eDungeonRank,	CX2DungeonRoom::RANK_TYPE,		CX2DungeonRoom::RT_NONE );
#endif SERV_EVENT_TITLE_SUBQUEST
			}
			break;

		case SQT_PVP_PLAY:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",	pSubQuestTemplet->m_ClearCondition.m_ePVPType,		CX2PVPRoom::PVP_GAME_TYPE,		CX2PVPRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPPlay",	pSubQuestTemplet->m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
			}
			break;

		case SQT_PVP_WIN:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",	pSubQuestTemplet->m_ClearCondition.m_ePVPType,		CX2PVPRoom::PVP_GAME_TYPE,		CX2PVPRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPWin",	pSubQuestTemplet->m_ClearCondition.m_iPVPWin,		-1, goto error_proc; );
			}
			break;

		case SQT_PVP_KILL:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",	pSubQuestTemplet->m_ClearCondition.m_ePVPType,		CX2PVPRoom::PVP_GAME_TYPE,		CX2PVPRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPKill",	pSubQuestTemplet->m_ClearCondition.m_iPVPKill,		-1, goto error_proc; );
#ifdef SERV_NEW_PVP_QUEST
				LUA_GET_VALUE_RETURN_ENUM( luaManager, "m_ePvpChannelClass", pSubQuestTemplet->m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PCC_NONE, goto error_proc; );
#endif SERV_NEW_PVP_QUEST
			}
			break;

			//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		case SQT_WITH_DIF_SERV_USER:
			{
#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}
#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
#endif //REFORM_QUEST

				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		pSubQuestTemplet->m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,	CX2Dungeon::DM_INVALID );				
#endif SERV_SECRET_HELL
				//}}
#ifndef REFORM_QUEST //m_cDifficulty 제거
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				pSubQuestTemplet->m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
#endif //REFORM_QUEST		

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, "m_bUpperDifficulty",	pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearCount",	pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
			}
			break;
#endif SERV_INTEGRATION
			//}}

			//{{ 2010. 10. 25	최육사	퀘스트 조건 추가
			//////////////////////////////////////////////////////////////////////////
#ifdef SERV_QUEST_CLEAR_EXPAND
		case SQT_ITEM_ENCHANT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iEnchantLevel",		pSubQuestTemplet->m_ClearCondition.m_iEnchantLevel,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, "m_iCollectionItemID",	pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID,	0 );
			}
			break;

		case SQT_ITEM_SOCKET:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iSocketCount",		pSubQuestTemplet->m_ClearCondition.m_iSocketCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, "m_iCollectionItemID",	pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID,	0 );
			}
			break;

		case SQT_ITEM_ATTRIB:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eAttribCountType",	pSubQuestTemplet->m_ClearCondition.m_eAttribCountType,	CX2EnchantItem::ATTRIB_COUNT_TYPE,	CX2EnchantItem::ACT_NONE,	goto error_proc; );
				LUA_GET_VALUE(				luaManager, "m_iCollectionItemID",	pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID,	0 );
				int iAttribEnchant = 0;
				LUA_GET_VALUE(				luaManager, "m_eAttribEnchant0",	iAttribEnchant,											0 );
				pSubQuestTemplet->m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant0 = static_cast<char>(iAttribEnchant);
				LUA_GET_VALUE(				luaManager, "m_eAttribEnchant1",	iAttribEnchant,											0 );
				pSubQuestTemplet->m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant1 = static_cast<char>(iAttribEnchant);
				LUA_GET_VALUE(				luaManager, "m_eAttribEnchant2",	iAttribEnchant,											0 );
				pSubQuestTemplet->m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant2 = static_cast<char>(iAttribEnchant);				
			}
			break;

		case SQT_ITEM_RESOLVE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iResolveCount",		pSubQuestTemplet->m_ClearCondition.m_iResolveCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, "m_iCollectionItemID",	pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID,	0 );
			}
			break;

		case SQT_ITEM_EQUIP_DUNGEON_CLEAR:
			{
#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}

#else
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );

#endif //REFORM_QUEST

				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		pSubQuestTemplet->m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,	CX2Dungeon::DM_INVALID );				
#endif SERV_SECRET_HELL
				//}}
#ifndef REFORM_QUEST //m_cDifficulty 제거
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				pSubQuestTemplet->m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
#endif //REFORM_QUEST			

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, "m_bUpperDifficulty",	pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearCount",	pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemID",	pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID,	0,		goto error_proc; );
			}
			break;

		case SQT_USE_SKILL_POINT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iSpUseCount",		pSubQuestTemplet->m_ClearCondition.m_iSpUseCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, "m_iSkillID",			pSubQuestTemplet->m_ClearCondition.m_iSkillID,			0 );
			}
			break;
#endif SERV_QUEST_CLEAR_EXPAND
			//////////////////////////////////////////////////////////////////////////
			//}}
#ifdef SERV_NEW_PVP_QUEST
		case SQT_PVP_NPC_HUNT:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",			pSubQuestTemplet->m_ClearCondition.m_ePVPType,  CX2PVPRoom::PVP_GAME_TYPE,  CX2PVPRoom::PGT_TEAM, goto error_proc; );

#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
				LUA_GET_VALUE(		luaManager, "m_iPVPWin",					pSubQuestTemplet->m_ClearCondition.m_iPVPWin,		-1 );
				LUA_GET_VALUE(		luaManager, "m_iPVPKill",					pSubQuestTemplet->m_ClearCondition.m_iPVPKill,		-1 );
				LUA_GET_VALUE(		luaManager, "m_iPVPPlay",					pSubQuestTemplet->m_ClearCondition.m_iPVPPlay,		-1 );
#else //SERV_PVP_NPC_HUNT_QUEST_EXTEND
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPWin",			pSubQuestTemplet->m_ClearCondition.m_iPVPWin,  -1, goto error_proc; );
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND

				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePvpChannelClass",	pSubQuestTemplet->m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PCC_NONE, goto error_proc; );
			}
			break;
#endif SERV_NEW_PVP_QUEST
			//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		case SQT_VISIT_VILLAGE:
			{
	#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eVillageID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.insert( buf );
						index++;
					}
					luaManager.EndTable();
				}
	#else
				LUA_GET_VALUE_RETURN(		luaManager, "m_eVillageID",		pSubQuestTemplet->m_ClearCondition.m_iVillageMapID,		0, goto error_proc; );
	#endif //REFORM_QUEST
			}
			break;

		case SQT_VISIT_FIELD:
			{
	#ifdef REFORM_QUEST
				if( luaManager.BeginTable( "m_eFieldID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}
					luaManager.EndTable();
				}
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(	luaManager, "m_iUseTitleID",					pSubQuestTemplet->m_ClearCondition.m_iUseTitleID,		-1);
#endif SERV_EVENT_TITLE_SUBQUEST
	#else
				LUA_GET_VALUE_RETURN(		luaManager, "m_eFieldID",			pSubQuestTemplet->m_ClearCondition.m_iBattleFieldID,	0, goto error_proc; );
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(	luaManager, "m_iUseTitleID",					pSubQuestTemplet->m_ClearCondition.m_iUseTitleID,		-1);
#endif SERV_EVENT_TITLE_SUBQUEST
	#endif //REFORM_QUEST
			}
			break;
	#ifdef REFORM_QUEST
		case SQT_VISIT_DUNGEON:
			{
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					luaManager.EndTable();
				}
			}
			break;
		case SQT_FIND_NPC:
			{
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				if( luaManager.BeginTable( "m_eFieldID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_iFindNPCID",	pSubQuestTemplet->m_ClearCondition.m_iFindNPCID, CX2UnitManager::NPC_UNIT_ID, CX2UnitManager::NUI_NONE, goto error_proc; );
			}
			break;
	#endif //REFORM_QUEST
#endif SERV_ENTER_FIELD_QUEST_CLEAR
#ifdef SERV_POINT_COUNT_SYSTEM
		case SQT_POINT_COUNT:
			{
				///여기에 서브 퀘스트 조건 받아오면 된다.
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPointCount", pSubQuestTemplet->m_ClearCondition.m_iPointCount, 0, goto error_proc; );
			}
			break;
#endif SERV_POINT_COUNT_SYSTEM	
#ifdef PVP_SEASON2
		case SQT_PVP_PLAY_ARRANGE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPPlay",			pSubQuestTemplet->m_ClearCondition.m_iPVPPlay,	0, goto error_proc; );
			} break;
#endif
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		case SQT_CHAR_LEVEL_UP:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iCharLevel",		pSubQuestTemplet->m_ClearCondition.m_iCharLevel, 0, goto error_proc; );
			} break;
#endif //SERV_ACCOUNT_MISSION_SYSTEM
			//}}

#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
		case SQT_PVP_HERO_NPC_KILL:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",			pSubQuestTemplet->m_ClearCondition.m_ePVPType,			CX2PVPRoom::PVP_GAME_TYPE,			CX2PVPRoom::PGT_TEAM,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPKill",			pSubQuestTemplet->m_ClearCondition.m_iPVPKill,			-1,																goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePvpChannelClass",	pSubQuestTemplet->m_ClearCondition.m_ePvpChannelClass,	KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PCC_NONE,	goto error_proc; );
			}
			break;
		case SQT_PVP_HERO_NPC_PLAY:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",			pSubQuestTemplet->m_ClearCondition.m_ePVPType,			CX2PVPRoom::PVP_GAME_TYPE,			CX2PVPRoom::PGT_TEAM,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPPlay",			pSubQuestTemplet->m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePvpChannelClass",	pSubQuestTemplet->m_ClearCondition.m_ePvpChannelClass,	KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PCC_NONE,	goto error_proc; );
			}
			break;
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND
#ifdef SERV_SUB_QUEST_USE_ITEM
		case SQT_ITEM_USE:
			{
				if( luaManager.BeginTable( "m_eDungeonID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				if( luaManager.BeginTable( "m_eFieldID" ) == true )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}

				if( luaManager.BeginTable( "m_listUseItemID" ) == true )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_vecUseItemID.push_back(buf);
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN(		luaManager, "m_iUseItemCnt",	pSubQuestTemplet->m_ClearCondition.m_iUseItemNum,	0, goto error_proc; );
			}
			break;
#endif SERV_SUB_QUEST_USE_ITEM
#ifdef SERV_SKILL_USE_SUBQUEST
		case SQT_SKILL_USE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iUseSkillCount",	pSubQuestTemplet->m_ClearCondition.m_iUseSkillCount,		-1, goto error_proc; );

				if( luaManager.BeginTable( "m_vecSkillID" ) == true )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setSkillID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
			}
			break;
#endif SERV_SKILL_USE_SUBQUEST
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
		case SQT_LEARN_NEW_SKILL:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iLearnNewSkillCount",	pSubQuestTemplet->m_ClearCondition.m_iLearnNewSkillCount,		-1, goto error_proc; );

				if( luaManager.BeginTable( "m_vecSkillID" ) == true )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							pSubQuestTemplet->m_ClearCondition.m_setSkillID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
			}
			break;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
		default:
			{
				luaManager.EndTable();
				return false;
			}
		}

		luaManager.EndTable();
		return true;
	}

	return false;


error_proc:
	luaManager.EndTable();
	return false;
}








void CX2QuestManager::ClearUnitQuest()
{

	QuestInst* pQuestInst = NULL;
	map<int,QuestInst*>::iterator iter3;

	iter3 = m_mapUnitQuest.begin();
	for( iter3 = m_mapUnitQuest.begin(); iter3 != m_mapUnitQuest.end(); iter3++ )
	{
		pQuestInst = iter3->second;
		SAFE_DELETE( pQuestInst );
	}
	m_mapUnitQuest.clear();

	m_mapCompleteQuest.clear();

}

bool CX2QuestManager::RemoveUnitQuest( int questID, bool bComplete )
{
	map<int,QuestInst*>::iterator it = m_mapUnitQuest.find( questID );
	if( m_mapUnitQuest.end() != it )
	{
		if( it->first == questID )
		{
			QuestInst* pQuestInst = it->second;
			SAFE_DELETE( pQuestInst );
			m_mapUnitQuest.erase( it );
		}

		if( true == bComplete )
			m_mapCompleteQuest[questID] = questID;

		return true;
	}

	return false;
}

bool CX2QuestManager::QuestInst::IsComplete() const
{
	for( UINT i=0; i<m_vecSubQuestInst.size(); i++ )
	{
		const SubQuestInst& subQuestInst = m_vecSubQuestInst[i];
		if( false == subQuestInst.IsComplete() )
			return false;
	}

	return true;
}
#ifdef REFORM_QUEST //임시
bool CX2QuestManager::QuestInst::IsCompleteVisitQuest( int iSubQuestID_ ) const
{
	bool bIsComplete = false;
	for( UINT i=0; i<m_vecSubQuestInst.size(); ++i )
	{
		if( true == bIsComplete )
		{
			//해당 퀘스트가 마지막 퀘스트가 아니라면 무조건 false 리턴
			bIsComplete = false;
			break;
		}

		if( iSubQuestID_ == m_vecSubQuestInst[i].m_iID )
		{
			const SubQuestInst& subQuestInst = m_vecSubQuestInst[i];
			if( true == subQuestInst.IsComplete() )
			{
				bIsComplete = true;
			}				
		}
	}

	return bIsComplete;
}
#endif //REFORM_QUEST

#ifdef SERV_EPIC_QUEST_HIDE_QUESTMARK_WHEN_SUBQUEST_COMPLETED
bool CX2QuestManager::QuestInst::HasNotCompletedSubQuest( int iSubQuestID ) const
{
	for ( int i = 0; i < (int)m_vecSubQuestInst.size(); i++ )
	{
		const CX2QuestManager::SubQuestInst& subQuestInst = m_vecSubQuestInst[i];
		if( subQuestInst.m_iID != iSubQuestID )
			continue;

		if( subQuestInst.IsComplete() == true )
			return false;
		else
			return true;
	}

	return false;
}
#endif SERV_EPIC_QUEST_HIDE_QUESTMARK_WHEN_SUBQUEST_COMPLETED

bool CX2QuestManager::SubQuestInst::IsComplete() const
{
	if( NULL == g_pData->GetQuestManager() )
		return false;

	const SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( m_iID );
	if( pSubQuestTemplet != NULL && 
		(SQT_ITEM_COLLECTION == pSubQuestTemplet->m_eClearType || SQT_QUEST_ITEM_COLLECTION == pSubQuestTemplet->m_eClearType ) )
	{
		// 인벤에서 아이템 개수를 헤아려 보자~ // 장착 장비는 빼고!
		const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
		int nCount = kInventory.GetNumItemByTID( pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID, true );

		if( nCount < pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum )
		{
			return false;
		}
	}
#ifdef SERV_POINT_COUNT_SYSTEM
	else if ( g_pInstanceData != NULL && pSubQuestTemplet != NULL && SQT_POINT_COUNT == pSubQuestTemplet->m_eClearType )
	{
#ifdef SERV_EVENT_SONGKRAN_FESTIVAL_DUNGEON
		if ( g_pInstanceData->GetSongkranEventWaterPoint() <  pSubQuestTemplet->m_ClearCondition.m_iPointCount )
		{
			return false;
		}
#endif SERV_EVENT_SONGKRAN_FESTIVAL_DUNGEON
	}
#endif //SERV_POINT_COUNT_SYSTEM
	else if( false == m_bIsSuccess )
	{
		return false;
	}

	return true;
}

wstring CX2QuestManager::LineBreakSubQuestString( int& nLineCount, wstring& wstrText, CKTDGFontManager::CUKFont* pFont, int iWidth )
{
	if( NULL == pFont )
		return wstrText;

	wstring wstrLineBreaked = L"";
	const wstring wstrNextLine = L"\n";

	nLineCount = 1;
	int iUniCharSize = 0;	
	int iStringSize = 0;	

	for( UINT i=0; i<wstrText.length(); i++ )
	{
		WCHAR wChar = wstrText[i];
		wstrLineBreaked += wChar;
		iUniCharSize = pFont->GetWidth( wChar );
		iStringSize += iUniCharSize;

		if( wChar == L'\n' )
		{
			iStringSize = 0;
			nLineCount++;
		}

		if( iStringSize >= iWidth )
		{
			wstrLineBreaked += L"\n";
			iStringSize = 0;
			nLineCount++;
		}
	}

	return wstrLineBreaked;
}

// @bShowCount 수집 퀘스트 등인 경우에 제일 끝에 수량 표시를 할 것 인지 말것인지
wstring CX2QuestManager::MakeSubQuestString( bool bAuto, const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet,
											const CX2QuestManager::QuestInst* pQuestInst, bool bShowCount /*= true*/ )
{
#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
	 	return L"";
#endif

	if( pSubQuestTemplet == NULL )
		return L"";

	const CX2QuestManager::ClearCondition& clearCond = pSubQuestTemplet->m_ClearCondition;
	wstringstream wstrmQuestCond;

	switch( pSubQuestTemplet->m_eClearType )
	{
		//{{ 2009. 8. 4  최육사		NPCID그룹핑
	case CX2QuestManager::SQT_NPC_HUNT:
		{
			// NPCID가 2개 이상이면 Auto출력 안함!
			if( clearCond.m_setKillNPCID.size() > 1 )
			{
				bAuto = false;
			}
#ifndef REFORM_QUEST //던전ID관련 자동완성에 대한 기획이 완료되지 않음, m_cDifficulty 제거
			const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( *clearCond.m_setKillNPCID.begin() );

#ifdef FIX_QUEST_DESC_CRASH
			if ( NULL == pNPCTemplet )	/// kimhc /// Error 보고를 위해 크래시 방어 안함
			{
				ErrorLogMsg( XEM_ERROR142, GET_REPLACED_STRING((STR_ID_18197, "ii", pSubQuestTemplet->m_iID, -1)) );
			}
#endif //FIX_QUEST_DESC_CRASH

			if( true == bAuto )
			{
				wstrmQuestCond << pNPCTemplet->m_Name ;
				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( clearCond.m_eDungeonID );
				if( NULL != pDungeonData )
				{
					wstrmQuestCond << L"(" << pDungeonData->m_DungeonName << L" ";

					//{{ 2009. 4. 1  최육사		특정 난이도 이상
					if( clearCond.m_bUpperDifficulty )
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_445 ) << L"')";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_446 ) << L"')";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"')";
							} break;

						default:
							{
								wstrmQuestCond << L")";
							} break;
						}
					}
					else
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_385 ) << L"')";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_386 ) << L"')";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"')";
							} break;

						default:
							{
								wstrmQuestCond << L")";
							} break;
						}
					}
					//}}
				}		

				if( NULL != pQuestInst )
				{
					int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
					wstrmQuestCond << L" " << iClearData << L"/" << clearCond.m_iKillNum /*<< L"마리 퇴치"*/;
				}
				else
				{
					wstrmQuestCond << L" " << L"0/" << clearCond.m_iKillNum /*<< L"마리 퇴치"*/;
				}
			}
			else
#endif //REFORM_QUEST	
			{
				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if ( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iKillNum << std::endl;
					}
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iKillNum << std::endl;
				}
			}

		} break;
		//}}

	case CX2QuestManager::SQT_ITEM_COLLECTION:
	case CX2QuestManager::SQT_QUEST_ITEM_COLLECTION:
		{
			// NPCID가 1개가 아니면 Auto출력 안함!
			if( clearCond.m_setKillNPCID.size() > 1 )
			{
				bAuto = false;
			}

			int nItemCount = 0;
			if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				nItemCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( clearCond.m_iCollectionItemID, true );
			}

#ifndef REFORM_QUEST //던전ID관련 자동완성에 대한 기획이 완료되지 않음, m_cDifficulty 제거
			if( true == bAuto )
			{
				//단단한 밀랍 수집 3/10개 수집 (포이즌 허니 하우스, 어둠의 숲 '모든 난이도')
                const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( clearCond.m_iCollectionItemID );

#ifdef FIX_QUEST_DESC_CRASH
				if ( NULL == pItemTemplet )	/// kimhc /// Error 보고를 위해 크래시 방어 안함
				{
					ErrorLogMsg( XEM_ERROR142, GET_REPLACED_STRING((STR_ID_18197, "ii", pSubQuestTemplet->m_iID, clearCond.m_iCollectionItemID)) );
				}
#endif	// FIX_QUEST_DESC_CRASH

                wstrmQuestCond << pItemTemplet->GetName();

				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( clearCond.m_eDungeonID );
				if( NULL != pDungeonData )
				{
					CX2UnitManager::NPCUnitTemplet* pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( *clearCond.m_setKillNPCID.begin() );
					if( NULL != pNPCTemplet )
					{
						wstrmQuestCond << L"(" << pNPCTemplet->m_Name << L", " << pDungeonData->m_DungeonName << L" ";

						//{{ 2009. 4. 1  최육사
						if( clearCond.m_bUpperDifficulty )
						{
							switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
							{
							case CX2Dungeon::DL_NORMAL:
								{
									wstrmQuestCond << L"'" << GET_STRING( STR_ID_445 ) << L"')";
								} break;

							case CX2Dungeon::DL_HARD:
								{
									wstrmQuestCond << L"'" << GET_STRING( STR_ID_446 ) << L"')";
								} break;

							case CX2Dungeon::DL_EXPERT:
								{
									wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"')";
								} break;

							default:
								{
									wstrmQuestCond << L")";
								} break;
							}
						}
						else
						{
							switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
							{
							case CX2Dungeon::DL_NORMAL:
								{
									wstrmQuestCond << L"'" << GET_STRING( STR_ID_385 ) << L"')";
								} break;

							case CX2Dungeon::DL_HARD:
								{
									wstrmQuestCond << L"'" << GET_STRING( STR_ID_386 ) << L"')";
								} break;

							case CX2Dungeon::DL_EXPERT:
								{
									wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"')";
								} break;

							default:
								{
									wstrmQuestCond << L")";
								} break;
							}
						}
					}				
					//}}					
				}
//{{ 09.05.29 태완 : 퀘스트 안 받았어도 현재 가진 퀘스트 템의 수량을 표시해 주도록 변경
				wstrmQuestCond << L" " << nItemCount << L"/" << clearCond.m_iCollectionItemNum /*<< L"개 수집"*/;
// 				if( NULL != pQuestInst )
// 				{
// 					wstrmQuestCond << L" " << nItemCount << L"/" << clearCond.m_iCollectionItemNum /*<< L"개 수집"*/;
// 				}
// 				else
// 				{
// 					wstrmQuestCond << L" " <<  L"0/" << clearCond.m_iCollectionItemNum /*<< L"개 수집"*/;
// 				}
//}} 09.05.29
			}
			else
#endif //REFORM_QUEST		
			{
				if( 0 == pSubQuestTemplet->m_wstrDescription.length() )
				{
					// subquest description 이 없으면 아예 화면에 안 보여준다
					wstrmQuestCond.str( L"" );
				}
				else
				{
					if( true == bShowCount )
					{
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << nItemCount << L"/" << clearCond.m_iCollectionItemNum << std::endl;
					}
					else
					{
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
					}
				}
			}

		} break;

	case CX2QuestManager::SQT_DUNGEON_TIME: 
		{
#ifndef REFORM_QUEST //던전ID관련 자동완성에 대한 기획이 완료되지 않음, m_cDifficulty 제거
			if( true == bAuto )
			{
				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( clearCond.m_eDungeonID );

#ifdef FIX_QUEST_DESC_CRASH
				if( NULL == pDungeonData )	/// kimhc /// Error 보고를 위해 크래시 방어 안함
				{
					ErrorLogMsg( XEM_ERROR142, GET_REPLACED_STRING((STR_ID_18197, "ii", pSubQuestTemplet->m_iID, clearCond.m_eDungeonID)) );
				}
				//else
#endif //FIX_QUEST_DESC_CRASH
				{
					wstrmQuestCond << pDungeonData->m_DungeonName << L" ";

					//{{ 2009. 4. 1  최육사		난이도 이상
					if( clearCond.m_bUpperDifficulty )
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_445 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_446 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;
						}
					}
					else
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_385 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_386 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;
						}
					}
					//}}				

					int iMinute = (int) ( clearCond.m_iDungeonClearTime / 60.f );
					int iSecond = (int) ( clearCond.m_iDungeonClearTime - iMinute * 60.f );

					if( 0 == iSecond )
					{
						wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_447, "i", iMinute ) ); 
					}
					else
					{
						wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_448, "ii", iMinute, iSecond ) ); 
					}
				}
			}
			else
#endif //REFORM_QUEST		
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}
		} break;

	case CX2QuestManager::SQT_DUNGEON_RANK:
		{

#ifndef REFORM_QUEST //던전ID관련 자동완성에 대한 기획이 완료되지 않음, m_cDifficulty 제거
			if( true == bAuto )
			{
				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( clearCond.m_eDungeonID );

#ifdef FIX_QUEST_DESC_CRASH		/// kimhc /// Error 보고를 위해 크래시 방어 안함
				if( NULL == pDungeonData )
				{
					ErrorLogMsg( XEM_ERROR142, GET_REPLACED_STRING((STR_ID_18197, "ii", pSubQuestTemplet->m_iID, clearCond.m_eDungeonID)) );
				}
				//else
#endif //FIX_QUEST_DESC_CRASH
				{
					wstrmQuestCond << pDungeonData->m_DungeonName << L" ";

					//{{ 2009. 4. 1  최육사		난이도 이상
					if( clearCond.m_bUpperDifficulty )
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_445 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_446 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;
						}
					}
					else
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_385 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_386 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;
						}
					}
					//}}				

					switch( clearCond.m_eDungeonRank )
					{
					case CX2DungeonRoom::RT_NONE:
						{
							wstrmQuestCond << L" " << GET_STRING( STR_ID_449 );
						} break;

					case CX2DungeonRoom::RT_F:
						{
							wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_450, "S", L"F" ) );
						} break;

					case CX2DungeonRoom::RT_E:
						{
							wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_450, "S", L"E" ) );
						} break;

					case CX2DungeonRoom::RT_D:
						{
							wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_450, "S", L"D" ) );
						} break;

					case CX2DungeonRoom::RT_C:
						{
							wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_450, "S", L"C" ) );
						} break;

					case CX2DungeonRoom::RT_B:
						{
							wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_450, "S", L"B" ) );
						} break;

					case CX2DungeonRoom::RT_A:
						{
							wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_450, "S", L"A" ) );
						} break;

					case CX2DungeonRoom::RT_S:
						{
							wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_450, "S", L"S" ) );
						} break;
					}
				}
			}
			else
#endif //REFORM_QUEST		
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;

				// 오현빈 // 특정 랭크 이상 클리어 조건에 카운트 추가
				if( clearCond.m_iDungeonClearCount > 0 )
				{
					if( NULL != pQuestInst )
					{
						const int iClearData = static_cast<int>(pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData);
						wstrmQuestCond << L" " << iClearData << L"/" << clearCond.m_iDungeonClearCount << std::endl;
					}
					else
					{
						wstrmQuestCond << L" 0/" << clearCond.m_iDungeonClearCount << std::endl;
					}
				}

			}

		} break;

	case CX2QuestManager::SQT_DUNGEON_DAMAGE:
		{

#ifndef REFORM_QUEST //던전ID관련 자동완성에 대한 기획이 완료되지 않음, m_cDifficulty 제거
			if( true == bAuto )
			{
				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( clearCond.m_eDungeonID );

#ifdef FIX_QUEST_DESC_CRASH
				if( NULL == pDungeonData )	/// kimhc /// Error 보고를 위해 크래시 방어 안함
				{
					ErrorLogMsg( XEM_ERROR142, GET_REPLACED_STRING((STR_ID_18197, "ii", pSubQuestTemplet->m_iID, clearCond.m_eDungeonID)) );
				}
				//else
#endif //FIX_QUEST_DESC_CRASH
				{
					wstrmQuestCond << pDungeonData->m_DungeonName << L" ";

					//{{ 2009. 4. 1  최육사		난이도 이상
					if( clearCond.m_bUpperDifficulty )
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_445 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_446 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;
						}
					}
					else
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_385 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_386 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;
						}
					}
					//}}

					wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_451, "i", clearCond.m_iDungeonDamage ) );
				}
			}
			else
#endif //REFORM_QUEST			
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}
		} break;

	case CX2QuestManager::SQT_DUNGEON_CLEAR_COUNT:
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	case CX2QuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
		{

#ifndef REFORM_QUEST //던전ID관련 자동완성에 대한 기획이 완료되지 않음, m_cDifficulty 제거
			if( true == bAuto )
			{
				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( clearCond.m_eDungeonID );
				if( NULL != pDungeonData )
				{

					wstrmQuestCond << pDungeonData->m_DungeonName << L" ";
					//{{ 2009. 4. 1  최육사		난이도 이상
					if( clearCond.m_bUpperDifficulty )
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_445 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_446 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;

						//{{ kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
					#ifdef SERV_SECRET_HELL
						default:
							{
								switch ( clearCond.m_eDungeonMode )
								{
								case CX2Dungeon::DM_SECRET_NORMAL:
									{
										wstrmQuestCond << L"'" << GET_STRING( STR_ID_5106 ) << L"'";
									} break;

								case CX2Dungeon::DM_SECRET_HELL:
									{
										wstrmQuestCond << L"'" << GET_STRING( STR_ID_4943 ) << L"'";
									} break;
								}
							} break;
					#endif SERV_SECRET_HELL
						//}} kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
						}
					}
					else
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_385 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_386 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;

						//{{ kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
					#ifdef SERV_SECRET_HELL
						default:
							{
								switch ( clearCond.m_eDungeonMode )
								{
								case CX2Dungeon::DM_SECRET_NORMAL:
									{
										wstrmQuestCond << L"'" << GET_STRING( STR_ID_938 ) << L"'";
									} break;

								case CX2Dungeon::DM_SECRET_HELL:
									{
										wstrmQuestCond << L"'" << GET_STRING( STR_ID_4943 ) << L"'";
									} break;
								}
							} break;
					#endif SERV_SECRET_HELL
							//}} kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
						}
					}
					//}}


					if( NULL != pQuestInst )
					{
						int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
						wstrmQuestCond << L" " << iClearData << L"/" << GET_REPLACED_STRING( ( STR_ID_452, "i", clearCond.m_iDungeonClearCount ) );
					}
					else
					{
						wstrmQuestCond << GET_REPLACED_STRING( ( STR_ID_452, "i", clearCond.m_iDungeonClearCount ) );
					}
				}
			}
			else
#endif //REFORM_QUEST			
			{
				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if ( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iDungeonClearCount;
					}
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iDungeonClearCount << std::endl;
				}
			}
		} break;

#ifdef SERV_INTEGRATION
//{{ oasis907 : 김상윤 [2010.5.18] // 대전 던전 서버군 통합
	case CX2QuestManager::SQT_WITH_DIF_SERV_USER:
		{
#ifndef REFORM_QUEST //던전ID관련 자동완성에 대한 기획이 완료되지 않음, m_cDifficulty 제거
			if( true == bAuto )
			{
				if( clearCond.m_eDungeonID != SEnum::DI_NONE )
				{
					wstrmQuestCond << GET_STRING( STR_ID_5123 ) << L" ";
					const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( clearCond.m_eDungeonID );
					wstrmQuestCond << pDungeonData->m_DungeonName << L" ";

					//{{ 2009. 4. 1  최육사		난이도 이상
					if( clearCond.m_bUpperDifficulty )
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_445 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_446 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;

							//{{ kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
#ifdef SERV_SECRET_HELL
						default:
							{
								switch ( clearCond.m_eDungeonMode )
								{
								case CX2Dungeon::DM_SECRET_NORMAL:
									{
										wstrmQuestCond << L"'" << GET_STRING( STR_ID_5106 ) << L"'";
									} break;

								case CX2Dungeon::DM_SECRET_HELL:
									{
										wstrmQuestCond << L"'" << GET_STRING( STR_ID_4943 ) << L"'";
									} break;
								}
							} break;
#endif SERV_SECRET_HELL
							//}} kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
						}
					}
					else
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_385 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_386 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmQuestCond << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;

							//{{ kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
#ifdef SERV_SECRET_HELL
						default:
							{
								switch ( clearCond.m_eDungeonMode )
								{
								case CX2Dungeon::DM_SECRET_NORMAL:
									{
										wstrmQuestCond << L"'" << GET_STRING( STR_ID_938 ) << L"'";
									} break;

								case CX2Dungeon::DM_SECRET_HELL:
									{
										wstrmQuestCond << L"'" << GET_STRING( STR_ID_4943 ) << L"'";
									} break;
								}
							} break;
#endif SERV_SECRET_HELL
							//}} kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
						}
					}
					//}}

					if( NULL != pQuestInst )
					{
						int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
						wstrmQuestCond << L" " << iClearData << L"/" << GET_REPLACED_STRING( ( STR_ID_452, "i", clearCond.m_iDungeonClearCount ) );
					}
					else
					{
						wstrmQuestCond << GET_REPLACED_STRING( ( STR_ID_452, "i", clearCond.m_iDungeonClearCount ) );
					}
				}
				else
				{
					wstrmQuestCond << GET_STRING( STR_ID_5124 ) << L" ";

					if( NULL != pQuestInst )
					{
						int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
						wstrmQuestCond << L" " << iClearData << L"/" << GET_REPLACED_STRING( ( STR_ID_452, "i", clearCond.m_iDungeonClearCount ) );
					}
					else
					{
						wstrmQuestCond << GET_REPLACED_STRING( ( STR_ID_452, "i", clearCond.m_iDungeonClearCount ) );
					}
				}

			}
			else
#endif //REFORM_QUEST
			{
				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iDungeonClearCount;
					}
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iDungeonClearCount << std::endl;
				}
			}
		} break;
//}} oasis907 : 김상윤 [2010.5.18] // 대전 던전 서버군 통합
#endif SERV_INTEGRATION

	case CX2QuestManager::SQT_NPC_TALK:
		{
			if( true == bAuto )
			{
				const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = NULL;
				if( NULL != g_pData && NULL != g_pData->GetUnitManager() )
				{
					pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( clearCond.m_eTalkNPCID );
				}
				if( NULL != pNPCTemplet )
				{
					wstrmQuestCond << pNPCTemplet->m_Description << L" ";
                    wstrmQuestCond << GET_REPLACED_STRING( ( STR_ID_455, "LI", pNPCTemplet->m_Name,
                                                             KHanSungPostWordUnicodeWrapper( (WCHAR*)pNPCTemplet->m_Name.c_str(), STR_ID_453, STR_ID_454 ) ) );
				}
#ifdef FIX_QUEST_DESC_CRASH
				else
				{
					ErrorLogMsg( XEM_ERROR142, GET_REPLACED_STRING((STR_ID_18197, "ii", pSubQuestTemplet->m_iID, clearCond.m_iCollectionItemID)) );
				}
#endif	// FIX_QUEST_DESC_CRASH
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}

			
		} break;

	case CX2QuestManager::SQT_PVP_PLAY:
		{
			if( true == bAuto )
			{
				switch( clearCond.m_ePVPType )
				{
				case CX2PVPRoom::PGT_TEAM:
					{
						wstrmQuestCond << GET_STRING( STR_ID_456 );
					} break;

				case CX2PVPRoom::PGT_TEAM_DEATH:
					{
						wstrmQuestCond << GET_STRING( STR_ID_457 );
					} break;

				case CX2PVPRoom::PGT_SURVIVAL:
					{
						wstrmQuestCond << GET_STRING( STR_ID_458 );
					} break;

				default:
					{
						wstrmQuestCond << GET_STRING( STR_ID_459 );
					} break;
				}

				if( NULL != pQuestInst )
				{
					int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
					wstrmQuestCond << L" " << iClearData << L"/" << GET_REPLACED_STRING( ( STR_ID_460, "i", clearCond.m_iPVPPlay ) );
				}
				else
				{
					wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_460, "i", clearCond.m_iPVPPlay ) );
				}
			}
			else
			{
				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iPVPPlay << std::endl;
					}
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iPVPPlay << std::endl;
				}
			}

		} break;

	case CX2QuestManager::SQT_PVP_WIN:
		{
			if( true == bAuto )
			{
				switch( clearCond.m_ePVPType )
				{
				case CX2PVPRoom::PGT_TEAM:
					{
						wstrmQuestCond << GET_STRING( STR_ID_456 );
					} break;

				case CX2PVPRoom::PGT_TEAM_DEATH:
					{
						wstrmQuestCond << GET_STRING( STR_ID_457 );
					} break;

				case CX2PVPRoom::PGT_SURVIVAL:
					{
						wstrmQuestCond << GET_STRING( STR_ID_458 );
					} break;

				default:
					{
						wstrmQuestCond << GET_STRING( STR_ID_459 );
					} break;

				}

				if( NULL != pQuestInst )
				{
					int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
					wstrmQuestCond << L" " << iClearData << L"/" << GET_REPLACED_STRING( ( STR_ID_461, "i", clearCond.m_iPVPWin ) );
				}
				else
				{
					wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_461, "i", clearCond.m_iPVPWin ) );
				}
			}
			else
			{
				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iPVPWin << std::endl;
					}
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iPVPWin << std::endl;
				}
			}

		} break;

	case CX2QuestManager::SQT_PVP_KILL:
		{
			if( true == bAuto )
			{
				switch( clearCond.m_ePVPType )
				{
				case CX2PVPRoom::PGT_TEAM:
					{
						wstrmQuestCond << GET_STRING( STR_ID_456 );
					} break;

				case CX2PVPRoom::PGT_TEAM_DEATH:
					{
						wstrmQuestCond << GET_STRING( STR_ID_457 );
					} break;

				case CX2PVPRoom::PGT_SURVIVAL:
					{
						wstrmQuestCond << GET_STRING( STR_ID_458 );
					} break;

				default:
					{
						wstrmQuestCond << GET_STRING( STR_ID_459 );
					} break;
				}

				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << L" " << iClearData << L"/" << GET_REPLACED_STRING( ( STR_ID_462, "i", clearCond.m_iPVPKill ) );
					}
				}
				else
				{
					wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_462, "i", clearCond.m_iPVPKill ) );
				}
			}
			else
			{
				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iPVPKill << std::endl;
					}
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iPVPKill << std::endl;
				}
			}
		} break;
#ifdef SERV_QUEST_CLEAR_EXPAND
	//{{ oasis907 : 김상윤 [2011.1.26] // 추가 퀘스트 클리어 타입 스트링
	case CX2QuestManager::SQT_ITEM_ENCHANT:
		{
//#ifndef FIX_QUEST_DESC_CRASH
//			//사용하지 않는 객체이기 떄문에 제거 하겠습니다.
//			CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//			//int nItemCount = kInventory.GetNumItemByTID( clearCond.m_iCollectionItemID, true );
//#endif //FIX_QUEST_DESC_CRASH

			if( true == bAuto )
			{
				if(clearCond.m_iCollectionItemID == 0)
				{
					wstrmQuestCond << GET_STRING( STR_ID_4808 );
				}
				else
				{
					if( NULL != g_pData && NULL != g_pData->GetItemManager() )
					{
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( clearCond.m_iCollectionItemID );
						if( NULL != pItemTemplet )
						{
							wstrmQuestCond << pItemTemplet->GetName();
						}
					}
				}
				wstrmQuestCond << L" " << GET_REPLACED_STRING( ( STR_ID_10536, "i", clearCond.m_iEnchantLevel ) );
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}

		} break;
	case CX2QuestManager::SQT_ITEM_SOCKET:
		{
//#ifndef FIX_QUEST_DESC_CRASH
//			//사용하지 않는 객체이기 떄문에 제거 하겠습니다.
//			CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//#endif //FIX_QUEST_DESC_CRASH

			if( true == bAuto )
			{
				if(clearCond.m_iCollectionItemID == 0)
				{
					wstrmQuestCond << GET_STRING( STR_ID_4808 );
				}
				else
				{
					if( NULL != g_pData && NULL != g_pData->GetItemManager() )
					{
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( clearCond.m_iCollectionItemID );
						if( NULL != pItemTemplet )
						{
							wstrmQuestCond << pItemTemplet->GetName();
						}
					}
				}

				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << L" " << GET_STRING( STR_ID_10537 ) << L" " << iClearData << L"/" << clearCond.m_iSocketCount << std::endl;;
					}
				}
				else
				{
					wstrmQuestCond << L" " << GET_STRING( STR_ID_10537 );
				}
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}

		} break;
	case CX2QuestManager::SQT_ITEM_ATTRIB:
		{
//#ifndef FIX_QUEST_DESC_CRASH
//			//사용하지 않는 객체이기 떄문에 제거 하겠습니다.
//			CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//#endif //FIX_QUEST_DESC_CRASH
			if( true == bAuto )
			{
				if(clearCond.m_iCollectionItemID == 0)
				{
					wstrmQuestCond << GET_STRING( STR_ID_4808 );
				}
				else
				{
					if( NULL != g_pData && NULL != g_pData->GetItemManager() )
					{
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( clearCond.m_iCollectionItemID );
						if( NULL != pItemTemplet )
						{
							wstrmQuestCond << pItemTemplet->GetName();
						}
					}
				}

				wstring wstrAttribCountType = L"";
				switch(clearCond.m_eAttribCountType)
				{	
				case CX2EnchantItem::ACT_NONE:
					break;
				case CX2EnchantItem::ACT_SINGLE:
					wstrAttribCountType = GET_STRING( STR_ID_10538 );
					break;
				case CX2EnchantItem::ACT_DUAL:
					wstrAttribCountType = GET_STRING( STR_ID_10539 );
					break;
				case CX2EnchantItem::ACT_TRIPLE:
					wstrAttribCountType = GET_STRING( STR_ID_10540 );
					break;
				}
				wstrmQuestCond << GET_REPLACED_STRING( ( STR_ID_10541, "L",  wstrAttribCountType) );
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}

		} break;
	case CX2QuestManager::SQT_ITEM_RESOLVE:
		{
//#ifndef FIX_QUEST_DESC_CRASH
//			//사용하지 않는 객체이기 떄문에 제거 하겠습니다.
//			CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//#endif //FIX_QUEST_DESC_CRASH
			if( true == bAuto )
			{
				if(clearCond.m_iCollectionItemID == 0)
				{
					wstrmQuestCond << GET_STRING( STR_ID_4808 );
				}
				else
				{
					if( NULL != g_pData && NULL != g_pData->GetItemManager() )
					{
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( clearCond.m_iCollectionItemID );
						if( NULL != pItemTemplet )
						{
							wstrmQuestCond << pItemTemplet->GetName();
						}
					}
				}

				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << L" " << GET_STRING( STR_ID_10544 ) << L" " << iClearData << L"/" << clearCond.m_iResolveCount << std::endl;
					}
				}
				else
				{
					wstrmQuestCond << L" " << GET_STRING( STR_ID_10544 );
				}
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}

		} break;
	case CX2QuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
		{
#ifndef REFORM_QUEST //던전ID관련 자동완성에 대한 기획이 완료되지 않음, m_cDifficulty 제거
//#ifndef FIX_QUEST_DESC_CRASH
//			//사용하지 않는 객체이기 떄문에 제거 하겠습니다.
//			CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//#endif //FIX_QUEST_DESC_CRASH

			if( true == bAuto )
			{
				if(clearCond.m_iCollectionItemID == 0)
				{
					wstrmQuestCond << GET_STRING( STR_ID_4808 );
				}
				else
				{
					wstrmQuestCond << g_pData->GetItemManager()->GetItemTemplet( clearCond.m_iCollectionItemID )->GetName();
				}

				wstringstream wstrmDungeonWhere;
				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( clearCond.m_eDungeonID );
				
				if( NULL != pDungeonData )
				{
					wstrmDungeonWhere << pDungeonData->m_DungeonName << L" ";

					//{{ 2009. 4. 1  최육사		난이도 이상
					if( clearCond.m_bUpperDifficulty )
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_445 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_446 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;

							//{{ kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
#ifdef SERV_SECRET_HELL
						default:
							{
								switch ( clearCond.m_eDungeonMode )
								{
								case CX2Dungeon::DM_SECRET_NORMAL:
									{
										wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_5106 ) << L"'";
									} break;

								case CX2Dungeon::DM_SECRET_HELL:
									{
										wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_4943 ) << L"'";
									} break;
								}
							} break;
#endif SERV_SECRET_HELL
							//}} kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
						}
					}
					else
					{
						switch( (CX2Dungeon::DIFFICULTY_LEVEL) clearCond.m_cDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							{
								wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_385 ) << L"'";
							} break;

						case CX2Dungeon::DL_HARD:
							{
								wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_386 ) << L"'";
							} break;

						case CX2Dungeon::DL_EXPERT:
							{
								wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_388 ) << L"'";
							} break;

							//{{ kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
#ifdef SERV_SECRET_HELL
						default:
							{
								switch ( clearCond.m_eDungeonMode )
								{
								case CX2Dungeon::DM_SECRET_NORMAL:
									{
										wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_938 ) << L"'";
									} break;

								case CX2Dungeon::DM_SECRET_HELL:
									{
										wstrmDungeonWhere << L"'" << GET_STRING( STR_ID_4943 ) << L"'";
									} break;
								}
							} break;
#endif SERV_SECRET_HELL
							//}} kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 퀘스트)
						}
					}
				}
				//}}
				wstring wstrDungeonWhere = wstrmDungeonWhere.str();

				if( NULL != pQuestInst )
				{
					int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;

					wstrmQuestCond << GET_REPLACED_STRING( ( STR_ID_10545, "L", wstrDungeonWhere ) ) << L" " << iClearData << L"/" << clearCond.m_iDungeonClearCount << std::endl;
				}
				else
				{
					wstrmQuestCond << GET_REPLACED_STRING( ( STR_ID_10545, "L", wstrDungeonWhere ) ) << L" " << clearCond.m_iDungeonClearCount << std::endl;
				}
			}
			else

#endif //REFORM_QUEST
			{
				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iDungeonClearCount;
					}
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
				}
			}
		} break;
	case CX2QuestManager::SQT_USE_SKILL_POINT:
		{
//#ifndef FIX_QUEST_DESC_CRASH
//			//사용하지 않는 객체이기 떄문에 제거 하겠습니다.
//			CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//#endif //FIX_QUEST_DESC_CRASH
			if( true == bAuto )
			{
				if(clearCond.m_iSkillID == 0)
				{
					wstrmQuestCond << GET_STRING( STR_ID_9668 );
				}
				else
				{
					if( NULL != g_pData && NULL != g_pData->GetSkillTree() )
					{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
						const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>(clearCond.m_iSkillID) );
	#else // UPGRADE_SKILL_SYSTEM_2013
						const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>(clearCond.m_iSkillID), 1);
	#endif // UPGRADE_SKILL_SYSTEM_2013
						if( NULL != pSkillTemplet )
						{
							wstring wstrSkillName = pSkillTemplet->m_wstrName;
							wstrmQuestCond << wstrSkillName;
						}
					}
				}

				if( NULL != pQuestInst )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
					if( NULL != pSubQuestInst )
					{
						int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
						wstrmQuestCond << GET_STRING( STR_ID_10546 ) << L" " << iClearData << L"/" << clearCond.m_iSpUseCount << std::endl;
					}
				}
				else
				{
					wstrmQuestCond << GET_STRING( STR_ID_10546 );
				}
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}

		} break;
		//}} oasis907 : 김상윤 [2011.1.26] // 추가 퀘스트 클리어 타입 스트링
#endif SERV_QUEST_CLEAR_EXPAND
#ifdef SERV_NEW_PVP_QUEST
	case CX2QuestManager::SQT_PVP_NPC_HUNT:
		{
			// oasis907 : 김상윤 [2011.7.27] 오토디스크립션은 필요하게 되면 작업
			if( NULL != pQuestInst )
			{
				const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
				if( NULL != pSubQuestInst )
				{
					int iClearData = static_cast<int>( pSubQuestInst->m_ucClearData );
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iPVPWin << std::endl;
				}
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iPVPWin << std::endl;
			}
		} break;
#endif SERV_NEW_PVP_QUEST

		// {{ kimhc // 2012-06-18 // 마을로 이동하기만 해도 완료되는 퀘스트 작업
#ifdef	SERV_ENTER_FIELD_QUEST_CLEAR
		// 마을 방문 퀘스트에서
	case CX2QuestManager::SQT_VISIT_VILLAGE:
		{
			// 오토디스크립션의 경우
			if ( bAuto )
			{
	#ifdef REFORM_QUEST
				std::set<int>::iterator sit = pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.begin();
				for( ; sit != pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.end(); ++sit)
				{
					const SEnum::VILLAGE_MAP_ID eVillageMapID = static_cast<SEnum::VILLAGE_MAP_ID>( *sit );
					const CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eVillageMapID );

					// @마을이름 방문하기 로 출력
					if ( NULL != pVillageTemplet )
					{
						wstrmQuestCond  << pVillageTemplet->m_Name << GET_STRING( STR_ID_17688 ) << std::endl;
					}
				}
	#else
				const SEnum::VILLAGE_MAP_ID eVillageMapID = static_cast<SEnum::VILLAGE_MAP_ID>( pSubQuestTemplet->m_ClearCondition.m_iVillageMapID );
				const CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eVillageMapID );

				// @마을이름 방문하기 로 출력
				if ( NULL != pVillageTemplet )
				{
					wstrmQuestCond  << pVillageTemplet->m_Name << GET_STRING( STR_ID_17688 ) << std::endl;
				}
	#endif //REFORM_QUEST
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}
		} break;
	#ifdef REFORM_QUEST
	case CX2QuestManager::SQT_VISIT_FIELD:
		{
			wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
		} break;
	case CX2QuestManager::SQT_VISIT_DUNGEON:
		{
			// 오토디스크립션의 경우
			if ( bAuto )
			{
				if( NULL != g_pData->GetDungeonManager() )
				{
					std::set<SEnum::DUNGEON_ID>::iterator sit = pSubQuestTemplet->m_ClearCondition.m_setDungeonID.begin();
					for( ; sit != pSubQuestTemplet->m_ClearCondition.m_setDungeonID.end(); ++sit)
					{
						const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( *sit );
						if( NULL != pDungeonData )
						{
							wstrmQuestCond  << pDungeonData->m_DungeonName << GET_STRING( STR_ID_17688 ) << std::endl;
						}
					}
				}
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}
		} break;
	case CX2QuestManager::SQT_FIND_NPC:
		{
			wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
		} break;
	#endif //REFORM_QUEST
#endif	SERV_ENTER_FIELD_QUEST_CLEAR
		// }} kimhc // 2012-06-18 // 마을로 이동하기만 해도 완료되는 퀘스트 작업
#ifdef SERV_SUB_QUEST_USE_ITEM
	case CX2QuestManager::SQT_ITEM_USE:
		{
			if( NULL != pQuestInst )
			{
				int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iUseItemNum;
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iUseItemNum << std::endl;
			}
		}
		break;
#endif SERV_SUB_QUEST_USE_ITEM		
#ifdef SERV_POINT_COUNT_SYSTEM
	case CX2QuestManager::SQT_POINT_COUNT:
		{
			//여기서 디스크립션 나타내면 된다.
			if( NULL != pQuestInst )
			{
				int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iPointCount << std::endl;
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iPointCount << std::endl;
			}
		}
		break;
#endif SERV_POINT_COUNT_SYSTEM
#ifdef PVP_SEASON2
	case CX2QuestManager::SQT_PVP_PLAY_ARRANGE:
		{
			if( NULL != pQuestInst )
			{
				int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iPVPPlay << std::endl;
			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << clearCond.m_iPVPPlay << std::endl;
			}
		} break;
#endif
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	case SQT_CHAR_LEVEL_UP:
		{
			if( NULL != pQuestInst )
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << std::endl;
			}
		} break;
#endif //SERV_ACCOUNT_MISSION_SYSTEM

#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
	case CX2QuestManager::SQT_PVP_HERO_NPC_KILL:
		{
			// oasis907 : 김상윤 [2011.7.27] 오토디스크립션은 필요하게 되면 작업
			if( NULL != pQuestInst )
			{
				int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iPVPKill << std::endl;

			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iPVPKill << std::endl;
			}
		} break;

	case CX2QuestManager::SQT_PVP_HERO_NPC_PLAY:
		{
			// oasis907 : 김상윤 [2011.7.27] 오토디스크립션은 필요하게 되면 작업
			if( NULL != pQuestInst )
			{
				int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iPVPPlay << std::endl;

			}
			else
			{
				wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iPVPPlay << std::endl;
			}
		} break;
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND
#ifdef SERV_SKILL_USE_SUBQUEST
	case CX2QuestManager::SQT_SKILL_USE:
		{
			// NPCID가 2개 이상이면 Auto출력 안함!
			if( clearCond.m_setSkillID.size() > 1 )
			{
				bAuto = false;
			}

#ifdef UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( ( CX2SkillTree::SKILL_ID )( *clearCond.m_setSkillID.begin()) );
#else
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( ( CX2SkillTree::SKILL_ID )( *clearCond.m_setSkillID.begin()), 1 );
#endif UPGRADE_SKILL_SYSTEM_2013

			if( true == bAuto )
			{
				wstrmQuestCond << pSkillTemplet->m_wstrName ;

				if( NULL != pQuestInst )
				{
					int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
					wstrmQuestCond << L" " << iClearData << L"/" << clearCond.m_iUseSkillCount /*<< L"마리 퇴치"*/;
				}
				else
				{
					wstrmQuestCond << L" " << L"0/" << clearCond.m_iUseSkillCount /*<< L"마리 퇴치"*/;
				}
			}
			else
			{
				if( NULL != pQuestInst )
				{
					int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iUseSkillCount << std::endl;
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iUseSkillCount << std::endl;
				}
			}

		} break;
#endif SERV_SKILL_USE_SUBQUEST
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
	case CX2QuestManager::SQT_LEARN_NEW_SKILL:
		{
			// NPCID가 2개 이상이면 Auto출력 안함!
			if( clearCond.m_setSkillID.size() > 1 )
			{
				bAuto = false;
			}

#ifdef UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( ( CX2SkillTree::SKILL_ID )( *clearCond.m_setSkillID.begin()) );
#else
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( ( CX2SkillTree::SKILL_ID )( *clearCond.m_setSkillID.begin()), 1 );
#endif UPGRADE_SKILL_SYSTEM_2013

			if( true == bAuto )
			{
				wstrmQuestCond << pSkillTemplet->m_wstrName ;

				if( NULL != pQuestInst )
				{
					int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
					wstrmQuestCond << L" " << iClearData << L"/" << clearCond.m_iLearnNewSkillCount /*<< L"마리 퇴치"*/;
				}
				else
				{
					wstrmQuestCond << L" " << L"0/" << clearCond.m_iLearnNewSkillCount /*<< L"마리 퇴치"*/;
				}
			}
			else
			{
				if( NULL != pQuestInst )
				{
					int iClearData = (int) pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID )->m_ucClearData;
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" " << iClearData << L"/" << clearCond.m_iLearnNewSkillCount << std::endl;
				}
				else
				{
					wstrmQuestCond << pSubQuestTemplet->m_wstrDescription << L" 0/" << clearCond.m_iLearnNewSkillCount << std::endl;
				}
			}
		} break;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
	}

	return wstrmQuestCond.str();
}

//{{ kimhc // 실시간으로 퀘스트에 필요한 아이템 획득 부분 체크
bool CX2QuestManager::CheckItemCollection( int iItemID )
{
	// 던전게임중 아이템 수집 퀘스트 실시간 표시 관련
	if( CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() ||
		CX2Main::XS_TRAINING_GAME == g_pMain->GetNowStateID() ||
		CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID())
	{
		if( false == g_pMain->GetIsPlayingTutorial() )
		{
			//CX2StateDungeonGame* pStateDungeonGame = (CX2StateDungeonGame*) g_pMain->GetNowState();

#ifdef SERV_EPIC_QUEST
			// 2009.01.05 김태완 : 새 퀘스트 UI로 이동
			if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
			{
				g_pData->GetUIManager()->CreateUIQuestNew();
			}
#else
			// 2009.01.05 김태완 : 새 퀘스트 UI로 이동
			if(g_pData->GetUIManager()->GetUIQuest() == NULL)
			{
				g_pData->GetUIManager()->CreateUIQuest();
			}
#endif SERV_EPIC_QUEST

#ifdef SERV_EPIC_QUEST
			// 2009.01.05 김태완 : 새 퀘스트 UI로 이동
			if( /*pStateDungeonGame != NULL &&*/ g_pData->GetUIManager()->GetUIQuestNew() != NULL)
#else
			// 2009.01.05 김태완 : 새 퀘스트 UI로 이동
			if( /*pStateDungeonGame != NULL &&*/ g_pData->GetUIManager()->GetUIQuest() != NULL )
#endif SERV_EPIC_QUEST
			{

				const map<int, QuestInst*>& unitQuestMap = GetUnitQuestMap();
				std::map<int, QuestInst*>::const_iterator it;

				//bool bDone = false;
				for( it = unitQuestMap.begin(); it != unitQuestMap.end(); it++ )
				{
					const QuestTemplet*		pQuestTemplet	= GetQuestTemplet( (int) it->first );
					const QuestInst*		pQuestInst		= it->second;

#ifdef ADD_SERVER_GROUP
					if( pQuestTemplet == NULL )
						continue;
#endif

					// 서브 퀘스트 
					for( UINT i=0; i< pQuestTemplet->m_vecSubQuest.size(); i++ )
					{							
						const SubQuestTemplet* pSubQuestTemplet	= GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );

						if( NULL == pSubQuestTemplet )
							continue;

						if( ( SQT_ITEM_COLLECTION == pSubQuestTemplet->m_eClearType ||
							SQT_QUEST_ITEM_COLLECTION == pSubQuestTemplet->m_eClearType ) && 
							pSubQuestTemplet->m_ClearCondition.m_iCollectionItemID == iItemID )
						{											
							int nCount = 0;

							const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
							{
								nCount += kInventory.GetNumItemByTID( iItemID );
							}

							if( nCount <= pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum )
							{
								wstring wstrSubQuestDesc = MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
									pSubQuestTemplet, pQuestInst );
#ifdef CLIENT_GLOBAL_LINEBREAK
								wstrSubQuestDesc = CWordLineHandler::GetStrByLineBreakInX2Main(wstrSubQuestDesc.c_str(), 500, XUF_DODUM_20_BOLD);
#endif //CLIENT_GLOBAL_LINEBREAK
								if ( g_pX2Game != NULL )
								{
									g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wstrSubQuestDesc.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
								}

								return true;	// 퀘스트용 수집 아이템
							}

						}
					}
				}
			}
		}
	}

	return false;	// 수집할 아이템이 아님

}
//}} kimhc // 실시간으로 퀘스트에 필요한 아이템 획득 부분 체크

//{{ kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
#ifdef	SERV_DAY_QUEST
void CX2QuestManager::SetTimedEventQuest( int iQuestID, bool bValue )
{
	map<int,QuestTemplet*>::iterator iter;
	iter = m_mapQuestTemplet.find( iQuestID );

	if( iter == m_mapQuestTemplet.end() )	
		return;

#ifdef SHOW_DAILY_REPEAT_MARK_ON_EVENT_QUEST_FOR_DEVELOPER_FIX
	iter->second->m_bStartTimeEvent = !bValue;
#endif //SHOW_DAILY_REPEAT_MARK_ON_EVENT_QUEST_FOR_DEVELOPER_FIX

	iter->second->m_bIsTimeEvent = bValue;
}
#endif	SERV_DAY_QUEST
//}} kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현


bool CX2QuestManager::IsForbiddenQuest( int iQuestID )
{


	if( NULL == g_pData->GetMyUser() )
		return false;

	if( NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return false;


#ifdef SERV_SKILL_NOTE
	const int iQuestMagicID1 = 9000;	// 기술의 노트 1/2
	const int iQuestMagicID2 = 9010;	// 기술의 노트 2/2
	if( ( iQuestID == iQuestMagicID1 || iQuestID == iQuestMagicID2 ) && g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot() > 0 )
		return true;
#endif

#ifdef SERV_SHARING_BANK_QUEST_CASH
	if( g_pData->GetMyUser()->IsSharingBankOpen() == true)
		if( iQuestID == 60720 || iQuestID == 60710 || iQuestID == 60700) // 은행 공유 퀘스트
			return true;
#endif SERV_SHARING_BANK_QUEST_CASH

	const CX2QuestManager::QuestTemplet* pQuestTemplet = GetQuestTemplet( iQuestID );
	if( NULL == pQuestTemplet )
		return true;





	if( CX2Unit::UT_NONE == pQuestTemplet->m_Condition.m_eUnitType )
	{
		if( false == pQuestTemplet->m_Condition.m_setUnitClass.empty() )
		{
			std::set< CX2Unit::UNIT_CLASS >::const_iterator it = pQuestTemplet->m_Condition.m_setUnitClass.find( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
			if( it == pQuestTemplet->m_Condition.m_setUnitClass.end() )
				return true;
		}
	}
	else
	{
		if( g_pData->GetMyUser()->GetSelectUnit()->GetType() != pQuestTemplet->m_Condition.m_eUnitType )
		{
			return true;
		}
	}


	BOOST_TEST_FOREACH( int, itemID, pQuestTemplet->m_vecShowItemID )
	{
		if( NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( itemID, true ) )
		{
			return true;
		}
	}


	BOOST_TEST_FOREACH( int, itemID, pQuestTemplet->m_vecHideItemID )
	{
		if( NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( itemID, true ) )
		{
			return true;
		}
	}



	BOOST_TEST_FOREACH( int, skillID, pQuestTemplet->m_vecHideSkillID )
	{
		if( true == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.IsSkillUnsealed( (CX2SkillTree::SKILL_ID) skillID ) )
		{
			return true;

		}
	}


	BOOST_TEST_FOREACH( const CX2Inventory::MEMBERSHIP_PRIVATE_BANK, bankGrade, pQuestTemplet->m_vecHideBankGrade )
	{
		if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetBankMembershipGrade() == bankGrade )
		{
			return true;
		}
	}
	
	return false;
}



void CX2QuestManager::GiveUpForbiddenQuest()
{
#ifdef SERV_GATHER_GIVE_UP_QUEST
	std::vector< int > vecGatherQuestID;
#endif // SERV_GATHER_GIVE_UP_QUEST

	map<int,QuestInst*>::iterator it;
	for( it = m_mapUnitQuest.begin(); it != m_mapUnitQuest.end(); it++ )
	{
		QuestInst* pQuestInst = it->second;
		if( NULL == pQuestInst )
			continue;

		if( true == IsForbiddenQuest( pQuestInst->m_iID ) )
		{
#ifdef SERV_GATHER_GIVE_UP_QUEST
	#ifdef SHOW_ONLY_MY_EVENT_QUEST
			const CX2QuestManager::QuestTemplet* pQuestTemplet 
				= g_pData->GetQuestManager()->GetQuestTemplet( pQuestInst->m_iID );

			if( NULL != pQuestTemplet )
			{
				switch( pQuestTemplet->m_eQuestType )
				{
					// 이벤트 타입은 유닛타입 검사하여, 자기 유닛의 퀘스트일 경우만 받기
				case CX2QuestManager::QT_EVENT:
					{
						if( CX2Unit::UT_NONE == pQuestTemplet->m_Condition.m_eUnitType ||
							g_pData->GetSelectUnitType() == pQuestTemplet->m_Condition.m_eUnitType )
							vecGatherQuestID.push_back( pQuestInst->m_iID );
					} break;
				default:
					vecGatherQuestID.push_back( pQuestInst->m_iID );
					break;
				}
			}
	#else
			vecGatherQuestID.push_back( pQuestInst->m_iID );
	#endif // SHOW_ONLY_MY_EVENT_QUEST
#else // SERV_GATHER_GIVE_UP_QUEST
#ifdef SERV_EPIC_QUEST
			ASSERT( NULL != g_pData->GetUIManager()->GetUIQuestNew() );
			if( NULL != g_pData->GetUIManager()->GetUIQuestNew() )
			{
				g_pData->GetUIManager()->GetUIQuestNew()->Handler_EGS_GIVE_UP_QUEST_REQ( pQuestInst->m_iID );
			}
#else
			ASSERT( NULL != g_pData->GetUIManager()->GetUIQuestReceive() );
			if( NULL != g_pData->GetUIManager()->GetUIQuestReceive() )
			{
				g_pData->GetUIManager()->GetUIQuestReceive()->Handler_EGS_GIVE_UP_QUEST_REQ( pQuestInst->m_iID );
			}
#endif SERV_EPIC_QUEST
#endif // SERV_GATHER_GIVE_UP_QUEST
		}
	}
	
#ifdef SERV_GATHER_GIVE_UP_QUEST
	ASSERT( NULL != g_pData->GetUIManager()->GetUIQuestNew() );
	if( NULL != g_pData->GetUIManager()->GetUIQuestNew() && !vecGatherQuestID.empty() )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->Handler_EGS_GATHER_GIVE_UP_QUEST_REQ( vecGatherQuestID );
	}
#endif // SERV_GATHER_GIVE_UP_QUEST

}
#ifdef SERV_EPIC_QUEST
wstring	CX2QuestManager::GetEpicGroupTitle( int iEpicGroupID)
{
	wstring tmp = L"";

	map<int, wstring>::iterator mItr = m_mapEpicGroupTitle.find( iEpicGroupID );

	if ( mItr != m_mapEpicGroupTitle.end() )
	{
		return mItr->second;
	}	
	else
	{
		return tmp;
	}
}
#endif SERV_EPIC_QUEST

#ifdef SERV_RANDOM_DAY_QUEST
void CX2QuestManager::SetRandomQuestInfo(std::map<int,int >& mapRandomQuestInfo )
{
	m_mapTodayRandomQuest = mapRandomQuestInfo;
	return;
}
bool CX2QuestManager::CheckGiveUpRandomQuest( int iRandomGroupId, int iQuestID )
{
	if(iRandomGroupId == 0)
		return false;

	map<int, int>::iterator mit = m_mapTodayRandomQuest.find( iRandomGroupId );

	if ( mit == m_mapTodayRandomQuest.end() )
		return true;
	else
	{
		if(mit->second == iQuestID)
		{
			map<int, std::vector< int >>::iterator mitRandom = m_mapRandomQuest.find( iRandomGroupId );

			std::vector< int >& vecGroupQuest = mitRandom->second;
			for(UINT i = 0 ; i < vecGroupQuest.size() ; i++ )
			{
				map<int,int>::iterator mit_complete;
				mit_complete = m_mapCompleteQuest.find( vecGroupQuest[i] );
				if(mit_complete == m_mapCompleteQuest.end())
				{
					continue;
				}
				else
				{
					return true;
				}
			}
			for(UINT i = 0 ; i < vecGroupQuest.size() ; i++ )
			{
				map<int,QuestInst*>::iterator mit_unit;
				mit_unit = m_mapUnitQuest.find( vecGroupQuest[i] );

				if( mit_unit == m_mapUnitQuest.end() )
				{
					continue;
				}
				else
				{
					return true;
				}
			}
			return false;
		}
		else
			return true;
	}
}
#endif SERV_RANDOM_DAY_QUEST

#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
void CX2QuestManager::CompleteQuestToVisitVillage()
{
	if( NULL != g_pData && NULL != g_pData->GetQuestManager() )
	{
		KEGS_ALL_COMPLETED_QUEST_COMPLETE_REQ kPacketNot;

		for( unsigned int i=0; i< m_vecCompletedQuestID.size(); ++i )
		{		
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_vecCompletedQuestID[i] );
			if( NULL != pQuestTemplet )
			{
				CX2State* pState = static_cast< CX2State* >( g_pMain->GetNowState() );
				if( NULL != pState )
				{
					kPacketNot.m_vecQuestCompleteReq.push_back( pState->CompleteQuestToVisitVillageList( pQuestTemplet ) );
				}
			}
		}
		m_vecCompletedQuestID.clear();
		if( NULL !=	g_pData->GetUIManager()->GetUIQuestNew() )
		{
			if( false == kPacketNot.m_vecQuestCompleteReq.empty() )
			{
				g_pData->GetUIManager()->GetUIQuestNew()->Handler_EGS_ALL_COMPLETED_QUEST_COMPLETE_REQ(kPacketNot);
			}
		}
	}

}
void CX2QuestManager::InsertCompletedVisitQuestID(QuestInst* pQuestInst)
{	
	if( NULL != g_pData && NULL != g_pData->GetQuestManager() )
	{	//서브퀘스트가 1개가 아니라면 자동완료 하지 않도록 처리
		if( pQuestInst->m_vecSubQuestInst.size() != 1 )
			return;
			
#ifdef NO_FINISH_EVENT_VISIT_QUEST
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( pQuestInst->m_iID );
		if( pQuestTemplet != NULL && pQuestTemplet->m_eQuestType == CX2QuestManager::QT_EVENT )
			return;
#endif // NO_FINISH_EVENT_VISIT_QUEST			

		const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestInst->m_vecSubQuestInst[0].m_iID );
#ifdef REFORM_QUEST
		if( NULL != pSubQuestTemplet )
		{
			switch ( pSubQuestTemplet->m_eClearType )
			{
			case CX2QuestManager::SQT_VISIT_VILLAGE:
			case CX2QuestManager::SQT_VISIT_FIELD:
			case CX2QuestManager::SQT_VISIT_DUNGEON:
				{
					if( true == pQuestInst->IsComplete() )
					{
						m_vecCompletedQuestID.push_back( pQuestInst->m_iID );
					}
				}
				break;
			}
		}
#else
		if( NULL != pSubQuestTemplet && CX2QuestManager::SQT_VISIT_VILLAGE == pSubQuestTemplet->m_eClearType )
		{
			if( true == pQuestInst->IsComplete() )
			{
				m_vecCompletedQuestID.push_back( pQuestInst->m_iID );
			}
		}
#endif //REFORM_QUEST
	}
}
#endif //SERV_ENTER_FIELD_QUEST_CLEAR

#ifdef QUEST_GUIDE
bool CX2QuestManager::GetNeedToTalkNPCList( OUT map<CX2UnitManager::NPC_UNIT_ID, bool>& mapGuideTarget_ )
{
	mapGuideTarget_.clear();

	//유닛이 가지고 있는 퀘스트 목록 순회
	map<int,QuestInst*>::iterator mit = m_mapUnitQuest.begin();
	for( ; mit != m_mapUnitQuest.end(); ++mit )
	{
		CX2QuestManager::QuestInst* pQuestInst = static_cast<CX2QuestManager::QuestInst*>(mit->second);

		if( NULL != pQuestInst )
		{
			const vector<SubQuestInst>& vecSubQuestInst = pQuestInst->m_vecSubQuestInst;
			for(UINT i=0; i<vecSubQuestInst.size(); ++i)
			{
				if( true != vecSubQuestInst[i].IsComplete() )
				{
					const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( vecSubQuestInst[i].m_iID );
					if( NULL != pSubQuestTemplet )
					{
						//퀘스트 타입이 SQT_NPC_TALK일 경우 해당 NPC ID와, 퀘스트 타입 기록
						if( SQT_NPC_TALK == pSubQuestTemplet->m_eClearType )
						{
							CX2UnitManager::NPC_UNIT_ID eTalkNPCID = pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID;
							bool bIsEpic = false;

							const QuestTemplet* pQuestTemplet = GetQuestTemplet( pQuestInst->m_iID );
							if( NULL != pQuestTemplet )
							{
								if( QT_EPIC == pQuestTemplet->m_eQuestType )
								{
									bIsEpic = true;
								}
							}
							mapGuideTarget_.insert( std::make_pair(eTalkNPCID, bIsEpic) );				
						}
					}
				}

			}
		}
	}

	return true;
}
bool CX2QuestManager::GetNeedToVisitVillageList( OUT map<int, bool>& mapGuideTarget_ )// 퀘스트 완료를 위해 방문이 필요한 VILLAGE ID
{
	//유닛이 가지고 있는 퀘스트 목록 순회
	map<int,QuestInst*>::iterator mit = m_mapUnitQuest.begin();
	for( ; mit != m_mapUnitQuest.end(); ++mit )
	{
		CX2QuestManager::QuestInst* pQuestInst = static_cast<CX2QuestManager::QuestInst*>(mit->second);

		if( NULL != pQuestInst )
		{
			//퀘스트 내의 세부 서브퀘스트 목록 순회
			const vector<SubQuestInst>& vecSubQuestInst = pQuestInst->m_vecSubQuestInst;
			for(UINT i=0; i<vecSubQuestInst.size(); ++i)
			{
				if( true != vecSubQuestInst[i].IsComplete() )
				{
					const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( vecSubQuestInst[i].m_iID );
					if( NULL != pSubQuestTemplet )
					{
						//마을방문 타입의 퀘스트
						if( SQT_VISIT_VILLAGE == pSubQuestTemplet->m_eClearType )
						{
							bool bIsEpic = false;
							const QuestTemplet* pQuestTemplet = GetQuestTemplet( pQuestInst->m_iID );
							if( NULL != pQuestTemplet )
							{
								if( QT_EPIC == pQuestTemplet->m_eQuestType )
								{
									bIsEpic = true;
								}
							}

							//해당 타입의 서브퀘스트와 연결된 마을ID 등록
							for( std::set<int>::iterator sit = pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.begin();
								sit != pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.end(); ++sit)
							{
								mapGuideTarget_.insert( std::make_pair(*sit, bIsEpic) );		
							}
						}
					}
				}

			}
		}

	}
	return true;
}

bool CX2QuestManager::GetNeedToVisitFieldList( OUT map<int, bool>& mapGuideTarget_ )// 퀘스트 완료를 위해 방문이 필요한 Field ID
{
	//유닛이 가지고 있는 퀘스트 목록 순회
	map<int,QuestInst*>::iterator mit = m_mapUnitQuest.begin();
	for( ; mit != m_mapUnitQuest.end(); ++mit )
	{
		CX2QuestManager::QuestInst* pQuestInst = static_cast<CX2QuestManager::QuestInst*>(mit->second);
		if( NULL != pQuestInst )
		{
			//퀘스트 내의 세부 서브퀘스트 목록 순회
			const vector<SubQuestInst>& vecSubQuestInst = pQuestInst->m_vecSubQuestInst;
			for(UINT i=0; i<vecSubQuestInst.size(); ++i)
			{
				if( true != vecSubQuestInst[i].IsComplete() )
				{
					const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( vecSubQuestInst[i].m_iID );
					if( NULL != pSubQuestTemplet )
					{
						//필드방문 타입의 퀘스트
						if( SQT_VISIT_FIELD == pSubQuestTemplet->m_eClearType )
						{
							bool bIsEpic = false;
							const QuestTemplet* pQuestTemplet = GetQuestTemplet( pQuestInst->m_iID );
							if( NULL != pQuestTemplet )
							{
								if( QT_EPIC == pQuestTemplet->m_eQuestType )
								{
									bIsEpic = true;
								}
							}

							//해당 타입의 서브퀘스트와 연결된 필드ID 등록
							for( std::set<int>::iterator sit = pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.begin();
								sit != pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.end(); ++sit)
							{
								mapGuideTarget_.insert( std::make_pair(*sit, bIsEpic) );		
							}
						}
					}
				}
			}
		}
	}
	return true;
}
void CX2QuestManager::GetCompleteTalkQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, int &nNormal )
{
	nNormal = 0;

	if( CX2UnitManager::NUI_NONE == eQuestNPCID )
		return;

	map<CX2UnitManager::NPC_UNIT_ID, bool> MapNeedToTalkNPCList;
	if( true == GetNeedToTalkNPCList(MapNeedToTalkNPCList ) )
	{
		map<CX2UnitManager::NPC_UNIT_ID, bool>::iterator it = MapNeedToTalkNPCList.find(eQuestNPCID);
		if( it != MapNeedToTalkNPCList.end() )
		{
			++nNormal;
		}
	}
}

bool CX2QuestManager::GetNeedToClearDungeonList( OUT map<SEnum::DUNGEON_ID, bool>& mapGuideTarget_)
{
	//유닛이 가지고 있는 퀘스트 목록 순회
	map<int,QuestInst*>::iterator mit = m_mapUnitQuest.begin();
	for( ; mit != m_mapUnitQuest.end(); ++mit )
	{
		CX2QuestManager::QuestInst* pQuestInst = static_cast<CX2QuestManager::QuestInst*>(mit->second);
		if( NULL != pQuestInst )
		{
			const vector<SubQuestInst>& vecSubQuestInst = pQuestInst->m_vecSubQuestInst;
			for(UINT i=0; i<vecSubQuestInst.size(); ++i)
			{
				if( true != vecSubQuestInst[i].IsComplete() )
				{
					const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( vecSubQuestInst[i].m_iID );
					if( NULL != pSubQuestTemplet )
					{
						bool bIsEpic = false;
						const QuestTemplet* pQuestTemplet = GetQuestTemplet( pQuestInst->m_iID );
						if( NULL != pQuestTemplet )
						{
							if( QT_EPIC == pQuestTemplet->m_eQuestType )
							{
								bIsEpic = true;
							}
						}

						//해당 타입의 서브퀘스트와 연결된 필드ID 등록
						for( std::set<SEnum::DUNGEON_ID>::iterator sit = pSubQuestTemplet->m_ClearCondition.m_setDungeonID.begin();
							sit != pSubQuestTemplet->m_ClearCondition.m_setDungeonID.end(); ++sit)
						{
							if( NULL != g_pData && NULL != g_pData->GetDungeonManager() && NULL != g_pData->GetDungeonManager()->GetDungeonData(*sit) )
							{	//난이도를 제외한 던전ID로 구별합니다.
								int iExecptDiffucltyLevelDungeonID = *sit - g_pData->GetDungeonManager()->GetDungeonData(*sit)->m_eDifficulty;
								mapGuideTarget_.insert( std::make_pair( static_cast<SEnum::DUNGEON_ID>(iExecptDiffucltyLevelDungeonID), bIsEpic) );	
							}	
						}						
					}
				}	
			}
		}
	}
	return true;
}

void CX2QuestManager::SetHasDungeonQuest()
{
	m_bHasDungeonQuest = false;
	//유닛이 가지고 있는 퀘스트 목록 순회
	map<int,QuestInst*>::iterator mit = m_mapUnitQuest.begin();
	for( ; mit != m_mapUnitQuest.end(); ++mit )
	{
		CX2QuestManager::QuestInst* pQuestInst = static_cast<CX2QuestManager::QuestInst*>(mit->second);
		if( NULL != pQuestInst )
		{
			const vector<SubQuestInst>& vecSubQuestInst = pQuestInst->m_vecSubQuestInst;
			for(UINT i=0; i<vecSubQuestInst.size(); ++i)
			{
				if( true != vecSubQuestInst[i].IsComplete() )
				{
					const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( vecSubQuestInst[i].m_iID );
					if( NULL != pSubQuestTemplet )
					{
						if( false == pSubQuestTemplet->m_ClearCondition.m_setDungeonID.empty() )
						{
							m_bHasDungeonQuest = true;
							return;
						}						
					}
				}	
			}
		}
	}
}
#endif //QUEST_GUIDE

#ifdef SERV_POINT_COUNT_SYSTEM
void CX2QuestManager::SetUpdataQuestInstance(std::map< int, KQuestInstance > mapQuestInstance)
{
	if ( mapQuestInstance.empty() == false )
	{
		std::map< int, KQuestInstance >::const_iterator cmit = mapQuestInstance.begin();

		for ( ; cmit != mapQuestInstance.end() ; ++cmit	)
		{
			
			for( int iIndexSub = 0; iIndexSub < (int)cmit->second.m_vecSubQuestInstance.size(); ++iIndexSub )
			{
				
				const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( cmit->second.m_vecSubQuestInstance[iIndexSub].m_iID );

				if ( pSubQuestTemplet == NULL)
					continue;

				switch( pSubQuestTemplet->m_eClearType ) // 2013.04.09 lygan_조성욱 // 각 퀘스트의 서브 타입별 체크해서 갱신 시킬 항목을 따로 선정해 갱신 시키도록 하자. point 의 경우 퀘스트 클리어시 소멸되는 point양을 다른 동일한 퀘스트들도 갱신해줘야 한다.
				{
				case CX2QuestManager::SQT_POINT_COUNT:
					{
						map<int,QuestInst*>::iterator mit = m_mapUnitQuest.find(cmit->first);

						if (  mit != m_mapUnitQuest.end() )
						{
							for( int iIndexUnitSub = 0; iIndexUnitSub < (int)mit->second->m_vecSubQuestInst.size(); ++iIndexUnitSub )
							{
								const CX2QuestManager::SubQuestTemplet* pSubUnitQuestTemplet = GetSubQuestTemplet( mit->second->m_vecSubQuestInst[iIndexUnitSub].m_iID );

								if ( pSubUnitQuestTemplet == NULL )
									continue;

								if ( pSubQuestTemplet->m_eClearType == pSubUnitQuestTemplet->m_eClearType )
								{
									mit->second->m_vecSubQuestInst[iIndexUnitSub].m_ucClearData = cmit->second.m_vecSubQuestInstance[iIndexSub].m_ucClearData;
									mit->second->m_vecSubQuestInst[iIndexUnitSub].m_bIsSuccess = cmit->second.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess;
								}
							}
						}
						
					}
					break;
				}

			}
		}
	}
}
#endif //SERV_POINT_COUNT_SYSTEM

//{{ 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력
#ifdef PRINT_INGAMEINFO_TO_EXCEL
/*
void CX2QuestManager::PrintQuestInfo_ToExcel()
{
	QuestTemplet* pQuestTemplet = NULL;
	map<int,QuestTemplet*>::iterator iter;
	QuestInst* pQuestInst = NULL;
	wofstream of(L"퀘스트 목록.txt");
	of.imbue(std::locale("kor",locale::ctype));

	int count = 1;
	for( iter = m_mapQuestTemplet.begin(); iter != m_mapQuestTemplet.end(); iter++ )
	{	
		pQuestTemplet = iter->second;
		pQuestInst = GetUnitQuest( iter->second->m_iID );	
		if(true != iter->second->m_vecSubQuest.empty())
		{
			of << L"MainQuest : " << pQuestTemplet->m_iID << endl << pQuestTemplet->m_wstrTitle << endl;
			for( UINT i=0; i< pQuestTemplet->m_vecSubQuest.size(); i++ )
			{	
				const SubQuestTemplet* pSubQuestTemplet= GetSubQuestTemplet(iter->second->m_vecSubQuest[i] );
				if( pSubQuestTemplet->m_bAutomaticDescription == true )
				{	
					wstring wstrSubQuestDesc = MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
						pSubQuestTemplet, pQuestInst );
					of << L"	SubQuest : " << pSubQuestTemplet->m_iID << L" : " << wstrSubQuestDesc << endl;
				}else{
					of << L"	SubQuest : " << pSubQuestTemplet->m_iID << L" : " << pSubQuestTemplet->m_wstrDescription << endl;	
				}
			}
			of << endl;
			of << L"----------------------------------------------------------------------------------------"<< endl;
		}
	}
	of.close();
}
*/
void CX2QuestManager::PrintQuestInfo_ToExcel()
{
	BasicExcel e;
	e.New(1);
	
	BasicExcelWorksheet* sheet = e.GetWorksheet((size_t)0);
	int cal = 1;
	QuestTemplet* pQuestTemplet = NULL;
	map<int,QuestTemplet*>::iterator iter;
	
	QuestInst* pQuestInst = NULL;
	// Title 출력
	sheet->Cell(0,0)->SetWString(L"메인퀘스트ID");	
	sheet->Cell(0,1)->SetWString(L"메인퀘스트 이름");	
	sheet->Cell(0,2)->SetWString(L"서브퀘스트/ID");	
	sheet->Cell(0,3)->SetWString(L"서브퀘스트 설명");	
	sheet->Cell(0,4)->SetWString(L"AutomaticDescription");

	// 내용 출력
	for( iter = m_mapQuestTemplet.begin(); iter != m_mapQuestTemplet.end(); iter++ )
	{	
		pQuestTemplet = iter->second;
		pQuestInst = GetUnitQuest( iter->second->m_iID );	
		if(true != iter->second->m_vecSubQuest.empty())	{
			sheet->Cell(cal,0)->SetInteger(pQuestTemplet->m_iID);
			sheet->Cell(cal,1)->SetWString(pQuestTemplet->m_wstrTitle.c_str());

			for( UINT i=0; i< pQuestTemplet->m_vecSubQuest.size(); i++ )
			{	
				const SubQuestTemplet* pSubQuestTemplet= GetSubQuestTemplet(iter->second->m_vecSubQuest[i] );

				if( pSubQuestTemplet == NULL )
					continue;

				if( pSubQuestTemplet->m_bAutomaticDescription == true )
				{	
					wstring wstrSubQuestDesc = MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
						pSubQuestTemplet, pQuestInst );
					sheet->Cell(cal,2)->SetInteger(pSubQuestTemplet->m_iID);
					sheet->Cell(cal,3)->SetWString(wstrSubQuestDesc.c_str());
					sheet->Cell(cal,4)->SetWString(L"TRUE");
				}
				else
				{	
					wstring wstrSubQuestDesc = MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
						pSubQuestTemplet, pQuestInst );
					sheet->Cell(cal,2)->SetInteger(pSubQuestTemplet->m_iID);
					
					wchar_t * nextline = L"\n";
					wchar_t * afternextline = L"\\n";
					size_t ipos = wstrSubQuestDesc.find(nextline);
					
					while(ipos < std::wstring::npos){
						wstrSubQuestDesc.replace(ipos, 1, afternextline );
						ipos = wstrSubQuestDesc.find(nextline, ipos);
					}
					sheet->Cell(cal,3)->SetWString(wstrSubQuestDesc.c_str());
					sheet->Cell(cal,4)->SetWString(L"FALSE");
				}
				cal++;
			}
		}
	}
	e.SaveAs("QUEST_LIST.xls");
}
#endif PRINT_INGAMEINFO_TO_EXCEL
//}} 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
bool CX2QuestManager::IsNewUserOnlyQuest( int iQuestID )
{
	int iSize = sizeof( arriRecruitEventQuestIDForNewUser ) / sizeof( arriRecruitEventQuestIDForNewUser[0] );
	for( int iIndex = 0; iIndex < iSize; ++iIndex )
	{
		if( arriRecruitEventQuestIDForNewUser[iIndex] == iQuestID )
			return true;
	}

	return false;
}
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

#ifdef SERV_SKILL_USE_SUBQUEST
bool CX2QuestManager::HasSkillUseSubQuest( const int& iSkillID )
{
	map< int, QuestInst* >::iterator mitQuestInst;
	for( mitQuestInst = m_mapUnitQuest.begin(); mitQuestInst != m_mapUnitQuest.end(); ++mitQuestInst )
	{
		QuestInst* pQuestInst = mitQuestInst->second;
		if( pQuestInst == NULL )
			continue;

		if( true == pQuestInst->IsComplete() )
			continue;

		vector< SubQuestInst >::iterator vit;
		for( vit = pQuestInst->m_vecSubQuestInst.begin(); vit != pQuestInst->m_vecSubQuestInst.end(); ++vit )
		{
			if( vit->m_bIsSuccess == true )
				continue;

			SubQuestTemplet* pSubQuestTemplet = m_mapSubQuestTemplet.find( vit->m_iID )->second;
			if( pSubQuestTemplet == NULL || pSubQuestTemplet->m_eClearType != SQT_SKILL_USE )
				continue;

			std::set< int >& setSkillID = pSubQuestTemplet->m_ClearCondition.m_setSkillID;
			if( setSkillID.find( iSkillID ) != setSkillID.end() )
				return true;
		}
	}

	return false;
}
#endif SERV_SKILL_USE_SUBQUEST