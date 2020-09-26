/**	@file	: X2ViewerMain.cpp
	@desc	: 뷰어 메인 코드
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#include "dxstdafx.h"
#include ".\x2viewermain.h"

#pragma region CX2ViewerMain
CX2ViewerMain::CX2ViewerMain(void)
{
	CX2ViewerCamera*	pCamera		= new CX2ViewerCamera();
	CX2ViewerGrid*		pGrid		= new CX2ViewerGrid();
	CX2ViewerSkinMesh*	pSkinMesh	= new CX2ViewerSkinMesh();
	CX2ViewerMesh*		pMesh		= new CX2ViewerMesh();
	CX2ViewerWorldMesh*	pWorldMesh	= new CX2ViewerWorldMesh();
	CX2ViewerUI*		pUI			= new CX2ViewerUI( this );
#ifndef UPDATE_X2VIEWER_2013 //JHKang
	CX2ViewerParticle*	pParticle	= new CX2ViewerParticle( pSkinMesh );
#endif //UPDATE_X2VIEWER_2013
	
	m_vecObject.push_back( pCamera );
	m_vecObject.push_back( pGrid );
	m_vecObject.push_back( pWorldMesh );
	m_vecObject.push_back( pSkinMesh );
	m_vecObject.push_back( pMesh );
	m_vecObject.push_back( pUI );
#ifndef UPDATE_X2VIEWER_2013 //JHKang
	m_vecObject.push_back( pParticle );
#endif //UPDATE_X2VIEWER_2013

//	m_pWeaponMatrix = NULL;
	m_SelectedAnimIndex = -1;

	m_AnimFileName = L"";
	m_AnimFileDir = L"";

	g_pKTDXApp->GetDeviceManager()->ResumeThread();

	ReadAdditionalResourceFolder();
	RegisterLuabind( g_pKTDXApp->GetLuaBinder() );

	pUI->GetMotionFileList( m_AnimFileDir );
	pSkinMesh->OpenScriptFile( L"BoneEnum.lua", g_pKTDXApp->GetLuaBinder() );
	pSkinMesh->RefreshBoneInfoScript( g_pKTDXApp->GetLuaBinder() );

	g_pKTDXApp->SkipFrame();
}
#pragma endregion 생성자

#pragma region ~CX2ViewerMain
CX2ViewerMain::~CX2ViewerMain(void)
{
	for( int i = 0; i < (int)m_vecObject.size(); ++i )
	{
		SAFE_DELETE( m_vecObject[i] );
	}

	m_vecObject.clear();
}
#pragma endregion 소멸자

#pragma region OnFrameMove
HRESULT CX2ViewerMain::OnFrameMove( double fTime, float fElapsedTime )
{
	for( int i = 0; i < (int)m_vecObject.size(); ++i )
	{
		m_vecObject[i]->OnFrameMove( fTime, fElapsedTime );
	}

	D3DVIEWPORT9 Viewport;
	g_pKTDXApp->GetDevice()->GetViewport( &Viewport );

	return S_OK;
}
#pragma endregion 매 프레임 갱신

#pragma region OnFrameRender
HRESULT CX2ViewerMain::OnFrameRender()
{
	for( int i = 0; i < static_cast<int>( m_vecObject.size() ); ++i )
	{
		m_vecObject[i]->OnFrameRender();
	}

	return S_OK;
}
#pragma endregion 매 프레임 렌더링

#pragma region MsgProc
bool CX2ViewerMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	for( int i = 0; i < (int)m_vecObject.size(); ++i )
	{
		if( m_vecObject[i]->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

	return false;
}
#pragma endregion 메시지 처리

#pragma region OnResetDevice
HRESULT CX2ViewerMain::OnResetDevice()
{
	for( int i = 0; i < (int)m_vecObject.size(); ++i )
	{
		m_vecObject[i]->OnResetDevice();
	}

	return S_OK;
}
#pragma endregion 장치 재설정

#pragma region OnLostDevice
HRESULT CX2ViewerMain::OnLostDevice()
{
	for( int i = 0; i < (int)m_vecObject.size(); ++i )
	{
		m_vecObject[i]->OnLostDevice();
	}

	return S_OK;
}
#pragma endregion 장치 소실

#pragma region GetObject
CX2ViewerObject*  CX2ViewerMain::GetObject( CX2ViewerObject::OBJECT_STYLE objectStyle )
{

	for( int i = 0; i < (int)m_vecObject.size(); ++i )
	{
		if( m_vecObject[i] != NULL && m_vecObject[i]->GetObjectStyle() == objectStyle )
			return m_vecObject[i];
	}

	return NULL;
}
#pragma endregion OBJECT_STYLE을 통한 ViewObject 얻기

#pragma region ReadAdditionalResourceFolder
void CX2ViewerMain::ReadAdditionalResourceFolder()
{
	KLuaManager	luaManager(g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true);
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"AdditionalResourceDir.lua" );
	if( Info == NULL )
	{		
		return;
	}

	if( luaManager.DoMemory( Info->pRealData, Info->size ) == false )
	{		
		return;
	}

	if( luaManager.BeginTable( "ADDITIONAL_DIR" ) == true )
	{
		int tableIndex = 1;
		while( luaManager.BeginTable( tableIndex ) )
		{
			string dirName;

			bool bIncludeSubFolder;
			LUA_GET_VALUE( luaManager, 1, dirName, "" );
			LUA_GET_VALUE( luaManager, 2, bIncludeSubFolder, false );
		
			AddAdditionalResourceFolder( dirName, bIncludeSubFolder );
			
			tableIndex++;
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "MOTION_DIR" ) == true )
	{
		LUA_GET_VALUE( luaManager, 1, m_AnimFileDir, L"" );
		luaManager.EndTable();
	}
}
#pragma endregion AdditionalResourceDir.lua 파일 파싱

#pragma region AddAdditionalResourceFolder
void CX2ViewerMain::AddAdditionalResourceFolder( string folder, bool bIncludeSub )
{
	if( folder.empty() )
		return;
	
	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( folder );

	HANDLE				hSearch;
	WIN32_FIND_DATAA	fd;
	char				szSearchPath[256];

	strcpy(szSearchPath, folder.c_str());
	strcat(szSearchPath, "\\*.*");

	hSearch = FindFirstFileA( szSearchPath, &fd );

	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	if( bIncludeSub )
	{
		do
		{
			if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
			{
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					char	szNewSearchDir[256];
					string	strDir;

					strcpy(szNewSearchDir, folder.c_str());
					strcat(szNewSearchDir, "\\");
					strcat(szNewSearchDir, fd.cFileName);

					//#ifndef _SERVICE_
					if( strcmp( fd.cFileName, ".svn" ) != 0 &&
						strcmp( fd.cFileName, "Branches" ) != 0)
						//#endif _SERVICE_
					{
						strDir = szNewSearchDir;
						strDir += "\\";
						AddAdditionalResourceFolder( strDir, bIncludeSub );
					}
				}
			}
		} while( FindNextFileA( hSearch, &fd ) );
	}	

	FindClose(hSearch);

	return;
}
#pragma endregion 리소스 추가

#pragma region RegisterLuabind
void CX2ViewerMain::RegisterLuabind( KLuabinder* pKLuabinder )
{
	lua_State* L= pKLuabinder->GetLuaState();
	
	lua_tinker::class_add<CX2ViewerSkinMesh>( L, "CX2ViewerSkinMesh" );
	lua_tinker::class_def<CX2ViewerSkinMesh>( L, "AddBoneInfo",	&CX2ViewerSkinMesh::AddBoneInfo );

	lua_tinker::class_add<CX2ViewerUI>( L, "CX2ViewerUI" );
	lua_tinker::class_def<CX2ViewerUI>( L, "AddEquip",	&CX2ViewerUI::AddEquip );
	lua_tinker::class_def<CX2ViewerUI>( L, "AddWeapon",	&CX2ViewerUI::AddWeapon );
	lua_tinker::class_def<CX2ViewerUI>( L, "AddAccessory",	&CX2ViewerUI::AddAccessory );
}
#pragma endregion 