#include "ClassChangeTable.h"
#include "X2Data/XSLUnit.h"

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
//////////////////////////////////////////////////////////////////////////

ImplementRefreshSingleton( KClassChangeTable );
KClassChangeTable::KClassChangeTable(void)
{
}

KClassChangeTable::~KClassChangeTable(void)
{
}

ImplToStringW( KClassChangeTable )
{
	/*
	stm_	<< L"----------[ World Mission Manager ]----------" << std::endl
		<< TOSTRINGW( m_bActive )
		<< BUILD_LOG( m_iTotalCCU )
		<< BUILD_LOG( m_iMissionTime )
		<< BUILD_LOG( m_iNextDOT )
		<< BUILD_LOG( m_usWorldBuffDurationTime )
		<< BUILD_LOG( m_usWorldBuffDurationTime )
		<< BUILD_LOG( m_iCrystalTargetCount )
		<< BUILD_LOG( m_iProtectedCrystalCount )
		<< BUILD_LOG( m_fCountPerWorldCrystal )
		<< BUILD_LOG( m_iWorldCrystalCount )
		<< BUILD_LOG( m_fNormalPlayRate )
		<< BUILD_LOG( m_fHardPlayRate )
		<< BUILD_LOG( m_fExpertPlayRate )
		<< BUILD_LOG( m_fNormalSuccessRate )
		<< BUILD_LOG( m_fHardSuccessRate )
		<< BUILD_LOG( m_fExpertSuccessRate )
		<< TOSTRINGW( m_kTimerDOT.elapsed() )
		;
	*/

	return stm_;
}

ImplementLuaScriptParser( KClassChangeTable )
{

	lua_tinker::class_add<KClassChangeTable>( GetLuaState(), "KClassChangeTable" );
	lua_tinker::class_def<KClassChangeTable>( GetLuaState(), "dump",									&KClassChangeTable::Dump );

	lua_tinker::class_def<KClassChangeTable>( GetLuaState(), "SetClassGroupID"	,						&KClassChangeTable::SetClassGroupID_Lua );
	lua_tinker::class_def<KClassChangeTable>( GetLuaState(), "SetClassChange_Skill",					&KClassChangeTable::SetClassChange_Skill_Lua );
	lua_tinker::class_def<KClassChangeTable>( GetLuaState(), "SetClassChange_Memo",						&KClassChangeTable::SetClassChange_Memo_Lua );
	lua_tinker::class_def<KClassChangeTable>( GetLuaState(), "SetClassChange_InventoryItem",			&KClassChangeTable::SetClassChange_InventoryItem_Lua );
	lua_tinker::class_def<KClassChangeTable>( GetLuaState(), "SetClassChange_Quest",					&KClassChangeTable::SetClassChange_Quest_Lua );
	lua_tinker::class_def<KClassChangeTable>( GetLuaState(), "SetClassChange_DelQuest",					&KClassChangeTable::SetClassChange_DelQuest_Lua );

	lua_tinker::decl( GetLuaState(), "ClassChangeTable", this );
}

// 직업별 그룹id 등록
void KClassChangeTable::SetClassGroupID_Lua( IN int iClassGroup, IN int iUnitClass )
{
	if( iClassGroup <= 0 || iUnitClass <= CXSLUnit::UC_NONE || iUnitClass >= CXSLUnit::UC_MAX )
	{
		START_LOG( cerr, L"직업 변경에 필요한 직업관련 스크립트 파싱 실패!" )
			<< BUILD_LOG( iClassGroup )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return;
	}

	std::map< int, int >::iterator mitClassGroupID = m_mapClassGroupID.find( iUnitClass );
	if( mitClassGroupID != m_mapClassGroupID.end() )
	{
		START_LOG( cerr, L"직업 변경에 필요한 직업관련 중복 파싱!" )
			<< BUILD_LOG( iClassGroup )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return;
	}

	m_mapClassGroupID.insert( std::make_pair( iUnitClass, iClassGroup ) );

	std::map< int, std::vector< int > >::iterator mitGroupID = m_mapUnitClassGroupID.find( iClassGroup );
	if( mitGroupID == m_mapUnitClassGroupID.end() )
	{
		std::vector< int > vecTemp;
		vecTemp.push_back( iUnitClass );

		m_mapUnitClassGroupID.insert( std::make_pair( iClassGroup, vecTemp ) );
	}
	else
	{
		std::vector< int >::iterator vit = mitGroupID->second.begin();
		if( vit == mitGroupID->second.end() )
		{
			START_LOG( cerr, L"ClassGroupID 에 데이터가 있다고 했는데....비어있다. 다시 넣자!" )
				<< BUILD_LOG( iClassGroup )
				<< BUILD_LOG( iUnitClass )
				<< END_LOG;

			mitGroupID->second.push_back( iUnitClass );
		}
		else
		{
			bool bRet = true;
			for( ; vit != mitGroupID->second.end() ; ++vit )
			{
#ifdef _CONVERT_VS_2010
				if( (*vit) == iUnitClass )
				{
					START_LOG( cerr, L"ClassGroupID 에 데이터가 있다고 했는데....중복 되어 있다!" )
						<< BUILD_LOG( iClassGroup )
						<< BUILD_LOG( iUnitClass )
						<< BUILD_LOG( (*vit) )
						<< END_LOG;

					bRet = false;
					break;
				}
#else
				if( vit != NULL )
				{
					if( (*vit) == iUnitClass )
					{
						START_LOG( cerr, L"ClassGroupID 에 데이터가 있다고 했는데....중복 되어 있다!" )
							<< BUILD_LOG( iClassGroup )
							<< BUILD_LOG( iUnitClass )
							<< BUILD_LOG( (*vit) )
							<< END_LOG;

						bRet = false;
						break;
					}
				}
#endif _CONVERT_VS_2010
			}

			if( bRet == true )
			{
				mitGroupID->second.push_back( iUnitClass );
			}
		}
	}
}

// 스킬
void KClassChangeTable::SetClassChange_Skill_Lua( IN int iSkillGroup, IN int iSkill, IN int iUnitClass )
{
	if( iSkillGroup <= 0 || iSkill <= 0 || iUnitClass <= CXSLUnit::UC_NONE || iUnitClass >= CXSLUnit::UC_MAX )
	{
		START_LOG( cerr, L"직업 변경에 필요한 스킬관련 스크립트 파싱 실패!" )
			<< BUILD_LOG( iSkillGroup )
			<< BUILD_LOG( iSkill )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return;
	}

	// 스킬별 그룹id 삽입
	std::map< int, int >::iterator mitSkillGroupID = m_mapSkillGroupID.find( iSkill );
	if( mitSkillGroupID != m_mapSkillGroupID.end() )
	{
		START_LOG( clog, L"스킬 별 그룹 아이디 중복 파싱!!! 상위 직종은 하위 직종의 스킬을 가질 수 있으므로 오류가 아니다" )
			<< BUILD_LOG( iSkillGroup )
			<< BUILD_LOG( iSkill )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
	}
	else
	{
		m_mapSkillGroupID.insert( std::make_pair( iSkill, iSkillGroup ) );
	}

	// 실제 스킬 목록을 저장 할 컨테이너에 삽입
	std::map< int, std::map< int, int > >::iterator mit = m_mapClassChangeSkill.find( iSkillGroup );
	if( mit == m_mapClassChangeSkill.end() )
	{
		std::map< int, int > mapTemp;
		mapTemp.insert( std::make_pair( iUnitClass, iSkill ) );

		m_mapClassChangeSkill.insert( std::make_pair( iSkillGroup, mapTemp) );
	}
	else
	{
		std::map< int, int >::iterator mitGroup = mit->second.find( iUnitClass );
		if( mitGroup != mit->second.end() )
		{
			START_LOG( clog, L"직업 변경에 필요한 스킬관련 중복 파싱!" )
				<< BUILD_LOG( iSkillGroup )
				<< BUILD_LOG( iSkill )
				<< BUILD_LOG( iUnitClass )
				<< END_LOG;
			return;
		}

		mit->second.insert( std::make_pair( iUnitClass, iSkill ) );
	}
}

// 메모
void KClassChangeTable::SetClassChange_Memo_Lua( IN int iMemoGroup, IN int iMemo, IN int iUnitClass )
{
	if( iMemoGroup < 0 || iMemo <= 0 || iUnitClass <= CXSLUnit::UC_NONE || iUnitClass >= CXSLUnit::UC_MAX )
	{
		START_LOG( cerr, L"직업 변경에 필요한 메모관련 스크립트 파싱 실패!" )
			<< BUILD_LOG( iMemoGroup )
			<< BUILD_LOG( iMemo )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return;
	}

	// 메모별 그룹id 삽입
	std::map< int, int >::iterator mitMemoGroupID = m_mapMemoGroupID.find( iMemo );
	if( mitMemoGroupID != m_mapMemoGroupID.end() )
	{
		START_LOG( clog, L"메모 별 그룹 아이디 중복 파싱!!!" )
			<< BUILD_LOG( iMemoGroup )
			<< BUILD_LOG( iMemo )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
	}
	else
	{
		m_mapMemoGroupID.insert( std::make_pair( iMemo, iMemoGroup ) );
	}

	// 실제 메모 목록을 저장 할 컨테이너에 삽입
	std::map< int, std::map< int, int > >::iterator mit = m_mapClassChangeMemo.find( iMemoGroup );
	if( mit == m_mapClassChangeMemo.end() )
	{
		std::map< int, int > mapTemp;
		mapTemp.insert( std::make_pair( iUnitClass, iMemo ) );

		m_mapClassChangeMemo.insert( std::make_pair( iMemoGroup, mapTemp) );
	}
	else
	{
		std::map< int, int >::iterator mitGroup = mit->second.find( iUnitClass );
		if( mitGroup != mit->second.end() )
		{
			START_LOG( cerr, L"직업 변경에 필요한 메모관련 중복 파싱!" )
				<< BUILD_LOG( iMemoGroup )
				<< BUILD_LOG( iMemo )
				<< BUILD_LOG( iUnitClass )
				<< END_LOG;
			return;
		}

		mit->second.insert( std::make_pair( iUnitClass, iMemo ) );
	}
}

// 교본 및 캐쉬 아이템
void KClassChangeTable::SetClassChange_InventoryItem_Lua( IN int iItemGroup, IN int iItem, IN int iUnitClass )
{
	if( iItemGroup < 0 || iItem <= 0 || iUnitClass <= CXSLUnit::UC_NONE || iUnitClass >= CXSLUnit::UC_MAX )
	{
		START_LOG( cerr, L"직업 변경에 필요한 메모관련 스크립트 파싱 실패!" )
			<< BUILD_LOG( iItemGroup )
			<< BUILD_LOG( iItem )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return;
	}

	// 스킬별 그룹id 삽입
	std::map< int, int >::iterator mitItemGroupID = m_mapItemGroupID.find( iItem );
	if( mitItemGroupID != m_mapItemGroupID.end() )
	{
		START_LOG( clog, L"아이템 별 그룹 아이디 중복 파싱!!!" )
			<< BUILD_LOG( iItemGroup )
			<< BUILD_LOG( iItem )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
	}
	else
	{
		m_mapItemGroupID.insert( std::make_pair( iItem, iItemGroup ) );
	}

	// 실제 메모 목록을 저장 할 컨테이너에 삽입
	std::map< int, std::map< int, int > >::iterator mit = m_mapClassChangeItem.find( iItemGroup );
	if( mit == m_mapClassChangeItem.end() )
	{
		std::map< int, int > mapTemp;
		mapTemp.insert( std::make_pair( iUnitClass, iItem ) );

		m_mapClassChangeItem.insert( std::make_pair( iItemGroup, mapTemp) );
	}
	else
	{
		std::map< int, int >::iterator mitGroup = mit->second.find( iUnitClass );
		if( mitGroup != mit->second.end() )
		{
			START_LOG( cerr, L"직업 변경에 필요한 메모관련 중복 파싱!" )
				<< BUILD_LOG( iItemGroup )
				<< BUILD_LOG( iItem )
				<< BUILD_LOG( iUnitClass )
				<< END_LOG;
			return;
		}

		mit->second.insert( std::make_pair( iUnitClass, iItem ) );
	}
}

// 퀘스트
void KClassChangeTable::SetClassChange_Quest_Lua( IN int iQuestGroup, IN int iQuest, IN int iUnitClass )
{
	if( iQuestGroup <= 0 || iQuest <= 0 || iUnitClass <= CXSLUnit::UC_NONE || iUnitClass >= CXSLUnit::UC_MAX )
	{
		START_LOG( cerr, L"직업 변경에 필요한 메모관련 스크립트 파싱 실패!" )
			<< BUILD_LOG( iQuestGroup )
			<< BUILD_LOG( iQuest )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return;
	}

	// 스킬별 그룹id 삽입
	std::map< int, int >::iterator mitQuestGroupID = m_mapQuestGroupID.find( iQuest );
	if( mitQuestGroupID != m_mapQuestGroupID.end() )
	{
		START_LOG( clog, L"스킬 별 그룹 아이디 중복 파싱!!!" )
			<< BUILD_LOG( iQuestGroup )
			<< BUILD_LOG( iQuest )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
	}
	else
	{
		m_mapQuestGroupID.insert( std::make_pair( iQuest, iQuestGroup ) );
	}

	// 실제 메모 목록을 저장 할 컨테이너에 삽입
	std::map< int, std::map< int, int > >::iterator mit = m_mapClassChangeQuest.find( iQuestGroup );
	if( mit == m_mapClassChangeQuest.end() )
	{
		std::map< int, int > mapTemp;
		mapTemp.insert( std::make_pair( iUnitClass, iQuest ) );

		m_mapClassChangeQuest.insert( std::make_pair( iQuestGroup, mapTemp) );
	}
	else
	{
		std::map< int, int >::iterator mitGroup = mit->second.find( iUnitClass );
		if( mitGroup != mit->second.end() )
		{
			START_LOG( clog, L"직업 변경에 필요한 메모관련 중복 파싱!" )
				<< BUILD_LOG( iQuestGroup )
				<< BUILD_LOG( iQuest )
				<< BUILD_LOG( iUnitClass )
				<< END_LOG;
			return;
		}

		mit->second.insert( std::make_pair( iUnitClass, iQuest ) );
	}

	// UnitClass 별로 데이터를 추가 하자
	mit = m_mapClassChangeQuestList.find( iUnitClass );
	if( mit == m_mapClassChangeQuestList.end() )
	{
		std::map< int, int > mapTemp;
		mapTemp.insert( std::make_pair( iQuest, iQuestGroup ) );

		m_mapClassChangeQuestList.insert( std::make_pair( iUnitClass, mapTemp) );
	}
	else
	{
		std::map< int, int >::iterator mitGroup = mit->second.find( iQuest );
		if( mitGroup != mit->second.end() )
		{
			START_LOG( cerr, L"직업 변경에 필요한 메모관련 중복 파싱!" )
				<< BUILD_LOG( iQuestGroup )
				<< BUILD_LOG( iQuest )
				<< BUILD_LOG( iUnitClass )
				<< END_LOG;
			return;
		}

		mit->second.insert( std::make_pair( iQuest, iQuestGroup ) );
	}
}

void KClassChangeTable::SetClassChange_DelQuest_Lua( IN int iQuest )
{
	if( iQuest <= 0 )
	{
		START_LOG( cerr, L"잘못된 Quest 입니다." )
			<< BUILD_LOG( iQuest )
			<< END_LOG;
	}

	std::set<int>::iterator sit = m_setClassChangeDeleteQuest.find( iQuest );
	if( sit != m_setClassChangeDeleteQuest.end() )
	{
		START_LOG( cerr, L"중복된 Quest 입니다." )
			<< BUILD_LOG( iQuest )
			<< END_LOG;
		return;
	}

	m_setClassChangeDeleteQuest.insert( iQuest );
}

bool KClassChangeTable::GetClassGroupID( IN int& iClass, OUT int& iClassGroupID )
{
	iClassGroupID = 0;

	std::map< int, int >::iterator mit = m_mapClassGroupID.find( iClass );
	if( mit == m_mapClassGroupID.end() )
	{
		START_LOG( clog, L"해당 iClass에 해당하는 iClassGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iClass )
			<< BUILD_LOG( iClassGroupID )
			<< END_LOG;
		return false;
	}

	iClassGroupID = mit->second;
	return true;
}

bool KClassChangeTable::GetSkillGroupID( IN int& iSkill, OUT int& iSkillGroupID )
{
	iSkillGroupID = 0;

	std::map< int, int >::iterator mit = m_mapSkillGroupID.find( iSkill );
	if( mit == m_mapSkillGroupID.end() )
	{
		START_LOG( clog, L"해당 iSkill에 해당하는 iClassGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iSkill )
			<< BUILD_LOG( iSkillGroupID )
			<< END_LOG;
		return false;
	}

	iSkillGroupID = mit->second;
	return true;
}

bool KClassChangeTable::GetMemoGroupID( IN int& iMemo, OUT int& iMemoGroupID )
{
	iMemoGroupID = -1;

	std::map< int, int >::iterator mit = m_mapMemoGroupID.find( iMemo );
	if( mit == m_mapMemoGroupID.end() )
	{
		START_LOG( clog, L"해당 iMemo에 해당하는 iClassGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iMemo )
			<< BUILD_LOG( iMemoGroupID )
			<< END_LOG;
		return false;
	}

	iMemoGroupID = mit->second;
	return true;
}

bool KClassChangeTable::GetItemGroupID( IN int& iItem, OUT int& iItemGroupID )
{
	iItemGroupID = -1;

	std::map< int, int >::iterator mit = m_mapItemGroupID.find( iItem );
	if( mit == m_mapItemGroupID.end() )
	{
		START_LOG( clog, L"해당 iItem에 해당하는 iClassGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iItem )
			<< BUILD_LOG( iItemGroupID )
			<< END_LOG;
		return false;
	}

	iItemGroupID = mit->second;
	return true;
}

bool KClassChangeTable::GetQuestGroupID( IN int& iQuest, OUT int& iQuestGroupID )
{
	iQuestGroupID = 0;

	std::map< int, int >::iterator mit = m_mapQuestGroupID.find( iQuest );
	if( mit == m_mapQuestGroupID.end() )
	{
		START_LOG( clog, L"해당 iQuest에 해당하는 iClassGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iQuest )
			<< BUILD_LOG( iQuestGroupID )
			<< END_LOG;
		return false;
	}

	iQuestGroupID = mit->second;
	return true;
}

bool KClassChangeTable::GetUnitClassList( IN int& iClassGroupID, OUT std::vector< int >& vecClassList )
{
	vecClassList.clear();

	std::map< int, std::vector< int > >::iterator mit = m_mapUnitClassGroupID.find( iClassGroupID );
	if( mit == m_mapUnitClassGroupID.end() )
	{
		START_LOG( clog, L"해당 iClassGroupID에 해당하는 ClassList 가 존재하지 않는다!" )
			<< BUILD_LOG( iClassGroupID )
			<< END_LOG;
		return false;
	}

	vecClassList = mit->second;
	return true;
}

#ifdef _CONVERT_VS_2010
bool KClassChangeTable::GetClassChange_NewSkill( IN int& iNewUnitClass, IN int iOldSkill, OUT int& iNewSkill )
#else
bool KClassChangeTable::GetClassChange_NewSkill( IN int& iNewUnitClass, IN int& iOldSkill, OUT int& iNewSkill )
#endif _CONVERT_VS_2010
{
	iNewSkill = 0;

	int iOldSkillGroupID = 0;

	if( GetSkillGroupID( iOldSkill, iOldSkillGroupID ) == false )
	{
		START_LOG( cerr, L"해당 iSkill에 해당하는 iSkillGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldSkill )
			<< BUILD_LOG( iNewSkill )
			<< END_LOG;
		return false;
	}

	std::map< int, std::map< int, int > >::const_iterator mit = m_mapClassChangeSkill.find( iOldSkillGroupID );
	if( mit == m_mapClassChangeSkill.end() )
	{
		START_LOG( cerr, L"해당 iSkill에 해당하는 SkillList 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldSkill )
			<< BUILD_LOG( iNewSkill )
			<< END_LOG;

		return false;
	}

	std::map< int, int >::const_iterator mitList = mit->second.find( iNewUnitClass );
	if( mitList == mit->second.end() )
	{
		START_LOG( clog, L"해당 iNewUnitClass에 해당하는 NewSkill 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldSkill )
			<< BUILD_LOG( iNewSkill )
			<< END_LOG;

		return false;
	}

	iNewSkill = mitList->second;

	return true;
}

bool KClassChangeTable::GetClassChange_NewMemo( IN int& iNewUnitClass, IN int& iOldMemo, OUT int& iNewMemo )
{
	iNewMemo = 0;

	int iOldMemoGroupID = 0;

	if( GetMemoGroupID( iOldMemo, iOldMemoGroupID ) == false )
	{
		START_LOG( clog, L"해당 iClass에 해당하는 iClassGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldMemo )
			<< BUILD_LOG( iNewMemo )
			<< END_LOG;
		return false;
	}

	std::map< int, std::map< int, int > >::const_iterator mit = m_mapClassChangeMemo.find( iOldMemoGroupID );
	if( mit == m_mapClassChangeMemo.end() )
	{
		START_LOG( cerr, L"해당 iMemo에 해당하는 MemoList 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldMemo )
			<< BUILD_LOG( iNewMemo )
			<< END_LOG;

		return false;
	}

	std::map< int, int >::const_iterator mitList = mit->second.find( iNewUnitClass );
	if( mitList == mit->second.end() )
	{
		START_LOG( clog, L"해당 iNewUnitClass에 해당하는 NewMemo 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldMemo )
			<< BUILD_LOG( iNewMemo )
			<< END_LOG;

		return false;
	}

	iNewMemo = mitList->second;

	return true;
}

bool KClassChangeTable::GetClassChange_NewItem( IN int& iNewUnitClass, IN int& iOldItem, OUT int& iNewItem )
{
	iNewItem = 0;

	int iOldItemGroupID = 0;

	if( GetItemGroupID( iOldItem, iOldItemGroupID ) == false )
	{
		START_LOG( clog, L"해당 iClass에 해당하는 iClassGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldItem )
			<< BUILD_LOG( iNewItem )
			<< END_LOG;
		return false;
	}

	std::map< int, std::map< int, int > >::const_iterator mit = m_mapClassChangeItem.find( iOldItemGroupID );
	if( mit == m_mapClassChangeItem.end() )
	{
		START_LOG( cerr, L"해당 iItem에 해당하는 ItemList 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldItem )
			<< BUILD_LOG( iNewItem )
			<< END_LOG;

		return false;
	}

	std::map< int, int >::const_iterator mitList = mit->second.find( iNewUnitClass );
	if( mitList == mit->second.end() )
	{
		START_LOG( clog, L"해당 iNewUnitClass에 해당하는 NewItem 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldItem )
			<< BUILD_LOG( iNewItem )
			<< END_LOG;

		return false;
	}

	iNewItem = mitList->second;

	return true;
}

bool KClassChangeTable::GetClassChange_NewQuest( IN int& iNewUnitClass, IN int& iOldQuest, OUT int& iNewQuest )
{
	iNewQuest = 0;

	int iOldQuestGroupID = 0;

	if( GetQuestGroupID( iOldQuest, iOldQuestGroupID ) == false )
	{
		START_LOG( clog, L"해당 iClass에 해당하는 iClassGroupID 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldQuest )
			<< BUILD_LOG( iNewQuest )
			<< END_LOG;
		return false;
	}

	std::map< int, std::map< int, int > >::const_iterator mit = m_mapClassChangeQuest.find( iOldQuestGroupID );
	if( mit == m_mapClassChangeQuest.end() )
	{
		START_LOG( cerr, L"해당 iQuest에 해당하는 QuestList 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldQuest )
			<< BUILD_LOG( iNewQuest )
			<< END_LOG;

		return false;
	}

	std::map< int, int >::const_iterator mitList = mit->second.find( iNewUnitClass );
	if( mitList == mit->second.end() )
	{
		START_LOG( clog, L"해당 iNewUnitClass에 해당하는 NewQuest 가 존재하지 않는다!" )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iOldQuest )
			<< BUILD_LOG( iNewQuest )
			<< END_LOG;

		return false;
	}

	iNewQuest = mitList->second;

	return true;
}

bool KClassChangeTable::GetClassQuestList( IN int& iUnitClass, OUT std::map< int, int >& mapQuestList )
{
	mapQuestList.clear();

	if( iUnitClass <= CXSLUnit::UC_NONE || iUnitClass >= CXSLUnit::UC_MAX )
	{
		START_LOG( cerr, L"존재하지 않는 UnitClass 입니다." )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return false;
	}

	std::map< int, std::map< int, int > >::const_iterator mit = m_mapClassChangeQuestList.find( iUnitClass );
	if( mit == m_mapClassChangeQuestList.end() )
	{
		START_LOG( cerr, L"데이터가 없다? 존재하지 않는 UnitClass 인가? 아니면 스크립트가 빠져있나?" )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return false;
	}

	mapQuestList = mit->second;

	return true;
}

bool KClassChangeTable::CompareUnitClass( IN int& iNowUnitClass, IN int& iNewUnitClass )
{
	int iNowGroupID = 0;
	int iNewGroupID = 0;
	if( GetClassGroupID( iNowUnitClass, iNowGroupID ) == false )
	{
		START_LOG( cerr, L"UnitGroupID 가 존재하지 않는 iNowGroupID 이다." )
			<< BUILD_LOG( iNowUnitClass )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iNowGroupID )
			<< BUILD_LOG( iNewGroupID )
			<< END_LOG;

		return false;
	}
	if( GetClassGroupID( iNewUnitClass, iNewGroupID ) == false )
	{
		START_LOG( cerr, L"UnitGroupID 가 존재하지 않는 iNewGroupID 이다." )
			<< BUILD_LOG( iNowUnitClass )
			<< BUILD_LOG( iNewUnitClass )
			<< BUILD_LOG( iNowGroupID )
			<< BUILD_LOG( iNewGroupID )
			<< END_LOG;

		return false;
	}	
	
	//////////////////////////////////////////////////////////////////////////
	
	if( iNewUnitClass == CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT && 
		iNowGroupID != iNewGroupID &&
		CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>(iNowUnitClass) ) == CXSLUnit::UC_ELSWORD_SWORDMAN )
	{
		return true;
	}
	
	if( iNewUnitClass == CXSLUnit::UC_ARME_BATTLE_MAGICIAN && 
		iNowGroupID != iNewGroupID &&
		CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>(iNowUnitClass) ) == CXSLUnit::UC_ARME_VIOLET_MAGE )
	{
		return true;
	}

	if( iNewUnitClass == CXSLUnit::UC_LIRE_TRAPPING_RANGER && 
		iNowGroupID != iNewGroupID &&
		CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>(iNowUnitClass) ) == CXSLUnit::UC_LIRE_ELVEN_RANGER )
	{
		return true;
	}

	if( iNewUnitClass == CXSLUnit::UC_RAVEN_WEAPON_TAKER && 
		iNowGroupID != iNewGroupID &&
		CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>(iNowUnitClass) ) == CXSLUnit::UC_RAVEN_FIGHTER )
	{
		return true;
	}

	if( iNewUnitClass == CXSLUnit::UC_EVE_ELECTRA && 
		iNowGroupID != iNewGroupID &&
		CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>(iNowUnitClass) ) == CXSLUnit::UC_EVE_NASOD )
	{
		return true;
	}

	if( iNewUnitClass == CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN && 
		iNowGroupID != iNewGroupID &&
		CXSLUnit::GetUnitClassBaseGrade( static_cast<CXSLUnit::UNIT_CLASS>(iNowUnitClass) ) == CXSLUnit::UC_CHUNG_IRON_CANNON )
	{
		return true;
	}
	//////////////////////////////////////////////////////////////////////////

	return (iNowGroupID == iNewGroupID);
}





#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}
