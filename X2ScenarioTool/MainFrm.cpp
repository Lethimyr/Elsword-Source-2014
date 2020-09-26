//--------------------------------------------------------------------------------------
// File: X2XcenarioTool.cpp
//
// Basic starting point for new Direct3D samples
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"

#pragma comment( lib, "MailMIME.lib" )
#pragma comment( lib, "MailSMTP.lib" )

#pragma message( )
//////////////////////////////////////////////////////////////////////////
//## extern pointer by scenario tool
//## Desc : 툴엔진의 전역 포인터로써 인터페이스와 연동하기 위해 존해함.
extern CX2ScenarioToolStage* g_pScenarioToolStage = NULL;
//## extern pointer by scenario tool
//////////////////////////////////////////////////////////////////////////


//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*              g_pFont = NULL;         // Font for drawing text
ID3DXSprite*            g_pTextSprite = NULL;   // Sprite for batching draw text calls
ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface
CModelViewerCamera      g_Camera;               // A model viewing camera
bool                    g_bShowHelp = true;     // If true, it renders the UI control text
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          // Device settings dialog
CDXUTDialog             g_HUD;                  // dialog for standard controls
extern CKTDGEffectStateManager *g_pEffectStateManager = NULL;
CX2SMTPMail*			g_pX2SMTPMail = NULL;

/*
#ifndef PROFILE_BUILD
#define PROFILE_BUILD
#endif
*/
#ifdef PROFILE_BUILD
RHProfileIterator*  g_pitor = NULL;
#endif


//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3



//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

void    InitApp();
void    RenderText();

LPDIRECT3DDEVICE9        g_pd3dDevice;
LPDIRECT3D9              g_pD3D;

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	CKTDXApp::StartUp();

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Set the callback functions. These functions allow DXUT to notify
	// the application about device changes, user input, and windows messages.  The 
	// callbacks are optional so you need only set callbacks for events you're interested 
	// in. However, if you don't handle the device reset/lost callbacks then the sample 
	// framework won't be able to reset your device since the application must first 
	// release all device resources before resetting.  Likewise, if you don't handle the 
	// device created/destroyed callbacks then DXUT won't be able to 
	// recreate your device resources.
	DXUTSetCallbackDeviceCreated( OnCreateDevice );
	DXUTSetCallbackDeviceReset( OnResetDevice );
	DXUTSetCallbackDeviceLost( OnLostDevice );
	DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( KeyboardProc );
	DXUTSetCallbackFrameRender( OnFrameRender );
	DXUTSetCallbackFrameMove( OnFrameMove );

	// Show the cursor and clip it when in full screen
	DXUTSetCursorSettings( true, true );

	InitApp();

	// Initialize DXUT and create the desired Win32 window and Direct3D 
	// device for the application. Calling each of these functions is optional, but they
	// allow you to set several options which control the behavior of the framework.
	DXUTInit( true, true, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
	DXUTCreateWindow( L"Event Scene Tool" );
	//DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 480, 200, IsDeviceAcceptable, ModifyDeviceSettings );
	DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 1024, 768, IsDeviceAcceptable, ModifyDeviceSettings );
	//DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 1280, 1023, IsDeviceAcceptable, ModifyDeviceSettings );

	// Pass control to DXUT for handling the message pump and 
	// dispatching render calls. DXUT will call your FrameMove 
	// and FrameRender callback when there is idle time between handling window messages.
	DXUTMainLoop();

	// Perform any application-level cleanup here. Direct3D device resources are released within the
	// appropriate callback functions and therefore don't require any cleanup code here.

	CKTDXApp::Exit();

	return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{

	// Initialize dialogs
	g_SettingsDlg.Init( &g_DialogResourceManager );
	g_HUD.Init( &g_DialogResourceManager );
	//g_SampleUI.Init( &g_DialogResourceManager );
	/*
	g_HUD.SetCallback( OnGUIEvent ); int iY = 10; 
	g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
	g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
	g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );
	*/
	// g_SampleUI.SetCallback( OnGUIEvent ); iY = 10; 
	/*

	TODO: add UI controls as needed
	g_SampleUI.AddComboBox( 19, 35, iY += 24, 125, 22 );
	g_SampleUI.GetComboBox( 19 )->AddItem( L"Text1", NULL );
	g_SampleUI.GetComboBox( 19 )->AddItem( L"Text2", NULL );
	g_SampleUI.GetComboBox( 19 )->AddItem( L"Text3", NULL );
	g_SampleUI.GetComboBox( 19 )->AddItem( L"Text4", NULL );
	g_SampleUI.AddCheckBox( 21, L"Checkbox1", 35, iY += 24, 125, 22 );
	g_SampleUI.AddCheckBox( 11, L"Checkbox2", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 12, 1, L"Radio1G1", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 13, 1, L"Radio2G1", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 14, 1, L"Radio3G1", 35, iY += 24, 125, 22 );
	g_SampleUI.GetRadioButton( 14 )->SetChecked( true ); 
	g_SampleUI.AddButton( 17, L"Button1", 35, iY += 24, 125, 22 );
	g_SampleUI.AddButton( 18, L"Button2", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 15, 2, L"Radio1G2", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 16, 2, L"Radio2G3", 35, iY += 24, 125, 22 );
	g_SampleUI.GetRadioButton( 16 )->SetChecked( true );
	g_SampleUI.AddSlider( 20, 50, iY += 24, 100, 22 );
	g_SampleUI.GetSlider( 20 )->SetRange( 0, 100 );
	g_SampleUI.GetSlider( 20 )->SetValue( 50 );
	g_SampleUI.AddEditBox( 20, L"Test", 35, iY += 24, 125, 32 );
	*/
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
								 D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	// Skip backbuffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3DObject(); 
	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
	// then switch to SWVP.
	if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
	{
		pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// Debugging vertex shaders requires either REF or software vertex processing 
	// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
	if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
	{
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
#endif
#ifdef DEBUG_PS
	pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif

	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if( s_bFirstTime )
	{
		s_bFirstTime = false;
		if( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
			DXUTDisplaySwitchingToREFWarning();
	}

	return true;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
	V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );

	// Initialize the font
	V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		L"Arial", &g_pFont ) );

	g_pd3dDevice = pd3dDevice;
	g_pD3D = DXUTGetD3DObject();

	g_pEffectStateManager = new CKTDGEffectStateManager;
	ASSERT( g_pEffectStateManager != NULL );

	//////////////////////////////////////////////////////////////////////////
	//## create engine begin
	g_pKTDXApp	= new CKTDXApp(DXUTGetHWND(), DXUTGetHINSTANCE(), pd3dDevice);

	g_pScenarioToolStage = new CX2ScenarioToolStage();

	if(g_pScenarioToolStage == NULL)
	{
		assert(g_pScenarioToolStage == NULL);
	}

	g_pKTDXApp->InsertStage(g_pScenarioToolStage);

	g_pScenarioToolStage->CreateDlg();
	//## create engine end
	//////////////////////////////////////////////////////////////////////////


#ifdef PROFILE_BUILD
	g_pitor = RHProfileManager::Get_Iterator();
	ASSERT( g_pitor );
	KProfileMan::GetSingleton().SetProfileIterator( g_pitor );
#endif

	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
							   const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnResetDevice() );
	V_RETURN( g_SettingsDlg.OnResetDevice() );

	if( g_pFont )
		V_RETURN( g_pFont->OnResetDevice() );

	// Create a sprite to help batch calls when drawing many lines of text
	V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pTextSprite ) );

	g_HUD.SetLocation( pBackBufferSurfaceDesc->Width-170, 0 );
	g_HUD.SetSize( 170, 170 );
	//g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width-170, pBackBufferSurfaceDesc->Height-350 );
	//g_SampleUI.SetSize( 170, 300 );

	g_pKTDXApp->OnResetDevice( pBackBufferSurfaceDesc );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnPreFrameMove( fElapsedTime );
#endif

	KTDXPROFILE();

	g_pKTDXApp->OnFrameMove( fTime, fElapsedTime );
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	KTDXPROFILE();

	HRESULT hr;

	// If the settings dialog is being shown, then
	// render it instead of rendering the app's scene
	if( g_SettingsDlg.IsActive() )
	{
		g_SettingsDlg.OnRender( fElapsedTime );
		return;
	}

	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		g_pKTDXApp->OnFrameRender();

		DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); // These events are to help PIX identify what the code is doing
		RenderText();
		V( g_HUD.OnRender( fElapsedTime ) );
		//V( g_SampleUI.OnRender( fElapsedTime ) );
		DXUT_EndPerfEvent();

#ifdef PROFILE_BUILD
		KProfileMan::GetSingleton().OnFrameRender( pd3dDevice, fElapsedTime );
#endif
		V( pd3dDevice->EndScene() );
	}
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves performance.
	// Output statistics
	CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

	txtHelper.Begin();
	txtHelper.SetInsertionPos( 5, 5 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	txtHelper.DrawTextLine( DXUTGetFrameStats() );
	txtHelper.DrawTextLine( DXUTGetDeviceStats() );

	WCHAR appData[255] = {0,};
	swprintf( appData, L"프레임무브 FPS: %f 지연시간: %f  프레임무브: %d회  랜더: %d회", g_pKTDXApp->GetFrameMoveFPS(), g_pKTDXApp->GetElapsedTime(), g_pKTDXApp->GetFrameMoveCount(), g_pKTDXApp->GetRenderCount() );
	txtHelper.DrawTextLine( appData );


	txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	/*
	if( g_pKTDXApp != NULL )
	*pbNoFurtherProcessing = g_pKTDXApp->MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
	return 0;


	// Always allow dialog resource manager calls to handle global messages
	// so GUI state is updated correctly
	*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
	return 0;

	if( g_SettingsDlg.IsActive() )
	{
	g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
	return 0;
	}

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
	return 0;
	// *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
	//if( *pbNoFurtherProcessing )
	//    return 0;
	*/
	*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	if( g_SettingsDlg.IsActive() )
	{
		g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
		return 0;
	}

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;
	//	*pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
	//	if( *pbNoFurtherProcessing )
	//		return 0;

	if( g_pKTDXApp != NULL )
		*pbNoFurtherProcessing = g_pKTDXApp->MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;	

	return 0;
}


//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{

#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnKeyboardProc( nChar, bKeyDown, bAltDown );
#endif

	if( bKeyDown )
	{
		switch( nChar )
		{
		case VK_F1: g_bShowHelp = !g_bShowHelp; break;
		}
	}
}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
	case IDC_TOGGLEREF:        DXUTToggleREF(); break;
	case IDC_CHANGEDEVICE:     g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
	}
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
	g_DialogResourceManager.OnLostDevice();
	g_SettingsDlg.OnLostDevice();
	if( g_pFont )
		g_pFont->OnLostDevice();
	SAFE_RELEASE( g_pTextSprite );
	g_pKTDXApp->OnLostDevice();
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	g_DialogResourceManager.OnDestroyDevice();
	g_SettingsDlg.OnDestroyDevice();
	SAFE_RELEASE( g_pFont );

	//////////////////////////////////////////////////////////////////////////
	//## delete engine
	SAFE_DELETE( g_pKTDXApp );
	//////////////////////////////////////////////////////////////////////////

#ifdef PROFILE_BUILD
	ASSERT( g_pitor );
	RHProfileManager::Release_Iterator( g_pitor );
	g_pitor = NULL;
	KProfileMan::GetSingleton().SetProfileIterator( NULL );
#endif


}



