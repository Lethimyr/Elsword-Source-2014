#include "stdafx.h"

#include "X2UIUnitManager.h"

CX2UIUnitManager::CX2UIUnitManager()
{
	Initialize();
}

CX2UIUnitManager::~CX2UIUnitManager()
{
	ClearAllUIUnit();
}

void CX2UIUnitManager::Initialize()
{
	CreateUIUnit( CX2UnitManager::NUI_ANNE );	
}

CX2UIUnitPtr CX2UIUnitManager::CreateUIUnit( int iUnitID )
{
	const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = 
		g_pData->GetUnitManager()->GetNPCUnitTemplet( (CX2UnitManager::NPC_UNIT_ID) iUnitID );

	if( pNPCTemplet == NULL )
		return CX2UIUnitPtr();

	ASSERT( false == pNPCTemplet->m_LuaFileNameUI.empty() );
	CX2UIUnitPtr pkUnit = CX2UIUnit::CreateUIUnit( pNPCTemplet->m_LuaFileNameUI.c_str() );
	ASSERT( pkUnit != NULL );
	m_mapUIUnit[iUnitID] = pkUnit;

	return pkUnit;
}

CX2UIUnit* CX2UIUnitManager::GetUIUnit( int iUnitID )
{
	CX2UIUnitPtr pkUnit;

	std::map< int, CX2UIUnitPtr >::iterator iU = m_mapUIUnit.find( iUnitID );
	if ( iU == m_mapUIUnit.end() )
	{
		pkUnit = CreateUIUnit( iUnitID );
	}
	else
	{
		pkUnit = (*iU).second;
	}

	return pkUnit.get();
}


void CX2UIUnitManager::ClearAllUIUnit()
{
	m_mapUIUnit.clear();
}
