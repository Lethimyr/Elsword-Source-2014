#pragma once

class KX2DTNpcScaleManager
{
public:
	KX2DTNpcScaleManager(void);
	virtual ~KX2DTNpcScaleManager(void);

	bool LoadScaleFromScriptFile( const WCHAR* pFileName, CX2UnitManager::NPC_UNIT_ID npcID );
	float GetNpcScale ( CX2UnitManager::NPC_UNIT_ID npcID );

private:
	KLuaManager				m_luaManager;
	std::map< CX2UnitManager::NPC_UNIT_ID, float >		m_mapNpcScale;
	typedef std::map< CX2UnitManager::NPC_UNIT_ID, float >::iterator NpcScaleIterator;
};