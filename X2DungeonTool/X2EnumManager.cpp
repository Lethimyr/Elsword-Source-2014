#include "dxstdafx.h"
#include ".\x2enummanager.h"

KX2EnumManager::KX2EnumManager(void)
{	
}

KX2EnumManager::~KX2EnumManager(void)
{
}

bool KX2EnumManager::OpenScriptFile( const WCHAR* pFileName )
{
	//바인더에 등록
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pEnumManager", this );

	//파일 로드
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		ErrorLogMsg( XEM_ERROR0, strFileName.c_str() );

		return false;
	}

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		ErrorLogMsg( XEM_ERROR72, strFileName.c_str() );

		return false;
	}

	return true;
}

bool KX2EnumManager::AddWorldEnumData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
	
	std::wstring		 wsWorldEnum;
	CX2World::WORLD_ID	 worldID;

	LUA_GET_VALUE_ENUM(	luaManager, L"ENUM_ID",			worldID,		CX2World::WORLD_ID,	CX2World::WI_NONE );
	LUA_GET_VALUE(		luaManager, L"ENUM_STRING",		wsWorldEnum,	L"" );
	
	m_mapWorldEnum.insert( std::make_pair( worldID, wsWorldEnum ) );
	return true;
}

bool KX2EnumManager::AddNpcEnumData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	std::wstring				 wsNpcEnum;
	CX2UnitManager::NPC_UNIT_ID	 npcID;

	LUA_GET_VALUE_ENUM(	luaManager, L"ENUM_ID",			npcID,		CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NPC_UNIT_ID::NUI_NONE );
	LUA_GET_VALUE(		luaManager, L"ENUM_STRING",		wsNpcEnum,	L"" );

	m_mapNpcEnum.insert( std::make_pair( npcID, wsNpcEnum ) );
	return true;
}

std::wstring& KX2EnumManager::GetWorldEnumString( CX2World::WORLD_ID worldID )
{
	WCHAR buf[128] = L"";
	WorldEnumIterator mit = m_mapWorldEnum.find( worldID );
	if( mit == m_mapWorldEnum.end() )
	{
		wsprintf( buf, L"WOLRD_ID[%d]의 문자열이 등록되지 않았음", (int)worldID );
		return m_errorStr = std::wstring( buf );
	}

	return mit->second;
}

std::wstring& KX2EnumManager::GetNpcEnumString( CX2UnitManager::NPC_UNIT_ID npcID )
{
	WCHAR buf[128] = L"";
	NpcEnumIterator mit = m_mapNpcEnum.find( npcID );
	if( mit == m_mapNpcEnum.end() )
	{
		wsprintf( buf, L"NPC_UNIT_ID[%d]의 문자열이 등록되지 않았음", (int)npcID );
		return m_errorStr = std::wstring( buf );
	}

	return mit->second;
}