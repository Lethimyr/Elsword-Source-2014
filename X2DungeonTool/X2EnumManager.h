#pragma once

class KX2EnumManager
{
public:
	KX2EnumManager(void);
	virtual ~KX2EnumManager(void);

	bool OpenScriptFile( const WCHAR* pFileName );
	bool AddWorldEnumData_LUA();
	bool AddNpcEnumData_LUA();

	std::wstring& GetWorldEnumString( CX2World::WORLD_ID worldID );
	std::wstring& GetNpcEnumString( CX2UnitManager::NPC_UNIT_ID npcID );

private:
	std::map< CX2World::WORLD_ID, std::wstring >			m_mapWorldEnum;
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >	m_mapNpcEnum;
	std::wstring m_errorStr;

	typedef std::map< CX2World::WORLD_ID, std::wstring >::iterator WorldEnumIterator;
	typedef std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator NpcEnumIterator;
};