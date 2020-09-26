#include "StdAfx.h"


#include ".\x2statelogin.h"

#ifdef LOGIN_KEYBOARD_SECURITY
#include <boost/assign.hpp>
#include <boost/format.hpp>
#endif LOGIN_KEYBOARD_SECURITY

#ifdef RULE_AGREEMENT
#include <boost/tokenizer.hpp>
#endif RULE_AGREEMENT

//{{ 2011.3.21	조효진	핵쉴드 모니터링 서버 연동
#ifdef HACKSHIELD_MORNITORING
#include "HShield.h"
#endif HACKSHIELD_MORNITORING
//}}

CX2StateLogin::CX2StateLogin(void) :
m_pDLGLoginBack( NULL ),
m_pDLGLoginFront( NULL )
#ifdef RULE_AGREEMENT
, m_pDLGRuleAggrement(NULL)
, m_pDLGBackGround(NULL)
#ifdef NEW_RULE_AGREEMENT
, m_pDLGNewRuleAggrement(NULL)
, m_pDLGNewBackGround(NULL)
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
#ifdef SERV_PUBLISHER_MATRIXKEY
, m_pDLGMatrix( NULL )
#endif SERV_PUBLISHER_MATRIXKEY
#ifdef LOGIN_KEYBOARD_SECURITY
, m_pDLGKeyboard( NULL)
, m_pLastFocusedControl( NULL )
, m_bKeyboardShift( false )
, m_bKeyBoardUse( false )
#endif LOGIN_KEYBOARD_SECURITY
#ifdef UIRRL_FIRST_SCREEN
,m_pTexutureBG( NULL )
#endif //UIRRL_FIRST_SCREEN
{
	
#ifdef NEW_LOGIN_PROCESS
#ifdef ADD_SERVER_GROUP
#ifdef EXTEND_SERVER_GROUP_MASK
	#ifdef CLIENT_COUNTRY_US
		// 미국은 여기서 파일을 읽어야 합니다. 이전 접속 서버를 알아야 하기 때문에...
	if ( g_pInstanceData->GetServerGroupID() == -1 )
		OpenScriptServerGroupFile();
	g_pMain->SetPickedChannelServerIPIndex( g_pInstanceData->GetServerGroupID() );
	#endif //CLIENT_COUNTRY_US
#else EXTEND_SERVER_GROUP_MASK
	if ( g_pInstanceData->GetServerGroupID() == SGI_INVALID )
		OpenScriptServerGroupFile();
	g_pMain->SetPickedChannelServerIPIndex( g_pInstanceData->GetServerGroupID() );
#endif EXTEND_SERVER_GROUP_MASK
#endif //ADD_SERVER_GROUP
#endif //NEW_LOGIN_PROCESS

#ifndef _SERVICE_
	if( g_pMain->GetManualLogin() == false )
	{
#ifdef NEW_LOGIN_PROCESS
		TryVerifyAccount();
#else NEW_LOGIN_PROCESS	
		switch( g_pMain->GetPublisher() )
		{
#ifdef _NEXON_KR_
		case CX2Main::XP_NEXON_KOREA:
			{
				StateChangeLoading();
			}
			break;
#endif _NEXON_KR_
#ifdef _NHN_JP_
		case CX2Main::XP_NHN_JP:
			{
				StateChangeLoading();
			} break;
#endif
#ifdef SERV_COUNTRY_PH
		case CX2Main::XP_GARENA_PH:
			{
				StateChangeLoading();
			} break;
#endif //SERV_COUNTRY_PH
		}
#endif NEW_LOGIN_PROCESS	
	}
	else
	{
		InitManualLogin();
	}
#else

	#ifdef _SERVICE_MANUAL_LOGIN_
	{
		InitManualLogin();
	}
	#else
	{
		//Service 모드일 때 들어오는 라인
		if( g_pMain->GetManualLogin() )
		{
			InitManualLogin();
		}
		else
		{
			RetrieveUserIDnPassword();
			//LoginNexonKorea();
#ifdef NEW_LOGIN_PROCESS
			TryVerifyAccount();
#else NEW_LOGIN_PROCESS
			StateChangeLoading();
#endif NEW_LOGIN_PROCESS
		}
	}
	#endif _SERVICE_MANUAL_LOGIN_

#endif _SERVICE_



	/*
	g_pKTDXApp->GetDGManager()->SetProjection( 
		g_pKTDXApp->GetDGManager()->GetNear(),
		g_pKTDXApp->GetDGManager()->GetFar(), false );

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( 1.0f );
	*/

	//CKTDGXMeshPlayer::CXMeshInstance* m_pMeshInstRandomBox = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox", 0.0f, -100.0f, 0.0f , 0,0,0, 0,0,0 );
	//m_pMeshInstRandomBox->SetOverUI( true );
	//CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"BoxJewel", 0,0,-100 );
	//pTemp->SetOverUI( true );




#ifdef FIRST_SCREEN_TEST

	if( false == g_pMain->GetManualLogin() )
	{
		m_pTexutureBG = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Unit_BackGround.tga" );
	}

#endif FIRST_SCREEN_TEST


#ifndef _SERVICE_

	if ( CheckIsVistaComputer() == true )
	{
		//MessageBox( g_pKTDXApp->GetHWND(), L"비스타 맞음?", L"확인", MB_OK );
	}

#endif






#ifdef IN_HOUSE_ONLY_NOTICE_MESSAGE_POPUP_TEST
	// 사내버전 클라이언트에서만 뜨는 팝업창입니다
	CKTDGUIDialogType pDLGInHouseNoticeDialog = new CKTDGUIDialog( this, L"DLG_Notice_Popup_InHouseOnly.lua" );
	if( NULL != pDLGInHouseNoticeDialog )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDLGInHouseNoticeDialog );	
	}
#endif IN_HOUSE_ONLY_NOTICE_MESSAGE_POPUP_TEST



	

	g_pKTDXApp->SkipFrame();

//{{ kimhc // 2010.5.11 // 사내에서 자동 로그인 할 수 있는 기능
#ifdef	AUTO_LOGIN_IN_HOUSE
	ReadIDAndPassword();
#endif	AUTO_LOGIN_IN_HOUSE
//}} kimhc // 2010.5.11 // 사내에서 자동 로그인 할 수 있는 기능

#ifdef LOGIN_KEYBOARD_SECURITY
	using namespace boost::assign;
	m_vecKeyList +=
		L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"-", L"=",
		L"Q", L"W", L"E", L"R", L"T", L"Y", L"U", L"I", L"O", L"P", L"[", L"]",	L"\\",
		L"A", L"S", L"D", L"F", L"G", L"H", L"J", L"K", L"L", L";", L"'",
		L"Z", L"X", L"C", L"V", L"B", L"N", L"M", L",", L".", L"/";

	std::vector<std::wstring> vecNormalKey;
	vecNormalKey +=
		L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"-", L"=",
		L"q", L"w", L"e", L"r", L"t", L"y", L"u", L"i", L"o", L"p", L"[", L"]",	L"\\",
		L"a", L"s", L"d", L"f", L"g", L"h", L"j", L"k", L"l", L";", L"\"",
		L"z", L"x", L"c", L"v", L"b", L"n", L"m", L",", L".", L"/";

	std::vector<std::wstring> vecShiftedKey;
	vecShiftedKey +=
		L"!", L"@", L"#", L"$", L"%", L"^", L"&", L"*", L"(", L")", L"_", L"+",
		L"Q", L"W", L"E", L"R", L"T", L"Y", L"U", L"I", L"O", L"P", L"{", L"}",	L"|",
		L"A", L"S", L"D", L"F", L"G", L"H", L"J", L"K", L"L", L":", L"\"",
		L"Z", L"X", L"C", L"V", L"B", L"N", L"M", L"<", L">", L"?";

	for(size_t i=0; i<m_vecKeyList.size(); ++i)
	{
		std::wstring strKey = L"KEY_" + m_vecKeyList[i];
		m_mapNormalKeyValue.insert( std::make_pair(strKey, vecNormalKey[i]) );
		m_mapShiftedKeyValue.insert( std::make_pair(strKey, vecShiftedKey[i]) );
	}
#endif LOGIN_KEYBOARD_SECURITY

#ifdef SERV_PUBLISHER_MATRIXKEY
	m_bDeleteMatrixDialog = false;
	m_fRotation = 0.0f;
	m_bRotate = false;
	m_nInput = 0;
#endif SERV_PUBLISHER_MATRIXKEY
}

CX2StateLogin::~CX2StateLogin(void)
{
#ifdef UIRRL_FIRST_SCREEN 
	SAFE_CLOSE( m_pTexutureBG );
#endif //UIRRL_FIRST_SCREEN
#ifdef SERV_PUBLISHER_MATRIXKEY
	SAFE_DELETE_DIALOG( m_pDLGMatrix );
#endif SERV_PUBLISHER_MATRIXKEY
	SAFE_DELETE_DIALOG( m_pDLGLoginBack );
	SAFE_DELETE_DIALOG( m_pDLGLoginFront );
#ifdef RULE_AGREEMENT
	SAFE_DELETE_DIALOG( m_pDLGRuleAggrement );
	SAFE_DELETE_DIALOG( m_pDLGBackGround );
#ifdef NEW_RULE_AGREEMENT
	SAFE_DELETE_DIALOG( m_pDLGNewRuleAggrement );
	SAFE_DELETE_DIALOG( m_pDLGNewBackGround );
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
#ifdef LOGIN_KEYBOARD_SECURITY
	SAFE_DELETE_DIALOG( m_pDLGKeyboard );
#endif LOGIN_KEYBOARD_SECURITY
}

// 09.10.14 태완 : 중복되는 곳이 많아서 함수로 묶어 정리
void CX2StateLogin::InitManualLogin()
{
	SAFE_DELETE_DIALOG(m_pDLGLoginBack);
	SAFE_DELETE_DIALOG(m_pDLGLoginFront);
	
#if defined CLIENT_COUNTRY_TW
	m_pDLGLoginBack = new CKTDGUIDialog( this, L"DLG_Login_Back_TW.lua" );
#elif defined CLIENT_COUNTRY_HK
	m_pDLGLoginBack = new CKTDGUIDialog( this, L"DLG_Login_Back_HK.lua" );
#else
	m_pDLGLoginBack = new CKTDGUIDialog( this, L"DLG_Login_Back.lua" );
#endif

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGLoginBack );

#ifndef DISABLE_LOGIN_FRONT
	m_pDLGLoginFront = new CKTDGUIDialog( this, L"DLG_Login_Front.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGLoginFront );
#endif DISABLE_LOGIN_FRONT

#ifdef SAVE_LOGIN_ID
	LoadID();
#endif SAVE_LOGIN_ID

#ifdef RULE_AGREEMENT
#ifdef NEW_RULE_AGREEMENT
	if( false == g_pMain->GetGameOption().GetNewRuleAgree() )
	{
#endif NEW_RULE_AGREEMENT
		if( false == g_pMain->GetGameOption().GetRuleAgree() )
		{
			OpenRuleAgreementDlg();
		}
#ifdef NEW_RULE_AGREEMENT
		OpenNewRuleAgreementDlg();
	}
#endif NEW_RULE_AGREEMENT
	else
	{
#ifdef SAVE_LOGIN_ID
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGLoginBack->GetControl( L"CheckBoxRememberID" );
		if( pCheckBox != NULL && pCheckBox->GetChecked() )
		{					
			m_pDLGLoginBack->GetControl(L"EditBoxLoginPassword")->RequestFocus();					
		}
		else
#endif SAVE_LOGIN_ID
		{
			m_pDLGLoginBack->GetControl(L"IMELoginID")->RequestFocus();
		}			
	}
#else RULE_AGREEMENT
#ifdef SAVE_LOGIN_ID
	LoadID();

	CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGLoginBack->GetControl( L"CheckBoxRememberID" );
	if( pCheckBox != NULL && pCheckBox->GetChecked() )
	{					
        if ( m_pDLGLoginBack != NULL && m_pDLGLoginBack->GetControl(L"EditBoxLoginPassword") != NULL )
        {
		    m_pDLGLoginBack->GetControl(L"EditBoxLoginPassword")->RequestFocus();					
        }
	}				
	else
#endif SAVE_LOGIN_ID
	{
        if ( m_pDLGLoginBack != NULL && m_pDLGLoginBack->GetControl(L"IMELoginID") != NULL )
        {
		    m_pDLGLoginBack->GetControl(L"IMELoginID")->RequestFocus();
        }
	}
#endif RULE_AGREEMENT

#ifdef SERV_PUBLISHER_OTP
	Handler_SLUCM_OTP_ON( NULL, 0, NULL, NULL );
#endif SERV_PUBLISHER_OTP
}

#ifdef SERV_LOGIN_MAC_ADDRESS
std::wstring CX2StateLogin::GetMACAddress() const
{
	std::wstring wstrMACAddress;

	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBuflen = sizeof(AdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBuflen);
	if(dwStatus == ERROR_SUCCESS)
	{
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
		PIP_ADAPTER_INFO pPrimaryAdapterInfo = pAdapterInfo;

		/*
		DWORD dwMinIndex = pAdapterInfo->Index;
		do 
		{
			if(pAdapterInfo->Index < dwMinIndex)
			{
				dwMinIndex = pAdapterInfo->Index;
				pPrimaryAdapterInfo = pAdapterInfo;
			}

			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);
		*/

		for (unsigned int i = 0; i < pPrimaryAdapterInfo->AddressLength; ++i)
		{
			// HLServer에서는 - 를 빼고 보내야하므로 주석 처리
			//if(i > 0)
			//	wstrMACAddress += L"-";
			
			wstrMACAddress += boost::str( boost::wformat(L"%02X") % pPrimaryAdapterInfo->Address[i]);
		}	
	}

	return wstrMACAddress;
}
#endif SERV_LOGIN_MAC_ADDRESS

HRESULT CX2StateLogin::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2State::OnFrameMove(fTime, fElapsedTime);

#ifdef SERV_PUBLISHER_MATRIXKEY
	FrameMoveMatrixPopup(fElapsedTime);
#endif SERV_PUBLISHER_MATRIXKEY

#ifdef CLOSE_ON_START_FOR_GAMEGUARD
#if !defined( CLIENT_COUNTRY_JP )
	if(g_pMain->IsCloseOnStart())
	{
		Handler_EGS_CLIENT_QUIT_REQ();
		g_pMain->SetCloseOnStart(false);
	}
#endif 
#endif CLOSE_ON_START_FOR_GAMEGUARD

#ifdef _US_LOGIN_ONLY_TAB_
	if(m_pDLGLoginBack->GetControl(L"IMELoginID")->GetHaveFocusIn() && 
		g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_TAB))
	{			
		m_pDLGLoginBack->RequestFocus(m_pDLGLoginBack->GetControl(L"EditBoxLoginPassword"));
	}
	else if(m_pDLGLoginBack->GetControl(L"EditBoxLoginPassword")->GetHaveFocusIn() && 
		g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_TAB))
	{
		m_pDLGLoginBack->RequestFocus(m_pDLGLoginBack->GetControl(L"IMELoginID"));		
	}
#endif _US_LOGIN_ONLY_TAB_

	return S_OK;
}

HRESULT CX2StateLogin::OnFrameRender()
{
	CX2State::OnFrameRender();

#ifdef FIRST_SCREEN_TEST

	if( false == g_pMain->GetManualLogin() )
	{
		if( NULL != m_pTexutureBG )
			m_pTexutureBG->Draw( 0, 0, 1024, 768 );
	}

#endif // FIRST_SCREEN_TEST


	return S_OK;
}

bool CX2StateLogin::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef LOGIN_KEYBOARD_SECURITY
	CKTDGUIControl* pControl;
	if(m_pDLGKeyboard != NULL)
		pControl = m_pDLGKeyboard->GetControlFocus();
#endif LOGIN_KEYBOARD_SECURITY

	if ( CX2State::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch(wParam)
	{
		case SLUCM_LOGIN:
			{
				RetrieveUserIDnPassword();
#ifdef NEW_LOGIN_PROCESS
				TryVerifyAccount();
#else NEW_LOGIN_PROCESS
				StateChangeLoading();
#endif NEW_LOGIN_PROCESS
			}
			break;

#ifdef _HOMEPAGE_MOVE_
		case SLUCM_HOMEPAGE_MOVE :
			{
				wstring wstrURL = L"";
#if defined( CLIENT_COUNTRY_US )
				wstrURL = L"http://www.elswordonline.com";
#elif defined( CLIENT_COUNTRY_ID )
				wstrURL = L"http://elsword.netmarble.co.id";
#elif defined( CLIENT_COUNTRY_BR )
				wstrURL = L"http://levelup.com.br/elsword";
#endif
				ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
			}
			break;
		case SLUCM_REGISTER_MOVE :
			{
				wstring wstrURL = L"";
#if defined( CLIENT_COUNTRY_US )
				wstrURL = L"https://myaccount.elswordonline.com/registration";
#elif defined( CLIENT_COUNTRY_ID )
				wstrURL = L"https://member.netmarble.co.id/join/index.asp";
#elif defined( CLIENT_COUNTRY_BR )
				wstrURL = L"https://minhaconta.levelupgames.com.br";
#endif
				ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
			}
			break;
#if defined( CLIENT_COUNTRY_US )
		case SLUCM_FACEBOOK_MOVE :
			{
				wstring wstrURL = L"http://www.facebook.com/ElswordOnlineNA";
				ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
			}
			break;
		case SLUCM_STEAM_COMMUNITY_MOVE :
			{
				wstring wstrURL = L"http://steamcommunity.com/sharedfiles/filedetails/?id=93245387";
				ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
			}
			break;
#endif
#endif _HOMEPAGE_MOVE_
		case SLUCM_CHANGE_STATE_MEMBER_JOIN:
			{
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_JOIN, NULL, false );
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
#ifdef MEMBER_JOIN_FROM_WEB
				std::wstring wstrURL = GetStrMemberJoinWeb();
#ifdef USE_DEFAULT_BROWSER
				ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
#else USE_DEFAULT_BROWSER
				ShellExecuteW(NULL, _T("open"), _T("iexplore.exe"), wstrURL.c_str(), NULL, SW_SHOW); 
#endif USE_DEFAULT_BROWSER
#else MEMBER_JOIN_FROM_WEB
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_JOIN, NULL, false );
#endif MEMBER_JOIN_FROM_WEB
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
				return true;
			}
			break;

		//case SLUCM_AUTO_LOGIN:
		//	{
		//		//g_pInstanceData->SetPlannerServerLogin( true );
		//	} break;
		//
		//case SLUCM_NOT_AUTO_LOGIN:
		//	{
		//		//g_pInstanceData->SetPlannerServerLogin( false );
		//	} break;

		case SLUCM_NEXON_KOREA_LOGIN:
			{
				
				if ( g_pMain->GetManualLogin() == true )
				{
					g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, SLUCM_LOGIN, NULL, false );
				}
			}
			break;

		case SLUCM_NEXON_KOREA_LOGIN_COMPLETE:
			{
				/*
				if( lParam == 1 )
				{
					g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, SLUCM_LOGIN, NULL, false );
					g_pMain->SetNexonLogin( true );
				}
				else
				{
					g_pMain->SetNexonLogin( false );
					if( g_pMain->GetManualLogin() == true )
					{
						g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, SLUCM_LOGIN, NULL, false );
					}
					else
					{
						g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
					}					
				}
				*/
			}
			break;
#ifdef RULE_AGREEMENT
		case SLUCM_RULE_AGREEMENT:
			{
				SAFE_DELETE_DIALOG(m_pDLGBackGround);
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRuleAggrement, NULL, false );
				m_pDLGRuleAggrement = NULL;
				g_pMain->GetGameOption().SetRuleAgree(true);		// 동의 하고나면 이후에는 뜨지않음.
#ifdef SAVE_LOGIN_ID
				CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGLoginBack->GetControl( L"CheckBoxRememberID" );
				if( pCheckBox != NULL && pCheckBox->GetChecked() )
				{					
					m_pDLGLoginBack->GetControl(L"EditBoxLoginPassword")->RequestFocus();					
				}				
				else
#endif SAVE_LOGIN_ID
				{
					m_pDLGLoginBack->GetControl(L"IMELoginID")->RequestFocus();
				}				
			} break;
#ifdef NEW_RULE_AGREEMENT
		case SLUCM_NEW_RULE_AGREEMENT:
			{
				SAFE_DELETE_DIALOG(m_pDLGNewBackGround);
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGNewRuleAggrement, NULL, false );
				m_pDLGNewRuleAggrement = NULL;
				g_pMain->GetGameOption().SetNewRuleAgree(true);	// 동의 하고나면 이후에는 뜨지않음.
			} break;
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
#ifdef SERV_PUBLISHER_MATRIXKEY
		case SLUCM_MATRIX_NUMBER_MOUSE_OVER:
			{
				m_bRotate = false;
			}
			break;
		case SLUCM_MATRIX_NUMBER_MOUSE_OUT:
			{
				m_bRotate = true;
			}
			break;
		case SLUCM_MATRIX_NUMBER_MOUSE_UP:
			return Handler_SLUCM_MATRIX_NUMBER(hWnd, uMsg, wParam, lParam);
		case SLUCM_MATRIX_OK:
			return Handler_SLUCM_MATRIX_OK(hWnd, uMsg, wParam, lParam);
		case SLUCM_MATRIX_CLEAR:
			return Handler_SLUCM_MATRIX_CLEAR(hWnd, uMsg, wParam, lParam);
#endif SERV_PUBLISHER_MATRIXKEY
#ifdef SERV_PUBLISHER_OTP
		case SLUCM_OTP_ON:
			return Handler_SLUCM_OTP_ON(hWnd, uMsg, wParam, lParam);
		case SLUCM_OTP_OFF:
			return Handler_SLUCM_OTP_OFF(hWnd, uMsg, wParam, lParam);
#endif SERV_PUBLISHER_OTP
#ifdef SAVE_LOGIN_ID
		case SLUCM_REMEMBER_ID_CHECK_ON:
			{

			} break;
		case SLUCM_REMEMBER_ID_CHECK_OFF:
			{

			} break;
#endif SAVE_LOGIN_ID
#ifdef LOGIN_KEYBOARD_SECURITY
		case SLUCM_KEYBOARD:
			return Handler_SLUCM_KEYBOARD(hWnd, uMsg, wParam, lParam);
		case SLUCM_KEYBOARD_KEY:
			return Handler_SLUCM_KEYBOARD_KEY(hWnd, uMsg, wParam, lParam);
		case SLUCM_KEYBOARD_ENTER:
			return Handler_SLUCM_KEYBOARD_ENTER(hWnd, uMsg, wParam, lParam);
		case SLUCM_KEYBOARD_SHIFT:
			return Handler_SLUCM_KEYBOARD_SHIFT(hWnd, uMsg, wParam, lParam);
		case SLUCM_KEYBOARD_BACK:
			return Handler_SLUCM_KEYBOARD_BACK(hWnd, uMsg, wParam, lParam);
		case SLUCM_KEYBOARD_EDIT_FOCUSOUT:
			return Handler_SLUCM_KEYBOARD_EDIT_FOCUSOUT(hWnd, uMsg, wParam, lParam);
		case SLUCM_KEYBOARD_CLOSE:
			return Handler_SLUCM_KEYBOARD_CLOSE(hWnd, uMsg, wParam, lParam);
#endif LOGIN_KEYBOARD_SECURITY
#ifdef	USE_ACCOUNT_DOMAIN
		case SLUCM_DOMAIN_TYPE_PLAYID:
			{
				Handler_SLUCM_DOMAIN_TYPE_PLAYID(hWnd, uMsg, wParam, lParam);
				break;
			}
		case SLUCM_DOMAIN_TYPE_TCG:
			{
				Handler_SLUCM_DOMAIN_TYPE_TCG(hWnd, uMsg, wParam, lParam);
				break;
			}
#endif	USE_ACCOUNT_DOMAIN
	}

	return false;
}

bool CX2StateLogin::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	// 2010. 9. 19 조효진   NEW_LOGIN_PROCESS
	switch( wParam )
	{
	case ECH_VERIFY_ACCOUNT_ACK:
#ifdef NEW_LOGIN_PROCESS
		return Handler_ECH_VERIFY_ACCOUNT_ACK( hWnd, uMsg, wParam, lParam );
#endif NEW_LOGIN_PROCESS
		break;	
#ifdef SERV_GLOBAL_AUTH
	case ECH_SECURITY_INFO_NOT:
		return Handler_ECH_SECURITY_INFO_NOT( hWnd, uMsg, wParam, lParam );
#endif SERV_GLOBAL_AUTH
	default:
		break;
	}

	return false;
}


void CX2StateLogin::StateChangeLoading()
{
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

#ifdef SAVE_LOGIN_ID
	if( m_pDLGLoginBack != NULL )
	{
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGLoginBack->GetControl( L"CheckBoxRememberID" );
		if ( pCheckBox != NULL )
		{
			if( pCheckBox->GetChecked() )
			{
				SaveID();
			}
			else
			{
				DeleteSavedID();
			}
		}
	}	
#endif SAVE_LOGIN_ID

#ifndef HIDE_SYSTEM_MESSAGE
	g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_656 ) );
#endif // HIDE_SYSTEM_MESSAGE

	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_LOADING, NULL, false );
}


// 로그인 방식에 따라 UI 혹은 nexon passport에서 user id와 password를 얻어오고, 서버 선택창에서 GS에 접속할 때 사용된다.
void CX2StateLogin::RetrieveUserIDnPassword()
{
	if( g_pMain->GetManualLogin() == true )
	{
		CKTDGUIIMEEditBox*	pIDEditBox			= NULL;
		CKTDGUIEditBox*		pPasswordEditBox	= NULL;

#ifdef LOGIN_KEYBOARD_SECURITY
		if(m_pDLGKeyboard != NULL && m_pDLGKeyboard->GetShow() == true)
		{
			pIDEditBox			= (CKTDGUIIMEEditBox*)m_pDLGKeyboard->GetControl( L"IMELoginID" );
			pPasswordEditBox	= (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"EditBoxLoginPassword" );
		}
		else
#endif LOGIN_KEYBOARD_SECURITY
		{
			pIDEditBox			= (CKTDGUIIMEEditBox*)m_pDLGLoginBack->GetControl( L"IMELoginID" );
			pPasswordEditBox	= (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"EditBoxLoginPassword" );
		}

		g_pInstanceData->SetUserID( pIDEditBox->GetText() );

#ifdef PASSWORD_MD5
		std::string strPasswordMD5 = KncUtil::GetMD5( KncUtil::toNarrowString(pPasswordEditBox->GetText()) );
		g_pInstanceData->SetUserPassword( KncUtil::toWideString(strPasswordMD5) );
#else PASSWORD_MD5
		g_pInstanceData->SetUserPassword( pPasswordEditBox->GetText() );
#endif PASSWORD_MD5

#ifdef SERV_PUBLISHER_OTP
		CKTDGUIEditBox*	pOTPEditBox = NULL;
#ifdef LOGIN_KEYBOARD_SECURITY
		if(m_pDLGKeyboard != NULL && m_pDLGKeyboard->GetShow() == true)
		{
			pOTPEditBox = (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"EditBoxLoginOTP" );
			if(pOTPEditBox)
			{
				m_wstrOTP = pOTPEditBox->GetText();
			}
		}
		else
#endif LOGIN_KEYBOARD_SECURITY
		{
			pOTPEditBox = (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"EditBoxLoginOTP" );
			if(pOTPEditBox)
			{
				m_wstrOTP = pOTPEditBox->GetText();
			}
		}		
#endif SERV_PUBLISHER_OTP
	}
	else
	{
		switch( g_pMain->GetPublisher() )
		{
#ifdef _NEXON_KR_
		case CX2Main::XP_NEXON_KOREA:
			{
				WCHAR passPortStr[PASSPORT_SIZE] = { 0, };
				CNMCOClientObject::GetInstance().GetNexonPassport( passPortStr );
				const WCHAR* pLoginID = CNMCOClientObject::GetInstance().GetMyLoginId();


				if( 0 == wcslen( passPortStr ) && 
					0 != g_pNexonPassport.length() )
				{
					g_pInstanceData->SetUserPassword( g_pNexonPassport.c_str() );	// 체험 아이디인 경우 	
				}
				else
				{
					g_pInstanceData->SetUserPassword( passPortStr );		// 일반 넥슨 계정인 경우
				}

				g_pInstanceData->SetUserID( pLoginID );

				//{{ kimhc // 2010-06-24 // OTP 작업
			#ifdef	SERV_OTP_AUTH
				// 유저의 하드웨어 ID를 획득
				WCHAR lpNxHWID[NXHWID_SIZE] = { 0, };
				CNMCOClientObject::GetInstance().GetNxHWID( lpNxHWID );
				g_pInstanceData->SetNxHWID( lpNxHWID );
			#endif	SERV_OTP_AUTH
				//}} kimhc // 2010-06-24 // OTP 작업

			} break;
#endif _NEXON_KR_
#ifdef CLIENT_COUNTRY_TWHK
		case CX2Main::XP_GAMANIA_TW:
		case CX2Main::XP_GAMANIA_HK:
			{
				//ID/Pass는 이미 인스턴스 데이터에 세트되어 있다.
				//g_pInstanceData->SetUserID( g_pMain->GetArgvID() );
				//g_pInstanceData->SetUserPassword( g_pMain->GetArgvPass() );
			} break;
#endif CLIENT_COUNTRY_TWHK
		}
	}



#ifdef _IN_HOUSE_
	wstringstream wstrmWindowTitle;
	wstrmWindowTitle << L"X2_" << g_pMain->GetClientVersion() << L"; " << __DATE__ << L", " << __TIME__ << L"; " << g_pInstanceData->GetUserID().c_str() << L"; " << _getpid();
	SetWindowText( g_pKTDXApp->GetHWND(), wstrmWindowTitle.str().c_str() );
#endif _IN_HOUSE_

#ifdef CLIENT_USE_XTRAP	 // XTRAP - 클라 유저정보 획득(캐릭터 선택 전)
	char szServer[2];
	_itoa(g_pInstanceData->GetServerGroupID(), szServer, 10);
	XTrap_C_SetUserInfoEx( (LPCSTR)g_pInstanceData->GetUserID().c_str(), szServer, NULL, NULL, NULL, NULL);
#endif	// CLIENT_USE_XTRAP

}
//{{ kimhc // 2010.5.11 // 사내에서 자동 로그인 할 수 있는 기능
#ifdef AUTO_LOGIN_IN_HOUSE
void CX2StateLogin::ReadIDAndPassword()
{
	const wstring wstrFileName( L"LoginKey.lua" );

	if( true == g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->IsValidFile( wstrFileName ) )
	{
		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

        if ( g_pKTDXApp->LoadAndDoMemory( &luaManager, wstrFileName.c_str() ) == true )
		{
			if ( luaManager.BeginTable( "LOGIN" ) == true )
			{
				wstring wstrID, wstrPassword;

				LUA_GET_VALUE(	luaManager, "ID", wstrID,	L"" );
				LUA_GET_VALUE(	luaManager, "PASSWORD", wstrPassword,	L"" );

				if ( wstrID.empty() == false && wstrPassword.empty() == false )
				{
					g_pInstanceData->SetUserID( wstrID );
					g_pInstanceData->SetUserPassword( wstrPassword );

					// 일단 OTP 넣지 않음

					StateChangeLoading();					

					luaManager.EndTable();
					return;
				}
				luaManager.EndTable();
			}
		}
	}

	const wstring wstrFileNameEx( L"LoginKeyEx.lua" );
	if( true == g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->IsValidFile( wstrFileNameEx ) )
	{
		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

        if ( g_pKTDXApp->LoadAndDoMemory( &luaManager, wstrFileNameEx.c_str() ) == true )
		{
			if ( luaManager.BeginTable( "LOGIN" ) == true )
			{
				wstring wstrID, wstrPassword;

				LUA_GET_VALUE(	luaManager, "ID", wstrID,	L"" );
				LUA_GET_VALUE(	luaManager, "PASSWORD", wstrPassword,	L"" );

				if ( wstrID.empty() == true || wstrPassword.empty() == true )
					return;
				else
				{
					if( 1 != MessageBox( g_pKTDXApp->GetHWND(), L"자동 로그인 기능 사용하시겠습니까?", L"!", MB_OKCANCEL ) )
						return;

					g_pInstanceData->SetUserID( wstrID );
					g_pInstanceData->SetUserPassword( wstrPassword );

					// 일단 OTP 넣지 않음

					StateChangeLoading();
				}
				luaManager.EndTable();
			}
		}
	}
}
#endif AUTO_LOGIN_IN_HOUSE
//}} kimhc // 2010.5.11 // 사내에서 자동 로그인 할 수 있는 기능

#ifdef NEW_LOGIN_PROCESS
#ifdef ADD_SERVER_GROUP
bool CX2StateLogin::OpenScriptServerGroupFile()
{
#ifdef SERVER_GROUP_ID_IN_GAME_OPTION
	return true;
#endif // SERVER_GROUP_ID_IN_GAME_OPTION

#ifdef EXTEND_SERVER_GROUP_MASK
	g_pInstanceData->SetServerGroupID( 0 );
	bool bParsingOK = true;
#else  EXTEND_SERVER_GROUP_MASK
	string			strFileName;
	SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
	bool			bParsingOK		= false;

	ConvertWCHARToChar( strFileName, g_pData->GetSavedServerGroupFileName() );

	ConvertFileAnsiToUTF8( strFileName, strFileName );

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

	if ( g_pKTDXApp->LoadLuaTinker( g_pData->GetSavedServerGroupFileName().c_str() ) == false )
	{
		ErrorLogMsg( XEM_ERROR9, strFileName.c_str() );
		return false;
	}
	else
	{
		LUA_GET_VALUE_ENUM( luaManager, "SERVER_GROUP", 			eServerGroupID,			SERVER_GROUP_ID,		SGI_INVALID	);
	}

	switch ( eServerGroupID )
	{
	case SGI_SOLES:
	case SGI_GAIA:
		{
			g_pInstanceData->SetServerGroupID( eServerGroupID );
			bParsingOK = true;
		}
		break;
	default:
		{
#ifdef RANDOM_SERVER
			if( g_pMain->GetDefaultChannelServerIPIndex() == SGI_INVALID )
			{
				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( (rand() % 2) ) );
			}
			else
			{
				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
			}
#else
			g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
#endif		
		}
		break;
	}
#endif EXTEND_SERVER_GROUP_MASK

	return bParsingOK;
}
#endif	//ADD_SERVER_GROUP

bool CX2StateLogin::TryVerifyAccount()
{
	if( NULL == g_pData->GetServerProtocol() )
	{
		g_pData->ResetServerProtocol();
	}

#ifdef CLIENT_COUNTRY_ID
	std::string userID, userPW;
	ConvertWCHARToChar( userID, g_pInstanceData->GetUserID() );
	ConvertWCHARToChar( userPW, g_pInstanceData->GetUserPassword() );

	if( userID.size() > 20 || userID.size() < 4 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GetErrStrF( NetError::ERR_CJ_02 ), this );
		return false;
	}
	else if( userPW.size() > 15 || userPW.size() < 6 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GetErrStrF( NetError::ERR_CJ_03 ), this );
		return false;
	}
#endif // CLIENT_COUNTRY_ID

#ifdef SERVER_GROUP_UI_ADVANCED
	std::wstring wstrChannelServerIP = g_pMain->GetPickedChannelServer().m_kServerIP;
	int iChannelServerPort = g_pMain->GetPickedChannelServer().m_usMasterPort;
#else // SERVER_GROUP_UI_ADVANCED
	std::wstring wstrChannelServerIP = g_pMain->GetPickedChannelServerIP();
	int iChannelServerPort = g_pMain->GetChannelServerPort();
#endif // SERVER_GROUP_UI_ADVANCED

	if( g_pData->GetServerProtocol()->IsChConnected() ||
		g_pData->GetServerProtocol()->ConnectedToChannelServer( wstrChannelServerIP.c_str(), iChannelServerPort ) == true )
	{
		return Handler_ECH_VERIFY_ACCOUNT_REQ();
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_735 ), this );
	}

	return false;
}

#ifdef CLIENT_DIRECT_CONNECT_AUTH_SERVER
bool CX2StateLogin::VerifyPublisherAccoutByDirect(OUT std::string& strUserToken)
{
	// 클라이언트와 인증 서버가 Direct로 접속하여서
	// ECH_VERIFY_ACCOUNT_REQ 전에 인증 과정이 있는 경우 해당 함수 사용함
	// 현재 인도네시아를 제외하고는 이런 경우가 없어서 여기에 함수 구현함.
	// 만약 여러 국가가 해당 구조를 사용하게 된다면 X2StateLogin_국가코드.cpp 로 달리해야 할것임
	
	strUserToken = "";

#ifdef _SERVICE_
	// 피드백 받으면서 추가해 본 두줄. 두번째 줄에서 크래시
	NMRUNPARAMAPIFUNC::SetDLLFileName( "NMRunParamDLL.dll" );

	NMClientAuthDLL_ClearAuthenticationData();	//인증과 관련된 모든 데이타 변수 초기화

	std::string userID, userPW;
	ConvertWCHARToChar( userID, g_pInstanceData->GetUserID() );
	ConvertWCHARToChar( userPW, g_pInstanceData->GetUserPassword() );
	const std::string serviceCode = "elsword";

#ifdef _OPEN_TEST_
	const std::string redirectUrl = "http://alpha-sign.netmarble.co.id";
	const std::string domain = "alpha-sign.netmarble.co.id";
	const std::string xmlURL = "https://alpha-sign.netmarble.co.id/application/authenticate";
#else _OPEN_TEST_
	const std::string redirectUrl = "http://sign.netmarble.co.id";
	const std::string domain = "sign.netmarble.co.id";
	const std::string xmlURL = "https://sign.netmarble.co.id/application/authenticate";
#endif _OPEN_TEST_

	const UINT countryCode = 1;
	if( FALSE == NMClientAuthDLL_SetLoginData( userID.c_str(), userPW.c_str(), serviceCode.c_str(), redirectUrl.c_str(), FALSE, TRUE, domain.c_str(), xmlURL.c_str() ) )	////인증과 관련된 정보들을 전달하여 정보를 셋팅
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GetErrStrF( NetError::ERR_CJ_01 ), this );
		return false;
	}

	if( FALSE == NMClientAuthDLL_LoginProcessExecution() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GetErrStrF( NetError::ERR_CJ_04 ), this );
		return false; 
	}

	BOOL loginSuccess;
	int queryResultCode;
	LPCSTR errorCode = new char[2];
	LPCSTR errorMessage = new char[255];
	if( FALSE == NMClientAuthDLL_IsLoginComplete(loginSuccess, errorMessage, errorCode, queryResultCode) || FALSE == loginSuccess )
	{
		switch( atoi( errorCode ) )
		{
		case -4:
		case -5:
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GetErrStrF( NetError::ERR_CJ_01 ), this );
			return false;
		}
		// 로그인이 실패하였습니다.
		std::wstringstream wstrstm;
		wstrstm << errorMessage << L" (" << errorCode << L")";
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this );

		return false;
	}

	NMRUNPARAM_RETCODE retCode;
	CNMRunParam& kNMRunParam = g_pInstanceData->GetNetMarbleRunParam();
	retCode = kNMRunParam.Load( "yekssapretrats", false );	//환경변수에 세팅된 인증정보를 로드
	if ( ERROR_NMRUNPARAM_SUCCESS != retCode )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"Error - NMRunParam Load", this );
		return false;
	}

	// 결제 창용 SSO Web Browser
	{
		PresetSSOWebBrowserSetSiteName( "indonesia" );

		// 폴더 설정
		char szPath[MAX_PATH];
		GetCurrentDirectoryA( sizeof( szPath ), szPath );
		PresetSSOWebBrowserBaseFolder( szPath );

		// runparam 가져오기
		std::string publicToken = kNMRunParam.GetParam( "cookies", "PublicToken" );
		std::string secureToken = kNMRunParam.GetParam( "cookies", "SecureToken" );

		if( !InitSSOWebBrowser( g_pKTDXApp->GetHWND(), publicToken.c_str(), secureToken.c_str(), "" ) )
		{
			// 오류메세지
			char szError[256];
			SSOWebLib_GetLastError( szError, sizeof( szError ) );

			std::wstring wstrError;
			ConvertCharToWCHAR( wstrError, szError );

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrError.c_str(), this );
			return false;
		}
	}

	strUserToken = std::string( "userToken=" ) + kNMRunParam.GetParam( "servers", "userToken" ) + "|extraInfo=";
#endif _SERVICE_
	return true;
}
#endif // CLIENT_DIRECT_CONNECT_AUTH_SERVER

bool CX2StateLogin::Handler_ECH_VERIFY_ACCOUNT_REQ()
{
#ifdef CLIENT_DIRECT_CONNECT_AUTH_SERVER
	std::string strUserToken = "";
	if (!VerifyPublisherAccoutByDirect(strUserToken))
	{
		return false;
	}
#endif //CLIENT_DIRECT_CONNECT_AUTH_SERVER

	KECH_VERIFY_ACCOUNT_REQ kPacket;
	kPacket.m_wstrID = g_pInstanceData->GetUserID();
	kPacket.m_wstrPassword = g_pInstanceData->GetUserPassword();
#ifdef SERV_PUBLISHER_OTP
	kPacket.m_wstrPublisherOTP = m_wstrOTP;
#endif SERV_PUBLISHER_OTP
#ifdef SERV_KOG_OTP_VERIFY
	kPacket.m_bLogin = !(g_pInstanceData->IsIDVerified());
#else
	kPacket.m_bLogin = true;
#endif
#ifdef SERV_LOGIN_MAC_ADDRESS
	kPacket.m_wstrMACAddress = GetMACAddress();
#endif // SERV_LOGIN_MAC_ADDRESS

#ifdef CLIENT_DIRECT_CONNECT_AUTH_SERVER
	kPacket.m_strUserToken = strUserToken;
#endif // CLIENT_DIRECT_CONNECT_AUTH_SERVER
#ifdef	USE_ACCOUNT_DOMAIN
	kPacket.m_wstrDomain = g_pInstanceData->GetDomainName();
#endif	USE_ACCOUNT_DOMAIN
#ifdef SERV_STEAM
	kPacket.m_iChannelingCode = g_pInstanceData->GetChannelingCode();

	// 스팀 전용 pw를 직접 입력했는지 체크 하고, 그렇다면 클라 꺼버린다.
	if( CX2Steam::IsValidSteamUser( kPacket.m_wstrPassword ) == false )
		g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
#endif //SERV_STEAM

	g_pData->GetServerProtocol()->SendChPacket( ECH_VERIFY_ACCOUNT_REQ, kPacket );
	g_pMain->AddServerPacket( ECH_VERIFY_ACCOUNT_ACK );

	return true;
}

bool CX2StateLogin::Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KECH_VERIFY_ACCOUNT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( ECH_VERIFY_ACCOUNT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->SetServerCurrentTime( kEvent.m_wstrCurrentTime );
			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->MassFileMapping();
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET
#ifdef SERV_KOG_OTP_VERIFY
			// 메뉴얼 로그인(클라이언트 로그인) 시  최초 성공 부분 (퍼블리셔 인증 성공)
			if( false == kEvent.m_wstrPassport.empty() )
			{
				g_pInstanceData->SetIDVerified(true);
				g_pInstanceData->SetUserID( kEvent.m_wstrUserID );
				g_pInstanceData->SetUserPassword( kEvent.m_wstrPassport );	
#ifdef SERV_PURCHASE_TOKEN
				g_pInstanceData->SetPurchaseToken( kEvent.m_wstrPurchaseTok );
#endif SERV_PURCHASE_TOKEN

#ifdef SERV_COUNTRY_TH
				g_pInstanceData->SetSocketID( kEvent.m_wstrSocketID );
#endif //SERV_COUNTRY_TH

				//{{ 2011.3.21	조효진	핵쉴드 모니터링 서버 연동
#ifdef HACKSHIELD_MORNITORING
				_AhnHS_SetUserIdW( kEvent.m_wstrUserID.c_str() );

#ifdef HACKSHIELD_MORNITORING_ADD_USERCUSTOMINFO
				WCHAR szUserUid[256];
#ifdef MONSTERID_TITLE_ADD_LINEBREAK
				// 유저 커스텀 정보를 가장하여 국가코드 전송
				switch(g_pMain->GetNationFlag())
				{
				case CX2Main::NF_DE:
					StringCchPrintf( szUserUid, MAX_PATH, L"DE" );
					break;
				case CX2Main::NF_FR:
					StringCchPrintf( szUserUid, MAX_PATH, L"FR" );
					break;
				case CX2Main::NF_IT:
					StringCchPrintf( szUserUid, MAX_PATH, L"IT" );
					break;
				case CX2Main::NF_PL:
					StringCchPrintf( szUserUid, MAX_PATH, L"PL" );
					break;
				case CX2Main::NF_ES:
					StringCchPrintf( szUserUid, MAX_PATH, L"ES" );
					break;
				case CX2Main::NF_EN:
					StringCchPrintf( szUserUid, MAX_PATH, L"EN" );
					break;
				default:

					break;
				}
#else //MONSTERID_TITLE_ADD_LINEBREAK
				StringCchPrintf( szUserUid, MAX_PATH, kEvent.m_iUserUID );	//버전
#endif //MONSTERID_TITLE_ADD_LINEBREAK
				_AhnHS_SetUserCustomInfoW( szUserUid );
#endif HACKSHIELD_MORNITORING_ADD_USERCUSTOMINFO

#endif HACKSHIELD_MORNITORING
				//}}

#ifdef SERV_SEPARATE_SERVERSET_US_USER
				// US의 남미, 북미의 경우 m_iChannelingCode 값이 서버군이 됨
#ifdef EXTEND_SERVER_GROUP_MASK
				if ( kEvent.m_iChannelingCode != -1 )
#else EXTEND_SERVER_GROUP_MASK
				if ( kEvent.m_iChannelingCode != SGI_INVALID )
#endif //EXTEND_SERVER_GROUP_MASK
				{
#ifdef SERV_STEAM
					// 스팀 인증을 통과했는데 실재 클라도 스팀 클라인지 다시 확인
					/* 인증 리턴값으론 확인 불가하다
					if( kEvent.m_iChannelingCode > 1 )
					{
						if( CX2Steam::IsSteamUser() == false )
						{
							// 스팀 클라이언트로 다시 실행시켜 주십시오
							//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4519 ), g_pMain->GetNowState() );
							g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
						}
					}
					*/
					g_pInstanceData->SetChannelingCode(kEvent.m_iChannelingCode);
					g_pInstanceData->SetServerGroupID(kEvent.m_iChannelingCode % 2);
#else //SERV_STEAM
					g_pInstanceData->SetServerGroupID((SERVER_GROUP_ID)kEvent.m_iChannelingCode);
#endif //SERV_STEAM
					g_pMain->SetPickedChannelServerIPIndex( g_pInstanceData->GetServerGroupID() );
				}
				
#endif //SERV_SEPARATE_SERVERSET_US_USER

			}
#endif SERV_KOG_OTP_VERIFY		

			StateChangeLoading();
			
#if defined(PLAYER_ID_IN_GAME_OPTION) || defined(SERVER_GROUP_ID_IN_GAME_OPTION)
			g_pMain->GetGameOption().SaveScriptFile();
#endif // PLAYER_ID_IN_GAME_OPTION || SERVER_GROUP_ID_IN_GAME_OPTION
		}
		else
		{	
#ifdef SERV_STEAM
			if( CX2Steam::IsSteamUser() == true )
			{
// 				std::wstring packetExplain;
// 				packetExplain = NetError::GetErrStrF(kEvent.m_iOK);
// 				packetExplain += L"\n";
// 				packetExplain += GET_STRING( STR_ID_26353 );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg( m_pMsgOkFailToConnectServer );
				m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),GET_STRING( STR_ID_26353 ), this );
				m_bFailToConnectServer = true;
				return false;
			}
#endif //SERV_STEAM

#ifdef SERV_LOGIN_TOU
			if( kEvent.m_iOK == NetError::ERR_K3RCOMBO_AUTH_02 )
			{
				ShellExecuteA(NULL, "open", kEvent.m_strAgreementURL.c_str(), NULL, NULL, SW_SHOW);
			}
#endif //SERV_LOGIN_TOU

			if( g_pMain->GetManualLogin() == false )
			{
				if(m_pDLGLoginBack == NULL)
				{
					InitManualLogin();
				}				
			}

			CKTDGUIEditBox* pPassEditBox = (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl(L"EditBoxLoginPassword");
			if(pPassEditBox != NULL)
				pPassEditBox->ClearText();

			pPassEditBox->RequestFocus();
		}
	}

	return false;
}
#endif NEW_LOGIN_PROCESS

#ifdef RULE_AGREEMENT
void CX2StateLogin::OpenRuleAgreementDlg()
{
	SAFE_DELETE_DIALOG(m_pDLGRuleAggrement);
	SAFE_DELETE_DIALOG(m_pDLGBackGround);

	m_pDLGBackGround = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Unit_State_RandomItem_BG.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBackGround );
	m_pDLGBackGround->SetCloseCustomUIEventID(-1);
	CKTDGUIStatic* pStatic_BackGround = (CKTDGUIStatic*)m_pDLGBackGround->GetControl( L"Choice_Item_Black" );
	pStatic_BackGround->SetColor( D3DXCOLOR(1, 1, 1, 0.7f ) );	

	m_pDLGRuleAggrement = new CKTDGUIDialog( this, L"DLG_HACKSHIELD_RULE_AGREEMENT.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRuleAggrement );

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*)m_pDLGRuleAggrement->GetControl(L"ListBox_Rule");

#ifdef CLIENT_COUNTRY_TW
	const WCHAR pFileName[256] = L"TermsOfAgreement_TW.lua";		// X2/dat 폴더에 있는 default script
#elif CLIENT_COUNTRY_HK
	const WCHAR pFileName[256] = L"TermsOfAgreement_HK.lua";		// X2/dat 폴더에 있는 default script
#else
	const WCHAR pFileName[256] = L"TermsOfAgreement.lua";			// X2/dat 폴더에 있는 default script
#endif

	KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;

	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		return;
	}
	if( kLuamanager.DoMemory( Info->pRealData, Info->size ) == false )
	{
		return;
	}

	wstring wstrTermsOfAggrement;
	LUA_GET_VALUE( kLuamanager, "TERMS", wstrTermsOfAggrement, L"" );

	const int MAGIC_TEXT_WIDTH = 565;

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBox->GetString()->fontIndex );
#ifdef CLIENT_COUNTRY_EU	
	CWordLineHandler::LineBreakInX2Main( wstrTermsOfAggrement, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true, false );
#endif //CLIENT_COUNTRY_EU
	typedef boost::tokenizer<boost::char_separator<wchar_t,std::wstring::traits_type>,std::wstring::const_iterator,std::wstring > TOKENIZER;

	boost::char_separator<wchar_t,std::wstring::traits_type> sep(L"\n", L"", boost::keep_empty_tokens);
	TOKENIZER tokens(wstrTermsOfAggrement, sep);
	for (TOKENIZER::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter )
	{
		pListBox->AddItem( (*tok_iter).c_str(), NULL );
	}
}
#ifdef NEW_RULE_AGREEMENT
void CX2StateLogin::OpenNewRuleAgreementDlg()
{
	SAFE_DELETE_DIALOG(m_pDLGNewRuleAggrement);
	SAFE_DELETE_DIALOG(m_pDLGNewBackGround);

	m_pDLGNewBackGround = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Unit_State_RandomItem_BG.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGNewBackGround );
	m_pDLGNewBackGround->SetCloseCustomUIEventID(-1);
	CKTDGUIStatic* pStatic_NewBackGround = (CKTDGUIStatic*)m_pDLGNewBackGround->GetControl( L"Choice_Item_Black" );
	pStatic_NewBackGround->SetColor( D3DXCOLOR(1, 1, 1, 0.7f ) );	

	m_pDLGNewRuleAggrement = new CKTDGUIDialog( this, L"DLG_NEW_HACKSHIELD_RULE_AGREEMENT.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGNewRuleAggrement );

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*)m_pDLGNewRuleAggrement->GetControl(L"ListBox_Rule");

	const WCHAR pFileName[256] = L"TermsandConditions.lua";			// 임홍락 2011.5.30 X2/dat 폴더에 새로 추가된 script

	KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;

	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		return;
	}
	if( kLuamanager.DoMemory( Info->pRealData, Info->size ) == false )
	{
		return;
	}

	wstring wstrTermsOfAggrement;
	LUA_GET_VALUE( kLuamanager, "TERMS", wstrTermsOfAggrement, L"" );

	const int MAGIC_TEXT_WIDTH = 565;

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBox->GetString()->fontIndex );
#ifdef CLIENT_COUNTRY_EU
	CWordLineHandler::LineBreakInX2Main( wstrTermsOfAggrement, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true, false );
#endif //CLIENT_COUNTRY_EU
	typedef boost::tokenizer<boost::char_separator<wchar_t,std::wstring::traits_type>,std::wstring::const_iterator,std::wstring > TOKENIZER;

	boost::char_separator<wchar_t,std::wstring::traits_type> sep(L"\n", L"", boost::keep_empty_tokens);
	TOKENIZER tokens(wstrTermsOfAggrement, sep);
	for (TOKENIZER::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter )
	{
		pListBox->AddItem( (*tok_iter).c_str(), NULL );
	}
}
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT

#ifdef SERV_PUBLISHER_OTP
bool CX2StateLogin::Handler_SLUCM_OTP_ON( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef ALWAYS_PUBLISTHER_OPT_TH
	CKTDGUIEditBox* pIDEditBox = (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"IMELoginID" );
	if( pIDEditBox != NULL)
		pIDEditBox->SetPrevTabControlName( L"EditBoxLoginOTP" );

	CKTDGUIEditBox* pPasswordEditBox = (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"EditBoxLoginPassword" );
	if( pPasswordEditBox != NULL)
		pPasswordEditBox->SetTabControlName( L"EditBoxLoginOTP" );
#ifdef LOGIN_KEYBOARD_SECURITY
	if(m_bKeyBoardUse == true)
	{
		if(m_pDLGKeyboard != NULL)
		{
			CKTDGUIStatic* pStaticKeyboard = (CKTDGUIStatic*)m_pDLGKeyboard->GetControl(L"g_pStaticKeyboard");	
			ASSERT(pStaticKeyboard);
			CKTDGUIControl::CPictureData* pPicture = (CKTDGUIControl::CPictureData*)pStaticKeyboard->GetPictureIndex(1);
			ASSERT(pPicture);
			pPicture->SetShow(false);

			CKTDGUIEditBox* pOTPEditBox	= (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"EditBoxLoginOTP" );
			pOTPEditBox->ClearText();
			pOTPEditBox->SetEnable(true);
		}
	}
	else
#else //LOGIN_KEYBOARD_SECURITY
	{
		/*
		CKTDGUIStatic* pStaticLogin = (CKTDGUIStatic*)m_pDLGLoginBack->GetControl(L"StaticLogin");
		ASSERT(pStaticLogin);
		CKTDGUIControl::CPictureData* pPicture = (CKTDGUIControl::CPictureData*)pStaticLogin->GetPictureIndex(1);
		ASSERT(pPicture);
		pPicture->SetShow(false);

		CKTDGUIEditBox* pOTPEditBox	= (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"EditBoxLoginOTP" );
		pOTPEditBox->ClearText();
		pOTPEditBox->SetText(L"");
		pOTPEditBox->SetEnable(true);
		*/
	}
#endif LOGIN_KEYBOARD_SECURITY
#else
	if(m_bKeyBoardUse == true)
	{
		if(m_pDLGKeyboard != NULL)
		{
			CKTDGUIEditBox* pIDEditBox = (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"IMELoginID" );
			if( pIDEditBox != NULL)
				pIDEditBox->SetPrevTabControlName( L"EditBoxLoginOTP" );

			CKTDGUIEditBox* pPasswordEditBox = (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"EditBoxLoginPassword" );
			if( pPasswordEditBox != NULL)
				pPasswordEditBox->SetTabControlName( L"EditBoxLoginOTP" );

			CKTDGUIStatic* pStaticKeyboard = (CKTDGUIStatic*)m_pDLGKeyboard->GetControl(L"g_pStaticKeyboard");	
			ASSERT(pStaticKeyboard);
			CKTDGUIControl::CPictureData* pPicture = (CKTDGUIControl::CPictureData*)pStaticKeyboard->GetPictureIndex(1);
			ASSERT(pPicture);
			pPicture->SetShow(false);

			CKTDGUIEditBox* pOTPEditBox	= (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"EditBoxLoginOTP" );
			pOTPEditBox->ClearText();
			pOTPEditBox->SetEnable(true);
		}
	}
	else
	{
		CKTDGUIEditBox* pIDEditBox = (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"IMELoginID" );
		if( pIDEditBox != NULL)
			pIDEditBox->SetPrevTabControlName( L"EditBoxLoginOTP" );

		CKTDGUIEditBox* pPasswordEditBox = (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"EditBoxLoginPassword" );
		if( pPasswordEditBox != NULL)
			pPasswordEditBox->SetTabControlName( L"EditBoxLoginOTP" );

		CKTDGUIStatic* pStaticLogin = (CKTDGUIStatic*)m_pDLGLoginBack->GetControl(L"StaticLogin");
		ASSERT(pStaticLogin);
		CKTDGUIControl::CPictureData* pPicture = (CKTDGUIControl::CPictureData*)pStaticLogin->GetPictureIndex(1);
		ASSERT(pPicture);
		pPicture->SetShow(false);

		CKTDGUIEditBox* pOTPEditBox	= (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"EditBoxLoginOTP" );
		pOTPEditBox->ClearText();
		pOTPEditBox->SetEnable(true);
	}
#endif ALWAYS_PUBLISTHER_OPT_TH

	return false;	
}

bool CX2StateLogin::Handler_SLUCM_OTP_OFF( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef ALWAYS_PUBLISTHER_OPT_TH
#else
	CKTDGUIEditBox* pIDEditBox = (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"IMELoginID" );
	if( pIDEditBox != NULL)
		pIDEditBox->SetPrevTabControlName( L"EditBoxLoginPassword" );

	CKTDGUIEditBox* pPasswordEditBox = (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"EditBoxLoginPassword" );
	if( pPasswordEditBox != NULL)
		pPasswordEditBox->SetTabControlName( L"IMELoginID" );

	if(m_bKeyBoardUse == false)
	{
		
		CKTDGUIStatic* pStaticLogin = (CKTDGUIStatic*)m_pDLGLoginBack->GetControl(L"StaticLogin");
		ASSERT(pStaticLogin);
		CKTDGUIControl::CPictureData* pPicture = (CKTDGUIControl::CPictureData*)pStaticLogin->GetPictureIndex(1);
		ASSERT(pPicture);
		pPicture->SetShow(true);

		CKTDGUIEditBox* pOTPEditBox	= (CKTDGUIEditBox*)m_pDLGLoginBack->GetControl( L"EditBoxLoginOTP" );
		pOTPEditBox->ClearText();
		pOTPEditBox->SetEnable(false);
	}
	else
	{
		if(m_pDLGKeyboard != NULL)
		{
			CKTDGUIStatic* pStaticKeyboard = (CKTDGUIStatic*)m_pDLGKeyboard->GetControl(L"g_pStaticKeyboard");
			ASSERT(pStaticKeyboard);
			CKTDGUIControl::CPictureData* pPicture = (CKTDGUIControl::CPictureData*)pStaticKeyboard->GetPictureIndex(1);
			ASSERT(pPicture);
			pPicture->SetShow(true);

			CKTDGUIEditBox* pOTPEditBox	= (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"EditBoxLoginOTP" );
			pOTPEditBox->ClearText();
			pOTPEditBox->SetEnable(false);

		}
		
	}
#endif ALWAYS_PUBLISTHER_OPT_TH	

	return false;
}
#endif SERV_PUBLISHER_OTP

#ifdef SERV_PUBLISHER_MATRIXKEY
void CX2StateLogin::CreateMatrixPopup()
{
	// 매트릭스 카드 윈도우
	SAFE_DELETE_DIALOG( m_pDLGMatrix );
	m_pDLGMatrix = new CKTDGUIDialog( this, L"DLG_Login_Matrix.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMatrix );
	m_pDLGMatrix->SetDisableUnderWindow(true);

	m_vCenter = D3DXVECTOR2(330/2, 330/2);
	m_fRadius = 330/2;
	m_fButtonSize = 30;
	m_fRotation = RandomFloat(0.0f, 2*D3DX_PI);

	boost::wformat fmtButtonName(L"%1%");

	for(int i=0; i<10; ++i)
	{
		CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(m_pDLGMatrix->GetControl(boost::str(fmtButtonName % i).c_str()));
		ASSERT(pButton);
		if(pButton)
		{
			m_pNumButton[i] = pButton;
		}
	}

	m_bRotate = true;
}

void CX2StateLogin::ResetMatrix(const std::wstring& wstrCoordinate)
{
	if(m_pDLGMatrix == NULL)
		return;

	// 랜덤 회전
	m_fRotation = RandomFloat(0.0f, 2*D3DX_PI);

	// 좌표값 생성
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGMatrix->GetControl( L"StaticCoordinate" );
	ASSERT(pStatic);
	if(pStatic == NULL)
		return;

	pStatic->RemoveAllPicture();

	float x = (wstrCoordinate.size() > 2) ? 162 : 166;
	float y = 107;

	boost::wformat fmtCoordName(L"COORD_%1%");

	for(int i=0; i<wstrCoordinate.size(); ++i)
	{
		CKTDGUIStatic::CPictureData* pPicture = new CKTDGUIStatic::CPictureData();
		std::wstring wstrCoord = wstrCoordinate.substr(i,1);
		pPicture->SetTex(L"DLG_Matrix.tga", boost::str(fmtCoordName % wstrCoord).c_str());

		CKTDGUIControl::UIPointData* pPoint = new CKTDGUIControl::UIPointData();
		pPoint->fChangeTime			= 0.0f;
		pPoint->bUseTextureSize		= true;
		pPoint->leftTopPoint		= D3DXVECTOR2(x, y);
		pPoint->rightTopPoint		= D3DXVECTOR2(x, y);
		pPoint->leftBottomPoint		= D3DXVECTOR2(x, y);
		pPoint->rightBottomPoint	= D3DXVECTOR2(x, y);
		pPoint->color				= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );	
		pPicture->SetPoint( pPoint );

		pStatic->AddPicture(pPicture);

		x += (i < 1) ? 20 : 9;
	}

	CKTDGUIIMEEditBox* pIMEMatrix = (CKTDGUIIMEEditBox*)m_pDLGMatrix->GetControl(L"IMEMatrix");
	ASSERT(pIMEMatrix);
	if(pIMEMatrix)
	{
		return pIMEMatrix->SetText(L"");
	}
}

LPCWSTR CX2StateLogin::GetMatrixString()
{
	if(m_pDLGMatrix != NULL)
	{
		CKTDGUIIMEEditBox* pIMEMatrix = (CKTDGUIIMEEditBox*)m_pDLGMatrix->GetControl(L"IMEMatrix");
		ASSERT(pIMEMatrix);
		if(pIMEMatrix)
		{
			return pIMEMatrix->GetText();
		}
	}

	return NULL;
}

void CX2StateLogin::FrameMoveMatrixPopup( float fElapsedTime )
{
	if(m_pDLGMatrix == NULL)
		return;

	const static int nButton = 10;
	const static D3DXVECTOR2 vUp(0.0f, 1.0f);
	const static float fSpeed = 0.1f;

	// 회전각 계산.
	if(m_bRotate)
	{
		m_fRotation = m_fRotation + (fSpeed*fElapsedTime) * 2*D3DX_PI;
		if(m_fRotation > 2*D3DX_PI)
			m_fRotation -= 2*D3DX_PI;
	}

	for(int i=0; i<10; ++i)
	{
		if(m_pNumButton[i] != NULL)
		{
			D3DXMATRIX matRotate;
			D3DXMatrixRotationZ(&matRotate, i*2*D3DX_PI/nButton + m_fRotation);

			D3DXVECTOR2 vDir;
			D3DXVec2TransformCoord(&vDir, &vUp, &matRotate);
			D3DXVec2Normalize(&vDir, &vDir);
			D3DXVECTOR2 vPos = m_vCenter + vDir*m_fRadius - D3DXVECTOR2(m_fButtonSize, m_fButtonSize);

			m_pNumButton[i]->SetOffsetPos(vPos);
		}
	}

	if(m_bDeleteMatrixDialog)
	{
		SAFE_DELETE_DIALOG( m_pDLGMatrix );
		m_bDeleteMatrixDialog = false;
	}
}

bool CX2StateLogin::Handler_SLUCM_MATRIX_OK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_pDLGMatrix == NULL)
		return false;

	if(m_wstrMatrixCoordinate.empty() || m_wstrMatrixCoordinate.length() != 6)
	{
		if( g_pData->GetServerProtocol() != NULL )
			g_pData->GetServerProtocol()->DisconnectFromGameServer();
		m_bDeleteMatrixDialog = true;
		return true;
	}

	LPCWSTR pMatrixString = GetMatrixString();
	if(pMatrixString != NULL)
        m_wstrMatrix.append(pMatrixString);

	if(m_nInput >= 2)
	{
		m_nInput = 0;

		m_bDeleteMatrixDialog = true;
		Handler_ECH_SECURITY_AUTH_REQ(hWnd, uMsg, wParam, lParam);
		m_wstrMatrix.clear();
	}
	else
	{
		++m_nInput;
		ResetMatrix(m_wstrMatrixCoordinate.substr(m_nInput*2, 2));
	}

	return true;
}

bool CX2StateLogin::Handler_SLUCM_MATRIX_NUMBER( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(lParam);
	ASSERT(pButton);
	CKTDGUIIMEEditBox* pIMEMatrix = (CKTDGUIIMEEditBox*)m_pDLGMatrix->GetControl(L"IMEMatrix");
	ASSERT(pIMEMatrix);
	
	if(pButton)
	{
		std::wstring wstrMatrix = GetMatrixString();
		
		wstrMatrix += pButton->GetName();

		if(!wstrMatrix.empty() && wstrMatrix.size() > 2)
		{
			wstrMatrix.erase( wstrMatrix.begin() );
		}
		pIMEMatrix->SetText(wstrMatrix.c_str());
	}

	return true;
}

bool CX2StateLogin::Handler_SLUCM_MATRIX_CLEAR( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CKTDGUIIMEEditBox* pIMEMatrix = (CKTDGUIIMEEditBox*)m_pDLGMatrix->GetControl(L"IMEMatrix");
	ASSERT(pIMEMatrix);
	if(pIMEMatrix)
	{
		pIMEMatrix->SetText(L"");
	}

	return true;
}

bool CX2StateLogin::HandleMsgByESCImp()
{
	if (m_pDLGMatrix != NULL)
	{
		SAFE_DELETE_DIALOG( m_pDLGMatrix );
		
		if( g_pData->GetServerProtocol() != NULL )
			g_pData->GetServerProtocol()->DisconnectFromChannelServer();

		return true;
	}
	else
		return __super::HandleMsgByESCImp();
}
#endif SERV_PUBLISHER_MATRIXKEY

#ifdef SERV_GLOBAL_AUTH
bool CX2StateLogin::Handler_ECH_SECURITY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 인증을 기다리지 않도록 처리
	g_pMain->DeleteServerPacket( ECH_VERIFY_ACCOUNT_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KECH_SECURITY_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

#ifdef SERV_PUBLISHER_MATRIXKEY
	m_wstrMatrixCoordinate = kEvent.m_wstrCoordinate;
	if(!m_wstrMatrixCoordinate.empty() && m_wstrMatrixCoordinate.length() == 6)
	{
		CreateMatrixPopup();
		ResetMatrix(m_wstrMatrixCoordinate.substr(0, 2));
	}
#endif SERV_PUBLISHER_MATRIXKEY

	return true;
}

bool CX2StateLogin::Handler_ECH_SECURITY_AUTH_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KECH_SECURITY_AUTH_REQ kPacket;
#if defined( SERV_PUBLISHER_MATRIXKEY )
	kPacket.m_iSecurityAuthType = KECH_SECURITY_AUTH_REQ::SAT_MATRIX_CARD;
	kPacket.m_wstrMatrix = m_wstrMatrix;
	//#elif defined( 미국 ) // 미국은 SECURITY_AUTH 여기서 사용안함. 2차 보안패드 설정할때 사용함
#endif 

	kPacket.m_wstrUserID = g_pInstanceData->GetUserID();

	g_pData->GetServerProtocol()->SendChPacket( ECH_SECURITY_AUTH_REQ, kPacket );
	g_pMain->AddServerPacket( ECH_VERIFY_ACCOUNT_ACK );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_647 ), this);

	return true;
}
#endif SERV_GLOBAL_AUTH

#ifdef SAVE_LOGIN_ID
bool CX2StateLogin::SaveID()
{
	CKTDGUIIMEEditBox* pIDEditBox = NULL;
	wstring wstrID;

#ifdef LOGIN_KEYBOARD_SECURITY
	if(m_pDLGKeyboard != NULL && m_pDLGKeyboard->GetShow() == true)
	{
		pIDEditBox = (CKTDGUIIMEEditBox*)m_pDLGKeyboard->GetControl( L"IMELoginID" );
		if(pIDEditBox == NULL)
			return false;
	}
	else
#endif LOGIN_KEYBOARD_SECURITY
	{
		pIDEditBox = (CKTDGUIIMEEditBox*)m_pDLGLoginBack->GetControl( L"IMELoginID" );
		if(pIDEditBox == NULL)
			return false;
	}

	wstrID = pIDEditBox->GetText();
	if(wstrID.empty())
		return false;

#ifdef PLAYER_ID_IN_GAME_OPTION
	g_pMain->GetGameOption().GetOptionList().m_wstrSavedLoginID = wstrID;
#else // PLAYER_ID_IN_GAME_OPTION

	const wstring strFileName = L"PlayerID.lua";

	FILE* file = NULL;
	file = _wfopen( strFileName.c_str(), L"w" );		
	if( NULL == file )
	{		
		return false;
	}

	// BOM
	fputc( 0xEF, file );
	fputc( 0xBB, file );
	fputc( 0xBF, file );

	fwprintf( file, L"\nLOGIN_ID = \"%s\" \n", wstrID.c_str() );
#ifdef	USE_ACCOUNT_DOMAIN
	fwprintf( file, L"\nACCOUNT_TYPE = %d\n", g_pInstanceData->GetDomain() );
#endif	USE_ACCOUNT_DOMAIN

	fclose(file);
#endif // PLAYER_ID_IN_GAME_OPTION
	return true;
}

bool CX2StateLogin::LoadID()
{
#ifdef PLAYER_ID_IN_GAME_OPTION
	if( L"" != g_pMain->GetGameOption().GetOptionList().m_wstrSavedLoginID && m_pDLGLoginBack != NULL)
	{
		CKTDGUIIMEEditBox* pIDEditBox = (CKTDGUIIMEEditBox*)m_pDLGLoginBack->GetControl( L"IMELoginID" );
		if(pIDEditBox == NULL)
			return false;
		pIDEditBox->SetText( g_pMain->GetGameOption().GetOptionList().m_wstrSavedLoginID.c_str() );

		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGLoginBack->GetControl( L"CheckBoxRememberID" );
		if(pCheckBox != NULL)
		{
			pCheckBox->SetChecked( true );
		}

		return true;
	}
#else // PLAYER_ID_IN_GAME_OPTION
	const WCHAR pFileName[256] = L"PlayerID.lua";			// X2/dat 폴더에 있는 default script
	// gameoptions.lua 파일을 읽어서 UTF-8 포맷이 아니면 변환한다
	ConvertFileAnsiToUTF8( "GameOptions.lua", "GameOptions.lua" );

	KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;

	bool bDoneParsingUserGameOptionFile = true;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info != NULL )
	{
		if( false == g_pKTDXApp->LoadLuaTinker( pFileName, false ) )
		{
			bDoneParsingUserGameOptionFile = false;
		}

		if( true == bDoneParsingUserGameOptionFile )
		{
			if( false == g_pKTDXApp->LoadAndDoMemory( &kLuamanager, pFileName ) )
			{
				bDoneParsingUserGameOptionFile = false;
			}
		}
	}
	else
	{
		bDoneParsingUserGameOptionFile = false;
	}

	if( false == bDoneParsingUserGameOptionFile )
	{
		if( false == g_pKTDXApp->LoadLuaTinker( pFileName ) )
		{
			return false;
		}

		if( false == g_pKTDXApp->LoadAndDoMemory( &kLuamanager, pFileName  ) )
		{
			return false;
		}
	}

	wstring wstrID;
	LUA_GET_VALUE( kLuamanager, "LOGIN_ID", wstrID, L"" );

	if( !wstrID.empty() && m_pDLGLoginBack != NULL)
	{
		CKTDGUIIMEEditBox* pIDEditBox = (CKTDGUIIMEEditBox*)m_pDLGLoginBack->GetControl( L"IMELoginID" );
		if(pIDEditBox == NULL)
			return false;
		pIDEditBox->SetText( wstrID.c_str() );

		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGLoginBack->GetControl( L"CheckBoxRememberID" );
		if(pCheckBox != NULL)
		{
			pCheckBox->SetChecked( true );
		}

#ifdef	USE_ACCOUNT_DOMAIN
		int iDomainType;

		LUA_GET_VALUE( kLuamanager, "ACCOUNT_TYPE", iDomainType, 0 );

		if( iDomainType == 0 )
		{
			CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*)m_pDLGLoginBack->GetControl( L"Radio_AccountType_PlayID" );
			if ( pRadio != NULL )
			{
				pRadio->SetChecked(true);
			}
		}
		else if( iDomainType == 1 )
		{
			CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*)m_pDLGLoginBack->GetControl( L"Radio_AccountType_TCG" );
			if ( pRadio != NULL )
			{
				pRadio->SetChecked(true);
			}
		}

		g_pInstanceData->SetDomain( static_cast<ACCOUNT_DOMAIN_TYPE>(iDomainType) );
#endif	USE_ACCOUNT_DOMAIN

		return true;
	}
	return false;
#endif // PLAYER_ID_IN_GAME_OPTION

}

bool CX2StateLogin::DeleteSavedID()
{
	DeleteFileW( L"PlayerID.lua" );
	return true;
}
#endif SAVE_LOGIN_ID

#ifdef LOGIN_KEYBOARD_SECURITY
CKTDGUIControl* CX2StateLogin::GetLastFocusedControl()
{
	if(m_pDLGKeyboard == NULL)
		return NULL;

	if( m_pLastFocusedControl == NULL )
		m_pLastFocusedControl = m_pDLGKeyboard->GetControl( L"IMELoginID" );

	return m_pLastFocusedControl;
}

void CX2StateLogin::AppendKeyValue(const std::wstring& wstrValue)
{
	CKTDGUIControl* pControl = GetLastFocusedControl();

	if( pControl )
	{
		if( std::wstring( L"IMELoginID" ) == pControl->GetName() )
		{
			CKTDGUIIMEEditBox* pEditBox = static_cast<CKTDGUIIMEEditBox*>( pControl );
			std::wstring wstrText = pEditBox->GetText() + wstrValue;
			pEditBox->SetText( wstrText.c_str() );
		}
		else
		{
			CKTDGUIEditBox* pEditBox = static_cast<CKTDGUIEditBox*>( pControl );
			std::wstring wstrText = pEditBox->GetText() + wstrValue;
			pEditBox->SetText( wstrText.c_str() );
		}
		pControl->RequestFocus();
	}
}

bool CX2StateLogin::Handler_SLUCM_KEYBOARD( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL == m_pDLGKeyboard )
	{
		m_pDLGKeyboard = new CKTDGUIDialog( this, L"DLG_Login_Keyboard.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGKeyboard );
		m_pDLGKeyboard->SetShowEnable(false, false);
	}
	
	if( NULL != m_pDLGKeyboard )
	{
		CKTDGUIIMEEditBox* pIDEditBoxBack = (CKTDGUIIMEEditBox*)m_pDLGLoginBack->GetControl( L"IMELoginID" );
		CKTDGUIIMEEditBox* pIDEditBoxFront = (CKTDGUIIMEEditBox*)m_pDLGKeyboard->GetControl( L"IMELoginID" );
		if(pIDEditBoxBack != NULL && pIDEditBoxFront != NULL)
		{
			pIDEditBoxFront->ClearText();
			pIDEditBoxFront->SetText( pIDEditBoxBack->GetText() );
			//pIDEditBoxFront->SetEnableInput(false);
		}

		CKTDGUICheckBox* pCheckBoxBack = (CKTDGUICheckBox*)m_pDLGLoginBack->GetControl( L"CheckBoxRememberID" );
		CKTDGUICheckBox* pCheckBoxFront = (CKTDGUICheckBox*)m_pDLGKeyboard->GetControl( L"CheckBoxRememberID" );
		if(pCheckBoxBack != NULL && pCheckBoxFront != NULL)
		{
			pCheckBoxFront->SetChecked( pCheckBoxBack->GetChecked() );
		}

		m_pDLGKeyboard->SetShowEnable(true, true);
		m_pDLGKeyboard->SetDisableUnderWindow(true);

		m_pDLGLoginBack->SetShowEnable(false, false);
		m_pDLGLoginBack->SetDisableUnderWindow(false);

		CKTDGUICheckBox* pOTPCheckBox = (CKTDGUICheckBox*)m_pDLGKeyboard->GetControl( L"CheckBoxOTP" );
		if(pOTPCheckBox != NULL)
		{
			pOTPCheckBox->SetChecked(true);
		}

		CKTDGUIEditBox* pOTPEditBox	= (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"EditBoxLoginOTP" );
		if(pOTPEditBox != NULL)
		{
			pOTPEditBox->ClearText();
			//pOTPEditBox->SetEnable(true);
		}

		m_bKeyBoardUse = true;

#ifdef SERV_PUBLISHER_OTP
		Handler_SLUCM_OTP_ON( NULL, 0, NULL, NULL );
#endif SERV_PUBLISHER_OTP

		return true;
	}

	return false;
}

bool CX2StateLogin::Handler_SLUCM_KEYBOARD_CLOSE( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL != m_pDLGLoginBack )
	{
		m_pDLGLoginBack->SetShowEnable(true, true);
		m_pDLGLoginBack->SetDisableUnderWindow(true);
	}

	if( NULL != m_pDLGKeyboard )
	{
		CKTDGUIIMEEditBox* pIDEditBox = (CKTDGUIIMEEditBox*)m_pDLGKeyboard->GetControl( L"IMELoginID" );
		if(pIDEditBox != NULL)
		{
			pIDEditBox->ClearText();
		}
		CKTDGUIEditBox* pPasswordEditBox = (CKTDGUIEditBox*)m_pDLGKeyboard->GetControl( L"EditBoxLoginPassword" );
		if(pPasswordEditBox != NULL)
		{
			pPasswordEditBox->ClearText();
		}
		m_bKeyboardShift = false;

		m_pDLGKeyboard->SetShowEnable(false, false);
		m_pDLGKeyboard->SetDisableUnderWindow(false);

		m_bKeyBoardUse = false;

		return true;
	}

	return false;
}

bool CX2StateLogin::Handler_SLUCM_KEYBOARD_KEY( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(lParam);
	if(m_pDLGKeyboard == NULL || pButton == NULL)
		return false;

	std::wstring strName = pButton->GetName();

	if(m_bKeyboardShift)
		AppendKeyValue(m_mapShiftedKeyValue[strName]);
	else
		AppendKeyValue(m_mapNormalKeyValue[strName]);

	return true;
}

bool CX2StateLogin::Handler_SLUCM_KEYBOARD_ENTER( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_pDLGKeyboard == NULL)
		return false;

	CKTDGUIControl* pEditBox = GetLastFocusedControl();
	if( pEditBox && std::wstring( L"EditBoxLoginPassword" ) == pEditBox->GetName() == 0 )
	{
		UICustomEventProc( hWnd, CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, SLUCM_LOGIN, reinterpret_cast<LPARAM>(pEditBox) );
	}

	return true;
}

bool CX2StateLogin::Handler_SLUCM_KEYBOARD_SHIFT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_pDLGKeyboard == NULL)
		return false;

	boost::wformat fmtNormalTETName(L"KEY_%1%");
	boost::wformat fmtOverTETName(L"KEY_OVER_%1%");
	std::wstring strNormalTETName, strOverTETName;

	CKTDGUICheckBox* pCheckBoxShift = (CKTDGUICheckBox*)m_pDLGKeyboard->GetControl( L"KEY_SHIFT" );
	m_bKeyboardShift = pCheckBoxShift->GetChecked();

	BOOST_FOREACH( std::wstring strKey, m_vecKeyList )
	{
		strNormalTETName = boost::str(fmtNormalTETName % strKey);
		strOverTETName = boost::str(fmtOverTETName % strKey);

		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGKeyboard->GetControl( strNormalTETName.c_str() );
		if(m_bKeyboardShift)
		{
			pButton->SetNormalTex( L"DLG_Login4_B.tga", strNormalTETName.c_str() );
			pButton->SetOverTex( L"DLG_Login4_C.tga", strOverTETName.c_str() );
			pButton->SetDownTex( L"DLG_Login4_C.tga", strOverTETName.c_str() );
		}
		else
		{
			pButton->SetNormalTex( L"DLG_Login4.tga", strNormalTETName.c_str() );
			pButton->SetOverTex( L"DLG_Login4_A.tga", strOverTETName.c_str() );
			pButton->SetDownTex( L"DLG_Login4_A.tga", strOverTETName.c_str() );
		}
	}

	return true;
}

bool CX2StateLogin::Handler_SLUCM_KEYBOARD_BACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CKTDGUIControl* pControl = GetLastFocusedControl();
	if( pControl )
	{
		std::wstring wstrText;
		if( std::wstring( L"IMELoginID" ) == pControl->GetName() )
		{
			CKTDGUIIMEEditBox* pEditBox = static_cast<CKTDGUIIMEEditBox*>( pControl );
			wstrText = pEditBox->GetText();
			if( !wstrText.empty() )
				pEditBox->ProcessBackspace();
		}
		else
		{
			CKTDGUIEditBox* pEditBox = static_cast<CKTDGUIEditBox*>( pControl );
			wstrText = pEditBox->GetText();
			if( !wstrText.empty() )
				pEditBox->ProcessBackspace();
		}
	}

	return true;
}

bool CX2StateLogin::Handler_SLUCM_KEYBOARD_EDIT_FOCUSOUT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	m_pLastFocusedControl = reinterpret_cast<CKTDGUIControl*>(lParam);

	return true;
}
#endif LOGIN_KEYBOARD_SECURITY

//{{ 2013.03.13 조효진	회원가입 버튼을 누르면 웹페이지를 띄워줌
#ifdef MEMBER_JOIN_FROM_WEB
std::wstring CX2StateLogin::GetStrMemberJoinWeb()
{
	std::wstring wstrURL = L"";

#ifdef _OPEN_TEST_
	// 테스트 서버
	switch(g_pMain->GetNationFlag())
	{
		case CX2Main::NF_DE:	wstrURL = L"http://de.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_FR:	wstrURL = L"http://fr.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_IT:	wstrURL = L"http://it.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_PL:	wstrURL = L"http://pl.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_ES:	wstrURL = L"http://es.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_EN:	wstrURL = L"http://en.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_TW:	wstrURL = L"http://www.elsword.com.tw/members/apply.aspx";	break;
		case CX2Main::NF_HK:	wstrURL = L"http://www.elsonline.com.hk/v1_03a.asp";		break;
	}
#else _OPEN_TEST_ 
	// 본 서버
	switch(g_pMain->GetNationFlag())
	{
		case CX2Main::NF_DE:	wstrURL = L"http://de.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_FR:	wstrURL = L"http://fr.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_IT:	wstrURL = L"http://it.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_PL:	wstrURL = L"http://pl.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_ES:	wstrURL = L"http://es.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_EN:	wstrURL = L"http://en.elsword.gameforge.com/clientregister";		break;
		case CX2Main::NF_TW:	wstrURL = L"http://www.elsword.com.tw/members/apply.aspx";	break;
		case CX2Main::NF_HK:	wstrURL = L"http://www.elsonline.com.hk/v1_03a.asp";		break;
	}
#endif _OPEN_TEST_

	return wstrURL;
}
#endif MEMBER_JOIN_FROM_WEB
//}}
#ifdef USE_ACCOUNT_DOMAIN
bool CX2StateLogin::Handler_SLUCM_DOMAIN_TYPE_PLAYID( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*)lParam;

	if( pRadio == NULL )
		return true;
	
	g_pInstanceData->SetDomain( ADT_PLAYID );

	return true;
}

bool CX2StateLogin::Handler_SLUCM_DOMAIN_TYPE_TCG( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*)lParam;

	if( pRadio == NULL )
		return true;

	g_pInstanceData->SetDomain( ADT_TCG );

	return true;
}
#endif USE_ACCOUNT_DOMAIN