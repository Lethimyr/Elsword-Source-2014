#include "dxstdafx.h"
#include ".\x2dtnpcscalemanager.h"

KX2DTNpcScaleManager::KX2DTNpcScaleManager(void)
{
	m_mapNpcScale.clear();
}

KX2DTNpcScaleManager::~KX2DTNpcScaleManager(void)
{
}

bool KX2DTNpcScaleManager::LoadScaleFromScriptFile( const WCHAR* pFileName, CX2UnitManager::NPC_UNIT_ID npcID )
{
	float scale = 1.0f;

	//스크립트 파일을 로드한다
	g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( pFileName );
	g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &m_luaManager, L"Enum.lua" );
	g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &m_luaManager, L"Unit_Common_Device.lua" );
	g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &m_luaManager, pFileName );

	if( m_luaManager.BeginTable( "INIT_SYSTEM" ) == true )
	{
		LUA_GET_VALUE( m_luaManager, "UNIT_SCALE", scale, 1.0f );

		m_luaManager.EndTable();
	}

	if( scale != 1.0f )
		m_mapNpcScale.insert( std::make_pair( npcID, scale ) );

	return true;
}

float KX2DTNpcScaleManager::GetNpcScale( CX2UnitManager::NPC_UNIT_ID npcID )
{
	NpcScaleIterator mit = m_mapNpcScale.find( npcID );
	if( mit == m_mapNpcScale.end() )
		return 1.0f;

	return mit->second;
}