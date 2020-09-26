#pragma once

/**
	@file	X2EnumManager.h
	@author	Wonpok
	@brief	CX2EnumManager를 가지는 헤더 파일
		
	@date	JHKang on 2013/02/21,
			- 기능 파악
			- 수정 작업( 주석처리, 블럭 나누기, 가로 넓이 지정 )
			- 클래스명 변경 ( KX2EnumManager --> CX2EnumManager )
*/

#pragma region CX2EnumManager
/** @class : CX2EnumManager
	@brief : World, NPC의 EnumStirng을 파싱, 문제가 있을 경우 경고
	@date  : 2013/02/21
*/
class CX2EnumManager
{
	#pragma region public
public:
	CX2EnumManager(void);
	virtual ~CX2EnumManager(void);

	bool OpenScriptFile( const WCHAR* pFileName );
	bool AddWorldEnumData_LUA();
	bool AddNpcEnumData_LUA();

	std::wstring& GetWorldEnumString( CX2World::WORLD_ID worldID );
	std::wstring& GetNpcEnumString( CX2UnitManager::NPC_UNIT_ID npcID );
	CX2UnitManager::NPC_UNIT_ID GetNpcEnum( wstring wstrNpcEnumString );
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring > GetMapNpcEnum() { return m_mapNpcEnum; }

	#pragma endregion function

	#pragma region private
private:
	std::map< CX2World::WORLD_ID, std::wstring >			m_mapWorldEnum;			/// World map ( Key : WorldID, Value : string ) 
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >	m_mapNpcEnum;			/// NPC map ( Key : NPC UID, Value : string )
	std::map< std::wstring, CX2UnitManager::NPC_UNIT_ID >	m_mapNpcEnumString;		/// NPC map ( Key : string, Value : NPC_UID )
	std::wstring m_errorStr;

	typedef std::map< CX2World::WORLD_ID, std::wstring >::iterator m_iterWorldEnum;			/// World map 반복자
	typedef std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator m_iterNpcEnum;	/// NPC map 반복자

	#pragma endregion function
};
#pragma endregion 