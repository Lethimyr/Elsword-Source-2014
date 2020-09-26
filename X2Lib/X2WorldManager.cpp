#include "stdafx.h"
#include ".\x2worldmanager.h"


#ifdef KHELP_SYSTEM_TEST // 2008-10-24
IMPLEMENT_HELP_SYSTEM( CX2WorldManager )
{
    WCHAR   wbuffer[1024];
    int     ix = 16;
    int     iy = 16;
    StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer), L"%S", __FUNCTION__ );

    if ( g_pMain && g_pData )
    {
	    g_pMain->GetFontForDebuf()->OutTextXY( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
		    CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
        g_pData->GetWorldManager()->Debug_RenderHelp( ix, iy );
    }//if
}//IMPLEMENT_HELP_SYSTEM( CX2WorldManager )
#endif // KHELP_SYSTEM_TEST // 2008-10-24


///-----------------------------------------------------------------------------
//
// constructor.
CX2WorldManager::CX2WorldManager()
{
	::InitializeCriticalSection( &m_csWorldManager );
    m_pX2WorldLatestRef         = NULL;
}//CX2WorldManager::CX2WorldManager()


// destructor.
CX2WorldManager::~CX2WorldManager(void)
{
	::EnterCriticalSection( &m_csWorldManager );

	for( int i = 0; i < (int)m_WorldDataList.size(); i++ )
	{
		CX2World::WorldData* pWorldData = m_WorldDataList[i];
		SAFE_DELETE( pWorldData );
	}
	m_WorldDataList.clear();

	::LeaveCriticalSection( &m_csWorldManager );

	::DeleteCriticalSection( &m_csWorldManager );
}//CX2WorldManager::~CX2WorldManager()

#ifdef X2TOOL
CX2World* CX2WorldManager::CreateWorld( wstring wstrWorldName, KLuabinder* pKLuabinder )
{
	CX2World::WorldData worldData;
	worldData.dataFileName = wstrWorldName;
	CX2World* pWorld = new CX2World( &worldData );
	pWorld->OpenScriptFile( wstrWorldName.c_str() );

	return pWorld;
}
#endif

CX2World* CX2WorldManager::CreateWorld( const CX2World::WORLD_ID worldID, KLuabinder* pKLuabinder /*= NULL*/, const bool bBackgroundLoad_ /*= false*/ )
{
	CX2World::WorldData* pWorldData = NULL;

	{
		CSLock locker( m_csWorldManager );

		for( UINT i = 0; i < m_WorldDataList.size(); i++ )
		{
			if( worldID == m_WorldDataList[i]->worldID )
			{
	#ifdef FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
				g_pKTDXApp->GetDSManager()->SetListenerData( D3DXVECTOR3( 99999.f, 99999.f, 99999.f ) );
	#endif //FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
				pWorldData = m_WorldDataList[i];
				break;
			}
		}//for
	}

	// WorldData가 없으면
	if ( NULL == pWorldData )
		return NULL;
	else // WorldData가 있으면
	{
		CX2World* pCX2World = new CX2World( pWorldData, bBackgroundLoad_ );
		ASSERT( pCX2World );

		m_pX2WorldLatestRef = pCX2World; // 가장 최근에 생성한 월드 객체에 대한 참고 포인터를 저장해 둔다.

		pCX2World->OpenScriptFile( pWorldData->dataFileName.c_str(), pKLuabinder );

#ifdef X2OPTIMIZE_GAMEOPTION_BUGFIX
		pCX2World->SetMapDetail( g_pMain->GetGameOption().GetOptionList().m_MapDetail );
#endif//X2OPTIMIZE_GAMEOPTION_BUGFIX

		return pCX2World;
	}
}//CX2WorldManager::CreateWorld()

bool CX2WorldManager::OpenScriptFile( const WCHAR* pFileName )
{
    ASSERT( pFileName );
    CSLock locker( m_csWorldManager );

	//바인더에 등록
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pWorldManager", this );

    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR72, pFileName );

		return false;
    }

	return true;
}


//{{ seojt // 2008-10-22, 10:33
void CX2WorldManager::OnPreDestroyWorld(CX2World* pWorld_)
{
    ASSERT( pWorld_ );
    /** 어떤 월드 객체가 파괴되면, 가장 최근에 생성한 월드 객체에 대한 참고 포인터를 무효화시킨다.
        CX2WorldManager가 객체를 생성하는 일은 담당하지만, 객체를 파괴하는 일은 맡고 있지
        않으므로, 이 부분을 수정해야 할 것이다. - jintaeks on 2008-10-22, 10:30
    
        @수정 안:
        
            step 1) CX2WorldManager는 자신이 생성한 모든 게임 월드 객체를 리스트에 유지한다.

            step 2) CX2World가 직접 delete되지 않는 다는 것을 보장하기 위해 CX2World의 파괴자
                    ~CX2World()를 private로 선언하고, CX2WorldManager에 CX2World 객체를 파괴하는
                    헬퍼 함수를 구현한다. CX2WorldManager는 CX2World의 private 파괴자에 접근할 수
                    있어야 하므로, CX2World에서 friend로 선언되어야 할 것이다.

            step 3) 각 게임 스테이트는 CX2World를 직접 파괴하지 않고 - 현재 직접 SAFE_DELETE()를 호출한다 -
                    CX2WorldManager에게 CX2World::WORLD_ID에 해당하는 월드를 파괴할 것을 요청한다.

            요약) CX2WorldManager가 객체를 생성하는 factory의 역할을 하므로, 객체의 파괴와 관리도 담당시켜서
                    완전한 factory가 되도록 해야 할 것이다.

        - jintaeks on 2008-10-22, 10:26 */

    m_pX2WorldLatestRef = NULL;

    // 현재 월드에서 요청한 리소스 정보를 모두 clear한다.
    ASSERT( g_pKTDXApp && g_pKTDXApp->GetDeviceManager() );
}//CX2WorldManager::OnPreDestroyWorld()
//}} seojt // 2008-10-22, 10:33



const CX2World::WorldData* CX2WorldManager::GetWorldData( CX2World::WORLD_ID eWorldID )
{
	for( UINT i=0; i<m_WorldDataList.size(); i++ )
	{
		CX2World::WorldData* pWorldData = m_WorldDataList[i];
		if( pWorldData->worldID == eWorldID )
		{
			return pWorldData;
		}
	}

	return NULL;
}


bool CX2WorldManager::AddWorldData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	CX2World::WorldData* pWorldData = new CX2World::WorldData;
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "worldID",			pWorldData->worldID,		CX2World::WORLD_ID,	CX2World::WI_NONE,	SAFE_DELETE(pWorldData); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, "worldName",		pWorldData->worldName,		L"",										SAFE_DELETE(pWorldData); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, "dataFileName",	pWorldData->dataFileName,	L"",										SAFE_DELETE(pWorldData); return false; );

	m_WorldDataList.push_back( pWorldData );
	return true;
}


//{{ seojt // 2008-10-21, 15:14
bool CX2WorldManager::SetPreprocessingData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	WORLD_PREPROCESSING_INFO    preInfo;

    // qfff
    preInfo.m_eInfoType = X2WM_PPIT_UNKNOWN;
	LUA_GET_VALUE( luaManager, "meshName",             preInfo.m_wstrName,         L"" );
	//LUA_GET_VALUE( luaManager, "centerPos",		    preInfo.m_vCenter,          D3DXVECTOR3(0,0,0) );
	LUA_GET_VALUE( luaManager, "boundingSphereRadius",	preInfo.m_fBSphereRadiua,  0.f );
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "centerPos", preInfo.m_vCenter, D3DXVECTOR3(0,0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    preInfo.m_vCenter = lua_tinker::get<D3DXVECTOR3>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "centerPos" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


    ///-----------------------------------------------------------------------------
    // 현재 생성된 CX2World*를 얻는다.
    // 이름에 해당하는 게임 객체를 찾아서 바운딩 구 정보를 설정한다.
    //
    if ( m_pX2WorldLatestRef )
    {
        CX2WorldObject* pWorldObject = m_pX2WorldLatestRef->FindObjectByName( preInfo.m_wstrName );
        if ( pWorldObject )
        {
            pWorldObject->SetBoundingRadius( preInfo.m_fBSphereRadiua );
            pWorldObject->SetCenter( preInfo.m_vCenter );
        }//if
    }//if

	return true;
}//CX2WorldManager::SetPreprocessingData_LUA()
//}} seojt // 2008-10-21, 15:14


CX2World::WORLD_ID CX2WorldManager::GetWorldIDUsingLineMapFileName( const WCHAR* pLineMapFileName )
{
	CSLock locker( m_csWorldManager );

	for( UINT i=0; i<m_WorldDataList.size(); i++ )
	{
		CX2World::WorldData* pWorldData = m_WorldDataList[i];
		CX2World* pWorld = new CX2World( pWorldData );

		lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pWorld", pWorld );

		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pWorldData->dataFileName );
		if( Info == NULL )
			continue;

		wstring wstrWorldScript;
		ConvertCharToWCHAR( wstrWorldScript, Info->pRealData, Info->size );

		int iIndex		= wstrWorldScript.find( L"g_pWorld:SetLineMap" );
		int iStartIndex = iIndex;
		iStartIndex		= wstrWorldScript.find( L"\"", iStartIndex );
		int iEndIndex	= iStartIndex+1;
		iEndIndex		= wstrWorldScript.find( L"\"", iEndIndex );

		wstring wstrLineMapFileName = wstrWorldScript.substr( iStartIndex+1, iEndIndex-iStartIndex-1 );
		MakeUpperCase( wstrLineMapFileName );
		wstring wstrInputLineMapFileName = pLineMapFileName;
		MakeUpperCase( wstrInputLineMapFileName );
		if( 0 == wstrLineMapFileName.compare( wstrInputLineMapFileName.c_str() ) )
		{
			SAFE_DELETE( pWorld );
			return pWorldData->worldID;
		}
		SAFE_DELETE( pWorld );
	}

	return CX2World::WI_NONE;
}

// #ifdef BACKGROUND_LOADING_TEST // 2008-10-18
// CX2World* CX2WorldManager::ThreadRequest_CreateWorld(CX2World::WORLD_ID worldID_, KLuabinder* pKLuabinder_ /*=NULL*/)
// {
//     /** EnableThreadLoading( true )의 의미:
// 
//         EnableThreadLoading( true )로 설정되면, CreateWorld()내부에서 읽어 들인 lua 스크립트의
//         메시 로드는 실제 로드가 아니라, 쓰레드 큐잉한다.
//         - jintaeks on 2008-10-24, 10:38 */
// 
//     ASSERT( g_pKTDXApp );
//     g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( true );
//     CX2World*   pWorld = CreateWorld( worldID_, pKLuabinder_ );
//     ASSERT( pWorld );
//     g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( false );
// 
//     return  pWorld;
// }//CX2WorldManager::ThreadRequest_CreateWorld()
// #endif // BACKGROUND_LOADING_TEST // 2008-10-18


void CX2WorldManager::Debug_RenderHelp( int ix_, int iy_ )
{
    int     ix = ix_ + 16;
    int     iy = iy_ + 16;
    WCHAR   wbuffer[1024];

    if ( m_pX2WorldLatestRef )
    {
        const CX2World::WorldData*  pWorldData = m_pX2WorldLatestRef->GetWorldData();
        if ( pWorldData )
        {
            StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer),
                L"WorldID = %i\n"
                L"WorldName = %s\n"
                L"DataFileName = %s\n",
                pWorldData->worldID,
                pWorldData->worldName.c_str(),
                pWorldData->dataFileName.c_str() );
	        g_pMain->GetFontForDebuf()->OutTextMultiline( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
		        CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
            iy += 16 * 3;
        }//if
    }//if
}//CX2WorldManager::Debug_RenderHelp()

