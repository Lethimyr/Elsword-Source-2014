#include "StdAfx.h"


#include ".\x2statestartup.h"
	  
CX2StateStartUp::CX2StateStartUp(void)
#ifdef FIRST_SCREEN_TEST
:
// 2009.01.09 김태완 : 초기화 코드정리
m_pTexutureBG(NULL)
#endif
{
#ifdef DXUT_KTDXLIB_SAFE_ON_RESET
	HRESULT hr = S_OK;
	g_pKTDXApp->SkipFrame();
#endif

#ifdef FIRST_SCREEN_TEST
	m_pTexutureBG = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Unit_BackGround.tga" );

#ifdef DXUT_KTDXLIB_SAFE_ON_RESET
	if ( DXUTGetDeviceLost() == false && DXUTGetDeviceObjectsReset() == true )
	{
		g_pKTDXApp->GetDevice()->BeginScene();
		OnFrameRender();
		g_pKTDXApp->GetDevice()->EndScene();
		hr = g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
		if( FAILED(hr) )
		{
			if( D3DERR_DEVICELOST == hr )
			{
				DXUTSetDeviceLost( true );
			}
			else if( D3DERR_DRIVERINTERNALERROR == hr )
			{
				// When D3DERR_DRIVERINTERNALERROR is returned from Present(),
				// the application can do one of the following:
				// 
				// - End, with the pop-up window saying that the application cannot continue 
				//   because of problems in the display adapter and that the user should 
				//   contact the adapter manufacturer.
				//
				// - Attempt to restart by calling IDirect3DDevice9::Reset, which is essentially the same 
				//   path as recovering from a lost device. If IDirect3DDevice9::Reset fails with 
				//   D3DERR_DRIVERINTERNALERROR, the application should end immediately with the message 
				//   that the user should contact the adapter manufacturer.
				// 
				// The framework attempts the path of resetting the device
				// 
				DXUTSetDeviceLost( true );
			}
		}
	}
#else
	g_pKTDXApp->GetDevice()->BeginScene();
	OnFrameRender();
	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
#endif
#endif //FIRST_SCREEN_TEST

	//프로그램 시작시 최초 진입하는 스테이트
	//필요한 전역 데이터들을 만들고 다음 스테이트로 들어간다
	LoadData();

	// 비디오 메모리가 40MB 이하
	DWORD totalTextureMem		= 0;
	DWORD availableTextureMem	= 0;
	g_pKTDXApp->GetVideoRam( totalTextureMem, availableTextureMem );
	totalTextureMem = totalTextureMem/1024/1024;
	if( totalTextureMem <= 40 )	
	{
		// 비디오 메모리가 일정 크기 이하이면 해상도를 최하로, color format도 16bit로,
		g_pMain->GetGameOption().SetResolution( 640, 480, 16 );
	}

	g_pMain->GetGameOption().ApplyAllOption( true );

	// 게임 시작할 때 무조건 풀스크린으로 시작하게
	//if( true == DXUTIsWindowed() )
	//{
	//	DXUTToggleFullScreen();
	//}
	//::SetFocus( g_pKTDXApp->GetHWND() );

#ifdef GET_ELSWORD_FOCUS
	SetForegroundWindow( g_pKTDXApp->GetHWND() );
	SetFocus( g_pKTDXApp->GetHWND() );
#endif GET_ELSWORD_FOCUS

#ifndef SERV_SERVER_TIME_GET
#ifndef X2TOOL
	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_LOGIN, NULL, false );
#endif
#endif SERV_SERVER_TIME_GET


//
//#ifndef _SERVICE_
//
//		AllocConsole();
//		g_fpStream = freopen("CONOUT$", "a", stdout);		// redirect printf to console
//		g_fpStreamError = freopen("CONOUT$", "a", stderr);	// redirect printf to console
//		dbg::logfile::SetLogLevel(2);
//
//#endif _SERVICE_

	
	g_pKTDXApp->SkipFrame();

}

CX2StateStartUp::~CX2StateStartUp(void)
{
}

void CX2StateStartUp::LoadData()
{
	//UDP 초기화
	g_pData->ResetGameUDP( g_pMain->GetGameP2PPort() );

	g_pData->ReadyCharacterMotion();

	//UI 파티클 시스템 생성
	g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
	g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );

	//게임 파티클 시스템 생성
	g_pData->ResetGameMajorParticle( L"GameMajorParticle.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle2.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle3.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle4.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle5.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle6.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle7.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle8.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle9.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle10.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle11.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle12.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle13.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle14.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle15.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle16.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle17.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle18.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle19.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle20.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle21.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle22.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle23.txt" );
	
	g_pData->ResetGameMinorParticle( L"GameMinorParticle.txt" );

	//UI 메시플레이어
	g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
	g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );

	//게임 메시플레이어
	g_pData->ResetGameMajorXMeshPlayer( L"GameMajorXMeshPlayer.txt" );
	g_pData->ResetGameMinorXMeshPlayer( L"GameMinorXMeshPlayer.txt" );


	g_pData->ResetUIEffectSet( L"UIEffectSet.lua" );
	g_pData->ResetGameEffectSet( L"EffectSet.lua" );

#ifdef INT_RESOURCE_DEVIDE
	g_pData->GetUIMajorParticle()->OpenScriptFile( L"UIMajorParticle_INT.txt" );
	//g_pData->GetUIMinorParticle()->OpenScriptFile( L"UIMinorParticle_INT.txt" ); 현재 쓸일 없음
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle_INT.txt" );
	g_pData->GetGameMinorParticle()->OpenScriptFile( L"GameMinorParticle_INT.txt" );
	g_pData->GetUIMajorXMeshPlayer()->OpenScriptFile( L"UIMajorXMeshPlayer_INT.txt" );
	//g_pData->GetUIMinorXMeshPlayer()->OpenScriptFile( L"UIMinorXMeshPlayer_INT.txt" ); 현재 쓸일 없음
	g_pData->GetGameMajorXMeshPlayer()->OpenScriptFile( L"GameMajorXMeshPlayer_INT.txt" );
	//g_pData->GetGameMinorXMeshPlayer()->OpenScriptFile( L"GameMinorXMeshPlayer_INT.txt" ); 현재 쓸일 없음
	g_pData->GetUIEffectSet()->OpenScriptFile( L"UIEffectSet_INT.lua" );
	g_pData->GetGameEffectSet()->OpenScriptFile( L"EffectSet_INT.lua" );
#endif INT_RESOURCE_DEVIDE

#ifndef X2TOOL
	CX2BuffTempletManager::GetInstance()->OpenScriptFileForTemplet();
	CX2BuffTempletManager::GetInstance()->OpenScriptFileForFactor();
#endif

	//유닛 데이타 로드
	g_pData->ResetUnitManager( L"UnitTemplet.lua", L"NPCTemplet.lua", L"NPCStat.lua" );
	g_pData->ResetUIUnitManager();

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	g_pData->ResetSocketItem( L"OptionItemData.lua", L"SocketOptionForm.lua" );
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	
	g_pData->ResetEnchantItem();
	
	// 파티 매니저 로드
	g_pData->ResetPartyManager();

	g_pMain->GetGameOption().OpenScriptFile();

#ifdef OPTIMIZATION_DEV_MODE
	g_pMain->GetGameOption().SetTexDetail( CX2GameOption::OL_LOW );
	g_pMain->GetGameOption().SetUnitDetail( CX2GameOption::OL_LOW );
	g_pMain->GetGameOption().SetMapDetail( CX2GameOption::OL_LOW );
	g_pMain->GetGameOption().SetFieldDetail( CX2GameOption::FL_LOW );
#endif OPTIMIZATION_DEV_MODE

	g_pData->ResetLocationManager();

	// Field_Data.lua 파싱
	g_pData->ResetBattleFieldManager( L"BattleFieldData.lua" );
}

#ifdef FIRST_SCREEN_TEST

HRESULT CX2StateStartUp::OnFrameRender()
{
#ifndef X2TOOL
	m_pTexutureBG->Draw( 0, 0, 1024, 768 );
#endif
	return S_OK;
}
#endif // FIRST_SCREEN_TEST
