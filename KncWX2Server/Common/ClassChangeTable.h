#pragma once

#include "ServerDefine.h"
#include "RefreshSingleton.h"
#include <map>
#include <vector>
//#include "ServerDefine.h"


//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM

class KClassChangeTable
{
	DeclareRefreshSingleton( KClassChangeTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	KClassChangeTable();
	~KClassChangeTable();

// 스크립트 파싱 함수
	// 직업별 그룹id 등록
	void SetClassGroupID_Lua( IN int iClassGroup, IN int iUnitClass );

	// 장착스킬 맵핑
	void SetClassChange_Skill_Lua( IN int iSkillGroup, IN int iSkill, IN int iUnitClass );
	
	// 장착메모 맵핑
	void SetClassChange_Memo_Lua( IN int iMemoGroup, IN int iMemo, IN int iUnitClass );
	
	// 인벤토리 내 교본, 캐쉬메모 맵핑
	void SetClassChange_InventoryItem_Lua( IN int iItemGroup, IN int iItem, IN int iUnitClass );
	
	// 수행 중 퀘스트 맵핑
	void SetClassChange_Quest_Lua( IN int iQuestGroup, IN int iQuest, IN int iUnitClass );
	
	// 삭제 해야 할 퀘스트 맵핑
	void SetClassChange_DelQuest_Lua( IN int iQuest );

//////////////////////////////////////////////////////////////////////////
	bool GetClassGroupID( IN int& iClass, OUT int& iClassGroupID );
	bool GetSkillGroupID( IN int& iSkill, OUT int& iSkillGroupID );
	bool GetMemoGroupID( IN int& iMemo, OUT int& iMemoGroupID );
	bool GetItemGroupID( IN int& iItem, OUT int& iItemGroupID );
	bool GetQuestGroupID( IN int& iQuest, OUT int& iQuestGroupID );

	bool GetUnitClassList( IN int& iClassGroupID, OUT std::vector< int >& vecClassList );
#ifdef _CONVERT_VS_2010
	bool GetClassChange_NewSkill( IN int& iNewUnitClass, IN int iOldSkill, OUT int& iNewSkill );
#else
	bool GetClassChange_NewSkill( IN int& iNewUnitClass, IN int& iOldSkill, OUT int& iNewSkill );
#endif _CONVERT_VS_2010
	bool GetClassChange_NewMemo( IN int& iNewUnitClass, IN int& iOldMemo, OUT int& iNewMemo );
	bool GetClassChange_NewItem( IN int& iNewUnitClass, IN int& iOldItem, OUT int& iNewItem );
	bool GetClassChange_NewQuest( IN int& iNewUnitClass, IN int& iOldQuest, OUT int& iNewQuest );

	bool GetClassQuestList( IN int& iUnitClass, OUT std::map< int, int >& mapQuestList );

	bool CompareUnitClass( IN int& iNowUnitClass, IN int& iNewUnitClass );

	void GetClassChangeDeleteQuest( OUT std::set<int>& setQuest )	{	setQuest = m_setClassChangeDeleteQuest;	}

private:
	// ClassChange 가능한 직업을 모아둠
	std::map< int, int >								m_mapClassGroupID;			// std::map< iClass, iClassGroup >
	std::map< int, std::vector< int > >					m_mapUnitClassGroupID;		// std::map< iClassGroup, std::vector< iClass > >

	// 장착스킬 맵핑
	std::map< int, int >								m_mapSkillGroupID;			// std::map< iSkill, iClassGroup >
	std::map< int, std::map< int, int > >				m_mapClassChangeSkill;		// std::map< iSkillGroup, std::map< iUnitClass, iSkill >>

	// 장착메모 맵핑
	std::map< int, int >								m_mapMemoGroupID;			// std::map< iMemo, iClassGroup >
	std::map< int, std::map< int, int > >				m_mapClassChangeMemo;		// std::map< iMemoGroup, std::map< iUnitClass, iMemo >>

	// 인벤토리 내 item 맵핑
	std::map< int, int >								m_mapItemGroupID;			// std::map< iItem, iClassGroup >
	std::map< int, std::map< int, int > >				m_mapClassChangeItem;		// std::map< iItemGroup, std::map< iUnitClass, iItem >>

	// 수행 중 퀘스트 맵핑
	std::map< int, int >								m_mapQuestGroupID;			// std::map< iQuest, iClassGroup >
	std::map< int, std::map< int, int > >				m_mapClassChangeQuest;		// std::map< iQuestGroup, std::map< iUnitClass, iQuest >>
	std::map< int, std::map< int, int > >				m_mapClassChangeQuestList;	// std::map< iUnitClass, std::map< iQuest, iQuestGroup >>

	// 삭제 해야 할 퀘스트 맵핑
	std::set< int >										m_setClassChangeDeleteQuest; // 삭제가 필요한 퀘스트
	
};

DefRefreshSingletonInline( KClassChangeTable );

#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}