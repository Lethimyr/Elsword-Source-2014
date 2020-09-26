#include "dxstdafx.h"
#include ".\x2dtworldmanager.h"

KX2DTWorldManager::KX2DTWorldManager(void)
{
}

KX2DTWorldManager::~KX2DTWorldManager(void)
{
}

bool KX2DTWorldManager::OpenScriptFile( const WCHAR* pFileName )
{
	//바인더에 등록
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pWorldManager", this );

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

KX2DTWorld* KX2DTWorldManager::CreateWorld( CX2World::WORLD_ID worldID )
{
	for( int i = 0; i < (int)m_WorldDataList.size(); i++ )
	{
		CX2World::WorldData* pWorldData = m_WorldDataList[i];
		if( pWorldData->worldID == worldID )
		{
			KX2DTWorld* pCX2World = new KX2DTWorld( pWorldData );
			pCX2World->OpenScriptFile( pWorldData->dataFileName.c_str() );

			return pCX2World;
		}
	}

	return NULL;
}

void KX2DTWorldManager::GetWorldNameList( std::map< CX2World::WORLD_ID, std::wstring >& mapWorldNameList )
{
	mapWorldNameList.clear();
	std::vector< CX2World::WorldData*>::iterator mit = m_WorldDataList.begin();
	for( ; mit != m_WorldDataList.end(); mit++ )
	{
		CX2World::WorldData* pWorldData = *mit;
		mapWorldNameList.insert( std::make_pair( pWorldData->worldID, pWorldData->worldName ) );
	}
}