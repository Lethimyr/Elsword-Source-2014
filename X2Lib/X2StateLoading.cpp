#include "StdAfx.h"


#include ".\x2stateloading.h"

#ifdef CLIENT_PURPLE_MODULE	// 임규수 일본 추가
#include "OnlyGlobal/JP/Auth/PurpleForClient.h"
#endif // CLIENT_PURPLE_MODULE

//extern bool g_bIsDXUTDeveloper;
//extern bool g_bIsDXUTSpecialUser;




CX2StateLoading::CX2StateLoading(void) :
// 2009.01.09 김태완 : 초기화 코드정리
m_pDLGLoadingFront(NULL),
m_fLogoTime(0.0f),
m_hCompanyLogo(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_hLogoBack(INVALID_PARTICLE_SEQUENCE_HANDLE)
#ifndef DELIBERATIONLOGO_SKIP
, m_hDeliberationLogo(INVALID_PARTICLE_SEQUENCE_HANDLE)
#endif DELIBERATIONLOGO_SKIP
//{{AFX
//m_hNexonLogo(INVALID_PARTICLE_SEQUENCE_HANDLE),
//m_hKOGBack(INVALID_PARTICLE_SEQUENCE_HANDLE),
//m_hKOGLogo(INVALID_PARTICLE_SEQUENCE_HANDLE)
//m_hKOGLogoMini(INVALID_PARTICLE_SEQUENCE_HANDLE),
//m_hNexonLogoMini(INVALID_PARTICLE_SEQUENCE_HANDLE),
//m_hPPoruSurprise(INVALID_PARTICLE_SEQUENCE_HANDLE),
//m_hPPoruLogo(INVALID_PARTICLE_SEQUENCE_HANDLE)
//m_pWillium(NULL),
//m_pElSword(NULL),
//m_pLena(NULL),
//m_pAisha(NULL),
//m_pBenders(NULL)
//m_pTextureTestMark;	// Commented in Header file
//}}AFX
#ifdef FIRST_GAME_LOADING_THREAD_TEST
,
m_pStateLoadingThread(NULL),
m_bSentEGS_STATE_CHANGE_SERVER_SELECT_REQ(false)
#endif FIRST_GAME_LOADING_THREAD_TEST
{
	//{{AFX
	// 초기화 ////////////////////////////////////////////////////////////////////////
#ifdef FIRST_GAME_LOADING_THREAD_TEST
	// 	m_bSentEGS_STATE_CHANGE_SERVER_SELECT_REQ = false;
	// 
	// 	m_pStateLoadingThread = NULL;
#endif FIRST_GAME_LOADING_THREAD_TEST
	//}}AFX

#ifdef _SERVICE_
#ifdef _NEXON_KR_
	//OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck3" );
	OpenGlobalURL( L"http://elsword.nexon.com/wiselog/gameinstall.aspx?section=check3" );
#endif
#endif
	//////////////////////////////////////////////////////////////////////////

//{{ robobeg : 2008-10-28
    g_pKTDXApp->EnterLuaThread();
//}} robobeg : 2008-10-28


	TextureReady(L"HQ_DeliberationLogo.dds");
	TextureReady(L"HQ_CompanyLogo.dds");
	//{{AFX
	// KOG 겨울 새 로고 
	//TextureReady( L"HQ_KOGLogoWinter.dds" );
	//TextureReady( L"HQ_KOGLogo.dds" );
	//TextureReady( L"HQ_PPoruLogo.dds" );
	//TextureReady( L"HQ_NexonLogoMini.tga" );
	//TextureReady( L"HQ_KOGLogoMini.tga" );
	//}}AFX
#if 0 // 인트로에 나오는 애들 빼기
	XSkinMeshReady( L"Motion_WilliamPPoru.x" );
	XSkinMeshReady( L"Logo_Motion_Elsword_Dash.x" );
	XSkinMeshReady( L"Logo_Motion_Arme_Dash.X" );
	XSkinMeshReady( L"Logo_Motion_Lire_Dash.X" );
	XSkinMeshReady( L"Logo_Motion_Benders.X" );
#endif
	SoundReady( L"NexonLogo.ogg" );
#ifdef ADD_NEW_YEAR_SOUND
	m_bPlayLogoSound = false;
#endif
	//g_pKTDXApp->GetDeviceManager()->PlaySound( L"NexonLogo.ogg" );

#if defined CLIENT_COUNTRY_TW
	m_pDLGLoadingFront = new CKTDGUIDialog( this, L"DLG_First_Loading_Front_TW.lua", 0.0f );
#elif defined CLIENT_COUNTRY_HK
	m_pDLGLoadingFront = new CKTDGUIDialog( this, L"DLG_First_Loading_Front_HK.lua", 0.0f );
#else 
	m_pDLGLoadingFront = new CKTDGUIDialog( this, L"DLG_First_Loading_Front.lua", 0.0f );
#endif

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGLoadingFront );

	wstring wstrClientVersion = g_pMain->GetClientVersion();
#ifdef VERSION_DOT
	wstrClientVersion += L".";
#else VERSION_DOT
	wstrClientVersion += L"_";
#endif VERSION_DOT
	wstrClientVersion += g_pMain->GetSubClientVersion_LUA();
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGLoadingFront->GetControl( L"Static_Client_Version" );
	if( pStatic != NULL )
	{
		pStatic->GetString(0)->msg = wstrClientVersion;
	}
	m_pDLGLoadingFront->SetShow( false );



	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1500, 0,0,0 );
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( 1.0f );

	SetLoadingGageBar( 0 );

	//m_pTextureTestMark = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Test_Mark.tga" );
#if defined( SERV_COUNTRY_TWHK )
	m_hCompanyLogo  = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"HongkongIndexLogo", 512, 384, 0, 9999, 9999, -1, 1 );
#elif defined (SERV_COUNTRY_JP)
	m_hCompanyLogo	= g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"CompanyLogo_JP", 512, 384, 0, 9999, 9999, -1, 1 );
#else	
	m_hLogoBack		= g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"LogoBack", 512, 384, 0, 9999, 9999, -1, 1 );
	m_hCompanyLogo	= g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"CompanyLogo", 512, 384, 0, 9999, 9999, -1, 1 );
#endif CLIENT_COUNTRY_TWHK

//{{AFX
#ifdef LOADING_ANIMATION_TEST
	g_pMain->GetLoadingAnimation()->SetShow( true );
#endif LOADING_ANIMATION_TEST

#ifdef FIRST_GAME_LOADING_THREAD_TEST
// 	m_bSentEGS_STATE_CHANGE_SERVER_SELECT_REQ = false;
// 	m_pStateLoadingThread = NULL;
#endif FIRST_GAME_LOADING_THREAD_TEST
//}}AFX

#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	m_bFirstVerifyAccount = true;
#endif

	g_pData->ResetServerProtocol();

#ifndef SERV_KOM_FILE_CHECK_ADVANCED
#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT
	if( false == g_pMain->GetIsValideCheckKomScript() )
	{
		g_pMain->SetQuitType( NetError::ERR_CLIENT_QUIT_03 );
		PostQuitMessage(0);
	}
#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT
#endif // SERV_KOM_FILE_CHECK_ADVANCED

#if defined( SERV_HACKING_TOOL_LIST )

	THEMIDA_VM_START
#if defined( _SERVICE_ ) 
	// checked
	ELSWORD_VIRTUALIZER_START
#endif

#ifdef	ADD_SERVER_GROUP
#ifdef EXTEND_SERVER_GROUP_MASK
#else EXTEND_SERVER_GROUP_MASK
	if ( g_pInstanceData->GetServerGroupID() == SGI_INVALID )
    {
		//OpenScriptServerGroupFile();
        g_pInstanceData->OpenScriptServerGroupFile();
    }
	g_pMain->SetPickedChannelServerIPIndex( g_pInstanceData->GetServerGroupID() );
#endif EXTEND_SERVER_GROUP_MASK
#endif

#ifdef CLOSE_ON_START_FOR_GAMEGUARD
#if defined( CLIENT_COUNTRY_JP ) || defined( CLIENT_COUNTRY_PH )
	if(g_pMain->IsCloseOnStart()== false)
	{
		ConnectToChannelServer();
	}
#endif
#else
	ConnectToChannelServer();
#endif CLOSE_ON_START_FOR_GAMEGUARD

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif
	THEMIDA_VM_END

#endif // defined( SERV_HACKING_TOOL_LIST )

	g_pKTDXApp->SkipFrame();

#ifdef CLOSE_ON_START_FOR_GAMEGUARD
#if defined( CLIENT_COUNTRY_JP )
	if(g_pMain->IsCloseOnStart()== true)
	{
		Handler_EGS_CLIENT_QUIT_REQ();
		g_pMain->SetCloseOnStart(false);
	}
#endif
#endif CLOSE_ON_START_FOR_GAMEGUARD
}

CX2StateLoading::~CX2StateLoading(void)
{
	SAFE_DELETE_DIALOG(m_pDLGLoadingFront);

	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hCompanyLogo );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hLogoBack );
#ifndef DELIBERATIONLOGO_SKIP
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hDeliberationLogo );
#endif DELIBERATIONLOGO_SKIP

//{{AFX
#if 0 
	g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pWillium );
	g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pElSword );
	g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pLena );
	g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pAisha );
	g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pBenders );
#endif

#ifdef LOADING_ANIMATION_TEST
	g_pMain->GetLoadingAnimation()->SetShow( false );
#endif LOADING_ANIMATION_TEST

#ifdef FIRST_GAME_LOADING_THREAD_TEST
	SAFE_DELETE( m_pStateLoadingThread );
#endif FIRST_GAME_LOADING_THREAD_TEST
//}}AFX

//{{ robobeg : 2008-10-28
    g_pKTDXApp->ExitLuaThread();
//}} robobeg : 2008-10-28

#ifdef NO_MORE_ERROR_TEST
		CheckOutPossibleError();	
#endif NO_MORE_ERROR_TEST
	
#ifdef ADD_NEW_YEAR_SOUND
	m_bPlayLogoSound = false;
#endif
}

HRESULT CX2StateLoading::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2State::OnFrameMove( fTime, fElapsedTime );

//{{AFX
#ifdef FIRST_GAME_LOADING_THREAD_TEST
	if( NULL != m_pStateLoadingThread )
	{
		if( true == m_pStateLoadingThread->GetLoadCompleted() && false == m_bSentEGS_STATE_CHANGE_SERVER_SELECT_REQ )
		{
			m_bSentEGS_STATE_CHANGE_SERVER_SELECT_REQ = true;
			//Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SERVER_SELECT, NULL, false );			

#ifdef _SERVICE_
#ifdef _NEXON_KR_
			OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck4" );
#endif
#endif
		}
	}	
	#ifdef CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
		if( NULL == m_pStateLoadingThread &&
			true == g_pMain->GetReceived_EGS_KNPROTECT_AUTH_CHECK_NOT() )
	#else
		if( NULL == m_pStateLoadingThread )
	#endif CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
		{
			m_pStateLoadingThread = new CX2StateLoadingThread;
			m_pStateLoadingThread->BeginThreadWithPreEndCB( NULL );
		}	
#endif FIRST_GAME_LOADING_THREAD_TEST
//}}AFX

#ifndef SKIP_INTRO_ANIMATION
	m_fLogoTime += fElapsedTime;

	//# 회사로고 4초 후 심의등급 로고로 전환
#ifndef DELIBERATIONLOGO_SKIP
	if( m_hCompanyLogo != INVALID_PARTICLE_SEQUENCE_HANDLE && m_fLogoTime > 4.0f )
#else  DELIBERATIONLOGO_SKIP
	if( m_hCompanyLogo != INVALID_PARTICLE_SEQUENCE_HANDLE && m_fLogoTime > 5.0f )
#endif DELIBERATIONLOGO_SKIP
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hCompanyLogo );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hLogoBack );
		m_hCompanyLogo	= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hLogoBack		= INVALID_PARTICLE_SEQUENCE_HANDLE;

		// 심의등급 로고 생성
#ifndef DELIBERATIONLOGO_SKIP
		m_hDeliberationLogo	= g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"DeliberationLogo", 512, 384, 0, 9999, 9999, -1, 1 );
#endif DELIBERATIONLOGO_SKIP
//{{AFX
#if 0 
		//m_hPPoruSurprise	= g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"LogoPPoruSurprise", 650, 450, 0, 9999, 9999, -1, 1 );
		m_pWillium			= g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LogoWilliamPPoru",	1000,-400,0, 0,0,0, 0,180,0 );
		m_pElSword			= g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LogoElSword",			1100,-400,0, 0,0,0, 0,180,0 );
		m_pLena				= g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LogoLena",			1500,-400,100, 0,0,0, 0,180,0 );
		m_pAisha			= g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LogoAisha",			1600,-400,-200, 0,0,0, 0,180,0 );
		//m_pBenders			= g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LogoBenders",			500,-230,-1100, 0,0,0, 0,180,0 );
#endif
//}}AFX
		//m_fLogoTime = 0.0f;	// 해외팀 주석 처리

#ifdef INACTIVEATION_MINIMIZE_TEST
		{
			// # 테스트를 위해 지정한 txt파일이 게임 폴더 내 존재할 때만 처리
			const string strFileName = "Inactivation_Mimize.txt";
			FILE* file = NULL;
			file = fopen( strFileName.c_str(), "r" );		
			if( NULL != file )
			{
				// #x2 윈도우의 위치가 정렬되지 않은 문제를 해결하기 위해 해상도 변경 작업 수행
				if( NULL != g_pMain )
				{
					const D3DXVECTOR2& vRect = g_pMain->GetGameOption().GetResolution();
					// # 기본 해상도이거나, 전체화면일 때 수행하지 않기
					if( !(true == IsSamef( 1024.f, vRect.x) && true == IsSamef(768.f, vRect.y)) &&
						false == g_pMain->GetGameOption().GetIsFullScreen() )
					{
						g_pMain->GetGameOption().SetResolution( static_cast<DWORD>(vRect.x), static_cast<DWORD>(vRect.y) );
					}
				}
				fclose(file);
			}
		}
#endif // INACTIVEATION_MINIMIZE_TEST


		g_pKTDXApp->SkipFrame();
#ifndef DELIBERATIONLOGO_SKIP
		m_fLogoTime = 0.0f;
	}
	else if( m_hDeliberationLogo != INVALID_PARTICLE_SEQUENCE_HANDLE )
#endif DELIBERATIONLOGO_SKIP

#endif SKIP_INTRO_ANIMATION
#ifndef DELIBERATIONLOGO_SKIP
	{
#endif DELIBERATIONLOGO_SKIP
//{{AFX
#ifdef SKIP_INTRO_ANIMATION

	#ifdef CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
		if( m_fLogoTime == 0.f &&
			true == g_pMain->GetReceived_EGS_KNPROTECT_AUTH_CHECK_NOT() )			
	#else
		if( m_fLogoTime == 0.f )
	#endif CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
		{
			m_fLogoTime = 100.f;
#else
//}}AFX
	#ifdef CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
		if( m_fLogoTime > 5.0f &&
			true == g_pMain->GetReceived_EGS_KNPROTECT_AUTH_CHECK_NOT() )
	#else

#ifdef ADD_NEW_YEAR_SOUND
		//4초후 공존의축제 노래 시작 시간 딜레이 시키기
		if ( false == m_bPlayLogoSound && m_fLogoTime > 2.5f )
		{
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"NexonLogo.ogg" );
			m_bPlayLogoSound = true;
		}
#endif
		if( m_fLogoTime > 5.0f )
	#endif CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
		{

#endif SKIP_INTRO_ANIMATION
			//# 모든 로그 랜더링이 끝나면
#ifndef DELIBERATIONLOGO_SKIP
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hDeliberationLogo );
#endif DELIBERATIONLOGO_SKIP

			//{{AFX
#if 0
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pWillium );
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pElSword );
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pLena );
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pAisha );
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_pBenders );
#endif
			//}}AFX
			

			//# 게임관련 사항 초기화 시작
			m_pDLGLoadingFront->SetShow( true );

#ifdef FIRST_GAME_LOADING_THREAD_TEST
#else


			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadUserUnitMotion1();

			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 5 ) ) );
			SetLoadingGageBar( 5 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadUserUnitMotion2();

			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 10 ) ) );
			SetLoadingGageBar( 10 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadUserUnitMotion3();
			g_pData->LoadUserUnitMotion4();


			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 15 ) ) );
			SetLoadingGageBar( 15 );

	

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadUserUnitMotion5();
			

			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 20 ) ) );
			SetLoadingGageBar( 20 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

#ifdef NEW_CHARACTER_CHUNG
			g_pData->LoadUserUnitMotion6();
#endif
#ifdef ARA_CHARACTER_BASE
			g_pData->LoadUserUnitMotion7();
#endif
#ifdef NEW_CHARACTER_EL
			g_pData->LoadUserUnitMotion8();
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			g_pData->LoadUserUnitMotion9();
#endif //SERV_9TH_NEW_CHARACTER


			g_pData->LoadCommonDevice();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 30 ) ) );
			SetLoadingGageBar( 30 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			//g_pData->LoadAishaDevice();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 40 ) ) );
			SetLoadingGageBar( 40 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			//g_pData->LoadLenaDevice();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 50 ) ) );
			SetLoadingGageBar( 50 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar1();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 54 ) ) );
			SetLoadingGageBar( 54 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar2();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 58 ) ) );
			SetLoadingGageBar( 58 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar3();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 62 ) ) );
			SetLoadingGageBar( 62 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar4();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 66 ) ) );
			SetLoadingGageBar( 66 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar5();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 70 ) ) );
			SetLoadingGageBar( 70 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar6();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 74 ) ) );
			SetLoadingGageBar( 74 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar7();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 78 ) ) );
			SetLoadingGageBar( 78 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar8();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 82 ) ) );
			SetLoadingGageBar( 82 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar9();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 86 ) ) );
			SetLoadingGageBar( 86 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			g_pData->LoadPicChar10();

			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 90 ) ) );
			SetLoadingGageBar( 90 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			// 훈련소 데이터 로드
			g_pData->ResetTrainingCenterTable( L"TrainingCenterTemplet.lua" );
			
			//아이템 데이타 로드
			g_pData->ResetItemManager(); // 80

			//{{ 2008. 10. 1  최육사
			g_pData->ResetTitleManager();
			//}}

			//{{ 2009. 6. 24  최육사	보상테이블
			g_pData->ResetRewardTable();
			//}}

			//{{ 허상형 : [2009/9/16] //	길드 매니저 생성
#ifdef GUILD_MANAGEMENT
			g_pData->ResetGuildManager();
#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/16] //	길드 매니저 생성

#ifdef NEW_VILLAGE_UI
			g_pData->ResetUIManager();
			g_pData->ResetUserMenu();
#endif

#ifdef CHARACTER_MOUSE_RBUTTON
			g_pData->ResetUserListPopUp();
#endif CHARACTER_MOUSE_RBUTTON

#ifdef SERV_PET_SYSTEM
			g_pData->ResetPetManager();
#endif

#ifdef ADDED_RELATIONSHIP_SYSTEM
			g_pData->ResetRelationshipManager();
			g_pData->ResetRelationshipEffectManager();
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef RIDING_SYSTEM
			g_pData->ResetRidingPetManager();
#endif //RIDING_SYSTEM

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			g_pData->ResetItemStatCalculator();
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

			//PVPResult테이블 로드
			g_pData->LoadPVPResultTable();
			//EXP테이블 로드
			g_pData->LoadEXPTable();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 92 ) ) );
			SetLoadingGageBar( 92 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			//월드 데이타 로드
			g_pData->ResetWorldManager( L"WorldData.lua" );	
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 93 ) ) );
			SetLoadingGageBar( 93 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			//던전 데이타 로드
			g_pData->ResetDungeonManager( L"DungeonData.lua" );
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 94 ) ) );
			SetLoadingGageBar( 94 );

			//{{ 허상형 : [2011/3/17/] //	월드미션
#ifdef SERV_INSERT_GLOBAL_SERVER
			g_pData->ResetWorldMissionManager();
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 허상형 : [2011/3/17/] //	월드미션

			//{{ JHKang / 강정훈 / 2011.6.8 / 휴면 복귀 유저
#ifdef COME_BACK_REWARD
			g_pData->ResetPremiumBuffManager();
#endif
			//}}

#ifdef BUFF_TEMPLET_SYSTEM	
			g_pData->GetPremiumBuffManager()->OpenScriptFile( L"BuffTemplet.lua" );
#endif BUFF_TEMPLET_SYSTEM

#ifdef SERV_GLOBAL_MISSION_MANAGER
			g_pData->ResetGlobalMissionManager();
#endif SERV_GLOBAL_MISSION_MANAGER

			//{{ 오현빈 // 2012-05-15 // 플레이 가이드 Class
			g_pData->ResetPlayGuide();
			//}}
			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

#ifdef SERV_LOCAL_RANKING_SYSTEM //지인시스템
			g_pData->ResetProfileManager();
#endif //SERV_LOCAL_RANKING_SYSTEM

			//퀘스트 데이타 로드
	#ifdef SERV_ACCOUNT_MISSION_SYSTEM
			g_pData->ResetQuestManager( L"FieldQuest.lua", L"FieldSubQuest.lua", L"AccountQuest.lua" );
	#else
			g_pData->ResetQuestManager( L"FieldQuest.lua", L"FieldSubQuest.lua" );
	#endif //SERV_ACCOUNT_MISSION_SYSTEM
			
#ifdef DAY_OF_THE_WEEK_QUEST
			g_pData->OpenDayOfWeekQuestScript( L"AccountQuestDate.lua" );
#endif // DAY_OF_THE_WEEK_QUEST

#ifdef CUBE_OPEN_IMAGE_MANAGER
			g_pData->ResetCubeOpenImageManager();
#endif // CUBE_OPEN_IMAGE_MANAGER

			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 95 ) ) );
			SetLoadingGageBar( 95 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			//스킬트리 데이타 로드

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			g_pData->ResetSkillTree( L"NewSkillTempletVer2.lua", L"SkillData.lua" ); // 10
#else // UPGRADE_SKILL_SYSTEM_2013

			g_pData->ResetSkillTree( L"NewSkillTemplet.lua", L"SkillData.lua" ); // 10
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef COMBO_TREE_UI_TEST
			g_pData->ResetComboTree( L"ComboTree.lua" );
#endif COMBO_TREE_UI_TEST

			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 96 ) ) );
			SetLoadingGageBar( 96 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

			//데미지 매니져
			g_pData->ResetDamageManager();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 97 ) ) );
			SetLoadingGageBar( 97 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

#ifdef EVENT_SCENE
			g_pData->ResetEventScene(L"EventSceneObject.lua", L"EventScene.lua");
#endif EVENT_SCENE

			// slide shot
			g_pData->ResetSlideShot();


			g_pInstanceData->ResetMiniMapUI(); // 3

#ifdef NEW_EMBLEM_MANAGER
			CX2EmblemManager::ResetEmblemManager();
#endif //NEW_EMBLEM_MANAGER

#ifdef NEW_MAIL_LOG
			CX2MailLogManager::ResetMailLogManager();
#endif // NEW_MAIL_LOG

#ifdef FIELD_BOSS_RAID
			CX2BossRaidManager::ResetBossRaidManager();
#endif // FIELD_BOSS_RAID
#ifdef REFORM_SKILL_NOTE_UI
			CX2SkillNoteManager::ResetSkillNoteManager();
			CX2SkillNoteManager::GetInstance()->OpenScriptFile();
#endif // REFORM_SKILL_NOTE_UI

			//드랍아이템 리소스
			DropItemLoading();
			SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 100 ) ) );
			SetLoadingGageBar( 100 );

			g_pKTDXApp->GetDevice()->BeginScene();
			OnFrameRender();
			g_pKTDXApp->GetDevice()->EndScene();
			g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );

//{{ kimhc // 2010.3.10 //	광고노출도 초기화
#ifdef	IGA_TEST
			CX2IGA::GetInstance()->Init();
#endif	IGA_TEST
//}} kimhc // 2010.3.10 //	광고노출도 초기화

			//Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
#ifdef ELSWORD_NEW_BEGINNING
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_BEGINNING, NULL, false );			
#else
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SERVER_SELECT, NULL, false );			
#endif ELSWORD_NEW_BEGINNING


#ifdef _SERVICE_
#ifdef _NEXON_KR_
			//OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck4" );
			OpenGlobalURL( L"http://elsword.nexon.com/wiselog/gameinstall.aspx?section=check4" );
#endif
#endif

#endif FIRST_GAME_LOADING_THREAD_TEST
		}
	}

#ifdef SERV_KEY_MAPPING_INT
	if( ( GET_KEY_STATE(GA_RETURN) || g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN) == TRUE ) ||
		GET_KEY_STATE(GAMEACTION_SLOT_CHANGE)
		|| g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState(MOUSE_LBUTTON) == TRUE )
#else // SERV_KEY_MAPPING_INT
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN) == TRUE
		|| g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_SPACE) == TRUE 
		|| g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState(MOUSE_LBUTTON) == TRUE )
#endif // SERV_KEY_MAPPING_INT
	{
		if( m_hCompanyLogo != INVALID_PARTICLE_SEQUENCE_HANDLE )
			m_fLogoTime = 5.0f;
#ifdef DELIBERATIONLOGO_SKIP
#else DELIBERATIONLOGO_SKIP
		else
		if( m_hDeliberationLogo != INVALID_PARTICLE_SEQUENCE_HANDLE &&
			m_fLogoTime >= 3.f	//심의등급 로고는 3초이상 보여주어야 함.
			)
			m_fLogoTime = 16.0f;
#endif DELIBERATIONLOGO_SKIP
	}
	
	return S_OK;
}

HRESULT CX2StateLoading::OnFrameRender()
{
	//CX2State::OnFrameRender();

	g_pKTDXApp->GetDGManager()->ObjectChainSort();
//{{ robobeg : 2008-10-17
    g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
    g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();

//}} robobeg : 2008-10-17
	g_pKTDXApp->GetDGManager()->FrontUIRender();

	g_pMain->GetTextManager()->OnFrameRender();

	if ( g_pMain->GetGameEdit() )
		g_pMain->GetGameEdit()->OnFrameRender();

	//{{AFX
#ifdef LOADING_ANIMATION_TEST
	g_pMain->GetLoadingAnimation()->OnFrameRender();
#endif LOADING_ANIMATION_TEST
	//}}AFX

	m_pCursor->OnFrameRender();

	return S_OK;
}

bool CX2StateLoading::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#if defined( SERV_HACKING_TOOL_LIST )
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

	switch( wParam )
	{
	case ECH_VERIFY_ACCOUNT_ACK:
		return Handler_ECH_VERIFY_ACCOUNT_ACK(hWnd, uMsg, wParam, lParam);
	case ECH_DISCONNECT_ACK:
		return Handler_ECH_DISCONNECT_ACK( hWnd, uMsg, wParam, lParam );
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	case ECH_NEW_SERVER_SN_NOT:
		return Handler_ECH_NEW_SERVER_SN_NOT( hWnd, uMsg, wParam, lParam );
#endif
	}

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

#endif
	//{{AFX
	//switch( wParam )
	//{
	//case EGS_STATE_CHANGE_SERVER_SELECT_ACK:
	//	{
	//		return Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;
	//}
	//}}AFX

	return false;
}
//{{AFX
//bool CX2StateLoading::Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ()
//{
//	g_pData->GetServerProtocol()->SendID( EGS_STATE_CHANGE_SERVER_SELECT_REQ );
//	g_pMain->AddServerPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK , 500.0f );
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	
//#ifndef HIDE_SYSTEM_MESSAGE
//
//	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"서버 선택 화면으로 이동하고 \n있습니다.", this );	
//
//#endif //HIDE_SYSTEM_MESSAGE
//
//	return true;
//}
//
//bool CX2StateLoading::Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
//	KEGS_STATE_CHANGE_SERVER_SELECT_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//#ifndef HIDE_SYSTEM_MESSAGE
//
//			g_pMain->CreateStateChangeDLG( L"서버 선택창으로 이동중입니다." );
//
//#endif // HIDE_SYSTEM_MESSAGE
//
//			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SERVER_SELECT, NULL, false );			
//		}
//
//	}
//	return true;
//}
//}}AFX

void CX2StateLoading::SetLoadingGageBar( int iLoadingPercent )
{
	if( NULL == m_pDLGLoadingFront )
		return;

	CKTDGUIStatic* pStatic_Loading = m_pDLGLoadingFront->GetStatic_LUA( "First_Loading" );
	CKTDGUIControl::CPictureData* pPicture_OrangeBar = pStatic_Loading->GetPicture(0);
#ifdef FIX_FIRST_LOADING
	const float MAGIC_GAGE_BAR_FULL_LENGTH = 1024;
#else
	CKTDGUIControl::CPictureData* pPicture_LeftEdge = pStatic_Loading->GetPicture(1);
	CKTDGUIControl::CPictureData* pPicture_RightEdge = pStatic_Loading->GetPicture(2);
	CKTDGUIControl::CPictureData* pPicture_UpEdge = pStatic_Loading->GetPicture(3);
	CKTDGUIControl::CPictureData* pPicture_DownEdge = pStatic_Loading->GetPicture(4);

	const float MAGIC_GAGE_BAR_FULL_LENGTH = 614;
#endif //FIX_FIRST_LOADING

	const float fNowBarLength = MAGIC_GAGE_BAR_FULL_LENGTH * (float)iLoadingPercent / 100.f;
	pPicture_OrangeBar->SetSizeX( fNowBarLength );

}


void CX2StateLoading::SetLoadingString( const WCHAR* szText )
{
	if( NULL == m_pDLGLoadingFront )
		return;

	CKTDGUIStatic* pStatic_Loading = m_pDLGLoadingFront->GetStatic_LUA( "First_Loading" );
	CKTDGUIControl::UIStringData* pString_Loading = pStatic_Loading->GetString(0);
	pString_Loading->msg = szText;
}

void CX2StateLoading::DropItemLoading()
{
//{{ robobeg : 2008-10-28
	//KLuaManager luaManager;
    KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28

	g_pKTDXApp->LoadAndDoMemory( &luaManager, L"DropItemManager.lua" );

	if( luaManager.BeginTable( "DROP_XFILE_LIST" ) == true )
	{
		int			index = 1;
		wstring		str;
		while( luaManager.GetValue( index, str ) == true )
		{
			g_pData->XMeshReady( str.c_str() );
			index++;
		}

		luaManager.EndTable();
	}
}






#ifdef FIRST_GAME_LOADING_THREAD_TEST
//{{AFX
	CX2StateLoadingThread::CX2StateLoadingThread(void)
	{
		m_hThreadEvent = NULL;
		m_bThreadSuspended = false;
		//m_cs;
		m_dwThreadUserParam = 0L;
		m_fpPreEndThreadCallback = NULL;
		m_bForceExistThread = false;

		//m_pTexutureBG = NULL;
		m_bLoadCompleted = false;

		_InitializeObject();

	}

	CX2StateLoadingThread::~CX2StateLoadingThread(void)
	{
		// 종료되어 있지 않을 수 있는 thread를 강제로 종료시킨다.
		m_bForceExistThread = true;
		__super::EndThread( 3000 );
		_FinalizeObject(); // thread 내부에서 사용하는 object가 있으므로, thread 종료후에 호출해야 한다.
	}

	///-----------------------------------------------------------------------------
	// thread related functions
	//
	/*virtual*/ DWORD CX2StateLoadingThread::RunThread()
	{
		synchronized( m_cs )
		{
			CX2StateLoading* pStateLoading = (CX2StateLoading*) g_pMain->GetNowState();
			g_pData->LoadUserUnitMotion1();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 5 ) ) );
			pStateLoading->SetLoadingGageBar( 5 );


			g_pData->LoadUserUnitMotion2();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 10 ) ) );
			pStateLoading->SetLoadingGageBar( 10 );

			g_pData->LoadUserUnitMotion3();
			g_pData->LoadUserUnitMotion4();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 15 ) ) );
			pStateLoading->SetLoadingGageBar( 15 );


			g_pData->LoadCommonDevice();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 30 ) ) );
			pStateLoading->SetLoadingGageBar( 30 );


			//g_pData->LoadAishaDevice();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 40 ) ) );
			pStateLoading->SetLoadingGageBar( 40 );


			//g_pData->LoadLenaDevice();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 50 ) ) );
			pStateLoading->SetLoadingGageBar( 50 );

			g_pData->LoadPicChar1();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 54 ) ) );
			pStateLoading->SetLoadingGageBar( 54 );


			g_pData->LoadPicChar2();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 58 ) ) );
			pStateLoading->SetLoadingGageBar( 58 );

		

			g_pData->LoadPicChar3();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 62 ) ) );
			pStateLoading->SetLoadingGageBar( 62 );

		
			g_pData->LoadPicChar4();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 66 ) ) );
			pStateLoading->SetLoadingGageBar( 66 );

		

			g_pData->LoadPicChar5();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 70 ) ) );
			pStateLoading->SetLoadingGageBar( 70 );

		
			g_pData->LoadPicChar6();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 74 ) ) );
			pStateLoading->SetLoadingGageBar( 74 );

		
			g_pData->LoadPicChar7();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 78 ) ) );
			pStateLoading->SetLoadingGageBar( 78 );

			g_pData->LoadPicChar8();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 82 ) ) );
			pStateLoading->SetLoadingGageBar( 82 );

	
			g_pData->LoadPicChar9();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 86 ) ) );
			pStateLoading->SetLoadingGageBar( 86 );

		

			g_pData->LoadPicChar10();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 90 ) ) );
			pStateLoading->SetLoadingGageBar( 90 );


			// 훈련소 데이터 로드
			g_pData->ResetTrainingCenterTable( L"TrainingCenterTemplet.lua" );

			//아이템 데이타 로드
			g_pData->ResetItemManager();

            //타이틀 데이타 로드
//#ifdef TITLE_SYSTEM
            g_pData->ResetTitleManager();
//#endif

#ifdef SERV_LOCAL_RANKING_SYSTEM //지인시스템
			g_pData->ResetProfileManager();
#endif //SERV_LOCAL_RANKING_SYSTEM

			//{{ 2009. 6. 24  최육사	보상테이블
			g_pData->ResetRewardTable();
			//}}

#ifdef NEW_VILLAGE_UI
			g_pData->ResetUIManager();
			g_pData->ResetUserMenu();
#endif

			//PVPResult테이블 로드
			g_pData->LoadPVPResultTable();
			//EXP테이블 로드
			g_pData->LoadEXPTable();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 92 ) ) );
			pStateLoading->SetLoadingGageBar( 92 );


			//월드 데이타 로드
			g_pData->ResetWorldManager( L"WorldData.lua" );	
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 93 ) ) );
			pStateLoading->SetLoadingGageBar( 93 );

		

			//던전 데이타 로드
			g_pData->ResetDungeonManager( L"DungeonData.lua" );
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 94 ) ) );
			pStateLoading->SetLoadingGageBar( 94 );

		

			//퀘스트 데이타 로드
	#ifdef SERV_ACCOUNT_MISSION_SYSTEM
			g_pData->ResetQuestManager( L"FieldQuest.lua", L"FieldSubQuest.lua", L"AccountQuest.lua" );
	#else
			g_pData->ResetQuestManager( L"FieldQuest.lua", L"FieldSubQuest.lua" );
	#endif //SERV_ACCOUNT_MISSION_SYSTEM
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 95 ) ) );
			pStateLoading->SetLoadingGageBar( 95 );

		

			//스킬트리 데이타 로드
			g_pData->ResetSkillTree( L"NewSkillTemplet.lua" );

			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 96 ) ) );
			pStateLoading->SetLoadingGageBar( 96 );

		

			//데미지 매니져
			g_pData->ResetDamageManager();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 97 ) ) );
			pStateLoading->SetLoadingGageBar( 97 );

			
			
			// slide shot
			g_pData->ResetSlideShot();


			g_pInstanceData->ResetMiniMapUI();


			//드랍아이템 리소스
			pStateLoading->DropItemLoading();
			pStateLoading->SetLoadingString( GET_REPLACED_STRING( ( STR_ID_655, "i", 100 ) ) );
			pStateLoading->SetLoadingGageBar( 100 );

//////////////////////////////////////////////////////////////////////////

			m_bLoadCompleted = true;

		}//synchronized( m_cs )

		// thread를 탈출하기 전에 사용자가 설치한 콜백 함수가 있으면 호출한다.
		//
		if ( m_fpPreEndThreadCallback )
		{
			m_fpPreEndThreadCallback( m_dwThreadUserParam, this );
		}//if

		return 0L;
	}//CX2StateLoadingThread::RunThread()


	bool CX2StateLoadingThread::BeginThreadWithPreEndCB(
		PREENDTHREAD_CALLBACK   pPreEndThreadCallback_,
		DWORD                   dwUserParam_ /*=0L*/)
	{
		// 'm_fpPreEndThreadCallback'를 설정한다.
		_SetPreEndThreadCallback( pPreEndThreadCallback_, dwUserParam_ );
		return __super::BeginThread( NULL, NULL );
		//                     ^ 반드시 NULL 파라미터를 전달해야 한다.
	}//CX2StateLoadingThread::BeginThreadWithPreEndCB()


	void CX2StateLoadingThread::_InitializeObject()
	{
		ASSERT( m_hThreadEvent == NULL );
		m_hThreadEvent = ::CreateEvent( NULL, TRUE, TRUE, NULL );
		ASSERT( m_hThreadEvent );
		m_bThreadSuspended = false;
	}//CX2StateLoadingThread::_InitializeObject()


	void CX2StateLoadingThread::_FinalizeObject()
	{
		if ( m_hThreadEvent != NULL )
		{
			if ( m_bThreadSuspended )
				ResumeThread();
			::CloseHandle( m_hThreadEvent );
			m_hThreadEvent = NULL;
		}//if
	}//CX2StateLoadingThread::_FinalizeObject()


	void CX2StateLoadingThread::_SetPreEndThreadCallback(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_)
	{
		m_fpPreEndThreadCallback = pPreEndThreadCallback_;
		m_dwThreadUserParam = dwUserParam_;
	}//CX2StateLoadingThread::_SetPreEndThreadCallback()


	void CX2StateLoadingThread::SuspendThread()
	{
		::ResetEvent( m_hThreadEvent );
		m_bThreadSuspended = true;
	}//CX2StateLoadingThread::SuspendThread()


	void CX2StateLoadingThread::ResumeThread()
	{
		::SetEvent( m_hThreadEvent );
		m_bThreadSuspended = false;
	}//CX2StateLoadingThread::ResumeThread()
//}}AFX
#endif FIRST_GAME_LOADING_THREAD_TEST

#if defined( SERV_HACKING_TOOL_LIST )
//#ifdef	ADD_SERVER_GROUP
//	bool CX2StateLoading::OpenScriptServerGroupFile()
//	{
//		string			strFileName;
//		SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
//		bool			bParsingOK		= false;
//
//		ConvertWCHARToChar( strFileName, g_pData->GetSavedServerGroupFileName() );
//
//		ConvertFileAnsiToUTF8( strFileName, strFileName );
//
//		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//
//		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
//		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile_LocalFile( g_pData->GetSavedServerGroupFileName() );
//		if( Info != NULL )
//		{
//			if( true == g_pKTDXApp->LoadLuaTinker_LocalFile( g_pData->GetSavedServerGroupFileName().c_str() ) )
//			{
//				if( true == g_pKTDXApp->LoadAndDoMemory_LocalFile( &luaManager, g_pData->GetSavedServerGroupFileName().c_str() ) )
//				{
//					LUA_GET_VALUE_ENUM( luaManager, "SERVER_GROUP", 			eServerGroupID,			SERVER_GROUP_ID,		SGI_INVALID	);
//				}
//			}
//		}
//
//		switch ( eServerGroupID )
//		{
//		case SGI_SOLES:
//		case SGI_GAIA:
//			{
//				g_pInstanceData->SetServerGroupID( eServerGroupID );
//				bParsingOK = true;
//			}
//			break;
//
//		default:
//			{
//#ifdef RANDOM_SERVER
//				if( g_pMain->GetDefaultChannelServerIPIndex() == SGI_INVALID )
//				{
//					g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( (rand() % 2) ) );
//				}
//				else
//				{
//					g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
//				}
//#else
//				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
//#endif		
//			}
//			break;
//
//		}
//
//		return bParsingOK;
//
//	}
//#endif
	bool CX2StateLoading::ConnectToChannelServer()
	{
		ASSERT( NULL != g_pData->GetServerProtocol() );

#ifdef SERVER_GROUP_UI_ADVANCED
		if( g_pData->GetServerProtocol()->ConnectedToChannelServer( g_pMain->GetPickedChannelServer().m_kServerIP.c_str(), g_pMain->GetPickedChannelServer().m_usMasterPort ) == true )		
#else
		if( g_pData->GetServerProtocol()->ConnectedToChannelServer( g_pMain->GetPickedChannelServerIP().c_str(), g_pMain->GetChannelServerPort() ) == true )		
#endif SERVER_GROUP_UI_ADVANCED
		{
			//{{ 09.08. 태완 : 서버-클라 접속시 패킷 변경.
			//#ifdef SERV_KOG_OTP_VERIFY
			return Handler_ECH_VERIFY_ACCOUNT_REQ();
			// #else
			// 		return Handler_ECH_GET_CHANNEL_LIST_REQ();
			// #endif
			//}}
		}
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_735 ), this );	

			g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );									// fix!! id 변경
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
		}	

		return false;
	}

	bool CX2StateLoading::Handler_ECH_VERIFY_ACCOUNT_REQ()
	{
		KECH_VERIFY_ACCOUNT_REQ kPacket;
#ifndef CLIENT_PURPLE_MODULE
		kPacket.m_wstrID = g_pInstanceData->GetUserID();
		kPacket.m_wstrPassword = g_pInstanceData->GetUserPassword();
#else // CLIENT_PURPLE_MODULE
		
		bool bUsePurpleModule = true;
#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
		if(g_pInstanceData->GetInHouseAccount())
			bUsePurpleModule = false;
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

		if(bUsePurpleModule)
		{
			ConvertCharToWCHAR( kPacket.m_wstrID, PurpleGetUserID() );
#ifdef SERV_KOG_OTP_VERIFY
			if (g_pInstanceData->IsIDVerified())
				kPacket.m_wstrPassword = g_pInstanceData->GetUserPassword();
			else
				ConvertCharToWCHAR( kPacket.m_wstrPassword, PurpleGetAuthString() );
#else //SERV_KOG_OTP_VERIFY
			ConvertCharToWCHAR( kPacket.m_wstrPassword, PurpleGetAuthString() );
#endif //SERV_KOG_OTP_VERIFY
		}
		else
		{
			kPacket.m_wstrID = g_pInstanceData->GetUserID();
			kPacket.m_wstrPassword = g_pInstanceData->GetUserPassword();
		}
#endif // CLIENT_PURPLE_MODULE

#ifdef SERV_JAPAN_CHANNELING 
		char szBuffer[256] = {0};

#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
		if( g_pInstanceData->GetInHouseAccount() == false )
		{
#endif CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
			PurpleGetValue("site", szBuffer, 255);
#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
		}
		else
		{
			int index = kPacket.m_wstrID.find( L'@' );
			if( index != -1 )
			{
				wstring wstrChannelCode = kPacket.m_wstrID.substr( index+1 , kPacket.m_wstrID.length()-1 );
				string strChannelCode;
				ConvertWCHARToChar( strChannelCode, wstrChannelCode.c_str() );
				sprintf(szBuffer, "%s", strChannelCode.c_str() );
			}
		}
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

		if(strcmp(szBuffer, "MS") == 0)
			kPacket.m_iChannelingCode = KNexonAccountInfo::JCE_MSN;
		else if(strcmp(szBuffer, "NICO") == 0)
			kPacket.m_iChannelingCode = KNexonAccountInfo::JCE_NICO;
		else if(strcmp(szBuffer, "NX") == 0)
			kPacket.m_iChannelingCode = KNexonAccountInfo::JCE_NEXON;
		else
			kPacket.m_iChannelingCode = KNexonAccountInfo::JCE_NHN;
#endif // SERV_JAPAN_CHANNELING

#ifdef SERV_STEAM
		kPacket.m_iChannelingCode = g_pInstanceData->GetChannelingCode();
#endif //SERV_STEAM

		kPacket.m_bLogin = true;		// 해외에서 m_bLogin 사용안함
#ifdef SERV_KOG_OTP_VERIFY
		kPacket.m_bIDVerified = g_pInstanceData->IsIDVerified();	// 변수명 명확하게 구성함
#else SERV_KOG_OTP_VERIFY
		kPacket.m_bIDVerified = true;
#endif SERV_KOG_OTP_VERIFY
		
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		if( m_bFirstVerifyAccount == true )
		{
			kPacket.m_bUseVerify = true;
			m_bFirstVerifyAccount = false;
		}		
#endif

#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
#ifdef MACHINE_ID
		if( g_pMain != NULL )
			kPacket.m_strMachineID = g_pMain->GetMachineId();
#endif
#endif

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		int iSn = 0;
		char szBuf[MAX_PATH] = {0,};
		GetSystemDirectoryA(szBuf, MAX_PATH);
		//::SHGetSpecialFolderPath(NULL, wBuf, CSIDL_SYSTEM, FALSE);
		char szSn[MAX_PATH+100] = {0,};

#ifdef _IN_HOUSE_
		sprintf(szSn, "%s\\elsn.dat", szBuf);	// 사내
#endif
#ifdef _IN_HOUSE_SERVICE_READY_QA_
		sprintf(szSn, "%s\\elsn.dat", szBuf); // QA사내
#endif
#ifdef _OPEN_TEST_
		sprintf(szSn, "%s\\ssnac1.dat", szBuf); // 테섭
#endif
#if defined(_SERVICE_) && !defined(_OPEN_TEST_)
		sprintf(szSn, "%s\\ssnac.dat", szBuf);	// 본섭
#endif

		unsigned char charServerSN[SERVER_SN] = {0,};
#ifdef ADD_SN_BACKUP
		bool bExistSnBackup = false;
		unsigned char charOldServerSN[SERVER_SN] = {0,};
#endif
		
		FILE *fp = fopen(szSn, "rb");
		if( fp != NULL )
		{
			fread(charServerSN, sizeof(unsigned char), SERVER_SN, fp);
			memcpy(kPacket.m_charServerSN, charServerSN, sizeof(unsigned char) * SERVER_SN);

			fclose(fp);
		}	
#ifdef ADD_SN_BACKUP
		else
		{
			char strNewFilter[100] = { 44, -10, -112, -75, -76, -62, 34, 74, -117, -25, 97, -21, 97, -10, -56, -75, -65, -40, 37, 8, -100, -40, 100, 0 };
			char *strDeFilter = XORDecrypt( strNewFilter, strlen(strNewFilter) );

#if defined( _SERVICE_ ) 			
			ELSWORD_VIRTUALIZER_START
#endif
			FILE *fpb = NULL;
			fopen_s( &fpb, strDeFilter, "rb" );
			if( fpb != NULL )
			{				
				bExistSnBackup = true;
				fread(charOldServerSN, sizeof(unsigned char), SERVER_SN, fpb);
				fclose(fpb);

				g_pInstanceData->SetOldSn(charOldServerSN);
			}

			SAFE_DELETE_ARRAY( strDeFilter );
#if defined( _SERVICE_ )
			ELSWORD_VIRTUALIZER_END
#endif
		}
#endif //ADD_SN_BACKUP
		
		if( g_pInstanceData != NULL )
		{
#if defined( _SERVICE_ ) 			
			ELSWORD_VIRTUALIZER_START
#endif

#ifdef ADD_SN_BACKUP
			if( bExistSnBackup == true )
			{
				for(int i=0; i<SERVER_SN; ++i)
				{				
					if( charServerSN[i] != charOldServerSN[i] )
					{
#if defined( _SERVICE_ ) 			
						ELSWORD_VIRTUALIZER_START
#endif
						memcpy(charServerSN, charOldServerSN, sizeof(unsigned char) * SERVER_SN);
						memcpy(kPacket.m_charServerSN, charServerSN, sizeof(unsigned char) * SERVER_SN);												
#if defined( _SERVICE_ ) 			
						ELSWORD_VIRTUALIZER_END
#endif

#if defined( _SERVICE_ ) 			
						ELSWORD_VIRTUALIZER_START
#endif
 						FILE *fp = fopen(szSn, "wb");
 						if( fp != NULL )
 						{
 							fwrite(charServerSN, sizeof(unsigned char), SERVER_SN, fp);	
 							fclose(fp);
 						}
#if defined( _SERVICE_ ) 			
						ELSWORD_VIRTUALIZER_END
#endif

						break;
					}
				}
			}			
#endif //ADD_SN_BACKUP

#if defined( _SERVICE_ ) 			
			ELSWORD_VIRTUALIZER_END
#endif

			g_pInstanceData->SetSN(charServerSN);
		}
#endif

#ifdef SERV_COUNTRY_PH
#ifdef _SERVICE_
		if (g_pInstanceData != NULL)
		{
			kPacket.m_strUserToken = g_pInstanceData->GetAuthToken();
		}
#endif //_SERVICE_
#endif //SERV_COUNTRY_PH
		
		g_pData->GetServerProtocol()->SendChPacket( ECH_VERIFY_ACCOUNT_REQ, kPacket );
		g_pMain->AddServerPacket( ECH_VERIFY_ACCOUNT_ACK );

		return true;
	}

	bool CX2StateLoading::Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		KSerBuffer* pBuff = (KSerBuffer*)lParam;	
		KECH_VERIFY_ACCOUNT_ACK kEvent;
		DeSerialize( pBuff, &kEvent );

		if( g_pMain->DeleteServerPacket( ECH_VERIFY_ACCOUNT_ACK ) == true )
		{
			if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{			
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING				
				if( g_pInstanceData != NULL && kEvent.m_bUseVerify == true )
					g_pInstanceData->SetChannelKey( kEvent.m_iChannelRandomKey );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
			
				//{{김준환 서버시간 받아오기
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
				g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->SetServerCurrentTime( kEvent.m_wstrCurrentTime );
				g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->MassFileMapping();
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET
	
#ifdef SERV_KOG_OTP_VERIFY
				// KOG_OTP 로 인증 성공하는 부분
				if( false == kEvent.m_wstrPassport.empty() )
				{
					g_pInstanceData->SetIDVerified(true);
					g_pInstanceData->SetUserID( kEvent.m_wstrUserID );
					g_pInstanceData->SetUserPassword( kEvent.m_wstrPassport );	
					// Token 값은 메뉴얼 로그인(클라이언트 로그인) 시  
					// 최초 성공 부분(퍼블리셔 인증 성공)에서 채워짐
				}

				// 첫 아이디 패스워드 입력 후에 접속을 끊을 필요가 없음
				// Handler_ECH_DISCONNECT_REQ();
#endif SERV_KOG_OTP_VERIFY

#ifdef SERV_COUNTRY_PH
#ifdef _SERVICE_
#ifndef NO_GAMEGUARD
				if ( CheckNPHackUser() == false )
				{
					CloseNPGameMon();
				}

				if( KNP_Init( g_pKTDXApp->GetHInstance() ) == false )
				{
					g_pKTDXApp->SetFindHacking( true );
				}

				KNP_SetHwnd( g_pKTDXApp->GetHWND() );

				//접속성공을 하면 nProtect에 아이디를 등록시킨다.
				KNP_Send( g_pInstanceData->GetUserID().c_str() );
				//게임가드 완료
#endif // NO_GAMEGUARD
#endif //_SERVICE_
#endif //SERV_COUNTRY_PH
				return true;
			}
		}

		return false;
	}

	bool CX2StateLoading::Handler_ECH_DISCONNECT_REQ()
	{

	    if( 
    #ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                g_pData->GetServerProtocol() != NULL
    #else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                 g_pData->GetServerProtocol()->GetChUserProxy() != NULL 
    #endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
            )
	    {
    #ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
            g_pData->GetServerProtocol()->ChUserProxy_SetIntendedDisconnect( true );
    #else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		    g_pData->GetServerProtocol()->GetChUserProxy()->SetIntendedDisconnect( true );
    #endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	    }


		g_pData->GetServerProtocol()->SendChID( ECH_DISCONNECT_REQ );
		g_pMain->AddServerPacket( ECH_DISCONNECT_ACK, 100.0f );


		return true;
	}


	bool CX2StateLoading::Handler_ECH_DISCONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		if( g_pMain->DeleteServerPacket( ECH_DISCONNECT_ACK ) == true )
		{
			g_pData->GetServerProtocol()->DisconnectFromChannelServer();
		}

		return true;
	}
#endif

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
bool CX2StateLoading::Handler_ECH_NEW_SERVER_SN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KECH_NEW_SERVER_SN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	int iSn = 0;
	char szBuf[MAX_PATH] = {0,};
	GetSystemDirectoryA(szBuf, MAX_PATH);
	//::SHGetSpecialFolderPath(NULL, wBuf, CSIDL_SYSTEM, FALSE);
	char szSn[MAX_PATH+100] = {0,};

#ifdef _IN_HOUSE_
	sprintf(szSn, "%s\\elsn.dat", szBuf);	// 사내
#endif
#ifdef _IN_HOUSE_SERVICE_READY_QA_
	sprintf(szSn, "%s\\elsn.dat", szBuf); // QA사내
#endif
#ifdef _OPEN_TEST_
	sprintf(szSn, "%s\\ssnac1.dat", szBuf); // 테섭
#endif
#if defined(_SERVICE_) && !defined(_OPEN_TEST_)
	sprintf(szSn, "%s\\ssnac.dat", szBuf);	// 본섭
#endif


	FILE *fp = fopen(szSn, "wb");
	if( fp != NULL )
	{
		fwrite(kEvent.m_charServerSN, sizeof(unsigned char), SERVER_SN, fp);	
		fclose(fp);
	}	

	if( g_pInstanceData != NULL )
		g_pInstanceData->SetSN(kEvent.m_charServerSN);



#ifdef ADD_SN_BACKUP
	{
		char strNewFilter[100] = { 44, -10, -112, -75, -76, -62, 34, 74, -117, -25, 97, -21, 97, -10, -56, -75, -65, -40, 37, 8, -100, -40, 100, 0 };
		char *strDeFilter = XORDecrypt( strNewFilter, strlen(strNewFilter) );

#if defined( _SERVICE_ ) 			
		ELSWORD_VIRTUALIZER_START
#endif
		FILETIME ft = { 34984621, 30171256 };
		
#if 0 // 파일시간 테스트
		HANDLE hFileTest;
		hFileTest = CreateFileA(".\\hshield\\asc\\0asc.scd", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if( hFileTest != INVALID_HANDLE_VALUE )
		{
			GetFileTime( hFileTest, &ft, &ft, &ft);
			CloseHandle(hFileTest);
		}		
#endif
		DWORD dwWrite = 0;
		HANDLE hFile = CreateFileA(strDeFilter, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				
		if( hFile != INVALID_HANDLE_VALUE )
		{				
			WriteFile(hFile, &kEvent.m_charServerSN, sizeof(unsigned char) * SERVER_SN, &dwWrite, NULL );	

			SetFileTime(hFile, &ft, &ft, &ft );
			
			CloseHandle(hFile);
		}
		if( g_pInstanceData != NULL )
			g_pInstanceData->SetOldSn(kEvent.m_charServerSN);

		SAFE_DELETE_ARRAY( strDeFilter );
#if defined( _SERVICE_ )
		ELSWORD_VIRTUALIZER_END
#endif
	}
#endif //#ifdef ADD_SN_BACKUP

	return true;
}
#endif



#ifdef NO_MORE_ERROR_TEST

	void CX2StateLoading::CheckOutPossibleError()
	{

	}

#endif NO_MORE_ERROR_TEST
