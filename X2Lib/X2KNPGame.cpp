#include "StdAfx.h"
#include ".\x2knpgame.h"

#include <direct.h>
#include "../x2/resource.h"
#include "./X2SMTPMail.h"

CNPGameLib*	pNpgl = NULL;

extern CX2SMTPMail* g_pX2SMTPMail; // x2.cpp에 정의되어있음
bool g_bCheckHackUser = true;

bool CheckNPHackUser()
{
#ifdef CHECK_NP_HACKUSER
	if ( g_bCheckHackUser == false )
	{
		return false;
	}

	if ( g_pData != NULL && g_pData->GetMyUser() != NULL && 
		g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
	{
		return true;
	}

	return false;
#else
    return true;
#endif
}

LRESULT CALLBACK SendGameGuardMailDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		{
			//g_pX2SMTPMail->SetReceiverEMail( "Game1@inca.co.kr" ); 
			g_pX2SMTPMail->SetReceiverEMail( "" ); 
			
			g_pX2SMTPMail->SetSenderName( "엘소드유저" );
			g_pX2SMTPMail->SetSubject( "게임가드 오류레포트입니다." );
			g_pX2SMTPMail->SetDesc( "게임가드 폴더 내의 .erl 파일을 첨부하였습니다." );

			g_pX2SMTPMail->ClearFile();			
			std::vector< std::string > vecFiles;

			char* buffer = NULL;
			if( (buffer = _getcwd( NULL, 255 )) == NULL )
			{
				// error
				return TRUE;
			}

			stringstream strmFolder;
			strmFolder << buffer << "\\GameGuard";
			free(buffer);

			// npgl.erl이 멜에 첨부가 안되는 경우가 있어서 파일을 복사해서 보내도록 한다
			stringstream strmSrcNPGL;
			strmSrcNPGL << strmFolder.str() << "\\npgl.erl";
			stringstream strmDestNPGL;
			strmDestNPGL << strmFolder.str() << "\\npgl_copy.erl";
			CopyFileA( strmSrcNPGL.str().c_str(), strmDestNPGL.str().c_str(), FALSE );



			g_pX2SMTPMail->FindFilesByExt( vecFiles, strmFolder.str().c_str(), ".erl" ); 
			//g_pX2SMTPMail->FindFilesByExt( vecFiles, "C:\\Nexon\\ElSword\\Data\\GameGuard", ".erl" );

			for( UINT i=0; i<vecFiles.size(); i++ )
			{
				g_pX2SMTPMail->AddFile( vecFiles[i].c_str());
			}

			bool bResult = g_pX2SMTPMail->SendMail( hDlg );

			return TRUE;
		} break;

	case WM_COMMAND:
		{
			return TRUE;
		} break;
	}
	return FALSE;
}



#ifdef ALWAYS_INTERNAL_NPGE_PACKING
bool KNP_Init( HINSTANCE hInst , string TempArg)
#else //ALWAYS_INTERNAL_NPGE_PACKING
bool KNP_Init( HINSTANCE hInst )
#endif ALWAYS_INTERNAL_NPGE_PACKING
{
#ifdef NO_GAMEGUARD
	return true;
#endif

	if ( CheckNPHackUser() == false )
	{
		return true;		
	}

#ifdef GAMEGUARD_INI_COUNTRY
	// 해외팀 각국가 전용 디파인
#if defined( CLIENT_COUNTRY_JP )
#ifdef _OPEN_TEST_
	pNpgl = new CNPGameLib( L"ElswordJPTest" );
#else // _OPEN_TEST_
	pNpgl = new CNPGameLib( L"ElswordJP" );
#endif
#elif defined( CLIENT_COUNTRY_CN )
	#ifdef _OPEN_TEST_
		pNpgl = new CNPGameLib( L"ElswordCNTest" );
	#else // _OPEN_TEST_
		pNpgl = new CNPGameLib( L"ElswordCN" );
	#endif
#elif defined( CLIENT_COUNTRY_TH )
#ifdef _OPEN_TEST_
	if( TempArg.compare("CloseOnStart") == 0 )
	{
		pNpgl = new CNPGameLib( L"ElswordTHINT" );
	}
	else
	{
		pNpgl = new CNPGameLib( L"ElswordTHTest" );
	}
#else _OPEN_TEST_
	if( TempArg.compare("CloseOnStart") == 0 )
	{
		pNpgl = new CNPGameLib( L"ElswordTHINT" );
	}
	else
	{
		pNpgl = new CNPGameLib( L"ElswordTH" );
	}
#endif _OPEN_TEST_

#elif defined( CLIENT_COUNTRY_ID )

#ifdef _OPEN_TEST_
if( TempArg.compare("CloseOnStart") == 0 )
{
	pNpgl = new CNPGameLib( L"ElswordIDINT" );
}
else
{
	pNpgl = new CNPGameLib( L"ElswordIDTest" );
}
#else _OPEN_TEST_
if( TempArg.compare("CloseOnStart") == 0 )
{
	pNpgl = new CNPGameLib( L"ElswordIDINT" );
}
else
{
	pNpgl = new CNPGameLib( L"ElswordID" );
}
#endif _OPEN_TEST_
	
#elif defined( CLIENT_COUNTRY_BR )
#ifdef _OPEN_TEST_
	pNpgl = new CNPGameLib( L"ElswordBRTest" );
#else _OPEN_TEST_
	pNpgl = new CNPGameLib( L"ElswordBR" );
#endif _OPEN_TEST_

#elif defined( CLIENT_COUNTRY_PH )
#ifdef _OPEN_TEST_
	pNpgl = new CNPGameLib( L"ElswordPHTest" );
#else _OPEN_TEST_
	pNpgl = new CNPGameLib( L"ElswordPH" );
#endif _OPEN_TEST_
#endif

#else // GAMEGUARD_INI_COUNTRY
	pNpgl = new CNPGameLib( L"ElswordKR" );
#endif // GAMEGUARD_INI_COUNTRY

	//nProtect 생성실패.?
	if( pNpgl == NULL )
		return false;

	DWORD dwResult = pNpgl->Init();

	//게임가드 초기화시 에러가 있으면 종료처리를 해준다.
	if( dwResult != NPGAMEMON_SUCCESS )
	{
		WCHAR wstrErrMSG[MAX_PATH] = L"";

		// 주요에러코드’를 참조하여 상황에 맞는 메시지를 출력해줍니다.
		switch( dwResult )
		{
		case NPGAMEMON_ERROR_EXIST:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_320 ) );
			break;
		case NPGAMEMON_ERROR_GAME_EXIST:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_321 ) );
			break;
		case NPGAMEMON_ERROR_INIT:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_322 ) );
			break;
		case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
		case NPGAMEMON_ERROR_NFOUND_GG:
		case NPGAMEMON_ERROR_AUTH_INI:
		case NPGAMEMON_ERROR_NFOUND_INI:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_323 ) );
			break;
		case NPGAMEMON_ERROR_CRYPTOAPI:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_324 ) );
			break;
		case NPGAMEMON_ERROR_EXECUTE:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_325 ) );
			break;
		case NPGAMEMON_ERROR_ILLEGAL_PRG:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_326 ) );
			break;
		case NPGMUP_ERROR_ABORT:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_327 ) );
			break;
		case NPGMUP_ERROR_CONNECT:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_328 ) );
			break;
		case NPGAMEMON_ERROR_GAMEGUARD:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_329 ) );
			break;
		case NPGMUP_ERROR_PARAM:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_330 ) );
			break;
		case NPGMUP_ERROR_INIT:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_331 ) );
			break;
		case NPGMUP_ERROR_DOWNCFG:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_332 ) );
			break;
		case NPGMUP_ERROR_AUTH:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_333 ) );
			break;
		case NPGAMEMON_ERROR_NPSCAN:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_334 ) );
			break;
		case NPGG_ERROR_COLLISION:
			wcscpy( wstrErrMSG, GET_STRING( STR_ID_335 ) );
			break;
		default:
			{
				// 적절한 종료 메시지 출력
				wcscpy( wstrErrMSG, GET_STRING( STR_ID_336 ) );
				DialogBox( hInst, (LPCTSTR)IDD_SEND_MAIL_DIALOG, NULL, (DLGPROC)SendGameGuardMailDlgProc );			
			} 
			break;
		}

		wcscat( wstrErrMSG, GET_REPLACED_STRING( ( STR_ID_337, "i", ( int )dwResult ) ) );

		// 게임 종료코드
		MessageBox( NULL, wstrErrMSG, GET_STRING( STR_ID_338 ), MB_OK | MB_ICONERROR );

		// 에러 코드를 넘겨주면 에러 발생시 자동으로 게임가드 FAQ 페이지를 띄워줌
		//pNpgl->RunFAQ(dwResult);

		return false;
	}

	return true;
}

void KNP_Close()
{
#ifdef NO_GAMEGUARD
	return;
#endif

	if ( CheckNPHackUser() == false )
	{
		return;
	}

	if( pNpgl != NULL )
	{
		SAFE_DELETE( pNpgl );
	}
}

void KNP_SetHwnd(HWND hWnd )
{

#ifdef NO_GAMEGUARD
	return;
#endif

	if ( CheckNPHackUser() == false )
		return;
	

	if( pNpgl == NULL )
	{
		ASSERT( pNpgl != NULL );
		return;
	}

	pNpgl->SetHwnd( hWnd );
}

DWORD	dwCheckNPTime	= 0;
DWORD	dwTime			= 0;
bool	bIsKNPSuccess	= true;

bool KNP_GetFindGameMon()
{
#ifdef NO_GAMEGUARD
	return true;
#endif

	if ( CheckNPHackUser() == false )
	{
		return true;
	}


	if( g_pKTDXApp == NULL )
	{
		ASSERT( g_pKTDXApp != NULL );
		return true;
	}

	if( pNpgl == NULL )
	{
		//ASSERT( pNpgl != NULL );
		return true;
	}

	if( bIsKNPSuccess == false )
	{
		return bIsKNPSuccess;
	}

	dwCheckNPTime += GetTickCount() - dwTime;
	//일정 시간주기로 체크를 하게 한다.(5~10s 간격으로),
	if( dwCheckNPTime > CHECK_KNP_TIME )
	{
		if( pNpgl->Check() != NPGAMEMON_SUCCESS )
		{
			g_pKTDXApp->SetFindHacking( true, GET_STRING( STR_ID_339 ) );

			bIsKNPSuccess = false;

			return bIsKNPSuccess;
		}

		dwCheckNPTime = 0;
	}

	dwTime = GetTickCount();

	return bIsKNPSuccess;
}

DWORD KNP_Send( LPCWSTR wstrMSG )
{
#ifdef NO_GAMEGUARD
	return 0;
#endif

	if ( CheckNPHackUser() == false )
	{
		return 0;
	}

	if( pNpgl == NULL )
	{
		ASSERT( pNpgl != NULL );
		return 0;
	}

	return pNpgl->Send( wstrMSG );
}

void KNP_AUTH2( GG_AUTH_DATA* pAuthData )
{
#ifdef NO_GAMEGUARD
	return;
#endif

	if ( CheckNPHackUser() == false )
	{
		return;
	}

	if( pNpgl == NULL )
	{
		ASSERT( pNpgl != NULL );
		return;
	}

	pNpgl->Auth2( pAuthData );
}

#ifdef SERV_NPROTECT_CS_AUTH_30
void KNP_AUTH3( BYTE *pbyteCSAuth3, UINT32 uReceivedSize )
{
#ifdef NO_GAMEGUARD
	return;
#endif

	/*if ( CheckNPHackUser() == false )
	{
		return;
	}*/

	if( pNpgl == NULL )
	{
		ASSERT( pNpgl != NULL );

		return;
	}

	pNpgl->Auth3( pbyteCSAuth3, uReceivedSize, g_pInstanceData->GetConnectChannelServerID() );
}
#endif SERV_NPROTECT_CS_AUTH_30

BOOL CALLBACK NPGameMonCallback( DWORD dwMsg, DWORD dwArg )
{
#ifdef NO_GAMEGUARD
	return TRUE;
#endif

	if ( CheckNPHackUser() == false )
	{
		return TRUE;
	}


	if( g_pKTDXApp == NULL )
	{
		return TRUE;
	}

	if( pNpgl == NULL )
	{
		ASSERT( pNpgl != NULL );
		return FALSE;
	}

	if ( g_pData == NULL || g_pData->GetServerProtocol() == NULL )
	{
		return FALSE;
	}

	if (g_pData->GetServerProtocol()->IsConnected() == false)
	{
		return FALSE;
	}

	WCHAR wstrErrMSG[MAX_PATH] = L"";

	switch (dwMsg)
	{
#ifdef SERV_NPROTECT_CS_AUTH_30
	case NPGAMEMON_CHECK_CSAUTH3:
		{
			PCSAuth3Data pCSAuth3 = (PCSAuth3Data)dwArg;
			
			if( pCSAuth3 != NULL && pCSAuth3->dwPacketSize > 0)
			{
				if( pCSAuth3->dwServerNumber == g_pInstanceData->GetConnectChannelServerID() )
				{
					KEGS_NPROTECT_CSAUTH3_RESPONE_NOT KPacketNot;
					for( DWORD i = 0; i < pCSAuth3->dwPacketSize; i++)
					{
						KPacketNot.m_deqCSAuth3.push_back( pCSAuth3->bPacket[i] );
					}

					g_pData->GetServerProtocol()->SendPacket( EGS_NPROTECT_CSAUTH3_RESPONE_NOT, KPacketNot );
				}
			}
		}
		return TRUE;
#endif SERV_NPROTECT_CS_AUTH_30

	case NPGAMEMON_CHECK_CSAUTH2:
		{
			KEGS_KNPROTECT_AUTH_CHECK_NOT kPacket;
			memcpy( &kPacket, (PVOID)dwArg, sizeof(GG_AUTH_DATA) );
			g_pData->GetServerProtocol()->SendPacket( EGS_KNPROTECT_AUTH_CHECK_NOT, kPacket );
		}
		return TRUE;

	case NPGAMEMON_COMM_ERROR:
	case NPGAMEMON_COMM_CLOSE:
		{
			g_pData->GetServerProtocol()->SendID( EGS_NPROTECT_HACKING_USER_NOT );
			g_pKTDXApp->SetFindHacking( true, wstrErrMSG );
#ifdef GAMEGUARD_DETECTED_HACK_POPUP
			g_pKTDXApp->SetGameGuardFindHacking(true);
#endif //GAMEGUARD_DETECTED_HACK_POPUP
		}
		return FALSE;

	case NPGAMEMON_INIT_ERROR:
		{
			g_pData->GetServerProtocol()->SendID( EGS_NPROTECT_HACKING_USER_NOT );
			g_pKTDXApp->SetFindHacking( true, GET_REPLACED_STRING( ( STR_ID_340, "i", ( int )dwArg ) ) );
		}
		return FALSE;

	case NPGAMEMON_SPEEDHACK:
		{
			g_pData->GetServerProtocol()->SendID( EGS_NPROTECT_HACKING_USER_NOT );
			g_pKTDXApp->SetFindHacking( true, GET_STRING( STR_ID_341 ) );
#ifdef GAMEGUARD_DETECTED_HACK_POPUP
			g_pKTDXApp->SetGameGuardFindHacking(true);
#endif //GAMEGUARD_DETECTED_HACK_POPUP
		}
		return FALSE;

	case NPGAMEMON_GAMEHACK_KILLED:
		{

			g_pData->GetServerProtocol()->SendID( EGS_NPROTECT_HACKING_USER_NOT );
			g_pKTDXApp->SetFindHacking( true, GET_REPLACED_STRING( ( STR_ID_342, "S", pNpgl->GetInfo() ) ) );
#ifdef GAMEGUARD_DETECTED_HACK_POPUP
			g_pKTDXApp->SetGameGuardFindHacking(true);
#endif //GAMEGUARD_DETECTED_HACK_POPUP
			
		}
		return FALSE;

	case NPGAMEMON_GAMEHACK_DETECT:
		{

			g_pData->GetServerProtocol()->SendID( EGS_NPROTECT_HACKING_USER_NOT );
			g_pKTDXApp->SetFindHacking( true, GET_REPLACED_STRING( ( STR_ID_342, "S", pNpgl->GetInfo() ) ) );
#ifdef GAMEGUARD_DETECTED_HACK_POPUP
			g_pKTDXApp->SetGameGuardFindHacking(true);
#endif //GAMEGUARD_DETECTED_HACK_POPUP
			
		}
		return FALSE;

	case NPGAMEMON_GAMEHACK_DOUBT:
		{
			g_pData->GetServerProtocol()->SendID( EGS_NPROTECT_HACKING_USER_NOT );
			g_pKTDXApp->SetFindHacking( true, GET_STRING( STR_ID_343 ) );

#ifdef GAMEGUARD_DETECTED_HACK_POPUP
			g_pKTDXApp->SetGameGuardFindHacking(true);
#endif //GAMEGUARD_DETECTED_HACK_POPUP

		}
		return FALSE;

	case NPGAMEMON_GAMEHACK_REPORT:
		{
			DWORD	dwHackInfoSize = 0;
			LPBYTE	pHackInfo = NULL;
			//pHackInfo = GetHackInfoFromGameMon(&dwHackInfoSize);
			pHackInfo = pNpgl->GetHackInfo(&dwHackInfoSize); // C++ 일 경우.
			if( pHackInfo && dwHackInfoSize > 0 )
			{
				// 아래 함수는 게임가드에서 제공하는 함수가 아닙니다. 
				//SendToHackLog(pHackInfo, dwHackInfoSize); // 서버로 데이터 전송.
#ifdef SERV_NPROTECT_CS_AUTH_30
				KEGS_NPROTECT_CSAUTH3_GAMEHACK_REPORT_NOT KPacketNot;
				for( DWORD i = 0; i < dwHackInfoSize; i++)
				{
					KPacketNot.m_deqCSAuth3.push_back( pHackInfo[i] );
				}

				g_pData->GetServerProtocol()->SendPacket( EGS_NPROTECT_CSAUTH3_GAMEHACK_REPORT_NOT, KPacketNot );
#endif SERV_NPROTECT_CS_AUTH_30

			}
		}
		return TRUE;

	}

	return TRUE; // 계속 진행
}