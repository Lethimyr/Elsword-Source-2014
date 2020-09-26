
// X2ToolView.cpp : CX2ToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "X2Tool.h"
#endif

#include "X2ToolDoc.h"
#include "X2ToolView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPDIRECT3DDEVICE9		g_pd3dDevice;
LPDIRECT3D9				g_pD3D;
CX2SMTPMail*			g_pX2SMTPMail = NULL;
CKTDGEffectStateManager* g_pEffectStateManager = NULL;

static bool g_bRenderEtc = false;	// toggle fullscreen, 따위의 버튼을 렌더링 할지 말지

static float	g_fLastUpdateTime_MemInfo = 10000.f;
DWORD			g_iAvailableTextureMem = 0;
DWORD			g_iTotalTextureMem = 0;
DWORD			g_iAvailableTextureMem2 = 0;
MEMORYSTATUS	g_MemStatus;

double			g_fTotalFrame = 0.0f;
int				g_RenderCount = 0;

HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void	CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

void    InitApp();
void    RenderText(float fElapsedTime);

void InitApp()
{
	// Initialize dialogs
#ifndef  REMOVE_DXUT_SAMPLE_CODE
	g_SettingsDlg.Init( &g_DialogResourceManager );
	g_HUD.Init( &g_DialogResourceManager );
	g_SampleUI.Init( &g_DialogResourceManager );

	g_HUD.SetCallback( OnGUIEvent ); int iY = 10; 
	g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
	g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
	g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22 );

	g_SampleUI.SetCallback( OnGUIEvent ); iY = 10; 
#endif  REMOVE_DXUT_SAMPLE_CODE
}

HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	HRESULT hr;
#endif
	// Initialize the font

	g_pd3dDevice = pd3dDevice;
	g_pD3D = DXUTGetD3DObject();

	//{{ robobeg : 2008-10-10
	g_pEffectStateManager = new CKTDGEffectStateManager;
	ASSERT( g_pEffectStateManager != NULL );
	//}} robobeg : 2008-10-10


	g_pKTDXApp	= new CKTDXApp( DXUTGetHWND(), DXUTGetHINSTANCE(), pd3dDevice );
	//g_pMain		= new CX2Main();
	CX2ToolMain* pStage = new CX2ToolMain;
	g_pMain = (CX2Main*)pStage;
	g_pKTDXApp->InsertStage( pStage );	


#ifndef REMOVE_DXUT_SAMPLE_CODE
	V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
	V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
#endif

	return S_OK;
}

HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	HRESULT hr;
#endif

#ifndef REMOVE_DXUT_SAMPLE_CODE
	V_RETURN( g_DialogResourceManager.OnResetDevice() );
	V_RETURN( g_SettingsDlg.OnResetDevice() );
#endif  REMOVE_DXUT_SAMPLE_CODE

	g_pKTDXApp->OnResetDevice( pBackBufferSurfaceDesc );

	return S_OK;
}

void CALLBACK OnLostDevice( void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	g_DialogResourceManager.OnLostDevice();
	g_SettingsDlg.OnLostDevice();
#endif  REMOVE_DXUT_SAMPLE_CODE

	g_pKTDXApp->OnLostDevice();
}

void CALLBACK OnDestroyDevice( void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	g_DialogResourceManager.OnDestroyDevice();
	g_SettingsDlg.OnDestroyDevice();
#endif  REMOVE_DXUT_SAMPLE_CODE

	SAFE_DELETE( g_pKTDXApp );

	KNP_Close();

	SAFE_DELETE( g_pX2SMTPMail );

#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnDestroyDevice();

	KProfileMan::GetSingleton().SetProfileIterator( NULL );
#endif


	//{{ robobeg : 2008-10-10
	SAFE_RELEASE( g_pEffectStateManager );
	g_pd3dDevice = NULL;
	//}} robobeg : 2008-10-10

#ifdef _SERVICE_
	HWND hWeb = ::FindWindow(NULL, L"Elsword - Windows Internet Explorer");
	if( hWeb != NULL )
	{
		::ShowWindow(hWeb, SW_RESTORE);
	}
#endif _SERVICE_
}

LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	if( g_pKTDXApp != NULL )
		*pbNoFurtherProcessing = g_pKTDXApp->MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	return 0;
}

void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( bKeyDown )
	{
		switch( nChar )
		{
			//case VK_F1: g_bShowHelp = !g_bShowHelp; break;
		case VK_HOME: 
			{
				if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
				{
					g_bRenderEtc = !g_bRenderEtc; 
#ifdef NEW_VILLAGE
					g_pData->ToggleRenderEtc();
#endif
				}
			}
			break;

#ifdef KHELP_SYSTEM_TEST
		case VK_PRIOR:
			if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				// Shift+End 키를 누른 경우 KHelpSystem을 토글한다.
				if ( GetAsyncKeyState(VK_LSHIFT) & 0x8000 )
				{
					// toggle help system
					_KHelpSystem.EnableDisplayHelp( !_KHelpSystem.IsDisplayHelp() );
				}
				else
				{
					// KHelpSystem이 활성화된 상태에서 End키를 누르면 다음 help를 활성화시킨다.
					if ( _KHelpSystem.IsDisplayHelp() )
					{
						_KHelpSystem.PrevHelp();
					}//if
				}//if.. else..
			}//if
			break;

		case VK_NEXT:
			if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				// Shift+End 키를 누른 경우 KHelpSystem을 토글한다.
				if ( GetAsyncKeyState(VK_LSHIFT) & 0x8000 )
				{
					// toggle help system
					_KHelpSystem.EnableDisplayHelp( !_KHelpSystem.IsDisplayHelp() );
				}
				else
				{
					// KHelpSystem이 활성화된 상태에서 End키를 누르면 다음 help를 활성화시킨다.
					if ( _KHelpSystem.IsDisplayHelp() )
					{
						_KHelpSystem.NextHelp();
					}//if
				}//if.. else..
			}//if
			break;
#endif // KHELP_SYSTEM_TEST
		}
	}
}

void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	KTDXPROFILE();

	g_pKTDXApp->OnFrameMove( fTime, fElapsedTime );
}

void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	//KTDXPROFILE();

	HRESULT hr;
	
	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{

		KTDXPROFILE_BEGIN("KTDXRender");
		g_pKTDXApp->OnFrameRender();
		KTDXPROFILE_END();

		if( true == g_bRenderEtc )
		{
			KTDXPROFILE_BEGIN("DXUTRender");
			DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); // These events are to help PIX identify what the code is doing
			KTDXPROFILE_BEGIN("RenderText");
			RenderText(fElapsedTime);
			KTDXPROFILE_END();

			DXUT_EndPerfEvent();
			KTDXPROFILE_END();
		}

#ifdef KHELP_SYSTEM_TEST // 2008-10-23
        _KHelpSystem.DisplayHelp();
#endif // KHELP_SYSTEM_TEST // 2008-10-23
		
#ifdef BANDICAM_RECORDING
		if( g_pMain != NULL && g_pMain->IsCapturing() == true )
		{
			switch( g_pMain->m_BandiCaptureLibrary.Work((LONG_PTR)pd3dDevice) )
			{
			case BC_OK_IS_NOT_CAPTURING:	//캡쳐중이 아닙니다.
			case BC_OK_COMPLETED_BY_TIME:	//녹화 자동 완료 설정(시간)에 의해 녹화 완료되었습니다.
			case BC_OK_COMPLETED_BY_SIZE:	//녹화 자동 완료 설정(파일크기)에 의해 녹화 완료 되었습니다.
			case BCERR_VIDEO_CAPTURE:		//캡쳐중 에러가 발생하였습니다.
			case BCERR_FILE_CREATE:			//파일 생성을 실패하였습니다.
				{
					g_pMain->Bandi_EndVidioCapture();
				}break;
			case BCERR_FILE_WRITE:			// 파일 쓰기를 실패하였습니다.
			case BCERR_NOT_ENOUGH_DISKSPACE:// 하드디스크 여유 공간 부족이 부족합니다.
				{
					g_pMain->Bandi_EndVidioCapture();
					AfxMessageBox(L"하드 디스크 용량이 부족 합니다.", MB_OK);
				}break;
			default:
				break;
			}
		}
#endif BANDICAM_RECORDING

		V( pd3dDevice->EndScene() );
	}
}

//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText(float fElapsedTime)
{
	g_fTotalFrame += DXUTGetFPS();
	g_RenderCount += 1;
	
	// Output statistics
	WCHAR appData[512] = {0,};
	WCHAR appData3[512] = {0,};
	WCHAR wszText[512] = L"";
	swprintf( appData, DXUTGetFrameStats() );
	wcscat( appData, L"\n" );
	wcscat( appData, DXUTGetDeviceStats() );
	wcscat( appData, L", 평균 프레임 : ");
	wcscat( appData, _itow( (int)g_fTotalFrame / g_RenderCount, appData3, 10 ) );
	if ( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", 오브젝트 LOD : %d %%", g_pX2Game->GetWorld()->GetLOD() );
		wcscat( appData, wszText );
	}

	if ( g_pData != NULL )
	{
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", 캐릭터 LOD : %d %%", g_pData->GetModelDetailPercent() );
		wcscat( appData, wszText );
	}

#ifdef FRAME_MOVE_CULL
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", FrameUpdate : %d %%", (int)CKTDGXSkinAnim::GetTestUpdate() );
	wcscat( appData, wszText );
#endif //FRAME_MOVE_CULL	

#ifdef  DYNAMIC_VERTEX_BUFFER_OPT

    if ( g_pKTDXApp != NULL && g_pKTDXApp->GetDVBManager() != NULL )
    {
        const WCHAR*  pDynamicVBMode = L"";
        switch( g_pKTDXApp->GetDVBManager()->GetDynamicVBMode() )
        {
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_MANAGED:
            pDynamicVBMode = L"MANAGED";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_PRIMITIVEUP:
            pDynamicVBMode = L"PrimitiveUP";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_DYNAMICVB:
            pDynamicVBMode = L"DynamicVB";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_DYNAMICVB_WITH_TRY_CATCH:
            pDynamicVBMode = L"DynamicVBWithTC";
            break;
        }//switch

	    swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", DynamicVBMode : %s", pDynamicVBMode );
	    wcscat( appData, wszText );
    }//if
#endif  DYNAMIC_VERTEX_BUFFER_OPT
	
	wcscat( appData, L"\n" );
	
	WCHAR appData2[256] = {0,};

	int iSimultaneousUserCount = 0;
	int iPVPRoomTeamCount = 0;
	int iPVPRoomTeamDeathCount = 0;
	int iPVPRoomSurvivalCount = 0;
	int iCurPartyCount = 0;
	int iPlayGamePartyCount = 0;
	if( NULL != g_pMain )
	{	
		iSimultaneousUserCount	= g_pMain->GetServerUserCount();
		std::map<int, int>& mapPVPRoomCount = g_pMain->GetMapServerPVPRoomCount();
		if( mapPVPRoomCount.size() > 0 )
		{
			iPVPRoomTeamCount		= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_TEAM ];
			iPVPRoomTeamDeathCount	= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_TEAM_DEATH ];
			iPVPRoomSurvivalCount	= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_SURVIVAL ];
		}

		iCurPartyCount				= g_pMain->GetCurPartyCount();
		iPlayGamePartyCount			= g_pMain->GetPlayGamePartyCount();
	}

#ifndef _SERVICE_
	swprintf( appData2, L"프레임무브 FPS: %f 지연시간: %f  프레임무브: %d회  랜더: %d회 \n", 
		g_pKTDXApp->GetFrameMoveFPS(), g_pKTDXApp->GetElapsedTime(), g_pKTDXApp->GetFrameMoveCount(), g_pKTDXApp->GetRenderCount() );
	wcscat( appData, appData2 );
#endif

	swprintf( appData2, L"동접: %d명, 대전방: <팀:%d, 팀데스:%d, 서바:%d>, 파티: %d, 게임중인파티: %d", 
		iSimultaneousUserCount, iPVPRoomTeamCount, iPVPRoomTeamDeathCount, iPVPRoomSurvivalCount, iCurPartyCount, iPlayGamePartyCount );
	wcscat( appData, appData2 );
	
	/*
	txtHelper.Begin();
	txtHelper.SetInsertionPos( 5, 5 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	txtHelper.DrawTextLine( appData );
	*/

	WCHAR timeBuf[200] = {0,};
	g_pMain->GetFontForDebuf()->OutTextMultiline( 5, 5, appData, D3DXCOLOR( 1, 1, 0, 1), 
		CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );

#ifndef _SERVICE_
	if( g_pKTDXApp != NULL && g_pKTDXApp->GetDevice() != NULL )
	{
		g_fLastUpdateTime_MemInfo += g_pKTDXApp->GetElapsedTime();
		if( g_fLastUpdateTime_MemInfo > 10.f ) 
		{
			g_fLastUpdateTime_MemInfo = 0.f;

			g_MemStatus.dwLength = sizeof( g_MemStatus );
			GlobalMemoryStatus( &g_MemStatus );

			g_pKTDXApp->GetVideoRam( g_iTotalTextureMem, g_iAvailableTextureMem );

			g_iAvailableTextureMem2 = g_pKTDXApp->GetDevice()->GetAvailableTextureMem();
		}
#ifdef CHECK_PROCESS_MEMORY_USAGE   
		PROCESS_MEMORY_COUNTERS_EX pmcex;
		GetProcessMemoryInfo( GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmcex, sizeof(pmcex)); 

		//최고 메모리
		swprintf( timeBuf, L"Peak WorkingSet : %d (MB)", (pmcex.PeakWorkingSetSize)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 520, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 

		//현재 메모리
		swprintf( timeBuf, L"Currnet WorkingSet : %d (MB)", (pmcex.WorkingSetSize)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 540, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );  

		//가상메모리
		swprintf( timeBuf, L"Virtual Memory : %d (MB)", (pmcex.PrivateUsage)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 560, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );  
#else
		swprintf( timeBuf, L"PM : %d/%d (MB)", (g_MemStatus.dwTotalPhys-g_MemStatus.dwAvailPhys)/1024/1024, 
			g_MemStatus.dwTotalPhys/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 540, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		swprintf( timeBuf, L"VM : %d/%d (MB)", (g_MemStatus.dwTotalVirtual-g_MemStatus.dwAvailVirtual)/1024/1024,
			g_MemStatus.dwTotalVirtual/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 560, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
#endif CHECK_PROCESS_MEMORY_USAGE 
		swprintf( timeBuf, L"Video M : %d/%d (MB)", (g_iTotalTextureMem-g_iAvailableTextureMem)/1024/1024, g_iTotalTextureMem/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 580, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		swprintf( timeBuf, L"TM : %d (MB)", g_iAvailableTextureMem2/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 600, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pData->GetDungeonRoom() != NULL && g_pX2Game != NULL )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if( pDungeonGame->GetDungeon() != NULL )
		{
			int iStageIndex = pDungeonGame->GetDungeon()->GetNowStageIndex();

			if( pDungeonGame->GetDungeon()->GetNowStage() != NULL )
			{
				int iSubStageIndex = pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStageIndex();
				swprintf( timeBuf, L"stage : %d-%d", iStageIndex, iSubStageIndex );
				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 40, 620, timeBuf, D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
			}
		}
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		int iLineIndex = (int)g_pX2Game->GetMyUnit()->GetSyncData().lastTouchLineIndex;

		swprintf( timeBuf, L"line : %d", iLineIndex );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 640, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		int posx = (int)g_pX2Game->GetMyUnit()->GetXPos();

		swprintf( timeBuf, L"Pos X: %d", posx );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 480, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		int posy = (int)g_pX2Game->GetMyUnit()->GetYPos();

		swprintf( timeBuf, L"Pos Y: %d", posy );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 500, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		int posz = (int)g_pX2Game->GetMyUnit()->GetZPos();

		swprintf( timeBuf, L"Pos Z: %d", posz );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 520, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pKTDXApp->GetDGManager() != NULL )
	{
		int nNum = g_pKTDXApp->GetDGManager()->GetRenderObjectChainSize();
		int nAlpha = g_pKTDXApp->GetDGManager()->GetAlphaObjectChainSize();
		int nNonAlpha = g_pKTDXApp->GetDGManager()->GetNonAlphaObjectChainSize();        
//{{ robobeg : 2008-10-18
        int nCull = g_pKTDXApp->GetDGManager()->GetCullCount();
		swprintf( timeBuf, L"obj chain(a/n): %d(%d,%d), Cull: %d", nNum, nAlpha, nNonAlpha, nCull );
//}} robobeg : 2008-10-18

        
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 660, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMajorParticle() != NULL )
	{
		int nMajorSeqNum =  static_cast<int>( g_pX2Game->GetMajorParticle()->EstimateParticleSequenceNum() );

		swprintf( timeBuf, L"major p: %d", nMajorSeqNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 680, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}


	if( g_pX2Game != NULL && g_pX2Game->GetMinorParticle() != NULL )
	{		
		int nMinorSeqNum =  static_cast<int>( g_pX2Game->GetMajorParticle()->EstimateParticleSequenceNum() );

		swprintf( timeBuf, L"minor p: %d", nMinorSeqNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 700, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}


	if( g_pX2Game != NULL && g_pX2Game->GetMajorXMeshPlayer() != NULL )
	{		
		int nMajorInstanceNum = static_cast<int>( g_pX2Game->GetMajorXMeshPlayer()->EstimateInstanceNum() );

		swprintf( timeBuf, L"major m: %d", nMajorInstanceNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 720, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMinorXMeshPlayer() != NULL )
	{		
		int nMinorInstanceNum =  static_cast<int>( g_pX2Game->GetMinorXMeshPlayer()->EstimateInstanceNum() );

		swprintf( timeBuf, L"minor m: %d", nMinorInstanceNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 740, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

#endif	
}
// CMainFrame //////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CX2ToolView
IMPLEMENT_DYNCREATE(CX2ToolView, CView)

#pragma region Message Map
BEGIN_MESSAGE_MAP(CX2ToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CX2ToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND( ID_VIEW_RENDER, OnRender )
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATEAPP()
	
	ON_COMMAND(ID_NEW_LINE, &CX2ToolView::OnNewLine)	
	ON_COMMAND(ID_OPEN_LINE, &CX2ToolView::OnLoadLineMap)
	ON_COMMAND(ID_LINE_SAVE, &CX2ToolView::OnLineSave)
	ON_COMMAND(ID_LINE_SAVEAS, &CX2ToolView::OnLineSaveas)
	ON_COMMAND(ID_OPEN_WORLD, &CX2ToolView::OnOpenWorld)
	ON_COMMAND(ID_LINE_EXPORT, &CX2ToolView::OnLineExport)
	ON_COMMAND(ID_WORLD_SAVE, &CX2ToolView::OnWorldSave)
	ON_COMMAND(ID_WORLD_SAVEAS, &CX2ToolView::OnWorldSaveas)
	ON_COMMAND(ID_NEW_WORLD, &CX2ToolView::OnNewWorld)
	ON_COMMAND(ID_LOAD_HEIGHTMAP, &CX2ToolView::OnLoadHeightmap)
	ON_COMMAND(ID_SHOW_OBJECT, &CX2ToolView::OnShowObject)
	ON_COMMAND(ID_FREEZING, &CX2ToolView::OnFreezing)
	ON_COMMAND(ID_VIEW_SELECTED, &CX2ToolView::OnViewSelected)
	ON_COMMAND(ID_VIEW_TOPFRONT, &CX2ToolView::OnViewTopfront)
	ON_COMMAND(ID_SECTOR, &CX2ToolView::OnSector)
	ON_COMMAND(ID_NEW_DUNGEON, &CX2ToolView::OnNewDungeon)
	ON_COMMAND(ID_LOAD_DUNGEON, &CX2ToolView::OnLoadDungeon)
	ON_COMMAND(ID_SAVEAS_DUNGEON, &CX2ToolView::OnSaveasDungeon)
END_MESSAGE_MAP()
#pragma endregion 메시지 맵

#pragma region CX2ToolView
CX2ToolView::CX2ToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}
#pragma endregion 생성자

#pragma region ~CX2ToolView
CX2ToolView::~CX2ToolView()
{
	if( g_pKTDXApp != NULL )
	{
		CKTDXApp::Exit();
		DXUTGetExitCode();
	}
}
#pragma endregion 소멸자

BOOL CX2ToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style = WS_CHILDWINDOW;

	return CView::PreCreateWindow(cs);
}
// CX2ToolView 그리기

void CX2ToolView::OnDraw(CDC* /*pDC*/)
{
	CX2ToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}
// CX2ToolView 인쇄

void CX2ToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CX2ToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CX2ToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CX2ToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CX2ToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CX2ToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CX2ToolView 진단
#ifdef _DEBUG
void CX2ToolView::AssertValid() const
{
	CView::AssertValid();
}

void CX2ToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CX2ToolDoc* CX2ToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CX2ToolDoc)));
	return (CX2ToolDoc*)m_pDocument;
}
#endif //_DEBUG

void CX2ToolView::OnRender()
{
	if( g_pKTDXApp == NULL )
		return;

	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
		if( pToolMain->m_bRenderTopView == true )
		{
			// 피봇 그리기
			pToolMain->RenderAxisView();
		}
	}

	if( DXUTGetD3DDevice() != NULL )
	{
		DXUTRender3DEnvironment();
	}

	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
		pToolMain->CreateThumbnail();
	}	
}

void CX2ToolView::OnInitialUpdate( )
{
	// Begin X2 Setting ////////////////////////////////////////////////////////////////////////////////
	if( g_pKTDXApp != NULL )
		CKTDXApp::Exit();

	g_pX2SMTPMail = new CX2SMTPMail;
	CKTDXApp::StartUp();

	DXUTSetCallbackDeviceCreated( OnCreateDevice );
	DXUTSetCallbackDeviceReset( OnResetDevice );
	DXUTSetCallbackDeviceLost( OnLostDevice );
	DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( KeyboardProc );
	DXUTSetCallbackFrameRender( OnFrameRender );
	DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCursorSettings( true, true );

	InitApp();

	DXUTInit( false, false, true, false );
#ifdef ENABLE_MULTI_MONITOR
	DXUTSetMultimonSettings(true);
#endif ENABLE_MULTI_MONITOR

	HWND hFocus = GetSafeHwnd(); //GetTopLevelParent()->GetSafeHwnd();
	HWND hDevice = GetSafeHwnd();
	DXUTSetWindow( hFocus, hDevice, hDevice, false );

	CRect rectClient;
	GetClientRect(&rectClient);
	HRESULT hr = DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 1024, 768 ); //rectClient.Width(), rectClient.Height() );
	if( FAILED(hr) )
	{
		MessageBox(L"DXUTCreateDevice", L"Error!", MB_OK);
	}

	// End X2 Setting ////////////////////////////////////////////////////////////////////////////////	

}

void CX2ToolView::OnSize(UINT nType, int cx, int cy)
{
	DXUTStaticWndProc(GetSafeHwnd(), WM_SIZE, nType, MAKELPARAM( cx, cy ));
	DXUTStaticWndProc(GetSafeHwnd(), WM_EXITSIZEMOVE, 0, 0);
}

void CX2ToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	//DXUTStaticWndProc(GetSafeHwnd(), WM_MOUSEMOVE, 
	CView::OnMouseMove(nFlags, point);
}

int CX2ToolView::PreTranslateMessage(MSG* pMsg)
{
	DXUTStaticWndProc(GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam);
	return CView::PreTranslateMessage(pMsg);	
}

void CX2ToolView::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	DXUTStaticWndProc(GetSafeHwnd(), WM_ACTIVATEAPP, (WPARAM)TRUE, (LPARAM)NULL);
	CView::OnActivateApp(bActive, dwThreadID);
}

#pragma region User Define Message

#pragma region OnNewLine
void CX2ToolView::OnNewLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( AfxMessageBox(L"새로운 라인맵을 만드시겠습니까?", MB_YESNO) == IDYES )
	{	
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{
			pMain->NewLine();
		}
	}
}
#pragma endregion 새로운 라인 그리기

#pragma region OnLoadLineMap
void CX2ToolView::OnLoadLineMap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"lua script\0*.lua\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{
			pMain->LoadLineMap( ofn.lpstrFile );
			
		}		
	}	
}
#pragma endregion 라인 불러오기

#pragma region OnLineSave
void CX2ToolView::OnLineSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pMain->m_wstrLineMapFullName.c_str() );
		wstring wstrPath = wszFilePathOnly;
		if( pMain->m_wstrLineMapFullName.empty() == true || wstrPath.empty() == true )
		{
			OnLineSaveas();
			return;
		}

		pMain->SaveLineMapToFile(NULL, false);		
	}
}
#pragma endregion 라인 저장하기

#pragma region OnLineSaveas
void CX2ToolView::OnLineSaveas()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		CString strFilter = _T("lua script (*.lua)|*.lua|");
		CFileDialog dlg(FALSE,L"lua",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

		if( dlg.DoModal() != IDOK )
			return;
		wstring wstrSaveFileName = (LPCTSTR)dlg.GetPathName();
		pMain->SaveLineMapToFile(wstrSaveFileName.c_str(), true);
		
	}
}
#pragma endregion 새 이름으로 라인 저장하기

#pragma region OnOpenWorld
void CX2ToolView::OnOpenWorld()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"lua script\0*.lua\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{
			pMain->LoadWorld( ofn.lpstrFile );
		}		
	}
}
#pragma endregion 월드 열기

#pragma region OnLineExport
void CX2ToolView::OnLineExport()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		CString strFilter = _T("lua script (*.ms)|*.ms|");
		CFileDialog dlg(FALSE,L"ms",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

		if( dlg.DoModal() != IDOK )
			return;
		wstring wstrSaveFileName = (LPCTSTR)dlg.GetPathName();
		pMain->ExportLineMap( wstrSaveFileName.c_str() );
		//pMain->SaveWorldToFile(wstrSaveFileName.c_str(), true);

	}
		
// 	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
// 	if( pMain != NULL )
// 	{
// 		pMain->ExportLineMap();
// 	}		
}
#pragma endregion Line Export

#pragma region OnWorldSave
void CX2ToolView::OnWorldSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pMain->m_wstrWorldFullName.c_str() );
		wstring wstrPath = wszFilePathOnly;
		if( pMain->m_wstrWorldFullName.empty() == true || wstrPath.empty() == true )
		{
			OnWorldSaveas();
			return;
		}

		pMain->SaveWorldToFile(NULL, false);
	}	
}
#pragma endregion 월드 저장

#pragma region OnWorldSaveas
void CX2ToolView::OnWorldSaveas()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		CString strFilter = _T("lua script (*.lua)|*.lua|");
		CFileDialog dlg(FALSE,L"lua",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

		if( dlg.DoModal() != IDOK )
			return;
		wstring wstrSaveFileName = (LPCTSTR)dlg.GetPathName();
		pMain->SaveWorldToFile(wstrSaveFileName.c_str(), true);

	}
}
#pragma endregion 새 이름으로 월드 저장

#pragma region OnNewWorld
void CX2ToolView::OnNewWorld()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( AfxMessageBox(L"새로운 월드를 만드시겠습니까?", MB_YESNO) == IDYES )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{
			pMain->LoadWorld( L"new_create_World.lua" );
		}
	}	
}
#pragma endregion 새 월드 만들기

#pragma region OnLoadHeightmap
void CX2ToolView::OnLoadHeightmap()
{	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bHeightMap == false )
	{
		AfxMessageBox(L"심심풀이로 만들었지만 사용할만한곳 없어서 기능 막음", MB_OK);
		return;
	}	

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"bmp\0*.bmp\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		CxImage cimage;
		string strFileName;
		ConvertWCHARToChar(strFileName, ofn.lpstrFile);
		cimage.Load( strFileName.c_str(), CXIMAGE_FORMAT_BMP );
		
		long iWidth = cimage.GetWidth();
		long iHeight = cimage.GetHeight();
		
		if( iWidth > 128 || iHeight > 128 )
		{
			AfxMessageBox(L"버벅거려서 해당 해상도 지원하지 않습니다.", MB_OK);
			return;
		}

		for(long y=0; y<iHeight; ++y)
		{
			for(long x=0; x<iWidth; ++x)
			{
				BYTE h = cimage.GetPixelGray(x, y);
				pMain->CreateHeightMap(x, y, h, iWidth, iHeight);	
			}
		}

		AfxMessageBox(L"HeightMap 로딩이 완료되었습니다.", MB_OK);
	}		
}
#pragma endregion 높이맵 부르기

#pragma region OnShowObject
void CX2ToolView::OnShowObject()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
		pMain->SetShowWorldObject();
}
#pragma endregion 오브젝트 보이기

#pragma region OnFreezing
void CX2ToolView::OnFreezing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
		pMain->SetFreezingWorldObject();
}
#pragma endregion 멈추기

#pragma region OnViewSelected
void CX2ToolView::OnViewSelected()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
		pMain->m_bViewSelectObject = !pMain->m_bViewSelectObject;
}
#pragma endregion View 선택

#pragma region OnViewTopfront
void CX2ToolView::OnViewTopfront()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
		pMain->m_bRenderTopView = !pMain->m_bRenderTopView;
}
#pragma endregion TopFront로 설정

#pragma region OnSector
void CX2ToolView::OnSector()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	m_DlgLineSector.Create(IDD_DIALOG_SECTOR, this);
	m_DlgLineSector.ShowWindow(SW_SHOW);
}
#pragma endregion 섹터 설정

#pragma region OnNewDungeon
void CX2ToolView::OnNewDungeon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		pMain->ResetGame(SEnum::DI_TEMP0);
	}
}
#pragma endregion 새 던전 만들기

#pragma region OnLoadDungeon
void CX2ToolView::OnLoadDungeon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	g_pData->GetDungeonManager()->OpenScriptFile(L"ToolDungeonData.lua");

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_wndClassView.FillClassView();
}
#pragma endregion 던전 불러오기

#pragma region OnSaveasDungeon
void CX2ToolView::OnSaveasDungeon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString strFilter = _T("lua script (*.lua)|*.lua|");
	CFileDialog dlg(FALSE,L"lua",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

	if( dlg.DoModal() != IDOK )
		return;
	wstring wstrSaveFileName = (LPCTSTR)dlg.GetPathName();
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_DlgDungeonEditor.SaveAs(wstrSaveFileName);
}
#pragma endregion 던전 저장

#pragma endregion 메시지 처리기

#pragma region ResetDungeon
void CX2ToolView::ResetDungeon()
{
	// 쓸모 없는 함수가 될수도
}
#pragma endregion 던전 초기화